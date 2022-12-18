#pragma 
#include "CGLMaterial.h"

#define	VAZA_BOJA_1 1.00, 0.00, 1.00
#define VAZA_BOJA_1_A .25, 0, .25
#define VAZA_BOJA_1_S .5, 0, .5

#define VAZA_BOJA_2 0.00, 1.00, 1.00
#define VAZA_BOJA_2_A 0, .25, .25
#define VAZA_BOJA_2_S 0, .5, .5

#define SVETLO_1 1.00, 0.00, 1.00
#define SVETLO_1_S 0.4, 0, 0.4

#define SVETLO_2 1.00, 0.00, 0.00
#define SVETLO_2_S 0.4, 0, 0

#define SVETLO_3 0.00, 0.00, 1.00
#define SVETLO_3_S 0, 0, 0.4

#define GRID 1.0, 0, 0

constexpr auto PP_SFERE = 0.6;
constexpr auto OSNOVA_V_K = 0.8;
constexpr auto VISINA_V_K = 2.4;

constexpr auto R_R1 = 5;
constexpr auto R_R2 = 3.5;
constexpr auto R_R3 = 2;
constexpr auto R_H = 1.2;

enum class Color {
	BOJA_1,
	BOJA_2,
	DEFAULT
};

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
	void UpdatePerspective(int dx, int dy);

	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void DrawSphere(double r = PP_SFERE, int nSegAlpha = 40, int nSegBeta = 40);
	void DrawCone(double h = VISINA_V_K, double r = OSNOVA_V_K, int nSeg = 40);
	void DrawCylinder(double h, double r1, double r2, bool showNormals, CGLMaterial& mat, int nSeg = 40, bool includeBase = false);
	void DrawAxis(double width);
	void DrawPot(double height, double r1, double r2);
	
	void DrawLight(double posX, double posY, double posZ, double r, CGLMaterial& mat, bool lightOn = true);
	void ConfigureLight(GLenum light, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, bool lightOn = true);

	void DrawStand();
	void DrawVase(bool showNormals = false);
	void DrawWall(double h, int nSeg);
	void DrawSide(double h, double w, int nSegH, int nSegW);
	void DrawCube(double h, int nSeg);
	void DrawCuboid(double h, double w, double d, int nSegH, int nSegW, int nSegD);

	void CalculateCameraPosition();
	void ZoomView(bool direction);

	void ToggleGrid();
	void ToggleNormals();
	void ToggleLight(char light);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
private:
	CGLMaterial vaseR1, vaseR2, light1Mat, light2Mat, light3Mat, table, wall;

	double cameraR = 40;
	double vAlpha = 30;
	double vBeta = 45;
	int upY = 1;
	double camera[3];

	bool gridOn = false;
	bool normalsOn = true;

	bool light1On = true, light2On = true, light3On = true;
};
