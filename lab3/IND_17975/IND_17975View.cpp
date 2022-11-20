
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
#include <string>

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
	for (auto i = 0; i < 9; i++) {
		puzzleImgs[i].Load(CString(("./images/puzzle" + std::to_string(i) + ".dib").c_str()));
	}
	prevT = { 0, 0, 0, 0 };
	gridOn = true;
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

void CIND17975View::ChromaKey(CDC* pDC, DImage& image, COLORREF key, Filter filter)
{
	int w = image.Width();
	int h = image.Height();

	CBitmap maskBmp, imageBmp;
	maskBmp.CreateBitmap(w, h, 1, 1, NULL);
	imageBmp.CreateCompatibleBitmap(pDC, w, h);

	CDC dstDC, srcDC;
	dstDC.CreateCompatibleDC(pDC);
	srcDC.CreateCompatibleDC(pDC);
	dstDC.SelectObject(&maskBmp);
	srcDC.SelectObject(&imageBmp);
	
	imageBmp.SetBitmapBits(w * h * image.BPP(), image.GetDIBBits());
	//image.Draw(&srcDC, CRect(0, 0, w, h), CRect(0, 0, w, h));

	srcDC.SetBkColor(key);
	dstDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);
	
	switch (filter) {
	case Filter::BLUE:
		BlueFilter(*srcDC.GetCurrentBitmap());
		break;
	case Filter::GRAY:
		GrayFilter(*srcDC.GetCurrentBitmap());
		break;
	}

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &dstDC, 0, 0, SRCAND);

	pDC->BitBlt(-w/2, -h/2, w, h, &dstDC, 0, 0, SRCAND);
	pDC->BitBlt(-w/2, -h/2, w, h, &srcDC, 0, 0, SRCPAINT);

	srcDC.DeleteDC();
	dstDC.DeleteDC();
}

void CIND17975View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm = { sX, 0, 0, sY };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xForm = { mx ? -1 : 1, 0, 0, my ? -1 : 1 };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND17975View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double angleRad = RAD(angle);
	double sina = sin(angleRad), cosa = cos(angleRad);
	XFORM xForm = { cosa, -sina, sina, cosa };
	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
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

void CIND17975View::BlueFilter(CBitmap& bmp)
{
	BITMAP bm;
	bmp.GetBitmap(&bm);
	int w = bm.bmWidthBytes;
	int h = bm.bmHeight;

	byte* bits = new byte[w * h];

	bmp.GetBitmapBits(w * h, bits);
	for (auto i = 0; i < w * h - 4; i += 4) {
		bits[i + 1] = bits[i + 2] = bits[i + 3] = 0;
		bits[i] = bits[i] + 64 > 255 ? 255 : bits[i] + 64;
	}
	bmp.SetBitmapBits(w * h, bits);

	delete[] bits;
}

void CIND17975View::GrayFilter(CBitmap& bmp)
{
	BITMAP bm;
	bmp.GetBitmap(&bm);
	int w = bm.bmWidthBytes;
	int h = bm.bmHeight;

	byte* bits = new byte[w * h];

	bmp.GetBitmapBits(w * h, bits);
	for (auto i = 0; i < w * h - 4; i += 4) {
		int gr = 64 + (bits[i] + bits[i + 1] + bits[i + 1]) / 3;
		bits[i] = bits[i + 1] = bits[i + 2] = gr > 255 ? 255 : gr;
		bits[3] = 0;
	}
	bmp.SetBitmapBits(w * h, bits);

	delete[] bits;
}

void CIND17975View::DrawToDC(CDC* pDC)
{
	if (gridOn) {
		Grid(pDC);
	}

	int prevGMode = pDC->SetGraphicsMode(GM_ADVANCED);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, BASE * 4 + 5, BASE * 4 + 5);
	Rotate(pDC, 25);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[0], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, 10 * BASE + 8, BASE * 4 + 1);
	Rotate(pDC, 55);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[1], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, GRID_LENGTH - BASE * 4 + 6, BASE * 4 - 6);
	Rotate(pDC, 104);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[2], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, 4 * BASE - 7, GRID_LENGTH / 2 + 4);
	Rotate(pDC, -73);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[3], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, GRID_LENGTH / 2 - 9, GRID_LENGTH / 2 - 1);
	Rotate(pDC, -122);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[4], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, GRID_LENGTH - 4 * BASE - 9, GRID_LENGTH / 2);
	Rotate(pDC, -108);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[5], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, 4 * BASE - 6, GRID_LENGTH - BASE * 4 - 8);
	Rotate(pDC, 19);
	Mirror(pDC, true, false);
	ChromaKey(pDC, puzzleImgs[6], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, GRID_LENGTH / 2 - 8, GRID_LENGTH - BASE * 4 + 5);
	Rotate(pDC, -67);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[7], RGB(0, 255, 0), Filter::BLUE);
	pDC->SetWorldTransform(&prevT);

	pDC->GetWorldTransform(&prevT);
	Translate(pDC, GRID_LENGTH - 4 * BASE - 1, GRID_LENGTH - BASE * 4 + 7);
	Rotate(pDC, -19);
	Mirror(pDC, false, true);
	ChromaKey(pDC, puzzleImgs[8], RGB(0, 255, 0), Filter::GRAY);
	pDC->SetWorldTransform(&prevT);

	pDC->SetGraphicsMode(prevGMode);
}

void CIND17975View::DrawAxes(CDC* pDC)
{
	POINT curPos = pDC->GetCurrentPosition();
	CPen* prevPen = pDC->GetCurrentPen();
	// Y - CRVENA
	CPen xAxisPen(PS_SOLID, 5, RGB(255, 0, 0));
	pDC->SelectObject(&xAxisPen);
	pDC->LineTo(curPos.x, curPos.y + 100);
	pDC->MoveTo(curPos.x, curPos.y);
	// X - PLAVA
	CPen yAxisPen(PS_SOLID, 5, RGB(0, 0, 255));
	pDC->SelectObject(&yAxisPen);
	pDC->LineTo(curPos.x + 100, curPos.y);
	pDC->MoveTo(curPos.x, curPos.y);

	pDC->SelectObject(prevPen);
}

void CIND17975View::OnDraw(CDC* pDC)
{
	CIND17975Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	CBrush brush(RGB(255, 255, 255));
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	memDC.SelectObject(&bmp);
	memDC.FillRect(clientRect, &brush);

	DrawToDC(&memDC);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	bmp.DeleteObject();
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
	if (nChar == 'G') {
		gridOn = !gridOn;
		Invalidate();
	}
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CIND17975View::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnPrepareDC(pDC, pInfo);
}


BOOL CIND17975View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	//return CView::OnEraseBkgnd(pDC);
}
