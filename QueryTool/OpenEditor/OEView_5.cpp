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
    04/08/2002 bug fix, print highlighting fails on next page in word wrapping mode (xml highlighter)
    17/08/2002 bug fix, wrong print highlighting after print preview
*/

#include "stdafx.h"
#include "OpenEditor/OEDocument.h"
#include "OpenEditor/OEView.h"
#include "OpenEditor/OEHighlighter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OpenEditor;


    /////////////////////////////////////////////////////////////////////////////
    // Print helpers

    class PrintHeader
    {
    public:
        enum EPart { epLeft = 0, epCenter = 1, epRight = 2 };

        void SetShape (const char*);
        void SetSubstVariables (int pages, const char* file);
        void GetHeaderPart (string& dest, EPart part, int page, int width);
        bool IsEmpty () const;

    private:
        void substitute (const string& shape, string& dest, bool& comleted);

        typedef std::map<char,string>::value_type MapValueType;
        typedef std::map<char,string>::const_iterator ConstIterator;

        map<char,string> m_substMap;

        struct Part
        {
            string   text;
            bool comleted;

            Part () { comleted = false; }
        }
        m_part[3];
    };


    struct PrintContext
    {
        struct Ruler
        {
            int m_Count,         // number of visible lines or columns
                m_CharSize,      // size of character
                m_ClientSize,    // size of client area
                m_Indent;

            // screen position to pixel coordinate (inline)
            int operator ()(int inx) const { return inx * m_CharSize + m_Indent; }
        }
        m_Rulers[2];

        CFont m_Fonts[8]; // bold 0x01, italic 0x02, underline 0x04
        int   m_FontCharacterExtra[8];

        int m_cacheLine, // it's different from a printed line so it's true text line
            m_cachePage,
            m_cacheWordWrapOffset;

        PrintHeader m_header, m_footer;
        bool m_hasHeader, m_hasFooter;
        bool m_blackAndWhite;
        counted_ptr<OpenEditor::HighlighterBase> m_highlighter;

        PrintContext ()
        {
            m_blackAndWhite = true;

            memset(m_Rulers, 0, sizeof(m_Rulers));

            m_cacheLine =
            m_cachePage =
            m_cacheWordWrapOffset = 0;
            m_hasHeader = m_hasFooter = false;
        }

        ~PrintContext ()
        {
        }
    };


/////////////////////////////////////////////////////////////////////////////
// COEditorView printing

BOOL COEditorView::OnPreparePrinting (CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

    inline void HIMETRICtoLP(CDC* pDC, CPoint& pt)
    {
        CSize sz(pt.x, pt.y);
        pDC->HIMETRICtoLP(&sz);
        pt.x = sz.cx; pt.y = sz.cy;
    }

void COEditorView::OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo)
{
    _ASSERTE(!pInfo->m_lpUserData);

//    const long logPixelsX     = pDC->GetDeviceCaps(LOGPIXELSX);
//    const long logPixelsY     = pDC->GetDeviceCaps(LOGPIXELSY);
    const long phsicalWidth   = pDC->GetDeviceCaps(PHYSICALWIDTH);
    const long phsicalHeight  = pDC->GetDeviceCaps(PHYSICALHEIGHT);
    const long phsicalOffsetX = pDC->GetDeviceCaps(PHYSICALOFFSETX);
    const long phsicalOffsetY = pDC->GetDeviceCaps(PHYSICALOFFSETY);
    const long horzRes        = pDC->GetDeviceCaps(HORZRES);
    const long vertRes        = pDC->GetDeviceCaps(VERTRES);

    CRect rcMargins;
    GetMargins(rcMargins);

    pDC->SetMapMode(MM_TEXT);
    HIMETRICtoLP(pDC, rcMargins.TopLeft());
    HIMETRICtoLP(pDC, rcMargins.BottomRight());

    rcMargins.left   = max(phsicalOffsetX, rcMargins.left);
    rcMargins.top    = max(phsicalOffsetY, rcMargins.top);
    rcMargins.right  = max(phsicalWidth  - horzRes - phsicalOffsetX, rcMargins.right);
    rcMargins.bottom = max(phsicalHeight - vertRes - phsicalOffsetY, rcMargins.bottom);

    pInfo->m_rectDraw.left   = rcMargins.left - phsicalOffsetX;
    pInfo->m_rectDraw.top    = rcMargins.top  - phsicalOffsetY;
    pInfo->m_rectDraw.right  = pInfo->m_rectDraw.left + phsicalWidth  - rcMargins.right  - rcMargins.left;
    pInfo->m_rectDraw.bottom = pInfo->m_rectDraw.top  + phsicalHeight - rcMargins.bottom - rcMargins.top;

	pDC->DPtoLP(&pInfo->m_rectDraw);

    const VisualAttributesSet& set = GetSettings().GetVisualAttributesSet();
    const VisualAttribute& textAttr = set.FindByName("Text");

    PrintContext& pc = *new PrintContext;
    pInfo->m_lpUserData = &pc;
    pc.m_blackAndWhite = GetSettings().GetPrintBlackAndWhite();

    CDC dcPrinter;
    dcPrinter.Attach(pDC->m_hAttribDC);

    TEXTMETRIC tm;
#ifdef ADJUST_FONT_BY_WIDEST
    int widest = 0;
#else
    int narrowest = INT_MAX;
#endif
    int highest = 0;

    int widths[8];

    LOGFONT logfont;
    memset(&logfont, 0, sizeof(logfont));

    logfont.lfHeight         = -10 * textAttr.m_FontSize;
    logfont.lfCharSet        = DEFAULT_CHARSET;
    logfont.lfOutPrecision   = CLIP_DEFAULT_PRECIS;
    logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    logfont.lfQuality        = DEFAULT_QUALITY;
    logfont.lfPitchAndFamily = FIXED_PITCH;
    strncpy(logfont.lfFaceName, textAttr.m_FontName.c_str(), LF_FACESIZE-1);

    for (int i = 0; i < 8; i++)
    {
        logfont.lfWeight    = (0x01 & i) ? FW_SEMIBOLD : FW_NORMAL;
        logfont.lfItalic    = (0x02 & i) ? 1 : 0;
        logfont.lfUnderline = (0x04 & i) ? 1 : 0;

        pc.m_Fonts[i].DeleteObject();
        pc.m_Fonts[i].CreatePointFontIndirect(&logfont, &dcPrinter);
        dcPrinter.SelectObject(pc.m_Fonts[i]);
        dcPrinter.GetTextMetrics(&tm);

        widths[i] = tm.tmAveCharWidth;

#ifdef ADJUST_FONT_BY_WIDEST
        if (widest < tm.tmAveCharWidth)
            widest = tm.tmAveCharWidth;
#else
        if (narrowest > tm.tmAveCharWidth)
            narrowest = tm.tmAveCharWidth;
#endif
        if (highest < tm.tmHeight + tm.tmExternalLeading)
            highest = tm.tmHeight + tm.tmExternalLeading;
    }
    dcPrinter.Detach();

#ifdef ADJUST_FONT_BY_WIDEST
    for (i = 0; i < 8; i++)
        pc.m_FontCharacterExtra[i] = widest - widths[i];

    pc.m_Rulers[0].m_CharSize = widest;
#else
    for (int i = 0; i < 8; i++)
        pc.m_FontCharacterExtra[i] = narrowest - widths[i];

    pc.m_Rulers[0].m_CharSize = narrowest;
#endif

    pc.m_Rulers[1].m_CharSize   = highest;
    pc.m_Rulers[0].m_Indent     = pInfo->m_rectDraw.left;
    pc.m_Rulers[1].m_Indent     = pInfo->m_rectDraw.top;
    pc.m_Rulers[0].m_ClientSize = pInfo->m_rectDraw.Width();
    pc.m_Rulers[1].m_ClientSize = pInfo->m_rectDraw.Height();
    pc.m_Rulers[0].m_Count      = pc.m_Rulers[0].m_ClientSize / pc.m_Rulers[0].m_CharSize;
    pc.m_Rulers[1].m_Count      = pc.m_Rulers[1].m_ClientSize / pc.m_Rulers[1].m_CharSize;

    // set header & footer text
    pc.m_header.SetShape(GetPrintHeader().c_str());
    pc.m_footer.SetShape(GetPrintFooter().c_str());

    // change page size because of header & footer
    if (!pc.m_header.IsEmpty())
    {
        pc.m_hasHeader = true;

        pc.m_Rulers[1].m_Indent += 2 * pc.m_Rulers[1].m_CharSize;
        pc.m_Rulers[1].m_Count  -= 2;
    }

    if (!pc.m_footer.IsEmpty())
    {
        pc.m_hasFooter = true;
        pc.m_Rulers[1].m_Count  -= 2;
    }
    // calculate the number of pages
    int pages;
    MoveToPage(pInfo, &pages);
    pInfo->SetMaxPage(pages);
    // substitute the number of pages, the file name and others
    pc.m_header.SetSubstVariables(pages, GetDocument()->GetTitle());
    pc.m_footer.SetSubstVariables(pages, GetDocument()->GetTitle());

    // 17/08/2002 bug fix, wrong print highlighting after print preview
    pc.m_highlighter = HighlighterFactory::CreateHighlighter(GetSettings().GetLanguage());
    if (pc.m_highlighter.get())
        pc.m_highlighter->Init(GetSettings().GetVisualAttributesSet());
}

void COEditorView::OnEndPrinting (CDC*, CPrintInfo* pInfo)
{
    delete (PrintContext*)pInfo->m_lpUserData;
    pInfo->m_lpUserData = 0;
}

void COEditorView::OnPrepareDC (CDC*, CPrintInfo* pInfo)
{
    PrintContext& pc = *(PrintContext*)pInfo->m_lpUserData;

    if (static_cast<int>(pInfo->m_nCurPage) != pc.m_cachePage)
        MoveToPage(pInfo);

    if (pc.m_cacheLine == GetLineCount())
        pInfo->m_bContinuePrinting = FALSE;
}

void COEditorView::OnPrint (CDC* pDC, CPrintInfo* pInfo)
{
    _ASSERTE(pInfo->m_lpUserData);

    if (!m_bAttached) return;

    PrintContext& pc = *(PrintContext*)pInfo->m_lpUserData;

    // for test purpose only
    if (pInfo->m_bPreview)
    {
        static CPen grayPen(PS_SOLID, 1, RGB(127, 127, 127));
        CPen* pOrgPen = pDC->SelectObject(&grayPen);
        CRect rect(pc.m_Rulers[0](0), pc.m_Rulers[1](0),
                   pc.m_Rulers[0](pc.m_Rulers[0].m_Count),
                   pc.m_Rulers[1](pc.m_Rulers[1].m_Count));
        rect.InflateRect(2,2);
        pDC->Rectangle(rect);
        pDC->SelectObject(pOrgPen);
    }

    if (static_cast<int>(pInfo->m_nCurPage) != pc.m_cachePage)
        MoveToPage(pInfo);

    HighlighterBase* phighlighter = m_bSyntaxHighlight ? pc.m_highlighter.get() : 0;

    LineTokenizer tokenizer(
        false,
        GetTabSpacing(), 
        phighlighter ? phighlighter->GetDelimiters() : GetDelimiters()
        );

    if (phighlighter)
    {
        int status(0), quoteId(0);
        bool parsing(false);
        ScanMultilineQuotes(pc.m_cacheLine, status, quoteId, parsing);
        phighlighter->SetMultilineQuotesState(status, quoteId, parsing);
    }

    CFont* pOldFont = pDC->SelectObject(&pc.m_Fonts[m_paintAccessories->m_TextFont]);
    pDC->SetTextCharacterExtra(pc.m_FontCharacterExtra[m_paintAccessories->m_TextFont]);
    pDC->SetBkMode(TRANSPARENT);

    int lineCount = GetLineCount();

    for (int pageLine(0);
        pageLine < pc.m_Rulers[1].m_Count && pc.m_cacheLine < lineCount;
        pageLine++, pc.m_cacheLine++)
    {
        const char* currentLine; 
        int currentLineLength;
        GetLine(pc.m_cacheLine, currentLine, currentLineLength);
        tokenizer.StartScan(currentLine, currentLineLength);

        if (phighlighter)
        {
            phighlighter->NextLine(currentLine, currentLineLength);
            tokenizer.EnableProcessSpaces(phighlighter->IsSpacePrintable());
        }

        while (!tokenizer.Eol())
        {
            int pos, len;
            const char* str;

            tokenizer.GetCurentWord(str, pos, len);

            // 04/08/2002 bug fix, print highlighting fails on next page in word wrapping mode (xml highlighter)
            if (phighlighter)
            {
                phighlighter->NextWord(str, len);
                tokenizer.EnableProcessSpaces(phighlighter->IsSpacePrintable());
            }

            if (pos >= pc.m_cacheWordWrapOffset)
            {
                if (pos + len - pc.m_cacheWordWrapOffset >= pc.m_Rulers[0].m_Count) // wrap word
                {
                    pc.m_cacheWordWrapOffset = pos;     // save a word offset in the current line

                    if (++pageLine == pc.m_Rulers[1].m_Count)  // is next page?
                        break;
                }

                if (phighlighter)
                {
                    pDC->SelectObject(&pc.m_Fonts[phighlighter->GetFontIndex()]);
                    pDC->SetTextCharacterExtra(pc.m_FontCharacterExtra[phighlighter->GetFontIndex()]);
                    if (!pc.m_blackAndWhite) pDC->SetTextColor(phighlighter->GetTextColor());
                }

                pDC->TextOut(pc.m_Rulers[0](pos - pc.m_cacheWordWrapOffset),
                             pc.m_Rulers[1](pageLine), str, len);
            }

            tokenizer.Next();
        }

        if (pageLine == pc.m_Rulers[1].m_Count)  // is next page?
            break;
        else
            pc.m_cacheWordWrapOffset = 0;
    }

    // prepare DC for headers printing
    pDC->SelectObject(&pc.m_Fonts[m_paintAccessories->m_TextFont]);   // use default font
    pDC->SetTextCharacterExtra(pc.m_FontCharacterExtra[m_paintAccessories->m_TextFont]);
    if (!pc.m_blackAndWhite) pDC->SetTextColor(RGB(0,0,0)); // use default color - replace to constatnt !!!

    if (pc.m_hasHeader)
        PrintHeader(pDC, pInfo);
    if (pc.m_hasFooter)
        PrintFooter(pDC, pInfo);

    pc.m_cachePage++;

    pDC->SelectObject(pOldFont);
}

void COEditorView::MoveToPage (CPrintInfo* pInfo, int* pMaxPage)
{
    int orgCurPage = -1;
    // if pMaxPage is not null the call is page count request !!!
    if (pMaxPage)
    {
        orgCurPage = pInfo->m_nCurPage;
        pInfo->m_nCurPage = INT_MAX;
    }

    _ASSERTE(pInfo->m_lpUserData);

    if (!m_bAttached) return;

    PrintContext& pc = *(PrintContext*)pInfo->m_lpUserData;

    // reset cache data
    pc.m_cachePage = 1;
    pc.m_cacheLine =
    pc.m_cacheWordWrapOffset = 0;

    LineTokenizer tokenizer(false, GetTabSpacing(), GetDelimiters());

    int pageLine = 0;
    int lineCount = GetLineCount();

    while (pc.m_cacheLine < lineCount 
    && pc.m_cachePage < static_cast<int>(pInfo->m_nCurPage))
    {
        const char* currentLine; 
        int currentLineLength;
        GetLine(pc.m_cacheLine, currentLine, currentLineLength);
        tokenizer.StartScan(currentLine, currentLineLength);

        while (!tokenizer.Eol())
        {
            int pos, len;
            const char* str;

            tokenizer.GetCurentWord(str, pos, len);

            if (pos + len - pc.m_cacheWordWrapOffset >= pc.m_Rulers[0].m_Count) // wrap word
            {
                pc.m_cacheWordWrapOffset = pos;             // save a word offset in the current line

                if (++pageLine == pc.m_Rulers[1].m_Count)              // is next page?
                {
                    pageLine = 0;
                    if (++pc.m_cachePage == static_cast<int>(pInfo->m_nCurPage))  // have we done?
                        return;
                }
            }

            tokenizer.Next();
        }

        pc.m_cacheLine++;
        pc.m_cacheWordWrapOffset = 0;

        if (++pageLine == pc.m_Rulers[1].m_Count)              // is next page?
        {
            pageLine = 0;
            if (++pc.m_cachePage == static_cast<int>(pInfo->m_nCurPage))  // have we done?
                return;
        }
    }

    // it was a page count request
    if (pMaxPage)
    {
        *pMaxPage = (!pageLine && pc.m_cachePage > 1) ? pc.m_cachePage - 1 : pc.m_cachePage;
        _ASSERTE(orgCurPage != -1);
        pInfo->m_nCurPage = static_cast<UINT>(orgCurPage);
        pc.m_cacheLine = 0; // clean it because i check it for finishing
    }
}

void COEditorView::PrintHeader (CDC* pDC, CPrintInfo* pInfo)
{
    PrintContext& pc = *(PrintContext*)pInfo->m_lpUserData;

    if (pc.m_hasHeader)
    {
        string text;
        UINT align = pDC->GetTextAlign();

        pc.m_header.GetHeaderPart(text, PrintHeader::epLeft, pInfo->m_nCurPage, pc.m_Rulers[1].m_Count);
        pDC->SetTextAlign(TA_LEFT);
        pDC->TextOut(pc.m_Rulers[0](0), pc.m_Rulers[1](-2), text.c_str(), (int)text.size());

        pc.m_header.GetHeaderPart(text, PrintHeader::epCenter, pInfo->m_nCurPage, pc.m_Rulers[1].m_Count);
        pDC->SetTextAlign(TA_CENTER);
        pDC->TextOut(pc.m_Rulers[0](pc.m_Rulers[0].m_Count/2), pc.m_Rulers[1](-2), text.c_str(), (int)text.size());

        pc.m_header.GetHeaderPart(text, PrintHeader::epRight, pInfo->m_nCurPage, pc.m_Rulers[1].m_Count);
        pDC->SetTextAlign(TA_RIGHT);
        pDC->TextOut(pc.m_Rulers[0](pc.m_Rulers[0].m_Count), pc.m_Rulers[1](-2), text.c_str(), (int)text.size());

        pDC->SetTextAlign(align);
    }
}

void COEditorView::PrintFooter (CDC* pDC, CPrintInfo* pInfo)
{
    PrintContext& pc = *(PrintContext*)pInfo->m_lpUserData;

    if (pc.m_hasFooter)
    {
        string text;
        UINT align = pDC->GetTextAlign();

        pc.m_footer.GetHeaderPart(text, PrintHeader::epLeft, pInfo->m_nCurPage, pc.m_Rulers[1].m_Count);
        pDC->SetTextAlign(TA_LEFT);
        pDC->TextOut(pc.m_Rulers[0](0), pc.m_Rulers[1](pc.m_Rulers[1].m_Count+1), text.c_str(), (int)text.size());

        pc.m_footer.GetHeaderPart(text, PrintHeader::epCenter, pInfo->m_nCurPage, pc.m_Rulers[1].m_Count);
        pDC->SetTextAlign(TA_CENTER);
        pDC->TextOut(pc.m_Rulers[0](pc.m_Rulers[0].m_Count/2), pc.m_Rulers[1](pc.m_Rulers[1].m_Count+1), text.c_str(), (int)text.size());

        pc.m_footer.GetHeaderPart(text, PrintHeader::epRight, pInfo->m_nCurPage, pc.m_Rulers[1].m_Count);
        pDC->SetTextAlign(TA_RIGHT);
        pDC->TextOut(pc.m_Rulers[0](pc.m_Rulers[0].m_Count), pc.m_Rulers[1](pc.m_Rulers[1].m_Count+1), text.c_str(), (int)text.size());

        pDC->SetTextAlign(align);
    }
}

void COEditorView::GetMargins (CRect& rc) const
{
    if (!GetPrintMarginMeasurement()) // millimeters
    {
	    rc.left   = (LONG)(GetPrintLeftMargin()   * 100);
	    rc.right  = (LONG)(GetPrintRightMargin()  * 100);
	    rc.top    = (LONG)(GetPrintTopMargin()    * 100);
	    rc.bottom = (LONG)(GetPrintBottomMargin() * 100);
    }
    else // U.S. system
    {
	    rc.left   = (LONG)(GetPrintLeftMargin()   * 25.2 * 100);
	    rc.right  = (LONG)(GetPrintRightMargin()  * 25.2 * 100);
	    rc.top    = (LONG)(GetPrintTopMargin()    * 25.2 * 100);
	    rc.bottom = (LONG)(GetPrintBottomMargin() * 25.2 * 100);
    }
}

void PrintHeader::SetShape (const char* shape)
{
    // split an original shape to three part (left, center and right)
    const char* ptr = shape;

    for (int i = 0; *ptr && i < 3; ptr++)
    {
        if (*ptr == '&' && *(ptr+1) == 'b')
        {
            ptr++; i++;
        }
        else
        {
            m_part[i].text += *ptr;
        }
    }
}

void PrintHeader::SetSubstVariables (int pages, const char* file)
{
    char buffer[80];
    // the file name
    m_substMap['F'] = file;
    itoa(pages, buffer, 10);
    // the number of pages
    m_substMap['P'] = buffer;
    // local format time
    GetTimeFormat(LOCALE_USER_DEFAULT, 0, 0, 0, buffer, sizeof(buffer));
    m_substMap['t'] = buffer;
    // 24H format time
    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, 0, 0, buffer, sizeof(buffer));
    m_substMap['T'] = buffer;
    // short format date
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, 0, 0,  buffer, sizeof(buffer));
    m_substMap['d'] = buffer;
    // long format date
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, 0, 0,  buffer, sizeof(buffer));
    m_substMap['D'] = buffer;

    // pre-print substitution
    for (int i = 0; i < 3; i++)
        substitute(m_part[i].text, m_part[i].text, m_part[i].comleted);
}

bool PrintHeader::IsEmpty () const
{
    bool retVal = true;

    for (int i(0); i < 3; i++)
        if (m_part[i].text.size() > 0)
            retVal = false;

    return retVal;
}

void PrintHeader::GetHeaderPart (string& dest, EPart part, int page, int width)
{
    // count existing parts
    int divisor = 0;

    for (int i(0); i < 3; i++)
        if (m_part[i].text.size() > 0)
            divisor++;

    //  arrange sizes of existing header parts (left, center or right)
    if (divisor > 0)
    {
        unsigned int partWidth = width / divisor;

        if (partWidth > 0)
        {
            bool dummy;
            char pageStr[40];
            itoa(page, pageStr, 10);
            m_substMap['p'] = pageStr;

            // if non complite then substitute a page number
            if (!m_part[part].comleted)
                substitute(m_part[part].text, dest, dummy);
            else
                dest = m_part[part].text;

            if (dest.size() > partWidth)
            {
                dest.resize(partWidth - 3);
                dest.resize(partWidth, '.');

                if (m_part[part].comleted)
                    m_part[part].text = dest;
            }
        }
    }
}

void PrintHeader::substitute (const string& shape, string& dest, bool& comleted)
{
    string _dest;
    bool _comleted = true;

    for (const char* ptr = shape.c_str(); *ptr; ptr++)
    {
        if (*ptr == '&')
        {
            if (*(ptr+1) != '&')
            {
                ConstIterator it = m_substMap.find(*(ptr+1));

                if (it != m_substMap.end())
                {
                    ptr++;
                    _dest += it->second;
                }
                else
                {
                    _comleted = false;
                    _dest += *ptr;
                }
            }
            else
            {
                ptr++;
                _dest += *ptr;
            }
        }
        else
        {
            _dest += *ptr;
        }
    }

    dest = _dest;
    comleted = _comleted;
}

