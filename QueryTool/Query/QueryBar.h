/*
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

#ifndef QUERYBAR_H
#define QUERYBAR_H

#include "stdafx.h"
#include "QueryTool.h"
#include "MultConnectionsDlg.h"

class QueryBar : public CToolBar
{
  DECLARE_DYNCREATE(QueryBar);
public:
   QueryBar();
  ~QueryBar();
 
  void OnSize(UINT nType, int cx, int cy);

  CFont   m_font;
  CStatic m_textQuery;
  CStatic m_textStatistics;
  CStatic m_textOuptput;
  CStatic m_textHistory;
};

#endif