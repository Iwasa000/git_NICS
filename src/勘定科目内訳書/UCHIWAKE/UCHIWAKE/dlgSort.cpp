//--------------------------------------------------
//	dlgSort.cpp
//
//	2006.02.19～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgsort.h"

extern	BOOL	bG_Kanso;										// No.200903 add
extern	BOOL	bG_InvNo;										// インボイス登録番号追加対応_23/11/06 add

// 改良No.21-0086,21-0529 cor -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);			// midori 156189,156190,156191 add
// ------------------------------
extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

IMPLEMENT_DYNAMIC(CdlgSort, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgSort::CdlgSort(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgSort::IDD, pParent),m_isAdvSet(FALSE)
{
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgSort::CdlgSort( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgSort::~CdlgSort()
{
}

void CdlgSort::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SORTORDER_COMBO1, m_cboSortOrder1);
	DDX_Control(pDX, IDC_SORTORDER_COMBO2, m_cboSortOrder2);
	DDX_Control(pDX, IDC_SORTORDER_COMBO3, m_cboSortOrder3);
	DDX_Control(pDX, IDC_KAIPAGE_CHECK, m_chkKaiPage);
	DDX_Control(pDX, IDC_TOTAL_AUTO, m_chkTotalAuto);
	DDX_Control(pDX, IDC_TOTAL_OUTPUT, m_chkTotalOutput);
	DDX_Control(pDX, IDC_TOTAL_NAME, m_txtTotalName);
	DDX_Control(pDX, IDC_ALLMONEY_CHECK, m_chkAllMoney);
	DDX_Control(pDX, IDC_ALLMONEY_MONEY, m_txtAllMoney);
// midori 151397 add -->
	DDX_Control(pDX, IDC_SAVE_BUTTON, m_btnSAVE);
// midori 151397 add <--
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_SMALL_COMBO, m_cboSmall);
	DDX_Control(pDX, IDC_RETURN_BUTTON, m_btnReturn);
	DDX_Control(pDX, IDC_TOTAL_SPACE, m_chkTotalSpace);
	DDX_Control(pDX, IDC_SORTORDER_LABEL1, m_lblSortOrder1);
	DDX_Control(pDX, IDC_SORTORDER_LABEL2, m_lblSortOrder2);
	DDX_Control(pDX, IDC_SORTORDER_LABEL3, m_lblSortOrder3);
	DDX_Control(pDX, IDC_TOTAL_LABEL, m_lblTotal);
	DDX_Control(pDX, IDC_BRACKET_LABEL, m_lblBracket);
	DDX_Control(pDX, IDC_ALLMONEY_LABEL, m_lblAllMoney);
	DDX_Control(pDX, IDC_ALLMONEY_CHECK2, m_chkAllMoneyOutZero);
	DDX_Control(pDX, IDC_BUTTON_ADVANCED_SETTING, m_btnIkkatuAdvSet);
	DDX_Control(pDX, IDC_CHECK_USE_ADV_SETTING, m_chkUseAdv);
// midori M-16120101 add -->
	DDX_Control(pDX, IDC_SHITEN_CHECK, m_chkShitenCheck);
// midori M-16120101 add <--
// midori 190505 add -->
	DDX_Control(pDX, IDC_KAMOKU_TAITLE_CHECK, m_chkKmkTitle);
// midori 190505 add <--
// 20-0873 add -->
	DDX_Control(pDX, IDC_BUTTON_AMOUNT, m_btnAmount);
// 20-0873 add <--
}


BEGIN_MESSAGE_MAP(CdlgSort, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_SORTORDER_COMBO1, &CdlgSort::OnCbnSelchangeSortorderCombo1)
	ON_CBN_SELCHANGE(IDC_SORTORDER_COMBO2, &CdlgSort::OnCbnSelchangeSortorderCombo2)
	ON_CBN_SELCHANGE(IDC_SORTORDER_COMBO3, &CdlgSort::OnCbnSelchangeSortorderCombo3)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgSort::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ALLMONEY_CHECK, &CdlgSort::OnBnClickedAllmoneyCheck)
	ON_BN_CLICKED(IDC_RETURN_BUTTON, &CdlgSort::OnBnClickedReturnButton)
	ON_BN_CLICKED(IDC_TOTAL_AUTO, &CdlgSort::OnBnClickedTotalAuto)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED_SETTING, &CdlgSort::OnBnClickedButtonAdvancedSetting)
	ON_BN_CLICKED(IDC_CHECK_USE_ADV_SETTING, &CdlgSort::OnBnClickedButtonCheckUseAdvSetting)
// midori M-16120101 add -->
	ON_BN_CLICKED(IDC_KAIPAGE_CHECK, &CdlgSort::OnBnClickedKaipageCheck)
// midori M-16120101 add <--
// midori 151397 add -->
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CdlgSort::OnBnClickedSaveButton)
// midori 151397 add <--
// 20-0873 add -->
	ON_BN_CLICKED(IDC_BUTTON_AMOUNT, &CdlgSort::OnBnClickedButtonAmount)
// 20-0873 add <--
END_MESSAGE_MAP()


void CdlgSort::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgSort::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgSort::OnInitDialog()
{
	ICSDialog::OnInitDialog();
		
	//	入力確定後のフォーマット指定
// No.200903 del -->
	//m_txtAllMoney.SetFormat( FMT12 );
// No.200903 del <--
// No.200903 add -->
	if(bG_Kanso == TRUE)	{
		switch(m_nFormSeq)	{
			case ID_FORMNO_041:		// ④-1仮払金（前渡金）の内訳書
			case ID_FORMNO_042:		// ④-2貸付金及び受取利息の内訳書
			case ID_FORMNO_061:		// ⑥  有価証券の内訳書
			case ID_FORMNO_071:		// ⑦  固定資産の内訳書
			case ID_FORMNO_101:		// ⑩-1仮受金の内訳書
			case ID_FORMNO_111:		// ⑪  借入金及び支払利子の内訳書
			case ID_FORMNO_131:		// ⑬  売上高等の事業所別内訳書
			case ID_FORMNO_151:		// ⑮-1地代家賃の内訳書
			case ID_FORMNO_152:		// ⑮-2権利金等の期中支払の内訳書
			case ID_FORMNO_153:		// ⑮-3工業所有権等の使用料の内訳書
			case ID_FORMNO_161:		// ⑯-1雑益等の内訳書
			case ID_FORMNO_162:		// ⑯-2雑損失等の内訳書
				// 明細12桁
				m_txtAllMoney.SetMax(12);
				m_txtAllMoney.SetFormat(FMT12);
				break;

			default:				// それ以外の様式
				// 明細13桁
				m_txtAllMoney.SetMax(13);
				m_txtAllMoney.SetFormat(FMT13);
				break;
		}
	}
	else	{
		m_txtAllMoney.SetMax(12);
		m_txtAllMoney.SetFormat(FMT12);
	}
// No.200903 add <--

	//	コンボボックスに括弧種別設定
	SetBracketKind();

	// ソートアイテムシーケンスリストの初期化
// インボイス登録番号追加対応_23/11/06 del -->
	//InitSortItemSeq( m_SortItemSeqID );
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
	if(bG_InvNo == TRUE)	InitSortItemSeq_Inv(m_SortItemSeqID);
	else					InitSortItemSeq(m_SortItemSeqID);
// インボイス登録番号追加対応_23/11/06 add <--
	
	//	ソートアイテム設定
	if ( SetSortItem( m_nFormSeq, m_pDB ) != 0 ){
		//	ソート有効
		m_fSort = TRUE;
	}
	else{
		//	ソート無効
		m_fSort = FALSE;
	}

	//	ソート項目の有効／無効設定
	SetSortItemEnable( m_fSort );
	//	ソートデータ取得	
	GetSortData( m_nFormSeq, m_pDB, m_fSort );
	
	//	ソート有効？
	if ( m_fSort == TRUE ){
		//	ソート順チェック
		CheckSortOrder();
		//	改頁チェック
		CheckKaiPage();
	}

	//	一括金額チェック
	CheckAllMoney();

	// リサイズ
	OnInitDialogEX();

	// 一括金額詳細設定管理クラス初期化
// 改良No.21-0086,21-0529 cor -->
	//m_IkkatuValHandler.Init(m_nFormSeq);
// ------------------------------
	m_IkkatuValHandler.Init(m_nFormSeq, bG_InvNo);
// 改良No.21-0086,21-0529 cor <--

// midori 151397 add -->
	// 終了サイン初期値
	m_SortSaveSign = -1;
// midori 151397 add <--

	//	戻値を返す
	return( TRUE ); 
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	コンボボックス選択変更（第一ソート項目）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnCbnSelchangeSortorderCombo1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CheckKaiPage();

// midori 190505 add -->
	// 「科目行を挿入する」の制御切替
	CheckKamokuTitle();
// midori 190505 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	コンボボックス選択変更（第二ソート項目）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnCbnSelchangeSortorderCombo2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CheckSortOrder();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	コンボボックス選択変更（第三ソート項目）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnCbnSelchangeSortorderCombo3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	チェックボックスクリック（自動小計を行う）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnBnClickedTotalAuto()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
// midori M-16120101 add -->
	int nIndex = 0;	// 第一ソート項目インデックス
// midori M-16120101 add <--
	
	//	自動小計を行うにチェックあり？
	if ( m_chkTotalAuto.GetCheck() == TRUE ){
		//	ソート項目の有効
		SetSortItemEnable( TRUE, FALSE );
	}
	else{
		//	ソート項目の無効
		SetSortItemEnable( FALSE, FALSE );
	}
// midori M-16120101 add -->
	//	第一ソート項目の選択されている行インデックスを取得
	nIndex = m_cboSortOrder1.GetCurSel();

	//	自動小計を行うまたは改頁にチェックあり？
	if(m_nFormSeq == 1 && m_fSort == TRUE && nIndex == 0 && 
		(m_chkTotalAuto.GetCheck() == TRUE || m_chkKaiPage.GetCheck() == TRUE))	{
		m_chkShitenCheck.EnableWindow(TRUE);
	}
	else	{
		m_chkShitenCheck.EnableWindow(FALSE);
	}
// midori M-16120101 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	チェックボックスクリック（一括金額）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnBnClickedAllmoneyCheck()
{
	CheckAllMoney();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（規定値に戻す）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnBnClickedReturnButton()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	閾値を規定値に戻す
	m_txtAllMoney.SetData( m_OpIkkatuValOrg );
	
	// 詳細金額を規定値に戻す
	SetMap(TRUE);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（実行）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnBnClickedOk()
{
// midori 190505 add -->
	CString			cst = _T("");
// midori 190505 add <--

	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	
	// 入力値チェック
	if (CheckInputData() != 0) {
		// 抜ける
		return;
	}

	//	警告メッセージ
// midori 190505 del -->
//// midori 152111 cor -->
////	if ( ICSMessageBox( _T("ソートを実行すると現在設定されている\n小計、中計、空行等は削除されます。よろしいですか？"), MB_YESNO, 0, 0, this ) == IDNO ){
//// ---------------------
//	if ( ICSMessageBox( _T("ソート処理を実行し、ソート順、小計、一括金額の条件設定を保存します。\nソートを実行すると、現在入力画面に設定されている小計、中計、空行等は削除されます。\nよろしいですか？"), MB_YESNO, 0, 0, this ) == IDNO ){
//// midori 152111 cor <--
// midori 190505 del <--
// midori 190505 add -->
// 改良No.21-0086,21-0529 cor -->
	//// ②受取手形の内訳書、⑪借入金及び支払利子の内訳書
	//if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111)	{
// ------------------------------
	// ②受取手形の内訳書、⑧支払手形の内訳書、⑪借入金及び支払利子の内訳書
	if(m_nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_nFormSeq == ID_FORMNO_081) || m_nFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
		cst = _T("ソート処理を実行し、ソート順、小計、一括金額の条件設定を保存します。\nソートを実行すると、現在入力画面に設定されている小計、中計、科目行、空行等は削除されます。\nよろしいですか？");
	}
	else {
		cst = _T("ソート処理を実行し、ソート順、小計、一括金額の条件設定を保存します。\nソートを実行すると、現在入力画面に設定されている小計、中計、空行等は削除されます。\nよろしいですか？");
	}
	if ( ICSMessageBox( cst, MB_YESNO, 0, 0, this ) == IDNO ){
// midori 190505 add <--
		//	抜ける
		return;
	}
	
	//	ソートデータ保存
	if ( SetSortData( m_nFormSeq, m_pDB ) == 0 ){
		//	成功
		m_nRet = ID_DLG_OK;
	}

// midori 151397 add -->
	// 終了サイン
	m_SortSaveSign = 0;
// midori 151397 add <--

	// 一括金額マップセット（詳細設定を押下せず、直接OKボタンが押される場合）
	if(m_IkkatuValHandler.GetMap().size() == 0){
		SetMap(FALSE);
	}

	// 規定値に戻すをされると、詳細設定ボタン押下時以外でもマップに変更がある為、書き込む
	SetTable();

	CDialog::OnOK();
}

// midori 151397 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（保存）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnBnClickedSaveButton()
{
	// 入力値チェック
	if (CheckInputData() != 0) {
		// 抜ける
		return;
	}
	
// midori 152111 add -->
	if ( ICSMessageBox( _T("ソート処理は実行せず、ソート順、小計、一括金額の条件設定の保存のみ行います。\nよろしいですか？"), MB_YESNO, 0, 0, this ) == IDNO ){
		//	抜ける
		return;
	}
// midori 152111 add <--

	//	ソートデータ保存
	if ( SetSortData( m_nFormSeq, m_pDB ) == 0 ){
		//	成功
		m_nRet = ID_DLG_OK;
	}

	// 終了サイン
	m_SortSaveSign = 1;

	// 一括金額マップセット（詳細設定を押下せず、直接OKボタンが押される場合）
	if(m_IkkatuValHandler.GetMap().size() == 0){
		SetMap(FALSE);
	}

	// 規定値に戻すをされると、詳細設定ボタン押下時以外でもマップに変更がある為、書き込む
	SetTable();

	CDialog::OnOK();
}
// midori 151397 add <--

//**************************************************
//	入力値のチェック
//	【引数】	なし
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgSort::CheckInputData()
{
	int			intErrFocus = 0;		// エラー発生時のフォーカス位置

	// 第１～３ソート項目を取得
	int	intSort1 = ( int )m_cboSortOrder1.GetItemData( m_cboSortOrder1.GetCurSel() );	//	第一ソート項目
	int	intSort2 = ( int )m_cboSortOrder2.GetItemData( m_cboSortOrder2.GetCurSel() );	//	第二ソート項目
	int	intSort3 = ( int )m_cboSortOrder3.GetItemData( m_cboSortOrder3.GetCurSel() );	//	第三ソート項目

	// 同一内容を選択させない
	if (intSort2 != 0) {
		// 複数選択されている場合のみ重複チェックを行う
		if (intSort3 != 0) {
			// 第１と第３ or 第２と第３ソート項目で同じ内容を選択？
			if ((intSort1 == intSort3) || (intSort2 == intSort3)) {
				intErrFocus = IDC_SORTORDER_COMBO3;
			}
		}
		// 第１と第２ソート項目で同じ内容を選択？
		if (intSort1 == intSort2) {
			intErrFocus = IDC_SORTORDER_COMBO2;
		}
		// エラーメッセージ表示
		if (intErrFocus != 0) {
			ICSMessageBox(_T("同じ内容が選択されています。"), MB_OK, 0, 0, this);
			GetDlgItem(intErrFocus)->SetFocus();
			return -1;
		}
	}

	// 第１～３ソート項目のフィールド情報を取得
	CString	strField1 = GetSortItemFieldString(intSort1);
	CString	strField2 = GetSortItemFieldString(intSort2);
	CString	strField3 = GetSortItemFieldString(intSort3);

// 20-0450 add -->
	if(bG_Kanso == TRUE && m_nFormSeq == ID_FORMNO_061)	{
		// 簡素化様式の「種類」「銘柄」は同じフィールド名になるので、強制的に分ける
		if(intSort1 == 2)		strField1 = _T("Syurui1");
		else if(intSort1 == 3)	strField1 = _T("Syurui2");
		if(intSort2 == 2)		strField2 = _T("Syurui1");
		else if(intSort2 == 3)	strField2 = _T("Syurui2");
		if(intSort3 == 2)		strField3 = _T("Syurui1");
		else if(intSort3 == 3)	strField3 = _T("Syurui2");
	}
// 20-0450 add <--

	// 同一項目を選択させない（大きい順／小さい順）
	if (strField2 != _T("")) {
		// フィールド名称が取得できた場合のみ重複チェックを行う
		if (strField3 != _T("")) {
			// 第１と第３ or 第２と第３ソート項目で同じ項目を選択？
			if ((strField1 == strField3) || (strField2 == strField3)) {
				intErrFocus = IDC_SORTORDER_COMBO3;
			}
		}
		// 第１と第２ソート項目で同じ項目を選択？
		if (strField1 == strField2) {
			intErrFocus = IDC_SORTORDER_COMBO2;
		}
		// エラーメッセージ表示
		if (intErrFocus != 0) {
			ICSMessageBox(_T("同じ項目が選択されています。"), MB_OK, 0, 0, this);
			GetDlgItem(intErrFocus)->SetFocus();
			return -1;
		}
	}

	return 0;
}

//**************************************************
//	ソート時のフィールド情報を取得
//	【引数】	intItem		…	ソート項目番号
//	【戻値】	フィールド名称
//**************************************************
CString CdlgSort::GetSortItemFieldString( int intItem )
{
	CdbUcLstItemSort	mfcRec(m_pDB);		//	「」テーブル用クラス
	CString				strTemp = _T("");
	int					intPos;

	// 条件：ソート項目が設定されている事
	if (intItem <= 0) {
		return strTemp;
	}

	// uc_lst_item_sortから、並び替え用SQL文字列を取得
	mfcRec.RequeryFormItem(m_nFormSeq, intItem);
	if (!mfcRec.IsEOF()) {
		// xxxx ASC / xxxx DESC など
		strTemp = mfcRec.m_OrderStr.Trim();

		// 不要な"ASC"以下を削除
		intPos = strTemp.Find(_T("ASC"));
		if (intPos >= 0) {
			strTemp = strTemp.Left(intPos);			// 以降を削除
		}

		// 不要な"DESC"以下を削除
		intPos = strTemp.Find(_T("DESC"));
		if (intPos >= 0) {
			strTemp = strTemp.Left(intPos);			// 以降を削除
		}

		// 不要な")"以下を削除
		intPos = strTemp.Find(_T(")"));
		if (intPos >= 0) {
			strTemp = strTemp.Left(intPos);			// 以降を削除
		}

		// 不要な"("を削除
		while (TRUE) {
			strTemp.Trim();
			intPos = strTemp.Find(_T("("));
			if (intPos >= 0) {
				strTemp = strTemp.Mid(intPos + 1);
			}
			else {
				break;
			}
		}
		strTemp.Trim();
	}
	mfcRec.Fin();

	return strTemp;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgSort::PreTranslateMessage(MSG* pMsg)
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
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
//	ソートアイテム設定
//	【引数】	nFormSeq	…	様式シーケンス番号
//				pDB			…	データベースハンドル
//	【戻値】	なし
//**************************************************
int CdlgSort::SetSortItem( short nFormSeq, CDatabase* pDB )
{
	CdbUcLstItemSort	mfcRec( pDB );	//	「uc_lst_item_sort」テーブル用クラス
	int					nIndex = 0;		//	インデックス
// midori 152137 add -->
	int					sw=0;
	int					sw2=0;
	int					nKamokuSort = 0;
	CString				cst=_T("");
// midori 152137 add <--

	//	ソート用コンボボックスのコンテナ破棄
	m_cboSortOrder1.ResetContent();
	m_cboSortOrder2.ResetContent();
	m_cboSortOrder3.ResetContent();

// midori 152137 add -->
	nKamokuSort = m_clsFunc.IsKamokuSort(nFormSeq,pDB);
// midori 152137 add <--

	try{
		//	レコード初期化成功？
		if ( mfcRec.Init( nFormSeq ) == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				//	レコードを先頭行に移動
				mfcRec.MoveFirst();

				int		brsign = 0;	// インボイス登録番号追加対応_23/11/06 add

				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
// インボイス登録番号追加対応_23/11/06 del -->
//// midori 152137 add -->
//					sw = m_clsFunc.GetSortItem(nFormSeq,mfcRec.m_ItemSeq);
//					sw2 = 0;
//					// 科目（カナ入力有）
//					if(sw == 1)	{
//						// 0:番号順 1:50音順
//						if(nKamokuSort == 0)						sw2 = 1;
//						else										sw2 = 2;
//					}
//					// 銀行（カナ入力有）
//					else if(sw == 2)	{
//						// 0:番号順 1:50音順
//						if(m_BankSort == 0)							sw2 = 1;
//						else										sw2 = 2;
//					}
//					// 取引先（カナ入力有）
//					else if(sw == 3)	{
//						// 0:番号順 1:50音順
//						if(m_AddressSort == 0)						sw2 = 1;
//						else										sw2 = 2;
//					}
//// midori 152137 add <--
//					//	シーケンス番号が「0 = なし」以外？
//// midori 152137 add -->
//					// アイテム名称の取得
//					cst = mfcRec.m_ItemName;
//					if(sw2 == 1)			cst = cst + _T("(番号順)");
//					else if(sw2 == 2)		cst = cst + _T("(50音順)");
//// midori 154598 add -->
//					if(mfcRec.m_FormSeq == ID_FORMNO_151)	{
//						cst.Replace(_T("支払貸借料"),_T("支払賃借料"));
//					}
//// midori 154598 add <--
//// midori 152137 add <--
//					if ( mfcRec.m_ItemSeq != 0 ){
//						//	第1ソート項目設定
//// midori 152137 del -->
////						m_cboSortOrder1.AddString( mfcRec.m_ItemName );
//// midori 152137 del <--
//// midori 152137 add -->
//						m_cboSortOrder1.AddString( cst );
//// midori 152137 add <--
//						m_cboSortOrder1.SetItemData( ( nIndex - 1 ), mfcRec.m_ItemSeq );
//// midori 190505 add -->
//						m_udSortItem[ ( nIndex - 1 ) ].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
//// midori 190505 add <--
//						m_udSortItem[ ( nIndex - 1 ) ].FgPage = mfcRec.m_FgPage;			//	改頁フラグ取得
//						m_udSortItem[ ( nIndex - 1 ) ].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
//					}
//// midori 152137 del -->
////					//	第2ソート項目設定
////					m_cboSortOrder2.AddString( mfcRec.m_ItemName );
////					m_cboSortOrder2.SetItemData( m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq );
////					//	第3ソート項目設定
////					m_cboSortOrder3.AddString( mfcRec.m_ItemName );
////					m_cboSortOrder3.SetItemData( m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq );
//// midori 152137 del <--
//// midori 152137 add -->
//					//	第2ソート項目設定
//					m_cboSortOrder2.AddString( cst );
//					m_cboSortOrder2.SetItemData( m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq );
//					//	第3ソート項目設定
//					m_cboSortOrder3.AddString( cst );
//					m_cboSortOrder3.SetItemData( m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq );
//// midori 152137 add <--
//
//					// 科目の挿入
//					if( nFormSeq == ID_FORMNO_021 ){
//						if( mfcRec.m_ItemSeq == 8 ){
//							// 末尾の科目の削除
//							m_cboSortOrder1.DeleteString(7);
//							m_cboSortOrder2.DeleteString(8);
//							m_cboSortOrder3.DeleteString(8);
//
//							// 科目の挿入
//// midori 152137 del -->
////							m_cboSortOrder1.InsertString(0, mfcRec.m_ItemName);
////							m_cboSortOrder2.InsertString(1, mfcRec.m_ItemName);
////							m_cboSortOrder3.InsertString(1, mfcRec.m_ItemName);
//// midori 152137 del <--
//// midori 152137 add -->
//							m_cboSortOrder1.InsertString(0, cst);
//							m_cboSortOrder2.InsertString(1, cst);
//							m_cboSortOrder3.InsertString(1, cst);
//// midori 152137 add <--
//
//							// 拡張データのセット
//							m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
//							for(int i=0; i<9; i++){
//								m_cboSortOrder2.SetItemData(i, m_SortItemSeqID[i]);
//								m_cboSortOrder3.SetItemData(i, m_SortItemSeqID[i]);
//							}
//
//							// ソートアイテム構造体
//							for( int i = 7; i >= 1; i-- ){
//								m_udSortItem[i + 1] = m_udSortItem[i];
//							}
//// midori 190505 add -->
//							m_udSortItem[ 1 ].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
//// midori 190505 add <--
//							m_udSortItem[ 1 ].FgPage = mfcRec.m_FgPage;				//	改頁フラグ取得
//							m_udSortItem[ 1 ].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
//						}
//					}
//
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
					if(bG_InvNo == FALSE) {	// インボイス登録番号追加以前のマスターの時
						if(nFormSeq == ID_FORMNO_021 && mfcRec.m_ItemSeq == 9)			brsign = 1;
						else if(nFormSeq == ID_FORMNO_031 && mfcRec.m_ItemSeq == 5)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_041 && mfcRec.m_ItemSeq == 5)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_042 && mfcRec.m_ItemSeq == 4)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_071 && mfcRec.m_ItemSeq == 7)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_081 && mfcRec.m_ItemSeq == 8)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_081 && mfcRec.m_ItemSeq == 9)		brsign = 1;		// 改良No.21-0086,21-0529 add
						else if(nFormSeq == ID_FORMNO_091 && mfcRec.m_ItemSeq == 5)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_101 && mfcRec.m_ItemSeq == 5)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_121 && mfcRec.m_ItemSeq == 8)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_151 && mfcRec.m_ItemSeq == 5)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_152 && mfcRec.m_ItemSeq == 6)		brsign = 1;
						else if(nFormSeq == ID_FORMNO_153 && mfcRec.m_ItemSeq == 5)		brsign = 1;
						else if(nFormSeq >= ID_FORMNO_161 && nFormSeq <= ID_FORMNO_162 && mfcRec.m_ItemSeq == 6)	brsign = 1;
						else if(nFormSeq >= ID_FORMNO_171 && nFormSeq <= ID_FORMNO_1720 && mfcRec.m_ItemSeq == 5)	brsign = 1;
						if(brsign == 1) {
							// 登録番号は使用しないためここで抜ける
							break;
						}

						sw = m_clsFunc.GetSortItem(nFormSeq, mfcRec.m_ItemSeq);
						sw2 = 0;
						// 科目（カナ入力有）
						if(sw == 1) {
							// 0:番号順 1:50音順
							if(nKamokuSort == 0)		sw2 = 1;
							else						sw2 = 2;
						}
						// 銀行（カナ入力有）
						else if(sw == 2) {
							// 0:番号順 1:50音順
							if(m_BankSort == 0)			sw2 = 1;
							else						sw2 = 2;
						}
						// 取引先（カナ入力有）
						else if(sw == 3) {
							// 0:番号順 1:50音順
							if(m_AddressSort == 0)		sw2 = 1;
							else						sw2 = 2;
						}
						// シーケンス番号が「0 = なし」以外？
						// アイテム名称の取得
						cst = mfcRec.m_ItemName;
						if(sw2 == 1)			cst = cst + _T("(番号順)");
						else if(sw2 == 2)		cst = cst + _T("(50音順)");
						if(mfcRec.m_FormSeq == ID_FORMNO_151) {
							cst.Replace(_T("支払貸借料"), _T("支払賃借料"));
						}
						if(mfcRec.m_ItemSeq != 0) {
							//	第1ソート項目設定
							m_cboSortOrder1.AddString(cst);
							m_cboSortOrder1.SetItemData((nIndex - 1), mfcRec.m_ItemSeq);
							m_udSortItem[(nIndex - 1)].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
							m_udSortItem[(nIndex - 1)].FgPage = mfcRec.m_FgPage;			//	改頁フラグ取得
							m_udSortItem[(nIndex - 1)].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
						}
						//	第2ソート項目設定
						m_cboSortOrder2.AddString(cst);
						m_cboSortOrder2.SetItemData(m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq);
						//	第3ソート項目設定
						m_cboSortOrder3.AddString(cst);
						m_cboSortOrder3.SetItemData(m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq);

						// 科目の挿入
						if(nFormSeq == ID_FORMNO_021) {
							if(mfcRec.m_ItemSeq == 8) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(7);
								m_cboSortOrder2.DeleteString(8);
								m_cboSortOrder3.DeleteString(8);
								// 科目の挿入
								m_cboSortOrder1.InsertString(0, cst);
								m_cboSortOrder2.InsertString(1, cst);
								m_cboSortOrder3.InsertString(1, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
								for(int i=0; i<9; i++) {
									m_cboSortOrder2.SetItemData(i, m_SortItemSeqID[i]);
									m_cboSortOrder3.SetItemData(i, m_SortItemSeqID[i]);
								}
								// ソートアイテム構造体
								for(int i=7; i>=1; i--) {
									m_udSortItem[i + 1] = m_udSortItem[i];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
					}
					else {
						sw = m_clsFunc.GetSortItem(nFormSeq, mfcRec.m_ItemSeq);
						sw2 = 0;
						// 科目（カナ入力有）
						if(sw == 1) {
							// 0:番号順 1:50音順
							if(nKamokuSort == 0)			sw2 = 1;
							else							sw2 = 2;
						}
						// 銀行（カナ入力有）
						else if(sw == 2) {
							// 0:番号順 1:50音順
							if(m_BankSort == 0)				sw2 = 1;
							else							sw2 = 2;
						}
						// 取引先（カナ入力有）
						else if(sw == 3) {
							// 0:番号順 1:50音順
							if(m_AddressSort == 0)			sw2 = 1;
							else							sw2 = 2;
						}
						//	シーケンス番号が「0 = なし」以外？
						// アイテム名称の取得
						cst = mfcRec.m_ItemName;
						if(sw2 == 1)			cst = cst + _T("(番号順)");
						else if(sw2 == 2)		cst = cst + _T("(50音順)");
						if(mfcRec.m_FormSeq == ID_FORMNO_151) {
							cst.Replace(_T("支払貸借料"), _T("支払賃借料"));
						}
						if(mfcRec.m_ItemSeq != 0) {
							//	第1ソート項目設定
							m_cboSortOrder1.AddString(cst);
							m_cboSortOrder1.SetItemData((nIndex - 1), mfcRec.m_ItemSeq);
							m_udSortItem[(nIndex - 1)].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
							m_udSortItem[(nIndex - 1)].FgPage = mfcRec.m_FgPage;			//	改頁フラグ取得
							m_udSortItem[(nIndex - 1)].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
						}
						//	第2ソート項目設定
						m_cboSortOrder2.AddString(cst);
						m_cboSortOrder2.SetItemData(m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq);
						//	第3ソート項目設定
						m_cboSortOrder3.AddString(cst);
						m_cboSortOrder3.SetItemData(m_SortItemSeqID[nIndex], mfcRec.m_ItemSeq);

						int ii=0;
						// 科目と登録番号の挿入
						if(nFormSeq == ID_FORMNO_021) {
							if(mfcRec.m_ItemSeq == 8) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(7);
								m_cboSortOrder2.DeleteString(8);
								m_cboSortOrder3.DeleteString(8);
								// 科目の挿入
								m_cboSortOrder1.InsertString(0, cst);
								m_cboSortOrder2.InsertString(1, cst);
								m_cboSortOrder3.InsertString(1, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
								for(ii=0; ii<10; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
// 修正No.168394 del -->
								//for(ii=7; ii>=1; ii--) {
								//	m_udSortItem[ii + 1] = m_udSortItem[ii];
								//}
								//m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								//m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								//m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 del <--
// 修正No.168394 add -->
								for(ii=7; ii>=0; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[0].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[0].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[0].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 add <--
							}
							else if(mfcRec.m_ItemSeq == 9) {
								// 末尾の登録番号の削除
								m_cboSortOrder1.DeleteString(8);
								m_cboSortOrder2.DeleteString(9);
								m_cboSortOrder3.DeleteString(9);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, mfcRec.m_ItemName);
								m_cboSortOrder2.InsertString(2, mfcRec.m_ItemName);
								m_cboSortOrder3.InsertString(2, mfcRec.m_ItemName);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<10; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
// 修正No.168394 del -->
								//// ソートアイテム構造体
								//for(ii=7; ii>=2; ii--) {
								//	m_udSortItem[ii + 1] = m_udSortItem[ii];
								//}
								//m_udSortItem[2].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								//m_udSortItem[2].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								//m_udSortItem[2].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 del <--
// 修正No.168394 add -->
								// ソートアイテム構造体
								for(ii=7; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 add <--
							}
						}
						else if(nFormSeq == ID_FORMNO_031) {
							if(mfcRec.m_ItemSeq == 5) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(4);
								m_cboSortOrder2.DeleteString(5);
								m_cboSortOrder3.DeleteString(5);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, cst);
								m_cboSortOrder2.InsertString(2, cst);
								m_cboSortOrder3.InsertString(2, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<6; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
// 修正No.168394 del -->
								//for(ii=3; ii>=2; ii--) {
								//	m_udSortItem[ii + 1] = m_udSortItem[ii];
								//}
								//m_udSortItem[2].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								//m_udSortItem[2].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								//m_udSortItem[2].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 del <--
// 修正No.168394 add -->
								for(ii=3; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 add <--
							}
						}
						else if(nFormSeq == ID_FORMNO_041) {
							if(mfcRec.m_ItemSeq == 5) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(4);
								m_cboSortOrder2.DeleteString(5);
								m_cboSortOrder3.DeleteString(5);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, cst);
								m_cboSortOrder2.InsertString(2, cst);
								m_cboSortOrder3.InsertString(2, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<6; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
// 修正No.168394 del -->
								//// ソートアイテム構造体
								//for(ii=3; ii>=2; ii--) {
								//	m_udSortItem[ii + 1] = m_udSortItem[ii];
								//}
								//m_udSortItem[2].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								//m_udSortItem[2].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								//m_udSortItem[2].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 del <--
// 修正No.168394 add -->
								// ソートアイテム構造体
								for(ii=3; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 add <--
							}
						}
						else if(nFormSeq == ID_FORMNO_042) {
							if(mfcRec.m_ItemSeq == 4) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(3);
								m_cboSortOrder2.DeleteString(4);
								m_cboSortOrder3.DeleteString(4);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(0, cst);
								m_cboSortOrder2.InsertString(1, cst);
								m_cboSortOrder3.InsertString(1, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
								for(ii=0; ii<5; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
// 修正No.168394 del -->
								//// ソートアイテム構造体
								//for(ii=2; ii>=1; ii--) {
								//	m_udSortItem[ii + 1] = m_udSortItem[ii];
								//}
								//m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								//m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								//m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 del <--
// 修正No.168394 add -->
								// ソートアイテム構造体
								for(ii=2; ii>=0; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[0].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[0].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[0].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
// 修正No.168394 add <--
							}
						}
						else if(nFormSeq == ID_FORMNO_071) {
							if(mfcRec.m_ItemSeq == 7) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(6);
								m_cboSortOrder2.DeleteString(7);
								m_cboSortOrder3.DeleteString(7);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(5, cst);
								m_cboSortOrder2.InsertString(6, cst);
								m_cboSortOrder3.InsertString(6, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(5, mfcRec.m_ItemSeq);
								for(ii=0; ii<8; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=5; ii>=5; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[5].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[5].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[5].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq == ID_FORMNO_081) {
// 改良No.21-0086,21-0529 cor -->
							//if(mfcRec.m_ItemSeq == 8) {
							//	// 末尾の科目の削除
							//	m_cboSortOrder1.DeleteString(7);
							//	m_cboSortOrder2.DeleteString(8);
							//	m_cboSortOrder3.DeleteString(8);
							//	// 登録番号の挿入
							//	m_cboSortOrder1.InsertString(0, cst);
							//	m_cboSortOrder2.InsertString(1, cst);
							//	m_cboSortOrder3.InsertString(1, cst);
							//	// 拡張データのセット
							//	m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
							//	for(ii=0; ii<9; ii++) {
							//		m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
							//		m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
							//	}
							//	// ソートアイテム構造体
							//	for(ii=6; ii>=0; ii--) {
							//		m_udSortItem[ii + 1] = m_udSortItem[ii];
							//	}
							//	m_udSortItem[0].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
							//	m_udSortItem[0].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
							//	m_udSortItem[0].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							//}
// ------------------------------
							if(mfcRec.m_ItemSeq == 8) {
								// 末尾の登録番号の削除
								m_cboSortOrder1.DeleteString(7);
								m_cboSortOrder2.DeleteString(8);
								m_cboSortOrder3.DeleteString(8);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(0, mfcRec.m_ItemName);
								m_cboSortOrder2.InsertString(1, mfcRec.m_ItemName);
								m_cboSortOrder3.InsertString(1, mfcRec.m_ItemName);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
								for(ii=0; ii<10; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=7; ii>=0; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[0].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[0].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[0].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
							else if(mfcRec.m_ItemSeq == 9) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(8);
								m_cboSortOrder2.DeleteString(9);
								m_cboSortOrder3.DeleteString(9);
								// 科目の挿入
								m_cboSortOrder1.InsertString(0, cst);
								m_cboSortOrder2.InsertString(1, cst);
								m_cboSortOrder3.InsertString(1, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
								for(ii=0; ii<10; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=7; ii>=0; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[0].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[0].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[0].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
// 改良No.21-0086,21-0529 cor <--
						}
						else if(nFormSeq == ID_FORMNO_091) {
							if(mfcRec.m_ItemSeq == 5) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(4);
								m_cboSortOrder2.DeleteString(5);
								m_cboSortOrder3.DeleteString(5);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, cst);
								m_cboSortOrder2.InsertString(2, cst);
								m_cboSortOrder3.InsertString(2, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<6; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=3; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq == ID_FORMNO_101) {
							if(mfcRec.m_ItemSeq == 5) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(4);
								m_cboSortOrder2.DeleteString(5);
								m_cboSortOrder3.DeleteString(5);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, cst);
								m_cboSortOrder2.InsertString(2, cst);
								m_cboSortOrder3.InsertString(2, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<6; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=3; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq == ID_FORMNO_121) {
							if(mfcRec.m_ItemSeq == 8) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(7);
								m_cboSortOrder2.DeleteString(8);
								m_cboSortOrder3.DeleteString(8);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(3, cst);
								m_cboSortOrder2.InsertString(4, cst);
								m_cboSortOrder3.InsertString(4, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(3, mfcRec.m_ItemSeq);
								for(ii=0; ii<9; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=6; ii>=3; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[3].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[3].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[3].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq == ID_FORMNO_151) {
							if(mfcRec.m_ItemSeq == 5) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(4);
								m_cboSortOrder2.DeleteString(5);
								m_cboSortOrder3.DeleteString(5);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, cst);
								m_cboSortOrder2.InsertString(2, cst);
								m_cboSortOrder3.InsertString(2, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<6; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=3; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq == ID_FORMNO_152) {
							if(mfcRec.m_ItemSeq == 6) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(5);
								m_cboSortOrder2.DeleteString(6);
								m_cboSortOrder3.DeleteString(6);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(0, cst);
								m_cboSortOrder2.InsertString(1, cst);
								m_cboSortOrder3.InsertString(1, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(0, mfcRec.m_ItemSeq);
								for(ii=0; ii<7; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=4; ii>=0; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[0].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[0].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[0].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq == ID_FORMNO_153) {
							if(mfcRec.m_ItemSeq == 5) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(4);
								m_cboSortOrder2.DeleteString(5);
								m_cboSortOrder3.DeleteString(5);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, cst);
								m_cboSortOrder2.InsertString(2, cst);
								m_cboSortOrder3.InsertString(2, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<6; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=3; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq == ID_FORMNO_161 || nFormSeq == ID_FORMNO_162) {
							if(mfcRec.m_ItemSeq == 6) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(5);
								m_cboSortOrder2.DeleteString(6);
								m_cboSortOrder3.DeleteString(6);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(2, cst);
								m_cboSortOrder2.InsertString(3, cst);
								m_cboSortOrder3.InsertString(3, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(2, mfcRec.m_ItemSeq);
								for(ii=0; ii<7; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=4; ii>=2; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[2].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[2].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[2].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
						else if(nFormSeq >= ID_FORMNO_171 && nFormSeq <= ID_FORMNO_1720) {
							if(mfcRec.m_ItemSeq == 5) {
								// 末尾の科目の削除
								m_cboSortOrder1.DeleteString(4);
								m_cboSortOrder2.DeleteString(5);
								m_cboSortOrder3.DeleteString(5);
								// 登録番号の挿入
								m_cboSortOrder1.InsertString(1, cst);
								m_cboSortOrder2.InsertString(2, cst);
								m_cboSortOrder3.InsertString(2, cst);
								// 拡張データのセット
								m_cboSortOrder1.SetItemData(1, mfcRec.m_ItemSeq);
								for(ii=0; ii<6; ii++) {
									m_cboSortOrder2.SetItemData(ii, m_SortItemSeqID[ii]);
									m_cboSortOrder3.SetItemData(ii, m_SortItemSeqID[ii]);
								}
								// ソートアイテム構造体
								for(ii=3; ii>=1; ii--) {
									m_udSortItem[ii + 1] = m_udSortItem[ii];
								}
								m_udSortItem[1].ItemSeq = mfcRec.m_ItemSeq;			//	アイテムNo
								m_udSortItem[1].FgPage  = mfcRec.m_FgPage;			//	改頁フラグ取得
								m_udSortItem[1].FgSykei = mfcRec.m_FgSykei;			//	自動小計フラグ取得
							}
						}
					}
// インボイス登録番号追加対応_23/11/06 add <--

					//	次のレコードへ移動
					mfcRec.MoveNext();
					//	インデックスをインクリメント
					nIndex++;
				}
				//	レコードを閉じる
				mfcRec.Fin();
			}
		}
	}
	catch(...){
		
		//	レコードが開かれてる？
		if ( mfcRec.IsOpen() == TRUE ){
			//	レコードを閉じる
			mfcRec.Fin();
		}
	}
	
	//	戻値を返す
	return( nIndex );
}

//**************************************************
//	ソートデータ取得
//	【引数】	nFormSeq	…	様式シーケンス番号
//				pDB			…	データベースハンドル
//				fSort		…	ソート有無フラグ
//	【戻値】	なし
//**************************************************
void CdlgSort::GetSortData( short nFormSeq, CDatabase* pDB, BOOL fSort )
{
	CdbUcInfSub	mfcRec( pDB );	//	uc_inf_subテーブルクラス
	BOOL fMoney;			//	一括金額有無フラグ
// midori M-16120101 add -->
	int					chkSiten=0;		//	改頁、小計の挿入支店名毎に行う　チェック
	CString				strTemp=_T("");	//	
	BOOL				fSiten=FALSE;	//	改頁、小計の挿入支店名毎に行う　有効フラグ
	CdbUcLstItemSort	mfcRecS(pDB);	//	uc_lst_item_sortテーブルクラス
// midori M-16120101 add <--

	try{
		// CdbUcInfSub
		//	初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				//	ソートする？
				if ( fSort == TRUE ){

					//	ソート順選択
					SelectSortOrder( &m_cboSortOrder1, mfcRec.m_Sort1 );
					SelectSortOrder( &m_cboSortOrder2, mfcRec.m_Sort2 );
					SelectSortOrder( &m_cboSortOrder3, mfcRec.m_Sort3 );
					
					//	改頁しない？
					if ( mfcRec.m_OpKaiPage == 0 ){
						m_chkKaiPage.SetCheck( FALSE );
					}
					else{
						m_chkKaiPage.SetCheck( TRUE );
					}
					
// midori 190505 add -->
					// 科目行を挿入する
					CCtrlSubGeneralVar cs(mfcRec);
// midori 157099,157119 del -->
//// midori 157042 del -->
//					//if(cs.bKamokuTitle)	m_chkKmkTitle.SetCheck(TRUE);
//// midori 157042 del <--
//// midori 157042 add -->
//					if(cs.bOutKamokuRow)	m_chkKmkTitle.SetCheck(TRUE);
//// midori 157042 add <--
//					else					m_chkKmkTitle.SetCheck(FALSE);
//// midori 190505 add <--
// midori 157099,157119 del <--
// midori 157099,157119 add -->
					if(cs.bKamokuTitle)		m_chkKmkTitle.SetCheck(TRUE);
					else					m_chkKmkTitle.SetCheck(FALSE);
// midori 157099,157119 add <--

					//	自動小計挿入しない？
					if ( mfcRec.m_OpSykeiAuto == 0 ){
						m_chkTotalAuto.SetCheck( FALSE );
					}
					else{
						m_chkTotalAuto.SetCheck( TRUE );
					}
					
					//	一行小計しない？
					if ( mfcRec.m_OpOutSyokei == 0 ){
						m_chkTotalOutput.SetCheck( FALSE );
					}
					else{
						m_chkTotalOutput.SetCheck( TRUE );
					}
					
					//	小計行の次に空行を挿入しない？
					if ( mfcRec.m_OpOutNullLine == 0 ){
						m_chkTotalSpace.SetCheck( FALSE );
					}
					else{
						m_chkTotalSpace.SetCheck( TRUE );
					}

					//	小計名称
					m_txtTotalName.SetCaption( mfcRec.m_OpSykeiName );
					//	小計の括弧
					m_cboSmall.SetCurSel( mfcRec.m_OpSykeiKakko ); 

// midori M-16120101 add -->
					//	改頁、小計の挿入支店名毎に行う
					if(nFormSeq == 1)	{
						chkSiten=0;
						mfcRecS.RequeryFormItem(nFormSeq,1);	// 1:金融機関名
						if(!mfcRecS.IsEOF())	{
							strTemp = mfcRecS.m_FieldSykei2.Trim();
							// 大文字どうしで比較する
							if(strTemp.MakeUpper() == _T("BKNAME2"))	{
								chkSiten=1;
							}
						}
						mfcRecS.Fin();
						m_chkShitenCheck.SetCheck(chkSiten);
						if(m_chkKaiPage.GetCheck() != 0 || m_chkTotalAuto.GetCheck() != 0)	{
							fSiten = TRUE;
						}
					}
// midori M-16120101 add <--

				}
				
				//	一括金額対象帳表？
				if ( mfcRec.m_OpIkkatuFg != 0 ){
					
					//	一括金額しない？
					if ( mfcRec.m_OpIkkatu == 0 ){
						m_chkAllMoney.SetCheck( FALSE );						
					}
					else{
						m_chkAllMoney.SetCheck( TRUE );						
					}

					//　一括金額に０円を含めない？
					if(m_OpIkkatuOutZero)	m_chkAllMoneyOutZero.SetCheck(TRUE);
					else					m_chkAllMoneyOutZero.SetCheck(FALSE);
					
					//	閾値
					m_txtAllMoney.SetData( mfcRec.m_OpIkkatuVal );
					//	閾値の初期値取得
					m_OpIkkatuValOrg = mfcRec.m_OpIkkatuValOrg;
					//	有効
					fMoney = TRUE;
				}
				else{
					//	無効
					fMoney = FALSE;
					
				}

				//	一括金額のコントロール設定
				m_chkAllMoney.EnableWindow( fMoney );
				m_chkAllMoneyOutZero.EnableWindow( fMoney );
				m_txtAllMoney.EnableWindow( fMoney );
				m_btnReturn.EnableWindow( fMoney );
				m_btnIkkatuAdvSet.EnableWindow( fMoney );
//				GetDlgItem( IDC_ALLMONEY_LABEL )->EnableWindow( fMoney );
				GetDlgItem( IDC_ALLMONEY_FRAME )->EnableWindow( fMoney );

// 20-0873 add -->
// 158145 del -->
				//if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081)	{
// 158145 del <--
// 158145 add -->
				if((m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081) && (bG_Kanso == TRUE))	{
// 158145 add <--
					m_btnAmount.EnableWindow(fMoney);
				}
				else	{
					m_btnAmount.EnableWindow(FALSE);
					m_btnAmount.ShowWindow(SW_HIDE);
				}
// 20-0873 add <--

				// 「詳細設定を使用する」
				CCtrlSubGeneralVar cs(mfcRec);
				if(cs.bUseAdvSetting)	m_isAdvSet = TRUE;
				else					m_isAdvSet = FALSE;

				// 詳細設定を使用するチェックボックスの設定
				if(m_isAdvSet)	m_chkUseAdv.SetCheck(TRUE);
				else			m_chkUseAdv.SetCheck(FALSE);

				//	レコード閉じる
				mfcRec.Fin();
			}
		}
// midori M-16120101 add -->
		m_chkShitenCheck.EnableWindow(fSiten);
// midori M-16120101 add <--

// midori 156188_4 del -->
//// midori 190505 add -->
//		// 様式② もしくは ⑪ のみ「科目行を挿入する」を使用
//		if((nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111)	&& m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 1)	{
//			m_chkKmkTitle.EnableWindow(TRUE);
//		}
//		else	{
//			m_chkKmkTitle.EnableWindow(FALSE);
//		}
//// midori 190505 add <--
// midori 156188_4 del <--
// midori 156188_4 add -->
		//if(KamokuRowEnableSgn(m_pDB,0) == 0)	{			// 改良No.21-0086,21-0529 del
		if(KamokuRowEnableSgn(m_pDB, 0, nFormSeq) == 0)	{	// 改良No.21-0086,21-0529 add
			m_chkKmkTitle.EnableWindow(FALSE);
			m_chkKmkTitle.ShowWindow(SW_HIDE);
		}
		else	{
			// 様式② もしくは ⑪ のみ「科目行を挿入する」を使用
// 修正No.168394 del -->
			//if((nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111)	&& m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 1)	{
			//	m_chkKmkTitle.EnableWindow(TRUE);
			//}
			//else	{
			//	m_chkKmkTitle.EnableWindow(FALSE);
			//}
// 修正No.168394 del <--
// 修正No.168394 add -->
			if(bG_InvNo == TRUE)	{
// 改良No.21-0086,21-0529 cor -->
				//if( (nFormSeq == ID_FORMNO_021 && m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 8) ||
				//	(nFormSeq == ID_FORMNO_111 && m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 1) ) {
// ------------------------------
				if( (nFormSeq == ID_FORMNO_021 && m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 8) ||
					(nFormSeq == ID_FORMNO_081 && m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 9) ||
					(nFormSeq == ID_FORMNO_111 && m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 1) ) {
// 改良No.21-0086,21-0529 cor <--
					m_chkKmkTitle.EnableWindow(TRUE);
				}
				else {
					m_chkKmkTitle.EnableWindow(FALSE);
				}
			}
			else	{
				if((nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111) && m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 1) {
					m_chkKmkTitle.EnableWindow(TRUE);
				}
				else {
					m_chkKmkTitle.EnableWindow(FALSE);
				}
			}
// 修正No.168394 add <--
		}
// midori 156188_4 add <--

	}
	catch(...){
		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
}

//**************************************************
//	ソートデータ保存
//	【引数】	nFormSeq	…	様式シーケンス番号
//				pDB			…	データベースハンドル
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgSort::SetSortData( short nFormSeq, CDatabase* pDB )
{
	CdbUcInfSub			mfcRec( pDB );	//	uc_inf_subテーブルクラス
// midori M-16120101 add -->
	CdbUcLstItemSort	mfcRecS( pDB );	//	uc_lst_item_sortテーブルクラス
// midori M-16120101 add <--
	short				nLen;			//	閾値用数値バッファ
	char				szBuf[16];		//	閾値用文字バッファ
	int					nRet = 1;		//	戻値
	
	//	バッファをクリア
	ZeroMemory( szBuf, sizeof( szBuf ) );

	//　一括金額を０円に含まないフラグはowntableにデータを持つので、ここでセット
	if(m_chkAllMoneyOutZero.GetCheck() == 0)		m_OpIkkatuOutZero = 0;
	else											m_OpIkkatuOutZero = 1;

	try{
		// CdbUcInfSub
		//	初期化成功？
		if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				//	レコードを先頭に移動
				mfcRec.MoveFirst();
				//	レコード編集開始
				mfcRec.Edit();

				//	フィールド値セット
				mfcRec.m_Sort1 = ( BYTE )m_cboSortOrder1.GetItemData( m_cboSortOrder1.GetCurSel() );	//	第一ソート項目
				mfcRec.m_Sort2 = ( BYTE )m_cboSortOrder2.GetItemData( m_cboSortOrder2.GetCurSel() );	//	第二ソート項目
				mfcRec.m_Sort3 = ( BYTE )m_cboSortOrder3.GetItemData( m_cboSortOrder3.GetCurSel() );	//	第三ソート項目

				//	改頁対象項目？
				if ( m_udSortItem[m_cboSortOrder1.GetCurSel()].FgPage != 0 ){
					//	改頁チェックボックスのチェック状態を設定
					mfcRec.m_OpKaiPage = m_chkKaiPage.GetCheck();
				}
				//	改頁対象項目じゃない
				else{
					//	強制的に改頁しないに設定
					mfcRec.m_OpKaiPage = 0;
				}

// midori 190505 add -->
				// 科目行挿入対象項目？
// midori 157127 del -->
				//if( (nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111) && m_udSortItem[m_cboSortOrder1.GetCurSel()].ItemSeq == 1)	{
// midori 157127 del <--
// midori 157127 add -->
// 改良No.21-0086,21-0529 cor -->
				//if((nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111))	{
// ------------------------------
				if(nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && nFormSeq == ID_FORMNO_081) || nFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
// midori 157127 add <--
					CCtrlSubGeneralVar cs(mfcRec);
					// 「科目行を挿入する」チェックボックスのチェック状態を設定
// midori 157099,157119 del -->
//// midori 157042 del -->
//					//cs.bKamokuTitle = m_chkKmkTitle.GetCheck();
//// midori 157042 del <--
//// midori 157042 add -->
//					cs.bOutKamokuRow = m_chkKmkTitle.GetCheck();
//// midori 157042 add <--
// midori 157099,157119 del <--
// midori 157099,157119 add -->
					cs.bKamokuTitle = m_chkKmkTitle.GetCheck();
// midori 157099,157119 add <--
					cs.Set(mfcRec);
				}
				else{
					// 強制的に科目行を挿入しないに設定
					CCtrlSubGeneralVar cs(mfcRec);
// midori 157099,157119 del -->
//// midori 157042 del -->
//					//cs.bKamokuTitle = 0;
//// midori 157042 del <--
//// midori 157042 add -->
//					cs.bOutKamokuRow = 0;
//// midori 157042 add <--
// midori 157099,157119 del <--
// midori 157099,157119 add -->
					cs.bKamokuTitle = 0;
// midori 157099,157119 add <--
					cs.Set(mfcRec);
				}
// midori 190505 add <--

				mfcRec.m_OpSykeiAuto = m_chkTotalAuto.GetCheck();								//	自動小計
				mfcRec.m_OpOutSyokei = m_chkTotalOutput.GetCheck();								//	一行小計
				mfcRec.m_OpOutNullLine = m_chkTotalSpace.GetCheck();							//	小計行の次に空行を挿入する
				mfcRec.m_OpSykeiName = m_txtTotalName.GetCaption();								//	小計名称
				mfcRec.m_OpSykeiKakko = m_cboSmall.GetCurSel();									//	小計の括弧
				mfcRec.m_OpIkkatu = m_chkAllMoney.GetCheck();									//	一括金額
				m_txtAllMoney.GetData( (LPUNKNOWN)szBuf, &nLen );								//	一括金額閾値
				mfcRec.m_OpIkkatuVal = szBuf;
				
				//	閾値がヌル？
				if ( mfcRec.m_OpIkkatuVal == "" ){
					//	閾値を「0」に設定
					mfcRec.m_OpIkkatuVal = "0";
				}

				CCtrlSubGeneralVar cs(mfcRec);
				// 「詳細金額を使用する」
				if(m_isAdvSet)	cs.bUseAdvSetting = TRUE;
				else			cs.bUseAdvSetting = FALSE;

// midori 20/10/08u del -->
//// midori 190505 add -->
//				// 「出力方法」
//				// 第１ソート項目が改頁対象ではない項目が選択されて、出力方法が「第１ソート項目で改頁する」が選択されている場合は、
//				// 「改頁･空行を詰める」に変更する
//// midori 156909 del -->
//				//if(m_udSortItem[m_cboSortOrder1.GetCurSel()].FgPage == 0)	{
//// midori 156909 del <--
//// midori 156909 add -->
//				if(m_udSortItem[m_cboSortOrder1.GetCurSel()].FgPage == 0 || mfcRec.m_OpKaiPage == 0)	{
//// midori 156909 add <--
//					if(cs.bPrnHoho3)	{
//						cs.bPrnHoho1 = TRUE;
//						cs.bPrnHoho2 = FALSE;
//						cs.bPrnHoho3 = FALSE;
//					}
//				}
//// midori 190505 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
				if(m_udSortItem[m_cboSortOrder1.GetCurSel()].FgPage == 0 || mfcRec.m_OpKaiPage == 0)	{
					if(cs.bPrnHoho2)	{
						cs.bPrnHoho1 = TRUE;
						cs.bPrnHoho2 = FALSE;
					}
				}
// midori 20/10/08u add <--

				cs.Set(mfcRec);

				//	レコード更新
				mfcRec.Update();
				
				//	メンバ変数に保持
				m_Sort1 = mfcRec.m_Sort1;					//	第一ソート項目
				m_Sort2 = mfcRec.m_Sort2;					//	第二ソート項目
				m_Sort3 = mfcRec.m_Sort3;					//	第三ソート項目
				m_OpKaiPage = mfcRec.m_OpKaiPage;			//	改頁
// midori 190505 add -->
				m_OpKmkTitle = m_chkKmkTitle.GetCheck();	// 科目行を挿入する
// midori 190505 add <--
				m_OpSykeiAuto = mfcRec.m_OpSykeiAuto;		//	自動小計
				m_OpOutSyokei = mfcRec.m_OpOutSyokei;		//	一行小計
				m_OpOutNullLine = mfcRec.m_OpOutNullLine;	//	小計行の次に空行を挿入する
				m_OpSykeiName = mfcRec.m_OpSykeiName;		//	小計名称
				m_OpSykeiKakko = mfcRec.m_OpSykeiKakko;		//	小計の括弧
				m_OpIkkatu = mfcRec.m_OpIkkatu;				//	一括金額
				m_OpIkkatuVal = mfcRec.m_OpIkkatuVal;		//	一括金額閾値
			
				//	範囲外？
				if (( m_OpSykeiKakko < 0 ) || ( m_OpSykeiKakko >= ID_BRACKET_MAX )){
					//	強制的になしに設定
					m_OpSykeiKakko = 0;
				}
				
				//	括弧文字列取得
				m_OpLeft = BRACKET_KIND[m_OpSykeiKakko].szLeft;
				m_OpRight = BRACKET_KIND[m_OpSykeiKakko].szRight;

// midori M-16120101 add -->
				//	改頁、小計の挿入支店名毎に行う
				if(nFormSeq == 1)	{
					mfcRecS.RequeryFormItem(nFormSeq,1);	// 1:金融機関名
					if(!mfcRecS.IsEOF())	{
						//	レコードを先頭に移動
						mfcRecS.MoveFirst();
						//	レコード編集開始
						mfcRecS.Edit();
						if(m_chkShitenCheck.GetCheck() == 0)	{
							mfcRecS.m_FieldSykei2 = _T("BkName1");
						}
						else	{
							mfcRecS.m_FieldSykei2 = _T("BkName2");
						}
						//	レコード更新
						mfcRecS.Update();
					}
					mfcRecS.Fin();
				}
// midori M-16120101 add <--

				nRet = 0;								//	戻値に成功をセット
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){
		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	ソート順選択
//	【引数】	pcboSort	…	コンボボックス
//	【戻値】	nIndex		…	インデックス
//**************************************************
void CdlgSort::SelectSortOrder( CComboBox* pcboSort, int nIndex )
{
	int	nCnt;			//	カウント用
	int nSelect = 0;	//	選択用
	
	//	アイテム数分ループ
	for( nCnt = 0; nCnt < pcboSort->GetCount(); nCnt++ ){
		
		//	アイテムとインデックスが一致した？
		if ( pcboSort->GetItemData( nCnt ) == nIndex ){
			//	カウント取得
			nSelect = nCnt;
			//	抜ける
			break;
		}
	}
	
	//	一致した行を選択
	pcboSort->SetCurSel( nSelect );
}

//**************************************************
//	ソート順チェック
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgSort::CheckSortOrder()
{
	int nIndex;		//	インデックス
	
	//	第二ソート項目で選択されている行インデックスを取得
	nIndex = m_cboSortOrder2.GetCurSel();
	
	//	「なし」が選択されている
	if ( m_cboSortOrder2.GetItemData( nIndex ) == 0 ){
		//	第三ソート項目を使用不可に設定
		m_cboSortOrder3.EnableWindow( FALSE );
	}
	else{
		//	第三ソート項目を使用許可に設定
		m_cboSortOrder3.EnableWindow( TRUE );
	}
}

//**************************************************
//	改頁チェック（※小計の自動挿入も含む）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgSort::CheckKaiPage()
{
	int nIndex;		//	インデックス
	
	//	第一ソート項目の選択されている行インデックスを取得
	nIndex = m_cboSortOrder1.GetCurSel();
	
	//	改頁フラグがオン？
	if ( m_udSortItem[nIndex].FgPage != 0 ){
		//	改頁チェックボックスを使用許可に設定
		m_chkKaiPage.EnableWindow( TRUE );
	}
	else{
		//	改頁チェックボックスを使用不可に設定
		m_chkKaiPage.EnableWindow( FALSE );
	}

	//	自動小計フラグがオン？
	if ( m_udSortItem[nIndex].FgSykei != 0 ){
		//	小計グループボックス有効
		GetDlgItem( IDC_TOTAL_FRAME )->EnableWindow( TRUE );
		//	自動小計有効
		m_chkTotalAuto.EnableWindow( TRUE );
		//	自動挿入のチェックボックスクリックイベント呼び出し
		OnBnClickedTotalAuto();
	}
	else{
		//	小計グループボックス無効
		GetDlgItem( IDC_TOTAL_FRAME )->EnableWindow( FALSE );
		//	自動小計無効
		m_chkTotalAuto.EnableWindow( FALSE );
		//	ソート項目の無効
		SetSortItemEnable( FALSE, FALSE );
// midori M-16120101 add -->
		m_chkShitenCheck.EnableWindow(FALSE);
// midori M-16120101 add <--
	}
}

// midori 190505 add -->
//**************************************************
//	「科目行を挿入する」の制御切替
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgSort::CheckKamokuTitle(void)
{
	int		nIndex=-1;			// インデックス
	
	// 第一ソート項目の選択されている行インデックスを取得
	nIndex = m_cboSortOrder1.GetCurSel();
	
// midori 156188_4 del -->
	//// 科目が選択されている？
	//if( (m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111) && m_udSortItem[nIndex].ItemSeq == 1 )	{
	//	// チェックボックスを使用許可に設定
	//	m_chkKmkTitle.EnableWindow( TRUE );
	//}
	//else	{
	//	// チェックボックスを使用不可に設定
	//	m_chkKmkTitle.EnableWindow( FALSE );
	//}
// midori 156188_4 del <--
// midori 156188_4 add -->
	//if(KamokuRowEnableSgn(m_pDB,0) == 1) {				// 改良No.21-0086,21-0529 del
	if(KamokuRowEnableSgn(m_pDB, 0, m_nFormSeq) == 1)	{	// 改良No.21-0086,21-0529 add
		// 科目が選択されている？
// 修正No.168394 del -->
		//if( (m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111) && m_udSortItem[nIndex].ItemSeq == 1 )	{
		//	// チェックボックスを使用許可に設定
		//	m_chkKmkTitle.EnableWindow( TRUE );
		//}
		//else	{
		//	// チェックボックスを使用不可に設定
		//	m_chkKmkTitle.EnableWindow( FALSE );
		//}
// 修正No.168394 del <--
// 修正No.168394 add -->
		if(bG_InvNo == TRUE) {
// 改良No.21-0086,21-0529 cor -->
			//if( (m_nFormSeq == ID_FORMNO_021 && m_udSortItem[nIndex].ItemSeq == 8) ||
			//	(m_nFormSeq == ID_FORMNO_111 && m_udSortItem[nIndex].ItemSeq == 1) ) {
// ------------------------------
			if( (m_nFormSeq == ID_FORMNO_021 && m_udSortItem[nIndex].ItemSeq == 8) ||
				(m_nFormSeq == ID_FORMNO_081 && m_udSortItem[nIndex].ItemSeq == 9) ||
				(m_nFormSeq == ID_FORMNO_111 && m_udSortItem[nIndex].ItemSeq == 1) ) {
// 改良No.21-0086,21-0529 cor <--
				// チェックボックスを使用許可に設定
				m_chkKmkTitle.EnableWindow(TRUE);
			}
			else {
				// チェックボックスを使用不可に設定
				m_chkKmkTitle.EnableWindow(FALSE);
			}
		}
		else	{
			if((m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111) && m_udSortItem[nIndex].ItemSeq == 1) {
				// チェックボックスを使用許可に設定
				m_chkKmkTitle.EnableWindow(TRUE);
			}
			else {
				// チェックボックスを使用不可に設定
				m_chkKmkTitle.EnableWindow(FALSE);
			}
		}
// 修正No.168394 add <--
	}
// midori 156188_4 add <--
}
// midori 190505 add <--

//**************************************************
//	一括金額チェック
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgSort::CheckAllMoney()
{	
	//	一括金額チェックボックスがオン
	if ( m_chkAllMoney.GetCheck() == TRUE ){
		//	閾値テキストを使用許可に設定
		m_txtAllMoney.EnableWindow( TRUE );
		//	閾値ラベルを使用許可に設定
//		GetDlgItem( IDC_ALLMONEY_LABEL )->EnableWindow( TRUE );
		m_clsFunc.ICSStaticEnableWindow( &m_lblAllMoney , TRUE );
		//	規定値に戻すボタンを使用許可に設定
		m_btnReturn.EnableWindow( TRUE );
		//　一括金額に０円を含めないチェックボックスを使用許可に設定
		m_chkAllMoneyOutZero.EnableWindow( TRUE );
		//  詳細設定を使用するを使用許可に設定
		m_chkUseAdv.EnableWindow( TRUE );
// 20-0873 add -->
		//  取引先毎の総額確認ボタン
// 158145 del -->
		//if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081)	{
// 158145 del <--
// 158145 add -->
		if((m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081) && (bG_Kanso == TRUE))	{
// 158145 add <--
			m_btnAmount.EnableWindow(TRUE);
		}
// 20-0873 add <--
	}
	else{
		//	閾値テキストを使用不可に設定
		m_txtAllMoney.EnableWindow( FALSE );
		//	閾値ラベルを使用不可に設定
//		GetDlgItem( IDC_ALLMONEY_LABEL )->EnableWindow( FALSE );
		m_clsFunc.ICSStaticEnableWindow( &m_lblAllMoney , FALSE );
		//	規定値に戻すボタンを使用不可に設定
		m_btnReturn.EnableWindow( FALSE );
		//　一括金額に０円を含めないチェックボックスをしよう使用不可に設定
		m_chkAllMoneyOutZero.EnableWindow( FALSE );
		//  詳細設定を使用するを使用不可に設定
		m_chkUseAdv.EnableWindow( FALSE );
// 20-0873 add -->
		//  取引先毎の総額確認ボタン
// 158145 del -->
		//if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081)	{
// 158145 del <--
// 158145 add -->
		if((m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081) && (bG_Kanso == TRUE))	{
// 158145 add <--
			m_btnAmount.EnableWindow(FALSE);
		}
// 20-0873 add <--
	}

	OnBnClickedButtonCheckUseAdvSetting();
}

//**************************************************
//	コンボボックスに括弧種別設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgSort::SetBracketKind()
{
	int			nCnt;		//	カウント用
	CString		szBuf;		//	バッファ
	
	//	コンボボックスクリア
	m_cboSmall.ResetContent();
	
	//	括弧の最大数分ループ
	for( nCnt = 0; nCnt < ID_BRACKET_MAX; nCnt++ ){
		//	バッファ初期化
		szBuf.Empty(); 
		
		//	括弧なし？
		if ( nCnt == ID_BRACKET_NOTHING ){
			//	「なし」
			szBuf = NAME_BRACKET_NOTHING;
		}
		else{
			//	括弧文字列取得
			szBuf = BRACKET_KIND[nCnt].szLeft + BRACKET_KIND[nCnt].szRight;
		}
		
		//	コンボボックスに文字列追加
		m_cboSmall.AddString( szBuf ); 
	}
}

//**************************************************
//	ソート項目の有効／無効設定
//	【引数】	fFlag	…	フラグ
//				fAll	…	全項目フラグ
//	【戻値】	なし
//**************************************************
void CdlgSort::SetSortItemEnable( BOOL fFlag, BOOL fAll /*=TRUE*/ )
{
	//	全項目フラグオン？
	if ( fAll == TRUE ){
		m_cboSortOrder1.EnableWindow( fFlag );							//	ソート項目１
		m_cboSortOrder2.EnableWindow( fFlag );							//	ソート項目２
		m_cboSortOrder3.EnableWindow( fFlag );							//	ソート項目３
		m_chkKaiPage.EnableWindow( fFlag );								//	改頁チェック
		m_chkTotalAuto.EnableWindow( fFlag );							//	自動小計チェック
/*
		GetDlgItem( IDC_SORTORDER_LABEL1 )->EnableWindow( fFlag );		//	ソート項目１ラベル
		GetDlgItem( IDC_SORTORDER_LABEL2 )->EnableWindow( fFlag );		//	ソート項目２ラベル
		GetDlgItem( IDC_SORTORDER_LABEL3 )->EnableWindow( fFlag );		//	ソート項目３ラベル
*/
		m_clsFunc.ICSStaticEnableWindow( &m_lblSortOrder1 , fFlag );
		m_clsFunc.ICSStaticEnableWindow( &m_lblSortOrder2 , fFlag );
		m_clsFunc.ICSStaticEnableWindow( &m_lblSortOrder3 , fFlag );
	}

	m_chkTotalOutput.EnableWindow( fFlag );							//	一行でも小計するチェック
	m_chkTotalSpace.EnableWindow( fFlag );							//	小計行の次に空行を挿入するチェック
	m_txtTotalName.EnableWindow( fFlag );							//	小計の名称テキスト
	m_cboSmall.EnableWindow( fFlag );								//	小計の括弧コンボ
/*
	GetDlgItem( IDC_TOTAL_LABEL )->EnableWindow( fFlag );			//	小計の名称ラベル
	GetDlgItem( IDC_BRACKET_LABEL )->EnableWindow( fFlag );			//	小計の括弧ラベル
*/
	m_clsFunc.ICSStaticEnableWindow( &m_lblTotal , fFlag );
	m_clsFunc.ICSStaticEnableWindow( &m_lblBracket , fFlag );
}

//**************************************************
//	ソートアイテム初期化
//	【引数】	pudData		…	ソートアイテム構造体
//	【戻値】	なし
//**************************************************
void CdlgSort::InitSortItem( _SD_SORT_ITEM* pudData )
{
	pudData->FgItem = 0;
	pudData->FgPage = 0;
	pudData->FgSykei = 0;
	pudData->FieldSykei.Empty();
	pudData->FieldSykei2.Empty();
	pudData->FormSeq = 0;
	pudData->ItemName.Empty();
	pudData->ItemSeq = 0;
	pudData->OrderStr.Empty();  
}

//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	実行ボタン押した
//				ID_DLG_CANCEL	…	キャンセルボタン押した
//**************************************************
// midori 152137 del -->
//int CdlgSort::ShowDialog( int IkkatuOutZero, CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/)
// midori 152137 del <--
// midori 152137 add -->
int CdlgSort::ShowDialog( int IkkatuOutZero, CDatabase* pDB /*=NULL*/, short nFormSeq /*=ID_FORMNO_011*/, int banksort /*=0*/, int addresssort /*=0*/)
// midori 152137 add <--
{
	int		nCnt;				//	カウント用

	m_nRet = ID_DLG_CANCEL;		//	戻値
	m_Sort1 = 1;				//	第一ソート項目
	m_Sort2 = 1;				//	第二ソート項目
	m_Sort3 = 1;				//	第三ソート項目
	m_OpKaiPage = 0;			//	改頁
	m_OpSykeiAuto = 0;			//	自動小計
	m_OpOutSyokei = 0;			//	一行小計
	m_OpOutNullLine = 0;		//	小計行の次に空行を挿入する
	m_OpSykeiName.Empty();		//	小計名称
	m_OpSykeiKakko = 0;			//	小計の括弧
	m_OpOutZero = 0;			//	0円仕訳
	m_OpIkkatu = 0;				//	一括金額
	m_OpIkkatuVal.Empty();		//	一括金額閾値
	
	m_OpLeft.Empty();			//	選択された括弧の左括弧
	m_OpRight.Empty();			//	選択された括弧の右括弧

	m_OpIkkatuValOrg.Empty();	//	閾値の初期値
// midori 152137 add -->
	m_BankSort = 0;				//	参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
	m_AddressSort = 0;			//	参照ダイアログ::表示順（取引先）	0:番号順	1:50音順
// midori 152137 add <--

	//　一括金額に０円を含まないフラグをメンバ変数にセット
	m_OpIkkatuOutZero = IkkatuOutZero;
	
	//	ソートアイテム構造体初期化
	for ( nCnt = 0; nCnt < 9; nCnt++ ){
		InitSortItem( &m_udSortItem[nCnt] );
	}

	//	データベースハンドルヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				m_nFormSeq = nFormSeq;		//	様式番号取得
// midori 152137 add -->
				m_BankSort = banksort;			//	参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
				m_AddressSort = addresssort;	//	参照ダイアログ::表示順（取引先）	0:番号順	1:50音順
// midori 152137 add <--
				DoModal();					//	モーダルで表示
			}
		}
		catch(...){
		}
	}
	
	//	戻値を返す
	return( m_nRet );	
}

//********************************************************************************
//	ソートアイテムシーケンスに初期値を設定する
//		【引数】	int* フォームシーケンス
//					順序は0,8/*科目*/,1,2,3,4,5,6,7
//		【戻値】	なし
//********************************************************************************
void CdlgSort::InitSortItemSeq(int* sortitemseq)
{
	// ②受取手形の内訳書のみ、オーダーと異なるIDを割り振る
	if( m_nFormSeq == ID_FORMNO_021 ){
		for(int i=0; i<9; i++){	// 0,8,1,2,3,4,5,6,7;
			switch(i){
			case 0:
				sortitemseq[i] = i;
				break;
			case 1:
				sortitemseq[i] = 8;
				break;
			default:
				sortitemseq[i] = i-1;
				break;
			}
		}
	}
	// ②受取手形の内訳書以外
	else{
		for(int i=0; i<9; i++){
			sortitemseq[i] = i;
		}
	}
}

// インボイス登録番号追加対応_23/11/06 add -->
//********************************************************************************
//	ソートアイテムシーケンスに初期値を設定する（インボイス登録番号追加版）
//		【引数】	int* フォームシーケンス
//					順序は各様式で異なる
//		【戻値】	なし
//********************************************************************************
void CdlgSort::InitSortItemSeq_Inv(int* sortitemseq)
{
	int		ii=0;
	int		cnt=0,max=0;

	// オーダーと異なるIDを割り振る
	switch(m_nFormSeq)	{
		case ID_FORMNO_021:		// ②受取手形の内訳書
			cnt = 0;
			max = 10;
			for(ii=0; ii<max; ii++)	{	// 0,8,9,1,2,3,4,5,6,7;
				if(ii == 1)	{
					sortitemseq[ii] = 8;
					cnt++;
				}
				else if(ii == 2)	{
					sortitemseq[ii] = 9;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_031:			//	③
			cnt = 0;
			max = 6;
			for(ii=0; ii<max; ii++)	{	// 0,1,5,2,3,4;
				if(ii == 2)	{
					sortitemseq[ii] = 5;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_041:			//	④-1
			cnt = 0;
			max = 6;
			for(ii=0; ii<max; ii++)	{	// 0,1,5,2,3,4;
				if(ii == 2)	{
					sortitemseq[ii] = 5;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_042:			//	④-2
			cnt = 0;
			max = 5;
			for(ii=0; ii<max; ii++)	{	// 0,4,1,2,3;
				if(ii == 1)	{
					sortitemseq[ii] = 4;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_071:			//	⑦
			cnt = 0;
			max = 8;
			for(ii=0; ii<max; ii++)	{	// 0,1,2,3,4,5,7,6;
				if(ii == 6)	{
					sortitemseq[ii] = 7;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_081:			//	⑧
			cnt = 0;
// 改良No.21-0086,21-0529 cor -->
			//max = 9;
			//for(ii=0; ii<max; ii++)	{	// 0,8,1,2,3,4,5,6,7;
			//	if(ii == 1)	{
			//		sortitemseq[ii] = 8;
			//		cnt++;
			//	}
			//	else	{
			//		sortitemseq[ii] = ii - cnt;
			//	}
			//}
// ------------------------------
			max = 10;
			for(ii=0; ii<max; ii++)	{	// 0,9,8,1,2,3,4,5,6,7;
				if(ii == 1)	{
					sortitemseq[ii] = 9;
					cnt++;
				}
				else if(ii == 2)	{
					sortitemseq[ii] = 8;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
// 改良No.21-0086,21-0529 cor <--
			break;
		case ID_FORMNO_091:			//	⑨
			cnt = 0;
			max = 6;
			for(ii=0; ii<max; ii++)	{	// 0,1,5,2,3,4;
				if(ii == 2)	{
					sortitemseq[ii] = 5;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_101:			//	⑩-1
			cnt = 0;
			max = 6;
			for(ii=0; ii<max; ii++)	{	// 0,1,5,2,3,4;
				if(ii == 2)	{
					sortitemseq[ii] = 5;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_121:			//	⑫
			cnt = 0;
			max = 9;
			for(ii=0; ii<max; ii++)	{	// 0,1,2,3,8,4,5,6,7;
				if(ii == 4)	{
					sortitemseq[ii] = 8;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_151:			//	⑮-1
			cnt = 0;
			max = 6;
			for(ii=0; ii<max; ii++)	{	// 0,1,5,2,3,4;
				if(ii == 2)	{
					sortitemseq[ii] = 5;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_152:			//	⑮-2
			cnt = 0;
			max = 7;
			for(ii=0; ii<max; ii++)	{	// 0,6,1,2,3,4,5;
				if(ii == 1)	{
					sortitemseq[ii] = 6;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_153:			//	⑮-3
			cnt = 0;
			max = 6;
			for(ii=0; ii<max; ii++)	{	// 0,1,5,2,3,4;
				if(ii == 2)	{
					sortitemseq[ii] = 5;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_161:			//	⑯-1
		case ID_FORMNO_162:			//	⑯-2
			cnt = 0;
			max = 7;
			for(ii=0; ii<max; ii++)	{	// 0,1,2,6,3,4,5;
				if(ii == 3)	{
					sortitemseq[ii] = 6;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;
		case ID_FORMNO_171:			//	⑰ 1
		case ID_FORMNO_172:			//	⑰ 2
		case ID_FORMNO_173:			//	⑰ 3
		case ID_FORMNO_174:			//	⑰ 4
		case ID_FORMNO_175:			//	⑰ 5
		case ID_FORMNO_176:			//	⑰ 6
		case ID_FORMNO_177:			//	⑰ 7
		case ID_FORMNO_178:			//	⑰ 8
		case ID_FORMNO_179:			//	⑰ 9
		case ID_FORMNO_1710:		//	⑰ 10
		case ID_FORMNO_1711:		//	⑰ 11
		case ID_FORMNO_1712:		//	⑰ 12
		case ID_FORMNO_1713:		//	⑰ 13
		case ID_FORMNO_1714:		//	⑰ 14
		case ID_FORMNO_1715:		//	⑰ 15
		case ID_FORMNO_1716:		//	⑰ 16
		case ID_FORMNO_1717:		//	⑰ 17
		case ID_FORMNO_1718:		//	⑰ 18
		case ID_FORMNO_1719:		//	⑰ 19
		case ID_FORMNO_1720:		//	⑰ 20
			cnt = 0;
			max = 6;
			for(ii=0; ii<max; ii++)	{	// 0,1,2,6,3,4,5;
				if(ii == 2)	{
					sortitemseq[ii] = 5;
					cnt++;
				}
				else	{
					sortitemseq[ii] = ii - cnt;
				}
			}
			break;

		case ID_FORMNO_011:			//	①
		case ID_FORMNO_051:			//	⑤
		case ID_FORMNO_061:			//	⑥
		case ID_FORMNO_102:			//	⑩-2
		case ID_FORMNO_111:			//	⑪
		case ID_FORMNO_131:			//	⑬
			if(m_nFormSeq == ID_FORMNO_011)			max = 5;	//	①
			else if(m_nFormSeq == ID_FORMNO_051)	max = 10;	//	⑤
			else if(m_nFormSeq == ID_FORMNO_061)	max = 7;	//	⑥
			else if(m_nFormSeq == ID_FORMNO_102)	max = 4;	//	⑩-2
			else if(m_nFormSeq == ID_FORMNO_111)	max = 5;	//	⑪
			else if(m_nFormSeq == ID_FORMNO_131)	max = 5;	//	⑬
			for(ii=0; ii<max; ii++)	{		// 上記の様式以外
				sortitemseq[ii] = ii;
			}
			break;
		default:
			break;
	}
}
// インボイス登録番号追加対応_23/11/06 add <--

//一括金額取得
CString CdlgSort::GetIkkatuVal(long order)
{
	// 共通の一括金額
	if(m_isAdvSet == FALSE)		return m_OpIkkatuVal;

	// 個別の一括金額
	CString val = m_IkkatuValHandler.GetIkkatuVal(order);
	if(val.IsEmpty())			return m_OpIkkatuVal;		// 取得に失敗した場合は共通の一括金額

	return val;
}
// 詳細設定ダイアログ表示
void CdlgSort::OnBnClickedButtonAdvancedSetting()
{
	// 詳細設定ダイアログにレコードを表示するためにマップセット
	if(m_IkkatuValHandler.GetMap().size() == 0){
		SetMap(FALSE);
	}

	CdlgIkkatuAdvSetting dlg;
	BOOL isChange = FALSE;
// No.200903 del -->
	//dlg.ShowDialog(m_IkkatuValHandler, isChange);
// No.200903 del <--
// No.200903 add -->
	dlg.ShowDialog(m_IkkatuValHandler, isChange, m_nFormSeq);
// No.200903 add <--

	if(isChange != FALSE){
		SetTable();
	}
}

// 詳細設定を使用するクリックイベント
void CdlgSort::OnBnClickedButtonCheckUseAdvSetting()
{
	m_isAdvSet = m_chkUseAdv.GetCheck();

	if(m_chkUseAdv.IsWindowEnabled() == FALSE){
		m_txtAllMoney.EnableWindow(FALSE);
		m_btnIkkatuAdvSet.EnableWindow(FALSE);
		return;
	}

	// 詳細設定ボタンの選択可/不可設定
	if(m_isAdvSet != BST_UNCHECKED){
		m_txtAllMoney.EnableWindow(FALSE);
		m_clsFunc.ICSStaticEnableWindow( &m_lblAllMoney , FALSE );
		m_btnIkkatuAdvSet.EnableWindow(TRUE);
	}
	else{
		m_txtAllMoney.EnableWindow(TRUE);
		m_clsFunc.ICSStaticEnableWindow( &m_lblAllMoney , TRUE );
		m_btnIkkatuAdvSet.EnableWindow(FALSE);
	}
}

// uc_lst_kamokuからマップに登録
void CdlgSort::SetMap(BOOL isDefault)
{
	// デフォルトセットフラグが立っている場合、ud_inf_subからデフォルト値を取得
	CString defalut_val;
	if(isDefault != FALSE){
		CdbUcInfSub mfcSub(m_pDB);
		if(mfcSub.RequeryFormSeq(m_nFormSeq) == DB_ERR_OK){
			defalut_val = mfcSub.m_OpIkkatuValOrg;
		}
	}

// 改良No.21-0086,21-0529 cor -->
	//if((m_nFormSeq != ID_FORMNO_042) && (m_nFormSeq != ID_FORMNO_081)){		// 科目
// ------------------------------
	if( !(m_nFormSeq == ID_FORMNO_042 || (bG_InvNo == FALSE && m_nFormSeq == ID_FORMNO_081)) )	{	// 科目
// 改良No.21-0086,21-0529 cor <--
		CdbUcLstKamoku mfcKamoku(m_pDB);
		// マップをセット（科目）
		m_IkkatuValHandler.SetMap(mfcKamoku, m_nFormSeq, defalut_val);
		if ( mfcKamoku.IsOpen() ){
			mfcKamoku.Fin();
		}
	}
	else	{																// 取引先
		CdbUcLstAddress mfcAddress(m_pDB);
		// マップをセット（取引先）
		int GrSeq = ID_ADDRESSGR_URI;									// 得意先
		if(m_nFormSeq == ID_FORMNO_081)	GrSeq = ID_ADDRESSGR_KAI;		// 仕入先
		m_IkkatuValHandler.SetMap(mfcAddress, GrSeq, defalut_val);
		if ( mfcAddress.IsOpen() ){
			mfcAddress.Fin();
		}
	}
}

// マップからuc_lst_kamokuに登録
void CdlgSort::SetTable()
{
// 改良No.21-0086,21-0529 cor -->
	//if((m_nFormSeq != ID_FORMNO_042) && (m_nFormSeq != ID_FORMNO_081)){		// 科目
// ------------------------------
	if( !(m_nFormSeq == ID_FORMNO_042 || (bG_InvNo == FALSE && m_nFormSeq == ID_FORMNO_081)) )	{	// 科目
// 改良No.21-0086,21-0529 cor <--
		CdbUcLstKamoku mfcKamoku(m_pDB);
		// テーブルをセット（科目）
		m_IkkatuValHandler.SetTable(mfcKamoku, m_nFormSeq);
		if ( mfcKamoku.IsOpen() ){
			mfcKamoku.Fin();
		}
	}
	else	{																// 取引先
		CdbUcLstAddress mfcAddress(m_pDB);
		// テーブルをセット（取引先）
		int GrSeq = ID_ADDRESSGR_URI;									// 得意先
		if(m_nFormSeq == ID_FORMNO_081)		GrSeq = ID_ADDRESSGR_KAI;	// 仕入先
		m_IkkatuValHandler.SetTable(mfcAddress, GrSeq);
		if ( mfcAddress.IsOpen() ){
			mfcAddress.Fin();
		}
	}
}

// midori M-16120101 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	チェックボックスクリック（改頁）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgSort::OnBnClickedKaipageCheck()
{
	int nIndex = 0;	// 第一ソート項目インデックス

	//	第一ソート項目の選択されている行インデックスを取得
	nIndex = m_cboSortOrder1.GetCurSel();

	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 下記条件の場合、改頁、小計の挿入を支店名毎に行う」を有効する
	// ・様式が「預貯金等の内訳書」
	// ・第1ソート項目が「金融機関名」
	// ・「「小計」を自動挿入する」のチェックがON又は「改頁」を行うのチェックがON
	if(m_nFormSeq == 1 && m_fSort == TRUE && nIndex == 0 && 
		(m_chkTotalAuto.GetCheck() == TRUE || m_chkKaiPage.GetCheck() == TRUE))	{
		m_chkShitenCheck.EnableWindow(TRUE);
	}
	else	{
		m_chkShitenCheck.EnableWindow(FALSE);
	}
}
// midori M-16120101 add <--

// 20-0873 add -->
// 取引先毎の総額確認ダイアログ表示
// 158065,158066 del -->
//void CdlgSort::OnBnClickedButtonAmount()
//{
//// 158057 add -->
//// 詳細設定ボタンを押さずに押下された場合、詳細金額が取得できていなかった
//	if(m_IkkatuValHandler.GetMap().size() == 0){
//		SetMap(FALSE);
//	}
//// 158057 add <--
//	if(m_chkUseAdv.GetCheck() == 0 || m_nFormSeq == ID_FORMNO_081)	{
//		short	len=0;
//		char	bf[16]=_T("");
//		CString	cs=_T("");
//		m_txtAllMoney.GetData((LPUNKNOWN)bf,&len);
//		cs.Format(_T("%s"),bf);
//		CdlgTorihikiAmount1	dlg;
//// 158057 del -->
//		//dlg.ShowDialog(m_pDB,m_nFormSeq,cs,m_chkUseAdv.GetCheck());
//// 158057 del <--
//// 158057 add -->
//		dlg.ShowDialog(m_pDB,m_nFormSeq,cs,m_chkUseAdv.GetCheck(),m_IkkatuValHandler);
//// 158057 add <--
//	}
//	else	{
//		CdlgTorihikiAmount2	dlg;
//// 158057 del -->
//		//dlg.ShowDialog(m_pDB,m_nFormSeq);
//// 158057 del <--
//// 158057 add -->
//		dlg.ShowDialog(m_pDB,m_nFormSeq,m_IkkatuValHandler);
//// 158057 add <--
//	}
//}
// 158065,158066 del <--
// 158065,158066 add -->
void CdlgSort::OnBnClickedButtonAmount()
{
	// 詳細設定ボタンを押さずに押下された場合は詳細金額を取得する
	if(m_IkkatuValHandler.GetMap().size() == 0){
		SetMap(FALSE);
	}
	short	len=0;
	char	bf[16]=_T("");
	CString	cs=_T("");
	m_txtAllMoney.GetData((LPUNKNOWN)bf,&len);
	cs.Format(_T("%s"),bf);
// 改良No.21-0086,21-0529 cor -->
	//if(m_chkUseAdv.GetCheck() == 0 || m_nFormSeq == ID_FORMNO_081)	{
// ------------------------------
	if(m_chkUseAdv.GetCheck() == 0 || (m_nFormSeq == ID_FORMNO_081 && bG_InvNo == FALSE))	{
// 改良No.21-0086,21-0529 cor <--
		CdlgTorihikiAmount1	dlg;
// 158125 del -->
		//dlg.ShowDialog(m_pDB,m_nFormSeq,cs,m_chkUseAdv.GetCheck(),m_IkkatuValHandler);
// 158125 del <--
// 158125 add -->
		dlg.ShowDialog(m_pDB,m_nFormSeq,cs,m_chkUseAdv.GetCheck(),m_IkkatuValHandler,m_chkAllMoneyOutZero.GetCheck());
// 158125 add <--
	}
	else	{
		CdlgTorihikiAmount2	dlg;
// 158125 del -->
		//dlg.ShowDialog(m_pDB,m_nFormSeq,cs,m_IkkatuValHandler);
// 158125 del <--
// 158125 add -->
		dlg.ShowDialog(m_pDB,m_nFormSeq,cs,m_IkkatuValHandler,m_chkAllMoneyOutZero.GetCheck());
// 158125 add <--
	}
}
// 158065,158066 add <--

//*************************************************************************************************
//	取引先毎の総額で判定するかを返送する
//		【引数】	int		様式番号
//		【戻値】	int		0:判定しない　1:判定する	2:判定する（詳細設定チェックあり）
//*************************************************************************************************
int CdlgSort::IsTorihikiAmount( int pFormSeq )
{
	int		ret=0;

// 158145 del -->
	//if(pFormSeq == ID_FORMNO_021 || pFormSeq == ID_FORMNO_081)	{
// 158145 del <--
// 158145 add -->
	if((pFormSeq == ID_FORMNO_021 || pFormSeq == ID_FORMNO_081) && (bG_Kanso == TRUE))	{
// 158145 add <--
		ret = 1;
// 改良No.21-0086,21-0529 cor -->
		//if(pFormSeq == ID_FORMNO_021)	{
// ------------------------------
		if(pFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && pFormSeq == ID_FORMNO_081))	{
// 改良No.21-0086,21-0529 cor <--
			if(ret != 0)	{
				if(m_isAdvSet == TRUE)	{
					ret = 2;
				}
			}
		}
	}
	return(ret);
}
// 20-0873 add <--

