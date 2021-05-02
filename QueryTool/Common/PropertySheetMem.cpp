#include "stdafx.h"
#include "Common/PropertySheetMem.h"

namespace Common {

// CPropertySheetMem
BEGIN_MESSAGE_MAP(CPropertySheetMem, CPropertySheet)
//    ON_COMMAND(IDOK, OnOk)
//    ON_COMMAND(IDCANCEL, OnCancel)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// CPropertySheetMem message handlers
void CPropertySheetMem::OnDestroy()
{
    m_selectPage = GetActiveIndex();
    CPropertySheet::OnDestroy();
}

}; // namespace Common
