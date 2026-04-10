#include "stdafx.h"
#include "TaxationListView.h"
//2015.03.05 INSERT START
#include "ShinSpecific22.h"
//2015.03.05 INSERT END
//2017.03.01 INSERT START
#include "ShinSpecific22keika.h"
//2017.03.01 INSERT END

//== 定数 ==
const long	HeadLn		=	9L;				// ヘッダ部　使用行数 
const long	EndLn		=	2L;				// エンド部　使用行数
const long	FrameLn		=	4L;				// フレーム部　先頭行
const long	OutColm		=	10L;			//  外枠拡張
const long	TaniColm	=	6L;				//  桁取拡張
const long	DownLn		=	2L;
const long	PH			=	(2540L / 10L);	// 1 character
const long	PV1			=	(2540L / 6L);	// 1/6 LF
const long	PV2			=	(2540L / 4L);	// 1/4 LF
const long	PV3			=	(2540L / 3L);	// 1/3 LF


//-----------------------------------------------------------------------------
// 特定収入に係る調整額計算書
//-----------------------------------------------------------------------------
// 引数	ListType	：
//		ListPage	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecificList( int ListType, int ListPage )
{
	int		status, sign, kojin;
	char	PW0[6]={0};

	// 初期化
	memset( m_SDATE, '\0', sizeof(m_SDATE) );
	memset( m_EDATE, '\0', sizeof(m_EDATE) );
	memset( m_COPNM, '\0', sizeof(m_COPNM) );
	memset( m_COPNO, '\0', sizeof(m_COPNO) );
	memset( m_DMME, '\0', sizeof(m_DMME) );

	// 調整税額処理
//2015.03.05 UPDATE START
	//// [04'07.29]
	//m_Arith.l_input( PW0, _T("50") );
	//if( m_Arith.l_cmp( m_pSpcListData->stval.SpTswr, PW0 ) > 0 ){
	//	sign = 1;	// 調整割合　有り
	//}
	//else{
	//	sign = 0;	// 調整割合　無し
	//}

	sign = 0;
//2015.03.05 UPDATE END

	// 課税期間
	char	yy, mm, dd;
	kojin = 0;
	switch( m_pSnHeadData->Sn_SKKBN ){
		case 1 : case 3 :
			if( IsJcContinuePrint() ){
				m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd);
				sprintf_s(m_SDATE, sizeof(m_SDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
				m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
				sprintf_s(m_EDATE, sizeof(m_EDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
			}
			else{
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd);
				sprintf_s(m_SDATE, sizeof(m_SDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
				sprintf_s(m_EDATE, sizeof(m_EDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
			}
			break;
		default:
			if( IsJcContinuePrint() ){
				m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd);
				sprintf_s(m_SDATE, sizeof(m_SDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
				m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd);
				sprintf_s(m_EDATE, sizeof(m_EDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
			}
			else{
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd);
				sprintf_s(m_SDATE, sizeof(m_SDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd);
				sprintf_s(m_EDATE, sizeof(m_EDATE), _T("%02X年%02X月%02X日"), yy, mm, dd);
			}
			if( ((m_pZmSub->zvol->apno&0xff)==0x10) && !(m_pSnHeadData->Sn_KOANB&0x01) || (m_pSnHeadData->Sn_KOJIN&0x01) ){
				kojin = 1;
			}
			break;
	}
	// 会社名
	if( m_pPset->psSigCoprn&0x02 ){
		//顧問先
		memmove( m_COPNM, m_PrintCmInfo->CopNam, sizeof(m_PrintCmInfo->CopNam) );
		sprintf_s( m_COPNO, sizeof(m_COPNO), _T("[%08d]"), m_PrintCmInfo->CopNum );
	}
	else{
		// マスター
		if( kojin ){	// 個人事業者の場合は代表者氏名
			memmove( m_COPNM, m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );
		}
		else{			// 法人名
			memmove( m_COPNM, m_pSnHeadData->Sn_CONAM, sizeof(m_pSnHeadData->Sn_CONAM) );
		}
		sprintf_s( m_COPNO, sizeof(m_COPNO), _T("[%08d]"), m_pZmSub->zvol->v_cod );
	}
	if( m_pPset->psSigCoprn&0x200 ){
		memset( m_COPNO, '\0', sizeof(m_COPNO) );
		strcpy_s( m_DMME, sizeof(m_DMME), _T("          ") );
	}

	switch( ListType ){
		case 1 :
//2017.03.01 UPDATE START
//			status = SpecList10( sign );
			if( IsKeikaType_Keisansyo() == TRUE ){
				if( m_pSnHeadData->SVmzsw == 1 ){
					status = SpecList10ExKeika( sign );
				}else{
					status = SpecList10Ex( sign );
				}
			}else{
				status = SpecList10( sign );
			}
//2017.03.01 UPDATE END
			break;
		case 2 :
//			status = SpecList21( sign, ListPage );
//2017.03.01 UPDATE START
//			status = SpecList21Ex( sign, ListPage );
			if( IsKeikaType_Keisansyo() == TRUE ){
				if( m_pSnHeadData->SVmzsw == 1 ){
					status = SpecList21Ex( sign, ListPage );
				}else{
					status = SpecList21Ex2( sign, ListPage );
				}
			}else{
				status = SpecList21Ex( sign, ListPage );
			}
//2017.03.01 UPDATE END
			break;
		case 3 :
//2015.03.05 UPDATE START
//			status = SpecList22( sign, ListPage );
			status = SpecList22( sign, ListPage, ID_ICSSH_SPC_5PER );
//2015.03.05 UPDATE END
			break;
		case 4 :
			status = SpecList34( sign );
			break;
		case 5 :	
			memset( PW0, '\0', 6 );
//			m_Arith.l_input( PW0, &m_pSnHeadData->m_UriRatioBorder );
//			if( m_Arith.l_cmp(m_pSpcListData->stval.SpUwri, PW0) >= 0 ){
			if( m_pSnHeadData->IsRatioOver95() ){
				// 課税売上割合９５％以上
				status = SpecList50( sign );
			}
			else{
				// 課税売上割合９５％未満
				if( m_pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
					// 比例配分
					status = SpecList52( sign );
				}
				else{
					// 個別対応
					status = SpecList51( sign );
				}
			}
			// 経過措置チェック
			if( m_pSnHeadData->SVmzsw == 0 ){
				status = 1;
			}
			break;
		case 6 :
			status = SpecList60( sign );
			status = 1;
			break;
//2015.03.12 INSERT START
		case 7 :	
			memset( PW0, '\0', 6 );
//			m_Arith.l_input( PW0, &m_pSnHeadData->m_UriRatioBorder );
//			if( m_Arith.l_cmp(m_pSpcListData->stval.SpUwri, PW0) >= 0 ){
//2015.03.12 UPDATE START
//			if( m_pSnHeadData->IsRatioOver95() ){
			if( IsRatioOver95() ){
//2015.03.12 UPDATE END
				// 課税売上割合９５％以上
				status = SpecList50Ex( sign );
			}
			else{
				// 課税売上割合９５％未満
				if( m_pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
					// 比例配分
					status = SpecList52Ex( sign );
				}
				else{
					// 個別対応
					status = SpecList51Ex( sign );
				}
			}
			break;
		case 8:
			status = SpecList22( sign, ListPage, ID_ICSSH_SPC_8PER );
			break;
//2017.03.01 INSERT START
		case 9:
			status = SpecList22Keika( sign, ListPage, ID_ICSSH_SPC_8PER );
			break;
		case 10:
			if( IsRatioOver95() ){
				// 課税売上割合９５％以上
				status = SpecList50Keika( sign );
			}
			else{
				// 課税売上割合９５％未満
				if( m_pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
					// 比例配分
					status = SpecList52Keika( sign );
				}
				else{
					// 個別対応
					status = SpecList51Keika( sign );
				}
			}
			break;
//2017.03.01 INSERT END
//2015.03.12 INSERT END
		default:
			status = 1;
			break;
	}

	return status;
}

//-----------------------------------------------------------------------------
// 計算表-１　資産の譲渡等の対価の額の計算（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList10( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = (SFp_xc|SFp_yc|SFscut|SFcitv);	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));				// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_XPosition = ( PH *  30L );
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	if( m_pVprn->DrawString(0, m_XPosition, m_YPosition, TBUF, &m_SfPar) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 計算表１　資産の譲渡等の対価の額の計算書
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表１　資産の譲渡等の対価の額の計算表") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,							// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
		(	m_XPosition +								// 最終列  座標
			( 70L * PH ) ),								// 外枠拡張
			m_YPosition + (PV1 * 18L)  );				// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=5; i++ ){
		if( i != 1 ) 
			m_pVprn->DrawWframe( 0,
					LKnrW1,								// 線種
					m_XPosition,						// 開始列　座標
					m_YPosition + m_StartLine, 			// 開始行　座標
				(	m_XPosition +						// 最終列  座標
					( 70L * PH ) ),						// 外枠拡張
					m_YPosition + m_StartLine  );		// 最終行　座標
		else
			m_pVprn->DrawWframe( 0,
					LKnrW1,								// 線種
					m_XPosition + ( PH * 3L ),			// 開始列　座標
						m_YPosition + m_StartLine, 		// 開始行　座標
					(	m_XPosition +					// 最終列  座標
					( 70L * PH ) ),						// 外枠拡張
					m_YPosition + m_StartLine  );		// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	
				strcpy_s( TBUF, sizeof( TBUF ), "①");
				break;
			case 1 :
				strcpy_s( TBUF, sizeof( TBUF ), "②");
				break;
			case 2 :
				strcpy_s( TBUF, sizeof( TBUF ), "③");
				break;
			case 3 :
				strcpy_s( TBUF, sizeof( TBUF ), "④");
				break;
			case 4 :
				strcpy_s( TBUF, sizeof( TBUF ), "⑤");
				break;
			default:
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;							// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );					// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;						// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;							// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;							// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;							// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;							// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;							// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;					// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;							// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}
		m_StartLine += ( PV1 * 3L );
	}
	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH *  8L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 6 ) );				// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 15 ) );				// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 18 ) );				// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　　　額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("円") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 1;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld = PV1;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH - 10L, m_YPosition + (PV1 * 6L)-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof( TBUF ), _T("通常の課税売上げ・役員へ贈与及び低額譲渡") );
//---------------
	strcpy_s( TBUF, sizeof( TBUF ), _T("通常の課税売上げ・役員への贈与及び低額譲渡") );
//---------------
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税標準額に対する消費税額の計算の特例適用の課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("免税売上げ（輸出取引等）") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("非課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("資産の譲渡等の対価の額の合計額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表３①、計算表４①へ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 金額
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=5; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ) {
			case 0 :	
				if( (m_pSnHeadData->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
					;
				}
				else{
					m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKzur, FMTX12 );
				}
				break;
			case 1 :
				if( (m_pSnHeadData->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
					m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKzur, FMTX12 );
				}
				break;
			case 2 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpMzur, FMTX12 );
				break;
			case 3 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpHkur, FMTX12 );
				break;
			case 4:
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTuwb, FMTX12 );
				break;
			default:
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc )					// ファンクションサイン
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );

	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表２（1）　特定収入金額合計（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//		page	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList21( int sign, int page )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_XPosition = ( PH *  30L );
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}


	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表２（１）　特定収入の金額及び内訳表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_pSpcListData->TsTbl.tp_rnum > 17 ){
		sprintf_s( TBUF, sizeof(TBUF), _T("ﾍﾟｰｼﾞ番号 %3d"), page );
	}
	else{
		strcpy_s( TBUF, sizeof(TBUF), _T(" ") );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  60L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
	
	m_XPosition = ( PH *  5L );
	m_YPosition = ( PV1 * (HeadLn + 7L) );
	for( int ln = 0; ln != 17; ln++ ) {
		// 横線
		m_pVprn->DrawWframe( 0,
				LKnrW1,											// 線種
				m_XPosition,									// 開始列　座標
				m_YPosition, 									// 開始行　座標
			(	m_XPosition +									// 最終列  座標
				( 70L * PH ) ),									// 外枠拡張
				m_YPosition  );									// 最終行　座標
		// 番号
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( ln ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①" );	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②" );	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③" );	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④" );	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤" );	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥" );	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦" );	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧" );	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨" );	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩" );	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪" );	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫" );	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬" );	break;
			case 13:	strcpy_s( TBUF, sizeof( TBUF ), "⑭" );	break;
			case 14:	strcpy_s( TBUF, sizeof( TBUF ), "⑮" );	break;
			case 15:	strcpy_s( TBUF, sizeof( TBUF ), "⑯" );	break;
			case 16:	strcpy_s( TBUF, sizeof( TBUF ), "⑰" );	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 4L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 15L ) + ( PH / 2 ), m_YPosition + PV2, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition += (PV3);
	}

	// 縦線
	m_YPosition = ( PV1 * ( HeadLn + 7L ) );
	m_XPosition = ( PH *  21L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 32L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  24L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  41L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標


	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  7L ) - ( PH / 2 );
	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内      容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 16L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  7L ) - ( PH * 2L );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("合   　　   計") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 16L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  24L ) - ( PH / 2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特　定　収　入") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  24L ) - ( PH * 2 );
	m_XPosition = ( PH *  22L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  24L ) - ( PH * 2 );
	m_XPosition = ( PH *  22L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表３②") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 3 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  40L ) + ( PH / 2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａのうち課税仕入れ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 1L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("等にのみ使途が特定") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("されている金額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("係る特定収入｣）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(１)②,(３)②") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH + ( PH / 2 ), m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){	
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  58L ) - PH;
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａ－Ｂの金額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 1L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入以外の") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入｣）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｃ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH / 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表４②") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 4L );
	m_XPosition = ( PH *  28L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 21L );
	m_XPosition = ( PH *  45L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 38L );
	m_XPosition = ( PH *  62L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 最終ページ合計印刷チェック
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  23L );
	memset( TBUF, '\0', sizeof(TBUF) );
	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGsyu, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  39L ) + PH;
	m_XPosition = ( PH *  40L );

	char	WORK0[6]={0};
	m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu, m_pSpcListData->stval.SpKsyu );
	m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu );
	memset( TBUF, '\0', sizeof(TBUF) );
	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_print( TBUF, WORK0, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  57L );
	memset( TBUF, '\0', sizeof(TBUF) );
	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	TSREC	TSrec;
	int		Rno, Cnt;
	long	lineCount;

	Rno = ( (page-1) * 16 );
	for( Cnt = 0; Cnt != 16; Cnt++, Rno++ ){
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, Rno ) ){
			return FALSE;
		}
		// 行ポイント
		lineCount = (PV3 * Cnt);
		// 名称
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  3L );
		strcpy_s( TBUF, sizeof(TBUF), TSrec.TsName );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 20L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		// 5% ('15.03.02)
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  23L );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsGsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		// 5%('15.03.02)
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  40L );
		memset( WORK0, '\0', sizeof(WORK0) );
		m_Arith.l_add( WORK0, TSrec.TsDtl[0].TsTsyu, TSrec.TsDtl[0].TsKsyu );
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsHsyu );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, WORK0, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		// 5%('15.02.02)
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  57L );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsFsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}
	}

	return FALSE;
}

//2015.03.05 INSERT START
//-----------------------------------------------------------------------------
// 計算表２（1）　特定収入金額合計（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//		page	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList21Ex( int sign, int page )
{
	if( GetMxList21Ex() == 0 ){
		return TRUE;
	}

	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_XPosition = ( PH *  30L );
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}


	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表２（１）　特定収入の金額及びその内訳書") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_pSpcListData->TsTbl.tp_rnum > 17 ){
		sprintf_s( TBUF, sizeof(TBUF), _T("ﾍﾟｰｼﾞ番号 %3d"), page );
	}
	else{
		strcpy_s( TBUF, sizeof(TBUF), _T(" ") );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  60L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
//-- '15.04.29 --
//			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
//---------------
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標
//---------------
	
	m_XPosition = ( PH *  5L );
//-- '15.04.29 --
//	m_YPosition = ( PV1 * (HeadLn + 7L) );
//---------------
	m_YPosition = ( PV1 * (HeadLn + 9L) );
//---------------
	for( int ln = 0; ln != 17; ln++ ) {
		// 横線
		m_pVprn->DrawWframe( 0,
				LKnrW1,											// 線種
				m_XPosition,									// 開始列　座標
				m_YPosition, 									// 開始行　座標
			(	m_XPosition +									// 最終列  座標
				( 70L * PH ) ),									// 外枠拡張
				m_YPosition  );									// 最終行　座標
		// 番号
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( ln ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①" );	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②" );	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③" );	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④" );	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤" );	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥" );	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦" );	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧" );	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨" );	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩" );	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪" );	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫" );	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬" );	break;
			case 13:	strcpy_s( TBUF, sizeof( TBUF ), "⑭" );	break;
			case 14:	strcpy_s( TBUF, sizeof( TBUF ), "⑮" );	break;
			case 15:	strcpy_s( TBUF, sizeof( TBUF ), "⑯" );	break;
			case 16:	strcpy_s( TBUF, sizeof( TBUF ), "⑰" );	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 4L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 15L ) + ( PH / 2 ), m_YPosition + PV2, TBUF, &m_SfPar ) ){
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 11L ) + ( PH / 2 ), m_YPosition + PV2, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition += (PV3);
	}

	//タイトル行 横線
//-- '15.04.29 --
//	m_YPosition = ( PV1 * ( HeadLn + 7L ) - PV3);
//---------------
	m_YPosition = ( PV1 * HeadLn ) + PV1;
//---------------
	m_XPosition = ( PH *  42L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
//-- '15.04.29 --
//			m_XPosition + ( PH * 22L ),							// 最終列  座標
//---------------
			m_XPosition + ( PH * 33L ),							// 最終列  座標
//---------------
			m_YPosition);										// 最終行　座標

	// 縦線
//-- '15.04.29 --
//	m_YPosition = ( PV1 * ( HeadLn + 7L ) );
//---------------
	m_YPosition = ( PV1 * ( HeadLn + 9L ) );
//---------------
//	m_XPosition = ( PH *  21L );
	m_XPosition = ( PH *  17L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 32L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  24L );
	m_XPosition = ( PH *  20L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
//-- '15.04.29 --
//			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
//---------------
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標
//---------------

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  41L );
	m_XPosition = ( PH *  31L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
//-- '15.04.29 --
//			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
//---------------
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標
//---------------

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  58L );
	m_XPosition = ( PH *  42L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
//-- '15.04.29 --
//			m_YPosition, 										// 開始行　座標
//---------------
			m_YPosition+PV1, 									// 開始行　座標
//---------------
			m_XPosition,	 									// 最終列  座標
//-- '15.04.29 --
//			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
//---------------
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標
//---------------

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  53L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
//-- '15.04.29 --
////			m_YPosition, 									// 開始行　座標
//			m_YPosition + (PV1 * (7L) - PV3),					// 開始行　座標
//---------------
			m_YPosition+PV1,									// 開始行　座標
//---------------
			m_XPosition,	 									// 最終列  座標
//-- '15.04.29 --
//			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
//---------------
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標
//---------------

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  64L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
//-- '15.04.29 --
//			m_YPosition, 										// 開始行　座標
//---------------
			m_YPosition+PV1,									// 開始行　座標
//---------------
			m_XPosition,	 									// 最終列  座標
//-- '15.04.29 --
//			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
//---------------
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標
//---------------

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  7L ) - ( PH / 2 );
	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内      容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 16L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

//-- '15.04.29 --
//	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//---------------
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 17L );
//---------------
//	m_XPosition = ( PH *  7L ) - ( PH * 2L );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("合   　　   計") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 16L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  20L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("資産の譲渡等の") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  20L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("対価以外の収入") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  24L ) - ( PH * 2 );

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  24L ) - ( PH / 2 );
//	m_XPosition = ( PH *  18L ) - ( PH / 2 );
	m_XPosition = ( PH *  31L );
	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("特　定　収　入") );
	strcpy_s( TBUF, sizeof(TBUF), _T("左のうち") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  24L ) - ( PH / 2 );
//	m_XPosition = ( PH *  18L ) - ( PH / 2 );
	m_XPosition = ( PH *  31L );
	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("特　定　収　入") );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  24L ) - ( PH * 2 );
//	m_XPosition = ( PH *  22L );
	m_XPosition = ( PH *  31L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//-- '15.04.29 --
//	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//---------------
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 17L );
//---------------
//	m_XPosition = ( PH *  24L ) - ( PH * 2 );
//	m_XPosition = ( PH *  22L );
	m_XPosition = ( PH *  31L );
	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表３②") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表３②へ") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT066z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 3 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

#ifdef _20150429_CUT_
	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  40L ) + ( PH / 2 );
	m_XPosition = ( PH *  42L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａのうち課税仕入れ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 22L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 1L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("等にのみ使途が特定") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 22L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("されている金額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 22L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 22L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("係る特定収入｣）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 22L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
#endif

//--> '15.04.29 INS START
	m_XPosition = ( PH *  41L ) + PH;
	m_YPosition = ( PV1 * HeadLn );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("うち税率4％が") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("適用される課税") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("仕入れ等にのみ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定され") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("ている金額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("(｢課税仕入れ等") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("に係る特定収入｣)") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
//<-- '15.04.29 INS END

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ") );
//-- '15.04.20 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ(8%)") );
//---------------
//--- '15.04.29 ---
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ(5%)") );
//-----------------
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ") );
//-----------------
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(１)②,(３)②") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(1)②,(3)②へ") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT066z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + PH + ( PH / 2 ), m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){	
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){	
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 17L ), TBUF, &m_SfPar ) ){	
//---------------
		return TRUE;
	}

//--> '15.04.29 INS START
	m_XPosition = ( PH *  52L ) + PH;
	m_YPosition = ( PV1 * HeadLn );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("うち税率6.3％が") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("適用される課税") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("仕入れ等にのみ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定され") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("ている金額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("(｢課税仕入れ等") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("に係る特定収入｣)") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
//<-- '15.04.29 INS END

//	m_XPosition = ( PH *  53L ) - ( PH / 2 );
	m_XPosition = ( PH *  53L );

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.20 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ(5%)") );
//---------------
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ(8%)") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｃ") );
//---------------
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(１)②,(３)②") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(1)②,(3)②へ") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(1)④,(3)④へ") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(1)②,(3)②へ") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT066z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + PH + ( PH / 2 ), m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){	
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){	
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 17L ), TBUF, &m_SfPar ) ){	
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  58L ) - PH;
	m_XPosition = ( PH *  64L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａ－(Ｂ＋Ｃ)") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 1L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ") );
	strcpy_s( TBUF, sizeof(TBUF), _T("(｢課税仕入れ等に") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("等に係る") );
	strcpy_s( TBUF, sizeof(TBUF), _T("係る特定収入以外") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("の特定収入｣)") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//--> '15.04.29 CUT START
//	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入｣）") );
//	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
//	m_SfPar.sf_hvsg = 0;										// 出力方向
////	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
//	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
//	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
//	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
//	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
//	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
//	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
//	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
////	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//		return TRUE;
//	}
//<-- '15.04.29 CUT END
	
	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｃ") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｄ") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH / 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH / 2 ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表４②") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表４②へ") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT066z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ	
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 17L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_XPosition = ( PH *  20L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 4L );
	m_XPosition = ( PH *  31L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_XPosition = ( PH *  42L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 21L );
	m_XPosition = ( PH *  53L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 38L );
	m_XPosition = ( PH *  64L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	// 最終ページ合計印刷チェック
	//資産の譲渡等の対価以外の収入　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  20L ) - ( PH/2 );

	TSREC	TSrec;
	char	WORK0[6]={0};
	int rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}
		// 資産の譲渡等の対価以外の収入合計
		m_Arith.l_add( WORK0, WORK0, TSrec.TsJsyu );
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
//		m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGsyu, FMTX12 );
		m_Arith.l_print( TBUF, WORK0, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	//左のうち特定収入　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  23L );
	m_XPosition = ( PH *  31L ) - ( PH/2 );


	memset( WORK0, '\0', sizeof(WORK0) );
	char	WORK1[6]={0};
	char	WORK_sum[6]={0};
	rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}
		
		//m_Arith.l_add( WORK0, WORK0, TSrec.TsJsyu );
		m_Arith.l_add( WORK1, WORK1, TSrec.TsDtl[1].TsGsyu ); //８％
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsGsyu ); //５％
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
//		m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGsyu, FMTX12 );
		m_Arith.l_add( WORK_sum, WORK0, WORK1 );
		m_Arith.l_print( TBUF, WORK_sum, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	//Ａのうち課税仕入れ等にのみ使途が特定されている金額(「課税仕入れ等に係る特定収入」)　５％　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  42L ) - ( PH/2 );

//	char	WORK0[6]={0};
	memset( WORK0, '\0', sizeof(WORK0) );
//	m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu, m_pSpcListData->stval.SpKsyu );
//	m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu );
//	m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu4, m_pSpcListData->stval.SpKsyu4 );
//	m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu4 );

	rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}

		// 課税仕入等にのみ係る特定収入
//-- '15.04.25 --
//		if( m_pSnHeadData->IsKobetuSiireAnbun() == TRUE ){					// 個別対応
//			m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsSsyu );
//		}
//		else{																	// 比例配分
//			m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsTsyu );
//		}
//---------------
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsTsyu );
//---------------
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_print( TBUF, WORK0, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}


	//Ａのうち課税仕入れ等にのみ使途が特定されている金額(「課税仕入れ等に係る特定収入」)　８％　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  39L ) + PH;
//	m_XPosition = ( PH *  40L );
	m_XPosition = ( PH *  53L ) - ( PH/2 );

	memset( WORK0, '\0', sizeof(WORK0) );
	//m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu63, m_pSpcListData->stval.SpKsyu63 );
	//m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu63 );
	rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}

		// 課税仕入等にのみ係る特定収入
//-- '15.04.25 --
//		if( m_pSnHeadData->IsKobetuSiireAnbun() == TRUE ){					// 個別対応
//			m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[1].TsSsyu );
//		}
//		else{																	// 比例配分
//			m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[1].TsTsyu );
//		}
//---------------
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[1].TsTsyu );
//---------------
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_print( TBUF, WORK0, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	//Ａ－Ｂの金額(「課税仕入れ等に係る特定収入以外の特定収入」)　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  57L );
//	m_XPosition = ( PH *  59L + PH * 1/2 );
	m_XPosition = ( PH *  64 ) - ( PH/2 );

	memset( WORK0, '\0', sizeof(WORK0) );
	memset( WORK1, '\0', sizeof(WORK1) );
	memset( WORK_sum, '\0', sizeof(WORK_sum) );
	rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}
		
		m_Arith.l_add( WORK1, WORK1, TSrec.TsDtl[1].TsFsyu ); //８％
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsFsyu ); //５％
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	if( ( page * 16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_add( WORK_sum, WORK0, WORK1 );
		m_Arith.l_print( TBUF, WORK_sum, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_xfld = ( 11L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//	TSREC	TSrec;
	int		Rno, Cnt;
	long	lineCount;

	Rno = ( (page-1) * 16 );
	for( Cnt = 0; Cnt != 16; Cnt++, Rno++ ){
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, Rno ) ){
			return FALSE;
		}
		// 行ポイント
		lineCount = (PV3 * Cnt);
		// 名称
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
//		m_XPosition = ( PH *  3L );
		m_XPosition = ( PH *  5L );
		strcpy_s( TBUF, sizeof(TBUF), TSrec.TsName );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
//		m_SfPar.sf_xfld = ( 20L * PH );							// 横出力領域幅
		m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//-- '15.04.29 --
//		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
			return TRUE;
		}

		//資産の譲渡等の対価以外の収入
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
//		m_XPosition = ( PH *  23L );
		m_XPosition = ( PH *  20L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, TSrec.TsJsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
//		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_xfld = ( 11L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//-- '15.04.29 --
//		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
			return TRUE;
		}

		char sum[6] = {0};
		//左のうち特定収入
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
//		m_XPosition = ( PH *  23L );
		m_XPosition = ( PH *  31L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );
//		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsGsyu, FMTX12 );
		m_Arith.l_print( TBUF, sum, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
//		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_xfld = ( 11L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//-- '15.04.29 --
//		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
			return TRUE;
		}

		//Ａのうち課税仕入れ等にのみ使途が特定されている金額(「課税仕入れ等に係る特定収入」)　５％
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  42L ) - ( PH/2 );
		memset( WORK0, '\0', sizeof(WORK0) );
//		m_Arith.l_add( WORK0, TSrec.TsDtl[0].TsTsyu, TSrec.TsDtl[0].TsKsyu );
//		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[1].TsHsyu );
//		memset( TBUF, '\0', sizeof(TBUF) );
//		m_Arith.l_print( TBUF, WORK0, FMTX12 );
//-- '15.04.25 --
//		if( m_pSnHeadData->IsKobetuSiireAnbun() == TRUE ){
//			m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsSsyu, FMTX12 );
//		}else{
//			m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsTsyu, FMTX12 );
//		}
//---------------
		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsTsyu, FMTX12 );
//---------------
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
//		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_xfld = ( 11L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//-- '15.04.29 --
//		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
			return TRUE;
		}


		//Ａのうち課税仕入れ等にのみ使途が特定されている金額(「課税仕入れ等に係る特定収入」)　８％
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
//		m_XPosition = ( PH *  40L );
		m_XPosition = ( PH *  53L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
//-- '15.04.25 --
//		if( m_pSnHeadData->IsKobetuSiireAnbun() == TRUE ){
//			m_Arith.l_print( TBUF, TSrec.TsDtl[1].TsSsyu, FMTX12 );
//		}else{
//			m_Arith.l_print( TBUF, TSrec.TsDtl[1].TsTsyu, FMTX12 );
//		}
//---------------
		m_Arith.l_print( TBUF, TSrec.TsDtl[1].TsTsyu, FMTX12 );
//---------------
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 11L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//-- '15.04.29 --
//		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
			return TRUE;
		}

		memset( sum, '\0', sizeof(sum));
		//Ａ－Ｂの金額(「課税仕入れ等に係る特定収入以外の特定収入」)
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
//		m_XPosition = ( PH *  57L );
		m_XPosition = ( PH *  64L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof(TBUF) );
//		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsFsyu, FMTX12 );
		m_Arith.l_add( sum, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
		m_Arith.l_print( TBUF, sum, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
//		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_xfld = ( 11L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//-- '15.04.29 --
//		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
			return TRUE;
		}
	}

	return FALSE;
}
//2015.03.05 INSERT END
//-----------------------------------------------------------------------------
// 計算表２（２）　特定収入金額合計（個別対応方式）	（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：	
//		page	：	
//-----------------------------------------------------------------------------
//2015.03.05 UPDATE STARt
//int CTaxationListView::SpecList22( int sign, int page )
int CTaxationListView::SpecList22( int sign, int page, int zeiType )
//2015.03.05 UPDATE END
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 計算表２（２）　特定収入金額合計（個別対応方式）
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表２（２）　特定収入の金額及びその内訳書（個別対応方式用）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_pSpcListData->TsTbl.tp_rnum > 17 ){
		sprintf_s( TBUF, sizeof(TBUF), _T("ﾍﾟｰｼﾞ番号 %3d"), page );
	}
	else{
		strcpy_s( TBUF, sizeof(TBUF), _T(" ") );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  60L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 39L) + (PV2*2) );				// 最終行　座標

//2017.03.01 UPDATE START
//	m_YPosition = ( PV1 * (HeadLn + 1L) );
	m_YPosition = ( PV1 * (HeadLn + 1L) ) + PV1/2;
//2017.03.01 UPDATE END
	m_XPosition = ( PH *  41L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 34L * PH ) ),										// 外枠拡張
			m_YPosition  );										// 最終行　座標

	m_XPosition = ( PH *  5L );
	m_YPosition = ( PV1 * (HeadLn + 8L) );
	for( int ln = 0; ln != 17; ln++ ) {
		// 横線
		m_pVprn->DrawWframe( 0,
				LKnrW1,											// 線種
				m_XPosition,									// 開始列　座標
				m_YPosition, 									// 開始行　座標
			(	m_XPosition +									// 最終列  座標
				( 70L * PH ) ),									// 外枠拡張
				m_YPosition  );									// 最終行　座標

		// 番号
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( ln ) {
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①" );	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②" );	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③" );	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④" );	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤" );	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥" );	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦" );	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧" );	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨" );	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩" );	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪" );	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫" );	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬" );	break;
			case 13:	strcpy_s( TBUF, sizeof( TBUF ), "⑭" );	break;
			case 14:	strcpy_s( TBUF, sizeof( TBUF ), "⑮" );	break;
			case 15:	strcpy_s( TBUF, sizeof( TBUF ), "⑯" );	break;
			case 16:	strcpy_s( TBUF, sizeof( TBUF ), "⑰" );	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 4L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 15L ) + ( PH / 2 ), m_YPosition + PV2, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition += (PV3);
	}

	// 縦線
//2017.03.01 INSERT START
	if( IsKeikaType_Keisansyo() == TRUE ){
		m_YPosition = ( PV1 * HeadLn );
		m_XPosition = ( PH *  41L );
		m_pVprn->DrawWframe( 0,
				LKnrW1,												// 線種
				m_XPosition,										// 開始列　座標
				m_YPosition, 										// 開始行　座標
				m_XPosition,	 									// 最終列  座標
				m_YPosition +  PV1 + (PV1/2));						// 最終行　座標

	}
//2017.03.01 INSERT END

	m_YPosition = ( PV1 * ( HeadLn + 8L ) );
	m_XPosition = ( PH *  21L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 32L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  24L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 39L) + PV3 + ( PV3 / 2 ) );	// 最終行　座標

//-- '15.04.29 --
//	m_YPosition = ( PV1 * HeadLn );
//---------------
//2017.03.01 UPDATE START
//	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_YPosition = ( PV1 * HeadLn ) + PV1 + (PV1/2);
//2017.03.01 UPDATE END
//---------------
	m_XPosition = ( PH *  41L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
//-- '15.04.29 --
//			m_YPosition + (PV1 * 39L) + PV3 + ( PV3 / 2 ) );	// 最終行　座標
//---------------
//2017.03.01 UPDATE START
//			m_YPosition + (PV1 * 38L) + PV3 + ( PV3 / 2 ) );	// 最終行　座標
			m_YPosition + (PV1 * 38L) + PV3 + ( PV3 / 2 ) - (PV1/2));	// 最終行　座標
//2017.03.01 UPDATE END
//---------------

//2017.03.01 UPDATE START
//	m_YPosition = ( PV1 * (HeadLn + 1L) );
	m_YPosition = ( PV1 * (HeadLn + 1L) ) + (PV1/2);
//2017.03.01 UPDATE END
	m_XPosition = ( PH *  58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
//2017.03.01 UPDATE START
//			m_YPosition + (PV1 * 39L) + PV3 );					// 最終行　座標
			m_YPosition + (PV1 * 39L) + PV3  - (PV1/2));		// 最終行　座標
//2017.03.01 UPDATE END

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH * 5L ) + ( PH * 2L );
	m_XPosition = ( PH * 7L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内      容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 16L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * (HeadLn + 1L) ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  7L ) - ( PH * 2L );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("合   　　   計") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 16L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

//2017.03.01 INSERT START
	m_YPosition = ( PV1 * HeadLn ) + PV1 + ( PV1/2 );
	m_XPosition = ( PH *  40L ) + ( PH / 2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("Ｄの金額のうち") );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 36L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}
//2017.03.01 INSERT END

	m_YPosition = ( PV1 * HeadLn );
//	m_XPosition = ( PH *  21L ) + ( PH * 2L );
	m_XPosition = ( PH *  23L );
	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等に係る") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入") );
//---------------
////2017.03.01 UPDATE START
//	if( zeiType == ID_ICSSH_SPC_8PER ){
//		strcpy_s( TBUF, sizeof(TBUF), _T("（税率6.3％）") );
//	}
//	else if( zeiType == ID_ICSSH_SPC_5PER ){
//		strcpy_s( TBUF, sizeof(TBUF), _T("（税率4％）に") );
//	}
////---------------

	if( IsKeikaType_Keisansyo() == TRUE ){
			strcpy_s( TBUF, sizeof(TBUF), _T("に係る特定収入") );
	}else{
		if( zeiType == ID_ICSSH_SPC_8PER ){
			strcpy_s( TBUF, sizeof(TBUF), _T("（税率6.3％）") );
		}
		else if( zeiType == ID_ICSSH_SPC_5PER ){
			strcpy_s( TBUF, sizeof(TBUF), _T("（税率4％）に") );
		}
	}
//2017.03.01 UPDATE END

	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//--> '15.04.29 INS START
	memset( TBUF, '\0', sizeof(TBUF) );
//2017.03.01 UPDATE START
	//if( zeiType == ID_ICSSH_SPC_8PER ){
	//	strcpy_s( TBUF, sizeof(TBUF), _T("に係る特定収入") );
	//}
	//else if( zeiType == ID_ICSSH_SPC_5PER ){
	//	strcpy_s( TBUF, sizeof(TBUF), _T("係る特定収入") );
	//}

	if( IsKeikaType_Keisansyo() == TRUE ){

	}else{
		if( zeiType == ID_ICSSH_SPC_8PER ){
			strcpy_s( TBUF, sizeof(TBUF), _T("に係る特定収入") );
		}
		else if( zeiType == ID_ICSSH_SPC_5PER ){
			strcpy_s( TBUF, sizeof(TBUF), _T("係る特定収入") );
		}
	}
////2017.03.01 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}
//<-- '15.04.29 INS END

//2017.03.01 INSERT START
	memset( TBUF, '\0', sizeof(TBUF) );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("(計算表２(1)Ｂ欄の") );
	}

	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
//2017.03.01 INSERT END

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２（1）の") );
//---------------
//2017.03.01 UPDATE START
//	if( zeiType == ID_ICSSH_SPC_8PER ){
//		strcpy_s( TBUF, sizeof(TBUF), _T("(計算表２(1)Ｃ)") );
//	}
//	else if( zeiType == ID_ICSSH_SPC_5PER ){
//		strcpy_s( TBUF, sizeof(TBUF), _T("(計算表２(1)Ｂ)") );
//	}
////---------------

	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("金額)") );
	}else{
		if( zeiType == ID_ICSSH_SPC_8PER ){
			strcpy_s( TBUF, sizeof(TBUF), _T("(計算表２(1)Ｃ)") );
		}
		else if( zeiType == ID_ICSSH_SPC_5PER ){
			strcpy_s( TBUF, sizeof(TBUF), _T("(計算表２(1)Ｂ)") );
		}
	}
//2017.03.01 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//2017.03.01 INSERT START
	memset( TBUF, '\0', sizeof(TBUF) );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("Ｄ") );
	}

	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

//2017.03.01 INSERT END

//--> '15.04.29 CUT START
//	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ欄の金額）") );
//	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
//	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
//	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
//	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
//	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
//	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
//	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
//	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
//		return TRUE;
//	}
//
//	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｄ") );
//	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
//	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
//	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
//	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
//	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
//	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
//	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
//	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//		return TRUE;
//	}
//
//	m_YPosition = ( PV1 * HeadLn );
////	m_XPosition = ( PH *  39L ) + PH + ( PH / 2 );
//	m_XPosition = ( PH *  40L ) + ( PH / 2 );
//	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("Ｄ　の　金　額　の　う　ち") );
//	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
//	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 36L * PH );								// 横出力領域幅
//	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
//	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
//	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
//	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
//	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
//	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 1L ), TBUF, &m_SfPar ) ){
//		return TRUE;
//	}
//<-- '15.04.29 CUT END

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  40L ) + ( PH / 2 );

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("うち課税売上げにのみ") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要す") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("うち課税売上げにのみ") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("要する課税仕入れ等に") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("る課税仕入れ等にのみ") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("要する課税仕入れ等に") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定されている") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されて") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定されている") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されて") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("いる特定収入") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("特定収入") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("いる特定収入") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｅ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５（２）②") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５（2）②へ") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH, m_YPosition + ( PV1 * 9L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  57L ) + ( PH / 2 );
	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("うち課税・非課税売上げ") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("うち課税・非課税売上げ") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("共通して要する課税仕入") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("に共通して要する課税") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("共通して要する課税仕") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("に共通して要する課税") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("れ等にのみ使途が特定") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("仕入れ等にのみ使途が") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("入れ等にのみ使途が特") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("仕入れ等にのみ使途が") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("されている特定収入") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("特定されている特定収入") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("定されている特定収入") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("特定されている特定収入") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｆ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５（２）④") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５（2）④へ") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH, m_YPosition + ( PV1 * 9L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 4L );
	m_XPosition = ( PH *  28L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 21L );
	m_XPosition = ( PH *  45L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
//	m_XPosition = ( PH *  24L ) + ( PH * 38L );
	m_XPosition = ( PH *  62L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 2 ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

//2015.03.05 DELETE START
//	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
////	m_XPosition = ( PH *  21L ) + ( PH * 2 );
//	m_XPosition = ( PH *  23L );
//	char	WORK0[6]={0};
//	m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu, m_pSpcListData->stval.SpKsyu );
//	m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu );
//	memset( TBUF, '\0', sizeof(TBUF) );
//	if( (page*16) <= m_pSpcListData->TsTbl.tp_rnum ){
//		sprintf_s( TBUF, sizeof(TBUF), _T(" ") );
//	}
//	else{
//		m_Arith.l_print( TBUF, WORK0, FMTX12 );
//	}
//	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
//	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
//	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
//	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
//	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
//	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
//	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
//	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//		return TRUE;
//	}
//
//	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
////	m_XPosition = ( PH *  39L ) + PH;
//	m_XPosition = ( PH *  40L );
//	memset( TBUF, '\0', sizeof(TBUF) );
//	if( ( page*16 ) <= m_pSpcListData->TsTbl.tp_rnum ){
//		sprintf_s( TBUF, sizeof(TBUF), _T(" ") );
//	}
//	else{
//		m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu, FMTX12 );
//	}
//	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
//	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
//	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
//	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
//	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
//	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
//	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
//	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//		return TRUE;
//	}
//
//	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
//	m_XPosition = ( PH *  57L );
//	memset( TBUF, '\0', sizeof(TBUF) );
//	if( (page*16) <= m_pSpcListData->TsTbl.tp_rnum ){
//		sprintf_s( TBUF, sizeof(TBUF), _T(" ") );
//	}
//	else{
//		m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsyu, FMTX12 );
//	}
//	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
//	m_SfPar.sf_hvsg = 0;										// 出力方向
//	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
//	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
//	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
//	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
//	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
//	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
//	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//		return TRUE;
//	}
//2015.03.05 DELETE END


	TSREC	TSrec;
	int		Rno, Cnt;
	int		lineCount;
//2015.03.05 INSERT START
	BOOL	last_page = FALSE;
	char	MONY1[6]={0}, MONY2[6]={0}, MONY3[6]={0};
//2015.03.05 INSERT END

//--> '15.03.02 INS START
	int		zrIdx=0;
//2015.03.05 UPDAE START
	//if( 1 ){	// 税率5% の帳票
	//	zrIdx=0;
	//}
	//else{		// 税率8% の帳票
	//	zrIdx=1;
	//}
	if( zeiType == ID_ICSSH_SPC_5PER ){	// 税率5% の帳票
		zrIdx=0;
	}
	else if( zeiType == ID_ICSSH_SPC_8PER ){		// 税率8% の帳票
		zrIdx=1;
	}else{
		return TRUE;
	}

	if( zeiType == ID_ICSSH_SPC_5PER ){	// 税率5% の帳票
		if( page == GetMxList22_5per() ){ // 最終ページ判定
			last_page = TRUE;
		}			
	}
	else if( zeiType == ID_ICSSH_SPC_8PER ){ // 税率8% の帳票
		if( page == GetMxList22_8per() ){ // 最終ページ判定
			last_page = TRUE;
		}			
	}else{
		return TRUE;
	}

//2015.03.05 UPDAE END
//<-- '15.03.02 INS END

//2015.03.05 UPDAE START
	//Rno = ( (page-1) * 16 );
	//for( Cnt=0; Cnt!=16; Cnt++, Rno++ ){
	//	if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, Rno ) ){
	//		return FALSE;
	//	}
	if( zeiType == ID_ICSSH_SPC_5PER ){	// 税率5% の帳票
		if( m_PageManage_22per5.IsEmpty() == TRUE ){
			return FALSE;
		}
		Rno = m_PageManage_22per5.GetAt( page - 1 );
	}else if( zeiType == ID_ICSSH_SPC_8PER ){		// 税率8% の帳票
		if( m_PageManage_22per8.IsEmpty() == TRUE ){
			return FALSE;
		}
		Rno = m_PageManage_22per8.GetAt( page - 1 );
	}

	for( Cnt=0; Cnt!=LINES_PER_PAGE_22; Rno++ ){
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, Rno ) ){
			if( last_page == TRUE ){
				break;
			}else{
				return FALSE;
			}
		}
//2015.03.05 UPDAE END

//2015.03.05 INSERT START
		if( zeiType == ID_ICSSH_SPC_5PER ){	// 税率5% の帳票
			if( m_pShinSpecific22per5 ){
				if( m_pShinSpecific22per5->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}
			}else{
				return FALSE;
			}
		}
		else if( zeiType == ID_ICSSH_SPC_8PER ){		// 税率8% の帳票
			if( m_pShinSpecific22per8 ){
				if( m_pShinSpecific22per8->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}
			}else{
				return FALSE;
			}
		}else{
			return TRUE;
		}
//2015.03.05 INSERT END

		// 行ポイント
		lineCount = (PV3 * Cnt);
		// 名称
		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  3L );
		strcpy_s( TBUF, sizeof(TBUF), TSrec.TsName );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 20L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  23L );
		memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
		//m_Arith.l_add( WORK0, TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsKsyu );
		//m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[zrIdx].TsHsyu );
		//memset( TBUF, '\0', sizeof(TBUF) );
		//m_Arith.l_print( TBUF, WORK0, FMTX12 );
//-- '15.04.25 --
//		m_Arith.l_print( TBUF, TSrec.TsDtl[zrIdx].TsSsyu, FMTX12 );
//---------------
		m_Arith.l_print( TBUF, TSrec.TsDtl[zrIdx].TsTsyu, FMTX12 );
//---------------
//2015.03.05 UPDATE END
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  40L );
		memset( TBUF, '\0', sizeof( TBUF ) );
//-- '15.04.25 --
//		m_Arith.l_print( TBUF, TSrec.TsDtl[zrIdx].TsTsyu, FMTX12 );
//---------------
		m_Arith.l_print( TBUF, TSrec.TsDtl[zrIdx].TsSsyu, FMTX12 );
//---------------
		memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  57L );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[zrIdx].TsKsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}
	
//2015.03.05 INSERT START
		Cnt++;
//2015.03.05 INSERT END
	}
//2015.03.05 INSERT START
	//合計印字
	if( last_page == TRUE ){
		
		int rno = 0;
		while( 1 ){
			if( rno ==  m_pSpcListData->TsTbl.tp_rnum ){
				break;
			}
			if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
				break;
			}

			if( zeiType == ID_ICSSH_SPC_5PER ){	// 税率5% の帳票
				if( m_pShinSpecific22per5 ){
					if( m_pShinSpecific22per5->IsNowZrtData( &TSrec ) == FALSE ){
						rno++;
						continue;
					}
				}else{
					return FALSE;
				}
			}
			else if( zeiType == ID_ICSSH_SPC_8PER ){ // 税率8% の帳票
				if( m_pShinSpecific22per8 ){
					if( m_pShinSpecific22per8->IsNowZrtData( &TSrec ) == FALSE ){
						rno++;
						continue;
					}
				}else{
					return FALSE;
				}
			}else{
				return TRUE;
			}

//-- '15.04.25 --
//			m_Arith.l_add( MONY1, MONY1, TSrec.TsDtl[zrIdx].TsSsyu );
//			m_Arith.l_add( MONY2, MONY2, TSrec.TsDtl[zrIdx].TsTsyu );
//---------------
			m_Arith.l_add( MONY1, MONY1, TSrec.TsDtl[zrIdx].TsTsyu );
			m_Arith.l_add( MONY2, MONY2, TSrec.TsDtl[zrIdx].TsSsyu );
//---------------
			m_Arith.l_add( MONY3, MONY3, TSrec.TsDtl[zrIdx].TsKsyu );

			rno++;
		}	

		// 行ポイント
		lineCount = (PV3 * LINES_PER_PAGE_22);

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  23L );
		memset( TBUF, '\0', sizeof(TBUF) );
		//m_Arith.l_add( WORK0, TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsKsyu );
		//m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[zrIdx].TsHsyu );
		//memset( TBUF, '\0', sizeof(TBUF) );
		//m_Arith.l_print( TBUF, WORK0, FMTX12 );
		m_Arith.l_print( TBUF, MONY1, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  40L );
		memset( TBUF, '\0', sizeof( TBUF ) );
		//m_Arith.l_print( TBUF, TSrec.TsDtl[zrIdx].TsTsyu, FMTX12 );
		m_Arith.l_print( TBUF, MONY2, FMTX12 );
		memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  57L );
		memset( TBUF, '\0', sizeof(TBUF) );
		//m_Arith.l_print( TBUF, TSrec.TsDtl[zrIdx].TsKsyu, FMTX12 );
		m_Arith.l_print( TBUF, MONY3, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

	}
//2015.03.05 INSERT END

	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表-３　特定収入割合の計算表, 計算表-４　調整割合の計算表 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList34( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 計算表-３　特定収入割合の計算表
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表３　特定収入割合の計算表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 15L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=4; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ) {
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );

	}
	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 12 ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 15 ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("資産の譲渡等の対価の額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑤）") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		if( m_pSnHeadData->SVmzsw == 1 ){
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑥Ｃ）") );
		}else{
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑥）") );
		}
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑤）") );
	}
//2017.03.01 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)のＡ）") );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＡ）") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ａ）") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		if( m_pSnHeadData->SVmzsw == 1 ){
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ａ）") );
		}else{
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＡ）") );
		}
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ａ）") );
	}
//2017.03.01 UPDATE END
//---------------
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("分母の額（①＋②）") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入割合（②÷③）") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("％") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 14L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 金額
	char	WORK0[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=5; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTuwb, FMTX12 );
				break;
			case 1 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGsyu, FMTX12 );
				break;
			case 2 :	
				memset( WORK0, '\0', 6 );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpGsyu );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 3 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTswr, FMTX4_1 );
				break;
			default:	
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );

	}

	//============================================================================
	// 計算表-４　調整割合の計算表
	//============================================================================
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表４　調整割合の計算表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 15L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=4; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );

	}
	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 33L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 12 ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 15 ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("資産の譲渡等の対価の額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑤）") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		if( m_pSnHeadData->SVmzsw == 1 ){
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑥Ｃ）") );
		}else{
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑥）") );
		}
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表１⑤）") );
	}
//2017.03.01 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入等に係る特定収入以外の特定収入") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等に係る特定収入以外の特定収入") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)のＣ）") );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＣ）") );
//---------------
//2017.03.01 UPDATE START
	if( IsKeikaType_Keisansyo() == TRUE ){
		if( m_pSnHeadData->SVmzsw == 1 ){
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｄ）") );
		}else{
			strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＣ）") );
		}
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｄ）") );
	}
//2017.03.01 UPDATE END
//---------------
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("分母の額（①＋②）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②の金額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 20L ), m_YPosition + ( PV1 * 13L ) + ( PV1 / 2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③の金額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 20L ), m_YPosition + ( PV1 * 15L ) - ( PV1 / 2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

//2017.03.01 INSERT START
	memset( TBUF, '\0', sizeof(TBUF) );
	if( IsKeikaType_Keisansyo() == TRUE ){
		if( m_pSnHeadData->SVmzsw == 1 ){
			strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(1)⑦、(2)⑭、(3)⑨へ") );
		}
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT072z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 13L ) - ( PV1 / 5 )+5, TBUF, &m_SfPar ) ){
		return TRUE;
	}
//2017.03.01 INSERT END

	m_YPosition = ( PV1 * (HeadLn + 33L) );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 17L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 30L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 33L) );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE END

	// 金額
	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=3; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTuwb, FMTX12 );
				break;
			case 1 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
				break;
			case 2 :
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			default:	
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );

	}

	m_YPosition = ( PV1 * (HeadLn + 40L) );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	memset( TBUF, '\0', sizeof(TBUF) );
	m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + ( PV1 / 2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * (HeadLn + 42L) );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	memset( WORK0, '\0', 6 );
	m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
	memset( TBUF, '\0', sizeof(TBUF) );
	m_Arith.l_print( TBUF, WORK0, FMTX12 );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) - ( PV1 / 2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表	売上割合９５％以上
//-----------------------------------------------------------------------------
// 引数	sign	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList50( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(１)　調整後税額の計算表（課税売上割合が95%以上の場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 27L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=8; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );
	}
	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 24L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 27L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ使途が特定されている特定収入") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率4％)にのみ使途が特定されている特定収入") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)のＢ）") );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＢ（５％））") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｂ）") );
//---------------
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　　４") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 17L),							// 開始列　座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 22L),							// 最終列  座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 14L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 19L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------


	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－③") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表４④）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){	
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){	
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("④×⑤　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 20L), TBUF, &m_SfPar ) ){	
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 20L), TBUF, &m_SfPar ) ){	
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（③＋⑥）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑦）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE END

	// 金額
	char	WORK0[6], WORK1[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=8; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgzg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz4, FMTX12 );
//2015.03.05 UPDATE START
				break;
			case 1 :
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu, m_pSpcListData->stval.SpKsyu );
//				m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu );
//-- '15.04.25 --
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu4, m_pSpcListData->stval.SpKsyu4 );
//				m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu4 );
//---------------
				memmove( WORK0, m_pSpcListData->stval.SpTsyu4, sizeof(WORK0) );
//---------------
//2015.03.05 UPDATE END
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 2 :
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				memmove( WORK0, m_pSpcListData->stval.SpGszg, sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg4, sizeof(WORK0) );
//2015.03.05 UPDATE END
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 3 :	
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				memmove( WORK0, m_pSpcListData->stval.SpGszg, sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg4, sizeof(WORK0) );
//2015.03.05 UPDATE END
				memset( WORK1, '\0', sizeof(WORK1) );
//2015.03.05 UPDATE START
//				m_Arith.l_sub( WORK1, m_pSpcListData->stval.SpKgzg, WORK0 );
				m_Arith.l_sub( WORK1, m_pSpcListData->stval.SpKgz4, WORK0 );
//2015.03.05 UPDATE END
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK1, FMTX12 );
				break;
			case 5 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 6 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGgtg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGgt4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 7 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSizg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			default:
				break;
		}
		if( i == 4 ){
//2015.03.05 UPDATE START
//			m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
//-- '15.04.25 --
//			m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu4, FMTX12 );
//---------------
			m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
//---------------
//2015.03.05 UPDATE END
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
				return TRUE;
			}

			memset( TBUF, '\0', sizeof(TBUF) );
			memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//			m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//-- '15.04.25 --
//			m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu4 );
//---------------
			m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//---------------
//2015.03.05 UPDATE END
			m_Arith.l_print( TBUF, WORK0, FMTX12 );
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine  - PV1 + (PV1 / 2L), TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}
		else	{
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	
	return FALSE;
}

//2015.03.05 INSERT START

//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表(８％)	売上割合９５％以上
//-----------------------------------------------------------------------------
// 引数	sign	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList50Ex( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(１)　調整後税額の計算表（課税売上割合が95%以上の場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 27L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=8; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );
	}
	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 24L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 27L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ使途が特定されている特定収入") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入") );

	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ使途が特定されている特定収入") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)のＢ）") );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＢ（８％））") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｃ）") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＢ）") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｃ）") );
	}
//2017.03.01 UPDATE END
//---------------
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　　４") );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　６．３") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 17L),							// 開始列　座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 22L),							// 最終列  座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 14L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 19L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------


	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－③") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表４④）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){	
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){	
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("④×⑤　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 20L), TBUF, &m_SfPar ) ){	
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 20L), TBUF, &m_SfPar ) ){	
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（③＋⑥）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑦）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE END

	// 金額
	char	WORK0[6], WORK1[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=8; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgzg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz63, FMTX12 );
				break;
			case 1 :
				memset( WORK0, '\0', sizeof(WORK0) );
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu, m_pSpcListData->stval.SpKsyu );
//				m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu );
//-- '15.04.25 --
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTsyu63, m_pSpcListData->stval.SpKsyu63 );
//				m_Arith.l_add( WORK0, WORK0, m_pSpcListData->stval.SpHsyu63 );
//---------------
				memmove( WORK0, m_pSpcListData->stval.SpTsyu63, sizeof(WORK0) );
//---------------
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 2 :
				memset( WORK0, '\0', sizeof(WORK0) );
//				memmove( WORK0, m_pSpcListData->stval.SpGszg, sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg63, sizeof(WORK0) );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 3 :	
				memset( WORK0, '\0', sizeof(WORK0) );
//				memmove( WORK0, m_pSpcListData->stval.SpGszg, sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg63, sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
//				m_Arith.l_sub( WORK1, m_pSpcListData->stval.SpKgzg, WORK0 );
				m_Arith.l_sub( WORK1, m_pSpcListData->stval.SpKgz63, WORK0 );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK1, FMTX12 );
				break;
			case 5 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag63, FMTX12 );
				break;
			case 6 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGgtg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGgt63, FMTX12 );
				break;
			case 7 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSizg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz63, FMTX12 );
				break;
			default:
				break;
		}
		if( i == 4 ){
//			m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
//-- '15.04.25 --
//			m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu63, FMTX12 );
//---------------
			m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
//---------------
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
				return TRUE;
			}

			memset( TBUF, '\0', sizeof(TBUF) );
			memset( WORK0, '\0', sizeof(WORK0) );
//			m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//-- '15.04.25 --
//			m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu63 );
//---------------
			m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//---------------
			m_Arith.l_print( TBUF, WORK0, FMTX12 );
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine  - PV1 + (PV1 / 2L), TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}
		else	{
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	
	return FALSE;
}
//2015.03.05 INSERT END

//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表	売上割合９５％未満　個別対応 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList51( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(２)　調整後税額の計算表（課税売上割合が95%未満で個別対応方式を採用している場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 70L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 42L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=13; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ) {
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪");	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫");	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}
		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 39L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 42L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等にのみ使途が") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等(税率4％)にのみ") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("特定されている特定収入") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定されている特定収入") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)のＥ）") );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(5%)⑰Ｅ）") );
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), "　　　　　　４" );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 13L),							// 開始列　座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 18L),							// 最終列  座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等に") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等(税率4％)に") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 13L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 13L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されている特定収入") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されている特定収入") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)のＦ）") );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(5%)⑰Ｆ）") );
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 15L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 15L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　　４") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("④×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 13L),							// 開始列　座標
//			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 18L),							// 最終列  座標
//			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標
//---------------

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上割合（準ずる割合の承認を受けている場合はその割合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 19L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 19L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑤×⑥　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③＋⑦") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－⑧") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 29L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 29L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 32L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 32L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表４④）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 33L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 33L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 31L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 31L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑨×⑩　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 35L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 35L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（⑧＋⑪）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 38L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 38L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑫）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 41L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 41L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE END

	// 金額
	char	WORK0[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=13; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgzg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 1 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu, FMTX12 );
//-- '15.04.25 --
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu4, FMTX12 );
//---------------
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSsyu4, FMTX12 );
//---------------
//2015.03.05 UPDATE END
				break;
			case 2 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 3 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsyu, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsyu4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 4 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 5 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuws, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuwb, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 6 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKszg4, FMTX12 );
//2015.03.05 UPDATE START
				break;
			case 7 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGszg4, FMTX12 );
//2015.03.05 UPDATE START
				break;
			case 8 :
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgzg, m_pSpcListData->stval.SpGszg );
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz4, m_pSpcListData->stval.SpGszg4 );
//2015.03.05 UPDATE START
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 9 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				memset( TBUF, '\0', sizeof(TBUF) );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 10:	
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 11:
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpGszg, m_pSpcListData->stval.SpTwag );	
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpGszg4, m_pSpcListData->stval.SpTwag4 );	
//2015.03.05 UPDATE END
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 12:	
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSizg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			default:
				break;
		}
		if( (i!=5) && (i!=9) ){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	

	return FALSE;
}

//2015.03.05 INSERT START
//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表	売上割合９５％未満　個別対応 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList51Ex( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(２)　調整後税額の計算表（課税売上割合が95%未満で個別対応方式を採用している場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 70L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 42L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=13; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ) {
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪");	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫");	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}
		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 39L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 42L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等にのみ使途が") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等(税率6.3％)にのみ") );

		if( IsKeikaType_Keisansyo() == TRUE ){
			strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等にのみ") );
		}else{
			strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等(税率6.3％)にのみ") );
		}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("特定されている特定収入") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定されている特定収入") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)のＥ）") );
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(8%)⑰Ｅ）") );

	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)⑰のＥ）") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(8%)⑰Ｅ）") );
	}
//2017.03.01 UPDATE END
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), "　　　　　６．３" );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 13L),							// 開始列　座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 18L),							// 最終列  座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等に") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等(税率6.3％)に") );

	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等に") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等(税率6.3％)に") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 13L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 13L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されている特定収入") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されている特定収入") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)のＦ）") );
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(8%)⑰Ｆ）") );

	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)⑰のＦ）") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(8%)⑰Ｆ）") );
	}
//2017.03.01 UPDATE END
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 15L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 15L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　６．３") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("④×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 13L),							// 開始列　座標
//			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 18L),							// 最終列  座標
//			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標
//---------------

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上割合（準ずる割合の承認を受けている場合はその割合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 19L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 19L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑤×⑥　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③＋⑦") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－⑧") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 29L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 29L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 32L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 32L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表４④）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 33L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 33L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 31L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 31L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑨×⑩　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 35L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 35L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（⑧＋⑪）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 38L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 38L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑫）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 6L ), m_YPosition + ( PV1 * 41L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 41L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE END

	// 金額
	char	WORK0[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=13; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgzg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz63, FMTX12 );
				break;
			case 1 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu, FMTX12 );
//-- '15.04.25 --
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu63, FMTX12 );
//---------------
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSsyu63, FMTX12 );
//---------------
				break;
			case 2 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg63, FMTX12 );
				break;
			case 3 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsyu, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsyu63, FMTX12 );
				break;
			case 4 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg63, FMTX12 );
				break;
			case 5 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuws, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuwb, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 6 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKszg63, FMTX12 );
				break;
			case 7 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGszg63, FMTX12 );
				break;
			case 8 :
				memset( WORK0, '\0', sizeof(WORK0) );
//				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgzg, m_pSpcListData->stval.SpGszg );
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz63, m_pSpcListData->stval.SpGszg63 );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 9 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
//-- '15.04.25 --
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu63, FMTX12 );
//---------------
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
//---------------
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				memset( TBUF, '\0', sizeof(TBUF) );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu63 );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 10:	
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag63, FMTX12 );
				break;
			case 11:
				memset( WORK0, '\0', sizeof(WORK0) );
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpGszg, m_pSpcListData->stval.SpTwag );	
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpGszg63, m_pSpcListData->stval.SpTwag63 );	
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 12:	
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSizg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz63, FMTX12 );
				break;
			default:
				break;
		}
		if( (i!=5) && (i!=9) ){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	

	return FALSE;
}
//2015.03.05 INSERT END

//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表	売上割合９５％未満　比例配分 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList52( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(３)　調整後税額の計算表（課税売上割合が95%未満で一括比例配分方式を採用している場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 70L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn);
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 33L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=10; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 30L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 33L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ使途が特定されている特定収入") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率4％)にのみ使途が特定されている特定収入") );
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPADATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)のＢ）") );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＢ（5%））") );
//---------------
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｂ）") );
//---------------
//2015.03.05 UPADATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　　４") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 17L),							// 開始列　座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 22),							// 最終列  座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 14L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 19L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 13L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 13L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③×④　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 17L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 17L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－⑤") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 23L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 23L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), "（計算表４④）" );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑥×⑦　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（⑤＋⑧）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 29L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 29L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑨）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 32L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 32L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE END

	// 金額
	char	WORK0[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=10; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgzg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 1 :	
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu4, FMTX12 );
//2015.03.05 UPDATE START
				break;
			case 2 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg4, FMTX12 );
//2015.03.05 UPDATE START
				break;
			case 3 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuws, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuwb, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 4 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 5 :
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgzg, m_pSpcListData->stval.SpTszg );
//-- '15.05.12 --
//				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgzg, m_pSpcListData->stval.SpTszg4 );
//---------------
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz4, m_pSpcListData->stval.SpTszg4 );
//---------------
//2015.03.05 UPDATE END
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 6 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				memset( TBUF, '\0', sizeof(TBUF) );
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//-- '15.05.15 --
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu4 );
//---------------
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//---------------
//2015.03.05 UPDATE END
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 7 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			case 8 :	
				memset( WORK0, '\0', sizeof(WORK0) );
//2015.03.05 UPDATE START
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTszg, m_pSpcListData->stval.SpTwag );	
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTszg4, m_pSpcListData->stval.SpTwag4 );	
//2015.03.05 UPDATE END
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 9 :
//2015.03.05 UPDATE START
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSizg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz4, FMTX12 );
//2015.03.05 UPDATE END
				break;
			default:
				break;
		}
		if( (i!=3) && (i!=6) ){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
			m_SfPar.sf_hvsg = 0;									// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;									// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	

	return FALSE;
}

//2015.03.05 INSERT START
//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表	売上割合９５％未満　比例配分 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList52Ex( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(３)　調整後税額の計算表（課税売上割合が95%未満で一括比例配分方式を採用している場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 70L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn);
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 33L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=10; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 30L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 33L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ使途が特定されている特定収入") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入") );
	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ使途が特定されている特定収入") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入") );
	}
//2017.03.01 UPDATE END
//---------------
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2015.03.05 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)のＢ）") );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＢ（8%））") );
//---------------
//2017.03.01 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｃ）") );

	if( IsKeikaType_Keisansyo() == TRUE ){
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰のＢ）") );
	}else{
		strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｃ）") );
	}
//2017.03.01 UPDATE END
//---------------
//2015.03.05 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　６．３") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
//-- '15.04.29 --
//			m_XPosition + (PH  * 17L),							// 開始列　座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
//			m_XPosition + (PH  * 22),							// 最終列  座標
//			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------
			m_XPosition + (PH  * 14L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 19L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標
//---------------

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 13L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 13L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③×④　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 17L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 17L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－⑤") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 23L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 23L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), "（計算表４④）" );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 30L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑥×⑦　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（⑤＋⑧）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 29L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 29L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑨）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
//-- '15.04.29 --
//	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + ( PV1 * 32L), TBUF, &m_SfPar ) ){
//---------------
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 32L), TBUF, &m_SfPar ) ){
//---------------
		return TRUE;
	}

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE START

	// 金額
	char	WORK0[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=10; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgzg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz63, FMTX12 );
				break;
			case 1 :	
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu63, FMTX12 );
				break;
			case 2 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg63, FMTX12 );
				break;
			case 3 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuws, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuwb, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 4 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg63, FMTX12 );
				break;
			case 5 :
				memset( WORK0, '\0', sizeof(WORK0) );
//				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgzg, m_pSpcListData->stval.SpTszg );
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz63, m_pSpcListData->stval.SpTszg63 );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 6 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu63, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				memset( TBUF, '\0', sizeof(TBUF) );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu63 );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 7 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag63, FMTX12 );
				break;
			case 8 :	
				memset( WORK0, '\0', sizeof(WORK0) );
//				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTszg, m_pSpcListData->stval.SpTwag );	
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTszg63, m_pSpcListData->stval.SpTwag63 );	
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 9 :
//				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSizg, FMTX12 );
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz63, FMTX12 );
				break;
			default:
				break;
		}
		if( (i!=3) && (i!=6) ){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
			m_SfPar.sf_hvsg = 0;									// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;									// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	

	return FALSE;
}
//2015.03.05 INSERT END
//-----------------------------------------------------------------------------
// 計算表-６　経過措置対象課税仕入れがある場合の特定収入に係る課税仕入れ等の税額の配賦計算表 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList60( int sign )
{
	// タイトル
	char	TBUF[256];
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
//2015.03.05 UPDATE START
//		sprintf_s( TBUF, sizeof(TBUF), _T("           %-40.40s %s 課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
//2015.03.05 UPDATE END
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表６　経過措置対象課税仕入れがある場合の特定収入に係る課税仕入れ等の税額の配賦計算表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 70L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 51L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=16; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪");	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫");	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬");	break;
			case 13:	strcpy_s( TBUF, sizeof( TBUF ), "⑭");	break;
			case 14:	strcpy_s( TBUF, sizeof( TBUF ), "⑮");	break;
			case 15:	strcpy_s( TBUF, sizeof( TBUF ), "⑯");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 48L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 51L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　額　　等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の新税率適用分の課税仕入れ等の税額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の旧税率適用分の課税仕入れ等の税額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計　①＋②") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る税額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表５（１）③）又は（計算表５（２）⑧）又は（計算表５（３）⑤）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 15L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表５（１）⑥）又は（計算表５（２）⑪）又は（計算表５（３）⑧）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等の金額（税込み）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等にのみ使途が特定されている特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 23L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２（１）Ｂ）又は（計算表２（２）Ｅ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等に係る対価の返還等の金額（税込み）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 26L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("新税率適用分の課税仕入れ等に係る金額（税込み）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 29L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("新税率適用分の課税仕入れ等に係る対価の返還等の金額（税込み）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 32L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　⑨－⑦－⑩") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 9L ), m_YPosition + ( PV1 * 34L ) + ( PV1 / 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("配賦割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 35L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　⑥－⑦－⑧") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 9L ), m_YPosition + ( PV1 * 36L ) - ( PV1 / 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 34L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 25),							// 最終列  座標
			m_YPosition + (PV1 * 34L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 54L),							// 開始列　座標
			m_YPosition + (PV1 * 34L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 69L),							// 最終列  座標
			m_YPosition + (PV1 * 34L) + (PV1 / 2)  );			// 最終行　座標

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("新税率適用分における配賦後の調整額　④＋（⑤×⑪）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 38L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("新税率適用分の控除対象仕入税額　①－⑫") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 41L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("旧税率適用分における配賦後の調整額　（④＋⑤）－⑫") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 44L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("旧税率適用分の控除対象仕入税額　②－⑭") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 47L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整後の控除対象仕入税額合計　⑬＋⑮") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 50L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

//2015.03.05 DELETE START
	//if( !sign ){
	//	// 調整計算不要
	//	return FALSE;
	//}
//2015.03.05 DELETE END

	// 金額
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=16; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz4, FMTX12 );
				break;
			case 1 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz3, FMTX12 );
				break;
			case 2 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgzg, FMTX12 );
				break;
			case 3 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGszg, FMTX12 );
				break;
			case 4 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag, FMTX12 );
				break;
			case 5 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsig, FMTX12 );
				break;
			case 6 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu, FMTX12 );
				break;
			case 7 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsbg, FMTX12 );
				break;
			case 8 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsi4, FMTX12 );
				break;
			case 9 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsb4, FMTX12 );
				break;
			case 10 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpHfgs, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpHfgb,FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 11:	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGgt4, FMTX12 );
				break;
			case 12:	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz4, FMTX12 );
				break;
			case 13:	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsh3, FMTX12 );
				break;
			case 14:	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz3, FMTX12 );
				break;
			case 15:	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSizg, FMTX12 );
				break;
			default:
				break;
		}
		if( i != 10 ){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	
	return FALSE;
}

//2017.03.01 INSERT START
//-----------------------------------------------------------------------------
// 計算表-１　資産の譲渡等の対価の額の計算（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList10Ex( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = (SFp_xc|SFp_yc|SFscut|SFcitv);	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));				// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_XPosition = ( PH *  30L );
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	if( m_pVprn->DrawString(0, m_XPosition, m_YPosition, TBUF, &m_SfPar) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 計算表１　資産の譲渡等の対価の額の計算書
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表１　資産の譲渡等の対価の額の計算表") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,							// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
		(	m_XPosition +								// 最終列  座標
			( 70L * PH ) ),								// 外枠拡張
			m_YPosition + (PV1 * 21L)  );				// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=6; i++ ){
		if( i != 1 ) 
			m_pVprn->DrawWframe( 0,
					LKnrW1,								// 線種
					m_XPosition,						// 開始列　座標
					m_YPosition + m_StartLine, 			// 開始行　座標
				(	m_XPosition +						// 最終列  座標
					( 70L * PH ) ),						// 外枠拡張
					m_YPosition + m_StartLine  );		// 最終行　座標
		else
			m_pVprn->DrawWframe( 0,
					LKnrW1,								// 線種
					m_XPosition + ( PH * 3L ),			// 開始列　座標
						m_YPosition + m_StartLine, 		// 開始行　座標
					(	m_XPosition +					// 最終列  座標
					( 70L * PH ) ),						// 外枠拡張
					m_YPosition + m_StartLine  );		// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	
				strcpy_s( TBUF, sizeof( TBUF ), "①");
				break;
			case 1 :
				strcpy_s( TBUF, sizeof( TBUF ), "②");
				break;
			case 2 :
				strcpy_s( TBUF, sizeof( TBUF ), "③");
				break;
			case 3 :
				strcpy_s( TBUF, sizeof( TBUF ), "④");
				break;
			case 4 :
				strcpy_s( TBUF, sizeof( TBUF ), "⑤");
				break;
			case 5 :
				strcpy_s( TBUF, sizeof( TBUF ), "⑥");
				break;
			default:
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;							// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );					// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;						// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;							// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;							// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;							// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;							// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;							// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;					// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;							// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}
		m_StartLine += ( PV1 * 3L );
	}
	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH *  8L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 6 ) );				// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 55L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 18 ) );				// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 58L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 21 ) );				// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("金　　　　額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("円") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 1;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld = PV1;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH - 10L, m_YPosition + (PV1 * 6L)-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
//-- '15.04.29 --
//	strcpy_s( TBUF, sizeof( TBUF ), _T("通常の課税売上げ・役員へ贈与及び低額譲渡") );
//---------------
	strcpy_s( TBUF, sizeof( TBUF ), _T("通常の課税売上げ・役員への贈与及び低額譲渡") );
//---------------
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税標準額に対する消費税額の計算の特例適用の課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("免税売上げ（輸出取引等）") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("非課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("国外における資産の譲渡等の対価の額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("資産の譲渡等の対価の額の合計額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 7L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表３①、計算表４①へ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 53L ), m_YPosition + ( PV1 * 19L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 金額
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=6; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ) {
			case 0 :	
				if( (m_pSnHeadData->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
					;
				}
				else{
					m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKzur, FMTX12 );
				}
				break;
			case 1 :
				if( (m_pSnHeadData->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
					m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKzur, FMTX12 );
				}
				break;
			case 2 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpMzur, FMTX12 );
				break;
			case 3 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpHkur, FMTX12 );
				break;
			case 4:
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgjt, FMTX12 );
				break;
			case 5:
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTuwb, FMTX12 );
				break;
			default:
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc )					// ファンクションサイン
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );

	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表-１　資産の譲渡等の対価の額の計算（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList10ExKeika( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	char	TBUF2[256]={0};
	char	TBUF3[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = (SFp_xc|SFp_yc|SFscut|SFcitv);	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));				// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_XPosition = ( PH *  30L );
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	if( m_pVprn->DrawString(0, m_XPosition, m_YPosition, TBUF, &m_SfPar) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 計算表１　資産の譲渡等の対価の額の計算書
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表１　資産の譲渡等の対価の額の計算表") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,							// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
		(	m_XPosition +								// 最終列  座標
			( 70L * PH ) ),								// 外枠拡張
			m_YPosition + (PV1 * 21L)  );				// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=6; i++ ){
		if( i != 1 ) 
			m_pVprn->DrawWframe( 0,
					LKnrW1,								// 線種
					m_XPosition,						// 開始列　座標
					m_YPosition + m_StartLine, 			// 開始行　座標
				(	m_XPosition +						// 最終列  座標
					( 70L * PH ) ),						// 外枠拡張
					m_YPosition + m_StartLine  );		// 最終行　座標
		else
			m_pVprn->DrawWframe( 0,
					LKnrW1,								// 線種
					m_XPosition + ( PH * 3L ),			// 開始列　座標
						m_YPosition + m_StartLine, 		// 開始行　座標
					(	m_XPosition +					// 最終列  座標
					( 70L * PH ) ),						// 外枠拡張
					m_YPosition + m_StartLine  );		// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	
				strcpy_s( TBUF, sizeof( TBUF ), "①");
				break;
			case 1 :
				strcpy_s( TBUF, sizeof( TBUF ), "②");
				break;
			case 2 :
				strcpy_s( TBUF, sizeof( TBUF ), "③");
				break;
			case 3 :
				strcpy_s( TBUF, sizeof( TBUF ), "④");
				break;
			case 4 :
				strcpy_s( TBUF, sizeof( TBUF ), "⑤");
				break;
			case 5 :
				strcpy_s( TBUF, sizeof( TBUF ), "⑥");
				break;
			default:
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;							// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );					// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;						// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;							// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;							// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;							// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;							// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;							// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;					// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;							// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 28L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}
		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH *  8L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 6 ) );				// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 33L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 18 ) );				// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 36L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 21 ) );				// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 21 ) );				// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,										// 線種
			m_XPosition,								// 開始列　座標
			m_YPosition, 								// 開始行　座標
			m_XPosition,	 							// 最終列  座標
			m_YPosition + ( PV1 * 21 ) );				// 最終行　座標

	// 斜線
	for(int cnt1=0; cnt1<2; cnt1++){
		m_YPosition = ( PV1 * HeadLn )+(( PV1*3L )*3 );
		m_XPosition = ( PH * 36L ) + (( PH * 13L )*cnt1);
		for(int cnt2=0; cnt2<4; cnt2++ ){
			m_YPosition += (PV1 * 3L);
			m_pVprn->DrawWframe( 0,
					LKnrW1,								// 線種
					m_XPosition,						// 開始列　座標
					m_YPosition, 						// 開始行　座標
					m_XPosition + (PH * 13L),			// 最終列  座標
					m_YPosition - (PV1 * 3L));			// 最終行　座標
		}
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 25L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 3L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率４％適用分") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 31L ), m_YPosition + ( PV1 * 2L )-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 31L ), m_YPosition + ( PV1 * 2L )+(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率6.3％適用分") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 2L )-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 2L )+(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("合　　　　計") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 2L )-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｃ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 2L )+(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("円") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 1;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld = PV1;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH - 10L, m_YPosition + (PV1 * 6L)-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("通常の課税売上げ・") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + (PV1 * 5L)-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof( TBUF ), _T("役員への贈与及び低額譲渡") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + (PV1 * 5L)+(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税標準額に対する消費税額の") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + (PV1 * 8L)-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算の特例適用の課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + (PV1 * 8L)+(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("免税売上げ（輸出取引等）") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("非課税売上げ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + ( PV1 * 14L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("国外における資産の譲渡等の") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + ( PV1 * 17L )-(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("対価の額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + ( PV1 * 17L )+(PV1/2L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("資産の譲渡等の対価の額の合計額") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表３①、計算表４①へ") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;								// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );						// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;							// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;								// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;								// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;								// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;								// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;								// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT072z;						// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;								// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 19L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 金額
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=6; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		memset( TBUF2, '\0', sizeof(TBUF) );
		memset( TBUF3, '\0', sizeof(TBUF) );
		switch( i ) {
			case 0 :	
				if( (m_pSnHeadData->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
					;
				}
				else{
					m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKzur, FMTX12 );
					m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpKzur4, FMTX12 );
					m_Arith.l_print( TBUF3, m_pSpcListData->stval.SpKzur63, FMTX12 );
				}
				break;
			case 1 :
				if( (m_pSnHeadData->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
					m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKzur, FMTX12 );
					m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpKzur4, FMTX12 );
					m_Arith.l_print( TBUF3, m_pSpcListData->stval.SpKzur63, FMTX12 );
				}
				break;
			case 2 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpMzur, FMTX12 );
				break;
			case 3 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpHkur, FMTX12 );
				break;
			case 4:
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgjt, FMTX12 );
				break;
			case 5:
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTuwb, FMTX12 );
				break;
			default:
				break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
//		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc )					// ファンクションサイン
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 52L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 54L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}
		if((i==0)||(i==1)){
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 28L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF2, &m_SfPar ) ){
				return TRUE;
			}
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 41L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF3, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );

	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表２（1）　特定収入金額合計（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//		page	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList21Ex2( int sign, int page )
{
	if( GetMxList21Ex() == 0 ){
		return TRUE;
	}

	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_XPosition = ( PH *  30L );
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}


	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表２（１）　特定収入の金額及びその内訳書") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_pSpcListData->TsTbl.tp_rnum > 17 ){
		sprintf_s( TBUF, sizeof(TBUF), _T("ﾍﾟｰｼﾞ番号 %3d"), page );
	}
	else{
		strcpy_s( TBUF, sizeof(TBUF), _T(" ") );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  60L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標
	
	m_XPosition = ( PH *  5L );
	m_YPosition = ( PV1 * (HeadLn + 9L) );
	for( int ln = 0; ln != 17; ln++ ) {
		// 横線
		m_pVprn->DrawWframe( 0,
				LKnrW1,											// 線種
				m_XPosition,									// 開始列　座標
				m_YPosition, 									// 開始行　座標
			(	m_XPosition +									// 最終列  座標
				( 70L * PH ) ),									// 外枠拡張
				m_YPosition  );									// 最終行　座標
		// 番号
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( ln ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①" );	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②" );	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③" );	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④" );	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤" );	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥" );	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦" );	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧" );	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨" );	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩" );	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪" );	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫" );	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬" );	break;
			case 13:	strcpy_s( TBUF, sizeof( TBUF ), "⑭" );	break;
			case 14:	strcpy_s( TBUF, sizeof( TBUF ), "⑮" );	break;
			case 15:	strcpy_s( TBUF, sizeof( TBUF ), "⑯" );	break;
			case 16:	strcpy_s( TBUF, sizeof( TBUF ), "⑰" );	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 4L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 14L ) + ( PH / 2 ), m_YPosition + PV2, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition += (PV3);
	}

	//タイトル行 横線
	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_XPosition = ( PH *  49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH * 26L ),							// 最終列  座標
			m_YPosition);										// 最終行　座標

	// 縦線
	m_YPosition = ( PV1 * ( HeadLn + 9L ) );
	m_XPosition = ( PH *  20L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 32L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  23L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  36L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition+PV1, 									// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition+PV1,									// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 41L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof( TBUF ) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内      容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 18L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 17L );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("合   　　   計") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 15L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  23L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("資産の譲渡等の") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  23L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("対価以外の収入") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  36L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("左のうち") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  36L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  36L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 17L );
	m_XPosition = ( PH *  36L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表３②へ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT066z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_XPosition = ( PH *  49L );
	m_YPosition = ( PV1 * HeadLn );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａのうち課税仕入") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("れ等にのみ使途が") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定されている金") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("額(「課税仕入れ等") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("に係る特定収入」)") );
	memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_XPosition = ( PH *  49L );

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｂ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表５(1)②,(3)②へ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT066z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 17L ), TBUF, &m_SfPar ) ){	
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  62L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ａ－Ｂの金額(「課") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税仕入れ等に係る") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入以外の特") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("定収入」)") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR ) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｃ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH / 2 ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" 計算表４②へ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT066z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ	
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ) + ( PV3 * 17L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_XPosition = ( PH *  23L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_XPosition = ( PH *  36L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_XPosition = ( PH *  49L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn ) + PV1;
	m_XPosition = ( PH *  62L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 最終ページ合計印刷チェック
	//資産の譲渡等の対価以外の収入　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  23L ) - ( PH/2 );

	TSREC	TSrec;
	char	WORK0[6]={0};
	int rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}
		// 資産の譲渡等の対価以外の収入合計
		m_Arith.l_add( WORK0, WORK0, TSrec.TsJsyu );
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_print( TBUF, WORK0, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//左のうち特定収入　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  36L ) - ( PH/2 );


	memset( WORK0, '\0', sizeof(WORK0) );
	char	WORK1[6]={0};
	char	WORK_sum[6]={0};
	rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}
		
		//m_Arith.l_add( WORK0, WORK0, TSrec.TsJsyu );
		m_Arith.l_add( WORK1, WORK1, TSrec.TsDtl[1].TsGsyu ); //８％
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsGsyu ); //５％
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_add( WORK_sum, WORK0, WORK1 );
		m_Arith.l_print( TBUF, WORK_sum, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//Ａのうち課税仕入れ等にのみ使途が特定されている金額(「課税仕入れ等に係る特定収入」)　８％　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  49L ) - ( PH/2 );

	memset( WORK0, '\0', sizeof(WORK0) );
	rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}

		// 課税仕入等にのみ係る特定収入
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[1].TsTsyu );
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_print( TBUF, WORK0, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//Ａ－Ｂの金額(「課税仕入れ等に係る特定収入以外の特定収入」)　合計
	m_YPosition = ( PV1 * HeadLn ) + ( PV3 * 16L );
	m_XPosition = ( PH *  62 ) - ( PH/2 );

	memset( WORK0, '\0', sizeof(WORK0) );
	memset( WORK1, '\0', sizeof(WORK1) );
	memset( WORK_sum, '\0', sizeof(WORK_sum) );
	rno = 0;
	while( 1 ){
		if( rno == m_pSpcListData->TsTbl.tp_rnum ){
			break;
		}
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
			break;
		}
		
		m_Arith.l_add( WORK1, WORK1, TSrec.TsDtl[1].TsFsyu ); //８％
		m_Arith.l_add( WORK0, WORK0, TSrec.TsDtl[0].TsFsyu ); //５％
		rno++;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( page < GetMxList21Ex()){
		sprintf_s( TBUF, sizeof( TBUF ), " " );
	}
	else{
		m_Arith.l_add( WORK_sum, WORK0, WORK1 );
		m_Arith.l_print( TBUF, WORK_sum, FMTX12 );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	int		Rno, Cnt;
	long	lineCount;

	Rno = ( (page-1) * 16 );
	for( Cnt = 0; Cnt != 16; Cnt++, Rno++ ){
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, Rno ) ){
			return FALSE;
		}
		// 行ポイント
		lineCount = (PV3 * Cnt);
		// 名称
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  5L );
		strcpy_s( TBUF, sizeof(TBUF), TSrec.TsName );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 15L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		//資産の譲渡等の対価以外の収入
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  23L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, TSrec.TsJsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 13L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		char sum[6] = {0};
		//左のうち特定収入
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  36L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );
		m_Arith.l_print( TBUF, sum, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 13L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		//Ａのうち課税仕入れ等にのみ使途が特定されている金額(「課税仕入れ等に係る特定収入」)　８％
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  49L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[1].TsTsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 13L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		memset( sum, '\0', sizeof(sum));
		//Ａ－Ｂの金額(「課税仕入れ等に係る特定収入以外の特定収入」)
		m_YPosition = ( PV1 * HeadLn ) + lineCount;
		m_XPosition = ( PH *  62L ) - ( PH/2 );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_add( sum, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
		m_Arith.l_print( TBUF, sum, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 13L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表２（２）　特定収入金額合計（個別対応方式）	（印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：	
//		page	：	
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList22Keika( int sign, int page, int zeiType )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 計算表２（２）　特定収入金額合計（個別対応方式）
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表２（２）　特定収入の金額及びその内訳書（個別対応方式用）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_pSpcListData->TsTbl.tp_rnum > 17 ){
		sprintf_s( TBUF, sizeof(TBUF), _T("ﾍﾟｰｼﾞ番号 %3d"), page );
	}
	else{
		strcpy_s( TBUF, sizeof(TBUF), _T(" ") );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  60L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 40L) + (PV2*2) );				// 最終行　座標
	
	m_YPosition = ( PV1 * (HeadLn + 1L) );
	m_XPosition = ( PH *  27L )  + ( PH/2 );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 19L * PH ) ),										// 外枠拡張
			m_YPosition  );										// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 1L) );
	m_XPosition = ( PH *  56L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 19L * PH ) ),										// 外枠拡張
			m_YPosition  );										// 最終行　座標

	m_XPosition = ( PH *  5L );
	m_YPosition = ( PV1 * (HeadLn + 9L) );
	for( int ln = 0; ln != 17; ln++ ) {
		// 横線
		m_pVprn->DrawWframe( 0,
				LKnrW1,											// 線種
				m_XPosition,									// 開始列　座標
				m_YPosition, 									// 開始行　座標
			(	m_XPosition +									// 最終列  座標
				( 70L * PH ) ),									// 外枠拡張
				m_YPosition  );									// 最終行　座標

		// 番号
		memset( TBUF, '\0', sizeof(TBUF) );
		switch( ln ) {
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①" );	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②" );	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③" );	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④" );	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤" );	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥" );	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦" );	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧" );	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨" );	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩" );	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪" );	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫" );	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬" );	break;
			case 13:	strcpy_s( TBUF, sizeof( TBUF ), "⑭" );	break;
			case 14:	strcpy_s( TBUF, sizeof( TBUF ), "⑮" );	break;
			case 15:	strcpy_s( TBUF, sizeof( TBUF ), "⑯" );	break;
			case 16:	strcpy_s( TBUF, sizeof( TBUF ), "⑰" );	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
//		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 15L ) + ( PH / 2 ), m_YPosition + PV2, TBUF, &m_SfPar ) ){
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 10L ), m_YPosition + PV2, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition += (PV3);
	}

	// 縦線
	m_YPosition = ( PV1 * ( HeadLn + 9L ) );
	m_XPosition = ( PH *  15L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 32L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  18L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 40L) + PV3 + ( PV3 / 2 ) );	// 最終行　座標

	m_YPosition = ( PV1 * HeadLn + 1L) + PV1;
	m_XPosition = ( PH *  27L ) + ( PH/2 );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 40L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 1L) );
	m_XPosition = ( PH *  37L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 40L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn) );
	m_XPosition = ( PH *  46L ) + ( PH/2 );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 40L) + PV3 + ( PV3 / 2 ) );	// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 1L) );
	m_XPosition = ( PH *  56L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 40L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 1L) );
	m_XPosition = ( PH *  65L )  + ( PH/2 );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + (PV1 * 40L) + PV3 );					// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内      容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * (HeadLn + 1L) ) + ( PV3 * 16L );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("合   　　   計") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  18L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（税率4％）に") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("係る特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("(計算表２(1)Ｂ)") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  27L ) + ( PH/2 );

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("うち課税売上") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("げにのみ要す") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("る課税仕入れ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("等にのみ使途") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("が特定されて") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("いる特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｅ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(2)②へ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT072z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH/2, m_YPosition + ( PV1 * 10L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  37L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("うち課税・非") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げに") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("共通して要す") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("る課税仕入れ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("等にのみ使途") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("が特定されて") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("いる特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｆ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(2)④へ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT072z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH/2, m_YPosition + ( PV1 * 10L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  46L ) + ( PH/2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（税率6.3％）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("に係る特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("(計算表２(1)Ｃ)") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  56L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("うち課税売上") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("げにのみ要す") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("る課税仕入れ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("等にのみ使途") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("が特定されて") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("いる特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｇ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(2)⑥へ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT072z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH/2, m_YPosition + ( PV1 * 10L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  65L ) + ( PH/2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("うち課税・非") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げに") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 3L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("共通して要す") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 4L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("る課税仕入れ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("等にのみ使途") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 6L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}
	
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("が特定されて") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 7L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("いる特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("Ｈ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(2)⑧へ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT072z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + PH/2, m_YPosition + ( PV1 * 10L ) + ( PV3 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
	m_XPosition = ( PH *  18L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
	m_XPosition = ( PH *  27L ) + ( PH/2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
	m_XPosition = ( PH *  37L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
	m_XPosition = ( PH *  46L )  + ( PH/2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
	m_XPosition = ( PH *  56L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * ( HeadLn + 1L ) ) + PV1;
	m_XPosition = ( PH *  65L )  + ( PH/2 );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );					// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 4 ), m_YPosition + ( PV1 * 8L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	TSREC	TSrec;
	int		Rno, Cnt;
	int		lineCount;
	BOOL	last_page = FALSE;
	char	MONY1[6]={0}, MONY2[6]={0}, MONY3[6]={0},MONY4[6]={0}, MONY5[6]={0}, MONY6[6]={0};

	//int		zrIdx=0;
	//if( zeiType == ID_ICSSH_SPC_5PER ){	// 税率5% の帳票
	//	zrIdx=0;
	//}
	//else if( zeiType == ID_ICSSH_SPC_8PER ){		// 税率8% の帳票
	//	zrIdx=1;
	//}else{
	//	return TRUE;
	//}

	//if( zeiType == ID_ICSSH_SPC_5PER ){	// 税率5% の帳票
	//	if( page == GetMxList22_5per() ){ // 最終ページ判定
	//		last_page = TRUE;
	//	}			
	//}
	//else if( zeiType == ID_ICSSH_SPC_8PER ){ // 税率8% の帳票
	//	if( page == GetMxList22_8per() ){ // 最終ページ判定
	//		last_page = TRUE;
	//	}			
	//}else{
	//	return TRUE;
	//}

	if( page == GetMxList22_keika() ){ // 最終ページ判定
		last_page = TRUE;
	}			


	if( m_PageManage_22keika.IsEmpty() == TRUE ){
		return FALSE;
	}
	Rno = m_PageManage_22keika.GetAt( page - 1 );

	for( Cnt=0; Cnt!=LINES_PER_PAGE_22; Rno++ ){
		if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, Rno ) ){
			if( last_page == TRUE ){
				break;
			}else{
				return FALSE;
			}
		}

		if( m_pShinSpecific22keika ){
			if( m_pShinSpecific22keika->IsNowZrtData( &TSrec ) == FALSE ){
				continue;
			}
		}else{
			return FALSE;
		}

		// 行ポイント
		lineCount = (PV3 * Cnt);
		// 名称
		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  5L );
		strcpy_s( TBUF, sizeof(TBUF), TSrec.TsName );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 10L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT090z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  17L )  + ( PH/2 );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsTsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  27L );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsSsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  36L ) + ( PH/2 );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[0].TsKsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}
	
		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  46L );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[1].TsTsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  55L ) + ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[1].TsSsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  65L );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, TSrec.TsDtl[1].TsKsyu, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		Cnt++;
	}

	//合計印字
	if( last_page == TRUE ){
		
		int rno = 0;
		while( 1 ){
			if( rno ==  m_pSpcListData->TsTbl.tp_rnum ){
				break;
			}
			if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rno ) ){
				break;
			}

			if( m_pShinSpecific22keika ){
				if( m_pShinSpecific22keika->IsNowZrtData( &TSrec ) == FALSE ){
					rno++;
					continue;
				}
			}else{
				return FALSE;
			}
			m_Arith.l_add( MONY1, MONY1, TSrec.TsDtl[0].TsTsyu );
			m_Arith.l_add( MONY2, MONY2, TSrec.TsDtl[0].TsSsyu );
			m_Arith.l_add( MONY3, MONY3, TSrec.TsDtl[0].TsKsyu );
			m_Arith.l_add( MONY4, MONY4, TSrec.TsDtl[1].TsTsyu );
			m_Arith.l_add( MONY5, MONY5, TSrec.TsDtl[1].TsSsyu );
			m_Arith.l_add( MONY6, MONY6, TSrec.TsDtl[1].TsKsyu );

			rno++;
		}	

		// 行ポイント
		lineCount = (PV3 * LINES_PER_PAGE_22);

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  17L )  + ( PH/2 );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, MONY1, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  27L );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, MONY2, FMTX12 );
		memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  36L ) + ( PH/2 );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, MONY3, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  46L );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, MONY4, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  55L ) + ( PH/2 );
		memset( TBUF, '\0', sizeof( TBUF ) );
		m_Arith.l_print( TBUF, MONY5, FMTX12 );
		memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_YPosition = ( PV1 * (HeadLn + 1L) ) + lineCount;
		m_XPosition = ( PH *  65L );
		memset( TBUF, '\0', sizeof(TBUF) );
		m_Arith.l_print( TBUF, MONY6, FMTX12 );
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 9L * PH ) + ( PH/2 );				// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT078h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表(経過措置)	売上割合９５％以上
//-----------------------------------------------------------------------------
// 引数	sign	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList50Keika( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	char	TBUF2[256]={0};
	memset( TBUF2, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(１)　調整後税額の計算表（課税売上割合が95%以上の場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 50L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 33L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=10; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 41L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );
	}
	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 46L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 30L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 33L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 21L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 24L));
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 9L ) );						// 最終行　座標

	//斜線
	m_YPosition = ( PV1 * (HeadLn + 9L) );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 12L) );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 15L) );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 18L) );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 5L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率４％適用分") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率6.3％適用分") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 47L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率４％)にのみ使途が特定されている特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 7L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）（計算表２(1)⑰Ｂ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 8L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｂ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　　４") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 13L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）（計算表２(1)⑰Ｃ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 14L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｃ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 15L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　６．３") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
//2018.05.09 UPDATE START
//	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	strcpy_s( TBUF, sizeof(TBUF), _T("④×　　　　　　　　　　　（１円未満の端数切捨て）") );
//2018.05.09 UPDATE END
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－③、①－⑤") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" ①－③") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 19L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" ①－⑤") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 19L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合（計算表４④）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 23L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表４④）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){	
	//	return TRUE;
	//}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 51L),							// 開始列　座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 63L),							// 最終列  座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑥×⑦　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 26L), TBUF, &m_SfPar ) ){	
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（③＋⑧、⑤＋⑧）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 29L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" ③＋⑧") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 28L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T(" ⑤＋⑧") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 28L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑨）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 32L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 金額
	char	WORK0[6], WORK1[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=10; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		memset( TBUF2, '\0', sizeof(TBUF2) );
		switch( i ){
			case 0 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpKgz63, FMTX12 );
				break;
			case 1 :
				memset( WORK0, '\0', sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpTsyu4, sizeof(WORK0) );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 2 :
				memset( WORK0, '\0', sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg4, sizeof(WORK0) );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 3 :
				memset( WORK0, '\0', sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpTsyu63, sizeof(WORK0) );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 4 :
				memset( WORK0, '\0', sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg63, sizeof(WORK0) );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				break;
			case 5 :	
				memset( WORK0, '\0', sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg4, sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Arith.l_sub( WORK1, m_pSpcListData->stval.SpKgz4, WORK0 );
				memset( TBUF, '\0', sizeof(TBUF) );
				m_Arith.l_print( TBUF, WORK1, FMTX12 );

				memset( WORK0, '\0', sizeof(WORK0) );
				memmove( WORK0, m_pSpcListData->stval.SpGszg63, sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Arith.l_sub( WORK1, m_pSpcListData->stval.SpKgz63, WORK0 );
				memset( TBUF2, '\0', sizeof(TBUF2) );
				m_Arith.l_print( TBUF2, WORK1, FMTX12 );

				break;
			case 7 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpTwag63, FMTX12 );
				break;
			case 8 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGgt4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpGgt63, FMTX12 );
				break;
			case 9 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpSiz63, FMTX12 );
				break;
			default:
				break;
		}
		if( i == 6 ){
			m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
				return TRUE;
			}

			memset( TBUF, '\0', sizeof(TBUF) );
			memset( WORK0, '\0', sizeof(WORK0) );
			m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
			m_Arith.l_print( TBUF, WORK0, FMTX12 );
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine  - PV1 + (PV1 / 2L), TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}
		else if(( i == 1)||( i == 2 )){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}else if(( i == 3)||( i == 4 )){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}
		else	{
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}

			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF2, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	
	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表	売上割合９５％未満　個別対応 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList51Keika( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	char	TBUF2[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(２)　調整後税額の計算表（課税売上割合が95%未満で個別対応方式を採用している場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 70L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 54L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=17; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ) {
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			case 10:	strcpy_s( TBUF, sizeof( TBUF ), "⑪");	break;
			case 11:	strcpy_s( TBUF, sizeof( TBUF ), "⑫");	break;
			case 12:	strcpy_s( TBUF, sizeof( TBUF ), "⑬");	break;
			case 13:	strcpy_s( TBUF, sizeof( TBUF ), "⑭");	break;
			case 14:	strcpy_s( TBUF, sizeof( TBUF ), "⑮");	break;
			case 15:	strcpy_s( TBUF, sizeof( TBUF ), "⑯");	break;
			case 16:	strcpy_s( TBUF, sizeof( TBUF ), "⑰");	break;
			case 17:	strcpy_s( TBUF, sizeof( TBUF ), "⑱");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 41L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}
		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 46L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 51L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 54L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 30L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 33L));
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 9L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 45L));
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 9L ) );						// 最終行　座標

	// 斜線
	m_YPosition = ( PV1 * (HeadLn + 21L));
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 24L));
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 27L));
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 30L));
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 9L));
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 12L));
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 15L));
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 18L));
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率４％適用分") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率6.3％適用分") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 47L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等(税率４％)にのみ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 7L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定されている特定収入（計算表２(2)⑰Ｅ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 8L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)⑰Ｅ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), "　　　　　　４" );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等(税率４％)に") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 13L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されている特定収入（計算表２(2)⑰Ｆ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 14L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)⑰Ｆ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 15L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　　４") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("④×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上げにのみ要する課税仕入れ等(税率6.3％)にのみ") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 19L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("使途が特定されている特定収入（計算表２(2)⑰Ｇ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 20L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(8%)⑰Ｅ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), "　　　　　６．３" );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 22L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑥×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 23L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 22L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税・非課税売上げに共通して要する課税仕入れ等(税率6.3％)に") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 25L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("のみ使途が特定されている特定収入（計算表２(2)⑰Ｈ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 26L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(2)(8%)⑰Ｆ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 27L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　６．３") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 28L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑧×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 29L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 30L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 28L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 28L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上割合（準ずる割合の承認を受けている場合はその割合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 32L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 51L),							// 開始列　座標
			m_YPosition + (PV1 * 31L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 63L),							// 最終列  座標
			m_YPosition + (PV1 * 31L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑤×⑩（１円未満の端数切捨て）、⑨×⑩（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 35L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑤×⑩") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 34L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑨×⑩") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 34L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③＋⑪、⑦＋⑪") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 38L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③＋⑪") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 37L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑦＋⑪") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 37L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－⑫") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 41L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合（計算表４④）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 44L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表４④）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 29L ), m_YPosition + ( PV1 * 45L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 51L),							// 開始列　座標
			m_YPosition + (PV1 * 43L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 63L),							// 最終列  座標
			m_YPosition + (PV1 * 43L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑬×⑭（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 47L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（⑫＋⑮）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 50L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑯）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 53L), TBUF, &m_SfPar ) ){
		return TRUE;
	}


	// 金額
	char	WORK0[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=17; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		memset( TBUF2, '\0', sizeof(TBUF2) );
		switch( i ){
			case 0 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpKgz63, FMTX12 );
				break;
			case 1 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSsyu4, FMTX12 );
				break;
			case 2 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg4, FMTX12 );
				break;
			case 3 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsyu4, FMTX12 );
				break;
			case 4 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg4, FMTX12 );
				break;
			case 5 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSsyu63, FMTX12 );
				break;
			case 6 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg63, FMTX12 );
				break;
			case 7 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKsyu63, FMTX12 );
				break;
			case 8 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg63, FMTX12 );
				break;
			case 9 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuws, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuwb, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 10 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKszg4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpKszg63, FMTX12 );
				break;
			case 11 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpGszg4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpGszg63, FMTX12 );
				break;
			case 12 :
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz4, m_pSpcListData->stval.SpGszg4 );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz63, m_pSpcListData->stval.SpGszg63 );
				m_Arith.l_print( TBUF2, WORK0, FMTX12 );
				break;
			case 13 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				memset( TBUF, '\0', sizeof(TBUF) );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 14:	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpTwag63, FMTX12 );
				break;
			case 15:
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpGszg4, m_pSpcListData->stval.SpTwag4 );	
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpGszg63, m_pSpcListData->stval.SpTwag63 );	
				m_Arith.l_print( TBUF2, WORK0, FMTX12 );
				break;
			case 16:
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpSiz63, FMTX12 );
				break;
			default:
				break;
		}
		if( (i==9)||(i==13)){

		}else if((1<=i)&&(i<=4)){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}else if((5<=i)&&(i<=8)){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}else{
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}

			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );				// ファンクションサイン
			m_SfPar.sf_hvsg = 0;										// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;										// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF2, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	

	return FALSE;
}

//-----------------------------------------------------------------------------
// 計算表-５　調整額の計算表	売上割合９５％未満　比例配分 （印刷）
//-----------------------------------------------------------------------------
// 引数	sign	：	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CTaxationListView::SpecList52Keika( int sign )
{
	// タイトル
	char	TBUF[256]={0};
	char	TBUF2[256]={0};
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計　算　表") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc | SFscut | SFcitv );	// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 20L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	(PV1 + ( PV1/2L));						// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT220z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 6L) );
	m_XPosition = ( PH *  30L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 会社情報
	memset( TBUF, '\0', sizeof(TBUF) );
	if( m_PrintCmInfo->OPL ){
		sprintf_s( TBUF, sizeof(TBUF), _T("%-40.40s %s            課税期間　%s～%s"), m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	else{
		sprintf_s( TBUF, sizeof(TBUF), _T("%s %-40.40s %s 課税期間　%s～%s"), m_COPNO, m_COPNM, m_DMME, m_SDATE, m_EDATE );
	}
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 120L * PH );							// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * (HeadLn - 3L) );
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("計算表５(３)　調整後税額の計算表（課税売上割合が95%未満で一括比例配分方式を採用している場合）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 70L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1 * 2;								// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	m_YPosition = ( PV1 * HeadLn);
	m_XPosition = ( PH *  5L );
	if( m_pVprn->DrawString( 0, m_XPosition, m_YPosition, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 外枠
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1 | FKrect,									// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
		(	m_XPosition +										// 最終列  座標
			( 70L * PH ) ),										// 外枠拡張
			m_YPosition + (PV1 * 39L)  );						// 最終行　座標
	// 横線
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=12; i++ ){
			m_pVprn->DrawWframe( 0,
					LKnrW1,										// 線種
					m_XPosition,								// 開始列　座標
						m_YPosition + m_StartLine, 				// 開始行　座標
					(	m_XPosition +							// 最終列  座標
					( 70L * PH ) ),								// 外枠拡張
					m_YPosition + m_StartLine  );				// 最終行　座標

		memset( TBUF, '\0', sizeof(TBUF) );
		switch( i ){
			case 0 :	strcpy_s( TBUF, sizeof( TBUF ), "①");	break;
			case 1 :	strcpy_s( TBUF, sizeof( TBUF ), "②");	break;
			case 2 :	strcpy_s( TBUF, sizeof( TBUF ), "③");	break;
			case 3 :	strcpy_s( TBUF, sizeof( TBUF ), "④");	break;
			case 4 :	strcpy_s( TBUF, sizeof( TBUF ), "⑤");	break;
			case 5 :	strcpy_s( TBUF, sizeof( TBUF ), "⑥");	break;
			case 6 :	strcpy_s( TBUF, sizeof( TBUF ), "⑦");	break;
			case 7 :	strcpy_s( TBUF, sizeof( TBUF ), "⑧");	break;
			case 8 :	strcpy_s( TBUF, sizeof( TBUF ), "⑨");	break;
			case 9 :	strcpy_s( TBUF, sizeof( TBUF ), "⑩");	break;
			case 10 :	strcpy_s( TBUF, sizeof( TBUF ), "⑪");	break;
			case 11 :	strcpy_s( TBUF, sizeof( TBUF ), "⑫");	break;
			default:	break;
		}
		memset( &m_SfPar, '\0', sizeof(SF_PAR) );
		m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );					// ファンクションサイン
		m_SfPar.sf_hvsg = 0;									// 出力方向
		m_SfPar.sf_xfld = ( 3L * PH );							// 横出力領域幅
		m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
		m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
		m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
		m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
		m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
		m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_ksiz =	FNT108z;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
		m_SfPar.sf_erto = 0;									// 文字拡大縮小率
		if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 41L ), m_YPosition + ( PV1 * 2L ) + m_StartLine, TBUF, &m_SfPar ) ){
			return TRUE;
		}

		m_StartLine += ( PV1 * 3L );
	}

	// 縦線
	m_YPosition = ( PV1 * (HeadLn + 3L) );
	m_XPosition = ( PH * 46L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 36L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 39L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 18L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 21L) );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 6L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 30L) );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition,	 									// 最終列  座標
			m_YPosition + ( PV1 * 9L ) );						// 最終行　座標

	//斜線
	m_YPosition = ( PV1 * (HeadLn + 15L) );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 18L) );
	m_XPosition = ( PH * 49L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標


	m_YPosition = ( PV1 * (HeadLn + 9L) );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * (HeadLn + 12L) );
	m_XPosition = ( PH * 62L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition,										// 開始列　座標
			m_YPosition, 										// 開始行　座標
			m_XPosition + ( PH  * 13L ),						// 最終列  座標
			m_YPosition - ( PV1 * 3L ) );						// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("内          　　　　　　容") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率４％適用分") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("税率6.3％適用分") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 13L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 2L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 47L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("円") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xc | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 17L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 60L ), m_YPosition + ( PV1 * 4L ) + 10L, TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整前の課税仕入れ等の税額の合計額") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 5L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率４％)にのみ使途が特定されている特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 7L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）（計算表２(1)⑰Ｂ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 8L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｃ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　　４") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 10L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("②×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 11L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０５") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 12L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 10L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 13L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("（｢課税仕入れ等に係る特定収入｣）（計算表２(1)⑰Ｃ）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 14L ) + ( PV1/2 ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), _T("（計算表２(1)⑰Ｃ）") );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 9L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　６．３") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 16L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("④×　　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 17L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("　　　　　１０８") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 18L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  1L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 12L),							// 開始列　座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 17L),							// 最終列  座標
			m_YPosition + (PV1 * 16L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("課税売上割合") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 20L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 51L),							// 開始列　座標
			m_YPosition + (PV1 * 19L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 63L),							// 最終列  座標
			m_YPosition + (PV1 * 19L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③×⑥（１円未満の端数切捨て）、⑤×⑥（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
//	m_SfPar.sf_ksiz =	FNT096z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 23L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("③×⑥") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 22L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑤×⑥") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 12L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 22L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("①－⑦") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 26L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("調整割合（計算表４④）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 29L ), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	//memset( TBUF, '\0', sizeof(TBUF) );
	//strcpy_s( TBUF, sizeof(TBUF), "（計算表４④）" );
	//memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	//m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	//m_SfPar.sf_hvsg = 0;										// 出力方向
	//m_SfPar.sf_xfld = ( 10L * PH );								// 横出力領域幅
	//m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	//m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	//m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	//m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	//m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	//m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_ksiz =	FNT090z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	//m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	//if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 27L ), m_YPosition + ( PV1 * 24L ), TBUF, &m_SfPar ) ){
	//	return TRUE;
	//}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_pVprn->DrawWframe( 0,
			LKnrW1,												// 線種
			m_XPosition + (PH  * 51L),							// 開始列　座標
			m_YPosition + (PV1 * 28L) + (PV1 / 2),				// 開始行　座標
			m_XPosition + (PH  * 63L),							// 最終列  座標
			m_YPosition + (PV1 * 28L) + (PV1 / 2)  );			// 最終行　座標

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("⑧×⑨　　　　　　　　　　（１円未満の端数切捨て）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 32L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("特定収入に係る課税仕入れ等の税額（⑦＋⑩）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 35L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	memset( TBUF, '\0', sizeof(TBUF) );
	strcpy_s( TBUF, sizeof(TBUF), _T("控除対象仕入税額（①－⑪）") );
	memset( &m_SfPar, '\0', sizeof(SF_PAR) );
	m_SfPar.sf_fcsg = ( SFp_xl | SFp_yc );						// ファンクションサイン
	m_SfPar.sf_hvsg = 0;										// 出力方向
	m_SfPar.sf_xfld = ( 40L * PH );								// 横出力領域幅
	m_SfPar.sf_yfld =	PV1;									// 縦出力領域幅
	m_SfPar.sf_mrgn[0] = 0;										// 領域内マージン　左
	m_SfPar.sf_mrgn[1] = 0;										// 　　〃　　　　　右
	m_SfPar.sf_mrgn[2] = 0;										// 　　〃　　　　　上
	m_SfPar.sf_mrgn[3] = 0;										// 　　〃　　　　　下
	m_SfPar.sf_kpit = 0;										// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_ksiz =	FNT108z;								// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
	m_SfPar.sf_erto = 0;										// 文字拡大縮小率
	if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 1L ), m_YPosition + ( PV1 * 38L), TBUF, &m_SfPar ) ){
		return TRUE;
	}

	// 金額
	char	WORK0[6];
	m_YPosition = ( PV1 * HeadLn );
	m_XPosition = ( PH *  5L );
	m_StartLine = ( PV1 * 3L );
	for( int i=0; i!=12; i++ ){
		memset( TBUF, '\0', sizeof(TBUF) );
		memset( TBUF2, '\0', sizeof(TBUF2) );
		switch( i ){
			case 0 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKgz4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpKgz63, FMTX12 );
				break;
			case 1 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu4, FMTX12 );
				break;
			case 2 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg4, FMTX12 );
				break;
			case 3 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTsyu63, FMTX12 );
				break;
			case 4 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpZszg63, FMTX12 );
				break;
			case 5 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuws, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpKuwb, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 6 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTszg4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpTszg63, FMTX12 );
				break;
			case 7 :
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz4, m_pSpcListData->stval.SpTszg4 );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_sub( WORK0, m_pSpcListData->stval.SpKgz63, m_pSpcListData->stval.SpTszg63 );
				m_Arith.l_print( TBUF2, WORK0, FMTX12 );
				break;
			case 8 :	
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpFsyu, FMTX12 );
				memset( &m_SfPar, '\0', sizeof(SF_PAR) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - (PV1 * 2L) + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}

				memset( TBUF, '\0', sizeof(TBUF) );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTuwb, m_pSpcListData->stval.SpFsyu );
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( &m_SfPar, '\0', sizeof( struct _SF_PAR ) );
				m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
				m_SfPar.sf_hvsg = 0;									// 出力方向
				m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
				m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
				m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
				m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
				m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
				m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
				m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
				m_SfPar.sf_erto = 0;									// 文字拡大縮小率
				if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 50L ) + ( PH/2 ), m_YPosition + ( PV1 * 3L ) + m_StartLine - PV1 + ( PV1 / 2L), TBUF, &m_SfPar ) ){
					return TRUE;
				}
				break;
			case 9 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpTwag4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpTwag63, FMTX12 );
				break;
			case 10 :	
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTszg4, m_pSpcListData->stval.SpTwag4 );	
				m_Arith.l_print( TBUF, WORK0, FMTX12 );
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_add( WORK0, m_pSpcListData->stval.SpTszg63, m_pSpcListData->stval.SpTwag63 );	
				m_Arith.l_print( TBUF2, WORK0, FMTX12 );
				break;
			case 11 :
				m_Arith.l_print( TBUF, m_pSpcListData->stval.SpSiz4, FMTX12 );
				m_Arith.l_print( TBUF2, m_pSpcListData->stval.SpSiz63, FMTX12 );
				break;
			default:
				break;
		}
		if((i==5)||(i==8)){

		}else if((i==1)||(i==2)){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
			m_SfPar.sf_hvsg = 0;									// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;									// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}else if((i==3)||(i==4)){
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
			m_SfPar.sf_hvsg = 0;									// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;									// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}
		}else{
			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
			m_SfPar.sf_hvsg = 0;									// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;									// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 44L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF, &m_SfPar ) ){
				return TRUE;
			}

			memset( &m_SfPar, '\0', sizeof(SF_PAR) );
			m_SfPar.sf_fcsg = ( SFp_xr | SFp_yc | SFctok );			// ファンクションサイン
			m_SfPar.sf_hvsg = 0;									// 出力方向
			m_SfPar.sf_xfld = ( 12L * PH );							// 横出力領域幅
			m_SfPar.sf_yfld =	PV1;								// 縦出力領域幅
			m_SfPar.sf_mrgn[0] = 0;									// 領域内マージン　左
			m_SfPar.sf_mrgn[1] = 0;									// 　　〃　　　　　右
			m_SfPar.sf_mrgn[2] = 0;									// 　　〃　　　　　上
			m_SfPar.sf_mrgn[3] = 0;									// 　　〃　　　　　下
			m_SfPar.sf_kpit = 0;									// 文字ピッチ幅　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_ksiz =	FNT108h;							// 文字サイズ　　X0.1ﾎﾟｲﾝﾄ
			m_SfPar.sf_erto = 0;									// 文字拡大縮小率
			if( m_pVprn->DrawString( 0, m_XPosition + ( PH * 57L ), m_YPosition + ( PV1 * 3L ) + m_StartLine, TBUF2, &m_SfPar ) ){
				return TRUE;
			}
		}

		m_StartLine += ( PV1 * 3L );
	}
	

	return FALSE;
}