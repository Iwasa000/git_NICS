// dlgUserSet.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgUserSet.h"

// midori 190301 add -->
extern	BOOL	bG_KansoLock;
// midori 190301 add <--
extern	BOOL	bG_InvNoLock;	// インボイス登録番号追加対応_23/11/06 add

// CdlgUserSet ダイアログ

IMPLEMENT_DYNAMIC(CdlgUserSet, ICSDialog)

CdlgUserSet::CdlgUserSet(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgUserSet::IDD, pParent)
{
}

CdlgUserSet::~CdlgUserSet()
{
}

void CdlgUserSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CHECK_MOVE_MODE, m_chkMoveMode);
	DDX_Control(pDX, IDC_CHECK_PRINT_IKKATSU_DLG, m_chkPrintDlg);
	DDX_Control(pDX, IDC_CHECK_DISP_YOUSHIKI_SELECT, m_chkDispYoushikiSelect);
	DDX_Control(pDX, IDC_CHECK_ENTER_CURSOL_MOVE_DOWN, m_chkEnterMoveDown);
// midori 160612 add -->
	DDX_Control(pDX, IDC_CHECK_DATA_KAKUTEI, m_chkDataKakutei);
// midori 160612 add <--
// midori 190301 add -->
	DDX_Control(pDX, IDC_CHECK_USE_STYLE, m_chkUseStyle);
// midori 190301 add <--
	DDX_Control(pDX, IDC_CHECK_USE_STYLE2, m_chkUseStyle2);		// インボイス登録番号追加対応_23/11/06 add
}


BEGIN_MESSAGE_MAP(CdlgUserSet, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CdlgUserSet::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CdlgUserSet::OnBnClickedButtonCancel)
// midori 151401 add -->
	ON_BN_CLICKED(IDC_CHECK_DATA_KAKUTEI, &CdlgUserSet::OnBnClickedCheckDataKakutei)
// midori 151401 add <--
// midori UC_0047 add -->
	ON_BN_CLICKED(IDC_CHECK_USE_STYLE,&CdlgUserSet::OnBnClickedCheckUseStyle)
// midori UC_0047 add <--
	ON_BN_CLICKED(IDC_CHECK_USE_STYLE2,&CdlgUserSet::OnBnClickedCheckUseStyle2)	// インボイス登録番号追加対応_23/11/06 add
END_MESSAGE_MAP()


// CdlgUserSet メッセージ ハンドラ

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OKボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgUserSet::OnBnClickedButtonOk()
{
	m_uoi.YoushikiSkip = m_chkMoveMode.GetCheck();	
	m_uoi.DispIkkatDlg = m_chkPrintDlg.GetCheck();
	m_uoi.DispYoushikiSelect = m_chkDispYoushikiSelect.GetCheck();
	m_uoi.EnterMoveDown = m_chkEnterMoveDown.GetCheck();
// midori 160612 add -->
	m_uoi.DataKakutei = m_chkDataKakutei.GetCheck();
// midori 160612 add <--
// midori 190301 add -->
	m_uoi.UseStyle = m_chkUseStyle.GetCheck();
// midori 190301 add <--
	m_uoi.UseStyle2 = m_chkUseStyle2.GetCheck();	// インボイス登録番号追加対応_23/11/06 add

	m_nRet = ID_DLG_OK;	//	戻値にOKをセット

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgUserSet::OnOK()
{
//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	キャンセルボタンクリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgUserSet::OnBnClickedButtonCancel()
{
	ICSDialog::OnCancel();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgUserSet::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	m_chkMoveMode.SetCheck(m_uoi.YoushikiSkip);

	int state = BST_UNCHECKED;
	if(m_uoi.DispIkkatDlg)	state = BST_CHECKED;
	else					state = BST_UNCHECKED;
	m_chkPrintDlg.SetCheck(state);

	if(m_uoi.DispYoushikiSelect)	state = BST_CHECKED;
	else							state = BST_UNCHECKED;
	m_chkDispYoushikiSelect.SetCheck(state);

	if(m_uoi.EnterMoveDown)	state = BST_CHECKED;
	else					state = BST_UNCHECKED;
	m_chkEnterMoveDown.SetCheck(state);

// midori 160612 add -->
	if(m_uoi.DataKakutei)	state = BST_CHECKED;
	else					state = BST_UNCHECKED;
	m_chkDataKakutei.SetCheck(state);
// midori 160612 add <--
// midori 190301 add -->
	if(m_uoi.UseStyle)		state = BST_CHECKED;
	else					state = BST_UNCHECKED;
	m_chkUseStyle.SetCheck(state);
	if(bG_KansoLock == TRUE || m_uoi.DataKakutei)	{
		m_chkUseStyle.EnableWindow(FALSE);
	}
	else	{
		m_chkUseStyle.EnableWindow(TRUE);
	}
// midori 190301 add <--
// インボイス登録番号追加対応_23/11/06 add -->
	// 登録番号追加様式の使用
	if(m_uoi.UseStyle2)		state = BST_CHECKED;
	else					state = BST_UNCHECKED;
	m_chkUseStyle2.SetCheck(state);
	if(bG_InvNoLock == TRUE || m_uoi.DataKakutei) {
		m_chkUseStyle2.EnableWindow(FALSE);
	}
	else {
		m_chkUseStyle2.EnableWindow(TRUE);
	}
	m_chkUseStyle2.ShowWindow(SW_SHOW);

	// 簡素化、登録番号付きの様式切替サインの表示/非表示切替
	if(bG_KansoLock == FALSE ||
	  (bG_KansoLock == TRUE && m_chkUseStyle.GetCheck() == 0) ||
	  (bG_InvNoLock == TRUE && m_chkUseStyle2.GetCheck() == 0))	{
		m_chkUseStyle.ShowWindow(SW_SHOW);
		m_chkUseStyle2.ShowWindow(SW_HIDE);
	}
	else if(bG_InvNoLock == FALSE || (bG_KansoLock == TRUE && m_chkUseStyle.GetCheck() == 1)) {
		m_chkUseStyle.ShowWindow(SW_HIDE);
		m_chkUseStyle2.ShowWindow(SW_SHOW);
	}
// インボイス登録番号追加対応_23/11/06 add <--

// midori 151401 add -->
	// 入力データを確定するにチェックがある場合、Enterキーでカーソルを下に移動するを無効にする
	if(m_uoi.DataKakutei)	m_chkEnterMoveDown.EnableWindow(FALSE);
	else					m_chkEnterMoveDown.EnableWindow(TRUE);
// midori 151401 add <--

// 24/01/30_23-0845 add -->
	// 参照モードの場合、入力確定データを確定するを選択不可にする
	if(m_uoi.CloudSansyoFlg == TRUE)	{
		m_chkDataKakutei.EnableWindow(FALSE);
// 修正No.168585 add -->
		m_chkUseStyle.EnableWindow(FALSE);		// 簡素化切り替え
		m_chkUseStyle2.EnableWindow(FALSE);		// 登録番号追加切り替え
// 修正No.168585 add -->
	}
// 24/01/30_23-0845 add <--

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//**************************************************
//	ダイアログ表示
//	【引数】	pDB					…	データベースハンドル
//				USER_OPTION_INFO	…	オプション設定の構造体
//	【戻値】	ID_DLG_OK			…	OKボタンが押された
//				ID_DLG_CANCEL		…	キャンセルボタンが押された
//**************************************************
int CdlgUserSet::ShowDialog(USER_OPTION_INFO& uoi, CDatabase* pDB /*=NULL*/ )
{
	m_nRet	= ID_DLG_CANCEL;				//	戻値を初期化
	m_uoi = uoi;

	if( pDB != NULL )	m_pDB = pDB;

	DoModal();

	uoi = m_uoi;

	return m_nRet;
}

// midori 151401 add -->
void CdlgUserSet::OnBnClickedCheckDataKakutei()
{
	// 入力データを確定するにチェックがある場合、Enterキーでカーソルを下に移動するを無効にする
// midori 190301 del -->
	//if(m_chkDataKakutei.GetCheck() == 1)	m_chkEnterMoveDown.EnableWindow(FALSE);
	//else									m_chkEnterMoveDown.EnableWindow(TRUE);
// midori 190301 del <--
// midori 190301 add -->
	if(m_chkDataKakutei.GetCheck() == 1)	{
		m_chkEnterMoveDown.EnableWindow(FALSE);
		m_chkUseStyle.EnableWindow(FALSE);
	}
	else	{
		m_chkEnterMoveDown.EnableWindow(TRUE);
		if(bG_KansoLock == FALSE)	{
			m_chkUseStyle.EnableWindow(TRUE);
		}
// インボイス登録番号追加対応_23/11/06 add -->
		if(bG_InvNoLock == FALSE)	{
			m_chkUseStyle2.EnableWindow(TRUE);
		}
// インボイス登録番号追加対応_23/11/06 add <--

	}
// midori 190301 add <--
}
// midori 151401 add <--

// midori UC_0047 add -->
// --------------------------------------------------------------------------------------------------------
//	記載内容の簡素化に対応した様式を使用する	クリックイベント 
// --------------------------------------------------------------------------------------------------------
void CdlgUserSet::OnBnClickedCheckUseStyle()
{
	CString		msg=_T("");

	// チェックが外れたら確認メッセージを表示する
	if(m_chkUseStyle.GetCheck() == 0) {
// midori 155279 del -->
//		msg = "平成31年 4月 1日以後終了する事業年度は、\r\n電子申告では新様式のみ使用できます。\r\n旧様式に変更しますか？";
// midori 155279 del <--
// midori 155279 add -->
		msg = "平成31年 4月 1日以後終了する事業年度は、\r\n電子申告では簡素化後の様式のみ使用できます。\r\n簡素化前の様式に変更しますか？";
// midori 155279 add <--
		if(ICSMessageBox(msg, MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
			m_chkUseStyle.SetCheck(1);
		}
	}
}
// midori UC_0047 add <--

// インボイス登録番号追加対応_23/11/06 add -->
// --------------------------------------------------------------------------------------------------------
//	登録番号の追加に対応した様式を使用する	クリックイベント 
// --------------------------------------------------------------------------------------------------------
void CdlgUserSet::OnBnClickedCheckUseStyle2()
{
	CString		msg=_T("");

	// チェックが外れたら確認メッセージを表示する
	if(m_chkUseStyle2.GetCheck() == 0) {
		msg = "令和 6年 3月 1日以後終了する事業年度は、\r\n電子申告では登録番号(法人番号)の追加様式のみ使用できます。\r\n登録番号(法人番号)欄追加前の様式に変更しますか？";
		if(ICSMessageBox(msg, MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
			m_chkUseStyle2.SetCheck(1);
		}
	}
}
// インボイス登録番号追加対応_23/11/06 add <--
