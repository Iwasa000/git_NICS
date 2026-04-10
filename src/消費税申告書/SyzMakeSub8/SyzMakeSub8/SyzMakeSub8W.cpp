#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CSyzMakeSubW::CSyzMakeSubW()
	: m_pWdlg(NULL)
	, m_owner_id(0)
	, m_plcusr(FALSE)
	, m_isKjok(TRUE)
{
	memset( m_mkCoName, '\0', sizeof(m_mkCoName) );
	memset( m_KIsgn, '\0', (sizeof(BYTE))*5*MONTHALL );
	memset( m_MSymd, '\0', (sizeof(UCHAR))*MONTHALL*5*3 );
	memset( m_MEymd, '\0', (sizeof(UCHAR))*MONTHALL*5*3 );
	memset( m_KIsgn, '\0', (sizeof(BYTE))*5*MONTHALL );
	memset( m_KJsgn, '\0', (sizeof(BYTE))*5*MONTHALL );
	memset( m_nMNdt1, '\0', sizeof(m_nMNdt1) );
	memset( m_nMNdt2, '\0', sizeof(m_nMNdt2) );
	memset( m_nInsw, '\0', sizeof(m_nInsw) );
	memset( m_nZKsgn, '\0', sizeof(m_nZKsgn) );

	//--->20.09.08
	Utiwake_ver = 0;
	Vtbl_utiwakeback = 0;
	//<----

}

//-----------------------------------------------------------------------------
// デストタクタ
//-----------------------------------------------------------------------------
CSyzMakeSubW::~CSyzMakeSubW()
{
	if( m_pWdlg != NULL ){
		m_pWdlg->SendMessage( WM_COMMAND, IDCANCEL );
		m_pWdlg->DestroyWindow();
		delete m_pWdlg;
		m_pWdlg = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージ取得
//-----------------------------------------------------------------------------
// 返送値	：	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CSyzMakeSubW::GetErrorMsg()
{
	return m_ErrMsg;
}

//-----------------------------------------------------------------------------
// 消費税の状態チェック
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	マスターに接続済の財務クラスのポインタ
//		pMsg	：	表示するメッセージの保存先
//		ChkInfo	：	状態チェック付加情報
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			1	：	正常終了（メッセージを表示する必要があります）
//			-1	：	エラー	（処理エラー）
//			-2	：	エラー	（マスタエラー）
//			-3	：	エラー	（マスタエラー：消費税申告書の法人／個人違い）
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckSyzState( CDBZmSub *pZmsub, CString *pMsg, SYZ_CHKINFO &ChkInfo )
{
	ASSERT( pZmsub );
	ASSERT( pZmsub->m_database );
	ASSERT( pMsg );
	pMsg->Empty();
	m_GuideMsg.Empty();

	if( (ChkInfo.calltype<ID_ICSSYZ_ZAIMU_CHK_CALLTYPE) || (ID_ICSSYZ_ZEIMU_CHK_CALLTYPE<ChkInfo.calltype) ){
		m_ErrMsg = _T("不正な引数が指定されました！");
		return -1;
	}

	int	rt = 0;

	// バージョンと期末年月日
	CRecordset	tmpZmRs( pZmsub->m_database );
	CString		sql, strSver, strEymd, strApno, strSsgn1, strZ_fail;
	sql.Format( _T("SELECT s_ver, ee_ymd, apno, s_sgn1, z_fail from zvolume") );
	try{
		tmpZmRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpZmRs.IsEOF() ){
			tmpZmRs.GetFieldValue( (SHORT)0, strSver );
			tmpZmRs.GetFieldValue( (SHORT)1, strEymd );
			tmpZmRs.GetFieldValue( (SHORT)2, strApno );
			tmpZmRs.GetFieldValue( (SHORT)3, strSsgn1 );
			tmpZmRs.GetFieldValue( (SHORT)4, strZ_fail );
		}
		else {
			tmpZmRs.Close();
			if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
				m_ErrMsg = _T("マスター内に消費税に関する情報がありません。\nマスター修復処理をおこなってください。");
			}
			else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
				m_ErrMsg = _T("基本情報内に、消費税に関する情報が無い不正な財務マスターです。");
				m_GuideMsg = _T("マスター修復処理をおこなってください。");
			}
			return -2;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpZmRs.Close();

	int	s_ver = atoi( strSver );
	int eymd = atoi( strEymd );
	int apno = atoi( strApno );
	int s_sgn1 = atoi( strSsgn1 );
	int z_fail = atoi( strZ_fail );

	if( z_fail == 1 ){
		// '14.06.25:マスター修復が必要なマスターは消費税バージョンを行わずに返す
		return 0;
	}

	// 動作可能な消費税バージョン？
	if( s_ver > VALID_SYZ_VER ){
		if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
			m_ErrMsg.Format( _T("このマスター(s_ver=%d)は、当プログラム(s_ver=%d)では扱えません。\n対応バージョンのプログラムを使用してください。"), s_ver, VALID_SYZ_VER );
		}
		else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
			m_ErrMsg.Format( _T("当プログラム(s_ver=%d)では扱えない財務マスター(s_ver=%d)です。"), VALID_SYZ_VER, s_ver );
			m_GuideMsg = _T("対応バージョンのプログラムを使用してください。");
		}
		return -2;
	}

//--> '14.05.09 CUT START
//	// 期末日が施行日以前？
//	if( eymd < PER8_SYZ_YMD ){
//		return 0;
//	}
//<-- '14.05.09 CUT END

	if( PER8_SYZ_VER > s_ver ){
		// 5->8 へのコンバージョン処理
		if( VALID_SYZ_VER >= PER8_SYZ_VER ){

//--> '14.05.09 INS START
			// 業種違いのチェックが必要
			CVerUpSub8	Vupsub;
			int st = Vupsub.VerUpIndTypeDif(pZmsub);
			if( st == -1 ){
				m_ErrMsg = Vupsub.GetVerUpSubErrMsg();
				return -1;
			}
			else if( st == 1 ){
				m_ErrMsg = _T("消費税申告書で【個人事業者】が選択されているため、バージョンアップできません。\r\n「会社登録・修正・削除」業務で業種区分を【個人事業】に切替えてください。");
				return -3;
			}

			if( Vupsub.VerUpExec(pZmsub, NULL, 0x00) == -1 ){
				m_ErrMsg = Vupsub.GetVerUpSubErrMsg();
				return -1;
			}
//<-- '14.05.09 INS END

			// 施行日以降の仕訳アリ？
/*- '14.05.20 -*/
//			CRecordset	tmpRs( pZmsub->m_database );
//			CString	sql, tblCnt;
//			sql.Format( _T("SELECT COUNT(*) FROM datarec WHERE ddate >= %d"), PER8_SYZ_YMD );
//			try{
//				tmpRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
//				if( !tmpRs.IsEOF() ){
//					tmpRs.GetFieldValue( (SHORT)0, tblCnt );
//				}
//				else {
//					tmpRs.Close();
//					if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
//						m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nマスター修復処理をおこなってください。");
//					}
//					else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
//						m_ErrMsg = _T("仕訳に関する情報が無い不正な財務マスターです。");
//						m_GuideMsg = _T("マスター修復処理をおこなってください。");
//					}
//					return -2;
//				}
//			}
//			catch( CDBException *pDe ){
//				m_ErrMsg = pDe->m_strError;
//				pDe->Delete();
//				return -1;
//			}
//			tmpRs.Close();
//
//			int	nTblCnt = atoi( tblCnt );
/*-------------*/

			int	nTblCnt = 0;
			if( !(ChkInfo.verupsub&0x02) ){
				
				CRecordset	tmpRs( pZmsub->m_database );
				CString	sql, tblCnt;
				sql.Format( _T("SELECT COUNT(*) FROM datarec WHERE ddate >= %d"), PER8_SYZ_YMD );
				try{
					tmpRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
					if( !tmpRs.IsEOF() ){
						tmpRs.GetFieldValue( (SHORT)0, tblCnt );
					}
					else {
						tmpRs.Close();
						if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
							m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nマスター修復処理をおこなってください。");
						}
						else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
							m_ErrMsg = _T("仕訳に関する情報が無い不正な財務マスターです。");
							m_GuideMsg = _T("マスター修復処理をおこなってください。");
						}
						return -2;
					}
				}
				catch( CDBException *pDe ){
					m_ErrMsg = pDe->m_strError;
					pDe->Delete();
					return -1;
				}
				tmpRs.Close();

				nTblCnt = atoi( tblCnt );
			}
/*-------------*/
			if( nTblCnt > 0 ){
				// 仕訳アリ
/*- '14.06.05 -*/
//				// メッセージを表示して、明示的なコンバージョン処理への誘導を行う。
//				if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
//					m_ErrMsg.Format( _T("このマスター(s_ver=%d)は、施行日以降に仕訳が入力されています。\n自動更新が行えませんので、消費税コンバージョンを行ってください。"), s_ver );
//				}
//				else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
//					m_ErrMsg.Format( _T("消費税バージョンアップ前に、施行日以降の仕訳が入力されてる不正な財務マスター(s_ver=%d)です。"), s_ver );
//					m_GuideMsg = _T("消費税コンバージョンを行ってください。");
//				}
//				rt = -2;
/*-------------*/
				// 残高再集計
				if( ((apno&0xff)==0x51) && ((s_sgn1&0xc0)==0xc0) ){
					if( db_recalc_ex(pZmsub, NULL, m_ErrMsg) ){
						m_ErrMsg = _T("残高再集計処理に失敗しました。");
						return -1;
					}
				}
				SYZ_CNVINFO	cnvInfo = {0};
				long		tmpMstst = 0;
				cnvInfo.cnvSw |= 0x01;
				if( ConversionToZei8(pZmsub, cnvInfo, tmpMstst) ){
					return -1;
				}
				if( tmpMstst&0x04 ){
					pMsg->Format( _T("平成26年4月消費税改正のバージョンアップを行いました。\r\n平成26年4月以降の仕訳は、税率８％に変換しました。\r\n仕訳をご確認ください。") );
					rt = 1;
				}
/*-------------*/
			}
			else{
				// 仕訳ナシ

				// 自動コンバージョンを行って、メッセージを表示する
				if( Conversion(s_ver, pZmsub, IC_ICSSYZ_5_TO_8_CNV) ){
					rt = -1;
				}
				else{
					if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
						if( ChkInfo.verupsub&0x01 ){
							rt = 0;
						}
						else{
							pMsg->Format( _T("このマスター(s_ver=%d)は、改正消費税対応版(s_ver=%d)に更新されました。"), s_ver, PER8_SYZ_VER );
							rt = 1;
						}
					}
					else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
						// 更新前のマスタを税務の財務連動側から呼び出された時には、更新完了メッセージを表示しないように！
						rt = 0;
					}
					s_ver = PER8_SYZ_VER;
				}
			}
		}
	}

//--> '14.12.12 INS START
	// szvol の更新
	int st=0, tmp_SVvers=0;
	if( (st=GetSzvolumeInfo(pZmsub->m_database, tmp_SVvers)) == -1 ){
		return -1;
	}
	else if( st == 1 ){
		m_ErrMsg = _T("マスター内に消費税管理情報(szvol)がありません。\nマスター修復処理をおこなってください。");
		return -1;
	}
	else{
		// バージョンを見てコンバージョン処理へ
		if( KANI6_SYZ_SZVOL_VER > tmp_SVvers ){
			if( ConversionToKani6(pZmsub) ){
				return -1;
			}
		}
	}
//<-- '14.12.12 INS END

//--> '19.02.21 CUT START
//	// 10%対応に辺り、下の処理を通すように
//	return rt;
//<-- '19.02.21 CUT END

//== '13.08.06 ==
	// 以降の処理は、10% 改正時をみこしてテスト的に作成。

	if( !(ChkInfo.verupsub&0x08) && (IsAbleToAutoVup()==false) ){
		// 期末日が施行日以前？
		if( eymd < PER10_SYZ_YMD ){
			return 0;
		}
		// 10％改正：一次対応では、自動バージョンアップは行わないので、ここで処理を返す。
		return rt;
	}

	if( PER10_SYZ_VER > s_ver ){
		// 8->10 へのコンバージョン処理
		if( VALID_SYZ_VER >= PER10_SYZ_VER ){
#ifdef _20190822_CUT_	//== 施工日以降の仕訳チェックは、フラグがオフの時にしか行わない！ ==
			// 施行日以降の仕訳アリ？
			CRecordset	tmpRs( pZmsub->m_database );
			CString	sql, tblCnt;
			sql.Format( _T("SELECT COUNT(*) FROM datarec WHERE ddate >= %d"), PER10_SYZ_YMD );
			try{
				tmpRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
				if( !tmpRs.IsEOF() ){
					tmpRs.GetFieldValue( (SHORT)0, tblCnt );
				}
				else {
					tmpRs.Close();
					if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
						m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nマスター修復処理をおこなってください。");
					}
					else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
						m_ErrMsg = _T("基本情報内に、消費税に関する情報が無い不正な財務マスターです。");
						m_GuideMsg = _T("マスター修復処理をおこなってください。");
					}
					return -2;
				}
			}
			catch( CDBException *pDe ){
				m_ErrMsg = pDe->m_strError;
				pDe->Delete();
				return -1;
			}
			tmpRs.Close();

			int	nTblCnt = atoi( tblCnt );
#else
			int	nTblCnt = 0;
#endif

			if( !(ChkInfo.verupsub&0x04) ){

				CRecordset	tmpRs(pZmsub->m_database);
				CString	sql, tblCnt;
				sql.Format(_T("SELECT COUNT(*) FROM datarec WHERE ddate >= %d"), PER10_SYZ_YMD);
				try{
					tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
					if( !tmpRs.IsEOF() ){
						tmpRs.GetFieldValue((SHORT)0, tblCnt);
					}
					else {
						tmpRs.Close();
						if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
							m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nマスター修復処理をおこなってください。");
						}
						else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
							m_ErrMsg = _T("仕訳に関する情報が無い不正な財務マスターです。");
							m_GuideMsg = _T("マスター修復処理をおこなってください。");
						}
						return -2;
					}
				}
				catch( CDBException *pDe ){
					m_ErrMsg = pDe->m_strError;
					pDe->Delete();
					return -1;
				}
				tmpRs.Close();

				nTblCnt = atoi(tblCnt);
			}

			if( nTblCnt > 0 ){
				// 仕訳アリ
				// メッセージを表示して、明示的なコンバージョン処理への誘導を行う。
				if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
					if( apno <= 0x30 ){
						m_ErrMsg.Format(_T("このマスター(s_ver=%d)は、施行日以降の仕訳が入力されているため、自動更新が行えません。\n「導入･更新」→「令和1年10月消費税改正／医療法人改正」で消費税バージョンアップを行ってください。"), s_ver);
					}
					else{
						m_ErrMsg.Format(_T("このマスター(s_ver=%d)は、施行日以降の仕訳が入力されているため、自動更新が行えません。\n「導入･更新」→「令和1年10月消費税改正」で消費税バージョンアップを行ってください。"), s_ver);
					}
				}
				else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
					m_ErrMsg.Format( _T("バージョンアップ(s_ver=%d)前に、施行日以降に仕訳が入力されてる不正な財務マスターです。"), PER10_SYZ_VER);
					m_GuideMsg = _T("消費税バージョンアップを行ってください。");
				}
				rt = -2;
			}
			else{
				// 仕訳ナシ
				// 自動コンバージョンを行って、メッセージを表示する
//-- CUT --
// この内部処理は変更する必要があります
//				if( Conversion(s_ver, pZmsub) ){
//					*pMsg = m_ErrMsg;
//					rt = -1;
//				}
//				else{
//					m_ErrMsg.Format( _T("このマスター(s_ver=%d)は、改正消費税対応版(s_ver=%d)に更新されました。"), s_ver, PER8_SYZ_VER );
//					*pMsg = m_ErrMsg;
//					rt = 1;
//				}
//---------

				// 自動コンバージョンを行って、メッセージを表示する
				if( Conversion(s_ver, pZmsub, IC_ICSSYZ_8_TO_10_CNV) ){
					rt = -1;
				}
				else{

					if( apno == KENSETU_APNO ){
						// 完成時振替コンバージョン
						bool zOpenflg = false;
						if( pZmsub->zvol == NULL ){
							if( pZmsub->VolumeOpen() ){
								m_ErrMsg = pZmsub->GetZmErrMsg();
								return -1;
							}
							zOpenflg = true;
						}

						bool	bIsChanged = false;
						if( CnvKensetuFurikaeShiawke(pZmsub, bIsChanged) ){
							return -1;
						}

						if( zOpenflg ){
							pZmsub->VolumeClose();
							zOpenflg = false;
						}
					}

					if( ChkInfo.calltype == ID_ICSSYZ_ZAIMU_CHK_CALLTYPE ){
						if( ChkInfo.verupsub&0x01 ){
							rt = 0;
						}
						else{
							pMsg->Format(_T("このマスター(s_ver=%d)は、改正消費税対応版(s_ver=%d)に更新されました。"), s_ver, PER10_SYZ_VER);
							rt = 1;
						}
					}
					else if( ChkInfo.calltype == ID_ICSSYZ_ZEIMU_CHK_CALLTYPE ){
						// 更新前のマスタを税務の財務連動側から呼び出された時には、更新完了メッセージを表示しないように！
						rt = 0;
					}
					s_ver = PER10_SYZ_VER;
				}


			}
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// szvolの情報を取得する
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		SVvers	：	ボリュームバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			1	：	消費税ボリュームラベル登録無し
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetSzvolumeInfo( class CDatabase *pChkDB, int &SVvers )
{
	int	rt = 0;

	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		m_ErrMsg = _T("不正な引数が指定されました！");
		return -1;
	}

	CRecordset	tmpZmRs( pChkDB );
	CString		sql, strSVvers;
	sql = GET_SVVER_MTYPE_FROM_SZVOL_SQL;
	try{
		tmpZmRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpZmRs.IsEOF() ){
			tmpZmRs.GetFieldValue( (SHORT)0, strSVvers );
		}
		else{
			// 消費税ボリュームラベル登録無し
			rt = 1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpZmRs.Close();

	// 変換
	SVvers = atoi( strSVvers );

	return 0;
}

//-----------------------------------------------------------------------------
// 平成27年改正 簡易課税第６種コンバージョン ('14.12.12)
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	マスターに接続済の財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ConversionToKani6( CDBZmSub *pZmsub )
{
	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		m_ErrMsg = _T("不正な引数が指定されました！");
		return -1;
	}

	// szvol 更新
	CString	sql;
	pZmsub->m_database->BeginTrans();
	try{
		sql = MENT_SZVOL_FOR_KANI6_SQL;
		pZmsub->m_database->ExecuteSQL( sql );
		sql.Format( VUP_SZVOL_FOR_KANI6_SQL, KANI6_SYZ_SZVOL_VER );
		pZmsub->m_database->ExecuteSQL( sql );
	}
	catch( CDBException *pDe ){
		pZmsub->m_database->Rollback();
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	pZmsub->m_database->CommitTrans();

	return 0;
}


//-----------------------------------------------------------------------------
// データ変換
//-----------------------------------------------------------------------------
// 引数	nowVersion	：	現バージョン
//		pZmsub		：	マスターに接続済の財務クラスのポインタ
//		cnvtype		：	コンバージョンタイプ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::Conversion( int nowVersion, CDBZmSub *pZmsub, EnumIdDataCnvType cnvtype )
{
	// ボリュームラベル内のバージョンを上げる(zvolume.s_ver)

	// 消費税管理ボリューム内の基本税率・経過税率・複数税率処理スイッチ
	CString	sql;

	if( IC_ICSSYZ_5_TO_8_CNV <= cnvtype ){
		// 同一バージョン時の再バージョンアップを可能に！
		if( ((nowVersion==PER8_SYZ_VER)&&(cnvtype==IC_ICSSYZ_5_TO_8_CNV)) || (nowVersion<PER8_SYZ_VER) ){
			pZmsub->m_database->BeginTrans();
			try{
				sql = VUP_SZVOL_FOR_H26SYZ_SQL;
				pZmsub->m_database->ExecuteSQL(sql);
				sql.Format(VUP_ZVOL_FOR_H26SYZ_SQL, PER8_SYZ_VER);
				pZmsub->m_database->ExecuteSQL(sql);
			}
			catch( CDBException *pDe ){
				pZmsub->m_database->Rollback();
				m_ErrMsg = pDe->m_strError;
				pDe->Delete();
				return -1;
			}
			pZmsub->m_database->CommitTrans();
		}
	}
	if( IC_ICSSYZ_8_TO_10_CNV <= cnvtype ){
		if( ((nowVersion==PER10_SYZ_VER)&&(cnvtype==IC_ICSSYZ_8_TO_10_CNV)) || (nowVersion<PER10_SYZ_VER) ){
			pZmsub->m_database->BeginTrans();
			try{
				sql = VUP_SZVOL_FOR_H31SYZ_SQL;
				pZmsub->m_database->ExecuteSQL(sql);
				sql.Format(VUP_ZVOL_FOR_H31SYZ_SQL, PER10_SYZ_VER);
				pZmsub->m_database->ExecuteSQL(sql);
			}
			catch( CDBException *pDe ){
				pZmsub->m_database->Rollback();
				m_ErrMsg = pDe->m_strError;
				pDe->Delete();
				return -1;
			}
			pZmsub->m_database->CommitTrans();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// データベースマスターの削除
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//			-2		：	例外
//-----------------------------------------------------------------------------
int CSyzMakeSubW::DeleteDBMaster( SYZMAKE_MKCOINFO &mkcoinfo )
{
	int	rt = 0;

	if( m_Mctl.SetDatabase(mkcoinfo.dbSvrName) < 0 ){
		m_ErrMsg = _T("マスターデータベースのオープンに失敗しました！");
		return -1;
	}
	if( m_Mctl.mctl_getphydisk(m_phydisk) == -1 ){
		m_ErrMsg = _T("マスター格納パスの取得に失敗しました！");
		return -1;
	}
	if( m_phydisk.IsEmpty() == TRUE ){
		m_ErrMsg = _T("マスター格納パスが設定されていません！");
		return -1;
	}
	// DB アクセスクラスオープン
	if( m_DBAccess.OpenDatabaseEx() < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -2;
	}

	// マスターの削除（会社情報）
	bool	haveImg = false;
	int st = DeleteMasterFromVctl(mkcoinfo, haveImg);
	if( st == -1 ){	// DB 上のエラーの時のみエラーとする
		return -1;
	}

	// データベース全削除
	rt = DropDatabaseAll( mkcoinfo, haveImg );
	if( rt < 0 ){
		return rt;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 財務サブスイッチ更新
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//			-2	：	エラー終了
//-----------------------------------------------------------------------------
int CSyzMakeSubW::UpdateZmsubSubsw( SYZMAKE_MKCOINFO &mkcoinfo )
{
	CDBZmSub	*pZmsub = NULL;
	if( (pZmsub = new CDBZmSub()) == NULL ){
		// エラーメッセージ
		return -1;
	}
	pZmsub->ExitMode( 1 );

	// マスター接続
//-- '15.07.10 --
//	if( pZmsub->SetMasterDB(1, mkcoinfo.dbSvrName, mkcoinfo.apno, mkcoinfo.cocode, mkcoinfo.ee_ymd) < 0 ){
//--------------
	if( pZmsub->SetMasterDB(3, mkcoinfo.dbSvrName, mkcoinfo.apno, mkcoinfo.cocode, mkcoinfo.ee_ymd) < 0 ){//20.09.08
//	if( pZmsub->SetMasterDB(2, mkcoinfo.dbSvrName, mkcoinfo.apno, mkcoinfo.cocode, mkcoinfo.ee_ymd) < 0 ){
//--------------
		return -2;
	}

	// ボリュームラベル
	if( pZmsub->VolumeOpen() == -1 ){
		return -2;
	}
	// sub_sw 修正
	pZmsub->zvol->Edit();
//	pZmsub->zvol->sub_sw |= (mkcoinfo.sub_sw&0xffff0000);
	pZmsub->zvol->sub_sw |= (mkcoinfo.sub_sw&0xffff8000);
//--> '15.07.10 INS START
	char	ss[64]={0};
	memmove( ss, mkcoinfo.hojin_no, sizeof(mkcoinfo.hojin_no) );
	pZmsub->zvol->hojin_no.Format( _T("%s"), ss );
	memset( ss, '\0', sizeof(ss) );
	memmove( ss, mkcoinfo.hojin_no_ext, sizeof(mkcoinfo.hojin_no_ext) );
	pZmsub->zvol->hojin_no_ext.Format( _T("%s"), ss );
	if( MynoAccessPerm() ){
		if( (pZmsub->zvol->ind_type!=0x00) || (pZmsub->zvol->apno==0x10) ){
			BYTE	bb[64]={0};
			memmove( bb, mkcoinfo.kojin_hash, sizeof(mkcoinfo.kojin_hash) );
			pZmsub->zvol->kojin_hash.RemoveAll();
			for( int i=0; i<sizeof(mkcoinfo.kojin_hash); i++ ){
				pZmsub->zvol->kojin_hash.Add( bb[i] );
			}
		}
	}
//<-- '15.07.10 INS END
	pZmsub->zvol->Update();

	pZmsub->VolumeClose();
	delete pZmsub;
	pZmsub = NULL;

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税不正仕訳のチェック
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	マスターに接続済の財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckWrongData( CDBZmSub *pZmsub )
{
	ASSERT( pZmsub );
	ASSERT( pZmsub->m_database );
	if( pZmsub->m_database == NULL ){
		m_ErrMsg = _T("財務マスターがオープンされていません！");
		return -1;
	}

	int	rt = 0;

	// 消費税バージョン
	int	s_ver=0, m_type=0;
	if( GetZvolumeSverMtype(pZmsub->m_database, s_ver, m_type) ){
		return -1;
	}

	// 不正仕訳のチェック
/*- '13.10.05 -*/
//	if( (s_ver==PER8_SYZ_VER) && (VALID_MKDLL_VER==PER5_SYZ_VER) ){
/*-------------*/
	if( (s_ver>PER5_SYZ_VER) && (VALID_MKDLL_VER==PER5_SYZ_VER) ){
/*-------------*/
		// 税率８％の仕訳有り！
		int st = ChkInputZei8Data( pZmsub->m_database );
		if( st == -1 ){
			return -1;
		}
		else if( st == 1 ){
			m_ErrMsg.Format( _T("このマスター(s_ver=%d)は、平成２６年４月消費税改正施行日以降の仕訳が入力されています。\n施行日以降の計算は未対応のため、処理を行えません。プログラムが対応するまでお待ちください。"), s_ver );
			rt = -1;
		}
	}
/*- '19.06.03 -*/
//	else if( (s_ver>PER8_SYZ_VER) && (VALID_MKDLL_VER==PER8_SYZ_VER) ){
/*-------------*/
	else if( (s_ver>PER8_SYZ_VER) && (VALID_MKDLL_VER==PER8_SYZ_VER) ){
/*-------------*/
				// 税率10％の仕訳有り！
		int st1 = ChkInputZei10Data(pZmsub->m_database);
		int st2 = ChkInputZei8RdData(pZmsub->m_database);
		int st3 = ChkInputExtImportData(pZmsub->m_database, -1, 99999999);
		int st4 = ChkInputKojiFurikaeData(pZmsub->m_database, -1, 99999999);
		if( (st1==-1) || (st2==-1) || (st3==-1) || (st4==-1) ){
			return -1;
		}
		else if( (st1==1) || (st2==1) || (st3==1) || (st4==1) ){
			if( (st1==1) || (st2==1) ){
				m_ErrMsg.Format(_T("このマスター(s_ver=%d)は、消費税率10％または8％軽減税率の仕訳が入力されています。\n10％・8％軽減税率の計算は未対応のため、処理を行えません。プログラムが対応するまでお待ちください。"), s_ver);
			}
			else if( st3==1 ){
				m_ErrMsg.Format(_T("このマスター(s_ver=%d)は、仕訳区分「輸入消費税」の仕訳が入力されています。\n仕訳区分「輸入消費税」の集計は未対応のため、処理を行えません。プログラムが対応するまでお待ちください。"), s_ver);
			}
			else if( st4==1 ){
				m_ErrMsg.Format(_T("このマスター(s_ver=%d)は、仕訳区分「完成時振替」の仕訳が入力されています。\n仕訳区分「完成時振替」の集計は未対応のため、処理を行えません。プログラムが対応するまでお待ちください。"), s_ver);
			}
			rt = -1;
		}
	}
	else if( s_ver == PER10_SYZ_VER ){
		CZmGen8	zmgen;
		if( zmgen.IsInvoiceVupMaster(pZmsub) == false ){
			if( ChkDataAfterDate(pZmsub->m_database, INVOICE_SYZ_YMD, false) > 0 ){
				m_ErrMsg.Format(_T("このマスター(s_ver=%d)は、令和5年10月1日以降の仕訳が入力されています。\n令和5年10月1日以降の計算は未対応のため、処理を行えません。プログラムが対応するまでお待ちください。"), s_ver);
				rt = -1;
			}
			if( (m_type&0x01) == 0x01 ){
				if( CheckZanAfterInvoice(pZmsub) > 0 ){
					m_ErrMsg.Format(_T("このマスター(s_ver=%d)は、令和5年10月1日以降の消費税残高が集計されています。\n令和5年10月1日以降の計算は未対応のため、処理を行えません。プログラムが対応するまでお待ちください。"), s_ver);
					rt = -1;
				}
			}
		}
	}
	

//--> '15.02.10 INS START
//---> '15.02.18 CUT START
//	else if( s_ver >= PER8_SYZ_VER ){
//		if( IsKaniMaster(pZmsub->m_database) == TRUE ){
//			// 簡易課税第６種の仕訳有り？
//			int st = ChkInputZeiKani6Data( pZmsub->m_database );
//			if( st == -1 ){
//				return -1;
//			}
//			else if( st == 1 ){
//				m_ErrMsg.Format( _T("このマスター(s_ver=%d)は、簡易課税第六種事業の仕訳が入力されています。\n簡易課税第六種事業の計算は未対応のため、処理を行えません。プログラムが対応するまでお待ちください。"), s_ver );
//				rt = -1;
//			}
//		}
//
//	}
//<--- '15.02.18 CUT END
//<-- '15.02.10 INS END

	return rt;
}

//-----------------------------------------------------------------------------
// 誘導メッセージ取得
//-----------------------------------------------------------------------------
// 返送値	：	誘導メッセージ
//-----------------------------------------------------------------------------
CString& CSyzMakeSubW::GetGuideMsg()
{
	return m_GuideMsg;
}

//-----------------------------------------------------------------------------
// 平成26年改正 消費税コンバージョン
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	マスターに接続済の財務クラスのポインタ
//		CnvInfo		：	コンバージョンオプション
//		rtMstst		：	マスターの状態（返送用）□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □■■■
//						 D0：DBの内容に変更有
//						 D1：施行日（平成26年4月1日）以降に仕訳あり
//						 D2：施行日（平成26年4月1日）以降に仕訳を変換
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ConversionToZei8( CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst )
{
	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		m_ErrMsg = _T("不正な引数が指定されています(ConversionToZei8)");
		return -1;
	}
	rtMstst = 0;

	int	nSver = 0;
	if( GetZvolumeSver(pZmsub->m_database, nSver) ){
		return -1;
	}
/*- '13.10.17 -*/
//	if( nSver >= PER8_SYZ_VER ){
/*-------------*/
	// 同一バージョン時の再バージョンアップを可能に
	if( nSver > PER8_SYZ_VER ){
/*-------------*/
		// コンバージョンの必要なし！
		return 0;
	}

	// 施行日以降の仕訳あり？
	BOOL	IsChanged = FALSE;
	int st = ChkDataAfterDate( pZmsub->m_database, PER8_SYZ_YMD, true );
	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		// 仕訳あり
		rtMstst |= 0x02;

		if( CnvInfo.cnvSw&0x01 ){
			// ボリュームラベルオープン
			unsigned long	opnFlg = 0x00;
			if( pZmsub->zvol == NULL ){
				if( pZmsub->VolumeOpen() ){
/*- '14.06.25 -*/
//					m_ErrMsg = pZmsub->GetZmErrMsg();
//					return -1;
/*-------------*/
					if( (pZmsub->zvol!=NULL) && (pZmsub->zvol->z_fail==1) ){
						;
					}
					else{
						m_ErrMsg = pZmsub->GetZmErrMsg();
						return -1;
					}
/*-------------*/
				}
				opnFlg |= 0x01;
			}

			// 変換可能？
			if( pZmsub->zvol->tl_cor&0x10 ){

				// 科目名称ファイルオープン
				if( pZmsub->knrec == NULL ){
					if( pZmsub->KamokuMeisyoOpen() ){
						m_ErrMsg = pZmsub->GetZmErrMsg();
						return -1;
					}
					opnFlg |= 0x02;
				}
				// 科目名称読込み
				if( pZmsub->pKnrec == NULL ){
					if( pZmsub->KamokuMeisyoRead() ){
						m_ErrMsg = pZmsub->GetZmErrMsg();
						return -1;
					}
					opnFlg |= 0x04;
				}

				// 仕訳変換！
				if( CnvWrongShiwake(pZmsub, IsChanged, IC_ICSSYZ_5_TO_8_CNV) ){
					if( IsChanged ){
						rtMstst |= 0x01;
					}
					if( opnFlg&0x01 ){
						pZmsub->VolumeClose();
					}
					if( opnFlg&0x02 ){
						pZmsub->KamokuMeisyoClose();
					}
					if( opnFlg&0x04 ){
						pZmsub->KamokuMeisyoFree();
					}
					return -1;
				}
				if( IsChanged ){
					rtMstst |= 0x04;
				}
			}

			if( opnFlg&0x01 ){
				pZmsub->VolumeClose();
			}
			if( opnFlg&0x02 ){
				pZmsub->KamokuMeisyoClose();
			}
			if( opnFlg&0x04 ){
				pZmsub->KamokuMeisyoFree();
			}
		}
	}

//--> '14.04.19 INS START
	// 輸入仕入コンバージョン
	if( CnvImportShiwake(pZmsub) ){
		return -1;
	}
//<-- '14.04.19 INS END

	// 最終コンバージョン
	if( Conversion(nSver, pZmsub, IC_ICSSYZ_5_TO_8_CNV) ){
		return -1;
	}
	rtMstst |= 0x01;

	return 0;
}

//-----------------------------------------------------------------------------
// 不正仕訳のコンバージョン
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	マスターに接続済の財務クラスのポインタ
//		IsChanged	：	データに変更を加えたかどうか
//		cnvtype		：	変換タイプ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CnvWrongShiwake( CDBZmSub *pZmsub, BOOL &IsChanged, EnumIdDataCnvType cnvtype )
{
	CDBSyohi	dbSyohi;
	dbSyohi.SyOpen( pZmsub );

	int				st;
	unsigned long	dbtKcd, creKcd;
	DBKNREC			*pDbtKn, *pCreKn;
	CString			filter;

	if( cnvtype == IC_ICSSYZ_8_TO_10_CNV ){
		filter.Format(_T("ddate >= %d"), PER10_SYZ_YMD);
	}
	else{
		filter.Format(_T("ddate >= %d"), PER8_SYZ_YMD);
	}
	pZmsub->DBDataOpen( filter );
	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pZmsub->dbdata->MoveFirst()==-1) || (pZmsub->dbdata->st==-1) ){
				break;
			}
		}
		else{
			if( (pZmsub->dbdata->MoveNext()==-1) || (pZmsub->dbdata->st==-1) ){
				break;
			}
		}
#ifdef _20190331_SYZ_REV_
/*- '13.10.03 -*/
//		if( (pZmsub->dbdata->dsign[1]&0x0f) == 0x00 ){
/*-------------*/
		if( ((pZmsub->dbdata->dsign[1]&0x0f)==0x00) && ((pZmsub->dbdata->dsign[7]&0x10)==0x00) ){
/*-------------*/
#else
		if( ((pZmsub->dbdata->dsign[7]&0x10)==0x00) &&
			((((pZmsub->dbdata->dsign[1]&0x0f)==0x00)&&(cnvtype==IC_ICSSYZ_5_TO_8_CNV)) || (((pZmsub->dbdata->dsign[1]&0x0f)==0x03)&&(cnvtype==IC_ICSSYZ_8_TO_10_CNV))) ){
#endif
			// 借方
			AscToBcd( (char*)&dbtKcd, pZmsub->dbdata->dbt.GetBuffer(), pZmsub->dbdata->dbt.GetLength() );
			pDbtKn = pZmsub->PjisToKnrec( dbtKcd );
			// 貸方
			AscToBcd( (char*)&creKcd, pZmsub->dbdata->cre.GetBuffer(), pZmsub->dbdata->cre.GetLength() );
			pCreKn = pZmsub->PjisToKnrec( creKcd );

			if( pDbtKn && pCreKn ){
				// 課税対象でなくても税率を8%に変換するように！
				if( cnvtype == IC_ICSSYZ_5_TO_8_CNV ){
					if( ((pDbtKn->knsgn[3]&0xf0)==DEFAULT_SYZ_KNSGN) || ((pDbtKn->knsgn[3]&0xf0)==PER8_SYZ_KNSGN) ||
						((pCreKn->knsgn[3]&0xf0)==DEFAULT_SYZ_KNSGN) || ((pCreKn->knsgn[3]&0xf0)==PER8_SYZ_KNSGN) ){
						// 5% -> 8%
						st = dbSyohi.SyIniz(pZmsub->dbdata);
						if( st == 0 ){
							dbSyohi.SyFnc(CD_RITU8);
							pZmsub->dbdata->DataCorrect();
							IsChanged = TRUE;
						}
					}
				}
				else if( cnvtype == IC_ICSSYZ_8_TO_10_CNV ){
					if( ((pDbtKn->knsgn[0]&0xff)&&((pDbtKn->knsgn[3]&0xf0)==DEFAULT_SYZ_KNSGN)) || ((pDbtKn->knsgn[0]&0xff)&&((pDbtKn->knsgn[3]&0xf0)==PER10_SYZ_KNSGN)) ||
						((pCreKn->knsgn[0]&0xff)&&((pCreKn->knsgn[3]&0xf0)==DEFAULT_SYZ_KNSGN)) || ((pCreKn->knsgn[0]&0xff)&&((pCreKn->knsgn[3]&0xf0)==PER10_SYZ_KNSGN)) ){
						// 8% -> 10%
						st = dbSyohi.SyIniz(pZmsub->dbdata);
						if( st == 0 ){
							dbSyohi.SyFnc(CD_RITU10);
							pZmsub->dbdata->DataCorrect();
							IsChanged = TRUE;
						}
					}
					else if( ((pDbtKn->knsgn[3]&0xf0)==PER10_SYZ_KNSGN) || ((pCreKn->knsgn[3]&0xf0)==PER10_SYZ_KNSGN) ){
						// 8% -> 8%（軽減）
						st = dbSyohi.SyIniz(pZmsub->dbdata);
						if( st == 0 ){
							dbSyohi.SyFnc(CD_RD_RITU8);
							pZmsub->dbdata->DataCorrect();
							IsChanged = TRUE;
						}
					}
					else if( (pZmsub->dbdata->dbt==pZmsub->szvol->SVkcod1) || (pZmsub->dbdata->dbt==pZmsub->szvol->SVkcod2) ||
						(pZmsub->dbdata->cre==pZmsub->szvol->SVkcod1) || (pZmsub->dbdata->cre==pZmsub->szvol->SVkcod2) ){
						// 8% -> 10%
						if( pZmsub->dbdata->dsign[1] == 0x03 ){
							st = dbSyohi.SyIniz(pZmsub->dbdata);
							if( st == 0 ){
								dbSyohi.SyFnc(CD_RITU10);
								pZmsub->dbdata->DataCorrect();
								IsChanged = TRUE;
							}
						}
					}
				}
			}
		}
	}
	pZmsub->DBDataClose();

	dbSyohi.SyClose();

	return 0;
}

//-----------------------------------------------------------------------------
// 変換 ( 文字列→BCD )
//-----------------------------------------------------------------------------
// 引数	buff	：	変換先
//		data	：	変換元
//		len		：	変換元の文字列長
//-----------------------------------------------------------------------------
void CSyzMakeSubW::AscToBcd( char* buff, char* data, int len )
{
	CString	str;
	str.Format( _T("%s"), data );
	str.MakeUpper();

	// 文字数チェック
	int slen = str.GetLength();
	if( slen < len ){
		return;
	}

	int	bcdlen = len / 2;
	int	hlsw = 0;
	if( len % 2 ){
		hlsw = 1;
		bcdlen++;
	}

	BYTE	hc, lc;

	for( int i=0, j=0; i<bcdlen; i++ ){
		if( hlsw ){
			hc = 0;
			hlsw = 1;
		}
		else{
			hc = str.GetAt( j++ );
		}

		if( (hc>='A') && (hc<='Z') ){
			hc = hc - 'A' + 0x0a;
		}
		else{
			hc = hc - '0';
		}
		hc = ((hc&0x0f) << 4) & 0xf0;

		lc = str.GetAt( j++ );

		if( (lc>='A') && (lc<='Z') ){
			lc = lc - 'A' + 0x0a;
		}
		else{
			lc = lc - '0';
		}

		*(buff + i) = hc + lc;
	}
}

//-----------------------------------------------------------------------------
// 該当日以降の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB		：	チェック対象データベース
//		ymd			：	チェック対象日付
//		isChkDel	：	削除仕訳もチェック対象とするかどうか
//-----------------------------------------------------------------------------
// 返送値		1		：	仕訳あり
//			0		：	仕訳なし
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkDataAfterDate( class CDatabase *pChkDB, int ymd, bool isChkDel )
{
	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		m_ErrMsg = _T("不正な引数が指定されています(ChkDataAfterDate)");
		return -1;
	}
	CRecordset	tmpRs( pChkDB );
	CString	sql, tblCnt;
	if( isChkDel ){
		sql.Format(GET_DATA_COUNT_FROM_DATA_SQL, ymd);
	}
	else{
		sql.Format(GET_DATA_COUNT_FROM_DATA_REM_DEL_SQL, ymd);
	}
	try{
		tmpRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue( (SHORT)0, tblCnt );
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi( tblCnt );
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 免税事業者からの仕入仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象DB
//		stymd	：	開始期日
//		edymd	：	終了期日
//-----------------------------------------------------------------------------
// 返送値		1	：	仕訳あり
//			0	：	仕訳無し
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputStockFromMenzeiData(class CDatabase *pChkDB, int stymd, int edymd )
{
	ASSERT(pChkDB);
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs(pChkDB);
	CString	sql, tblCnt;
	sql.Format( GET_STOCK_FROM_MENZEI_FROM_DATA_SQL, stymd, edymd );
	try{
		tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue((SHORT)0, tblCnt);
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi(tblCnt);
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 該当税率の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		ztype	：	税率タイプ
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZeiDataSub(class CDatabase *pChkDB, EmumIdZeirituType ztype)
{
	ASSERT(pChkDB);
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs(pChkDB);
	CString	sql, tblCnt;
	if( ztype == ID_ICSSYZ_8PER_TYPE ){
		sql.Format(GET_ZEI8COUNT_FROM_DATA_SQL);
	}
	else if( ztype == ID_ICSSYZ_10PER_TYPE ){
		sql.Format(GET_ZEI10COUNT_FROM_DATA_SQL);
	}
	else if( ztype == ID_ICSSYZ_RD8PER_TYPE ){
		sql.Format(GET_ZEI8RDCOUNT_FROM_DATA_SQL);
	}
	try{
		tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue((SHORT)0, tblCnt);
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi(tblCnt);
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 8% の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZei8Data( class CDatabase *pChkDB )
{
	return ChkInputZeiDataSub(pChkDB, ID_ICSSYZ_8PER_TYPE);
}

//-----------------------------------------------------------------------------
// 10% の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZei10Data(class CDatabase *pChkDB)
{
	return ChkInputZeiDataSub(pChkDB, ID_ICSSYZ_10PER_TYPE);
}

//-----------------------------------------------------------------------------
// 8%（軽減） の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZei8RdData(class CDatabase *pChkDB)
{
	return ChkInputZeiDataSub(pChkDB, ID_ICSSYZ_RD8PER_TYPE);
}

//-----------------------------------------------------------------------------
// 該当税率の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		stymd	：	開始期日
//		edymd	：	終了期日
//		ztype	：	チェック対象税率
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZeiDataSub(class CDatabase *pChkDB, int stymd, int edymd, EmumIdZeirituType ztype)
{
	ASSERT(pChkDB);
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs(pChkDB);
	CString	sql, tblCnt;
	if( ztype == ID_ICSSYZ_8PER_TYPE ){
		sql.Format(GET_ZEI8COUNT_BY_DATE_FROM_DATA_SQL, stymd, edymd);
	}
	else if( ztype == ID_ICSSYZ_10PER_TYPE ){
		sql.Format(GET_ZEI10COUNT_BY_DATE_FROM_DATA_SQL, stymd, edymd);
	}
	else if( ztype == ID_ICSSYZ_RD8PER_TYPE ){
		sql.Format(GET_ZEI8RDCOUNT_BY_DATE_FROM_DATA_SQL, stymd, edymd);
	}

	try{
		tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue((SHORT)0, tblCnt);
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi(tblCnt);
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 8% の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		stymd	：	開始期日
//		edymd	：	終了期日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZei8Data( class CDatabase *pChkDB, int stymd, int edymd )
{
	return ChkInputZeiDataSub( pChkDB, stymd, edymd, ID_ICSSYZ_8PER_TYPE );
}

//-----------------------------------------------------------------------------
// 10% の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		stymd	：	開始期日
//		edymd	：	終了期日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZei10Data(class CDatabase *pChkDB, int stymd, int edymd)
{
	return ChkInputZeiDataSub(pChkDB, stymd, edymd, ID_ICSSYZ_10PER_TYPE);
}

//-----------------------------------------------------------------------------
// 8%（軽減） の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		stymd	：	開始期日
//		edymd	：	終了期日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZei8RdData(class CDatabase *pChkDB, int stymd, int edymd)
{
	return ChkInputZeiDataSub(pChkDB, stymd, edymd, ID_ICSSYZ_RD8PER_TYPE);
}

//-----------------------------------------------------------------------------
// 消費税バージョンの取得
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		nSver	：	バージョンの取得先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetZvolumeSver( class CDatabase *pChkDB, int &nSver )
{
	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		m_ErrMsg = _T("不正な引数が指定されています(GetZvolumeSver)");
		return -1;
	}

	CRecordset	tmpZmRs( pChkDB );
	CString	sql, strSver;
	sql.Format( GET_SVER_FROM_ZVOL_SQL );
	try{
		tmpZmRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpZmRs.IsEOF() ){
			tmpZmRs.GetFieldValue( (SHORT)0, strSver );
		}
		else {
			tmpZmRs.Close();
			m_ErrMsg = _T("マスター内に消費税に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpZmRs.Close();

	// 変換
	nSver = atoi( strSver );

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税バージョンと期末年月日の取得 ('13.12.14)
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		nSver	：	バージョンの取得先
//		nEymd	：	期末年月日の取得先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetZvolumeSverEymd( class CDatabase *pChkDB, int &nSver, int &nEymd )
{
	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		m_ErrMsg = _T("不正な引数が指定されています(GetZvolumeSver)");
		return -1;
	}

	CRecordset	tmpZmRs( pChkDB );
	CString	sql, strSver, strEmyd;
	sql.Format( GET_SVER_EYMD_FROM_ZVOL_SQL );
	try{
		tmpZmRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpZmRs.IsEOF() ){
			tmpZmRs.GetFieldValue( (SHORT)0, strSver );
			tmpZmRs.GetFieldValue( (SHORT)1, strEmyd );
		}
		else {
			tmpZmRs.Close();
			m_ErrMsg = _T("マスター内に消費税に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpZmRs.Close();

	// 変換
	nSver = atoi( strSver );
	nEymd = atoi( strEmyd );

	return 0;
}

//-----------------------------------------------------------------------------
// ５％を明記すべきマスター？
//-----------------------------------------------------------------------------
// 引数		pZmsub	：	マスターに接続済の財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	明記すべきマスター
//			FALSE	：	明記の必要が無いマスター
//-----------------------------------------------------------------------------
BOOL CSyzMakeSubW::IsNeed5PerNote( CDBZmSub *pZmsub )
{
	BOOL	bRt = FALSE;

	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		return bRt;
	}
	ASSERT( pZmsub->m_database );
	if( pZmsub->m_database == NULL ){
		return bRt;
	}

	// バージョンアップ済み？
	int	s_ver=0, ee_ymd=0;
	if( GetZvolumeSverEymd(pZmsub->m_database, s_ver, ee_ymd) ){
		return bRt;
	}
	if( s_ver < PER8_SYZ_VER ){
		return bRt;
	}

	// 条件２：期末日が平成26年4月1日以降？
	if( ee_ymd < PER8_SYZ_YMD ){
		return bRt;
	}

	// 最終
	bRt = TRUE;

	return bRt;
}

//-----------------------------------------------------------------------------
// 輸入仕訳のコンバージョン
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	マスターに接続済の財務クラス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CnvImportShiwake( CDBZmSub *pZmsub )
{
	ASSERT( pZmsub );

	// 原則課税：個別対応方式のみ
	BOOL	openFlg = FALSE;
	if( pZmsub->zvol == NULL ){
		if( pZmsub->VolumeOpen() ){
/*- '14.06.25 -*/
//			return -1;
/*-------------*/
			if( (pZmsub->zvol!=NULL) && (pZmsub->zvol->z_fail==1) ){
				// マスター再集計が必要なマスターに関しては、輸入仕入のコンバージョンは行わない
				return 0;
			}
			else{
				return -1;
			}
/*-------------*/
		}
		openFlg = TRUE;
	}

	// 原則課税：個別対応方式のみ
	if( (pZmsub->zvol->s_sgn2&0x0f) != 0x00 ){
		if( openFlg ){
			pZmsub->VolumeClose();
		}
		return 0;
	}
	else if( (pZmsub->zvol->s_sgn4&0x80) != 0x80 ){
		if( openFlg ){
			pZmsub->VolumeClose();
		}
		return 0;
	}
	// 基本仕入区分
	unsigned char	siire = 0x00;
	if( pZmsub->zvol->s_sgn6 == 1 ){
		siire = 0x10;
	}
	else if( pZmsub->zvol->s_sgn6 == 2 ){
		siire = 0x20;
	}
	else if( pZmsub->zvol->s_sgn6 == 3 ){
		siire = 0x30;
	}

	if( openFlg ){
		pZmsub->VolumeClose();
		openFlg = FALSE;
	}

	// 仮払消費税のコード取得
	ASSERT( pZmsub->m_database );
	CRecordset	tmpSzRs( pZmsub->m_database );
	CString		sql, strSvkcod1;
	sql.Format( _T("SELECT SVkcod1 from szvol") );
	try{
		tmpSzRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpSzRs.IsEOF() ){
			tmpSzRs.GetFieldValue( (SHORT)0, strSvkcod1 );
		}
		else{
			tmpSzRs.Close();
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpSzRs.Close();

	CString	strKariKcd  = strSvkcod1;

	pZmsub->m_database->BeginTrans();
	try{
		sql.Format( UPDATE_DATAREC_IMPORT_KARI_SQL, siire, strKariKcd, strKariKcd );
		pZmsub->m_database->ExecuteSQL( sql );

		sql.Format( UPDATE_MINFO_OFF_SYKSIGN_SQL );
		pZmsub->m_database->ExecuteSQL( sql );
	}
	catch( CDBException *pde ){
		pZmsub->m_database->Rollback();
		pde->Delete();
		return -1;
	}
	pZmsub->m_database->CommitTrans();

	return 0;
}

//-----------------------------------------------------------------------------
// 完成時振替仕訳のコンバージョン
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	マスターに接続済の財務クラス
//		IsChanged	：	変更が
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CnvKensetuFurikaeShiawke(CDBZmSub *pZmsub, bool &bIsChanged)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->zvol);
	bIsChanged = false;
	CDBSyohi	dbSyohi;

	if( IsKensetsuSpMaster(pZmsub) == 0 ){
		return 0;
	}
	if( pZmsub->zvol->ee_ymd < PER10_SYZ_YMD ){
		return 0;
	}

	CString	filter = GET_MISEIKOUJI_DATA_SQL;

	int	openFlg = 0;
	if( pZmsub->dbdata == NULL ){
		if( pZmsub->DBDataOpen(filter) ){
			m_ErrMsg = pZmsub->GetZmErrMsg();
			goto ERROR_OPEN;
		}
		openFlg |= 0x01;
	}
	else{
		if( pZmsub->dbdata->Requery(filter,0,0) ){
			m_ErrMsg = pZmsub->GetZmErrMsg();
			goto ERROR_OPEN;
		}
	}

	if( pZmsub->knrec == NULL ){
		if( pZmsub->KamokuMeisyoOpen() ){
			m_ErrMsg = pZmsub->GetZmErrMsg();
			goto ERROR_OPEN;
		}
		openFlg |= 0x02;
	}
	if( pZmsub->pKnrec == NULL ){
		if( pZmsub->KamokuMeisyoRead() ){
			m_ErrMsg = pZmsub->GetZmErrMsg();
			goto ERROR_OPEN;
		}
		openFlg |= 0x04;
	}

	if( dbSyohi.SyOpen(pZmsub) ){
		goto ERROR_OPEN;
	}

	int				st = 0;
	unsigned char	slSign = 0;
	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pZmsub->dbdata->MoveFirst()==-1) || (pZmsub->dbdata->st==-1) ){
				break;
			}
		}
		else{
			if( (pZmsub->dbdata->MoveNext()==-1) || (pZmsub->dbdata->st==-1) ){
				break;
			}
		}
		slSign = (pZmsub->dbdata->dsign[7]&0x0f);
		if( (slSign==0x03) || (slSign==0x04) || (slSign==0x05) ){
			st = dbSyohi.SyIniz(pZmsub->dbdata);
			if( st == 0 ){
				dbSyohi.SyFnc(CD_KANSEI_FURI);
				if( slSign == 0x03 ){
					dbSyohi.SyFnc(CD_KAZEI);
					dbSyohi.SyFnc(CD_RITU5);
				}
				else if( slSign == 0x04 ){
					dbSyohi.SyFnc(CD_HIKAZ);
				}
				else if( slSign == 0x05 ){
					dbSyohi.SyFnc(CD_FUKAZ);
				}
				pZmsub->dbdata->DataCorrect();
				bIsChanged = true;
			}
		}
	}
	dbSyohi.SyClose();

/*
	// 参考：SQLにて付箋サインを探すなら。
	// 付箋サイン３の仕訳
	select * from dbo.datarec where (cast(cast(dsign as binary(8)) as int) & 0x0f) = 0x03;
	// 付箋サイン４の仕訳
	select * from dbo.datarec where (cast(cast(dsign as binary(8)) as int) & 0x0f) = 0x04;
	// 付箋サイン５の仕訳
	select * from dbo.datarec where (cast(cast(dsign as binary(8)) as int) & 0x0f) = 0x05;
*/
	if( openFlg&0x01 ){
		pZmsub->DBDataClose();
	}
	if( openFlg&0x02 ){
		pZmsub->KamokuMeisyoClose();
	}
	if( openFlg&0x04 ){
		pZmsub->KamokuMeisyoFree();
	}

	return 0;

ERROR_OPEN:

	if( openFlg&0x01 ){
		pZmsub->DBDataClose();
	}
	if( openFlg&0x02 ){
		pZmsub->KamokuMeisyoClose();
	}
	if( openFlg&0x04 ){
		pZmsub->KamokuMeisyoFree();
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 建設：未成工事支出金タイプで消費税判定に付箋使用マスター？
//-----------------------------------------------------------------------------
// 返送値	1	：	対象マスター
//			0	：	対象マスターでない
//-----------------------------------------------------------------------------
int CSyzMakeSubW::IsKensetsuSpMaster(CDBZmSub *pZmsub)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->zvol);

	int	rt = 0;
	if( pZmsub->zvol->apno == KENSETU_APNO ){

		// テーブルの有無
		CString	tbname = _T("KJVOL");
		if( IsExistTbl(pZmsub->m_database, tbname) == false ){
			return 0;
		}

		CRecordset	rcset(pZmsub->m_database);
		int			mtype=0, useFusen=0;
		CString		sql, tmpStr;
		sql = GET_MISEIKOUJI_KJVOL_SQL;
		try{
			rcset.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
			if( !rcset.IsEOF() ){
				rcset.GetFieldValue((SHORT)0, tmpStr);
				mtype = atoi(tmpStr);
				rcset.GetFieldValue((SHORT)1, tmpStr);
				useFusen = atoi(tmpStr);
			}
			else{
				// テーブル内のレコード無し
			}
		}
		catch( CDBException *pDe ){
			m_ErrMsg = pDe->m_strError;
			pDe->Delete();
			return 0;
		}
		rcset.Close();

		if( mtype && useFusen ){
			rt = 1;
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// テーブルの存在確認
//-----------------------------------------------------------------------------
// 引数	pChkDB		：	チェック対象DB
//		tblname		：	対象テーブル名
//-----------------------------------------------------------------------------
// 返送値	true	：	存在する
//			false	：	存在しない
//-----------------------------------------------------------------------------
bool CSyzMakeSubW::IsExistTbl(class CDatabase *pChkDB, CString& tblname)
{
	bool bRt = false;

	CString	sql;
	sql.Format(_T("SELECT name FROM sys.objects WHERE object_id = object_id('dbo.%s')"), tblname);

	CRecordset	tmpRs(pChkDB);
	try{
		int st = tmpRs.Open(CRecordset::forwardOnly, sql);
		if( st && tmpRs.IsEOF() ){
			st = 0;
		}
		tmpRs.Close();

		if( st ){
			bRt = true;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 消費税バージョンとマスタータイプの取得 ('14.05.22)
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//		nSver	：	バージョンの取得先
//		nMtype	：	マスタータイプの取得先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetZvolumeSverMtype( class CDatabase *pChkDB, int &nSver, int &nMtype )
{
	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		m_ErrMsg = _T("不正な引数が指定されています(GetZvolumeSverMtype)");
		return -1;
	}

	CRecordset	tmpZmRs( pChkDB );
	CString	sql, strSver, strMtype;
	sql.Format( GET_SVER_MTYPE_FROM_ZVOL_SQL );
	try{
		tmpZmRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpZmRs.IsEOF() ){
			tmpZmRs.GetFieldValue( (SHORT)0, strSver );
			tmpZmRs.GetFieldValue( (SHORT)1, strMtype );
		}
		else {
			tmpZmRs.Close();
			m_ErrMsg = _T("マスター内に消費税に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpZmRs.Close();

	// 変換
	nSver = atoi( strSver );
	nMtype = atoi( strMtype );

	return 0;
}

//-----------------------------------------------------------------------------
// 簡易第六種 の仕訳あり？('15.02.11)
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZeiKani6Data( class CDatabase *pChkDB )
{
	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs( pChkDB );
	CString	sql, tblCnt;
	sql.Format( GET_KANI6COUNT_FROM_DATA_SQL );
	try{
		tmpRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue( (SHORT)0, tblCnt );
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi( tblCnt );
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 簡易第六種 の仕訳あり？('15.02.23)
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZeiKani6Data( class CDatabase *pChkDB, int stymd, int edymd )
{
	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs( pChkDB );
	CString	sql, tblCnt;
	sql.Format( GET_KANI6COUNT_BY_DATE_FROM_DATA_SQL, stymd, edymd );
	try{
		tmpRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue( (SHORT)0, tblCnt );
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi( tblCnt );
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 簡易課税マスター？('15.02.11)
//-----------------------------------------------------------------------------
// 引数		pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	簡易課税マスター
//			FALSE	：	簡易課税マスターでない
//-----------------------------------------------------------------------------
BOOL CSyzMakeSubW::IsKaniMaster( CDatabase *pChkDB )
{
	BOOL	bRt = FALSE;

	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		m_ErrMsg = _T("不正な引数が指定されています(GetZvolumeSver)");
		return -1;
	}

	CRecordset	tmpZmRs( pChkDB );
	CString	sql, strS_Sgn2;
	sql.Format( GET_SZINFO_FROM_ZVOL_SQL );
	try{
		tmpZmRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpZmRs.IsEOF() ){
			tmpZmRs.GetFieldValue( (SHORT)0, strS_Sgn2 );
		}
		else {
			tmpZmRs.Close();
			m_ErrMsg = _T("マスター内に消費税に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpZmRs.Close();

	// 変換
	int s_sgn2 = atoi( strS_Sgn2 );

	if( (s_sgn2&0x0f) > 0x01 ){
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 特定収入仕訳あり？('15.03.31)
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZeiSpcData( class CDatabase *pChkDB, int stymd, int edymd )
{
	ASSERT( pChkDB );
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs( pChkDB );
	CString	sql, tblCnt;
	sql.Format( GET_SPC8COUNT_BY_DATE_FROM_DATA_SQL, stymd, edymd );
	try{
		tmpRs.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue( (SHORT)0, tblCnt );
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi( tblCnt );
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 平成31年改正 消費税コンバージョン
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	マスターに接続済の財務クラスのポインタ
//		CnvInfo		：	コンバージョンオプション
//		rtMstst		：	マスターの状態（返送用）□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ ■■■■
//						 D0：DBの内容に変更有
//						 D1：施行日（2019年10月1日）以降に仕訳あり
//						 D2：施行日（2019年10月1日）以降に仕訳を変換
//						 D3：建設未成工事支出金の仕訳を変換
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ConversionToZei10(CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst)
{
	ASSERT(pZmsub);
	if( pZmsub == NULL ){
		m_ErrMsg = _T("不正な引数が指定されています(ConversionToZei10)");
		return -1;
	}
	rtMstst = 0;

	int	nSver = 0;
	if( GetZvolumeSver(pZmsub->m_database, nSver) ){
		return -1;
	}

	// 同一バージョン時の再バージョンアップを可能に
	if( nSver > PER10_SYZ_VER ){
		// コンバージョンの必要なし！
		return 0;
	}

	// 施行日以降の仕訳あり？
	BOOL	IsChanged = FALSE;
	int st = ChkDataAfterDate(pZmsub->m_database, PER10_SYZ_YMD, true);
	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		// 仕訳あり
		rtMstst |= 0x02;

		if( CnvInfo.cnvSw&0x01 ){
			// ボリュームラベルオープン
			unsigned long	opnFlg = 0x00;
			if( pZmsub->zvol == NULL ){
				if( pZmsub->VolumeOpen() ){
					if( (pZmsub->zvol!=NULL) && (pZmsub->zvol->z_fail==1) ){
						;
					}
					else{
						m_ErrMsg = pZmsub->GetZmErrMsg();
						return -1;
					}
				}
				opnFlg |= 0x01;
			}

			// 変換可能？
			if( pZmsub->zvol->tl_cor&0x10 ){

				// 科目名称ファイルオープン
				if( pZmsub->knrec == NULL ){
					if( pZmsub->KamokuMeisyoOpen() ){
						m_ErrMsg = pZmsub->GetZmErrMsg();
						return -1;
					}
					opnFlg |= 0x02;
				}
				// 科目名称読込み
				if( pZmsub->pKnrec == NULL ){
					if( pZmsub->KamokuMeisyoRead() ){
						m_ErrMsg = pZmsub->GetZmErrMsg();
						return -1;
					}
					opnFlg |= 0x04;
				}

				// 仕訳変換！
				if( CnvWrongShiwake(pZmsub, IsChanged, IC_ICSSYZ_8_TO_10_CNV) ){
					if( IsChanged ){
						rtMstst |= 0x01;
					}
					if( opnFlg&0x01 ){
						pZmsub->VolumeClose();
					}
					if( opnFlg&0x02 ){
						pZmsub->KamokuMeisyoClose();
					}
					if( opnFlg&0x04 ){
						pZmsub->KamokuMeisyoFree();
					}
					return -1;
				}
				if( IsChanged ){
					rtMstst |= 0x04;
				}
			}

			if( opnFlg&0x01 ){
				pZmsub->VolumeClose();
			}
			if( opnFlg&0x02 ){
				pZmsub->KamokuMeisyoClose();
			}
			if( opnFlg&0x04 ){
				pZmsub->KamokuMeisyoFree();
			}
		}
	}

	// 輸入仕入コンバージョン
	if( CnvImportShiwake(pZmsub) ){
		return -1;
	}

	// 完成時振替コンバージョン
	bool	bIsChanged = false;
	if( CnvKensetuFurikaeShiawke(pZmsub, bIsChanged) ){
		return -1;
	}
	if( bIsChanged ){
		rtMstst |= 0x08;
	}

	// 消費税集計テーブル（szsyk）のPK変換（この処理は、10%に上げる一回のみに）
	if( nSver < PER10_SYZ_VER ){
		if( CnvSzsykPK(pZmsub) ){
			return -1;
		}
	}

	// 最終コンバージョン
	if( Conversion(nSver, pZmsub, IC_ICSSYZ_8_TO_10_CNV) ){
		return -1;
	}
	rtMstst |= 0x01;

	return 0;
}

//-----------------------------------------------------------------------------
// 3%か5%の輸入仕入の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象のDB
//		stymd	：	チェック対象期間の開始日
//		edymd	：	チェック対象期間の最終日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInput3or5ImportData(class CDatabase *pChkDB, int stymd, int edymd)
{
	ASSERT(pChkDB);
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs(pChkDB);
	CString	sql, tblCnt;
	sql.Format(GET_IMPORT_BY_DATE_FROM_DATA_SQL, stymd, edymd);
	try{
		tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue((SHORT)0, tblCnt);
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi(tblCnt);
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
// 仮受・仮払消費税の仕訳あり？
//----------------------------------------------------------------------------
// 引数	pZmsub	：	チェック対象の財務クラス
//		stymd	：	チェック対象期間の開始日
//		edymd	：	チェック対象期間の最終日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputKariUBData(CDBZmSub *pZmsub, int stymd, int edymd)
{
	int rt = 0;

	// 仮受・仮払消費税
	CString	strKribaraiKcd, strKriukeKcd;
	if( GetKariUkeBaraiKcd(pZmsub, strKribaraiKcd, strKriukeKcd) ){
		return -1;
	}

	CString	filter;
	filter.Format(_T("((cast(cast(dsign as binary(1)) as int)&0x01)=0x00) AND ((dbt='%s') OR (cre='%s') OR (dbt='%s') OR (cre='%s')) AND (ddate>=%d) AND (ddate<=%d)"), strKribaraiKcd, strKribaraiKcd, strKriukeKcd, strKriukeKcd, stymd, edymd);

	bool openflg = false;
	if( pZmsub->data == NULL ){
		if( pZmsub->DataOpen(filter) ){
			return -1;
		}
		openflg = true;
	}
	else{
		if( pZmsub->data->Requery(filter, 0, 0) ){
			return -1;
		}
	}

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pZmsub->data->MoveFirst()) || (pZmsub->data->st==-1) ){
				break;
			}
		}
		else{
			if( (pZmsub->data->MoveNext()) || (pZmsub->data->st==-1) ){
				break;
			}
		}
		// 輸入仕入？
		if( (pZmsub->data->dsign[4]==0x04) || (pZmsub->data->dsign[4]==0x20) || (pZmsub->data->dsign[4]==0x21) || (pZmsub->data->dsign[4]==0x22) ){
			;
		}
		// 精算？
		else if( pZmsub->data->dsign[4]==0x1c ){
			;
		}
		else{
			rt = 1;
			break;
		}
	}

	if( openflg ){
		pZmsub->DataClose();
		openflg = false;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 別記の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	チェック対象の財務クラス
//		stymd	：	チェック対象期間の開始日
//		edymd	：	チェック対象期間の最終日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputSepParaData(CDBZmSub *pZmsub, int stymd, int edymd)
{
	int rt = 0;

//	CDBSyohi	dbSyohi;
//	dbSyohi.SyOpen(pZmsub);

	// 別記仕訳
	CString	filter;
	filter.Format( _T("((cast(SUBSTRING(dsign,2,1) as int)&0x30=0x20)) AND ((cast(cast(dsign as binary(1)) as int)&0x01)=0x00) AND (((cast(SUBSTRING(dsign,5,1) as int)&0xFF!=0x04)) AND ((cast(SUBSTRING(dsign,5,1) as int)&0xFF!=0x20)) AND ((cast(SUBSTRING(dsign,5,1) as int)&0xFF!=0x21)) AND ((cast(SUBSTRING(dsign,5,1) as int)&0xFF!=0x22)) AND ((cast(SUBSTRING(dsign,5,1) as int)&0xFF!=0x1c)))  AND (ddate>=%d) AND (ddate<=%d)"), stymd, edymd );

	bool openflg = false;
	if( pZmsub->data == NULL ){
		if( pZmsub->DataOpen(filter) ){
			return -1;
		}
		openflg = true;
	}
	else{
		if( pZmsub->data->Requery(filter, 0, 0) ){
			return -1;
		}
	}

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pZmsub->data->MoveFirst()) || (pZmsub->data->st==-1) ){
				break;
			}
		}
		else{
			if( (pZmsub->data->MoveNext()) || (pZmsub->data->st==-1) ){
				break;
			}
		}

		// 取消し仕訳・輸入仕入・精算はクエリ文で除外
		rt = 1;
		break;

		// 科目属性のチェックは必要！
//		if( pZmsub->data->dsign[4] == 0x00 ){
//			// 消費税モジュールにて仕訳属性の確認が必要
//			dbSyohi.SyIniz(pZmsub->data);
//			if( pZmsub->data->dsign[4] == 0x03 ){
//				rt = 1;
//				break;
//			}
//		}
	}

	if( openflg ){
		pZmsub->DataClose();
		openflg = false;
	}

//	dbSyohi.SyClose();

	return rt;
}

//-----------------------------------------------------------------------------
// 国税と地方税を分割した輸入仕入の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象のDB
//		stymd	：	チェック対象期間の開始日
//		edymd	：	チェック対象期間の最終日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputExtImportData( class CDatabase *pChkDB, int stymd, int edymd )
{
	ASSERT(pChkDB);
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs(pChkDB);
	CString	sql, tblCnt;
	sql.Format(GET_EXTIMPORT_BY_DATE_FROM_DATA_SQL, stymd, edymd);
	try{
		tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue((SHORT)0, tblCnt);
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi(tblCnt);
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 完成時振替の仕訳あり？
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象のDB
//		stymd	：	チェック対象期間の開始日
//		edymd	：	チェック対象期間の最終日
//-----------------------------------------------------------------------------
// 返送値	1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputKojiFurikaeData(class CDatabase *pChkDB, int stymd, int edymd)
{
	ASSERT(pChkDB);
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs(pChkDB);
	CString	sql, tblCnt;
	sql.Format(GET_KOJI_FURIKAE_BY_DATE_FROM_DATA_SQL, stymd, edymd);
	try{
		tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue((SHORT)0, tblCnt);
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi(tblCnt);
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税集計テーブル（szsyk）のPK変換
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	マスターに接続済の財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CnvSzsykPK(CDBZmSub *pZmsub)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->m_database);

	try{
		pZmsub->m_database->ExecuteSQL(ALTER_SZSYK_CONSTRAINT_SQL);
		pZmsub->m_database->ExecuteSQL(ALTER_SZSYK_PK_SQL);
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 平成31年改正 自動コンバージョンを行える？
//-----------------------------------------------------------------------------
// 返送値	true	：	行える
//			false	：	行えない
//-----------------------------------------------------------------------------
bool CSyzMakeSubW::IsAbleToAutoVup()
{
	time_t	now_t = time(NULL);
	tm	start ={ 0,0,0,1,9,(2019-1900) };
	time_t start_t = mktime(&start);
	double dif = difftime(start_t, now_t);
	if( dif < 0 ){
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// 免税事業者からの課税仕入 の仕訳あり？('23.03.29)
//-----------------------------------------------------------------------------
// 引数	pChkDB	：	チェック対象データベース
//-----------------------------------------------------------------------------
// 返送値		1	：	仕訳あり
//			0	：	仕訳なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInputZeiMzSrData(class CDatabase *pChkDB, int stymd, int edymd)
{
	ASSERT(pChkDB);
	if( pChkDB == NULL ){
		return -1;
	}

	CRecordset	tmpRs(pChkDB);
	CString	sql, tblCnt;
	sql.Format(GET_MENZEICOUNT_BY_DATE_FROM_DATA_SQL, stymd, edymd);
	try{
		tmpRs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if( !tmpRs.IsEOF() ){
			tmpRs.GetFieldValue((SHORT)0, tblCnt);
		}
		else {
			tmpRs.Close();
			m_ErrMsg = _T("マスター内に仕訳に関する情報がありません。\nお近くのサポートセンターに御連絡ください。");
			return -1;
		}
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpRs.Close();

	int	nTblCnt = atoi(tblCnt);
	if( nTblCnt > 0 ){
		return 1;
	}

	return 0;
}
