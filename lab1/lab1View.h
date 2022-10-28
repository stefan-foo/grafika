
// lab1View.h : interface of the Clab1View class
//

#pragma once


class Clab1View : public CView
{
protected: // create from serialization only
	Clab1View() noexcept;
	DECLARE_DYNCREATE(Clab1View)

// Attributes
	const COLORREF white = RGB(255, 255, 255);
	const COLORREF orange = RGB(255, 153, 51);
	const COLORREF grey = RGB(220, 220, 220);
	const COLORREF yellow = RGB(255, 255, 0);
	const COLORREF blue = RGB(104, 154, 255);
	const COLORREF pink = RGB(255, 153, 204);
	const COLORREF purple = RGB(153, 0, 204);
	const COLORREF green = RGB(50, 205, 50);
	const COLORREF red = RGB(255, 0, 0);
	const int gridLines = 21;
	const int base = 50;

	bool gridOn;
public:
	Clab1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	double GetDistance(POINT a, POINT b);
	void Triangle(CDC* pDC, POINT a, POINT b, POINT c);
	void SolidTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF fill);
	void HatchedTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF bck, COLORREF color, ULONG_PTR hatch);
	void InscribedSolidTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF fill, int inscribedPolygonVertices);
	void InscribedHatchedTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF bck, COLORREF color, ULONG_PTR hatch, int inscribedPolygonVertice);
	void Quadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d);
	void Square(CDC* pDC, POINT a, int width, COLORREF fill, float rotAngle = 0);
	void SolidQuadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d, COLORREF fill);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawRegularPolygon(CDC* pDC, POINT center, int r, int n, float rotAngle);
	void InscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n);
	void Grid(CDC* pDC);

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Clab1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in lab1View.cpp
inline Clab1Doc* Clab1View::GetDocument() const
   { return reinterpret_cast<Clab1Doc*>(m_pDocument); }
#endif

