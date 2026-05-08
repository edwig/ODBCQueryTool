////////////////////////////////////////////////////////////////////////
//
// File: ExecuteThread.h
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
#pragma once
#include <OEView.h>
#include <SQLQuery.h>

using namespace SQLComponents;

// Start using the object here
unsigned __stdcall ExecuteSQLThread(void* p_context);

class ExecuteThread
{
public:
  ExecuteThread(COEditorView* p_view);
 ~ExecuteThread();

  // Execute the thread in parallel
  unsigned  Execute();
  // Get the current running state
  bool      GetRunning();
  // Stopping the query thread
  void      StopRunning();
  // Continue on running
  bool      SignalEvent();

  // Execute a query from a beginning line
  bool      ExecuteQuery(int p_line,CString& p_odbcCommand,bool p_doBatch = false);
  // Execute the rest of the query
  void      ReadRestOfQuery();
  // Getting the column for an update
  SQLVariant* GetColumnValue(int p_column);
  // Lines fetched so far
  int       GetLinesFetchedSoFar();

private:
  void CreateEvent();
  int  WaitForEvent(int p_milli);
  int  ScriptSelect(int p_line);
  void ScriptSelect(int p_line,int p_longest,int p_row);
  void GetLineFromQuery(int row);
  void TranslateText(CString& p_text,int p_translation,CString p_charset);

  bool ExecuteQuery();

  SQLQuery      m_query;
  COEditorView* m_view    { nullptr };
  bool          m_running { false   };
  HANDLE        m_event   { NULL    };
  int           m_linesFetched  { 0 };
  // What to execute
  int           m_line    {       0 };
  bool          m_batch   {   false };
  CString       m_odbcCommand;
};
