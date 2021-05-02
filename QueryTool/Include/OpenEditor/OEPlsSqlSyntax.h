/* 
    Copyright (C) 2003 Aleksey Kochetov

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

#ifndef __OEPlsSqlSyntax_h__
#define __OEPlsSqlSyntax_h__

#include <string>
#include <vector>
#include "Common/Fastmap.h"
#include "OpenEditor/OEHelpers.h"
#include "OpenEditor/OEPlsSqlParser.h"


namespace OpenEditor
{ 
    using std::string;
    using std::vector;

    class EditContext;

    enum ELexeme
    {
        elUNIT,
        elBLOCK,
        elPACKAGE,
        elPROCEDURE,
        elSELECTION,
        elITERATION,
        elEXPRESSION,
        elSELECT,
        elINSERT,
        elUPDATE,
        elDELETE,
        elOPEN_CURSOR
    };

    //
    // Attention: a tree builder is responsinle for destroing all nodes!
    //
    class SyntaxNode
    {
    public:
        SyntaxNode ();
        virtual ~SyntaxNode ();

        virtual bool PutToken (const Token&) = 0;

        virtual ELexeme GetType () const = 0;
        virtual bool IsComplete () const = 0;
        virtual bool IsSQL      () const = 0;
        virtual bool IsPLSQL    () const = 0;
        virtual const char* GetDebugString () const = 0;

        int GetLevel () const;
        virtual bool StopSign (EToken token) const;
        virtual bool FindToken (int line, int offset, const SyntaxNode*& node, int& index) const;
        virtual void GetToken (int index, int& line, int& offset, int& length) const;
        virtual void GetNextToken (int index, int& line, int& offset, int& length, bool& backward) const;
        
        void Failure()                              { m_failure = true; }
        bool IsFailure () const                     { return m_failure; }
        const vector<Token> GetTokens ()            { return m_tokens; }

        void AppendSibling (SyntaxNode* sibling);   
        void AppendChild  (SyntaxNode* child);  

        SyntaxNode* GetSibling ()                   { return m_sibling; }
        SyntaxNode* GetParent ()                    { return m_parent; }
        SyntaxNode* GetChild ()                     { return m_child; }

        const SyntaxNode* GetSibling () const       { return m_sibling; }
        const SyntaxNode* GetParent () const        { return m_parent; }
        const SyntaxNode* GetChild () const         { return m_child; }

        SyntaxNode* DetachSibling ()                { SyntaxNode* sibling = m_sibling; m_sibling = 0; return sibling; }

        void DbgPrintNode ();

    protected:
        bool m_failure;
        vector<Token> m_tokens;
        SyntaxNode *m_sibling, *m_parent, *m_child;
    };

    class UnitNode : public SyntaxNode
    {
    public:
        ~UnitNode ();

        void Clear ();

        virtual bool PutToken (const Token&)        { return false; };

        virtual ELexeme GetType () const { return elUNIT; }
        virtual bool IsComplete () const { return false; }
        virtual bool IsSQL      () const { return true; }
        virtual bool IsPLSQL    () const { return true; }
        virtual const char* GetDebugString () const { return "Unit"; };

        virtual bool FindToken (int line, int offset, const SyntaxNode*& node, int& index) const;
    };

    class BlockNode : public SyntaxNode
    {
        enum State { eDeclare, eBegin, eException, eEnd, eComplete };
        State m_state;

    public:
        BlockNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elBLOCK; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return false; }
        virtual bool IsPLSQL    () const { return true; }
        virtual const char* GetDebugString () const { return "Block"; };
    };

    class PackageNode : public SyntaxNode
    {
        enum State { ePackage, eBody, eName1, eNameSeparator, eName2, eIs, eBegin, eException, eEnd, eComplete };
        State m_state;
        bool m_body;

    public:
        PackageNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elPACKAGE; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return false; }
        virtual bool IsPLSQL    () const { return true; }
        virtual const char* GetDebugString () const { return m_body ? "Package" : "Package Body"; };
    };

    class ProcedureNode : public SyntaxNode
    {
        enum State { eProcedure, eName1, eNameSeparator, eName2, eArguments, eIs, eBegin, eException, eEnd, eComplete, eLanguage };
        State m_state;
        bool m_declaration;

    public:
        ProcedureNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elPROCEDURE; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return false; }
        virtual bool IsPLSQL    () const { return true; }
        virtual const char* GetDebugString () const { return m_tokens.begin()->token == etPROCEDURE ? "Procedure" : "Function"; };
    };

    class SelectionNode : public SyntaxNode
    {
        enum State { eIf, eElsIf, eElse, eEnd, eEndIf, eComplete };
        State m_state;

    public:
        SelectionNode (const Token& token);

        virtual bool PutToken (const Token& token)  ;

        virtual ELexeme GetType () const { return elSELECTION; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return false; }
        virtual bool IsPLSQL    () const { return true; }
        virtual const char* GetDebugString () const { return "Selection"; };
    };

    class IterationNode : public SyntaxNode
    {
        enum State { eFor, eWhile, eLoop, eEnd, eEndLoop, eComplete };
        State m_state;

    public:
        IterationNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elITERATION; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return false; }
        virtual bool IsPLSQL    () const { return true; }
        virtual const char* GetDebugString () const { return "Iteration"; };
    };


    class ExpressionNode : public SyntaxNode
    {
        enum State { eOpen, eComplete };
        State m_state;

    public:
        ExpressionNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elEXPRESSION; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return m_parent ? m_parent->IsSQL() : true; }
        virtual bool IsPLSQL    () const { return m_parent ? m_parent->IsPLSQL() : true; }
        virtual const char* GetDebugString () const { return "Expression"; };
    };

    class SelectNode : public SyntaxNode
    {
        enum State { eSelect, eFrom, eWhere, eComplete };
        State m_state;

    public:
        SelectNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elSELECT; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return true; }
        virtual bool IsPLSQL    () const { return false; }
        virtual const char* GetDebugString () const { return "Select"; };
    };

    class InsertNode : public SyntaxNode
    {
        enum State { eInsert, eComplete };
        State m_state;

    public:
        InsertNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elINSERT; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return true; }
        virtual bool IsPLSQL    () const { return false; }
        virtual const char* GetDebugString () const { return "Insert"; };
    };

    class UpdateNode : public SyntaxNode
    {
        enum State { eUpdate, eSet, eWhere, eComplete };
        State m_state;

    public:
        UpdateNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elUPDATE; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return true; }
        virtual bool IsPLSQL    () const { return false; }
        virtual const char* GetDebugString () const { return "Update"; };
    };

    class DeleteNode : public SyntaxNode
    {
        enum State { eDelete, eWhere, eComplete };
        State m_state;

    public:
        DeleteNode (const Token& token);

        virtual bool PutToken (const Token& token);

        virtual ELexeme GetType () const { return elUPDATE; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return true; }
        virtual bool IsPLSQL    () const { return false; }
        virtual const char* GetDebugString () const { return "Update"; };
    };

    class OpenCursorNode : public SyntaxNode
    {
        enum State { eOpen, eFor, eComplete };
        State m_state;

    public:
        OpenCursorNode (const Token& token);

        virtual bool PutToken (const Token& token);        

        virtual ELexeme GetType () const { return elOPEN_CURSOR; }
        virtual bool IsComplete () const { return m_state == eComplete; }
        virtual bool IsSQL      () const { return true; }
        virtual bool IsPLSQL    () const { return false; }
        virtual const char* GetDebugString () const { return "Open Cursor"; };
    };


    class PlsSqlAnalyzer : public SyntaxAnalyser
    {
    public:
        PlsSqlAnalyzer ();
        ~PlsSqlAnalyzer ();

        void PutToken (const Token&);

        bool FindToken (int line, int offset, const SyntaxNode*& node, int& index) const;
        void Clear ();

    private:
	    UnitNode    m_root;
        SyntaxNode* m_top;
        vector<SyntaxNode*> m_pool;

        bool m_error;

        void OpenLexeme (SyntaxNode*);
        void CloseLexeme ();
        void ColapseAll (const Token&) ;
    };

    inline
    bool PlsSqlAnalyzer::FindToken (int line, int offset, const SyntaxNode*& node, int& index) const
        { return m_root.FindToken(line, offset, node, index); }
};

#endif//__OEPlsSqlSyntax_h__
