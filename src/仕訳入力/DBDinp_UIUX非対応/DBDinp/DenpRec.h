#ifndef _DENPREC_H_
#define _DENPREC_H_

#include "ocxdef.h"


//入力行ライン数
#define DP_SCROLL_NL	100
#define DP_SCROLL_MAX	(100*2)		//複合仕訳を詰めないので MAX。
//#define DP_SCROLL_NL	20

#define DP_SCROLL_NL_F	7
#define DP_SCROLL_NL_N	6
#define DP_SCROLL_NL_S	6

// 振替伝票モード
#define FURI_TANBMON	0	//単一部門・工事 入力
#define FURI_DATABMON	1	//仕訳ごと部門・工事 入力

//ポジションサイン
//
// 伝票番号・日付
#define DP_DENP_PN		0	//伝票番号
#define DP_MM_PN		1	//月
#define DP_DD_PN		2	//日
#define DP_SYONUM_PN	3	//証憑番号
#define DP_BMN_PN		4	//部門(借貸で同じコード)
#define DP_KOJI_PN		5	//工事番号(借貸で同じコード)
#define	DP_KESYU_PN		6	//決修の表示

// 仕訳データ　振替伝票
#define FD_DBVAL_PN		7	//借方金額
#define FD_DBZEI_PN		8	//借方税額
#define FD_DBMN_PN		9	//借方部門
#define FD_DKOJI_PN		10	//借方工事
#define FD_DEBT_PN		11	//借方科目
#define FD_DSYOG_PN		12	//借方諸口枝番
#define FD_CRVAL_PN		13	//貸方金額
#define FD_CRZEI_PN		14	//貸方税額
#define FD_CBMN_PN		15	//貸方部門
#define FD_CKOJI_PN		16	//貸方工事
#define FD_CRED_PN		17	//貸方科目
#define FD_CSYOG_PN		18	//貸方諸口枝番
#define FD_TKY_PN		19	//摘要
#define FD_DSKBN_PN		20	//借方消費税区分
#define FD_CSKBN_PN		21	//貸方消費税区分

#define FD_TGNUM_PN		22	//手形番号
#define FD_TGDATE_PN	23	//手形期日
#define FD_HJIYU_PN		24	//変動事由
#define FD_HUSEN_PN		25	//付箋

#define FD_MAX_PN		(FD_HUSEN_PN+1)

// 諸口貸借
#define	DP_SYOGT_PN		30


#ifdef CLOSE	// 未使用 //////////////////////////////////////////////////

// 仕訳データ　入金伝票
#define ND_DBVAL_PN		-1	//借方金額
#define ND_DBMN_PN		-1	//借方部門
#define ND_DKOJI_PN		-1	//借方工事
#define ND_DEBT_PN		-1	//借方科目
#define	ND_CRVAL_PN		FD_CRVAL_PN		//貸方金額
#define	ND_CRZEI_PN		FD_CRZEI_PN		//貸方金額
#define ND_CBMN_PN		FD_CBMN_PN		//貸方部門
#define ND_CKOJI_PN		FD_CKOJI_PN		//貸方工事
#define	ND_CRED_PN		FD_CRED_PN		//貸方科目
#define	ND_TKY_PN		FD_TKY_PN		//摘要
#define	ND_SKBN_PN		FD_CSKBN_PN		//消費税区分

// 仕訳データ　出金伝票
#define SD_CBMN_PN		-1	//貸方部門
#define SD_CKOJI_PN		-1	//貸方工事
#define SD_CRED_PN		-1	//貸方科目
#define SD_CRVAL_PN		-1	//貸方金額
#define SD_DBVAL_PN		FD_DBVAL_PN		//借方金額
#define SD_DBZEI_PN		FD_DBZEI_PN		//借方金額
#define SD_DBMN_PN		FD_DBMN_PN		//借方部門
#define SD_DKOJI_PN		FD_DKOJI_PN		//借方工事
#define SD_DEBT_PN		FD_DEBT_PN		//借方科目
#define SD_TKY_PN		FD_TKY_PN		//摘要
#define SD_SKBN_PN		FD_DSKBN_PN		//消費税区分

#endif /////////////////////////////////////////////////////////////////////


struct __SYDATA {
	unsigned char	_DSGN[SZ_DSGN];	//仕訳サイン
								// [1]税率・税区分
								// [2]課税区分
								// [3]仕入 及び 売上区分
								// [4]消費税仕訳区分

	char SKBNtxt[20];		// 区分名称
	char TKattr, sy_dmy;	// 摘要属性
	char TKsyohi[6];		// 摘要消費税エリア(対価格納)
	char sy_chg;			// 消費税変更サイン
	char sy_dmy2;			//
	char TKsign[6];			// 摘要消費税サイン
	char TKinvno[16];		// 摘要登録番号
	char sy_dmy3[60];		//

	DWORD		swk_sw,		//消費税仕訳区分
				ritu_sw,	//税率
				zeikbn_sw,	//課税区分
				uri_sw,		//売上・仕入
				toku_sw,	//特定収入[06.03 /08]
				kts_sw;		//固定資産譲渡・取得
	//消費税デフォルト
	DWORD		swk_def,
				ritu_def,
				zeikbn_def,
				uri_def,
				toku_def,	//特定収入[06.03 /08]
				kts_def;
};


struct _DenpData {
	union {
		ITEM_INPUT		XDTA;
		struct __SYDATA _SYDATA;
	} _udta;

	int	dp_nbcd;

#define DP_SEQ		_udta.XDTA.ii_seq		// シーケンス番号
#define DP_DENP		_udta.XDTA.ii_dnp		// 伝票番号
#define DP_DAY		_udta.XDTA.ii_day.date		// 日付
#define DP_BCDDAY	_udta.XDTA.ii_day.bcddate
#define DP_BMN		_udta.XDTA.ii_bmn		// 部門コード
#define DP_KOJI		_udta.XDTA.ii_koj
#define DP_stKMK	_udta.XDTA.ii_kmk		// 科目 構造体
#define DP_KNAM		_udta.XDTA.ii_kmk.kd_name	// 科目名称
#define DP_CdNum	_udta.XDTA.ii_kmk.kd_code	// 科目コード
#define DP_BRN		_udta.XDTA.ii_kmk.kd_eda	// ＋枝番
#define DP_cBRN		_udta.XDTA.ii_brn		// 枝番
#define DP_VAL		_udta.XDTA.ii_val		// 金額
#define DP_TKY		_udta.XDTA.ii_tek		// 摘要

#define DP_SNUM		_udta.XDTA.ii_syon			// 手形番号
#define DP_TGDAY	_udta.XDTA.ii_tgdt.date		// 手形期日
#define DP_TGBCD	_udta.XDTA.ii_tgdt.bcddate	// BCD
#define DP_HUSEN	_udta.XDTA.ii_husen.husen	// 付箋
#define DP_HCMNT	_udta.XDTA.ii_husen.hcmnt	// 付箋コメント

#define SYDATA		_udta._SYDATA		//消費税
#define NAIBCD		dp_nbcd				//内部コード
};


//入力データ 定義
struct _DNPLINE_CND {
	char			INP_sg;		// 入力済みサイン
	char			SET_sg;		// 入力データセットサイン
								// 借方・貸方				自動仕訳で消費税科目セットした時ON
								// 借方消費税・貸方消費税	自動仕訳で消費税科目をセットした時ON
	char			INP_type;		// 入力タイプ（「借貸枝番」で使用）
									//	０...なし
									//	１...直接入力
									//	２...「借貸枝番」＝摘要枝番による枝番。
									// 税額欄：
									//	１：有価証券非課税譲渡仕訳
									//	２：消費税仕訳でないとき
									//  ３：消費税額がデフォルトと異なる
									//  ４：    〃    (色を変える場合)
									//  ５：輸出売上仕訳

	char			ETC_sg[4];		//税額欄ETC_sg[0]
									// [0]	1：外税で免税事業者等からの課税仕入れの場合
									// [1] 移動元のポジション 借方科目欄からの場合は FD_DEBT_PN
									// [2] 税額欄にフォーカスセットした回数
									// [3] 税額欄のF10押下サイン

	char			ETC_msg[48];	// 追加名称(枝番名称等)格納
	COLORREF		ETC_col;		// 追加名称文字カラー

	char			EDT_sg;		//転送済み仕訳入力許可サイン
								//０：通常仕訳
								//１：転送済み入力可
								//２：確定仕訳入力不可
								//３：転送済み入力不可
								//10：参照モード入力不可
};

//
// 伝票番号・日付
//
struct _DP_DATE_LINE {
	struct _DenpData	_XXDTA1[ DP_KOJI_PN + 1 ];
#define DENP_DATA	_XXDTA1[ DP_DENP_PN ]
#define MM_DATA		_XXDTA1[ DP_MM_PN ]
#define DD_DATA		_XXDTA1[ DP_DD_PN ]
#define SYONUM_DATA	_XXDTA1[ DP_SYONUM_PN ]
#define BMN_DATA	_XXDTA1[ DP_BMN_PN ]
#define KOJI_DATA	_XXDTA1[ DP_KOJI_PN ]
	struct _DNPLINE_CND LINE_CND[ DP_KOJI_PN + 1 ];
};

// 仕訳データ
struct _DP_DATA_LINE {
	struct _DenpData	_XXDTA2[ FD_HUSEN_PN - FD_DBVAL_PN + 1 ];

#define DBVAL_DATA	_XXDTA2[ FD_DBVAL_PN - FD_DBVAL_PN ]
#define DBZEI_DATA	_XXDTA2[ FD_DBZEI_PN - FD_DBVAL_PN ]
#define DBMN_DATA	_XXDTA2[ FD_DBMN_PN - FD_DBVAL_PN ]
#define DKOJI_DATA	_XXDTA2[ FD_DKOJI_PN - FD_DBVAL_PN ]
#define DEBT_DATA	_XXDTA2[ FD_DEBT_PN - FD_DBVAL_PN ]
#define DSYOG_DATA	_XXDTA2[ FD_DSYOG_PN - FD_DBVAL_PN ]
#define CRVAL_DATA	_XXDTA2[ FD_CRVAL_PN - FD_DBVAL_PN ]
#define CRZEI_DATA	_XXDTA2[ FD_CRZEI_PN - FD_DBVAL_PN ]
#define CBMN_DATA	_XXDTA2[ FD_CBMN_PN - FD_DBVAL_PN ]
#define	CKOJI_DATA	_XXDTA2[ FD_CKOJI_PN - FD_DBVAL_PN ]
#define	CRED_DATA	_XXDTA2[ FD_CRED_PN - FD_DBVAL_PN ]
#define CSYOG_DATA	_XXDTA2[ FD_CSYOG_PN - FD_DBVAL_PN ]
#define	TKY_DATA	_XXDTA2[ FD_TKY_PN - FD_DBVAL_PN ]
#define	DSKBN_DATA	_XXDTA2[ FD_DSKBN_PN - FD_DBVAL_PN ]
#define	CSKBN_DATA	_XXDTA2[ FD_CSKBN_PN - FD_DBVAL_PN ]

#define	TGNUM_DATA	_XXDTA2[ FD_TGNUM_PN  - FD_DBVAL_PN ]
#define	TGDATE_DATA	_XXDTA2[ FD_TGDATE_PN - FD_DBVAL_PN ]
#define	DPHJYU_DATA	_XXDTA2[ FD_HJIYU_PN  - FD_DBVAL_PN ]
#define DPHSEN_DATA	_XXDTA2[ FD_HUSEN_PN  - FD_DBVAL_PN ]

	struct _DNPLINE_CND LINE_CND[ FD_HUSEN_PN - FD_DBVAL_PN + 1 ];
};


// 伝票クリアサイン
#define SHEET_ALL		0
#define SHEET_TAISYAKU	1
#define SHEET_MOTIKOSI	2


enum Denpyo {
	KIND_FURIKAE	=	1,
	KIND_NYUKIN		=	2,
	KIND_SYUKKIN	=	3,
	TYPE_TANDOKU	=	0,
	TYPE_FUKUGOU	=	1,
	MODE_NORMAL		=	0,
	MODE_AUTO		=	1,
	MODE_NOP		=	-1,
	JOB_INPUT		=	0,
	JOB_SCANKEY		=	1,
	JOB_SCAN		=	2,

	DENPNO_NONE		=	0,
	DENPNO_CHK		=	1,
	DENPNO_AUTO		=	2,
};

//資金繰り枝番入力
#define	SKNBRN_NONE		0
#define	SKNBRN_INPUT	1



// 伝票の位置情報
typedef struct _DNP_POS
{
	int	dpno;		//ダイアグラムIDを決定する番号(0～)
	int dpindex;	//ダイアグラムインデックス

	int	dp_pn;		//伝票ポジション番号
	int	dp_enable;	//入力可能か？

} DNP_POS;


typedef struct _SETMEISYO
{
	char		sm_name[30];	//表示名称
	char		sm_etc[40];		//追加名称(科目名称＋枝番名称)

	COLORREF	sm_etccol;		//追加名称の文字色
} SETMEISYO;


#define DNP_HEAD	0	//伝票ヘッド部
#define DNP_DATA	1	//伝票データ部
#define DNP_KEI		2	//伝票計部





#endif	// end of _DENPREC_H_
