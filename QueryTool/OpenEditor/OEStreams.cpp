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

// 02.06.2003 bug fix, diagnostics improvement

#include "pch.h"
#include "COMMON/StrHelpers.h"
#include <COMMON/ExceptionHelper.h>
#include "OEStreams.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable: 4996)

namespace OpenEditor
{
    using std::hex;
    using std::dec;
    using std::endl;
    using std::getline;


Stream::Section::Section (Stream& stream, int section)
: m_stream(stream)
{
    char buff[40];
    m_length = m_stream.m_sectionKey.append(itoa(section, buff, 10));
}

Stream::Section::Section (Stream& stream, const char* section)
: m_stream(stream)
{
    m_length = m_stream.m_sectionKey.append(section);
}

Stream::Section::Section (Stream& stream, const string& section)
: m_stream(stream)
{
    m_length = m_stream.m_sectionKey.append(section.c_str());
}

Stream::Section::~Section ()
{
    m_stream.m_sectionKey.cut(m_length);
}

Stream::StreamKey::StreamKey ()
{
    m_length = 0;
}

string& Stream::StreamKey::Format (const char* property)
{
    m_key.resize(m_length);
    if (m_length) m_key += '.';
    m_key += property;
    return m_key;
}

string& Stream::StreamKey::Format (const string& property)
{
    return Format(property.c_str());
}

int Stream::StreamKey::append (const char* section)
{
    m_key.resize(m_length);
    int length = m_length;
    if (m_length) m_key += '.';
    m_key += section;
    m_length = (int)m_key.size();
    return length;
}

void Stream::StreamKey::cut (int len)
{
    _ASSERTE(len >= 0);
    m_length = len;
    m_key.resize(m_length);
}


/////////////////////////////////////////////////////////////////////////////
// 	InStream & OutStream

void OutStream::write (const string& section, const vector<string>& _vector)
{
    Section sect(*this, section);

    write("Count", (int)_vector.size());

    char buff[40];
    std::vector<string>::const_iterator it = _vector.begin();
    for (int i(0); it != _vector.end(); it++, i++)
        write(itoa(i, buff, 10), *it);
}

void OutStream::write (const string& section, const vector<vector<string> >& vec_of_vec)
{
    Section sect(*this, section);

    write("Count", (int)vec_of_vec.size());

    char buff[40];
    std::vector<vector<string> >::const_iterator it = vec_of_vec.begin();
    for (int i(0); it != vec_of_vec.end(); it++, i++)
        write(itoa(i, buff, 10), *it);
}

void InStream::read (const string& section, vector<string>& _vector)
{
    Section sect(*this, section);

    int count;
    read("count", count);
    _vector.resize(count);

    char buff[80];
    for (int i(0); i < count; i++)
        read(itoa(i, buff, 10), _vector.at(i));
}

void InStream::read (const string& section, vector<vector<string> >& vec_of_vec)
{
    Section sect(*this, section);

    int count;
    read("count", count);
    vec_of_vec.resize(count);

    char buff[80];
    for (int i(0); i < count; i++)
        read(itoa(i, buff, 10), vec_of_vec.at(i));
}

void OutStream::write (const string& section, const set<string>& _set)
{
    Section sect(*this, section);

    write("Count", (int)_set.size());
    
    char buff[80];
    std::set<string>::const_iterator it = _set.begin();
    for (int i(0); it != _set.end(); it++, i++)
        write(itoa(i, buff, 10), *it);
}

void InStream::read (const string& section, set<string>& _set)
{
    Section sect(*this, section);

    int count;
    read("Count", count);

    char buff[80];
    for (int i(0); i < count; i++)
    {
        string value;
        read(itoa(i, buff, 10), value);
        _set.insert(value);
    }
}

void OutStream::write (const string& section, const pair<string, string>& _pair)
{
    Section sect(*this, section);

    write("0", _pair.first);
    write("1", _pair.second);
}

void InStream::read (const string& section, pair<string, string>& _pair)
{
    Section sect(*this, section);

    read("0", _pair.first);
    read("1", _pair.second);
}


/////////////////////////////////////////////////////////////////////////////
// 	FileInStream & FileOutStream

// 02.06.2003 bug fix, diagnostics improvement
FileInStream::FileInStream (const char* filename) 
: m_infile(filename) 
{
    _CHECK_AND_THROW_(m_infile.good(), (string("Cannot open file \"") + filename + "\" for reading.").c_str());
}

FileOutStream::FileOutStream(const char* filename)
  : m_outfile(filename)
{
  _CHECK_AND_THROW_(m_outfile.good(),(string("Cannot open file \"") + filename + "\" for writing.").c_str());
}

void FileOutStream::write(const string& name,const char* _val)
{
  string val;
  Common::to_printable_str(_val,val);
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileOutStream::write(const string& name,const string& _val)
{
  string val;
  Common::to_printable_str(_val.c_str(),val);
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const string& name,string& val,bool p_skip /*= false*/)
{
  string _name,_val;

  if(!m_last.empty())
  {
    if(stricmp(name.c_str(),m_last.c_str()) != 0)
    {
      val.clear();
      return;
    }
  }
  else
  {
    getline(m_infile,_name,'=');
    validateEntryName(name,_name,&p_skip);
    if(p_skip)
    {
      m_last = _name;
      return;
    }
  }
  getline(m_infile,_val);
  _ASSERTE(m_infile.good());
  m_last.clear();
  Common::to_unprintable_str(_val.c_str(),val);
}


void FileOutStream::write(const string& name,double val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const string& name,double& val,bool p_skip /*= false*/)
{
  string _name;
  getline(m_infile,_name,'=');
  validateEntryName(name,_name,&p_skip);
  if(p_skip)
  {
    return;
  }
  m_infile >> val; m_infile.get();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write(const string& name,long val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const string& name,long& val,bool skip /*=false*/)
{
  string _name;
  getline(m_infile,_name,'=');
  validateEntryName(name,_name);
  m_infile >> val; m_infile.get();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write(const string& name,unsigned long val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << hex << val << dec << endl;
}


void FileInStream::read(const string& name,unsigned long& val,bool skip /*=false*/)
{
  string _name;
  getline(m_infile,_name,'=');
  validateEntryName(name,_name);
  m_infile.setf(std::ifstream::hex,std::ifstream::basefield);
  m_infile >> val;
  m_infile.setf(std::ifstream::dec,std::ifstream::basefield);
  m_infile.get();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write(const string& name,int val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const string& name,int& val,bool skip /*=false*/)
{
  _ASSERTE(m_infile.good());
  string _name;

  if(!m_last.empty())
  {
    if(stricmp(name.c_str(),m_last.c_str()) != 0)
    {
      val = 0;
      return;
    }
  }
  else
  {
    getline(m_infile,_name,'=');
    validateEntryName(name,_name,&skip);
    if(skip)
    {
      m_last = _name;
      return;
    }
  }
  m_infile >> val; 
  m_infile.get();
  m_last.clear();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write (const string& name, unsigned int val)
{
    m_outfile << m_sectionKey.Format(name) << '=' << hex << val << dec << endl;
}


void FileInStream::read  (const string& name, unsigned int& val,bool /*p_skip = false*/)
{
    string _name;
    getline(m_infile,_name,'=');
    validateEntryName(name,_name);
    m_infile.setf( std::ifstream::hex, std::ifstream::basefield);
    m_infile >> val;
    m_infile.setf( std::ifstream::dec, std::ifstream::basefield);
    m_infile.get();
    _ASSERTE(m_infile.good());
}


void FileOutStream::write (const string& name, bool val)
{
    m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}

void FileInStream::read  (const string& name, bool& val,bool p_skip /*= false*/)
{
    string _name;

    if(!m_last.empty())
    {
      if(stricmp(name.c_str(),m_last.c_str()) != 0)
      {
        val = false;
        return;
      }
    }
    else
    {
      getline(m_infile,_name,'=');
      validateEntryName(name,_name,&p_skip);
      if(p_skip)
      {
        m_last = _name;
        return;
      }
    }
    m_infile >> val; 
    m_infile.get();
    m_last.clear();
    _ASSERTE(m_infile.good());
}

void FileInStream::validateEntryName(const string& name, const string& entryName,bool* p_skip)
{
  if(stricmp(m_sectionKey.Format(name).c_str(), entryName.c_str()))
  {
    if(!p_skip)
    {
      throw std::exception("Unexpected entry in stream.");
    }
    return;
  }
  if(p_skip)
  {
    *p_skip = false;
  }
}

};//namespace OpenEditor
