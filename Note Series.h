// Note Series.h: file di intestazione principale per l'applicazione Note Series

#pragma once

#ifndef __AFXWIN_H__
	#error "includere 'stdafx.h' prima di includere questo file per PCH"
#endif

#include "resource.h"       // simboli principali


// CNoteSeriesApp:
// Per l'implementazione di questa classe, vedere Note Series.cpp
//

class CNoteSeriesApp : public CWinAppEx
{
public:
	CNoteSeriesApp();
	CString m_grid = _T(""); // Stringa che ha il compito di mostrare l'array di struct della griglia

// Override
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementazione
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void ShowOptions();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPrintDoc();
};

extern CNoteSeriesApp theApp;
