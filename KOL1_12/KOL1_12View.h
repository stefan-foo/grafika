
// KOL1_12View.h : interface of the CKOL112View class
//

#pragma once


class CKOL112View : public CView
{
protected: // create from serialization only
	CKOL112View() noexcept;
	DECLARE_DYNCREATE(CKOL112View)

// Attributes
	XFORM prevT = {};
	float angle = 0;
public:
	CKOL112Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	void Translate(CDC* pDC, float x, float y);
	void Rotate(CDC* pDC, float angle);

	void Trapez(CDC* pDC, int a, int b, int h);
	void DrawScene(CDC* pDC);
	void DrawGear(CDC* pDC, int spokeCount, int spokeWidth);

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKOL112View();
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

#ifndef _DEBUG  // debug version in KOL1_12View.cpp
inline CKOL112Doc* CKOL112View::GetDocument() const
   { return reinterpret_cast<CKOL112Doc*>(m_pDocument); }
#endif

