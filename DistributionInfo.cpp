#include "StdAfx.h"
#include "DistributionInfo.h"

//IMPLEMENT_DYNAMIC ( CDistributionInfo, CObject )

CDistributionInfo::CDistributionInfo(double mean, double cov, int iDistType ):
	m_mean(mean), m_cov(cov), m_DistType(iDistType) //, CInfo(CInfo::iDistributionInfo)
{

}

void CDistributionInfo::UpdateDistribution(double mean, double cov, int iDistType )
{
	m_mean = mean;
	m_cov = cov;
	m_DistType = iDistType;
}


CDistributionInfo::~CDistributionInfo(void)
{
}


int CDistributionInfo::GetDistributionType(void)
{
	return m_DistType;
}

double CDistributionInfo::GetMean(void)
{
	return m_mean;
}

double CDistributionInfo::GetCOV(void)
{
	return m_cov;
}


CDistributionInfo::CDistributionInfo(void) //: CInfo(CInfo::iDistributionInfo)
{
}

void CDistributionInfo::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
		ar << m_mean << m_cov << m_DistType;
	else
		ar >> m_mean >> m_cov >> m_DistType;
}


//CDistributionInfo::CDistributionInfo(const CDistributionInfo& distInfo)
//{
//	m_mean = distInfo.m_mean;
//	m_cov = distInfo.m_cov;
//	m_DistType = distInfo.m_DistType;
//}
//
//void CDistributionInfo::operator=(const CDistributionInfo& distInfo)
//{
//	m_mean = distInfo.m_mean;
//	m_cov = distInfo.m_cov;
//	m_DistType = distInfo.m_DistType;
//}
//
