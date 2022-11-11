
// IND_17975View.h : interface of the CIND17975View class
//

#pragma once
constexpr int BASE = 25;
constexpr int GRID_SIZE = 20;
constexpr int GRID_LINES = 21;
constexpr int GRID_LENGTH = BASE * GRID_SIZE;

// Colors

constexpr COLORREF TURQUOISE = RGB(135, 206, 235);
constexpr COLORREF GRID = RGB(196, 231, 245);
constexpr COLORREF POT_COLOR = RGB(222, 148, 0);
constexpr COLORREF POT_EDGE = RGB(144, 96, 0);
constexpr COLORREF DARK_GREEN = RGB(0, 204, 0);
constexpr COLORREF LIGHT_GREEN = RGB(0, 255, 0);
constexpr COLORREF BLACK = RGB(0, 0, 0);
constexpr COLORREF WHITE = RGB(255, 255, 255);
constexpr COLORREF TEXT_COLOR = RGB(255, 255, 0);

constexpr RECT REGULAR = { 0, 0, long(1.6 * BASE + 0.5), long(2.7 * BASE + 0.5) };
constexpr RECT LARGE = { 0, 0, long(2.5 * BASE + 0.5), long(3 * BASE) };
constexpr RECT SMALL = { 0, 0, long(BASE), long(3 * BASE)};

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
	void DrawCactusBM(CDC* pDC, HENHMETAFILE hmf);
	void DrawCactucBMWS(CDC* pDC, HENHMETAFILE hmf, float sX, float sY);
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
};

#ifndef _DEBUG  // debug version in IND_17975View.cpp
inline CIND17975Doc* CIND17975View::GetDocument() const
   { return reinterpret_cast<CIND17975Doc*>(m_pDocument); }
#endif

