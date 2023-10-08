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
#include <COMMON/ExceptionHelper.h>
#include "OETemplateStreams.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OESMS_WRITE_MEMBER(o,p) m_out.write(#p, o.m_##p);
#define OESMS_READ_MEMBER(o,p)  m_in.read(#p, o.m_##p);
#define OESMS_VER_READ_MEMBER(ver,o,p,def)  \
    if (m_version >= ver) m_in.read(#p, o.m_##p); \
    else o.m_##p = def;

namespace OpenEditor
{
/////////////////////////////////////////////////////////////////////////////
// TemplateCollection Reader/Writter

const int TheTemplatesVersion = 1011;

void TemplateCollectionWriter::operator << (const SettingsManager& mgr)
{
    m_out.write(_T("Version"), TheTemplatesVersion);
    m_out.write(_T("Languages"), mgr.m_templateCollection.GetCount());

    TemplateCollection::ConstIterator 
        colIt = mgr.m_templateCollection.Begin(),
        colEnd = mgr.m_templateCollection.End();

    Stream::Section sect1(m_out, _T("Lng"));
    for (int i = 0; colIt != colEnd; colIt++, i++)
    {
        Stream::Section sect2(m_out, i);

        m_out.write(_T("Name"), colIt->first);

        Template::ConstIterator 
            templIt = colIt->second->Begin(),
            templEnd = colIt->second->End();

        m_out.write(_T("AlwaysListIfAlternative"), 
            colIt->second->GetAlwaysListIfAlternative());

        m_out.write(_T("Count"), colIt->second->GetCount());

        Stream::Section sect3(m_out, _T("Tmpl"));
        for (int j = 0; templIt != templEnd; templIt++, j++)
        {
            Stream::Section sect4(m_out, j);
            m_out.write(_T("Name"),      templIt->name);
            m_out.write(_T("Keyword"),   templIt->keyword);
            m_out.write(_T("MinLength"), templIt->minLength);
            m_out.write(_T("CurLine"),   templIt->curLine);
            m_out.write(_T("CurPos"),    templIt->curPos);
            m_out.write(_T("Text"),      templIt->text);
        }
    }
}

void TemplateCollectionReader::operator >> (SettingsManager& mgr)
{
    m_version = 0;
    m_in.read(_T("Version"), m_version);

    _CHECK_AND_THROW_(m_version >= 1010 && m_version <= TheTemplatesVersion, 
                      _T("Unsupprorted templates version!"));

    int langCount = 0;
    m_in.read(_T("Languages"), langCount);

    mgr.m_templateCollection.Clear();

    Stream::Section sect1(m_in, _T("Lng"));
    for (int i = 0; i < langCount; i++)
    {
        Stream::Section sect2(m_in, i);

        CString language;
        m_in.read(_T("Name"), language);
        TemplatePtr templ = mgr.m_templateCollection.Add(language);
        
        bool ListIfAlternative = false;
        if (m_version >= 1011) m_in.read(_T("AlwaysListIfAlternative"), ListIfAlternative);
        templ->SetAlwaysListIfAlternative(ListIfAlternative);

        int templCount = 0;
        m_in.read(_T("Count"), templCount);

        Stream::Section sect3(m_in, _T("Tmpl"));
        for (int j = 0; j < templCount; j++)
        {
            Stream::Section sect4(m_in, j);
            Template::Entry entry;
            m_in.read(_T("Name"),      entry.name);
            m_in.read(_T("Keyword"),   entry.keyword);
            m_in.read(_T("MinLength"), entry.minLength);
            m_in.read(_T("CurLine"),   entry.curLine);
            m_in.read(_T("CurPos"),    entry.curPos);
            m_in.read(_T("Text"),      entry.text);
            templ->AppendEntry(entry);
        }

    }
}

};//namespace OpenEditor
