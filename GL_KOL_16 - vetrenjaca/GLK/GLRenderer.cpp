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
	m_dist = 20;
	m_rot = 0;
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
	glEnable(GL_DEPTH_TEST);

	m_envTex = LoadTexture("./env.png");
	m_brickTex = LoadTexture("./brick.png");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

UINT CGLRenderer::LoadTexture(char* fileName) {
	UINT texId;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	DImage img;
	img.Load(CString(fileName));

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(0, 0, -m_dist);
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);

	DrawCube(100);

	float pos[] = { 0.5, 1, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glTranslated(0, -10, 0);

	DrawTube(3.5, 2.5, 10, 32);
	glTranslated(0, 10, 0);
	DrawCone(3.8, 2, 32);
	
	glTranslated(0, 0, 3.8);
	glRotated(m_rot, 0, 0, 1);
	DrawPaddle(8, 1.5);
	glRotated(-90, 0, 0, 1);
	DrawPaddle(8, 1.5);	
	glRotated(-90, 0, 0, 1);
	DrawPaddle(8, 1.5);
	glRotated(-90, 0, 0, 1);
	DrawPaddle(8, 1.5);

	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawPaddle(double length, double width) {
	double w8 = width / 8;
	double l8 = length / 8;

	glColor3d(1, 1, 1);

	glPushMatrix();

	glTranslated(-w8/2, w8 / 2, 0);

	glBegin(GL_QUADS);
	glVertex3d(0, 0, 0);
	glVertex3d(0, -w8, 0);
	glVertex3d(l8, -w8, 0);
	glVertex3d(l8, 0, 0);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(l8, 0, 0);
	glVertex3d(l8, -w8, 0);
	glVertex3d(2 * l8, -width, 0);
	glVertex3d(length, -width, 0);
	glVertex3d(length, 0, 0);
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawCone(double r, double h, int n) {
	double L = sqrt(r * r + h * h);
	double ny = r / L;
	double nr = h / L;

	double a = 0;
	double ai = 2 * M_PI / n;

	glColor3d(1, 1, 1);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < n; i++) {
		glNormal3d(nr * cos(a), ny, nr * sin(a));
		glVertex3d(0, h, 0);
		glVertex3d(r * cos(a), 0, r * sin(a));

		glNormal3d(nr * cos(a + ai), ny, nr * sin(a+ ai));
		glVertex3d(r * cos(a + ai), 0, r * sin(a + ai));

		a += ai;
	}
	glEnd();
}

void CGLRenderer::DrawTube(double r1, double r2, double h, int n) {
	glBindTexture(GL_TEXTURE_2D, m_brickTex);

	double r = (r1 - r2) / 2;
	double L = sqrt(r * r + h * h);
	double ny = r / L;
	double nr = h / L;

	double a = 0;
	double ai = 2 * M_PI / n;
	double ts = 1. / n;
	double t = 0;

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= n; i++) {
		glNormal3d(nr * cos(a), ny, nr * sin(a));
		glTexCoord2d(t, 1);
		glVertex3d(cos(a) * r2, h, sin(a) * r2);

		glTexCoord2d(t, 0);
		glVertex3d(cos(a) * r1, 0, sin(a) * r1);

		a += ai;
		t += ts;
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / h, 1, 200);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawCube(double a) {
	glBindTexture(GL_TEXTURE_2D, m_envTex);

	double ch = 1. / 3;
	double cw = 1. / 4;
	double a2 = a / 2;

	glColor3d(1, 1, 1);

	glPushMatrix();

	glBegin(GL_QUADS);

	glTexCoord2d(0, 2 * ch);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(cw, 2 * ch);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(cw, ch);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(0, ch);
	glVertex3d(a2, a2, -a2);

	glEnd();

	glRotated(-90, 1, 0, 0);

	glBegin(GL_QUADS);

	glTexCoord2d(cw, 2 * ch);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(2 * cw, 2 * ch);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(2 * cw, ch);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(cw, ch);
	glVertex3d(a2, a2, -a2);

	glEnd();

	glRotated(-90, 1, 0, 0);

	glBegin(GL_QUADS);

	glTexCoord2d(2 * cw, 2 * ch);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(3 * cw, 2 * ch);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(3 * cw, ch);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(2 * cw, ch);
	glVertex3d(a2, a2, -a2);

	glEnd();

	glRotated(-90, 1, 0, 0);

	glBegin(GL_QUADS);

	glTexCoord2d(3 * cw, 2 * ch);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(4 * cw, 2 * ch);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(4 * cw, ch);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(3 * cw, ch);
	glVertex3d(a2, a2, -a2);

	glEnd();

	glRotated(-90, 1, 0, 0);
	glRotated(-90, 0, 1, 0);

	glBegin(GL_QUADS);

	glTexCoord2d(cw, 0);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(cw, ch);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(2 * cw, ch);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(2 * cw, 0);
	glVertex3d(a2, a2, -a2);

	glEnd();

	glRotated(180, 0, 1, 0);

	glBegin(GL_QUADS);

	glTexCoord2d(2 * cw, 3 * ch);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(2 * cw, 2 * ch);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(cw, 2 * ch);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(cw, 3 * ch);
	glVertex3d(a2, a2, -a2);

	glEnd();


	glPopMatrix();
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
