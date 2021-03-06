#include "CLanguage.h"
#include "CDbConn.h"
extern CLanguage m_currlang;
#pragma once

// Finestra di dialogo DlgVersioni

class DlgVersioni : public CDialogEx
{
	DECLARE_DYNAMIC(DlgVersioni)

public:
	DlgVersioni(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgVersioni();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VERSIONI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lista;
	CString m_sql;
};
