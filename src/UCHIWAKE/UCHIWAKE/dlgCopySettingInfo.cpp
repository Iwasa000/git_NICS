// dlgCopySettingInfo.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgCopySettingInfo.h"
// midori 181002 add -->
#include "..\UCHIIMP\DBUchiImp8.h"
// midori 181002 add <--

// midori 181002 del -->
////// midori 180104 add -->
//extern BOOL	CALLBACK	_DBUtwCoselCallBack( void *pctl, int a, int b );
////// midori 180104 add <--
// midori 181002 del <--

// CdlgCopySettingInfo ダイアログ

// midori 156189,156190,156191 add -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);		// 改良No.21-0086,21-0529 del ※未使用のためコメント
// midori 156189,156190,156191 add <--

IMPLEMENT_DYNAMIC(CdlgCopySettingInfo, ICSDialog)

CdlgCopySettingInfo::CdlgCopySettingInfo(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgCopySettingInfo::IDD, pParent)
{

}

CdlgCopySettingInfo::~CdlgCopySettingInfo()
{
}

void CdlgCopySettingInfo::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CHECK_PRINT, m_chkPrint);
	DDX_Control(pDX, IDC_CHECK_KAMOKU, m_chkKamoku);
	DDX_Control(pDX, IDC_CHECK_TITLE, m_chkTitle);
}

// CdlgCopySettingInfo メッセージ ハンドラ
BEGIN_MESSAGE_MAP(CdlgCopySettingInfo, ICSDialog)
	ON_BN_CLICKED(IDOK, &CdlgCopySettingInfo::OnBnClickedOk)
END_MESSAGE_MAP()

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgCopySettingInfo::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// チェックボックス初期化（常にON）
	m_chkPrint.SetCheck( TRUE );
// midori 160612 cor -->
//	m_chkKamoku.SetCheck( TRUE );
//	m_chkTitle.SetCheck( TRUE );
// ---------------------
	// 入力データを確定するにチェックがある場合、
	// 科目情報、タイトル情報のコピーを無効とする
	if(m_DataKakutei)	{
		m_chkKamoku.SetCheck( FALSE );
		m_chkTitle.SetCheck( FALSE );
		m_chkKamoku.EnableWindow(FALSE);
		m_chkTitle.EnableWindow(FALSE);
	}
	else	{
		m_chkKamoku.SetCheck( TRUE );
		m_chkTitle.SetCheck( TRUE );
	}
// midori 160612 cor <--
	
	// 戻値を返す
	return( TRUE );
}

// midori 160612 cor -->
////**************************************************
////	ダイアログ表示
////	【引数】	pDB				…	データベースハンドル
////	【戻値】	ID_DLG_OK		…	Enterキーが押された
////				ID_DLG_CANCEL	…	Escキーが押された
////**************************************************
//int CdlgCopySettingInfo::ShowDialog( CDatabase* pDB)
// ---------------------
//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				pDataKakutei	…	入力データを確定する
//	【戻値】	ID_DLG_OK		…	Enterキーが押された
//				ID_DLG_CANCEL	…	Escキーが押された
//**************************************************
int CdlgCopySettingInfo::ShowDialog( CDatabase* pDB,BOOL pDataKakutei)
// midori 160612 cor <--
{
	m_nRet		= ID_DLG_CANCEL;		//	戻値初期化
	m_nErrPoint = 0;					// エラー箇所初期化
// midori 160612 add -->
	m_DataKakutei = pDataKakutei;
// midori 160612 add <--

	//	データベースハンドルあり？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;			//	データベースハンドル
				DoModal();				//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return( m_nRet );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgCopySettingInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		//switch( pMsg->wParam ){

		////	F2キーが押された
		//case VK_F2:
		//	keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
		//	keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
		//	keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
		//	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
		//	return( 1 );
		//	break;
		//}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////////
// GetSettingInfoFromOtherMaster
//
// ■各設定情報（科目、タイトル、出力設定）を他マスターからコピーする処理
//
// 引数		：int			index
//
// 戻り値	：int			0
//
////////////////////////////////////////////////////////////////////////////////
int CdlgCopySettingInfo::GetSettingInfoFromOtherMaster()
{
	// 各会社情報の取得
	char	szSQLServer[30];		// SQLサーバ名
	char	szApno[20];				// アプリケーション番号
	char	szCoCode[20];			// 会社コード
	char	szKikan[20];			// 期末年月日
// 21/07/21 add -->
	//char	szSvSQLServer[30];		// SQLサーバ名
	//char	szSvApno[20];			// アプリケーション番号
	//char	szSvCoCode[20];			// 会社コード
	//char	szSvKikan[20];			// 期末年月日
// 21/07/21 add <--

	// 会社選択画面表示
// midori 155314 add -->
	int		updsw=0;
// midori 155314 add <--
	int		nRet;
// midori 181002 add -->
	int		sw=0;
// midori 181002 add <--
// midori 155262 add -->
	int    nkanso[2]={0};
	BOOL   bkanso=FALSE;
// midori 155262 add <--
	CoSel	cs;						// 会社選択クラス
	COPAK	cop;
// midori 155262 add -->
	CString strData=_T("");
	CRecordset rs(((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->m_database);
// midori 155262 add <--

// 21/07/21 add -->
	//// 今の情報を取得
	//if(((CUCHIWAKEApp*)AfxGetApp())->GetPCOMSELECTCODSNPublic(szSvSQLServer, szSvApno, szSvCoCode, szSvKikan) == FALSE)	{
	//	return(-2);
	//}
// 21/07/21 add <--

// midori 181002 del -->
//	cop.sgn0 = 0x10;
//	cop.sgn1 = 0x10;
//// midori 180104-2 del -->
////// midori 180104 add -->
////	cop.pksw |= 0x1;
////	cop.cod[0] = cop.cod[1] = cop.cod[2] = cop.cod[3] = 0xff;
////	cop.apno = 0xffff;
////	cop.subs = 0x20;		// 勘定科目内訳書サイン
//////	cop.sgn4ex = 0x20;		// SUBマスターのみでも表示
//////	cop.sgn4ex|= 0x1000;	// 財務マスターを表示
////	cop.apfnc = (void(*)())_DBUtwCoselCallBack;
////// midori 180104 add <--
//// midori 180104-2 del <--
//// midori UC_0015 add -->
//	if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwInfoRegister)	{
//		cop.pksw |= 0x1;
//		cop.cod[0] = cop.cod[1] = cop.cod[2] = cop.cod[3] = 0xff;
//		cop.apno = 0xffff;
//		cop.subs = 0x20;		// 勘定科目内訳書サイン
//		cop.apfnc = (void(*)())_DBUtwCoselCallBack;
//	}
//// midori UC_0015 add <--
//	nRet = cs.co_ok( cop );
// midori 181002 del <--
// midori 181002 add -->
	if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwInfoRegister == 0) {
		cop.sgn0 = 0x10;
		cop.sgn1 = 0x10;
// midori UC_0037 add -->
#ifndef _DEBUG
// midori 150885 del -->
		//cop.pksw |= 0x1;
		//cop.sgn4ex = 0x1020;
		//cop.subs = 0x100000;
// midori 150885 del <--
// midori 150885 add -->
		CString	strGrNum = m_clsFunc.GetGrNo();
// 157759 del -->
		//if(strGrNum.CompareNoCase(_T("1B")) == 0)	{
// 157759 del <--
// 157759 add -->
		if(strGrNum.CompareNoCase(_T("1B")) == 0 && ((CUCHIWAKEApp*)AfxGetApp())->m_swIcspCoop == 0)	{
// 157759 add <--
			cop.pksw |= 0x1;
			cop.sgn4ex = 0x1020;
			cop.subs = 0x100000;
		}
// midori 150885 add <--
#endif
// midori UC_0037 add <--
		nRet = cs.co_ok( cop );
	}
	else {
		while( 1 ) {
			cop.sgn0 = 0x10;
			cop.sgn1 = 0x10;
			cop.pksw |= 0x1;
			cop.sgn4ex = 0x20;	// SUBマスターのみでも表示
			memset(cop.cod,0xff,sizeof( cop.cod ));
			nRet = cs.co_ok( cop );
			if(nRet != IDOK)	{
				if(sw != 0)	{
					m_nRet = -1;
					ICSDialog::OnCancel();
					return(1);
				}
				break;
			}
			nRet = MasterCheck();
			// エラー
			if(nRet == -1) {
				return( -1 );
			}
			// マスター無し
			if(nRet == 0) {
				sw = 1;
				continue;
			}
			break;
		}
	}
// midori 181002 add <--
	if( nRet == -1 ){
		return nRet;
	}
	else if( nRet == IDCANCEL ){
		return 1;
	}

	if( ((CUCHIWAKEApp*)AfxGetApp())->GetPCOMSELECTCODSNPublic(szSQLServer, szApno, szCoCode, szKikan) == FALSE ){
		return -2;
	}

	// 取得したデータより、コピー元とコピー先データベース名を作成する
	CString strBaseTable, strTargetTable;
	strBaseTable = ((CUCHIWAKEApp*)AfxGetApp())->MakeDatabaseNamePublic(szApno, szCoCode, szKikan);
	strTargetTable = ((CUCHIWAKEApp*)AfxGetApp())->GetTargetDatabaseName();

	// 同一マスターなら終了
	if( strBaseTable.Compare(strTargetTable) == 0 ){
		return -3;
	}

	if( ICSMessageBox( _T("コピーを開始します。よろしいですか？"), MB_OKCANCEL ) != IDOK ){
		return IDCANCEL;
	}

	// 財務クラスインスタンス作成
	CDBZmSub* pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;		// 財務クラス取得

	// コピー元のバージョンチェック
	if( CheckTableVersion(pZmSub, strBaseTable) == FALSE ){
		return -4;
	}

	// コピー元にレコードが存在するかチェックいる？

	CString strSQL;
	// ************************************
	// 印刷設定
	// ************************************
	if( m_chkPrint.GetCheck() == BST_CHECKED ){
		// トランザクション開始
		pZmSub->m_database->BeginTrans();

		try{
			// 出力設定(共通部分取得)コピー
			strSQL.Format(_T("UPDATE [%s].[dbo].[uc_inf_main] SET OutKikan = base.OutKikan, OutKingaku = base.OutKingaku, "
				"OutPage = base.OutPage, OutCoName = base.OutCoName FROM [%s].[dbo].[uc_inf_main] AS base "),
				strTargetTable, strBaseTable);

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

// midori UC_0056 add -->
			// 日付に元号を出力する
			strSQL.Empty();
			strSQL = GetSQLGeneralVarCopy_Infmain(strBaseTable, strTargetTable, 3, BIT_D3);
			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);
// midori UC_0056 add <--

			// 出力設定(個別部分)
			strSQL.Format(_T("UPDATE [%s].[dbo].[uc_inf_sub] SET OpTitleKn = base.OpTitleKn "
				"FROM [%s].[dbo].[uc_inf_sub] AS base WHERE base.FormSeq = [%s].[dbo].[uc_inf_sub].FormSeq "),
				strTargetTable, strBaseTable, strTargetTable);

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

			// 出力設定(金融機関一段表示)
			strSQL.Empty();
			strSQL.Format(_T("DECLARE @const_val binary DECLARE @kinyu binary "));
			for(int i=0; i<UC_ID_FORMNO_MAX; i++){
				strSQL.Format(strSQL + _T("SET @const_val = [dbo].[get_byte]((SELECT GeneralConstVal FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d ), 15) "
					"SET @kinyu = [dbo].[get_byte]((SELECT GeneralVar FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d ), 0) "
					"IF((@const_val & %d) > 0) BEGIN IF((@kinyu & %d) > 0) "
					"UPDATE [%s].[dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, %d, 1) WHERE FormOrder = %d "
					"ELSE UPDATE [%s].[dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, %d, 0) WHERE FormOrder = %d END "),
					strBaseTable, i+1, strBaseTable, i+1, BIT_D0, BIT_D0, strTargetTable, BIT_D0, i+1, strTargetTable, BIT_D0, i+1);
			}

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

// midori 190505 del -->
			//// 出力設定(科目行を出力する)
			//strSQL.Empty();
			//strSQL.Format(_T("DECLARE @const_val binary DECLARE @kamoku binary "));
			//for(int i=0; i<UC_ID_FORMNO_MAX; i++){
			//	strSQL.Format(strSQL + _T("SET @const_val = [dbo].[get_byte]((SELECT GeneralConstVal FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d ), 15) "
			//		"SET @kamoku = [dbo].[get_byte]((SELECT GeneralVar FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d ), 0) "
			//		"IF((@const_val & %d) > 0) BEGIN IF((@kamoku & %d) > 0) "
			//		"UPDATE [%s].[dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, %d, 1) WHERE FormOrder = %d "
			//		"ELSE UPDATE [%s].[dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, %d, 0) WHERE FormOrder = %d END "),
			//		strBaseTable, i+1, strBaseTable, i+1, BIT_D1, BIT_D4, strTargetTable, BIT_D4, i+1, strTargetTable, BIT_D4, i+1);
			//}

			//// SQL実行
			//pZmSub->m_database->ExecuteSQL(strSQL);
// midori 190505 del <--
// midori 156188 add -->
// midori 157042 del -->
			//if(KamokuRowEnableSgn(pZmSub->m_database,0) == 0) {
// midori 157042 del <--
			// 出力設定(科目行を出力する)
			strSQL.Empty();
			strSQL.Format(_T("DECLARE @const_val binary DECLARE @kamoku binary "));
			for(int i=0; i<UC_ID_FORMNO_MAX; i++){
				strSQL.Format(strSQL + _T("SET @const_val = [dbo].[get_byte]((SELECT GeneralConstVal FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d ), 15) "
					"SET @kamoku = [dbo].[get_byte]((SELECT GeneralVar FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d ), 0) "
					"IF((@const_val & %d) > 0) BEGIN IF((@kamoku & %d) > 0) "
					"UPDATE [%s].[dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, %d, 1) WHERE FormOrder = %d "
					"ELSE UPDATE [%s].[dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, %d, 0) WHERE FormOrder = %d END "),
					strBaseTable, i+1, strBaseTable, i+1, BIT_D1, BIT_D4, strTargetTable, BIT_D4, i+1, strTargetTable, BIT_D4, i+1);
			}

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);
// midori 157042 del -->
			//}
// midori 157042 del <--
// midori 156188 add <--

			// 金額０円データを出力しない
			strSQL.Empty();
			strSQL = GetSQLGeneralVarCopy(strBaseTable, strTargetTable, 0, BIT_D7);

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

			// 金額空欄データを出力しない
			strSQL.Empty();
			strSQL = GetSQLGeneralVarCopy(strBaseTable, strTargetTable, 1, BIT_D0);
			//strSQL.Format(_T("DECLARE @print_null binary "));
			//for(int i=0; i<UC_ID_FORMNO_MAX; i++){
			//	strSQL.Format(strSQL + _T("SET @print_null = [dbo].[get_byte]((SELECT GeneralVar FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d), 1) "
			//		"IF((@print_null & %d) > 0)	UPDATE [%s].dbo.[uc_inf_sub] set GeneralVar = [dbo].[set_byte2](GeneralVar, 1, %d, 1) WHERE FormOrder = %d"
			//		"ELSE						UPDATE [%s].dbo.[uc_inf_sub] set GeneralVar = [dbo].[set_byte2](GeneralVar, 1, %d, 0) WHERE FormOrder = %d"), 
			//		strBaseTable, i+1, BIT_D0, strTargetTable, BIT_D0, strTargetTable, BIT_D0);
			//}

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

			// 金額０円を印字する
			strSQL.Empty();
			strSQL = GetSQLGeneralVarCopy(strBaseTable, strTargetTable, 1, BIT_D1);
			//strSQL.Format(_T("DECLARE @hide_zero binary "));
			//for(int i=0; i<UC_ID_FORMNO_MAX; i++){
			//	strSQL.Format(strSQL + _T("SET @hide_zero = [dbo].[get_byte]((SELECT GeneralVar FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d), 1) "
			//		"IF((@hide_zero & %d) > 0)	UPDATE [%s].dbo.[uc_inf_sub] set GeneralVar = [dbo].[set_byte2](GeneralVar, 1, %d, 1) WHERE FormOrder = %d"
			//		"ELSE						UPDATE [%s].dbo.[uc_inf_sub] set GeneralVar = [dbo].[set_byte2](GeneralVar, 1, %d, 0) WHERE FormOrder = %d"), 
			//		strBaseTable, i+1, BIT_D1, strTargetTable, BIT_D1, strTargetTable, BIT_D1);
			//}

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

			// 省略文字(テーブルごとコピー)
			strSQL.Format(_T("if object_id('[%s].[dbo].[uc_inf_sub_omit]') is not null DROP TABLE [%s].[dbo].[uc_inf_sub_omit] "
				"SELECT * INTO [%s].[dbo].[uc_inf_sub_omit] FROM [%s].[dbo].[uc_inf_sub_omit] "),
				strTargetTable, strTargetTable, strTargetTable, strBaseTable);

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

			// トランザクションのコミット
			pZmSub->m_database->CommitTrans();
		}
		catch(CException* e){
			// 失敗したらロールバック
			pZmSub->m_database->Rollback();
			// エラー通知
			m_nErrPoint = ID_ERR_COPY_PRINT;

			e->Delete();

			return DB_ERR_EXESQL;
		}
// midori 155262 add -->
		// コピー先の簡素化フラグを取得
		strSQL = _T("SELECT [dbo].[get_byte](GeneralVar,2) FROM [") + strTargetTable + _T("].[dbo].[uc_inf_main] ");
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0,strData);
		nkanso[0] = _tstoi(strData);
		rs.Close();

		// コピー元の簡素化フラグを取得
		strSQL = _T("SELECT [dbo].[get_byte](GeneralVar,2) FROM [") + strBaseTable + _T("].[dbo].[uc_inf_main] ");
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0,strData);
		nkanso[1] = _tstoi(strData);
		rs.Close();

// midori 155314 add -->
		// コピー元のマスターに対して修復処理が行われているかを調べる
		strSQL = _T("SELECT [dbo].[get_byte](GeneralVar,3) FROM [") + strBaseTable + _T("].[dbo].[uc_inf_main] ");
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0,strData);
		updsw = _tstoi(strData);
		rs.Close();
		if((updsw & BIT_D0) == 0)	{
			// 件数を取得
			strSQL = _T("SELECT COUNT(FormSeq) FROM [") + strBaseTable + _T("].[dbo].[uc_inf_sub_omit] WHERE FormSeq = 1 ");
			rs.Open(CRecordset::forwardOnly, strSQL);
			rs.GetFieldValue((short)0,strData);
			rs.Close();
			// 新様式に設定されているのに様式１のレコードが３件ではない
			if((nkanso[1] & BIT_D1) == BIT_D1 && _tstoi(strData) != 3)	{
				nkanso[1] = 0;
			}
			// 旧様式に設定されているのに様式１のレコードが２件ではない
			if((nkanso[1] & BIT_D1) == 0 && _tstoi(strData) != 2)	{
				nkanso[1] = BIT_D1;
			}
		}
// midori 155314 add <--

		if((nkanso[0] & BIT_D1) != (nkanso[1] & BIT_D1)) {
			// コピー元：旧様式、コピー先：新様式
			if((nkanso[0] & BIT_D1) == 0 && (nkanso[1] & BIT_D1) == BIT_D1) {
				bkanso = FALSE;
			}
			// コピー元：新様式、コピー先：旧様式
			else {
				bkanso = TRUE;
			}
			((CUCHIWAKEApp*)AfxGetApp())->KansoOmitConvert(bkanso);
		}
// midori 155262 add <--
	}

	// ************************************
	// 科目情報
	// ************************************
	strSQL.Empty();
	if( m_chkKamoku.GetCheck() == BST_CHECKED ){
		// トランザクション開始
		pZmSub->m_database->BeginTrans();
		try{
			// 科目テーブルは削除後、データテーブルの科目情報も削除して、コピー
			strSQL.Format(_T("DELETE [%s].[dbo].[uc_lst_kamoku] "),
				strTargetTable, strTargetTable);

			pZmSub->m_database->ExecuteSQL(strSQL);

			// 各データテーブルの科目情報クリア　未使用
			//virFncReferenceClearKamokuTblAll();

			// uc_lst_kamokuテーブルコピー
			strSQL.Format(_T("INSERT INTO [%s].[dbo].[uc_lst_kamoku] SELECT * FROM [%s].[dbo].[uc_lst_kamoku] "),
				strTargetTable, strBaseTable);

			//SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

			// トランザクションのコミット
			pZmSub->m_database->CommitTrans();
		}
		catch(CException* e){
			// 失敗したらロールバック
			pZmSub->m_database->Rollback();
			// エラー通知
			m_nErrPoint = ID_ERR_COPY_KAMOKU;

			e->Delete();

			return DB_ERR_EXESQL;
		}
	}

	// 各様式テーブルの科目名更新
	SynchroReferenceData();
	
	// ************************************
	// タイトル情報
	// ************************************
	strSQL.Empty();
	if( m_chkTitle.GetCheck() == BST_CHECKED ){
		// トランザクション開始
		pZmSub->m_database->BeginTrans();
		try{
			// 出力設定(個別部分)、タイトル取得
			strSQL.Format(_T("UPDATE [%s].[dbo].[uc_inf_sub] SET TitleSave = base.TitleSave, TitleSaveTujo = base.TitleSaveTujo, "
				"Title = base.Title FROM [%s].[dbo].[uc_inf_sub] AS base WHERE base.FormSeq = [%s].[dbo].[uc_inf_sub].FormSeq "),
				strTargetTable, strBaseTable, strTargetTable);

			// SQL実行
			pZmSub->m_database->ExecuteSQL(strSQL);

			// トランザクションのコミット
			pZmSub->m_database->CommitTrans();
		}
		catch(CException* e){
			// 失敗したらロールバック
			pZmSub->m_database->Rollback();
			// エラー通知
			m_nErrPoint = ID_ERR_COPY_TITLE;

			e->Delete();

			return DB_ERR_EXESQL;
		}
	}

// 21/07/21 add -->
	//// 顧問情報に元の情報をセットしなおす
	//if( ((CUCHIWAKEApp*)AfxGetApp())->SetPCOMSELECTCODSNPublic(szSvSQLServer, szSvApno, szSvCoCode, szSvKikan) == FALSE)	{
	//	return(-2);
	//}
// 21/07/21 add <--

	return DB_ERR_OK;
}

////////////////////////////////////////////////////////////////////////////////
// SynchroReferenceData()
//
// ■コピーされた情報で各様式テーブルをKnOrderで更新（同期）する
//	未使用
//
// 引数		：なし
//
// 戻り値	：int			0
//
////////////////////////////////////////////////////////////////////////////////
int CdlgCopySettingInfo::SynchroReferenceData()
{
	CfrmUc000Common *pView = NULL;

	for(int i=ID_FORMNO_011; i<=ID_FORMNO_162; i++){
		// FrameObject取得
		pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(i);
		if( pView == NULL ){
			return FUNCTION_NG;
		}
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(i);		// 共通データ
		pView->virSetInitialValue();				// 帳表固有データ
		//科目更新
		pView->virFncReferenceUpdateTbl(ID_DLGTYPE_KAMOKU, ID_REFERENCE_MODE_COPY);

		if (pView != NULL) {
			delete pView;
			pView = NULL;
		}
	}

	for(int i=0; i<20; i++){
		// FrameObject取得
		pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_171 + i);
		if( pView == NULL ){
			return FUNCTION_NG;
		}
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_171 + i);		// 共通データ
		pView->virSetInitialValue();								// 帳表固有データ
		//科目更新
		pView->virFncReferenceUpdateTbl(ID_DLGTYPE_KAMOKU, ID_REFERENCE_MODE_COPY);

		if (pView != NULL) {
			delete pView;
			pView = NULL;
		}

		// FrameObject取得
		pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_181 + i);
		if( pView == NULL ){
			return FUNCTION_NG;
		}
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_181 + i);		// 共通データ
		pView->virSetInitialValue();								// 帳表固有データ
		//科目更新
		pView->virFncReferenceUpdateTbl(ID_DLGTYPE_KAMOKU, ID_REFERENCE_MODE_COPY);

		if (pView != NULL) {
			delete pView;
			pView = NULL;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// CheckTableVersion
//
// ■対象マスターの内訳書テーブルが最新バージョンと一致するかチェック
//
// 引数		：CDBZmSub		コピー元の財務クラス
//			：CString		コピー元のテーブル名
//
// 戻り値	：int			TRUE	…　一致する
//							FALSE	…　一致しない
//							  -1	…	エラー
//
////////////////////////////////////////////////////////////////////////////////
int CdlgCopySettingInfo::CheckTableVersion(CDBZmSub* pZmSub, CString strBaseTable)
{
	int version = DB_UC_TABLE_VERSION;
	int retVer = -1;
	int	intRet = TRUE;
	CRecordset	rs(pZmSub->m_database);
	CString		strSQL;
	CString		strVers;

	CString strMsg = "";

	// バージョンチェック
	strSQL.Format(_T("SELECT * FROM [%s].[dbo].[vtbl] WHERE type = %d AND objname = '%s'"),
		strBaseTable, DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);
	if ( !rs.Open( CRecordset::forwardOnly, strSQL ) ) {
		rs.Close();
		return DB_ERR_OPEN;
	}

	if( rs.GetRecordCount() == 0 ){
		rs.Close();
		return FALSE;
	}
	// 取得
	rs.GetFieldValue( _T("vers"), strVers );
	sscanf_s( strVers, "%d", &retVer);

	rs.Close();

	//　エラー(
	if ( retVer < 0 ){
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FALSE;
	}
	// バージョンが古い
	else if ( retVer < version ){
		strMsg.Format("コピー元のマスターが古いため、コピーできません。(バージョン：%d)。\nコピー元のマスターをバージョンアップしてください。",retVer);
		ICSMessageBox( strMsg, MB_OK | MB_ICONEXCLAMATION );
		intRet = FALSE;
	}
	// バージョンが新しすぎる
	else if ( retVer > version ){
		strMsg.Format("コピー元のマスターが更新されているため、コピーできません。(バージョン：%d)。\nアプリのバージョンアップが必要です。",retVer);
		ICSMessageBox( strMsg, MB_OK | MB_ICONEXCLAMATION );
		intRet = FALSE;
	}

	return intRet;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// [OK]ボタン クリック（OK）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgCopySettingInfo::OnBnClickedOk()
{
	// コピー項目チェック
	if(( m_chkPrint.GetCheck() == BST_UNCHECKED &&
		m_chkKamoku.GetCheck() == BST_UNCHECKED &&
		m_chkTitle.GetCheck() == BST_UNCHECKED )){

			ICSMessageBox( _T( "コピー項目が選択されていません。" ), MB_OK, 0, 0, this );

			return;
	}

	// コピー処理実行
	int nRet = GetSettingInfoFromOtherMaster();

	// 返送値からメッセージを表示する
	switch( nRet ){
		case DB_ERR_OK:
			ICSMessageBox( _T( "コピー完了。" ), MB_ICONINFORMATION, 0, 0, this );
			m_nRet = ID_DLG_OK;
			ICSDialog::OnOK();
			break;

		case DB_ERR_EXESQL:
			{
				CString strErr;
				switch( m_nErrPoint ){
					case ID_ERR_COPY_PRINT:
						strErr = _T("印刷設定コピー");
						break;
					case ID_ERR_COPY_KAMOKU:
						strErr = _T("科目情報コピー");
						break;
					case ID_ERR_COPY_TITLE:
						strErr = _T("タイトル情報コピー");
						break;
				}
				strErr += _T("でエラーが発生しました。");
				ICSMessageBox( _T( "コピー失敗。\n" ) + strErr, MB_ICONSTOP, 0, 0, this );
				m_nRet = ID_DLG_CANCEL;
				ICSDialog::OnCancel();
			}
			break;

		case IDCANCEL:
			m_nRet = ID_DLG_CANCEL;
			ICSDialog::OnCancel();
			break;

		case 1:
			break;

		case -1:
			ICSMessageBox( _T( "マスターをオープンできません。処理を終了します。" ), MB_ICONSTOP, 0, 0, this );
			m_nRet = ID_DLG_CANCEL;
			ICSDialog::OnCancel();
			break;

		case -2:
			ICSMessageBox( _T( "コモン情報が、取得できません。(SELECTCODSN)。\n処理を終了します。" ), MB_ICONSTOP, 0, 0, this );
			m_nRet = ID_DLG_CANCEL;
			ICSDialog::OnCancel();
			break;

		case -4:
			ICSMessageBox( _T( "内訳書マスターが存在しないマスターです。\nコピー元のマスターを選択し直してください。" ), MB_ICONWARNING, 0, 0, this );
			break;

		case -3:
			ICSMessageBox( _T( "同一マスターです。" ), MB_OK, 0, 0, this );
			break;

		default:
			m_nRet = ID_DLG_CANCEL;
			ICSDialog::OnCancel();
			break;
	}
}

CString CdlgCopySettingInfo::GetSQLGeneralVarCopy(CString strBaseTable, CString strTargetTable, int offset, int bit)
{
	CString sql;

	sql.Format(_T("DECLARE @val binary "));
	for(int i=0; i<UC_ID_FORMNO_MAX; i++){
		sql.Format(sql + _T("SET @val = [dbo].[get_byte]((SELECT GeneralVar FROM [%s].[dbo].[uc_inf_sub] WHERE FormOrder = %d), %d) "
			"IF((@val & %d) > 0)	UPDATE [%s].dbo.[uc_inf_sub] set GeneralVar = [dbo].[set_byte2](GeneralVar, %d, %d, 1) WHERE FormOrder = %d "
			"ELSE					UPDATE [%s].dbo.[uc_inf_sub] set GeneralVar = [dbo].[set_byte2](GeneralVar, %d, %d, 0) WHERE FormOrder = %d"), 
			strBaseTable, i+1, offset, bit, strTargetTable, offset, bit, i+1, strTargetTable, offset, bit, i+1);
	}

	return sql;
}

// midori UC_0056 add -->
CString CdlgCopySettingInfo::GetSQLGeneralVarCopy_Infmain(CString strBaseTable, CString strTargetTable, int offset, int bit)
{
	CString sql;

	sql.Format(_T("DECLARE @val binary "));
	sql.Format(sql + _T("SET @val = [dbo].[get_byte]((SELECT GeneralVar FROM [%s].[dbo].[uc_inf_main] ), %d) "
		"IF((@val & %d) > 0)	UPDATE [%s].dbo.[uc_inf_main] set GeneralVar = [dbo].[set_byte2](GeneralVar, %d, %d, 1)  "
		"ELSE					UPDATE [%s].dbo.[uc_inf_main] set GeneralVar = [dbo].[set_byte2](GeneralVar, %d, %d, 0) "), 
		strBaseTable, offset, bit, strTargetTable, offset, bit, strTargetTable, offset, bit);

	return sql;
}
// midori UC_0056 add <--
