#include "pch.h"
#include "CGLRenderer.h"

#include "gl\gl.h"
#include "gl\glu.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD(deg) ((deg) * M_PI / 180)

CGLRenderer::CGLRenderer(void)
{
	CalculateCameraPosition();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 0);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(0.5, 0.75, 1.0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(camera[0], camera[1], camera[2], 0, 0, 0, 0, upY, 0);
	//glTranslated(0, -5, 0);

	glPushMatrix();
	{
		glScaled(0.75, 0.75, 0.75);

		if (gridOn) {
			DrawGrid(10, 10, 10, 10);
		}

		DrawFigure(rotAngle);
	}
	glPopMatrix();
	DrawAxis(20);

	//---------------------------------
	SwapBuffers(pDC->GetSafeHdc());
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::UpdatePerspective(int dx, int dy)
{
	vAlpha += dy * 0.2;
	vBeta += dx * 0.2;

	upY = cos(RAD(vAlpha)) < 0 ? -1 : 1;

	CalculateCameraPosition();
}

void CGLRenderer::SetRotAngle(double alpha)
{
	this->rotAngle = alpha;
}

inline double CGLRenderer::GetRotAngle()
{
	return rotAngle;
}

void CGLRenderer::CalculateCameraPosition()
{
	camera[0] = cameraR * cos(RAD(vAlpha)) * cos(RAD(vBeta));
	camera[1] = cameraR * sin(RAD(vAlpha));
	camera[2] = cameraR * cos(RAD(vAlpha)) * sin(RAD(vBeta));
}

void CGLRenderer::ZoomView(bool direction)
{
	cameraR += direction ? 1 : -1;

	CalculateCameraPosition();
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	glPushMatrix();
	glTranslatef(-width / 2, 0, -height / 2);

	double segW = width / nSegW;
	double segH = height / nSegH;
	glBegin(GL_LINES);
	{
		glColor3f(GRID);
		for (auto i = 0; i <= width; i += segW) {
			glVertex3f(i, 0, 0);
			glVertex3f(i, 0, height);
		}

		for (auto i = 0; i <= height; i += segH) {
			glVertex3f(0, 0, i);
			glVertex3f(width, 0, i);
		}
	}
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	double step = 2 * M_PI / nSeg;

	glBegin(GL_TRIANGLE_FAN);
	{
		for (double a = 0; a <= 2 * M_PI; a += step) {
			glVertex3f(r1 * cos(a), 0, r1 * sin(a));
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	{
		for (double a = 0; a <= 2 * M_PI; a += step) {
			glVertex3d(r1 * cos(a), 0, r1 * sin(a));
			glVertex3d(r2 * cos(a), h, r2 * sin(a));
		}
	}
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	{
		for (double a = 0; a <= 2 * M_PI; a += step) {
			glVertex3f(r2 * cos(a), h, r2 * sin(a));
		}
	}
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	double bstep = 2 * M_PI / nSegBeta;
	double astep = M_PI / nSegAlpha;

	glColor3f(SFERA);

	glBegin(GL_QUAD_STRIP);
	{
		for (double a = 0; a <= 2 * M_PI; a += astep) {
			for (double b = 0; b <= M_PI + bstep; b += bstep) {
				glVertex3f(r * cos(a) * cos(b), r * sin(a), r * cos(a) * sin(b));
				glVertex3f(r * cos(a + astep) * cos(b), r * sin(a + astep), r * cos(a + astep) * sin(b));
			}
		}
	}
	glEnd();
}


void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	double alphaStep = 2 * M_PI / nSeg;

	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, h, 0);

		for (double alpha = 0; alpha <= 2 * M_PI; alpha += alphaStep) {
			double x = r * cos(alpha);
			double z = r * sin(alpha);
			glVertex3d(x, 0, z);
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, 0, 0);

		for (double alpha = 0; alpha <= 2 * M_PI; alpha += alphaStep) {
			glVertex3d(r * cos(alpha), 0, r * sin(alpha));
		}
	}
	glEnd();
}

void CGLRenderer::DrawFigure(double angle)
{
	DrawPot(3, 2, 2.25);

	glColor3f(DEO_KAKTUSA);
	DrawCylinder();

	glTranslated(0, VISINA_V_K + PP_SFERE, 0);
	glColor3f(SFERA);
	DrawSphere();

	//osnova
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);

		glTranslated(0, PP_SFERE, 0);
		glColor3d(DEO_KAKTUSA);
		DrawCone();

		glTranslated(0, PP_SFERE + VISINA_V_K, 0);
		glColor3d(SFERA);
		DrawSphere();

		glPushMatrix();
		{
			glRotated(angle, 1, 0, 0);
			glTranslated(0, PP_SFERE, 0);
			glColor3d(0, 1, 1);
			DrawCone();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotated(-45, 1, 0, 0);
			glTranslated(0, PP_SFERE, 0);
			glColor3d(DEO_KAKTUSA);
			DrawCylinder();
		}
		glPopMatrix();

	}
	glPopMatrix();
}

void CGLRenderer::DrawAxis(double width)
{
	glBegin(GL_LINES);
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(width, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, width, 0.0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, width);
	}
	glEnd();

	glFlush();
}

void CGLRenderer::DrawPot(double height, double r1, double r2)
{
	double bh = height / 6;

	glColor3d(VAZA);
	DrawCylinder(bh * 4, r1, r2, 8);
	glTranslated(0, bh * 4, 0);
	DrawCylinder(bh * 2, r2 * 1.25, r2 * 1.25, 8);
	glTranslated(0, bh * 2, 0);
}

void CGLRenderer::ToggleGrid()
{
	gridOn = !gridOn;
}
