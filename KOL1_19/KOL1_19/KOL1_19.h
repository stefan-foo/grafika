
// KOL1_19.h : main header file for the KOL1_19 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CKOL119App:
// See KOL1_19.cpp for the implementation of this class
//

class CKOL119App : public CWinApp
{
public:
	CKOL119App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKOL119App theApp;
