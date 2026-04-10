#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// テーブル作成用のスクリプトを取得
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//		table		：	マスター下のテーブル情報
//		systbl		：	SYSCTL下のテーブル情報
//		tmax		：	テーブル数
//-----------------------------------------------------------------------------
void CSyzMakeSubW::GetMakeTableScript( SYZMAKE_MKCOINFO &mkcoinfo, char*** table, char*** systbl, int &tmax )
{
	if( mkcoinfo.apno == KOEKI_APNO ){
		*table	= INSERT_TARGET_TABLE_NP;
		*systbl	= INSERT_TARGET_TABLE_NP;
		tmax	= INSERT_TABLE_MAX_NP;
	}
	else if( mkcoinfo.apno == KOEKI3_APNO ){
		*table	= INSERT_TARGET_TABLE_NP3;
		*systbl	= INSERT_TARGET_TABLE_NP3;
		tmax	= INSERT_TABLE_MAX_NP3;
	}
	else if( mkcoinfo.apno == SYAKAI_APNO ){
		if( mkcoinfo.nptyp&0x01 ){	// 新会計基準
			*table	= INSERT_TARGET_TABLE_SFKS2;
			*systbl	= INSERT_TARGET_TABLE_SFKS2_SYS;
			tmax	= INSERT_TABLE_MAX_SFKS2;
		}
		else{						// 旧会計基準
			*table	= INSERT_TARGET_TABLE_SFKS;
			*systbl	= INSERT_TARGET_TABLE_SFKS;
			tmax	= INSERT_TABLE_MAX_SFKS;
		}
	}
	else if( mkcoinfo.apno == SYUKYO_APNO ){
		*table	= INSERT_TARGET_TABLE_SKYO;
		*systbl	= INSERT_TARGET_TABLE_SKYO;
		tmax	= INSERT_TABLE_MAX_SKYO;
	}
	else if( mkcoinfo.apno == GAKKO_APNO ){
		*table	= INSERT_TARGET_TABLE_GAKKO;
		*systbl	= INSERT_TARGET_TABLE_GAKKO;
		tmax	= INSERT_TABLE_MAX_GAKKO;
	}
	else{			// 営利法人
		*table	= INSERT_TARGET_TABLE;
		*systbl	= INSERT_TARGET_TABLE;
		tmax	= INSERT_TABLE_MAX;
	}
}

//-----------------------------------------------------------------------------
// データベース内 テーブルデータの挿入
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::InsertInitTbData( SYZMAKE_MKCOINFO &mkcoinfo )
{
	int		ret = 0, commit = 0;
	CString	dboptSql;

	// リンクサーバーのチェック
	if( _strnicmp(mkcoinfo.dbSvrName, mkcoinfo.dbSysDbName, 256) != 0 ){
		ret = m_DBAccess.CheckLinkedServer( DBA_SYS_ACCESS, mkcoinfo.dbSvrName );
		if( ret <= 0 ){
			if( m_DBAccess.AddLinkedServer(DBA_SYS_ACCESS, mkcoinfo.dbSvrName) < 0 ){
				if( m_ErrMsg.IsEmpty() ){
					m_DBAccess.GetErrorMessage( m_ErrMsg );
				}
				return -1;
			}
		}
	}

	// トランザクション開始
	if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, _T("begin tran CSyzMakeSubW_MakeDBMaster_t1")) < 0 ){
		goto COPY_ERROR;
	}

	char	**table = NULL, **systbl = NULL;
	int		tmax = 0;

#ifdef _20160613_CUT_
//-- '14.11.05 --
//	if( mkcoinfo.apno == KOEKI_APNO ){
//		table	= INSERT_TARGET_TABLE_NP;
//		systbl	= INSERT_TARGET_TABLE_NP;
//		tmax	= INSERT_TABLE_MAX_NP;
//	}
//	else if( mkcoinfo.apno == KOEKI3_APNO ){
//		table	= INSERT_TARGET_TABLE_NP3;
//		systbl	= INSERT_TARGET_TABLE_NP3;
//		tmax	= INSERT_TABLE_MAX_NP3;
//	}
//	else if( mkcoinfo.apno == SYAKAI_APNO ){
//		if( mkcoinfo.nptyp&0x01 ){	// 新会計基準
//			table	= INSERT_TARGET_TABLE_SFKS2;
//			systbl	= INSERT_TARGET_TABLE_SFKS2_SYS;
//			tmax	= INSERT_TABLE_MAX_SFKS2;
//		}
//		else{						// 旧会計基準
//			table	= INSERT_TARGET_TABLE_SFKS;
//			systbl	= INSERT_TARGET_TABLE_SFKS;
//			tmax	= INSERT_TABLE_MAX_SFKS;
//		}
//	}
//	else if( mkcoinfo.apno == SYUKYO_APNO ){
//		table	= INSERT_TARGET_TABLE_SKYO;
//		systbl	= INSERT_TARGET_TABLE_SKYO;
//		tmax	= INSERT_TABLE_MAX_SKYO;
//	}
//	else if( mkcoinfo.apno == GAKKO_APNO ){
//		table	= INSERT_TARGET_TABLE_GAKKO;
//		systbl	= INSERT_TARGET_TABLE_GAKKO;
//		tmax	= INSERT_TABLE_MAX_GAKKO;
//	}
//	else{			// 営利法人
//		table	= INSERT_TARGET_TABLE;
//		systbl	= INSERT_TARGET_TABLE;
//		tmax	= INSERT_TABLE_MAX;
//	}
//---------------
	if( mkcoinfo.mkOpt1&0x01 ){
		table	= INSERT_TARGET_TABLE_ONLYTAX;
		systbl	= INSERT_TARGET_TABLE_ONLYTAX;
		tmax	= INSERT_TABLE_MAX_ONLYTAX;
	}
	else{
		if( mkcoinfo.apno == KOEKI_APNO ){
			table	= INSERT_TARGET_TABLE_NP;
			systbl	= INSERT_TARGET_TABLE_NP;
			tmax	= INSERT_TABLE_MAX_NP;
		}
		else if( mkcoinfo.apno == KOEKI3_APNO ){
			table	= INSERT_TARGET_TABLE_NP3;
			systbl	= INSERT_TARGET_TABLE_NP3;
			tmax	= INSERT_TABLE_MAX_NP3;
		}
		else if( mkcoinfo.apno == SYAKAI_APNO ){
			if( mkcoinfo.nptyp&0x01 ){	// 新会計基準
				table	= INSERT_TARGET_TABLE_SFKS2;
				systbl	= INSERT_TARGET_TABLE_SFKS2_SYS;
				tmax	= INSERT_TABLE_MAX_SFKS2;
			}
			else{						// 旧会計基準
				table	= INSERT_TARGET_TABLE_SFKS;
				systbl	= INSERT_TARGET_TABLE_SFKS;
				tmax	= INSERT_TABLE_MAX_SFKS;
			}
		}
		else if( mkcoinfo.apno == SYUKYO_APNO ){
			table	= INSERT_TARGET_TABLE_SKYO;
			systbl	= INSERT_TARGET_TABLE_SKYO;
			tmax	= INSERT_TABLE_MAX_SKYO;
		}
		else if( mkcoinfo.apno == GAKKO_APNO ){
			table	= INSERT_TARGET_TABLE_GAKKO;
			systbl	= INSERT_TARGET_TABLE_GAKKO;
			tmax	= INSERT_TABLE_MAX_GAKKO;
		}
		else{			// 営利法人
			table	= INSERT_TARGET_TABLE;
			systbl	= INSERT_TARGET_TABLE;
			tmax	= INSERT_TABLE_MAX;
		}
	}
//---------------
#else
	GetMakeTableScript( mkcoinfo, &table, &systbl, tmax );
#endif

	// データベーステーブルコピー
	for( int i=0; i<tmax; i++ ){

		char copy[512] = {0};

		// ボリュームラベル
		if( strcmp(*(table+i), _T("zvolume")) == 0 ){
			char buf[2048] = { 0 };
			MakeVolumeDataSql( buf, mkcoinfo );
			CString str;
			str.Format( _T("INSERT INTO [%s].[dbo].[%s] VALUES (%s)"), m_mkCoName, *(table+i), buf );
			if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, str) < 0 ){
				goto COPY_ERROR;
			}
		}
		// 部門ボリュームラベル
		else if( strcmp(*(table+i), _T("bmvol")) == 0 ){
			char buf[2048] = { 0 };
			MakeBmVolumeDataSql( buf, mkcoinfo );
			CString str;
			str.Format( _T("INSERT INTO [%s].[dbo].[%s] VALUES (%s)"), m_mkCoName, *(table+i), buf );
			if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, str) < 0 ){
				goto COPY_ERROR;
			}
		}
		// 月別情報
		else if( strcmp(*(table+i), _T("month_info")) == 0 ){
			char buf[512] = { 0 };
			for( int j=0; j<MONTHALL; j++ ){
				CString str;
				MakeMonthInfoDataSql( j, buf, mkcoinfo );
				str.Format( _T("INSERT INTO [%s].[dbo].[%s] VALUES (%s)"), m_mkCoName, *(table+i), buf );
				if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, str) < 0 ){
					goto COPY_ERROR;
				}
				memset( buf, '\0', sizeof(buf) );
			}
		}
		// 前期月別情報
		else if( strcmp(*(table+i), _T("zmonth_info")) == 0 ){
			char buf[512] = { 0 };
			for( int j=0; j<4; j++ ){
				for( int k=0; k<TSCOUNT; k++ ){
					MakeZMonthInfoDataSql( j, k, buf, mkcoinfo );
					CString str;
					str.Format( _T("INSERT INTO [%s].[dbo].[%s] VALUES (%s)"), m_mkCoName, *(table+i), buf );
					if ( m_DBAccess.ExecuteSQL( DBA_MST_ACCESS, str ) < 0 ){
						goto COPY_ERROR;
					}
					memset( buf, '\0', sizeof(buf) );
				}
			}
		}
		else{
			if( !strcmp(*(table+i), _T("rdlabel")) ){
				if( m_DBAccess.ExecuteSQL(DBA_SYS_ACCESS, _T("SET IDENTITY INSERT rdlabel ON")) < 0 ){
					goto COPY_ERROR;
				}
			}
			if( !strcmp(*(table+i), _T("fuzokukamoku")) ){
//				CString sql;
//				sql.Format( _T("SET IDENTITY_INSERT [%s].[dbo].[%s] ON"), m_mkCoName, *(table+i) );
//				if ( m_DBAccess.ExecuteSQL( DBA_MST_ACCESS, sql ) < 0 )
//					goto COPY_ERROR;
			}
			if( _strnicmp(mkcoinfo.dbSvrName, mkcoinfo.dbSysDbName, 256) ){
				if( !strcmp( *(table+i), _T("kshctl") ) || !strcmp( *(table+i), _T("kshnam") ) || !strcmp( *(table+i), _T("kshrec") ) || !strcmp( *(table+i), _T("kprf") ) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[%s]"),
							mkcoinfo.dbSvrName, m_mkCoName, *(table+i), *(systbl+i) );
				}
				else if( !strcmp( *(table+i), _T("anaclq") ) || !strcmp( *(table+i), _T("anahyo") ) || !strcmp( *(table+i), _T("anahyoitm") ) || 
					!strcmp( *(table+i), _T("anaitm") ) || !strcmp( *(table+i), _T("anasyk") ) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[%s%02x]"),
						mkcoinfo.dbSvrName, m_mkCoName, *(table+i), *(systbl+i), mkcoinfo.apno );
				}
/*				else if( (mkcoinfo.apno == KOJIN_APNO) && (m_nIfn&0x100) && (!strcmp( *(table+i), _T("knrec") ) || !strcmp( *(table+i), _T("gkrec") ) ||
					!strcmp( *(table+i), _T("kzrec") ) || !strcmp( *(table+i), _T("kzysrec") )) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[Z%02x%sk]"),
							mkcoinfo.dbSvrName, m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				}*/
/*				else if( (mkcoinfo.apno == KOJIN_APNO) && (m_nKgsyu == 0) && !strcmp( *(table+i), _T("knrec") ) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[Z%02x%s1]"),
							mkcoinfo.dbSvrName, m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				}*/
//				else if ( strcmp( *(table+i), _T("fuzokukamoku") ) == 0 )
//					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[%s].[dbo].[%s](no,kind,uchisign,editsign,kncod,knseq,knatt,knvoid,kubun) SELECT no,kind,uchisign,editsign,kncod,knseq,knatt,knvoid,kubun FROM [SYSCTL].[dbo].[Z%02x%s]"),
//							mkcoinfo.dbSvrName, m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				else{
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[Z%02x%s]"),
							mkcoinfo.dbSvrName, m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				}
/*				if ( !strcmp( *(table+i), _T("kshctl") ) || !strcmp( *(table+i), _T("kshnam") ) | !strcmp( *(table+i), _T("kshrec") ) )
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [%s].[SYSCTL].[dbo].[%s]"),
							m_mkCoName, *(table+i), m_SysDbName, *(systbl+i) );
				else if ( !strcmp( *(table+i), _T("anaclq") ) || !strcmp( *(table+i), _T("anahyo") ) || !strcmp( *(table+i), _T("anahyoitm") ) || 
						!strcmp( *(table+i), _T("anaitm") ) || !strcmp( *(table+i), _T("anasyk") ) )
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [%s].[SYSCTL].[dbo].[%s%02x]"),
							m_mkCoName, *(table+i), m_SysDbName, *(systbl+i), mkcoinfo.apno );
				else if ( (mkcoinfo.apno == KOJIN_APNO) && (m_nKgsyu == 0) && !strcmp( *(table+i), _T("knrec") ) )
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [%s].[SYSCTL].[dbo].[Z%02x%s1]"),
							m_mkCoName, *(table+i), m_SysDbName, mkcoinfo.apno, *(systbl+i) );
				else
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [%s].[SYSCTL].[dbo].[Z%02x%s]"),
							m_mkCoName, *(table+i), m_SysDbName, mkcoinfo.apno, *(systbl+i) );*/
			}
			else{
				if( !strcmp( *(table+i), _T("kshctl") ) || !strcmp( *(table+i), _T("kshnam") ) || !strcmp( *(table+i), _T("kshrec") ) || !strcmp( *(table+i), _T("kprf") ) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[%s]"),
							m_mkCoName, *(table+i), *(systbl+i) );
				}
				else if( !strcmp( *(table+i), _T("anaclq") ) || !strcmp( *(table+i), _T("anahyo") ) || !strcmp( *(table+i), _T("anahyoitm") ) || 
					!strcmp( *(table+i), _T("anaitm") ) || !strcmp( *(table+i), _T("anasyk") ) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[%s%02x]"),
							m_mkCoName, *(table+i), *(systbl+i), mkcoinfo.apno );
				}
/*				else if( (mkcoinfo.apno == KOJIN_APNO) && (m_nIfn&0x100) && (!strcmp( *(table+i), _T("knrec") ) || !strcmp( *(table+i), _T("gkrec") )||
					!strcmp( *(table+i), _T("kzrec") ) || !strcmp( *(table+i), _T("kzysrec") )) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[Z%02x%sk]"),
							m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				}*/
/*				else if( (mkcoinfo.apno == KOJIN_APNO) && (m_nKgsyu == 0) && !strcmp( *(table+i), _T("knrec") ) ){
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[Z%02x%s1]"),
							m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				}*/
//				else if ( strcmp( *(table+i), _T("fuzokukamoku") ) == 0 )
//					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s](no,kind,uchisign,editsign,kncod,knseq,knatt,knvoid,kubun) SELECT no,kind,uchisign,editsign,kncod,knseq,knatt,knvoid,kubun FROM [SYSCTL].[dbo].[Z%02x%s]"),
//							m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				else{
					sprintf_s( copy, sizeof(copy), _T("INSERT INTO [%s].[dbo].[%s] SELECT * FROM [SYSCTL].[dbo].[Z%02x%s]"),
							m_mkCoName, *(table+i), mkcoinfo.apno, *(systbl+i) );
				}
			}
			if( m_DBAccess.ExecuteSQL( DBA_SYS_ACCESS, copy ) < 0 ){
//			if ( m_DBAccess.ExecuteSQL( DBA_MST_ACCESS, copy ) < 0 )
				goto COPY_ERROR;
			}

			if( strcmp( *(table+i), _T("rdlabel") ) == 0 ){
				if ( m_DBAccess.ExecuteSQL( DBA_SYS_ACCESS, _T("SET IDENTITY INSERT rdlabel OFF") ) < 0 ){
					goto COPY_ERROR;
				}
			}
			if ( !strcmp( *(table+i), _T("fuzokukamoku") ) ) {
//				CString sql;
//				sql.Format( _T("SET IDENTITY_INSERT [%s].[dbo].[%s] OFF"), m_mkCoName, *(table+i) );
//				if ( m_DBAccess.ExecuteSQL( DBA_MST_ACCESS, sql ) < 0 )
//					goto COPY_ERROR;
			}
		}
	}

	// トランザクション正常終了
	if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, _T("commit tran CSyzMakeSubW_MakeDBMaster_t1")) < 0 ){
		goto COPY_ERROR;
	}
	commit = 1;

	// チェックポイント
	if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, _T("checkpoint")) < 0 ){
		goto COPY_ERROR;
	}

	// 自動終了オプション
	dboptSql.Format( _T("ALTER DATABASE [%s] SET AUTO_CLOSE ON"), m_mkCoName );
	if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, dboptSql) < 0 ){
		goto COPY_ERROR;
	}

	if( _strnicmp(mkcoinfo.dbSvrName, mkcoinfo.dbSysDbName, 256) != 0 ){
		m_DBAccess.DropServer( DBA_SYS_ACCESS, mkcoinfo.dbSvrName );
	}

	return 0;

COPY_ERROR:
	if( commit == 0 ){
		m_DBAccess.ExecuteSQL( DBA_MST_ACCESS, _T("rollback tran CSyzMakeSubW_MakeDBMaster_t1") );
	}

	if( _strnicmp(mkcoinfo.dbSvrName, mkcoinfo.dbSysDbName, 256) != 0 ){
		m_DBAccess.DropServer( DBA_SYS_ACCESS, mkcoinfo.dbSvrName );
	}

	m_DBAccess.GetErrorMessage( m_ErrMsg );

	return -1;
}

//-----------------------------------------------------------------------------
// ボリュームラベルデータ用SQLの作成
//-----------------------------------------------------------------------------
// 引数	pbuf		：	SQLデータの作成先
//		mkcoinfo	：	作成先の情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CSyzMakeSubW::MakeVolumeDataSql( char *pbuf, SYZMAKE_MKCOINFO &mkcoinfo )
{
	CString str, tstr;

	// ボリュームラベルバージョン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.vver, 4, tstr );
	// 無効化バージョン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.voidver, 4, tstr );
	// アプリケーション番号
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.apno, 1, tstr );
	// マスターバージョン
	if( m_mkZVolume.apno == SYAKAI_APNO ){
		if( mkcoinfo.nptyp&0x01 ){
			m_mkZVolume.m_ver = 0x11;
		}
		else{
			m_mkZVolume.m_ver = 0x10;
		}
	}
	else{
		if( (mkcoinfo.nIfn&0x0f) ){
			m_mkZVolume.m_ver = 0x12;
		}
		else{
			m_mkZVolume.m_ver = 0x10;
		}
	}
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.m_ver, 4, tstr );
	// マスタータイプ
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.m_type, 1, tstr );
	// 月次　保存月数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.g_hsw, 1, tstr );
	// 0 = 全データあり、1 = なし
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.d_nall, 1, tstr );
	// 残高集計不良サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.z_fail, 1, tstr );
	// 電子データ保存サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.e_save, 1, tstr );
	// 税効果マスターサイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.z_efct, 1, tstr );
	// 会社コード
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.v_cod, 4, tstr );
	// 会社名
	ConvertToSqldata( ID_ICSSYZ_STRING_SQLTYPE, (LPVOID)(LPCTSTR)m_mkZVolume.c_nam, 0, tstr );
	// 期首年月日
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ss_ymd, 4, tstr );
	// 期末年月日
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ee_ymd, 4, tstr );
	// 計算上期首年月日
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.cs_ymd, 4, tstr );
	// 開始状態
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ss_jsw, 1, tstr );
	// 入力開始処理年月日
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ip_ymd, 4, tstr );
	// 部門桁数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.bmcol, 1, tstr );
	// 工事番号桁数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.kjcol, 1, tstr );
	// 枝番桁数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.edcol, 1, tstr );
	// 科目残　保存年数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.zk_hn, 1, tstr );
	// 科目残　前４年分　登録サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.zk_sg, 1, tstr );
	// 枝番残　保存年数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.zb_hn, 1, tstr );
	// 枝番残　前４年分　登録サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.zb_sg, 1, tstr );
	// 当期枝番残高登録sw
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.br_sw, 1, tstr );
	// 資金繰残高登録sw
	BYTE apno = m_mkZVolume.apno;
	BYTE cksw = m_mkZVolume.ck_sw;
	if( (apno==HOJIN_APNO) || (apno==UNSO_APNO) || (apno==KENSETU_APNO) || (apno==BYOIN_APNO) ){
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ck_sw, 1, tstr );
	}
	else{
		cksw = 0;
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &cksw, 1, tstr );
	}
	// subシステム登録sw
/*- '13.08.23 -*/
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.sub_sw, 4, tstr );
/*-------------*/
//	int	tmpSub_sw = m_mkZVolume.sub_sw&0xffff;
	int	tmpSub_sw = m_mkZVolume.sub_sw&0x7fff;
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &tmpSub_sw, 4, tstr );
/*-------------*/
	// 自動棚卸　サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.a_tana, 1, tstr );
	// 伝票番号使用サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.dpsgn, 1, tstr );
	// 証憑番号、手形期日入力サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.evdsgn, 1, tstr );
	// 部門コード　入力サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.bm_isw, 1, tstr );
	// 工事番号　入力サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.kj_isw, 1, tstr );
	// 枝番名称　出力サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.br_nsw, 1, tstr );
	// 科目コード　出力サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.kc_psw, 1, tstr );
	// 摘要入力文字数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.tk_ln, 1, tstr );
	// 連想摘要サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.tk_sw, 1, tstr );
	// 摘要枝番サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.tk_br, 1, tstr );
	// 通常入力オプション
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.in_opt, 4, tstr );
	// 転送済み仕訳修正許可 sw
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.tl_cor, 1, tstr );
	// ＡＮＫ入力サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ank_in, 1, tstr );
	// 転送済みＳＮＯ　データ送信
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.dlin_t, 4, tstr );
	// 消費税マスター登録サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_mst, 1, tstr );
	// 消費税対応バージョン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_ver, 1, tstr );
	// 分離・税込み区分
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_sgn1, 1, tstr );
	// 課税方式区分
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_sgn2, 1, tstr );
	// 科目・摘要属性登録 sw
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_sgn3, 1, tstr );
	// 処理選択 sw
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_sgn4, 1, tstr );
	// 一括税抜　処理月サイン  0=期末、1=各月
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_sgn5, 1, tstr );
	// 消費税集計サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_sgn6, 1, tstr );
	// 個人月別収支サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_sw, 1, tstr );
	// 個人みなし法人サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_kk, 1, tstr );
	// 個人控除額
	char			mny[32] = {0};
	unsigned char	m_Aomny[6] = {0};
	m_Arith.l_defn( 0x16 );
	m_Arith.l_print( mny, m_Aomny, _T("sssssssssss9") );
//	ConvertToSqldata( 2, (LPVOID)(LPCTSTR)m_mkZVolume.ind_vl, 0, tstr );
	ConvertToSqldata( ID_ICSSYZ_NUMSTR_SQLTYPE, mny, 0, tstr );
	// 学校資金収支サイン　1=全科目
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.sch_ca1, 1, tstr );
	// 学校現預金明細
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.sch_ca2, 1, tstr );
	// 学校前年度支払金
	ConvertToSqldata( ID_ICSSYZ_NUMSTR_SQLTYPE, (LPVOID)(LPCTSTR)m_mkZVolume.sch_ca3, 0, tstr );
	// 学校翌年度支払金
	ConvertToSqldata( ID_ICSSYZ_NUMSTR_SQLTYPE, (LPVOID)(LPCTSTR)m_mkZVolume.sch_ca4, 0, tstr );
	// イメージファイル存在デバイス
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ocr_idev, 1, tstr );
/*- ICS_SYZ -*/
//	// 決算タイプ
//	int ktype = 0;
//	switch ( m_nKsgn ) {
//		case 0:		ktype = 3;		break;		// 四半期決算
//		case 1:		ktype = 2;		break;		// 中間決算
//		case 2:		ktype = 0;		break;		// 期末決算
//		case 3:		ktype = 1;		break;		// 決算なし
//	}
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &ktype, 1, tstr );
/*-----------*/
	// 決算タイプ
	int ktype = 0;	// 期末決算
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &ktype, 1, tstr );
/*-----------*/

/*- ICS_SYZ -*/
//	// 前期棚卸サイン
//	BYTE flg = ((m_nSflg>>(1*4))&0x0F);
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
//	// 前々期棚卸サイン
//	flg = ((m_nSflg>>(2*4))&0x0F);
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
//	// ３期前棚卸サイン
//	flg = ((m_nSflg>>(3*4))&0x0F);
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
//	// ４期前棚卸サイン
//	flg = ((m_nSflg>>(4*4))&0x0F);
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
/*-----------*/
	// 前期～４期前 棚卸サイン
	BYTE flg = 0x00;
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &flg, 1, tstr );
/*-----------*/
	// 前期～４期前　期首・期末年月日
	int nssymd = 0, npssymd = 0, npeeymd = 0;
	UCHAR ssymd[3] = { 0 }, pssymd[3] = { 0 }, peeymd[3] = { 0 };
//	db_datecnv( m_mkZVolume.ss_ymd, &nssymd, 0, 0 );		// 西暦→平成
	// 西暦→和暦
	int	ssgengo=0, ppgengo=0;
	db_datecnvGen( 0, m_mkZVolume.ss_ymd, &ssgengo, &nssymd, 0, 0 );
	int_bcd( ssymd, nssymd, 3 );
	// 期首日の前日を取得
//	_vd_getprevdateex( peeymd, ssymd );
	_vd_getprevdateexGen2( &ppgengo, peeymd, ssgengo, ssymd );

	// 取得データの確認
	// 和暦→西暦
	int		tmpgengo=0, ntmpymd=0;
	UCHAR	bcd_tmpymd[3]={0}, bcd_ssymd[3]={0};
	int_bcd( bcd_ssymd, (m_mkZVolume.ss_ymd%1000000), 3 );
	_db_datecnvGen( ppgengo, peeymd, &tmpgengo, bcd_tmpymd, 1, 0 );

	if ( _vd_ymdcmp( bcd_ssymd, bcd_tmpymd ) < 0 ){
		// int -> BCD
		UCHAR	bcd_tmpYmd[3]={0};
		int_bcd( bcd_tmpYmd, (ntmpymd%1000000), 3 );
		_vd_calcbcd( bcd_tmpYmd[0], 1, 1 );
		// 西暦→和暦
		_db_datecnvGen( 0, bcd_tmpYmd, &ppgengo, peeymd, 0, 0 );
	}

	// 和暦→西暦
	_db_datecnvGen( ppgengo, peeymd, &tmpgengo, bcd_tmpymd, 1, 0 );
	// 西暦で期首を取得
	_vd_getsymd( pssymd, bcd_tmpymd );

//	npssymd = bcd_int( pssymd, 3 );
//	npeeymd = bcd_int( peeymd, 3 );
//	db_datecnv( npssymd, &npssymd, 1, 0 );		// 平成→西暦
//	db_datecnv( npeeymd, &npeeymd, 1, 0 );
	// 欲しいのは西暦のINT
	// 西暦→和暦（BCD）
	UCHAR	bcd_wPssymd[3]={0}, bcd_wPeeymd[3]={0};
	int		pssgengo=0, peegengo=0;
	_db_datecnvGen( 0, pssymd, &pssgengo, bcd_wPssymd, 0, 0 );
	_db_datecnvGen( 0, bcd_tmpymd, &peegengo, bcd_wPeeymd, 0, 0 );
	npssymd = bcd_int( bcd_wPssymd, 3 );
	npeeymd = bcd_int( bcd_wPeeymd, 3 );
	// 和暦→西暦（INT）
	db_datecnvGen( pssgengo, npssymd, &tmpgengo, &npssymd, 1, 0 );
	db_datecnvGen( peegengo, npeeymd, &tmpgengo, &npeeymd, 1, 0 );
	for ( int i = 0; i < 4; i++ ) {
		UCHAR s[3] = { 0 }, e[3] = { 0 };
		long symd = npssymd-(10000*i);
		long eymd = npeeymd-(10000*i);
		int_bcd( s, (symd%1000000), 3 );
		int_bcd( e, (eymd%1000000), 3 );
		_vd_getfebeday( s );
		_vd_getfebeday( e );
		symd = ((symd/1000000)*1000000);
		eymd = ((eymd/1000000)*1000000);
		symd += bcd_int( s, 3 );
		eymd += bcd_int( e, 3 );
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &symd, 4, tstr );
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &eymd, 4, tstr );
	}
	// 決算期
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ki, 4, tstr );
	// 税入力方式サイン
//	ConvertToSqldata( 0x10, &m_mkZVolume.s_sgn7, 1, tstr );
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.s_sgn7, 1, tstr );

	// 個人業種サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_type, 1, tstr );
	// 総額積み上げサイン
//	ConvertToSqldata( 0x10, &m_mkZVolume.so_tumisgn, 1, tstr );
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.so_tumisgn, 1, tstr );

	// 未入力部門集計コード
	tstr += _T("NULL,");

	// 個人業種テーブルサイン
/*- ICS_SYZ -*/
//	BYTE ind_table = (m_nIfn&0x100) ? 1 : 0;
////	ConvertToSqldata( 0x10, &ind_table, 1, tstr );
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &ind_table, 1, tstr );
/*-----------*/
	BYTE ind_table = 0;
	if( mkcoinfo.nIfn&0x100 ){
		ind_table = 1;
	}
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_table, 1, tstr );
/*-----------*/

	// 個人控除優先順位
/*- ICS_SYZ -*/
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_bodr, 1, tstr );
/*-----------*/
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_kjpriority, 1, tstr );
/*-----------*/

	// 個人不動産１０万限度
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_kjlimit, 1, tstr );

	// 個人サイン
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_sgn1, 1, tstr );
/*- ICS_SYZ -*/
//	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_bssw, 1, tstr );
/*-----------*/
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_sgn2, 1, tstr );
/*-----------*/

	int clm = GetColumnCount( _T("zvolume"), mkcoinfo.dbSvrName );

	if( clm == CLMMAX_ZVOLUME ){
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_sgn3, 1, tstr, FALSE );
	}
	else{
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.ind_sgn3, 1, tstr );
	}

	// 各バージョンによってカラム数が違うので、ここから先は切り分け
	if( clm > CLMMAX_ZVOLUME ){
		// docid
		ConvertToSqldata( ID_ICSSYZ_NULL_SQLTYPE, &m_mkZVolume.docid, 1, tstr );
		// cnocol
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.cnocol, 1, tstr );
		// snocol
		if( clm == (CLMMAX_ZVOLUME+3) ){
			ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.snocol, 1, tstr, FALSE );
		}
		else{
			ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_mkZVolume.snocol, 1, tstr );
		}
	}
	if( clm > CLMMAX_ZVOLUME+3 ){
		// hojin_no
		ConvertToSqldata( ID_ICSSYZ_NULL_SQLTYPE, &m_mkZVolume.hojin_no, 1, tstr );
		// hojin_no_ext
		ConvertToSqldata( ID_ICSSYZ_NULL_SQLTYPE, &m_mkZVolume.hojin_no_ext, 1, tstr );
		// kojin_hash
		if( clm == (CLMMAX_ZVOLUME+6) ){
			ConvertToSqldata( ID_ICSSYZ_NULL_SQLTYPE, &m_mkZVolume.kojin_hash, 1, tstr, FALSE );
		}
		else{
			ConvertToSqldata( ID_ICSSYZ_NULL_SQLTYPE, &m_mkZVolume.kojin_hash, 1, tstr );
		}
	}

	// ダミーデータ挿入
	if( clm > (CLMMAX_ZVOLUME+6) ){
		int dmy = 0;
		for( int i=0; i<(clm-(CLMMAX_ZVOLUME+6)); i++ ){
			tstr += ",";
			ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &dmy, 1, tstr, FALSE );
		}
	}

	memmove( pbuf, tstr, tstr.GetLength() );

	return 0;
}

//-----------------------------------------------------------------------------
// SQL文字列データへの変換
//-----------------------------------------------------------------------------
// 引数	nType		：	変換方法
//		pData		：	元データ
//		nSize		：	データサイズ
//		rStr		：	データ格納用
//		doSeparate	：	データ分割を行うかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ConvertToSqldata( EnumIdIcsSyzCnvSqlType nType, void* pData, int nSize, CString& rStr, BOOL doSeparate/*=TRUE*/ )
{
	CString str;

	if( nType == ID_ICSSYZ_NUMBER_SQLTYPE ){
		switch ( nSize ) {
			case 1:
				str.Format( _T("%d"), *(BYTE*)pData );
				break;
			case 2:
				str.Format( _T("%d"), *(USHORT*)pData );
				break;
			case 4:
				str.Format( _T("%d"), *(UINT*)pData );
				break;
		}
	}
	else if( nType == ID_ICSSYZ_STRING_SQLTYPE ){
		char bf1[256] = {0}, bf2[256] = {0};
		CCvtString	cvt;
		CString		buf;
		buf.Format( _T("%s"), pData );
		if( cvt.GetMBCharType(buf, (buf.GetLength()-1)) == MBTYPE_LEAD ){
			buf = buf.Left( buf.GetLength()-1 );
		}
		buf.Replace( _T("'"), _T("''") );
		str.Format( _T("\'%s\'"), buf );
	}
	else if( nType == ID_ICSSYZ_NUMSTR_SQLTYPE ){
		int num = atoi( (LPCTSTR)pData );
		str.Format( _T("%d"), num );
	}
	else if( nType == ID_ICSSYZ_DATE_SQLTYPE ){
//		UCHAR nymd[3] = { 0 };
//		m_VolDate.db_datecnv( (UCHAR*)pData, nymd, 1, 0 );		// 平成→西暦変換
		// 和暦→西暦
		JCYMD	*pJcymd = (JCYMD*)pData;
		int		tmpgengo=0, nWYmd=0, nSYmd=0;
		nWYmd = bcd_int( pJcymd->ymd, 3 );
		m_VolDate.db_datecnvGen( pJcymd->gengo, nWYmd, &tmpgengo, &nSYmd, 1, 0 );
		str.Format( _T("%d"), nSYmd );
	}
	else if( nType == ID_ICSSYZ_NULL_SQLTYPE ){
		str.Format( _T("NULL") );
	}

	if( doSeparate ){
		str += _T(",");
	}

	rStr += str;

	return 0;
}

//-----------------------------------------------------------------------------
// データベースカラム数取得
//-----------------------------------------------------------------------------
// 引数	pTname		：	テーブル名
//-----------------------------------------------------------------------------
// 返送値	0以上	：	カラム数
//			-1		：	エラー
//			-2		：	例外
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetColumnCount( LPCTSTR pTname, LPCTSTR pSvrName )
{
	int dbaid = m_DBAccess.OpenDatabase( pSvrName, m_mkCoName );
	if( dbaid < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -1;
	}
	int count = m_DBAccess.GetColumnCount( dbaid, pTname );
	if( count < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
	}
	m_DBAccess.CloseDatabase( dbaid );

	return count;
}

//-----------------------------------------------------------------------------
// 部門ボリュームラベルデータ用SQLの作成
//-----------------------------------------------------------------------------
// 引数	pbuf		：	SQLデータの作成先
//		mkcoinfo	：	作成先の情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CSyzMakeSubW::MakeBmVolumeDataSql( char *pbuf, SYZMAKE_MKCOINFO &mkcoinfo )
{
	CString str, tstr;

	// 部門マスターバージョン
	tstr += _T("0,");
	// 配賦マスターバージョン
	tstr += _T("0,");
	// 前期科目残高登録ＳＷ
	tstr += _T("15,");
	// 配賦サイン
//	ConvertToSqldata( 0, &m_nHsgn, 1, tstr );
	tstr += _T("0,");
	// 部門コード集計
	tstr += _T("0,");
	// 予算実績サイン
	tstr += _T("0,");
	// 部門コード未入力データ集計サイン
	tstr += _T("0,");
	// 部門コード未入力時の集計部門
	tstr += _T("-1,");
	for( int i=0; i<5; i++ ){
		// 経常利益配賦率１
		tstr += _T("100,");
		// 経常利益配賦率２
		tstr += _T("0,");
		// 経常利益配賦率３
		tstr += _T("0,");
		// 経常利益配賦率４
		tstr += _T("0,");
		// 製造経費配賦率１
		tstr += _T("100,");
		// 製造経費配賦率２
		tstr += _T("0,");
		// 製造経費配賦率３
		tstr += _T("0,");
		// 製造経費配賦率４
		if ( i < 4 ){
			tstr += _T("0,");
		}
		else{
			tstr += _T("0");
		}
	}

	// ダミーデータ追加
	int clm = GetColumnCount( _T("bmvol"), mkcoinfo.dbSvrName );
	if( clm > CLMMAX_BVOLUME ){
		int dmy = 0;
		for( int i=0; i<(clm - CLMMAX_BVOLUME); i++ ){
			tstr += _T(",");
			ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &dmy, 1, tstr, FALSE );
		}
	}

	memmove( pbuf, tstr, tstr.GetLength() );

	return 0;
}

//-----------------------------------------------------------------------------
// 月別情報データ用SQLの作成
//-----------------------------------------------------------------------------
// 引数	mofs		：	月オフセット
//		pbuf		：	SQLデータの作成先
//		pDbSvrName	：	作成先のサーバー名
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CSyzMakeSubW::MakeMonthInfoDataSql( int mofs, char *pbuf, char *pDbSvrName )
{
	CString str, tstr;

	// 月オフセット
	ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &mofs, 1, tstr);
	// 入力許可サイン
	if( (m_mkZVolume.m_type&0xff) == 0x01 ){
		tstr += _T("0,");
	}
	else{
		ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &m_nInsw[mofs], 1, tstr);
	}
	// 月次サイン
	tstr += _T("0,");
	// 開始年月日
	JCYMD	JcYmd={ 0 };
	//	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, m_MSymd[mofs], 0, tstr );
	CnvToJcymd(IC_ICSSYZ_START_JCYTYPE, mofs, JcYmd);
	ConvertToSqldata(ID_ICSSYZ_DATE_SQLTYPE, &JcYmd, 0, tstr);
	// 終了年月日
//	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, m_MEymd[mofs], 0, tstr );
	CnvToJcymd(IC_ICSSYZ_END_JCYTYPE, mofs, JcYmd);
	ConvertToSqldata(ID_ICSSYZ_DATE_SQLTYPE, &JcYmd, 0, tstr);
	// 月次処理状態
	tstr += _T("0,");
	// 集計サイン
	tstr += _T("0,");
	// 資金収支集計（学校）
	tstr += _T("0,");
	// 入力仕訳数
	tstr += _T("0,");
	// 従業員数
	if( (m_mkZVolume.apno==KOEKI_APNO) || (m_mkZVolume.apno==KOEKI3_APNO) || (m_mkZVolume.apno==SYAKAI_APNO) || (m_mkZVolume.apno==SYUKYO_APNO) ){
		tstr += _T("0,");
	}
	else{		// 営利法人
		ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &m_nMNdt1[mofs], 4, tstr);
	}
	// 粗利率
	if( (m_mkZVolume.apno==KOEKI_APNO) || (m_mkZVolume.apno==KOEKI3_APNO) || (m_mkZVolume.apno==SYAKAI_APNO) || (m_mkZVolume.apno==SYUKYO_APNO) ){
		tstr += _T("0,");
	}
	else{		// 営利法人
		ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &m_nMNdt2[mofs], 4, tstr);
	}
	// 月次転送済みsno（データ送信）
	tstr += _T("0,");
	// 仕訳合計
	tstr += _T("0,");
	// 貸借合計
	tstr += _T("0,");
	// 諸口合計１
	tstr += _T("0,");
	// 諸口合計２
	tstr += _T("0,");
	// イメージ摘要使用サイン
	tstr += _T("0,");
	// テーブルエンドサイン
	tstr += _T("0,");
	// 決算修正へのオフセット
	int ksgn = 0, kofs = 0;
	if( m_KIsgn[0][mofs] != 0xff ){
		//	if ( 1 ){
		int ofs1 = -1, ofs2 = -1, ofs3 = -1, ofs4 = -1;
		int jofs1 = -1, jofs2 = -1, jofs3 = -1, jofs4 = -1;
		for( int i=0; i<MONTHALL; i++ ){
			switch( m_KIsgn[0][i]&0xF ){
			case 0x1:
				ofs1 = i;
				//					jofs1 = (m_KJsgn[0][i]&0x1) ? (i-1) : i;
				//					jofs1 = (i-1);
				jofs1 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
				break;
			case 0x2:
				ofs2 = i;
				//					jofs2 = (m_KJsgn[0][i]&0x1) ? (i-1) : i;
				//					jofs2 = (i-1);
				jofs2 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
				break;
			case 0x3:
				ofs3 = i;
				//					jofs3 = (m_KJsgn[0][i]&0x1) ? (i-1) : i;
				//					jofs3 = (i-1);
				jofs3 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
				break;
			case 0x4:
				//					ofs4 = jofs4 = i;
				ofs4 = i;
				//					jofs4 = (i-1);
				jofs4 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
				break;
			default:
				break;
			}
		}
		if( (ofs1!=-1) && (mofs<ofs1) && (m_nKsgn<1) ){
			kofs = jofs1;
		}
		else if( (ofs1!=-1) && (mofs==ofs1) && (m_nKsgn<1) ){
			kofs = ofs1;
		}
		else if( (ofs2!=-1) && (mofs<ofs2) && (m_nKsgn<2) ){
			kofs = jofs2;
		}
		else if( (ofs2!=-1) && (mofs==ofs2) && (m_nKsgn<2) ){
			kofs = ofs2;
		}
		else if( (ofs3!=-1) && (mofs<ofs3) && (m_nKsgn<1) ){
			kofs = jofs3;
		}
		else if( (ofs3!=-1) && (mofs==ofs3) && (m_nKsgn<1) ){
			kofs = ofs3;
		}
		else if( (ofs4!=-1) && (mofs<=jofs4) && (m_nKsgn==3) ){
			kofs = jofs4;
		}
		else if( (ofs4!=-1) && (mofs<ofs4) && (m_nKsgn<3) ){
			kofs = jofs4;
		}
		else if( (ofs4!=-1) && (mofs<=ofs4) && (m_nKsgn<3) ){
			kofs = ofs4;
		}
		else{
			kofs = 0;
		}
		/*		if ( (ofs1 != -1) && (mofs < ofs1) && (m_nKsgn < 1) ) {
					kofs = jofs1;
				}
				else if ( (ofs1 != -1) && (mofs == ofs1) && (m_nKsgn < 1) ) {
					kofs = ofs1;
				}
				else if ( (ofs2 != -1) && (mofs < ofs2) && (m_nKsgn < 2) ) {
					kofs = jofs2;
				}
				else if ( (ofs2 != -1) && (mofs == ofs2) && (m_nKsgn < 2) ) {
					kofs = ofs2;
				}
				else if ( (ofs3 != -1) && (mofs < ofs3) && (m_nKsgn < 1)  ) {
					kofs = jofs3;
				}
				else if ( (ofs3 != -1) && (mofs == ofs3) && (m_nKsgn < 1)  ) {
					kofs = ofs3;
				}
				else if ( (ofs4 != -1) && (mofs <= ofs4) && (m_nKsgn < 3 ) ) {
					kofs = ofs4;
				}
				else if ( (ofs4 != -1) && (mofs <= (ofs4-1)) && (m_nKsgn == 3) ) {
					kofs = (ofs4-1);
				}
				else {
					kofs = 0;
				}*/

				// 各月一括税抜き時
		if( ((m_mkZVolume.s_sgn5&0x0F)==0x01) && (mofs<=ofs4) ){
			kofs = mofs;
		}
	}
	else if( (mofs<=AM1_OFFSET) && (m_nKsgn<1) ){
		kofs = AM1_OFFSET;
	}
	else if( (mofs<=IAM_OFFSET) && (m_nKsgn<2) ){
		kofs = IAM_OFFSET;
	}
	else if( (mofs<=AM3_OFFSET) && (m_nKsgn<1) ){
		kofs = AM3_OFFSET;
	}
	else if( (mofs<=TAM_OFFSET) && (m_nKsgn<3) ){
		kofs = TAM_OFFSET;
	}
	else if( (mofs<=TAM2_OFFSET) && (m_nKsgn==3) ){
		kofs = TAM2_OFFSET;
	}

	ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &kofs, 1, tstr);
	// 決算修正サイン
	// 四半期決算対応 (05/08/24)
/*	switch ( mofs ) {
		case AM1_OFFSET:	ksgn = 1;		break;
		case IAM_OFFSET:	ksgn = 2;		break;
		case AM3_OFFSET:	ksgn = 3;		break;
		case TAM_OFFSET:	ksgn = 4;		break;
		default:			ksgn = 0;		break;
	}*/
	if( m_KIsgn[0][mofs] != 0xff ) {
		ksgn = (m_KIsgn[0][mofs]&0xF);
	}
	ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &ksgn, 1, tstr);

	// 中間決算集計　除外サイン 1 = 除外
//	tstr += _T("0,");
	BYTE kjsgn = 0;
	if( (m_KJsgn[0][mofs]&0x1) && (m_KIsgn[0][mofs]&0x20) ){
		kjsgn = 1;
	}
	ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &kjsgn, 1, tstr);
	// 消費税集計サイン
	tstr += _T("0,");
	// 部門集計サイン
	tstr += _T("0,");
	// 一括税抜き　作成サイン
	tstr += _T("0,");
	// 日時
	tstr += _T("NULL,");
	// 日時
	tstr += _T("NULL");

	// ダミーデータ追加
	int clm = GetColumnCount(_T("month_info"), pDbSvrName );
	if( clm != CLMMAX_MINFO ){
		int dmy = 0;
		for( int i=0; i<(clm - CLMMAX_MINFO); i++ ){
			tstr += _T(",");
			ConvertToSqldata(ID_ICSSYZ_NUMBER_SQLTYPE, &dmy, 1, tstr, FALSE);
		}
	}

	memmove(pbuf, tstr, tstr.GetLength());

	return 0;
}

//-----------------------------------------------------------------------------
// 月別情報データ用SQLの作成
//-----------------------------------------------------------------------------
// 引数	mofs		：	月オフセット
//		pbuf		：	SQLデータの作成先
//		mkcoinfo	：	作成先の情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CSyzMakeSubW::MakeMonthInfoDataSql( int mofs, char *pbuf, SYZMAKE_MKCOINFO &mkcoinfo )
{

	return MakeMonthInfoDataSql( mofs, pbuf, mkcoinfo.dbSvrName );

#ifdef _20200828_CUT_
	CString str, tstr;

	// 月オフセット
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &mofs, 1, tstr );
	// 入力許可サイン
	if( (m_mkZVolume.m_type&0xff) == 0x01 ){
		tstr += _T("0,");
	}
	else{
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_nInsw[mofs], 1, tstr );
	}
	// 月次サイン
	tstr += _T("0,");
	// 開始年月日
	JCYMD	JcYmd={0};
//	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, m_MSymd[mofs], 0, tstr );
	CnvToJcymd( IC_ICSSYZ_START_JCYTYPE, mofs, JcYmd );
	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, &JcYmd, 0, tstr );
	// 終了年月日
//	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, m_MEymd[mofs], 0, tstr );
	CnvToJcymd( IC_ICSSYZ_END_JCYTYPE, mofs, JcYmd );
	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, &JcYmd, 0, tstr );
	// 月次処理状態
	tstr += _T("0,");
	// 集計サイン
	tstr += _T("0,");
	// 資金収支集計（学校）
	tstr += _T("0,");
	// 入力仕訳数
	tstr += _T("0,");
	// 従業員数
	if( (m_mkZVolume.apno==KOEKI_APNO) || (m_mkZVolume.apno==KOEKI3_APNO) || (m_mkZVolume.apno==SYAKAI_APNO) || (m_mkZVolume.apno==SYUKYO_APNO) ){
		tstr += _T("0,");
	}
	else{		// 営利法人
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_nMNdt1[mofs], 4, tstr );
	}
	// 粗利率
	if( (m_mkZVolume.apno==KOEKI_APNO) || (m_mkZVolume.apno==KOEKI3_APNO) || (m_mkZVolume.apno==SYAKAI_APNO) || (m_mkZVolume.apno==SYUKYO_APNO) ){
		tstr += _T("0,");
	}
	else{		// 営利法人
		ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_nMNdt2[mofs], 4, tstr );
	}
	// 月次転送済みsno（データ送信）
	tstr += _T("0,");
	// 仕訳合計
	tstr += _T("0,");
	// 貸借合計
	tstr += _T("0,");
	// 諸口合計１
	tstr += _T("0,");
	// 諸口合計２
	tstr += _T("0,");
	// イメージ摘要使用サイン
	tstr += _T("0,");
	// テーブルエンドサイン
	tstr += _T("0,");
	// 決算修正へのオフセット
	int ksgn = 0, kofs = 0;
	if ( m_KIsgn[0][mofs] != 0xff ){
//	if ( 1 ){
		int ofs1 = -1, ofs2 = -1, ofs3 = -1, ofs4 = -1;
		int jofs1 = -1, jofs2 = -1, jofs3 = -1, jofs4 = -1;
		for( int i=0; i<MONTHALL; i++ ){
			switch( m_KIsgn[0][i]&0xF ){
				case 0x1:
					ofs1 = i;
//					jofs1 = (m_KJsgn[0][i]&0x1) ? (i-1) : i;
//					jofs1 = (i-1);
					jofs1 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
					break;
				case 0x2:
					ofs2 = i;
//					jofs2 = (m_KJsgn[0][i]&0x1) ? (i-1) : i;
//					jofs2 = (i-1);
					jofs2 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
					break;
				case 0x3:
					ofs3 = i;
//					jofs3 = (m_KJsgn[0][i]&0x1) ? (i-1) : i;
//					jofs3 = (i-1);
					jofs3 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
					break;
				case 0x4:
//					ofs4 = jofs4 = i;
					ofs4 = i;
//					jofs4 = (i-1);
					jofs4 = (((m_mkZVolume.s_sgn5&0xF) == 2) && (m_nKsgn != 3)) ? i : (i-1);
					break;
				default:
					break;
			}
		}
		if( (ofs1!=-1) && (mofs<ofs1) && (m_nKsgn<1) ){
			kofs = jofs1;
		}
		else if( (ofs1!=-1) && (mofs==ofs1) && (m_nKsgn<1) ){
			kofs = ofs1;
		}
		else if( (ofs2!=-1) && (mofs<ofs2) && (m_nKsgn<2) ){
			kofs = jofs2;
		}
		else if( (ofs2!=-1) && (mofs==ofs2) && (m_nKsgn<2) ){
			kofs = ofs2;
		}
		else if( (ofs3!=-1) && (mofs<ofs3) && (m_nKsgn<1) ){
			kofs = jofs3;
		}
		else if( (ofs3!=-1) && (mofs==ofs3) && (m_nKsgn<1) ){
			kofs = ofs3;
		}
		else if( (ofs4!=-1) && (mofs<=jofs4) && (m_nKsgn==3) ){
			kofs = jofs4;
		}
		else if( (ofs4!=-1) && (mofs<ofs4) && (m_nKsgn<3) ){
			kofs = jofs4;
		}
		else if( (ofs4!=-1) && (mofs<=ofs4) && (m_nKsgn<3) ){
			kofs = ofs4;
		}
		else{
			kofs = 0;
		}
/*		if ( (ofs1 != -1) && (mofs < ofs1) && (m_nKsgn < 1) ) {
			kofs = jofs1;
		}
		else if ( (ofs1 != -1) && (mofs == ofs1) && (m_nKsgn < 1) ) {
			kofs = ofs1;
		}
		else if ( (ofs2 != -1) && (mofs < ofs2) && (m_nKsgn < 2) ) {
			kofs = jofs2;
		}
		else if ( (ofs2 != -1) && (mofs == ofs2) && (m_nKsgn < 2) ) {
			kofs = ofs2;
		}
		else if ( (ofs3 != -1) && (mofs < ofs3) && (m_nKsgn < 1)  ) {
			kofs = jofs3;
		}
		else if ( (ofs3 != -1) && (mofs == ofs3) && (m_nKsgn < 1)  ) {
			kofs = ofs3;
		}
		else if ( (ofs4 != -1) && (mofs <= ofs4) && (m_nKsgn < 3 ) ) {
			kofs = ofs4;
		}
		else if ( (ofs4 != -1) && (mofs <= (ofs4-1)) && (m_nKsgn == 3) ) {
			kofs = (ofs4-1);
		}
		else {
			kofs = 0;
		}*/

		// 各月一括税抜き時
		if( ((m_mkZVolume.s_sgn5&0x0F)==0x01) && (mofs<=ofs4) ){
			kofs = mofs;
		}
	}
	else if( (mofs<=AM1_OFFSET) && (m_nKsgn<1) ){
		kofs = AM1_OFFSET;
	}
	else if( (mofs<=IAM_OFFSET) && (m_nKsgn<2) ){
		kofs = IAM_OFFSET;
	}
	else if( (mofs<=AM3_OFFSET) && (m_nKsgn<1) ){
		kofs = AM3_OFFSET;
	}
	else if( (mofs<=TAM_OFFSET) && (m_nKsgn<3) ){
		kofs = TAM_OFFSET;
	}
	else if( (mofs<=TAM2_OFFSET) && (m_nKsgn==3) ){
		kofs = TAM2_OFFSET;
	}

	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &kofs, 1, tstr );
	// 決算修正サイン
	// 四半期決算対応 (05/08/24)
/*	switch ( mofs ) {
		case AM1_OFFSET:	ksgn = 1;		break;
		case IAM_OFFSET:	ksgn = 2;		break;
		case AM3_OFFSET:	ksgn = 3;		break;
		case TAM_OFFSET:	ksgn = 4;		break;
		default:			ksgn = 0;		break;
	}*/
	if ( m_KIsgn[0][mofs] != 0xff ) {
		ksgn = (m_KIsgn[0][mofs]&0xF);
	}
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &ksgn, 1, tstr );

	// 中間決算集計　除外サイン 1 = 除外
//	tstr += _T("0,");
	BYTE kjsgn = 0;
	if( (m_KJsgn[0][mofs]&0x1) && (m_KIsgn[0][mofs]&0x20) ){
		kjsgn = 1;
	}
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &kjsgn, 1, tstr );
	// 消費税集計サイン
	tstr += _T("0,");
	// 部門集計サイン
	tstr += _T("0,");
	// 一括税抜き　作成サイン
	tstr += _T("0,");
	// 日時
	tstr += _T("NULL,");
	// 日時
	tstr += _T("NULL");

	// ダミーデータ追加
	int clm = GetColumnCount( _T("month_info"), mkcoinfo.dbSvrName );
	if( clm != CLMMAX_MINFO ){
		int dmy = 0;
		for( int i=0; i<(clm - CLMMAX_MINFO); i++ ){
			tstr += _T(",");
			ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &dmy, 1, tstr, FALSE );
		}
	}

	memmove( pbuf, tstr, tstr.GetLength() );

	return 0;
#endif
}

//-----------------------------------------------------------------------------
// 前期月別情報テーブルの作成
//-----------------------------------------------------------------------------
// 引数	kofs		：	期オフセット
//		mofs		：	月オフセット
//		pbuf		：	データ
//		mkcoinfo	：	作成先の情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::MakeZMonthInfoDataSql( int kofs, int mofs, char* pbuf, SYZMAKE_MKCOINFO &mkcoinfo )
{
	CString str, tstr;

	// 期オフセット
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &kofs, 1, tstr );
	// 月オフセット
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &mofs, 1, tstr );
	// 情報登録フラグ
	switch( mofs ){
		case AM1_OFFSET:		// 第１四半
			if( m_nZKsgn[kofs] < 1 ){
				tstr += _T("1,");
			}
			else{
				tstr += _T("0,");
			}
			break;
		case IAM_OFFSET:		// 中間決修
			if( m_nZKsgn[kofs] < 2 ){
				tstr += _T("1,");
			}
			else{
				tstr += _T("0,");
			}
			break;
		case AM3_OFFSET:		// 第３四半
			if( m_nZKsgn[kofs] < 1 ){
				tstr += _T("1,");
			}
			else{
				tstr += _T("0,");
			}
			break;
		case TAM_OFFSET:		// 期末決修
			if( m_nZKsgn[kofs] < 3 ){
				tstr += _T("1,");
			}
			else{
				tstr += _T("0,");
			}
			break;
		default:
			tstr += _T("1,");
			break;
	}

	// 自動棚卸フラグ(reserve)
	tstr += _T("0,");
//	BYTE flg = ((m_nSflg>>((kofs+1)*4))&0x0F);
//	ConvertToSqldata( 0, &flg, 1, tstr );
	// 従業員数
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_nMNdt1[mofs+((kofs+1)*MONTHALL)], 4, tstr );
	// 粗利率
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &m_nMNdt2[mofs+((kofs+1)*MONTHALL)], 4, tstr );

	// 開始年月日
	JCYMD	JcYmd={0};
//	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, m_MSymd[mofs+((kofs+1)*MONTHALL)], 0, tstr );
	CnvToJcymd( IC_ICSSYZ_START_JCYTYPE, mofs+((kofs+1)*MONTHALL), JcYmd );
	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, &JcYmd, 0, tstr );
	// 終了年月日
//	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, m_MEymd[mofs+((kofs+1)*MONTHALL)], 0, tstr );
	CnvToJcymd( IC_ICSSYZ_END_JCYTYPE, mofs+((kofs+1)*MONTHALL), JcYmd );
	ConvertToSqldata( ID_ICSSYZ_DATE_SQLTYPE, &JcYmd, 0, tstr );

	// 決算修正サイン 1 = 決算修正
	int ksgn = 0;
	// 四半期決算対応 (05/08/24)
	switch( mofs ){
		case AM1_OFFSET:
			ksgn = 1;
			break;
		case IAM_OFFSET:
			ksgn = 2;
			break;
		case AM3_OFFSET:
			ksgn = 3;
			break;
		case TAM_OFFSET:
			ksgn = 4;
			break;
	}
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &ksgn, 1, tstr );

	// 中間決算集計　除外サイン 1 = 除外
//	tstr += _T("0,");
	BYTE kjsgn = (m_KJsgn[kofs+1][mofs]&0x1);
	ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &kjsgn, 1, tstr );

	// 決修有効サイン
	switch( mofs ){
		case AM1_OFFSET:		// 第１四半
			if( m_nZKsgn[kofs] < 1 ){
				tstr += _T("1");
			}
			else{
				tstr += _T("0");
			}
			break;
		case IAM_OFFSET:		// 中間決修
			if( m_nZKsgn[kofs] < 2 ){
				tstr += _T("1");
			}
			else{
				tstr += _T("0");
			}
			break;
		case AM3_OFFSET:		// 第３四半
			if( m_nZKsgn[kofs] < 1 ){
				tstr += _T("1");
			}
			else{
				tstr += _T("0");
			}
			break;
		case TAM_OFFSET:		// 期末決修
			if( m_nZKsgn[kofs] < 3 ){
				tstr += _T("1");
			}
			else{
				tstr += _T("0");
			}
			break;
		default:
			tstr += _T("0");
			break;
	}

	int clm = GetColumnCount( _T("zmonth_info"), mkcoinfo.dbSvrName );
	if( clm != CLMMAX_ZMINFO ){
		int dmy = 0;
		for( int i=0; i<(clm - CLMMAX_ZMINFO); i++ ){
			tstr += ",";
			ConvertToSqldata( ID_ICSSYZ_NUMBER_SQLTYPE, &dmy, 1, tstr, FALSE );
		}
	}

	memmove( pbuf, tstr, tstr.GetLength() );

	return 0;
}

//-----------------------------------------------------------------------------
// データ変換
//-----------------------------------------------------------------------------
// 引数	type		：	変換タイプ（0:開始, 1:終了）
//		smofs		：	変換元データのオフセット
//		dstJcymd	：	変換先データ
//-----------------------------------------------------------------------------
void CSyzMakeSubW::CnvToJcymd( int type, int smofs, JCYMD& dstJcymd )
{
	if( (smofs<0) || (MONTHALL*5<=smofs) ){
		return;
	}

	memset( &dstJcymd, '\0', sizeof(dstJcymd) );

	if( type == IC_ICSSYZ_START_JCYTYPE ){
		dstJcymd.gengo = m_MSgengo[smofs];
		memmove( dstJcymd.ymd, m_MSymd[smofs], sizeof(dstJcymd.ymd) );
	}
	else if( type == IC_ICSSYZ_END_JCYTYPE ){
		dstJcymd.gengo = m_MEgengo[smofs];
		memmove( dstJcymd.ymd, m_MEymd[smofs], sizeof(dstJcymd.ymd) );
	}
}


