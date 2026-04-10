#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"
#include "DBBwMast.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString Alter_Index_Query = "\
IF EXISTS(SELECT name FROM sys.columns WHERE object_id = object_id('szsnk') AND name = 'Sn_SEQ') AND \n\
   NOT EXISTS (SELECT name FROM sys.indexes WHERE name = N'PK_szsnk') BEGIN \n\
	ALTER TABLE szsnk ADD CONSTRAINT PK_szsnk PRIMARY KEY CLUSTERED \n\
	( \n\
		[Sn_SEQ] \n\
	) \n\
	WITH \n\
	( \n\
		PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, \n\
ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON \n\
	)ON [PRIMARY] \n\
END \n";

// 24/05/08_168730 add -->
extern	CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub;
extern	CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_KAZ;
extern	CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_ZEI;
// 24/05/08_168730 add <--

//-----------------------------------------------------------------------------
// 消費税申告書の読込み
//-----------------------------------------------------------------------------
// 引数	number		：	何番目のデータ？
//		errmsg		：	エラーメッセージ保存先
//		isFirlst	：	初回？（初期作成時にしか、インデックスの確認をしないようにする為）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::OpenSzsnk( int number, CString &errmsg, bool isFirst/*=false*/ )
{
	if( isFirst && (m_bAlterIndex==false) ){
		m_pZmSub->m_database->BeginTrans();
		try{
			m_pZmSub->m_database->ExecuteSQL( Alter_Index_Query );
		}
		catch( CDBException *pDbe ){
			m_pZmSub->m_database->Rollback();
			errmsg = pDbe->m_strError;
			pDbe->Delete();
			return -1;
		}
		m_pZmSub->m_database->CommitTrans();

		m_bAlterIndex = true;
	}
	else if( isFirst == false ){
		m_bAlterIndex = true;
	}

	if( number == m_SnSeq ){
		return 0;
	}

	// 消費税申告書
	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), number );
	if( m_pZmSub->szsnk == NULL ){
		if( m_pZmSub->SzsnkOpen(filter) == ERR ){
			errmsg = m_pZmSub->GetZmErrMsg();
			return ERR;
		}
	}
	else{
		m_pZmSub->szsnk->Requery( filter );
	}

/*	// データ変換
	char	telbf[64];
	int		i;
	// 申告書管理エリアの変換
	memset( &Snh, '\0', sizeof( struct Sn_HEAD ) );
	memset( &Snd, '\0', sizeof( struct Sn_DATA) );
*/

	// 新規チェック
	if( m_pZmSub->szsnk->st == -1 ){
		return 0;
	}

	if( m_SnSeq != -1 ){
		m_SnHeadData.SetData( m_pZmSub, m_SnSeq );
	}
	m_SnHeadData.GetData( m_pZmSub, number );

	// シーケンス保存
	m_SnSeq = number;

	// この下に旧ファイル形式に変換して読み込む箇所あり！

	// 現時点ではクローズせずに進める。。。
//	m_pZmSub->SzsnkClose();

	return 0;
}

//-----------------------------------------------------------------------------
// マスターオープン
//-----------------------------------------------------------------------------
// 引数	number	：	何番目のデータ？
//		errmsg	：	エラーメッセージ保存先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::OpenSyzShin( int number, CString &errmsg )
{
	// 消費税申告書
	if( OpenSzsnk( number, errmsg ) == ERR )	{
		return ERR;
	}
/*	// 仕入控除税額に関する明細書
	if( SzskjOpen( number, errmsg ) == ERR )	{
		return( ERR );
	}
	// 仕入れ控除税額に関する明細書の明細入力
	if( SkjrecOpen( number, errmsg ) == ERR )	{
		// 更正の請求テーブル削除
		SkjrecDelFunc( -1, errmsg );
		return( ERR );
	}
	// 消費税の還付申告に関する明細書
	if( OpenRefund( number, errmsg ) == ERR )	{
		// 更正の請求テーブル削除
		RefundDelFunc( -1, errmsg );
		return( ERR );
	}
	// 更正の請求
	if( ReaskOpen( number, errmsg ) == ERR ) {
		// 更正の請求テーブル削除
		ReaskDelFunc ( -1, errmsg );
		return( ERR );
	}
	if( m_Basic ) {
		// 課税取引金額計算表
		if( KjsnkOpen( number, errmsg ) == ERR ) {
			KjsnkDelFunc( -1, errmsg );
			return( ERR );
		}
	}
	if( m_SpcSg ) {
		// 特定収入計算表　実額入力
		if( SpcListOpen( number, errmsg ) == ERR ) {
			return( ERR );
		}
	}*/
	return 0;
}

#ifdef _20140207_EXTEND_
//-----------------------------------------------------------------------------
// 消費税申告書の書込み
//-----------------------------------------------------------------------------
// 引数	number	：	何番目のデータ？
//		errmsg	：	エラーメッセージ保存先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::CloseSzsnk( int number, CString &errmsg )
{
	char	telbf[64]={0}, tel1[10]={0}, tel2[10]={0}, tel3[10]={0};
	CString	tel;
//	int		i;

	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
		m_SnHeadData.Sn_Sign4 &= 0x7f;
		m_SnHeadData.Sn_Sign4 |= 0x80;
	}

	// 消費税申告書
	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), number );
	if( m_pZmSub->SzsnkOpen( filter ) == ERR )				{
		errmsg = m_pZmSub->GetZmErrMsg();
		return -1;
	}

	// この下は未実装・・・
	return 0;

	if( m_pZmSub->szsnk->st == -1  ){
		m_pZmSub->szsnk->AddNew();
	}
	else{
		m_pZmSub->szsnk->Edit();
	}

/*	// データ変換
	大量にコードがあるが未実装
*/

	if( m_pZmSub->szsnk->Update() == -1 ){
		errmsg.Format( _T("消費税申告書データの書き込み中に障害が発生しました。保存番号(%d)"), number );
		return -1;
	}

	m_pZmSub->SzsnkClose();

	return 0;
}
#else
//-----------------------------------------------------------------------------
// 消費税申告書の書込み
//-----------------------------------------------------------------------------
// 引数	number	：	何番目のデータ？
//		errmsg	：	エラーメッセージ保存先
//		doClose	：	現データを破棄するかどうか
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::CloseSzsnk( int number, CString &errmsg, BOOL doClose )
{
	// 消費税ヘッダ情報
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
		m_SnHeadData.Sn_Sign4 &= 0x7f;
		m_SnHeadData.Sn_Sign4 |= 0x80;
	}
	m_SnHeadData.SetData( m_pZmSub, number );

	// 10%改正対応
	if (GetKaiseiVer() == ID_VER_SYZ26) {	//19.09.06
//	if ( GetActViewVer() == ID_VER_SYZ26 ) {
		CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 2 ));
		if( pH26View != NULL ){
			pH26View->WriteData( doClose, FALSE );
		}
	}
	else if (GetKaiseiVer() == ID_VER_SYZ31) {	//19.09.06
//	else if ( GetActViewVer() == ID_VER_SYZ31 ) {
		CH31HyoView *pH31View = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 1 ) );
		if ( pH31View != NULL ) {
			pH31View->WriteData ( doClose, FALSE );
		}
	}

	// 課税取引計算書
	if(m_pMainView != NULL){
		if( m_pMainView->ChkCallProgramType(1) == 1 ){//判定のみ（引数：１）
			CBaseTaxHyoView *pBaseTaxView = (CBaseTaxHyoView *)((CDBSyzShinMainApp*)AfxGetApp())->GetTaxListView();
			if( pBaseTaxView != NULL ){
				//pBaseTaxView->WriteData( doClose , FALSE );
				pBaseTaxView->WriteData( doClose );
			}
		}
	}

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// 消費税申告書データの新規作成
//-----------------------------------------------------------------------------
// 引数	errmsg	：	エラーメッセージ保存先
//-----------------------------------------------------------------------------
// 返送値	0	；	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::AddNewSzsnkData( CString &errmsg )
{
	if( OpenSzsnk(0, errmsg, true) ){
		return -1;
	}

	// メモリ上にレコードを貯める。
	m_SnHeadData.Sn_VER = 0x00;
	m_SnHeadData.Sn_SKKBN = 1;
	m_SnHeadData.Sn_SEIRI[0]  = 0xff;			
	m_SnHeadData.Sn_ZSEIRI[0] = 0xff;
	m_SnHeadData.Sn_ITIREN[0] = 0xff;
	m_SnHeadData.Sn_KOZA[0]	 = 0xff;

	if( !m_SnHeadData.Sn_KDAYS ){
		m_SnHeadData.Sn_KDAYS = m_pZmSub->zvol->ss_ymd;
	}
	if( !m_SnHeadData.Sn_KDAYE ){
		m_SnHeadData.Sn_KDAYE = m_pZmSub->zvol->ee_ymd;
	}

	if( (m_pZmSub->zvol->apno&0xff) != 0x10 ){
		int lng = m_Util.MixSpaceCutLength( m_pZmSub->zvol->c_nam.GetBuffer(), 40 );
		memmove( m_SnHeadData.Sn_CONAM, m_pZmSub->zvol->c_nam, lng );
	}
	m_SnHeadData.m_s_sgn4 = m_pZmSub->zvol->s_sgn4;
	m_SnHeadData.m_s_sgn2 = m_pZmSub->zvol->s_sgn2;

//	Kj_SkData.Kj_SpCrcNen = Snh.Sn_KDAYE[0];

	// 欄外印字　拡張対応（英数字より漢字及び英数字混在対応の為の変換）
	m_SnHeadData.Sn_PPVER = 0x01;

	// ９５％ルール改正対応期間チェック -> 無条件にこの期間のはず！
//	if( (Vol1.S_SGN2 & 0x0f) < 2 ){
//		TaxReformsInitCheck();
//	}

	m_SnHeadData.Sn_ZeiHou &= 0x7f;
	m_SnHeadData.Sn_ZeiHou |= 0x80;

	// 非連動DB
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
		m_SnHeadData.Sn_Sign4 &= 0x7f;
		m_SnHeadData.Sn_Sign4 |= 0x80;
	}

	// 新規マスターの場合には、不具合修正済みとしておく
	if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) {
		m_SnHeadData.Sn_GenericSgn |= 0x04;
	}

	// 2021/9リリースプロ起動サイン
	m_SnHeadData.Sn_GenericSgn |= 0x80;

// 改良依頼 20-1013,19-1184 修正 -->
	// 印刷の用紙設定を「提出用（カラー  白紙）」にする
	// イメージ印刷
	m_SnHeadData.Sn_TYPE |= 0x01;
	// カラー
	m_SnHeadData.Sn_TYPE |= 0x02;
// 改良依頼 20-1013,19-1184 修正 <--

// 修正依頼 160293 -->
	// 新規作成時のデフォルトを「提出用カラー」「控用モノクロ」にする
	m_SnHeadData.Sn_PrintOutSgn2 |= 0x01;
// 修正依頼 160293 <--

	// 一度も消費税申告書を起動していないマスターで一括印刷を行うと
	// 出力設定に旧様式の選択が表示される不具合に対応するため
	// (部数指定対応時に修正)
	if(m_SnHeadData.Sn_KDAYE >= R03_SDAY)	{
		m_SnHeadData.m_DispTabSgn |= 0x1000;
	}

	m_SnHeadData.SetData( m_pZmSub, 0/*初期作成*/ );
	m_SnSeq = 0;

	// 改正バージョン ('19.05.21)
	SetActViewVer( &m_SnHeadData );

//--> '15.07.11 INS START
	m_SnHeadData.TakeInMynumber( m_pZmSub, &m_TgPDat, errmsg, NTBMOD_GYFLG_SYOHI );
//<-- '15.07.11 INS END

	return 0;
}

//-----------------------------------------------------------------------------
// 履歴バージョンチェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::CheckHistoryVersion()
{
	if( m_pZmSub->szvol->SVvers < 0x11 ){	// 履歴対応へ、バージョンアップ

		if( 0/*フォーク時*/ ){
			CString	msg;
			msg += _T("このバージョンの消費税申告書マスター（V10）は扱えません。\n");
			msg += _T("消費税申告書業務にてバージョンアップを行ってから処理してください。");
			ICSMessageBox( msg );
			PostMessage( WM_CLOSE, 0x2000 );
//			m_swForkErr = 1;
			return 1;
		}

/*		int mes_flg = ( (m_pZmSub->zvol->apno&0xf0) != 0x10 ) && !m_pZmSub->zvol->ind_type && Snh.Sn_KOJIN;
		CVerCheckDlg dlg(mes_flg, m_UnConnect);
		if( IDOK != dlg.DoModal() ){
			ICSMessageBox( _T("処理を中止します。") );
			AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0x2000 );
			return 1;
		}*/
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//★//[12'06.08]///
// 顧問先情報取り込み後、全ての保存情報に書き込む必要があるのか？
/*int CMainFrame::AdviseInfoRenew( int type, CString &errmsg )
{
	return 0;
}*/

//-----------------------------------------------------------------------------
// 起動時の９５％ルール改正対応期間チェック
//-----------------------------------------------------------------------------
void CMainFrame::Chk_H24Reform_InInit()
{
	m_isAfterH24Reform = FALSE;
	if( m_pZmSub->szsnk->Sn_KDAYS >= ICS_SH_H24_REFORM_DAY ){
		m_isAfterH24Reform = TRUE;

		// 個人事業者＆決算期間の期末年度が24年度＆期間短縮無はOFF:９５％ルール改正未対応期間
		if( m_pZmSub->szsnk->Sn_KOJIN || ((m_pZmSub->zvol->apno&0xff)==0x10) ){
			if( ((m_pZmSub->zvol->ee_ymd/10000)==2012) && ((m_pZmSub->szsnk->Sn_KZTNSK&0x0f)==0x00) /*&& ((Kk_SG&0x10)==0x00)*/ ){
				m_isAfterH24Reform = FALSE;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 消費税申告書情報の書き込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::UpdateSyzShin()
{
	if( m_pMainView ){
		m_pMainView->SetSnHeadToCSnset( m_SnHeadData );
	}

	// 当月月管理情報の更新（中間申告情報）
	int		i=0, month=0;
	m_pZmSub->minfo->MoveFirst();
	do{
		switch( i ) {
			case 3 :	// 中間決算
			case 7 :
			case 11:
				break;
			case 15:	// 通常決算修正月
			default:
				// レコード編集
				m_pZmSub->minfo->Edit();
				m_pZmSub->minfo->sz_syk &= 0xfe;
//				m_pZmSub->minfo->sz_syk |= (m_SnHeadData.SVtmth[month]&0x01);
				// レコード更新
				if( m_pZmSub->minfo->Update() == -1 ){
					return -1;
				}
				month++;
				break;
		}
		i++;
		if( i == 16 ){
			break;
		}
	} while( m_pZmSub->minfo->MoveNext() != -1 );

	// 仕入控除明細書新規登録チェック
	if( (m_SnHeadData.SVsasw&0x10) == 0 ){
		m_SnHeadData.SVsasw |= 0x10;
	}

	// 非連動用：申告書集計強制
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
		m_SnHeadData.SVsacq &= 0xfe;
		m_SnHeadData.SVsacq |= 0x01;
	}

	// ヘッダ情報
	m_SnHeadData.SetData( m_pZmSub, m_SnSeq );

	return 0;
}

//-----------------------------------------------------------------------------
// 仕入控除税額に関する明細書(個人用)　財務連動 
//				個人決算書対応	　　		    
//				08'09.24						
//-----------------------------------------------------------------------------
int CMainFrame::SkjPersonalConvert()
{
	char				Codbf[10];
	int					Cnt, Seq, Tai, Flg, Sjc;
	struct	BW_TABLE	BwTbRec, *BwTable;
	SH_SKJ_CNV			SkjCnv;

	// 科目名称
	Cnt = 0;
	while( 1 ) {
		if( !Cnt ){
			if( m_pZmSub->knrec->MoveFirst() == ERR ){
				return FALSE;
			}
		}
		else{
			if( m_pZmSub->knrec->MoveNext() == ERR ){
				break;
			}
		}
		Cnt++;
		if( m_pZmSub->knrec->knvoid == 1 ){
			continue;
		}
		// 大文字変換
		memset( Codbf, '\0', sizeof( Codbf ) );
		memmove( Codbf, m_pZmSub->knrec->kncod, 8 );
		_strupr_s( Codbf, sizeof( Codbf ) );
		memset( &SkjCnv, '\0', sizeof( SH_SKJ_CNV ) );
		memmove( SkjCnv.SKJCod, Codbf, 8 );
		if( m_Tblhdl.th_acs( thRead, &SkjCnv, &m_SkjCnvtbl ) ){
			continue;
		}
		// 貸借＆損益
		Tai = 0;
		if( strncmp( Codbf, "08", 2 ) < 0 ){
			Tai = 1;		// 貸借科目
		}
		else{
			Tai = 0;		// 損益科目
		}
		// 個人業種
		Flg = 0;
		switch( m_pZmSub->knrec->ind_ctg&0x0f ) {
			case 0x01 :	// 一般
				if( m_pZmSub->zvol->ind_type&0x10 ){
					BwTable = WITbl;
				}
				else{
					BwTable = BITbl;
				}
				// 決算書項目
				if( Tai	){
					Seq = m_pZmSub->knrec->ind_prf1;	
				}
				else{
					if( m_pZmSub->zvol->ind_type&0x10 ){
						Seq = m_pZmSub->knrec->ind_prf2;
					}
					else{
						Seq = m_pZmSub->knrec->ind_prf1;
					}
				}
				break;
			case 0x02 :	// 不動産
				if( m_pZmSub->zvol->ind_type&0x10 ){
					BwTable = WFTbl;
				}
				else{
					BwTable = BFTbl;
				}
				// 決算書項目
				if( Tai	){
					Seq = m_pZmSub->knrec->ind_prf2;	
				}
				else{
					if( m_pZmSub->zvol->ind_type&0x10 ){
						Seq = m_pZmSub->knrec->ind_prf2;
					}
					else{
						Seq = m_pZmSub->knrec->ind_prf1;
					}
				}
				break;
			case 0x04 :	// 農業
				if( m_pZmSub->zvol->ind_type&0x10 ){
					BwTable = WNTbl;
				}
				else{
					BwTable = BNTbl;
				}
				// 決算書項目
				if( Tai	){
					Seq = m_pZmSub->knrec->ind_prf3;	
				}
				else{
					if( m_pZmSub->zvol->ind_type&0x10 ){
						Seq = m_pZmSub->knrec->ind_prf2;
					}
					else{
						Seq = m_pZmSub->knrec->ind_prf1;
					}
				}
				break;
			default   :	// 業種？
				Flg = 1;
				break;
		}
		
		// 変換テーブル読み込み
		memmove( &BwTbRec, (BwTable) + Seq, sizeof( struct BW_TABLE ) );

		// 貸借科目
		if( Tai ) {
			// 固定資産の取得
			if( m_pZmSub->knrec->knsgn[0] == 0x11 ) {

				switch( m_pZmSub->knrec->ind_ctg&0x0f ) {
					case 0x01 :	
						SkjCnv.KJNCod = 3;	
						break;	// 一　般 ③
					case 0x02 :
						SkjCnv.KJNCod = 6;	
						break;	// 不動産 ⑥
					case 0x04 :	
						SkjCnv.KJNCod = 3;	
						break;	// 農　業は一般の③
					default   : 
						break;
				}
			}
			else{
				SkjCnv.KJNCod = 0;
			}
		}
		else{
			// 損益科目
/*- '14.04.29 -*/
//			Sjc = BwTable[Seq-1].BTskjno;
//			if( Sjc > 0 ){
//				SkjCnv.KJNCod = Sjc;
//			}
//			else{
//				SkjCnv.KJNCod = 0;
//			}
/*-------------*/
			if( Seq > 0 ){
				Sjc = BwTable[Seq-1].BTskjno;
				if( Sjc > 0 ){
					SkjCnv.KJNCod = Sjc;
				}
				else{
					SkjCnv.KJNCod = 0;
				}
			}
//--> '15.03.10 INS START
			else{
				SkjCnv.KJNCod = 0;
			}
//<-- '15.03.10 INS END
/*-------------*/
		}

		// 棚卸科目は含まない。
		if( !strncmp( Codbf, "090101", 6 )	||	// 期首商品製品棚卸
			!strncmp( Codbf, "090401", 6 )	||	// 期末商品税品棚卸
			!strncmp( Codbf, "0F0101", 6 )	||	// 期首材料
			!strncmp( Codbf, "0F0105", 6 ) ) {	// 期末材料
			SkjCnv.KJNCod = 0;			// ×
		}
		if( m_pZmSub->zvol->apno == 0x01 ){	// 運送業
			if( !strncmp( Codbf, "0F0A01", 6 )	||	// 期首仕掛品
				!strncmp( Codbf, "0F0A02", 6 ) ){	// 期末仕掛品
				SkjCnv.KJNCod = 0;		// ×
			}
		}
		else	{					// 運送業以外
			if( !strncmp( Codbf, "0F0501", 6 )	||	// 期首仕掛品
				!strncmp( Codbf, "0F0502", 6 ) ){	// 期末仕掛品
				SkjCnv.KJNCod = 0;		// ×
			}
		}
		// 集計外科目の追加
		if(  !strncmp( Codbf, "020110", 6 ) ||											// 減価償却累計額
			(!strncmp( Codbf, "020115", 6 ) && (m_pZmSub->zvol->apno != 0x10) ) ||		// 減損損失累計額
			(!strncmp( Codbf, "020306", 6 ) && (m_pZmSub->zvol->apno == 0x00) ) ||		// 繰延税金資産
			(!strncmp( Codbf, "020306", 6 ) && (m_pZmSub->zvol->apno == 0x01) ) ||		// 繰延税金資産
			(!strncmp( Codbf, "020306", 6 ) && (m_pZmSub->zvol->apno == 0x20) ) ||		// 繰延税金資産
			(!strncmp( Codbf, "020307", 6 ) && (m_pZmSub->zvol->apno != 0x30) ) ||		// 繰延消費税等
			(!strncmp( Codbf, "02030C", 6 ) && (m_pZmSub->zvol->apno == 0x30) ) ||		// 繰延税金資産
			(!strncmp( Codbf, "02030D", 6 ) && (m_pZmSub->zvol->apno == 0x30) ) ||		// 繰延消費税等
			(!strncmp( Codbf, "020319", 6 ) && (m_pZmSub->zvol->apno != 0x10) ) )	{	// 貸倒引当金
			SkjCnv.KJNCod = 0;		// ×
		}
		if(	(!strncmp( Codbf, "020319", 6 ) && (m_pZmSub->zvol->apno == 0x10) ) ||		// 
			(!strncmp( Codbf, "0301", 4 )	&& (m_pZmSub->zvol->apno == 0x10) ) )	{	// 
			SkjCnv.KJNCod = 0;		// ×
		}
		// 期首・期末棚卸
		if( m_pZmSub->knrec->knsgn[0] == 0x04 && ( m_pZmSub->knrec->knsgn[1] == 0x05 || m_pZmSub->knrec->knsgn[1] == 0x06 ) ) {
			SkjCnv.HJNCod = 0;		// ×
			SkjCnv.KJNCod = 0;		// ×
		}

#ifdef	_DEBUG_EX
	TRACE("\n[%-8.8s] (%02d)", Codbf, SkjCnv.KJNCod );
#endif
		if( m_Tblhdl.th_acs( thWritex, &SkjCnv, &m_SkjCnvtbl ) )	return( ERR );
	}
	return( FALSE );
}

//-----------------------------------------------------------------------------
// 仕入控除税額に関する明細書の変換テーブルの再取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetPastSkjData()
{
	if( m_SkjCnvtbl.tp_tno == -1 ){
		return 0;
	}
	if( m_pZmSub == NULL ){
		return -1;
	}

	if( m_pZmSub->owntb == NULL ){
		if( m_pZmSub->OwnTblOpen() == -1 ){
			return -1;
		}
	}

	SH_SKJ_CNV	skjcnv;
	char		codbf[10];
	CString		filter;
	if( (m_pZmSub->zvol->apno==0x52) && (m_pZmSub->zvol->m_ver==0x11) ){
		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ52_ITMNAME, m_usertbl.user_id );
	}
	else{
		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ_ITMNAME, m_usertbl.user_id );
	}
	if( m_pZmSub->owntb->Requery(filter) != -1 )	{	
		int cnt = 0;
		while( 1 ){
			if( !cnt ){
				if( m_pZmSub->owntb->MoveFirst() == ERR ){
					return	FALSE;
				}
			}
			else{
				if( m_pZmSub->owntb->MoveNext() == ERR ){
					break;
				}
			}

			cnt++;
			// 大文字変換
			memset( codbf, '\0', sizeof( codbf ) );
			m_Util.cstring_to_char( (unsigned char *)codbf, m_pZmSub->owntb->code[0], 8 );
			_strupr_s( codbf, sizeof( codbf ) );
			memset( &skjcnv, '\0', sizeof( SH_SKJ_CNV ) );
			memmove( skjcnv.SKJCod, codbf, 8 );
			if( m_Tblhdl.th_acs( thRead, &skjcnv, &m_SkjCnvtbl ) ){
				continue;
			}
//-- '14.10.31 --
//			skjcnv.HJNCod = m_pZmSub->owntb->vl[0];
//			skjcnv.KJNCod = m_pZmSub->owntb->vl[1];
//---------------
			// 不正値が入る可能性があるのでカット
			if( (0<=m_pZmSub->owntb->vl[0]) && (m_pZmSub->owntb->vl[0]<10) ){
				skjcnv.HJNCod = m_pZmSub->owntb->vl[0];
			}
			if( (0<=m_pZmSub->owntb->vl[1]) && (m_pZmSub->owntb->vl[1]<10) ){
				skjcnv.KJNCod = m_pZmSub->owntb->vl[1];
			}
//---------------
			if( m_Tblhdl.th_acs( thWritex, &skjcnv, &m_SkjCnvtbl ) ){
				continue;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 仕入控除税額に関する明細書の変換テーブルの取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetSkjCnvtbl()
{
	if( m_SkjCnvtbl.tp_tno != -1 ){
		m_Tblhdl.th_close( m_SkjCnvtbl.tp_tno );
		memset( &m_SkjCnvtbl, '\0', sizeof(_TB_PAR) );
		m_SkjCnvtbl.tp_tno = -1;
	}

	unsigned int rsiz = (unsigned int)sizeof( SH_SKJ_CNV );								// レコード長
	unsigned int rcnt = (unsigned int)(m_pZmSub->knrec->GetRecordCount() * 2);		// レコード数
	unsigned long tsiz = (unsigned long)rsiz * (unsigned long)rcnt;

	int thd = m_Tblhdl.th_open( tsiz, rsiz );
	if( thd < 0 ){
		return -1;
	}
	m_SkjCnvtbl.tp_tno  = thd;
	m_SkjCnvtbl.tp_rmax = rcnt;
	m_SkjCnvtbl.tp_rnum = 0;
	m_SkjCnvtbl.tp_rlen = rsiz;
	m_SkjCnvtbl.tp_koff = 0;
	m_SkjCnvtbl.tp_klen = 8;	// 科目コード
	m_SkjCnvtbl.tp_dupm = 0;
	m_SkjCnvtbl.tp_sufm = 0;
	m_SkjCnvtbl.tp_cp   = 0;
	m_SkjCnvtbl.tp_err  = 0;

	SH_SKJ_CNV	skjcnv;
	int			cnt=0, flg=0, seq=1;
	char		codbf[10];
	for( int i=0;; i++ ){
		if( i==0 ){
			if( m_pZmSub->knrec->MoveFirst() == ERR ){
				return -1;
			}
		}
		else{
			if( m_pZmSub->knrec->MoveNext() == ERR ){
				break;
			}
		}
		cnt++;
		if( m_pZmSub->knrec->knvoid == 1 ){
			continue;
		}
		// 大文字変換
		memset( codbf, '\0', sizeof(codbf) );
		memmove( codbf, m_pZmSub->knrec->kncod, 8 );
		_strupr_s( codbf, sizeof(codbf) );
		memset( &skjcnv, '\0', sizeof(SH_SKJ_CNV) );
		memmove( skjcnv.SKJCod, codbf, 8 );
		flg = 0;

//2016.02.23 INSERT START
		if(( m_pZmSub->knrec->knsgn[0] == 0x01 )&&( m_pZmSub->knrec->knsgn[1] == 0x00 )){ //消費税区分「資産取得」
			skjcnv.HJNCod = 0;
			skjcnv.KJNCod = 0;
			flg++;
		}
//2016.02.23 INSERT END
		if( (m_pZmSub->zvol->apno&0xf0) > 0x30 ){
			// 非営利法人
			skjcnv.HJNCod = 0;
			skjcnv.KJNCod = 0;
			switch( m_pZmSub->zvol->apno ){
				case 0x51 :	// 公益Ⅲ
					if( !strncmp(codbf, _T("0105"), 4 )	){
						if( m_SnHeadData.IsUseTaxRefund() ){
							skjcnv.HJNCod = 8;		// ⑧
						}
						else{
							skjcnv.HJNCod = 6;		// ⑥
						}
						flg++;
					}
					if( !strncmp(codbf, _T("02"), 2 ) ){
						if( !strncmp( codbf, _T("0201"), 4 )	||
							!strncmp( codbf, _T("0202"), 4 )	||
							!strncmp( codbf, _T("02031"), 5 )	||
							!strncmp( codbf, _T("02050A"), 6 )	||
							!strncmp( codbf, _T("02050B"), 6 )	||
							!strncmp( codbf, _T("02051"), 5 ) ){
							skjcnv.HJNCod = 0;
						}
						else{
							skjcnv.HJNCod = 6;
						}
						flg++;
					}
					if( !strncmp( codbf, _T("09"), 2 ) || 
						!strncmp( codbf, _T("0A"), 2 ) || 
						!strncmp( codbf, _T("8"), 1 ) )	{ 
						if( m_pZmSub->knrec->knsgn[0] == 0x04 ){
							skjcnv.HJNCod = 1;
						}
						if( m_pZmSub->knrec->knsgn[0] == 0x08 ){
							skjcnv.HJNCod = 2;
						}
						flg++;
					}
					if( !strncmp( codbf, _T("0B"), 2 ) ){
						skjcnv.HJNCod = 0;
						flg++;
					}
					if( !strncmp( codbf, _T("0C02"), 4 ) ||
						!strncmp( codbf, _T("0C04"), 4 ) ||	
						!strncmp( codbf, _T("0C06"), 4 ) ){
						skjcnv.HJNCod = 3;
						flg++;
					}
					if( !strncmp(codbf, _T("0E"), 2)	){
						skjcnv.HJNCod = 3;
						flg++;
					}
					if( !strncmp( codbf, _T("12"), 2 ) ||
						!strncmp( codbf, _T("13"), 2 ) ){
						skjcnv.HJNCod = 4;
						flg++;
					}
					if( !strncmp( codbf, _T("1502"), 4 ) ){
						skjcnv.HJNCod = 0;
						flg++;
					}
					break;
					
				case 0x52 :	// 社会福祉
					if( m_pZmSub->zvol->m_ver == 0x11 ){
						if( !strncmp( codbf, _T("0104"), 4 ) ){		// 有価証券
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("02"), 2 )	)	{			//基財・土地	～	長期繰延税資産
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("19"), 2 )	)	{			// 役員報酬		～	基金繰入額（事
							skjcnv.HJNCod = 2;		// ②販売費・一般管理費
							flg++;
						}
						if( !strncmp( codbf, _T("1907"), 4 )	)	{			// 事)国補取崩額
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("1B"), 2 )	)	{			// 支払利息		～	雑支出
							skjcnv.HJNCod = 3;		// ②販売費・一般管理費
							flg++;
						}
						if( !strncmp( codbf, _T("1D"), 2 )	)	{			// １号基本組入額　～　他特別損失
							skjcnv.HJNCod = 3;		// ②販売費・一般管理費
							flg++;
						}
						if( !strncmp( codbf, _T("1D04"), 4 )	)	{			// 特)国補取崩額
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("21"), 2 ) )	{			// 期首製品棚卸高　事)	～	期末材料棚卸高
							skjcnv.HJNCod = 1;		// ①商品仕入等
							flg++;
						}
						if( !strncmp( codbf, _T("210101"), 6 ) ||
							!strncmp( codbf, _T("210201"), 6 ) ||
							!strncmp( codbf, _T("210203"), 6 ) ||
							!strncmp( codbf, _T("210510"), 6 ) ||
							!strncmp( codbf, _T("210601"), 6 ) ||
							!strncmp( codbf, _T("210701"), 6 ) ||
							!strncmp( codbf, _T("210901"), 6 ) )	{			// 利用者賃金	～	雑費
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("22"), 2 ) )	{			// 特）利用者賃金	～	雑費
							skjcnv.HJNCod = 2;		// ②販売費・一般管理費
							flg++;
						}
						if( !strncmp( codbf, _T("20211"), 4 ) )	{			// 特）利用者賃金	～	雑費
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("23"), 2 ) )	{			// 材料仕入高
							skjcnv.HJNCod = 1;		// ①商品仕入等
							flg++;
						}
						if( !strncmp( codbf, _T("230411"), 6 ) ||
							!strncmp( codbf, _T("2305"), 4 ) )	{		// 利用者工賃	～	雑費
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("2B01"), 4 ) ||
							!strncmp( codbf, _T("2B02"), 4 ) ||
							!strncmp( codbf, _T("2B03"), 4 ) )	{		// 管理費返還	～	設備償還金
							skjcnv.HJNCod = 4;		// ④その他
							flg++;
						}

						if( !strncmp( codbf, _T("2B04"), 4 ) ||
							!strncmp( codbf, _T("2B05"), 4 ) ||
							!strncmp( codbf, _T("2B06"), 4 ) ||
							!strncmp( codbf, _T("2B07"), 4 ) ||
							!strncmp( codbf, _T("2B08"), 4 ) ||
							!strncmp( codbf, _T("2B0A"), 4 ) ||
							!strncmp( codbf, _T("2B0C"), 4 ) ||
							!strncmp( codbf, _T("2B0D"), 4 ) ||
							!strncmp( codbf, _T("2B0E"), 4 ) ||
							!strncmp( codbf, _T("2B0F"), 4 ) ||
							!strncmp( codbf, _T("2B10"), 4 ) ||
							!strncmp( codbf, _T("2B11"), 4 ) ||
							!strncmp( codbf, _T("2B12"), 4 ) ||
							!strncmp( codbf, _T("2B13"), 4 ) ||
							!strncmp( codbf, _T("2B14"), 4 ) )	{
							skjcnv.HJNCod = 6;		// ⑥固定資産
							flg++;
						}
						// 有価証券
						if( !strncmp( codbf, _T("2B09"), 4 ) ||
							!strncmp( codbf, _T("2B0B"), 4 ) )	{
							skjcnv.HJNCod = 8;		// ⑧その他
							flg++;
						}

						if( !strncmp( codbf, _T("2C"), 2 )	)	{			// 有価証券
							skjcnv.HJNCod = 0;		// ①商品仕入等
							flg++;
						}
					}
					else	{
						if( !strncmp( codbf, _T("0104"), 4 ) )	{
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("02"), 2 ) )	{
							skjcnv.HJNCod = 0;
							flg++;
						}
						if( !strncmp( codbf, _T("09"), 2 ) )	{
							skjcnv.HJNCod = 2;
							flg++;
						}
						if( !strncmp( codbf, _T("0B"), 2 ) )	{
							skjcnv.HJNCod = 3;
							flg++;
						}
						if( !strncmp( codbf, _T("0D"), 2 ) )	{
							skjcnv.HJNCod = 4;
							flg++;
						}
						if( !strncmp( codbf, _T("0F"), 2 )	)	{
							if( !strncmp( codbf, _T("0F01"), 4 ) ) skjcnv.HJNCod = 4;
							else	if( !strncmp( codbf, _T("0F1"), 3 ) ) skjcnv.HJNCod = 0;
							else	skjcnv.HJNCod = 6;
							flg++;
						}
					}
					break;
				case 0x58 :
					// 固定資産の範囲
					if( !strncmp( codbf, _T("0201"), 4 ) || !strncmp( codbf, _T("0202"), 4 ) ) {
						if( !strncmp( codbf, _T("02022"), 5 ) )	
								skjcnv.HJNCod = 0;
						else	skjcnv.HJNCod = 6;
						flg++;
					}
					if( !strncmp( codbf, _T("0305"), 4 )	)	{
						if( m_SnHeadData.IsUseTaxRefund() ){
							skjcnv.HJNCod = 8;		// ⑧
						}
						else{
							skjcnv.HJNCod = 6;		// ⑥
						}
						flg++;
					}
					if( !strncmp( codbf, _T("09"), 2 ) || 
						!strncmp( codbf, _T("0A"), 2 ) )	{ 
						if( m_pZmSub->knrec->knsgn[0] == 0x08 )	skjcnv.HJNCod = 2;
						flg++;
					}
					break;
				case 0x59 :
					if( !strncmp( codbf, _T("02"), 2 )	)	{
						skjcnv.HJNCod = 0;
						flg++;
					}
					if( !strncmp( codbf, _T("03"), 2 )	)	{
						skjcnv.HJNCod = 0;
						flg++;
					}
					if( !strncmp( codbf, _T("0A"), 2 )	)	{
						skjcnv.HJNCod = 2;
						flg++;
					}
					if( !strncmp( codbf, _T("0A0B01"), 6 )	)	{
						if( m_SnHeadData.IsUseTaxRefund() ){
							skjcnv.HJNCod = 8;		// ⑧
						}
					}
					if( !strncmp( codbf, _T("0B01"), 4 )	||
						!strncmp( codbf, _T("0B02"), 4 )	||
						!strncmp( codbf, _T("0B03"), 4 )	||
						!strncmp( codbf, _T("0B04"), 4 )	||
						!strncmp( codbf, _T("0B05"), 4 )	||
						!strncmp( codbf, _T("0B06"), 4 )	||
						!strncmp( codbf, _T("0B09"), 4 )	)	{
						skjcnv.HJNCod = 6;
						flg++;
					}
					break;
				default   :	// 公益
					if( !strncmp( codbf, _T("0105"), 4 )	)	{
						if( m_SnHeadData.IsUseTaxRefund() ){
							skjcnv.HJNCod = 8;		// ⑧
						}
						else{
							skjcnv.HJNCod = 6;		// ⑥
						}
						flg++;
					}
					if( !strncmp( codbf, _T("02"), 2 )	)	{
						if( !strncmp( codbf, _T("0201"), 4 )	||
							!strncmp( codbf, _T("0202"), 4 )	||
							!strncmp( codbf, _T("02031"), 5 )	||
							!strncmp( codbf, _T("02050A"), 6 )	||
							!strncmp( codbf, _T("02050B"), 6 )	||
							!strncmp( codbf, _T("02051"), 5 ) )
								skjcnv.HJNCod = 0;
						else	skjcnv.HJNCod = 6;
						flg++;
					}
					if( !strncmp( codbf, _T("09"), 2 ) || 
						!strncmp( codbf, _T("0A"), 2 ) || 
						!strncmp( codbf, _T("8"), 1 ) || 
						!strncmp( codbf, _T("0B"), 2 ) )	{
						if( m_pZmSub->knrec->knsgn[0] == 0x04 )	skjcnv.HJNCod = 1;
						if( m_pZmSub->knrec->knsgn[0] == 0x08 )	skjcnv.HJNCod = 2;
						flg++;
					}
					if( !strncmp( codbf, _T("0D"), 2 )	)	{
						skjcnv.HJNCod = 3;
						flg++;
					}
					if( !strncmp( codbf, _T("11"), 2 ) ||
						!strncmp( codbf, _T("12"), 2 )	)	{
						skjcnv.HJNCod = 4;
						flg++;
					}
					break;
			}
		}
		else	{
			// 有価証券
			if( !strncmp( codbf, _T("01"), 2 ) && m_pZmSub->knrec->knsgn[0] == 0x11 )	{
				if( m_SnHeadData.IsUseTaxRefund() ){
					skjcnv.HJNCod = 8;		// ⑥	
				}
				else{
					skjcnv.HJNCod = 6;		// ⑥
				}
				skjcnv.KJNCod = 3;		// ③
				flg++;
			}
			// 固定資産
			if( !strncmp( codbf, _T("02"), 2 )	)	{
				skjcnv.HJNCod = 6;		// ⑥
				skjcnv.KJNCod = 3;		// ③
				flg++;
			}
			// 繰延資産
			if( !strncmp( codbf, _T("03"), 2 )	)	{
				skjcnv.HJNCod = 7;		// ⑦
				skjcnv.KJNCod = 3;		// ③
				flg++;
			}
			// 仕入
			if( !strncmp( codbf, _T("09"), 2 )	)	{
				skjcnv.HJNCod = 1;		// ①
				skjcnv.KJNCod = 1;		// ①
				flg++;
			}
			// 経費
			if( !strncmp( codbf, _T("0A"), 2 )	)	{
				skjcnv.HJNCod = 2;		// ②
				skjcnv.KJNCod = 2;		// ②
				flg++;
			}
			// 営業外費用
			if( !strncmp( codbf, _T("0B02"), 4 ))	{
				skjcnv.HJNCod = 3;		// ③
				skjcnv.KJNCod = 2;		// ②
				flg++;
			}
			// 特別損失
			if( !strncmp( codbf, _T("0C02"), 4 ))	{
				skjcnv.HJNCod = 4;		// ④
				skjcnv.KJNCod = 2;		// ②
				flg++;
			}
			// 製造原価
			if( !strncmp( codbf, _T("0F"), 2 )	)	{
				skjcnv.HJNCod = 1;		// ①
				skjcnv.KJNCod = 1;		// ①
				flg++;
			}
			// 棚卸科目は含まない。
			if( !strncmp( codbf, _T("090101"), 6 )	||	// 期首商品製品棚卸
				!strncmp( codbf, _T("090401"), 6 )	||	// 期末商品税品棚卸
				!strncmp( codbf, _T("0F0101"), 6 )	||	// 期首材料
				!strncmp( codbf, _T("0F0105"), 6 ) ) {	// 期末材料
				skjcnv.HJNCod = 0;			// ×
				skjcnv.KJNCod = 0;			// ×
			}
			if( m_pZmSub->zvol->apno == 0x01 )	{	// 運送業
				if( !strncmp( codbf, _T("0F0A01"), 6 )	||	// 期首仕掛品
					!strncmp( codbf, _T("0F0A02"), 6 ) ) {	// 期末仕掛品
					skjcnv.HJNCod = 0;		// ×
					skjcnv.KJNCod = 0;		// ×
				}
			}
			else	{					// 運送業以外
				if( !strncmp( codbf, _T("0F0501"), 6 )	||	// 期首仕掛品
					!strncmp( codbf, _T("0F0502"), 6 ) ) {	// 期末仕掛品
					skjcnv.HJNCod = 0;		// ×
					skjcnv.KJNCod = 0;		// ×
				}
			}
			// 集計外科目の追加
			if(  !strncmp( codbf, _T("020110"), 6 ) ||											// 減価償却累計額
				(!strncmp( codbf, _T("020115"), 6 ) && (m_pZmSub->zvol->apno != 0x10) ) ||		// 減損損失累計額
				(!strncmp( codbf, _T("020306"), 6 ) && (m_pZmSub->zvol->apno == 0x00) ) ||		// 繰延税金資産
				(!strncmp( codbf, _T("020306"), 6 ) && (m_pZmSub->zvol->apno == 0x01) ) ||		// 繰延税金資産
				(!strncmp( codbf, _T("020306"), 6 ) && (m_pZmSub->zvol->apno == 0x20) ) ||		// 繰延税金資産
				(!strncmp( codbf, _T("020307"), 6 ) && (m_pZmSub->zvol->apno != 0x30) ) ||		// 繰延消費税等
				(!strncmp( codbf, _T("02030C"), 6 ) && (m_pZmSub->zvol->apno == 0x30) ) ||		// 繰延税金資産
				(!strncmp( codbf, _T("02030D"), 6 ) && (m_pZmSub->zvol->apno == 0x30) ) ||		// 繰延消費税等
				(!strncmp( codbf, _T("020319"), 6 ) && (m_pZmSub->zvol->apno != 0x10) ) )	{	// 貸倒引当金
				skjcnv.HJNCod = 0;		// ×
				skjcnv.KJNCod = 0;		// ×
			}
			if(	(!strncmp( codbf, _T("020319"), 6 ) && (m_pZmSub->zvol->apno == 0x10) ) ||		// 
				(!strncmp( codbf, _T("0301"), 4 )	&& (m_pZmSub->zvol->apno == 0x10) ) )	{	// 
				skjcnv.KJNCod = 0;		// ×
			}
			// 期首・期末棚卸
			if( m_pZmSub->knrec->knsgn[0] == 0x04 && ( m_pZmSub->knrec->knsgn[1] == 0x05 || m_pZmSub->knrec->knsgn[1] == 0x06 ) ) {
				skjcnv.HJNCod = 0;		// ×
				skjcnv.KJNCod = 0;		// ×
			}
		}
		if( flg ) {
			skjcnv.SKJSeq = seq;
			if( m_Tblhdl.th_acs(thWritex, &skjcnv, &m_SkjCnvtbl) ){
				return -1;
			}
			seq++;
		}

	}

/*- '14.03.24 -*/
	// owntb から取得
//	if( m_pZmSub->owntb == NULL ){
//		if( m_pZmSub->OwnTblOpen() == -1 ){
//			return -1;
//		}
//	}
//	CString	filter;
//	if( (m_pZmSub->zvol->apno==0x52) && (m_pZmSub->zvol->m_ver==0x11) ){
//		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ52_ITMNAME, m_usertbl.user_id );
//	}
//	else{
//		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ_ITMNAME, m_usertbl.user_id );
//	}
//	if( m_pZmSub->owntb->Requery(filter) != -1 )	{	
//		cnt = 0;
//		while( 1 ){
//			if( !cnt ){
//				if( m_pZmSub->owntb->MoveFirst() == ERR ){
//					return	FALSE;
//				}
//			}
//			else{
//				if( m_pZmSub->owntb->MoveNext() == ERR ){
//					break;
//				}
//			}
//
//			cnt++;
//			// 大文字変換
//			memset( codbf, '\0', sizeof( codbf ) );
//			m_Util.cstring_to_char( (unsigned char *)codbf, m_pZmSub->owntb->code[0], 8 );
//			_strupr_s( codbf, sizeof( codbf ) );
//			memset( &skjcnv, '\0', sizeof( SH_SKJ_CNV ) );
//			memmove( skjcnv.SKJCod, codbf, 8 );
//			if( m_Tblhdl.th_acs( thRead, &skjcnv, &m_SkjCnvtbl ) ){
//				continue;
//			}
//			skjcnv.HJNCod = m_pZmSub->owntb->vl[0];
//			skjcnv.KJNCod = m_pZmSub->owntb->vl[1];
//			if( m_Tblhdl.th_acs( thWritex, &skjcnv, &m_SkjCnvtbl ) ){
//				continue;
//			}
//		}
//	}
/*-------------*/
	if( GetPastSkjData() ){
		return -1;
	}
/*-------------*/

	return 0;
}

//-----------------------------------------------------------------------------
// マスター情報の保存
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::SaveNowSyzInfo()
{
	// 税務署コードの取得の後始末
	if( m_isOpenEselect ){
		Esel_EtaxMasterClose();
		m_isOpenEselect = FALSE;
	}

	// 消費税申告書情報の書き込み
	if( m_pMainView ){
		// ユーザー情報保存
		m_pMainView->SetInitInfo( m_usertbl );
		m_pMainView->SetInitInfo2();
// 24/05/08_168730 add -->
		// 特定収入計算表 個別出力設定
		if( m_pMapPrnEXSub.GetCount() > 0 || 
			m_pMapPrnEXSub_KAZ.GetCount() > 0 || 
			m_pMapPrnEXSub_ZEI.GetCount() > 0 ) {
			m_pMainView->SetPrnExSubInfo();
		}
// 24/05/08_168730 add <--
		// 印刷微調整情報の保存
		m_pMainView->SetPrAdjustInfo();
	}
	// 消費税申告書情報の書き込み
	UpdateSyzShin();

	return 0;
}

//-----------------------------------------------------------------------------
// 異マスターの再構築
//-----------------------------------------------------------------------------
//int CMainFrame::MasterReConstruct()
int CMainFrame::ReConstructMaster()
{
	// 前回タブ情報
//	int	m_BasicEx = m_Basic;
//	int	m_SpcSgEx = m_SpcSg;

	// マスター情報の保存
	SaveNowSyzInfo();

	// 他表のクローズ
	EndOtherView();

	// 消費税関連クローズ
	SyzEndJob();
	// 財務関連クローズ
	CloseZmtbl();

	//　マインバー破棄
	if( m_pMainView ){
		m_pMainView->FinalizeMyno ( false );
	}

	InitVariable();

	// 財務クラスの再構築
	char ProType[128];
	PCOM_GetString( _T("ProgramType"), ProType );

	// アプリケーションの初期化
	BeginWaitCursor();
	if( InitApl(FALSE) ){
		EndWaitCursor();
//		ICSExit( 0, _T("初期化失敗！！") );
		return -1;
	}
	EndWaitCursor();

	HINSTANCE hInst = AfxGetResourceHandle();

	// 初期化
	CDBSyzShinMainView	*pView = (CDBSyzShinMainView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 0 ));
	if( pView != NULL ){
		m_pMainView = pView;

		memset( &m_ShinInfo, '\0', sizeof(SYC_SHININFO) );
		m_ShinInfo.pBaseView = m_pMainView;
		m_ShinInfo.pParent = this;
		m_ShinInfo.pEselect = &m_Eselect;
		m_ShinInfo.pIsOpenEselect = &m_isOpenEselect;
//-- '15.04.04 --
//		m_ShinInfo.sgSpc = m_sgSpc;
//---------------
		m_ShinInfo.pSgSpc = &m_sgSpc;
//---------------
		m_ShinInfo.isExistOffice = m_isExistOffice;
		m_ShinInfo.pO_DATA = &m_O_DATA;
		m_ShinInfo.pZ_DATA = &m_Z_DATA;
		m_ShinInfo.pOver500MillionSw = &m_Over500MillionSw;
		m_ShinInfo.pToolBar = GetToolBar( 0 );
		m_ShinInfo.pProgress = &m_Progress;
		m_ShinInfo.pFinish = &m_Finish;
		m_ShinInfo.pFinishSw = &m_FinishSw;
		m_ShinInfo.pFinishPrn = m_FinishPrn;
		m_ShinInfo.isDotPrn = m_sgDotPrn;
		if( pView->Init(m_pZmSub, this, &m_SnHeadData, &m_ShinInfo, &m_Tblhdl, &m_SkjCnvtbl) ){
			ErrorExit( _T("基本情報画面の作成に失敗しました") );
			return 0;
		}
	}
	else{
		m_pMainView = NULL;
	}

//--> '15.02.19 INS START
	if( m_pMainView ){
		if( m_symd >= ICS_SH_SIXKIND_REFORM_DAY ){
			m_pMainView->m_SixKindFlg = TRUE;
		}
		else{
			m_pMainView->m_SixKindFlg = FALSE;
		}
	}
//<-- '15.02.19 INS END

	//---->特定課税仕入対応150713
	if( m_pMainView ){
		m_pMainView->SetTkkzSign( m_eymd );
	}
	//<-----------------------------

	// 事務所情報を読込み
//-- '16.08.08 --
//	if( ReadOfficeInfo(TRUE)  == 1){
//		m_ShinInfo.isExistOffice = TRUE;
//	}
//	else{
//		m_ShinInfo.isExistOffice = FALSE;
//	}
//---------------
	if( IsCompanyPackageInSyz() ){
		m_ShinInfo.isExistOffice = FALSE;
	}
	else{
		if( ReadOfficeInfo(TRUE)  == 1){
			m_ShinInfo.isExistOffice = TRUE;
		}
		else{
			m_ShinInfo.isExistOffice = FALSE;
		}
	}
//---------------

	
#ifdef SHOWTABTEST
	if (m_SnHeadData.Kaisei_VER == ID_VER_SYZ31) {
		AfxSetResourceHandle(GetH31HyoInstance());
		if (InitOtherView(3, m_pZmSub)) {
			return 0;
		}
	}
	else {
		AfxSetResourceHandle(GetH26HyoInstance());
		if (InitOtherView(0, m_pZmSub)) {
			return 0;
		}
	}
	m_nowTabmax++;
#else
	// 他のビューの初期化
	if (GetActViewVer() == ID_VER_SYZ31) {
		AfxSetResourceHandle(GetH31HyoInstance());
		if (InitOtherView(3, m_pZmSub)) {
			return 0;
		}
//		m_nowTabver = ID_VER_SYZ31;
		SetKaiseiVer(ID_VER_SYZ31);
	}
	else {
		AfxSetResourceHandle(GetH26HyoInstance());
		if (InitOtherView(0, m_pZmSub)) {
			return 0;
		}
//		m_nowTabver = ID_VER_SYZ26;
		SetKaiseiVer(ID_VER_SYZ26);
	}
#endif
	//'14.10.08
	// 課税取引計算書
	if(	m_Util.IsBasicMst(m_pZmSub->zvol) ){
		if( m_nowTabmax < 2 ){
			m_nowTabmax++;
		}
		if (GetActViewVer() == ID_VER_SYZ31) {
			AfxSetResourceHandle( GetTaxationList31Instance() );
			//CTaxationList31View *pTaxationList31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 4 ));
			//if( pTaxationList31View == NULL ){
			//	if(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(4)){
			//		ErrorExit( _T("課税取引計算書画面の作成に失敗しました") );
			//		return 0;
			//	}
			//	m_nowTabmax++;
			//}
			// 他のビューの初期化
			if( InitOtherView(4, m_pZmSub) ){
				return 0;
			}
		}
		else{
			AfxSetResourceHandle( GetTaxationListInstance() );
			CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
			//if( pTaxationListView == NULL ){
			//	if(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(3)){
			//		ErrorExit( _T("課税取引計算書画面の作成に失敗しました") );
			//		return 0;
			//	}
			//	m_nowTabmax++;
			//}
			// 他のビューの初期化
			if( InitOtherView(1, m_pZmSub) ){
				return 0;
			}
		}
	}
//--> '15.03.18 INS START
	else if( m_Util.IsSpcMaster(m_pZmSub) ){	// 特定収入計算表
		//AfxSetResourceHandle( GetTaxationListInstance() );
		//CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
		//if( pTaxationListView == NULL ){
		//	if(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(3)){
		//		ErrorExit( _T("特定収入計算表画面の作成に失敗しました") );
		//		return 0;
		//	}
		//	m_nowTabmax++;
		//} 他のビューの初期化
		//if( InitOtherView(2, m_pZmSub) ){
		//	return 0;
		//}

		if( m_nowTabmax < 2 ){
			m_nowTabmax++;
		}
		if (GetActViewVer() == ID_VER_SYZ31) {
			AfxSetResourceHandle( GetTaxationList31Instance() );
			// 他のビューの初期化
			if( InitOtherView(5, m_pZmSub) ){
				return 0;
			}
		}
		else{
			AfxSetResourceHandle( GetTaxationListInstance() );
			CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
			// 他のビューの初期化
			if( InitOtherView(2, m_pZmSub) ){
				return 0;
			}
		}
	}
//<-- '15.03.18 INS END

	// 元に戻す
	AfxSetResourceHandle( hInst );

	SetTabTitle(1);

	// 進捗可能チェック
	InitProgressInfo();

//-- '19.02.26 --
// CUT：Open処理の中に吸収したので処理をカット
//	// 過年度マスター存在チェック＆読み込み
//	m_KndFlg = SetKanendoData();
//---------------

	return 0;
}

//-----------------------------------------------------------------------------
// 変数初期設定
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::InitVariable()
{
	memset( &m_SkjCnvtbl, '\0', sizeof(_TB_PAR) );
	m_SkjCnvtbl.tp_tno = -1;
	m_Arith.l_defn( 0x16 );
	memset( &m_SvPayment, '\0', sizeof(m_SvPayment) );
	memset( &m_Month26, '\0', sizeof(m_Month26) );
	memset( &m_SYsw, '\0', sizeof(m_SYsw) );
	memset( &m_dtl, '\0', sizeof(m_dtl) );
	memset( &m_ShinInfo, '\0', sizeof(SYC_SHININFO) );
	memset( &m_Eselect, '\0', sizeof(ESELECT) );
	memset( &m_O_HEAD, '\0', sizeof(OFC_HEAD) );
	memset( &m_O_DATA, '\0', sizeof(OFC_DATA) );
	memset( &m_Z_DATA, '\0', sizeof(ZEI_DATA) );
	memset( &m_C_DATA, '\0', sizeof(COP_DATA) );
	memset( &m_Progdata, '\0', sizeof(struct ProgDataEx) );
	memset( &m_FinishPrn, '\0', sizeof(m_FinishPrn) );
//--> '16.01.30 INS START
	memset( &m_TgPDat, '\0', sizeof(TgPackData) );
	m_TgPDat.TgCode = -1;
//<-- '16.01.30 INS END

//--> '18.12.17 INS START
	m_bAlterIndex = false;
	m_rtIsRegistMcode = -999;
	memset( &m_ZMasterinfo, '\0', sizeof(ZEN_MASTERINF) );
//<-- '18.12.17 INS END
	
	m_SnHeadData.ClearReadData();//初期化
	m_ActViewVer = -1;
	m_ActedViewFlg = 0;
	
	if( m_pMainView ){
		m_pMainView->InitVariable();
	}

	return 0;
}

