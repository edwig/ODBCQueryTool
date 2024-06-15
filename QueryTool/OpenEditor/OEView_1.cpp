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

/*
    13.05.2002 bug fix, processing for control chars has been added (esc,...)
    10.07.2002 bug fix, double/triple click on spaces does not select line
    08.09.2002 bug fix, the searched text has been found but may be still invisible
    08.09.2002 improvement, new copy commands
    28.09.2002 improvement, splitter panes/views synchronization
    28.09.2002 bug fix, scrolling on mouse wheel support is twice faster then defined
    19.11.2002 improvement, smart vertical scrolling for goto and bookmark jumps
    08.01.2003 improvement, dropdown list control for auto complete/templates
    16.03.2003 improvement, triple click for line selection has been discontinued
    16.03.2003 bug fix, mouse selection fails sometimes on left button click with pressed <shift>
    17.03.2003 bug fix, cursor position become wrong after document type was changed
    22.03.2003 bug fix, mouse click on gutter does not select line (since 16.03.2003)
    23.03.2003 improvement, added a new mouse word selection on left button click with pressed ctrl
    24.03.2003 improvement, MouseSelectionDelimiters has been added (hidden) which influences on double click selection behavior
    31.03.2003 bug fix, editor context menu position is wrong on Shift+F10
    29.06.2003 improvement, status line indicator for character under cursor 
*/

#include "pch.h"
#include <fstream>
#include <COMMON/AppGlobal.h>
#include <COMMON/ExceptionHelper.h>
#include "COMMON/GUICommandDictionary.h"
#include "OEDocument.h"
#include "OEView.h"
#include "OEFindReplaceDlg.h"
#include "OEGoToDialog.h"
#include "OEHighlighter.h" // for a destructor
#include <WinFile.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OpenEditor;

#if _MFC_VER <= 0x0600
    const UINT COEditorView::SEL_MOUSE_TIMER           = (UINT)-1;
    const UINT COEditorView::SEL_MOUSE_TIMER_ELAPSE    = 60;
    const UINT COEditorView::DRAG_MOUSE_TIMER          = (UINT)-2;
    const UINT COEditorView::DRAG_MOUSE_TIMER_ELAPSE   = 120;
#endif


static const TCHAR g_szClassName[] = _T("OpenEditorWindow");
static HCURSOR g_hCurIBeam = ::LoadCursor(NULL, IDC_IBEAM);
static HCURSOR g_hCurArrow = ::LoadCursor(NULL, IDC_ARROW);
BOOL COEditorView::m_isOverWriteMode = FALSE;
UINT COEditorView::m_uWheelScrollLines = 0;
COEAutocompleteCtrl COEditorView::m_autocompleteList;

int g_cyPixelsPerInch = CWindowDC(CWnd::GetDesktopWindow()).GetDeviceCaps(LOGPIXELSY);
int PixelToPoint (int pixels) { return MulDiv(pixels, 72, g_cyPixelsPerInch); }
int PointToPixel (int points) { return MulDiv(points, g_cyPixelsPerInch, 72); }

/////////////////////////////////////////////////////////////////////////////
// COEditorView

IMPLEMENT_DYNCREATE(COEditorView, CView)

BEGIN_MESSAGE_MAP(COEditorView, CView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
  ON_WM_CONTEXTMENU()
  ON_WM_INITMENUPOPUP()
  ON_WM_CHAR()
  ON_WM_MOUSEWHEEL()
  ON_WM_SETTINGCHANGE()
  ON_COMMAND          (ID_EDIT_COLUMN_SEL,  OnEditToggleColumnarSelection)
	ON_COMMAND          (ID_EDIT_STREAM_SEL,  OnEditToggleStreamSelection)
  ON_COMMAND          (ID_EDIT_TOGGLES_SEL, OnToggleStreamSelection)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COLUMN_SEL,  OnUpdate_SelectionType)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STREAM_SEL,  OnUpdate_SelectionType)
	ON_COMMAND          (ID_EDIT_COPY,        OnEditCopy)
	ON_COMMAND          (ID_EDIT_CUT,         OnEditCut)
	ON_COMMAND          (ID_EDIT_PASTE,       OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,        OnUpdate_EditCopyAndCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE,       OnUpdate_EditPaste)
	ON_COMMAND          (ID_TEST,                     OnTest)
	ON_COMMAND          (ID_EDIT_UNDO,                OnEditUndo)
	ON_COMMAND          (ID_EDIT_REDO,                OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO,                OnUpdate_EditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO,                OnUpdate_EditRedo)
	ON_COMMAND          (ID_EDIT_SELECT_ALL,          OnEditSelectAll)
	ON_COMMAND          (ID_EDIT_DELETE_LINE,         OnEditDeleteLine)
	ON_COMMAND          (ID_EDIT_DELETE_WORD_TO_LEFT, OnEditDeleteWordToLeft)
	ON_COMMAND          (ID_EDIT_DELETE_WORD_TO_RIGHT,OnEditDeleteWordToRight)
	ON_COMMAND          (ID_EDIT_FIND,                OnEditFind)
	ON_COMMAND          (ID_EDIT_REPLACE,             OnEditReplace)
	ON_COMMAND          (ID_EDIT_FIND_NEXT,           OnEditFindNext)
	ON_COMMAND          (ID_EDIT_FIND_PREVIOUS,       OnEditFindPrevious)
	ON_COMMAND          (ID_EDIT_BKM_TOGGLE,          OnEditBookmarkToggle)
	ON_COMMAND          (ID_EDIT_BKM_NEXT,            OnEditBookmarkNext)
	ON_COMMAND          (ID_EDIT_BKM_PREV,            OnEditBookmarkPrev)
	ON_COMMAND          (ID_EDIT_BKM_REMOVE_ALL,      OnEditBookmarkRemoveAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_NEXT,           OnUpdate_EditFindNext)
	ON_COMMAND          (ID_EDIT_LOWER,               OnEditLower)
	ON_COMMAND          (ID_EDIT_UPPER,               OnEditUpper)
  ON_COMMAND          (ID_EDIT_CAPITALIZE,          OnEditCapitalize)
  ON_COMMAND          (ID_EDIT_INVERT_CASE,         OnEditInvertCase)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TABIFY,              OnUpdate_SelectionOperation)
  ON_UPDATE_COMMAND_UI(ID_EDIT_TABIFY_LEADING,      OnUpdate_SelectionOperation)
	ON_COMMAND          (ID_EDIT_TABIFY,              OnBlockTabify)
  ON_COMMAND          (ID_EDIT_TABIFY_LEADING,      OnBlockTabifyLeading)
	ON_COMMAND          (ID_EDIT_UNTABIFY,            OnBlockUntabify)
	ON_COMMAND          (ID_EDIT_INDENT,              OnEditIndent)
	ON_COMMAND          (ID_EDIT_UNDENT,              OnEditUndent)
	ON_COMMAND          (ID_EDIT_FIND_MATCH,          OnEditFindMatch)
	ON_COMMAND          (ID_EDIT_FIND_MATCH_N_SELECT, OnEditFindMatchAndSelect)
	ON_COMMAND          (ID_EDIT_COMMENT,             OnEditComment)
	ON_COMMAND          (ID_EDIT_UNCOMMENT,           OnEditUncomment)
	ON_COMMAND          (ID_EDIT_GOTO,                OnEditGoto)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT,             OnUpdate_EditCopyAndCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_PREVIOUS,   OnUpdate_EditFindNext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNTABIFY,        OnUpdate_SelectionOperation)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDENT,          OnUpdate_SelectionOperation)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDENT,          OnUpdate_SelectionOperation)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS,        OnUpdate_Pos)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCROLL_POS, OnUpdate_ScrollPos)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR,        OnUpdate_Mode)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_BLOCK_TYPE, OnUpdate_BlockType)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_CUR_CHAR,   OnUpdate_CurChar)
  // Random bookmark commands
  ON_COMMAND_RANGE(ID_EDIT_BKM_SET_0, ID_EDIT_BKM_SET_9, OnSetRandomBookmark)
  ON_COMMAND_RANGE(ID_EDIT_BKM_GET_0, ID_EDIT_BKM_GET_9, OnGetRandomBookmark)
  /*
  it is commented because it does not work dynamically and commands are not accessible
  ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_BKM_GET_0, ID_EDIT_BKM_GET_9, OnUpdate_GetRandomBookmark)
  */
  ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_BKM_NEXT, ID_EDIT_BKM_REMOVE_ALL, OnUpdate_BookmarkGroup)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT,          CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT,   CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,  OnFilePrintPreview)
  ON_COMMAND(ID_EDIT_SCROLL_UP,      OnEditScrollUp    )
  ON_COMMAND(ID_EDIT_SCROLL_DOWN,    OnEditScrollDown  )
  ON_COMMAND(ID_EDIT_SCROLL_CENTER,  OnEditScrollCenter)
  ON_COMMAND(ID_EDIT_EXPAND_TEMPLATE,OnEditExpandTemplate)
  
  ON_COMMAND          (ID_EDIT_CUT_N_APPEND,      OnEditCutAndAppend)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT_N_APPEND,      OnUpdate_SelectionOperation)
  ON_COMMAND          (ID_EDIT_CUT_BOOKMARKED,    OnEditCutBookmarked)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CUT_BOOKMARKED,    OnUpdate_BookmarkGroup)
  ON_COMMAND          (ID_EDIT_COPY_N_APPEND,     OnEditCopyAndAppend)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_N_APPEND,     OnUpdate_SelectionOperation)
  ON_COMMAND          (ID_EDIT_COPY_BOOKMARKED,   OnEditCopyBookmarked)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_BOOKMARKED,   OnUpdate_BookmarkGroup)
  ON_COMMAND          (ID_EDIT_DELETE_BOOKMARKED, OnEditDeleteBookmarked)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE_BOOKMARKED, OnUpdate_BookmarkGroup)
  ON_COMMAND          (ID_EDIT_SELECT_WORD,       OnEditSelectWord)
  ON_COMMAND          (ID_EDIT_SELECT_LINE,       OnEditSelectLine)
  ON_COMMAND          (ID_EDIT_DELETE,            OnEditDelete)
  ON_COMMAND          (ID_EDIT_NORMALIZE_TEXT,    OnEditNormalizeText)
  // QUERY TOOL
  ON_COMMAND          (ID_SCRIPT_EXECUTE,    OnScriptExecute)
  ON_UPDATE_COMMAND_UI(ID_SCRIPT_EXECUTE,    OnUpdateCommandScriptExecute)
  ON_COMMAND          (ID_SCRIPT_CURRENT,    OnScriptCurrent)
  ON_UPDATE_COMMAND_UI(ID_SCRIPT_CURRENT,    OnUpdateCommandScriptCurrent)
  ON_COMMAND          (ID_SCRIPT_NATIVE,     OnScriptNative)
  ON_UPDATE_COMMAND_UI(ID_SCRIPT_NATIVE,     OnUpdateCommandScriptNative)
  ON_COMMAND          (ID_SCRIPT_EXECUTESTEP,OnScriptExecuteStep)
  ON_UPDATE_COMMAND_UI(ID_SCRIPT_EXECUTESTEP,OnUpdateCommandScriptExecuteStep)
  ON_COMMAND          (ID_SCRIPT_FINDTABLE,  OnScriptFindTable)
  ON_COMMAND          (ID_SCRIPT_TABLEDDL,   OnScriptTableDDL)
  ON_COMMAND          (ID_SCRIPT_VARIABLES,  OnScriptVariables)
  ON_COMMAND          (ID_SCRIPT_NEXTERROR,  OnScriptNextError)
  ON_UPDATE_COMMAND_UI(ID_SCRIPT_NEXTERROR,  OnUpdateCommandScriptNextError)
  ON_COMMAND          (ID_SCRIPT_PREVERROR,  OnScriptPrevError)
  ON_UPDATE_COMMAND_UI(ID_SCRIPT_PREVERROR,  OnUpdateCommandScriptPrevError)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEditorView construction/destruction

COEditorView::COEditorView ()
{
    memset(m_Rulers, 0, sizeof(m_Rulers));
    m_bAttached               =
    m_bMouseSelection         =
    m_bMouseSelectionStart    = FALSE;
    m_nMouseSelStartLine      = -1;
    m_bSyntaxHighlight        = TRUE;
    m_nDigitsInLatsLineNumber = 1;
    m_nMaxLineLength          = 0;
    m_bDragScrolling          = FALSE;
    m_currentError            = 0;
    m_scriptOutput            = NULL;
    m_scriptCompare           = true;
    m_scriptSelect            = false;
    m_ifLast                  = false;
    m_inboundBufferSize       = 0;
    m_interval                = 0;
    m_repeat                  = 0;
    m_wholeMinutes            = false;
}

COEditorView::~COEditorView ()
{
  m_query.Close(false);
  while(m_historyMap.size())
  {
    m_historyMap.pop_back();
  }
  m_historyMap.clear();
  for (int i(0); i < 2; i++)
  {
    DetachSibling(i);
  }
  if(m_scriptOutput)
  {
    if(m_scriptOutput->GetIsOpen())
    {
      m_scriptOutput->Close();
    }
    delete m_scriptOutput;
    m_scriptOutput = nullptr;
  }
}

void COEditorView::AttachSibling (COEditorView* sibling, UINT ruler)
{
    DetachSibling(ruler);
    sibling->DetachSibling(ruler);
    m_Rulers[ruler].m_sibling = sibling;
    sibling->m_Rulers[ruler].m_sibling = this;
}

void COEditorView::DetachSibling (UINT ruler)
{
    if (m_Rulers[ruler].m_sibling) 
    {
        _ASSERTE(m_Rulers[ruler].m_sibling->m_Rulers[ruler].m_sibling == this);
        m_Rulers[ruler].m_sibling->m_Rulers[ruler].m_sibling = 0;
        m_Rulers[ruler].m_sibling = 0;
    }
}

int COEditorView::CalculateDigitsInLatsLineNumber ()
{
    int digits;
    int lines;
    for (digits = 1, lines = GetLineCount(); lines; lines /= 10, digits++)
        ;
    return digits;
}

int COEditorView::CalculateLeftMargin ()
{
    // 17.03.2003 bug fix, cursor position become wrong after document type was changed
    int digits = CalculateDigitsInLatsLineNumber();

    return (GetSettings().GetLineNumbers() && digits > 1) ? m_Rulers[0].m_CharSize * (digits + 1) : m_Rulers[0].m_CharSize * 25 / 10;
}

    static 
    ULONG getSplitterPaneID (ULONG row, ULONG col)
    {
        return AFX_IDW_PANE_FIRST + row * 16 + col;
    }
    static 
    void getSplitterRowCol (ULONG id, ULONG& row, ULONG& col)
    {
        id -= AFX_IDW_PANE_FIRST;
        col = 0xf & id;
        row = id >> 4;
    }
    
COEditorView* COEditorView::GetSibling (ULONG row, ULONG col)
{
    CWnd* parent = GetParent();
    if (parent)
    {
        CWnd* sibling = parent->GetDlgItem(getSplitterPaneID(row, col));
        if (sibling && sibling->IsKindOf(RUNTIME_CLASS(COEditorView)))
            return static_cast<COEditorView*>(sibling);
    }
    _ASSERTE(0);
    return 0;
}

COEditorView::Ruler* COEditorView::GetSiblingRuler (ULONG row, ULONG col, UINT ruler)
{
    _ASSERTE(ruler < 2);
    if (ruler < 2)
    {
        CWnd* parent = GetParent();
        if (parent)
        {
            COEditorView* sibling = GetSibling(row, col);
            if (sibling)
                return sibling->m_Rulers + ruler;
        }
    }
    _ASSERTE(0);
    return 0;
}

void COEditorView::OnInitialUpdate ()
{
    SetStorage(&GetDocument()->GetStorage());
    m_bAttached = TRUE;
    m_nDigitsInLatsLineNumber = CalculateDigitsInLatsLineNumber();

    OnSettingsChanged();

    // on splitting
    ULONG id = ::GetWindowLong(m_hWnd, GWL_ID);
    
    _ASSERTE(getSplitterPaneID(0,0) <= id && id <= getSplitterPaneID(1,1));
    
    if (getSplitterPaneID(0,0) < id && id <= getSplitterPaneID(1,1))
    {
        ULONG row, col;
        getSplitterRowCol(id, row, col);
        
        // initialize initial scrolling and cursor positions
        if (row == 1 && col == 0 || row == 0 && col == 1) // 2 views
        {
            Ruler* siblingRuler;
            siblingRuler = GetSiblingRuler(0, 0, 1);
            if (siblingRuler) m_Rulers[1].m_Topmost = siblingRuler->m_Topmost;
            siblingRuler = GetSiblingRuler(0, 0, 0);
            if (siblingRuler) m_Rulers[0].m_Topmost = siblingRuler->m_Topmost;
            
            // link siblings for synchronized scrolling
            COEditorView* sibling;
            if (col) // for vertical scrolling
            { 
                sibling = GetSibling(0, row);
                if (sibling) AttachSibling(sibling, 1);
            }
            if (row) // for horizontal scrolling
            {
                sibling = GetSibling(col, 0);
                if (sibling) AttachSibling(sibling, 0);
            }
        }
        else // 4 views
        {
            Ruler* siblingRuler;
            siblingRuler = GetSiblingRuler(1, 0, 1);
            if (siblingRuler) m_Rulers[1].m_Topmost = siblingRuler->m_Topmost;
            siblingRuler = GetSiblingRuler(0, 1, 0);
            if (siblingRuler) m_Rulers[0].m_Topmost = siblingRuler->m_Topmost;

            COEditorView* sibling;
            sibling = GetSibling(1, 0);
            if (sibling) AttachSibling(sibling, 1);
            sibling = GetSibling(0, 1);
            if (sibling) AttachSibling(sibling, 0);
        }

        Position pos;
        pos.line = m_Rulers[1].m_Topmost;
        pos.column = m_Rulers[0].m_Topmost;
        MoveTo(pos);
    }

    RECT rect;
    GetClientRect(&rect);
    OnSize(SIZE_RESTORED, rect.right, rect.bottom);

    AdjustCaretPosition();
	Invalidate(FALSE);
}

BOOL COEditorView::PreCreateWindow (CREATESTRUCT& cs)
{
	WNDCLASS wndClass;
	BOOL bRes = CView::PreCreateWindow(cs);
	HINSTANCE hInstance = AfxGetInstanceHandle();

	// see if the class already exists
	if (!::GetClassInfo(hInstance, g_szClassName, &wndClass))
    {
		// get default stuff
		::GetClassInfo(hInstance, cs.lpszClass, &wndClass);
		wndClass.lpszClassName = g_szClassName;
		wndClass.style &= ~(CS_OWNDC | CS_CLASSDC | CS_PARENTDC);
        wndClass.style &= ~(CS_HREDRAW | CS_VREDRAW);
		wndClass.style |= CS_DBLCLKS /*| CS_OWNDC*/;
        wndClass.hbrBackground = 0;
        wndClass.hCursor = 0;
		// register a new class
		if (!AfxRegisterClass(&wndClass))
			AfxThrowResourceException();
	}
    cs.lpszClass = g_szClassName;

	return bRes;
}

LRESULT COEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    try
    {
        return CView::WindowProc(message, wParam, lParam);
    }
    _OE_DEFAULT_HANDLER_;

    return 0;
}

BOOL COEditorView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    try
    {
        if (!pExtra // only for commands
        && nCode != 0
        && nCode != ID_EDIT_DELETE // one exception
        && m_autocompleteList.IsActive()) 
            m_autocompleteList.HideControl();

	    return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
    }
    catch(...)
    {
      int a = 1;
    }
    // _OE_DEFAULT_HANDLER_;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COEditorView message handlers

int COEditorView::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

    m_DropTaget.Register(this);// for Drag & Drop

	return 0;
}

void COEditorView::OnSize (UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	DoSize(nType, cx, cy);
}

void COEditorView::OnHScroll (UINT nSBCode, UINT /*nPos*/, CScrollBar*)
{
    SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.fMask = SIF_TRACKPOS;

	GetScrollInfo(SB_HORZ, &scrollInfo);

    if(m_bAttached && m_Rulers[0].OnScroll(nSBCode, scrollInfo.nTrackPos, m_nMaxLineLength - 1))
    {
        Invalidate(FALSE);
        AdjustHorzScroller();
        SetCaretPosition();
    }

    // refresh status bar
    if (nSBCode == SB_THUMBTRACK)
        AfxGetApp()->OnIdle(0);
}

void COEditorView::OnVScroll (UINT nSBCode, UINT /*nPos*/, CScrollBar*)
{
    SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.fMask = SIF_TRACKPOS;

	GetScrollInfo(SB_VERT, &scrollInfo);

    if(m_bAttached && m_Rulers[1].OnScroll(nSBCode, scrollInfo.nTrackPos, GetLineCount() - 1))
    {
        Invalidate(FALSE);
        AdjustVertScroller();
        SetCaretPosition();
    }

    // refresh status bar
    if (nSBCode == SB_THUMBTRACK)
        AfxGetApp()->OnIdle(0);
}

void COEditorView::DoPageDown ()
{
    OnVScroll(SB_PAGEDOWN, 0, 0);
    AdjustVertSibling();
}

void COEditorView::DoPageUp ()
{
    OnVScroll(SB_PAGEUP, 0, 0);
    AdjustVertSibling();
}

void COEditorView::ShowCaret (bool show, bool dont_destroy_caret)
{
    if (show)
    {
        if (m_bAttached)
        {
            ::CreateCaret(
                    *this, (HBITMAP)NULL,
                    !m_isOverWriteMode
                        ? 2 * GetSystemMetrics(SM_CXBORDER) : m_Rulers[0].m_CharSize/*nWidth*/,
                    m_Rulers[1].m_CharSize/*nHeight*/
                );

            SetCaretPosition();

            ::ShowCaret(*this);
        }
    }
    else
    {
        if (!dont_destroy_caret)
        {
            ::HideCaret(*this);
            ::DestroyCaret();
        }
    }
}

void COEditorView::OnSetFocus (CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

    ShowCaret(TRUE);
}

void COEditorView::OnKillFocus (CWnd* pNewWnd)
{
	CView::OnKillFocus(pNewWnd);

    ShowCaret(FALSE);
}

void COEditorView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!m_bAttached) return;

    Position prevPos = GetPosition();

    bool _shift = (0xFF00 & GetKeyState(VK_SHIFT))   ? true : false;
    bool _cntrl = (0xFF00 & GetKeyState(VK_CONTROL)) ? true : false;
    bool _force = GetBlockMode() == ebtColumn && _shift ? true : false;

    switch (nChar)
    {
    default:
        CView::OnKeyUp(nChar, nRepCnt, nFlags);
        return;

    case VK_INSERT:
        if (!_shift && !_cntrl && !(0xFF00 & GetKeyState(VK_MENU)))
        {
            m_isOverWriteMode = !m_isOverWriteMode;
            ShowCaret(TRUE);
            return;
        }
        break;
    case VK_DELETE:
        if (!IsSelectionEmpty() && GetBlockDelAndBSDelete())
            DeleteBlock();
        else
            Delete();
        break;
    case VK_BACK:
        if (!IsSelectionEmpty() && GetBlockDelAndBSDelete())
            DeleteBlock();
        else
            Backspace();
        break;
    case VK_UP:
        GoToUp(_force);
        break;
    case VK_DOWN:
        GoToDown(_force);
        break;
    case VK_LEFT:
        if (_cntrl)
            WordLeft();
        else
            if (!_shift && !IsSelectionEmpty())
            {
                Square blkPos;
                GetSelection(blkPos);
                blkPos.normalize();

                if (prevPos != blkPos.start)
                    MoveTo(blkPos.start);
                else
                    ClearSelection();
            }
            else
                GoToLeft(_force);
        break;
    case VK_RIGHT:
        if (_cntrl)
            WordRight();
        else
            if (!_shift && !IsSelectionEmpty())
            {
                Square blkPos;
                GetSelection(blkPos);
                blkPos.normalize();

                if (prevPos != blkPos.end)
                    MoveTo(blkPos.end);
                else
                    ClearSelection();
            }
            else
                GoToRight(_force);
        break;
    case VK_HOME:
        if (_cntrl)
            GoToTop();
        else
            SmartGoToStart();
        break;
    case VK_END:
        if (_cntrl)
            GoToBottom();
        else
            GoToEnd();
        break;
    case VK_PRIOR:
        {
            AdjustCaretPosition();
            Position pos = GetPosition();

            int screenLine = pos.line - m_Rulers[1].m_Topmost;
            int topmost = m_Rulers[1].m_Topmost;

            DoPageUp();

            if (m_Rulers[1].m_Topmost != topmost)
                pos.line = m_Rulers[1].m_Topmost + screenLine;
            else
                pos.line = 0;

            MoveTo(pos, _force);
        }
        break;
    case VK_NEXT:
        {
            AdjustCaretPosition();
            Position pos = GetPosition();

            int screenLine = pos.line - m_Rulers[1].m_Topmost;
            int topmost = m_Rulers[1].m_Topmost;

            DoPageDown();

            if (m_Rulers[1].m_Topmost != topmost)
                pos.line = m_Rulers[1].m_Topmost + screenLine;

            pos.line = min(pos.line, max(0, GetLineCount()- 1));

            MoveTo(pos, _force);
        }
        break;
    case VK_ESCAPE:
        if (!IsSelectionEmpty())
            ClearSelection();
        break;
    };

    if (_shift)
    {
		SelectByCursor(prevPos);
    }
    else
    {
        if (!IsSelectionEmpty())
		    ClearSelection();
    }

    AdjustCaretPosition();

    // for better visual response
    if (nRepCnt) UpdateWindow();
}

void COEditorView::OnChar (UINT nChar, UINT nRepCnt, UINT /*nFlags*/)
{
    if (!m_bAttached) return;

    NormalizeOnCharCxt cxt;
    PreNormalizeOnChar(cxt, static_cast<TCHAR>(nChar));

    switch (nChar)
    {
    default:
        if (!IsSelectionEmpty())
            if (GetBlockTypingOverwrite()) DeleteBlock();
            else ClearSelection();

        if (m_isOverWriteMode)
            Overwrite(static_cast<TCHAR>(nChar));
        else
            Insert(static_cast<TCHAR>(nChar));
        break;

    case '\t':
        if (!IsSelectionEmpty() && !GetBlockTabIndent())
            if (GetBlockTypingOverwrite()) DeleteBlock();
            else ClearSelection();

        if ((0xFF00 & GetKeyState(VK_SHIFT)))
        {
            if (IsSelectionEmpty())
                DoUndent();
            else
            {
                CWaitCursor wait;
                UndentBlock();
            }
        }
        else
        {
            if (IsSelectionEmpty())
                DoIndent(m_isOverWriteMode ? true : false);
            else
            {
                CWaitCursor wait;
                IndentBlock();
            }
        }
        break;

     // it's processed already.
    case VK_ESCAPE:
	case VK_BACK:
        break;

     // control characters are not supported
    case 0x00: // (NUL)
    case 0x01: // (SOH)
    case 0x02: // (STX)
    case 0x03: // (ETX)
    case 0x04: // (EOT)
    case 0x05: // (ENQ)
    case 0x06: // (ACK)
    case 0x07: // (BEL)
    case 0x0A: // (LF)
    case 0x0B: // (VT)
    case 0x0C: // (FF)
    case 0x0E: // (SI)
    case 0x0F: // (SO)
    case 0x10: // (DLE)
    case 0x11: // (DC1)
    case 0x12: // (DC2)
    case 0x13: // (DC3)
    case 0x14: // (DC4)
    case 0x15: // (NAK)
    case 0x16: // (SYN)
    case 0x17: // (ETB)
    case 0x18: // (CAN)
    case 0x19: // (EM)
    case 0x1A: // (SUB)
    case 0x1C: // (FS)
    case 0x1D: // (GS)
    case 0x1E: // (RS)
    case 0x1F: // (US)
        MessageBeep((UINT)-1);
    }

    if (cxt.matched)
        NormalizeOnChar(cxt);

    AdjustCaretPosition();
    // for better visual response
    if (nRepCnt) UpdateWindow();
}

void COEditorView::OnMouseMove (UINT nFlags, CPoint point)
{
    if (!m_bAttached) return;

    if (point.x < m_Rulers[0].m_Indent
    && !(m_bMouseSelection && m_nMouseSelStartLine == -1))
        SetCursor(g_hCurArrow);
    else
    {
        m_nMouseSelStartLine = -1;
        SetCursor(g_hCurIBeam);
    }

    if (m_bMouseSelection && (nFlags & MK_LBUTTON))
    {
        SelectByMouse(point);

        point.x -= m_Rulers[0].m_Indent;

        if (point.x < m_Rulers[0].m_CharSize / 2
         || point.x > m_Rulers[0].m_ClientSize - m_Rulers[0].m_CharSize / 2
         || point.y < m_Rulers[1].m_CharSize / 2
         || point.y > m_Rulers[1].m_ClientSize - m_Rulers[1].m_CharSize / 2)
        {
            SetTimer(SEL_MOUSE_TIMER, SEL_MOUSE_TIMER_ELAPSE, NULL);
        }
    }
    else if (point.x > m_Rulers[0].m_Indent
    && !(MK_LBUTTON & nFlags) && CursorOnSelection(point)) // cursor above selection
        SetCursor(g_hCurArrow);
}

void COEditorView::OnLButtonDown (UINT nFlags, CPoint point)
{
    if (!m_bAttached) return;

    // 16.03.2003 improvement, triple click for line selection has been discontinued

    if (CursorOnSelection(point))
    {
       StartDrag();
    }
    else
    {
        Position pos = MouseToPos(point);

        if (point.x < m_Rulers[0].m_Indent) // 22.03.2003 bug fix, mouse click on gutter does not select line (since 16.03.2003)
        {
            SetCapture();
            m_bMouseSelection = TRUE;
            m_bMouseSelectionStart = TRUE;

            if (point.x < m_Rulers[0].m_Indent)
                m_nMouseSelStartLine = pos.line;

		    SelectLine(pos.line);
        }
        else
        {
            if (MK_SHIFT & nFlags)
            {
                // 16.03.2003 bug fix, mouse selection fails sometimes on left button click with pressed <shift>
                m_bMouseSelection = 
                m_bMouseSelectionStart = FALSE;

                SelectByMouse(point);
            }
            else if (MK_CONTROL & nFlags) // process it as dblclick
            {
                // 23.03.2003 improvement, added a new mouse word selectiton on left butten click with pressed ctrl
                OnLButtonDblClk(nFlags, point);
            }
            else
            {
                SetCapture();
                m_bMouseSelection = TRUE;
                m_bMouseSelectionStart = TRUE;
                m_nMouseSelStartLine = -1;

                if (point.x < m_Rulers[0].m_Indent)
                    m_nMouseSelStartLine = pos.line;

                if (!IsSelectionEmpty())
			        ClearSelection();
                MoveTo(pos);
            }
        }
    }
}

void COEditorView::OnLButtonUp (UINT /*nFlags*/, CPoint point)
{
    if (!m_bAttached) return;

    if (m_bMouseSelection)
    {
        ::ReleaseCapture();

        if (m_bMouseSelection && !IsSelectionEmpty())
        {
            Square selection;
            GetSelection(selection);
            Position pos = GetPosition();

            if (selection.start.line <= selection.end.line
            && pos.line < selection.end.line)
            {
                pos.column = 0;
                pos.line = selection.end.line;
                MoveTo(pos);
            }
        }

        m_bMouseSelection = FALSE;
        m_nMouseSelStartLine = -1;
    }

    // cursor above selection
    if (point.x > m_Rulers[0].m_Indent
    && CursorOnSelection(point))
        SetCursor(g_hCurArrow);
}

void COEditorView::OnLButtonDblClk (UINT /*nFlags*/, CPoint point)
{
    if (!m_bAttached) return;

	ClearSelection();

	Square selection;
    Position pos = MouseToPos(point);

    // 24.03.2003 improvement, MouseSelectionDelimiters has been added (hidden) which influences on double click selection behavior
    LPCTSTR delims = !GetMouseSelectionDelimiters().IsEmpty() ? GetMouseSelectionDelimiters().GetString() : 0;

    if (WordOrSpaceFromPoint(pos, selection, delims))
    {
        MoveTo(selection.end);
        SetSelection(selection);
        InvalidateSquare(selection);
    }
    else
        MoveTo(pos);
}

void COEditorView::OnTimer (UINT_PTR nIDEvent)
{
  if(nIDEvent == QUERY_TIMER)
  {
    CString empty;
    KillTimer(nIDEvent);
    ExecuteQueryRepeat(0,empty,false);
  }
  if (m_bAttached)
  {
      if (nIDEvent == SEL_MOUSE_TIMER)
      {
          if (GetKeyState(VK_LBUTTON) & 0xFF00)
          {
              bool scroll = false;

              POINT point;
              GetCursorPos(&point);
              ScreenToClient(&point);

              if (point.x - m_Rulers[0].m_Indent < m_Rulers[0].m_CharSize / 2)
              {
                  scroll = true;
                  SendMessage(WM_HSCROLL, SB_LINELEFT);
              }
              if (point.x - m_Rulers[0].m_Indent
                  > m_Rulers[0].m_ClientSize - m_Rulers[0].m_CharSize / 2)
              {
                  scroll = true;
                  SendMessage(WM_HSCROLL, SB_LINERIGHT);
              }
              if (point.y < m_Rulers[1].m_CharSize / 2)
              {
                  scroll = true;
                  SendMessage(WM_VSCROLL, SB_LINELEFT);
              }
              if (point.y > m_Rulers[1].m_ClientSize - m_Rulers[1].m_CharSize / 2)
              {
                  scroll = true;
                  SendMessage(WM_VSCROLL, SB_LINERIGHT);
              }
              if (scroll)
              {
                  SelectByMouse(point);
                  UpdateWindow();
              }
          }
          else
          {
              KillTimer(nIDEvent);
          }
      }
      else if (nIDEvent == DRAG_MOUSE_TIMER)
      {
          m_bDragScrolling = TRUE;            
          KillTimer(nIDEvent);
      }
    }
    else
        CView::OnTimer(nIDEvent);
}

void COEditorView::OnUpdate_Mode (CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_isOverWriteMode);
}

void COEditorView::OnUpdate_Pos (CCmdUI* pCmdUI)
{
    TCHAR buff[80];
    Position pos = GetPosition();
    _stprintf_s(buff,80,_T(" Ln: %d, Col: %d"), pos.line + 1, pos.column + 1);
    pCmdUI->SetText(buff);
    pCmdUI->Enable(TRUE);
}

void COEditorView::OnUpdate_ScrollPos (CCmdUI* pCmdUI)
{
    TCHAR buff[80];
    _stprintf_s(buff,80, _T(" Top: %d, Left: %d"), m_Rulers[1].m_Topmost + 1, m_Rulers[0].m_Topmost + 1);
    pCmdUI->SetText(buff);
    pCmdUI->Enable(TRUE);
}

void COEditorView::OnUpdate_BlockType (CCmdUI* pCmdUI)
{
    pCmdUI->SetText(GetBlockMode() == ebtColumn ? _T(" Sel: Columnar ") : _T(" Sel: Stream "));
    pCmdUI->Enable(TRUE);
}

// 29.06.2003 improvement, status line indicator for character under cursor 
void COEditorView::OnUpdate_CurChar (CCmdUI* pCmdUI)
{
    Position pos = GetPosition();

    if (m_curCharCache.pos != pos)
    {
        m_curCharCache.pos = pos;
        m_curCharCache.message = _T(" Chr: ");

        if (pos.line < GetLineCount()
        && pos.column < GetLineLength(pos.line))
        {
            int len;
            TCHAR buff[80];
            LPCTSTR str;
            GetLine(pos.line, str, len);
            int inx = pos2inx(str, len, pos.column);
            m_curCharCache.message += '\'';
            m_curCharCache.message += str[inx];
            m_curCharCache.message += _T("'=0x");
            _itot_s(str[inx], buff, 80, 16);
            m_curCharCache.message += buff;
            m_curCharCache.message += _T("=");
            _itot_s(str[inx], buff, 80, 10);
            m_curCharCache.message += buff;
        }
    }

    pCmdUI->SetText(m_curCharCache.message.GetString());
    pCmdUI->Enable(TRUE);
}

void COEditorView::OnEditToggleColumnarSelection()
{
    SetBlockMode(ebtColumn);
}

void COEditorView::OnEditToggleStreamSelection()
{
    SetBlockMode(ebtStream);
}

void COEditorView::OnToggleStreamSelection()
{
  EBlockMode blkMode = GetBlockMode();
  switch(blkMode)
  {
    case ebtStream: SetBlockMode(ebtColumn); break;
    case ebtColumn: SetBlockMode(ebtStream); break;
  }
}

void COEditorView::OnUpdate_SelectionType(CCmdUI* pCmdUI)
{
    BOOL enable = FALSE;
    EBlockMode blkMode = GetBlockMode();
    switch (pCmdUI->m_nID)
    {
      case ID_EDIT_COLUMN_SEL: enable = blkMode == ebtColumn; break;
      case ID_EDIT_STREAM_SEL: enable = blkMode == ebtStream; break;
    }
    pCmdUI->SetRadio(enable);
}

void COEditorView::DoEditCopy (const CString& buff, bool append)
{
  CWaitCursor wait;
  StylePutStringToClipboard(buff,GetSafeHwnd(),append);
}

void COEditorView::OnEditCopy()
{
    if (!m_bAttached) return;

    if (!IsSelectionEmpty())
    {
        CWaitCursor wait;

        CString buff;
        GetBlock(buff);
        DoEditCopy(buff);
    }
}

void COEditorView::OnEditCut()
{
    if (!m_bAttached) return;

    if (!IsSelectionEmpty())
    {
        OnEditCopy();
        DeleteBlock();
    }
}

void COEditorView::OnEditPaste()
{
  if (!m_bAttached) return;

  CString text = StyleGetStringFromClipboard(GetSafeHwnd());

  if(!text.IsEmpty())
  {
    if(!IsSelectionEmpty())
    {
      if(GetBlockMode() == ebtColumn)
      {
        Square blkPos;
        GetSelection(blkPos);
        Position curPos;
        curPos.line   = min(blkPos.start.line,   blkPos.end.line);
        curPos.column = min(blkPos.start.column, blkPos.end.column);
        DeleteBlock();
        MoveTo(curPos, true);
      }
      else
      {
        DeleteBlock();
      }
    }
    InsertBlock(text);
  }
}

void COEditorView::OnUpdate_EditCopyAndCut (CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!IsSelectionEmpty());
}

void COEditorView::OnUpdate_EditPaste (CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_TEXT));
}

void COEditorView::OnEditUndo ()
{
    CWaitCursor wait;

    if (m_bAttached) Undo();
}

void COEditorView::OnEditRedo ()
{
    CWaitCursor wait;

    if (m_bAttached) Redo();
}

void COEditorView::OnUpdate_EditUndo (CCmdUI* pCmdUI)
{
    if (m_bAttached)
        pCmdUI->Enable(CanUndo() ? TRUE : FALSE);
    else
        pCmdUI->Enable(FALSE);
}

void COEditorView::OnUpdate_EditRedo (CCmdUI* pCmdUI)
{
    if (m_bAttached)
        pCmdUI->Enable(CanRedo() ? TRUE : FALSE);
    else
        pCmdUI->Enable(FALSE);
}

void COEditorView::OnEditSelectAll()
{
    SelectAll();
}

void COEditorView::OnEditDeleteLine()
{
  DeleteLine();
}

void COEditorView::OnEditDeleteWordToLeft()
{
  DeleteWordToLeft();
}

void COEditorView::OnEditDeleteWordToRight()
{
  DeleteWordToRight();
}

void 
COEditorView::OnEditFind()
{
  CString buff;
  Square sqr;

  GetBlockOrWordUnderCursor(buff, sqr, true/*onlyOneLine*/);
  SetSearchText(buff);

  CFindReplaceDlg(FALSE, this).DoModal(false);
}

void 
COEditorView::OnEditReplace()
{
  CString buff;
  Square sqr;

  if (GetBlockOrWordUnderCursor(buff, sqr, true/*onlyOneLine*/))
  {
    SetSearchText(buff);
  }
  CFindReplaceDlg(TRUE, this).DoModal(false);
}

void COEditorView::OnEditFindNext()
{
  COEditorView* pView = this;
	RepeatSearch(esdDown, pView);
}

void COEditorView::OnEditFindPrevious()
{
  COEditorView* pView = this;
  GoToLeft();
	RepeatSearch(esdUp, pView);
}

bool COEditorView::Find (const COEditorView*& pView, int& line, int& start, int& end, bool thruEof) const
{
    const OpenEditor::EditContext* pEditContext;

    if (EditContext::Find(pEditContext, line, start, end, thruEof))
    {
        pView = static_cast<const COEditorView*>(pEditContext);

        CWnd* pParent = pView->GetParent();
        while (pParent && !pParent->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
            pParent = pParent->GetParent();

        if (pParent)
            ((CMDIChildWnd*)(pParent))->MDIActivate();
        else
            _ASSERTE(0);

        return true;
    }
    return false;
}

bool COEditorView::RepeatSearch (SearchDirection direction, COEditorView*& pView)
{
    CWaitCursor wait;

    Square blk;
    GetSelection(blk);
    Position pos = GetPosition();

    if (!blk.is_empty()
    &&  blk.start.line   == pos.line   && blk.end.line   == pos.line
    && (blk.start.column == pos.column || blk.end.column == pos.column))
    {
        blk.normalize();
        pos = blk.end;
    }
    else
    {
        blk.start = blk.end = pos;
    }

    bool backward = IsBackwardSearch();

    if (direction == esdDown && backward || direction == esdUp && !backward)
    {
      SetBackwardSearch(direction == esdUp ? true : false);
    }
    const COEditorView* view = pView;
    if (Find(view, blk.end.line, blk.start.column, blk.end.column))
    {
        blk.start.line = blk.end.line;
        bool vcenter = blk.end.line < m_Rulers[1].m_Topmost || blk.end.line > m_Rulers[1].m_Topmost + m_Rulers[1].m_Count;

        pView->SetSelection(blk);
        pView->MoveTo(blk.start);  // 08/09/2002 bug fix, the searched text has been found but may be still invisible
        if (vcenter) OnEditScrollCenter();
        pView->MoveTo(blk.end);
       

        if (pView == this &&
           (direction == esdDown && (blk.end == pos || blk.end < pos) || 
            direction == esdUp   && (blk.end == pos || pos < blk.end) ))
        {
            Common::SetStatusText(_T("Passed the end of the file"));
        }
        else
        {
          Common::SetStatusText(_T(""));
        }
        if (direction == esdDown && backward || direction == esdUp && !backward)
        {
          SetBackwardSearch(backward);
        }
        return true;
    }

    CString message(_T("Cannot find the string \'"));
    message += GetSearchText();
    message += _T("\'.");
    AfxMessageBox(message.GetString());

    if (direction == esdDown && backward || direction == esdUp && !backward)
    {
      SetBackwardSearch(backward);
    }
    return false;
}

void COEditorView::SetQueueBookmark (int line, bool on)
{
    try
    {
        SetBookmark(line, eBmkGroup1, on);
    }
    catch (const std::out_of_range&)
    {
        MessageBeep((UINT)-1);
        Common::SetStatusText(_T("Cannot toggle a bookmark below EOF!"));
    }
}

void COEditorView::OnEditBookmarkToggle ()
{
    try
    {
        Position pos = GetPosition();
        SetBookmark(pos.line, eBmkGroup1, !IsBookmarked(pos.line, eBmkGroup1));
    }
    catch (const std::out_of_range&)
    {
        MessageBeep((UINT)-1);
        Common::SetStatusText(_T("Cannot toggle a bookmark below EOF!"));
    }
}

void COEditorView::OnEditBookmarkNext ()
{
    Position pos = GetPosition();

    if (EditContext::NextBookmark(pos.line, eBmkGroup1))
    {
        pos.column = 0;
        MoveToAndCenter(pos);
    }
}

void COEditorView::OnEditBookmarkPrev ()
{
    Position pos = GetPosition();

    if (EditContext::PreviousBookmark(pos.line, eBmkGroup1))
    {
        pos.column = 0;
        MoveToAndCenter(pos);
    }
}

void COEditorView::OnEditBookmarkRemoveAll ()
{
    EditContext::RemoveAllBookmark(eBmkGroup1);
}

void COEditorView::OnSetRandomBookmark (UINT nBookmarkCmd)
{
    RandomBookmark bookmark(static_cast<_TUCHAR>(nBookmarkCmd - ID_EDIT_BKM_SET_0));

    _CHECK_AND_THROW_(bookmark.Valid(), _T("Invalid id for random bookmark!"));

    try
    {
        const RandomBookmarkArray& bookmarks = GetRandomBookmarks();

        int line = bookmarks.GetLine(bookmark);

        // turn off a bookmark with the same number
        if (line != -1)
            SetRandomBookmark(bookmark, line, false);

        Position pos = GetPosition();
        if (line != pos.line)
        {
            // turn of another bookmark on the line
            // i must do it because of a implementation limitation
            for (RandomBookmark i(RandomBookmark::FIRST); i.Valid(); i++)
                if (bookmarks.GetLine(i) == pos.line)
                    SetRandomBookmark(i, pos.line, false);

            // turn on
            SetRandomBookmark(bookmark, pos.line, true);
        }
    }
    catch (const std::out_of_range&)
    {
        MessageBeep((UINT)-1);
        Common::SetStatusText(_T("Cannot toggle a bookmark below EOF!"));
    }
}

void COEditorView::OnGetRandomBookmark (UINT nBookmarkCmd)
{
    RandomBookmark bookmark(static_cast<_TUCHAR>(nBookmarkCmd - ID_EDIT_BKM_GET_0));

    _CHECK_AND_THROW_(bookmark.Valid(), _T("Invalid id for random bookmark!"));

    Position pos;
    pos.column = 0;
    pos.line   = 0;

    if (GetRandomBookmark(bookmark, pos.line))
        MoveToAndCenter(pos);
}

void COEditorView::OnUpdate_GetRandomBookmark (CCmdUI* pCmdUI)
{
    int line;
    RandomBookmark bookmark(static_cast<_TUCHAR>(pCmdUI->m_nID - ID_EDIT_BKM_GET_0));
    pCmdUI->Enable((bookmark.Valid() && GetRandomBookmark(bookmark, line)) ? TRUE : FALSE);
}

void COEditorView::OnUpdate_BookmarkGroup (CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetBookmarkNumber(eBmkGroup1) > 0);
}

void COEditorView::OnContextMenu (CWnd*, CPoint point)
{
    if (point.x == -1 && point.y == -1)
    {
        Position pos = GetPosition();

        // 31.03.2003 bug fix, editor context menu position is wrong on Shift+F10
        point.x = m_Rulers[0].PosToPix(pos.column);
        point.y = m_Rulers[1].PosToPix(pos.line) + m_Rulers[1].m_CharSize/2;

        CRect rc;
        GetClientRect(rc);
        if (!rc.PtInRect(point))
            point.x = point.y = 4;

        ClientToScreen(&point);
    }

    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_OE_EDIT_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    ASSERT_KINDOF(CFrameWnd, AfxGetMainWnd());
    Common::GUICommandDictionary::AddAccelDescriptionToMenu(pPopup->m_hMenu);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void COEditorView::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    if (!bSysMenu && pPopupMenu)
    {
        UINT flag = (IsSelectionEmpty()) ? MF_BYCOMMAND|MF_GRAYED : MF_BYCOMMAND|MF_ENABLED;
        pPopupMenu->EnableMenuItem(ID_EDIT_COPY, flag);
        pPopupMenu->EnableMenuItem(ID_EDIT_CUT, flag);

        flag = (!IsClipboardFormatAvailable(CF_TEXT)) ? MF_BYCOMMAND|MF_GRAYED : MF_BYCOMMAND|MF_ENABLED;
        pPopupMenu->EnableMenuItem(ID_EDIT_PASTE, flag);

        flag = (!CanUndo()) ? MF_BYCOMMAND|MF_GRAYED : MF_BYCOMMAND|MF_ENABLED;
        pPopupMenu->EnableMenuItem(ID_EDIT_UNDO, flag);

        flag = (!CanRedo()) ? MF_BYCOMMAND|MF_GRAYED : MF_BYCOMMAND|MF_ENABLED;
        pPopupMenu->EnableMenuItem(ID_EDIT_REDO, flag);

        flag = (IsSearchTextEmpty()) ? MF_BYCOMMAND|MF_GRAYED : MF_BYCOMMAND|MF_ENABLED;
        pPopupMenu->EnableMenuItem(ID_EDIT_FIND_NEXT, flag);
        pPopupMenu->EnableMenuItem(ID_EDIT_FIND_PREVIOUS, flag);
    }
    else
	    CView::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

void COEditorView::OnUpdate_EditFindNext(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(IsSearchTextEmpty() ? FALSE : TRUE);
}

void COEditorView::OnUpdate_SelectionOperation (CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!IsSelectionEmpty());
}


void COEditorView::OnEditLower ()
{
    CWaitCursor wait;
    ScanAndReplaceText(LowerText, IsSelectionEmpty());
}

void COEditorView::OnEditUpper ()
{
    CWaitCursor wait;
    ScanAndReplaceText(UpperText, IsSelectionEmpty());
}

void COEditorView::OnEditCapitalize()
{
    CWaitCursor wait;
    ScanAndReplaceText(CapitalizeText, IsSelectionEmpty());
}

void COEditorView::OnEditInvertCase()
{
    CWaitCursor wait;
    ScanAndReplaceText(InvertCaseText, IsSelectionEmpty());
}

void COEditorView::OnBlockTabify()
{
    CWaitCursor wait;

    if (!IsSelectionEmpty() && GetBlockMode() == ebtColumn)
        SetBlockMode(ebtStream);

    ScanAndReplaceText(TabifyText, false);
}

void COEditorView::OnBlockUntabify()
{
    CWaitCursor wait;

    if (!IsSelectionEmpty() && GetBlockMode() == ebtColumn)
        SetBlockMode(ebtStream);

    ScanAndReplaceText(UntabifyText, false);
}


void COEditorView::OnBlockTabifyLeading()
{
    CWaitCursor wait;

    if (!IsSelectionEmpty() && GetBlockMode() == ebtColumn)
        SetBlockMode(ebtStream);

    ScanAndReplaceText(TabifyLeadingSpaces, false);
}

void COEditorView::OnEditIndent()
{
    CWaitCursor wait;

    IndentBlock();
}

void COEditorView::OnEditUndent()
{
    CWaitCursor wait;

    UndentBlock();
}

void COEditorView::OnEditFindMatch()
{
    CWaitCursor wait;
    EditContext::FindMatch(false);
}

void COEditorView::OnEditFindMatchAndSelect()
{
    CWaitCursor wait;
    EditContext::FindMatch(true);
}

void COEditorView::OnEditComment ()
{
    DoCommentText(true);
}

void COEditorView::OnEditUncomment ()
{
    DoCommentText(false);
}

void COEditorView::DoCommentText (bool comment)
{
    CWaitCursor wait;

    Position pos = GetPosition();
    EBlockMode blkMode = GetBlockMode();
    bool selected = !IsSelectionEmpty();

    if (selected && blkMode == ebtColumn)
    {
        AfxMessageBox(_T("The comment/uncomment operation currently supports only stream selection."));
        return;
    }

    try
    {
        LanguagePtr lang = LanguagesCollection::Find(GetSettings().GetLanguage());
        
        if (lang->GetEndLineComment().IsEmpty())
        {
            AfxMessageBox(CString(_T("Line comment string has not been defined for language \"")) + lang->GetName() + _T("\"."));
            return;
        }

        if (selected)
            ConvertSelectionToLines();
        else
		    SelectLine(pos.line);

        if (comment)
            ScanAndReplaceText(CommentText, false);
        else
            ScanAndReplaceText(UncommentText, false);

        if (!selected)
        {
            ClearSelection();
            SetBlockMode(blkMode);
        }
    }
    catch (const std::logic_error& x) // language not found
    {
        AfxMessageBox(CString(_T("Cannot comment/uncomment the selection.\n")) + x.what());
        return;
    }
}

bool COEditorView::CommentText (const OpenEditor::EditContext& context, CString& str)
{
    CString lineComment = LanguagesCollection::Find(context.GetSettings().GetLanguage())->GetEndLineComment();

    // skip spaces and tabs
    int i;
    int len;
    for (i = 0, len = str.GetLength(); isspace(str[i]) && i < len; i++)
        ;

    // comment line
    if(!str.IsEmpty() && i < len)
    {
      str = lineComment + str;
    }
    return true;
}

bool COEditorView::UncommentText (const OpenEditor::EditContext& context, CString& str)
{
    CString lineComment = LanguagesCollection::Find(context.GetSettings().GetLanguage())->GetEndLineComment();

    // skip spaces and tabs
    int i,len;
    for (i = 0, len = (int)str.GetLength(); isspace(str[i]) && i < len; i++)
        ;


    if(!str.IsEmpty() && i < len
       && !_tcsncmp(str.GetString() + i,lineComment.GetString(),lineComment.GetLength()))
    {
      str = str.Left(i - 1) + str.Mid(i + lineComment.GetLength());
      // str.erase(i,lineComment.GetLength());
    }
    return true;
}

void 
COEditorView::OnEditGoto()
{
  Position pos = GetPosition();

  static int whereFrom = 0;
  int whereTo = (whereFrom == 0) ? pos.line + 1 : 0;
  CString infoString;
  infoString.Format(_T("There are %u lines in the file.\r\nThe current line is %u.")
                    ,GetLineCount(), pos.line + 1);

  COEGoToDialog dlg(this,whereFrom,whereTo,infoString);
 
  if (dlg.DoModal() == IDOK)
  {
    switch (dlg.m_WhereFrom)
    {
      case 0: // from top of the file
              pos.line = dlg.m_WhereTo - 1;
              break;
      case 1: // from bottom of the file
              pos.line = GetLineCount() - dlg.m_WhereTo;
              break;
      case 2: // from current position
              pos.line += dlg.m_WhereTo;
              break;
    }
    MoveToAndCenter(pos);
    whereFrom = dlg.m_WhereFrom;
  }
}

void COEditorView::OnEditScrollUp ()
{
    AdjustCaretPosition();

    OnVScroll(SB_LINEUP, 0, 0);

    if (GetPosition().line >= m_Rulers[1].m_Topmost + m_Rulers[1].m_Count)
        GoToUp();

    AdjustCaretPosition();
    AdjustVertSibling();
}

void COEditorView::OnEditScrollDown ()
{
    AdjustCaretPosition();

    OnVScroll(SB_LINEDOWN, 0, 0);

    if (GetPosition().line < m_Rulers[1].m_Topmost)
        GoToDown();

    AdjustCaretPosition();
    AdjustVertSibling();
}

void COEditorView::OnEditScrollCenter ()
{
    int topmost = GetPosition().line - m_Rulers[1].m_Count/2;

    if (topmost < 0) topmost = 0;

    if (topmost != m_Rulers[1].m_Topmost)
    {
        m_Rulers[1].m_Topmost = topmost;
        Invalidate(FALSE);
        AdjustHorzScroller();
        AdjustVertSibling();
        SetCaretPosition();
    }

}

void COEditorView::OnEditExpandTemplate ()
{
    /* !!! 2 modes for expand - quick and exact !!! */
    if (!ExpandTemplate()) 
    {
        if (!m_autocompleteList.m_hWnd) 
            m_autocompleteList.Create();

        if (m_autocompleteList.m_hWnd)  
            m_autocompleteList.ShowControl(this);
    }
}

void COEditorView::MoveToBottom ()
{
    ClearSelection();
    GoToBottom();
    AdjustCaretPosition();
}

void COEditorView::MoveToAndCenter (Position pos)
{
    bool center 
        = pos.line < m_Rulers[1].m_Topmost 
            || pos.line > m_Rulers[1].m_Topmost + m_Rulers[1].m_Count;

    if (!IsSelectionEmpty())
        ClearSelection();

    MoveTo(pos);

    if (center) 
        OnEditScrollCenter();
}

void COEditorView::GetBlock (CString& str, const Square* sqr) const
{
    CWaitCursor wait;

    EditContext::GetBlock(str, sqr);
}

void COEditorView::InsertBlock (LPCTSTR str, bool hideSelection, bool putSelInUndo)
{
    CWaitCursor wait;

    EditContext::InsertBlock(str, hideSelection, putSelInUndo);
}

void COEditorView::DeleteBlock (bool putSelInUndo)
{
    CWaitCursor wait;

    EditContext::DeleteBlock(putSelInUndo);
}

void COEditorView::OnEditDelete ()
{
    if (!m_bAttached) return;

    if (!IsSelectionEmpty() && GetBlockDelAndBSDelete())
        DeleteBlock();
    else
        Delete();
}

void COEditorView::OnEditCutAndAppend ()
{
    if (!m_bAttached) return;

    if (!IsSelectionEmpty())
    {
        OnEditCopyAndAppend();
        DeleteBlock();
    }
}

void COEditorView::OnEditCopyAndAppend ()
{
    if (!m_bAttached) return;

    if (!IsSelectionEmpty())
    {
        CWaitCursor wait;

        CString buff;
        GetBlock(buff);
        DoEditCopy(buff, true);
    }
}

void COEditorView::OnEditCutBookmarked ()
{
    OnEditCopyBookmarked();
    OnEditDeleteBookmarked();
}

void COEditorView::OnEditCopyBookmarked ()
{
    CWaitCursor wait;

    CString buff;
    CopyBookmarkedLines(buff);
    DoEditCopy(buff);
}

void COEditorView::OnEditDeleteBookmarked ()
{
    CWaitCursor wait;

    DeleteBookmarkedLines();
}

void COEditorView::OnEditSelectWord ()
{
	Square selection;
    if (WordFromPoint(GetPosition(), selection))
    {
        MoveTo(selection.end);
        SetSelection(selection);
        InvalidateSquare(selection);
    }
}

void COEditorView::OnEditSelectLine ()
{
    SelectLine(GetPosition().line);
}

BOOL COEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (!m_uWheelScrollLines)
	    ::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &m_uWheelScrollLines, 0);

	if (m_uWheelScrollLines == WHEEL_PAGESCROLL)
	{
        OnVScroll(zDelta > 0 ? SB_PAGEDOWN : SB_PAGEUP , 0, 0);
	}
    else
    {
		int nToScroll = ::MulDiv(zDelta, m_uWheelScrollLines, WHEEL_DELTA);

        if (zDelta > 0)
            while (nToScroll--)
                OnVScroll(SB_LINELEFT, 0, 0);
        else
            while (nToScroll++)
                OnVScroll(SB_LINERIGHT, 0, 0);
    }

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void COEditorView::OnSettingChange (UINT, LPCTSTR)
{
    m_uWheelScrollLines = 0;
}

void COEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
    if(!bActivate)
    {
        if (m_autocompleteList.IsActive()) 
            m_autocompleteList.HideControl();
    }
    else
    {
      GetDocument()->SetTitle();
    }
    CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void COEditorView::OnEditNormalizeText()
{
    CWaitCursor wait;
    ScanAndReplaceText(NormalizeText, IsSelectionEmpty());
}

void COEditorView::OnTest ()
{
}
