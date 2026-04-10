// BlkIns.cpp : 実装ファイル
//

#include "stdafx.h"

#ifdef _BLK_DATA_	// 06.27 /08

//#include "dbrecal8.h"

#include <io.h>
#include <share.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <afxtempl.h>

# ifdef _BLKINS_1	// 07.29 /08
#  ifndef _BLKINS_2	// 07.31 /08
#include <winnetwk.h>
#include <lm.h>
#  else
#include "Bkcp.h"
//extern int  AFX_EXT_API DBBulkCopy( CString disdb, CString disdbn, CString srcdb, CString srcdbn, CStringArray &tbnary );
#  endif
# endif

#include "CStRecordSet.h"


#  ifndef _DCNTL_TBL_	// 06.27 /08
#  define _DCNTL_TBL_
// 登録数テーブルオフセット
enum dcntl_ofs {
	ofs_dcntl = 0,
	ofs_darec,
	ofs_kzrec,
	ofs_ezrec,
	ofs_knrec,
	ofs_gkrec,
	ofs_image,
	ofs_tkrec,
	ofs_bmname,
	ofs_bmseq,
	ofs_bmkname,
	ofs_clrange,
	ofs_bmkzrec,
	ofs_bmezrec,
	ofs_bmprt,
	ofs_rtrec,
	ofs_hwcnbmn,
	ofs_hwbmseq,
	ofs_hwbyopt,
	ofs_hwbygkmk,
	ofs_hwcmkmk,
	ofs_kzysrec,
	ofs_ezysrec,
	ofs_bmkzysrec,
	ofs_bmezysrec,
	ofs_szsyk,
	ofs_strec,
	ofs_zkzrec1,
	ofs_zkzrec2,
	ofs_zkzrec3,
	ofs_zkzrec4,
	ofs_bmzkzrec1,
	ofs_bmzkzrec2,
	ofs_bmzkzrec3,
	ofs_bmzkzrec4,
	ofs_i_user,
	ofs_icno,
	ofs_namelog,
	ofs_datalog,
	ofs_owntb,
	ofs_bmprsel,
	ofs_kshctl,
	ofs_kshnam,
	ofs_kshrec,
	ofs_skzan,
	ofs_hyjrec,
	ofs_fl
};
#   endif

#ifdef CLOSE
// 高速化 08.06 '08
extern char	_sqlsvr[30], _apno[20], _cocode[20], _kikan[20];
#endif


CString CBlkIns::pcom_sqlsvr;
CString CBlkIns::pcom_apno;
CString CBlkIns::pcom_cocode;
CString CBlkIns::pcom_kikan;

//
// 登録数
//
long CBlkIns::RecCnt( int tblofs )
{
	CDCNTL	*dcntl = gdcntl( tblofs );

	if( dcntl )
		return dcntl->cnt;
	else
		return 0;
}

//
// データコントロールデータのゲット
//
CDCNTL* CBlkIns::gdcntl( int tblofs )
{
	int st;
	CDCNTL	*dcntl = m_Zmp->dcntl;
	CString filter;

	filter.Format( _T("seq = %u"), tblofs );
	st = dcntl->Requery( filter );
	if( st == -1 )
		return NULL;
	else
		return dcntl;
}


// CBlkIns

CBlkIns::CBlkIns()
: m_wpath(_T(""))
, m_Zmp(NULL)
{
	// バッファ初期化
	BufClr();
}

CBlkIns::~CBlkIns()
{
	// クローズ処理
	EndClose();
}


// CBlkIns メンバ関数

// バッファクリア
void CBlkIns::BufClr( void )
{
	// ワークディレクトリパス
	m_wpath.Empty();
#ifdef _BLKINS_1	// 07.29 /08
	// ワークファイル名
	m_wfname.Empty();
#endif
	// ワークＦＵＬＬパス
	m_wfpath.Empty();
	// 財務クラスポインタ
	m_Zmp = NULL;
	// データベースポインタ
	m_db = NULL;
#ifdef _BLKINS_1	// 07.29 /08
# ifndef _BLKINS_2	// 07.31 /08
	// 共有
	memset( m_uni_globalfolda, 0, sizeof( m_uni_globalfolda ) ); 
	m_share = FALSE;
# endif
# ifdef _BLKINS_2	// 08.04 /08
	// データベース側マシン名 etc.
	m_sqlsvr.Empty();
	m_machine.Empty();
	m_apno.Empty();
	m_cocode.Empty();
	m_kikan.Empty();
# endif

#endif
	// バルクインサート用ファイルハンドル
	m_fd = -1;
	//// フラッシュバッファ
	//m_wrtbf = NULL;
	//m_wbfpn = 0;
#ifdef _BLK_DATA2_	// 07.04 /08
	m_wrtpn = 0;;
#endif

	// seq カウンター
	m_seq = 0;

// 10.08 /08
// 内部伝票番号
	m_icno = -1;		// 内部伝票番号チェック用
	m_set_icno = -1;	// 内部伝票番号設定用


	// エラーメッセージ
	m_ErrMsg.Empty();

}

// Close
void CBlkIns::EndClose(void)
{
#ifdef _BLKINS_1	// 07.29 /08
# ifndef _BLKINS_2	// 07.31 /08
	// 共有削除
	if( m_share ) {
		NET_API_STATUS res = NetShareDel(
			NULL,						// 実行対象のリモートサーバー
			(LPWSTR)m_uni_globalfolda,	// 削除対象の共有
			0							// 予約済み
		);
		if( res != NERR_Success )
			m_ErrMsg = _T("ワークフォルダの共有解除に失敗しました。");

		m_share = FALSE;
	}
# endif
#endif
	if( m_fd != -1 ) {
		// バルクコピー用ファイルクローズ
		_close( m_fd );
		m_fd = -1;
	}
	if( m_wfpath.IsEmpty() == FALSE ) {
		// バルクコピー用ファイルク削除
		remove( m_wfpath );
		m_wfpath.Empty();
	}
	//// フラッシュバッファ
	//if( m_wrtbf ) {
	//	// バッファ削除
	//	delete m_wrtbf;
	//	m_wrtbf = NULL;
	//	m_wbfpn = 0;
	//}

	// バッファクリア
	BufClr();
}

int CBlkIns::Iniz(CDBZmSub* zm, LPCTSTR wpath )
{
	// ワークディレクトリパス
#ifdef _BLKINS_1	// 07.29 /08
	// Full Path Get
	char tmp[MAX_PATH];
	LPTSTR p;
	DWORD ret = GetFullPathName( 
				wpath,			// ファイル名
				sizeof( tmp ),	// パス名を格納するバッファのサイズ
				tmp,			// パス名を格納するバッファ
				&p				// パス内のファイル名のアドレス
	);
	if( !ret ) {
		m_ErrMsg.Format( _T("存在しないワークフォルダ\"%s\"が指定されました。"), wpath );
		return -1;
	}
	m_wpath = tmp;
#else
	m_wpath = wpath;
#endif

	// 財務クラスポインタ
	m_Zmp = zm;

	// データベースポインタ
	m_db = zm->m_database;

#ifdef _BLKINS_1	// 08.04 /08
	// データカウントﾁｪｯｸ
	// 仕訳カウント
//	m_Zmp->dbdata->Requery( _T(""), 0, 0 );
//	DWORD seq1 = m_Zmp->dbdata->GetRecordCount();

	// 最終仕訳の番号
	DWORD seq1;
	CStRecordSet st_rec( m_Zmp->m_database );
	st_rec.m_sql = _T( "select top(1) seq as st from datarec order by seq DESC" );

	TRY {
		if( ! st_rec.Open() ) {
			m_ErrMsg = _T( "最終仕訳の番号が調べられません！" );
			return -1;
		}

		while( ! st_rec.IsEOF() ) {
			st_rec.MoveNext();
		}

		if( st_rec.GetRecordCount() > 0 ) {
			seq1	= st_rec.m_st;
		}
		else {
			seq1	= 0;
		}

		st_rec.Close();

	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( m_ErrMsg.GetBuffer(_MAX_PATH), _MAX_PATH );
		m_ErrMsg.ReleaseBuffer();
		return -1;	
	}
	CATCH( CDBException, de ) {
		m_ErrMsg = de->m_strError;
		return -1;
	}
	END_CATCH

	// コントロール仕訳数
	CDCNTL *dcntl = gdcntl( ofs_darec );
	if( !dcntl ) {
		m_ErrMsg = _T("データコントロールの取得に失敗しました。");
		return -1;
	}
	DWORD seq2 = dcntl->cnt;	// 最終seq番号

	if( seq1 > 0 ) {
		if( seq1 != seq2 ) {
			dcntl->Edit();
			dcntl->cnt = seq1;		// 仕訳カウント数
			dcntl->Update();
		}
	}

# ifdef _BLKINS_2	// 08.04 /08
#  ifdef CLOSE
	// データベース側マシン名 etc.
	char	sqlsvr[30],apno[20],cocode[20],kikan[20];
#  endif

# ifdef CLOSE
	if( PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan ) != 4 )	{
		m_ErrMsg = _T("DB 情報[SELECTCODSN]を取得できないので 処理を中止します。" );
		return -1;
	}
	m_sqlsvr = sqlsvr;
	
	int i = m_sqlsvr.Find( '\\' );
	if( i == -1 ) {
		m_ErrMsg = _T("転送先が見つかりません。");
		return -1;
	}
	m_machine = m_sqlsvr.Left( i );	// 転送先マシン名
	m_apno.Format( _T("%02u"), atoi(apno));
	m_cocode.Format( _T("%08u"), atoi( cocode ));
	m_kikan.Format( _T("%04u"), (atoi(kikan)%1000000)/100 );

# endif

	m_sqlsvr = pcom_sqlsvr;
	
	int i = m_sqlsvr.Find( '\\' );
	if( i == -1 ) {
		m_ErrMsg = _T("転送先が見つかりません。");
		return -1;
	}
	m_machine = m_sqlsvr.Left( i );	// 転送先マシン名

	// 09.30 '08 業種のデータが以降しない
	// m_apno.Format( _T("%02u"), atoi(_apno));

	m_apno.Format( _T("%02x"), atoi(pcom_apno) );

	m_cocode.Format( _T("%08u"), atoi( pcom_cocode ));
	m_kikan.Format( _T("%04u"), (atoi(pcom_kikan)%1000000)/100 );


	// CRC計算初期化	08.05 /08
	if( m_CRC.BuildCRCTable() ) {
		m_ErrMsg = "CRC テーブル:作成エラー";
		return -1;
	}


# endif
#endif

	return 0;
}

// バルクインサート用データ1仕訳分作成
int CBlkIns::PutBlkData( CDBDATA* drec )
{

	CString line;

	// バルクインサート用ファイルオープン
	// イニシャライズ
	if( m_fd == -1 ) {
		// ファイルオープン
#ifdef _BLKINS_1	// 07.29 /08
//		int x = GetCurrentProcessId();			// プログラム ＩＤ
		int x = m_Zmp->DBUserNo();				// データベースユーザ番号
//		m_wfname.Format( _T("db%06u%s" ), x, _T("datarec.blk") );
# ifndef CLOSE	// 08.05 /08
		CString str;
		// マシン名、AP番号、会社コード、期間 で、CRC値を算出
		str.Format( _T("%s%s%s%s"),m_machine, m_apno, m_cocode, m_kikan );
		DWORD crc = m_CRC.Calculate( str, str.GetLength() );
		// ファイル名 = "TmpDBユーザー番号CRC値datarec.blk"
		m_wfname.Format( _T("Tmp%06u%08x%s" ), x, crc, _T("datarec.blk") );
# else
		m_wfname.Format( _T("db%06u%s%s%s%s" ), x, m_apno, m_cocode, m_kikan, _T("datarec.blk") );
# endif
		m_wfpath.Format( _T("%s\\%s"), m_wpath, m_wfname );
#else
		m_wfpath.Format( _T("%s\\%s"), m_wpath, _T("datarec.blk") );
#endif
		errno_t err = _sopen_s( &m_fd, m_wfpath, (_O_WRONLY|_O_CREAT|_O_TRUNC), _SH_DENYNO, (_S_IREAD|_S_IWRITE) );
		if( m_fd == -1 ) {
			m_ErrMsg.Format( _T("ワークファイル\"%s\"のオープンに失敗しました。"), m_wfpath );
			return -1;
		}
		//// フラッシュバッファ削除
		//if( m_wrtbf )
		//	delete m_wrtbf;
		//m_wrtbf = new BYTE[BLK_BUF_SIZE];
		//m_wbfpn = 0;

		// 現在ＳＥＱリード
		m_seq = RecCnt( ofs_darec );
	}

	// 1行作成
	drec->seq = ++m_seq;
//	TRACE( "seq =%5d\n", m_seq); 

	line = MkBlkData( drec );

	// 1行ファイルＷｒｉｔｅ
	//DWORD lng = line.GetLength();
	//if( lng > ( BLK_BUF_SIZE - m_wbfpn ) ) {

	//	_write( m_fd, m_wrtbf, m_wbfpn );

	//	m_wbfpn = 0;
	//}
	//memmove( m_wrtbf + m_wbfpn, (LPCTSTR)line, lng );
	//m_wbfpn += lng;

	_write( m_fd, line, line.GetLength() );

	return 0;
}

#ifdef _BLK_DATA2_	// 07.04 /08
int CBlkIns::BlkInsData( void )
{
	if( m_wrtpn ) {
		 GENERIC_WRITE 
	}
# ifdef _BLK_DATA2_	// 07.04 /08
	long m_wrtpn;
#define DATA_WRITE_CNT	5000
# endif

	return 0;
}
#endif

// 仕訳バルクインサート
int CBlkIns::WrtBlkData( void )
{
	if( m_fd != -1 ) {
		//// 残りバッファＷｒｉｔｅ
		//if( m_wbfpn ) {

		//	_write( m_fd, m_wrtbf, m_wbfpn );

		//	delete m_wrtbf;
		//	m_wrtbf = NULL;
		//	m_wbfpn = 0;
		//}
		// バルクコピー用ファイルクローズ
		_close( m_fd );
		m_fd = -1;

#ifdef _BLKINS_1	// 07.29 /08
		int i;
		int ret;
		DWORD lng;
		CString str;
		char tmp[MAX_PATH];
		CString path;

		CString db_machine,local_machine;

		// データベース側マシン名
		//char	sqlsvr[30],apno[20],cocode[20],kikan[20];
		//if( PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan ) != 4 )	{
		//	m_ErrMsg = _T("DB 情報[SELECTCODSN]を取得できないので 処理を中止します。" );
		//	return -1;
		//}
		//str = sqlsvr;
		//i = str.Find( '\\' );
		//if( i == -1 ) {
		//	m_ErrMsg = _T("転送先が見つかりません。");
		//	return -1;
		//}
		//db_machine.Format( _T("%s" ), str.Left( i ) );	// 転送先マシン名
		db_machine = m_machine;

		// ローカルマシン名
		lng = sizeof(tmp);
		ret = GetComputerName( tmp, &lng );
		if( !ret ) {
			m_ErrMsg = _T("転送元が見つかりません。");
			return -1;
		}
		local_machine.Format( _T("%s"), tmp);

# ifdef _BLKINS_2	// 07.31 /08
		// マシン名ﾁｪｯｸ
		if( db_machine != local_machine ) {
			//
			// データベースと処理マシンが違っていた場合
			//
			
			// 仮データをSYSCTLに作成しそれをターゲットにコピーする。
			//

			CString sql;
			CString str;

			// ＢＬＫ コピー クラス
			CBkcp targ_bk;				// ターゲットＤＢ Z??_C????????_Y????
			CBkcp tmp_bk;				// 仮ＤＢ

			// サーバー名
//			CString targ_svr = sqlsvr;	// ターゲット "XXX\ICS" 
			CString targ_svr = m_sqlsvr;	// ターゲット "XXX\ICS" 
			CString tmp_svr;			// 仮 "XXX\ICS"

			// ＤＢテーブル名
			CString targ_name = _T("datarec");	// ターゲットテーブル名
			CString tmp_name = m_wfname.Left( m_wfname.Find( '.' ));		// 仮テーブル名

			////////////////////////////////////////////////////////////////////
			// ターゲットＤＢをオープン
			////////////////////////////////////////////////////////////////////
			CString targetdbname;			// ターゲットデータベース名
//			targetdbname.Format( _T("Z%02u_C%08u_Y%04u"), atoi(apno), atoi(cocode), (atoi(kikan)%1000000)/100 );
			targetdbname.Format( _T("Z%s_C%s_Y%s"), m_apno, m_cocode, m_kikan );
			ret = targ_bk.SetDatabase( targ_svr, targetdbname ); 
			switch( ret ) {
				case -1: m_ErrMsg = _T("データベースの接続に失敗しました。");
					return -1;
				case -2: m_ErrMsg = _T("データベースが見つかりませんでした。");
					return -1;
				default:	// OK
					break;
			}
			////////////////////////////////////////////////////////////////////
			// 仮用ＤＢをオープン "SYSCTL"
			////////////////////////////////////////////////////////////////////
			CString tempdbname = _T("SYSCTL");			// 仮データベース名
			i = targ_svr.Find( _T('\\') );
			if( i < 0 ) {
				m_ErrMsg.Format( _T("ＩＣＳデータベース\"%s\"が正しくありません。"), targ_svr);
				targ_bk.CloseDatabase();
				return -1;
			}
			tmp_svr.Format( _T("%s\\%s"), local_machine, targ_svr.Mid(i+1) );
			ret = tmp_bk.SetDatabase( tmp_svr, tempdbname );
			switch( ret ) {
				case -1: m_ErrMsg = _T("仮データベースの接続に失敗しました。");
					targ_bk.CloseDatabase();
					return -1;
				case -2: m_ErrMsg = _T("仮データベースが見つかりませんでした。");
					targ_bk.CloseDatabase();
				return -1;
				default:	// OK
					break;
			}

			// 仮テーブル削除
			CString deltbl;
			deltbl.Format( _T("IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[%s]') AND type in (N'U')) DROP TABLE [dbo].[%s]"), tmp_name, tmp_name );
			TRY
			{
				ret = tmp_bk.ExecuteSQL( deltbl );
			}
			CATCH( CFileException, e )
			{
				m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
				targ_bk.CloseDatabase();
				tmp_bk.CloseDatabase();
				return -1;
			}
			END_CATCH
			if( ret ) {
				m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
				targ_bk.CloseDatabase();
				tmp_bk.CloseDatabase();
				return -1;
			}

			// ターゲットテーブルのスクリプトをGetして仮テーブル用ディスクリプタを作成する。
			CString tmpscrpt;
			ret = targ_bk.GetScript( targ_name, tmpscrpt );
			if( ret ) {
				m_ErrMsg = _T("仕訳ファイルの記述の取得ができませんでした。");
				targ_bk.CloseDatabase();
				tmp_bk.CloseDatabase();
				return -1;
			}
			// 仮テーブル作成
			tmpscrpt.Replace( targ_name, tmp_name );	// "datarec" -> "Tmp～datarec"
			TRY
			{
				ret = tmp_bk.ExecuteSQL( tmpscrpt );
			}
			CATCH( CFileException, e )
			{
				m_ErrMsg = _T("仮仕訳ファイルの作成ができませんでした。");
				targ_bk.CloseDatabase();
				tmp_bk.CloseDatabase();
				return -1;
			}
			END_CATCH
			if( ret ) {
				m_ErrMsg = _T("仮仕訳ファイルの作成ができませんでした。");
				targ_bk.CloseDatabase();
				tmp_bk.CloseDatabase();
				return -1;
			}
			// 仮テーブルにデータ作成
			sql.Format( _T("BULK INSERT [dbo].[%s] FROM '%s' WITH ( FIELDTERMINATOR = '\t',	ROWTERMINATOR = '\n', 	KEEPNULLS )"), tmp_name, m_wfpath );
			TRY
			{
				//sys_db->ExecuteSQL( sql );
				ret = tmp_bk.ExecuteSQL( sql );
			}
			CATCH( CFileException, e )
			{
				m_ErrMsg = _T("仮仕訳の書き込みに失敗しました。");

				// 仮テーブル削除
				TRY
				{
					ret = tmp_bk.ExecuteSQL( deltbl );
				}
				CATCH( CFileException, e )
				{
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				END_CATCH
				if( ret ) {
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				tmp_bk.CloseDatabase();

				targ_bk.CloseDatabase();
				return -1;
			}
			END_CATCH
			if( ret ) {
				m_ErrMsg = _T("仮仕訳の書き込みに失敗しました。");

				// 仮テーブル削除
				TRY
				{
					ret = tmp_bk.ExecuteSQL( deltbl );
				}
				CATCH( CFileException, e )
				{
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				END_CATCH
				if( ret ) {
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				tmp_bk.CloseDatabase();

				targ_bk.CloseDatabase();
				return -1;
			}

			////////////////////////////////////////////////////////////////////
			// 仮の仕訳テーブルをターゲットの仕訳テーブルにコピー
			////////////////////////////////////////////////////////////////////
			TRY
			{
				ret = targ_bk.BulkCopy(	targ_name,		// 転送先テーブル名
										&tmp_bk,		// 転送元データベース
										tmp_name,		// 転送元テーブル名
										_T("")			// 抽出条件
										);
			}
			CATCH( CFileException, e )
			{
				m_ErrMsg = _T("仕訳のコピーに失敗しました。");

				// 仮テーブル削除
				TRY
				{
					ret = tmp_bk.ExecuteSQL( deltbl );
				}
				CATCH( CFileException, e )
				{
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				END_CATCH
				if( ret ) {
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				tmp_bk.CloseDatabase();

				targ_bk.CloseDatabase();
				return -1;
			}
			END_CATCH

			if( ret ) {
				m_ErrMsg = _T("仕訳のコピーに失敗しました。");

				// 仮テーブル削除
				TRY
				{
					ret = tmp_bk.ExecuteSQL( deltbl );
				}
				CATCH( CFileException, e )
				{
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				END_CATCH
				if( ret ) {
					m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
					targ_bk.CloseDatabase();
					tmp_bk.CloseDatabase();
					return -1;
				}
				tmp_bk.CloseDatabase();

				targ_bk.CloseDatabase();
				return -1;
			}

			////////////////////////////////////////////////////////////////////
			// 仮の仕訳テーブルを削除
			////////////////////////////////////////////////////////////////////
			// 仮テーブル削除
			TRY
			{
				ret = tmp_bk.ExecuteSQL( deltbl );
			}
			CATCH( CFileException, e )
			{
				m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
				targ_bk.CloseDatabase();
				tmp_bk.CloseDatabase();
				return -1;
			}
			END_CATCH
			if( ret ) {
				m_ErrMsg = _T("仮仕訳ファイルの削除に失敗しました。");
				targ_bk.CloseDatabase();
				tmp_bk.CloseDatabase();
				return -1;
			}

			// データベースクローズ
			targ_bk.CloseDatabase();
			tmp_bk.CloseDatabase();


			//////////////////////////////////////////////////////////////////////
			//// ローカルマシンのデータベース ICS\SYSCTL に仮の仕訳テーブルを作成
			//////////////////////////////////////////////////////////////////////
			//// システムデータベースを仮ＤＢとして使用
			//char SysServer[MAX_PATH+1] = {0};
			//if( GCOM_GetString( "SystemDataBase", SysServer ) != 1 ) {
			//	m_ErrMsg = _T("DB 情報[SystemDataBase]を取得できないので 処理を中止します。");
			//	return -1;
			//}
			//

			//CDatabase	*sys_db;	// SYSCTL データベース
			//
			//// システムデータベースセット
			//char SysServer[MAX_PATH+1] = {0};
			//if( GCOM_GetString( "SystemDataBase", SysServer ) != 1 ) {
			//	m_ErrMsg = _T("DB 情報[SystemDataBase]を取得できないので 処理を中止します。");
			//	return -1;
			//}
			//else if( m_Zmp->SysCtlDB( SysServer /*, int ntype=0, char *id=0, char *pwd=0*/ ) ) {
			//	ICSExit();
			//	return -2;
			//}
			//sys_db = m_Zmp->m_sysctl;	// SYSCTL データベース


			////////////////////////////////////////////////
			//// SYSCTLに仮のデータテーブル作成
			////////////////////////////////////////////////

			//CString target = _T("datarec");	// 目的テーブル名
			//
			//// 仮仕訳仕訳バルクインサート
			//CString sql;

			//sql.Format( _T("DELETE %s"), target );
			//TRY
			//{
			//	sys_db->ExecuteSQL( sql );
			//}
			//CATCH( CFileException, e )
			//{
			//	m_ErrMsg = _T("仮仕訳の削除に失敗しました。");
			//	return -1;
			//}
			//END_CATCH

			//sql.Format( _T("BULK INSERT %s FROM '%s' WITH ( FIELDTERMINATOR = '\t',	ROWTERMINATOR = '\n', 	KEEPNULLS )"), target, m_wfpath );
			//TRY
			//{
			//	sys_db->ExecuteSQL( sql );
			//}
			//CATCH( CFileException, e )
			//{
			//	m_ErrMsg = _T("仮仕訳の書き込みに失敗しました。");
			//	return -1;
			//}
			//END_CATCH

			//////////////////////////////////////////////////////////////////////
			//// 仮の仕訳テーブルをターゲットの仕訳テーブルにコピー
			//////////////////////////////////////////////////////////////////////
			//
			//CString targetdbname;				// ターゲットデータベース名

			//targetdbname.Format( _T("Z%02u_C%08u_Y%04u"), atoi(apno), atoi(cocode), (atoi(kikan)%1000000)/100 );
			//CString soudbname = _T("SYSCTL");	// 転送元データベース名		

			///*
			//	extern int  AFX_EXT_API DBBulkCopy( CString disdb, CString disdbn,
			//	CString srcdb , CString srcdbn, CStringArray &tbnary );

			//	disdb  転送先接続名 (例)  ATLAS-WSV\ICS
			//	disdbn 転送先データベース名　(例） Z00_XXXXXXXX

			//	srcdb  転送元接続名 (例)  ATLAS-WSV\ICS
			//	disdbn 転送元データベース名　(例） Z00_XXXXXXXX

			//	tbnary バルクコピーを行うテーブルリスト
			//*/
			//CString targetinstance = sqlsvr;
			//CString souinstance;
			//i = targetinstance.Find( _T('\\') );
			//if( i < 0 ) {
			//	m_ErrMsg.Format( _T("ＩＣＳデータベース\"%s\"が正しくありません。"), targetinstance);
			//	return -1;
			//}
			//souinstance.Format( _T("%s\\%s"), local_machine, targetinstance.Mid(i+1) );
			//CStringArray ary;
			//ary.RemoveAll();
			//ary.Add( target );
			//TRY
			//{
			//	ret = DBBulkCopy( targetinstance, targetdbname, souinstance, soudbname, ary );
			//}
			//CATCH( CFileException, e )
			//{
			//	m_ErrMsg = _T("仕訳の書き込みに失敗しました。");
			//	return -1;
			//}
			//END_CATCH

			//if( ret ) {
			//	m_ErrMsg.Format( _T("データ書き込み中にエラー(%d)が発生しました。"), ret );
			//	return -1;
			//}

		}
		else {
			//
			// データベースと処理マシンが同一の場合
			//

			path = m_wfpath;

			// 仕訳バルクインサート
			CString sql;
			sql.Format( _T("BULK INSERT datarec FROM '%s' WITH ( FIELDTERMINATOR = '\t',	ROWTERMINATOR = '\n', 	KEEPNULLS )"), path );

			TRY
			{
				m_db->ExecuteSQL( sql );
			}
			CATCH( CFileException, e )
			{
				m_ErrMsg = _T("仕訳の書き込みに失敗しました。");
				return -1;
			}
			END_CATCH

		}
# else
		// マシン名ﾁｪｯｸ
		if( db_machine != local_machine ) {
			//
			// データベースと処理マシンが違っていた場合
			//

			// 該当フォルダのネットワーク共有化
			//
			CString str_machine;
			CString str_globalfolda;
			char uni_globalfolda[MAX_PATH];
			char uni_foldapath[MAX_PATH];

			// ローカルマシン名
			str_machine = local_machine;
			
			// 共有名 （ワークディレクトリパスの最終フォルダ名）
			i = m_wpath.ReverseFind( '\\' );
			if( i >= 0 )
				str_globalfolda = m_wpath.Mid( i + 1 );
			else
				str_globalfolda = m_wpath;

			/* UNICODE 化 */
			ret = MultiByteToWideChar(	// to UNICODE
				CP_ACP,						// コードページ
				MB_PRECOMPOSED,				// 文字の種類を指定するフラグ
				str_globalfolda,			// マップ元文字列のアドレス
				-1,							// マップ元文字列のバイト数
				(LPWSTR)uni_globalfolda,	// マップ先ワイド文字列を入れるバッファのアドレス
				sizeof( uni_globalfolda )	// バッファのサイズ
			);
			// ワークディレクトリパス
			/* UNICODE 化 */
			ret = MultiByteToWideChar(	// to UNICODE
				CP_ACP,						// コードページ
				MB_PRECOMPOSED,				// 文字の種類を指定するフラグ
				m_wpath,					// マップ元文字列のアドレス
				-1,							// マップ元文字列のバイト数
				(LPWSTR)uni_foldapath,		// マップ先ワイド文字列を入れるバッファのアドレス
				sizeof( uni_foldapath )		// バッファのサイズ
			);
			// 共有名へ
			SHARE_INFO_2 share;
			share.shi2_netname      = (LPWSTR)uni_globalfolda;			// 共有名 
			share.shi2_type         = STYPE_DISKTREE;					// ディスクドライブ
			share.shi2_remark       = L"DB仕訳 BULK INSERT で使用中";	// コメント
			share.shi2_permissions  = 0;   
			share.shi2_max_uses     = SHI_USES_UNLIMITED;				 // 無制限
			share.shi2_current_uses = 0;
			share.shi2_path         = (LPWSTR)uni_foldapath;			// フォルダのパス
			share.shi2_passwd       = NULL;								// パスワードなし
		
			DWORD dwErr = 0;
			NET_API_STATUS res = NetShareAdd(NULL, 2, (LPBYTE)&share, &dwErr);
			if( res != NERR_Success ) {
				m_ErrMsg = _T("ワークフォルダの共有に失敗しました。");
				return -1;
			}
			memmove( m_uni_globalfolda, uni_globalfolda, sizeof( m_uni_globalfolda ) ); 
			m_share = TRUE;

			path.Format( _T("%s\\%s\\%s"), str_machine, str_globalfolda, m_wfname );
		}
		else {
			//
			// データベースと処理マシンが同一の場合
			//

			path = m_wfpath;
		}

		// 仕訳バルクインサート
		CString sql;
		sql.Format( _T("BULK INSERT datarec FROM '%s' WITH ( FIELDTERMINATOR = '\t',	ROWTERMINATOR = '\n', 	KEEPNULLS )"), path );

		TRY
		{
			m_db->ExecuteSQL( sql );
		}
		CATCH( CFileException, e )
		{
			m_ErrMsg = _T("仕訳の書き込みに失敗しました。");
			return -1;
		}
		END_CATCH
# endif
#else
		// 仕訳バルクインサート
		CString sql;
		sql.Format( _T("BULK INSERT datarec FROM '%s' WITH ( FIELDTERMINATOR = '\t',	ROWTERMINATOR = '\n', 	KEEPNULLS )"), m_wfpath );

		m_db->ExecuteSQL( sql );
#endif


		// 仕訳数セット
		CDCNTL *dcntl = gdcntl( ofs_darec );
		if( !dcntl ) {
			m_ErrMsg = _T("データコントロールの取得に失敗しました。");
			return -1;
		}
		dcntl->Edit();
		dcntl->cnt = m_seq;	// 最終seq番号
		dcntl->Update();
	}

	return 0;
}

// 仕訳データバルクコピー用1行作成
CString CBlkIns::MkBlkData( CDBDATA* drec )
{
	int i,j,max;
	CString sep = "\t";
	CString str,line;

	str.Empty();
	line.Empty();

	// ＳＥＱ番号	NOT NULL
	str.Format( "%d", drec->seq ); line += (str + sep);

	// 決算サイン	NOT NULL
	str.Format( "%d", drec->ksign); line += (str + sep);

	// 日付			NOT NULL
	str.Format( "%08d", drec->ddate ); line += (str + sep);

	// 月オフセット	NOT NULL
	str.Format( "%d", drec->mofs ); line += (str + sep);

	// 伝票番号
	if( drec->cno == -1 )
		str.Empty();
	else
		str.Format( "%d", drec->cno );
	line += (str + sep);

	// 借方 部門
	if( drec->dbmn == -1 )
		str.Empty();
	else
		str.Format( "%d", drec->dbmn );
	line += (str + sep );

	// 借方 工事番号
	str = drec->dkno; line += (str + sep);

	// 借方 科目
	str = drec->dbt; line += (str + sep);

	// 借方 枝番 NOT NULL
	if( drec->dbr == -1 )
		str.Empty();
	else
		str.Format( "%d", drec->dbr );
	line += (str + sep);

	//
	// 貸方 部門
	if( drec->cbmn == -1 )
		str.Empty();
	else
		str.Format( "%d", drec->cbmn );
	line += (str + sep );

	// 貸方 工事番号
	str = drec->ckno; line += (str + sep);

	// 貸方 科目
	str = drec->cre; line += (str + sep);

	// 貸方 枝番 NOT NULL
	if( drec->cbr == -1 )
		str.Empty();
	else
		str.Format( "%d", drec->cbr );
	line += (str + sep);

	// 金額	NOT NULL
	str = drec->val; line += (str + sep);

	// 税額
	str = drec->zei; line += (str + sep);

	// データサイン	NOT NULL
	for( i = 0, j = (int)drec->dsign.GetSize(), max = 16 ; i < max ; ++i ) {
		if( i < j )
			str.Format( "%02x", drec->dsign[i] );
		else
			str = "00";
		line += str;
	}
	line += sep;

	// オーナーフラグ //////////////////////////
	str.Format( "%d", m_Zmp->DBUserNo() ); line += (str + sep);	// データベースユーザ番号

	// 伝票タイプ
	str.Format( "%d", drec->dtype ); line += (str + sep);

// 内部伝票番号
// 10.08 '08
#ifdef CLOSE
	if( drec->icno == -1 )
		str.Empty();
	else
		str.Format( "%d", drec->icno );
	line += (str + sep );
#endif

	// 内部伝票番号
#ifdef DBPICK_CLOSE
	if( m_icno != drec->icno ) {

		m_icno = drec->icno;

		if( m_icno < 0 )
			m_set_icno = -1;
		else if( m_icno == 0 )
			m_set_icno = 0;
		else
			m_set_icno = m_Zmp->DcntlLock( 36, 8 );	// icno 付け直し


	}
	drec->icno = m_set_icno;
#endif
	drec->icno = -1;

	if( drec->icno < 0 )
		str.Empty();
	else
		str.Format( "%d", drec->icno );

	line += (str + sep );


	// 資金繰り科目
	if( drec->skn.IsEmpty() )
		str.Empty();
	else
		str = drec->skn;
	line += (str + sep);

	// 変動事由コード NOT NULL
	str.Format( "%d", drec->hjiyu ); line += (str + sep);

	// 対価金額
	str = drec->taika; line += (str + sep);

	// サイン (reserve)
	for( i = 0, j = (int)drec->tsign.GetSize(), max = 1 ; i < max ; ++i ) {
		if( i < j )
			str.Format( "%02x", drec->tsign[i] );
		else
			str = "00";
		line += str;
	}
	line += sep;
	//str.Format( "%02x", 0 ); line += (str + sep);

	// 摘要
	str = drec->tekiyo; line += (str + sep);

	// イメージ摘要番号
	if( drec->imgsq == -1 )
		str.Empty();
	else
		str.Format( "%d", drec->imgsq );
	line += (str + sep);

	// 手形番号
//	str = drec->snumber;
	str = drec->rnumber;
	line += (str + sep);

	// 手形期日
	if( drec->tdate == -1 )
		str.Empty();
	else
		str.Format( "%08d", drec->tdate );
	line += (str + sep);

	// 仕訳作成年月日
#ifndef CLOSE	// 07.12 /08
	str = m_Zmp->DBGetdate().Left( 8+2 );	// "YYYY-MM-DD"
	str.Remove( '-' );						// "YYYYMMDD"
#else
	if( drec->edate == -1 )
		str.Empty();
	else
		str.Format( "%08d", drec->edate );
#endif
	line += (str + sep);

	// 仕訳変更日付	//////////////////
	//if( drec->udate == -1 )
	//	str.Empty();
	//else
	//	str.Format( "%08d", drec->udate );
	str.Empty();
	line += (str + sep);

	// 付箋コメント
	str = drec->tag; line += (str + sep);

	// 変更ユーザー	//////////////////
	//if( drec->cuser == -1 )
	//	str.Empty();
	//else
	//	str.Format( "%d", drec->cuser );
	str.Empty();
	line += str;	// Not sep

	// line end terminator
	str.Format( "\n" ); line += str;

	return line;
}
#endif	// end of _BLK_DATA_