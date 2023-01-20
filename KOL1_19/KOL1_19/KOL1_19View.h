
// KOL1_19View.h : interface of the CKOL119View class
//

#pragma once

#include "DImage.h"

class CKOL119View : public CView
{
protected: // create from serialization only
	CKOL119View() noexcept;
	DECLARE_DYNCREATE(CKOL119View)

// Attributes
	DImage body, arm1, arm2, leg1, leg2, background;
	float arm1Rot = 0, arm2Rot = 0, leg1Rot = 0, leg2Rot = 0, bodyRot = 0;
	float alpha5 = 0, alpha1 = 0, alpha2 = 0, alpha3 = 0, alpha4 = 0;
public:
	CKOL119Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void DrawScene(CDC* pDC);
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawImageTransperent(CDC* pDC, DImage* image);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody(CDC* pDC);
	void DrawAxes(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKOL119View();
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

#ifndef _DEBUG  // debug version in KOL1_19View.cpp
inline CKOL119Doc* CKOL119View::GetDocument() const
   { return reinterpret_cast<CKOL119Doc*>(m_pDocument); }
#endif

