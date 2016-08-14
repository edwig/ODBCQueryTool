// SortStringArray.h: Schnittstelle für die Klasse CSortStringArray.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTSTRINGARRAY_H__64C6B322_B44E_11D2_955E_204C4F4F5020__INCLUDED_)
#define AFX_SORTSTRINGARRAY_H__64C6B322_B44E_11D2_955E_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSortStringArray : public CStringArray  
{
public:
	void Sort();
private: 
	BOOL CompareAndSwap( int pos );

};

#endif // !defined(AFX_SORTSTRINGARRAY_H__64C6B322_B44E_11D2_955E_204C4F4F5020__INCLUDED_)
