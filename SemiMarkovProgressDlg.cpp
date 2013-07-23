// SemiMarkovProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SemiMarkovGUI.h"
#include "SemiMarkovProgressDlg.h"
#include "afxdialogex.h"


// CSemiMarkovProgressDlg dialog

IMPLEMENT_DYNAMIC(CSemiMarkovProgressDlg, CDialogEx)

CSemiMarkovProgressDlg::CSemiMarkovProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSemiMarkovProgressDlg::IDD, pParent)
{

}

CSemiMarkovProgressDlg::~CSemiMarkovProgressDlg()
{
}

void CSemiMarkovProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SMP, m_SemiMarkovProgressBar);
}


BEGIN_MESSAGE_MAP(CSemiMarkovProgressDlg, CDialogEx)
END_MESSAGE_MAP()


// CSemiMarkovProgressDlg message handlers
