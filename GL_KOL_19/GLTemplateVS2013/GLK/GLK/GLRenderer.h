#pragma once

#include "DImage.h"

class CGLRenderer
{


public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	
	UINT LoadTexture(char* fileName);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);
	// dealocira resurse alocirane u drugim funkcijama ove klase,
	void DrawFork();
	void DrawAxis(double len);
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void CGLRenderer::DrawExcavator();
	void CGLRenderer::DrawEnvCube(double a);
	void CGLRenderer::DrawSide(double a);
	void DrawArm(double zh);
	void DrawBase();
	void DrawBody();
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT excavator, front, left, right, back, top, bot;
public:
	double m_dist;
	double m_a, m_b;

	double kabina12 = 0;
	double ruka34 = 90;
	double drukaruka56 = -90;
	double viljuska78 = -90;

	const double x = 1. / 16;
	const double rc = 0.5;
};
