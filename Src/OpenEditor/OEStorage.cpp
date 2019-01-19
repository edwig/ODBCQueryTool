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
    09.04.2002 bug fix, debug exception on undo & redo
    13.05.2002 bug fix, undo operation completes with garbage on empty lines or following by them
    17.09.2002 performance tuning, insert/delete block operation
    23.09.2002 improvement/performance tuning, undo has been reimplemented
    24.09.2002 bug fix, go to previos bookmark does not work properly
    06.12.2002 bug fix, in find dialog count and mark change "modified" status
    06.01.2002 performance tuning, file reading became 5-10 faster as result of lines' array preallocation.
               Actually it's a fix for performance degradation since 097 build 27.
    13.10.2003 small improvement, if pos == 0 then the second line inherits bookmarks
*/


#include "stdafx.h"
#include <COMMON/ExceptionHelper.h>
#include "OpenEditor/OESettings.h"
#include "OpenEditor/OEStorage.h"
#include "OpenEditor/OELanguageManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{
#define PUSH_IN_UNDO_STACK(undo) \
    if (m_bUndoEnabled && !m_bUndoRedoProcess) pushInUndoStack(undo);

    using namespace std;
    using namespace Common;

    const char Storage::m_cszDosLineDelim  [3] = "\r\n";
    const char Storage::m_cszMacLineDelim  [3] = "\n\r";
    const char Storage::m_cszUnixLineDelim [3] = "\n";

    Searcher Storage::m_defSearcher(true /*persistent*/);

    Storage::
    UndoGroup::UndoGroup (Storage& storage, const Position* pos)
    : m_Storage(storage)
    {
        if (!m_Storage.m_pUndoGroup)
        {
            m_Storage.m_pUndoGroup = this;

            if (pos)
                m_Storage.pushInUndoStack(new UndoCursorPosition(pos->line, pos->column));
        }
    }

    Storage::
    UndoGroup::~UndoGroup ()
    {
        if (m_Storage.m_pUndoGroup == this)
        {
            m_Storage.m_pUndoGroup = 0;
            m_Storage.m_UndoStack.NextUndoGroup();
        }
    }

    Storage::NotificationDisabler::NotificationDisabler (Storage* owner, bool noNotificationUndo)
    : m_pOwner(owner),
    m_noNotificationUndo(noNotificationUndo)
    {
        m_pOwner->m_bDisableNotifications = true;
        if (!m_noNotificationUndo)
            m_pOwner->pushInUndoStack(new UndoNotification(true));

    }

    void Storage::NotificationDisabler::Enable ()
    {
        m_pOwner->m_bDisableNotifications = false;
        m_pOwner->Notify_ChangedLines(0, m_pOwner->GetLineCount());
        if (!m_noNotificationUndo)
            m_pOwner->pushInUndoStack(new UndoNotification(false));
    }

    /// private helper class //////////////////////////////////////////////////
    class QuoteBalanceChecker
    {
        int state[3], quoteId[3];
        bool parsing[3];
    public:
        void ScanBeforeUpdate (const Storage* pStorage, int line);
        void NotifyAboutChanged (Storage* pStorage, int line);
    };

    void QuoteBalanceChecker::ScanBeforeUpdate (const Storage* pStorage, int line)
    {
        if (!pStorage->m_bDisableNotifications)
        {
            state[0]   = 0;
            quoteId[0] = 0;
            parsing[0] = false;
            pStorage->m_Scanner.Scan(line, state[0], quoteId[0], parsing[0]);

            state[1]   = state[0];
            quoteId[1] = quoteId[0];
            parsing[1] = parsing[0];
            pStorage->m_Scanner.ScanLine(pStorage->m_Lines.at(line).data(),
                                        (int)pStorage->m_Lines[line].length(),
                                        state[1], quoteId[1], parsing[1]);
        }
    }

    void QuoteBalanceChecker::NotifyAboutChanged (Storage* pStorage, int line)
    {
        if (!pStorage->m_bDisableNotifications)
        {
            state[2]   = state[0];
            quoteId[2] = quoteId[0];
            parsing[2] = parsing[0];
            pStorage->m_Scanner.ScanLine(pStorage->m_Lines.at(line).data(),
                                        (int)pStorage->m_Lines[line].length(),
                                        state[2], quoteId[2], parsing[2]);

            if (state[1] != state[2]
            || quoteId[1] != quoteId[2] // ?????
            || parsing[1] != parsing[2])
            {
                pStorage->Notify_ChangedLines(line, (int)pStorage->m_Lines.size());
            }
            else
            {
                pStorage->Notify_ChangedLines(line, line);
            }
        }
    }
    /// private helper class //////////////////////////////////////////////////

#pragma warning ( disable : 4355 )
Storage::Storage ()
    : m_Scanner(*this),
    m_Lines(2 * 1024,  16 * 1024)
{
    m_bDisableNotifications = false;
    setQuotesValidLimit(0);
    m_Lines.append();

    m_bUndoEnabled = true;
    m_bUndoRedoProcess = false;
    m_pUndoGroup = 0;

    m_pSettings = 0;
    m_pSearcher = 0;
    SetSearcher(&m_defSearcher);

    memset(m_BookmarkCountersByGroup, 0, sizeof m_BookmarkCountersByGroup);
    memset(m_szLineDelim, 0, sizeof m_szLineDelim);
}
#pragma warning ( default : 4355 )

Storage::~Storage ()
{
    if (m_pSettings)
        m_pSettings->RemoveSubscriber(this);

    if (m_pSearcher)
        m_pSearcher->RemoveRef(this);
}

void Storage::Clear ()
{
    memset(m_BookmarkCountersByGroup, 0, sizeof m_BookmarkCountersByGroup);
    partialClear();

    int orgLines = (int)m_Lines.size();
    m_Lines.clear();
    Notify_ChangedLinesQuantity(0, -orgLines);
}

void Storage::partialClear ()
{
    m_actionSeq++;

    setQuotesValidLimit(0);

    m_UndoStack.Clear();
}

void Storage::LinkContext (EditContextBase* context)
{
    if (context)
    {
        vector<EditContextBase*>::iterator
            it(m_refContextList.begin()), end(m_refContextList.end());

        for (; it != end && *it != context; it++);

        if (it == end)
           m_refContextList.push_back(context);
    }
}

void Storage::UnlinkContext (EditContextBase* context)
{
    if (context)
    {
        vector<EditContextBase*>::iterator
            it(m_refContextList.begin()), end(m_refContextList.end());

        for (; it != end && *it != context; it++);

        if (*it == context)
           m_refContextList.erase(it, it + 1);
    }
}

void Storage::Notify_ChangedLines (int startLine, int endLine)
{
    if (!m_bDisableNotifications)
    {
        vector<EditContextBase*>::iterator
            it(m_refContextList.begin()), end(m_refContextList.end());

        for (; it != end; it++)
            (*it)->OnChangedLines(startLine, endLine);

        if (startLine != endLine)
            setQuotesValidLimit(startLine);
    }
}

void Storage::Notify_ChangedLinesQuantity (int line, int quantity)
{
    if (!m_bDisableNotifications)
    {
        vector<EditContextBase*>::iterator
            it(m_refContextList.begin()), end(m_refContextList.end());

        for (; it != end; it++)
            (*it)->OnChangedLinesQuantity(line, quantity);

        setQuotesValidLimit(line);
    }
}

void Storage::Notify_ChangedLinesStatus (int startLine, int endLine)
{
    if (!m_bDisableNotifications)
    {
        vector<EditContextBase*>::iterator
            it(m_refContextList.begin()), end(m_refContextList.end());

        for (; it != end; it++)
            (*it)->OnChangedLinesStatus(startLine, endLine);
    }
}

void Storage::OnSettingsChanged ()
{
    m_languageSupport
        = LanguageManager::CreateLanguageSupport(GetSettings().GetLanguage(), this);

    m_UndoStack.SetCountLimit(GetSettings().GetUndoLimit());
    m_UndoStack.SetMemLimit(GetSettings().GetUndoMemLimit());

    m_delimitersMap.Set(GetSettings().GetDelimiters().c_str());

    vector<EditContextBase*>::iterator
        it(m_refContextList.begin()), end(m_refContextList.end());

    for (; it != end; it++)
        (*it)->OnSettingsChanged();
}

void Storage::SetText (
    const char* text, unsigned long length, // text pointer end length
    bool use_buffer,                        // use external persistent buffer
    bool refresh,                           // rescan for reallocated buffer
    bool external                           // external changes, it requires additional synchronization
    )
{
    size_t orgLines = m_Lines.size();
    size_t orgIncrement = m_Lines.increment();

    if (!refresh)
    {
        Clear();
        m_Lines.reserve(length / 80);
        m_Lines.increment() = length / 160;
    }

    // default
    memcpy(m_szLineDelim, m_cszDosLineDelim, sizeof(m_szLineDelim));

    {
    // check text type (Unix (\n), Dos (\r\n), Mac (\n\r))
    const char* ptr;
    for (ptr = text; ptr && *ptr != '\n' && *ptr != '\r' && ptr < text + length; ptr++)
        ;
    if (ptr < text + length)
    {
        if (ptr[0] == '\n')
        {
            if (ptr + 1 < text + length && ptr[1] == '\r')
                memcpy(m_szLineDelim, m_cszMacLineDelim, sizeof(m_szLineDelim));
            else
                memcpy(m_szLineDelim, m_cszUnixLineDelim, sizeof(m_szLineDelim));
        }
        else if (ptr[0] == '\r')
        {
            if (ptr + 1 < text + length && ptr[1] == '\n')
                memcpy(m_szLineDelim, m_cszDosLineDelim, sizeof(m_szLineDelim));
            else
                ; // show warning and use default
        }
    }
    }

    String buff;
    int pos = 0;
    const char* ptr = text;
    bool trunc = GetSettings().GetTruncateSpaces();

    unsigned line(0), size((int)m_Lines.size());
    while (ptr + pos < text + length)
    {
        if (ptr[pos] == m_szLineDelim[0])
        {
            if (use_buffer)
                buff.assign_static(ptr, pos);
            else
                buff.assign(ptr, pos);

            if (trunc) buff.truncate();

            if (!refresh || line >= size)
            {
                buff.tag.id = m_lineSeq.GetNextVal();
                m_Lines.push_back(buff);
                line++;
            }
            else
            {
                m_Lines[line++].assign(buff);
            }

            if (m_szLineDelim[1] && ((ptr + pos + 1) < text + length))
            {
                if (ptr[pos + 1] == m_szLineDelim[1])
                    pos++;
                else
                    ; // do nothing, ignore unregular eol
            }

            ptr += pos + 1;
            pos = 0;
        }
        else
            pos++;
    }

    if (pos) // ptr is not empty
    {
        if (use_buffer)
            buff.assign_static(ptr, pos);
        else
            buff.assign(ptr, pos);

        if (trunc) buff.truncate();

        if (!refresh || line >= size)
        {
            buff.tag.id = m_lineSeq.GetNextVal();
            m_Lines.push_back(buff);
            line++;
        }
        else
        {
            m_Lines[line++].assign(buff);
        }
    }
    // the previos line exists and has <CR>
    else if (m_Lines.size() > 1)
    {
        buff.cleanup();

        if (!refresh || line >= size)
        {
            buff.tag.id = m_lineSeq.GetNextVal();
            m_Lines.push_back(buff);
            line++;
        }
        else
        {
            m_Lines[line++].assign(buff);
        }
    }
    // it's a reload and a new file has less lines than previous one
    if (!refresh || line < size)
        m_Lines.reduce(line);


    if (m_Lines.size() != orgLines)
        Notify_ChangedLinesQuantity(
                max(0, static_cast<int>(min((int)m_Lines.size(), (int)orgLines)) - 1),
                (int)m_Lines.size() - (int)orgLines
            );

    // invalidate all views
    if (external)
        Notify_ChangedLines(0, (int)m_Lines.size() - 1);

    // reset undo and scaner
    if (external || m_Lines.size() != orgLines)
        partialClear();

    m_Lines.increment() = orgIncrement;
}


const char* Storage::getLineDelim () const
{
    int saveAs = GetSettings().GetFileSaveAs();

    if (saveAs == effDefault)
    {
        if (m_szLineDelim[0]) return m_szLineDelim;

        saveAs = GetSettings().GetFileCreateAs();
    }

    switch (saveAs)
    {
    case effMac:    return m_cszMacLineDelim;
    case effUnix:   return m_cszUnixLineDelim;
    case effDos:    return m_cszDosLineDelim;
    default:        _ASSERTE(0);
    }

    return m_cszDosLineDelim;
}

const char* Storage::GetFileFormatName () const
{
    const char* delim = getLineDelim();
    if (delim[0] == '\r')       return " Dos ";
    else if (delim[1] == '\r')  return " Mac ";
    else                        return " Unix ";
}

unsigned long Storage::GetTextLength () const
{
    // initialize a line delimiter
    int delim_length = getLineDelim()[1] ? 2 : 1;

    // calculate text size
    unsigned long usage = 0;
    bool the_first = true;

    for (int line(0), nlines = (int)m_Lines.size(); line < nlines; line++)
    {
        usage += (int)m_Lines[line].length() + (!the_first ? delim_length : 0);
        the_first = false;
    }

    return usage;
}

unsigned long Storage::GetText (char* buffer, unsigned long size) const
{
    // initialize a line delimiter
    const char* ln_delim = getLineDelim();
    int ln_delim_len = ln_delim[1] ? 2 : 1;

    // copy text
    char* ptr = buffer;
    bool the_first = true;

    for (int line(0), nlines = (int)m_Lines.size(); line < nlines; line++)
    {
        if (the_first)
            the_first = false;
        else
        {
            _CHECK_AND_THROW_((ptr + ln_delim_len) <= (buffer + size), "Text buffer size is not enough.");
            memcpy(ptr, ln_delim, ln_delim_len);
            ptr += ln_delim_len;
        }

        const FixedString& str = m_Lines[line];
        _CHECK_AND_THROW_((ptr + str.length()) <= (buffer + size), "Text buffer size is not enough.");
        memcpy(ptr, str.data(), str.length());
        ptr += str.length();
    }

    return (long)(ptr - buffer);
}

void Storage::TruncateSpaces (bool force)
{
    if (force || GetSettings().GetTruncateSpaces())
    {
        for (int line(0), nlines = (int)m_Lines.size(); line < nlines; line++)
            m_Lines[line].truncate();
    }
}

void Storage::GetLine  (int line, const char*& ptr) const
{
    ptr = m_Lines.at(line).data();
}

void Storage::GetLine (int line, const char*& ptr, int& len) const
{
  if(line >= 0 && line < (int)m_Lines.size())
  {
    ptr = m_Lines.at(line).data();
    len = (int)m_Lines[line].length();
  }
  else
  {
    ptr = nullptr;
    len = 0;
  }
}

void Storage::Insert (char ch, int line, int pos)
{
    m_actionSeq++;

    if (ch != '\r')
    {
        char buff[2] = { ch, 0 };
        InsertLinePart(line, pos, buff, 1);
    }
    else
    {
        if (!m_Lines.size()) m_Lines.append();
        SplitLine(line, pos);
    }
}

void Storage::Overwrite (char ch, int line, int pos)
{
    _ASSERTE(ch != '\r');

    if (ch != '\r')
    {
        m_actionSeq++;

        QuoteBalanceChecker chk;
        chk.ScanBeforeUpdate(this, line);

        char orgStr[2] = { m_Lines.at(line).at(pos), 0 };
        m_Lines[line].replace(pos, ch);
        char newStr[2] = { ch, 0 };

        PUSH_IN_UNDO_STACK(new UndoOverwrite(line, pos, orgStr, 1, newStr, 1));

        chk.NotifyAboutChanged(this, line);
    }
}

void Storage::Delete (int line, int pos)
{
    _ASSERTE(line < static_cast<int>(m_Lines.size()));

	if (line < static_cast<int>(m_Lines.size()))
	{
        m_actionSeq++;

		if (pos == static_cast<int>(m_Lines.at(line).length()))
            MergeLines(line);
		else
            DeleteLinePart(line, pos, pos + 1);
	}
}

// string mustn't have '\r' or '\n'
void Storage::InsertLine (int line, const char* str, int len)
{
    m_actionSeq++;

    String buff;
    buff.tag.id = m_lineSeq.GetNextVal();
    if (str) buff.assign(str, len);
    m_Lines.insert(buff, line);
    PUSH_IN_UNDO_STACK(new UndoInsertLine(line, str, len));

    Notify_ChangedLinesQuantity(line, 1);
    Notify_ChangedLines(line, (int)m_Lines.size());
}

void Storage::InsertLines (int line, const StringArray& lines)
{
    m_actionSeq++;
    m_Lines.insert(lines, line);

    for (unsigned i = line; i < line + lines.size(); i++)
        m_Lines[i].tag.id = m_lineSeq.GetNextVal();

    PUSH_IN_UNDO_STACK(new UndoInsertLines(line, lines));

    Notify_ChangedLinesQuantity(line, (int)lines.size());
    Notify_ChangedLines(line, (int)m_Lines.size());
}


// string mustn't have '\r' or '\n'
void Storage::InsertLinePart (int line, int pos, const char* str, int len)
{
    m_actionSeq++;

    QuoteBalanceChecker chk;
    chk.ScanBeforeUpdate(this, line);

    m_Lines.at(line).insert(pos, str, len);

    if (m_bUndoEnabled && !m_bUndoRedoProcess)
    {
        if (!m_UndoStack.AppendOnInsert(line, pos, str, len, m_delimitersMap))
            PUSH_IN_UNDO_STACK(new UndoInsert(line, pos, str, len));
    }

    chk.NotifyAboutChanged(this, line);
}

void Storage::ReplaceLinePart (int line, int from_pos, int to_pos, const char* str, int len)
{
    _ASSERTE((to_pos - from_pos) >= 0);

	if (line < static_cast<int>(m_Lines.size())
    && from_pos < static_cast<int>(m_Lines[line].length())
    && (to_pos - from_pos) >= 0)
	{
        m_actionSeq++;

        QuoteBalanceChecker chk;
        chk.ScanBeforeUpdate(this, line);

        String orgStr = m_Lines[line];

        if (to_pos > static_cast<int>(orgStr.length()))
            to_pos = (int)orgStr.length();

        m_Lines[line].erase(from_pos, to_pos - from_pos);
        m_Lines[line].insert(from_pos, str, len);

        PUSH_IN_UNDO_STACK(new UndoOverwrite(line, from_pos, orgStr.data() + from_pos, to_pos - from_pos, str, len));

        chk.NotifyAboutChanged(this, line);
    }
}

void Storage::DeleteLine (int line)
{
	if (line < static_cast<int>(m_Lines.size()))
	{
        m_actionSeq++;

        String str = m_Lines[line];
        if (str.tag.bmkmask)
            for (int i(0); i < BOOKMARK_GROUPS_SIZE; i++)
                if (str.tag.bmkmask & (1 << i))
                    m_BookmarkCountersByGroup[i]--;

        m_Lines.erase(line);
        PUSH_IN_UNDO_STACK(new UndoDeleteLine(line, str.data(), (int)str.length()));

		Notify_ChangedLinesQuantity(max(0, line - 1), -1);
        Notify_ChangedLines(line, (int)m_Lines.size());
	}
}

void Storage::DeleteLines (int line, int count)
{
	if (line < static_cast<int>(m_Lines.size()))
	{
        m_actionSeq++;

        StringArray lines(count, 1024);
        for (int i = 0; i < count; i++)
        {
            const String& str = m_Lines[line + i];
            lines.append().assign(str.data(), str.length(), true);

            if (str.tag.bmkmask)
                for (int j(0); j < BOOKMARK_GROUPS_SIZE; j++)
                    if (str.tag.bmkmask & (1 << j))
                        m_BookmarkCountersByGroup[j]--;
        }
        m_Lines.erase(line, count);
        PUSH_IN_UNDO_STACK(new UndoDeleteLines(line, lines));

		Notify_ChangedLinesQuantity(max(0, line - 1), -count);
        Notify_ChangedLines(line, (int)m_Lines.size());
	}
}

void Storage::DeleteLinePart (int line, int from_pos, int to_pos)
{
    _ASSERTE((to_pos - from_pos) >= 0);

	if (line < static_cast<int>(m_Lines.size())
    && from_pos < static_cast<int>(m_Lines[line].length())
    && (to_pos - from_pos) >= 0)
	{
        m_actionSeq++;

        QuoteBalanceChecker chk;
        chk.ScanBeforeUpdate(this, line);

        String str = m_Lines[line];

        if (to_pos > static_cast<int>(str.length()))
            to_pos = (int)str.length();

        m_Lines[line].erase(from_pos, to_pos - from_pos);

        PUSH_IN_UNDO_STACK(new UndoDelete(line, from_pos, str.data() + from_pos, to_pos - from_pos));

        chk.NotifyAboutChanged(this, line);
    }
}

void Storage::SplitLine (int line, int pos)
{
    _ASSERTE(line < static_cast<int>(m_Lines.size()));

    if (line < static_cast<int>(m_Lines.size()))
    {
        m_actionSeq++;

        FixedString str = m_Lines.at(line);

        if (static_cast<int>(str.length()) < pos)
            pos = (int)str.length();

        String buff;
        buff.tag.id = m_lineSeq.GetNextVal();

        if (!pos) 
        {
            m_Lines.insert(buff, line);
        }
        else
        {
            if (pos < static_cast<int>(str.length()))
                buff.assign(str.data() + pos, str.length() - pos);

            m_Lines.insert(buff, line + 1);
            m_Lines.at(line).erase(pos);
        }

        PUSH_IN_UNDO_STACK(new UndoSplitLine(line, pos));

        Notify_ChangedLinesQuantity(line, 1);
        Notify_ChangedLines(line, (int)m_Lines.size());
    }
}

// ALTERNATIVE BOOKMARKS HANDLING:
// the result line has to have bookmark if at least one of lines has it
// the first line has higher priority if both lines have random bookmarks
// the result line should get the second line id if the first line empty and
// its id less then second line id - OR - the second line has ERROR MARK!
void Storage::MergeLines (int line)
{
	if (line + 1 < static_cast<int>(m_Lines.size()))
	{
        m_actionSeq++;

        int size = (int)m_Lines[line].length();
        m_Lines[line].append(m_Lines[line + 1]);
        String::Tag tag = m_Lines[line + 1].tag;
        m_Lines.erase(line + 1);
        PUSH_IN_UNDO_STACK(new UndoMergeLine(line, size));

        if (tag.bmkmask)
        {
            if (!size && !m_Lines[line].tag.bmkmask)
                m_Lines[line].tag = tag;
            else
                for (int i(0); i < BOOKMARK_GROUPS_SIZE; i++)
                    if (tag.bmkmask & (1 << i))
                        m_BookmarkCountersByGroup[i]--;
        }

        Notify_ChangedLinesQuantity(line, -1);
        Notify_ChangedLines(line, (int)m_Lines.size());
   }
}

void Storage::ExpandLinesTo (int line)
{
    if (line >= static_cast<int>(m_Lines.size()))
    {
        m_actionSeq++;

        int nlines = GetLineCount();
        Notify_ChangedLines(nlines, nlines);
        m_Lines.expand(line + 1);

        StringArray lines(line + 1 - nlines);
        lines.expand(line + 1 - nlines);
        PUSH_IN_UNDO_STACK(new UndoInsertLines(nlines, lines));

        Notify_ChangedLines(nlines, GetLineCount());
    }
}

void Storage::SetSettings (const Settings* settings)
{
    _CHECK_AND_THROW_(settings, "Invalid initialization!");

    if (m_pSettings)
        m_pSettings->RemoveSubscriber(this);

    if (settings)
        settings->AddSubscriber(this);

    m_pSettings = settings;

    OnSettingsChanged();
}

void Storage::SetSearcher (Searcher* searcher)
{
    _CHECK_AND_THROW_(searcher, "Invalid initialization!");

    if (m_pSearcher)
        m_pSearcher->RemoveRef(this);

    if (searcher)
        searcher->AddRef(this);

    m_pSearcher = searcher;
}

void Storage::SetBookmark (int line, EBookmarkGroup group, bool on)
{
    _CHECK_AND_THROW_(group < BOOKMARK_GROUPS_SIZE, "Invalid value for bookmark group!");

    m_actionSeq++;

    unsigned int oldState = m_Lines.at(line).tag.bmkmask;

    if (on)
      m_Lines[line].tag.bmkmask |= 1 << group;
    else
      m_Lines[line].tag.bmkmask &= ~(1 << group);

    if (oldState != m_Lines[line].tag.bmkmask)
    {
        m_BookmarkCountersByGroup[group] += on ? 1 : -1;
        _ASSERTE(m_BookmarkCountersByGroup[group] >= 0);
        Notify_ChangedLines(line, line);
    }
}

void Storage::RemoveAllBookmark (EBookmarkGroup group)
{
    _CHECK_AND_THROW_(group < BOOKMARK_GROUPS_SIZE, "Invalid value for bookmark group!");

    m_actionSeq++;

    unsigned mask = 1 << group;

    NotificationDisabler disabler(this);
    int first = -1;
    int last  = -1;
    for (int line(0), nlines((int)m_Lines.size()); line < nlines; line++)
        if (m_Lines[line].tag.bmkmask & mask)
        {
            m_Lines[line].tag.bmkmask &= ~mask;
            if (first == -1) first = line;
            last = line;
        }

   m_BookmarkCountersByGroup[group] = 0;

   disabler.Enable();
   Notify_ChangedLines(first, last); // actually we need invalidate only line state :(
}

int Storage::GetBookmarkNumber (EBookmarkGroup group)
{
    _CHECK_AND_THROW_(group < BOOKMARK_GROUPS_SIZE, "Invalid value for bookmark group!");

    return m_BookmarkCountersByGroup[group];
}

bool Storage::NextBookmark (int& _line, EBookmarkGroup group) const
{
    _CHECK_AND_THROW_(group < BOOKMARK_GROUPS_SIZE, "Invalid value for bookmark group!");

    unsigned mask = 1 << group;

    if (_line + 1 < static_cast<int>(m_Lines.size()))
    {
        for (int line = _line + 1, nlines = (int)m_Lines.size(); line < nlines; line++)
            if (m_Lines[line].tag.bmkmask & mask)
            {
                _line = line;
                return true;
            }
    }

    for (int line = 0, nlines = _line; line < nlines; line++)
        if (m_Lines[line].tag.bmkmask & mask)
        {
            _line = line;
            return true;
        }

    return false;
}

bool Storage::PreviousBookmark (int& _line, EBookmarkGroup group) const
{
    _CHECK_AND_THROW_(group < BOOKMARK_GROUPS_SIZE, "Invalid value for bookmark group!");

    unsigned mask = 1 << group;

    int line = min((unsigned)_line, (unsigned)m_Lines.size())-1;

    for (; line > 0; line--)
        if (m_Lines[line].tag.bmkmask & mask)
        {
            _line = line;
            return true;
        }

    line = (int)m_Lines.size() - 1;
    int end = _line;

    for (; line > end; line--)
        if (m_Lines[line].tag.bmkmask & mask)
        {
            _line = line;
            return true;
        }

    return false;
}

void Storage::GetBookmarkedLines (std::vector<int>& lines, EBookmarkGroup group) const
{
    _CHECK_AND_THROW_(group < BOOKMARK_GROUPS_SIZE, "Invalid value for bookmark group!");

    lines.empty();

    unsigned mask = 1 << group;

    for (int line = 0, nlines = (int)m_Lines.size(); line < nlines; line++)
        if (m_Lines[line].tag.bmkmask & mask)
            lines.push_back(line);
}

void Storage::SetRandomBookmark (RandomBookmark bookmark, int line, bool on)
{
    _CHECK_AND_THROW_(bookmark.Valid(), "Invalid id for random bookmark!");
    SetBookmark(line, eRandomBmkGroup, on);
    m_Lines[line].tag.bookmark = bookmark.GetId();
}

bool Storage::GetRandomBookmark (RandomBookmark bookmark, int& _line) const
{
    _CHECK_AND_THROW_(bookmark.Valid(), "Invalid id for random bookmark!");

    // update cache if it's necessary
    GetRandomBookmarks();

    int line = m_RandomBookmarks.GetLine(bookmark);

    if (line != -1)
    {
        _line = line;
        return true;
    }

    return false;
}

// the vector will be valid untill actionNumber is the same
const RandomBookmarkArray& Storage::GetRandomBookmarks () const
{
    if (m_RandomBookmarks.GetActionId() != m_actionSeq)
    {
        m_RandomBookmarks.Reset();

        for (int line = 0, nlines = (int)m_Lines.size(); line < nlines; line++)
            if (m_Lines[line].tag.bmkmask & 1 << eRandomBmkGroup)
                m_RandomBookmarks.SetLine(RandomBookmark(m_Lines[line].tag.bookmark), line);

        m_RandomBookmarks.SetActionNumber(m_actionSeq);
    }
    return m_RandomBookmarks;
}

bool Storage::FindById (LineId id, int& _line) const
{
    for (int line = 0, nlines = (int)m_Lines.size(); line < nlines; line++)
        if (m_Lines[line].tag.id == id.GetId())
        {
            _line = line;
            return true;
        }

    return false;
}

// Memory usage reporting
void Storage::GetMemoryUsage (unsigned& usage, unsigned& allocated, unsigned& undo) const
{
    usage     = 0;
    allocated = 0;
    undo      = 0;

    // calculate text buffer memory usage
    {
        for (int line = 0, nlines = (int)m_Lines.size(); line < nlines; line++)
        {
            usage     += (int)m_Lines[line].length();
            allocated += (m_Lines[line].capacity() ? (unsigned)m_Lines[line].capacity() : (unsigned)m_Lines[line].length()) + sizeof(String);
        }
    }
    // calculate undo memory usage
    undo = m_UndoStack.GetMemUsage();
}

// for background processing
bool Storage::OnIdle ()
{
    return m_languageSupport.get() ? m_languageSupport->OnIdle() : false;
}

};
