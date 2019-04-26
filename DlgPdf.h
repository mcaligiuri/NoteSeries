#import "PdfClient.tlb" no_namespace
#pragma once


// Finestra di dialogo DlgPdf

class DlgPdf : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPdf)

public:
	DlgPdf(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgPdf();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PDF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // supporto DDX/DDV
	// Variabili salvataggio griglia
	CString m_nome;
	CString m_cartella;
	CString m_inizio;
	CString m_fine;
	CString m_stato;
	CString m_priorità;
	CString m_sito;
	CString m_voto;
	CString m_commento;
	// Chiavi di registro
	CRegKey chiave;
	ULONG len;
	CString retLang;
	CString retPath;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void GetDescUpdate();
	void GetList();
	void SvuotaVariabili();
	void SetButton();
	CNoteSeriesDoc* GetDoc();
	CNoteSeriesDoc* pDoc;
	CString m_temp;
private:
	CStatic m_lblTitle;		// Titolo finestra
	CMFCButton m_btnCrea;	// Bottone che crea pdf
	CMFCButton m_btnCanc;	// Chiude la finestra
public:
	afx_msg void Annulla();
	afx_msg void SetPdf();
private:
	CStatic m_lblNome;		// Etichetta nome
	CEdit m_txtNome;		// Casella di testo per inserire il nome del pdf
	CMFCButton m_btnApriPdf;// Apre il pdf selezionato
	CMFCButton m_btnApriDir;// Apre la cartella dei pdf
	CListBox m_lstPdf;		// Lista dei pdf
	CMFCButton m_btnElimina;// Elimino pdf selezionato
public:
	afx_msg void ApriCartellaPdf();
	afx_msg void ApriPdf();
private:
	CEdit m_txtRen;			// Casella che permette di rinominare un file pdf
	CMFCButton m_btnRin;	// Bottone per rinominare
	CFont* m_pBtnFont;
	CFont* m_SizeTitle;
public:
	afx_msg void GetChange();
	afx_msg void EliminaPdf();
	afx_msg void RinominaPdf();
	afx_msg void OnDoubleClick();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); // Metodo da ridefinire per cambiare il colore alle label
};
