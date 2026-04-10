//--------------------------------------------------
//	dlgTanaoroshi.cpp
//
//	2006.03.01～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgTanaoroshi.h"
#include "ConvOutRangai.h"

// CdlgTanaoroshi ダイアログ

IMPLEMENT_DYNAMIC(CdlgTanaoroshi, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgTanaoroshi::CdlgTanaoroshi(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgTanaoroshi::IDD, pParent)
{
	m_CurrentPage	= 1;
	m_MaxPage		= 1;
	m_MaxPageDB		= 0;
	m_PageMaxFrom	= 1;
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgTanaoroshi::~CdlgTanaoroshi()
{
}

void CdlgTanaoroshi::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_A, m_rdoA);
	DDX_Control(pDX, IDC_RADIO_B, m_rdoB);
	DDX_Control(pDX, IDC_RADIO_C, m_rdoC);
	DDX_Control(pDX, IDC_RADIO_D, m_rdoD);
	DDX_Control(pDX, IDC_DATE_TEXT, m_txtDate);
	DDX_Control(pDX, IDC_NAME_TEXT, m_txtName);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_PRINT_CHECK, m_chkPrint);
	DDX_Control(pDX, IDC_NOTE_LABEL, m_lblNote);
	DDX_Control(pDX, IDC_DATE_LABEL, m_lblDate);
	DDX_Control(pDX, IDC_PRINT_COMBO, m_cmbPrint);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnMovePrev);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnMoveNext);
	DDX_Control(pDX, IDC_STATIC_PAGE_CURRENT, m_txtCurrent);
	DDX_Control(pDX, IDC_STATIC_PAGE_MAX, m_txtMax);
	DDX_Control(pDX, IDC_RADIO_COMMON, m_rdoCommon);
	DDX_Control(pDX, IDC_RADIO_INDIV, m_rdoIndiv);
}


BEGIN_MESSAGE_MAP(CdlgTanaoroshi, ICSDialog)
	ON_BN_CLICKED(IDC_RADIO_A, &CdlgTanaoroshi::OnBnClickedRadioA)
	ON_BN_CLICKED(IDC_RADIO_B, &CdlgTanaoroshi::OnBnClickedRadioB)
	ON_BN_CLICKED(IDC_RADIO_C, &CdlgTanaoroshi::OnBnClickedRadioC)
	ON_BN_CLICKED(IDC_RADIO_D, &CdlgTanaoroshi::OnBnClickedRadioD)
	ON_BN_CLICKED(IDC_PRINT_CHECK, &CdlgTanaoroshi::OnBnClickedPrintCheck)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgTanaoroshi::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CdlgTanaoroshi::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CdlgTanaoroshi::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_RADIO_COMMON, &CdlgTanaoroshi::OnBnClickedRadioCommon)
	ON_BN_CLICKED(IDC_RADIO_INDIV, &CdlgTanaoroshi::OnBnClickedRadioIndiv)

	ON_CBN_SELCHANGE(IDC_PRINT_COMBO, &CdlgTanaoroshi::OnCbnSelchangePrintCombo)
	ON_MESSAGE(WM_SETCURSORBUTEPOS, SetCursorBytePosHandler)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgTanaoroshi, ICSDialog)
	ON_EVENT(CdlgTanaoroshi, IDC_DATE_TEXT, 1, CdlgTanaoroshi::TerminationDateText, VTS_I2)
	ON_EVENT(CdlgTanaoroshi, IDC_DATE_TEXT, 4, CdlgTanaoroshi::CheckDataDateText, VTS_UNKNOWN)
END_EVENTSINK_MAP()

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgTanaoroshi::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	//	ツールバーの設定成功？
	if ( SetToolBar() == 0 ){
		m_ToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_FAKE, FALSE );
	}

	//	データ取得
	//GetData( m_pDB );
	Read();

//	SyncPage();

	// 出力形式が「ページ共通共通」なら、強制的に1p目を表示させる
	if(m_rdoCommon.GetCheck())	m_CurrentPage = 1;
	
	//Disp(m_CurrentPage);
	MovePage(m_CurrentPage);

	//OnBnClickedPrintCheck();

// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	{
		// コントロールを無効にする
		// 印字する
		m_chkPrint.EnableWindow(FALSE);
		// ページコンボボックス
		m_cmbPrint.EnableWindow(FALSE);
		// ページ共通出力
		m_rdoCommon.EnableWindow(FALSE);
		// ページ個別出力
		m_rdoIndiv.EnableWindow(FALSE);
		// A 実地棚卸
		m_rdoA.EnableWindow(FALSE);
		// B 帳簿棚卸
		m_rdoB.EnableWindow(FALSE);
		// AとBとの併用
		m_rdoC.EnableWindow(FALSE);
		// 棚卸を行った時期
		m_clsFunc.ICSStaticEnableWindow( &m_lblDate , FALSE );	//	日付ラベル
		m_txtDate.EnableWindow(FALSE);
		// D 登録名称を印字する
		m_rdoD.EnableWindow(FALSE);
		// 登録名称
		m_txtName.EnableWindow(FALSE);
		// ツールバー F7 ページ削除
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, FALSE);
		// ツールバー F9 ページ追加
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F9, FALSE);
	}
// midori 160612 add <--

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// CdlgTanaoroshi メッセージ ハンドラ

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（Ａ）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::OnBnClickedRadioA()
{
	//	ラジオボタン設定
	SetRadioButton( IDC_RADIO_A );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（Ｂ）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::OnBnClickedRadioB()
{
	//	ラジオボタン設定
	SetRadioButton( IDC_RADIO_B );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（Ｃ）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::OnBnClickedRadioC()
{
	//	ラジオボタン設定
	SetRadioButton( IDC_RADIO_C );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ラジオボタンクリック（Ｄ）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::OnBnClickedRadioD()
{
	//	ラジオボタン設定
	SetRadioButton( IDC_RADIO_D );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	チェックボックスクリック（印字する）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::OnBnClickedPrintCheck()
{
	CheckPrintStatus();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OK）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::OnBnClickedOk()
{
	//	データ設定
	Write();
	//	戻値を設定
	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::OnOK()
{
//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgTanaoroshi::PreTranslateMessage(MSG* pMsg)
{
	int	nID = m_clsFunc.GetFocusControlID( this );			//	現在フォーカスのあるコントロールID取得
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
		//	↑キーが押された
		case VK_UP:
		//	←キーが押された
		case VK_LEFT:
			// キーボードで「ページ共通出力」にチェックをつけた時、OnBnClickedRadioCommonSub()内のm_rdoCommon.GetCheck()がFALSEになってしまう為
			if(nID == IDC_RADIO_INDIV){
				m_rdoIndiv.SetCheck(FALSE);
				m_rdoCommon.SetCheck(TRUE);
				PostMessage( WM_COMMAND, MAKEWPARAM(IDC_RADIO_COMMON, BN_CLICKED), 0 );
				m_rdoCommon.SetFocus();
				return 1;
			}
			 break;
		 //	↓キーが押された
		case VK_DOWN:
		//	→キーが押された
		case VK_RIGHT:
			// キーボードで「ページ個別出力」にチェックをつけた時、OnBnClickedRadioIndiv()内のm_rdoIndiv.GetCheck()がFALSEになってしまう為
			if(nID == IDC_RADIO_COMMON){
				m_rdoIndiv.SetCheck(TRUE);
				m_rdoCommon.SetCheck(FALSE);
				PostMessage( WM_COMMAND, MAKEWPARAM(IDC_RADIO_INDIV, BN_CLICKED), 0 );
				m_rdoIndiv.SetFocus();
				return 1;
			}
			break;
		case VK_F7:
			PostMessage( WM_COMMAND, ID_TOOL_BUTTON_F7 );
			break;
		case VK_F9:
			PostMessage( WM_COMMAND, ID_TOOL_BUTTON_F9 );
			break;
		case VK_PRIOR:
			PostMessage( WM_COMMAND, IDC_BUTTON_PREV );
			break;
		case VK_NEXT:
			PostMessage( WM_COMMAND, IDC_BUTTON_NEXT );
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント
//	【引数】	nCnar	…	仮想キーコード
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::TerminationDateText(short nChar)
{
/*	フォーカスを抜けた際の日付変換処理を「CheckDataDateText」に移行		2006.05.31
	int		nDate = 0;	//	日付格納

	//	仮想キーコードで分岐
	switch( nChar ){
	//	Tabキー
	case VK_TAB:
	//	Enterキー
	case VK_RETURN:

		//	入力確定時に日付変換を行う
		nDate = GetICSInputDate();			//	ICSInputコントロールから日付値を取得
		SetICSInputDate( nDate );			//	ICSInputコントロールに日付値を設定
		break;

	//	その他
	default:
		break;
	}
*/
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	日付データの確認
//	【引数】	data	…	INPUTDATA構造体の先頭ポインタ
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgTanaoroshi::CheckDataDateText(LPUNKNOWN data)
{
	CdateConvert	clsDate;	//	日付変換クラス
	short			nFlag = 1;	//	データ処理フラグ
								//		 0：データ内容は正しい。
								//		 1：データ内容は正しい。
								//		-1：データ内容が不正であった。

// midori 181101 add -->
	if(IsValidKaigen() == TRUE) {
		if((( INPUTDATA* )data)->ip_day[0] == 0x01 && (( INPUTDATA* )data)->ip_day[1] == 0x01 && 
			(( INPUTDATA* )data)->ip_day[2] >= 0x01 && (( INPUTDATA* )data)->ip_day[2] <= 0x07) {
			memset((( INPUTDATA* )data)->ip_day,0x00,sizeof((( INPUTDATA* )data)->ip_day));
			nFlag = -1;
		}
	}
// midori 181101 add <--

	//	日付変換
	clsDate.Convert( 0x00, (( INPUTDATA* )data)->ip_day[0], (( INPUTDATA* )data)->ip_day[1], (( INPUTDATA* )data)->ip_day[2] );

	//	入力した日付を保持
	m_bSpDate = clsDate.m_nDate;

	//	日付が不正？
	if ( clsDate.m_nDate == 0 ){
		//	データ処理フラグを不正扱いにする
		nFlag = -1;
	}
	else{
		//	日付変換した結果を設定
		(( INPUTDATA* )data)->ip_day[0] = clsDate.m_cWareki;
		(( INPUTDATA* )data)->ip_day[1] = clsDate.m_cMonth;
		(( INPUTDATA* )data)->ip_day[2] = clsDate.m_cDay;
	}
	
	//	コントロールの更新
	m_txtDate.ReplyData( nFlag, data );
}

//**************************************************
//	ラジオボタン設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgTanaoroshi::SetRadioButton()
{
// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	return;
// midori 160612 add <--

	//	Dにチェックあり？
	if(m_chkPrint.GetCheck() == FALSE){
		m_txtName.EnableWindow( FALSE ); 
		return;
	}

	if ( m_rdoD.GetCheck() == TRUE )	m_txtName.EnableWindow(TRUE); 
	else								m_txtName.EnableWindow(FALSE); 
}

//**************************************************
//	ラジオボタン設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgTanaoroshi::SetRadioButton( long nID )
{
// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	return;
// midori 160612 add <--

	//	Dがチェックされた？
	if ( nID == IDC_RADIO_D )	m_txtName.EnableWindow(TRUE); 
	else						m_txtName.EnableWindow(FALSE); 
}

//**************************************************
//	印字するチェックボックス確認
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgTanaoroshi::CheckPrintStatus()
{
	// ベクタに「印字する」の更新
	m_vecRangai[m_CurrentPage - 1].isPrint = m_chkPrint.GetCheck();
	//	印字するにチェックあり？
	if ( m_vecRangai[m_CurrentPage - 1].isPrint == TRUE )	SetAllControl( TRUE );
	else													SetAllControl( FALSE );
}

//**************************************************
//	全てのコントロール設定
//	【引数】	fFlag	…	表示フラグ（TRUE：表示する／FALSE：表示しない）
//	【戻値】	なし
//**************************************************
void CdlgTanaoroshi::SetAllControl( BOOL fFlag )
{
// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	return;
// midori 160612 add <--

	m_rdoA.EnableWindow( fFlag );							//	ラジオボタンＡ
	m_rdoB.EnableWindow( fFlag );							//	ラジオボタンＢ
	m_rdoC.EnableWindow( fFlag );							//	ラジオボタンＣ
	m_rdoD.EnableWindow( fFlag );							//	ラジオボタンＤ
	m_txtDate.EnableWindow( fFlag );						//	日付テキスト
	m_clsFunc.ICSStaticEnableWindow( &m_lblDate , fFlag );	//	日付ラベル
	//m_cmbPrint.EnableWindow( fFlag );						//　出力コンボボックス

	//	表示する？
	if(fFlag == TRUE)	SetRadioButton();					//	ラジオボタン設定
	else				m_txtName.EnableWindow( fFlag );	//	名称テキスト

	// ページ関連コントロール状態設定
	OnCbnSelchangePrintComboSub(1);

	// 全てのラジオボタンがDisable状態になるとフォーカスがなくなり、
	// PreTranslateMessage()が走らなくなる為、コンボボックスにフォーカスを当てる
	if(this->GetFocus() == NULL)	m_chkPrint.SetFocus();
}

//**************************************************
//	ICSInputコントロールに日付値を設定
//	【引数】	nDate	…	日付値
//	【戻値】	なし
//**************************************************
void CdlgTanaoroshi::SetICSInputDate( int nDate )
{
	CdateConvert	clsDate;	//	日付変換クラス
	INPUTDATA		udData;		//	INPUTDATA構造体
	char			cBuf[4];	//	バッファ

	//	初期化
	InitInputData( &udData );
	ZeroMemory( cBuf, sizeof( cBuf ) );
	
	//	日付を変換
	clsDate.Convert( nDate, cBuf, DC_DATE_WMD );
	//	ICSInputコントロールからデータ取得
	m_txtDate.GetData( ( LPUNKNOWN )&udData );
	
	//	変換したデータをINPUTDATA構造体に設定
	udData.ip_day[0] = cBuf[0];		//	和暦（BCD）
	udData.ip_day[1] = cBuf[1];		//	月（BCD）
	udData.ip_day[2] = cBuf[2];		//	日（BCD）

	//	ICSInputコントロールにデータ設定
	m_txtDate.SetData( ( LPUNKNOWN )&udData );
}

//**************************************************
//	ICSInputコントロールから日付値を取得
//	【引数】	なし
//	【戻値】	日付値
//**************************************************
int CdlgTanaoroshi::GetICSInputDate()
{
	int				nRet = 0;		//	戻値
	CdateConvert	clsDate;		//	日付変換クラス
	INPUTDATA		udData;			//	INPUTDATA構造体
	char			chrDate[4] = "";//	前回入力時の日付格納

	//	初期化
	InitInputData( &udData );
	memset( chrDate,'\0', sizeof(chrDate));

	//	ICSInputコントロールからデータ取得
	m_txtDate.GetData( ( LPUNKNOWN )&udData );

	// 前回入力時の日付を「和暦.月.日」に変換)
	clsDate.Convert( m_bSpDate, chrDate, DC_DATE_WMD );

	// 変更前の「和暦.月.日」と現在の「和暦.月.日」を比較
	if ( chrDate[0] == udData.ip_day[0] && 
		 chrDate[1] == udData.ip_day[1] &&
		 chrDate[2] == udData.ip_day[2] && 
		 chrDate[3] == udData.ip_day[3] ){
		nRet = m_bSpDate;
	}
	else{
		//	日付を変換
		nRet = clsDate.Convert( 0x00, udData.ip_day[0], udData.ip_day[1], udData.ip_day[2] );
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	INPUTDATA構造体初期化
//	【引数】	pudData		…	INPUTDDATA構造体のポインタ
//	【戻値】	なし
//**************************************************
void CdlgTanaoroshi::InitInputData( INPUTDATA* pudData )
{
	ZeroMemory( pudData->chk_day, sizeof( pudData->chk_day ) );
	pudData->chk_kmk.edaban = 0;
	pudData->chk_kmk.kamoku = 0;
	pudData->chk_kmk.sgn = 0;
	ZeroMemory( pudData->ip_bmn, sizeof( pudData->ip_bmn ) );
	ZeroMemory( pudData->ip_day, sizeof( pudData->ip_day ) );
	pudData->ip_dpn = 0;
	ZeroMemory( pudData->ip_eda, sizeof( pudData->ip_eda ) );
	pudData->ip_kmk.edaban = 0;
	pudData->ip_kmk.kamoku = 0;
	ZeroMemory( pudData->ip_kmk.kmkname, sizeof( pudData->ip_kmk.kmkname ) );
	pudData->ip_seq = 0;
	ZeroMemory( pudData->ip_tek, sizeof( pudData->ip_tek ) );
	pudData->ip_type = 0;
	ZeroMemory( pudData->ip_val, sizeof( pudData->ip_val ) );
}

// midori 160612 cor -->
////**************************************************
////	ダイアログ表示
////	【引数】	pDB				…	データベースハンドル
////	【戻値】	ID_DLG_OK		…	OKボタンで終了
////				ID_DLG_CANCEL	…	キャンセルボタンで終了
////**************************************************
//int CdlgTanaoroshi::ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info )
// ---------------------
//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				pDataKakutei	…	入力データを確定する
//	【戻値】	ID_DLG_OK		…	OKボタンで終了
//				ID_DLG_CANCEL	…	キャンセルボタンで終了
//**************************************************
int CdlgTanaoroshi::ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info, BOOL pDataKakutei )
// midori 160612 cor <--
{
	m_nRet = ID_DLG_CANCEL;	//	戻値
	m_bSpDate = 0;			//　前回入力時の日付
// midori 160612 add -->
	m_DataKakutei = pDataKakutei;	// 入力データ確定
// midori 160612 add <--

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){		

		try{

			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;			//	データベースハンドル取得
				m_CurrentPage = page_info.intCurPage;
				m_PageMaxFrom = page_info.intMaxPage;
				DoModal();				//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return( m_nRet );
}

int CdlgTanaoroshi::Read()
{
	if(m_pDB == NULL)	return -1;
	CdbUc052Tanaoroshi2	mfcRec(m_pDB);

	mfcRec.Init();
	m_MaxPageDB = mfcRec.GetNumPage();
	// Read時点ではDBの最大ページイコールメモリの最大値ページ
	// 後にセットするが、SetAllControl()でm_MaxPageが参照される為ここでプリセットする
	m_MaxPage = m_MaxPageDB;

	for(int i=0; i<m_MaxPageDB; i++){
		if(mfcRec.Init(i+1) != DB_ERR_OK)	return -1;
		if(mfcRec.IsEOF()){
			mfcRec.Fin();
			return -1;
		}

		if(i==0)	SetControl(mfcRec);

		// ベクタに追加
		AddVec(i+1, mfcRec);
	}
	mfcRec.Fin();

	m_MaxPage = m_vecRangai.size();

	// 棚卸資産の内訳書のページ数が欄外登録のページ数より多い場合、カレントページを1ページ目にセット
	if(m_MaxPage < m_CurrentPage)	m_CurrentPage = 1;
	return 0;
}

int CdlgTanaoroshi::Write()
{
	if(m_pDB == NULL)	return -1;
	CdbUc052Tanaoroshi2	mfcRec(m_pDB);

	// 入力データをベクタに反映
	UpdateVector();

	// ベクタの値をDBに反映
	int page = 1;
	for(vector<RangaiTana>::iterator it = m_vecRangai.begin(); it != m_vecRangai.end(); it++, page++){
		mfcRec.Init(page);

		UpdateRecord(mfcRec, it, page);
	}

	// レコード数より実データ数が少ない場合、レコードを削除
	if(m_MaxPageDB > m_MaxPage)		mfcRec.DeletePage(m_MaxPage + 1);

	return DB_ERR_OK;
}

int CdlgTanaoroshi::UpdateRecord(CdbUc052Tanaoroshi2& db, vector<RangaiTana>::iterator& it, int page)
{
	db.Edit();

	if (it->isPrint)	db.m_FgShow = 0;					// チェックON→0
	else				db.m_FgShow = ID_FGSHOW_TANAOROSHI;	// チェックOFF→ID_FGSHOW_TANAOROSHI
	db.m_NumPage = page;
	db.m_NumRow = 1;
	db.m_SpDate = it->SpDate;
	db.m_HdName = it->HdName;
	db.m_Method = it->Method;

	// 1ページ目のみにチェックボックス、コンボボックスの値を持つ
	if(page == 1){
		CConvOutRangai cr;
		db.m_OutRangai = cr.ToDB(m_cmbPrint.GetCurSel(), m_rdoIndiv.GetCheck());
	}

	db.Update();

	return DB_ERR_OK;
}

void CdlgTanaoroshi::AddVec(int page, CdbUc052Tanaoroshi2& mfcRec)
{	
	// ベクタに登録
	RangaiTana rk(mfcRec);
	m_vecRangai.push_back(rk);
}

void CdlgTanaoroshi::Disp(int page)
{
	RangaiTana rt = m_vecRangai[page - 1];
	//	登録名称を設定
	m_txtName.SetCaption(rt.HdName);
	// カーソルをセット（ポスト）
	PostMessage(WM_SETCURSORBUTEPOS, page);	
	//　オープン時の日付値を保持
	m_bSpDate = rt.SpDate;
	//	ICSInputコントロールに日付値を設定
	SetICSInputDate(m_bSpDate);
	// 棚卸方法
	SetMethod(rt.Method);
	// チェックボックス
	BOOL fFlag = FALSE;
	if ( m_vecRangai[page - 1].isPrint )	fFlag = TRUE;
	else									fFlag = FALSE;
	m_chkPrint.SetCheck( fFlag );

	CheckPrintStatus();

	// ページ
	DispPage(page);
}

void CdlgTanaoroshi::DispPage(int page)
{
	CString str_page;
	str_page.Format("%d", page);
	m_txtCurrent.SetWindowTextA(str_page);

	str_page.Format("%d", m_MaxPage);
	m_txtMax.SetWindowTextA(str_page);
}

void CdlgTanaoroshi::UpdateVector()
{
	BOOL isPrint = m_chkPrint.GetCheck();
	BYTE Method = GetMethod();
	CString hoge = m_txtName.GetCaption();
	TRACE("m_txtName.GetCaption() = %s\n", hoge);
	//CString HdName = m_clsFunc.DeleteRightSpace( m_txtName.GetCaption() );
	CString HdName = m_clsFunc.DeleteRightSpace( hoge );
	long SpDate = GetICSInputDate();
	RangaiTana rt(isPrint, Method, HdName, SpDate);

	m_vecRangai[m_CurrentPage - 1] = rt;
}


void CdlgTanaoroshi::OnCbnSelchangePrintCombo()
{
	OnCbnSelchangePrintComboSub(0);
}

void CdlgTanaoroshi::OnCbnSelchangePrintComboSub(BOOL mode)
{
	int cur_sel = m_cmbPrint.GetCurSel();
	BOOL state = FALSE;

	if(cur_sel == 2)	state = TRUE;	// 全ページ
	else				state = FALSE;

	SetPrintTypeControlls(state);

	if(mode == 1)	return;

	// 「全ページ」以外なら強制的にページ共通出力
	if(state == FALSE){
		m_rdoCommon.SetCheck(TRUE);
		m_rdoIndiv.SetCheck(FALSE);

		if(m_CurrentPage == 1)	return;

		UpdateVector();
		MovePage(1);
	}
}

void CdlgTanaoroshi::SetPrintTypeControlls(BOOL fFlag)
{
// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	return;
// midori 160612 add <--

	if(m_cmbPrint.IsWindowEnabled() == FALSE)	fFlag = FALSE;

	// ラジオボタン状態設定
	m_rdoCommon.EnableWindow(fFlag);
	m_rdoIndiv.EnableWindow(fFlag);

	// ラベル、ボタン状態設定
	OnBnClickedRadioCommonSub();
}

int CdlgTanaoroshi::SetToolBar()
{
	CRect	ClientStart;	//	クライアント開始矩形
	CRect	ClientNow;		//	クライアント現在矩形
	CPoint	Offset;			//	オフセット位置
	CRect	Child;			//	子ウィンドウ矩形
	CWnd*	pChild = NULL;	//	子ウィンドウハンドル
	CRect	Window;			//	ウィンドウ矩形
	
	//	ツールバー作成
	if ( m_ToolBar.Create( this ) == 0 ){
		//	エラー
		return( -1 );
	}
	
	//	ビットマップ読み込み
	if ( m_ToolBar.LoadToolBar( IDR_RANGAI_BAR ) == 0 ){
		//	エラー
		return( -2 );
	}
	
	//------------------------------
	//	スタイル設定
	//		CBRS_TOOLTIPS	…	ツールチップ表示
	//		CBRS_FLYBY		…	メッセージテキストの更新
	//		CBRS_BORDER_ANY	…	フレームを任意に設定
	//------------------------------
	m_ToolBar.SetBarStyle( m_ToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_BORDER_ANY );
	//	拡張スタイルをフラットツールバーに設定
	m_ToolBar.ModifyStyle( 0, 0x0800 );

	//	クライアント領域での再配置/サイズ変更
	RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0 );
	
	//	成功
	return( 0 );
}

BOOL CdlgTanaoroshi::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch( wParam ){
		//	[F7]削除
		case ID_TOOL_BUTTON_F7:
			OnToolButtonF7();
			break;

		//	[F9]追加
		case ID_TOOL_BUTTON_F9:
			OnToolButtonF9();
			break;
	}

	return ICSDialog::OnCommand(wParam, lParam);
}

void CdlgTanaoroshi::OnToolButtonF7()
{
	if(m_ToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F7) == 0)		return;

	// 確認ダイアログ
	if( ICSMessageBox( _T("現在表示している欄外ページを削除します。\n\n削除してもよろしいですか？"), MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2 ) != IDOK ){
		return;
	}

	// 現在表示中のイテレータを取得
	vector<RangaiTana>::iterator it = m_vecRangai.begin() + m_CurrentPage - 1;
	// 削除
	m_vecRangai.erase(it);

	// 要素がなくなったら一つ新規追加（1ページしかないときはF7をDisableにしているので入ってこないとは思うが）
	if(m_vecRangai.size() == 0){
		m_MaxPage--;
		OnToolButtonF9();
		return;
	}

	m_MaxPage = m_vecRangai.size();

	// 最後のページを削除した場合、カレントページを調整
	if(m_CurrentPage > m_MaxPage)	m_CurrentPage = m_MaxPage;

	MovePage(m_CurrentPage);

	SetToolBarState();
}

void CdlgTanaoroshi::OnToolButtonF9()
{
	if(m_ToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F9) == 0)		return;

	UpdateVector();

	int MaxPageTemp = m_MaxPage + 1;

	RangaiTana rt;
	rt.Method = DT_METHOD_D;	// デフォルトはＤ　登録名称を印字
	// 末尾に追加
	m_vecRangai.push_back(rt);

	m_MaxPage = m_vecRangai.size();
	m_CurrentPage = m_MaxPage;

	MovePage(m_MaxPage);

	SetToolBarState();
}

void CdlgTanaoroshi::OnBnClickedButtonPrev()
{
	if(m_btnMovePrev.IsWindowEnabled() == FALSE)	return;
	int old_page = m_CurrentPage;

	// 入力データをベクタに反映
	UpdateVector();

	m_CurrentPage -= 1;
	if(m_CurrentPage < 1)	m_CurrentPage = 1;

	if(old_page == m_CurrentPage)	return;

	MovePage(m_CurrentPage);

	// フォーカスが飛ぶのを防ぐ
	if(m_clsFunc.GetFocusControlID(this) == 0){
		SetFocus();
	}
}

void CdlgTanaoroshi::OnBnClickedButtonNext()
{
	if(m_btnMoveNext.IsWindowEnabled() == FALSE)	return;
	int old_page = m_CurrentPage;	

	// 入力データをベクタに反映
	UpdateVector();

	m_CurrentPage += 1;
	if(m_CurrentPage > m_MaxPage)	m_CurrentPage = m_MaxPage;

	if(old_page == m_CurrentPage)	return;

	MovePage(m_CurrentPage);

	// フォーカスが飛ぶのを防ぐ
	if(m_clsFunc.GetFocusControlID(this) == 0){
		SetFocus();
	}
}

void CdlgTanaoroshi::OnBnClickedRadioCommon()
{
	OnBnClickedRadioCommonSub();

	UpdateVector();

	MovePage(1);
}

void CdlgTanaoroshi::OnBnClickedRadioCommonSub()
{
	if(m_rdoCommon.GetCheck())	SetPageControlls(FALSE);
	else						SetPageControlls(TRUE);
}

void CdlgTanaoroshi::OnBnClickedRadioIndiv()
{
	if(m_rdoIndiv.GetCheck())	SetPageControlls(TRUE);
	else						SetPageControlls(FALSE);
}

void CdlgTanaoroshi::SetPageControlls(BOOL fFlag)
{
// midori 160612 cor -->
//	if(m_rdoCommon.IsWindowEnabled() == FALSE)	fFlag = FALSE;
// ---------------------
	if(m_cmbPrint.GetCurSel() != 2)	fFlag = FALSE;
// midori 160612 cor <--

	//m_txtCurrent.EnableWindow(fFlag);
	//m_txtMax.EnableWindow(fFlag);

	if(fFlag == FALSE){
		m_btnMovePrev.EnableWindow(fFlag);
		m_btnMoveNext.EnableWindow(fFlag);
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, FALSE);
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F9, FALSE);
	}
	else{
		SetMoveButtonState();
		SetToolBarState();
	}
}

void CdlgTanaoroshi::SetMoveButtonState()
{
	SetMovePrevButtonState();
	SetMoveNextButtonState();
}

// ページ戻りボタン状態設定
void CdlgTanaoroshi::SetMovePrevButtonState()
{
	BOOL state = FALSE;
	//if(m_rdoCommon.IsWindowEnabled() == FALSE)	state = FALSE;
	//else{
		if(m_CurrentPage == 1)	state = FALSE;
		else					state = TRUE;
	//}

	m_btnMovePrev.EnableWindow(state);
}

// ページ送りボタン状態設定
void CdlgTanaoroshi::SetMoveNextButtonState()
{
	BOOL state = FALSE;
	//if(m_rdoCommon.IsWindowEnabled() == FALSE)	state = FALSE;
	//else{
		if(m_CurrentPage == m_MaxPage)	state = FALSE;
		else							state = TRUE;
	//}

	m_btnMoveNext.EnableWindow(state);
}

void CdlgTanaoroshi::SetToolBarState()
{
	if(m_rdoIndiv.IsWindowEnabled() == FALSE){
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, FALSE);
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F9, FALSE);
	}
	else{
		if(m_rdoIndiv.GetCheck()){
			if(m_MaxPage == 1)	m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, FALSE);
			else				m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, TRUE);

			m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F9, TRUE);
		}
		else{
			m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, FALSE);
			m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F9, FALSE);
		}
	}
}

void CdlgTanaoroshi::SetMethod(BYTE method)
{
	m_rdoA.SetCheck(FALSE);
	m_rdoB.SetCheck(FALSE);
	m_rdoC.SetCheck(FALSE);
	m_rdoD.SetCheck(FALSE);
	switch(method){
		//	Ａ　実施棚卸
		case DT_METHOD_A:
			m_rdoA.SetCheck( TRUE ); 
			break;
			//	Ｂ　帳簿棚卸
		case DT_METHOD_B:
			m_rdoB.SetCheck( TRUE ); 
			break;
			//	Ｃ　ＡとＢとの併用
		case DT_METHOD_C:
			m_rdoC.SetCheck( TRUE ); 
			break;
			//	Ｄ　登録名称を印字
		case DT_METHOD_D:
			m_rdoD.SetCheck( TRUE ); 
			break;
			//	例外
		default:
			break;
	}
}

BYTE CdlgTanaoroshi::GetMethod()
{
	BYTE ret = DT_METHOD_NOTHING;
	//	Ａ　実施棚卸がチェックされている？
	if ( m_rdoA.GetCheck() == TRUE )		ret = DT_METHOD_A;
	//	Ｂ　帳簿棚卸がチェックされている？
	else if ( m_rdoB.GetCheck() == TRUE )	ret = DT_METHOD_B;
	//	Ｃ　ＡとＢとの併用がチェックされている？
	else if ( m_rdoC.GetCheck() == TRUE )	ret = DT_METHOD_C;
	//	Ｄ　登録名称を印字がチェックされている？
	else if ( m_rdoD.GetCheck() == TRUE )	ret = DT_METHOD_D;

	return ret;
}

void CdlgTanaoroshi::MovePage(int page)
{
	m_CurrentPage = page;

	Disp(page);

	SetMoveButtonState();

	SetRadioButton();

	if(m_rdoCommon.GetCheck())	SetPageControlls(FALSE);
	else						SetPageControlls(TRUE);
}

void CdlgTanaoroshi::SetControl(const CdbUc052Tanaoroshi2& mfcRec)
{
	BOOL isPrint = FALSE;
	//	印字する？
	if ( mfcRec.m_FgShow == ID_FGSHOW_TANAOROSHI )	isPrint = FALSE;
	else											isPrint = TRUE;

	//	印字するのチェック
	m_chkPrint.SetCheck(isPrint);

	//	期末棚卸の方法で分岐
	SetMethod(mfcRec.m_Method);

	//　コンボボックスのデータセット
	CConvOutRangai cr;
	int PrintPage = 0;
	int PrintType = 0;
	cr.ToMemory(mfcRec.m_OutRangai, PrintPage, PrintType);
	m_cmbPrint.SetCurSel(PrintPage);
	if(PrintType){
		m_rdoCommon.SetCheck(FALSE);
		m_rdoIndiv.SetCheck(TRUE);
	}
	else{
		m_rdoCommon.SetCheck(TRUE);
		m_rdoIndiv.SetCheck(FALSE);
	}
}

void CdlgTanaoroshi::AddPage()
{
	RangaiTana rt;
	rt.Method = DT_METHOD_D;	// デフォルトはＤ　登録名称を印字
	// 末尾に追加
	m_vecRangai.push_back(rt);

	m_MaxPage = m_vecRangai.size();
}

void CdlgTanaoroshi::SyncPage()
{
	if(m_PageMaxFrom > m_MaxPage){			// 現在の欄外ページより棚卸資産の内訳書のページが多い場合は空白ページを埋める
		int diff = m_PageMaxFrom - m_MaxPage;
		for(int i=0; i<diff; i++){
			AddPage();
		}
	}
	else if(m_PageMaxFrom < m_MaxPage){		// 現在の欄外ページより棚卸資産の内訳書のページが少ない場合は末尾から削除
		int diff = m_MaxPage - m_PageMaxFrom;
		for(int i=0; i<diff; i++){
			// 末尾を削除
			m_vecRangai.pop_back();

			// 要素がなくなったら一つ新規追加（1ページしかないときはF7をDisableにしているので入ってこないとは思うが）
			if(m_vecRangai.size() == 0){
				AddPage();
				return;
			}
		}
		m_MaxPage = m_vecRangai.size();
	}

	OnBnClickedRadioCommonSub();
}

// 修正依頼No150520 1p,2pともDに設定（登録名称はまだ空）し、1pに文字を入力後、PageDownでページ切替
// その直後バックスペースで(iChar >= 0) && (iChar <= GetLength())のアサーションが出ていた不具合の修正
LRESULT CdlgTanaoroshi::SetCursorBytePosHandler(WPARAM w, LPARAM l)
{
	int page = (int)w;
	RangaiTana rt = m_vecRangai[page - 1];

	int len = rt.HdName.GetLength();
	m_txtName.SetCursorBytePos(len);

	return 1;
}