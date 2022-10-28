
// test3View.h : interface of the Ctest3View class
//

#pragma once


class Ctest3View : public CView
{
protected: // create from serialization only
	Ctest3View() noexcept;
	DECLARE_DYNCREATE(Ctest3View)

// Attributes
public:
	Ctest3Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Ctest3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg
	bool gridOn;

	double GetDistance(POINT a, POINT b);
	void DrawInscribedPolygon(CDC* pDC, POINT r, POINT a, POINT b, int n);
	void DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c);
	void DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF color, tagLOGBRUSH brush);
	void DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF color, COLORREF fill = NULL);
	void DrawShape(CDC* pDC, POINT points[], int numOfPoints, COLORREF color, COLORREF fill);
	void DrawSquare(CDC* pDC, POINT a, int width, COLORREF color, COLORREF fill, float rotAngle = 0);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawGrid(CDC* pDC, POINT start, int length, int distance, COLORREF color);
	void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in test3View.cpp
inline Ctest3Doc* Ctest3View::GetDocument() const
   { return reinterpret_cast<Ctest3Doc*>(m_pDocument); }
#endif

