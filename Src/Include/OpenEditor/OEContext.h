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

#pragma once
#ifndef __OEContext_h__
#define __OEContext_h__

#include <common/arg_shared.h>
#include "OpenEditor/OEHelpers.h"
#include "OpenEditor/OEStorage.h"

#define OEC_DECLARE_GET_PROPERTY(T,N) \
        const T& Get##N () const \
        { _ASSERTE(m_pStorage); return m_pStorage->GetSettings().Get##N(); };

namespace std
{
    class istrstream;
}

namespace OpenEditor
{
    using std::map;
    using std::vector;
    using std::string;
    using std::istream;
    using std::ostream;
    using std::counted_ptr;

    class GeneralMatchSearcher;
    class PlSqlMatchSearcher;

    class EditContext : public EditContextBase
    {
    public:
        EditContext ();
        virtual ~EditContext ();

        void Init ();
        void SetStorage (Storage*);

        int  GetLineCount () const;
        LineId GetLineId (int line) const;
        int  GetLineLength (int line) const;
        void GetLine (int line, const char*& ptr, int& len) const;

        int GetCursorLine () const;
        int GetCursorColumn () const;
        Position GetPosition () const;

        int InxToPos (int, int) const;
        int PosToInx (int, int, bool = false) const;

        /*
           all navigation methods return true
           if action is successful and false if it is not,
           but cursor position may be was changed by adjustment
        */
        bool GoToUp (bool force = false);
        bool GoToDown (bool force = false);
        bool GoToLeft (bool force = false);
        bool GoToRight (bool force = false);

        bool GoToTop ();
        bool GoToBottom ();
        bool GoToStart (bool force = false);
        bool GoToEnd (bool force = false);

        bool WordRight ();
        bool WordLeft ();

        bool MoveTo (Position, bool force = false);
        void AdjustPosition (Position& pos, bool force = false) const;
        bool SmartGoToStart ();

        void DoIndent (bool moveOnly = false);
        void DoUndent ();
        void DoCarriageReturn ();

        void Insert     (char);
        void Overwrite  (char);
        void Backspace  ();
        void Delete     ();

        void DeleteLine ();
	      void DeleteWordToLeft ();
	      void DeleteWordToRight ();

        bool CanUndo () const;
        bool CanRedo () const;
        void Undo ();
        void Redo ();

        bool IsSelectionEmpty () const;
        void SetSelection (const Square&);
        void GetSelection (Square&) const;
        void ClearSelection ();
        void ConvertSelectionToLines ();

        void SelectAll ();
        void SelectLine (int line);
        void SelectByCursor (Position prevPos);

        bool WordFromPoint (Position, Square&, const char* delims = 0) const;
        bool GetBlockOrWordUnderCursor (string& buff, Square& sqr, bool onlyOneLine, const char* delims = 0);
        bool WordOrSpaceFromPoint (Position, Square&, const char* delims = 0) const;

        virtual void GetBlock    (string&, const Square* = 0) const;
        virtual void InsertBlock (const char*);
        virtual void InsertBlock (const char*, bool hideSelection, bool putSelInUndo = true);
        virtual void DeleteBlock (bool putSelInUndo = true);

        void IndentBlock ();
        void UndentBlock ();

        void CopyBookmarkedLines (string&) const;
        void DeleteBookmarkedLines ();

        bool ExpandTemplate (int = -1);

        void FindMatch (bool select = false);

        void SetBlockMode (EBlockMode);
        EBlockMode GetBlockMode () const;

        void ScanMultilineQuotes (int to_line, int& state, int& quoteId, bool& parsing) const;
        MultilineQuotesScanner& GetMultilineQuotesScanner ();

        void OnSettingsChanged ();

        virtual void AdjustCaretPosition() = 0;
        virtual void InvalidateWindow () = 0;
        virtual void InvalidateLines  (int start, int end) = 0;
        virtual void InvalidateSquare (const Square&) = 0;
        virtual void MoveToAndCenter (Position pos) = 0;

        // Text searching
        bool IsSearchTextEmpty () const;
        const char* GetSearchText () const;
        void SetSearchText (const char* str);
        void GetSearchOption (bool& backward, bool& wholeWords, bool& matchCase, bool& regExpr, bool& searchAll) const;
        void SetSearchOption (bool backward, bool wholeWords, bool matchCase, bool regExpr, bool searchAll);
        bool IsBackwardSearch () const;
        void SetBackwardSearch (bool backward);

        // 1st is out-parameter, 2d,3d,4th are in-out
        bool Find (const EditContext*&, int& line, int& start, int& end, bool thruEof = true) const;
        // 1st,2d,3d are in-parameter, 4th is out
        bool Replace (const char* text, int line, int start, int& end);
        int  SearchBatch (const char* text, ESearchBatch mode);

        // Bookmark supporting
        bool IsBookmarked      (int line, EBookmarkGroup group) const;
        void SetBookmark       (int line, EBookmarkGroup group, bool on);
        void RemoveAllBookmark (EBookmarkGroup group);
        int  GetBookmarkNumber (EBookmarkGroup group);
        bool NextBookmark      (int& line, EBookmarkGroup group) const;
        bool PreviousBookmark  (int& line, EBookmarkGroup group) const;

        bool IsRandomBookmarked(int line, RandomBookmark& bookmarkId) const;
        void SetRandomBookmark (RandomBookmark bookmark, int line, bool on);
        bool GetRandomBookmark (RandomBookmark bookmark, int& line) const;
        // the bookmarks will be valid untill storage content is not changed
        const RandomBookmarkArray& GetRandomBookmarks () const;

        bool FindById (LineId id, int& line) const;

        // Scan for reading and writing (see OpenEditor_Context_5.cpp)
        void StartScan (EBlockMode, const Square* = 0);
        bool Next ();
        bool Eof () const;
        void GetScanLine (const char*& ptr, int& len) const;
        void GetScanLine (int& line, int& start, int& end) const;
        void PutLine (const char* ptr, int len);
        void StopScan ();

        // Text manipulation utilities (see OpenEditor_Context_5.cpp)
        void ScanAndReplaceText (bool (pmfnDo)(const EditContext&, string&), bool curentWord);
        static bool LowerText (const EditContext&, string&);
        static bool UpperText (const EditContext&, string&);
        static bool CapitalizeText (const EditContext&, string&);
        static bool InvertCaseText (const EditContext&, string&);
        static bool TabifyText (const EditContext&, string&);
        static bool TabifyLeadingSpaces (const EditContext&, string&);
        static bool UntabifyText (const EditContext&, string&);
        static bool UntabifyLeadingSpaces (const EditContext&, string&);

        // Settings
        const Settings& GetSettings () const;
        const DelimitersMap& GetDelimiters() const;

        OEC_DECLARE_GET_PROPERTY(int, TabSpacing);
        OEC_DECLARE_GET_PROPERTY(int, IndentSpacing);
        OEC_DECLARE_GET_PROPERTY(int, IndentType);
        OEC_DECLARE_GET_PROPERTY(bool, TabExpand);
        OEC_DECLARE_GET_PROPERTY(bool, CursorBeyondEOL);
        OEC_DECLARE_GET_PROPERTY(bool, CursorBeyondEOF);

        OEC_DECLARE_GET_PROPERTY(bool, BlockKeepMarking               );
        OEC_DECLARE_GET_PROPERTY(bool, BlockKeepMarkingAfterDragAndDrop);
        OEC_DECLARE_GET_PROPERTY(bool, BlockDelAndBSDelete            );
        OEC_DECLARE_GET_PROPERTY(bool, BlockTypingOverwrite           );
        OEC_DECLARE_GET_PROPERTY(bool, BlockTabIndent                 );
        OEC_DECLARE_GET_PROPERTY(bool, ColBlockDeleteSpaceAfterMove   );
        OEC_DECLARE_GET_PROPERTY(bool, ColBlockCursorToStartAfterPaste);
        OEC_DECLARE_GET_PROPERTY(string, MouseSelectionDelimiters     );

        OEC_DECLARE_GET_PROPERTY(string, PrintHeader);
	      OEC_DECLARE_GET_PROPERTY(string, PrintFooter);
        OEC_DECLARE_GET_PROPERTY(int,    PrintMarginMeasurement);
	      OEC_DECLARE_GET_PROPERTY(double, PrintLeftMargin);
	      OEC_DECLARE_GET_PROPERTY(double, PrintRightMargin);
	      OEC_DECLARE_GET_PROPERTY(double, PrintTopMargin);
	      OEC_DECLARE_GET_PROPERTY(double, PrintBottomMargin);

        void HighlightCurrentLine (bool);
        bool IsCurrentLineHighlighted () const;

    private:
        Square m_ScanSquare;
        EBlockMode m_ScanBlockMode;
        int m_nScanCurrentLine, m_nScanStartPos, m_nScanEndPos;
        void calculateScanLine ();
    public:

        class UndoGroup : public Storage::UndoGroup
        {
        public:
            UndoGroup (EditContext&);
        };

        void PushInUndoStack (Position);
        void PushInUndoStack (EBlockMode, Square);

        // Memory usage reporting
        void GetMemoryUsage (unsigned& usage, unsigned& allocated, unsigned& undo) const;
    private:
        friend UndoGroup;

        // string mustn't have '\r' or '\n'
        void InsertLine     (int, const char*, int);      
        void InsertLinePart (int, int, const char*, int);
        void DeleteLinePart (int, int, int);
        void SplitLine      (int, int);

        void expandVirtualSpace (int line, int column);

        bool adjustCursorPosition ();

    protected:
        int inx2pos (int, int) const;
        int inx2pos (const char*, int, int) const;
        int pos2inx (int, int, bool = false) const;
        int pos2inx (const char*, int, int, bool = false) const;
        void moveCurrentLine (int);
    private:
        int adjustPosByTab (int, int, bool = false) const;

        void line2buff (int, int, int, string&, bool = false) const;

        Position wordRight (bool hurry);
        Position wordLeft (bool hurry);

        static bool getLine (std::istrstream&, string&, bool&);

        Storage* m_pStorage;

        Position m_curPos;
        int      m_orgHrzPos;
        bool     m_bDirtyPosition;
        bool     m_bCurrentLineHighlighted;

        Square  m_blkPos;
        EBlockMode m_BlockMode;

        static bool tabify (const EditContext&, string&, bool leading);
        static bool untabify (const EditContext&, string&, bool leading);
    };

    ///////////////////////////////////////////////////////////////////////////

    inline
    int EditContext::GetCursorLine () const
    {
        return m_curPos.line;
    }

    inline
    int EditContext::GetCursorColumn () const
    {
        return m_curPos.column;
    }

    inline
    Position EditContext::GetPosition () const
    {
        return m_curPos;
    }

    inline
    int EditContext::InxToPos (int line, int col) const
    {
        return inx2pos(line, col);
    }

    inline
    int EditContext::PosToInx (int line, int col, bool force) const
    {
        return pos2inx(line, col, force);
    }

    inline
    int EditContext::GetLineCount () const
    {
        return m_pStorage->GetLineCount();
    }

    inline
    LineId EditContext::GetLineId (int line) const
    {
        return m_pStorage->GetLineId(line);
    }

    inline
    void EditContext::GetLine (int line, const char*& ptr, int& len) const
    {
        m_pStorage->GetLine(line, ptr, len);
    }

    inline
    bool EditContext::CanUndo () const
    {
        return m_pStorage->CanUndo();
    }

    inline
    bool EditContext::CanRedo () const
    {
        return m_pStorage->CanRedo();
    }

    inline
    bool EditContext::IsSelectionEmpty () const
    {
        return m_blkPos.is_empty();
    }

    inline
    void EditContext::GetSelection (Square& pos) const
    {
        if (m_blkPos.is_empty())
            pos.start = pos.end = m_curPos;
        else
            pos = m_blkPos;
    }

    inline
    void EditContext::SetBlockMode (EBlockMode mode)
    {
        m_BlockMode = mode;
        InvalidateLines(m_blkPos.start.line, m_blkPos.end.line);
    }

    inline
    EBlockMode EditContext::GetBlockMode () const
    {
        return m_BlockMode;
    }

    inline
    void EditContext::ScanMultilineQuotes (int to_line, int& state, int& quoteId, bool& parsing) const
    {
        _ASSERTE(m_pStorage);
        m_pStorage->ScanMultilineQuotes(to_line, state, quoteId, parsing);
    }

    inline
    MultilineQuotesScanner& EditContext::GetMultilineQuotesScanner ()
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->GetMultilineQuotesScanner();
    }

    // Text searching
    inline
    bool EditContext::IsSearchTextEmpty () const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->IsSearchTextEmpty();
    }

    inline
    const char* EditContext::GetSearchText () const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->GetSearchText();
    }

    inline
    void EditContext::SetSearchText (const char* str)
    {
        _ASSERTE(m_pStorage);
        m_pStorage->SetSearchText(str);
    }

    inline
    void EditContext::GetSearchOption (bool& backward, bool& wholeWords, bool& matchCase, bool& regExpr, bool& searchAll) const
    {
        _ASSERTE(m_pStorage);
        m_pStorage->GetSearchOption(backward, wholeWords, matchCase, regExpr, searchAll);
    }

    inline
    void EditContext::SetSearchOption (bool backward, bool wholeWords, bool matchCase, bool regExpr, bool searchAll)
    {
        _ASSERTE(m_pStorage);
        m_pStorage->SetSearchOption(backward, wholeWords, matchCase, regExpr, searchAll);
    }

    inline
    bool EditContext::IsBackwardSearch () const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->IsBackwardSearch();
    }

    inline
    void EditContext::SetBackwardSearch (bool backward)
    {
        _ASSERTE(m_pStorage);
        m_pStorage->SetBackwardSearch(backward);
    }

    // Bookmark supporting
    inline
    bool EditContext::IsBookmarked (int line, EBookmarkGroup group) const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->IsBookmarked(line, group);
    }

    inline
    void EditContext::SetBookmark (int line, EBookmarkGroup group, bool on)
    {
        _ASSERTE(m_pStorage);
        m_pStorage->SetBookmark(line, group, on);
    }

    inline
    void EditContext::RemoveAllBookmark (EBookmarkGroup group)
    {
        _ASSERTE(m_pStorage);
        m_pStorage->RemoveAllBookmark(group);
    }

    inline
    int EditContext::GetBookmarkNumber (EBookmarkGroup group)
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->GetBookmarkNumber(group);
    }

    inline
    bool EditContext::NextBookmark (int& line, EBookmarkGroup group) const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->NextBookmark(line, group);
    }

    inline
    bool EditContext::PreviousBookmark (int& line, EBookmarkGroup group) const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->PreviousBookmark(line, group);
    }

    inline
    bool EditContext::IsRandomBookmarked (int line, RandomBookmark& bookmarkId) const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->IsRandomBookmarked(line, bookmarkId);
    }

    inline
    void EditContext::SetRandomBookmark (RandomBookmark bookmark, int line, bool on)
    {
        _ASSERTE(m_pStorage);
        m_pStorage->SetRandomBookmark(bookmark, line, on);
    }

    inline
    bool EditContext::GetRandomBookmark (RandomBookmark bookmark, int& line) const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->GetRandomBookmark(bookmark, line);
    }

    inline
    const RandomBookmarkArray& EditContext::GetRandomBookmarks () const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->GetRandomBookmarks();
    }

    inline
    bool EditContext::FindById (LineId id, int& line) const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->FindById(id, line);
    }

    inline
    EditContext::
    UndoGroup::UndoGroup (EditContext& context)
    : Storage::UndoGroup(*context.m_pStorage)
    {
    }

    // Memory usage reporting
    inline
    void EditContext::GetMemoryUsage (unsigned& usage, unsigned& allocated, unsigned& undo) const
    {
        _ASSERTE(m_pStorage);
        m_pStorage->GetMemoryUsage(usage, allocated, undo);
    }

    inline
    const Settings& EditContext::GetSettings () const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->GetSettings();
    }

    inline
    const DelimitersMap& EditContext::GetDelimiters() const
    {
        _ASSERTE(m_pStorage);
        return m_pStorage->GetDelimiters();
    }

    // the current line highlighting 
    inline
    void EditContext::HighlightCurrentLine (bool enable)
    {
        m_bCurrentLineHighlighted = enable;
    }

    inline
    bool EditContext::IsCurrentLineHighlighted () const
    {
        return m_bCurrentLineHighlighted;
    }

    inline
    void EditContext::moveCurrentLine (int line)
    {
        m_curPos.line = line;
    }
};

#endif//__OEContext_h__
