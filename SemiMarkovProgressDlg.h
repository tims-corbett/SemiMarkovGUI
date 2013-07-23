#pragma once
#include "afxcmn.h"


// CSemiMarkovProgressDlg dialog

class CSemiMarkovProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSemiMarkovProgressDlg)

public:
	CSemiMarkovProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSemiMarkovProgressDlg();

// Dialog Data

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_SemiMarkovProgressBar;
};
