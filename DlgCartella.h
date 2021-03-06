#pragma once


// Finestra di dialogo DlgCartella

class DlgCartella : public CDialogEx
{
	DECLARE_DYNAMIC(DlgCartella)

public:
	DlgCartella(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~DlgCartella();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOLDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_lblTitle;	 // Intestazione finestra di dialogo
	CStatic m_lblSize;	// Label dimensione file
	CStatic m_lblNome;	// Label nome file
	CStatic m_lblCreate;// Label vicina a data di creazione
	CStatic m_gprDati;	// GroupBox coi dati
	CStatic m_lblMod;	// Data ultima modifica
	CListBox m_screen;	// ListBox
	CButton m_btnDel;	// permette di eliminare un file
	CButton m_btnApri;	// permette di aprire un file
	CButton m_btnRen;	// Bottone per applicare il rename di un file
	CButton m_btnClose;	// Chiude la finestra di dialogo
	CEdit m_txtName;	// Casella per rinominare un file
	CEdit m_txtSize;	// Valore dimensione file
	CEdit m_txtMod;		// Valore data ultima modifica
	CEdit m_txtCrea;	// Valore data di creazione
	ULONG len;
	int m_sel;
	CString retUser;
	CString m_path;
	CString m_relPath;
	CString m_nome;
	CRegKey chiave;
	CArray<CString, CString>m_pathArray;
	afx_msg void EliminaElemento();
	afx_msg void ApriFile();
	afx_msg void OnLbnSelchangeScreen();
	afx_msg void DoppioClickElem();
	afx_msg void CaricaContenuto();
	afx_msg void GetPercorso();
	afx_msg void GetDescUpdate();
	afx_msg void RinominaFile();
};
