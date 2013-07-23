
// SemiMarkovGUIView.cpp : implementation of the CSemiMarkovGUIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SemiMarkovGUI.h"
#endif

#include "SemiMarkovGUIDoc.h"
#include "SemiMarkovGUIView.h"
#include "NodeInfo.h"
#include "MainFrm.h"
#include "OutputWnd.h"
#include "TransitionCollection.h"
#include "StateCollection.h"
#include "SemiMarkovProgressDlg.h"

#include "../BoostTest/TestResults.h"

#include "RunSemiMarkovDlg.h"
#include "TransitionDlg.h"

#include "../BoostTest/SemiMarkovModel.h"
#include "../BoostTest/TestResults.h"
#include "../BoostTest/JaggedMatrix.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CSemiMarkovGUIView

IMPLEMENT_DYNCREATE(CSemiMarkovGUIView, CView)

BEGIN_MESSAGE_MAP(CSemiMarkovGUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSemiMarkovGUIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_WM_QUERYOPEN()
ON_COMMAND(ID_MODEL_ADDNODE, &CSemiMarkovGUIView::OnModelAddnode)
ON_COMMAND(ID_MODEL_ADDTRANSITION, &CSemiMarkovGUIView::OnModelAddtransition)
ON_WM_SHOWWINDOW()
ON_COMMAND(ID_MODEL_RUNMODEL, &CSemiMarkovGUIView::OnModelRunmodel)
END_MESSAGE_MAP()

// CSemiMarkovGUIView construction/destruction

CSemiMarkovGUIView::CSemiMarkovGUIView()
{
	// TODO: add construction code here
	m_pOutputWindow = NULL;


}

CSemiMarkovGUIView::~CSemiMarkovGUIView()
{
}

BOOL CSemiMarkovGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSemiMarkovGUIView drawing

void CSemiMarkovGUIView::OnDraw(CDC* pDC)
{
	
	CSemiMarkovGUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if(bResultRequested == false)
		return;
	
	CRect		clientRect;
	int			nOldMode;

	nOldMode = pDC->GetMapMode();
	GetClientRect( clientRect );
	nOldMode = pDC->SetMapMode( MM_LOMETRIC );
	pDC->DPtoLP( (LPPOINT) &clientRect, 2 );
	m_Chart.OnDraw( pDC, clientRect );
	pDC->SetMapMode( nOldMode );
	
}


// CSemiMarkovGUIView printing


void CSemiMarkovGUIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSemiMarkovGUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSemiMarkovGUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSemiMarkovGUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSemiMarkovGUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSemiMarkovGUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSemiMarkovGUIView diagnostics

#ifdef _DEBUG
void CSemiMarkovGUIView::AssertValid() const
{
	CView::AssertValid();
}

void CSemiMarkovGUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSemiMarkovGUIDoc* CSemiMarkovGUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSemiMarkovGUIDoc)));
	return (CSemiMarkovGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CSemiMarkovGUIView message handlers
const int CTRL_ID = 280;

int CSemiMarkovGUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	// TODO:  Add your specialized creation code here
  //Create the control, just make sure to use WS_CHILD and | WS_VISIBLE.

    if (!m_webBrowser.Create(L"SMPWebBrowserWnd", WS_CHILD  | WS_VISIBLE ,  CRect(0, 0, 0, 0), this, CTRL_ID))
    {
      //  AfxMessageBox("Failed to create adobe wnd");
        return -1;
    }

	m_pOutputWindow = ((CMainFrame*)GetParentFrame())->GetOutputWindow();
	redirector.m_pOutput = m_pOutputWindow;


	return 0;
}


void CSemiMarkovGUIView::OnSize(UINT nType, int cx, int cy)
{

	CView::OnSize(nType, cx, cy);

	  //Resize the control with the window.

    if (IsWindow(m_hWnd))
    {
        CRect rc;
        GetClientRect(rc);
        m_webBrowser.MoveWindow(rc);
    }
}



void CSemiMarkovGUIView::OnModelAddnode()
{

	ModifyState(NULL);
}

void CSemiMarkovGUIView::ModifyState(CStateInfo* pStateInfo)
{
	CNodeInfoDlg nodeInfo(GetDocument(), pStateInfo, this);
	HRESULT result = nodeInfo.DoModal();

	CString dotFile = GetDocument()->GenerateGraphVizFile();
	DisplayStateSpaceDiagram(dotFile);
}

void CSemiMarkovGUIView::OnModelAddtransition()
{
	ModifyTransition(NULL, NULL);
}

void CSemiMarkovGUIView::ModifyTransition(CTransitionInfo* pTransitionInfo, CDistributionInfo* pDistributionInfo)
{
	CTransitionDlg transitionDlg(GetDocument()->GetTransitionList(),
								pTransitionInfo,
								pDistributionInfo, this);

	HRESULT result  = transitionDlg.DoModal();

	CString dotFile = GetDocument()->GenerateGraphVizFile();
	DisplayStateSpaceDiagram(dotFile);
}

void CSemiMarkovGUIView::DisplayStateSpaceDiagram(CString dotFile)
{
	if(dotFile == L"" || GetDocument()->GetTransitionList()->GetTransitionCount()==0)
		return;

	bResultRequested = false;

	if( ::IsWindow(m_webBrowser.m_hWnd) == false)
	{
		
		if (!m_webBrowser.Create(L"SMPWebBrowserWnd", WS_CHILD  | WS_VISIBLE ,  CRect(0, 0, 0, 0), this, CTRL_ID))
			return;

	   if (IsWindow(m_hWnd))
		{
			CRect rc;
			GetClientRect(rc);
			m_webBrowser.MoveWindow(rc);
		}
	}

	m_webBrowser.ShowWindow(SW_SHOW);


	bNavigationComplete = false;
	m_webBrowser.Navigate(L"file:///C:/Civil-7/Research/multi-state/Proposal/Chapter 5 - SMP/VCProjects/BoostTest/SemiMarkovGUI/PleaseWait.mht",NULL, NULL,NULL,NULL);

	while(bNavigationComplete == false)
		WaitForProcessing();
	
	m_pOutputWindow->SetWindowTextW(L"");
	redirector.Close();
	CString temp = L"cmd /C GeneratePDF.bat newtest";
	redirector.Open((LPCTSTR)temp);

	try{
	while(redirector.RedirectStdout() != 0) //Until all batch process is complete
		WaitForProcessing();		
	}catch(...) {}


	//redirector.Close();
	m_webBrowser.Navigate(L"file:///C:/Civil-7/Research/multi-state/Proposal/Chapter 5 - SMP/VCProjects/BoostTest/SemiMarkovGUI/newtest.pdf",NULL, NULL,NULL,NULL);
	
}

BEGIN_EVENTSINK_MAP(CSemiMarkovGUIView, CView)
   ON_EVENT(CSemiMarkovGUIView, CTRL_ID, 259 /* DocumentComplete */,
            OnDocumentComplete, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


void CSemiMarkovGUIView::OnDocumentComplete(LPDISPATCH lpDisp,
                                    VARIANT FAR* URL)
{
   IUnknown*  pUnk;
   LPDISPATCH lpWBDisp;
   HRESULT    hr;

   pUnk = m_webBrowser.GetControlUnknown();
   ASSERT(pUnk);

   hr = pUnk->QueryInterface(IID_IDispatch, (void**)&lpWBDisp);
   ASSERT(SUCCEEDED(hr));

   if (lpDisp == lpWBDisp )
	   bNavigationComplete = true;

  lpWBDisp->Release();
}

void CSemiMarkovGUIView::WaitForProcessing(void)
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

void CSemiMarkovGUIView::UpdateTransitionView(void)
{
	((CMainFrame*)GetParentFrame())->GetClassView()->FillClassView(GetDocument()->GetTransitionList());
}

void CSemiMarkovGUIView::UpdateDistributionPropertyPage(CTransitionInfo* pTransitionInfo, CDistributionInfo* pDistributionInfo)
{
	((CMainFrame*)GetParentFrame())->GetPropertiesWindow()->UpdateDistributionInfo(pTransitionInfo, pDistributionInfo);
}

void CSemiMarkovGUIView::UpdateTransitionPropertyPage(CTransitionInfo* pTransitionInfo)
{
	((CMainFrame*)GetParentFrame())->GetPropertiesWindow()->UpdateTransitionInfo(pTransitionInfo);
}

void CSemiMarkovGUIView::UpdateStatePropertyPage(CStateInfo* pStateInfo)
{
	((CMainFrame*)GetParentFrame())->GetPropertiesWindow()->UpdateStateInfo(pStateInfo);
}


void CSemiMarkovGUIView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

}


void CSemiMarkovGUIView::OnModelRunmodel()
{
	CRunSemiMarkovDlg dlgRunSMP(GetDocument());
	HRESULT result = dlgRunSMP.DoModal();

	if(result == IDCANCEL)
		return;

}


void CSemiMarkovGUIView::PopulateResults(CStateInfo* pInfo)
{
	if(pInfo == NULL)
		return;

	long rowCount;
	int columnCount;
	
	TestResults fileData("newtest.dat");
	std::vector<double> XYData;

	{
		std::vector<double> entireData = fileData.Load(rowCount, columnCount);
		XYData = fileData.ExtractXY(entireData, pInfo->GetStateNumber(), rowCount, columnCount);
	}

	int		nDims, dims[2];
	int		dataID;
	CAxis	*axis;

	nDims = 2;
	dims[0] = 2;
	dims[1] = rowCount;

	m_Chart.DeleteAllData();
	dataID = m_Chart.AddData( &XYData[0], nDims, dims );


	m_Chart.SetTitle( pInfo->GetStateName() +  L" Probability" );
	axis = m_Chart.AddAxis( kLocationLeft );
	axis->SetTitle( L"Probability" );
	axis = m_Chart.AddAxis( kLocationBottom );
	axis->SetTitle( L"Time" );


	if( ::IsWindow(m_webBrowser.m_hWnd))
		m_webBrowser.ShowWindow(SW_HIDE);

	bResultRequested = true;
	Invalidate();

}


void CSemiMarkovGUIView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


void CSemiMarkovGUIView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	UpdateTransitionView();	

	CSemiMarkovGUIDoc* pDoc = GetDocument();
	if(pDoc->GetTransitionList()->GetTransitionCount() > 0 )
	{
		CString dotFile = GetDocument()->GenerateGraphVizFile();
		DisplayStateSpaceDiagram(dotFile);
	}

}
