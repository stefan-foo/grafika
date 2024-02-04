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
	m_alpha = m_beta = m_moonRot = 0;
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
	glClearColor(1, 1, 1, 1);

	m_moonTex[0] = LoadTexture("./M0.jpg");
	m_moonTex[1] = LoadTexture("./M1.jpg");
	m_moonTex[2] = LoadTexture("./M2.jpg");
	m_moonTex[3] = LoadTexture("./M3.jpg");
	m_moonTex[4] = LoadTexture("./M4.jpg");
	m_moonTex[5] = LoadTexture("./M5.jpg");

	m_spaceTex[0] = LoadTexture("./S0.jpg");
	m_spaceTex[1] = LoadTexture("./S1.jpg");
	m_spaceTex[2] = LoadTexture("./S2.jpg");
	m_spaceTex[3] = LoadTexture("./S3.jpg");
	m_spaceTex[4] = LoadTexture("./S4.jpg");
	m_spaceTex[5] = LoadTexture("./S5.jpg");

	m_earchTex[0] = LoadTexture("./TSC0.jpg");
	m_earchTex[1] = LoadTexture("./TSC1.jpg");
	m_earchTex[2] = LoadTexture("./TSC2.jpg");
	m_earchTex[3] = LoadTexture("./TSC3.jpg");
	m_earchTex[4] = LoadTexture("./TSC4.jpg");
	m_earchTex[5] = LoadTexture("./TSC5.jpg");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis() {
	glBegin(GL_LINES);

	glColor3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(10, 0, 0);

	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 10, 0);

	glColor3d(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 10);

	glEnd();
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	//glTranslated(0, 0, -m_dist);
	glPushMatrix();
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);

	glDisable(GL_DEPTH_TEST);
	DrawSpace(2, 20);

	glPopMatrix();

	glTranslated(0, 0, -m_dist);
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);

	float light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	float pos[] = { 0, .2, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	DrawEarth(3, 20);
	glTranslated(50, 0, 0);
	glRotated(m_moonRot, 0, 0, 1);
	DrawMoon(1, 20);

	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawMoon(double R, int n) {
	DrawSphere(R, n, m_moonTex);
}

void CGLRenderer::DrawEarth(double R, int n) {
	DrawSphere(R, n, m_earchTex);
}

void CGLRenderer::DrawSpace(double R, int n) {
	DrawSphere(R, n, m_spaceTex);
}

void CGLRenderer::DrawSphere(double R, int n, UINT m_earchTex[]) {
	glBindTexture(GL_TEXTURE_2D, m_earchTex[0]);
	DrawPatch(R, n);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, m_earchTex[1]);
	DrawPatch(R, n);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, m_earchTex[2]);
	DrawPatch(R, n);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, m_earchTex[3]);
	DrawPatch(R, n);
	glRotated(90, 0, 1, 0);
	glRotated(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, m_earchTex[4]);
	DrawPatch(R, n);
	glRotated(180, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, m_earchTex[5]);
	DrawPatch(R, n);
}

UINT CGLRenderer::LoadTexture(char* fileName) {
	UINT texId;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	DImage img;
	img.Load(CString(fileName));

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texId;
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawPatch(double R, int n) {
	double chg = 2. / n;

	double s = 0, t = 0;
	double tei = 1. / n;

	double y = 1;
	double x = -1;
	
	glNormal3d(0, 0, 1);
	glColor3d(1, 1, 1);
	for (int i = 0; i < n; i++) {
		s = 0;
		x = -1;
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= n; j++) {
			double a, b;
			ParToPolar(x, y, a, b);

			double xc, yc, zc;
			xc = R * cos(a) * sin(b);
			yc = R * sin(a);
			zc = R * cos(a) * cos(b);
			
			glNormal3f(xc / R, yc / R, zc / R);
			glTexCoord2d(s, t);
			glVertex3d(xc, yc, zc);

			ParToPolar(x, y - chg, a, b);

			xc = R * cos(a) * sin(b);
			yc = R * sin(a);
			zc = R * cos(a) * cos(b);

			glNormal3f(xc / R, yc / R, zc / R);
			glTexCoord2d(s, t + tei);
			glVertex3d(xc, yc, zc);

			x += chg;
			s += tei;
		}
		glEnd();
		y -= chg;
		t += tei;
	}
}

void CGLRenderer::ParToPolar(double x, double y, double& a, double& b) {
	b = atan(x);
	a = atan(y * cos(b));
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(6, m_spaceTex);
	glDeleteTextures(6, m_moonTex);
	glDeleteTextures(6, m_earchTex);


	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
