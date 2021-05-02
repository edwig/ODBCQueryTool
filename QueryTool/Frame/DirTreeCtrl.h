#ifndef __DIRTREECTRL_H__
#define __DIRTREECTRL_H__

class CDirTreeCtrl : public CTreeCtrl
{
public:
	CDirTreeCtrl();
	virtual ~CDirTreeCtrl();

	BOOL    SetSelPath( LPCTSTR strPath );
	CString GetFullPath( HTREEITEM hItem );
	LPCTSTR GetSubPath( LPCTSTR strPath );
	BOOL    DisplayTree( LPCTSTR strRoot, BOOL bFiles = FALSE );
	CImageList& GetSysImageList ();
	void    RefreshFolder (HTREEITEM hItem);
  void    SetFilter(CString& filter);

protected:
	BOOL      IsValidPath( LPCTSTR strPath );
	void      ExpandItem( HTREEITEM hItem, UINT nCode );
	HTREEITEM SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText );
	BOOL      FindSubDir( LPCTSTR strPath );
	HTREEITEM AddItem( HTREEITEM hParent, LPCTSTR strPath );
	void      DisplayPath( HTREEITEM hParent, LPCTSTR strPath );
	BOOL      DisplayDrives();
	BOOL      GetSysImgList();
	
  BOOL       m_bFiles;
	CString    m_strError;
  CImageList m_imgList;
	CString    m_strRoot;
  CString    m_filter;

	//{{AFX_VIRTUAL(CDirTreeCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDirTreeCtrl)
	afx_msg void OnItemexpanded (NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

inline
CImageList& 
CDirTreeCtrl::GetSysImageList()
{
  if (!m_imgList.m_hImageList) 
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

#endif//__DIRTREECTRL_H__
