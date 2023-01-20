
// KOL1_19View.cpp : implementation of the CKOL119View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KOL1_19.h"
#endif

#include "KOL1_19Doc.h"
#include "KOL1_19View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD(deg) ((deg) * M_PI / 180)

// CKOL119View

IMPLEMENT_DYNCREATE(CKOL119View, CView)

BEGIN_MESSAGE_MAP(CKOL119View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKOL119View construction/destruction

CKOL119View::CKOL119View() noexcept
{
	background.Load(L"./res/background.jpg");
	arm1.Load(L"./res/arm1.png");
	arm2.Load(L"./res/arm2.png");
	leg1.Load(L"./res/leg1.png");
	leg2.Load(L"./res/leg2.png");
	body.Load(L"./res/body1.png");
}

CKOL119View::~CKOL119View()
{
}

BOOL CKOL119View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CKOL119View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKOL119View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	XFORM xForm{};
	xForm.eM11 = xForm.eM22 = cosf(RAD(angle));
	xForm.eM21 = -(xForm.eM12 = sinf(RAD(angle)));
	pDC->ModifyWorldTransform(&xForm, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKOL119View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = { sX, 0, 0, sY };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKOL119View::DrawScene(CDC* pDC)
{
	CRect cr;
	GetClientRect(&cr);

	DrawBackground(pDC, cr);

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	Translate(pDC, 300, 300);
	Scale(pDC, 0.5, 0.5);
	DrawLeg2(pDC);
	Translate(pDC, 5, -65);
	DrawLeg1(pDC);	
	Translate(pDC, 211, -7);
	DrawBody(pDC);

	pDC->SetBkMode(TRANSPARENT);

	DrawArm2(pDC);
	Translate(pDC, 178, 54);
	DrawArm1(pDC);

	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
}

void CKOL119View::DrawBackground(CDC* pDC, CRect rc)
{
	background.Draw(pDC, CRect(0, 0, background.Width(), background.Height()), rc);
}

void CKOL119View::DrawImageTransperent(CDC* pDC, DImage* image)
{
	int w = image->Width();
	int h = image->Height();

	CBitmap imageBmp, maskBmp;
	maskBmp.CreateBitmap(w, h, 1, 1, NULL);
	imageBmp.CreateCompatibleBitmap(pDC, w, h);
	
	CDC srcDC, dstDC;
	srcDC.CreateCompatibleDC(pDC);
	dstDC.CreateCompatibleDC(pDC);
	srcDC.SelectObject(&imageBmp);
	dstDC.SelectObject(&maskBmp);

	image->Draw(&srcDC, CRect(0, 0, w, h), CRect(0, 0, w, h));

	byte* bits = image->GetDIBBits();
	COLORREF key = RGB(bits[2], bits[1], bits[0]);

	srcDC.SetBkColor(key);
	dstDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &dstDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, &dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCPAINT);

	srcDC.DeleteDC();
	dstDC.DeleteDC();
}

void CKOL119View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 34, 31);
	Rotate(pDC, -alpha1);
	Translate(pDC, -34, -31);
	DrawImageTransperent(pDC, &arm1);
}

void CKOL119View::DrawArm2(CDC* pDC)
{
	XFORM xForm{};
	pDC->GetWorldTransform(&xForm);

	Translate(pDC, 178, 54);
	Translate(pDC, 34, 31);
	Rotate(pDC, -alpha1);
	Translate(pDC, -34, -31);

	Translate(pDC, 210, 102);
	//Translate(pDC, 23, 61);
	Rotate(pDC, 180 - alpha5);

	Translate(pDC, -23, -61);
	DrawImageTransperent(pDC, &arm2);
	pDC->SetWorldTransform(&xForm);
	
}

void CKOL119View::DrawLeg1(CDC* pDC)
{
	XFORM xForm{};
	//pDC->GetWorldTransform(&xForm);

	Translate(pDC, 30, 125);
	Rotate(pDC, -alpha3);
	Translate(pDC, -30, -125);

	DrawImageTransperent(pDC, &leg1);

	//pDC->SetWorldTransform(&xForm);
}

void CKOL119View::DrawLeg2(CDC* pDC)
{
	XFORM xForm{};
	pDC->GetWorldTransform(&xForm);

	Translate(pDC, 35, 60);
	Rotate(pDC, alpha4);
	Translate(pDC, -35, -60);

	DrawImageTransperent(pDC, &leg2);

	pDC->SetWorldTransform(&xForm);
}

void CKOL119View::DrawBody(CDC* pDC)
{
	XFORM xForm{};
	//pDC->GetWorldTransform(&xForm);

	Translate(pDC, 26, 133);
	Rotate(pDC, -alpha2);
	Translate(pDC, -26, -133);

	DrawImageTransperent(pDC, &body);

	//pDC->SetWorldTransform(&xForm);
}

void CKOL119View::DrawAxes(CDC* pDC)
{
	CPen xPen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen yPen(PS_SOLID, 2, RGB(0, 0, 255));

	CPen* prevPen;
	POINT pos = pDC->GetCurrentPosition();

	prevPen = pDC->SelectObject(&xPen);
	pDC->MoveTo(0, 0);
	pDC->LineTo(100, 0);

	pDC->SelectObject(&yPen);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);

	pDC->SelectObject(prevPen);
	pDC->MoveTo(pos);
}

// CKOL119View drawing

void CKOL119View::OnDraw(CDC* pDC)
{
	CKOL119Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect cr;
	GetClientRect(&cr);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, cr.Width(), cr.Height());
	memDC->SelectObject(&bmp);

	DrawScene(memDC);

	pDC->BitBlt(0, 0, cr.Width(), cr.Height(), memDC, 0, 0, SRCCOPY);

	delete memDC;
}


// CKOL119View printing

BOOL CKOL119View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKOL119View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKOL119View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKOL119View diagnostics

#ifdef _DEBUG
void CKOL119View::AssertValid() const
{
	CView::AssertValid();
}

void CKOL119View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKOL119Doc* CKOL119View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOL119Doc)));
	return (CKOL119Doc*)m_pDocument;
}
#endif //_DEBUG


// CKOL119View message handlers


void CKOL119View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case 'A':
		alpha1 += 5;
		if (alpha1 > 270) alpha1 = 270;
		break;
	case 'Q':
		alpha1 -= 5;
		if (alpha1 < 0) alpha1 = 0;
		break;
	case 'T':
		alpha5 += 15;
		if (alpha5 > 180) alpha5 = 0;
		break;
	case 'G':
		alpha5 -= 15;
		if (alpha5 < 0) alpha5 = 0;
		break;
	case 'R':
		alpha4 += 5;
		break;
	case 'F':
		alpha4 -= 5;
		break;
	case 'W':
		alpha2 += 5;
		if (alpha2 > 90) alpha2 = 90;
		break;
	case 'S':
		alpha2 -= 5;
		if (alpha2 < 0) alpha2 = 0;
		break;
	case VK_LEFT:
		if (alpha3 < 110) {
			alpha3 += 5;
			alpha1 += 12.2;
			alpha2 -= 0.9;
			alpha4 += 5;
			alpha5 += 12.2;
		}
		else {
			alpha3 = 110;
			alpha1 = 270;
			alpha2 = -20;
			alpha4 = 110;
			alpha5 = 270;
		}
		break;
	case VK_RIGHT:
		if (alpha3 > 0) {
			alpha3 -= 5;
			alpha1 -= 12.2;
			alpha2 += 0.9;
			alpha4 -= 5;
			alpha5 -= 12.2;
		}
		else {
			alpha3 = 0;
			alpha1 = 0;
			alpha2 = 0;
			alpha4 = 0;
			alpha5 = 0;
		}
		break;
	}


	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKOL119View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
