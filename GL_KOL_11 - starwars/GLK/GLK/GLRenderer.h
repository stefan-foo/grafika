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
	void DrawAxis(double width);
	UINT CGLRenderer::LoadTexture(char* path);
	void CGLRenderer::DrawBody(double r, double h1, double h2, int n);
	void CGLRenderer::DrawCompoundBody();
	void CGLRenderer::DrawWing();
	void CGLRenderer::DrawWings();
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT m_tex;
public:
	double m_alpha, m_beta, m_dist;

};
