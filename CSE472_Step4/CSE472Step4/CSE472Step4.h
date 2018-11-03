
// CSE472Step4.h : main header file for the CSE472Step4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCSE472Step4App:
// See CSE472Step4.cpp for the implementation of this class
//

class CCSE472Step4App : public CWinAppEx
{
public:
	CCSE472Step4App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCSE472Step4App theApp;
