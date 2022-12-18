
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


// CIND17975View

IMPLEMENT_DYNCREATE(CIND17975View, CView)

BEGIN_MESSAGE_MAP(CIND17975View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CIND17975View construction/destruction

CIND17975View::CIND17975View() noexcept
{
	// TODO: add construction code here

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

	m_glRenderer.DrawScene(pDC);
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
	case VK_SPACE:
		m_glRenderer.ToggleGrid();
		Invalidate(NULL);
		break;
	case 'N':
		m_glRenderer.ToggleNormals();
		Invalidate(NULL);
		break;
	case '1':
	case '2':
	case '3':
		m_glRenderer.ToggleLight(nChar);
		Invalidate();
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CIND17975View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
	// TODO: Add your specialized code here and/or call the base class
}


void CIND17975View::OnLButtonDown(UINT nFlags, CPoint point)
{
	onDrag = true;
	pX = point.x;
	pY = point.y;
	CView::OnLButtonDown(nFlags, point);
}


void CIND17975View::OnLButtonUp(UINT nFlags, CPoint point)
{
	onDrag = false;
	CView::OnLButtonUp(nFlags, point);
}


void CIND17975View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (onDrag) {
		m_glRenderer.UpdatePerspective(point.x - pX, point.y - pY);
		pX = point.x;
		pY = point.y;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}


void CIND17975View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CIND17975View::OnDestroy()
{
	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);

	CView::OnDestroy();
}


BOOL CIND17975View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


int CIND17975View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


BOOL CIND17975View::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::OnScroll(nScrollCode, nPos, bDoScroll);
}


BOOL CIND17975View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	m_glRenderer.ZoomView(zDelta > 0 ? false : true);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
