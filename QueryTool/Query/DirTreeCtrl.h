////////////////////////////////////////////////////////////////////////
//
// File: DirTreeCtrl.h
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
#pragma once

class CDirTreeCtrl : public StyleTreeCtrl
{
public:
	CDirTreeCtrl();
	virtual ~CDirTreeCtrl();

	BOOL    SetSelPath (LPCTSTR strPath );
	CString GetFullPath(HTREEITEM hItem );
	LPCTSTR GetSubPath (LPCTSTR strPath );
	BOOL    DisplayTree(LPCTSTR strRoot, BOOL bFiles = FALSE );
	CImageList& GetSysImageList ();
	void    RefreshFolder(HTREEITEM hItem);
  void    SetFilter(CString& filter);

protected:
	BOOL      IsValidPath(LPCTSTR strPath );
	void      ExpandItem(HTREEITEM hItem, UINT nCode );
	HTREEITEM SearchSiblingItem(HTREEITEM hItem, LPCTSTR strText );
	BOOL      FindSubDir(LPCTSTR strPath );
	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR strPath );
	void      DisplayPath(HTREEITEM hParent, LPCTSTR strPath );
	BOOL      DisplayDrives();
	BOOL      GetSysImgList();
	
  BOOL       m_bFiles;
	CString    m_strError;
  CImageList m_imgList;
	CString    m_strRoot;
  CString    m_filter;

	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

inline
CImageList& 
CDirTreeCtrl::GetSysImageList()
{
  if(!m_imgList.m_hImageList) 
  {
    GetSysImgList();
  }
  return m_imgList;
}

inline void
CDirTreeCtrl::SetFilter(CString& filter)
{
  m_filter = filter;
}

