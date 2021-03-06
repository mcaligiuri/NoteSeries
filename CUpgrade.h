#include "CLanguage.h"
extern CLanguage m_currlang;
#pragma once

// Finestra di dialogo CUpgrade

class CUpgrade : public CDialogEx
{
	DECLARE_DYNAMIC(CUpgrade)

public:
	CUpgrade(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~CUpgrade();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPGRADE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void Annulla();
	afx_msg void Installa();
	afx_msg void GetDescUpdate();
	CString temp;
	CString msg;
	CStatic m_lblTtitolo;
	CStatic m_lblVersione;
	CStatic m_lblNversione; // Nuova versione disponibile
	CButton m_btnInstalla;
	CButton m_btnAnnulla;
};
