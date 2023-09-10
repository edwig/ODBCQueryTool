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

Stream::Section::Section (Stream& stream, const CString& section)
: m_stream(stream)
{
    m_length = m_stream.m_sectionKey.append(section.GetString());
}

Stream::Section::~Section ()
{
    m_stream.m_sectionKey.cut(m_length);
}

Stream::StreamKey::StreamKey ()
{
    m_length = 0;
}

CString& Stream::StreamKey::Format (const char* property)
{
  m_key.Truncate(m_length);
  if (m_length) m_key += '.';
  m_key += property;
  return m_key;
}

CString& Stream::StreamKey::Format (const CString& property)
{
    return Format(property.GetString());
}

int Stream::StreamKey::append (const char* section)
{
    m_key.Truncate(m_length);
    int length = m_length;
    if (m_length) m_key += '.';
    m_key += section;
    m_length = (int)m_key.GetLength();
    return length;
}

void Stream::StreamKey::cut (int len)
{
    _ASSERTE(len >= 0);
    m_length = len;
    m_key.Truncate(m_length);
}


/////////////////////////////////////////////////////////////////////////////
// 	InStream & OutStream

void OutStream::write (const CString& section, const vector<CString>& _vector)
{
    Section sect(*this, section);

    write(_T("Count"), (int)_vector.size());

    char buff[40];
    std::vector<CString>::const_iterator it = _vector.begin();
    for (int i(0); it != _vector.end(); it++, i++)
        write(itoa(i, buff, 10), *it);
}

void OutStream::write (const CString& section, const vector<vector<CString> >& vec_of_vec)
{
    Section sect(*this, section);

    write(_T("Count"), (int)vec_of_vec.size());

    char buff[40];
    std::vector<vector<CString> >::const_iterator it = vec_of_vec.begin();
    for (int i(0); it != vec_of_vec.end(); it++, i++)
        write(itoa(i, buff, 10), *it);
}

void InStream::read (const CString& section, vector<CString>& _vector)
{
    Section sect(*this, section);

    int count;
    read(_T("count"), count);
    _vector.resize(count);

    char buff[80];
    for (int i(0); i < count; i++)
        read(itoa(i, buff, 10), _vector.at(i));
}

void InStream::read (const CString& section, vector<vector<CString> >& vec_of_vec)
{
    Section sect(*this, section);

    int count;
    read(_T("count"), count);
    vec_of_vec.resize(count);

    char buff[80];
    for (int i(0); i < count; i++)
        read(itoa(i, buff, 10), vec_of_vec.at(i));
}

void OutStream::write (const CString& section, const set<CString>& _set)
{
    Section sect(*this, section);

    write(_T("Count"), (int)_set.size());
    
    char buff[80];
    std::set<CString>::const_iterator it = _set.begin();
    for (int i(0); it != _set.end(); it++, i++)
        write(itoa(i, buff, 10), *it);
}

void InStream::read (const CString& section, set<CString>& _set)
{
    Section sect(*this, section);

    int count;
    read(_T("Count"), count);

    char buff[80];
    for (int i(0); i < count; i++)
    {
        CString value;
        read(itoa(i, buff, 10), value);
        _set.insert(value);
    }
}

void OutStream::write (const CString& section, const pair<CString, CString>& _pair)
{
    Section sect(*this, section);

    write(_T("0"), _pair.first);
    write(_T("1"), _pair.second);
}

void InStream::read (const CString& section, pair<CString, CString>& _pair)
{
    Section sect(*this, section);

    read(_T("0"), _pair.first);
    read(_T("1"), _pair.second);
}


/////////////////////////////////////////////////////////////////////////////
// 	FileInStream & FileOutStream

// 02.06.2003 bug fix, diagnostics improvement
FileInStream::FileInStream (const char* filename) 
: m_infile(filename) 
{
    _CHECK_AND_THROW_(m_infile.good(), (CString(_T("Cannot open file \"")) + filename + _T("\" for reading.")));
}

FileOutStream::FileOutStream(const char* filename)
  : m_outfile(filename)
{
  _CHECK_AND_THROW_(m_outfile.good(),(CString(_T("Cannot open file \"")) + filename + _T("\" for writing.")));
}

void FileOutStream::write(const CString& name,const char* _val)
{
  CString val;
  Common::to_printable_str(_val,val);
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileOutStream::write(const CString& name,const CString& _val)
{
  CString val;
  Common::to_printable_str(_val,val);
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const CString& name,CString& val,bool p_skip /*= false*/)
{
  CString _name;
  std::string inval;

  if(!m_last.IsEmpty())
  {
    if(stricmp(name.GetString(),m_last.GetString()) != 0)
    {
      val.Empty();
      return;
    }
  }
  else
  {
    getline(m_infile,inval,'=');
    _name = inval.c_str();
    validateEntryName(name,_name,&p_skip);
    if(p_skip)
    {
      m_last = _name;
      return;
    }
  }
  getline(m_infile,inval);
  _ASSERTE(m_infile.good());
  m_last.Empty();
  Common::to_unprintable_str(inval.c_str(),val);
}


void FileOutStream::write(const CString& name,double val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const CString& name,double& val,bool p_skip /*= false*/)
{
  CString _name;
  std::string inval;
  getline(m_infile,inval,'=');
  _name = inval.c_str();
  validateEntryName(name,_name,&p_skip);
  if(p_skip)
  {
    return;
  }
  m_infile >> val; m_infile.get();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write(const CString& name,long val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const CString& name,long& val,bool skip /*=false*/)
{
  std::string inval;
  getline(m_infile,inval,'=');
  CString _name(inval.c_str());
  validateEntryName(name,_name);
  m_infile >> val; 
  m_infile.get();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write(const CString& name,unsigned long val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << hex << val << dec << endl;
}


void FileInStream::read(const CString& name,unsigned long& val,bool skip /*=false*/)
{
  std::string inval;
  getline(m_infile,inval,'=');
  CString _name(inval.c_str());

  validateEntryName(name,_name);
  m_infile.setf(std::ifstream::hex,std::ifstream::basefield);
  m_infile >> val;
  m_infile.setf(std::ifstream::dec,std::ifstream::basefield);
  m_infile.get();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write(const CString& name,int val)
{
  m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}


void FileInStream::read(const CString& name,int& val,bool skip /*=false*/)
{
  _ASSERTE(m_infile.good());

  if(!m_last.IsEmpty())
  {
    if(name.CompareNoCase(m_last) != 0)
    {
      val = 0;
      return;
    }
  }
  else
  {
    std::string inval;
    getline(m_infile,inval,'=');
    CString _name(inval.c_str());
    validateEntryName(name,_name,&skip);
    if(skip)
    {
      m_last = _name;
      return;
    }
  }
  m_infile >> val; 
  m_infile.get();
  m_last.Empty();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write (const CString& name, unsigned int val)
{
    m_outfile << m_sectionKey.Format(name) << '=' << hex << val << dec << endl;
}


void FileInStream::read  (const CString& name, unsigned int& val,bool /*p_skip = false*/)
{
  std::string inval;
  getline(m_infile,inval,'=');
  CString _name(inval.c_str());

  validateEntryName(name,_name);
  m_infile.setf( std::ifstream::hex, std::ifstream::basefield);
  m_infile >> val;
  m_infile.setf( std::ifstream::dec, std::ifstream::basefield);
  m_infile.get();
  _ASSERTE(m_infile.good());
}


void FileOutStream::write (const CString& name, bool val)
{
    m_outfile << m_sectionKey.Format(name) << '=' << val << endl;
}

void FileInStream::read  (const CString& name, bool& val,bool p_skip /*= false*/)
{
  if(!m_last.IsEmpty())
  {
    if(name.CompareNoCase(m_last) != 0)
    {
      val = false;
      return;
    }
  }
  else
  {
    std::string inval;
    getline(m_infile,inval,'=');
    CString _name(inval.c_str());
    validateEntryName(name,_name,&p_skip);
    if(p_skip)
    {
      m_last = _name;
      return;
    }
  }
  m_infile >> val; 
  m_infile.get();
  m_last.Empty();
  _ASSERTE(m_infile.good());
}

void FileInStream::validateEntryName(const CString& name, const CString& entryName,bool* p_skip)
{
  if(stricmp(m_sectionKey.Format(name).GetString(), entryName.GetString()))
  {
    if(!p_skip)
    {
      throw std::exception(_T("Unexpected entry in stream."));
    }
    return;
  }
  if(p_skip)
  {
    *p_skip = false;
  }
}

};//namespace OpenEditor
