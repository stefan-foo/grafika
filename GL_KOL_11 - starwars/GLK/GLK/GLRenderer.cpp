#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#define _USE_MATH_DEFINES
#include <math.h>

CGLRenderer::CGLRenderer(void)
{
	m_alpha = m_beta = 0;
	m_dist = 10;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);

	GLfloat amb[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat dif[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dif);

	m_tex = LoadTexture("./Kol2011.bmp");
	//glEnable(GL_LIGHTING);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

UINT CGLRenderer::LoadTexture(char* path) {
	UINT texId;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	DImage img;
	img.Load(CString(path));

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	glBindTexture(GL_TEXTURE_2D, 0);
	return texId;
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// KADA SE SVETLO OVDE POSTAVI ONO SE KRECE SA POSMATRACEM

	glTranslated(0, 0, -m_dist);
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);

	// KADA JE OVDE STAVLJENO SVETLO JE STATICKI POZOCIONIRNO
	float pos[] = { 10, 5, 20, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	DrawAxis(10);

	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_tex);
	//glBegin(GL_QUADS);
	//glTexCoord2d(0, 0);
	//glVertex3d(0, 0, 0);
	//glTexCoord2d(1, 0);
	//glVertex3d(1, 0, 0);
	//glTexCoord2d(1, 1);
	//glVertex3d(1, 1, 0);
	//glTexCoord2d(0, 1); 
	//glVertex3d(0, 1, 0);
	//glEnd();

	DrawCompoundBody();
	DrawWings();
	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawBody(double r, double h1, double h2, int n) {
	double a = 0, ainc = 2 * M_PI / n;

	double ph = (double)(h1 - h2) / 2;

	glPushMatrix();
	glTranslated(0, -h1 / 2, 0);

	double s = 0.25;
	double t = 0.5;

	//double si = 0.25 / n;
	double ti = 0.5 / n;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; i++) {		
		glNormal3d(cos(a), 0, sin(a));

		glTexCoord2d(0.5, t);
		glVertex3d(r * cos(a), ph, r * sin(a));
		glTexCoord2d(0.25, t);
		glVertex3d(r * cos(a), h1 - ph , r * sin(a));

		a += ainc;
		t += ti;
	}
	glEnd();

	double L = sqrt(ph * ph + r * r);
	double ny = r / L;
	double nr = ph / L;

	a = 0;

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2d(.13, .75);
	glVertex3d(0, h1, 0);

	for (int i = 0; i <= n; i++) {
		glNormal3d(nr * cos(a), ny, nr * sin(a));

		//glTexCoord2d(0.125, 0.75);
		//glVertex3d(0, h1, 0);

		double s = 0.13 + cos(a) * 0.125;
		double t = 0.74 + sin(a) * 0.25;

		glTexCoord2d(s, t);
		glVertex3d(r * cos(a), h1 - ph, r * sin(a));
		a += ainc;
	}

	glEnd();

	//glBegin(GL_TRIANGLE_STRIP);
	//for (int i = 0; i <= n; i++) {
	//	glNormal3d(nr * cos(a), ny, nr * sin(a));

	//	glTexCoord2d(0.125, 0.75);
	//	glVertex3d(0, h1, 0);

	//	double s = 0.125 + cos(a) * 0.125;
	//	double t = 0.75 + sin(a) * 0.25;

	//	glTexCoord2d(s, t);
	//	glVertex3d(r * cos(a), h1 - ph, r * sin(a));
	//	a += ainc;
	//}
	//glEnd();

	a = 0;

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= n; i++) {
		glNormal3d(nr * cos(a), -ny, nr * sin(a));

		glVertex3d(0, 0, 0);
		glVertex3d(r * cos(a), ph, r * sin(a));
		a += ainc;
	}
	glEnd();
	glPopMatrix();
}

void CGLRenderer::DrawCompoundBody() {
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	DrawBody(1.0, 2.5, 2.0, 8);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 0, 0, 1);
	DrawBody(0.5, 3.5, 3.0, 8);
	DrawBody(0.25, 5.0, 5.0, 8);
	glPopMatrix();
}

void CGLRenderer::DrawWings() {
	glPushMatrix();
	glTranslated(2.5, 0, 0);
	glRotated(90, 0, 1, 0);
	DrawWing();
	glPopMatrix();

	glPushMatrix();
	glScalef(-1, 1, 1);
	glTranslated(2.5, 0, 0);
	glRotated(90, 0, 1, 0);
	DrawWing();
	glPopMatrix();
}

void CGLRenderer::DrawWing() {
	glPushMatrix();

	glTranslated(-1, -0.75 / 2, 0);

	glPushMatrix();

	glTranslated(0, 0.75, 0);
	glRotated(-20, 1, 0, 0);
	glTranslated(0, -0.75, 0);

	glNormal3d(0, 0, 1);

	glBegin(GL_TRIANGLE_FAN);

	glVertex3d(0, 2.75, 0);
	glVertex3d(-4, 0.75, 0);
	glVertex3d(2, 0.75, 0);
	glVertex3d(2, 2.75, 0);
	
	glEnd();
	glPopMatrix();

	glBegin(GL_QUADS);

	glNormal3d(0, 0, 1);

	glVertex3d(0, 0, 0);
	glVertex3d(2, 0, 0);
	glVertex3d(2, 0.75, 0);
	glVertex3d(0, 0.75, 0);

	glEnd();

	glPushMatrix();
	glScalef(1, -1, 1);
	glRotated(-20, 1, 0, 0);
	glTranslated(0, -0.75, 0);

	glNormal3d(0, 0, 1);

	glBegin(GL_TRIANGLE_FAN);

	glVertex3d(0, 2.75, 0);
	glVertex3d(-4, 0.75, 0);
	glVertex3d(2, 0.75, 0);
	glVertex3d(2, 2.75, 0);

	glEnd();
	glPopMatrix();

	glPopMatrix();
}

void CGLRenderer::DrawAxis(double width) {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(width, 0, 0);

	glColor3f(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, width);

	glColor3f(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, width, 0);

	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
