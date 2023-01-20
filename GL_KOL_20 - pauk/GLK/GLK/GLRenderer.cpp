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
	m_dist = 10;
	m_alpha = m_beta = 0;
	m_light = false;
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

	int i = 0;
	m_texEnv[i] = LoadTexture("./front.jpg");
	i++;
	m_texEnv[i] = LoadTexture("./left.jpg");
	i++;
	m_texEnv[i] = LoadTexture("./back.jpg");
	i++;
	m_texEnv[i] = LoadTexture("./right.jpg");
	i++;
	m_texEnv[i] = LoadTexture("./top.jpg");
	i++;
	m_texEnv[i] = LoadTexture("./bot.jpg");

	m_spiderTex = LoadTexture("./spider.png");

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	float amb[] = { 1, 0, 0, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
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

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glDisable(GL_LIGHT0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	float gla[] = { 0.2, 0.2, 0.2, 1 };
	float gld[] = { 1, 0, 0, 1 };
	float gls[] = { 1, 0, 0, 1 };
	float glp[] = { 0, 1, 0, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, gla);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gld);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gls);

	glTranslated(0, 0, -m_dist);
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, glp);

	if (m_light) {
		glEnable(GL_LIGHT0);
	}

	DrawAxes();

	glPushMatrix();
	glTranslated(0, 50, 0);

	DrawEnvCube(100);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, m_spiderTex);
	glTranslated(0, (double)4 + c_height, 0);
	DrawSpider();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	glFlush();
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawSpider() {
	DrawSpiderBody();

	glPushMatrix();

	glTranslated(3.8, 0.6 * 3, 0);


	for (int i = 0; i < (m_copterMode ? 16 : 4); i++) {
		glPushMatrix();

		if (!m_copterMode) {
			glRotated(45 + i * 30, 0, 1, 0);
			glRotated(55, 0, 0, 1);
		}
		else {
			glRotated(c_rotation + 45 + i * 30, 0, 1, 0);
		}

		DrawLeg();
		glPopMatrix();

		glPushMatrix();
		
		if (!m_copterMode) {
			glRotated(-45 - i * 30, 0, 1, 0);
			glRotated(55, 0, 0, 1);
		}
		else {
			glRotated(c_rotation - 45 - i * 30, 0, 1, 0);
		}

		DrawLeg();
		glPopMatrix();
	}

	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody() {
	glPushMatrix();
	glPushMatrix();
	glScalef(0.5, 1, 1);
	DrawSphere(2, 10, 0.75, 0.25, 0.25);
	glPopMatrix();

	glTranslated(3.8, 0, 0);

	glPushMatrix();
	glScalef(1, 0.8, 1);
	DrawSphere(3, 10, 0.25, 0.25, 0.25);
	glPopMatrix();
	glTranslated(7.5, 0, 0);

	glPushMatrix();
	glScalef(1, 0.8, 1);
	DrawSphere(5, 10, 0.25, 0.25, 0.25);
	glPopMatrix();
	glPopMatrix();
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR) {
	double ai = 2 * M_PI / nSeg;
	double a = 0;

	double L = sqrt(r * r + h * h);
	double nr = h / L;
	double ny = r / L;

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < nSeg; i++) {
		double c = cos(a), s = sin(a);

		glNormal3d(nr * c, ny, nr * s);
		glTexCoord2d(texU, texV);
		glVertex3d(0, h, 0);

		glNormal3d(nr * c, ny, nr * s);
		glTexCoord2d(texU + c * texR, texV + s * texR);
		glVertex3d(r * c, 0, r * s);

		c = cos(a + ai), s = sin(a + ai);

		glNormal3d(nr * c, ny, nr * s);
		glTexCoord2d(texU + c * texR, texV + s * texR);
		glVertex3d(r * c, 0, r * s);

		a += ai;
	}

	glEnd();
}

void CGLRenderer::DrawLeg() {
	glPushMatrix();

	DrawLegSegment(1, 10, 5);
	glTranslated(0, 10.5, 0);
	glRotated(85, 0, 0, 1);
	DrawLegSegment(1, 15, 5);

	glPopMatrix();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg) {
	glPushMatrix();

	DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.25);
	//glTranslated(0, r / 2, 0);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.25);

	glPopMatrix();
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR) {
	double ai = M_PI / nSeg;
	double a = -M_PI_2;
	double b = 0;

	for (int i = 0; i < nSeg; i++) {
		b = 0;
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= 2 * nSeg; j++) {
			double nx = cos(a) * cos(b);
			double ny = sin(a);
			double nz = cos(a) * sin(b);

			glNormal3d(nx, ny, nz);
			glTexCoord2d(texU + nx * texR, texV + nz * texR);
			glVertex3d(r * nx, r * ny, r * nz);

			nx = cos(a + ai) * cos(b);
			ny = sin(a + ai);
			nz = cos(a + ai) * sin(b);

			glNormal3d(nx, ny, nz);
			glTexCoord2d(texU + nx * texR, texV + nz * texR);
			glVertex3d(r * nx, r * ny, r * nz);

			b += ai;
		}
		glEnd();
		a += ai;
	}
}

void CGLRenderer::DrawEnvCube(double a) {
	double a2 = a / 2;
	glColor3f(1, 1, 1);

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, m_texEnv[0]);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(0, 1);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(1, 1);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(1, 0);
	glVertex3d(a2, a2, -a2);
	glEnd();

	glRotated(90, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, m_texEnv[1]);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(0, 1);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(1, 1);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(1, 0);
	glVertex3d(a2, a2, -a2);
	glEnd();

	glRotated(90, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, m_texEnv[2]);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(0, 1);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(1, 1);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(1, 0);
	glVertex3d(a2, a2, -a2);
	glEnd();


	glRotated(90, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, m_texEnv[3]);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(0, 1);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(1, 1);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(1, 0);
	glVertex3d(a2, a2, -a2);
	glEnd();

	glRotated(90, 0, 1, 0);
	glRotated(90, 1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, m_texEnv[4]);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(0, 1);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(1, 1);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(1, 0);
	glVertex3d(a2, a2, -a2);
	glEnd();


	glRotated(180, 1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, m_texEnv[5]);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-a2, a2, -a2);
	glTexCoord2d(0, 1);
	glVertex3d(-a2, -a2, -a2);
	glTexCoord2d(1, 1);
	glVertex3d(a2, -a2, -a2);
	glTexCoord2d(1, 0);
	glVertex3d(a2, a2, -a2);
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawAxes() {
	glLineWidth(2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBegin(GL_LINES);

	glColor3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 50, 0);

	glColor3d(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(50, 0, 0);

	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 50);

	glEnd();
}


void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / h, 0.1, 200);
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
