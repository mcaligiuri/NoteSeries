#pragma once


// Finestra di dialogo DlgCategorie

class DlgCategorie : public CDialogEx
{
	DECLARE_DYNAMIC(DlgCategorie)

public:
	DlgCategorie(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgCategorie();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODIFICA_CATEGORIE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	// GroupBox controlli
	CStatic m_grpCat;
	// Casella di testo per aggiungere categoria
	CEdit m_txtAggiungi;
	// Casella di testo per rinominare categoria
	CEdit m_txtRinomina;
	// Bottone per aggiungere categoria
	CButton m_btnAggiungi;
	// Bottone per rinominare categoria
	CButton m_btnRinomina;
	// Bottone per eliminare categoria
	CButton m_btnElimina;
	// Lista categorie
	CListBox m_lstCat;
	virtual BOOL OnInitDialog();
	afx_msg void GetDescUpdate();
	afx_msg void GetCateGorie(CListBox* categorie);
public:
	CString m_temp;
	CString m_cat;
	CString m_idcat;
	CString m_sql;
	afx_msg void OnClickAggiungi();
	afx_msg void OnClickRinomina();
	afx_msg void OnClickElimina();
	afx_msg void OnCambioSelezione();
	afx_msg void OnBnClickedBtncancel();
	CButton m_btnAnnulla;
	CNoteSeriesDoc* pDoc;
	CNoteSeriesDoc* GetDoc();
};
