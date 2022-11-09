
// IND_17975View.cpp : implementation of the CIND17975View class
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

// CIND17975View

IMPLEMENT_DYNCREATE(CIND17975View, CView)

BEGIN_MESSAGE_MAP(CIND17975View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIND17975View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND17975View construction/destruction

CIND17975View::CIND17975View() noexcept
{
	darkCactus = GetEnhMetaFile(L"./res/cactus_part.emf");
	lightCactus = GetEnhMetaFile(L"./res/cactus_part_light.emf");
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

// CIND17975View drawing

void CIND17975View::OnDraw(CDC* pDC)
{
	CIND17975Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect(0, 0, GRID_LENGTH, GRID_LENGTH);
	CBrush background(TURQUOISE);
	pDC->FillRect(rect, &background);
	
	pDC->SetGraphicsMode(GM_ADVANCED);

	DrawFigure(pDC);

	if (this->gridActive) {
		Grid(pDC);
	}
}

void CIND17975View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply) {
	XFORM xForm = {1, 0, 0, 1, dX, dY};
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float angleRad = RAD(angle);
	float sinA = sinf(angleRad), cosA = cosf(angleRad);
	XFORM xForm = { cosA, -sinA, sinA, cosA, 0, 0 };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::TRT(CDC* pDC, POINT origin, float angle, bool rightMultiply)
{
	Translate(pDC, origin.x, origin.y, rightMultiply);
	Rotate(pDC, angle, rightMultiply);
	Translate(pDC, -origin.x, -origin.y, rightMultiply);
}

void CIND17975View::DrawFigure(CDC* pDC)
{
	RECT rect;
	TRT(pDC, { 10 * BASE, 17 * BASE }, rootAngle);

	Translate(pDC, 8.8 * BASE, 14.1 * BASE);
	PlayEnhMetaFile(pDC->m_hDC, lightCactus, &BIG);
	Translate(pDC, -8.8 * BASE, -14.1 * BASE);

	//tri sitna
	Translate(pDC, 9.5 * BASE, 10.8 * BASE + 0.5);

	PlayEnhMetaFile(pDC->m_hDC, darkCactus, &SLIM);

	TRT(pDC, { int(0.5 * BASE), int(3.2 * BASE) }, 45);
	PlayEnhMetaFile(pDC->m_hDC, darkCactus, &SLIM);

	TRT(pDC, { int(0.5 * BASE), int(3.2 * BASE) }, -90);
	PlayEnhMetaFile(pDC->m_hDC, darkCactus, &SLIM);

	//TRT(pDC, { int(0.5 * BASE), int(3.2 * BASE) }, 45);

	Translate(pDC, SLIM.right, -SLIM.bottom);
	PlayEnhMetaFile(pDC->m_hDC, lightCactus,)

	Translate(pDC, -9.5 * BASE, -10.8 * BASE - 0.5);

	Circle(pDC, 10 * BASE, 17 * BASE, BASE, BLACK, DARK_GREEN);
	Circle(pDC, 10 * BASE, 14 * BASE, BASE, BLACK, DARK_GREEN);

	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	DrawPot(pDC);
}

void CIND17975View::DrawPot(CDC* pDC)
{
	POINT vertices[] = {
		{int(8 * BASE + 0.5), int(18 * BASE + 0.5)},
		{int(12 * BASE + 0.5), int(18 * BASE + 0.5)},
		{int(11.5 * BASE + 0.5), GRID_LENGTH - 1 },
		{int(8.5 * BASE + 0.5), GRID_LENGTH - 1}
	};

	CPen newPen(PS_SOLID, 0, POT_EDGE);
	CBrush newBrush(POT_COLOR);

	CBrush* oldBrush = pDC->SelectObject(&newBrush);
	CPen* oldPen = pDC->SelectObject(&newPen);

	pDC->Polygon(vertices, 4);
	pDC->Rectangle(int(7.6 * BASE + 0.5), int(17.2 * BASE + 0.5), int(12.4 * BASE + 0.5), int(18 * BASE + 0.5));

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CIND17975View::Circle(CDC* pDC, int cx, int cy, int r, COLORREF color, COLORREF fill)
{
	CPen newPen(PS_SOLID, 0, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	int dif = r / 2.0 + 0.5;
	pDC->Ellipse(cx - dif, cy - dif, cx + dif, cy + dif);

	pDC->SelectObject(prevPen);
	pDC->SelectObject(prevBrush);
}

void CIND17975View::Grid(CDC* pDC) {
	CPen newPen(PS_SOLID, 2, GRID);
	CPen* prevPen = pDC->SelectObject(&newPen);

	int gridLines = GRID_SIZE + 1;
	POINT* points = new POINT[gridLines * 4];
	DWORD* lpPolyPoints = new DWORD[gridLines * 2];

	int coord = 0;
	for (int i = 0; i < gridLines; i++) {
		points[i * 4] = { coord, 0 };
		points[i * 4 + 1] = { coord, GRID_LENGTH };
		points[i * 4 + 2] = { 0, coord };
		points[i * 4 + 3] = { GRID_LENGTH, coord };
		lpPolyPoints[i] = lpPolyPoints[gridLines + i] = 2;
		coord += BASE;
	}

	pDC->PolyPolyline(points, lpPolyPoints, gridLines * 2);
	pDC->SelectObject(prevPen);

	delete[] points;
	delete[] lpPolyPoints;
}


// CIND17975View printing


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


// CIND17975View diagnostics

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


// CIND17975View message handlers


void CIND17975View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (isspace(nChar)) {
		this->gridActive = !this->gridActive;
		Invalidate();
	}
	else if (nChar == 'L') {
		this->rootAngle += 5;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
