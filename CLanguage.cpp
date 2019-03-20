#include "stdafx.h"
#include "CLanguage.h"
#include "CDbConn.h"

CLanguage m_currlang;

CLanguage::CLanguage()
{
	retStr =_T("");
	sql = _T("");
	m_dbfLang = _T("");
	m_db = _T("");
}


CLanguage::~CLanguage()
{
}

BOOL CLanguage::dbopen()
{
	if (!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Path")))
	{
		ULONG len = 255;
		chiave.QueryStringValue((_T("SharedMDB")), retUser.GetBufferSetLength(len), &len);
		m_dbfLang.Format(_T("%s/language.mdb"), retUser);
	}
	try
	{
		m_db.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s;PWD=%s"), _T("MICROSOFT ACCESS DRIVER (*.mdb)"), m_dbfLang, m_dbPass);
		if (!language.Open(NULL, FALSE, FALSE, m_db))
			return FALSE;
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Database error: ") + e->m_strError);
		return FALSE;
	}
	return TRUE;
}

// A seconda della chiave di registro prendo la traduzione
CString CLanguage::GetDesc(long iddescr, CString temp)
{
	CRegKey chiave;
	ULONG len = 10;

	if (!dbopen())
	{
		AfxMessageBox(_T("Error: database not found in "+ retUser + " the program will be close."));
		PostQuitMessage(0);
	}
	if(!chiave.Open(HKEY_CURRENT_USER, _T("Software\\MC SOFTWARE\\Note Series\\Settings")))
	{
		chiave.QueryStringValue(_T("Lingua"), retStr.GetBufferSetLength(len), &len);
		retStr.ReleaseBuffer();
		if (retStr == _T("ITALIANO"))
		{
			sql.Format(_T("SELECT DESCR FROM [ITALIANO] WHERE IDDESCR = %d"), iddescr);
		}
		else
		{
			sql.Format(_T("SELECT DESCR FROM [ENGLISH] WHERE IDDESCR = %d"), iddescr);
		}
		try
		{
			CRecordset righe(&language);
			righe.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
			while (!righe.IsEOF())
			{
				righe.GetFieldValue(L"DESCR", temp);
				righe.MoveNext();
			}
			righe.Close();
			language.Close();
		}
		catch (CDBException* e)
		{
			AfxMessageBox(_T("Database error: ") + e->m_strError);
		}
	}
	return temp;
}