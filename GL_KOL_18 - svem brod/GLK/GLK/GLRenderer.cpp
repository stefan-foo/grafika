#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define _USE_MATH_DEFINES
#include <math.h>

constexpr double toDeg = 180.0 / M_PI;

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
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	int i = 0;
	m_cubeTex[i] = LoadTexture("./front.jpg");
	i++;
	m_cubeTex[i] = LoadTexture("./left.jpg");
	i++;
	m_cubeTex[i] = LoadTexture("./right.jpg");
	i++;
	m_cubeTex[i] = LoadTexture("./back.jpg");
	i++;
	m_cubeTex[i] = LoadTexture("./top.jpg");
	i++;
	m_cubeTex[i] = LoadTexture("./bot.jpg");

	m_shipTex = LoadTexture("./ShipT1.png");

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);
	DrawSpaceCube(10);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	glTranslated(0, 0, -m_dist);
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);


	GLfloat ldir[] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, ldir);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	DrawAxis(10);
	DrawShip();
	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawShip() {
	double a = atan2(2.15, 5.8);
	double a1 = atan2(2.15, 5.8);

	float rep = 3.0;
	// Gornji levo
	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * toDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	// Gornji desno
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * toDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	// Levo
	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * toDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	// Desno
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * toDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	glPopMatrix();
}

void CGLRenderer::DrawSpaceCube(double a) {

	double a2 = a / 2;

	glPushMatrix();
	////front
	//glColor3f(1, 1, 1);
	//glBindTexture(GL_TEXTURE_2D, m_cubeTex[0]);

	//glBegin(GL_QUADS);
	//glTexCoord2f(0, 1);
	//glVertex3f(-a / 2, -a / 2, a / 2);
	//glTexCoord2f(0, 0);
	//glVertex3f(-a / 2, a / 2, a / 2);
	//glTexCoord2f(1, 0);
	//glVertex3f(a / 2, a / 2, a / 2);
	//glTexCoord2f(1, 1);
	//glVertex3f(a / 2, -a / 2, a / 2);
	//glEnd();

	//glRotatef(90, 0, 1, 0);
	//// levo
	//glBindTexture(GL_TEXTURE_2D, m_cubeTex[2]);

	//glBegin(GL_QUADS);

	//glTexCoord2f(0, 1);
	//glVertex3f(-a / 2, -a / 2, a / 2);

	//glTexCoord2f(0, 0);
	//glVertex3f(-a / 2, a / 2, a / 2);

	//glTexCoord2f(1, 0);
	//glVertex3f(a / 2, a / 2, a / 2);


	//glTexCoord2f(1, 1);
	//glVertex3f(a / 2, -a / 2, a / 2);

	//glEnd();

	glRotatef(90, 0, 1, 0);
	// pozadi
	glBindTexture(GL_TEXTURE_2D, m_cubeTex[3]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(90, 0, 1, 0);
	// desno
	glBindTexture(GL_TEXTURE_2D, m_cubeTex[1]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	// dole
	glBindTexture(GL_TEXTURE_2D, m_cubeTex[5]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(2 * 90, 1, 0, 0);
	// dole
	glBindTexture(GL_TEXTURE_2D, m_cubeTex[4]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawTriangle(float d1, float d2, float rep) {
	double a = atan2(d2, d1);
	double c = sqrt(d1 * d1 + d2 * d2);
	double h = d1 * sin(a) / c;
	double cr = d1 * cos(a) / c;

	glBindTexture(GL_TEXTURE_2D, m_shipTex);

	glNormal3d(0, 0, 1);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);

	glTexCoord2d(0.5 * rep, 0);
	glVertex3d(0, 0, 0);
	glTexCoord2d((0.5 + h) * rep, cr * rep);
	glVertex3d(d1, 0, 0);
	glTexCoord2d(0.5 * rep, rep);
	glVertex3d(d1, d2, 0);
	glEnd();

}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / h, 1, 2000);
	glMatrixMode(GL_MODELVIEW);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	DImage img;
	img.Load(CString(fileName));

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texId;
}

void CGLRenderer::DrawAxis(double l) {
	glBegin(GL_LINES);

	glColor3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(l, 0, 0);

	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, l, 0);

	glColor3d(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, l);

	glEnd();
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(6, m_cubeTex);
	glDeleteTextures(1, &m_shipTex);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
