//--------------------------------------------------
//	dlgPrintAll.cpp
//
//	2006.02.19～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgprintall.h"
// midori 160610 add -->
#include "dlgReference2.h"
// midori 160610 add <--
// midori 180101 add -->
#include "dlgPrintHyoshi.h"
// midori 180101 add <--
// midori 190505 add -->
#include "dlgIkkatuConfirmation.h"
// midori 190505 add <--

// midori 154982 add -->
extern	BOOL	bG_Kanso;
// midori 154982 add -->

// midori 190505 add -->
extern int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec);
// midori 190505 add <--


IMPLEMENT_DYNAMIC(CdlgPrintAll, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
//CdlgPrintAll::CdlgPrintAll(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgPrintAll::IDD, pParent)
CdlgPrintAll::CdlgPrintAll( CBatchPrt* pBatchPrt, CWnd* pParent /*=NULL*/) : ICSDialog(CdlgPrintAll::IDD, pParent), m_CheckNum(-1)
{
	// 2010/07/XX	一括印刷対応
	// CBatchPrtクラス
	m_pBatchPrt	= pBatchPrt;

	// 財務dbクラス
	m_pZmSub	= ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	// COwntbDataクラス
	if (m_pBatchPrt->IsBatchExec() == TRUE) {
		// owntb オープン
		m_pZmSub->OwnTblOpen();

		m_pOwntbData	= new COwntbData( m_pZmSub, pBatchPrt);

		//----------------------------------------------------------------------------------------
		// 2010/12/17 修正依頼No.N-035 一括印刷ユーザ情報共通化（運用中マスタ対応）
		// owntbのチェック 結果
		// owntb 一括印刷用 ユーザIDレコードチェック（共通ID、ログインID）
		// → 該当レコード無しの場合、誘導メッセージ表示 ＆ 一括印刷出力/CDR 正常終了
		//----------------------------------------------------------------------------------------
// 24/01/16_ScoreLink cor -->
		//if ( m_pBatchPrt->IsBatchModePrt() || m_pBatchPrt->IsBatchModeCDR() ) {
// --------------------------
		if((m_pBatchPrt->IsBatchModePrt() || m_pBatchPrt->IsBatchModeCDR()) && m_pBatchPrt->IsScoreLinkExec() == FALSE) {
// 24/01/16_ScoreLink cor <--
			// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
			//if ( m_pBatchPrt->IsDoSetMode() ) {
			//	// メッセージ表示
			//	ICSMessageBox( BATCHPRT_MSG_DOSETMODE, MB_ICONINFORMATION, 0, 0, this );
			//	// 一括印刷用 終了指示
			//	m_pBatchPrt->ExitBatchMode();
			//	return;
			//}
			int nRetSetMode = m_pBatchPrt->IsDoSetMode();
			if ((nRetSetMode == OWNTB_CHKREC_NG_DOSET01) ||
				(nRetSetMode == OWNTB_CHKREC_NG_DOSET02) ||
				(nRetSetMode == OWNTB_CHKREC_NG_DOSET03)) {
				// メッセージ表示
				switch ( nRetSetMode ) {
				case OWNTB_CHKREC_NG_DOSET01:
					ICSMessageBox( BATCHPRT_MSG_DOSETMODE01, MB_ICONINFORMATION, 0, 0, this );
					break;
				case OWNTB_CHKREC_NG_DOSET02:
					ICSMessageBox( BATCHPRT_MSG_DOSETMODE02, MB_ICONINFORMATION, 0, 0, this );
					break;
				case OWNTB_CHKREC_NG_DOSET03:
					ICSMessageBox( BATCHPRT_MSG_DOSETMODE03, MB_ICONINFORMATION, 0, 0, this );
					break;
				}
				// 一括印刷用 終了指示
				m_pBatchPrt->ExitBatchMode();
				return;
			}
		}
	}
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgPrintAll::CdlgPrintAll( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgPrintAll::~CdlgPrintAll()
{
	// 2010/07/XX	一括印刷対応
	if (m_pBatchPrt->IsBatchExec() == TRUE) {
		// owntb クローズ
		m_pZmSub->OwnTblClose();

		// COwntbDataクラス　解放
		if (m_pOwntbData != NULL) {
			delete m_pOwntbData;
		}
	}
}

void CdlgPrintAll::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_PRINTSETUP, m_btnPrintSetup);
// midori 160610 add -->
	DDX_Control(pDX, IDC_BUTTON_KAMOKU, m_btnKamoku);
// midori 160610 add <--
// midori 180101 add -->
	DDX_Control(pDX, IDC_BUTTON_HYOSHI, m_btnHyoshi);
// midori 180101 add <--
	DDX_Control(pDX, IDC_TITLE_CHECK, m_chkTitle);
	DDX_Control(pDX, IDC_NOW_CHECK, m_chkNow);
// midori 160608 add -->
	DDX_Control(pDX, IDC_ZERO_CHECK, m_chkZero);
// midori 160608 add <--
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDC_CUSTOM_REPORT_PRINTALL, m_report);
	DDX_Control(pDX, IDC_BUTTON_HYO_ALL, m_btnHyoChk);
}

BEGIN_MESSAGE_MAP(CdlgPrintAll, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TITLE_CHECK, &CdlgPrintAll::OnBnClickedTitleCheck)
	ON_BN_CLICKED(ID_PRINTSETUP, &CdlgPrintAll::OnBnClickedPrintsetup)
// midori 180101 del -->
//	ON_BN_CLICKED(IDC_NOW_CHECK, &CdlgPrintAll::OnBnClickedNowCheck)
// midori 180101 del <--
// midori 160608 add -->
	ON_BN_CLICKED(IDC_ZERO_CHECK, &CdlgPrintAll::OnBnClickedZeroCheck)
// midori 160608 add <--
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgPrintAll::OnBnClickedOK)
	ON_BN_CLICKED(IDC_BUTTON_HYO_ALL, &CdlgPrintAll::OnBnClickedButtonHyoAll)
	ON_NOTIFY(ICS_NM_REPORT_CHECKED, IDC_CUSTOM_REPORT_PRINTALL, OnNMReportCheckedCUSTOM_REPORT_PRINTALL)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_CUSTOM_REPORT_PRINTALL, OnNMReportEditkeydownCUSTOM_REPORT_PRINTALL)
	ON_NOTIFY(ICS_NM_REPORT_EDITKILLFOCUS, IDC_CUSTOM_REPORT_PRINTALL, OnNMReportEditkillfocusCUSTOM_REPORT_PRINTALL)
// midori 160610 add -->
	ON_BN_CLICKED(IDC_BUTTON_KAMOKU, &CdlgPrintAll::OnBnClickedButtonKamoku)
// midori 160610 add <--
// midori 180101 add -->
	ON_BN_CLICKED(IDC_BUTTON_HYOSHI, &CdlgPrintAll::OnBnClickedButtonHyoshi)
// midori 180101 add <--
END_MESSAGE_MAP()


void CdlgPrintAll::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgPrintAll::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgPrintAll::OnInitDialog()
{
	// 2010/07/XX 一括印刷対応　ログ
	const CString	FN = _T("CdlgPrintAll::GetPageData()");
	CString			strLog;
// midori 160608 add -->
	int				row=0;
// midori 160608 add <--

	DbgWriteLog( FN, _T("START") );

	ICSDialog::OnInitDialog();
	
	//ICSProgressWnd	icsPro( this );	//	プログレスバークラス
	
	//	プログレスバー開始
	//icsPro.BeginProgress(); 
	AfxGetApp()->DoWaitCursor(1);

// midori 160608 add -->
	// 一括印刷の場合
	// 「帳票イメージのみ出力する」のチェックはレポートの内容に影響するので
	// 他のチェックと同じ SetOutputChkOwntbData() 関数ではなく、ここで行う
	if(m_CheckNum == -1)	{
		if(g_BatchPrt.IsBatchExec() == TRUE)	{
			// 帳票イメージのみ出力するチェックの設定
			m_chkZero.SetCheck(m_pOwntbData->m_uInfo.uPrtCmn.nZeroSw);
		}
	}
// midori 160608 add <--

	InitReportHeader();

	CreatePrintAllVec();

// midori 160608 add -->
	if(m_chkZero.GetCheck() == 1)	{
		// 各様式のデータ有無チェック、及び「イメージ帳表のみ出力する」のチェックによるEnable/Disableの再設定
		for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++,row++){
			m_vecPrintAll[row].start = 1;
			m_vecPrintAll[row].end = 9999;
			m_vecPrintAll[row].inputStart = 1;
			m_vecPrintAll[row].inputEnd = 9999;
			m_vecPrintAll[row].str_start = _T("1");
			m_vecPrintAll[row].str_end = _T("1");
			m_vecPrintAll[row].isEnable = m_vecPrintAll[row].isEnableSgn;
// midori 190505 add -->
			m_vecPrintAll[row].inputCmb = -1;
			m_vecPrintAll[row].SortChk = 0;
// midori 190505 add <--
		}
	}

	//「イメージ帳表のみ出力する」にチェックがある場合、科目指定ボタンを無効にする
	if(m_chkZero.GetCheck() == 1)	m_btnKamoku.EnableWindow( FALSE );
	else							m_btnKamoku.EnableWindow( TRUE );
// midori 160608 add <--

// midori 190505 del -->
	//// 2010/07/XX 一括印刷対応
	//// 出力ページ範囲に、owntbに保存していた情報を反映させる。
	//// 　→一括印刷 全モード（設定、出力、CDR出力）で実行する
	//// －１、－２のページ数やenable状態を整理するため、ここでowntbの値を
	//// マップに反映する
	//SetPageRangeOwntbData();
// midori 190505 del <--

// midori 160608 cor -->
//	// ④－１④－２等のページ数を整理する
//	UpdateFormTypePage();
// ---------------------
	// 帳票イメージを出力しない場合
	if(m_chkZero.GetCheck() == 0)	{
		// ④－１④－２等のページ数を整理する
		UpdateFormTypePage();
	}
// midori 160608 cor <--

// midori 190505 add -->
	// 2010/07/XX 一括印刷対応
	// 出力ページ範囲に、owntbに保存していた情報を反映させる。
	// 　→一括印刷 全モード（設定、出力、CDR出力）で実行する
	// －１、－２のページ数やenable状態を整理するため、ここでowntbの値を
	// マップに反映する
	SetPageRangeOwntbData();
// midori 190505 add <--

	// レコード追加
	AddRecords();

	// 2010/07/XX 一括印刷対応
	// owntbに保存している出力ON/OFFの設定を画面に反映させる
	// 　→一括印刷 全モード（設定、出力、CDR出力）で実行する
	int focus_row = 0;
	if(m_CheckNum == -1)	SetOutputChkOwntbData();
	else					SetOutputChk(focus_row);

	OnBnClickedTitleCheck();	// 決算期間に「現在」を出力するの制御
// midori 180101 del -->
//	OnBnClickedNowCheck();
// midori 180101 del <--
// midori 160608 add -->
	//OnBnClickedZeroCheck();
	// ※「帳表イメージのみ出力する」のクリックイベント内にレポートの再表示関数が呼ばれているため、
	// 直接チェックボックスがチェックされているかどうかを確認して、変数に入れるようにします
	if ( IsDlgButtonChecked( IDC_ZERO_CHECK ) == TRUE )	m_nOutZero = 1;	// 「帳票イメージのみ出力する」ON
	else												m_nOutZero = 0;	// 「帳票イメージのみ出力する」OFF
// midori 160608 add <--

	UpdateVectorData();
	UpdateRecordState();

	//	プログレスバー終了
	//icsPro.EndProgress(); 
	AfxGetApp()->DoWaitCursor(-1);

	// リサイズ
	OnInitDialogEX();

	// レポートコントロールにフォーカス
	m_report.SetFocus();
	m_report.SetFocusedRow(focus_row);

	//-----------------------------------------------------
	// 一括印刷 出力 実行
	//-----------------------------------------------------
	// 2010/07/XX 一括印刷対応
	// 起動モード取得
	int	nMode = g_BatchPrt.GetBatchMode();
	switch ( nMode ) {
	case	ID_BATCHMODE_PRT:				// 一括印刷 出力モード
	case	ID_BATCHMODE_CDR:				// 一括印刷 CDR出力モード
		// F5押下メッセージ送信
		//g_BatchPrt.DoBatchPrint();
		PostMessage( WM_COMMAND, MAKEWPARAM(ID_OK_BUTTON, BN_CLICKED), 0 );
		break;

	case	ID_BATCHMODE_SET:				// 一括印刷 設定モード
		// 2010/07/31 仕様変更（7/30） エクセル出力以外は使用を許可する

		// ツールバーの Fボタン 使用禁止
		// → CCmnView::OnUpdateButtonF?() イベントにて対応。
		break;
	}

	// 2010/07/XX 一括印刷対応　ログ
	DbgWriteLog( FN, _T("END") );

	//	戻値を返す
	return( TRUE );
}

//**************************************************
//	出力ページ範囲 設定（owntb参照）
//	【引数】	なし
//	【戻値】	なし
//
//　2010/07/XX 一括印刷対応
//**************************************************
void CdlgPrintAll::SetPageRangeOwntbData( void )
{
	if (m_pBatchPrt->IsBatchExec() != TRUE)		return;

	const CString	FN = _T("CdlgPrintAll::SetPageRangeOwntbData()");
	CString			strLog;
	DbgWriteLog( FN, _T("START") );

	// 出力ページ範囲に、owntbに保存していた情報を反映させる。
	// 　→一括印刷 全モード（設定、出力、CDR出力）で実行する

	// ログ
	DbgWriteLog( FN, _T("出力ページ範囲をowntbより取得　開始") );

	for( int i = 0; i < UC_ID_FORMNO_MAX; i++ ){
		// 様式シーケンス番号を取得する
		int nFormSeq = m_vecFormOrder[i];

		vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), nFormSeq);
		if(it == m_vecPrintAll.end())	continue;

		// owntbから開始・終了出力ページ番号を取得する
		int nPageMin = m_pOwntbData->m_uInfo.uBPrtCmn.nOPRange_From[i],		// 開始出力ページ番号
			nPageMax = m_pOwntbData->m_uInfo.uBPrtCmn.nOPRange_To[i];		// 終了出力ページ番号

		// 出力チェックON？OFF？
		// →出力ONのみ、ページ番号を書き換える
		if (m_pOwntbData->m_uInfo.uBPrtCmn.nOutPut[i] == TRUE) {
			// 開始出力ページ
			//if (nPageMin <= GetPageAllFromList(nFormSeq)) {
			if(nPageMin <= it->end){
				// 開始出力ページを設定
				// →アプリが算出した終了出力ページ番号以下であれば、画面に反映させる
				CString startPage;
				startPage.Format("%d", nPageMin);
				//m_report.SetCaption(i, pa_start, startPage);
// midori M-16082401 del -->
				//it->start = nPageMin;
// midori M-16082401 del <--
				it->str_start = startPage;
				it->inputStart = nPageMin;
// midori 160608 add -->
				it->svstart = nPageMin;
// midori 160608 add <--
			}
			// 終了出力ページ
			//if (nPageMax <= GetPageAllFromList(nFormSeq)) {
			if(nPageMax <= it->end){
				// 終了出力ページを設定
				// →アプリが算出した終了出力ページ番号以下であれば、画面に反映させる
				CString endPage;
				endPage.Format("%d", nPageMax);
				//m_report.SetCaption(i, pa_end, endPage);
// midori M-16082401 del -->
				//it->end = nPageMax;
// midori M-16082401 del <--
				it->str_end = endPage;
				it->inputEnd = nPageMax;
// midori 160608 add -->
				it->svend = nPageMax;
// midori 160608 add <--
			}
		}

		// ログ
		strLog.Format( _T("%02d：FormSeq = %05d 開始ページ = [%04d] 終了ページ = [%04d]"), i+1, nFormSeq, nPageMin, nPageMax );
		DbgWriteLog( FN, strLog );
	}
	m_report.Populate();

	// ログ
	DbgWriteLog( FN, _T("出力ページ範囲をowntbより取得　開始") );

	// ログ
	DbgWriteLog( FN, _T("END") );
}

//**************************************************
//	出力ON/OFF 設定（owntb参照）
//	【引数】	なし
//	【戻値】	なし
//
//　2010/07/XX 一括印刷対応
//**************************************************
void CdlgPrintAll::SetOutputChkOwntbData( void )
{
	// ログ
	const CString	FN = _T("CdlgPrintAll::SetOutputOwntbData()");
	CString			strLog;
	DbgWriteLog( FN, _T("START") );

	// owntbに保存している出力ON/OFFの設定を画面に反映させる
	// 　→一括印刷 全モード（設定、出力、CDR出力）で実行する
	if(g_BatchPrt.IsBatchExec() == FALSE) return;

	int cnt = 0;
	for(int i=0; i<UC_ID_FORMNO_MAX; i++){
		int nFormSeq = m_vecFormOrder[i];
		vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), nFormSeq);
		if(it == m_vecPrintAll.end())	continue;

		m_report.SetChecked(cnt++, pa_select, m_pOwntbData->m_uInfo.uBPrtCmn.nOutPut[i]);
	}

	// 2010/07/XX 一括印刷対応
	// owntbに保存している設定を画面に反映させる
	// 　→一括印刷 全モード（設定、出力、CDR出力）で実行する
	// 表紙を出力するチェックの設定
	m_chkTitle.SetCheck(m_pOwntbData->m_uInfo.uPrtCmn.nCoverSw);
	// 決算期間の出力チェックの設定
	m_chkNow.SetCheck(m_pOwntbData->m_uInfo.uPrtCmn.nKsnAddNow);

	// ログ
	DbgWriteLog( FN, _T("END") );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	チェックボックスクリック（タイトル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintAll::OnBnClickedTitleCheck() 
{
	//	チェックボックスがチェックされている？
	if ( IsDlgButtonChecked( IDC_TITLE_CHECK ) == TRUE ){
		GetDlgItem( IDC_NOW_CHECK )->EnableWindow( TRUE );
		m_PrtWorkList[0].FgPrint = 1;
	}
	else{
		GetDlgItem( IDC_NOW_CHECK )->EnableWindow( FALSE );
		m_PrtWorkList[0].FgPrint = 0;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（印刷設定）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintAll::OnBnClickedPrintsetup() 
{
	CdlgPrintSetup		dlgSetup( this );	//	印刷設定ダイアログ
// midori 152385 add -->
	int svrow = 0;
// midori 152385 add <--
// midori 152393 add -->
	int child_num = 0;
	int ii=0;
	int row3=0;
// midori 152393 add <--

	UpdateVectorData();		// 印刷設定前のレポートコントロールの状態をマップに更新
	vector<PRINT_ALL_DATA> oldVecPrintAll = m_vecPrintAll;

// midori 151404 cor -->
//	BOOL isChange = FALSE;
//// midori 160608 cor -->
////	dlgSetup.ShowDialog( isChange, m_pDB , m_nFormSeq );
//// ---------------------
//	dlgSetup.ShowDialog( isChange, m_pDB , m_nFormSeq, m_nOutZero );
//// midori 160608 cor <--
// ---------------------
	int		row=0,row2=0;
	int		sw=0;
	BOOL	isChange = FALSE;
	BOOL	isYousiki[UC_ID_FORMNO_MAX] = {FALSE};
	CString	strPage = _T("");

	dlgSetup.ShowDialog( isChange, isYousiki, m_pDB , m_nFormSeq, m_nOutZero );
// midori 151404 cor <--

// midori 160608 add -->
	if(m_chkZero.GetCheck() == 1)			return;	// 帳票イメージのみ出力するにチェックがある場合更新しない
// midori 160608 add <--
	if(isChange == FALSE)					return;	// 「金額０円データを出力しない」or「金額空欄データを出力しない」に変更なければリターン

// midori 152385 add -->
	svrow = m_report.GetFocusedRow()->GetIndex();
// midori 152385 add <--

// midori M-16082402 cor -->
	//if(m_pBatchPrt->IsBatchExec() == TRUE)	return;	// 一括印刷時は更新しない
// -------------------------
	if(m_pBatchPrt->IsBatchExec() == TRUE)	{
		// 一括印刷時、詳細設定以外は更新しない
		if(m_pBatchPrt->IsBatchModeSet() != TRUE)	{
			return;
		}
	}
// midori M-16082402 cor <--

	AfxGetApp()->DoWaitCursor(1);

	CreatePrintAllVec();

	UpdateFormTypePage();

	AddRecords();

// midori 151404,M-16113006 cor -->
//	// AddRecordsでチェックは外れているため、チェック状態を元に戻す
//	for(vector<PRINT_ALL_DATA>::iterator it_oldvec = oldVecPrintAll.begin(); it_oldvec != oldVecPrintAll.end(); it_oldvec++){
//		if(it_oldvec->isEnable == FALSE)	continue;
//		if(it_oldvec->isSelect == FALSE)	continue;
//
//		// ０円設定によりoldVecPrintAllのレコード数とm_vecPrintAllのレコード数が異なることがあるので、
//		// FormSeqが一致するレコードのみチェックをつける
//		int row = 0;
//		for(vector<PRINT_ALL_DATA>::iterator it_newvec = m_vecPrintAll.begin(); it_newvec != m_vecPrintAll.end(); it_newvec++, row++){
//			if(it_oldvec->FormSeq == it_newvec->FormSeq){
//				m_report.SetChecked(row, pa_select, TRUE);
//				break;
//			}
//		}
//	}
// --------------------------------
	// 通常の場合、グループ内いずれかの様式に変更サインが立っていれば、グループ内すべての様式の変更サインを立てる
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(it->FormNo);
		if(it2 != mapFormType.end()){
			if(it2->second.FormType != ID_FORMTYPE_KOBETU)	{
				sw = 0;
				for(row2=0;row2<it2->second.num;row2++)	{
					if(isYousiki[it->FormOrder-1+row2] == TRUE)	{
						sw = 1;
						break;
					}
				}
				if(sw == 1)	{
					for(row2=0;row2<it2->second.num;row2++)	{
						isYousiki[it->FormOrder-1+row2] = TRUE;
					}
				}
			}
		}
	}

	row = 0;
	for(vector<PRINT_ALL_DATA>::iterator it_oldvec = oldVecPrintAll.begin(); it_oldvec != oldVecPrintAll.end(); it_oldvec++, row++){
		if(it_oldvec->isEnable == FALSE)	continue;

		// ０円設定によりoldVecPrintAllのレコード数とm_vecPrintAllのレコード数が異なることがあるので、
		// FormSeqが一致するレコードのみチェックをつける
		row2 = 0;
		for(vector<PRINT_ALL_DATA>::iterator it_newvec = m_vecPrintAll.begin(); it_newvec != m_vecPrintAll.end(); it_newvec++, row2++){
			if(it_oldvec->FormSeq == it_newvec->FormSeq){
				// AddRecordsでチェックは外れているため、チェック状態を元に戻す
				if(it_oldvec->isSelect != FALSE)	m_report.SetChecked(row2, pa_select, TRUE);
				if(isYousiki[row] == FALSE)	{
					// 変更無しの開始頁、終了頁を元の頁に戻す
					m_vecPrintAll[row2].start = oldVecPrintAll[row].start;
					m_vecPrintAll[row2].end = oldVecPrintAll[row].end;
					m_vecPrintAll[row2].inputStart = oldVecPrintAll[row].inputStart;
					m_vecPrintAll[row2].inputEnd = oldVecPrintAll[row].inputEnd;
					m_vecPrintAll[row2].str_start = oldVecPrintAll[row].str_start;
					m_vecPrintAll[row2].str_end = oldVecPrintAll[row].str_end;
// midori 190505 add -->
					m_vecPrintAll[row2].inputCmb = oldVecPrintAll[row].inputCmb;
					m_vecPrintAll[row2].SortChk = oldVecPrintAll[row].SortChk;
// midori 190505 add <--
// midori 20/10/08u add -->
					m_vecPrintAll[row2].isMainasu = oldVecPrintAll[row].isMainasu;
// midori 20/10/08u add <--
					m_vecPrintAll[row2].svstart = oldVecPrintAll[row].svstart;
					m_vecPrintAll[row2].svend = oldVecPrintAll[row].svend;
					m_vecPrintAll[row2].start2 = oldVecPrintAll[row].start2;
					m_vecPrintAll[row2].end2 = oldVecPrintAll[row].end2;
					m_vecPrintAll[row2].inputStart2 = oldVecPrintAll[row].inputStart2;
					m_vecPrintAll[row2].inputEnd2 = oldVecPrintAll[row].inputEnd2;
					m_vecPrintAll[row2].str_start2 = oldVecPrintAll[row].str_start2;
					m_vecPrintAll[row2].str_end2 = oldVecPrintAll[row].str_end2;
// midori 190505 add -->
					m_vecPrintAll[row2].inputCmb2 = oldVecPrintAll[row].inputCmb2;
					m_vecPrintAll[row2].SortChk2 = oldVecPrintAll[row].SortChk2;
// midori 190505 add <--
// midori 20/10/08u add -->
					m_vecPrintAll[row2].isMainasu2 = oldVecPrintAll[row].isMainasu2;
// midori 20/10/08u add <--
					strPage.Empty();
					strPage.Format(_T("%d"),m_vecPrintAll[row2].inputStart);
					m_report.SetCaption(row2, pa_start, strPage);
					strPage.Empty();
					strPage.Format(_T("%d"),m_vecPrintAll[row2].inputEnd);
					m_report.SetCaption(row2, pa_end, strPage);
				}
				break;
			}
		}
	}
// midori 151404,M-16113006 cor <--
// midori 152393 add -->
	SetOutputChk2(isYousiki);
// midori 152393 add <--

	UpdateVectorData();

	UpdateRecordState();

	AfxGetApp()->DoWaitCursor(-1);

// midori 152385 add -->
	m_report.SetFocusedRow(svrow);
	m_btnPrintSetup.SetFocus();
// midori 152385 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// [OK]ボタン クリック（OK）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintAll::OnBnClickedOK()
{
	// ベクタのデータ更新
	UpdateVectorData();

	// ページチェック
	if(GetCheckedNum() < 1)	{
		ICSMessageBox( "出力データがありません。", MB_OK, 0, 0, this );
		return;
	}

// midori 190505 del -->
//	// 2010/07/XX 一括印刷対応
//	// 画面の情報をowntbに渡す
//	// 　→一括印刷 設定モード時のみ実行
//	if (m_pBatchPrt->IsBatchModeSet() == TRUE) {
//		//--------------------------------------
//		// 帳表出力ON/OFFと出力ページ範囲
//		//--------------------------------------
//		// 表紙+62帳表分=63回のループを回す
//		// (ただし、1回目は表紙の情報が入っているのでスキップする)
//		// ※CheckPageAll()で画面の最新データを取得している。
//
//		for (int cnt=0; cnt<UC_ID_FORMNO_MAX; cnt++){
//			vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), m_vecFormOrder[cnt]);
//			if(it == m_vecPrintAll.end())	continue;
//			m_pOwntbData->m_uInfo.uBPrtCmn.nOutPut[cnt] = it->isSelect;
//			if(it->isSelect == FALSE)	continue;
//
//			m_pOwntbData->m_uInfo.uBPrtCmn.nOPRange_From[cnt] = it->inputStart;
//			m_pOwntbData->m_uInfo.uBPrtCmn.nOPRange_To[cnt] = it->inputEnd;
//		}
//
//		//--------------------------------------
//		// 「表紙を出力する」と「決算期間の出力」
//		//--------------------------------------
//		// 表紙を出力するチェックの取得
//		m_pOwntbData->m_uInfo.uPrtCmn.nCoverSw		= m_chkTitle.GetCheck();
//		// 決算期間の出力チェックの取得
//		m_pOwntbData->m_uInfo.uPrtCmn.nKsnAddNow	= m_chkNow.GetCheck();
//// midori 160608 add -->
//		// 帳票イメージのみ出力するチェックの取得
//		m_pOwntbData->m_uInfo.uPrtCmn.nZeroSw		= m_chkZero.GetCheck();
//// midori 160608 add <--
//	}
//
//	// 出力リストにデータを格納
//	// 表紙
//	int index = 1;	// インデックス0は予約済み
//	if(m_chkTitle.GetCheck()){
//		m_PrtList[index].FormSeq = SchSeqCvr;
//		m_PrtList[index].PageMin = 1;
//		m_PrtList[index].PageMax = 1;
//		index++;
//	}
//
//	// 各様式
//	int cnt = 0;
//	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
//		// 出力する
//		if(it->isSelect != FALSE){
//			m_PrtList[index].FormSeq = it->FormSeq;
//			m_PrtList[index].PageMin = atoi(m_report.GetCaption(cnt, pa_start));
//			m_PrtList[index].PageMax = atoi(m_report.GetCaption(cnt, pa_end));
//			index++;
//		}
//		cnt++;
//	}
//
//	// 2010/07/XX 一括印刷対応
//	// owntbに情報を保存する→一括印刷 設定モード時のみ実行
//	// ※出力リストのデータ有無に関わらず、owntbのデータはこのタイミングで確定する。
//	if (m_pBatchPrt->IsBatchModeSet() == TRUE) {
//		m_pOwntbData->SaveAllData();
//	}
//
//	// 出力リストに、データがひとつもない場合は、キャンセルの処理
//	// データがある場合は、必ず出力リストのインデックス1のFormSeqに「0」以外の値が入る
//	if( m_PrtList[1].FormSeq == 0 ){
//
//		ICSDialog::OnCancel();
//		return;
//	}
// midori 190505 del <--
// midori 190505 add -->
// midori 20/10/08_2 add -->
	if(prnConfimationSw == TRUE) {
// midori 20/10/08_2 add <--
		// →一括印刷 設定モード時のみ実行
		if(m_pBatchPrt->IsBatchExec() == FALSE || (m_pBatchPrt->IsBatchExec() == TRUE && m_pBatchPrt->IsBatchModeSet() == TRUE))	{
			// 一括印刷用の確認ダイアログを表示する
			// 「帳表イメージのみ出力」にチェックが付いているときは表示しない
			if(m_chkZero.GetCheck() != 1)	{
				int		ret=0;
				ret = CreateIkkatuConfDlg();
				if(ret == ID_DLG_CANCEL)	{
					return;
				}

				// 一括印刷でない時、出力方法を CdbUcInfSub に保存
				if(m_pBatchPrt->IsBatchExec() == FALSE)	{
					CdbUcInfSub		mfcRec(m_pDB);
					for (int cnt=0; cnt<UC_ID_FORMNO_MAX; cnt++){
						vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), m_vecFormOrder[cnt]);
						if(it == m_vecPrintAll.end())	continue;
						// 一括印刷用の確認ダイアログに表示されていない様式は「出力方法」保存の対象外
						if(it->isSelect2 != TRUE || it->SortChk != 1)	continue;

						if(mfcRec.RequeryFormSeq(it->FormSeq) == DB_ERR_OK)	{
							if( !(mfcRec.IsEOF()) )	{
								CCtrlSubGeneralVar	cs(mfcRec);

								// 編集
								mfcRec.Edit();
// midori 20/10/08u del -->
								//// 出力方法
								//if(it->inputCmb == 0)	{
								//	// 金額０円(空欄)データを削除した分、詰めて出力
								//	cs.bPrnHoho1 = TRUE;
								//	cs.bPrnHoho2 = FALSE;
								//	cs.bPrnHoho3 = FALSE;
								//}
								//else if(it->inputCmb == 1)	{
								//	// 金額０円(空欄)データを削除するが、画面上で行った改頁を保って出力
								//	cs.bPrnHoho1 = FALSE;
								//	cs.bPrnHoho2 = TRUE;
								//	cs.bPrnHoho3 = FALSE;
								//}
								//else if(it->inputCmb == 2)	{
								//	// 編集－ソート設定の第１ソート項目でソートを行い、改頁して出力
								//	cs.bPrnHoho1 = FALSE;
								//	cs.bPrnHoho2 = FALSE;
								//	cs.bPrnHoho3 = TRUE;
								//}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
								// 出力方法
								if(it->inputCmb == 0)	{
									// 金額０円(空欄)データを削除した分、詰めて出力
									cs.bPrnHoho1 = TRUE;
									cs.bPrnHoho2 = FALSE;
								}
								else if(it->inputCmb == 1)	{
									// 編集－ソート設定の第１ソート項目でソート改頁して出力
									cs.bPrnHoho1 = FALSE;
									cs.bPrnHoho2 = TRUE;
								}
// midori 20/10/08u add <--
								cs.Set(mfcRec);
								// 更新
								mfcRec.Update();
							}
							mfcRec.Fin();
						}
					}
				}
			}
		}
// midori 20/10/08_2 add -->
	}
// midori 20/10/08_2 add <--

	// 出力リストにデータを格納
	// 表紙
	int index = 1;	// インデックス0は予約済み
	if(m_chkTitle.GetCheck())	{
		m_PrtList[index].FormSeq = SchSeqCvr;
		m_PrtList[index].PageMin = 1;
		m_PrtList[index].PageMax = 1;
		index++;
	}
	// 各様式
	int cnt = 0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
		// 出力する
		if(it->isSelect != FALSE)	{
			m_PrtList[index].FormSeq = it->FormSeq;
			m_PrtList[index].PageMin = it->inputStart;
			m_PrtList[index].PageMax = it->inputEnd;
			index++;
		}
		cnt++;
	}

	// 2010/07/XX 一括印刷対応
	// 画面の情報をowntbに渡す
	// 　→一括印刷 設定モード時のみ実行
	if (m_pBatchPrt->IsBatchModeSet() == TRUE) {
		//--------------------------------------
		// 帳表出力ON/OFFと出力ページ範囲
		//--------------------------------------
		// 表紙+62帳表分=63回のループを回す
		// (ただし、1回目は表紙の情報が入っているのでスキップする)
		// ※CheckPageAll()で画面の最新データを取得している。

		for (int cnt=0; cnt<UC_ID_FORMNO_MAX; cnt++){
			vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), m_vecFormOrder[cnt]);
			if(it == m_vecPrintAll.end())	continue;
			m_pOwntbData->m_uInfo.uBPrtCmn.nOutPut[cnt] = it->isSelect;
			if(it->isSelect == FALSE)	continue;

			m_pOwntbData->m_uInfo.uBPrtCmn.nOPRange_From[cnt] = it->inputStart;
			m_pOwntbData->m_uInfo.uBPrtCmn.nOPRange_To[cnt] = it->inputEnd;
// midori 20/10/08_2 del -->
			//// 一括印刷用の確認ダイアログに表示されていない様式は「出力方法」保存の対象外
			//if(it->isSelect2 == TRUE && it->SortChk == 1) {
			//	m_pOwntbData->m_uInfo.uBPrtCmn.nOPPrnHoho[cnt] = it->inputCmb;
			//}
// midori 20/10/08_2 del <--
// midori 20/10/08_2 add -->
			if(prnConfimationSw == TRUE) {
				// 一括印刷用の確認ダイアログに表示されていない様式は「出力方法」保存の対象外
				if(it->isSelect2 == TRUE && it->SortChk == 1) {
					m_pOwntbData->m_uInfo.uBPrtCmn.nOPPrnHoho[cnt] = it->inputCmb;
				}
			}
// midori 20/10/08_2 add <--
		}

		//--------------------------------------
		// 「表紙を出力する」と「決算期間の出力」
		//--------------------------------------
		// 表紙を出力するチェックの取得
		m_pOwntbData->m_uInfo.uPrtCmn.nCoverSw		= m_chkTitle.GetCheck();
		// 決算期間の出力チェックの取得
		m_pOwntbData->m_uInfo.uPrtCmn.nKsnAddNow	= m_chkNow.GetCheck();
		// 帳票イメージのみ出力するチェックの取得
		m_pOwntbData->m_uInfo.uPrtCmn.nZeroSw		= m_chkZero.GetCheck();
	}

	// 2010/07/XX 一括印刷対応
	// owntbに情報を保存する→一括印刷 設定モード時のみ実行
	// ※出力リストのデータ有無に関わらず、owntbのデータはこのタイミングで確定する。
	if (m_pBatchPrt->IsBatchModeSet() == TRUE) {
		m_pOwntbData->SaveAllData();
	}

	// 出力リストに、データがひとつもない場合は、キャンセルの処理
	// データがある場合は、必ず出力リストのインデックス1のFormSeqに「0」以外の値が入る
	if( m_PrtList[1].FormSeq == 0 )	{
		ICSDialog::OnCancel();
		return;
	}
// midori 190505 add <--

	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintAll::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgPrintAll::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

		//	F2キーが押された
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
			break;

		case VK_DECIMAL:		// テンキーの「.」
		case VK_OEM_PERIOD:		// .（かなの「る」）
		case VK_SUBTRACT:		// テンキーの「-」
		case VK_OEM_MINUS:		// -（かなの「ほ」）
			return 1;

		case VK_SPACE:
		case VK_RETURN:
		case VK_TAB:
			if(m_clsFunc.GetFocusControlID(this) == IDC_CUSTOM_REPORT_PRINTALL){
				UpdateStateFromKey((int)pMsg->wParam);
				return 1;
			}
			break;
		case VK_ESCAPE:
			ICSDialog::OnCancel();
			return 1;

		case VK_F5:
			OnBnClickedOK();
			return 1;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// midori 180101 del -->
////++++++++++++++++++++++++++++++++++++++++++++++++++
//// チェックボックス クリック（「現在」出力）
////++++++++++++++++++++++++++++++++++++++++++++++++++
//void CdlgPrintAll::OnBnClickedNowCheck() 
//{
//	//	チェックボックスがチェックされている？
//	if ( IsDlgButtonChecked( IDC_NOW_CHECK ) == TRUE )	m_nOutGenzai = 1;	// 「現在」出力 ON
//	else												m_nOutGenzai = 0;	// 「現在」出力 OFF
//}
// midori 180101 del <--

// midori 160608 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
// チェックボックス クリック（「帳票イメージのみ出力する」）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintAll::OnBnClickedZeroCheck()
{
	int row = 0;
// midori 152385 add -->
	int svrow = 0;

	svrow = m_report.GetFocusedRow()->GetIndex();
// midori 152385 add <--

	//	チェックボックスがチェックされている？
	if ( IsDlgButtonChecked( IDC_ZERO_CHECK ) == TRUE )	m_nOutZero = 1;	// 「帳票イメージのみ出力する」ON
	else												m_nOutZero = 0;	// 「帳票イメージのみ出力する」OFF

	// 各様式のデータ有無チェック、及び「イメージ帳表のみ出力する」のチェックによるEnable/Disableの再設定
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++,row++){
		if(m_chkZero.GetCheck() == 1)	{
			m_vecPrintAll[row].start = 1;
			m_vecPrintAll[row].end = 9999;
			m_vecPrintAll[row].inputStart = 1;
			m_vecPrintAll[row].inputEnd = 9999;
			m_vecPrintAll[row].str_start = _T("1");
			m_vecPrintAll[row].str_end = _T("1");
			m_vecPrintAll[row].isEnable = m_vecPrintAll[row].isEnableSgn;
// midori 190505 add -->
			m_vecPrintAll[row].inputCmb = -1;
			m_vecPrintAll[row].SortChk = 0;
// midori 190505 add <--
// midori 20/10/08u add -->
			m_vecPrintAll[row].isMainasu = 0;
// midori 20/10/08u add <--
		}
		else	{
			m_vecPrintAll[row].start = m_vecPrintAll[row].start2;
			m_vecPrintAll[row].end = m_vecPrintAll[row].end2;
			m_vecPrintAll[row].inputStart = m_vecPrintAll[row].inputStart2;
			m_vecPrintAll[row].inputEnd = m_vecPrintAll[row].inputEnd2;
			m_vecPrintAll[row].str_start = m_vecPrintAll[row].str_start2;
			m_vecPrintAll[row].str_end = m_vecPrintAll[row].str_end2;
			m_vecPrintAll[row].isEnable = m_vecPrintAll[row].isEnable2;
// midori 190505 add -->
			m_vecPrintAll[row].inputCmb = m_vecPrintAll[row].inputCmb2;
			m_vecPrintAll[row].SortChk = m_vecPrintAll[row].SortChk2;
// midori 190505 add <--
// midori 20/10/08u add -->
			m_vecPrintAll[row].isMainasu = m_vecPrintAll[row].isMainasu2;
// midori 20/10/08u add <--
		}
	}
	if(m_chkZero.GetCheck() == 1)	{
		//「イメージ帳表のみ出力する」にチェックがある場合、科目指定ボタンを無効にする
		m_btnKamoku.EnableWindow( FALSE );
	}
	else							{
		// 出力形式の設定により、様式のページ数、有効無効を切り替える
		UpdateFormTypePage();
		//「イメージ帳表のみ出力する」にチェックが無い場合、科目指定ボタンを有効にする
		m_btnKamoku.EnableWindow( TRUE );
	}

	// レポート、レコードの追加。再表示もここで賄っている
	AddRecords();

	UpdateVectorData();

	UpdateRecordState();

// midori 152385 add -->
	m_report.SetFocusedRow(svrow);
	m_chkZero.SetFocus();
// midori 152385 add <--

}
// midori 160608 add <--

//**************************************************
//	ダイアログ表示
//	【引数】	call_by			…	-1:一括印刷　0:F5キー　左記以外:F4キー（様式シーケンス）
//				pDB				…	データベースハンドル
//	【戻値】	ID_DLG_OK		…	印刷ボタンが押された
//				ID_DLG_CANCEL	…	キャンセルボタンが押された
//**************************************************
int CdlgPrintAll::ShowDialog(int call_by, int nFormSeq, CDatabase* pDB )
{
	m_nRet = ID_DLG_CANCEL;		//	戻値初期化
// midori 180101 del -->
//	m_nOutGenzai = 0;			//  「現在」出力の初期化（0:出力しない）
// midori 180101 del <--
// midori 160608 add -->
	m_nOutZero = 0;				//  「帳票イメージのみ出力する」の初期化（0:出力しない）
// midori 160608 add <--
	m_blnPrnChk = FALSE;		//	印刷可能帳表判断フラグ初期化

	// 出力リストの初期化
	ZeroMemory( m_PrtList, sizeof( m_PrtList ) );

	// 作業用リストの初期化
	ZeroMemory( m_PrtWorkList, sizeof( m_PrtWorkList ) );

	// 総ページ数リストの初期化
	ZeroMemory( m_PrtPageAllList, sizeof( m_PrtPageAllList ) );

	// 2010/07/XX 一括印刷対応
	// owntbデータ 取得
	// 　→一括印刷　全モード（設定、出力、CDR出力）が対象
	if (m_pBatchPrt->IsBatchExec() == TRUE) {
		// owntbに保存している
		if (m_pOwntbData->LoadAllData() == FALSE) {
			// エラー処理
			ICSMessageBox(_T("データの読込みに失敗しました"));
			m_nRet = ID_DLG_CANCEL;
		}
	}

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;	//	データベースハンドル取得
				m_CheckNum = call_by;
				m_nFormSeq = nFormSeq;
				DoModal();		//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return( m_nRet );
}

//**************************************************
//	本データ有無のチェック（メイン）
//	【引数】	CdbUcInfSub		…	様式個別データ
//	【戻値】	TRUE			…	本データ有り
//				FALSE			…	本データ無し
//**************************************************
BOOL CdlgPrintAll::CheckPrintDataMain(CdbUcInfSub& mfcSub)
{
	// 通常なら、様式シーケンス番号でデータ有無をチェックする
	// ただし、通常帳表では、④-1、④-2のいずれかデータありで判断する
	CdbUcInfFormType	rsFormType(m_pDB);
	int					intFormNo = mfcSub.m_FormNo;
	int					intFormType = ID_FORMTYPE_KOBETU;		// 個別のみの帳表もあるため
	int					nFormSeq = mfcSub.m_FormSeq;

	try {
		// uc_inf_form_type参照
		if (rsFormType.Init(intFormNo) == DB_ERR_OK) {
			// 通常/個別の情報を取得
			if (!rsFormType.IsEOF()) {
				intFormType = rsFormType.m_FormType;
			}
			rsFormType.Fin();
		}

		// データ有無のチェック
		if (intFormType == ID_FORMTYPE_KOBETU) {
			// 個別帳表なら、FormSeq番号でデータ有無をチェック
			return CheckPrintDataSub(nFormSeq);
		}
		else {
			// 通常帳表なら、関連データのいずれかデータありで判断する
			switch (nFormSeq) {
			case ID_FORMNO_041:
			case ID_FORMNO_042:
				// ④-1 or ④-2
				if (CheckPrintDataSub(ID_FORMNO_041) == TRUE) {
					return TRUE;
				}
				if (CheckPrintDataSub(ID_FORMNO_042) == TRUE) {
					return TRUE;
				}
				break;
			case ID_FORMNO_101:
			case ID_FORMNO_102:
				// ⑩-1 or ⑩-2
				if (CheckPrintDataSub(ID_FORMNO_101) == TRUE) {
					return TRUE;
				}
				if (CheckPrintDataSub(ID_FORMNO_102) == TRUE) {
					return TRUE;
				}
				break;
			case ID_FORMNO_141:
			case ID_FORMNO_142:
				// ⑭-1 or ⑭-2
				if (CheckPrintDataSub(ID_FORMNO_141) == TRUE) {
					return TRUE;
				}
				if (CheckPrintDataSub(ID_FORMNO_142) == TRUE) {
					return TRUE;
				}
				break;
			case ID_FORMNO_151:
			case ID_FORMNO_152:
			case ID_FORMNO_153:
				// ⑮-1 or ⑮-2 or ⑮-3
				if (CheckPrintDataSub(ID_FORMNO_151) == TRUE) {
					return TRUE;
				}
				if (CheckPrintDataSub(ID_FORMNO_152) == TRUE) {
					return TRUE;
				}
				if (CheckPrintDataSub(ID_FORMNO_153) == TRUE) {
					return TRUE;
				}
				break;
			case ID_FORMNO_161:
			case ID_FORMNO_162:
				// ⑯-1 or ⑯-2
				if (CheckPrintDataSub(ID_FORMNO_161) == TRUE) {
					return TRUE;
				}
				if (CheckPrintDataSub(ID_FORMNO_162) == TRUE) {
					return TRUE;
				}
				break;
			}
			return FALSE;
		}
	}
	catch (...) {
		if (rsFormType.IsOpen()) {
			rsFormType.Fin();
		}
		return FALSE;
	}
}

//**************************************************
//	本データ有無のチェック（サブ）
//	【引数】	nFormSeq		…	様式シーケンス番号
//	【戻値】	TRUE			…	本データ有り
//				FALSE			…	本データ無し
//**************************************************
BOOL CdlgPrintAll::CheckPrintDataSub( int nFormSeq )
{
	CdbUc000Common*	mfcCmn;
	BOOL  blnRet = FALSE;

	try{
		// 対象様式シーケンス番号に対応したDBクラスのポインタを取得
		switch( nFormSeq ){

		case SchSeqCvr:

			// 表紙のページ数は必ず「1」
			return TRUE;

		case ID_FORMNO_011:

			mfcCmn = (CdbUc000Common*)( new CdbUc011Yotyokin( m_pDB ) );
			break;

		case ID_FORMNO_021:

			mfcCmn = (CdbUc000Common*)( new CdbUc021Uketoritegata( m_pDB ) );
			break;

		case ID_FORMNO_031:

			mfcCmn = (CdbUc000Common*)( new CdbUc031Urikakekin( m_pDB ) );
			break;

		case ID_FORMNO_041:

			mfcCmn = (CdbUc000Common*)( new CdbUc041Karibarai( m_pDB ) );
			break;

		case ID_FORMNO_042:

			mfcCmn = (CdbUc000Common*)( new CdbUc042Kasituke( m_pDB ) );
			break;

		case ID_FORMNO_051:

			mfcCmn = (CdbUc000Common*)( new CdbUc051Tanaoroshi( m_pDB ) );
			break;

		case ID_FORMNO_061:

			mfcCmn = (CdbUc000Common*)( new CdbUc061Yuukasyouken( m_pDB ) );
			break;

		case ID_FORMNO_071:

			mfcCmn = (CdbUc000Common*)( new CdbUc071Koteishisan( m_pDB ) );
			break;

		case ID_FORMNO_081:

			mfcCmn = (CdbUc000Common*)( new CdbUc081Siharaitegata( m_pDB ) );
			break;

		case ID_FORMNO_091:

			mfcCmn = (CdbUc000Common*)( new CdbUc091Kaikakekin( m_pDB ) );
			break;

		case ID_FORMNO_101:

			mfcCmn = (CdbUc000Common*)( new CdbUc101Kariukekin( m_pDB ) );
			break;

		case ID_FORMNO_102:

			mfcCmn = (CdbUc000Common*)( new CdbUc102Gensen( m_pDB ) );
			break;

		case ID_FORMNO_111:

			mfcCmn = (CdbUc000Common*)( new CdbUc111Kariirekin( m_pDB ) );
			break;

		case ID_FORMNO_121:

			mfcCmn = (CdbUc000Common*)( new CdbUc121Totiuriage( m_pDB ) );
			break;

		case ID_FORMNO_131:

			mfcCmn = (CdbUc000Common*)( new CdbUc131Uriagedaka( m_pDB ) );
			break;

		case ID_FORMNO_141:

			mfcCmn = (CdbUc000Common*)( new CdbUc141Yakuin( m_pDB ) );
			break;

		case ID_FORMNO_142:

			mfcCmn = (CdbUc000Common*)( new CdbUc142Jinkenhi( m_pDB ) );
			break;

		case ID_FORMNO_151:

			mfcCmn = (CdbUc000Common*)( new CdbUc151Tidaiyatin( m_pDB ) );
			break;

		case ID_FORMNO_152:

			mfcCmn = (CdbUc000Common*)( new CdbUc152Kenrikin( m_pDB ) );
			break;

		case ID_FORMNO_153:

			mfcCmn = (CdbUc000Common*)( new CdbUc153Kougyou( m_pDB ) );
			break;

		case ID_FORMNO_161:

			mfcCmn = (CdbUc000Common*)( new CdbUc161Zatueki( m_pDB ) );
			break;

		case ID_FORMNO_162:

			mfcCmn = (CdbUc000Common*)( new CdbUc162Zatuson( m_pDB ) );
			break;

		case ID_FORMNO_171:
		case ID_FORMNO_172:
		case ID_FORMNO_173:
		case ID_FORMNO_174:
		case ID_FORMNO_175:
		case ID_FORMNO_176:
		case ID_FORMNO_177:
		case ID_FORMNO_178:
		case ID_FORMNO_179:
		case ID_FORMNO_1710:
		case ID_FORMNO_1711:
		case ID_FORMNO_1712:
		case ID_FORMNO_1713:
		case ID_FORMNO_1714:
		case ID_FORMNO_1715:
		case ID_FORMNO_1716:
		case ID_FORMNO_1717:
		case ID_FORMNO_1718:
		case ID_FORMNO_1719:
		case ID_FORMNO_1720:

			mfcCmn = (CdbUc000Common*)( new CdbUc171Sonota1( m_pDB, nFormSeq ) );
			break;

		case ID_FORMNO_181:
		case ID_FORMNO_182:
		case ID_FORMNO_183:
		case ID_FORMNO_184:
		case ID_FORMNO_185:
		case ID_FORMNO_186:
		case ID_FORMNO_187:
		case ID_FORMNO_188:
		case ID_FORMNO_189:
		case ID_FORMNO_1810:
		case ID_FORMNO_1811:
		case ID_FORMNO_1812:
		case ID_FORMNO_1813:
		case ID_FORMNO_1814:
		case ID_FORMNO_1815:
		case ID_FORMNO_1816:
		case ID_FORMNO_1817:
		case ID_FORMNO_1818:
		case ID_FORMNO_1819:
		case ID_FORMNO_1820:

			mfcCmn = (CdbUc000Common*)( new CdbUc181Sonota2( m_pDB, nFormSeq ) );
			break;

		default:
			return FUNCTION_NG;
		}

		// 2011/12/22 処理No22空ページ削除処理改善において
		// はい→一括印刷なし　いいえ→一括印刷ありの対応
		// 本データの有無をチェック
		//if ( mfcCmn->RequeryNextNotNullLine( 0 , 0 ) == DB_ERR_OK ){
		
		// レコードが1つでも存在する場合は戻り値をTRUEにする
		mfcCmn->RequeryPage( 1 );	// レコードが存在する場合、PageNumは必ず1以上である
		// データが1件以上あり?
		// 2012/07/20 保管データ(FgShow != 1)はカウント外とする
		//if ( mfcCmn->GetRecordCount() > 0 ){
		if ( mfcCmn->GetCountVisualDataRecord(1) > 0 ){
			blnRet = TRUE;
		}
		mfcCmn->Fin();
		//}
		delete mfcCmn;
	}
	catch( ... ){

		if( mfcCmn->IsOpen() ){
			mfcCmn->Fin();
			delete mfcCmn;
		}
	}

	return blnRet;
}

// midori 160913 cor -->
//// 一括印刷以外の初期チェック状態
//void CdlgPrintAll::SetOutputChk(int& focus_row)
//{
//	if(m_CheckNum == -1)	return;
//	else if(m_CheckNum == 0){
//		SetAllOutputSpecify();
//		return;
//	}
//
//	// 様式シーケンスからFormNoを探索
//	int row = 0;
//	vector<PRINT_ALL_DATA>::iterator it;
//	for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
//		if(it->FormSeq == m_CheckNum)			break;
//	}	
//	if(it == m_vecPrintAll.end())	return;
//
//	// FormNoからそのFormNoの－１を検索
//	map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(it->FormNo);
//	if(it2 == mapFormType.end()){	// －１、－２がない様式
//		// 出力ページなしならチェックしない
//		if(it->isEnable == FALSE)	return;
//
//		m_report.SetChecked(row, pa_select, TRUE);
//		focus_row = row;
//		return;
//	}
//
//	row = 0;
//	if(it2->second.FormType == ID_FORMTYPE_KOBETU){	// m_CheckNumのレコードをチェック
//		for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
//			if(it->FormSeq == m_CheckNum)	break;
//			// インクリメント
//			row++;
//		}
//	}
//	else{		// m_CheckNumの所属する－１の様式シーケンスのレコードをチェック
//		for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
//			if(it->FormSeq == it2->second.FormSeq)		break;
//			// インクリメント
//			row++;
//		}
//	}
//
//	// 出力ページなしならチェックしない
//	if(it->isEnable == FALSE)	return;
//
//	m_report.SetChecked(row, pa_select, TRUE);
//	focus_row = row;
//}
// ---------------------
// 一括印刷以外の初期チェック状態
void CdlgPrintAll::SetOutputChk(int& focus_row)
{
	int		ii=0;
	int		child_num=0;

	if(m_CheckNum == -1)	return;
	else if(m_CheckNum == 0){
		SetAllOutputSpecify();
		return;
	}

	// 様式シーケンスからFormNoを探索
	int row = 0;
	vector<PRINT_ALL_DATA>::iterator it;
	for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		if(it->FormSeq == m_CheckNum)			break;
	}	
	if(it == m_vecPrintAll.end())	return;

	// FormNoからそのFormNoの－１を検索
	map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(it->FormNo);
	if(it2 == mapFormType.end()){	// －１、－２がない様式
		// 出力ページなしならチェックしない
		if(it->isEnable != FALSE)	{
			m_report.SetChecked(row, pa_select, TRUE);
		}
		focus_row = row;
		return;
	}

	row = 0;
	if(it2->second.FormType == ID_FORMTYPE_KOBETU){	// m_CheckNumのレコードをチェック
		for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
			if(it->FormSeq == m_CheckNum)	break;
			// インクリメント
			row++;
		}
	}
	else{		// m_CheckNumの所属する－１の様式シーケンスのレコードをチェック
		for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
			if(it->FormSeq == it2->second.FormSeq)		break;
			// インクリメント
			row++;
		}
	}

	// 出力ページなしならチェックしない
	if(it->isEnable != FALSE)	{
		m_report.SetChecked(row, pa_select, TRUE);
		if(it2->second.FormType != ID_FORMTYPE_KOBETU)	{
			// 通常タイプの場合、－①がONになったら、－②（－③）もチェックをつける
			child_num = it2->second.num - 1;
			for(ii=0; ii<child_num; ii++)	{
				m_report.SetChecked((row + ii + 1), pa_select, TRUE);
			}
		}
	}
	focus_row = row;
}
// midori 160913 cor <--

// midori 152393 add -->
//**************************************************
// 印刷設定、科目設定でデータに影響のある項目が変更された場合、
// 呼び出した様式のチェックを付ける
//**************************************************
void CdlgPrintAll::SetOutputChk2(BOOL isYousiki[])
{
	int		ii=0;
	int		child_num=0;

	// 一括印刷または印刷押下時
	if(m_CheckNum == -1 || m_CheckNum == 0)	return;

	// 様式シーケンスからFormNoを探索
	int row = 0;
	vector<PRINT_ALL_DATA>::iterator it;
	for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		if(it->FormSeq == m_CheckNum)			break;
	}	
	if(it == m_vecPrintAll.end())	return;

	// FormNoからそのFormNoの－１を検索
	map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(it->FormNo);
	if(it2 == mapFormType.end()){	// －１、－２がない様式
		// 出力ページなしならチェックしない
		// 印刷、科目ダイアログで変更がない様式の場合チェックしない
		if(it->isEnable != FALSE && isYousiki[row] == TRUE)	{
			m_report.SetChecked(row, pa_select, TRUE);
		}
		return;
	}

	row = 0;
	if(it2->second.FormType == ID_FORMTYPE_KOBETU){	// m_CheckNumのレコードをチェック
		for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
			if(it->FormSeq == m_CheckNum)	break;
			// インクリメント
			row++;
		}
	}
	else{		// m_CheckNumの所属する－１の様式シーケンスのレコードをチェック
		for(it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
			if(it->FormSeq == it2->second.FormSeq)		break;
			// インクリメント
			row++;
		}
	}

	// 出力ページなしならチェックしない
	// 印刷、科目ダイアログで変更がない様式の場合チェックしない
	if(it->isEnable != FALSE && isYousiki[row] == TRUE)	{
		m_report.SetChecked(row, pa_select, TRUE);
		if(it2->second.FormType != ID_FORMTYPE_KOBETU)	{
			// 通常タイプの場合、－①がONになったら、－②（－③）もチェックをつける
			child_num = it2->second.num - 1;
			for(ii=0; ii<child_num; ii++)	{
				m_report.SetChecked((row + ii + 1), pa_select, TRUE);
			}
		}
	}
}
// midori 152393 add <--

//**************************************************
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgPrintAll::InitReportHeader()
{
	//m_report.Resize( TRUE, ICSResizeDefault );
	
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
// midori 160610 cor -->
//	m_report.AddColumn(new ICSReportColumn(0, NULL, 1));
//	m_report.AddColumn(new ICSReportColumn(1, NULL, 2));
//	m_report.AddColumn(new ICSReportColumn(2, NULL, 8));
//	m_report.AddColumn(new ICSReportColumn(3, NULL, 1));
//	m_report.AddColumn(new ICSReportColumn(4, NULL, 1));
// ---------------------
	m_report.AddColumn(new ICSReportColumn(0, NULL, 10));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 20));
	m_report.AddColumn(new ICSReportColumn(2, NULL, 80));
	m_report.AddColumn(new ICSReportColumn(3, NULL, 11));
	m_report.AddColumn(new ICSReportColumn(4, NULL, 11));
	m_report.AddColumn(new ICSReportColumn(5, NULL, 13));
// midori 160610 cor <--

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	ICSReportColumns *m_pClms = m_report.GetColumns();
	int cnt = sizeof(strPrintAllHeader) / sizeof(strPrintAllHeader[0]);
	for(int i = 0; i < cnt; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		m_pClm->SetCaption(strPrintAllHeader[i]);
	}

	// フォント設定
	CFont*		pFont;
	LOGFONT		font = {0};

	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_report.SetTextFont(font);
	m_report.SetHeaderFont(font);
}

//**************************************************
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	TRUE:成功
//				FALSE:失敗
//**************************************************
void CdlgPrintAll::AddRecords()
{
// midori 160610 add -->
	CString		strKs=_T("");
// midori 160610 add <--

	m_report.RemoveAllRecords();

	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
		ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 選択
		pRecord->AddItem( new ICSReportRecordItemText(it->TitleNo));	// 様式番号
		pRecord->AddItem( new ICSReportRecordItemText(it->Title));		// 様式名称
		CString start = it->str_start;
		CString end = it->str_end;
		if(it->isEnable == FALSE){
			start.Empty();
			end.Empty();
		}
// midori 156949 add -->
// midori 20/10/08u del -->
		//if(it->SortChk == 1)	{
		//	start.Format(_T("-"));
		//	end.Format(_T("-"));
		//}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		if(it->isMainasu == 1)	{
			start.Format(_T("-"));
			end.Format(_T("-"));
		}
// midori 20/10/08u add <--
// midori 156949 add <--
		pRecord->AddItem( new ICSReportRecordItemText(start, ICS_EDS_NUMERIC, TRUE, 1, 4, ICS_EDS_RIGHT));		// 開始ページ　最大4桁入力
		pRecord->AddItem( new ICSReportRecordItemText(end, ICS_EDS_NUMERIC, TRUE, 1, 4, ICS_EDS_RIGHT));		// 終了ページ　最大4桁入力
// midori 160610 add -->
		// 科目指定
		if(m_chkZero.GetCheck() == 1)	{
			strKs=_T("");
		}
		else	{
			if(it->KamokuSitei != 0)	strKs=_T("*");
			else						strKs=_T("");
		}
		pRecord->AddItem( new ICSReportRecordItemText(strKs,ICS_EDS_ASCII,FALSE,FALSE,0,ICS_EDS_CENTER));		// 科目指定
// midori 160610 add <--
	}

	// ここ以降でないと、効かない
	m_report.AllowEdit(TRUE);								// 全て編集許可にしたあと
	m_report.AllowEditColumn(pa_youshiki_no, FALSE);		// 様式番号と
	m_report.AllowEditColumn(pa_youshiki_name, FALSE);		// 様式名称はEdit不可に
// midori 160610 add -->
	m_report.AllowEditColumn(pa_kamoku_sitei, FALSE);		// 科目指定もEdit不可に
// midori 160610 add <--

	// 行の高さ
	m_report.SetItemHeight(20);

	m_report.Populate();
}

//**************************************************
//	レポートコントロールのレコードに付随するデータのベクタ作成
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgPrintAll::CreatePrintAllVec()
{
// midori 190505 add -->
	int		cnt=0;
	int		nSortChk = 0;
	int		nPHoho = 0;
// midori 190505 add <--
// midori 160608 add -->
	int		nPageMax = 0;
	int		nOutZero = 0;
	BOOL	svEnable = FALSE;
// midori 160608 add <--

	CdbUcInfSub	mfcRec(m_pDB);
	CdbUcInfFormType mfcFormType(m_pDB);

	mfcRec.InitOrder();
	mfcRec.MoveFirst();

	m_vecFormOrder.clear();
	m_vecPrintAll.clear();
	mapFormType.clear();

// midori 190505 add -->
	cnt = 0;
// midori 190505 add <--
	while(!mfcRec.IsEOF()){
		// FormSeqベクタ作成
		m_vecFormOrder.push_back(mfcRec.m_FormSeq);

		// 出力形式マップ作成
		BOOL isEnable = CreateFormTypeMap(mfcFormType, mfcRec);
// midori 160608 add -->
		svEnable = isEnable;
// midori 160608 add <--

// midori 20/10/08_2 add -->
		if(prnConfimationSw == TRUE) {
// midori 20/10/08_2 add <--

// midori 190505 add -->
			nSortChk = ChkIkkatuSort(m_pDB, &mfcRec);

			// 出力方法
			nPHoho = 0;
			CCtrlSubGeneralVar	cs(mfcRec);
// midori 20/10/08u del -->
			//if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE && cs.bPrnHoho3 == FALSE)	{
			//	// 新規起動時
			//	// ソートダイアログの「改頁を行う」チェックがONの時
			//	if(mfcRec.m_OpKaiPage == 1)	nPHoho = 2;
// midori 20/10/08u del <--
// midori 20/10/08u add -->
			if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE)	{
				// 新規起動時
				// ソートダイアログの「改頁を行う」チェックがONの時
				if(mfcRec.m_OpKaiPage == 1)	nPHoho = 1;
// midori 20/10/08u add <--
				else						nPHoho = 0;
			}
			else	{
				if(cs.bPrnHoho1)		nPHoho = 0;
				else if(cs.bPrnHoho2)	nPHoho = 1;
// midori 20/10/08u del -->
				//else if(cs.bPrnHoho3)	nPHoho = 2;
// midori 20/10/08u del <--
			}

			if(m_pBatchPrt->IsBatchExec() == TRUE)	{
				// 共通：一括印刷アプリから起動
				int nbuf = m_pOwntbData->m_uInfo.uBPrtCmn.nOPPrnHoho[cnt];
// midori 20/10/08u del -->
				//if(nbuf == 0 || nbuf == 1 || nbuf == 2)	{
// midori 20/10/08u del <--
// midori 20/10/08u add -->
				if(nbuf == 0 || nbuf == 1)	{
// midori 20/10/08u add <--
					nPHoho = nbuf;
				}
			}

// midori 20/10/08u del -->
			//if(nSortChk == 0 && nPHoho == 2)	{
// midori 20/10/08u del <--
// midori 20/10/08u add -->
			if(nSortChk == 0 && nPHoho != 0)	{
// midori 20/10/08u add <--
				nPHoho = 0;
			}
// midori 190505 add <--
// midori 20/10/08_2 add -->
		}
// midori 20/10/08_2 add <--

		CprtJobFunction prtJob;
		// 最大ページ数取得
// midori 190505 del -->
		//int nPageMax = prtJob.GetDbDataPageAll( mfcRec.m_FormSeq, m_pDB, prtJob.GetOutZero(mfcRec) );
// midori 190505 del <--
// midori 190505 add -->
		int nPageMax = prtJob.GetDbDataPageAll(mfcRec.m_FormSeq, m_pDB, prtJob.GetOutZero(mfcRec), -1, 0, nPHoho);
// midori 190505 add <--
		// 欄外登録の複数ページ対応により、欄外登録のページ数も考慮する
// midori 154982 del -->
		//if((mfcRec.m_FormSeq == ID_FORMNO_051) || (mfcRec.m_FormSeq == ID_FORMNO_091)){
// midori 154982 del <--
// midori 154982 add -->
		if((mfcRec.m_FormSeq == ID_FORMNO_051 && bG_Kanso != TRUE) || (mfcRec.m_FormSeq == ID_FORMNO_091)){
// midori 154982 add <--
// midori 155525 del -->
			//int nPageMaxRangai = GetRangaiPage(mfcRec.m_FormSeq);
// midori 155525 del <--
// midori 155525 add -->
			int nPageMaxRangai = GetRangaiPage(mfcRec.m_FormSeq,prtJob.GetOutZero(mfcRec));
// midori 155525 add <--
			if(nPageMaxRangai > nPageMax)	nPageMax = nPageMaxRangai;
		}

		// データがない様式は選択不可
		if(nPageMax == 0){
			isEnable = FALSE;
		}

// midori 20/10/08u del -->
//// midori 190505 del -->
////// midori 160608 cor -->
//////		PRINT_ALL_DATA	pa_data(isEnable, mfcRec, 1, nPageMax);
////// ---------------------
////		PRINT_ALL_DATA	pa_data(isEnable, mfcRec, 1, nPageMax, svEnable);
////// midori 160608 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		PRINT_ALL_DATA	pa_data(isEnable, mfcRec, 1, nPageMax, svEnable, nSortChk, nPHoho);
//// midori 190505 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		PRINT_ALL_DATA	pa_data(isEnable, mfcRec, 1, nPageMax, svEnable, nSortChk, nPHoho, nSortChk);
// midori 20/10/08u add <--
		// ベクタの最後尾に追加
		m_vecPrintAll.push_back(pa_data);
		// 次の様式へ
		mfcRec.MoveNext();
// midori 190505 add -->
		cnt++;
// midori 190505 add <--
	}

	mfcRec.Fin();
}

BOOL CdlgPrintAll::CreateFormTypeMap(CdbUcInfFormType& mfcFormType, CdbUcInfSub& mfcSub)
{
	// 出力形式を取得
	mfcFormType.Init(mfcSub.m_FormNo);
	if(mfcFormType.IsEOF()){
		mfcFormType.Fin();
		return TRUE;
	}

	// 登録済みなら何もしない
	map<int, FORM_TYPE_DATA>::iterator it = mapFormType.find(mfcSub.m_FormNo);
	if(mfcFormType.m_FormType == ID_FORMTYPE_NORMAL){
		if(it != mapFormType.end())		return FALSE;
	}

	FORM_TYPE_DATA ftd;

	ftd.FormSeq = mfcSub.m_FormSeq;
	ftd.FormType = mfcFormType.m_FormType;

	if(mfcSub.m_FormSeq == ID_FORMNO_151)	ftd.num = 3;
	else									ftd.num = 2;

	mapFormType[mfcSub.m_FormNo] = ftd;

	mfcFormType.Fin();

	return TRUE;
}

//********************************************************************************
//	－1、－２（－３）の全ての様式がない場合はレコードベクタから削除する
//　一つでもあれば、その中で最大のものを－１の最大ページ数にセットする
//　－１－２はuc_inf_form_typeに登録されているもので、その他の内訳書は対象外
//	【引数】	なし
//	【戻値】	なし
//********************************************************************************
void CdlgPrintAll::UpdateFormTypePage()
{	
	for(map<int, FORM_TYPE_DATA>::iterator it = mapFormType.begin(); it != mapFormType.end(); it++){
		// 出力形式が個別は何もしない
		if(it->second.FormType == ID_FORMTYPE_KOBETU)		continue;

		// レコードベクタから一致する様式を取得
		int max_no	= 0;
		int max_page = 0;
		for(int i=0; i<it->second.num; i++){
			int seq = it->second.FormSeq;
			seq += i;
			// 最大ページ数を探索
			vector<PRINT_ALL_DATA>::iterator it2 = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), seq);
			if(it2 != m_vecPrintAll.end()){
				if(max_page < it2->end){
					max_no = it2->FormNo;
					max_page = it2->end;
				}
			}
		}
		// 全て空ページならレコードベクタから削除
		if(max_page > 0){
			// 全て空ページでないなら、最大値を－１にセット
			int max_seq = 0;
			// mapFormTypeのFormNoから－1のFormSeqを探索
			map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(max_no);
			if(it2 != mapFormType.end()){
				max_seq = it2->second.FormSeq;
			}
			// 見つかった－１のFormSeqをキーにレコードベクタの最大値を更新
			vector<PRINT_ALL_DATA>::iterator it3 = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), max_seq);
			if(it3 != m_vecPrintAll.end()){
				it3->end = max_page;
				it3->str_end.Format("%d", it3->end);
// midori 20/10/08u add -->
				it3->inputEnd = it3->end;
// midori 20/10/08u add <--
// midori M-16113010 add -->
				it3->svend = it3->end;
// midori M-16113010 add <--
				if(it3->isEnable == FALSE){	// －１のデータがなく、－２のデータによりEnabelにされた
					it3->isEnable = TRUE;
					it3->start = 1;
					it3->str_start.Format("%d", it3->start);
// midori 20/10/08u add -->
					it3->inputStart = it3->start;
// midori 20/10/08u add <--
// midori M-16113010 add -->
					it3->svstart = it3->start;
// midori M-16113010 add <--
				}
			}
		}
// midori 20/10/08u add -->
		// レコードベクタから一致する様式を取得
		// ⑯－１、⑯－２様式において、
		// 「出力形式」が通常のとき
		// －２、－３がソート確認ダイアログに表示される場合、
		// －１が確認ダイアログに表示されなくても、－１の開始頁、終了頁に「-」を表示して入力を無効にする
		int nsortchk = 0;
		for(int i=0; i<it->second.num; i++){
			int seq = it->second.FormSeq;
			seq += i;
			if(seq == ID_FORMNO_162) {
				// －２、－３のデータを取得
				vector<PRINT_ALL_DATA>::iterator it2 = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), seq);
				if(it2 != m_vecPrintAll.end()){
					nsortchk = it2->SortChk;
				}
				if(nsortchk != 0) {
					// mapFormTypeのFormNoから－1のFormSeqを探索
					map<int, FORM_TYPE_DATA>::iterator it3 = mapFormType.find(it2->FormNo);
					if(it3 != mapFormType.end()){
						// 見つかった－１のFormSeqをキーにレコードベクタのSortChkを更新
						vector<PRINT_ALL_DATA>::iterator it4 = find(m_vecPrintAll.begin(), m_vecPrintAll.end(), it3->second.FormSeq);
						if(it4 != m_vecPrintAll.end()){
							// －１のデータをマイナス表示にする
							it4->isMainasu = 1;
							it4->isMainasu2 = 1;
							// －２のデータのマイナス表示をクリア
							it2->isMainasu = 0;
							it2->isMainasu2 = 0;
						}
					}
				}
				break;
			}
		}
// midori 20/10/08u add <--
	}
}
void CdlgPrintAll::OnBnClickedButtonHyoAll()
{
	m_clsFunc.ChangeReportColumnAll(&m_report, pa_select);

	BOOL state = m_report.IsChecked(0, pa_select);
	m_chkTitle.SetCheck(state);
	m_chkNow.EnableWindow(state);

	UpdateRecordState();
}

void CdlgPrintAll::UpdateRecordState()
{
// midori 151393,M-16113006 add -->
	BOOL	sgn=FALSE;
// midori 151393,M-16113006 add <--
	int row = 0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++){
		// 選択カラムがOFFのレコードはEdit不可
// midori 151393,M-16113006 add -->
		// レコードが入力不可の場合チェックをOFFにする
		map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(it->FormNo);
		if(it2 == mapFormType.end()){
			sgn = FALSE;
			if(it->isEnable == FALSE)	m_report.SetChecked(row, pa_select, FALSE);
		}
		else	{
			if(it2->second.FormType == ID_FORMTYPE_KOBETU)	{
				if(it->isEnable == FALSE)	m_report.SetChecked(row, pa_select, FALSE);
			}
			else	{
				if(it->FormNo2 == 1)	{
					if(it->isEnable == FALSE)	m_report.SetChecked(row, pa_select, FALSE);
					sgn = m_report.IsChecked(row,pa_select);
				}
				else					{
					m_report.SetChecked(row, pa_select, sgn);
				}
			}
		}
// midori 151393,M-16113006 add <--

		BOOL check = m_report.IsChecked(row, pa_select);
// midori 20/10/08u del -->
//// midori 156910 add -->
//		// 確認ダイアログに表示する様式の場合は、開始頁と終了頁もEdit不可に
//		if(it->SortChk == 1)	check = FALSE;
//// midori 156910 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		// 確認ダイアログに表示する様式の場合は、開始頁と終了頁もEdit不可に
		if(it->isMainasu == 1)	check = FALSE;
// midori 20/10/08u add <--
		m_report.AllowEditItem(row, pa_start, check);
		m_report.AllowEditItem(row, pa_end, check);

// midori 156910 add -->
		BOOL state_pg = TRUE;
		// 開始頁、終了頁の背景色を変更
// midori 20/10/08u del -->
		//if(it->SortChk != 0)	{
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		if(it->isMainasu == 1)	{
// midori 20/10/08u add <--
			state_pg = FALSE;
		}
		SetColorItem(row, pa_start, state_pg);
		SetColorItem(row, pa_end, state_pg);
// midori 156910 add <--

		BOOL state = FALSE;
		if(it->isEnable != FALSE)	state = TRUE;
		// 出力形式が通常の－２－３は常にEdit不可
		if(state == FALSE){
			m_report.AllowEditRow(row,  state);
			SetColor(row, state);
		}

		row++;
	}
}
void CdlgPrintAll::OnNMReportCheckedCUSTOM_REPORT_PRINTALL(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//チェックされたカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//チェックされた行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//チェックされたアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();
	BOOL	bCheck	=	pItem->IsChecked();	//チェック状況
// midori 190505 add -->
	int		iSort	=	0;
// midori 190505 add <--
// midori 190505 del -->
	//SetCheckBoxState(indexRow, bCheck);
// midori 190505 del <--
// midori 190505 add -->
// midori 20/10/08u del -->
	//iSort = m_vecPrintAll[indexRow].SortChk;
// midori 20/10/08u del <--
// midori 20/10/08u add -->
	iSort = m_vecPrintAll[indexRow].isMainasu;
// midori 20/10/08u add <--

	SetCheckBoxState(indexRow, bCheck, iSort);
// midori 190505 add <--

	m_report.RedrawControl();
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				check	…	選択チェックボックスの状態
//	【戻値】	なし
//**************************************************
void CdlgPrintAll::SetColor( int nIndex, BOOL check)
{
	if(check != FALSE)		m_report.SetRowBackColor(nIndex, GetSysColor( COLOR_WINDOW ), FALSE);
	else					m_report.SetRowBackColor(nIndex, GetSysColor( COLOR_BTNFACE ), FALSE);
}

// midori 156910 add -->
//**************************************************
//	背景色を設定（１カラム）
//	【引数】	nIndex	…	インデックス
//				check	…	選択チェックボックスの状態
//	【戻値】	なし
//**************************************************
void CdlgPrintAll::SetColorItem(int nIndex, int nColumn, BOOL check)
{
	if(check != FALSE)		m_report.SetCellBackColor(nIndex, nColumn, GetSysColor( COLOR_WINDOW ), FALSE);
	else					m_report.SetCellBackColor(nIndex, nColumn, GetSysColor( COLOR_BTNFACE ), FALSE);
}
// midori 156910 add <--

void CdlgPrintAll::OnNMReportEditkeydownCUSTOM_REPORT_PRINTALL(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();
	//ターミネーションキーコード
	UINT	nChar	=	pItemNotify->nChar;

	UpdateStateFromKey(nChar);
}

void CdlgPrintAll::UpdateStateFromKey(int key)
{
	ICSReportSelectedRows* s_rows = m_report.GetSelectedRows();
	if(s_rows == NULL)	return;
	ICSReportRow* r_row = s_rows->GetAt(0);
	if(r_row == NULL)	return;
	int indexRow = r_row->GetIndex();
	BOOL check = m_report.IsChecked(indexRow, pa_select);
	ICSReportRecordItem	*pItem	= m_report.GetActiveItem();
// midori 190505 add -->
	int		isort=0;
// midori 190505 add <--

	switch(key){
		case VK_SPACE:
			// 背景が灰色のレコードに対してはチェック入れない
			if(m_vecPrintAll[indexRow].isEnable == FALSE)	return;

			// 選択カラムの状態反転
			check = !check;
// midori 20/10/08u del -->
//// midori 190505 add -->
//			// サインを取得
//			isort = m_vecPrintAll[indexRow].SortChk;
//// midori 190505 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
			// サインを取得
			isort = m_vecPrintAll[indexRow].isMainasu;
// midori 20/10/08u add <--

			if(pItem == NULL){
				m_report.SetChecked(indexRow, pa_select, check);

				// チェックボックス更新
// midori 190505 del -->
				//SetCheckBoxState(indexRow, check);
// midori 190505 del <--
// midori 190505 add -->
				SetCheckBoxState(indexRow, check, isort);
// midori 190505 add <--
				// 背景色更新
				m_report.RedrawControl();
			}
			else{
				int indexClm = pItem->GetIndex();
				if(indexClm == pa_select){
					m_report.SetChecked(indexRow, pa_select, check);
					// チェックボックス更新
// midori 190505 del -->
					//SetCheckBoxState(indexRow, check);
// midori 190505 del <--
// midori 190505 add -->
					SetCheckBoxState(indexRow, check, isort);
// midori 190505 add <--
					// 背景色更新
					m_report.RedrawControl();
				}
				else if(indexClm == pa_start || indexClm == pa_end){
					BOOL check2 = !m_report.IsChecked(indexRow, indexClm);
					m_report.SetChecked(indexRow, indexClm, check2);

					m_report.AllowEditItem( indexRow, indexClm, TRUE );	// この3行を入れないとVK_SPACE後Editが抜けてしまう
					m_report.Populate();
					m_report.EditItem( indexRow, indexClm, TRUE );
				}
			}
			break;

		case VK_RETURN:
			if(check == FALSE){
				m_report.MoveDown( FALSE, FALSE );
				return;
			}
			
// midori 156910 del -->
			//if(pItem != NULL){	// エディットが入っている→次のカラムにエディット
			//	int indexClm = pItem->GetIndex();
			//	// 入力された値が1～最大ページ数かチェック　→EditKillFocusで呼んでいるのでコメントアウト
			//	//CheckPageNum(indexRow, indexClm);

			//	// 次のカラムに移動
			//	if(indexClm == pa_select){
			//		indexClm = pa_start;
			//		m_report.EditItem(indexRow, indexClm, TRUE);
			//	}
			//	else if(indexClm == pa_start){
			//		indexClm = pa_end;
			//		m_report.EditItem(indexRow, indexClm, TRUE);
			//	}
			//	else if(indexClm == pa_end){	// 終了ペーじカラムなら次の行にエディット
			//		m_report.MoveDown( FALSE, FALSE );

			//		ICSReportRecords * records = m_report.GetRecords();
			//		int max_cnt = records->GetCount();

			//		if(indexRow+1<max_cnt){
			//			// 背景が灰色のレコードに対してはEdit入れない
			//			if(m_vecPrintAll[indexRow + 1].isEnable == FALSE)	return;

			//			m_report.EditItem( indexRow + 1, zm_select, TRUE );
			//		}
			//	}
			//}
			//else{	// エディットが入っていない→選択カラムにエディット
			//	if(m_vecPrintAll[indexRow].isEnable == FALSE){
			//		m_report.MoveDown( FALSE, FALSE );
			//		return;
			//	}
			//	m_report.EditItem( indexRow, pa_start, TRUE );
			//}
// midori 156910 del <--
// midori 156910 add -->
			if(pItem != NULL){	// エディットが入っている→次のカラムにエディット
				int indexClm = pItem->GetIndex();
				// 次のカラムに移動
				if(indexClm == pa_select)	{
// midori 20/10/08u del -->
					//if(m_vecPrintAll[indexRow].SortChk == 1)	{
// midori 20/10/08u del <--
// midori 20/10/08u add -->
					if(m_vecPrintAll[indexRow].isMainasu == 1)	{
// midori 20/10/08u add <--
						// 確認ダイアログに表示する様式の場合は、次の行にエディット
						m_report.MoveDown( FALSE, FALSE );
						ICSReportRecords * records = m_report.GetRecords();
						int max_cnt = records->GetCount();
						if(indexRow+1<max_cnt){
							// 背景が灰色のレコードに対してはEdit入れない
							if(m_vecPrintAll[indexRow + 1].isEnable == FALSE)	return;
							m_report.EditItem(indexRow + 1, zm_select, TRUE);
						}
					}
					else	{
						indexClm = pa_start;
						m_report.EditItem(indexRow, indexClm, TRUE);
					}
				}
				else if(indexClm == pa_start){
					indexClm = pa_end;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
				else if(indexClm == pa_end){	// 終了ペーじカラムなら次の行にエディット
					m_report.MoveDown( FALSE, FALSE );

					ICSReportRecords * records = m_report.GetRecords();
					int max_cnt = records->GetCount();

					if(indexRow+1<max_cnt){
						// 背景が灰色のレコードに対してはEdit入れない
						if(m_vecPrintAll[indexRow + 1].isEnable == FALSE)	return;

						m_report.EditItem( indexRow + 1, zm_select, TRUE );
					}
				}
			}
			else{	// エディットが入っていない→選択カラムにエディット
				if(m_vecPrintAll[indexRow].isEnable == FALSE)	{
					m_report.MoveDown( FALSE, FALSE );
					return;
				}
				m_report.EditItem( indexRow, pa_select, TRUE );
			}

// midori 156910 add <--
			break;

		case VK_TAB:
			char	cShift =( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得
			//	SHIFTキーが押されてる？
			if ( cShift & 0x80 )	PrevDlgCtrl();
			else					NextDlgCtrl();
			break;
	}
}

// midori 190505 del -->
//void CdlgPrintAll::SetCheckBoxState(int indexRow, BOOL state)
// midori 190505 del <--
// midori 190505 add -->
void CdlgPrintAll::SetCheckBoxState(int indexRow, BOOL state, int state2)
// midori 190505 add <--
{
// midori 156910 del -->
//	m_report.AllowEditItem(indexRow, pa_start, state);
//	m_report.AllowEditItem(indexRow, pa_end, state);
// midori 156910 del <--
// midori 156910 add -->
	BOOL	edit=FALSE;
	BOOL	edit2=FALSE;

	edit = TRUE;
	if(state == FALSE)		edit = FALSE;
	else if(state2 != 0)	edit = FALSE;
	m_report.AllowEditItem(indexRow, pa_start, edit);
	m_report.AllowEditItem(indexRow, pa_end, edit);
// midori 156910 add <--

	// －①がONになったら、－②（－③）もチェックをつける
	PRINT_ALL_DATA pad = m_vecPrintAll.at(indexRow);
	map<int, FORM_TYPE_DATA>::iterator it = mapFormType.find(pad.FormNo);
	if(it == mapFormType.end())		return;

	if((*it).second.FormType == ID_FORMTYPE_KOBETU)	return;	// 「個別」の時は、独立しているので－②はチェックしない

	int child_num = ((*it).second.num - 1);
	for(int i=0; i<child_num; i++){
		m_report.SetChecked((indexRow + i + 1), pa_select, state);
	}
}

int CdlgPrintAll::GetCheckedNum()
{
	int row = 0;
	int cnt_selected = 0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		if(it->isEnable == FALSE)	continue;

		//BOOL check = m_report.IsChecked(row, pa_select);
		BOOL check = it->isSelect;
		if(check != FALSE)	cnt_selected++;

// midori 20/10/08u del -->
//// midori 156949 add -->
//		// 確認ダイアログに上げる対象の様式の場合、頁の範囲チェックはここでは行わない
//		if(it->SortChk == 1)		continue;
//// midori 156949 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		// 確認ダイアログに上げる対象の様式の場合、頁の範囲チェックはここでは行わない
		if(it->isMainasu == 1)		continue;
// midori 20/10/08u add <--

		// 最後にもう一度各ページの開始ページ、終了ページが範囲内かチェックする
		if(CheckPageNum(row, pa_start) == FUNCTION_NG)	return FUNCTION_NG;
		if(CheckPageNum(row, pa_end) == FUNCTION_NG)	return FUNCTION_NG;
	}
	
	// 表紙を出力する
	if(m_chkTitle.GetCheck())	cnt_selected++;

	return cnt_selected;
}

//**************************************************
// 入力されたページ数が1～最大ページ数かどうかチェック
// 異なっていれば開始 = 1　終了 = 最大ページをセットする
//	【引数】	indexRow	…	現在の行
//				indexClm	…	現在のカラム
//	【戻値】	ページ数
//**************************************************
// midori M-16082403 cor -->
//int CdlgPrintAll::CheckPageNum(int indexRow, int indexClm)
//{
//	CString  strPage = m_report.GetCaption(indexRow, indexClm);
//	int	nPage = atoi(strPage);
//
//	if(nPage < 1 || nPage > m_vecPrintAll[indexRow].end){
//		// 開始
//		//if(indexClm == pa_start)		m_report.SetCaption(indexRow, indexClm, "1");
//		if(indexClm == pa_start)		m_report.SetCaption(indexRow, indexClm, m_vecPrintAll[indexRow].str_start);
//		else if(indexClm == pa_end)		m_report.SetCaption(indexRow, indexClm, m_vecPrintAll[indexRow].str_end);
//		m_report.RedrawControl();
//
//		return FUNCTION_NG;
//	}
//	else{	// 1～3頁等で、01や03が許可されてしまう為、ここで頭の0を払う
//		CString page;
//		page.Format("%d", nPage);
//		m_report.SetCaption(indexRow, indexClm, page);
//	}
//
//	return nPage;
//}
// -------------------------
int CdlgPrintAll::CheckPageNum(int indexRow, int indexClm)
{
	CString		strPage = m_report.GetCaption(indexRow,indexClm);
	CString		page=_T("");
	int			nPage = atoi(strPage);
	int			nPage2 = 0;
	int			nCheck = 0;

	// 開始頁・終了とも
	if(nPage < 1 || nPage > m_vecPrintAll[indexRow].end)	{
		nCheck = 1;
	}
	else	{
		// 開始頁
		if(indexClm == pa_start)	{
			strPage = m_report.GetCaption(indexRow,pa_end);
			nPage2 = atoi(strPage);
			if(nPage > nPage2)	{
				//nCheck = 1;
				// 最大頁は超えていないが、終了頁を超えている場合
				// 終了頁に開始頁をセットする
				m_vecPrintAll[indexRow].svend = nPage;
				page.Format("%d",nPage);
				m_report.SetCaption(indexRow,pa_end,page);
				m_report.RedrawControl();
			}
		}
		// 終了頁
		if(indexClm == pa_end)	{
			strPage = m_report.GetCaption(indexRow,pa_start);
			nPage2 = atoi(strPage);
			if(nPage < nPage2)	nCheck = 1;
		}
	}

	if(nCheck)	{
		// 開始
		if(indexClm == pa_start)	{
			//m_report.SetCaption(indexRow, indexClm, m_vecPrintAll[indexRow].str_start);
			strPage.Format(_T("%d"),m_vecPrintAll[indexRow].svstart);
			m_report.SetCaption(indexRow, indexClm, strPage);
		}
		else if(indexClm == pa_end)	{
			// 元に戻すとき、デフォルト値が「開始」より小さくなっていれば
			// 「開始」と同じ頁数とする(※「帳票イメージのみ出力する」にチェックを付けた場合に起こり得る)
			strPage.Format(_T("%d"),m_vecPrintAll[indexRow].svend);
			m_report.SetCaption(indexRow,indexClm,strPage);
		}
		m_report.RedrawControl();

		return FUNCTION_NG;
	}
	else{	// 1～3頁等で、01や03が許可されてしまう為、ここで頭の0を払う
		page.Format("%d", nPage);
		m_report.SetCaption(indexRow, indexClm, page);
	}

	return nPage;
}
// midori M-16082403 cor <--

void CdlgPrintAll::OnNMReportEditkillfocusCUSTOM_REPORT_PRINTALL(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();
// midori 160608 add -->
	CString		strPage = _T("");
// midori 160608 add <--

	if(!(indexColumn == pa_start || indexColumn == pa_end))	return;

	// ******************************************
	// 問題点：Editがpa_startにいるときにEnterキーで移動すると、indexColumnがpa_startとpa_endの2回入ってきてしまう
	// ******************************************
	CheckPageNum(indexRow, indexColumn);

// midori 160608 add -->
	// ページ変更後、現在の値を保持する
	if(indexColumn == pa_start)	{
		strPage = m_report.GetCaption(indexRow,pa_start);
		m_vecPrintAll[indexRow].svstart = atoi(strPage);
	}
	if(indexColumn == pa_end)	{
		strPage = m_report.GetCaption(indexRow,pa_end);
		m_vecPrintAll[indexRow].svend = atoi(strPage);
	}
// midori 160608 add <--
}

void CdlgPrintAll::UpdateVectorData()
{
	int row = 0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
// midori 190505 add -->
		m_vecPrintAll[row].isSelect2 = m_report.IsChecked(row, pa_select);
// midori 190505 add <--

		if(m_vecPrintAll[row].isEnable == FALSE)	continue;

		m_vecPrintAll[row].isSelect = m_report.IsChecked(row, pa_select);

// midori 20/10/08u del -->
//// midori 156949 add -->
//		// 確認ダイアログにあげる対象の様式の場合、ページ番号の上書きは行いません。
//		// （"-"が入ってしまうため）
//		if(m_vecPrintAll[row].SortChk == 1)			continue;
//// midori 156949 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		// 確認ダイアログにあげる対象の様式の場合、ページ番号の上書きは行いません。
		// （"-"が入ってしまうため）
		if(m_vecPrintAll[row].isMainasu == 1)		continue;
// midori 20/10/08u add <--

		CString startPage = m_report.GetCaption(row, pa_start);
		CString endPage = m_report.GetCaption(row, pa_end);

		m_vecPrintAll[row].inputStart = atoi(startPage);
		m_vecPrintAll[row].inputEnd = atoi(endPage);
	}
}

//**************************************************
//	全レコードチェック
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgPrintAll::SetAllOutputSpecify()
{	
	// 各様式
	int row = 0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		if(it->isEnable == FALSE)	continue;
		m_report.SetChecked(row, pa_select, TRUE);
	}
	// タイトル
	m_chkTitle.SetCheck(TRUE);
}

void CdlgPrintAll::SetGrid()
{
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
	arryGridColumn.Add(2);
	arryGridColumn.Add(3);
// midori 160610 add -->
	arryGridColumn.Add(4);
// midori 160610 add <--

	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
// midori 160610 add -->
	arrayGridColor.Add(RGB(255, 0, 0));
// midori 160610 add <--

	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}

void CdlgPrintAll::SetPage()
{
	// 各様式
	int row = 0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		if(it->isEnable == FALSE)	continue;
		m_report.SetChecked(row, pa_select, TRUE);
	}
}

// midori 155525 del -->
//int CdlgPrintAll::GetRangaiPage(int FormSeq)
// midori 155525 del <--
// midori 155525 add -->
int CdlgPrintAll::GetRangaiPage( int FormSeq, int pOutZero )
// midori 155525 add <--
{
	if(FormSeq == ID_FORMNO_051){
		CdbUc052Tanaoroshi2 mfcRec(m_pDB);

// midori 155525 del -->
		//return mfcRec.GetPrintPageNum();
// midori 155525 del <--
// midori 155525 add -->
		return mfcRec.GetPrintPageNum(pOutZero);
// midori 155525 add <--
	}
	else if(FormSeq == ID_FORMNO_091){
		CdbUc092Kaikakekin2 mfcRec(m_pDB);

// midori 155525 del -->
		//return mfcRec.GetPrintPageNum();
// midori 155525 del <--
// midori 155525 add -->
		return mfcRec.GetPrintPageNum(pOutZero);
// midori 155525 add <--
	}

	return -1;
}

// midori 160610 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（科目指定）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintAll::OnBnClickedButtonKamoku()
{
// midori 151404 cor -->
//	int					row=0;
//	BOOL				isChange=FALSE;
//	CdlgReference2		dlgRef2(this);	// 科目指定ダイアログ
// ---------------------
	int					row=0,row2=0;
	int					sw=0;
// midori 152385 add -->
	int					svrow=0;
// midori 152385 add <--
	BOOL				isChange=FALSE;
	BOOL				isYousiki[UC_ID_FORMNO_MAX] = {FALSE};
	CString				strPage = _T("");
	CdlgReference2		dlgRef2(this);	// 科目指定ダイアログ
// midori 151404 cor <--

	UpdateVectorData();		// 印刷設定前のレポートコントロールの状態をマップに更新
	vector<PRINT_ALL_DATA> oldVecPrintAll = m_vecPrintAll;

// midori 151404 cor -->
//	dlgRef2.ShowDialog(isChange,m_pDB,m_nFormSeq);
// ---------------------
	dlgRef2.ShowDialog(isChange,isYousiki,m_pDB,m_nFormSeq);
// midori 151404 cor <--

	if(m_chkZero.GetCheck() == 1)			return;	// 帳票イメージのみ出力するにチェックがある場合更新しない
	if(isChange == FALSE)					return;	// 科目指定の内容に変更なければ更新しない

// midori 152385 add -->
	svrow = m_report.GetFocusedRow()->GetIndex();
// midori 152385 add <--
// midori M-16082402 cor -->
	//if(m_pBatchPrt->IsBatchExec() == TRUE)	return;	// 一括印刷時は更新しない
// -------------------------
	if(m_pBatchPrt->IsBatchExec() == TRUE)	{
		// 一括印刷時、詳細設定以外は更新しない
		if(m_pBatchPrt->IsBatchModeSet() != TRUE)	{
			return;
		}
	}
// midori M-16082402 cor <--

	// リストを更新する
	AfxGetApp()->DoWaitCursor(1);
	CreatePrintAllVec();
	UpdateFormTypePage();
	AddRecords();
// midori 151404,M-16113006 cor -->
	//// AddRecordsでチェックは外れているため、チェック状態を元に戻す
	//for(vector<PRINT_ALL_DATA>::iterator it_oldvec = oldVecPrintAll.begin(); it_oldvec != oldVecPrintAll.end(); it_oldvec++)	{
	//	if(it_oldvec->isEnable == FALSE)	continue;
	//	if(it_oldvec->isSelect == FALSE)	continue;
	//	// 科目指定の変更により、oldVecPrintAllのレコード数とm_vecPrintAllのレコード数が異なることがあるので、
	//	// FormSeqが一致するレコードのみチェックをつける
	//	row = 0;
	//	for(vector<PRINT_ALL_DATA>::iterator it_newvec = m_vecPrintAll.begin(); it_newvec != m_vecPrintAll.end(); it_newvec++, row++)	{
	//		if(it_oldvec->FormSeq == it_newvec->FormSeq)	{
	//			m_report.SetChecked(row, pa_select, TRUE);
	//			break;
	//		}
	//	}
	//}
// --------------------------------
	// 通常の場合、グループ内いずれかの様式に変更サインが立っていれば、グループ内すべての様式の変更サインを立てる
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(it->FormNo);
		if(it2 != mapFormType.end()){
			if(it2->second.FormType != ID_FORMTYPE_KOBETU)	{
				sw = 0;
				for(row2=0;row2<it2->second.num;row2++)	{
					if(isYousiki[it->FormOrder-1+row2] == TRUE)	{
						sw = 1;
						break;
					}
				}
				if(sw == 1)	{
					for(row2=0;row2<it2->second.num;row2++)	{
						isYousiki[it->FormOrder-1+row2] = TRUE;
					}
				}
			}
		}
	}

	row = 0;
	for(vector<PRINT_ALL_DATA>::iterator it_oldvec = oldVecPrintAll.begin(); it_oldvec != oldVecPrintAll.end(); it_oldvec++,row++)	{
		if(it_oldvec->isEnable == FALSE)	continue;
		// 科目指定の変更により、oldVecPrintAllのレコード数とm_vecPrintAllのレコード数が異なることがあるので、
		// FormSeqが一致するレコードのみチェックをつける
		row2 = 0;
		for(vector<PRINT_ALL_DATA>::iterator it_newvec = m_vecPrintAll.begin(); it_newvec != m_vecPrintAll.end(); it_newvec++, row2++)	{
			if(it_oldvec->FormSeq == it_newvec->FormSeq)	{
				// AddRecordsでチェックは外れているため、チェック状態を元に戻す
				if(it_oldvec->isSelect != FALSE)	m_report.SetChecked(row2, pa_select, TRUE);
				if(isYousiki[row] == FALSE)	{
					// 変更無しの開始頁、終了頁を元の頁に戻す
					m_vecPrintAll[row2].start = oldVecPrintAll[row].start;
					m_vecPrintAll[row2].end = oldVecPrintAll[row].end;
					m_vecPrintAll[row2].inputStart = oldVecPrintAll[row].inputStart;
					m_vecPrintAll[row2].inputEnd = oldVecPrintAll[row].inputEnd;
					m_vecPrintAll[row2].str_start = oldVecPrintAll[row].str_start;
					m_vecPrintAll[row2].str_end = oldVecPrintAll[row].str_end;
// midori 190505 add -->
					m_vecPrintAll[row2].inputCmb = oldVecPrintAll[row].inputCmb;
					m_vecPrintAll[row2].SortChk = oldVecPrintAll[row].SortChk;
// midori 190505 add <--
// midori 20/10/08u add -->
					m_vecPrintAll[row2].isMainasu = oldVecPrintAll[row].isMainasu;
// midori 20/10/08u add <--
					m_vecPrintAll[row2].svstart = oldVecPrintAll[row].svstart;
					m_vecPrintAll[row2].svend = oldVecPrintAll[row].svend;
					m_vecPrintAll[row2].start2 = oldVecPrintAll[row].start2;
					m_vecPrintAll[row2].end2 = oldVecPrintAll[row].end2;
					m_vecPrintAll[row2].inputStart2 = oldVecPrintAll[row].inputStart2;
					m_vecPrintAll[row2].inputEnd2 = oldVecPrintAll[row].inputEnd2;
					m_vecPrintAll[row2].str_start2 = oldVecPrintAll[row].str_start2;
					m_vecPrintAll[row2].str_end2 = oldVecPrintAll[row].str_end2;
// midori 190505 add -->
					m_vecPrintAll[row2].inputCmb2 = oldVecPrintAll[row].inputCmb2;
					m_vecPrintAll[row2].SortChk2 = oldVecPrintAll[row].SortChk2;
// midori 190505 add <--
					strPage.Empty();
					strPage.Format(_T("%d"),m_vecPrintAll[row2].inputStart);
					m_report.SetCaption(row2, pa_start, strPage);
					strPage.Empty();
					strPage.Format(_T("%d"),m_vecPrintAll[row2].inputEnd);
					m_report.SetCaption(row2, pa_end, strPage);
				}
				break;
			}
		}
	}
// midori 151404,M-16113006 cor <--
// midori 152393 add -->
	SetOutputChk2(isYousiki);
// midori 152393 add <--
	UpdateVectorData();
	UpdateRecordState();
	AfxGetApp()->DoWaitCursor(-1);
// midori 152385 add -->
	m_report.SetFocusedRow(svrow);
	m_btnKamoku.SetFocus();
// midori 152385 add <--
}
// midori 160610 add <--

// midori 180101 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（表紙設定）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgPrintAll::OnBnClickedButtonHyoshi()
{
	CdlgPrintHyoshi		dlgHyoshi(this);	// 表紙設定ダイアログ


	UpdateVectorData();		// 印刷設定前のレポートコントロールの状態をマップに更新

	if(dlgHyoshi.ShowDialog(m_pDB) == ID_DLG_OK)	{
	}

	m_btnHyoshi.SetFocus();
}
// midori 180101 add <--

// midori 190505 add -->
// --------------------------------------------------------------------------------------------------------------------
//	一括印刷用の確認ダイアログを表示する
// --------------------------------------------------------------------------------------------------------------------
int CdlgPrintAll::CreateIkkatuConfDlg(void)
{
	int						ret=0;
	int						cnt=0;
	int						row=0;
	int						idx=0;
	int						sort=0;
	int						sortmax=0;

	int						f041en=0;
	int						f101en=0;
	int						f141en=0;
	int						f151en=0;
	int						f152en=0;
	int						f161en=0;
// No.200601 del -->
	//BYTE					fgPage[9];
	//BYTE					ItemDt[9];
// No.200601 del <--
	int						flg=0;				// 編集～ソートの「改頁を行う」の制御
// midori 156909 add -->
	int						flg2=0;				// 編集～ソートの「改頁を行う」のフラグ
// midori 156909 add <--
	BOOL					isenable=FALSE;
	CdlgIkkatuConfirmation	dlg(this);
	CdbUcInfSub				mfcRec(m_pDB);		// uc_inf_subテーブルクラス
	CdbUcLstItemSort		mfcRec2(m_pDB);		// uc_lst_item_sortテーブルクラス

	// ダイアログを表示する前にデータをセット
	cnt = 0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll.begin(); it != m_vecPrintAll.end(); it++, row++){
		m_vecPrintAll[row].formType = -1;
// midori 20/10/08u del -->
		//m_vecPrintAll[row].sortEdit = TRUE;
// midori 20/10/08u del <--
		m_vecPrintAll[row].isKEnable = TRUE;

		// 出力する
		if(it->isSelect2 == TRUE && it->SortChk == 1)	{
			// 出力形式
			map<int, FORM_TYPE_DATA>::iterator it2 = mapFormType.find(it->FormNo);
			if(it2 != mapFormType.end()){
				m_vecPrintAll[row].formType = it2->second.FormType;	// 出力形式 0:通常、1:個別（確認ダイアログで使用）
			}

// midori 20/10/08u del -->
//			// 出力方法の選択肢制御
//			flg = 0;
//// midori 156909 add -->
//			flg2 = 0;
//// midori 156909 add <--
//			if(mfcRec.RequeryFormSeq(it->FormSeq) == DB_ERR_OK)	{
//				if( !(mfcRec.IsEOF()) )	{
//					if(mfcRec.m_Sort1 == 0)	{
//						// 一度も編集～ソートを開いていない場合は、第１ソート項目の項目番号が書かれていない
//						if(it->FormSeq == 2)	mfcRec.m_Sort1 = 8;
//						else					mfcRec.m_Sort1 = 1;
//					}
//					if(mfcRec2.RequeryFormItem(it->FormSeq, mfcRec.m_Sort1) == DB_ERR_OK)	{
//						if(!mfcRec2.IsEOF())	{
//							flg = mfcRec2.m_FgPage;			// 1:改頁対象項目
//						}
//					}
//					mfcRec2.Fin();
//				}
//// midori 156909 add -->
//				if(flg != 0)	{
//					if(mfcRec.m_OpKaiPage != 0)	{
//						flg2 = 1;
//					}
//				}
//// midori 156909 add <--
//				mfcRec.Fin();
//			}
//// midori 156909 del -->
////			m_vecPrintAll[row].sortEdit = flg;		// 確認ダイアログ用、頁の入力制限
//// midori 156909 del <--
//// midori 156909 add -->
//			m_vecPrintAll[row].sortEdit = flg2;		// 確認ダイアログ用、頁の入力制限
//// midori 156909 add <--
// midori 20/10/08u del <--

			// ページ番号の入力制御
			// 出力形式「通常」で－１,－２,－３が全て確認ダイアログの対象の場合は、－２,－３の列は編集不可にする
			// 上記以外の様式、もしくは出力形式「個別」の場合は、常に編集可。
			isenable = TRUE;
			if((it->FormSeq == ID_FORMNO_041 || it->FormSeq == ID_FORMNO_101 || it->FormSeq == ID_FORMNO_141 ||
				it->FormSeq == ID_FORMNO_151 || it->FormSeq == ID_FORMNO_161) && m_vecPrintAll[row].formType == ID_FORMTYPE_NORMAL)
			{
				if(it->FormSeq == ID_FORMNO_041)	f041en = 1;
				if(it->FormSeq == ID_FORMNO_101)	f101en = 1;
				if(it->FormSeq == ID_FORMNO_141)	f141en = 1;
				if(it->FormSeq == ID_FORMNO_151)	f151en = 1;
				if(it->FormSeq == ID_FORMNO_161)	f161en = 1;
			}
			else if((it->FormSeq == ID_FORMNO_042 || it->FormSeq == ID_FORMNO_102 || it->FormSeq == ID_FORMNO_142 ||
						it->FormSeq == ID_FORMNO_152 || it->FormSeq == ID_FORMNO_153 || it->FormSeq == ID_FORMNO_162) &&
						m_vecPrintAll[row].formType == ID_FORMTYPE_NORMAL)
			{
				if(it->FormSeq == ID_FORMNO_042 && f041en == 1)						isenable = FALSE;
				if(it->FormSeq == ID_FORMNO_102 && f101en == 1)						isenable = FALSE;
				if(it->FormSeq == ID_FORMNO_142 && f141en == 1)						isenable = FALSE;
				if(it->FormSeq == ID_FORMNO_152 && f151en == 1)						isenable = FALSE;
				if(it->FormSeq == ID_FORMNO_153 && (f151en == 1 || f152en == 1))	isenable = FALSE;
				if(it->FormSeq == ID_FORMNO_162 && f161en == 1)						isenable = FALSE;

				if(it->FormSeq == ID_FORMNO_152)	f152en = 1;
			}
			m_vecPrintAll[row].isKEnable = isenable;		// 確認ダイアログ用、出力方法の選択肢

			cnt++;
		}
	}

	//------------------------------------------------------------
	// ダイアログ表示
	if(cnt > 0)	{
		ret = dlg.ShowDialog(m_pDB, &m_vecPrintAll);
	}
	//------------------------------------------------------------

	return(ret);
}
// midori 190505 add <--
