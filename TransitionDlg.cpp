// TransitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SemiMarkovGUI.h"
#include "TransitionDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "SemiMarkovGUIDoc.h"
#include "SemiMarkovGUIView.h"

#include "TransitionCollection.h"
#include "TransitionInfo.h"
#include "DistributionInfo.h"



#include<cstringt.h>

// CTransitionDlg dialog

CTransitionDlg::CTransitionDlg( CTransitionCollection* pTransitionList, CTransitionInfo* pTransitionInfo, CDistributionInfo* pDistributionInfo, CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransitionDlg::IDD, pParent)
{
	m_pTransitionList = pTransitionList;
	m_pTransitionInfo = pTransitionInfo;
	m_pDistributionInfo = pDistributionInfo;
}

CTransitionDlg::~CTransitionDlg()
{
}

void CTransitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_COMBO_FROM, m_ComboFrom);
	DDX_Control(pDX, IDC_COMBO_To, m_ComboToState);
	DDX_Control(pDX, IDC_COMBO_FROM, m_ComboFromState);
	DDX_Control(pDX, IDC_COMBO_DIST, m_ComboDistribution);
	DDX_Control(pDX, IDC_EDIT_COV, m_EditCOV);
	DDX_Control(pDX, IDC_EDIT_MEAN, m_EditMean);
	DDX_Control(pDX, IDC_EDIT_LABEL, m_EditTransitionLabel);
	DDX_Control(pDX, IDOK, m_ButtonOK);
}


BEGIN_MESSAGE_MAP(CTransitionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTransitionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTransitionDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_DIST, &CTransitionDlg::OnCbnSelchangeComboDist)
END_MESSAGE_MAP()


// CTransitionDlg message handlers


BOOL CTransitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

		
	std::vector<std::string> states = m_pTransitionList->GetStateList()->GetStateNames();
	CString temp;
	
	for(int i=0; i<states.size() ; i++)
	{
		temp = CString(states.at(i).c_str());
		m_ComboFromState.AddString(temp);
		m_ComboToState.AddString(temp);
	}

	CString sDistributions[] = {
		CString(MAKEINTRESOURCE( IDS_STRING_UNIFORM )),
		CString(MAKEINTRESOURCE( IDS_STRING_EXPONENTIAL )),
		CString(MAKEINTRESOURCE( IDS_STRING_GAMMA )),
		CString(MAKEINTRESOURCE( IDS_STRING_GUMBELMAX )),
		CString(MAKEINTRESOURCE( IDS_STRING_LOGISTIC )),
		CString(MAKEINTRESOURCE( IDS_STRING_LOGNORMAL )),
		CString(MAKEINTRESOURCE( IDS_STRING_NORMAL )),
		CString(MAKEINTRESOURCE( IDS_STRING_WEIBULL )),
		CString(MAKEINTRESOURCE( IDS_STRING_POLYWEIBULL ))
	};

	for(int i=0; i < 9; i++)
		m_ComboDistribution.InsertString(i, sDistributions[i] );


	if( m_pTransitionInfo == NULL)
		m_ComboDistribution.SetCurSel(1);
	else
	{
		m_ButtonOK.SetWindowTextW(CString( MAKEINTRESOURCE(IDS_STRING_MODIFY_TRANSITION)) );

		m_ComboDistribution.SetCurSel(m_pDistributionInfo->GetDistributionType() );
		m_ComboFromState.SetCurSel(m_pTransitionInfo->GetFromState()->GetStateNumber());
		m_ComboToState.SetCurSel(m_pTransitionInfo->GetToState()->GetStateNumber() );

		CString strNum;
		strNum.Format(L"%f", m_pDistributionInfo->GetMean()  );
		m_EditMean.SetWindowTextW( strNum  );

		strNum.Format(L"%f", m_pDistributionInfo->GetCOV()  );
		m_EditCOV.SetWindowTextW(strNum);

		m_EditTransitionLabel.SetWindowTextW( m_pTransitionInfo->GetTransitionLabel() );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTransitionDlg::OnBnClickedOk()
{
	CStateCollection *pStateList = m_pTransitionList->GetStateList();
	CStateInfo *pFromState = pStateList->GetState( m_ComboFromState.GetCurSel() );
	CStateInfo *pToState = pStateList->GetState( m_ComboToState.GetCurSel() );

	CString transitionLabel;
	m_EditTransitionLabel.GetWindowTextW(transitionLabel);


	CString temp;
	m_EditMean.GetWindowTextW(temp);
	double mean = _wtof(temp);

	m_EditCOV.GetWindowTextW(temp);
	double cov = _wtof(temp);

	if( m_pTransitionInfo == NULL)	//Add Transition
	{
		CTransitionInfo *pTransition = m_pTransitionList->GetTransition(pFromState, pToState);

		if(pTransition == NULL)
			pTransition = m_pTransitionList->GetNewTransition(pFromState, pToState, transitionLabel);

		pTransition->AddDistribution(mean, cov, (DistributionFactory::DistributionType)m_ComboDistribution.GetCurSel() );
	}
	else		//Modify transition
	{
		m_pDistributionInfo->UpdateDistribution(mean, cov, (DistributionFactory::DistributionType)m_ComboDistribution.GetCurSel() );
		m_pTransitionInfo->UpdateTransition(pFromState, pToState, transitionLabel);
	}

	((CSemiMarkovGUIView*)m_pParentWnd)->UpdateTransitionView();

	ClearFields();

	if( m_pTransitionInfo != NULL)	//Add Transition
		CDialogEx::OnOK();
}


void CTransitionDlg::ClearFields(void)
{
	m_ComboFromState.SetCurSel(-1);
	m_ComboToState.SetCurSel(-1);
	m_ComboDistribution.SetCurSel(1);
	m_EditCOV.SetWindowTextW(L"");
	m_EditMean.SetWindowTextW(L"");
	m_EditTransitionLabel.SetWindowTextW(L"");

	m_ComboFromState.SetFocus();

}

void CTransitionDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CTransitionDlg::OnCbnSelchangeComboDist()
{
	// TODO: Add your control notification handler code here
}
