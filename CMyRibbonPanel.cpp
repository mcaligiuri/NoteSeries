#include "stdafx.h"
#include "CMyRibbonPanel.h"


CMyRibbonPanel::CMyRibbonPanel(){}

CMyRibbonPanel::~CMyRibbonPanel(){}

// Modifico il nome del pannello nella ribbon bar
void CMyRibbonPanel::SetName(CString & name){m_strName = name;}
