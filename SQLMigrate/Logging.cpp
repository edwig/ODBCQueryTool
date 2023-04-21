////////////////////////////////////////////////////////////////////////
//
// File: Logging.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version number: See SQLComponents.h
//
#include "stdafx.h" 
#include "Logging.h"
#include "SQLMigrateApp.h"
#include "SQLMigrateDialog.h"
#include <time.h>

#pragma warning (disable: 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Logging::Logging() 
        :m_status(nothing)
        ,m_tables(0)
{
  m_flog  = nullptr;
  m_fout  = nullptr;
  m_fdrop = nullptr;

  m_logfile    = FILENAME_LOGFILE;
  m_script     = FILENAME_OUTPUT;
  m_dropscript = FILENAME_DROPFILE;
}

Logging::~Logging()
{
  Close();
}

void 
Logging::SetTables(int num)
{
  m_tables = num;
}

void 
Logging::SetLogStatus(LogType status)
{
  m_status = status;
}

void 
Logging::SetScript(XString p_script)
{
  m_script = p_script;
}

void 
Logging::SetDropScript(XString p_script)
{
  m_dropscript = p_script;
}

void
Logging::SetLogfile(XString p_logfile)
{
  m_logfile = p_logfile;
}

// Try to open all log files in 'append' mode
// If successful: close again
int
Logging::Open(bool p_scripting)
{
  // Always open standard logfile
  m_flog = _fsopen(m_logfile,"a+",_SH_DENYWR);
  if(m_flog == nullptr)
  {
    if(!m_flog)
    {
      StyleMessageBox(nullptr
                     ,"No write access for file: " FILENAME_LOGFILE
                     ,"No access"
                     ,MB_OK | MB_ICONWARNING);
      return 0;
    }
  }
  // If we do scripting: open the script files
  if(p_scripting)
  {
    m_fout  = _fsopen(m_script,    "a+",_SH_DENYWR);
    m_fdrop = _fsopen(m_dropscript,"a+",_SH_DENYWR);
    if(m_fout == nullptr || m_fdrop == nullptr)
    {
      if(!m_fout)
      {
        StyleMessageBox(nullptr
                       ,"No write access for file: " FILENAME_OUTPUT
                       ,"No Access"
                       ,MB_OK | MB_ICONWARNING);
      }
      if(!m_fdrop)
      {
        StyleMessageBox(nullptr
                       ,"No write access for file: " FILENAME_DROPFILE
                       ,"No Access"
                       ,MB_OK | MB_ICONWARNING);
      }
      return 0;
    }
  }
  return 1;
}

void
Logging::Close()
{
  if(m_flog)  fclose(m_flog);
  if(m_fout)  fclose(m_fout);
  if(m_fdrop) fclose(m_fdrop);
  m_flog = m_fout = m_fdrop = nullptr;
}

void 
Logging::SetDBType(bool p_source, XString p_type)
{
  SQLMigrateApp* appl = (SQLMigrateApp*)AfxGetApp();
  SQLMigrateDialog* dlg = (SQLMigrateDialog*)(appl->m_pMainWnd);

  if(dlg != nullptr && p_source)
  {
    dlg->SetSourceType(p_type);
  }
  if (dlg != nullptr && p_source == false)
  {
    dlg->SetTargetType(p_type);
  }
  XString text = "Database type       : " + p_type;
  WriteLog(text);
}

void
Logging::WriteLog(XString message)
{
#ifdef DEBUG
#if    DEBUG_DUMPING
  afxDump << message << "\n";
#endif
#endif
  SQLMigrateApp* appl   = (SQLMigrateApp*)AfxGetApp();
  SQLMigrateDialog* dlg = (SQLMigrateDialog*)(appl->m_pMainWnd);

  bool tolog = true;

  if(dlg != nullptr)
  {
    dlg->AddLogLine(message);
    dlg->HandleMessages();
    // User added/removed selection?
    tolog = dlg->m_toLogfile;
  }
  // Write to logfile
  if(tolog)
  {
    if(m_flog == nullptr)
    {
      m_flog = _fsopen(m_logfile,"a",_SH_DENYWR);
    }
    if(m_flog)
    {
      time_t deSeconde;
      struct tm *nu;
      time(&deSeconde);
      nu = localtime(&deSeconde);
      fprintf(m_flog,"%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d "
              ,nu->tm_year + 1900
              ,nu->tm_mon  + 1
              ,nu->tm_mday
              ,nu->tm_hour
              ,nu->tm_min
              ,nu->tm_sec);

      fputs(message,m_flog);
      fputc('\n',m_flog);
    }
  }
}

void 
Logging::WriteOut(XString statement,bool p_delim /* = false*/)
{
#ifdef DEBUG
#if    DEBUG_DUMPING
  afxDump << statement << "\n";
#endif
#endif
  if(m_fout == nullptr)
  {
    m_fout = _fsopen(m_script,"a",_SH_DENYWR);
  }
  if(m_fout)
  {
    fputs(statement,m_fout);
    if(p_delim)
    {
      fputc(';', m_fout);
    }
    fputc('\n',m_fout);
  }
}

void
Logging::WriteDrop(XString statement,bool p_delim /*=false*/)
{
#ifdef DEBUG
#if    DEBUG_DUMPING
  afxDump << statement << "\n";
#endif
#endif
  if(m_fdrop == nullptr)
  {
    m_fdrop = _fsopen(m_dropscript,"a",_SH_DENYWR);
  }
  if(m_fdrop)
  {
    fputs(statement,m_fdrop);
    if(p_delim)
    {
      fputc(';', m_fdrop);
    }
    fputc('\n',m_fdrop);
  }
}

void 
Logging::SetTableGauge(int num,int maxnum)
{
  SQLMigrateApp*   appl = (SQLMigrateApp *)AfxGetApp();
  SQLMigrateDialog *dlg = (SQLMigrateDialog *)(appl->m_pMainWnd);
  if(dlg)
  {
    dlg->SetTableGauge(num,maxnum);
    dlg->HandleMessages();
  }
}

void 
Logging::SetTablesGauge(int num)
{
  SQLMigrateApp*   appl = (SQLMigrateApp *)AfxGetApp();
  SQLMigrateDialog* dlg = (SQLMigrateDialog *)(appl->m_pMainWnd);
  if(dlg)
  {
    dlg->SetTablesGauge(num,m_tables);
    dlg->HandleMessages();
  }
}

void
Logging::SetStatus(XString status)
{
  SQLMigrateApp*   appl = (SQLMigrateApp *)AfxGetApp();
  SQLMigrateDialog* dlg = (SQLMigrateDialog *)(appl->m_pMainWnd);
  if(dlg)
  {
    dlg->SetStatus(status);
    WriteLog(status);
    dlg->HandleMessages();
  }
}
