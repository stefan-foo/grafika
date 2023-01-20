
// KOL1_12View.cpp : implementation of the CKOL112View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KOL1_12.h"
#endif

#include "KOL1_12Doc.h"
#include "KOL1_12View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

#define RAD(deg) ((deg) * M_PI / 180)
// CKOL112View

IMPLEMENT_DYNCREATE(CKOL112View, CView)

BEGIN_MESSAGE_MAP(CKOL112View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKOL112View construction/destruction

CKOL112View::CKOL112View() noexcept
{
	// TODO: add construction code here
	prevT = {};
}

CKOL112View::~CKOL112View()
{
}

BOOL CKOL112View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CKOL112View::Translate(CDC* pDC, float x, float y)
{
	XFORM xForm = { 1, 0, 0, 1, x, y };
	pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
}

void CKOL112View::Rotate(CDC* pDC, float angle)
{
	float aRad = RAD(angle);
	XFORM xForm = {};
	xForm.eM11 = xForm.eM22 = cosf(aRad);
	xForm.eM12 = -(xForm.eM21 = sinf(aRad));
	pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
}

void CKOL112View::Trapez(CDC* pDC, int a, int b, int h)
{
	POINT vertices[4] = {
		{ -b / 2, -h / 2 },
		{ -a / 2, h / 2 },
		{ a / 2, h / 2 },
		{ b / 2, -h / 2 }
	};
	pDC->Polygon(vertices, 4);
}

void CKOL112View::DrawScene(CDC* pDC)
{
	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);

	//Translate(pDC, 300, 300);

	XFORM saveT;
	pDC->GetWorldTransform(&saveT);

	pDC->Rectangle(100, 100, 400, 300);

	//Translate()

	pDC->GetWorldTransform(&saveT);

	Translate(pDC, 0, -0.2 * 100);
	Shear(pDC, 0, -0.5);
	pDC->Rectangle(100, 100, 140, 200);
	pDC->SetWorldTransform(&saveT);

	pDC->Rectangle(140, 100 + 0.2 * 40, 180, 200 + 0.2 * 40);
	
	//int spokeCount = 20, spokeWidth = 20;
	//float r1 = 1.4 * 1.5 * spokeCount * spokeWidth / (2 * M_PI);
	//Rotate(pDC, -angle);
	//DrawGear(pDC, spokeCount, spokeWidth);

	//pDC->SetWorldTransform(&saveT);

	//spokeCount = 10, spokeWidth = 20;
	//float r2 = 1.4 * 1.5 * spokeCount * spokeWidth / (2 * M_PI);
	//Rotate(pDC, 135);
	//Translate(pDC, r1 + r2, 0);
	//Rotate(pDC, -127 + angle * 2);
	//DrawGear(pDC, spokeCount, spokeWidth);
	//
	//pDC->SetWorldTransform(&saveT);

	//Translate(pDC, r1 + r2, 0);
	//Rotate(pDC, angle * 2);
	//DrawGear(pDC, spokeCount, spokeWidth);

	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	pDC->SetGraphicsMode(prevMode);
}

void CKOL112View::Shear(CDC* pDC, float sX, float sY) {
	XFORM x = { 1, sX, sY, 1 };
	pDC->ModifyWorldTransform(&x, MWT_LEFTMULTIPLY);
}

void CKOL112View::DrawGear(CDC* pDC, int spokeCount, int spokeWidth)
{
	float r = 1.4 * 1.5 * spokeCount * spokeWidth / (2 * M_PI);
	float angleDif = 360.0 / spokeCount;

	CBrush inner(RGB(0, 150, 200));
	CPen solidPen(PS_SOLID, 2, RGB(0, 0, 50));
	CPen penEf(PS_DASH, 0, RGB(100, 100, 100));
	
	CBrush* prevBrush = pDC->SelectObject(&inner);
	CPen* prevPen = pDC->SelectObject(&solidPen);

	pDC->SelectStockObject(HOLLOW_BRUSH);
	pDC->SelectObject(&penEf);
	pDC->Ellipse(CRect(-r, -r, r, r));

	pDC->SelectObject(&inner);
	pDC->SelectObject(&solidPen);

	XFORM saveT;
	pDC->GetWorldTransform(&prevT);
	for (auto i = 0; i < spokeCount; i++) {
		Translate(pDC, 0, -r);
		Trapez(pDC, 1.4 * spokeWidth, spokeWidth / 1.4, spokeWidth);
		Translate(pDC, 0, r);
		Rotate(pDC, angleDif);
	}
	pDC->SetWorldTransform(&prevT);

	r -= spokeWidth * 0.5 - 0.5;
	pDC->SelectObject(&inner);
	pDC->SelectObject(&solidPen);
	pDC->Ellipse(CRect(-r, -r, r, r));

	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	LOGFONTW logFont = {};
	logFont.lfHeight = 18;
	wcscpy_s(logFont.lfFaceName, 32, L"Times New Roman");
	CFont font;
	//font.CreateFontW()
	font.CreateFontIndirectW(&logFont);
	pDC->SelectObject(&font);

	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&blackPen);
	pDC->Rectangle(-spokeWidth / 2, -spokeWidth / 2, spokeWidth / 2, spokeWidth / 2);
	pDC->TextOutW(0, spokeWidth / 2, CString((std::to_string(spokeCount) + " zubaca").c_str()));
}

// CKOL112View drawing

void CKOL112View::OnDraw(CDC* pDC)
{
	CKOL112Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	CDC *memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	memDC->SelectObject(&bmp);
	CBrush white(RGB(255, 255, 255));
	memDC->FillRect(clientRect, &white);

	DrawScene(memDC);

	pDC->BitBlt(0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	bmp.DeleteObject();
	delete memDC;
}


// CKOL112View printing

BOOL CKOL112View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKOL112View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKOL112View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKOL112View diagnostics

#ifdef _DEBUG
void CKOL112View::AssertValid() const
{
	CView::AssertValid();
}

void CKOL112View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKOL112Doc* CKOL112View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOL112Doc)));
	return (CKOL112Doc*)m_pDocument;
}
#endif //_DEBUG


// CKOL112View message handlers


void CKOL112View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP) {
		angle += 1;
		Invalidate();
	}
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKOL112View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	//return CView::OnEraseBkgnd(pDC);
}
