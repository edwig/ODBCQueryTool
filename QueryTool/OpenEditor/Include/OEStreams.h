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

#ifndef __OEStreams_h__
#define __OEStreams_h__

#include <CString>
#include <vector>
#include <set>
#include <fstream>


namespace OpenEditor
{
    using std::vector;
    using std::set;
    using std::pair;

    ///////////////////////////////////////////////////////////////////////////
    //  The original idea was to create general streams for storing application 
    //  settings, which operate on text files as well as windows registry,
    //  but I dropped registry support...
    ///////////////////////////////////////////////////////////////////////////

    class Stream // just to hide StreamKey
    {
    public:
        // a stream key controller, 
        // a constructor adds a section to a key, a destructor cuts it
        class Section
        {
        public:
            Section (Stream&, int);
            Section (Stream&, const char* section);
            Section (Stream&, const CString& section);
            ~Section ();
        private:
            Stream& m_stream;
            int m_length;
        };

    protected:
        class StreamKey
        {
        public:
            StreamKey ();
            CString& Format (const char* property);
            CString& Format (const CString& property);
            
        private:
            friend class Section;
            int  append (const char* section); // it's invoked from Section only
            void cut    (int shift);           // it's invoked from ~Section only
            int m_length;
            CString m_key;
        };
        
        friend Section;
        StreamKey m_sectionKey;
    };

    class InStream : public Stream
    {
    public:
        virtual ~InStream () {}

        virtual void read  (const CString&, CString&,bool p_skip = false)        = 0;
        virtual void read  (const CString&, double&,bool p_skip = false)        = 0;
        virtual void read  (const CString&, long&,bool p_skip = false)          = 0;
        virtual void read  (const CString&, unsigned long&,bool p_skip = false) = 0;
        virtual void read  (const CString&, unsigned int&,bool p_skip = false)  = 0;
        virtual void read  (const CString&, int&,bool p_skip = false)           = 0;
        virtual void read  (const CString&, bool&,bool p_skip = false)          = 0;

        void read (const CString&, vector<CString>&);
        void read (const CString&, vector<vector<CString> >&);
        void read (const CString&, set<CString>&);
        void read (const CString&, pair<CString, CString>&);
    };


    class OutStream : public Stream
    {
    public:
        virtual void write (const CString&, const char*)    = 0;
        virtual void write (const CString&, const CString&)  = 0;
        virtual void write (const CString&, double)         = 0;
        virtual void write (const CString&, long)           = 0;
        virtual void write (const CString&, unsigned long)  = 0;
        virtual void write (const CString&, int)            = 0;
        virtual void write (const CString&, unsigned int)   = 0;
        virtual void write (const CString&, bool)           = 0;

        void write (const CString&, const vector<CString>&);
        void write (const CString&, const vector<vector<CString> >&);
        void write (const CString&, const set<CString>&);
        void write (const CString&, const pair<CString, CString>&);
    };


    class FileInStream : public InStream
    {
    public:
        FileInStream (const char* filename);

    public:
        virtual void read  (const CString&, CString&,bool p_skip = false);
        virtual void read  (const CString&, double&,bool p_skip = false);
        virtual void read  (const CString&, long&,bool p_skip = false);
        virtual void read  (const CString&, unsigned long&,bool p_skip = false);
        virtual void read  (const CString&, unsigned int&,bool p_skip = false);
        virtual void read  (const CString&, int&, bool p_skip = false);
        virtual void read  (const CString&, bool&,bool p_skip = false);

    private:
        void validateEntryName (const CString&, const CString&,bool* p_skip = nullptr);
        std::ifstream m_infile;
        CString       m_last;
    };


    class FileOutStream : public OutStream
    {
    public:
        FileOutStream (const char* filename);

    public:
        virtual void write (const CString&, const char*);
        virtual void write (const CString&, const CString&);
        virtual void write (const CString&, double);
        virtual void write (const CString&, long);
        virtual void write (const CString&, unsigned long);
        virtual void write (const CString&, int);
        virtual void write (const CString&, unsigned int);
        virtual void write (const CString&, bool);

    private:
        std::ofstream m_outfile;
    };


};//namespace OpenEditor


#endif//__OEStreams_h__

