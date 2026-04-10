// DenpScan.cpp : 伝票形式スキャンクラスの実装
#include "stdafx.h"

#include "DBInpSubFunc.h"
#include "DInputSub.h"

#include "DenpRec.h"
#include "DenpScan.h"



//extern SCAN_ITEMEX	SC_Item;
//extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル

// 借方・貸方 ポジションチェック
extern BOOL IsDebtPosition( int pn );
extern BOOL IsCredPosition( int pn );

//extern int GetDataOwnerSign();


CDenpScan::CDenpScan()
{
	m_pZm = NULL;
	m_pDInput = NULL;
	m_daINDEX = 0;
	DenpScanMode() = 0;

//	SyogSiwakeMode() = FALSE;
	SyogSiwakeMode() = TRUE;

	init_sheet( &m_Sheet );

	m_user_att = -1;
}


CDenpScan::~CDenpScan()
{

}

// イニシャライズ
void	CDenpScan::DenpScanInit( CDBZmSub* pZm )
{
	m_pZm = pZm;
}

// イニシャライズ
void	CDenpScan::DenpScanInit( CDInputSub* pDInput, DENP_INPUTPAR* denpara )
{
	m_pDInput = pDInput;
	m_pZm = m_pDInput->m_pZm;
	m_pDENPPAR = denpara;
}


// 内部伝票番号ソート
static
int __cdecl denpcompare(void *context, const void *ele1, const void *ele2 )
{
	int ret;

	DWORD* dw1, *dw2;

	dw1 = (DWORD*)ele1;
	dw2 = (DWORD*)ele2;

	if( *dw1 > *dw2 )			ret = 1;
	else if( *dw1 < *dw2 )		ret = -1;
	else						ret = 0;

	return ret;
}



// 内部伝票番号ソート( 日付順 )
static
int __cdecl ICNODatecompare(void *context, const void *ele1, const void *ele2 )
{
	int ret;

	ICNOINFO* ii1, *ii2;

	ii1 = (ICNOINFO*)ele1;
	ii2 = (ICNOINFO*)ele2;

	if( ii1->ii_mofs > ii2->ii_mofs )	//月オフセット
		ret = 1;
	else if( ii1->ii_mofs < ii2->ii_mofs )
		ret = -1;
	else {
		if( ii1->ii_date > ii2->ii_date )		//日付
			ret = 1;
		else if( ii1->ii_date < ii2->ii_date )
			ret = -1;
		else {
			if( ii1->ii_dpNo < 0 )
				ret = 1;
			else if( ii2->ii_dpNo < 0 )
				ret = -1;
			else if( ii1->ii_dpNo > ii2->ii_dpNo )	//伝票番号
				ret = 1;
			else if( ii1->ii_dpNo < ii2->ii_dpNo )
				ret = -1;
			else {
				if( ii1->ii_icno > ii2->ii_icno )	//内部伝票番号
					ret = 1;
				else if( ii1->ii_icno < ii2->ii_icno )
					ret = -1;
				else {
					ret = 0;
				}
			}
		}
	}

//MyTrace( "SORT(1) ii_icno = %d, date = %d, DPNO = %d\n", ii1->ii_icno, ii1->ii_date, ii1->ii_dpNo );
//MyTrace( "SORT(2) ii_icno = %d, date = %d, DPNO = %d\n", ii2->ii_icno, ii2->ii_date, ii2->ii_dpNo );
//MyTrace( "SORT(3) ret = %d\n", ret );

	return ret;
}



//------------------------------------------------------------
//	仕訳権限 を返送
//	return  1:	他のユーザの仕訳も閲覧可
//	return  0:	自ユーザの仕訳のみ
//------------------------------------------------------------
int CDenpScan::data_authority()
{
	int mode = 0;

	if( m_user_att == -1 ) {
		ICSMCTL	mctrl;
		mctl_usertbl	utbl;

		if( mctrl.SetDatabase() == 0 ) {
			mctrl.get_usertbl( utbl );
			m_user_att = utbl.user_att;	// ユーザ属性
		}
	}

	if( m_user_att != -1 ) {
		if( (m_user_att & 0x11) )
			mode = -2;
	}

	return mode;
}



//-----------------------------------------------
//	伝票修正時の ユーザ種別を返送
//	return -2:	全ユーザ
//	return  0:	カレント
//-----------------------------------------------
int CDenpScan::get_scanuser()
{
#ifdef LATER_CLOSE
	SCAN_ITEMEX *key = &SC_Item;
#endif
	int DtOwner;

	if( ! data_authority() ) {
		// 他ユーザの仕訳の権限なし
		DtOwner = 0;
	}
	else {
		DtOwner = -2;

#ifdef LATER_CLOSE
		if( ! DenpScanMode() ) {
			// 直接　伝票形式にきた場合は、全ユーザ
			DtOwner = -2;
		}
		else {
			// 検索画面より伝票形式にきた場合
			// 対象データ
			DtOwner = GetDataOwnerSign();
		}
#endif
	}

	return DtOwner;
}

// 伝票形式スキャン開始
//	CString filter	検索 SQL
//	WORD	dnpsgn	伝票サイン SC_FURIDEN, SC_NYUKIN, SC_SYUKKIN
//
//	return -1		該当伝票なし
//			0		該当伝票あり
//
int		CDenpScan::DenpScanOpen( CString filter/* = _T( "" )*/, WORD dnpsgn /*=0*/ )
{
	CString buf, tmp, dnptype, wk1, wk2;
	int sgn;
	if( ! filter.IsEmpty() ) {
		tmp.Format( "and %s", filter );
	}

	if( dnpsgn ) {
		sgn = 0;
		char* or = " OR ";

		if( dnpsgn & SC_NYUKIN ) {
			wk2 = "dtype = 0x10";
			sgn++;
		}
		if( dnpsgn & SC_SYUKKIN ) {
			wk1 = "dtype = 0x20";
			if( sgn ) {
				wk2 += or;
				wk2 += wk1;
			}
			else {
				wk2 = wk1;
			}
			sgn++;
		}
		if( dnpsgn & SC_FURIDEN ) {
			wk1 = "dtype = 0x30";
			if( sgn ) {
				wk2 += or;
				wk2 += wk1;
			}
			else {
				wk2 = wk1;
			}
			sgn++;
		}

		if( sgn >= 2 )	dnptype.Format( "and ( %s )", wk2 );
		else			dnptype.Format( "and %s", wk2 );

	}
	else {
		dnptype.Format( "and dtype > 0" );
	}

//	buf.Format( "icno IN select icno from darec where icno > 0 and dtype > 0 %s )", tmp );
	// 取消仕訳は除く
	buf.Format( "(cast(cast(dsign as binary(1)) as int) & 0x01) = 0 and icno > 0 %s %s", dnptype, tmp );

//MyTrace( "denpscan buf = %s\n", buf );
//MyTrace( "--- get_scanuser = %d\n", get_scanuser() );

	int st;
	st = m_pZm->data->Requery( get_scanuser(), 0, buf, 0, 3 );
//	m_pZm->data_Requery() = TRUE;

	m_daICNO.RemoveAll();

	vector<int>	vecICno;

//	CBit	bit;
//	bit.mk_bitbl( m_pZm->m_damax, 0 );	//使用済みicno チェックビットテーブル

	m_ICNOArray.RemoveAll();
	ICNOINFO	icinfo;

	if( st == 0 ) {
		// 内部伝票番号でテーブル作成
		m_pZm->data->MoveFirst();

		if( m_pZm->data->st != -1 ) {
			do {
				sgn = 0;

		//		if( bit.bitncnd( m_pZm->data->icno - 1 ) )
		//			continue;
		//		bit.onbitn( m_pZm->data->icno - 1 );

				if( vector_finder( vecICno, m_pZm->data->icno ) )
					continue;

				vecICno.push_back( m_pZm->data->icno );

				m_daICNO.Add( m_pZm->data->icno );

				// 02.28 /12
				icinfo.ii_icno = m_pZm->data->icno;
				icinfo.ii_date = m_pZm->data->ddate;
				icinfo.ii_mofs = m_pZm->data->mofs;
				icinfo.ii_dpNo = m_pZm->data->cno;
				m_ICNOArray.Add( icinfo );

				m_pDInput->SetInpDate( m_pZm->data->ddate, m_pZm->data->ksign );

			}while( m_pZm->data->MoveNext() == 0 );

			

#ifdef LATER_CLOSE
			SCAN_ITEMEX *key = &SC_Item;
			if( key->sc_type == SCT_ORDERDATE ) {
				m_daICNO.RemoveAll();
				INT_PTR	iacnt = m_ICNOArray.GetCount();
				qsort_s( &m_ICNOArray[0],iacnt, sizeof ICNOINFO, ICNODatecompare, m_pZm );

				for( int n = 0; n < iacnt; n++ ) {
					m_daICNO.Add( m_ICNOArray[n].ii_icno );
				}
			}
			else {
				qsort_s( &m_daICNO[0], m_daICNO.GetCount(), sizeof DWORD, denpcompare, m_pZm );
			}
#else
			m_daICNO.RemoveAll();
			INT_PTR	iacnt = m_ICNOArray.GetCount();
			qsort_s( &m_ICNOArray[0],iacnt, sizeof ICNOINFO, ICNODatecompare, m_pZm );

			for( int n = 0; n < iacnt; n++ ) {
				m_daICNO.Add( m_ICNOArray[n].ii_icno );
			}
#endif
		}
	}

	return st;
}


void	CDenpScan::Close()
{
	m_daINDEX = 0;
	m_daICNO.RemoveAll();

	init_sheet( &m_Sheet );
}


int		CDenpScan::SetFirstSheet()
{
	if( m_daICNO.GetCount() <= 0 )
		return -1;

	m_daINDEX = 0;

	return set_sheet( m_daICNO[m_daINDEX], 0 );
}


//最終伝票の表示
//int dseq: 伝票ダイアログを呼び出した時の仕訳番号
//
int		CDenpScan::SetLastSheet(int dseq)
{
	int cnt;
	cnt = m_daICNO.GetCount();

	if( cnt <= 0 )
		return -1;

	m_daINDEX = cnt-1;
	int st = -1;

	if( set_sheet( m_daICNO[m_daINDEX], 0 ) == 0 ) {
		st = 0;
		// 現伝票に次の同内部伝票番号データがある場合
		if( ! m_Sheet.dp_end ) {
		//	while(1) {
				//指定SEQのページまで検索
				if( set_sheet( m_daICNO[m_daINDEX], 1, dseq) != 0 )
					return -1;

			//	if( m_Sheet.dp_end ) {
					st = 0;
			//		break;
			//	}
		//	}
		}
	}

	return st;
}

//-----------------------------------------------------------
//	次のの伝票をセットする
//	BOOL bNext = TRUE:	現伝票が複数ページの1ページ目以降でも、
//						前の内部伝票番号の伝票をセットする
//
//-----------------------------------------------------------
int		CDenpScan::SetNextSheet( BOOL bNext/*=FALSE*/ )
{
	int cnt, vect = 0;
	cnt = m_daICNO.GetCount();

	if( ! m_Sheet.dp_end && ! bNext ) {
		// 現伝票に次の同内部伝票番号データがある
		vect = 1;
	}
	else {
		//現在データが最終
		if( (m_daINDEX+1) >= cnt )
			return -1;
		
		m_daINDEX++;
	}

	return set_sheet( m_daICNO[m_daINDEX], vect );
}

//-----------------------------------------------------------
//	前の伝票をセットする
//	BOOL bPrev = TRUE:	現伝票が複数ページの1ページ目以降でも、
//						前の内部伝票番号の伝票をセットする
//
//-----------------------------------------------------------
int		CDenpScan::SetPrevSheet( BOOL bPrev/*=FALSE*/ )
{
	if( m_daICNO.GetCount() <= 0 )
		return -1;

	int vect = 0;
	if( m_Sheet.page > 1 && ! bPrev ) {
		// 現伝票に前の同内部伝票番号データがある
		vect = -1;
	}
	else {
		if( m_daINDEX == 0 )
			return -1;
		m_daINDEX--;
	}

	return set_sheet( m_daICNO[m_daINDEX], vect );
}


//-----------------------------------------------------------
//	現在の内部伝票番号の 伝票を 最初のページから表示する
//		データ再表示用
//-----------------------------------------------------------
int	CDenpScan::SetCurrentSheet()
{
	if( m_daICNO.GetCount() <= 0 )
		return -1;

	return set_sheet( m_daICNO[m_daINDEX], 0 );
}


// シートテーブルにデータをセット
//	DWORD	icno	内部伝票番号
//	int		vect	-1 = 戻り, 1 = 送り, 
//					0 = 内部伝票番号を使ってRequery
//
//	return -1 ===> データなし
//
int CDenpScan::set_sheet_old( DWORD icno, int vect )
{
	CString filter;
	int dtype, line;
	int karicnt, kasicnt, page, nowpg, new_page;
	int	dbtdata, credata;
	BOOL bSet;
	char	karittl[6] ={0}, kasittl[6] ={0};
	char	kari[6] ={0}, kasi[6] = {0};

#ifdef KEEP
	int		*karitbl;
	int		*kasitbl;
	int		*tantbl;
	int		*dofstbl;
	int		*cofstbl;
	int		*tofstbl;

	karitbl	=	new int[reccnt];
	kasitbl	=	new int[reccnt];
	tantbl	=	new int[reccnt];
	dofstbl	=	new int[reccnt];
	cofstbl	=	new int[reccnt];
	tofstbl	=	new int[reccnt];
#endif

	page = 1;

	if( vect == 0 ) {
		filter.Format( "(cast(cast(dsign as binary(1)) as int) & 0x01) = 0 and icno = %d", icno );
		m_pZm->data->Requery( get_scanuser(), 0, filter, 0, 0 );	// 日付順から、SEQ順へ
//		m_pZm->data_Requery() = TRUE;
		m_pZm->data->MoveFirst();
	}
	else {
		if( vect == -1 ) {
			page		= m_Sheet.page-1;
			new_page	= m_Sheet.page-1;
			// 戻りはその内部伝票番号の頭から 該当ページまで戻る
			m_pZm->data->MoveFirst();
			m_Sheet.s_seq = m_pZm->data->seq;
		}
		else {
			page		= m_Sheet.page+1;
			new_page	= m_Sheet.page+1;

			m_pZm->data->MoveFirst();

/*
			// 送りはレコードを現伝票の最終SEQ+1の位置へ
			if( m_pZm->data->seq > m_Sheet.e_seq ) {
				while(1) {
					if( m_pZm->data->MovePrev() == -1 )
						break;
					if( m_pZm->data->seq == m_Sheet.e_seq ) {
						m_pZm->data->MoveNext();
						break;
					}
				}
			}
			else {
				while(1) {
					if( m_pZm->data->seq == m_Sheet.e_seq ) {
						m_pZm->data->MoveNext();
						break;
					}
					if( m_pZm->data->MoveNext() == -1 )
						break;
				}
			}
*/
		}
	}

	if( m_pZm->data->st == -1 )	return -1;
	
//	reccnt	= m_pZm->data->GetRecordCount();
	dtype	= m_pZm->data->dtype;
	if( dtype == TYPE_FURIKAE )	line	=	LINE_FURIKAE;
	else						line	=	LINE_NYUSYUKKIN;

	init_sheet( &m_Sheet );
	m_Sheet.icno	= icno;
	m_Sheet.dtype	= dtype;

	CDBINPDataRec rec;
	BOOL	bMax;

	dbtdata = credata = 0;	//該当ページ 借方・貸方 データ数
	karicnt = kasicnt = 0;	//借方・貸方 のページ処理用 データ数
	m_Sheet.dp_end = 1;		//この伝票で終了サインON

	nowpg = 1;
	do {

MAX_CHECK:

		bMax = FALSE;
		if( dtype == TYPE_FURIKAE ) {
			if( karicnt >= line && kasicnt >= line )	// 貸借が合致して、かつMAX行まで登録されている
		//	if( karicnt >= line || kasicnt >= line )	// 貸借が合致しなくとも、借貸 どちらかが、MAX行 で登録されている
				bMax = TRUE;
		}
		else if( dtype == TYPE_NYUKIN ) {
			if( kasicnt >= line )
				bMax = TRUE;
		}
		else {
			if( karicnt >= line )
				bMax = TRUE;
		}

MAX_CHECK2:

		if( bMax ) {	//まだ伝票が続く
			karicnt = kasicnt = 0;

			if( page > 0 ) {
				if( page == nowpg ) {
					m_Sheet.dp_end = 0;
					// 初回は 合計計算のため breakしない
				//	if( vect != 0 )
				//		break;
				}
			}
			else {
				m_Sheet.dp_end = 0;
				break;
			}
			nowpg++;
		}
		if( page > 0 ) {
			bSet = (nowpg == page) ? TRUE : FALSE;
		}
		else	bSet = TRUE;

		m_pDInput->GetCData( &rec );
		//振替伝票
		if( dtype == TYPE_FURIKAE ) {

			if( isSYOGT_CODE( rec.m_dbt ) || 
				isSYOGT_CODE( rec.m_cre ) ) {

				if( ! isSYOGT_CODE( rec.m_dbt ) ) {
					// 行数チェック
					if( karicnt >= line ) {
						bMax = TRUE;
						goto MAX_CHECK2;
					}

					l_add( karittl, karittl, rec.m_val );
					l_add( kari, kari, rec.m_val );
					// 外税は税額＋
					if( sotozei_data( &rec )) {
						l_add( karittl, karittl, rec.m_zei );
						l_add( kari, kari, rec.m_zei );
					}

					if( bSet ) {
						m_Sheet.kari[karicnt]		= rec.m_seq;
						m_Sheet.karisgn[karicnt]	= 1;
						m_Sheet.kariorg[karicnt]	= rec;

						dbtdata++;
					}
					karicnt++;
				}
				if( ! isSYOGT_CODE( rec.m_cre ) ) {
					// 行数チェック
					if( kasicnt >= line ) {
						bMax = TRUE;
						goto MAX_CHECK2;
					}

					l_add( kasittl, kasittl, rec.m_val );
					l_add( kasi, kasi, rec.m_val );
					// 外税は税額＋
					if( sotozei_data( &rec )) {
						l_add( kasittl, kasittl, rec.m_zei );
						l_add( kasi, kasi, rec.m_zei );
					}

					if( bSet ) {

						m_Sheet.kasi[kasicnt]		= rec.m_seq;
						m_Sheet.kasisgn[kasicnt]	= 1;
						m_Sheet.kasiorg[kasicnt]	= rec;

						credata++;
					}
					kasicnt++;
				}

				//貸借金額が一致した所で貸借位置を区切る
				if( !l_jsgn( kari, kasi, sizeof kari ) ) {
					karicnt	=	__max( karicnt, kasicnt );
					kasicnt	=	__max( karicnt, kasicnt );

					::ZeroMemory( kari, sizeof kari );
					::ZeroMemory( kasi, sizeof kasi );
				}
			}
			else {
				// 単一仕訳
				l_add( karittl, karittl, rec.m_val );
				l_add( kasittl, kasittl, rec.m_val );
				// 外税は税額＋
				if( sotozei_data( &rec )) {
					l_add( karittl, karittl, rec.m_zei );
					l_add( kasittl, kasittl, rec.m_zei );
				}

				karicnt	=	__max( karicnt, kasicnt );
				kasicnt	=	__max( karicnt, kasicnt );
				// 単一仕訳の段階で、最大行まで、きていた。
				if( karicnt >= line && kasicnt >= line )
					goto MAX_CHECK;

				if( bSet ) {
					m_Sheet.kari[karicnt]		= rec.m_seq;
					m_Sheet.karisgn[karicnt]	= 1;
					m_Sheet.kariorg[karicnt]	= rec;
				
					m_Sheet.kasi[kasicnt]		= rec.m_seq;
					m_Sheet.kasisgn[kasicnt]	= 1;
					m_Sheet.kasiorg[kasicnt]	= rec;

					dbtdata++;
					credata++;
				}
				karicnt++;
				kasicnt++;
			}
		}
		else {
			if( dtype == TYPE_NYUKIN ) {	// 入金伝票
				l_add( kasittl, kasittl, rec.m_val );
				// 外税は税額＋
				if( sotozei_data( &rec )) {
					l_add( kasittl, kasittl, rec.m_zei );
				}
				if( bSet ) {
					m_Sheet.kasi[kasicnt]		=	rec.m_seq;
					m_Sheet.kasisgn[kasicnt]	=	1;
					m_Sheet.kasiorg[kasicnt]	=	rec;

					credata++;
				}
				kasicnt++;
			}
			else {				// 出金伝票
				l_add( karittl, karittl, rec.m_val );
				// 外税は税額＋
				if( sotozei_data( &rec )) {
					l_add( karittl, karittl, rec.m_zei );
				}

				if( bSet ) {
					m_Sheet.kari[karicnt]		=	rec.m_seq;
					m_Sheet.karisgn[karicnt]	=	1;
					m_Sheet.kariorg[karicnt]	=	rec;

					dbtdata++;
				}
				karicnt++;
			}
		}

		m_Sheet.e_seq = rec.m_seq;

TRACE( "@@ rec.m_seq = %d\n", rec.m_seq );

	} while( m_pZm->data->MoveNext() == 0 );

	if( dtype == TYPE_FURIKAE ) {
		if( vect != 0 ) {
			// 複数ページにわたるとき
			if( dbtdata > 0 || credata > 0 ) {
				m_Sheet.page = new_page;
			}
		}
	}

	memcpy( m_Sheet.karittl, karittl, 6 );
	memcpy( m_Sheet.kasittl, kasittl, 6 );

	return 0;
}

// シートテーブルにデータをセット
//	DWORD	icno	内部伝票番号
//	int		vect	-1 = 戻り, 1 = 送り, 
//					0 = 内部伝票番号を使ってRequery
//	int dseq		伝票ダイアログ表示する、仕訳データSEQ
//
//	return -1 ===> データなし
//
int CDenpScan::set_sheet( DWORD icno, int vect, int dseq )
{
	CString filter;
	int dtype, line, i, j, cnt, numpdmax;
	int tancnt, karicnt, kasicnt, page;
	char	karittl[6] ={0}, kasittl[6] ={0};
	char	kari[6] = {0}, kasi[6] = {0};

	page = 1;

	if( vect == 0 ) {
		filter.Format( "(cast(cast(dsign as binary(1)) as int) & 0x01) = 0 and icno = %d", icno );
//		m_pZm->data->Requery( get_scanuser(), 0, filter, 0, 0 );	// 日付順から、SEQ順へ
		m_pZm->data->Requery( get_scanuser(), 0, filter, 0, 3 );	// 挿入SEQ順へ
//		m_pZm->data_Requery() = TRUE;
		m_pZm->data->MoveFirst();

		m_ShtArray.RemoveAll();
		if( m_pZm->data->st == -1 )	return -1;

		unsigned int	*karitbl;
		unsigned int	*kasitbl;
		unsigned int	*tantbl;
		unsigned short	*dofstbl;
		unsigned short	*cofstbl;
		unsigned short	*tofstbl;

		int	reccnt = m_pZm->data->GetRecordCount();

		if( SyogSiwakeMode() )	reccnt*= 2;

		karitbl	=	new unsigned int[reccnt];
		kasitbl	=	new unsigned int[reccnt];
		tantbl	=	new unsigned int[reccnt];
		dofstbl	=	new unsigned short[reccnt];
		cofstbl	=	new unsigned short[reccnt];
		tofstbl	=	new unsigned short[reccnt];

		dtype	= m_pZm->data->dtype;
#ifdef CLOSE
		if( dtype == TYPE_FURIKAE )	line	=	LINE_FURIKAE;
		else						line	=	LINE_NYUSYUKKIN;
#endif
		line	=	get_MaxLine(dtype);

		CDBINPDataRec rec;

		i		= 1;	//absolutepostion
		page	= 1;
		cnt		=	0;
		tancnt	=	0;
		karicnt	=	0;
		kasicnt	=	0;

		memset( kari, 0, sizeof kari );
		memset( kasi, 0, sizeof kasi );
		memset( karittl, 0, sizeof karittl );
		memset( kasittl, 0, sizeof kasittl );

		memset( karitbl, 0, sizeof(unsigned int) * reccnt );
		memset( kasitbl, 0, sizeof(unsigned int) * reccnt );
		memset( tantbl,  0, sizeof(unsigned int) * reccnt );
		memset( dofstbl, 0xffff, sizeof(unsigned short) * reccnt );
		memset( cofstbl, 0xffff, sizeof(unsigned short) * reccnt );
		memset( tofstbl, 0xffff, sizeof(unsigned short) * reccnt );

		init_sheet( &m_Sheet );
		m_Sheet.icno	= icno;

		do {

			m_pDInput->GetCData( &rec );
			//振替伝票
			if( dtype == TYPE_FURIKAE ) {

				//複合仕訳
				if( isSYOGT_CODE( rec.m_dbt ) || isSYOGT_CODE( rec.m_cre ) ) {

					if( ! SyogSiwakeMode() ) {	//諸口仕訳詰める。
						if( ! isSYOGT_CODE( rec.m_dbt ) ) {
							l_add( kari, kari, rec.m_val );
							l_add( karittl, karittl, rec.m_val );
							// 外税は税額＋
							if( sotozei_data( &rec )) {
								l_add( kari, kari, rec.m_zei );
								l_add( karittl, karittl, rec.m_zei );
							}

							dofstbl[karicnt]	=	i;
							karitbl[karicnt++]	=	rec.m_seq;
						}
						if( ! isSYOGT_CODE( rec.m_cre ) ) {
							l_add( kasi, kasi, rec.m_val );
							l_add( kasittl, kasittl, rec.m_val );

							// 外税は税額＋
							if( sotozei_data( &rec )) {
								l_add( kasi, kasi, rec.m_zei );
								l_add( kasittl, kasittl, rec.m_zei );
							}

							cofstbl[kasicnt]	=	i;
							kasitbl[kasicnt++]	=	rec.m_seq;
						}
						cnt++;

						//貸借金額が一致した所で貸借位置を区切る
						if( !l_jsgn( kari, kasi, sizeof kari ) ) {
							karicnt	=	__max( karicnt, kasicnt );
							kasicnt	=	__max( karicnt, kasicnt );
						}

						if( tancnt ) {
							//貸借金額が一致した所で貸借位置を区切って単一仕訳を追加
							if( !l_jsgn( kari, kasi, sizeof kari ) ) {
								karicnt	=	__max( karicnt, kasicnt );
								kasicnt	=	__max( karicnt, kasicnt );

								for( j = 0; j < tancnt; j++ ) {
									dofstbl[karicnt]	=	tofstbl[j];
									karitbl[karicnt++]	=	tantbl[j];
									cofstbl[kasicnt]	=	tofstbl[j];
									kasitbl[kasicnt++]	=	tantbl[j];
								}
								tancnt	=	0;

								memset( kari, 0, sizeof kari );
								memset( kasi, 0, sizeof kasi );
								cnt	=	0;
							}
						}
					}
					else {	//諸口仕訳詰めない
						if( ! isSYOGT_CODE( rec.m_dbt ) ) {
							l_add( kari, kari, rec.m_val );
							l_add( karittl, karittl, rec.m_val );
							// 外税は税額＋
							if( sotozei_data( &rec )) {
								l_add( kari, kari, rec.m_zei );
								l_add( karittl, karittl, rec.m_zei );
							}

							dofstbl[karicnt]	=	i;
							karitbl[karicnt]	=	rec.m_seq;
							karicnt++;
							kasicnt++;
						}
						else {
							l_add( kasi, kasi, rec.m_val );
							l_add( kasittl, kasittl, rec.m_val );

							// 外税は税額＋
							if( sotozei_data( &rec )) {
								l_add( kasi, kasi, rec.m_zei );
								l_add( kasittl, kasittl, rec.m_zei );
							}

							cofstbl[kasicnt]	=	i;
							kasitbl[kasicnt]	=	rec.m_seq;
							karicnt++;
							kasicnt++;
						}
						cnt++;
					}
				}
				//単一仕訳
				else {
					l_add( karittl, karittl, rec.m_val  );
					l_add( kasittl, kasittl, rec.m_val  );

					// 外税は税額＋
					if( sotozei_data( &rec )) {
						l_add( karittl, karittl, rec.m_zei );
						l_add( kasittl, kasittl, rec.m_zei );
					}

					if( cnt ) {
						//貸借金額が一致した所で貸借位置を区切って単一仕訳を追加
						if( !l_jsgn( kari, kasi, sizeof kari ) ) {
							karicnt	=	__max( karicnt, kasicnt );
							kasicnt	=	__max( karicnt, kasicnt );
							dofstbl[karicnt]	=	i;
							karitbl[karicnt++]	=	rec.m_seq;
							cofstbl[kasicnt]	=	i;
							kasitbl[kasicnt++]	=	rec.m_seq;

							memset( kari, 0, sizeof kari );
							memset( kasi, 0, sizeof kasi );
							cnt	=	0;
						}
						else {
							tofstbl[tancnt]		=	i;
							tantbl[tancnt++]	=	rec.m_seq;;
						}
					}
					else {
						dofstbl[karicnt]	=	i;
						karitbl[karicnt++]	=	rec.m_seq;;
						cofstbl[kasicnt]	=	i;
						kasitbl[kasicnt++]	=	rec.m_seq;;
					}
				}
			}
			//入金、出金伝票
			else {

				if( dtype == TYPE_NYUKIN ) {
					l_add( kasittl, kasittl, rec.m_val );
					// 外税は税額＋
					if( sotozei_data( &rec )) {
						l_add( kasittl, kasittl, rec.m_zei );
					}

					m_Sheet.kasi[karicnt]	=	rec.m_seq;;
					m_Sheet.kasisgn[karicnt]	=	1;
					m_Sheet.kasiorg[karicnt]	=	rec;
					karicnt++;
				}
				else {	// 出金伝票
					l_add( karittl, karittl, rec.m_val );
					// 外税は税額＋
					if( sotozei_data( &rec )) {
						l_add( karittl, karittl, rec.m_zei );
					}

					m_Sheet.kari[karicnt]	=	rec.m_seq;;
					m_Sheet.karisgn[karicnt]	=	1;
					m_Sheet.kariorg[karicnt]	=	rec;
					karicnt++;
				}

				m_Sheet.e_seq = rec.m_seq;

				if( karicnt >= line ) {
					m_Sheet.dtype	=	dtype;
					m_Sheet.page	=	page;
					page++;

					m_ShtArray.Add( m_Sheet );
					init_sheet( &m_Sheet );
					m_Sheet.icno	= icno;

					karicnt	=	0;
				}
			}	//End of 入金、出金伝票

			i++;

		} while( m_pZm->data->MoveNext() == 0 );

		if( dtype != TYPE_FURIKAE ) {
			if( karicnt ) {
				m_Sheet.dtype	=	dtype;
				m_Sheet.icno	= icno;

				m_Sheet.page	=	page;
				m_ShtArray.Add( m_Sheet );
			}
		}
		else {
			if( tancnt ) {
				karicnt	=	__max( karicnt, kasicnt );
				kasicnt	=	__max( karicnt, kasicnt );

				for( j = 0; j < tancnt; j++ ) {
					dofstbl[karicnt]	=	tofstbl[j];
					karitbl[karicnt++]	=	tantbl[j];
					cofstbl[kasicnt]	=	tofstbl[j];
					kasitbl[kasicnt++]	=	tantbl[j];
				}
			}

			karicnt	=	__max( karicnt, kasicnt );
			kasicnt	=	__max( karicnt, kasicnt );

			numpdmax	=	(karicnt + line - 1) / line;

			for( j = 0, cnt = 0; j < karicnt; j++ ) {
				m_Sheet.kari[cnt]	=	karitbl[j];
				if( m_Sheet.kari[cnt] ) {
					m_Sheet.karisgn[cnt]	=	1;

					m_pZm->data->SetAbsolutePosition( dofstbl[j] );
					m_pDInput->GetCData( &rec );
					m_Sheet.kariorg[cnt]	=	rec;
				}
				m_Sheet.kasi[cnt]	=	kasitbl[j];
				if( m_Sheet.kasi[cnt] ) {
					m_Sheet.kasisgn[cnt]	=	1;

					m_pZm->data->SetAbsolutePosition( cofstbl[j] );
					m_pDInput->GetCData( &rec );
					m_Sheet.kasiorg[cnt]	=	rec;
				}
				cnt++;

				if( cnt == line ) {
					m_Sheet.dtype	=	dtype;
					m_Sheet.page	=	page;
					page++;
					memmove( m_Sheet.karittl, karittl, sizeof karittl );
					memmove( m_Sheet.kasittl, kasittl, sizeof kasittl );

					m_ShtArray.Add( m_Sheet );

					init_sheet( &m_Sheet );
					m_Sheet.icno	= icno;
					cnt	=	0;
				}
			}
			if( cnt ) {
				m_Sheet.dtype	=	dtype;
				m_Sheet.page	=	page;

				memmove( m_Sheet.karittl, karittl, sizeof karittl );
				memmove( m_Sheet.kasittl, kasittl, sizeof kasittl );

				m_ShtArray.Add( m_Sheet );
				init_sheet( &m_Sheet );
				m_Sheet.icno	= icno;
			}
		}

		delete [] karitbl;
		delete [] kasitbl;
		delete [] tantbl;
		delete [] dofstbl;
		delete [] cofstbl;
		delete [] tofstbl;

		cnt = m_ShtArray.GetCount();
		m_ShtArray[cnt-1].dp_end = 1;		//この伝票で終了サインON

		if( cnt > 1 ) {
			for( int n = 0; n < (cnt-1); n++ ) {
				memcpy( m_ShtArray[n].karittl, m_ShtArray[cnt-1].karittl, 6 );
				memcpy( m_ShtArray[n].karittl, m_ShtArray[cnt-1].kasittl, 6 );
			}
		}

		m_Sheet = m_ShtArray[0];

	}
	else {
		if( vect == -1 ) {
			page	= m_Sheet.page-1;

			if( page <= 0 )	page = 1;
			m_Sheet = m_ShtArray[page-1];
		}
		else {
			int find_idx = -1;
			if (dseq != -1) {
				page = m_ShtArray.GetCount();
				//仕訳SEQと一致する伝票番号
				for (int n = 0; n < page; n++) {
					SHEET_TABLE& sht = m_ShtArray[n];
					for (int i = 0; i < DP_SCROLL_MAX; i++) {
						if (sht.kari[i] == dseq || sht.kasi[i] == dseq) {
							find_idx = n;
							break;
						}
					}
					if (find_idx != -1) {
						break;
					}
				}
			}

			if(find_idx == -1 ) {
				page = m_Sheet.page + 1;
				if (page > m_ShtArray.GetCount())
					page = m_ShtArray.GetCount();

				m_Sheet = m_ShtArray[page - 1];
			}
			else {
				m_Sheet = m_ShtArray[find_idx];
			}
		}
	}

//	memcpy( m_Sheet.karittl, karittl, 6 );
//	memcpy( m_Sheet.kasittl, kasittl, 6 );

	return 0;
}



// シートテーブル初期化
void  CDenpScan::init_sheet( SHEET_TABLE* tbl )
{
	tbl->icno	= 0;
	tbl->dtype	= 0;
	tbl->s_seq	= 0;
	tbl->e_seq	= 0;
	tbl->page	= 1;
	tbl->dp_end = 0;
	tbl->newcnt = 0;

	::ZeroMemory( tbl->karittl, sizeof tbl->karittl );
	::ZeroMemory( tbl->kasittl, sizeof tbl->kasittl );

	for( int i = 0; i < DP_SCROLL_MAX; i++ ) {
		tbl->kari[i]		= 0;
		tbl->kasi[i]		= 0;
		tbl->karisgn[i]		= 0;
		tbl->kasisgn[i]		= 0;
		tbl->delsgn[i]		= 0;
		tbl->inssgn[i]		= 0;

		tbl->kariorg[i].Reset();
		tbl->kasiorg[i].Reset();

		tbl->karirec[i].Reset();
		tbl->kasirec[i].Reset();

		tbl->addkari[i] = 0;
		tbl->addkasi[i] = 0;
		tbl->delkari[i] = 0;
		tbl->delkasi[i] = 0;

		::ZeroMemory( &tbl->zeikari[i], sizeof tbl->zeikari[0] );
		::ZeroMemory( &tbl->zeikasi[i], sizeof tbl->zeikasi[0] );
	}
}


CDBINPDataRec& CDenpScan::GetOriginData( int ln, int karikasi )
{
	if( karikasi ) {
		return	m_Sheet.kasiorg[ln-1];
	}
	else {
		return	m_Sheet.kariorg[ln-1];
	}
}

// 02.20 /08
// もとのデータを更新
//
void CDenpScan::SetOriginData( int ln, int karikasi, CDBINPDataRec& rec )
{
	if( GetSheetCount() > 1 ) {	//複数ページ対応 --- 03.11 /10 ---
		int idx = m_Sheet.page - 1;

		if( karikasi ) {
			m_ShtArray[idx].kasiorg[ln-1] = rec;
		}
		else {
			m_ShtArray[idx].kariorg[ln-1] = rec;
		}
	}
	else {
		if( karikasi ) {
			m_Sheet.kasiorg[ln-1] = rec;
		}
		else {
			m_Sheet.kariorg[ln-1] = rec;
		}
	}
}


//
//	伝票修正中に他のプログラムに移行した場合、追加仕訳分をスキャン仕訳と扱う
//
void CDenpScan::SheetTableUpdate( int newcnt, CDBINPDataRec* rec, ADDNEWTBL* antbl )
{

	int i, ln, seq = -1;

	SHEET_TABLE* pSheet;

	int loop, lpmax, cnt;
	cnt = m_ShtArray.GetCount();
	if( cnt >= 1 ) {
		lpmax = 2;
	}
	else {
		lpmax = 1;
	}

	for( loop = 0; loop < lpmax; loop++ ) {
		if( loop == 0 ) {
			pSheet = &m_Sheet;
		}
		else {
			pSheet = &m_ShtArray[m_Sheet.page-1];
		}

		for( i = 0; i < newcnt; i++ ) {
			ln = antbl[i].ln;

			if( seq < (rec+i)->m_seq )
				seq = (rec+i)->m_seq;

			switch( antbl[i].karikasi ) {
			case 0:
				pSheet->kari[ln-1]		= (rec+i)->m_seq;
				pSheet->karisgn[ln-1]	= 1;
				pSheet->kariorg[ln-1]	= *(rec+i);
				pSheet->addkari[ln-1]	= 0;
				::ZeroMemory( &pSheet->zeikari[ln-1], sizeof SHEET_SYKBN );
				break;
			case 1:
				pSheet->kasi[ln-1]		= (rec+i)->m_seq;
				pSheet->kasisgn[ln-1]	= 1;
				pSheet->kasiorg[ln-1]	= *(rec+i);
				pSheet->addkasi[ln-1]	= 0;
				::ZeroMemory( &pSheet->zeikasi[ln-1], sizeof SHEET_SYKBN );
				break;
			case 2:
				pSheet->kari[ln-1]		= (rec+i)->m_seq;
				pSheet->karisgn[ln-1]	= 1;
				pSheet->kariorg[ln-1]	= *(rec+i);
				pSheet->addkari[ln-1]	= 0;
				::ZeroMemory( &pSheet->zeikari[ln-1], sizeof SHEET_SYKBN );

				pSheet->kasi[ln-1]		= (rec+i)->m_seq;
				pSheet->kasisgn[ln-1]	= 1;
				pSheet->kasiorg[ln-1]	= *(rec+i);
				pSheet->addkasi[ln-1]	= 0;
				::ZeroMemory( &pSheet->zeikasi[ln-1], sizeof SHEET_SYKBN );
				break;
			}
		}

		if( pSheet->e_seq < seq )
			pSheet->e_seq = seq;

		pSheet->newcnt -= newcnt;
		if( pSheet->newcnt < 0 )	pSheet->newcnt = 0;
	}
}



void CDenpScan::GetCrctData( int ln, int karikasi, CDBINPDataRec* drec )
{
	if( karikasi ) {
		*drec = m_Sheet.kasirec[ln-1];
	}
	else {
		*drec = m_Sheet.karirec[ln-1];
	}
}

// スキャンで読み込んだデータか？
int CDenpScan::IsOriginData( int ln, int karikasi )
{
	if( karikasi )
		return	m_Sheet.kasisgn[ln-1];
	else
		return	m_Sheet.karisgn[ln-1];
}

// 修正された仕訳かどうか？
int CDenpScan::IsCrctData( int ln, int karikasi )
{
	if( IsOriginData( ln, karikasi ) ) {
		if( karikasi ) {
			if( m_Sheet.kasirec[ln-1].m_ddate > 0 )	return	1;
		}
		else {
			if( m_Sheet.karirec[ln-1].m_ddate > 0 )	return	1;
		}
	}
	return	0;
}

// 新たに伝票に追加された仕訳か？
BOOL CDenpScan::IsAddData( int ln, int pn )
{
	if( !pn ) {
		if( m_Sheet.addkari[ln-1] )	return	TRUE;
	}
	else {
		if( m_Sheet.addkasi[ln-1] )	return	TRUE;
	}
	return	FALSE;
}

// 削除された仕訳か？
int CDenpScan::IsDeleteData( int ln, int pn )
{
	if( pn ) {
		return	m_Sheet.delkasi[ln-1];
	}
	else {
		return	m_Sheet.delkari[ln-1];
	}
}


BOOL CDenpScan::IsFurikaeEnd( int vect )
{
	if( m_Sheet.dtype == TYPE_FURIKAE ) {
		if( vect < 0 ) {
			if( m_Sheet.page == 1 )	return	TRUE;
		}
		else {
			if( m_Sheet.dp_end )	return	TRUE;
		}
	}
	return	FALSE;
}

//元の状態が複合仕訳かどうか
BOOL CDenpScan::IsFukugoSiwake( int ln )
{
	if( m_Sheet.dtype == TYPE_FURIKAE ) {
		int idx = ln-1;

		//諸口を詰めない場合は、複合仕訳としない。
		if( SyogSiwakeMode() )	return FALSE;

		if( m_Sheet.karisgn[idx] && m_Sheet.kasisgn[idx] ) {
			// SEQ番号が異なる場合は複合仕訳
			if( m_Sheet.kari[idx] == m_Sheet.kasi[idx] )	return	FALSE;
			else											return	TRUE;
		}
		if( m_Sheet.karisgn[idx] || m_Sheet.kasisgn[idx] )	return	TRUE;
	}

	return	FALSE;
}



int CDenpScan::GetSeq( int ln, int karikasi )
{
	int idx = ln-1;

	if( karikasi ) {
		if( m_Sheet.addkasi[idx] )	return	-1;
		return	m_Sheet.kasi[idx];
	}
	else {
		if( m_Sheet.addkari[idx] )	return	-1;
		return	m_Sheet.kari[idx];
	}
}

//-------------------------------------------------------------------------
//	現在処理中の内部伝票番号の伝票の枚数
//	1 = 1枚
//  2 = 2枚以上(データを全部読むわけではないので正確な数はわからない)
//-------------------------------------------------------------------------
int CDenpScan::GetSheetCount()
{
	if( m_Sheet.page == 1 && m_Sheet.dp_end )
		return 1;
	else
		return 2;
}


//-------------------------------------------------------------
//	伝票の合計金額を
//	mode	0 = 借方, 1 = 貸方, 2 = 両方
//
//-------------------------------------------------------------
void CDenpScan::SetSheetTotal( unsigned char* kari, unsigned char* kasi )
{
	//トータル金額書き込み
	memcpy( m_Sheet.karittl, kari, sizeof m_Sheet.karittl );
	memcpy( m_Sheet.kasittl, kasi, sizeof m_Sheet.kasittl );

	int cnt = m_ShtArray.GetCount();

	if( cnt >= 0 ) {
		for( int n = 0; n < cnt; n++ ) {
			memcpy( m_ShtArray[n].karittl, kari, sizeof m_Sheet.karittl );
			memcpy( m_ShtArray[n].kasittl, kasi, sizeof m_Sheet.kasittl );
		}
	}
}


//-------------------------------------------------------------
//	伝票修正時の仕訳追加
//	mode	0 = 借方, 1 = 貸方, 2 = 両方
//
//-------------------------------------------------------------
void CDenpScan::AddSheetTable( int ln, int mode, CDBINPDataRec* drec, struct __SYDATA* syd )
{

	int idx = ln-1;

	SHEET_TABLE* pSheet;

	int loop, lpmax, cnt;
	cnt = m_ShtArray.GetCount();
	if( cnt >= 1 ) {
		lpmax = 2;
	}
	else {
		lpmax = 1;
	}

	for( loop = 0; loop < lpmax; loop++ ) {
		if( loop == 0 ) {
			pSheet = &m_Sheet;
		}
		else {
			pSheet = &m_ShtArray[m_Sheet.page-1];
		}

		if( mode == 0 || mode == 2 ) {
			if( drec == NULL ) {
				pSheet->karirec[idx].Reset();
				pSheet->addkari[idx]	=	0;
				::ZeroMemory( &pSheet->zeikari[idx], sizeof pSheet->zeikari[0] );
				pSheet->delkari[idx]	=	0;
			}
			else {
				pSheet->karirec[idx]	=	*drec;
				pSheet->addkari[idx]	=	1;
				pSheet->zeikari[idx].swk_sw	=	syd->swk_sw;
				pSheet->zeikari[idx].ritu_sw	=	syd->ritu_sw;
				pSheet->zeikari[idx].zeikbn_sw	=	syd->zeikbn_sw;
				pSheet->zeikari[idx].uri_sw		=	syd->uri_sw;
				pSheet->zeikari[idx].toku_sw	=	syd->toku_sw;
				pSheet->zeikari[idx].kts_sw	=	syd->kts_sw;
				
				pSheet->delkari[idx]	=	0;
			}
		}
		if( mode == 1 || mode == 2 ) {
			if( drec == NULL ) {
				pSheet->kasirec[idx].Reset();
				pSheet->addkasi[idx]	=	0;
				::ZeroMemory( &pSheet->zeikasi[idx], sizeof pSheet->zeikasi[0] );
				pSheet->delkasi[idx]	=	0;
			}
			else {
				pSheet->kasirec[idx]	=	*drec;
				pSheet->addkasi[idx]	=	1;
				pSheet->zeikasi[idx].swk_sw	=	syd->swk_sw;
				pSheet->zeikasi[idx].ritu_sw	=	syd->ritu_sw;
				pSheet->zeikasi[idx].zeikbn_sw	=	syd->zeikbn_sw;
				pSheet->zeikasi[idx].uri_sw		=	syd->uri_sw;
				pSheet->zeikasi[idx].toku_sw	=	syd->toku_sw;
				pSheet->zeikasi[idx].kts_sw	=	syd->kts_sw;

				pSheet->delkasi[idx]	=	0;
			}
		}
	}

#ifdef OLD_CLOSE // 04.24 /12
	if( mode == 0 || mode == 2 ) {
		if( drec == NULL ) {
			m_Sheet.karirec[idx].Reset();
			m_Sheet.addkari[idx]	=	0;
			::ZeroMemory( &m_Sheet.zeikari[idx], sizeof m_Sheet.zeikari[0] );
			m_Sheet.delkari[idx]	=	0;
		}
		else {
			m_Sheet.karirec[idx]	=	*drec;
			m_Sheet.addkari[idx]	=	1;
			m_Sheet.zeikari[idx].swk_sw	=	syd->swk_sw;
			m_Sheet.zeikari[idx].ritu_sw	=	syd->ritu_sw;
			m_Sheet.zeikari[idx].zeikbn_sw	=	syd->zeikbn_sw;
			m_Sheet.zeikari[idx].uri_sw		=	syd->uri_sw;
			m_Sheet.zeikari[idx].toku_sw	=	syd->toku_sw;
			m_Sheet.zeikari[idx].kts_sw	=	syd->kts_sw;
			
			m_Sheet.delkari[idx]	=	0;
		}
	}
	if( mode == 1 || mode == 2 ) {
		if( drec == NULL ) {
			m_Sheet.kasirec[idx].Reset();
			m_Sheet.addkasi[idx]	=	0;
			::ZeroMemory( &m_Sheet.zeikasi[idx], sizeof m_Sheet.zeikasi[0] );
			m_Sheet.delkasi[idx]	=	0;
		}
		else {
			m_Sheet.kasirec[idx]	=	*drec;
			m_Sheet.addkasi[idx]	=	1;
			m_Sheet.zeikasi[idx].swk_sw	=	syd->swk_sw;
			m_Sheet.zeikasi[idx].ritu_sw	=	syd->ritu_sw;
			m_Sheet.zeikasi[idx].zeikbn_sw	=	syd->zeikbn_sw;
			m_Sheet.zeikasi[idx].uri_sw		=	syd->uri_sw;
			m_Sheet.zeikasi[idx].toku_sw	=	syd->toku_sw;
			m_Sheet.zeikasi[idx].kts_sw	=	syd->kts_sw;

			m_Sheet.delkasi[idx]	=	0;
		}
	}
#endif
}

//--------------------------------------------------------------
//		消費税サインを返送
//
//--------------------------------------------------------------
SHEET_SYKBN&	CDenpScan::GetSheetSykbn( int ln, int karikasi )
{
	if( karikasi ) {
		return m_Sheet.zeikasi[ln-1];
	}
	else {
		return m_Sheet.zeikari[ln-1];
	}
}


// 03.11 /10
//
void CDenpScan::SetOriginValttl()
{
	CDBINPDataRec* pd;

	if( GetSheetCount() > 1 ) {	//複数ページ対応 --- 03.11 /10 ---
		INT_PTR max = m_ShtArray.GetCount();

		char karittl[6], kasittl[6];

		::ZeroMemory( karittl, sizeof karittl );
		::ZeroMemory( kasittl, sizeof kasittl );

		for( int idx = 0; idx < max; idx++ ) {
			int line = get_MaxLine( m_ShtArray[idx].dtype );

#ifdef OLD_CLOSE
			if( m_ShtArray[idx].dtype == TYPE_FURIKAE )	
				line	=	LINE_FURIKAE;
			else
				line	=	LINE_NYUSYUKKIN;
#endif

			for( int i = 0; i < line; i++ ) {
				if( m_ShtArray[idx].karisgn[i] ) {
					pd = &m_ShtArray[idx].kariorg[i];
					l_add( karittl, karittl, pd->m_val );
					// 外税は税額＋
					if( sotozei_data( pd )) {
						l_add( karittl, karittl, pd->m_zei );
					}
				}

				if( m_ShtArray[idx].kasisgn[i] ) {
					pd = &m_ShtArray[idx].kasiorg[i];
					l_add( kasittl, kasittl, pd->m_val );
					// 外税は税額＋
					if( sotozei_data( pd )) {
						l_add( kasittl, kasittl, pd->m_zei );
					}
				}
			}
		}
		for( int idx = 0; idx < max; idx++ ) {
			memcpy( m_ShtArray[idx].karittl, karittl, sizeof karittl );
			memcpy( m_ShtArray[idx].kasittl, kasittl, sizeof kasittl );
		}
	}
}


//
//	伝票削除処理
//
int CDenpScan::DenpDelelteJob()
{
	SHEET_TABLE	shTmp;

	INT_PTR	cnt;
	int i, ln, ret, seq;

	CDBINPDataRec	data;

	cnt = m_ShtArray.GetCount();

	for( i = 0; i < cnt; i++ ) {
		shTmp = m_ShtArray[i];

		int maxLine = get_MaxLine( shTmp.dtype );

		for( ln = 1; ln <= maxLine; ln++ ) {

			seq = -1;

			if( shTmp.karisgn[ln-1] ) {
				data = shTmp.kariorg[ln-1];
				data.m_dsign[0] |= 0x01;
				ret = m_pDInput->DB_DataCorrect( &data );

				if( ret != -2 ) {
					m_pDENPPAR->mdfseq.push_back( data.m_seq );
				//	MODEFY_BIT.onbitn( data.m_seq - 1);
				}

				seq = shTmp.kari[ln-1];
			}

			if( shTmp.kasisgn[ln-1] ) {
				if( shTmp.dtype == TYPE_FURIKAE ) {
					//振替伝票の単一仕訳
					if( seq != -1 && seq == shTmp.kasi[ln-1] ) {
						continue;
					}
				}

				data = shTmp.kasiorg[ln-1];
				data.m_dsign[0] |= 0x01;
				ret = m_pDInput->DB_DataCorrect( &data );
				if( ret != -2 ) {
					m_pDENPPAR->mdfseq.push_back( data.m_seq );
				//	MODEFY_BIT.onbitn( data.m_seq - 1);
				}
			}
		}
	}
	return 0;
}


//
//	既入力伝票への１ライン 挿入処理
//
int CDenpScan::InsertDenpLine( int ln )
{
	int cnt = m_ShtArray.GetCount();
	SHEET_TABLE*	ptbl;
	SHEET_TABLE		tmpSht, prevSht;

	if( cnt > 1 ) {
		ptbl = &m_ShtArray[cnt-1];

		if( LastLineCheck( ptbl ) )
			return 0;

		int shtidx = m_Sheet.page -1;

		init_sheet( &prevSht );

		if( ! m_Sheet.dp_end ) {
			//伝票最終ページでない

			for( int nSht = shtidx; nSht < cnt; nSht++ ) {
				ptbl = &m_ShtArray[nSht];
				tmpSht = m_ShtArray[nSht];

				if( nSht == shtidx ) {
					InsertLineJob( ln, ptbl, NULL );
				}
				else {
					InsertLineJob( 1, ptbl, &prevSht );
				}
				// 次伝票の先頭行用に 前ページを保存
				prevSht = tmpSht;
			}
		}
		else {
			//伝票最終ページ
			InsertLineJob( ln, &m_ShtArray[shtidx], NULL );
		}

		// 現在データ更新
		m_Sheet = m_ShtArray[shtidx];
	}
	else {
		if( LastLineCheck( &m_Sheet ) )
			return 0;

		InsertLineJob( ln, &m_Sheet, NULL );
	}

	return 0;
}


//--------------------------------------------------------------------------------
//	伝票の指定行にデータを挿入する
//		int ln				挿入する行
//		SHEET_TABLE* ptb	挿入する伝票テーブル
//		SHEET_TABLE* prev	挿入する行が 1行目で、この変数が
//							NULLでない場合は、このてーぶるの最終行を
//							1行目にセットする(複数ページで挿入ページでない時
//
//--------------------------------------------------------------------------------
void CDenpScan::InsertLineJob( int ln, SHEET_TABLE* ptbl,  SHEET_TABLE* prev )
{
	int maxLine = get_MaxLine( ptbl->dtype );

	int lidx = (maxLine-1);

	for( int i = lidx; i >= (ln-1); i-- ) {
		if( i == (ln-1) ) {
			if( ln == 1 && prev != NULL ) {
				ptbl->kari[i]		= prev->kari[lidx];
				ptbl->kasi[i]		= prev->kasi[lidx];
				ptbl->karisgn[i]	= prev->karisgn[lidx];
				ptbl->kasisgn[i]	= prev->kasisgn[lidx];
				ptbl->delsgn[i]		= prev->delsgn[lidx];
				ptbl->inssgn[i]		= prev->inssgn[lidx];

				ptbl->kariorg[i]	= prev->kariorg[lidx];
				ptbl->kasiorg[i]	= prev->kasiorg[lidx];

				ptbl->karirec[i]	= prev->karirec[lidx];
				ptbl->kasirec[i]	= prev->kasirec[lidx];

				ptbl->addkari[i]	= prev->addkari[lidx];
				ptbl->addkasi[i]	= prev->addkasi[lidx];
				ptbl->delkari[i]	= prev->delkari[lidx];
				ptbl->delkasi[i]	= prev->delkasi[lidx];

				memcpy( &ptbl->zeikari[i], &prev->zeikari[lidx], sizeof ptbl->zeikari[0] );
				memcpy( &ptbl->zeikasi[i], &prev->zeikasi[lidx], sizeof ptbl->zeikasi[0] );
			}
			else {
				ptbl->kari[i]		= 0;
				ptbl->kasi[i]		= 0;
				ptbl->karisgn[i]		= 0;
				ptbl->kasisgn[i]		= 0;
				ptbl->delsgn[i]		= 0;
				ptbl->inssgn[i]		= 1;
				ptbl->kariorg[i].Reset();
				ptbl->kasiorg[i].Reset();

				ptbl->karirec[i].Reset();
				ptbl->kasirec[i].Reset();

				ptbl->addkari[i] = 0;
				ptbl->addkasi[i] = 0;
				ptbl->delkari[i] = 0;
				ptbl->delkasi[i] = 0;

				::ZeroMemory( &ptbl->zeikari[i], sizeof ptbl->zeikari[0] );
				::ZeroMemory( &ptbl->zeikasi[i], sizeof ptbl->zeikasi[0] );
			}
		}
		else {
			ptbl->kari[i]		= ptbl->kari[i-1];
			ptbl->kasi[i]		= ptbl->kasi[i-1];
			ptbl->karisgn[i]	= ptbl->karisgn[i-1];
			ptbl->kasisgn[i]	= ptbl->kasisgn[i-1];
			ptbl->delsgn[i]		= ptbl->delsgn[i-1];
			ptbl->inssgn[i]		= ptbl->inssgn[i-1];

			ptbl->kariorg[i]	= ptbl->kariorg[i-1];
			ptbl->kasiorg[i]	= ptbl->kasiorg[i-1];

			ptbl->karirec[i]	= ptbl->karirec[i-1];
			ptbl->kasirec[i]	= ptbl->kasirec[i-1];

			ptbl->addkari[i]	= ptbl->addkari[i-1];
			ptbl->addkasi[i]	= ptbl->addkasi[i-1];
			ptbl->delkari[i]	= ptbl->delkari[i-1];
			ptbl->delkasi[i]	= ptbl->delkasi[i-1];

			memcpy( &ptbl->zeikari[i], &ptbl->zeikari[i-1], sizeof ptbl->zeikari[0] );
			memcpy( &ptbl->zeikasi[i], &ptbl->zeikasi[i-1], sizeof ptbl->zeikasi[0] );
		}
	}
}


//--------------------------------------------------------------
//	伝票の最終行にデータがあるか？(挿入チェックのため)
//
//--------------------------------------------------------------
int CDenpScan::LastLineCheck( SHEET_TABLE* ptb )
{
	int maxLine = get_MaxLine( ptb->dtype );
	int chk = maxLine-1;
	int ret = 0;

	if( ptb->karisgn[chk] || ptb->kasisgn[chk] ) {
		ret++;
	}

	return ret;
}

// 挿入行か？
BOOL CDenpScan::IsInsertLine( int ln )
{
	return	m_Sheet.inssgn[ln-1];
}


//
//	既入力伝票への１ライン 削除処理
//
int CDenpScan::DeleteDenpLine( int ln )
{
	int cnt = m_ShtArray.GetCount();
	SHEET_TABLE*	ptbl;
	SHEET_TABLE		tmpSht, nextSht;

	if( cnt > 1 ) {
		int shtidx = m_Sheet.page -1;

		ptbl = &m_ShtArray[shtidx];

		if( ! ptbl->inssgn[ln-1] )
			return 0;

		init_sheet( &nextSht );

		if( ! m_Sheet.dp_end ) {
			//伝票最終ページでない

			for( int nSht = shtidx; nSht < cnt; nSht++ ) {
				ptbl = &m_ShtArray[nSht];
				tmpSht = m_ShtArray[nSht];

				//次伝票
				if( nSht < (cnt-1) ) {
					nextSht = m_ShtArray[nSht+1];
				}

				if( nSht == shtidx ) {
					DeleteLineJob( ln, ptbl, &nextSht );
				}
				else if( nSht == (cnt-1) ) {
					DeleteLineJob( 1, ptbl, NULL );
				}
				else {
					DeleteLineJob( 1, ptbl, &nextSht );
				}
			}
		}
		else {
			//伝票最終ページ
			InsertLineJob( ln, &m_ShtArray[shtidx], NULL );
		}

		// 現在データ更新
		m_Sheet = m_ShtArray[shtidx];
	}
	else {
		if( ! IsInsertLine( ln ) )
			return 0;

		DeleteLineJob( ln, &m_Sheet, NULL );
	}

	return 0;
}


//--------------------------------------------------------------------------------
//	伝票の指定行から１ラインデータを削除する
//		int ln				削除する行
//		SHEET_TABLE* ptb	削除する伝票テーブル
//		SHEET_TABLE* prev	削除する行が 最終行で、この変数が
//							NULLでない場合は、このバッファの先頭行を
//							最終行にセットする(複数ページ時)
//
//--------------------------------------------------------------------------------
void CDenpScan::DeleteLineJob( int ln, SHEET_TABLE* ptbl,  SHEET_TABLE* next )
{
	int maxLine = get_MaxLine( ptbl->dtype );
	int idx_max = (maxLine-1);

	for( int i = (ln-1); i <= idx_max; i++ ) {
		if( i == idx_max ) {	//最終行
			if( next != NULL ) {
				ptbl->kari[i]		= next->kari[0];
				ptbl->kasi[i]		= next->kasi[0];
				ptbl->karisgn[i]	= next->karisgn[0];
				ptbl->kasisgn[i]	= next->kasisgn[0];
				ptbl->delsgn[i]		= next->delsgn[0];
				ptbl->inssgn[i]		= next->inssgn[0];

				ptbl->kariorg[i]	= next->kariorg[0];
				ptbl->kasiorg[i]	= next->kasiorg[0];

				ptbl->karirec[i]	= next->karirec[0];
				ptbl->kasirec[i]	= next->kasirec[0];

				ptbl->addkari[i]	= next->addkari[0];
				ptbl->addkasi[i]	= next->addkasi[0];
				ptbl->delkari[i]	= next->delkari[0];
				ptbl->delkasi[i]	= next->delkasi[0];

				memcpy( &ptbl->zeikari[i], &next->zeikari[0], sizeof ptbl->zeikari[0] );
				memcpy( &ptbl->zeikasi[i], &next->zeikasi[0], sizeof ptbl->zeikasi[0] );
			}
			else {
				ptbl->kari[i]		= 0;
				ptbl->kasi[i]		= 0;
				ptbl->karisgn[i]	= 0;
				ptbl->kasisgn[i]	= 0;
				ptbl->delsgn[i]		= 0;
				ptbl->inssgn[i]		= 0;
				ptbl->kariorg[i].Reset();
				ptbl->kasiorg[i].Reset();

				ptbl->karirec[i].Reset();
				ptbl->kasirec[i].Reset();

				ptbl->addkari[i] = 0;
				ptbl->addkasi[i] = 0;
				ptbl->delkari[i] = 0;
				ptbl->delkasi[i] = 0;

				::ZeroMemory( &ptbl->zeikari[i], sizeof ptbl->zeikari[0] );
				::ZeroMemory( &ptbl->zeikasi[i], sizeof ptbl->zeikasi[0] );
			}
		}
		else {
			ptbl->kari[i]		= ptbl->kari[i+1];
			ptbl->kasi[i]		= ptbl->kasi[i+1];
			ptbl->karisgn[i]	= ptbl->karisgn[i+1];
			ptbl->kasisgn[i]	= ptbl->kasisgn[i+1];
			ptbl->delsgn[i]		= ptbl->delsgn[i+1];
			ptbl->inssgn[i]		= ptbl->inssgn[i+1];

			ptbl->kariorg[i]	= ptbl->kariorg[i+1];
			ptbl->kasiorg[i]	= ptbl->kasiorg[i+1];

			ptbl->karirec[i]	= ptbl->karirec[i+1];
			ptbl->kasirec[i]	= ptbl->kasirec[i+1];

			ptbl->addkari[i]	= ptbl->addkari[i+1];
			ptbl->addkasi[i]	= ptbl->addkasi[i+1];
			ptbl->delkari[i]	= ptbl->delkari[i+1];
			ptbl->delkasi[i]	= ptbl->delkasi[i+1];

			memcpy( &ptbl->zeikari[i], &ptbl->zeikari[i+1], sizeof ptbl->zeikari[0] );
			memcpy( &ptbl->zeikasi[i], &ptbl->zeikasi[i+1], sizeof ptbl->zeikasi[0] );
		}
	}
}


//
//	伝票形式 最大行数を返送する。
//
int CDenpScan::get_MaxLine( int dtype )
{
	int max = DP_SCROLL_NL;

	if( dtype == TYPE_FURIKAE ) {
		if( SyogSiwakeMode() )	max = DP_SCROLL_MAX;
	}

	return max;
}


//諸口の仕訳かどうか？
BOOL CDenpScan::IsFuriSyogPostion( int ln, int pn )
{
	BOOL bRet = FALSE;

	if( m_Sheet.dtype == TYPE_FURIKAE ) {
		int idx = ln-1;

		if( m_Sheet.karisgn[idx] ) {
			if( isSYOGT_CODE( m_Sheet.kariorg[idx].m_cre ) ) {
				if( IsCredPosition( pn ) )
					bRet = TRUE;
			}
		}
		else if( m_Sheet.kasisgn[idx] ) {
			if( isSYOGT_CODE( m_Sheet.kasiorg[idx].m_dbt ) ) {
				if( IsDebtPosition( pn ) )
					bRet = TRUE;
			}
		}
	}

	return	bRet;
}

//振替伝票 複合仕訳 諸口の仕訳で、諸口でない科目側(借／貸)かどうか？
BOOL CDenpScan::IsFuriNotSyogPostion(int ln, int dcsw)
{
	BOOL bRet = FALSE;

	if (m_Sheet.dtype == TYPE_FURIKAE) {
		int idx = ln - 1;

		if (m_Sheet.karisgn[idx]) {
			if (isSYOGT_CODE(m_Sheet.kariorg[idx].m_cre)) {
				if (dcsw == 0)
					bRet = TRUE;
			}
		}
		else if (m_Sheet.kasisgn[idx]) {
			if (isSYOGT_CODE(m_Sheet.kasiorg[idx].m_dbt)) {
				if (dcsw == 1)
					bRet = TRUE;
			}
		}
	}

	return	bRet;
}


//元の状態が単一仕訳かどうか(振替伝票)
BOOL CDenpScan::IsTanitsuSiwake( int ln )
{
	if( m_Sheet.dtype == TYPE_FURIKAE ) {
		int idx = ln-1;

		if( m_Sheet.karisgn[idx] && m_Sheet.kasisgn[idx] ) {
			// SEQ番号が異なる場合は複合仕訳
			if( m_Sheet.kari[idx] == m_Sheet.kasi[idx] )	return	TRUE;
			else											return	FALSE;
		}
		if( m_Sheet.karisgn[idx] || m_Sheet.kasisgn[idx] )	return	FALSE;
	}

	return	FALSE;
}


//--------------------------------------------------------------
//	伝票のデータ最終行を返送
//
//--------------------------------------------------------------
int CDenpScan::GetLastDataLine()
{
	if( m_Sheet.dtype != TYPE_FURIKAE )
		return DP_SCROLL_NL;

	//振替伝票は 諸口を詰めない場合、100行を超える
	int maxLine = get_MaxLine( m_Sheet.dtype );
	int idx, ret;

	ret = DP_SCROLL_NL;

	for( idx = maxLine-1; idx >= 0; idx-- ) {

		if( m_Sheet.karisgn[idx] || m_Sheet.kasisgn[idx] ) {
			break;
		}
	}

	if( (idx+1) > ret )	ret = (idx+1);

	return ret;
}


// 指定ＳＥＱの仕訳が何行目か？
BOOL CDenpScan::GetDenpyoLineFromSeq( int seq, int& line, int& dc_sw )
{
	//振替伝票は 諸口を詰めない場合、100行を超える
	int maxLine = get_MaxLine( m_Sheet.dtype );

	int chk_ln = -1;
	int chk_dc = -1;

	for( int n = 0; n < maxLine; n++ ) {
		if( m_Sheet.karisgn[n] ) {
			if( m_Sheet.kari[n] == seq ) {
				chk_ln = n+1;
				chk_dc = 0;
				break;
			}
		}
		if( m_Sheet.kasisgn[n] ) {
			if( m_Sheet.kasi[n] == seq ) {
				chk_ln = n+1;
				chk_dc = 1;
				break;
			}
		}
	}

	if( chk_ln == -1 )	return FALSE;

	line = chk_ln;
	dc_sw = chk_dc;

	return	TRUE;
}



