#pragma once
#ifndef __PropertySheetMem_h__
#define __PropertySheetMem_h__

namespace Common {

class CPropertySheetMem : public CPropertySheet
{
    UINT& m_selectPage; 
public:
    CPropertySheetMem::CPropertySheetMem(LPCTSTR pszCaption, UINT& selectPage, CWnd* pParentWnd = NULL)
        : CPropertySheet(pszCaption, pParentWnd, selectPage), m_selectPage(selectPage) {}

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDestroy();
};

}; // namespace Common

#endif//__PropertySheetMem_h__