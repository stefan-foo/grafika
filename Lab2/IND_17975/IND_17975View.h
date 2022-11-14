
// IND_17975View.h : interface of the CIND17975View class
//
#pragma once
#include "constants.h"

constexpr int BASE = 25;
constexpr int GRID_SIZE = 20;
constexpr int GRID_LINES = 21;
constexpr int GRID_LENGTH = BASE * GRID_SIZE;

constexpr double fcWidthFactor = 2.5;
constexpr double fcHeightFactor = 3;
constexpr double scWidthFactor = 0.8;
constexpr double scHeightFactor = 3;
constexpr double rcHeightFactor = 3;
constexpr double rcWidthFactor = 1.6;

class CIND17975View : public CView
{
protected: // create from serialization only
	CIND17975View() noexcept;
	DECLARE_DYNCREATE(CIND17975View)

// Attributes
private:
	bool gridOn = true;
	HENHMETAFILE lightCactus;
	HENHMETAFILE darkCactus;
	int rootAngle = 0;
	int branchAngle = 0;

	POINT potJoin;
	POINT midJoin;
	POINT rightJoin;
	POINT rightUJoin;
	POINT rightRJoin;
	POINT rightTJoin;
	POINT rightDJoin;

public:
	CIND17975Doc* GetDocument() const;

// Operations
public:

private:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void TRT(CDC* pDC, float sX, float dX, float angle, bool rightMultiply = false);

	void Grid(CDC* pDC);
	void DrawFigure(CDC* pDC);
	void DrawPot(CDC* pDC);
	void DrawCactusBC(CDC* pDC, HENHMETAFILE hmf);
	void DrawCactucBCWS(CDC* pDC, HENHMETAFILE hmf, float sX, float sY);
	void WriteText(CDC* pDC, CString text, POINT location);
	void Circle(CDC* pDC, int cx, int cy, int r, COLORREF color, COLORREF fill);
	inline void Circle(CDC* pDC, POINT center, int r, COLORREF color, COLORREF fill);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in IND_17975View.cpp
inline CIND17975Doc* CIND17975View::GetDocument() const
   { return reinterpret_cast<CIND17975Doc*>(m_pDocument); }
#endif

