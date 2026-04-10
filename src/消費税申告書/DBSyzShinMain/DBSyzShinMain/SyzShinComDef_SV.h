#pragma	once

//=============================================================================
// 消費税申告書：共通定義
//-----------------------------------------------------------------------------
// start	'13.12.24
// latest	'14.02.03
//=============================================================================

#include	<eselect8.h>
#include	<office8.h>
#include	<toolbar.h>


//== 構造体 ==
typedef	struct	_tag_ICS_SYZCOM_SHININFO_ {
	CWnd		*pBaseView;			// 基本情報ビューのポインタ
	CWnd		*pParent;			// 親ウィンドウのポインタ
	ESELECT		*pEselect;			// 電子申告情報
	BOOL		*pIsOpenEselect;	// 税務署コードモジュールをオープンした？
	BOOL		isUseSpcIncome;		// 特定収入を処理する？
	int			sgSpc;				// 特定収入を処理する？（1:処理する, 0:処理しない）
	BOOL		isExistOffice;		// 事務所名あり？
	OFC_DATA	*pO_DATA;			// 事務所情報
	ZEI_DATA	*pZ_DATA;			// 税理士個人情報
	BOOL		*pIsOver500Million;	// 課税売上高が５億円越え？
	ICSToolBar	*pToolBar;			// ツールバー
} SYC_SHININFO;

//== 印刷情報 ==
typedef struct	_tag_ICS_SH_PRINT_CMINFO_ {
	char	PRtype;			// D0：申告書本表出力
							// D1：付表１又は４出力	
							// D2：付表２又は５出力
							// D3：付表２-(2)又は５-(2)出力
							// D4：付表３出力
							// D7：版下出力
	char	PRtable[50];	// 仕入控除税額に関する明細書（次葉）	←　本当？？？
	int		isColorPrn;		// モノクロ＆カラーサイン				←　m_isColorPrn
	int		PgMx;			// 出力枚数サイン（複写は含まない）
} SH_PRINT_CMINFO;
