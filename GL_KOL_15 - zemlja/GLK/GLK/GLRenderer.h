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
	UINT CGLRenderer::LoadTexture(char* fileName);
	void DrawAxis();
	void ParToPolar(double x, double y, double &a, double &b);
	void CGLRenderer::DrawPatch(double R, int n);
	void CGLRenderer::DrawSphere(double R, int n, UINT tex[]);
	void CGLRenderer::DrawMoon(double R, int n);
	void CGLRenderer::DrawEarth(double R, int n);
	void CGLRenderer::DrawSpace(double R, int n);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT m_spaceTex[6];
	UINT m_moonTex[6];
	UINT m_earchTex[6];
public:
	double m_alpha, m_beta, m_dist, m_moonRot;
	
};
