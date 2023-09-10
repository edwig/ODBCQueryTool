/* 
    Copyright (C) 2002 Aleksey Kochetov

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OEView_h__
#define __OEView_h__

#include <set>
#include <afxole.h>             // for Drag & Drop
#include <common/arg_shared.h>
#include <vector>
#include <algorithm>
#include "OEContext.h"
#include "OEAutocompleteCtrl.h"
#include "OEViewPaintAccessories.h"
#include "Query/GridView.h"
#include "Query/QueryPanelWnd.h"
#include "SQLQuery.h"
#include "SQLInfo.h"

using arg::counted_ptr;
using namespace SQLComponents;
class COEDocument;
namespace OpenEditor { class HighlighterBase; };
enum SearchDirection { esdDown, esdUp, esdDefault };

class QueryHistory
{
public:
  QueryHistory();
 ~QueryHistory();
  int     number;
  int     reruns;
  CString command;
};

typedef std::vector<QueryHistory> HistoryMap;
typedef std::vector<int>          ErrorMap;

typedef struct _PrimKeyInfo
{
  CString  m_colName;
  int      m_colPos;
  int      m_queryPos;
}
PrimKeyInfo;

typedef std::map<int,PrimKeyInfo>   PrimaryMap;

class COEditorView : public CView, protected OpenEditor::EditContext
{
	// Ruler is a struct, which helpes to draw character grid
    struct Ruler
    {
        int m_Topmost,       // topmost visible line or column
            m_Count,         // number of visible lines or columns
            m_CharSize,      // size of character
            m_ClientSize,    // size of client area
            m_Indent;

        COEditorView* m_sibling; // sibling for scrolling

        int  InxToPix (int inx) const;					// screen position to pixel coordinate (inline)
        int  PosToPix (int pos) const;                  // buffer position to pixel coordinate (inline)
        bool OnScroll (UINT code, UINT pos, int last);  // helper function for scrolling service
    }
    m_Rulers[2];
   
    friend COEAutocompleteCtrl;
    static COEAutocompleteCtrl m_autocompleteList; // an embedded control

    static BOOL m_isOverWriteMode;     // overwrite mode for text input
    static UINT m_uWheelScrollLines;   // cached value for MS Weel support
    BOOL m_bAttached;           // it's FALSE till context initialization
    int  m_nMaxLineLength;		// max line length (for horisontal scroller)
    BOOL m_bSyntaxHighlight;
    BOOL m_bMouseSelection;     // it's TRUE after WM_LBUTTONDOWN
    BOOL m_bMouseSelectionStart;// it's TRUE after WM_LBUTTONDOWN before first WM_MOUSEMOVE
    int  m_nMouseSelStartLine;  // it's not -1 if the left border area is used for a mouse selection
    int  m_nDigitsInLatsLineNumber; // the number of digits in the last line number

    BOOL m_bDragScrolling; //  data is dragged over the window and scrolling has started after a small delay

    struct CurCharCache
    {
        OpenEditor::Position pos;
        CString message;

        CurCharCache () { pos.line = pos.column = -1; }
        void Reset ()   { pos.line = pos.column = -1; }
    }
    m_curCharCache;

#if _MFC_VER <= 0x0600
    static const UINT SEL_MOUSE_TIMER;
    static const UINT SEL_MOUSE_TIMER_ELAPSE;
    static const UINT DRAG_MOUSE_TIMER;
    static const UINT DRAG_MOUSE_TIMER_ELAPSE;
#else
    static const UINT SEL_MOUSE_TIMER           = (UINT)-1;
    static const UINT SEL_MOUSE_TIMER_ELAPSE    = 60;
    static const UINT DRAG_MOUSE_TIMER          = (UINT)-2;
    static const UINT DRAG_MOUSE_TIMER_ELAPSE   = 120;
#endif
    static const UINT QUERY_TIMER = 0xF;

    int CalculateDigitsInLatsLineNumber ();
    int CalculateLeftMargin ();
    
    COEditorView* GetSibling (ULONG row, ULONG col);
    Ruler* GetSiblingRuler (ULONG row, ULONG col, UINT ruler);
    void AttachSibling (COEditorView*, UINT ruler);
    void DetachSibling (UINT ruler);
    void AdjustVertSibling ();
    void AdjustHorzSibling ();
    void DoPageDown ();
    void DoPageUp ();

    COEViewPaintAccessoriesPtr m_paintAccessories; // container for shared GDI objects
    counted_ptr<OpenEditor::HighlighterBase> m_highlighter;

    // a settings synchronization handler
    void OnSettingsChanged ();

    // Bookmarks
    void DrawLineNumber (CDC&, RECT, int line);
    void DrawBookmark (CDC&, RECT);
    void DrawRandomBookmark (CDC&, RECT, int index, bool halfclip);

	//
	//  implementations of this methods are stored in OpenQueryView_3.cpp
	//
public:
    void EnableSyntaxHighlight (BOOL);

    using EditContext::GetLine;
    using EditContext::GetLineLength;
    using EditContext::GetLineCount;
    using EditContext::GetLineId;
    using EditContext::FindById;

    using EditContext::GetPosition;
    using EditContext::MoveTo;
    using EditContext::InxToPos;
    using EditContext::PosToInx;

    using EditContext::GetBlockMode;
    using EditContext::GetSelection;
    using EditContext::SetSelection;
    using EditContext::IsSelectionEmpty;
    using EditContext::InsertBlock;
    
    using EditContext::GetBlockOrWordUnderCursor;

    // not implemented yet
    void SetQueueBookmark (int, bool = true);

    // cursor navigation for external usage
    // I changed the method name from Go... to Move... 
    // avoiding any impact on the existing code
    void MoveToBottom ();
    void MoveToAndCenter (OpenEditor::Position);

    virtual void GetBlock    (CString&, const OpenEditor::Square* = 0) const;
    virtual void InsertBlock (const char*, bool hideSelection, bool putSelInUndo = true);
    virtual void DeleteBlock (bool putSelInUndo = true);

    // Searching
    bool RepeatSearch (SearchDirection, COEditorView*&);
    using EditContext::GetSearchText;
    using EditContext::SetSearchText;
    using EditContext::GetSearchOption;
    using EditContext::SetSearchOption;
    using EditContext::IsBackwardSearch;
    using EditContext::SetBackwardSearch;

    // 1st is out-parameter, 2d,3d,4th are in-out
    bool Find (const COEditorView*&, int& line, int& start, int& end, bool thruEof = true) const;

    using EditContext::Replace;
    using EditContext::SearchBatch;

    void ShowCaret (bool show, bool dont_destroy_caret=false);
    bool GetCaretPosition (CPoint&) const;

    class UndoGroup : public OpenEditor::EditContext::UndoGroup
    {
    public:
        UndoGroup (COEditorView&);
    };
    //void PushInUndoStack (Position);
    //void PushInUndoStack (EBlockMode, Square);
    using EditContext::PushInUndoStack;

protected:
    void SetCaretPosition ();
    void AdjustCaretPosition ();

    void AdjustVertScroller ();
    void AdjustHorzScroller ();
    void DoSize (UINT nType, int cx, int cy);

    OpenEditor::Position MouseToPos (CPoint, bool force = false) const;
    void SelectByMouse (CPoint);

    // override base methods
    void InvalidateWindow ();
    void InvalidateLines  (int start, int end);
    void InvalidateSquare (const OpenEditor::Square&);

    void OnChangedLines (int startLine, int endLine);
    void OnChangedLinesQuantity (int line, int quantity);
    void OnChangedLinesStatus (int, int);

public:
    static void DoEditCopy (const CString&, bool append = false);

protected:

    //
    //  Drag & Drop implementation (see OpenQueryView_3.cpp)
	//
    struct CDragAndDropData
    {
        bool   bVisible;
        CPoint ptCaret, ptCursor;
        CSize  sCursor;
        CView* pSrcWnd, *pDstWnd;
        CDragAndDropData () { memset(this, 0, sizeof (CDragAndDropData)); }
    };

    static int cnScrollThreshold;
    static CDragAndDropData m_DragAndDropData;
    COleDropTarget m_DropTaget;

    void DrawCursor (CPoint, bool erase = false);
    bool CursorOnSelection (CPoint) const;
    void StartDrag ();
    void PositionToPoint (OpenEditor::Position, CPoint&) const;

    virtual DROPEFFECT OnDragEnter (COleDataObject*, DWORD dwKeyState, CPoint);
    virtual DROPEFFECT OnDragOver  (COleDataObject*, DWORD dwKeyState, CPoint);
    virtual BOOL OnDrop            (COleDataObject*, DROPEFFECT, CPoint);
    virtual void OnDragLeave       ();

    // comment/uncomment helper functions
    void  DoCommentText (bool comment); // uncomment on false
    static bool CommentText (const OpenEditor::EditContext&, CString&);
    static bool UncommentText (const OpenEditor::EditContext&, CString&);

    // printing helper
    void GetMargins (CRect& rc) const;
	  void MoveToPage (CPrintInfo* pInfo, int* pMaxPage = NULL);
	  void PrintHeader (CDC* pDC, CPrintInfo* pInfo);
	  void PrintFooter (CDC* pDC, CPrintInfo* pInfo);

    // keyword normalization helpers
    struct NormalizeOnCharCxt 
    {
        bool matched;
        CString keyword; 
        OpenEditor::Square sqr; 
    };
    bool PreNormalizeOnChar (NormalizeOnCharCxt&, char ch);
    void NormalizeOnChar    (NormalizeOnCharCxt&);
    // it's not comment and string position
    bool IsNormalizeablePos (OpenEditor::Position pos) const;
    static bool NormalizeText (const OpenEditor::EditContext&, CString&);

protected: // create from serialization only
	COEditorView();
	DECLARE_DYNCREATE(COEditorView)

// Attributes
public:
	  COEDocument* GetDocument();

	  afx_msg void OnUpdate_Mode      (CCmdUI* pCmdUI);
	  afx_msg void OnUpdate_Pos       (CCmdUI* pCmdUI);
	  afx_msg void OnUpdate_ScrollPos (CCmdUI* pCmdUI);
    afx_msg void OnUpdate_BlockType (CCmdUI* pCmdUI);
    afx_msg void OnUpdate_CurChar   (CCmdUI* pCmdUI);

public:
	//{{AFX_VIRTUAL(COEditorView)
	public:
	  virtual void    OnInitialUpdate();
    virtual void    OnDraw(CDC*) {};  // overridden to draw this view
	  virtual BOOL    PreCreateWindow(CREATESTRUCT& cs);
	  virtual BOOL    OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	  virtual void    OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  protected:
    virtual void    OnFilePrintPreview();
	  virtual BOOL    OnPreparePrinting(CPrintInfo* pInfo) override;
	  virtual void    OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	  virtual void    OnEndPrinting  (CDC* pDC, CPrintInfo* pInfo) override;
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	  virtual void    OnPrint(CDC* pDC, CPrintInfo* pInfo);
	  //}}AFX_VIRTUAL

public:
	virtual ~COEditorView();

protected:
	//{{AFX_MSG(COEditorView)
	afx_msg void OnPaint(); // moved to OpenQueryView_2.cpp
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditToggleColumnarSelection();
	afx_msg void OnEditToggleStreamSelection();
  afx_msg void OnToggleStreamSelection();
	afx_msg void OnUpdate_SelectionType(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdate_EditCopyAndCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdate_EditPaste(CCmdUI* pCmdUI);
	afx_msg void OnTest();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdate_EditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdate_EditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditDeleteLine();
	afx_msg void OnEditDeleteWordToLeft();
	afx_msg void OnEditDeleteWordToRight();
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnEditFindNext();
	afx_msg void OnEditFindPrevious();
	afx_msg void OnEditBookmarkToggle();
	afx_msg void OnEditBookmarkNext();
	afx_msg void OnEditBookmarkPrev();
	afx_msg void OnEditBookmarkRemoveAll();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdate_EditFindNext(CCmdUI* pCmdUI);
	afx_msg void OnEditLower();
	afx_msg void OnUpdate_SelectionOperation(CCmdUI* pCmdUI);
	afx_msg void OnEditUpper();
	afx_msg void OnBlockTabify();
	afx_msg void OnBlockUntabify();
  afx_msg void OnBlockTabifyLeading();
  afx_msg void OnEditExpandTemplate();
	afx_msg void OnEditIndent();
	afx_msg void OnEditUndent();
	afx_msg void OnEditFindMatch();
	afx_msg void OnEditFindMatchAndSelect();
	afx_msg void OnEditComment();
	afx_msg void OnEditUncomment();
	afx_msg void OnEditGoto();
	//}}AFX_MSG
  afx_msg void OnSetRandomBookmark       (UINT nBookmark);
  afx_msg void OnGetRandomBookmark       (UINT nBookmark);
  afx_msg void OnUpdate_GetRandomBookmark (CCmdUI* pCmdUI);
  afx_msg void OnUpdate_BookmarkGroup     (CCmdUI* pCmdUI);
  afx_msg void OnEditScrollUp ();
  afx_msg void OnEditScrollDown ();
  afx_msg void OnEditScrollCenter ();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEditCutAndAppend();
  afx_msg void OnEditCutBookmarked();
  afx_msg void OnEditCopyAndAppend();
  afx_msg void OnEditCopyBookmarked();
  afx_msg void OnEditDelete();
  afx_msg void OnEditDeleteBookmarked();
  afx_msg void OnEditSelectWord();
  afx_msg void OnEditSelectLine();
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
  afx_msg void OnEditCapitalize();
  afx_msg void OnEditInvertCase();
  // QUERY TOOL
  afx_msg void OnScriptExecute();
  afx_msg void OnScriptNative();
  afx_msg void OnScriptCurrent();
  afx_msg void OnScriptExecuteStep();
  afx_msg void OnScriptFindTable();
  afx_msg void OnScriptTableDDL();
  afx_msg void OnScriptVariables();
  afx_msg void OnScriptNextError();
  afx_msg void OnScriptPrevError();
  afx_msg void OnUpdateCommandScriptNative(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCommandScriptExecute(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCommandScriptCurrent(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCommandScriptExecuteStep(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCommandScriptNextError(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCommandScriptPrevError(CCmdUI* pCmdUI);

  void RegisterGrid      (CWnd *grid);
  void RegisterHistory   (CWnd *histo);
  void RegisterOutput    (CWnd* output);
  void RegisterStatistics(CWnd* stats);
  void RegisterQueryPanel(CQueryPanelWnd* panel);
  int  GetODBCCommand(int& curLine
                     ,int& endFileLine
                     ,CString& odbcCommand);
  bool ExecuteQuery(int p_line
                   ,CString& odbcCommand
                   ,bool batch = false
                   ,FILE* p_script = NULL);
  bool ExecuteQueryRepeat(int p_line
                         ,CString& odbcCommand
                         ,bool batch = false);
  void GetLineFromQuery(int row);
  void ReadRestOfQuery();
  int  LinesFetchedSoFar();
  bool IsDigits(char* token);
  void TranslateText(CString& p_text,int p_translation,CString p_charset);

  // Write a line with info to the statistics view
  void    WriteStatisticsLine(CString p_step
                             ,CString p_line
                             ,bool p_reset = false
                             ,long p_totalticks = 0);
  void    WriteOutputLine(int p_line
                         ,int p_affected
                         ,CString p_command
                         ,CString p_result);
  void    WriteHistoryLine(CString p_command);
  bool    CanEdit();
  bool    UpdateTable(int row,int col,CString text);
  CString UpdateCondition(int row,CString column);
  bool    ScriptCommand         (int p_line,CString &odbcCommand);
  bool    ScriptCommandPrint    (int p_line,CString print);
  bool    ScriptCommandFile     (int p_line,CString file);
  bool    ScriptCommandRebind   (int p_line,CString rebind);
  bool    ScriptCommandIf       (int p_line,CString command);
  bool    ScriptCommandVariable (int p_line,int p_varNum,CString p_tail);
  bool    ScriptCommandSelect   (int p_line,CString p_tail);
  bool    ScriptCommandAtExec   (int p_line,CString p_tail);
  bool    ScriptCommandRepeat   (int p_line,CString p_tail);
  bool    ScriptCommandExit     (int p_line,CString p_tail);
  bool    ScriptCommandEndRepeat(int p_line,CString p_tail);
  int     ScriptSelect(int p_line);
  void    ScriptSelect(int p_line,int p_longest,int p_row);
  long    MilisecondsToMinute();

  CGridView*      m_gridView;
  CGridView*      m_histoView;
  CGridView*      m_outptView;
  CGridView*      m_statsView;
  CQueryPanelWnd* m_queryPanel;
  HistoryMap      m_historyMap;
  ErrorMap        m_errorMap;
  int             m_currentError;
  int             m_linesFetched;   // aantal regels opgehaald
  CString         m_tableOne;
  bool            m_findPrimary;
  SQLQuery        m_query;
  PrimaryMap      m_keyMap;
  CString         m_primaryName;
  FILE*           m_scriptOutput;
  bool            m_scriptCompare;
  bool            m_scriptSelect;
  bool            m_ifLast;
  int             m_inboundBufferSize;
  int             m_interval;
  int             m_repeat;
  bool            m_wholeMinutes;
  // END QUERY TOOL
protected:
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
    afx_msg void OnEditNormalizeText();
};

inline
COEDocument* COEditorView::GetDocument()
    { return (COEDocument*)m_pDocument; }

inline
void COEditorView::InvalidateWindow ()
    { Invalidate(FALSE); }

inline
int COEditorView::Ruler::InxToPix (int inx) const
    { return inx * m_CharSize + m_Indent; }

inline
int COEditorView::Ruler::PosToPix (int pos) const
    { return (pos - m_Topmost) * m_CharSize + m_Indent; }

inline
COEditorView::
UndoGroup::UndoGroup (COEditorView& view)
: OpenEditor::EditContext::UndoGroup(view)
{
}

inline int
COEditorView::LinesFetchedSoFar()
{
  return m_linesFetched;
}

//{{AFX_INSERT_LOCATION}}

#endif//__OEView_h__
