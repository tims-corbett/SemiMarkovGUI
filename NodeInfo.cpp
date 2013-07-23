// NodeInfo.cpp : implementation file
//

#include "stdafx.h"
#include "SemiMarkovGUI.h"
#include "SemiMarkovGUIDoc.h"
#include "SemiMarkovGUIView.h"
#include "NodeInfo.h"
#include "afxdialogex.h"


// CNodeInfoDlg dialog

//IMPLEMENT_DYNAMIC(CNodeInfoDlg, CDialogEx)

CNodeInfoDlg::CNodeInfoDlg(CSemiMarkovGUIDoc* pDocument, CStateInfo* pStateInfo, CWnd* pParent /*=NULL*/)
	: CDialogEx(CNodeInfoDlg::IDD, pParent)
{
	m_pDocument = pDocument;
	m_pStateInfo = pStateInfo;

}

CNodeInfoDlg::~CNodeInfoDlg()
{
}

void CNodeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_StateName);
	DDX_Control(pDX, IDC_EDIT_SHORT_NAME, m_StateShortName);
	//DDX_Text(pDX, IDC_EDIT_NAME, m_StateName);
	//DDX_Text(pDX, IDC_EDIT_SHORT_NAME, m_StateShortName);
	DDX_Control(pDX, IDOK, m_OKButton);
}


BEGIN_MESSAGE_MAP(CNodeInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNodeInfoDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CNodeInfoDlg::OnChangeEditName)
END_MESSAGE_MAP()


// CNodeInfoDlg message handlers




void CNodeInfoDlg::OnBnClickedOk()
{

	CString mStateName, mStateShortName;

	m_StateName.GetWindowTextW(mStateName);
	m_StateShortName.GetWindowTextW(mStateShortName);

	if( m_pStateInfo == NULL)
	{
		CStateInfo* state = new CStateInfo();
		state->SetStateInfo(mStateName, mStateShortName, m_pDocument->GetStateCount());
		m_pDocument->AddState(state);
	}
	else
		m_pStateInfo->SetStateInfo(mStateName, mStateShortName, m_pStateInfo->GetStateNumber()  );
	
	
	((CSemiMarkovGUIView*)m_pParentWnd)->UpdateTransitionView();
	ClearFields();

	if(m_pStateInfo != NULL)
		CDialogEx::OnOK();
}

void CNodeInfoDlg::ClearFields(void)
{
	m_StateName.SetWindowTextW(L"");
	m_StateShortName.SetWindowTextW(L"");
	m_StateName.SetFocus();

	UpdateData(false);
}

BOOL CNodeInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if( m_pStateInfo != NULL)
	{
		m_StateName.SetWindowTextW( m_pStateInfo->GetStateName());
		m_StateShortName.SetWindowTextW(m_pStateInfo->GetStateShortName());
		m_OKButton.SetWindowTextW(CString(MAKEINTRESOURCE(IDS_STRING_MODIFY_STATE)) );

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNodeInfoDlg::OnChangeEditName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	CString shortName;
	m_StateShortName.GetWindowTextW(shortName);

	if(shortName.IsEmpty() == false)
		return;

	CString longName;
	m_StateName.GetWindowTextW(longName);

	if(longName.IsEmpty())
		return;

	shortName.Format( L"%c", longName.GetAt(0) );
	m_StateShortName.SetWindowTextW( shortName);


	// TODO:  Add your control notification handler code here
}
