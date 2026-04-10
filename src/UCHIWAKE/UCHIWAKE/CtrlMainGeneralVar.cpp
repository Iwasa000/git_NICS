#include "StdAfx.h"
#include "CtrlMainGeneralVar.h"

CCtrlMainGeneralVar::CCtrlMainGeneralVar(const CdbUcInfMain& rc)
{
	bKanaDone			= rc.m_GeneralVar[0] & BIT_D0;
	bHokanDataMente		= rc.m_GeneralVar[0] & BIT_D1;
	bDispIkkatDlg		= rc.m_GeneralVar[0] & BIT_D2;
	bYoushikiSkip		= rc.m_GeneralVar[0] & BIT_D3;
	bCopyOwnData		= rc.m_GeneralVar[0] & BIT_D4;
	bDoneUpdate1_0_2_1	= rc.m_GeneralVar[0] & BIT_D5;
	bDispYoushikiSelect	= rc.m_GeneralVar[0] & BIT_D6;
	bReturnMoveDown		= rc.m_GeneralVar[0] & BIT_D7;

	bRegAutoRef			= rc.m_GeneralVar[1] & BIT_D0;
// midori 150892 add -->
	bMakeUpMethod		= rc.m_GeneralVar[1] & BIT_D1;
// midori 150892 add <--

// midori 160612 add -->
	bDataKakutei		= rc.m_GeneralVar[1] & BIT_D2;
// midori 160612 add <--

// midori 161103 del -->
	bZenFlg = rc.m_GeneralVar[1] & BIT_D4;
// midori 161103 del <--

// midori 152135,152136 add -->
	bBankSort = rc.m_GeneralVar[1] & BIT_D5;		// 参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
	bAddressSort = rc.m_GeneralVar[1] & BIT_D6;		// 参照ダイアログ::表示順（取引先）		0:番号順	1:50音順
// midori 152135,152136 add <--
// midori 152745 add -->
	bAddressSort2 = rc.m_GeneralVar[1] & BIT_D7;	// 参照ダイアログ::表示順（取引先） 得意先		0:番号順	1:50音順
// midori 152745 add <--
// midori UC_0004 add -->
	bUpKakutei = rc.m_GeneralVar[2] & BIT_D0;		// 入力確定更新サイン		0:更新する	1:更新しない
// midori UC_0004 add <--
// midori 190301 add -->
	bUseStyle = rc.m_GeneralVar[2] & BIT_D1;		// 使用する様式				TRUE:新様式(簡素化)		FALSE:旧様式
// midori 190301 add <--

// 157270 del -->
//// midori 155399 del -->
////// midori 190504 add -->
////	bMstUpd = rc.m_GeneralVar[2] & BIT_D2;			// マスター更新サイン(改良依頼No.190505 データベース修正) 1:更新済み
////// midori 190504 add <--
//// midori 155399 del <--
//// midori 155399 add -->
//	bMstUpd = rc.m_GeneralVar[3] & BIT_D1;			// マスター更新サイン(改良依頼No.190505 データベース修正) 1:更新済み
//// midori 155399 add <--
// 157270 del <--
// 157270 add -->
	bMstUpd = rc.m_GeneralVar[3] & BIT_D7;			// マスター更新サイン
// 157270 add <--

// midori 155314 add -->
	bOmitUpd = rc.m_GeneralVar[3] & BIT_D0;			// 印刷設定 省略文字 修復サイン		1:修復済み
// midori 155314 add <--
// midori 191002 add -->
	bKeiUpd = rc.m_GeneralVar[3] & BIT_D2;			// 小計名称更新サイン		1:更新済み
// midori 191002 add <--
// midori UC_0060 add -->
	bOmitUpd2 = rc.m_GeneralVar[3] & BIT_D4;		// 印刷設定 省略文字 修復サイン２		1:修復済み
// midori UC_0060 add <--
// midori 157043 del -->
//// midori 156186 add -->
//	bKmkRowChk = rc.m_GeneralVar[3] & BIT_D5;		// 画面表示「科目行」対応後、科目行処理方法変更メッセージ表示サイン　1:メッセージ表示済み　[勘定科目内訳書修正依頼20200611.xlsx] No.156186
//// midori 156186 add <--
// midori 157043 del <--
// midori 156188_3 add -->
	bKmkRowCnv = rc.m_GeneralVar[3] & BIT_D6;		// 画面表示「科目行」対応後、科目行挿入済みサイン　　　　　　　　　　1:科目行挿入済み　　　[勘定科目内訳書修正依頼20200611.xlsx] No.156188
// midori 156188_3 add <--

// インボイス登録番号追加対応_23/11/06 add -->
	bUseStyle2 = rc.m_GeneralVar[4] & BIT_D5;		// 使用する様式		TRUE:新様式(登録番号追加版)	FALSE:旧様式(簡素化or旧)
// インボイス登録番号追加対応_23/11/06 add <--
}

void CCtrlMainGeneralVar::Set(CdbUcInfMain& rc)
{
	if( bKanaDone )				rc.m_GeneralVar[0] |= BIT_D0;	else rc.m_GeneralVar[0] &= ~BIT_D0;
	if( bHokanDataMente )		rc.m_GeneralVar[0] |= BIT_D1;	else rc.m_GeneralVar[0] &= ~BIT_D1;
	if( bDispIkkatDlg )			rc.m_GeneralVar[0] |= BIT_D2;	else rc.m_GeneralVar[0] &= ~BIT_D2;
	if( bYoushikiSkip )			rc.m_GeneralVar[0] |= BIT_D3;	else rc.m_GeneralVar[0] &= ~BIT_D3;
	if( bCopyOwnData )			rc.m_GeneralVar[0] |= BIT_D4;	else rc.m_GeneralVar[0] &= ~BIT_D4;
	if( bDoneUpdate1_0_2_1 )	rc.m_GeneralVar[0] |= BIT_D5;	else rc.m_GeneralVar[0] &= ~BIT_D5;
	if( bDispYoushikiSelect )	rc.m_GeneralVar[0] |= BIT_D6;	else rc.m_GeneralVar[0] &= ~BIT_D6;
	if( bReturnMoveDown )		rc.m_GeneralVar[0] |= BIT_D7;	else rc.m_GeneralVar[0] &= ~BIT_D7;

	if( bRegAutoRef )			rc.m_GeneralVar[1] |= BIT_D0;	else rc.m_GeneralVar[1] &= ~BIT_D0;
// midori 150892 add -->
	if( bMakeUpMethod )			rc.m_GeneralVar[1] |= BIT_D1;	else rc.m_GeneralVar[1] &= ~BIT_D1;
// midori 150892 add <--

// midori 160612 add -->
	if( bDataKakutei )			rc.m_GeneralVar[1] |= BIT_D2;	else rc.m_GeneralVar[1] &= ~BIT_D2;
// midori 160612 add <--
// midori 161103 add -->
	if( bZenFlg )				rc.m_GeneralVar[1] |= BIT_D4;	else rc.m_GeneralVar[1] &= ~BIT_D4;
// midori 161103 add <--
// midori 152135,152136 add -->
	if( bBankSort )				rc.m_GeneralVar[1] |= BIT_D5;	else rc.m_GeneralVar[1] &= ~BIT_D5;	// 参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
	if( bAddressSort )			rc.m_GeneralVar[1] |= BIT_D6;	else rc.m_GeneralVar[1] &= ~BIT_D6;	// 参照ダイアログ::表示順（取引先）		0:番号順	1:50音順
// midori 152135,152136 add <--
// midori 152745 add -->
	if( bAddressSort2 )			rc.m_GeneralVar[1] |= BIT_D7;	else rc.m_GeneralVar[1] &= ~BIT_D7;	// 参照ダイアログ::表示順（取引先） 得意先		0:番号順	1:50音順
// midori 152745 add <--
// midori UC_0004 add -->
	if( bUpKakutei )			rc.m_GeneralVar[2] |= BIT_D0;	else rc.m_GeneralVar[2] &= ~BIT_D0;	// 入力確定更新サイン	0:更新する	1:更新しない
// midori UC_0004 add <--
// midori 190301 add -->
	if(bUseStyle)				rc.m_GeneralVar[2] |= BIT_D1;	else rc.m_GeneralVar[2] &= ~BIT_D1;	// 使用する様式				TRUE:新様式(簡素化)		FALSE:旧様式
// midori 190301 add <--

// 157270 del -->
//// midori 155399 del -->
////// midori 190504 add -->
////	if(bMstUpd)					rc.m_GeneralVar[2] |= BIT_D2;	else rc.m_GeneralVar[2] &= ~BIT_D2;	// マスター更新サイン(改良依頼No.190505 データベース修正) 1:更新済み
////// midori 190504 add <--
//// midori 155399 del <--
//// midori 155399 add -->
//	if(bMstUpd)					rc.m_GeneralVar[3] |= BIT_D1;	else rc.m_GeneralVar[3] &= ~BIT_D1;	// マスター更新サイン(改良依頼No.190505 データベース修正) 1:更新済み
//// midori 155399 add <--
// 157270 del <--
// 157270 add -->
	if(bMstUpd)					rc.m_GeneralVar[3] |= BIT_D7;	else rc.m_GeneralVar[3] &= ~BIT_D7;	// マスター更新サイン
// 157270 add <--

// midori 155314 add -->
	if(bOmitUpd)				rc.m_GeneralVar[3] |= BIT_D0;	else rc.m_GeneralVar[3] &= ~BIT_D0;	// 印刷設定 省略文字 修復サイン		1:修復済み
// midori 155314 add <--
// midori 191002 add -->
	if (bKeiUpd)				rc.m_GeneralVar[3] |= BIT_D2;	else rc.m_GeneralVar[3] &= ~BIT_D2;	// 小計名称更新サイン		1:更新済み
// midori 191002 add <--
// midori UC_0060 add -->
	if(bOmitUpd2)				rc.m_GeneralVar[3] |= BIT_D4;	else rc.m_GeneralVar[3] &= ~BIT_D4;	// 印刷設定 省略文字 修復サイン２		1:修復済み
// midori UC_0060 add <--
// midori 157043 del -->
//// midori 156186 add -->
//	if (bKmkRowChk)				rc.m_GeneralVar[3] |= BIT_D5;	else rc.m_GeneralVar[3] &= ~BIT_D5;	// 画面表示「科目行」対応後、科目行処理方法変更メッセージ表示サイン　1:メッセージ表示済み　[勘定科目内訳書修正依頼20200611.xlsx] No.156186
//// midori 156186 add <--
// midori 157043 del <--
// midori 156188_3 add -->
	if (bKmkRowCnv)				rc.m_GeneralVar[3] |= BIT_D6;	else rc.m_GeneralVar[3] &= ~BIT_D6;	// 画面表示「科目行」対応後、科目行挿入済みサイン　　　　　　　　　　1:科目行挿入済み　　　[勘定科目内訳書修正依頼20200611.xlsx] No.156188
// midori 156188_3 add <--

// インボイス登録番号追加対応_23/11/06 add -->
	if(bUseStyle2)				rc.m_GeneralVar[4] |= BIT_D5;	else rc.m_GeneralVar[4] &= ~BIT_D5;	// 使用する様式		TRUE:新様式(登録番号追加版)	FALSE:旧様式(簡素化or旧)
// インボイス登録番号追加対応_23/11/06 add <--
}
