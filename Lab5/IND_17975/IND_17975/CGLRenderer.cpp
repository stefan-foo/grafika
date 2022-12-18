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

	light1Mat.SetAmbient(SVETLO_1, 1);
	light3Mat.SetDiffuse(SVETLO_1, 1);
	light1Mat.SetEmission(SVETLO_1, 1);
	light3Mat.SetShininess(128);

	light2Mat.SetAmbient(SVETLO_2, 1);
	light3Mat.SetDiffuse(SVETLO_2, 1);
	light2Mat.SetEmission(SVETLO_2, 1);
	light3Mat.SetShininess(128);

	light3Mat.SetAmbient(SVETLO_3, 1);
	light3Mat.SetDiffuse(SVETLO_3, 1);
	light3Mat.SetEmission(SVETLO_3, 1);
	light3Mat.SetShininess(128);

	vaseR1.SetAmbient(VAZA_BOJA_1_A, 1.0);
	vaseR1.SetDiffuse(VAZA_BOJA_1, 1.0);
	vaseR1.SetSpecular(VAZA_BOJA_1_S, 1);

	vaseR2.SetAmbient(VAZA_BOJA_2_A, 1.0);
	vaseR2.SetDiffuse(VAZA_BOJA_2, 1.0);
	vaseR2.SetSpecular(VAZA_BOJA_2_S, 1);

	wall.SetAmbient(.2, .2, .2, 1);
	wall.SetDiffuse(.6, .6, .6, 1);
	wall.SetEmission(0, 0, 0, 1);
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
	pfd.cDepthBits = 32;
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
	glClearColor(0.0, 0.0, 0.0, 1);
	glEnable(GL_DEPTH_TEST);

	GLfloat lm_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	GLfloat light_ambient[] = {0.6, 0.6, 0.6, 1.0};

	GLfloat light0_diffuse[] = { 0.4, 0.4, 0.4, 0 };
	GLfloat light0_specular[] = { 0.1, 0.1, 0.1, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);

	GLfloat light1_diffuse[] = { SVETLO_1, 1 };
	GLfloat light1_specular[] = { SVETLO_1_S, 1 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);

	GLfloat light2_diffuse[] = { SVETLO_2, 1 };
	GLfloat light2_specular[] = { SVETLO_2_S, 1 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25);

	GLfloat light3_diffuse[] = { SVETLO_3, 1 };
	GLfloat light3_specular[] = { SVETLO_3_S, 1 };
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25);

	glEnable(GL_LIGHTING);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(camera[0], camera[1], camera[2], 0, 0, 0, 0, upY, 0);

	ConfigureLight(GL_LIGHT0, 0, 20, 0, 0, -1, 0, true);
	DrawLight(-20, 0, 0, 1, light1Mat, light1On);
	ConfigureLight(GL_LIGHT1, -20, 0, 0, 1, 0, 0, light1On);
	DrawLight(20, 0, 0, 1, light2Mat, light2On);
	ConfigureLight(GL_LIGHT2, 20, 0, 0, -1, 0, 0, light2On);
	DrawLight(0, 20, 0, 1, light3Mat, light3On);
	ConfigureLight(GL_LIGHT3, 0, 20, 0, 0, -1, 0, light3On);

	glPushMatrix();
	{
		DrawCube(40, 100);
		glTranslated(0, -20, 0);
		DrawStand();
		glTranslated(0, 12, 0);
		DrawVase(normalsOn);
	}
	glPopMatrix();

	glTranslated(0, -20, 0);
	DrawAxis(50);

	glFlush();
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

void CGLRenderer::CalculateCameraPosition()
{
	camera[0] = cameraR * cos(RAD(vAlpha)) * cos(RAD(vBeta));
	camera[1] = cameraR * sin(RAD(vAlpha));
	camera[2] = cameraR * cos(RAD(vAlpha)) * sin(RAD(vBeta));
}

void CGLRenderer::ZoomView(bool direction)
{
	cameraR += direction ? 1 : -1;

	if (cameraR < 1) {
		cameraR = 1;
	}
	else if (cameraR > 80) {
		cameraR = 80;
	}

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

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	double bstep = 2 * M_PI / nSegBeta;
	double astep = M_PI / nSegAlpha;

	glBegin(GL_TRIANGLE_STRIP);
	{
		for (double a = - M_PI / 2; a < M_PI / 2; a += astep) {
			for (double b = 0; b < 2.01 * M_PI; b += bstep) {
				double x = cos(a) * cos(b);
				double y = sin(a);
				double z = cos(a) * sin(b);

				glNormal3d(x, y, z);
				glVertex3f(r * x, r * y, r * z);

				x = cos(a + astep) * cos(b);
				y = sin(a + astep);
				z = cos(a + astep) * sin(b);

				glNormal3d(x, y, z);
				glVertex3f(r * x, r * y, r * z);
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


void CGLRenderer::DrawAxis(double width)
{
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawPot(double height, double r1, double r2)
{
	double bh = height / 6;

	//glColor3d(VAZA);
	//DrawCylinder(bh * 4, r1, r2, 8);
	//glTranslated(0, bh * 4, 0);
	//DrawCylinder(bh * 2, r2 * 1.25, r2 * 1.25, 8);
	//glTranslated(0, bh * 2, 0);
}

void CGLRenderer::DrawLight(double posX, double posY, double posZ, double r, CGLMaterial& mat, bool lightOn)
{
	if (!lightOn) {
		return;
	}

	mat.Select(GL_FRONT);

	glPushMatrix();
	{
		glTranslated(posX, posY, posZ);
		DrawSphere(r);
	}
	glPopMatrix();
}

void CGLRenderer::ConfigureLight(GLenum light, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, bool lightOn)
{

	if (!lightOn) {
		glDisable(light);
		return;
	}

	GLfloat light_pos[] = { posX, posY, posZ, 1 };
	GLfloat spot_dir[] = { dirX, dirY, dirZ, 1 };

	glLightfv(light, GL_POSITION, light_pos);
	glLightfv(light, GL_SPOT_DIRECTION, spot_dir);

	glEnable(light);
}

void CGLRenderer::DrawStand()
{
	wall.Select(GL_FRONT);

	glPushMatrix();
		DrawSphere(6, 40, 40);
		DrawCylinder(10, 3, 3, false, wall, 8, true);
		glTranslated(0, 10, 0);

		DrawCuboid(2, 15, 15, 2, 15, 15);
	glPopMatrix();
}

void CGLRenderer::DrawVase(bool showNormals)
{
	glPushMatrix();
	{
		DrawCylinder(R_H, R_R1, R_R2, showNormals, vaseR2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, showNormals, vaseR1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R3, showNormals, vaseR2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R3, R_R3, showNormals, vaseR1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, showNormals, vaseR2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, showNormals, vaseR1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, showNormals, vaseR2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R1, showNormals, vaseR1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R1, R_R2, showNormals, vaseR2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, showNormals, vaseR1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, showNormals, vaseR2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R3, showNormals, vaseR1);
		glTranslated(0, R_H, 0);

		DrawCylinder(R_H, R_R3, R_R2, showNormals, vaseR2);
		glTranslated(0, R_H, 0);
		DrawCylinder(R_H, R_R2, R_R1, showNormals, vaseR1);
		glTranslated(0, R_H, 0);
	}
}

void CGLRenderer::DrawWall(double w, int nSeg)
{
	double segW = w / nSeg;

	wall.Select(GL_FRONT);

	glNormal3d(0, 0, 1);
	glBegin(GL_QUADS);
	{
		for (double i = 0; i <= w - segW; i += segW) {
			for (double j = 0; j <= w - segW; j += segW) {
				glVertex3d(j, i, 0);
				glVertex3d(j + segW, i, 0);
				glVertex3d(j + segW, i + segW, 0);
				glVertex3d(j, i + segW, 0);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawSide(double h, double w, int nSegH, int nSegW)
{
	double segW = w / nSegW;
	double segH = h / nSegH;

	glNormal3d(0, 0, 1);
	glBegin(GL_QUADS);
	{
		for (double i = 0; i < h; i += segH) {
			for (double j = 0; j < w; j += segW) {
				glVertex3d(j, i, 0);
				glVertex3d(j + segW, i, 0);
				glVertex3d(j + segW, i + segH, 0);
				glVertex3d(j, i + segH, 0);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCube(double w, int nSeg)
{
	glEnable(GL_CULL_FACE);

	glPushMatrix();
	{
		glTranslated(-w / 2, -w / 2, -w / 2);

		glPushMatrix();
		{
			DrawWall(w, nSeg);

			glPushMatrix();
			{
				glRotated(90, 0, 1, 0);
				glTranslated(-w, 0, 0);
				DrawWall(w, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(-90, 0, 1, 0);
				glTranslated(0, 0, -w);
				DrawWall(w, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(180, 0, 1, 0);
				glTranslated(-w, 0, -w);
				DrawWall(w, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(90, 1, 0, 0);
				glTranslated(0, 0, -w);
				DrawWall(w, nSeg);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glRotated(-90, 1, 0, 0);
				glTranslated(0, -w, 0);
				DrawWall(w, nSeg);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawCuboid(double h, double w, double d, int nSegH, int nSegW, int nSegD)
{
	// prednja
	glPushMatrix();
	{
		glTranslated(-w / 2, 0, d / 2);
		DrawSide(h, w, nSegH, nSegW);
	}
	glPopMatrix();

	// zadnja
	glPushMatrix();
	{
		glRotated(180, 0, 1, 0);
		glTranslated(-w / 2, 0, d / 2);
		DrawSide(h, w, nSegH, nSegW);
	}
	glPopMatrix();

	// gornja
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glTranslated(-w / 2, -d / 2, - h);
		DrawSide(w, d, nSegH, nSegW);
	}
	glPopMatrix();

	// donja
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glTranslated(-w / 2, -d / 2, 0);
		DrawSide(w, d, nSegH, nSegW);
	}
	glPopMatrix();

	//leva
	glPushMatrix();
	{
		glRotated(90, 0, 1, 0);
		glTranslated(-d / 2, 0, - w/ 2);
		DrawSide(h, d, nSegH, nSegW);
	}
	glPopMatrix();
	//desna
	glPushMatrix();
	{
		glRotated(-90, 0, 1, 0);
		glTranslated(-d / 2, 0, -w / 2);
		DrawSide(h, d, nSegH, nSegW);
	}
	glPopMatrix();
}



void CGLRenderer::DrawCylinder(double h, double r1, double r2, bool showNormals, CGLMaterial& mat, int nSeg, bool includeBase)
{
	double step = 2 * M_PI / nSeg;

	double r = abs(r2 - r1) / 2;
	double L = sqrt(pow(r, 2) + pow(h, 2));
	double ny = r1 >= r2 ? r / L : -r / L;
	double nr = h / L;

	mat.Select(GL_FRONT_AND_BACK);

	glBegin(GL_TRIANGLE_STRIP);
	{
		for (double a = 0; a < 2 * M_PI + step; a += step) {
			glNormal3d(nr * cos(a), ny, nr * sin(a));

			glVertex3d(r1 * cos(a), 0, r1 * sin(a));
			glVertex3d(r2 * cos(a), h, r2 * sin(a));
		}
	}
	glEnd();

	if (includeBase) {
		glNormal3d(0, -1, 0);

		glBegin(GL_TRIANGLE_FAN);
		{
			for (double a = 2 * M_PI; a >= 0; a -= step) {
				glVertex3f(r1 * cos(a), 0, r1 * sin(a));
			}
		}
		glEnd();

		glNormal3d(0, 1, 0);

		glBegin(GL_TRIANGLE_FAN);
		{
			for (double a = 2 * M_PI; a >= 0; a -= step) {
				glVertex3f(r1 * cos(a), h, r1 * sin(a));
			}
		}
		glEnd();
	}

	if (!showNormals) {
		return;
	}

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	{
		glColor3d(0, 1, 0);

		for (double a = 0; a <= 2 * M_PI; a += step) {
			double x1 = r1 * cos(a);
			double z1 = r1 * sin(a);

			double x2 = r2 * cos(a);
			double z2 = r2 * sin(a);

			glVertex3d(x1, 0, z1);
			glVertex3d(x1 + nr * cos(a), ny, z1 + nr * sin(a));

			glVertex3d(x2, h, z2);
			glVertex3d(x2 + nr * cos(a), h + ny, z2 + nr * sin(a));
		}
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

void CGLRenderer::ToggleGrid()
{
	gridOn = !gridOn;
}

void CGLRenderer::ToggleNormals()
{
	normalsOn = !normalsOn;
}

void CGLRenderer::ToggleLight(char light)
{
	switch (light) {
	case '1':
		light1On = !light1On;
		break;
	case '2':
		light2On = !light2On;
		break;
	case '3':
		light3On = !light3On;
		break;
	}
}
