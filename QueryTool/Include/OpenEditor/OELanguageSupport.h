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

#ifndef __OELanguageSupport_h__
#define __OELanguageSupport_h__

#include <common/arg_shared.h>
#include "OpenEditor/OEHelpers.h"


namespace OpenEditor
{
    class Storage;

    // interface for language support
    class LanguageSupport
    {
    public:
        LanguageSupport (Storage* pstorage) : m_pStorage(pstorage) {}
        virtual ~LanguageSupport () {}

    // Services:
        // line & offset are in/out parameters, length is out only
        struct Match 
        { 
            bool found, backward, partial, broken; 
            int line[2], offset[2], length[2]; 
            Match () { memset(this, 0, sizeof(*this)); }
        };

        virtual bool FindMatch (int line, int offset, Match&) = 0;

        // for background processing
        virtual bool OnIdle () { return false; };

    protected:
        Storage* m_pStorage;
    };

    typedef arg::counted_ptr<LanguageSupport> LanguageSupportPtr;


};//namespace OpenEditor


#endif//__OELanguageSupport_h__
