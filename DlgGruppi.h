#include "Note SeriesDoc.h"
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
private:
	CMFCButton m_btnAdd;	// Bottone per creare una label
	CMFCButton m_btnDel;	// Bottone per eliminare un'etichetta
	CComboBox m_cmbLabel;	// Combobox con le label
	CEdit m_txtAdd;			// Casella di testo per aggiungere label
	CMFCButton m_btnAssoc;	// Bottone per associare un nome a una label
	CMFCButton m_btnDis;	// Bottone per dissociare uno o pi� nomi da una label
private:
	CListCtrl m_lstNomi;	// Lista nomi non associati
	CListCtrl m_lstAss;		// Lista nomi associati
public:
	CString m_temp;			// Ritorno per la GetDesc
	CString m_sql;			// Per le query SQL
	CString m_selNome;		// Nome elemento selezionato
	int m_pos;
	short m_rows;			// Numero di righe
	short m_id;				// ID label
public:
	virtual BOOL OnInitDialog();
	afx_msg void SetEtichetta();
	afx_msg void DelEtichetta();
private:
	CEdit m_txtId;	// Id di una label
public:
	afx_msg void OnCambioLabel();
	afx_msg void OnSetAssoc();
	afx_msg void OnDelAssoc();
	void GetNomi();
	void GetDescUpdate();
	CNoteSeriesDoc* GetDoc();
	CNoteSeriesDoc* pDoc;
	CStatic m_lblSel;
	CStatic m_lblnomi;
	CStatic m_lblNomiE;
};
