////////////////////////////////////////////////////////////////////////
//
// File: DirTreeCtrl.cpp
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "pch.h"
#include "WinException.h"
#include "DirTreeCtrl.h"
#include "SortStringArray.h"
#include "QueryTool.h"
#include "Common\ExceptionHelper.h"
#include "Common\DocManagerExt.h"

#define SUBDIR_ALWAYS 1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void 
GetFileImage (LPCTSTR path, TVITEM& item, UINT flags = SHGFI_ICON|SHGFI_SMALLICON/*|SHGFI_OPENICON*/)
{
	SHFILEINFO shFinfo;
  if (SHGetFileInfo(path, 0, &shFinfo, sizeof(shFinfo), flags))
  {
    item.mask |= TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	  item.iImage = item.iSelectedImage = shFinfo.iIcon;
    DestroyIcon(shFinfo.hIcon); // we only need the index of the system image list
  }
}

/////////////////////////////////////////////////////////////////////////////
// CDirTreeCtrl

CDirTreeCtrl::CDirTreeCtrl()
{
}

CDirTreeCtrl::~CDirTreeCtrl()
{
    m_imgList.Detach();
}

BEGIN_MESSAGE_MAP(CDirTreeCtrl, StyleTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL 
CDirTreeCtrl::DisplayTree(LPCTSTR strRoot, BOOL bFiles)
{
	DWORD dwStyle = GetStyle();   // read the window style
	if ( dwStyle & TVS_EDITLABELS ) 
	{
		// Don't allow the user to edit ItemLabels
		ModifyStyle( TVS_EDITLABELS , 0 );
	}
	
	// Display the DirTree with the Rootname e.g. C:\
	// if Rootname == NULL then Display all Drives on this PC
    // First, we need the system-ImageList
	
	DeleteAllItems();

	if ( !GetSysImgList() )
  {
    return FALSE;
  }
  m_bFiles = bFiles;  // if TRUE, Display Path- and Filenames 
	if ( strRoot == NULL || strRoot[0] == '\0' )
	{
		if ( !DisplayDrives() )
    {
      return FALSE;
    }
		m_strRoot = _T("");
	}
  else
	{
		m_strRoot = strRoot;
		if ( m_strRoot.Right(1) != '\\' )
    {
      m_strRoot += _T("\\");
    }
		HTREEITEM hParent = AddItem( TVI_ROOT, m_strRoot );

    if (!hParent) // 11.06.2002   minimal error handling has been added
    {
      MessageBeep((UINT)-1);
      AfxMessageBox(CString(_T("Cannot show \"")) + m_strRoot + _T("\"."), MB_OK|MB_ICONSTOP);
	    return FALSE;	
    }
		DisplayPath( hParent, strRoot );
	}
	return TRUE;	
}

BOOL 
CDirTreeCtrl::GetSysImgList()
{
    // 17.11.02   Win98 support
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = (HIMAGELIST)SHGetFileInfo(_T(""), 0, &shFinfo, sizeof(shFinfo), SHGFI_SMALLICON|SHGFI_SYSICONINDEX);

	if ( !hImgList )
	{
		m_strError = _T("Cannot retrieve the Handle of SystemImageList!");
		return FALSE;
	}

  ::SendMessage(m_hWnd, TVM_SETIMAGELIST, (UINT)TVSIL_NORMAL, (LPARAM)hImgList);

	if (m_imgList.m_hImageList)
	{
		m_imgList.Detach();
	}
  m_imgList.Attach(hImgList);

	return TRUE;   // OK
}

BOOL 
CDirTreeCtrl::DisplayDrives()
{
	//
	// Displaying the Available Drives on this PC
	// This are the First Items in the TreeCtrl
	//
	DeleteAllItems();
	TCHAR  szDrives[128];
	TCHAR* pDrive;

	if ( !GetLogicalDriveStrings( sizeof(szDrives), szDrives ) )
	{
		m_strError = _T("Error Getting Logical DriveStrings!");
		return FALSE;
	}

	pDrive = szDrives;
	while( *pDrive )
	{
		HTREEITEM hParent = AddItem( TVI_ROOT, pDrive );
    if(hParent)
    {
			if (FindSubDir(pDrive))
			{
				InsertItem(_T(""), 0, 0, hParent);
			}
			pDrive += _tcslen( pDrive ) + 1;
    }
    else
    {
      MessageBeep((UINT)-1);
      AfxMessageBox(CString(_T("Cannot show \"")) + pDrive + _T("\"."), MB_OK|MB_ICONSTOP);
      return FALSE;
    }
	}
	return TRUE;
}

void 
CDirTreeCtrl::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
{
	//
	// Displaying the Path in the TreeCtrl
	//
	CFileFind find;
	CString   strPathFiles = strPath;
	BOOL      bFind;
	CSortStringArray strDirArray;
	CSortStringArray strFileArray;
	
	if(strPathFiles.Right(1) != _T("\\") )
  {
    strPathFiles += _T("\\");
  }
	strPathFiles += _T("*.*");

	bFind = find.FindFile( strPathFiles );
  if (!bFind) 
  {
    throw new CWinException();
  }
	while ( bFind )
	{
		bFind = find.FindNextFile();

    if(!find.IsHidden())
    {
      if ( find.IsDirectory() && !find.IsDots() )
		  {		
			  strDirArray.Add( find.GetFilePath() );
		  }
		  if ( !find.IsDirectory() && m_bFiles )
      {
        CString path = find.GetFilePath();
        if(m_filter.IsEmpty())
        {
          strFileArray.Add(path);
        }
        else
        {
          int len = m_filter.GetLength();
          if(path.Right(len) == m_filter)
          {
            strFileArray.Add(path);
          }
        }
      }
    }
	}
	strDirArray.Sort();
	SetRedraw( FALSE );
	CWaitCursor wait;

  int i = 0;
	for (i = 0; i < strDirArray.GetSize(); i++ )
	{
		HTREEITEM hItem = AddItem( hParent, strDirArray.GetAt(i) );

    if(hItem) // 11.06.2002   minimal error handling has been added
    {
		  if(FindSubDir( strDirArray.GetAt(i) ) )
      {
        InsertItem( _T(""), 0, 0, hItem );
      }
     }
     else
     {
       MessageBeep((UINT)-1);
       if (AfxMessageBox(CString(_T("Cannot show \"")) + strDirArray.GetAt(i) + _T("\"."), MB_OKCANCEL|MB_ICONSTOP) == IDCANCEL)
       {
         SetRedraw(TRUE);
         return;
       }
     }
	}

	if ( m_bFiles )
	{
		strFileArray.Sort();
		for ( i = 0; i < strFileArray.GetSize(); i++ )
		{
			HTREEITEM hItem = AddItem( hParent, strFileArray.GetAt(i) );
			
      if (!hItem) // 11.06.2002   minimal error handling has been added
      {
        MessageBeep((UINT)-1);
        if (AfxMessageBox(CString(_T("Cannot show \"")) + strFileArray.GetAt(i) + _T("\"."), MB_OKCANCEL|MB_ICONSTOP) == IDCANCEL)
        {
          SetRedraw(TRUE);
          return;
        }
      }
		}
	}

  //
  // change item image to an open folder
  //
  TVITEM item;
  memset(&item, 0, sizeof(item));
  item.hItem = hParent;
  GetFileImage (strPath, item, SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON);
  SetItem(&item);
    
	SetRedraw( TRUE );
}

HTREEITEM 
CDirTreeCtrl::AddItem (HTREEITEM hParent, LPCTSTR strPath)
{
	// Adding the Item to the TreeCtrl with the current Icons
  CString strTemp = strPath;
    
	if ( strTemp.Right(1) != '\\' )
  {
    strTemp += _T("\\");
  }
  TVINSERTSTRUCT item;
  memset(&item, 0, sizeof(hParent));
  item.hParent = hParent;
  item.hInsertAfter = TVI_LAST;
    
  item.item.mask = TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
  GetFileImage(strTemp, item.item, SHGFI_ICON|SHGFI_SMALLICON);
	
	if (strTemp.Right(1) == _T("\\"))
  {
    strTemp.SetAt(strTemp.GetLength() - 1, '\0');
  }
  if (hParent == TVI_ROOT)
  {
    item.item.pszText = (LPTSTR)(LPCTSTR)strTemp;
  }
  else
  {
    item.item.pszText = (LPTSTR)(LPCTSTR)GetSubPath(strTemp);
  }
	return this->InsertItem(&item);
}

LPCTSTR 
CDirTreeCtrl::GetSubPath(LPCTSTR strPath)
{
	//
	// getting the last SubPath from a PathString
	// e.g. C:\temp\readme.txt
	// the result = readme.txt
	static CString strTemp;
	int     iPos;

	strTemp = strPath;
	if ( strTemp.Right(1) == '\\' )
  {
    strTemp.SetAt( strTemp.GetLength() - 1, '\0' );
  }
	iPos = strTemp.ReverseFind( '\\' );
	if ( iPos != -1 )
  {
    strTemp = strTemp.Mid( iPos + 1);
  }
	return (LPCTSTR)strTemp;
}

#if (SUBDIR_ALWAYS==1)

BOOL 
CDirTreeCtrl::FindSubDir (LPCTSTR)
{
    return TRUE;
}

#else // SUBDIR_ALWAYS

BOOL CDirTreeCtrl::FindSubDir (LPCTSTR strPath)
{
	CFileFind find;
	CString   strTemp = strPath;
	BOOL      bFind;

	if ( strTemp[strTemp.GetLength()-1] == '\\' )
		strTemp += _T("*.*");
	else
		strTemp += _T("\\*.*");
		
	bFind = find.FindFile( strTemp );
	
	
	while ( bFind )
	{
		bFind = find.FindNextFile();

		if ( find.IsDirectory() && !find.IsDots() )
		{
			return TRUE;
		}
		if ( !find.IsDirectory() && m_bFiles && !find.IsHidden() )
			return TRUE;
		
	}

	return FALSE;
}

#endif//SUBDIR_ALWAYS

void 
CDirTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString strPath;
	 
	if ( pNMTreeView->itemNew.state & TVIS_EXPANDED )
	{
		ExpandItem( pNMTreeView->itemNew.hItem, TVE_EXPAND );
	}
	else
	{
		//
		// Delete the Items, but leave one there, for 
		// expanding the item next time
		//
		HTREEITEM hChild = GetChildItem( pNMTreeView->itemNew.hItem );
				
		while ( hChild ) 
		{
			DeleteItem( hChild );
			hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		}
		InsertItem(_T(""), pNMTreeView->itemNew.hItem );
    
    TVITEM item;
    memset(&item, 0, sizeof(item));
    item.hItem = pNMTreeView->itemNew.hItem;
    GetFileImage(GetFullPath(item.hItem), item, SHGFI_ICON|SHGFI_SMALLICON);
    SetItem(&item);
	}

	*pResult = 0;
}

CString 
CDirTreeCtrl::GetFullPath(HTREEITEM hItem)
{
	// get the Full Path of the item
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;

	strReturn = _T("");

	while ( hParent )
	{
		
		strTemp  = GetItemText( hParent );
		strTemp += _T("\\");
		strReturn = strTemp + strReturn;
		hParent = GetParentItem( hParent );
	}
    
	strReturn.TrimRight( '\\' );

    return strReturn;

}

BOOL 
CDirTreeCtrl::SetSelPath(LPCTSTR strPath)
{
	// Setting the Selection in the Tree
	HTREEITEM hParent  = TVI_ROOT;
	int       iLen    = (int)_tcslen(strPath) + 2;
	TCHAR*     pszPath = new TCHAR[iLen];
	TCHAR*     pPath   = pszPath;
	BOOL      bRet    = FALSE;
    
	if ( !IsValidPath( strPath ) )
	{
		delete [] pszPath; // this must be added 29.03.99
		return FALSE;
	}
		
	_tcscpy_s( pszPath,iLen, strPath );
	_tcsupr_s( pszPath,iLen);
	
	if (pszPath[_tcslen(pszPath) - 1] != '\\')
	{
		_tcscat_s(pszPath,iLen, _T("\\"));
	}
	int iLen2 = (int)_tcslen( pszPath );
	
	for(WORD i = 0; i < iLen2; i++ )
	{
		if(pszPath[i] == '\\' )
		{
			SetRedraw( FALSE );
			pszPath[i] = _T('\0');
			hParent = SearchSiblingItem( hParent, pPath );
			if(!hParent)  // Not found!
			{
				break;
			}
			else
			{				
				// Info:
				// the notification OnItemExpanded 
				// will not called every time 
				// after the call Expand. 
				// You must call Expand with TVE_COLLAPSE | TVE_COLLAPSERESET
				// to Reset the TVIS_EXPANDEDONCE Flag
				
				UINT uState;
				uState = GetItemState( hParent, TVIS_EXPANDEDONCE );
				if ( uState )
				{
					Expand( hParent, TVE_EXPAND );
					Expand( hParent, TVE_COLLAPSE | TVE_COLLAPSERESET );
					InsertItem(_T(""), hParent ); // insert a blank child-item
					Expand( hParent, TVE_EXPAND ); // now, expand send a notification
				}
				else
				{
					Expand(hParent, TVE_EXPAND);
				}
			}
			pPath += _tcslen(pPath) + 1;
		}
	}

	delete [] pszPath;
	
	if(hParent) // Ok the last subpath was found
	{		
    EnsureVisible(hParent); // 06.08.2002 bug fix, make sure that selected item is visible
    SelectItem(hParent ); // select the last expanded item
    bRet = TRUE;
  }
  else
  {
    bRet = FALSE;
  }
  SetRedraw( TRUE );
  return bRet;
}

HTREEITEM 
CDirTreeCtrl::SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText)
{
	HTREEITEM hFound = GetChildItem( hItem );
	CString   strTemp;
	while ( hFound )
	{
		strTemp = GetItemText( hFound );
    strTemp.MakeUpper();
		if (strTemp == strText)
		{
			return hFound;
		}
		hFound = GetNextItem( hFound, TVGN_NEXT );
  }
  return NULL;
}

void 
CDirTreeCtrl::ExpandItem(HTREEITEM hItem, UINT nCode)
{	
	CString strPath;
	
	if ( nCode == TVE_EXPAND )
	{
		HTREEITEM hChild = GetChildItem( hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( hItem );
		}
        
		strPath = GetFullPath( hItem );
		DisplayPath( hItem, strPath );
	}
}

BOOL 
CDirTreeCtrl::IsValidPath(LPCTSTR strPath)
{
  // 06.08.2002 bug fix, Win95 compatibility
  return (GetFileAttributes(strPath) != 0xFFFFFFFF) ? TRUE : FALSE;
}

void CDirTreeCtrl::RefreshFolder (HTREEITEM hItem)
{
	UINT uState = GetItemState(hItem, TVIS_EXPANDEDONCE|TVIS_EXPANDED);

  if (uState & TVIS_EXPANDED)
  {
	  SetRedraw(FALSE);

	  if(uState & TVIS_EXPANDEDONCE)
	  {
		  Expand( hItem, TVE_EXPAND );
		  Expand( hItem, TVE_COLLAPSE | TVE_COLLAPSERESET );
		  InsertItem(_T(""), hItem ); // insert a blank child-item
		  Expand( hItem, TVE_EXPAND ); // now, expand send a notification
	  }
	  SetRedraw(TRUE);
  }
}

