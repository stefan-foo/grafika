
// KOL1_11View.cpp : implementation of the CKOL111View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KOL1_11.h"
#endif

#include "KOL1_11Doc.h"
#include "KOL1_11View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#define RAD(deg) ((deg) * M_PI / 180);

// CKOL111View

IMPLEMENT_DYNCREATE(CKOL111View, CView)

BEGIN_MESSAGE_MAP(CKOL111View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_KEYDOWN()
ON_WM_KEYDOWN()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKOL111View construction/destruction

CKOL111View::CKOL111View() noexcept
{
	clioEMF = GetEnhMetaFile(L"./res/clio.emf");
	wheelEMF = GetEnhMetaFile(L"./res/tocak.emf");
	skyImg.Load(L"./res/sky.jpg");
}

CKOL111View::~CKOL111View()
{
}

BOOL CKOL111View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CKOL111View::Translate(CDC* pDC, float dX, float dY)
{
	XFORM xForm = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
}

void CKOL111View::Rotate(CDC* pDC, float angle)
{
	float aRad = RAD(angle);
	XFORM xForm = {};
	xForm.eM11 = xForm.eM22 = cos(aRad);
	xForm.eM12 = -(xForm.eM21 = sin(aRad));
	pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
}

void CKOL111View::OnDraw(CDC* pDC)
{
	CKOL111Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	int w = clientRect.Width(), h = clientRect.Height();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, w, h);
	memDC.SelectObject(&bmp);

	DrawScene(&memDC);

	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	bmp.DeleteObject();
}

void CKOL111View::DrawScene(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	int x = clientRect.Width(), y = clientRect.Height();

	XFORM prevT;
	pDC->GetWorldTransform(&prevT);
	int prevGMode = pDC->SetGraphicsMode(GM_ADVANCED);

	skyImg.Draw(pDC, CRect(0, 0, skyImg.Width(), skyImg.Height()), clientRect);
	Translate(pDC, x / 2, y / 2);

	int cw = 450;
	int ch = 450 / 2.5;

	CBrush innerRB(RGB(255, 255, 255));
	CBrush outerRB(RGB(100, 100, 100));
	CBrush shadow(RGB(0, 0, 0));

	CRect outerRect(-cw / 2 - 40, -ch / 2 - 40, cw / 2 + 40, ch / 2 + 50);

	pDC->SelectObject(&shadow);
	pDC->Rectangle(- 100,  0, 100, y / 2);

	pDC->RoundRect(CRect(outerRect.left + 3, outerRect.top + 3, outerRect.right + 3, outerRect.bottom + 3), { 10, 10 });
	pDC->SelectObject(&outerRB);
	pDC->RoundRect(outerRect, { 10, 10 });

	pDC->SelectObject(&shadow);
	pDC->RoundRect(CRect(outerRect.left + 23, outerRect.top + 23, outerRect.right - 17, outerRect.bottom - 17), { 10, 10 });
	pDC->SelectObject(&innerRB);
	pDC->RoundRect(CRect(outerRect.left + 20, outerRect.top + 20, outerRect.right - 20, outerRect.bottom - 20), {  10, 10 });

	Translate(pDC, m_x, 0);
	DrawClio(pDC, cw, ch);

	pDC->SetWorldTransform(&prevT);
	pDC->SetGraphicsMode(prevGMode);
}

void CKOL111View::DrawClio(CDC* pDC, int w, int h)
{
	PlayEnhMetaFile(pDC->GetSafeHdc(), clioEMF, CRect(-w / 2, -h / 2, w / 2, h / 2));

	pDC->GetWorldTransform(&saveT);

	Translate(pDC, 157, 70);
	Rotate(pDC, rotAngle);
	DrawWheel(pDC);
	pDC->SetWorldTransform(&saveT);

	Translate(pDC, -135, 70);
	Rotate(pDC, rotAngle);
	DrawWheel(pDC);
	pDC->SetWorldTransform(&saveT);
}

void CKOL111View::DrawWheel(CDC* pDC)
{
	PlayEnhMetaFile(pDC->GetSafeHdc(), wheelEMF, CRect(-35, -35, 35, 35));
}

// CKOL111View printing

BOOL CKOL111View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKOL111View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKOL111View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKOL111View diagnostics

#ifdef _DEBUG
void CKOL111View::AssertValid() const
{
	CView::AssertValid();
}

void CKOL111View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKOL111Doc* CKOL111View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOL111Doc)));
	return (CKOL111Doc*)m_pDocument;
}
#endif //_DEBUG


// CKOL111View message handlers


void CKOL111View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT) {
		m_x -= 5;
	}
	else if (nChar == VK_RIGHT) {
		m_x += 5;
	}

	rotAngle = -m_x / 35.0 * 180 / M_PI;

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


//void CKOL111View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CView::OnKeyDown(nChar, nRepCnt, nFlags);
//}


BOOL CKOL111View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	//return CViews::OnEraseBkgnd(pDC);
}
