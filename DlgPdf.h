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

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CNoteSeriesDoc* GetDoc();
	CNoteSeriesDoc* pDoc;
};
