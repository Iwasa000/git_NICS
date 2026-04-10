#ifndef _DINPSUB_H_
#define _DINPSUB_H_

#define APPEND_LINE 0
#define normal_back_color(ln) (ln == APPEND_LINE ? RGB_WHITE : RGB_PAPER_WHITE)		//通常背景色

//入力ラインポジション
#define SEQ_PN		DATA_SEQ	//番号( == 0 )
#define DNPTYPE_PN	DATA_USER1	//伝票タイプ
#define DATE_PN		DATA_DATE	//日付
#define DENP_PN		DATA_DENP	//伝票番号

#define BMON_PN		DATA_DEB_BMON	//借方-部門
#define KOJI_PN		DATA_DEB_KNO	//借方-工事番号
#define DEBT_PN		DATA_DEBT		//借方

#define CBMN_PN		DATA_CRE_BMON	//貸方-部門
#define CKJI_PN		DATA_CRE_KNO	//貸方-工事番号
#define CRED_PN		DATA_CRED		//貸方

#define VAL_PN		DATA_VAL		//金額
#define ZEI_PN		DATA_ZEI		//金額
#define SKBN_PN		DATA_SYZ		//消費税	
#define TKY_PN		DATA_TEKIYO		//摘要

#define SYON_PN		DATA_SYONO		//手形番号
#define TGDT_PN		DATA_TGDATE		//手形期日
#define SKIN_PN		DATA_SKIN		//資金
#define HSEN_PN		DATA_HUSEN		//付箋
#define HJIYU_PN	DATA_HJIYU		//変動事由
// 11.20 /12
#define DOCEVI_PN	DATA_DOCEVI		//証憑番号

#define LNCND_MAX	DATA_DOCEVI+1



#define ONEHUSEN_PN		100		// 1Line のときの 付箋


struct _DATA_LINE {
	short			ZREC_NO;	//ZREC[ZREC_NO-1]

	class DTLIST_DATA	_XXDTA;

	BYTE BcdDate[3];
	char rsv;

#ifdef OLD_CLOSE	// 11.20 /12
	struct _LINE_CND LINE_CND[ HJIYU_PN + 1 ];
#else
	struct _LINE_CND LINE_CND[ LNCND_MAX ];
#endif

	int		ImgSEQ;	// イメージ番号(なしのときは 0)
	char	Denp[4];

	char	Menz[4];	//免税事業者
	char	Invno[16];	//インボイス登録番号
};


#define NORMAL_MST		0	// 非消費税マスター
#define NORMAL_SYMST	1	// 通常
#define KOBET_SYMST		2	// 個別対応
#define KANI_SYMST		3	// 簡易


#endif	// end of _DINPSUB_H_
