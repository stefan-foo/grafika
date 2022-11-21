
// KOL1_12.h : main header file for the KOL1_12 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKOL112App:
// See KOL1_12.cpp for the implementation of this class
//

class CKOL112App : public CWinApp
{
public:
	CKOL112App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKOL112App theApp;
