#include "StdAfx.h"
#include "SortStringArray.h"

void CSortStringArray::Sort()
{
	BOOL	bNotDone=TRUE;
	int		pos=0;

	while(bNotDone)	{
		bNotDone = FALSE;
		for(pos=0; pos<GetUpperBound(); pos++)	{
			bNotDone |= CompareAndSwap(pos);
		}
	}
}

BOOL CSortStringArray::CompareAndSwap(int pos)
{
	CString	temp=_T("");
//	CString	cst1=_T(""),cst2=_T("");
	int		posFirst=pos;
	int		posNext=pos+1;

//	cst1 = GetAt(posFirst);
//	cst2 = GetAt(posNext);

	if(GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)	{
//	if(GetAt(posFirst).Compare(GetAt(posNext)) > 0)	{
//	cst1 = GetAt(posFirst);
//	cst2 = GetAt(posNext);
//	if(cst1 > cst2)	{
		temp = GetAt(posFirst);
		SetAt(posFirst, GetAt(posNext));
		SetAt(posNext, temp);
		return(TRUE);
	}
	return(FALSE);
}
