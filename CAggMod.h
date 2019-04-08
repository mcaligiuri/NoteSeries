#pragma once
// Finestra di dialogo CAggMod

class CAggMod : public CDialogEx
{
	DECLARE_DYNAMIC(CAggMod)
public:
	CAggMod(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~CAggMod();
	afx_msg void GetDescUpdate();
	void SetIcons();
	CString temp; // Usato come ritorno per la GetDesc
	CComboBox m_dlgstato, m_dlgvoto, m_dlgpriorita;
	CEdit m_frmnome;
	CEdit m_dlgsito;
	CEdit m_dlgcom;
	CEdit m_dlgcartella;
	CButton m_dlgChkCartella;
	CStatic m_dlgLabel;					// Label con indicazione
	CMFCEditBrowseCtrl m_dlgsimbolo;
	short width;					// Lunghezza picture control con logo settato
	short height;					// Altezza picture control con logo settato
	CRegKey chiave;
	ULONG len;
	CString relPath;
	CString m_sql;
	CStatic m_lblNome;
	CStatic m_lblSito;
	CStatic m_lblSimbolo;
	CStatic m_lblPriorita;
	CStatic m_lblStato;
	CStatic m_lblVoto;
	CStatic m_lblCom;
	CButton m_btnInizia;
	CButton m_btnSalva;
	CButton m_btnAggiungi;
	CButton m_btnTerm;
	CButton m_btnDel;
	CButton m_btnReset;
	CButton m_btnApri;		// Pulsante per aprire cartella della serie selezionata
	CButton m_rdoViste;
	CButton m_btnCopiaURL;
	CStatic m_gprType;		// GroupBox con radio
	CNoteSeriesDoc* pDoc;

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGGIUNGI_MODIFICA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnapricartella();
	void SerieSelezionata(NMHDR * pNMHDR, LRESULT * pResult);
	void OnBnClickedCheckcartella();
	void GetCartella();
	void AggiungiSerie();
	void ModificaSerie();
	void OnCbnSelchangeCombostato();
	virtual void OnBnClickedBtnreset();
	void OnClickCopiaURL();
	virtual void OnOK();
	virtual void OnCancel();
	CNoteSeriesDoc* GetDoc();
};
