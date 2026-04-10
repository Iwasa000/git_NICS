
#include "stdafx.h"
#include "DBZmSub8.h"
#include "DBNpSub8.h"

#include "ICSARITH8.h"
#include "ICSTblsub8.h"
#include "VolDatedb8.h"
#include "Volume8.h"

#include "DBSyzDef.h"
#include "DBSyzShinKoku.h"

#include "DBShinDef.h"
#include "DBShinDef1.h"
#include "DBShinDef2.h"

#include "ShinSumilatePac8.h"
#include "ShinCalculateW.h"

#include "ZmGen8.h"
#include "SyzSyukei8.h"
#include "DBSyzAccess8.h"
#include "ShinCalculateDef.h"
#include "DBAddUp8.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// 平成26年4月改正対応
//---------------------------------------------------------------------------
// 引数		なし
//---------------------------------------------------------------------------
// 返送値	：	-2: 平成24年3月31日以前の対象外
//				-1: エラー
//				 0: 本関数の対象外マスター＝平成26年4月改正未対応マスター
//				 1: ５億円超え
//				 2: ４億五千万以上５億円未満
//				 3: ４億五千万未満
//---------------------------------------------------------------------------
int CShinCalculateW::CheckShinkokuH26()
{
	// マスタのチェック
	int		st = 0;
	CZmGen8	ZmGen;
	st = ZmGen.CheckValiableVersion( pZaimuSub->m_database, ID_H26SYZ_VUP_CHK );

	if( st != 1 )	return 0;	// 処理可能なマスター以外はすべて0で返し、以前までの処理を行う

	char	sysserver[_MAX_PATH] = {0};

	if( pZaimuSub->m_sysctl == NULL ) {
		// システムデータベース
		if( GCOM_GetString(_T("SystemDataBase"), sysserver) == ERR ) {
			ErrMessage = SYSCTL_NAME_ERROR;
			DbgViewTrace( ErrMessage );
			return ERR;
		}
		if( pZaimuSub->SysCtlDB(sysserver) == ERR ) {
			ErrMessage = pZaimuSub->GetZmErrMsg();
			DbgViewTrace( ErrMessage );
			return ERR;
		}
	}

	// ボリュームラベル
	if( pZaimuSub->zvol == NULL ) {
		if( pZaimuSub->VolumeOpen() == ERR ) {
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return ERR;
		}
		m_OpenFlg1 |= 0x01;
	}

	//===========================================================================
	// →（2016.01.06）
	if( m_Type == 1 ) {	// 翌期更新・前年度からの更新
		st = 5;
		if( m_eymd >= YmdTable[st] ) {	// 更新前の期末年月日が、平成26年10月1日を越えるもの
			CheckMessage = KEIKA_271001_KOUSHIN;	// 消費税法附則36条に関する注意喚起メッセージ

			// 消費税インボイス１次対応
			CheckMessage.Empty();	// 返送メッセージを空白に

			return 4;	// 返送値は、下記みなし仕入れ率の場合と同じ「4」で
		}
	}
	// ←（2016.01.06）
	//===========================================================================
	// 平成27年4月改正対応（2015.02.06）
	if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 0x02 ) {
		// 簡易課税
		if( m_Type == 1 ) {	// 翌期更新・前年度からの更新
			st = 4;
			if( m_eymd >= YmdTable[st] ) {	// 更新前の期末年月日が、平成26年4月1日以降
				CheckMessage = KANI_270401_KOUSHIN;	// みなし仕入率の変更に関する注意喚起メッセージ
				return st;	// 返送値は「4」
			}
		}
	}
	//===========================================================================

	// 当期月別情報
	if( pZaimuSub->minfo == NULL ) {
		if( pZaimuSub->MinfoOpen() == ERR ) {
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return ERR;
		}
		m_OpenFlg1 |= 0x08;
	}
	else	pZaimuSub->minfo->Requery( "" );

	// 消費税
	if( pZaimuSub->szvol == NULL ) {
		if( pZaimuSub->SyohizeiOpen() == ERR ) {
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return ERR;
		}
		m_OpenFlg2 |= 0x01;
	}
/*
// アプリ側ですでに集計済み（既存の関数で「#ifdef	_CLOSE」となっている）
// 一括税抜？の場合、仕訳の集計済みサインによらず毎回集計が掛かるのでカットしておく(2014.03.06)
	// 消費税・資金繰集計
	if( pZaimuSub->zvol->apno < 0x50 ) {
		st = DBSySkZanCalq( pZaimuSub );
	}
	else {
		st = NPSySkZanCalq( pZaimuSub );
	}
	if( st == ERR ) {
		ErrMessage = GetSsErrMsg();
		return ERR;
	}
	pZaimuSub->minfo->Requery( "" );
*/
	// 消費税申告書
	if( pZaimuSub->szsnk == NULL ) {
		if( pZaimuSub->SzsnkOpen() == ERR ) {
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return ERR;
		}
		m_OpenFlg2 |= 0x02;
	}

	// 消費税集計
	if( pZaimuSub->szsyk == NULL ) {
		if( pZaimuSub->SzsykOpen("") == ERR ) {
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return ERR;
		}
		m_OpenFlg2 |= 0x04;
	}
	else	pZaimuSub->szsyk->Requery( "" );

	// 科目名称
	if( pZaimuSub->knrec == NULL ) {
		if( pZaimuSub->KamokuMeisyoOpen() == ERR ) {
			ErrMessage = pZaimuSub->GetZmErrMsg();
			return ERR;
		}
		m_OpenFlg1 |= 0x02;
	}

	// 
	int		kojin = 0, ofset;
	switch( m_Kind ) {
	case  1	:	kojin = 0;	break;	// 法人
	case  2	:	kojin = 1;	break;	// 個人
	default	:	if( pZaimuSub->szsnk->Sn_KOJIN&0x01 )	kojin = 1;	// 個人
				else									kojin = 0;	// 法人
				break;
	}
	if( pZaimuSub->zvol->apno == _KOJIN || kojin ) {
		if( m_Type == 1 )	ofset = 0;	// 更新
		else				ofset = 1;	// 通常
	}
	else {
		if( m_Type == 1 )	ofset = 2;	// 更新
		else				ofset = 3;	// 通常
	}

	// 95％ルール改正対応期間チェック
	if( ofset == 2 ) {	// 法人・更新のみ期末日
		if( pZaimuSub->zvol->ee_ymd < YmdTable[ofset] )	return -2;	// 対象外期間 → 終了
	}
	else {				// 他は期首日
		if( pZaimuSub->zvol->ss_ymd < YmdTable[ofset] )	return -2;	// 対象外期間 → 終了
	}

	/////////////////////////////////////////////////////////////////////////////
	// 月オフセットと経過月数
	int		smofs, emofs, months;

	smofs = emofs = months = -1;

	do {
		// TISW修正[24-0327] 2026/02/16 START
		if (pZaimuSub->minfo->smd > m_eymd)
		{
			// 集計期間の範囲外に到達
			break;
		}
		// TISW修正[24-0327] 2026/02/16 END

		// 指定開始月オフセット
		if( smofs == -1 ) {
			if( (pZaimuSub->minfo->smd <= m_symd) && (m_symd <= pZaimuSub->minfo->emd) ) {
				smofs = pZaimuSub->minfo->ofset;
				months = 0;
			}
		}
		// 経過月数（指定開始月～指定最終月）
		if( months >= 0 ) {	// 指定開始月オフセット以降のみカウント
			if( !pZaimuSub->minfo->kes_sgn ) {	// 決修月はカウントしない
				months++;
			}
		}
		// 指定最終月オフセット
		//if( emofs == -1 ) { // TISW修正[24-0327] 2026/02/16
		if( (pZaimuSub->minfo->smd <= m_eymd) && (m_eymd <= pZaimuSub->minfo->emd) ) {
			emofs = pZaimuSub->minfo->ofset;
			//break; // TISW修正[24-0327] 2026/02/16
		}
		//} // TISW修正[24-0327] 2026/02/16

	} while( pZaimuSub->minfo->MoveNext() != ERR );

	if( (smofs == -1) || (emofs == -1) || (months == -1) ) {
		DbgViewTrace( "開始又は最終年月日が正しくないため、95％ルール改正に伴う課税売上額の判定は行われません。" );
		return -2;	// 期間外と同じ扱い＝何もしない
	}

	// 課税開始日判定
	BOOL to_kazei = FALSE;
	if ((m_symd <= pZaimuSub->zvol->to_kz_ymd) && (pZaimuSub->zvol->to_kz_ymd <= m_eymd)) {
		to_kazei = TRUE;	// 指定期間に免税期間を含む
	}

	/////////////////////////////////////////////////////////////////////////////
	// 課税売上額と免税売上額の取得

	CDBSyzAccess SyzAccess;
	CSyzSyukei*	SyzSyukei;	// 消費税計算モジュール
	CSyzScopeOption	sop;
	CSyzCalqOption	cop;
	MoneyBasejagArray	moneybase;
	char	chk_mny_max[6] = {0}, chk_mny_min[6] = {0}, check_money[6] = {0}, bunsi[6] = {0}, bunbo[6] = {0}, mbuf[128] = {0};

	SyzSyukei = new CSyzSyukei;
	SyzSyukei->initialize( pZaimuSub );

	// 対象範囲と集計オプションの設定
	sop.sofs = smofs;
	sop.eofs = emofs;
	SyzSyukei->stmcalq( &sop, &cop );

	l_defn( 0x16 );
	// 課税売上高の５億超えチェック
	l_input( chk_mny_max, MoneyTable[0] );	// ５億
	l_input( chk_mny_min, MoneyTable[1] );	// ４億５千万

	// 対象金額の取得
	if (to_kazei) {	// 免税期間を含む場合
		SyzAccess.IsOver5okuYen( pZaimuSub, m_symd, m_eymd, check_money );
	}
	moneybase = SyzSyukei->GetShinkokuData( CHECK_RULE_95_CODE );
	// 対象金額(課税売上高)＝課税売上額＋免税売上額
	l_add( check_money, check_money, moneybase[0][0].arith );

	if( months != 12 ) {	// 指定期間が一年未満
		sprintf_s( mbuf, sizeof(mbuf), _T("%d"), months );
		l_input( bunsi, "12" );
		l_input( bunbo, mbuf );
		l_div( check_money, check_money, bunbo );	// / 分母（経過月数）
		l_mul( check_money, check_money, bunsi );	// * 分子（12）
	}

	if( l_cmp(check_money, chk_mny_max) > 0 )		st = 1;	// ５億超
	else if( l_cmp(check_money, chk_mny_min) >= 0 )	st = 2;	// ４億５千万以上（５億以下）
	else											st = 3;	// ４億５千万未満

	if( (st == 1) || (st == 2 && m_Type) ) {
		// 個別対応方式選択時は対象外
		if( pZaimuSub->zvol->s_sgn4&0x80 )	st = -2;
		// 簡易課税は対象外
		else if( (pZaimuSub->zvol->s_sgn2&0x0f) >= 0x02 )	st = -2;
		// 
		else {
			switch( m_Type ) {
			case  1	:	// 翌期更新・前年度からの更新
						CheckMessage = OVER_5OKU_KOUSHIN;	// ５億円超
						break;
			default	:	// 試算表・課税売上割合・消費税申告書・消費税額試算表
						if( st == 1 ) {
							if( months == 12 )	CheckMessage = OVER_5OKU_NENKAN;	// ５億円超
							else				CheckMessage = OVER_5OKU_KANSAN;	// 年換算５億円超
							// 一行表示以外
							if( m_Type != 2 ) {
								CheckMessage += HIREI_NO_SENTAKU;
							}
						}
						break;
			}
		}
	}
	else {
		::ZeroMemory( check_money, sizeof( check_money ) );
		if( (st == 3 || st == 2) && (pZaimuSub->zvol->ss_ymd > YmdTable[0]) ) {
			// 特定期間の課税売上高の取得
			if (to_kazei) {	// 免税期間を含む場合
				pZaimuSub->minfo->Requery( "" );
				long smd = pZaimuSub->minfo->smd;
				pZaimuSub->minfo->SetAbsolutePosition( 8 );	// ofset 7
				long emd = pZaimuSub->minfo->emd;
				SyzAccess.IsOver5okuYen( pZaimuSub, smd, emd, check_money );
			}

			sop.sofs = 0;	// 期首のオフセットは常に "0"
			sop.eofs = 7;	// 期首から６ヶ月後のオフセットは常に "7"
			SyzSyukei->stmcalq( &sop, &cop );

			if ((pZaimuSub->zvol->s_sgn2 & 0x0f) == 0x01) {	// 免税マスター
				// 対象金額(税込課税売上高)の取得
				moneybase = SyzSyukei->GetShinkokuData( CHECK_1000MAN_CODE );
			}
			else {
				// 対象金額(税抜課税売上高)の取得
				moneybase = SyzSyukei->GetShinkokuData( CHECK_KAZ_NUK_CODE );
			}
			// 対象金額
			l_add( check_money, check_money, moneybase[0][0].arith );
			l_input( chk_mny_max, MoneyTable[2] );	// 1000万円
			if( l_cmp(check_money, chk_mny_max) > 0 ) {
				// 免税マスター
				if( (pZaimuSub->zvol->s_sgn2&0x0f) == 0x01 ) {
					CheckMessage = MENZEI_10MIL_KOMI;
				}
				else {
					CheckMessage = MENZEI_10MIL_NUKI;
				}
				CheckMessage += MENZEITEN_WARNING;
				CheckMessage += MENZEITEN_KYUYOTO;
				st = 1;
			}
			else if (to_kazei) {
				;	// 免税期間を含む場合は、課税マスターの税込課税売上高が取れないので、以下の警告は行わないようにしておく
			}
			else if( (pZaimuSub->zvol->s_sgn2&0x0f) == 0x00 ) {	// 課税
				// 税込課税売上高の取得
				moneybase = SyzSyukei->GetShinkokuData( CHECK_1000MAN_CODE );
				memmove( check_money, moneybase[0][0].arith, 6 );
				// 税込で1000万を超えていないかを判定
				if( l_cmp(check_money, chk_mny_max) > 0 ) {
					if( m_Type == 1 ) {	// 翌期更新・前年度からの更新
						CheckMessage = MENZEI_10MIL_KOMI;
						CheckMessage += KAZEI_MAST_MENZEI;
						CheckMessage += MENZEITEN_WARNING;
					}
					else {
						CheckMessage = MENZEI_10MIL_KOMI;
						CheckMessage += MENZEI_10MIL_OVER;
					}
					CheckMessage += MENZEITEN_KYUYOTO;
					st = 1;
				}
			}
		}
	}
	delete SyzSyukei;

	return st;
}

/////////////////////////////////////////////////////////////////////////////
