///////////////////////////////////////////////////////////////////////////////
//
// Manual:
// =======
//
// RegExp exp;
// exp.SetUseCase(true/false); // To search case (in)dependent
// exp.SetExp("pattern*");     // Can alsoo be done directly
// 
// RegExp exp1("Searchpattern*",true);
//
// Walk the data and match each string
// ...
//    if(exp.Match(<string to search in>) >= 0)
//    {
//      // Ok, found our match
//
#include "StdAfx.h"

#ifndef RegExp_h
#define RegExp_h

class RegExp
{
public:
  // Constructors with optional regular-expression and use-case 
	RegExp();
	RegExp(const CString& regexp);
	RegExp(const CString& regexp,bool reg);
  // Destructor
	~RegExp();

	// SETTINGS
  // Set the use-case
	bool SetUseCase(bool useCase);
  // Set the regular expression to match for
	bool SetExp(const CString& regexp);

  // MATCHING
  // Returns the offset of the match or -1
	int  Match(CString& match);
	int  Match(LPCTSTR match);
  // Match the 'pos' position in the regular expression
	bool MatchPos(int pos,TCHAR car);

  // STATUS (after searching)
  int GetMatchPos();
  int GetMatchLen();
  int GetMatchEnd();
private:
	// Implementation
	void    set_escape(TCHAR escape,TCHAR zero_one,TCHAR closure);
	int     isfactor(TCHAR c);
	LPTSTR  strccat(TCHAR *s,TCHAR ch);

	// parse and compile expression
	LPTSTR  makepat(LPTSTR re,LPTSTR pat);
	LPTSTR  parse_expression(void);
	LPTSTR  parse_term(void);
	LPTSTR  parse_factor(void);
	TCHAR   parse_escape(void);
	int     parse_closure(LPTSTR pat,TCHAR c);
	LPTSTR  parse_cc1(void);
	LPTSTR  parse_dash(LPTSTR pat,TCHAR ch);

	// match expression
	LPTSTR  str_match(LPTSTR s,LPTSTR re);
	LPTSTR  re_match(LPTSTR s,LPTSTR pat);
	bool    re_matchpos(TCHAR car,LPTSTR pat,int pos);
	int     match_term(int inx,LPTSTR s,LPTSTR pat,int aantalpos = 0);
	int     match_or(int inx,LPTSTR s,LPTSTR pat);
	LPTSTR  skip_term(LPTSTR pat,int aantalpos = 0);
	int     match_0_1(int inx,LPTSTR s,LPTSTR pat);
	int     match_closure(int inx,LPTSTR s,LPTSTR pat,LPTSTR clopat);
	short   match_cc1(TCHAR c,LPTSTR pat);

private:
	CString m_regExp;	
	CString m_regExpPat;

	// Special
	TCHAR M_CLOSURE;          /* Closure of line		*/
	TCHAR M_ZERO_ONE;         /* One or more characters	*/
	TCHAR M_ESCAPE;           /* Escape character		*/
	TCHAR m_nfac_chars[8];
	// Helpers

	int  m_rstart  { 0 };         /* Start  of matched substring  */
	int  m_rlength { 0 };         /* Length of matched substring  */
	bool m_use_case{ true };      /* Case is significant          */
	bool m_sql     { true };      /* Translate as SQL             */
                                // ?  ==> .
                                // *  ==> .*
  LPTSTR m_re_ptr{ nullptr };   /* Global for pattern building  */
	LPTSTR m_s_end { nullptr };   /* Points to last char matched  */
	LPTSTR m_c     { nullptr };   /* String continuation pointer  */

};  // RegExp

inline
int RegExp::GetMatchPos()
{
  return m_rstart;
}

inline int
RegExp::GetMatchLen()
{
  return m_rlength;
}

inline int
RegExp::GetMatchEnd()
{
  return (m_rstart + m_rlength);
}

#endif

