
// SemiMarkovGUIDoc.h : interface of the CSemiMarkovGUIDoc class
//


#pragma once

#include<afxtempl.h>

#include "StateInfo.h"
#include "StateCollection.h"
#include "../BoostTest/JaggedMatrix.h"
#include "TransitionCollection.h"

class CSemiMarkovGUIDoc : public CDocument
{
protected: // create from serialization only
	CSemiMarkovGUIDoc();
	DECLARE_DYNCREATE(CSemiMarkovGUIDoc)

	CStateCollection m_StateList;
	CTransitionCollection m_TransitionList;

// Attributes
public:

// Operations
public:
	bool AddState(CStateInfo* stateInfo);
	CStateCollection* GetStateList(void);
	CTransitionCollection* GetTransitionList(void);
	int GetStateCount(void);
	CString GenerateGraphVizFile(void);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSemiMarkovGUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
};
