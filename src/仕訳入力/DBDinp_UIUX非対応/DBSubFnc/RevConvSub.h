#ifndef _REVCONVSUB_INCULUDE_
#define _REVCONVSUB_INCULUDE_



#include "msime.h"


class CRevConvSub
{
public:
	CRevConvSub();
	virtual ~CRevConvSub();

	int		_GetFurigana( DWORD dwOpt, CString& str, CString& kana );

public:
	IFELanguage*	pIFE;
	BOOL	m_bInit;
};




#endif // _REVCONVSUB_INCULUDE_