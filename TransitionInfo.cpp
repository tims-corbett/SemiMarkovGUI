

#include "StdAfx.h"
#include "TransitionInfo.h"
#include "Info.h"
#include "StateInfo.h"
#include "DistributionInfo.h"

//IMPLEMENT_DYNAMIC(CTransitionInfo, CObject)

CTransitionInfo::CTransitionInfo(CStateInfo* pFromState, CStateInfo* pToState, CString transitionLabel):
	m_pFromState(pFromState), m_pToState(pToState), m_TranstionLabel(transitionLabel) //, CInfo(CInfo::iTransitionInfo)
{
}


void CTransitionInfo::UpdateTransition(CStateInfo* pFromState, CStateInfo* pToState, CString transitionLabel)
{
	m_pFromState = pFromState;
	m_pToState = pToState;
	m_TranstionLabel = transitionLabel;
}

void CTransitionInfo::AddDistribution(double mean, double cov, int iDistType)
{
	CDistributionInfo* temp = new CDistributionInfo(mean, cov, iDistType);
	m_DistributionList.Add(temp);
}

CTransitionInfo::CTransitionInfo()
{
	m_pFromState=NULL;
	m_pToState=NULL;
}

CTransitionInfo::~CTransitionInfo()
{
	for(int i=0; i < m_DistributionList.GetCount(); i++)
		delete m_DistributionList[i];

	m_DistributionList.RemoveAll();
}

CStateInfo* CTransitionInfo::GetFromState(void)
{
	return m_pFromState;
}

CStateInfo* CTransitionInfo::GetToState(void)
{
	return m_pToState;
}

int CTransitionInfo::GetDistributionCount(void)
{
	return m_DistributionList.GetCount();
}

CDistributionInfo* CTransitionInfo::GetDistribution(int i)
{
	return m_DistributionList[i];
}

CString CTransitionInfo::GetTransitionLabel(void)
{
	return m_TranstionLabel;
}

void CTransitionInfo::Serialize(CArchive& ar, int* iFromState, int* iToState)
{
	int i;

	if(ar.IsStoring())
	{
		ar	<< m_pFromState->GetStateNumber()
			<< m_pToState->GetStateNumber()
			<< m_TranstionLabel
			<< m_DistributionList.GetCount();

		for( i = 0; i < m_DistributionList.GetCount(); i++)
			m_DistributionList[i]->Serialize(ar);
	}
	else
	{
		int iDistributionCount;

		ar	>> *iFromState 
			>> *iToState
			>> m_TranstionLabel
			>> iDistributionCount;

		CDistributionInfo* pDistributionInfo;

		for(i = 0; i < iDistributionCount; i++)
		{
			pDistributionInfo = new CDistributionInfo();
			pDistributionInfo->Serialize(ar);
			m_DistributionList.Add(pDistributionInfo);
		}
	}
}				

void CTransitionInfo::SetTransitionStates(CStateInfo* pFromState, CStateInfo* pToState)
{
	m_pFromState = pFromState;
	m_pToState = pToState;
}




//CTransitionInfo::CTransitionInfo(CTransitionInfo& pInfo)
//{
//  m_pFromState = pInfo.m_pFromState;
//  m_pToState = pInfo.m_pToState;
//  m_TranstionLabel = pInfo.m_TranstionLabel;
//  m_DistributionList= pInfo.m_DistributionList;
//}
//
//
//void CTransitionInfo::operator=(CTransitionInfo& pInfo)
//{
//  m_pFromState = pInfo.m_pFromState;
//  m_pToState = pInfo.m_pToState;
//  m_TranstionLabel = pInfo.m_TranstionLabel;
//  m_DistributionList= pInfo.m_DistributionList;
//}
