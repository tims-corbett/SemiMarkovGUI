// RunSemiMarkovDlg.cpp : implementation file
//http://stackoverflow.com/questions/1372967/how-do-you-use-createthread-for-functions-which-are-class-members

#include "stdafx.h"
#include "SemiMarkovGUI.h"
#include "RunSemiMarkovDlg.h"
#include "afxdialogex.h"
#include "SemiMarkovGUIDoc.h"

#include "TransitionCollection.h"
#include "StateCollection.h"

#include "../BoostTest/SemiMarkovModel.h"
#include "../BoostTest/TestResults.h"


#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>


// CRunSemiMarkovDlg dialog

IMPLEMENT_DYNAMIC(CRunSemiMarkovDlg, CDialogEx)

	CRunSemiMarkovDlg::CRunSemiMarkovDlg(CSemiMarkovGUIDoc* pDoc,  CWnd* pParent /*=NULL*/)
	: CDialogEx(CRunSemiMarkovDlg::IDD, pParent)
	, m_dXAxisMaximum(0)
	, m_lTrapZPoints(0)
{
	m_pDoc = pDoc;
	m_pTransitionList = pDoc->GetTransitionList();
}

CRunSemiMarkovDlg::~CRunSemiMarkovDlg()
{
}

void CRunSemiMarkovDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_XAXISMAX, m_dXAxisMaximum);
	DDX_Text(pDX, IDC_EDIT_POINTS, m_lTrapZPoints);
	DDX_Control(pDX, IDC_COMBO_INIT_STATE, m_ComboStates);
	DDX_Control(pDX, IDC_PROGRESS_SMP, m_ComputationProgress);
}


BEGIN_MESSAGE_MAP(CRunSemiMarkovDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRunSemiMarkovDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRunSemiMarkovDlg message handlers


void CRunSemiMarkovDlg::OnBnClickedOk()
{
	UpdateData();
	
	bIsProgressing = true;
	m_iInitialState = m_ComboStates.GetCurSel();
	m_ComputationProgress.SetRange32(0, m_lTrapZPoints);
	
	DWORD tid;
	HANDLE h = CreateThread(NULL, 0, StaticSemiMarkovWorkerThread, (void*)this, 0, &tid);
	CloseHandle(h);

	while(bIsProgressing)
		WaitForProcessing();

	CDialogEx::OnOK();
}




BOOL CRunSemiMarkovDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	std::vector<std::string> states = m_pTransitionList->GetStateList()->GetStateNames();
	CString temp;
	
	for(int i=0; i<states.size() ; i++)
	{
		temp = CString(states.at(i).c_str());
		m_ComboStates.AddString(temp);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CRunSemiMarkovDlg::SemiMarkovWorkerThread()
{

	JaggedMatrix *pJagged = m_pTransitionList->GetJaggedMatrix();

	SemiMarkovModel smpWbl(pJagged);
	smpWbl.SetModelInput(m_dXAxisMaximum, m_lTrapZPoints  );
	smpWbl.SetupMatrices();

	smpWbl.ComputeStateProbabilities(
		boost::bind(&CRunSemiMarkovDlg::DisplayTimeStep, this, _1)
		);

	while( smpWbl.bContinue == true )
		WaitForProcessing();

	std::ostringstream fpath;
	fpath << "newtest.dat";
	TestResults sysresults(fpath.str());

	vector<double> time = smpWbl.GetTimeVector();
	sysresults.AddResult(time);

	
	for(int j=0; j < m_pTransitionList->GetStateList()->GetStateCount(); j++)
	{
		vector<double> pij = smpWbl.GetStateProbability(m_iInitialState, j); 
		sysresults.AddResult(pij);
	}

	sysresults.Serialize( );

	delete pJagged;
	bIsProgressing = false;

	return 0;
}


void CRunSemiMarkovDlg::DisplayTimeStep(SemiMarkovModel* pModel)
{
	static CString temp;
	boost::xtime xt;
	try
	{
		while(pModel->bContinue  == true )
		{
			m_ComputationProgress.SetPos(pModel->timeStep);
			boost::xtime_get(&xt, boost::TIME_UTC);
			xt.sec += 1; // change xt to next second
			boost::thread::sleep(xt); 
		}
	}catch(...)
	{
	}

	m_ComputationProgress.SetPos(pModel->timeStep);

}


DWORD WINAPI CRunSemiMarkovDlg::StaticSemiMarkovWorkerThread(void* param)
{
	CRunSemiMarkovDlg* pDlg = (CRunSemiMarkovDlg*)param;
	return pDlg->SemiMarkovWorkerThread();
}


void CRunSemiMarkovDlg::WaitForProcessing(void)
{
	MSG msg;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if ( ::GetMessage(&msg, NULL, 0, 0))
		{
			::TranslateMessage(&msg);
			:: DispatchMessage(&msg);
		}
		else
			break;
	}

}
