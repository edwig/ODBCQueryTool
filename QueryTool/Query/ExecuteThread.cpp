////////////////////////////////////////////////////////////////////////
//
// File: ExecuteThread.cpp
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
#include "pch.h"
#include "ExecuteThread.h"
#include "QueryTool.h"
#include <GetLastErrorAsString.h>
#include <ConvertWideString.h>
#include <SQLInfoDB.h>

unsigned __stdcall
ExecuteSQLThread(void* p_context)
{
  // Gebruik StdExceptions i.p.v. SEH
  _set_se_translator(SeTranslator);

  ExecuteThread* sqlexec = reinterpret_cast<ExecuteThread*>(p_context);
  sqlexec->Execute();

  delete sqlexec;
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
//  THREAD EXECUTE
//
ExecuteThread::ExecuteThread(COEditorView* p_view)
              :m_view(p_view)
{
  CreateEvent();
}

ExecuteThread::~ExecuteThread()
{
  m_query.Close(false);
  CloseHandle(m_event);
}

bool
ExecuteThread::GetRunning()
{
  return m_running;
}

// Stopping the query thread
void
ExecuteThread::StopRunning()
{
  m_running = false;
  SignalEvent();
}

unsigned
ExecuteThread::Execute()
{
  m_running = true;

  // De verwerkingsloop
  try
  {
    while(m_running)
    {
      if(WaitForEvent(INFINITE) == 1)
      {
        if(m_running)
        {
          if(m_odbcCommand.IsEmpty())
          {
            ReadRestOfQuery();
          }
          else
          {
            ExecuteQuery();
          }
          m_view->QueryReady();
        }
      }
    }
  }
  catch(StdException& ex)
  {
    CString message(ex.GetErrorMessage());
    AfxMessageBox(message,MB_OK | MB_ICONHAND);
  }
  return 0;
}

void
ExecuteThread::CreateEvent()
{
  m_event = ::CreateEvent(NULL,FALSE,FALSE,NULL);
  DWORD error = GetLastError();
  if(error > 0)
  {
    CString message(_T("Fout bij aanmaken RunQueue event: ") + GetLastErrorAsString(error));
    AfxMessageBox(message,MB_OK | MB_ICONHAND);

  }
  if(m_event == NULL)
  {
    CString message(_T("RunQueue event kon niet worden aangemaakt"));
    AfxMessageBox(message,MB_OK | MB_ICONHAND);
  }
}

int
ExecuteThread::WaitForEvent(int p_milli)
{
  DWORD res = 0;
  if(p_milli == 0)
  {
    p_milli = INFINITE;
  }
  res = WaitForSingleObject(m_event,p_milli);
  if(res == WAIT_TIMEOUT)
  {
    return 0;
  }
  if(res == WAIT_OBJECT_0)
  {
    // Event komt binnen
    return 1;
  }
  // res = WAIT_ABANDONED | WAIT_FAILED
  AfxMessageBox(_T("Fout tijdens het wachten op RunQueue event"),MB_OK|MB_ICONERROR);
  return 0;
}

bool
ExecuteThread::SignalEvent()
{
  if(m_event)
  {
    return ::SetEvent(m_event);
  }
  return false;
}

bool
ExecuteThread::ExecuteQuery(int      p_line
                           ,CString& p_odbcCommand
                           ,bool     p_batch   /* = false */)
{
  // Record what to do
  m_line        = p_line;
  m_batch       = p_batch;
  m_odbcCommand = p_odbcCommand;

  // Go do it!
  if(!p_odbcCommand.IsEmpty())
  {
    return SignalEvent();
  }
  return false;
}

bool
ExecuteThread::ExecuteQuery()
{
  QueryToolApp* app         = dynamic_cast<QueryToolApp*>(AfxGetApp());
  SQLDatabase& database     = app->GetDatabase();
  bool     selectQuery      = false;
  bool     preMatureStopped = false;
  int      panelWindow      = m_batch ? QPW_OUTPUT_VIEW : QPW_QUERY_VIEW;
  int      prefetchLines    = m_view->GetDocument()->GetSettings().GetSQLPrefetchLines();
  int      lengthOption     = m_view->GetDocument()->GetSettings().GetSQLLengthOption();
  VarMap&  variables        = theApp.GetVariables();
  int      numVariables     = (int)variables.size();
  int      longestColumn    = 0;
  CString  partialCommand;

  m_odbcCommand.TrimLeft('\n');

  // Init the SQLQuery object
  m_query.Init(&database);
  m_query.SetLengthOption((LOption)lengthOption);

  if(!database.IsOpen())
  {
    AfxMessageBox(_T("Cannot execute ODBC query: Not connected to a database"),MB_OK | MB_ICONEXCLAMATION);
    m_odbcCommand.Empty();
    return false;
  }
  if(m_odbcCommand == _T(""))
  {
    AfxMessageBox(_T("No query to execute"),MB_OK | MB_ICONEXCLAMATION);
    m_odbcCommand.Empty();
    return false;
  }
  // See if the last IF statement prevents us from executing this SQL statement
  // So that scripting can decide whether or not to query this statement
  if(m_view->GetScriptCompare() == false)
  {
    if(m_view->GetScriptOutput() && m_view->GetScriptOutput()->GetIsOpen())
    {
      m_view->GetScriptOutput()->Write(XString(_T("SQL prevented by :IF statement\n")));
    }
    m_view->SetScriptCompare(true);
    m_odbcCommand.Empty();
    return true;
  }
  if(m_odbcCommand.GetLength() > 5)
  {
    if(m_odbcCommand.Left(6).CompareNoCase(_T("PROMPT")) == 0)
    {
      m_odbcCommand.Empty();
      return true;
    }
    if(m_odbcCommand.Left(6).CompareNoCase(_T("SELECT")) == 0 ||
       m_odbcCommand.Left(4).CompareNoCase(_T("WITH")) == 0 ||
       m_odbcCommand.Left(7).CompareNoCase(_T("EXPLAIN")) == 0)
    {
      selectQuery = true;
    }
    partialCommand = m_odbcCommand.Left(100) + _T(".....");
  }
  if(prefetchLines <= 1)
  {
    prefetchLines = 0x7ffffff;
  }
  if(!m_batch)
  {
    m_view->WriteStatisticsLine(_T("Query"),_T("Start: ") + partialCommand,true); // Reset the timer
  }

  // Look at the type of the command
  CString command = m_odbcCommand.Left(6);

  // Is it a commit?
  if(command.CompareNoCase(_T("COMMIT")) == 0)
  {
    app->OnODBCCommit();
    m_odbcCommand.Empty();
    return true;
  }

  // Make sure we have a transaction in case we do a DML command
  if(command.CompareNoCase(_T("INSERT")) == 0 ||
     command.CompareNoCase(_T("UPDATE")) == 0 ||
     command.CompareNoCase(_T("DELETE")) == 0 ||
     command.CompareNoCase(_T("MERGE ")) == 0 )
  {
    app->OnODBCBegin();
  }
  try
  {
    long row = 0;
    // INIT the grid again
    m_view->GetGridView()->InitGridEmpty(TYPE_QUERY);
    m_view->GetGridView()->SetPrefetchLines(0);

    // also record the history of the command
    m_view->WriteHistoryLine(m_odbcCommand);

    // Set rebind map for executing a query
    RebindMap* rebinds = theApp.GetRebinds();
    m_query.SetRebindMap(rebinds);
    // Set the buffer size for piece-wise extraction
    m_query.SetBufferSize(m_view->GetInboundBufferSize());

    // No lines fetched (yet)
    m_linesFetched = 0;

    // Run the query
    bool mustPrepare = (numVariables > 0) && (m_odbcCommand.Find('?') > 0);
    if(mustPrepare)
    {
      // Bind the variables
      int number = 0;
      for(auto& param : variables)
      {
        m_query.SetParameter(++number,param.second);
      }

      // Prepare the query
      m_query.DoSQLPrepare(XString(m_odbcCommand));
      if(!m_batch)
      {
        m_view->WriteStatisticsLine(_T("0 =>"),_T("Query prepared: ") + partialCommand);
      }
      // Go and execute
      m_query.DoSQLExecute();
    }
    else
    {
      m_query.DoSQLStatement(XString(m_odbcCommand));
    }
    if(!m_batch)
    {
      m_view->WriteStatisticsLine(_T("1 =>"),(mustPrepare ? _T("Execute ready: ") : _T("Query ready: ")) + partialCommand);
    }
    if(selectQuery)
    {
      // Init the CGridView
      m_view->GetGridView()->SetAutoExpand(false);
      m_view->GetGridView()->InitGridEmpty(TYPE_QUERY); // No first column
      m_view->GetGridView()->DeleteColumn(1);

      // Build the header of the grid
      UINT format = DT_SINGLELINE | DT_VCENTER | DT_WORDBREAK | DT_END_ELLIPSIS | DT_NOPREFIX | DT_EXPANDTABS;
      for(int k = 1; k <= m_query.GetNumberOfColumns();++k)
      {
        XString colname;
        SQLVariant* var = m_query.GetColumn(k);
        SWORD type = (SWORD)var->GetDataType();
        format &= ~(DT_LEFT | DT_RIGHT);
        format |= (type == SQL_C_CHAR) ? DT_LEFT : DT_RIGHT;
        if(var->IsNumericType())
        {
          // Numeric sort
          format |= DT_SORT_NUMERIC;
        }
        m_query.GetColumnName(k,colname);
        m_view->GetGridView()->InsertColumn((LPCTSTR)colname,format,k);
      }

      // Header of the select in the output script
      if(m_view->GetScriptSelect())
      {
        longestColumn = ScriptSelect(m_line);
      }
      if(m_query.GetRecord())
      {
        do
        {
          // Insert an extra row in the CGridView
          GetLineFromQuery(++row);
          ScriptSelect(m_line,longestColumn,row);
          if(row >= prefetchLines || !m_running)
          {
            // Reset pre-fetched lines
            m_view->GetGridView()->SetPrefetchLines(row);
            m_linesFetched   = row;
            preMatureStopped = true;
            break;
          }
        }
        while(m_query.GetRecord());
      }
    }
    else
    {
      // Not a select query -> Output panel is default
      panelWindow = QPW_OUTPUT_VIEW;
    }
    // Size the grid again
    m_view->GetGridView()->AutoSize();

    if(!row)
    {
      row = m_query.GetNumberOfRows();
    }
    if(!m_batch)
    {
      m_view->WriteStatisticsLine(_T("2 =>"),_T("Fetch ready: ") + partialCommand);
    }
    m_view->WriteOutputLine(m_line,row,partialCommand,m_query.GetError().GetString());
  }
  catch(TCHAR* errorText)
  {
    if(!m_batch)
    {
      CString text;
      text.Format(_T("Error in ODBC Query: %s"),errorText);
      AfxMessageBox(text,MB_OK | MB_ICONHAND);
    }
    m_view->WriteStatisticsLine(_T("2 =>"),_T("Error: ") + partialCommand);
    m_view->WriteOutputLine(m_line,m_query.GetNumberOfRows(),partialCommand,errorText);
    panelWindow = QPW_OUTPUT_VIEW;
  }
  catch(CString errorText)
  {
    if(!m_batch)
    {
      AfxMessageBox(errorText,MB_OK | MB_ICONHAND);
    }
    m_view->WriteStatisticsLine(_T("2 =>"),_T("Error: ") + partialCommand);
    m_view->WriteOutputLine(m_line,m_query.GetNumberOfRows(),partialCommand,errorText);
    panelWindow = QPW_OUTPUT_VIEW;
  }
  catch(StdException& ex)
  {
    if(!m_batch)
    {
      AfxMessageBox(ex.GetErrorMessage(),MB_OK | MB_ICONERROR);
    }
    m_view->WriteStatisticsLine(_T("2 =>"),_T("Error: ") + partialCommand);
    m_view->WriteOutputLine(m_line,m_query.GetNumberOfRows(),partialCommand,ex.GetErrorMessage().GetString());
    panelWindow = QPW_OUTPUT_VIEW;
  }
  catch(...)
  {
    if(!m_batch)
    {
      CString text;
      text.Format(_T("Unknown error in ODBC Query: %s"),m_odbcCommand.GetString());
      AfxMessageBox(text,MB_OK | MB_ICONHAND);
    }
    m_view->WriteStatisticsLine(_T("2 =>"),_T("Unknown: ") + partialCommand);
    m_view->WriteOutputLine(m_line,m_query.GetNumberOfRows(),partialCommand,_T("ERROR"));
    panelWindow = QPW_OUTPUT_VIEW;
  }
  // Clock is ready ticking
  if(!m_batch)
  {
    m_view->WriteStatisticsLine(_T("3 =>"),_T("Ready: ") + partialCommand);
  }

  // Auto change to the right panel or show the batch results
  if(m_batch)
  {
    m_view->GetQueryPanel()->GotoEndOfPanel(QPW_OUTPUT_VIEW);
    m_view->PumpMessages();
  }
  else
  {
    m_view->GetQueryPanel()->ChangePanel(panelWindow);
  }
  m_odbcCommand.Empty();
  return true;
}

void
ExecuteThread::ReadRestOfQuery()
{
  try
  {
    if(m_query.GetRecord())
    {
      do
      {
        GetLineFromQuery(++m_linesFetched);
      }
      while(m_query.GetRecord() && m_running);
    }
  }
  catch(CString& er)
  {
    CString error(_T("ERROR reading query: "));
    error += er;
    AfxMessageBox(error,MB_OK | MB_ICONERROR);
  }
  catch(StdException& ex)
  {
    CString error(_T("ERROR reading query: "));
    error += ex.GetErrorMessage();
    AfxMessageBox(error,MB_OK | MB_ICONERROR);
  }
  // Reset to 0, so grid will not attempt a second time
  m_linesFetched = 0;
}

int
ExecuteThread::ScriptSelect(int p_line)
{
  if(!m_view->GetScriptOutput() || !m_view->GetScriptSelect() || !m_view->GetScriptOutput()->GetIsOpen())
  {
    return 0;
  }

  int longest = 0;
  for(int k=1; k <= m_query.GetNumberOfColumns();++k)
  {
    XString name;
    XString result;
    m_query.GetColumnName(k,name);

    result.Format(_T(":COLUMN [%d] = [%s]\n"),k,name.GetString());
    m_view->GetScriptOutput()->Write(result);
    m_view->WriteOutputLine(p_line,1,result.GetString(),_T(""));

    if(name.GetLength() > longest)
    {
      longest = name.GetLength();
    }
  }
  return longest;
}

void
ExecuteThread::ScriptSelect(int p_line,int p_longest,int p_row)
{
  if(!m_view->GetScriptSelect())
  {
    return;
  }

  VarMap& vars = theApp.GetVariables();

  for(int k = 1; k <= m_query.GetNumberOfColumns();++k)
  {
    XString text;
    SQLVariant* column = m_query.GetColumn(k);
    column->GetAsString(text);

    XString name;
    m_query.GetColumnName(k,name);

    XString result;
    result.Format(_T(":ROW [%d:%-*s] = [%s]\n"),p_row,p_longest,name.GetString(),text.GetString());
    if(m_view->GetScriptOutput() && m_view->GetScriptOutput()->GetIsOpen())
    {
      m_view->GetScriptOutput()->Write(result);
    }
    m_view->WriteOutputLine(p_line,1,result.GetString(),_T(""));

    // Get the result in the variables array
    // so we can use the variables in the 'if' statement!
    VarMap::iterator it = vars.find(k);
    if(it != vars.end())
    {
      *(it->second) = *column;
    }
    else
    {
      vars[k] = new SQLVariant(column);
    }
  }
}

void
ExecuteThread::GetLineFromQuery(int row)
{
  UINT format = DT_SINGLELINE | DT_VCENTER | DT_WORDBREAK | DT_END_ELLIPSIS | DT_NOPREFIX | DT_EXPANDTABS;
  CString rowNumber;
  int charsetTranslation = 0;
  CString charset;
  // Getting charset translation
  COEDocument* doc = m_view->GetDocument();
  if(doc)
  {
    charsetTranslation = doc->GetSettings().GetSQLCharsetTranslation();
    charset = doc->GetSettings().GetSQLCharsetUsed();
  }

  rowNumber.Format(_T("%d"),row);
  m_view->GetGridView()->InsertRow((LPCTSTR)rowNumber,-1);

  // Insert the row info
  for(int k = 1; k <= m_query.GetNumberOfColumns(); ++k)
  {
    SQLVariant* var = m_query.GetColumn(k);
    SWORD type = (SWORD)var->GetDataType();

    if(type == SQL_C_BINARY)
    {
      int len = var->GetDataSize();
      _TUCHAR* buffer = (_TUCHAR*)calloc(2,(len * 2) + 6);
      const void* binbuf = var->GetAsBinary();
      _tcsncpy_s((TCHAR*)buffer,len * 2 + 1,(TCHAR*)binbuf,len * 2);
      buffer[len * 2] = 0;
      CString text((TCHAR*)buffer);
      m_view->GetGridView()->InsertItem(row,k,text,DT_LEFT);
      free(buffer);
    }
    if(type == SQL_C_CHAR)
    {
      XString s;
      var->GetAsString(s);
      CString text(s);
      if(charsetTranslation)
      {
        TranslateText(text,charsetTranslation,charset);
      }
      format &= ~(DT_LEFT | DT_RIGHT);
      format |= (type == SQL_CHAR || type == SQL_VARCHAR) ? DT_LEFT : DT_RIGHT;
      m_view->GetGridView()->InsertItem(row,k,text,format,var->GetDataType());
    }
    else
    {
      XString s;
      var->GetAsString(s);
      CString text(s);
      format &= ~(DT_LEFT | DT_RIGHT);
      format |= (type == SQL_CHAR || type == SQL_VARCHAR) ? DT_LEFT : DT_RIGHT;
      m_view->GetGridView()->InsertItem(row,k,text,format,var->GetDataType());
    }
  }
}

void
ExecuteThread::TranslateText(CString& p_text,int p_translation,CString p_charset)
{
  XString source;

  if(p_translation == 1)
  {
    QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
    if(!app->GetDatabase().GetSQLInfoDB()->MakeInfoDefaultCharset(source))
    {
      return;
    }
  }
  else if(p_translation == 2)
  {
    source = p_charset;
  }
  if(source.IsEmpty() || source == _T("-") || p_translation < 0 || p_translation > 2)
  {
    return;
  }
  p_text = DecodeStringFromTheWire(XString(p_text),source);
}

// Getting the column for an update
SQLVariant* 
ExecuteThread::GetColumnValue(int p_column)
{
  return m_query.GetColumn(p_column);
}

int
ExecuteThread::GetLinesFetchedSoFar()
{
  return m_linesFetched;
}

