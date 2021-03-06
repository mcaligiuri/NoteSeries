#include "CLanguage.h"
extern CLanguage m_currlang;
#pragma once


// Finestra di dialogo DlgOptions

class DlgOptions : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptions)

public:
	DlgOptions(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgOptions();
	void GetDescUpdate();
	afx_msg void OnBnClickedApriserie();
	afx_msg void OnBnClickedOptsalva();
	afx_msg void OnBnClickedApriloghi();
	afx_msg void OnBnClickedBtndb();
	afx_msg void OnBnClickedAnnulla();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPZIONI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CRegKey chiave;
	CString retUser;
	ULONG len;
	CEdit m_tel, m_soc, m_utente;
	CArray<CEdit*, CEdit*> m_edit;															// Array dinamico per i nomi delle chiavi di registro
	CString txtKey[4] = { _T("Utente"),_T("Tel"),_T("Società"), _T("Mail") };				// Array coi nomi sotto la chiave Settings
	CString txtKeyPath[4] = { _T("SharedMDB"),_T("Cartella"),_T("Loghi"), _T("Export") };	// Array coi nomi sotto la chiave path
	CEdit m_mail;
	CMFCEditBrowseCtrl m_archivi;
	CMFCEditBrowseCtrl m_cartella;
	CComboBox m_lingua;
	CString temp;
	CStatic m_lblUser;
	CStatic m_lblSoc;
	CStatic m_lblFolder;
	CStatic m_lblDb;
	CStatic m_lblLingua;
	CStatic m_lblLoghi;
	CButton m_btnSalva;
	CButton m_btnAnnulla;
	CButton m_btnSerie;
	CButton m_btnDb;
	CButton m_btnExp;
	CButton m_btnLoghi;
	CMFCEditBrowseCtrl m_loghi;
	CMFCEditBrowseCtrl m_export;
	CStatic m_lblExport;
	bool lingua;
	afx_msg void OnBnClickedBtnexp();
	afx_msg void OnCbnSelchangeLingua();
	CStatic m_grpadv;
	// Attiva opzioni avanzate
	CButton m_chkadv;
	afx_msg void OnMostraAvanzate();
	CButton m_btnback;
	// Cancella vecchi database
	CButton m_btnold;
	// Svuota il database
	CButton m_btndel;
	// Abilita o disabilita backup automatico
	CButton m_chkauto;
	// label vecchi db
	CStatic m_lblold;
	// Label svuota database
	CStatic m_lbldel;
	// Label "esporta in sql"
	CStatic m_lblesp;
	CButton m_chkall;
	CButton m_chkviste;
	CButton m_chkfut;
	CButton m_chkcat;
	// Esporta in sql
	CButton m_btnsql;
	// Numero di tabelle da creare in ".sql"
	short m_table;
	CString dir = _T("");
	afx_msg void EseguiBackup();
	afx_msg void DistruggiDatabase();
	afx_msg void RipristinaBackup();
	afx_msg void CheckBackup();
	afx_msg void EsportaTutto();
	afx_msg void EsportaInSQL();
	afx_msg void checkViste();
	afx_msg void checkFuture();
	afx_msg void checkCat();
	BOOL getViste(CString* nomefile);
	BOOL getFuture(CString* nomefile);
	BOOL getCat(CString* nomefile);
	// Titolo del dialog
	CStatic m_lbltitle;
};

