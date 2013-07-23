
#include "stdafx.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree



#include "StateInfo.h"
#include "TransitionInfo.h"
#include "SemiMarkovGUIView.h"

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, &CViewTree::OnNMClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}



void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSemiMarkovGUIView* pView = GetView();
	HTREEITEM hTreeItem = GetSelectedItem();
	HTREEITEM hParent = GetParentItem(hTreeItem);

	if(NULL == hTreeItem || hParent == NULL)
		return;

	CString parentLabel = GetItemText(hParent);

	DWORD_PTR dpInfo = GetItemData(hTreeItem);

	if(dpInfo == NULL)
		return;
	
	if( parentLabel == L"States")		//User clicked on one of the states
	{
		CStateInfo* pStateInfo = (CStateInfo*)dpInfo;
		pView->ModifyState(pStateInfo);
		return;
	}


	if( parentLabel == L"Transitions" )		//User clicked a transition and not a distribution
			return;

	if( parentLabel == L"State Probabilities")
	{
		CStateInfo* pStateInfo = (CStateInfo*)dpInfo;
		pView->PopulateResults(pStateInfo);
		return;
	}
	
	//A distribution has been double clicked
	CDistributionInfo* pDistributionInfo = (CDistributionInfo*)dpInfo;
	
	if(pDistributionInfo == NULL)
		return;
	
	CTransitionInfo* pTransitionInfo = (CTransitionInfo*)GetItemData(hParent);

	if(pTransitionInfo != NULL)
		pView->ModifyTransition(pTransitionInfo, pDistributionInfo);
		 
	*pResult = 0;
}




// View implementation file
CSemiMarkovGUIView* CViewTree::GetView()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

	if( pFrame == NULL)
		return NULL;

    CView * pView = pFrame->GetActiveView();

    if ( !pView )
        return NULL;

    // Fail if view is of wrong kind
    // (this could occur with splitter windows, or additional
    // views on a single document
    if ( ! pView->IsKindOf( RUNTIME_CLASS(CSemiMarkovGUIView) ) )
        return NULL;

    return (CSemiMarkovGUIView*) pView;
}

void CViewTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{


	*pResult = 0;
}


void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CSemiMarkovGUIView* pView = GetView();

	if( pView == NULL)
		return;

	HTREEITEM hTreeItem = GetSelectedItem();
	HTREEITEM hParent = GetParentItem(hTreeItem);

	if(NULL == hTreeItem || hParent == NULL)
		return;

	CString parentLabel = GetItemText(hParent);

	DWORD_PTR dpInfo = GetItemData(hTreeItem);

	if(dpInfo == NULL)
		return;
	
	if( parentLabel == L"States")		//User clicked on one of the states
	{
		CStateInfo* pStateInfo = (CStateInfo*)dpInfo;
		pView->UpdateStatePropertyPage(pStateInfo);
		return;
	}


	if( parentLabel == L"Transitions" )		//User clicked a transition and not a distribution
	{
		CTransitionInfo* pTransitionInfo = (CTransitionInfo*)dpInfo;

		if(pTransitionInfo != NULL)
			pView->UpdateTransitionPropertyPage(pTransitionInfo);
		
		return;
	}

	if( parentLabel == L"State Probabilities")
	{
		CStateInfo* pStateInfo = (CStateInfo*)dpInfo;
		return;
	}
	
	//A distribution has been double clicked
	CDistributionInfo* pDistributionInfo = (CDistributionInfo*)dpInfo;
	
	if(pDistributionInfo == NULL)
		return;
	
	CTransitionInfo* pTransitionInfo = (CTransitionInfo*)GetItemData(hParent);

	if(pTransitionInfo != NULL)
		pView->UpdateDistributionPropertyPage(pTransitionInfo, pDistributionInfo);


	*pResult = 0;
}
