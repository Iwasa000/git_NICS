#ifndef _ETCMSG_H_
#define _ETCMSG_H_

typedef struct tag_BRN_REC {
	unsigned short	kn50;	// 50音
	int			 	brn;	// 枝番
	int				tkno;	// 摘要レコード番号
	CString			brmsg;	// 名称
} BRN_REC;

typedef struct tag_BRN_TB {
	CString			code;		// 科目コード
	OLE_COLOR		ForeCol;	// 表示文字色
	unsigned short	kn50_use;	// 50音検索サイン = 0 ... No, 'ｶﾅ' ... Yes
	BOOL			full;		// 名称の表示タイプ
								//		TRUE ... "科目.摘要名称"
								//		FALSE .. "摘要名称"
	unsigned short	page_max;	// 最大ページ
	unsigned short	page_now;	// 現在ページ
	unsigned short	page_recn;	// 1ページ分のレコード数
	unsigned short	rec_n;		// 現在ページのレコード数
	tag_BRN_REC *pBRN_REC;

} BRN_TB;

#endif	// end of _ETCMSG_H_
