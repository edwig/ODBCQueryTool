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

#include "stdafx.h"
#include <algorithm>
#include <COMMON/ExceptionHelper.h>
#include "OpenEditor/OETemplates.h"
#include "OpenEditor/OESettingsStreams.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace OpenEditor
{

///////////////////////////////////////////////////////////////////////////////
// Template

bool Template::ExpandKeyword (int index, string& text, Position& pos)
{
    if (index >= 0 && index < static_cast<int>(m_entries.size()))
    {
        text        = m_entries[index].text;
        pos.line    = m_entries[index].curLine;
        pos.column  = m_entries[index].curPos;
        return true;
    }
    return false;
}

bool Template::ExpandKeyword (const string& keyword, string& text, Position& pos)
{
    std::vector<Entry>::const_iterator it = m_entries.begin();
    
    bool matched = false, alternative = false;

    for (; it != m_entries.end(); it++)
    {
        bool is_eql = !strncmp(it->keyword.c_str(), keyword.c_str(), keyword.size());

        if (is_eql && alternative 
        && m_AlwaysListIfAlternative) 
            return false;

        if (!matched && is_eql 
        &&  it->minLength <= static_cast<int>(keyword.size()))
        {
            matched = true;
            text = it->text;
            pos.line = it->curLine;
            pos.column = it->curPos;

            if (!m_AlwaysListIfAlternative)
                return true;
        }

        alternative |= is_eql;
    }

    return matched;
}

void Template::AppendEntry (const Entry& entry)
{
    m_entries.push_back(entry);
}

void Template::InsertEntry (int pos, const Entry& entry)
{
    m_entries.insert(m_entries.begin() + pos, entry);
}

void Template::UpdateEntry (int pos, const Entry& entry)
{
    m_entries[pos] = entry;
}

void Template::DeleteEntry (int pos)
{
    m_entries.erase(m_entries.begin() + pos);
}

bool Template::MoveUp (int pos)
{
    if (pos > 0)
    {
        std::swap(m_entries.at(pos), m_entries.at(pos-1));
        return true;
    }
    return false;
}

bool Template::MoveDown (int pos)
{
    if (pos < static_cast<int>(m_entries.size())-1)
    {
        std::swap(m_entries.at(pos), m_entries.at(pos+1));
        return true;
    }
    return false;
}
        
static int _least (const Template::Entry& e1, const Template::Entry& e2)
{
    return e1.name < e2.name;
}

// for template development only
void Template::_sortByName ()
{
    std::stable_sort(m_entries.begin(), m_entries.end(), _least);
}

///////////////////////////////////////////////////////////////////////////////
// TemplateCollection

TemplateCollection::TemplateCollection (const TemplateCollection& other)
{
    *this = other;
}

TemplateCollection& TemplateCollection::operator = (const TemplateCollection& other)
{
    if (&other != this)
    {
        m_templates.clear();

        std::map<string, TemplatePtr>::const_iterator
            it = other.m_templates.begin(),
            end = other.m_templates.end();

        for (it; it != end; it++)
        {
            *(Add(it->first)) = *(it->second);
        }

    }

    return *this;
}

TemplatePtr TemplateCollection::Add (const string& lang)
{
    TemplatePtr ptr(new Template);

    if (m_templates.find(lang) == m_templates.end())
        m_templates.insert(std::map<string, TemplatePtr>::value_type(lang, ptr));
    else
        throw std::logic_error(string("Template \"") + lang + "\" already exist.");

    return ptr;
}

const TemplatePtr TemplateCollection::Find (const string& lang) const
{
    ConstIterator it = m_templates.find(lang);

    if (it == m_templates.end())
        throw std::logic_error(string("Template \"") + lang + "\" not found.");

    return it->second;
}

};//namespace OpenEditor
