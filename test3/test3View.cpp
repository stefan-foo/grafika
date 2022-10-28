
// test3View.cpp : implementation of the Ctest3View class
//



#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "test3.h"
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD(degrees) ((degrees) * M_PI / 180) 
#define DEG(radians) ((radians) * 180 / M_PI)

#include "test3Doc.h"
#include "test3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctest3View

IMPLEMENT_DYNCREATE(Ctest3View, CView)

BEGIN_MESSAGE_MAP(Ctest3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Ctest3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Ctest3View construction/destruction

Ctest3View::Ctest3View() noexcept
{
	// TODO: add construction code 
	gridOn = true;
}

Ctest3View::~Ctest3View()
{
}

BOOL Ctest3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Ctest3View drawing

void Ctest3View::OnDraw(CDC* pDC)
{
	int ORANGE = RGB(255, 153, 51);
	int YELLOW = RGB(255, 255, 0);
	int BLUE = RGB(104, 154, 255);
	int PINK = RGB(255, 153, 204);
	int PURPLE = RGB(153, 0, 204);
	int GREEN = RGB(50, 205, 50);
	int RED = RGB(255, 0, 0);

	int base = 50;

	Ctest3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect(0, 0, base * 20, base * 20);
	CBrush background(RGB(190, 190, 190));
	pDC->FillRect(rect, &background);
	pDC->SetBkColor(RGB(150, 150, 150));

	//narandzasti trougao
	DrawTriangle(pDC, { base, (int) (5.5 * base) }, { 7 * base, (int) (5.5 * base) }, { 7 * base, (int) (11.5 * base) }, YELLOW, ORANGE);

	//zeleni trougao
	DrawTriangle(pDC, { 7 * base, (int)(5.5 * base) }, { 10 * base, (int) (8.5 * base) }, {7 * base, (int) (8.5 * base)}, YELLOW, GREEN);

	//roze kvadrat
	DrawSquare(pDC, { 7 * base, (int)(8.5 * base) }, sqrt(2) * base * 3, YELLOW, PINK);

	//crveni trougao
	DrawTriangle(pDC, { 10 * base, (int)(8.5 * base) }, { 13 * base, (int)(5.5 * base) }, { 13 * base, (int)(11.5 * base) }, YELLOW, RED);

	//zuti trougao
	DrawTriangle(pDC, { 13 * base, (int)(5.5 * base) }, { 19 * base, (int)(5.5 * base)}, {13 * base, (int)(11.5 * base)}, YELLOW, YELLOW);

	//ljubicasti paralelogram
	POINT points[] = { { 10 * base, (int)(8.5 * base) }, { 13 * base, (int)(11.5 * base) }, { 13 * base, (int)(14.5 * base) }, { 10 * base, (int)(11.5 * base) } };
	DrawShape(pDC, points, 4, YELLOW, PURPLE);

	//srafirani trougao
	COLORREF bckPrev = pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetBrushOrg(-5, 0);
	DrawTriangle(pDC, { 7 * base, (int)(11.5 * base) }, { 10 * base, (int)(11.5 * base) }, { 7 * base, (int)(14.5 * base) }, YELLOW, { BS_HATCHED, (COLORREF)BLUE, HS_CROSS });
	pDC->SetBrushOrg(0, 0);
	pDC->SetBkColor(bckPrev);

	CPen polygonsPen(PS_SOLID | PS_GEOMETRIC, 2, YELLOW);
	CPen* prevPen = pDC->SelectObject(&polygonsPen);
	CBrush* prevBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
	//narandzasti trougao
	DrawInscribedPolygon(pDC, { 7 * base, (int)(5.5 * base) }, { base, (int)(5.5 * base) }, { 7 * base, (int)(11.5 * base) }, 8);

	//zeleni trougao
	DrawInscribedPolygon(pDC, { 7 * base, (int)(8.5 * base) }, { 10 * base, (int)(8.5 * base) }, { 7 * base, (int)(5.5 * base) }, 4);

	//crveni trougao
	DrawInscribedPolygon(pDC, { 10 * base, (int)(8.5 * base) }, { 13 * base, (int)(5.5 * base) }, { 13 * base, (int)(11.5 * base) }, 6);

	//srafirani trougao
	DrawInscribedPolygon(pDC, { 7 * base, (int)(11.5 * base) }, { 10 * base, (int)(11.5 * base) }, { 7 * base, (int)(14.5 * base) }, 5);

	pDC->SelectObject(prevPen);
	pDC->SelectObject(prevBrush);

	if (this->gridOn) {
		DrawGrid(pDC, { 0, 0 }, base * 20, base, RGB(255, 255, 255));
	}
}

double Ctest3View::GetDistance(POINT a, POINT b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void Ctest3View::DrawInscribedPolygon(CDC* pDC, POINT c, POINT a, POINT b, int n) {
	double cLen = GetDistance(a, b);
	double aLen = GetDistance(c, b);
	double bLen = GetDistance(c, a);
	double sum = cLen + bLen + aLen;

	DrawRegularPolygon(pDC, (cLen * c.x + bLen * b.x + aLen * a.x) / sum, (cLen * c.y + bLen * b.y + aLen * a.y) / sum, max(cLen, bLen, aLen) / 9, n, 0);
}

void Ctest3View::DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c) {
	POINT vertices[] = { a, b, c };
	pDC->Polygon(vertices, 3);
}

void Ctest3View::DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF color, tagLOGBRUSH brush) {
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CBrush newBrush;
	newBrush.CreateBrushIndirect(&brush);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);
	DrawTriangle(pDC, a, b, c);
	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void Ctest3View::DrawShape(CDC* pDC, POINT points[], int numOfPoints, COLORREF color, COLORREF fill) {
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);
	pDC->Polygon(points, numOfPoints);
	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void Ctest3View::DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF color, COLORREF fill) {
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);
	DrawTriangle(pDC, a, b, c);
	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void Ctest3View::DrawSquare(CDC* pDC, POINT a, int width, COLORREF color, COLORREF fill, float rotAngle) {
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);
	DrawRegularPolygon(pDC, a.x + ceil(width / (sqrt(2) * 2)), a.y + ceil(width / (sqrt(2) * 2)), width / 2, 4, rotAngle + 45);
	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void Ctest3View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	double rotAngleRad = RAD(rotAngle);
	double interceptAngleChange = 2 * M_PI / n;

	POINT* vertices = new POINT[n];

	for (auto i = 0; i < n; i++) {
		vertices[i].x = cx + r * cos(rotAngleRad);
		vertices[i].y = cy + r * sin(rotAngleRad);
		rotAngleRad += interceptAngleChange;
	}

	pDC->Polygon(vertices, n);
	delete[] vertices;
}

void Ctest3View::DrawGrid(CDC* pDC, POINT start, int size, int base, COLORREF color) {
	CPen newPen(PS_SOLID, 0, color);
	CPen* prevPen = pDC->SelectObject(&newPen);

	int gridSize = size / base;
	POINT* points = new POINT[gridSize * 4 + 4];
	DWORD* lpPolyPoints = new DWORD[gridSize * 2 + 2];

	for (int i = 0; i <= gridSize; i++) {
		points[i * 4] = { start.x + i * base, start.y };
		points[i * 4 + 1] = { start.x + i * base, start.y + size };
		points[i * 4 + 2] = { start.x, start.y + i * base };
		points[i * 4 + 3] = { start.x + size, start.y + i * base };

		lpPolyPoints[i] = lpPolyPoints[gridSize + i + 1] = 2;
	}

	pDC->PolyPolyline(points, lpPolyPoints, gridSize * 2 + 2);
	pDC->SelectObject(prevPen);

	delete[] points;
	delete[] lpPolyPoints;
}

void Ctest3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Ctest3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Ctest3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Ctest3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Ctest3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Ctest3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Ctest3View diagnostics

#ifdef _DEBUG
void Ctest3View::AssertValid() const
{
	CView::AssertValid();
}

void Ctest3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ctest3Doc* Ctest3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctest3Doc)));
	return (Ctest3Doc*)m_pDocument;
}
#endif //_DEBUG


// Ctest3View message handlers


void Ctest3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (isspace(nChar)) {
		this->gridOn = !this->gridOn;
		InvalidateRect(NULL);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
