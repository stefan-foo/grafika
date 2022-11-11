
// IND_17975.h : main header file for the IND_17975 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND17975App:
// See IND_17975.cpp for the implementation of this class
//

class CIND17975App : public CWinApp
{
public:
	CIND17975App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND17975App theApp;
