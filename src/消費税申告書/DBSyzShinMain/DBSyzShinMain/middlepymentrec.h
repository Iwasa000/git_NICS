#ifndef  __ICS_MIDDLEPAYMENTREC__
#define  __ICS_MIDDLEPAYMENTREC__

#pragma pack( push, 2 )

struct	MPAYMENTVAL	{
	
	char	KMPval[6],		// 中間納付税額　国税
			TMPval[6];		// 中間納付税額　地方税

	int		kubun;
	int		smd,emd;

	int		edit;

	MPAYMENTVAL()
	{
		memset( KMPval, 0, sizeof(KMPval) );	// 中間納付税額　国税
		memset( TMPval, 0, sizeof(TMPval) );	// 中間納付税額　地方税

		kubun = 0;
		smd = emd =0;
		edit = 1;
	}
	
	MPAYMENTVAL(int k,int s, int e,char* kmp,char* tmp) : kubun(k),smd(s),emd(e)
	{
		memmove( KMPval, kmp, 6 );	// 中間納付税額　国税
		memmove( TMPval, tmp, 6 );	// 中間納付税額　地方税
		edit = 0;
	}

};

#pragma pack( pop )

#endif		// __ICS_MIDDLEPAYMENTREC__