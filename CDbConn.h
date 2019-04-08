#include <afx.h>
#include <afxdb.h>
#include "Note SeriesDoc.h"
#pragma once

// Classe usata per la connessione al db "serie.mdb" 
// e per l'esecuzione di tutte le query principali del programma
class CDbConn
{
public:
#define MAXELEM 8
	CDbConn();
	~CDbConn();
	virtual BOOL dbopen();
	void UpdateSerieSel(CString sql, CListCtrl *pList);
	void GetSerieCorrente(CString sql);
	void GetVersioni(CString sql, CListCtrl *pList);
	afx_msg BOOL SetSerie(CString query);
	void GetCategoria(CTabCtrl* schede, TCITEM elem);
	void GetIDCategoria(CString *nometab, CEdit* control);
	int ContaOmonimi(CString nome, CString tabella, short categoria);
	CNoteSeriesDoc* GetDoc();
	CNoteSeriesDoc* pDoc;
	short m_riga;
	CDatabase* serie;
	CDatabase language;
	CString m_dbserie, m_dblang;
	CString m_dbfSerie, m_dbfLang;
	CRegKey chiave;
	CString retUser;
	CString temp;
	CString m_cat;
	CString m_idcat;
	CString m_dbPass;
};