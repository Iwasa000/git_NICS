// dlgYokukiMain.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgYokukiMain.h"


// CdlgYokukiMain ダイアログ

IMPLEMENT_DYNAMIC(CdlgYokukiMain, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgYokukiMain::CdlgYokukiMain(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgYokukiMain::IDD, pParent)
{

}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgYokukiMain::~CdlgYokukiMain()
{
	vecInfoYokuki.clear();
}

void CdlgYokukiMain::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_HYOCHK_BUTTON, m_btnHyoChk);
	DDX_Control(pDX, ID_CLEARCHK_BUTTON, m_btnClearChk);
// midori 152387 add -->
	DDX_Control(pDX, ID_NULLCLEARCHK_BUTTON, m_btnNullClearChk);
// midori 152387 add <--
	DDX_Control(pDX, ID_HOKANCHK_BUTTON, m_btnHokanChk);
	DDX_Control(pDX, IDC_CUSTOM_REPORT2, m_report);
}


BEGIN_MESSAGE_MAP(CdlgYokukiMain, ICSDialog)
	ON_BN_CLICKED(ID_HYOCHK_BUTTON, &CdlgYokukiMain::OnBnClickedHyochkButton)
	ON_BN_CLICKED(ID_CLEARCHK_BUTTON, &CdlgYokukiMain::OnBnClickedClearchkButton)
	ON_BN_CLICKED(ID_HOKANCHK_BUTTON, &CdlgYokukiMain::OnBnClickedHokanchkButton)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgYokukiMain::OnBnClickedOkButton)
	ON_NOTIFY(ICS_NM_REPORT_CHECKED, IDC_CUSTOM_REPORT2, OnNMReportCheckedCUSTOM_REPORT2)
// midori 152387 add -->
	ON_BN_CLICKED(ID_NULLCLEARCHK_BUTTON, &CdlgYokukiMain::OnBnClickedNullclearchkButton)
// midori 152387 add <--
END_MESSAGE_MAP()


// CdlgYokukiMain メッセージ ハンドラ

BOOL CdlgYokukiMain::OnInitDialog()
{
	ICSDialog::OnInitDialog();

// midori 152387 del -->
//// midori 161103 add -->
//	// 画面名称を変更する
//	if(m_flg == 0)	{
//		this->SetWindowText("翌期更新");
//		this->GetDlgItem(IDC_STATIC1)->SetWindowText(_T("\n勘定科目内訳書の翌期更新を行います。"));
//		this->GetDlgItem(IDC_STATIC_HOKAN_MSG)->SetWindowText("「金額等を０クリア」について\n チェックを付けて更新すると、その様式のデータはすべて金額が０円になります。\n「データを保管へ移動」について\n チェックを付けて更新すると、その様式のデータはすべて保管データになり入力画面には表示されません。\n 保管されたデータは、メニューバーの保管の保管参照から入力画面に戻すことができます。");
//	}
//	else	{
//		this->SetWindowText("");
//		this->GetDlgItem(IDC_STATIC1)->SetWindowText(_T("前年度からの更新で勘定科目内訳書の更新処理が行われました。\n更新後データの処理を選択して、実行ボタンを押してください。"));
//		this->GetDlgItem(IDC_STATIC_HOKAN_MSG)->SetWindowText("「金額等を０クリア」について\n チェックを付けて実行すると、その様式のデータはすべて金額が０円になります。\n「データを保管へ移動」について\n チェックを付けて実行すると、その様式のデータはすべて保管データになり入力画面には表示されません。\n 保管されたデータは、メニューバーの保管の保管参照から入力画面に戻すことができます。");
//	}
//// midori 161103 add <--
// midori 152387 del <--
// midori 152387 add -->
	// 画面名称を変更する
	if(m_flg == 0)	{
		this->SetWindowText("翌期更新");
		this->GetDlgItem(IDC_STATIC1)->SetWindowText(_T("\n勘定科目内訳書の翌期更新を行います。"));
		this->GetDlgItem(IDC_STATIC_HOKAN_MSG)->SetWindowText("「金額等を０クリア」について\n チェックを付けて更新すると、その様式のデータはすべて金額が０円になります。\n「金額等を空欄にする」について\n チェックを付けて更新すると、その様式のデータはすべて金額が空欄になります。\n「データを保管へ移動」について\n チェックを付けて更新すると、その様式のデータはすべて保管データになり入力画面には表示されません。\n 保管されたデータは、メニューバーの保管の保管参照から入力画面に戻すことができます。");
	}
	else	{
		this->SetWindowText("");
		this->GetDlgItem(IDC_STATIC1)->SetWindowText(_T("前年度からの更新で勘定科目内訳書の更新処理が行われました。\n更新後データの処理を選択して、実行ボタンを押してください。"));
		this->GetDlgItem(IDC_STATIC_HOKAN_MSG)->SetWindowText("「金額等を０クリア」について\n チェックを付けて実行すると、その様式のデータはすべて金額が０円になります。\n「金額等を空欄にする」について\n チェックを付けて実行すると、その様式のデータはすべて金額が空欄になります。\n「データを保管へ移動」について\n チェックを付けて実行すると、その様式のデータはすべて保管データになり入力画面には表示されません。\n 保管されたデータは、メニューバーの保管の保管参照から入力画面に戻すことができます。");
	}
// midori 152387 add <--

	InitReportHeader();

	AddRecords();

// midori 160605 cor -->
	//CreateYokukiMap();
// ---------------------
	if(CreateYokukiMap() == 0)	{
		m_nRet = ID_DLG_MAX;
		EndDialog(0);
		return TRUE;
	}
// midori 160605 cor <--

	SetData();

	m_report.Populate();
	// フォーカスをレポートコントロールにあてる
	m_report.SetFocusedRow(0);

	// リサイズ
	OnInitDialogEX();

	// ダイアログを画面の作業領域の中央に表示
	// 解像度によってはタスクバーにはみ出る形で表示されるため(例：1366×768)
	m_clsFunc.DlgMoveCenter( this );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（帳表全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYokukiMain::OnBnClickedHyochkButton()
{
	m_clsFunc.ChangeReportColumnAll(&m_report, yk_select);
	int row = 0;
	for(vector<infoYokuki>::iterator it = vecInfoYokuki.begin();it != vecInfoYokuki.end(); it++){
// midori 160605 add -->
		if((*it).m_isEnable == FALSE)	{
// midori 151393 add -->
			// レコードが入力不可の場合、チェックをOFFにする
			m_report.SetChecked(row, yk_select, FALSE);
// midori 151393 add <--
			row++;
			continue;
		}
// midori 160605 add <--
		UpdateYokukiVec(row, TRUE);
		SetCheckBoxState(row, it->select);
		row++;
	}
	m_report.Populate();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（0クリア全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYokukiMain::OnBnClickedClearchkButton()
{
	m_clsFunc.ChangeReportColumnAll(&m_report, yk_0clear);

	int row = 0;
	for(vector<infoYokuki>::iterator it = vecInfoYokuki.begin();it != vecInfoYokuki.end(); it++){
// midori 151393 add -->
		// レコードが入力不可又は選択にチェックが付いていない場合、0クリア全選択/解除ボタンのチェックをOFFにする
		if((*it).m_isEnable == FALSE || m_report.IsChecked(row, yk_select) == FALSE)	m_report.SetChecked(row, yk_0clear, FALSE);
// midori 151393 add <--
// midori 152387 add -->
		if(m_report.IsChecked(row,yk_0clear) == TRUE)	{
			m_report.SetChecked(row,yk_nullclear,FALSE);
		}
// midori 152387 add <--
		UpdateYokukiVec(row, TRUE);
		row++;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（一時保管全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYokukiMain::OnBnClickedHokanchkButton()
{
	m_clsFunc.ChangeReportColumnAll(&m_report, yk_move_hokan);

	// ⑭－２は保管がないので常にDisable
	m_report.SetChecked((ID_FORMNO_142-1), yk_move_hokan, FALSE);

	int row = 0;
	for(vector<infoYokuki>::iterator it = vecInfoYokuki.begin();it != vecInfoYokuki.end(); it++){
// midori 151393 add -->
		// レコードが入力不可又は選択にチェックが付いていない場合、一時保管全選択/解除ボタンのチェックをOFFにする
		if((*it).m_isEnable == FALSE || m_report.IsChecked(row, yk_select) == FALSE)	m_report.SetChecked(row, yk_move_hokan, FALSE);
// midori 151393 add <--
		UpdateYokukiVec(row, TRUE);
		row++;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYokukiMain::OnBnClickedOkButton()
{
	int cnt = m_clsFunc.GetReportCheckNum(&m_report, yk_select);
	if(cnt < 1){
		ICSMessageBox( _T("対象帳表が選択されていません。"), MB_OK, 0, 0, this );
		return;
	}

	int st = SaveData();
	if(st != FALSE)		m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgYokukiMain::OnOK()
{
//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgYokukiMain::PreTranslateMessage(MSG* pMsg)
{
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

		case VK_SPACE:
		case VK_RETURN:
		case VK_TAB:
			if(m_clsFunc.GetFocusControlID(this) == IDC_CUSTOM_REPORT2){
				UpdateStateFromKey((int)pMsg->wParam);
				return 1;
			}
			break;
		case VK_ESCAPE:
			ICSDialog::OnCancel();
			return 1;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgYokukiMain::GetFocusControlID( CWnd* pParent /*=NULL*/)
{
	int		nRet = 0;			//	戻値
	CWnd*	pControl = NULL;	//	コントロール
	
	//	親ハンドルあり？
	if ( pParent != NULL ){

		//	フォーカスのあるコントロールを取得
		pControl = pParent->GetFocus();
		
		//	コントロール取得成功？
		if ( pControl != NULL ){
			//	コントロールID取得
			nRet = pControl->GetDlgCtrlID();
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	指定したキー種によりフォーカスを移動
//	【引数】	intID		…	現在フォーカスのあるコントロールID
//				intArrow	…	VK_UP:上への移動　VK_DOWN:下への移動
//	【戻値】	なし
//**************************************************
int CdlgYokukiMain::SetMoveControlFocus( int intID , int intArrow )
{
	//int intCnt = 0;		// カウント
	//int intCnt2 = 0;	// カウント2
	//int	intFlg = 0;		// チェック種(1:様式 2:０クリア 3:一時保管)
	//int intMove = 0;	// 移動先算出用
	//int intTrueMove = 0;// 移動先
	//
	//BOOL blnEnable = FALSE;	// 移動先が有効:TRUE

	//// ↑キー、↓キーのみ有効
	//if ( ( intArrow == VK_UP ) || ( intArrow == VK_DOWN ) ){
	//	// 現在のフォーカス対象を検索
	//	for( intCnt = 0; intCnt < UC_ID_FORMNO_MAX; intCnt++ ){
	//		if ( m_chkYokuki[intCnt]->GetDlgCtrlID() == intID ){
	//			intFlg = 1;
	//			break;
	//		}
	//		else if ( m_chkClear[intCnt]->GetDlgCtrlID() == intID ){
	//			intFlg = 2;
	//			break;
	//		}
	//		else if ( m_chkHokan[intCnt]->GetDlgCtrlID() == intID ){
	//			intFlg = 3;
	//			break;
	//		}
	//	}
	//	
	//	// ↑キーの場合
	//	if ( intArrow == VK_UP ){
	//		// 有効な移動先を取得
	//		for ( intCnt2 = ( intCnt - 1 ); intCnt2 > -1; intCnt2-- ){		
	//			switch( intFlg ){

	//			case 1:
	//				if ( m_chkYokuki[intCnt2]->IsWindowEnabled() == TRUE ){
	//					blnEnable = TRUE;
	//				}
	//				break;
	//			case 2:
	//				if ( m_chkClear[intCnt2]->IsWindowEnabled() == TRUE ){
	//					blnEnable = TRUE;
	//				}
	//				break;
	//			case 3:
	//				if ( m_chkHokan[intCnt2]->IsWindowEnabled() == TRUE ){
	//					blnEnable = TRUE;
	//				}
	//				break;
	//			}

	//			// 有効なコントロールが見つかればOK
	//			if ( blnEnable == TRUE ){
	//				break;
	//			}	
	//		}
	//	}
	//	// ↓キーの場合
	//	else if ( intArrow == VK_DOWN ){
	//		// 有効な移動先を取得
	//		for ( intCnt2 = ( intCnt + 1 ); intCnt2 < UC_ID_FORMNO_MAX; intCnt2++ ){		
	//			switch( intFlg ){

	//			case 1:
	//				if ( m_chkYokuki[intCnt2]->IsWindowEnabled() == TRUE ){
	//					blnEnable = TRUE;
	//				}
	//				break;
	//			case 2:
	//				if ( m_chkClear[intCnt2]->IsWindowEnabled() == TRUE ){
	//					blnEnable = TRUE;
	//				}
	//				break;
	//			case 3:
	//				if ( m_chkHokan[intCnt2]->IsWindowEnabled() == TRUE ){
	//					blnEnable = TRUE;
	//				}
	//				break;
	//			}

	//			// 有効なコントロールが見つかればOK
	//			if ( blnEnable == TRUE ){
	//				break;
	//			}	
	//		}
	//	}

	//	// 移動先があった
	//	if ( blnEnable == TRUE ){
	//		
	//		switch(intFlg){

	//		// 様式チェックでの移動
	//		case 1:
	//			m_chkYokuki[intCnt2]->SetFocus();
	//			break;
	//		// ０クリアチェックでの移動
	//		case 2:
	//			m_chkClear[intCnt2]->SetFocus();
	//			break;
	//		// 一時保管チェックでの移動
	//		case 3:
	//			m_chkHokan[intCnt2]->SetFocus();
	//			break;
	//		}
	//	}
	//	// ↑キーで最初の選択項目を越えた場合
	//	else if ( intArrow == VK_UP ){
	//		// 「キャンセル」ボタンに移動
	//		m_btnCancel.SetFocus();
	//	}
	//	// ↓キーで最初の選択項目を越えた場合
	//	else if ( intArrow == VK_DOWN ){
	//		// 「全選択」ボタンに移動
	//		m_btnHyoChk.SetFocus();
	//	}
	//}
	//
	//return intFlg;
return FALSE;

}

// midori 161103 cor -->
////**************************************************
////	ダイアログ表示
////	【引数】	pDB				…	データベースハンドル
////	【戻値】	ID_DLG_OK		…	印刷ボタンが押された
////				ID_DLG_CANCEL	…	キャンセルボタンが押された
////**************************************************
//int CdlgYokukiMain::ShowDialog( CDatabase* pDB )
// ---------------------
//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				pFlg			…	呼び出しサイン 0:翌期更新での呼び出し 1:前年度からの更新処理での呼び出し
//	【戻値】	ID_DLG_OK		…	印刷ボタンが押された
//				ID_DLG_CANCEL	…	キャンセルボタンが押された
//**************************************************
int CdlgYokukiMain::ShowDialog( CDatabase* pDB, int pFlg )
// midori 161103 cor <--
{
	m_nRet = ID_DLG_CANCEL;	//	戻値初期化
	m_blnHyoChk = FALSE;		// 帳表全選択/解除フラグ
	m_blnClearChk = FALSE;	// 0クリア全選択/解除フラグ
	m_blnHokanChk = TRUE;	// 一時保管全選択/解除フラグ
// midori 161103 add -->
	m_flg = pFlg;			// 呼び出しサイン 0:翌期更新での呼び出し 1:前年度からの更新処理での呼び出し
// midori 161103 add <--

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;	//	データベースハンドル取得
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
void CdlgYokukiMain::InitReportHeader()
{
// midori 152387 add -->
	int				iHeight=0;
	ICSReportColumn	*_pRepClm;
// midori 152387 add <--

	//m_report.Resize( TRUE, ICSResizeDefault );
	
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
// midori 152387 del -->
//	m_report.AddColumn(new ICSReportColumn(0, NULL, 1));
//	m_report.AddColumn(new ICSReportColumn(1, NULL, 2));
//	m_report.AddColumn(new ICSReportColumn(2, NULL, 10));
//	m_report.AddColumn(new ICSReportColumn(3, NULL, 4));
//	m_report.AddColumn(new ICSReportColumn(4, NULL, 4));
// midori 152387 del <--
// midori 152387 add -->
	m_report.AddColumn(new ICSReportColumn(0, NULL, 5));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 10));
	m_report.AddColumn(new ICSReportColumn(2, NULL, 49));
	_pRepClm = m_report.AddColumn(new ICSReportColumn(3, NULL,12));
	_pRepClm->SetAlignment( DT_WORDBREAK );	// アライメントの設定
	_pRepClm = m_report.AddColumn(new ICSReportColumn(4, NULL,12));
	_pRepClm->SetAlignment( DT_WORDBREAK );	// アライメントの設定
	_pRepClm = m_report.AddColumn(new ICSReportColumn(5, NULL,12));
	_pRepClm->SetAlignment( DT_WORDBREAK );	// アライメントの設定
// midori 152387 add <--

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	ICSReportColumns *m_pClms = m_report.GetColumns();
	int cnt = sizeof(strYokukiHeader) / sizeof(strYokukiHeader[0]);
	for(int i = 0; i < cnt; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		m_pClm->SetCaption(strYokukiHeader[i]);
	}

	// フォント設定
	CFont*		pFont;
	LOGFONT		font = {0};

	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_report.SetTextFont(font);
	m_report.SetHeaderFont(font);
// midori 152387 add -->
	// 高さの変更
	iHeight = m_report.GetHeaderHeight();
	m_report.SetHeaderHeight( iHeight + iHeight / 2 );
// midori 152387 add <--
}

//**************************************************
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	TRUE:成功
//				FALSE:失敗
//**************************************************
int CdlgYokukiMain::AddRecords()
{
	CdbUcInfSub	mfcRec(m_pDB);
	for( int i=0; i<UC_ID_FORMNO_MAX; i++ ){
		if(mfcRec.RequeryFormOrder(i + 1) != DB_ERR_OK){
			mfcRec.Fin();
			return FALSE;
		}

		ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
		pRecord->AddItem( new ICSReportRecordItemCheckBox());									// 選択
		pRecord->AddItem( new ICSReportRecordItemText(mfcRec.m_TitleNo + mfcRec.m_TitleNo2));	// 様式番号
		pRecord->AddItem( new ICSReportRecordItemText(mfcRec.m_Title));							// 様式名称
		pRecord->AddItem( new ICSReportRecordItemCheckBox());									// ０クリア
// midori 152387 add -->
		pRecord->AddItem( new ICSReportRecordItemCheckBox());									// 空欄にする
// midori 152387 add <--
		pRecord->AddItem( new ICSReportRecordItemCheckBox());									// 保管
	}

	// 行の高さ
	m_report.SetItemHeight(20);

	return TRUE;
}

void CdlgYokukiMain::OnNMReportCheckedCUSTOM_REPORT2(NMHDR * pNotifyStruct, LRESULT * result)
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
	//チェックされた位置
	POINT pt	=	pItemNotify->pt;

// midori 152387 del -->
//	if(indexColumn != yk_select)	return;
//
//	SetCheckBoxState(indexRow, bCheck);
// midori 152387 del <--
// midori 152387 add -->
	if(indexColumn != yk_select && indexColumn != yk_0clear && indexColumn != yk_nullclear)	return;

	if(indexColumn == yk_select)	{
		SetCheckBoxState(indexRow, bCheck);
	}
	// 金額等を０クリア
	else if(indexColumn == yk_0clear)	{
		m_report.SetChecked(indexRow,yk_nullclear,FALSE);
	}
	// 金額等を空欄にする
	else if(indexColumn == yk_nullclear)	{
		m_report.SetChecked(indexRow,yk_0clear,FALSE);
	}
// midori 152387 add <--
	UpdateYokukiVec(indexRow, TRUE);
}

void CdlgYokukiMain::SetCheckBoxState(int indexRow, BOOL state)
{
	m_report.AllowEditItem(indexRow, yk_0clear, state);
// midori 152387 add -->
	m_report.AllowEditItem(indexRow, yk_nullclear, state);
// midori 152387 add <--
	// ⑭－２は常に選択不可	
	if(indexRow != 16)	m_report.AllowEditItem(indexRow, yk_move_hokan, state);
	else				m_report.AllowEditItem(indexRow, yk_move_hokan, FALSE);
}

// midori 160605 cor -->
//void CdlgYokukiMain::CreateYokukiMap()
// ---------------------
int CdlgYokukiMain::CreateYokukiMap()
// midori 160605 cor <--
{
// midori 160605 add -->
	int		rv=0;
// midori 160605 add <--

	CdbUcInfYokuki mfcRec(m_pDB);
	mfcRec.InitSort();
	mfcRec.MoveFirst();
	vecInfoYokuki.clear();
	while(!mfcRec.IsEOF())
	{
		infoYokuki iy = {0};

		iy.select = mfcRec.m_OpYokuki;
		iy.zero_clear = mfcRec.m_OpClear;
// midori 152387 add -->
		iy.null_clear = mfcRec.m_OpNullClear;
// midori 152387 add <--
		iy.move_hokan = mfcRec.m_OpHokan;
		iy.m_OpYokukiFg = mfcRec.m_OpYokukiFg;
		iy.m_OpClearFg = mfcRec.m_OpClearFg;
// midori 152387 add -->
		iy.m_OpNullClearFg = mfcRec.m_OpNullClearFg;
// midori 152387 add <--
		iy.m_OpHokanFg = mfcRec.m_OpHokanFg;

// midori 160605 add -->
		BOOL hasData = CheckYousikiData(mfcRec.m_FormSeq);
		if(hasData != NULL){	// データがなければ選択不可
			iy.m_isEnable = TRUE;
			rv++;
		}
// midori 160605 add <--

		vecInfoYokuki.push_back(iy);

		mfcRec.MoveNext();
	}
	mfcRec.Fin();
// midori 160605 add -->
	return(rv);
// midori 160605 add <--
}

void CdlgYokukiMain::SetData()
{
	int cnt = 0;
	for(vector<infoYokuki>::iterator it = vecInfoYokuki.begin();it != vecInfoYokuki.end(); it++){
// midori 160605 add -->
		// データのない行
		if((*it).m_isEnable == FALSE){
		//if(it->m_isEnable == FALSE){
			SetCheckBoxState(cnt, FALSE);
			// 背景色更新
			SetColor(cnt, (*it).m_isEnable);
			//SetColor(cnt, it->m_isEnable);
			// データがない行は「選択」カラムも選択不可
// midori 151393 cor -->
//			m_report.AllowEditItem(cnt, zm_select, FALSE);
// ---------------------
			m_report.AllowEditItem(cnt, yk_select, FALSE);
// midori 151393 cor <--
		}
		else	{
// midori 160605 add <--
			if(it->m_OpYokukiFg != FALSE){
				// 様式チェック
				m_report.SetChecked(cnt, yk_select, TRUE);

				BOOL check = FALSE;
				// ０クリアチェック
				if(it->m_OpClearFg != FALSE)	check = TRUE;
				else							check = FALSE;
				m_report.SetChecked(cnt, yk_0clear, check);

// midori 152387 add -->
				// 空白にするチェック
				if(it->m_OpNullClearFg != FALSE && it->m_OpClearFg == FALSE)	check = TRUE;
				else															check = FALSE;
				m_report.SetChecked(cnt, yk_nullclear, check);
// midori 152387 add <--

				// 一時保管データ移動チェック	
				//if(it->m_OpHokanFg != FALSE)		check = TRUE;
				//else									check = FALSE;
				check = FALSE;	// 2011/12/22 デフォルトOFF 
				m_report.SetChecked(cnt, yk_move_hokan, check);
			}
			else{
				m_report.SetChecked(cnt, yk_select, FALSE);
				m_report.SetChecked(cnt, yk_0clear, FALSE);
// midori 152387 add -->
				m_report.SetChecked(cnt, yk_nullclear, FALSE);
// midori 152387 add <--
				m_report.SetChecked(cnt, yk_move_hokan, FALSE);
			}
// midori 160605 add -->
		}
// midori 160605 add <--
		cnt++;
	}

	// ⑭－２の保管へ移動は常に選択不可
	m_report.AllowEditItem(16, yk_move_hokan, FALSE);
}

void CdlgYokukiMain::UpdateYokukiVec(int row, BOOL flag)
{
	if(flag != FALSE){	// コントロールから変数へ
		vecInfoYokuki[row].select = m_report.IsChecked(row, yk_select);
		vecInfoYokuki[row].zero_clear = m_report.IsChecked(row, yk_0clear);
// midori 152387 add -->
		vecInfoYokuki[row].null_clear = m_report.IsChecked(row, yk_nullclear);
// midori 152387 add <--
		vecInfoYokuki[row].move_hokan = m_report.IsChecked(row, yk_move_hokan);
	}
	else{				// 変数からコントロールへ
		m_report.SetChecked(row, yk_select, vecInfoYokuki[row].select);
		m_report.SetChecked(row, yk_0clear, vecInfoYokuki[row].zero_clear);
// midori 152387 add -->
		m_report.SetChecked(row, yk_nullclear, vecInfoYokuki[row].null_clear);
// midori 152387 add <--
		m_report.SetChecked(row, yk_move_hokan, vecInfoYokuki[row].move_hokan);
	}
}

BOOL CdlgYokukiMain::SaveData()
{
	CdbUcInfYokuki mfcRec(m_pDB);
	if(mfcRec.InitSort() != DB_ERR_OK)	return FALSE;
	mfcRec.MoveFirst();
	
	int cnt = 0;
	while(!mfcRec.IsEOF())
	{
		UpdateYokukiVec(cnt, TRUE);

		mfcRec.Edit();
// midori 160605 cor -->
		//if((vecInfoYokuki[cnt].m_OpYokukiFg != FALSE) && vecInfoYokuki[cnt].select != FALSE){
// ---------------------
		// 選択にチェックが付いている場合でも、選択不可の様式は対象に含めない
		// (「様式全選択」ボタンを押すと、選択不可の様式にもチェックが付く)
		// 保管データのみの様式は選択不可にするようにしましたが、「様式全選択」ボタンを押して
		// 選択にチェックが付くと処理の対象となり、更新後に保管データが削除されていました。
		if((vecInfoYokuki[cnt].m_OpYokukiFg != FALSE) && 
			vecInfoYokuki[cnt].select != FALSE && vecInfoYokuki[cnt].m_isEnable != FALSE)	{
// midori 160605 cor <--
			mfcRec.m_OpYokuki = TRUE;
			// ０クリア有効?
			if ( vecInfoYokuki[cnt].m_OpClearFg == TRUE ){
				mfcRec.m_OpClear = vecInfoYokuki[cnt].zero_clear;
			}

// midori 152387 add -->
			// 金額等を空欄にする有効?
			if ( vecInfoYokuki[cnt].m_OpNullClearFg == TRUE ){
				mfcRec.m_OpNullClear = vecInfoYokuki[cnt].null_clear;
			}
// midori 152387 add <--

			// 一時保管移動有効?
			if ( vecInfoYokuki[cnt].m_OpHokanFg == TRUE ){
				mfcRec.m_OpHokan = vecInfoYokuki[cnt].move_hokan;
			}
		}
		else{
			mfcRec.m_OpYokuki = FALSE;
			mfcRec.m_OpClear = FALSE;
			mfcRec.m_OpHokan = FALSE;
		}
		cnt++;
		mfcRec.Update();
		mfcRec.MoveNext();
	}
	
	mfcRec.Fin();

	return TRUE;
}

void CdlgYokukiMain::UpdateStateFromKey(int key)
{
	ICSReportSelectedRows* s_rows = m_report.GetSelectedRows();
	if(s_rows == NULL)	return;
	ICSReportRow* r_row = s_rows->GetAt(0);
	if(r_row == NULL)	return;
	int indexRow = r_row->GetIndex();
	BOOL check = m_report.IsChecked(indexRow, yk_select);
// midori 152387 add -->
	BOOL check2 = FALSE;
// midori 152387 add <--
	ICSReportRecordItem	*pItem	= m_report.GetActiveItem();

	switch(key){
		case VK_SPACE:
// midori 160605 add -->
			if(IsExistData(indexRow) == FALSE)	return;	// データのない様式はチェックを入れさせない
// midori 160605 add <--

			// 選択カラムの状態反転
			check = !check;
			if(pItem == NULL){
				m_report.SetChecked(indexRow, yk_select, check);

				// チェックボックス更新
				SetCheckBoxState(indexRow, check);
				// 背景色更新
				m_report.RedrawControl();
			}
			else{
				int indexClm = pItem->GetIndex();
				if(indexClm == yk_select){
					m_report.SetChecked(indexRow, yk_select, check);
					// チェックボックス更新
					SetCheckBoxState(indexRow, check);
					// 背景色更新
					m_report.RedrawControl();
				}
// midori 152387 del -->
//				else if(indexClm == yk_0clear || indexClm == yk_move_hokan){
// midori 152387 del <--
// midori 152387 add -->
				else if(indexClm == yk_0clear || indexClm == yk_nullclear || indexClm == yk_move_hokan){
// midori 152387 add <--
					BOOL check2 = !m_report.IsChecked(indexRow, indexClm);
					m_report.SetChecked(indexRow, indexClm, check2);
// midori 152387 add -->
					if(indexClm == yk_0clear)	{
						if(check2 == TRUE)	{
							m_report.SetChecked(indexRow,yk_nullclear,FALSE);
						}
					}
					if(indexClm == yk_nullclear)	{
						if(check2 == TRUE)	{
							m_report.SetChecked(indexRow,yk_0clear,FALSE);
						}
					}
// midori 152387 add <--
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
			
			if(pItem != NULL){	// エディットが入っている→次のカラムにエディット				
				int indexClm = pItem->GetIndex();
				// 次のカラムに移動
				if(indexClm == yk_select){			// チェックボックスなら０円クリアへ
					indexClm = yk_0clear;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
// midori 152387 del -->
				//else if(indexClm == yk_0clear){		// ０円クリアなら保管へ移動へ
				//	indexClm = yk_move_hokan;
				//	m_report.EditItem(indexRow, indexClm, TRUE);
				//}
// midori 152387 del <--
// midori 152387 add -->
				else if(indexClm == yk_0clear){		// ０円クリアなら金額欄等を空白にするへ
					indexClm = yk_nullclear;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
				else if(indexClm == yk_nullclear){	// 金額欄等を空白にするなら保管へ移動へ
					indexClm = yk_move_hokan;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
// midori 152387 add <--
				else if(indexClm == yk_move_hokan){	// 保管へ移動なら次の行へ
					m_report.MoveDown( FALSE, FALSE );

					ICSReportRecords * records = m_report.GetRecords();
					int max_cnt = records->GetCount();

					if(indexRow+1<max_cnt)	m_report.EditItem( indexRow + 1, yk_select, TRUE );	// 最終行はチェックボックスへ
				}
			}
			else{	// エディットが入っていない→選択カラムにエディット
				m_report.EditItem( indexRow, yk_select, TRUE );
			}
			break;
		case VK_TAB:
			char	cShift =( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得			
			//	SHIFTキーが押されてる？
			if ( cShift & 0x80 )	PrevDlgCtrl();
			else					NextDlgCtrl();
			break;
	}
}

void CdlgYokukiMain::SetGrid()
{
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
	arryGridColumn.Add(2);
	arryGridColumn.Add(3);
// midori 152387 add -->
	arryGridColumn.Add(4);
// midori 152387 add <--

	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
// midori 152387 add -->
	arrayGridColor.Add(RGB(255, 0, 0));
// midori 152387 add <--

	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}

// midori 160605 add -->
//**************************************************
//	指定帳表データ有無チェック処理
//	【引数】	intFormSeq	…	帳表FormSeq番号
//	【戻値】	TRUE		…	空行/頁計/累計以外の行あり
//				FALSE		…	空行/頁計/累計行のみ
//**************************************************
BOOL CdlgYokukiMain::CheckYousikiData( int intFormSeq )
{
	CdbUc000Common* rsData = NULL;

	// ＤＢオブジェクト取得
	rsData = m_clsFunc.CreateDbObject(intFormSeq, m_pDB);

	// データ取得
	if ( rsData->GetCountDataRecord(1) == 0 ){
		// 0件
		rsData->Fin();
		delete rsData;

		return FALSE;
	}

	rsData->Fin();
	delete rsData;

	return TRUE;
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				check	…	選択チェックボックスの状態
//	【戻値】	なし
//**************************************************
void CdlgYokukiMain::SetColor( int nIndex, BOOL check)
{
	if(check == FALSE)	m_report.SetRowBackColor(nIndex, GetSysColor( COLOR_BTNFACE ), FALSE);
}

BOOL CdlgYokukiMain::IsExistData(int row)
{
	try{
		infoYokuki zmd = vecInfoYokuki.at(row);
		return zmd.m_isEnable;
	}
	catch(...){
		return FALSE;
	}

	return FALSE;
}

// midori 160605 add <--

// midori 152387 add -->
void CdlgYokukiMain::OnBnClickedNullclearchkButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_clsFunc.ChangeReportColumnAll(&m_report, yk_nullclear);

	int row = 0;
	for(vector<infoYokuki>::iterator it = vecInfoYokuki.begin();it != vecInfoYokuki.end(); it++){
		// レコードが入力不可又は選択にチェックが付いていない場合、0クリア全選択/解除ボタンのチェックをOFFにする
		if((*it).m_isEnable == FALSE || m_report.IsChecked(row, yk_select) == FALSE)	m_report.SetChecked(row, yk_nullclear, FALSE);
		if(m_report.IsChecked(row,yk_nullclear) == TRUE)	{
			m_report.SetChecked(row,yk_0clear,FALSE);
		}
		UpdateYokukiVec(row, TRUE);
		row++;
	}
}
// midori 152387 add <--
