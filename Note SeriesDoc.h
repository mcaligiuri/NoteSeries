// Questo codice sorgente degli esempi di MFC illustra l'utilizzo dell'interfaccia utente Microsoft Office Fluent di MFC 
// e viene fornito esclusivamente come riferimento in supplemento 
// al materiale di riferimento e alla documentazione in formato elettronico MFC 
// forniti con il software della libreria MFC C++.  
// Le condizioni di licenza per la copia, l'utilizzo o la distribuzione dell'interfaccia utente Microsoft Office Fluent sono disponibili separatamente.  
// Per ulteriori informazioni sul programma di licenza dell'interfaccia utente Microsoft Office Fluent, visitare il sito 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// Tutti i diritti sono riservati.

// Note SeriesDoc.h: interfaccia della classe CNoteSeriesDoc
//


#pragma once

//#include "CDbConn.h"
class CNoteSeriesDoc : public CDocument
{
protected: // creare solo da serializzazione.
	CNoteSeriesDoc();
	DECLARE_DYNCREATE(CNoteSeriesDoc)

// Attributi
public:
	CString m_nome, m_cartella, m_sito, m_inizio, m_fine, m_voto, m_stato, m_com, m_bmp, m_priorita;
	CString m_tabella;
	CString m_idv, m_ida, m_nodo, m_sezNodo, m_data, m_requery;
	CString m_dbvers, m_backup;
	CString m_versione;
	int m_lbl;
	// Serialize
	short m_nstato;
	short m_nvoto;
	short m_npriorita;
	short m_orderby;
	short m_cat;
	CArray<int, int>m_idl;
// Operazioni
public:
	struct Griglia {
		CString nome;
		CString cartella;
		CString inizio;
		CString fine;
		CString stato;
		CString priorità;
		CString sito;
		CString voto;
		CString commento;
		int idl;
	};
	short m_gridCount;
	Griglia *m_griglia;
	
	// Descrizioni
	CArray<CString,CString>m_stati;		// Per memorizzare gli stati all'avvio
	CArray<CString, CString>m_prior;	// Per memorizzare le priorità all'avvio
	CArray<CString, CString>m_armesi;	// Per memorizzare i mesi all'avvio
	CArray<CString, CString>m_artitoli;	// Per memorizzare i titolini all'avvio
	CArray<CString, CString>m_form;		// Per memorizzare le descrizioni della form principale
	CArray<CString, CString>m_date;		// Per memorizzare le descrizioni del dialog delle date
	CArray<CString, CString>m_setting;	// Per memorizzare le descrizioni del pannello di controllo
	CArray<CString, CString>m_label;	// Per memorizzare le descrizioni del dialog delle etichette
// Sostituzioni
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementazione
public:
	virtual ~CNoteSeriesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funzioni generate per la mappa dei messaggi
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Funzione helper che imposta il contenuto della ricerca per il gestore di ricerche
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};
