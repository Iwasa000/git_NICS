#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//== 定数 ==
static	short daytl[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };

//-----------------------------------------------------------------------------
// 月日情報の変換
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::CnvZvolMinfo()
{
	memset( &m_Vol2, '\0', sizeof(VOL2) );

	int month = 0;
	for( int i=0; i<SH_MONTH_MAX; i++ ){
		if( i==0 ){
			if( m_pZmSub->minfo->MoveFirst() == ERR ){
				break;
			}
		}
		else{
			if( m_pZmSub->minfo->MoveNext() == ERR ){
				break;
			}
		}
		if( m_pZmSub->minfo->st == -1 ){
			break;
		}
		else if( (m_pZmSub->minfo->kes_sgn==1) || (m_pZmSub->minfo->kes_sgn==2) || (m_pZmSub->minfo->kes_sgn==3) ){
			// 変換しない
			;
		}
		else{
			m_Vol2.D_TBL[month]._INSW = m_pZmSub->minfo->insw;							// 入力許可サイン
			m_Util.date_to_bcd( 0x01, m_Vol2.D_TBL[month]._SMD, m_pZmSub->minfo->smd );	// 開始年月日
			m_Util.date_to_bcd( 0x01, m_Vol2.D_TBL[month]._EMD, m_pZmSub->minfo->emd );	// 最終年月日
			m_Vol2.D_TBL[month].C_KSW = m_pZmSub->minfo->c_ksw;							// 集計サイン
			m_Month26[month] = m_pZmSub->minfo->sz_syk;									// 消費税申告書中間サイン
			month++;
		}
	}

	return 0;
}

//--> '14.06.07 INS START
//-----------------------------------------------------------------------------
// 月日が指定範囲内？
//-----------------------------------------------------------------------------
// 引数	mofs		：	対象月
//		mm			：	チェック月
//		dd			：	チェック日
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	範囲内
//			FALSE	：	範囲外
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsInMonthRange( int mofs, unsigned char mm, unsigned char dd )
{
	BOOL	bRt = FALSE;

	if( mm == m_Vol2.D_TBL[mofs]._EMD[0] ){
		if( mm == m_Vol2.D_TBL[mofs]._SMD[0] ){
			if( (m_Vol2.D_TBL[mofs]._SMD[1]<=dd) && (dd<=m_Vol2.D_TBL[mofs]._EMD[1]) ){
				bRt = TRUE;
			}
		}
		else{
			if( dd <= m_Vol2.D_TBL[mofs]._EMD[1] ){
				bRt = TRUE;
			}
		}
	}

	return bRt;
}
//<-- '14.06.07 INS START

//-----------------------------------------------------------------------------
// 入力年月　開始・最終設定
//-----------------------------------------------------------------------------
//int	CDBSyzShinView::ofs_get( void )
int	CMainFrame::GetMofs()
{
	int		s_no, d_no, tno;
	int		sno, i;
	unsigned char	ttk;
	
	if( (m_pZmSub->zvol->m_type&0xff) == 0 ){	// 年次
		unsigned char	IP_YMD[3] = {0};
		m_Util.date_to_bcd( 0x00, IP_YMD, m_pZmSub->zvol->ip_ymd );

		if( GetShinofst(&IP_YMD[1], &i) ){
			return -1;
		}
		m_ShinInfo.INP_S = m_ShinInfo.s_tno = i;

//--> '14.06.10 INS START
//		unsigned char	SS_YMD[3] = {0};
//		m_Util.date_to_bcd( 0x00, SS_YMD, m_pZmSub->zvol->ss_ymd );
//		if( GetShinofst(&SS_YMD[1], &i) ){
//			return -1;
//		}
//		m_ShinInfo.s_tno = i;
//<-- '14.06.10 INS END

		if( (m_pZmSub->zvol->m_type&0xff) == 0x01 ){	// 合併
			m_ShinInfo.INP_E = m_ShinInfo.d_tno = 12;
		}
		else{
			for( i=12; i>=0; i-- ){	// 最終年月オフセット
				if( m_Vol2.D_TBL[i]._INSW ){
					break;
				}
			}
			m_ShinInfo.INP_E = m_ShinInfo.d_tno = i;
		}
	}
	else {	// 月次
		for( m_ShinInfo.d_tno=0; m_ShinInfo.d_tno<13; m_ShinInfo.d_tno++ ){
			if( m_Vol2.D_TBL[m_ShinInfo.d_tno]._INSW ){
				break;
			}
		}
		if( m_ShinInfo.d_tno > 12 ){
			return -1;		// 月次処理（該当月）？
		}
		m_ShinInfo.INP_S = m_ShinInfo.INP_E = m_ShinInfo.d_tno;
	}

	unsigned char	ttd=0x00;	// 日付

	char	tmpYMD[3] = {0};
	ttk = 0x13;
	if( m_SnHeadData.Sn_SKKBN % 2 ){
		// 確定申告
		m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &tmpYMD[0], &tmpYMD[1], &tmpYMD[2] );
		ttk = tmpYMD[1];
		ttd = tmpYMD[2];
	}
	else{
		// 中間申告
		m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &tmpYMD[0], &tmpYMD[1], &tmpYMD[2] );
		ttk = tmpYMD[1];
		ttd = tmpYMD[2];
	}
/*- '14.06.06 -*/
//	if( (ttk==m_Vol2.D_TBL[12]._EMD[0]) || (ttk==m_Vol2.D_TBL[11]._EMD[0]) ){
/*-------------*/
	if( IsInMonthRange(12, ttk, ttd) || IsInMonthRange(11, ttk, ttd) ){
/*-------------*/

		if( (m_pZmSub->zvol->m_type&0xff) == 0x01 ){
			tno = 12;	// 合併
		}
		else{
			tno = ( m_Vol2.D_TBL[12]._INSW ? 12 : 11 );
		}
	}
	else{
		for( tno=0; tno<12; tno++ ){
/*- '14.06.07 -*/
//			if( ttk == m_Vol2.D_TBL[tno]._EMD[0] ){
/*-------------*/
			if( IsInMonthRange(tno, ttk, ttd) ){
/*-------------*/
				break;
			}
		}
		if( tno >= 12 ){
			return -1;		// 年次処理（該当月処理月無し！）
		}
	}
	m_ShinInfo.d_tno = tno;
	i = 1;
	sno = s_no = m_ShinInfo.s_tno;
	switch( m_SnHeadData.Sn_SKKBN ){
		case 1 :
		case 3 :		// 確定申告
			if( m_SnHeadData.Sn_Sign4&0x40 ){
				m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &tmpYMD[0], &tmpYMD[1], &tmpYMD[2] );
				if( GetShinofst((unsigned char*)&tmpYMD[1], &sno) ){
					return -1;
				}
			}
			else{
				if( m_SnHeadData.Sn_KZTNSK&0x0f ){ 	// 課税期間短縮
					m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &tmpYMD[0], &tmpYMD[1], &tmpYMD[2] );
					if( GetShinofst((unsigned char*)&tmpYMD[1], &sno) ){
						return -1;
					}
					if( (m_SnHeadData.Sn_KZTNSK&0x0f) == 0x02 ){
						d_no = 12;
						if( m_pZmSub->zvol->ss_jsw&0x01 ){
							m_SnHeadData.GetYmdDataGen( ID_ICSSH_KESSAN_TO, &tmpYMD[0], &tmpYMD[1], &tmpYMD[2] );
							if( GetShinofst((unsigned char*)&tmpYMD[1], &d_no) ){
								return -1;
							}
							d_no++;	// 決算修正月
						}
						if( m_ShinInfo.d_tno == d_no ){
							sno = m_ShinInfo.d_tno - 1;
						}
						else{
							sno = m_ShinInfo.d_tno;
						}
					}
					else{
						if( (m_ShinInfo.d_tno-sno) > 2 ){ 
							sno = m_ShinInfo.d_tno - ( m_ShinInfo.d_tno == 12 ? 3 : 2 );
						}
					}
//					if( (Ver&0xff) == 1 ){
//						if( m_Arith.l_jsgn( Vol2.D_TBL[sno]._SMD, &GEN2[1], 2 ) >= 0 )
//							Ver = 0x02;
//					}
				}
				else if( (!m_pZmSub->zvol->ss_jsw && !m_Vol2.D_TBL[0]._INSW) ||
					(((m_pZmSub->zvol->apno&0xff)==0x10) && !(m_SnHeadData.Sn_KOANB&0x01)) ||
					(m_SnHeadData.Sn_KOJIN&0x01) ){
					i = sno = 0;
				}
			}
			break;
		default:				// 中間申告
			switch( m_SnHeadData.Sn_MCOUNT ){
				case 3 :	// 年３回 
					sno = m_ShinInfo.d_tno - 2;
					break;
				case 11:	// 年11回
					sno = m_ShinInfo.d_tno;	
					break;
				default:	// 年１回
					sno = m_ShinInfo.d_tno - 5;
					break;
			}
			if( !m_pZmSub->zvol->ss_jsw && !m_Vol2.D_TBL[0]._INSW ){
				i = 0;
			}
			break;
	}
	if( !i || (s_no<sno) ){
		m_ShinInfo.s_tno = sno;
	}
	
	if( m_ShinInfo.s_tno < 0 ){
		m_ShinInfo.s_tno = 0;
	}
	if( m_ShinInfo.d_tno > 12 ){
		m_ShinInfo.d_tno = 12;
	}

//--> '14.06.10 INS START
	char	YMDS[4]={0}, YMDE[4]={0};
	// ５億円 集計期間の取得
	switch( m_SnHeadData.Sn_SKKBN ){
		case 1 : case 3 :		//確定・確定修正
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &YMDS[0], &YMDS[1], &YMDS[2] );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &YMDE[0], &YMDE[1], &YMDE[2] );
			break;
		default:				//中間・中間修正
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &YMDS[0], &YMDS[1], &YMDS[2] );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &YMDE[0], &YMDE[1], &YMDE[2] );
			break;
	}
	// 開始オフセットの取得
	if( GetShinofst( (unsigned char*)&YMDS[1], &m_ShinInfo.bil_stno ) ){
		m_ShinInfo.bil_stno = 0;
	}
	// 最終オフセットの取得
	if( GetShinofst( (unsigned char*)&YMDE[1], &m_ShinInfo.bil_dtno ) ){
		m_ShinInfo.bil_dtno = 11;
	}
//<-- '14.06.10 INS END
	
	return( ChekCalqed( (s_no < sno ? sno : s_no), m_ShinInfo.d_tno )+1 );
}

//-----------------------------------------------------------------------------
// 月日よりオフセット返送
//-----------------------------------------------------------------------------
//	( short )
//	shin_ofst( date, ofst, Vol1, Vol2 )
//	char		*date;		月日
//	int			*ofst;		オフセット（返送）
//	struct	_VOL1	*Vol1;	（管理情報部）
//	struct	_VOL2	*Vol2;	（各月情報部）
//-----------------------------------------------------------------------------
//		status	 0		処理ＯＫ！
//				-1		設定不良	
//-----------------------------------------------------------------------------
//short CDBSyzShinView::shin_ofst( unsigned char *date, int *ofst, struct _VOL1 *Vol1, struct _VOL2 *Vol2 )
short CMainFrame::GetShinofst( unsigned char *date, int *ofst )
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
	if( !(MONTH&0xff) ){
		return -1;
	}

	// 初期設定 平成年度で１３ヶ月分のデータ管理テーブル作成
	InizShinDate();

	char	EYMD[3]={0};
	m_Util.date_to_bcd( 0x00, (unsigned char*)EYMD, m_pZmSub->zvol->ee_ymd );

	if( (DAY&0xff) == 0xff ){
		// 月のみ
		if( !(MONTH&0x1f) || (MONTH&0x80) )	{
			// 決算修正
			if( MONTH&0x1f ){
				if( ((MONTH&0x1f)<=0x00) || ((MONTH&0x1f)>0x12) ){
					return -1;
				}
				if( (MONTH&0x1f) != (EYMD[1]&0x1f) ){
					return -1;
				}
			}
			mofs = 12;
		}
		else {
			for( mofs=0, err=0; mofs!=12; mofs++ ){
				if( (m_Vol2.D_TBL[mofs]._EMD[0]&0x1f) == (MONTH&0x1f)) {
					err = 1;
					break;
				}
			}
			if( !err ){
				return -1;
			}
		}
		// オフセット
		*ofst = (int)mofs;
	}
	else {
		// 月日
		// 閏年チェック
		if( (MONTH&0x1f) == 0x02 ){
			sprintf_s( ASC, sizeof( ASC ), _T("%02x"), (DAY&0xff) );
			eday = atoi( ASC );
			if( eday > (short)shin_uday(m_pZmSub->zvol->ee_ymd) ){
				return -1;
			}
		}

		if( !(MONTH&0x1f) || (MONTH&0x80) ){
			// 決算修正
			if( MONTH&0x1f ){
				if( ((MONTH&0x1f)<=0x00) || ((MONTH&0x1f)>0x12) ){
					return -1;
				}
				CMNTH = ( MONTH & 0x1f );
			}
			else{
				if( (DAY&0xff) >= (m_dtl[12].Symd[2]&0xff) ){
					CMNTH = (m_dtl[12].Symd[1]&0x1f);
				}
				if( (DAY&0xff) <= (m_dtl[12].Eymd[2]&0xff) ){
					CMNTH = (m_dtl[12].Eymd[1]&0x1f);
				}
			}
				
			if( !(shin_mdchk( CMNTH, DAY, m_dtl[12].Symd, m_dtl[12].Eymd, m_pZmSub->zvol->ee_ymd) ) ) {
				mofs = 12;
			}
			else{
				return -1;
			}
		}
		else{

			// 期首～１２ヶ月目
			for( mofs=0, err=0; mofs!=12; mofs++ ){
				
				if ( !(shin_mdchk( (MONTH&0x1f), DAY, m_dtl[mofs].Symd, m_dtl[mofs].Eymd, m_pZmSub->zvol->ee_ymd) ) ) {
					err = 1;
					break;
				}
			}
			if( !err ){
				return -1;
			}
		}
		// オフセット
		*ofst = (int)mofs;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 集計済みチェック
//-----------------------------------------------------------------------------
// 引数	sm
//		km
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
//int CMainFrame::skchek( int sm, int km )
int CMainFrame::ChekCalqed( int sm, int km )
{
	int	i, last, stat, emon, smon;
	char	asci[20];

	if( ((m_pZmSub->zvol->m_type&0xff)==0x00) || ((m_pZmSub->zvol->m_type&0xff)==0x01) ){
		// 年次・合併
		// 開始月設定
		sprintf_s( asci,  sizeof(asci),"%08d", m_pZmSub->zvol->ip_ymd );
		int	ymd = atoi( &asci[4] );
		if( m_VolDate.db_vd_offset(0, ymd, &stat, m_pZmSub) ){
			smon = sm; 
		}
		else{
			if( sm < stat ){
				smon = stat;
			}
			else{
				smon = sm;
			}
		}
		// 最終月設定
/*- '14.02.18 -*/
//		if( (m_pZmSub->zvol->m_type&0xff) == 0x00 ){
//			emon = km;	// 月次
//		}
//		else{
//			if( (m_Vol2.D_TBL[12]._INSW&0x0f) == 0x00 ){
//				last = 11;
//			}
//			else{
//				last = 12;
//			}
//			if( km > last ){
//				emon = last;
//			}
//			else{
//				emon = km;
//			}
//		}
/*-------------*/
		if( (m_Vol2.D_TBL[12]._INSW&0x0f) == 0x00 ){
			last = 11;
		}
		else{
			last = 12;
		}
		if( km > last ){
			emon = last;
		}
		else{
			emon = km;
		}
/*-------------*/
		for( i=smon; i<=emon; ++i ){
			if( !m_SYsw[i] ){
				return( i );
			}
		}
	}
	else{
		// 月次
		if( !m_SYsw[km] ){
			return km;
		}
		else{
			return -1;
		}
	}
	
	return -1;	// 集計済み
}

// 期間処理モジュール	初期設定
//	shin_iniz( Vl1, Vl2 )
//	struct	VOL_N1	*Vl1;	（管理情報部）
//	struct	VOL_N2	*Vl2;	（各月情報部）
//short CDBSyzShinView::shin_iniz( struct	_VOL1 *Vol1, struct _VOL2 *Vol2 )
int CMainFrame::InizShinDate()
{
	// 	決算期間　年度変換
	//	西暦→平成
/*	memmove( SYMD, Vol1->SS_YMD, 3 );
	shin_datecnv( Vol1->SS_YMD[0], (unsigned char *)SYMD, 0 );
	memmove( EYMD, Vol1->EE_YMD, 3 );
	shin_datecnv( Vol1->EE_YMD[0], (unsigned char *)EYMD, 0 );
	memmove( CYMD, Vol1->CS_YMD, 3 );
	shin_datecnv( Vol1->CS_YMD[0], (unsigned char *)CYMD, 0 );
	memmove( IYMD, Vol1->IP_YMD, 3 );
	shin_datecnv( Vol1->IP_YMD[0], (unsigned char *)IYMD, 0 );
*/

	unsigned char	EYMD[3]={0}, CYMD[3]={0};
	m_Util.date_to_bcd( 0x00, EYMD, m_pZmSub->zvol->ee_ymd );
	m_Util.date_to_bcd( 0x00, CYMD, m_pZmSub->zvol->cs_ymd );


	// 閏年設定
	daytl[2] = shin_uday( m_pZmSub->zvol->ee_ymd );
	
	// データ期間　算出
	memset( m_dtl, '\0', sizeof(SH_DAYPAC)*14 );
	for( int i=0; i!=13; i++ ){
		memmove( &m_dtl[i].Symd[1], m_Vol2.D_TBL[i]._SMD, 2 );
		memmove( &m_dtl[i].Eymd[1], m_Vol2.D_TBL[i]._EMD, 2 );
		// 年度　設定
		if( ((m_dtl[i].Symd[1]&0x1f) < (EYMD[1]&0x1f)) ||
			(((m_dtl[i].Symd[1]&0x1f) == (EYMD[1]&0x1f)) &&	((m_dtl[i].Symd[2]&0xff) <= (EYMD[2]&0xff))) ){
			m_dtl[i].Symd[0] = EYMD[0];
		}
		else{
			m_dtl[i].Symd[0] = CYMD[0];
		}
		if( ((m_dtl[i].Eymd[1]&0x1f) < (EYMD[1]&0x1f)) ||
			(((m_dtl[i].Eymd[1]&0x1f) == (EYMD[1]&0x1f)) &&	((m_dtl[i].Eymd[2]&0xff) <= (EYMD[2]&0xff))) ){
			m_dtl[i].Eymd[0] = EYMD[0];
		}
		else{
			m_dtl[i].Eymd[0] = CYMD[0];
		}
	}
	return 0;
}

#ifdef _H_KAIGEN_CUT_
//-----------------------------------------------------------------------------
//		平成～西暦　変換
//	----------------------------
//	shin_datecnv( year, date, sign ) 
//-----------------------------------------------------------------------------
//	char	year;	年  変換前
//	char	*date;  年　変換後
//	char	sign;   変換方向
//-----------------------------------------------------------------------------
//		Status	 0 = OK!
//				-1 = ERROR
//-----------------------------------------------------------------------------
//int CDBSyzShinView::shin_datecnv( unsigned char year, unsigned char *date, int sign )
int CMainFrame::shin_datecnv( unsigned char year, unsigned char *date, int sign )
{
	unsigned	char	ymd[4], dchk[10];
	int		yy, y1;

	sprintf_s( (char *)dchk,  sizeof(dchk), _T("%02x"), year );
	y1 = atoi( (char *)dchk );
	if( (y1<0) || (y1>99) ){
		return -1;
	}

	if( sign ){	// 平成→西暦
		if( y1 >= 12 ){
			yy = y1 - 12;	// 西暦２０００年以上
		}
		else{
			yy = y1 + 88;	// 　　　　〃　　未満
		}
		sprintf_s( (char *)dchk, sizeof(dchk), _T("%02d"), yy );
		m_Arith.l_pack( ymd, dchk, 2 );
		*date = ymd[0];
	}
	else{		//  西暦→平成
		if( y1 < 89 ){
			yy = y1 + 12;	// 西暦２０００年以上
		}
		else{
			yy = y1 - 88;	// 　　　　〃　　未満
		}
		sprintf_s( (char *)dchk, sizeof(dchk), _T("%02d"), yy );
		m_Arith.l_pack( ymd, dchk, 2 );
		*date = ymd[0];
	}
	return 0;
}
#endif

#ifdef _H_KAIGEN_CUT_
//-----------------------------------------------------------------------------
// 閏年計算
//-----------------------------------------------------------------------------
//	(short)
//	shin_uday( eymd )
//	char *eymd; 期末年月日
//				（西暦年度）
//-----------------------------------------------------------------------------
//	 status	 28 or 29
//-----------------------------------------------------------------------------
//short CDBSyzShinView::shin_uday( char *eymd )
short CMainFrame::shin_uday( int eymd )
{
	short	mmdd;
	short	dyy, dy, dm, dd, i;

	dy = eymd / 10000;
	mmdd = eymd % 10000;
	dm = mmdd / 100;
	dd = mmdd % 100;

	if( (dm==1) || ( (dm==2)&&(dd<=28) ) ){
		dyy = dy - 1;
	}
	else{
		dyy = dy;
	}

	i = shin_Feb(dyy);
	if (((shin_Feb(dyy+1) + 7 - i) % 7) == 1 ){
		return 28;
	}
	else{
		return 29;
	}
}
#endif

//-----------------------------------------------------------------------------
// 期末年月日より期中の２月末日を取得
//-----------------------------------------------------------------------------
// 引数	eymd	：	期末年月日（西暦）
//-----------------------------------------------------------------------------
// 返送値		：	２月末日
//-----------------------------------------------------------------------------
short CMainFrame::shin_uday(int eymd)
{
	int	dy=0, dm=0, dd=0, mmdd=0, dyy=0;
	dy = eymd / 10000;
	mmdd = eymd % 10000;
	dm = mmdd / 100;
	dd = mmdd % 100;

	// チェックする２月末日は何年分？
	if( (dm==1) || ((dm==2)&&(dd<=28)) ){
		dyy = dy - 1;
	}
	else{
		dyy = dy;
	}
	
	BYTE	feb_ymd[4] = {0};
	int		chkYmd = (dyy%100)*10000 + 228;	// チェックする末日
	int_bcd( feb_ymd, chkYmd, 3 );
	_vd_getfebeday( feb_ymd );

	if( feb_ymd[2] == 0x29 ){
		return 29;
	}
	else{
		return 28;
	}
}

#ifdef _H_KAIGEN_CUT_
//-----------------------------------------------------------------------------
// 閏年計算
//-----------------------------------------------------------------------------
//	(short)
//	shin_uday( eymd )
//	char *eymd; 期末年月日
//				（平成年度）
//-----------------------------------------------------------------------------
//	 status	 28 or 29
//-----------------------------------------------------------------------------
//short CDBSyzShinView::shin_uday( char *eymd )
short CMainFrame::shin_uday( char *eymd )
{
	char	dchk[20]={0};
	short	dy,dm,dd;

	// 期末年月日　西歴変換
	sprintf_s( dchk, sizeof(dchk), _T("%02x"), (*eymd&0xff) );
	dy = atoi ( dchk );
	sprintf_s( dchk, sizeof(dchk), _T("%02x"), (*(eymd+1)&0x1f) );
	dm = atoi ( dchk );
	sprintf_s( dchk, sizeof(dchk), _T("%02x"), (*(eymd+2)&0xff) );
	dd = atoi ( dchk );
	
	if( dy >= 12 ){
		dy = ( dy - 12 ) + 2000;	// 西暦２０００年以上
	}
	else{
		dy = ( dy + 88 ) + 1900;	// 西暦２０００年未満
	}

	int	ymd = (dy*10000) + (dm*100) + dd;
	return shin_uday( ymd );
}
#endif

#ifdef _H_KAIGEN_CUT_
//-----------------------------------------------------------------------------
// 閏年計算用モジュール
//-----------------------------------------------------------------------------
// 引数	yr	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
short CMainFrame::shin_Feb( short yr )
{
	short	y,d;

	y = yr - 1900;
	d = 1 + y + ( y + 3 ) / 4;
	if( yr > 1900 ){
		--d;
	}
	return (d%7);
}
#endif

//-----------------------------------------------------------------------------
// 月日よりデータ期間チェック
//-----------------------------------------------------------------------------
//	( short )
//	shin_mdchk( mnth, day, symd, eymd, eeymd ) 
//	char	mnth;	該当月
//	char	day;	該当日
//	char	*symd;	データ開始年月日
//	char	*eymd;	データ最終年月日
//	char	*eeymd;	期末年月日
//
//-----------------------------------------------------------------------------
//		status	0 :	該当データ期間内
//				1 : 該当データ期間外
//-----------------------------------------------------------------------------
//short CDBSyzShinView::shin_mdchk( char mnth, char day, char *symd, char *eymd, char *eeymd )
short CMainFrame::shin_mdchk( char mnth, char day, char *symd, char *eymd, int eeymd )
{
	short		matu;
	char		CDAY, ASC[10]={0};

	// 末日チェック
	matu = shin_mday( (mnth&0x1f), eeymd );
	sprintf_s(ASC, sizeof( ASC ), _T("%02d"), matu );
	m_Arith.l_pack( &CDAY, ASC, 2 );
	if( (day&0xff) > (CDAY&0xff) ){
		return -1;
	}

	if( (*(symd+1)&0x1f) == (*(eymd+1)&0x1f) ){
		// 同一月のデータ期間
		if( (mnth&0x1f) == (*(symd+1)&0x1f) &&
			(*(symd+2)&0xff) <= (day&0xff) &&
			(*(eymd+2)&0xff) >= (day&0xff) ){
			return 0;
		}
	}
	else {
		// 月またがりのデータ期間
		// データ開始より末日まで
		matu = shin_mday( (mnth&0x1f), eeymd );
		sprintf_s(ASC, sizeof(ASC), _T("%02d"), matu );
		m_Arith.l_pack( &CDAY, ASC, 2 );
		if( (mnth&0x1f) == (*(symd+1)&0x1f) &&
			(*(symd+2)&0xff) <= (day&0xff) &&
			(CDAY&0xff) >= (day&0xff) ){
			
			return 0;
		}

		// １日よりデータ最終まで
		if( (mnth&0x1f) == (*(eymd+1)&0x1f) &&
			(day&0xff) >= 0x01 &&
			(*(eymd+2)&0xff) >= (day&0xff) ){
			return 0;
		}
	}
	return 1;
}

//-----------------------------------------------------------------------------
// 期末月計算
//	( short )
//-----------------------------------------------------------------------------
//	shin_mday( month, eymd )
//	char	month;  該当月(BCD)
//	char	*eymd; 期末年月日
//				（平成年度）
//-----------------------------------------------------------------------------
//		status  末日
//-----------------------------------------------------------------------------
//short CDBSyzShinView::shin_mday( char month, char *eymd )
short CMainFrame::shin_mday( char month, int eymd )
{
	char	dchk[20];
	short	dmnth;

	sprintf_s( dchk, sizeof(dchk), _T("%02x"), (month&0x1f) );
	dmnth = atoi( dchk );
	if( dmnth == 2 ){
		return( (short)shin_uday(eymd) );
	}
	else{
		return( (short)daytl[ dmnth ] );
	}
}



