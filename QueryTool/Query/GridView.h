// GridView.h : interface of the CGridView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GridView_H)
#define AFX_GridView_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MGridCtrl.h"
#include "OEDocument.h"

#define TYPE_QUERY    0
#define TYPE_STATS    1
#define TYPE_OUTPT    2
#define TYPE_HISTO    3

struct _initialGridCol
{
  int   size;
  bool  sortNum;
  char* name;
};

typedef struct _initialGridCol InitalGridCol;

struct _gridType
{
  int           cols;
  InitalGridCol inital[5];
};

typedef _gridType GridType;


class CGridView : public CView
{
//protected: // create from serialization only
public:
	CGridView();
  virtual void  PostNcDestroy();

	//DECLARE_DYNCREATE(CGridView)

// Attributes
public:
	COEDocument* GetDocument();
	MCGridCtrl*  m_pGridCtrl;
  void    RegisterEditor(CWnd *editor,int p_type = 0);
  void    InitGridEmpty(int p_type = 0,bool nofirst = false);
  void    InsertColumn(LPCTSTR p_heading,UINT p_format,int p_column);
  void    InsertRow   (LPCTSTR p_heading,int p_row);
  void    InsertItem(int p_row,int p_col,CString& p_text,UINT p_format = 0,int p_sqlType = 0);
  void    InsertImage(int p_row,int p_col,int p_image);
  CString GetItemText(int p_row,int p_col);
  void    SetItemText(int p_row,int p_col,CString p_text);
  int     GetColumnCount();
  int     GetPrefetchLines();
  CString GetQueryTerminator();
  void    DeleteColumn(int p_column);
  int     AppendRow(LPCTSTR p_heading);
  void    SetRowCount(int p_rows);
  void    SetAutoExpand(bool p_expand);
  void    SetPrefetchLines(int p_lines);
  void    SetQueryTerminator(CString queryTerminator);
  void    SizeColumns(int p_style);
  void    GotoQueryBegin();
  void    GotoQueryPrevPage();
  void    GotoQueryPrev();
  void    GotoQueryNext();
  void    GotoQueryNextPage();
  void    GotoQueryEnd();
  bool    AtBegin();
  bool    AtEnd();
  bool    IsFilled();
  bool    ReadRestOfQuery();
  void    ExportToExcel();
  void    ExportToExcelXLS();
  void    ExportToExcelSLK();
  void    ExportToHTML();
  void    ExportToTXT();

  void    AutoSize();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  afx_msg void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBeginEdit     (NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
  void  SetInitialFont();

	//{{AFX_MSG(CGridView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToggleReadonly();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  COEditorView* m_editorView;     // COnnected editor view
  int           m_type;           // type of view
  bool          m_autoExpand;     // Auto expand last column
  CImageList    m_imageList;      //
  int           m_prefetchLines;  // Number of prefetched lines
  CString       m_queryTerminator;// Terminator of SQL text
  CString       m_saveEdit;       // Placeholder for restore of primary key
};

inline void 
CGridView::SetAutoExpand(bool p_expand)
{
  m_autoExpand = p_expand;
}

inline void
CGridView::SetPrefetchLines(int p_lines)
{
  m_prefetchLines = p_lines;
}

inline void
CGridView::SetQueryTerminator(CString queryTerminator)
{
  m_queryTerminator = queryTerminator;
}

inline int
CGridView::GetPrefetchLines()
{
  return m_prefetchLines;
}

inline CString
CGridView::GetQueryTerminator()
{
  return m_queryTerminator;
}

#endif 
