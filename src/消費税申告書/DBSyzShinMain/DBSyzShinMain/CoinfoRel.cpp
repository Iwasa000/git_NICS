#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
// 会社コードの変換
//-----------------------------------------------------------------------------
// 引数	pCod	：	コードの変換先
//-----------------------------------------------------------------------------
void CMainFrame::CnvCono( char *pCod )
{
	char	buf[128] = {0};
	sprintf_s( buf, sizeof(buf), _T("%08d"), m_pZmSub->zvol->v_cod );
	l_pack( pCod, buf, 8 );

}

//-----------------------------------------------------------------------------
// 会社情報クラスの初期化
//-----------------------------------------------------------------------------
// 引数	errmsg	：	エラーメッセージ
//		type	：	呼出タイプ（0:マスター起動＆書込み, 1:呼出）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了（処理継続）
//			1	：	正常終了（処理中断）
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::InitCCoinfCtl( CString &errmsg, int type )
{
	// クラス構築
	bool	isNeedOpen = false;
	if( m_pCoinfCtl == NULL ){
		if( (m_pCoinfCtl = new CCoinfCtl) == NULL ){
			return -1;
		}
		isNeedOpen = true;
	}

//-- '18.12.17 移動 --
//	// マスターデバイス
//	char	MDev[4]={0};
//	char	MDdm[4]={0};
//	if( PCOM_GetString( _T("MasterDevice"), MDev, MDdm ) == -1 ) {
//		errmsg =  _T("マスターデバイスが取得できません。" );
//		return -1;
//	}
//	sprintf_s( MDdm, sizeof(MDdm), _T("%s:"), MDev );
//
//	if( m_pCoinfCtl->OpenFile(1, MDdm ) == FALSE ){
//		delete m_pCoinfCtl;
//		m_pCoinfCtl = NULL;
//		return 1;
//	}
//--------------------
	_TgtLimit_Kmn( true );
	if( isNeedOpen ){

//-- '18.12.17 移動 --
		// マスターデバイス
		char	MDev[4]={0};
		char	MDdm[4]={0};
		if( PCOM_GetString( _T("MasterDevice"), MDev, MDdm ) == -1 ) {
			errmsg =  _T("マスターデバイスが取得できません。" );
			return -1;
		}
		sprintf_s( MDdm, sizeof(MDdm), _T("%s:"), MDev );
//--------------------
		m_pCoinfCtl->TgtLimit_Kmn( LM_MAININFO|LM_KINYU|LM_TANTOZEIRI|LM_UPDATE );
		if( m_pCoinfCtl->OpenFile(1, MDdm ) == FALSE ){
			delete m_pCoinfCtl;
			m_pCoinfCtl = NULL;
			return 1;
		}
	}

	int st = m_rtIsRegistMcode;
	if( m_rtIsRegistMcode == -999 ){

		char	cono[4] = {0};
		CnvCono( cono );

		// 業種と会社コードを使用して、顧問先情報に登録されているかを確認！
		m_rtIsRegistMcode = m_pCoinfCtl->IsRegistMcode( 1, (m_pZmSub->zvol->apno&0xff), 0, (LPBYTE)cono, NULL );
		st = m_rtIsRegistMcode;
	}
	if( type == 0 ){
		if( st != 1 ){
//			m_pCoinfCtl->CloseFile();
//			delete m_pCoinfCtl;
//			m_pCoinfCtl = NULL;
			return 1;
		}
		else{
			m_Finish = 1;
		}
	}
	else if( type == 1 ){
		if( st == -2 ){
//			m_pCoinfCtl->CloseFile();
//			delete m_pCoinfCtl;
//			m_pCoinfCtl = NULL;
			return 1;
		}
	}

	return 0;
}

#ifdef _20150811_EXP_
//-----------------------------------------------------------------------------
// 会社情報取り出し条件パケットの初期化
//-----------------------------------------------------------------------------
// 引数	NCpar	：	取り出し条件パラメータ
//		CoRec	：	会社情報レコード
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::InitCoinfPacket( NCOF_PAR &NCpar, CINF_REC &CoRec )
{
	char	buf[128] = {0};
	char	cono[4] = {0};

	NCpar.cpPare = this;
	NCpar.cpDtyp = 1;
	NCpar.cpMtyp = 0;
	NCpar.cpApno = (m_pZmSub->zvol->apno&0xff);
	sprintf_s( buf, sizeof(buf), _T("%08d"), m_pZmSub->zvol->v_cod );
	CnvCono( cono );//会社コードをセット
	memmove( NCpar.cpCode, cono, 4 );

	NCpar.cpFunc |= 0x60;	// 金融機関オプション選択及びメッセージを表示

	// 顧問先選択時の法人・個人選択の絞込みサイン設定
//	if( m_SnHeadData.IsSoleProprietor() == FALSE ){
	if( m_Util.IsBasicMst(m_pZmSub->zvol) == FALSE ){
		NCpar.cpHKsg = 1;	// 法人
	}
	else{
		NCpar.cpHKsg = 2;	// 個人
	}

	long& item_flg0 = NCpar.cpEflg[0];
	long& item_flg1 = NCpar.cpEflg[1];

	item_flg0 = 0;
	item_flg0 |=0x00000004;		// 顧問先　氏名
	item_flg0 |=0x00000008;		// 〃　　　フリガナ
	if( m_Util.IsBasicMst(m_pZmSub->zvol) == FALSE ){
		item_flg0 |=0x00800000;	// 代表者　氏名
		item_flg0 |=0x01000000;	// 〃　　　フリガナ
	}
	else{
		item_flg0 |=0x00000080;	// 屋号
		item_flg0 |=0x00000100;	// 〃　　　フリガナ
	}
	item_flg1 |=0x00000002;		// 経理担当者
	item_flg1 |=0x00000010;		// 還付先の金融機関
	item_flg1 |=0x00000040;		// 税務署名＆税務署番号
	item_flg1 |=0x00000080;		// 整理番号
	item_flg1 |=0x00000400;		// 納税地　郵便番号
	item_flg1 |=0x00000800;		// 〃　　　上段
	item_flg1 |=0x00002000;		// 〃　　　下段
	item_flg1 |=0x00008000;		// 〃　　　電話番号

//--> '15.07.27 INS START
	if( m_Util.IsBasicMst(m_pZmSub->zvol) == FALSE ){
		if( m_SnSeq > 0 ){
			item_flg0 |=0x00000002;		// 法人番号
		}
	}
	else{
		item_flg1 |=0x00000001;		// 個人番号
	}
//<-- '15.07.27 INS END

	return 0;
}
#else
//-----------------------------------------------------------------------------
// 会社情報取り出し条件パケットの初期化
//-----------------------------------------------------------------------------
// 引数	NCpar		：	取り出し条件パラメータ
//		CoRec		：	会社情報レコード
//		IsDispKjno	：	個人番号の取込を表示するかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::InitCoinfPacket( NCOF_PAR &NCpar, CINF_REC &CoRec, BOOL IsDispKjno )
{
	char	buf[128] = {0};
	char	cono[4] = {0};

	NCpar.cpPare = this;
	NCpar.cpDtyp = 1;
	NCpar.cpMtyp = 0;
	NCpar.cpApno = (m_pZmSub->zvol->apno&0xff);
	sprintf_s( buf, sizeof(buf), _T("%08d"), m_pZmSub->zvol->v_cod );
	CnvCono( cono );//会社コードをセット
	memmove( NCpar.cpCode, cono, 4 );

	NCpar.cpFunc |= 0x60;	// 金融機関オプション選択及びメッセージを表示

	// 顧問先選択時の法人・個人選択の絞込みサイン設定
	if( m_Util.IsBasicMst(m_pZmSub->zvol) == FALSE ){
		NCpar.cpHKsg = 1;	// 法人
	}
	else{
		NCpar.cpHKsg = 2;	// 個人
	}

	long& item_flg0 = NCpar.cpEflg[0];
	long& item_flg1 = NCpar.cpEflg[1];

	item_flg0 = 0;
	item_flg0 |=0x00000004;		// 顧問先　氏名
	item_flg0 |=0x00000008;		// 〃　　　フリガナ
	if( m_Util.IsBasicMst(m_pZmSub->zvol) == FALSE ){
		item_flg0 |=0x00800000;	// 代表者　氏名
		item_flg0 |=0x01000000;	// 〃　　　フリガナ
//--> '15.12.15 INS START
		item_flg0 |=0x02000000;	// 代表者　郵便番号
		item_flg0 |=0x04000000;	// 〃　　　住所（上段）
		item_flg0 |=0x08000000;	// 〃　　　住所（下段）
//<-- '15.12.15 INS END
//--> '15.12.23 INS START
		item_flg0 |=0x10000000;	// 〃　　　電話番号
//<-- '15.12.23 INS END
	}
	else{
		item_flg0 |=0x00000080;	// 屋号
		item_flg0 |=0x00000100;	// 〃　　　フリガナ
	}
	item_flg1 |=0x00000002;		// 経理担当者
	item_flg1 |=0x00000010;		// 還付先の金融機関
	item_flg1 |=0x00000040;		// 税務署名＆税務署番号
	item_flg1 |=0x00000080;		// 整理番号
	item_flg1 |=0x00000400;		// 納税地　郵便番号
	item_flg1 |=0x00000800;		// 〃　　　上段
	item_flg1 |=0x00002000;		// 〃　　　下段
	item_flg1 |=0x00008000;		// 〃　　　電話番号

	if( m_Util.IsBasicMst(m_pZmSub->zvol) == FALSE ){
		if( m_SnSeq > 0 ){
			item_flg0 |=0x00000002;		// 法人番号
		}
	}
	else{
		if( IsDispKjno ){
			item_flg1 |=0x00000001;		// 個人番号
		}
	}

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// マスター起動時の総合会社情報の取り込み
//-----------------------------------------------------------------------------
// 引数	errmsg	：	エラーメッセージ
//-----------------------------------------------------------------------------
// 返送値	0	：	取込無し
//			1	：	取込有り
//-----------------------------------------------------------------------------
int CMainFrame::InitCoinfo( CString &errmsg )
{
//--> '14.06.18 MOVE START
//	if( m_isKndMster ){
//		return 0;
//	}
//<-- '14.06.18 MOVE END

	int	st = 0;

	// クラス構築
	st = InitCCoinfCtl( errmsg, 0 );
	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		return 0;
	}

//--> '14.06.18 MOVE START
	if( m_isKndMster ){
		return 0;
	}
//<-- '14.06.18 MOVE END

	// 会社情報の取り込み
	NCOF_PAR	NCpar = {0};
	CINF_REC	CoRec = {0};
//-- '15.08.11 --
//	InitCoinfPacket( NCpar, CoRec );
//---------------
	InitCoinfPacket( NCpar, CoRec, TRUE );
//---------------

	// 会社コード
	char	cono[4] = {0};
	CnvCono( cono );

	st = 0;	// [12'06.08]//取り込み有り
//	if( m_pCoinfCtl->GetCoinf( 0x50, &CoRec, &NCpar ) == 0 ){
	if( m_rtIsRegistMcode == 1 ){	// 登録有りでアクセス権限がある時だけ、取り込む！
		if( (NCpar.cpRflg[0]!=0) || (NCpar.cpRflg[1]!=0) ){
/*			if( GetHoujinKojin( 0, CoRec.ciHKkbn, m_Kojin ) ){
				m_pCoinfCtl->CloseFile();
				delete( m_pCoinfCtl );
				m_pCoinfCtl = NULL;
				return 0;
			}*/
		}
		st = 1;	// [12'06.08]//取り込み有り

		// マスターデバイスからサーバー名を取得
		char	server[256] = {0};
		strcpy_s( server, sizeof(server), GET_DBNAME() );

		typedef int (WINAPI *PFN_CTOM)( CTOM_PAR* pCpar, CDBZmSub* pDBZmSub );

		CString	Module;
		Module.Empty();

#if _MSC_VER >= 1900
#ifdef	_DEBUG
		Module.Format("C:\\ICSDEV\\DLL_DEBUG15\\ICS15_CTOM1B30D.dll");
#else
		Module.Format("%s\\ICS15_CTOM1B30.dll", m_PrgPath);
#endif
#else
#ifdef	_DEBUG
		Module.Format("D:\\()最新プロジェクト一式\\消費税プロジェクト\\Integrated\\CTOM1B30\\debug\\CTOM1B30D.dll");
		//		Module.Format("%c:\\Develop_V80\\Integrated\\CTOM1B30\\debug\\CTOM1B30D.dll", MDev );
#else
		Module.Format("%s\\CTOM1B30.dll", m_PrgPath);
#endif
#endif // _MSC_VER >= 1900

		HMODULE hModule = ::LoadLibrary( Module );

		CTOM_PAR	Cpar = {0};

		Cpar.cmPare		= this;
		Cpar.cmSknd		= 1;
		strcpy_s( Cpar.cmMdev, server );
		Cpar.cmMtyp		= 0;
		Cpar.cmApno		= (m_pZmSub->zvol->apno&0xff);
		memmove( &Cpar.cmCflg, &NCpar.cpRflg, sizeof( Cpar.cmCflg ) );
		memmove( Cpar.cmCcod, cono, 4 );

		Cpar.cmFkbn = NCpar.cpFkbn;
		Cpar.cmFkb2 = NCpar.cpFkb2;

		memmove( &Cpar.cmCinf, &CoRec, sizeof(CINF_REC) );

		PFN_CTOM func = ( PFN_CTOM )GetProcAddress( hModule,"PFN_CTOM" );
		if( !func ){
			FreeLibrary( hModule );
			m_pCoinfCtl->CloseFile();
			delete m_pCoinfCtl;
			m_pCoinfCtl = NULL;
			return 0;
		}
		st = func( &Cpar, m_pZmSub );
		FreeLibrary( hModule );
	}

//-- '18.12.17 --
// オープン→クローズの回数を減らして、最終的にクローズするように！
//	if( m_pCoinfCtl->CloseFile() == FALSE ){
//		delete m_pCoinfCtl;
//		m_pCoinfCtl = NULL;
//		return 0;
//	}
//	delete m_pCoinfCtl;
//	m_pCoinfCtl = NULL;
//---------------


	return st;
}

//-----------------------------------------------------------------------------
// 関連づいている顧問先情報の保存
//-----------------------------------------------------------------------------
// 引数	pCoRec	：	保存する顧問先情報
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::SaveLinkedCoInfo( CINF_REC *pCoRec )
{
	ASSERT( pCoRec );
	if( pCoRec == NULL ){
		return -1;
	}

	m_TgPDat.HKkbn = pCoRec->ciHKkbn;
	int code = bcd_int( pCoRec->ciCode8, 4 );
	m_TgPDat.TgCode = code;
	memmove( m_TgPDat.TgName, pCoRec->ciName, sizeof(m_TgPDat.TgName) );

	return 0;
}

//-----------------------------------------------------------------------------
// 既存のハッシュ値と等しい？('15.08.04)
//-----------------------------------------------------------------------------
// 引数		pCoRec	：	比較元の会社情報
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	等しい
//			FALSE	：	等しくない
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsEqualNowKjhash( CINF_REC *pCoRec )
{

	ASSERT( pCoRec );
	if( pCoRec == NULL ){
		return FALSE;
	}

	BOOL bRt = TRUE;

	unsigned char	tmpKjhash[64]={0};
	int		size = sizeof(pCoRec->ciKjnNo) / sizeof(pCoRec->ciKjnNo[0]);
	for( int i=0; i<size/2; i++ ){
		CString str_hash;
		str_hash = pCoRec->ciKjnNo[i*2];
		str_hash += pCoRec->ciKjnNo[(i*2)+1];

		BYTE hash = (BYTE)strtol(str_hash, NULL, 16);
		tmpKjhash[i] = hash;
	}

	// 比較
	int	size1 = sizeof(tmpKjhash);
	int	size2 = (int)m_pZmSub->zvol->kojin_hash.GetSize();
	int max = size1 < size2 ? size1 : size2;
	for( int i=0; i<max; i++ ){
		if( m_pZmSub->zvol->kojin_hash[i] != tmpKjhash[i] ){
			bRt = FALSE;
			break;
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 個人番号ハッシュ値の取込('15.08.04)
//-----------------------------------------------------------------------------
// 引数	pCoRec	：	取込元会社情報
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::TakeInMynumberFromCoinfo( CINF_REC *pCoRec )
{
	ASSERT( pCoRec );
	if( pCoRec == NULL ){
		return -1;
	}

//--> '15.08.24 INS START
	if( m_pZmSub->zvol->kojin_hash.GetSize() && (m_SnHeadData.IsRightKjhash(m_pZmSub->zvol->kojin_hash)==TRUE) ){
		// 削除履歴保存
		RegistDelHistory();
	}
//<-- '15.08.24 INS END

	// ボリュームラベル更新
	m_pZmSub->zvol->Edit();
	m_pZmSub->zvol->kojin_hash.RemoveAll();

	int		size = sizeof(pCoRec->ciKjnNo) / sizeof(pCoRec->ciKjnNo[0]);
	for( int i=0; i<size/2; i++ ){
		CString str_hash;
		str_hash = pCoRec->ciKjnNo[i*2];
		str_hash += pCoRec->ciKjnNo[(i*2)+1];

		BYTE hash = (BYTE)strtol(str_hash, NULL, 16);
		m_pZmSub->zvol->kojin_hash.Add( hash );
	}

	m_pZmSub->zvol->Update();

	// 消費税申告書更新
	CString	errmsg;
	m_SnHeadData.TakeInMynumber( m_pZmSub, &m_TgPDat, errmsg, (NTBMOD_GYFLG_ZAIMU|NTBMOD_GYFLG_SYOHI), 0/*最新期*/ );

	return 0;
}

//-----------------------------------------------------------------------------
// 関連づいている顧問先情報の保存('15.09.14)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::SaveRelCoInfo()
{
	if( m_isKndMster ){
		return 0;
	}

	CString	errmsg;
	int st = InitCCoinfCtl( errmsg, 0 );
	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		return 0;
	}

	// 不要な判定
//	if( m_pCoinfCtl == NULL ){
//		return 0;
//	}

	// 会社情報の取り込み
	NCOF_PAR	NCpar = {0};
	CINF_REC	CoRec = {0};
	InitCoinfPacket( NCpar, CoRec, FALSE );
	
	// 何も表示せずに取込のみ
	NCpar.cpFunc = 0;
	NCpar.cpFunc |= 0x100;
//--> '15.12.19 INS START
	NCpar.cpEflg[0] = 0;
	NCpar.cpEflg[1] = 0;
//<-- '15.12.19 INS END

//--> 20230324 顧問先情報の取込書込
	CINF_REC CoRec_MstDat;
	SetMasterInfo( CoRec_MstDat );
//<-- 20230324

	long flg = 0;
	if( m_pCoinfCtl->GetCoinf(flg, &CoRec, &NCpar) == 0 ){
		SaveLinkedCoInfo( &CoRec );
	}

//-- '18.12.17 --
// オープン→クローズの回数を減らして、最終的にクローズするように！
//	m_pCoinfCtl->CloseFile();
//	delete m_pCoinfCtl;
//	m_pCoinfCtl = NULL;
//---------------

	return 0;
}

//-----------------------------------------------------------------------------
// 会社情報から取り込み
//-----------------------------------------------------------------------------
// 引数	type	：	取込方法
//						0: 起動時で無条件取り込み
//						1: 起動時で新規登録ダイアログ表示
//						2: 任意取り込み
//						3: 顧問先確認
//		pWnd	：	親ウィンドウ
//-----------------------------------------------------------------------------
// 返送値	0	：	取り込み無し
//			1	：	取り込み有り
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetCoInfo( int type, CWnd *pWnd/*=NULL*/ )
{
	if( pWnd == NULL ){
		pWnd = this;
	}

	int st = 0;

	if( type != 2 ){
		if( m_isKndMster ){
			return 0;
		}
	}

	// クラス構築
	CString	errmsg;
	int calltype = type;
//	int calltype = 0;
	if( type == 2 ){
		calltype = 1;
	}
//--> '15.07.27 INS START
	else if( type == 3 ){
		calltype = 0;
	}
//<-- '15.07.27 INS END
	st = InitCCoinfCtl( errmsg, calltype );
	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		return 0;
	}

	// 会社情報の取り込み
	NCOF_PAR	NCpar = {0};
	CINF_REC	CoRec = {0};
//-- '15.08.11 --
//	InitCoinfPacket( NCpar, CoRec );
//---------------
	BOOL IsDispKjno = TRUE;
	if( type == 2 ){
//-- '15.12.14 --
//		if( MynoAccessPerm() == FALSE ){
//			IsDispKjno = FALSE;
//		}
//---------------
		if( MynoAccessPerm() == FALSE ){
			IsDispKjno = FALSE;
		}
		else if( m_SnSeq == 0 ){
			// 最新分は、財務と絶えず連動するので変更不可！
			IsDispKjno = FALSE;
		}
//---------------
	}
//--> '15.12.17 INS START
	else if( type == 1 ){
		// 新規作成時は財務と連動
		IsDispKjno = FALSE;
	}
//<-- '15.12.17 INS END
	InitCoinfPacket( NCpar, CoRec, IsDispKjno );
//---------------

	if( m_isNewMake ){
		memmove( &NCpar.cpCflg, &NCpar.cpEflg, sizeof(NCpar.cpCflg) );
	}

//--> 20230324 顧問先情報の取込書込
	CINF_REC CoRec_MstDat = {0};
	SetMasterInfo( CoRec_MstDat );
//<-- 20230324

	st = 0;	// [12'06.08]//取り込み有り
	long flg = 0x40;
//-- '15.07.27 --
//	if( type == 0 ){
//---------------
	if( (type==0) || (type==3) ){
//---------------
		flg |= 0x10;
	}
	if( m_pCoinfCtl->GetCoinf(flg, &CoRec, &NCpar) == 0 ){
		st = 1;	// [12'06.08]//取り込み有り

		char	WKbuf[128]={0}, Ascii[128]={0}, kana[128]={0}, kanj[128]={0};
		long& item_flg0 = NCpar.cpRflg[0];
		long& item_flg1 = NCpar.cpRflg[1];

//--> '15.07.27 INS START
		// 関連づいている顧問先情報の保存
		if( type == 3 ){
//---> '15.09.14 CUT START
//			SaveLinkedCoInfo( &CoRec );
//<--- '15.09.14 CUT END

//---> '15.08.04 INS START
			// 個人番号ハッシュ値の比較
			if( (CoRec.ciHKkbn==1) && m_SnHeadData.IsSoleProprietor() ){
				if( IsEqualNowKjhash(&CoRec) == FALSE ){
					// ハッシュ値の取込
					TakeInMynumberFromCoinfo( &CoRec );
				}
			}
//<--- '15.08.04 INS END

//-- '18.12.17 --
// オープン→クローズの回数を減らして、最終的にクローズするように！
//			m_pCoinfCtl->CloseFile();
//
//			delete m_pCoinfCtl;
//			m_pCoinfCtl = NULL;
//---------------
			//return 0;
		}
//<-- '15.07.27 INS END

//		if( GetHoujinKojin(0, CoRec.ciHKkbn, m_Kojin) ){
//--> '18.12.17:使用していないのでカット
//		if( 0 ){
//			m_pCoinfCtl->CloseFile();
//			delete m_pCoinfCtl;
//			m_pCoinfCtl = NULL;
//			return 0;
//		}
//<--

		// 整理番号
		if( item_flg1 & 0x00000080 ){
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			m_Arith.l_pack( WKbuf, CoRec.ciRegNo, 8 );
			memmove( m_SnHeadData.Sn_SEIRI, WKbuf, 4 );
		}
		// 税務署名
		if( item_flg1 & 0x00000040 )	{
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, CoRec.ciTaxName, 12 );
			memmove( m_SnHeadData.Sn_ZMSYO, WKbuf, 12 );
			
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, CoRec.ciTaxCode, 5 );
			if( (WKbuf[0]&0xff) == 0xff ){
				memset( WKbuf, 'F', 5 );
			}
			memmove( m_SnHeadData.Sn_TAXNO, WKbuf, 5 );
		}

		// 納税地のみ取得
		// 郵便番号
		if( item_flg1 & 0x00000400 ){
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, CoRec.ciTppPno, 8 );
			memmove( m_SnHeadData.Sn_ZIP, WKbuf, 8 );
		}
		// 納税地
		if( item_flg1 & 0x00002800 ){
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, CoRec.ciTppAd1, 40 );
			if( item_flg1 & 0x00000800 ){
				memmove( m_SnHeadData.Sn_NOZEI, WKbuf, 40 );
			}
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, CoRec.ciTppAd2, 40 );
			if( item_flg1 & 0x00002000 ){
				memmove( &m_SnHeadData.Sn_NOZEI[40], WKbuf, 40 );
			}
		}
		// 電話番号
		if( item_flg1 & 0x00008000 ){
			int	i, j, k;
			memset( Ascii, '\0', sizeof( Ascii ) );
			for( j = 0; j < 14; ++j ){
				Ascii[j] = 0x20;
			}
			for( j=k=0; j<14 || CoRec.ciTppTel[j]; ++j ){
				if( CoRec.ciTppTel[j] == 0x2d ||
						CoRec.ciTppTel[j] == 0xb0 ||
							CoRec.ciTppTel[j] == 0x20 ) {
					switch( k ){
						case 0 :
							k = ( (j < 6) ? j : 6 );
							memmove( &Ascii[0], &CoRec.ciTppTel[0], k );
							k = 1;	i = j + 1;
							continue;
						case 1 :
							if( (j-i) < 5 )	k = (j - i);
							else			k = 5;
							memmove( &Ascii[8],  &CoRec.ciTppTel[i],   k );
							memmove( &Ascii[13], &CoRec.ciTppTel[++j], 5 );
							k = 2;
							break;
						default:
							break;
					}
					if( k > 1 ){
						break;
					}
				}
			}
			if( !k ){
				memmove( Ascii, &CoRec.ciTppTel[0], (j > 16 ? 16 : j) );
			}

			memmove( m_SnHeadData.Sn_TELNO1, Ascii, 6 );
			memmove( m_SnHeadData.Sn_TELNO2, &Ascii[8], 4 );
			memmove( m_SnHeadData.Sn_TELNO3, &Ascii[13], 4 );
		}

		// 代表者氏名及び氏名又は屋号
		if( CoRec.ciHKkbn ){
			// 個人は会社名称を代表者氏名に転送

			// 代表者氏名
			if( item_flg0 & 0x00000004 ){
				memset( WKbuf, '\0', sizeof( WKbuf ) );
				memmove( WKbuf, CoRec.ciName, 40 );
				memmove( m_SnHeadData.Sn_DIHYO, WKbuf, 40 );
			}
			// 代表者氏名（ふりがな）漢字変換が必要
			if( item_flg0 & 0x00000008 ){
				memset( kana,'\0', sizeof( kana ) );
				memset( kanj,'\0', sizeof( kanj ) );
				memmove( kana, CoRec.ciKana, 34 );
				m_Util.AtoJis( kanj, kana, 34 );
				memset( m_SnHeadData.Sn_DIFRI, '\0', 68 );
				memmove( m_SnHeadData.Sn_DIFRI, kanj, strlen( kanj ) );
			}
			// 屋号又は氏名
			if( item_flg0 & 0x00000080 ){
				memset( WKbuf, '\0', sizeof( WKbuf ) );
				memmove( WKbuf, CoRec.ciYagou, 36 );
				memmove( m_SnHeadData.Sn_CONAM, WKbuf, 40 );
			}

			// 屋号又は氏名（ふりがな）漢字変換が必要
			if( item_flg0 & 0x00000100 ){
				memset( kana,'\0', sizeof( kana ) );
				memset( kanj,'\0', sizeof( kanj ) );
				memmove( kana, CoRec.ciYgKna, 24 );
				m_Util.AtoJis( kanj, kana, 24 );
				memset( m_SnHeadData.Sn_COFRI, '\0', 48 );
				memmove( m_SnHeadData.Sn_COFRI, kanj, strlen( kanj ) );
			}

//--> '15.07.29 INS START
			// 個人番号ハッシュ
			if( type == 2 ){
				if( MynoAccessPerm() == TRUE ){
					// 更新
					if( m_SnSeq > 0 ){
						if( item_flg1 & 0x00000001 ){

							// 削除確認
							BOOL IsUpdateKjhash = TRUE;
							if( m_SnHeadData.IsRightKjhash() ){
								if( m_pMainView ){
									if( m_pMainView->DeleteKjno() != RET_NO_CANCEL ){
										IsUpdateKjhash = FALSE;
									}
								}
							}

							if( IsUpdateKjhash ){
								memset( m_SnHeadData.Sn_KOJIN_HASH, '\0', sizeof(m_SnHeadData.Sn_KOJIN_HASH) );
								int size = sizeof(CoRec.ciKjnNo) / sizeof(CoRec.ciKjnNo[0]);
								for( int i=0; i<size/2; i++ ){
									CString str_hash;
									str_hash = CoRec.ciKjnNo[i*2];
									str_hash += CoRec.ciKjnNo[(i*2)+1];

									BYTE hash = (BYTE)strtol(str_hash, NULL, 16);
									m_SnHeadData.Sn_KOJIN_HASH[i] = hash;
								}
								// データセット
								TgPackData	tmpTgPDat;		// 一時関連元情報
								tmpTgPDat.HKkbn = CoRec.ciHKkbn;
								int code = bcd_int( CoRec.ciCode8, 4 );
								tmpTgPDat.TgCode = code;
								memmove( tmpTgPDat.TgName, CoRec.ciName, sizeof(tmpTgPDat.TgName) );
								m_SnHeadData.RegistMynumber( m_pZmSub, &tmpTgPDat, m_SnSeq, 0x82 );
							}
						}
					}
				}
			}
//<-- '15.07.29 INS END
		}
		else{
			// 法人は会社名称を屋号又は氏名に転送
			// 名称又は屋号
			if( item_flg0 & 0x00000004 ){
				memset( WKbuf, '\0', sizeof( WKbuf ) );
				memmove( WKbuf, CoRec.ciName, 40 );
				memmove( m_SnHeadData.Sn_CONAM, WKbuf, 40 );
			}
			// 名称又は屋号（ふりがな）漢字変換が必要
			if( item_flg0 & 0x00000008 ){
				memset( kana,'\0', sizeof( kana ) );
				memset( kanj,'\0', sizeof( kanj ) );
//2016.10.20 UPDATE START
				//memmove( kana, CoRec.ciKana, 34 );
				//m_Util.AtoJis( kanj, kana, 34 );
				//memset( m_SnHeadData.Sn_COFRI, '\0', 68 );
				memmove( kana, CoRec.ciKana, 40 );
				m_Util.AtoJis( kanj, kana, 40 );
				memset( m_SnHeadData.Sn_COFRI, '\0', 80 );
//2016.10.20 UPDATE END
				memmove( m_SnHeadData.Sn_COFRI, kanj, strlen( kanj ) );
			}
			// 代表者名
			if( item_flg0 & 0x00800000 ){
				memset( WKbuf, '\0', sizeof( WKbuf ) );
				memmove( WKbuf, CoRec.ciRepName, 24 );
				memmove( m_SnHeadData.Sn_DIHYO, WKbuf, 40 );
			}
			// 代表社名（ふりがな）
			if( item_flg0 & 0x01000000 ){
				memset( kana,'\0', sizeof( kana ) );
				memset( kanj,'\0', sizeof( kanj ) );
				memmove( kana, CoRec.ciRepKana, 24 );
				m_Util.AtoJis( kanj, kana, 24 );
				memset( m_SnHeadData.Sn_DIFRI, '\0', 48 );
				memmove( m_SnHeadData.Sn_DIFRI, kanj, strlen( kanj ) );
			}

//--> '15.07.29 INS START
			// 法人番号
			if( item_flg0 & 0x00000002 ){
				memmove( m_SnHeadData.Sn_HOJIN_NO, CoRec.ciHjnNo1, sizeof(CoRec.ciHjnNo1) );
				memmove( m_SnHeadData.Sn_HOJIN_NO_EXT, CoRec.ciHjnNo2, sizeof(CoRec.ciHjnNo2) );
			}
//<-- '15.07.29 INS END

//--> '15.12.15 INS START
			// 代表者郵便番号
			if( item_flg0 & 0x02000000 ){
				memset( WKbuf, '\0', sizeof( WKbuf ) );
				memmove( WKbuf, CoRec.ciRepPno, 8 );
				memmove( m_SnHeadData.Sn_DIZIP, WKbuf, 8 );
			}
			// 代表者住所
			if( item_flg0 & 0x0c000000 ){
				memset( WKbuf, '\0', sizeof( WKbuf ) );
				memmove( WKbuf, CoRec.ciRepAd1, 40 );
				if( item_flg0 & 0x04000000 ){
					memmove( m_SnHeadData.Sn_DIADR, WKbuf, 40 );
				}
				memset( WKbuf, '\0', sizeof( WKbuf ) );
				memmove( WKbuf, CoRec.ciRepAd2, 40 );
				if( item_flg0 & 0x08000000 ){
					memmove( &m_SnHeadData.Sn_DIADR[40], WKbuf, 40 );
				}
			}
//<-- '15.12.15 INS END
//--> '15.12.23 INS START
			// 代表者電話番号
			if( item_flg0 & 0x10000000 ){
				int	i, j, k;
				memset( Ascii, '\0', sizeof( Ascii ) );
				for( j = 0; j < 14; ++j ){
					Ascii[j] = 0x20;
				}
				for( j=k=0; j<14 || CoRec.ciRepTel[j]; ++j ){
					if( CoRec.ciRepTel[j] == 0x2d ||
							CoRec.ciRepTel[j] == 0xb0 ||
								CoRec.ciRepTel[j] == 0x20 ) {
						switch( k ){
							case 0 :
								k = ( (j < 6) ? j : 6 );
								memmove( &Ascii[0], &CoRec.ciRepTel[0], k );
								k = 1;	i = j + 1;
								continue;
							case 1 :
								if( (j-i) < 5 )	k = (j - i);
								else			k = 5;
								memmove( &Ascii[8],  &CoRec.ciRepTel[i],   k );
								memmove( &Ascii[13], &CoRec.ciRepTel[++j], 5 );
								k = 2;
								break;
							default:
								break;
						}
						if( k > 1 ){
							break;
						}
					}
				}
				if( !k ){
					memmove( Ascii, &CoRec.ciRepTel[0], (j > 16 ? 16 : j) );
				}

				memmove( m_SnHeadData.Sn_DITELNO1, Ascii, 6 );
				memmove( m_SnHeadData.Sn_DITELNO2, &Ascii[8], 4 );
				memmove( m_SnHeadData.Sn_DITELNO3, &Ascii[13], 4 );
			}
//<-- '15.12.23 INS END
		}

		// 経理担当社名
		if( item_flg1 & 0x00000002 ){
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, CoRec.ciAcName, 24 );
			memmove( m_SnHeadData.Sn_KEIRI, WKbuf, 40 );
		}

		// 郵便＆銀行情報の取得
		if( item_flg1 & 0x00000010 ){

			int Bank = -1;	// 銀行情報サイン
			int	Post = -1;	// 郵便情報サイン

			if( NCpar.cpFkbn != -1 ){
				if( CoRec.ciFinTbl[NCpar.cpFkbn].rfKubn == 1 ){	// 金融機関はゆうちょ銀行
					m_SnHeadData.Sn_BYsw = 1;
					Post = NCpar.cpFkbn;
					if( NCpar.cpFkb2 != -1 )	{
						if( CoRec.ciFinTbl[NCpar.cpFkb2].rfKubn > 1 ){
							Bank = NCpar.cpFkb2;
						}
					}
				}
				else{
					m_SnHeadData.Sn_BYsw = 0;
					Bank = NCpar.cpFkbn;
					if( NCpar.cpFkb2 != -1 ){
						if( CoRec.ciFinTbl[NCpar.cpFkb2].rfKubn == 1 ){
							Post = NCpar.cpFkb2;
						}
					}
				}
			}
			else{
				if( NCpar.cpFkb2 != -1 ){
					if( CoRec.ciFinTbl[NCpar.cpFkb2].rfKubn == 1 ){	// 金融機関はゆうちょ銀行
						m_SnHeadData.Sn_BYsw = 1;
						Post = NCpar.cpFkb2;
					}
					else{
						m_SnHeadData.Sn_BYsw = 0;
						Bank = NCpar.cpFkb2;
					}
				}
			}

			if( Bank != -1 ){
				// 銀行名称
				memmove( m_SnHeadData.Sn_BANK, CoRec.ciFinTbl[Bank].rfFnam, 20 );
				// 支店名称
				memmove( m_SnHeadData.Sn_SITEN, CoRec.ciFinTbl[Bank].rfSnam, 12 );
				// 預金種別rfDepo
				switch( CoRec.ciFinTbl[Bank].rfDepo ){
					case 1 :
						memmove( m_SnHeadData.Sn_YOKIN, "普通　　", 8 );
						break;
					case 2 :	
						memmove( m_SnHeadData.Sn_YOKIN, "当座　　", 8 );
						break;
					case 3 :
						memmove( m_SnHeadData.Sn_YOKIN, "納税準備", 8 );
						break;
					case 4 :
						memmove( m_SnHeadData.Sn_YOKIN, "貯蓄　　", 8 );	
						break;
					case 5 :	
						memmove( m_SnHeadData.Sn_YOKIN, CoRec.ciFinTbl[Bank].rfDnam, 8 );
						break;
					default:
						break;
				}
				// 口座番号
				m_Arith.l_pack( m_SnHeadData.Sn_KOZA, CoRec.ciFinTbl[Bank].rfKzno, 10 );
				// 銀行・金庫等
				switch( CoRec.ciFinTbl[Bank].rfKubn ){
					case 2 :	
						m_SnHeadData.Sn_FINANCE = 0x00;
						break;	// 銀行
					case 3 :	
						m_SnHeadData.Sn_FINANCE = 0x01;	
						break;	// 金庫
					case 4 :	
						m_SnHeadData.Sn_FINANCE = 0x02;	
						break;	// 組合
					case 5 :	
						m_SnHeadData.Sn_FINANCE = 0x03;
						break;	// 農協
					case 6 :	
						m_SnHeadData.Sn_FINANCE = 0x04;
						break;	// 漁協
					default:	
						break;
				}
				// 本店・支店等
				m_SnHeadData.Sn_CHAIN = CoRec.ciFinTbl[Bank].rfSkbn;
				switch( CoRec.ciFinTbl[Bank].rfSkbn ){
					case 1 :
						m_SnHeadData.Sn_CHAIN = 0x00;	
						break;	// 本店
					case 2 :
						m_SnHeadData.Sn_CHAIN = 0x01;	
						break;	// 支店
					case 3 :
						m_SnHeadData.Sn_CHAIN = 0x02;
						break;	// 本所
					case 4 :	
						m_SnHeadData.Sn_CHAIN = 0x03;
						break;	// 支所
					case 5 :	
						m_SnHeadData.Sn_CHAIN = 0x05;
						break;	// 出張所
					case 6 :	
						m_SnHeadData.Sn_CHAIN = 0x06;	
						break;	// 上記以外 ★[13'11.27]
					default:	
						m_SnHeadData.Sn_CHAIN = 0x04;	
						break;	// 無し
				}
			}
			// 郵便局情報
			if( Post != -1 ){
				// 郵便局名
				memmove( m_SnHeadData.Sn_YUBIN, CoRec.ciFinTbl[Post].rfFnam, 12 );
				// 郵便貯金の記号番号の取込み
				memset( m_SnHeadData.Sn_YUNUM1, '\0', 5 );
				memset( m_SnHeadData.Sn_YUNUM2, '\0', 8 );
				int   i, j, pos;
				for( i=j=pos=0; CoRec.ciFinTbl[Post].rfKzno[i]; ++i ){
					if( j < 0 ){
						if( (CoRec.ciFinTbl[Post].rfKzno[i]&0xff) != '-' ){
							continue;
						}
					}
					if( (CoRec.ciFinTbl[Post].rfKzno[i]&0xff) == '-' ){
						++i;	++pos;	j = 0;
					}
					if( (CoRec.ciFinTbl[Post].rfKzno[i]&0xff) == ' ' ){
						continue;
					}
					switch( pos ){
						case 0 :	//上5桁
							m_SnHeadData.Sn_YUNUM1[j] = CoRec.ciFinTbl[Post].rfKzno[i]&0xff;
							if( ++j == 5 ){
								j = -1;
							}
							break;
						case 1 :	//下8桁
							m_SnHeadData.Sn_YUNUM2[j] = CoRec.ciFinTbl[Post].rfKzno[i]&0xff;
							if( ++j == 8 ){
								j = -1;
							}
							break;
						default:
							break;
					}
				}
			}
		}
	}

//-- '18.12.17 --
// オープン→クローズの回数を減らして、最終的にクローズするように！
//	if( m_pCoinfCtl->CloseFile() == FALSE )	{
//		delete m_pCoinfCtl;
//		m_pCoinfCtl = NULL;
//		return 0;
//	}
//
//	delete m_pCoinfCtl;
//	m_pCoinfCtl = NULL;
//---------------

	// 総合会社情報の為の起動時の保存
	Copy_NowCoInfo();

	return st;
}

//-----------------------------------------------------------------------------
// 税務署コードのセット
//-----------------------------------------------------------------------------
// 引数	sign	：	全て１だが。。。
//					データが入っている時にチェックを行うかどうか？
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::SetTxationOffice( int sign )
{
	char	Number[20]={0}, SName[64]={0}, DName[64]={0}, TName[64]={0};
	int		len, st;

	// 電子申告のチェックが通らない時に、「１」で返すように記述していますが、実際には何で返してもOK。
	st = 1;

	// 税務署名
	memset( SName, '\0', sizeof(SName) );
	memset( DName, '\0', sizeof(DName) );
//	memmove( SName, Snh.Sn_ZMSYO, 12 );
	// 税務署番号
	memset( Number, '\0', sizeof(Number) );
//	memmove( Number, Snd.Sn_TAXNO, 5 );
	// 都道府県名取得
	memset( TName, '\0', sizeof(TName) );
	// 電子申告チェック
	if( (Number[0]&0xff) != 'F' )	{
		// 起動時のチェック
		if( sign && strlen(SName) && strlen(Number) ){
			return 0;
		}

		// 漢字スペースカットで税務署名が有り、税務署番号が無い時のチェック
		len = CutKanjiSpace( DName, SName, 12 );
		if( len && (strlen(Number)==0) ){
			st = Esel_TaxOfficeFromName( DName, Number, TName );
			if( st == 0 ){
//				memmove( Snd.Sn_TAXNO, Number, 5 );		// 税務署コード取得
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
				st = ICSMessageBox( msg, MB_YESNO, 0 );
				if( st == IDYES ){
					memset( &m_Eselect, '\0', sizeof(ESELECT) );
					m_Eselect.cd_kind = ESELECT_KIND_ZEIMUSYO;
					memmove( m_Eselect.nm_office, DName, len );
					if( Esel_SelectTaxOffice( &m_Eselect ) == IDOK ){
//						memmove( Snd.Sn_TAXNO, m_Eselect.cd_office, 5 );
/*- '13.12.30 -*/
//						memset( KBF, '\0', sizeof( KBF ) );
//						KjLn = (int)strlen( m_Eselect.nm_office );
//						memmove( KBF, m_Eselect.nm_office, KjLn );
/*-------------*/
						char	buf[512]={0};
						int		kjlen = (int)strlen( m_Eselect.nm_office );
						memmove( buf, m_Eselect.nm_office, kjlen );
/*-------------*/
//						memmove( Snh.Sn_ZMSYO, KBF, 12 );
						st = 0;
					}
					else{
//						memset( Snd.Sn_TAXNO, 'F', 5 );
					}
				}
				else{
//					memset( Snd.Sn_TAXNO, 'F', 5 );
				}
			}
		}
	}
	return st;
}

//-----------------------------------------------------------------------------
// 漢字スペースカット
//-----------------------------------------------------------------------------
// 引数	pDsKnji	：	スペースカット後
//		pSrKnji	：	スペースカット前
//		len		：	最大文字数
//-----------------------------------------------------------------------------
// 返送値　		：	文字数
//-----------------------------------------------------------------------------
int	CMainFrame::CutKanjiSpace( char *pDsKnji, char *pSrKnji, int len )
{
	ASSERT( pDsKnji );
	ASSERT( pSrKnji );

	char	buf[256] = {0};

	int		i, j;
	for( i=0, j=0; i<len; i++ ){
		if( *(pSrKnji+(i*2)) == 0x00 ){
			break;
		}
		if( strncmp( (pSrKnji+(i*2)), "　", 2 ) ){
			memmove( &buf[j*2], (pSrKnji+(i*2)), 2 );
			j++;
		}
	}
	if( j ){
		memmove( pDsKnji, buf, j*2 );
	}
	return ( j*2 );

}

//-----------------------------------------------------------------------------
// 半角スペースカット
//-----------------------------------------------------------------------------
// 引数	pBuf	：	対象バッファ
//		len		：	文字列長
//-----------------------------------------------------------------------------
void CMainFrame::CutAnkSpace( char *pBuf, int len )
{
	for( int i=(len-1); i>=0; i-- ){
		if( *(pBuf+i) == 0x00 ){
			continue;
		}
		if( *(pBuf+i) != 0x20 ){
			break;
		}
		*(pBuf+i) = 0x00;
	}
}

//-----------------------------------------------------------------------------
// 事務所情報の読込み
//-----------------------------------------------------------------------------
// 引数	wParam	：	
//		lParam	：	
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserReadZeirishi( WPARAM wParam, LPARAM lParam ) 
{
	int ret = ReadOfficeInfo( FALSE );

	return ret;
}

//-----------------------------------------------------------------------------
// 事務所情報の読込み
//-----------------------------------------------------------------------------
// 引数	isFirst	：	初期起動時？
//-----------------------------------------------------------------------------
// 返送値	1	：	事務所名あり
//			0	：	事務所名無し
//			2   ：	キャンセルのとき
//-----------------------------------------------------------------------------
int CMainFrame::ReadOfficeInfo( BOOL isFirst )
{
	// 会社コード
	char	cono[4] = {0};
	CnvCono( cono );

	// 税理士情報読み込み
	if( isFirst ){
		memset( &m_C_DATA, '\0', sizeof(COP_DATA) );
		memset( &m_O_DATA, '\0', sizeof(OFC_DATA) );
		memset( &m_Z_DATA, '\0', sizeof(ZEI_DATA) );
//--> '15.02.25 INS START
		memset( &m_ZeiriInfo, '\0', sizeof(SH_ZEIRI_INFO) );
		m_SnHeadData.m_Zrkbn = 0;
//<-- '15.02.25 INS END
	}

	memset( &m_oTax, '\0', sizeof(Old_TaxInf8) );
	memset( &m_nTax, '\0', sizeof(New_TaxInf8) );
	if( isFirst ){
		memmove( m_oTax.Cop_dat.CopCod, cono, 4 );
		m_oTax.Cop_dat.CopApn = 0x0001;
		m_oTax.Cop_dat.CopZim = 0x0001;
		m_nTax.dlgsw = m_oTax.dlgsw = 0x00;
		m_nTax.zno = m_oTax.zno = m_SnHeadData.SVzisq;
		m_nTax.subinf = m_SnHeadData.SubInf;
	}
	else{
		m_oTax.Cop_dat.CopCod[0] = 0xff;
		m_oTax.Cop_dat.CopCod[1] = 0xff;
		int	Sq;
		if( m_SnHeadData.SVzisq > 0 ){
			Sq = m_SnHeadData.SVzisq;
		}
		else{
			Sq = 0;
		}

		m_nTax.dlgsw = m_nTax.dlgsw = 0x07;
		m_nTax.zno = m_oTax.zno = Sq;
		m_nTax.subinf = m_SnHeadData.SubInf;
	}
// 24/04/03_327 cor -->
	//int	flg = 0x01;
// --------------------
	// 税務代理権限証書に連動させるため「住所」を選択できるようにする
	int	flg = 0x00;
// 24/04/03_327 cor <--
	int st = GetTaxAccountant8( &m_oTax, &m_nTax, flg, this );

	int ret = 0;

	if( st > 0 ){
		if( isFirst == FALSE ){
			m_ShinInfo.isExistOffice = TRUE;
		}
		if( m_oTax.taxsgn ){
			// 旧
			memmove( &m_C_DATA, &m_oTax.Cop_dat, sizeof(COP_DATA) );
			memmove( &m_O_DATA, &m_oTax.Ofc_dat, sizeof(OFC_DATA) );
			memmove( &m_Z_DATA, &m_oTax.Zei_dat, sizeof(ZEI_DATA) );
			m_SnHeadData.SVzisq = (char )st;
			memmove( m_Z_DATA.z_zeinam, m_Z_DATA.z_zeinam, 40 );
			memmove( m_O_DATA.o_telno, m_Z_DATA.ji_denwa, 30 );
//--> '15.02.25 INS START
			memset( &m_ZeiriInfo, '\0', sizeof(SH_ZEIRI_INFO) );
			m_SnHeadData.m_Zrkbn = 0;
//<-- '15.02.25 INS END
		}
		else if( m_nTax.taxsgn ){
			// 新
			m_SnHeadData.SVzisq = m_nTax.zno;
			m_SnHeadData.SubInf = m_nTax.subinf;
			// 事務所名
			memmove( m_O_DATA.o_ofcnam, m_nTax.Jim_dat.rj_ofcnam, 40 );	
			// 税理士氏名
			memmove( m_O_DATA.o_zeinam, m_nTax.Zeiri_dat.rz_zeinam, 40 );
			memmove( m_Z_DATA.z_zeinam, m_nTax.Zeiri_dat.rz_zeinam, 40 );	
			// 電話番号
			char	TelNo1[10]={0},
					TelNo2[10]={0},
					TelNo3[10]={0},
					stel[64]={0},
					ptel[64]={0};
			int		len1, len2, len3;

			if( m_nTax.subinf&0x02 ){
				memmove( TelNo1, m_nTax.Zeiri_dat.rz_tel1, 6 );
				memmove( TelNo2, m_nTax.Zeiri_dat.rz_tel2, 4 );
				memmove( TelNo3, m_nTax.Zeiri_dat.rz_tel3, 4 );
			}
			else{
				memmove( TelNo1, m_nTax.Jim_dat.rj_tel1, 6 );
				memmove( TelNo2, m_nTax.Jim_dat.rj_tel2, 4 );
				memmove( TelNo3, m_nTax.Jim_dat.rj_tel3, 4 );
			}
			CutAnkSpace( TelNo1, 6 );
			CutAnkSpace( TelNo2, 4 );
			CutAnkSpace( TelNo3, 4 );
			len1 = (int)strlen( TelNo1 );
			len2 = (int)strlen( TelNo2 );
			len3 = (int)strlen( TelNo3 );
			if( len1 || len2 || len3 ){
				int i=0;
				if( !len1 ){
					strcat_s( ptel, sizeof(ptel), "    -" );
				}
				else{
					strcat_s( ptel, sizeof(ptel), TelNo1 );
					i = (int)strlen( ptel );
					ptel[i] = '-';
					ptel[i+1] = 0x00;
				}
				if( !len2 ){
					strcat_s( ptel, sizeof(ptel), "    -" );
				}
				else{
					strcat_s( ptel, sizeof(ptel), TelNo2 );
					i = (int)strlen( ptel );
					ptel[i] = '-';
					ptel[i+1] = 0x00;
				}
				if( !len3 ){
					strcat_s( ptel, sizeof(ptel), "    " );
				}
				else{
					strcat_s( ptel, sizeof(ptel), TelNo3 );
				}
//--> '15.01.24 INS START
				memset( m_O_DATA.o_telno, '\0', sizeof(m_O_DATA.o_telno) );
				memset( m_Z_DATA.ji_denwa, '\0', sizeof(m_Z_DATA.ji_denwa) );
//--> '15.01.24 INS END
				memmove( m_O_DATA.o_telno, ptel, strlen( ptel ) );
				memmove( m_Z_DATA.ji_denwa, ptel, strlen( ptel ) );
			}

//--> '15.02.25 INS START
			memset( &m_ZeiriInfo, '\0', sizeof(SH_ZEIRI_INFO) );
			m_ZeiriInfo.sign1 |= 0x01;
			memmove( &m_ZeiriInfo.zeiridata, &m_nTax.Zeiri_dat, sizeof(R_ZEIRI) );
			m_SnHeadData.m_Zrkbn = m_nTax.Zeiri_dat.rz_zeiKbn;
//<-- '15.02.25 INS END
		}
		ret = 1;
	}
	else{
		if( (st==-2) || (st==0) || (st==-1) ){
			if( isFirst == FALSE ){
				memset( &m_C_DATA, '\0', sizeof(COP_DATA) );
				memset( &m_O_DATA, '\0', sizeof(OFC_DATA) );
				memset( &m_Z_DATA, '\0', sizeof(ZEI_DATA) );
				m_ShinInfo.isExistOffice = FALSE;
//--> '15.02.25 INS START
				memset( &m_ZeiriInfo, '\0', sizeof(SH_ZEIRI_INFO) );
				m_SnHeadData.m_Zrkbn = 0;
//<-- '15.02.25 INS END
			}
			m_SnHeadData.SVzisq = 0x00;
		}
		else{
			if( st == -3 ){//キャンセルのとき
				ret = 2;
			}
			else{
				ret = 0;
			}
		}
	}

//	SyzShin_UpDate();
	UpdateSyzShin();

	return ret;
}

//-----------------------------------------------------------------------------
// 事務所情報の読込み
//-----------------------------------------------------------------------------
// 引数	seq		：	税理士番号
//		subinf	：
//-----------------------------------------------------------------------------
// 返送値	
// 
//-----------------------------------------------------------------------------
int CMainFrame::ReadOfficeInfo( int seq, int subinf )
{
	int		i;
	// 税理士情報読み込み
	memset( &m_C_DATA, '\0', sizeof(COP_DATA) );
	memset( &m_O_DATA, '\0', sizeof(OFC_DATA) );
	memset( &m_Z_DATA, '\0', sizeof(ZEI_DATA) );
//--> '15.02.25 INS START
	memset( &m_ZeiriInfo, '\0', sizeof(SH_ZEIRI_INFO) );
	m_SnHeadData.m_Zrkbn = 0;
//<-- '15.02.25 INS END

	R_JIMUNAME JimData = {0};
	R_ZEIRI ZeiDataN = {0};
	R_HEAD Rhed = {0};
	
	int res = GetJimuandZeiDataNEW8( seq, subinf, &m_O_DATA, &m_Z_DATA, &JimData, &ZeiDataN, &Rhed, NULL);

	if( res == 1 ){
		// 新
		// 事務所名
		memmove( m_O_DATA.o_ofcnam, JimData.rj_ofcnam, 40 );	
		// 税理士氏名
		memmove( m_O_DATA.o_zeinam, ZeiDataN.rz_zeinam, 40 );
		memmove( m_Z_DATA.z_zeinam, ZeiDataN.rz_zeinam, 40 );	
		// 電話番号
		char	TelNo1[10] = {0};
		char	TelNo2[10] = {0};
		char	TelNo3[10] = {0};
		char	stel[64] = {0};
		char	ptel[64] = {0};
		int		len1, len2, len3;
		if( subinf&0x02 ){
			memmove( TelNo1, ZeiDataN.rz_tel1, 6 );
			memmove( TelNo2, ZeiDataN.rz_tel2, 4 );
			memmove( TelNo3, ZeiDataN.rz_tel3, 4 );
		}
		else{
			memmove( TelNo1, JimData.rj_tel1, 6 );
			memmove( TelNo2, JimData.rj_tel2, 4 );
			memmove( TelNo3, JimData.rj_tel3, 4 );
		}
		m_Util.AnkSpaceCut( TelNo1, 6 );
		m_Util.AnkSpaceCut( TelNo2, 4 );
		m_Util.AnkSpaceCut( TelNo3, 4 );
		len1 = (int)strlen( TelNo1 );
		len2 = (int)strlen( TelNo2 );
		len3 = (int)strlen( TelNo3 );
		if( len1 || len2 || len3 ){
			if( !len1 ){
				strcat_s( ptel, sizeof( ptel ), _T("    -") );
			}
			else{
				strcat_s( ptel, sizeof( ptel ), TelNo1 );
				i = (int)strlen( ptel );
				ptel[i] = '-';
				ptel[i+1] = 0x00;
			}
			if( !len2 ){
				strcat_s( ptel, sizeof( ptel ), _T("    -") );
			}
			else{
				strcat_s( ptel, sizeof( ptel ), TelNo2 );
				i = (int)strlen( ptel );
				ptel[i] = '-';
				ptel[i+1] = 0x00;
			}
			if( !len3 ){
				strcat_s( ptel, sizeof( ptel ), _T("    ") );
			}
			else{
				strcat_s( ptel, sizeof( ptel ), TelNo3 );
			}
			memmove( m_O_DATA.o_telno, ptel, strlen( ptel ) );
			memmove( m_Z_DATA.ji_denwa, ptel, strlen( ptel ) );
		}
//--> '15.02.25 INS START
		m_ZeiriInfo.sign1 |= 0x01;
		memmove( &m_ZeiriInfo.zeiridata, &ZeiDataN, sizeof(R_ZEIRI) );
		m_SnHeadData.m_Zrkbn = ZeiDataN.rz_zeiKbn;
//<-- '15.02.25 INS END
	}
	else if( res == 0 ){
		memmove( &m_C_DATA, &m_oTax.Cop_dat, sizeof( COP_DATA ) );
		memmove( &m_O_DATA, &m_oTax.Ofc_dat, sizeof( OFC_DATA ) );
		memmove( &m_Z_DATA, &m_oTax.Zei_dat, sizeof( ZEI_DATA ) );
		memmove( m_Z_DATA.z_zeinam, m_Z_DATA.z_zeinam, 40 );
		memmove( m_O_DATA.o_telno, m_Z_DATA.ji_denwa, 30 );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 進捗情報の初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::InitProgressInfo()
{
	m_isAbleProgress = FALSE;
	m_isPrintEtaxFinish = FALSE;

	char	MDev[4]={0};
	char	MDdm[4]={0};
	if( PCOM_GetString( _T("MasterDevice"), MDev, MDdm ) == -1 ) {
//		errmsg =  _T("マスターデバイスが取得できません。" );
		return -1;
	}

	// 進捗処理可能チェック
	if( GetShincyokuSign((MDev[0]&0xff)) == 1 ){
		m_isAbleProgress = TRUE;
//		if( Snd.Sn_Sign2 & 0x80 ){
		if( m_pZmSub->szsnk->Sn_Sign2 & 0x80 ){
			m_isPrintEtaxFinish = TRUE;
			m_FinishSw = 1;	// 20230614 一括印刷の場合、OpenZmtbl内だと取得が早すぎるため、owntbから印刷情報を取得しているこのタイミングでサインをセットする。
		}
		else{
			m_isPrintEtaxFinish = FALSE;
			m_FinishSw = 0; // 20230614
		}
	}

	return 0;
}

void CMainFrame::OnButtonF0()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	ProgressAdminister();
}

void CMainFrame::OnUpdateButtonF0(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_pMainView == NULL ){
		return;
	}

	ICSReportRow*	pReportRow = NULL;	
	int	FocusRow = 0;
	if( (pReportRow=m_pMainView->m_PastReport.GetFocusedRow()) && ((FocusRow=pReportRow->GetIndex())>=0) ){
		;
	}
//	if( ( (m_pMainView->m_Progress==0) || m_swFork ){
//		pCmdUI->Enable( FALSE );
//	}
	if( m_nowTabIdx == 2 ){
		pCmdUI->Enable( FALSE );
	}
}

//-----------------------------------------------------------------------------
// 進捗記録
//-----------------------------------------------------------------------------
void CMainFrame::ProgressAdminister()
{
	char	Bymd[4] = {0};
	char	Aymd[20] = {0};
	unsigned char	year = 0x00;
	unsigned char	dmme = 0x00;
	int		date = 0;
	char	yymmdd[3] = {0};
	int		gengo=0;
	CString	strGengo;

	if( !(m_SnHeadData.Sn_SKKBN%2) ){
		m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		if( (yymmdd[0]==0) || (yymmdd[1]==0) || (yymmdd[2]==0) ){
			ICSMessageBox( "正しい中間申告期間を入力してください。" );
			return;
		}
		m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		if( (yymmdd[0]==0) || (yymmdd[1]==0) || (yymmdd[2]==0) ){
			ICSMessageBox( "正しい中間申告期間を入力してください。" );
			return;
		}
	}

	CProgress_Mod	ProgMod;

	if( ProgMod.CProgress_Iniz() == 0 )	{

		memset( &m_Progdata, '\0', sizeof( struct ProgData ) );
		// 法人・個人
		if( m_SnHeadData.IsSoleProprietor() ){
			m_Progdata.Komon_HKkbn = 1;	// 個人
		}
		else{
			m_Progdata.Komon_HKkbn = 0;	// 法人
		}
		m_Progdata.Komon_Code = -1;
		// 納税地
		memmove( m_Progdata.Komon_Add1, m_SnHeadData.Sn_NOZEI, 40 );
		memmove( m_Progdata.Komon_Add2, &m_SnHeadData.Sn_NOZEI[40], 40 );

		if( m_SnHeadData.IsSoleProprietor() ){
			// 氏名
			memmove( m_Progdata.Komon_Nam, m_SnHeadData.Sn_DIHYO, 40 );
			// 内容１
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
			m_VolDate.db_vd_NoToStrGen( gengo, strGengo );
			sprintf_s( m_Progdata.Naiyou_1, sizeof( m_Progdata.Naiyou_1 ), "%s%x年分", strGengo.GetBuffer(), (yymmdd[0]&0xff) );
		}
		else{
			// 氏名
			memmove( m_Progdata.Komon_Nam, m_SnHeadData.Sn_CONAM, 40 );
			// 内容１
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
			m_VolDate.db_vd_NoToStrGen( gengo, strGengo );
			sprintf_s( m_Progdata.Naiyou_1, sizeof( m_Progdata.Naiyou_1 ), "%s%x年%x月期", strGengo.GetBuffer(), (yymmdd[0]&0xff), (yymmdd[1]&0xff) );
		}
		// 税理士情報
		if( m_ShinInfo.isExistOffice ){
			m_Progdata.Z_ID = (long)m_SnHeadData.SVzisq;
			memmove( m_Progdata.Z_Nam, m_O_DATA.o_zeinam, 40 );
		}
		else{
			m_Progdata.Z_ID = -1;	
		}
		m_Progdata.Z_Kind = -1;
		
		int ret = GetShincyokuAdminSign(2);									// システムデータから日付を持ってくるサイン
		if( ret == 1 ){
			m_Progdata.SysTimeGet = (SYS_TIME_1GOU);
		}
		else if(ret == 2){
			m_Progdata.SysTimeGet = (SYS_TIME_2GOU);
		}
		else{
			m_Progdata.SysTimeGet = (SYS_TIME_1GOU) | (SYS_TIME_2GOU);
		}
		
		if( m_SnHeadData.IsMiddleProvisional() == FALSE ){
			if( m_SnHeadData.Sn_ZeiHou & 0x01 ){
				m_Progdata.SysTimeGet |= SYS_TIME_DAIRI;
			}
			if( m_SnHeadData.Sn_ZeiHou & 0x02 ){
				m_Progdata.Tenp_Flg = 3;
			}
		}

		strcpy_s( m_Progdata.m_pd_gyid, sizeof( m_Progdata.m_pd_gyid ), _T("GK000240") );	// 業務区分
		switch( m_SnHeadData.Sn_SKKBN ){
			case 1 :
				if( m_SnHeadData.Sn_Syuusei & 0x02 ){
					if( m_SnHeadData.Sn_Syuusei & 0x04 ){	// 更正の申出
						strcpy_s( m_Progdata.m_pd_syid, sizeof( m_Progdata.m_pd_gyid ), "SK999102" );	
					}
					else{									// 更正の請求
						strcpy_s( m_Progdata.m_pd_syid, sizeof( m_Progdata.m_pd_gyid ), "SK999100" );	
					}
				}
				else{										// 確定申告	
					strcpy_s( m_Progdata.m_pd_syid, sizeof( m_Progdata.m_pd_gyid ), "SK000100" );
				}
				break;
			case 2 :	// 中間申告（仮決算）
				strcpy_s( m_Progdata.m_pd_syid, sizeof( m_Progdata.m_pd_gyid ), "SK000020" );	
				break;
			case 3 :	// 修正確定	
				strcpy_s( m_Progdata.m_pd_syid, sizeof( m_Progdata.m_pd_gyid ), "SK000101" );
				break;			
			case 4 :	// 修正中間	
				strcpy_s( m_Progdata.m_pd_syid, sizeof( m_Progdata.m_pd_gyid ), "SK000021" );	
				break;
			default:	// その他
				strcpy_s( m_Progdata.m_pd_syid, sizeof( m_Progdata.m_pd_gyid ), "SK999999" );
				break;	
		}
		m_Progdata.pd_bw_sign = -1;																				// 青白区分
		m_Progdata.pd_etax_sign = 0;																				// D0:ON電子申告サイン D1:ONインポートデータの申告
	//  strcpy( m_Progdata.pd_etax_recno, "9999999999999" );														// 電子申告受付番号
		if( m_SnHeadData.SVkazei&0x0f ){
			m_Progdata.pd_kazei_kbn = 1;						// 消費税課税方式区分
		}
	//	m_Progdata.pd_nenbun =																					// 年分及び年月分
	//	m_Progdata.pd_jigyo_symd =																				// 事業年度	自
	//	m_Progdata.pd_jigyo_eymd =																				// 事業年度	至
		if( !(m_SnHeadData.Sn_SKKBN%2) ){
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
			memmove( Bymd, yymmdd, 3 );	
		}
		else{
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
			memmove( Bymd, yymmdd, 3 );	
		}
//		shin_datecnv( Bymd[0], &year, 1 );	Bymd[0] = year;						/* 和暦→西暦 */
		// 和暦→西暦変換
		int		tmpGengo=0;
		UCHAR	tmpYear[4]={ 0 };
		_db_datecnvGen(gengo, (UCHAR*)Bymd, &tmpGengo, tmpYear, 1, 0);
		Bymd[0] = tmpYear[0];

		m_Arith.l_unpac( Aymd, Bymd, 6 );
		date = atoi( Aymd );
		m_Progdata.pd_kazei_symd	= ( date + 20000000 );															// 課税期間 自
		if( !(m_SnHeadData.Sn_SKKBN%2) ){
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
			memmove( Bymd, yymmdd, 3 );	
		}
		else{
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
			memmove( Bymd, yymmdd, 3 );	
		}
//		shin_datecnv( Bymd[0], &year, 1 );	Bymd[0] = year;						/* 和暦→西暦 */
		// 和暦→西暦変換
		tmpGengo=0;
		memset(tmpYear, '\0', sizeof(tmpYear));
		_db_datecnvGen(gengo, (UCHAR*)Bymd, &tmpGengo, tmpYear, 1, 0);
		Bymd[0] = tmpYear[0];

		m_Arith.l_unpac( Aymd, Bymd, 6 );
		date = atoi( Aymd );
		m_Progdata.pd_kazei_eymd	= ( date + 20000000 );															// 課税期間 自

		if( m_SnHeadData.Sn_TAXNO[0] != 'F' ){
			memmove( m_Progdata.pd_zckd_code, m_SnHeadData.Sn_TAXNO, 5 );					// 税務署番号
		}

		ProgMod.ProgTuuti_DialogEx( GetActiveFrame()->GetActiveView(), &m_Progdata );	
		ProgMod.CProgress_end();
	}
}

//-----------------------------------------------------------------------------
// 終了時の進捗保存
//-----------------------------------------------------------------------------
void CMainFrame::ProgressEnd()
{
	CKmnCtrl			KmnCtrl;
	int					Status = 1;
	PRCT_BPINFO			Bpinfo;
	KMNCTL_GETSKINFPAC	Gkspac;

	Gkspac.flg = 0;
	memset( Gkspac.dmy, '\0', sizeof(Gkspac.dmy) );
	memset( Gkspac.kanryo.dmy, '\0', sizeof(Gkspac.kanryo.dmy) );

	if( GetShincyokuSignEX() == 1 ){
		Bpinfo.svname = GET_DBNAME();
		if( KmnCtrl.BeginProc( &Bpinfo ) != ERR ){
			char	Bymd[4] = {0};
			char	Aymd[20] = {0};
			unsigned char	year = 0x00;
			unsigned char	dmme = 0x00;
			int		date = 0;
			int		gengo = 0;
			Gkspac.kanryo.nen = -1;																// 年分及び年月分 (yyyymm)
			Gkspac.kanryo.from = -1;															// 事業年度　自 (yyyymmdd)
			Gkspac.kanryo.to = -1;																// 事業年度　至 (yyyymmdd)
			if( !(m_SnHeadData.Sn_SKKBN%2) ){
				char	yymmdd[4]={0};
				m_SnHeadData.GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				memmove( Bymd, yymmdd, 3 );	
			}
			else{
				char	yymmdd[4]={0};
				m_SnHeadData.GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				memmove( Bymd, yymmdd, 3 );	
			}
//			shin_datecnv( Bymd[0], &year, 1 );
//			Bymd[0] = year;	// 和暦→西暦
			// 和暦→西暦変換
			int		tmpGengo=0;
			UCHAR	tmpYear[4]={ 0 };
			_db_datecnvGen(gengo, (UCHAR*)Bymd, &tmpGengo, tmpYear, 1, 0);
			Bymd[0] = tmpYear[0];
			
			m_Arith.l_unpac( Aymd, Bymd, 6 );
			date = atoi( Aymd );
			Gkspac.kanryo.kzfrom	= ( date + 20000000 );										// 課税期間　自 (yyyymmdd)
			if( !(m_SnHeadData.Sn_SKKBN%2) ){
				char	yymmdd[4]={0};
				m_SnHeadData.GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
				memmove( Bymd, yymmdd, 3 );	
			}
			else{
				char	yymmdd[4]={0};
				m_SnHeadData.GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				memmove( Bymd, yymmdd, 3 );	
			}
//			shin_datecnv( Bymd[0], &year, 1 );
//			Bymd[0] = year;	// 和暦→西暦
			// 和暦→西暦変換
			tmpGengo=0;
			memset(tmpYear, '\0', sizeof(tmpYear));
			_db_datecnvGen(gengo, (UCHAR*)Bymd, &tmpGengo, tmpYear, 1, 0);
			Bymd[0] = tmpYear[0];

			m_Arith.l_unpac( Aymd, Bymd, 6 );
			date = atoi( Aymd );
			Gkspac.kanryo.kzto	= ( date + 20000000 );											// 課税期間　至 (yyyymmdd)
			Gkspac.kanryo.gkbn =  _T( "GK000240" );												// 業務区分
			Gkspac.kanryo.mskbn.Empty();														// 業務明細区分ID
			switch( m_SnHeadData.Sn_SKKBN ){
				case 1 :	
					if( m_SnHeadData.Sn_Syuusei & 0x02 ){
						if( m_SnHeadData.Sn_Syuusei & 0x04 ){
							Gkspac.kanryo.skkbn = _T( "SK999102" );
						}// 更正の申出
						else{
							Gkspac.kanryo.skkbn = _T( "SK999100" );				// 更正の請求
						}
					}
					else{
						Gkspac.kanryo.skkbn = _T( "SK000100" );						// 確定申告	
					}
					break;
				case 2 : 
					Gkspac.kanryo.skkbn = _T( "SK000020" );
					break;						// 中間申告
				case 3 : 
					Gkspac.kanryo.skkbn = _T( "SK000101" );	
					break;						// 修正確定	
				case 4 : 
					Gkspac.kanryo.skkbn = _T( "SK000021" );	
					break;						// 修正中間	
				default: 
					Gkspac.kanryo.skkbn = _T( "SK999999" );	
					break;						// その他
			}
			Gkspac.kanryo.esign = -1;															// 電子申告サイン	1:電子申告 -1:以外
			Gkspac.kanryo.zckd_code.Empty();													// 全国地方公共団体コード
			// 会社名
			if( m_SnHeadData.IsSoleProprietor() ){
				Gkspac.kanryo.kmn_name = m_SnHeadData.Sn_DIHYO;	// 個人
			}
			else{
				Gkspac.kanryo.kmn_name = m_SnHeadData.Sn_CONAM;	// 法人
			}
			Status = KmnCtrl.GetShinkokuKanryoInfo( &Gkspac );
			// 後始末
			KmnCtrl.EndProc();
			if( Status == 0 ) {
				CProgress_Mod	ProgMod;
				if( ProgMod.CProgress_Iniz() ){
					return;
				}

//-- '16.08.17 --
//				int ret = ICSMessageBox( ProgMod.Get_Sin_msg(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );
//				ProgMod.CProgress_end();
//
//				if( ret == IDYES ){
//					ProgressAdminister();
//				}
//---------------
				int ret = ProgMod.Get_Sin_msgbox();
				ProgMod.CProgress_end();

				if( ret == IDYES ){
					ProgressAdminister();
				}
//---------------
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 会社読込み時の状態保存
//-----------------------------------------------------------------------------
void CMainFrame::SetOldData()
{
	CSnset	curset = m_pMainView->GetSnset();

	m_old_Sn_SKKBN  = m_SnHeadData.Sn_SKKBN;

	if( curset.IsKakuteiShinkoku() && curset.IsMinashi() ){
		m_old_tansyuku = CTime(-1);
		m_old_Sn_KZTNSK = 0xff;
		m_old_Sn_MCOUNT = 0xff;
	}
	else{
		m_old_tansyuku  = m_pMainView->FindTansyukuKaishi();
		m_old_Sn_KZTNSK = curset.IsKakuteiShinkoku() ? m_SnHeadData.Sn_KZTNSK : 0;
		m_old_Sn_MCOUNT = curset.IsTyukanShinkoku() ? m_SnHeadData.Sn_MCOUNT : 0;
	}
}

//-----------------------------------------------------------------------------
// 顧問先情報取り込み後、全ての保存情報に書き込む必要があるのか？
//-----------------------------------------------------------------------------
// 引数	type	：	
//		errmes	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CMainFrame::AdviseInfoRenew( int type, CString& errmes )
{
	CString	filter;
	char	telbf[64];
	int		number = 0;
	char	buf[512]={0};

	filter.Empty();
	filter.Format( _T("Sn_SEQ = %d"), number );
	// 消費税申告書
	BOOL	openFlg = FALSE;
	if( m_pZmSub->szsnk == NULL ){
		if( m_pZmSub->SzsnkOpen( filter ) == ERR ){
			errmes = m_pZmSub->GetZmErrMsg();
			return ERR;
		}
	}
	else{
		m_pZmSub->szsnk->Requery( filter );
	}
	
	// データ変換
	// 消費税申告書情報の更新
	m_pZmSub->szsnk->Edit();

	m_Util.bcd_to_asci( &m_pZmSub->szsnk->Sn_SEIRI, m_SnHeadData.Sn_SEIRI, 8 );			// 整理番号
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_ZMSYO, m_SnHeadData.Sn_ZMSYO, 12 );	// 税務署名
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_KSNO, m_SnHeadData.Sn_TAXNO, 5 );		// 税務署コード
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_ZIP, m_SnHeadData.Sn_ZIP, 8 );			// 郵便番号
	memset( telbf, '\0', sizeof( telbf ) );
	memset( telbf, 0x20, sizeof( telbf ) );
	memmove( telbf, m_SnHeadData.Sn_TELNO1, 6 );
	memmove( &telbf[7], m_SnHeadData.Sn_TELNO2, 4 );
	memmove( &telbf[12], m_SnHeadData.Sn_TELNO3, 4 );
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_TELNO,(unsigned char *)telbf, 16 );		// 電話番号
	memset( buf, '\0', sizeof( buf ) );
	memset( buf, 0x20, 40 );
	int KjLn = kjlen( m_SnHeadData.Sn_NOZEI, 40 );
	memmove( buf, m_SnHeadData.Sn_NOZEI, KjLn );
	KjLn = kjlen( &m_SnHeadData.Sn_NOZEI[40], 40 );
	memmove( &buf[40], &m_SnHeadData.Sn_NOZEI[40], KjLn );
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_NOZEI, (unsigned char *)buf, 80 );		// 納税地
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_CONAM, m_SnHeadData.Sn_CONAM, 40 );				// 氏名又は屋号
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_COFRI, m_SnHeadData.Sn_COFRI, 80 );				// 氏名又は屋号フリガナ
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_DIHYO, m_SnHeadData.Sn_DIHYO, 40 );				// 代表者氏名
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_DIFRI, m_SnHeadData.Sn_DIFRI, 80 );				// 代表者氏名フリガナ
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_KEIRI, m_SnHeadData.Sn_KEIRI, 40 );				// 経理担当者氏名
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_BANK,  m_SnHeadData.Sn_BANK, 20 );				// 銀行名称
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_SITEN, m_SnHeadData.Sn_SITEN,20 );				// 支店名称
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_YOKIN, m_SnHeadData.Sn_YOKIN, 8 );				// 預金種別
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_YUBIN, m_SnHeadData.Sn_YUBIN,16 );				// 郵便局名
	m_Util.bcd_to_asci( &m_pZmSub->szsnk->Sn_KOZA, m_SnHeadData.Sn_KOZA, 10 );						// 口座番号
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_YUNUM1, m_SnHeadData.Sn_YUNUM1, 5 );				// 郵便貯金番号①
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_YUNUM2, m_SnHeadData.Sn_YUNUM2, 8 );				// 郵便貯金番号②
	m_pZmSub->szsnk->Sn_FINANCE	= m_SnHeadData.Sn_FINANCE;								// 銀行、金庫等
	m_pZmSub->szsnk->Sn_CHAIN	= m_SnHeadData.Sn_CHAIN;								// 本店、支店等
	if( type ){
		m_Util.bcd_to_asci( &m_pZmSub->szsnk->Sn_ITIREN, m_SnHeadData.Sn_ITIREN, 8 );		// 一連番号
		m_Util.bcd_to_asci( &m_pZmSub->szsnk->Sn_ZSEIRI, m_SnHeadData.Sn_ZSEIRI, 8 );		// 納税署整理番号
		m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_SYOKN, m_SnHeadData.Sn_SYOKN, 3 );				// 所管
	}

//--> '15.12.15 INS START
	memset( buf, '\0', sizeof( buf ) );
	memset( buf, 0x20, 40 );
	KjLn = kjlen( m_SnHeadData.Sn_DIADR, 40 );
	memmove( buf, m_SnHeadData.Sn_DIADR, KjLn );
	KjLn = kjlen( &m_SnHeadData.Sn_DIADR[40], 40 );
	memmove( &buf[40], &m_SnHeadData.Sn_DIADR[40], KjLn );
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_DIADR, (unsigned char *)buf, 80 );			// 代表者住所
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_ADRD4[5], m_SnHeadData.Sn_DIZIP, 8 );		// 郵便番号
//<-- '15.12.15 INS END

//--> '15.12.23 INS START
	memset( telbf, '\0', sizeof( telbf ) );
	memset( telbf, 0x20, sizeof( telbf ) );
	memmove( telbf, m_SnHeadData.Sn_DITELNO1, 6 );
	memmove( &telbf[7], m_SnHeadData.Sn_DITELNO2, 4 );
	memmove( &telbf[12], m_SnHeadData.Sn_DITELNO3, 4 );
	m_Util.char_to_cstring( &m_pZmSub->szsnk->Sn_ADRD4[6], (unsigned char *)telbf, 16 );	// 代表者電話番号
//<-- '15.12.23 INS END

	if( m_pZmSub->szsnk->Update() == -1 ){
		errmes.Format( "消費税申告書データの書き込み中に障害が発生しました。保存番号(%d)", number );
		return( ERR );
	}

	if( openFlg ){
		m_pZmSub->SzsnkClose();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 現状顧問先情報のコピー処理
//-----------------------------------------------------------------------------
void CMainFrame::Copy_NowCoInfo()
{
	memmove( NowCoInf.Sn_SEIRI , m_SnHeadData.Sn_SEIRI, 4 );
	memmove( NowCoInf.Sn_ZMSYO , m_SnHeadData.Sn_ZMSYO, 12 );
	memmove( NowCoInf.Sn_TAXNO , m_SnHeadData.Sn_TAXNO, 5 );
	memmove( NowCoInf.Sn_ZIP , m_SnHeadData.Sn_ZIP, 10 );
	memmove( NowCoInf.Sn_NOZEI , m_SnHeadData.Sn_NOZEI, 80 );
	memmove( NowCoInf.Sn_TELNO1 , m_SnHeadData.Sn_TELNO1, 6 );
	memmove( NowCoInf.Sn_TELNO2 , m_SnHeadData.Sn_TELNO2, 4 );
	memmove( NowCoInf.Sn_TELNO3 , m_SnHeadData.Sn_TELNO3, 4 );
	memmove( NowCoInf.Sn_DIHYO , m_SnHeadData.Sn_DIHYO, 40 );
	memmove( NowCoInf.Sn_DIFRI , m_SnHeadData.Sn_DIFRI, 80 );
	memmove( NowCoInf.Sn_CONAM , m_SnHeadData.Sn_CONAM, 40 );
	memmove( NowCoInf.Sn_COFRI , m_SnHeadData.Sn_COFRI, 80 );
	memmove( NowCoInf.Sn_KEIRI , m_SnHeadData.Sn_KEIRI, 40 );
	memmove( NowCoInf.Sn_YUBIN , m_SnHeadData.Sn_YUBIN, 16 );
	memmove( NowCoInf.Sn_YUNUM1 , m_SnHeadData.Sn_YUNUM1, 5 );
	memmove( NowCoInf.Sn_YUNUM2 , m_SnHeadData.Sn_YUNUM2, 8 );
	memmove( NowCoInf.Sn_BANK , m_SnHeadData.Sn_BANK, 20 );
	memmove( NowCoInf.Sn_SITEN , m_SnHeadData.Sn_SITEN, 20 );
	memmove( NowCoInf.Sn_YOKIN , m_SnHeadData.Sn_YOKIN, 8 );
	memmove( NowCoInf.Sn_KOZA , m_SnHeadData.Sn_KOZA, 5 );
	NowCoInf.Sn_FINANCE = m_SnHeadData.Sn_FINANCE;
	NowCoInf.Sn_CHAIN = m_SnHeadData.Sn_CHAIN;
//--> '15.12.15 INS START
	memmove( NowCoInf.Sn_DIADR, m_SnHeadData.Sn_DIADR, sizeof(m_SnHeadData.Sn_DIADR) );
	memmove( NowCoInf.Sn_DIZIP, m_SnHeadData.Sn_DIZIP, sizeof(m_SnHeadData.Sn_DIZIP) );
//<-- '15.12.15 INS END
//--> '15.12.23 INS START
	memmove( NowCoInf.Sn_DITELNO1, m_SnHeadData.Sn_DITELNO1, sizeof(m_SnHeadData.Sn_DITELNO1) );
	memmove( NowCoInf.Sn_DITELNO2, m_SnHeadData.Sn_DITELNO2, sizeof(m_SnHeadData.Sn_DITELNO2) );
	memmove( NowCoInf.Sn_DITELNO3, m_SnHeadData.Sn_DITELNO3, sizeof(m_SnHeadData.Sn_DITELNO3) );
//<-- '15.12.23 INS END

	return;
}

//-----------------------------------------------------------------------------
// 税理士タイプの取得('15.02.25)
//-----------------------------------------------------------------------------
// 返送値	税理士タイプ
//-----------------------------------------------------------------------------
int CMainFrame::GetZeirishiType()
{
	int	type = 0;

	if( m_ZeiriInfo.sign1&0x01 ){
		type = m_ZeiriInfo.zeiridata.rz_zeiKbn;
	}
	else{
		type = 1;	// 社員税理士
	}

	return type;
}

//-----------------------------------------------------------------------------
// 顧問先情報データのメモリアップを切り替える('18.11.06)
//-----------------------------------------------------------------------------
// 引数	bSw	：	true:メモリに上げない, false:メモリに上げる
//-----------------------------------------------------------------------------
void CMainFrame::_TgtLimit_Kmn( bool bSw )
{
	if( m_pCoinfCtl ){
		if( bSw ){
			m_pCoinfCtl->TgtLimit_Kmn( LM_NO_MEMUP );
		}
		else{
			m_pCoinfCtl->TgtLimit_Kmn( 0x00 );
		}
	}
}

//-----------------------------------------------------------------------------
// 顧問先情報モジュールにマスター側のデータをセット
//-----------------------------------------------------------------------------
void CMainFrame::SetMasterInfo( CINF_REC &CoRec )
{
	// SetCoInformationの処理を引用

	memset( &CoRec, '\0', sizeof(CoRec) );

	char	asci[512] = {0};
	char	CoNo[4] = {0};
	memset( asci, '\0', sizeof( asci ) );
	memset( CoNo, '\0', sizeof( CoNo ) );
	sprintf_s( asci, sizeof( asci ), "%08d", m_pZmSub->zvol->v_cod );
	l_pack( CoNo, asci, 8 );

	int ExsistData = 0;
	CINF_REC	CoRecGt = {0};
	CTBL_PAR	CtParGt	= {0};
	RFIN_REC	RfinRec = {0};
	CtParGt.tpDtyp = 1;
	CtParGt.tpMtyp = 0;
	CtParGt.tpApno = (m_pZmSub->zvol->apno&0xff);
	memmove( CtParGt.tpCcod, CoNo, 4 );
	if( m_pCoinfCtl->FindRecFromMcod( 0, &CoRecGt, &CtParGt ) != 1 ){
		//m_pCoinfCtl->CloseFile();
		//delete( m_pCoinfCtl );
		//m_pCoinfCtl = NULL;	// ('14.03.13)
		//return;
		// 該当するレコードがない場合でもデータを表示させたいのでセットの流れに行く。
	}
	else{
		ExsistData = 1;
	}

	int  Ck = 0, Ct = 0, Ln = 0;
	int  BankGt = -1;	// 銀行情報サイン
	int	 PostGt = -1;	// 郵便情報サイン
	char WKbuf[128] = {0}, Ascii[128] = {0}, TelNo1[10] = {0}, TelNo2[10] = {0}, TelNo3[10] = {0};

	if( m_SnHeadData.Sn_BYsw == 1 )	{	// 金融機関はゆうちょ銀行
		PostGt = 0;
		RfinRec.rfKubn = 1;
		// 郵便局名
		memmove( RfinRec.rfFnam, m_SnHeadData.Sn_YUBIN, 16 );
		// 郵便貯金の記号番号１
		memmove( RfinRec.rfKzno, m_SnHeadData.Sn_YUNUM1, 5 );
		// 郵便貯金の記号番号２
		memmove( &RfinRec.rfKzno[6], m_SnHeadData.Sn_YUNUM2, 8 );
	}
	else{								// 金融機関は銀行
		BankGt = 0;
		RfinRec.rfKubn = 2;
		// 銀行名称
		memmove( RfinRec.rfFnam, m_SnHeadData.Sn_BANK, 20 );
		// 支店名称
		memmove( RfinRec.rfSnam, m_SnHeadData.Sn_SITEN, 20 );
		// 預金種別
		memset( WKbuf, '\0', sizeof( WKbuf ) );
		memmove( WKbuf, m_SnHeadData.Sn_YOKIN, 8 );
		if( !CompareStringSJIS( WKbuf,_T("普通　　") ) )	RfinRec.rfDepo = 1;
		if( !CompareStringSJIS( WKbuf,_T("当座　　") ) )	RfinRec.rfDepo = 2;
		if( !CompareStringSJIS( WKbuf,_T("納税準備") ) )	RfinRec.rfDepo = 3;
		if( !CompareStringSJIS( WKbuf,_T("貯蓄　　") ) )	RfinRec.rfDepo = 4;
		if( !RfinRec.rfDepo ) {
			memmove( RfinRec.rfDnam, m_SnHeadData.Sn_YOKIN, 8 );
		}
		// 口座番号
		l_unpac( RfinRec.rfKzno, m_SnHeadData.Sn_KOZA, 10 );
		// 銀行・金庫等
		switch(m_SnHeadData.Sn_FINANCE)	{
			case 0x00 :	RfinRec.rfKubn = 2;	break;	// 銀行
			case 0x01 :	RfinRec.rfKubn = 3;	break;	// 金庫
			case 0x02 :	RfinRec.rfKubn = 4;	break;	// 組合
			case 0x03 :	RfinRec.rfKubn = 5;	break;	// 農協 
			case 0x04 :	RfinRec.rfKubn = 6;	break;	// 漁協
			default   : RfinRec.rfKubn = 0;	break;	// 無し
		}
		// 本店・支店等
		switch(m_SnHeadData.Sn_CHAIN)	{
			case 0x00 :	RfinRec.rfSkbn = 1;	break;	// 本店
			case 0x01 :	RfinRec.rfSkbn = 2;	break;	// 支店
			case 0x02 :	RfinRec.rfSkbn = 3;	break;	// 本所
			case 0x03 :	RfinRec.rfSkbn = 4;	break;	// 支所
			case 0x05 :	RfinRec.rfSkbn = 5;	break;	// 出張所
			case 0x06 : RfinRec.rfSkbn = 6;	break;	// 上記以外 ★[13'11.27]
			default   : RfinRec.rfSkbn = 0;	break;	// 無し
		}
	}

	// 会社情報へ書き込み
	//NCOF_PAR	NCpar = {0};
	
	//NCpar.cpPare = this;
	//NCpar.cpDtyp = 1;
	//NCpar.cpMtyp = 0;
	//NCpar.cpApno = (m_pZmSub->zvol->apno&0xff);
	//memmove( NCpar.cpCode, CoNo, 4 );

	//long& item_flg0 = NCpar.cpEflg[0];
	//long& item_flg1 = NCpar.cpEflg[1];
	
	// 整理番号
	//if( m_SnHeadData.Sn_SEIRI[0] != NowCoInf.Sn_SEIRI[0] || m_SnHeadData.Sn_SEIRI[1] != NowCoInf.Sn_SEIRI[1] ||
	//	m_SnHeadData.Sn_SEIRI[2] != NowCoInf.Sn_SEIRI[2] || m_SnHeadData.Sn_SEIRI[3] != NowCoInf.Sn_SEIRI[3] ) {
	//	item_flg1 |= 0x00000080;
		memset( WKbuf, '\0', sizeof( WKbuf ) );
		l_unpac( WKbuf, m_SnHeadData.Sn_SEIRI,  8 );
		memmove(  CoRec.ciRegNo, WKbuf, 8 );
	//}

	// 税務署名
	//if( m_Util.StringCompare( m_SnHeadData.Sn_ZMSYO, NowCoInf.Sn_ZMSYO, 12 ) )	{
	//	item_flg1 |= 0x00000040;
		Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_ZMSYO, 12 );
		memmove( CoRec.ciTaxName, m_SnHeadData.Sn_ZMSYO, Ln );
	//}
	//if( m_Util.StringCompare( m_SnHeadData.Sn_TAXNO, NowCoInf.Sn_TAXNO, 5 ) )	{
	//	item_flg1 |= 0x00000040;
		if( m_SnHeadData.Sn_TAXNO[0] == 'F' )	memset( CoRec.ciTaxCode, 0xff, 5 );
		else									memmove( (BYTE*)CoRec.ciTaxCode, m_SnHeadData.Sn_TAXNO, 5 );
	//}

	
	// 納税地　郵便番号
	//if( m_Util.StringCompare( m_SnHeadData.Sn_ZIP, NowCoInf.Sn_ZIP, 8 ) ){
		char	ZipNo[10] = {0}; 	
		if( PostcodeConvert( ZipNo, m_SnHeadData.Sn_ZIP ) ) {
			//item_flg1 |= 0x00000400;
			memmove( CoRec.ciTppPno, ZipNo, 8 );
			if( ExsistData ){
				if( CoRecGt.ciNzsw1 == 1 )	{
					//item_flg0 |= 0x00000800;
					memmove( CoRec.ciPostNo, ZipNo, 8 );
				}
				if( CoRecGt.ciNzsw1 == 2 )	{
					//item_flg1 |= 0x00020000;
					memmove( CoRec.ciUtpPno, ZipNo, 8 );
				}				
			}
			else{
				memmove( CoRec.ciPostNo, ZipNo, 8 );
			}
		}
	//}

	// 納税地上段及び下段
	//if( m_Util.StringCompare( m_SnHeadData.Sn_NOZEI, NowCoInf.Sn_NOZEI, 40 ) ||
	//	m_Util.StringCompare( &m_SnHeadData.Sn_NOZEI[40], &NowCoInf.Sn_NOZEI[40], 40 ) ){
		// 納税地（上段）
		//item_flg1 |= 0x00000800;
		Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_NOZEI, 40 );
		memmove( CoRec.ciTppAd1, m_SnHeadData.Sn_NOZEI, Ln );
		if( ExsistData ){
			if( CoRecGt.ciNzsw1 == 1 )	{
				//item_flg0 |= 0x00001000;
				memmove( CoRec.ciAdrs1, m_SnHeadData.Sn_NOZEI, Ln );
			}
			if( CoRecGt.ciNzsw1 == 2 )	{
				//item_flg1 |= 0x00040000;
				memmove( CoRec.ciUtpAd1, m_SnHeadData.Sn_NOZEI, Ln );
			}			
		}
		else{
			memmove( CoRec.ciAdrs1, m_SnHeadData.Sn_NOZEI, Ln );
		}
		// 納税地（下段）
		//item_flg1 |= 0x00002000;
		Ln = m_Util.MixSpaceCutLength( (char *)&m_SnHeadData.Sn_NOZEI[40], 40 );
		memmove( CoRec.ciTppAd2, &m_SnHeadData.Sn_NOZEI[40], Ln );
		if( ExsistData ){
			if( CoRecGt.ciNzsw1 == 1 )	{
				//item_flg0 |= 0x00004000;
				memmove( CoRec.ciAdrs2, &m_SnHeadData.Sn_NOZEI[40], Ln );
			}
			if( CoRecGt.ciNzsw1 == 2 )	{
				//item_flg1 |= 0x00100000;
				memmove( CoRec.ciUtpAd2, &m_SnHeadData.Sn_NOZEI[40], Ln );
			}		
		}
		else{
			memmove( CoRec.ciAdrs2, &m_SnHeadData.Sn_NOZEI[40], Ln );
		}
	//}

	// 納税地 電話番号
	//if( m_Util.StringCompare( m_SnHeadData.Sn_TELNO1, NowCoInf.Sn_TELNO1, 6 ) ||
	//	m_Util.StringCompare( m_SnHeadData.Sn_TELNO2, NowCoInf.Sn_TELNO2, 4 ) ||
	//	m_Util.StringCompare( m_SnHeadData.Sn_TELNO3, NowCoInf.Sn_TELNO3, 4 ) )	{
		//item_flg1 |= 0x00008000;
		memset( WKbuf, '\0', sizeof( WKbuf ) );
		memset( TelNo1, '\0', sizeof( TelNo1 ) );
		memset( TelNo2, '\0', sizeof( TelNo2 ) );
		memset( TelNo3, '\0', sizeof( TelNo3 ) );
		memmove( TelNo1, m_SnHeadData.Sn_TELNO1, 6 );
		memmove( TelNo2, m_SnHeadData.Sn_TELNO2, 4 );
		memmove( TelNo3, m_SnHeadData.Sn_TELNO3, 4 );
		m_Util.AnkSpaceCut( TelNo1, 6 );
		m_Util.AnkSpaceCut( TelNo2, 4 );
		m_Util.AnkSpaceCut( TelNo3, 4 );
		if( strlen( TelNo1 ) > 0 || strlen( TelNo2 ) > 0 || strlen( TelNo3 ) > 0 ){
			sprintf_s( WKbuf, sizeof( WKbuf ), "%s-%s-%s", TelNo1, TelNo2, TelNo3 );
			memmove( CoRec.ciTppTel, WKbuf, 16 );
			if( ExsistData ){
				if( CoRecGt.ciNzsw1 == 1 )	{
					//item_flg0 |= 0x00010000;
					memmove( CoRec.ciTeln2, WKbuf, 16 );
				}
				if( CoRecGt.ciNzsw1 == 2 )	{
					//item_flg1 |= 0x00010000;
					memmove( CoRec.ciUtpTel, WKbuf, 16 );
				}		
			}
			else{
				memmove( CoRec.ciTeln2, WKbuf, 16 );
			}
		}
	//}
	
	// 代表者氏名及び氏名又は屋号 ●セットする時は顧問先の法人及び個人に従う●
	int KubunFlg = 0;
	if( ExsistData ){
		KubunFlg = CoRecGt.ciHKkbn;
	}
	else{
		// 紐づけがない場合は、個人以外はすべて法人でセットしておく
		if( (m_pZmSub->zvol->apno&0xff) == 0x10 ){
			KubunFlg = 1;
		}
		else{
			if( m_pZmSub->zvol->ind_type ){	// 法人の個人
				KubunFlg = 1;
			}
			else{
				KubunFlg = 0;
			}
		}
	}

	//if( CoRecGt.ciHKkbn == 1 ) {
	if( KubunFlg == 1 ){	// 個人

		// 個人は会社名称を代表者氏名に転送
		// 代表者氏名
		//if( m_Util.StringCompare( m_SnHeadData.Sn_DIHYO, NowCoInf.Sn_DIHYO, 40 ) ){
		//	item_flg0 |= 0x00000004;
			Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_DIHYO, 40 );
			memmove( CoRec.ciName, m_SnHeadData.Sn_DIHYO, Ln );
		//}
		// 代表者氏名（ふりがな）漢字変換はCompareStringSJIS()関数がしてくれる
		//if( m_Util.StringCompare( m_SnHeadData.Sn_DIFRI, NowCoInf.Sn_DIFRI, 34*2 ) ){
		//	item_flg0 |= 0x00000008;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_DIFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			m_Util.LetterAlter( WKbuf, Ascii, 34 );
			Ln =m_Util. MixSpaceCutLength( (char *)Ascii, 34 );
			memmove( CoRec.ciKana, Ascii, Ln );
		//}
		// 屋号又は氏名
		//if( m_Util.StringCompare( m_SnHeadData.Sn_CONAM, NowCoInf.Sn_CONAM, 36 ) ){
			//item_flg0 |= 0x00000080;
			Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_CONAM, 36 );
			memmove( CoRec.ciYagou, m_SnHeadData.Sn_CONAM, Ln );
		//}
		// 屋号又は氏名（ふりがな）漢字変換が必要
		//if( m_Util.StringCompare( m_SnHeadData.Sn_COFRI, NowCoInf.Sn_COFRI, 34*2 ) ){
			//item_flg0 |= 0x00000100;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_COFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			m_Util.LetterAlter( WKbuf, Ascii, 30 );
			Ln = m_Util.MixSpaceCutLength( (char *)Ascii, 30 );
			memmove( CoRec.ciYgKna, Ascii, Ln );
		//}
	}
	else{
		// 法人は会社名称を屋号又は氏名に転送
		// 名称又は屋号
		//if(  m_Util.StringCompare( m_SnHeadData.Sn_CONAM, NowCoInf.Sn_CONAM, 40 ) ){
			//item_flg0 |= 0x00000004;
			Ln =  m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_CONAM, 40 );
			memmove( CoRec.ciName, m_SnHeadData.Sn_CONAM, Ln );
		//}
		// 名称又は屋号（ふりがな）漢字変換が必要
		//if(  m_Util.StringCompare( m_SnHeadData.Sn_COFRI, NowCoInf.Sn_COFRI, 34*2 ) ){
			//item_flg0 |= 0x00000008;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_COFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			 m_Util.LetterAlter( WKbuf, Ascii, 34 );
			Ln =  m_Util.MixSpaceCutLength( (char *)Ascii, 34 );
			memmove( CoRec.ciKana, Ascii, Ln );
		//}
		// 代表者名
		//if(  m_Util.StringCompare(  m_SnHeadData.Sn_DIHYO, NowCoInf.Sn_DIHYO, 40 ) ){
			//item_flg0 |= 0x00800000;
			Ln =  m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_DIHYO, 24 );
			memmove( CoRec.ciRepName, m_SnHeadData.Sn_DIHYO, Ln );
		//}
		// 代表社名（ふりがな）
		//if(  m_Util.StringCompare( m_SnHeadData.Sn_DIFRI, NowCoInf.Sn_DIFRI, 34*2 ) ){
			//item_flg0 |= 0x01000000;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_DIFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			 m_Util.LetterAlter( WKbuf, Ascii, 24 );
			Ln =  m_Util.MixSpaceCutLength( (char *)Ascii, 24 );
			memmove( CoRec.ciRepKana, Ascii, Ln );
		//}
//--> '15.12.15 INS START
		// 代表者　郵便番号
		//if( m_Util.StringCompare( m_SnHeadData.Sn_DIZIP, NowCoInf.Sn_DIZIP, 8 ) )	{
			char	ZipNo[10] = {0}; 	
			if( PostcodeConvert( ZipNo, m_SnHeadData.Sn_DIZIP ) ) {
				//item_flg0 |= 0x02000000;
				memmove( CoRec.ciRepPno, ZipNo, 8 );
			}
		//}

		// 代表者住所上段及び下段
		//if( m_Util.StringCompare( m_SnHeadData.Sn_DIADR, NowCoInf.Sn_DIADR, 40 ) ||
		//	m_Util.StringCompare( &m_SnHeadData.Sn_DIADR[40], &NowCoInf.Sn_DIADR[40], 40 ) ) {
			// 代表者住所（上段）
			//item_flg0 |= 0x04000000;
			Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_DIADR, 40 );
			memmove( CoRec.ciRepAd1, m_SnHeadData.Sn_DIADR, Ln );
			// 代表者住所（下段）
			//item_flg0 |= 0x08000000;
			Ln = m_Util.MixSpaceCutLength( (char *)&m_SnHeadData.Sn_DIADR[40], 40 );
			memmove( CoRec.ciRepAd2, &m_SnHeadData.Sn_DIADR[40], Ln );
		//}
//<-- '15.12.15 INS END

//--> '15.12.23 INS START
		//if( m_Util.StringCompare( m_SnHeadData.Sn_DITELNO1, NowCoInf.Sn_DITELNO1, 6 ) ||
		//	m_Util.StringCompare( m_SnHeadData.Sn_DITELNO2, NowCoInf.Sn_DITELNO2, 4 ) ||
		//	m_Util.StringCompare( m_SnHeadData.Sn_DITELNO3, NowCoInf.Sn_DITELNO3, 4 ) )	{
		//	item_flg0 |= 0x10000000;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memset( TelNo1, '\0', sizeof( TelNo1 ) );
			memset( TelNo2, '\0', sizeof( TelNo2 ) );
			memset( TelNo3, '\0', sizeof( TelNo3 ) );
			memmove( TelNo1, m_SnHeadData.Sn_DITELNO1, 6 );
			memmove( TelNo2, m_SnHeadData.Sn_DITELNO2, 4 );
			memmove( TelNo3, m_SnHeadData.Sn_DITELNO3, 4 );
			m_Util.AnkSpaceCut( TelNo1, 6 );
			m_Util.AnkSpaceCut( TelNo2, 4 );
			m_Util.AnkSpaceCut( TelNo3, 4 );
			if( strlen( TelNo1 ) > 0 || strlen( TelNo2 ) > 0 || strlen( TelNo3 ) > 0 ){
				sprintf_s( WKbuf, sizeof( WKbuf ), "%s-%s-%s", TelNo1, TelNo2, TelNo3 );
				memmove( CoRec.ciRepTel, WKbuf, 16 );
			}
		//}
//<-- '15.12.23 INS END
	}

	// 経理担当社名
	//if( m_Util.StringCompare( m_SnHeadData.Sn_KEIRI, NowCoInf.Sn_KEIRI, 40 ) )	{
	//	item_flg1 |= 0x00000002;
		memmove( CoRec.ciAcName, m_SnHeadData.Sn_KEIRI, 24 );
	//}

	// 郵便＆銀行情報の取得
	int Bank = -1;	// 銀行情報サイン
	int	Post = -1;	// 郵便情報サイン
	if( m_SnHeadData.Sn_BYsw == 1 )	{	// 金融機関はゆうちょ銀行
		// 郵便局名
		//if( m_Util.StringCompare( m_SnHeadData.Sn_YUBIN, NowCoInf.Sn_YUBIN, 16 ) )					Post = 0;
		// 郵便貯金の記号番号１
		//if( m_Util.StringCompare( m_SnHeadData.Sn_YUNUM1, NowCoInf.Sn_YUNUM1, 5 ) )					Post = 0;
		// 郵便貯金の記号番号２
		//if( m_Util.StringCompare( m_SnHeadData.Sn_YUNUM2, NowCoInf.Sn_YUNUM2, 8 ) )					Post = 0;
		//if( Post != -1 )	CoRec.ciFinTbl[Post].rfKubn = 1;
		CoRec.ciFinTbl[Post].rfKubn = 1;
		Post = 0;
	}
	else	{								// 金融機関は銀行
		// 銀行名称
		/*if( m_Util.StringCompare( m_SnHeadData.Sn_BANK, NowCoInf.Sn_BANK, 20 ) ){
			Bank = 0;
		}
		// 支店名称
		if( m_Util.StringCompare( m_SnHeadData.Sn_SITEN, NowCoInf.Sn_SITEN, 20 ) ){
			Bank = 0;
		}
		// 預金種別
		if( m_Util.StringCompare( m_SnHeadData.Sn_YOKIN, NowCoInf.Sn_YOKIN, 8 ) ){
			Bank = 0;
		}
		// 口座番号
		if( strncmp( (char *)m_SnHeadData.Sn_KOZA, (char *)NowCoInf.Sn_KOZA, 5 )	){
			Bank = 0;
		}
		// 銀行・金庫等
		if( m_SnHeadData.Sn_FINANCE != NowCoInf.Sn_FINANCE ){
			Bank = 0;
		}
		// 本店・支店等
		if( m_SnHeadData.Sn_CHAIN != NowCoInf.Sn_CHAIN ){
			Bank = 0;
		}
		if( Bank != -1 ){
			CoRec.ciFinTbl[Bank].rfKubn = 2;
		}*/
		CoRec.ciFinTbl[Bank].rfKubn = 2;
		Bank = 0;
	}

	// 金融テーブルの比較
	if( Post != -1 || Bank != -1 ) {
		for( Ct = 0; Ct != 5; Ct++ ) {
			if( CoRecGt.ciFinTbl[Ct].rfKubn == RfinRec.rfKubn ) {
				Ck = 0;
				if( m_SnHeadData.Sn_BYsw == 1 ) {
					// 郵便局名
					if( !m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfFnam, (BYTE*)RfinRec.rfFnam, 16 ) )	Ck++;
					// 郵便貯金の記号番号１・２
					if( !m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfKzno, (BYTE*)RfinRec.rfKzno, 14 ) )	Ck++;
					if( Ck == 2 )	{
						CoRec.ciFinTbl[Post].rfKubn = 0;
						Post = -1;
					}
				}
				else	{
					// 銀行名称
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfFnam, (BYTE*)RfinRec.rfFnam, 20 ) )		Ck++;
					// 支店名称										
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfSnam, (BYTE*)RfinRec.rfSnam, 20 ) )		Ck++;
					// 預金種別
					if( CoRecGt.ciFinTbl[Ct].rfDepo != RfinRec.rfDepo )												Ck++;
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfDnam, (BYTE*)RfinRec.rfDnam, 8 ) )		Ck++;
					// 口座番号										
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfKzno, (BYTE*)RfinRec.rfKzno, 10 ) )		Ck++;
					// 銀行・金庫等
					if( CoRecGt.ciFinTbl[Ct].rfKubn != RfinRec.rfKubn )												Ck++;
					// 本店・支店等
					if( CoRecGt.ciFinTbl[Ct].rfSkbn != RfinRec.rfSkbn )												Ck++;
					if( Ck == 7 )	{
						CoRec.ciFinTbl[Ct].rfKubn = 0;
						Bank = -1;
					}

				}
			}
		}

		// 銀行情報
		if( Bank != -1 ){
			// ゆうちょ銀行＆銀行情報
			//item_flg1 |= 0x00000010;

			// 銀行名称
			memmove( CoRec.ciFinTbl[Bank].rfFnam, m_SnHeadData.Sn_BANK, 20 );
			// 支店名称
			memmove( CoRec.ciFinTbl[Bank].rfSnam, m_SnHeadData.Sn_SITEN, 20 );
			// 預金種別
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_YOKIN, 8 );
			if( !CompareStringSJIS( WKbuf,_T("普通　　") ) )	CoRec.ciFinTbl[Bank].rfDepo = 1;
			if( !CompareStringSJIS( WKbuf,_T("当座　　") ) )	CoRec.ciFinTbl[Bank].rfDepo = 2;
			if( !CompareStringSJIS( WKbuf,_T("納税準備") ) )	CoRec.ciFinTbl[Bank].rfDepo = 3;
			if( !CompareStringSJIS( WKbuf,_T("貯蓄　　") ) )	CoRec.ciFinTbl[Bank].rfDepo = 4;
			if( !CoRec.ciFinTbl[Bank].rfDepo ) {
				memmove( CoRec.ciFinTbl[Bank].rfDnam, m_SnHeadData.Sn_YOKIN, 8 );
			}
			// 口座番号
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			l_unpac( WKbuf, m_SnHeadData.Sn_KOZA, 10 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			m_Util.AllAnkSpaceCut( Ascii, WKbuf, 10 );
			memmove( CoRec.ciFinTbl[Bank].rfKzno, Ascii, 10 );
			// 銀行・金庫等
			switch(m_SnHeadData.Sn_FINANCE)	{
				case 0x00 :	CoRec.ciFinTbl[Bank].rfKubn = 2;	break;	// 銀行
				case 0x01 :	CoRec.ciFinTbl[Bank].rfKubn = 3;	break;	// 金庫
				case 0x02 :	CoRec.ciFinTbl[Bank].rfKubn = 4;	break;	// 組合
				case 0x03 :	CoRec.ciFinTbl[Bank].rfKubn = 5;	break;	// 農協 
				case 0x04 :	CoRec.ciFinTbl[Bank].rfKubn = 6;	break;	// 漁協
				//default   :  item_flg1 &= 0xffffffef;	break;	// 無し
			}
			// 本店・支店等
			switch(m_SnHeadData.Sn_CHAIN)	{
				case 0x00 :	CoRec.ciFinTbl[Bank].rfSkbn = 1;	break;	// 本店
				case 0x01 :	CoRec.ciFinTbl[Bank].rfSkbn = 2;	break;	// 支店
				case 0x02 :	CoRec.ciFinTbl[Bank].rfSkbn = 3;	break;	// 本所
				case 0x03 :	CoRec.ciFinTbl[Bank].rfSkbn = 4;	break;	// 支所
				case 0x05 :	CoRec.ciFinTbl[Bank].rfSkbn = 5;	break;	// 出張所
				case 0x06 :	CoRec.ciFinTbl[Bank].rfSkbn = 6;	break;	// 上記以外 ★[13'11.27]
				//default   : item_flg1 &= 0xffffffef;	break;	// 無し
			}
			
			/*if( !(item_flg1 & 0x00000010) ) {
				char		ErrMsg[256];
				memset( ErrMsg, '\0', sizeof( ErrMsg ) );
				strcpy_s( ErrMsg, sizeof( ErrMsg ), _T("還付先金融機関の種別（銀行・金庫及び本店・支店）が選択されていない為、還付金融機関の項目は転記されません。" ) );
				NCpar.cpMsgp = ErrMsg;
				NCpar.cpFunc |= 0x40;			// メッセージを表示
			}*/
		}
		// 郵便局情報
		if( Post != -1 )	{
			// ゆうちょ銀行＆銀行情報
			//item_flg1 |= 0x00000010;

			// 郵便局名
			memmove( CoRec.ciFinTbl[Post].rfFnam, m_SnHeadData.Sn_YUBIN, 16 );

			memset( TelNo1, '\0', sizeof( TelNo1 ) );
			m_Util.AllAnkSpaceCut( TelNo1, (char*)m_SnHeadData.Sn_YUNUM1, 5 );

			memset( TelNo2, '\0', sizeof( TelNo2 ) );
			m_Util.AllAnkSpaceCut( TelNo2, (char*)m_SnHeadData.Sn_YUNUM2, 8 );

			if( strlen( TelNo1 ) > 0 || strlen( TelNo2 ) > 0 ){
				memset( Ascii, '\0', sizeof( Ascii ) );
				sprintf_s( Ascii, sizeof( Ascii ), "%s-%s", TelNo1, TelNo2 );
				Ln = m_Util.MixSpaceCutLength( (char *)Ascii, 14 );
				memmove( CoRec.ciFinTbl[Post].rfKzno, Ascii, Ln );
			}
		}	
	}


	//CSnset	curset = m_pMainView->GetSnset();

	//if( !( curset.IsKakuteiShinkoku() && !curset.IsKikanTansyuku() ) ){
	if( m_SnHeadData.Sn_SKKBN != 1 && m_SnHeadData.Sn_KZTNSK != 1 ){
	
		//if( curset.IsTyukanShinkoku() ){
		if( m_SnHeadData.Sn_SKKBN == 2 ){
			// 中間申告 処理単位の設定
			if( m_SnHeadData.Sn_MCOUNT != m_old_Sn_MCOUNT ){
				//item_flg1 |= 0x00000020;
				CoRec.ciSyDcTimes = m_SnHeadData.Sn_MCOUNT;
			}
			
			// 課税期間短縮 処理単位のクリア
			if( (m_SnHeadData.Sn_KZTNSK&0x0f) != (m_old_Sn_KZTNSK&0x0f) ){
				//item_flg1 |= 0x00000200;
				CoRec.ciSyTShort = 0;
			}

			//if( m_old_tansyuku != CTime() )
				//item_flg1 |= 0x00010000;
		}

		//if( curset.IsKakuteiShinkoku() && curset.IsKikanTansyuku() ) {	// 上ではじいてるから来ることないのでは？
		/*if( m_SnHeadData.Sn_SKKBN == 1 && m_SnHeadData.Sn_KZTNSK == 1 ){
			// 課税期間短縮 処理単位の設定
			//if( !curset.IsMinashi() ) {
			if( (m_SnHeadData.Sn_Sign4&0xf0) != 0x20 ){
				if( (m_SnHeadData.Sn_KZTNSK&0x0f) != (m_old_Sn_KZTNSK&0x0f) )	{
					item_flg1 |= 0x00000200;
					if(m_SnHeadData.Sn_KZTNSK&0x0f) // 顧問先情報の仕様により反転
						CoRec.ciSyTShort = (m_SnHeadData.Sn_KZTNSK&0x0f) == 0x01 ? 0x02 : 0x01;
					else
						CoRec.ciSyTShort = 0;
				}

				// 中間申告 処理単位のクリア
				if( m_SnHeadData.Sn_MCOUNT != m_old_Sn_MCOUNT ){
					item_flg1 |= 0x00000020;
					CoRec.ciSyDcTimes = 0;
				}

				// 課税期間短縮 開始年月日の設定orクリア
				CTime tansyuku = m_pMainView->FindTansyukuKaishi();
				if( tansyuku != CTime() ){
//					CoRec.ciSyTSDay[0] = 0x04;
//					CTimeControl::SetYMD(&CoRec.ciSyTSDay[1],tansyuku);
					CTimeControl::SetYMD(CoRec.ciSyTSDay, tansyuku);
				}

				if( tansyuku != m_old_tansyuku ){
					item_flg1 |= 0x00010000;
				}
			}
		}*/
	}

	/*memmove( &NCpar.cpCflg, &NCpar.cpEflg, sizeof( NCpar.cpEflg ) );

	if( item_flg0 == 0x00000000 && item_flg1 == 0x00000000 ){
		if( m_pCoinfCtl->CloseFile() == FALSE )	{
			delete( m_pCoinfCtl );
			m_pCoinfCtl = NULL;	// ('14.03.13)
			return;
		}
		delete( m_pCoinfCtl );
		m_pCoinfCtl = NULL;	// ('14.03.13)
		return;
	}*/

	m_pCoinfCtl->SetMstDat( &CoRec, KMNCTL_GKIND_SYZ );
}

// 24/04/04_モジュール仕様変更 add -->
//-----------------------------------------------------------------------------
// 税理士情報を返す
//-----------------------------------------------------------------------------
// 引　数	R_HEAD*		管理情報
//			R_ZEIRI*	税理士情報
// 返送値	なし
//-----------------------------------------------------------------------------
void CMainFrame::GetZeiriInfo( R_HEAD* rHead, R_ZEIRI* rData )
{
	memcpy(rHead,&m_nTax.R_hed,sizeof(R_HEAD));
	memcpy(rData,&m_nTax.Zeiri_dat,sizeof(R_ZEIRI));
}
// 24/04/04_モジュール仕様変更 add <--
