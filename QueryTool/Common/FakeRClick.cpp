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
#include "Common/FakeRClick.h"


BEGIN_MESSAGE_MAP(CFakeRClick, CWnd)
    ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CFakeRClick::OnRButtonDown (UINT /*nFlags*/, CPoint)
{
    NMHDR nmhr;
    if (GetParent() && GetParent()->GetParent())
    {
        nmhr.hwndFrom = GetParent()->m_hWnd;
        nmhr.idFrom   = GetWindowLong(nmhr.hwndFrom, GWL_ID);
        nmhr.code     = NM_RCLICK;
        GetParent()->GetParent()->SendMessage(WM_NOTIFY, nmhr.idFrom, (LPARAM)&nmhr);
    }
}

