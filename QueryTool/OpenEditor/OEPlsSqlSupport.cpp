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

/*
    16.03.2003 bug fix, plsql match sometimes fails after some edit operations
*/

#include "pch.h"
#include "OEContext.h"
#include "OEPlsSqlSupport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{

PlSqlSupport::PlSqlSupport (Storage* pstorage) 
             :LanguageSupport(pstorage),
              m_parser(m_analyzer)
{
  m_done       = false;
  m_currLine   = 0;
  m_batchLimit = 100;
}

bool PlSqlSupport::FindMatch (int line, int offset, Match& match)
{
  _CHECK_AND_THROW_(m_pStorage, _T("SQL Support has not been initialized!"))

  processText(INT_MAX);

	int index;
	const SyntaxNode* node;
	
	if (m_analyzer.FindToken(line, offset, node, index))
	{
    match.found   = true;
    match.broken  = false;
    match.partial = false;
    node->GetToken(index, match.line[0], match.offset[0], match.length[0]);
		node->GetNextToken(index, match.line[1], match.offset[1], match.length[1], match.backward);
    return true;
	}
  match.found = false;
  return false;
}

bool PlSqlSupport::OnIdle ()
{
    return processText(m_batchLimit);
}

bool PlSqlSupport::processText (int batchLimit)
{
  if (m_pStorage->GetActionId() != m_actionId)
  {
    m_done = false;
    m_currLine  = 0;
    m_parser.Clear();
    m_analyzer.Clear(); // 16.03.2003 bug fix, plsql match sometimes fails after some edit operations
    m_actionId = m_pStorage->GetActionId();
  }

  if (!m_done)
  {
    int nlines = m_pStorage->GetLineCount();

    for (int i = 0; i < batchLimit && m_currLine < nlines; i++, m_currLine++)
    {
	    int length;
	    const char* str;
      m_pStorage->GetLine(m_currLine, str, length);
	    m_parser.PutLine(m_currLine, str, length);
    }
    if (m_currLine == nlines)
    {
        m_done = true;
        m_parser.PutEOF(m_currLine);
    }
    TRACE(_T("SQL Support::processText: curr line = %d, lines = %d\n"), m_currLine, nlines);
  }
  return  !m_done;
}

};