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
    16.03.2003 bug fix, keyword normalization fails in many cases
*/

#include "pch.h"
#include "OEView.h"
#include "OEHighlighter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

    using namespace std;
    using namespace OpenEditor;

bool COEditorView::PreNormalizeOnChar (NormalizeOnCharCxt& cxt, char ch)
{
    cxt.matched = false;

    if (GetSettings().GetNormalizeKeywords())
    {
        Position pos = GetPosition();
        HighlighterBase* phighlighter = &*m_highlighter;
        
        if (phighlighter 
        && IsSelectionEmpty()
        && (phighlighter->GetDelimiters()[ch])
        && pos.line < GetLineCount())
        {
            Position pos = GetPosition();
            const char* str;
            int len;
            GetLine(pos.line, str, len);
            pos.column = pos2inx(str, len, pos.column);
            int i;
            for (i = pos.column - 1; i >= 0 && !phighlighter->GetDelimiters()[str[i]]; i--)
                ;
            if (i != pos.column - 1)
            {
                cxt.sqr.start.line = cxt.sqr.end.line = pos.line;
                cxt.sqr.start.column = i + 1;
                cxt.sqr.end.column   = pos.column;

                if (phighlighter->IsKeyword(str + cxt.sqr.start.column, cxt.sqr.end.column - cxt.sqr.start.column, cxt.keyword)
                && strncmp(cxt.keyword.c_str(), str + cxt.sqr.start.column, cxt.sqr.end.column - cxt.sqr.start.column))
                {
                    cxt.matched = true;
                    cxt.sqr.start.column = inx2pos(str, len, cxt.sqr.start.column);
                    cxt.sqr.end.column   = inx2pos(str, len, cxt.sqr.end.column);
                }
            }
        }
    }

    return cxt.matched;
}

// it does not seem to cheap :(
void COEditorView::NormalizeOnChar (NormalizeOnCharCxt& cxt)
{
    if (cxt.matched
    && IsNormalizeablePos(cxt.sqr.start))
    {
        UndoGroup undo(*this);
        Position pos = GetPosition();
        PushInUndoStack(pos);

        SetSelection(cxt.sqr);
        DeleteBlock(false);

        MoveTo(cxt.sqr.start);
        InsertBlock(cxt.keyword.c_str(), true, false);

        MoveTo(pos);
        PushInUndoStack(pos);
    }
}

bool COEditorView::IsNormalizeablePos (Position pos) const
{
    HighlighterBase* phighlighter = m_bSyntaxHighlight ? m_highlighter.get() : 0;

    if (phighlighter)
    {
        bool parsing(false);
        int status(0), quoteId(0);
        ScanMultilineQuotes(pos.line, status, quoteId, parsing);
        phighlighter->SetMultilineQuotesState(status, quoteId, parsing);

        int len;
        const char* str; 
        GetLine(pos.line, str, len);
        phighlighter->NextLine(str, len);

        LineTokenizer tokenizer(false, GetTabSpacing(), phighlighter->GetDelimiters());
        tokenizer.EnableProcessSpaces(true);
        tokenizer.StartScan(str, len);

        while (!tokenizer.Eol())
        {
            int curPos, len;
            const char* str;

            tokenizer.GetCurentWord(str, curPos, len);
            phighlighter->NextWord(str, len);

            if (curPos >= pos.column && pos.column < curPos + len)
                return phighlighter->IsPlainText();

            tokenizer.Next();
        }
    }

    return false;
}

bool COEditorView::NormalizeText (const EditContext& cxt, string& _str)
{
    int line, start, end;
    cxt.GetScanLine(line, start, end);
    
    const COEditorView& view = static_cast<const COEditorView&>(cxt);
    HighlighterBase* phighlighter = view.m_bSyntaxHighlight ? view.m_highlighter.get() : 0;

    if (phighlighter)
    {
        bool parsing(false);
        int status(0), quoteId(0);
        cxt.ScanMultilineQuotes(line, status, quoteId, parsing);
        phighlighter->SetMultilineQuotesState(status, quoteId, parsing);

        int llen;
        const char* str; 
        cxt.GetLine(line, str, llen);
        phighlighter->NextLine(str, llen);

        LineTokenizer tokenizer(false, cxt.GetTabSpacing(), phighlighter->GetDelimiters());
        tokenizer.EnableProcessSpaces(true);
        tokenizer.StartScan(str, llen);

        string buffer, keyword;

        while (!tokenizer.Eol())
        {
            int pos, wlen;
            const char* str;

            tokenizer.GetCurentWord(str, pos, wlen);
            phighlighter->NextWord(str, wlen);

            // 16.03.2003 bug fix, keyword normalization fails in many cases
            if (((start <= pos && pos <= end) || (start <= (pos + wlen - 1) && (pos + wlen - 1) <= end))
            && phighlighter->IsPlainText() && phighlighter->IsKeyword(str, wlen, keyword))
            {
                _ASSERTE(wlen == static_cast<int>(keyword.length()));

                int inx = cxt.PosToInx(line, pos, llen) - start;

                for (int i(0); i < wlen; i++)
                    if (inx + i >= 0 && inx + i < end - start)
                        _str.at(inx + i) = keyword.at(i);
            }

            tokenizer.Next();
        }
    }

    return true;
}
