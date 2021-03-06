#pragma once


// Finestra di dialogo DlgDate

class DlgDate : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDate)

public:
	DlgDate(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgDate();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODIFICA_DATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickSalva();
	virtual BOOL OnInitDialog();
	void CheckDataInizio();
	void CheckDataFine();
	void GetDescUpdate();
	CNoteSeriesDoc* GetDoc();
	CNoteSeriesDoc* pDoc;
	// Giorno di inizio
	CComboBox m_cmbGiorno;
	// Mese di inizio
	CComboBox m_cmbMese;
	// Anno di fine
	CComboBox m_cmbAnno;
	// Giorno di fine
	CComboBox m_cmbGiornoF;
	// Mese di fine
	CComboBox m_cmbMeseF;
	// Anno di fine
	CComboBox m_cmbAnnoF;
	// Bottone di salvataggio
	CButton m_btnSalva;
	// Titolo del dialog
	CStatic m_lblTitle;
	// Label di inizio
	CStatic m_lblInizio;
	// Label di fine
	CStatic m_lblFine;
	// Giorni
	CString m_giorni;
	// Mesi
	CString m_mesi;
	// Anni
	CString m_anni;
	// Esplodo variabile del documento
	CString m_explode;
	// Posizione del cursore nella tokenize
	int m_pos;
	// Nuova data di inizio
	CString m_data;
	// Nuova data di fine
	CString m_dataF;
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	// Messaggio di errore
	CStatic m_lblError;
	BOOL GetAnnoBisestile(CString anno);
	afx_msg void OnCbnSelchangeCmbmese();
	afx_msg void OnCbnSelchangeCmbmesef();
	afx_msg void OnCbnSelchangeCmbanno();
	afx_msg void OnCbnSelchangeCmbannof();
	virtual void OnOK();
	virtual void OnCancel();
};
