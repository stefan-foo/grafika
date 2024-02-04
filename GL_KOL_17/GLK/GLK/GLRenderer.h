#pragma once

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

	void CGLRenderer::DrawAxis(double width);
	void CGLRenderer::DrawGear(double a, double alpha, int n, double h, double d);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	double m_alpha, m_beta, m_dist;
};
