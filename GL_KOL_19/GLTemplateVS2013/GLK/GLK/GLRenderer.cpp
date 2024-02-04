#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_dist = 20;
	m_a = 30;
	m_b = 45;
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

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n) {
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < n; i++) {
		glColor3f(1, 1, 1);

		glTexCoord2d(texCoords[i].x, texCoords[i].y);
		glVertex3d(points[i].x, points[i].y, 0);
	}
	glEnd();
}

void CGLRenderer::DrawSide(double a) {
	glPushMatrix();
	{
		glTranslated(-a / 2, -a / 2, -a / 2);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 1);
			glVertex3d(0, 0, 0);
			glTexCoord2d(1, 1);
			glVertex3d(a, 0, 0);
			glTexCoord2d(1, 0);
			glVertex3d(a, a, 0);
			glTexCoord2d(0, 0);
			glVertex3d(0, a, 0);
		}
		glEnd();
	}
	glPopMatrix();
}

void CGLRenderer::DrawEnvCube(double a) {
	glPushMatrix();

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, right);
	DrawSide(a);
	glRotated(90, 0, 1, 0);
	DrawSide(a);
	glRotated(90, 0, 1, 0);
	DrawSide(a);
	glRotated(90, 0, 1, 0);
	DrawSide(a);
	glRotated(90, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, top);
	glRotated(90, 1, 0, 0);
	DrawSide(a);

	glBindTexture(GL_TEXTURE_2D, bot);
	glRotated(-180, 1, 0, 0);
	DrawSide(a);

	glPopMatrix();
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b) {
	glPushMatrix();
	{
		glTranslated(0, 0, -zh / 2);

		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		{
		 
			glColor3f(r, g, b);
			for (int i = 0; i < n; i++) {
				glVertex3d(points[i].x, points[i].y, zh);
				glVertex3d(points[i].x, points[i].y, 0);
			}
		}
		glEnd();
		glEnable(GL_TEXTURE_2D);

		DrawPolygon(points, texCoords, n);
		glTranslated(0, 0, zh);
		DrawPolygon(points, texCoords, n);
	}
	glPopMatrix();
}

void CGLRenderer::DrawBase()
{
	POINTF points[] = { {15 * rc, 0}, {16 * rc, 1 * rc}, {16 * rc, 3 * rc}, {15 * rc, 4 * rc}, {1 * rc, 4 * rc}, { 0, 3 * rc }, { 0, 1 * rc} , { rc, 0 }, {15 * rc, 0} };
	POINTF texCoords[] = { {15 * x, 16 * x}, {16 * x, 15 * x}, {16 * x, 13 * x}, {15 * x, 12 * x}, {1 * x, 12 * x}, { 0, 13 * x }, { 0, 15 * x} , { x, 16 * x },{15 * x, 16 * x} };

	DrawExtrudedPolygon(points, texCoords, 9, 5, 0, 0, 0);
}

void CGLRenderer::DrawBody() {
	POINTF texCoords[] = { { 8 * x, 8 * x }, {16 * x, 8 * x}, {16 * x, 4 * x}, { 12 * x, 0 }, { 8 * x, 0 }, { 8 * x, 8 * x } };
	POINTF points[] = { {0, 0}, { 8 * rc }, { 8 * rc, 4 * rc }, { 4 * rc, 8 * rc}, {0, 8 * rc}, {0, 0} };

	DrawExtrudedPolygon(points, texCoords, 6, 4, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawArm(double zh) {
	POINTF texCoords[] = { {x, 12 * x}, { 4 * x, 12 * x }, {16 * x, 11 * x}, {16 * x, 9 * x}, { 4 * x, 8 * x}, { x, 8 * x}, {0, 9 * x}, {0, 11 * x }, {x, 12 * x} };
	POINTF points[] = { {rc, 0}, {4 * rc, 0 }, {16 * rc, rc}, {16 * rc, 3 * rc}, {4 * rc, 4 * rc}, {rc, 4 * rc}, {0, 3 * rc}, {0, rc}, {rc, 0} };

	DrawExtrudedPolygon(points, texCoords, 9, zh, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawFork() {
	POINTF points[] = { {0, 0}, {8 * rc, 0 }, {8 * rc, 5 * rc}, { 7 * rc, 6 * rc}, {rc, 6 * rc}, {0, 5 * rc}, {0, 0} };
	POINTF texCoords[] = { {0, 6 * x}, {8 * x, 6 * x }, { 8 * x, x}, { 7 * x, 0}, {x, 0}, {0, x}, {0, 6 * x} };

	DrawExtrudedPolygon(points, texCoords, 7, 1, .7, .7, .7);
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT tid;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &tid);

	glBindTexture(GL_TEXTURE_2D, tid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	DImage img;
	img.Load(CString(fileName));
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return tid;
}
void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 0);

	excavator = LoadTexture("./res/excavator.png");
	left = LoadTexture("./res/left.jpg");
	right = LoadTexture("./res/right.jpg");
	back = LoadTexture("./res/back.jpg");
	top = LoadTexture("./res/top.jpg");
	front = LoadTexture("./res/front.jpg");
	bot = LoadTexture("./res/bot.jpg");
	
	//GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	//GLfloat lmodel_difuse[] = { 0.2, 0.2, 0.2, 1.0 };
	//GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lmodel_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lmodel_difuse);

	//glLighti(GL_LIGHT_MODEL_AMBIENT, GL_AMBIENT)

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawExcavator() {
	glBindTexture(GL_TEXTURE_2D, excavator);
	DrawBase();

	glTranslated(8 * rc, 4 * rc, 0);
	glRotated(kabina12, 0, 1, 0);
	glTranslated(-4 * rc, 0, 0);
	DrawBody();

	glTranslated(6 * rc, 2 * rc, -4.5 * rc);
	glRotated(ruka34, 0, 0, 1);
	glTranslated(-2 * rc, - 2 * rc, 0);
	DrawArm(1);

	glTranslated(15 * rc, 2 * rc, 0);
	glRotated(drukaruka56, 0, 0, 1);
	glTranslated(-2 * rc, -2 * rc, 0);
	DrawArm(1.5);

	glTranslated(15 * rc, 2 * rc, 0);
	glRotated(viljuska78, 0, 0, 1);
	glTranslated(-2 * rc, -2 * rc, 0);
	DrawFork();
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	glTranslated(0, 0, -m_dist);
	glRotated(m_a, 1, 0, 0);
	glRotated(-m_b, 0, 1, 0);

	glDisable(GL_TEXTURE_2D);
	DrawAxis(10);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glTranslated(0, 50, 0);
		DrawEnvCube(100);
	}
	glPopMatrix();

	glTranslated(-8 * rc, 0, 0);
	DrawExcavator();
	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, (double)w / (double)h, 1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->GetSafeHdc(), m_hrc);
	wglMakeCurrent(NULL, NULL);

	glDeleteTextures(1, &left);
	glDeleteTextures(1, &top);
	glDeleteTextures(1, &right);
	glDeleteTextures(1, &bot);
	glDeleteTextures(1, &front);
	glDeleteTextures(1, &back);
	glDeleteTextures(1, &excavator);

	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxis(double len)
{
	glLineWidth(2);
	glBegin(GL_LINES);
	{
		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(len, 0, 0);

		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, len, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, len);
	}
	glEnd();
}
