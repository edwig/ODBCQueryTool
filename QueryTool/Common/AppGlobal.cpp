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
#include "COMMON\AppGlobal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Common
{

static HWND hWndStatus;

HWND SetStatusHwnd (HWND hWnd)
{
    HWND _hWnd = hWndStatus;
    hWndStatus = hWnd;
    return _hWnd;
}

void SetStatusText (const char* text, BOOL update)
{
    if (hWndStatus)
    {
        CWnd* wnd = CWnd::FromHandle(hWndStatus);
        if (wnd && wnd->IsKindOf(RUNTIME_CLASS(CStatusBar)))
        {
            ((CStatusBar*)wnd)->SetWindowText(text);
            if (update)
                ((CStatusBar*)wnd)->UpdateWindow();
        }
    }
}


}//namespace Common
