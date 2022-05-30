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

#ifndef __OELanguageStreams_h__
#define __OELanguageStreams_h__

#include "OEStreams.h"
#include "OELanguage.h"


namespace OpenEditor
{
    ///////////////////////////////////////////////////////////////////////////
    // LanguagesCollection Reader/Writter
    ///////////////////////////////////////////////////////////////////////////

    class LanguagesCollectionReader
    {
    public:
        LanguagesCollectionReader (InStream& in) : m_in(in) {}
        virtual ~LanguagesCollectionReader () {}

        void operator >> (LanguagesCollection&);

    protected:
        void read (Language&);
        void read (LanguageKeywordMapPtr&, vector<string>&, bool caseSensiteve);

        InStream& m_in;
        int       m_version;
    };


    class LanguagesCollectionWriter
    {
    public:
        LanguagesCollectionWriter (OutStream& out) : m_out(out) {}
        virtual ~LanguagesCollectionWriter () {}

        void operator << (const LanguagesCollection&);

    protected:
        void write (const Language&);
        void write (const LanguageKeywordMapPtr&, const vector<string>&);

        OutStream& m_out;
    };

};//namespace OpenEditor


#endif//__OELanguageStreams_h__

