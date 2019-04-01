#include "stdafx.h"
#include "CDbConn.h"
#include "Note SeriesDoc.h"
#include "Note SeriesView.h"
#include "mainfrm.h"

CDbConn dbconfig;

CDbConn::CDbConn()
{
	m_dbfSerie = _T("");
	retUser = _T("");
	temp = _T("");
	m_cat = _T("");
	m_idcat = _T("");
	m_dbPass = _T("Daniela_Serie");
	m_riga = 0;
	serie = new CDatabase;
}


CDbConn::~CDbConn()
{
	delete serie;
}

BOOL CDbConn::dbopen()
{
	// Connessione al database
	// Uso la chiave di registro in modo da poter decidere dove puntare
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		ULONG len = 255;
		chiave.QueryStringValue((_T("SharedMDB")), retUser.GetBufferSetLength(len), &len);
		m_dbfSerie.Format(_T("%s/serie.mdb"), retUser);
	}
	m_dbserie.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s;PWD=%s"), _T("MICROSOFT ACCESS DRIVER (*.mdb)"), m_dbfSerie, m_dbPass);
	try
	{
		if (!serie->Open(NULL, FALSE, FALSE, m_dbserie))
			return FALSE;
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return FALSE;
	}
	return TRUE;
}

// Questa funzione serve per tutte le query di aggiornamento (INSERT,UPDATE,DELETE)
BOOL CDbConn::SetSerie(CString query)
{
	if (!dbopen())
		return FALSE;
	try
	{
		serie->ExecuteSQL(query);
		//AfxMessageBox((m_currlang.GetDesc(35, temp))); // Operazione avvenuta con successo
		serie->Close();
		return TRUE;
	}
	catch (CDBException* e)
	{
		AfxMessageBox((m_currlang.GetDesc(75, temp))); // Errore
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		serie->Close();
		return FALSE;
	}
}

// Funzione che ha il compito di aggiornare la griglia di output
void CDbConn::UpdateSerieSel(CString sql, CListCtrl *pList)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	long n = 0;
	/*pList->SetRedraw(FALSE);*/
	pList->DeleteAllItems();
	if (!dbopen())
		return;
	try
	{
		CRecordset* righe = NULL;
		righe = new CRecordset(serie);
		righe->Open(CRecordset::snapshot, sql, CRecordset::readOnly);
		
		if (pDoc->m_tabella == _T("[anime]")) 
		{
			while (!righe->IsEOF())
			{
				righe->GetFieldValue(L"nome", pDoc->m_nome);
				righe->GetFieldValue(L"cartella", pDoc->m_cartella);
				righe->GetFieldValue(L"inizio", pDoc->m_inizio);
				righe->GetFieldValue(L"fine", pDoc->m_fine);
				righe->GetFieldValue(L"sito", pDoc->m_sito);
				righe->GetFieldValue(L"commenti", pDoc->m_com);
				righe->GetFieldValue(L"voto", pDoc->m_voto);
				righe->GetFieldValue(L"stato", pDoc->m_stato);
				

				// Inserisco gli elementi nella griglia
				n = pList->InsertItem(0, _T(""));
				pList->SetItemText(n, 0, pDoc->m_nome);
				pList->SetItemText(n, 1, pDoc->m_cartella);
				pList->SetItemText(n, 2, pDoc->m_inizio);
				pList->SetItemText(n, 3, pDoc->m_fine);

				// Controllo sul voto
				// fa schifo lo so..
				switch (_wtoi(pDoc->m_voto))
				{
				case 0:
					pDoc->m_voto = (_T("-"));
					break;
				case 1:
					pDoc->m_voto = (_T("0"));
					break;
				case 2:
					pDoc->m_voto = (_T("0.5"));
					break;
				case 3:
					pDoc->m_voto = (_T("1"));
					break;
				case 4:
					pDoc->m_voto = (_T("1.5"));
					break;
				case 5:
					pDoc->m_voto = (_T("2"));
					break;
				case 6:
					pDoc->m_voto = (_T("2.5"));
					break;
				case 7:
					pDoc->m_voto = (_T("3"));
					break;
				case 8:
					pDoc->m_voto = (_T("3.5"));
					break;
				case 9:
					pDoc->m_voto = (_T("4"));
					break;
				case 10:
					pDoc->m_voto = (_T("4.5"));
					break;
				case 11:
					pDoc->m_voto = (_T("5"));
					break;
				case 12:
					pDoc->m_voto = (_T("5.5"));
					break;
				case 13:
					pDoc->m_voto = (_T("6"));
					break;
				case 14:
					pDoc->m_voto = (_T("6.5"));
					break;
				case 15:
					pDoc->m_voto = (_T("7"));
					break;
				case 16:
					pDoc->m_voto = (_T("7.5"));
					break;
				case 17:
					pDoc->m_voto = (_T("8"));
					break;
				case 18:
					pDoc->m_voto = (_T("8.5"));
					break;
				case 19:
					pDoc->m_voto = (_T("9"));
					break;
				case 20:
					pDoc->m_voto = (_T("9.5"));
					break;
				case 21:
					pDoc->m_voto = (_T("10"));
					break;
				}
				pList->SetItemText(n, 4, pDoc->m_voto);
				pList->SetItemText(n, 5, pDoc->m_sito);
				pList->SetItemText(n, 6, _T("-"));

				// Controllo sullo stato
				// 0 finito, 1 in attesa, 2 in corso, 3 in uscita, 4 concluso
				switch(_wtoi(pDoc->m_stato))
				{ 
				case 0:
					//pDoc->m_stato = m_currlang.GetDesc(1,temp);
					pDoc->m_stato = pDoc->m_stati.GetAt(0);
					break;			
				case 2:				
					pDoc->m_stato = pDoc->m_stati.GetAt(2);
					break;			
				}
				pList->SetItemText(n, 7, pDoc->m_stato);
				pList->SetItemText(n, 8, pDoc->m_com);
				righe->MoveNext();
			}
			// Scorro la griglia e ne memorizzo i dati in un array di struct
			// Memorizzo anche il numero di elementi
			pDoc->m_gridCount = pList->GetItemCount();
			pDoc->m_griglia = new CNoteSeriesDoc::Griglia[pDoc->m_gridCount];
			for (int i = 0; i < pList->GetItemCount(); i++)
			{
				pDoc->m_griglia[i].nome = pList->GetItemText(i, 0);
				pDoc->m_griglia[i].cartella = pList->GetItemText(i, 1);
				pDoc->m_griglia[i].inizio = pList->GetItemText(i, 2);
				pDoc->m_griglia[i].fine = pList->GetItemText(i, 3);
				pDoc->m_griglia[i].voto = pList->GetItemText(i, 4);
				pDoc->m_griglia[i].sito = pList->GetItemText(i, 5);
				pDoc->m_griglia[i].priorità = pList->GetItemText(i, 6);
				pDoc->m_griglia[i].stato = pList->GetItemText(i, 7);
				pDoc->m_griglia[i].commento = pList->GetItemText(i, 8);
			}
		}
		else
		{
			while (!righe->IsEOF())
			{
				righe->GetFieldValue(L"idv", pDoc->m_idv);
				righe->GetFieldValue(L"nome", pDoc->m_nome);
				righe->GetFieldValue(L"commenti", pDoc->m_com);
				righe->GetFieldValue(L"priorità", pDoc->m_priorita);
				righe->GetFieldValue(L"stato", pDoc->m_stato);
				
				n = pList->InsertItem(0, _T(""));
				pList->SetItemText(n, 0, pDoc->m_nome);
				pList->SetItemText(n, 1, _T("-"));
				pList->SetItemText(n, 2, _T("-"));
				pList->SetItemText(n, 3, _T("-"));
				pList->SetItemText(n, 4, _T("-"));
				pList->SetItemText(n, 5, _T("-"));

				// Controllo sulla priorità
				// 0 = nessuna, 1 = alta, 2 = media, 3 = bassa, -
				switch (_wtoi(pDoc->m_priorita))
				{
				case 0:
					pDoc->m_priorita = pDoc->m_prior.GetAt(0);
					break;
				case 1:
					pDoc->m_priorita = pDoc->m_prior.GetAt(1);
					break;
				case 2:
					pDoc->m_priorita = pDoc->m_prior.GetAt(2);
					break;
				case 3:
					pDoc->m_priorita = pDoc->m_prior.GetAt(3);
					break;
				}
				pList->SetItemText(n, 6, pDoc->m_priorita);

				// Controllo sullo stato
				// 0 finito, 1 in attesa, 2 in corso, 3 in uscita, 4 concluso
				switch (_wtoi(pDoc->m_stato))
				{
				case 1:
					pDoc->m_stato = pDoc->m_stati[1];
					break;
				case 3:
					pDoc->m_stato = pDoc->m_stati[3];
					break;
				case 4:
					pDoc->m_stato = pDoc->m_stati[4];
					break;
				}
				pList->SetItemText(n, 7, pDoc->m_stato);
				pList->SetItemText(n, 8, pDoc->m_com);
				righe->MoveNext();
			}
			// Scorro la griglia e ne memorizzo i dati in un array di struct
			// Memorizzo anche il numero di elementi
			pDoc->m_gridCount = pList->GetItemCount();
			pDoc->m_griglia = new CNoteSeriesDoc::Griglia[pDoc->m_gridCount];
			for (int i = 0; i < pList->GetItemCount(); i++)
			{
				pDoc->m_griglia[i].nome = pList->GetItemText(i, 0);
				pDoc->m_griglia[i].cartella = pList->GetItemText(i, 1);
				pDoc->m_griglia[i].inizio = pList->GetItemText(i, 2);
				pDoc->m_griglia[i].fine = pList->GetItemText(i, 3);
				pDoc->m_griglia[i].voto = pList->GetItemText(i, 4);
				pDoc->m_griglia[i].sito = pList->GetItemText(i, 5);
				pDoc->m_griglia[i].priorità = pList->GetItemText(i, 6);
				pDoc->m_griglia[i].stato = pList->GetItemText(i, 7);
				pDoc->m_griglia[i].commento = pList->GetItemText(i, 8);
			}
		}
		righe->Close();
		delete righe;
		serie->Close();
		/*pList->SetRedraw(TRUE);
		pList->Invalidate();
		pList->UpdateWindow();*/
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
}

// Prendo i dati della seria selezionata in griglia
void CDbConn::GetSerieCorrente(CString sql)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	if (!dbopen())
		return;
	try
	{
		CRecordset righe(serie);
		righe.Open(CRecordset::forwardOnly, sql);
		if (pDoc->m_tabella == _T("[anime]"))
		{
			while (!righe.IsEOF())
			{
				righe.GetFieldValue(L"ida", pDoc->m_ida);
				righe.GetFieldValue(L"nome", pDoc->m_nome);
				righe.GetFieldValue(L"inizio", pDoc->m_inizio);
				righe.GetFieldValue(L"fine", pDoc->m_fine);
				righe.GetFieldValue(L"sito", pDoc->m_sito);
				righe.GetFieldValue(L"commenti", pDoc->m_com);
				righe.GetFieldValue(L"stato", pDoc->m_stato);
				righe.GetFieldValue(L"voto", pDoc->m_voto);
				righe.GetFieldValue(L"bitmap", pDoc->m_bmp);
				righe.GetFieldValue(L"cartella", pDoc->m_cartella);
				righe.MoveNext();
			}
		}
		else
		{
			while (!righe.IsEOF())
			{
				righe.GetFieldValue(L"idv", pDoc->m_idv);
				righe.GetFieldValue(L"nome", pDoc->m_nome);
				righe.GetFieldValue(L"commenti", pDoc->m_com);
				righe.GetFieldValue(L"priorità", pDoc->m_priorita);
				righe.GetFieldValue(L"stato", pDoc->m_stato);
				righe.MoveNext();
			}
		}
		righe.Close();
		serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
	/*serie.Close();*/
}

// Visualizzo le versioni di database e dll memorizzati in una tabella
void CDbConn::GetVersioni(CString sql, CListCtrl* pList)
{
	int n = 0;
	short i = 0;
	CString nome = _T("");
	CString vers = _T("");
	dbopen();
	try
	{
		CRecordset righe(serie);
		righe.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"FILE", nome);
			righe.GetFieldValue(L"VERSIONE", vers);
			n = pList->InsertItem(i, _T(""));
			pList->SetItemText(n, 0, nome);
			pList->SetItemText(n, 1, vers);
			i++;
			righe.MoveNext();
		}
		pList->InsertItem(0, _T("")); // Riga bianca
		righe.Close();
		serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
}

void CDbConn::GetCategoria(CTabCtrl *schede, /*CComboBox *cmbCat,*/ TCITEM elem)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CNoteSeriesView *pView = (CNoteSeriesView *)pMain->GetActiveView();
	CNoteSeriesDoc* pDoc = pView->GetDocument();
	short i = 0;
	dbopen();
	try
	{
		CRecordset righe(serie);
		righe.Open(CRecordset::forwardOnly, L"SELECT * FROM [CATEGORIE]");
		schede->DeleteAllItems();
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"IDCAT", m_idcat);
			righe.GetFieldValue(L"NOMECAT", m_cat);
			elem.pszText = (LPWSTR)(LPCWSTR)m_cat;
			schede->InsertItem(i, &elem);
			i++;
			righe.MoveNext();
		}
		righe.Close();
		serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
}


void CDbConn::GetIDCategoria(CString *nometab, CEdit* control)
{
	CString query = _T("");
	CString id = _T("");

	// Controllo apici singoli per SQL
	if (nometab->Find(L"'") != -1)
		nometab->Replace(L"'", L"''");

	query.Format(_T("SELECT IDCAT FROM [CATEGORIE] WHERE NOMECAT='%s'"),*nometab);
	dbopen();
	try
	{
		CRecordset righe(serie);
		righe.Open(CRecordset::forwardOnly, query);
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"IDCAT", m_idcat);
			control->SetWindowTextW(m_idcat);
			righe.MoveNext();
		}
		righe.Close();
		serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}

}

int CDbConn::ContaOmonimi(CString nome, CString tabella, short cat)
{
	int count = 0;
	CString sql = _T("");

	sql.Format(_T("SELECT nome FROM %s WHERE IDCAT=%d AND nome='%s'"),tabella,cat,nome);

	dbopen();
	try
	{
		CRecordset righe(serie);
		righe.Open(CRecordset::forwardOnly, sql);
		while (!righe.IsEOF())
		{
			count = righe.GetRecordCount();
			righe.MoveNext();
		}
		righe.Close();
		serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return -1;
	}

	return count;
}

void CDbConn::GetGruppi(CArray<CString, CString> *pStruct)
{
	dbopen();
	try
	{
		CString sql,id,nome,buf = _T("");
		sql.Format(_T("SELECT * FROM [ETICHETTE]"));
		CRecordset righe(serie);
		righe.Open(CRecordset::forwardOnly, sql);
		while (!righe.IsEOF())
		{
			righe.GetFieldValue(L"NOME", nome);
			righe.GetFieldValue(L"IDL", id);
			pStruct->Add(id + L"° " + nome);
			righe.MoveNext();
		}
		righe.Close();
		serie->Close();
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return;
	}
}