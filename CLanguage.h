#pragma once
class CLanguage: public CDbConn
{
public:
	CLanguage();
	CString GetDesc(long iddescr, CString temp);
	CString retStr, sql;
	BOOL dbopen() override;
	CDatabase language; // Oggetto databsse
	CString m_db;		// Stringa di connessione coi parametri
	~CLanguage();
};

