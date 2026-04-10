#include "stdafx.h"
#include "resource.h"
#include "TaxationList31View.h"
#include "StzRendoDef.h"
#include <stzdbincdef8.h>
#include <STZ15_DBIF.h>


//========================================================================
// 所得税連動
//------------------------------------------------------------------------
// 返送値
//			0			:			正常終了
//			-1			:			エラー
//========================================================================
int CTaxationList31View::StzRendo( int idx )
{
	if( idx == 0 ) {
		return 0;
	}

	bool bChange = false;
	CString icsWin;
	char curDir[MAX_PATH] = { 0 };

	CString	filePath;
	char dev[8] = { 0 };
	char gr[8] = { 0 }, jobs[8] = { 0 };

	memset( gr, '\0', sizeof( gr ));
	memset( jobs, '\0', sizeof( jobs ));

	// 実行パスを取得
	PCOM_GetString( _T( "ProgramDevice" ), dev );
	PCOM_GetString ( _T( "ExecGrJobs" ), gr, jobs );

	filePath.Format( _T( "%s:\\ICSWin\\Cmds\\GR%s\\JOBS%s\\Koketsu.txt"), dev, gr, jobs );
	if( !PathFileExists( filePath )) {
		// Koketsu.txtが存在していない場合は、下記処理を通さない
		// 所得税モジュールがリリースされていない可能性があるため
		return 0;
	}

	// カレントディレクトリ取得
	memset( curDir, '\0', sizeof( curDir ));
	::GetCurrentDirectory( sizeof( curDir ), curDir );

#ifdef _DEBUG
	icsWin = curDir;
#else
	icsWin.Format( _T( "%s:\\ICSWin\\Objs" ), dev );
#endif
	if( icsWin.CompareNoCase( curDir ) != 0 ) {
		bChange = true;
		// STZ15_DBIF.dllは遅延ロードしているためICSWin\Objsにカレントディレクトリ変更
		::SetCurrentDirectory( icsWin );
	}

	STZ15DBIF *pStzIf = nullptr;
	STZDB_MKCOINFO stzCoInfo = { 0 };
	char sqlSvr[MAX_PATH] = { 0 };
	CStringArray retAry, xpathAry;
	CString syzData;

	// インスタンス名
	PCOM_GetString( _T( "SelectSQLServer" ), sqlSvr );
	sprintf_s( stzCoInfo.dbSvrName, sizeof( stzCoInfo.dbSvrName ), _T( "%s" ), sqlSvr );

	// アプリケーション番号
	stzCoInfo.apno = m_pZmsub->zvol->apno;
	stzCoInfo.cocode = m_pZmsub->zvol->v_cod;
	stzCoInfo.ee_ymd = m_pZmsub->zvol->ee_ymd;

	// 所得税マスターオープン
	pStzIf = new STZ15DBIF;
	if( pStzIf->StzMastOnlyOpen( &stzCoInfo ) != 0 ) {
		ICSMessageBox( pStzIf->m_errMsg );
		delete pStzIf;
		pStzIf = nullptr;

		if( bChange ) {
			// カレントディレクトリを元に戻しておく
			::SetCurrentDirectory( curDir );
		}

		return -1;
	}

	int st = 0;
	CString errMsg;
	for( int i = RENDO_IPPAN; i <= RENDO_NOUGYOU; i++ ) {
		retAry.RemoveAll();
		xpathAry.RemoveAll();

		if( i == RENDO_IPPAN ) {
			// 一般
			if( !( idx & GYOSYU_IPPAN )) {
				continue;
			}

			for( LINKINFO linkInfo : IPPANINFO ) {
				xpathAry.Add( linkInfo.LinkXpath );
			}
		}
		else if( i == RENDO_FUDOUSAN ) {
			// 不動産
			if( !( idx & GYOSYU_FUDOUSAN )) {
				continue;
			}

			for( LINKINFO linkInfo : FUDOUSANINFO ) {
				xpathAry.Add( linkInfo.LinkXpath );
			}
		}
		else {
			// 農業
			if( !( idx & GYOSYU_NOUGYOU )) {
				continue;
			}

			for( LINKINFO linkInfo : NOUGYOUINFO ) {
				xpathAry.Add( linkInfo.LinkXpath );
			}
		}

		// 所得税マスターより取得
		st = pStzIf->StzGetSyohizeiFromKoketuData( &retAry, &xpathAry );
		if( st == 0 ) {
			if( i == RENDO_IPPAN ) {
				// 一般
				if( m_pShzCalc ){
					m_pShzCalc->Shz_DelKkval( m_pTaxListData->NRec.KJval, m_pTaxListData->Head.KHkjmx );
				}
				st = StzRendo_Ippan( &retAry );
			}
			else if( i == RENDO_FUDOUSAN ) {
				// 不動産
				if( m_pShzCalc ){
					m_pShzCalc->Shz_DelKkval( m_pTaxListData->NRec.KFval, m_pTaxListData->Head.KHkfmx );
				}
				st = StzRendo_Fudousan( &retAry );
			}
			else {
				// 農業
				if( m_pShzCalc ){
					m_pShzCalc->Shz_DelKkval( m_pTaxListData->NRec.KNval, m_pTaxListData->Head.KHknmx );
				}
				st = StzRendo_Nougyou( &retAry );
			}
		}
		else {
			errMsg = pStzIf->m_errMsg;
		}

		if( st != 0 ) {
			ICSMessageBox( errMsg );
			pStzIf->StzMastOnlyClose();
			delete pStzIf;
			pStzIf = nullptr;

			if( bChange ) {
				// カレントディレクトリを元に戻しておく
				::SetCurrentDirectory( curDir );
			}

			return -1;
		}
	}

	// 個人決算書の名称取得を解除する
	m_pTaxListData->Head.KHkoke = 0;

	m_pTaxListData->SetData2( m_pZmsub, m_pSnHeadData, m_SnSeq );

	if( bChange ) {
		// カレントディレクトリを元に戻しておく
		::SetCurrentDirectory( curDir );
	}

	return 0;
}

//========================================================================
// 所得税　一般業種連動
//------------------------------------------------------------------------
// 引数
//			pRetAry		:			金額
//------------------------------------------------------------------------
// 返送値
//			0			:			正常終了
//			-1			:			エラー
//========================================================================
int CTaxationList31View::StzRendo_Ippan( CStringArray *pRetAry )
{
	ASSERT( pRetAry );
	if( pRetAry == nullptr ) {
		return -1;
	}

	KKVAL *pKkval = &m_pTaxListData->NRec.KJval[0];
	LINKINFO linkInfo = { 0 };

	CString strVal;
	int count = ( int )pRetAry->GetCount();
	for( int i = 0; i < count; i++ ) {
		memset( &linkInfo, '\0', sizeof( LINKINFO ));
		strVal = pRetAry->GetAt( i );

		linkInfo = IPPANINFO[i];
		if( linkInfo.LinkFlg ) {
			// 項目名
			if( pKkval + linkInfo.LinkId ) {
				m_Util.cstring_to_char(( unsigned char* )( pKkval + linkInfo.LinkId )->KKnam, strVal, 40 );
			}
		}
		else {
			// 金額
			if( pKkval + linkInfo.LinkId ) {
				m_Util.val_to_bin(( unsigned char* )( pKkval + linkInfo.LinkId )->KKval1, strVal );	
			}
		}
	}

	return 0;
}

//========================================================================
// 所得税　不動産連動
//------------------------------------------------------------------------
// 引数
//			pRetAry		:			金額
//------------------------------------------------------------------------
// 返送値
//			0			:			正常終了
//			-1			:			エラー
//========================================================================
int CTaxationList31View::StzRendo_Fudousan( CStringArray *pRetAry )
{
	ASSERT( pRetAry );
	if( pRetAry == nullptr ) {
		return -1;
	}

	KKVAL *pKkval = &m_pTaxListData->NRec.KFval[0];
	LINKINFO linkInfo = { 0 };

	CString strVal;
	int count = ( int )pRetAry->GetCount();
	for( int i = 0; i < count; i++ ) {
		memset( &linkInfo, '\0', sizeof( LINKINFO ));
		strVal = pRetAry->GetAt( i );

		linkInfo = FUDOUSANINFO[i];
		if( linkInfo.LinkFlg ) {
			// 項目名
			if( pKkval + linkInfo.LinkId ) {
				m_Util.cstring_to_char(( unsigned char* )( pKkval + linkInfo.LinkId )->KKnam, strVal, 40 );
			}
		}
		else {
			// 金額
			if( pKkval + linkInfo.LinkId ) {
				// 金額がセットされていれば、足し合わせる
				if( m_Arith.l_test(( pKkval + linkInfo.LinkId )->KKval1 ) != 0 ) {
					char arith[MONY_BUF_SIZE] = { 0 };
					m_Util.val_to_bin(( unsigned char* )arith, strVal );
					m_Arith.l_add(( pKkval + linkInfo.LinkId )->KKval1, ( pKkval + linkInfo.LinkId )->KKval1, arith );
				}
				else {
					m_Util.val_to_bin(( unsigned char* )( pKkval + linkInfo.LinkId )->KKval1, strVal );	
				}
			}
		}
	}

	return 0;
}

//========================================================================
// 所得税　農業連動
//------------------------------------------------------------------------
// 引数
//			pRetAry		:			金額
//------------------------------------------------------------------------
// 返送値
//			0			:			正常終了
//			-1			:			エラー
//========================================================================
int CTaxationList31View::StzRendo_Nougyou( CStringArray *pRetAry )
{
	ASSERT( pRetAry );
	if( pRetAry == nullptr ) {
		return -1;
	}

	KKVAL *pKkval = &m_pTaxListData->NRec.KNval[0];
	LINKINFO linkInfo = { 0 };

	CString strVal;
	int count = ( int )pRetAry->GetCount();
	for( int i = 0; i < count; i++ ) {
		memset( &linkInfo, '\0', sizeof( LINKINFO ));
		strVal = pRetAry->GetAt( i );

		linkInfo = NOUGYOUINFO[i];
		if( linkInfo.LinkFlg ) {
			// 項目名
			if( pKkval + linkInfo.LinkId ) {
				m_Util.cstring_to_char(( unsigned char* )( pKkval + linkInfo.LinkId )->KKnam, strVal, 40 );
			}
		}
		else {
			// 金額
			if( pKkval + linkInfo.LinkId ) {
				// 金額がセットされていれば、足し合わせる
				if( m_Arith.l_test(( pKkval + linkInfo.LinkId )->KKval1 ) != 0 ) {
					char arith[MONY_BUF_SIZE] = { 0 };
					m_Util.val_to_bin(( unsigned char* )arith, strVal );
					m_Arith.l_add(( pKkval + linkInfo.LinkId )->KKval1, ( pKkval + linkInfo.LinkId )->KKval1, arith );
				}
				else {
					m_Util.val_to_bin(( unsigned char* )( pKkval + linkInfo.LinkId )->KKval1, strVal );	
				}
			}
		}
	}

	return 0;
}