#pragma once
#include "afxwin.h"

class CSemiMarkovGUIDoc;
class CTransitionCollection;
class CTransitionInfo;
class CDistributionInfo;

// CTransitionDlg dialog

class CTransitionDlg : public CDialogEx
{
	

public:
	CTransitionDlg( CTransitionCollection* pTransitionList,
					CTransitionInfo* pTransitionInfo,
					CDistributionInfo* pDistributionInfo,  
					CWnd* pParent = NULL);   // standard constructor

	virtual ~CTransitionDlg();
	void ClearFields(void);

// Dialog Data
	enum { IDD = IDD_TRANSITIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CTransitionCollection* m_pTransitionList;
	CTransitionInfo* m_pTransitionInfo;
	CDistributionInfo* m_pDistributionInfo;

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboToState;
	CComboBox m_ComboFromState;
	CComboBox m_ComboDistribution;
	CEdit m_EditCOV;
	CEdit m_EditMean;
	CTreeCtrl m_TreeTransitions;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CEdit m_EditTransitionLabel;
	CButton m_ButtonOK;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboDist();
};
