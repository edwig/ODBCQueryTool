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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OETemplates_h__
#define __OETemplates_h__

#include <string>
#include <vector>
#include <map>
#include <common/arg_shared.h>
#include "OEHelpers.h"

namespace OpenEditor
{
    using std::vector;
    using std::map;
    using arg::counted_ptr;

    class Template;
    typedef arg::counted_ptr<Template> TemplatePtr;

    class Template 
    {
    public:
        Template ();

        struct Entry
        {
            CString name;
            CString keyword;
            CString text;
            int    minLength;
            int    curLine, curPos;

            Entry () { minLength = curLine = curPos = 0; }
        };

        bool ExpandKeyword (int index, CString& text, Position& pos);
        bool ExpandKeyword (const CString& keyword, CString& text, Position& pos);
        
        typedef std::vector<Entry>::const_iterator ConstIterator;
        ConstIterator Begin () const;
        ConstIterator End () const;
        int GetCount () const;
        const Entry& GetEntry (int) const;

        void AppendEntry (const Entry&);
        void InsertEntry (int pos, const Entry&);
        void UpdateEntry (int pos, const Entry&);
        void DeleteEntry (int pos);
        bool MoveUp (int pos);
        bool MoveDown (int pos);

        // for template development only
        void _sortByName ();

        bool GetAlwaysListIfAlternative () const;
        void SetAlwaysListIfAlternative (bool);
    private:
        bool m_AlwaysListIfAlternative;
        vector<Entry> m_entries;
    };

    
    
    class TemplateCollection
    {
    public:
        TemplateCollection () {};
        TemplateCollection (const TemplateCollection&);
        TemplateCollection& operator = (const TemplateCollection&);

        TemplatePtr Add (const CString& lang);
        const TemplatePtr Find (const CString& lang) const;

        typedef std::map<CString, TemplatePtr>::const_iterator ConstIterator;
        ConstIterator Begin () const;
        ConstIterator End () const;
        int GetCount () const;
        void Clear ();

    private:
        map<CString, TemplatePtr> m_templates;
    };

    
    inline
    Template::Template () 
        { m_AlwaysListIfAlternative = false; }

    inline
    const Template::Entry& Template::GetEntry (int inx) const
        { return m_entries.at(inx); }

    inline
    Template::ConstIterator Template::Begin () const
        { return m_entries.begin(); }

    inline
    Template::ConstIterator Template::End () const
        { return m_entries.end(); }
    
    inline
    int Template::GetCount () const
        { return (int)m_entries.size(); }

    inline
    bool Template::GetAlwaysListIfAlternative () const
        { return m_AlwaysListIfAlternative; }

    inline
    void Template::SetAlwaysListIfAlternative (bool enable)
        { m_AlwaysListIfAlternative = enable; }

    inline
    TemplateCollection::ConstIterator TemplateCollection::Begin () const
        { return m_templates.begin(); }

    inline
    TemplateCollection::ConstIterator TemplateCollection::End () const
        { return m_templates.end(); }
    
    inline
    int TemplateCollection::GetCount () const
        { return (int)m_templates.size(); }

    inline
    void TemplateCollection::Clear ()
        { m_templates.clear(); }
};

#endif//__OETemplates_h__
