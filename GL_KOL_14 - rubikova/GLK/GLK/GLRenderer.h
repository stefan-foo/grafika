#pragma once

#include "DImage.h"

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void CGLRenderer::DrawCube(float a, int i, int j, int k, int max = 3);
	void CGLRenderer::DrawSide(float a, double t1 = 0, double s1 = 0, int max =  3);
	void CGLRenderer::DrawAxis(float w);
	void CGLRenderer::DrawRubikCube(double a, int count);
	UINT CGLRenderer::LoadTexture(char* path);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
public:
	double m_alpha, m_beta, m_dist;
	double m_cRot[10] = {0, 0, 0, 0};

	UINT m_tex;
};
