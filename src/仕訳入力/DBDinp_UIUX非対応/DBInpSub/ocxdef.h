/****************************************************

				- "ocxdef.h" -

				ICS OCX  structure etc.

			日付	:	９７年１１月０６日
*****************************************************/
#include "icsocx.h"

#ifndef _OCXDEF_
#define _OCXDEF_

// ICSSel.OCX
struct _DataTable {
	CString	Data;		// 表示テキスト
	int		number;		// 表示番号 (BIN)
};


typedef	struct tag_ITEM_INPUT {
	union {
		int 		ii_seq;
		int			ii_dnp;
		struct {
			int			date;
			BYTE		bcddate[4];
		//	short		ofs;
		} ii_day;
		int				ii_bmn;
		unsigned char	ii_koj[16];

		struct	_KamokuData ii_kmk;

		int				ii_brn;
		unsigned char	ii_val[6];
		unsigned char	ii_tek[82];

		struct {
			int			date;
			BYTE		bcddate[4];
		//	short		ofs;
		} ii_tgdt;

	//	unsigned char	ii_syon[16];	//手形番号
		unsigned char	ii_syon[22];	//手形番号(電子記録債権）

		struct {
			int			husen;
		//	BYTE		hcmnt[32];
			BYTE		hcmnt[42];
		} ii_husen;
	};

}	ITEM_INPUT;


struct _ItemData {
	union {
		ITEM_INPUT	XDTA;

		struct {
			short	_skbn_sgn;
			WORD	_skbn_sgn2;
			char _skbn_txt[20];
		}_SYDTA;
	} _udta;
	char _txt[42];
	int	 _img_sq;

	// インボイス対応
	char _invno[16];	// 登録番号
	BYTE _menzei;	// 免税事業者からの課税仕入れ

#define IP_SEQ		_udta.XDTA.ii_seq		// シーケンス番号
#define IP_DENP		_udta.XDTA.ii_dnp		// 伝票番号
#define IP_DAY		_udta.XDTA.ii_day.date		// 日付
#define IP_BCDDAY	_udta.XDTA.ii_day.bcddate
#define IP_BMN		_udta.XDTA.ii_bmn		// 部門コード
#define IP_KOJI		_udta.XDTA.ii_koj
#define IP_stKMK	_udta.XDTA.ii_kmk		// 科目 構造体
#define IP_KNAM		_udta.XDTA.ii_kmk.kd_name	// 科目名称
#define IP_CdNum	_udta.XDTA.ii_kmk.kd_code	// 科目コード
#define IP_BRN		_udta.XDTA.ii_kmk.kd_eda	// ＋枝番
#define IP_cBRN		_udta.XDTA.ii_brn		// 枝番
#define IP_VAL		_udta.XDTA.ii_val		// 金額
#define IP_TKY		_udta.XDTA.ii_tek		// 摘要

#define IP_SNUM		_udta.XDTA.ii_syon		// 証憑番号
#define IP_TGDAY	_udta.XDTA.ii_tgdt.date		// 手形期日
#define IP_TGBCD	_udta.XDTA.ii_tgdt.bcddate	// BCD

#define IP_HUSEN	_udta.XDTA.ii_husen.husen	// 付箋
#define IP_HUCMT	_udta.XDTA.ii_husen.hcmnt	// 付箋コメント

#define SKBN_TXT	_udta._SYDTA._skbn_txt		//消費税属性名
#define SKBN_SGN1	_udta._SYDTA._skbn_sgn		//消費税区分サイン１
#define SKBN_SGN2	_udta._SYDTA._skbn_sgn2		//消費税区分サイン２

#define ETC_TXT		_txt			//別表示用テキストデータ
#define IMG_SQ		_img_sq			//イメージSEQ

// インボイス対応
#define INVNO		_invno			// 登録番号
#define MENZEI		_menzei			// 免税事業者からの課税仕入れ

};

// データコンディションサイン
struct _LINE_CND {
	char			INP_sg;			// 入力済みサイン
	char			INP_type;		// 入力タイプ（「借貸枝番」で使用）
									//	０...なし
									//	１...直接入力
									//	２...「借貸枝番」＝摘要枝番による枝番。
	char			TKattr;			// 摘要消費税コード実行サイン(コード)
	char			KEY_inp;		// キーボード入力あり
									// 科目欄で 科目入力した時
									// 税額欄で F10 押下した時

	char			NEXT_MOVE_KEY;
	char			IMG_sg;			// イメージサイン
	char			ETC_TXT_sg;		// 他表示サイン
#ifdef _SLIP_ACCOUNTS_
	char			dmy[2];			// '12.09.27：外部変数の初期化時に正しく初期化されなくなったので、アライメント補正用に追加
#endif
	OLE_COLOR		ETC_TXT_ForeCOL;// 他表示用テキストデータ色
};

// input ctrl
struct _InpCtl {
	int			IDC_X;		// IDC_.....
	int			IDC_X2;		// 第２コントロール（ＣＩＣＳＤｉｓｐ用）
	long		p_obj;		// CICSInput objct Adr.
	short		INP_ena;	// 入力許可サイン
	OLE_COLOR	BACK_COLOR;	// 背景色
	short		real_nChar;	// 本当のVK_キー
};



// キーによる移動
struct _KeyMoveTbl
{
	short	nowpn,		// 現在位置
			up_pn,		// 上
			dn_pn,		// 下
			left_pn,	// 左
			right_pn;	// 右
};


#endif	//_OCXDEF_

