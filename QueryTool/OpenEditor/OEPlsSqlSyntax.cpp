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

#include "stdafx.h"
#include <map>
#include <COMMON/AppGlobal.h>
#include <COMMON/ExceptionHelper.h>
#include "OpenEditor/OEPlsSqlSyntax.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{

bool SyntaxNode::StopSign (EToken token) const
{
    switch (token)
    {
    case etDECLARE:				return true;
    case etFUNCTION:			return true;
    case etPROCEDURE:			return true;
    case etPACKAGE:				return true;
    case etBODY:				return false;
    case etBEGIN:				return true;
    case etEXCEPTION:			return true;
    case etEND:					return false;
    case etIF:					return false;
    case etTHEN:				return false;
    case etELSE:				return true;
    case etELSIF:				return true;
    case etFOR:					return true;
    case etWHILE:				return true;
    case etLOOP:				return false;
    case etEXIT:				return true;
    case etIS:					return false;
    case etAS:					return false;
    case etSEMICOLON:			return true;
    case etQUOTE:				return true;
    case etDOUBLE_QUOTE:		return false;
    case etLEFT_ROUND_BRACKET:	return true;
    case etRIGHT_ROUND_BRACKET:	return true;
    case etMINUS:				return false;
    case etSLASH:				return true;
    case etSTAR:				return false;
    case etSELECT:				return true;
    case etINSERT:				return true;
    case etUPDATE:				return true;
    case etDELETE:				return true;
    case etALTER:				return true;
    case etANALYZE:				return true;
    case etCREATE:				return true;
    case etDROP:				return true;
    case etFROM:				return true;
    case etWHERE:				return true;
    case etSET:					return true;
    case etOPEN:                return true;
    }
    return false;
}

SyntaxNode::SyntaxNode ()                               
{ 
    m_failure = false; 
    m_sibling = m_parent = m_child = 0; 
}

SyntaxNode::~SyntaxNode ()
{ 
}

void SyntaxNode::AppendSibling (SyntaxNode* sibling)    
{ 
    if (!m_sibling)
    {
        m_sibling = sibling; 
        sibling->m_parent = m_parent; 
    }
    else
        m_sibling->AppendSibling(sibling);
}

void SyntaxNode::AppendChild  (SyntaxNode* child)       
{ 
    if (!m_child)
    {
        m_child = child; 
        child->m_parent = this; 
    }
    else
        m_child->AppendSibling(child);
}

int SyntaxNode::GetLevel () const
{
    int level;
    const SyntaxNode* node = this;
    for (level = 0; node && node->m_parent; node = node->m_parent, level++);
    return level;
}

bool SyntaxNode::FindToken (int line, int offset, const SyntaxNode*& node, int& index) const
{
    if (m_tokens.size())
    {
        TRACE("SyntaxNode::FindToken, Node=%s, line=%d, offset=%d\n", GetDebugString(), line, offset);
        TRACE("\tm_tokens.begin()->line=%d, m_tokens.begin()->offset=%d\n", m_tokens.begin()->line, m_tokens.begin()->offset);
        TRACE("\tm_tokens.rbegin()->line=%d, m_tokens.rbegin()->offset=%d\n", m_tokens.rbegin()->line, m_tokens.rbegin()->offset);

        if ((m_tokens.begin()->line < line 
            || (m_tokens.begin()->line == line && m_tokens.begin()->offset <= offset))
        && (m_tokens.rbegin()->line > line
            || (m_tokens.rbegin()->line == line && (m_tokens.rbegin()->offset + m_tokens.rbegin()->length) > offset)))
        {
            for (std::vector<Token>::const_iterator it = m_tokens.begin();
                it != m_tokens.end();
                ++it)
            {
                if (it->line == line && (it->offset <= offset && offset < it->offset + it->length))
                {
                    node = this;
                    index = (int)(it - m_tokens.begin());
                    return true;
                }
            }

            return m_child ? m_child->FindToken(line, offset, node, index) : false;
        }
    }
    
    return m_sibling ? m_sibling->FindToken(line, offset, node, index) : false;
}

void SyntaxNode::GetToken (int index, int& line, int& offset, int& length) const
{
    line   = m_tokens.at(index).line;
    offset = m_tokens.at(index).offset;
    length = m_tokens.at(index).length;
}

// always stop on the first and last token
//   or on token with "stop sign"
void SyntaxNode::GetNextToken (int index, int& line, int& offset, int& length, bool& backward) const
{
    if (++index >= static_cast<int>(m_tokens.size()))
    {
        backward = true;
        index = 0;
    }
    else
    {
        backward = false;

        for (; index < static_cast<int>(m_tokens.size()); index++)
        {
            if (StopSign(m_tokens[index]))
                break;
        }

        if (static_cast<int>(m_tokens.size()) == index)
            --index;
    }

    line   = m_tokens.at(index).line;
    offset = m_tokens.at(index).offset;
    length = m_tokens.at(index).length;
}

UnitNode::~UnitNode ()
{
    Clear();
}

void UnitNode::Clear ()
{
    _ASSERTE(!m_sibling);
    m_child = 0;
}

bool UnitNode::FindToken (int line, int offset, const SyntaxNode*& node, int& index) const
{
#ifdef _DEBUG
    if (m_child)
        m_child->DbgPrintNode();
#endif

    return m_child ? m_child->FindToken(line, offset, node, index) : false;
}

BlockNode::BlockNode (const Token& token)
{
    m_tokens.push_back(token);

    switch (token) 
    {
    case etDECLARE: m_state = eDeclare; break;
    case etBEGIN:   m_state = eBegin; break;
    default:        Failure();
    }
}

bool BlockNode::PutToken (const Token& token)  
{
    switch (m_state) 
    {
    case eDeclare:
        if (token == etBEGIN)
        {
            m_tokens.push_back(token);
            m_state = eBegin;
            return true;
        }
        break;
    case eBegin:
        if (token == etEXCEPTION)
        {
            m_tokens.push_back(token);
            m_state = eException;
            return true;
        }
        // no break
    case eException:
        if (token == etEND)
        {
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eEnd:
        switch (token)
        {
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        default:
            Failure();
        }
        break;
    }
    return false;
}

PackageNode::PackageNode (const Token& token)
{
    m_body = false;
    m_tokens.push_back(token);
    
    if (token == etPACKAGE) 
        m_state = ePackage;
    else
        Failure();
}

bool PackageNode::PutToken (const Token& token)  
{
    switch (m_state)
    {
    case ePackage:
        switch (token)
        {
        case etBODY:
            m_tokens.push_back(token);
            m_state = eBody;
            m_body = true;
            return true;
        case etUNKNOWN:
        case etIDENTIFIER:
        case etDOUBLE_QUOTED_STRING:
            m_tokens.push_back(token);
            m_state = eName1;
            return true;
        }
        Failure();
        break;
    case eBody:
        switch (token)
        {
        case etUNKNOWN:
        case etIDENTIFIER:
        case etDOUBLE_QUOTED_STRING:
            m_tokens.push_back(token);
            m_state = eName1;
            return true;
        }
        Failure();
        break;
    case eName1:
        switch (token)
        {
        case etDOT:
            m_tokens.push_back(token);
            m_state = eNameSeparator;
            return true;
        case etIS:
        case etAS:
            m_tokens.push_back(token);
            m_state = eIs;
            return true;
        }
        Failure();
        break;
    case eNameSeparator:
        switch (token)
        {
        case etUNKNOWN:
        case etIDENTIFIER:
        case etDOUBLE_QUOTED_STRING:
            m_tokens.push_back(token);
            m_state = eName2;
            return true;
        }
        Failure();
        break;
    case eName2:
        switch (token)
        {
        case etIS:
        case etAS:
            m_tokens.push_back(token);
            m_state = eIs;
            return true;
        }
        Failure();
        break;
    case eIs:
        switch (token)
        {
        case etBEGIN:
            m_tokens.push_back(token);
            m_state = eBegin;
            return true;
        case etEND:
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eBegin:
        switch (token)
        {
        case etEXCEPTION:
            m_tokens.push_back(token);
            m_state = eException;
            return true;
        case etEND:
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eException:
        if (token == etEND)
        {
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eEnd:
        switch (token)
        {
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        case etUNKNOWN:
        case etIDENTIFIER: // skip package name
            return true;
        default:
            Failure();
        }
        break;
    }

    return false;
}

ProcedureNode::ProcedureNode (const Token& token)
{
    m_declaration = false;

    m_tokens.push_back(token);
    
    switch (token)
    {
    case etPROCEDURE:
    case etFUNCTION:
        m_state = eProcedure;
        break;
    default:
        Failure();
    }
}

bool ProcedureNode::PutToken (const Token& token)  
{
    switch (m_state)
    {
    case eProcedure:
        switch (token)
        {
        case etUNKNOWN:
        case etIDENTIFIER:
        case etDOUBLE_QUOTED_STRING:
            m_tokens.push_back(token);
            m_state = eName1;
            return true;
        }
        Failure();
        break;
    case eName1:
        switch (token)
        {
        case etDOT:
            m_tokens.push_back(token);
            m_state = eNameSeparator;
            return true;
        case etIS:
        case etAS:
            m_tokens.push_back(token);
            m_state = eIs;
            return true;
        case etUNKNOWN:
        case etIDENTIFIER:
        case etLEFT_ROUND_BRACKET:
            m_state = eArguments;
            return false; // it's parsed as an expression!!!
        case etSEMICOLON:
            m_state = eComplete;
            m_declaration = true;
            return true;
        }
        Failure();
        break;
    case eNameSeparator:
        switch (token)
        {
        case etUNKNOWN:
        case etIDENTIFIER:
        case etDOUBLE_QUOTED_STRING:
            m_tokens.push_back(token);
            m_state = eName2;
            return true;
        }
        Failure();
        break;
    case eName2:
        switch (token)
        {
        case etLEFT_ROUND_BRACKET:
            m_state = eArguments;
            return false; // it's parsed as an expression!!!
        case etSEMICOLON:
            m_state = eComplete;
            m_declaration = true;
            return true;
        case etIS:
        case etAS:
            m_tokens.push_back(token);
            m_state = eIs;
            return true;
        }
        Failure();
        break;
    case eArguments:
        switch (token)
        {
        case etSEMICOLON:
            m_state = eComplete;
            m_declaration = true;
            return true;
        case etIS:
        case etAS:
            m_tokens.push_back(token);
            m_state = eIs;
            return true;
        }
        break;
    case eIs:
        switch (token)
        {
        case etBEGIN:
            m_tokens.push_back(token);
            m_state = eBegin;
            return true;
        case etLANGUAGE:
            m_tokens.push_back(token);
            m_state = eLanguage;
            return true;
        }
        break;
    case eBegin:
        if (token == etEXCEPTION)
        {
            m_tokens.push_back(token);
            m_state = eException;
            return true;
        }
        // continue
    case eException:
        if (token == etEND)
        {
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eEnd:
        switch (token)
        {
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        case etUNKNOWN:
        case etIDENTIFIER: // skip package name
            return true;
        default:
            Failure();
        }
        break;
    case eLanguage:
        switch (token)
        {
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        }
        break;
    }

    return false;
}

SelectionNode::SelectionNode (const Token& token)
{
    m_tokens.push_back(token);

    if (token == etIF) 
        m_state = eIf;
    else
        Failure();
}

bool SelectionNode::PutToken (const Token& token)  
{
    switch (m_state)
    {
    case eIf:
    case eElsIf:
        switch (token)
        {
        case etELSIF:
            m_tokens.push_back(token);
            m_state = eElsIf;
            return true;
        case etELSE:
            m_tokens.push_back(token);
            m_state = eElse;
            return true;
        case etEND:
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eElse:
        if (token == etEND)
        {
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eEnd:
        switch (token)
        {
        case etIF:
            m_tokens.push_back(token);
            m_state = eEndIf;
            return true;
        default:
            Failure();
        }
        break;
    case eEndIf:
        switch (token)
        {
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        default:
            Failure();
        }
        break;
    }
    return false;
}

IterationNode::IterationNode (const Token& token)
{
    m_tokens.push_back(token);

    switch (token)
    {
    case etWHILE:
        m_state = eWhile;
        break;
    case etFOR:
        m_state = eFor;
        break;
    case etLOOP:
        m_state = eLoop;
        break;
    default:
        Failure();
    }
}

bool IterationNode::PutToken (const Token& token)  
{
    switch (m_state)
    {
    case eWhile:
    case eFor:
        if (token == etLOOP)
        {
            m_tokens.push_back(token);
            m_state = eLoop;
            return true;
        }
        break;
    case eLoop:
        switch (token)
        {
        case etEXIT:
            m_tokens.push_back(token);
            return true;
        case etEND:
            m_tokens.push_back(token);
            m_state = eEnd;
            return true;
        }
        break;
    case eEnd:
        switch (token)
        {
        case etLOOP:
            m_tokens.push_back(token);
            m_state = eEndLoop;
            return true;
        default:
            Failure();
        }
        break;
    case eEndLoop:
        switch (token)
        {
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        default:
            Failure();
        }
        break;
    }
    return false;
}

ExpressionNode::ExpressionNode (const Token& token)
{
    m_tokens.push_back(token);

    if (token == etLEFT_ROUND_BRACKET)
        m_state = eOpen;
    else
        Failure();
}

bool ExpressionNode::PutToken (const Token& token)  
{
    if (m_state == eOpen) 
    {
        if (token == etRIGHT_ROUND_BRACKET)
        {
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        }
    }
    return false;
}

SelectNode::SelectNode (const Token& token)
{
    m_tokens.push_back(token);

    if (token == etSELECT) 
        m_state = eSelect;
    else
        Failure();
}

bool SelectNode::PutToken (const Token& token)  
{
    switch (m_state) 
    {
    case eSelect:
        if (token == etFROM)
        {
            m_tokens.push_back(token);
            m_state = eFrom;
            return true;
        }
        break;
    case eFrom:
        switch (token)
        {
        case etWHERE:
            m_tokens.push_back(token);
            m_state = eWhere;
            return true;
        }
        // continue
    case eWhere:
        switch (token)
        {
        case etEOF:
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;

            // close a driver insert statement
            if (m_parent)
            {
                switch(m_parent->GetType())
                {
                case elINSERT:
                case elOPEN_CURSOR:
                    m_parent->PutToken(token);
                }
            }
            return true;
        case etRIGHT_ROUND_BRACKET:
            // for inner queries
            {
                Token tk;
                tk = etUNKNOWN;
                tk.line = token.line;
                tk.offset = token.offset;
                m_tokens.push_back(tk);
                m_state = eComplete;
                // close an expression
                m_parent->PutToken(token);
            }
            break;
        }
    }
    return false;
}

InsertNode::InsertNode (const Token& token)
{
    m_tokens.push_back(token);

    if (token == etINSERT) 
        m_state = eInsert;
    else
        Failure();
}

bool InsertNode::PutToken (const Token& token)  
{
    switch (m_state) 
    {
    case eInsert:
        switch (token)
        {
        case etEOF:
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        }
    }
    return false;
}

UpdateNode::UpdateNode (const Token& token)
{
    m_tokens.push_back(token);

    if (token == etUPDATE) 
        m_state = eUpdate;
    else
        Failure();
}

bool UpdateNode::PutToken (const Token& token)  
{
    switch (m_state) 
    {
    case eUpdate:
        if (token == etSET)
        {
            m_tokens.push_back(token);
            m_state = eSet;
            return true;
        }
    case eSet:
        if (token == etWHERE)
        {
            m_tokens.push_back(token);
            m_state = eWhere;
            return true;
        }
        // continue
    case eWhere:
        switch (token)
        {
        case etEOF:
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        }
    }
    return false;
}

DeleteNode::DeleteNode (const Token& token)
{
    m_tokens.push_back(token);

    if (token == etDELETE) 
        m_state = eDelete;
    else
        Failure();
}

bool DeleteNode::PutToken (const Token& token)  
{
    switch (m_state) 
    {
    case eDelete:
        if (token == etWHERE)
        {
            m_tokens.push_back(token);
            m_state = eWhere;
            return true;
        }
        // continue
    case eWhere:
        switch (token)
        {
        case etEOF:
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        }
    }
    return false;
}

OpenCursorNode::OpenCursorNode (const Token& token)
{
    m_tokens.push_back(token);

    if (token == etOPEN) 
        m_state = eOpen;
    else
        Failure();
}

bool OpenCursorNode::PutToken (const Token& token)  
{
    switch (m_state) 
    {
    case eOpen:
        if (token == etFOR)
        {
            m_tokens.push_back(token);
            m_state = eFor;
            return true;
        }
        // continue
    case eFor:
        switch (token)
        {
        case etSEMICOLON:
            m_tokens.push_back(token);
            m_state = eComplete;
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////

PlsSqlAnalyzer::PlsSqlAnalyzer ()
{
    m_top   = &m_root;
    m_error = false;
}

PlsSqlAnalyzer::~PlsSqlAnalyzer ()
{
    Clear();
}

void PlsSqlAnalyzer::Clear ()
{ 
    m_error = false; 

    m_top = &m_root; 

	std::vector<SyntaxNode*>::const_iterator it = m_pool.begin();
    for (; it != m_pool.end(); ++it)
        delete *it;

    m_pool.clear();

    m_root.Clear(); 
}

void PlsSqlAnalyzer::OpenLexeme (SyntaxNode* node)
{
    if (m_top)
    {
        //TRACE("Open %s, level = %d, line = %d, col = %d\n", 
        //    node->GetDebugString(), m_top->GetLevel()+1,
        //    node->m_tokens.begin()->line+1, node->m_tokens.begin()->offset+1);
        m_top->AppendChild(node);
        m_top = node;
        m_pool.push_back(node);
    }
}

void PlsSqlAnalyzer::CloseLexeme ()
{
    _ASSERTE(m_top);
    
    if (m_top)
    {
        //TRACE("Close %s, level = %d, line = %d, col = %d\n",
        //    m_top->GetDebugString(), m_top->GetLevel(),
        //    m_top->m_tokens.begin()->line+1, m_top->m_tokens.begin()->offset+1);

        if (!m_top->IsComplete()) 
            m_top->Failure();

        m_top = m_top->GetParent();
    }
}

void PlsSqlAnalyzer::ColapseAll (const Token& token) 
{
    _ASSERTE(m_top);

    if (m_top)
    {
        TRACE("ColapseAll, level = %d, line = %d, col = %d\n", 
            m_top->GetLevel(),token.line+1, token.offset+1);

        while (m_top->GetParent())
            CloseLexeme();
    }
}

void PlsSqlAnalyzer::PutToken (const Token& token)
{
	if (token == etSLASH && token.offset == 0)
	{
		ColapseAll(token);
		m_error = false;
		return;
	}

	if (!m_error)
	{
		bool done = m_top->PutToken(token);

		if (m_top->IsFailure()) //check parsing status
		{
            TRACE("FALURE: token = %s, line = %d, col = %d\n", Token::GetString(token), token.line+1, token.offset+1);
			m_error = true;
			return;
		}

		while (m_top->IsComplete())
			CloseLexeme();
		
		if (!done)
		{
			if (!m_top->IsPLSQL())
			{
				switch (token)
				{
					case etLEFT_ROUND_BRACKET:
						OpenLexeme(new ExpressionNode(token));
						break;
					case etSELECT:
						OpenLexeme(new SelectNode(token));
						break;
				}
			}
			else
			{
				switch (token)
				{
					case etDECLARE:
					case etBEGIN:
						OpenLexeme(new BlockNode(token));
						break;
					case etPACKAGE:
						OpenLexeme(new PackageNode(token));
						break;
					case etFUNCTION:
						OpenLexeme(new ProcedureNode(token));
						break;
					case etPROCEDURE:
						OpenLexeme(new ProcedureNode(token));
						break;
					case etIF:
						OpenLexeme(new SelectionNode(token));
						break;
					case etFOR:
					case etWHILE:
					case etLOOP:
						OpenLexeme(new IterationNode(token));
						break;
					case etLEFT_ROUND_BRACKET:
						OpenLexeme(new ExpressionNode(token));
						break;
					case etOPEN:
						OpenLexeme(new OpenCursorNode(token));
						break;
					case etEXIT:
						// seach for a nearest iteration
						break;

					case etSELECT:
						OpenLexeme(new SelectNode(token));
						break;
					case etINSERT:
						OpenLexeme(new InsertNode(token));
						break;
					case etUPDATE:
						OpenLexeme(new UpdateNode(token));
						break;
					case etDELETE:
						OpenLexeme(new DeleteNode(token));
						break;

					case etALTER:
					case etANALYZE:
					case etCREATE:
					case etDROP:
						;
				}
			}
		}
	}
}

void SyntaxNode::DbgPrintNode ()
{
	std::string indent;
	indent.append(GetLevel(), '>');

	if (GetTokens().size())
		TRACE("%4d%s%s\n", GetTokens().begin()->line + 1,  indent.c_str(), GetDebugString());
	else
		TRACE("%4d%s%s\n", -1, indent.c_str(), GetDebugString());

	if (SyntaxNode* node = GetChild())
        node->DbgPrintNode();

	//@EH
	//indent.clear();
	indent.append(GetLevel(), '<');

	if (m_tokens.size())
		TRACE("%4d%s%s\n", GetTokens().rbegin()->line + 1,  indent.c_str(), GetDebugString());
	else
		TRACE("%4d%s%s\n", -1, indent.c_str(), GetDebugString());

	if (SyntaxNode* node = GetSibling())
        node->DbgPrintNode();
}

};