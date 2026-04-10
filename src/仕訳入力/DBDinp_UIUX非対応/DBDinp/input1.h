#ifndef __INPUT1_H__
#define __INPUT1_H__
/***************************************

	data input define & structure table
	
		file input1.h

****************************************/

#define	err_ext		_errex

#define	uint	unsigned int
#define uchar	unsigned char

#define ERR	-1
#define OK 0

#define	_BIN	0x16
#define _BIN_LNG (_BIN&0x0f)

#define	FIX_l	2		/* 固定科目表示ライン	*/

#define KMNLEN	14
#define	TKLENG	80
//#define KMNLEN	sizeof( ((KNREC*)0)->KNnam )
//#define	TKLENG	sizeof( ((DATAREC*)0)->_TKS-8 )
//#define	RECLENG	sizeof(DAREC)


#define	KMK_l	13 	/* ﾆｭｳﾘｮｸ ｺｳﾓｸ ﾋｮｳｼﾞﾗｲﾝ */

#define RECMAX		50
#define	AC_MAX		32
#define	PG_MAX		 8
#define DP_MAX		16			/* ﾋｮｳｼﾞ MAX ｺｽｳ */ 
#define RENT_MAX	30

#define	TOP_COUNT	1000		// Requery などの TOP 数
#define SCAN_TOP	100


//#define	SCROLL_NL_	(4+1)			/* データ表示ライン数	*/
//#define SCROLL_NL	(INP_mode == _KEYSCAN ? SCROLL_NL_ : SCROLL_NL_ - 1)

#define	_ADD		 0
#define _SUB		-1
#define	_SEARCH		 0
#define	_ENTRY		 1
#define	_APPEND		 0
#define	_RETREV	  0x10
#define	_MODEFY	  0x11
#define _MDFEND	  0x12
#define _KEYSCAN  0x13


// 摘要枝番呼び出し
struct _TKYBRN_SEL {
	int				BRN_BMN;	// 検索部門コード
	CString			BRN_KCOD;	// 検索コード
	CString			BRN_KANA;	// 仮名１文字
	short BRN_PAGE;				// 現在表次ページ
	OLE_COLOR BRN_ForeCol;		// 表示文字色

	void TS_Reset() {
		BRN_BMN = -1;
		BRN_KCOD.Empty();
		BRN_KANA.Empty();
		BRN_PAGE = 0;
		BRN_ForeCol = 0;
	}
};


// 仕訳コピーの選択
#define SELCPY_NONE		 0
#define SELCPY_UP		-1
#define SELCPY_DOWN		 1


#define SEL_NONE	0
#define SEL_SHIFT	1
#define SEL_CTRL	2



#endif	// end of __INPUT1_H__
