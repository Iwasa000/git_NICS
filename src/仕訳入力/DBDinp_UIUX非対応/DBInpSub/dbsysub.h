
#include "DBSyohi8.h"

#ifndef _DBSYSUB_H_
#define _DBSYSUB_H_

//=============================================================================
// DBSySub.h
//=============================================================================

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
//#define dsignMax 6		// 仕訳区分実用サイズ
//#define DSIGNMAX_IN_DATA 6	// 仕訳区分実用サイズ ('13.10.11)
#define DSIGNMAX_IN_DATA 7	// 仕訳区分実用サイズ ('19.03.09)

// コントロールサイン
// 借 貸  区分
#define _CTL_DEBT			0x10	// 借方
#define _CTL_CRED			0x01	// 貸方
#define _CTL_DBCR			0x11	// 借貸

// 処理状態サイン( == RUN_MODE )
#define _CTL_INIZ			0	// イニシャライズ
#define _CTL_MODEFY			1	// 修正

// タイプサイン
// 科目タイプ
#define _KMK_TYP_KARIBARAI		0x01	// 仮払消費税等
#define _KMK_TYP_KARIUKE		0x02	// 仮受消費税等

#define _KMK_TYP_KISYU_TANA		0x10	// 期首棚卸

#define _KMK_TYP_KIMATU_TANA	0x20	// 期末棚卸

#define _KMK_TYP_KOTEI			0x30	// 固定資産
#define _KMK_TYP_YUKA			0x31	// 有価証券
#define _KMK_TYP_TOCHI			0x32	// 土地

#define	_KMK_TYP_MISEIKOUJI		0x40	// 未成工事支出金

// 状態 サイン
// 登録状態
#define CND_NON_TO_REGIST		1	// 登録された
#define CND_REGIST_TO_NON		2	// 未登録になった
#define CND_MODEFY				3	// 変更された
#define CND_REGIST				4	// 登録中
#define CND_NON_REGIST			5	// 未登録中
#define CND_BMN_MODEFY			6	// 部門が変更された

// モード
#define _MODE_URI				0	// 売上(仮受消費税)
#define _MODE_SIRE				1	// 仕入(仮払消費税)

//-- 改正消費税対応('13.06.14) --
typedef enum {
	ID_ICSSYZ_NO_KIKAN		=	-1,	// 不定値
	ID_ICSSYZ_5PER_KIKAN	=	0,	// 5% の期間
	ID_ICSSYZ_8PER_KIKAN,			// 8% の期間
	ID_ICSSYZ_10PER_KIKAN			// 10%の期間
} EnumIdICSSyzKikan;

typedef enum {
	ID_ICSSYZ_NO_REV		=	-1,	// 不定値
	ID_ICSSYZ_H26_REV		=	0,	// 平成26年度改正（税率5%->8%）
	ID_ICSSYZ_H27_REV,				// 平成27年度改正（簡易課税第6種追加）
	ID_ICSSYZ_H27TK_REV,			// 平成27年度改正（特定課税仕入追加）
	ID_ICSSYZ_H31_REV,				// 平成31年度改正（税率8%->10%）
} EnumIdICSSyzRevType;

// 年月日
const	int	PER8_SYZ_START_YMD	=	20140401;	// 消費税：８％施行日
const	int	PER5_SYZ_START_YMD	=	19970401;	// 消費税：５％施行日
const	int	KANISIX_START_YMD	=	20150401;	// 消費税：簡易課税第六業種 施行日	'14.11.11
const	int	TKKZSR_START_YMD	=	20151001;	// 消費税：特定課税仕入 施行日		'15.06.24
const	int	PER10_SYZ_START_YMD =	20191001;	// 消費税：１０％施行日				'19.01.21

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------

// 科目コード展開（use to "struct _DBSY_KAMOK"  ）
struct _SY_KMK {
	char			_KMKCD[20];					// 科目コード
	int				_KMKTYP;					// 仮受、仮払消費税区分
												//		０．．．通常科目
												//		１．．．仮払消費税等
												//		２．．．仮受消費税等
												//		.... 
	long			_KZECD;						// 枝番コード
	long			_KZBCD;						// 部門コード
};

// 消費税区分展開用コードバッファ（use to "struct _DBSY_KAMOK.KM_SY_ATTR"  ）
struct _code_buf {
	DWORD	code[50];							// 選択可能コードテーブル
	int		pos;								// 現在選択ポジション（１～）
	int		chk;								// 各属性により内容は異なる。
												//（現在は　仕入／売上 区分　で使用）
};

// 科目名称レコ－ド内消費税属性エリア（use to "struct _DBSY_KAMOK.KM_SY_ATTR"  ）
struct _DBSY_ATTR {

	unsigned char	_ATTR[8];	// '13.10.11 中垣内 記録
								// KNREC.knsgn[] に連動
#define	_A	_ATTR[0]			//  KNREC.knsgn[0]
#define	_B	_ATTR[1]			//  KNREC.knsgn[1]
#define	_C	_ATTR[2]			//  KNREC.knsgn[2]
#define	_D	_ATTR[3]			//  KNREC.knsgn[3]
#define	_E	_ATTR[4]			//  KNREC.knsgn[4]
#define	_F	_ATTR[5]			//  KNREC.knsgn[5]
#define	_G	_ATTR[6]
#define	_H	_ATTR[7]

	DWORD	tana;	// 棚卸区分
	char	knatt;	// 貸借区分
	char	kninp;	// 入力用 内／外区分 01.21 /08 ３＝別記
};

// 科目消費税展開（use to "struct _DBSY_KAMOK"  ）
struct _KM_SY_ATTR {

	// 消費税区分展開用コードバッファ
	struct _code_buf							// 消費税区分
					_ZRITU,						// 税率サイン	：Ｄ０～Ｄ３	５％～
					_ZKBN,						// 税区分サイン	：Ｄ４～Ｄ７	外税～
					_KZKBN,						// 課税区分		：Ｄ０～Ｄ３	非課税～
					_DMY1,						// 未使用		：Ｄ６～Ｄ７	
					_TAIKAKBN,					// 固定資産区分	：Ｄ４～Ｄ５	
					_URIKBN,					// 売上業種区分	：Ｄ０～Ｄ３	卸売業～
					_SIREKBN,					// 仕入区分		：Ｄ４～Ｄ７	課税売上～
					_DMY2,						// 未使用		：Ｄ０～Ｄ３	
					_SIWAKEKBN,					// 仕訳区分		：Ｄ４～Ｄ７	売上～
	
					_TOKTEIKBN,					// 特定収入区分	：Ｄ０～Ｄ１
					_TK_SIREKBN,				// 特定収入仕入区分

					// 付加属性
					// 売上/仕入 属性
					_URI_SIRE_KBN,
					// 免税事業者からの仕入 属性
					_EXEMPT_SIRE_KBN;			// 免税事業者からの仕入サイン：Ｄ５

					// 科目による消費税属性
	DWORD			_KMKSWKBN_CODE;				// ここの項目と同一の仕訳区分は仕訳にはおとさない

	// 科目名称レコ－ド内消費税属性エリア
//	DWORD				_ATTR_CODE;				// 消費税属性 コード		０　 … _DEF_ATTR
												//							etc. … _ETC_ATTR[0]～
	struct _DBSY_ATTR	_DEF_ATTR,				// 消費税属性		 ... そのままの属性
												// 以下は可能属性
//-- '15.06.24 --
//					_ETC_ATTR[30];
//---------------
					_ETC_ATTR[60];
//---------------
												//				[ 0] ... "輸入仕入"… 0x04 0x04
												//				[ 1] ... "輸出売上"… そのままの属性
												//				[ 2] ... "非課税輸出売上"… 0x02 0x0A
												//				[ 3] ... "貸倒回収"… 0x02 0x05
												// 科目属性あり	[ 4] ... "貸倒損失"… 0x08 0x03
												//				[ 5] ... "販売奨励金収入"… 0x02 0x06
												//				[ 6] ... "販売奨励金費用"… 0x04 0x02
												//				[ 7] ... "仕入割引"… 0x02 0x06
												//				[ 8] ... "売上割引"… 0x04 0x03
												//				[ 9] ... "棚卸調整加算額"… 0x04 0x05
												//				[10] ... "棚卸調整減算額"… 0x04 0x06

												// 科目属性あり	[11] ... "輸出売上"... 消費税区分による
};

// 消費税科目展開バッファ
struct _DBSY_KAMOK {

	// 科目コード展開
	struct _SY_KMK		SY_KMK;

	// 科目消費税展開
	struct _KM_SY_ATTR  KM_SY_ATTR;

};

// 仕訳展開作業バッファ
struct _DBSYSUB {

	// チェックサイン
	int				m_DATE_RGST;				// 日付展開サイン
	int				m_DEBT_RGST;				// 借方展開サイン
	int				m_CRED_RGST;				// 貸方展開サイン
	int				m_DBCR_RGST;				// 仕訳(借貸より)展開サイン
	int				m_VALUE_RGST;				// 金額展開サイン
	int				m_TAIKAVAL_RGST;			// 対価金額展開サイン
	int				m_INVNO_RGST;				// インボイス登録番号展開サイン

	// チェックサイン登録関数
	int&	is_DATE_RGST()		{ return m_DATE_RGST; }
	int&	is_DEBT_RGST()		{ return m_DEBT_RGST; }
	int&	is_CRED_RGST()		{ return m_CRED_RGST; }
	int&	is_DBCR_RGST()		{ return m_DBCR_RGST; }
	int&	is_VALUE_RGST()		{ return m_VALUE_RGST;}
	int&	is_TAIKAVAL_RGST()	{ return m_TAIKAVAL_RGST; }
	int&	is_INVNO_RGST()		{ return m_INVNO_RGST; }

#ifdef CLOSE
	// 作業仕訳データセーブ
	CDATAS DATAS;
	// データセーブアドレス
	XCDBDATA* pDDATA;
#endif

	// 仕訳展開データ
	int					_SY_DATE;				// 日付 
	struct _DBSY_KAMOK	_SY_DEBT,				// 借方科目属性
						_SY_CRED,				// 貸方科目属性
						_SY_DBCR;				// 仕訳属性（借方／貸方科目属性から作成する）
	char				_SY_VALUE[20],			// 仕訳金額
						_SY_ZEIGAK[20];			// 仕訳税額
	char				_SY_TAIKAVAL[20];		// 対価金額
	char				_SY_TAIKAZEI[20];		// 対価税額
	char				_SY_INVNO[20];			// インボイス登録番号


	// ユーザ参照テーブル
	struct _SY_MSG_TBL	MSG_USR_TBL_RITU;
	CString				MSG_SELSTR_RITU;	// 選択メッセージ
	struct _SY_MSG_TBL	MSG_USR_TBL_ZEIKAZEI;
	CString				MSG_SELSTR_ZEI;
	// 特定収入
	struct _SY_MSG_TBL	MSG_USR_TBL_TOKTEI;
	CString				MSG_SELSTR_TOKTEI;
	struct _SY_MSG_TBL	MSG_USR_TBL_URI;
	struct _SY_MSG_TBL	MSG_USR_TBL_SIRE;
	CString				MSG_SELSTR_URISIRE;
	struct _SY_MSG_TBL	MSG_USR_TBL_TAIKAKBN;
	CString				MSG_SELSTR_TAIKAKBN;
	struct _SY_MSG_TBL	MSG_USR_TBL_SWKBN;
	CString				MSG_SELSTR_SWKBN;
	CString				MSG_LIN_MSG;
	// インボイス対応
	// 免税事業者からの仕入
	struct _SY_MSG_TBL	MSG_USR_TBL_EXEMPT;
	CString				MSG_SELSTR_EXEMPT;
};

#endif	// end of _DBSYSUB_H_

