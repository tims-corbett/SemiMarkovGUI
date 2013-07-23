
// SemiMarkovGUIDoc.cpp : implementation of the CSemiMarkovGUIDoc class
//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche16.html
//http://msdn.microsoft.com/en-us/library/bs315d2c(v=vs.80).aspx
//http://www.cis.temple.edu/~wolfgang/MFC-STLPaper.PDF
//http://msdn.microsoft.com/en-us/library/zs9e0dfa.aspx#_core_to_iterate_a_list


#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SemiMarkovGUI.h"
#endif

#include "SemiMarkovGUIDoc.h"


#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_traits.hpp> 


#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSemiMarkovGUIDoc

IMPLEMENT_DYNCREATE(CSemiMarkovGUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CSemiMarkovGUIDoc, CDocument)
END_MESSAGE_MAP()


// CSemiMarkovGUIDoc construction/destruction

CSemiMarkovGUIDoc::CSemiMarkovGUIDoc()
{
	m_TransitionList.SetStateList(&m_StateList);
}

CSemiMarkovGUIDoc::~CSemiMarkovGUIDoc()
{
}

BOOL CSemiMarkovGUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}




// CSemiMarkovGUIDoc serialization
/*
FILE FORMAT
	1. State Count
	3. States
	2. Transition Count
	4. Transitions
*/

void CSemiMarkovGUIDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring() == false)
	{
		m_StateList.ClearCollection();
		m_TransitionList.ClearCollection();
	}


	m_StateList.Serialize(ar);
	m_TransitionList.Serialize(ar);


	if(ar.IsStoring() == false)
		UpdateAllViews(NULL);

}



#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSemiMarkovGUIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSemiMarkovGUIDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSemiMarkovGUIDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSemiMarkovGUIDoc diagnostics

#ifdef _DEBUG
void CSemiMarkovGUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSemiMarkovGUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


bool CSemiMarkovGUIDoc::AddState(CStateInfo* stateInfo)
{
	m_StateList.AddState(stateInfo);
	return true;
}

CStateCollection* CSemiMarkovGUIDoc::GetStateList(void)
{
	return &m_StateList;
}

int CSemiMarkovGUIDoc::GetStateCount(void)
{
	return m_StateList.GetStateCount();
}

CTransitionCollection* CSemiMarkovGUIDoc::GetTransitionList(void)
{
	return &m_TransitionList;
}

CString CSemiMarkovGUIDoc::GenerateGraphVizFile(void)
{
	int iTransitionCount = m_TransitionList.GetTransitionCount();

	if(iTransitionCount == 0)
		return L"";


	std::wofstream fout("newtest.dot");
	  fout	<< "digraph A {\n"
			<< "rankdir=LR\n"
			<< "edge[style=\"ultra thick\"\n"
			<< "node[shape=circle, weight=bold]\n"
			<< "ranksep = 0.75\n"
			<< "nodesep = 0.25\n\n";
	
	  
	unsigned _size = m_StateList.GetStateCount();
	  
	CStateInfo* pStateInfo;
	
	for( unsigned i=0; i < _size; i++)
	{
		pStateInfo = m_StateList.GetState(i);
		fout	<< i 
				<< "[label=\" \" texlbl=\"$\\begin{matrix}\\text{" 
				<< (LPCWSTR)pStateInfo->GetStateName() 
				<< "}\\\\\\text{"
				<< (LPCWSTR)pStateInfo->GetStateShortName() 
				<<	"}\\end{matrix}$"
				<< "\"];\n";
	}


	CTransitionInfo* pTransition;
	for( int i=0; i < iTransitionCount; i++)
	{
		pTransition = m_TransitionList.GetTransition(i);
		
		fout << pTransition->GetFromState()->GetStateNumber() << " -> " 
			 << pTransition->GetToState()->GetStateNumber() << "[label=\" \" texlbl=\"" << (LPCWSTR)pTransition->GetTransitionLabel() << "\"];\n";
	}

	fout << "};\n\n";

	fout.close();

	
	return L"newtest.dot";
}

/*
CString CSemiMarkovGUIDoc::GenerateGraphVizFile(void)
{
	int iTransitionCount = m_TransitionList.GetTransitionCount();

	if(iTransitionCount == 0)
		return L"";

	unsigned _size = m_StateList.GetStateCount();

	typedef std::pair<int,int> Edge;

	std::vector<std::pair<int,int> > used_by;
	int index=0;

	CTransitionInfo* pTransition;
	for( int i=0; i < iTransitionCount; i++)
	{
		pTransition = m_TransitionList.GetTransition(i);
		used_by.push_back(std::make_pair(pTransition->GetFromState()->GetStateNumber(), pTransition->GetToState()->GetStateNumber() ));
	}


  std::vector<std::string> names = m_StateList.GetStateNames();

 
  const int n_edges = used_by.size();
  std::vector<int> weights(n_edges,1);

  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::property< boost::vertex_color_t, boost::default_color_type >,
    boost::property< boost::edge_weight_t, int >
    > Graph;

  Graph g(used_by.begin(), used_by.end(), weights.begin(), n_edges-1); //WORKS

  //Write graph to file
  {
    std::ofstream f("newtest.dot");
    boost::write_graphviz(f, g, boost::make_label_writer(&names[0]));
    f.close();
  }
  
  
  return L"newtest.dot";
}
*/

// CSemiMarkovGUIDoc commands

