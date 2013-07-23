#include "StdAfx.h"
#include "Info.h"

IMPLEMENT_DYNAMIC(CInfo, CObject)


CInfo::CInfo(void)
{
}


CInfo::~CInfo(void)
{
}


CInfo::CInfo(InfoType itInfoType):
		m_InfoType(itInfoType)
{
	
}


CInfo::CInfo(const CInfo& info)
{
	m_InfoType = info.m_InfoType;
}

void CInfo::operator=(const CInfo& info)
{
	m_InfoType = info.m_InfoType;
}
