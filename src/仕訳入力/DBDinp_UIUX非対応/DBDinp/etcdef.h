// 合計問い合わせ テキストテーブルストラクチャ
#ifndef __ETCDEF_H__
#define __ETCDEF_H__

//貸借合計関係
struct _TTLDSP_REC
{
	long	GATUBUN_ATTR;
	CString GATUBUN;
//	char	GATUBUN[10];
	char TUJYO[6],
		SYOG_DEB[6],
		SYOG_CRE[6],
		TAISYAKU[6],
		INPLINE[6];
};

struct _TTLDSP_TBL {
	int m_Mode;
	int TTL_CNT;
	CString TITLE_BAR;		//タイトルバーメッセージ
	CString TITLE[6];		//項目名称
	struct _TTLDSP_REC	DSP_REC[20];	//各月
};

#endif	//end of __ETCDEF_H__