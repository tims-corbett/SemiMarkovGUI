#include "StdAfx.h"
#include "TransitionCollection.h"
#include "../BoostTest/JaggedMatrix.h"
#include "StateCollection.h"
#include "DistributionInfo.h"
#include "TransitionInfo.h"
#include "StateInfo.h"

CTransitionCollection::CTransitionCollection(void)
{
}

void CTransitionCollection::SetStateList(CStateCollection* pStateCollection)
{
	m_pStateCollection = pStateCollection;
}

void CTransitionCollection::ClearCollection(void)
{
	for( int i=0; i < m_TransitionList.GetCount(); i++)
		delete m_TransitionList[i];
	m_TransitionList.RemoveAll();
}

CTransitionCollection::~CTransitionCollection(void)
{
	ClearCollection();
}

CTransitionInfo* CTransitionCollection::GetTransition(CStateInfo* pFromState, CStateInfo* pToState)
{
	CTransitionInfo* pTransition = NULL;
	bool bFound = false;

	for( int i=0; i < m_TransitionList.GetCount(); i++)
	{
		pTransition = m_TransitionList[i];
		if(pTransition->GetFromState() == pFromState && pTransition->GetToState() == pToState)
		{
			bFound = true;
			break;
		}
	}

	return (bFound == true) ? pTransition : NULL;
}

CTransitionInfo* CTransitionCollection::GetNewTransition(CStateInfo* pFromState, CStateInfo* pToState, CString transitionLabel)
{
	CTransitionInfo* pTransition = new CTransitionInfo(pFromState, pToState, transitionLabel);
	m_TransitionList.Add(pTransition);
	return pTransition;
}

CTransitionInfo* CTransitionCollection::GetTransition(int i)
{
	return m_TransitionList[i];
}

int CTransitionCollection::GetTransitionCount(void)
{
	return m_TransitionList.GetCount();
}

CStateCollection* CTransitionCollection::GetStateList(void)
{
	return m_pStateCollection;
}

JaggedMatrix* CTransitionCollection::GetJaggedMatrix(void)
{
	JaggedMatrix* pJagged = new JaggedMatrix(m_pStateCollection->GetStateCount());

	CTransitionInfo* pTransition;
	CDistributionInfo* pDistribution;

	for( int i=0; i < m_TransitionList.GetCount(); i++)
	{
		pTransition = m_TransitionList[i];

		for(int j=0; j<pTransition->GetDistributionCount(); j++)
		{
			pDistribution = pTransition->GetDistribution(j);
			pJagged->AddDistribution(
				pTransition->GetFromState()->GetStateNumber(),
				pTransition->GetToState()->GetStateNumber(),
				pDistribution->GetMean(),
				pDistribution->GetCOV(),
				pDistribution->GetDistributionType()
				);
		}
	}

	return pJagged;
}

void CTransitionCollection::Serialize(CArchive& ar)
{
	int count, i;
	CTransitionInfo* pInfo;

	if(ar.IsStoring())
	{
		count = m_TransitionList.GetCount();
		ar << count;
		for(int i = 0; i < count; i++)
			m_TransitionList[i]->Serialize(ar, NULL, NULL);
	}
	else
	{
		int iFromState, iToState;

		ar >> count;
		for(int i = 0; i < count; i++)
		{
			pInfo = new CTransitionInfo();
			pInfo->Serialize(ar, &iFromState, &iToState);
			pInfo->SetTransitionStates( m_pStateCollection->GetState(iFromState), m_pStateCollection->GetState(iToState) );
			m_TransitionList.Add(pInfo);
		}
	}
}

