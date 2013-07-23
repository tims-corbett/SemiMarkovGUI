
// SemiMarkovGUIView.h : interface of the CSemiMarkovGUIView class
//

#pragma once

#include "CWebBrowser2.h"

#include "Redir.h"
#include "CPlot Sources/Chart.h"

class CSemiMarkovGUIDoc;
class COutputList;
class SemiMarkovModel;
class CTransitionInfo;
class CDistributionInfo;
class CStateInfo;

class CSemiMarkovGUIView : public CView
{
private:
	CWebBrowser2 m_webBrowser;

protected: // create from serialization only
	CSemiMarkovGUIView();
	DECLARE_DYNCREATE(CSemiMarkovGUIView)
	COutputList* m_pOutputWindow;
	CRedirector redirector;
	bool bResultRequested;

// Attributes
public:
	CSemiMarkovGUIDoc* GetDocument() const;
	void PopulateResults(CStateInfo* pInfo);

	CXYChart m_Chart;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void UpdateTransitionView(void);
	void DisplayStateSpaceDiagram(CString dotFile);

	void DisplayTimeStep(SemiMarkovModel* pModel);	//Callback from CSemiMarkovModel

	void ModifyState(CStateInfo* pStateInfo);
	void ModifyTransition(CTransitionInfo* pTransitionInfo, CDistributionInfo* pDistributionInfo);
	void UpdateDistributionPropertyPage(CTransitionInfo* pTransitionInfo, CDistributionInfo* pDistributionInfo);
	void UpdateTransitionPropertyPage(CTransitionInfo* pTransitionInfo);
	void UpdateStatePropertyPage(CStateInfo* pStateInfo);


protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	
	void WaitForProcessing(void);
	DWORD WINAPI SemiMarkovWorkerThread(PVOID param);

// Implementation
public:
	virtual ~CSemiMarkovGUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool bNavigationComplete;
// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnModelAddnode();
	afx_msg void OnModelAddtransition();
	afx_msg void OnDocumentComplete(LPDISPATCH lpDisp,VARIANT FAR* URL);

	DECLARE_EVENTSINK_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnModelRunmodel();
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in SemiMarkovGUIView.cpp
inline CSemiMarkovGUIDoc* CSemiMarkovGUIView::GetDocument() const
   { return reinterpret_cast<CSemiMarkovGUIDoc*>(m_pDocument); }
#endif

