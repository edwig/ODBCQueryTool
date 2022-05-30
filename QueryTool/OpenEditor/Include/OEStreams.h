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

#include <string>
#include <vector>
#include <set>
#include <fstream>


namespace OpenEditor
{
    using std::string;
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
            Section (Stream&, const string& section);
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
            string& Format (const char* property);
            string& Format (const string& property);
            
        private:
            friend class Section;
            int  append (const char* section); // it's invoked from Section only
            void cut    (int shift);           // it's invoked from ~Section only
            int m_length;
            string m_key;
        };
        
        friend Section;
        StreamKey m_sectionKey;
    };

    class InStream : public Stream
    {
    public:
        virtual ~InStream () {}

        virtual void read  (const string&, string&)        = 0;
        virtual void read  (const string&, double&)        = 0;
        virtual void read  (const string&, long&)          = 0;
        virtual void read  (const string&, unsigned long&) = 0;
        virtual void read  (const string&, unsigned int&)  = 0;
        virtual void read  (const string&, int&)           = 0;
        virtual void read  (const string&, bool&)          = 0;

        void read (const string&, vector<string>&);
        void read (const string&, vector<vector<string> >&);
        void read (const string&, set<string>&);
        void read (const string&, pair<string, string>&);
    };


    class OutStream : public Stream
    {
    public:
        virtual void write (const string&, const char*)    = 0;
        virtual void write (const string&, const string&)  = 0;
        virtual void write (const string&, double)         = 0;
        virtual void write (const string&, long)           = 0;
        virtual void write (const string&, unsigned long)  = 0;
        virtual void write (const string&, int)            = 0;
        virtual void write (const string&, unsigned int)   = 0;
        virtual void write (const string&, bool)           = 0;

        void write (const string&, const vector<string>&);
        void write (const string&, const vector<vector<string> >&);
        void write (const string&, const set<string>&);
        void write (const string&, const pair<string, string>&);
    };


    class FileInStream : public InStream
    {
    public:
        FileInStream (const char* filename);

    public:
        virtual void read  (const string&, string&);
        virtual void read  (const string&, double&);
        virtual void read  (const string&, long&);
        virtual void read  (const string&, unsigned long&);
        virtual void read  (const string&, unsigned int&);
        virtual void read  (const string&, int&);
        virtual void read  (const string&, bool&);

    private:
        void validateEntryName (const string&, const string&);
        std::ifstream m_infile;
    };


    class FileOutStream : public OutStream
    {
    public:
        FileOutStream (const char* filename);

    public:
        virtual void write (const string&, const char*);
        virtual void write (const string&, const string&);
        virtual void write (const string&, double);
        virtual void write (const string&, long);
        virtual void write (const string&, unsigned long);
        virtual void write (const string&, int);
        virtual void write (const string&, unsigned int);
        virtual void write (const string&, bool);

    private:
        std::ofstream m_outfile;
    };


};//namespace OpenEditor


#endif//__OEStreams_h__

