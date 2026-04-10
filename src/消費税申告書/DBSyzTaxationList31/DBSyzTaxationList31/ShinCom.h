
#pragma once

//-----------------------------------------------------------------------------
// 消費税申告書：共通情報
//-----------------------------------------------------------------------------

//== クラス ==
class	CShinCom
{
public:
	CShinCom();
	~CShinCom();

	CBrush& GetBkClrBrsh(){ return m_BkClrBrsh; };	

private:
	CBrush	m_BkClrBrsh;	// バックカラー
};

//== 定数 ==
const	COLORREF	SIROIRO_COLOR =	RGB( 0xff, 0xfb, 0xf0 );
const	COLORREF	BC_WHITE		=	RGB(255,255,255);		// 入力不可	（白色）
const	COLORREF	BC_CREAM		=	RGB(211,245,188);		// 入力可	（クリーム色）
const	COLORREF	BC_GRAY			=	RGB(192,192,192);		// ロック	（灰色）
const	COLORREF	BC_BLUE			=	RGB(160,255,255);		// 特殊①	（青）
const	COLORREF	BC_GREEN		=	RGB(132,255,193);		// 特殊②	（緑）
const	COLORREF	BC_GREEN_L		=	RGB(190,243,199);		// 特殊③	（薄緑）

const	int			MONY_BUF_SIZE	=	6;

//== 構造体 ==
// 画面制御テーブル
typedef struct _tag_ICS_SH_ITEM_INDEX_ {
	short	Index,					// 金額インデックス番号（ダイアグ）
			Connect,				// 連動時　　0:入力不可 1:入力許可 2:経過措置時は入力不可
			UnConnect,				// 非連動時　0:入力不可 1:入力許可 2:経過措置時は入力不可
			AllUnConnect;			// 確定時、入力不可
//} ITEMINDEX;
} SH_ITEMINDEX;
