
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
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIND17975View construction/destruction

CIND17975View::CIND17975View() noexcept
{
	darkCactus = GetEnhMetaFile(L"./res/cactus_part.emf");
	lightCactus = GetEnhMetaFile(L"./res/cactus_part_light.emf");

	potJoin = { 10 * BASE, 17 * BASE };
	midJoin = { potJoin.x, long(potJoin.y - BASE * fcHeightFactor) };
	rightJoin = { 
		long(midJoin.x + sinf(RAD(45)) * BASE * rcHeightFactor), 
		long(midJoin.y - cosf(RAD(45)) * BASE * rcHeightFactor) 
	};
	rightUJoin = {
		rightJoin.x,
		long(rightJoin.y - scHeightFactor * BASE)
	};
	rightRJoin = {
		long(rightJoin.x + scHeightFactor * BASE),
		rightJoin.y
	};
	rightDJoin = {
		long(rightJoin.x + sinf(RAD(45)) * BASE * scHeightFactor),
		long(rightJoin.y - cosf(RAD(45)) * BASE * scHeightFactor)
	};
}

CIND17975View::~CIND17975View()
{
	DeleteEnhMetaFile(lightCactus);
	DeleteEnhMetaFile(darkCactus);
}

BOOL CIND17975View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CIND17975View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply) {
	XFORM xForm = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = { sX, 0, 0, sY };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float angleRad = RAD(angle);
	float sinA = sinf(angleRad), cosA = cosf(angleRad);
	XFORM xForm = { cosA, -sinA, sinA, cosA };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::TRT(CDC* pDC, float dX, float dY, float angle, bool rightMultiply)
{
	if (rightMultiply) 
	{
		Translate(pDC, -dX, -dY, rightMultiply);
		Rotate(pDC, angle, rightMultiply);
		Translate(pDC, dX, dY, rightMultiply);
	}
	else 
	{
		Translate(pDC, dX, dY, rightMultiply);
		Rotate(pDC, angle, rightMultiply);
		Translate(pDC, -dX, -dY, rightMultiply);
	}
}

void CIND17975View::Grid(CDC* pDC)
{
	CPen newPen(PS_SOLID, 2, GRID);
	CPen* prevPen = pDC->SelectObject(&newPen);

	POINT* points = new POINT[GRID_LINES * 4];
	DWORD* lpPolyPoints = new DWORD[GRID_LINES * 2];

	int coord = 0;
	for (int i = 0; i < GRID_LINES; i++) {
		points[i * 4] = { coord, 0 };
		points[i * 4 + 1] = { coord, GRID_LENGTH };
		points[i * 4 + 2] = { 0, coord };
		points[i * 4 + 3] = { GRID_LENGTH, coord };
		lpPolyPoints[i] = lpPolyPoints[GRID_LINES + i] = 2;
		coord += BASE;
	}

	pDC->PolyPolyline(points, lpPolyPoints, GRID_LINES * 2);
	pDC->SelectObject(prevPen);

	delete[] points;
	delete[] lpPolyPoints;
}

void CIND17975View::DrawFigure(CDC* pDC)
{
	XFORM tForm;
	pDC->GetWorldTransform(&tForm);

	TRT(pDC, potJoin.x, potJoin.y, rootAngle);

	//srednji spoj
	Translate(pDC, midJoin.x, midJoin.y);
	Rotate(pDC, 45);
	DrawCactucBCWS(pDC, darkCactus, rcWidthFactor, rcHeightFactor);
	Rotate(pDC, -90);
	DrawCactucBCWS(pDC, lightCactus, rcWidthFactor, rcHeightFactor);
	Rotate(pDC, -135);
	DrawCactucBCWS(pDC, darkCactus, fcWidthFactor, fcHeightFactor);
	Rotate(pDC, 180);
	Circle(pDC, { 0, 0 }, BASE, BLACK, DARK_GREEN);
	Translate(pDC, -midJoin.x, -midJoin.y);

	//kruzic kod vaze
	Translate(pDC, potJoin.x, potJoin.y);
	Circle(pDC, { 0, 0 }, BASE, BLACK, DARK_GREEN);
	Translate(pDC, -potJoin.x, -potJoin.y);

	//desni spoj
	TRT(pDC, rightJoin.x, rightJoin.y, branchAngle);

	Translate(pDC, rightJoin.x, rightJoin.y);
	DrawCactucBCWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, -45);
	DrawCactucBCWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, -45);
	DrawCactucBCWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, 90);
	Circle(pDC, { 0, 0 }, BASE, BLACK, DARK_GREEN);
	Translate(pDC, -rightJoin.x, -rightJoin.y);

	//desni spoj gore
	Translate(pDC, rightUJoin.x, rightUJoin.y);
	DrawCactucBCWS(pDC, darkCactus, fcWidthFactor, fcHeightFactor);
	Circle(pDC, { 0, 0 }, BASE, BLACK, DARK_GREEN);
	Translate(pDC, -rightUJoin.x, -rightUJoin.y);

	//krajnje desno 
	Translate(pDC, rightRJoin.x, rightRJoin.y);
	Rotate(pDC, -90);
	DrawCactucBCWS(pDC, darkCactus, fcWidthFactor, fcHeightFactor);
	Circle(pDC, { 0, 0 }, BASE, BLACK, DARK_GREEN);
	Rotate(pDC, 90);
	Translate(pDC, -rightRJoin.x, -rightRJoin.y);

	//desno po dijagonali
	Translate(pDC, rightDJoin.x, rightDJoin.y);
	DrawCactucBCWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, -45);
	DrawCactucBCWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, -45);
	DrawCactucBCWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, 90);
	Circle(pDC, { 0, 0 }, BASE, BLACK, DARK_GREEN);
	Translate(pDC, -rightJoin.x, -rightJoin.y);

	pDC->SetWorldTransform(&tForm);

	DrawPot(pDC);
}

void CIND17975View::DrawCactusBC(CDC* pDC, HENHMETAFILE hmf) {
	CRect rect(-(ceil(BASE / 2.0)), -BASE, ceil(BASE / 2.0), 0);
	PlayEnhMetaFile(pDC->m_hDC, hmf, &rect);
}

void CIND17975View::DrawCactucBCWS(CDC* pDC, HENHMETAFILE hmf, float sX, float sY)
{
	Scale(pDC, sX, sY);
	DrawCactusBC(pDC, hmf);
	Scale(pDC, 1.0 / sX, 1.0 / sY);
}

void CIND17975View::DrawPot(CDC* pDC)
{
	POINT vertices[] = {
		{int(8 * BASE + 0.5), int(18 * BASE + 0.5)},
		{int(12 * BASE + 0.5), int(18 * BASE + 0.5)},
		{int(11.5 * BASE + 0.5), GRID_LENGTH },
		{int(8.5 * BASE + 0.5), GRID_LENGTH }
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

void CIND17975View::WriteText(CDC* pDC, CString text, POINT location)
{
	LOGFONTW logFont {};
	//logFont.lfEscapement = -900;
	logFont.lfHeight = 35;
	//logFont.lfOrientation = -900;

	CFont font;
	font.CreateFontIndirectW(&logFont);

	CFont* prevFont = pDC->SelectObject(&font);
	COLORREF prevBk = pDC->SetBkMode(TRANSPARENT);
	COLORREF prevTextColor = pDC->SetTextColor(BLACK);

	pDC->TextOutW(location.x - 3, location.y - 1, text);
	pDC->SetTextColor(TEXT_COLOR);
	pDC->TextOutW(location.x - 5, location.y, text);

	pDC->SelectObject(prevFont);
	pDC->SetTextColor(prevTextColor);
	pDC->SetBkMode(prevBk);
}

void CIND17975View::Circle(CDC* pDC, int cx, int cy, int r, COLORREF color, COLORREF fill)
{
	CPen newPen(PS_SOLID, 0, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	int dif = 0.9 * r / 2.0 + 0.5;
	pDC->Ellipse(cx - dif, cy - dif, cx + dif, cy + dif);

	pDC->SelectObject(prevPen);
	pDC->SelectObject(prevBrush);
}

void CIND17975View::Circle(CDC* pDC, POINT center, int r, COLORREF color, COLORREF fill)
{
	Circle(pDC, center.x, center.y, r, color, fill);
}

void CIND17975View::OnDraw(CDC* pDC)
{
	CIND17975Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap memBM;
	memBM.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	memDC->SelectObject(&memBM);
	memDC->Rectangle(clientRect);

	CRect rect(0, 0, GRID_LENGTH, GRID_LENGTH);
	CBrush background(TURQUOISE);
	memDC->FillRect(rect, &background);

	CRgn clipRgn;
	clipRgn.CreateRectRgn(0, 0, rect.right, rect.bottom);
	memDC->SelectClipRgn(&clipRgn);

	int prevGraphicsMode = memDC->SetGraphicsMode(GM_ADVANCED);

	TRT(memDC, GRID_LENGTH / 2.0, GRID_LENGTH / 2.0, 90);
	DrawFigure(memDC);
	WriteText(memDC, L"17975 Stefan Stojadinovic", { BASE, BASE });

	memDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	memDC->SetGraphicsMode(prevGraphicsMode);

	if (gridOn) {
		Grid(memDC);
	}

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), memDC, 0, 0, SRCCOPY);

	memDC->DeleteDC();
	delete memDC;
}


// CIND17975View printing

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
	switch (nChar) {
	case 'G':
		gridOn = !gridOn;
		break;
	case 'Q':
		rootAngle += 5;
		break;
	case 'E':
		rootAngle -= 5;
		break;
	case 'A':
		branchAngle += 5;
		break;
	case 'D':
		branchAngle -= 5;
		break;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CIND17975View::OnEraseBkgnd(CDC* pDC)
{
	return true;
	//return CView::OnEraseBkgnd(pDC);
}
