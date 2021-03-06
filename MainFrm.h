
// MainFrm.h: interfaccia della classe CMainFrame
//

#pragma once
#include "CDbConn.h"
class CNoteSeriesView;

class CMainFrame : public CFrameWndEx
{
	
protected: // creare solo da serializzazione.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributi
protected:
	CSplitterWnd m_wndSplitter;
public:

// Operazioni
public:
	CDbConn dbconn; // Oggetto di connessione ai database
	CNoteSeriesDoc* pDoc;
// Sostituzioni
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementazione
public:
	virtual ~CMainFrame();
	CMFCRibbonButton* m_btnopen;
	CMFCRibbonButton* m_btnsave;
	CMFCRibbonButton* m_btnprint;
	CMFCRibbonButton* m_btninfo;
	CMFCRibbonButton* m_btnstat;
	CMFCRibbonButton* m_btnexce;
	CMFCRibbonButton* m_setting;
	CMFCRibbonButton* m_btnpdf;
	CMFCRibbonComboBox* m_cmbgroup;

	CMFCRibbonButton* m_mnuexce;
	CMFCRibbonButton* m_mnuopen;
	CMFCRibbonButton* m_mnusave;
	CMFCRibbonButton* m_mnuprint;
	CMFCRibbonButton* m_mnuset;
	CMFCRibbonButton* m_mnuexit;
	CMFCRibbonButton* m_mnupdf;
	//CNoteSeriesView* GetRightPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // membri incorporati della barra di controllo
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	//CArray<CString, CString>m_des;

// Funzioni generate per la mappa dei messaggi
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	CNoteSeriesDoc* GetDoc();
	afx_msg void GetGruppi();
	void GetDescUpdate();
	afx_msg void OnGruppi();

	struct Label {
		int id = 0;
		CString nome = _T("");
	};
	Label *m_lbl;
	int m_i;
	afx_msg void OnBtnpdf();
};


