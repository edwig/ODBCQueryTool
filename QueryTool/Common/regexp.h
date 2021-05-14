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
	int  Match(LPCSTR match);
  // Match the 'pos' position in the regular expression
	bool MatchPos(int pos,char car);

  // STATUS (after searching)
  int GetMatchPos();
  int GetMatchLen();
  int GetMatchEnd();
private:
	// Implementation
	void	set_escape(char escape,char zero_one,char closure);
	int   isfactor(char c);
	char* strccat(char *s,char ch);

	// parse and compile expression
	char* makepat(char *re,char *pat);
	char* parse_expression(void);
	char* parse_term(void);
	char* parse_factor(void);
	char  parse_escape(void);
	int   parse_closure(char *pat,char c);
	char* parse_cc1(void);
	char* parse_dash(char *pat,char ch);

	// match expression
	char* str_match(char *s,char *re);
	char* re_match(char *s,char *pat);
	bool  re_matchpos(char car,char *pat,int pos);
	int   match_term(int inx,char *s,char *pat,int aantalpos = 0);
	int   match_or(int inx,char *s,char *pat);
	char* skip_term(char *pat,int aantalpos = 0);
	int   match_0_1(int inx,char *s,char *pat);
	int   match_closure(int inx,char *s,char *pat,char *clopat);
	short match_cc1(char c,char *pat);

private:
	CString m_regExp;	
	CString m_regExpPat;

	// Special
	char	M_CLOSURE;       /* Closure of line		*/
	char	M_ZERO_ONE;      /* One or more characters	*/
	char	M_ESCAPE;        /* Escape character		*/
	char  m_nfac_chars[7];
	// Hulpjes

	int  m_rstart;	      /* Start  of matched substring		*/
	int  m_rlength;	      /* Length of matched substring		*/
	bool m_use_case;      /* Case is significant			*/
	bool m_sql;           /* vertaling als sql			*/
                              // ?  ==> .
                              // *  ==> .*
	char *m_re_ptr;	      /* Global for pattern building		*/
	char *m_s_end;	      /* Points to last char matched		*/
	char *m_c;	          /* String continuation pointer		*/

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

