
// lab1View.cpp : implementation of the Clab1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_17975.h"
#endif

#include "IND_17975Doc.h"
#include "IND_17975View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD(degrees) ((degrees) * M_PI / 180) 
#define DEG(radians) ((radians) * 180 / M_PI)

// CIND_17975View

IMPLEMENT_DYNCREATE(CIND17975View, CView)

BEGIN_MESSAGE_MAP(CIND17975View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIND17975View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab1View construction/destruction

CIND17975View::CIND17975View() noexcept
{
	this->gridOn = true;
}

CIND17975View::~CIND17975View()
{
}

BOOL CIND17975View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Clab1View drawing
void CIND17975View::OnDraw(CDC* pDC)
{
	CIND17975Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect(0, 0, base * gridSize, base * gridSize);
	CBrush background(grey);
	pDC->FillRect(rect, &background);

	CPen* prevPen = pDC->SelectObject(new CPen(PS_SOLID, 5, purple));
	//crveni trougao
	InscribedSolidTriangle(pDC, 
		{ base, long(13 * base) }, 
		{ long(4.2 * base + 0.5), long(9.5 * base + 0.5) }, 
		{ long(4.2 * base + 0.5), long(16.2 * base + 0.5) }, red, 6);
	//narandzasti kvadrat
	Square(pDC, 
		{ long(4.2 * base + 0.5), long(9.5 * base + 0.5) }, 
		long(base * 3.5 + 0.5), orange);
	//zeleni trougao
	InscribedSolidTriangle(pDC, 
		{ long(7.6 * base + 0.5), long(9.5 * base + 0.5) }, 
		{ long(7.6 * base + 0.5), long(16.2 * base + 0.5) }, 
		{ long(14.2  * base + 0.5), long(9.5 * base + 0.5) }, green, 4);
	//roze trougao
	InscribedSolidTriangle(pDC, 
		{ long(7.6 * base + 0.5), long(9.5 * base + 0.5) }, 
		{ 17 * base, (long)(9.5 * base + 0.5)}, 
		{ long(12.5 * base + 0.5), long(4.8 * base + 0.5) }, pink, 5);
	//ljubicasti trougao
	InscribedSolidTriangle(pDC, 
		{ long(7.6 * base + 0.5), long(16.2 * base + 0.5) }, 
		{ long(10 * base + 0.5), long(13.7 * base + 0.5) }, 
		{ 10 * base, long(18.5 * base + 0.5) }, darkPurple, 7);
	//zuti paralelogram
	SolidQuadrilateral(pDC, 
		{ long(12.5 * base + 0.5), long(4.8 * base + 0.5) }, 
		{ long(15.7 * base + 0.5), long(4.8 * base + 0.5) }, 
		{ long(18.9 * base + 0.5), long(1.5 * base + 0.5) }, 
		{ long(15.7 * base + 0.5), long(1.5 * base + 0.5) }, yellow);
	//srafirani trougao
	InscribedHatchedTriangle(pDC, 
		{ long(15.8 * base + 0.5), long(9.5 * base + 0.5) }, 
		{ 19 * base, long(9.5 * base + 0.5) }, 
		{ 19 * base, long(12.8 * base + 0.5) }, white, blue, HS_HORIZONTAL, 8);

	if (this->gridOn) {
		Grid(pDC);
	}

	delete pDC->SelectObject(prevPen);
}

double CIND17975View::GetDistance(POINT a, POINT b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void CIND17975View::Triangle(CDC* pDC, POINT a, POINT b, POINT c) {
	POINT vertices[] = { a, b, c };
	pDC->Polygon(vertices, 3);
}

void CIND17975View::SolidTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF fill) {
	CBrush* prevBrush = pDC->SelectObject(new CBrush(fill));
	Triangle(pDC, a, b, c);
	delete pDC->SelectObject(prevBrush);
}

void CIND17975View::HatchedTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF bck, COLORREF color, ULONG_PTR hatch) {
	COLORREF prevBck = pDC->SetBkColor(bck);
	CBrush* prevBrush = pDC->SelectObject(new CBrush(hatch, color));
	Triangle(pDC, a, b, c);
	pDC->SetBkColor(prevBck);
	delete pDC->SelectObject(prevBrush);
}

void CIND17975View::InscribedSolidTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF fill, int inscribedPolygonVertices) {
	SolidTriangle(pDC, a, b, c, fill);
	InscribedPolygon(pDC, a, b, c, inscribedPolygonVertices);
}

void CIND17975View::InscribedHatchedTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF bck, COLORREF color, ULONG_PTR hatch, int inscribedPolygonVertices) {
	HatchedTriangle(pDC, a, b, c, bck, color, hatch);
	InscribedPolygon(pDC, a, b, c, inscribedPolygonVertices);
}

void CIND17975View::Square(CDC* pDC, POINT a, int width, COLORREF fill, float rotAngle) {
	CBrush newBrush(fill);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);
	DrawRegularPolygon(pDC, a.x + 0.5 + width / 2.0, a.y + 0.5 + width / 2.0, width * sqrt(2) / 2, 4, rotAngle + 45);
	pDC->SelectObject(prevBrush);
}

void CIND17975View::Quadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d) {
	POINT vertices[] = { a, b, c, d };
	pDC->Polygon(vertices, 4);
}

void CIND17975View::SolidQuadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d, COLORREF fill) {
	CBrush* prevBrush = pDC->SelectObject(new CBrush(fill));
	Quadrilateral(pDC, a, b, c, d);
	delete pDC->SelectObject(prevBrush);
}

void CIND17975View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	double rotAngleRad = RAD(rotAngle);
	double interceptAngleChange = 2 * M_PI / n;

	POINT* vertices = new POINT[n];

	for (auto i = 0; i < n; i++) {
		vertices[i].x = (long)(cx + r * cos(rotAngleRad));
		vertices[i].y = (long)(cy + r * sin(rotAngleRad));
		rotAngleRad += interceptAngleChange;
	}

	pDC->Polygon(vertices, n);
	delete[] vertices;
}

void CIND17975View::DrawRegularPolygon(CDC* pDC, POINT center, int r, int n, float rotAngle) {
	DrawRegularPolygon(pDC, center.x, center.y, r, n, rotAngle);
}

void CIND17975View::InscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n) {
	double cLen = GetDistance(a, b);
	double aLen = GetDistance(c, b);
	double bLen = GetDistance(c, a);

	CPen* prevPen = pDC->GetCurrentPen();

	LOGPEN logPen;
	prevPen->GetLogPen(&logPen);

	CPen newPen(PS_SOLID, 2, logPen.lopnColor);
	pDC->SelectObject(&newPen);
	CBrush* prevBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);

	double sum = cLen + bLen + aLen;
	double s = sum / 2;
	double r = sqrt(s * (s - aLen) * (s - bLen) * (s - cLen)) / s;

	DrawRegularPolygon(pDC, (cLen * c.x + bLen * b.x + aLen * a.x) / sum + 0.5, (cLen * c.y + bLen * b.y + aLen * a.y) / sum + 0.5, r / 2 * 1.1, n, 0);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void CIND17975View::Grid(CDC* pDC) {
	CPen newPen(PS_SOLID, 2, gridGrey);
	CPen* prevPen = pDC->SelectObject(&newPen);

	int gridLength = gridSize * base, gridLines = gridSize + 1;
	POINT* points = new POINT[gridLines * 4];
	DWORD* lpPolyPoints = new DWORD[gridLines * 2];

	for (int i = 0; i < gridLines; i++) {
		points[i * 4] = { i * base, 0 };
		points[i * 4 + 1] = { i * base, gridLength };
		points[i * 4 + 2] = { 0, i * base };
		points[i * 4 + 3] = { gridLength, i * base };
		lpPolyPoints[i] = lpPolyPoints[gridLines + i] = 2;
	}

	pDC->PolyPolyline(points, lpPolyPoints, gridLines * 2);
	pDC->SelectObject(prevPen);

	delete[] points;
	delete[] lpPolyPoints;
}

void CIND17975View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIND17975View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND17975View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND17975View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIND17975View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIND17975View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Clab1View diagnostics

#ifdef _DEBUG
void CIND17975View::AssertValid() const
{
	CView::AssertValid();
}

void CIND17975View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND17975Doc* CIND17975View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND17975Doc)));
	return (CIND17975Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab1View message handlers



void CIND17975View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (isspace(nChar)) {
		this->gridOn = !this->gridOn;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
