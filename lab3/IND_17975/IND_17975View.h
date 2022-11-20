
// IND_17975View.h : interface of the CIND17975View class
//

#pragma once
#include "DImage.h"

constexpr int BASE = 25;
constexpr int GRID_SIZE = 20;
constexpr int GRID_LINES = 21;
constexpr int GRID_LENGTH = BASE * GRID_SIZE;

constexpr COLORREF GRID = RGB(196, 231, 245);

enum class Filter {
	NOFILTER,
	GRAY,
	BLUE
};

class CIND17975View : public CView
{
protected: // create from serialization only
	CIND17975View() noexcept;
	DECLARE_DYNCREATE(CIND17975View)

// Attributes
private:
	DImage puzzleImgs[9];
	bool gridOn = true;
	XFORM prevT = {};
public:
	CIND17975Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	void ChromaKey(CDC* pDC, DImage& image, COLORREF key, Filter filter = Filter::NOFILTER);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx = false, bool my = false, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	
	void DrawToDC(CDC* pDC);
	void DrawAxes(CDC* pDC);
	void Grid(CDC* pDC);

	void BlueFilter(CBitmap& bmp);
	void GrayFilter(CBitmap& bmp);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND17975View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in IND_17975View.cpp
inline CIND17975Doc* CIND17975View::GetDocument() const
   { return reinterpret_cast<CIND17975Doc*>(m_pDocument); }
#endif

