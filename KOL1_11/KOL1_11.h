
// KOL1_11.h : main header file for the KOL1_11 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKOL111App:
// See KOL1_11.cpp for the implementation of this class
//

class CKOL111App : public CWinApp
{
public:
	CKOL111App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKOL111App theApp;
