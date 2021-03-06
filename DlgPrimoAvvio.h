#pragma once


// Finestra di dialogo DlgPrimoAvvio

class DlgPrimoAvvio : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPrimoAvvio)

public:
	DlgPrimoAvvio(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgPrimoAvvio();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRIMO_AVVIO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString* GetVersioneDB();
	void OnClickSalva();
	void GetDescUpdate();
	BOOL AggiornaDB();
	BOOL EliminaIndici();
	int ContaCategorie(CString* sql);
	int m_count;					// Tiene il conto del numero di categorie presenti nel db
	CString m_vers;					// Variabile che contiene la versione del database
	CButton m_btnSalva;				// Bottone per il salvataggio delle impostazioni
	CString m_msg;					// Variabile per messaggi
	CString m_tmp;					// Variabile di ritorno per GetDesc()
	CComboBox m_cmbLingua;			// Combo lingua predefinita
	CEdit m_categoria;				// Casella di testo per categoria predefinita
	CEdit m_nome;
	CEdit m_mail;
	CEdit m_soc;
	CEdit m_tel;
	CMFCEditBrowseCtrl m_loghi;		// path loghi
	CMFCEditBrowseCtrl m_export;	// Path Export
	CMFCEditBrowseCtrl m_cartella;	// Path directory radice dove vengono salvate le cartelle per gli elementi
	CArray<CEdit*, CEdit*> m_edit;	// Array dinamico per i nomi delle chiavi di registro
	CRegKey chiave;
	ULONG m_len;
	CString m_ret;
	CString txtKey[4] = { _T("Utente"),_T("Tel"),_T("Società"), _T("Mail") };	// Array coi nomi sotto la chiave Settings
	CString txtKeyPath[3] = { _T("Loghi"),_T("Export"),_T("Cartella") };		// Array coi nomi sotto la chiave path
	CStatic m_lblLang;
	CStatic m_lblCat;
	CStatic m_gpbAnag;
	CStatic m_gpbPath;
	CStatic m_lblNome;
	CStatic m_lblMail;
	CStatic m_lblSoc;
	CStatic m_lblTel;
	CStatic m_lblLoghi;
	CStatic m_lblExport;
	CStatic m_lblRadice;
	CStatic m_lblDbVers;
	CStatic m_lblTitle;
	CStatic m_lblNota;
	CStatic m_lblSez1;
	CStatic m_lblSez2;
	CStatic m_lblSez3;
	CStatic m_lblEsempi;
};
