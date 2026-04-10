
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// データベースマスターの作成
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//		pParent		：	親ウィンドウ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//			-2		：	例外
//-----------------------------------------------------------------------------
int CSyzMakeSubW::MakeDBMaster( SYZMAKE_MKCOINFO &mkcoinfo, CWnd *pParent )
{
	BOOL	vctladd = FALSE,
			mastadd = FALSE;

	CString	filter;

	// 物理ディスク
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

#ifndef _DEBUG
	if( m_Mctl.get_usertbl(m_user) != 0 ){
		m_ErrMsg = _T("ログインユーザの取得に失敗しました！");
		return -1;
	}
#endif

	// DB アクセスクラスオープン
	if( m_DBAccess.OpenDatabaseEx() < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -2;
	}

	// 遠隔ターミナル対応
	Cusertbl2* pUtbl = m_Mctl.get_usertbl( ICSMCTL::MAIN_USER );
	if( pUtbl == NULL ){
		m_ErrMsg = _T("ユーザー情報のチェックでエラーが発生しました！");
		return -1;
	}
	if( pUtbl->GetRecordCount() > 0 ){
		// ユーザの取得
		pUtbl->MoveFirst();
		do{
			mctl_usertbl2 user;
			user.user_name = pUtbl->m_user_name;
			user.real_name = pUtbl->m_real_name;
			user.user_type = pUtbl->m_user_type;
			user.user_att = pUtbl->m_user_att;
			user.user_id = pUtbl->m_user_id;
			user.certify = pUtbl->m_certify;
			user.ucd = pUtbl->m_ucd;
			user.option1 = pUtbl->m_option1;
			user.option2 = pUtbl->m_option2;
			user.entry_type = pUtbl->m_entry_type;
			user.entry_class = pUtbl->m_entry_class;
			user.entry_info = pUtbl->m_entry_info;
			user.entry_stop = pUtbl->m_entry_stop;
			user.entry_sdate = pUtbl->m_entry_sdate;
			user.entry_edate = pUtbl->m_entry_edate;
			user.entry_coacs = pUtbl->m_entry_coacs;
			m_UserAry.Add( user );
			pUtbl->MoveNext();
		}while( pUtbl->IsEOF() == FALSE );
	}
	pUtbl->Close();

	// 上手君ユーザーの取得
	pUtbl = m_Mctl.get_usertbl( ICSMCTL::JOZU_USER );
	if( pUtbl == NULL ){
		m_isKjok = FALSE;
	}
	else{
		if( pUtbl->GetRecordCount() < 1 ){
			m_isKjok = FALSE;
		}
		else{
			pUtbl->MoveFirst();
			do {
				mctl_usertbl2 user;
				user.user_name = pUtbl->m_user_name;
				user.real_name = pUtbl->m_real_name;
				user.user_type = pUtbl->m_user_type;
				user.user_att = pUtbl->m_user_att;
				user.user_id = pUtbl->m_user_id;
				user.certify = pUtbl->m_certify;
				user.ucd = pUtbl->m_ucd;
				user.option1 = pUtbl->m_option1;
				user.option2 = pUtbl->m_option2;
				user.entry_type = pUtbl->m_entry_type;
				user.entry_class = pUtbl->m_entry_class;
				user.entry_info = pUtbl->m_entry_info;
				user.entry_stop = pUtbl->m_entry_stop;
				user.entry_sdate = pUtbl->m_entry_sdate;
				user.entry_edate = pUtbl->m_entry_edate;
				user.entry_coacs = pUtbl->m_entry_coacs;
				m_jUserAry.Add( user );
				pUtbl->MoveNext();
			} while( pUtbl->IsEOF() == FALSE );
		}
		pUtbl->Close();
	}

	// システムデータベース存在チェック
	int ret = m_DBAccess.IsExistsDatabase( DBA_SYS_ACCESS, _T("sysctl") );
	if( ret == 0 ){
		// システムデータ無し
		m_ErrMsg = _T("システムデータベースが登録されていないため、マスターの作成ができません。");
		return -1;
	}
	else if( ret < 0 ){
		CString	tmpErr;
		m_DBAccess.GetErrorMessage( tmpErr );
		if( tmpErr.IsEmpty() == TRUE ){
			m_ErrMsg = _T("システムデータベースチェックでエラーが発生しました。");
		}
		else{
			m_ErrMsg.Format( _T("システムデータベースチェックでエラーが発生しました。\n%s"), tmpErr );
		}
		return -1;
	}

	// 必須項目チェック
	// 業種区分
	if( mkcoinfo.apno == 0xff ){
		m_ErrMsg = _T("業種区分が未設定です！");
		return -1;
	}
	// 会社コード
	if( (mkcoinfo.cocode<0) || (99999999<mkcoinfo.cocode) ){
		m_ErrMsg = _T("会社コードが不正です！");
		return -1;
	}
	// 決算期間
	if( (mkcoinfo.ss_ymd==-1) || (mkcoinfo.ee_ymd==-1) ){
		m_ErrMsg = _T("決算期間が未設定です！");
		return -1;
	}
	if( mkcoinfo.ee_ymd < NCM_YMD ){
		if( (mkcoinfo.apno==HOJIN_APNO) || (mkcoinfo.apno==UNSO_APNO) || (mkcoinfo.apno==KENSETU_APNO) ){
			m_ErrMsg = _T("新会社法対象期間外のマスターは作成できません");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < KH_TS_YMD ){
		if( mkcoinfo.apno == KOEKI_APNO ){
			m_ErrMsg = _T("平成18年4月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < KH3_TS_YMD ){
		if( mkcoinfo.apno == KOEKI3_APNO ){
			m_ErrMsg = _T("平成20年12月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < SFH_TS_YMD ){
		if( mkcoinfo.apno == SYAKAI_APNO ){
			m_ErrMsg = _T("平成12年4月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < SFH_SKTS_YMD ){
/*- '13.12.06 -*/
//		if( (mkcoinfo.apno==SYAKAI_APNO) ){
////		if( (mkcoinfo.apno==SYAKAI_APNO) && (m_sftyp==1) ){
/*-------------*/
		if( (mkcoinfo.apno==SYAKAI_APNO) && (mkcoinfo.nptyp&0x01) ){
/*-------------*/
			m_ErrMsg = _T("平成24年4月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < ALL_TS_YMD ){
		m_ErrMsg = _T("平成12年1月1日以降の期間を対象とします！");
		return -1;
	}

	// 登録先会社コードロック
	int	lhdl = 0;
	int	eeymd = ((mkcoinfo.ee_ymd/100)%10000);
	if( !(mkcoinfo.mkOpt1&0x02) ){
		if( (lhdl = co_lock_ms(mkcoinfo.apno, mkcoinfo.cocode, eeymd, 0x101)) < 0 ){
			m_ErrMsg = _T("入力された会社コードは、他で使用中です。");
			return -1;
		}
	}
	else{
		if( ChkRegistCoCode(mkcoinfo) ){
			m_ErrMsg = _T("入力された会社コードは、他で使用中です。");
			return -1;
		}
	}

//--> '15.07.10 INS START
	if( (mkcoinfo.apno==0x10) || (mkcoinfo.nIfn&0xff) ){
		// 個人マイナンバー登録
	}
//<-- '15.07.10 INS END

	// ウェイトダイアログ作成
	if( pParent ){
		ShowWaitDialog( pParent );
	}

	// ボリュームラベル初期化
	if( InitZVolume(mkcoinfo, true) ){
		goto ERR_MKMAST;
	}

	// ボリュームコントロール登録

	//ボリュームコントロール更新
	//ここで会社情報から自動判定するかフラグもらうか。今回はフラグもらう形で作成
	if ((mkcoinfo.mkOpt1 & 0x04)) {//既存マスターに申告書のみ作成　'20.04.16
		if (UpdateMasterToVlctl(mkcoinfo)) {
			goto ERR_MKMAST;
		}
	}
	else {
		if (AddMasterToVlctl(mkcoinfo)) {
			goto ERR_MKMAST;
		}
	}
	vctladd = TRUE;

	// 遠隔ターミナル対応
//	m_AcsAry.Append( m_JkAcsAry );


	if (!(mkcoinfo.mkOpt1 & 0x04)) {//既存マスター作成じゃない場合　'20.04.16

		// 公開ユーザ追加
		m_AcsAry.RemoveAll();
		if (mkcoinfo.open_userum) {
			COACS	coacs;
			int		userMax = (int)m_UserAry.GetCount();
			for (int i = 0; i < mkcoinfo.open_userum; i++) {
				for (int j = 0; j < userMax; j++) {
					if (mkcoinfo.pUserInfo[i].user_id == m_UserAry[j].user_id) {
						memset(&coacs, '\0', sizeof(COACS));
						coacs.apno = mkcoinfo.apno;
						coacs.cocode = mkcoinfo.cocode;
						coacs.option1 = m_UserAry[j].option1;
						coacs.option2 = m_UserAry[j].option2;
						coacs.user_id = m_UserAry[j].user_id;
						coacs.user_att = m_UserAry[j].user_att;
						coacs.acsok = 1;
						if (mkcoinfo.owner_id == m_UserAry[j].user_id) {
							coacs.owner = 1;
						}
						// 追加
						m_AcsAry.Add(coacs);
						break;
					}
				}
			}
		}

		// 会社アクセステーブル登録
		filter.Format(_T("apno = %02d and cocode = %08d"), mkcoinfo.apno, mkcoinfo.cocode);
		COAccess* pAccess = m_Mctl.coa_open(1, filter);
		if (pAccess == NULL) {	// 登録無し
			pAccess = m_Mctl.coa_open();

			int owner = 0;
			INT_PTR max = m_AcsAry.GetSize();
			for (int i = 0; i < max; i++) {
				COACS coacs = m_AcsAry.GetAt(i);

				if (coacs.acsok) {
					// 公開ユーザ登録
					pAccess->AddNew();
					pAccess->apno = coacs.apno;
					pAccess->cocode = coacs.cocode;
					pAccess->user_id = coacs.user_id;
					pAccess->user_att = coacs.user_att;
					pAccess->owner = coacs.owner;
					if (coacs.owner != 0) {
						pAccess->access = m_plcusr ? 1 : 0;
					}
					else {
						pAccess->access = coacs.access;
					}
					pAccess->option1 = coacs.option1;
					pAccess->option2 = (BYTE)coacs.option2;

					if (pAccess->Update() < 0) {
						goto ERR_MKMAST;
					}
					if (coacs.owner != 0) {
						owner = 1;
					}
				}
			}
			if (owner == 0) {
				// 遠隔ターミナル
				mctl_usertbl2	user;
				int cnt = (int)m_UserAry.GetSize();
				for (int j = 0; j < cnt; j++) {
					user = m_UserAry.GetAt(j);
					if (user.user_id == m_owner_id) {
						break;
					}
				}
				// オーナーユーザ登録
				pAccess->AddNew();
				pAccess->apno = mkcoinfo.apno;
				pAccess->cocode = mkcoinfo.cocode;
				pAccess->user_id = m_owner_id;
				pAccess->user_att = (m_owner_id != 0) ? user.user_att : 1;
				pAccess->owner = 1;
				pAccess->access = m_plcusr ? 1 : 0;
				pAccess->option1 = (m_owner_id != 0) ? user.option1 : 0;
				pAccess->option2 = 0;

				if (pAccess->Update() < 0) {
					goto ERR_MKMAST;
				}
			}
		}
	}
	m_Mctl.coa_close();

	// 会社名作成
	UCHAR	tmpSymd[4]={0};
	UCHAR	tmpEymd[4]={0};
	int_bcd( tmpSymd, mkcoinfo.ss_ymd, 4 );
	int_bcd( tmpEymd, mkcoinfo.ee_ymd, 4 );
	memset( m_mkCoName, '\0', sizeof(m_mkCoName) );
	sprintf_s( m_mkCoName, sizeof(m_mkCoName), _T("Z%02x_C%08d_Y%02x%02x"), mkcoinfo.apno, mkcoinfo.cocode, tmpEymd[1], tmpEymd[2] );

	// 月別情報の初期化
	if( InitMonthInfo(mkcoinfo) ){
		goto ERR_MKMAST;
	}

	// データベース作成
	if( pParent ){
		SetWaitTitle( _T("データベース作成中．．．") );
	}
	if( CreateDataBase(mkcoinfo) < 0 ){
		goto ERR_MKMAST;
	}
	mastadd = TRUE;

	// 会社初期データ追加
	if( pParent ){
		SetWaitTitle( _T("新規会社登録中．．．") );
	}
	if( InsertInitTbData(mkcoinfo) == -1 ){
		goto ERR_MKMAST;
	}
	if( UpdateZmsubSubsw(mkcoinfo) < 0 ){
		goto ERR_MKMAST;
	}

	//--->内訳書マスターの場合、vtbl情報を戻す処理を追加20.09.08
	//2019/07/22
	if (Vtbl_utiwakeback) {//内訳書の関連するVTBL情報を操作
		CDatabase* db = m_DBAccess.GetDatabase(DBA_MST_ACCESS);
		UpdateVtblUtiwakeVer(db, Utiwake_ver, Vtbl_utiwakeback , mkcoinfo);
	}
	//<-----


// '13.08.02：コピーから作成する事は考えていない！
//	// コピーから作成
//	if( sel == ID_ICSSYZ_MAKE_FROMCOPY ){
//		;
//	}
//------

	// 財務マスターに接続した後の初期化処理は未実装

	// ウエイトダイアログ非表示
	if( pParent ){
		HideWaitDialog( pParent );
	}


	// 登録先会社コードロック解除
	if( !(mkcoinfo.mkOpt1&0x02) ){
		co_unlock( lhdl );
	}

	return 0;

// エラー処理
ERR_MKMAST:

	// ウエイトダイアログ非表示
	if( pParent ){
		HideWaitDialog( pParent );
	}

	CString	tmpErr;
	m_DBAccess.GetErrorMessage( tmpErr );
	if( m_ErrMsg.IsEmpty() == FALSE ){
		;
	}
	else if( tmpErr.IsEmpty() == FALSE ){
		m_ErrMsg = tmpErr;
	}
	else{
		m_ErrMsg = _T("マスターの登録に失敗しました！");
	}

	// ウエイトダイアログ表示
	if( pParent ){
		ShowWaitDialog( pParent );
	}

	// ボリューム管理データ削除
	bool	haveImg = false;
	if( vctladd == TRUE ){
		if( pParent ){
			SetWaitTitle( _T("ボリュームコントロール削除中．．．") );
		}
		SYZMAKE_MKCOINFO	tmpMkcoinfo;
		memmove( &tmpMkcoinfo, &mkcoinfo, sizeof(SYZMAKE_MKCOINFO) );
		tmpMkcoinfo.ee_ymd = -1;	// 全削除
		DeleteMasterFromVctl( tmpMkcoinfo, haveImg );
	}

	// マスター削除
	if( mastadd == TRUE ){
		if( pParent ){
			SetWaitTitle( _T("マスター削除中．．．") );
		}
		SYZMAKE_MKCOINFO	tmpMkcoinfo;
		memmove( &tmpMkcoinfo, &mkcoinfo, sizeof(SYZMAKE_MKCOINFO) );
		tmpMkcoinfo.ee_ymd = -1;	// 全削除
		DropDatabaseAll( tmpMkcoinfo, haveImg );
	}

	// ウエイトダイアログ非表示
	if( pParent ){
		HideWaitDialog( pParent );
	}

	return -2;
}

//-----------------------------------------------------------------------------
// ボリュームコントロール登録
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::AddMasterToVlctl( SYZMAKE_MKCOINFO &mkcoinfo )
{
	int			ret = 0;
	CO_CTL_DB	coctl={0}, svcoctl={0};

	// ボリュームコントロールオープン
	if( (ret = m_Mctl.vctl_open(MODE_WRITE, mkcoinfo.apno, TYPE_MS)) != 0 ){
		switch( ret ){
			case	-1:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
			case	-2:
				m_ErrMsg = _T("ＩＣＳマスターの登録がありません！");
				break;
			case	-3:
				m_ErrMsg = _T("");
				break;
			default:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
		}
		return -1;
	}

	// 会社数チェック
	DT_CTL_DB	dtctl;
	m_Mctl.vctl_hedder( &dtctl );
	if( dtctl.d_max <= dtctl.d_cnt ){
		m_ErrMsg = _T("会社登録数が最大登録可能数に達しているため\n会社を登録することはできません！");
		m_Mctl.vctl_close();
		return -1;
	}

	// 会社ロックチェック
	UCHAR	tmpSymd[4] = {0};
	UCHAR	tmpEymd[4] = {0};
	int_bcd( tmpSymd, mkcoinfo.ss_ymd, 4 );
	int_bcd( tmpEymd, mkcoinfo.ee_ymd, 4 );
	UCHAR	tmpCode[4] = {0};
	int_bcd( tmpCode, mkcoinfo.cocode, 4 );
	int	year = bcd_int( &tmpEymd[1], 2 );
	if( !(mkcoinfo.mkOpt1&0x02) ){
		if( co_lock_ms(mkcoinfo.apno, bcd_int(tmpCode, 4), year, 0x101) < 0 ){
			m_ErrMsg = _T("入力された会社コードは、他で使用中です。");//エラーメッセージ間違い？？修正'20.04.16
//			m_ErrMsg = _T("他で、同一の会社番号が登録されています！");
			m_Mctl.vctl_close();
			return -1;
		}
	}

	// 会社情報の取得
	coctl.c_cod[0] = tmpCode[0];
	coctl.c_cod[1] = tmpCode[1];
	coctl.c_cod[2] = tmpCode[2];
	coctl.c_cod[3] = tmpCode[3];
	CO_CTL_Array	coarray;
	ret = m_Mctl.vctl_coinf( &coctl, &coarray );

	if( ret == 0 ){	// DB マスターあり
		int	max = (int)coarray.GetCount();
		for( int i=0; i<max; i++ ){
			if( memcmp( &tmpEymd[1], &coarray[i].c_kikan[3], (sizeof(UCHAR))*3 ) == 0 ){
				// 同一のものアリ！
				m_ErrMsg = _T("入力された番号で会社が既に登録されています！");
				m_Mctl.vctl_close();
				return -1;
			}
		}
	}

	// 会社名
	int	slen = 0;
	if( slen = (int)strlen(mkcoinfo.c_name) ){
		memmove( coctl.c_name, mkcoinfo.c_name, min(sizeof(coctl.c_name), slen) );
	}
	else{
		memset( coctl.c_name, '\0', sizeof(coctl.c_name) );
	}

	// カナ
	if( slen = (int)strlen(mkcoinfo.c_kana) ){
		memmove( coctl.c_kana, mkcoinfo.c_kana, min(sizeof(coctl.c_kana), slen) );
	}
	else{
		memset( coctl.c_kana, '\0', sizeof(coctl.c_kana) );
	}

	// 決算期間
	memmove( coctl.c_kikan, &tmpSymd[1], 3 );
	memmove( &coctl.c_kikan[3], &tmpEymd[1], 3 );

	// マスタータイプ
	coctl.c_mtyp = m_mkZVolume.m_type;

	// サブシステム登録ＳＷ
	coctl.c_subsw = m_mkZVolume.sub_sw;

	// 業種番号
	coctl.c_gsno = 0xff;

	// 消費税属性
	coctl.c_exbin[0] = m_mkZVolume.s_sgn1;
	coctl.c_exbin[1] = m_mkZVolume.s_sgn2;
	coctl.c_exbin[2] = m_mkZVolume.s_sgn3;
	coctl.c_exbin[3] = m_mkZVolume.s_sgn4;
	coctl.c_exbin[4] = m_mkZVolume.s_sgn5;
	coctl.c_exbin[5] = m_mkZVolume.s_sgn6;
	coctl.c_exbin[6] = m_mkZVolume.s_sgn7;
	coctl.c_exbin[128] |= 0x01;				// 財務マスター未作成フラグ

//--> '15.07.10 INS START
	// 法人番号
	char	chkbuf[24]={0};
	if( memcmp(chkbuf, mkcoinfo.hojin_no, sizeof(chkbuf)) != 0 ){
		CArithEx	arith(0x16);
		arith.l_pack( coctl.c_hojinno, mkcoinfo.hojin_no, sizeof(chkbuf) );
	}
	else{
		memset( coctl.c_hojinno, '\0', sizeof(coctl.c_hojinno) );
	}
//<-- '15.07.10 INS END

	// 簡易体系マスター？
	// 個人業種サイン？

	// ボリュームコントロールへ追加
	ret = m_Mctl.vctl_add( &coctl );
	if( ret != 0 ){
		m_ErrMsg = _T("ボリュームコントロールの追加に失敗しました！");
		m_Mctl.vctl_close();
		return -1;
	}

	m_Mctl.vctl_close();

	return 0;
}

//-----------------------------------------------------------------------------
// DB の作成
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CreateDataBase( SYZMAKE_MKCOINFO &mkcoinfo )
{

	if ((mkcoinfo.mkOpt1 & 0x04)) {//既存マスターに申告書のみ作成　'20.04.16
		//該当テーブル削除処理
		//Coctlの情報みるようにして、のみマスターの場合にはこの処理通すようにする
		//CORECの情報取得
		int ret = 0;
		CO_CTL_DB coctl = { 0 };
		// ボリュームコントロールオープン
		if ((ret = m_Mctl.vctl_open(MODE_WRITE, mkcoinfo.apno, TYPE_MS, SORT_ASC)) != 0) {
			switch (ret) {
			case -1:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
			case -2:
				m_ErrMsg = _T("ＩＣＳマスターの登録がありません！");
				break;
			case -3:
				m_ErrMsg = _T("");
				break;
			default:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
			}
			if (ret == -2) {
				return -2;
			}
			else {
				return -1;
			}
		}

		// 会社情報の取得
		coctl.c_apno = mkcoinfo.apno;
		UCHAR	tmpCode[4] = { 0 };
		int_bcd(tmpCode, mkcoinfo.cocode, 4);
		coctl.c_cod[0] = tmpCode[0];
		coctl.c_cod[1] = tmpCode[1];
		coctl.c_cod[2] = tmpCode[2];
		coctl.c_cod[3] = tmpCode[3];

		if (m_Mctl.vctl_coinf(&coctl) != 0) {		// 登録なし
			m_ErrMsg = _T("指定された番号で会社は登録されていません！");
			m_Mctl.vctl_close();
			return -2;
		}

		m_Mctl.vctl_close();


		if (coctl.c_exbin[128] & 0x1) {	// 財務ｄｂマスター未作成

			//ここを関数化し、非連動登録プロから、申告書削除呼ばれた場合もいけるようにする。
//			int optionflg = 0;
//			if (DeleteSyzOnlyData(mkcoinfo, optionflg) < 0) {
//
//				return -1;
//			}

			//関数化前の処理
			
			CDatabase* db = m_DBAccess.GetDatabase( DBA_MST_ACCESS );
			CStringArray nameary;
			CString sql, dbname, name;
			CRecordset rset( db );

			UCHAR	tmpSymd[4]={0};
			UCHAR	tmpEymd[4]={0};
			int_bcd( tmpSymd, mkcoinfo.ss_ymd, 4 );
			int_bcd( tmpEymd, mkcoinfo.ee_ymd, 4 );
			dbname.Format( _T("Z%02x_C%08d_Y%02x%02x"), mkcoinfo.apno, mkcoinfo.cocode, tmpEymd[1], tmpEymd[2] );
			sql.Format( _T("select name from %s.sys.tables where name not like"), dbname );

			//減価償却ｄｂ
			if( coctl.c_subsw & 0x10000 ) {
				sql += _T( "(\'DEP_%%\')" );	// 「\'」「%%」にしなくてもよい？
				//---->所得税db対応 21.04.09
				if(coctl.c_subsw & 0x600000 || coctl.c_subsw & 0x40000 || coctl.c_subsw & 0x20 ) {
				//---
//				if( coctl.c_subsw & 0x40000 || coctl.c_subsw & 0x20 ) {
				//<----
					sql += _T( " and name not like" );
				}
			}
			// 法人税ｄｂ
			if( coctl.c_subsw & 0x40000 ) {
				sql += _T( "(\'HOJ_%%\')" );	// 「\'」「%%」にしなくてもよい？

				// 法人税と所得税は同居しないため、所得税マスターが存在するかチェックしない21.04.09
				if (coctl.c_subsw & 0x20) {
					sql += _T(" and name not like");
				}
			}

			if (coctl.c_subsw & 0x20) {//内訳書
				sql += _T("(\'uc_%%\')");	// 「\'」「%%」にしなくてもよい？

				//---->内訳書データある場合は削除前にvtblの内訳バージョンを取得しておく 20.09.08
				Vtbl_utiwakeback = 0;
				int ret = GetVtblUtiwakeVer(db, Utiwake_ver , mkcoinfo);
				if (ret == 0) {//レコードあり。あとで戻す
					Vtbl_utiwakeback = 1;
				}
				else if (ret == 1) {//レコード無し。削除する
					Vtbl_utiwakeback = 2;
				}
				//<-------------------------

				// 内訳書と所得税は同居しないため、所得税マスターが存在するかチェックしない21.04.09

			}

			//---> 所得税db対応 21.04.09
			if ( coctl.c_subsw & 0x600000 ) {
				sql += _T( "(\'KTS_%%\')" );
			}
			//<----

			rset.Open( CRecordset::forwardOnly, sql );
			if ( rset.IsBOF() == FALSE ) {
				do {
					rset.GetFieldValue( (short)0, name );
					nameary.Add( name );
					rset.MoveNext();
				} while ( rset.IsEOF() == FALSE );
			}
			rset.Close();

			int		ocrflg = 0;	// "ocrrdata" が存在するか

			for ( int i = 0; i < nameary.GetSize(); i++ ) {
				CString	name = nameary.GetAt( i );
				if( name.CompareNoCase(_T("ocrrdata")) == 0 ) {
					// "ocrrdata" は、ここでは消さない
					ocrflg = 1;
					continue;
				}

				//---->20.09.08
				//内訳書ある時はprfkin(決算書データ)、cpdata（表紙情報）を除外する 
				//また、Vtblの中にある内訳書バージョンを取得し、マスター作成後、戻す処理を追加する必要がある
				if (coctl.c_subsw & 0x20) {//内訳書
					if (name.CompareNoCase(_T("prfkin")) == 0) {
						continue;
					}
					if (name.CompareNoCase(_T("cpdata")) == 0) {
						continue;
					}
				}
				//<-----------------

				sql.Format( _T("drop table %s.dbo.%s"), dbname, name );
				db->ExecuteSQL( sql );
			}
			// "ocrrdata" があれば、最後に消す
			if( ocrflg ) {
				sql.Format( _T("drop table %s.dbo.ocrrdata"), dbname );
				db->ExecuteSQL( sql );
			}
			
		}
		//<---------------

	}
	else {
		// 
		CString	existsSql, createSql;
		existsSql.Format(_T("IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'%s') ")
			_T("DROP DATABASE [%s]"), m_mkCoName, m_mkCoName);
		createSql.Format(_T("CREATE DATABASE [%s]  ON (NAME = N'%s',FILENAME = N'%s\\%s.mdf')")
			_T("LOG ON (NAME = N'%s_log', FILENAME = N'%s\\%s_log.LDF', SIZE = 4MB, FILEGROWTH = 16MB)")
			_T("COLLATE Japanese_CI_AS"), m_mkCoName, m_mkCoName, m_phydisk, m_mkCoName, m_mkCoName, m_phydisk, m_mkCoName);

		CString	dboptSql;
		dboptSql.Format(_T("ALTER DATABASE [%s] SET AUTO_CLOSE OFF"), m_mkCoName);

		// 不正データベース削除
		if( !IsCloudEnv() ){
			// ATOMⅡクラウド環境以外のファイル参照が出来る場合には、直接ファイルを削除！
			CFileFind	find;
			CString		mdf, ldf, cdev, ncdev;
			_GetCommonDevicePath(mkcoinfo.dbSvrName, m_phydisk, cdev, ncdev);
			mdf.Format(_T("%s\\%s.mdf"), cdev, m_mkCoName);
			ldf.Format(_T("%s\\%s_log.ldf"), cdev, m_mkCoName);
			if( find.FindFile(mdf) == TRUE ) {
				_unlink(mdf);
			}
			if( find.FindFile(ldf) == TRUE ) {
				_unlink(ldf);
			}
			find.Close();
		}
		else{
			// DBマスターの削除
			int ret = 0;
			if( ret = DropDatabaseAllInCloud(mkcoinfo, true) ){
				return ret;
			}
		}

		// データベース作成
		if (m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, createSql) < 0) {
			m_DBAccess.GetErrorMessage(m_ErrMsg);
			return -1;
		}
		if (m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, dboptSql) < 0) {
			m_DBAccess.GetErrorMessage(m_ErrMsg);
			return -1;
		}
	}

	// データベーステーブル作成
	if( CreateInitZtable(mkcoinfo) == -1 ){
		if( m_ErrMsg.IsEmpty() == TRUE ){
			m_ErrMsg = _T("データベーステーブルの作成に失敗しました。");
		}
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// スクリプトファイルのパスを取得
//-----------------------------------------------------------------------------
// 引数	sqlPath		：	パス格納先
//		mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
void CSyzMakeSubW::GetZtableScriptPath( CString &sqlPath, SYZMAKE_MKCOINFO &mkcoinfo )
{
	char	fpath[_MAX_PATH]={0};

	if( mkcoinfo.mkOpt1&0x01 ){
		CString	tmpSql;
		char	grno[8]={0};
		// 財務側にインストールされている？
		if( (mkcoinfo.apno&0xff) <= BYOIN_APNO ){
			sprintf_s( grno, sizeof(grno), _T("1b") );
		}
		else if( (mkcoinfo.apno&0xff) == KOEKI_APNO ){
			sprintf_s( grno, sizeof(grno), _T("1e") );
		}
		else if( (mkcoinfo.apno&0xff) == KOEKI3_APNO ){
			sprintf_s( grno, sizeof(grno), _T("1f") );
		}
		else if( (mkcoinfo.apno&0xff) == SYAKAI_APNO ){
			sprintf_s( grno, sizeof(grno), _T("2e") );
		}
		else if( (mkcoinfo.apno&0xff) == GAKKO_APNO ){
			sprintf_s( grno, sizeof(grno), _T("28") );
		}
		else if( (mkcoinfo.apno&0xff) == SYUKYO_APNO ){
			sprintf_s( grno, sizeof(grno), _T("29") );
		}
		sprintf_s( fpath, sizeof(fpath), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs20"), mkcoinfo.pdev[0], grno );
		tmpSql.Format( _T("%s\\ztable.sql"), fpath );

		//---->22.02.25
		CFileFind	findZm, findSys;
		CTime		tZm, tSys;
		if( findZm.FindFile(tmpSql) ){
			findZm.FindNextFile();
			findZm.GetLastWriteTime(tZm);
			findZm.Close();

			char	fpath2[_MAX_PATH]={ 0 };
			sprintf_s(fpath2, sizeof(fpath2), _T("%c:\\ICSWin\\SYS"), mkcoinfo.sysdev[0]);
			CString	tmpSql2;
			tmpSql2.Format(_T("%s\\ztable.sql"), fpath2);
			if( findSys.FindFile(tmpSql2) ){
				findSys.FindNextFile();
				findSys.GetLastWriteTime(tSys);
				findSys.Close();

				if( tZm < tSys ){
					sprintf_s(fpath, sizeof(fpath), _T("%c:\\ICSWin\\SYS"), mkcoinfo.sysdev[0]);
				}
			}
		}
		else{
			sprintf_s(fpath, sizeof(fpath), _T("%c:\\ICSWin\\SYS"), mkcoinfo.sysdev[0]);
			findZm.Close();
		}
		//-----
//		CFileFind	find;
//		if( find.FindFile(tmpSql) == 0 ){
//			sprintf_s(fpath, sizeof(fpath), _T("%c:\\ICSWin\\SYS"), mkcoinfo.sysdev[0]);
//		}
//		find.Close();
		//<-----
	}
	else{
#ifdef _DEBUG
		if( (mkcoinfo.apno&0xff) <= BYOIN_APNO ){
			sprintf_s( fpath, sizeof(fpath), _T("%c:\\ICSWin\\CMDS\\GR1b\\Jobs20"), mkcoinfo.pdev[0] );
		}
		else{
			sprintf_s( fpath, sizeof(fpath), _T("%c:\\ICSWin\\CMDS\\GR1e\\Jobs20"), mkcoinfo.pdev[0] );
		}
#else
		sprintf_s( fpath, sizeof(fpath), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs20"), mkcoinfo.pdev[0], mkcoinfo.grno );
#endif

		CString	tmpSql;
		tmpSql.Format( _T("%s\\ztable.sql"), fpath );

		CFileFind	find;
		if( find.FindFile(tmpSql) == 0 ){
			find.Close();

			sprintf_s( fpath, sizeof(fpath), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs%s"), mkcoinfo.pdev[0], mkcoinfo.grno, mkcoinfo.jobno );
			tmpSql.Format( _T("%s\\ztable.sql"), fpath );
			if( find.FindFile(tmpSql) == 0 ){
				sprintf_s( fpath, sizeof(fpath), _T("%c:\\ICSWin\\SYS"), mkcoinfo.sysdev[0] );
			}
		}
		find.Close();
	}

	sqlPath = fpath;
}

#ifdef _20160613_CUT_
//-----------------------------------------------------------------------------
// データベース内 初期テーブルの作成
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CreateInitZtable( SYZMAKE_MKCOINFO &mkcoinfo )
{
	char		m_Zsql[_MAX_PATH];			// スクリプトファイル格納パス

//-- '14.11.05 --
//#ifdef _DEBUG
////	sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR1E\\Jobs20"), pdev[0], grno, jobno );
//	sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR1E\\Jobs20"), mkcoinfo.pdev[0] );
//#else
////	sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs%s"), pdev[0], grno, jobno );
//	sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs20"), mkcoinfo.pdev[0], mkcoinfo.grno );
//#endif
//---------------
#ifdef _DEBUG
	if( mkcoinfo.mkOpt1&0x01 ){
		sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs%s"), mkcoinfo.pdev[0], mkcoinfo.grno, mkcoinfo.jobno );
	}
	else{
		sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR1E\\Jobs20"), mkcoinfo.pdev[0] );
	}
#else
	if( mkcoinfo.mkOpt1&0x01 ){
		sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs%s"), mkcoinfo.pdev[0], mkcoinfo.grno, mkcoinfo.jobno );
	}
	else{
		sprintf_s( m_Zsql, sizeof(m_Zsql), _T("%c:\\ICSWin\\CMDS\\GR%s\\Jobs20"), mkcoinfo.pdev[0], mkcoinfo.grno );
	}
#endif
//---------------

	CString	strPath;
	strPath.Format( _T("%s\\ztable.sql"), m_Zsql );

	int dbaid = m_DBAccess.OpenDatabase( mkcoinfo.dbSvrName, m_mkCoName );
	if( dbaid < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -1;
	}

	int ret = m_DBAccess.ExecuteSQLScript( dbaid, strPath );
	if( ret < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
	}

	// 各業種対応
	if( (mkcoinfo.apno==KOEKI_APNO) || (mkcoinfo.apno==KOEKI3_APNO) || (mkcoinfo.apno==SYAKAI_APNO) || (mkcoinfo.apno==SYUKYO_APNO) || (mkcoinfo.apno==GAKKO_APNO) ){
		strPath.Format( _T("%s\\ztable2.sql"), m_Zsql );

		int dbaid = m_DBAccess.OpenDatabase( mkcoinfo.dbSvrName, m_mkCoName );
		if( dbaid < 0 ){
			m_DBAccess.GetErrorMessage( m_ErrMsg );
			return -1;
		}

		ret = m_DBAccess.ExecuteSQLScript( dbaid, strPath );
		if( ret < 0 ){
			m_DBAccess.GetErrorMessage( m_ErrMsg );
		}
	}
	m_DBAccess.CloseDatabase( dbaid );

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// データベース内 初期テーブルの作成
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CreateInitZtable( SYZMAKE_MKCOINFO &mkcoinfo )
{
	CString	sqlPath;
	GetZtableScriptPath( sqlPath, mkcoinfo );

	CString	strPath;
	strPath.Format( _T("%s\\ztable.sql"), sqlPath.GetBuffer() );

	int dbaid = m_DBAccess.OpenDatabase( mkcoinfo.dbSvrName, m_mkCoName );
	if( dbaid < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -1;
	}

	int ret = m_DBAccess.ExecuteSQLScript( dbaid, strPath );
	if( ret < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
	}

	// 各業種対応
	if( (mkcoinfo.apno==KOEKI_APNO) || (mkcoinfo.apno==KOEKI3_APNO) || (mkcoinfo.apno==SYAKAI_APNO) || (mkcoinfo.apno==SYUKYO_APNO) || (mkcoinfo.apno==GAKKO_APNO) ){
		strPath.Format( _T("%s\\ztable2.sql"), sqlPath.GetBuffer() );

		int dbaid = m_DBAccess.OpenDatabase( mkcoinfo.dbSvrName, m_mkCoName );
		if( dbaid < 0 ){
			m_DBAccess.GetErrorMessage( m_ErrMsg );
			return -1;
		}

		ret = m_DBAccess.ExecuteSQLScript( dbaid, strPath );
		if( ret < 0 ){
			m_DBAccess.GetErrorMessage( m_ErrMsg );
		}
	}
	m_DBAccess.CloseDatabase( dbaid );

	return 0;
}

//-----------------------------------------------------------------------------
// マスターの削除（会社情報）
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	削除対象
//		haveImg		：	原票会計用DBを保持しているかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//			-2		：	エラー（該当マスターなし）
//-----------------------------------------------------------------------------
int CSyzMakeSubW::DeleteMasterFromVctl( SYZMAKE_MKCOINFO &mkcoinfo, bool &haveImg )
{
	// 引数クリア
	haveImg = false;

	int ret = 0;
	CO_CTL_DB coctl = { 0 };

	// ボリュームコントロールオープン
	if( (ret = m_Mctl.vctl_open(MODE_WRITE, mkcoinfo.apno, TYPE_MS, SORT_ASC)) != 0 ){
		switch( ret ){
			case -1:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
			case -2:
				m_ErrMsg = _T("ＩＣＳマスターの登録がありません！");
				break;
			case -3:
				m_ErrMsg = _T("");
				break;
			default:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
		}
		if( ret == -2 ){
			return -2;
		}
		else{
			return -1;
		}
	}

	// 会社情報の取得
	coctl.c_apno = mkcoinfo.apno;
	UCHAR	tmpCode[4] = {0};
	int_bcd( tmpCode, mkcoinfo.cocode, 4 );
	coctl.c_cod[0] = tmpCode[0];
	coctl.c_cod[1] = tmpCode[1];
	coctl.c_cod[2] = tmpCode[2];
	coctl.c_cod[3] = tmpCode[3];

	if( mkcoinfo.ee_ymd == -1 ){	// 全削除
		if( m_Mctl.vctl_coinf(&coctl) != 0 ){		// 登録なし
			m_ErrMsg = _T("指定された番号で会社は登録されていません！");
			m_Mctl.vctl_close();
			return -2;
		}

		// 原票判定
		if( coctl.c_subsw&0x40 ){
			haveImg = true;
		}

		// ボリュームコントロールから削除
		ret = m_Mctl.vctl_del( tmpCode );
		if ( ret != 0 ) {
			m_ErrMsg = _T("ボリュームコントロールの削除に失敗しました。");
			m_Mctl.vctl_close();
			return -1;
		}

		// 会社アクセステーブル削除
		CString fil;
		fil.Format( _T("apno = %02d and cocode = %02x%02x%02x%02x"), mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3] );
		COAccess* acs = m_Mctl.coa_open( 1, fil, 0 );
		if( acs != NULL ){
			int rcnt = acs->GetRecordCount();
			while( rcnt > 0 ){
				acs->Delete();
				if( acs->MoveNext() != 0 ){
					break;
				}
				rcnt--;
			}
			m_Mctl.coa_close();
		}

		m_Mctl.vctl_close();
	}
	else{							// 指定削除
		CO_CTL_Array	coarray;
		if( m_Mctl.vctl_coinf(&coctl, &coarray) != 0 ){		// 登録なし
			m_ErrMsg = _T("指定された番号で会社は登録されていません！");
			m_Mctl.vctl_close();
			return -2;
		}
		int	dmax = (int)coarray.GetCount();

		// 該当年度あり？
		BOOL	isFind = FALSE;
		UCHAR	tmpEymd[4] = {0};
		int_bcd( tmpEymd, mkcoinfo.ee_ymd, 4 );
		for( int i=0; i<dmax; i++ ){
			if( memcmp( &tmpEymd[1], &coarray[i].c_kikan[3], (sizeof(UCHAR))*3 ) == 0 ){
				// あり！
				isFind = TRUE;
				// 原票判定
				if( coarray[i].c_subsw&0x40 ){
					haveImg = true;
				}
			}
		}
		if( isFind == FALSE ){
			m_ErrMsg = _T("指定された番号で会社は登録されていません！");
			m_Mctl.vctl_close();
			return -2;
		}

		if( dmax == 1 ){
			// ボリュームコントロールから削除
			ret = m_Mctl.vctl_del( tmpCode );
			if ( ret != 0 ) {
				m_ErrMsg = _T("ボリュームコントロールの削除に失敗しました。");
				m_Mctl.vctl_close();
				return -1;
			}

			// 会社アクセステーブル削除
			CString fil;
			fil.Format( _T("apno = %02d and cocode = %02x%02x%02x%02x"), mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3] );
			COAccess* acs = m_Mctl.coa_open( 1, fil, 0 );
			if( acs != NULL ){
				int rcnt = acs->GetRecordCount();
				while( rcnt > 0 ){
					acs->Delete();
					if( acs->MoveNext() != 0 ){
						break;
					}
					rcnt--;
				}
				m_Mctl.coa_close();
			}

			m_Mctl.vctl_close();
		}
		else if( dmax > 0 ){
			// ボリュームコントロールから削除
			ret = m_Mctl.vctl_del( tmpCode, mkcoinfo.ee_ymd );
			if ( ret != 0 ) {
				m_ErrMsg = _T("ボリュームコントロールの削除に失敗しました。");
				m_Mctl.vctl_close();
				return -1;
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// データベース全削除（ATOMⅡクラウド環境）
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	削除対象
//		haveImg		：	原票会計用DBを保持しているかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常
//			-1		：	エラー
//			-2		：	例外
//-----------------------------------------------------------------------------
int CSyzMakeSubW::DropDatabaseAllInCloud( SYZMAKE_MKCOINFO &mkcoinfo, bool haveImg )
{
	// 会社コード
	UCHAR	tmpCode[4] ={ 0 };
	int_bcd(tmpCode, mkcoinfo.cocode, 4);

	CString ZmDBstr, ImgDBstr;
	if( mkcoinfo.ee_ymd == -1 ){
		ZmDBstr.Format( _T("Z%02x_C%02x%02x%02x%02x_Y????"), mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3] );
		ImgDBstr.Format( _T("D%02x_C%02x%02x%02x%02x_Y????"), mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3] );
	}
	else{
		UCHAR	tmpSymd[4]={ 0 }, tmpEymd[4]={ 0 };
		int_bcd(tmpSymd, mkcoinfo.ss_ymd, 4);
		int_bcd(tmpEymd, mkcoinfo.ee_ymd, 4);
		ZmDBstr.Format( _T("Z%02x_C%02x%02x%02x%02x_Y%02x%02x"), mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3], tmpEymd[1], tmpEymd[2] );
		ImgDBstr.Format( _T("D%02x_C%02x%02x%02x%02x_Y%02x%02x"), mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3], tmpEymd[1], tmpEymd[2] );
	}

	int ret = 0;
	if( (ret = m_DBAccess.DropDatabase(DBA_MST_ACCESS, ZmDBstr)) < 0 ){
		m_DBAccess.GetErrorMessage(m_ErrMsg);
		return ret;
	}
	if( haveImg ){
		if( (ret = m_DBAccess.DropDatabase(DBA_MST_ACCESS, ImgDBstr)) < 0 ){
			m_DBAccess.GetErrorMessage(m_ErrMsg);
			return ret;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// データベース全削除
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	削除対象
//		haveImg		：	原票会計用DBを保持しているかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常
//			-1		：	エラー
//			-2		：	例外
//-----------------------------------------------------------------------------
int CSyzMakeSubW::DropDatabaseAll( SYZMAKE_MKCOINFO &mkcoinfo, bool haveImg )
{
	if( IsCloudEnv() ){
		// ATOMⅡクラウド環境では、DBファイルの存在確認は出来ないので別関数化！
		return DropDatabaseAllInCloud(mkcoinfo, haveImg);
	}

	int ret = 0, bret = FALSE;
	CFileFind find;

	CString cdev, scdev;
	_GetCommonDevicePath( mkcoinfo.dbSvrName, m_phydisk, cdev, scdev );

	// 会社コード
	UCHAR	tmpCode[4] = {0};
	int_bcd( tmpCode, mkcoinfo.cocode, 4 );

	CString fstr, dfstr;
	if( mkcoinfo.ee_ymd == -1 ){
		fstr.Format( _T("%s\\Z%02x_C%02x%02x%02x%02x_Y????.mdf"), cdev, mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3] );
		dfstr.Format( _T("%s\\D%02x_C%02x%02x%02x%02x_Y????.mdf"), cdev, mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3] );
	}
	else{
		UCHAR	tmpSymd[4]={0}, tmpEymd[4]={0};
		int_bcd( tmpSymd, mkcoinfo.ss_ymd, 4 );
		int_bcd( tmpEymd, mkcoinfo.ee_ymd, 4 );
		fstr.Format( _T("%s\\Z%02x_C%02x%02x%02x%02x_Y%02x%02x.mdf"), cdev, mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3], tmpEymd[1], tmpEymd[2] );
		dfstr.Format( _T("%s\\D%02x_C%02x%02x%02x%02x_Y%02x%02x.mdf"), cdev, mkcoinfo.apno, tmpCode[0], tmpCode[1], tmpCode[2], tmpCode[3], tmpEymd[1], tmpEymd[2] );
	}

	if( find.FindFile(fstr) ){
		do {
			bret = find.FindNextFile();
			CString name = find.GetFileTitle();
			if( (ret = m_DBAccess.DropDatabase(DBA_MST_ACCESS, name)) < 0 ){
				find.Close();
				m_DBAccess.GetErrorMessage(m_ErrMsg);
				return ret;
			}
		} while( bret == TRUE );
		find.Close();
	}

	if( find.FindFile(dfstr) ){
		do {
			bret = find.FindNextFile();
			CString name = find.GetFileTitle();
			if( (ret = m_DBAccess.DropDatabase(DBA_MST_ACCESS, name)) < 0 ){
				find.Close();
				m_DBAccess.GetErrorMessage(m_ErrMsg);
				return ret;
			}
		} while( bret == TRUE );
		find.Close();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ボリュームラベル初期化
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	初期化情報
//		bGetJzuser	：	上手くんユーザー情報を取得するかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::InitZVolume( SYZMAKE_MKCOINFO &mkcoinfo, bool bGetJzuser )
{
	// 財務ボリュームラベル
#ifdef _20160613_CUT_
//-- '14.11.05 --
//	m_mkZVolume.vver = 0;						// ボリュームラベルバージョン
//---------------
	if( mkcoinfo.mkOpt1&0x01 ){
		m_mkZVolume.vver = -1;					// ボリュームラベルバージョン
	}
	else{
		m_mkZVolume.vver = 0;					// ボリュームラベルバージョン
	}
//---------------
#else
	m_mkZVolume.vver = 0;						// ボリュームラベルバージョン
#endif
	m_mkZVolume.voidver = 0;					// 無効化バージョン
	m_mkZVolume.apno = mkcoinfo.apno;			// アプリケーションNo.
	m_mkZVolume.m_ver = NOW_MVER;				// マスターバージョン
	m_mkZVolume.m_type = 0;						// マスタータイプ
//-- '15.12.09 --
//	m_mkZVolume.g_hsw = 0;						// 月次保存月数
//---------------
	m_mkZVolume.g_hsw = mkcoinfo.g_hsw;			// 月次保存月数
//---------------
	m_mkZVolume.d_nall = 0;						// 全データ有り無しフラグ
	m_mkZVolume.z_fail = 0;						// 残高集計不良サイン
	m_mkZVolume.e_save = 0;						// 電子データ保存
	m_mkZVolume.z_efct = 0;						// 計算時の端数処理
	m_mkZVolume.v_cod = mkcoinfo.cocode;		// 会社コード
	m_mkZVolume.c_nam.Empty();					// 会社名称
	m_mkZVolume.c_nam.Format( _T("%s"), mkcoinfo.c_name );
	m_mkZVolume.ss_ymd = mkcoinfo.ss_ymd;		// 期首年月日
	m_mkZVolume.ee_ymd = mkcoinfo.ee_ymd;		// 期末年月日
	m_mkZVolume.cs_ymd = mkcoinfo.ss_ymd;		// 計算上期首年月日
	m_mkZVolume.ss_jsw = 0;						// 開始状態
	m_mkZVolume.ip_ymd = mkcoinfo.ss_ymd;		// 入力開始処理年月日

	m_mkZVolume.bmcol = 4;						// 部門桁数
	m_mkZVolume.kjcol = 8;						// 工事番号桁数
	m_mkZVolume.edcol = 4;						// 枝番桁数

	m_mkZVolume.zk_hn = 4;						// 科目残高　保存年数
	m_mkZVolume.zk_sg = 0xF;					// 科目残高　前４期分　登録サイン
	m_mkZVolume.zb_hn = 0;						// 枝番残高　保存年数
	m_mkZVolume.zb_sg = 0;						// 枝番残高　前４期分　登録サイン
	m_mkZVolume.br_sw = 0;						// 当期枝番残高登録サイン
	m_mkZVolume.ck_sw = 0x1;					// 資金繰残高登録サイン

	m_mkZVolume.sub_sw = mkcoinfo.sub_sw;		// サブシステム登録サイン
	if( m_mkZVolume.apno == KENSETU_APNO ){
		m_mkZVolume.sub_sw |= 0x4;
	}
	m_mkZVolume.a_tana = 0;						// 自動棚卸サイン
	m_mkZVolume.dpsgn = 0x1;					// 伝票番号使用サイン
	m_mkZVolume.evdsgn = 0;						// 証憑番号、手形期日入力サイン

	m_mkZVolume.bm_isw = 0;						// 部門番号　入力サイン
	m_mkZVolume.kj_isw = 0;						// 工事番号　入力サイン
	if( m_mkZVolume.apno == KENSETU_APNO ){
		m_mkZVolume.kj_isw = 0x1;
	}
	m_mkZVolume.br_nsw = 0;						// 枝番名称　出力サイン
	m_mkZVolume.kc_psw = 0;						// 科目コード　出力サイン

	m_mkZVolume.tk_ln = 40;						// 摘要入力文字数
	m_mkZVolume.tk_sw = 0;						// 連想摘要サイン
	m_mkZVolume.tk_br = 0;						// 摘要枝番サイン
	m_mkZVolume.in_opt = 0;						// 通常入力オプション
	m_mkZVolume.tl_cor = 0x10;					// 転送済み仕訳修正許可サイン
	m_mkZVolume.ank_in = 0x3;					// ＡＮＫ入力サイン

	m_mkZVolume.dlin_t = 0;						// 転送済みＳＮＯ　データ送信

	m_mkZVolume.s_mst = 0x1;					// 消費税マスター登録サイン
	m_mkZVolume.s_ver = 0x10;					// 消費税対応バージョン
	m_mkZVolume.s_sgn1 = 0;						// 分離・税込み区分
	if( mkcoinfo.s_sgn1&0xc0 ){
		if( (mkcoinfo.s_sgn1&0x0f) == 0 ){
			m_mkZVolume.s_sgn1 = ((mkcoinfo.s_sgn1&0xc0)|0x0f);
		}
		else if( (mkcoinfo.s_sgn1&0x02) == 0 ){
			m_mkZVolume.s_sgn1 = ((mkcoinfo.s_sgn1&0xcf)|0x02);
		}
		else{
			m_mkZVolume.s_sgn1 = mkcoinfo.s_sgn1;
		}
	}
	else{
		m_mkZVolume.s_sgn1 = mkcoinfo.s_sgn1;
	}
/*- '13.08.22 -*/
//	m_mkZVolume.s_sgn2 = 0;						// 課税方式区分
//	m_mkZVolume.s_sgn3 |= 0x3;					// 科目・摘要属性登録サイン
//	m_mkZVolume.s_sgn4 = 0;						// 処理選択サイン
//	m_mkZVolume.s_sgn4 |= 0x10;					// 処理選択サイン (08/01/08)
//	m_mkZVolume.s_sgn4 |= 0x20;
//	m_mkZVolume.s_sgn4 &= ~0x40;				// 元帳表記 (01/02/08)
//	m_mkZVolume.s_sgn6 = 0;						// 基本仕入区分サイン
/*-------------*/
	m_mkZVolume.s_sgn2 = 0;						// 課税方式区分
	m_mkZVolume.s_sgn2 |= mkcoinfo.s_sgn2;		// 課税方式区分
	m_mkZVolume.s_sgn3 = 0;
	m_mkZVolume.s_sgn3 |= 0x3;					// 科目・摘要属性登録サイン
	if( m_mkZVolume.s_sgn2&0x80 ){
		m_mkZVolume.s_sgn3 |= mkcoinfo.s_sgn3;	// 
	}
	m_mkZVolume.s_sgn4 = 0;						// 処理選択サイン
	m_mkZVolume.s_sgn4 |= 0x10;					// 処理選択サイン (08/01/08)
	m_mkZVolume.s_sgn4 |= 0x20;
	m_mkZVolume.s_sgn4 &= ~0x40;				// 元帳表記 (01/02/08)
	m_mkZVolume.s_sgn4 |= mkcoinfo.s_sgn4;
	m_mkZVolume.s_sgn6 = 0;						// 基本仕入区分サイン
	m_mkZVolume.s_sgn6 |= mkcoinfo.s_sgn6;		// 基本仕入区分サイン
	if( ((m_mkZVolume.s_sgn2&0x0f)==0) && (m_mkZVolume.s_sgn4&0x80) ){
		if( m_mkZVolume.s_sgn6 == 0 ){
			m_mkZVolume.s_sgn6 = 1;
		}
	}
/*-------------*/
	m_mkZVolume.s_sgn5 = 0;						// 一括税抜き　処理月サイン
	m_mkZVolume.s_sgn7 = 0;						// 税入力方式サイン
	m_mkZVolume.ind_sw = 0;						// 個人月別収支サイン
	m_mkZVolume.ind_kk = 0;						// 個人みなし法人サイン
	m_mkZVolume.ind_vl.Empty();					// 個人控除額

	m_mkZVolume.sch_ca1 = 0;					// 学校資金収支サイン
	m_mkZVolume.sch_ca2 = 0;					// 学校現預金明細
	m_mkZVolume.sch_ca3.Empty();				// 学校前年度支払金
	m_mkZVolume.sch_ca4.Empty();				// 学校翌年度支払金
	m_mkZVolume.ocr_idev = 0;					// イメージファイル存在デバイス

	m_mkZVolume.kes_type = 0;					// 決算修正フラグ

	::ZeroMemory( m_mkZVolume.z_tana, sizeof(m_mkZVolume.z_tana) );		// 自動棚卸サイン
	m_mkZVolume.ss_ymd1 = -1;					// 期首年月日（前期）
	m_mkZVolume.ee_ymd1 = -1;					// 期末年月日（前期）
	m_mkZVolume.ss_ymd2 = -1;					// 期首年月日（前々期）
	m_mkZVolume.ee_ymd2 = -1;					// 期末年月日（前々期）
	m_mkZVolume.ss_ymd3 = -1;					// 期首年月日（３期前）
	m_mkZVolume.ee_ymd3 = -1;					// 期末年月日（３期前）
	m_mkZVolume.ss_ymd4 = -1;					// 期首年月日（４期前）
	m_mkZVolume.ee_ymd4 = -1;					// 期末年月日（４期前）

	m_mkZVolume.ki = mkcoinfo.nKi;				// 決算期

	m_mkZVolume.ind_type = 0;					// 個人業種タイプ
	if( mkcoinfo.nIfn&0x0f ){
		m_mkZVolume.ind_type |= (mkcoinfo.nIfn&0x0f);
	}
	if( mkcoinfo.nIfn&0x10 ){
		m_mkZVolume.ind_type |= 0x10;
	}
	m_mkZVolume.so_tumisgn = 0xA9;				// 総額積み上げサイン

	m_mkZVolume.ind_table = 0;					// 個人業種テーブルサイン	0x00=標準　0x01=兼業
	if( mkcoinfo.nIfn&0x100 ){
		m_mkZVolume.ind_table = 1;
	}
	m_mkZVolume.ind_kjpriority = 0;				// 個人控除優先順位
	if( mkcoinfo.nIfn&0xf0000 ){
		m_mkZVolume.ind_kjpriority |= ((mkcoinfo.nIfn&0xf0000)>>16);
	}
	m_mkZVolume.ind_kjlimit = 0;				// 個人不動産控除１０万限度
	m_mkZVolume.ind_sgn1 = 0;					// 個人サイン
	m_mkZVolume.ind_sgn2 = 0;					// 個人サイン
	if( mkcoinfo.nIfn&0xff000000 ){
		m_mkZVolume.ind_sgn2 |= ((mkcoinfo.nIfn&0xff000000)>>24);
	}
	m_mkZVolume.ind_sgn3 = 0;					// 個人サイン rsv

	m_mkZVolume.cnocol = 7;						// 伝票番号桁数
	m_mkZVolume.snocol = 4;						// 証憑番号桁数

/*	memset( m_Kana, '\0', sizeof(m_Kana) );	// カナ
	m_nSime = 0;
	m_nNsMonth = 0;
	m_sftyp = 1;

	// 初期値セーブ
	CopyZVolume( m_mkZVolume, m_SvZVolume );*/

	m_owner_id = mkcoinfo.owner_id;
	if( mkcoinfo.plcusr_sw&0x01 ){
		m_plcusr = TRUE;
	}
	else{
		m_plcusr = FALSE;
	}

	// 会社アクセステーブル初期化
	if( bGetJzuser ){
		m_AcsAry.RemoveAll();
		m_JkAcsAry.RemoveAll();
		MakeJkAcsAry();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 上手君ユーザ情報作成
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CSyzMakeSubW::MakeJkAcsAry()
{
	m_jCoUserAry.RemoveAll();

	COAccess* acs;
	if( (acs=m_Mctl.coa_open(1, _T(""), 0)) != NULL ){
		int cnt = acs->GetRecordCount();
		for( int i=0; i<m_jUserAry.GetSize(); i++ ){
			BOOL rsw=FALSE, rsw2=FALSE, msw=FALSE;
			mctl_usertbl2 utbl = m_jUserAry.GetAt( i );

			// 無効ＩＤチェック
			if( utbl.entry_stop > 0 ){
				msw = TRUE;
//				continue;
			}

			// 使用開始終了年月日チェック
			SYSTEMTIME st, sst, est;
			GetLocalTime( &st );
			utbl.entry_sdate.GetAsSystemTime( sst );
			utbl.entry_edate.GetAsSystemTime( est );

			if( (st.wYear<sst.wYear) || 
				((st.wYear<=sst.wYear) && (st.wMonth<sst.wMonth)) ||
				((st.wYear<=sst.wYear) && (st.wMonth<=sst.wMonth) && (st.wDay<sst.wDay)) ){
				msw = TRUE;
//				continue;
			}
			if( (est.wYear<st.wYear) ||
				((est.wYear<=st.wYear) && (est.wMonth<st.wMonth)) ||
				((est.wYear<=st.wYear) && (est.wMonth<=st.wMonth) && (est.wDay<st.wDay)) ){
				msw = TRUE;
//				continue;
			}

			acs->MoveFirst();
			for( int j=0; j<cnt; j++ ){
				// 給与db対応 (11/02/25)
//				if ( ((acs->apno != m_Vol.apno) || (acs->cocode != m_Vol.v_cod)) && (utbl.user_id == acs->user_id) ) {
				if ( (acs->apno<=ZAIMUEND_APNO) && ((acs->apno!=m_mkZVolume.apno) || (acs->cocode!=m_mkZVolume.v_cod)) && (utbl.user_id==acs->user_id) ){
					// 他業種で使用済みチェック
					rsw = TRUE;
					break;
				}
				if( (acs->apno==m_mkZVolume.apno) && (acs->cocode==m_mkZVolume.v_cod) && (utbl.user_id==acs->user_id) ){
					// 使用済みチェック
					rsw2 = TRUE;
					break;
				}
				if( acs->MoveNext() < 0 ){
					break;
				}
			}

			if( (rsw == FALSE) && (msw == FALSE) ){		// 未使用ＩＤ＆使用可能
//			if ( (rsw == FALSE) ) {		// 未使用ＩＤ＆使用可能
				m_jCoUserAry.Add( utbl );
			}
//-- CUT --
//			else if ( type&0x1 ) {		// 修正時
//				if ( (rsw2 == TRUE) )
//					m_jCoUserAry.Add( utbl );
//			}
//---------
		}
		m_Mctl.coa_close();
	}
	else {
		m_jCoUserAry.Copy( m_jUserAry );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 月別情報データの初期化
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	初期化情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::InitMonthInfo( SYZMAKE_MKCOINFO &mkcoinfo )
{
	// 締日
	m_nSime = 0;
	if( mkcoinfo.nSime == -1 ){
		m_nSime = mkcoinfo.ee_ymd%100;
	}
	else{
		m_nSime = mkcoinfo.nSime;
	}

	if( (m_nSime<0) || (31<mkcoinfo.nSime) ){
		// エラー
		return -1;
	}

/*	// 期首・期末・計算上期首年月日 平成変換
	UCHAR	ssymd[3]={0}, eeymd[3]={0}, csymd[3]={0};
	UCHAR	ssy[3]={0}, eey[3]={0};
	int_bcd( ssy, (m_mkZVolume.ss_ymd%1000000), 3 );
	int_bcd( eey, (m_mkZVolume.ee_ymd%1000000), 3 );
	int_bcd( ssymd, (m_mkZVolume.ss_ymd%1000000), 3 );
	int_bcd( eeymd, (m_mkZVolume.ee_ymd%1000000), 3 );
	int_bcd( csymd, (m_mkZVolume.cs_ymd%1000000), 3 );

	m_VolDate.db_datecnv( ssymd, ssymd, 0, 0 );
	m_VolDate.db_datecnv( eeymd, eeymd, 0, 0 );
	m_VolDate.db_datecnv( csymd, csymd, 0, 0 );*/

	// 期首・期末・計算上期首年月日（西暦→和暦）
	int ssymd=0, eeymd=0, csymd=0, ssgengo=0, eegegno=0, csgengo=0;
	m_VolDate.db_datecnvGen( 0, m_mkZVolume.ss_ymd, &ssgengo, &ssymd, 0, 0 );
	m_VolDate.db_datecnvGen( 0, m_mkZVolume.ee_ymd, &eegegno, &eeymd, 0, 0 );
	m_VolDate.db_datecnvGen( 0, m_mkZVolume.cs_ymd, &csgengo, &csymd, 0, 0 );

	UCHAR	bcd_ssymd[3]={0}, bcd_eeymd[3]={0}, bcd_csymd[3]={0};
	int_bcd( bcd_ssymd, ssymd, 3 );
	int_bcd( bcd_eeymd, eeymd, 3 );
	int_bcd( bcd_csymd, csymd, 3 );

//	if( m_VolDate.db_datesetex(ssymd, eeymd, csymd, m_nSime, m_Dapac) < 0 ){
//		return -1;
//	}
	if( m_VolDate.db_datesetexGen(ssgengo, bcd_ssymd, eegegno, bcd_eeymd, csgengo, bcd_csymd, m_nSime, m_Dapac) < 0 ){
		return -1;
	}

	// 決算修正サイン更新
	UpdateKIsgn();

	// 当期決算期間より前期決算期間を取得
//	UCHAR pssymd[3] = { 0 }, peeymd[3] = { 0 };
//	m_VolDate.vd_getprevdateex( peeymd, ssy );
//	m_VolDate.vd_getsymd( pssymd, peeymd );

	// 当期決算期間より前期決算期間を取得（和暦）
	int pregengo=0;
	UCHAR pssymd[3]={0}, peeymd[3]={0}, tmpymd[3]={0};
	m_VolDate.vd_getprevdateexGen( &pregengo, tmpymd, ssgengo, bcd_ssymd );
	// 和暦→西暦
	int tmpgengo=0;
	m_VolDate.db_datecnvGen( pregengo, tmpymd, &tmpgengo, peeymd, 1, 0 );
	// 西暦で期末日より期首日を計算
	m_VolDate.vd_getsymd( pssymd, peeymd );

	// 前期月別情報取得
	for( int i=0; i<4; i++ ){
		if( i != 0 ){
			// 期首年取得
			if( pssymd[0] != 0x00 ){
				int y = bcd_int( &pssymd[0], 1 );
				y--;
				int_bcd( &pssymd[0], y, 1 );
			}
			else{
				pssymd[0] = 0x99;
			}
			// 期末年取得
			if( peeymd[0] != 0x00 ){
				int y = bcd_int( &peeymd[0], 1 );
				y--;
				int_bcd( &peeymd[0], y, 1 );
			}
			else{
				peeymd[0] = 0x99;
			}
		}
		m_VolDate.vd_getfebeday( pssymd );
		m_VolDate.vd_getfebeday( peeymd );

//		UCHAR pssy[3] = {0}, peey[3] = {0};
//		m_VolDate.db_datecnv( pssymd, pssy, 0, 0 );
//		m_VolDate.db_datecnv( peeymd, peey, 0, 0 );

		// 西暦→和暦
		UCHAR pssy[3] = {0}, peey[3] = {0};
		int	psgengo=0, pegengo=0;
		m_VolDate.db_datecnvGen( 0, pssymd, &psgengo, pssy, 0, 0 );
		m_VolDate.db_datecnvGen( 0, peeymd, &pegengo, peey, 0, 0 );

//		if( m_VolDate.db_dateset(pssy, peey, m_nSime, &m_Dapac[(i+1)*MONTHALL]) < 0 ){
		if( m_VolDate.db_datesetGen(psgengo, pssy, pegengo, peey, m_nSime, &m_Dapac[(i+1)*MONTHALL]) < 0 ){
			// 閏年チェック
			if( (pssy[1] == 0x2) && (pssy[2] == 0x29) ){
				pssy[2] = 0x28;
			}
			else if ( (peey[1] == 0x2) && (peey[2] == 0x29) ){
				peey[2] = 0x28;
			}
			else{
				return -1;
			}
//			if( m_VolDate.db_dateset(pssy, peey, m_nSime, &m_Dapac[(i+1)*MONTHALL]) < 0 ){
			if( m_VolDate.db_datesetGen(psgengo, pssy, pegengo, peey, m_nSime, &m_Dapac[(i+1)*MONTHALL]) < 0 ){
				return -1;
			}
		}
	}

	for( int i=0; i<MONTHALL*5; i++ ){
		m_MSgengo[i] = m_Dapac[i].Sgengo;
		memmove( m_MSymd[i], m_Dapac[i].Symd, 3 );
		m_MEgengo[i] = m_Dapac[i].Egengo;
		memmove( m_MEymd[i], m_Dapac[i].Eymd, 3 );
	}

/*	// 人数
	int	*pnMNdt1 = m_pBaseInfoDlg->GetNinzu( m_mkZVolume.apno );
	// 粗利益
	int	*pnMNdt2 = m_pBaseInfoDlg->GetArari( m_mkZVolume.apno );
	if( pnMNdt1 && pnMNdt2 ){
		for( int i=0; i<MONTHALL*5; i++ ){
			m_nMNdt1[i] = pnMNdt1[i];
			m_nMNdt2[i] = pnMNdt2[i];
		}
	}*/

	// 西暦→和暦
	UCHAR	ipymd[3]={0};
	int		iymd = 0;
	int		igengo=0;
//	m_VolDate.db_datecnv( m_mkZVolume.ip_ymd, &iymd, 0, 0 );
	m_VolDate.db_datecnvGen( 0, m_mkZVolume.ip_ymd, &igengo, &iymd, 0, 0 );
	int_bcd( ipymd, iymd, 3 );

	int ofs1 = -1, ofs2 = -1, ofs3 = -1, ofs4 = -1;
	for( int i=0; i<MONTHALL; i++ ){
		switch( m_KIsgn[0][i]&0x0f ){
			case 0x1:
				ofs1 = i;
				break;
			case 0x2:
				ofs2 = i;
				break;
			case 0x3:
				ofs3 = i;
				break;
			case 0x4:
				ofs4 = i;
				break;
			default:
				break;
		}
	}

	// 入力許可サイン
	int isg = 0;
	int ipsw = 0;
	memset( m_nInsw, '\0', sizeof(m_nInsw) );
	for( int i=0; i<MONTHALL; i++ ){
		if( CmpWarekiYMD(igengo, &ipymd[0], m_MSgengo[i], &m_MSymd[i][0], 2) == 0 ){
			if( ipymd[2] >= m_MSymd[i][2] ){
				if( CmpWarekiYMD(igengo, &ipymd[0], m_MEgengo[i], &m_MEymd[i][0], 2) == 0 ){
					if( ipymd[2] <= m_MEymd[i][2] ){
						isg = 1;
					}
				}
				else{
					isg = 1;
				}
			}
		}
		else if( CmpWarekiYMD(igengo, &ipymd[0], m_MEgengo[i], &m_MEymd[i][0], 2) == 0 ){
			if( ipymd[2] <= m_MEymd[i][2] ){
				isg = 1;
			}
		}
		int svisg = isg;
		if( isg == 1 ){
			if( i == ofs4 ){		// 期末決算修正サイン
				isg = (m_nKsgn < 3) ? 1 : 0;
			}
			else if( i == ofs2 ){		// 中間決算修正サイン
				int svisg = isg;
				if( isg ){
					isg = (m_nKsgn < 2) ? 1 : 0;
				}
			}
			else if( (i==ofs1) || (i==ofs3) ){		// 第１四半期・第３四半期決算修正サイン
				int svisg = isg;
				if( isg ){
					isg = (m_nKsgn < 1) ? 1 : 0;
				}
			}
			if( (ipsw==0) && (m_mkZVolume.ss_jsw&0x10) ){
				isg = 0;
				ipsw = 1;
			}
		}

		if( i < TSCOUNT ){		// 当期分
			m_nInsw[i] = isg;
		}
		else{		// 翌期分
			m_nInsw[i] = 0;
		}

		if( (ipsw==1) && (m_mkZVolume.ss_jsw&0x10) ){
			isg = svisg;
			ipsw = 2;
		}
		if( (i==ofs1) || (i==ofs2) || (i==ofs3) ){		// 第１四半～第３四半決算修正サイン
			isg = svisg;
		}
		else if( i == ofs4 ){
			isg = 0;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 和暦年月日の比較
//-----------------------------------------------------------------------------
// 引数	gengo1	：	元号１
//		pYmd1	：	年月日１
//		gengo2	：	元号２
//		pYmd2	：	年月日２
//		cmpNum	：	比較バイト数（3:年月日, 2:年月, 1:年）
//-----------------------------------------------------------------------------
// 返送値	負	：	２が、より未来
//			０	：	等しい
//			正	：	１が、より未来
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CmpWarekiYMD( int gengo1, BYTE* pYmd1, int gengo2, BYTE* pYmd2, int cmpNum/*=3*/ )
{
	if( gengo1 < gengo2 ){
		return -1;
	}
	else if( gengo1 > gengo2 ){
		return 1;
	}
	else{
		return memcmp( pYmd1, pYmd2, cmpNum );
	}
}

#ifdef _20190621_CUT_
//-----------------------------------------------------------------------------
// 決算修正サイン更新
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::UpdateKIsgn()
{
	memset( m_KIsgn[0], '\0', sizeof(BYTE)*MONTHALL );

	// 当期月別情報テーブル
/*	BYTE ssymd[3]={0}, eeymd[3]={0}, ipymd[3]={0};
	int_bcd( ssymd, (m_mkZVolume.ss_ymd%1000000), 3 );
	int_bcd( eeymd, (m_mkZVolume.ee_ymd%1000000), 3 );
	int_bcd( ipymd, (m_mkZVolume.ip_ymd%1000000), 3 );
	_db_datecnv( ssymd, ssymd, 0, 0 );		// 西暦→平成変換
	_db_datecnv( eeymd, eeymd, 0, 0 );
	_db_datecnv( ipymd, ipymd, 0, 0 );*/
	// 西暦→和暦
	int nSsymd=0, nEeymd=0, nIpymd=0, ssgengo=0, eegegno=0, ipgengo=0;
	db_datecnvGen( 0, m_mkZVolume.ss_ymd, &ssgengo, &nSsymd, 0, 0 );
	db_datecnvGen( 0, m_mkZVolume.ee_ymd, &eegegno, &nEeymd, 0, 0 );
	db_datecnvGen( 0, m_mkZVolume.ip_ymd, &ipgengo, &nIpymd, 0, 0 );
	BYTE ssymd[3]={0}, eeymd[3]={0}, ipymd[3]={0};
	int_bcd( ssymd, nSsymd, 3 );
	int_bcd( eeymd, nEeymd, 3 );
	int_bcd( ipymd, nIpymd, 3 );

	for( int i=0, kct=1, kncnt=0; i<(TSCOUNT-1); i++ ){
		// 決算期間内チェック
		if( (memcmp(ssymd, m_Dapac[i].Eymd, 3)<=0) && (memcmp(m_Dapac[i].Eymd, eeymd, 3)<=0) ){
			m_KIsgn[0][i] |= 0x20;
			kncnt++;
		}
		else{
			continue;
		}

		// 決算修正月チェック
		if( (kct!=-1) && (memcmp(m_Dapac[i].Symd, m_Dapac[i+1].Symd, 3)==0) && (memcmp(m_Dapac[i].Eymd, m_Dapac[i+1].Eymd, 3)==0) ){
			BYTE kisgn = m_KIsgn[0][i];
			i++;
			// 最終決算修正月チェック
			if( memcmp(eeymd, m_Dapac[i].Eymd, 3) == 0 ){
				m_KIsgn[0][i] |= (kisgn|4);
				kct = -1;
//				m_Jugyo[kncnt]	= m_Jugyo[kncnt-1];
//				m_Doctor[kncnt]	= m_Doctor[kncnt-1];
//				m_Seito[kncnt]	= m_Seito[kncnt-1];
//				m_Sikiti[kncnt]	= m_Sikiti[kncnt-1];
//				m_nArari[kncnt]	= m_nArari[kncnt-1];
				m_KJsgn[0][i]	&= ~0x1;
			}
			else{
				m_KIsgn[0][i] |= (kisgn|kct);
				kct++;
			}
			kncnt++;
		}
	}

	// 入力許可チェック
	int ofs1=-1, ofs2=-1, ofs3=-1, ofs4=-1;
	for( int i=0; i<MONTHALL; i++ ){
		switch( m_KIsgn[0][i]&0xF ){
			case 0x1:
				ofs1 = i;
				break;
			case 0x2:
				ofs2 = i;
				break;
			case 0x3:
				ofs3 = i;
				break;
			case 0x4:
				ofs4 = i;
				break;
			default:
				break;
		}
	}

	int isg = 0;
	for( int i=0; i<TSCOUNT; i++ ){
		if( memcmp(ipymd, m_Dapac[i].Symd, 2) == 0 ){
			if( ipymd[2] >= m_Dapac[i].Symd[2] ){
				if( memcmp( ipymd, m_Dapac[i].Eymd, 2 ) == 0 ){
					if( ipymd[2] <= m_Dapac[i].Eymd[2] ){
						isg = 1;
					}
				}
				else{
					isg = 1;
				}
			}
		}
		else if( memcmp(ipymd, m_Dapac[i].Eymd, 2) == 0 ){
			if( ipymd[2] <= m_Dapac[i].Eymd[2] ){
				isg = 1;
			}
		}

		int svisg = isg;
		if( isg == 1 ){
			if( i == ofs4 ){		// 期末決算修正サイン
				isg = (m_nKsgn < 3) ? 1 : 0;
			}
			else if( i == ofs2 ){		// 中間決算修正サイン
				int svisg = isg;
				if( isg ){
					isg = (m_nKsgn < 2) ? 1 : 0;
				}
			}
			else if( (i==ofs1) || (i==ofs3) ){		// 第１四半期・第３四半期決算修正サイン
				int svisg = isg;
				if( isg ){
					isg = (m_nKsgn < 1) ? 1 : 0;
				}
			}
		}

		if( i < TSCOUNT ){		// 当期分
			if( isg == 1 ){
				m_KIsgn[0][i] |= 0x10;
			}
			else{
				m_KIsgn[0][i] &= ~0x10;
			}
		}
		else{		// 翌期分
			m_KIsgn[0][i] &= ~0x10;
		}

		if( (i==ofs1) || (i==ofs2) || (i==ofs3) ){		// 第１四半～第３四半決算修正サイン
			isg = svisg;
		}
		else if( (i==ofs4) ){
			isg = 0;
		}
	}

	// 前期月別情報テーブル
	for( int i = 1; i < 5; i++ ){
		memset( m_KIsgn[i], '\0', sizeof(BYTE)*MONTHALL );
		for( int j=0; j<TSCOUNT; j++ ){
			switch( j ){
				case AM1_OFFSET:
					m_KIsgn[i][j] |= 0x01;
					break;
				case IAM_OFFSET:
					m_KIsgn[i][j] |= 0x02;
					break;
				case AM3_OFFSET:
					m_KIsgn[i][j] |= 0x03;
					break;
				case TAM_OFFSET:
					m_KIsgn[i][j] |= 0x04;
					break;
			}
			m_KIsgn[i][j] |= 0x20;
		}
	}

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// 決算修正サイン更新
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::UpdateKIsgn()
{
	memset(m_KIsgn[0], '\0', sizeof(BYTE)*MONTHALL);

	// 当期月別情報テーブル
/*	BYTE ssymd[3]={0}, eeymd[3]={0}, ipymd[3]={0};
	int_bcd( ssymd, (m_mkZVolume.ss_ymd%1000000), 3 );
	int_bcd( eeymd, (m_mkZVolume.ee_ymd%1000000), 3 );
	int_bcd( ipymd, (m_mkZVolume.ip_ymd%1000000), 3 );
	_db_datecnv( ssymd, ssymd, 0, 0 );		// 西暦→平成変換
	_db_datecnv( eeymd, eeymd, 0, 0 );
	_db_datecnv( ipymd, ipymd, 0, 0 );*/
	// 西暦→和暦
	int nSsymd=0, nEeymd=0, nIpymd=0, ssgengo=0, eegegno=0, ipgengo=0;
	db_datecnvGen(0, m_mkZVolume.ss_ymd, &ssgengo, &nSsymd, 0, 0);
	db_datecnvGen(0, m_mkZVolume.ee_ymd, &eegegno, &nEeymd, 0, 0);
	db_datecnvGen(0, m_mkZVolume.ip_ymd, &ipgengo, &nIpymd, 0, 0);
	BYTE ssymd[3]={ 0 }, eeymd[3]={ 0 }, ipymd[3]={ 0 };
	int_bcd(ssymd, nSsymd, 3);
	int_bcd(eeymd, nEeymd, 3);
	int_bcd(ipymd, nIpymd, 3);

	for( int i=0, kct=1, kncnt=0; i<(TSCOUNT-1); i++ ){
		// 決算期間内チェック
		if( (CmpWarekiYMD(ssgengo, ssymd, m_Dapac[i].Egengo, m_Dapac[i].Eymd)<=0) && (CmpWarekiYMD(m_Dapac[i].Egengo, m_Dapac[i].Eymd, eegegno, eeymd)<=0) ){
			m_KIsgn[0][i] |= 0x20;
			kncnt++;
		}
		else{
			continue;
		}

		// 決算修正月チェック
		if( (kct!=-1) && (CmpWarekiYMD(m_Dapac[i].Sgengo, m_Dapac[i].Symd, m_Dapac[i+1].Sgengo, m_Dapac[i+1].Symd)==0) && (CmpWarekiYMD(m_Dapac[i].Egengo, m_Dapac[i].Eymd, m_Dapac[i+1].Egengo, m_Dapac[i+1].Eymd)==0) ){
			BYTE kisgn = m_KIsgn[0][i];
			i++;
			// 最終決算修正月チェック
			if( CmpWarekiYMD(eegegno, eeymd, m_Dapac[i].Egengo, m_Dapac[i].Eymd) == 0 ){
				m_KIsgn[0][i] |= (kisgn|4);
				kct = -1;
				//				m_Jugyo[kncnt]	= m_Jugyo[kncnt-1];
				//				m_Doctor[kncnt]	= m_Doctor[kncnt-1];
				//				m_Seito[kncnt]	= m_Seito[kncnt-1];
				//				m_Sikiti[kncnt]	= m_Sikiti[kncnt-1];
				//				m_nArari[kncnt]	= m_nArari[kncnt-1];
				m_KJsgn[0][i]	&= ~0x1;
			}
			else{
				m_KIsgn[0][i] |= (kisgn|kct);
				kct++;
			}
			kncnt++;
		}
	}

	// 入力許可チェック
	int ofs1=-1, ofs2=-1, ofs3=-1, ofs4=-1;
	for( int i=0; i<MONTHALL; i++ ){
		switch( m_KIsgn[0][i]&0xF ){
		case 0x1:
			ofs1 = i;
			break;
		case 0x2:
			ofs2 = i;
			break;
		case 0x3:
			ofs3 = i;
			break;
		case 0x4:
			ofs4 = i;
			break;
		default:
			break;
		}
	}

	int isg = 0;
	for( int i=0; i<TSCOUNT; i++ ){
		if( CmpWarekiYMD(ipgengo, ipymd, m_Dapac[i].Sgengo, m_Dapac[i].Symd, 2) == 0 ){
			if( ipymd[2] >= m_Dapac[i].Symd[2] ){
				if( CmpWarekiYMD(ipgengo, ipymd, m_Dapac[i].Egengo, m_Dapac[i].Eymd, 2) == 0 ){
					if( ipymd[2] <= m_Dapac[i].Eymd[2] ){
						isg = 1;
					}
				}
				else{
					isg = 1;
				}
			}
		}
		else if( CmpWarekiYMD(ipgengo, ipymd, m_Dapac[i].Egengo, m_Dapac[i].Eymd, 2) == 0 ){
			if( ipymd[2] <= m_Dapac[i].Eymd[2] ){
				isg = 1;
			}
		}

		int svisg = isg;
		if( isg == 1 ){
			if( i == ofs4 ){		// 期末決算修正サイン
				isg = (m_nKsgn < 3) ? 1 : 0;
			}
			else if( i == ofs2 ){		// 中間決算修正サイン
				int svisg = isg;
				if( isg ){
					isg = (m_nKsgn < 2) ? 1 : 0;
				}
			}
			else if( (i==ofs1) || (i==ofs3) ){		// 第１四半期・第３四半期決算修正サイン
				int svisg = isg;
				if( isg ){
					isg = (m_nKsgn < 1) ? 1 : 0;
				}
			}
		}

		if( i < TSCOUNT ){		// 当期分
			if( isg == 1 ){
				m_KIsgn[0][i] |= 0x10;
			}
			else{
				m_KIsgn[0][i] &= ~0x10;
			}
		}
		else{		// 翌期分
			m_KIsgn[0][i] &= ~0x10;
		}

		if( (i==ofs1) || (i==ofs2) || (i==ofs3) ){		// 第１四半～第３四半決算修正サイン
			isg = svisg;
		}
		else if( (i==ofs4) ){
			isg = 0;
		}
	}

	// 前期月別情報テーブル
	for( int i = 1; i < 5; i++ ){
		memset(m_KIsgn[i], '\0', sizeof(BYTE)*MONTHALL);
		for( int j=0; j<TSCOUNT; j++ ){
			switch( j ){
			case AM1_OFFSET:
				m_KIsgn[i][j] |= 0x01;
				break;
			case IAM_OFFSET:
				m_KIsgn[i][j] |= 0x02;
				break;
			case AM3_OFFSET:
				m_KIsgn[i][j] |= 0x03;
				break;
			case TAM_OFFSET:
				m_KIsgn[i][j] |= 0x04;
				break;
			}
			m_KIsgn[i][j] |= 0x20;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ウエイトダイアログ表示
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ShowWaitDialog( CWnd *pParent )
{
	if( m_pWdlg == NULL ){
		m_pWdlg = new CDBWaitDialog;

		if( m_pWdlg != NULL ){
			if( m_pWdlg->Create( IDD_DIALOG_DBWAIT, pParent ) == FALSE ){
				HideWaitDialog( pParent );
				return 0;
			}
		}
	}

	if( m_pWdlg != NULL ){
		m_pWdlg->UpdateWindow();
	}

	pParent->EnableWindow( FALSE );

	return 0;
}

//-----------------------------------------------------------------------------
// ウエイトダイアログ非表示
//-----------------------------------------------------------------------------
int CSyzMakeSubW::HideWaitDialog( CWnd *pParent )
{
	if( m_pWdlg != NULL ){
		m_pWdlg->SendMessage( WM_COMMAND, IDCANCEL );
		m_pWdlg->DestroyWindow();
		delete m_pWdlg;
		m_pWdlg = NULL;
	}

	pParent->EnableWindow( TRUE );

	return 0;
}

//-----------------------------------------------------------------------------
// タイトルセット
//-----------------------------------------------------------------------------
int CSyzMakeSubW::SetWaitTitle( LPCTSTR pTitle )
{
	if( m_pWdlg ){
		m_pWdlg->SetWaitText( pTitle );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 会社コード登録チェック ('17.04.10)
//-----------------------------------------------------------------------------
// 引数	cocode	：	会社コード
//		ym		：	年度
//-----------------------------------------------------------------------------
// 返送値	1	：	登録あり
//			0	：	登録なし
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkRegistCoCode( SYZMAKE_MKCOINFO &mkcoinfo )
{
	int			ret = 0;
	CO_CTL_DB	coctl={0}, svcoctl={0};

	// ボリュームコントロールオープン
	if( (ret = m_Mctl.vctl_open(MODE_READ, mkcoinfo.apno, TYPE_MS)) != 0 ){
		switch( ret ){
			case	-1:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
			case	-2:
				m_ErrMsg = _T("ＩＣＳマスターの登録がありません！");
				break;
			case	-3:
				m_ErrMsg = _T("");
				break;
			default:
				m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
				break;
		}
		return -1;
	}

	// 会社情報の取得
	UCHAR	tmpCode[4] = {0};
	int_bcd( tmpCode, mkcoinfo.cocode, 4 );
	coctl.c_cod[0] = tmpCode[0];
	coctl.c_cod[1] = tmpCode[1];
	coctl.c_cod[2] = tmpCode[2];
	coctl.c_cod[3] = tmpCode[3];
	CO_CTL_Array	coarray;
	ret = m_Mctl.vctl_coinf( &coctl, &coarray );

	UCHAR	tmpSymd[4] = {0};
	UCHAR	tmpEymd[4] = {0};
	int_bcd( tmpSymd, mkcoinfo.ss_ymd, 4 );
	int_bcd( tmpEymd, mkcoinfo.ee_ymd, 4 );

	if( ret == 0 ){	// DB マスターあり
		int	max = (int)coarray.GetCount();
		for( int i=0; i<max; i++ ){
			if( memcmp( &tmpEymd[1], &coarray[i].c_kikan[3], (sizeof(UCHAR))*3 ) == 0 ){
				m_Mctl.vctl_close();
				return 1;
			}
		}
	}

	m_Mctl.vctl_close();

	return 0;
}

//-----------------------------------------------------------------------------
// zvolumeの再作成('17.05.01)
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ReMakeZvolume( SYZMAKE_MKCOINFO &mkcoinfo )
{
	static	char	*ZVOLUME_TBNAME = _T("zvolume");

	if( InitZVolume(mkcoinfo, false) ){
		m_ErrMsg = _T("ボリュームラベル情報の初期化に失敗しました！");
		return -1;
	}

	// DB アクセスクラスオープン
	if( m_DBAccess.OpenDatabaseEx() < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -1;
	}

	// 会社名作成
	UCHAR	tmpSymd[4]={0};
	UCHAR	tmpEymd[4]={0};
	int_bcd( tmpSymd, mkcoinfo.ss_ymd, 4 );
	int_bcd( tmpEymd, mkcoinfo.ee_ymd, 4 );
	memset( m_mkCoName, '\0', sizeof(m_mkCoName) );
	sprintf_s( m_mkCoName, sizeof(m_mkCoName), _T("Z%02x_C%08d_Y%02x%02x"), mkcoinfo.apno, mkcoinfo.cocode, tmpEymd[1], tmpEymd[2] );

	char buf[2048] = { 0 };
	MakeVolumeDataSql( buf, mkcoinfo );
	CString sql;

	// 既存データクリア
	sql.Format( _T("DELETE FROM [%s].[dbo].[%s]"), m_mkCoName, ZVOLUME_TBNAME );
	if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, sql) < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -1;
	}

	sql.Format( _T("INSERT INTO [%s].[dbo].[%s] VALUES (%s)"), m_mkCoName, ZVOLUME_TBNAME, buf );
	if( m_DBAccess.ExecuteSQL(DBA_MST_ACCESS, sql) < 0 ){
		m_DBAccess.GetErrorMessage( m_ErrMsg );
		return -1;
	}

	m_DBAccess.CloseDatabase();

	if( UpdateZmsubSubsw(mkcoinfo) < 0 ){
		m_ErrMsg = _T("番号情報の更新に失敗しました！");
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// マスターを作成する時の必須項目をチェック('18.08.21)
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成情報
//-----------------------------------------------------------------------------
// 返送値	0		：	作成可能
//			-1		：	作成不可
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckEssentialItem( SYZMAKE_MKCOINFO &mkcoinfo )
{
	// 業種区分
	if( mkcoinfo.apno == 0xff ){
		m_ErrMsg = _T("業種区分が未設定です！");
		return -1;
	}
	// 会社コード
	if( (mkcoinfo.cocode<0) || (99999999<mkcoinfo.cocode) ){
		m_ErrMsg = _T("会社コードが不正です！");
		return -1;
	}
	// 決算期間
	if( (mkcoinfo.ss_ymd==-1) || (mkcoinfo.ee_ymd==-1) ){
		m_ErrMsg = _T("決算期間が未設定です！");
		return -1;
	}
	if( mkcoinfo.ee_ymd < NCM_YMD ){
		if( (mkcoinfo.apno==HOJIN_APNO) || (mkcoinfo.apno==UNSO_APNO) || (mkcoinfo.apno==KENSETU_APNO) ){
			m_ErrMsg = _T("新会社法対象期間外のマスターは作成できません");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < KH_TS_YMD ){
		if( mkcoinfo.apno == KOEKI_APNO ){
			m_ErrMsg = _T("平成18年4月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < KH3_TS_YMD ){
		if( mkcoinfo.apno == KOEKI3_APNO ){
			m_ErrMsg = _T("平成20年12月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < SFH_TS_YMD ){
		if( mkcoinfo.apno == SYAKAI_APNO ){
			m_ErrMsg = _T("平成12年4月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < SFH_SKTS_YMD ){
		if( (mkcoinfo.apno==SYAKAI_APNO) && (mkcoinfo.nptyp&0x01) ){
			m_ErrMsg = _T("平成24年4月1日以降の期間を対象とします！");
			return -1;
		}
	}
	if( mkcoinfo.ss_ymd < ALL_TS_YMD ){
		m_ErrMsg = _T("平成12年1月1日以降の期間を対象とします！");
		return -1;
	}

	return 0;
}



//-----------------------------------------------------------------------------
// ボリュームコントロール更新 '20.04.16
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::UpdateMasterToVlctl(SYZMAKE_MKCOINFO &mkcoinfo)
{
	int			ret = 0;
	CO_CTL_DB	coctl = { 0 }, svcoctl = { 0 };

	// ボリュームコントロールオープン
	if ((ret = m_Mctl.vctl_open(MODE_WRITE, mkcoinfo.apno, TYPE_MS)) != 0) {
		switch (ret) {
		case	-1:
			m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
			break;
		case	-2:
			m_ErrMsg = _T("ＩＣＳマスターの登録がありません！");
			break;
		case	-3:
			m_ErrMsg = _T("");
			break;
		default:
			m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
			break;
		}
		return -1;
	}

	// 会社数チェック
	DT_CTL_DB	dtctl;
	m_Mctl.vctl_hedder(&dtctl);
	if (dtctl.d_max <= dtctl.d_cnt) {
		m_ErrMsg = _T("会社登録数が最大登録可能数に達しているため\n会社を登録することはできません！");
		m_Mctl.vctl_close();
		return -1;
	}

	// 会社ロックチェック
	UCHAR	tmpSymd[4] = { 0 };
	UCHAR	tmpEymd[4] = { 0 };
	int_bcd(tmpSymd, mkcoinfo.ss_ymd, 4);
	int_bcd(tmpEymd, mkcoinfo.ee_ymd, 4);
	UCHAR	tmpCode[4] = { 0 };
	int_bcd(tmpCode, mkcoinfo.cocode, 4);
	int	year = bcd_int(&tmpEymd[1], 2);
	if (!(mkcoinfo.mkOpt1 & 0x02)) {
		if (co_lock_ms(mkcoinfo.apno, bcd_int(tmpCode, 4), year, 0x101) < 0) {
			m_ErrMsg = _T("入力された会社コードは、他で使用中です。");//エラーメッセージ間違い？？修正'20.04.16
//			m_ErrMsg = _T("他で、同一の会社番号が登録されています！");
			m_Mctl.vctl_close();
			return -1;
		}
	}

	// 会社情報の取得
	coctl.c_cod[0] = tmpCode[0];
	coctl.c_cod[1] = tmpCode[1];
	coctl.c_cod[2] = tmpCode[2];
	coctl.c_cod[3] = tmpCode[3];
	CO_CTL_Array	coarray;
	ret = m_Mctl.vctl_coinf(&coctl, &coarray);


	INT_PTR max = coarray.GetSize();
	for (int i = 0; i < max; i++) {
		CO_CTL_DB co = coarray.GetAt(i);
		if (!memcmp(&co.c_kikan[3], &tmpEymd[1], 3)) {
			memmove(&coctl, &co, sizeof(CO_CTL_DB));
			//			sw = 1;
			break;
		}
	}

	// 会社名
	int	slen = 0;
	if (slen = (int)strlen(mkcoinfo.c_name)) {
		memset(coctl.c_name, '\0', sizeof(coctl.c_name));//20.08.27
		memmove(coctl.c_name, mkcoinfo.c_name, min(sizeof(coctl.c_name), slen));
	}
	else {
		memset(coctl.c_name, '\0', sizeof(coctl.c_name));
	}

	// カナ
	if (slen = (int)strlen(mkcoinfo.c_kana)) {
		memset(coctl.c_kana, '\0', sizeof(coctl.c_kana));//20.08.27
		memmove(coctl.c_kana, mkcoinfo.c_kana, min(sizeof(coctl.c_kana), slen));
	}
	else {
		memset(coctl.c_kana, '\0', sizeof(coctl.c_kana));
	}

	// 決算期間
	memmove(coctl.c_kikan, &tmpSymd[1], 3);
	memmove(&coctl.c_kikan[3], &tmpEymd[1], 3);

	// マスタータイプ
	coctl.c_mtyp = m_mkZVolume.m_type;

	// サブシステム登録ＳＷ
	coctl.c_subsw = m_mkZVolume.sub_sw;

	// 業種番号
	coctl.c_gsno = 0xff;

	// 消費税属性
	coctl.c_exbin[0] = m_mkZVolume.s_sgn1;
	coctl.c_exbin[1] = m_mkZVolume.s_sgn2;
	coctl.c_exbin[2] = m_mkZVolume.s_sgn3;
	coctl.c_exbin[3] = m_mkZVolume.s_sgn4;
	coctl.c_exbin[4] = m_mkZVolume.s_sgn5;
	coctl.c_exbin[5] = m_mkZVolume.s_sgn6;
	coctl.c_exbin[6] = m_mkZVolume.s_sgn7;
	coctl.c_exbin[128] |= 0x01;				// 財務マスター未作成フラグ

//--> '15.07.10 INS START
	// 法人番号
	char	chkbuf[24] = { 0 };
	if (memcmp(chkbuf, mkcoinfo.hojin_no, sizeof(chkbuf)) != 0) {
		CArithEx	arith(0x16);
		memset(coctl.c_hojinno, '\0', sizeof(coctl.c_hojinno));
		arith.l_pack(coctl.c_hojinno, mkcoinfo.hojin_no, sizeof(chkbuf));
	}
	else {
		memset(coctl.c_hojinno, '\0', sizeof(coctl.c_hojinno));
	}
	//<-- '15.07.10 INS END

		// 簡易体系マスター？
		// 個人業種サイン？

		// ボリュームコントロール更新
	ret = m_Mctl.vctl_comnt(coctl.seq, &coctl);
	if (ret != 0) {
		m_ErrMsg = _T("ボリュームコントロールの更新に失敗しました！");
		m_Mctl.vctl_close();
		return -1;
	}

	m_Mctl.vctl_close();

	return 0;
}

//<------------------------

//-----------------------------------------------------------------------------
// 消費税申告書に関連するデータの削除 ('20.02.20)
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	削除するマスター情報
// 引数	optionflg	：	削除オプション（ビット管理）
//					：　□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□■
//					：　// D0　ON:MCTL＆zvolume情報更新(sub_swのOFF処理)
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::DeleteSyzData(SYZMAKE_MKCOINFO &mkcoinfo , int optionflg)
{

	int ret = 0;

	if (m_Mctl.SetDatabase(mkcoinfo.dbSvrName) < 0) {
		m_ErrMsg = _T("マスターデータベースのオープンに失敗しました！");
		return -1;
	}

	CDatabase	*pDatabase = m_Mctl.GetCDatabase();
	if (pDatabase == NULL) {
		m_ErrMsg = _T("マスターデータベースの取得に失敗しました！");
		return -1;
	}

	// ボリュームコントロールオープン
	if ((ret = m_Mctl.vctl_open(MODE_WRITE, mkcoinfo.apno, TYPE_MS, SORT_ASC)) != 0) {
		switch (ret) {
		case -1:
			m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
			break;
		case -2:
			m_ErrMsg = _T("ＩＣＳマスターの登録がありません！");
			break;
		case -3:
			m_ErrMsg = _T("");
			break;
		default:
			m_ErrMsg = _T("ボリュームコントロールのオープンに失敗しました！");
			break;
		}
		if (ret == -2) {
			return -2;
		}
		else {
			return -1;
		}
	}


	CString	dbname;
	UCHAR	tmpSymd[4] = { 0 };
	UCHAR	tmpEymd[4] = { 0 };
	int_bcd(tmpSymd, mkcoinfo.ss_ymd, 4);
	int_bcd(tmpEymd, mkcoinfo.ee_ymd, 4);
	dbname.Format(_T("Z%02x_C%08d_Y%02x%02x"), mkcoinfo.apno, mkcoinfo.cocode, tmpEymd[1], tmpEymd[2]);

	// 本体削除テーブル一覧リスト
	const	char*	droptblname[] = { _T("reask"), _T("splist"), _T("refundvol"), _T("refundrec"), _T("shininfo"), _T("shinpayment"), _T("shinkinfo"), _T("shinminfo")
										, _T("SYZ_SNK_LST_VAL"), _T("SYZ_SNK_LST_RATE"), _T("syz_sozoku"), _T("fhyo60rec"), _T("simku_eigyobi"), _T("simku_syukujitsu")
										, _T("simku_kyujitsu"), _T("simku_settingholiday"), _T("simku_syukeitenki"), _T("syz_tokurei"), _T("") };
	// 内容削除テーブル一覧リスト
	const	char*	deltblname[] = { _T("szsnk"), _T("skjrec"), _T("kjsnk"), _T("szskj"), _T("") };

	// owntb の申告書に関する行
	const	char*	aplname[] = { _T("SIMULATE_TAX"), _T("DBSYZSHIN"), _T("DBSYZBASIC"), _T("") };

	try {
		pDatabase->BeginTrans();

		// 本体削除
		CString	Sql;
		for (int i = 0;; i++) {
			if (strlen(droptblname[i]) == 0) {
				break;
			}

			Sql.Format(_T("if exists (select * from [%s].[dbo].[sysobjects] where id = object_id(N'[%s].[dbo].[%s]') ) drop table [%s].[dbo].[%s]"), dbname, dbname, droptblname[i], dbname, droptblname[i]);//'20.08.25
//			Sql.Format(_T("if exists (select * from dbo.sysobjects where id = object_id(N'[%s].[dbo].[%s]') ) drop table [%s].[dbo].[%s]"), dbname, droptblname[i], dbname, droptblname[i]);
			pDatabase->ExecuteSQL(Sql);
		}

		// 内容削除
		for (int i = 0;; i++) {
			if (strlen(deltblname[i]) == 0) {
				break;
			}

			Sql.Format(_T("if exists (select * from [%s].[dbo].[sysobjects] where id = object_id(N'[%s].[dbo].[%s]') ) delete from [%s].[dbo].[%s]"), dbname, dbname, deltblname[i], dbname, deltblname[i]);//'20.08.25
//			Sql.Format(_T("if exists (select * from dbo.sysobjects where id = object_id(N'[%s].[dbo].[%s]') ) delete from [%s].[dbo].[%s]"), dbname, droptblname[i], dbname, droptblname[i]);
			pDatabase->ExecuteSQL(Sql);
		}

		// szvolの更新
		Sql.Format(_T("update [%s].[dbo].[szvol] set Svsasw = 0"), dbname);
		pDatabase->ExecuteSQL(Sql);

		// owntbの行削除
		for (int i = 0;; i++) {
			if (strlen(aplname[i]) == 0) {
				break;
			}

			Sql.Format(_T("delete from [%s].[dbo].[owntb] where apl_name = '%s'"), dbname, aplname[i]);
			pDatabase->ExecuteSQL(Sql);
		}

		pDatabase->CommitTrans();


		//D0ビットONの場合、MCTL＆zvolumeも更新
		if (optionflg & 0x01) {

			
			CO_CTL_DB coctl = { 0 };
			
			// 会社情報の取得
			coctl.c_apno = mkcoinfo.apno;
			UCHAR	tmpCode[4] = { 0 };
			int_bcd(tmpCode, mkcoinfo.cocode, 4);
			coctl.c_cod[0] = tmpCode[0];
			coctl.c_cod[1] = tmpCode[1];
			coctl.c_cod[2] = tmpCode[2];
			coctl.c_cod[3] = tmpCode[3];

			//---->20.08.06
			//複数年度削除時、会社情報更新が当年を更新してしまっていたのを修正

			UCHAR	tmpSymd[4] = { 0 };
			UCHAR	tmpEymd[4] = { 0 };
			int_bcd(tmpSymd, mkcoinfo.ss_ymd, 4);
			int_bcd(tmpEymd, mkcoinfo.ee_ymd, 4);

			CO_CTL_Array	coarray;
			ret = m_Mctl.vctl_coinf(&coctl, &coarray);

			INT_PTR max = coarray.GetSize();
			for (int i = 0; i < max; i++) {
				CO_CTL_DB co = coarray.GetAt(i);
				if (!memcmp(&co.c_kikan[3], &tmpEymd[1], 3)) {
					memmove(&coctl, &co, sizeof(CO_CTL_DB));
					break;
				}
			}

			//corec内c_subsw、消費税非連動サインOFF
			coctl.c_subsw &= ~0x20000;

			ret = m_Mctl.vctl_comnt(coctl.seq, &coctl);
			if (ret != 0) {
				m_ErrMsg = _T("ボリュームコントロールの更新に失敗しました！");
				m_Mctl.vctl_close();
				return -1;
			}

			//zvolume内sub_sw、消費税非連動サインOFF
			Sql.Empty();
			Sql.Format(_T("update [%s].[dbo].[zvolume] set sub_sw = %d"), dbname, coctl.c_subsw);
			pDatabase->ExecuteSQL(Sql);
			//-----
			/*
			if (m_Mctl.vctl_coinf(&coctl) != 0) {		// 登録なし
				m_ErrMsg = _T("指定された番号で会社は登録されていません！");
				m_Mctl.vctl_close();
				return -2;
			}


			//corec内c_subsw、消費税非連動サインOFF
			coctl.c_subsw &= ~0x20000;

			ret = m_Mctl.vctl_comnt(coctl.seq, &coctl);
			if (ret != 0) {
				m_ErrMsg = _T("ボリュームコントロールの更新に失敗しました！");
				m_Mctl.vctl_close();
				return -1;
			}


			//zvolume内sub_sw、消費税非連動サインOFF
			Sql.Empty();
			Sql.Format(_T("update [%s].[dbo].[zvolume] set sub_sw = %d"), dbname, coctl.c_subsw);
			pDatabase->ExecuteSQL(Sql);
			*/
			//<-----

		}

		m_Mctl.vctl_close();//'20.07.28


	}
	catch (CDBException *pDbe) {
		pDatabase->Rollback();
		pDbe->Delete();

		m_Mctl.vctl_close();

		return -1;
	}
//	pDatabase->CommitTrans();

	return 0;
}

//---->20.09.08
//==================================================================
//内訳書Vtblバージョン取得
//	----------------------------------------------------------
// 引数	db		：	チェック対象データベース
//		nVver	：	バージョンの取得先
//	mkcoinfo	：	マスター情報
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			1	：	バージョン登録なし
//			-1	：	エラー
//==================================================================
int CSyzMakeSubW::GetVtblUtiwakeVer(CDatabase* db, int &uVver, SYZMAKE_MKCOINFO &mkcoinfo)
{

	int	rt = 0;

	CString m_ErrMsg;

	CString dbname;
	UCHAR	tmpSymd[4] = { 0 };
	UCHAR	tmpEymd[4] = { 0 };
	int_bcd(tmpSymd, mkcoinfo.ss_ymd, 4);
	int_bcd(tmpEymd, mkcoinfo.ee_ymd, 4);
	dbname.Format(_T("Z%02x_C%08d_Y%02x%02x"), mkcoinfo.apno, mkcoinfo.cocode, tmpEymd[1], tmpEymd[2]);


	CRecordset	tmpVtbl(db);
	CString	sql, strVers;
	sql.Format("SELECT vers from [%s].[dbo].vtbl where objname = \'DBUchiwake\'" , dbname);

	try {
		tmpVtbl.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if (!tmpVtbl.IsEOF()) {
			tmpVtbl.GetFieldValue((SHORT)0, strVers);
		}
		else {
			// バージョン登録なし
			rt = 1;
		}
	}
	catch (CDBException *pDe) {
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}
	tmpVtbl.Close();

	// 変換
	uVver = atoi(strVers);

	return rt;

}


//==================================================================
//内訳書Vtblバージョン更新
//	----------------------------------------------------------
// 引数	db		：	更新対象データベース
//		nVver	：	バージョン
//		mode	：	1:削除＆戻す　２：削除のみ
//	mkcoinfo	：	マスター情報
//-----------------------------------------------------------------------------
// 返送値	0	：	更新正常終了
//			-1	：	エラー
//==================================================================
int CSyzMakeSubW::UpdateVtblUtiwakeVer(CDatabase* db, int &uVver, int mode , SYZMAKE_MKCOINFO &mkcoinfo)
{
	CString m_ErrMsg;
	CString sql, sql2;
	int	rt = 0;

	CString dbname;
	UCHAR	tmpSymd[4] = { 0 };
	UCHAR	tmpEymd[4] = { 0 };
	int_bcd(tmpSymd, mkcoinfo.ss_ymd, 4);
	int_bcd(tmpEymd, mkcoinfo.ee_ymd, 4);
	dbname.Format(_T("Z%02x_C%08d_Y%02x%02x"), mkcoinfo.apno, mkcoinfo.cocode, tmpEymd[1], tmpEymd[2]);


	sql.Format("delete from [%s].[dbo].[vtbl] where objname = \'DBUchiwake\'", dbname);
	sql2.Format("insert into [%s].[dbo].[vtbl] values( 1001, \'DBUchiwake\', %d )", dbname , uVver);

	try {
		//レコード削除
		if (mode == 1 || mode == 2) {
			db->ExecuteSQL(sql);
		}

		if (mode == 1) {//戻す必要あるケースの場合は戻す処理も同時に行う
			db->ExecuteSQL(sql2);
		}
	}
	catch (CDBException *pDe) {
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return -1;
	}

	return 0;

}

//<---------------


//-----------------------------------------------------------------------------
// //消費税申告書のデータクリア処理（会社登録課税区分切り替え時に呼び出し） '22.11.29
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	財務クラス
// 引数	optionflg	：	データクリアオプション（現在未使用。今後の為用に作成）
//					：　□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□
//					：　// D0　
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ClearSyzShinData(CDBZmSub *pZmsub, int optionflg)
{
	int rt = 0;
	int openflg = 0;//　1：モジュール側テーブルオープンszsnk 

	if (pZmsub->szsnk == NULL) {
		if (pZmsub->SzsnkOpen() != 0) {
			m_ErrMsg = _T("消費税テーブルオープンに失敗しました。");
			return -1;
		}
		openflg = 1;//モジュール側オープンフラグON
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	//												初期化処理開始														//
	//（元アプリ側（消費税申告書db非連動）登録プロより抜き出し。処理コメントはそのまま使用）							//
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	pZmsub->szsnk->Edit();
	// 消費税履歴保存対応 (13/05/22)
//	pZmsub->szsnk->Sn_SKKBN = 1;			// 申告区分
	pZmsub->szsnk->Sn_KOJIN = 0;			// 個人以外の事業者         ON :12ヶ月計算
	pZmsub->szsnk->Sn_KOANB = 0;			// 個人事業者　ON:月数按分, OFF:12ヶ月計算
	pZmsub->szsnk->Sn_KOANB = 0;			// 個人事業者　ON:月数按分, OFF:12ヶ月計算

	// 消費税履歴保存対応 (13/05/22)
//	pZmsub->szsnk->Sn_MDAYS = 0;			// 中間申告の計算期間  ［　　〃　　］
//	pZmsub->szsnk->Sn_MDAYE = 0;			// 　　　　　　　　　  ［　　〃　　］
//	pZmsub->szsnk->Sn_SPECIAL = 0;		// D0: ON 簡易計算する

	pZmsub->szsnk->Sn_KAPPU = 0;			// 割賦基準の適用　　　１＝有　０＝無
	pZmsub->szsnk->Sn_NOBE = 0;			// 延払基準の適用　　　１＝有　０＝無
	pZmsub->szsnk->Sn_KOJI = 0;			// 工事進行基準の適用　１＝有　０＝無
	pZmsub->szsnk->Sn_GENKN = 0;			// 現金主義会計の適用　１＝有　０＝無
	// 消費税履歴保存対応 (13/05/22)
//	pZmsub->szsnk->Sn_TUMIAG = 0;			// 規則22条１項の適用　１＝有　０＝無
	// 課税方式を変更した場合でも変更しない (10/05/31)
//	pZmsub->szsnk->Sn_KZTNSK = 0;			// 課税期間短縮　　　　１＝有　０＝無
	// 消費税履歴保存対応 (13/05/22)
//	pZmsub->szsnk->Sn_MCOUNT = 0;			// 中間申告　年間回数　３＝３回 １＝１回
//	pZmsub->szsnk->Sn_JCOUNT = 0;			// 中間申告　現在申告状態　１～４回
	pZmsub->szsnk->Sn_TOKUR = 0;			// 特例計算摘要ＳＷ
	pZmsub->szsnk->Sn_UPERS = 0;			// 課税売上割合９５％以上のＳＷ
	pZmsub->szsnk->Sn_TYPE = 0;			// 試行署及び試行署以外サインD0:ON試行署用紙

	pZmsub->szsnk->Sn_KSTDM = _T("0");	// 課税標準額（自動車）　［ＢＩＮ１２桁］
	pZmsub->szsnk->Sn_KSTD = _T("0");		// 　　〃　　（その他）　［ＢＩＮ１２桁］
	pZmsub->szsnk->Sn_TIKAM = _T("0");	// 対価の返還等をした金額（自動車）［〃］
	pZmsub->szsnk->Sn_TIKA = _T("0");		// 　　　　　〃　　　　　（その他）［〃］
	pZmsub->szsnk->Sn_SYTXM = _T("0");	// 消費税額（自動車）　　［ＢＩＮ１２桁］
	pZmsub->szsnk->Sn_SYTX = _T("0");		// 　　〃　（その他）　　［　　 〃 　　］

	pZmsub->szsnk->Sn_KJADJZ = _T("0");	// 控除過大調整税額・一般［　　 〃 　　］
											// 貸倒回収に係税額・簡易［　　 〃 　　］

	pZmsub->szsnk->Sn_SIREZ = _T("0");	// 控除対象仕入税額　　　［　　 〃 　　］
	pZmsub->szsnk->Sn_HKANZ = _T("0");	// 返還等対価に係る税額　［　　 〃 　　］
	pZmsub->szsnk->Sn_KSITZ = _T("0");	// 貸倒れに係る税額　　　［　　 〃 　　］

	pZmsub->szsnk->Sn_TYNOFZ = _T("0");	// 中間納付税額　　　　　［　　 〃 　　］
	pZmsub->szsnk->Sn_EDNOFZ = _T("0");	// 最終納付税額　　　　　［　　 〃 　　］
	pZmsub->szsnk->Sn_KAKTIZ = _T("0");	// 既確定税額　　　　　　［　　 〃 　　］

	pZmsub->szsnk->Sn_TTYWAR = _T("0");	// 地方消費税　中間納付譲渡割額
	pZmsub->szsnk->Sn_TKAKTZ = _T("0");	// 　　〃　　　既確定譲渡割額
	pZmsub->szsnk->Sn_TEDNOF = _T("0");	// 　　〃　　　最終納付税額

//	pZmsub->szsnk->Sn_KZURI = _T("0");	// 課税売上高　　　　　　［　　 〃 　　］
	pZmsub->szsnk->Sn_SOURI = _T("0");	// 総売上高　　　　　　　［　　 〃 　　］

	for (int i = 0; i < 5; i++) {
		pZmsub->szsnk->Sn_KURI[i] = _T("0");	// 第１種～５種事業　課税売上　［　　 〃 　　］
	}

	for (int i = 0; i < 5; i++) {
		pZmsub->szsnk->Sn_UP[i] = 0;			// 第１種～５種事業　売上割合
	}

	pZmsub->szsnk->Sn_KURIA = _T("0");		// 事業別課税売上高合計額

	for (int i = 0; i < 12; i++) {
		pZmsub->szsnk->Sn_TYNF[i] = _T("0");	// 中間納付税額	１～１２回目	 ［ＢＩＮ１２桁］
	}

	for (int i = 0; i < 12; i++) {
		pZmsub->szsnk->Sn_TTYNF[i] = _T("0");	// 中間納付税額	１～１２回目	 ［地方消費税用］
	}

	pZmsub->szsnk->Sn_MENZsw = 0;				// 免税⇔課税切換ｻｲﾝ

	pZmsub->szsnk->Sn_MENZK = _T("0");		// ３調整課税仕入等に係る棚卸資産額
	pZmsub->szsnk->Sn_MENZZ = _T("0");		// ３調整課税仕入等に係る棚卸消費税額
	pZmsub->szsnk->Sn_MNK40 = _T("0");		// ４調整課税仕入等に係る棚卸資産額
	pZmsub->szsnk->Sn_MNZ40 = _T("0");		// ４調整課税仕入等に係る棚卸消費税額

	pZmsub->szsnk->Sn_tansw = 0;				// 棚卸調整仕訳入力サイン

	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_mnk40[i] = _T("0");	// ３調整課税仕入等に係る棚卸資産額		課税に係る
	}
	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_mnz40[i] = _T("0");	// ３調整課税仕入等に係る棚卸消費税額 	課税に係る
	}
	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_menzk[i] = _T("0");	// ４調整課税仕入等に係る棚卸資産額		課税に係る
	}
	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_mebzz[i] = _T("0");	// ４調整課税仕入等に係る棚卸消費税額 	課税に係る
	}

	pZmsub->szsnk->Sn_UENZK = _T("0");		// ３輸入仕入に係る棚卸資産額
	pZmsub->szsnk->Sn_UENZZ = _T("0");		// ３輸入仕入に係る棚卸消費税額
	pZmsub->szsnk->Sn_UNK40 = _T("0");		// ４輸入仕入に係る棚卸資産額
	pZmsub->szsnk->Sn_UNZ40 = _T("0");		// ４輸入仕入に係る棚卸消費税額

	pZmsub->szsnk->Sn_usisw = 0;				// 輸入仕入仕訳入力サイン

	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_unk40[i] = _T("0");	// ３輸入仕入に係る棚卸資産額		課税に係る
	}
	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_unz40[i] = _T("0");	// ３輸入仕入に係る棚卸消費税額 	課税に係る
	}
	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_uenzk[i] = _T("0");	// ４輸入仕入に係る棚卸資産額		課税に係る
	}
	for (int i = 0; i < 3; i++) {
		pZmsub->szsnk->Sn_uenzz[i] = _T("0");	// ４輸入仕入に係る棚卸消費税額 	課税に係る
	}

	pZmsub->szsnk->Sn_EXP100 = 0;				// 課税売上高100％

	// 消費税履歴保存対応 (13/05/22)
//	pZmsub->szsnk->Sn_ZeiHou = 0;				// 税理士法３０条及び３３条の２(D0,D1)
//	pZmsub->szsnk->Sn_Syuusei = 0;			// 修正を追加
//	pZmsub->szsnk->Sn_Sign1 = 0;				// 新設サイン①
//	pZmsub->szsnk->Sn_Sign2 = 0;				// 新設サイン②
//	pZmsub->szsnk->Sn_Sign3 = 0;				// 新設サイン③
//	pZmsub->szsnk->Sn_Sign4 = 0;				// 新設サイン④

	//pZmsub->szsnk->Sn_ZKDYS = 0;				// 前年課税期間　自 // TISW修正[25-0204] 2026/02/16
	//pZmsub->szsnk->Sn_ZKDYE = 0;				// 〝　　　　　　至 // TISW修正[25-0204] 2026/02/16
	pZmsub->szsnk->Sn_ZZDAY = 0;				// 修正等の年月日
	pZmsub->szsnk->Sn_KTNFZ = _T("0");		// 消費税及び地方消費税の合計納付税額
	pZmsub->szsnk->Sn_BSMON = 0;				// 月換算　分子
	pZmsub->szsnk->Sn_BBMON = 0;				// ″　　　分母
	for (int i = 0; i < 12; i++) {
		pZmsub->szsnk->Sn_ZSKZN[i] = _T("0");	// 国　税　この申告前の税額
	}
	for (int i = 0; i < 12; i++) {
		pZmsub->szsnk->Sn_ZSKAD[i] = _T("0");	// 〃　　　この申告により増加する税額
	}
	for (int i = 0; i < 12; i++) {
		pZmsub->szsnk->Sn_ZSTZN[i] = _T("0");	// 地方税　この申告前の税額
	}
	for (int i = 0; i < 12; i++) {
		pZmsub->szsnk->Sn_ZSTAD[i] = _T("0");	// 〃　　　この申告により増加する税額
	}
	for (int i = 0; i < 12; i++) {
		pZmsub->szsnk->Sn_ZSKTZ[i] = _T("0");	// 消費税及び地方消費税の合計納付税額
	}

	pZmsub->szsnk->Sn_1F1B = _T("0");			// 課税標準額 税率４％適用分 	・-Ｂ
	pZmsub->szsnk->Sn_1F2A = _T("0");			// 　　〃　　 旧税率適用分   	・-Ａ
	pZmsub->szsnk->Sn_1F4B = _T("0");			// 消費税額   税率４％適用分 	・-Ｂ
	pZmsub->szsnk->Sn_1F5A = _T("0");			// 　　〃　　 旧税率適用分   	・-Ａ
	pZmsub->szsnk->Sn_1F7A = _T("0");			// 貸倒回収に係る消費税額    	・-Ａ
	pZmsub->szsnk->Sn_1F7B = _T("0");			// 　　　　　〃　　　　　    	・-Ｂ
	pZmsub->szsnk->Sn_1F8A = _T("0");			// 控除対象仕入税額 		・-Ａ
	pZmsub->szsnk->Sn_1F8B = _T("0");			// 　　　 〃　　　　 		・-Ｂ
	pZmsub->szsnk->Sn_1F8C = _T("0");			// 　　　 〃　　　　 		・-Ｃ
	pZmsub->szsnk->Sn_1F9A = _T("0");			// 返還等対価に係る税額 	 	・-Ａ
	pZmsub->szsnk->Sn_1F9B = _T("0");			// 　　　　 〃　　　　　 	・-Ｂ
	pZmsub->szsnk->Sn_1F9C = _T("0");			// 　　　　 〃　　　　　 	・-Ｃ
	pZmsub->szsnk->Sn_1F10A = _T("0");		// 貸倒に係る税額 		・-Ａ
	pZmsub->szsnk->Sn_1F10B = _T("0");		// 　　　〃　　　 		・-Ｂ
	pZmsub->szsnk->Sn_1F12B = _T("0");		// 控除不足税額　 		・-Ｂ
	pZmsub->szsnk->Sn_1F13B = _T("0");		// 差引税額		　 	・-Ｂ
	pZmsub->szsnk->Sn_1F14C = _T("0");		// 合計差引税額　 		・-Ｃ

	pZmsub->szsnk->Sn_5FURIK = _T("0");		// 事業区分別課税売上高合計額(期間計)
	pZmsub->szsnk->Sn_5FURIT = _T("0");		// 事業区分別課税売上高合計額(税率4%分)

	for (int i = 0; i < 5; i++) {
		pZmsub->szsnk->Sn_5FURI[i] = _T("0");	// 　　　　〃　　　　　第一種　　〃
	}
	pZmsub->szsnk->Sn_5FUZTT = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	for (int i = 0; i < 5; i++) {
		pZmsub->szsnk->Sn_5FUZT[i] = _T("0");	// 　　　　〃　　　　第一種　　  〃
	}
	pZmsub->szsnk->Sn_5FUZKT = _T("0");		// 　　　　〃　　　　合計額（税率4%分）
	for (int i = 0; i < 5; i++) {
		pZmsub->szsnk->Sn_5FUZK[i] = _T("0");	// 　　　　〃　　　　第一種　　　〃
	}
	pZmsub->szsnk->Sn_5FSZTG = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	pZmsub->szsnk->Sn_5FSZTT = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	pZmsub->szsnk->Sn_5FSZKG = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	pZmsub->szsnk->Sn_5FSZKT = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	pZmsub->szsnk->Sn_5Fdm1 = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	pZmsub->szsnk->Sn_5Fdm2 = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	pZmsub->szsnk->Sn_5Fdm3 = _T("0");		// 事業区分別消費税額合計額(課税期間計)
	pZmsub->szsnk->Sn_5Fdm4 = _T("0");		// 事業区分別消費税額合計額(課税期間計)

	pZmsub->szsnk->Sn_5FSZsw = 0;				// 特例計算適用SW 0:原則計算 1:一種類特例

	pZmsub->szsnk->Sn_5FSBsw = 0;				// 特例計算適用SW (税率4%分)
	pZmsub->szsnk->Sn_5FCQsw = 0;				// 集計・計算済サイン
	pZmsub->szsnk->Sn_5FGKsw = 0;				// 原則計算⑱手入力サインD0:期間  D4:うち
	pZmsub->szsnk->Sn_2F21sw = 0;				// 控除対象仕入税額　手入力サイン D0: D4:

	for (int i = 0; i < 5; i++) {
		pZmsub->szsnk->Sn_5FUP[i] = 0;		// 第一種事業　売り上げ割合(税率4%分)
	}

	pZmsub->szsnk->Sn_2F17A = _T("0");		// 付表2｢17｣-A 個別対応控除税額手入力額
	pZmsub->szsnk->Sn_2F17B = _T("0");		// 付表2｢17｣-B 個別対応控除税額手入力額
	pZmsub->szsnk->Sn_2F18A = _T("0");		// 付表2｢18｣-A 比例配分控除税額手入力額
	pZmsub->szsnk->Sn_2F18B = _T("0");		// 付表2｢18｣-B 比例配分控除税額手入力額
	pZmsub->szsnk->Sn_2F21A = _T("0");		// 付表2｢21｣-A 控除対象仕入税額手入力額
	pZmsub->szsnk->Sn_2F21B = _T("0");		// 付表2｢21｣-B 控除対象仕入税額手入力額

	pZmsub->szsnk->Sn_TYPE = 0x3;


	//---->Sn_ADRD4は複数データを文字列データとして保持。全てを初期化できないので分解して初期化
	long	Zrkbn = 0;
	long	ZroutSgn = 0;
	long	DispTabSgn = 0;
	long	PrintOutSgn = 0;
	long	GenericSgn = 0;
	long	PrintOutSgn2 = 0;

	int		max = 0;
	int		mode = 0;

	//=======文字列データをサイン毎に分解========//
	const	int	EVERY_OPTION_STRLEN = 8;
	CString	dStr;

	if (pZmsub->szsnk->Sn_ADRD4[4].GetLength() > 40) {
		max = 6;
		mode = 1;

	}
	else {
		max = 5;
		mode = 0;
	}
	for (int i = 0; i < max; i++) {
		dStr = pZmsub->szsnk->Sn_ADRD4[4].Mid(i*EVERY_OPTION_STRLEN, EVERY_OPTION_STRLEN);
		if (i == 0) {
			AscToBcd((char*)&Zrkbn, dStr.GetBuffer(), EVERY_OPTION_STRLEN);
		}
		else if (i == 1) {
			AscToBcd((char*)&ZroutSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN);
		}
		else if (i == 2) {
			AscToBcd((char*)&DispTabSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN);
		}
		else if (i == 3) {
			AscToBcd((char*)&PrintOutSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN);
		}
		else if (i == 4) {
			AscToBcd((char*)&GenericSgn, dStr.GetBuffer(), EVERY_OPTION_STRLEN);
		}
		else if (i == 5) {//印刷処理改良時追加
			AscToBcd((char*)&PrintOutSgn2, dStr.GetBuffer(), EVERY_OPTION_STRLEN);
		}
	}

	//===印刷データ部分のみ初期化===
	PrintOutSgn = 0;
	PrintOutSgn2 = 0;
	//===============================

	//=======再度サインを文字列データに結合========//
	pZmsub->szsnk->Sn_ADRD4[4].Empty();
	long	tmpZrkbn = Zrkbn;
	hl_rev(&tmpZrkbn, sizeof(tmpZrkbn));
	long	tmpZroutSgn = ZroutSgn;
	hl_rev(&tmpZroutSgn, sizeof(tmpZroutSgn));
	long	tmpDispTabSgn = DispTabSgn;
	hl_rev(&tmpDispTabSgn, sizeof(tmpDispTabSgn));
	long	tmpPrintOutSgn = PrintOutSgn;
	hl_rev(&tmpPrintOutSgn, sizeof(tmpPrintOutSgn));
	long tmpGenericSgn = GenericSgn;
	hl_rev(&tmpGenericSgn, sizeof(tmpGenericSgn));

	if (mode == 1) {
		long	tmpPrintOutSgn2 = 0;
		PrintOutSgn2 |= 0x01;//提出用カラーをデフォルトに　'22.11.30
		tmpPrintOutSgn2 = PrintOutSgn2;
		hl_rev(&tmpPrintOutSgn2, sizeof(tmpPrintOutSgn2));
		pZmsub->szsnk->Sn_ADRD4[4].Format(_T("%08X%08X%08X%08X%08X%08X"), tmpZrkbn, tmpZroutSgn, tmpDispTabSgn, tmpPrintOutSgn, tmpGenericSgn, tmpPrintOutSgn2);
	}
	else {
		pZmsub->szsnk->Sn_ADRD4[4].Format(_T("%08X%08X%08X%08X%08X"), tmpZrkbn, tmpZroutSgn, tmpDispTabSgn, tmpPrintOutSgn, tmpGenericSgn);
	}
	//<-----------------------------

	//データ更新
	pZmsub->szsnk->Update();
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//


	//モジュール側でテーブル開けた場合閉じておく
	if (openflg == 1) {
		pZmsub->SzsnkClose();
	}

	return rt;
}
