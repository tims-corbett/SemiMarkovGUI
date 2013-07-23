#include "StdAfx.h"
#include "StateInfo.h"

//IMPLEMENT_DYNAMIC( CStateInfo, CObject )
	
CStateInfo::CStateInfo()
{
}


CStateInfo::~CStateInfo()
{
}


CString CStateInfo::GetStateName(void)
{
	return m_szStateName;
}

void CStateInfo::SetStateInfo( CString szStateName, CString szStateShortName, int iStateNumber)
{
	m_szStateName = szStateName;
	m_szStateShortName = szStateShortName;
	m_StateNumber = iStateNumber;
}

int CStateInfo::GetStateNumber(void)
{
	return m_StateNumber;
}

CString CStateInfo::GetStateShortName(void)
{
	return m_szStateShortName;
}

void CStateInfo::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
		ar << m_szStateName << m_szStateShortName << m_StateNumber;
	else
		ar >> m_szStateName >> m_szStateShortName >> m_StateNumber;
}



//CStateInfo::CStateInfo(const CStateInfo& oStateInfo)
//{
//	m_StateNumber = oStateInfo.m_StateNumber;
//	m_szStateName = oStateInfo.m_szStateName;
//	m_szStateShortName = oStateInfo.m_szStateShortName;
//}
//
//void CStateInfo::operator=(const CStateInfo& oStateInfo)
//{
//	m_StateNumber = oStateInfo.m_StateNumber;
//	m_szStateName = oStateInfo.m_szStateName;
//	m_szStateShortName = oStateInfo.m_szStateShortName;
//}
