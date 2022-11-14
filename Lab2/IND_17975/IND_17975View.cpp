
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
	darkCactus = GetEnhMetaFile(L"./cactus_part.emf");
	lightCactus = GetEnhMetaFile(L"./cactus_part_light.emf");

	potJoin = { 10 * BASE, 17 * BASE };
	middleJoin = { potJoin.x, long(potJoin.y - BASE * fcHeightFactor) };
	rightJoin = { 
		long(middleJoin.x + sinf(RAD(45)) * BASE * scHeightFactor), 
		long(middleJoin.y - cosf(RAD(45)) * BASE * scHeightFactor) 
	};
	farRightJoin = {
		long(rightJoin.x + rcHeightFactor * BASE),
		rightJoin.y
	};
	leftJoin = {
		long(middleJoin.x - sinf(RAD(45)) * BASE * scHeightFactor),
		farRightJoin.y
	};
	topLeftJoin = {
		leftJoin.x,
		long(leftJoin.y - rcHeightFactor * BASE)
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
	Translate(pDC, dX, dY, rightMultiply);
	Rotate(pDC, angle, rightMultiply);
	Translate(pDC, -dX, -dY, rightMultiply);
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
	Translate(pDC, middleJoin.x, middleJoin.y);
	DrawCactucBMWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, 45);
	DrawCactucBMWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, -90);
	DrawCactucBMWS(pDC, darkCactus, scWidthFactor, scHeightFactor);
	Rotate(pDC, -135);
	DrawCactucBMWS(pDC, darkCactus, fcWidthFactor, fcHeightFactor);
	Rotate(pDC, 180);
	Translate(pDC, -middleJoin.x, -middleJoin.y);
	
	//krajnji desni spoj
	Translate(pDC, farRightJoin.x, farRightJoin.y);
	Rotate(pDC, 90);
	DrawCactucBMWS(pDC, darkCactus, rcWidthFactor, rcHeightFactor);
	Rotate(pDC, 135);
	DrawCactucBMWS(pDC, darkCactus, rcWidthFactor, rcHeightFactor);
	Rotate(pDC, 90);
	DrawCactucBMWS(pDC, darkCactus, rcWidthFactor, rcHeightFactor);
	Rotate(pDC, 45);
	Translate(pDC, -farRightJoin.x, -farRightJoin.y);

	//levi spoj
	Translate(pDC, leftJoin.x, leftJoin.y);
	DrawCactucBMWS(pDC, darkCactus, rcWidthFactor, rcHeightFactor);
	Rotate(pDC, 90);
	DrawCactucBMWS(pDC, darkCactus, rcWidthFactor, rcHeightFactor);
	Rotate(pDC, -90);
	Translate(pDC, -leftJoin.x, -leftJoin.y);

	//levi gore
	Translate(pDC, topLeftJoin.x, topLeftJoin.y);
	DrawCactucBMWS(pDC, darkCactus, fcWidthFactor, fcHeightFactor);
	Translate(pDC, -topLeftJoin.x, -topLeftJoin.y);

	Circle(pDC, topLeftJoin, BASE, BLACK, DARK_GREEN);
	Circle(pDC, leftJoin, BASE, BLACK, DARK_GREEN);
	Circle(pDC, potJoin, BASE, BLACK, DARK_GREEN);
	Circle(pDC, farRightJoin, BASE, BLACK, DARK_GREEN);
	Circle(pDC, middleJoin, BASE, BLACK, DARK_GREEN);

	Translate(pDC, rightJoin.x, rightJoin.y);
	Rotate(pDC, branchAngle);
	DrawCactucBMWS(pDC, lightCactus, rcWidthFactor, rcHeightFactor);
	Translate(pDC, -rightJoin.x, -rightJoin.y);

	Circle(pDC, rightJoin, BASE, BLACK, DARK_GREEN);

	ModifyWorldTransform(pDC->m_hDC, NULL, MWT_IDENTITY);

	DrawPot(pDC);

	pDC->SetWorldTransform(&tForm);
}

void CIND17975View::DrawCactusBM(CDC* pDC, HENHMETAFILE hmf) {
	CRect rect(0, 0, BASE, BASE);
	Translate(pDC, -BASE / 2.0, -BASE);
	PlayEnhMetaFile(pDC->m_hDC, hmf, &rect);
	Translate(pDC, BASE / 2.0, BASE);
}

void CIND17975View::DrawCactucBMWS(CDC* pDC, HENHMETAFILE hmf, float sX, float sY)
{
	Scale(pDC, sX, sY);
	DrawCactusBM(pDC, hmf);
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
	logFont.lfEscapement = -900;
	logFont.lfHeight = 40;

	CFont font;
	font.CreateFontIndirectW(&logFont);

	CFont* prevFont = pDC->SelectObject(&font);
	COLORREF prevBk = pDC->SetBkMode(TRANSPARENT);
	COLORREF prevTextColor = pDC->SetTextColor(BLACK);

	pDC->TextOutW(location.x + 2, location.y + 2, text);
	pDC->SetTextColor(TEXT_COLOR);
	pDC->TextOutW(location.x, location.y, text);

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

	DrawFigure(memDC);

	memDC->SetGraphicsMode(prevGraphicsMode);

	WriteText(memDC, L"17975 Stefan Stojadinovic", { GRID_LENGTH - BASE, BASE });

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
	// TODO: Add your message handler code here and/or call default
	return true;
	//return CView::OnEraseBkgnd(pDC);
}
