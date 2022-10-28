
// lab1View.cpp : implementation of the Clab1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab1.h"
#endif

#include "lab1Doc.h"
#include "lab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD(degrees) ((degrees) * M_PI / 180) 
#define DEG(radians) ((radians) * 180 / M_PI)

// Clab1View

IMPLEMENT_DYNCREATE(Clab1View, CView)

BEGIN_MESSAGE_MAP(Clab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Clab1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab1View construction/destruction

Clab1View::Clab1View() noexcept
{
	// TODO: add construction code here
	gridOn = true;
}

Clab1View::~Clab1View()
{
}

BOOL Clab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Clab1View drawing
void Clab1View::OnDraw(CDC* pDC)
{
	Clab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect(0, 0, base * (gridLines-1), base * (gridLines-1));
	CBrush background(grey);
	pDC->FillRect(rect, &background);

	CPen* pen = pDC->SelectObject(new CPen(PS_SOLID | PS_GEOMETRIC, 5, yellow));
	//narandzasti trougao
	InscribedSolidTriangle(pDC, { base, (int)(5.5 * base) }, { 7 * base, (int)(5.5 * base) }, { 7 * base, (int)(11.5 * base) }, orange, 8);
	//zeleni trougao
	InscribedSolidTriangle(pDC, { 7 * base, (int)(5.5 * base) }, { 10 * base, (int)(8.5 * base) }, { 7 * base, (int)(8.5 * base) }, green, 4);
	//roze kvadrat
	Square(pDC, { 7 * base, (int)(8.5 * base) }, base * 3, pink);
	//crveni trougao
	InscribedSolidTriangle(pDC, { 10 * base, (int)(8.5 * base) }, { 13 * base, (int)(5.5 * base) }, { 13 * base, (int)(11.5 * base) }, red, 6);
	//zuti trougao
	SolidTriangle(pDC, { 13 * base, (int)(5.5 * base) }, { 19 * base, (int)(5.5 * base) }, { 13 * base, (int)(11.5 * base) }, yellow);
	//ljubicasti paralelogram
	SolidQuadrilateral(pDC, { 10 * base, (int)(8.5 * base) }, { 13 * base, (int)(11.5 * base) }, { 13 * base, (int)(14.5 * base) }, { 10 * base, (int)(11.5 * base) }, purple);
	//srafirani trougao
	InscribedHatchedTriangle(pDC, { 7 * base, (int)(11.5 * base) }, { 10 * base, (int)(11.5 * base) }, { 7 * base, (int)(14.5 * base) }, white, blue, HS_CROSS, 5);

	if (this->gridOn) {
		Grid(pDC);
	}
}

double Clab1View::GetDistance(POINT a, POINT b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void Clab1View::Triangle(CDC* pDC, POINT a, POINT b, POINT c) {
	POINT vertices[] = { a, b, c };
	pDC->Polygon(vertices, 3);
}

void Clab1View::SolidTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF fill) {
	CBrush* prevBrush = pDC->SelectObject(new CBrush(fill));
	Triangle(pDC, a, b, c);
	delete pDC->SelectObject(prevBrush);
}

void Clab1View::HatchedTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF bck, COLORREF color, ULONG_PTR hatch) {
	COLORREF prevBck = pDC->SetBkColor(bck);
	CBrush* prevBrush = pDC->SelectObject(new CBrush(hatch, color));
	Triangle(pDC, a, b, c);
	pDC->SetBkColor(prevBck);
	delete pDC->SelectObject(prevBrush);
}

void Clab1View::InscribedSolidTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF fill, int inscribedPolygonVertices) {
	SolidTriangle(pDC, a, b, c, fill);
	InscribedPolygon(pDC, a, b, c, inscribedPolygonVertices);
}

void Clab1View::InscribedHatchedTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF bck, COLORREF color, ULONG_PTR hatch, int inscribedPolygonVertices) {
	HatchedTriangle(pDC, a, b, c, bck, color, hatch);
	InscribedPolygon(pDC, a, b, c, inscribedPolygonVertices);
}

void Clab1View::Square(CDC* pDC, POINT a, int width, COLORREF fill, float rotAngle) {
	CBrush newBrush(fill);
	width *= sqrt(2);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);
	DrawRegularPolygon(pDC, a.x + ceil(width / (sqrt(2) * 2)), a.y + ceil(width / (sqrt(2) * 2)), width / 2, 4, rotAngle + 45);
	pDC->SelectObject(prevBrush);
}

void Clab1View::Quadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d) {
	POINT vertices[] = { a, b, c, d };
	pDC->Polygon(vertices, 4);
}

void Clab1View::SolidQuadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d, COLORREF fill) {
	CBrush* prevBrush = pDC->SelectObject(new CBrush(fill));
	Quadrilateral(pDC, a, b, c, d);
	delete pDC->SelectObject(prevBrush);
}

void Clab1View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	double rotAngleRad = RAD(rotAngle);
	double interceptAngleChange = 2 * M_PI / n;

	POINT* vertices = new POINT[n];

	for (auto i = 0; i < n; i++) {
		vertices[i].x = (long) (cx + r * cos(rotAngleRad));
		vertices[i].y = (long) (cy + r * sin(rotAngleRad));
		rotAngleRad += interceptAngleChange;
	}

	pDC->Polygon(vertices, n);
	delete[] vertices;
}

void Clab1View::DrawRegularPolygon(CDC* pDC, POINT center, int r, int n, float rotAngle) {
	DrawRegularPolygon(pDC, center.x, center.y, r, n, rotAngle);
}

void Clab1View::InscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n) {
	double cLen = GetDistance(a, b);
	double aLen = GetDistance(c, b);
	double bLen = GetDistance(c, a);
	double sum = cLen + bLen + aLen;

	CPen* prevPen = pDC->GetCurrentPen();

	LOGPEN logPen;
	prevPen->GetLogPen(&logPen);
	CPen newPen(logPen.lopnColor, 2, logPen.lopnColor);
	pDC->SelectObject(&newPen);
	CBrush* prevBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);

	DrawRegularPolygon(pDC, (cLen * c.x + bLen * b.x + aLen * a.x) / sum + 0.5, (cLen * c.y + bLen * b.y + aLen * a.y) / sum + 0.5, max(max(cLen, aLen), bLen) / 9, n, 0);
	
	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void Clab1View::Grid(CDC* pDC) {
	CPen newPen(PS_SOLID, 0, white);
	CPen* prevPen = pDC->SelectObject(&newPen);
	int size = (gridLines - 1) * base;

	POINT* points = new POINT[gridLines * 4];
	DWORD* lpPolyPoints = new DWORD[gridLines * 2];


	for (int i = 0; i < gridLines; i++) {
		points[i * 4] = { i * base, 0 };
		points[i * 4 + 1] = { i * base, size };
		points[i * 4 + 2] = { 0, i * base };
		points[i * 4 + 3] = { size, i * base };
		lpPolyPoints[i] = lpPolyPoints[gridLines + i] = 2;
	}

	pDC->PolyPolyline(points, lpPolyPoints, gridLines * 2);
	pDC->SelectObject(prevPen);

	delete[] points;
	delete[] lpPolyPoints;
}
// Clab1View printing


void Clab1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Clab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Clab1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Clab1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Clab1View diagnostics

#ifdef _DEBUG
void Clab1View::AssertValid() const
{
	CView::AssertValid();
}

void Clab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab1Doc* Clab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab1Doc)));
	return (Clab1Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab1View message handlers


void Clab1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (isspace(nChar)) {
		this->gridOn = !this->gridOn;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
