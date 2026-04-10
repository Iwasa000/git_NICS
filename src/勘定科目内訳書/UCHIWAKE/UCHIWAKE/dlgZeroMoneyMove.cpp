// dlgZeroMoneyMove.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgZeroMoneyMove.h"


// CdlgZeroMoneyMove ダイアログ

IMPLEMENT_DYNAMIC(CdlgZeroMoneyMove, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgZeroMoneyMove::CdlgZeroMoneyMove(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgZeroMoneyMove::IDD, pParent)
{
	// 初期化
	for( int i = 0; i < UC_ID_FORMNO_MAX; i++ ){
		ZeroMemory( &m_HokanData[i], sizeof(HOKAN_DATA) );
	}
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgZeroMoneyMove::~CdlgZeroMoneyMove()
{
}

void CdlgZeroMoneyMove::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_YOUSHIKI_01, m_chkYoshiki011);
	//DDX_Control(pDX, IDC_YOUSHIKI_02, m_chkYoshiki021);
	//DDX_Control(pDX, IDC_YOUSHIKI_03, m_chkYoshiki031);
	//DDX_Control(pDX, IDC_YOUSHIKI_04_01, m_chkYoshiki041);
	//DDX_Control(pDX, IDC_YOUSHIKI_04_02, m_chkYoshiki042);
	//DDX_Control(pDX, IDC_YOUSHIKI_05, m_chkYoshiki051);
	//DDX_Control(pDX, IDC_YOUSHIKI_06, m_chkYoshiki061);
	//DDX_Control(pDX, IDC_YOUSHIKI_07, m_chkYoshiki071);
	//DDX_Control(pDX, IDC_YOUSHIKI_08, m_chkYoshiki081);
	//DDX_Control(pDX, IDC_YOUSHIKI_09, m_chkYoshiki091);
	//DDX_Control(pDX, IDC_YOUSHIKI_10_01, m_chkYoshiki101);
	//DDX_Control(pDX, IDC_YOUSHIKI_10_02, m_chkYoshiki102);
	//DDX_Control(pDX, IDC_YOUSHIKI_11, m_chkYoshiki111);
	//DDX_Control(pDX, IDC_YOUSHIKI_12, m_chkYoshiki121);
	//DDX_Control(pDX, IDC_YOUSHIKI_13, m_chkYoshiki131);
	//DDX_Control(pDX, IDC_YOUSHIKI_14_01, m_chkYoshiki141);
	//DDX_Control(pDX, IDC_YOUSHIKI_14_02, m_chkYoshiki142);
	//DDX_Control(pDX, IDC_YOUSHIKI_15_01, m_chkYoshiki151);
	//DDX_Control(pDX, IDC_YOUSHIKI_15_02, m_chkYoshiki152);
	//DDX_Control(pDX, IDC_YOUSHIKI_15_03, m_chkYoshiki153);
	//DDX_Control(pDX, IDC_YOUSHIKI_16_01, m_chkYoshiki161);
	//DDX_Control(pDX, IDC_YOUSHIKI_16_02, m_chkYoshiki162);
	//DDX_Control(pDX, IDC_YOUSHIKI_171, m_chkYoshiki171);
	//DDX_Control(pDX, IDC_YOUSHIKI_172, m_chkYoshiki172);
	//DDX_Control(pDX, IDC_YOUSHIKI_173, m_chkYoshiki173);
	//DDX_Control(pDX, IDC_YOUSHIKI_174, m_chkYoshiki174);
	//DDX_Control(pDX, IDC_YOUSHIKI_175, m_chkYoshiki175);
	//DDX_Control(pDX, IDC_YOUSHIKI_181, m_chkYoshiki181);
	//DDX_Control(pDX, IDC_YOUSHIKI_182, m_chkYoshiki182);
	//DDX_Control(pDX, IDC_YOUSHIKI_183, m_chkYoshiki183);
	//DDX_Control(pDX, IDC_YOUSHIKI_184, m_chkYoshiki184);
	//DDX_Control(pDX, IDC_YOUSHIKI_185, m_chkYoshiki185);

	//DDX_Control(pDX, IDC_CLEAR_ZERO_01, m_chkZero011);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_02, m_chkZero021);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_03, m_chkZero031);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_04_01, m_chkZero041);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_04_02, m_chkZero042);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_05, m_chkZero051);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_06, m_chkZero061);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_07, m_chkZero071);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_08, m_chkZero081);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_09, m_chkZero091);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_10_01, m_chkZero101);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_10_02, m_chkZero102);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_11, m_chkZero111);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_12, m_chkZero121);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_13, m_chkZero131);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_14_01, m_chkZero141);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_14_02, m_chkZero142);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_15_01, m_chkZero151);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_15_02, m_chkZero152);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_15_03, m_chkZero153);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_16_01, m_chkZero161);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_16_02, m_chkZero162);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_171, m_chkZero171);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_172, m_chkZero172);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_173, m_chkZero173);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_174, m_chkZero174);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_175, m_chkZero175);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_181, m_chkZero181);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_182, m_chkZero182);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_183, m_chkZero183);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_184, m_chkZero184);
	//DDX_Control(pDX, IDC_CLEAR_ZERO_185, m_chkZero185);

	//DDX_Control(pDX, IDC_CLEAR_NULL_01, m_chkNull011);
	//DDX_Control(pDX, IDC_CLEAR_NULL_02, m_chkNull021);
	//DDX_Control(pDX, IDC_CLEAR_NULL_03, m_chkNull031);
	//DDX_Control(pDX, IDC_CLEAR_NULL_04_01, m_chkNull041);
	//DDX_Control(pDX, IDC_CLEAR_NULL_04_02, m_chkNull042);
	//DDX_Control(pDX, IDC_CLEAR_NULL_05, m_chkNull051);
	//DDX_Control(pDX, IDC_CLEAR_NULL_06, m_chkNull061);
	//DDX_Control(pDX, IDC_CLEAR_NULL_07, m_chkNull071);
	//DDX_Control(pDX, IDC_CLEAR_NULL_08, m_chkNull081);
	//DDX_Control(pDX, IDC_CLEAR_NULL_09, m_chkNull091);
	//DDX_Control(pDX, IDC_CLEAR_NULL_10_01, m_chkNull101);
	//DDX_Control(pDX, IDC_CLEAR_NULL_10_02, m_chkNull102);
	//DDX_Control(pDX, IDC_CLEAR_NULL_11, m_chkNull111);
	//DDX_Control(pDX, IDC_CLEAR_NULL_12, m_chkNull121);
	//DDX_Control(pDX, IDC_CLEAR_NULL_13, m_chkNull131);
	//DDX_Control(pDX, IDC_CLEAR_NULL_14_01, m_chkNull141);
	//DDX_Control(pDX, IDC_CLEAR_NULL_14_02, m_chkNull142);
	//DDX_Control(pDX, IDC_CLEAR_NULL_15_01, m_chkNull151);
	//DDX_Control(pDX, IDC_CLEAR_NULL_15_02, m_chkNull152);
	//DDX_Control(pDX, IDC_CLEAR_NULL_15_03, m_chkNull153);
	//DDX_Control(pDX, IDC_CLEAR_NULL_16_01, m_chkNull161);
	//DDX_Control(pDX, IDC_CLEAR_NULL_16_02, m_chkNull162);
	//DDX_Control(pDX, IDC_CLEAR_NULL_171, m_chkNull171);
	//DDX_Control(pDX, IDC_CLEAR_NULL_172, m_chkNull172);
	//DDX_Control(pDX, IDC_CLEAR_NULL_173, m_chkNull173);
	//DDX_Control(pDX, IDC_CLEAR_NULL_174, m_chkNull174);
	//DDX_Control(pDX, IDC_CLEAR_NULL_175, m_chkNull175);
	//DDX_Control(pDX, IDC_CLEAR_NULL_181, m_chkNull181);
	//DDX_Control(pDX, IDC_CLEAR_NULL_182, m_chkNull182);
	//DDX_Control(pDX, IDC_CLEAR_NULL_183, m_chkNull183);
	//DDX_Control(pDX, IDC_CLEAR_NULL_184, m_chkNull184);
	//DDX_Control(pDX, IDC_CLEAR_NULL_185, m_chkNull185);

	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_HYOCHK_BUTTON, m_btnHyoChk);
	DDX_Control(pDX, ID_CLEARZEROCHK_BUTTON, m_btnZeroChk);
	DDX_Control(pDX, ID_CLEARNULLCHK_BUTTON, m_btnNullChk);
	DDX_Control(pDX, IDC_CUSTOM_REPORT_ZERO_MOVE, m_report);
}


BEGIN_MESSAGE_MAP(CdlgZeroMoneyMove, ICSDialog)
	
	//ON_BN_CLICKED(IDC_YOUSHIKI_01, &CdlgZeroMoneyMove::OnBnClickedYoushiki01)
	//ON_BN_CLICKED(IDC_YOUSHIKI_02, &CdlgZeroMoneyMove::OnBnClickedYoushiki02)
	//ON_BN_CLICKED(IDC_YOUSHIKI_03, &CdlgZeroMoneyMove::OnBnClickedYoushiki03)
	//ON_BN_CLICKED(IDC_YOUSHIKI_04_01, &CdlgZeroMoneyMove::OnBnClickedYoushiki0401)
	//ON_BN_CLICKED(IDC_YOUSHIKI_04_02, &CdlgZeroMoneyMove::OnBnClickedYoushiki0402)
	//ON_BN_CLICKED(IDC_YOUSHIKI_05, &CdlgZeroMoneyMove::OnBnClickedYoushiki05)
	//ON_BN_CLICKED(IDC_YOUSHIKI_06, &CdlgZeroMoneyMove::OnBnClickedYoushiki06)
	//ON_BN_CLICKED(IDC_YOUSHIKI_07, &CdlgZeroMoneyMove::OnBnClickedYoushiki07)
	//ON_BN_CLICKED(IDC_YOUSHIKI_08, &CdlgZeroMoneyMove::OnBnClickedYoushiki08)
	//ON_BN_CLICKED(IDC_YOUSHIKI_09, &CdlgZeroMoneyMove::OnBnClickedYoushiki09)
	//ON_BN_CLICKED(IDC_YOUSHIKI_10_01, &CdlgZeroMoneyMove::OnBnClickedYoushiki1001)
	//ON_BN_CLICKED(IDC_YOUSHIKI_10_02, &CdlgZeroMoneyMove::OnBnClickedYoushiki1002)
	//ON_BN_CLICKED(IDC_YOUSHIKI_11, &CdlgZeroMoneyMove::OnBnClickedYoushiki11)
	//ON_BN_CLICKED(IDC_YOUSHIKI_12, &CdlgZeroMoneyMove::OnBnClickedYoushiki12)
	//ON_BN_CLICKED(IDC_YOUSHIKI_13, &CdlgZeroMoneyMove::OnBnClickedYoushiki13)
	//ON_BN_CLICKED(IDC_YOUSHIKI_14_01, &CdlgZeroMoneyMove::OnBnClickedYoushiki1401)
	//ON_BN_CLICKED(IDC_YOUSHIKI_14_02, &CdlgZeroMoneyMove::OnBnClickedYoushiki1402)
	//ON_BN_CLICKED(IDC_YOUSHIKI_15_01, &CdlgZeroMoneyMove::OnBnClickedYoushiki1501)
	//ON_BN_CLICKED(IDC_YOUSHIKI_15_02, &CdlgZeroMoneyMove::OnBnClickedYoushiki1502)
	//ON_BN_CLICKED(IDC_YOUSHIKI_15_03, &CdlgZeroMoneyMove::OnBnClickedYoushiki1503)
	//ON_BN_CLICKED(IDC_YOUSHIKI_16_01, &CdlgZeroMoneyMove::OnBnClickedYoushiki1601)
	//ON_BN_CLICKED(IDC_YOUSHIKI_16_02, &CdlgZeroMoneyMove::OnBnClickedYoushiki1602)
	//ON_BN_CLICKED(IDC_YOUSHIKI_171, &CdlgZeroMoneyMove::OnBnClickedYoushiki171)
	//ON_BN_CLICKED(IDC_YOUSHIKI_172, &CdlgZeroMoneyMove::OnBnClickedYoushiki172)
	//ON_BN_CLICKED(IDC_YOUSHIKI_173, &CdlgZeroMoneyMove::OnBnClickedYoushiki173)
	//ON_BN_CLICKED(IDC_YOUSHIKI_174, &CdlgZeroMoneyMove::OnBnClickedYoushiki174)
	//ON_BN_CLICKED(IDC_YOUSHIKI_175, &CdlgZeroMoneyMove::OnBnClickedYoushiki175)
	//ON_BN_CLICKED(IDC_YOUSHIKI_181, &CdlgZeroMoneyMove::OnBnClickedYoushiki181)
	//ON_BN_CLICKED(IDC_YOUSHIKI_182, &CdlgZeroMoneyMove::OnBnClickedYoushiki182)
	//ON_BN_CLICKED(IDC_YOUSHIKI_183, &CdlgZeroMoneyMove::OnBnClickedYoushiki183)
	//ON_BN_CLICKED(IDC_YOUSHIKI_184, &CdlgZeroMoneyMove::OnBnClickedYoushiki184)
	//ON_BN_CLICKED(IDC_YOUSHIKI_185, &CdlgZeroMoneyMove::OnBnClickedYoushiki185)
	ON_BN_CLICKED(ID_HYOCHK_BUTTON, &CdlgZeroMoneyMove::OnBnClickedHyochkButton)
	ON_BN_CLICKED(ID_CLEARZEROCHK_BUTTON, &CdlgZeroMoneyMove::OnBnClickedZerochkButton)
	ON_BN_CLICKED(ID_CLEARNULLCHK_BUTTON, &CdlgZeroMoneyMove::OnBnClickedNullchkButton)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgZeroMoneyMove::OnBnClickedOkButton)

	ON_NOTIFY(ICS_NM_REPORT_CHECKED, IDC_CUSTOM_REPORT_ZERO_MOVE, OnNMReportCheckedCUSTOM_REPORT_ZERO_MOVE)
END_MESSAGE_MAP()


// CdlgZeroMoneyMove メッセージ ハンドラ

//+++++++++++++++++++++++++++++++++++++++++++
// ダイアログ起動時のイベント
//+++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgZeroMoneyMove::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	InitReportHeader();

	CreateZeroMoveVec();

	AddRecords();

	SetData();

	m_report.SetFocus();
	m_report.SetFocusedRow(0);

	// リサイズ
	OnInitDialogEX();

	// ダイアログを画面の作業領域の中央に表示
	// 解像度によってはタスクバーにはみ出る形で表示されるため(例：1366×768)
	//m_clsFunc.DlgMoveCenter( this );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（帳表全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyMove::OnBnClickedHyochkButton()
{
	m_clsFunc.ChangeReportColumnAll(&m_report, zm_select);

	int row = 0;
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin();it != m_vecZeroMoveData.end(); it++, row++){
// midori 151393 cor -->
//		if((*it).isEnable == FALSE)	continue;
// ---------------------
		if((*it).isEnable == FALSE)	{
			// レコードが入力不可の場合、チェックをOFFにする
			m_report.SetChecked(row, zm_select, FALSE);
			continue;
		}
// midori 151393 cor <--

		BOOL isCheck = m_report.IsChecked(row, zm_select);
		SetCheckBoxState(row, isCheck);
	}

	//ReDrawRowColor(m_report.IsChecked(0, zm_select));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（０円移動全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyMove::OnBnClickedZerochkButton()
{
// midori 151393 add -->
	int	row = 0;
// midori 151393 add <--

	m_clsFunc.ChangeReportColumnAll(&m_report, zm_0clear);
// midori 151393 add -->
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin();it != m_vecZeroMoveData.end(); it++, row++){
		// レコードが入力不可又は選択にチェックが付いていない場合、０円移動全選択/解除ボタンのチェックをOFFにする
		if((*it).isEnable == FALSE || m_report.IsChecked(row,zm_select) == FALSE)	m_report.SetChecked(row, zm_0clear, FALSE);
	}
// midori 151393 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（空欄移動全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyMove::OnBnClickedNullchkButton()
{
// midori 151393 add -->
	int	row = 0;
// midori 151393 add <--

	m_clsFunc.ChangeReportColumnAll(&m_report, zm_null_clear);
// midori 151393 add -->
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin();it != m_vecZeroMoveData.end(); it++, row++){
		// レコードが入力不可又は選択にチェックが付いていない場合、空欄移動全選択/解除ボタンのチェックをOFFにする
		if((*it).isEnable == FALSE || m_report.IsChecked(row,zm_select) == FALSE)	m_report.SetChecked(row, zm_null_clear, FALSE);
	}
// midori 151393 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyMove::OnBnClickedOkButton()
{
	// レポートコントロールの状態をベクタに更新
	UpdateZeroMoveData();

	// 選択カラムが1つ以上チェックされてる？
	if(GetSelectCheckNum() < 1){
		ICSMessageBox( _T("様式が１件も選択されていません。"), MB_OK, 0, 0, this );
		m_report.SetFocus();
		return;
	}
	
	// チェック結果を保持
	SetCheckData();

	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyMove::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgZeroMoneyMove::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	int	nID;

	//	フォーカスのあるコントロールのID取得
	nID = m_clsFunc.GetFocusControlID( this );

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

		//	F2キーが押された
		case VK_F2:

			//	F2キーをShift + Tabイベントに書き換え
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );									//	抜ける
			break;
		case VK_SPACE:
		case VK_RETURN:
			if(nID == IDC_CUSTOM_REPORT_ZERO_MOVE){
				UpdateStateFromKey((int)pMsg->wParam);
				return 1;
			}
			break;
		// ↑キー
		//case VK_UP:
		//	if ( SetMoveControlFocus( nID , VK_UP ) > 0 ){
		//		return( 1 );
		//	}
		//	break;

		//// ↓キー
		//case VK_DOWN:
		//	if ( SetMoveControlFocus( nID , VK_DOWN ) > 0 ){
		//		return( 1 );
		//	}
		//	break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
//	指定帳表データ有無チェック処理
//	【引数】	intFormSeq	…	帳表FormSeq番号
//	【戻値】	TRUE		…	空行/頁計/累計以外の行あり
//				FALSE		…	空行/頁計/累計行のみ
//**************************************************
BOOL CdlgZeroMoneyMove::CheckYousikiData( int intFormSeq )
{
	CdbUc000Common* rsData = NULL;

// midori 152389 add -->
	// 人件費の内訳書はデータの有無に関係なく対象外とする
	if(intFormSeq == ID_FORMNO_142)	{
		return(FALSE);
	}
// midori 152389 add <--

	// ＤＢオブジェクト取得
	rsData = m_clsFunc.CreateDbObject(intFormSeq, m_pDB);

	// データ取得
// midori 160607 cor -->
	//if ( rsData->GetCountDataRecord() == 0 ){
// ---------------------
	if ( rsData->GetCountDataRecord(1) == 0 ){
// midori 160607 cor <--

		rsData->Fin();
		delete rsData;

		return FALSE;
	}

	rsData->Fin();
	delete rsData;

	return TRUE;
}

//**************************************************
//	チェックボックスの選択結果を格納
//	【引数】	なし
//	【戻値】	TRUE	…	成功
//				FALSE	…	失敗
//**************************************************
BOOL CdlgZeroMoneyMove::SetCheckData()
{
	int intFormSeq = 0;
	int cnt = 0;
	CdbUcInfSub mfcSub(m_pDB);

	// FormOrder昇順でクエリ
	mfcSub.InitOrder();
	mfcSub.MoveFirst();

	// 全帳表分ループを回し、その中でベクタに登録されている様式のみチェックボックスの値を更新する
	vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin();
	while(!mfcSub.IsEOF()){
		if(it == m_vecZeroMoveData.end())	break;

		// 様式シーケンスは必ずセット
		m_HokanData[cnt].intFormSeq = mfcSub.m_FormSeq;

		if(m_HokanData[cnt].intFormSeq == (*it).FormSeq){
			if((*it).is_select != FALSE && ((*it).isEnable != FALSE)){
				m_HokanData[cnt].blnZeroFg = (*it).is0_clear;
				m_HokanData[cnt].blnNullFg = (*it).isNULL_clear;
			}
			it++;
		}

		cnt++;
		mfcSub.MoveNext();
	}
	mfcSub.Fin();

	return TRUE;
}

//**************************************************
//	ダイアログ表示
//	【引数】	intFormSeq		…	ダイアログを呼び出した際のカレント帳表
//				pDB				…	データベースハンドル
//	【戻値】	ID_DLG_OK		…	印刷ボタンが押された
//				ID_DLG_CANCEL	…	キャンセルボタンが押された
//**************************************************
int CdlgZeroMoneyMove::ShowDialog( int intFormSeq, CDatabase* pDB )
{
	m_nRet = ID_DLG_CANCEL;	//	戻値初期化
	m_nFormSeq = ID_FORMNO_011;

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;	//	データベースハンドル取得
				m_nFormSeq = intFormSeq;		//	様式番号取得
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
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgZeroMoneyMove::InitReportHeader()
{
	//m_report.Resize( TRUE, ICSResizeDefault );
	
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
	m_report.AddColumn(new ICSReportColumn(0, NULL, 1));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 2));
	m_report.AddColumn(new ICSReportColumn(2, NULL, 10));
	m_report.AddColumn(new ICSReportColumn(3, NULL, 3));
	m_report.AddColumn(new ICSReportColumn(4, NULL, 3));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	ICSReportColumns *m_pClms = m_report.GetColumns();
	int cnt = sizeof(strZeroMoveHeader) / sizeof(strZeroMoveHeader[0]);
	for(int i = 0; i < cnt; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		m_pClm->SetCaption(strZeroMoveHeader[i]);
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
int CdlgZeroMoneyMove::AddRecords()
{
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin(); it != m_vecZeroMoveData.end(); it++){
		ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 選択
		pRecord->AddItem( new ICSReportRecordItemText(it->TitleNo));	// 様式番号
		pRecord->AddItem( new ICSReportRecordItemText(it->Title));		// 様式名称
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 金額０円行を保管へ移動
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 金額空欄行を保管へ移動
	}
	// 行の高さ
	m_report.SetItemHeight(20);

	return TRUE;
}

//**************************************************
//	レコードのチェックボックスにデータセット
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgZeroMoneyMove::SetData()
{
	int row = 0;
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin(); it != m_vecZeroMoveData.end(); it++, row++){
		// データのない行は背景色も更新
		if((*it).isEnable == FALSE){
			SetCheckBoxState(row, FALSE);
			// 背景色更新
			SetColor(row, (*it).isEnable);
			// データがない行は「選択」カラムも選択不可
			m_report.AllowEditItem(row, zm_select, FALSE);
		}
		else{
			BOOL isFromFormSeq = FALSE;	// 現在入力中の様式かどうかかどうか
			if(m_nFormSeq == (*it).FormSeq)		isFromFormSeq = TRUE;
			m_report.SetChecked(row, zm_select,isFromFormSeq);
			SetCheckBoxState(row, isFromFormSeq);	// 入力中の様式のみ、チェック可
		}

// midori 151393 cor -->
//		m_report.SetChecked(row, zm_0clear,TRUE);
//		m_report.SetChecked(row, zm_null_clear,TRUE);
// ---------------------
		// チェックの付いている様式のみ金額０円行、金額空欄行にチェックを付ける
		if(m_report.IsChecked(row,zm_select) == TRUE)	{
			m_report.SetChecked(row, zm_0clear,TRUE);
			m_report.SetChecked(row, zm_null_clear,TRUE);
		}
// midori 151393 cor <--
	}
}

void CdlgZeroMoneyMove::CreateZeroMoveVec()
{
	CdbUcInfSub	mfcRec(m_pDB);
	mfcRec.InitOrder();
	mfcRec.MoveFirst();
	while(!mfcRec.IsEOF()){
		ZERO_MONEY_DATA	zero_money_data;
		zero_money_data.FormSeq = mfcRec.m_FormSeq;
		zero_money_data.TitleNo = mfcRec.m_TitleNo + mfcRec.m_TitleNo2;
		zero_money_data.Title	= mfcRec.m_Title;

		BOOL hasData = CheckYousikiData(mfcRec.m_FormSeq);
		if(hasData != NULL){	// データがなければ選択不可
			zero_money_data.isEnable = TRUE;
		}

		// ベクタの最後尾に追加
		m_vecZeroMoveData.push_back(zero_money_data);
		// 次の様式へ
		mfcRec.MoveNext();
	}
}

void CdlgZeroMoneyMove::SetCheckBoxState(int indexRow, BOOL state)
{
	m_report.AllowEditItem(indexRow, zm_0clear, state);
	m_report.AllowEditItem(indexRow, zm_null_clear, state);
}

void CdlgZeroMoneyMove::OnNMReportCheckedCUSTOM_REPORT_ZERO_MOVE(NMHDR * pNotifyStruct, LRESULT * result)
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

	if(indexColumn != zm_select)		return;
	//if(IsExistData(indexRow)  == FALSE)	return;

	// チェックボックス更新
	SetCheckBoxState(indexRow, bCheck);
	// 背景色更新
	//SetColor(indexRow, bCheck);

	m_report.RedrawControl();
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				check	…	選択チェックボックスの状態
//	【戻値】	なし
//**************************************************
void CdlgZeroMoneyMove::SetColor( int nIndex, BOOL check)
{
	if(check == FALSE)	m_report.SetRowBackColor(nIndex, GetSysColor( COLOR_BTNFACE ), FALSE);
	//if(check != FALSE)		m_report.SetRowBackColor(nIndex, GetSysColor( COLOR_WINDOW ), FALSE);
	//else					m_report.SetRowBackColor(nIndex, GetSysColor( COLOR_BTNFACE ), FALSE);
}

//**************************************************
//	全ての行の背景色を更新
//	【引数】	BOOL	チェックボックスON or OFF
//	【戻値】	なし
//**************************************************
void CdlgZeroMoneyMove::ReDrawRowColor(BOOL check)
{
	int row = 0;
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin(); it != m_vecZeroMoveData.end(); it++){
		//SetColor(row, check);
		row++;
	}

	m_report.RedrawControl();
}

void CdlgZeroMoneyMove::UpdateStateFromKey(int key)
{
	if(m_clsFunc.GetFocusControlID(this) != IDC_CUSTOM_REPORT_ZERO_MOVE)	return;

	ICSReportSelectedRows* s_rows = m_report.GetSelectedRows();
	if(s_rows == NULL)	return;
	ICSReportRow* r_row = s_rows->GetAt(0);
	if(r_row == NULL)	return;
	int indexRow = r_row->GetIndex();
	BOOL check = m_report.IsChecked(indexRow, zm_select);
	ICSReportRecordItem	*pItem	= m_report.GetActiveItem();

	switch(key){
		case VK_SPACE:
			if(IsExistData(indexRow) == FALSE)	return;	// データのない様式はチェックを入れさせない

			// 選択カラムの状態反転
			check = !check;
			if(pItem == NULL){
				m_report.SetChecked(indexRow, zm_select, check);

				// チェックボックス更新
				SetCheckBoxState(indexRow, check);
				// 背景色更新
				//SetColor(indexRow, check);
				m_report.RedrawControl();
			}
			else{
				int indexClm = pItem->GetIndex();
				if(indexClm == zm_select){
					m_report.SetChecked(indexRow, zm_select, check);
					// チェックボックス更新
					SetCheckBoxState(indexRow, check);
					// 背景色更新
					//SetColor(indexRow, check);
					m_report.RedrawControl();
				}
				else if(indexClm == zm_0clear || indexClm == zm_null_clear){
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
				MoveDown(indexRow);
				return;
			}
			
			if(pItem != NULL){	// エディットが入っている→次のカラムにエディット
				int indexClm = pItem->GetIndex();
				if(indexClm == zm_select){
					indexClm = zm_0clear;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
				else if(indexClm == zm_0clear){
					indexClm = zm_null_clear;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
				else if(indexClm == zm_null_clear){	// 空欄カラムなら次の行にエディット
					MoveDown(indexRow);
					ICSReportRecords * records = m_report.GetRecords();
					int max_cnt = records->GetCount();
					if(indexRow+1<max_cnt)		m_report.EditItem( indexRow + 1, zm_select, TRUE );
				}
			}
			else{	// エディットが入っていない→選択カラムにエディット
				m_report.EditItem( indexRow, zm_select, TRUE );
			}
	}
}

void CdlgZeroMoneyMove::UpdateZeroMoveData()
{
	ICSReportRecords * records = m_report.GetRecords();
	if(records == NULL)	return;
	int max_cnt = records->GetCount();

	for(int row=0; row<max_cnt; row++){
		m_vecZeroMoveData[row].is_select = m_report.IsChecked(row, zm_select);
		m_vecZeroMoveData[row].is0_clear = m_report.IsChecked(row, zm_0clear);
		m_vecZeroMoveData[row].isNULL_clear = m_report.IsChecked(row, zm_null_clear);
	}
}

int CdlgZeroMoneyMove::GetSelectCheckNum()
{
	int cnt = 0;
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin();it != m_vecZeroMoveData.end(); it++){
		if(it->is_select != FALSE){
			cnt++;
		}
	}
	return cnt;
}

void CdlgZeroMoneyMove::SetGrid()
{
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
	arryGridColumn.Add(2);
	arryGridColumn.Add(3);

	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));

	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}

BOOL CdlgZeroMoneyMove::IsExistData(int row)
{
	try{
		ZERO_MONEY_DATA zmd = m_vecZeroMoveData.at(row);
		return zmd.isEnable;
	}
	catch(...){
		return FALSE;
	}

	return FALSE;
}

void CdlgZeroMoneyMove::MoveDown(int cur_row)
{
	int cnt = 0;
	for(vector<ZERO_MONEY_DATA>::iterator it = m_vecZeroMoveData.begin();it != m_vecZeroMoveData.end(); it++, cnt++){
		if(cnt < cur_row)				continue;
		if(IsExistData(cnt+1) == FALSE)	continue;

		m_report.SetFocusedRow(cnt+1);
		break;
	}
}