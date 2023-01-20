#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

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

	m_tex = LoadTexture("./OpenGL.bmp");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(0, 0, -m_dist);
	glRotated(m_alpha, 1, 0, 0);
	glRotated(m_beta, 0, 1, 0);

	DrawAxis(10);

	DrawRubikCube(0.5, 3);
	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

UINT CGLRenderer::LoadTexture(char* path) {
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
	img.Load(CString(path));

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texId;

}

void CGLRenderer::DrawRubikCube(double a, int count) {
	//count = 10;
	//a = 0.05;

	glPushMatrix();
	for (int i = 0; i < count; i++) {
		glPushMatrix();
		glTranslated(0,  1.55 * a - 0.5 * a, 1.55 * a - 0.5 * a);
		glRotated(m_cRot[i], 1, 0, 0);
		glTranslated(0, -1.55 * a + 0.5 * a, -1.55 * a + 0.5 * a);

		for (int j = 0; j < count; j++) {
			glPushMatrix();
			for (int k = 0; k < count; k++) {
				DrawCube(a, i, j, k, count);
				glTranslated(0, 0, 1.05 * a);
			}
			glPopMatrix();
			glTranslated(0, 1.05 * a, 0);
		}
		glPopMatrix();
		glTranslated(1.05 * a, 0, 0);
	}
	glPopMatrix();
}

void CGLRenderer::DrawCube(float a, int i, int j, int k, int max) {

	glBindTexture(GL_TEXTURE_2D, m_tex);

	glPushMatrix();
	glColor3d(1, 0, 0);
	DrawSide(a, max - 1 - j, i, max);

	glRotated(90, 0, 1, 0);
	glColor3d(0, 0, 1);
	DrawSide(a, max - j - 1, max - k - 1, max);

	glRotated(90, 0, 1, 0);
	glPushMatrix();
	glScalef(1, -1, 1);
	glColor3d(1, 0.4, 0);
	DrawSide(a, j, max - i - 1, max);
	glPopMatrix();

	//glCullFace()

	glRotated(90, 0, 1, 0);
	glColor3d(1, 0.8, 0);
	DrawSide(a, max - j - 1, k, max);

	glColor3d(1, 1, 1);
	glRotated(90, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	DrawSide(a, max - 1 - k, i, max);

	glColor3d(1, 0, 1);
	glRotated(180, 1, 0, 0);
	DrawSide(a, k, i, max);
	DrawSide(a);

	glPopMatrix();
}

void CGLRenderer::DrawAxis(float w) {
	glBegin(GL_LINES);
	{
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(w, 0, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, w, 0);

		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, w);
	}
	glEnd();
}

void CGLRenderer::DrawSide(float a, double t1, double s1, int max) {
	double c = 1. / max;

	t1 *= c;
	s1 *= c;

	double a2 = a / 2;
	double t2 = t1 + c;
	double s2 = s1 + c;

	glNormal3d(0, 0, 1);

	glBegin(GL_QUADS);
	{
		glTexCoord2d(s1, t2);
		glVertex3d(-a2, -a2, a2);
		glTexCoord2d(s2, t2);
		glVertex3d(a2, -a2, a2);
		glTexCoord2d(s2, t1);
		glVertex3d(a2, a2, a2);
		glTexCoord2d(s1, t1);
		glVertex3d(-a2, a2, a2);
	}
	glEnd();
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / h, 1, 2000);
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
