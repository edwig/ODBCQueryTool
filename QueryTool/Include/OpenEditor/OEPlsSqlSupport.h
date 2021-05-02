/* 
    Copyright (C) 2003 Aleksey Kochetov

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

#ifndef __OEPlsSqlSupport_h__
#define __OEPlsSqlSupport_h__

#include "OpenEditor/OELanguageSupport.h"
#include "OpenEditor/OEPlsSqlParser.h"
#include "OpenEditor/OEPlsSqlSyntax.h"
#include "OpenEditor/OEStorage.h"


namespace OpenEditor
{
    // interface for language support
    class PlSqlSupport : public LanguageSupport
    {
    public:
        PlSqlSupport (Storage*);

    // Services:
        virtual bool FindMatch (int line, int offset, Match&);

        // for background processing
        virtual bool OnIdle ();

    protected:
        bool processText (int);

        bool m_done;
        SequenceId m_actionId;
        int m_currLine, m_batchLimit;

        PlsSqlAnalyzer m_analyzer;
        PlsSqlParser m_parser;
    };

};

#endif//__OEPlsSqlSupport_h__
