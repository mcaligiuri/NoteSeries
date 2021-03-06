// LeftView.h: interfaccia della classe CLeftView

#pragma once

class CNoteSeriesDoc;

class CLeftView : public CTreeView
{
protected: // creare solo da serializzazione.
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributi
public:
	CNoteSeriesDoc* GetDocument();
	HTREEITEM hRoot;
	CString retVal, m_nodo, m_sezNodo;
	CString tabella;
	ULONG len = 0;
	HTREEITEM hViste;
	HTREEITEM hFuture;
	HTREEITEM hAnni;
	HTREEITEM hAnniF;
	HTREEITEM hMesi;
	HTREEITEM hStati;
	HTREEITEM hStatiV;
	HTREEITEM hPriorita;
	HTREEITEM hAlfa;
	HTREEITEM hAlfaFut;
	HTREEITEM hInizio;
	HTREEITEM hFine;
// Operazioni
public:

// Sostituzioni
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // richiamato la prima volta dopo la costruzione

// Implementazione
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funzioni generate per la mappa dei messaggi
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void SetOrdinamenti(CTreeCtrl & pList, HTREEITEM root);
};

#ifndef _DEBUG  // versione di debug in LeftView.cpp
inline CNoteSeriesDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CNoteSeriesDoc*>(m_pDocument); }
#endif

