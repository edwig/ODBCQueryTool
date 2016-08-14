/*
    Copyright (C) 2004 Edwig Huisman

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
#ifndef CODEER_INCLUDE
#define CODEER_INCLUDE

#include "stdafx.h"

class CCodeer : public CObject
{
public:
  CCodeer(long p_schuifregister = 0);
  char CodeerChar(char p_kar);
  char DeCodeerChar(char p_kar);
  void Reset(long p_schuifregister = 0);

  CString CodeerString     (CString p_str,int p_stap = 10);
  CString DeCodeerString   (CString p_str,int p_stap = 10);
  CString CodeerStringHex  (CString p_str,int p_stap = 10);
  CString DeCodeerStringHex(CString p_str,int p_stap = 10);
  
  CString CodeerString     (CString p_str,CString p_mask);
  CString CodeerStringHex  (CString p_str,CString p_mask);
  CString DeCodeerString   (CString p_str,CString p_mask);
  CString DeCodeerStringHex(CString p_str,CString p_mask);
private:
  static unsigned long m_polynomen[20];
  unsigned long m_schuifregister;
  int m_poltel;
};

#endif