#include "StdAfx.h"
#include "SnHeadData.h"


void DbgViewTrace(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[1024];

	nBuf = vsprintf_s(szBuffer, sizeof szBuffer, lpszFormat, args);
	ASSERT(nBuf < sizeof(szBuffer));

	OutputDebugString(szBuffer);

	va_end(args);
}



extern	HINSTANCE shComInstance;

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const	int	MYNUM_KOJIN_HASH_SIZE	=	16;	// マイナンバー個人番号ハッシュ値のサイズ

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CSnHeadData::CSnHeadData()
{
	ClearReadData();

	m_Arith.l_defn( 0x16 );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CSnHeadData::~CSnHeadData()
{
}

//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{

	DbgViewTrace("GetDataの中");



	ASSERT( pDBNpSub );

	BOOL openFlg = FALSE;
	if( pDBNpSub->zvol == NULL ){
		if( pDBNpSub->VolumeOpen() ){
			return -1;
		}
		openFlg = TRUE;
	}
	EnumIdIcsShKazeihoushiki	kzhoushiki = ID_ICSSH_GENSOKU;	// '15.11.21
	m_apno = pDBNpSub->zvol->apno;
	if( sn_seq == 0 ){
		m_s_sgn2 = pDBNpSub->zvol->s_sgn2;
		m_s_sgn4 = pDBNpSub->zvol->s_sgn4;
		m_ss_ymd = pDBNpSub->zvol->ss_ymd;
		m_ee_ymd = pDBNpSub->zvol->ee_ymd;
//--> '15.11.21 INS START
		kzhoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
//<-- '15.11.21 INS END
	}

	// 場所移動
	// 改正バージョン ('19.02.27)
/*	long matsubi;
	Kaisei_VER = ID_VER_SYZ26;
	if (IsTyukanShinkoku())
		matsubi = pDBNpSub->szsnk->Sn_MDAYE;
	else
		matsubi = pDBNpSub->szsnk->Sn_KDAYE;
	if (matsubi >= 20191001)
		Kaisei_VER = ID_VER_SYZ31;
*/
	if( openFlg ){
		pDBNpSub->VolumeClose();
	}

	if( sn_seq ){
//		static	char	*GET_SHININFO_SQL	=	_T("IF OBJECTPROPERTY ( object_id('ShinInfo'),'ISTABLE') IS NOT NULL BEGIN SELECT S_SGN2,S_SGN4 FROM shininfo WHERE seq = %d END");
//		CString	sql;
//		sql.Format( GET_SHININFO_SQL, sn_seq );
		static	char	*GET_SHININFO_SQL	=	_T("SELECT S_SGN2,S_SGN4, SVmzsw FROM shininfo WHERE seq = %d");
		CString	sql;
		sql.Format( GET_SHININFO_SQL, sn_seq );

		m_s_sgn2 = 0;
		m_s_sgn4 = 0;

		CRecordset dbrec(pDBNpSub->m_database);
		try {
			CString	str_S_SGN2, str_S_SGN4;
			dbrec.Open( CRecordset::forwardOnly, sql );
			while ( dbrec.IsEOF() == FALSE ){
				dbrec.GetFieldValue( "S_SGN2", str_S_SGN2 );
				dbrec.GetFieldValue( "S_SGN4", str_S_SGN4 );

				dbrec.MoveNext();
			}
			m_s_sgn2 = atoi( str_S_SGN2 );
			m_s_sgn4 = atoi( str_S_SGN4 );
		}
		catch( CDBException* dbe ){
			dbe->Delete();
		}
		dbrec.Close();
	}

	// szvol 関連
	if( GetDataSzvolSub(pDBNpSub, sn_seq) ){
		return -1;
	}

	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );

	if( pDBNpSub->szsnk ){
		pDBNpSub->szsnk->Requery( filter );
	}
	else{
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
	}
	if( pDBNpSub->szsnk->st == -1 ){
		// 新規
		return 0;
	}

	CSZSNK *pSZSNK = pDBNpSub->szsnk;

	// データ読込み
	Sn_VER		= pSZSNK->Sn_VER;												// ファイルバージョン
	Sn_SKKBN	= pSZSNK->Sn_SKKBN;												// 申告区分
	Sn_KOJIN	= pSZSNK->Sn_KOJIN;												// 個人以外の事業者　ON:12ヶ月按分
	Sn_KOANB	= pSZSNK->Sn_KOANB;												// 個人事業者　ON:月按分　OFF:12ヶ月按分	
	m_Util.asci_to_bcd( Sn_ITIREN, &pSZSNK->Sn_ITIREN, 8 );						// 一連番号
	m_Util.asci_to_bcd( Sn_SEIRI, &pSZSNK->Sn_SEIRI, 8 );						// 整理番号

	unsigned char	Work[20];
	memset( Work, '\0', sizeof( Work ) );
	&pSZSNK->Sn_ZSEIRI.TrimRight();												// スペースカット
	m_Util.cstring_to_char( Work, pSZSNK->Sn_ZSEIRI, 8 );						// 納税署整理番号
	m_Arith.l_pack( Sn_ZSEIRI, Work, 8 );
	Sn_KDAYS = pSZSNK->Sn_KDAYS;												// 課税期間　自
	Sn_KDAYE = pSZSNK->Sn_KDAYE;												// 課税期間　至
	Sn_MDAYS = pSZSNK->Sn_MDAYS;												// 中間期間　自
	Sn_MDAYE = pSZSNK->Sn_MDAYE;												// 中間期間　至
	Sn_PRDAY = pSZSNK->Sn_PRDAY;												// 提出年月日

	Sn_SPECIAL	= pSZSNK->Sn_SPECIAL;											// 特殊処理サイン
	m_Util.cstring_to_char( Sn_ZMSYO, pSZSNK->Sn_ZMSYO, 12 );					// 税務署名
	m_Util.cstring_to_char( Sn_ZIP, pSZSNK->Sn_ZIP, 8 );						// 郵便番号
	m_Util.cstring_to_char( Sn_SYOKN, pSZSNK->Sn_SYOKN, 3 );					// 所管

	char	telbf[64] = {0};
	memset( telbf, '\0', sizeof( telbf ) );
	if( pSZSNK->Sn_TELNO.GetLength() == 0 ){
		pSZSNK->Sn_TELNO = _T ("                " );
	}
	m_Util.cstring_to_char( (unsigned char *)telbf, pSZSNK->Sn_TELNO, 16 );		// 電話番号
	m_Util.cstring_to_char( Sn_TELNO1, telbf, 6 );								// 電話番号　市外
	m_Util.cstring_to_char( Sn_TELNO2, &telbf[7], 4 );							// 電話番号　市内
	m_Util.cstring_to_char( Sn_TELNO3, &telbf[12], 4 );							// 電話番号　個人

	m_Util.cstring_to_char( Sn_NOZEI, pSZSNK->Sn_NOZEI, 80 );					// 納税地

	if( kjlen( Sn_NOZEI, 80 ) == 0 ){
		memset( Sn_NOZEI, '\0', 80 );
	}

	m_Util.cstring_to_char( Sn_CONAM, pSZSNK->Sn_CONAM, 40 );					// 氏名又は屋号
	m_Util.cstring_to_char( Sn_COFRI, pSZSNK->Sn_COFRI, 80 );					// 氏名又は屋号フリガナ
	m_Util.cstring_to_char( Sn_DIHYO, pSZSNK->Sn_DIHYO, 40 );					// 代表者氏名
	m_Util.cstring_to_char( Sn_DIFRI, pSZSNK->Sn_DIFRI, 80 );					// 代表者氏名フリガナ
	m_Util.cstring_to_char( Sn_KEIRI, pSZSNK->Sn_KEIRI, 40 );					// 経理担当者氏名

	m_Util.cstring_to_char( Sn_GPNAM, pSZSNK->Sn_ADRD4[3], 40 );				// 被合併名
	SubInf		= strtol( pSZSNK->Sn_ADRD4[2], 0, 16 );							// 税理士情報
	Sn_KAPPU	= pSZSNK->Sn_KAPPU;												// 割賦基準の適用
	Sn_NOBE		= pSZSNK->Sn_NOBE;												// 延払基準の適用
	Sn_KOJI		= pSZSNK->Sn_KOJI;												// 工事進行基準の適用
	Sn_GENKN	= pSZSNK->Sn_GENKN;												// 現金主義会計の適用
	Sn_TUMIAG	= pSZSNK->Sn_TUMIAG;											// 規則22条1項の適用
	Sn_KZTNSK	= pSZSNK->Sn_KZTNSK;											// 課税期間短縮
	Sn_MCOUNT	= pSZSNK->Sn_MCOUNT;											// 中間期間　年回数
	Sn_JCOUNT	= pSZSNK->Sn_JCOUNT;											// 中間申告　現在申告状態
	Sn_TOKUR	= pSZSNK->Sn_TOKUR;												// 特例計算適用サイン
	Sn_UPERS	= pSZSNK->Sn_UPERS;												// 課税売上割合90%以上のサイン
	Sn_TYPE		= pSZSNK->Sn_TYPE;												// 施行署及び施行署以外のサイン
	Sn_BYsw		= pSZSNK->Sn_BYsw;												// 金融機関選択サイン

	m_Util.cstring_to_char( Sn_BANK, pSZSNK->Sn_BANK, 20 );						// 銀行名称
	m_Util.cstring_to_char( Sn_SITEN, pSZSNK->Sn_SITEN, 20 );					// 支店名称
	m_Util.cstring_to_char( Sn_YOKIN, pSZSNK->Sn_YOKIN, 8 );					// 預金種別
	m_Util.cstring_to_char( Sn_YUBIN, pSZSNK->Sn_YUBIN, 16 );					// 郵便局名
	m_Util.asci_to_bcd( Sn_KOZA, &pSZSNK->Sn_KOZA, 10 );						// 口座番号
	m_Util.cstring_to_char( Sn_YUNUM1, pSZSNK->Sn_YUNUM1, 5 );					// 郵便貯金番号①
	m_Util.cstring_to_char( Sn_YUNUM2, pSZSNK->Sn_YUNUM2, 8 );					// 郵便貯金番号②
	Sn_FINANCE	= pSZSNK->Sn_FINANCE;											// 銀行、金庫等
	Sn_CHAIN	= pSZSNK->Sn_CHAIN;												// 本店、支店等

	Sn_ZeiHou	= pSZSNK->Sn_ZeiHou;											// 税理士法サイン
/*- '14.05.13 -*/
//	if( m_ss_ymd >= ICS_SH_8PER_REFORM_DAY ){
/*-------------*/
	// 期末日が越えているものは絶えず還付申告に！
	if( m_ee_ymd >= ICS_SH_8PER_REFORM_DAY ){
/*-------------*/
		Sn_ZeiHou |= 0x80;
	}
	Sn_Syuusei	= pSZSNK->Sn_Syuusei;											// 修正申告サイン
	Sn_Sign1	= pSZSNK->Sn_Sign1;												// 新設サイン①
	Sn_Sign2	= pSZSNK->Sn_Sign2;												// 新設サイン②
	Sn_Sign3	= pSZSNK->Sn_Sign3;												// 新設サイン③
	Sn_Sign4	= pSZSNK->Sn_Sign4;												// 新設サイン④
	m_Util.cstring_to_char( Sn_TAXNO,(LPCTSTR)pSZSNK->Sn_KSNO, 5 );				// 税務署コード
	Sn_EXP100	= pSZSNK->Sn_EXP100;											// 課税売上高100％

	// 中間申告
	for( int i=0; i<12; i++ ){
		m_Util.val_to_bin( Sn_TYNF[i], pSZSNK->Sn_TYNF[i] );
		m_Util.val_to_bin( Sn_TTNF[i], pSZSNK->Sn_TTYNF[i] );
		// 年３回
		switch( i ) {
			case 0 :
				m_Util.val_to_bin( Sn_TYNF1, pSZSNK->Sn_TYNF[i] );	
				m_Util.val_to_bin( Sn_TTYNF1, pSZSNK->Sn_TTYNF[i] );
				break;
			case 1 :
				m_Util.val_to_bin( Sn_TYNF2, pSZSNK->Sn_TYNF[i] );	
				m_Util.val_to_bin( Sn_TTYNF2, pSZSNK->Sn_TTYNF[i] );
				break;
			case 2 :
				m_Util.val_to_bin( Sn_TYNF3, pSZSNK->Sn_TYNF[i] );	
				m_Util.val_to_bin( Sn_TTYNF3, pSZSNK->Sn_TTYNF[i] );
				break;
			case 3 :	
				m_Util.val_to_bin( Sn_TYNF4, pSZSNK->Sn_TYNF[i] );	
				m_Util.val_to_bin( Sn_TTYNF4, pSZSNK->Sn_TTYNF[i] );
				break;
			default:
				break;
		}
	}

	// 自動車税
	m_Util.val_to_bin( Sn_CK45, pSZSNK->Sn_CK45 );							// 自動車　課税標準額　４．５％
	m_Util.val_to_bin( Sn_CZ45, pSZSNK->Sn_CZ45 );							// 自動車　消費税額　　　〃
	m_Util.val_to_bin( Sn_CH45, pSZSNK->Sn_CH45 );							// 自動車　対価の返還額　〃
	m_Util.val_to_bin( Sn_CK60, pSZSNK->Sn_CK60 );							// 自動車　課税標準額　６．０％
	m_Util.val_to_bin( Sn_CZ60, pSZSNK->Sn_CZ60 );							// 自動車　消費税額　　　〃
	m_Util.val_to_bin( Sn_CH60, pSZSNK->Sn_CH60 );							// 自動車　対価の返還額　〃

	m_Util.val_to_bin( Sn_KURIA, pSZSNK->Sn_KURIA );						// 事業別課税売上高合計額

	// 特殊処理
	if( m_Arith.l_test(Sn_CK45) || m_Arith.l_test(Sn_CZ45) || m_Arith.l_test(Sn_CH45) ){
		m_Arith.l_add( Sn_CK60, Sn_CK60, Sn_CK45 );
		m_Arith.l_add( Sn_CZ60, Sn_CZ60, Sn_CZ45 );
		m_Arith.l_add( Sn_CH60, Sn_CH60, Sn_CH45 );
		memset( Sn_CK45, '\0', sizeof(Sn_CK45) );
		memset( Sn_CZ45, '\0', sizeof(Sn_CZ45) );
		memset( Sn_CH45, '\0', sizeof(Sn_CH45) );
	}

	m_Util.val_to_bin( Sn_WURI, pSZSNK->Sn_WURI );						// 課税売上高（ワーク）

	// 課税売上割合の閾値 の判断が必要！

	//[Y]欄外印字設定情報セット
	m_Util.cstring_to_char( Sn_ADRU1, pSZSNK->Sn_ADRU1, 30 );		// 上段①　左
	m_Util.cstring_to_char( Sn_ADRU2, pSZSNK->Sn_ADRU2, 20 );		// 上段①　右
	m_Util.cstring_to_char( Sn_ADRC1, pSZSNK->Sn_ADRC1, 20 );		// 上段②　左  
	m_Util.cstring_to_char( Sn_ADRC2, pSZSNK->Sn_ADRC2, 20 );		// 上段②　右  
	m_Util.cstring_to_char( Sn_ADRD1, pSZSNK->Sn_ADRD1, 30 );		// 下段①　左  
	m_Util.cstring_to_char( Sn_ADRD2, pSZSNK->Sn_ADRD2, 16 );		// 下段②　右  
	m_Util.cstring_to_char( Sn_ADRD4, pSZSNK->Sn_ADRD4[0], 50 );	// 下段①　左  
	m_Util.cstring_to_char( Sn_ADSNO, pSZSNK->Sn_ADRD4[1], 10 );		// 下段②　右  

//--> '15.02.27 INS START
	SepStrToSign( 1, pSZSNK->Sn_ADRD4[4] );
//---> '15.11.21 INS START
	if( kzhoushiki != ID_ICSSH_GENSOKU ){
		m_DispTabSgn &= ~0x01;
	}
	else if( m_DispTabSgn&0x01 ){
		// 期間を見て、自動的にOFFにする
		BYTE	kbn = GetKubun();
		long	chkEymd=0;
		if( (kbn==ID_ICSSH_TYUUKAN) || (kbn==ID_ICSSH_TYUUKAN_SYUUSEI) ){
			chkEymd = Sn_MDAYE;
		}
		else{
			chkEymd = Sn_KDAYE;
		}
		if( chkEymd < ICS_SH_TOKUTEI_REFORM_DAY ){
			m_DispTabSgn &= ~0x01;
		}
	}
//<--- '15.11.21 INS END
//<-- '15.02.27 INS END

//--> '15.07.11 INS START
	// 法人番号
	memset( Sn_HOJIN_NO, '\0', sizeof(Sn_HOJIN_NO) );
	if( pSZSNK->Sn_HOJIN_NO.GetLength() ){
		wsprintf( Sn_HOJIN_NO, _T("%s"), pSZSNK->Sn_HOJIN_NO.GetBuffer() );
	}
	// 法人番号付番
	memset( Sn_HOJIN_NO_EXT, '\0', sizeof(Sn_HOJIN_NO_EXT) );
	if( pSZSNK->Sn_HOJIN_NO_EXT.GetLength() ){
		wsprintf( Sn_HOJIN_NO_EXT, _T("%s"), pSZSNK->Sn_HOJIN_NO_EXT.GetBuffer() );
	}
	// 個人番号ハッシュ
	memset( Sn_KOJIN_HASH, '\0', sizeof(Sn_KOJIN_HASH) );
	if( IsRightKjhash(pSZSNK->Sn_KOJIN_HASH) == TRUE ){
		int size = (int)pSZSNK->Sn_KOJIN_HASH.GetSize();
		if( size > sizeof(Sn_KOJIN_HASH) ){
			size = sizeof(Sn_KOJIN_HASH);
		}
		for( int i=0; i<size; i++ ){
			Sn_KOJIN_HASH[i] = pSZSNK->Sn_KOJIN_HASH.GetAt(i);
		}
	}
//<-- '15.07.11 INS END

//--> '15.12.15 INS START
	// 代表者住所
	m_Util.cstring_to_char( Sn_DIADR, pSZSNK->Sn_DIADR, 80 );
	// 代表者郵便番号
	m_Util.cstring_to_char( Sn_DIZIP, pSZSNK->Sn_ADRD4[5], 8 );
//<-- '15.12.15 INS END

//--> '15.12.23 INS START
	memset( telbf, '\0', sizeof( telbf ) );
	if( pSZSNK->Sn_ADRD4[6].GetLength() == 0 ){
		pSZSNK->Sn_ADRD4[6] = _T ("                " );
	}
	m_Util.cstring_to_char( (unsigned char *)telbf, pSZSNK->Sn_ADRD4[6], 16 );	// 電話番号
	m_Util.cstring_to_char( Sn_DITELNO1, telbf, 6 );							// 電話番号　市外
	m_Util.cstring_to_char( Sn_DITELNO2, &telbf[7], 4 );						// 電話番号　市内
	m_Util.cstring_to_char( Sn_DITELNO3, &telbf[12], 4 );						// 電話番号　個人
//<-- '15.12.23 INS END

	// 改正様式セット
	long matsubi;
	Kaisei_VER = ID_VER_SYZ26;
	if (IsTyukanShinkoku())
		matsubi = pDBNpSub->szsnk->Sn_MDAYE;
	else
		matsubi = pDBNpSub->szsnk->Sn_KDAYE;
	if (matsubi >= 20191001)
		Kaisei_VER = ID_VER_SYZ31;

	return 0;
}

//-----------------------------------------------------------------------------
// DBへデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	// szvol 関連
	if( SetDataSzvolSub(pDBNpSub, sn_seq) ){
		return -1;
	}

	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );

	if( pDBNpSub->szsnk ){
		pDBNpSub->szsnk->Requery( filter );
	}
	else{
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
	}

	char	telbf[64]={0}, tel1[10]={0}, tel2[10]={0}, tel3[10]={0};
	CString	tel;
	char	buf[512]={0};
//--> '15.09.19 INS START
	BOOL	IsNewData = FALSE;
//<-- '15.09.19 INS END

//--> '15.07.25 INS START
	BOOL	IsNeedClearKjHash = FALSE;
//<-- '15.07.25 INS END

	CSZSNK *pSZSNK = pDBNpSub->szsnk;
	if( pSZSNK->st == -1 ){
		// 新規作成
		pSZSNK->AddNew();
		// 新規作成時にデフォルト値は下のものだけでOK??
//--> '15.09.19 INS START
		IsNewData = TRUE;
//<-- '15.09.19 INS END
	}
	else{
		// 更新
		pSZSNK->Edit();
	}

	pSZSNK->Sn_SEQ = sn_seq;													// 履歴シーケンス
/*- '14.04.02 -*/
//	pSZSNK->Sn_VER = Sn_VER;
/*-------------*/
//-- '15.04.13 --
//	pSZSNK->Sn_VER = GetEselVersion(Sn_SKKBN, Sn_Syuusei, Sn_KDAYE, Sn_MDAYE);	// 電子申告バージョン
//---------------
	pSZSNK->Sn_VER = GetEselVersion(pDBNpSub, Sn_SKKBN, Sn_Syuusei, Sn_KDAYE, Sn_MDAYE, Sn_KDAYS, Sn_MDAYS);	// 電子申告バージョン
//---------------
/*-------------*/
	pSZSNK->Sn_SKKBN = Sn_SKKBN;												// 申告区分
	pSZSNK->Sn_KOJIN = Sn_KOJIN;												// 個人以外の事業者　ON:12ヶ月按分
	pSZSNK->Sn_KOANB = Sn_KOANB;												// 個人事業者　ON:月按分　OFF:12ヶ月按分	
	m_Util.bcd_to_asci( &pSZSNK->Sn_ITIREN, Sn_ITIREN, 8 );						// 一連番号
	m_Util.bcd_to_asci( &pSZSNK->Sn_SEIRI, Sn_SEIRI, 8 );						// 整理番号
	m_Util.bcd_to_asci( &pSZSNK->Sn_ZSEIRI, Sn_ZSEIRI, 8 );						// 納税署整理番号

	pSZSNK->Sn_KDAYS = Sn_KDAYS;												// 課税期間　自
	pSZSNK->Sn_KDAYE = Sn_KDAYE;												// 課税期間　至
	pSZSNK->Sn_MDAYS = Sn_MDAYS;												// 中間期間　自
	pSZSNK->Sn_MDAYE = Sn_MDAYE;												// 中間期間　至
	pSZSNK->Sn_PRDAY = Sn_PRDAY;												// 提出年月日
	pSZSNK->Sn_SPECIAL = Sn_SPECIAL;											// 特殊処理サイン
	m_Util.char_to_cstring( &pSZSNK->Sn_ZMSYO, Sn_ZMSYO, 12 );					// 税務署名
	m_Util.char_to_cstring( &pSZSNK->Sn_ZIP, Sn_ZIP, 8 );						// 郵便番号
	m_Util.char_to_cstring( &pSZSNK->Sn_SYOKN, Sn_SYOKN, 3 );					// 所管

	memset( telbf, 0x20, sizeof(telbf) );
	memmove( tel1, Sn_TELNO1, 6 );
	memmove( tel2, Sn_TELNO2, 4 );
	memmove( tel3, Sn_TELNO3, 4 );
	tel = tel1;
	memmove( telbf, tel, tel.GetLength() );
	tel = tel2;
	memmove( &telbf[7], tel, tel.GetLength() );
	tel = tel3;
	memmove( &telbf[12], tel, tel.GetLength() );
	m_Util.char_to_cstring( &pSZSNK->Sn_TELNO,(unsigned char *)telbf, 16 );		// 電話番号

	memset( buf, '\0', sizeof(buf) );
	memset( buf, 0x20, 40 );
	int kjln = kjlen( Sn_NOZEI, 40 );
	memmove( buf, Sn_NOZEI, kjln );
	kjln = kjlen( &Sn_NOZEI[40], 40 );
	memmove( &buf[40], &Sn_NOZEI[40], kjln );
	m_Util.char_to_cstring( &pSZSNK->Sn_NOZEI, (unsigned char *)buf, 80 );		// 納税地

	m_Util.char_to_cstring( &pSZSNK->Sn_CONAM, Sn_CONAM, sizeof(Sn_CONAM) );				// 氏名又は屋号
	m_Util.char_to_cstring( &pSZSNK->Sn_COFRI, Sn_COFRI, sizeof(Sn_COFRI) );				// 氏名又は屋号フリガナ
	m_Util.char_to_cstring( &pSZSNK->Sn_DIHYO, Sn_DIHYO, sizeof(Sn_DIHYO) );				// 代表者氏名
	m_Util.char_to_cstring( &pSZSNK->Sn_DIFRI, Sn_DIFRI, sizeof(Sn_DIFRI) );				// 代表者氏名フリガナ
	m_Util.char_to_cstring( &pSZSNK->Sn_KEIRI, Sn_KEIRI, sizeof(Sn_KEIRI) );				// 経理担当者氏名

//	Snh.Sn_UPERS &= 0xfd;
//	if( RATIO &0x02 )	Snh.Sn_UPERS |= 0x02;

	m_Util.char_to_cstring( &pSZSNK->Sn_ADRD4[3], (unsigned char *)Sn_GPNAM, 40 );			// 被合併名
	pSZSNK->Sn_ADRD4[2].Format( _T("%x"), SubInf );					// 税理士情報

	pSZSNK->Sn_KAPPU	=	Sn_KAPPU;								// 割賦基準の適用
	pSZSNK->Sn_NOBE		=	Sn_NOBE;								// 延払基準の適用
	pSZSNK->Sn_KOJI		=	Sn_KOJI;								// 工事進行基準の適用
	pSZSNK->Sn_GENKN	=	Sn_GENKN;								// 現金主義会計の適用
	pSZSNK->Sn_TUMIAG	=	Sn_TUMIAG;								// 規則22条1項の適用
	pSZSNK->Sn_KZTNSK	=	Sn_KZTNSK;								// 課税期間短縮
	pSZSNK->Sn_MCOUNT	=	Sn_MCOUNT;								// 中間期間　年回数
	pSZSNK->Sn_JCOUNT	=	Sn_JCOUNT;								// 中間申告　現在申告状態
	pSZSNK->Sn_TOKUR	=	Sn_TOKUR;								// 特例計算適用サイン
	pSZSNK->Sn_UPERS	=	Sn_UPERS;								// 課税売上割合90%以上のサイン
	pSZSNK->Sn_TYPE		=	Sn_TYPE;								// 施行署及び施行署以外のサイン
	pSZSNK->Sn_BYsw		=	Sn_BYsw;								// 金融機関選択サイン

	m_Util.char_to_cstring( &pSZSNK->Sn_BANK, Sn_BANK, 20 );		// 銀行名称
	m_Util.char_to_cstring( &pSZSNK->Sn_SITEN, Sn_SITEN,20 );		// 支店名称
	m_Util.char_to_cstring( &pSZSNK->Sn_YOKIN, Sn_YOKIN, 8 );		// 預金種別
	m_Util.char_to_cstring( &pSZSNK->Sn_YUBIN, Sn_YUBIN,16 );		// 郵便局名
	m_Util.bcd_to_asci( &pSZSNK->Sn_KOZA, Sn_KOZA, 10 );			// 口座番号
	m_Util.char_to_cstring( &pSZSNK->Sn_YUNUM1, Sn_YUNUM1, 5 );		// 郵便貯金番号①
	m_Util.char_to_cstring( &pSZSNK->Sn_YUNUM2, Sn_YUNUM2, 8 );		// 郵便貯金番号②

	pSZSNK->Sn_FINANCE	=	Sn_FINANCE;								// 銀行、金庫等
	pSZSNK->Sn_CHAIN	=	Sn_CHAIN;								// 本店、支店等

//	Snd.Sn_Sign2 &= 0xcf;
//	if( OPsign & 0x01 )	Snd.Sn_Sign2 |= 0x10;
//	if( Kk_SG & 0x10 )	Snd.Sn_Sign2 |= 0x20;

	pSZSNK->Sn_EXP100 = Sn_EXP100;									// 課税売上高１００％
	pSZSNK->Sn_ZeiHou = Sn_ZeiHou;									// 税理士法３０及び３３条の２
	pSZSNK->Sn_Syuusei = Sn_Syuusei;								// 修正（確定）を追加
	pSZSNK->Sn_Sign1 = Sn_Sign1;
	pSZSNK->Sn_Sign2 = Sn_Sign2;
	pSZSNK->Sn_Sign3 = Sn_Sign3;
	pSZSNK->Sn_Sign4 = Sn_Sign4;
	m_Util.char_to_cstring( &pSZSNK->Sn_KSNO, Sn_TAXNO, 5 );		// 税務署コード

//== TEST ==
//	CString	hoge;
//	hoge.Format( _T("内部：Sn_Seq:%d, Sn_Sign4:%d"), sn_seq, Sn_Sign4 );
//	AfxMessageBox( hoge );
//==========

	// 中間申告
	for( int i=0; i<12; i++ ){
		// 年１１回
		m_Util.val_to_asci( &pSZSNK->Sn_TYNF[i], Sn_TYNF[i] );
		m_Util.val_to_asci( &pSZSNK->Sn_TTYNF[i], Sn_TTNF[i] );
		// 年３回
		switch( i ){
			case 0 :
				m_Util.val_to_asci( &pSZSNK->Sn_TYNF[i], Sn_TYNF1 );	
				m_Util.val_to_asci( &pSZSNK->Sn_TTYNF[i], Sn_TTYNF1 );
				break;
			case 1 :
				m_Util.val_to_asci( &pSZSNK->Sn_TYNF[i], Sn_TYNF2 );	
				m_Util.val_to_asci( &pSZSNK->Sn_TTYNF[i], Sn_TTYNF2 );
				break;
			case 2 :
				m_Util.val_to_asci( &pSZSNK->Sn_TYNF[i], Sn_TYNF3 );	
				m_Util.val_to_asci( &pSZSNK->Sn_TTYNF[i], Sn_TTYNF3 );
				break;
			case 3 :
				m_Util.val_to_asci( &pSZSNK->Sn_TYNF[i], Sn_TYNF4 );
				m_Util.val_to_asci( &pSZSNK->Sn_TTYNF[i], Sn_TTYNF4 );
				break;
			default:
				break;
		}
	}

	// 自動車税
	m_Util.val_to_asci( &pSZSNK->Sn_CK45, Sn_CK45 );							// 自動車　課税標準額　４．５％
	m_Util.val_to_asci( &pSZSNK->Sn_CZ45, Sn_CZ45 );							// 自動車　消費税額　　　〃
	m_Util.val_to_asci( &pSZSNK->Sn_CH45, Sn_CH45 );							// 自動車　対価の返還額　〃
	m_Util.val_to_asci( &pSZSNK->Sn_CK60, Sn_CK60 );							// 自動車　課税標準額　６．０％
	m_Util.val_to_asci( &pSZSNK->Sn_CZ60, Sn_CZ60 );							// 自動車　消費税額　　　〃
	m_Util.val_to_asci( &pSZSNK->Sn_CH60, Sn_CH60 );							// 自動車　対価の返還額　〃

	m_Util.val_to_asci( &pSZSNK->Sn_KURIA, Sn_KURIA );							// 事業別課税売上高合計額

	m_Util.val_to_asci( &pSZSNK->Sn_WURI, Sn_WURI );							// 課税売上高（ワーク）

	//[Y]欄外印字設定情報保存
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRU1, Sn_ADRU1, 30 );		// 上段①　左
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRU2, Sn_ADRU2, 20 );		// 上段①　右  
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRC1, Sn_ADRC1, 20 );		// 上段②　左  
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRC2, Sn_ADRC2, 20 );		// 上段②　右  
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRD1, Sn_ADRD1, 30 );		// 下段①　左  
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRD2, Sn_ADRD2, 16 );		// 下段②　右  
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRD4[0], Sn_ADRD4, 50 );		// 下段①左の統一（下段①左＋下段①左の１４文字拡張分＋２桁） 
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRD4[1], Sn_ADSNO, 10 );		//  署別一連番号７桁（数字）

//--> '15.02.27 INS START
	BindSignToStr( 1, pSZSNK->Sn_ADRD4[4] );
//<-- '15.02.27 INS END

//--> '15.07.25 INS START
	// 法人番号
	pSZSNK->Sn_HOJIN_NO = Sn_HOJIN_NO;
	// 法人番号付番
	pSZSNK->Sn_HOJIN_NO_EXT = Sn_HOJIN_NO_EXT;
	
	// 個人番号ハッシュ
	if( IsRightKjhash() ){
		if( IsChangedKojinHash(pSZSNK->Sn_KOJIN_HASH) ){
			pSZSNK->Sn_KOJIN_HASH.RemoveAll();
			for( int i=0; i<MYNUM_KOJIN_HASH_SIZE; i++ ){
				pSZSNK->Sn_KOJIN_HASH.Add( Sn_KOJIN_HASH[i] );
			}
		}
	}
	else{
		if( IsNewData ){
			pSZSNK->Sn_KOJIN_HASH.SetSize( MYNUM_KOJIN_HASH_SIZE );
			IsNeedClearKjHash = TRUE;
		}
	}
//<-- '15.07.25 INS END

//--> '15.12.15 INS START
	// 代表者住所
	memset( buf, '\0', sizeof(buf) );
	memset( buf, 0x20, 40 );
	kjln = kjlen( Sn_DIADR, 40 );
	memmove( buf, Sn_DIADR, kjln );
	kjln = kjlen( &Sn_DIADR[40], 40 );
	memmove( &buf[40], &Sn_DIADR[40], kjln );
	m_Util.char_to_cstring( &pSZSNK->Sn_DIADR, (unsigned char *)buf, 80 );
	// 代表者郵便番号
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRD4[5], Sn_DIZIP, 8 );
//<-- '15.12.15 INS END

//--> '15.12.23 INS START
	// 代表者電話番号
	memset( telbf, 0x20, sizeof(telbf) );
	memmove( tel1, Sn_DITELNO1, 6 );
	memmove( tel2, Sn_DITELNO2, 4 );
	memmove( tel3, Sn_DITELNO3, 4 );
	tel = tel1;
	memmove( telbf, tel, tel.GetLength() );
	tel = tel2;
	memmove( &telbf[7], tel, tel.GetLength() );
	tel = tel3;
	memmove( &telbf[12], tel, tel.GetLength() );
	m_Util.char_to_cstring( &pSZSNK->Sn_ADRD4[6], (unsigned char *)telbf, 16 );
//<-- '15.12.23 INS END

	pSZSNK->Update();

//--> '15.09.19 INS START
	if( IsNewData ){
		pDBNpSub->SzsnkClose();
		pDBNpSub->SzsnkOpen(filter);
	}
//<-- '15.09.19 INS END

//--> '15.07.25 INS START
	if( IsNeedClearKjHash ){
//-- '15.09.14 --
//		ClearKjhashBySql( pDBNpSub, sn_seq );
//---------------
		ClearKjhashBySql( pDBNpSub, sn_seq, NULL );
//---------------
	}
//<-- '15.07.25 INS END

	return 0;
}

//-----------------------------------------------------------------------------
// DB からデータ読込み(szvol 関連)
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::GetDataSzvolSub( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	if( sn_seq == 0 ){	// 最新データは、szvol から取得

		BOOL openFlg = FALSE;
		if( pDBNpSub->szvol == NULL ){
			if( pDBNpSub->SyohizeiOpen() ){
				return -1;
			}
			openFlg = TRUE;
		}

		SVvers = pDBNpSub->szvol->SVvers;
		SVnver = pDBNpSub->szvol->SVnver;
		SVsign = pDBNpSub->szvol->SVsign;
		SVmzsw = pDBNpSub->szvol->SVmzsw;
		SVsper[0] = pDBNpSub->szvol->SVsper1;
		SVsper[1] = pDBNpSub->szvol->SVsper2;
		SVsper[2] = pDBNpSub->szvol->SVsper3;
		SVsper[3] = pDBNpSub->szvol->SVsper4;
		SVsper[4] = pDBNpSub->szvol->SVsper5;
		SVsper[5] = pDBNpSub->szvol->SVsper6;	// '15.02.23
		if( SVvers == ICS_SH_H26_SZVOL_VER ){
			// 不正に消去されているマスターがある為の対処。
			if( SVsper[0] == 0 ){
				SVsper[0] = 100;
			}
			if( SVsper[1] == 0 ){
				SVsper[1] = 200;
			}
			if( SVsper[2] == 0 ){
				SVsper[2] = 300;
			}
			if( SVsper[3] == 0 ){
				SVsper[3] = 400;
			}
			if( SVsper[4] == 0 ){
				SVsper[4] = 500;
			}
//--> '15.02.23 INS START
			if( SVsper[5] == 0 ){
				SVsper[5] = 600;
			}
//<-- '15.02.23 INS END
		}
		SVsasw = pDBNpSub->szvol->SVsasw;
		SVhksw = pDBNpSub->szvol->SVhksw;
		SVd5sw = pDBNpSub->szvol->SVd5sw;
		SVkojn = pDBNpSub->szvol->SVkojn;
		SVzchg = pDBNpSub->szvol->SVzchg;
		SVkazei = pDBNpSub->szvol->SVkazei;
		SVsacq = pDBNpSub->szvol->SVsacq;
		SVzisq = pDBNpSub->szvol->SVzisq;

		// この値は読込みのみで戻さない！
		m_Util.val_to_bin( SVkan0, pDBNpSub->szvol->SVkan0 );
		m_Util.val_to_bin( SVkan1, pDBNpSub->szvol->SVkan1 );

		if( openFlg ){
			pDBNpSub->SyohizeiClose();
			openFlg = FALSE;
		}
	}
	else{

		BOOL openFlg = FALSE;
		if( pDBNpSub->szvol == NULL ){
			if( pDBNpSub->SyohizeiOpen() ){
				return -1;
			}
			openFlg = TRUE;
		}
		SVvers = pDBNpSub->szvol->SVvers;
		SVnver = pDBNpSub->szvol->SVnver;
		SVsign = pDBNpSub->szvol->SVsign;
		SVsper[0] = pDBNpSub->szvol->SVsper1;
		SVsper[1] = pDBNpSub->szvol->SVsper2;
		SVsper[2] = pDBNpSub->szvol->SVsper3;
		SVsper[3] = pDBNpSub->szvol->SVsper4;
		SVsper[4] = pDBNpSub->szvol->SVsper5;
		SVsper[5] = pDBNpSub->szvol->SVsper6;	// '15.02.23
		if( SVvers == ICS_SH_H26_SZVOL_VER ){
			// 不正に消去されているマスターがある為の対処。
			if( SVsper[0] == 0 ){
				SVsper[0] = 100;
			}
			if( SVsper[1] == 0 ){
				SVsper[1] = 200;
			}
			if( SVsper[2] == 0 ){
				SVsper[2] = 300;
			}
			if( SVsper[3] == 0 ){
				SVsper[3] = 400;
			}
			if( SVsper[4] == 0 ){
				SVsper[4] = 500;
			}
//--> '15.02.23 INS START
			if( SVsper[5] == 0 ){
				SVsper[5] = 600;
			}
//<-- '15.02.23 INS END
		}
		// この値は読込みのみで戻さない！
		m_Util.val_to_bin( SVkan0, pDBNpSub->szvol->SVkan0 );
		m_Util.val_to_bin( SVkan1, pDBNpSub->szvol->SVkan1 );

		if( openFlg ){
			pDBNpSub->SyohizeiClose();
			openFlg = FALSE;
		}

		// 履歴情報から取得
		static	char	*GET_SHININFO_SQL	=	_T("SELECT SVsasw, SVmzsw, SVhksw, SVd5sw, SVkojn, SVzchg, SVkazei, SVsacq, SVzisq FROM shininfo WHERE seq = %d");
		CString	sql;
		sql.Format( GET_SHININFO_SQL, sn_seq );

		SVsasw = 0;
		SVmzsw = 0;
		SVhksw = 0;
		SVd5sw = 0;

		SVkojn = 0;
		SVzchg = 0;
		SVkazei = 0;
		SVsacq = 0;
		SVzisq = 0;

		CRecordset dbrec(pDBNpSub->m_database);
		try {
			CString	str_SVsasw, str_SVmzsw, str_SVhksw, str_SVd5sw, str_SVkojn, str_SVzchg, str_SVkazei, str_SVsacq, str_SVzisq;
			dbrec.Open( CRecordset::forwardOnly, sql );
			while ( dbrec.IsEOF() == FALSE ){
				dbrec.GetFieldValue( "SVsasw", str_SVsasw );
				dbrec.GetFieldValue( "SVmzsw", str_SVmzsw );
				dbrec.GetFieldValue( "SVhksw", str_SVhksw );
				dbrec.GetFieldValue( "SVd5sw", str_SVd5sw );

				dbrec.GetFieldValue( "SVkojn", str_SVkojn );
				dbrec.GetFieldValue( "SVzchg", str_SVzchg );
				dbrec.GetFieldValue( "SVkazei", str_SVkazei );
				dbrec.GetFieldValue( "SVsacq", str_SVsacq );
				dbrec.GetFieldValue( "SVzisq", str_SVzisq );
				dbrec.MoveNext();
			}
			SVsasw = atoi( str_SVsasw );
			SVmzsw = atoi( str_SVmzsw );
			SVhksw = atoi( str_SVhksw );
			SVd5sw = atoi( str_SVd5sw );

			SVkojn = atoi( str_SVkojn );
			SVzchg = atoi( str_SVzchg );
			SVkazei = atoi( str_SVkazei );
			SVsacq = atoi( str_SVsacq );
			SVzisq = atoi( str_SVzisq );
		}
		catch( CDBException* dbe ){
			dbe->Delete();
		}
		dbrec.Close();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DB へデータ書込み(szvol 関連)
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::SetDataSzvolSub( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	if( sn_seq == 0 ){	// 最新データは、szvol に保存

		BOOL openFlg = FALSE;
		if( pDBNpSub->szvol == NULL ){
			if( pDBNpSub->SyohizeiOpen() ){
				return -1;
			}
			openFlg = TRUE;
		}

		pDBNpSub->szvol->Edit();
/*- '14.04.02 -*/
// ここでは、バージョンは書き換えない!
//		pDBNpSub->szvol->SVvers = SVvers;
/*-------------*/
		pDBNpSub->szvol->SVnver = SVnver;
		pDBNpSub->szvol->SVsign = SVsign;
		pDBNpSub->szvol->SVmzsw = SVmzsw;
//		pDBNpSub->szvol->SVsper1 = SVsper[0];
//		pDBNpSub->szvol->SVsper2 = SVsper[1];
//		pDBNpSub->szvol->SVsper3 = SVsper[2];
//		pDBNpSub->szvol->SVsper4 = SVsper[3];
//		pDBNpSub->szvol->SVsper5 = SVsper[4];
		pDBNpSub->szvol->SVsasw = SVsasw;
		pDBNpSub->szvol->SVhksw = SVhksw;
		pDBNpSub->szvol->SVd5sw = SVd5sw;
		pDBNpSub->szvol->SVkojn  = SVkojn;
		pDBNpSub->szvol->SVzchg = SVzchg;
		pDBNpSub->szvol->SVkazei = SVkazei;
		pDBNpSub->szvol->SVsacq = SVsacq;
		pDBNpSub->szvol->SVzisq = SVzisq;
		pDBNpSub->szvol->Update();

		if( openFlg ){
			pDBNpSub->SyohizeiClose();
			openFlg = FALSE;
		}
	}
	else{
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上割合に準ずる割合を使用？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	課税売上割合に準ずる割合を使用
//			FALSE	：	課税売上割合に準ずる割合を使用していない
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsUseSemiUriRatio()
{
	BOOL	bRt = FALSE;

	if( Sn_Sign4 & 0x80 ){
		if( Sn_UPERS & 0x02 ){
			bRt = TRUE;
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 仕入税額の按分方法が個別対応？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	仕入税額按分法法が個別対応
//			FALSE	：	仕入税額按分法法が比例配分
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsKobetuSiireAnbun()
{
	BOOL	bRt = FALSE;
	
	if( m_s_sgn4 & 0x80 ){
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 課税売上割合95%以上？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	95%以上
//			FALSE	：	95%未満
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsRatioOver95()
{
	if( Sn_UPERS&0x01 ){
		return FALSE;
	}
	else{
		return TRUE;
	}
}

//-----------------------------------------------------------------------------
// 修正申告？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	修正申告（確定・中間）
//			FALSE	：	修正申告以外
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsRevisedTaxReturn()
{
//	if( pSyzShin->Snh.Sn_SKKBN == 0x03 || pSyzShin->Snh.Sn_SKKBN == 0x03 ) St = 1;
	if( (Sn_SKKBN==0x03) || (Sn_SKKBN==0x04) ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 個人？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	個人
//			FALSE	：	個人でない
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsSoleProprietor()
{
	if( ((m_apno&0xff)==0x10) || (Sn_KOJIN&0x01) ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 消費税の還付申告明細書を使用？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	消費税の還付申告に関する明細書を使用
//			FALSE	：	仕入控除税額に関する明細書を使用
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsUseTaxRefund()
{
/*- '14.3.03 -*/
//	if( Sn_ZeiHou&0x80 ){
//		return TRUE;
//	}
//	else{
//		return FALSE;
//	}
/*-----------*/
	// 絶えず使用
	return TRUE;
/*-----------*/
}

//-----------------------------------------------------------------------------
// 中間予定申告？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	中間予定申告（第26号）
//			FALSE	：	中間予定申告　以外
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsMiddleProvisional()
{
	if( (Sn_SKKBN==0x02) && (Sn_Syuusei&0x08) ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 年月日データの取得
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	取得元データの種類
//		pYy		：	年
//		pMm		：	月
//		pDd		：	日
//-----------------------------------------------------------------------------
void CSnHeadData::GetYmdData( EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd )
{
	long	*ptmpYmd = NULL;

	if( dsDtype == ID_ICSSH_KAZEIKIKAN_FROM ){		// Sn_KDAYS：課税期間　自
		ptmpYmd = &Sn_KDAYS;
	}
	else if( dsDtype == ID_ICSSH_KAZEIKIKAN_TO ){	// Sn_KDAYE：課税期間　至
		ptmpYmd = &Sn_KDAYE;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_FROM ){	// Sn_MDAYS：中間申告の計算期間　自
		ptmpYmd = &Sn_MDAYS;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_TO ){		// Sn_MDAYE：中間申告の計算期間　至
		ptmpYmd = &Sn_MDAYE;
	}
	else if( dsDtype == ID_ICSSH_PRESENT_DATE ){	// Sn_PRDAY：提出年月日
		ptmpYmd = &Sn_PRDAY;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_FROM ){		// 決算期間 自
		ptmpYmd = &m_ss_ymd;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_TO ){		// 決算期間 至
		ptmpYmd = &m_ee_ymd;
	}
	else{
		// 該当 元データ無し
	}

	if( (*ptmpYmd) == 0 ){
		return;
	}

	// 西暦を平成に変換
	int	dYear = 0;
	CVolDateDB	voldate;

	//[Y]
	if((*ptmpYmd) / 10000)
	{
		voldate.db_datecnv( (*ptmpYmd), &dYear, 0, 2 );
	}
	m_Arith.int_bcd( (void*)pYy, dYear, 1 );

	// 月
	int dMon = 0;
	dMon = ((*ptmpYmd) % 10000) / 100;
	m_Arith.int_bcd( (void*)pMm, dMon, 1 );

	// 日
	int dDay = 0;
	dDay = (*ptmpYmd) % 100;
	m_Arith.int_bcd( (void*)pDd, dDay, 1 );
}

//-----------------------------------------------------------------------------
// 年月日データの取込
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	データの取込先
//		type	：	年？月？日？
//		data	：	BCDデータ
//-----------------------------------------------------------------------------
void CSnHeadData::SetYmdData( EnumIdIcsShDateType dsDtype, int type, char data )
{
	int	rtYmd = 0;

	// 取得データの変換
	int nData = m_Arith.bcd_int( &data, 1 );

	if( type == 0 ){		// 年
		// 平成を西暦に変換
		int	dYear = 0;
		CVolDateDB	voldate;
		voldate.db_datecnv( nData, &dYear, 1, 2 );
		nData = dYear;
		if( data == 0x00 ){	// 取消を意味する！
			nData = 0;
		}
	}
	else if( type == 1 ){	//[Y] 月
		if( (nData<1) || (12<nData) ){
			// 不正データ
		}
	}
	else if( type == 2 ){	//[Y] 日
		if( (nData<1) || (31<nData) ){
			// 不正データ
		}
	}

	long	*ptmpYmd = NULL;

	if( dsDtype == ID_ICSSH_KAZEIKIKAN_FROM ){		// Sn_KDAYS：課税期間　自
		ptmpYmd = &Sn_KDAYS;
	}
	else if( dsDtype == ID_ICSSH_KAZEIKIKAN_TO ){	// Sn_KDAYE：課税期間　至
		ptmpYmd = &Sn_KDAYE;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_FROM ){	// Sn_MDAYS：中間申告の計算期間　自
		ptmpYmd = &Sn_MDAYS;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_TO ){		// Sn_MDAYE：中間申告の計算期間　至
		ptmpYmd = &Sn_MDAYE;
	}
	else if( dsDtype == ID_ICSSH_PRESENT_DATE ){	// Sn_PRDAY：提出年月日
		ptmpYmd = &Sn_PRDAY;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_FROM ){		// 決算期間 自
		// 取り込みは未許可
		return;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_TO ){		// 決算期間 至
		// 取り込みは未許可
		return;
	}
	else{
		// 該当 元データ無し
	}

	if( type == 0 ){		// 年
		int	mmdd = (*ptmpYmd) % 10000;
		(*ptmpYmd) = mmdd + (nData * 10000);
	}
	else if( type == 1 ){	// 月
		int yy = (*ptmpYmd) / 10000;
		int dd = (*ptmpYmd) % 100;
		(*ptmpYmd) = (yy * 10000) + (nData * 100) + dd;
	}
	else if( type == 2 ){	// 日
		int yymm = (*ptmpYmd) / 100;
		(*ptmpYmd) = (yymm * 100) + nData;
	}
}

//-----------------------------------------------------------------------------
// 税務署コードの取得
//-----------------------------------------------------------------------------
// 引数	pParent		：	親ウィンドウ
//		pEselect	：	電子申告情報
//-----------------------------------------------------------------------------
// 返送値	0		：	
//-----------------------------------------------------------------------------
int CSnHeadData::GetTaxationOffice( CWnd *pParent, ESELECT *pEselect )
{
	char	Number[20]={0}, SName[64]={0}, DName[64]={0}, TName[64]={0};
	int		len, st=IDNO;
	char	buf[256] = {0};

	// 税務署名
	memmove( SName, Sn_ZMSYO, sizeof(Sn_ZMSYO) );
	// 税務署番号
	memmove( Number, Sn_TAXNO, sizeof(Sn_TAXNO) );
	// 都道府県名取得
	;

	// 電子申告チェック
	if( (Number[0]&0xff) != 'F' ){

		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle( shComInstance );

		// 漢字スペースカットで税務署名が有り、税務署番号が無い時のチェック
		len = m_Util.MojiSearch( DName, SName, 12 );
		if( len && (strlen(Number)==0) ){
			st = Esel_TaxOfficeFromName( DName, Number, TName );
			if( st == 0 ){
				memmove( Sn_TAXNO, Number, sizeof(Sn_TAXNO) );		// 税務署コード取得
			}
			else{
				CString msg;
				if( st == -1 ) {
					msg  = _T("入力された税務署名に該当する税務署が見つかりませんでした。\n");
					msg += _T("電子申告を行う場合は、正確な提出先税務署を設定する必要があります。\n\n");
					msg += _T("税務署選択を行いますか？");
				}
				else	{
					msg  = _T("入力された税務署名と類似した税務署名が確認できました。\n");
					msg += _T("電子申告を行う場合は、正確な提出先税務署を設定する必要があります。\n\n");
					msg += _T("税務署選択を行いますか？");
				}
				st = ICSMessageBox( msg, MB_YESNO, 0, 0, pParent );
				if( st == IDYES ){
					memset( pEselect, '\0', sizeof(ESELECT) );
					pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
					memmove( pEselect->nm_office, DName, len );
					if( Esel_SelectTaxOffice(pEselect) == IDOK ){
						memmove( Sn_TAXNO, pEselect->cd_office, sizeof(Sn_TAXNO) );
						memset( buf, '\0', sizeof(buf) );
						len = (int)strlen( pEselect->nm_office );
						memmove( buf, pEselect->nm_office, len );
						memmove( Sn_ZMSYO, buf, sizeof(Sn_ZMSYO) );
						// 税務署名
//						memset( buf, '\0', sizeof(buf) );
//						len = m_Util.MixSpaceCutLength( (char*)Sn_ZMSYO, sizeof(Sn_ZMSYO) );
//						memmove( buf, Sn_ZMSYO, len );
//						DiagData.data_edit = buf;
//						m_IppanEx2.SetData( ID2010, (struct IUnknown *)&DiagData );
						st = 0;
					}
					else{
						memset( Sn_TAXNO, 'F', sizeof(Sn_TAXNO) );
					}
				}
				else{
					memset( Sn_TAXNO, 'F', sizeof(Sn_TAXNO) );
				}
			}
		}
		AfxSetResourceHandle( svhInstResource );
	}

	return st;
}

//-----------------------------------------------------------------------------
// ロック解除をするかどうか
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		pParent		：	親ウィンドウ
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CSnHeadData::DoUnlock( CDBNpSub *pDBNpSub, CWnd *pParent )
{
	if( Sn_Sign4&0x01 ){
		if( m_Util.IsUnConnectMst(pDBNpSub->zvol) == FALSE ){

			HINSTANCE svhInstResource = AfxGetResourceHandle();
			AfxSetResourceHandle( shComInstance );

			CString str;
			str += _T("入力ロックを解除しますか？\n");
			str += _T("※ロック解除後は、自動集計が行われます。\n");
			str += _T("（非連動処理又は履歴確認時は、自動集計しません。）\n");
			if( ICSMessageBox(str, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, 0, 0, pParent) != IDYES ){
				AfxSetResourceHandle( svhInstResource );
				return IDNO;
			}

			AfxSetResourceHandle( svhInstResource );
		}
		Sn_Sign4 &= 0xfe;
	}
	else{
		Sn_Sign4 |= 0x01;
	}

	return IDYES;
}

//-----------------------------------------------------------------------------
// 課税売上高５億円チェック
//-----------------------------------------------------------------------------
// 引数	
//
// 
// 
//-----------------------------------------------------------------------------
//	1: ５億円超え
//  2: ４億五千万以上５億円未満
//  3: ５億円以下
//-----------------------------------------------------------------------------
//int CDBSyzShinView::TaxationSalesCheck( char *kazeiuri, char *valcnv )
int CSnHeadData::CheckTaxationSales( char *kazeiuri, char *valcnv, int s_tno, int d_tno )
{
	CArith	arith(0x16);

	int		MKno;
	char	WKS[6]={0}, WKB[6]={0}, WK1[6]={0}, WK2[6]={0}, WK3[6]={0}, CBF[20]={0};
	char	YMDS[4]={0}, YMDE[4]={0};

	if( d_tno == 12 ){
		// 最終決修月は12ヶ月目と同値で扱う
		d_tno = 11;
	}

	MKno = ( d_tno - s_tno ) + 1;
	memmove( WK1, kazeiuri, 6 );			// 課税売上高
	arith.l_input( WK2, _T("500000000") );	// ５億
	arith.l_input( WK3, _T("450000000") );	// ４億５千万
//-- '15.02.12 --
//	if( MKno != 12 ){
//		sprintf_s( CBF, sizeof(CBF), _T("%d"), MKno );
//		arith.l_input( WKB, CBF );				// 
//		arith.l_div( WK1, WK1, WKB );			// ÷指定月数 ＝ 一月当たりの残高
//
//		arith.l_input( WKS, "12" );				// 
//		arith.l_mul( WK1, WK1, WKS );			// ×年間月数 ＝ 年間残高
//		memmove( valcnv, WK1, 6 );
//	}
//	else{
//		memmove( valcnv, kazeiuri, 6 );
//	}
//---------------
//--- '15.04.17 ---
//	if( (Sn_SKKBN==0x02) || (Sn_SKKBN==0x04) ){
//		if( MKno != 12 ){
//			sprintf_s( CBF, sizeof(CBF), _T("%d"), MKno );
//			arith.l_input( WKB, CBF );				// 
//			arith.l_div( WK1, WK1, WKB );			// ÷指定月数 ＝ 一月当たりの残高
//
//			arith.l_input( WKS, "12" );				// 
//			arith.l_mul( WK1, WK1, WKS );			// ×年間月数 ＝ 年間残高
//			memmove( valcnv, WK1, 6 );
//		}
//		else{
//			memmove( valcnv, kazeiuri, 6 );
//		}
//	}
//	else{
//		memmove( valcnv, kazeiuri, 6 );
//	}
//-----------------
	if( MKno != 12 ){
		sprintf_s( CBF, sizeof(CBF), _T("%d"), MKno );
		arith.l_input( WKB, CBF );				// 
		arith.l_div( WK1, WK1, WKB );			// ÷指定月数 ＝ 一月当たりの残高

		arith.l_input( WKS, "12" );				// 
		arith.l_mul( WK1, WK1, WKS );			// ×年間月数 ＝ 年間残高
		memmove( valcnv, WK1, 6 );
	}
	else{
		memmove( valcnv, kazeiuri, 6 );
	}
//-----------------
//---------------

#ifdef	_DEBUG_EX
	char	buf[128], dpmy[20];
	l_print( dpmy, valcnv, "SSS,SSS,SSS,SS9" );
	if( MKno != 12 ) 	sprintf_s( buf, sizeof( buf ), "課税売上割合[%s 円] (%d / 12)", dpmy, MKno );
	else				sprintf_s( buf, sizeof( buf ), "課税売上割合[%s 円]", dpmy );
	ICSMessageBox( buf );
#endif

	if( arith.l_cmp(WK1, WK2) > 0 ){
		return 1;									// ５億越え
	}
	else{
		if( (arith.l_cmp(WK1, WK3)>=0) && (arith.l_cmp(WK1, WK2)<=0) ){
			return 2;	// ４億５千万以下で５億以下
		}
		else{
			return 3;	// ４億５千万未満
		}
	}
}

//-----------------------------------------------------------------------------
// 申告区分の取得
//-----------------------------------------------------------------------------
BYTE CSnHeadData::GetKubun( unsigned char skkbn, unsigned char syuusei )
{
	if( (skkbn == 0x01) && (syuusei&0x02) ){
		return ID_ICSSH_KOUSEI;
	}
	if( (skkbn == 0x02) && (syuusei&0x08) ){
		return ID_ICSSH_YOTEI;
	}

	switch( skkbn ){
		case 1 :
			return ID_ICSSH_KAKUTEI;
		case 2 :
			return ID_ICSSH_TYUUKAN;
		case 3 :
			return ID_ICSSH_KAKUTEI_SYUUSEI;
		default:
			return ID_ICSSH_TYUUKAN_SYUUSEI;
	}
}

//-----------------------------------------------------------------------------
// 申告区分の取得
//-----------------------------------------------------------------------------
BYTE CSnHeadData::GetKubun()
{ 
	return GetKubun( Sn_SKKBN, Sn_Syuusei );
}

//-----------------------------------------------------------------------------
// 申告区分のセット
//-----------------------------------------------------------------------------
void CSnHeadData::SetKubun( int kubun )
{
	Sn_Syuusei &= ~0x0E;
	switch( kubun ){
		case ID_ICSSH_KOUSEI :	// 更正の請求書
			Sn_SKKBN = 0x01;
			Sn_Syuusei |= 0x02;
			break;
		case ID_ICSSH_YOTEI :	// 予定(26号)
			Sn_SKKBN = 0x02;
			Sn_Syuusei |= 0x08;
			break;
		case ID_ICSSH_KAKUTEI:
			Sn_SKKBN = 1;
			break;
		case ID_ICSSH_TYUUKAN:
			Sn_SKKBN = 2;
			break;
		case ID_ICSSH_KAKUTEI_SYUUSEI:
			Sn_SKKBN = 3;
			break;
//2018.03.13 INSERT START
		case ID_ICSSH_JUNKAKUTEI:
			Sn_SKKBN = 0x01;
			Sn_Syuusei |= 0x10;
			break;
//2018.03.13 INSERT END
		default:
			Sn_SKKBN = 4;
			break;
	}
}

//-----------------------------------------------------------------------------
// 期間短縮？
//-----------------------------------------------------------------------------
int CSnHeadData::IsKikanTansyuku()
{
	return Sn_KZTNSK ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
// 中間申告？
//-----------------------------------------------------------------------------
int CSnHeadData::IsTyukanShinkoku()
{
	const int skkbn = GetKubun();
	return skkbn == ID_ICSSH_TYUUKAN || skkbn == ID_ICSSH_TYUUKAN_SYUUSEI || skkbn == ID_ICSSH_YOTEI;
}

//-----------------------------------------------------------------------------
// 確定？
//-----------------------------------------------------------------------------
int CSnHeadData::IsKakuteiShinkoku()
{
	const int skkbn = GetKubun();
	return skkbn == ID_ICSSH_KAKUTEI || skkbn == ID_ICSSH_KAKUTEI_SYUUSEI || skkbn == ID_ICSSH_KOUSEI;
}

//2018.03.13 INSERT START
//-----------------------------------------------------------------------------
// 準確定？
//-----------------------------------------------------------------------------
//  準確定申告には、
//   確定申告・修正確定・更正の請求の準確定申告が存在します。
//   ですので、上記３パターンのどれかの準確定申告かどうかを判定しています。
//-----------------------------------------------------------------------------
int CSnHeadData::IsJunkakuteiShinkoku()
{
	if( IsTyukanShinkoku() == FALSE ){
		if( Sn_Syuusei&0x10 ){
			return TRUE;
		}
	}
	return FALSE;
}
//2018.03.13 INSERT END

//-----------------------------------------------------------------------------
// 集計済みデータのセット
//-----------------------------------------------------------------------------
// 引数	pSyzSyukei	：	消費税集計クラス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::SetCalqedData( CSyzSyukei *pSyzSyukei )
{
	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("15") );

	int cnt = (int)money.GetCount();
	int incnt = 0;
	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code.Left(6) == _T("150600") ){
				memmove( Sn_KURIA, money[i][j].arith, 6 );
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 集計データの読込み必要？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	集計データの読込み　必要
//			FALSE	：	集計データの読込み　不要
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsReadCalqedData()
{
	if( ( (Sn_SKKBN==0x01) || ((Sn_SKKBN==0x02)&&(IsMiddleProvisional()==FALSE)) || (Sn_SKKBN==0x03) || (Sn_SKKBN==0x04) ) && ((Sn_Sign4&0x81)==0x00) ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 非連動処理の入力状態？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	必要あり
//			FALSE	：	必要なし
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsInUnconnectInput( CDBNpSub *pDBNpSub )
{
	BOOL	bRt = FALSE;

	if( m_Util.IsUnConnectMst(pDBNpSub->zvol) || (Sn_Sign4&0x80) ){
		if( !(Sn_Sign4&0x01) ){
			bRt = TRUE;
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 更新時のデータクリア
//-----------------------------------------------------------------------------
// 引数	
// 
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CSnHeadData::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	m_Arith.l_clear( Sn_CK45 );
	m_Arith.l_clear( Sn_CK60 );
	m_Arith.l_clear( Sn_CH45 );
	m_Arith.l_clear( Sn_CH60 );
	m_Arith.l_clear( Sn_CZ45 );
	m_Arith.l_clear( Sn_CZ60 );

	for( int i=0; i<12; i++ ){
		m_Arith.l_clear( Sn_TYNF[i] );
		m_Arith.l_clear( Sn_TTNF[i] );
		// 年３回
		switch( i ) {
			case 0 :
				m_Arith.l_clear( Sn_TTYNF1 );
				m_Arith.l_clear( Sn_TYNF1 );
				break;
			case 1 :
				m_Arith.l_clear( Sn_TTYNF2 );
				m_Arith.l_clear( Sn_TYNF2 );
				break;
			case 2 :
				m_Arith.l_clear( Sn_TTYNF3 );
				m_Arith.l_clear( Sn_TYNF3 );
				break;
			case 3 :	
				m_Arith.l_clear( Sn_TTYNF4 );
				m_Arith.l_clear( Sn_TYNF4 );
				break;
			default:
				break;
		}
	}
	
	Sn_Syuusei = 0x00;
	Sn_TOKUR = 0;
	Sn_TUMIAG = 0;
	SVzchg = 0;
	SVmzsw = 0;
	Sn_MCOUNT = 0;
	Sn_JCOUNT = 0;

	ASSERT( pDBNpSub );

	// szvol 関連
//	if( GetDataSzvolSub(pDBNpSub, sn_seq) ){
//		return -1;
//	}

/*	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), 0 );

	if( pDBNpSub->szsnk ){
		pDBNpSub->szsnk->Requery( filter );
	}
	else{
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
	}
	if( pDBNpSub->szsnk->st == -1 ){
		// 新規
		return 0;
	}

	CSZSNK *pSZSNK = pDBNpSub->szsnk;

	pSZSNK->Edit();

	pSZSNK->Sn_KSTDM = _T("0");
	pSZSNK->Sn_TIKAM = _T("0");
	pSZSNK->Sn_SYTXM = _T("0");
	pSZSNK->Sn_CK45 = _T("0");
	pSZSNK->Sn_CK60 = _T("0");
	pSZSNK->Sn_CH45 = _T("0");
	pSZSNK->Sn_CH60 = _T("0");
	pSZSNK->Sn_CZ45 = _T("0");
	pSZSNK->Sn_CZ60 = _T("0");

	for( int i=0; i<12; i++ ){
		pSZSNK->Sn_TYNF[i] = _T("0");
		pSZSNK->Sn_TTYNF[i] = _T("0");
		// 年３回
		switch( i ) {
			case 0 :
				pSZSNK->Sn_TYNF[i] = _T("0");
				pSZSNK->Sn_TTYNF[i] = _T("0");
				break;
			case 1 :
				pSZSNK->Sn_TYNF[i] = _T("0");
				pSZSNK->Sn_TTYNF[i] = _T("0");
				break;
			case 2 :
				pSZSNK->Sn_TYNF[i] = _T("0");	
				pSZSNK->Sn_TTYNF[i] = _T("0");
				break;
			case 3 :	
				pSZSNK->Sn_TYNF[i] = _T("0");	
				pSZSNK->Sn_TTYNF[i] = _T("0");
				break;
			default:
				break;
		}
	}
	pSZSNK->Sn_Syuusei = 0;

	pSZSNK->Update();*/


	return 0;
}

//-----------------------------------------------------------------------------
// 既存データクリア
//   データベースとの連結を行わない
//-----------------------------------------------------------------------------
void CSnHeadData::ClearReadData()
{
	Sn_VER		= 0;
	Sn_SKKBN	= 0;
	Sn_KOJIN	= 0;
	Sn_KOANB	= 0;
	memset( Sn_ITIREN, '\0', sizeof(Sn_ITIREN) );
	memset( Sn_SEIRI, '\0', sizeof(Sn_SEIRI) );
	memset( Sn_ZSEIRI, '\0', sizeof(Sn_ZSEIRI) );
	Sn_KDAYS	= 0;
	Sn_KDAYE	= 0;
	Sn_MDAYS	= 0;
	Sn_MDAYE	= 0;
	Sn_PRDAY	= 0;
	Sn_SPECIAL	= 0;
	memset( Sn_ZMSYO, '\0', sizeof(Sn_ZMSYO) );
	memset( Sn_ZIP, '\0', sizeof(Sn_ZIP) );
	memset( Sn_SYOKN, '\0', sizeof(Sn_SYOKN) );
	memset( Sn_TELNO1, '\0', sizeof(Sn_TELNO1) );
	memset( Sn_TELNO2, '\0', sizeof(Sn_TELNO2) );
	memset( Sn_TELNO3, '\0', sizeof(Sn_TELNO3) );
	memset( Sn_NOZEI, '\0', sizeof(Sn_NOZEI) );
	memset( Sn_CONAM, '\0', sizeof(Sn_CONAM) );
	memset( Sn_COFRI, '\0', sizeof(Sn_COFRI) );
	memset( Sn_DIHYO, '\0', sizeof(Sn_DIHYO) );
	memset( Sn_DIFRI, '\0', sizeof(Sn_DIFRI) );
	memset( Sn_KEIRI, '\0', sizeof(Sn_KEIRI) );
	memset( Sn_GPNAM, '\0', sizeof(Sn_GPNAM) );
	Sn_KAPPU	= 0;
	Sn_NOBE		= 0;
	Sn_KOJI		= 0;
	Sn_GENKN	= 0;
	Sn_TUMIAG	= 0;
	Sn_KZTNSK	= 0;
	Sn_MCOUNT	= 0;
	Sn_JCOUNT	= 0;
	Sn_TOKUR	= 0;
	Sn_UPERS	= 0;
	Sn_TYPE		= 0;
	Sn_BYsw		= 0;
	memset( Sn_BANK, '\0', sizeof(Sn_BANK) );
	memset( Sn_SITEN, '\0', sizeof(Sn_SITEN) );
	memset( Sn_YOKIN, '\0', sizeof(Sn_YOKIN) );
	memset( Sn_YUBIN, '\0', sizeof(Sn_YUBIN) );
	memset( Sn_KOZA, '\0', sizeof(Sn_KOZA) );
	memset( Sn_YUNUM1, '\0', sizeof(Sn_YUNUM1) );
	memset( Sn_YUNUM2, '\0', sizeof(Sn_YUNUM2) );
	Sn_FINANCE	= 0;
	Sn_CHAIN	= 0;
	Sn_ZeiHou	= 0;
	Sn_Syuusei	= 0;
	Sn_Sign1	= 0;
	Sn_Sign2	= 0;
	Sn_Sign3	= 0;
	Sn_Sign4	= 0;
	//Sn_Sign5	= 0;
	//Sn_Sign6	= 0;
	memset( Sn_TAXNO, '\0', sizeof(Sn_TAXNO) );
	memset( Sn_ADRU1, '\0', sizeof(Sn_ADRU1) );
	memset( Sn_ADRU2, '\0', sizeof(Sn_ADRU2) );
	memset( Sn_ADRC1, '\0', sizeof(Sn_ADRC1) );
	memset( Sn_ADRC2, '\0', sizeof(Sn_ADRC2) );
	memset( Sn_ADRD1, '\0', sizeof(Sn_ADRD1) );
	memset( Sn_ADRD2, '\0', sizeof(Sn_ADRD2) );
	memset( Sn_ADDM0, '\0', sizeof(Sn_ADDM0) );
	Sn_PPVER	= 0;
	Sn_PPTYP	= 0;
	memset( Sn_ADRD3, '\0', sizeof(Sn_ADRD3) );
	memset( Sn_ADKUP, '\0', sizeof(Sn_ADKUP) );
	memset( Sn_ADKDW, '\0', sizeof(Sn_ADKDW) );
	memset( Sn_ADKNO, '\0', sizeof(Sn_ADKNO) );
	memset( Sn_ADRD4, '\0', sizeof(Sn_ADRD4) );
	memset( Sn_ADSNO, '\0', sizeof(Sn_ADSNO) );
	memset( Sn_ADKDC, '\0', sizeof(Sn_ADKDC) );
	SubInf		= 0;

	SVvers		= 0;
	SVnver		= 0;
	SVsign		= 0x00;
	SVmzsw		= 0x00;
	memset( SVsper, '\0', sizeof(SVsper) );
	SVsasw = 0x00;
	SVhksw = 0x00;
	SVd5sw = 0x00;
	SVkojn = 0x00;
	SVzchg = 0x00;
	SVkazei = 0x00;
	SVsacq = 0x00;
	SVzisq = 0x00;
	memset( SVtval1, '\0', sizeof(SVtval1) );
	memset( SVtval2, '\0', sizeof(SVtval2) );
	memset( SVzuri, '\0', sizeof(SVzuri) );

	m_apno = 0x00;
	m_s_sgn2 = 0x00;
	m_s_sgn4 = 0x00;
	m_ss_ymd = 0;
	m_ee_ymd = 0;

	m_UriRatioBorder = 9500;	// 95%

	memset( Sn_WURI, '\0', sizeof(Sn_WURI) );

	memset( Sn_TYNF1, '\0', sizeof(Sn_TYNF1) );
	memset( Sn_TYNF2, '\0', sizeof(Sn_TYNF2) );
	memset( Sn_TYNF3, '\0', sizeof(Sn_TYNF3) );
	memset( Sn_TYNF4, '\0', sizeof(Sn_TYNF4) );
	memset( Sn_TTYNF1, '\0', sizeof(Sn_TTYNF1) );
	memset( Sn_TTYNF2, '\0', sizeof(Sn_TTYNF2) );
	memset( Sn_TTYNF3, '\0', sizeof(Sn_TTYNF3) );
	memset( Sn_TTYNF4, '\0', sizeof(Sn_TTYNF4) );
	memset( Sn_TYNF, '\0', sizeof(Sn_TYNF) );
	memset( Sn_TTNF, '\0', sizeof(Sn_TTNF) );

	memset( Sn_CK45, '\0', sizeof(Sn_CK45) );
	memset( Sn_CZ45, '\0', sizeof(Sn_CZ45) );
	memset( Sn_CH45, '\0', sizeof(Sn_CH45) );
	memset( Sn_CK60, '\0', sizeof(Sn_CK60) );
	memset( Sn_CZ60, '\0', sizeof(Sn_CZ60) );
	memset( Sn_CH60, '\0', sizeof(Sn_CH60) );
	memset( Sn_KURIA, '\0', sizeof(Sn_KURIA) );

	memset( rsv1, '\0', sizeof(rsv1) );
	memset( rsv2, '\0', sizeof(rsv2) );

	memset( SVkan0, '\0', sizeof(SVkan0) );
	memset( SVkan1, '\0', sizeof(SVkan1) );

//--> '15.02.27 INS START
	m_Zrkbn = 0;
	m_ZroutSgn = 0;
//<-- '15.02.27 INS END

//--> '15.07.11 INS START
	memset( Sn_HOJIN_NO, '\0', sizeof(Sn_HOJIN_NO) );
	memset( Sn_HOJIN_NO_EXT, '\0', sizeof(Sn_HOJIN_NO_EXT) );
	memset( Sn_KOJIN_HASH, '\0', sizeof(Sn_KOJIN_HASH) );
	memset( rsv1, '\0', sizeof(rsv1) );
//<-- '15.02.27 INS END

//--> '15.08.26 INS START
	m_DispTabSgn = 0;
//<-- '15.08.26 INS END

//--> '15.12.15 INS START
	memset( Sn_DIADR, '\0', sizeof(Sn_DIADR) );
	memset( Sn_DIZIP, '\0', sizeof(Sn_DIZIP) );
//<-- '15.12.15 INS END

//--> '15.12.15 INS START
	memset( Sn_DITELNO1, '\0', sizeof(Sn_DITELNO1) );
	memset( Sn_DITELNO2, '\0', sizeof(Sn_DITELNO2) );
	memset( Sn_DITELNO3, '\0', sizeof(Sn_DITELNO3) );
//<-- '15.12.15 INS END

	Sn_PrintOutSgn = 0;
	Sn_PrintOutSgn2 = 0;
	Sn_GenericSgn = 0;
}

//-----------------------------------------------------------------------------
// 連動処理？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	財務データ 連動
//			FALSE	：	財務データ 非連動
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsDataConnected()
{
	if( !(Sn_Sign4&0x80) ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

#ifdef _20151204_EXTEND_
//-----------------------------------------------------------------------------
// 電子申告バージョンの取得
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスポインタ
//		skkbn		：	申告区分フラグ
//		syuusei		：	修正申告フラグ
//		kdayE		：	課税期間（至）
//		mdayE		：	中間申告の計算期間（至）
//		kdayS		：	課税期間（自）
//		mdayS		：	中間申告の計算期間（自）
//-----------------------------------------------------------------------------
// 返送値			：	電子申告バージョン
//-----------------------------------------------------------------------------
int CSnHeadData::GetEselVersion( CDBNpSub *pDBNpSub, unsigned char skkbn, unsigned char syuusei, long kdayE, long mdayE, long kdayS, long mdayS )
{
	int	rtVersion = ICS_SH_H25_ETAX_VER;

	EnumIdIcsShShinkokuKubun	curKbn = (EnumIdIcsShShinkokuKubun)GetKubun(skkbn, syuusei);

	long	chkDateE=-1, chkDateS=-1;
	if( (curKbn==ID_ICSSH_KAKUTEI) || (curKbn==ID_ICSSH_KAKUTEI_SYUUSEI) ){
		chkDateS = kdayS;
		chkDateE = kdayE;
	}
	else if( (curKbn==ID_ICSSH_TYUUKAN) || (curKbn==ID_ICSSH_TYUUKAN_SYUUSEI) ){
//-- '15.10.07 --
//		chkDateS = kdayS;
//---------------
		chkDateS = mdayS;
//---------------
		chkDateE = mdayE;
	}
	else if( curKbn == ID_ICSSH_YOTEI ){
//-- '15.10.07 --
//		chkDateS = kdayS;
//---------------
		chkDateS = mdayS;
//---------------
		chkDateE = mdayE;
	}
	else if( curKbn == ID_ICSSH_KOUSEI ){
		chkDateS = kdayS;
		chkDateE = kdayE;
	}

//--- '15.10.07 ---
////-- '15.04.13 --
////	if( chkDate >= ICS_SH_8PER_REFORM_DAY ){
////		rtVersion = ICS_SH_H26_ETAX_VER;
////	}
////---------------
//	if( chkDateS >= ICS_SH_SIXKIND_REFORM_DAY ){
//		CSyzUtil	SyzUtil;
//		EnumIdIcsShKazeihoushiki	kzhoushiki = SyzUtil.GetKazeihoushiki(pDBNpSub->zvol);
//		if( kzhoushiki == ID_ICSSH_KANNI ){
//			rtVersion = ICS_SH_H27_ETAX_VER;
//		}
//		else{
//			rtVersion = ICS_SH_H26_ETAX_VER;
//		}
//	}
//	else if( chkDateE >= ICS_SH_8PER_REFORM_DAY ){
//		rtVersion = ICS_SH_H26_ETAX_VER;
//	}
////---------------
//-----------------
	if( chkDateE >= ICS_SH_TOKUTEI_REFORM_DAY ){
//-- '15.10.29 --
//		rtVersion = ICS_SH_H27_2_ETAX_VER;
//---------------
		CSyzUtil	SyzUtil;
		EnumIdIcsShKazeihoushiki	kzhoushiki = SyzUtil.GetKazeihoushiki(pDBNpSub->zvol);
		if( kzhoushiki == ID_ICSSH_KANNI ){
//--- '15.11.13 ---
//			rtVersion = ICS_SH_H27_ETAX_VER;
//-----------------
			if( chkDateS >= ICS_SH_SIXKIND_REFORM_DAY ){
//--- '15.11.25 ---
//				rtVersion = ICS_SH_H27_ETAX_VER;
//-----------------
				rtVersion = ICS_SH_H27_2_ETAX_VER;
//-----------------
			}
			else{
				rtVersion = ICS_SH_H26_ETAX_VER;
			}
		}
		else{
			rtVersion = ICS_SH_H27_2_ETAX_VER;
		}
//---------------
	}
	else if( chkDateS >= ICS_SH_SIXKIND_REFORM_DAY ){
		CSyzUtil	SyzUtil;
		EnumIdIcsShKazeihoushiki	kzhoushiki = SyzUtil.GetKazeihoushiki(pDBNpSub->zvol);
		if( kzhoushiki == ID_ICSSH_KANNI ){
			rtVersion = ICS_SH_H27_ETAX_VER;
		}
		else{
			rtVersion = ICS_SH_H26_ETAX_VER;
		}
	}
	else if( chkDateE >= ICS_SH_8PER_REFORM_DAY ){
		rtVersion = ICS_SH_H26_ETAX_VER;
	}
//-----------------

	return rtVersion;
}
#else
//-----------------------------------------------------------------------------
// 電子申告バージョンの取得
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスポインタ
//		skkbn		：	申告区分フラグ
//		syuusei		：	修正申告フラグ
//		kdayE		：	課税期間（至）
//		mdayE		：	中間申告の計算期間（至）
//		kdayS		：	課税期間（自）
//		mdayS		：	中間申告の計算期間（自）
//-----------------------------------------------------------------------------
// 返送値			：	電子申告バージョン
//-----------------------------------------------------------------------------
int CSnHeadData::GetEselVersion( CDBNpSub *pDBNpSub, unsigned char skkbn, unsigned char syuusei, long kdayE, long mdayE, long kdayS, long mdayS )
{
	int	rtVersion = ICS_SH_H25_ETAX_VER;

	EnumIdIcsShShinkokuKubun	curKbn = (EnumIdIcsShShinkokuKubun)GetKubun(skkbn, syuusei);

	long	chkDateE=-1, chkDateS=-1;
	long	chkKaniDateS=-1;
	if( (curKbn==ID_ICSSH_KAKUTEI) || (curKbn==ID_ICSSH_KAKUTEI_SYUUSEI) ){
		chkDateS = kdayS;
		chkDateE = kdayE;
		chkKaniDateS = kdayS;
	}
	else if( (curKbn==ID_ICSSH_TYUUKAN) || (curKbn==ID_ICSSH_TYUUKAN_SYUUSEI) ){
		chkDateS = mdayS;
		chkDateE = mdayE;
		chkKaniDateS = kdayS;
	}
	else if( curKbn == ID_ICSSH_YOTEI ){
		chkDateS = mdayS;
		chkDateE = mdayE;
		chkKaniDateS = kdayS;
	}
	else if( curKbn == ID_ICSSH_KOUSEI ){
		chkDateS = kdayS;
		chkDateE = kdayE;
		chkKaniDateS = kdayS;
	}

	if( IsJunkakuteiShinkoku() ){
		rtVersion = ICS_SH_JYUN_KAKU_VER;
	}
	else if( chkDateE >= ICS_SH_INVOICE_DAY ) {
		// 令和5年10月1日以降の課税期間（インボイス）
		if( IsSoleProprietor() ){
//------>('23.10.20 cor)個人の帳票を公開する為、電子申告バージョンを上げるように変更。
			//// 個人は帳票が未公開のため、電子申告バージョンを上げない
			//rtVersion = ICS_SH_R04KOJIN_ETAX_VER;
//-------
			//rtVersion = ICS_SH_INV_ETAX_VER;
			//23.11.1　個人の対応（課税取引金額計算表）が未了のため、バージョンを上げておく
			rtVersion = ICS_SH_INVKOJIN_ETAX_VER;
//<------
		}
		else {
			rtVersion = ICS_SH_INV_ETAX_VER;
		}
	}
	else if( chkDateE >= ICS_SH_R04_KOJINREFORM_DAY ) {
		if( IsSoleProprietor() ){
			// 令和4年4月1日以降の課税期間かつ個人業種の場合0x09
			rtVersion = ICS_SH_R04KOJIN_ETAX_VER;
		}
		else {
			// 法人の場合0x08
			rtVersion = ICS_SH_R04_ETAX_VER;
		}
	}
	else if( chkDateE >= ICS_SH_R04_REFORM_DAY ){
		rtVersion = ICS_SH_R04_ETAX_VER;
	}
	else if( chkDateE >= ICS_SH_10PER_REFORM_DAY ){
		if ( m_DispTabSgn & 0x1000 ) {
			rtVersion = ICS_SH_R02_ETAX_VER;
		}
		else {
			rtVersion = ICS_SH_H31_ETAX_VER;
		}
	}
	else if( chkDateS >= ICS_SH_MYNUMBER_REFORM_DAY ){
		rtVersion = ICS_SH_H28_MYNOETAX_VER;
	}
	else if( chkDateE >= ICS_SH_TOKUTEI_REFORM_DAY ){
		CSyzUtil	SyzUtil;
		EnumIdIcsShKazeihoushiki	kzhoushiki = SyzUtil.GetKazeihoushiki(pDBNpSub->zvol);
		if( kzhoushiki == ID_ICSSH_KANNI ){
//-- '15.12.22 --
//			if( chkDateS >= ICS_SH_SIXKIND_REFORM_DAY ){
//---------------
			if( chkKaniDateS >= ICS_SH_SIXKIND_REFORM_DAY ){
//---------------
				rtVersion = ICS_SH_H27_2_ETAX_VER;
			}
			else{
				rtVersion = ICS_SH_H26_ETAX_VER;
			}
		}
		else{
			rtVersion = ICS_SH_H27_2_ETAX_VER;
		}
	}
//-- '15.12.22 --
//	else if( chkDateS >= ICS_SH_SIXKIND_REFORM_DAY ){
//---------------
	else if( chkKaniDateS >= ICS_SH_SIXKIND_REFORM_DAY ){
//---------------
		CSyzUtil	SyzUtil;
		EnumIdIcsShKazeihoushiki	kzhoushiki = SyzUtil.GetKazeihoushiki(pDBNpSub->zvol);
		if( kzhoushiki == ID_ICSSH_KANNI ){
			rtVersion = ICS_SH_H27_ETAX_VER;
		}
		else{
			rtVersion = ICS_SH_H26_ETAX_VER;
		}
	}
	else if( chkDateE >= ICS_SH_8PER_REFORM_DAY ){
		rtVersion = ICS_SH_H26_ETAX_VER;
	}

	return rtVersion;
}
#endif


//-----------------------------------------------------------------------------
// ヘッダバージョンの書き換え
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		version		：	セットするバージョン
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::SetSzvolHeadVersion( CDBNpSub *pDBNpSub, int version )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	BOOL openFlg = FALSE;
	if( pDBNpSub->szvol == NULL ){
		if( pDBNpSub->SyohizeiOpen() ){
			return -1;
		}
		openFlg = TRUE;
	}

	pDBNpSub->szvol->Edit();
	pDBNpSub->szvol->SVvers = version;
	pDBNpSub->szvol->Update();
	
	return 0;
}

//-----------------------------------------------------------------------------
// 電子申告バージョンの付加
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	非営利クラス
//		sn_seq		：	付加対象レコードシーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::SetEselVersion( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );

	BOOL openFlg = FALSE;
	if( pDBNpSub->szsnk ){
		pDBNpSub->szsnk->Requery( filter );
	}
	else{
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
		openFlg = TRUE;
	}
	if( pDBNpSub->szsnk->st == -1 ){
		// 新規
		if( openFlg ){
			pDBNpSub->SzsnkClose();
		}
		return 0;
	}

	CSZSNK *pSZSNK = pDBNpSub->szsnk;

//-- '15.04.13 --
//	int tmpSn_Ver = GetEselVersion( pSZSNK->Sn_SKKBN, pSZSNK->Sn_Syuusei, pSZSNK->Sn_KDAYE, pSZSNK->Sn_MDAYE );
//---------------
	int tmpSn_Ver = GetEselVersion( pDBNpSub, pSZSNK->Sn_SKKBN, pSZSNK->Sn_Syuusei, pSZSNK->Sn_KDAYE, pSZSNK->Sn_MDAYE, pSZSNK->Sn_KDAYS, pSZSNK->Sn_MDAYS );
//---------------

	pSZSNK->Edit();
	pSZSNK->Sn_VER = tmpSn_Ver;
	pSZSNK->Update();

	if( openFlg ){
		pDBNpSub->SzsnkClose();
	}

	return 0;

}

//-----------------------------------------------------------------------------
// 文字列データをサインに分解	('15.02.27)
//-----------------------------------------------------------------------------
// 引数	type	：	分解先データの種類
//						1:Sn_ADRD4[4], ...
//		srcStr	：	分解元文字列
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CSnHeadData::SepStrToSign( int type, CString &srcStr )
{
	const	int	EVERY_OPTION_STRLEN	=	8;
	// AAAAAAAABBBBBBBBCCCCCCCCDDDDDDDD(16進数データを文字列化して保存)

	CString	dStr;
// 20-0225 del -->
	//int		max = srcStr.GetLength() / EVERY_OPTION_STRLEN;
// 20-0225 del <--
// 20-0225 add -->
	int		max=0;
	if(Is10PerKikan() == TRUE)	{
		if(srcStr.GetLength() <= 40)	{
			// 従前の値を新エリアに移行
			CString	cs = _T("");
			int		sw1=0,sw2=0;
			sw1 = Sn_TYPE;
			cs = srcStr.Mid(3*EVERY_OPTION_STRLEN,EVERY_OPTION_STRLEN);
			m_Util.AscToBcd((char*)&sw2,cs.GetBuffer(),EVERY_OPTION_STRLEN);
			int		rv = PrintOutSgnConvert(sw1,sw2);
			hl_rev(&rv,4);
			cs.Format(_T("%08X"),rv);
			srcStr = srcStr + cs;
		}
		max=6;
	}
	else	{
		max=5;
	}
// 20-0225 add <--

	if( type == 1 ){
		for( int i=0; i<max; i++ ){
			dStr = srcStr.Mid( i*EVERY_OPTION_STRLEN, EVERY_OPTION_STRLEN );
			if( i==0 ){
				m_Util.AscToBcd( (char*)&m_Zrkbn, dStr.GetBuffer(), EVERY_OPTION_STRLEN );
			}
			else if( i==1 ){
				m_Util.AscToBcd( (char*)&m_ZroutSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN );
			}
			//------>特定課税仕入対応yoshida150713
			else if( i==2 ){
				m_Util.AscToBcd( (char*)&m_DispTabSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN );
			}
			//<-------------------------------
			else if (i == 3) {
				m_Util.AscToBcd( (char*)&Sn_PrintOutSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN );
			}
			// 汎用サイン追加 19.09.03
			else if ( i == 4 ) {
				m_Util.AscToBcd ( ( char* )&Sn_GenericSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN );
			}
// 20-0225 add -->
			// 各帳票の出力サイン２を追加　22.03.03
			else	{
				m_Util.AscToBcd((char*)&Sn_PrintOutSgn2,dStr.GetBuffer(),EVERY_OPTION_STRLEN);
			}
// 20-0225 add <--
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// サインを文字列データに結合	('15.02.27)
//-----------------------------------------------------------------------------
// 引数	type	：	結合元データの種類
//						1:Sn_ADRD4[4], ...
//		dstStr	：	結合先文字列
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CSnHeadData::BindSignToStr( int type, CString &dstStr )
{
	if( type == 1 ){
		dstStr.Empty();
		long	tmpZrkbn = m_Zrkbn;
		hl_rev( &tmpZrkbn, sizeof(tmpZrkbn) );
		long	tmpZroutSgn = m_ZroutSgn;
		hl_rev( &tmpZroutSgn, sizeof(tmpZroutSgn) );
		//------>特定課税仕入対応yoshida150713
		long	tmpDispTabSgn = m_DispTabSgn;
		hl_rev( &tmpDispTabSgn, sizeof(tmpDispTabSgn) );
		long	tmpPrintOutSgn = Sn_PrintOutSgn;
		hl_rev( &tmpPrintOutSgn, sizeof(tmpPrintOutSgn) );
		//dstStr.Format( _T("%08X%08X%08X%08X%08X"), tmpZrkbn, tmpZroutSgn, tmpDispTabSgn, tmpPrintOutSgn, 0 );

		// 汎用サイン追加 19.09.03
		long tmpGenericSgn = Sn_GenericSgn;
		hl_rev ( &tmpGenericSgn, sizeof ( tmpGenericSgn ) );

// 20-0225 del -->
		//dstStr.Format ( _T( "%08X%08X%08X%08X%08X" ), tmpZrkbn, tmpZroutSgn, tmpDispTabSgn, tmpPrintOutSgn, tmpGenericSgn );
		////-------------------------------
		////dstStr.Format( _T("%08X%08X%08X%08X%08X"), tmpZrkbn, tmpZroutSgn, 0, 0, 0 );
		////<------------------------------
// 20-0225 del <--
// 20-0225 add -->
		if(Is10PerKikan() == TRUE)	{
			long	tmpPrintOutSgn2=0;
			tmpPrintOutSgn2 = Sn_PrintOutSgn2;
			hl_rev(&tmpPrintOutSgn2,sizeof(tmpPrintOutSgn2));
			dstStr.Format(_T("%08X%08X%08X%08X%08X%08X"),tmpZrkbn,tmpZroutSgn,tmpDispTabSgn,tmpPrintOutSgn,tmpGenericSgn,tmpPrintOutSgn2);
		}
		else	{
			dstStr.Format(_T("%08X%08X%08X%08X%08X"),tmpZrkbn,tmpZroutSgn,tmpDispTabSgn,tmpPrintOutSgn,tmpGenericSgn);
		}
// 20-0225 add <--
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入計算表 金額連動？('15.03.09)
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	金額連動中
//			FALSE	：	金額連動無し
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsSpcDataLinked( CDBNpSub *pDBNpSub )
{
	BOOL	bRt = FALSE;

	if( m_Util.IsSpcMaster(pDBNpSub) ){
		if( (Sn_Sign2&0x02) == 0x00 ){
			bRt = TRUE;
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// マイナンバーの取込('15.07.11)
//  zvol->kojin_hash からの取込
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		pTgdata		：	顧問先情報
//		ErrMsg		：	エラーメッセージ
//		sn_seq		：	履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::TakeInMynumber( CDBNpSub *pDBNpSub, TgPackData *pTgdata, CString &ErrMsg, int gyousyu_flg, int sn_seq/*=0*/ )
{
	int	nRt = -1;

	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return nRt;
	}
	ASSERT( pTgdata );
	if( pTgdata == NULL ){
		return nRt;
	}

	BOOL openFlg = FALSE;
	if( pDBNpSub->zvol == NULL ){
		if( pDBNpSub->VolumeOpen() ){
			return nRt;
		}
		openFlg = TRUE;
	}

	NTBMOD8_BeginProc( 0, NULL );

	NODATATBL ndtbl;
	NTBMOD8_InitNoDataTbl( 0, &ndtbl );
//--> '15.09.08 MOVE START
//	// 使用業種フラグ
//	ndtbl.gyousyu_flg |= gyousyu_flg;
//<-- '15.09.08 MOVE END

	NOHISTTBL nhtbl;
	SetNoHistory( &nhtbl, pDBNpSub, pTgdata );

	BOOL	IsUpdateKjhash = FALSE;
	BOOL	IsClearKjhash = FALSE;
	int		noSt = 0;
	for( ;; ){
		CString	filter;
		filter.Format( _T("Sn_SEQ = %d"), sn_seq );

		if( pDBNpSub->szsnk ){
			pDBNpSub->szsnk->Requery( filter );
		}
		else{
			if( pDBNpSub->SzsnkOpen(filter) ){
				nRt = -1;
			}
		}
		if( pDBNpSub->szsnk->st == -1 ){
			// 新規
			nRt = 0;
			break;
		}

		// 個人番号の有無を確認
		if( m_Util.IsBasicMst(pDBNpSub->zvol) == TRUE ){
			if( (noSt=GetNoData(pDBNpSub, &ndtbl)) == -1 ){
				nRt = -1;
				break;
			}
		}

		pDBNpSub->szsnk->Edit();

		if( m_Util.IsBasicMst(pDBNpSub->zvol) == TRUE ){
			// 個人番号不一致？
			int len = (int)strlen(ndtbl.no);
			if( (len==0) && (IsRightKjhash(pDBNpSub->zvol->kojin_hash)==FALSE) ){
				if( IsRightKjhash(pDBNpSub->szsnk->Sn_KOJIN_HASH) == TRUE ){
					IsClearKjhash = TRUE;
				}
			}
			else if( CompareKojinHash(pDBNpSub->szsnk->Sn_KOJIN_HASH, pDBNpSub->zvol->kojin_hash) ){
				// 個人番号ハッシュ更新
//-- '15.09.14 --
//				if( IsRightKjhash(pDBNpSub->szsnk->Sn_KOJIN_HASH) == TRUE ){
//					nhtbl.user_play |= NTBMOD_USER_PLAY_UPDATE;
//				}
//				else{
//					nhtbl.user_play |= NTBMOD_USER_PLAY_INSERT;
//				}
//---------------
				nhtbl.user_play |= NTBMOD_USER_PLAY_UPDATE;
//---------------
				pDBNpSub->szsnk->Sn_KOJIN_HASH.RemoveAll();
				pDBNpSub->szsnk->Sn_KOJIN_HASH.Copy( pDBNpSub->zvol->kojin_hash );
				IsUpdateKjhash = TRUE;
			}
		}
		else{
			// 法人番号
			pDBNpSub->szsnk->Sn_HOJIN_NO = pDBNpSub->zvol->hojin_no;
			// 法人番号付番
			pDBNpSub->szsnk->Sn_HOJIN_NO_EXT = pDBNpSub->zvol->hojin_no_ext;
		}

		pDBNpSub->szsnk->Update();

		if( IsClearKjhash ){
			if( ClearKjhashBySql(pDBNpSub, sn_seq, pTgdata) ){
				nRt = -1;
				break;
			}
			pDBNpSub->szsnk->Requery( filter );
		}

		if( IsUpdateKjhash ){
//--> '15.09.08 MOVE START
			// 使用業種フラグ
			ndtbl.gyousyu_flg |= gyousyu_flg;
//<-- '15.09.08 MOVE END
			// 個人番号の更新
			int st = NTBMOD8_SetNoDataTbl( 0, &ndtbl, &nhtbl );
			if( st == -1 ){
				NTBMOD8_GetErrorMessage( ErrMsg );
				nRt = -1;
				break;
			}
			st = NTBMOD8_UpdateNoDataTbl_LastTime( 0, &ndtbl );
			if( st == -1 ){
				NTBMOD8_GetErrorMessage( ErrMsg );
				nRt = -1;
				break;
			}
		}

		nRt = 0;
		break;
	}

	if( openFlg ){
		pDBNpSub->VolumeClose();
	}
	NTBMOD8_EndProc();

	if( nRt == 0 ){
		// 法人番号
		memset( Sn_HOJIN_NO, '\0', sizeof(Sn_HOJIN_NO) );
		if( pDBNpSub->szsnk->Sn_HOJIN_NO.GetLength() ){
			wsprintf( Sn_HOJIN_NO, _T("%s"), pDBNpSub->szsnk->Sn_HOJIN_NO.GetBuffer() );
		}
		// 法人番号付番
		memset( Sn_HOJIN_NO_EXT, '\0', sizeof(Sn_HOJIN_NO_EXT) );
		if( pDBNpSub->szsnk->Sn_HOJIN_NO_EXT.GetLength() ){
			wsprintf( Sn_HOJIN_NO_EXT, _T("%s"), pDBNpSub->szsnk->Sn_HOJIN_NO_EXT.GetBuffer() );
		}
		// 個人番号ハッシュ
		memset( Sn_KOJIN_HASH, '\0', sizeof(Sn_KOJIN_HASH) );
		if( IsRightKjhash(pDBNpSub->szsnk->Sn_KOJIN_HASH) == TRUE ){
			int size = (int)pDBNpSub->szsnk->Sn_KOJIN_HASH.GetSize();
			if( size > sizeof(Sn_KOJIN_HASH) ){
				size = sizeof(Sn_KOJIN_HASH);
			}
			for( int i=0; i<size; i++ ){
				Sn_KOJIN_HASH[i] = pDBNpSub->szsnk->Sn_KOJIN_HASH.GetAt(i);
			}
		}
	}

	return nRt;
}

//-----------------------------------------------------------------------------
// 履歴情報のセット
//-----------------------------------------------------------------------------
// 引数	pNohist		：	履歴のセット先
//		pDBNpSub	：	書込み元マスター
//		pTgdata		：	書込み元顧問先情報
//-----------------------------------------------------------------------------
void CSnHeadData::SetNoHistory( NOHISTTBL *pNohist, CDBNpSub *pDBNpSub, TgPackData *pTgdata )
{
	ASSERT( pNohist );
	if( pNohist == NULL ){
		return;
	}
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return;
	}

	NTBMOD8_InitNoHistoryTbl( 0, pNohist );

	// 履歴操作方法
	pNohist->user_play |= NTBMOD_USER_PLAY_APPLI;

	// マスター情報
	pNohist->mst_code = pDBNpSub->zvol->v_cod;
	pNohist->mst_apno = pDBNpSub->zvol->apno;
	memset( pNohist->mst_nam, '\0', sizeof(pNohist->mst_nam) );
//-- '15.08,11 --
//	memmove( pNohist->mst_nam, pDBNpSub->zvol->c_nam, min(sizeof(pNohist->mst_nam), pDBNpSub->zvol->c_nam.GetLength()) );
//---------------
//2018.08.21 UPDATE START
	//int len =  m_Util.MixSpaceCutLength( (char *)Sn_DIHYO, sizeof(Sn_DIHYO) );
	//if( len ){
	//	memmove( pNohist->mst_nam, Sn_DIHYO, sizeof(Sn_DIHYO) );
	//}
	//else{
	//	memmove( pNohist->mst_nam, pDBNpSub->zvol->c_nam, min(sizeof(pNohist->mst_nam), pDBNpSub->zvol->c_nam.GetLength()) );
	//}

		memmove( pNohist->mst_nam, pDBNpSub->zvol->c_nam, min(sizeof(pNohist->mst_nam), pDBNpSub->zvol->c_nam.GetLength()) );
	memmove( pNohist->tgt_char, Sn_DIHYO, sizeof(Sn_DIHYO) );

	CString mst_nam_str,kessan_str;
	
	mst_nam_str = pNohist->mst_nam;
	mst_nam_str = mst_nam_str.TrimRight();
	GetString_KessanKikan( &kessan_str );
	strcpy_s( pNohist->mst_nam, sizeof(pNohist->mst_nam), mst_nam_str + kessan_str );
//2018.08.21 UPDATE END
//---------------
	strcpy_s( pNohist->gy_nam, sizeof(pNohist->gy_nam), _T("消費税申告書") );

	// 顧問先情報
	if( pTgdata == NULL ){
		return;
	}
//-- '16.01.30 --
//	if( pTgdata->TgCode != 0 ){
//---------------
	if( pTgdata->TgCode != -1 ){
//---------------
		pNohist->kmn_code = pTgdata->TgCode;
		pNohist->kmn_hkkbn = pTgdata->HKkbn;
		memmove( pNohist->kmn_nam, pTgdata->TgName, sizeof(pTgdata->TgName) );
	}
}

//-----------------------------------------------------------------------------
// マイナンバーデータの初期化('15.07.30)
//-----------------------------------------------------------------------------
// 引数	pNodata	：	初期化対象の個人番号保存テーブル
//-----------------------------------------------------------------------------
void CSnHeadData::InitNodatatbl( NODATATBL *pNodata )
{
	ASSERT( pNodata );
	if( pNodata == NULL ){
		return;
	}

	NTBMOD8_InitNoDataTbl( 0, pNodata );
	// 使用業種フラグ
	pNodata->gyousyu_flg |= NTBMOD_GYFLG_SYOHI;
}

//-----------------------------------------------------------------------------
// 正しい個人番号ハッシュ値？
//-----------------------------------------------------------------------------
// 引数		kj_hash	：	チェック対象の個人番号ハッシュ値
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	正しい
//			FALSE	：	不正
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsRightKjhash( CByteArray& kj_hash )
{
	int size = (int)kj_hash.GetCount();
	if( size != MYNUM_KOJIN_HASH_SIZE ){
		return FALSE;
	}

	BOOL is_hash_empty = TRUE;
	for( int i=0; i<size; i++ ){
		int zero = 0;
		if( kj_hash[i] != zero ){
			is_hash_empty = FALSE;
			break;
		}
	}
	if( is_hash_empty == TRUE ){
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 正しい個人番号ハッシュ値？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	正しい
//			FALSE	：	不正
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsRightKjhash()
{
	CByteArray tmpKj_hash;
	for( int i=0; i<MYNUM_KOJIN_HASH_SIZE; i++ ){
		tmpKj_hash.Add( Sn_KOJIN_HASH[i] );
	}

	return IsRightKjhash( tmpKj_hash );
}

//-----------------------------------------------------------------------------
// SQL 文による個人番号ハッシュのデータクリア
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		seq			：	対象履歴シーケンス
//		pTgdata		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::ClearKjhashBySql( CDBNpSub *pDBNpSub, int seq, TgPackData *pTgdata )
{
	ASSERT( pDBNpSub );
	ASSERT( pDBNpSub->m_database );

	CString	sql;
	sql.Format( _T("update szsnk set Sn_KOJIN_HASH=NULL where Sn_SEQ=%d"), seq );

	pDBNpSub->m_database->BeginTrans();
	try{
		pDBNpSub->m_database->ExecuteSQL( sql );
	}
	catch( CDBException *pde ){
		pDBNpSub->m_database->Rollback();
		pde->Delete();
		return -1;
	}
	pDBNpSub->m_database->CommitTrans();

	// 削除履歴
	if( pTgdata ){
		NOHISTTBL nhtbl;
		SetRirekiInfo( pTgdata, pDBNpSub, &nhtbl );
		nhtbl.user_play |= NTBMOD_USER_PLAY_APPLI;
		nhtbl.user_play |= NTBMOD_USER_PLAY_DELETE;

		CNoHistoryTblArray nhtblary;
		nhtblary.Add( nhtbl );
		NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );
	}

	return 0;
}

//----------------------------------------------------------------------------
// 履歴情報レコードのデータセット('15.09.14)
//----------------------------------------------------------------------------
// 引数	pTgdata	：	顧問先情報
//		zmsub	：	財務クラス
//		pNhtbl	：	セット先履歴情報
//-----------------------------------------------------------------------------
void CSnHeadData::SetRirekiInfo( TgPackData* pTgdata, CDBNpSub* pDBNpSub, NOHISTTBL* pNhtbl )
{
	NTBMOD8_InitNoHistoryTbl( 0, pNhtbl );
	pNhtbl->mst_code = pDBNpSub->zvol->v_cod;
	pNhtbl->mst_apno = pDBNpSub->zvol->apno;
	memset( pNhtbl->mst_nam, '\0', sizeof(pNhtbl->mst_nam) );
//2018.08.21 UPDATE START
	//if( pDBNpSub->szsnk->Sn_DIHYO.GetLength() ){
	//	wsprintf( pNhtbl->mst_nam, _T("%s"), pDBNpSub->szsnk->Sn_DIHYO );
	//}
	//else{
	//	wsprintf( pNhtbl->mst_nam, _T("%s"), pDBNpSub->zvol->c_nam );
	//}

		wsprintf( pNhtbl->mst_nam, _T("%s"), pDBNpSub->zvol->c_nam );
	wsprintf( pNhtbl->tgt_char, _T("%s"), pDBNpSub->szsnk->Sn_DIHYO );

	CString mst_nam_str,kessan_str;
	
	mst_nam_str = pNhtbl->mst_nam;
	mst_nam_str = mst_nam_str.TrimRight();
	GetString_KessanKikan( &kessan_str );
	strcpy_s( pNhtbl->mst_nam, sizeof(pNhtbl->mst_nam), mst_nam_str + kessan_str );
//2018.08.21 UPDATE END
	strcpy_s( pNhtbl->gy_nam, sizeof(pNhtbl->gy_nam), _T("消費税申告書") );

	// 顧問先情報
//-- '16.01.30 --
//	pNhtbl->kmn_code = pTgdata->TgCode;
//	pNhtbl->kmn_hkkbn = pTgdata->HKkbn;
//	memmove( pNhtbl->kmn_nam, pTgdata->TgName, sizeof(pTgdata->TgName) );
//---------------
	if( pTgdata->TgCode != -1 ){
		pNhtbl->kmn_code = pTgdata->TgCode;
		pNhtbl->kmn_hkkbn = pTgdata->HKkbn;
		memmove( pNhtbl->kmn_nam, pTgdata->TgName, sizeof(pTgdata->TgName) );
	}
//---------------
}

//-----------------------------------------------------------------------------
// 個人番号管理データの取得
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		ndtbl		：	個人番号管理データの取得先
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了（該当データあり）
//			1		：	正常終了（該当データ無し）
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::GetNoData( CDBNpSub *pDBNpSub, void *ndtbl )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}
	ASSERT(ndtbl);
	if( ndtbl == NULL ){
		return -1;
	}
	NODATATBL	*pNdtbl = (NODATATBL*)ndtbl;

	CString hash_all;
	int size = (int)pDBNpSub->zvol->kojin_hash.GetSize();
	for( int i=0; i<size; i++ ){
		BYTE hush = pDBNpSub->zvol->kojin_hash.GetAt(i);

		CString num;
		num.Format( _T("%02x"), hush );
		hash_all += num.Left(1) + num.Right(1);
	}
	// ハッシュ値を個人番号保存テーブルにセット
	memmove( pNdtbl->arrow, hash_all, sizeof(pNdtbl->arrow) );

	// 個人番号取得
	int st = NTBMOD8_GetNoDataTbl( 0, pNdtbl );
	if( st < 0 ){
		// エラー
		// データの有無の判定は可能？
		return -1;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 個人番号ハッシュ値の比較
//-----------------------------------------------------------------------------
// 引数		h1		：	比較対象１
//			h2		：	比較対象２
//-----------------------------------------------------------------------------
// 返送値	0		：	一致
//			0以外	：	不一致
//-----------------------------------------------------------------------------
int CSnHeadData::CompareKojinHash( CByteArray &h1, CByteArray &h2 )
{
	int nRt = 0;

	int len1 = (int)h1.GetSize();
	int len2 = (int)h2.GetSize();

	if( len1 == len2 ){
		for( int i=0; i<len1; i++ ){
			if( h1[i] != h2[i] ){
				nRt = 1;
				break;
			}
		}
	}
	else{
		nRt = 1;
	}

	return nRt;
}

//-----------------------------------------------------------------------------
// 個人番号ハッシュ値に変更有？('15.07.11)
//-----------------------------------------------------------------------------
// 引数		h1		：	比較対象の個人ハッシュ値
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	変更有
//			FALSE	：	変更無し
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsChangedKojinHash( CByteArray &h1 )
{
	BOOL bRt = FALSE;

	CByteArray	h2;
	for( int i=0; i<MYNUM_KOJIN_HASH_SIZE; i++ ){
		h2.Add( Sn_KOJIN_HASH[i] );
	}

	if( CompareKojinHash(h1, h2) ){
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// マイナンバーデータの登録
//-----------------------------------------------------------------------------
// 引数	pDBNpSub：	DBを保持している財務クラスのポインタ
//		pTgdata	：	顧問先情報
//		sn_seq	：	履歴シーケンス
//		kbFlg	：	登録対象	■□□□ □□■■
//					（D0:法人, D1:個人, D7:履歴を登録するかどうか(ON:登録, OFF:登録しない)）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSnHeadData::RegistMynumber( CDBNpSub *pDBNpSub, TgPackData *pTgdata, int sn_seq, unsigned char kbFlg )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	if( (kbFlg&(0x01|0x02)) == 0x00 ){
		return 0;
	}

	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );

	if( pDBNpSub->szsnk ){
		pDBNpSub->szsnk->Requery( filter );
	}
	else{
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
	}

	BOOL	IsCrearKjhash = FALSE;

	// 更新
	pDBNpSub->szsnk->Edit();
	if( kbFlg&0x01 ){	// 法人
		// 法人番号
		pDBNpSub->szsnk->Sn_HOJIN_NO = Sn_HOJIN_NO;
		// 法人番号付番
		pDBNpSub->szsnk->Sn_HOJIN_NO_EXT = Sn_HOJIN_NO_EXT;
	}

	// 履歴情報
	NOHISTTBL nhtbl;
	SetNoHistory( &nhtbl, pDBNpSub, pTgdata );

	if( kbFlg&0x02 ){	// 個人
		// 個人番号ハッシュ
		if( IsRightKjhash() ){
			if( pDBNpSub->szsnk->Sn_KOJIN_HASH.GetSize() ){
				if( IsChangedKojinHash(pDBNpSub->szsnk->Sn_KOJIN_HASH) ){
					nhtbl.user_play |= NTBMOD_USER_PLAY_UPDATE;
				}
			}
			else{
				nhtbl.user_play |= NTBMOD_USER_PLAY_INSERT;
			}

			pDBNpSub->szsnk->Sn_KOJIN_HASH.RemoveAll();
			for( int i=0; i<MYNUM_KOJIN_HASH_SIZE; i++ ){
				pDBNpSub->szsnk->Sn_KOJIN_HASH.Add( Sn_KOJIN_HASH[i] );
			}
		}
		else{
			if( IsRightKjhash(pDBNpSub->szsnk->Sn_KOJIN_HASH) ){
				IsCrearKjhash = TRUE;
				nhtbl.user_play |= NTBMOD_USER_PLAY_DELETE;
			}
		}
	}
	pDBNpSub->szsnk->Update();

	if( IsCrearKjhash ){
//-- '15.09.14 --
//		ClearKjhashBySql( pDBNpSub, sn_seq );
//---------------
		ClearKjhashBySql( pDBNpSub, sn_seq, NULL );
//---------------
		pDBNpSub->szsnk->Requery( filter );
	}

	// 履歴更新
	if( kbFlg&0x80 ){
		CNoHistoryTblArray	nhtblary;
		nhtblary.Add( nhtbl );
		if( nhtbl.user_play&(NTBMOD_USER_PLAY_UPDATE|NTBMOD_USER_PLAY_INSERT|NTBMOD_USER_PLAY_DELETE) ){
			NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 特定課税仕入表示必要有り？('15.08.20)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	表示必要有り
//			FALSE	：	表示必要無し
//-----------------------------------------------------------------------------
BOOL CSnHeadData::IsNeedTkkzDisp()
{
	BOOL	bRt = FALSE;

	if( m_DispTabSgn&0x01 ){
		bRt = TRUE;
	}

	return bRt;
}

//2015.10.20 INSERT START
//-----------------------------------------------------------------------------
//処理概要：個人番号・法人番号の取得
//
//引数　　：個人番号・法人番号格納領域
//返送値　：0…正常終了　-1…エラー　1…空白
//-----------------------------------------------------------------------------
int CSnHeadData::GetMyNumber( CString &str )
{
	
	if( IsSoleProprietor() == FALSE ){
		// 法人番号
		str.Format( _T("%s"), Sn_HOJIN_NO );
	}else{
		// 個人番号
		NODATATBL ndtbl;				// これを使用するのは個人番号でnewされたときのみ
//2016.03.23 INSERT START
		::ZeroMemory( &ndtbl, sizeof(ndtbl));
//2016.03.23 INSERT END
		NTBMOD8_InitNoDataTbl( 0, &ndtbl );

		unsigned char	ss[64]={0};
		memmove( ss, Sn_KOJIN_HASH, sizeof(Sn_KOJIN_HASH) );

////--> '15.07.31 INS START
//		if( IsEmptyKjhash(m_pSnHeadData->Sn_KOJIN_HASH, sizeof(m_pSnHeadData->Sn_KOJIN_HASH)) == TRUE ){
//			data.Empty();
//			return 0;
//		}
////<-- '15.07.31 INS END

		CString	hash_all;
		for( int i=0; i<16; i++ ){
			CString	num;
			num.Format( _T("%02x"), ss[i] );
			hash_all += num.Left(1) + num.Right(1);
		}
		// ハッシュ値を個人番号保存テーブルにセット
		memmove( ndtbl.arrow, hash_all, sizeof(ndtbl.arrow) );

		// 個人番号取得
		int st = NTBMOD8_GetNoDataTbl( 0, &ndtbl );
		if( st < 0 ){
			CString err;
			NTBMOD8_GetErrorMessage(err);
			if(!err.IsEmpty()){
				ICSMessageBox(err);
//				data.Empty();
				return -1;
			}
		}

//2015.12.03 INSERT START
		if( st == 1 ){ //該当データなし(ハッシュ値有 法人個人番号無)
			return 1;
		}
//2015.12.03 INSERT END

		str.Format( _T("%s"), ndtbl.no );
	}

	if( str.GetLength() == 0 ){
		return 1; //あり得ない？
	}

	return 0;
}

//-----------------------------------------------------------------------------
//処理概要：個人番号・法人番号の取得(ハイフン編集)
//　　　　　個人番号 0000-0000-0000　法人番号 0-0000-0000-0000
//
//引数　　：個人番号・法人番号格納領域
//返送値　：0…正常終了　-1…エラー　1…空白
//-----------------------------------------------------------------------------
int CSnHeadData::GetMyNumber_hyphen( CString &str )
{
	int ret = -1;
	CString edit_str;

	ret = GetMyNumber( edit_str );

	if( ret != 0 ){
		return ret;
	}

	str.Empty();

	if( IsSoleProprietor() == FALSE ){
		//法人番号
		str += edit_str.Mid(0,1);
		str += '-';
		str += edit_str.Mid(1,4);
		str += '-';
		str += edit_str.Mid(5,4);
		str += '-';
		str += edit_str.Mid(9,4);
		if(str.GetLength() != 16){
			return -1;
		}
	}else{
		//個人番号
		str += edit_str.Mid(0,4);
		str += '-';
		str += edit_str.Mid(4,4);
		str += '-';
		str += edit_str.Mid(8,4);
		if(str.GetLength() != 14){
			return -1;
		}
	}

	return 0;

}
//2015.10.20 INSERT END

//-----------------------------------------------------------------------------
// 法人・個人番号欄付き出力？('16.07.11)
//  期間外の期間（課税期間の開始日が平成28年1月1日以前）であっても、番号欄付きの
//  用紙で出力するかどうかの選択。
//  通常の期間（課税期間の開始日が平成28年1月1日以降）の時には、その期間が優先
//-----------------------------------------------------------------------------
// 返送値	true	：	番号欄付き
//			false	：	通常版
//-----------------------------------------------------------------------------
bool CSnHeadData::IsPrintWithMynoHonpyo()
{
	if( Sn_TYPE&0x80 ){
		return true;
	}
	else{
		return false;
	}
}

//2018.08.21 INSERT START
int CSnHeadData::GetString_KessanKikan( CString* kessan )
{
	CString nengo1,nengo2,y1,y2,m1,m2;
	//nengo = "平成";

	CVolDateDB	voldate;

	char	yy=0, mm=0, dd=0;
	int		g = 0;
	//GetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd, &g );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){
		y1.Format( ("%2x"), yy );
		m1.Format( ("%2x"), mm );
	}

	voldate.db_vd_NoToStrGen( g, nengo1 );

	yy=0, mm=0, dd=0;
	g = 0;
	//GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd, &g );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){ 
		y2.Format( ("%2x"), yy );
		m2.Format( ("%2x"), mm );
	}

	voldate.db_vd_NoToStrGen( g, nengo2 );

	CString str;
	//*kessan = "(" + nengo + y1 + "年" + m1 + "月～" + nengo + y2 + "年" + m2 + "月" + ")";
	*kessan = "(" + nengo1 + y1 + "年" + m1 + "月～" + nengo2 + y2 + "年" + m2 + "月" + ")";

	return 0;
}
//2018.08.21 INSERT END

//-----------------------------------------------------------------------------
// 年月日データの取得（改元対応）
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	取得元データの種類
//		pYy		：	年
//		pMm		：	月
//		pDd		：	日
//-----------------------------------------------------------------------------
void CSnHeadData::GetYmdDataGen( EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd, int *pG/*=NULL*/ )
{
	long	*ptmpYmd = NULL;
	int		gengo	 = 0;

	if( dsDtype == ID_ICSSH_KAZEIKIKAN_FROM ){		// Sn_KDAYS：課税期間　自
		ptmpYmd = &Sn_KDAYS;
	}
	else if( dsDtype == ID_ICSSH_KAZEIKIKAN_TO ){	// Sn_KDAYE：課税期間　至
		ptmpYmd = &Sn_KDAYE;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_FROM ){	// Sn_MDAYS：中間申告の計算期間　自
		ptmpYmd = &Sn_MDAYS;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_TO ){		// Sn_MDAYE：中間申告の計算期間　至
		ptmpYmd = &Sn_MDAYE;
	}
	else if( dsDtype == ID_ICSSH_PRESENT_DATE ){	// Sn_PRDAY：提出年月日
		ptmpYmd = &Sn_PRDAY;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_FROM ){		// 決算期間 自
		ptmpYmd = &m_ss_ymd;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_TO ){		// 決算期間 至
		ptmpYmd = &m_ee_ymd;
	}
	else{
		// 該当 元データ無し
	}

	if( (*ptmpYmd) == 0 ){
		return;
	}

	// 西暦を和暦に変換
	int	dYear = 0;
	if((*ptmpYmd) / 10000){
		CVolDateDB	voldate;
		int tmpMon = ((*ptmpYmd) % 10000) / 100;
		if( tmpMon ){
			int tmpDay = ((*ptmpYmd) % 10000) % 100;
			if( tmpDay ){
				voldate.db_datecnvGen(0, (*ptmpYmd), &gengo, &dYear, 0, 0);
				dYear = (dYear / 10000);
			}
			else{
				voldate.db_datecnvGen(0, (*ptmpYmd), &gengo, &dYear, 0, 1);
				dYear = (dYear / 100);
			}
		}
		else{
			voldate.db_datecnvGenEx(0, (*ptmpYmd), &gengo, &dYear, 0, 2);
		}
	}
	m_Arith.int_bcd( (void*)pYy, dYear, 1 );

	// 月
	int dMon = 0;
	dMon = ((*ptmpYmd) % 10000) / 100;
	m_Arith.int_bcd( (void*)pMm, dMon, 1 );

	// 日
	int dDay = 0;
	dDay = (*ptmpYmd) % 100;
	m_Arith.int_bcd( (void*)pDd, dDay, 1 );

	if( pG != NULL ){
		*pG = gengo;
	}
}

//-----------------------------------------------------------------------------
// 年月日データの取込（改元対応）
//-----------------------------------------------------------------------------
// 引数	dsDtype	：	データの取込先
//		type	：	年？月？日？
//		data	：	BCDデータ
//-----------------------------------------------------------------------------
// 暫定で平成固定となっています
void CSnHeadData::SetYmdDataGen( EnumIdIcsShDateType dsDtype, int type, char data, int gengo/*=4*/ )
{
	int	rtYmd = 0;

	// 取得データの変換
	int nData = m_Arith.bcd_int(&data, 1);

	if( type == 0 ){		// 年
		if( data == 0x00 ){	// 取消を意味する！
			nData = 0;
		}
		else{
			//和暦を西暦に変換
			int	dYear = 0;
			int dummy;
			CVolDateDB	voldate;
			voldate.db_datecnvGen(gengo, nData, &dummy, &dYear, 1, 2);
			nData = dYear;
		}
	}
	else if( type == 1 ){	//[Y] 月
		if( (nData<1) || (12<nData) ){
			// 不正データ
		}
	}
	else if( type == 2 ){	//[Y] 日
		if( (nData<1) || (31<nData) ){
			// 不正データ
		}
	}

	long	*ptmpYmd = NULL;

	if( dsDtype == ID_ICSSH_KAZEIKIKAN_FROM ){		// Sn_KDAYS：課税期間　自
		ptmpYmd = &Sn_KDAYS;
	}
	else if( dsDtype == ID_ICSSH_KAZEIKIKAN_TO ){	// Sn_KDAYE：課税期間　至
		ptmpYmd = &Sn_KDAYE;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_FROM ){	// Sn_MDAYS：中間申告の計算期間　自
		ptmpYmd = &Sn_MDAYS;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_TO ){		// Sn_MDAYE：中間申告の計算期間　至
		ptmpYmd = &Sn_MDAYE;
	}
	else if( dsDtype == ID_ICSSH_PRESENT_DATE ){	// Sn_PRDAY：提出年月日
		ptmpYmd = &Sn_PRDAY;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_FROM ){		// 決算期間 自
		// 取り込みは未許可
		return;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_TO ){		// 決算期間 至
		// 取り込みは未許可
		return;
	}
	else{
		// 該当 元データ無し
	}

	if( type == 0 ){		// 年
		int	mmdd = (*ptmpYmd) % 10000;
		(*ptmpYmd) = mmdd + (nData * 10000);
	}
	else if( type == 1 ){	// 月
		int yy = (*ptmpYmd) / 10000;
		int dd = (*ptmpYmd) % 100;
		(*ptmpYmd) = (yy * 10000) + (nData * 100) + dd;
	}
	else if( type == 2 ){	// 日
		int yymm = (*ptmpYmd) / 100;
		(*ptmpYmd) = (yymm * 100) + nData;
	}
}

//-----------------------------------------------------------------------------
// 年月日データの取込（改元対応）
//-----------------------------------------------------------------------------
// 引数	dsDtype
//		mType	：	セットすべきメイン
//		yy
//		mm
//		dd
//		gengo
//-----------------------------------------------------------------------------
void CSnHeadData::SetYmdDataGen( EnumIdIcsShDateType dsDtype, int mType, char yy, char mm, char dd, int gengo/*=4*/ )
{
	// 取得データの変換
	int nYyData=0, nMmData=0, nDdData=0;
	if( (mType==0) || (mType==-1) ){
		if( yy == 0x00 ){	// 取消を意味する！
			nYyData = 0;
		}
		else{
			nYyData = m_Arith.bcd_int(&yy, 1);
		}
	}
	if( (mType==1) || (mType==-1) ){
		nMmData = m_Arith.bcd_int(&mm, 1);
		if( (nMmData<1) || (12<nMmData) ){
			// 不正データ
		}
	}
	if( (mType==2) || (mType==-1) ){
		nDdData = m_Arith.bcd_int(&dd, 1);
		if( (nDdData<1) || (31<nDdData) ){
			// 不正データ
		}
	}

	// 和暦の設定
	UCHAR	waYmd[4]={ 0 };
	if( yy != 0 ){
		waYmd[0] = yy;
	}
	if( mm != 0 ){
		waYmd[1] = mm;
	}
	if( dd != 0 ){
		waYmd[2] = dd;
	}

	int	type = -1;
	if( (yy!=0) && (mm!=0) && (dd!=0) ){
		type = 0;
	}
	else if( (yy!=0) && (mm!=0) ){
		type = 1;
	}
	else if( yy!=0 ){
		type = 2;
	}


	UCHAR	chkYmd[4]={ 0 };
	chkYmd[0] = 0x31;
	chkYmd[1] = 0x05;
	if( IsValidKaigen() && (gengo==0) ){
		if( (type==1) || (type==0) ){
			if( (waYmd[0]>chkYmd[0]) ||
				((waYmd[0]==chkYmd[0])&&(waYmd[1]>=chkYmd[1])) ){
				// 平成31年5月以降が入力された時→令和1年5月が入力されたと自動変換
				waYmd[0] -= 0x30;
			}
		}
	}

	// 和暦→西暦変換
	UCHAR	seYmd[4]={ 0 };
	int		tmpGengo=0;
	int		seYY=0;
	int		st = 0;
	if( type != -1 ){
		CVolDateDB	voldate;
		if( type == 2 ){
			st = voldate.db_datecnvGenEx(gengo, waYmd, &tmpGengo, seYmd, 1, type);
		}
		else{
			if( type == 1 ){
				waYmd[2] = 0x10;
				type = 0;
			}
			st = voldate.db_datecnvGen(gengo, waYmd, &tmpGengo, seYmd, 1, type);
		}
		if( st != -1 ){
			seYY = bcd_int(seYmd, 1);
			if( seYY >= 89 ){
				seYY += 1900;
			}
			else{
				seYY += 2000;
			}
		}
		else{
			// 不正データ
			seYY = -1;
		}
	}

	long	*ptmpYmd = NULL;

	if( dsDtype == ID_ICSSH_KAZEIKIKAN_FROM ){		// Sn_KDAYS：課税期間　自
		ptmpYmd = &Sn_KDAYS;
	}
	else if( dsDtype == ID_ICSSH_KAZEIKIKAN_TO ){	// Sn_KDAYE：課税期間　至
		ptmpYmd = &Sn_KDAYE;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_FROM ){	// Sn_MDAYS：中間申告の計算期間　自
		ptmpYmd = &Sn_MDAYS;
	}
	else if( dsDtype == ID_ICSSH_CHUUKAN_TO ){		// Sn_MDAYE：中間申告の計算期間　至
		ptmpYmd = &Sn_MDAYE;
	}
	else if( dsDtype == ID_ICSSH_PRESENT_DATE ){	// Sn_PRDAY：提出年月日
		ptmpYmd = &Sn_PRDAY;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_FROM ){		// 決算期間 自
		// 取り込みは未許可
		return;
	}
	else if( dsDtype == ID_ICSSH_KESSAN_TO ){		// 決算期間 至
		// 取り込みは未許可
		return;
	}
	else{
		// 該当 元データ無し
	}

	if( mType == 0 ){		// 年
		int	mmdd = (*ptmpYmd) % 10000;
		if( (seYY!=0) && (seYY!=-1) ){
			(*ptmpYmd) = mmdd + (seYY * 10000);
		}
		else{
			(*ptmpYmd) = mmdd;
		}
	}
	else if( mType == 1 ){	// 月
		int yy = (*ptmpYmd) / 10000;
		if( seYY == -1 ){
			yy = 0;
		}
		else if( seYY != 0 ){
			yy = seYY;
		}
		int dd = (*ptmpYmd) % 100;
		(*ptmpYmd) = (yy * 10000) + (nMmData * 100) + dd;
	}
	else if( mType == 2 ){	// 日
		int yy = (*ptmpYmd) / 10000;
		if( seYY == -1 ){
			yy = 0;
		}
		else if( seYY != 0 ){
			yy = seYY;
		}
		int mm = ((*ptmpYmd) % 10000) / 100;
		(*ptmpYmd) = (yy * 10000) + (mm * 100) + nDdData;
	}
	else if( mType == -1 ){
		if( seYY == -1 ){
			seYY = 0;
		}
		(*ptmpYmd) = (seYY * 10000) + (nMmData * 100) + nDdData;
	}
}

// 20-0225 add -->
//-----------------------------------------------------------------------------
// 10%対応以降の期間であるか（DBSyzShinMain CMainFrame::SetActViewVerと同じ判定）
//-----------------------------------------------------------------------------
// 引　数	なし
// 戻り値	TRUE:10%対応以降の期間
//-----------------------------------------------------------------------------
BOOL CSnHeadData::Is10PerKikan( void )
{
	BOOL	brv=FALSE;
	long	matsubi=0;

	if(IsTyukanShinkoku())	matsubi = Sn_MDAYE;
	else					matsubi = Sn_KDAYE;
	if(matsubi >= ICS_SH_10PER_REFORM_DAY)	{
		brv=TRUE;
	}
	return(brv);
}

//-----------------------------------------------------------------------------
// 従前の値を新エリアに移行
//-----------------------------------------------------------------------------
// 引　数	int		Sn_TYPE
//			int		Sn_PrintOutSgn
// 戻り値	int		Sn_PrintOutSgn2
//-----------------------------------------------------------------------------
//int CSnHeadData::PrintOutSgnConvert( int pSw1, int pSw2 )
//{
//	int		ret=0;
//
//	// 白紙
//	if(pSw1 & 0x01)	{
//		// 提出用カラー
//		if(pSw1 & 0x02)	{
//			// 申告書本表「提出用」
//			if(pSw2 & 0x01)	{
//				ret = ret | 0x01;	// D0
//				ret = ret | 0x02;	// D1
//			}
//			// 課税標準額等の内訳書「提出用」
//			if(pSw2 & 0x04)	{
//				ret = ret | 0x20;	// D5
//				ret = ret | 0x40;	// D6
//			}
//		}
//		// 提出用モノクロ
//		else	{
//			// 申告書本表「提出用」
//			if(pSw2 & 0x01)	{
//				ret = ret | 0x01;	// D0
//				ret = ret | 0x04;	// D2
//			}
//			// 課税標準額等の内訳書「提出用」
//			if(pSw2 & 0x04)	{
//				ret = ret | 0x20;	// D5
//				ret = ret | 0x80;	// D7
//			}
//		}
//		// 控用カラー
//		if(pSw1 & 0x04)	{
//			// 申告書本表「控用」
//			if(pSw2 & 0x02)	{
//				ret = ret | 0x01;	// D0
//				ret = ret | 0x08;	// D3
//			}
//			// 課税標準額等の内訳書「控用」
//			if(pSw2 & 0x08)	{
//				ret = ret | 0x20;	// D5
//				ret = ret | 0x100;	// D8
//			}
//		}
//		// 控用モノクロ
//		else	{
//			// 申告書本表「控用」
//			if(pSw2 & 0x02)	{
//				ret = ret | 0x01;	// D0
//				ret = ret | 0x10;	// D4
//			}
//			// 課税標準額等の内訳書「控用」
//			if(pSw2 & 0x08)	{
//				ret = ret | 0x20;	// D5
//				ret = ret | 0x200;	// D9
//			}
//		}
//	}
//	// ＯＣＲ用紙
//	else	{
//		// 申告書本表「提出用」
//		if(pSw2 & 0x01)	ret = ret | 0x01;	// D0
//		// 課税標準額等の内訳書「提出用」
//		if(pSw2 & 0x04)	ret = ret | 0x20;	// D5
//	}
//
//	return(ret);
//}
int CSnHeadData::PrintOutSgnConvert( int pSw1, int pSw2 )
{
	int		ret=0;

	// 白紙
	if(pSw1 & 0x01)	{
		// -------------------------------------------------
		// 提出用
		// -------------------------------------------------
		// 提出用カラー
		if(pSw1 & 0x02)	{
			ret |= 0x01;	// D0:提出用カラー
		}
		// 申告書本表「提出用」
		if(pSw2 & 0x01)	{
			ret |= 0x04;	// D2
			ret |= 0x08;	// D3
		}
		// 課税標準額等の内訳書「提出用」
		if(pSw2 & 0x04)	{
			ret |= 0x20;	// D5
			ret |= 0x40;	// D6
		}
		// -------------------------------------------------
		// 控　用
		// -------------------------------------------------
		// 控用カラー
		if(pSw1 & 0x04)	{
			ret |= 0x02;	// D1:控　用カラー
		}
		// 申告書本表「控　用」
		if(pSw2 & 0x02)	{
			ret |= 0x04;	// D2
			ret |= 0x10;	// D4
		}
		// 課税標準額等の内訳書「控　用」
		if(pSw2 & 0x08)	{
			ret |= 0x20;	// D5
			ret |= 0x80;	// D7
		}
	}
	// ＯＣＲ用紙
	else	{
		ret |= 0x01;	// D0:ON  提出用カラー
		//ret &= ~0x02;	// D1:OFF 控用モノクロ
		// 申告書本表
		if(pSw2 & 0x01)	ret |= 0x04;	// D2
		// 課税標準額等の内訳書
		if(pSw2 & 0x04)	ret |= 0x20;	// D5
	}

	return(ret);
}
// 20-0225 add <--
