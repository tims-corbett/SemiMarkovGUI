#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CTransitionCollection;
class CSemiMarkovGUIDoc;
class SemiMarkovModel;

// CRunSemiMarkovDlg dialog

class CRunSemiMarkovDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRunSemiMarkovDlg)

public:
	CRunSemiMarkovDlg(CSemiMarkovGUIDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRunSemiMarkovDlg();
	static DWORD WINAPI StaticSemiMarkovWorkerThread(void* param);
	DWORD SemiMarkovWorkerThread();
	bool bIsProgressing;

// Dialog Data
	enum { IDD = IDD_DIALOG_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CTransitionCollection* m_pTransitionList;
	CSemiMarkovGUIDoc* m_pDoc;
	void WaitForProcessing(void);

	void DisplayTimeStep(SemiMarkovModel* pModel);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	double m_dXAxisMaximum;
	int m_lTrapZPoints;
	CComboBox m_ComboStates;
	int m_iInitialState;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_ComputationProgress;
};
