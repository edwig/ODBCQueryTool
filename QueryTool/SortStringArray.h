// SortStringArray.h: Schnittstelle f�r die Klasse CSortStringArray.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CSortStringArray : public CStringArray  
{
public:
	void Sort();
private: 
	BOOL CompareAndSwap( int pos );

};

