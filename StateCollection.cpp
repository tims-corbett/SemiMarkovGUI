#include "StdAfx.h"
#include "StateCollection.h"
#include "StateInfo.h"

#include<string>

CStateCollection::CStateCollection(void)
{
}


void CStateCollection::ClearCollection(void)
{
	for( int i=0; i<m_lstStateInfoList.GetCount(); i++ )
		delete m_lstStateInfoList[i];

	m_lstStateInfoList.RemoveAll();
}

CStateCollection::~CStateCollection(void)
{
	ClearCollection();
}

void CStateCollection::AddState(CStateInfo* oStateInfo)
{
	m_lstStateInfoList.Add(oStateInfo);
}

std::vector<std::string> CStateCollection::GetStateNames(void)
{
	std::vector<std::string> stateList;


	for( int i=0; i<m_lstStateInfoList.GetCount(); i++ )
	{
		std::string s = CStringA(m_lstStateInfoList[i]->GetStateName());
		stateList.push_back(s);
	}

	return stateList;
}

int CStateCollection::GetStateCount(void)
{
	return m_lstStateInfoList.GetCount();
}

CStateInfo* CStateCollection::GetState(int i)
{
	return m_lstStateInfoList[i];
}

void CStateCollection::Serialize(CArchive& ar)
{
	int count;
	
	if( ar.IsStoring() )
	{
		count = m_lstStateInfoList.GetCount();
		ar << count;

		for( int i=0; i < count; i++ )
			m_lstStateInfoList[i]->Serialize(ar);
	}
	else
	{
		CStateInfo* pInfo;

		ar >> count;
		for( int i=0; i < count; i++ )
		{
			pInfo = new CStateInfo();
			pInfo->Serialize(ar);
			m_lstStateInfoList.Add( pInfo );
		}
	}
}