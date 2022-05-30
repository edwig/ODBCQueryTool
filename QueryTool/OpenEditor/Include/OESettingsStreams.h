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

#ifndef __OESettingsStreams_h__
#define __OESettingsStreams_h__

#include "OEStreams.h"
#include "OESettings.h"
#include "OELanguage.h"


namespace OpenEditor
{
    using Common::VisualAttribute;
    using Common::VisualAttributesSet;

    ///////////////////////////////////////////////////////////////////////////
    // SettingsManager Reader/Writer
    ///////////////////////////////////////////////////////////////////////////

    class SettingsManagerReader
    {
    public:
        SettingsManagerReader (InStream& in) : m_in(in) {}
        virtual ~SettingsManagerReader () {}

        void operator >> (SettingsManager&);

    protected:
        void read (GlobalSettings&);
        void read (ClassSettings&);
        void read (VisualAttributesSet&);
        void read (VisualAttribute&);

        InStream& m_in;
        int       m_version;
    };


    class SettingsManagerWriter
    {
    public:
        SettingsManagerWriter (OutStream& out) : m_out(out) {}
        virtual ~SettingsManagerWriter () {}

        void operator << (const SettingsManager&);

    protected:
        void write (const GlobalSettings&);
        void write (const ClassSettings&);
        void write (const VisualAttributesSet&);
        void write (const VisualAttribute&);

        OutStream& m_out;
    };

};//namespace OpenEditor


#endif//__OESettingsStreams_h__

