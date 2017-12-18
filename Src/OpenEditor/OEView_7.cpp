/* 
    OVIEW_7.cpp
    Copyright (C) 2004 - 2017 Edwig Huisman

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

#include "stdafx.h"
#include "OpenEditor/OEView.h"
#include "OpenEditor/OEHighlighter.h"
#include "OpenEditorApp.h"

#undef String
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLVariant.h"
#include "SQLInfoDB.h"
#include "Query\NativeSQLDlg.h"
#include "Query\VariablesDlg.h"
#include <sql.h>
#include <time.h>

using namespace OpenEditor;
using namespace SQLComponents;

// MAXIMUM TOKEN NAME LENGTH FOR ODBC Environments
const int maxTokenlength = 512;

QueryHistory::QueryHistory()
{
}

QueryHistory::~QueryHistory()
{
}

void
COEditorView::OnUpdateCommandScriptExecute(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void 
COEditorView::OnScriptExecute()
{
  CString odbcCommand;
  int orgLine     = 0;
  int endFileLine = GetLineCount();
  int curLine     = GetPosition().line;
  long ticks = GetTickCount();
  bool first = true;
  CWaitCursor take_a_deep_sigh;

  m_queryPanel->ChangePanel(QPW_OUTPUT_VIEW);

  // Empty these grids
  m_statsView->InitGridEmpty(TYPE_STATS);
  m_outptView->InitGridEmpty(TYPE_OUTPT);

  WriteStatisticsLine("Script","Start of script",true);
  while(curLine < endFileLine)
  {
    int startline = GetODBCCommand(curLine,endFileLine,odbcCommand);
    if(odbcCommand == "")
    {
      break;
    }
    first = false;
    if(m_interval && (odbcCommand.GetAt(0) != ':'))
    {
      if(!ExecuteQueryRepeat(startline,odbcCommand,false))
      {
        break;
      }
    }
    else
    {
      ExecuteQuery(startline,odbcCommand,true);
    }
    // Reset the command
    odbcCommand = "";
    // And step
    moveCurrentLine(curLine);
    if(curLine == orgLine)
    {
      GoToDown();
    }
    else
    {
      GoToEnd();
      WordRight();
    }
    AdjustCaretPosition();

    // Redraw the current line marker
    RECT rect;
    GetClientRect(&rect);
    this->InvalidateRect(&rect);

    // Get the new current line
    curLine = GetPosition().line;
    endFileLine = GetLineCount();
  }

  if(first)
  {
    AfxMessageBox("No query to execute",MB_OK | MB_ICONEXCLAMATION);
  }
  // Total batch time
  WriteStatisticsLine("Script","End of script",false,ticks);
}

void
COEditorView::OnUpdateCommandScriptNative(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
COEditorView::OnUpdateCommandScriptCurrent(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
COEditorView::OnScriptNative()
{
  CString odbcCommand;
  int endFileLine = GetLineCount();
  int curLine = GetPosition().line;
  CWaitCursor take_a_deep_sigh;

  int firstline = GetODBCCommand(curLine,endFileLine,odbcCommand);
  CString native = theApp.FindNativeSQL(odbcCommand);

  // Show the Native SQL in a dialog
  native.Replace("\n","\r\n");
  CNativeSQLDlg dlg(this,native);
  dlg.DoModal();
}

void
COEditorView::OnScriptCurrent()
{
  CString odbcCommand;
  int endFileLine = GetLineCount();
  int curLine = GetPosition().line;
  CWaitCursor take_a_deep_sigh;

  int firstline = GetODBCCommand(curLine,endFileLine,odbcCommand);

  // Empty these grids
  m_statsView->InitGridEmpty(TYPE_STATS);
  m_outptView->InitGridEmpty(TYPE_OUTPT);

  ExecuteQuery(firstline,odbcCommand);
}

void
COEditorView::OnUpdateCommandScriptExecuteStep(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
COEditorView::OnScriptFindTable()
{
  Square blkPos;
  char   buffer[128];
  const char*  line = nullptr;
  int    len = 0;
  int    ind = 0;
  if (!WordFromPoint(GetPosition(), blkPos))
  {
    GetSelection(blkPos);
  }
  GetLine(blkPos.start.line,line,len);
  line += blkPos.start.column;
  for(ind = 0; ind < (blkPos.end.column - blkPos.start.column); ++ind)
  {
    buffer[ind] = *line++;
  }
  buffer[ind] = 0;
  // Find via the Application
  CString tableToFind(buffer);
  if(!tableToFind.IsEmpty())
  {
    theApp.FindTable(tableToFind);
  }
}

void
COEditorView::OnScriptTableDDL()
{
  Square blkPos;
  char   buffer[128];
  const char*  line = nullptr;
  int    len = 0;
  int    ind = 0;
  if(!WordFromPoint(GetPosition(),blkPos))
  {
    GetSelection(blkPos);
  }
  GetLine(blkPos.start.line,line,len);
  line += blkPos.start.column;
  for(ind = 0; ind < (blkPos.end.column - blkPos.start.column); ++ind)
  {
    buffer[ind] = *line++;
  }
  buffer[ind] = 0;
  // Find via the Application
  CString tableToFind(buffer);
  if(!tableToFind.IsEmpty())
  {
    theApp.TableDDL(tableToFind);
  }
}

void
COEditorView::OnScriptVariables()
{
  SQLVariant var1r; // (SQL_C_CHAR,   "This is a test for Edwig's Birthday");
  SQLVariant var2r; // (SQL_C_LONG,   "12345678");
  SQLVariant var3r; // (SQL_C_NUMERIC,"1234.56781234567890123456789");
  SQLVariant var4r; // (SQL_C_TYPE_TIMESTAMP,"1959-10-15 16:20:58.123456");

  SQLVariant* var1 = NULL;
  SQLVariant* var2 = NULL;
  SQLVariant* var3 = NULL;
  SQLVariant* var4 = NULL;

  VarMap& vars = theApp.GetVariables();
  VarMap::iterator it = vars.begin();
  if(it != vars.end()) { var1 = it->second; ++it; }
  if(it != vars.end()) { var2 = it->second; ++it; }
  if(it != vars.end()) { var3 = it->second; ++it; }
  if(it != vars.end()) { var4 = it->second; ++it; }

  VariablesDlg dlg(this
                  ,var1 ? var1 : &var1r
                  ,var2 ? var2 : &var2r
                  ,var3 ? var3 : &var3r
                  ,var4 ? var4 : &var4r);
  if(dlg.DoModal() == IDOK)
  {
    if((var1 == NULL) && var1r.GetDataType())
    {
      var1  = new SQLVariant();
      *var1 = var1r;
      vars.insert(std::make_pair(1,var1));
    }
    if((var2 == NULL) && var2r.GetDataType())
    {
      var2  = new SQLVariant();
      *var2 = var2r;
      vars.insert(std::make_pair(2,var2));
    }
    if((var3 == NULL) && var3r.GetDataType())
    {
      var3  = new SQLVariant();
      *var3 = var3r;
      vars.insert(std::make_pair(3,var3));
    }
    if((var4 == NULL) && var4r.GetDataType())
    {
      var4  = new SQLVariant();
      *var4 = var4r;
      vars.insert(std::make_pair(4,var4));
    }
  }
}

void
COEditorView::OnScriptExecuteStep()
{
  CString odbcCommand;
  int orgLine;
  int endFileLine = GetLineCount();
  int curLine     = GetPosition().line;
  CWaitCursor take_a_deep_sigh;

  orgLine = curLine;
  int firstline = GetODBCCommand(curLine,endFileLine,odbcCommand);

  // Empty these grids
  m_statsView->InitGridEmpty(TYPE_STATS);
  m_outptView->InitGridEmpty(TYPE_OUTPT);

  // Now execute
  ExecuteQuery(firstline,odbcCommand);

  // And step
  moveCurrentLine(curLine);
  if(curLine == orgLine)
  {
    GoToDown();
  }
  else
  {
    GoToEnd();
    WordRight();
  }
  AdjustCaretPosition();

  // Redraw the current line marker. OnPaint is the only way!
  RECT rect;
  GetClientRect(&rect);
  this->InvalidateRect(&rect);
}

// Get the ODBC Query command from this position
// input,output -> Curline
// Returns a first line with a keyword
int
COEditorView::GetODBCCommand(int& curLine
                            ,int& endFileLine
                            ,CString& odbcCommand)
{

  // Try a selected buffer first. This has priority over the
  // parsing of free text in the rest of the buffer
  string buff;
  GetBlock(buff);
  if(strlen(buff.c_str()))
  {
    odbcCommand = buff.c_str();
    return  curLine;
  }
  // No selected buffer. Try from current position to parse SQL text
  int i = 0;
  int firstline = -1;
  m_tableOne = "";
  m_findPrimary = false;
  HighlighterBase* phighlighter = m_bSyntaxHighlight ? m_highlighter.get() : 0;
  LineTokenizer tokenizer(false // Show spaces
                         ,GetTabSpacing()
                         ,phighlighter ? phighlighter->GetDelimiters() : GetDelimiters());
  char token[maxTokenlength + 1];
  CString multiLineCommand;

  // Status flags
  bool createSeen    = false;
  bool functionSeen  = false;
  bool procedureSeen = false;
  bool multiLine     = false;
  bool fromSeen      = false;
  bool tableSeen     = false;
  char instring      = '\0';

  CString endToken1  = ";";
  CString endToken2  = GetDocument()->GetSettings().GetSQLQueryTerminator().c_str();

  // Check endToken
  if(!endToken2.GetLength())
  {
    endToken2 = "/";
  }

  while(curLine < endFileLine)
  {
    const char* currentLine; 
    int currentLineLength;
    bool lastSpace = false;
    GetLine(curLine, currentLine, currentLineLength);
    CString tempLine;
    if(currentLine)
    {
      tempLine = currentLine;
      if(tempLine.GetLength() > currentLineLength)
      {
        tempLine.SetAt(currentLineLength,0);
      }
    }
    tempLine.Trim();
    if(tempLine.Left(2) == "--")
    {
      // This was a comment line
      ++curLine;
      continue;
    }
    if(multiLine)
    {
      multiLineCommand += "\n";
    }
    multiLineCommand += tempLine;
    // recalculation for maximum of visible line lengths
    tokenizer.StartScan(currentLine, currentLineLength);

    while (!tokenizer.Eol())
    {
      int pos, len, minLen;
      const char* str;

      tokenizer.GetCurrentSQLToken(str, pos, len, instring);
      if(firstline < 0)
      {
        firstline = curLine;
      }
      minLen = min(len,maxTokenlength);
      strncpy(token,str,minLen);
      token[minLen] = 0;
      // Status search
      if(stricmp(token,"create") == 0)
      {
        createSeen = true;
      }
      if(createSeen && stricmp(token,"procedure") == 0)
      {
        procedureSeen = true;
        endToken1     = endToken2;
      }
      if(createSeen && stricmp(token,"function") == 0)
      {
        functionSeen = true;
        endToken1    = endToken2;
      }
      if((endToken1.CompareNoCase(token) == 0) ||
         (endToken2.CompareNoCase(token) == 0 && pos == 0))
      {
        if(multiLine)
        {
          odbcCommand = multiLineCommand;
          int len1 = endToken1.GetLength();
          if(odbcCommand.Right(len1) == endToken1)
          {
            odbcCommand = odbcCommand.Left(odbcCommand.GetLength() - len1);
          }
          int len2 = endToken2.GetLength();
          if(odbcCommand.Right(len2) == endToken2)
          {
            odbcCommand = odbcCommand.Left(odbcCommand.GetLength() - len2);
          }
          odbcCommand.TrimLeft('\n');
          odbcCommand.TrimRight('\n');
        }
        TRACE("ODBC Ended command: %s\n",odbcCommand.Left(255));
        // End of command found
        return firstline;
      }
      // OneTable select for update option
      if(fromSeen && tableSeen)
      {
        if(stricmp(token,"where") && stricmp(token,"group") &&
           stricmp(token,"order") && stricmp(token,"having"))
        {
          m_tableOne = "";
        }
        fromSeen = false;
      }
      if(fromSeen && !tableSeen)
      {
        tableSeen = true;
        if(m_tableOne.IsEmpty())
        {
          m_tableOne = token;
        }
      }
      if(stricmp(token,"from") == 0)
      {
        fromSeen = true;
      }
      // Put token after odbcCommand
      if(!lastSpace && !odbcCommand.IsEmpty() && minLen > 1)
      {
        if(odbcCommand.Right(1) != '.')
        {
          odbcCommand += " ";
        }
      }
      odbcCommand += CString(token);
      if(instring)
      {
        break;
      }
      lastSpace = false;
      if(len > 1 && !IsDigits(token))
      {
        odbcCommand += " ";
        lastSpace = true;
      }
      tokenizer.Next();
    }
    ++curLine;
    multiLine = true;
  }
  if(multiLine)
  {
    odbcCommand = multiLineCommand;
  }
  TRACE("ODBC Command: %s\n",odbcCommand);
  return firstline;
}

bool
COEditorView::IsDigits(char* token)
{
  const char* pnt = token;
  while(pnt && *pnt)
  {
    if(isdigit(*pnt))
    {
      ++pnt;
    }
    else
    {
      return false;
    }
  }
  return true;
}

void
COEditorView::OnUpdateCommandScriptNextError(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_errorMap.size() != 0);
}

void
COEditorView::OnScriptNextError()
{
  bool found = false;
  ErrorMap::iterator iter;
  for(iter  = m_errorMap.begin();
      iter != m_errorMap.end();
      ++iter)
  {
    if(*iter > m_currentError)
    {
      found = true;
      break;
    }
  }
  if(!found)
  {
    m_currentError = 0;
    AfxMessageBox("No next error",MB_OK);
    return;
  }
  m_currentError = *iter;
  OpenEditor::Position pos;
  pos.line = m_currentError;
  MoveToAndCenter(pos);
}

void
COEditorView::OnUpdateCommandScriptPrevError(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_errorMap.size() != 0);
}

void
COEditorView::OnScriptPrevError()
{
  bool found = false;

  int iter;
  for(iter = (int)m_errorMap.size() - 1; iter >= 0; --iter)
  {
    if(m_errorMap[iter] < m_currentError)
    {
      found = true;
      break;
    }
  }
  if(!found)
  {
    int endFileLine = GetLineCount();
    m_currentError = endFileLine;
    AfxMessageBox("No previous error",MB_OK);
    return;
  }
  m_currentError = m_errorMap[iter];
  OpenEditor::Position pos;
  pos.line = m_currentError;
  MoveToAndCenter(pos);
}

// This is the handle to the output grid on Create time of the MDIChildView
void
COEditorView::RegisterGrid(CWnd *grid)
{
  m_gridView = (CGridView*) grid;
}

void 
COEditorView::RegisterHistory(CWnd *histo)
{
  m_histoView = (CGridView*) histo;
}

void 
COEditorView::RegisterOutput(CWnd* output)
{
  m_outptView = (CGridView*) output;
}

void 
COEditorView::RegisterStatistics(CWnd* stats)
{
  m_statsView = (CGridView*) stats;
}

void
COEditorView::RegisterQueryPanel(CQueryPanelWnd* panel)
{
  m_queryPanel = panel;
}

static CString s_odbcCommand;

bool
COEditorView::ExecuteQueryRepeat(int p_line,CString& odbcCommand,bool batch /*=false*/)
{
  if(odbcCommand.IsEmpty())
  {
    odbcCommand = s_odbcCommand;
  }
  // Check for 'wholeminutes' option
  if(m_wholeMinutes)
  {
    long remainder = MilisecondsToMinute();
    if(remainder)
    {
      CWaitCursor takeADeepSigh;
      ::Sleep(remainder);
    }
  }
  // Execute the query
  bool result = ExecuteQuery(p_line,odbcCommand,batch,m_scriptOutput);

  // Check on a specified number of iterations
  if(m_repeat >= 0)
  {
    if(--m_repeat == 0)
    {
      // exit the application
      PostQuitMessage(0);
      // Don't trigger the timer
      m_interval = 0;
    }
  }
  // register for next execution
  if(m_interval > 0)
  {
    s_odbcCommand = odbcCommand;
    SetTimer(QUERY_TIMER,m_interval * 1000,NULL);
  }
  return result;
}

bool
COEditorView::ExecuteQuery(int      p_line
                          ,CString& odbcCommand
                          ,bool     batch     /*=false*/
                          ,FILE*    p_script  /*=NULL*/)
{
  COpenEditorApp *app       = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  SQLDatabase&   database   = app->GetDatabase();
  bool     selectQuery      = false;
  bool     preMatureStopped = false;
  int      panelWindow      = batch ? QPW_OUTPUT_VIEW : QPW_QUERY_VIEW;
  UINT     defFormat        = DT_LEFT|DT_VCENTER|DT_WORDBREAK|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EXPANDTABS;
  int      prefetchLines    = GetDocument()->GetSettings().GetSQLPrefetchLines();
  VarMap&  variables        = theApp.GetVariables();
  int      numVariables     = (int)variables.size();
  CString  partialCommand;

  // Init the query recordset
  m_query.Init(&database);

  odbcCommand.TrimLeft('\n');
  if(odbcCommand.Left(1) == ':')
  {
    return ScriptCommand(p_line,odbcCommand);
  }
  if(!database.IsOpen())
  {
    AfxMessageBox("Cannot execute ODBC query: Not connected to a database",MB_OK | MB_ICONEXCLAMATION);
    return false;
  }
  if(odbcCommand == "")
  {
    AfxMessageBox("No query to execute",MB_OK | MB_ICONEXCLAMATION);
    return false;
  }
  if(odbcCommand.GetLength() > 5)
  {
    if(odbcCommand.Left(6).CompareNoCase("SELECT") == 0)
    {
      selectQuery = true;
    }
    partialCommand = odbcCommand.Left(100) + ".....";
  }
  if(prefetchLines <= 1)
  {
    prefetchLines = 0x7ffffff;
  }
  WriteStatisticsLine("Query","Start: " + partialCommand,true); // Reset the timer
  try
  {
    long row = 0;
    // INIT the grid again
    m_gridView->InitGridEmpty(TYPE_QUERY);
    m_gridView->SetPrefetchLines(0);

    // also record the history of the command
    WriteHistoryLine(odbcCommand);

    // Set rebind map for executing a query
    RebindMap* rebinds = theApp.GetRebinds();
    m_query.SetRebindMap(rebinds);
    // Set the buffer size for piece-wise extraction
    m_query.SetBufferSize(m_inboundBufferSize);

    // Run the query
    bool mustPrepare = (numVariables > 0) && (odbcCommand.Find('?') > 0);
    if(mustPrepare)
    {
      m_query.DoSQLPrepare(odbcCommand);
      WriteStatisticsLine("0 =>","Query prepared: " + partialCommand);
      m_query.DoSQLExecute();
    }
    else
    {
      m_query.DoSQLStatement(odbcCommand); // TODO ,0,p_script);
    }
    WriteStatisticsLine("1 =>",(mustPrepare ? "Execute ready: " : "Query ready: ") + partialCommand);
    if(selectQuery)
    {
      // Init the CGridView
      m_gridView->SetAutoExpand(false);
      m_gridView->InitGridEmpty(TYPE_QUERY); // No first column
      m_gridView->DeleteColumn(1);

      // Build the header of the grid
      UINT format = DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EXPANDTABS;
      for(int k = 1; k <= m_query.GetNumberOfColumns();++k)
      {
        CString kolnaam;
        SQLVariant* var = m_query.GetColumn(k);
        SWORD type = var->GetDataType();
        format &= ~(DT_LEFT | DT_RIGHT);
        format |= (type == SQL_C_CHAR) ? DT_LEFT : DT_RIGHT;
        if(var->IsNumericType())
        {
          // Numeric sort
          format |= DT_SORT_NUMERIC;
        }
        m_query.GetColumnName(k,kolnaam);
        m_gridView->InsertColumn((LPCTSTR)kolnaam,format,k);

      }
      // Header of the select in the output script
      if(m_scriptSelect)
      {
        ScriptSelect(p_line);
      }
      if(m_query.GetRecord())
      {
        do
        {
          // Insert an extra row in the CGridView
          GetLineFromQuery(++row);
          if(row >= prefetchLines)
          {
            // Reset prefetched lines
            m_gridView->SetPrefetchLines(row);
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
    m_gridView->AutoSize();

    if(!row)
    {
      row = m_query.GetNumberOfRows();
    }
    WriteStatisticsLine("2 =>","Fetch ready: " + partialCommand);
    WriteOutputLine(p_line,row,partialCommand,m_query.GetError());
  }
  catch(char *errorTekst)
  {
    if(!batch)
    {
      CString text;
      text.Format("Error in ODBC Query: %s",errorTekst);
      AfxMessageBox(text,MB_OK | MB_ICONHAND);
    }
    WriteStatisticsLine("2 =>","Error: " + partialCommand);
    WriteOutputLine(p_line,m_query.GetNumberOfRows(),partialCommand,errorTekst);
    panelWindow = QPW_OUTPUT_VIEW;
  }
  catch(CString errorText)
  {
    if(!batch)
    {
      AfxMessageBox(errorText,MB_OK | MB_ICONHAND);
    }
    // @EH TODO Print to output pane
    WriteStatisticsLine("2 =>","Error: " + partialCommand);
    WriteOutputLine(p_line,m_query.GetNumberOfRows(),partialCommand,errorText);
    panelWindow = QPW_OUTPUT_VIEW;
  }
  catch(...)
  {
    if(!batch)
    {
      CString text;
      text.Format("Unknown error in ODBC Query: %s",odbcCommand);
      AfxMessageBox(text,MB_OK | MB_ICONHAND);
    }
    WriteStatisticsLine("2 =>","Unknown: " + partialCommand);
    WriteOutputLine(p_line,m_query.GetNumberOfRows(),partialCommand,"ERROR");
    panelWindow = QPW_OUTPUT_VIEW;
  }
  // Clock is ready ticking
  WriteStatisticsLine("3 =>","Ready: " + partialCommand);

  // Auto change to the right panel
  if(!batch)
  {
    m_queryPanel->ChangePanel(panelWindow);
  }
  return true;
}

void
COEditorView::GetLineFromQuery(int row)
{
  UINT format = DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EXPANDTABS;
  CString rowNumber;
  rowNumber.Format("%d",row);
  m_gridView->InsertRow((LPCSTR) rowNumber,-1);

  // Insert the row info
  for(int k = 1; k <= m_query.GetNumberOfColumns(); ++k)
  {
    SQLVariant* var = m_query.GetColumn(k);
    SWORD type = var->GetDataType();
    if(type == SQL_C_BINARY)
    {
      int len = var->GetDataSize();
      unsigned char* buffer = (unsigned char*) calloc(2,(len * 2) + 6);
      const char* binbuf = var->GetAsChar();
      strncpy_s((char*)buffer,len*2+1,binbuf,len*2);
      buffer[len * 2]  = 0;
      CString text = (char*)buffer;
      m_gridView->InsertItem(row,k,text,DT_LEFT);
      free(buffer);
    }
    else
    {
      CString text;
	    var->GetAsString(text);
      format &= ~(DT_LEFT | DT_RIGHT);
      format |= (type == SQL_CHAR || type == SQL_VARCHAR) ? DT_LEFT : DT_RIGHT;
      m_gridView->InsertItem(row,k,text,format,var->GetDataType());
    }
  }
}

void
COEditorView::ReadRestOfQuery()
{
  if(m_query.GetRecord())
  {
    do 
    {
      GetLineFromQuery(++m_linesFetched);
    } 
    while(m_query.GetRecord());
  }
  // Reset to 0, so grid will not attempt a second time
  m_linesFetched = 0;
}

// Write a line with info to the statistics view
void
COEditorView::WriteStatisticsLine(CString p_step,CString p_line,bool p_reset /*=false*/,long p_totalticks /* = 0 */)
{
  UINT    defFormat  = DT_LEFT|DT_VCENTER|DT_WORDBREAK|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EXPANDTABS;
  static  long ticks = 0;
  long    interval;
  CString totalTime;

  if(p_reset)
  {
    ticks = GetTickCount();
  }
  // Normal timer interval
  interval = GetTickCount() - ticks;
  // Override for batches
  if(p_totalticks)
  {
    interval = GetTickCount() - p_totalticks;
  }
  totalTime.Format("%d.%03d",interval/1000,(interval%1000));

  int row = m_statsView->AppendRow(p_step);
  m_statsView->InsertItem(row,1,totalTime,defFormat);
  m_statsView->InsertItem(row,2,p_line,defFormat);
  m_statsView->AutoSize();
}

void
COEditorView::WriteOutputLine(int p_line,int p_affected,CString p_command,CString p_result)
{
  UINT defFormat  = DT_LEFT|DT_VCENTER|DT_WORDBREAK|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EXPANDTABS;
  CString lineNumber;
  CString rowsNumber;
  CString text;
  int     image = 0;  // OK Image

  text  = p_command + " : ";
  if(p_result != "" && p_result != "OK")
  {
    image = 1; // Error image
    text += p_result;

    // Append to the error map
    m_errorMap.push_back(p_line);
    std::sort(m_errorMap.begin(), m_errorMap.end());

  }
  else
  {
    text += "OK";
  }
  lineNumber.Format("%d",p_line + 1);
  rowsNumber.Format("%d",p_affected);

  int row = m_outptView->AppendRow(lineNumber);
  m_outptView->InsertImage(row,1,image);
  m_outptView->InsertItem(row,2,rowsNumber,defFormat);
  m_outptView->InsertItem(row,3,text,defFormat);
  m_outptView->AutoSize();
}

void
COEditorView::WriteHistoryLine(CString p_command)
{
  UINT defFormat  = DT_LEFT|DT_VCENTER|DT_WORDBREAK|DT_END_ELLIPSIS|DT_NOPREFIX|DT_EXPANDTABS;
  int row         = -1;
  int newNumber   = 1;
  int reruns      = 0;
  HistoryMap::iterator iter;

  for(iter = m_historyMap.begin(); iter != m_historyMap.end(); ++iter)
  {
    ++newNumber;
    if(p_command.CompareNoCase(iter->command) == 0)
    {
      reruns = ++(iter->reruns);
      row = iter->number;
      break;
    }
  }
  if(row == -1)
  {
    QueryHistory hist;
    hist.command = p_command;
    hist.number  = newNumber;
    hist.reruns  = 1;
    m_historyMap.push_back(hist);
    CString num;
    num.Format("%d",newNumber);
    row = m_histoView->AppendRow(num);
    m_histoView->InsertItem(row,0,num,defFormat);
    num = "1"; // reruns
    m_histoView->InsertItem(row,1,num,defFormat);
    m_histoView->InsertItem(row,2,p_command,defFormat);
  }
  else
  {
    CString num;
    num.Format("%d",row);
    for(int i=0; i < m_histoView->GetColumnCount(); ++i)
    {
      if(m_histoView->GetItemText(i,0) == num)
      {
        num.Format("%d",reruns);
        m_histoView->SetItemText(i,1,num);
        break;
      }
    }
  }
  m_histoView->AutoSize();

  if(m_interval)
  {
    return;
  }
  if(m_scriptOutput)
  {
    fprintf(m_scriptOutput,"Query: %s\n",p_command.GetString());
  }
}

bool
COEditorView::CanEdit()
{
  if(!m_findPrimary)
  {
    COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
    SQLDatabase& database  = app->GetDatabase();
    MPrimaryMap primaries;

    m_keyMap.clear();

    if(!m_tableOne.IsEmpty())
    {
      if(database.GetSQLInfoDB()->GetPrimaryKeyInfo(m_tableOne,m_primaryName,primaries))
      {
        int ind = 0;
        for(auto& prim : primaries)
        {
          PrimKeyInfo info;
          info.m_colName  = prim.m_columnName;
          info.m_colPos   = prim.m_columnPosition;
          info.m_queryPos = 0;
          m_keyMap.insert(std::make_pair(ind,info));
          ++ind;
        }
      }
    }
    m_findPrimary = true;
  }
  // Can edit the query if a minimum of one column is found
  return (m_keyMap.size() > 0);
}

CString
COEditorView::UpdateCondition(int row,CString column)
{
  if(m_keyMap.empty())
  {
    return "";
  }
  CString condition;

  for(unsigned ind = 0;ind < m_keyMap.size(); ++ind)
  {
    int xx = 0;
    PrimKeyInfo* info = &m_keyMap[ind];

    // Check that we do not use the primary key itself
    if(column.CompareNoCase(info->m_colName) == 0)
    {
      // Oeps
      CString error;
      error.Format("Cannot update. The field \"%s\" belongs to the primary key \"%s\"!",column,m_primaryName);
      AfxMessageBox(error);
      return "";
    }
    // Append to previous condition
    if(!condition.IsEmpty())
    {
      condition += "\n   AND ";
    }
    condition += info->m_colName;
    condition += " = ";
    if(!info->m_queryPos)
    {
      for(int xx=1; xx < m_gridView->GetColumnCount(); ++xx)
      {
        CString column = m_gridView->GetItemText(0,xx);
        if(column.CompareNoCase(info->m_colName)==0)
        {
          info->m_queryPos = xx;
          break;
        }
      }
    }
    xx = info->m_queryPos;
    if(xx == 0)
    {
      CString error;
      error.Format("Cannot update, field \"%s\" of primary key \"%s\" is missing from your query!",info->m_colName,m_primaryName);
      AfxMessageBox(error);
      return "";
    }
    CString text = m_gridView->GetItemText(row,xx);
    SQLVariant* var = m_query.GetColumn(xx);
    if(var == NULL)
    {
      return "";
    }
    SWORD type = var->GetDataType();
    if(type == SQL_C_CHAR)
    {
      text.Replace("\'","\'\'");
      text = CString("'") + text + "'";
    }
    condition += text;
  }
  return condition;
}

bool
COEditorView::UpdateTable(int row,int col,CString text)
{
  if(m_tableOne.IsEmpty())
  {
    // Cannot do this
    return false;
  }
  COpenEditorApp *app   = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  SQLDatabase& database = app->GetDatabase();

  // Get column
  CString column = m_gridView->GetItemText(0,col);
  SQLVariant* var = m_query.GetColumn(col);
  if(var->GetDataType() == SQL_C_CHAR)
  {
    text.Replace("\'","\'\'");
    text = CString("'") + text + "'";
  }
  CString query = "UPDATE " + m_tableOne + "\n" + 
                  "   SET " + column + " = " + text + "\n" +
                  " WHERE ";
  CString searchCondition = UpdateCondition(row,column);
  if(searchCondition.IsEmpty())
  {
    return false;
  }
  query += searchCondition;
  SQLQuery rs(&database);
  rs.DoSQLStatement(query);

  CString info = "Row updated by primary/unique key: ";
  info += m_primaryName;
  Common::SetStatusText(info);

  return true;
}

// Parse script command
bool
COEditorView::ScriptCommand(int p_line,CString &odbcCommand)
{
  odbcCommand.TrimLeft(':');
  odbcCommand.TrimLeft(' ');
  int end   = odbcCommand.Find(' ');
  if(end >= 0)
  {
    try
    {
      CString word = odbcCommand.Left(end);
      word.MakeLower();
      CString tail = odbcCommand.Mid(end+1);
           if(word == "file")       return ScriptCommandFile     (p_line,tail);
      else if(word == "print")      return ScriptCommandPrint    (p_line,tail);
      else if(word == "rebind")     return ScriptCommandRebind   (p_line,tail);
      else if(word == "if")         return ScriptCommandIf       (p_line,tail);
      else if(word == "select")     return ScriptCommandSelect   (p_line,tail);
      else if(word == "atexec")     return ScriptCommandAtExec   (p_line,tail);
      else if(word == "repeat")     return ScriptCommandRepeat   (p_line,tail);
//    else if(word == "endrepeat")  return ScriptCommandEndRepeat(p_line,tail);
      else if(word.Left(8) == "variable")
      {
         int varNum = atoi(word.Mid(8));
         return ScriptCommandVariable(p_line,varNum,tail);
      }
    }
    _OE_DEFAULT_HANDLER_;
  }
  return false;
};

bool
COEditorView::ScriptCommandPrint(int p_line,CString print)
{
  CString error;
  if(m_scriptOutput)
  {
    print.Trim();
    print.TrimLeft('\'');
    print.TrimRight('\'');
    if(m_scriptCompare)
    {
      fprintf(m_scriptOutput,":COMPARE OK\n");
    }
    else
    {
      fprintf(m_scriptOutput,"%s\n",print.GetString());
      if(m_ifLast)
      {
        error = "ERROR";
        WriteOutputLine(p_line,0,":print " + print,error);
      }
    }
  }
  // Can be set by the IF command!
  m_scriptCompare = false;
  m_ifLast        = false;
  return true;
}

bool
COEditorView::ScriptCommandFile(int p_line,CString file)
{
  bool result = false;
  CString error;
  CString mode = "w";
  int pos = file.Find(' ');
  if(pos >= 0)
  {
    mode = file.Left(pos);
    file = file.Mid(pos);
    file.Trim();
    file.TrimLeft('\'');
    file.TrimRight('\'');
    mode.MakeLower();
         if(mode == "append") mode = "a";
    else if(mode == "write")  mode = "w";
    else if(mode == "open")   mode = "w";
    else if(mode == "close")
    {
      if(m_scriptOutput)
      {
        fprintf(m_scriptOutput,"==================\n");
        fprintf(m_scriptOutput,"ODBC Script closed\n\n\n");
        fclose(m_scriptOutput);
        m_scriptOutput = NULL;
      }
      WriteOutputLine(p_line,0,":file " + file,error);
      return true;
    }
    else
    {
      error = "Missing file mode";
      WriteOutputLine(p_line,0,":file " + file,error);
      return false;
    }
    if(m_scriptOutput)
    {
      fclose(m_scriptOutput);
    }
    m_scriptOutput = fopen(file.GetString(),mode.GetString());
    if(m_scriptOutput)
    {
      time_t time;
      _tzset();
      _time64(&time);
      struct tm* now = localtime(&time);
      fprintf(m_scriptOutput
             ,"ODBC Script output: %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d\n"
              "=======================================\n"
             ,now->tm_year + 1900
             ,now->tm_mon  + 1
             ,now->tm_mday
             ,now->tm_hour
             ,now->tm_min
             ,now->tm_sec);
             
    }
    result = (m_scriptOutput != NULL);
  }
  if(file.CompareNoCase("close") == 0)
  {
    if(m_scriptOutput)
    {
      fprintf(m_scriptOutput,"==================\n");
      fprintf(m_scriptOutput,"ODBC Script closed\n");
      fclose(m_scriptOutput);
      m_scriptOutput = NULL;
    }
    result = true;
  }
  WriteOutputLine(p_line,0,":file " + file,error);
  return result ;
}

bool
COEditorView::ScriptCommandRebind(int p_line,CString rebind)
{
  bool result = false;
  CString error;
  RebindMap* map = theApp.GetRebinds();
  int pos = rebind.Find(',');

  if(pos >= 0)
  {
    CString word2;
    CString word1 = rebind.Left(pos);
    word2 = rebind.Mid(pos+1);

    SQLVariant var;
    word1.Trim();
    word2.Trim();
    int from = var.FindDatatype((char*)(LPCSTR)word1);
    int to   = var.FindDatatype((char*)(LPCSTR)word2);
    int sql  = var.FindSQLDatatype((char*)(LPCSTR)word2);
    if(to == 0 && sql != 0)
    {
      // Use SQL_XXXX type to rebind (BINARY or CHAR) type
      to = sql;
    }
    if(from != 0 && to != 0)
    {
      map->insert(std::make_pair(from,to));
      if(m_scriptOutput)
      {
        fprintf(m_scriptOutput,":REBIND datatype [%s] to [%s]\n",word1.GetString(),word2.GetString());
      }
      result = true;
    }
    else
    {
      error = "No valid datatypes found";
    }
  }
  else
  {
    rebind.Trim();
    if(rebind.CompareNoCase("RESET") == 0)
    {
      map->clear();
      if(m_scriptOutput)
      {
        fprintf(m_scriptOutput,":REBIND reset\n");
      }
      result = true;
    }
    else
    {
      error = "No reset";
    }
  }
  WriteOutputLine(p_line,0,":rebind " + rebind,error);
  return false;
}

bool
COEditorView::ScriptCommandIf(int p_line,CString command)
{
  bool result = false;
  CString error;

  command.Trim();
  command.TrimRight(';');
  CString var = command.Left(8);
  command = command.Mid(8);
  if(var.CompareNoCase("variable") == 0)
  {
    int numVar = atoi(command);
    VarMap& varMap = theApp.GetVariables();
    VarMap::iterator it = varMap.find(numVar);
    if(it != varMap.end())
    {
      SQLVariant* var = it->second;
      int pos = command.Find("<>");
      if(pos > 0)
      {
        command = command.Mid(pos + 2);
        command.Trim();
        command.TrimLeft ('\'');
        command.TrimRight('\'');

        CString value;
		    var->GetAsString(value);
        m_scriptCompare = (command.Compare(value) == 0);
        m_ifLast = true;
        if(m_scriptOutput)
        {
          fprintf(m_scriptOutput
                 ,":IF variable%d [%s] %s [%s]\n"
                 ,numVar
                 ,value.GetString()
                 ,m_scriptCompare ? "==" : "<>"
                 ,command.GetString());
        }
        result = true;
      }
    }
  }
  else
  {
    error = "Need variable";
  }
  WriteOutputLine(p_line,0,":if " + command,error);
  return result;
}

bool
COEditorView::ScriptCommandVariable(int p_line,int p_varNum,CString p_tail)
{
  bool result = false;
  CString error;
  VarMap& varMap = theApp.GetVariables();
  VarMap::iterator it = varMap.find(p_varNum);
  if(it == varMap.end())
  {
    if(p_tail.Find('=') < 0)
    {
      SQLVariant* var = new SQLVariant();
      varMap.insert(std::make_pair(p_varNum,var));
      it = varMap.find(p_varNum);
      if(m_scriptOutput)
      {
        fprintf(m_scriptOutput,":NEW variable%d\n",p_varNum);
      }
    }
  }
  if(it != varMap.end())
  {
    SQLVariant* var = it->second;
    int pos = p_tail.Find(' ');
    if(pos >= 0)
    {
      CString word = p_tail.Left(pos);

      if(word == '=')
      {
        // Assignment
        p_tail = p_tail.Mid(pos+1);
        p_tail.Trim();
        p_tail.TrimLeft('\'');
        p_tail.TrimRight('\'');
        if(m_scriptOutput)
        {
          fprintf(m_scriptOutput,":ASSIGN variable%d = [%s]\n",p_varNum,p_tail.GetString());
        }
        var->SetData(var->GetDataType(),(char*)p_tail.GetString());
        result = true;
      }
      else
      {
        // Must be "[param] datatype"
        int paramType = var->FindParamtype((char*)(LPCSTR)word);
        if(paramType > 0)
        {
          p_tail = p_tail.Mid(pos+1);
          var->SetParameterType(paramType);
          if(m_scriptOutput)
          {
            fprintf(m_scriptOutput,":PARAM variable%d [%s]\n",p_varNum,word.GetString());
          }
        }
        // Now do datatype
        p_tail.Trim();
        int dataType = var->FindDatatype((char*)(LPCSTR)p_tail);
        if(dataType != 0)
        {
          var->SetData(dataType,"");
          if(m_scriptOutput)
          {
            fprintf(m_scriptOutput,":DTYPE variable%d [%s]\n",p_varNum,p_tail.GetString());
          }
          result = true;
        }
      }
    }
  }
  else
  {
    error = "No variable found/added";
  }
  WriteOutputLine(p_line,p_varNum,":variable " + p_tail,error);
  return result;
}

bool
COEditorView::ScriptCommandSelect(int p_line,CString p_tail)
{
  CString result;
  p_tail.Trim();
  m_scriptSelect = false;
  if(p_tail.CompareNoCase("on") == 0)
  {
    m_scriptSelect = true;
  }
  if(m_scriptOutput)
  {
    fprintf(m_scriptOutput,":SELECT %s\n", m_scriptSelect ? "on" : "off");
  }
  WriteOutputLine(p_line,0,":select " + p_tail,result);
  return true;
}

bool
COEditorView::ScriptCommandAtExec(int p_line,CString p_tail)
{
  bool result = false;
  CString error;
  VarMap& varMap = theApp.GetVariables();

  p_tail.Trim();
  p_tail.MakeLower();
  if(p_tail.Left(7).CompareNoCase("inbound") == 0)
  {
    p_tail.TrimLeft("inbound");
    p_tail.TrimLeft();

    m_inboundBufferSize = atoi(p_tail);
    if(m_inboundBufferSize < 0 || m_inboundBufferSize > (4 * 1024))
    {
      m_inboundBufferSize = 0;
    }
  }
  p_tail.TrimLeft("variable");
  int pos = p_tail.Find(' ');
  if(pos > 0)
  {
    int varNum = atoi(p_tail.Left(pos));
    int size   = atoi(p_tail.Mid(pos+1));
    VarMap::iterator it = varMap.find(varNum);
    if(it != varMap.end())
    {
      SQLVariant* var = it->second;
      var->SetAtExec(size > 0);
      var->SetBinaryPieceSize(size);
      if(m_scriptOutput)
      {
        fprintf(m_scriptOutput,":ATEXEC variable<%d> set %s, size: %d\n",varNum,size > 0 ? "ON" : "OFF", size);
      }
      result = true;
    }
  }
  else
  {
    error = "No variable";
  }
  WriteOutputLine(p_line,0,":atexec " + p_tail,error);
  return result;
}

bool
COEditorView::ScriptCommandRepeat(int p_line,CString p_tail)
{
  bool result = false;

  // Repeat indefinetly
  m_repeat = -1;

  int pos = p_tail.Find(' ');
  if(pos >= 0)
  {
    CString word = p_tail.Left(pos);
    m_interval = atoi(word);

    p_tail = p_tail.Mid(pos + 1);
    pos = p_tail.Find(' ');
    while(pos >= 0)
    {
      word = p_tail.Left(pos);
      if(word.CompareNoCase("wholeminutes") == 0)
      {
        m_wholeMinutes = true;
      }
      if(word.CompareNoCase("times") == 0)
      {
        p_tail = p_tail.Mid(pos + 1);
        pos = p_tail.Find(' ');
        m_repeat = atoi(p_tail);

        if(pos < 0)
        {
          break;
        }
      }
      // Next word
      p_tail = p_tail.Mid(pos + 1);
      pos = p_tail.Find(' ');
    }
  }
  CString line;
  line.Format(":REPEAT interval [%d]",m_interval);
  if(m_repeat)
  {
    CString text;
    text.Format(" repeat-times [%d]",m_repeat);
    line += text;
  }
  if(m_wholeMinutes)
  {
    line += " wholeminutes start";
  }
  WriteOutputLine(p_line,0,line,"");
  if(m_scriptOutput)
  {
    fprintf(m_scriptOutput,"%s\n\n",line.GetString());
    fprintf(m_scriptOutput,"Date                  Miliseconds\n");
    fprintf(m_scriptOutput,"-------------------   ---------------\n");
  }
  return result;
}

bool
COEditorView::ScriptCommandEndRepeat(int p_line,CString p_tail)
{
  // NOOP?
//   m_interval = 0;
//   m_repeat   = 0;
//   m_wholeMinutes = false;
  return true;
}

void
COEditorView::ScriptSelect(int p_line)
{
  CString error;
  VarMap& vars = theApp.GetVariables();
  VarMap::iterator it;
  for(int k=1; k <= m_query.GetNumberOfColumns();++k)
  {
    SQLVariant* var = NULL;
    SQLVariant* column = m_query.GetColumn(k);
    it = vars.find(k);
    if(it != vars.end())
    {
      var = it->second;
      *var = *column;
    }
    else
    {
	    CString data;
      int   type = column->GetDataType();
      column->GetAsString(data);
      var = new SQLVariant();
	    var->SetData(type,data);
      vars.insert(std::make_pair(k,var));
    }
    var->SetParameterType(SQL_RESULT_COL);
    CString result;
	  column->GetAsString(result);
    result.Format(":COLUMN [%d] = [%s]\n",k,result);
    if(m_scriptOutput)
    {
      fprintf(m_scriptOutput,result.GetString());
    }
    WriteOutputLine(p_line,1,result,error);
  }
}

long
COEditorView::MilisecondsToMinute()
{
  time_t time;
  _tzset();
  _time64(&time);
  struct tm* now = localtime(&time);

  return (60 - now->tm_sec) * 1000;
}
