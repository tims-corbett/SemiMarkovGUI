#pragma once
#include "afxwin.h"

class CSemiMarkovGUIDoc;
class CStateInfo;


// CNodeInfoDlg dialog
class CNodeInfoDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(CNodeInfoDlg)

	CSemiMarkovGUIDoc* m_pDocument;
	CStateInfo* m_pStateInfo;

public:
	CNodeInfoDlg(CSemiMarkovGUIDoc* pDocument, CStateInfo* pStateInfo,  CWnd* pParent = NULL);   // standard constructor
	virtual ~CNodeInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_STATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void ClearFields(void);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnModelAddnode();
	CEdit m_StateName;
	CEdit m_StateShortName;
	//CString m_StateName;
	//CString m_StateShortName;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CButton m_OKButton;
	afx_msg void OnChangeEditName();
};
