// ShinCalculateRec8.h

#ifndef  __ICS_SHINCALCULATEREC__
#define  __ICS_SHINCALCULATEREC__

#define	MONTH_MAX	16

// 仕入区分 分類科目単位 設定情報
const	TCHAR	SHINCAL_OWNTBL_SQL[]	=	_T("apl_name = '%s' and itm_name = '%s' and u_id=%d");
const	TCHAR	SHINCAL_APLNAME[]		=	_T("INDIVIDUAL_TAX");
const	TCHAR	SHINCAL_ITMNAME[]		=	_T("STOCK_DIVIDE");

#define	_SVL	sizeof( struct S_VAL )
#define	_SUL	sizeof( struct SUK_R )
#define	AMAX	35
#define	BMAX	18

#pragma pack( push, 2 )

// 科目名称テーブル
struct	_KN_REC	{
	unsigned char	Kn_cod[8];				/* 科目（内部）コード (８桁)			*/
	unsigned char	Kn_att,					/* 科目属性（貸借サイン）				*/
					Kn_sz1,					/* 消費税処理グループ区分				*/
					Kn_sz2,					/* 消費税処理グループ区分明細			*/
					Kn_sz3,					/* 消費税属性／自動分離					*/
					Kn_sz4,					/* 消費税課税区分						*/
					Kn_sz5;					/* 消費税仕入区分						*/
			char	Kn_ctg,					/* 業種サイン							*/
					Kn_Dmm;					/* リザーブ								*/
			short	Kn_prf1,				/* 青色申告								*/
					Kn_prf2,				/* 白色申告								*/
					Kn_prf3;				/* 月別									*/
};
// 以降追加

#pragma pack( pop )

#endif

