//////////////////////////////////////////////////////////////////////////
//
// Getting the framework settings for this library/program
// Uses the $(SolutionDir)Framework.h file for configuration of 
// the MFC CString or the std::string SMX_String
//
#pragma once

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely-used stuff from Windows headers

#include "..\Framework.h"

//////////////////////////////////////////////////////////////////////////
//
// Can be extended beyond this point with extra MFC requirements
//
//////////////////////////////////////////////////////////////////////////

// IF We want SQLInfoTree to work, we need the whole shebang from MFC
// IF we want SQLDatasetXLS to work, we need CArchive for oldstyle *.XLS
//    so we also need to include the full set
// So we include the full MFC set, otherwise: we just need CString
//
// #include <afxwin.h>             // MFC core and standard components
