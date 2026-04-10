#include "stdafx.h"

#include "ZeroPercent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
// 消費税申告書集計
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SyzShinCalq( void )
{
	// 確定・中間で非連動しない
	if( m_pSnHeadData->IsReadCalqedData() ){

		if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU) && m_pSnHeadData->IsSoleProprietor() ){
//			if( SkjPersonalConvert())	{
//				ICSMessageBox( _T("仕入控除税額に関する明細書の連動情報が取得できませんでした。"), MB_OK, 0 );
//				return FALSE;
//			}
		}
//		if( ShinCalq()){
//			return 1;
//		}
		int st = 0;
		st = ShinCalq();
		if( st == 2 || st == 0 ){
			// 更正の請求の場合
			if( m_pSnHeadData->Sn_Syuusei&0x02 ){
	//			ShzBillValSet( 0, 1 );
				if( m_pMainFrame ){
//--> '14.03.15 INS START
					// 申告データ作成に必要なデータ読込み
					m_pMainFrame->ReadDataForNextShinkoku();
//<-- '14.03.15 INS END
					m_pMainFrame->CalqSyzForKsk( 1 );
				}
			}

	//		SyzShin_UpDate();
			if( m_pMainFrame ){
				m_pMainFrame->UpdateSyzShin();
//--> '15.04.07 INS START
				if( m_pSnHeadData->IsMiddleProvisional() == FALSE ){
					// 申告データ作成に必要なデータ読込み
					m_pMainFrame->ReadDataForNextShinkoku();
				}
//<-- '15.04.07 INS END
				// 金額データ更新
				m_pMainFrame->RegetCalqedMony();
				
			}
		}
		if( st ){
			return 1;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 申告書集計
//-----------------------------------------------------------------------------
// 引数	middle_calq	：
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了 
//			1		：	キャンセルで、『集計→読込み』を通さない
//			2		：　キャンセルだが、『集計→読込み』を通す
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::ShinCalq( int middle_calq/*=FALSE*/ )
{
	int TenkiSign = 0;

	if( middle_calq && ((m_pSnHeadData->Sn_SKKBN&0xff)%2) && !(m_pSnHeadData->Sn_KZTNSK&0x0f) ){
		vector<MPAYMENTVAL> mpvec = MiddlePaymentRead();
		if( mpvec.size() ){  // 中間申告以外
			if( ICSMessageBox( _T("中間納付額の再転記を行いますか？"), (MB_YESNO|MB_ICONQUESTION) ) == IDYES ){

				TenkiSign = 1;	// 191120

				//-->191120 関数の最後で中間納付税額をセットする流れに変更
////			CArith arith(0x16);
//				char KMPvalTotal[6] = {};
//				char TMPvalTotal[6] = {};
//				for( UINT i=0; i<mpvec.size(); i++ ){
//					m_Arith.l_add(KMPvalTotal,KMPvalTotal,mpvec[i].KMPval);
//					m_Arith.l_add(TMPvalTotal,TMPvalTotal,mpvec[i].TMPval);
//				}
///*- '14.02.21 -*/
////				memmove( Snd.Sn_TYNOFZ, KMPvalTotal, 6 );
////				memmove( Snd.Sn_TTYWAR, TMPvalTotal, 6 );
///*-------------*/
//				CString	tag, chtag;
//				if (m_ShRevType < ID_ICSSH_10PER_REVTYPE) {
//					tag = _T("AAB00110");
//					SetMony(tag, chtag, KMPvalTotal);
//					tag = _T("Sn_TTYWAR");
//					SetMony(tag, chtag, TMPvalTotal);
//				}
//				else {
//					tag = _T("AAJ00110");
//					SetMony(tag, chtag, KMPvalTotal);
//					tag = _T("AAK00070");
//					SetMony(tag, chtag, TMPvalTotal);
//				}
				//<--191120

/*-------------*/
			}
			else {
				return 1;
			}
		}
	}

//	if( IkTaxZanRead() ) {
//		strcpy_s( BUF, sizeof( BUF ), "仮受・仮払消費税の残高が取得できません！" );
//		ICSMessageBox( BUF, MB_OK, 0 );	// IDOK IDCANCEL
//		return( TRUE );
//	}

//-- '15.04.04 --
//	int		StTs, cno;
//	char	BUF[512], FBF[10], ChkSw, ChkDm;
//---------------
	char	ChkSw, ChkDm;
//---------------
	char	Xymd[4] = { 0x09, 0x04, 0x01, 0x00 };


	// 中間期間のオフセットセーブ
//	int	tmpSofs, tmpEofs;
//	tmpSofs = tmpEofs = -1;
//	Ssofs = Seofs = -1;	
//	memset( FBF, 0xff, 2 );
//	if( (m_pSnHeadData->Sn_MCOUNT==3) || (m_pSnHeadData->Sn_MCOUNT==11) ){
//		if( !m_MatuDay ){
//			memmove( FBF, &Snh.Sn_MDAYS[1], 2 );
//		}
//		else{
//			FBF[0] = Snh.Sn_MDAYS[1];
//		}
//		if( FBF[0] ){
//			shin_ofst( (unsigned char *)FBF, &Ssofs , &Vol1, &Vol2);
//		}
//		if( !m_MatuDay ){
//			memmove( FBF, &Snh.Sn_MDAYE[1], 2 );
//		}
//		else{
//			FBF[0] = Snh.Sn_MDAYE[1];
//		}
//		if( FBF[0] ){
//			shin_ofst( (unsigned char *)FBF, &Seofs, &Vol1, &Vol2 );
//		}
//	}

	// 登録項目チェック
	if( CheckEntry( 0 ) == TRUE ){
/*- '14.06.13 -*/
//		return FALSE;
/*-------------*/
		return 0;
/*-------------*/
	}

	// ９５％ルール改正対応期間チェック
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		CheckTaxReforms( 0 );
	}

	// 消費税申告書　処理準備
	// 消費税集計の為のサイン生成
//	Skbn = (int)(m_pSnHeadData->Sn_SKKBN&0xff);
	// 消費税マスターバージョンのチェック
	CString	msg;
	int n_ver = m_pMainFrame->CheckSyzVersion(msg);
	if( n_ver == 0 ){
/*- '14.06.13 -*/
//		return FALSE;
/*-------------*/
		return 0;
/*-------------*/
	}

	// 申告書データ登録チェック
	if( !(m_pSnHeadData->SVsasw&0x01) ){
/*- '14.06.13 -*/
//		return TRUE;	// 申告書用データが作成されていません。
/*-------------*/
		return 1;
/*-------------*/
	}

	// 消費税集計の為のサイン生成
/*	Skbn = (int)(m_pSnHeadData->Sn_SKKBN&0xff);
//	if( Ver = (N_ver&0xff) ) {	//バージョンチェック
		switch( m_pSnHeadData->Sn_SKKBN&0xff ){
			case 1 : case 3 :	//確定・確定修正
				if( l_jsgn( Snh.Sn_KDAYE, Xymd, 3 ) < 0 )	Ver = 0; //H9.4ﾌｸﾑ
				break;
			default:				//中間・中間修正
				if( l_jsgn( Snh.Sn_MDAYE, Xymd, 3 ) < 0 )	Ver = 0; //H9.4ﾌｸﾑ
				break;
		}
//	}*/

	// サインを、ここで再設定する意味は？？	
	m_toku = (int)(m_pSnHeadData->Sn_TOKUR & 0xff);		// 特例計算の適用
	m_K221sw = m_H301sw = 0;
	if( m_pSnHeadData->Sn_TUMIAG&0x01 ){
		m_K221sw = 1;		// 規則２２条１項の適用（積み上げ方式）
	}
	if( m_pSnHeadData->Sn_TUMIAG&0x02 ){
		m_H301sw = 1;		// 法第３０条第１項の適用（積み上げ方式）
	}
	if( m_K221sw || m_H301sw ){
		m_SimpSw = 0;		// 積み上げ方式選択時は処理不可！
	}
	else{
		if( m_pSnHeadData->Sn_SPECIAL&0x01 ){
			m_SimpSw = 1;
		}
		else{
			m_SimpSw = 0;
		}
	}

	m_pSnHeadData->Sn_UPERS &= 0xef;
//	m_pSnHeadData->Sn_2FUWSEX = 0;

	// 入力年月　開始・最終設定
//	ofs_get();
	m_pMainFrame->GetMofs();

	//S移動
//	if(BasPerideTaxSold( 0 ) ){
//		return 2;
//	}

/*	if ( ( ErNo = read_tbl( (short)1 ) ) )	{
		memset( ERMS, '\0', sizeof( ERMS ) );
		switch( ErNo )	{
			case 5 :
				return( TRUE );
				break;
			case 6 :
				strcat_s( ERMS, sizeof( ERMS ), "申告区分が正しくありません。" );
				break;
			default :
				strcat_s( ERMS, sizeof( ERMS ), "申告データが読み込めません" );
				break;
		}
		ICSMessageBox( ERMS, MB_OK, 0 );
		return( TRUE );
	}*/

	// 課税売上割合に準ずる割合を有効サイン
	m_pSnHeadData->Sn_UPERS &= 0xfd;

	if( m_toku ){
		m_Kenei |= 0x10;	// 特例計算摘要ＳＷ
	}

	// 集計テーブル  イニシャライズ
/*	if( forcreat( &foreA, &foreB, &foreC ) )	{
		ICSMessageBox( _T("売上割合集計テーブルが確保できません"), MB_OK, 0 );
		return( TRUE ); // 集計テーブル
	}

	// 消費税科目集計ファイルリード
	if( rui_read( &foreA, &foreB, &foreC ) )		{
		ICSMessageBox( _T("消費税科目集計ファイルが読み込めません"), MB_OK, 0 );
		return( TRUE );	// 消費税科目集計ファイル
	}

	// 課税仕入区分・売上区分集計エリアのリード
	if( suk_read( &SVsum, &foreB ) )	{
		ICSMessageBox( _T("課税仕入区分・売上区分集計領域が読み込めません"), MB_OK, 0 );
		return( TRUE );
	}*/

	// 該当月選択＆集計
	int	save_tno = 0;
	int	ans_tno = 0;

	// 課税期間で申告書集計を行う
/*	if( Kk_SG&0x10 ){
		save_tno = s_tno;
		if( shin_ofst( &Snh.Sn_KDAYS[1], &ans_tno, &Vol1, &Vol2 ) )	{
			Kk_SG &= 0xef;
			s_tno = (char)save_tno;
		}
		else	s_tno = (char)ans_tno;
	}*/

	// 簡易計算
	m_pSnHeadData->Sn_SPECIAL &= 0xfe;
	if( m_SimpSw ){
		m_pSnHeadData->Sn_SPECIAL |= 0x01;
	}
//	// 貨物の税額を入力税額より計算
	ChkSw = ChkDm = 0x00;
//	ChkSw = TBhead.SVmzsw;

	// 科目残高集計
/*	if( formmake( s_tno, d_tno, &kr, &karibri_5, &kariuke_5 ) )	{
		ICSMessageBox( _T("科目残高集計中に障害が発生しました"), MB_OK, 0 );
		return( TRUE );
	}

	// 経過措置　仮払・仮受消費税集計
	formmake3( s_tno, d_tno, &kr3, &karibri_3, &kariuke_3 );

	l_add( kr.barai, kr.barai, kr3.barai );
	l_add( kr.uke, kr.uke, kr3.uke );*/

	// 集計レコードクリア
/*	if( for_clear( &foreA ) )	{
		ICSMessageBox( _T("集計レコードクリア中に障害が発生しました"), MB_OK, 0 );
		return( TRUE );
	}

	if( m_SimpSw ){
		StTs = stm_scalq( s_tno, d_tno, kr.barai, kr.uke, kr3.barai, kr3.uke, &foreA, &foreB );
	}
	else{
		StTs = stm_ucalq( s_tno, d_tno, kr.barai, kr.uke, kr3.barai, kr3.uke, &foreA, &foreB );
	}
	if( StTs ){
		if( StTs == -2 ){
			ICSMessageBox( _T("申告書集計処理を中止します。"), MB_OK, 0 );
			return( TRUE );
		}
		else{
			ICSMessageBox( _T("消費税集計中に障害が発生しました"), MB_OK, 0 );
			return( TRUE );
		}
	}*/

/*	if( (N_ver==2) && m_pSnHeadData->SVd5sw ){
		Ver |= 0x10;
	}*/

	// 申告書集計
	char	uwriai[MONY_BUF_SIZE]={0};

	if( m_pMainFrame->StartSyzSyukei(uwriai) ){
		ICSMessageBox( _T("消費税集計中に障害が発生しました"), MB_OK, 0 );
		return -1;
	}

	

	if(BasPerideTaxSold( 0 ) ){
		return 2;
	}

	// 仮受・仮払消費税チェック
	if( m_pMainFrame->CheckKariukeKaribarai() ){
//		ICSMessageBox( _T("仮払及び仮受消費税等の残高が０円になっている為、申告処理ができません"), MB_OK, 0 );
		ICSMessageBox( _T("仮払及び仮受消費税等の残高が０円になっている為、申告処理ができません。\n消費税精算仕訳等の仕訳区分を確認してください。"), MB_OK, 0 );
/*- '14.06.13 -*/
//		return 2;
/*-------------*/
		return 1;
/*-------------*/
	}

//--> '14.06.30 INS START
	// 金額取得
	if( m_pMainFrame ){
		m_pMainFrame->GetCalqedMony();
	}
//<-- '14.06.30 INS END

	// 棚卸計算
//	StackMake( s_tno, d_tno, &kr, stack_data );
	if( m_pMainFrame ){
		m_pMainFrame->CalqStatckSyz();
	}

	// 課税売上割合 0%チェック
	m_pSnHeadData->Sn_EXP100 = 0x00;
	char	PW0[MONY_BUF_SIZE] = {0};
	m_Arith.l_input( PW0, _T("1") );
	if( (m_Arith.l_cmp(uwriai, PW0)>=0) || m_pSnHeadData->IsKobetuSiireAnbun() ){
		;
	}
	else{
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			if ( !( curset.m_DispTabSgn & 0x2000 ) ) {	//2割特例計算使用時もメッセージを表示しない様に
				CZeroPercent	ZeroPercent;

				int dSt = (int)ZeroPercent.DoModal();
				if (dSt == IDCANCEL) {
					//				return -2;
					return 2;
				}
				if (dSt == IDOK) {
					//				memset( stval.uwriai, '\0', 6 );
				}
				else {
					//				m_Arith.l_input( stval.uwriai, "10000" );
					m_pSnHeadData->Sn_EXP100 = 0x01;
				}
			}
		}
	}

	m_pSnHeadData->SVsacq &= 0xfe;	// 申告書集計強制サインＯＮ
	m_pSnHeadData->SVsacq |= 0x01;

	m_pSnHeadData->SVmzsw &= 0x0f;
	//--->yoshida190820
	if (m_pSnHeadData->SVmzsw & 0x01) {
		curset.SVmzsw |= (m_pSnHeadData->SVmzsw & 0x01);
	}
	else {
		curset.SVmzsw &= ~0x01;
	}
	//----
	//元々の処理
	//curset.SVmzsw |= (m_pSnHeadData->SVmzsw & 0x01);
	//<-----

	//連動で仕訳チェック後立った経過措置サインが履歴に反映かかってない。yoshida190820
	pastset.SVmzsw = curset.SVmzsw;

	m_Check5.EnableWindow( TRUE );
	if( curset.SVmzsw == 1 ){
		m_Check5.SetCheck( 1 );
	}
	else{
		m_Check5.SetCheck( 0 );
	}
	m_Check5.EnableWindow( FALSE );

	// 特定収入　処理
	if( m_pMainFrame && (*(m_pMainFrame->GetsgSpc())==1) ){
		if( m_pMainFrame ){
			if( m_pMainFrame->ConnectedSpcList() ){
				ICSMessageBox( _T("特定収入計算表等の財務連動中に障害が発生しました"), MB_OK, 0 );
				return -1;
			}
		}

/*		// 特定収入に係る課税仕入れ等の税額計算
		if( m_pSnHeadData->SVmzsw == 1 ){
//			SPassOnCalq();		// 経過措置有り
		}
		else{
//			SPassOffCalq();		// 経過措置なし
		}
		m_SpcCq = 1;
		m_SpcTk = 1;
		m_pSnHeadData->Sn_Sign2 &= 0xfd;
		memset( WORK0, '\0', 6 );
		l_input( WORK0, "50" );
		Snd.Sn_2F21sw = 0x00;
		Snd.Sn_Sign2 &= 0xfd;
		if( l_cmp( stval.SpTswr, WORK0 ) > 0 )	{
			// 初期設定
			memset( Snd.Sn_2F21A, '\0', 6 );
			memset( Snd.Sn_2F21B, '\0', 6 );
			if( TBhead.SVmzsw == 1 )	{
			// 経過措置対象の課税資産の譲渡等有り
				Snd.Sn_2F21sw |= 0x01;
				memmove( Snd.Sn_2F21A, stval.SpSiz3, 6 );
				Snd.Sn_2F21sw |= 0x10;
				memmove( Snd.Sn_2F21B, stval.SpSiz4, 6 );
			}
			else	{
			// 経過措置対象の課税資産の譲渡等無し
				Snd.Sn_2F21sw |= 0x10;
				memmove( Snd.Sn_2F21B, stval.SpSizg, 6 );
			}
		}
		else	Snd.Sn_Sign2 |= 0x02;*/
	}

/*	// 消費税ファイルも更新
	so_close();						// 旧ﾊﾞｰｼﾞｮﾝ用
	sn_close();						// 新ﾊﾞｰｼﾞｮﾝ用
*/

	// 仕入控除税額に関する明細書の財務連動
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
//		if( SkjConnecteed( &foreC, s_tno, d_tno ) ){
//			ICSMessageBox( _T("仕入控除税額に関する明細書の財務連動中に障害が発生しました"), MB_OK, 0 );
//			return -1;
//		}
		if( m_pMainFrame ){
			if( m_pMainFrame->ConnectedSkj(false) ){
				ICSMessageBox( _T("仕入控除税額に関する明細書の財務連動中に障害が発生しました"), MB_OK, 0 );
				return -1;
			}
		}
	}
//2016.02.23 INSERT START
	else{
		if( m_pMainFrame ){
			if( m_pMainFrame->ClearShiire() ){
				ICSMessageBox( _T("仕入金額等の明細の初期化中に障害が発生しました"), MB_OK, 0 );
				return -1;
			}
		}
	}
//2016.02.23 INSERT END

	if( m_pMainFrame && (*(m_pMainFrame->GetsgSpc())==0) ){
		// 課税取引金額計算表の財務連動
		if( m_pSnHeadData->IsSoleProprietor() ){
//--> '14.08.08 INS START
			if( m_pMainFrame ){
				if( ChkCallProgramType() == 1 ){
					//過年度の場合には課税取引の集計は通さない
					if( m_pMainFrame->ConnectedTaxList() ){
						ICSMessageBox( _T("課税取引金額計算表等の財務連動中に障害が発生しました"), MB_OK, 0 );
						return -1;
					}
				}

			}
//<-- '14.08.08 INS END

//			if( KjsnkCalculat( s_tno, d_tno,  &foreC ) ){
//				ICSMessageBox( _T("課税取引金額計算表等の財務連動中に障害が発生しました"), MB_OK, 0 );
//				return( TRUE );
//			}
//			// 再集計
//			Scalc.Shz_AllCalc(0);
		}
	}

	// 課税期間で申告書集計を行う	[03'04.15]
//	if( Kk_SG&0x10 ){
//		s_tno = save_tno;
//	}
//
//	m_TaxCalcusw = 0;		// ９０％ルール変更時、警告サイン [12'05.10]


	// -->191120 関数の最後で中間納付税額をセットする流れに変更
	if( TenkiSign == 1 ){
		vector<MPAYMENTVAL> mpvec = MiddlePaymentRead();
		char KMPvalTotal[6] = {};
		char TMPvalTotal[6] = {};
		for( UINT i=0; i<mpvec.size(); i++ ){
			m_Arith.l_add(KMPvalTotal,KMPvalTotal,mpvec[i].KMPval);
			m_Arith.l_add(TMPvalTotal,TMPvalTotal,mpvec[i].TMPval);
		}
		CString	tag, chtag;
		if (m_ShRevType < ID_ICSSH_10PER_REVTYPE) {
			tag = _T("AAB00110");
			SetMony(tag, chtag, KMPvalTotal);
			tag = _T("Sn_TTYWAR");
			SetMony(tag, chtag, TMPvalTotal);
		}
		else {
			tag = _T("AAJ00110");
			SetMony(tag, chtag, KMPvalTotal);
			tag = _T("AAK00070");
			SetMony(tag, chtag, TMPvalTotal);
		}
	}
	// <--191120


	return 0;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 入力項目チェック
//int CDBSyzShinMainView::EntryCheck( int sign )
int CDBSyzShinMainView::CheckEntry( int sign )
{
	// 本表のデータ取得
	char	zzuri[MONY_BUF_SIZE]={0};
	CString	tag, chtag;
	tag = _T("AAF00000");
	m_pMainFrame->GetMony( tag, chtag, zzuri );

/*- '14.01.30 -*/
//	if( m_Arith.l_cmp(WZURI, zzuri) ){	// 基準期間の課税売上高
//		memmove( m_pSnHeadData->SVzuri, zzuri, 6 );
//	}
/*-------------*/
	memmove( m_pSnHeadData->SVzuri, zzuri, 6 );
/*-------------*/
	m_pSnHeadData->SVkojn = 0x00;
	if( (m_pSnHeadData->Sn_KOJIN&0x01) ){
		m_pSnHeadData->SVkojn |= 0x01;	// 個人以外の事業者 ON:12ヶ月計算
	}
	if( m_pSnHeadData->Sn_KOANB ){
		m_pSnHeadData->SVkojn |= 0x10;	// 個人事業者　ON:月数按分, OFF:12ヶ月計算
	}
//	if( (m_pSnHeadData->SVzchg = (Snd.Sn_MENZsw&0x1f)) == 0x10 ){
//		memmove( m_pSnHeadData->SVtval1, Snd.Sn_MENZK, 6 );
//		memmove( m_pSnHeadData->SVtval2, Snd.Sn_MENZZ, 6 );
//	}
//	else {
//		memset( m_pSnHeadData->SVtval1, '\0', 6 );
//		memset( m_pSnHeadData->SVtval2, '\0', 6 );
//	}

	return 0;
}

//-----------------------------------------------------------------------------
// ９５％ルール改正対応期間チェック
//-----------------------------------------------------------------------------
//int CDBSyzShinView::TaxReformsCheck( int type )
int CDBSyzShinMainView::CheckTaxReforms( int type )
{
/*	char	CheckDay[4] = { 0x24, 0x04, 0x01, 0x00 };
	int		Taxrerms = 0;

	// 個人・法人の取得
	m_Kojin = (Snh.Sn_KOJIN&0x01);
	if( (Vol1.APNO&0xf0) == 0x10 )	m_Kojin = 1;

	Taxrerms = m_TaxReforms;
	m_TaxReforms = 0;		// OFF:９５％ルール改正未対応期間
	if( l_jsgn( Snh.Sn_KDAYS, CheckDay, 3 ) >= 0 )	{
		m_TaxReforms = 1;	// ON:９５％ルール改正対応期間
		// 個人事業者＆決算期間の期末年度が24年度＆期間短縮無はOFF:９５％ルール改正未対応期間
		if( m_Kojin && ((Vol1.EE_YMD[0]&0xff) == 0x12) && ((Snh.Sn_KZTNSK&0x0f) == 0x00) && (Kk_SG&0x10) == 0x00 )	m_TaxReforms = 0;
	}
	// ９５％ルール変更期間チェック
	if( Taxrerms != m_TaxReforms ) m_TaxCalcusw = 1;
	if( type && m_TaxCalcusw ) {
		CString		Msg;
		if( m_TaxReforms )	Msg = _T("課税期間が平成24年4月1日以降開始になっています。\n仕入控除制度の見直しが適用されますので"); 
		else				Msg = _T("課税期間が平成24年3月31日以前開始になっています。\n仕入控除制度の見直しが適用されませんので");
		Msg += _T("\n必ず申告書集計を行ってください。");
		if( ICSMessageBox( Msg, MB_YESNO | MB_ICONSTOP, 0 ) == IDYES )	return( 1 );
	}*/
	
	return 0;
}	


//-----------------------------------------------------------------------------
// 基準期間の課税売上高チェック
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::BasPerideTaxSold( int sign )
{
	CString	Workmess;
	int		skip;

	Workmess.Empty();
	skip = 0;

	char	Sn_ZZURI[MONY_BUF_SIZE] = {0};
	CString	tag, chtag;
	
	// 10%改正対応
	//tag = _T("ABB00180");
	if ( GetActViewVer() == ID_VER_SYZ31 ) {
		if( (m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01 ){	// 簡易課税
			tag = _T("AAM00030");
		}
		else{	// 原則課税
			tag = _T("ABI00180");
		}
	}
	else {
		tag = _T("ABB00180");
	}

	m_pMainFrame->GetMony( tag, chtag, Sn_ZZURI );
//	m_Arith.l_print( Sn_ZZURI,  tmpMony, _T("SSSSSSSSSSS9") );


	if( m_Arith.l_test( Sn_ZZURI ) ) {
		// 免税事業者チェック
		if( m_Arith.l_cmp( m_pSnHeadData->SVkan0 , Sn_ZZURI ) >= 0 ) {	// [05'06.06]
			if ( !(m_pSnHeadData->m_DispTabSgn & 0x2000) || ( m_pZmSub->zvol->to_kz_ymd == 0 )) {
				//Workmess = "基準期間の課税売上高が免税業者の適用条件に該当します。\n";
				//Workmess += "基準期間の課税売上高を確認してください。\n";
				//Workmess += "『消費税課税事業者選択届出書』を提出している場合以外は\n";
				//Workmess += "申告書の作成は不要です。処理を継続しますか？";
				Workmess = "基準期間の課税売上高が免税事業者の適用条件に該当します。\n";	// インボイスを加味した文言に変更
				Workmess += "基準期間の課税売上高を確認してください。\n";
				Workmess += "適格請求書発行事業者の場合、また『消費税課税事業者選択届出書』を\n";
				Workmess += "提出している場合等は申告書の作成が必要です。処理を継続しますか？";
				skip = 1;
			}
		}
		else	{

			if( m_pSnHeadData->m_DispTabSgn & 0x2000 ){
				// 基準期間の課税売上高が1000万越えの場合は2割特例を適用できないため、警告メッセージを出す
				if( m_Arith.l_cmp( m_pSnHeadData->SVkan0, Sn_ZZURI ) < 0 ){
					Workmess  = "基準期間の課税売上高が1,000万円を超えているため、2割特例を適用できません。\n";
					Workmess += "基準期間の課税売上高を確認してください。\n";
					//if( !sign )	Workmess += "処理を継続しますか？\n";
					skip = 4;
				}
			}
			else{
				if( (m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01 )	{
					// 簡易課税
					if( m_Arith.l_cmp( m_pSnHeadData->SVkan1, Sn_ZZURI ) < 0 ) {
						Workmess  = "基準期間の課税売上高が簡易課税の範囲を超えています。\n";

						long symd = 0, eymd = 0;
						GetKazeiKikan( symd, eymd );
						if( eymd < ICS_SH_INVOICE_DAY ) {
							// 令和5年10月1日以降の課税期間の場合は、税額計算の特例のメッセージを表示しない
							Workmess += "また、税額計算の特例の適用条件に該当しません。\n";
						}

						Workmess += "基準期間の課税売上高を確認して、必要であれば\n";
						if( m_pSnHeadData->Sn_Sign4&0x80 )
								Workmess += "課税方式を、通常課税に変更して\n";
						else	Workmess += "新規会社登録・修正・削除業務の「消費税情報」で、原則課税に変更して\n";//yoshida190722
	//					else	Workmess += "新規会社登録・修正・削除業務の「消費税情報」で、通常課税に変更して\n";
	//					if( sign )	Workmess += "申告書を作成してください。";
	//					else		Workmess += "申告書を作成してください。処理を継続しますか？";
						Workmess += "申告書を作成してください。";
						skip = 2;
					}
				}
				else	{
					//特例5000万チェック。
					if (m_pSnHeadData->m_DispTabSgn & 0x80) {//特例チェックONの時
						if (m_Arith.l_cmp(m_pSnHeadData->SVkan1, Sn_ZZURI) < 0) {//5000万チェック
							CString msg;
							msg = "基準期間の課税売上高が税額計算の特例の適用条件に該当しません。\n";
							msg += "基準期間の課税売上高を確認してください。\n";
							msg += "処理を継続しますか？";
							if (ICSMessageBox(msg, MB_YESNO, 0) == IDYES) {
								//継続してもサインはそのまま
							}
							else {
								return(1);
							}
						}
					}
					// 原則課税
					if( m_Arith.l_cmp( m_pSnHeadData->SVkan1, Sn_ZZURI ) >= 0 ) {
						Workmess  = "基準期間の課税売上高が簡易課税の適用条件に該当します。\n";
						Workmess += "基準期間の課税売上高を確認してください。\n";
						Workmess += "『消費税簡易課税制度選択届出書』を提出している場合は\n";
						if( m_pSnHeadData->Sn_Sign4&0x80 )
								Workmess += "課税方式を、簡易課税に変更して\n";
						else	Workmess += "新規会社登録・修正・削除業務の「消費税情報」で、簡易課税に変更して\n";
						Workmess += "申告書を作成してください。処理を継続しますか？";
						skip = 3;
					}
				}
			}
		}
		if( skip ) {
			//if( sign && skip == 2 )	{
			//if( sign && (skip == 2 || skip == 4) )	{
			if( skip == 2 || skip == 4 )	{
				// 処理不可
				ICSMessageBox( Workmess, MB_OK, 0 );
				return( 1 );	
			}
			else	{
				if( ICSMessageBox( Workmess, MB_YESNO, 0 ) == IDYES )
						return( 0 );
				else	return( 1 );
			}
		}
	}

	return( 0 );
}

