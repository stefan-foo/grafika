
// KOL1_11View.h : interface of the CKOL111View class
//

#pragma once
#include "DImage.h"


class CKOL111View : public CView
{
protected: // create from serialization only
	CKOL111View() noexcept;
	DECLARE_DYNCREATE(CKOL111View)

// Attributes
	DImage skyImg;
	HENHMETAFILE clioEMF;
	HENHMETAFILE wheelEMF;
	XFORM saveT;
	int m_x = 0;
	int rotAngle = 0;
public:
	CKOL111Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawScene(CDC* pDC);
	void DrawClio(CDC* pDC, int cw, int ch);
	void DrawWheel(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void Translate(CDC* pDC, float dX, float dY);
	void Rotate(CDC* pDC, float angle);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKOL111View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in KOL1_11View.cpp
inline CKOL111Doc* CKOL111View::GetDocument() const
   { return reinterpret_cast<CKOL111Doc*>(m_pDocument); }
#endif

