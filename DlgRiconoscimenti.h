#pragma once


// Finestra di dialogo DlgRiconoscimenti

class DlgRiconoscimenti : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRiconoscimenti)

public:
	DlgRiconoscimenti(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgRiconoscimenti();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RICONOSCIMENTI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	// Lista persone
	CListBox m_list;
	// Descrizione
	CStatic m_descr;
	// Ritorno per la GetDesc()
	CString retVal;
	virtual BOOL OnInitDialog();
};
