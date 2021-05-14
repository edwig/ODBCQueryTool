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

#include "pch.h"
#include "OEHighlighter.h"
#include "OEViewPaintAccessories.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


    using namespace std;
    using namespace Common;
    using namespace OpenEditor;

    typedef map<string, COEViewPaintAccessoriesPtr> COEViewPaintAccessoriesMap;
    
    static COEViewPaintAccessoriesMap g_PaintAccessoriesMap;


COEViewPaintAccessoriesPtr COEViewPaintAccessories::GetPaintAccessories (const std::string& category)
{
    COEViewPaintAccessoriesMap::iterator it = g_PaintAccessoriesMap.find(category);
    
    if (g_PaintAccessoriesMap.end() == it)
    {
        COEViewPaintAccessoriesPtr ptr(new COEViewPaintAccessories);

        g_PaintAccessoriesMap.insert(COEViewPaintAccessoriesMap::value_type(category, ptr));

        return ptr;
    }

    return it->second;
}

COEViewPaintAccessories::COEViewPaintAccessories ()
{ 
    memset(&m_FontCharacterExtra, 0, sizeof(m_FontCharacterExtra));
    m_TextFont = 0;
}

void COEViewPaintAccessories::OnSettingsChanged (CWnd* pwnd, const VisualAttributesSet& set_)
{
    const VisualAttribute& textAttr = set_.FindByName("Text");
    m_TextFont = (textAttr.m_FontBold ? HighlighterBase::efmBold : HighlighterBase::efmNormal)
            | (textAttr.m_FontItalic ? HighlighterBase::efmItalic : HighlighterBase::efmNormal)
            | (textAttr.m_FontUnderline ? HighlighterBase::efmUnderline : HighlighterBase::efmNormal);

    m_TextForeground = textAttr.m_Foreground;
    m_TextBrush.DeleteObject();
    m_TextBrush.CreateSolidBrush(textAttr.m_Background);

    const VisualAttribute& selTextAttr = set_.FindByName("Selected Text");
    m_SelTextForeground = selTextAttr.m_Foreground;
    m_SelTextBrush.DeleteObject();
    m_SelTextBrush.CreateSolidBrush(selTextAttr.m_Background);

    const VisualAttribute& curLinetAttr = set_.FindByName("Current Line");
    m_CurrentLineBrush.DeleteObject();
    m_CurrentLineBrush.CreateSolidBrush(curLinetAttr.m_Background);

    const VisualAttribute& bmkAttr = set_.FindByName("Bookmark");
    m_BmkBrush.DeleteObject();
    m_BmkBrush.CreateSolidBrush(bmkAttr.m_Background);
    m_BmkPen.DeleteObject();
    m_BmkPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));

    const VisualAttribute& rdmBmkAttr = set_.FindByName("Random Bookmark");
    m_RndBmkForeground = rdmBmkAttr.m_Foreground;
    m_RndBmkBrush.DeleteObject();
    m_RndBmkBrush.CreateSolidBrush(rdmBmkAttr.m_Background);
    m_RndBmkPen.DeleteObject();
    m_RndBmkPen.CreatePen(PS_SOLID, 1, rdmBmkAttr.m_Foreground);

    m_EOFForeground = set_.FindByName("End of File").m_Foreground;


    TEXTMETRIC tm;
    CClientDC dc(pwnd);

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
    strncpy_s(logfont.lfFaceName, textAttr.m_FontName.c_str(), LF_FACESIZE-1);


    for (int i = 0; i < 8; i++)
    {
        logfont.lfWeight    = (0x01 & i) ? FW_SEMIBOLD : FW_NORMAL;
        logfont.lfItalic    = (0x02 & i) ? 1 : 0;
        logfont.lfUnderline = (0x04 & i) ? 1 : 0;

        m_Fonts[i].DeleteObject();
        m_Fonts[i].CreatePointFontIndirect(&logfont, &dc);
        dc.SelectObject(m_Fonts[i]);
        dc.GetTextMetrics(&tm);

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

#ifdef ADJUST_FONT_BY_WIDEST
    for (i = 0; i < 8; i++)
        m_FontCharacterExtra[i] = widest - widths[i];

    m_CharSize.cx = widest;
#else
    for (int i = 0; i < 8; i++)
        m_FontCharacterExtra[i] = narrowest - widths[i];

    m_CharSize.cx = narrowest;
#endif

    m_CharSize.cy = highest;
}

