#pragma 

#define	POZADINA 0.50, 0.75, 1.00
#define VAZA  1.00, 0.60, 0.00
#define DEO_KAKTUSA 0.47, 0.00, 0.00
#define SFERA 1.00, 1.00, 1.00
#define GRID 0.00, 0.80, 0.00

#define PP_SFERE 0.6
#define OSNOVA_V_K 0.8
#define VISINA_V_K 2.4

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
	void DrawCylinder(double h = VISINA_V_K, double r1 = OSNOVA_V_K, double r2 = OSNOVA_V_K, int nSeg = 8);
	void DrawSphere(double r = PP_SFERE, int nSegAlpha = 40, int nSegBeta = 40);
	void DrawCone(double h = VISINA_V_K, double r = OSNOVA_V_K, int nSeg = 8);
	void DrawFigure(double angle);
	void DrawAxis(double width);
	void DrawPot(double height, double r1, double r2);
	
	void SetRotAngle(double alpha);
	inline double GetRotAngle();
	void IncrementRotAngle(double angle) { rotAngle += angle; };
	void DecrementRotAngle(double angle) { rotAngle -= angle; };
	void CalculateCameraPosition();
	void ZoomView(bool direction);

	void ToggleGrid();
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
private:

	double vAlpha = 30;
	double vBeta = 45;
	int upY = 1;
	double cameraR = 30;
	double camera[3];

	bool gridOn = false;

	double rotAngle = 0;
};
