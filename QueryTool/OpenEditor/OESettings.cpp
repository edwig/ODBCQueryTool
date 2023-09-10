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

// 08.11.2003 bug fix, file extension is not recognized properly if it's shorter then 3 chars

#include "pch.h"
#include "OESettings.h"
#include "OEHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace OpenEditor
{
    using namespace Common;

    const int ClassSettings::m_FileSaveAsDefault = effDefault;

/////////////////////////////////////////////////////////////////////////////
// 	BaseSettings

void BaseSettings::AddSubscriber (SettingsSubscriber* psubscriber) const
{
  if (psubscriber)
  {
     m_subscribers.push_back(psubscriber);
  }
}


void BaseSettings::RemoveSubscriber(SettingsSubscriber* psubscriber) const
{
  if(psubscriber)
  {
    std::vector<SettingsSubscriber*>::iterator it = m_subscribers.begin();
    while(it != m_subscribers.end())
    {
      if(*it == psubscriber)
      {
        it = m_subscribers.erase(it);
        continue;
      }
      ++it;
    }
  }
}


void BaseSettings::NotifySettingsChanged ()
{
  for(auto& subscriber : m_subscribers)
  {
    subscriber->OnSettingsChanged();
  }
}


/////////////////////////////////////////////////////////////////////////////
// 	ClassSettings

GlobalSettings::GlobalSettings ()
{

  m_PrintHeader       = ""  ;
	m_PrintFooter       = "&P";
	m_PrintLeftMargin   = 20  ;
	m_PrintRightMargin  = 10  ;
	m_PrintTopMargin    = 10  ;
	m_PrintBottomMargin = 10  ;

  m_PrintMarginMeasurement = GetSysPrintMeasuremnt();

  m_DefFileExtension = "sql";
  m_UndoLimit      = 1000 ;
  m_UndoMemLimit   = 1000 ; // kb
  m_TabExpand      = true ;
  m_TruncateSpaces = true ;
  m_CursorBeyondEOL = false;
  m_CursorBeyondEOF = false;

  m_NormalizeKeywords = false;

	m_BlockKeepMarking                = false;
  m_BlockKeepMarkingAfterDragAndDrop= true;
	m_BlockDelAndBSDelete             = true ;
	m_BlockTypingOverwrite            = true ;
	m_BlockTabIndent                  = true ;
	m_ColBlockDeleteSpaceAfterMove    = true ;
	m_ColBlockCursorToStartAfterPaste = true ;

  m_FileBackup  = true;
  m_FileLocking = true;

  m_FileWholeWord  = false;
  m_FileMatchCase  = false;
  m_FileUseRegEx   = false;
  m_FileSubfolders = true;
  m_FileSaveBefore = false;
  m_FileCollapse   = false;
  m_FileWhatToFind = "";
  m_FileFileTypes  = "";
  m_FileStartDirectory = "";

  m_FileAutoscrollAfterReload = false;
  m_Locale = "English";
}


int GlobalSettings::GetSysPrintMeasuremnt ()
{
    char buff[4];

    if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, buff, sizeof(buff)))
    {
        return (buff[0] == '1') ? 1 : 0;
    }

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// 	ClassSettings

ClassSettings::ClassSettings (SettingsManager& owner)
: m_owner(owner),
m_globalSettings(owner.GetGlobalSettings())
{
}


ClassSettings::ClassSettings (SettingsManager& owner, const ClassSettings& other)
: m_owner(owner),
m_globalSettings(owner.GetGlobalSettings())
{
    *this = other;
}


ClassSettings::~ClassSettings ()
{
}


ClassSettings& ClassSettings::operator = (const ClassSettings& other)
{
    m_Name           = other.m_Name           ;
    m_Extensions     = other.m_Extensions     ;
    m_Language       = other.m_Language       ;
    m_Delimiters     = other.m_Delimiters     ;
    m_TabSpacing     = other.m_TabSpacing     ;
    m_IndentSpacing  = other.m_IndentSpacing  ;
    m_IndentType     = other.m_IndentType     ;
    m_TabExpand      = other.m_TabExpand      ;
    m_LineNumbers    = other.m_LineNumbers    ;
    m_VisibleSpaces  = other.m_VisibleSpaces  ;

    m_NormalizeKeywords = other.m_NormalizeKeywords;
    m_FileCreateAs   = other.m_FileCreateAs   ;

    m_VisualAttributesSet = other.m_VisualAttributesSet;

    NotifySettingsChanged();
    return *this;
}


const TemplatePtr ClassSettings::GetTemplate () const
{
    return m_owner.GetTemplateCollection().Find(GetName());
}

/////////////////////////////////////////////////////////////////////////////
// 	SettingsManager

SettingsManager::SettingsManager ()
{
}


SettingsManager::SettingsManager (const SettingsManager& cloned)
{
    *this = cloned;
}


SettingsManager::~SettingsManager ()
{
}


SettingsManager& SettingsManager::operator = (const SettingsManager& other)
{
    m_globalSettings = other.m_globalSettings;
    m_templateCollection = other.m_templateCollection;

    ClassSettingsVector::const_iterator
        other_it(other.m_classSettingsVector.begin()),
        other_end(other.m_classSettingsVector.end());

    // it is a not initialized object
    if (m_classSettingsVector.size() == 0)
    {
        for (; other_it != other_end; other_it++)
        {
            m_classSettingsVector.push_back(ClassSettingsPtr(new ClassSettings(*this, **other_it)));
        }

    }
    // copy from a cloned one
    else if (m_classSettingsVector.size() == other.m_classSettingsVector.size())
    {
        ClassSettingsVector::iterator
            it(m_classSettingsVector.begin()),
            end(m_classSettingsVector.end());

        for (; it != end && other_it != other_end; it++, other_it++)
        {
            **it = **other_it;
        }
    }
    else
    {
        throw std::logic_error("SettingsManager: illegal assigment.");
    }

    return *this;
}


void SettingsManager::CreateClass (const char* name)
{
    m_classSettingsVector.push_back(ClassSettingsPtr(new ClassSettings(*this)));
    m_classSettingsVector.back()->SetName(name);
}


void SettingsManager::DestroyClass (const char* name)
{
    ClassSettingsVector::iterator
        it(m_classSettingsVector.begin()),
        end(m_classSettingsVector.end());

    for (; it != end; it++)
        if ((**it).GetName() == name)
        {
            m_classSettingsVector.erase(it);
            break;
        }
}


inline
bool is_ext_supported (const CString& _list, const char* _ext)
{
  if (_ext && *_ext)
  {
    CString list = " " + _list + " ";
    list.MakeUpper();

    CString ext = " " + CString(_ext) + " ";
    ext.MakeUpper();

    if(list.Find(ext) >= 0)
    {
      return true;
    }
  }
  return false;
}

const ClassSettings& SettingsManager::FindByExt(const char* ext, bool _default) const
{
  for(auto& classet : m_classSettingsVector)
  {
    if(is_ext_supported((*classet).GetExtensions(),ext))
    {
      return *classet;
    }
  }
  if(!_default)
  {
    throw StdException(XString("Class for extension [") + ext + " not found!");
  }
  return *m_classSettingsVector.at(0);
}


const ClassSettings& SettingsManager::FindByName (const char* name) const
{
  for(auto& classet : m_classSettingsVector)
  {
    if((*classet).GetName().Compare(name) == 0)
    {
      return *classet;
    }
  }
  throw StdException(XString("Class with name [") + name + "] not found!");
}

};//namespace OpenEditor
