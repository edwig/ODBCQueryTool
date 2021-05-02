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

// 17.09.2002 performance tuning, insert/delete block operation
// 23.09.2002 improvement/performance tuning, undo has been reimplemented
// 06.12.2002 bug fix, in find dialog count and mark change "modified" status

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OEStorage_h__
#define __OEStorage_h__

#include <map>
#include <deque>
#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <COMMON/ExceptionHelper.h>
#include <COMMON/FixedString.h>
#include "OpenEditor/OEHelpers.h"
#include "OpenEditor/OESettings.h"
#include "OpenEditor/OEUndo.h"
#include "OpenEditor/OELanguageSupport.h"


namespace OpenEditor
{
    using std::map;
    using std::deque;
    using std::vector;
    using std::string;
    using std::istream;
    using std::ostream;
    using arg::counted_ptr;

    static const size_t maxlen = USHRT_MAX;

    class Storage : SettingsSubscriber
    {
    public:
        Storage ();
        ~Storage ();

        void Clear ();

        void LinkContext   (EditContextBase*);
        void UnlinkContext (EditContextBase*);
        void Notify_ChangedLines (int, int);
        void Notify_ChangedLinesQuantity (int, int);
        void Notify_ChangedLinesStatus (int, int);
        void OnSettingsChanged ();

        // Load & Save
        void SetText (
            const char* text, unsigned long length, // text pointer end length
            bool use_buffer = false,                // use external persistent buffer
            bool refresh    = false,                // rescan for reallocated buffer
            bool external   = false                 // external changes, it requires additional synchronization
            );
        unsigned long GetTextLength () const;
        unsigned long GetText (char*, unsigned long) const;
        void TruncateSpaces (bool force = false);
        void SetSavedUndoPos ();

        const char* GetFileFormatName () const; 

        // Line
        int  GetLineCount  () const;
        int  GetLineLength (int line) const;
        LineId GetLineId   (int line) const;

        char GetChar (int line, int pos) const /*throw(std::out_of_range)*/;
        void GetLine (int line, const char*& ptr) const;
        void GetLine (int line, const char*& ptr, int& len) const;
        void ScanMultilineQuotes (int to_line, int& state, int& quoteId, bool& parsing) const;

        // Edit
        // get id of the last data-updating operation
        SequenceId GetActionId () const;

        void Insert     (char, int, int);
        void Overwrite  (char, int, int);
        void Delete     (int, int);

        // string mustn't have '\r' or '\n'
        void InsertLine     (int, const char*, int);
        void InsertLinePart (int, int, const char*, int);
        void ReplaceLinePart(int, int, int, const char*, int);
        void DeleteLine     (int);
        void DeleteLinePart (int, int, int);
        void SplitLine      (int, int);
        void MergeLines     (int);
        void ExpandLinesTo  (int);

        void InsertLines (int, const StringArray&);
        void DeleteLines (int, int);

        // Undo
        void EnableUndo (bool = true);
        bool IsUndoEnabled () const;
        bool CanUndo () const;
        bool CanRedo () const;
        bool Undo (UndoContext& cxt);
        bool Redo (UndoContext& cxt);
        void ClearUndo ();
        bool IsModified () const;

        void PushInUndoStack (const Position&);
        void PushInUndoStack (EBlockMode, const Square&);

        // Settings
        const Settings& GetSettings () const;
        void SetSettings (const Settings*);
        const DelimitersMap& GetDelimiters () const;

        // Searcher
        void SetSearcher (Searcher*);
        Searcher& GetSearcher ();
        const Searcher& GetSearcher () const;

        bool IsSearchTextEmpty () const;
        const char* GetSearchText () const;
        void SetSearchText (const char* str);
        void GetSearchOption (bool& backward, bool& wholeWords, bool& matchCase, bool& regExpr, bool& searchAll) const;
        void SetSearchOption (bool backward, bool wholeWords, bool matchCase, bool regExpr, bool searchAll);
        bool IsBackwardSearch () const;
        void SetBackwardSearch (bool backward);

        // 1st is out-parameter, 2d,3d,4th are in-out
        bool Find (const Storage*&, int& line, int& start, int& end, bool thruEof) const;

        // 1st,2d,3d are in-parameters ,4th is out
        bool Replace (const char* text, int line, int start, int& end);
        int  SearchBatch (const char* text, ESearchBatch mode, Square& last);

        MultilineQuotesScanner& GetMultilineQuotesScanner ();

        EditContextBase* GetFistEditContext();
        const EditContextBase* GetFistEditContext() const;

        // Bookmark supporting
        bool IsBookmarked      (int line, EBookmarkGroup group) const;
        void SetBookmark       (int line, EBookmarkGroup group, bool on);
        void RemoveAllBookmark (EBookmarkGroup group);
        int  GetBookmarkNumber (EBookmarkGroup group);
        bool NextBookmark      (int& line, EBookmarkGroup group) const;
        bool PreviousBookmark  (int& line, EBookmarkGroup group) const;
        void GetBookmarkedLines (std::vector<int>&, EBookmarkGroup group) const;

        bool IsRandomBookmarked(int line, RandomBookmark& bookmarkId) const;
        void SetRandomBookmark (RandomBookmark bookmark, int line, bool on);
        bool GetRandomBookmark (RandomBookmark bookmark, int& line) const;
        // the bookmarks will be valid untill storage content is not changed
        const RandomBookmarkArray& GetRandomBookmarks () const;

        bool FindById (LineId id, int& line) const;

        // Memory usage reporting
        void GetMemoryUsage (unsigned& usage, unsigned& allocated, unsigned& undo) const;

        // Helpers
        class NotificationDisabler
        {
        public:
            NotificationDisabler (Storage*, bool noNotificationUndo = false);
            ~NotificationDisabler ();

            void Enable ();
            
            Storage* GetStorage ();
            const Storage* GetStorage () const;
        private:
            Storage* m_pOwner;
            bool m_noNotificationUndo;
        };

        class UndoGroup
        {
        public:
            UndoGroup (Storage&, const Position* = 0);
            ~UndoGroup ();
        private:
            Storage& m_Storage;
        };

        // language support
        LanguageSupportPtr GetLanguageSupport () { return m_languageSupport; }

        // for background processing
        bool OnIdle ();

    private:
        friend class NotificationDisabler;
        friend class QuoteBalanceChecker;
        friend class UndoStack;
        friend class UndoGroup;
        friend class UndoNotification;

        StringArray m_Lines;

        char m_szLineDelim[3]; // not 0 for loaded files
        static const char m_cszDosLineDelim  [3]; //= "\r\n";
        static const char m_cszMacLineDelim  [3]; //= "\n\r";
        static const char m_cszUnixLineDelim [3]; //= "\n";
        const char* getLineDelim () const; // it returns poiter to char[3]

        bool m_bDisableNotifications;
        mutable MultilineQuotesScanner m_Scanner;

        Sequence m_actionSeq, m_lineSeq; 
        bool m_bUndoEnabled, m_bUndoRedoProcess;
        UndoStack m_UndoStack;
        UndoGroup* m_pUndoGroup;

        vector<EditContextBase*> m_refContextList;

        const Settings* m_pSettings; // shared settings

        Searcher* m_pSearcher;
        static Searcher m_defSearcher;

        // Bookmark supporting
        int m_BookmarkCountersByGroup[BOOKMARK_GROUPS_SIZE]; // 1st group is reserved for random bookmark

        // a copy from settings
        DelimitersMap m_delimitersMap;

        // private methods
        void partialClear ();
        void setQuotesValidLimit (int) const;
        void pushInUndoStack (UndoBase*);

        // language support
        LanguageSupportPtr m_languageSupport;

        // bookmark cache
        mutable RandomBookmarkArray m_RandomBookmarks;
    };

    inline
    void Storage::SetSavedUndoPos () 
    { 
        m_UndoStack.SetSavedPos(); 
    }

    inline
    int Storage::GetLineCount () const
    {
        return (int)m_Lines.size();
    }

    inline
    int Storage::GetLineLength (int line) const
    {
        return (int)m_Lines.at(line).length();
    }

    inline
    LineId Storage::GetLineId (int line) const
    {
        return LineId(m_Lines.at(line).tag.id);
    }

    inline
    char Storage::GetChar (int line, int pos) const /*throw(std::out_of_range)*/
    {
        // remove cast after FixedString update
        //if (pos > static_cast<int>(FixedString::maxlen))
        if (pos > static_cast<int>(maxlen))
            FixedString::_Xran();

        return m_Lines.at(line).at(pos);
    }

    inline
    SequenceId Storage::GetActionId () const
    {
        return m_actionSeq;
    }

    inline
    void Storage::setQuotesValidLimit (int line) const
    {
        m_Scanner.Invalidate(line);
    }

    inline
    void Storage::ScanMultilineQuotes (int to_line, int& state, int& quoteId, bool& parsing) const
    {
       m_Scanner.Scan(to_line, state, quoteId, parsing);
    }

    inline
    MultilineQuotesScanner& Storage::GetMultilineQuotesScanner ()
    {
        return m_Scanner;
    }

    inline
    bool Storage::CanUndo () const
    {
        return m_bUndoEnabled && m_UndoStack.CanUndo();
    }

    inline
    bool Storage::CanRedo () const
    {
        return m_bUndoEnabled && m_UndoStack.CanRedo();
    }

    inline
    bool Storage::Undo (UndoContext& cxt)
    {
        return m_UndoStack.Undo(cxt);
    }

    inline
    bool Storage::Redo (UndoContext& cxt)
    {
        return m_UndoStack.Redo(cxt);
    }

    inline
    void Storage::ClearUndo ()
    {
        m_UndoStack.Clear();
    }

    inline
    bool Storage::IsModified () const
    {
        return m_UndoStack.IsDataModified();
    }

    inline
    void Storage::PushInUndoStack (const Position& pos)
    {
        pushInUndoStack(new UndoCursorPosition(pos.line, pos.column));
    }

    inline
    void Storage::PushInUndoStack (EBlockMode mode, const Square& sel)
    {
        pushInUndoStack(new UndoSelection(mode, sel));
    }


    inline
    const Settings& Storage::GetSettings () const
    {
        _ASSERTE(m_pSettings);
        return *m_pSettings;
    }

    inline
    const DelimitersMap& Storage::GetDelimiters () const
    {
        return m_delimitersMap;
    }

    inline
    Searcher& Storage::GetSearcher ()
    {
        _ASSERTE(m_pSearcher);
        return *m_pSearcher;
    }

    inline
    const Searcher& Storage::GetSearcher () const
    {
        _ASSERTE(m_pSearcher);
        return *m_pSearcher;
    }

    inline
    const char* Storage::GetSearchText () const
    {
        _ASSERTE(m_pSearcher);
        return m_pSearcher->GetText();
    }

    inline
    bool Storage::IsSearchTextEmpty () const
    {
        _ASSERTE(m_pSearcher);
        return m_pSearcher->IsTextEmpty();
    }

    inline
    void Storage::SetSearchText (const char* str)
    {
        _ASSERTE(m_pSearcher);
        m_pSearcher->SetText(str);
    }

    inline
    void Storage::GetSearchOption (bool& backward, bool& wholeWords, bool& matchCase, bool& regExpr, bool& searchAll) const
    {
        _ASSERTE(m_pSearcher);
        m_pSearcher->GetOption(backward, wholeWords, matchCase, regExpr, searchAll);
    }

    inline
    void Storage::SetSearchOption (bool backward, bool wholeWords, bool matchCase, bool regExpr, bool searchAll)
    {
        _ASSERTE(m_pSearcher);
        m_pSearcher->SetOption(backward, wholeWords, matchCase, regExpr, searchAll);
    }

    inline
    bool Storage::IsBackwardSearch () const
    {
        _ASSERTE(m_pSearcher);
        return m_pSearcher->IsBackwardSearch();
    }

    inline
    void Storage::SetBackwardSearch (bool backward)
    {
        _ASSERTE(m_pSearcher);
        m_pSearcher->SetBackwardSearch(backward);
    }

    inline
    bool Storage::Find (const Storage*& pStorage, int& line, int& start, int& end, bool thruEof) const
    {
        _ASSERTE(m_pSearcher);
        pStorage = this;

        return m_pSearcher->Find(pStorage, line, start, end, thruEof);
    }

    inline
    bool Storage::Replace (const char* text, int line, int start, int& end)
    {
        _ASSERTE(m_pSearcher);
        return m_pSearcher->Replace(this, text, line, start, end);
    }

    inline
    int Storage::SearchBatch (const char* text, ESearchBatch mode, Square& last)
    {
        _ASSERTE(m_pSearcher);
        return m_pSearcher->DoBatch(this, text, mode, last);
    }

    inline
    EditContextBase* Storage::GetFistEditContext()
    {
        return m_refContextList.size() ? m_refContextList.front() : 0;
    }

    inline
    const EditContextBase* Storage::GetFistEditContext() const
    {
        return m_refContextList.size() ? m_refContextList.front() : 0;
    }

    inline
    bool Storage::IsBookmarked (int line, EBookmarkGroup group) const
    {
        return m_Lines.at(line).tag.bmkmask & (1 << group) ? true : false;
    }

    inline
    bool Storage::IsRandomBookmarked (int line, RandomBookmark& bookmarkId) const
    {
        if (Storage::IsBookmarked(line, eRandomBmkGroup))
        {
            bookmarkId = RandomBookmark(m_Lines[line].tag.bookmark);
            return true;
        }
        return false;
    }

    inline
    void Storage::pushInUndoStack (UndoBase* undo)
    {
        if (m_bUndoEnabled && !m_bUndoRedoProcess) 
            m_UndoStack.Push(undo);
        else
        {
            _ASSERTE(0);
            delete undo;
        }
    }

    inline
    Storage::NotificationDisabler::~NotificationDisabler ()              
    { 
        Enable(); 
    }

    inline
    Storage* Storage::NotificationDisabler::GetStorage ()                
    { 
        return m_pOwner; 
    }
    
    inline
    const Storage* Storage::NotificationDisabler::GetStorage () const    
    { 
        return m_pOwner; 
    }
};

#endif//__OEStorage_h__
