// Note SeriesView.h: interfaccia della classe CNoteSeriesView
//
#include "CLanguage.h"
#include "Note SeriesDoc.h"
#include "DlgCategorie.h"
#include "DlgDate.h"
#include "CAggMod.h"
#include "DlgGruppi.h"
extern CLanguage m_currlang;
extern CDbConn dbconfig;
#pragma once

class CNoteSeriesView : public CFormView
{
protected: // creare solo da serializzazione.
	CNoteSeriesView();
	DECLARE_DYNCREATE(CNoteSeriesView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_NOTESERIES_FORM };
#endif

// Attributi
public:
	CNoteSeriesDoc* GetDocument() const;

// Operazioni
public:
	afx_msg void GetDescUpdate();
	CString temp; // Usato come ritorno per la GetDesc
// Sostituzioni
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // supporto DDX/DDV
	virtual void OnInitialUpdate(); // richiamato la prima volta dopo la costruzione

// Implementazione
public:
	virtual ~CNoteSeriesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funzioni generate per la mappa dei messaggi
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void SetIcons();
	void GetLogo();
	void OnBnClickedBtnreset();
	void SerieSelezionata(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void CambiaMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void IniziaSerie();
	afx_msg void TerminaSerie();
	afx_msg void EliminaSerie();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnExportExcel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClickRdoViste();
	afx_msg void OnClickRdoFuture();
	afx_msg void NascondiColonne(CString tabella);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickCategorie();
	afx_msg void OnClickModificaDate();
	afx_msg void SalvaProgetto();
	afx_msg void OnFileOpen();
	afx_msg void OnClickColonna(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL ControllaAggiornamenti();
	afx_msg void EvidenziaPerPriorita(NMHDR *pNMHDR, LRESULT *pResult);
	CNoteSeriesView * GetView();
	CListCtrl m_output;
	short width = 128;		// Lunghezza picture control con logo settato
	short height = 128;		// Altezza picture control con logo settato
	BOOL m_orderNome = TRUE;	// Ordinamento per colonna nome
	CRegKey chiave;
	ULONG len = 255;
	CString relPath;
	CString m_sql;
	CMFCButton m_btnInizia;
	CMFCButton m_btnTerm;
	CMFCButton m_btnDel;
	CButton m_rdoViste;			// imposta tabella db su "anime"
	CButton m_rdoFuture;		// Imposta tabella db su "da vedere"
	CTabCtrl m_TabCtrl;			// Schede delle categorie al di sopra della griglia
	CEdit m_txtIdCat;			// casella di testo nascosta che memorizza id categoria
	CStatic m_lblCat;			// Label delle categorie
	// Enumerazione per le priorità
	// serve per i colori
	enum Priority 
	{
		ALTA,
		MEDIA,
		BASSA,
		NESSUNA,
		VUOTA,
	};

	Priority priorità;
	CTabCtrl m_tabCom;		// Menù a tab nella vista
	CWnd* m_mostraDlg;		// Puntatore che contiene l'indirizzo del dialog da mostrare
	CAggMod dlg0;			// Form per modificare o aggiungere una serie
	DlgDate dlg2;			// Dialog per le date
	DlgGruppi dlg1;			// Dialog per assegnare le etichette
	
};

#ifndef _DEBUG  // versione di debug in Note SeriesView.cpp
inline CNoteSeriesDoc* CNoteSeriesView::GetDocument() const
   { return reinterpret_cast<CNoteSeriesDoc*>(m_pDocument); }
#endif

