// ShinCalculateW.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include "DBZmSub8.h"
//#include "DBZmDef8.h"
#include "DBNpSub8.h"
#include "ICSGEN80.h"
#include "ICSGENL8.h"
#include "ICSARITH8.h"
#include "ICSTblsub8.h"
#include "VolDatedb8.h"
#include "Volume8.h"
#include "CoSel8.h"

// 消費税情報の変換後の定義ファイル
#include "DBSyzDef.h"
#include "DBSyzShinKoku.h"
//　消費税申告書　集計用変数
#include "DBShinDef.h"
#include "DBShinDef1.h"
#include "DBShinDef2.h"

#include "ShinSumilatePac8.h"
#include "ShinCalculateW.h"

static	short daytl[] =	 { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
static	char	GEN1[4] = { 0x08, 0x04, 0x01, 0 };	// ８年度改正年月日
static	char	GEN2[4] = { 0x09, 0x04, 0x01, 0 };	// ９年度改正年月日＆限界控除廃止年月日

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CFundsAddUpW
CShinCalculateW::CShinCalculateW()
{

	pZaimuSub = NULL;			// 財務クラス
	pVolDate = NULL;			// 期間関数クラス
	pTblHdl = NULL;				// テーブル関数クラス
	pArithEx = NULL;			// 倍長演算

	INP_S = 0;					// 開始処理経過月
	INP_E = 0;					// 最終処理経過月

	// 各サイン初期設定
	Kk_SG = 0x00;				// 期間短縮解除後の課税期間の開始日以降で申告書集計を行う。
	m_Middle = 0;				// 前年実績による中間申告サイン
	Anbun = 0;					// ﾎﾞﾘｭｰﾑﾗﾍﾞﾙ  1=個別対応  0=比例配分
	Srx = 0;					// 課税売上割合  1= 95%以下  0= 95%以上
	Kani = 0;					// 簡易課税選択時ＳＷ
	Skbn = 0;					// 申告区分
	s_tno = 0x00;
	d_tno = 0x00;				// 該当月   オフセット
	memset( SYsw, '\0', sizeof( SYsw ) );	// 消費税集計ｻｲﾝ 
	K221sw = 0;					// 規則２２条１項の適用（積み上げ方式）
	H301sw = 0;					// 法第３０条第１項の適用（積み上げ方式）
	SimpSw = 0;					// 簡易計算サイン
	Gsw = 0;					// 限界控除制度廃止期間チェックSW
	Kenei = 0,					// 兼営事業選択ＳＷ
	toku = 0;					// 特例計算適用ＳＷ
	N_ver = 0x00;
	Ver = 0x00;					// 消費税マスターバージョンＳＷ
	Kk_DM = 0x00;				// 期間短縮解除後の課税期間の開始日以降で申告書集計を行う。
	Ssofs = 0;
	Seofs = 0;
	WSGN = 0;
	Matu = 0;
	VdSw = 0;
	ItCt = 0;
	ErNo = 0;
	s_tbl = 0;
	e_tbl = 0;
	UP1 = 0;
	UP2 = 0;
	UP3 = 0;
	UP4 = 0;
	UP5 = 0;
	m_Specnt = 0;	// 特定収入マスター登録数
	m_Class = 0;	// クラスサイン
	m_SpcSg = 0;	// 特定収入処理サイン

	memset( SYMD, '\0', sizeof( SYMD ) );
	memset( EYMD, '\0', sizeof( EYMD ) );
	memset( CYMD, '\0', sizeof( CYMD ) );
	memset( IYMD, '\0', sizeof( IYMD ) );
	memset( dtl, '\0', sizeof( DAYPAC ) * 14 );
	memset( ymdtl, '\0', sizeof( DATES ) * 14 );

	// テーブルハンドリング後始末変数　初期設定
	foreA.tp_tno = -1;
	foreB.tp_tno = -1;
	foreC.tp_tno = -1;
	foreD.tp_tno = -1;
	KnTbl.tp_tno = -1;
	SpTbl.tp_tno = -1;
	m_ErroFlg = 0x00;		// エラーフラグ						
	ErrMessage.Empty();		// エラーメッセージ
	m_CheckFlg = 0x00;		// チェックフラグ
	CheckMessage.Empty();	// チェックメッセージ
	
	// マスターオープンフラグ
	m_OpenFlg1 = 0x00;
	m_OpenFlg2 = 0x00;
}

CShinCalculateW::~CShinCalculateW()
{
	// ボリューム・ラベル
	if( m_OpenFlg1&0x01 )	pZaimuSub->VolumeClose();
	// 科目名称
	if( m_OpenFlg1&0x02 )	pZaimuSub->KamokuMeisyoClose();
	// 科目残高
	if( m_OpenFlg1&0x04 )	pZaimuSub->KamokuZanClose();
	// 月情報テーブル
	if( m_OpenFlg1&0x08 )	pZaimuSub->MinfoClose();

	// 消費税管理
	if( m_OpenFlg2&0x01 )	pZaimuSub->SyohizeiClose();
	// 消費税申告書
	if( m_OpenFlg2&0x02 )	pZaimuSub->SzsnkClose();
	// 消費税残高集計ファイル
	if( m_OpenFlg2&0x04 )	pZaimuSub->SzsykClose();

#ifdef	VERSIONUP
	// 特定収入残高集計ファイル
	if( m_OpenFlg2&0x08 )	pZaimuSub->NpTokuteiSyunyuClose();
#endif

	// テーブルハンドリング
	if( foreA.tp_tno != ERR )	pTblHdl->th_close( foreA.tp_tno );
	if( foreB.tp_tno != ERR )	pTblHdl->th_close( foreB.tp_tno );
	if( foreC.tp_tno != ERR )	pTblHdl->th_close( foreC.tp_tno );
	if( foreD.tp_tno != ERR )	pTblHdl->th_close( foreD.tp_tno );
	if( KnTbl.tp_tno != ERR )	pTblHdl->th_close( KnTbl.tp_tno );
	if( SpTbl.tp_tno != ERR )	pTblHdl->th_close( SpTbl.tp_tno );
	// 期間関数クラス
	if( pVolDate != NULL ) {
		delete( pVolDate );		pVolDate = NULL;
	}
	// テーブル関数クラス
	if( pTblHdl != NULL ) {
		delete( pTblHdl );		pTblHdl = NULL;
	}
	// 倍長演算
	if( pArithEx != NULL ) {
		delete( pArithEx );		pArithEx = NULL;
	}
}

////////////////////////
// 課税売上高チェック //
////////////////////////

// 財務クラス
// type	:	0:各業務　試算表・消費税額試算表・消費税申告書・課税売上割合
//		:	1:翌期更新・前年度からの更新
// start:	開始年月日 西暦(long) 
//				[例]2011年 1月1日→20110101 
// end  :	最終年月日 西暦(long)
//				[例]2011年12月1日→20111231
// zmsub:	財務クラスのポインター
// 返送値
// -1: エラー
//	0: 平成２４年３月３１日以前の対象外及び個別対応及び簡易課税
//	1: ５億円超え
//  2: ４億五千万以上５億円未満
//  3: ４億五千万未満
//  4: みなし仕入率の変更メッセージ
int CShinCalculateW::CheckDBShin( int type, long start, long end, CDBZmSub* zmsub )
{
	m_Check = 1;
//★//[12'02.02]///
//	m_Type = type;
///////////////////
	switch( type )	{
		case 0 : m_Type = 0;	m_Kind = 0;	break;	// 各業務　／法人又は個人
		case 1 : m_Type = 1;	m_Kind = 0;	break;	// 更新業務／法人又は個人
		case 10: m_Type = 0;	m_Kind = 1;	break;	// 各業務　／法人 
		case 11: m_Type = 1;	m_Kind = 1;	break;	// 更新業務／法人
		case 20: m_Type = 0;	m_Kind = 2;	break;	// 各業務　／個人
		case 21: m_Type = 1;	m_Kind = 2;	break;	// 更新業務／個人
	}
///////////////////
	memset( Start, '\0', sizeof( Start ) );
	date_to_bcd( 0x04, Start, start );
	memset( End, '\0', sizeof( End ) );
	date_to_bcd( 0x04, End, end );
	pZaimuSub = ( CDBNpSub * )zmsub;
	m_symd = start;
	m_eymd = end;
	return( CheckShinkoku() );
}

// 非営利クラス
// type	:	0:各業務　試算表・消費税額試算表・消費税申告書・課税売上割合
//		:	1:翌期更新・前年度からの更新
// start:	開始年月日 西暦(long) 
//				[例]2011年 1月1日→20110101 
// end  :	最終年月日 西暦(long)
//				[例]2011年12月1日→20111231
// zmsub:	財務クラスのポインター
// 返送値
// -1: エラー
//	0: 平成２４年３月３１日以前の対象外及び個別対応及び簡易課税
//	1: ５億円超え
//  2: ４億五千万以上５億円未満
//  3: ４億五千万未満
//  4: みなし仕入率の変更メッセージ
int CShinCalculateW::CheckNPShin( int type, long start, long end, CDBNpSub* zmsub )
{
	m_Check = 1;
//★//[12'02.02]///
//	m_Type = type;
///////////////////
	switch( type )	{
		case 0 : m_Type = 0;	m_Kind = 0;	break;	// 各業務　／法人又は個人
		case 1 : m_Type = 1;	m_Kind = 0;	break;	// 更新業務／法人又は個人	
		case 10: m_Type = 0;	m_Kind = 1;	break;	// 各業務　／法人 	 
		case 11: m_Type = 1;	m_Kind = 1;	break;	// 更新業務／法人	 
		case 20: m_Type = 0;	m_Kind = 2;	break;	// 各業務　／個人	 
		case 21: m_Type = 1;	m_Kind = 2;	break;	// 更新業務／個人
	}
///////////////////
	memset( Start, '\0', sizeof( Start ) );
	date_to_bcd( 0x04, Start, start );
	memset( End, '\0', sizeof( End ) );
	date_to_bcd( 0x04, End, end );
	pZaimuSub = ( CDBNpSub * )zmsub;
	m_symd = start;
	m_eymd = end;
	return( CheckShinkoku() );
}

// エラーメッセージの取得
CString& CShinCalculateW::ErrMsgCheckShin( void )
{
	return ErrMessage;
}

// エラーメッセージの取得
CString& CShinCalculateW::GetMsgCheckShin( void )
{
	return CheckMessage;
}

///////////////////////
// 消費税 申告書集計 //
///////////////////////

// 財務クラス
// start	:	開始年月日 西暦(long) 
//					[例]2011年 1月1日→20110101 
// end		:	最終年月日 西暦(long)
//					[例]2011年12月1日→20111231
// suminfo	:	処理情報パケット
// sumlist	:	実額及び集計後の金額パケット
// zmsub	:	財務クラスのポインター
// 返送値
// -1: エラー
//	0: 平成２４年３月３１日以前の対象外
//	1: 申告書集計完了
int CShinCalculateW::CalculateDBShin( long start, long end, SUMINFO* suminfo, SUMLIST* sumlist, CDBZmSub* zmsub )
{
	m_Check = 0;	// ON=チェック OFF=計算
	m_Type = 0;		// 0:各業務 1:更新業務（チェックのみ有効）
	m_Kind = 0;		// 法人・個人の別	  （チェックのみ有効）
	m_Compare = suminfo->SumCompareSw;			// シミュレーションサイン	ON:行う			OFF:行わない
	m_Simple = suminfo->SumSimpleSw;			// 簡易計算サイン			ON:計算をする 　OFF:計算をしない
	m_Category = suminfo->SumCategorySw;		// 1=卸売 2=小売 3=製造 4=ｻｰﾋﾞｽ 5=その他
	m_Entry = suminfo->SumEntrySw;				// 仕入区分登録種別　ON:科目別・分類別に設定する。OFF:分類別のみ設定する
	m_BasisStock = suminfo->SumStockSw;			// 1=課税売上げに係る 2=非課税売上に係る 3=共通に係る
	m_SumSettleSw = suminfo->SumSettleSw;		// 消費税申告書集計時の最終月の集計方法(★13'08.02)
	memset( Start, '\0', sizeof( Start ) );
	date_to_bcd( 0x04, Start, start );
	memset( End, '\0', sizeof( End ) );
	date_to_bcd( 0x04, End, end );
	pZaimuSub = ( CDBNpSub * )zmsub;
	return( CalculateShinkoku( suminfo, sumlist ) );
}

// 公益クラス
// start	:	開始年月日 西暦(long) 
//					[例]2011年 1月1日→20110101 
// end		:	最終年月日 西暦(long)
//					[例]2011年12月1日→20111231
// suminfo	:	処理情報パケット
// sumlist	:	実額及び集計後の金額パケット
// zmsub	:	財務クラスのポインター
// 返送値
// -1: エラー
//	0: 平成２４年３月３１日以前の対象外
//	1: 申告書集計完了
int CShinCalculateW::CalculateNPShin( long start, long end, SUMINFO* suminfo, SUMLIST* sumlist, CDBNpSub* zmsub )
{
	m_Check = 0;	// ON=チェック OFF=計算
	m_Type = 0;		// 0:各業務 1:更新業務（チェックのみ有効）
	m_Kind = 0;		// 法人・個人の別     （チェックのみ有効）
	m_Compare = suminfo->SumCompareSw;			// シミュレーションサイン	ON:行う			OFF:行わない
	m_Simple = suminfo->SumSimpleSw;			// 簡易計算サイン			ON:計算をする	OFF:計算をしない
	m_Category = suminfo->SumCategorySw;		// 1=卸売 2=小売 3=製造 4=ｻｰﾋﾞｽ 5=その他
	m_Entry = suminfo->SumEntrySw;				// 仕入区分登録種別　ON:科目別・分類別に設定する。OFF:分類別のみ設定する
	m_BasisStock = suminfo->SumStockSw;			// 1=課税売上げに係る 2=非課税売上に係る 3=共通に係る
	m_SumSettleSw = suminfo->SumSettleSw;		// 消費税申告書集計時の最終月の集計方法(★13'08.02)
	memset( Start, '\0', sizeof( Start ) );
	date_to_bcd( 0x04, Start, start );
	memset( End, '\0', sizeof( End ) );
	date_to_bcd( 0x04, End, end );
	pZaimuSub = ( CDBNpSub * )zmsub;
	return( CalculateShinkoku ( suminfo, sumlist ) );
}

// エラーメッセージの取得
CString& CShinCalculateW::ErrMsgCalculateShin( void )
{
	return ErrMessage;
}

////////////////////////
// 課税売上高チェック //
////////////////////////
int CShinCalculateW::CheckShinkoku( void )
{
	//===========================================================================
	// 平成26年4月改正対応（2014.02.20）
	int	st;
	if( st = CheckShinkokuH26() ) {
		// 改正対応マスターは上記関数内で判定まで終えるので、ここで終了
		if( st == -2 )	st = 0;	// 対象外期間のマスターの返送値へ置き換え
		return st;
	}
	// 改正未対応マスターは、以下で今まで通りの判定を行う
	//===========================================================================

	// 消費税関係マスターの読み込み
	if( Shinkoku_Read() == ERR )	return( ERR );
//★//[12'08.01]///
//	// ９５％ルール改正対応期間チェック
//	if( TaxReformsCheck() == 0 )					return( 0 );
//	// 簡易課税チェックはチェック対象外
//	if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 0x02 )	return( 0 );
//	// 個別対応方式選択時はチェック対象外
//	if( pZaimuSub->zvol->s_sgn4 & 0x80 )			return( 0 );
//
//	// 申告書集計
//	memset( &SumData, '\0', sizeof( SUMDATA ) );
//	m_Calcusw = 0;
//	return( Shinkoku_Calcurate( &SumData ) );
///////////////////
	int	St = 0;
	// ５億円チェック
	memset( &SumData, '\0', sizeof( SUMDATA ) );
	m_Calcusw = 0;
	St = Shinkoku_Calcurate( &SumData );
	if( St == 1 || (St == 2 && m_Type) )	{
		// ９５％ルール改正対応期間チェック
		if( TaxReformsCheck() == 0 )					return( 0 );
		// 簡易課税チェックはチェック対象外
		if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 0x02 )	return( 0 );
		// 個別対応方式選択時はチェック対象外
		if( pZaimuSub->zvol->s_sgn4 & 0x80 )			return( 0 );
		return( St );
	}
	else	{
		if( (St == 3 || St == 2 ) && pZaimuSub->zvol->ss_ymd > 20120000 ) {
			// 免税点チェック
			// 期首及び６ヶ月の取得（特定期間）
			int		Smnth, Emnth;
			if( shin_ofst( &Vol1.SS_YMD[1], &Smnth, &Vol1, &Vol2 ) )	Smnth = 0;
			Emnth = ( Smnth + 5 );
			if( Emnth > 11 )	Emnth = 12;
			memset( Start, '\0', sizeof( Start ) );
			memset( End, '\0', sizeof( End ) );
			memmove( Start, &dtl[Smnth].Symd[0], 3 );
			memmove( End, &dtl[Emnth].Eymd[0], 3 );
			// 常に新規から始まる
			SyzShin_Make();
			TBhead.SVsasw = 0x01;
			if( (pZaimuSub->zvol->apno&0xf0) == 0x10 || pZaimuSub->zvol->ind_type )	Snh.Sn_KOJIN |= 0x01;
			// 課税期間セット
			memmove( Snh.Sn_KDAYS, Start, 3 );
			memmove( Snh.Sn_KDAYE, End, 3 );
			// 消費税情報設定
			memset( SYsw, '\0', sizeof( SYsw ) );
			for( int MoCt = 0; MoCt < 13; ++MoCt ) {
				SYsw[MoCt] = (Vol2.D_TBL[MoCt].C_KSW & 0x40) ? 0x01 : 0;
			}
			// 申告書集計
			memset( &SumData, '\0', sizeof( SUMDATA ) );
			m_Calcusw = 0;
			St = Shinkoku_CalcurateEX( &SumData );
		}
		return( St );
	}
///////////////////
}

///////////////////////
// 消費税 申告書集計 //
///////////////////////
int CShinCalculateW::CalculateShinkoku( SUMINFO* suminfo, SUMLIST* sumlist )
{
	// 消費税関係マスターの読み込み
	if( Shinkoku_Read() == ERR )	return( ERR );

	// ９５％ルール改正対応期間チェック
	m_TaxReforms = TaxReformsCheck();

	// 課税売上割合計算式パケット 初期設定
	memset( &SumRatio, '\0', sizeof( SUMRATIO ) );
	// 実額保存
	memmove( &SumInvent, &sumlist->SumInv, sizeof( SUMINVENT ) );				// 棚卸調整(実額)
	for( int i = 0; i != 4; i++ ) {
		memmove( &SumMony[i], &sumlist->SumMny[i], sizeof( SUMMONY ) );			// その他の金額(実額)
	}

	int	St = 0;
	if( !m_Compare )	{
		// シミュレーションを行わない。
		// 初期設定
		for(  m_Calcusw = 0; m_Calcusw != 4; m_Calcusw++ ) {
			memset( &sumlist->SumList[m_Calcusw], '\0', sizeof( SUMDATA ) );
		}
		////////////////////
		// 各マスター変換 //
		////////////////////
		// データベース会社情報をアトラス会社情報へコンバージョン
		ZaimuVolume_Cnv();
		// 消費税ボリュームラベル変換
		SyzVolume_Cnv();
		// 常に新規から始まる
		SyzShin_Make();
		TBhead.SVsasw = 0x01;
		if( (pZaimuSub->zvol->apno&0xf0) == 0x10 || pZaimuSub->zvol->ind_type )	Snh.Sn_KOJIN |= 0x01;
		// 課税期間セット
		memmove( Snh.Sn_KDAYS, Start, 3 );
		memmove( Snh.Sn_KDAYE, End, 3 );
		// 消費税情報設定
		memset( SYsw, '\0', sizeof( SYsw ) );
		for( int MoCt = 0; MoCt < 13; ++MoCt ) {
			SYsw[MoCt] = (Vol2.D_TBL[MoCt].C_KSW & 0x40) ? 0x01 : 0;
		}
		// 各サイン初期設定
		K221sw = 0;									// 規則２２条１項の適用積み上げ方式サイン
		H301sw = 0;									// 法第３０条第１項の適用積み上げ方式サイン
		SimpSw = m_Simple;							// 簡易計算サイン
		toku = 0;
		Kenei = 0x00;
		if( Vol1.S_SGN2 & 0x80 )	Kenei = 0x01;	// 兼業事業者
		else						Kenei = 0x00;	// 単一事業者
		Kani = (int)(Vol1.S_SGN2 & 0x0f);			// 業種区分
		if( Vol1.S_SGN4 & 0x80 )	Anbun = 1;		// 個別対応
		else						Anbun = 0;		// 比例配分
		// 消費税集計の為のサイン生成
		Skbn = (int)(Snh.Sn_SKKBN&0xff);
		// 課税売上割合に準ずる割合を有効サイン
		Snh.Sn_UPERS &= 0xfd;
		RATIO = 0x00;
		// 消費税集計
		ofs_get();
		// 申告書用ワークエリアの初期化
		so_read();		// 旧ﾊﾞｰｼﾞｮﾝ用
		sn_read();		// 新ﾊﾞｰｼﾞｮﾝ用
		m_Stock = 0;	// 比例配分方式選択時の個別対応計算（科目の仕入区分参照）
//		m_Kani = 0x00;	// 原則課税時の簡易課税計算（単一事業者）
		m_Kani = Vol1.S_SGN2;

		memset( &SumData, '\0', sizeof( SUMDATA ) );
		if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 2 )	m_Calcusw = 3;	// 簡易課税
		else	{
			if( pZaimuSub->zvol->s_sgn4 & 0x80 )	m_Calcusw = 1;	// 個別対応
			else									m_Calcusw = 2;	// 比例配分
		}
		memset( &SumData, '\0', sizeof( SUMDATA ) );
		if( (St = Shinkoku_Calcurate( &SumData )) != ERR )
			memmove( &sumlist->SumList[m_Calcusw], &SumData, sizeof( SUMDATA ) );
	}
	else	{
		// シミュレーションを行う
		// 申告書集計
		for(  m_Calcusw = 0; m_Calcusw != 4; m_Calcusw++ ) {
			// 初期設定
			memset( &sumlist->SumList[m_Calcusw], '\0', sizeof( SUMDATA ) );
			memset( &SumData, '\0', sizeof( SUMDATA ) );
			////////////////////
			// 各マスター変換 //
			////////////////////
			// データベース会社情報をアトラス会社情報へコンバージョン
			ZaimuVolume_Cnv();
			// 消費税ボリュームラベル変換
			SyzVolume_Cnv();
			// 常に新規から始まる
			SyzShin_Make();
			TBhead.SVsasw = 0x01;
			if( (pZaimuSub->zvol->apno&0xf0) == 0x10 || pZaimuSub->zvol->ind_type )	Snh.Sn_KOJIN |= 0x01;
			// 課税期間セット
			memmove( Snh.Sn_KDAYS, Start, 3 );
			memmove( Snh.Sn_KDAYE, End, 3 );
			// 消費税情報設定
			memset( SYsw, '\0', sizeof( SYsw ) );
			for( int MoCt = 0; MoCt < 13; ++MoCt ) {
				SYsw[MoCt] = (Vol2.D_TBL[MoCt].C_KSW & 0x40) ? 0x01 : 0;
			}
			// 各サイン初期設定
			K221sw = 0;									// 規則２２条１項の適用積み上げ方式サイン
			H301sw = 0;									// 法第３０条第１項の適用積み上げ方式サイン
			SimpSw = m_Simple;							// 簡易計算サイン
			toku = 0;
			Kenei = 0x00;
			if( Vol1.S_SGN2 & 0x80 )	Kenei = 0x01;	// 兼業事業者
			else						Kenei = 0x00;	// 単一事業者
			Kani = (int)(Vol1.S_SGN2 & 0x0f);			// 業種区分
			if( Vol1.S_SGN4 & 0x80 )	Anbun = 1;		// 個別対応
			else						Anbun = 0;		// 比例配分
			// 消費税集計の為のサイン生成
			Skbn = (int)(Snh.Sn_SKKBN&0xff);
			// 課税売上割合に準ずる割合を有効サイン
			Snh.Sn_UPERS &= 0xfd;
			RATIO = 0x00;
			// 消費税集計
			ofs_get();
			// 申告書用ワークエリアの初期化
			so_read();		// 旧ﾊﾞｰｼﾞｮﾝ用
			sn_read();		// 新ﾊﾞｰｼﾞｮﾝ用
			m_Stock = 0;	// 比例配分方式選択時の個別対応計算（科目の仕入区分参照）
			m_Kani = 0x00;		// 原則課税時の簡易課税計算（単一事業者）
			// シミュレーション情報の取得
			switch( m_Calcusw ) {
				case 0 : // 全額控除
					Anbun = 0;
					if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 2 )	{
						// 原則課税に強制
						Vol1.S_SGN2 &= 0xf0;
//						Vol1.S_SGN2 |= 0x01;
//★//[12'01.18]//
//						Kani = 0x01;
//////////////////
						Kani = 0x00;
//////////////////
					}
					break;
				case 1 : // 個別対応
					Anbun = 1;
					if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 2 )	{
						// 原則課税に強制
						Vol1.S_SGN2 &= 0xf0;
//						Vol1.S_SGN2 |= 0x01;
//★//[12'01.18]//
//						Kani = 0x01;
//////////////////
						Kani = 0x00;
//////////////////
						m_Stock = 1;// 簡易課税時（科目の仕入区分参照）
					}
					else	{
						if( !(pZaimuSub->zvol->s_sgn4 & 0x80) ) m_Stock = 1;// 比例配分方式選択時（科目の仕入区分参照）
					}
					break;
				case 2 : // 比例配分
					Anbun = 0;
					if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 2 )	{
						// 原則課税に強制
						Vol1.S_SGN2 &= 0xf0;
//						Vol1.S_SGN2 |= 0x01;
//★//[12'01.18]//
//						Kani = 0x01;
//////////////////
						Kani = 0x00;
//////////////////
					}
					break;
				default: // 簡易課税
					Anbun = 0;
					if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 2 )	{
						// 簡易課税
						m_Kani = Vol1.S_SGN2;
					}
					else	{		
						// 原則課税
						switch( m_Category )	{
							case 2 :	Vol1.S_SGN2 = 0x03;	m_Kani = 0x02;	break;
							case 3 :	Vol1.S_SGN2 = 0x04;	m_Kani = 0x03;	break;
							case 4 :	Vol1.S_SGN2 = 0x05;	m_Kani = 0x04;	break;
							case 5 :	Vol1.S_SGN2 = 0x06;	m_Kani = 0x05;	break;
							default:	Vol1.S_SGN2 = 0x02;	m_Kani = 0x01;	break;	
						}
					}
					Kani = Vol1.S_SGN2;
					break;
			}
			if(( St = Shinkoku_Calcurate( &SumData ) == ERR ) )	break;
			memmove( &sumlist->SumList[m_Calcusw], &SumData, sizeof( SUMDATA ) );
		}
	}

	// 課税売上割合計算式パケット
	memmove( &sumlist->SumRat, &SumRatio, sizeof( SUMRATIO ) );					// 課税売上割合計算式パケット
	// 実額保存
	memmove( &sumlist->SumInv, &SumInvent, sizeof( SUMINVENT ) );				// 棚卸調整(実額)
	for( int i = 0; i != 4; i++ ) {
		memmove( &sumlist->SumMny[i], &SumMony[i], sizeof( SUMMONY ) );			// その他の金額(実額)
	}
	if( TBhead.SVmzsw&0x01 )	sumlist->SumInv.InSw2 = 0x01;					// 経過措置処理あり

	return ( St );
}

//////////////
// 共通関数 //
//////////////

// 科目名称読み込み
int CShinCalculateW::KnTablRead( void )
{
		 int	thd, Cnt;
unsigned int	rsiz, rcnt;
unsigned long	tsiz;
struct _KN_REC	Knrec;
	
	rsiz = (unsigned int)sizeof( struct _KN_REC );						// レコード長
	rcnt = (unsigned int)(pZaimuSub->knrec->GetRecordCount() + 10);		// レコード数　科目名称の２倍
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	// テーブル イニシャライズ
	if( KnTbl.tp_tno == -1 ) {
		// テーブル イニシャライズ
		if ((thd = pTblHdl->th_open( tsiz, rsiz )) < 0)  return( ERR );
	}
	memset( &KnTbl, '\0', sizeof(struct TB_PAR) );	
	KnTbl.tp_tno  = thd;
	KnTbl.tp_rmax = rcnt;
	KnTbl.tp_rnum = 0;
	KnTbl.tp_rlen = rsiz;
	KnTbl.tp_koff = 0;
	KnTbl.tp_klen = 8;		/* 科目（内部）コード (８桁) */
	KnTbl.tp_dupm = 0;
	KnTbl.tp_sufm = 0;
	KnTbl.tp_cp   = 0;
	KnTbl.tp_err  = 0;

	// 消費税集計マスターの読み込み
	Cnt = 0;
	while( 1 ) {
		if( !Cnt )	{
			if( pZaimuSub->knrec->MoveFirst() == ERR )	return( ERR );
		}
		else	{
			if( pZaimuSub->knrec->MoveNext() == ERR )	break;
		}
		Cnt++;
		if( pZaimuSub->knrec->knvoid == 1 )	continue;
		memset( &Knrec, '\0', sizeof( struct _KN_REC ) );
		// 内部コード
		memset( Knrec.Kn_cod, '0', 8 );
		memmove( Knrec.Kn_cod, pZaimuSub->knrec->kncod, 8 );
		Knrec.Kn_att = pZaimuSub->knrec->knatt;			/* 科目属性（貸借サイン）		*/
		Knrec.Kn_sz1 = pZaimuSub->knrec->knsgn[0];		/* 消費税処理グループ区分		*/
		Knrec.Kn_sz2 = pZaimuSub->knrec->knsgn[1];		/* 消費税処理グループ区分明細	*/
		Knrec.Kn_sz3 = pZaimuSub->knrec->knsgn[2];		/* 消費税属性／自動分離			*/
		Knrec.Kn_sz4 = pZaimuSub->knrec->knsgn[3];		/* 消費税課税区分				*/
		Knrec.Kn_sz5 = pZaimuSub->knrec->knsgn[4];		/* 消費税仕入区分				*/
		Knrec.Kn_ctg = pZaimuSub->knrec->ind_ctg;		/* 業種番号						*/
		Knrec.Kn_prf1 = pZaimuSub->knrec->ind_prf1;		/* 青色項目番号					*/
		Knrec.Kn_prf2 = pZaimuSub->knrec->ind_prf2;		/* 白色項目番号					*/
		Knrec.Kn_prf3 = pZaimuSub->knrec->ind_prf3;		/* 月別							*/
		if( pTblHdl->th_add( &Knrec, &KnTbl ) )	return( ERR );
	}
	return( 0 );
}

// 仕入区分対象科目チェック
// kbn1		消費税区分
// kbn2		消費税区分明細
// sjsg		D0: 固定資産　ON=譲渡 OFF=取得
char CShinCalculateW::StockSignGet( char *kcode, char kbn1, char kbn2, char sjsg )
{
	char			Ans, Dmm;
	struct _KN_REC	Knrec;
	int				r;

	Ans = Dmm = 0x00;

	switch( kbn1 & 0xff ) {
		case 0x02 :	r = 0;	break;						// 売上
		case 0x01 :	r = ((kbn2&0x0f) == 0x01) ? 0 : 1;	// 流動負債
					break;
		case 0x08 :	r = ((kbn2&0x0f) == 0x03) ? 0 : 1;	// 貸倒損失
					break;
		case 0x04 : r = 1;								// 仕入
					break;
		case 0x11 :	// 固定資産の取得＆譲渡
					if( sjsg & 0x01 )		r = 0;		// 譲渡
					else					r = 1;		// 取得
					break;
		default   :	r = 1;								// その他
					break;
	}
	if( r ) {
		// 仕入科目
		memset( &Knrec, '\0', sizeof( struct _KN_REC ) );
		memmove( Knrec.Kn_cod, kcode, 8 );
		if( pTblHdl->th_acs( thRead, &Knrec, &KnTbl ) == 0 ) {
			// 科目区分参照条件は課税科目
			if( Knrec.Kn_sz5&0x0f )	Ans = Knrec.Kn_sz5;	// 登録有り
			else					{
				switch( m_BasisStock ) {				// 登録なし
				 case 2 :	Ans = 0x02;	break;			// 非課税売上に係る
				 case 3 :	Ans = 0x03;	break;			// 共通に係る
				 default:	Ans = 0x01;	break;			// 課税売上に係る
				}
			}
		}
	}
	return( Ans );
}

// 売上区分対象科目チェック
// kbn1		消費税区分
// kbn2		消費税区分明細
// sjsg		D0: 固定資産　ON=譲渡 OFF=取得
char CShinCalculateW::SalesSignGet( char *kcode, char kbn1, char kbn2, char sjsg )
{
	char			Ans, Dmm;
//★//[12'03.16]///
//	struct _KN_REC	Knrec;
///////////////////
	int				r;

	Ans = Dmm = 0x00;

	switch( kbn1 & 0xff ) {
		case 0x04 :	case 0x08 :	r = 0;	break;			//仕入・経費
		case 0x01 :	r = ((kbn2&0x0f) == 0x00) ? 0 : 1;	//流動資産
					break;
		case 0x02 :	r = ((kbn2&0x0f) == 0x05) ? 0 : 1;	//貸倒回収
					break;
		case 0x11 :	// 固定資産の取得＆譲渡
					if( sjsg & 0x01 )		r = 1;		// 譲渡
					else					r = 0;		// 取得
					break;
		default   :	r = 1;								// その他
					break;
	}
	if( r ) {
		// 売上科目
//★//[12'03.16]///
//		memset( &Knrec, '\0', sizeof( struct _KN_REC ) );
//		memmove( Knrec.Kn_cod, kcode, 8 );
//		if( pTblHdl->th_acs( thRead, &Knrec, &KnTbl ) == 0 ) {
//			// 科目区分参照条件は課税科目
//			if( Knrec.Kn_sz4&0x0f )	Ans = Knrec.Kn_sz4;	// 登録有り
//			else					Ans = m_Kani;		// 登録無し『選択事業種』
//		}
///////////////////
		Ans = m_Kani;
///////////////////
	}
	return( Ans );
}

// 財務ボリュームラベル情報の変換
void CShinCalculateW::ZaimuVolume_Cnv( void )
{
	// 初期設定
	memset( &Vol1, '\0', sizeof( struct _VOL1 ) );
	memset( &Vol2, '\0', sizeof( struct _VOL2 ) );
	memset( Month26, '\0', sizeof( Month26 ) );

	Vol1.APNO	= pZaimuSub->zvol->apno;											// 業種番号
	Vol1.M_VER	= 0x10;																// マスターバージョン
	switch( pZaimuSub->zvol->m_type&0xff ) {										// マスタータイプ
		case 0x11 : Vol1.M_TYPE = 0x00;	break; 
		default   : Vol1.M_TYPE = 0x10;	break;
	}
	cstring_to_char( Vol1.C_NAM, pZaimuSub->zvol->c_nam, 40 );						// 会社名
	date_to_bcd( 0x00, Vol1.SS_YMD, pZaimuSub->zvol->ss_ymd );						// 決算期間　自
	date_to_bcd( 0x00, Vol1.EE_YMD, pZaimuSub->zvol->ee_ymd );						// 決算期間　至
	date_to_bcd( 0x00, Vol1.CS_YMD, pZaimuSub->zvol->cs_ymd );						// 計算上の期首 年月日
	Vol1.SS_JSW	= pZaimuSub->zvol->ss_jsw;											// 開始状態
	date_to_bcd( 0x00, Vol1.IP_YMD, pZaimuSub->zvol->ip_ymd );						// 入力開始処理 年月日
	Vol1.S_MST	= pZaimuSub->zvol->s_mst;											// 消費税マスター登録サイン
	Vol1.S_VER	= pZaimuSub->zvol->s_ver;											// 〃			 バージョン
	Vol1.S_SGN1	= pZaimuSub->zvol->s_sgn1;											// 〃			 分離／税込み区分
	Vol1.S_SGN2	= pZaimuSub->zvol->s_sgn2;											// 〃			 課税方式区分
	Vol1.S_SGN3	= pZaimuSub->zvol->s_sgn3;											// 〃			 科目／摘要属性登録サイン
	Vol1.S_SGN4	= pZaimuSub->zvol->s_sgn4;											// 〃			 処理選択サイン
	Vol1.S_SGN5	= pZaimuSub->zvol->s_sgn5;											// 〃			 一括説抜きサイン
	Vol1.M_VER  = pZaimuSub->zvol->m_ver;											// マスターバージョン
	Vol1.IND_GK = pZaimuSub->zvol->ind_type;										// 個人決算書業種サイン

	if( Vol1.S_SGN2 == 0x01 )	Vol1.S_SGN2	= 0x00;									// 

	// 該当月情報
	int	i, month;
	month = i = 0;
	pZaimuSub->minfo->MoveFirst();
	do {
		switch( i ) {
			case 3 :	// 中間決算
			case 7 :
			case 11:
				break;
			case 15:	// 通常決算修正月
			default:
				Vol2.D_TBL[month]._INSW = pZaimuSub->minfo->insw;					// 入力許可サイン
				date_to_bcd( 0x01, Vol2.D_TBL[month]._SMD, pZaimuSub->minfo->smd );	// 開始月日
				date_to_bcd( 0x01, Vol2.D_TBL[month]._EMD, pZaimuSub->minfo->emd );	// 最終月日
				Vol2.D_TBL[month].C_KSW = pZaimuSub->minfo->c_ksw;					// 集計サイン
				Month26[month] = pZaimuSub->minfo->sz_syk;							// 消費税申告書中間サイン[新規サイン]
				month++;
				break;
		}
		i++;
		if( i == 16 )	break;

	} while( pZaimuSub->minfo->MoveNext() != -1 );

	// 	決算期間　年度変換
	//	西暦→平成
	memmove( SYMD, Vol1.SS_YMD, 3 );
	shin_datecnv( Vol1.SS_YMD[0], (unsigned char *)SYMD, 0 );
	memmove( EYMD, Vol1.EE_YMD, 3 );
	shin_datecnv( Vol1.EE_YMD[0], (unsigned char *)EYMD, 0 );
	memmove( CYMD, Vol1.CS_YMD, 3 );
	shin_datecnv( Vol1.CS_YMD[0], (unsigned char *)CYMD, 0 );
	memmove( IYMD, Vol1.IP_YMD, 3 );
	shin_datecnv( Vol1.IP_YMD[0], (unsigned char *)IYMD, 0 );

}

// 消費税ボリュームラベルの変換
void CShinCalculateW::SyzVolume_Cnv( void )
{
	memset( &TBhead, '\0', sizeof( struct _SVHED ) );

	TBhead.SVvers = 0x10;															// ファイルバージョン
	TBhead.SVrmax = (short)pZaimuSub->szvol->SVrmax;								// 税率最大登録数
	TBhead.SVrnum = (short)pZaimuSub->szvol->SVrnum;								// 税率最大登録数
	TBhead.SVzper[0] = (short)pZaimuSub->szvol->SVzper;								// 基本税率 ５%
	TBhead.SVzper[1] = (short)pZaimuSub->szvol->SVzper1;							// 経過税率 ３%
	TBhead.SVzper[2] = (short)pZaimuSub->szvol->SVzper2;							// 経過税率 ０%
	TBhead.SVzper[0] =	50;		// 基本税率 ５%
	TBhead.SVzper[1] =	30;		// 経過税率 ３%
	TBhead.SVzper[2] =	0;		// 経過税率 ０%
	TBhead.SVsper[0] = (short)pZaimuSub->szvol->SVsper1;							// 簡易課税 みなし仕入税率(卸売業)
	TBhead.SVsper[1] = (short)pZaimuSub->szvol->SVsper2;							// 簡易課税 みなし仕入税率(小売業)
	TBhead.SVsper[2] = (short)pZaimuSub->szvol->SVsper3;							// 簡易課税 みなし仕入税率(製造業)
	TBhead.SVsper[3] = (short)pZaimuSub->szvol->SVsper4;							// 簡易課税 みなし仕入税率(その他)
	TBhead.SVsper[4] = (short)pZaimuSub->szvol->SVsper5;							// 簡易課税 みなし仕入税率(ｻｰﾋﾞｽ業)	
	TBhead.SVzrto = 40;
	TBhead.SVlrto = 10;
	val_to_bin( TBhead.SVkan0, pZaimuSub->szvol->SVkan0 );							// 簡易課税適用の課税売上高下限
	val_to_bin( TBhead.SVkan1, pZaimuSub->szvol->SVkan1 );							// 簡易課税適用の課税売上高上限
	BYTE	stymd[4] = {0}, mymd[4] = {0};
	memmove( &TBhead.SVkcod[0], pZaimuSub->szvol->SVkcod1, 8 );						// 仮払消費税
	memmove( &TBhead.SVkcod[1], pZaimuSub->szvol->SVkcod2, 8 );						// 仮受消費税
	memmove( &TBhead.SVkcod[2], pZaimuSub->szvol->SVkcod3, 8 );						// 未収消費税
	memmove( &TBhead.SVkcod[3], pZaimuSub->szvol->SVkcod4, 8 );						// 未払消費税
	memmove( &TBhead.SVkcod[4], pZaimuSub->szvol->SVkcod5, 8 );						// 消費税
	memmove( &TBhead.SVkcod[5], pZaimuSub->szvol->SVkcod6, 8 );						// 繰延消費税
	memmove( &TBhead.SVkcod[6], pZaimuSub->szvol->SVkcod7, 8 );						// 固定資産売却益
	memmove( &TBhead.SVkcod[7], pZaimuSub->szvol->SVkcod8, 8 );						// 固定資産売却損
	TBhead.SVsasw = pZaimuSub->szvol->SVsasw;										// 申告書作成スイッチ
	TBhead.SVmzsw = pZaimuSub->szvol->SVmzsw;										// 複数税率処理スイッチ
	TBhead.SVhksw = pZaimuSub->szvol->SVhksw;										// 販売奨励金収入科目登録スイッチ
	TBhead.SV_5sw = pZaimuSub->szvol->SVd5sw;										// 第５種事業に係る経過措置に関するスイッチ
	TBhead.SV_5sw = 0x00;
	TBhead.SVkojn = pZaimuSub->szvol->SVkojn;										// 個人事業種選択スイッチ
	TBhead.SVzchg = pZaimuSub->szvol->SVzchg;										// 免税⇔課税業者切り替えスイッチ
	TBhead.SVkazei = pZaimuSub->szvol->SVkazei;										// 消費税　課税区分／仕入税額按分法スイッチ
	TBhead.SVsacq = pZaimuSub->szvol->SVsacq;										// 申告書集計スイッチ
	TBhead.SVzisq = pZaimuSub->szvol->SVzisq;										// 担当税理士番号スイッチ
	TBhead.SVsign = pZaimuSub->szvol->SVsign;										// 特殊処理サイン
	val_to_bin( TBhead.SVzuri, pZaimuSub->szvol->SVzuri );							// 基準期間の課税売上高
	// 中間申告情報
	memmove( TBhead.SVtmth, Month26, 12 );											// 中間申告管理テーブル
//★//[12'01.19]///
	TBhead.SVmzsw = 0x01;
///////////////////
}

// 消費税申告書ファイルの作成（登録）
void CShinCalculateW::SyzShin_Make( void )
{
	// 消費税申告書・ヘッダ－部イニシャライズ
	memset( &Snh, '\0', sizeof( SN_HEAD ) );
	// 消費税申告書・データ部イニシャライズ
	memset( &Snd, '\0', sizeof( SN_DATA ) );

	Snh.Sn_VER = 0x00;					// ５％改正
	Ver = N_ver = 0x02;
	Skbn = Snh.Sn_SKKBN = 1;			// 確定'にセット
	Snh.Sn_SEIRI[0]  = 0xff;			
	Snh.Sn_ZSEIRI[0] = 0xff;
	Snh.Sn_ITIREN[0] = 0xff;
	Snh.Sn_KOZA[0]	 = 0xff;
	
	if ( ! Snh.Sn_KDAYS[0] ) {
		memmove( Snh.Sn_KDAYS, Vol1.SS_YMD, 3 );
		shin_datecnv(Vol1.SS_YMD[0], Snh.Sn_KDAYS, 0 );
	}

	if ( ! Snh.Sn_KDAYE[0] ) {
		memmove( Snh.Sn_KDAYE, Vol1.EE_YMD, 3 );
		shin_datecnv( Vol1.EE_YMD[0], Snh.Sn_KDAYE, 0 );
	}

	// 課税売上割合の判定
	memset( RatioAsc, '\0', 6 );
	strcpy_s( RatioAsc, sizeof( RatioAsc ), "9500" );
	RatioInt = 9500;

//★//[12'01.28]///
	// 消費税申告書
	if( TBhead.SVsasw & 0x01 ) {
		Snh.Sn_VER		= pZaimuSub->szsnk->Sn_VER;										// ファイルバージョン
		Snh.Sn_KOJIN	= pZaimuSub->szsnk->Sn_KOJIN;									// 個人以外の事業者　ON:12ヶ月按分
		Snh.Sn_KOANB	= pZaimuSub->szsnk->Sn_KOANB;									// 個人事業者　ON:月按分　OFF:12ヶ月按分	
	}
///////////////////

}

//////////////////
// その他の関数 //
//////////////////

// 日付をＢＩＮからＢＣＤに変換（BIN側の年は西暦）
// ----------------------------------------------------------------------------
// date_to_bcd( char Type, unsigned char *d_ymd, long s_ymd	)
//	type	0x00 :  YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)(年は西暦)	
//			0x01 :	YYYY.MM.DD(BIN) →　MM.DD(BCD)
//			0x02 :	YYYY.MM.DD(BIN) →　MM(BCD)
//			0x03 :	YYYY.MM.DD(BIN) →　DD(BCD)
//			0x04 :  YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)(年は平成)
//  d_ymd	変換後の日付（ＢＣＤ）
//  s_yms	変換前の日付（ＢＩＮ）
//
void CShinCalculateW::date_to_bcd( char type, unsigned char *d_ymd, long s_ymd )
{
	char	asci[10], ansr[10];//, year[4];
	//int		yy;
	UCHAR	ucymd[4] = {0};
	int		gengo;

	memset( asci, '\0', sizeof( asci ) );
	memset( ansr, '\0', sizeof( ansr ) );
	if( !s_ymd || s_ymd == NULL ) return;				// [05'06.23]
	switch( type & 0xff ) {
		case 0x00 :	// YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)
			sprintf_s( asci, sizeof( asci ), "%08d", s_ymd );
			l_pack( ansr, &asci[2], 6 );
			memmove( d_ymd, ansr, 3 );
			break;
		case 0x01 :	// YYYY.MM.DD(BIN) →　MM.DD(BCD)
			sprintf_s( asci, sizeof( asci ), "%08d", s_ymd );
			l_pack( ansr, &asci[4], 4 );
			memmove( d_ymd, ansr, 2 );
			break;
		case 0x02 :	// YYYY.MM.DD(BIN) →　MM(BCD)
			sprintf_s( asci, sizeof( asci ), "%08d", s_ymd );
			l_pack( ansr, &asci[4], 2 );
			memmove( d_ymd, ansr, 1 );
			break;
		case 0x03 :	// YYYY.MM.DD(BIN) →　DD(BCD)
			sprintf_s( asci, sizeof( asci ), "%08d", s_ymd );
			l_pack( ansr, &asci[6], 2 );
			memmove( d_ymd, ansr, 1 );
			break;
		case 0x04 :	// YYYY.MM.DD(BIN) →　YY.MM.DD(BCD)
			// 西暦年を平成年に変換
			int_bcd( ucymd, (s_ymd%1000000), 3 );
			_db_datecnvGen( 0, ucymd, &gengo, d_ymd, 0, 0 );	// 西暦→和暦
			//sprintf_s( asci, sizeof( asci ), "%08d", s_ymd );
			//memset( year, '\0', sizeof( year ) );
			//memmove( year, &asci[2], 2 );
			//yy = atoi( year );
			//if( yy < 89 )	yy += 12;	/*  西暦２０００年以上  */
			//else			yy -= 88;	/*  　　　　〃　　未満  */
			//sprintf_s( year, sizeof( year ), "%02d", yy );
			//memmove( &asci[2], year, 2 );
			//l_pack( ansr, &asci[2], 6 );
			//memmove( d_ymd, ansr, 3 );
			break;
		default   :
			break;
	}
}

// 日付をＢＣＤからＢＩＮに変換（BIN側の年は西暦）
// YY.MM.DD(BCD) →　YYYY.MM.DD(BIN)(年は西暦)
// ----------------------------------------------------------------------------
// date_to_bin( int sg,  long *d_ymd, unsigned unsigned char *s_ymd )
// f_ymd	変換後日付（ＢＩＮ）
// s_ymd	変換前日付（ＢＣＤ）
// 
void CShinCalculateW::date_to_bin( int sg, long *d_ymd, unsigned char *s_ymd )
{
	char	year[4], month[4], date[4];
	long	yy, mm, dd;

	memset( year, '\0', sizeof( year ) );
	memset( month, '\0', sizeof( month ) );
	memset( date, '\0', sizeof( date ) );
	sprintf_s( year, sizeof( year ), "%02x",    (*s_ymd&0xff) );
	sprintf_s( month, sizeof( month ), "%02x",(*(s_ymd+1)&0x1f) );
	sprintf_s( date, sizeof( date ), "%02x",(*(s_ymd+2)&0xff) );
	yy = atoi ( year );
	mm = atoi ( month );
	dd = atoi ( date );
	if( sg == 0 ) {			// [07'06.01]
		if( yy == 0 || mm == 0 || dd == 0 )	{
			*d_ymd = 0L;
			return;
		}
	}
	else	{
		if( yy == 0 && mm == 0 && dd == 0 )	{
			*d_ymd = 0L;
			return;
		}
	}
	if( yy >= 12 )	yy = ( yy - 12 ) + 2000;	/*  西暦２０００年以上  */
	else			yy = ( yy + 88 ) + 1900;	/*  西暦２０００年未満  */
	yy *= 10000;
	mm *= 100;
	*d_ymd = ( yy + mm + dd );
}

// 金額変換（バイナリーより文字列に変換）
// ----------------------------------------------------------------------------
// void val_to_asci( CString *d_val, unsigned char *s_val )
//	d_val	金額（文字列）
//	s_val	金額（６倍長　バイナリ）
void CShinCalculateW::val_to_asci( CString *d_val, unsigned char *s_val )
{
	char	asci[20];

	d_val->Empty();
	memset( asci, '\0', sizeof( asci ) );
	l_print( asci, s_val, "SSSSSSSSSSSSS9" );
	*d_val += asci;
	d_val->TrimLeft();		// スペースカット
}

// 該当残高の集計（文字列よりバイナリーに変換）
// ----------------------------------------------------------------------------
// void val_to_bin( char *d_val, CString *s_val )
//	d_val	金額（６倍長　バイナリ）
//	s_val	金額（文字列）
//	-------------------------------------------------------------------
void CShinCalculateW::val_to_bin( unsigned char *d_val, CString s_val )
{
	char	asci[20];

	memset( d_val, '\0', 6 );
	memset( asci, '\0', sizeof( asci ) );
	strcpy_s( asci, (LPCTSTR)s_val );
	l_input( d_val, asci );
}

// 文字列をBCDにパック
// ----------------------------------------------------------------------------
// void asci_to_bcd( char *bcdbg, CString *ascbf, lnt lng )
//	char	*bcdbf			BCD
//	CString	*ascbf			文字列
//	int		lng				文字数
void CShinCalculateW::asci_to_bcd( unsigned char *bcdbf, CString *ascbf, int lng )
{
	char	asci[20], bcde[10];

	memset( asci, '\0', sizeof( asci ) );
	memset( bcde, '\0', sizeof( bcde ) );
	memmove( asci, *ascbf, lng );
	l_pack( bcde, asci, lng );
	memmove( bcdbf, bcde, lng );
}

///////////////////////////////////////////////////////////////////////////////
// 文字列転送（CString　から　Char へ）
// ----------------------------------------------------------------------------
// void cstring_to_char( unsigned char *d_asci, CString s_asci, int lng )
//	d_asci（char）
//	s_asci（CString）
//	-------------------------------------------------------------------
void CShinCalculateW::cstring_to_char( unsigned char *d_asci, CString s_asci, int lng )
{
	char	asci[128];
	int		cnt;

	memset( d_asci, '\0', lng );
	memset( asci, '\0', sizeof( asci ) );
	strcpy_s( asci, (LPCTSTR)s_asci );
	if( (cnt = (int)strlen( asci )) ) {
		if( lng < cnt )	cnt = lng;
		memmove( d_asci, asci, cnt );
	}
}

//		平成～西暦　変換
//	----------------------------
//	shin_datecnv( year, date, sign ) 
//	char	year;	年  変換前
//	char	*date;  年　変換後
//	char	sign;   変換方向
//		Status	 0 = OK!
//				-1 = ERROR
//
int CShinCalculateW::shin_datecnv( unsigned char year, unsigned char *date, int sign )
{
	unsigned	char	ymd[4], dchk[10];
	int		yy, y1;

	sprintf_s( (char *)dchk,  sizeof( dchk ),"%02x", year );
	y1 = atoi( (char *)dchk );
	if( y1 < 0 || y1 > 99 ) return( -1 );

	if( sign ) {	/*  平成→西暦  */
		if( y1 >= 12 )	yy = y1 - 12;	/*  西暦２０００年以上  */
		else			yy = y1 + 88;	/*  　　　　〃　　未満  */
		sprintf_s( (char *)dchk, sizeof( dchk ), "%02d",yy);
		l_pack( ymd, dchk, 2 );
		*date = ymd[0];
	}
	else	{		/*  西暦→平成  */
		if( y1 < 89 )	yy = y1 + 12;	/*  西暦２０００年以上  */
		else			yy = y1 - 88;	/*  　　　　〃　　未満  */
		sprintf_s( (char *)dchk, sizeof( dchk ), "%02d",yy);
		l_pack( ymd, dchk, 2 );
		*date = ymd[0];
	}
	return( 0 );
}

// 年月日の比較
//	(short)
//	shin_cmpymd( date1, date2 )
//	char	*date1,
//			*date2;
//	Status	1  >
//			0  =
//			-1 <
short CShinCalculateW::shin_cmpymd( char *date1, char *date2 )
{
unsigned int		year, mnth, day, ans1, ans2;
		 char		ASC[4];
	
	sprintf_s(ASC,sizeof( ASC ),"%02x",(*date1&0xff));	year = atoi( ASC );
	sprintf_s(ASC,sizeof( ASC ),"%02x",(*(date1+1)&0x1f));	mnth = atoi( ASC );
	sprintf_s(ASC,sizeof( ASC ),"%02x",(*(date1+2)&0xff));	day  = atoi( ASC );
	ans1 = ( year * 10000 ) + ( mnth * 100 ) + day;

	sprintf_s(ASC,sizeof( ASC ),"%02x",(*date2&0xff));		year = atoi( ASC );
	sprintf_s(ASC,sizeof( ASC ),"%02x",(*(date2+1)&0x1f));	mnth = atoi( ASC );
	sprintf_s(ASC,sizeof( ASC ),"%02x",(*(date2+2)&0xff));	day  = atoi( ASC );
	ans2 = ( year * 10000 ) + ( mnth * 100 ) + day;
	
	if( ans1 == ans2 )	return( 0 );
	if( ans1 <  ans2 )	return( -1 );
	return( 1 );
}

// 閏年計算用モジュール
short CShinCalculateW::shin_Feb( short yr )
{
	short	y,d;

	y = yr - 1900;
	d = 1 + y + ( y + 3 ) / 4;
	if( yr > 1900 ) --d;
	return(d%7);
}

// 閏年計算
//	(short)
//	shin_uday( eymd )
//	char *eymd; 期末年月日
//				（平成年度）
//	 status	 28 or 29
short CShinCalculateW::shin_uday( char *eymd )
{
	char	dchk[20];
	short		dyy,dy,dm,dd,i;

	// 期末年月日　西歴変換
	sprintf_s( dchk, sizeof( dchk ), "%02x",    (*eymd&0xff) );	dy = atoi ( dchk );
	sprintf_s( dchk, sizeof( dchk ), "%02x",(*(eymd+1)&0x1f) );	dm = atoi ( dchk );
	sprintf_s( dchk, sizeof( dchk ), "%02x",(*(eymd+2)&0xff) );	dd = atoi ( dchk );
	
	if( dy >= 12 )	dy = ( dy - 12 ) + 2000;	// 西暦２０００年以上
	else			dy = ( dy + 88 ) + 1900;	// 西暦２０００年未満

	if ( (dm == 1) || ( (dm == 2) && (dd <= 28) ) )	dyy = dy - 1;
	else			dyy = dy;
	i = shin_Feb(dyy);
	if (((shin_Feb(dyy+1) + 7 - i) % 7) 	== 1 ) 	return( 28 );
	else									return( 29 );
}

// 期末月計算
//	( short )
//	shin_mday( month, eymd )
//	char	month;  該当月(BCD)
//	char	*eymd; 期末年月日
//				（平成年度）
//		status  末日
short CShinCalculateW::shin_mday( char month, char *eymd )
{
	char	dchk[20];
	short		dmnth;

	sprintf_s( dchk, sizeof( dchk ), "%02x",(month&0x1f) );	dmnth = atoi ( dchk );
	if( dmnth == 2 )	return( (short)shin_uday( eymd ) );
	else				return( (short)daytl[ dmnth ] );
}

// 期間処理モジュール	初期設定
//	shin_iniz( Vl1, Vl2 )
//	struct	VOL_N1	*Vl1;	（管理情報部）
//	struct	VOL_N2	*Vl2;	（各月情報部）
short CShinCalculateW::shin_iniz( struct	_VOL1 *Vol1, struct _VOL2 *Vol2 )
{
	short		i;

	// 	決算期間　年度変換
	//	西暦→平成
	memmove( SYMD, Vol1->SS_YMD, 3 );
	shin_datecnv( Vol1->SS_YMD[0], (unsigned char *)SYMD, 0 );
	memmove( EYMD, Vol1->EE_YMD, 3 );
	shin_datecnv( Vol1->EE_YMD[0], (unsigned char *)EYMD, 0 );
	memmove( CYMD, Vol1->CS_YMD, 3 );
	shin_datecnv( Vol1->CS_YMD[0], (unsigned char *)CYMD, 0 );
	memmove( IYMD, Vol1->IP_YMD, 3 );
	shin_datecnv( Vol1->IP_YMD[0], (unsigned char *)IYMD, 0 );

	// 閏年設定
	daytl[2] = shin_uday( EYMD );
	
	// データ期間　算出
	memset( dtl, '\0', sizeof( DAYPAC ) * 13 );
	for( i = 0; i != 13; i++ ) {
		memmove(&dtl[i].Symd[1], Vol2->D_TBL[i]._SMD, 2);
		memmove(&dtl[i].Eymd[1], Vol2->D_TBL[i]._EMD, 2);
		// 年度　設定
		if( ((dtl[i].Symd[1]&0x1f) < (EYMD[1]&0x1f)) ||
				(((dtl[i].Symd[1]&0x1f) == (EYMD[1]&0x1f)) &&
					((dtl[i].Symd[2]&0xff) <= (EYMD[2]&0xff))) )
				dtl[i].Symd[0] = EYMD[0];
		else	dtl[i].Symd[0] = CYMD[0];
		if( ((dtl[i].Eymd[1]&0x1f) < (EYMD[1]&0x1f)) ||
				(((dtl[i].Eymd[1]&0x1f) == (EYMD[1]&0x1f)) &&
					((dtl[i].Eymd[2]&0xff) <= (EYMD[2]&0xff))) )
				dtl[i].Eymd[0] = EYMD[0];
		else	dtl[i].Eymd[0] = CYMD[0];
	}
	return( 0 );
}

// 月日よりデータ期間チェック
//	( short )
//	shin_mdchk( mnth, day, symd, eymd, eeymd ) 
//	char	mnth;	該当月
//	char	day;	該当日
//	char	*symd;	データ開始年月日
//	char	*eymd;	データ最終年月日
//	char	*eeymd;	期末年月日
//
//		status	0 :	該当データ期間内
//				1 : 該当データ期間外
short CShinCalculateW::shin_mdchk( char mnth, char day, char *symd, char *eymd, char *eeymd )
{
	short		matu;
	char		CDAY, ASC[10];

	// 末日チェック
	matu = shin_mday( (mnth&0x1f), eeymd );
	sprintf_s(ASC, sizeof( ASC ),"%02d",matu);
	l_pack( &CDAY, ASC, 2 );
//	if( (day&0xff) > (CDAY&0xff) )	return( -1 );

	if( (*(symd+1)&0x1f) == (*(eymd+1)&0x1f) )	{
	// 同一月のデータ期間
		if( (mnth&0x1f) == (*(symd+1)&0x1f) &&
				(*(symd+2)&0xff) <= (day&0xff) &&
						(*(eymd+2)&0xff) >= (day&0xff) )	return( 0 );
	}
	else {
	// 月またがりのデータ期間
		// データ開始より末日まで
		matu = shin_mday( (mnth&0x1f), eeymd );
		sprintf_s(ASC, sizeof( ASC ),"%02d",matu);
		l_pack( &CDAY, ASC, 2 );
		if( (mnth&0x1f) == (*(symd+1)&0x1f) &&
				(*(symd+2)&0xff) <= (day&0xff) &&
						(CDAY&0xff) >= (day&0xff) )	return( 0 );

		// １日よりデータ最終まで
		if( (mnth&0x1f) == (*(eymd+1)&0x1f) &&
				(day&0xff) >= 0x01 &&
					(*(eymd+2)&0xff) >= (day&0xff) )	return( 0 );
	}
	return( 1 );
}

// 月日よりオフセット返送
//	( short )
//	shin_ofst( date, ofst, Vol1, Vol2 )
//	char		*date;		月日
//	int			*ofst;		オフセット（返送）
//	struct	_VOL1	*Vol1;	（管理情報部）
//	struct	_VOL2	*Vol2;	（各月情報部）
//		status	 0		処理ＯＫ！
//				-1		設定不良	
short CShinCalculateW::shin_ofst( unsigned char *date, int *ofst, struct _VOL1 *Vol1, struct _VOL2 *Vol2 )
{
	short		mofs, eday, err;

	unsigned	char	MONTH, DAY, CMNTH;
				char	ASC[20];

	// 初期設定
	*ofst = 0;

	// 設定年月日　生成
	MONTH = ( *date & 0x9f );
	DAY   = ( *(date+1) & 0xff );

	// 申告書では００月は存在しない
	if( !(MONTH&0xff) )	return( -1 );

	// 初期設定 平成年度で１３ヶ月分のデータ管理テーブル作成
	shin_iniz( Vol1, Vol2 );

	if( (DAY&0xff) == 0xff ) {
	// 月のみ
		if( !(MONTH&0x1f) || (MONTH&0x80) )	{
		// 決算修正
			if( MONTH&0x1f )	{
				if( (MONTH&0x1f) <= 0x00 || (MONTH&0x1f) > 0x12 )	return( -1 );
				if( (MONTH&0x1f) != (EYMD[1]&0x1f) )				return( -1 );
			}
			mofs = 12;
		}
		else {
			for( mofs = 0, err = 0; mofs != 12; mofs++ ) {
				if( (Vol2->D_TBL[mofs]._EMD[0]&0x1f) == (MONTH&0x1f)) {
					err = 1;
					break;
				}
			}
			if( !err )	return( -1 );
		}
		// オフセット
		*ofst = (int)mofs;
	}
	else {
	// 月日
		// 閏年チェック
		if( (MONTH&0x1f) == 0x02 )	{
			sprintf_s( ASC, sizeof( ASC ), "%02x", (DAY&0xff) );
			eday = atoi( ASC );
			if( eday >	(short)shin_uday( EYMD ) )	return( -1 );
		}

		if( !(MONTH&0x1f) || (MONTH&0x80) ) {
		// 決算修正
			if( MONTH&0x1f ) {
				if( (MONTH&0x1f) <= 0x00 || (MONTH&0x1f) > 0x12 )	return( -1 );
				CMNTH = ( MONTH & 0x1f );
			}
			else	{
				if( (DAY&0xff) >= (dtl[12].Symd[2]&0xff) )
												CMNTH = (dtl[12].Symd[1]&0x1f);
				if( (DAY&0xff) <= (dtl[12].Eymd[2]&0xff) )
												CMNTH = (dtl[12].Eymd[1]&0x1f);
			}
				
			if ( !(shin_mdchk( CMNTH, DAY, dtl[12].Symd, dtl[12].Eymd, EYMD ) ) ) {
				mofs = 12;
			}
			else	return( -1 );
		}
		else	{
			// 期首～１２ヶ月目
			for( mofs = 0, err = 0; mofs != 12; mofs++ ) {

				if ( !(shin_mdchk( (MONTH&0x1f), DAY,
							dtl[mofs].Symd, dtl[mofs].Eymd, EYMD ) ) ) {
					err = 1;
					break;
				}
			}
			if( !err )	return( -1 );
		}
		// オフセット
		*ofst = (int)mofs;
	}
	return( 0 );
}

// 月日よりデータ期間 返送
//	( short )
//	shin_date( date, symd, eymd, Vol1, Vol2 )
//	char	*date;		月日
//	char	*symd;		データ開始
//	char	*eymd;		データ最終
//	struct	_VOL1	*Vol1;	（管理情報部）
//	struct	_VOL2	*Vol2;	（各月情報部）
//		status	 0		正常期間
//				-1		設定不良	
short CShinCalculateW::shin_date( unsigned char *date, unsigned char *symd, unsigned char *eymd, struct _VOL1 *Vol1, struct _VOL2 *Vol2 )
{
	short		mofs, err;

	unsigned	char	MONTH, DAY, CMNTH;
				char	ymd[4];

	// 初期設定
	memset( symd, '\0', 3 );
	memset( eymd, '\0', 3 );

	// 設定年月日　生成
	MONTH = ( *date & 0x9f );
	DAY   = ( *(date+1) & 0xff );

	// 初期設定	平成年度で１３ヶ月分のデータ管理テーブル作成
	shin_iniz( Vol1, Vol2 );

	if( (DAY&0xff) == 0xff ) {
	// 月のみ
		if( !(MONTH&0x1f) || (MONTH&0x80) )	{
	/*  ========
		決算修正
		========  */
			if( MONTH&0x1f )	{
				if( (MONTH&0x1f) <= 0x00 || (MONTH&0x1f) > 0x12 )	return( -1 );
				if( (MONTH&0x1f) != (EYMD[1]&0x1f) )		return( -1 );
			}
			mofs = 12;
			// １３ヶ月決修チェック
			memmove(symd,dtl[12].Symd,3);	// データ開始期間
			memmove(eymd,dtl[12].Eymd,3);	// データ最終期間
			*(symd+1) |= 0x80;
			if( (*symd&0xff) != (CYMD[0]&0xff) )	*(symd+1) |= 0x20;
			*(eymd+1) |= 0x80;
			if( (*eymd&0xff) != (CYMD[0]&0xff) )	*(eymd+1) |= 0x20;
		}
		else {
			// 期首～１２ヶ月目
			for( mofs = 0, err = 0; mofs != 12; mofs++ ) {
				if( (Vol2->D_TBL[mofs]._EMD[0]&0xff) == (MONTH&0x1f)) {
					memmove(symd,dtl[mofs].Symd,3);	// データ開始期間
					memmove(eymd,dtl[mofs].Eymd,3);	// データ最終期間
					if( (*symd&0xff) != (CYMD[0]&0xff) )
													*(symd+1) |= 0x20;
					if( (*eymd&0xff) != (CYMD[0]&0xff) )
													*(eymd+1) |= 0x20;
					err = 1;
					break;
				}
			}
			if( !err )	return( -1 );
		}
		ymd[0] = dtl[mofs].Eymd[0];
		ymd[1] = dtl[mofs].Eymd[1];
		if( mofs == 12 )	ymd[1] |= 0x80;
		if( (ymd[0]&0xff) != (CYMD[0]&0xff) )	ymd[1] |= 0x20;

		// 期首年月日チェック
		if( shin_cmpymd( dtl[mofs].Eymd, SYMD ) >= 0 )	return( 0 );
	}
	else {
		// 月日

		// １３ヶ月決修チェック
		if( !(MONTH&0x1f) || (MONTH&0x80) ) {
		// 決算修正
			if( MONTH&0x1f ) {
				if( (MONTH&0x1f) <= 0x00 || (MONTH&0x1f) > 0x12 )	return( -1 );
				CMNTH = ( MONTH & 0x1f );
			}
			else	{
				if( (DAY&0xff) >= (dtl[12].Symd[2]&0xff) )
												CMNTH = (dtl[12].Symd[1]&0x1f);
				if( (DAY&0xff) <= (dtl[12].Eymd[2]&0xff) )
												CMNTH = (dtl[12].Eymd[1]&0x1f);
			}
				
			if ( !(shin_mdchk( CMNTH, DAY, dtl[12].Symd, dtl[12].Eymd, EYMD ) ) ) {

				if( (CMNTH&0x1f) == (dtl[12].Symd[1]&0x1f) &&
								(DAY&0xff) >= (dtl[12].Symd[2]&0xff) )	{
					ymd[0] = dtl[12].Symd[0];			// 設定年の生成
					ymd[1] = dtl[12].Symd[1];			// 設定月の生成
				}
				else	{
					ymd[0] = dtl[12].Eymd[0];			// 設定年の生成
					ymd[1] = dtl[12].Eymd[1];			// 設定月の生成
				}
				ymd[2] = DAY;							// 設定日の生成
				ymd[1] |= 0x80;
				if( (ymd[0]&0xff) != (CYMD[0]&0xff) )	ymd[1] |= 0x20;

				memmove(symd,dtl[12].Symd,3);	// データ開始期間
				memmove(eymd,dtl[12].Eymd,3);	// データ最終期間
				*(symd+1) |= 0x80;
				if( (*symd&0xff) != (CYMD[0]&0xff) )	*(symd+1) |= 0x20;
				*(eymd+1) |= 0x80;
				if( (*eymd&0xff) != (CYMD[0]&0xff) )	*(eymd+1) |= 0x20;
				mofs = 12;
			}
			else	return( -1 );
		}
		else	{
			// 期首～１２ヶ月目
			for( mofs = 0, err = 0; mofs != 12; mofs++ ) {

				if ( !(shin_mdchk( (MONTH&0x1f), DAY,
							dtl[mofs].Symd, dtl[mofs].Eymd, EYMD ) ) ) {

														// 設定年の生成
					if( (MONTH&0x1f) == (dtl[mofs].Symd[1]&0x1f) )
										ymd[0] = dtl[mofs].Symd[0];
					else {
						if( (MONTH&0x1f) == (dtl[mofs].Eymd[1]&0x1f) )
										ymd[0] = dtl[mofs].Eymd[0];
						else			return( -1 );
					}
					ymd[1] = MONTH&0x1f;			// 設定月の生成
					ymd[2] = DAY;					// 設定日の生成
					if( (ymd[0]&0xff) != (CYMD[0]&0xff) ) ymd[1] |= 0x20;

					memmove(symd,dtl[mofs].Symd,3);	// データ開始期間
					memmove(eymd,dtl[mofs].Eymd,3);	// データ最終期間
					if( (*symd&0xff) != (CYMD[0]&0xff) )
														*(symd+1) |= 0x20;
					if( (*eymd&0xff) != (CYMD[0]&0xff) )
														*(eymd+1) |= 0x20;
					err = 1;
					break;
				}
			}
			if( !err )	return( -1 );
		}
	}
	return( 0 );
}

// 月日よりデータ生成 ＆ 月日チェック
//	( int )
//	shin_sign( date, ymd, Vol1, Vol2 )
//	char	*date;		月日
//	char	*ymd;		生成された年月日
//	struct	_VOL1	*Vol1;	（管理情報部）
//	struct	_VOL2	*Vol2;	（各月情報部）
//
//		status	 0		正常期間
//				-1		設定不良	
short CShinCalculateW::shin_sign( char *date, char *ymd, struct _VOL1 *Vol1, struct _VOL2 *Vol2 )
{
	int					mofs, err;

	unsigned	char	MONTH, DAY, CMNTH;

	// 初期設定
	memset( ymd, '\0', 3 );

	// 設定年月日　生成
	MONTH = ( *date & 0x9f );
	DAY   = ( *(date+1) & 0xff );

	// 初期設定	平成年度で１３ヶ月分のデータ管理テーブル作成
	shin_iniz( Vol1, Vol2 );

	// 決算修正で、日の設定が無ければ期末年月日の日を設定
	if( !(MONTH&0x1f) || (MONTH&0x80) )	{
		if( (DAY&0xff) == 0xff ) {
			if( MONTH&0x1f ) {
				if( (MONTH&0x1f) != (EYMD[1]&0x1f) )	return( -1 );
			}
			DAY = (EYMD[2]&0xff);
		}
	}

	if( (DAY&0xff) == 0xff ) {
	// 月のみ
		// 期首～１２ヶ月目
		for( mofs = 0, err = 0; mofs != 12; mofs++ ) {
			if( (Vol2->D_TBL[mofs]._EMD[0]&0x1f) == (MONTH&0x1f)) {
				err = 1;
				break;
			}
		}
		if( !err )	return( -1 );
		*ymd     = dtl[mofs].Eymd[0];
		*(ymd+1) = dtl[mofs].Eymd[1];
		if( mofs == 12 )	*(ymd+1) |= 0x80;
		if( (*ymd&0xff) != (CYMD[0]&0xff) )	*(ymd+1) |= 0x20;

		return( 0 );
	}
	else {

		if( !(MONTH&0x1f) || (MONTH&0x80) ) {
		// 決算修正
			if( MONTH&0x1f ) {
				if( (MONTH&0x1f) <= 0x00 || (MONTH&0x1f) > 0x12 )	return( -1 );
				CMNTH = ( MONTH & 0x1f );
			}
			else	{
				if( (DAY&0xff) >= (dtl[12].Symd[2]&0xff) )
												CMNTH = (dtl[12].Symd[1]&0xff);
				if( (DAY&0xff) <= (dtl[12].Eymd[2]&0xff) )
												CMNTH = (dtl[12].Eymd[1]&0xff);
			}
				
			if ( !(shin_mdchk( CMNTH, DAY, dtl[12].Symd, dtl[12].Eymd, EYMD ) ) ) {

				if( (CMNTH&0x1f) == (dtl[12].Symd[1]&0x1f) &&
								(DAY&0xff) >= (dtl[12].Symd[2]&0xff) )	{
					*ymd = dtl[12].Symd[0];				// 設定年の生成
					*(ymd+1) = dtl[12].Symd[1];			// 設定月の生成
				}
				else	{
					*ymd = dtl[12].Eymd[0];				// 設定年の生成
					*(ymd+1) = dtl[12].Eymd[1];			// 設定月の生成
				}
				*(ymd+2) = DAY;							// 設定日の生成
				*(ymd+1) |= 0x80;
				if( (*ymd&0xff) != (CYMD[0]&0xff) )	*(ymd+1) |= 0x20;
				mofs = 12;
			}
			else	return( -1 );
		}
		else	{
			// 期首～１２ヶ月目
			for( mofs = 0, err = 0; mofs != 12; mofs++ ) {

				if ( !(shin_mdchk( (MONTH&0x1f), DAY,
							dtl[mofs].Symd, dtl[mofs].Eymd, EYMD ) ) ) {

														// 設定年の生成
					if( (MONTH&0x1f) == (dtl[mofs].Symd[1]&0x1f) )
										*ymd = dtl[mofs].Symd[0];
					else {
						if( (MONTH&0x1f) == (dtl[mofs].Eymd[1]&0x1f) )
										*ymd = dtl[mofs].Eymd[0];
						else			return( -1 );
					}
					*(ymd+1) = MONTH&0x1f;			// 設定月の生成
					*(ymd+2) = DAY;					// 設定日の生成
					if( (*ymd&0xff) != (CYMD[0]&0xff) ) *(ymd+1) |= 0x20;
					err = 1;
					break;
				}
			}
			if( !err )	return( -1 );
		}
	}
	return( 0 );
}

//			決算期間より各月データ期間計算
//	-----------------------------------------------
//	( int )
//	shin_dateset( sdate, edate, sime, dapac )
//	unsigned
//	char			*sdate,	期首年月日
//					*edate,	期末年月日
//					sime;	締日
//	struct	_DAPAC	dapac[];	データ期間テーブル
//		status	 0 = 処理ＯＫ！
//				-1 = 期首期間入力ｅｒｒｏｒ
//				-2 = 期末期間入力ｅｒｒｏｒ
//				-3 = 期間範囲ｅｒｒｏｒ
int	CShinCalculateW::shin_dateset( unsigned char *sdate, unsigned char *edate, unsigned char sime, DAY_PAC dapac[] )
{
			int		i, j;
unsigned	long	sy, sm, sd, ey, em, ed, usi, sim, ksm, uy,
					syy, smm, sdd, eyy, emm, edd;
unsigned	char	ch0[20];

/*  ======================================================
	期首年月日を平成から西暦に変換＆チェック＆ＢＩＮに変換
	======================================================  */
	sprintf_s( (char *)ch0, sizeof( ch0 ), "%02x",(*sdate&0xff) );		sy  = atoi ( (char *)ch0 );
	sprintf_s( (char *)ch0, sizeof( ch0 ), "%02x",(*(sdate+1)&0xff) );	smm = atoi ( (char *)ch0 );
	sprintf_s( (char *)ch0, sizeof( ch0 ), "%02x",(*(sdate+2)&0xff) );	sdd = atoi ( (char *)ch0 );
	if( sy >= 12 )	syy = ( sy - 12 ) + 2000;	/*  西暦２０００年以上  */
	else			syy = ( sy + 88 ) + 1900;	/*  西暦２０００年未満  */

/*  ==============================================
	期末年月日を西暦に変換＆チェック＆ＢＩＮに変換
	==============================================  */
	sprintf_s( (char *)ch0, sizeof( ch0 ), "%02x",(*edate&0xff) );		ey  = atoi ( (char *)ch0 );
	sprintf_s( (char *)ch0, sizeof( ch0 ), "%02x",(*(edate+1)&0xff) );	emm = atoi ( (char *)ch0 );
	sprintf_s( (char *)ch0, sizeof( ch0 ), "%02x",(*(edate+2)&0xff) );	edd = atoi ( (char *)ch0 );
	if( ey >= 12 )	eyy = ( ey - 12 ) + 2000;	/*  西暦２０００年以上  */
	else			eyy = ( ey + 88 ) + 1900;	/*  西暦２０００年未満  */

/*  ============================================
	締日＆閏年計算　（以降ＢＩＮで年月日を設定）
	============================================  */
	sprintf_s( (char *)ch0, sizeof( ch0 ), "%02x",(sime&0xff) );	sim = atoi ( (char *)ch0 );
	if ( sim ) {
		if ( sim >= 28 )	sim = ksm = 0;
		else				ksm = sim + 1;
	}
	else	ksm = 0;

	/*  閏年の年度計算  */
	switch( emm ) {
		case 1 :
			usi = eyy - 1;
			break;
		case 2 :
			uy = eyy - 1;
			i = shin_Feb((short)uy);
			if (((shin_Feb((short)uy+1) + 7 - i) % 7) == 1 )	uy = 28;
			else									uy = 29;

			if( edd < 28 )					usi = eyy - 1;
			else	{
				if( edd == 28 && uy == 29 )	usi = eyy - 1;
				else						usi = eyy;
			}
			break;
		default:
			usi = eyy;
			break;
	}

	i = shin_Feb((short)usi);
	daytl[2] = 29;
	if (((shin_Feb((short)usi+1) + 7 - i) % 7) 	== 1 ) daytl[2] = 28;
	
/*  ====================
	決算期間範囲チェック
	====================  */
	if ( (smm > 12) || (smm < 1) || (emm > 12) || (emm < 1) )	return( -3 );
	if ( eyy != syy ) 	{
		if ( eyy != ( syy + 1 ) )								return( -3 );
		if ( smm < emm )										return( -3 );
		if ( (smm == emm) && ( sdd <= edd ) )					return( -3 );
	}
	else	if ( smm > emm )									return( -3 );

	if ( ((unsigned int)daytl[smm] < sdd) || ((unsigned int)daytl[emm] < edd) )				return( -3 );

	sy = ( syy * 10000 ) + ( smm * 100 ) + sdd;
	ey = ( eyy * 10000 ) + ( emm * 100 ) + edd;
	if ( sy >= ey ) 											return( -3 );	

/*  ==========================================
	データ期間用パケットアドレスがＮＵＬＬの時
	==========================================  */
	if(!(dapac) )												return( 0 );

/*  ========================
	期末月より計算上期首設定
	========================  */
	ymdtl[0].e_yy = eyy;	ymdtl[0].e_mm = emm;	ymdtl[0].e_dd = edd;
	if ( (unsigned int)daytl[emm] == edd ) {					/*  末日          */
		if ( emm == 12)	{
			ymdtl[0].s_yy = eyy;
			ymdtl[0].s_mm = 1;
			ymdtl[0].s_dd = 1;
		}
		else	{
			ymdtl[0].s_yy = eyy - 1;
			ymdtl[0].s_mm = emm + 1;
			ymdtl[0].s_dd = 1;
		}
	}
	else {										/*  中間日        */
			ymdtl[0].s_yy = eyy - 1;
			ymdtl[0].s_mm = emm;
			ymdtl[0].s_dd = edd + 1;
	}
	syy = ymdtl[0].s_yy;						/*  計算上期首年  */

/*  ========================
	期首が閏年の２月２９の時
	========================  */
	if( ymdtl[0].s_mm == 2 && ymdtl[0].s_dd == 29 ) {
		if( smm == 2 && sdd == 29 ) ;
		else	{
			ymdtl[0].s_mm = 3;
			ymdtl[0].s_dd = 1;
		}
	}

/*  ========================
	一ヶ月目のデータ期間設定
	========================  */
	if ( !sim ) {
		if ( ymdtl[0].s_dd == 1 ) {
			ymdtl[0].e_yy = ymdtl[0].s_yy;
			ymdtl[0].e_mm = ymdtl[0].s_mm;
			ymdtl[0].e_dd = daytl[ymdtl[0].s_mm];
		}
		else {
			if ( ymdtl[0].s_mm == 12 ) {
				ymdtl[0].e_yy = ymdtl[0].s_yy + 1;
				ymdtl[0].e_mm =  1;
				ymdtl[0].e_dd = 31;
			}
			else {
				ymdtl[0].e_yy = ymdtl[0].s_yy;
				ymdtl[0].e_mm = ymdtl[0].s_mm + 1;
				ymdtl[0].e_dd = daytl[ymdtl[0].e_mm];
			}
		}
	}
	else {
		if ( ymdtl[0].s_dd == 1 ) {
			ymdtl[0].e_yy = ymdtl[0].s_yy;
			ymdtl[0].e_mm = ymdtl[0].s_mm;
			ymdtl[0].e_dd = sim;
		}
		else {
			if ( ymdtl[0].s_mm == 12 ) {
				ymdtl[0].e_yy = ymdtl[0].s_yy + 1;
				ymdtl[0].e_mm =  1;
				ymdtl[0].e_dd = sim;
			}
			else {
				ymdtl[0].e_yy = ymdtl[0].s_yy;
				ymdtl[0].e_mm = ymdtl[0].s_mm + 1;
				ymdtl[0].e_dd = sim;
			}
		}
	}
/*  ============================
	２ヶ月目以降のデータ期間設定
	============================  */
	for ( j = 1; j != 12; j++ ) {
		sy = ymdtl[j-1].s_yy; sm = ymdtl[j-1].s_mm; sd = ymdtl[j-1].s_dd;
		ey = ymdtl[j-1].e_yy; em = ymdtl[j-1].e_mm; ed = ymdtl[j-1].e_dd;
		if ( !sim ) {
			if ( em == 12 ) {
				ymdtl[j].s_yy = eyy; ymdtl[j].s_mm = 1; ymdtl[j].s_dd = 1;
				ymdtl[j].e_yy = eyy; ymdtl[j].e_mm = 1;
				ymdtl[j].e_dd = 31;
			}
			else {
				ymdtl[j].s_mm = em + 1;
				if ( emm >= (unsigned int)ymdtl[j].s_mm ) ymdtl[j].s_yy = eyy;
				else 						ymdtl[j].s_yy = ymdtl[0].s_yy;
				ymdtl[j].s_dd = 1;
				ymdtl[j].e_yy = ymdtl[j].s_yy;
				ymdtl[j].e_mm = ymdtl[j].s_mm;
				ymdtl[j].e_dd = daytl[ymdtl[j].s_mm];
			}
		}
		else {
			if ( em == 12 ) {
				ymdtl[j].s_yy = syy; ymdtl[j].s_mm = 12; ymdtl[j].s_dd = ksm;
				ymdtl[j].e_yy = eyy; ymdtl[j].e_mm =  1; ymdtl[j].e_dd = sim;
			}
			else {
				ymdtl[j].s_yy = ymdtl[j-1].e_yy;
				ymdtl[j].s_mm = ymdtl[j-1].e_mm;
				ymdtl[j].s_dd = ksm;
				ymdtl[j].e_mm = em + 1;
				ymdtl[j].e_dd = sim;
				if ( emm >= (unsigned int)ymdtl[j].e_mm ) ymdtl[j].e_yy = eyy;
				else 						ymdtl[j].e_yy = ymdtl[0].s_yy;
			}
		}
	}
	ymdtl[11].e_dd = edd;

/*  ==========================
	１３ヶ月目のデータ期間設定
	==========================  */
	ymdtl[12].s_yy = ymdtl[11].s_yy;
	ymdtl[12].s_mm = ymdtl[11].s_mm;
	ymdtl[12].s_dd = ymdtl[11].s_dd;
	ymdtl[12].e_yy = ymdtl[11].e_yy;
	ymdtl[12].e_mm = ymdtl[11].e_mm;
	ymdtl[12].e_dd = ymdtl[11].e_dd;

/*  ============
	ＢＣＤに変換
	============  */
	for ( i = 0; i != 13; i++ ) {
		sy = ymdtl[i].s_yy;
		ey = ymdtl[i].e_yy;
		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%04d",sy);
		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%04d",ey);
/*  ================
	西暦を平成に変換
	================  */
		if( sy >= 2000 )	sy = ( sy - 2000 ) + 12;	/*  西暦２０００年以上  */
		else				sy = ( sy - 1988 );			/*  　　　　〃　　未満  */
		if( ey >= 2000 )	ey = ( ey - 2000 ) + 12;	/*  西暦２０００年以上  */
		else				ey = ( ey - 1988 );			/*  　　　　〃　　未満  */

		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%02d",sy);
		l_pack( &dapac[i].Symd[0], ch0, 2 );
		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%02d",ey);
		l_pack( &dapac[i].Eymd[0], ch0, 2 );
		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%02d",ymdtl[i].s_mm);
		l_pack( &dapac[i].Symd[1],ch0,2);
		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%02d",ymdtl[i].s_dd);
		l_pack( &dapac[i].Symd[2],ch0,2);
		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%02d",ymdtl[i].e_mm);
		l_pack( &dapac[i].Eymd[1],ch0,2);
		memset( ch0, 0, 20 );	sprintf_s( (char *)ch0, sizeof( ch0 ),"%02d",ymdtl[i].e_dd);
		l_pack( &dapac[i].Eymd[2],ch0,2);
	}
	return( 0 );
}

// 集計済みチェック
int CShinCalculateW::skchek( int sm, int km )
{
	int	i, last, stat, emon, smon;
	char	asci[20];

	if( (Vol1.M_TYPE&0xff) == 0x00 )	{
		// 月次
		if( ! SYsw[ km ] )	return( km );
		else				return( -1 );
	}
	else	{
		// 年次・合併
		// 開始月設定
		sprintf_s( asci,  sizeof( asci ),"%08d", pZaimuSub->zvol->ip_ymd );
		int	ymd = atoi( &asci[4] );
		if( pVolDate->db_vd_offset( 0, ymd, &stat, pZaimuSub ) )	smon = sm; 
		else	{
			if( sm < stat )	smon = stat;
			else			smon = sm;
		}
		// 最終月設定
		if( (Vol1.M_TYPE&0xff) == 0x00 )				emon = km;	// 月次
		else	{
			if( (Vol2.D_TBL[12]._INSW&0x0f) == 0x00 )	last = 11;
			else										last = 12;
			if( km > last )	emon = last;
			else			emon = km;
		}		
		for( i = smon; i <= emon; ++i ) {
			if( ! SYsw[i] )	return( i );
		}
	}
	
	return( -1 );	// 集計済み
}
// 入力年月　開始・最終設定
int	CShinCalculateW::ofs_get( void )
{
	int		s_no, d_no, tno;
	int		sno, i;
	unsigned char	ttk;
	
	if( Vol1.M_TYPE & 0x10 ) {	// 年次
		if( shin_ofst( &Snh.Sn_KDAYS[1], &i, &Vol1, &Vol2 ) )	return( TRUE );
		INP_S = s_tno = i;		
		if( Vol1.M_TYPE & 0x01 ) {	// 合併
			INP_E = d_tno = 12;
		}
		else	{
			for( i = 12; i >= 0; i-- ) {	// 最終年月オフセット
				if( Vol2.D_TBL[i]._INSW )	break;
			}
			INP_E = d_tno = i;
		}
	}
	else {	// 月次
		for( d_tno = 0; d_tno < 13; d_tno++ )	{
			if( Vol2.D_TBL[d_tno]._INSW )	break;
		}
		if( d_tno > 12 )	return( TRUE );		// 月次処理（該当月）？
		INP_S = INP_E = d_tno;
	}
	ttk = 0x13;
	if( Skbn % 2 )	ttk = Snh.Sn_KDAYE[1];		// 確定申告
	else			ttk = Snh.Sn_MDAYE[1];		// 中間申告
	if( ttk == Vol2.D_TBL[12]._EMD[0] || ttk == Vol2.D_TBL[11]._EMD[0] )	{
		if( Vol1.M_TYPE & 0x01 )	tno = 12;	// 合併
		else						tno = ( Vol2.D_TBL[12]._INSW ? 12 : 11 );
	}
	else {
		for( tno = 0; tno < 12; tno++ ) {
			if( ttk == Vol2.D_TBL[tno]._EMD[0] )	break;
		}
		if( tno >= 12 )	return( TRUE );		// 年次処理（該当月処理月無し！）
	}
	d_tno = tno;
	i = 1;
	sno = s_no = s_tno;
	switch( Skbn ) {
		case 1 : case 3 :		// 確定申告
			if( Snh.Sn_KZTNSK&0x0f ) { 	// 課税期間短縮 [04'03.04]
				if( shin_ofst( &Snh.Sn_KDAYS[1], &sno, &Vol1, &Vol2 ) )	return( TRUE );
				if( (Snh.Sn_KZTNSK&0x0f) == 0x02 )	{
					d_no = 12;
					if( Vol1.SS_JSW&0x01 ) {
						if( shin_ofst( &Vol1.EE_YMD[1], &d_no, &Vol1, &Vol2 ) )	return( TRUE );
						d_no++;	// 決算修正月
					}
					if( d_tno == d_no )	sno = d_tno - 1;
					else				sno = d_tno;
				}
				else	{
					if( (d_tno-sno) > 2 ) 
						sno = d_tno - ( d_tno == 12 ? 3 : 2 );
				}
				if( (Ver&0xff) == 1 ) {
					if( l_jsgn( Vol2.D_TBL[sno]._SMD, (char *)&GEN2[1], 2 ) >= 0 )
						Ver = 0x02;
				}
			}
//★//[12'01.13]///
//			else if( (! Vol1.SS_JSW && ! Vol2.D_TBL[0]._INSW) ||
//				((Vol1.APNO&0xff) == 0x10 && ! (Snh.Sn_KOANB&0x01)) ||
//				(Snh.Sn_KOJIN&0x01) )	i = sno = 0;
///////////////////
			break;
		default:				// 中間申告
			switch( Snh.Sn_MCOUNT ) {
				case 3 :	sno = d_tno - 2;	break;	// 年３回 
				case 11:	sno = d_tno;		break;	// 年11回
				default:	sno = d_tno - 5;	break;	// 年１回
			}
			if( ! Vol1.SS_JSW && ! Vol2.D_TBL[0]._INSW )	i = 0;
			break;
	}
	if( ! i || s_no < sno )	s_tno = sno;
	
	if( s_tno < 0 )		s_tno = 0;
	if( d_tno > 12 )	d_tno = 12;
	
	return( skchek( (s_no < sno ? sno : s_no), d_tno )+1 );
}

// 税抜・税込・一括経理処理時の込み・抜きの生成
void CShinCalculateW::IsManagement(  struct _SZREC *szrecp )
{
	int		i;
	char	Mony[6];

	switch( Vol1.S_SGN1 & 0xc0 )	{
		case 0x00 :	// 込みに
			for( i = 0; i != 13; i++ ) {
				memset( Mony, '\0', sizeof( Mony ) );
				l_add( Mony, szrecp->SZdtbl[i].SDval[5], szrecp->SZdtbl[i].SDval[6] );
				l_add( szrecp->SZdtbl[i].SDval[3], szrecp->SZdtbl[i].SDval[3], Mony );
				l_add( szrecp->SZdtbl[i].SDval[4], szrecp->SZdtbl[i].SDval[4], szrecp->SZdtbl[i].SDval[6] );
				memset( szrecp->SZdtbl[i].SDval[5], '\0', 6 );
				memset( szrecp->SZdtbl[i].SDval[6], '\0', 6 );
			}
			break;
		case 0x80 : // 抜き
			if( IsTaxDivide( szrecp->SZgrp1, szrecp->SZsjsg ) ) {
				// 抜き
				for( i = 0; i != 13; i++ ) {
					memset( Mony, '\0', sizeof( Mony ) );
					l_sub( Mony, szrecp->SZdtbl[i].SDval[3], szrecp->SZdtbl[i].SDval[4] );
					l_add( szrecp->SZdtbl[i].SDval[5], szrecp->SZdtbl[i].SDval[5], Mony );
					l_add( szrecp->SZdtbl[i].SDval[6], szrecp->SZdtbl[i].SDval[6], szrecp->SZdtbl[i].SDval[4] );
					memset( szrecp->SZdtbl[i].SDval[3], '\0', 6 );
					memset( szrecp->SZdtbl[i].SDval[4], '\0', 6 );
				}
			}
			else	{
				// 込み
				for( i = 0; i != 13; i++ ) {
					memset( Mony, '\0', sizeof( Mony ) );
					l_add( Mony, szrecp->SZdtbl[i].SDval[5], szrecp->SZdtbl[i].SDval[6] );
					l_add( szrecp->SZdtbl[i].SDval[3], szrecp->SZdtbl[i].SDval[3], Mony );
					l_add( szrecp->SZdtbl[i].SDval[4], szrecp->SZdtbl[i].SDval[4], szrecp->SZdtbl[i].SDval[6] );
					memset( szrecp->SZdtbl[i].SDval[5], '\0', 6 );
					memset( szrecp->SZdtbl[i].SDval[6], '\0', 6 );
				}
			}
			break;
		case 0xc0 :	// 一括税抜
			if( (K221sw && UriShiireGet( szrecp )) || (H301sw && !UriShiireGet( szrecp )) )	{
			// 積上げ方式				
				// 抜き
				for( i = 0; i != 13; i++ ) {
					memset( Mony, '\0', sizeof( Mony ) );
					l_sub( Mony, szrecp->SZdtbl[i].SDval[3], szrecp->SZdtbl[i].SDval[4] );
					l_add( szrecp->SZdtbl[i].SDval[5], szrecp->SZdtbl[i].SDval[5], Mony );
					l_add( szrecp->SZdtbl[i].SDval[6], szrecp->SZdtbl[i].SDval[6], szrecp->SZdtbl[i].SDval[4] );
					memset( szrecp->SZdtbl[i].SDval[3], '\0', 6 );
					memset( szrecp->SZdtbl[i].SDval[4], '\0', 6 );
				}
			}
			break;
		default   :
			break;
	}
}
// 税抜経理処理時の区分毎の税込み・税抜きの取得
// 返送地　OFF: 税込み
// 　　　　ON:  税抜き
int CShinCalculateW::IsTaxDivide( char Type, char Jsgn )
{
	int	Ans = 1;

	// 固定資産の譲渡は売上
	if( (Type&0xff) == 0x11 && (Jsgn&0xff) )	{
		if( !(pZaimuSub->zvol->s_sgn1 & 0x02) )	Ans = 0;
		return( Ans );
	}

	switch( Type & 0x0f ) {
		case 0x01 :	// 資産
			if( !(pZaimuSub->zvol->s_sgn1 & 0x01) )	Ans = 0;
			break;
		case 0x02 :	// 売上
			if( !(pZaimuSub->zvol->s_sgn1 & 0x02) )	Ans = 0;
			break;
		case 0x04 :	// 仕入
			if( !(pZaimuSub->zvol->s_sgn1 & 0x04) )	Ans = 0;
			break;
		case 0x08 :	// 経費
			if( !(pZaimuSub->zvol->s_sgn1 & 0x08) )	Ans = 0;
			break;
		default   : break;
	}
	return( Ans );
}
// 設立時の最終月のオフセットの取得
int CShinCalculateW::Establish( void )
{
	// 初期設定
	int	Last_mon = -1;
	int	Last_kmn =	0;
	int	emd =		0;

	if( pZaimuSub->zvol->ss_jsw ) {
		// 最終決算経過月の取得
		switch( pZaimuSub->zvol->kes_type ) {
			case 1 :	// 決算修正無し
				emd = (pZaimuSub->zvol->ee_ymd / 10000 ) * 10000;
				emd = pZaimuSub->zvol->ee_ymd - emd;
				if (pVolDate->db_vd_offset( 0, emd, &Last_mon, pZaimuSub ))	Last_mon = -1;
				break;
			case 2 :	// 半期決算
				if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 4, pZaimuSub ) ) == -1 ) {
					Last_mon = pVolDate->db_vd_qmoffset( 0, 2, pZaimuSub );
				}
				break;
			case 3 :	// 四半期決算
				if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 4, pZaimuSub ) ) == -1 ) {
					if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 3, pZaimuSub ) ) == -1 )	{
						if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 2, pZaimuSub ) ) == -1 )
								Last_mon = pVolDate->db_vd_qmoffset( 0, 1, pZaimuSub );
					}
				}
				break;
			default:	// 期末決算
				Last_mon = pVolDate->db_vd_qmoffset( 0, 4, pZaimuSub );
				break;
		}
	}
	return( Last_mon );
}



// 集計テ－ブル作成
int	CShinCalculateW::forcreat( struct TB_PAR *foreA, struct TB_PAR *foreB,  struct TB_PAR *foreC )
{
unsigned int	rsiz, rcnt;
unsigned long	tsiz;
		 int	thd, i;

	rsiz = (unsigned int)_SVL;			// レコード長
	rcnt = (unsigned int)AMAX;			// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	if( foreA->tp_tno == -1 ) {
		// テーブル イニシャライズ
		if ((thd = pTblHdl->th_open( tsiz, rsiz )) < 0)  return( 1 );// 『売上割合集計テーブルＡ』が作成できません。
		foreA->tp_tno  = thd;
	}

	foreA->tp_rmax = rcnt;
	foreA->tp_rnum = 0;
	foreA->tp_rlen = rsiz;
	foreA->tp_koff = 0;
	foreA->tp_klen = 4;
	foreA->tp_dupm = 0;
	foreA->tp_sufm = 0;
	foreA->tp_cp   = 0;
	foreA->tp_err  = 0;
	for( i = 0; i < AMAX; ++i ) {
		memset( &Asval, '\0', _SVL );
		if( ((Vol1.APNO&0xff) == 0x10) && i == 9 ) {	// 個人:受取利息
			Asval.pj_kbn1  = REC_TBL[29].sp_kbn1;
			Asval.pj_kbn2  = REC_TBL[29].sp_kbn2;
		}
		else {
			Asval.pj_kbn1  = REC_TBL[i].sp_kbn1;
			Asval.pj_kbn2  = REC_TBL[i].sp_kbn2;
		}
		Asval.pj_zino  = REC_TBL[i].sp_zino;
		Asval.pj_sjsg  = REC_TBL[i].sp_sjsg;

		if( pTblHdl->th_add( &Asval, foreA ) )	return( 2 );	// 売上割合集計テーブルＡ』に書き込みができません
	}
	
	rsiz = (unsigned int)_SUL;			// レコード長
	rcnt = (unsigned int)BMAX;			// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	if( foreB->tp_tno == -1 ) {
		// テーブル イニシャライズ
		if ((thd = pTblHdl->th_open( tsiz, rsiz )) < 0)  return( 3 );	// 『売上割合集計テーブルＢ』が作成できません
		foreB->tp_tno  = thd;
	}
	foreB->tp_rmax = rcnt;
	foreB->tp_rnum = 0;
	foreB->tp_rlen = rsiz;
	foreB->tp_koff = 0;
	foreB->tp_klen = 2;
	foreB->tp_dupm = 0;
	foreB->tp_sufm = 0;
	foreB->tp_cp   = 0;
	foreB->tp_err  = 0;

	rsiz = (unsigned int)sizeof( SZREC );								// レコード長
	rcnt = (unsigned int)(pZaimuSub->knrec->GetRecordCount() * 2);		// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	if( foreC->tp_tno == -1 ) {
		// テーブル イニシャライズ
		if ((thd = pTblHdl->th_open( tsiz, rsiz )) < 0)  return( 4 );
		foreC->tp_tno  = thd;
	}
	foreC->tp_rmax = rcnt;
	foreC->tp_rnum = 0;
	foreC->tp_rlen = rsiz;
	foreC->tp_koff = 0;
	foreC->tp_klen = 13;			// 譲渡サインまで
	foreC->tp_dupm = 0;
	foreC->tp_sufm = 0;
	foreC->tp_cp   = 0;
	foreC->tp_err  = 0;
	return( FALSE );
}

// 累積デ－タ リ－ド
int CShinCalculateW::rui_read( struct TB_PAR *foret,  struct TB_PAR *suktb,  struct TB_PAR *syztb )
{
	struct	S_VAL	srecp;
	struct _SZREC	szrec, *pszrec;
	char			Ascbf[64], Mony[6], *valp;
	int				i, j;
	// 初期設定

	// 仮払消費税集計エリア
	memset( &karibri_5, '\0', sizeof( struct _SZREC ) );		// ５％
	memmove( karibri_5.SZkcod, &TBhead.SVkcod[0], 8 );
	memset( &karibri_3, '\0', sizeof( struct _SZREC ) );		// ３％
	memmove( karibri_3.SZkcod, &TBhead.SVkcod[0], 8 );
	// 仮受消費税集計エリア
	memset( &kariuke_5, '\0', sizeof( struct _SZREC ) );		// ５％
	memmove( kariuke_5.SZkcod, &TBhead.SVkcod[1], 8 );
	kariuke_5.SZkatt = 0x80;
	memset( &kariuke_3, '\0', sizeof( struct _SZREC ) );		// ３％
	memmove( kariuke_3.SZkcod, &TBhead.SVkcod[1], 8 );
	kariuke_3.SZkatt = 0x80;

	// 輸入仕入集計エリア・売上区分／仕入区分エリアクリア
	memset( &SVsum, '\0', sizeof( struct _H9SUM ) );
	for( i = 0; i != 16; i++ )	{
		SVsum.SSstbl[i].SUskb = US_TBL[i].us_kbn1;
		SVsum.SSstbl[i].SUdkb = US_TBL[i].us_kbn2;
		if( 1 < 3 )
			memset( SVsum.SSstbl[i].SUtax, 0x01, 13 );			/* 4%での集計済みサイン */
	}
	// 棚卸調整額集計エリア
	Stack_Clear( stack_data );

	// 個別対応時の仕入金額
	memset( &kobetu_5, '\0', sizeof( struct _SZREC ) );		// ５％
	memset( &kobetu_3, '\0', sizeof( struct _SZREC ) );		// ３％

//★//[13'05.31]///
// 輸入仕税額保存
	memset( KazIp4, '\0', 6 );
	memset( KazIp3, '\0', 6 ); 
	memset( ComIp4, '\0', 6 ); 
	memset( ComIp3, '\0', 6 ); 
///////////////////

	//////////////////
	// 設立チェック //
	//////////////////
	int	Last_mon = -1;
	int	Last_kmn = 0;
	int	emd =	0;
	if( pZaimuSub->zvol->ss_jsw ) {
		// 最終決算経過月の取得
		switch( pZaimuSub->zvol->kes_type ) {
			case 1 :	// 決算修正無し
				emd = (pZaimuSub->zvol->ee_ymd / 10000 ) * 10000;
				emd = pZaimuSub->zvol->ee_ymd - emd;
				if (pVolDate->db_vd_offset( 0, emd, &Last_mon, pZaimuSub ))	Last_mon = -1;
				break;
			case 2 :	// 半期決算
				if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 4, pZaimuSub ) ) == -1 ) {
					Last_mon = pVolDate->db_vd_qmoffset( 0, 2, pZaimuSub );
				}
				break;
			case 3 :	// 四半期決算
				if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 4, pZaimuSub ) ) == -1 ) {
					if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 3, pZaimuSub ) ) == -1 )	{
						if( (Last_mon = pVolDate->db_vd_qmoffset( 0, 2, pZaimuSub ) ) == -1 )
								Last_mon = pVolDate->db_vd_qmoffset( 0, 1, pZaimuSub );
					}
				}
				break;
			default:	// 期末決算
				Last_mon = pVolDate->db_vd_qmoffset( 0, 4, pZaimuSub );
				break;
		}
	}

	// 消費税集計残高マスターの読み込み
	pZaimuSub->szsyk->MoveFirst();
	if( !pZaimuSub->szsyk->GetRecordCount() )	return( FALSE );
	do {
		// 精算残高のチェック　仮受消費税
		if( pZaimuSub->szsyk->SZgrp1 == 0x02 && pZaimuSub->szsyk->SZgrp2 == 0x0A )	continue;
		// 精算残高のチェック　仮払消費税
		if( pZaimuSub->szsyk->SZgrp1 == 0x04 && pZaimuSub->szsyk->SZgrp2 == 0x07 )	continue;
		// 総額一括税抜
		if( (pZaimuSub->zvol->s_sgn1&0xc0) == 0xc0 && (pZaimuSub->zvol->s_sgn5&0x10) && (pZaimuSub->szsyk->SZzchg&0x03) == 0x01 )	continue;

		// アトラス形式マスターに変換
		memset( &szrec, '\0', sizeof( struct _SZREC ) );
		// 内部コード変換
		memmove( szrec.SZkcod, pZaimuSub->szsyk->SZkcod, 8 );
		// 消費税処理グループ区分
		szrec.SZgrp1 = pZaimuSub->szsyk->SZgrp1;
		// 消費税処理グループ区分・明細
		szrec.SZgrp2 = pZaimuSub->szsyk->SZgrp2;
		// 税率テーブル番号
		szrec.SZzkbn = pZaimuSub->szsyk->SZzkbn;
		// 売上／仕入区分サイン
		szrec.SZitem = pZaimuSub->szsyk->SZitm;
		// 固定資産の譲渡サイン
		szrec.SZsjsg = (pZaimuSub->szsyk->SZsjsg&0x01);
		if( m_Calcusw == 3 ) {
			// 簡易課税
			if( (pZaimuSub->zvol->s_sgn2&0x0f) < 0x02 )	{
				// 単一事業者
				switch( SalesSignGet( (char *)szrec.SZkcod, szrec.SZgrp1, szrec.SZgrp2, szrec.SZsjsg ) )	{
					case 0x01 : szrec.SZitem = 0x01;	break;
					case 0x02 : szrec.SZitem = 0x02;	break;
					case 0x03 : szrec.SZitem = 0x03;	break; 
					case 0x04 : szrec.SZitem = 0x04;	break; 
					case 0x05 : szrec.SZitem = 0x05;	break; 
					default   : szrec.SZitem = 0x00;	break; 
				}
			}
		}
		else	{
			// 原則課税で個別対応
			if( m_Stock ) {
				switch( StockSignGet( (char *)szrec.SZkcod, szrec.SZgrp1, szrec.SZgrp2, szrec.SZsjsg ) )	{
					case 0x01 : szrec.SZitem = 0x10;	break;
					case 0x02 : szrec.SZitem = 0x20;	break;
					case 0x03 : szrec.SZitem = 0x30;	break; 
					default   : szrec.SZitem = 0x00;	break; 
				}
			}
		}
		//	貸借サイン
		if( pZaimuSub->szsyk->SZkatt & 0x01 )	szrec.SZkatt = 0x80;
		// 金額を文字列からバイナリーへ変換
		for( i = 0, j = 0; i < 16; i++ ) {	// 月変換 (naG)
			// 除外チェック
			switch( i ) {
				case 0 : j = 0;	break;
				case 1 : j = 1;	break;
				case 2 : j = 2;	break;
				case 3 : if( d_tno != 2 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 2;	
						 break;
				case 4 : j = 3;	break;
				case 5 : j = 4;	break;
				case 6 : j = 5;	break;
				case 7 : if( d_tno != 5 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 5;
						 break;
				case 8 : j = 6;	break;
				case 9 : j = 7;	break;
				case 10: j = 8;	break;
				case 11: if( d_tno != 8 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 8;
						 break;
				case 12: j = 9;	break;
				case 13: j =10;	break;
//★//[13'07.25]///
//				case 14: j =11;	break;
//				case 15: j =12;	break;
///////////////////
				case 14: if( m_Check == 0 && m_SumSettleSw == 1 )	continue;		// 消費税申告書集計時の最終月の集計方法(★13'08.02)
						 else	j = 11;
						 break;
				case 15: if( m_Check == 0 && m_SumSettleSw == 0 )	continue;		// 消費税申告書集計時の最終月の集計方法(★13'08.02)
						 else	j = 12;
						 break;
///////////////////

			}
			// 設立時のチェックLast_kmn
			if( Last_mon != -1 ) {
				if( Last_mon == i ) j = Last_mon - 1;
				if( Last_mon == i ) {
					if( shin_ofst( &Vol1.EE_YMD[1], &j, &Vol1, &Vol2 ) )	j = 0;
				}
			}
			// 対象外
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			strcpy_s( Ascbf, pZaimuSub->szsyk->SZtaig[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[0], Mony );
			// 非課税
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			strcpy_s( Ascbf,  pZaimuSub->szsyk->SZhikz[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[1], szrec.SZdtbl[j].SDval[1], Mony );
			// 有価証券５％
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			memset( Mony, '\0', sizeof( Mony ) );
			strcpy_s( Ascbf,  pZaimuSub->szsyk->SZyuka[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[2], szrec.SZdtbl[j].SDval[2], Mony );
			// 込み
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			memset( Mony, '\0', sizeof( Mony ) );
			strcpy_s( Ascbf, pZaimuSub->szsyk->SZkomi[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[3], szrec.SZdtbl[j].SDval[3], Mony );
			// 込み税額
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			memset( Mony, '\0', sizeof( Mony ) );
			strcpy_s( Ascbf,  pZaimuSub->szsyk->SZkomz[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[4], szrec.SZdtbl[j].SDval[4], Mony );
			// 抜き
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			memset( Mony, '\0', sizeof( Mony ) );
			strcpy_s( Ascbf,  pZaimuSub->szsyk->SZnuki[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[5], szrec.SZdtbl[j].SDval[5], Mony );
			// 抜き税額
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			memset( Mony, '\0', sizeof( Mony ) );
			strcpy_s( Ascbf, pZaimuSub->szsyk->SZnukz[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[6], szrec.SZdtbl[j].SDval[6], Mony );
			// 別記
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			memset( Mony, '\0', sizeof( Mony ) );
			strcpy_s( Ascbf,  pZaimuSub->szsyk->SZbeki[i] );
			l_input( Mony, Ascbf );
			if( szrec.SZkatt & 0x80 )	l_neg( Mony );		// [06'11.08]
			l_add( szrec.SZdtbl[j].SDval[7], szrec.SZdtbl[j].SDval[7], Mony );
			j++;
		}
		if( pTblHdl->th_acs( thWritex, &szrec, syztb ) )	return( ERR );
		// 輸入仕入
		if( szrec.SZgrp1 == 0x04 && szrec.SZgrp2 == 0x04 )	{
			Import_Stock( &SVsum, &szrec );
			// 売上区分／仕入区分集計
			Kubun_Stock( &szrec, suktb );
			continue;
		}
		// 棚卸調整額
		if( (szrec.SZgrp1 == 0x04) && (szrec.SZgrp2 == 0x05 || szrec.SZgrp2 == 0x06 ) )	{
			IsManagement( &szrec );						// 税抜・税込・一括経理処理時の込み・抜きの生成
			Stack_Stock( &szrec, stack_data );
			continue;
		}
		// 仮払消費税額（対象外に格納）
		if( !strncmp( (char *)&TBhead.SVkcod[0], (char *)szrec.SZkcod, 8-2 ) ) {	// 仮払消費税
			if( szrec.SZzkbn & 0x0f )	pszrec = &karibri_3;	// ３％
			else						pszrec = &karibri_5;	// ５％
			for( j = 0; j != 13; j++ ) {
				l_add( pszrec->SZdtbl[j].SDval[0], pszrec->SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[0] );
			}
			continue;
		}
		// 仮受消費税（対象外に格納）
		if( !strncmp( (char *)&TBhead.SVkcod[1], (char *)szrec.SZkcod, 8-2 ) ) 	{	// 仮受消費税
			if( szrec.SZzkbn & 0x0f )	pszrec = &kariuke_3;	// ３％
			else						pszrec = &kariuke_5;	// ５％
			for( j = 0; j != 13; j++ ) {
				l_add( pszrec->SZdtbl[j].SDval[0], pszrec->SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[0] );
			}
			continue;
		}
		// 個別対応時の仕入金額（輸入仕入及び棚卸調整額は集計しない）
		if( iskobetm() ) {
			if( szrec.SZzkbn & 0x0f )	pszrec = &kobetu_3;	// ３％
			else						pszrec = &kobetu_5;	// ５％
			for( j = 0; j != 13; j++ ) {
				switch( szrec.SZitem&0xf0 )	{
					case 0x10 :	// 課税に係る
						l_add( pszrec->SZdtbl[j].SDval[0], pszrec->SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[3] ); // 込み
						l_add( pszrec->SZdtbl[j].SDval[0], pszrec->SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[5] ); // 抜き
						l_add( pszrec->SZdtbl[j].SDval[0], pszrec->SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[6] ); // 抜き税額
						l_add( pszrec->SZdtbl[j].SDval[1], pszrec->SZdtbl[j].SDval[1], szrec.SZdtbl[j].SDval[7] ); // 別記
						break;													   
					case 0x20 : // 非課税に係る									   
						l_add( pszrec->SZdtbl[j].SDval[2], pszrec->SZdtbl[j].SDval[2], szrec.SZdtbl[j].SDval[3] ); // 込み
						l_add( pszrec->SZdtbl[j].SDval[2], pszrec->SZdtbl[j].SDval[2], szrec.SZdtbl[j].SDval[5] ); // 抜き
						l_add( pszrec->SZdtbl[j].SDval[2], pszrec->SZdtbl[j].SDval[2], szrec.SZdtbl[j].SDval[6] ); // 抜き税額
						l_add( pszrec->SZdtbl[j].SDval[3], pszrec->SZdtbl[j].SDval[3], szrec.SZdtbl[j].SDval[7] ); // 別記
						break;													   
					case 0x30 : // 共通に係る									   
						l_add( pszrec->SZdtbl[j].SDval[4], pszrec->SZdtbl[j].SDval[4], szrec.SZdtbl[j].SDval[3] ); // 込み
						l_add( pszrec->SZdtbl[j].SDval[4], pszrec->SZdtbl[j].SDval[4], szrec.SZdtbl[j].SDval[5] ); // 抜き
						l_add( pszrec->SZdtbl[j].SDval[4], pszrec->SZdtbl[j].SDval[4], szrec.SZdtbl[j].SDval[6] ); // 抜き税額
						l_add( pszrec->SZdtbl[j].SDval[5], pszrec->SZdtbl[j].SDval[5], szrec.SZdtbl[j].SDval[7] ); // 別記
						break;
					default   :
						break;
				}
			}
		}

		// アトラスへ変換
		memset( &srecp, '\0', sizeof( struct S_VAL ) );
		// 消費税処理グループ区分
		srecp.pj_kbn1 = szrec.SZgrp1;
		// 消費税処理グループ区分・明細
		srecp.pj_kbn2 = szrec.SZgrp2;
		// 税率テーブル番号
		if( (szrec.SZzkbn&0x0f) == 0x02 )	srecp.pj_zino = (szrec.SZzkbn&0x0f);
		// 譲渡サイン
		srecp.pj_sjsg = szrec.SZsjsg;
		// 販売奨励金収入は売上返品に変更
		if( srecp.pj_kbn1 == 0x02 && srecp.pj_kbn2 == 0x06 )	srecp.pj_kbn2 = 0x01;
		// 売上割引は売上返品に変更
		if( srecp.pj_kbn1 == 0x02 && srecp.pj_kbn2 == 0x07 )	srecp.pj_kbn2 = 0x01;
		// 販売奨励金費用は仕入返品に変更
		if( srecp.pj_kbn1 == 0x04 && srecp.pj_kbn2 == 0x02 )	srecp.pj_kbn2 = 0x01;
		// 仕入割引は仕入返品に変更
		if( srecp.pj_kbn1 == 0x04 && srecp.pj_kbn2 == 0x03 )	srecp.pj_kbn2 = 0x01;
		// レコード検索
		if( pTblHdl->th_acs( thRead, &srecp, foret ) ) {
			if( (srecp.pj_kbn1 == 0x02) && srecp.pj_zino ) {	// 売上ﾁｪｯｸ
				srecp.pj_kbn2 = 0x00;							// 輸出売上
				if( pTblHdl->th_acs( thRead, &srecp, foret ) )	continue;
			}
			else	continue;
		}
		if( srecp.pj_kbn1 != 0x08 ) {	// 経費は借方『★育成費用は経費でも唯一貸方なので注意』
			if( szrec.SZkatt&0x80 ) srecp.pj_ksign |= 0x80;
		}
		// 輸入売上値引・戻り貸方設定
		if( (szrec.SZgrp1&0xff) == 0x02 && (szrec.SZgrp2&0xff) == 0x01 &&
					(szrec.SZzkbn&0x0f) == 0x02 )	srecp.pj_ksign |= 0x80;
		// 建設会計の材料仕入値引き・戻しは仕入扱い
		if( (Vol1.APNO&0xf0) == 0x20 ) {
			if( (szrec.SZgrp1&0xff) == 0x04 && (szrec.SZgrp2&0xff) == 0x00 )	
														srecp.pj_ksign &= 0x7f;
		}
		// 宗教法人の『前期末建設勘定』は借方扱いをする
		if( (Vol1.APNO&0xff) == 0x59 ) {
			if( (szrec.SZgrp1&0xff) == 0x04 && (szrec.SZgrp2&0xff) == 0x00 )
														srecp.pj_ksign &= 0x7f;
		}
		// 税抜経理処理＆税込み経理処理時の込み・抜きの生成
		switch( Vol1.S_SGN1 & 0xc0 )	{
			case 0x00 :	// 込みに
				for( i = 0; i != 13; i++ ) {
					memset( Mony, '\0', sizeof( Mony ) );
					l_add( Mony, szrec.SZdtbl[i].SDval[5], szrec.SZdtbl[i].SDval[6] );
					l_add( szrec.SZdtbl[i].SDval[3], szrec.SZdtbl[i].SDval[3], Mony );
					l_add( szrec.SZdtbl[i].SDval[4], szrec.SZdtbl[i].SDval[4], szrec.SZdtbl[i].SDval[6] );
					memset( szrec.SZdtbl[i].SDval[5], '\0', 6 );
					memset( szrec.SZdtbl[i].SDval[6], '\0', 6 );
				}
				break;
			case 0x80 : // 抜き
				if( IsTaxDivide( szrec.SZgrp1, szrec.SZsjsg ) ) {
					// 抜き
					for( i = 0; i != 13; i++ ) {
						memset( Mony, '\0', sizeof( Mony ) );
						l_sub( Mony, szrec.SZdtbl[i].SDval[3], szrec.SZdtbl[i].SDval[4] );
						l_add( szrec.SZdtbl[i].SDval[5], szrec.SZdtbl[i].SDval[5], Mony );
						l_add( szrec.SZdtbl[i].SDval[6], szrec.SZdtbl[i].SDval[6], szrec.SZdtbl[i].SDval[4] );
						memset( szrec.SZdtbl[i].SDval[3], '\0', 6 );
						memset( szrec.SZdtbl[i].SDval[4], '\0', 6 );
					}
				}
				else	{
					// 込み
					for( i = 0; i != 13; i++ ) {
						memset( Mony, '\0', sizeof( Mony ) );
						l_add( Mony, szrec.SZdtbl[i].SDval[5], szrec.SZdtbl[i].SDval[6] );
						l_add( szrec.SZdtbl[i].SDval[3], szrec.SZdtbl[i].SDval[3], Mony );
						l_add( szrec.SZdtbl[i].SDval[4], szrec.SZdtbl[i].SDval[4], szrec.SZdtbl[i].SDval[6] );
						memset( szrec.SZdtbl[i].SDval[5], '\0', 6 );
						memset( szrec.SZdtbl[i].SDval[6], '\0', 6 );
					}
				}
				break;
			case 0xc0 :	// 一括税抜
				if( (K221sw && UriShiireGet( &szrec )) || (H301sw && !UriShiireGet( &szrec )) )	{
				// 積上げ方式				
					// 抜き
					for( i = 0; i != 13; i++ ) {
						memset( Mony, '\0', sizeof( Mony ) );
						l_sub( Mony, szrec.SZdtbl[i].SDval[3], szrec.SZdtbl[i].SDval[4] );
						l_add( szrec.SZdtbl[i].SDval[5], szrec.SZdtbl[i].SDval[5], Mony );
						l_add( szrec.SZdtbl[i].SDval[6], szrec.SZdtbl[i].SDval[6], szrec.SZdtbl[i].SDval[4] );
						memset( szrec.SZdtbl[i].SDval[3], '\0', 6 );
						memset( szrec.SZdtbl[i].SDval[4], '\0', 6 );
					}
				}
				break;
			default   :
				break;
		}
		for( i = 0; i < 13; i++ ) {			
			// 税率集計判定
			if( szrec.SZzkbn&0x0f )		valp = (char *)srecp.h3_mnth[i].tgai3_val;	// 3%
			else						valp = (char *)srecp.h_mnth[i].tgai_val;	// 5%
			for( j = 0; j < 8; j++, valp += 6 )
				l_add( valp, valp, szrec.SZdtbl[i].SDval[j] );
			// 別記の金額は税抜きに加算される
			if( szrec.SZzkbn&0x0f )		valp = (char *)srecp.h3_mnth[i].nuki3_val;	// 3%
			else						valp = (char *)srecp.h_mnth[i].nuki_val;	// 5%
			l_add( valp, valp, szrec.SZdtbl[i].SDval[7] );		// 別記 (SEQ=7)
			// 別記の税額は計算し、抜き税額に加算される
			memset( Mony, '\0', sizeof( Mony ) );
			if( szrec.SZzkbn&0x0f )	{
				percent( Mony, (char *)&szrec.SZdtbl[i].SDval[7], 1, 1, 0 );	// 別記３％
				valp = (char *)srecp.h3_mnth[i].nukz3_val;
			}
			else	{
				percent( Mony, (char *)&szrec.SZdtbl[i].SDval[7], 1, 0, 0 );	// 別記５％
				valp = (char *)srecp.h_mnth[i].nukz_val;
			}
			l_add( valp, valp, Mony );		// 別記 (SEQ=7)	
		}

		if( pTblHdl->th_acs( thWritex, &srecp, foret ) )	return( ERR );
		// 売上区分／仕入区分集計
		Kubun_Stock( &szrec, suktb );
		// 仮受仮払抜き税額集計 [06'11.08]
		if( !UriShiireGet( &szrec ) )	{
			if( szrec.SZzkbn & 0x0f )	pszrec = &karibri_3;	// ３％
			else						pszrec = &karibri_5;	// ５％
			for( j = 0; j != 13; j++ ) {
				l_add( pszrec->SZdtbl[j].SDval[0], pszrec->SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[6] );
			}
		}
		else	{
			if( szrec.SZzkbn & 0x0f )	pszrec = &kariuke_3;	// ３％
			else						pszrec = &kariuke_5;	// ５％
			for( j = 0; j != 13; j++ ) {
				l_add( pszrec->SZdtbl[j].SDval[0], pszrec->SZdtbl[j].SDval[0], szrec.SZdtbl[j].SDval[6] );
			}
		}
	}
	while( pZaimuSub->szsyk->MoveNext() != -1 );

	return( FALSE );
}

// 科目属性より仮受消費税及び、仮払消費税の取得
// 0 : 仮払消費税
// 1 : 仮受消費税
int	CShinCalculateW::UriShiireGet( struct _SZREC	*szrec )
{
	int	status = 0;

	switch( szrec->SZgrp1 ) {
		case 0x01 :	// 流動資産
			if( szrec->SZgrp2 == 0x00 )	status = 0;	// 取得
			else						status = 1;	// 譲渡
			break;
		case 0x11 :	// 固定資産
			if( szrec->SZsjsg == 0x00 )	status = 0;	// 取得
			else						status = 1;	// 譲渡
			break;
		case 0x02 :	// 売上
			status = 1;								// 売上
			break;
		case 0x08 :	// 経費
			if( szrec->SZgrp2 == 0x03 )	{
				status = 1;	// 貸倒損失
				break;
			}
		default   :	// 仕入
			status = 0;								// 仕入
			break;
	}
	return( status );
}

// 課税仕入区分・売上区分集計エリアのリード
int CShinCalculateW::suk_read( struct _H9SUM *SVsum, struct TB_PAR *suktb )
{
	struct	SUK_R	srecp;
	int	i, j, k;
	int		loop, kikan, kikan_tbl;
	char	*par;

	loop = 16;
	// 消費税課税期間の判定
	kikan = is5perm();
	kikan_tbl = is5pertbl();
	for( i = 0; i < loop; ++i ) {
		memset( &srecp, '\0', sizeof( struct SUK_R ) );
		
		srecp.su_kbn1 = SVsum->SSstbl[i].SUskb;		// 消費税処理区分
		srecp.su_kbn2 =	SVsum->SSstbl[i].SUdkb;		// 仕訳区分
		srecp.su_ofs  =	SVsum->SSstbl[i].SUcof;		// 車税率変更ofset
		if( i < 3 )		// 個別対応4%集計済みｻｲﾝ
			memmove( srecp.su_per, SVsum->SSstbl[i].SUtax, 13 );
		k = (int) (srecp.su_ofs & 0x0f);	k--;
		for( j = 0; j < 13; ++j ) {
			par = (char *)srecp.su_val[j];
			// 旧税率集計判定
			if( ( kikan < 0 ) ) par = (char *)srecp.su3_val[j];
			else if( (kikan > 0) && (i >= 8) ) 	{	// 経過措置税率区分
					par = (char *)srecp.su3_val[j];
			}
			else if( ! kikan ) {
				if( (kikan_tbl > j) ) par = (char *)srecp.su3_val[j];
				else if( i >= 8 ) par = (char *)srecp.su3_val[j];
			}
			memmove( par, SVsum->SSstbl[i].SUval[j], 6 );
			if( (srecp.su_ofs & 0x10) && j == k )
				memmove( srecp.su_val2, SVsum->SSstbl[i].SUval2, 6 );
			if( i < 8 ) {
				memmove( srecp.su_kvl[j], SVsum->SSstbl[i].SUkvl[j], 6 );
				memmove( srecp.su_nvl[j], SVsum->SSstbl[i].SUnvl[j], 6 );
				memmove( srecp.su_kbk[j], SVsum->SSstbl[i].SUkbk[j], 6 );
				memmove( srecp.su_zei[j], SVsum->SSstbl[i].SUZi5[j], 6 );
			}
			else	{
				memmove( srecp.su_kvl3[j], SVsum->SSstbl[i].SUkvl[j], 6 );
				memmove( srecp.su_nvl3[j], SVsum->SSstbl[i].SUnvl[j], 6 );
				memmove( srecp.su_kbk3[j], SVsum->SSstbl[i].SUkbk[j], 6 );
				memmove( srecp.su_zei3[j], SVsum->SSstbl[i].SUZi3[j], 6 );
			}
		}
		
		if( pTblHdl->th_rput( &srecp, suktb, i ) )	return( -1 );
	}
	
	return( FALSE );
}

// 棚卸調整データの取得エリアのクリア
// 0:仮払5％＋３％
// 1:課税／課税に係る5％ 　2:課税／課税に係る 3％ 
// 3:　　／非課税に係る5％ 4:　　／非課税に係る3％ 
// 5:　　／共通に係る3％   6:　　／共通に係る3％ 
void CShinCalculateW::Stack_Clear( struct _SZREC *stack )
{
	memset( stack, '\0', sizeof( struct _SZREC ) );		// 0:仮払5％
	memset( (stack+1), '\0', sizeof( struct _SZREC ) );	// 1:課税／課税に係る5％
	memset( (stack+2), '\0', sizeof( struct _SZREC ) );	// 2:課税／課税に係る 3％ 
	memset( (stack+3), '\0', sizeof( struct _SZREC ) );	// 3:　　／非課税に係る5％ 
	memset( (stack+4), '\0', sizeof( struct _SZREC ) );	// 4:　　／非課税に係る3％
	memset( (stack+5), '\0', sizeof( struct _SZREC ) );	// 5:　　／共通に係る3％
	memset( (stack+6), '\0', sizeof( struct _SZREC ) );	// 6:　　／共通に係る3％
}

// 棚卸調整データ集計
int CShinCalculateW::Stack_Stock( struct _SZREC *szrcp, struct _SZREC *stack )
{
	struct	_SZREC	*SDrcp;
	int		i, j;

	if( szrcp->SZgrp1 == 0x04 &&
		( (szrcp->SZgrp2&0x0f) == 0x05 || (szrcp->SZgrp2&0x0f) == 0x06 ) ) {  
		if( !strncmp( (char *)&TBhead.SVkcod[0], (char *)szrcp->SZkcod, 8-2 ) ) {
		// 仮払消費税データの取得
			SDrcp = stack;
			for( i = 0; i != 13; i++ ) {
				// 仮払消費税データは SDval[0] に集計
				l_add( SDrcp->SZdtbl[i].SDval[0], SDrcp->SZdtbl[i].SDval[0], 
													szrcp->SZdtbl[i].SDval[0] );
			}
		}
		else	{
		// 棚卸調整科目データの取得
			if( iskobetm() ) {
				// 個別対応
				switch( szrcp->SZitem&0xf0 ) {
					case 0x10 :
						if( !szrcp->SZzkbn )	SDrcp = (stack + 1);
						else					SDrcp = (stack + 2);
						break;
					case 0x20 :
						if( !szrcp->SZzkbn )	SDrcp = (stack + 3);
						else					SDrcp = (stack + 4);
						break;
					case 0x30 :
						if( !szrcp->SZzkbn )	SDrcp = (stack + 5);
						else					SDrcp = (stack + 6);
						break;
					default   :					SDrcp = NULL;
						break;
				}
				if( SDrcp != NULL ) {
				// 棚卸調整科目データ
				// 3:税込み 3:税込み（税額） 5:税抜き 6:税抜き（税額） 7:別記
					for( i = 0; i != 13; i++ ) {
						for( j = 3; j < 8; j++ ) {
							l_add( SDrcp->SZdtbl[i].SDval[j], SDrcp->SZdtbl[i].SDval[j],
															szrcp->SZdtbl[i].SDval[j] );
						}
					}
				}
			}
			else	{
				// 比例配分
				if( !szrcp->SZzkbn )	SDrcp = (stack + 1);
				else					SDrcp = (stack + 2);
				// 棚卸調整科目データ
				// 3:税込み 3:税込み（税額） 5:税抜き 6:税抜き（税額） 7:別記
				for( i = 0; i != 13; i++ ) {
					for( j = 3; j < 8; j++ ) {
						l_add( SDrcp->SZdtbl[i].SDval[j], SDrcp->SZdtbl[i].SDval[j],
														szrcp->SZdtbl[i].SDval[j] );
					}
				}
			}
		}

		return( TRUE );
	}

	return( FALSE );
}

// 輸入仕入データ集計
int CShinCalculateW::Import_Stock( struct _H9SUM *h9sum, struct _SZREC *szrec )
{
	int		mct;
	char	val_nuk[6], val_nkz[6], val_bek[6];
	if( szrec->SZgrp1 == 0x04 && szrec->SZgrp2 == 0x04 ) {
		//	輸入仕入
		memset( val_nkz, '\0', sizeof( val_nkz ) );
		memset( val_nuk, '\0', sizeof( val_nuk ) );
		memset( val_bek, '\0', sizeof( val_bek ) );
		for( mct = 0; mct <= 13; mct++ ) {
			if( !strncmp( (char *)&TBhead.SVkcod[0], (char *)szrec->SZkcod, 8-2 ) ) {	// 仮払消費税
				memmove( val_nkz, szrec->SZdtbl[mct].SDval[0], 6 );	// 対象外 (SEQ=0)
			}
			else	{
				memmove( val_nuk, szrec->SZdtbl[mct].SDval[5], 6 );	// 抜き金額 (SEQ=5)
				memmove( val_nkz, szrec->SZdtbl[mct].SDval[6], 6 );	// 抜き税額 (SEQ=6)
				memmove( val_bek, szrec->SZdtbl[mct].SDval[7], 6 );	// 別記金額 (SEQ=7)
				l_add( val_nuk, val_nuk, val_bek );
			}

			if( szrec->SZzkbn&0x01 ) {	// ３％	
				l_add( h9sum->SSustb[mct].SSval3, h9sum->SSustb[mct].SSval3, val_nuk ); 
				l_add( h9sum->SSustb[mct].SSzei3, h9sum->SSustb[mct].SSzei3, val_nkz ); 
			}
			else {		// ５％
				l_add( h9sum->SSustb[mct].SSval5, h9sum->SSustb[mct].SSval5, val_nuk ); 
				l_add( h9sum->SSustb[mct].SSzei5, h9sum->SSustb[mct].SSzei5, val_nkz ); 
				// ４％輸入仕入
				l_add( h9sum->SSustb[mct].SSzei4,  h9sum->SSustb[mct].SSzei4, szrec->SZdtbl[mct].SDval[2] );
			}
		}
	}
	return( FALSE );
}

// 売上区分／仕入区分データ集計
int CShinCalculateW::Kubun_Stock( struct _SZREC *szrec, struct TB_PAR *suktb )
{
	int				mct, cnt;
	char			*valp, mony[6], kkom[6], komi[6], nuki[6], beki[6], work[6];
	// 売上／仕入区分集計
	if( szrec->SZitem ) {	// 売上／仕入区分サイン
		switch( szrec->SZitem&0xff ) {
		case  0x10	:	cnt = 0;	break;	// 課税売上に係る
		case  0x20	:	cnt = 1;	break;	// 非課税売上に係る
		case  0x30	:	cnt = 2;	break;	// 共通に係る
		case  0x01	:	cnt = 3;	break;	// 卸売業
		case  0x02	:	cnt = 4;	break;	// 小売業
		case  0x03	:	cnt = 5;	break;	// 製造業
		case  0x04	:	cnt = 6;	break;	// その他
		case  0x05	:	cnt = 7;	break;	// サービス
		default     :	cnt = -1;	break;	//★[13'08.26]///
		}
		if( cnt == -1 )	return( FALSE );	//★[13'08.26]///

		if( szrec->SZzkbn&0x01 )	cnt += 8;	// 3%

		for( mct = 0; mct < 13; mct++ ) {
			valp = (char *)SVsum.SSstbl[cnt].SUval[mct];
			memset( mony, '\0', sizeof(mony) );
			memset( komi, '\0', sizeof(komi) );
			memset( kkom, '\0', sizeof(kkom) );
			memset( nuki, '\0', sizeof(nuki) );
			memset( beki, '\0', sizeof(beki) );
			memset( work, '\0', sizeof(work) );
			if( szrec->SZitem&0xf0 && Anbun )	{
				// 仕入区分 
				if( cnt < 8 )	{
					// ４％税額
					if( szrec->SZgrp1 == 0x04 && szrec->SZgrp2 == 0x04 ) {
						// 輸入仕入
						l_add( mony, mony, &szrec->SZdtbl[mct].SDval[2] );					// ４％輸入仕入税額
//★//[13'05.31]///
						switch( cnt ) {
							case 0 :	l_add( KazIp4, KazIp4, &szrec->SZdtbl[mct].SDval[2]);	break;
							case 2 :	l_add( ComIp4, ComIp4, &szrec->SZdtbl[mct].SDval[2]);	break;
							default:	break;
								break;
						}
///////////////////
					}
					else	{
						if( H301sw ) {
							// 法第３０条第１項の適用（積み上げ方式）
							memset( work, '\0', 6 );
							l_input( work, "80" );
							pArithEx->l_mul100( (BYTE *)komi, (BYTE *)&szrec->SZdtbl[mct].SDval[4], (BYTE *)work, 0, 0 );
							pArithEx->l_mul100( (BYTE *)nuki, (BYTE *)&szrec->SZdtbl[mct].SDval[6], (BYTE *)work, 0, 0 );
							percent( beki,(char *)&szrec->SZdtbl[mct].SDval[7], 6, 40, 0 );	// 別記　金額(SEQ=7)
							l_add( mony, komi, nuki );
							l_add( mony, mony, beki );
						}
						else	{
							if( SimpSw ) {
								percent( komi,(char *)&szrec->SZdtbl[mct].SDval[3], 4, 40, 0 );	// 税込み金額(SEQ=3) 
								percent( kkom,(char *)&szrec->SZdtbl[mct].SDval[5], 6, 40, 0 );	// 税抜　金額(SEQ=5)
								percent( beki,(char *)&szrec->SZdtbl[mct].SDval[7], 6, 40, 0 );	// 別記　金額(SEQ=7)
								l_add( mony, komi, kkom );
								l_add( mony, mony, beki );
							}
							else	{
								memset( work, '\0', 6 );
								percent( komi,(char *)&szrec->SZdtbl[mct].SDval[3], 4, 40, 0 );	// 税込み金額(SEQ=3) 
								l_add( work, &szrec->SZdtbl[mct].SDval[5], &szrec->SZdtbl[mct].SDval[6] );
								percent( kkom, work, 4, 40, 0 );								// 税抜を税込み金額に変更 
								percent( beki,(char *)&szrec->SZdtbl[mct].SDval[7], 6, 40, 0 );	// 別記　金額(SEQ=7)
								l_add( mony, komi, kkom );
								l_add( mony, mony, beki );
							}
						}
					}
				}
				else	{
					// ３％税額
//★//[13'05.31]///
					if( szrec->SZgrp1 == 0x04 && szrec->SZgrp2 == 0x04 ) {
						// 輸入仕入
						int	ofs = 0; 
						if( !strncmp( (char *)&TBhead.SVkcod[0], (char *)szrec->SZkcod, 8-2 ) ) ofs = 0;	// 仮払消費税
						else																	ofs = 6;
						l_add( mony, mony, &szrec->SZdtbl[mct].SDval[ofs] );								// ４％輸入仕入税額
						switch( cnt ) {
							case 8 :	l_add( KazIp3, KazIp3, &szrec->SZdtbl[mct].SDval[ofs]);	break;
							case 10:	l_add( ComIp3, ComIp3, &szrec->SZdtbl[mct].SDval[ofs]);	break;
							default:	break;
								break;
						}
					}
///////////////////
					else	{
						if( SimpSw ) {
							percent( komi,(char *)&szrec->SZdtbl[mct].SDval[3], 0, 30, 0 );			// 込み　税額 3%
							percent( kkom,(char *)&szrec->SZdtbl[mct].SDval[5], 1, 30, 0 );			// 抜き　税額 3%
							percent( beki,(char *)&szrec->SZdtbl[mct].SDval[7], 1, 30, 0 );			// 別記　税額 3%
							l_add( mony, komi, kkom );
							l_add( mony, mony, beki );
						}
						else	{
							percent( mony,(char *)&szrec->SZdtbl[mct].SDval[7], 1, 30, 0 );			// 別記　税額 3%
							l_add( mony, mony, &szrec->SZdtbl[mct].SDval[4] );						// 込み税額	(SEQ=4)
							l_add( mony, mony, &szrec->SZdtbl[mct].SDval[6] );						// 抜き税額	(SEQ=6)
						}
					}
				}
			}
			else {						// 売上区分（抜き金額）
				l_add( mony, mony, &szrec->SZdtbl[mct].SDval[3] );	// 込み金額	(SEQ=3)
				l_sub( mony, mony, &szrec->SZdtbl[mct].SDval[4] );	// 込み税額	(SEQ=4)
				l_add( mony, mony, &szrec->SZdtbl[mct].SDval[5] );	// 抜き金額	(SEQ=5)
				l_add( mony, mony, &szrec->SZdtbl[mct].SDval[7] );	// 別記金額	(SEQ=7)
			}

			l_add( valp, valp, mony );

			if( szrec->SZitem&0x0f )	{	// 簡易課税　業種別売上（込み積み上げ）
				memset( komi, '\0', sizeof(komi) );
				memset( beki, '\0', sizeof(beki) );
				l_add( komi, komi, &szrec->SZdtbl[mct].SDval[3] );	// 込み金額
				l_add( komi, komi, &szrec->SZdtbl[mct].SDval[5] );	// 抜き金額
				l_add( komi, komi, &szrec->SZdtbl[mct].SDval[6] );	// 抜き税額
				l_add( beki, beki, &szrec->SZdtbl[mct].SDval[7] );	// 別記
				l_add( SVsum.SSstbl[cnt].SUkvl[mct], SVsum.SSstbl[cnt].SUkvl[mct], komi );
				l_add( SVsum.SSstbl[cnt].SUkbk[mct], SVsum.SSstbl[cnt].SUkbk[mct], beki );
				// 積上げ方式 [07'04.16]
				if( K221sw ) {
					// 本体抜き積上げ
					memset( nuki, '\0', sizeof(nuki) );
					l_add( nuki, nuki, &szrec->SZdtbl[mct].SDval[3] );	// 込み金額
					l_add( nuki, nuki, &szrec->SZdtbl[mct].SDval[5] );	// 抜き金額
					l_sub( nuki, nuki, &szrec->SZdtbl[mct].SDval[4] );	// 込み税額
					l_add( SVsum.SSstbl[cnt].SUnvl[mct], SVsum.SSstbl[cnt].SUnvl[mct], nuki );
					// 税額積上げ
					memset( beki, '\0', sizeof(beki) );
					if( cnt < 8 )	{
						l_add( SVsum.SSstbl[cnt].SUZi5[mct], SVsum.SSstbl[cnt].SUZi5[mct], &szrec->SZdtbl[mct].SDval[4] );
						l_add( SVsum.SSstbl[cnt].SUZi5[mct], SVsum.SSstbl[cnt].SUZi5[mct], &szrec->SZdtbl[mct].SDval[6] );
						percent( beki,(char *)&szrec->SZdtbl[mct].SDval[7], 2, 0, 0 );			// 別記　税額 ５%
						l_add( SVsum.SSstbl[cnt].SUZi5[mct], SVsum.SSstbl[cnt].SUZi5[mct], beki );
					}
					else	{
						l_add( SVsum.SSstbl[cnt].SUZi3[mct], SVsum.SSstbl[cnt].SUZi3[mct], &szrec->SZdtbl[mct].SDval[4] );
						l_add( SVsum.SSstbl[cnt].SUZi3[mct], SVsum.SSstbl[cnt].SUZi3[mct], &szrec->SZdtbl[mct].SDval[6] );
						percent( beki,(char *)&szrec->SZdtbl[mct].SDval[7], 1, 30, 0 );			// 別記　税額 ３%
						l_add( SVsum.SSstbl[cnt].SUZi3[mct], SVsum.SSstbl[cnt].SUZi3[mct], beki );
					}
				}
			}
		}
	}
	return( FALSE );
}

// ６倍長　切り捨て　モジュ－ル
//	 0x0:千円未満 0x?0:切り捨て
//	 0x1:百円未満 0x?1:切り上げ
//	 0x2:十円未満 0x?2:四捨五入
void CShinCalculateW::l_calq( char *des, char *sou, char sgn )
{
	char 	buf1[10], buf2[10], val100[10], val10[10], val4[10];
	int		msign;
	
	l_defn( 0x16 );
	memset( buf1,   '\0', sizeof( buf1 ) );
	memset( val100, '\0', sizeof( val100 ) );
	memset( val10,  '\0', sizeof( val10 ) );
	memmove( buf1, sou, 6 );
	// 計算
	switch( sgn&0xf0 ) {
		case 0x00 :
			l_input( val100, "1000" );	l_input( val10, "100" );	break;
		case 0x10 :
			l_input( val100, "100" );	l_input( val10, "10" );		break;
		default   :
			l_input( val100, "10" );	l_input( val10, "0" );		break;
	}	
	// 計算
	if( (sgn &= 0x03) ) {
		l_input( val4,  "4" );
		
		if( (msign = l_test( buf1 )) < 0 )	l_neg( buf1 );
		l_mod( buf2, buf1, val100 );
		l_div( buf2, buf2, val10 );
		
		if( sgn == 1 ) {		// 切り上げ
			if( l_test( buf2 ) )	l_input( buf2, "1" );
		}
		else if( sgn == 2 ) {	// 四捨五入
			l_sub( buf2, buf2, val4 );
			if( l_test( buf2 ) > 0 )	l_input( buf2, "1" );
			else						l_clear( buf2 );
		}
		l_div( buf1, buf1, val100 );
		l_add( buf1, buf1, buf2 );
		l_mul( buf1, buf1, val100 );
		if( msign < 0 )	l_neg( buf1 );
	}
	else {						// 切り捨て
		l_div( buf1, buf1, val100 );
		l_mul( buf1, buf1, val100 );
	}
	
	memmove( des, buf1, 6 );
}

// ６倍長　集計　モジュ－ル
void CShinCalculateW::l_6calq( char *dis, char *sou, char *buns, char *bunb )
{
	char 	t_l, t_l2, WK[6], WS[6], WB[6];
	
	t_l = l_retn();
	if( (t_l & 0x0f) < 6 ) {
		t_l2 = (t_l & 0xf0) | 6;
		l_defn( t_l2 );
		l_let( WS, buns, t_l );
		l_let( WB, bunb, t_l  );
		l_let( WK, sou,  t_l  );
	}
	else {
		memmove( WS, buns, 0x0f & t_l );
		memmove( WB, bunb, 0x0f & t_l  );
		memmove( WK, sou,  0x0f & t_l  );
	}
	
	l_mul( WK, WK, WS );		// sou = sou * 分子
	l_div( WK, WK, WB );		// sou = sou / 分母
	if( (t_l & 0x0f) < 6 ) {	// dis = sou
		l_defn( t_l );
		l_let( dis, WK, t_l2 );
	}
	else memmove( dis, WK, 0x0f & t_l );
}

// 税率計算
//char 	*des;			/* 結果 */
//char	*sou;			/* 数値 */
//int		sgn;			/*	0 = 税込み    
//							1 = 税抜き
//						  	2 = 5 / 100   
//						  	3 = 3 / 106 （特別消費税）
//							4 = 4 / 100 + 5
//							5 = (税率/2) / 100 */
//							6 = 4 / 100;
//char	ztno,			/* 税率テ－ブル　＃ */
//		hagaku;			/* 0 = 切り捨て　1 = 切り上げ　2 = 四捨五入 */
void CShinCalculateW::percent( char *des, char *sou, int sgn, char ztno, char hagaku )
{
	char t_l, t_l2, xbuf[10];
	char buf1[10], buf2[10], buf3[10], buf4[10], val10[10], val4[10];
	int		msign;
	short	zeirit;
	
	t_l = l_retn();
	if( (t_l & 0x0f) < 6 ) {
		t_l2 = (t_l & 0xf0) | 6;
		l_defn( t_l2 );
		l_let( buf1, sou, t_l );
	}
	else memmove( buf1, sou, 0x0f & t_l );	
	if( ztno < 0 )			zeirit = 60;
	else if( ztno == 30 ) {
		if( TBhead.SVvers ) zeirit = TBhead.SVzper[1];
		else 				zeirit = 30;
	}
	else if( ztno == 40 ) {
		if( TBhead.SVvers ) zeirit = TBhead.SVzrto;
		else 				zeirit = 40;
	}
	else					zeirit = TBhead.SVzper[ztno];	
	if( sgn == 0 || sgn == 1 || sgn == 3 || sgn == 4 )
		sprintf_s( xbuf, sizeof( xbuf ), "%d", zeirit );
	else if( sgn == 5 )	sprintf_s( xbuf, sizeof( xbuf ), "%d", zeirit/2 );
	else if( sgn == 2 ) strcpy_s(  xbuf, "50" );
	else if( sgn == 6 ) strcpy_s(  xbuf, "40" );
	l_input( buf2, xbuf );
	hagaku &= 0x03;
	if( sgn == 0 )
		sprintf_s( xbuf, sizeof( xbuf ), "%d", zeirit + 1000 );
	else if( sgn == 3 )	 
		sprintf_s( xbuf, sizeof( xbuf ), "%d", zeirit * 2 + 1000 );
	else if( sgn == 4 )	 
		sprintf_s( xbuf, sizeof( xbuf ), "%d", 50 + 1000 );
	else if( sgn == 1 || sgn == 2 || sgn == 5 || sgn == 6 )
		strcpy_s( xbuf, "1000" );
	l_input( buf3, xbuf );
	// 計算
	if( hagaku ) {
		l_input( val10, "10" );
		l_input( val4,  "4" );
		
		if( (msign = l_test( buf1 )) < 0 )	l_neg( buf1 );
		l_mul( buf1, buf1, buf2 );
		
		l_mod( buf4, buf1, buf3 );
		l_mul( buf4, buf4, val10 );
		l_div( buf4, buf4, buf3 );
		if( hagaku == 1 ) {			// 切り上げ
			if( l_test( buf4 ) )	l_input( buf4, "1" );
		}
		else if( hagaku == 2 ) {	// 四捨五入
			l_sub( buf4, buf4, val4 );
			if( l_test( buf4 ) > 0 )	l_input( buf4, "1" );
			else	l_clear( buf4 );
		}
		l_div( buf1, buf1, buf3 );
		l_add( buf1, buf1, buf4 );
		if( msign < 0 )	l_neg( buf1 );
	}
	else {
		l_mul( buf1, buf1, buf2 );
		l_div( buf1, buf1, buf3 );
	}	
	if( (t_l & 0x0f) < 6 ) {
		l_defn( t_l );
		l_let( des, buf1, t_l2 );
	}
	else memmove( des, buf1, 0x0f & t_l );
}

///////////////////////////////////////////////////////////////////////////////
// ６倍長　切り捨て　モジュ－ル
//void p_calq( char *des, char* sou, char sgn )
//	char 	*des;			 結果
//	char	*sou;			 数値
//	char	sgn;			 | | |･|･| | |･|･|
//							 0x0:千円未満 0x?0:切り捨て
//							 0x1:百円未満 0x?1:切り上げ
//							 0x2:十円未満 0x?2:四捨五入
void CShinCalculateW::p_calq( char *des, char* sou, char sgn )
{
	char 	buf1[10], buf2[10], val100[10], val10[10], val4[10];
	int		msign;
	
	l_defn( 0x16 );
	
	memset( buf1,  '\0', sizeof( buf1 ) );
	memset( val10, '\0', sizeof( val10 ) );
	memmove( buf1, sou, 6 );
	
	// 計算
	switch( sgn&0xf0 ) {
		case 0x00 :
			l_input( val100, "1000" );	l_input( val10, "100" );	break;
		case 0x10 :
			l_input( val100, "100" );	l_input( val10, "10" );		break;
		default   :
			l_input( val100, "10" );	l_input( val10, "0" );		break;
	}
	
	// 計算
	if( (sgn &= 0x03) ) {
		l_input( val4,  "4" );
		
		if( (msign = l_test( buf1 )) < 0 )	l_neg( buf1 );
		l_mod( buf2, buf1, val100 );
		l_div( buf2, buf2, val10 );
		
		if( sgn == 1 ) {		// 切り上げ
			if( l_test( buf2 ) )	l_input( buf2, "1" );
		}
		else if( sgn == 2 ) {	// 四捨五入
			l_sub( buf2, buf2, val4 );
			if( l_test( buf2 ) > 0 )	l_input( buf2, "1" );
			else						l_clear( buf2 );
		}
		l_div( buf1, buf1, val100 );
		l_add( buf1, buf1, buf2 );
		l_mul( buf1, buf1, val100 );
		if( msign < 0 )	l_neg( buf1 );
	}
	else {						// 切り捨て
		l_div( buf1, buf1, val100 );
		l_mul( buf1, buf1, val100 );
	}
	memmove( des, buf1, 6 );
}

///////////////////////////////////////////////////////////////////////////////
//
// 消費税マスターバージョンチェック
//
//		返送値	TRUE	処理不可
//				FALSE	処理　可
//
int CShinCalculateW::isVerchk( char *msg )
{
	char	Cymd[4];

	memset( Cymd, '\0', sizeof( Cymd ) );

	Cymd[0] = (char )0x94;
	Cymd[1] = (char )0x04;

	switch( TBhead.SVvers & 0xff ) {
		case 0x10 :	return( is5perm() + 1 );					// 新バージョン
		case 0x00 :	if( l_jsgn( Vol1.EE_YMD, Cymd, 2 ) < 0 ) {	// 旧バージョン
						strcpy_s( msg, sizeof( msg ), "平成６年３月３１日以前の期間の" );
						strcat_s( msg, sizeof( msg ), "マスターは扱えません。" );
						return( TRUE );
					}
					return( FALSE );
		default	:	sprintf_s( msg, sizeof( msg ), "この消費税マスター(V%02x)は扱えません。", 
															TBhead.SVvers );
					return( TRUE );
	}
}

// 消費税マスターチェック
//
//		返送値	TRUE	消費税マスター
//				FALSE	未消費税マスター
int CShinCalculateW::issymst( void )
{
int retn;

	if( (Vol1.S_MST & 0x01) )	{
		retn = TRUE;
	}
	else	{
		retn = FALSE;
	}

	return( retn );
}

// 簡易課税マスターチェック
//
//		返送値	TRUE	簡易課税マスター
//				FALSE	未簡易課税マスター
//
int CShinCalculateW::iskanim( void )
{
int		retn;

	if( (Vol1.S_SGN2 & 0x0f) > 1 )	retn = TRUE;
	else							retn = FALSE;

	return( retn );
}

// 個別対応マスタ－チェック
//
//		返送値	TRUE	個別対応マスタ－
//				FALSE	未個別対応マスタ－
//
int CShinCalculateW::iskobetm( void )
{
int retn;
//★//[12'01.13]///
//	if( Vol1.S_SGN4 & 0x80 )
//	{
//		if( Anbun )	retn = TRUE;
//		else		retn = FALSE;
//	}
//	else
//	{
//		retn = FALSE;
//	}
///////////////////
	if( Anbun )	retn = TRUE;
	else		retn = FALSE;
///////////////////
	return( retn );
}

// 一括税抜きマスタ－チェック
//
//		返送値	TRUE	一括税抜きマスタ－
//				FALSE	未一括税抜きマスタ－
int CShinCalculateW::isikkatm( void )
{
int	retn;

	if( (Vol1.S_SGN1 & 0xc0) == 0xc0)
	{
		retn = TRUE;
	}
 	else
	{
		retn = FALSE;
	}

	return( retn );
}

// 売上業種区分取得
int CShinCalculateW::ukbngt( char selsgn[] )
{
int		selcnt;
char	deflt_sgn;

	deflt_sgn = ( (Vol1.S_SGN2 & 0x0f) -1 ) & 0x0f;

	selcnt = 0;
	if( Vol1.S_SGN3 & 0x10 || deflt_sgn == 0x01 ) {
		selsgn[ selcnt ] = 0x01;
		selcnt++;
	}
	if( Vol1.S_SGN3 & 0x20 || deflt_sgn == 0x02 )	{
		selsgn[ selcnt] = 0x02;
		selcnt++;
	}
	if( Vol1.S_SGN3 & 0x40 || deflt_sgn == 0x03 )	{
		selsgn[ selcnt ] = 0x03;
		selcnt++;
	}
	if( Vol1.S_SGN3 & 0x80 || deflt_sgn == 0x04 ) {
		selsgn[ selcnt ] = 0x04;
		selcnt++;
	}
	
	return( selcnt );
}

// 決算期間内 消費税５％改正 チェック
int CShinCalculateW::is5perm( void )
{

	unsigned char	enday[3];

	// 消費税 5% 改正年月日の１年前
	enday[0] = 0x96;
	enday[1] = 0x04;
	enday[2] = 0x01;
	// 西暦２０００年以降の判定
	if( (Vol1.SS_YMD[0] < 0x80) || (Vol1.EE_YMD[0] < 0x80) ) {
		return( 1 );
	}
	// ３％消費税期間
	if( l_jsgn( Vol1.SS_YMD, enday, 3 ) <= 0 )	return( -1 );

	// ３％消費税期間
	enday[0] = 0x97;
	if( l_jsgn( Vol1.EE_YMD, enday, 3 ) < 0 )	return( -1 );

	// ５％消費税期間
	if( l_jsgn( Vol1.SS_YMD, enday, 3 ) >= 0 )	return( 1 );

	// 決算期間中に３％から５％への改正あり
	return( FALSE );
}

// 決算期間内 消費税５％改正オフセット
int CShinCalculateW::is5pertbl( void )
{
	int		i;

	if( ! is5perm() ) {
		for( i = 0; i < 13; i++ ) {
			if( Vol2.D_TBL[i]._EMD[0] == 0x04 ) return( i );
		}
	}
	else if( is5perm() > 0 )	return( 13 );
	return( -1 );
}

// 特定収入集計テ－ブルの作成＆読み込み
int CShinCalculateW::SpRead( void )
{
		 int	thd, Cnt, Mct, Tct;
unsigned int	rsiz, rcnt;
unsigned long	tsiz;
char			Ascbf[20];
char			Mony1[6], Mony2[6], Mony3[6], Mony4[6], Mony5[6], Mony6[6];

SPCLQ			Sprec;
	
	// 集計テーブルの作成
	if( SpTbl.tp_tno != ERR )	pTblHdl->th_close( SpTbl.tp_tno );
	rsiz = (unsigned int)sizeof( SPCLQ );					// レコード長
	rcnt = (unsigned int)(m_Specnt + 10 );					// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	// テーブル イニシャライズ
	if( SpTbl.tp_tno == -1 ) {
		// テーブル イニシャライズ
		if ((thd = pTblHdl->th_open( tsiz, rsiz )) < 0)  return( 1 );
	}
	memset( &SpTbl, '\0', sizeof(struct TB_PAR) );	
	SpTbl.tp_tno  = thd;
	SpTbl.tp_rmax = rcnt;
	SpTbl.tp_rnum = 0;
	SpTbl.tp_rlen = rsiz;
	SpTbl.tp_koff = 0;
	SpTbl.tp_klen = 8;		/* 科目（内部）コード (８桁) */
	SpTbl.tp_dupm = 0;
	SpTbl.tp_sufm = 0;
	SpTbl.tp_cp   = 0;
	SpTbl.tp_err  = 0;

	// 消費税集計マスターの読み込み
	Cnt = 0;
	while( 1 ) {
		// 最終レコードチェック
		if( m_Specnt == Cnt )	break;		// 最終チェック
		// レコードの読み込み
		if( Cnt == 0 ){
			if( pZaimuSub->m_pSpsyk->MoveFirstEx() ){
				break;
			}
		}
		else{
			if(  pZaimuSub->m_pSpsyk->MoveNextEx() ){
				break;
			}
		}
		Cnt++;
		
		memset( &Sprec, '\0', sizeof( SPCLQ ) );
		// 内部コード
		memset( Sprec.Sp_kcod, '0', 8 );
		memmove( Sprec.Sp_kcod, pZaimuSub->m_pSpsyk->m_SPkcod, 8 );
		// グループサイン
		Sprec.Sp_grp1 = pZaimuSub->m_pSpsyk->m_SPgrp[0];
		// 貸借サイン
		Sprec.Sp_katt = pZaimuSub->m_pSpsyk->m_SPkatt;

		for( Mct = 0, Tct = 0; Mct != MONTH_MAX; Mct++ ) {
		// 除外チェック
			switch( Mct ) {
				case 0 : Tct = 0;	break;
				case 1 : Tct = 1;	break;
				case 2 : Tct = 2;	break;
				case 3 : Tct = 2;	break;
				case 4 : Tct = 3;	break;
				case 5 : Tct = 4;	break;
				case 6 : Tct = 5;	break;
				case 7 : Tct = 5;	break;
				case 8 : Tct = 6;	break;
				case 9 : Tct = 7;	break;
				case 10: Tct = 8;	break;
				case 11: Tct = 8;	break;
				case 12: Tct = 9;	break;
				case 13: Tct =10;	break;
				case 14: Tct =11;	break;
				case 15: Tct =11;	break;
			}
			// 設立時のチェック
			int Last_mon = Establish();
			if( Last_mon != -1 ) {
				if( Last_mon == Mct ) {
					Tct = Last_mon - 1;
					if( shin_ofst( &Vol1.EE_YMD[1], &Tct, &Vol1, &Vol2 ) )	Tct = 0;
				}
			}
			strcpy_s( Ascbf, pZaimuSub->m_pSpsyk->m_SPtkzn[ Mct ] );		// 特定収入	個別対応時　課税売上に要する
			l_input( Mony1, Ascbf );	
			strcpy_s( Ascbf, pZaimuSub->m_pSpsyk->m_SPthzn[ Mct ] );		// 〃		〃　　　　　非課税売上に要する
			l_input( Mony2, Ascbf );
			strcpy_s( Ascbf, pZaimuSub->m_pSpsyk->m_SPtczn[ Mct ] );		// 〃		〃　　　　　共通に要する
			l_input( Mony3, Ascbf );
			strcpy_s( Ascbf, pZaimuSub->m_pSpsyk->m_SPtszn[ Mct ] );		// 使途が特定されていない特定収入
			l_input( Mony4, Ascbf );
			strcpy_s( Ascbf, pZaimuSub->m_pSpsyk->m_SPtfzn[ Mct ] );		// 特定とならない課税対象外収入
			l_input( Mony5, Ascbf );
			strcpy_s( Ascbf, pZaimuSub->m_pSpsyk->m_SPtzan[ Mct ] );		// リザーブ
			l_input( Mony6, Ascbf );
			// 貸借逆転
			if( Sprec.Sp_katt & 0x01 )	{
				l_neg( Mony1 );
				l_neg( Mony2 );
				l_neg( Mony3 );
				l_neg( Mony4 );
				l_neg( Mony5 );
				l_neg( Mony6 );

			}
			l_add( &Sprec.Sp_tkzn[ Tct ], &Sprec.Sp_tkzn[ Tct ], Mony1 );	
			l_add( &Sprec.Sp_thzn[ Tct ], &Sprec.Sp_thzn[ Tct ], Mony2 );
			l_add( &Sprec.Sp_tczn[ Tct ], &Sprec.Sp_tczn[ Tct ], Mony3 );
			l_add( &Sprec.Sp_tszn[ Tct ], &Sprec.Sp_tszn[ Tct ], Mony4 );
			l_add( &Sprec.Sp_tfzn[ Tct ], &Sprec.Sp_tfzn[ Tct ], Mony5 );
			l_add( &Sprec.Sp_tfzn[ Tct ], &Sprec.Sp_tfzn[ Tct ], Mony6 );
		}

		if( pTblHdl->th_add( &Sprec, &SpTbl ) )	return( ERR );
	}
	return( FALSE );

}

// 一括税抜時の仮受・仮払消費税額の取得
int CShinCalculateW::IkTaxZanRead( void )
{
	int				i, j, mct, sgn;
	char			Ascbf[64], Mony[6], Kasi[6], Kari[6];
	struct	_SZREC	*KrSzrec, *KrSzrecEx;

	//////////////
	// 初期設定 //
	//////////////
	for( mct = 0; mct < 19; mct++ )	{
		m_Ikmon[mct] = 0;
		if( mct < 13 )	TBhead.SVmctl[mct] = 0x00;
	}
	// 仮払消費税の科目残高の一括税抜月の読み込み
	memset( &ikkrbri, '\0', sizeof( struct _SZREC ) );		// ５％＋３％
	memmove( ikkrbri.SZkcod, pZaimuSub->szvol->SVkcod1, 8 );
	// 仮受消費税の科目残高の一括税抜月の読み込み
	memset( &ikkruke, '\0', sizeof( struct _SZREC ) );		// ５％＋３％
	memmove( ikkruke.SZkcod, pZaimuSub->szvol->SVkcod2, 8 );
	ikkruke.SZkatt = 0x80;
	// 精算仮払消費税の科目残高の一括税抜月の読み込み
	memset( &sikrbri, '\0', sizeof( struct _SZREC ) );		// ３％＋３％
	memmove( sikrbri.SZkcod, pZaimuSub->szvol->SVkcod1, 8 );
	// 精算仮受消費税の科目残高の一括税抜月の読み込み
	memset( &sikruke, '\0', sizeof( struct _SZREC ) );		// ５％＋３％
	memmove( sikruke.SZkcod, pZaimuSub->szvol->SVkcod2, 8 );
	sikruke.SZkatt = 0x80;

	if( !(m_Ikat&0xC0) )	return( 0 );					// 一括税抜以外は初期化のみ行なう

	//////////////////////
	// 一括税抜月の取得 //
	//////////////////////
	pZaimuSub->minfo->MoveFirst();
	mct = 0;
	do {
		if( m_Ikmon[ mct ] == mct )	m_Ikmon[ mct ] = 1;
		mct++;
		if( mct == 16 )	break;

	} while( pZaimuSub->minfo->MoveNext() != -1 );
	//////////////
	// 科目残高 //
	//////////////
	pZaimuSub->kzrec->MoveFirst();
	do {
		sgn = 0;
		// 仮払消費税
		if( !strncmp( pZaimuSub->szvol->SVkcod1, pZaimuSub->kzrec->kzkcd, 8-2 ) )	sgn = 1;
		// 仮受消費税（対象外に格納）
		if( !strncmp( pZaimuSub->szvol->SVkcod2, pZaimuSub->kzrec->kzkcd, 8-2 ) )	sgn = 2;
		if( !sgn )	continue;
		// 仮受・仮払消費税チェック
		if( sgn == 1 )	KrSzrec = (struct _SZREC *)&ikkrbri;
		else			KrSzrec = (struct _SZREC *)&ikkruke;
		// 金額を文字列からバイナリーへ変換
		for( i = 0, j = 0; i < 16; i++ ) {
			// 除外チェック
			switch( i ) {
				case 0 : j = 0;	break;
				case 1 : j = 1;	break;
				case 2 : j = 2;	break;
				case 3 : if( d_tno != 2 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 2;	
						 break;
				case 4 : j = 3;	break;
				case 5 : j = 4;	break;
				case 6 : j = 5;	break;
				case 7 : if( d_tno != 5 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 5;
						 break;
				case 8 : j = 6;	break;
				case 9 : j = 7;	break;
				case 10: j = 8;	break;
				case 11: if( d_tno != 8 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 8;
						 break;
				case 12: j = 9;	break;
				case 13: j =10;	break;
				case 14: j =11;	break;
				case 15: j =12;	break;
			}
			// 設立時のチェック
			int Last_mon = Establish();
			if( Last_mon != -1 ) {
				if( Last_mon == i ) {
					j = Last_mon - 1;
					if( shin_ofst( &Vol1.EE_YMD[1], &j, &Vol1, &Vol2 ) )	j = 0;
				}
			}
			// 各月発生残高
			memset( Kari, '\0', 6 );
			strcpy_s( Ascbf, pZaimuSub->kzrec->lhs[ i ] );
			l_input( Kari, Ascbf );
			memset( Kasi, '\0', 6 );
			strcpy_s( Ascbf, pZaimuSub->kzrec->rhs[ i ] );
			l_input( Kasi, Ascbf );
			memset( Mony, '\0', 6 );
			l_sub( Mony, Kari, Kasi );
			l_add( KrSzrec->SZdtbl[j].SDval[0], KrSzrec->SZdtbl[j].SDval[0], Mony );
			j++;
		}
	}
	while( pZaimuSub->kzrec->MoveNext() != -1 );
	// 消費税集計残から精算残高の取得
	pZaimuSub->szsyk->MoveFirst();
	do {
		sgn = 0;
		// 仮払消費税額（対象外に格納）
		if( !strncmp( pZaimuSub->szvol->SVkcod1, pZaimuSub->szsyk->SZkcod, 8-2 ) &&
			pZaimuSub->szsyk->SZgrp1 == 0x04 && pZaimuSub->szsyk->SZgrp2 == 0x07 )	sgn = 1;
		// 仮受消費税（対象外に格納）
		if( !strncmp( pZaimuSub->szvol->SVkcod2, pZaimuSub->szsyk->SZkcod, 8-2 ) &&
			pZaimuSub->szsyk->SZgrp1 == 0x02 && pZaimuSub->szsyk->SZgrp2 == 0x0A )	sgn = 2;
		if( !sgn )	continue;
		// 仮受・仮払消費税チェック
		if( sgn == 1 )	KrSzrec = (struct _SZREC *)&sikrbri;
		else			KrSzrec = (struct _SZREC *)&sikruke;

		// 消費税処理グループ区分
		KrSzrec->SZgrp1 = pZaimuSub->szsyk->SZgrp1;
		// 消費税処理グループ区分・明細
		KrSzrec->SZgrp2 = pZaimuSub->szsyk->SZgrp2;
		// 税率テーブル番号
		KrSzrec->SZzkbn = pZaimuSub->szsyk->SZzkbn;
		// 金額を文字列からバイナリーへ変換
		for( i = 0, j = 0; i < 16; i++ ) {
			// 除外チェック
			switch( i ) {
				case 0 : j = 0;	break;
				case 1 : j = 1;	break;
				case 2 : j = 2;	break;
				case 3 : if( d_tno != 2 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 2;	
						 break;
				case 4 : j = 3;	break;
				case 5 : j = 4;	break;
				case 6 : j = 5;	break;
				case 7 : if( d_tno != 5 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 5;
						 break;
				case 8 : j = 6;	break;
				case 9 : j = 7;	break;
				case 10: j = 8;	break;
				case 11: if( d_tno != 8 && m_Except[i] )	continue;			// 除外チェック
						 else	j = 8;
						 break;
				case 12: j = 9;	break;
				case 13: j =10;	break;
				case 14: j =11;	break;
				case 15: j =12;	break;
			}
			// 設立時のチェック
			int Last_mon = Establish();
			if( Last_mon != -1 ) {
				if( Last_mon == i ) {
					j = Last_mon - 1;
					if( shin_ofst( &Vol1.EE_YMD[1], &j, &Vol1, &Vol2 ) )	j = 0;
				}
			}
			// 対象外
			memset( Ascbf, '\0', sizeof( Ascbf ) );
			strcpy_s( Ascbf, pZaimuSub->szsyk->SZtaig[i] );
			l_input( Mony, Ascbf );
			if( KrSzrec->SZkatt & 0x80 )	l_neg( Mony );
			l_add( KrSzrec->SZdtbl[j].SDval[0], KrSzrec->SZdtbl[j].SDval[0], Mony );
			j++;
		}
	}
	while( pZaimuSub->szsyk->MoveNext() != -1 );
	//////////////////////
	// 決算修正月の変換 //
	//////////////////////
	if( (pZaimuSub->zvol->s_sgn5&0x03) == 0x01 ) {
		// 各月決算修正
		for( i = 0; i != 13; i++ ) {
			TBhead.SVmctl[i] = 0x80;
		}
	}
	else	{
		switch( pZaimuSub->zvol->kes_type ) {
			case 1 : // 決算修正無し
				TBhead.SVmctl[11] = 0x80;
				break;
			case 2 : // 半期決算
				TBhead.SVmctl[5] = 0x80;
				TBhead.SVmctl[12] = 0x80;
				break;
			case 3 : // ４半期決算
				TBhead.SVmctl[2] = 0x80;
				TBhead.SVmctl[5] = 0x80;
				TBhead.SVmctl[8] = 0x80;
				TBhead.SVmctl[12] = 0x80;
				break;
			default: // 期末決算
				TBhead.SVmctl[12] = 0x80;
				break;
		}
		// 設立時のチェック
		int month = 0;
		if( shin_ofst( &Vol1.EE_YMD[1], &month, &Vol1, &Vol2 ) )	month = 0;
		if( month < 11 )	TBhead.SVmctl[month] = 0x80;
	}
	//////////////////////////////////////////////////////////////////////////////
	// 一括税抜月以外の仮払・仮受消費税額を一括税抜月に移動し精算残高を差し引く //
	//////////////////////////////////////////////////////////////////////////////
	for( sgn = 0; sgn != 2; sgn++ ) {
		// 仮受・仮払消費税チェック
		if( !sgn )	KrSzrec = (struct _SZREC *)&ikkrbri;
		else		KrSzrec = (struct _SZREC *)&ikkruke;
		if( !sgn )	KrSzrecEx = (struct _SZREC *)&sikrbri;
		else		KrSzrecEx = (struct _SZREC *)&sikruke;
		memset( Kari, '\0', sizeof( Kari ) );
		memset( Kasi, '\0', sizeof( Kasi ) );
		for( i = 0; i < 13; i++ ) {
			l_add( Kari, Kari, KrSzrec->SZdtbl[i].SDval[0] );
			memset( KrSzrec->SZdtbl[i].SDval[0], '\0', 6 );
			l_add( Kasi, Kasi, KrSzrecEx->SZdtbl[i].SDval[0] );
			memset( KrSzrecEx->SZdtbl[i].SDval[0], '\0', 6 );
			if( TBhead.SVmctl[i]&0x80 )	{
				l_add( KrSzrec->SZdtbl[i].SDval[0], KrSzrec->SZdtbl[i].SDval[0], Kari );
				l_add( KrSzrecEx->SZdtbl[i].SDval[0], KrSzrecEx->SZdtbl[i].SDval[0], Kasi );
				
				l_sub( KrSzrec->SZdtbl[i].SDval[0], KrSzrec->SZdtbl[i].SDval[0], KrSzrecEx->SZdtbl[i].SDval[0] );
				
				memset( Kari, '\0', sizeof( Kari ) );
				memset( Kasi, '\0', sizeof( Kasi ) );
			}
		}
	}
	return( 0 );
}

// 消費税関係マスターの読み込み
// 返送値 -1:エラー
//		   0::読み込みＯＫ
//		   1:中間２６号様式
int CShinCalculateW::Shinkoku_Read( void )
{
	// クラスの取得
	if( (pZaimuSub->zvol->apno&0xff) > 0x30 )	m_Class = 1;	// 公益クラス
	else										m_Class = 0;	// 財務クラス
	// 期間モジュール
	if( pVolDate == NULL )	pVolDate = new CVolDateDB;
	if( pVolDate == NULL ) {
		ErrMessage = _T("ＤＢ期間モジュールクラスの構築が出来ませんでした。");
		return( ERR );
	}
	// テーブルハンドリングモジュール
	if( pTblHdl == NULL )	pTblHdl = new CTblhdl;
	if( pTblHdl == NULL ) {
		ErrMessage = _T("ＤＢテーブルハンドリングモジュールクラスの構築が出来ませんでした。");
		return( ERR );
	}
	// 倍長演算
	if( pArithEx == NULL )	pArithEx = new CArithEx;
	if( pArithEx == NULL ) {
		ErrMessage = _T("ＤＢ倍長演算クラスの構築が出来ませんでした。");
		return( ERR );
	}
#ifdef	_CLOSE
	char	SecBf1[20], SecBf2[20], SecBf3[20], SecBf4[20];
	memset( (char *)SecBf1, '\0', sizeof( SecBf1 ) );
	memset( (char *)SecBf2, '\0', sizeof( SecBf2 ) );
	memset( (char *)SecBf3, '\0', sizeof( SecBf3 ) );
	memset( (char *)SecBf4, '\0', sizeof( SecBf4 ) );		
	if( PCOM_GetString( "ProgramDevice", SecBf1 ) == -1 )	{
		ErrMessage =  _T("プログラムデバイスが取得できませんでした。" );
		return( ERR );
	}
	// マスターデバイス
	if( PCOM_GetString( _T("MasterDevice"), MDev, MDdm ) == -1 ) {
		ErrMessage =  _T("マスターデバイスが取得できません。" );
		return( ERR );
	}
	// グループ番号
	if( PCOM_GetString( "ExecGrJobs", SecBf2, SecBf3 ) == -1 )	{
		ErrMessage =  _T("プログラム管理番号が取得できませんでした。" );
		return( ERR );
	}
	// 共用デバイス
	memset( GroupTh, '\0', sizeof( GroupTh ) );
	strcpy_s( GroupTh, sizeof( GroupTh ), SecBf2 );
	if( GCOM_GetString( "SystemDrive", SecBf4 ) == -1 )	{
		ErrMessage =  _T("共用デバイスが取得できませんでした。" );
		return( ERR );
	}
	sprintf_s( Jpath, sizeof( Jpath ), "%s:\\ICSWIN\\CMDS\\GR%s\\JOBS%s", SecBf1, SecBf2, SecBf3 );
	sprintf_s( Spath, sizeof( Spath ), "%s:\\ICSWIN\\SYS", SecBf4 );

	// 共通グループフォルダ名取得
	sprintf_s( SGpath, sizeof( SGpath ), "%s\\GR%s",Spath, GroupTh );

	// オーナーＩＤの取得	ユーザ登録テーブルオープン時参照
	if( icsmctl.SetDatabase() ) {
		ErrMessage = _T("オーナーＩＤが取得出来ませんでした。");
		return( ERR );
	}
	IcsMctl.get_usertbl( m_usertbl );
#endif

	// 倍長演算指定
	l_defn( 0x16 );
	pArithEx->l_defn( 0x16 );

	//////////////////////////
	// 財務マスターオープン //
	//////////////////////////

#ifdef	_CLOSE
	// 特定収入対応
	if( m_Class )	m_Err = NPSySkZanCalq( pZaimuSub );
	else			m_Err = DBSySkZanCalq( pZaimuSub );
	if( m_Err ) {
		ErrMessage = GetSsErrMsg();
		return( ERR );
	}
#endif

	// ボリューム・ラベル
	if( pZaimuSub->zvol == NULL )	{
		if( pZaimuSub->VolumeOpen() == ERR )	{
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return( ERR );
		}
		m_OpenFlg1 |= 0x01;
	}

	// 課税取引金額計算表 個人決算書対応マスターチェック
	m_Basic = 0x00;
	if( pZaimuSub->zvol->m_ver == 0x12 && 
		( (pZaimuSub->zvol->apno&0xf0) == 0x10 || pZaimuSub->zvol->ind_type ) )	m_Basic = 0x01;
	// 一括税抜マスターチェック
	m_Ikat = 0x00;
	m_Ikat = ((pZaimuSub->zvol->s_sgn1&0xc0) & 0xc0 );
	// 科目名称
	if( pZaimuSub->knrec == NULL ) { 
		if( pZaimuSub->KamokuMeisyoOpen( "" ) == ERR )	{
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return( ERR );
		}
		m_OpenFlg1 |= 0x02;
	}
	else	pZaimuSub->knrec->Requery("",0);
	// 科目名称読み込み
	if( KnTablRead() == ERR ) {
		ErrMessage = _T("科目名称を読み込み中に障害が発生しました。");
		return( ERR );
	}
	// 科目残高
	if( pZaimuSub->kzrec == NULL ) {
		if( pZaimuSub->KamokuZanOpen( "", 0 ) == ERR )	{
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return( ERR );
		}
		m_OpenFlg1 |= 0x04;
	}
	else	pZaimuSub->kzrec->Requery("");
	//////////////////////
	// 月情報　オープン //
	//////////////////////
	if( pZaimuSub->minfo == NULL ) {
		if( pZaimuSub->MinfoOpen() == ERR )	{
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return( ERR );
		}
		m_OpenFlg1 |= 0x08;
	}
	else	pZaimuSub->minfo->Requery("");
	// 各月集計サインチェック
	for( int Mcnt = 0; Mcnt != MONTH_MAX; Mcnt++ ) {
		// データの読み込み
		if( !Mcnt ) {
			if( pZaimuSub->minfo->MoveFirst() == ERR )	{
				ErrMessage = _T("月情報の読み込み中に障害が発生しました。");
				pZaimuSub->MinfoClose();
				return( ERR );
			}
		}
		else	{
			if( pZaimuSub->minfo->MoveNext() == ERR )	{
				ErrMessage = _T("月情報の読み込み中に障害が発生しました。");
				pZaimuSub->MinfoClose();
				return( ERR );
			}
		}
		// 決算除外サイン
		m_Except[ Mcnt ] = pZaimuSub->minfo->kes_syk_cut;
	}

	// 消費税管理
	if( pZaimuSub->szvol == NULL ) {
		if( pZaimuSub->SyohizeiOpen() == ERR )	{
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return( ERR );
		}
		m_OpenFlg2 |= 0x01;
	}
	// 消費税申告書
	if( pZaimuSub->szsnk == NULL ) {
		if( pZaimuSub->SzsnkOpen() == ERR )	{
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return( ERR );
		}
		m_OpenFlg2 |= 0x02;
	}

	// 消費税残高集計ファイル
	if( pZaimuSub->szsyk == NULL ) {
		if( pZaimuSub->SzsykOpen("") == ERR )	{
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return( ERR );
		}
		m_OpenFlg2 |= 0x04;
	}
	else	pZaimuSub->szsyk->Requery("");

#ifdef	VERSIONUP
	// 特定収入マスターチェック
	m_SpcSg = 0;
	if( (pZaimuSub->zvol->apno&0xf0) == 0x40 || (pZaimuSub->zvol->apno&0xf0) == 0x50 )	{	// 学校・公益・福祉・宗教
		if( (pZaimuSub->zvol->s_sgn2&0x0f) == 0x00 )	{									// 原則課税
			// 特定収入
			if( pZaimuSub->m_pSpsyk == NULL ) {
				if( pZaimuSub->NpTokuteiSyunyuOpen("") == ERR )	{
					ErrMessage = pZaimuSub->GetZmErrMsg();
					return( ERR );
				}
				m_OpenFlg2 |= 0x08;
			}
			else	pZaimuSub->m_pSpsyk->RequeryEx("");
			m_Specnt = pZaimuSub->m_pSpsyk->GetRecordCountEx();
			m_SpcSg = 1;
		}
	}
#endif

	////////////////////
	// 各マスター変換 //
	////////////////////
	// データベース会社情報をアトラス会社情報へコンバージョン
	ZaimuVolume_Cnv();
	// 消費税ボリュームラベル変換
	SyzVolume_Cnv();

#ifdef	VERSIONUP
	// 特定収入
	if( m_SpcSg ) {
		// 特定収入
		if( SpRead() == ERR ) {
			ErrMessage = _T("特定収入マスターの読み込み中に障害が発生しました。");
			return( ERR );
		}
	}
#endif

	// 常に新規から始まる
	SyzShin_Make();
	TBhead.SVsasw = 0x01;
	if( (pZaimuSub->zvol->apno&0xf0) == 0x10 || pZaimuSub->zvol->ind_type )	Snh.Sn_KOJIN |= 0x01;
	// 課税期間セット
	memmove( Snh.Sn_KDAYS, Start, 3 );
	memmove( Snh.Sn_KDAYE, End, 3 );

	// 消費税情報設定
	memset( SYsw, '\0', sizeof( SYsw ) );
	for( int MoCt = 0; MoCt < 13; ++MoCt ) {
		SYsw[MoCt] = (Vol2.D_TBL[MoCt].C_KSW & 0x40) ? 0x01 : 0;
	}
	// 各サイン初期設定
	K221sw = 0;		// 規則２２条１項の適用積み上げ方式サイン
	H301sw = 0;		// 法第３０条第１項の適用積み上げ方式サイン
	SimpSw = 0;		// 簡易計算サイン
	toku = 0;
	Kenei = 0x00;
	if( Vol1.S_SGN2 & 0x80 )	Kenei = 0x01;	// 兼業事業者
	else						Kenei = 0x00;	// 単一事業者
	Kani = (int)(Vol1.S_SGN2 & 0x0f);			// 業種区分
	if( Vol1.S_SGN4 & 0x80 )	Anbun = 1;		// 個別対応
	else						Anbun = 0;		// 比例配分
	// 消費税集計の為のサイン生成
	Skbn = (int)(Snh.Sn_SKKBN&0xff);
	// 課税売上割合に準ずる割合を有効サイン
	Snh.Sn_UPERS &= 0xfd;
	RATIO = 0x00;
	// 消費税集計
	ofs_get();

	// 申告書用ワークエリアの初期化
	so_read();		// 旧ﾊﾞｰｼﾞｮﾝ用
	sn_read();		// 新ﾊﾞｰｼﾞｮﾝ用

	if( forcreat( &foreA, &foreB, &foreC ) )	{
		ErrMessage = _T("売上割合集計テーブルが確保できません" );
		return( ERR );
	}
	// 消費税科目集計ファイルリード
	if( rui_read( &foreA, &foreB, &foreC ) )		{
		ErrMessage = _T("消費税科目集計ファイルが読み込めません" );
		return( ERR );
	}	
	// 一括税抜時の仮受・仮払消費税額の取得
	if( IkTaxZanRead() ) {
		ErrMessage = _T("仮受・仮払消費税の残高が取得できません！");
		return( ERR );
	}
	// 設立開始オフセット
	SetUpOfs = 0;
	if( shin_ofst( &Vol1.SS_YMD[1], &SetUpOfs, &Vol1, &Vol2 ) )	SetUpOfs = 0;

	return( 0 );
}

// 申告書集計
int CShinCalculateW::Shinkoku_Calcurate( SUMDATA* sumdata )
{
	int		StTs;
	char	ChkSw, ChkDm;
	char	Xymd[4] = { 0x09, 0x04, 0x01, 0x00 };

	toku = (int)(Snh.Sn_TOKUR & 0xff);			//特例計算の適用
	K221sw = H301sw = 0;
	if( Snh.Sn_TUMIAG & 0x01 )	K221sw = 1;		// 規則２２条１項の適用（積み上げ方式）
	if( Snh.Sn_TUMIAG & 0x02 )	H301sw = 1;		// 法第３０条第１項の適用（積み上げ方式）
	Snh.Sn_UPERS &= 0xef;
	Snd.Sn_2FUWSEX = 0;
	// 消費税集計
	ofs_get();

	if( toku )	Kenei |= 0x10;							//特例計算摘要ＳＷ		
	// 集計テーブル  イニシャライズ
	if( forcreat( &foreA, &foreB, &foreC ) )	{
		ErrMessage = _T("売上割合集計テーブルが確保できません");
		return( ERR );
	}
	// 消費税科目集計ファイルリード
	if( rui_read( &foreA, &foreB, &foreC ) )		{
		ErrMessage = _T("消費税科目集計ファイルが読み込めません");
		return( ERR );
	}
	// 一括税抜時の仮受・仮払消費税額の取得
	if( IkTaxZanRead() ) {
		ErrMessage = _T("仮受・仮払消費税の残高が取得できません！");
		return( ERR );
	}
	// 設立開始オフセット
	SetUpOfs = 0;
	if( shin_ofst( &Vol1.SS_YMD[1], &SetUpOfs, &Vol1, &Vol2 ) )	SetUpOfs = 0;
	// 課税仕入区分・売上区分集計エリアのリード
	if( suk_read( &SVsum, &foreB ) )	{
		ErrMessage = _T("課税仕入区分・売上区分集計領域が読み込めません");
		return( ERR );
	}
	// 該当月選択＆集計
	int	save_tno = 0;
	int	ans_tno = 0;
	// 課税期間で申告書集計を行う
	if( Kk_SG&0x10 ) {
		save_tno = s_tno;
		if( shin_ofst( &Snh.Sn_KDAYS[1], &ans_tno, &Vol1, &Vol2 ) )	{
			Kk_SG &= 0xef;
			s_tno = (char)save_tno;
		}
		else	s_tno = (char)ans_tno;
	}
	// 簡易計算
	Snh.Sn_SPECIAL &= 0xfe;
	if( SimpSw )	Snh.Sn_SPECIAL |= 0x01;
	// 貨物の税額を入力税額より計算
	Snh.Sn_SPECIAL &= 0xfd;
	ChkSw = ChkDm = 0x00;
	ChkSw = TBhead.SVmzsw;
	// 科目残高集計
	if( formmake( s_tno, d_tno, &kr, &karibri_5, &kariuke_5 ) )	{
		ErrMessage = _T("科目残高集計中に障害が発生しました");
		return( ERR );
	}
	// 経過措置　仮払・仮受消費税集計
	formmake3( s_tno, d_tno, &kr3, &karibri_3, &kariuke_3 );
	l_add( kr.barai, kr.barai, kr3.barai );
	l_add( kr.uke, kr.uke, kr3.uke );

	// 棚卸計算
	if( Anbun && m_Stock == 1 )	{ 
		// 比例配分及び簡易課税マスター時の棚卸調整の金額を取り込み
		// 税込・税抜サイン設定
		TBhead.SVsign &= 0xef;
		if( SumInvent.InSw1&0x10 )	TBhead.SVsign = 0x10;
		// 棚卸選択
		Snd.Sn_MENZsw =	0x00;
		switch( SumInvent.InSw1&0x0f ) {
			case 0x01 :	Snd.Sn_MENZsw =	0x10;	break;
			case 0x02 :	Snd.Sn_MENZsw =0x11;	break;	
			default:	break;
		}
		TBhead.SVzchg = Snd.Sn_MENZsw;
		memmove( Snd.Sn_mnk40[0], SumInvent.INVal[0].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
		memmove( Snd.Sn_mnz40[0], SumInvent.INVal[1].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_mnk40[1], SumInvent.INVal[2].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
		memmove( Snd.Sn_mnz40[1], SumInvent.INVal[3].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_mnk40[2], SumInvent.INVal[4].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
		memmove( Snd.Sn_mnz40[2], SumInvent.INVal[5].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_menzk[0], SumInvent.INVal[6].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　課税に係る
		memmove( Snd.Sn_menzz[0], SumInvent.INVal[7].val, 6 );	// 				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_menzk[1], SumInvent.INVal[8].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　非課税に係る
		memmove( Snd.Sn_menzz[1], SumInvent.INVal[9].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_menzk[2], SumInvent.INVal[10].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　共通に係る
		memmove( Snd.Sn_menzz[2], SumInvent.INVal[11].val, 6 );	//				 〃	   消費税額   (〃      )　〃
	}
	else	{
		StackMake( s_tno, d_tno, &kr, stack_data );
		if( Anbun && (pZaimuSub->zvol->s_sgn4 & 0x80) )	{
			// マスターが個別対応時は棚卸調整を返す
			SumInvent.InSw1 = 0x00;
			if( TBhead.SVsign == 0x10 )	SumInvent.InSw1 = 0x10;
			switch( Snd.Sn_MENZsw ) {
				case 0x10 :	SumInvent.InSw1 |= 0x01;	break;
				case 0x11 :	SumInvent.InSw1 |= 0x02;	break;	
				default   :
					break;
			}
			memmove(  SumInvent.INVal[0].val,Snd.Sn_mnk40[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
			memmove(  SumInvent.INVal[1].val,Snd.Sn_mnz40[0], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[2].val,Snd.Sn_mnk40[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
			memmove(  SumInvent.INVal[3].val,Snd.Sn_mnz40[1], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[4].val,Snd.Sn_mnk40[2], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
			memmove(  SumInvent.INVal[5].val,Snd.Sn_mnz40[2], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[6].val,Snd.Sn_menzk[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　課税に係る
			memmove(  SumInvent.INVal[7].val,Snd.Sn_menzz[0], 6 );	// 				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[8].val,Snd.Sn_menzk[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　非課税に係る
			memmove(  SumInvent.INVal[9].val,Snd.Sn_menzz[1], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[10].val,Snd.Sn_menzk[2], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　共通に係る
			memmove(  SumInvent.INVal[11].val,Snd.Sn_menzz[2], 6 );	//				 〃	   消費税額   (〃      )　〃
		}
	}
	// その他の金額
	if( Kani < 2 ) {
		// 調整対象固定資産に係る調整税額
		memmove( Snd.Sn_2F19B, SumMony[m_Calcusw].MOVal[0].val, 6 );
		// 調整対象固定資産を課税業務用に転用した調整額
		memmove( Snd.Sn_2F20B, SumMony[m_Calcusw].MOVal[1].val, 6 );
	}
	// 中間納付税額
	memmove( Snd.Sn_TYNOFZ, SumMony[m_Calcusw].MOVal[2].val, 6 );
	// 中間納付譲渡割額
	memmove( Snd.Sn_TTYWAR, SumMony[m_Calcusw].MOVal[3].val, 6 );

	// 集計レコードクリア
	if( for_clear( &foreA ) )	{
		ErrMessage = _T("集計レコードクリア中に障害が発生しました");
		return( ERR );
	}
	if( SimpSw )
			StTs = stm_scalq( s_tno, d_tno, kr.barai, kr.uke, kr3.barai, kr3.uke, &foreA, &foreB );
	else	{
		StTs = stm_ucalq( s_tno, d_tno, kr.barai, kr.uke, kr3.barai, kr3.uke, &foreA, &foreB );
	}
	if( m_Check ) {
		// ５億円チェックチェックメッセージを作成
		if( StTs == 1 || StTs == 2 ) {
			switch( m_Type )	{
				case 1 :	// 翌期更新・前年度からの更新
					// 翌期更新・前年度からの更新
					CheckMessage =  _T("参考\n平成２４年４月１日以後開始する課税期間より、課税売上高が５億円超の事業者は、\n");
					CheckMessage += _T("課税売上割合が９５％以上であっても消費税の仕入税額の全額控除はできません。\n");
					CheckMessage += _T("来期は課税売上高が５億円超を見込まれる場合、予め個別対応方式を検討してください。\n");
					CheckMessage += _T("なお、一括比例配分方式を選択した場合は、２年間以上継続した後でなければ\n");
					CheckMessage += _T("個別対応方式に変更できません。");
					break;
				case 2 :	// 消費税額試算表（一行表示可）
					if( StTs == 1 )	{
						if( l_test( m_CnvVal ) == 0 )	CheckMessage = _T("課税売上高が、５億円を超えています。" );
						else							CheckMessage = _T("課税売上高が、年換算で５億円を超えています。" );
					}
					break;
				default:	// 試算表・課税売上割合・消費税申告書（二行表示可）
					if( StTs == 1 )	{
						if( l_test( m_CnvVal ) == 0 )	CheckMessage = _T("課税売上高が、５億円を超えています。\n" );
						else							CheckMessage = _T("課税売上高が、年換算で５億円を超えています。\n" );
						CheckMessage += _T("比例配分方式の選択も含めて注意してください。" );
					}
					break;
			}
		}
		if( StTs != 3 )	ErrMessage = _T("消費税集計中に障害が発生しました");
		return( StTs );
	}
	else	{
		if( StTs == ERR ) {
//★//[12'02.29]///
//			ErrMessage = _T("消費税集計中に障害が発生しました");
///////////////////
			if( ErrMessage.GetAllocLength() == 0 )	ErrMessage = _T("消費税集計中に障害が発生しました");
///////////////////
			return( ERR );
		}
	}
	// 初回、課税売上高割合の計算式表示用残高セット
	if( !m_Calcusw || !m_Compare ) {
		memmove( SumRatio.RTVal[0].val, stval.kwbuns, 6 );	// 課税売上高
		memmove( SumRatio.RTVal[1].val, stval.kwbunb, 6 );	// 総売上高
		memmove( SumRatio.RTVal[2].val, stval.uwriai, 6 );	// 課税売上割合
	}
///////////////////

	// 経過措置有り
#ifdef	VERSIONUP
	// 特定収入　処理
	if( m_SpcSg ) {
		// 初期設定
		Snd.Sn_2F21sw = 0x00;
		memset( Snd.Sn_2F21A, '\0', 6 );
		memset( Snd.Sn_2F21B, '\0', 6 );
//★//[12'03.12]///
		SPassOffCalq();		// 経過措置なし
		Snd.Sn_2F21sw |= 0x10;
		memmove( Snd.Sn_2F21B, stval.SpSizg, 6 );
///////////////////
//		// 特定収入に係る課税仕入れ等の税額計算
//		if( TBhead.SVmzsw == 1 )	SPassOnCalq();		// 経過措置有り
//		else						SPassOffCalq();		// 経過措置なし
//		if( TBhead.SVmzsw == 1 )	{
//		// 経過措置対象の課税資産の譲渡等有り
//			Snd.Sn_2F21sw |= 0x01;
//			memmove( Snd.Sn_2F21A, stval.SpSiz3, 6 );
//			Snd.Sn_2F21sw |= 0x10;
//			memmove( Snd.Sn_2F21B, stval.SpSiz4, 6 );
//		}
//		else	{
//		// 経過措置対象の課税資産の譲渡等無し
//			Snd.Sn_2F21sw |= 0x10;
//			memmove( Snd.Sn_2F21B, stval.SpSizg, 6 );
//		}
///////////////////
	}
#endif

	// 消費税ファイルも更新
	so_close();						// 旧ﾊﾞｰｼﾞｮﾝ用
	sn_close();						// 新ﾊﾞｰｼﾞｮﾝ用
	// 課税期間で申告書集計を行う	[03'04.15]
	if( Kk_SG&0x10 )	s_tno = save_tno;

	// 課税方式比較表のパケットへデータを転送
	SumilateConvert( sumdata );

	return( 0 );
}

// 簡易課税　2種類以上の業種での特例計算チェック
int CShinCalculateW::toku75( char *Base, char *Add1, char *Add2  )
{
	char	EXPW0[6], EXPW1[6], KZMY0[6], KZMY1[6], KZMY2[6];
	int		Status;

	// 計算準備
	memset( EXPW0, '\0', 6 );
	memset( EXPW1, '\0', 6 );
	l_input( EXPW0, "750" );
	l_input( EXPW1, "1000" );

	// 選択された2業種を合計する
	memset( KZMY0, '\0', 6 );
	l_add( KZMY0, Add1, Add2 );

	// 事業区分合計
	memset( KZMY1, '\0', 6 );
	memmove( KZMY1, Base, 6 );

	// 選択された2業種を合計を基に割合計算
	memset( KZMY2, '\0', 6 );
	l_6calq( KZMY2, KZMY0, EXPW1, KZMY1 );	// 構成比
	
	// 特例７５％比較
	if( l_cmp( KZMY2, EXPW0 ) >= 0 )	Status = 1;	// 75%以上
	else								Status = 0;

	return( Status );
}

// 財務マスタ－集計
int CShinCalculateW::formmake( int sm, int km, struct KARI *krval, struct _SZREC *barai, struct _SZREC *uke )
{
	int	i;

	// 仮受・仮払消費税集計
	memset( krval->barai, '\0', 6 );
	memset( krval->uke, '\0', 6 );

	for( i = sm; i < ( km + 1 ); i++ ) {
		l_add( krval->barai, krval->barai, barai->SZdtbl[i].SDval[0] );
		l_add( krval->uke, krval->uke, uke->SZdtbl[i].SDval[0] );
	}

	if( barai->SZkatt & 0x80 ) l_neg( krval->barai );
	if( uke->SZkatt & 0x80 ) l_neg( krval->uke );

	return( FALSE );
}

// 経過措置税率	仮払・仮受消費税　集計
void CShinCalculateW::formmake3( int sm, int km, struct KARI *des, struct _SZREC *barai, struct _SZREC *uke )
{
	int		i;

	memset( des->barai, '\0', 6 );
	memset( des->uke, '\0', 6 );

	for( i = sm; i < ( km + 1 ); i++ ) {
		l_add( des->barai, des->barai, barai->SZdtbl[i].SDval[0] );
		l_add( des->uke, des->uke, uke->SZdtbl[i].SDval[0] );
	}

	if( barai->SZkatt & 0x80 ) l_neg( des->barai );
	if( uke->SZkatt & 0x80 ) l_neg( des->uke );
}

// 棚卸調整税額を科目残より差引く
void CShinCalculateW::StackMake( int sm, int km, struct KARI *kr, struct _SZREC *sou )
{
	int		i, j, k;
	struct	_SZREC	*recp;

	// 前回、棚卸調整仕訳があった場合関係エリアを全てクリアする。[04'10.15]
	if( Snd.Sn_tansw & 0x01 ) {
		Snd.Sn_MENZsw = 0x00;
		l_clear( Snd.Sn_MENZK );	//	免税 控除対象仕入額 
		l_clear( Snd.Sn_MENZZ );	//	免税 控除対象仕入税額
		l_clear( Snd.Sn_MNK40 );	//	免税 控除対象仕入額(新税率分)
		l_clear( Snd.Sn_MNZ40 );	//	免税 控除対象仕入税額(新税率分)	
		Snd.Sn_tansw = 0x00;
		for( i = 0; i != 3; i++ ) {
			memset( Snd.Sn_mnk40[i], '\0', 6 );
			memset( Snd.Sn_mnz40[i], '\0', 6 );
			memset( Snd.Sn_menzk[i], '\0', 6 );
			memset( Snd.Sn_menzz[i], '\0', 6 );
		}
	}

	// 棚卸中間バッファクリア
	for( i = 0; i != 6; i++ ) {
		memset( ST_komi[i], '\0', 6 );
		memset( ST_komz[i], '\0', 6 );
		memset( ST_nuki[i], '\0', 6 );
		memset( ST_nukz[i], '\0', 6 );
		memset( ST_nzei[i], '\0', 6 );
	}

	// 棚卸調整額＆税額の取得
	if( Anbun ) {
	// 個別対応
		for( i = sm; i < ( km + 1 ); i++ ) {
			
			for( j = 0; j != 6; j++ ) {
				switch( j ) {
					case  0 :	recp = ( sou + 1 );	break;
					case  1 :	recp = ( sou + 3 );	break;
					case  2 :	recp = ( sou + 5 );	break;
					case  3 :	recp = ( sou + 2 );	break;
					case  4 :	recp = ( sou + 4 );	break;
					case  5 :	recp = ( sou + 6 );	break;
					default :	return;
								break;
				}
				memset( PW0, '\0', 6 );
				l_input( PW0, "80" );
				// 抜き
				memset( WORK0, '\0', 6 );
				l_add( ST_nuki[j], ST_nuki[j], recp->SZdtbl[i].SDval[5] );
				if( !SimpSw ) {	// 簡易計算
					if( j < 3 )	{
						if( !H301sw )	{
							memset( WORK1, '\0', 6 );
							l_add( WORK1, recp->SZdtbl[i].SDval[5], recp->SZdtbl[i].SDval[6]);
							percent( WORK0, WORK1, 4, 40, 0 );
						}
						else	pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)recp->SZdtbl[i].SDval[6],(unsigned char *)PW0, 0, 0 );
					}
					else		memmove( WORK0, recp->SZdtbl[i].SDval[6], 6 );
				}
				else	{
					if( j < 3 )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 1, 40, 0 );
					else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 1, 30, 0 );
				}
				l_add( ST_nukz[j], ST_nukz[j], WORK0 );

				//	別記
				memset( WORK0, '\0', 6 );
				l_add( ST_nuki[j], ST_nuki[j], recp->SZdtbl[i].SDval[7] );
				if( j < 3 )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 1, 40, 0 );
				else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 1, 30, 0 );
				l_add( ST_nukz[j], ST_nukz[j], WORK0 );

				// 抜き・込み混在時は込みとする為の税額
				memset( WORK0, '\0', 6 );
				if( !SimpSw ) {	// 簡易計算
					if( j < 3 )	memmove( WORK0, recp->SZdtbl[i].SDval[6], 6 );
					else		memmove( WORK0, recp->SZdtbl[i].SDval[6], 6 );
				}
				else	{
					if( j < 3 )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 2, 50, 0 );
					else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 1, 30, 0 );
				}
				l_add( ST_nzei[j], ST_nzei[j], WORK0 );

				// 別記
				memset( WORK0, '\0', 6 );
				if( j < 3 )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 2, 50, 0 );
				else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 1, 30, 0 );
				l_add( ST_nzei[j], ST_nzei[j], WORK0 );

				// 税込み
				memset( WORK0, '\0', 6 );
				l_add( ST_komi[j], ST_komi[j], recp->SZdtbl[i].SDval[3] );
				if( !SimpSw ) {	// 簡易計算
					if( j < 3 )	{
						if( !H301sw )	percent( WORK0,(char *) recp->SZdtbl[i].SDval[3], 4, 40, 0 );
						else			pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)recp->SZdtbl[i].SDval[4],(unsigned char *)PW0, 0, 0 );
					}
					else		memmove( WORK0, recp->SZdtbl[i].SDval[4], 6 );
				}
				else	{
					if( j < 3 )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[3], 4, 40, 0 );
					else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[3], 0, 30, 0 );
				}
				l_add( ST_komz[j], ST_komz[j], WORK0 );

			}
		}

		Snd.Sn_tansw &= 0xfe;
		for( i = 0,j = 0,k = 0; i != 6; i++ ) {
			if( l_test( ST_nuki[i] ) )	j++;
			if( l_test( ST_komi[i] ) )	k++;
		}
		if( j || k ) {
			// 棚卸調整仕訳有り
			Snd.Sn_tansw |= 0x01;
			// 該当アリアへ転送
			if( j && k ) {
				// 税抜き・税込み混在は税込みとする
				for( i = 0; i != 6; i++ ) {
					l_add( ST_komi[i], ST_komi[i], ST_nuki[i] );
					l_add( ST_komi[i], ST_komi[i], ST_nzei[i] );
					l_add( ST_komz[i], ST_komz[i], ST_nukz[i] );
					memset( ST_nuki[i], '\0', 6 );
					memset( ST_nukz[i], '\0', 6 );
					memset( ST_nzei[i], '\0', 6 );
				}
				j = 0;
			}

			TBhead.SVsign &= 0xef;
			Snd.Sn_MENZsw = 0x10;
			if( k )	{	// 込み
				TBhead.SVsign |= 0x10;
				memset( WORK0, '\0', 6 );
				for( i = 0; i != 6; i++ ) {
					l_add( WORK0, WORK0, ST_komi[i] );
				}
				if( l_test( WORK0 ) < 0 )	{
					Snd.Sn_MENZsw |= 0x01;
					for( i = 0; i != 6; i++ ) {
						l_neg( ST_komi[i] );
						l_neg( ST_komz[i] );
					}
				}
				memmove( Snd.Sn_mnk40[0], ST_komi[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
				memmove( Snd.Sn_mnz40[0], ST_komz[0], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_mnk40[1], ST_komi[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
				memmove( Snd.Sn_mnz40[1], ST_komz[1], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_mnk40[2], ST_komi[2], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
				memmove( Snd.Sn_mnz40[2], ST_komz[2], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_menzk[0], ST_komi[3], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　課税に係る
				memmove( Snd.Sn_menzz[0], ST_komz[3], 6 );	// 				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_menzk[1], ST_komi[4], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　非課税に係る
				memmove( Snd.Sn_menzz[1], ST_komz[4], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_menzk[2], ST_komi[5], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　共通に係る
				memmove( Snd.Sn_menzz[2], ST_komz[5], 6 );	//				 〃	   消費税額   (〃      )　〃
			}
			else	{	// 抜き
				memset( WORK0, '\0', 6 );
				for( i = 0; i != 6; i++ ) {
					l_add( WORK0, WORK0, ST_nuki[i] );
				}
				if( l_test( WORK0 ) < 0 )	{
					Snd.Sn_MENZsw |= 0x01;
					for( i = 0; i != 6; i++ ) {
						l_neg( ST_nuki[i] );
						l_neg( ST_nukz[i] );
					}
				}
				memmove( Snd.Sn_mnk40[0], ST_nuki[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
				memmove( Snd.Sn_mnz40[0], ST_nukz[0], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_mnk40[1], ST_nuki[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
				memmove( Snd.Sn_mnz40[1], ST_nukz[1], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_mnk40[2], ST_nuki[2], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
				memmove( Snd.Sn_mnz40[2], ST_nukz[2], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_menzk[0], ST_nuki[3], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　課税に係る
				memmove( Snd.Sn_menzz[0], ST_nukz[3], 6 );	// 				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_menzk[1], ST_nuki[4], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　非課税に係る
				memmove( Snd.Sn_menzz[1], ST_nukz[4], 6 );	//				 〃	   消費税額   (〃      )　〃
				memmove( Snd.Sn_menzk[2], ST_nuki[5], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　共通に係る
				memmove( Snd.Sn_menzz[2], ST_nukz[5], 6 );	//				 〃	   消費税額   (〃      )　〃
				// 消費税集計時に再度集計が必要　９５％未満
			}
			TBhead.SVzchg = Snd.Sn_MENZsw;	// [04'11.16]
		}
	}
	else	{
	// 比例配分
		for( i = sm; i < ( km + 1 ); i++ ) {
			
			for( j = 0; j != 2; j++ ) {
				switch( j ) {
					case  0 :	recp = ( sou + 1 );	break;
					case  1 :	recp = ( sou + 2 );	break;
					default :	return;
								break;
				}
				memset( PW0, '\0', 6 );
				l_input( PW0, "80" );
				// 抜き
				memset( WORK0, '\0', 6 );
				l_add( ST_nuki[j], ST_nuki[j], recp->SZdtbl[i].SDval[5] );
				if( !SimpSw ) {	// 簡易計算
					if( !j )	{
						if( !H301sw )	{
							memset( WORK1, '\0', 6 );
							l_add( WORK1, recp->SZdtbl[i].SDval[5], recp->SZdtbl[i].SDval[6]);
							percent( WORK0, WORK1, 4, 40, 0 );
						}
						else	pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)recp->SZdtbl[i].SDval[6],(unsigned char *)PW0, 0, 0 );
					}
					else		memmove( WORK0, recp->SZdtbl[i].SDval[6], 6 );
				}
				else	{
					if( !j )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 1, 40, 0 );
					else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 1, 30, 0 );
				}
				l_add( ST_nukz[j], ST_nukz[j], WORK0 );

				//	別記
				memset( WORK0, '\0', 6 );
				l_add( ST_nuki[j], ST_nuki[j], recp->SZdtbl[i].SDval[7] );
				if( !j )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 1, 40, 0 );
				else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 1, 30, 0 );
				l_add( ST_nukz[j], ST_nukz[j], WORK0 );

				// 抜き・込み混在時は込みとする為の税額
				memset( WORK0, '\0', 6 );
				if( !SimpSw ) {	// 簡易計算
					if( !j )	memmove( WORK0, recp->SZdtbl[i].SDval[6], 6 );
					else		memmove( WORK0, recp->SZdtbl[i].SDval[6], 6 );
				}
				else	{
					if( !j )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 2, 50, 0 );
					else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[5], 1, 30, 0 );
				}
				l_add( ST_nzei[j], ST_nzei[j], WORK0 );

				// 別記
				memset( WORK0, '\0', 6 );
				if( !j )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 2, 50, 0 );
				else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[7], 1, 30, 0 );
				l_add( ST_nzei[j], ST_nzei[j], WORK0 );

				// 税込み
				memset( WORK0, '\0', 6 );
				l_add( ST_komi[j], ST_komi[j], recp->SZdtbl[i].SDval[3] );
				if( !SimpSw ) {	// 簡易計算
					if( !j ) {
						if( !H301sw )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[3], 4, 40, 0 );
						else			pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)recp->SZdtbl[i].SDval[4],(unsigned char *)PW0, 0, 0 );
					}
					else		memmove( WORK0, recp->SZdtbl[i].SDval[4], 6 );
				}
				else	{
					if( !j )	percent( WORK0, (char *)recp->SZdtbl[i].SDval[3], 4, 40, 0 );
					else		percent( WORK0, (char *)recp->SZdtbl[i].SDval[3], 0, 30, 0 );
				}
				l_add( ST_komz[j], ST_komz[j], WORK0 );

			}
		}

		Snd.Sn_tansw &= 0xfe;
		for( i = 0,j = 0,k = 0; i != 2; i++ ) {
			if( l_test( ST_nuki[i] ) )	j++;
			if( l_test( ST_komi[i] ) )	k++;
		}
		if( j || k ) {
			// 棚卸調整仕訳有り
			Snd.Sn_tansw |= 0x01;
			// 該当アリアへ転送
			if( j && k ) {
				// 税抜き・税込み混在は税込みとする
				for( i = 0; i != 2; i++ ) {
					l_add( ST_komi[i], ST_komi[i], ST_nuki[i] );
					l_add( ST_komi[i], ST_komi[i], ST_nzei[i] );
					l_add( ST_komz[i], ST_komz[i], ST_nukz[i] );
					memset( ST_nuki[i], '\0', 6 );
					memset( ST_nukz[i], '\0', 6 );
					memset( ST_nzei[i], '\0', 6 );
					j = 0;
				}
			}

			TBhead.SVsign &= 0xef;
			Snd.Sn_MENZsw = 0x10;
			if( k )	{	// 込み
				TBhead.SVsign |= 0x10;
				memset( WORK0, '\0', 6 );
				for( i = 0; i != 2; i++ ) {
					l_add( WORK0, WORK0, ST_komi[i] );
				}
				if( l_test( WORK0 ) < 0 )	{
					Snd.Sn_MENZsw |= 0x01;
					for( i = 0; i != 2; i++ ) {
						l_neg( ST_komi[i] );
						l_neg( ST_komz[i] );
					}
				}

				memmove( Snd.Sn_MNK40, ST_komi[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)
				memmove( Snd.Sn_MNZ40, ST_komz[0], 6 );	//				 〃	   消費税額   (〃      )
				memmove( Snd.Sn_MENZK, ST_komi[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)
				memmove( Snd.Sn_MENZZ, ST_komz[1], 6 );	// 				 〃	   消費税額   (〃      )
			}
			else	{	// 抜き
				memset( WORK0, '\0', 6 );
				for( i = 0; i != 2; i++ ) {
					l_add( WORK0, WORK0, ST_nuki[i] );
				}
				if( l_test( WORK0 ) < 0 )	{
					Snd.Sn_MENZsw |= 0x01;
					for( i = 0; i != 2; i++ ) {
						l_neg( ST_nuki[i] );
						l_neg( ST_nukz[i] );
					}
				}
				memmove( Snd.Sn_MNK40, ST_nuki[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)
				memmove( Snd.Sn_MNZ40, ST_nukz[0], 6 );	//				 〃	   消費税額   (〃      )
				memmove( Snd.Sn_MENZK, ST_nuki[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)
				memmove( Snd.Sn_MENZZ, ST_nukz[1], 6 );	// 				 〃	   消費税額   (〃      )
			}
			TBhead.SVzchg = Snd.Sn_MENZsw;	// [04'11.16]
		}
	}
}

// 集計レコ－ドクリア－
int CShinCalculateW::for_clear( struct TB_PAR *foret )
{
	register int	i;
	
	for( i = 0; i < foret->tp_rnum; i++ ) {
		if( pTblHdl->th_rget( &Asval, foret, i ) ) return( -1 );
		memset( Asval.komi, '\0', T_SIZE );
		Asval.t_sign = 0;
		if( pTblHdl->th_rput( &Asval, foret, i ) ) return( -1 );
	}
	
	return( FALSE );
}

// キィー設定
int CShinCalculateW::key_set( struct TB_PAR *foret, unsigned char sg1, unsigned char sg2,
											unsigned char sg3, unsigned char sg4 )
{
	memset( &Asval, '\0', _SVL );
	Asval.pj_kbn1  = sg1 & 0xff;
	Asval.pj_kbn2  = sg2 & 0xff;
	Asval.pj_zino  = sg3 & 0xff;
	Asval.pj_sjsg  = sg4 & 0xff;
	return( pTblHdl->th_read( 's', &Asval, foret ) );
}

// 消費税集計(集計業務)
int CShinCalculateW::stm_ucalq( int sno, int tno, char *karb, char *karu, char *karb3, char *karu3,
														struct TB_PAR *foreA, struct TB_PAR *foreB )
{
	int		i, i1, j, tc, sc, sw, uck, ct;
	unsigned	char	ksg, Furi, tksg;
	char	WORK0[6], WORK1[6], WORK2[6], WORK3[6], WORK4[6], WORK5[6],  WORK6[6], WORK7[6], 
			PW0[6], PW1[6], PW3[6], PW4[6], PW5[6], PW6[6], KU[6], KB[6]; 

	// 経過月計算
	Kazmon = tc = tno + 1;
	sc = sno;
	if( Kazmon > 12 )	Kazmon = 12;
	Kazmon = Kazmon - sc;
	if( ! Kazmon )	Kazmon = 1;

	memset( s_ymd, '\0', sizeof( s_ymd ) );
	memset( e_ymd, '\0', sizeof( e_ymd ) );
	memmove( s_ymd, Vol1.SS_YMD, 3 );
	shin_datecnv( Vol1.SS_YMD[0], (unsigned char *)s_ymd, 0 );
	memmove( e_ymd, Vol1.EE_YMD, 3 );
	shin_datecnv( Vol1.EE_YMD[0], (unsigned char *)e_ymd, 0 );
			
	// 消費税マスタ－　金額集計
	l_defn( 0x16 );
	memset( &stval, '\0', sizeof( struct _STUVAL ) );
	kani1 = kensg = 0;						//	簡易課税適用範囲ＳＷ
	toku = 0x00;							// 	特例ＳＷ
	Kenei &= 0x01;							//  兼営事業選択ＳＷクリア
	ksg = 0x00;								//  兼営事業　残高有無ＳＷ
	Furi = 0x00;							//	振替仕訳チェック

	memset( Kenuri, '\0', 6 );				//	事業別課税売上高合計
	memset( Kenuri_n, '\0', 6 );			//	事業別課税売上高合計(新税率分)

	// 輸入仕入計算
	memset( WORK0, '\0', 6 );
	if( (TBhead.SVvers & 0xf0) >= 0x10 ) {
		for( j = sc ; j < tc ; ++j ) {	// 輸入仕入集計
			l_add( stval.IpStkz_5, stval.IpStkz_5, SVsum.SSustb[j].SSval5 );
			l_add( stval.IpStze_5, stval.IpStze_5, SVsum.SSustb[j].SSzei5 );
			l_add( stval.IpStze_4, stval.IpStze_4, SVsum.SSustb[j].SSzei4 );
			l_add( stval.IpStkz_3, stval.IpStkz_3, SVsum.SSustb[j].SSval3 );
			l_add( stval.IpStze_3, stval.IpStze_3, SVsum.SSustb[j].SSzei3 );
			memmove( stval.IpStz0_3, stval.IpStze_3, 6 );
		}
	}

//////////////////
// 集計レコード //
//////////////////

#ifdef	VERSIONUP
	// 特定収入計算表
	if( m_SpcSg ) {
		SPCLQ	SPrec;
		for( i = 0 ; i < SpTbl.tp_rnum; ++i ) {
			if( pTblHdl->th_rget( &SPrec, &SpTbl, i ) ) return( -1 );
			for( j = sc ; j < tc ; ++j ) {
				// 特定収入（課税売上に係る）
				l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_tkzn[j] );
				// 〃　　　（非課税に係る）
				l_add( stval.SpHsyu, stval.SpHsyu, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				l_add( stval.SpKsyu, stval.SpKsyu, &SPrec.Sp_tczn[j] );
				//	不特定収入
				l_add( stval.SpFsyu, stval.SpFsyu, &SPrec.Sp_tszn[j] );
			}
		}
		l_neg( stval.SpTsyu );
		l_neg( stval.SpKsyu );
		l_neg( stval.SpFsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpTsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpKsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpFsyu );
		l_neg( stval.SpHsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpHsyu );
	}
#endif

	for( i = 0 ; i < foreA->tp_rnum; ++i ) {
		if( pTblHdl->th_rget( &Asval, foreA, i ) ) return( -1 );
		memset( &Asval.komi, '\0', T_SIZE );
		
		// 月途中での税率切替テーブルオフセット
		i1 = (int) (Asval.pj_scar & 0x0f);	i1--;
		
		for( j = tc - 1, sw = 0; j >= sc ; --j ) {
			l_add( Asval.hika, Asval.hika, Asval.h_mnth[j].hika_val );
			l_add( Asval.hika, Asval.hika, Asval.h3_mnth[j].hika3_val );
			
			if( Asval.pj_zino == 1 && i1 >= j ) {
				l_add( Asval.koms, Asval.koms, Asval.h_mnth[j].komi_val );
				l_add( Asval.nuks, Asval.nuks, Asval.h_mnth[j].nuki_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuks,Asval.knuks,Asval.h_mnth[j].komi_val );
			}
			else {
				l_add( Asval.komi, Asval.komi, Asval.h_mnth[j].komi_val );
				l_add( Asval.nuki, Asval.nuki, Asval.h_mnth[j].nuki_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuki,Asval.knuki,Asval.h_mnth[j].komi_val );
				
				// 旧税率集計
				l_add( Asval.komi3, Asval.komi3, Asval.h3_mnth[j].komi3_val );
				l_add( Asval.nuki3, Asval.nuki3, Asval.h3_mnth[j].nuki3_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuki3,Asval.knuki3,
										Asval.h3_mnth[j].komi3_val);
			}
			if( (Asval.pj_scar & 0x10) && i1 == j ) {
				l_add( Asval.nuki3, Asval.nuki3, Asval.nuks_val );
				l_add( Asval.komi3, Asval.komi3, Asval.koms_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuki3, Asval.knuki3, Asval.koms_val );
			}
			// 税額積み上げ
			l_add( Asval.komz, Asval.komz, Asval.h_mnth[j].komz_val );
			l_add( Asval.nukz, Asval.nukz, Asval.h_mnth[j].nukz_val );
			l_add( Asval.yuka, Asval.yuka, Asval.h_mnth[j].yuka_val );
			if( !sw )		// 一括税抜まで集計
					l_add( Asval.knkz, Asval.knkz, Asval.h_mnth[j].komz_val );
			l_add( Asval.komz3, Asval.komz3, Asval.h3_mnth[j].komz3_val );
			l_add( Asval.nukz3, Asval.nukz3, Asval.h3_mnth[j].nukz3_val );
			l_add( Asval.yuka3, Asval.yuka3, Asval.h3_mnth[j].yuka3_val );
			if( !sw )		// 一括税抜まで集計
					l_add( Asval.knkz3, Asval.knkz3, Asval.h3_mnth[j].komz3_val );
		}
		if( Asval.pj_ksign || Asval.pj_sjsg ) {
			l_neg( Asval.hika );
			l_neg( Asval.komi );
			l_neg( Asval.nuki );
			l_neg( Asval.knuki );
			l_neg( Asval.koms );
			l_neg( Asval.nuks );
			l_neg( Asval.knuks );
			l_neg( Asval.komi3 );
			l_neg( Asval.nuki3 );
			l_neg( Asval.knuki3 );
			l_neg( Asval.komz );
			l_neg( Asval.nukz );
			l_neg( Asval.knkz );
			l_neg( Asval.yuka );
			l_neg( Asval.komz3 );
			l_neg( Asval.nukz3 );
			l_neg( Asval.knkz3 );
			l_neg( Asval.yuka3 );
		}
		if( pTblHdl->th_rput( &Asval, foreA, i ) ) return( -1 );
	}
	
	if( (Ver&0x0f) == 2 && TBhead.SV_5sw )	uck = 1;
	else									uck = 0;
	memset( PW0, '\0', 6 );	l_input( PW0, "400" );
	memset( PW1, '\0', 6 );	l_input( PW1, "500" );
	for( i = 0 ; i < foreB->tp_rnum ; ++i ) {
		
		if( pTblHdl->th_rget( &Bsval, foreB, i ) ) return( -1 );
		memset( Bsval.su_nuki, '\0', 6 * 3 );
		i1 = (int) (Bsval.su_ofs & 0x0f);	i1--;
		for( j = sc ; j < tc ; ++j ) {	// 課税売上割合用集計
			memset( WORK0, '\0', 6 );
			
			if( TBhead.SVvers == 0 ) {
				if( (i > 6) && (i1 >= j) )	// 旧マスター売上業種区分（車）
					l_add( Bsval.su_nuks, Bsval.su_nuks, Bsval.su_val[j] );
				else
					l_add( Bsval.su_nuki3, Bsval.su_nuki3, Bsval.su3_val[j] );
			}
			else	{
				// 5%
				memmove( WORK0, Bsval.su_val[j], 6 );
				// 3%
				l_add( Bsval.su_nuki3, Bsval.su_nuki3, Bsval.su3_val[j] );
			}
			
			if( (Bsval.su_ofs & 0x10) && j == i1 )
					l_add( Bsval.su_nuki3, Bsval.su_nuki3, Bsval.su_val2 );
			if( (i < 3) && l_test( WORK0 ) ) {
				if( ! Bsval.su_per[j] ) { 	// 5% × 40/50
					pArithEx->l_mul100( (unsigned char *)WORK1, (unsigned char *)WORK0, (unsigned char *)PW0, 0, 0 );
					pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)WORK1, (unsigned char *)PW1, 0, 5 );
				}
			}
			l_add( Bsval.su_nuki, Bsval.su_nuki, WORK0 );
		}
		if( i > 2 ) {
			if( TBhead.SVvers ) {
				if( (i < 8) || (i > 10) ) {
					l_neg( Bsval.su_nuki );
					l_neg( Bsval.su_nuki3 );
					l_neg( Bsval.su_nuks );
				}
			}
			else	{
				l_neg( Bsval.su_nuki );
				l_neg( Bsval.su_nuki3 );
				l_neg( Bsval.su_nuks );
			}
		}
		
		if( uck && (i == 7 || i == 15) ) {		// 第五種事業の経過措置
			memset( SBval.su_nuki, '\0', 6 * 3 );
			if( pTblHdl->th_rget( &SBval, foreB, i-1 ) ) return( -1 );
			
			l_add( SBval.su_nuki,  SBval.su_nuki,  Bsval.su_nuki );
			l_add( SBval.su_nuki3, SBval.su_nuki3, Bsval.su_nuki3 );
			l_add( SBval.su_nuks,  SBval.su_nuks,  Bsval.su_nuks );
			
			if( pTblHdl->th_rput( &SBval, foreB, i-1 ) ) return( -1 );
			
			memset( Bsval.su_nuki, '\0', 6 * 3 );
		}	
		if( pTblHdl->th_rput( &Bsval, foreB, i ) ) return( -1 );
	}
	// 簡易課税
	if( Kani > 1 )	{
		memset( PW0, '\0', 6 );	l_input( PW0, "1050" );
		memset( PW1, '\0', 6 );	l_input( PW1, "1030" );
		for( i = 0 ; i < foreB->tp_rnum ; ++i ) {		
			if( pTblHdl->th_rget( &Bsval, foreB, i ) ) return( -1 );
			memset( WORK0, '\0', 6 );	// ５％込み
			memset( WORK1, '\0', 6 );	// ５％抜き（別記）
			memset( WORK2, '\0', 6 );	// ３％込み
			memset( WORK3, '\0', 6 );	// ３％抜き（別記）
			memset( WORK4, '\0', 6 );	// ５％　税額
			memset( WORK5, '\0', 6 );	// ３％　税額
			memset( WORK6, '\0', 6 );	// ５％抜き
			memset( WORK7, '\0', 6 );	// ５％抜き
			for( j = sc; j < tc; ++j ) {
				if( i < 8 ) {
					l_add( WORK0, WORK0, Bsval.su_kvl[j] );
					l_add( WORK1, WORK1, Bsval.su_kbk[j] );
					l_add( WORK4, WORK4, Bsval.su_zei[j] );
					l_add( WORK6, WORK6, Bsval.su_nvl[j] );
				}
				else	{
					l_add( WORK2, WORK2, Bsval.su_kvl3[j] );
					l_add( WORK3, WORK3, Bsval.su_kbk3[j] );
					l_add( WORK5, WORK5, Bsval.su_zei3[j] );
					l_add( WORK7, WORK7, Bsval.su_nvl3[j] );
				}
			}
			// 
			l_neg( WORK0 );
			l_neg( WORK1 );
			l_neg( WORK2 );
			l_neg( WORK3 );
			l_neg( WORK4 );
			l_neg( WORK5 );
			l_neg( WORK6 );
			l_neg( WORK7 );	
			memset( Bsval.su_nuki, '\0', 6 * 3 );
			memset( Bsval.su_zi5, '\0', 6 * 2 );
			// 積上げ方式 [07'04.16]
			if( K221sw ) {
				// 本体
				l_add( Bsval.su_nuki,  WORK1, WORK6 );
				l_add( Bsval.su_nuki3, WORK3, WORK7 );

				// 税額
				l_add( Bsval.su_zi5, Bsval.su_zi5, WORK4 );
				l_add( Bsval.su_zi3, Bsval.su_zi3, WORK5 );
			}
			else	{
				pArithEx->l_par100( (unsigned char *)Bsval.su_nuki, (unsigned char *)WORK0,(unsigned char *)PW0, 1, 0 );
				l_add( Bsval.su_nuki, Bsval.su_nuki, WORK1 );
				pArithEx->l_par100( (unsigned char *)Bsval.su_nuki3, (unsigned char *)WORK2,(unsigned char *)PW1, 1, 0 );
				l_add( Bsval.su_nuki3, Bsval.su_nuki3, WORK3 );
			}
			if( pTblHdl->th_rput( &Bsval, foreB, i ) ) return( -1 );
		}
	}
	// 売上割合　金額集計
	// 営業収益 経費売上も含む[06'11.06]
	// 全売上にかかわる科目の込金額からまとめて税額をぬく様変更
	//		A.   課税売上高	:	C.   非課税売上高	:	課税商品売上高
	memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
	memset( WORK2, '\0', 6 );	memset( WORK3, '\0', 6 );
	memset( WORK5, '\0', 6 );
	if( ! key_set( foreA, 0x02, 0, 0, 0 ) ) {		// 売上高セット
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// C.
	}	
	//		A.   C.		＋	B. （輸出売上高） 税抜き ＋ 税込み
	if( ! key_set( foreA, 0x02, 0, 0x02, 0 ) ) {	// 輸出売上高
		l_add( stval.exptur, Asval.nuki,   Asval.komi );		// A.
		l_add( stval.exptur, stval.exptur, Asval.nuki3 );
		l_add( stval.exptur, stval.exptur, Asval.komi3 );		// A.
		l_add( stval.hikazu, stval.hikazu, Asval.hika );		// C.
	}
	
	if( ! key_set( foreA, 0x11, 0x02, 0x02, 0x01 ) ) {	// 譲渡輸出売上高
		// 課税取引は免税売上額に計上
		l_add( stval.exptur, stval.exptur, Asval.nuki );			// A.
		l_add( stval.exptur, stval.exptur, Asval.komi );			// A.
		l_add( stval.exptur, stval.exptur, Asval.nuki3 );
		l_add( stval.exptur, stval.exptur, Asval.komi3 );
		// その他営業外収入（非課税分）に集計
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
	
	// 非課税輸出売上 [06'11.06]
	if( ! key_set( foreA, 0x02, 0x08, 0x00, 0x00 ) ) {
		// 非課税売上げは非課税資産の輸出等の金額に計上される
		l_add( stval.expjyo, stval.expjyo, Asval.hika );
	}

	//	営業外収益
	
	//	課税分	
	//		E.   土地以外の固定資産譲渡収入	:	課税商品売上高
	if( ! key_set( foreA, 0x11, 0x02, 0, 0x01 ) ) {		// 固定資産譲渡
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		// 固定資産譲渡収入　（非課税分）
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
	
	//		F.   その他営業外収入	: 	K.   その他営業外収入（非課税分）
	if( ! key_set( foreA, 0x02, 0x02, 0, 0 ) ) {		// 営業利益
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
	//		＋　特別利益 
	if( ! key_set( foreA, 0x02, 0x03, 0, 0 ) ) {		// 特別利益
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計 
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		// 非課税分
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
	//		F. その他営業外収入	＋流動負債譲渡
	if( ! key_set( foreA, 0x01, 0x01, 0, 0 ) ) {		// 流動負債譲渡
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		// 非課税分
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
		
	//	非課税分
	//		H.   受取利息
	if( (Vol1.APNO&0xff) == 0x10 ) {	// 個人:受取利息
		if( ! key_set( foreA, 0x01, 0x05, 0, 0 ) ) {
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}
	}
	else {
		if( ! key_set( foreA, 0x02, 0x04, 0, 0 ) ) {		// 受取利息
			l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
			l_add( stval.komiur, stval.komiur, Asval.knuki );
			l_add( stval.kkomur, stval.kkomur, Asval.komi );
			
			// 旧税率集計
			l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
			l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
			l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
			
			// 非課税分
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}
	}
	if( Vol1.APNO == 0x52 || Vol1.APNO == 0x53 || Vol1.APNO == 0x59 )	{	// 社会福祉(52)・新社会福祉(53)・宗教法人(59)
		//		I.   土地等譲渡収入
		if( ! key_set( foreA, 0x02, 0x0C, 0, 0x00 ) ) {
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}	
		//		J.   有価証券譲渡収入の　５％
		if( ! key_set( foreA, 0x02, 0x0B, 0, 0x00 ) ) {
			memset( WORK0, '\0', sizeof( WORK0 ) );
			l_add( WORK0, Asval.yuka, Asval.yuka3 );
			l_add( stval.hikazu, stval.hikazu, WORK0 );
#ifdef	VERSIONUP
			// 特定収入集計
			if( m_SpcSg ) {
				memmove( stval.SpYuka, Asval.hika, 6 );		// 全額
				memmove( stval.SpYuk5, WORK0, 6 );			// ５％
			}
#endif
		}
	}
	else	{
		//		I.   土地等譲渡収入
		if( ! key_set( foreA, 0x11, 0x04, 0, 0x01 ) ) {
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}
		
		//		J.   有価証券譲渡収入の　５％
		if( ! key_set( foreA, 0x11, 0x03, 0, 0x01 ) ) {
			memset( WORK0, '\0', sizeof( WORK0 ) );
			l_add( WORK0, Asval.yuka, Asval.yuka3 );
			l_add( stval.hikazu, stval.hikazu, WORK0 );

#ifdef	VERSIONUP
			// 特定収入集計
			if( m_SpcSg ) {
				memmove( stval.SpYuka, Asval.hika, 6 );		// 全額
				memmove( stval.SpYuk5, WORK0, 6 );			// ５％
			}
#endif

		}
	}
	// 消費税額計算書出力用額集計
	// 税抜き処理　金額集計
	// 仮払・仮受消費税の発生チェック
	if( (m_Ikat&0xc0) && (TBhead.SVmctl[tno]&0x80) )	{
		Furi |= 0x80;	// 一括税抜き処理あり
		memset( KU, '\0', 6 );
		memset( KB, '\0', 6 );
		for( ct = 0;  ct <= tno; ct++ ) {
			l_add( KU, KU, ikkruke.SZdtbl[ct].SDval[0] );
			l_add( KB, KB, ikkrbri.SZdtbl[ct].SDval[0] );
		}
		if( ! l_test( KU ) )	Furi |= 0x01;
		if( ! l_test( KB ) )	Furi |= 0x10;
	}
	else	{
		if( ! l_test( karu ) )	Furi |= 0x01;
		if( ! l_test( karb ) )	Furi |= 0x10;
	}
	
	// 仮受消費税（科目残）
	if( Ver ) {
		memmove( stval.kriuke_3, karu3, 6 );
		l_sub( stval.kriuke, karu, stval.kriuke_3 );
								}
	else	memmove( stval.kriuke_3, karu, 6 );
	
	// 仮払消費税（科目残）
	if( Ver ) {
		memmove( stval.kribar_3, karb3, 6 );
		l_sub( stval.kribar, karb, stval.kribar_3 );
	}
	else	memmove( stval.kribar_3, karb, 6 );

	// 貸倒損失税額
	memset( WORK5, '\0', 6 );
	// 貸倒損失 
	if( ! key_set( foreA, 0x08, 0x03, 0, 0 ) ) {
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memmove( stval.ksidao, Asval.nukz, 6 );			// 税抜税額
		memmove( WORK0, Asval.komz, 6 );				// 税抜税額
		memmove( WORK1, Asval.knkz, 6 );				// 税抜税額
		l_sub( WORK0, WORK0, WORK1 );
		l_add( stval.ksidao, stval.ksidao, WORK0 );
		
		// 科目残　仮受消費税＋貸倒損失分
		l_add( stval.kriuke, stval.kriuke, stval.ksidao );
		
		// 税込み貸倒損失
		l_add( stval.komksi, stval.ksidao, Asval.nuki );
		l_add( stval.komksi, stval.komksi, Asval.komi );
		l_sub( stval.komksi, stval.komksi, WORK0 );
		
		// 積み上げ時の税額　仮受消費税＋込み税額
		l_add( stval.ksidao, stval.ksidao, WORK1 );
		
		l_add( WORK5, WORK5, Asval.nuki );		// 税抜き取引発生チェック
		
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memmove( stval.ksidao_3, Asval.nukz3, 6 );		// 税抜税額
		memmove( WORK0, Asval.komz3, 6 );				// 税抜税額
		memmove( WORK1, Asval.knkz3, 6 );				// 税抜税額

		l_sub( WORK0, WORK0, WORK1 );
		l_add( stval.ksidao_3, stval.ksidao_3, WORK0 );
		
		// 科目残　仮受消費税＋貸倒損失分
		l_add( stval.kriuke_3, stval.kriuke_3, stval.ksidao_3 );
		
		// 税込み貸倒損失
		l_add( stval.komksi_3, stval.ksidao_3, Asval.nuki3 );
		l_add( stval.komksi_3, stval.komksi_3, Asval.komi3 );
		l_sub( stval.komksi_3, stval.komksi_3, WORK0 );
		
		// 積み上げ時の税額　仮受消費税＋込み税額
		l_add( stval.ksidao_3, stval.ksidao_3, WORK1 );
		
		l_add( WORK5, WORK5, Asval.nuki3 );		// 税抜き取引発生チェック
	}
	
	// 売上等の対価の返還額
	memset( WORK4, '\0', 6 );	// 税込み額ＳＡＶＥ用
	// N.   課税　  返品・値引		:	O.   非課税　返品・値引
	if( ! key_set( foreA, 0x02, 0x01, 0, 0 ) ) {		// 売上値引き
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memmove( stval.henkan, Asval.nukz, 6 );			// 税抜税額
		memmove( WORK0, Asval.komz, 6 );				// 税抜税額
		memmove( WORK1, Asval.knkz, 6 );				// 税抜税額

		l_sub( WORK0, WORK0, WORK1 );
		l_add( stval.henkan, stval.henkan, WORK0 );
		
		// 科目残　仮受消費税＋売上値引分
		l_add( stval.kriuke, stval.kriuke, stval.henkan );
		
		// 税込み売上値引
		l_add( stval.knebik, stval.henkan, Asval.nuki );
		l_add( stval.knebik, stval.knebik, Asval.komi );
		l_sub( stval.knebik, stval.knebik, WORK0 );
		
		// 積み上げ時の税額　仮受消費税＋込み税額
		l_add( stval.henkan, stval.henkan, WORK1 );
		
		l_add( WORK5, WORK5, Asval.nuki );		// 税抜き取引発生チェック
		l_add( WORK4, WORK4, Asval.komi );		// 税込取引発生チェック
		
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memmove( stval.henkan_3, Asval.nukz3, 6 );		// 税抜税額
		memmove( WORK0, Asval.komz3, 6 );				// 税抜税額
		memmove( WORK1, Asval.knkz3, 6 );				// 税抜税額

		l_sub( WORK0, WORK0, WORK1 );
		l_add( stval.henkan_3, stval.henkan_3, WORK0 );
		
		// 科目残　仮受消費税＋売上値引分
		l_add( stval.kriuke_3, stval.kriuke_3, stval.henkan_3 );
		
		// 税込み売上値引
		l_add( stval.knebik_3, stval.henkan_3, Asval.nuki3 );
		l_add( stval.knebik_3, stval.knebik_3, Asval.komi3 );
		l_sub( stval.knebik_3, stval.knebik_3, WORK0 );
		
		// 積み上げ時の税額　仮受消費税＋込み税額
		l_add( stval.henkan_3, stval.henkan_3, WORK1 );
		
		l_add( WORK5, WORK5, Asval.nuki3 );		// 税抜き取引発生チェック
		l_add( WORK4, WORK4, Asval.komi3 );		// 税込取引発生チェック
		
		//	非課税分
		memmove( stval.hnebik, Asval.hika, 6 );			// O.
	}


	// 貸倒回収に係る税額
	
	// 貸倒回収
	if( ! key_set( foreA, 0x02, 0x05, 0, 0 ) ) {
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memmove( stval.kaishu, Asval.nukz, 6 );			// 税抜税額
		memmove( WORK0, Asval.komz, 6 );				// 税抜税額
		memmove( WORK1, Asval.knkz, 6 );				// 税抜税額

		l_sub( WORK0, WORK0, WORK1 );
		l_add( stval.kaishu, stval.kaishu, WORK0 );
		
		// 科目残　仮受消費税－貸倒回収分
		l_sub( stval.kriuke, stval.kriuke, stval.kaishu );
		
		// 税込み貸倒回収
		l_add( stval.komksy, stval.kaishu, Asval.nuki );
		l_add( stval.komksy, stval.komksy, Asval.komi );
		l_sub( stval.komksy, stval.komksy, WORK0 );
		
		// 積み上げ時の税額　仮受消費税＋込み税額
		l_add( stval.kaishu, stval.kaishu, WORK1 );
		
		l_add( WORK5, WORK5, Asval.nuki );		// 税抜き取引発生チェック
		
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memmove( stval.kaishu_3, Asval.nukz3, 6 );		// 税抜税額
		memmove( WORK0, Asval.komz3, 6 );				// 税抜税額
		memmove( WORK1, Asval.knkz3, 6 );				// 税抜税額

		l_sub( WORK0, WORK0, WORK1 );
		l_add( stval.kaishu_3, stval.kaishu_3, WORK0 );
		
		// 科目残　仮受消費税－貸倒回収分
		l_sub( stval.kriuke_3, stval.kriuke_3, stval.kaishu_3 );
		
		// 税込み貸倒損失
		l_add( stval.komksy_3, stval.kaishu_3, Asval.nuki3 );
		l_add( stval.komksy_3, stval.komksy_3, Asval.komi3 );
		l_sub( stval.komksy_3, stval.komksy_3, WORK0 );
		
		// 積み上げ時の税額　仮受消費税＋込み税額
		l_add( stval.kaishu_3, stval.kaishu_3, WORK1 );
		
		l_add( WORK5, WORK5, Asval.nuki3 );		// 税抜き取引発生チェック
	}
	
	// 税抜き取引の発生チェック
	if( l_test( stval.kazeiu ) || l_test( stval.kazeiu_3 ) || l_test( WORK5 ) )
		Furi |= 0x02;
	
	// 税込み取引発生チェック
	if( l_test( stval.kkomur ) || l_test( stval.kkomur_3 ) || l_test( WORK4 ) )
		Furi |= 0x04;
	
	// 税抜きのみの発生で且積上げ方式の場合は、税抜き額合計が課税標準

	if( K221sw )	{	// 積み上げ方式

		if( !l_test( WORK4 ) && !l_test( stval.kkomur ) && !l_test( stval.kkomur_3 ) ) {
		// 税抜きのみ
			// 税抜き課税　返品・値引
			if( ! key_set( foreA, 0x02, 0x01, 0, 0 ) ) {		// 売上値引き
				memmove( stval.knebik,   Asval.nuki,  6 );	// （旧税率分）
				memmove( stval.knebik_3, Asval.nuki3, 6 );	// （新税率分）
			}
		}
		else	{
			// 税抜き・税込み混在
			memset( PW0, '\0', 6 );		l_input( PW0, "1030" );
			memset( PW1, '\0', 6 );		l_input( PW1, "1050" );

			//	（旧税率）込み税額　＝　３０／１０３０
			percent( (char *)stval.komusz_3, (char *)stval.komiur_3, 0, 30, 0 );
			//	（旧税率）課税売上（税抜き）　＝　税込み課税売上Ｘ１００／１０３０
			memset( WORK0, '\0', 6 );
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)stval.kkomur_3,(unsigned char *)PW0, 1, 0 );
			l_add( stval.kazeiu_3, stval.kazeiu_3, WORK0 );

			//	（新税率）込み税額　＝　４０／１０５０
			percent( (char *)stval.komusz, (char *)stval.komiur, 4, 40, 0 );
			//	（新税率）課税売上（税抜き）　＝　税込み課税売上Ｘ１００／１０５０
			memset( WORK0, '\0', 6 );
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)stval.kkomur, (unsigned char *)PW1, 1, 0 );
			l_add( stval.kazeiu, stval.kazeiu, WORK0 );

			// 売上値引き・返品	
			if( ! key_set( foreA, 0x02, 0x01, 0, 0 ) ) {		// 売上値引き

				//	（旧税率）税抜き売上値引き＝税込み売上値引きＸ１００／１０３
					memset( WORK0, '\0', 6 );
				memset( stval.knebik_3, '\0', 6 );
				pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)Asval.komi3,(unsigned char *)PW0, 1, 0 );
				l_add( stval.knebik_3, stval.knebik_3, Asval.nuki3 );
				l_add( stval.knebik_3, stval.knebik_3, WORK0 );

				//	（新税率）税抜き売上値引き＝税込み売上値引きＸ１００／１０５
				memset( WORK0, '\0', 6 );
				memset( stval.knebik, '\0', 6 );
				pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)Asval.komi,(unsigned char *)PW1, 1, 0 );
				l_add( stval.knebik, stval.knebik, Asval.nuki );
				l_add( stval.knebik, stval.knebik, WORK0 );
			}
		}
	}
	else {
		//（旧税率分）　込税額　＝　３０ ／ １０３０
		percent( (char *)stval.komusz_3, (char *)stval.komiur_3, 0, 30, 0 );
		percent( (char *)WORK0, 		 (char *)stval.kkomur_3, 0, 30, 0 );	// 全体の込金額
		l_sub( WORK0, WORK0, stval.komusz_3 );
		//（旧税率分）　　課税売上＝込＋（抜き＋仮受消費税）
		l_add( stval.komiur_3, stval.kkomur_3, stval.kriuke_3 );
		l_add( stval.komiur_3, stval.komiur_3, stval.kazeiu_3 );
		l_sub( stval.komiur_3, stval.komiur_3, WORK0 );
		
		//（新税率分）　込税額　＝　４０ ／ １０５０
		percent( (char *)stval.komusz, (char *)stval.komiur, 4, 40, 0 );
		percent( (char *)WORK1, (char *)stval.komiur, 0, 0, 0 );
		percent( (char *)WORK0, (char *)stval.kkomur, 0, 0, 0 );	// 全体の込金額

		l_sub( WORK0, WORK0, WORK1 );
		//（新税率分）　　課税売上＝込＋（抜き＋仮受消費税）
		l_add( stval.komiur, stval.kkomur, stval.kriuke );
		l_add( stval.komiur, stval.komiur, stval.kazeiu );
		l_sub( stval.komiur, stval.komiur, WORK0 );
		
//★//[12'06.26]///
		memset( m_KzuValKm, '\0', 6 );
		l_add( m_KzuValKm,  m_KzuValKm, stval.komiur );
		l_sub( m_KzuValKm,  m_KzuValKm, stval.knebik );
		l_add( m_KzuValKm,  m_KzuValKm, stval.komiur_3 );
		l_sub( m_KzuValKm,  m_KzuValKm, stval.knebik_3 );
#ifdef	_DEBUG_EX
char	wkbf[128];
l_print( wkbf, m_KzuValKm, "SSS,SSS,SSS,SS9" );
TRACE( "\nm_KzuValKm=[%s]", wkbf );
l_print( wkbf, stval.komiur, "SSS,SSS,SSS,SS9" );
TRACE( "\nkomiur=     [%s]", wkbf );
l_print( wkbf, stval.knebik, "SSS,SSS,SSS,SS9" );
TRACE( "\nknebik=    [%s]", wkbf );
l_print( wkbf, stval.komiur_3, "SSS,SSS,SSS,SS9" );
TRACE( "\nkomiur_3=  [%s]", wkbf );
l_print( wkbf, stval.knebik_3, "SSS,SSS,SSS,SS9" );
TRACE( "\nknebik_3=  [%s]", wkbf );
TRACE("\n");
#endif
///////////////////

		//（旧税率分）　課税標準＝税込み課税売上×１００／１０３
		memset( PW0, '\0', 6 );	l_input( PW0, "1030" );
		memset( stval.kazeiu_3, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.kazeiu_3, (unsigned char *)stval.komiur_3, (unsigned char *)PW0, 1, 0 );
		
		//（旧税率分）　税抜き売上値引＝税込み売上値引×１００／１０３
		memset( WORK0, '\0', 6 );
		memmove( WORK0, stval.knebik_3, 6 );
		memset( stval.knebik_3, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.knebik_3, (unsigned char *)WORK0, (unsigned char *)PW0, 1, 0 );
		
		//（新税率分）　課税標準＝税込み課税売上×１００／１０５
		memset( PW0, '\0', 6 );	l_input( PW0, "1050" );
		memset( stval.kazeiu, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.kazeiu, (unsigned char *)stval.komiur, (unsigned char *)PW0, 1, 0 );

		//（新税率分）　税抜き売上値引＝税込み売上値引×１００／１０５
		memset( WORK0, '\0', 6 );
		memmove( WORK0, stval.knebik, 6 );	memset( stval.knebik,'\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.knebik, (unsigned char *)WORK0, (unsigned char *)PW0, 1, 0 );

	}

	// 貸倒損失
	if( ! key_set( foreA, 0x08, 0x03, 0, 0 ) ) {
		//（旧税率分）　税抜き貸倒損失＝税込み貸倒損失×１００／１０３
		memset( WORK0, '\0', 6 );
		memset( PW0, '\0', 6 );	l_input( PW0, "1030" );
		memmove( WORK0, stval.komksi_3, 6 );
		memset( stval.komksi_3, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.komksi_3, (unsigned char *)WORK0, (unsigned char *)PW0, 1, 0 );
		
		//（新税率分）　税抜き貸倒損失＝税込み貸倒損失×１００／１０５
		memset( WORK0, '\0', 6 );
		memset( PW0, '\0', 6 );	l_input( PW0, "1050" );
		memmove( WORK0, stval.komksi, 6 );	memset( stval.komksi, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.komksi, (unsigned char *)WORK0, (unsigned char *)PW0, 1, 0 );
	}
	// 貸倒回収
	if( ! key_set( foreA, 0x02, 0x05, 0, 0 ) ) {
		//（旧税率分）　税抜き貸倒回収＝税込み貸倒回収×１００／１０３
		memset( WORK0, '\0', 6 );
		memset( PW0, '\0', 6 );	l_input( PW0, "1030" );
		memmove( WORK0, stval.komksy_3, 6 );
		memset( stval.komksy_3, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.komksy_3, (unsigned char *)WORK0, (unsigned char *)PW0, 1, 0 );
		
		//（新税率分）　税抜き貸倒回収＝税込み貸倒回収×１００／１０５
		memset( WORK0, '\0', 6 );
		memset( PW0, '\0', 6 );	l_input( PW0, "1050" );
		memmove( WORK0, stval.komksy, 6 );	memset( stval.komksy, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)stval.komksy, (unsigned char *)WORK0, (unsigned char *)PW0, 1, 0 );
	}

	// 課税売上高割合金額
	
	// Q. = A + G - N   分子
	l_add( stval.kwbuns, stval.kazeiu, stval.kazeiu_3 );
	l_add( stval.kwbuns, stval.kwbuns, stval.exptur );
	l_add( stval.kwbuns, stval.kwbuns, stval.expjyo );		// 非課税輸出売上　★[14'01.17]
	l_sub( stval.kwbuns, stval.kwbuns, stval.knebik );
	l_sub( stval.kwbuns, stval.kwbuns, stval.knebik_3 );
	
	// R. = D + M - P   分母
	l_add( stval.kwbunb, stval.kwbuns, stval.hikazu );
	l_sub( stval.kwbunb, stval.kwbunb, stval.hnebik );
	
	// 分子×１００００／分母
	// 課税売上高割合
	pArithEx->l_par100( (unsigned char *)stval.uwriai, (unsigned char *)stval.kwbuns,(unsigned char *)stval.kwbunb, 2, 0 );

	// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
	memset( PW6, '\0', sizeof( PW6 ) );
	if( l_cmp( stval.kwbuns, PW6 ) < 0 )	memset( stval.uwriai, '\0', 6 ); 
	if( l_cmp( stval.kwbunb, PW6 ) < 0 )	memset( stval.uwriai, '\0', 6 );

#ifdef	VERSIONUP
	// 特定収入集計
	if( m_SpcSg ) {
		memmove( stval.SpKuws, stval.kwbuns, 6 );			// 課税売上高割合（分子）
		memmove( stval.SpKuwb, stval.kwbunb, 6 );			// 〃　　　　　　（分母）
		memmove( stval.SpUwri, stval.uwriai, 6 );			// 課税売上高割合

		memmove( stval.SpUnb3, stval.knebik_3, 6 );			// 売上返還　３％
		memmove( stval.SpUnb4, stval.knebik, 6 );			// 〃　　　　４％
		l_add( stval.SpUnbg, stval.SpUnb3, stval.SpUnb4 );	// 〃　　　　合計

		l_add( stval.SpKzur, stval.kazeiu, stval.kazeiu_3 );// 課税売上（準売上）
		memmove( stval.SpMzur, stval.exptur, 6 );			// 免税売上

		memmove( stval.SpHkur, stval.hikazu, 6 );			// 非課税売上（有価証券を全額で含む）
		l_sub( stval.SpHkur, stval.SpHkur, stval.SpYuk5 );
		l_add( stval.SpHkur, stval.SpHkur, stval.SpYuka );
	}
#endif

	// 課税売上高の５億超えチェック
	memset( m_CnvVal, '\0', 6 );
	memset( m_KzuVal, '\0', 6 );
	l_add( m_KzuVal, stval.kazeiu, stval.exptur );
	int St = YearConvert( m_CnvVal, m_KzuVal );
	if( m_Check ) return( St );
	else	{
//★//[12'02.23]///
		if( m_Compare == 0 ) {
			if( St == 1 && m_TaxReforms )	{	// ９５％ルール改正対応期間で５億円超え
				memset( RatioAsc, '\0', 6 );
				strcpy_s( RatioAsc, sizeof( RatioAsc ), "11000" );
				RatioInt = 11000;
			}
			else	{			// ４億５千万以上で５億以下又は４億５千万未満
				memset( RatioAsc, '\0', 6 );
				strcpy_s( RatioAsc, sizeof( RatioAsc ), "9500" );
				RatioInt = 9500;
			}
		}
		else	{
			if( m_Calcusw ) {
				memset( RatioAsc, '\0', 6 );
				strcpy_s( RatioAsc, sizeof( RatioAsc ), "11000" );
				RatioInt = 11000;
			}
			else	{
				memset( RatioAsc, '\0', 6 );
				RatioInt = 0;
			}
		}
///////////////////
	}
	// 課税売上割合　0％チェック
	Snd.Sn_EXP100 = 0x00;

	l_input( PW0, "1" );
	if( l_cmp( stval.uwriai, PW0 ) >= 0 || Anbun ) ;
	else {
		// ０％で計算する
		memset( stval.uwriai, '\0', 6 );
	}

	// 税込み(仕入)処理　金額集計
	// 課税商品仕入高
	// 商品仕入高 ＋ 営業経費
	if( ! key_set( foreA, 0x04, 0, 0, 0 ) ) {				// 仕入高
		memmove( stval.komisi,   Asval.knuki,  6 );
		memmove( stval.nukisi,   Asval.nuki,   6 );
		memmove( stval.kkomsi,   Asval.komi,   6 );
		memmove( stval.komisi_3, Asval.knuki3, 6 );
		memmove( stval.nukisi_3, Asval.nuki3,  6 );
		memmove( stval.kkomsi_3, Asval.komi3,  6 );
	}
	
	if( ! key_set( foreA, 0x08, 0, 0, 0 ) )	{				// 経費
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	// ＋ 営業外費用 ＋ 特別損失
	if( ! key_set( foreA, 0x08, 0x01, 0, 0 ) ) {			// 営業費用
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	if( ! key_set( foreA, 0x08, 0x02, 0, 0 ) ) {			// 特別損失
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	// ＋ 固定資産取得
	if( ! key_set( foreA, 0x11, 0x02, 0, 0 ) ) {			// 固定資産取得
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	// ＋ 流動資産取得
	if( ! key_set( foreA, 0x01, 0x00, 0, 0 ) ) {			// 流動資産取得
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}

	// ＋ 投資有価証券　取得
	if( ! key_set( foreA, 0x11, 0x03, 0, 0 ) ) {
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	// ＋ 土地　取得
	if( ! key_set( foreA, 0x11, 0x04, 0, 0 ) ) {
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}

	if( Vol1.APNO == 0x52 || Vol1.APNO == 0x53 || Vol1.APNO == 0x59 )	{	// 社会福祉(52)・新社会福祉(53)・宗教法人(59)
		// ＋ 投資有価証券　取得
		if( ! key_set( foreA, 0x04, 0x08, 0, 0 ) ) {
			l_add( stval.komisi,   stval.komisi,   Asval.knuki );
			l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
			l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
			l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
			l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
			l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
		}
		// ＋ 土地　取得
		if( ! key_set( foreA, 0x04, 0x09, 0, 0 ) ) {
			l_add( stval.komisi,   stval.komisi,   Asval.knuki );
			l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
			l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
			l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
			l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
			l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
		}
	}

	// （新税率分）税込み対応消費税
	percent( (char *)stval.komssz, (char *)stval.komisi, 4, 40, 0 );
	percent( (char *)WORK1,        (char *)stval.komisi, 0,  0, 0 );
	percent( (char *)WORK0,        (char *)stval.kkomsi, 0,  0, 0 );
	l_sub( WORK0, WORK0, WORK1 );

	//（新税率分）税込み仕入＝対応消費税＋仮払消費税＋抜き仕入
	l_add( stval.komisi, stval.kkomsi, stval.nukisi );
	l_add( stval.komisi, stval.komisi, stval.kribar );
	l_sub( stval.komisi, stval.komisi, WORK0 );
	
	//（旧税率分）税込み対応消費税
	percent( (char *)stval.komssz_3, (char *)stval.komisi_3, 0, 30, 0 );
	percent( (char *)WORK0,          (char *)stval.kkomsi_3, 0, 30, 0 );
	l_sub( WORK0, WORK0, stval.komssz_3 );

	//（旧税率分）税込み仕入＝対応消費税＋仮払消費税＋抜き仕入
	l_add( stval.komisi_3, stval.kkomsi_3, stval.nukisi_3 );
	l_add( stval.komisi_3, stval.komisi_3, stval.kribar_3 );
	l_sub( stval.komisi_3, stval.komisi_3, WORK0 );

#ifdef	VERSIONUP
	// 特定収入集計
	if( m_SpcSg ) {
		memmove( stval.SpKsi3, stval.komisi_3, 6 );				// 旧税率分・税込み仕入
		memmove( stval.SpKsi4, stval.komisi,   6 );				// 新税率分・税込み仕入
		l_add( stval.SpKsig, stval.komisi_3, stval.komisi );	// 合計　　・税込み仕入	
	}
#endif

	// 商品仕入値引高
	memset( WORK1, '\0', 6 );	memset( WORK4, '\0', 6 );
	memset( WORK5, '\0', 6 );
	if( ! key_set( foreA, 0x04, 0x01, 0, 0 ) ) {		// 仕入値引き
		memset( WORK1, '\0', 6 );
		percent( (char *)stval.sihenk, (char *)Asval.knuki, 0, Asval.pj_zino, 0 );	// 50 / 1050
		percent( (char *)WORK1, 	   (char *)Asval.komi,  0, Asval.pj_zino, 0 );	// 50 / 1050
		l_sub( WORK1, WORK1, stval.sihenk );
		percent( (char *)stval.sihenk, (char *)Asval.knuki, 4, 40, 0 );				// 40 / 1050
		
		// 税込み仕入値引
		l_add( stval.sinebk, Asval.nuki,   Asval.komi );
		l_sub( stval.sinebk, stval.sinebk, WORK1 );
		
		l_add( WORK4, WORK4, Asval.nuki );	// 税抜き仕入値引
		l_add( WORK5, WORK5, Asval.komi );	// 税込み仕入値引

		if( !H301sw ) {
			l_add( stval.kribar, stval.kribar, Asval.nukz );
			l_add( stval.sinebk, stval.sinebk, Asval.nukz );
			l_add( stval.komisi, stval.komisi, Asval.nukz );
		}

		memset( WORK1,'\0', 6 );
		percent( (char *)stval.sihenk_3, (char *)Asval.knuki3, 0, 30, 0 );	// 30 / 1000
		percent( (char *)WORK1, (char *)Asval.komi3,  0, 30, 0 );			// 30 / 1000
		l_sub( WORK1, WORK1, stval.sihenk_3 );
		
		// 税込み仕入値引
		l_add( stval.sinebk_3, Asval.nuki3,    Asval.komi3 );
		l_sub( stval.sinebk_3, stval.sinebk_3, WORK1 );
		
		l_add( WORK4, WORK4, Asval.nuki3 );	// 税抜き仕入値引
		l_add( WORK5, WORK5, Asval.komi3 );	// 税込み仕入値引

		if( !H301sw ) {
			l_add( stval.kribar_3, stval.kribar_3, Asval.nukz3 );
			l_add( stval.sinebk_3, stval.sinebk_3, Asval.nukz3 );
			l_add( stval.komisi_3, stval.komisi_3, Asval.nukz3 );
		}
	}

#ifdef	VERSIONUP
	// 特定収入集計
	if( m_SpcSg ) {
		memmove( stval.SpKsb3, stval.sinebk_3, 6 );				// 旧税率分・税込み仕入値引
		memmove( stval.SpKsb4, stval.sinebk,   6 );				// 新税率分・税込み仕入値引
		l_add( stval.SpKsbg, stval.sinebk_3, stval.sinebk );	// 合計　　・税
	}
#endif

	// 税抜き取引の発生チェック
	if( l_test( stval.nukisi ) || l_test( stval.nukisi_3 ) || l_test( WORK4 ) )
		Furi |= 0x20;
	
	// 税込み取引発生チェック
	if( l_test( stval.kkomsi ) || l_test( stval.kkomsi_3 ) || l_test( WORK5 ) )
		Furi |= 0x40;


	// 振替仕訳チェック
	i1 = 0;
	char	val5[6], val3[6];
	l_input( val5, "20" );
	l_input( val3, "34" );

	if( Furi & 0x10 ) {		// 仮払受残高なし
		if( ((Furi&0x80) && (Furi&0x40)) ||	// 税込み取引有 且 一括税抜有
			 (Furi&0x20) ) {				// 税抜き取引有
			if( Kani < 2 )	{
				if( l_cmp( stval.nukisi, val5 ) > 0 )	i1++;
				if( l_cmp( stval.nukisi_3, val3 ) > 0 )	i1++;
				if( l_cmp( stval.kkomsi, val5 ) > 0 )	i1++;
				if( l_cmp( stval.kkomsi_3, val3 ) > 0 )	i1++;
			}
		}
	}
	if( Furi & 0x01 ) {		// 仮受残高無し
		if( ((Furi&0x80) && (Furi&0x04)) ||	// 税込み取引有 且 一括税抜有
			 (Furi&0x02) )	{
				if( l_cmp( stval.kazeiu, val5 ) > 0 )	i1++;	// 税抜き取引有
				if( l_cmp( stval.kazeiu_3, val3 ) > 0 )	i1++;	// 税抜き取引有
				if( l_cmp( stval.kkomur, val5 ) > 0 )	i1++;	// 税抜き取引有
				if( l_cmp( stval.kkomur_3, val3 ) > 0 )	i1++;	// 税抜き取引有
		}
	}

	if( i1 ) {
		char	mes[128];
		memset( mes, '\0', sizeof( mes ) );
		ErrMessage = _T("仮払及び仮受消費税等の残高が０円になっている為、\r\n申告処理ができません。" );
		return( ERR );
	}

	// 申告書形式計算

	// 課税標準額に対する消費税計算に積み上げ方式を採用する
	if( K221sw ) {
		memset( PW0, '\0', 6 );	l_input( PW0, "400" );
		memset( PW1, '\0', 6 );	l_input( PW1, "500" );
		
		// 旧税率 課税標準消費税額
		l_add( stval.urisyz_3, stval.kriuke_3, stval.komusz_3 );
		
		// 新税率 課税標準消費税額
		if( TBhead.SVvers ) {			// 5% × 40 / 50
			pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)stval.kriuke, (unsigned char *)PW0, 0, 0 );
			pArithEx->l_par100( (unsigned char *)stval.urisyz, (unsigned char *)WORK0, (unsigned char *)PW1, 0, 5 );
			pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)stval.henkan, (unsigned char *)PW0, 0, 0 );
			pArithEx->l_par100( (unsigned char *)stval.henkan, (unsigned char *)WORK0, (unsigned char *)PW1, 0, 5 );
			pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)stval.ksidao, (unsigned char *)PW0, 0, 0 );
			pArithEx->l_par100( (unsigned char *)stval.ksidao, (unsigned char *)WORK0, (unsigned char *)PW1, 0, 5 );
			pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)stval.kaishu, (unsigned char *)PW0, 0, 0 );
			pArithEx->l_par100( (unsigned char *)stval.kaishu, (unsigned char *)WORK0, (unsigned char *)PW1, 0, 5 );
		}
		else	memmove( stval.urisyz, stval.kriuke, 6 );
		l_add( stval.urisyz, stval.urisyz, stval.komusz );
	}
	else	{
		// 課税標準額
		l_calq( WORK1, stval.kazeiu,   0 );	// 1000円未満切り捨て
		l_calq( WORK0, stval.kazeiu_3, 0 );	// 1000円未満切り捨て
		
		// 課税標準に係る　消費税額
		percent( (char *)stval.urisyz,   (char *)WORK1, 1, 40, 0 );
		percent( (char *)stval.urisyz_3, (char *)WORK0, 1, 30, 0 );
		
		// 返還等対価に係る税額
		percent( (char *)stval.henkan,   (char *)stval.knebik,   1, 40, 0 );
		percent( (char *)stval.henkan_3, (char *)stval.knebik_3, 1, 30, 0 );
		
		// 貸倒損失に係る税額
		percent( (char *)stval.ksidao,   (char *)stval.komksi,   1, 40, 0 );
		percent( (char *)stval.ksidao_3, (char *)stval.komksi_3, 1, 30, 0 );
		
		// 貸倒回収に係る税額
		percent( (char *)stval.kaishu,   (char *)stval.komksy,   1, 40, 0 );
		percent( (char *)stval.kaishu_3, (char *)stval.komksy_3, 1, 30, 0 );
	}
	
	// 課税仕入に係る消費税計算に積み上げ方式を採用する
	if( H301sw ) {
		memset( PW0, '\0', 6 );	l_input( PW0, "400" );
		memset( PW1, '\0', 6 );	l_input( PW1, "500" );
		// 課税標準控除対象仕入税額 － 仕入値引
		l_add( stval.sirsyz_3, stval.kribar_3, stval.komssz_3 );
		l_sub( stval.sirsyz_3, stval.sirsyz_3, stval.sihenk_3 );
		if( TBhead.SVvers ) {			// 5% × 40 / 50
			pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)stval.kribar, (unsigned char *)PW0, 0, 0 );
			pArithEx->l_par100( (unsigned char *)stval.sirsyz, (unsigned char *)WORK0, (unsigned char *)PW1, 0, 5 );
		}
		else	memmove( stval.sirsyz, stval.kribar, 6 );
		l_add( stval.sirsyz, stval.sirsyz, stval.komssz );
		l_sub( stval.sirsyz, stval.sirsyz, stval.sihenk );
	}
	else	{
		// 税込み課税標準控除対象仕入税額////////
		percent( (char *)stval.sirsyz,   (char *)stval.komisi,   4, 40, 0 );
		percent( (char *)stval.sirsyz_3, (char *)stval.komisi_3, 0, 30, 0 );
		
		// 税込み対応消費税 － 仕入値引分
		percent( (char *)WORK1, (char *)stval.sinebk,   4, 40, 0 );
		l_sub( stval.sirsyz,   stval.sirsyz,   WORK1 );
		percent( (char *)WORK1, (char *)stval.sinebk_3, 0, 30, 0 );
		l_sub( stval.sirsyz_3, stval.sirsyz_3, WORK1 );
	}
	
	// 税込み仕入 －＝ 仕入値引
	l_sub( stval.komisi,   stval.komisi,   stval.sinebk );
	l_sub( stval.komisi_3, stval.komisi_3, stval.sinebk_3 );

	// 経過措置有無チェック
	if( Ver == 2 ) {
		if( Kani < 2 ) {		// 原則課税
			if( l_test( stval.kazeiu_3 ) || l_test( stval.urisyz_3 ) ||
				l_test( stval.knebik_3 ) || l_test( stval.komksi_3 ) ||
				l_test( stval.komisi_3 ) || l_test( stval.sirsyz_3 ) ||
				l_test( stval.komksy_3 ) || l_test( stval.IpStkz_3 ) ||
				l_test( stval.IpStze_3) )	TBhead.SVmzsw = 0x01;
			else							TBhead.SVmzsw = 0;
		}
		else {					// 簡易課税
			if( l_test( stval.kazeiu_3 ) || l_test( stval.urisyz_3 ) ||
				l_test( stval.knebik_3 ) || l_test( stval.komksy_3 ) ||
				l_test( stval.komksi_3 ) || l_test( stval.IpStkz_3 ) ||
				l_test( stval.IpStze_3) )	TBhead.SVmzsw = 0x01;
			else							TBhead.SVmzsw = 0;
		}
	}

//★//[12'01.19]///
	TBhead.SVmzsw = 0x01;
///////////////////

	// 課税売上高割合
	Kobet = 0;
	l_input( PW0, RatioAsc );
	memset( WORK0, '\0', 6 );
//★//[12'02.23]///
	int	m_Junction = 0;
	if( m_Compare == 0 )	{
		if( l_cmp( stval.uwriai, PW0 ) >= 0 )	m_Junction = 1;
	}
	else	{
		// シミュレーションする
		if(  m_Calcusw == 0 )					m_Junction = 1;
	}
	if( m_Junction )	{	// 課税売上割合９５％以上
///////////////////
		Srx = 0;
		// 控除対象仕入税額
		memmove( stval.koujo0,   stval.sirsyz, 6 );
		memmove( stval.koujo0_3, stval.sirsyz_3, 6 );
		strcpy_s( WORK0, sizeof( WORK0 ), "10000" );
	}
	else {									// 課税売上割合９５％以下
		Srx = 1;
		if( ! Kani && Anbun ) {		// 原則課税
			// 控除対象仕入税額 （個別対応方式）
			l_input( PW0, "10000" );	memset( WORK2, '\0', 6 );
			if( ! pTblHdl->th_rget( &Bsval, foreB, 0 ) )	{	// 課売に係る
				l_add( stval.kauriz,   Bsval.su_nuki,  Bsval.su_nuks );
				l_add( stval.kauriz_3, stval.kauriz_3, Bsval.su_nuki3 );
			}
			if( ! pTblHdl->th_rget( &Bsval, foreB, 2 ) ) {	// 共通に係る
				l_add( stval.kyuriz,   Bsval.su_nuki,  Bsval.su_nuks );
				l_add( stval.kyuriz_3, stval.kyuriz_3, Bsval.su_nuki3 );
			}
			
			// ３％
			if( ! pTblHdl->th_rget( &Bsval, foreB, 8 ) )		// 課売に係る
				l_add( stval.kauriz_3, stval.kauriz_3, Bsval.su_nuki3 );
			
			if( ! pTblHdl->th_rget( &Bsval, foreB, 10 ) )	// 共通に係る
				l_add( stval.kyuriz_3, stval.kyuriz_3, Bsval.su_nuki3 );
			
			// 共売に係る控除対象仕入税額x課税売上割合(%) 小数点以下第五位迄
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)stval.kwbuns, (unsigned char *)stval.kwbunb, 5, 0 );

			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof( PW6 ) );
			if( l_cmp( stval.kwbuns, PW6 ) < 0 )	memset( WORK0, '\0', 6 );
			if( l_cmp( stval.kwbunb, PW6 ) < 0 )	memset( WORK0, '\0', 6 );

			pArithEx->l_mul100( (unsigned char *)WORK1, (unsigned char *)stval.kyuriz, (unsigned char *)WORK0, 5, 0 );
			l_add( stval.koujo0, stval.kauriz, WORK1 );
			
			pArithEx->l_mul100( (unsigned char *)WORK1, (unsigned char *)stval.kyuriz_3, (unsigned char *)WORK0, 5, 0 );
			l_add( stval.koujo0_3, stval.kauriz_3, WORK1 );
			Kobet = 1;
		}
		else {		// 控除対象仕入税額 （一括比例配分方式）
			memset( WORK0, '\0', 6 );
			// 控除対象仕入税額x課税売上割合(%) 小数点以下第五位迄
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)stval.kwbuns, (unsigned char *)stval.kwbunb, 5, 0 );

			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof( PW6 ) );
			if( l_cmp( stval.kwbuns, PW6 ) < 0 )	memset( WORK0, '\0', 6 );
			if( l_cmp( stval.kwbunb, PW6 ) < 0 )	memset( WORK0, '\0', 6 );

			pArithEx->l_mul100( (unsigned char *)stval.koujo0,   (unsigned char *)stval.sirsyz,   (unsigned char *)WORK0, 5, 0 );
			pArithEx->l_mul100( (unsigned char *)stval.koujo0_3, (unsigned char *)stval.sirsyz_3, (unsigned char *)WORK0, 5, 0 );
		}
	}
	
	// 控除税額合計１
	l_add( stval.kjgok0, stval.koujo0, stval.henkan );
	l_add( stval.kjgok0, stval.kjgok0, stval.ksidao );
	l_add( stval.kjgok0_3, stval.koujo0_3, stval.henkan_3 );
	l_add( stval.kjgok0_3, stval.kjgok0_3, stval.ksidao_3 );
	// 差引納付税額１
	l_add( stval.nouhu0, stval.urisyz, stval.kaishu );
	l_sub( stval.nouhu0, stval.nouhu0, stval.kjgok0 );
	l_add( stval.nouhu0_3, stval.urisyz_3, stval.kaishu_3 );
	l_sub( stval.nouhu0_3, stval.nouhu0_3, stval.kjgok0_3 );
	
	// 新旧合計
	l_add( stval.koujo0, stval.koujo0, stval.koujo0_3 );
	l_add( stval.kjgok0, stval.kjgok0, stval.kjgok0_3 );
	l_add( stval.nouhu0, stval.nouhu0, stval.nouhu0_3 );
		
	if( ! Kenei ) {	// 原則課税 or 簡易課税・単一業者
		// 簡易課税　税額計算
		l_input( PW1, "1000" );
		if( Kani >= 2 )	{	// 簡易課税・単一事業者選択時
			Kenei |= 0x02;	// 兼営事業ＳＷ　ON
			goto KENEI;
		}
	}
	else {		// 兼営事業者

KENEI:
		// 事業別売上税額	&　控除対象仕入税額
		l_input( PW0, "1000" );
		l_input( PW5, "750" );		// 特例計算適用チェック
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memset( WORK2, '\0', 6 );	memset( WORK3, '\0', 6 );
		memset( Kenuri, '\0', 6 );	memset( Kenuri_n, '\0', 6 );
		
		// 事業別課税売上高
		if( ! pTblHdl->th_rget( &Bsval, foreB, 3 ) )	{	// 第１業種 3%
			l_add( stval.knuri1, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri1_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 11 ) )	// 第１業種 3%
				l_add( stval.knuri1, stval.knuri1, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) ) {	// 第１業種 4.5%
				l_add( stval.knuri1, stval.knuri1, Bsval.su_nuki );
				l_add( stval.knuri1, stval.knuri1, Bsval.su_nuks );
			}
		}
		
		if( ! pTblHdl->th_rget( &Bsval, foreB, 4 ) )	{	// 第２業種 3%
			l_add( stval.knuri2, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri2_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 12 ) )		// 第２業種 3%
				l_add( stval.knuri2, stval.knuri2, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 8 ) ) {		// 第２業種 4.5%
				l_add( stval.knuri2, stval.knuri2, Bsval.su_nuki );
				l_add( stval.knuri2, stval.knuri2, Bsval.su_nuks );
			}
		}
		
		if( ! pTblHdl->th_rget( &Bsval, foreB, 5 ) )	{	// 第３業種 3%
			l_add( stval.knuri3, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri3_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 13 ) )		// 第３業種 3%
				l_add( stval.knuri3, stval.knuri3, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 9 ) ) {	// 第３業種 4.5%
				l_add( stval.knuri3, stval.knuri3, Bsval.su_nuki );
				l_add( stval.knuri3, stval.knuri3, Bsval.su_nuks );
			}
		}
		
		if( ! pTblHdl->th_rget( &Bsval, foreB, 6 ) )	{	// 第４業種 3%
			l_add( stval.knuri4, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri4_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 14 ) )		// 第４業種 3%
				l_add( stval.knuri4, stval.knuri4, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 10 ) ) {	// 第４業種 4.5%
				l_add( stval.knuri4, stval.knuri4, Bsval.su_nuki );
				l_add( stval.knuri4, stval.knuri4, Bsval.su_nuks );
			}
		}
		
		if( TBhead.SVvers && ((Ver&0xff) == 0x02) ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) )		{	// 第５業種 3%
				l_add( stval.knuri5, Bsval.su_nuki, Bsval.su_nuki3 );
				memmove( stval.knuri5_n, Bsval.su_nuki, 6 );
			}
			if( ! pTblHdl->th_rget( &Bsval, foreB, 15 ) )		// 第５業種 3%
				l_add( stval.knuri5, stval.knuri5, Bsval.su_nuki3 );
		}
		
		// 事業別課税売上高合計
		// マイナスの場合は、合計に加えない（０円とする）
		if( l_test( stval.knuri1 ) < 0 )	l_input( stval.knuri1, "0" );
		if( l_test( stval.knuri2 ) < 0 )	l_input( stval.knuri2, "0" );
		if( l_test( stval.knuri3 ) < 0 )	l_input( stval.knuri3, "0" );
		if( l_test( stval.knuri4 ) < 0 )	l_input( stval.knuri4, "0" );
		if( l_test( stval.knuri5 ) < 0 )	l_input( stval.knuri5, "0" );
		if( l_test( stval.knuri1_n ) < 0 )	l_input( stval.knuri1_n, "0" );
		if( l_test( stval.knuri2_n ) < 0 )	l_input( stval.knuri2_n, "0" );
		if( l_test( stval.knuri3_n ) < 0 )	l_input( stval.knuri3_n, "0" );
		if( l_test( stval.knuri4_n ) < 0 )	l_input( stval.knuri4_n, "0" );
		if( l_test( stval.knuri5_n ) < 0 )	l_input( stval.knuri5_n, "0" );

		// 事業別課税売上高合計(縦計ではなく、全体の課税売上高) 
		l_add( Kenuri,   Kenuri,   stval.knuri1 );
		l_add( Kenuri,   Kenuri,   stval.knuri2 );
		l_add( Kenuri,   Kenuri,   stval.knuri3 );
		l_add( Kenuri,   Kenuri,   stval.knuri4 );
		l_add( Kenuri,   Kenuri,   stval.knuri5 );
		l_add( Kenuri_n, Kenuri_n, stval.knuri1_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri2_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri3_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri4_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri5_n );

		// 事業別売上発生チェック
		tksg = 0;
		if( l_test( stval.knuri1 ) )	tksg++;
		if( l_test( stval.knuri2 ) )	tksg++;
		if( l_test( stval.knuri3 ) )	tksg++;
		if( l_test( stval.knuri4 ) )	tksg++;
		if( l_test( stval.knuri5 ) )	tksg++;
		
		if( l_test( stval.knuri1 ) ) {		// 卸売
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			memset( WORK3, '\0', 6 );	memset( WORK4, '\0', 6 );
			ksg |= 0x01;	++kensg;
			if( tksg == 1 ) {		// 単一事業
				memmove( stval.knuri1, Kenuri,   6 );
				memmove( stval.knuri1_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri_n, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 3 ) )	{	// 第１業種 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 11 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) )		// 第１業種 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			// 課税売上に係る税額
			if( !K221sw ) {
				percent( (char *)stval.knzei1_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
				percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
				l_add( stval.knzei1, stval.knzei1_n, WORK1 );
			}
			else	{
				// ４％積上げ
				pTblHdl->th_rget( &Bsval, foreB, 3 );	// 第１業種 5%
				memset( WORK0, '\0', 6 );	l_input( WORK0, "80" );
				pArithEx->l_mul100( (unsigned char *)stval.knzei1_n, (unsigned char *)Bsval.su_zi5,
																	(unsigned char *)WORK0, 0, 0 );
				pTblHdl->th_rget( &Bsval, foreB, 11 );	// 第１業種 3%
				l_add( stval.knzei1, stval.knzei1_n, Bsval.su_zi3 );
			}
			
			l_6calq( (char *)stval.knwai1, (char *)stval.knuri1, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai1_n, (char *)stval.knuri1_n, PW0, Kenuri_n );
			if( l_cmp( stval.knwai1, PW5 ) >= 0 )	toku |= 0x01;	// 75%以上
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[0] ) );
			l_input( PW3, WORK2 );
			l_input( PW4, WORK2 );
			l_6calq( (char *)stval.koujo1,    (char *)stval.knzei1,   PW3, PW0 );	// 原則
			l_6calq( (char *)stval.koujo11,   (char *)stval.knzei1,   PW3, PW0 );	// 特例
			
			if( Kenei & 0x02 )	goto KEN4;		// 単一事業 & 資産譲渡有
		}
		
		if( l_test( stval.knuri2 ) ) {		// 小売
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x02;
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri2,   Kenuri,   6 );
				memmove( stval.knuri2_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 4 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 12 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 8 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			// 課税売上に係る税額
			if( !K221sw ) {
				// 課税売上に係る税額
				percent( (char *)stval.knzei2_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
				percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
				l_add( stval.knzei2, stval.knzei2_n, WORK1 );
			}
			else	{
				// ４％積上げ
				pTblHdl->th_rget( &Bsval, foreB, 4 );	// 第2業種 5%
				memset( WORK0, '\0', 6 );	l_input( WORK0, "80" );
				pArithEx->l_mul100( (unsigned char *)stval.knzei2_n, (unsigned char *)Bsval.su_zi5,
																	(unsigned char *)WORK0, 0, 0 );
				pTblHdl->th_rget( &Bsval, foreB, 12 );	// 第2業種 3%
				l_add( stval.knzei2, stval.knzei2_n, Bsval.su_zi3 );
			}
			
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[1] ) );
			l_6calq( (char *)stval.knwai2, (char *)stval.knuri2, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai2_n, (char *)stval.knuri2_n, PW0, Kenuri_n );
			if( ! toku ) {	// 特例なし
				if( kensg ) {	// 第１業種残高有り
					l_add( WORK3, stval.knuri1, stval.knuri2 );	// (1)+(2)
					l_6calq( WORK3, WORK3, PW0, Kenuri );	// チェック
				}
				else	l_input( WORK3, "0" );
				if( (l_cmp( stval.knwai2, PW5 ) >= 0) ||	// 75%以上
					(l_cmp( WORK3, PW5 ) >= 0) ) {
					if( l_test( WORK3 ) )	toku |= 0x12;
					else					toku |= 0x02;
				}
				l_input( PW3, WORK2 );
			}
			l_input( PW4, WORK2 );	++kensg;
			l_6calq( (char *)stval.koujo2,    (char *)stval.knzei2,   PW4, PW0 );	// 原則計算
			l_6calq( (char *)stval.koujo12,   (char *)stval.knzei2,   PW3, PW0 );	// 特例計算
			if( Kenei & 0x02 )	goto KEN4;			// 単一事業 & 資産譲渡有
		}
		
		if( l_test( stval.knuri3 ) ) {		// 製造
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x04;
			
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri3,   Kenuri,   6 );
				memmove( stval.knuri3_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 5 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 13 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 9 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			if( !K221sw ) {
				// 課税売上に係る税額
				percent( (char *)stval.knzei3_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
				percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
				l_add( stval.knzei3, stval.knzei3_n, WORK1 );
			}
			else	{
				// ４％積上げ
				pTblHdl->th_rget( &Bsval, foreB, 5 );	// 第3業種 5%
				memset( WORK0, '\0', 6 );	l_input( WORK0, "80" );
				pArithEx->l_mul100( (unsigned char *)stval.knzei3_n, (unsigned char *)Bsval.su_zi5,
																	(unsigned char *)WORK0, 0, 0 );
				pTblHdl->th_rget( &Bsval, foreB, 13 );	// 第3業種 3%
				l_add( stval.knzei3, stval.knzei3_n, Bsval.su_zi3 );
			}
			
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[2] ) );
			l_6calq( (char *)stval.knwai3, (char *)stval.knuri3, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai3_n, (char *)stval.knuri3_n, PW0, Kenuri_n );
			if( ! toku ) {	// 特例無し
				l_input( WORK3, "0" );	l_input( WORK4, "0" );
				if( kensg ) {	// 第１ or 第２業種残高有り
					if( l_test( stval.knuri1 ) ) {
						l_add( WORK3, stval.knuri3, stval.knuri1 );
						l_6calq( WORK3, WORK3, PW0, Kenuri );
					}
					if( l_test( stval.knuri2 ) ) {
						l_add( WORK4, stval.knuri3, stval.knuri2 );
						l_6calq( WORK4, WORK4, PW0, Kenuri );
					}
					if( l_cmp( WORK3, PW5 ) >= 0 )	toku |= 0x14;
					else	l_input( WORK3, "0" );
					if( l_cmp( WORK4, PW5 ) >= 0 )	toku |= 0x24;
					else	l_input( WORK4, "0" );
				}
				else {
					if( l_cmp( stval.knwai3, PW5 ) >= 0 )	toku |= 0x04;
				}
				l_input( PW3, WORK2 );
			}
			l_input( PW4, WORK2 );	++kensg;
			l_6calq( (char *)stval.koujo3,    (char *)stval.knzei3,   PW4, PW0 );	// 原則
			l_6calq( (char *)stval.koujo13,   (char *)stval.knzei3,   PW3, PW0 );	// 特例
			if( l_test( WORK3 ) || l_test( WORK4 ) ) {	// 特例の判定
				if( l_test( WORK3 ) ) {
					l_6calq( WORK3, (char *)stval.knzei2, PW3, PW0 );
					l_add( WORK3, WORK3, stval.koujo1 );	// 第1+第3
					l_add( WORK3, WORK3, stval.koujo13 );	// の特例
				}
				if( l_test( WORK4 ) ) {
					l_6calq( WORK4, (char *)stval.knzei1, PW3, PW0 );
					l_add( WORK4, WORK4, stval.koujo2 );
					l_add( WORK4, WORK4, stval.koujo13 );	// 第2+第3の特例
				}
				if( (l_cmp( WORK3, WORK4 )) >= 0 ) {	// 第1+第3の特例採用
					l_6calq( (char *)stval.koujo12, (char *)stval.knzei2, PW3, PW0 );
					toku &= 0x1f;
				}
				else {									// 第2+第3の特例採用
					l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
					toku &= 0x2f;
				}
			}
			if( Kenei & 0x02 )	goto KEN4;		// 単一事業 & 資産譲渡有
		}
KEN4:
		if( l_test( stval.knuri4 ) ) { 		// その他
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x08;
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri4,   Kenuri,   6 );
				memmove( stval.knuri4_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 6 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 14 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 10 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			if( !K221sw ) {
				// 課税売上に係る税額
				percent( (char *)stval.knzei4_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
				percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
				l_add( stval.knzei4, stval.knzei4_n, WORK1 );
			}
			else	{
				// ４％積上げ
				pTblHdl->th_rget( &Bsval, foreB, 6 );	// 第4業種 5%
				memset( WORK0, '\0', 6 );	l_input( WORK0, "80" );
				pArithEx->l_mul100( (unsigned char *)stval.knzei4_n, (unsigned char *)Bsval.su_zi5,
																	(unsigned char *)WORK0, 0, 0 );
				pTblHdl->th_rget( &Bsval, foreB, 14 );	// 第4業種 3%
				l_add( stval.knzei4, stval.knzei4_n, Bsval.su_zi3 );
			}

			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[3] ) );
			l_6calq( (char *)stval.knwai4, (char *)stval.knuri4, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai4_n, (char *)stval.knuri4_n, PW0, Kenuri_n );
			if( ! toku ) {
				l_input( WORK3, "0" );	l_input( WORK4, "0" );
				l_input( WORK5, "0" );
				if( kensg ) {	// 第１ or 第２ or 第３業種残高有り
					if( l_test( stval.knuri1 )) {		// 第1 + 第4
						l_add( WORK3, stval.knuri4, stval.knuri1 );
						l_6calq( WORK3, WORK3, PW0, Kenuri );
					}
					if( l_test( stval.knuri2 )) {		// 第2 + 第4
						l_add( WORK4, stval.knuri4, stval.knuri2 );
						l_6calq( WORK4, WORK4, PW0, Kenuri );
					}
					if( l_test( stval.knuri3 )) {		// 第3 + 第4
						l_add( WORK5, stval.knuri4, stval.knuri3 );
						l_6calq( WORK5, WORK5, PW0, Kenuri );
					}

					if( l_cmp( WORK3, PW5 ) >= 0 ) toku |= 0x18;
					else	l_input( WORK3, "0" );
					if( l_cmp( WORK4, PW5 ) >= 0 ) toku |= 0x28;
					else	l_input( WORK4, "0" );
					if( l_cmp( WORK5, PW5 ) >= 0 ) toku |= 0x48;
					else	l_input( WORK5, "0" );
				}
				else	{
					if( l_cmp( stval.knwai4, PW5 ) >= 0 ) toku |= 0x08;
				}
				l_input( PW3, WORK2 );	// 特例計算摘要なし
			}
			
			l_input( PW4, WORK2 );	++kensg;
			l_6calq( (char *)stval.koujo4,   (char *)stval.knzei4,   PW4, PW0 );	// 原則
			l_6calq( (char *)stval.koujo14,  (char *)stval.knzei4,   PW3, PW0 );	// 特例
			
			if( l_test( WORK3 ) || l_test( WORK4 ) || l_test( WORK5 ) ) {
				if( l_test( WORK3 ) ) {		// 特例の判定
					l_6calq( WORK1, (char *)stval.knzei2, PW3, PW0 );
					l_6calq( WORK3, (char *)stval.knzei3, PW3, PW0 );
					l_add( WORK3, WORK3, WORK1 );
					l_add( WORK3, WORK3, stval.koujo1 );
					l_add( WORK3, WORK3, stval.koujo14 );	// 第1＋第4の特例
				}
				if( l_test( WORK4 ) ) {
					l_6calq( WORK1, (char *)stval.knzei1, PW3, PW0 );
					l_6calq( WORK4, (char *)stval.knzei3, PW3, PW0 );
					l_add( WORK4, WORK4, WORK1 );
					l_add( WORK4, WORK4, stval.koujo2 );
					l_add( WORK4, WORK4, stval.koujo14 ); // 第2＋第4の特例
				}
				if( l_test( WORK5 ) ) {
					l_6calq( WORK1, stval.knzei1, PW3, PW0 );
					l_6calq( WORK5, stval.knzei2, PW3, PW0 );
					l_add( WORK5, WORK5, WORK1 );
					l_add( WORK5, WORK5, stval.koujo3 );
					l_add( WORK5, WORK5, stval.koujo14 ); // 第3＋第4の特例
				}
				
				if( (l_cmp( WORK3, WORK4 )) >= 0 ) { // 第1＋第4の特例採用
					if( (l_cmp( WORK3, WORK5 )) >= 0 ) {
						l_6calq( stval.koujo12, stval.knzei2, PW3, PW0 );
						l_6calq( stval.koujo13, stval.knzei3, PW3, PW0 );
						toku &= 0x1f;
					}
					else	{
						l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
						l_6calq( (char *)stval.koujo12, (char *)stval.knzei2, PW3, PW0 );
						toku &= 0x4f;
					}
				}
				else	{							// 第2＋第4の特例採用
					if( (l_cmp( WORK4, WORK5 )) >= 0 ) {
						l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
						l_6calq( (char *)stval.koujo13, (char *)stval.knzei3, PW3, PW0 );
						toku &= 0x2f;
					}
					else	{
						l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
						l_6calq( (char *)stval.koujo12, (char *)stval.knzei2, PW3, PW0 );
						toku &= 0x4f;
					}
				}
			}
		}
		
		if( l_test( stval.knuri5 ) ) {		// サービス
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x10;
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri5,   Kenuri,   6 );
				memmove( stval.knuri5_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 15 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 11 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			if( !K221sw ) {
				// 課税売上に係る税額
				percent( (char *)stval.knzei5_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
				percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
				l_add( stval.knzei5, stval.knzei5_n, WORK1 );
			}
			else	{
				// ４％積上げ
				pTblHdl->th_rget( &Bsval, foreB, 7 );	// 第5業種 5%
				memset( WORK0, '\0', 6 );	l_input( WORK0, "80" );
				pArithEx->l_mul100( (unsigned char *)stval.knzei5_n, (unsigned char *)Bsval.su_zi5,
																	(unsigned char *)WORK0, 0, 0 );
				pTblHdl->th_rget( &Bsval, foreB, 15 );	// 第5業種 3%
				l_add( stval.knzei5, stval.knzei5_n, Bsval.su_zi3 );
			}
			
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[4] ) );
			l_6calq( (char *)stval.knwai5, (char *)stval.knuri5, PW0, Kenuri );		// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai5_n, (char *)stval.knuri5_n, PW0, Kenuri_n );
			if( !toku )	l_input( PW3, WORK2 );
			l_input( PW4, WORK2 );	++kensg;
			
			l_6calq( (char *)stval.koujo5,   (char *)stval.knzei5,   PW4, PW0 );	// 原則
			l_6calq( (char *)stval.koujo15,   (char *)stval.knzei5,   PW3, PW0 );	// 特例
		}
		else {
			l_clear( stval.knuri5 );	l_clear( stval.knuri5_n );
			l_clear( stval.knzei5 );	l_clear( stval.knzei5_n );
			l_clear( stval.knwai5 );	l_clear( stval.knwai5_n );
		}

		switch( kensg ) {	// 特例計算適用可能チェック
			case 1 :	// 単一事業のみ発生
						toku = 0x00;	break;
			case 2 :	// 2事業合計で75%以上
						if( toku & 0xf0 )	toku = 0x00;	break;
			default:	break;
		}
	}
	
	// 申告書マスター計算
	Gsw = gen_chk( sno );

	stm_new( ksg );
	memmove( Stm.Kuri1, stval.knuri1, 6 );	// 第１種事業　課税売上
	memmove( Stm.Kuri2, stval.knuri2, 6 );	// 第２種事業　　 〃
	memmove( Stm.Kuri3, stval.knuri3, 6 );	// 第３種事業　　 〃
	memmove( Stm.Kuri4, stval.knuri4, 6 );	// 第４種事業　　 〃
	memmove( Stm.Kuri5, stval.knuri5, 6 );	// 第５種事業　　 〃
	Stm.Up1 = UP1;
	Stm.Up2 = UP2;
	Stm.Up3 = UP3;
	Stm.Up4 = UP4;
	Stm.Up5 = UP5;

	return( FALSE );
}

// 個別対応税額計算（簡易計算のみ）
int CShinCalculateW::skb_calq( int sc, int tc, char *kzei4, char *kzei3, char *czei4, char *czei3 )
{
	int	j;
	char	kkomi4[6], knuki4[6], kkomi3[6], knuki3[6], ckomi4[6], cnuki4[6], ckomi3[6], cnuki3[6];
	char	kkomz4[6], knukz4[6], kkomz3[6], knukz3[6], ckomz4[6], cnukz4[6], ckomz3[6], cnukz3[6];

	// 金額の積み上げ
	memset( kkomi4, '\0', 6 );	memset( kkomz4, '\0', 6 );	// 課税に係る
	memset( knuki4, '\0', 6 );	memset( knukz4, '\0', 6 );	// 〃
	memset( kkomi3, '\0', 6 );	memset( kkomz3, '\0', 6 );	// 〃
	memset( knuki3, '\0', 6 );	memset( knukz3, '\0', 6 );	// 〃
	memset( ckomi4, '\0', 6 );	memset( ckomz4, '\0', 6 );	// 共通に係る
	memset( cnuki4, '\0', 6 );	memset( cnukz4, '\0', 6 );	// 〃
	memset( ckomi3, '\0', 6 );	memset( ckomz3, '\0', 6 );	// 〃
	memset( cnuki3, '\0', 6 );	memset( cnukz3, '\0', 6 );	// 〃
	for( j = sc ; j < tc ; ++j ) {
		// ５％
		l_add( kkomi4, kkomi4, &kobetu_5.SZdtbl[j].SDval[0] );	// 課税に係る
		l_add( knuki4, knuki4, &kobetu_5.SZdtbl[j].SDval[1] );	// 〃
		l_add( ckomi4, ckomi4, &kobetu_5.SZdtbl[j].SDval[4] );	// 共通に係る
		l_add( cnuki4, cnuki4, &kobetu_5.SZdtbl[j].SDval[5] );	// 〃
		// ３％
		l_add( kkomi3, kkomi3, &kobetu_3.SZdtbl[j].SDval[0] );	// 課税に係る
		l_add( knuki3, knuki3, &kobetu_3.SZdtbl[j].SDval[1] );	// 〃
		l_add( ckomi3, ckomi3, &kobetu_3.SZdtbl[j].SDval[4] );	// 共通に係る
		l_add( cnuki3, cnuki3, &kobetu_3.SZdtbl[j].SDval[5] );	// 〃

	}
	// 税額計算
	percent( kkomz4, kkomi4, 4, 40, 0 );	// ４％課税に係る　込み
	l_add( kzei4, kzei4, kkomz4 );
	percent( knukz4, knuki4, 1, 40, 0 );	// ４％課税に係る　抜き
	l_add( kzei4, kzei4, knukz4 );
	percent( kkomz3, kkomi3, 0, 30, 0 );	// ３％課税に係る　込み
	l_add( kzei3, kzei3, kkomz3 );
	percent( knukz3, knuki3, 1, 30, 0 );	// ３％課税に係る　抜き
	l_add( kzei3, kzei3, knukz3 );

	percent( ckomz4, ckomi4, 4, 40, 0 );	// ４％課税に係る　込み
	l_add( czei4, czei4, ckomz4 );
	percent( cnukz4, cnuki4, 1, 40, 0 );	// ４％課税に係る　抜き
	l_add( czei4, czei4, cnukz4 );
	percent( ckomz3, ckomi3, 0, 30, 0 );	// ３％課税に係る　込み
	l_add( czei3, czei3, ckomz3 );
	percent( cnukz3, cnuki3, 1, 30, 0 );	// ３％課税に係る　抜き
	l_add( czei3, czei3, cnukz3 );

//★//[13'05.31]///
	// 輸入仕入税額の加算
	l_add( kzei4, kzei4, KazIp4 );
	l_add( kzei3, kzei3, KazIp3 );
	l_add( czei4, czei4, ComIp4 );
	l_add( czei3, czei3, ComIp3 );
///////////////////
	return( FALSE );
}


// 消費税集計（簡易計算）
int CShinCalculateW::stm_scalq( int sno, int tno, char *karb, char *karu, char *karb3, char *karu3,
					struct TB_PAR *foreA, struct TB_PAR *foreB )
{
	int		i, i1, j, tc, sc, sw, uck;
	unsigned	char	ksg, Furi, tksg;
	char	WORK0[6], WORK1[6], WORK2[6], WORK3[6], WORK4[6], WORK5[6], WORK6[6], WORK7[6],
			PW0[6], PW1[6], PW3[6], PW4[6], PW5[6], PW6[6]; 

	// 経過月計算
	Kazmon = tc = tno + 1;
	sc = sno;
	if( Kazmon > 12 )	Kazmon = 12;
	Kazmon = Kazmon - sc;
	if( ! Kazmon )	Kazmon = 1;

	memset( s_ymd, '\0', sizeof( s_ymd ) );
	memset( e_ymd, '\0', sizeof( e_ymd ) );
	memmove( s_ymd, Vol1.SS_YMD, 3 );
	shin_datecnv( Vol1.SS_YMD[0], (unsigned char *)s_ymd, 0 );
	memmove( e_ymd, Vol1.EE_YMD, 3 );
	shin_datecnv( Vol1.EE_YMD[0], (unsigned char *)e_ymd, 0 );
		
	// 消費税マスタ－　金額集計
	l_defn( 0x16 );
	memset( &stval, '\0', sizeof( struct _STUVAL ) );
	kani1 = kensg = 0;						//	簡易課税適用範囲ＳＷ
	toku = 0x00;							// 	特例ＳＷ
	Kenei &= 0x01;							//  兼営事業選択ＳＷクリア
	ksg = 0x00;								//  兼営事業　残高有無ＳＷ
	Furi = 0x00;							//	振替仕訳チェック

	memset( Kenuri, '\0', 6 );				//	事業別課税売上高合計
	memset( Kenuri_n, '\0', 6 );			//	事業別課税売上高合計(新税率分)

	// 輸入仕入計算
	memset( WORK0, '\0', 6 );
	if( (TBhead.SVvers & 0xf0) >= 0x10 ) {
		for( j = sc ; j < tc ; ++j ) {	// 輸入仕入集計
			l_add( stval.IpStkz_5, stval.IpStkz_5, SVsum.SSustb[j].SSval5 );
			l_add( stval.IpStze_5, stval.IpStze_5, SVsum.SSustb[j].SSzei5 );
			l_add( stval.IpStze_4, stval.IpStze_4, SVsum.SSustb[j].SSzei4 );
			l_add( stval.IpStkz_3, stval.IpStkz_3, SVsum.SSustb[j].SSval3 );
			l_add( stval.IpStze_3, stval.IpStze_3, SVsum.SSustb[j].SSzei3 );
			memmove( stval.IpStz0_3, stval.IpStze_3, 6 );
		}
	}

//////////////////
// 集計レコード //
//////////////////

#ifdef	VERSIONUP
	// 特定収入計算表
	if( m_SpcSg ) {
		SPCLQ	SPrec;
		for( i = 0 ; i < SpTbl.tp_rnum; ++i ) {
			if( pTblHdl->th_rget( &SPrec, &SpTbl, i ) ) return( -1 );
			for( j = sc ; j < tc ; ++j ) {
				// 特定収入（課税売上に係る）
				l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_tkzn[j] );
				// 〃　　　（非課税に係る）
				l_add( stval.SpHsyu, stval.SpHsyu, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				l_add( stval.SpKsyu, stval.SpKsyu, &SPrec.Sp_tczn[j] );
				//	不特定収入
				l_add( stval.SpFsyu, stval.SpFsyu, &SPrec.Sp_tszn[j] );
			}
		}
		l_neg( stval.SpTsyu );
		l_neg( stval.SpKsyu );
		l_neg( stval.SpFsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpTsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpKsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpFsyu );
		l_neg( stval.SpHsyu );
		l_add( stval.SpGsyu, stval.SpGsyu, stval.SpHsyu );
	}
#endif

	for( i = 0 ; i < foreA->tp_rnum; ++i ) {
		if( pTblHdl->th_rget( &Asval, foreA, i ) ) return( -1 );
		memset( &Asval.komi, '\0', T_SIZE );
		
		// 月途中での税率切替テーブルオフセット
		i1 = (int) (Asval.pj_scar & 0x0f);	i1--;
		
		for( j = tc - 1, sw = 0; j >= sc ; --j ) {
						
			l_add( Asval.hika, Asval.hika, Asval.h_mnth[j].hika_val );
			l_add( Asval.hika, Asval.hika, Asval.h3_mnth[j].hika3_val );
			
			if( Asval.pj_zino == 1 && i1 >= j ) {
				l_add( Asval.koms, Asval.koms, Asval.h_mnth[j].komi_val );
				l_add( Asval.nuks, Asval.nuks, Asval.h_mnth[j].nuki_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuks,Asval.knuks,Asval.h_mnth[j].komi_val );
			}
			else {
				l_add( Asval.komi, Asval.komi, Asval.h_mnth[j].komi_val );
				l_add( Asval.nuki, Asval.nuki, Asval.h_mnth[j].nuki_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuki,Asval.knuki,Asval.h_mnth[j].komi_val );
				
				// 旧税率集計
				l_add( Asval.komi3, Asval.komi3, Asval.h3_mnth[j].komi3_val );
				l_add( Asval.nuki3, Asval.nuki3, Asval.h3_mnth[j].nuki3_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuki3,Asval.knuki3,
										Asval.h3_mnth[j].komi3_val);
			}
			if( (Asval.pj_scar & 0x10) && i1 == j ) {
				l_add( Asval.nuki3, Asval.nuki3, Asval.nuks_val );
				l_add( Asval.komi3, Asval.komi3, Asval.koms_val );
				if( !sw )		// 一括税抜まで集計
					l_add( Asval.knuki3, Asval.knuki3, Asval.koms_val );
			}
			// 税額積み上げ
			l_add( Asval.komz, Asval.komz, Asval.h_mnth[j].komz_val );
			l_add( Asval.nukz, Asval.nukz, Asval.h_mnth[j].nukz_val );
			l_add( Asval.yuka, Asval.yuka, Asval.h_mnth[j].yuka_val );
			if( !sw )		// 一括税抜まで集計
					l_add( Asval.knkz, Asval.knkz, Asval.h_mnth[j].komz_val );
			l_add( Asval.komz3, Asval.komz3, Asval.h3_mnth[j].komz3_val );
			l_add( Asval.nukz3, Asval.nukz3, Asval.h3_mnth[j].nukz3_val );
			l_add( Asval.yuka3, Asval.yuka3, Asval.h3_mnth[j].yuka3_val );
			if( !sw )		// 一括税抜まで集計
					l_add( Asval.knkz3, Asval.knkz3, Asval.h3_mnth[j].komz3_val );
		}

		if( Asval.pj_ksign || Asval.pj_sjsg ) {
			l_neg( Asval.hika );
			l_neg( Asval.komi );
			l_neg( Asval.nuki );
			l_neg( Asval.knuki );
			l_neg( Asval.koms );
			l_neg( Asval.nuks );
			l_neg( Asval.knuks );
			l_neg( Asval.komi3 );
			l_neg( Asval.nuki3 );
			l_neg( Asval.knuki3 );
			l_neg( Asval.komz );
			l_neg( Asval.nukz );
			l_neg( Asval.knkz );
			l_neg( Asval.yuka );
			l_neg( Asval.komz3 );
			l_neg( Asval.nukz3 );
			l_neg( Asval.knkz3 );
			l_neg( Asval.yuka3 );
		}
		if( pTblHdl->th_rput( &Asval, foreA, i ) ) return( -1 );

	}
	
	if( (Ver&0x0f) == 2 && TBhead.SV_5sw )	uck = 1;
	else									uck = 0;
	memset( PW0, '\0', 6 );	l_input( PW0, "400" );
	memset( PW1, '\0', 6 );	l_input( PW1, "500" );
	for( i = 0 ; i < foreB->tp_rnum ; ++i ) {
		
		if( pTblHdl->th_rget( &Bsval, foreB, i ) ) return( -1 );
		memset( Bsval.su_nuki, '\0', 6 * 3 );
		i1 = (int) (Bsval.su_ofs & 0x0f);	i1--;
		for( j = sc ; j < tc ; ++j ) {	// 課税売上割合用集計
			memset( WORK0, '\0', 6 );
			
			if( TBhead.SVvers == 0 ) {
				if( (i > 6) && (i1 >= j) )	// 旧マスター売上業種区分（車）
					l_add( Bsval.su_nuks, Bsval.su_nuks, Bsval.su_val[j] );
				else
					l_add( Bsval.su_nuki3, Bsval.su_nuki3, Bsval.su3_val[j] );
			}
			else	{
				// 5%
				memmove( WORK0, Bsval.su_val[j], 6 );
				// 3%
				l_add( Bsval.su_nuki3, Bsval.su_nuki3, Bsval.su3_val[j] );
			}
			
			if( (Bsval.su_ofs & 0x10) && j == i1 )
					l_add( Bsval.su_nuki3, Bsval.su_nuki3, Bsval.su_val2 );
			if( (i < 3) && l_test( WORK0 ) ) {
				if( ! Bsval.su_per[j] ) { 	// 5% × 40/50
					pArithEx->l_mul100( (unsigned char *)WORK1, (unsigned char *)WORK0, (unsigned char *)PW0, 0, 0 );
					pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)WORK1, (unsigned char *)PW1, 0, 5 );
				}
			}
			l_add( Bsval.su_nuki, Bsval.su_nuki, WORK0 );
		}
		if( i > 2 ) {
			if( TBhead.SVvers ) {
				if( (i < 8) || (i > 10) ) {
					l_neg( Bsval.su_nuki );
					l_neg( Bsval.su_nuki3 );
					l_neg( Bsval.su_nuks );
				}
			}
			else	{
				l_neg( Bsval.su_nuki );
				l_neg( Bsval.su_nuki3 );
				l_neg( Bsval.su_nuks );
			}
		}
		
		if( uck && (i == 7 || i == 15) ) {		// 第五種事業の経過措置
			memset( SBval.su_nuki, '\0', 6 * 3 );
			if( pTblHdl->th_rget( &SBval, foreB, i-1 ) ) return( -1 );
			
			l_add( SBval.su_nuki,  SBval.su_nuki,  Bsval.su_nuki );
			l_add( SBval.su_nuki3, SBval.su_nuki3, Bsval.su_nuki3 );
			l_add( SBval.su_nuks,  SBval.su_nuks,  Bsval.su_nuks );
			
			if( pTblHdl->th_rput( &SBval, foreB, i-1 ) ) return( -1 );
			
			memset( Bsval.su_nuki, '\0', 6 * 3 );
		}
		
		if( pTblHdl->th_rput( &Bsval, foreB, i ) ) return( -1 );
	}
	
	// 簡易課税
	if( Kani > 1 && (Vol1.S_SGN1&0xc0) == 0x00 )	{
		memset( PW0, '\0', 6 );	l_input( PW0, "1050" );
		memset( PW1, '\0', 6 );	l_input( PW1, "1030" );
		for( i = 0 ; i < foreB->tp_rnum ; ++i ) {		
			if( pTblHdl->th_rget( &Bsval, foreB, i ) ) return( -1 );
			memset( WORK0, '\0', 6 );	// ５％込み
			memset( WORK1, '\0', 6 );	// ５％抜き（別記）
			memset( WORK2, '\0', 6 );	// ３％込み
			memset( WORK3, '\0', 6 );	// ３％抜き（別記）
			memset( WORK4, '\0', 6 );	// ５％　税額
			memset( WORK5, '\0', 6 );	// ３％　税額
			memset( WORK6, '\0', 6 );	// ５％抜き
			memset( WORK7, '\0', 6 );	// ５％抜き
			for( j = sc; j < tc; ++j ) {
				if( i < 8 ) {
					l_add( WORK0, WORK0, Bsval.su_kvl[j] );
					l_add( WORK1, WORK1, Bsval.su_kbk[j] );
					l_add( WORK4, WORK4, Bsval.su_zei[j] );
					l_add( WORK6, WORK6, Bsval.su_nvl[j] );
				}
				else	{
					l_add( WORK2, WORK2, Bsval.su_kvl3[j] );
					l_add( WORK3, WORK3, Bsval.su_kbk3[j] );
					l_add( WORK5, WORK5, Bsval.su_zei3[j] );
					l_add( WORK7, WORK7, Bsval.su_nvl3[j] );
				}
			}
			// 
			l_neg( WORK0 );
			l_neg( WORK1 );
			l_neg( WORK2 );
			l_neg( WORK3 );
			l_neg( WORK4 );
			l_neg( WORK5 );
			l_neg( WORK6 );
			l_neg( WORK7 );	
			memset( Bsval.su_nuki, '\0', 6 * 3 );
			memset( Bsval.su_zi5, '\0', 6 * 2 );
			// 積上げ方式 [07'04.16]
			if( K221sw ) {
				// 本体
				l_add( Bsval.su_nuki,  WORK1, WORK6 );
				l_add( Bsval.su_nuki3, WORK3, WORK7 );

				// 税額
				l_add( Bsval.su_zi5, Bsval.su_zi5, WORK4 );
				l_add( Bsval.su_zi3, Bsval.su_zi3, WORK5 );
			}
			else	{
				pArithEx->l_par100( (unsigned char *)Bsval.su_nuki, (unsigned char *)WORK0,(unsigned char *)PW0, 1, 0 );
				l_add( Bsval.su_nuki, Bsval.su_nuki, WORK1 );
				pArithEx->l_par100( (unsigned char *)Bsval.su_nuki3, (unsigned char *)WORK2,(unsigned char *)PW1, 1, 0 );
				l_add( Bsval.su_nuki3, Bsval.su_nuki3, WORK3 );
			}

			if( pTblHdl->th_rput( &Bsval, foreB, i ) ) return( -1 );
		}
	}

	// 売上割合　金額集計
	// 営業収益
	// 全売上にかかわる科目の込金額からまとめて税額をぬく様変更
	//		A.   課税売上高	:	C.   非課税売上高	:	課税商品売上高
	memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
	memset( WORK2, '\0', 6 );	memset( WORK3, '\0', 6 );
	memset( WORK5, '\0', 6 );
	if( ! key_set( foreA, 0x02, 0, 0, 0 ) ) {		// 売上高セット
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// C.
	}
	
	//		A.   C.   ｢課税商品自動車売上高」	＋ 	自動車売上高
	if( ! key_set( foreA, 0x02, 0, 0x01, 0 ) ) {	// 自動車売上高
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuks );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.komiur, stval.komiur, Asval.knuks );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		l_add( stval.kkomur, stval.kkomur, Asval.koms );
		
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// C.
	}
	
	//		A.   C.		＋	B. （輸出売上高） 税抜き ＋ 税込み
	if( ! key_set( foreA, 0x02, 0, 0x02, 0 ) ) {	// 輸出売上高
		l_add( stval.exptur, Asval.nuki,   Asval.komi );		// A.
		l_add( stval.exptur, stval.exptur, Asval.nuki3 );
		l_add( stval.exptur, stval.exptur, Asval.komi3 );		// A.
		l_add( stval.hikazu, stval.hikazu, Asval.hika );		// C.
	}

	if( ! key_set( foreA, 0x11, 0x02, 0x02, 0x01 ) ) {	// 譲渡輸出売上高
		// 課税取引は免税売上額に計上
		l_add( stval.exptur, stval.exptur, Asval.nuki );			// A.
		l_add( stval.exptur, stval.exptur, Asval.komi );			// A.
		l_add( stval.exptur, stval.exptur, Asval.nuki3 );
		l_add( stval.exptur, stval.exptur, Asval.komi3 );
		// その他営業外収入（非課税分）に集計
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}

	// 非課税輸出売上 [06'11.06]
	if( ! key_set( foreA, 0x02, 0x08, 0x00, 0x00 ) ) {
		// 非課税売上げは非課税資産の輸出等の金額に計上される
		l_add( stval.expjyo, stval.expjyo, Asval.hika );
	}

	//	営業外収益
	
	//	課税分
	
	//		E.   土地以外の固定資産譲渡収入	:	課税商品売上高
	if( ! key_set( foreA, 0x11, 0x02, 0, 0x01 ) ) {		// 固定資産譲渡
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		// 固定資産譲渡収入　（非課税分）
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
	
	//		F.   その他営業外収入	: 	K.   その他営業外収入（非課税分）
	if( ! key_set( foreA, 0x02, 0x02, 0, 0 ) ) {		// 営業利益
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
	//		＋　特別利益 
	if( ! key_set( foreA, 0x02, 0x03, 0, 0 ) ) {		// 特別利益
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計 
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		// 非課税分
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
	//		F. その他営業外収入	＋流動負債譲渡
	if( ! key_set( foreA, 0x01, 0x01, 0, 0 ) ) {		// 流動負債譲渡
		l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
		l_add( stval.komiur, stval.komiur, Asval.knuki );
		l_add( stval.kkomur, stval.kkomur, Asval.komi );
		
		// 旧税率集計
		l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
		l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
		l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
		
		// 非課税分
		l_add( stval.hikazu, stval.hikazu, Asval.hika );	// K.
	}
		
	//	非課税分
	//		H.   受取利息
	if( (Vol1.APNO&0xff) == 0x10 ) {	// 個人:受取利息
		if( ! key_set( foreA, 0x01, 0x05, 0, 0 ) ) {
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}
	}
	else {
		if( ! key_set( foreA, 0x02, 0x04, 0, 0 ) ) {		// 受取利息
			l_add( stval.kazeiu, stval.kazeiu, Asval.nuki );
			l_add( stval.komiur, stval.komiur, Asval.knuki );
			l_add( stval.kkomur, stval.kkomur, Asval.komi );
			
			// 旧税率集計
			l_add( stval.kazeiu_3, stval.kazeiu_3, Asval.nuki3 );
			l_add( stval.komiur_3, stval.komiur_3, Asval.knuki3 );
			l_add( stval.kkomur_3, stval.kkomur_3, Asval.komi3 );
			
			// 非課税分
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}
	}

	if( Vol1.APNO == 0x52 || Vol1.APNO == 0x53 || Vol1.APNO == 0x59 )	{	// 社会福祉(52)・新社会福祉(53)・宗教法人(59)
			//		I.   土地等譲渡収入
		if( ! key_set( foreA, 0x02, 0x0C, 0, 0x00 ) ) {
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}		
		//		J.   有価証券譲渡収入の　５％
		if( ! key_set( foreA, 0x02, 0x0B, 0, 0x00 ) ) {
			memset( WORK0, '\0', 6 );
			l_add( WORK0, Asval.yuka, Asval.yuka3 ); 
			l_add( stval.hikazu, stval.hikazu, WORK0 );

#ifdef	VERSIONUP
			// 特定収入集計
			if( m_SpcSg ) {
				memmove( stval.SpYuka, Asval.hika, 6 );		// 全額
				memmove( stval.SpYuk5, WORK0, 6 );			// ５％
			}
#endif

		}
	}
	else	{
		//		I.   土地等譲渡収入
		if( ! key_set( foreA, 0x11, 0x04, 0, 0x01 ) ) {
			l_add( stval.hikazu, stval.hikazu, Asval.hika );
		}		
		//		J.   有価証券譲渡収入の　５％
		if( ! key_set( foreA, 0x11, 0x03, 0, 0x01 ) ) {
			memset( WORK0, '\0', 6 );
			l_add( WORK0, Asval.yuka, Asval.yuka3 ); 
			l_add( stval.hikazu, stval.hikazu, WORK0 );

#ifdef	VERSIONUP
			// 特定収入集計
			if( m_SpcSg ) {
				memmove( stval.SpYuka, Asval.hika, 6 );		// 全額
				memmove( stval.SpYuk5, WORK0, 6 );			// ５％
			}
#endif

		}
	}

	//		O.	非課税　返品・値引
	if( ! key_set( foreA, 0x02, 0x01, 0, 0 ) ) {		// 売上値引き
		memmove( stval.hnebik, Asval.hika, 6 );			// O.
	}
	
	// 消費税額計算書　出力用集計
	// 税抜き処理　金額集計

	// 課税標準額
	memset( WORK0, '\0', 6 );
	memset( PW0, '\0', 6 );		l_input( PW0, "1030" );
	pArithEx->l_par100((unsigned char *)WORK0, (unsigned char *)stval.kkomur_3,
												(unsigned char *)PW0, 1, 0 );
	l_add( stval.kazeiu_3, stval.kazeiu_3, WORK0 );
	memset( WORK0, '\0', 6 );
	memset( PW0, '\0', 6 );		l_input( PW0, "1050" );
	pArithEx->l_par100((unsigned char *)WORK0, (unsigned char *)stval.kkomur,
												(unsigned char *)PW0, 1, 0 );
	l_add( stval.kazeiu, stval.kazeiu, WORK0 );

	// 税抜き売上値引き
	// N.   課税　  返品・値引
	if( ! key_set( foreA, 0x02, 0x01, 0, 0 ) ) {		// 売上値引き
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		percent( (char *)stval.henkan_3, (char *)Asval.nuki3, 1, 30, 0 );	// 30 / 1030
		percent( (char *)WORK0, (char *)Asval.komi3,  0, 30, 0 );			// 30 / 1000
		l_add( stval.henkan_3, stval.henkan_3, WORK0 );
				
		// 税込み売上値引
		l_add( stval.knebik_3, Asval.nuki3 , Asval.komi3 );
		l_sub( stval.knebik_3, stval.knebik_3, WORK0 );

		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		percent( (char *)stval.henkan, (char *)Asval.nuki, 1, Asval.pj_zino, 0 );	// 50 / 1050
		percent( (char *)WORK0, (char *)Asval.komi,  0, Asval.pj_zino, 0 );			// 50 / 1000
		l_add( stval.henkan, stval.henkan, WORK0 );
		
		// 税込み売上値引
		l_add( stval.knebik, Asval.nuki, Asval.komi );
		l_sub( stval.knebik, stval.knebik, WORK0 );				
	}
	// 貸倒損失 
	if( ! key_set( foreA, 0x08, 0x03, 0, 0 ) ) {
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		percent( (char *)stval.ksidao_3, (char *)Asval.nuki3, 1, 30, 0 );	// 30 / 1030
		percent( (char *)WORK0, (char *)Asval.komi3,  0, 30, 0 );			// 30 / 1000
		l_add( stval.ksidao_3, stval.ksidao_3, WORK0 );

		// 税込み貸倒損失
		l_add( stval.komksi_3, Asval.nuki3, Asval.komi3 );
		l_sub( stval.komksi_3, stval.komksi_3, WORK0 );

		l_sub( WORK0, WORK0, WORK1 );
		l_add( stval.ksidao_3, stval.ksidao_3, WORK0 );
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		percent( (char *)stval.ksidao, (char *)Asval.nuki, 1, Asval.pj_zino, 0 );	// 50 / 1050
		percent( (char *)WORK0, (char *)Asval.komi,  0, Asval.pj_zino, 0 );			// 50 / 1000
		l_add( stval.ksidao, stval.ksidao, WORK0 );
		// 税込み貸倒損失
		l_add( stval.komksi, Asval.nuki, Asval.komi );
		l_sub( stval.komksi, stval.komksi, WORK0 );
	}


	// 貸倒回収
	if( ! key_set( foreA, 0x02, 0x05, 0, 0 ) ) {
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		percent( (char *)stval.kaishu_3, (char *)Asval.nuki3, 1, 30, 0 );	// 30 / 1030
		percent( (char *)WORK0, (char *)Asval.komi3,  0, 30, 0 );			// 30 / 1000
		l_add( stval.kaishu_3, stval.kaishu_3, WORK0 );
		// 税込み貸倒回収
		l_add( stval.komksy_3, Asval.nuki3, Asval.komi3 );
		l_sub( stval.komksy_3, stval.komksy_3, WORK0 );


		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		percent( (char *)stval.kaishu, (char *)Asval.nuki, 1, Asval.pj_zino, 0 );	// 50 / 1050
		percent( (char *)WORK0, (char *)Asval.komi,  0, Asval.pj_zino, 0 );			// 50 / 1000
		l_add( stval.kaishu, stval.kaishu, WORK0 );
		// 税込み貸倒回収
		l_add( stval.komksy, Asval.nuki, Asval.komi );
		l_sub( stval.komksy, stval.komksy, WORK0 );
	}

	// 課税売上高割合金額
	
	// Q. = A + G - N   分子
	l_add( stval.kwbuns, stval.kazeiu, stval.kazeiu_3 );
	l_add( stval.kwbuns, stval.kwbuns, stval.exptur );
	l_add( stval.kwbuns, stval.kwbuns, stval.expjyo );		// 非課税輸出売上　★[14'01.17]
	l_sub( stval.kwbuns, stval.kwbuns, stval.knebik );
	l_sub( stval.kwbuns, stval.kwbuns, stval.knebik_3 );
	
	// R. = D + M - P   分母
	l_add( stval.kwbunb, stval.kwbuns, stval.hikazu );
	l_sub( stval.kwbunb, stval.kwbunb, stval.hnebik );
	
	// 分子×１００００／分母
	// 課税売上高割合
	pArithEx->l_par100( (unsigned char *)stval.uwriai, (unsigned char *)stval.kwbuns, (unsigned char *)stval.kwbunb, 2, 0 );

	// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
	memset( PW6, '\0', sizeof( PW6 ) );
	if( l_cmp( stval.kwbuns, PW6 ) < 0 )	memset( stval.uwriai, '\0', 6 );
	if( l_cmp( stval.kwbunb, PW6 ) < 0 )	memset( stval.uwriai, '\0', 6 );

#ifdef	VERSIONUP
	// 特定収入集計
	if( m_SpcSg ) {
		memmove( stval.SpKuws, stval.kwbuns, 6 );			// 課税売上高割合（分子）
		memmove( stval.SpKuwb, stval.kwbunb, 6 );			// 〃　　　　　　（分母）
		memmove( stval.SpUwri, stval.uwriai, 6 );			// 課税売上高割合

		memmove( stval.SpUnb3, stval.knebik_3, 6 );			// 売上返還　３％
		memmove( stval.SpUnb4, stval.knebik, 6 );			// 〃　　　　４％
		l_add( stval.SpUnbg, stval.SpUnb3, stval.SpUnb4 );	// 〃　　　　合計

		l_add( stval.SpKzur, stval.kazeiu, stval.kazeiu_3 );// 課税売上（準売上）
		memmove( stval.SpMzur, stval.exptur, 6 );			// 免税売上

		memmove( stval.SpHkur, stval.hikazu, 6 );			// 非課税売上（有価証券を全額で含む）
		l_sub( stval.SpHkur, stval.SpHkur, stval.SpYuk5 );
		l_add( stval.SpHkur, stval.SpHkur, stval.SpYuka );
	}
#endif

	// 課税売上高の５億超えチェック
	memset( m_CnvVal, '\0', 6 );
	memset( m_KzuVal, '\0', 6 );
	l_add( m_KzuVal, stval.kazeiu, stval.exptur );
	int St = YearConvert( m_CnvVal, m_KzuVal );
	if( m_Check ) return( St );
	else	{
//★//[12'02.23]///
		if( m_Compare == 0 ) {
			if( St == 1 && m_TaxReforms )	{	// ９５％ルール改正対応期間で５億円超え
				memset( RatioAsc, '\0', 6 );
				strcpy_s( RatioAsc, sizeof( RatioAsc ), "11000" );
				RatioInt = 11000;
			}
			else	{			// ４億５千万以上で５億以下又は４億５千万未満
				memset( RatioAsc, '\0', 6 );
				strcpy_s( RatioAsc, sizeof( RatioAsc ), "9500" );
				RatioInt = 9500;
			}
		}
		else	{
			if( m_Calcusw ) {
				memset( RatioAsc, '\0', 6 );
				strcpy_s( RatioAsc, sizeof( RatioAsc ), "11000" );
				RatioInt = 11000;
			}
			else	{
				memset( RatioAsc, '\0', 6 );
				RatioInt = 0;
			}
		}
///////////////////
	}
	// 課税売上割合　0％チェック
	Snd.Sn_EXP100 = 0x00;

	l_input( PW0, "1" );
	if( l_cmp( stval.uwriai, PW0 ) >= 0 || Anbun ) ;
	else {
		// ０％で計算する
		memset( stval.uwriai, '\0', 6 );
	}
	
	// 税込み(仕入)処理　金額集計
	// 課税商品仕入高
	// 商品仕入高 ＋ 営業経費
	if( ! key_set( foreA, 0x04, 0, 0, 0 ) ) {				// 仕入高
		memmove( stval.komisi,   Asval.knuki,  6 );
		memmove( stval.nukisi,   Asval.nuki,   6 );
		memmove( stval.kkomsi,   Asval.komi,   6 );
		memmove( stval.komisi_3, Asval.knuki3, 6 );
		memmove( stval.nukisi_3, Asval.nuki3,  6 );
		memmove( stval.kkomsi_3, Asval.komi3,  6 );
	}
	
	if( ! key_set( foreA, 0x08, 0, 0, 0 ) )	{				// 経費
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	// ＋ 営業外費用 ＋ 特別損失
	if( ! key_set( foreA, 0x08, 0x01, 0, 0 ) ) {			// 営業費用
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	if( ! key_set( foreA, 0x08, 0x02, 0, 0 ) ) {			// 特別損失
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	// ＋ 固定資産取得
	if( ! key_set( foreA, 0x11, 0x02, 0, 0 ) ) {			// 固定資産取得
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	
	// ＋ 流動資産取得
	if( ! key_set( foreA, 0x01, 0x00, 0, 0 ) ) {			// 流動資産取得
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}

	// ＋ 投資有価証券　取得
	if( ! key_set( foreA, 0x11, 0x03, 0, 0 ) ) {
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}
	// ＋ 土地　取得
	if( ! key_set( foreA, 0x11, 0x04, 0, 0 ) ) {
		l_add( stval.komisi,   stval.komisi,   Asval.knuki );
		l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
		l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
		l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
		l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
		l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
	}

	if( Vol1.APNO == 0x52 || Vol1.APNO == 0x53 || Vol1.APNO == 0x59 )	{	// 社会福祉(52)・新社会福祉(53)・宗教法人(59)
		// ＋ 投資有価証券　取得
		if( ! key_set( foreA, 0x04, 0x08, 0, 0 ) ) {
			l_add( stval.komisi,   stval.komisi,   Asval.knuki );
			l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
			l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
			l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
			l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
			l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
		}
		// ＋ 土地　取得
		if( ! key_set( foreA, 0x04, 0x09, 0, 0 ) ) {
			l_add( stval.komisi,   stval.komisi,   Asval.knuki );
			l_add( stval.nukisi,   stval.nukisi,   Asval.nuki );
			l_add( stval.kkomsi,   stval.kkomsi,   Asval.komi );
			l_add( stval.komisi_3, stval.komisi_3, Asval.knuki3 );
			l_add( stval.nukisi_3, stval.nukisi_3, Asval.nuki3 );
			l_add( stval.kkomsi_3, stval.kkomsi_3, Asval.komi3 );
		}
	}
	
	// 税込み商品仕入高
	memset( WORK0, '\0', 6 );
	l_add( stval.komisi, stval.kkomsi, stval.nukisi );
	percent( WORK0, stval.nukisi, 1, 0, 0 );
	l_add( stval.komisi, stval.komisi, WORK0 );

	memset( WORK0, '\0', 6 );
	l_add( stval.komisi_3, stval.kkomsi_3, stval.nukisi_3 );
	percent( WORK0, stval.nukisi_3, 1, 30, 0 );
	l_add( stval.komisi_3, stval.komisi_3, WORK0 );

#ifdef	VERSIONUP
	// 特定収入集計
	if( m_SpcSg ) {
		memmove( stval.SpKsi3, stval.komisi_3, 6 );				// 旧税率分・税込み仕入
		memmove( stval.SpKsi4, stval.komisi,   6 );				// 新税率分・税込み仕入
		l_add( stval.SpKsig, stval.komisi_3, stval.komisi );	// 合計　　・税込み仕入	
	}
#endif

	// 商品仕入値引高
	if( ! key_set( foreA, 0x04, 0x01, 0, 0 ) ) {		// 仕入値引き
		memset( WORK1, '\0', 6 );
		percent( (char *)stval.sihenk, (char *)Asval.knuki, 0, Asval.pj_zino, 0 );	// 50 / 1050
		percent( (char *)WORK1, 	   (char *)Asval.komi,  0, Asval.pj_zino, 0 );	// 50 / 1050
		l_sub( WORK1, WORK1, stval.sihenk );
		percent( (char *)stval.sihenk, (char *)Asval.knuki, 4, 40, 0 );				// 40 / 1050
		
		// 税込み仕入値引
		memset( WORK1, '\0', 6 );
		percent( (char *)WORK1, (char *)Asval.nuki, 1, Asval.pj_zino, 0 );	// 50 / 1050

		l_add( stval.sinebk, Asval.nuki,   Asval.komi );
		l_add( stval.sinebk, stval.sinebk, WORK1 );
				
		memset( WORK1,'\0', 6 );
		percent( (char *)stval.sihenk_3, (char *)Asval.knuki3, 0, 30, 0 );	// 30 / 1000
		percent( (char *)WORK1, (char *)Asval.komi3,  0, 30, 0 );			// 30 / 1000
		l_sub( WORK1, WORK1, stval.sihenk_3 );
		
		// 税込み仕入値引
		memset( WORK1,'\0', 6 );
		percent( (char *)WORK1, (char *)Asval.nuki3,  1, 30, 0 );			// 30 / 1000
		l_add( stval.sinebk_3, Asval.nuki3,    Asval.komi3 );
		l_add( stval.sinebk_3, stval.sinebk_3, WORK1 );
	}

#ifdef	VERSIONUP
	// 特定収入集計
	if( m_SpcSg ) {
		memmove( stval.SpKsb3, stval.sinebk_3, 6 );				// 旧税率分・税込み仕入値引
		memmove( stval.SpKsb4, stval.sinebk,   6 );				// 新税率分・税込み仕入値引
		l_add( stval.SpKsbg, stval.sinebk_3, stval.sinebk );	// 合計　　・税込み仕入値引	
	}
#endif

	// 申告書形式計算
	// 課税標準額
	l_calq( WORK1, stval.kazeiu,   0 );	// 1000円未満切り捨て
	l_calq( WORK0, stval.kazeiu_3, 0 );	// 1000円未満切り捨て
	
	// 課税標準に係る　消費税額
	percent( (char *)stval.urisyz,   (char *)WORK1, 1, 40, 0 );
	percent( (char *)stval.urisyz_3, (char *)WORK0, 1, 30, 0 );
	
	// 返還等対価に係る税額
	percent( (char *)stval.henkan,   (char *)stval.knebik,   1, 40, 0 );
	percent( (char *)stval.henkan_3, (char *)stval.knebik_3, 1, 30, 0 );
	
	// 貸倒損失に係る税額
	percent( (char *)stval.ksidao,   (char *)stval.komksi,   1, 40, 0 );
	percent( (char *)stval.ksidao_3, (char *)stval.komksi_3, 1, 30, 0 );
	
	// 貸倒回収に係る税額
	percent( (char *)stval.kaishu,   (char *)stval.komksy,   1, 40, 0 );
	percent( (char *)stval.kaishu_3, (char *)stval.komksy_3, 1, 30, 0 );
	
	// 税込み課税標準控除対象仕入税額////////+
	percent( (char *)stval.sirsyz,   (char *)stval.komisi,   4, 40, 0 );
	percent( (char *)stval.sirsyz_3, (char *)stval.komisi_3, 0, 30, 0 );

	// 税込み対応消費税 － 仕入値引分
	percent( (char *)WORK1, (char *)stval.sinebk,   4, 40, 0 );
	l_sub( stval.sirsyz,   stval.sirsyz,   WORK1 );
	percent( (char *)WORK1, (char *)stval.sinebk_3, 0, 30, 0 );
	l_sub( stval.sirsyz_3, stval.sirsyz_3, WORK1 );
	
	// 税込み仕入 －＝ 仕入値引
	l_sub( stval.komisi,   stval.komisi,   stval.sinebk );
	l_sub( stval.komisi_3, stval.komisi_3, stval.sinebk_3 );

	// 経過措置有無チェック
	if( Ver == 2 ) {
		if( Kani < 2 ) {		// 原則課税
			if( l_test( stval.kazeiu_3 ) || l_test( stval.urisyz_3 ) ||
				l_test( stval.knebik_3 ) || l_test( stval.komksi_3 ) ||
				l_test( stval.komisi_3 ) || l_test( stval.sirsyz_3 ) ||
				l_test( stval.komksy_3 ) || l_test( stval.IpStkz_3 ) ||
				l_test( stval.IpStze_3) )	TBhead.SVmzsw = 0x01;
			else							TBhead.SVmzsw = 0;
		}
		else {					// 簡易課税
			if( l_test( stval.kazeiu_3 ) || l_test( stval.urisyz_3 ) ||
				l_test( stval.knebik_3 ) || l_test( stval.komksy_3 ) ||
				l_test( stval.komksi_3 ) || l_test( stval.IpStkz_3 ) ||
				l_test( stval.IpStze_3) )	TBhead.SVmzsw = 0x01;
			else							TBhead.SVmzsw = 0;
		}
	}

//★//[12'01.19]///
	TBhead.SVmzsw = 0x01;
///////////////////

	// 課税売上高割合
	Kobet = 0;
	l_input( PW0, RatioAsc );
	memset( WORK0, '\0', 6 );
//★//[12'02.23]///
	int	m_Junction = 0;
	if( m_Compare == 0 )	{
		if( l_cmp( stval.uwriai, PW0 ) >= 0 )	m_Junction = 1;
	}
	else	{
		// シミュレーションする
		if(  m_Calcusw == 0 )					m_Junction = 1;
	}
	if( m_Junction )	{	// 課税売上割合９５％以上
///////////////////
		Srx = 0;
		// 控除対象仕入税額
		memmove( stval.koujo0,   stval.sirsyz, 6 );
		memmove( stval.koujo0_3, stval.sirsyz_3, 6 );
		strcpy_s( WORK0, sizeof( WORK0 ), "10000" );
	}
	else {									// 課税売上割合９５％以下
		Srx = 1;
		if( ! Kani && Anbun ) {		// 原則課税
			char	KZei4[6], KZei3[6], CZei4[6], CZei3[6];
			// 税額計算
			memset( KZei4, '\0', 6 );
			memset( KZei3, '\0', 6 );
			memset( CZei4, '\0', 6 );
			memset( CZei3, '\0', 6 );
			if( skb_calq( sc, tc, KZei4, KZei3, CZei4, CZei3 ) )	return( -1 );
			// ４％
			// 税売上に係る
			l_add( stval.kauriz,   stval.kauriz,   KZei4 );
			// 共通に係る
			l_add( stval.kyuriz,   stval.kyuriz,   CZei4 );

			// ３％
			// 税売上に係る
			l_add( stval.kauriz_3, stval.kauriz_3, KZei3 );
			// 共通に係る
			l_add( stval.kyuriz_3, stval.kyuriz_3, CZei3 );

			// 共売に係る控除対象仕入税額x課税売上割合(%) 小数点以下第五位迄
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)stval.kwbuns, (unsigned char *)stval.kwbunb, 5, 0 );

			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof( PW6 ) );
			if( l_cmp( stval.kwbuns, PW6 ) < 0 )	memset( WORK0, '\0', 6 );
			if( l_cmp( stval.kwbunb, PW6 ) < 0 )	memset( WORK0, '\0', 6 );

			pArithEx->l_mul100( (unsigned char *)WORK1, (unsigned char *)stval.kyuriz, (unsigned char *)WORK0, 5, 0 );
			l_add( stval.koujo0, stval.kauriz, WORK1 );
			
			pArithEx->l_mul100( (unsigned char *)WORK1, (unsigned char *)stval.kyuriz_3, (unsigned char *)WORK0, 5, 0 );
			l_add( stval.koujo0_3, stval.kauriz_3, WORK1 );
			Kobet = 1;
		}
		else {		// 控除対象仕入税額 （一括比例配分方式）
			memset( WORK0, '\0', 6 );
			// 控除対象仕入税額x課税売上割合(%) 小数点以下第五位迄
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)stval.kwbuns, (unsigned char *)stval.kwbunb, 5, 0 );

			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof( PW6 ) );
			if( l_cmp( stval.kwbuns, PW6 ) < 0 )	memset( WORK0, '\0', 6 );
			if( l_cmp( stval.kwbunb, PW6 ) < 0 )	memset( WORK0, '\0', 6 );

			pArithEx->l_mul100( (unsigned char *)stval.koujo0,   (unsigned char *)stval.sirsyz,   (unsigned char *)WORK0, 5, 0 );
			pArithEx->l_mul100( (unsigned char *)stval.koujo0_3, (unsigned char *)stval.sirsyz_3, (unsigned char *)WORK0, 5, 0 );
		}
	}
	
	// 控除税額合計１
	l_add( stval.kjgok0, stval.koujo0, stval.henkan );
	l_add( stval.kjgok0, stval.kjgok0, stval.ksidao );
	l_add( stval.kjgok0_3, stval.koujo0_3, stval.henkan_3 );
	l_add( stval.kjgok0_3, stval.kjgok0_3, stval.ksidao_3 );
	// 差引納付税額１
	l_add( stval.nouhu0, stval.urisyz, stval.kaishu );
	l_sub( stval.nouhu0, stval.nouhu0, stval.kjgok0 );
	l_add( stval.nouhu0_3, stval.urisyz_3, stval.kaishu_3 );
	l_sub( stval.nouhu0_3, stval.nouhu0_3, stval.kjgok0_3 );
	
	// 新旧合計
	l_add( stval.koujo0, stval.koujo0, stval.koujo0_3 );
	l_add( stval.kjgok0, stval.kjgok0, stval.kjgok0_3 );
	l_add( stval.nouhu0, stval.nouhu0, stval.nouhu0_3 );
	
	if( ! Kenei ) {	// 原則課税 or 簡易課税・単一業者
		// 簡易課税　税額計算
		l_input( PW1, "1000" );
		if( Kani >= 2 )	{	// 簡易課税・単一事業者選択時
			Kenei |= 0x02;	// 兼営事業ＳＷ　ON
			goto KENEI;
		}
	}
	else {		// 兼営事業者

KENEI:
		// 事業別売上税額	&　控除対象仕入税額
		l_input( PW0, "1000" );
		l_input( PW5, "750" );		// 特例計算適用チェック
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		memset( WORK2, '\0', 6 );	memset( WORK3, '\0', 6 );
		memset( Kenuri, '\0', 6 );	memset( Kenuri_n, '\0', 6 );
		
		// 事業別課税売上高
		if( ! pTblHdl->th_rget( &Bsval, foreB, 3 ) )	{	// 第１業種 3%
			l_add( stval.knuri1, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri1_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 11 ) )	// 第１業種 3%
				l_add( stval.knuri1, stval.knuri1, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) ) {	// 第１業種 4.5%
				l_add( stval.knuri1, stval.knuri1, Bsval.su_nuki );
				l_add( stval.knuri1, stval.knuri1, Bsval.su_nuks );
			}
		}
		
		if( ! pTblHdl->th_rget( &Bsval, foreB, 4 ) )	{	// 第２業種 3%
			l_add( stval.knuri2, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri2_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 12 ) )		// 第２業種 3%
				l_add( stval.knuri2, stval.knuri2, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 8 ) ) {		// 第２業種 4.5%
				l_add( stval.knuri2, stval.knuri2, Bsval.su_nuki );
				l_add( stval.knuri2, stval.knuri2, Bsval.su_nuks );
			}
		}
		
		if( ! pTblHdl->th_rget( &Bsval, foreB, 5 ) )	{	// 第３業種 3%
			l_add( stval.knuri3, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri3_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 13 ) )		// 第３業種 3%
				l_add( stval.knuri3, stval.knuri3, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 9 ) ) {	// 第３業種 4.5%
				l_add( stval.knuri3, stval.knuri3, Bsval.su_nuki );
				l_add( stval.knuri3, stval.knuri3, Bsval.su_nuks );
			}
		}
		
		if( ! pTblHdl->th_rget( &Bsval, foreB, 6 ) )	{	// 第４業種 3%
			l_add( stval.knuri4, Bsval.su_nuki, Bsval.su_nuki3 );
			if( TBhead.SVvers )	memmove( stval.knuri4_n, Bsval.su_nuki, 6 );
		}
		if( TBhead.SVvers ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 14 ) )		// 第４業種 3%
				l_add( stval.knuri4, stval.knuri4, Bsval.su_nuki3 );
		}
		else	{
			if( ! pTblHdl->th_rget( &Bsval, foreB, 10 ) ) {	// 第４業種 4.5%
				l_add( stval.knuri4, stval.knuri4, Bsval.su_nuki );
				l_add( stval.knuri4, stval.knuri4, Bsval.su_nuks );
			}
		}
		
		if( TBhead.SVvers && ((Ver&0xff) == 0x02) ) {
			if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) )		{	// 第５業種 3%
				l_add( stval.knuri5, Bsval.su_nuki, Bsval.su_nuki3 );
				memmove( stval.knuri5_n, Bsval.su_nuki, 6 );
			}
			if( ! pTblHdl->th_rget( &Bsval, foreB, 15 ) )		// 第５業種 3%
				l_add( stval.knuri5, stval.knuri5, Bsval.su_nuki3 );
		}

		// 事業別課税売上高合計
		// マイナスの場合は、合計に加えない（０円とする）
		if( l_test( stval.knuri1 ) < 0 )	l_input( stval.knuri1, "0" );
		if( l_test( stval.knuri2 ) < 0 )	l_input( stval.knuri2, "0" );
		if( l_test( stval.knuri3 ) < 0 )	l_input( stval.knuri3, "0" );
		if( l_test( stval.knuri4 ) < 0 )	l_input( stval.knuri4, "0" );
		if( l_test( stval.knuri5 ) < 0 )	l_input( stval.knuri5, "0" );
		if( l_test( stval.knuri1_n ) < 0 )	l_input( stval.knuri1_n, "0" );
		if( l_test( stval.knuri2_n ) < 0 )	l_input( stval.knuri2_n, "0" );
		if( l_test( stval.knuri3_n ) < 0 )	l_input( stval.knuri3_n, "0" );
		if( l_test( stval.knuri4_n ) < 0 )	l_input( stval.knuri4_n, "0" );
		if( l_test( stval.knuri5_n ) < 0 )	l_input( stval.knuri5_n, "0" );
		
		// 事業別課税売上高合計(縦計ではなく、全体の課税売上高) 
		l_add( Kenuri,   Kenuri,   stval.knuri1 );
		l_add( Kenuri,   Kenuri,   stval.knuri2 );
		l_add( Kenuri,   Kenuri,   stval.knuri3 );
		l_add( Kenuri,   Kenuri,   stval.knuri4 );
		l_add( Kenuri,   Kenuri,   stval.knuri5 );
		l_add( Kenuri_n, Kenuri_n, stval.knuri1_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri2_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri3_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri4_n );
		l_add( Kenuri_n, Kenuri_n, stval.knuri5_n );

		// 事業別売上発生チェック
		tksg = 0;
		if( l_test( stval.knuri1 ) )	tksg++;
		if( l_test( stval.knuri2 ) )	tksg++;
		if( l_test( stval.knuri3 ) )	tksg++;
		if( l_test( stval.knuri4 ) )	tksg++;
		if( l_test( stval.knuri5 ) )	tksg++;
		
		if( l_test( stval.knuri1 ) ) {		// 卸売
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x01;	++kensg;
			if( tksg == 1 ) {		// 単一事業
				memmove( stval.knuri1, Kenuri,   6 );
				memmove( stval.knuri1_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri_n, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 3 ) )	{	// 第１業種 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 11 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) )		// 第１業種 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			// 課税売上に係る税額
			percent( (char *)stval.knzei1_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
			percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
			l_add( stval.knzei1, stval.knzei1_n, WORK1 );
			
			l_6calq( (char *)stval.knwai1, (char *)stval.knuri1, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai1_n, (char *)stval.knuri1_n, PW0, Kenuri_n );
			if( l_cmp( stval.knwai1, PW5 ) >= 0 )	toku |= 0x01;	// 75%以上
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[0] ) );
			l_input( PW3, WORK2 );
			l_input( PW4, WORK2 );
			l_6calq( (char *)stval.koujo1,    (char *)stval.knzei1,   PW3, PW0 );	// 原則
			l_6calq( (char *)stval.koujo11,   (char *)stval.knzei1,   PW3, PW0 );	// 特例
			
			if( Kenei & 0x02 )	goto KEN4;		// 単一事業 & 資産譲渡有
		}
		
		if( l_test( stval.knuri2 ) ) {		// 小売
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x02;
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri2,   Kenuri,   6 );
				memmove( stval.knuri2_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 4 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 12 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 8 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			// 課税売上に係る税額
			percent( (char *)stval.knzei2_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
			percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
			l_add( stval.knzei2, stval.knzei2_n, WORK1 );
			
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[1] ) );
			l_6calq( (char *)stval.knwai2, (char *)stval.knuri2, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai2_n, (char *)stval.knuri2_n, PW0, Kenuri_n );
			if( ! toku ) {	// 特例なし
				if( kensg ) {	// 第１業種残高有り
					l_add( WORK3, stval.knuri1, stval.knuri2 );	// (1)+(2)
					l_6calq( WORK3, WORK3, PW0, Kenuri );	// チェック
				}
				else	l_input( WORK3, "0" );
				if( (l_cmp( stval.knwai2, PW5 ) >= 0) ||	// 75%以上
					(l_cmp( WORK3, PW5 ) >= 0) ) {
					if( l_test( WORK3 ) )	toku |= 0x12;
					else					toku |= 0x02;
				}
				l_input( PW3, WORK2 );
			}
			l_input( PW4, WORK2 );	++kensg;
			l_6calq( (char *)stval.koujo2,    (char *)stval.knzei2,   PW4, PW0 );	// 原則計算
			l_6calq( (char *)stval.koujo12,   (char *)stval.knzei2,   PW3, PW0 );	// 特例計算
			if( Kenei & 0x02 )	goto KEN4;			// 単一事業 & 資産譲渡有
		}
		
		if( l_test( stval.knuri3 ) ) {		// 製造
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x04;
			
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri3,   Kenuri,   6 );
				memmove( stval.knuri3_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 5 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 13 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 9 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			// 課税売上に係る税額
			percent( (char *)stval.knzei3_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
			percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
			l_add( stval.knzei3, stval.knzei3_n, WORK1 );
			
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[2] ) );
			l_6calq( (char *)stval.knwai3, (char *)stval.knuri3, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai3_n, (char *)stval.knuri3_n, PW0, Kenuri_n );
			if( ! toku ) {	// 特例無し
				l_input( WORK3, "0" );	l_input( WORK4, "0" );
				if( kensg ) {	// 第１ or 第２業種残高有り
					if( l_test( stval.knuri1 ) ) {
						l_add( WORK3, stval.knuri3, stval.knuri1 );
						l_6calq( WORK3, WORK3, PW0, Kenuri );
					}
					if( l_test( stval.knuri2 ) ) {
						l_add( WORK4, stval.knuri3, stval.knuri2 );
						l_6calq( WORK4, WORK4, PW0, Kenuri );
					}
					if( l_cmp( WORK3, PW5 ) >= 0 )	toku |= 0x14;
					else	l_input( WORK3, "0" );
					if( l_cmp( WORK4, PW5 ) >= 0 )	toku |= 0x24;
					else	l_input( WORK4, "0" );
				}
				else {
					if( l_cmp( stval.knwai3, PW5 ) >= 0 )	toku |= 0x04;
				}
				l_input( PW3, WORK2 );
			}
			l_input( PW4, WORK2 );	++kensg;
			l_6calq( (char *)stval.koujo3,    (char *)stval.knzei3,   PW4, PW0 );	// 原則
			l_6calq( (char *)stval.koujo13,   (char *)stval.knzei3,   PW3, PW0 );	// 特例
			if( l_test( WORK3 ) || l_test( WORK4 ) ) {	// 特例の判定
				if( l_test( WORK3 ) ) {
					l_6calq( WORK3, (char *)stval.knzei2, PW3, PW0 );
					l_add( WORK3, WORK3, stval.koujo1 );	// 第1+第3
					l_add( WORK3, WORK3, stval.koujo13 );	// の特例
				}
				if( l_test( WORK4 ) ) {
					l_6calq( WORK4, (char *)stval.knzei1, PW3, PW0 );
					l_add( WORK4, WORK4, stval.koujo2 );
					l_add( WORK4, WORK4, stval.koujo13 );	// 第2+第3の特例
				}
				if( (l_cmp( WORK3, WORK4 )) >= 0 ) {	// 第1+第3の特例採用
					l_6calq( (char *)stval.koujo12, (char *)stval.knzei2, PW3, PW0 );
					toku &= 0x1f;
				}
				else {									// 第2+第3の特例採用
					l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
					toku &= 0x2f;
				}
			}
			if( Kenei & 0x02 )	goto KEN4;		// 単一事業 & 資産譲渡有
		}
KEN4:
		if( l_test( stval.knuri4 ) ) { 		// その他
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x08;
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri4,   Kenuri,   6 );
				memmove( stval.knuri4_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 6 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 14 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 10 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			// 課税売上に係る税額
			percent( (char *)stval.knzei4_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
			percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
			l_add( stval.knzei4, stval.knzei4_n, WORK1 );
			
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[3] ) );
			l_6calq( (char *)stval.knwai4, (char *)stval.knuri4, PW0, Kenuri );	// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai4_n, (char *)stval.knuri4_n, PW0, Kenuri_n );
			if( ! toku ) {
				l_input( WORK3, "0" );	l_input( WORK4, "0" );
				l_input( WORK5, "0" );
				if( kensg ) {	// 第１ or 第２ or 第３業種残高有り
					if( l_test( stval.knuri1 )) {		// 第1 + 第4
						l_add( WORK3, stval.knuri4, stval.knuri1 );
						l_6calq( WORK3, WORK3, PW0, Kenuri );
					}
					if( l_test( stval.knuri2 )) {		// 第2 + 第4
						l_add( WORK4, stval.knuri4, stval.knuri2 );
						l_6calq( WORK4, WORK4, PW0, Kenuri );
					}
					if( l_test( stval.knuri3 )) {		// 第3 + 第4
						l_add( WORK5, stval.knuri4, stval.knuri3 );
						l_6calq( WORK5, WORK5, PW0, Kenuri );
					}

					if( l_cmp( WORK3, PW5 ) >= 0 ) toku |= 0x18;
					else	l_input( WORK3, "0" );
					if( l_cmp( WORK4, PW5 ) >= 0 ) toku |= 0x28;
					else	l_input( WORK4, "0" );
					if( l_cmp( WORK5, PW5 ) >= 0 ) toku |= 0x48;
					else	l_input( WORK5, "0" );
				}
				else	{
					if( l_cmp( stval.knwai4, PW5 ) >= 0 ) toku |= 0x08;
				}
				l_input( PW3, WORK2 );	// 特例計算摘要なし
			}
			
			l_input( PW4, WORK2 );	++kensg;
			l_6calq( (char *)stval.koujo4,   (char *)stval.knzei4,   PW4, PW0 );	// 原則
			l_6calq( (char *)stval.koujo14,  (char *)stval.knzei4,   PW3, PW0 );	// 特例
			
			if( l_test( WORK3 ) || l_test( WORK4 ) || l_test( WORK5 ) ) {
				if( l_test( WORK3 ) ) {		// 特例の判定
					l_6calq( WORK1, (char *)stval.knzei2, PW3, PW0 );
					l_6calq( WORK3, (char *)stval.knzei3, PW3, PW0 );
					l_add( WORK3, WORK3, WORK1 );
					l_add( WORK3, WORK3, stval.koujo1 );
					l_add( WORK3, WORK3, stval.koujo14 );	// 第1＋第4の特例
				}
				if( l_test( WORK4 ) ) {
					l_6calq( WORK1, (char *)stval.knzei1, PW3, PW0 );
					l_6calq( WORK4, (char *)stval.knzei3, PW3, PW0 );
					l_add( WORK4, WORK4, WORK1 );
					l_add( WORK4, WORK4, stval.koujo2 );
					l_add( WORK4, WORK4, stval.koujo14 ); // 第2＋第4の特例
				}
				if( l_test( WORK5 ) ) {
					l_6calq( WORK1, stval.knzei1, PW3, PW0 );
					l_6calq( WORK5, stval.knzei2, PW3, PW0 );
					l_add( WORK5, WORK5, WORK1 );
					l_add( WORK5, WORK5, stval.koujo3 );
					l_add( WORK5, WORK5, stval.koujo14 ); // 第3＋第4の特例
				}
				
				if( (l_cmp( WORK3, WORK4 )) >= 0 ) { // 第1＋第4の特例採用
					if( (l_cmp( WORK3, WORK5 )) >= 0 ) {
						l_6calq( stval.koujo12, stval.knzei2, PW3, PW0 );
						l_6calq( stval.koujo13, stval.knzei3, PW3, PW0 );
						toku &= 0x1f;
					}
					else	{
						l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
						l_6calq( (char *)stval.koujo12, (char *)stval.knzei2, PW3, PW0 );
						toku &= 0x4f;
					}
				}
				else	{							// 第2＋第4の特例採用
					if( (l_cmp( WORK4, WORK5 )) >= 0 ) {
						l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
						l_6calq( (char *)stval.koujo13, (char *)stval.knzei3, PW3, PW0 );
						toku &= 0x2f;
					}
					else	{
						l_6calq( (char *)stval.koujo11, (char *)stval.knzei1, PW3, PW0 );
						l_6calq( (char *)stval.koujo12, (char *)stval.knzei2, PW3, PW0 );
						toku &= 0x4f;
					}
				}
			}
		}
		
		if( l_test( stval.knuri5 ) ) {		// サービス
			
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			memset( WORK2, '\0', 6 );
			ksg |= 0x10;
			if( tksg == 1 ) {	// 単一事業
				memmove( stval.knuri5,   Kenuri,   6 );
				memmove( stval.knuri5_n, Kenuri_n, 6 );
				memmove( WORK0, Kenuri, 6 );
				l_sub( WORK2, Kenuri, Kenuri_n );
			}
			else {				// 複数事業
				if( ! pTblHdl->th_rget( &Bsval, foreB, 7 ) )	{	// 3%
					memmove( WORK0, Bsval.su_nuki,  6 );
					memmove( WORK2, Bsval.su_nuki3, 6 );
				}
				if( TBhead.SVvers ) {
					if( ! pTblHdl->th_rget( &Bsval, foreB, 15 ) )
						l_add( WORK2, WORK2, Bsval.su_nuki3 );
				}
				else	{
					if( ! pTblHdl->th_rget( &Bsval, foreB, 11 ) )		// 4.5%
						l_add( WORK2, WORK2, Bsval.su_nuki );
				}
			}
			// 課税売上に係る税額
			percent( (char *)stval.knzei5_n, (char *)WORK0, 1, (TBhead.SVvers ? 40: 30), 0 );
			percent( (char *)WORK1, (char *)WORK2, 1, 30, 0 );
			l_add( stval.knzei5, stval.knzei5_n, WORK1 );
			
			sprintf_s( WORK2, sizeof( WORK2 ), "%d", ( 1000 - TBhead.SVsper[4] ) );
			l_6calq( (char *)stval.knwai5, (char *)stval.knuri5, PW0, Kenuri );		// 構成比
			if( l_test( Kenuri_n ) )
				l_6calq( (char *)stval.knwai5_n, (char *)stval.knuri5_n, PW0, Kenuri_n );
			if( !toku )	l_input( PW3, WORK2 );
			l_input( PW4, WORK2 );	++kensg;
			
			l_6calq( (char *)stval.koujo5,   (char *)stval.knzei5,   PW4, PW0 );	// 原則
			l_6calq( (char *)stval.koujo15,   (char *)stval.knzei5,   PW3, PW0 );	// 特例
		}
		else {
			l_clear( stval.knuri5 );	l_clear( stval.knuri5_n );
			l_clear( stval.knzei5 );	l_clear( stval.knzei5_n );
			l_clear( stval.knwai5 );	l_clear( stval.knwai5_n );
		}

		switch( kensg ) {	// 特例計算適用可能チェック
			case 1 :	// 単一事業のみ発生
						toku = 0x00;	break;
			case 2 :	// 2事業合計で75%以上
						if( toku & 0xf0 )	toku = 0x00;	break;
			default:	break;
		}
	}
	
	// 申告書マスター計算
	Gsw = gen_chk( sno );

	stm_new( ksg );
	memmove( Stm.Kuri1, stval.knuri1, 6 );	// 第１種事業　課税売上
	memmove( Stm.Kuri2, stval.knuri2, 6 );	// 第２種事業　　 〃
	memmove( Stm.Kuri3, stval.knuri3, 6 );	// 第３種事業　　 〃
	memmove( Stm.Kuri4, stval.knuri4, 6 );	// 第４種事業　　 〃
	memmove( Stm.Kuri5, stval.knuri5, 6 );	// 第５種事業　　 〃
	Stm.Up1 = UP1;
	Stm.Up2 = UP2;
	Stm.Up3 = UP3;
	Stm.Up4 = UP4;
	Stm.Up5 = UP5;

	return( FALSE );
}

// 限界控除制度　適用期間チェック
int CShinCalculateW::gen_chk( int sno )
{
	int		i, ii, j, mcnt, mon;
	char	SYMD[4], EYMD[4], cmd[8], smd[4], emd[4];
	
	rMON = fMON = sMON = ii = j = 0;
	memset( cmd,  '\0', sizeof( cmd ) );
	memset( SYMD, '\0', sizeof( SYMD ) );
	memset( EYMD, '\0', sizeof( EYMD ) );


	switch( Skbn ) {
		case 1 :	case 3 :		// 確定・確定修正
			memmove( &SYMD[0], &Snh.Sn_KDAYS[0], 3 );
			memmove( &EYMD[0], &Snh.Sn_KDAYE[0], 3 );
			if( ((Vol1.APNO&0xff) == 0x10 && ! (Snh.Sn_KOANB&0x01)) ||
												(Snh.Sn_KOJIN&0x01) ) {
				SYMD[0] = 0x09;	SYMD[1] = 0x01;	SYMD[2] = 0x01;
				j = 1;
			}
			else if( Snh.Sn_KZTNSK&0x0f ) {	// [04'03.04]
				memmove( &SYMD[1], Vol2.D_TBL[sno]._SMD, 2 );
				if( SYMD[1] < Vol1.CS_YMD[1] )	SYMD[0] = e_ymd[0];
			}
			break;
		case 2 :	case 4 :	// 中間・中間修正
			memmove( &SYMD[0], &Snh.Sn_MDAYS[0], 3 );
			memmove( &EYMD[0], &Snh.Sn_MDAYE[0], 3 );
			break;
		default:
			ErrMessage = _T("申告区分が正しくありません。" );
			return( -1 );
			break;
	}
	
	// 決算期間のチェック
	// 限界控除制度廃止
	if( l_jsgn( Snh.Sn_KDAYS, GEN2, 3 ) >= 0 )	i = 0;
	else if( l_jsgn( Snh.Sn_KDAYE, GEN2, 3 ) >= 0 ) {	// 平成9年4月1日を含む
		i = 2;
		if( l_jsgn( EYMD, GEN2, 3 ) >= 0 )	ii = 1;
	}
	else if( l_jsgn( Snh.Sn_KDAYE, GEN1, 3 ) >= 0 ) {	// 平成8年4月1日を含む
		if( j && l_jsgn( Snh.Sn_KDAYS, SYMD, 3 ) >= 0 )	i = 2;
		else {
			i = 1;
			if( l_jsgn( EYMD, GEN1, 3 ) >= 0 )	ii = 1;
		}
	}
	else {
		SYMD[0] = 0x08;
		if( j && l_jsgn( Snh.Sn_KDAYS, SYMD, 3 ) >= 0 )	i = 1;
		else										i = -1;		// 現行法
	}
	
	if( i > 0 ) {
		if( j ) {
			fMON = 3;		rMON = 9;
			if( i == 2 )	sMON = 9;
			return( i );
		}
		if( l_jsgn( SYMD, (i == 1 ? GEN1 : GEN2), 3 ) < 0 ) {	// 4/1前より開始
			memset( smd, '\0', sizeof( smd ) );
			memset( emd, '\0', sizeof( emd ) );
			memset( cmd, '\0', sizeof( cmd ) );
			cmd[0] = 0x03;	cmd[1] = 0x31;
			memmove( smd, &SYMD[1], 2 );
			smd[0] &= 0x1f;
			for( mcnt = mon = 0, j = sno; j < 12; ++j, ++mcnt, ++mon ) {
				memmove( emd, Vol2.D_TBL[j]._EMD, 2 );
				// ３月３１日の存在するオフセットの取得
				if( (l_jsgn( smd, cmd, 2 ) < 0 && l_jsgn( emd, cmd, 2 ) >= 0)
											 || ! l_jsgn( emd, cmd, 2 ) ) {
					mcnt++;	mon++;
					if( ! l_jsgn( emd, cmd, 2 ) ) {	// 3/31
						if( (SYMD[1]&0x1f) != Vol2.D_TBL[sno]._EMD[0] )
							++mcnt;
					}
					else	--mon;
					break;
				}
				memmove( smd, Vol2.D_TBL[j]._EMD, 2 );
			}
			if( ! ii && (mcnt > Kazmon) )	fMON = Kazmon;
			else {
				if( (rMON = Kazmon - mcnt) < 0 )	rMON = 0;
				fMON = ( rMON ? mcnt : Kazmon );
			}
			if( i == 2 ) {
				if( ii || (mon <= Kazmon) ) {
					if( (sMON = Kazmon - mon) < 0 )	sMON = 0;
				}
			}
		}
		else	rMON = sMON = Kazmon;
		
		// 課税事業者選択届出書提出者のためのチェック
		if( i == 1 ) {	// 平成８年度改正のみ
			if( (Snd.Sn_MENZsw&0xff) == 0x10 ) {				// 当期より課税事業者
				if( l_cmp( Snd.Sn_ZZURI, TBhead.SVgen0 ) > 0 )	// 改正法
						i += 100;
				else	i += 10;								// 現行法
			}
		}
	}
	return( i );
}

// 改正法　限界控除計算
void CShinCalculateW::gencalq( char *gzei, char *gmae )
{
register
	int		i;
	char	GGEND1[6], GGEND2[6], wzei[6], WORK0[6], PW0[6], PW1[6];
	
	if( ! Gsw || (l_test( gmae ) < 0) )	{	// 限界控除制度廃止日又は還付
		l_clear( gzei );
		Gsw = -1;
	}
	else if( Gsw < 0 ) ;					// 現行法で計算
	else {
		for( i = 0; i < 3; ++i )	memset( Gkai_tb[i].aft, '\0', 6 );
		memset( wzei, '\0', sizeof( wzei ) );
		memset( PW0, '\0', sizeof( PW0 ) );
		memset( PW1, '\0', sizeof( PW1 ) );
		memset( GGEND1, '\0', sizeof( GGEND1 ) );
		memset( GGEND2, '\0', sizeof( GGEND2 ) );
		l_input( GGEND1, "100000" );	// 平成８年度改正限度額（10万円）
		l_input( GGEND2,  "80000" );	// 平成９値度改正限度額（８万円）
		
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", rMON );	// 適用日以後の月数
		l_input( PW0, WORK0 );
		l_input( PW1, "12" );
		memset( WORK0, '\0', sizeof( WORK0 ) );
		switch( Gsw ) {
			case 1 : case 101 :			// ８年度改正
						sprintf_s( WORK0, sizeof( WORK0 ), "%d", Kazmon );
						l_input( PW0, WORK0 );
						l_6calq( WORK0, GGEND1, PW0, PW1 );
						if( l_cmp( gzei, WORK0 ) <= 0 ) {
							Gsw = 11;	break;
						}
						sprintf_s( WORK0, sizeof( WORK0 ), "%d", rMON );	// 適用日以後の月数
						l_input( PW0, WORK0 );
						l_6calq( wzei, GGEND1, PW0, PW1 );
						memmove( Gkai_tb[1].aft, wzei, 6 );
						if( fMON ) {
							sprintf_s( WORK0, sizeof( WORK0 ), "%d", fMON );
							l_input( PW0, WORK0 );
							sprintf_s( WORK0, sizeof( WORK0 ), "%d", Kazmon );	// 課税期間(経過)月数
							l_input( PW1, WORK0 );
							l_6calq( WORK0, gzei, PW0, PW1 );
							memmove( Gkai_tb[0].aft, WORK0, 6 );
						}
						else	l_clear( WORK0 );
						l_add( gzei, wzei, WORK0 );
						memmove( Gkai_tb[2].aft, gzei, 6 );
						break;
			case 2 : case 102 : 		// ９年度改正
						l_6calq( wzei, GGEND2, PW0, PW1 );
						memmove( Gkai_tb[1].aft, wzei, 6 );
						if( fMON ) {
							sprintf_s( WORK0, sizeof( WORK0 ), "%d", fMON );
							l_input( PW0, WORK0 );
							l_6calq( WORK0, GGEND1, PW0, PW1 );
							memmove( Gkai_tb[0].aft, WORK0, 6 );
						}
						else	l_clear( WORK0 );
						l_add( wzei, wzei, WORK0 );
						memmove( Gkai_tb[2].aft, wzei, 6 );
						memmove( gzei, wzei, 6 );
						break;
			default :	break;
		}
	}
	if( Gsw != 2 && Gsw != 102 ) { 		// ９年度改正以外
		if( l_cmp( gmae, gzei ) < 0 )	memmove( gzei, gmae, 6 );
	}
}


// 消費税申告書用  旧 WORK AREA 転送
void CShinCalculateW:: so_read( void )
{
	memset( &Stm, '\0', sizeof( struct _STMVAL ) );
	memmove( Stm.Kstd,   Snd.Sn_KSTD,   6 );	// 課税標準額（その他）
	memmove( Stm.Tika,   Snd.Sn_TIKA,   6 );	// 対価の返還等をした金額:その他
	memmove( Stm.Sytx,   Snd.Sn_SYTX,   6 );	// 消費税額  （その他）
	memmove( Stm.Kjadjz, Snd.Sn_KJADJZ, 6 );	// 控除過大調整税額
	memmove( Stm.Sirez,  Snd.Sn_SIREZ,  6 );	// 控除対象仕入税額
	memmove( Stm.Menzz,  Snd.Sn_MENZZ,  6 );	// 免税控除対象仕入税額
	if( l_test( Stm.Menzz ) && (Snd.Sn_MENZsw&0x01) )	l_neg( Stm.Menzz );
	memmove( Stm.Hkanz,  Snd.Sn_HKANZ,  6 );	// 返還等対価に係る税額
	memmove( Stm.Ksitz,  Snd.Sn_KSITZ,  6 );	// 貸倒れに係る税額
	memmove( Stm.Tynofz, Snd.Sn_TYNOFZ, 6 );	// 中間納付税額
	memmove( Stm.Ednofz, Snd.Sn_EDNOFZ, 6 );	// 最終納付税額
	memmove( Stm.Kaktiz, Snd.Sn_KAKTIZ, 6 );	// 既確定税額
	memmove( Stm.Kzuri,  Snd.Sn_KZURI,  6 );	// 課税売上高
	memmove( Stm.Souri,  Snd.Sn_SOURI,  6 );	// 総売上
	memmove( Stm.Zzuri,  Snd.Sn_ZZURI,  6 );	// 基準期間の課税売上高
	memmove( Stm.Genkai, Snd.Sn_GENKAI, 6 );	// 限界控除税額
	switch( Snh.Sn_MCOUNT ) {
		case 3 :	// 年３回
			memmove( Stm.Tynof1, Snd.Sn_TYNF1,  6 );	// 中間納付税額１回目
			memmove( Stm.Tynof2, Snd.Sn_TYNF2,  6 );	// 中間納付税額２回目
			memmove( Stm.Tynof3, Snd.Sn_TYNF3,  6 );	// 中間納付税額３回目
			memmove( Stm.Tynof4, Snd.Sn_TYNF4,  6 );	// 中間納付税額４回目
			break;
		case 11:	// 年11回
			memmove( Stm.Tynof1, Snd.Sn_TYNF[0],  6 );	// 中間納付税額１回目
			memmove( Stm.Tynof2, Snd.Sn_TYNF[1],  6 );	// 中間納付税額２回目
			memmove( Stm.Tynof3, Snd.Sn_TYNF[2],  6 );	// 中間納付税額３回目
			memmove( Stm.Tynof4, Snd.Sn_TYNF[3],  6 );	// 中間納付税額４回目
			memmove( Stm.Tynof5, Snd.Sn_TYNF[4],  6 );	// 中間納付税額５回目
			memmove( Stm.Tynof6, Snd.Sn_TYNF[5],  6 );	// 中間納付税額６回目
			memmove( Stm.Tynof7, Snd.Sn_TYNF[6],  6 );	// 中間納付税額７回目
			memmove( Stm.Tynof8, Snd.Sn_TYNF[7],  6 );	// 中間納付税額８回目
			memmove( Stm.Tynof9, Snd.Sn_TYNF[8],  6 );	// 中間納付税額９回目
			memmove( Stm.Tynof10,Snd.Sn_TYNF[9],  6 );	// 中間納付税額10回目
			memmove( Stm.Tynof11,Snd.Sn_TYNF[10], 6 );	// 中間納付税額11回目
			break;
		default:	// 年１回
			break;
	}

	memmove( Stm.Kuri1,  Snd.Sn_KURI1,  6 );	// 第１種事業　課税売上高
	memmove( Stm.Kuri2,  Snd.Sn_KURI2,  6 );	// 第２種事業　　　〃
	memmove( Stm.Kuri3,  Snd.Sn_KURI3,  6 );	// 第３種事業　　　〃
	memmove( Stm.Kuri4,  Snd.Sn_KURI4,  6 );	// 第４種事業　　　〃
	memmove( Stm.Kuri5,  Snd.Sn_KURI5,  6 );	// 第５種事業　　　〃
	Stm.Up1 = Snd.Sn_UP1;						// 第１種事業　売上割合
	Stm.Up2 = Snd.Sn_UP2;						// 第２種事業　　　〃
	Stm.Up3 = Snd.Sn_UP3;						// 第３種事業　　　〃
	Stm.Up4 = Snd.Sn_UP4;						// 第４種事業　　　〃
	Stm.Up5 = Snd.Sn_UP5;						// 第５種事業　　　〃
}

// 消費税申告書用  旧 WORK AREA WRITE
void CShinCalculateW::so_close( void )
{
	memmove( Snd.Sn_KSTDM,  Stm.Kstdm,  6 );	// 課税標準額（自動車）
	memmove( Snd.Sn_CK45,   Stm.Ck45,   6 );	// 	  〃	  (4.5%)
	memmove( Snd.Sn_CK60,   Stm.Ck60,   6 );	// 	  〃	  (  6%)
	memmove( Snd.Sn_KSTD,   Stm.Kstd,   6 );	// 　 〃　　（その他）
	memmove( Snd.Sn_TIKAM,  Stm.Tikam,  6 );	// 対価の返還等をした金額:自動車
	memmove( Snd.Sn_CH45,   Stm.Ch45,   6 );	// 		〃	  (4.5%)
	memmove( Snd.Sn_CH60,   Stm.Ch60,   6 );	// 		〃	  (  6%)
	memmove( Snd.Sn_TIKA,   Stm.Tika,   6 );	//　　　〃				:その他
	memmove( Snd.Sn_SYTXM,  Stm.Sytxm,  6 );	// 消費税額（自動車）
	memmove( Snd.Sn_CZ45,   Stm.Cz45,   6 );	//　 〃		(4.5%)
	memmove( Snd.Sn_CZ60,   Stm.Cz60,   6 );	//　 〃		(  6%)
	memmove( Snd.Sn_SYTX,   Stm.Sytx,   6 );	//　 〃　 （その他）
	memmove( Snd.Sn_KJADJZ, Stm.Kjadjz, 6 );	// 控除過大調整税額
	memmove( Snd.Sn_SIREZ,  Stm.Sirez,  6 );	// 控除対象仕入税額
	memmove( Snd.Sn_HKANZ,  Stm.Hkanz,  6 );	// 返還等対価に係る税額
	memmove( Snd.Sn_KSITZ,  Stm.Ksitz,  6 );	// 貸倒れに係る税額
	memmove( Snd.Sn_EDNOFZ, Stm.Ednofz, 6 );	// 最終納付税額
	memmove( Snd.Sn_KZURI,  Stm.Kzuri,  6 );	// 課税売上高
	memmove( Snd.Sn_SOURI,  Stm.Souri,  6 );	// 総売上
	memmove( Snd.Sn_KURI1,  Stm.Kuri1,  6 );	// 第１種事業　課税売上
	memmove( Snd.Sn_KURI2,  Stm.Kuri2,  6 );	// 第２種事業　　 〃
	memmove( Snd.Sn_KURI3,  Stm.Kuri3,  6 );	// 第３種事業　　 〃
	memmove( Snd.Sn_KURI4,  Stm.Kuri4,  6 );	// 第４種事業　　 〃
	memmove( Snd.Sn_KURIA,  Kenuri,     6 );	// 事業別課税売上高合計
	memmove( Snd.Sn_GENKAI, Stm.Genkai, 6 );	// 限界控除税額
	memset( Snd.Sn_WGENKA, '\0', 6  );			// 限界控除手入力額　クリア
	switch( Snh.Sn_MCOUNT ) {
		case 3 :	// 年３回
			memmove( Snd.Sn_TYNF1, Stm.Tynof1, 6 );		// 中間納付税額 １回目
			memmove( Snd.Sn_TYNF2, Stm.Tynof2, 6 );		//     〃      ２回目
			memmove( Snd.Sn_TYNF3, Stm.Tynof3, 6 );		//     〃      ３回目
			memmove( Snd.Sn_TYNF4, Stm.Tynof4, 6 );		//     〃      ４回目
			break;
		case 11:	// 年11回
			memmove( Snd.Sn_TYNF[0], Stm.Tynof1, 6 );		// 中間納付税額 １回目
			memmove( Snd.Sn_TYNF[1], Stm.Tynof2, 6 );		//     〃      ２回目
			memmove( Snd.Sn_TYNF[2], Stm.Tynof3, 6 );		//     〃      ３回目
			memmove( Snd.Sn_TYNF[3], Stm.Tynof4, 6 );		//     〃      ４回目
			memmove( Snd.Sn_TYNF[4], Stm.Tynof5, 6 );		//     〃      ５回目
			memmove( Snd.Sn_TYNF[5], Stm.Tynof6, 6 );		//     〃      ６回目
			memmove( Snd.Sn_TYNF[6], Stm.Tynof7, 6 );		//     〃      ７回目
			memmove( Snd.Sn_TYNF[7], Stm.Tynof8, 6 );		//     〃      ８回目
			memmove( Snd.Sn_TYNF[8], Stm.Tynof9, 6 );		//     〃      ９回目
			memmove( Snd.Sn_TYNF[9], Stm.Tynof10,6 );		//     〃      10回目
			memmove( Snd.Sn_TYNF[10],Stm.Tynof11,6 );		//     〃      11回目
			break;
		default:	// 年１回
			break;
	}

	Snd.Sn_UP1 = Stm.Up1;						// 第１種事業　売上割合
	Snd.Sn_UP2 = Stm.Up2;						// 第２種事業　　 〃
	Snd.Sn_UP3 = Stm.Up3;						// 第３種事業　　 〃
	Snd.Sn_UP4 = Stm.Up4;						// 第４種事業　　 〃
	Snd.Sn_UP5 = Stm.Up5;						// 第５種事業　　 〃
	if( Kenei & 0x10 )	Snh.Sn_TOKUR |= 0x01;	// 特例計算摘要ＳＷ
	else				Snh.Sn_TOKUR &= 0x00;

	Snh.Sn_TUMIAG &= 0xfe;
	if( K221sw )	Snh.Sn_TUMIAG |= 0x01;
	Snh.Sn_TUMIAG &= 0xfd;
	if( H301sw && Kani < 2 )	Snh.Sn_TUMIAG |= 0x02;

	Snh.Sn_UPERS &= 0xfe;
	if( Srx )	Snh.Sn_UPERS |= 0x01;
}

// 申告書用データ　ＲＥＡＤ
void CShinCalculateW::sn_read( void )
{
	char	sg;
	
	memset( &fh1, '\0', sizeof( struct _F1 ) );
	memset( &fh2, '\0', sizeof( struct _F2 ) );
	memset( &fh3, '\0', sizeof( struct _F3 ) );
	memset( &fh5, '\0', sizeof( struct _F5 ) );
	memset( &shn, '\0', sizeof( struct _SHIN ) );
	
//★//[12'01.19]///
	TBhead.SVmzsw = 0x01;
///////////////////
	if( (Ver&0x0f) == 1 )	TBhead.SVmzsw = 0x01;
	sg = ( TBhead.SVmzsw&0x01 );				// 経過措置処理あり
	
	if( Kani < 2 ) {							// 原則課税：付表２及び２-(2)
		// (免税)控除対象調整税額
		if( TBhead.SVzchg & 0x10 ) {
			if( sg )	memmove( fh2.F212A, Snd.Sn_MENZZ, 6 );
			memmove( fh2.F212B, Snd.Sn_MNZ40, 6 );
		}

		// 非課税資産の輸出	[03'10.14]
		memmove( fh2.F23A,  Snd.Sn_2F3A, 6 );
		memmove( fh2.F23B,  Snd.Sn_2F3B, 6 );

		// 調整対象固定資産に係る調整税額
		if( sg )	memmove( fh2.F219A, Snd.Sn_2F19A, 6 );
		memmove( fh2.F219B, Snd.Sn_2F19B, 6 );
		// 調整対象固定資産を課税業務用に転用した調整額
		if( sg )	memmove( fh2.F220A, Snd.Sn_2F20A, 6 );
		memmove( fh2.F220B, Snd.Sn_2F20B, 6 );
		// 貸倒回収に係る税額
		if( sg )	memmove( fh2.F223A, Snd.Sn_2F23A, 6 );
		memmove( fh2.F223B, Snd.Sn_2F23B, 6 );
	}
	else {				// 簡易課税：付表５及び５-(2)
		// 特例計算適用ＳＷ	
		fh5.SZsw = Snd.Sn_5FSZsw&0xff;
		fh5.SBsw = Snd.Sn_5FSBsw&0xff;
		fh5.CQsw = Snd.Sn_5FCQsw&0x0f;
		fh5.GKsw = Snd.Sn_5FGKsw&0xff;
	}
}

// 申告書用データ　ＷＲＩＴＥ
void CShinCalculateW::sn_close( void )
{
	char	sg;

	if( ! (sg = ( TBhead.SVmzsw&0x01 )) ) {	// 経過措置処理の有無
		memset( fh1.F12A,  '\0', 6 );		// 無の時,付表1旧税率分内容ｸﾘｱ
		memset( fh1.F15A,  '\0', 6 );
		memset( fh1.F17A,  '\0', 6 );
		memset( fh1.F18A,  '\0', 6 );
		memset( fh1.F19A,  '\0', 6 );
		memset( fh1.F110A, '\0', 6 );
	}
	
	// 付表１ or ４
	// 課税標準額
	memmove( Snd.Sn_1F1B, fh1.F11B, 6 );
	memmove( Snd.Sn_1F2A, fh1.F12A, 6 );
	// 消費税額
	memmove( Snd.Sn_1F4B, fh1.F14B, 6 );
	memmove( Snd.Sn_1F5A, fh1.F15A, 6 );
	// 控除過大調整税額	
	memmove( Snd.Sn_1F7A, fh1.F17A, 6 );
	memmove( Snd.Sn_1F7B, fh1.F17B, 6 );
	// 控除対象仕入額
	memmove( Snd.Sn_1F8A, fh1.F18A, 6 );
	memmove( Snd.Sn_1F8B, fh1.F18B, 6 );
	memmove( Snd.Sn_1F8C, fh1.F18C, 6 );
	// 返還等対価に係る税額
	memmove( Snd.Sn_1F9A, fh1.F19A, 6 );
	memmove( Snd.Sn_1F9B, fh1.F19B, 6 );
	memmove( Snd.Sn_1F9C, fh1.F19C, 6 );
	// 貸倒れに係る税額	
	memmove( Snd.Sn_1F10A, fh1.F110A, 6 );
	memmove( Snd.Sn_1F10B, fh1.F110B, 6 );
	// 控除不足税額 Ｂ
	memmove( Snd.Sn_1F12B, fh1.F112B, 6 );
	// 差引税額　　 Ｂ
	memmove( Snd.Sn_1F13B, fh1.F113B, 6 );
	// 合計差引税額 Ｃ
	memmove( Snd.Sn_1F14C, fh1.F114C, 6 );
	
	if( Kani < 2 ) {			// 原則課税：付表２及び２-(2)
		// 課税標準額
		memmove( Snd.Sn_2F1A, fh2.F21A,  6 );
		memmove( Snd.Sn_2F1B, fh2.F21B,  6 );
		// 免税売上額
		memmove( Snd.Sn_2F2A, fh2.F22A,  6 );
		memmove( Snd.Sn_2F2B, fh2.F22B,  6 );
		// 非課税資産の輸出
		memmove( Snd.Sn_2F3A, fh2.F23A,  6 );
		memmove( Snd.Sn_2F3B, fh2.F23B,  6 );
		// 非課税売上額	
		memmove( Snd.Sn_2F6A, fh2.F26A,  6 );
		memmove( Snd.Sn_2F6B, fh2.F26B,  6 );
		// 課税売上割合	
		Snd.Sn_2FUWS = fh2.F2UWS;
		// 課税仕入に係る支払対価の額
		memmove( Snd.Sn_2F8A, fh2.F28A,  6 );
		memmove( Snd.Sn_2F8B, fh2.F28B,  6 );
		// 課税仕入に係る消費税額
		memmove( Snd.Sn_2F9A, fh2.F29A,  6 );
		memmove( Snd.Sn_2F9B, fh2.F29B,  6 );
		// 課税貨物に係る支払対価の額
		memmove( Snd.Sn_2F10A, fh2.F210A, 6 );
		memmove( Snd.Sn_2F10B, fh2.F210B, 6 );
		// 課税貨物に係る消費税額
		memmove( Snd.Sn_2F11A, fh2.F211A, 6 );
		memmove( Snd.Sn_2F11B, fh2.F211B, 6 );
		// 課税売上に係る仕入税額
		memmove( Snd.Sn_2F15A, fh2.F215A, 6 );
		memmove( Snd.Sn_2F15B, fh2.F215B, 6 );
		// 共通の売上に係る仕入税額
		memmove( Snd.Sn_2F16A, fh2.F216A, 6 );
		memmove( Snd.Sn_2F16B, fh2.F216B, 6 );
		// 控除対象仕入税額
		memmove( Snd.Sn_1F8A, fh2.F221A, 6 );
		memmove( Snd.Sn_1F8B, fh2.F221B, 6 );
		// 控除過大調整税額	･ 貸倒回収に係る消費税額
		l_add( Snd.Sn_1F7A,  fh2.F222A, fh2.F223A );
		l_add( Snd.Sn_1F7B,  fh2.F222B, fh2.F223B );
		memmove( Snd.Sn_2F23A, fh2.F223A, 6 );
		memmove( Snd.Sn_2F23B, fh2.F223B, 6 );
		
		// 個別対応･比例配分方式控除仕入税額手入力額クリア
		memset( Snd.Sn_2F17A, '\0', 12 );
		memset( Snd.Sn_2F18A, '\0', 12 );

#ifdef	VERSIONUP
		// 特定収入　以外
		if( !m_SpcSg ) {
			memset( Snd.Sn_2F21A, '\0', 12 );
			Snd.Sn_2F21sw = 0x00;
		}
#else
		memset( Snd.Sn_2F21A, '\0', 12 );
		Snd.Sn_2F21sw = 0x00;
#endif

	}
	else {				// 簡易課税：付表５及び５-(2)
		// 事業区分別の合計額
		memmove( Snd.Sn_5FURIK, fh5.F56T,  6 );
		memmove( Snd.Sn_5FURIT, fh5.F56B,  6 );
		memmove( Snd.Sn_5FUZTT, fh5.F512T, 6 );
		memmove( Snd.Sn_5FUZKT, fh5.F512B, 6 );
		// 第一種事業課税売上高
		memmove( Snd.Sn_KURI1, stval.knuri1, 6 );
		memmove( Snd.Sn_5FURI1, fh5.F57B, 6 );
		if( sg )	Snd.Sn_5FUP1 = fh5.F51UPb;
		memmove( Snd.Sn_5FUZT1, fh5.F513T, 6 );
		memmove( Snd.Sn_5FUZK1, fh5.F513B, 6 );
		// 第二種事業課税売上高	
		memmove( Snd.Sn_KURI2, stval.knuri2, 6 );
		memmove( Snd.Sn_5FURI2, fh5.F58B, 6 );
		if( sg )	Snd.Sn_5FUP2 = fh5.F52UPb;
		memmove( Snd.Sn_5FUZT2, fh5.F514T, 6 );
		memmove( Snd.Sn_5FUZK2, fh5.F514B, 6 );
		// 第三種事業課税売上高	
		memmove( Snd.Sn_KURI3,  stval.knuri3, 6 );
		memmove( Snd.Sn_5FURI3, fh5.F59B, 6 );
		if( sg )	Snd.Sn_5FUP3 = fh5.F53UPb;
		memmove( Snd.Sn_5FUZT3, fh5.F515T, 6 );
		memmove( Snd.Sn_5FUZK3, fh5.F515B, 6 );
		// 第四種事業課税売上高	
		memmove( Snd.Sn_KURI4,  stval.knuri4, 6 );
		memmove( Snd.Sn_5FURI4, fh5.F510B, 6 );
		if( sg )	Snd.Sn_5FUP4 = fh5.F54UPb;
		memmove( Snd.Sn_5FUZT4, fh5.F516T, 6 );
		memmove( Snd.Sn_5FUZK4, fh5.F516B, 6 );
		// 第五種事業課税売上高
		memmove( Snd.Sn_KURI5, stval.knuri5, 6 );
		memmove( Snd.Sn_5FURI5, fh5.F511B, 6 );
		if( sg )	Snd.Sn_5FUP5 = fh5.F55UPb;
		memmove( Snd.Sn_5FUZT5, fh5.F517T, 6 );
		memmove( Snd.Sn_5FUZK5, fh5.F517B, 6 );
		// 原則計算を適用する場合の控除対象仕入税額
		memmove( Snd.Sn_5FSZTG, fh5.F518T, 6 );
		memmove( Snd.Sn_5FSZKG, fh5.F518B, 6 );
		// 特例計算を適用する場合の控除対象仕入税額
		memmove( Snd.Sn_5FSZTT, fh5.F519T, 6 );
		memmove( Snd.Sn_5FSZKT, fh5.F519B, 6 );
		// １種類の事業の専業者の場合
		memset( Snd.Sn_1F8B, '\0', 6 );
		memset( Snd.Sn_1F8C, '\0', 6 );
		if( fh5.CQsw & 0x0f ) {
			memmove( Snd.Sn_1F8B, fh5.F55B, 6 );
			memmove( Snd.Sn_1F8C, fh5.F55T, 6 );
		}
		else {
			if( fh5.SBsw ) {
				if( l_cmp( fh5.F518B, fh5.F519B ) <= 0 )
						memmove( Snd.Sn_1F8B, fh5.F519B, 6 );
				else	memmove( Snd.Sn_1F8B, fh5.F518B, 6 );
			}
			else		memmove( Snd.Sn_1F8B, fh5.F518B, 6 );
			if( fh5.SZsw ) {
				if( l_cmp( fh5.F518T, fh5.F519T ) <= 0 )
						memmove( Snd.Sn_1F8C, fh5.F519T, 6 );
				else	memmove( Snd.Sn_1F8C, fh5.F518T, 6 );
			}
			else		memmove( Snd.Sn_1F8C, fh5.F518T, 6 );
		}
		// 特例計算適用ＳＷ
		Snd.Sn_5FSZsw = fh5.SZsw;
		Snd.Sn_5FSBsw = fh5.SBsw;
		Snd.Sn_5FCQsw = (fh5.CQsw | 0x10);
		Snd.Sn_5FGKsw = 0x00;
	}
	
	// 1.限界控除前の税額
	memmove( Snd.Sn_3F1, fh3.F31, 6 );
	// 2.限界控除税額
	memmove( Snd.Sn_3FA, fh3.GEN_A, 6 );
	memmove( Snd.Sn_3FB, fh3.GEN_B, 6 );
	memmove( Snd.Sn_3F2, fh3.F32, 6 );
	memmove( Snd.Sn_GENKAI, fh3.F32, 6 );
	// 3.差引税額
	memmove( Snd.Sn_3F3, fh3.F33, 6 );
	// 4.地方消費税・課税標準の基礎金額
	memmove( Snd.Sn_3F4, fh3.F34, 6 );
	// 5.地方消費税・限界控除税額相当額
	memmove( Snd.Sn_3F5, fh3.F35, 6 );
	// 6.地方消費税・課税標準となる消費税額
	memmove( Snd.Sn_3F6, fh3.F36, 6 );
	// 月数
	Snd.Sn_3FMON1 = fh3.Fmon1&0xff;
	Snd.Sn_3FMON2 = fh3.Fmon2&0xff;
	Snd.Sn_3FMON3 = fh3.Fmon3&0xff;
	
	// 課税標準額
	memmove( Snd.Sn_KSTD, shn.shin1, 6 );
	// 消費税額	
	memmove( Snd.Sn_SYTX, shn.shin2, 6 );
	// 控除過大調整税額	
	memmove( Snd.Sn_KJADJZ, shn.shin3, 6 );
	// 控除対象仕入額
	memmove( Snd.Sn_SIREZ, shn.shin4, 6 );
	// 返還等対価に係る税額
	memmove( Snd.Sn_HKANZ, shn.shin5, 6 );
	// 貸倒れに係る税額
	memmove( Snd.Sn_KSITZ, shn.shin6, 6 );
	//	既確定税額
	memmove( Snd.Sn_KAKTIZ, shn.shin13, 6 );
	//	課税売上高	
	memmove( Snd.Sn_KZURI,  shn.shin15, 6 );
	if( Kani < 2 ) {
		// 総売上高
		memmove( Snd.Sn_SOURI, shn.shin16, 6 );
		memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
		// 課税売上高割合

		l_input( WORK1, RatioAsc );
		pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)Snd.Sn_KZURI, (unsigned char *)Snd.Sn_SOURI, 2, 0 );
		// 売上割合９５％以上の場合の仕入控除方式の出力ＳＷ
		Snh.Sn_UPERS &= 0xfe;
		if( l_cmp( WORK0, WORK1 ) < 0 )	Snh.Sn_UPERS |= 0x01;	// 95%未満
	}
	else	// 基準期間の課税売上高
		memmove( Snd.Sn_ZZURI, shn.shin16, 6 );
	
	//	地方消費税	
	//	既確定税額
	memmove( Snd.Sn_TKAKTZ, shn.shin24, 6 );
	
	//	最終納付税額・書き込み
	if( (Snh.Sn_SKKBN&0xff) < 3 ) {
		memmove( Snd.Sn_EDNOFZ, shn.shin11, 6 );
		memmove( Snd.Sn_TEDNOF, shn.shin22, 6 );
	}
	else {
		memmove( Snd.Sn_EDNOFZ, shn.shin14, 6 );
		memmove( Snd.Sn_TEDNOF, shn.shin25, 6 );
	}
	//	年３回中間申告時の各回納付税額・書き込み
	switch( Snh.Sn_MCOUNT ) {
		case 3 :	// 年３回
			switch( Snh.Sn_JCOUNT ) {
				case 1 :	memmove( Snd.Sn_TYNF1,  Snd.Sn_EDNOFZ, 6 );
							memmove( Snd.Sn_TTYNF1, Snd.Sn_TEDNOF, 6 );	break;
				case 2 :	memmove( Snd.Sn_TYNF2,  Snd.Sn_EDNOFZ, 6 );
							memmove( Snd.Sn_TTYNF2, Snd.Sn_TEDNOF, 6 );	break;
				case 3 :	memmove( Snd.Sn_TYNF3,  Snd.Sn_EDNOFZ, 6 );
							memmove( Snd.Sn_TTYNF3, Snd.Sn_TEDNOF, 6 );	break;
				case 4 :	memmove( Snd.Sn_TYNF4,  Snd.Sn_EDNOFZ, 6 );
							memmove( Snd.Sn_TTYNF4, Snd.Sn_TEDNOF, 6 );	break;
				default:	break;
			}
			break;
		case 11:	// 年11回
			if( Snh.Sn_JCOUNT < 12 ) {
				memmove( Snd.Sn_TYNF[ Snh.Sn_JCOUNT - 1 ], Snd.Sn_EDNOFZ, 6 );	
				memmove( Snd.Sn_TTNF[ Snh.Sn_JCOUNT - 1 ], Snd.Sn_TEDNOF, 6 );
			}
			break;
		default:	// 年１回
			break;
	}
}		

// 申告書マスター用　計算
void CShinCalculateW::stm_new( char ksg )
{
	fh1_set( ksg );
	stm_calq();	
}

// 付表１ or 付表四内容セット
void CShinCalculateW::fh1_set( char ksg )
{
	// 課税標準額　（旧税率分)
	memmove( fh1.F12A, stval.kazeiu_3, 6 );
	memmove( fh2.F21A, fh1.F12A, 6 );		// 付表２ 1-(A) へコピー

	// 課税標準額　（税率4%分)
	memmove( fh1.F11B, stval.kazeiu, 6 );
	memmove( fh2.F21B, fh1.F11B, 6 );		// 付表２ 1-(B) へコピー
	
	l_calq( fh1.F11B, fh1.F11B, 0 );		// 1000円未満切り捨て
	l_calq( fh1.F12A, fh1.F12A, 0 );		// 1000円未満切り捨て
	l_add( fh1.F13C, fh1.F11B, fh1.F12A );

	// 消費税額（旧税率分）
	memmove( fh1.F15A, stval.urisyz_3, 6 );
	
	// 消費税額（税率4%分）	
	memmove( fh1.F14B, stval.urisyz, 6 );
	l_add( fh1.F16C, fh1.F14B, fh1.F15A );

	// 控除過大調整税額
	memmove( fh1.F17B, stval.kaishu, 6 );
	if( TBhead.SVmzsw&0x01 ) {		// 経過措置処理あり
		memmove( fh1.F17A, stval.kaishu_3, 6 );
	}
	l_add( fh1.F17C, fh1.F17A, fh1.F17B );
	
	// 対価の返還等をした税額　（旧税率分）
	memmove( fh1.F19A, stval.henkan_3, 6 );

	// 対価の返還等をした税額　（税率4%分）
	memmove( fh1.F19B, stval.henkan, 6 );
	l_add( fh1.F19C, fh1.F19B, fh1.F19A );

	// 貸倒に係る税額	（旧税率分）
	memmove( fh1.F110A, stval.ksidao_3, 6 );
	
	// 貸倒に係る税額	（税率4%分）
	memmove( fh1.F110B, stval.ksidao, 6 );
	l_add( fh1.F110C, fh1.F110B, fh1.F110A );

	if( Kani < 2 ) {		// 原則課税
		// 付表２ or ２-(2) の内容計算
		fh2_set();
		// 控除過大調整税額
		l_add( fh1.F17A, fh2.F222A, fh2.F223A );
		l_add( fh1.F17B, fh2.F222B, fh2.F223B );
		l_add( fh1.F17C, fh1.F17A,  fh1.F17B );
		
		// 控除対象仕入税額
		memmove( fh1.F18A, fh2.F221A, 6 );
		memmove( fh1.F18B, fh2.F221B, 6 );
		l_add( fh1.F18C, fh1.F18A, fh1.F18B );
	}
	else {					// 簡易課税
		// 付表５ or ５-(2) の内容計算
		fh5_set( ksg );

		// 控除対象仕入税額
		if( fh5.CQsw & 0x01 ) {		// 特例なし
		 	memmove( fh1.F18B, fh5.F55B, 6 );
			memmove( fh1.F18C, fh5.F55T, 6 );
		}
		else {
			if( fh5.SBsw ) {
				if( l_cmp( fh5.F519B, fh5.F518B ) > 0 )
						memmove( fh1.F18B, fh5.F519B, 6 );
				else	memmove( fh1.F18B, fh5.F518B, 6 );
			}
			else		memmove( fh1.F18B, fh5.F518B, 6 );
			if( fh5.SZsw ) {
				if( l_cmp( fh5.F519T, fh5.F518T ) > 0 )
						memmove( fh1.F18C, fh5.F519T, 6 );
				else	memmove( fh1.F18C, fh5.F518T, 6 );
			}
			else		memmove( fh1.F18C, fh5.F518T, 6 );
		}
	}
	
	// 控除税額小計
	if( Kani < 2 ) {	// 原則課税
		l_add( fh1.F111A, fh1.F18A,  fh1.F19A );
		l_add( fh1.F111A, fh1.F111A, fh1.F110A );
	}
	l_add( fh1.F111B, fh1.F18B,  fh1.F19B );
	l_add( fh1.F111B, fh1.F111B, fh1.F110B );
	l_add( fh1.F111C, fh1.F18C,  fh1.F19C );
	l_add( fh1.F111C, fh1.F111C, fh1.F110C );
	
	// 控除不足税額
	if( Kani < 2 ) {
		l_add( fh1.F113A, fh1.F15A,  fh1.F17A );
		l_sub( fh1.F113A, fh1.F113A, fh1.F111A );
		if( l_test( fh1.F113A ) < 0 ) {
			l_neg( fh1.F113A );
			memmove( fh1.F112A, fh1.F113A, 6 );
			memset(  fh1.F113A, '\0', 6 );
		}
	}
	l_add( fh1.F113B, fh1.F14B,  fh1.F17B );
	l_sub( fh1.F113B, fh1.F113B, fh1.F111B );
	if( l_test( fh1.F113B ) < 0 ) {
		l_neg( fh1.F113B );
		memmove( fh1.F112B, fh1.F113B, 6 );
		memset( fh1.F113B, '\0', 6 );
	}
	
	if( Kani < 2 ) {	// 原則課税・付表１
		l_add( fh1.F112C, fh1.F112A, fh1.F112B );
		l_add( fh1.F113C, fh1.F113A, fh1.F113B );
	}
	else {				// 簡易課税・付表４
		l_add( fh1.F113C, fh1.F16C,  fh1.F17C );
		l_sub( fh1.F113C, fh1.F113C, fh1.F111C );
		if( l_test( fh1.F113C ) < 0 ) {
			l_neg( fh1.F113C );
			memmove( fh1.F112C, fh1.F113C, 6 );
			memset( fh1.F113C, '\0', 6 );
		}
	}
	l_sub( fh1.F114C, fh1.F113C, fh1.F112C );
}

// 付表２ or ２-(2)内容セット
void CShinCalculateW::fh2_set( void )
{
	char	buf[20], PW6[6];
	int		i;

	// 課税売上額　（課税標準額－返還等対価の額）	（旧税率分）
	l_sub( fh2.F21A, fh2.F21A, stval.knebik_3 );
	
	// 課税売上額　（課税標準額－返還等対価の額）	（税率4%分）
	l_sub( fh2.F21B, fh2.F21B, stval.knebik );
	
	// 免税売上額
	memmove( fh2.F22B, stval.exptur, 6 );

	// 非課税資産の輸出等の金額 [06'11.08]
	memmove( fh2.F23B, stval.expjyo, 6 );
	
	// 非課税売上額
	l_sub( fh2.F26B, stval.hikazu, stval.hnebik );
	
	// 資産の譲渡等の対価の額
	l_add( fh2.F24C, fh2.F21A, fh2.F21B );
	l_add( fh2.F24C, fh2.F24C, fh2.F22B );
	l_add( fh2.F24C, fh2.F24C, fh2.F23B );
	l_add( fh2.F27C, fh2.F24C, fh2.F26B );
	
	// 課税売上高割合
	memset( WORK0, '\0', 6 );
	memset( buf,  '\0', 20 );
	pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)fh2.F24C,( unsigned char *)fh2.F27C, 2, 0 );

	// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
	memset( PW6, '\0', sizeof( PW6 ) );
	if( l_cmp( fh2.F24C, PW6 ) < 0 )	memset( WORK0, '\0', 6 );
	if( l_cmp( fh2.F27C, PW6 ) < 0 )	memset( WORK0, '\0', 6 );

	if( ! l_test( WORK0 ) ) {
		memset( WORK1, '\0', 6 );
		l_input( WORK1, "10000" );
		if( l_cmp( stval.uwriai, WORK1 ) == 0 )	memmove( WORK0, WORK1, 6 );
	}
	l_print( buf, WORK0, "SSSSS" );
	fh2.F2UWS = atoi( buf );
	
	// 課税仕入に係る支払対価の額
	memmove( fh2.F28A, stval.komisi_3, 6 );
	memmove( fh2.F28B, stval.komisi,   6 );
	
	// 課税仕入に係る消費税額
	memmove( fh2.F29A, stval.sirsyz_3, 6 );
	memmove( fh2.F29B, stval.sirsyz,   6 );

	// 輸入仕入計算
	if( (TBhead.SVvers & 0xf0) >= 0x10 ) {
		// 課税貨物に係る消費税額
		memmove( fh2.F211A, stval.IpStz0_3, 6 );
		memmove( fh2.F211B, stval.IpStze_4, 6 );
	}
	
	// 課税貨物に係る支払対価の額及び消費税額
	memset( fh2.F210A, '\0', 6 );		memset( fh2.F210B, '\0', 6 );
	
	// (免税)控除対象調整税額
	if( TBhead.SVzchg & 0x10 ) {
		// 棚卸調整額の算出
		if( Anbun ) {
			// ３％分
			memset( Snd.Sn_MENZZ, '\0', 6 );
			for( i = 0; i != 3; i++ ) {
				l_add( Snd.Sn_MENZZ, Snd.Sn_MENZZ, Snd.Sn_menzz[i] );
			}
			// ４％分
			memset( Snd.Sn_MNZ40, '\0', 6 );
			for( i = 0; i != 3; i++ ) {
				l_add( Snd.Sn_MNZ40, Snd.Sn_MNZ40, Snd.Sn_mnz40[i] );
			}
		}
		if( l_test( Snd.Sn_MENZZ ) )	TBhead.SVmzsw |= 0x01;			// 経過措置有り

		memmove( fh2.F212A, Snd.Sn_MENZZ, 6 );
		memmove( fh2.F212B, Snd.Sn_MNZ40, 6 );
		if( TBhead.SVzchg & 0x01 ) {
			l_neg( fh2.F212A );		l_neg( fh2.F212B );
		}	
	}
	
	// 課税仕入等の税額の合計額
	l_add( fh2.F213A, fh2.F29A,  fh2.F211A );
	l_add( fh2.F213A, fh2.F213A, fh2.F212A );
	l_add( fh2.F213B, fh2.F29B,  fh2.F211B );
	l_add( fh2.F213B, fh2.F213B, fh2.F212B );
	
	Snh.Sn_UPERS &= 0xfe;
//★//[12'02.23]///
	int	m_Junction = 0;
	if( m_Compare == 0 ) {
		// シミュレーションしない
		if( fh2.F2UWS < RatioInt ) m_Junction = 1;	
	}
	else	{
		// シミュレーションする
		if( (fh2.F2UWS < RatioInt) || m_Calcusw != 0 )	m_Junction = 1;
	}
	if( m_Junction )	{
///////////////////
		Snh.Sn_UPERS |= 0x01;
		memset( WORK0, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)fh2.F24C, (unsigned char *)fh2.F27C, 5, 0 );

		// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
		memset( PW6, '\0', sizeof( PW6 ) );
		if( l_cmp( fh2.F24C, PW6 ) < 0 )	memset( WORK0, '\0', 6 );
		if( l_cmp( fh2.F27C, PW6 ) < 0 )	memset( WORK0, '\0', 6 );

		if( Anbun ) {			// 個別対応選択時

			memmove( fh2.F215A, stval.kauriz_3, 6 );
			memmove( fh2.F215B, stval.kauriz, 	6 );
			memmove( fh2.F216A, stval.kyuriz_3, 6 );
			memmove( fh2.F216B, stval.kyuriz, 	6 );

// 棚卸調整後へ移動 ★[14'01.17]
//			pArithEx->l_mul100( (unsigned char *)fh2.F217A, (unsigned char *)fh2.F216A, (unsigned char *)PW6, 5, 0 );
//			l_add( fh2.F217A, fh2.F217A, fh2.F215A );
//			
//			pArithEx->l_mul100( (unsigned char *)fh2.F217B, (unsigned char *)fh2.F216B, (unsigned char *)PW6, 5, 0 );
//			l_add( fh2.F217B, fh2.F217B, fh2.F215B );
//			memset( fh2.F218A, '\0', 6*2 );
////////////////////////////////

#ifdef	VERSIONUP
			// 課税仕入れ等の税額　９０％未満の個別対応
			if( m_SpcSg ) {	
				memmove( stval.SpKgz3, fh2.F217A, 6 );				// 旧税率分・税込み仕入税額
				memmove( stval.SpKgz4, fh2.F217B,   6 );			// 新税率分・税込み仕入税額
				l_add( stval.SpKgzg, stval.SpKgz3, stval.SpKgz4 );	// 合計　　・税込み仕入	税額
			}
#endif

			// 棚卸調整９５％未満
			if( Snd.Sn_MENZsw & 0x10 ) {
				// 課税に係る
				memmove( WORK0, Snd.Sn_menzz[0], 6 );
				memmove( WORK1, Snd.Sn_mnz40[0], 6 );
				if( Snd.Sn_MENZsw & 0x01 )	{
					l_neg( WORK0 );
					l_neg( WORK1 );
				}
				l_add( fh2.F215A, fh2.F215A, WORK0 );	// ３％
				l_add( fh2.F215B, fh2.F215B, WORK1 );	// ４％

				// 共通に係る
				memmove( WORK0, Snd.Sn_menzz[2], 6 );
				memmove( WORK1, Snd.Sn_mnz40[2], 6 );
				if( Snd.Sn_MENZsw & 0x01 )	{
					l_neg( WORK0 );
					l_neg( WORK1 );
				}
				l_add( fh2.F216A, fh2.F216A, WORK0 );	// ３％
				l_add( fh2.F216B, fh2.F216B, WORK1 );	// ４％
			}
// 棚卸調整後へ移動 ★[14'01.17]
			pArithEx->l_mul100( (unsigned char *)fh2.F217A, (unsigned char *)fh2.F216A, (unsigned char *)PW6, 5, 0 );
			l_add( fh2.F217A, fh2.F217A, fh2.F215A );
			
			pArithEx->l_mul100( (unsigned char *)fh2.F217B, (unsigned char *)fh2.F216B, (unsigned char *)PW6, 5, 0 );
			l_add( fh2.F217B, fh2.F217B, fh2.F215B );
			memset( fh2.F218A, '\0', 6*2 );
////////////////

#ifdef	_DEBUG_EX
			char	mny[20] = {0};
			TRACE( "\n" );
			l_print( mny, PW6, "SSS,SSS,SSS,SS9" );
			TRACE( "PW6  (%s) ", mny );
			l_print( mny, fh2.F215B, "SSS,SSS,SSS,SS9" );
			TRACE( "F215B(%s) ", mny );
			l_print( mny, fh2.F216B, "SSS,SSS,SSS,SS9" );
			TRACE( "F216B(%s) ", mny );
			l_print( mny, fh2.F217B, "SSS,SSS,SSS,SS9" );
			TRACE( "F217B(%s) ", mny );

			TRACE( "\n" );
#endif
		}
		else {					// 比例配分選択時
			pArithEx->l_mul100( (unsigned char *)fh2.F218A, (unsigned char *)fh2.F213A, (unsigned char *)WORK0, 5, 0 );
			pArithEx->l_mul100( (unsigned char *)fh2.F218B, (unsigned char *)fh2.F213B, (unsigned char *)WORK0, 5, 0 );
			memset( fh2.F215A, '\0', 6*6 );

#ifdef	VERSIONUP
			// 課税仕入れ等の税額　９０％未満の比例配分
			if( m_SpcSg ) {
				memmove( stval.SpKgz3, fh2.F218A, 6 );				// 旧税率分・税込み仕入税額
				memmove( stval.SpKgz4, fh2.F218B,   6 );			// 新税率分・税込み仕入税額
				l_add( stval.SpKgzg, stval.SpKgz3, stval.SpKgz4 );	// 合計　　・税込み仕入	税額
			}
#endif

		}
	}
	else {
		memset( fh2.F215A, '\0', 6*8 );

#ifdef	VERSIONUP
		// 課税仕入れ等の税額　９０％以上
		if( m_SpcSg ) {
			memmove( stval.SpKgz3, fh2.F213A, 6 );				// 旧税率分・税込み仕入税額
			memmove( stval.SpKgz4, fh2.F213B,   6 );			// 新税率分・税込み仕入税額
			l_add( stval.SpKgzg, stval.SpKgz3, stval.SpKgz4 );	// 合計　　・税込み仕入	税額
		}
#endif

	}
	
	// 調整対象固定資産に係る調整税額
	memmove( fh2.F219A, Snd.Sn_2F19A, 6 );
	memmove( fh2.F219B, Snd.Sn_2F19B, 6 );
	// 調整対象固定資産を課税業務用に転用した調整額
	memmove( fh2.F220A, Snd.Sn_2F20A, 6 );
	memmove( fh2.F220B, Snd.Sn_2F20B, 6 );

	// 差引
	memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
	if( !(Snh.Sn_UPERS & 0x01) ) {		// 課税売上割合が95％以上
		memmove( WORK0, fh2.F213A, 6 );
		memmove( WORK1, fh2.F213B, 6 );
	}
	else {						// 課税売上割合が95％未満
		if( Anbun ) {			// 個別対応
			memmove( WORK0, fh2.F217A, 6 );
			memmove( WORK1, fh2.F217B, 6 );
		}
		else {					// 比例配分
			memmove( WORK0, fh2.F218A, 6 );
			memmove( WORK1, fh2.F218B, 6 );
		}
	}
	l_add( fh2.F221A, fh2.F219A, fh2.F220A );
	l_add( fh2.F221A, fh2.F221A, WORK0 );
	if( l_test( fh2.F221A ) < 0 ) {
		l_neg( fh2.F221A );
		memmove( fh2.F222A, fh2.F221A, 6 );
		memset( fh2.F221A, '\0', 6 );
	}
	
	l_add( fh2.F221B, fh2.F219B, fh2.F220B );
	l_add( fh2.F221B, fh2.F221B, WORK1 );
	if( l_test( fh2.F221B ) < 0 ) {
		l_neg( fh2.F221B );	memmove( fh2.F222B, fh2.F221B, 6 );
		memset( fh2.F221B, '\0', 6 );
	}
	
	// 貸倒回収に係る消費税額
	memmove( fh2.F223B, stval.kaishu, 6 );
	if( TBhead.SVmzsw&0x01 ) {		// 経過措置処理あり
		memmove( fh2.F223A, stval.kaishu_3, 6 );
	}
}

// 付表５用計算バッファクリア
void CShinCalculateW::fhbf_clr( char sg, char ksw )
{
	memset( &fh5.F51UPb, '\0', 2*5 );
	switch( sg ) {
		case 0 :	if( ! ksw )	memset( fh5.F55T, '\0', 6*30 );
					else		memset( fh5.F56T, '\0', 6*28 );	break;
		default:	if( ! ksw )	memset( fh5.F55B, '\0', 6 );
					memset( fh5.F56B, '\0', 6 );
					memset( fh5.F57B, '\0', 6 );	memset( fh5.F58B,  '\0', 6 );
					memset( fh5.F59B, '\0', 6 );	memset( fh5.F510B, '\0', 6 );
					memset( fh5.F511B,'\0', 6 );	memset( fh5.F512B, '\0', 6 );
					memset( fh5.F513B,'\0', 6 );	memset( fh5.F514B, '\0', 6 );
					memset( fh5.F515B,'\0', 6 );	memset( fh5.F516B, '\0', 6 );
					memset( fh5.F517B,'\0', 6 );	memset( fh5.F518B, '\0', 6 );
					memset( fh5.F519B,'\0', 6 );	break;
	}
}

// 別表５の 内容計算
int CShinCalculateW::fh5_set( char ksg )
{
	char	PW0[6], val1[6], val2[6];
	int		i;
	ushort	szsg, per1, per2;

	// 課税標準額に対する消費税額
	memmove( fh5.F51T, fh1.F16C, 6 );
	memmove( fh5.F51B, fh1.F14B, 6 );

	// 貸倒回収額に対する消費税額
	memmove( fh5.F52T, fh1.F17C, 6 );
	memmove( fh5.F52B, fh1.F17B, 6 );

	// 売上対価の返還等に係る消費税額
	memmove( fh5.F53T, fh1.F19C, 6 );
	memmove( fh5.F53B, fh1.F19B, 6 );

	// 控除対象仕入税額計算の基礎となる消費税額
	l_add( fh5.F54T, fh5.F51T, fh5.F52T );
	l_sub( fh5.F54T, fh5.F54T, fh5.F53T );
	l_add( fh5.F54B, fh5.F51B, fh5.F52B );
	l_sub( fh5.F54B, fh5.F54B, fh5.F53B );
	if( l_test( fh5.F54T ) < 0 && l_test( fh5.F54B ) < 0 ) {
		fhbf_clr( 0, 0 );
		return( FALSE );
	}

	// 兼営事業の課税売上高に係る消費税額の計算
	memmove( fh5.F56T,  Kenuri,  6 );			//	事業別課税　売上高合計
	memmove( fh5.F57T,  stval.knuri1, 6 );		//	第１種事業　課税売上高
	memmove( fh5.F58T,  stval.knuri2, 6 );		//	第２種事業　　　”	
	memmove( fh5.F59T,  stval.knuri3, 6 );		//	第３種事業　　　”	
	memmove( fh5.F510T, stval.knuri4, 6 );		//	第４種事業　　　”	
	memmove( fh5.F511T, stval.knuri5, 6 );		//	第５種事業　　　”	
	memset( WORK0, '\0', 6 );
	l_print( WORK0, stval.knwai1, "ssss" );
	UP1 = atoi( WORK0 );						//	第１種事業　売上割合
	l_print( WORK0, stval.knwai2, "ssss" );
	UP2 = atoi( WORK0 );			   			//	第２種事業　　　”	
	l_print( WORK0, stval.knwai3, "ssss" );
	UP3 = atoi( WORK0 );						//	第３種事業　　　”
	l_print( WORK0, stval.knwai4, "ssss" );
	UP4 = atoi( WORK0 );		  				//	第４種事業　　　”
	l_print( WORK0, stval.knwai5, "ssss" );
	UP5 = atoi( WORK0 );		  				//	第５種事業　　　”
	memmove( fh5.F513T, stval.knzei1, 6 );		//	第１種事業　消費税額
	memmove( fh5.F514T, stval.knzei2, 6 );		//	第２種事業　　　”
	memmove( fh5.F515T, stval.knzei3, 6 );		//	第３種事業　　　”
	memmove( fh5.F516T, stval.knzei4, 6 );		//	第４種事業　　　”
	memmove( fh5.F517T, stval.knzei5, 6 );		//	第５種事業　　　”
	l_add( fh5.F512T, fh5.F512T, stval.knzei1 );
	l_add( fh5.F512T, fh5.F512T, stval.knzei2 );
	l_add( fh5.F512T, fh5.F512T, stval.knzei3 );
	l_add( fh5.F512T, fh5.F512T, stval.knzei4 );
	l_add( fh5.F512T, fh5.F512T, stval.knzei5 );
	l_add( fh5.F512B, fh5.F512B, stval.knzei1_n );
	l_add( fh5.F512B, fh5.F512B, stval.knzei2_n );
	l_add( fh5.F512B, fh5.F512B, stval.knzei3_n );
	l_add( fh5.F512B, fh5.F512B, stval.knzei4_n );
	l_add( fh5.F512B, fh5.F512B, stval.knzei5_n );

	if( l_test( Kenuri_n ) ) {
		memmove( fh5.F56B,  Kenuri_n,  6 );		//事業別課税　売上高合計(4%)
		memmove( fh5.F57B,  stval.knuri1_n, 6 );//第１種事業  課税売上高(4%)
		memmove( fh5.F58B,  stval.knuri2_n, 6 );//第２種事業　　　”　　(4%)
		memmove( fh5.F59B,  stval.knuri3_n, 6 );//第３種事業　　　”	(4%)
		memmove( fh5.F510B, stval.knuri4_n, 6 );//第４種事業　　　”	(4%)
		memmove( fh5.F511B, stval.knuri5_n, 6 );//第５種事業　　　”	(4%)
		l_print( WORK0, stval.knwai1_n, "ssss" );
		fh5.F51UPb = atoi( WORK0 );				//	第１種事業　売上割合(4%)
		l_print( WORK0, stval.knwai2_n, "ssss" );
		fh5.F52UPb = atoi( WORK0 );	   			//	第２種事業　　　”
		l_print( WORK0, stval.knwai3_n, "ssss" );
		fh5.F53UPb = atoi( WORK0 );				//	第３種事業　　　”
		l_print( WORK0, stval.knwai4_n, "ssss" );
		fh5.F54UPb = atoi( WORK0 );	  			//	第４種事業　　　”
		l_print( WORK0, stval.knwai5_n, "ssss" );
		fh5.F55UPb = atoi( WORK0 );	  			//	第５種事業　　　”
		memmove( fh5.F513B, stval.knzei1_n, 6 );// 第１種事業　消費税額(4%)
		memmove( fh5.F514B, stval.knzei2_n, 6 );// 第２種事業　　　”
		memmove( fh5.F515B, stval.knzei3_n, 6 );// 第３種事業　　　”
		memmove( fh5.F516B, stval.knzei4_n, 6 );// 第４種事業　　　”
		memmove( fh5.F517B, stval.knzei5_n, 6 );// 第５種事業　　　”
	}
	else	fhbf_clr( 1, 0 );
	
	// １種類の事業の専業者の場合
	if( kensg == 1 ) {
		switch( ksg&0xff ) {
			case 0x01 :	per1 = TBhead.SVsper[0];	break;
			case 0x02 :	per1 = TBhead.SVsper[1];	break;
			case 0x04 :	per1 = TBhead.SVsper[2];	break;
			case 0x08 :	per1 = TBhead.SVsper[3];	break;
			case 0x10 :	if( (Ver&0x0f) == 2 && TBhead.SV_5sw )
								per1 = TBhead.SVsper[3];
						else	per1 = TBhead.SVsper[4];
						break;
			default   :	per1 = 0;					break;
		}
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per1 ) );
		memset( WORK1, '\0', 6 );	l_input( WORK1, WORK0 );
		memset( WORK0, '\0', 6 );	l_input( WORK0, "1000" );
		memset( fh5.F55B, '\0', 6 );
		memset( fh5.F55T, '\0', 6 );
		if( l_test( fh5.F54B ) )
			l_6calq( fh5.F55B, fh5.F54B, WORK1, WORK0 );
		if( l_test( fh5.F54T ) )
			l_6calq( fh5.F55T, fh5.F54T, WORK1, WORK0 );
		fh5.CQsw |= 0x01;	fh5.SZsw = fh5.SBsw = fh5.GKsw = 0x00;
		fhbf_clr( 0, 1 );	// ２種類以上の欄クリア
	}
	else {		// ２種類以上の兼営
		memset( fh5.F55T, '\0', 6 );
		memset( fh5.F55B, '\0', 6 );
		fh5.CQsw = fh5.SZsw = fh5.SBsw = fh5.GKsw = 0x00;
		
		// 原則計算を適用する場合
		memset( WORK1, '\0', 6 );	l_input( WORK1, "1000" );
		memset( fh5.F518T, '\0', 6 );
		memset( fh5.F518B, '\0', 6 );
		// 第一種事業の計算
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - TBhead.SVsper[0] ) );
		memset( PW0, '\0', 6 );	l_input( PW0, WORK0 );
		if( l_test( fh5.F513T ) ) {
			l_6calq( WORK0, fh5.F513T, PW0, WORK1 );
			l_add( fh5.F518T, fh5.F518T, WORK0 );
		}
		if( l_test( fh5.F513B ) ) {
			l_6calq( WORK0, fh5.F513B, PW0, WORK1 );
			l_add( fh5.F518B, fh5.F518B, WORK0 );
		}
		// 第二種事業の計算
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - TBhead.SVsper[1] ) );
		memset( PW0, '\0', 6 );	l_input( PW0, WORK0 );
		if( l_test( fh5.F514T ) ) {
			l_6calq( WORK0, fh5.F514T, PW0, WORK1 );
			l_add( fh5.F518T, fh5.F518T, WORK0 );
		}
		if( l_test( fh5.F514B ) ) {
			l_6calq( WORK0, fh5.F514B, PW0, WORK1 );
			l_add( fh5.F518B, fh5.F518B, WORK0 );
		}
		// 第三種事業の計算
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - TBhead.SVsper[2] ) );
		memset( PW0, '\0', 6 );	l_input( PW0, WORK0 );
		if( l_test( fh5.F515T ) ) {
			l_6calq( WORK0, fh5.F515T, PW0, WORK1 );
			l_add( fh5.F518T, fh5.F518T, WORK0 );
		}
		if( l_test( fh5.F515B ) ) {
			l_6calq( WORK0, fh5.F515B, PW0, WORK1 );
			l_add( fh5.F518B, fh5.F518B, WORK0 );
		}
		// 第四種事業の計算
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - TBhead.SVsper[3] ) );
		memset( PW0, '\0', 6 );	l_input( PW0, WORK0 );
		if( l_test( fh5.F516T ) ) {
			l_6calq( WORK0, fh5.F516T, PW0, WORK1 );
			l_add( fh5.F518T, fh5.F518T, WORK0 );
		}
		if( l_test( fh5.F516B ) ) {
			l_6calq( WORK0, fh5.F516B, PW0, WORK1 );
			l_add( fh5.F518B, fh5.F518B, WORK0 );
		}
		// 第五種事業の計算
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - TBhead.SVsper[4] ) );
		memset( PW0, '\0', 6 );	l_input( PW0, WORK0 );
		if( l_test( fh5.F517T ) ) {
			l_6calq( WORK0, fh5.F517T, PW0, WORK1 );
			l_add( fh5.F518T, fh5.F518T, WORK0 );
		}
		if( l_test( fh5.F517B ) ) {
			l_6calq( WORK0, fh5.F517B, PW0, WORK1 );
			l_add( fh5.F518B, fh5.F518B, WORK0 );
		}
		
		if( l_test( fh5.F518T ) && l_test( fh5.F512T ) ) {
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)fh5.F518T, (unsigned char *)fh5.F512T, 7, 0 );
			pArithEx->l_mul100( (unsigned char *)fh5.F518T, (unsigned char *)fh5.F54T, (unsigned char *)WORK0, 7, 0 );
		}
		if( l_test( fh5.F518B ) && l_test( fh5.F512B ) ) {
			pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)fh5.F518B, (unsigned char *)fh5.F512B, 7, 0 );
			pArithEx->l_mul100( (unsigned char *)fh5.F518B, (unsigned char *)fh5.F54B,  (unsigned char *)WORK0, 7, 0 );
		}
		
		// 特例計算適用の控除対象仕入税額の計算
		Snh.Sn_TOKUR = 0;	Kenei &= 0x0f;
		fh5.SZsw = 1;
		szsg = per1 = per2 = 0;
		memset( fh5.F519T, '\0', 6 );
		memset( fh5.F519B, '\0', 6 );
		if( UP1 >= 750 ) {
			per1 = TBhead.SVsper[0];
			if( l_test( fh5.F58T ) )	szsg |= 0x0001;
			if( l_test( fh5.F59T ) )	szsg |= 0x0002;
			if( l_test( fh5.F510T ) )	szsg |= 0x0004;
			if( l_test( fh5.F511T ) )	szsg |= 0x0008;
		}
		else if( UP2 >= 750 ) {
			per1 = TBhead.SVsper[1];
			if( l_test( fh5.F57T ) )	szsg |= 0x0001;
			if( l_test( fh5.F59T ) )	szsg |= 0x0010;
			if( l_test( fh5.F510T ) )	szsg |= 0x0020;
			if( l_test( fh5.F511T ) )	szsg |= 0x0040;
		}
		else if( UP3 >= 750 ) {
			per1 = TBhead.SVsper[2];
			if( l_test( fh5.F57T ) )	szsg |= 0x0002;
			if( l_test( fh5.F58T ) )	szsg |= 0x0010;
			if( l_test( fh5.F510T ) )	szsg |= 0x0080;
			if( l_test( fh5.F511T ) )	szsg |= 0x0100;
		}
		else if( UP4 >= 750 ) {
			per1 = TBhead.SVsper[3];
			if( l_test( fh5.F57T ) )	szsg |= 0x0004;
			if( l_test( fh5.F58T ) )	szsg |= 0x0020;
			if( l_test( fh5.F59T ) )	szsg |= 0x0080;
			if( l_test( fh5.F511T ) )	szsg |= 0x0200;
		}
		else if( UP5 >= 750 ) {
			per1 = TBhead.SVsper[4];
			if( l_test( fh5.F57T ) )	szsg |= 0x0008;
			if( l_test( fh5.F58T ) )	szsg |= 0x0040;
			if( l_test( fh5.F59T ) )	szsg |= 0x0100;
			if( l_test( fh5.F510T ) )	szsg |= 0x0200;
		}
		else {
			fh5.SZsw = 0;
			if( kensg <= 2 )	goto CALQB;
			// 簡易課税　2種類以上の業種での特例計算チェック
			if( toku75( fh5.F56T, fh5.F57T,  fh5.F58T ) )		szsg |= 0x0001;
			if( toku75( fh5.F56T, fh5.F57T,  fh5.F59T ) )		szsg |= 0x0002;
			if( toku75( fh5.F56T, fh5.F57T,  fh5.F510T) )		szsg |= 0x0004;
			if( toku75( fh5.F56T, fh5.F57T,  fh5.F511T) )		szsg |= 0x0008;
			if( toku75( fh5.F56T, fh5.F58T,  fh5.F59T ) )		szsg |= 0x0010;
			if( toku75( fh5.F56T, fh5.F58T,  fh5.F510T) )		szsg |= 0x0020;
			if( toku75( fh5.F56T, fh5.F58T,  fh5.F511T) )		szsg |= 0x0040;
			if( toku75( fh5.F56T, fh5.F59T,  fh5.F510T) )		szsg |= 0x0080;
			if( toku75( fh5.F56T, fh5.F59T,  fh5.F511T) )		szsg |= 0x0100;
			if( toku75( fh5.F56T, fh5.F510T, fh5.F511T) )		szsg |= 0x0200;
		}
		
		memset( PW0, '\0', 6 );	l_input( PW0, "1000" );
		if( fh5.SZsw && l_test( fh5.F54T ) ) {
			memset( WORK1, '\0', 6 );
			sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per1 ) );
			l_input( WORK1, WORK0 );
			l_6calq( fh5.F519T, fh5.F54T, WORK1, PW0 );
		}
		for( i = 0; i < 10; ++i, (szsg >>= 1) ) {
			if( szsg & 0x01 ) {
				memset( val1,  '\0', 6 );	memset( val2, '\0', 6 );
				memset( WORK1, '\0', 6 );
				switch( i ) {
					case 9 :	memmove( val1, fh5.F516T, 6 );
								per1 = TBhead.SVsper[3];
								break;
					case 8 : case 7 :
								memmove( val1, fh5.F515T, 6 );
								per1 = TBhead.SVsper[2];
								break;
					case 6 : case 5 : case 4 :
								memmove( val1, fh5.F514T, 6 );
								per1 = TBhead.SVsper[1];
								break;
					default:	memmove( val1, fh5.F513T, 6 );
								per1 = TBhead.SVsper[0];
								break;
				}
				switch( i ) {
					case 0 :	per2 = TBhead.SVsper[1];	break;
					case 1 : case 4 :
								per2 = TBhead.SVsper[2];	break;
					case 2 : case 5 : case 7 :
								per2 = TBhead.SVsper[3];	break;
					default:	per2 = TBhead.SVsper[4];	break;
				}
				l_sub( val2, fh5.F512T, val1 );
				sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per1 ) );
				l_input( WORK1, WORK0 );
				if( l_test( val1 ) )	l_6calq( val1, val1, WORK1, PW0 );
				sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per2 ) );
				l_input( WORK1, WORK0 );
				if( l_test( val2 ) )	l_6calq( val2, val2, WORK1, PW0 );
				l_add( val1, val1, val2 );
				
				if( l_test( val1 ) ) {
					pArithEx->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)fh5.F512T, 7, 0 );
					pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)fh5.F54T, (unsigned char *)WORK1, 7, 0 );
				}
				else	memset( WORK0, '\0', 6 );
				
				if( l_cmp( fh5.F519T, WORK0 ) < 0 ) {
					if( (fh5.SZsw == 1) && (kensg <= 2) ) {
						fh5.SZsw = 0;
						goto CALQB;
					}
					memmove( fh5.F519T, WORK0, 6 );
					fh5.SZsw = (char) (i + 2);
				}
			}
		}
		if( fh5.SZsw ) {
			if( l_cmp( fh5.F518T, fh5.F519T ) <= 0 ) {
				Snh.Sn_TOKUR = 1;	Kenei |= 0x10;
			}
		}
		if( ! l_test( Kenuri_n ) ) {
			fhbf_clr( 1, 0 );
			return( 0 );
		}
CALQB:
		fh5.SBsw = 1;
		szsg = per1 = per2 = 0;
		memset( fh5.F519B, '\0', 6 );
		
		if( fh5.F51UPb >= 750 ) {
			per1 = TBhead.SVsper[0];
			if( l_test( fh5.F58B ) )	szsg |= 0x0001;
			if( l_test( fh5.F59B ) )	szsg |= 0x0002;
			if( l_test( fh5.F510B ) )	szsg |= 0x0004;
			if( l_test( fh5.F511B ) )	szsg |= 0x0008;
		}
		else if( fh5.F52UPb >= 750 ) {
			per1 = TBhead.SVsper[1];
			if( l_test( fh5.F57B ) )	szsg |= 0x0001;
			if( l_test( fh5.F59B ) )	szsg |= 0x0010;
			if( l_test( fh5.F510B ) )	szsg |= 0x0020;
			if( l_test( fh5.F511B ) )	szsg |= 0x0040;
		}
		else if( fh5.F53UPb >= 750 ) {
			per1 = TBhead.SVsper[2];
			if( l_test( fh5.F57B ) )	szsg |= 0x0002;
			if( l_test( fh5.F58B ) )	szsg |= 0x0010;
			if( l_test( fh5.F510B ) )	szsg |= 0x0080;
			if( l_test( fh5.F511B ) )	szsg |= 0x0100;
		}
		else if( fh5.F54UPb >= 750 ) {
			per1 = TBhead.SVsper[3];
			if( l_test( fh5.F57B ) )	szsg |= 0x0004;
			if( l_test( fh5.F58B ) )	szsg |= 0x0020;
			if( l_test( fh5.F59B ) )	szsg |= 0x0080;
			if( l_test( fh5.F511B ) )	szsg |= 0x0200;
		}
		else if( fh5.F55UPb >= 750 ) {
			per1 = TBhead.SVsper[4];
			if( l_test( fh5.F57B ) )	szsg |= 0x0008;
			if( l_test( fh5.F58B ) )	szsg |= 0x0040;
			if( l_test( fh5.F59B ) )	szsg |= 0x0100;
			if( l_test( fh5.F510B ) )	szsg |= 0x0200;
		}
		else {
			fh5.SBsw = 0;
			if( kensg <= 2 )	goto CALQ;
			// 簡易課税　2種類以上の業種での特例計算チェック
			if( toku75( fh5.F56B, fh5.F57B,  fh5.F58B ) )		szsg |= 0x0001;
			if( toku75( fh5.F56B, fh5.F57B,  fh5.F59B ) )		szsg |= 0x0002;
			if( toku75( fh5.F56B, fh5.F57B,  fh5.F510B) )		szsg |= 0x0004;
			if( toku75( fh5.F56B, fh5.F57B,  fh5.F511B) )		szsg |= 0x0008;
			if( toku75( fh5.F56B, fh5.F58B,  fh5.F59B ) )		szsg |= 0x0010;
			if( toku75( fh5.F56B, fh5.F58B,  fh5.F510B) )		szsg |= 0x0020;
			if( toku75( fh5.F56B, fh5.F58B,  fh5.F511B) )		szsg |= 0x0040;
			if( toku75( fh5.F56B, fh5.F59B,  fh5.F510B) )		szsg |= 0x0080;
			if( toku75( fh5.F56B, fh5.F59B,  fh5.F511B) )		szsg |= 0x0100;
			if( toku75( fh5.F56B, fh5.F510B, fh5.F511B) )		szsg |= 0x0200;
		}
		
		memset( PW0, '\0', 6 );	l_input( PW0, "1000" );		
		if( fh5.SBsw ) {
			memset( WORK1, '\0', 6 );
			sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per1 ) );
			l_input( WORK1, WORK0 );
			if( l_test( fh5.F54B ) )
				l_6calq( fh5.F519B, fh5.F54B, WORK1, PW0 );
		}
		for( i = 0; i < 10; ++i, (szsg >>= 1) ) {
			if( szsg & 0x0001 ) {
				memset( val1,  '\0', 6 );	memset( val2, '\0', 6 );
				memset( WORK1, '\0', 6 );
				switch( i ) {
					case 9 :	memmove( val1, fh5.F516B, 6 );
								per1 = TBhead.SVsper[3];
								break;
					case 8 : case 7 :
								memmove( val1, fh5.F515B, 6 );
								per1 = TBhead.SVsper[2];
								break;
					case 6 : case 5 : case 4 :
								memmove( val1, fh5.F514B, 6 );
								per1 = TBhead.SVsper[1];
								break;
					default:	memmove( val1, fh5.F513B, 6 );
								per1 = TBhead.SVsper[0];
								break;
				}
				switch( i ) {
					case 0 :	per2 = TBhead.SVsper[1];	break;
					case 1 : case 4 :
								per2 = TBhead.SVsper[2];	break;
					case 2 : case 5 : case 7 :
								per2 = TBhead.SVsper[3];	break;
					default:	per2 = TBhead.SVsper[4];	break;
				}
				l_sub( val2, fh5.F512B, val1 );
				sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per1 ) );
				l_input( WORK1, WORK0 );
				if( l_test( val1 ) )
					l_6calq( val1, val1, WORK1, PW0 );
				sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per2 ) );
				l_input( WORK1, WORK0 );
				if( l_test( val2 ) )
					l_6calq( val2, val2, WORK1, PW0 );
				l_add( val1, val1, val2 );
				
				if( l_test( val1 ) ) {
					pArithEx->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)fh5.F512B, 7, 0 );
					pArithEx->l_mul100( (unsigned char *)WORK0, (unsigned char *)fh5.F54B, (unsigned char *)WORK1, 7, 0 );
				}
				else	memset( WORK0, '\0', 6 );
				if( l_cmp( fh5.F519B, WORK0 ) < 0 ) {
					if( (fh5.SBsw == 1) && (kensg <= 2) ) {
						fh5.SBsw = 0;
						goto CALQ;
					}
					memmove( fh5.F519B, WORK0, 6 );
					fh5.SBsw = (char) (i + 2);
				}
			}
		}
	}
CALQ:
	return( FALSE );
}

// 付表３の 内容計算
int CShinCalculateW::fh3_set( void )
{
	char	PW0[6], PW1[6], dbuf[10];

	/* 限界控除前の税額	*/
	memset( fh3.F31, '\0', 6 );	memset( fh3.F34, '\0', 6 );
	memset( fh3.F32, '\0', 6 );	memset( fh3.F33, '\0', 6 );
	memset( fh3.F35, '\0', 6 );	memset( fh3.F36, '\0',6 );
	memset( Snd.Sn_GENKAI, '\0', 6 );
	if( l_test( fh1.F114C ) <= 0 ) {
		fh3.Fmon1 = fh3.Fmon2 = fh3.Fmon3 = 0;
		Gsw = 0;
		return( FALSE );
	}
	
	if( Gsw < 2 )	return( FALSE );
	else {
		// 限界控除前税額の発生チェック
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", Kazmon );
		memset( PW0, '\0', 6 );	l_input( PW0, WORK0 );
		memset( PW1, '\0', 6 );	l_input( PW1, "12" );
  		
		memmove( fh3.F31, fh1.F114C, 6 );
		// 限界控除制度廃止期間チェック
		
		// 月次推定課税範囲設定  金額 × 経過月／１２カ月
		l_6calq( WORK0, (char *)TBhead.SVgen0, PW0, PW1 );			//	下限
		l_6calq( (char *)fh3.JOGEN, (char *)TBhead.SVgen1, PW0, PW1 );		//	上限
		l_sub( fh3.SAGAK, TBhead.SVgen1, TBhead.SVgen0 );
		l_6calq( (char *)fh3.SAGAK, (char *)fh3.SAGAK, PW0, PW1 );			// 差額
		
		if( Kani >= 2 )		// 簡易課税
			memmove( fh3.KAURI, stval.kwbuns, 6 );			// 課税売上
		else				// 原則課税
			l_sub( fh3.KAURI, fh2.F24C,  fh2.F23B );		// 課税売上高
		//	限界控除適用の課税売上高範囲（下限）≦ 課税売上高 ＜（上限）
		if( l_cmp( fh3.KAURI, fh3.JOGEN ) < 0 ) {
			if( l_cmp( fh3.KAURI, WORK0 ) < 0 )	memmove( fh3.KAURI, WORK0, 6 );
			l_sub( WORK1, fh3.JOGEN, fh3.KAURI );
			l_6calq( (char *)fh3.GEN_A, (char *)fh3.F31, WORK1, fh3.SAGAK );
			gencalq( fh3.GEN_B, fh3.F31 );		// 改正法による限界控除税額
			
			sprintf_s( dbuf, sizeof( dbuf ), "%02d", fMON );
			l_pack( &fh3.Fmon1, dbuf, 2 );				// 適用日前月数
			sprintf_s( dbuf, sizeof( dbuf ), "%02d", rMON );
			l_pack( &fh3.Fmon2, dbuf, 2 );				// 適用日以後月数
			
			switch( Gsw ) {								// 限界控除税額
				case 102 :	memmove( fh3.F32, fh3.GEN_B, 6 );	break;
				case  12 :	memmove( fh3.F32, fh3.GEN_A, 6 );	break;
				case   2 :	if( l_cmp( fh3.GEN_A, fh3.GEN_B ) < 0 )
									memmove( fh3.F32, fh3.GEN_A, 6 );
							else	memmove( fh3.F32, fh3.GEN_B, 6 );
							break;
				default:	break;
			}
			if( l_cmp( fh3.F31, fh3.F32 ) < 0 )	memmove( fh3.F32, fh3.F31, 6 );
			l_sub( fh3.F33, fh3.F31, fh3.F32 );			// 差引税額
			memmove( Snd.Sn_GENKAI, fh3.F32, 6 );
			
			memset( PW0, '\0', 6 );
			// 地方消費税･課税標準の基礎金額
			if( l_test( fh1.F113B ) <= 0 )	return( FALSE );
			memmove( fh3.F34, fh1.F113B, 6 );
			sprintf_s( dbuf, sizeof( dbuf ), "%d", sMON );
			l_input( PW0, dbuf );
			sprintf_s( dbuf, sizeof( dbuf ), "%02d", sMON );
			l_pack( &fh3.Fmon3, dbuf, 2 );			// 平成９年４月以後の月数
			memset( dbuf, '\0', 10 );	l_input( dbuf, "12" );
			l_6calq( (char *)fh3.F35, (char *)fh3.F32, PW0, dbuf );	// 限界控除税額相当額
			if( l_cmp( fh3.F34, fh3.F35 ) > 0 )
				l_sub( fh3.F36, fh3.F34, fh3.F35 );	// 課税標準消費税額
		}
		else {
			Gsw = 0;
			memset( &fh3, '\0', sizeof( struct _F3 ) );
		}
	}
	return( FALSE );
}

// 申告書本表　内容計算
void CShinCalculateW::stm_calq( void )
{
	// 課税売上高
	// １．課税標準額
	memmove( shn.shin1, fh1.F13C, 6 );
	l_calq( shn.shin1, shn.shin1, 0 );
	// 2.消費税額
	memmove( shn.shin2, fh1.F16C, 6 );
	// 3.控除過大調整税額
	memmove( shn.shin3, fh1.F17C, 6 );
	// 4.控除対象仕入税額
	memmove( shn.shin4, fh1.F18C, 6 );
	// 5.返還等対価に係る税額
	memmove( shn.shin5, fh1.F19C, 6 );
	// 6.貸倒に係る税額
	memmove( shn.shin6, fh1.F110C, 6 );
	// 7.控除税額小計
	l_add( shn.shin7, shn.shin4, shn.shin5 );
	l_add( shn.shin7, shn.shin7, shn.shin6 );
	// 8.控除不足還付税額 ・ 9.差引税額（２＋３－７）
	fh3_set();
	memset( shn.shin8, '\0', 6 );
	memset( shn.shin9, '\0', 6 );
	if( TBhead.SVmzsw ) {
		if( l_test( fh1.F114C ) < 0 ) {
			if( (Snh.Sn_SKKBN&0xff) % 2 ) {		// 中間申告以外
				memmove( shn.shin8, fh1.F114C, 6 );
				l_neg( shn.shin8 );
			}
		}
		else {
			if( l_test( Snd.Sn_GENKAI ) )	memmove( shn.shin9, fh3.F33, 6 );
			else							memmove( shn.shin9, fh1.F114C, 6 );
			// １００以下の切り捨て
			l_calq( shn.shin9, shn.shin9, 0x10 );
		}
	}
	else {
		l_add ( shn.shin9, shn.shin2, shn.shin3 );	//
		l_sub ( shn.shin9, shn.shin9, shn.shin7 );
		// 差引税額がマイナスの時
		if ( l_test( shn.shin9 ) < 0 ) {
			if( (Snh.Sn_SKKBN&0xff) % 2 ) {		// 中間申告以外
				l_neg( shn.shin9 );
				memmove( shn.shin8, shn.shin9, 6 );
			}
			memset( shn.shin9, '\0', 6 );
		}
		else	l_calq( shn.shin9, shn.shin9, 0x10 );
	}
	// 10.中間納付税額
	if( (Snh.Sn_SKKBN&0xff) % 2 )				//	中間申告以外
		memmove( shn.shin10, Snd.Sn_TYNOFZ, 6 );
	// 11.納付税額（9-10）・ 12.中間納付還付
	memset( shn.shin11, '\0', 6 );
	memset( shn.shin12, '\0', 6 );
	l_sub ( shn.shin11, shn.shin9, shn.shin10 );
	if ( l_test( shn.shin11 ) < 0 ) {
		l_neg( shn.shin11 );
		memmove( shn.shin12, shn.shin11, 6 );
		memset( shn.shin11, '\0', 6 );
		l_calq( shn.shin12, shn.shin12, 0x10 );
	}
	else	l_calq( shn.shin11, shn.shin11, 0x10 );
	memset( shn.shin13, '\0', 6 );
	memset( shn.shin14, '\0', 6 );
	if( (Snh.Sn_SKKBN&0xff) > 2 ) {	// 確定修正・中間修正
		// 13.既確定税額
		memmove( shn.shin13, Snd.Sn_KAKTIZ, 6 );
		// 14.差引き納付税額
		if( l_test( shn.shin11 ) > 0 )	// 納付税額が有る場合
			l_sub( shn.shin14, shn.shin11, shn.shin13 );
		else {
			memmove( shn.shin14, shn.shin12, 6 );
			l_neg( shn.shin14 );
			l_sub( shn.shin14, shn.shin14, shn.shin13 );
		}
		l_sub( shn.shin14, shn.shin14, shn.shin8 );
		// １００以下の切り捨て
		l_calq( shn.shin14, shn.shin14, 0x10 );
	}
	if( Kani < 2 ) {
		memmove( shn.shin15, fh2.F24C, 6 );			// 課税売上高
		memmove( shn.shin16, fh2.F27C, 6 );			// 総売上高
	}
	else {
		memmove( shn.shin15, stval.kwbuns,  6 );	// 課税売上高
		memmove( shn.shin16, Snd.Sn_ZZURI, 6 );		// 基準期間の課税売上高
	}
	
	if( ! ((Snh.Sn_SKKBN&0xff) % 2) && N_ver != 2 ); // 中間且改正日前開始
	else {
		// 17.控除不足還付税額（地方消費税）
		memset( shn.shin17, '\0', 6 );
		if( (Snh.Sn_SKKBN&0xff) % 2 ) {				// 中間申告以外
			if( TBhead.SVmzsw )	memmove( shn.shin17, fh1.F112B, 6 );
			else				memmove( shn.shin17, shn.shin8, 6 );
		}
		// 18.差引税額（地方消費税）
		memset( shn.shin18, '\0', 6 );
		if( l_test( fh1.F114C ) < 0 && ! ((Snh.Sn_SKKBN&0xff) % 2) ) ;
		else {				// 還付 中間申告以外
			if( TBhead.SVmzsw ) {
				if( l_test( Snd.Sn_GENKAI ) )
						memmove( shn.shin18, fh3.F36,   6 );
				else	memmove( shn.shin18, fh1.F113B, 6 );
			}
			else		memmove( shn.shin18, shn.shin9, 6 );
		}
		// １００以下の切り捨て
		if( l_test( shn.shin18 ) )	l_calq( shn.shin18, shn.shin18, 0x10 );
		// 19.譲渡割額還付額 ･ 20.譲渡割額納税額（地方消費税）
		memset( shn.shin19, '\0', 6 );
		memset( shn.shin20, '\0', 6 );
		memset( WORK0, '\0', 6 );	l_input( WORK0, "250" );
		if( l_test( shn.shin17 ) )
			pArithEx->l_mul100( (unsigned char *)shn.shin19, (unsigned char *)shn.shin17, (unsigned char *)WORK0, 1, 0 );
		if( l_test( shn.shin18 ) ) {
			pArithEx->l_mul100( (unsigned char *)shn.shin20, (unsigned char *)shn.shin18, (unsigned char *)WORK0, 1, 0 );
			l_calq( shn.shin20, shn.shin20, 0x10 );
		}
		//	21.中間納付譲渡割額
		if( (Snh.Sn_SKKBN&0xff) % 2 )				// 中間申告以外
			memmove( shn.shin21, Snd.Sn_TTYWAR, 6 );
		// 22.納付譲渡割額（20-21）・ 23.中間還付譲渡割額
		memset( shn.shin22, '\0', 6 );
		memset( shn.shin23, '\0', 6 );
		l_sub( shn.shin22, shn.shin20, shn.shin21 );
		// １００以下の切り捨て
		l_calq( shn.shin22, shn.shin22, 0x10 );
		if( l_test( shn.shin22 ) < 0 ) {
			l_neg( shn.shin22 );
			memmove( shn.shin23, shn.shin22, 6 );
			memset( shn.shin22, '\0', 6 );
		}
		memset( shn.shin24, '\0', 6 );
		memset( shn.shin25, '\0', 6 );
		if( (Snh.Sn_SKKBN&0xff) > 2 ) {	//	確定修正・中間修正
			//	24.既確定税額
			memmove( shn.shin24, Snd.Sn_TKAKTZ, 6 );
			// 25.差引納付譲渡割額
			if( l_test( shn.shin22 ) > 0 )	// 納付税額が有る場合
				memmove( shn.shin25, shn.shin22, 6 );
			else {
				l_add( shn.shin25, shn.shin19, shn.shin23 );
				if( l_test( shn.shin25 ) )	l_neg( shn.shin25 );
			}
			l_sub( shn.shin25, shn.shin25, shn.shin24 );
			// １００以下の切り捨て
			l_calq( shn.shin25, shn.shin25, 0x10 );
		}
	}
	// 26.消費税及び地方消費税の合計（納付又は還付）税額
	memset( shn.shin26, '\0', 6 );
	memset( WORK0, '\0', 6 );
	switch( Snh.Sn_SKKBN&0xff ) {
		case 1 : case 2 :		// 確定・中間申告
			l_add( shn.shin26,  shn.shin11,  shn.shin22 );
			l_add( WORK0, shn.shin8,   shn.shin12 );
			l_add( WORK0, WORK0, shn.shin19 );
			l_add( WORK0, WORK0, shn.shin23 );
			l_sub( shn.shin26,  shn.shin26,  WORK0 );
			break;
		default	:				// 修正申告
			l_add( shn.shin26,  shn.shin14,  shn.shin25 );
			break;
	}
}

// 申告書マスタ－用　計算
int CShinCalculateW::stm_old( unsigned char ksg )
{
	char	WORK0[6], WORK1[6], WORK2[6], WORK3[6], WORK4[6], 
			PW0[6], PW1[6], PW3[6];
	ushort	per;

	// 課税標準額
	memmove( Stm.Kstd, stval.kazeiu_3, 6 );
	
	// 対価の返還等をした金額・税額
	memmove( Stm.Tika,  stval.knebik_3, 6 );
	memmove( Stm.Hkanz, stval.henkan_3, 6 );
	
	// 貸倒回収に係る税額
	memmove( Stm.Kjadjz, stval.kaishu_3, 6 );

	// 消費税額
	memmove( Stm.Sytx, stval.urisyz_3, 6 );
	
	// 控除対象仕入税額５
	memset( WORK0, '\0', 6 );	memset( WORK2, '\0', 6 );
	memset( WORK3, '\0', 6 );	memset( WORK4, '\0', 6 );
	memset( PW0, '\0', 6 );		l_input( PW0, "1000" );
	if( Kani >= 2 ) {
		memmove( Stm.Kuri1, stval.knuri1, 6 );	// 第１種事業課税売上高
		memmove( Stm.Kuri2, stval.knuri2, 6 );	// 第２種事業課税売上高
		memmove( Stm.Kuri3, stval.knuri3, 6 );	// 第３種事業課税売上高
		memmove( Stm.Kuri4, stval.knuri4, 6 );	// 第４種事業課税売上高
		memset( WORK0, '\0', 6 );
		l_print( WORK0, stval.knwai1, "SSSS" );
		Stm.Up1 = (ushort) atoi( WORK0 );			// 第１種事業　売上割合
		l_print( WORK0, stval.knwai2, "SSSS" );
		Stm.Up2 = (ushort) atoi( WORK0 );			// 第２種事業　売上割合
		l_print( WORK0, stval.knwai3, "SSSS" );
		Stm.Up3 = (ushort) atoi( WORK0 );			// 第３種事業　売上割合
		l_print( WORK0, stval.knwai4, "SSSS" );
		Stm.Up4 = (ushort) atoi( WORK0 );			// 第４種事業　売上割合
		Stm.Up5 = 0;								// 第５種事業　　　〃
		
		// 単一事業のみ発生で返還等対価に係る税額が無い場合のみ簡便法
		if( kensg == 1 ) {
			switch( ksg ) {
				case 0x01 :	per = TBhead.SVsper[0];	break;
				case 0x02 :	per = TBhead.SVsper[1];	break;
				case 0x04 :	per = TBhead.SVsper[2];	break;
				case 0x08 :	per = TBhead.SVsper[3];	break;
				case 0x10 :	per = TBhead.SVsper[4];	break;
				default:	per = 0;				break;
			}
			memset( WORK0, '\0', 6 );	memset( WORK1, '\0', 6 );
			sprintf_s( PW1, sizeof( PW1 ), "%d", ( 1000 - per ) );
			l_input( WORK0, PW1 );
			if( ! l_test( Stm.Hkanz ) && ! l_test( Stm.Kjadjz ) )	//簡便法
				memmove( WORK1, Stm.Sytx, 6 );
			else {
				l_sub( WORK1, Stm.Sytx, Stm.Hkanz );	//-対価返還等に係る税額
				l_add( WORK1, WORK1,    Stm.Kjadjz );	//+貸倒回収に係る税額
			}
			l_6calq( (char *)Stm.Sirez, WORK1, WORK0, PW0 );
		}
		else {														//原則法
			l_add( stval.koujo6, stval.koujo1, stval.koujo2 );
			l_add( stval.koujo6, stval.koujo6, stval.koujo3 );
			l_add( stval.koujo6, stval.koujo6, stval.koujo4 );
			l_add( stval.koujo6, stval.koujo6, stval.koujo5 );
			percent( WORK0, Kenuri, 1, 30, 0 );
			pArithEx->l_par100( (unsigned char *)WORK1, (unsigned char *)stval.koujo6, (unsigned char *)WORK0, 7, 0 );
			l_sub( WORK2, Stm.Sytx, Stm.Hkanz );	//-対価返還等に係る税額
			l_add( WORK2, WORK2,    Stm.Kjadjz );	//+貸倒回収に係る税額
			pArithEx->l_mul100( (unsigned char *)Stm.Sirez, (unsigned char *)WORK2, (unsigned char *)WORK1, 7, 0 );
			
			if( Kenei && toku ) {		/*	兼営事業者 ＆ 特例計算適用可能  */
				if( toku & 0xf0 ) {
					switch( toku & 0xf0 ) {
						case 0x10 :		//第一種が主
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[0]) );
							memmove( WORK3, stval.knzei1, 6 );
							break;
						case 0x20 :		//第二種が主
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[1]) );
							memmove( WORK3, stval.knzei2, 6 );
							break;
						case 0x40 :		/* 第三種が主 */
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[2]) );
						memmove( WORK3, stval.knzei3, 6 );
							break;
						default :	break;
					}
					l_input( PW3, WORK4 );
					l_6calq( (char *)stval.koujo17, WORK3, PW3, PW0 );
					l_sub( WORK1, WORK0, WORK3 );
					
					switch( toku & 0x0f ) {
						case 0x02 :		// 第二種が副
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[1]) );
							break;
						case 0x04 :		// 第三種が副
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[2]) );
							break;
						case 0x08 :		// 第四種が副
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[3]) );
							break;
						default :	break;
					}
					l_input( PW3, WORK4 );
					l_6calq( (char *)stval.koujo18, WORK1, PW3, PW0 );
					l_add( stval.koujo16, stval.koujo17, stval.koujo18 );
					
					pArithEx->l_par100( (unsigned char *)WORK3, (unsigned char *)stval.koujo16, (unsigned char *)WORK0, 7, 0 );
					pArithEx->l_mul100( (unsigned char *)stval.koujo16,(unsigned char *) WORK2, (unsigned char *)WORK3, 7, 0 );
				}
				else {
					switch( toku & 0x0f ) {
						case 0x01 :		//第一種
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[0]) );
							break;
						case 0x02 :		//第二種
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[1]) );
							break;
						case 0x04 :		//第三種
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[2]) );
							break;
						case 0x08 :		// 第四種
							sprintf_s( WORK4, sizeof( WORK4 ), "%d", (1000 - TBhead.SVsper[3]) );
							break;
						default :	break;
					}
					l_input( PW3, WORK4 );
					l_6calq( (char *)stval.koujo16, WORK2, PW3, PW0 );
				}
				// 特例・原則　比較
				if( l_cmp( Stm.Sirez, stval.koujo16 ) > 0 )	toku = 0x00;
			}
		}
	}
	else	memmove( Stm.Sirez, stval.koujo0, 6 );		//原則課税
	
	// 貸し倒れに係る税額
	memmove( Stm.Ksitz, stval.ksidao_3, 6 );
	
	// 課税売上高
	memmove( Stm.Kzuri, stval.kwbuns, 6 );
	
	// 総売上高
	memmove( Stm.Souri, stval.kwbunb, 6 );
	
	// 貸倒回収に係る税額
	memmove( Stm.Kjadj7, Stm.Kjadjz, 6 );
	
	memmove( WORK0, Stm.Menzz, 6 );			//免税事業調整額
	l_add( Stm.Sirez8, Stm.Sirez, WORK0 );
	if( l_test( Stm.Sirez8 ) < 0 ) {		//仕入控除税額がﾏｲﾅｽ
		l_neg( Stm.Sirez8 );
		l_add( Stm.Kjadj7, Stm.Kjadj7, Stm.Sirez8 );
		l_clear( Stm.Sirez8 );	l_clear( WORK0 );
	}
	else	memmove( Stm.Sirez8, Stm.Sirez, 6 );
	//	控除税額小計
	l_add( Stm.Kojozk, Stm.Sirez8, WORK0 );
	l_add( Stm.Kojozk, Stm.Kojozk, Stm.Hkanz );
	l_add( Stm.Kojozk, Stm.Kojozk, Stm.Ksitz );
	
	// 限界控除前の税額
	l_add( Stm.Genmae, Stm.Sytx, Stm.Kjadj7 );
	// 控除不足還付税額
	if( l_cmp( Stm.Genmae, Stm.Kojozk ) >= 0 ) {
		l_sub( Stm.Genmae, Stm.Genmae, Stm.Kojozk );
		l_clear( Stm.Kanpuz );
	}
	else {
		l_sub( Stm.Kanpuz, Stm.Kojozk, Stm.Genmae );
		l_clear( Stm.Genmae );
	}
	
	// 差引納付税額４・５・６
	sprintf_s( WORK0, sizeof( WORK0 ), "%d", Kazmon );
	l_input( PW0, WORK0 );
	l_input( PW1, "12" );
	memset( Gkai_tb, '\0', sizeof( struct GETBL )*5 );
	
	// 月次推定課税範囲設定  金額 × 経過月／１２カ月
	l_6calq( WORK0, (char *)TBhead.SVgen0, PW0, PW1 );				//下限
	l_6calq( (char *)stval.genmax, (char *)TBhead.SVgen1, PW0, PW1 );		//上限
	l_sub( WORK2, TBhead.SVgen1, TBhead.SVgen0 );			//上限－下限
	l_6calq( WORK2, WORK2, PW0, PW1 );						//差額
	memmove( Gkai_tb[0].bef, stval.genmax, 6 );				//現行法上限
	memmove( Gkai_tb[1].bef, WORK2, 6 );					//現行法差額
	
	if( l_test( Stm.Genmae ) > 0 ) {
		// 限界控除適用の課税売上高範囲（上限）＜ 課税売上高
		if( l_cmp( Stm.Kzuri, stval.genmax ) < 0 ) {
			//	課税売上高 ＜ 限界控除適用範囲（下限）
			if( l_cmp( Stm.Kzuri, WORK0 ) < 0 ) {
				kani1 = 1;
				l_sub( WORK1, stval.genmax,  WORK0 );
			}
			else	l_sub( WORK1, stval.genmax, Stm.Kzuri );
			
			l_6calq( (char *)Stm.Genkai, (char *)Stm.Genmae, WORK1, WORK2 );	//限界控除税額
			memmove( Gkai_tb[2].bef, WORK1,  6 );		//現行法（上限-課売）
			memmove( Gkai_tb[3].bef, Stm.Genkai, 6 );	//現行法計算結果
			memmove( Gkai_tb[3].aft, Stm.Genmae, 6 );	//限界控除前税額
			
			gencalq( (char *)Stm.Genkai, (char *)Stm.Genmae );
			if( l_cmp( Stm.Genkai, Stm.Genmae ) > 0 )
				memmove( Stm.Genkai, Stm.Genmae, 6 );
			l_sub( Stm.Genato, Stm.Genmae, Stm.Genkai );	//差引税額
		}
		else {
			l_clear( Stm.Genkai );
			memmove( Stm.Genato, Stm.Genmae, 6 );
			Gsw = 0;
		}
	}
	else {
		l_clear( Stm.Genkai );	l_clear( Stm.Genato );
		Gsw = 0;
	}
	if( l_test( Stm.Genato ) )
		l_calq( (char *)Stm.Genato, (char *)Stm.Genato, 0x10 );		//１００円未満切り捨て
	
	if( ! (Skbn % 2) ) {		//中間申告時、控除不足還付税額は表示しない
		l_clear( Stm.Kanpuz );	l_clear( Stm.Tynofz );
	}
	
	if( l_test( Stm.Kanpuz ) ) {						//中間納付還付税額
		l_clear( Stm.Noufuz );
		if( l_test( Stm.Tynofz ) )	memmove( Stm.Tykanp, Stm.Tynofz, 6 );
		else						l_clear( Stm.Tykanp );
	}
	else {
		if( l_cmp( Stm.Genato, Stm.Tynofz ) >= 0 ) {	//納付税額
			l_sub( Stm.Noufuz, Stm.Genato, Stm.Tynofz );
			if( l_test( Stm.Noufuz ) )	l_calq( (char *)Stm.Noufuz, (char *)Stm.Noufuz, 0x10 );
			l_clear( Stm.Tykanp );
		}
		else {
			l_sub( Stm.Tykanp, Stm.Tynofz, Stm.Genato );//中間納付還付税額
			l_clear( Stm.Noufuz );
		}
	}
	
	l_clear( WORK0 );
	if( l_test( Stm.Noufuz ) )	l_add( WORK0, WORK0, Stm.Noufuz );
	if( l_test( Stm.Kanpuz ) )	l_add( WORK0, WORK0, Stm.Kanpuz );
	if( l_test( Stm.Tykanp ) )	l_add( WORK0, WORK0, Stm.Tykanp );
	
	// 修正申告
	if( Skbn >= 3 ) {
		l_sub( Stm.Synouf, WORK0, Stm.Kaktiz );			//差引納付税額
		if( l_test( Stm.Synouf ) )	l_calq( (char *)Stm.Synouf, (char *)Stm.Synouf, 0x10 );
	}
	else {
		l_clear( Stm.Kaktiz );
		l_clear( Stm.Synouf );
	}
	
	// 最終納付税額
	if( Skbn <= 2 )	memmove( Stm.Ednofz, Stm.Noufuz, 6 );
	else			memmove( Stm.Ednofz, Stm.Synouf, 6 );
	
	// 年３回中間申告
	switch( Snh.Sn_MCOUNT ) {
		case 3 :	// 年３回
			switch( Snh.Sn_JCOUNT ) {
				case 1 :	memmove( Stm.Tynof1, Stm.Ednofz, 6 );	break;
				case 2 :	memmove( Stm.Tynof2, Stm.Ednofz, 6 );	break;
				case 3 :	memmove( Stm.Tynof3, Stm.Ednofz, 6 );	break;
				case 4 :	memmove( Stm.Tynof4, Stm.Ednofz, 6 );	break;
				default:	break;
			}
			break;
		case 11:	// 年11回
			switch( Snh.Sn_JCOUNT ) {
				case 1 :	memmove( Stm.Tynof1, Stm.Ednofz, 6 );	break;
				case 2 :	memmove( Stm.Tynof2, Stm.Ednofz, 6 );	break;
				case 3 :	memmove( Stm.Tynof3, Stm.Ednofz, 6 );	break;
				case 4 :	memmove( Stm.Tynof4, Stm.Ednofz, 6 );	break;
				case 5 :	memmove( Stm.Tynof5, Stm.Ednofz, 6 );	break;
				case 6 :	memmove( Stm.Tynof6, Stm.Ednofz, 6 );	break;
				case 7 :	memmove( Stm.Tynof7, Stm.Ednofz, 6 );	break;
				case 8 :	memmove( Stm.Tynof8, Stm.Ednofz, 6 );	break;
				case 9 :	memmove( Stm.Tynof9, Stm.Ednofz, 6 );	break;
				case 10:	memmove( Stm.Tynof10, Stm.Ednofz, 6 );	break;
				case 11:	memmove( Stm.Tynof11, Stm.Ednofz, 6 );	break;
				default:	break;
			}
			break;
		default:	// 年１回
			break;
	}
	return( FALSE );
}

#ifdef	VERSIONUP
////////////////////
// 特定収入の計算 //
////////////////////

// 特定収入に係る調整税額計算
void CShinCalculateW::SPassOffCalq( void )
{ 
	char	WORK0[6], WORK1[6], WORK2[6], PW0[6], PW6[6]; 

	// 調整額計算用　課税売上割合（分母） 課税売上＋非課税売上（非課税有価証券全額分）
	memset( WORK0, '\0', 6 );
	l_sub( WORK0, stval.SpKuwb, stval.SpYuk5 );
	l_add( stval.SpTuwb, WORK0, stval.SpYuka );
	// 売上返還を差し引く
	l_add( stval.SpTuwb, stval.SpTuwb, stval.SpUnbg );

	// 特定収入割合
	// 分母
	memset( stval.SpTswb, '\0', 6 );
	l_add( stval.SpTswb, stval.SpTuwb, stval.SpGsyu );

	// 特定収入割合
	pArithEx->l_par100( (unsigned char*)stval.SpTswr, (unsigned char*)stval.SpGsyu,
											(unsigned char*)stval.SpTswb, 1, 9 );
	memset( PW0, '\0', 6 );
	l_input( PW0, "50" );

	if( l_cmp( stval.SpTswr, PW0 ) > 0 ) {	// 特例収入割合５％以上	
		
		// 調整割合
		l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
		pArithEx->l_par100( (unsigned char *)stval.SpTwar, (unsigned char *)stval.SpFsyu,
											(unsigned char *)stval.SpTywb, 2, 0 );
		// 特定収入に係る税額
		memset( PW0, '\0', 6 );
		l_input( PW0, RatioAsc );
//★//[12'03.12]///
//		if( l_cmp( stval.SpUwri, PW0 ) >= 0 ) {							// 課税売上割合９５％以上
///////////////////
		int	m_Junction = 0;
		if( m_Compare == 0 )	{
			if( l_cmp( stval.SpUwri, PW0 ) >= 0 )	m_Junction = 1;
		}
		else	{
			// シミュレーションする
			if(  m_Calcusw == 0 )					m_Junction = 1;
		}
		if( m_Junction )	{	// 課税売上割合９５％以上
///////////////////
			memset( WORK0, '\0', 6 );
			l_add( WORK0, WORK0, stval.SpTsyu );
			l_add( WORK0, WORK0, stval.SpKsyu );
			l_add( WORK0, WORK0, stval.SpHsyu );
			// 特定収入税額 合計
			percent( stval.SpGszg, WORK0, 4, 40, 0 );					// ４０／１０５０
		}
		else {															// 課税売上割合９５％以下
			if ( Anbun )	{
			// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額
				percent( stval.SpTszg, stval.SpTsyu, 4, 40, 0 );		// ４０／１０５０
				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', 6 );
				memset( WORK1, '\0', 6 );	
				percent( WORK0, stval.SpKsyu, 4, 40, 0 );				// ４０／１０５０

				memmove( stval.SpZszg, WORK0, 6 );						// 売上割合計算前の税額

				pArithEx->l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( l_cmp( stval.SpKuws, PW6 ) < 0 )	memset( WORK1, '\0', 6 );
				if( l_cmp( stval.SpKuwb, PW6 ) < 0 )	memset( WORK1, '\0', 6 );

				pArithEx->l_mul100( (unsigned char *)stval.SpKszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
			// 特定収入税額 合計
				l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
			}
			else {
			// 控除対象仕入税額 （一括比例配分方式）
				// 特定収入税額＝（特定収入＋共特定収入）ｘ４０／１０５０
				memset( stval.SpKszg, '\0', 6 );
				memset( WORK0, '\0', 6 );
				memset( WORK1, '\0', 6 );
				percent( WORK0, stval.SpTsyu, 4, 40, 0 );				// ４０／１０５０
			
				memmove( stval.SpZszg, WORK0, 6 );						// 売上割合計算前の税額

				// 特定収入税額Ｘ課税売上割合
				pArithEx->l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( l_cmp( stval.SpKuws, PW6 ) < 0 )	memset( WORK1, '\0', 6 );
				if( l_cmp( stval.SpKuwb, PW6 ) < 0 )	memset( WORK1, '\0', 6 );

				pArithEx->l_mul100( (unsigned char *)stval.SpTszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
				
				// 特定収入税額 合計
				l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
			}
		}
		// 通常の控除額－特定収入税額
		memset( WORK0, '\0', 6 );
		l_sub( WORK0, stval.SpKgzg, stval.SpGszg );
		// 調整割合額
		l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
		memset( WORK2, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpFsyu, (unsigned char *)stval.SpTywb, 5, 0 );
		pArithEx->l_mul100( (unsigned char *)stval.SpTwag, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );

		// 減額調整額
		l_add( stval.SpGgtg, stval.SpGszg, stval.SpTwag );

		// 控除対象仕入税額
		l_sub( stval.SpSizg, stval.SpKgzg, stval.SpGgtg );
	}
#ifdef	_DEBUG_EX
char	my1[20], my2[20], my3[20], my4[20], my5[20], my6[20], my7[20], my8[20], my9[20], my10[20];
l_print( my1, stval.SpTsyu, "SSS,SSS,SSS,SS9" );
l_print( my2, stval.SpKsyu, "SSS,SSS,SSS,SS9" );
l_print( my3, stval.SpHsyu, "SSS,SSS,SSS,SS9" );
l_print( my4, stval.SpKgzg, "SSS,SSS,SSS,SS9" );
l_print( my5, stval.SpGszg, "SSS,SSS,SSS,SS9" );
l_print( my6, stval.SpFsyu, "SSS,SSS,SSS,SS9" );
l_print( my7, stval.SpTuwb, "SSS,SSS,SSS,SS9" );
l_print( my8, stval.SpTwag, "SSS,SSS,SSS,SS9" );
l_print( my9, stval.SpGgtg, "SSS,SSS,SSS,SS9" );
l_print( my10, stval.SpSizg, "SSS,SSS,SSS,SS9" );
TRACE( "\n\n(NO:%d) 特定(%s) 共特定(%s) 非特定(%s)", m_Calcusw, my1, my2, my3 );
TRACE( "\n仕入控除額        (%s) - 特定収入税額(%s) ", my4, my5 );
TRACE( "\n不特定調整額      (%s) ／ 調整額合計 (%s) ", my6, my7 );
TRACE( "\n調整額合計        (%s) ", my8 );
TRACE( "\n減額調整額        (%s) ", my9 );
TRACE( "\n控除対象仕入税額  (%s)\n\n ", my10 );
#endif
}

// 特定収入に係る調整税額計算
void CShinCalculateW::SPassOnCalq( void )
{ 
	char	WORK0[6], WORK1[6], WORK2[6], PW0[6], PW6[6]; 

	// 調整額計算用　課税売上割合（分母） 課税売上＋非課税売上（非課税有価証券全額分）
	memset( WORK0, '\0', 6 );
	l_sub( WORK0, stval.SpKuwb, stval.SpYuk5 );
	l_add( stval.SpTuwb, WORK0, stval.SpYuka );
	// 売上返還を差し引く
	l_add( stval.SpTuwb, stval.SpTuwb, stval.SpUnbg );

	// 特定収入割合

	// 分母
	memset( stval.SpTswb, '\0', 6 );
	l_add( stval.SpTswb, stval.SpTuwb, stval.SpGsyu );

	// 特定収入割合
	// [04'07.29]
	pArithEx->l_par100( (unsigned char*)stval.SpTswr, (unsigned char*)stval.SpGsyu,
											(unsigned char*)stval.SpTswb, 1, 9 );
	memset( PW0, '\0', 6 );
	// [04'07.29]
	l_input( PW0, "50" );

	if( l_cmp( stval.SpTswr, PW0 ) > 0 ) {								// 特例収入割合５％以上	
		
		// 調整割合
		l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
		pArithEx->l_par100( (unsigned char *)stval.SpTwar, (unsigned char *)stval.SpFsyu,
											(unsigned char *)stval.SpTywb, 2, 0 );
		// 特定収入に係る税額
		memset( PW0, '\0', 6 );
		l_input( PW0, RatioAsc );
//★//[12'03.12]///
//		if( l_cmp( stval.SpUwri, PW0 ) >= 0 ) {							// 課税売上割合９５％以上
///////////////////
		int	m_Junction = 0;
		if( m_Compare == 0 )	{
			if( l_cmp( stval.SpUwri, PW0 ) >= 0 )	m_Junction = 1;
		}
		else	{
			// シミュレーションする
			if(  m_Calcusw == 0 )					m_Junction = 1;
		}
		if( m_Junction )	{	// 課税売上割合９５％以上
///////////////////
			memset( WORK0, '\0', 6 );
			l_add( WORK0, WORK0, stval.SpTsyu );
			l_add( WORK0, WORK0, stval.SpKsyu );
			l_add( WORK0, WORK0, stval.SpHsyu );
			// 特定収入税額 合計
			percent( stval.SpGszg, WORK0, 4, 40, 0 );					// ４０／１０５０
		}
		else {															// 課税売上割合９５％以下
			if ( Anbun )	{
				// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額	
				percent( stval.SpTszg, stval.SpTsyu, 4, 40, 0 );		// ４０／１０５０
				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', 6 );
				memset( WORK1, '\0', 6 );
				percent( WORK0, stval.SpKsyu, 4, 40, 0 );				// ４０／１０５０
				memmove( stval.SpZszg, WORK0, 6 );						// 売上割合計算前の税額
				pArithEx->l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( l_cmp( stval.SpKuws, PW6 ) < 0 )	memset( WORK1, '\0', 6 );
				if( l_cmp( stval.SpKuwb, PW6 ) < 0 )	memset( WORK1, '\0', 6 );

				pArithEx->l_mul100( (unsigned char *)stval.SpKszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
				// 特定収入税額 合計
				l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
			}
			else {
			// 控除対象仕入税額 （一括比例配分方式）
				// 特定収入税額＝（特定収入＋共特定収入）ｘ４０／１０５０
				memset( WORK0, '\0', 6 );
				memset( WORK1, '\0', 6 );
				percent( WORK0, stval.SpTsyu, 4, 40, 0 );				// ４０／１０５０
				memmove( stval.SpZszg, WORK0, 6 );						// 売上割合計算前の税額
				// 特定収入税額Ｘ課税売上割合
				pArithEx->l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( l_cmp( stval.SpKuws, PW6 ) < 0 )	memset( WORK1, '\0', 6 );
				if( l_cmp( stval.SpKuwb, PW6 ) < 0 )	memset( WORK1, '\0', 6 );

				pArithEx->l_mul100( (unsigned char *)stval.SpTszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
				// 特定収入税額 合計
				l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
			}
		}
		// 配賦割合
		l_add( stval.SpHfgs, stval.SpHfgs, stval.SpKsi4 );
		l_sub( stval.SpHfgs, stval.SpHfgs, stval.SpTsyu );
		l_sub( stval.SpHfgs, stval.SpHfgs, stval.SpKsb4 );

		l_add( stval.SpHfgb, stval.SpHfgb, stval.SpKsig );
		l_sub( stval.SpHfgb, stval.SpHfgb, stval.SpTsyu );
		l_sub( stval.SpHfgb, stval.SpHfgb, stval.SpKsbg );

		pArithEx->l_par100( (unsigned char *)stval.SpKshf, (unsigned char *)stval.SpHfgs,
											(unsigned char *)stval.SpHfgb, 2, 0 );
		// 通常の控除額－特定収入税額
		l_sub( WORK0, stval.SpKgzg, stval.SpGszg );
		// 調整割合額
		l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
		memset( WORK2, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpFsyu, (unsigned char *)stval.SpTywb, 5, 0 );
		pArithEx->l_mul100( (unsigned char *)stval.SpTwag, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );

		// 新税率適用分の配布額

		//	配賦割合額
		memset( WORK2, '\0', 6 );
		pArithEx->l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpTwag, (unsigned char *)stval.SpHfgb, 5, 0 );
		pArithEx->l_mul100( (unsigned char *)stval.SpKsh4, (unsigned char *)stval.SpHfgs, (unsigned char *)WORK2, 5, 0 );

		// 減額調整額
		l_add( stval.SpGgt4, stval.SpGszg, stval.SpKsh4 );
		// 控除対象仕入税額
		l_sub( stval.SpSiz4, stval.SpKgz4, stval.SpGgt4 );

		// 旧税率適用分の配布額

		// 特定収入に係る課税仕入れ等の合計
		memset( WORK0, '\0', 6 );
		l_add( WORK0, stval.SpTwag, stval.SpGszg );
		l_sub( stval.SpKsh3, WORK0, stval.SpGgt4 );

		// 減額調整額
		l_add( stval.SpGgtg, stval.SpGszg, stval.SpTwag );

		// 控除対象仕入税額
		l_sub( stval.SpSiz3, stval.SpKgz3, stval.SpKsh3 );

		//
		l_add( stval.SpSizg, stval.SpSiz3, stval.SpSiz4 );
	}
#ifdef	_DEBUG_EX
char	my1[20], my2[20], my3[20], my4[20], my5[20], my6[20], my7[20], my8[20], my9[20], my10[20];
l_print( my1, stval.SpTsyu, "SSS,SSS,SSS,SS9" );
l_print( my2, stval.SpKsyu, "SSS,SSS,SSS,SS9" );
l_print( my3, stval.SpHsyu, "SSS,SSS,SSS,SS9" );
l_print( my4, stval.SpKgzg, "SSS,SSS,SSS,SS9" );
l_print( my5, stval.SpGszg, "SSS,SSS,SSS,SS9" );
l_print( my6, stval.SpFsyu, "SSS,SSS,SSS,SS9" );
l_print( my7, stval.SpTuwb, "SSS,SSS,SSS,SS9" );
l_print( my8, stval.SpTwag, "SSS,SSS,SSS,SS9" );
l_print( my9, stval.SpGgtg, "SSS,SSS,SSS,SS9" );
l_print( my10, stval.SpSizg, "SSS,SSS,SSS,SS9" );
TRACE( "\n\n(NO:%d) 特定(%s) 共特定(%s) 非特定(%s)", m_Calcusw, my1, my2, my3 );
TRACE( "\n仕入控除額        (%s) - 特定収入税額(%s) ", my4, my5 );
TRACE( "\n不特定調整額      (%s) ／ 調整額合計 (%s) ", my6, my7 );
TRACE( "\n調整額合計        (%s) ", my8 );
TRACE( "\n減額調整額        (%s) ", my9 );
TRACE( "\n控除対象仕入税額  (%s)\n\n ", my10 );
#endif
}
#endif

// 課税売上高５億円チェック
//	1: ５億円超え
//  2: ４億五千万以上５億円未満
//  3: ５億円以下
int CShinCalculateW::YearConvert( char *kzuanbun, char *kazeiuri )
{
	int		MKno, SKno, EKno;
	char	WKS[6], WKB[6], WK1[6], WK2[6], WK3[6], CBF[20];
	
	// 開始オフセットの取得
	if( shin_ofst( &Snh.Sn_KDAYS[1], &SKno, &Vol1, &Vol2 ) )	SKno = 0;
	// 最終オフセットの取得
	if( shin_ofst( &Snh.Sn_KDAYE[1], &EKno, &Vol1, &Vol2 ) )	EKno = 11;
	MKno = ( EKno - SKno ) + 1;
	memmove( WK1, kazeiuri, 6 );		// 課税売上高
	l_input( WK2, "500000000" );		// ５億
	l_input( WK3, "450000000" );		// ４億５千万
	if( MKno != 12 ) {
		sprintf_s( CBF, "%d", MKno );
		l_input( WKB, CBF );				// * 分母
		l_input( WKS, "12" );				// / 分子
		l_div( WK1, WK1, WKB );				// sou = sou / 分母
		l_mul( WK1, WK1, WKS );				// sou = sou * 分子
		memmove( kzuanbun, WKS, 6 );		// 按分された金額をセット
	}

#ifdef	_DEBUG_EX
CString	Mes;
char	dbf1[20], dbf2[20], dbf3[20], dbf4[20];
l_print( dbf1, kazeiuri, "SSS,SSS,SSS,SS9" );
l_print( dbf2, WK1, "SSS,SSS,SSS,SS9" );
l_print( dbf3, WKS, "SSS,SSS,SSS,SS9" );
l_print( dbf4, WKB, "SSS,SSS,SSS,SS9" );
if( MKno == 12 )	Mes.Format( "\n課税売上高 %s 円", dbf1 );
else	{
	Mes.Format( "\n課税売上高 %s 円\n按分課税売上高 %s 円 = ( %s / %s ) * %s", dbf1, dbf2, dbf1, dbf4, dbf3 );
}
ICSMessageBox( Mes, MB_OK );
#endif

	if( l_cmp( WK1, WK2 ) > 0 )	return( 1 );									// ５億越え
	else	{
		if( l_cmp( WK1, WK3 ) >= 0 && l_cmp( WK1, WK2 ) <= 0 )	return( 2 );	// ４億５千万以下で５億以下
		else													return( 3 );	// ４億５千万未満
	}
}

// 課税方式比較表のパケットへデータを転送
void CShinCalculateW::SumilateConvert( SUMDATA* sumdata )
{
	char	WORK[6], a[6], b[6], c[6];

	// 計算用エリアクリア
	memset( &HANS, '\0', sizeof( SUMVAL )*30 );		// 本表用	(ANS)
	memset( &WANS, '\0', sizeof( SUMVAL )*10 );		// ワーク用	(WANS)
	memset( &FANS, '\0', sizeof( SUMVAL )*20 );		// 付表用	(W0～W10)

	//////////
	// 付表 //
	//////////
	l_add( FANS[0].val, Snd.Sn_2F1B, Snd.Sn_2F2B );
	l_add( FANS[0].val, FANS[0].val, Snd.Sn_2F3B );							//( 4)-C
	if( (Ver&0x0f) == 1 || ((Ver&0x0f) == 2 && TBhead.SVmzsw) ) {
		l_add( FANS[0].val, FANS[0].val, Snd.Sn_2F1A );
		l_add( FANS[0].val, FANS[0].val, Snd.Sn_2F2A );
		l_add( FANS[0].val, FANS[0].val, Snd.Sn_2F3A );
	}
	l_add( FANS[1].val, FANS[0].val, Snd.Sn_2F6B );							// ( 7)-C
	if( (Ver&0x0f) == 1 || ((Ver&0x0f) == 2 && TBhead.SVmzsw) ) 
							l_add( FANS[1].val, FANS[1].val, Snd.Sn_2F6A );
	
	// (免税)控除対象調整税額
	if( Snd.Sn_MENZsw & 0x10 ) {							// (12)-A･B
		memmove( FANS[2].val, Snd.Sn_MENZZ, 6 );
		memmove( FANS[3].val, Snd.Sn_MNZ40, 6 );
	}
	memset( WORK0, '\0', 6 );
	if( (Ver&0x0f) == 1 || ((Ver&0x0f) == 2 && TBhead.SVmzsw) ) {
		if( Snd.Sn_MENZsw & 0x01 ) {
			if( l_test( FANS[2].val ) )	l_neg( FANS[2].val );
		}
		l_add( FANS[4].val, Snd.Sn_2F9A, Snd.Sn_2F11A );
		l_add( FANS[4].val, FANS[4].val, FANS[2].val );
	}
	if( Snd.Sn_MENZsw & 0x01 ) {							// (13)-B
		if( l_test( FANS[3].val ) )	l_neg( FANS[3].val );
	}
	l_add( FANS[5].val, Snd.Sn_2F9B, Snd.Sn_2F11B );
	l_add( FANS[5].val, FANS[5].val, FANS[3].val );
	
	if( Snh.Sn_UPERS&0x10 ) {
		wsprintf( DBF, "%d", (Snd.Sn_2FUWSEX * 1000) );
		l_input( WORK0, DBF );
	}
	else	{
		pArithEx->l_par100( (unsigned char *)WORK0, (unsigned char *)FANS[0].val, (unsigned char *)FANS[1].val, 5, 0 );
	}

	// 売上割合がマイナスの場合は０％で計算する。
	memset( PW6, '\0', sizeof( PW6 ) );
	if( l_cmp( FANS[0].val, PW6 ) < 0 )	memset( WORK0, '\0', 6 ); 
	if( l_cmp( FANS[1].val, PW6 ) < 0 )	memset( WORK0, '\0', 6 );
	if( l_test( WORK0 ) == 0 )	{
		if( !(Snd.Sn_EXP100&0x01) )	Srx = 1;	// 95%以下
	}
	if( Anbun ) {			// 個別対応	:(17)-A･B
		if( (Ver&0x0f) == 1 || ((Ver&0x0f) == 2 && TBhead.SVmzsw) ) {
			pArithEx->l_mul100( (unsigned char *)FANS[6].val, (unsigned char *)Snd.Sn_2F16A, (unsigned char *)WORK0, 5, 0 );
			l_add( FANS[6].val, FANS[6].val, Snd.Sn_2F15A );
		}
		pArithEx->l_mul100( (unsigned char *)FANS[7].val, (unsigned char *)Snd.Sn_2F16B, (unsigned char *)WORK0, 5, 0 );
		l_add( FANS[7].val, FANS[7].val, Snd.Sn_2F15B );

	}
	else {							// 比例配分 :(18)-A･B
		if( (Ver&0x0f) == 1 || ((Ver&0x0f) == 2 && TBhead.SVmzsw) ) {
			pArithEx->l_mul100( (unsigned char *)FANS[6].val, (unsigned char *)FANS[4].val, (unsigned char *)WORK0, 5, 0 );
		}
		pArithEx->l_mul100( (unsigned char *)FANS[7].val, (unsigned char *)FANS[5].val, (unsigned char *)WORK0, 5, 0 );
	}
	
	if( (Ver&0x0f) == 1 || ((Ver&0x0f) == 2 && TBhead.SVmzsw) ) {	// (21)or(22)-A･B
		if( Snd.Sn_2F21sw&0x01 )	{
			if( l_test( Snd.Sn_2F21A )	)	{
				memmove( FANS[8].val, Snd.Sn_2F21A, 6 );		// 手入力
				memmove( Snd.Sn_SIREZ, Snd.Sn_2F21A, 6 );		// 手入力
			}
			else	{
				// 控除過大調整税額
				l_sub( FANS[8].val, Snd.Sn_1F7A, Snd.Sn_2F23A );
				l_neg( FANS[8].val );
			}
		}
		else	{
			l_add( FANS[8].val, Snd.Sn_2F19A, Snd.Sn_2F20A );
			if( ! Srx )	l_add( FANS[8].val, FANS[8].val, FANS[4].val );
			else		l_add( FANS[8].val, FANS[8].val, FANS[6].val );
		}
	}
	if( Snd.Sn_2F21sw&0x10 )	{
		if( l_test( Snd.Sn_2F21B ) )	{
			memmove( FANS[9].val, Snd.Sn_2F21B, 6 );		// 手入力優先
			memmove( Snd.Sn_SIREZ, Snd.Sn_2F21B, 6 );		// 手入力
		}
		else	{
			l_sub( FANS[9].val, Snd.Sn_1F7B, Snd.Sn_2F23B );
			l_neg( FANS[9].val );
		}
	}
	else	{
		l_add( FANS[9].val, Snd.Sn_2F19B, Snd.Sn_2F20B );
		if( ! Srx )	l_add( FANS[9].val, FANS[9].val, FANS[5].val );
		else		l_add( FANS[9].val, FANS[9].val, FANS[7].val );
	}

	// 残高変換
	if( (Ver&0x0f) == 1 || ((Ver&0x0f) == 2 && TBhead.SVmzsw) ) {
	// 経過措置あり
		// 課税仕入れに係る支払対価の額		⑧
		memset( WORK, '\0', 6 );
		l_add( WORK, Snd.Sn_2F8A, Snd.Sn_2F8B );
		memmove( sumdata->FHVal[0].val, WORK, 6 );
		// 課税仕入れに係る消費税額			⑨
		memset( WORK, '\0', 6 );
		l_add( WORK, Snd.Sn_2F9A, Snd.Sn_2F9B );
		memmove( sumdata->FHVal[1].val, WORK, 6 );
		// 課税貨物に係る消費税額			⑩
		memset( WORK, '\0', 6 );
		l_add( WORK, Snd.Sn_2F11A, Snd.Sn_2F11B );
		memmove( sumdata->FHVal[2].val, WORK, 6 );
		// 棚卸調整額						⑪
		memset( WORK, '\0', 6 );
		l_add( WORK, FANS[2].val, FANS[3].val );
		memmove( sumdata->FHVal[3].val, WORK, 6 );
		// 課税仕入れ等の税額の合計額		⑫
		memset( WORK, '\0', 6 );
		l_add( WORK, FANS[4].val, FANS[5].val );
		memmove( sumdata->FHVal[4].val, WORK, 6 );

		// 課税売上割合が９５％以上の場合	⑬
		memset( WORK, '\0', 6 );
		if( ! Srx ) {
			l_add( WORK, FANS[4].val, FANS[5].val );
			memmove( sumdata->FHVal[5].val, WORK, 6 );
		}
		// ⑫のうち課税に係る				⑭
		memset( WORK, '\0', 6 );
		if( Srx && Anbun ) {
			l_add( WORK, Snd.Sn_2F15A, Snd.Sn_2F15B );
			memmove( sumdata->FHVal[6].val, WORK, 6 );
		}

		// ⑫のうち共通に係る				⑮
		memset( WORK, '\0', 6 );
		if( Srx && Anbun ) {
			l_add( WORK, Snd.Sn_2F16A, Snd.Sn_2F16B );
			memmove( sumdata->FHVal[7].val, WORK, 6 );
		}
		// 95％未満及び５億超え
		memset( WORK, '\0', 6 );
		if( Srx ) {
			if( Anbun ) {
				l_add( WORK, FANS[6].val, FANS[7].val );
				memmove( sumdata->FHVal[8].val, WORK, 6 );		// 個別対応	⑯
			}
			else	{
				l_add( WORK, FANS[6].val, FANS[7].val );
				memmove( sumdata->FHVal[9].val, WORK, 6 );		// 比例配分	⑰
			}
		}
		// 控除税額の調整（１）				⑱
		memset( WORK, '\0', 6 );
		l_add( WORK, Snd.Sn_2F19A, Snd.Sn_2F19B );
		memmove( sumdata->FHVal[10].val, WORK, 6 );
		// 控除税額の調整（２）				⑲
		memset( WORK, '\0', 6 );
		l_add( WORK, Snd.Sn_2F20A, Snd.Sn_2F20B );
		memmove( sumdata->FHVal[11].val, WORK, 6 );

		// 控除対象仕入れ税額			⑳
		char ii = 0x00;
		if( l_test( FANS[8].val ) > 0 ) {					// （20）-A				//(21)-A
			ii |= 0x01;
		}
		if( l_test( FANS[9].val ) > 0 ) {					// （20）-B　　			//(21)-B
			ii |= 0x10;
		}
		memset( FANS[10].val, '\0', 6 );
		switch( ii & 0xff ) {						// （20）-C　　			//(21)-C
			case 0x11 :	l_add( FANS[10].val, FANS[8].val, FANS[9].val );	break;
			case 0x01 :	memmove( FANS[10].val, FANS[8].val, 6 );	break;
			case 0x10 :	memmove( FANS[10].val, FANS[9].val, 6 );	break;
			default   :	break;
		}
		if( ii ) {
			memmove( sumdata->FHVal[12].val, FANS[10].val, 6 );
		}

		// 控除過大調整税額				(21)
		ii = 0x00;
		if( l_test( FANS[8].val ) < 0 ) {					// （21）-A　			//(22)-A
			l_neg( FANS[8].val );
			ii |= 0x01;
		}
		if( l_test( FANS[9].val ) < 0 ) {					// （21）-B　　			//(22)-B
			l_neg( FANS[9].val );
			ii |= 0x10;
		}
		memset( FANS[10].val, '\0', 6 );
		switch( ii & 0xff ) {						// （21）-C	　　		//(22)-C
			case 0x11 :	l_add( FANS[10].val, FANS[8].val, FANS[9].val );	break;
			case 0x01 :	memmove( FANS[10].val, FANS[8].val, 6 );	break;
			case 0x10 :	memmove( FANS[10].val, FANS[9].val, 6 );	break;
			default   :	break;
		}
		if( ii ) {
			memmove( sumdata->FHVal[13].val, FANS[10].val, 6 );
		}
		// 貸倒回収に係る消費税額		(22)
		memset( WORK, '\0', 6 );
		l_add( WORK, Snd.Sn_2F23A, Snd.Sn_2F23B );
		memmove( sumdata->FHVal[14].val, Snd.Sn_2F23B, 6 );
	}
	else	{
	// 経過措置なし
		// 課税仕入れに係る支払対価の額		⑧
		memmove( sumdata->FHVal[0].val, Snd.Sn_2F8B, 6 );
		// 課税仕入れに係る消費税額			⑨
		memmove( sumdata->FHVal[1].val, Snd.Sn_2F9B, 6 );
		// 課税貨物に係る消費税額			⑩
		memmove( sumdata->FHVal[2].val, Snd.Sn_2F11B, 6 );
		// 棚卸調整額						⑪
		memmove( sumdata->FHVal[3].val, FANS[3].val, 6 );
		// 課税仕入れ等の税額の合計額		⑫
		memmove( sumdata->FHVal[4].val, FANS[5].val, 6 );
		// 課税売上割合が９５％以上の場合	⑬
		if( ! Srx )		memmove( sumdata->FHVal[5].val, FANS[5].val, 6 );
		// ⑫のｐうち課税に係る				⑭
		if( Srx && Anbun ) memmove( sumdata->FHVal[6].val, Snd.Sn_2F15B, 6 );
		// ⑫のうち共通に係る				⑮
		if( Srx && Anbun ) memmove( sumdata->FHVal[7].val, Snd.Sn_2F16B, 6 );
		// 95％未満及び５億超え
		if( Srx ) {
			if( Anbun ) memmove( sumdata->FHVal[8].val, FANS[7].val, 6 );		// 個別対応	⑯
			else		memmove( sumdata->FHVal[9].val, FANS[7].val, 6 );		// 比例配分	⑰
		}
		// 控除税額の調整（１）				⑱
		memmove( sumdata->FHVal[10].val, Snd.Sn_2F19B, 6 );
		// 控除税額の調整（２）				⑲
		memmove( sumdata->FHVal[11].val, Snd.Sn_2F20B, 6 );

		if( l_test( FANS[9].val ) > 0 ) {
			// 控除対象仕入れ税額			⑳
			memmove( sumdata->FHVal[12].val, FANS[9].val, 6 );
		}
		else if( l_test( FANS[9].val ) < 0 ) {
			l_neg( FANS[9].val );
			// 控除過大調整税額				(21)
			memmove( sumdata->FHVal[13].val, FANS[9].val, 6 );
		}
		// 貸倒回収に係る消費税額
		memmove( sumdata->FHVal[14].val, Snd.Sn_2F23B, 6 );

	}
	//////////
	// 本表 //
	//////////
	char	vers = N_ver;
	switch( vers ) {
		case 1 :	if( ! TBhead.SVmzsw )	vers = 2;	break;
		case 2 :	if(   TBhead.SVmzsw )	vers = 1;	break;
		default:	break;
	}
	// 課税標準額
	// 普通乗用自動車 ６％分
	memmove( a, Snd.Sn_CK60, 6 );
	p_calq( (char *)WANS[0].val, (char *)a, 0 );				// １０００円未満　切り捨て	
	/* 普通乗用自動車 ４．５％分 */
	memmove( a, Snd.Sn_CK45, 6 );
	p_calq( (char *)WANS[1].val, (char *)a, 0 );				// １０００円未満　切り捨て	
	// 消費税額
	// 普通乗用自動車
	// 6%
	memmove( WANS[2].val, Snd.Sn_CZ60, 6 );
	// 4.5%
	memmove( WANS[3].val, Snd.Sn_CZ45, 6 );	
	switch( vers&0x0f ) {
		case 1 :	// 経過措置対象
			// 課税標準額		
			memmove( a, Snd.Sn_1F1B, 6 );				// ４％分
			p_calq( (char *)WANS[4].val, (char *)a, 0 );				// １０００円未満　切り捨て			
			memmove( a, Snd.Sn_1F2A, 6 );				// ３％分
			p_calq( (char *)WANS[5].val, (char *)a, 0 );				// １０００円未満　切り捨て			
			l_add( HANS[0].val, WANS[4].val, WANS[5].val );			
			// 消費税額
			memmove( WANS[6].val, Snd.Sn_1F4B, 6 );		// ４％分			
			memmove( WANS[7].val, Snd.Sn_1F5A, 6 );		// ３％分			
			l_add( HANS[1].val, WANS[6].val, WANS[7].val );
			break;
		case 2 :	// 経過措置対象外
			// 課税標準額
			memmove( a, Snd.Sn_KSTD, 6 );
			p_calq( (char *)WANS[4].val, (char *)a, 0 );		// １０００円未満　切り捨て			
			// 消費税額
			memmove( WANS[6].val, Snd.Sn_SYTX, 6 );			
			memmove( HANS[0].val, WANS[4].val, 6 );
			memmove( HANS[1].val, WANS[6].val, 6 );
			break;
		default:	// 旧バージョンマスター
			// 普通乗用自動車
			memmove( a, Snd.Sn_KSTDM, 6 );
			p_calq( (char *)HANS[0].val, (char *)a, 0 );			// １０００円未満　切り捨て			
			// その他の課税分
			memmove( a, Snd.Sn_KSTD, 6 );
			p_calq( (char *)HANS[1].val, (char *)a, 0 );			// １０００円未満　切り捨て			
			// 計 (1 + 2)
			l_add( HANS[2].val, HANS[0].val, HANS[1].val );			
			// 消費税額
			memmove( HANS[3].val, Snd.Sn_SYTXM, 6 );			
			// その他の課税分
			memmove( HANS[4].val, Snd.Sn_SYTX, 6 );			
			// 計 (4 + 5)
			l_add( HANS[5].val, HANS[3].val, HANS[4].val );
			break;
	}
	
	// 簡易 -> 貸倒回収に係る or 税額一般 -> 控除過大調整税額
	if( vers )	memmove( HANS[2].val, Snd.Sn_KJADJZ, 6 );
	else {
		memmove( a, Snd.Sn_SIREZ, 6 );
		memmove( c, Snd.Sn_MENZZ, 6 );
		if( Snd.Sn_MENZsw & 0x01 ) {
			if( l_test( c ) )	l_neg( c );
		}
		l_add( a, a, c );
		memmove( b, Snd.Sn_KJADJZ, 6 );
		if( l_test( a ) < 0 ) {
				l_neg( a );
				l_add( HANS[6].val, b, a );
		}
		else	memmove( HANS[6].val, b, 6 );
	}
	// 控除税額
	// 全額控除で売上割合が0％
	if( m_Calcusw == 0  && l_test( stval.uwriai ) == 0 ) {
		if( l_test( Snd.Sn_1F14C ) < 0 )	l_add( Snd.Sn_1F14C, Snd.Sn_1F14C,  Snd.Sn_SIREZ );
		else								l_sub( Snd.Sn_1F14C, Snd.Sn_1F14C,  Snd.Sn_SIREZ );
		if( l_test( Snd.Sn_1F12B ) < 0 )	l_add( Snd.Sn_1F12B, Snd.Sn_1F12B,  Snd.Sn_SIREZ );
		else								l_sub( Snd.Sn_1F12B, Snd.Sn_1F12B,  Snd.Sn_SIREZ );
		memset( Snd.Sn_SIREZ, '\0', 6 );
	}
	// 控除対象仕入税額
	memmove( a, Snd.Sn_SIREZ, 6 );
//★[14'01.17]///////
//	if( m_Calcusw == 1 ) {
//		l_add( a, a, sumdata->FHVal[3].val );
//	}
/////////////////////
	if( ! vers )	l_add( a, a, c );
	if( l_test( a ) < 0 )	memset( (vers ? HANS[3].val : HANS[7].val), '\0', 6  );
	else					memmove( (vers ? HANS[3].val : HANS[7].val), a, 6 );	

	// 返還等対価の税額
	memmove( (vers ? HANS[4].val : HANS[8].val), Snd.Sn_HKANZ, 6 );	
	// 貸倒れに係る税額
	memmove( (vers ? HANS[5].val : HANS[9].val), Snd.Sn_KSITZ, 6 );	
	// 控除税額小計 vers ? (4 + 5 + 6) : (8 + 9 + 10)
	if( vers ) {
		l_add( HANS[6].val, HANS[3].val, HANS[4].val );
		l_add( HANS[6].val, HANS[6].val, HANS[5].val );
	}
	else {
		l_add( HANS[10].val, HANS[7].val,  HANS[8].val );
		l_add( HANS[10].val, HANS[10].val, HANS[9].val );
	}

	// 限界控除税額
	memmove( (vers ? WANS[8].val : HANS[13].val), Snd.Sn_GENKAI, 6 );	
	switch( vers&0x0f ) {
	// 控除不足還付税額 (7 - 2 - 3) ･ 差引税額 (2 + 3 - 7)
		case 1 :	if( l_test( Snd.Sn_1F14C ) < 0 ) {
						if( (Snh.Sn_SKKBN&0xff) % 2 ) {	// 中間申告以外
							memmove( HANS[7].val, Snd.Sn_1F14C, 6 );
							l_neg( HANS[7].val );
						}
					}
					else {
						if( l_test( Snd.Sn_GENKAI ) )
								memmove( a, Snd.Sn_3F3,   6 );
						else	memmove( a, Snd.Sn_1F14C, 6 );
						p_calq( (char *)HANS[8].val, (char *)a, 0x10 );	//1 00円未満切り捨て
					}
					break;
		case 2 :	l_add( a, HANS[1].val, HANS[2].val );
					l_sub( a, a, HANS[6].val );
					// 限界控除前の税額がマイナスの時
					if( l_test( a ) < 0 ) {
						if( (Snh.Sn_SKKBN&0xff) % 2 ) {	// 中間申告以外
							l_neg( a );
							memmove( HANS[7].val, a, 6 );
						}
					}
					else	p_calq( (char *)HANS[8].val, (char *)a, 0x10 );	// 100円未満切り捨て
					break;	
				// 限界控除前の税額 (6 + 7 - 11)
		default:	l_add( a, HANS[5].val, HANS[6].val );
					l_sub( a, a, HANS[10].val );
					if( l_test( a ) < 0 )	memset( HANS[11].val, '\0', 6 );
					else					memmove( HANS[11].val, a, 6 );					
				// 控除不足還付税額 (11 - 6 - 7)
					l_sub( a, HANS[10].val, HANS[5].val );
					l_sub( a, a, HANS[6].val );
					// 中間申告以外
					if( (l_test( a ) < 0 ) || ! (Snh.Sn_SKKBN % 2) )
							memset( HANS[12].val, '\0', 6 );
					else	memmove( HANS[12].val, a, 6 );					
				// 差引税額 (12 - 14)
					if( ! l_test( HANS[12].val ) )
							l_sub( HANS[14].val, HANS[11].val, HANS[13].val );
					else	memset( HANS[14].val, '\0', 6 );					
					// １００円未満　切り捨て
					if( l_test( HANS[14].val ) )
							p_calq( (char *)HANS[14].val, (char *)HANS[14].val, 0x10 );
					break;
	}

	// 中間納付税額
	memmove( a, Snd.Sn_TYNOFZ, 6 );
	p_calq( (char *)(vers ? HANS[9].val : HANS[15].val), (char *)a, 0x10 );	// 100円未満切り捨て	
	// 納付税額 (15 - 16) ･ 中間納付還付税額
	if( vers ) {
		l_sub( a, HANS[8].val, HANS[9].val );
		p_calq( (char *)a, (char *)a, 0x10 );			// 100円未満切り捨て
		if( l_test( a ) < 0 ) {
			l_neg( a );
			memmove( HANS[11].val, a, 6 );
		}
		else	memmove( HANS[10].val, a, 6 );
	}
	else {
		if( ! l_test( HANS[12].val ) ) {
			l_sub( a, HANS[14].val, HANS[15].val );
			if( l_test( a ) < 0 )	memset( HANS[16].val, '\0', 6 );
			else	p_calq( (char *)HANS[16].val, (char *)a, 0x10 );		// 100円未満　切り捨て
		}
		else	memset( HANS[16].val, '\0', 6 );
		
		if( l_test( HANS[12].val ) )	memmove( HANS[17].val, HANS[15].val, 6 );
		else {
			l_sub( a, HANS[15].val, HANS[14].val );
			if( l_test( a ) < 0 )	memset( HANS[17].val, '\0', 6 );
			else					memmove( HANS[17].val, a, 6 );
		}
	}	
	memmove( Snd.Sn_EDNOFZ, (vers ? HANS[10].val : HANS[16].val), 6 );

	// 修正申告
	if( Snh.Sn_SKKBN == 3 || Snh.Sn_SKKBN == 4 ) {
		// 既確定税額
		memmove( (vers ? HANS[12].val : HANS[18].val), Snd.Sn_KAKTIZ, 6 );		
		if( vers ) {
			if( l_test( HANS[10].val ) > 0 )		// 納付税額が有る場合
				l_sub( a, HANS[10].val, HANS[12].val );
			else {
				memmove( a, HANS[11].val, 6 );
				l_neg( a );
				l_sub( a, a, HANS[12].val );
			}
			l_sub( a, a, HANS[7].val );
		}
		else {
			// 差引納付税額 (17 - 19)
			if( l_test( HANS[12].val ) ) {
				l_add( a, HANS[12].val, HANS[17].val );
				l_add( a, a, HANS[18].val );
				l_neg( a );
			}
			else {
				if( l_test( HANS[16].val ) )
					l_sub( a, HANS[16].val, HANS[18].val );
				else {
					l_add( a, HANS[17].val, HANS[18].val );
					l_neg( a );
				}
			}
		}
		// １００円未満　切り捨て
		p_calq( (char *)(vers ? HANS[13].val : HANS[19].val), (char *)a, 0x10 );
	}	
	// 課税資産の譲渡等の対価の額
	memmove( (vers ? HANS[14].val : HANS[20].val), Snd.Sn_KZURI, 6 );
	if( Kani > 1 )	 // 基準期間の課税売上高
		memmove( (vers ? HANS[15].val : HANS[21].val), Snd.Sn_ZZURI, 6 );
	else			 // 資産の譲渡等の対価の額
		memmove( (vers ? HANS[15].val : HANS[21].val), Snd.Sn_SOURI, 6 );	
	if( ! vers )	return;	
	// 地方消費税の計算
	// 控除不足還付税額
	if( ! ((Snh.Sn_SKKBN&0xff) % 2) && N_ver != 2 ) ;	// 中間且改正日前開始
	else {
		if( (Snh.Sn_SKKBN&0xff) % 2 ) {	// 中間申告以外
			if( vers == 1 )	memmove( HANS[16].val, Snd.Sn_1F12B, 6 );
			else			memmove( HANS[16].val, HANS[7].val, 	6 );
		}
		
		// 差引税額
		if( l_test( Snd.Sn_1F14C ) < 0 && ! ((Snh.Sn_SKKBN&0xff) % 2) ) ;
		else {		// 還付　中間申告以外
			if( vers == 1 ) {
				if( l_test( Snd.Sn_GENKAI ) )
						memmove( a, Snd.Sn_3F6,   6 );
				else	memmove( a, Snd.Sn_1F13B, 6 );
			}
			else		memmove( a, HANS[8].val, 6 );
			p_calq( (char *)HANS[17].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}		
		// 譲渡割額還付額 ･ 譲渡割額納税額
		l_input( b, "250" );
		if( l_test( HANS[16].val ) )
			pArithEx->l_mul100( (unsigned char *)HANS[18].val, (unsigned char *)HANS[16].val, (unsigned char *)b, 1, 0 );
		if( l_test( HANS[17].val ) ) {
			pArithEx->l_mul100( (unsigned char *)a, (unsigned char *)HANS[17].val, (unsigned char *)b, 1, 0 );
			p_calq( (char *)HANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}		
		// 中間納付譲渡割額
		if( (Snh.Sn_SKKBN&0xff) % 2 )	// 中間申告以外
			p_calq( (char *)HANS[20].val, (char *)Snd.Sn_TTYWAR, 0x10 );	// 100円未満切り捨て
		// 納付譲渡割額 (20 - 21) ･ 中間還付譲渡割額 (21 - 20)
		l_sub( a, HANS[19].val, HANS[20].val );
		if ( l_test( a ) < 0 ) {
			l_neg( a );
			p_calq( (char *)HANS[22].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
		else	p_calq( (char *)HANS[21].val, (char *)a, 0x10 );	// １００円未満　切り捨て		
	// 修正申告
		if( Snh.Sn_SKKBN == 3 || Snh.Sn_SKKBN == 4 ) {
			// 既確定税額
			memmove( HANS[23].val, Snd.Sn_TKAKTZ, 6 );
			
			// 差引納付譲渡割額
			if( l_test( HANS[21].val ) > 0 )	// 納付税額が有る場合
				l_sub( a, HANS[21].val, HANS[23].val );
			else {
				l_add( a, HANS[18].val, HANS[22].val );
				if( l_test( a ) )	l_neg( a );
				l_sub( a, a, HANS[23].val );
			}
			p_calq( (char *)HANS[24].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
	}	
	// 消費税及び地方消費税の合計（納付又は還付）税額
	switch( Snh.Sn_SKKBN&0xff ) {
		case 1 : case 2 :		// 確定・中間申告
			l_add( a, HANS[10].val, HANS[21].val );
			l_add( b, HANS[7].val,  HANS[11].val );
			l_add( b, b, HANS[18].val );
			l_add( b, b, HANS[22].val );
			l_sub( HANS[25].val, a, b );
			break;
		default	:				// 修正申告
			l_add( HANS[25].val, HANS[13].val, HANS[24].val );
			break;
	}

	// 消費税額の計算

	// 課税標準額					①
	memmove( sumdata->HSVal[0].val, HANS[0].val, 6 );
	// 消費税額						②
	memmove( sumdata->HSVal[1].val, HANS[1].val, 6 );
	// 控除過大調整税額				③
	memmove( sumdata->HSVal[2].val, HANS[2].val, 6 );

	// 控除対象仕入税額				④
	memmove( sumdata->HSVal[3].val, HANS[3].val, 6 );
	// 返還等対価に係る税額			⑤
	memmove( sumdata->HSVal[4].val, HANS[4].val, 6 );
	// 貸倒れに係る税額				⑥
	memmove( sumdata->HSVal[5].val, HANS[5].val, 6 );
	// 控除税額承継					⑦
	memmove( sumdata->HSVal[6].val, HANS[6].val, 6 );
	// 控除不足還付税額				⑧
	memmove( sumdata->HSVal[7].val, HANS[7].val, 6 );
	// 差引税額						⑨
	memmove( sumdata->HSVal[8].val, HANS[8].val, 6 );
	// ⑩中間納付税額
	memmove( sumdata->HSVal[9].val, HANS[9].val, 6 );
	// ⑪納付税額
	memmove( sumdata->HSVal[10].val, HANS[10].val, 6 );
	// ⑫中間納付還付税額
	memmove( sumdata->HSVal[11].val, HANS[11].val, 6 );

	// 地方消費税額の計算

	// 控除不足還付税額				⑰
	memmove( sumdata->HTVal[0].val, HANS[16].val, 6 );
	// 差引税額						⑱
	memmove( sumdata->HTVal[1].val, HANS[17].val, 6 );
	// 還付額						⑲
	memmove( sumdata->HTVal[2].val, HANS[18].val, 6 );
	// 納付額						⑳
	memmove( sumdata->HTVal[3].val, HANS[19].val, 6 );
	// 21中間納付譲渡割額
	memmove( sumdata->HTVal[4].val, HANS[20].val, 6 );
	// 22納付譲渡割額
	memmove( sumdata->HTVal[5].val, HANS[21].val, 6 );
	// 23中間納付還付譲渡割額
	memmove( sumdata->HTVal[6].val, HANS[22].val, 6 );

	// 消費税及び地方消費税の合計	26
	memmove( sumdata->HTVal[7].val, HANS[25].val, 6 );
}

// ９５％ルール改正対応期間チェック
// Shinkoku_Read()の直後に呼び出す
// 返送値　	0:対応期間外  1:対応期間内
int CShinCalculateW::TaxReformsCheck( void )
{
	int		Kind = 0;
	switch( m_Kind )	{
		case 1 : Kind = 0;	break;					// 法人
		case 2 : Kind = 1;	break;					// 個人
		default: if(Snh.Sn_KOJIN&0x01)	Kind = 1;	// 個人
				 else					Kind = 0;	// 法人
				 break;
	}
	if( (Vol1.APNO&0xf0) == 0x10 || Kind )	{
		// 個人
		if( m_Type == 1 )	{	// 更新
			if( pZaimuSub->zvol->ss_ymd < 20120000 )	return( 0 );
		}
		else	{				// 通常
			if( pZaimuSub->zvol->ss_ymd < 20130000 )	return( 0 );
		}
	}
	else	{
		// 法人
		if( m_Type == 1 )	{	// 更新
			if( pZaimuSub->zvol->ee_ymd < 20120331 )	return( 0 );
		}
		else	{				// 通常
			if( pZaimuSub->zvol->ss_ymd < 20120401 )	return( 0 );
		}
	}
	return( 1 );
}

// 申告書集計
int CShinCalculateW::Shinkoku_CalcurateEX( SUMDATA* sumdata )
{
	int		StTs;
	char	ChkSw, ChkDm;
	char	Xymd[4] = { 0x09, 0x04, 0x01, 0x00 };

	toku = 0;
	SimpSw = K221sw = H301sw = 0;
	Snh.Sn_UPERS &= 0xef;
	Snd.Sn_2FUWSEX = 0;

	// 消費税集計
	ofs_get();

	if( toku )	Kenei |= 0x10;							//特例計算摘要ＳＷ		
	// 集計テーブル  イニシャライズ
	if( forcreat( &foreA, &foreB, &foreC ) )	{
		ErrMessage = _T("売上割合集計テーブルが確保できません");
		return( ERR );
	}
	// 消費税科目集計ファイルリード
	if( rui_read( &foreA, &foreB, &foreC ) )		{
		ErrMessage = _T("消費税科目集計ファイルが読み込めません");
		return( ERR );
	}
	// 一括税抜時の仮受・仮払消費税額の取得
	if( IkTaxZanRead() ) {
		ErrMessage = _T("仮受・仮払消費税の残高が取得できません！");
		return( ERR );
	}
	// 設立開始オフセット
	SetUpOfs = 0;
	if( shin_ofst( &Vol1.SS_YMD[1], &SetUpOfs, &Vol1, &Vol2 ) )	SetUpOfs = 0;
	// 課税仕入区分・売上区分集計エリアのリード
	if( suk_read( &SVsum, &foreB ) )	{
		ErrMessage = _T("課税仕入区分・売上区分集計領域が読み込めません");
		return( ERR );
	}
	// 該当月選択＆集計
	int	save_tno = 0;
	int	ans_tno = 0;
	// 課税期間で申告書集計を行う
	if( Kk_SG&0x10 ) {
		save_tno = s_tno;
		if( shin_ofst( &Snh.Sn_KDAYS[1], &ans_tno, &Vol1, &Vol2 ) )	{
			Kk_SG &= 0xef;
			s_tno = (char)save_tno;
		}
		else	s_tno = (char)ans_tno;
	}
	// 簡易計算
	Snh.Sn_SPECIAL &= 0xfe;
	if( SimpSw )	Snh.Sn_SPECIAL |= 0x01;
	// 貨物の税額を入力税額より計算
	Snh.Sn_SPECIAL &= 0xfd;
	ChkSw = ChkDm = 0x00;
	ChkSw = TBhead.SVmzsw;
	// 科目残高集計
	if( formmake( s_tno, d_tno, &kr, &karibri_5, &kariuke_5 ) )	{
		ErrMessage = _T("科目残高集計中に障害が発生しました");
		return( ERR );
	}
	// 経過措置　仮払・仮受消費税集計
	formmake3( s_tno, d_tno, &kr3, &karibri_3, &kariuke_3 );
	l_add( kr.barai, kr.barai, kr3.barai );
	l_add( kr.uke, kr.uke, kr3.uke );

#ifdef	_CLOSE_20120801
	// 棚卸計算
	if( Anbun && m_Stock == 1 )	{ 
		// 比例配分及び簡易課税マスター時の棚卸調整の金額を取り込み
		// 税込・税抜サイン設定
		TBhead.SVsign &= 0xef;
		if( SumInvent.InSw1&0x10 )	TBhead.SVsign = 0x10;
		// 棚卸選択
		Snd.Sn_MENZsw =	0x00;
		switch( SumInvent.InSw1&0x0f ) {
			case 0x01 :	Snd.Sn_MENZsw =	0x10;	break;
			case 0x02 :	Snd.Sn_MENZsw =0x11;	break;	
			default:	break;
		}
		TBhead.SVzchg = Snd.Sn_MENZsw;
		memmove( Snd.Sn_mnk40[0], SumInvent.INVal[0].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
		memmove( Snd.Sn_mnz40[0], SumInvent.INVal[1].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_mnk40[1], SumInvent.INVal[2].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
		memmove( Snd.Sn_mnz40[1], SumInvent.INVal[3].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_mnk40[2], SumInvent.INVal[4].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
		memmove( Snd.Sn_mnz40[2], SumInvent.INVal[5].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_menzk[0], SumInvent.INVal[6].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　課税に係る
		memmove( Snd.Sn_menzz[0], SumInvent.INVal[7].val, 6 );	// 				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_menzk[1], SumInvent.INVal[8].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　非課税に係る
		memmove( Snd.Sn_menzz[1], SumInvent.INVal[9].val, 6 );	//				 〃	   消費税額   (〃      )　〃
		memmove( Snd.Sn_menzk[2], SumInvent.INVal[10].val, 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　共通に係る
		memmove( Snd.Sn_menzz[2], SumInvent.INVal[11].val, 6 );	//				 〃	   消費税額   (〃      )　〃
	}
	else	{
		StackMake( s_tno, d_tno, &kr, stack_data );
		if( Anbun && (pZaimuSub->zvol->s_sgn4 & 0x80) )	{
			// マスターが個別対応時は棚卸調整を返す
			SumInvent.InSw1 = 0x00;
			if( TBhead.SVsign == 0x10 )	SumInvent.InSw1 = 0x10;
			switch( Snd.Sn_MENZsw ) {
				case 0x10 :	SumInvent.InSw1 |= 0x01;	break;
				case 0x11 :	SumInvent.InSw1 |= 0x02;	break;	
				default   :
					break;
			}
			memmove(  SumInvent.INVal[0].val,Snd.Sn_mnk40[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　課税に係る
			memmove(  SumInvent.INVal[1].val,Snd.Sn_mnz40[0], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[2].val,Snd.Sn_mnk40[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　非課税に係る
			memmove(  SumInvent.INVal[3].val,Snd.Sn_mnz40[1], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[4].val,Snd.Sn_mnk40[2], 6 );	// 調整課税仕入等に係る棚卸資産額 (新税率分)　共通に係る
			memmove(  SumInvent.INVal[5].val,Snd.Sn_mnz40[2], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[6].val,Snd.Sn_menzk[0], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　課税に係る
			memmove(  SumInvent.INVal[7].val,Snd.Sn_menzz[0], 6 );	// 				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[8].val,Snd.Sn_menzk[1], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　非課税に係る
			memmove(  SumInvent.INVal[9].val,Snd.Sn_menzz[1], 6 );	//				 〃	   消費税額   (〃      )　〃
			memmove(  SumInvent.INVal[10].val,Snd.Sn_menzk[2], 6 );	// 調整課税仕入等に係る棚卸資産額 (旧税率分)　共通に係る
			memmove(  SumInvent.INVal[11].val,Snd.Sn_menzz[2], 6 );	//				 〃	   消費税額   (〃      )　〃
		}
	}
	// その他の金額
	if( Kani < 2 ) {
		// 調整対象固定資産に係る調整税額
		memmove( Snd.Sn_2F19B, SumMony[m_Calcusw].MOVal[0].val, 6 );
		// 調整対象固定資産を課税業務用に転用した調整額
		memmove( Snd.Sn_2F20B, SumMony[m_Calcusw].MOVal[1].val, 6 );
	}
	// 中間納付税額
	memmove( Snd.Sn_TYNOFZ, SumMony[m_Calcusw].MOVal[2].val, 6 );
	// 中間納付譲渡割額
	memmove( Snd.Sn_TTYWAR, SumMony[m_Calcusw].MOVal[3].val, 6 );
#endif

	// 集計レコードクリア
	if( for_clear( &foreA ) )	{
		ErrMessage = _T("集計レコードクリア中に障害が発生しました");
		return( ERR );
	}
	StTs = stm_ucalq( s_tno, d_tno, kr.barai, kr.uke, kr3.barai, kr3.uke, &foreA, &foreB );
	if( StTs == 3 ) {
		char	Kzur[6] = {0};
		char	Mony[6] = {0};
		l_input( Mony, "10000000" );
		if( (pZaimuSub->zvol->s_sgn2&0x0f) == 0x01 )	memmove( Kzur, m_KzuValKm, 6 );
		else											memmove( Kzur, m_KzuVal, 6 );
		if( l_cmp( Kzur, Mony ) > 0 )	{
			if( (pZaimuSub->zvol->s_sgn2&0x0f) == 0x01 )
					CheckMessage =  _T("上半期の課税売上(税込)が1000万円を超えています。\n");
			else	CheckMessage =  _T("上半期の課税売上(税抜)が1000万円を超えています。\n");
			CheckMessage += _T("来期の消費税の申告方法や届出等にご注意ください。\n");
			CheckMessage += _T("＊給与等支払額合計の判定は行っていません。");
			return( 1 );
		}
		else	{
			if( (pZaimuSub->zvol->s_sgn2&0x0f) == 0x00 )	{	// 課税
				char	ValUp[6] = {0};
				char	ValLw[6] = {0};
				l_input( ValUp, "10000000" );
				l_input( ValLw, "9523810" );
				if( l_cmp( m_KzuVal, ValLw ) >= 0 && l_cmp( m_KzuVal, ValUp ) <= 0 )	{
					if( m_Type == 1 ) {
						CheckMessage  = _T("上半期の課税売上高(税込）が1000万を超えています。\n");
						CheckMessage += _T("課税方式を課税の状態で「免税」処理をしている場合、\n");
						CheckMessage += _T("来期の消費税の申告方法や届出等にご注意ください。\n");
						CheckMessage += _T("＊給与等支払額合計の判定は行っていません。");
					}
					else	{
						CheckMessage  = _T("上半期課税売上高(税込）が1000万超です。課税の状態で\n");
						CheckMessage += _T("免税の処理をしている場合、来期申告や届出等にご注意ください。\n");
						CheckMessage += _T("＊給与等支払額合計の判定は行っていません。");
					}
					return( 1 );
				}
			}
		}
	}
	return( 0 );
}
