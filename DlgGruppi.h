#pragma once


// Finestra di dialogo DlgGruppi

class DlgGruppi : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGruppi)

public:
	DlgGruppi(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgGruppi();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRUPPI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
};
