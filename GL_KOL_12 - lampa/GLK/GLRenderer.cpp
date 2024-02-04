#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#define _USE_MATH_DEFINES
#include <math.h>

constexpr double toRad = M_PI / 180;
constexpr double toDeg = 180 / M_PI;

CGLRenderer::CGLRenderer(void)
{
	m_alpha = atan2(2, 3 * sqrt(2)) * toDeg;
	m_beta = atan2(3, 3) * toDeg;
	m_dist = sqrt(22);
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

	m_metal  = LoadTexture(L"./Metal1.bmp");
	m_marble = LoadTexture(L"./Marble.bmp");
	m_fabric = LoadTexture(L"./Fabric1.bmp");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis() {
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(100, 0, 0);

	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 100, 0);

	glColor3d(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 100);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


UINT CGLRenderer::LoadTexture(CString fileName) {
	UINT texId;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	DImage img;
	img.Load(fileName);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	
	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	double x = m_dist * cos(m_alpha * toRad) * cos(m_beta * toRad);
	double y = m_dist * sin(m_alpha * toRad);
	double z = m_dist * cos(m_alpha * toRad) * sin(m_beta * toRad);

	gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);

	//glTranslated(0, 0, -10);
	//glRotated(m_alpha, 1, 0, 0);
	//glRotated(m_beta, 0, 1, 0);


	float pos[] = { 6, 4, 6, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//glTranslated(0, 0.5, 0);
	DrawAxis();

	Kocka(1);
	glTranslated(0, 0.5, 0);
	Telo(0.2, 0.1, 0.05, 1.0, 16);
	glTranslated(0, 1, 0);
	Abazur(0.5, 0.25, 0.2, 16);
	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Telo(double r1, double r2, double r3, double h, int step) {
	glBindTexture(GL_TEXTURE_2D, m_metal);

	double bounds[] = { r1, r2, r3, r3, r2 };

	for (int i = 0; i < 4; i++) {
		double a = 0, ai = 2 * M_PI / step;
		double sc = 0, si = 1. / step;

		glBegin(GL_QUAD_STRIP);

		for (int j = 0; j <= step; j++) {
			double s = sin(a), c = cos(a);

			glNormal3d(c, 0, s);
			glTexCoord2d(sc, 0.25 * i);
			glVertex3d(bounds[i+1] * c, h / 4 * (i + 1), bounds[i+1] * s);
			glTexCoord2d(sc, 0.25 * (i + 1));
			glVertex3d(bounds[i] * c, h / 4 * i, bounds[i] * s);

			sc += si;
			a += ai;
		}

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::Abazur(double r1, double r2, double h, int step) {
	glBindTexture(GL_TEXTURE_2D, m_fabric);

	double a = 0, ai = 2 * M_PI / step;
	double sc = 0, si = 1. / step;

	glBegin(GL_QUAD_STRIP);

	for (int i = 0; i <= step; i++) {
		double s = sin(a), c = cos(a);
		
		glNormal3d(c, 0, s);
		glTexCoord2d(sc, 0);
		glVertex3d(r2 * c, h, r2 * s);
		glTexCoord2d(sc, 1);
		glVertex3d(r1 * c, 0, r1 * s);

		sc += si;
		a += ai;
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::Kocka(double size) {
	glBindTexture(GL_TEXTURE_2D, m_marble);

	glPushMatrix();

	glColor3f(1, 1, 1);
	double s2 = size / 2;

	glNormal3d(0, 0, 1);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-s2, -s2, s2);
	glTexCoord2d(1, 0);
	glVertex3d(s2, -s2, s2);
	glTexCoord2d(1, 1);
	glVertex3d(s2, s2, s2);
	glTexCoord2d(0, 1);
	glVertex3d(-s2, s2, s2);
	glEnd();

	//glPushMatrix();
	glRotated(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-s2, -s2, s2);
	glTexCoord2d(1, 0);
	glVertex3d(s2, -s2, s2);
	glTexCoord2d(1, 1);
	glVertex3d(s2, s2, s2);
	glTexCoord2d(0, 1);
	glVertex3d(-s2, s2, s2);
	glEnd();
	//glPopMatrix();

	glRotated(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-s2, -s2, s2);
	glTexCoord2d(1, 0);
	glVertex3d(s2, -s2, s2);
	glTexCoord2d(1, 1);
	glVertex3d(s2, s2, s2);
	glTexCoord2d(0, 1);
	glVertex3d(-s2, s2, s2);
	glEnd();

	glRotated(90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-s2, -s2, s2);
	glTexCoord2d(1, 0);
	glVertex3d(s2, -s2, s2);
	glTexCoord2d(1, 1);
	glVertex3d(s2, s2, s2);
	glTexCoord2d(0, 1);
	glVertex3d(-s2, s2, s2);
	glEnd();

	glRotated(90, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-s2, -s2, s2);
	glTexCoord2d(1, 0);
	glVertex3d(s2, -s2, s2);
	glTexCoord2d(1, 1);
	glVertex3d(s2, s2, s2);
	glTexCoord2d(0, 1);
	glVertex3d(-s2, s2, s2);
	glEnd();

	glRotated(180, 1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-s2, -s2, s2);
	glTexCoord2d(1, 0);
	glVertex3d(s2, -s2, s2);
	glTexCoord2d(1, 1);
	glVertex3d(s2, s2, s2);
	glTexCoord2d(0, 1);
	glVertex3d(-s2, s2, s2);
	glEnd();

	glPopMatrix();
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / h, 1, 100);
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
