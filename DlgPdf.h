#import "K:\redist\C#\PdfClient\bin\Debug\PdfClient.tlb" no_namespace
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
	CNoteSeriesDoc* GetDoc();
	CNoteSeriesDoc* pDoc;
	CString m_temp;
private:
	CStatic m_lblTitle;	// Titolo finestra
	CButton m_btnCrea;	// Bottone che crea pdf
	CButton m_btnCanc;	// Chiude la finestra
public:
	afx_msg void Annulla();
	afx_msg void SetPdf();
private:
	// Etichetta nome
	CStatic m_lblNome;
	// Casella di testo per inserire il nome del pdf
	CEdit m_txtNome;
};
