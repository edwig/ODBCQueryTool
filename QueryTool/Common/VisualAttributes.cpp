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
#include <string>
#include "COMMON\VisualAttributes.h"
#include <stdexcept>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4073)
#pragma init_seg(lib)

namespace Common
{


///////////////////////////////////////////////////////////////////////////////
//  VisualAttribute
///////////////////////////////////////////////////////////////////////////////

VisualAttribute::VisualAttribute ()
{
    Clear();
}


VisualAttribute::VisualAttribute (const VisualAttribute& other)
{
    *this = other;
}

VisualAttribute::VisualAttribute (
    const char* name, 
    const char* fontName, int size,
    bool bold, bool italic, bool underline,
    COLORREF foreground, COLORREF background,
    unsigned mask
    )
{
    m_Name          = name;
    m_FontName      = fontName;
    m_FontSize      = size;
    m_FontBold      = bold;
    m_FontItalic    = italic;
    m_FontUnderline = underline;

    if (vaoForeground|mask)
        m_Foreground = (foreground != -1) ? foreground : GetSysColor(COLOR_WINDOWTEXT);
    if (vaoBackground|mask)
        m_Background = (background != -1) ? background : GetSysColor(COLOR_WINDOW);

    m_Mask = mask;
}


void VisualAttribute::Clear ()
{
    m_FontName.Empty();
    m_FontSize = 0;
    m_FontBold = m_FontItalic = m_FontUnderline = false;
    m_Foreground = m_Background = (UINT)-1;
    m_Mask = 0;
}


VisualAttribute& VisualAttribute::operator = (const VisualAttribute& other)
{
    m_Name          = other.m_Name;
    m_FontName      = other.m_FontName;
    m_FontSize      = other.m_FontSize;
    m_FontBold      = other.m_FontBold;
    m_FontItalic    = other.m_FontItalic;
    m_FontUnderline = other.m_FontUnderline;
    m_Foreground    = other.m_Foreground;
    m_Background    = other.m_Background;
    m_Mask          = other.m_Mask;
    return *this;
}


void VisualAttribute::Construct (VisualAttribute& dest, const VisualAttribute& def)
{
    dest.m_FontName      = (m_Mask & vaoFontName)      ? m_FontName      : def.m_FontName;
    dest.m_FontSize      = (m_Mask & vaoFontSize)      ? m_FontSize      : def.m_FontSize;
    dest.m_FontBold      = (m_Mask & vaoFontBold)      ? m_FontBold      : def.m_FontBold;
    dest.m_FontItalic    = (m_Mask & vaoFontItalic)    ? m_FontItalic    : def.m_FontItalic;
    dest.m_FontUnderline = (m_Mask & vaoFontUnderline) ? m_FontUnderline : def.m_FontUnderline;
    dest.m_Foreground    = (m_Mask & vaoForeground)    ? m_Foreground    : def.m_Foreground;
    dest.m_Background    = (m_Mask & vaoBackground)    ? m_Background    : def.m_Background;
    dest.m_Mask          = m_Mask | def.m_Mask;
}


void VisualAttribute::Construct (int count, COLORREF* dest, const COLORREF* src, const COLORREF* def)
{
    for (int i(0); i < count; i++)
        dest[i] = (src[i] != -1) ? src[i] : def[i];
}


CFont* VisualAttribute::NewFont () const
{
    CFont* font = new CFont;

    font->CreateFont(
          -PointToPixel(m_FontSize), 0, 0, 0,
          m_FontBold ? FW_SEMIBOLD : FW_NORMAL,
          m_FontItalic ? 1 : 0,
          m_FontUnderline ? 1 : 0,
          0, ANSI_CHARSET,//DEFAULT_CHARSET,
          OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
          m_FontName.GetString()
        );

    return font;
}


    static int g_cyPixelsPerInch = CWindowDC(CWnd::GetDesktopWindow()).GetDeviceCaps(LOGPIXELSY);


int VisualAttribute::PixelToPoint (int pixels)
{
    return MulDiv(pixels, 72, g_cyPixelsPerInch);
}


int VisualAttribute::PointToPixel (int points)
{
    return MulDiv(points, g_cyPixelsPerInch, 72);
}


///////////////////////////////////////////////////////////////////////////////
//  VisualAttributesSet
///////////////////////////////////////////////////////////////////////////////

VisualAttributesSet::VisualAttributesSet (const char* name, int size)
: m_name(name)
{
    m_attrs.reserve(size);
}

VisualAttributesSet& VisualAttributesSet::operator = (const VisualAttributesSet& other)
{
    m_name  = other.m_name;
    m_attrs = other.m_attrs;

    return *this;
}

const VisualAttribute& VisualAttributesSet::FindByName (const char* name) const
{
    _ASSERTE(name);

    if (name)
    {
        std::vector<VisualAttribute>::const_iterator
            it = m_attrs.begin(), end = m_attrs.end();

        for (; it != end; it++)
            if (it->m_Name == name)
                return *it;
    }

    throw std::out_of_range(CString("Visual attribute \"") + name + CString("\" not found in set \"") + m_name + CString("\"."));
}

}//namespace Common
