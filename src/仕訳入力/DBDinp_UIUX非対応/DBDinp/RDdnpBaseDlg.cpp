// DnpBaseDlg.cpp : 実装ファイル
//

#include "stdafx.h"

#include "DenpRec.h"
#include "RDdnpBaseDlg.h"

#include "DnpInView.h"


#include "input1.h"
#include "external.h"
#include "MstRw.h"



// 定型仕訳
extern CMKRDSub Mkrd;

extern CDnpInView* p_DnpView;


// 消費税文字列 改行を入れる
extern void check_syzstring( CString& str );


/*************************************************************
	伝票ダイアログ CIcsdiag8ctrl の配置仕様
	
	------------------------------------------------
	タイトル・伝票番号・日付等  = IDC_ICSDIAG8CTRL1
	------------------------------------------------

	伝票実データ部 データ×７行 = IDC_ICSDIAG8CTRL2

	合計行
	------------------------------------------------

***************************************************************/


// CRDdnpBaseDlg ダイアログ

IMPLEMENT_DYNAMIC(CRDdnpBaseDlg, ICSDialog)

int CRDdnpBaseDlg::m_BmonLNG = 0;
int CRDdnpBaseDlg::m_KojiLNG = 0;


// 部門・工事番号の入力レングスセット( 対象マスターでない場合は 0 )
void CRDdnpBaseDlg::SetMasterPar( int bmlng, int kjlng )
{
	m_BmonLNG = bmlng;
	m_KojiLNG = kjlng;
}



CRDdnpBaseDlg::CRDdnpBaseDlg(UINT dlgID, CWnd* pParent /*=NULL*/)
	: ICSDialog(dlgID, pParent)
{
	m_dlgID = dlgID;
	m_pDiag = NULL;

	m_dgno = -1;
	m_index = -1;

	m_DclmCount = 0;

	m_pHDpos = NULL;
	m_pDTpos = NULL;
	m_pKeiPos = NULL;

	m_kind = KIND_FURIKAE;

	m_ItemCount = 0;
	m_AllLineCount	= p_DnpView->get_MaxLine();
}

CRDdnpBaseDlg::~CRDdnpBaseDlg()
{
}

void CRDdnpBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRDdnpBaseDlg, ICSDialog)
	ON_WM_SIZE()
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus )
	ON_MESSAGE( WM_RETROFOCUS2, OnRetroFocus2 )
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CRDdnpBaseDlg メッセージ ハンドラ

BOOL CRDdnpBaseDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	GetDlgItem(IDC_ST_RDNPLABEL)->SetWindowText( m_sLabel );

//	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//--------------------------------------------
//	ダイアログ初期化
//	DNP_POS* dpos	伝票項目詳細データ構造体
//
//--------------------------------------------
void CRDdnpBaseDlg::InitDnpBase( DNP_POS* head, DNP_POS* data, DNP_POS* kei, int totalitemcnt )
{

	m_pHDpos	= head;
	m_pDTpos	= data;
	m_pKeiPos	= kei;

	// 一仕訳あたりのDiag項目数
	int cnt = 0;
	for( int i = 0; (m_pDTpos + i)->dpno != -1; i++ ) {
		if( (m_pDTpos + i)->dpno != -1 )
			cnt++;
	}
	m_DclmCount = cnt;

	m_ItemCount = totalitemcnt;
}


//---------------------------------------------------
//	ダイアグラム変数を初期化
//	CICSDiag* pDiag	ダイアグラムコントロール変数
//
//---------------------------------------------------
void CRDdnpBaseDlg::InitBaseDiag( CIcsdiag8ctrl* pDiag, int diagcnt )
{
	m_pDiag = pDiag;

	for( int i = 0; i < diagcnt; i++ ) {
		(m_pDiag+i)->CursorEndMode(1);
		(m_pDiag+i)->EnableDelete(0);
	}

	DiagInit();
}

//----------------------------------------------------
//	コントロールポジションセット
//
//----------------------------------------------------
void CRDdnpBaseDlg::ControlPos()
{
}


//----------------------------------------------------
//	イメージ等 コントロールイニシャライズ
//
//----------------------------------------------------
void CRDdnpBaseDlg::control_iniz()
{
}


//----------------------------------------------------
//	ダイアグラムの現在位置より伝票行・項目取得
//	int& ln		行( 1 から始まる)
//	int& pn		ポジションサイン
//
//----------------------------------------------------
void CRDdnpBaseDlg::GetDnpNowPos( int& ln, int& pn )
{
	DNP_POS* pos;
	pos = m_pHDpos;

	int chk = 0;
	//ヘッド部
	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			ln = -1;
			pn = -1;
			break;
		}

		if( pos->dpno == m_dgno && pos->dpindex == m_index ) {
			ln = -1;
			pn = pos->dp_pn;
			chk++;
			break;
		}
	}

	if( chk )	return;
	//借方・貸方 合計
	pos = m_pKeiPos;

	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			ln = -1;
			pn = -1;
			break;
		}
		if( pos->dpno == m_dgno && pos->dpindex == m_index ) {
			ln = -1;
			pn = pos->dp_pn;
			chk++;
			break;
		}
	}

	if( chk )	return;
	//仕訳データ
	pos = m_pDTpos;

	int dataidx, line;

	int data_pg;

	data_pg = (m_index / m_ItemCount) + 1;

	int ln_item = GetLineItemCount();
	int ln_sbn = ln_item * (data_pg - 1);

	int index = m_index - ln_sbn;

	dataidx = (index % m_DclmCount);
	line = (index / m_DclmCount) + 1;

	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			ln = -1;
			pn = -1;
			break;
		}
		if( pos->dpno == m_dgno && pos->dpindex == dataidx ) {
			ln = line;
			pn = pos->dp_pn;
			chk++;
			break;
		}
	}

ASSERT( chk );

	if( chk )	return;
}


//----------------------------------------------------
//	指定ポジションより DNP_POSデータを取得
//	int ln		行
//	int pn		ポジションサイン
//
//----------------------------------------------------
DNP_POS* CRDdnpBaseDlg::GetDNP_POS( int ln, int pn )
{
	DNP_POS* pos, *pRpos;
	int i, sgn = 0;;

	pRpos = NULL;

	if( ln == -1 || (pn >= DP_DENP_PN && pn <= DP_KOJI_PN) ) {
		// ヘッド部
		pos = m_pHDpos;

		for( i = 0; (pos+i)->dpno != -1 ; i++ ) {
			if( pn == (pos+i)->dp_pn ) {
				m_PosBuf = *(pos+i);
				pRpos = &m_PosBuf;
				break;
			}
		}
	}
	else {
		// 仕訳データ部
		pos = m_pDTpos;
		int data_pg = (ln / m_LineCount) - 1;


		for( i = 0; (pos+i)->dpno != -1 ; i++ ) {
			if( pn == (pos+i)->dp_pn ) {
				m_PosBuf = *(pos+i);
				int idx;

				idx =  GetPosIndex( ln, &m_PosBuf );
				m_PosBuf.dpindex = idx;

				pRpos = &m_PosBuf;
				break;
			}
		}
	}

	return pRpos;
}

//----------------------------------------------------
//	決修、四半期などの表示
//
//----------------------------------------------------
void CRDdnpBaseDlg::kesyu_dsp( char* str )
{
	DNP_POS* pos;
	pos = m_pHDpos;

	int chk = 0;
	DIAGRAM_DATA dd;

	//ヘッド部
	for( ; pos->dpno != -1; pos++ ) {
		if( pos->dp_pn == DP_KESYU_PN ) {
			if( str[0] ) {
				dd.data_disp = str;
				dd.data_imgdata = NULL;
				(m_pDiag + pos->dpno)->SetData( pos->dpindex, (LPUNKNOWN)&dd );
			}
			else {
				(m_pDiag + pos->dpno)->DataClear( pos->dpindex, TRUE );
			}
			break;
		}
	}
}

//----------------------------------------------------
//	伝票へデータのセット
//		基本的にはデータはクリアされている
//
//----------------------------------------------------
void CRDdnpBaseDlg::inpdata_dsp()
{
	int i, ln, n, idx, bmn;
	DNP_POS* p;
	struct _DenpData	check = {0};
	DBKNREC	*knrec;
	char	edabuf[30], tmp[128];

	SETMEISYO	stm;
	struct _SET_KMKETC sk;
	char	val[6];
	CString	kno, txt;
	int len;
	struct _SY_MSG_PACK2 smpk;
	int swk, ritu, zei, uri, tai, toku;

	l_defn( _BIN );

	static CDBINPDataRec	dbtdata;
	static CDBINPDataRec	credata;

	CRDNPdata* prd;
	prd = Mkrd.dnpData;

	prd->MoveFirst();
	if( prd->st == -1 )
		return;

	// ヘッド部
	p = m_pHDpos;
	for( i = 0; (p+i)->dpno != -1; i++ ) {
		// 入力可能項目のみ
		if( ! (p+i)->dp_enable )
			continue;

		idx = (p+i)->dpindex;

		switch( (p+i)->dp_pn ) {
#ifdef LATER_CLOSE
			case DP_DENP_PN:
				if( pcnd->INP_sg )
					_SetDnpyoNo( (p+i)->dpno, (p+i)->dpindex, pdata->DP_DENP );
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
			case DP_MM_PN:
				if( pcnd->INP_sg )
					_SetMM( (p+i)->dpno, (p+i)->dpindex, pdata->DP_BCDDAY[0] );
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
			case DP_DD_PN:
				if( pcnd->INP_sg )
					_SetDD( (p+i)->dpno, (p+i)->dpindex, pdata->DP_BCDDAY[0] );
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
#endif
			case DP_BMN_PN:
				if( m_kind == KIND_NYUKIN || m_kind == KIND_SYUKKIN ) {
					if( m_kind == KIND_SYUKKIN ) {
						bmn = prd->rdp_dbmn;
					}
					else {
						bmn = prd->rdp_cbmn;
					}

					if( bmn != -1 ) {
						sk.bmn = bmn;
						sk.brn = -1;
						sk.etc.Empty();
						sk.etc_col = 0;
						if( p_DnpView->GetBmnEtc( &sk ))
							_SetBmon( (p+i)->dpno, idx, bmn, &sk );
						else
							_SetBmon((p + i)->dpno, idx, bmn, NULL );
					}
					else {	
						DiagClear( (p+i)->dpno, idx );
					}
				}
				break;
			case DP_KOJI_PN:
				if( m_kind == KIND_NYUKIN || m_kind == KIND_SYUKKIN ) {
					if( m_kind == KIND_SYUKKIN ) {
						kno = prd->rdp_dkno;
					}
					else {
						kno = prd->rdp_ckno;
					}
					if( ! kno.IsEmpty() ) {
						sk.bmn = -1;
						sk.kno = kno;
						sk.brn = -1;
						sk.etc.Empty();
						sk.etc_col = 0;
						if( p_DnpView->GetKojiEtc( &sk ) )
							_SetKoji( (p+i)->dpno, idx, kno, &sk );
						else
							_SetKoji( (p+i)->dpno, idx, kno, NULL );
					}
					else {	
						DiagClear( (p+i)->dpno, idx );
					}
				}
				break;
			default:
				ASSERT( FALSE );
				break;
		}
	}

	// 仕訳データ部
	p = m_pDTpos;

	BOOL bSkbn;
	_SY_MSG_PACK2	smp2;

	char	dbt_kei[6] = {0}, cre_kei[6] = {0};

	BOOL bset = FALSE;
	CDBINPDataRec* pd;
	int dbtsgn, cresgn;

	for( ln = 1; ln <= m_AllLineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);
		dbtsgn = cresgn = 0;

		//消費税データを作成
		for( i = 0; (p + i)->dpno != -1; i++ ) {
			// 行ごとの インデックス(線の分を調整している)
			idx = GetPosIndex( ln, (p+i) );

			if( (p + i)->dp_pn == FD_DSKBN_PN ) {
				if( m_kind == KIND_NYUKIN ) {
					dbtdata.Reset();
					break;
				}
				dbtdata.m_seq = pDBzm->m_lastdata.m_seq + 1;
				p_DnpView->setDATE_CREC( &dbtdata );

				l_input( dbtdata.m_val, (void*)(LPCTSTR)prd->rdp_dval );
				l_input( dbtdata.m_zei, (void*)(LPCTSTR)prd->rdp_dzei );

				dbtdata.m_dbt = prd->rdp_dbt;
				dbtdata.m_dbr = prd->rdp_dbr;
				dbtdata.m_dsign.Copy( prd->rdp_ddsgn );

				if( m_kind == KIND_SYUKKIN ) {
					dbtdata.m_cre = p_DnpView->GNKINcode();
					dbtdata.m_cbr = -1;
				}
				else {
					dbtdata.m_cre = SYOGT;
					dbtdata.m_cbr = -1;
				}

				if( dbtdata.m_dbt.IsEmpty() )	dbtdata.m_dbt = SYOGT;

				pd = &dbtdata;
			}
			else if( (p + i)->dp_pn == FD_CSKBN_PN ) {
				if( m_kind == KIND_SYUKKIN ) {
					credata.Reset();
					break;
				}
				credata.m_seq = pDBzm->m_lastdata.m_seq + 2;
				p_DnpView->setDATE_CREC( &credata );

				if( m_kind == KIND_SYUKKIN ) {
					credata.m_dbt = p_DnpView->GNKINcode();
					credata.m_dbr = -1;
				}
				else {
					credata.m_dbt = SYOGT;
					credata.m_dbr = -1;
				}

				l_input( credata.m_val, (void*)(LPCTSTR)prd->rdp_cval );
				l_input( credata.m_zei, (void*)(LPCTSTR)prd->rdp_czei );

				credata.m_cre = prd->rdp_cre;
				credata.m_cbr = prd->rdp_dbr;
				credata.m_dsign.Copy( prd->rdp_cdsgn );

				if( credata.m_cre.IsEmpty() )	credata.m_cre = SYOGT;

				pd = &credata;
			}
			else {
				continue;
			}

			DB_SyIniz( pd );

			txt = p_DnpView->sy_chk_txt( pd, &smp2 );
			// 表示
			PutsSyz( (p + i)->dpno, idx, (char*)(LPCTSTR)txt );
			pDBsy->SyMsg( &smpk );

			swk = ritu = zei = uri = tai = toku = 0;

			swk = smpk.SY_MSG_SWKBN->sel_no;
			ritu = smpk.SY_MSG_RITU->sel_no;
			zei = smpk.SY_MSG_ZEI->sel_no;
			uri = smpk.SY_MSG_URISIRE->sel_no;
			toku = smpk.SY_MSG_TOKTEI->sel_no;
			tai = smpk.SY_MSG_TAIKAKBN->sel_no;

			bSkbn = (swk || ritu || zei || uri || tai);

			//有価証券の非課税譲渡仕訳のために 最後で表示
			//借方税額
			if( (p + i)->dp_pn == FD_DSKBN_PN && bSkbn ) {
				memmove( check.DP_VAL, pd->m_zei, sizeof pd->m_zei );
				if( pDBsy->IsTaikaData( pDBzm->dbdata ) == 2 )		dbtsgn = 1;
				if (!is_syohizeisiwake( pDBzm->dbdata ))			dbtsgn = 2;
			}
			//貸方税額
			if( (p + i)->dp_pn == FD_CSKBN_PN && bSkbn ) {
				memmove( check.DP_VAL, pd->m_zei, sizeof pd->m_zei );
				if( pDBsy->IsTaikaData( pDBzm->dbdata ) == 2 )		cresgn = 1;
				if (!is_syohizeisiwake( pDBzm->dbdata ))			cresgn = 2;
			}
		}

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			// 入力可能項目のみ
			if( ! (p+i)->dp_enable )
				continue;

			// 行ごとの インデックス(線の分を調整している)
			idx = GetPosIndex( ln, (p+i) );
//TRACE( "inpdata_dsp, %d, idx = %d, pn = %d\n", (p+i)->dpno, idx, (p+i)->dp_pn );

			switch( (p+i)->dp_pn ) {
			case FD_DBVAL_PN:
			case FD_DBZEI_PN:
				if( Mkrd.dnpData->rdp_dvalsgn ) {
					memcpy( val, dbtdata.m_val, sizeof val );

					if( (p+i)->dp_pn == FD_DBVAL_PN ) {
						l_add( dbt_kei, dbt_kei, val );
						if( sotozei_data( &dbtdata )) {
							l_add( dbt_kei, dbt_kei, dbtdata.m_zei );
						}
						_SetVal( (p+i)->dpno, idx, (char*)val );
					}
					else {
						_SetVal( (p+i)->dpno, idx, (char*)dbtdata.m_zei, dbtsgn );
					}
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;
			case FD_CRVAL_PN:
			case FD_CRZEI_PN:
				if( Mkrd.dnpData->rdp_cvalsgn ) {
					memcpy( val, credata.m_val, sizeof val );

					if( (p+i)->dp_pn == FD_CRVAL_PN ) {
						l_add( cre_kei, cre_kei, val );
						if( sotozei_data( &credata )) {
							l_add( cre_kei, cre_kei, credata.m_zei );
						}
						_SetVal( (p+i)->dpno, idx, (char*)val );
					}
					else {
						_SetVal( (p+i)->dpno, idx, (char*)credata.m_zei, cresgn );
					}
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_DBMN_PN:
			case FD_CBMN_PN:
				bmn = -1;
				if( (p+i)->dp_pn == FD_DBMN_PN ) {
					if( ! prd->rdp_dbt.IsEmpty() )
						bmn = prd->rdp_dbmn;
				}
				else {
					if( ! prd->rdp_cre.IsEmpty() )
						bmn = prd->rdp_cbmn;
				}

				if( bmn != -1 ) {
					sk.bmn = bmn;
					sk.brn = -1;
					sk.etc.Empty();
					sk.etc_col = 0;
					if( p_DnpView->GetBmnEtc( &sk ) )
						_SetBmon( (p+i)->dpno, idx, bmn, &sk );
					else
						_SetBmon((p + i)->dpno, idx, bmn, NULL);
				}
				else {	
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_DKOJI_PN:
			case FD_CKOJI_PN:
				kno.Empty();
				if( (p+i)->dp_pn == FD_DKOJI_PN ) {
					if( ! prd->rdp_dbt.IsEmpty() )
						kno = prd->rdp_dkno;
				}
				else {
					if( ! prd->rdp_cre.IsEmpty() )
						kno = prd->rdp_ckno;
				}

				if( ! kno.IsEmpty() ) {
					sk.bmn = -1;
					sk.kno = kno;
					sk.brn = -1;
					sk.etc.Empty();
					sk.etc_col = 0;

					if( p_DnpView->GetKojiEtc( &sk ) )
						_SetKoji( (p+i)->dpno, idx, kno, &sk );
					else
						_SetKoji( (p+i)->dpno, idx, kno, NULL );
				}
				else {	
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_DEBT_PN:
			case FD_CRED_PN:

				::ZeroMemory( &check, sizeof check );
				knrec	=	NULL;

				if( (p+i)->dp_pn == FD_DEBT_PN ) {
					strcpy_s( check.DP_CdNum, sizeof check.DP_CdNum, prd->rdp_dbt );
					check.DP_BRN = prd->rdp_dbr;
					if( BMON_MST ) {
						sk.bmn = prd->rdp_dbmn;
					}
					else {
						sk.bmn = -1;
					}
				}
				else {
					strcpy_s( check.DP_CdNum, sizeof check.DP_CdNum, prd->rdp_cre );
					check.DP_BRN = prd->rdp_cbr;
					if( BMON_MST ) {
						sk.bmn = prd->rdp_cbmn;
					}
					else {
						sk.bmn = -1;
					}
				}

				knrec	=	pDBzm->DB_PjisToKnrec( check.DP_CdNum, FALSE );
				if( knrec ) {
					KamokuString( knrec, check.DP_KNAM, sizeof check.DP_KNAM );
				}

				sk.code = check.DP_CdNum;
				sk.brn  = check.DP_BRN;

				if( _set_kmketc( &sk ) ) {
					strcpy_s( stm.sm_etc, sizeof stm.sm_etc, sk.etc );
					stm.sm_etccol = sk.etc_col;
				}
				else {
					::ZeroMemory( stm.sm_etc, sizeof stm.sm_etc );
				}

				pDBzm->EdabanToStr( edabuf, sizeof edabuf, check.DP_BRN );

				if( edabuf[0] )
					sprintf_s( stm.sm_name, sizeof stm.sm_name, "%s%s", check.DP_KNAM, edabuf );
				else
					strcpy_s( stm.sm_name, sizeof stm.sm_name, check.DP_KNAM );
				
				if( knrec ) {
					_SetKnam( (p+i)->dpno, idx, &stm );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_DSYOG_PN:
			case FD_CSYOG_PN:
				DiagClear( (p+i)->dpno, idx );

				if( m_kind == KIND_FURIKAE ) {
					if( p_DnpView->IsFuriSyogBrn() ) {
						check.DP_cBRN = ((p+i)->dp_pn == FD_DSYOG_PN) ? prd->rdp_dsyogbrn : prd->rdp_csyogbrn;
						if( check.DP_cBRN != -1 ) {
							_SetSyogBrn( (p+i)->dpno, idx, check.DP_cBRN );
						}
						else {
							DiagClear( (p+i)->dpno, idx );
						}
					}
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_TKY_PN:
				len	=	Voln1->tk_ln * 2;
				sprintf_s( (char*)tmp, sizeof(tmp), _T("%s"), (LPCTSTR)prd->rdp_dtky );
				memmove( check.DP_TKY, tmp, len );

				if( tmp[0] ) {
					_SetTekiyo( (p+i)->dpno, idx, (char*)check.DP_TKY );
				}
				else	DiagClear( (p+i)->dpno, idx );

				break;

#ifdef LATER_CLOSE
			case FD_TGNUM_PN:
				if( pcnd->INP_sg ) {
					_SetTgNumber( (p+i)->dpno, idx, (char*)pdata->DP_SNUM );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_TGDATE_PN:
				if( pcnd->INP_sg ) {
					_SetTgDate( (p+i)->dpno, idx, pdata->DP_TGBCD );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_HJIYU_PN:
				if( pcnd->INP_sg ) {
					CString txt;
				//	p_DnpView->get_hjiyustr( pdata->DP_BMN, &txt );
					_get_hjiyustr( pdata->DP_BMN, pdata->NAIBCD, txt );
					_SetHjiyu( (p+i)->dpno, idx, &txt );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_HUSEN_PN:
				SetHusen( ln, FD_HUSEN_PN, pdata->DP_HUSEN );
				break;
#endif

			default:
				break;
			}
		}

		prd->MoveNext();
		if( prd->st == -1 )
			break;
	}

	// 合計データ部
	p = m_pKeiPos;

	int cnt = 0;
	for( int i = 0; (p + i)->dpno != -1; i++ ) {
		if( (p+i)->dp_pn == FD_DBVAL_PN ) {
			_SetVal( (p+i)->dpno, (p+i)->dpindex, (char*)dbt_kei );
		}
		else if( (p+i)->dp_pn == FD_CRVAL_PN ) {
			_SetVal( (p+i)->dpno, (p+i)->dpindex, (char*)cre_kei );
		}
	}
}



//----------------------------------------------------
//	ダイアグ Clear
//	int dgno ...... ダイアグラム番号(0から）
//	int index ..... インデックス
//----------------------------------------------------
void CRDdnpBaseDlg::DiagClear( int dgno, int index )
{
	(m_pDiag+dgno)->DataClear(index, TRUE);
}


//---------------------------------------------
//	ダイアグラムInit
//
//---------------------------------------------
void CRDdnpBaseDlg::DiagInit()
{
	int		index, flg, no;
	// システム情報ファイル
//	ICSSysInfo	Csysinfo;
//	Csysinfo.GetInformation();
//	flg = Csysinfo.TekiyoEnter();
	flg = 0;

	DIAGRAM_ATTRIBUTE	datr;


	for( int ln = 0; ln < m_AllLineCount; ln++ ) {
	
		for( int i = 0; (m_pDTpos+i)->dpno != -1; i++ ) {

			index = GetPosIndex( ln+1, (m_pDTpos+i) );
	
			no = (m_pDTpos+i)->dpno;
			(m_pDiag + no)->GetAttribute( index, (LPUNKNOWN)&datr );

			switch( (m_pDTpos+i)->dp_pn ) {
			case FD_DEBT_PN:
			case FD_CRED_PN:
				datr.attr_editattr	|=	0x0300;
//				datr.attr_editattr	|=	0x0100;
				break;
			case FD_TKY_PN:
				datr.attr_editattr	|=	0x0300;
				if( flg ) datr.attr_editattr	|= 0x0400/*DIAG_EATTR_TEKIKEY*/;
				break;
			default:
				datr.attr_editattr	|=	0x0100;
				break;
			}

			(m_pDiag + no)->SetAttribute( index, (LPUNKNOWN)&datr, FALSE );
		}
	}

}

//------------------------------------------------------
//	入力許可サインをセット(部門など)
//
//------------------------------------------------------
void CRDdnpBaseDlg::ctl_init( int pn, BOOL enable )
{
	DNP_POS*	pDNpos;
	int i, no, idx, sgn;

	no = idx = -1;
	sgn = 0;

	//ヘッド部
	pDNpos = m_pHDpos;
	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		if( pn == (pDNpos+i)->dp_pn ) {
			sgn = 1;

			(pDNpos+i)->dp_enable = enable;
			break;
		}
	}

	if( sgn )	return;

	// データ部
	pDNpos = m_pDTpos;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		if( pn == (pDNpos+i)->dp_pn ) {
			sgn = 1;

			(pDNpos+i)->dp_enable = enable;
			break;
		}
	}
}

//------------------------------------------------------
//	1仕訳最初の入力位置をセット(仕訳データ部)
//
//------------------------------------------------------
DNP_POS* CRDdnpBaseDlg::GetFirstDnpPos()
{
	DNP_POS *pDNpos, *pRet;
	pDNpos = m_pDTpos;
	pRet = NULL;
	int i, chkpn;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		chkpn = (pDNpos+i)->dp_pn;

		if( ! (pDNpos+i)->dp_enable ) {
			continue;
		}
		else {
			pRet = (pDNpos+i);
			break;
		}
	}

	return pRet;
}


//------------------------------------------------------
//	1仕訳の最後の入力位置をセット(仕訳データ部)
//
//------------------------------------------------------
DNP_POS* CRDdnpBaseDlg::GetLastDnpPos()
{
	DNP_POS *pDNpos, *pRet;
	pDNpos = m_pDTpos;
	pRet = NULL;
	int i, chkpn;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		chkpn = (pDNpos+i)->dp_pn;

		if( ! (pDNpos+i)->dp_enable ) {
			continue;
		}
		else {
			pRet = (pDNpos+i);
		}
	}

	return pRet;
}


// フォーカスセット
//	PostMessage用
//
LRESULT CRDdnpBaseDlg::OnRetroFocus( WPARAM w, LPARAM l )
{
//	MAKEWORD

	int sgn = (short)HIWORD( w );
	int wp = (short)LOWORD( w );

	if( sgn == 0 ) {
		set_focus( wp, l );
	}
	else {
		(m_pDiag+wp)->SetPosition( l );
	}

	return 1;
}


LRESULT CRDdnpBaseDlg::OnRetroFocus2( WPARAM w, LPARAM l )
{
	CWnd* pwnd = GetDlgItem( w );
	if( pwnd != NULL ) {
		GotoDlgCtrl( pwnd );
	}

	return 1;
}


//------------------------------------------------------
//	フォーカスセット
//		int ln ... 行数(-1 のときはヘッド部)
//		inp pn ... ポジションサイン
//
//		return	 0 = OK, 
//				-1 = フォーカスセットできない
//------------------------------------------------------
int CRDdnpBaseDlg::set_focus( int ln, int pn )
{
	int i, ret;
	int no, idx;

	DNP_POS*	pDNpos;

	no = idx = -1;
	ret = -1;

	//ヘッド部
	if( ln == -1 ) {
		pDNpos = m_pHDpos;
		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			if( pn == (pDNpos+i)->dp_pn ) {

				if( (pDNpos+i)->dp_enable ) {
					no = (pDNpos+i)->dpno;
					idx = (pDNpos+i)->dpindex;
				}
				break;
			}
		}
	}
	else {
		// データ部
		pDNpos = m_pDTpos;

		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {

			if( pn == (pDNpos+i)->dp_pn ) {

				if( (pDNpos+i)->dp_enable ) {
					no = (pDNpos+i)->dpno;
					idx = GetPosIndex( ln, (pDNpos+i) );
				}
				break;
			}
		}
	}

	if( no != -1 && idx != -1 ) {
		(m_pDiag + no)->SetPosition( idx );
		ret = 0;
	}

	return ret;
}

//------------------------------------------------------
//	次の位置へフォーカスセット
//		int ln ... 行数(-1 のときはヘッド部)
//		inp pn ... ポジションサイン
//------------------------------------------------------
int CRDdnpBaseDlg::set_nextfocus( int ln, int pn )
{
	int i, line, vect = 0;
	int chkpn, sgn, n;
	DNP_POS*	pDNpos;
	DNP_POS		newpos = {0};
	DNP_POS		fstpos = {0};
	newpos.dp_pn	= -1;
	newpos.dpindex	= -1;
	newpos.dpno		= -1;
	fstpos.dp_pn	= -1;
	fstpos.dpindex	= -1;
	fstpos.dpno		= -1;

	sgn = 0;
	chkpn = line = -1;

	//現在位置がヘッド部
	if( ln == -1 ) {
		pDNpos = m_pHDpos;
		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			if( pn == (pDNpos+i)->dp_pn ) {
				sgn = 1;
				continue;
			}

			if( sgn ) {
				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
	}
	//データ部検索
	if( newpos.dpno == -1 ) {
		pDNpos = m_pDTpos;
		sgn = 0;
		if( ln == -1 ) {
			pn = pDNpos->dp_pn;
		}

		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			chkpn = (pDNpos+i)->dp_pn;

			if( pn == (pDNpos+i)->dp_pn ) {
				if( ln == -1 ) {	// ヘッド部からの送り
					if( (pDNpos+i)->dp_enable ) {
						line = (ln == -1) ? 1 : ln;
						newpos = *(pDNpos+i);
						break;
					}
				}
				sgn = 1;
				continue;
			}

			if( sgn ) {
				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					line = (ln == -1) ? 1 : ln;
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
		//最初の位置ヘセット
		if( newpos.dpno == -1 ) {
			DNP_POS* ps;
			ps = GetFirstDnpPos();
			if( ps ) {
				line = (ln == -1) ? 1 : ln;
				line++;
				memcpy( &newpos, ps, sizeof DNP_POS );
			}
		}
#ifdef LATER_CLOSE
		//転送済み仕訳のチェックのため
		if( newpos.dpno != -1 ) {
			p_DnpView->set_position( line, newpos.dp_pn );
		}
#endif
		return 0;
	}

	if( newpos.dpno != -1 ) {
		if( line > m_AllLineCount )	line = m_AllLineCount;
		n = 0;

		int index = 0;
		if( line > 0 )	index = GetPosIndex( line, &newpos );
		(m_pDiag + newpos.dpno)->SetPosition( index );
	}

	return 0;
}


//------------------------------------------------------
//	前の位置へフォーカスセット
//		int ln ... 行数(-1 のときはヘッド部)
//		inp pn ... ポジションサイン
//------------------------------------------------------
int CRDdnpBaseDlg::set_prevfocus( int ln, int pn )
{
	int i, line, vect = 0;
	int chkpn, spos;
	DNP_POS*	pDNpos;
	DNP_POS		newpos = {0};
	DNP_POS		fstpos = {0};
	newpos.dp_pn	= -1;
	newpos.dpindex	= -1;
	newpos.dpno		= -1;
	fstpos.dp_pn	= -1;
	fstpos.dpindex	= -1;
	fstpos.dpno		= -1;

	spos = 0;
	chkpn = line = -1;

	//現在位置がヘッド部
	if( ln == -1 ) {
		pDNpos = m_pHDpos;
		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			if( pn == (pDNpos+i)->dp_pn ) {
				spos = i;
				break;
			}
		}

		if( spos > 0 ) {
			for( i = (spos-1); i >= 0; i-- ) {

				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
	}
	else {
		//データ部検索
		pDNpos = m_pDTpos;
		spos = 0;

		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			chkpn = (pDNpos+i)->dp_pn;

			if( pn == (pDNpos+i)->dp_pn ) {
				spos = i;
				break;;
			}
		}

		if( spos > 0 ) {
			// 一つ前の位置よりサーチ
			for( i = (spos-1); i >= 0; i-- ) {

				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					line = ln;
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
		else {
			// ヘッド部の最終位置へ
			pDNpos = m_pHDpos;
			int idx = -1;
			for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
				if( (pDNpos+i)->dp_enable ) {
					idx = i;
				}
			}

			if( idx >= 0 ) {
				newpos = *(pDNpos+idx);
				line = -1;
			}
		}

		//最初の位置ヘセット
		if( newpos.dpno == -1 ) {
			DNP_POS* ps;
			ps = GetFirstDnpPos();
			if( ps ) {
				line = (ln == -1) ? 1 : ln;
				line++;
				memcpy( &newpos, ps, sizeof DNP_POS );
			}
		}
	}

	if( newpos.dpno != -1 ) {
		if( line > m_AllLineCount )	line = m_AllLineCount;
		int index = 0;
		if( line > 0 )	index = GetPosIndex( line, &newpos );
		(m_pDiag + newpos.dpno)->SetPosition( index );
	}

	return 0;
}


//------------------------------------------------------
//	データ行の先頭へフォーカスセット
//------------------------------------------------------
int CRDdnpBaseDlg::set_datafirstfocus()
{
	int i, line, vect = 0;
	int chkpn, spos;
	DNP_POS*	pDNpos;
	DNP_POS		newpos = {0};
	newpos.dp_pn	= -1;
	newpos.dpindex	= -1;
	newpos.dpno		= -1;

	spos = 0;
	chkpn = line = -1;

	//データ部検索
	pDNpos = m_pDTpos;
	spos = 0;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {

		if( ! (pDNpos+i)->dp_enable ) {
			continue;
		}
		else {
			line = 1;
			newpos = *(pDNpos+i);
			break;
		}
	}

	if( newpos.dpno != -1 ) {
		if( line > m_AllLineCount )	line = m_AllLineCount;
		int index = 0;
		if( line > 0 )	index = GetPosIndex( line, &newpos );
		(m_pDiag + newpos.dpno)->SetPosition( index );
	}

	return 0;
}

//------------------------------------------------------
//		伝票をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetDnpyoNo(int dgno, int index, int dnpno )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( dnpno != -1 ) {
		dd.data_edit.Format( "%d", dnpno );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}

//	(m_pDiag + dno)->OffControl( TRUE );
//	(m_pDiag + dno)->SetPosition( idx );
//	(m_pDiag + dno)->OffControl( FALSE );

}


//------------------------------------------------------
//		月をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetMM(int dgno, int index, BYTE mm )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( mm != 0 ) {
		dd.data_edit.Format( "%02x", mm & 0x1f );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		日をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetDD(int dgno, int index, BYTE day )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( day != 0 ) {
	//	dd.data_edit.Format( "%02x", day & 0x31 );
		dd.data_day[0] = day;
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		部門をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetBmon( int dgno, int index, int bmn, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( bmn != -1 ) {
		DIAGRAM_ATTRIBUTE	da;
		(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

		if( psk == NULL || psk->etc.IsEmpty() ) {
			pDBzm->BumonCodeToStr( dd.data_edit.GetBuffer(64), 64, bmn );
			dd.data_edit.ReleaseBuffer();
			da.attr_fcolor = RGB_BLACK;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else {
			// 部門名称をセット
			dd.data_edit = psk->etc;
			LPSTR p = dd.data_edit.GetBuffer(64);
			int n = get_strcnt( p, 8 );
			p[n] = '\0';
			dd.data_edit.ReleaseBuffer(-1);

			da.attr_fcolor = psk->etc_col;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}

		dd.data_imgdata = NULL;

		if( bReset )	(m_pDiag + dno)->OffControl( TRUE );

		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );

		if( bReset ) {
			(m_pDiag + dno)->SetPosition( idx );
			(m_pDiag + dno)->OffControl( FALSE );
		}
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		工事番号をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetKoji( int dgno, int index, CString koji, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;
	char bf[128];

	if( ! koji.IsEmpty() ) {

		DIAGRAM_ATTRIBUTE	da;
		(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

		if( psk == NULL || psk->etc.IsEmpty() ) {
			pDBzm->KojiCodeToStr( bf, sizeof bf, koji );
			dd.data_edit.Format( "%s", bf );
			da.attr_fcolor = RGB_BLACK;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else {
			// 工事名称をセット
			dd.data_edit = psk->etc;
			LPSTR p = dd.data_edit.GetBuffer(64);
			int n = get_strcnt( p, 12 );
			p[n] = '\0';
			dd.data_edit.ReleaseBuffer(-1);

			da.attr_fcolor = psk->etc_col;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}

		dd.data_imgdata = NULL;

		if( bReset )	(m_pDiag + dno)->OffControl( TRUE );

		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );

		if( bReset ) {
			(m_pDiag + dno)->SetPosition( idx );
			(m_pDiag + dno)->OffControl( FALSE );
		}
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		科目名称をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetKnam(int dgno, int index, SETMEISYO* knam, BOOL bReset/*=FALSE*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;
	DIAGRAM_DATA dd;
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

	if( knam == NULL ) {
		da.attr_fcolor = RGB_BLACK;
		(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
	else {
		if( knam->sm_etc[0] ) {
			dd.data_edit = knam->sm_etc;
			da.attr_fcolor = knam->sm_etccol;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else {
			dd.data_edit = knam->sm_name;
			da.attr_fcolor = RGB_BLACK;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		dd.data_imgdata = NULL;

		if( bReset ) {
			(m_pDiag + dno)->OffControl( TRUE );
		}

		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );

		if( bReset ) {
			(m_pDiag + dno)->SetPosition( idx );
			(m_pDiag + dno)->OffControl( FALSE );
		}
	}
}


//------------------------------------------------------
//		諸口枝番をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetSyogBrn(int dgno, int index, int syogbrn )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

	if( syogbrn <= 0 ) {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
	else {
		da.attr_fcolor = RGB_DARK_BLUE;
		(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

		dd.data_edit.Format( "%03d", syogbrn );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
}


//------------------------------------------------------
//	金額をダイアグラムにセット
//	sgn = 0 金額表示
//	sgn = 1 有価証券非課税(*)
//  sgn = 2 金額を表示しない
//------------------------------------------------------
void CRDdnpBaseDlg::_SetVal(int dgno, short index, char* val, int sgn/*=0*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	if( val != NULL && l_test( val ) < 0 )
			da.attr_fcolor = RGB_RED;
	else	da.attr_fcolor = RGB_BLACK;

	DNP_POS* pos;
	pos = m_pKeiPos;
	BOOL bKei = FALSE;

	for(int i = 0; (pos+i)->dpno != -1; i++ ) {
		if( (pos+i)->dpindex == index ) {
			bKei = TRUE;
		}
	}

	if( sgn == 0 || sgn > 1 ) {
		da.attr_editformat = bKei ? "S,SSS,SSS,SSS,SSS" : "SSS,SSS,SSS,SSS";
	}
	else {
		da.attr_editformat = bKei ? "*SSSS,SSS,SSS,SSS" : "*SSS,SSS,SSS,SSS";
	}

	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );

	if( val != NULL && sgn != 2 ) {
		memcpy( dd.data_val, val, sizeof dd.data_val );;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		摘要をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetTekiyo(int dgno, short index, char* tky)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

#ifdef KEEP
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );
#endif

	if( tky != NULL ) {
		dd.data_edit = tky;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
TRACE( "_SetTekiyo Clear dno = %d, idx = %d\n", dno, idx );

		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		手形番号をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetTgNumber(int dgno, short index, char* tgnum)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

#ifdef KEEP
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );
#endif

	if( tgnum != NULL ) {
		dd.data_edit = tgnum;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		手形期日をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetTgDate(int dgno, short index, BYTE* tgdate)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

#ifdef KEEP
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );
#endif

	if( tgdate != NULL ) {
		memcpy( dd.data_day, tgdate, 3 );
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		変動事由をダイアグラムにセット
//
//------------------------------------------------------
void CRDdnpBaseDlg::_SetHjiyu(int dgno, short index, CString* hjiyu)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

	if( hjiyu != NULL && ! hjiyu->IsEmpty() ) {
		dd.data_edit = *hjiyu;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}



// 他から呼ばれるpublic なデータセット関数
//
void CRDdnpBaseDlg::SetDnpyoNo( int pn, int dnpno )
{
	DNP_POS* pos;
	pos = GetDNP_POS( -1, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	
	_SetDnpyoNo( pos->dpno, pos->dpindex, dnpno );
}

void CRDdnpBaseDlg::SetMM( int pn, BYTE mm )
{
	DNP_POS* pos;
	pos = GetDNP_POS( -1, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetMM( pos->dpno, pos->dpindex, mm );
}

void CRDdnpBaseDlg::SetDD( int pn, BYTE day )
{
	DNP_POS* pos;
	pos = GetDNP_POS( -1, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetDD( pos->dpno, pos->dpindex, day );
}

void CRDdnpBaseDlg::SetBmon( int ln, int pn, int bmn, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetBmon( pos->dpno, pos->dpindex, bmn, psk, bReset);
}

void CRDdnpBaseDlg::SetKoji( int ln, int pn, CString koji, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetKoji( pos->dpno, pos->dpindex, koji, psk, bReset );
}

void CRDdnpBaseDlg::SetKnam( int ln, int pn,  SETMEISYO* knam, BOOL bReset/*=FALSE*/ )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetKnam( pos->dpno, pos->dpindex, knam, bReset );
}

void CRDdnpBaseDlg::SetSyogBrn( int ln, int pn, int syogbrn )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}

	_SetSyogBrn( pos->dpno, pos->dpindex, syogbrn );
}


void CRDdnpBaseDlg::SetVal( int ln, int pn, char* val, int sgn)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetVal( pos->dpno, pos->dpindex, val, sgn );
}


void CRDdnpBaseDlg::SetTekiyo( int ln, int pn, char* tky)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetTekiyo( pos->dpno, pos->dpindex, tky );
}

#ifdef RDNP_CLOSE

// 手形番号
void CRDdnpBaseDlg::SetTgNumber( int ln, int pn, char* tgnum)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetTgNumber( pos->dpno, pos->dpindex, tgnum );
}

// 手形期日
void CRDdnpBaseDlg::SetTgDate( int ln, int pn, BYTE* tgdate)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetTgDate( pos->dpno, pos->dpindex, tgdate );
}

// 変動事由
void CRDdnpBaseDlg::SetHjiyu( int ln, int pn, CString* hjiyu )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetHjiyu( pos->dpno, pos->dpindex, hjiyu );
}

// 付箋
void CRDdnpBaseDlg::SetHusen( int ln, int pn, int husen )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}

	CHusenStatic* pHusen;
	pHusen = (CHusenStatic*)GetDlgItem( IDC_HUSEN1 + (ln-1) );

	pHusen->SetIconIndex( husen );
	pHusen->Invalidate();
	pHusen->UpdateWindow();
}

#endif


//-------------------------------------------------------
//	消費税文字列をセット
//
//-------------------------------------------------------
void CRDdnpBaseDlg::PutsSyz( int dgno, int dgidx, char* syztxt )
{
	DIAGRAM_DATA dd;
	dd.data_edit = syztxt;
	dd.data_imgdata = NULL;

//	(m_pDiag + dno)->OffControl( TRUE );
	check_syzstring( dd.data_edit );

	(m_pDiag + dgno)->SetData( dgidx, (LPUNKNOWN)&dd );

//	(m_pDiag + dno)->SetPosition( idx );
}


//-----------------------------------------------------------
//	入力項目の矩形取得
//-----------------------------------------------------------
void CRDdnpBaseDlg::GetRect( int ln, int pn, CRect& rect )
{
	DNP_POS* pos;

	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}

	(m_pDiag+pos->dpno)->GetRect( pos->dpindex, (LPUNKNOWN)&rect );
}


//------------------------------------------
//	諸口貸借を表示
//	char* syog		諸口金額
//------------------------------------------
void CRDdnpBaseDlg::SyogDisp( char* syog )
{
	DNP_POS* pos;

	pos = GetDNP_POS( -1, DP_SYOGT_PN );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	char buf[36];

	l_print( buf, syog, "sss,sss,sss,ss9" );

	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	(m_pDiag + pos->dpno)->GetAttribute( pos->dpindex, (LPUNKNOWN)&da );
	if( l_test( syog ) < 0 )
			da.attr_fcolor = RGB_RED;
	else	da.attr_fcolor = RGB_BLACK;
	(m_pDiag + pos->dpno)->SetAttribute( pos->dpindex, (LPUNKNOWN)&da, TRUE );

	dd.data_disp = buf;
	dd.data_imgdata = NULL;

	(m_pDiag + pos->dpno)->SetData( pos->dpindex, (LPUNKNOWN)&dd );
}

//----------------------------------------
//	借方合計表示
//	貸方合計表示
//----------------------------------------
void CRDdnpBaseDlg::total_dsp()
{
	DNP_POS* pos;
	pos = m_pKeiPos;

	// 一仕訳あたりのDiag項目数
	int cnt = 0;
	for( int i = 0; (pos + i)->dpno != -1; i++ ) {
#ifdef LATER_CLOSE
		if( (pos+i)->dp_pn == FD_DBVAL_PN ) {
			if( DEBT_TOTAL_CND.INP_sg ) {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, (char*)DEBT_TOTAL.DP_VAL );
			}
			else {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, NULL );
			}
		}
		else if( (pos+i)->dp_pn == FD_CRVAL_PN ) {
			if( CRED_TOTAL_CND.INP_sg ) {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, (char*)CRED_TOTAL.DP_VAL );
			}
			else {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, NULL );
			}
		}
#endif
	}
}

/*
void CRDdnpBaseDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete this;
//	ICSDialog::PostNcDestroy();
}
*/

BOOL CRDdnpBaseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR ) {
			long code = pMsg->wParam == VK_NEXT ? SB_PAGEDOWN : SB_PAGEUP;
			(m_pDiag+1)->ScrollPos( code, 0);
			return TRUE;
		}
		else if( pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP ) {
			long code = pMsg->wParam == VK_DOWN ? SB_LINEDOWN : SB_LINEUP;
			(m_pDiag+1)->ScrollPos( code, 0);
			return TRUE;
		}
	}
	else if( pMsg->message == WM_CHAR ) {
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


//--------------------------------------------------
//	ダイアグの設定
//	int bmon	部門ありか？
//	int koji	工事ありか？
//--------------------------------------------------
void CRDdnpBaseDlg::diag_inz()
{
	DNP_POS* p;
	int ln, n, i, idx;
	DIAGRAM_ATTRIBUTE da;

	// 仕訳ヘッド部
	p = m_pHDpos;

	BOOL bEnable;

	for( i = 0; (p+i)->dpno != -1; i++ ) {
		idx = (p+i)->dpindex;

		(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );

		// 入力レングス設定
		if( (p+i)->dp_pn == DP_BMN_PN ) {
			(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
			da.attr_inpmax = m_BmonLNG;
			(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

			if( m_BmonLNG > 0 )	bEnable = TRUE;
			else				bEnable = FALSE;
			ctl_init( (p+i)->dp_pn, bEnable );
		}
		else if( (p+i)->dp_pn == DP_KOJI_PN ) {
			(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
			da.attr_inpmax = m_KojiLNG;
			(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

			if( m_KojiLNG > 0 )	bEnable = TRUE;
			else				bEnable = FALSE;
			ctl_init( (p+i)->dp_pn, bEnable );
		}
	}

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_AllLineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			idx = GetPosIndex( ln, (p+i) );
	
			(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );

			// 入力レングス設定
			if( (p+i)->dp_pn == FD_DBMN_PN || (p+i)->dp_pn == FD_CBMN_PN ) {
				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = m_BmonLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

				if( m_BmonLNG > 0 )	bEnable = TRUE;
				else				bEnable = FALSE;
				ctl_init( (p+i)->dp_pn, bEnable );
			}
			else if( (p+i)->dp_pn == FD_DKOJI_PN || (p+i)->dp_pn == FD_CKOJI_PN ) {
				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = m_KojiLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

				if( m_KojiLNG > 0 )	bEnable = TRUE;
				else				bEnable = FALSE;
				ctl_init( (p+i)->dp_pn, bEnable );
			}
		}
	}
}


//--------------------------------------------------
//	Diag の 仕訳データ部の部門・工事の設定
//	BOOL bTanBmn	単一部門入力か？
//--------------------------------------------------
void CRDdnpBaseDlg::diag_tanbmninz( BOOL bTanBmn )
{
	DNP_POS* p;
	int ln, n, i, idx;
	DIAGRAM_ATTRIBUTE da;

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			idx = GetPosIndex( ln, (p+i) );
	
			// 入力レングス設定
			if( (p+i)->dp_pn == FD_DBMN_PN || (p+i)->dp_pn == FD_CBMN_PN ) {
				// 入力可 or 不可

				if( (p+i)->dp_enable ) {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, bTanBmn ? DIAG_MDFY_READONLY : DIAG_MDFY_EDIT );
				}
				else {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
				}

				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = bTanBmn ? 0 : m_BmonLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
			}
			else if( (p+i)->dp_pn == FD_DKOJI_PN || (p+i)->dp_pn == FD_CKOJI_PN ) {
				// 入力可 or 不可
				if( (p+i)->dp_enable ) {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, bTanBmn ? DIAG_MDFY_READONLY : DIAG_MDFY_EDIT );
				}
				else {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
				}

				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = bTanBmn ? 0 : m_KojiLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
			}
		}
	}
}


//
// 入力不可 イニシャライズ
//
void CRDdnpBaseDlg::diag_disableinz()
{
	DNP_POS* p;
	int ln, n, i, idx;
	DIAGRAM_ATTRIBUTE da;

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			idx = GetPosIndex( ln, (p+i) );

#ifdef LATER_CLOSE
			if( (p+i)->dp_pn == FD_HJIYU_PN ) {
			//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) {	// 社会福祉用
				if( IsHjiyuItemGyosyu() == 0 ) {
					// 入力不可
					if( m_bHjiyuEna ) {
						(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
						m_bHjiyuEna = FALSE;
						(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
						da.attr_inpmax = 0;
						(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
					}
				}
				else {
					if( ! m_bHjiyuEna ) {
						(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_EDIT );
						m_bHjiyuEna = TRUE;
					}
				}
			}
			else if( (p+i)->dp_pn == FD_DSYOG_PN || (p+i)->dp_pn == FD_CSYOG_PN ) {
				if( Voln1->apno > 0x50 ) {	// 公益以降
					// 入力不可
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
					(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
					da.attr_inpmax = 0;
					(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
				}
			}
			else if( (p+i)->dp_pn == FD_DBZEI_PN || (p+i)->dp_pn == FD_CRZEI_PN ) {
				if( p_DnpView->IsZeigkMdfy() ) {
					// 入力可
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_EDIT );
				}
				else {
					// 入力不可
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
				}
			}
#endif
		}
	}
}




// 仕訳データ色の 変更
void CRDdnpBaseDlg::SetDataLineColor( int ln, COLORREF back, COLORREF txt )
{
	DIAGRAM_ATTRIBUTE da;

	//仕訳データ
	DNP_POS* pos;
	pos = m_pDTpos;
	int dataidx;
	dataidx = GetPosIndex( ln, pos );

	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			break;
		}
		// 色の変更
		dataidx = GetPosIndex( ln, pos );

		(m_pDiag + pos->dpno)->GetAttribute( dataidx, (LPUNKNOWN)&da );

		da.attr_bcolor	= back;
		da.attr_frcolor	= back;
//		da.attr_fcolor	= txt;

		(m_pDiag + pos->dpno)->SetAttribute( dataidx, (LPUNKNOWN)&da, FALSE );
	}
	// 再描画
	(m_pDiag + 1)->Refresh();

}




void CRDdnpBaseDlg::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_pDiag == NULL )	return;

	ControlPos();
}








// 修正 文言
void CRDdnpBaseDlg::ChangeJob( int job )
{
}

//
//	他のウィンドウからの コントロールへのカーソル移動
//
void CRDdnpBaseDlg::cursor_set( UINT id )
{
	GotoDlgCtrl( GetDlgItem( id ) );
}


//--------------------------------------------
//	ダイアグラム再描画
//--------------------------------------------
int CRDdnpBaseDlg::DiagRefresh()
{
	DNP_POS* p;

	// 仕訳データ部
	p = m_pDTpos;

	this->Invalidate();
	this->UpdateWindow();


	for( int i = 0; (p+i)->dpno != -1; i++ ) {
		// 入力可能項目のみ
		(m_pDiag + (p+i)->dpno)->Refresh();
		break;
	}

	return 0;
}

// ダイアグラムのデータ表示以外の線項目の個数を返送
int CRDdnpBaseDlg::GetLineItemCount()
{
	int ln_item = (m_ItemCount - (m_DclmCount*m_LineCount)) + 1;

	return ln_item;
}



// 行位置(１～)と、DNP_POS から、データインデックスを返送する。
int CRDdnpBaseDlg::GetPosIndex( int ln, DNP_POS* pos )
{
	int data_pg, index, ln_sbn, ln_cnt;

	data_pg = ln / m_LineCount;
	if( (ln % m_LineCount) )	data_pg++;

	ln_cnt = GetLineItemCount();

	index = pos->dpindex + ((ln-1) * m_DclmCount);
	ln_sbn = (data_pg-1)*ln_cnt;
	index += ln_sbn;

	return index;
}


// 定型伝票ラベル名
void CRDdnpBaseDlg::SetRDNPLabel( CString& label )
{
	m_sLabel = label;
}



BOOL CRDdnpBaseDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	long code = (zDelta > 0) ? SB_LINEUP : SB_LINEDOWN;

	if( zDelta > 0 ) {
		keybd_event( VK_UP, 0, KEYEVENTF_EXTENDEDKEY, 0 );
		keybd_event( VK_UP, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0 );
	}
	else {
		keybd_event( VK_DOWN, 0, KEYEVENTF_EXTENDEDKEY, 0 );
		keybd_event( VK_DOWN, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0 );
	}

	//	(m_pDiag+1)->ScrollPos( code, 0);

	return ICSDialog::OnMouseWheel(nFlags, zDelta, pt);
}
