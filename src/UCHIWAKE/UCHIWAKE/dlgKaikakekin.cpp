//--------------------------------------------------
//	dlgKaikakekin.cpp
//
//	2006.03.23	～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgKaikakekin.h"
#include "ConvOutRangai.h"

// midori 154927 add -->
extern	BOOL	bG_Kanso;
// midori 154927 add <--

// CdlgKaikakekin ダイアログ

IMPLEMENT_DYNAMIC(CdlgKaikakekin, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgKaikakekin::CdlgKaikakekin(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgKaikakekin::IDD, pParent)
{
	m_CurrentPage	= 1;
	m_MaxPage		= 1;
	m_MaxPageDB		= 0;
	m_PageMaxFrom	= 1;

	m_mapIndex[14] = 1;
	m_mapIndex[15] = 1;
	m_mapIndex[16] = 2;
	m_mapIndex[17] = 2;
	m_mapIndex[18] = 3;
	m_mapIndex[19] = 3;
	m_mapIndex[20] = 4;
	m_mapIndex[21] = 4;
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgKaikakekin::~CdlgKaikakekin()
{
}

void CdlgKaikakekin::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KAIKAKEKIN_DIAG, m_diagInput);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_NOTE_LABEL, m_lblNote);
	DDX_Control(pDX, IDC_PRINT_CHECK, m_chkPrint);
	DDX_Control(pDX, IDC_PRINT_COMBO, m_cmbPrint);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnMovePrev);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnMoveNext);
	DDX_Control(pDX, IDC_STATIC_PAGE_CURRENT, m_txtCurrent);
	DDX_Control(pDX, IDC_STATIC_PAGE_MAX, m_txtMax);
	DDX_Control(pDX, IDC_RADIO_COMMON, m_rdoCommon);
	DDX_Control(pDX, IDC_RADIO_INDIV, m_rdoIndiv);
}


BEGIN_MESSAGE_MAP(CdlgKaikakekin, ICSDialog)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgKaikakekin::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PRINT_CHECK, &CdlgKaikakekin::OnBnClickedPrintCheck)
	ON_BN_CLICKED(IDC_RADIO_COMMON, &CdlgKaikakekin::OnBnClickedRadioCommon)
	ON_BN_CLICKED(IDC_RADIO_INDIV, &CdlgKaikakekin::OnBnClickedRadioIndiv)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CdlgKaikakekin::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CdlgKaikakekin::OnBnClickedButtonNext)
	ON_CBN_SELCHANGE(IDC_PRINT_COMBO, &CdlgKaikakekin::OnCbnSelchangePrintCombo)
// 修正No.157633_元号日付 add -->
	ON_MESSAGE(WM_USER_FOCUSSET2, OnUserFocusset2)
	ON_WM_ACTIVATE()
// 修正No.157633_元号日付 add <--
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgKaikakekin, ICSDialog)
ON_EVENT(CdlgKaikakekin, IDC_KAIKAKEKIN_DIAG, 1, CdlgKaikakekin::EditONKaikakekinDiag, VTS_I2)
ON_EVENT(CdlgKaikakekin, IDC_KAIKAKEKIN_DIAG, 2, CdlgKaikakekin::EditOFFKaikakekinDiag, VTS_I2)
ON_EVENT(CdlgKaikakekin, IDC_KAIKAKEKIN_DIAG, 3, CdlgKaikakekin::TerminationKaikakekinDiag, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// CdlgKaikakekin メッセージ ハンドラ

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgKaikakekin::OnInitDialog()
{
// midori 160612 add -->
	int					ii				= 0;
	BOOL				bTitle			= FALSE;	// タイトルダイアログ
	COLORREF			color			= 0;
	DIAGRAM_ATTRIBUTE	diaatt;						// ICSDiag属性構造体
// midori 160612 add <--
// No.200903 add -->
	int					len=0;
	CString				sFormat=_T("");
	DIAGRAM_ATTRIBUTE	diaatt2;					// ICSDiag属性構造体
// No.200903 add <--

	ICSDialog::OnInitDialog();

// 修正No.157633_元号日付 add -->
	if(bG_Kanso == TRUE)	{
// 修正No.157856 del -->
		//for(int ii=0; ii<5; ii++)	{
// 修正No.157856 del <--
// 修正No.157856 add -->
		for(ii=0; ii<5; ii++)	{
// 修正No.157856 add <--
			m_OldDate[ii] = 0;		// 日付データ入力更新チェック用の値
			m_OldGengo[ii] = 0;		// 日付(元号番号)データ入力更新チェック用の値
		}
		m_GenListSw = 0;			//
		m_EditSign = -1;			// EditON⇔EditOFFがペアで通ってるか確認するためのサイン
	}
	m_CurCol = -1;					// 現在編集中のカラム
	m_OldCol = -1;					// 前回編集していたカラム
// 修正No.157633_元号日付 add <--
// 修正No.157856 add -->
	for(ii=0; ii<4; ii++)	{
		m_bReDate[ii] = -1;
	}
// 修正No.157856 add <--

	//	ツールバーの設定成功？
	if ( SetToolBar() == 0 ){
		m_ToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_FAKE, FALSE );
	}

// midori 200101 add -->
	// 簡素化様式の場合、日付の入力方法を"元号 ××.××.××"に変更する
	if(bG_Kanso == TRUE) {
		// 未払配当金
		m_diagInput.GetAttribute(14,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 15;
		m_diagInput.SetAttribute(14,(LPUNKNOWN)&diaatt,TRUE);
		m_diagInput.GetAttribute(16,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 15;
		m_diagInput.SetAttribute(16,(LPUNKNOWN)&diaatt,TRUE);
		// 未払役員賞与
		m_diagInput.GetAttribute(18,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 15;
		m_diagInput.SetAttribute(18,(LPUNKNOWN)&diaatt,TRUE);
		m_diagInput.GetAttribute(20,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 15;
		m_diagInput.SetAttribute(20,(LPUNKNOWN)&diaatt,TRUE);

// 修正No.157633_元号日付 add -->
		// MODIFY_GENGODEF → 元号付き日付入力時、デフォルトの元号位置をシステム時間とするモード。
		//					　元号付入力個所へフォーカスが移動した時、システム時間の元号を選択した状態で、年入力位置にカーソルをセットする。
		m_diagInput.ModifyItem(14, MODIFY_GENGODEF);
		m_diagInput.ModifyItem(16, MODIFY_GENGODEF);
		m_diagInput.ModifyItem(18, MODIFY_GENGODEF);
		m_diagInput.ModifyItem(20, MODIFY_GENGODEF);
// 修正No.157633_元号日付 add <--
	}
	else {
		// 未払配当金
		m_diagInput.GetAttribute(14,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 4;
		m_diagInput.SetAttribute(14,(LPUNKNOWN)&diaatt,TRUE);
		m_diagInput.GetAttribute(16,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 4;
		m_diagInput.SetAttribute(16,(LPUNKNOWN)&diaatt,TRUE);
		// 未払役員賞与
		m_diagInput.GetAttribute(18,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 4;
		m_diagInput.SetAttribute(18,(LPUNKNOWN)&diaatt,TRUE);
		m_diagInput.GetAttribute(20,(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 4;
		m_diagInput.SetAttribute(20,(LPUNKNOWN)&diaatt,TRUE);
	}
// midori 200101 add <--

// No.200903 add -->
	// 簡素化様式の場合、金額桁数は13桁
	if(bG_Kanso == TRUE)	{
		len=13;
		sFormat=_T("s,sss,sss,sss,sss");
	}
	else	{
		len=12;
		sFormat=_T("sss,sss,sss,sss");
	}
	// 未払配当金
	m_diagInput.GetAttribute(15, (LPUNKNOWN)&diaatt2);
	diaatt2.attr_inpmax = len;
	diaatt2.attr_editformat = sFormat;
	m_diagInput.SetAttribute(15, (LPUNKNOWN)&diaatt2, TRUE);
	m_diagInput.GetAttribute(17, (LPUNKNOWN)&diaatt2);
	diaatt2.attr_inpmax = len;
	diaatt2.attr_editformat = sFormat;
	m_diagInput.SetAttribute(17, (LPUNKNOWN)&diaatt2, TRUE);
	// 未払役員賞与
	m_diagInput.GetAttribute(19, (LPUNKNOWN)&diaatt2);
	diaatt2.attr_inpmax = len;
	diaatt2.attr_editformat = sFormat;
	m_diagInput.SetAttribute(19, (LPUNKNOWN)&diaatt2, TRUE);
	m_diagInput.GetAttribute(21, (LPUNKNOWN)&diaatt2);
	diaatt2.attr_inpmax = len;
	diaatt2.attr_editformat = sFormat;
	m_diagInput.SetAttribute(21, (LPUNKNOWN)&diaatt2, TRUE);
// No.200903 add <--

	//	データ設定
	SetData();

	//	最初のフォーカスを設定
// midori 160612 cor -->
//	if( m_chkPrint.GetCheck() != FALSE ){
// ---------------------
	if( m_chkPrint.GetCheck() != FALSE && m_DataKakutei == FALSE){
// midori 160612 cor <--
		m_diagInput.SetPosition( DK_INPUTINDEX_FIRST );
	}

	// 背景色取得＆指定
	ICSColorInfo icsColInfo;
	m_ViewColor = GetSysColor( COLOR_BTNFACE );
	if ( ((ICSWinApp *)AfxGetApp())->GetStanderdColor( icsColInfo ) == 0 ){
		if ( icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON ){
			m_ViewColor = icsColInfo.ViewColor();
		}
	}
	m_diagInput.SetBackColor( m_ViewColor );
// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	{
		// 印字する
		m_chkPrint.EnableWindow(FALSE);
		// ページコンボボックス
		m_cmbPrint.EnableWindow(FALSE);
		// ページ共通出力
		m_rdoCommon.EnableWindow(FALSE);
		// ページ個別出力
		m_rdoIndiv.EnableWindow(FALSE);
		// ダイアグラムの背景色を変更
		if(m_chkPrint.GetCheck() == TRUE)	{
			color = BC_DISABLE;
			//	入力項目を入力不可にする
			for(ii=14;ii<=21;ii++)	{
				// 属性を取得
				m_diagInput.GetAttribute(ii, (LPUNKNOWN)&diaatt);
				// 背景色を設定
				memcpy(&diaatt.attr_bcolor, &color, sizeof(COLORREF));
				// 属性を変更
				m_diagInput.SetAttribute(ii, (LPUNKNOWN)&diaatt, TRUE);
				m_diagInput.ModifyItem(ii,DIAG_MDFY_READONLY);
			}
		}
		else	{
			for(ii=14;ii<=21;ii++)	{
				m_diagInput.ModifyItem(ii,DIAG_MDFY_READONLY);
				SetAllBackColor( GetSysColor( COLOR_BTNSHADOW ));	// 入力不可のときはグレーアウト
			}
		}
		// ツールバー F7 ページ削除
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, FALSE);
		// ツールバー F7 ページ追加
		m_ToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F9, FALSE);
	}
// midori 160612 cor <--

// midori 154927 add -->
	// 通常と簡素化の場合でメッセージの番号を変更
	CString		cst=_T("");

	if(bG_Kanso == TRUE)	{
		cst.Format(_T("５．未払配当金又は未払役員賞与がある場合には、次の欄にその内訳を記入してください。"));
	}
	else	{
		cst.Format(_T("４．未払配当金又は未払役員賞与がある場合には、次の欄にその内訳を記入してください。"));
	}
	m_lblNote.UpdateCaption(cst, 0);
// midori 154927 add <--

	// リサイズ
	OnInitDialogEX();

// 修正No.157633_元号日付 add -->
	if(m_chkPrint.GetCheck() != FALSE && m_DataKakutei == FALSE)	{
		PostMessage(WM_USER_FOCUSSET2, 0, 0);
	}
// 修正No.157633_元号日付 add <--

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージをフィルタリング
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgKaikakekin::PreTranslateMessage(MSG* pMsg)
{
	char	cShift = 0;	//	SHIFTキー状態
	int		nID = 0;	//	ID
// midori 155866 add -->
	int		pos = 0;
	int		sw = 0;
// midori 155866 add <--
// 修正No.157633_元号日付 add -->
	int		nn = 0;
// 修正No.157633_元号日付 add <--

	nID = GetFocusControlID( this );			//	現在フォーカスのあるコントロールID取得

	//	キーダウンメッセージ？
	if ( pMsg->message == WM_KEYDOWN )	{
// midori 155866 add -->
		if( nID != IDC_PRINT_CHECK && nID != IDC_PRINT_COMBO && nID != IDC_RADIO_COMMON &&
			nID != IDC_RADIO_INDIV && nID != IDC_BUTTON_PREV && nID != IDC_BUTTON_NEXT &&
			nID != ID_OK_BUTTON && nID != IDCANCEL) {
			sw = 1;
		}
// midori 155866 add <--

		//	パラメータで分岐
		switch( pMsg->wParam ){

		//	「Tab」キーが押された
		case VK_TAB:
			
			cShift = ( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得
			
			//	SHIFTキーが押されてる？
			if ( cShift & 0x80 ){

				//	OKボタンにフォーカスあり
				if ( nID == ID_OK_BUTTON ){
					//	Diagの最終インデックスへ移動
// midori 160612 cor -->
//					m_diagInput.SetPosition( DK_INPUTINDEX_LAST );	
// ---------------------
					// 入力データを確定するにチェック無し
					if(m_DataKakutei == FALSE)	m_diagInput.SetPosition( DK_INPUTINDEX_LAST );	
// midori 160612 cor <--
					return( 1 );
				}
			}
			//	SHIFTキーが押されてない
			else{

				//	キャンセルボタンにフォーカスあり？
				if ( nID == IDCANCEL ){
					//	Diagの先頭インデックスへ移動
// midori 160612 cor -->
//					m_diagInput.SetPosition( DK_INPUTINDEX_FIRST );
// ---------------------
					// 入力データを確定するにチェック無し
					if(m_DataKakutei == FALSE)	m_diagInput.SetPosition( DK_INPUTINDEX_FIRST );
// midori 160612 cor <--
					return( 1 );
				}
			}
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
			
			//	OKボタンにフォーカスあり？
			if ( nID == ID_OK_BUTTON ){
				keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
				return( 1 );
			}

// midori 155866 add -->
			if(pMsg->wParam == VK_LEFT) {
				// リストにカーソルがある場合
				if(sw == 1)	{
					pos = m_diagInput.GetPosition();
					// pos(14、16、18、20):年月日入力 
					if(pos == 14 || pos == 16 || pos == 18 || pos == 20) {
						keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
						keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
						keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
						keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
						return( 1 );
					}
				}
			}
// midori 155866 add <--

// 修正No.157633_元号日付 add -->
			if(bG_Kanso == TRUE && pMsg->wParam == VK_UP)	{
				if(sw == 1)	{
					pos = m_diagInput.GetPosition();
					// pos(14、16、18、20):年月日入力 
					if(pos == 14 || pos == 16 || pos == 18 || pos == 20) {
						if(pos == 14)		nn = 0;
						else if(pos == 16)	nn = 1;
						else if(pos == 18)	nn = 2;
						else if(pos == 20)	nn = 3;

						m_OldDate[nn] = DiagGetDate(&m_diagInput, pos, DC_DATE_GWMD);
						m_OldGengo[nn] = DiagGetDate(&m_diagInput, pos, DC_DATE_GENGOU);

						m_GenListSw = 0;
					}
				}
			}
// 修正No.157633_元号日付 add <--
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

			//	キャンセルボタンにフォーカスあり？
			if ( nID == IDCANCEL ){
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				return( 1 );
			}

// midori 155866 add -->
			if(pMsg->wParam == VK_RIGHT)	{
				// リストにカーソルがある場合
				if(sw == 1)	{
					pos = m_diagInput.GetPosition();
					// pos(14、16、18、20):年月日入力
					if(pos == 14 || pos == 16 || pos == 18 || pos == 20)	{
						keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
						keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
						return( 1 );
					}
				}
			}
// midori 155866 add <--

// 修正No.157633_元号日付 add -->
			if(bG_Kanso == TRUE && pMsg->wParam == VK_DOWN)	{
				// リストにカーソルがある場合
				if(sw == 1)	{
					pos = m_diagInput.GetPosition();
					// pos(14、16、18、20):年月日入力
					if(pos == 14 || pos == 16 || pos == 18 || pos == 20)	{
						if(pos == 14)		nn = 0;
						else if(pos == 16)	nn = 1;
						else if(pos == 18)	nn = 2;
						else if(pos == 20)	nn = 3;

						m_OldDate[nn] = DiagGetDate(&m_diagInput, pos, DC_DATE_GWMD);
						m_OldGengo[nn] = DiagGetDate(&m_diagInput, pos, DC_DATE_GENGOU);

						m_GenListSw = 0;
					}
				}
			}
// 修正No.157633_元号日付 add <--
			break;

		//	F2キーが押された
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
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
		//	その他
		default:
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKaikakekin::OnBnClickedOk()
{	
	// 入力ボックスを消去する（消去した際にEditOFFが発生）
	m_diagInput.DeleteInput(); 
	//	データ書き込み
	Write();
	//	戻値を設定
	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKaikakekin::OnOK()
{
//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	エディットオン（ICSDiag）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKaikakekin::EditONKaikakekinDiag(short index)
{
// 修正No.157633_元号日付 add -->
	if(bG_Kanso == TRUE)	{
		int		pos = m_diagInput.GetPosition();

		m_CurCol = pos * m_CurrentPage;				// 現在編集中のカラム
		if(m_OldCol < 0) {
			m_OldCol = m_CurCol;
			m_GenListSw = 0;
		}

		// 頁、行、列のいずれかが変わったらEditOFF発行カウントをクリア
		if(m_OldCol != m_CurCol)	{
			m_GenListSw = 0;
		}

		// pos(14、16、18、20):年月日入力 
		if(pos == 14 || pos == 16 || pos == 18 || pos == 20)	m_EditSign = 1;
		else													m_EditSign = -1;

		switch(pos)	{
			case 14:
				if(m_GenListSw == 0) {
					m_OldDate[0] = 0;
					m_OldGengo[0] = 0;
				}
				break;
			case  16:
				if(m_GenListSw == 0) {
					m_OldDate[1] = 0;
					m_OldGengo[1] = 0;
				}
				break;
			case 18:
				if(m_GenListSw == 0) {
					m_OldDate[2] = 0;
					m_OldGengo[2] = 0;
				}
				break;
			case 20:
				if(m_GenListSw == 0) {
					m_OldDate[3] = 0;
					m_OldGengo[3] = 0;
				}
				break;
			default:
				break;
		}

// 修正No.157856 add -->
		if(pos == 14 || pos == 16 || pos == 18 || pos == 20)	{
			// 日付が変更されたかをチェック
			if(DataCheck(pos) == 1)	{
				m_diagInput.SetPosition(pos);
			}

		}
// 修正No.157856 add <--
	}
// 修正No.157633_元号日付 add <--
}

// 修正No.157856 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	日付が変更されたかをチェック
//++++++++++++++++++++++++++++++++++++++++++++++++++
int CdlgKaikakekin::DataCheck(int pPos)
{
	int				ret = 0;		// 戻り値
	int				num = 0;
	char			chrDate[4];		// 変更前の日付格納
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	clsDate;		// 日付変換クラス

	ret = 0;

	switch(pPos)	{
		case 14:	num = 0;	break;
		case 16:	num = 1;	break;
		case 18:	num = 2;	break;
		case 20:	num = 0;	break;
		default:				break;
	}

	// データを取得
	m_clsFunc.DiagInit(&diadata);
	m_diagInput.GetData(pPos, (LPUNKNOWN)&diadata);

	// 前回入力時の日付を取得
	memset(chrDate,'\0', sizeof(chrDate));
	clsDate.Convert(m_bReDate[num], chrDate, DC_DATE_GWMD);

	if( chrDate[0] == diadata.data_day[0] &&
		chrDate[1] == diadata.data_day[1] &&
		chrDate[2] == diadata.data_day[2] &&
		chrDate[3] == diadata.data_day[3]) {
		ret = 0;
	}
	else {
		ret = 1;
		m_bReDate[num] = clsDate.Convert(diadata.data_day[0], diadata.data_day[1], diadata.data_day[2], diadata.data_day[3]);
	}

	return(ret);
}
// 修正No.157856 add <--

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	エディットオフ（ICSDiag）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKaikakekin::EditOFFKaikakekinDiag(short index)
{
	//	入力項目を強制的に確定
	SetEnterReDraw( &m_diagInput, index );

// 修正No.157633_元号日付 add -->
	if(bG_Kanso == TRUE && m_OldCol > 0)	{
		int		pos = m_diagInput.GetPosition();
		// pos(14、16、18、20):年月日入力 
		if(pos == 14 || pos == 16 || pos == 18 || pos == 20)	{
			// 元号+年月日の入力状況を取得
			Date_ChkAndSet(&m_diagInput, index, pos, 1);
			m_EditSign = 0;
		}
		else	{
			m_EditSign = -1;
		}
	}
	m_OldCol = m_CurCol;
// 修正No.157633_元号日付 add -->
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント（データ用）
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKaikakekin::TerminationKaikakekinDiag(short index, short nChar, short length, LPUNKNOWN data)
{
	char	cShift = 0;		//	SHIFTキー状態
	int		nMovePos;		//	移動位置
// 修正No.157633_元号日付 add -->
	int		pos = 0;
	int		nn = 0;
// 修正No.157633_元号日付 add <--

	//	仮想キーコードで分岐
	switch( nChar ){
	case VK_DELETE:	//	Delete
		//	データをクリア
		m_diagInput.DataClear( index, TRUE );
// 修正No.157633_元号日付 add -->
		if(bG_Kanso == TRUE)	{
			pos = m_diagInput.GetPosition();
			// pos(14、16、18、20):年月日入力 
			if(pos == 14 || pos == 16 || pos == 18 || pos == 20) {
				if(pos == 14)		nn = 0;
				else if(pos == 16)	nn = 1;
				else if(pos == 18)	nn = 2;
				else if(pos == 20)	nn = 3;

				m_OldDate[nn] = 0;
				m_OldGengo[nn] = 0;
			}

			m_GenListSw = 0;

			if(pos == 14 || pos == 16 || pos == 18 || pos == 20)	{
				m_diagInput.SetPosition(pos);
			}
		}
// 修正No.157633_元号日付 add -->
		break;

	case VK_TAB:	//	Tab
	case VK_RETURN:	//	Enter
	case VK_LEFT:	//	←
	case VK_RIGHT:	//	→
		//	SHIFTキー状態取得
		cShift = ( char )GetKeyState( VK_SHIFT );
		
		//	SHIFTキー押されてて且つ「Tab」キーが押されてる？
		if ((( cShift & 0x80 ) && ( nChar == VK_TAB )) || ( nChar == VK_LEFT )){
			//	先頭インデックス？
			if ( index == DK_INPUTINDEX_FIRST ){
				//	キャンセルボタンにフォーカス移動
				m_btnCancel.SetFocus(); 
			}
			else{
				//	一つ前の位置に移動
				m_diagInput.SetPrevPosition(); 
			}
		}
		else{
			//	最終インデックス？
			if ( index == DK_INPUTINDEX_LAST ){
				//	OKボタンにフォーカス移動
				m_btnOK.SetFocus(); 
			}
			else{
				//	一つ次のの位置に移動
				m_diagInput.SetNextPosition(); 
			}
		}
		break;

	case VK_UP:		//	↑
		
		//	オフセットで分岐
		switch( index ){
		//------------------------------
		//	支払確定年月日（先頭行）
		case DK_INPUTINDEX_FIRST:
		//	期末現在高（先頭行）
		case ( DK_INPUTINDEX_FIRST + 1 ):
		//------------------------------
			//	移動位置算出
//			nMovePos = DK_INPUTINDEX_LAST + ( index - DK_INPUTINDEX_FIRST - 1 );
			nMovePos = index;
			break;
		
		//	デフォルト
		default:
			//	移動位置算出
			nMovePos = index - 2;
			break;
		}
		//	移動
		m_diagInput.SetPosition( nMovePos ); 
		break;

	case VK_DOWN:	//	↓

		//	オフセットで分岐
		switch( index ){
		//------------------------------
		//	支払確定年月日（最終行）
		case ( DK_INPUTINDEX_LAST - 1 ):
		//	期末現在高（先頭行）
		case DK_INPUTINDEX_LAST:
		//------------------------------
			//	移動位置算出
//			nMovePos = DK_INPUTINDEX_FIRST + ( index - DK_INPUTINDEX_LAST + 1 );
			nMovePos = index;
			break;
		
		//	デフォルト
		default:
			//	移動位置算出
			nMovePos = index + 2;
			break;
		}
		//	移動
		m_diagInput.SetPosition( nMovePos ); 
		break;
	}
}

//**************************************************
//	データ設定
//	【引数】	なし
//	【戻値】	0以外		…	成功
//				0			…	失敗
//**************************************************
int CdlgKaikakekin::SetData()
{
	Read();

//	SyncPage();

	// 出力形式が「ページ共通共通」なら、強制的に1p目を表示させる
	if(m_rdoCommon.GetCheck())	m_CurrentPage = 1;
	
	//Disp(m_CurrentPage);
	MovePage(m_CurrentPage);

	// コントロールのenable/disanableの設定
	//SetAllControl( fFlag );
	OnBnClickedPrintCheck();

	return(0);
}

//**************************************************
//	日付設定
//	【引数】	pDiag		…	ICSDiagコントロールハンドル
//				nIndex		…	インデックス
//				nDate		…	日付（数値）
//	【戻値】	なし
//**************************************************
void CdlgKaikakekin::SetDate( CICSDiag* pDiag, int nIndex, int nDate )
{
	DIAGRAM_DATA	udData;		//	DIAGRAM_DATA構造体
	CdateConvert	clsDate;	//	日付変換クラス

	//	DIAGRAM_DATA構造体初期化
	InitDiag( &udData );
	//	データを取得
	pDiag->GetData( nIndex, ( LPUNKNOWN )&udData );
	//	日付変換
	clsDate.Convert( nDate );
	//	日付データを設定
// midori 200101 del -->
	//udData.data_day[0] = clsDate.m_cWareki; 
	//udData.data_day[1] = clsDate.m_cMonth;
	//udData.data_day[2] = clsDate.m_cDay;
	//udData.data_day[3] = 0x00;
// midori 200101 del <--
// midori 200101 add -->
	// 簡素化様式の場合、日付の入力方法を"元号 ××.××.××"に変更する
	if(bG_Kanso == TRUE) {
		udData.data_day[0] = clsDate.m_cGengou; 
		udData.data_day[1] = clsDate.m_cWareki; 
		udData.data_day[2] = clsDate.m_cMonth;
		udData.data_day[3] = clsDate.m_cDay;
	}
	else {
		udData.data_day[0] = clsDate.m_cWareki; 
		udData.data_day[1] = clsDate.m_cMonth;
		udData.data_day[2] = clsDate.m_cDay;
		udData.data_day[3] = 0x00;
	}
// midori 200101 add <--
	//	データを設定
	pDiag->SetData( nIndex, ( LPUNKNOWN )&udData );
}

//**************************************************
//	金額設定
//	【引数】	pDiag		…	ICSDiagコントロールハンドル
//				nIndex		…	インデックス
//				szMoney		…	金額
//	【戻値】	なし
//**************************************************
void CdlgKaikakekin::SetMoney( CICSDiag* pDiag, int nIndex, CString szMoney )
{
	DIAGRAM_DATA	udData;		//	DIAGRAM_DATA構造体
	
	//	金額がヌルじゃない？
	if ( szMoney != "" ){ 
		//	DIAGRAM_DATA構造体初期化
		InitDiag( &udData );
		//	データを取得
		pDiag->GetData( nIndex, ( LPUNKNOWN )&udData );
		//	金額値→倍長フィールド変換関数
		ChangeFieldValData( &udData.data_val[0], szMoney ); 
		//	データを設定
		pDiag->SetData( nIndex, ( LPUNKNOWN )&udData );
	}
	else{
		//	データをクリア
		pDiag->DataClear( nIndex, TRUE ); 
	}
}

//**************************************************
//	日付取得
//	【引数】	pDiag		…	ICSDiagコントロールハンドル
//				nIndex		…	インデックス
//				nRow		…	対象行
//	【戻値】	日付（数値）
//**************************************************
int CdlgKaikakekin::GetDate( CICSDiag* pDiag, int nIndex, int nRow )
{
	DIAGRAM_DATA	udData;		//	DIAGRAM_DATA構造体
	CdateConvert	clsDate;	//	日付変換クラス
	int				nDate = 0;	//	日付（数値）
	char			chrDate[4] = "";//	前回入力時の日付格納

	//	DIAGRAM_DATA構造体初期化
	InitDiag( &udData );
	memset( chrDate,'\0', sizeof(chrDate));

	//	データを取得
	pDiag->GetData( nIndex, ( LPUNKNOWN )&udData );

	// 前回入力時の日付を「和暦.月.日」に変換)
// midori 200101 del -->
//	clsDate.Convert( m_bSpDate[nRow-1], chrDate, DC_DATE_WMD );
// midori 200101 del <--
// midori 200101 add -->
	if(bG_Kanso == TRUE) {
		clsDate.Convert( m_bSpDate[nRow-1], chrDate, DC_DATE_GWMD );
	}
	else	{
		clsDate.Convert( m_bSpDate[nRow-1], chrDate, DC_DATE_WMD );
	}
// midori 200101 add <--

	// 変更前の「和暦.月.日」と現在の「和暦.月.日」を比較
	if ( chrDate[0] == udData.data_day[0] && 
		 chrDate[1] == udData.data_day[1] &&
		 chrDate[2] == udData.data_day[2] && 
		 chrDate[3] == udData.data_day[3] ){
		nDate = m_bSpDate[nRow-1];
	}
	else{
		//	日付変換
// midori 200101 del -->
//		nDate = clsDate.Convert( 0x00, udData.data_day[0], udData.data_day[1], udData.data_day[2] );
// midori 200101 del <--
// midori 200101 add -->
		if(bG_Kanso == TRUE) {
			nDate = clsDate.Convert( udData.data_day[0], udData.data_day[1], udData.data_day[2], udData.data_day[3] );
		}
		else	{
			nDate = clsDate.Convert( 0x00, udData.data_day[0], udData.data_day[1], udData.data_day[2] );
		}
// midori 200101 add <--
	}

	//	戻値を返す
	return( nDate );
}

// 修正No.157633_元号日付 add -->
//**************************************************
//	日付取得
//	【引数】	pDiag		…	ICSDiagコントロールハンドル
//				nIndex		…	インデックス
//	【戻値】	日付（数値）
//**************************************************
int CdlgKaikakekin::DiagGetDate( CICSDiag* pDiag, int nIndex, int intFlg )
{
	DIAGRAM_DATA	udData;		//	DIAGRAM_DATA構造体
	CdateConvert	clsDate;	//	日付変換クラス
	int				nDate = 0;	//	日付（数値）

	// DIAGRAM_DATA構造体初期化
	InitDiag( &udData );

	//	データを取得
	pDiag->GetData( nIndex, ( LPUNKNOWN )&udData );

	//	日付変換
	if(bG_Kanso == TRUE) {
		switch (intFlg) {
		case DC_DATE_GENGOU:
			nDate = udData.data_day[0];
			break;
		// 元号、和暦、月、日
		case DC_DATE_GWMD:
		default:
			nDate = clsDate.Convert( udData.data_day[0], udData.data_day[1], udData.data_day[2], udData.data_day[3] );
			break;	
		}
	}
	else	{
		nDate = clsDate.Convert( 0x00, udData.data_day[0], udData.data_day[1], udData.data_day[2] );
	}

	//	戻値を返す
	return( nDate );
}
// 修正No.157633_元号日付 add <--

//**************************************************
//	金額取得
//	【引数】	pDiag		…	ICSDiagコントロールハンドル
//				nIndex		…	インデックス
//	【戻値】	金額
//**************************************************
CString CdlgKaikakekin::GetMoney( CICSDiag* pDiag, int nIndex )
{
	DIAGRAM_DATA	udData;		//	DIAGRAM_DATA構造体
	CString			szMoney;
	
	//	初期化
	szMoney.Empty(); 
	//	DIAGRAM_DATA構造体初期化
	InitDiag( &udData );
	//	データを取得
	pDiag->GetData( nIndex, ( LPUNKNOWN )&udData );
	
	//	入力がヌルじゃない？
	if ( udData.data_edit != "" ){
		//	カンマ省略金額値取得
		GetNotCommaValData( &szMoney, &udData.data_val[0] );
		//	空行をトリミング
		szMoney.TrimLeft(); 
	}

	//	戻値を返す
	return( szMoney );
}


//************************************************************
//	入力項目を強制的に確定
//		※入力項目で入力後、「Enter／Tab」でデータを確定せずに
//	　	　マウスで他の項目に移動したら設定した入力形式にフォーマットされない問題対応
//	【引数】	pDiag	…	ICSDiagオブジェクトのポインタ
//				nIndex	…	コントロールのインデックス
//	【戻値】	0		…	成功
//				-1		…	失敗
//************************************************************
int CdlgKaikakekin::SetEnterReDraw( CICSDiag* pDiag, int nIndex )
{	
	int					nRet = 0;	//	戻値
	int					nRow = 0;	//  行数
	int					nDate = 0;	//	日付格納バッファ
	char				chrDate[4] = "";	// 前回入力の日付格納バッファ
	DIAGRAM_DATA		udData;		//	DIAGRAM_DATA構造体
	DIAGRAM_ATTRIBUTE	udAttr;		//	DIAGRAM_ATTRIBUTE構造体
	CdateConvert		clsDate;	//	日付変換クラス

	//	DIAGRAM_DATA構造体初期化
	InitDiag( &udData );
	//	DIAGRAM_ATTRIBUTE構造体初期化
	InitAttr( &udAttr );
	//	項目の属性取得
	pDiag->GetAttribute( nIndex, ( LPUNKNOWN )&udAttr );	
	//	項目のデータ取得
	nRet = pDiag->GetData( nIndex, ( LPUNKNOWN )&udData );
	
	//	データ取得成功？かつ入力がヌルじゃない？
	if (( nRet == 0 ) && ( udData.data_edit != "" )){

		// 日付欄なら日付変換実行(偶数なら日付欄、奇数なら金額欄)
		if ( GetControlCol( nIndex ) == DK_COL_092_SPDATE ){

// midori 181101 add -->
			if(IsValidKaigen() == TRUE) {
// midori 200101 del -->
				//if(udData.data_day[0] == 0x01 && udData.data_day[1] == 0x01 && udData.data_day[2] >= 0x01 && udData.data_day[2] <= 0x07) {
				//	memset(udData.data_day,0x00,sizeof(udData.data_day));
				//	pDiag->SetPosition(nIndex);
				//}
// midori 200101 del <--
// midori 200101 add -->
				if(bG_Kanso == TRUE) {
					// 明治1年9月8日以前が入力された場合は入力無しにする
					if(udData.data_day[0] == 0x01 && udData.data_day[1] == 0x01 && (udData.data_day[2] < 0x09 || (udData.data_day[2] == 0x09 && udData.data_day[3] < 0x08))) {
						memset(udData.data_day,0x00,sizeof(udData.data_day));
						pDiag->SetPosition(nIndex);
					}
				}
				else	{
					// 平成1年1月1日～7日は無いので処理を戻す
					if(udData.data_day[0] == 0x01 && udData.data_day[1] == 0x01 && udData.data_day[2] >= 0x01 && udData.data_day[2] <= 0x07) {
						memset(udData.data_day,0x00,sizeof(udData.data_day));
						pDiag->SetPosition(nIndex);
					}
				}
// midori 200101 add <--
			}
// midori 181101 add <--

			// バッファ初期化
			memset( chrDate,'\0', sizeof(chrDate));

			// 行数取得
			nRow = (( nIndex - DK_INPUTINDEX_FIRST ) / 2 );

// midori 200101 del -->
//			// 前回入力時の日付を「和暦.月.日」に変換)
//			clsDate.Convert( m_bSpDate[nRow], chrDate, DC_DATE_WMD );
//
//			// 日付変換
//			nDate = clsDate.Convert( 0x00, udData.data_day[0], udData.data_day[1], udData.data_day[2] );
// midori 200101 del <--
// midori 200101 add -->
			if(bG_Kanso == TRUE) {
				// 前回入力時の日付を「和暦.月.日」に変換)
				clsDate.Convert( m_bSpDate[nRow], chrDate, DC_DATE_GWMD );
				// 日付変換
				nDate = clsDate.Convert( udData.data_day[0], udData.data_day[1], udData.data_day[2], udData.data_day[3] );
			}
			else	{
				// 前回入力時の日付を「和暦.月.日」に変換)
				clsDate.Convert( m_bSpDate[nRow], chrDate, DC_DATE_WMD );
				// 日付変換
				nDate = clsDate.Convert( 0x00, udData.data_day[0], udData.data_day[1], udData.data_day[2] );
			}
// midori 200101 add <--

			// 変更前の「和暦.月.日」と現在の「和暦.月.日」を比較
			if ( chrDate[0] != udData.data_day[0] || 
				 chrDate[1] != udData.data_day[1] ||
				 chrDate[2] != udData.data_day[2] || 
				 chrDate[3] != udData.data_day[3] ){
					 m_bSpDate[nRow] = nDate;
			}

// 修正No.157856 add -->
			int		num = 0;
			// 日付チェック用
			if(nIndex == 14)		num = 0;
			else if(nIndex == 16)	num = 1;
			else if(nIndex == 18)	num = 2;
			else if(nIndex == 20)	num = 3;
			m_bReDate[num] = nDate;
// 修正No.157856 add <--

			// 変換した日付のセット
// midori 200101 del -->
			//udData.data_day[0] = clsDate.m_cWareki;
			//udData.data_day[1] = clsDate.m_cMonth;
			//udData.data_day[2] = clsDate.m_cDay;
// midori 200101 del <--
// midori 200101 add -->
			if(bG_Kanso == TRUE) {
				udData.data_day[0] = clsDate.m_cGengou;
				udData.data_day[1] = clsDate.m_cWareki;
				udData.data_day[2] = clsDate.m_cMonth;
				udData.data_day[3] = clsDate.m_cDay;
			}
			else	{
				udData.data_day[0] = clsDate.m_cWareki;
				udData.data_day[1] = clsDate.m_cMonth;
				udData.data_day[2] = clsDate.m_cDay;
				udData.data_day[3] = 0x00;
			}
// midori 200101 add <--
		}

		//	取得したデータをそのまま設定（これで再描画がかかる）
		nRet = pDiag->SetData( nIndex, ( LPUNKNOWN )&udData );
	}
	else{
		
		//	入力種別がエディット？
		if ( udAttr.attr_inpkind == 0 ){
			//	データをクリア
			pDiag->DataClear( nIndex, TRUE );
		}
	}

	//	戻値を返す
	return( nRet );
}


//**************************************************************
// DIAGRAM_DATA構造体初期化関数
//【引数】	*diadata	…	初期化構造体のハンドル
//【戻値】	なし
//***************************************************************
void CdlgKaikakekin::InitDiag( DIAGRAM_DATA *diadata ){
	
	// 初期化
	diadata->data_edit.Empty();
	diadata->data_disp.Empty();
	diadata->data_combo = 0;
	diadata->data_check = 0;
	memset( diadata->data_val, '\0', sizeof(diadata->data_val));
	memset( diadata->data_day,'\0', sizeof(diadata->data_day));
	diadata->data_imgdata = (char*)NULL;
	diadata->data_imgwidth = 0;
	diadata->data_imgheight = 0;
	diadata->text_combo.Empty();
	diadata->text_check.Empty();
	diadata->movepos = 0;
	
}

//**************************************************************
// DIAGRAM_ATTRIBUTE構造体初期化関数
//【引数】	*diadata	…	初期化構造体のハンドル
//【戻値】	なし
//***************************************************************
void CdlgKaikakekin::InitAttr( DIAGRAM_ATTRIBUTE *diaatt )
{
	// 初期化
	memset( &diaatt->attr_bcolor , '\0' , sizeof( diaatt->attr_bcolor ) );
	memset( &diaatt->attr_fcolor , '\0' , sizeof( diaatt->attr_fcolor ) );
	memset( &diaatt->attr_frcolor , '\0' , sizeof( diaatt->attr_frcolor ) );
	diaatt->attr_ofstx = 0;
	diaatt->attr_ofsty = 0;
	diaatt->attr_align = 0;
	diaatt->attr_linestyle = 0;
	diaatt->attr_linewidth = 0;
	memset( &diaatt->attr_lfcolor , '\0' , sizeof( diaatt->attr_lfcolor ) );
	memset( &diaatt->attr_lbcolor , '\0' , sizeof( diaatt->attr_lbcolor ) );
	diaatt->attr_rframe = 0;
	diaatt->attr_rwidth = 0;
	diaatt->attr_rheight = 0;
	diaatt->attr_inpkind = 0;
	diaatt->attr_inpmax = 0;
	diaatt->attr_linemax =0;
	diaatt->attr_frctmax = 0;
	diaatt->attr_editkind = 0;
	diaatt->attr_editattr = 0;
	diaatt->attr_editformat.Empty();
	diaatt->attr_dayattr = 0;
}

//********************************************************************************
//	金額値→倍長フィールド変換関数
//		IN		char*			倍長フィールド金額値格納用ポインタ
//				CString			金額値文字列
//		RET		なし
//********************************************************************************
void CdlgKaikakekin::ChangeFieldValData(char* chrVal, CString strVal)
{
	int		intDefn;				// 倍長フィールド保存用
	char	chrValData[32];

	// 現在の倍長フィールド取得
	intDefn = l_retn();
	// 倍長フィールド指定
	l_defn( 0x16 );

	// メモリクリア
	memset( chrVal, '\0', sizeof(char[6]) );

	// 文字列格納
	//strcpy( &chrValData[0] , strVal );
	strcpy_s( &chrValData[0] , sizeof(chrValData), strVal );
	
	// 金額値を倍長フィールドに変換
	l_input( &chrVal[0] , &chrValData[0]);
	
	// 倍長フィールドを元に戻す
	l_defn( intDefn );
}

//********************************************************************************
//	カンマ省略金額値取得関数　※カンマ付の金額値ではデータベースに保存できないため
//	【引数】	strVal	…	カンマ省略金額値格納用変数のポインタ
//				chrVal	…	倍長フィールド型金額値	
//	【戻り値】	なし
//********************************************************************************
void CdlgKaikakekin::GetNotCommaValData(CString* strVal, char* chrVal)
{
	char	chrData[32];				// 変換金額値格納用
	char*	hfmt = "ssssssssssssss9";	// 金額値書式フォーマット･･･現在テーブルは最大14桁
	int		intDefn;					// 倍長フィールド保存用

	// 現在の倍長フィールド取得
	intDefn = l_retn();
	// 倍長フィールド指定
	l_defn( 0x16 );

	// 倍長型金額値をカンマ抜き金額値に変換
	l_print( &chrData[0] , chrVal ,hfmt);
	
	// CString型変数に金額値を格納
	*strVal = chrData;

	// 倍長フィールドを元に戻す
	l_defn( intDefn );
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgKaikakekin::GetFocusControlID( CWnd* pParent /*=NULL*/)
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

//********************************************************************************
//	現在のコントロール番号から列番号を取得
//		IN		なし
//		RET		列番号
//********************************************************************************
int CdlgKaikakekin::GetControlCol( int nIndex )
{
	int			intRet;		// 取得列格納
	int			intPer;		// 余りを格納

	// 余りを格納
	intPer = (nIndex % DK_COLMAXDATA_092);

	// 列取得
	intRet = intPer + 1;

	return ( intRet );
}

// midori 160612 cor -->
////**************************************************
////	ダイアログ表示
////	【引数】	pDB				…	データベースのハンドル
////	【戻値】	ID_DLG_OK		…	OKが押された
////				ID_DLG_CANCEL	…	キャンセルが押された
////**************************************************
//int	CdlgKaikakekin::ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info )
// ---------------------
//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースのハンドル
//				pDataKakutei	…	入力データを確定する
//	【戻値】	ID_DLG_OK		…	OKが押された
//				ID_DLG_CANCEL	…	キャンセルが押された
//**************************************************
int	CdlgKaikakekin::ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info, BOOL pDataKakutei )
// midori 160612 cor <--
{
	m_nRet = ID_DLG_CANCEL;		//	戻値を初期化
	memset( m_bSpDate, 0, sizeof(m_bSpDate) );	// 前回入力した日付を初期化
// midori 160612 add -->
	m_DataKakutei = pDataKakutei;
// midori 160612 add <--

	//	データベースがヌルじゃない？
	if ( pDB != NULL ){
		
		//	データベース開いてる？
		if ( pDB->IsOpen() ){
			//	データベースハンドル参照
			m_pDB = pDB;
			m_CurrentPage = page_info.intCurPage;
			m_PageMaxFrom = page_info.intMaxPage;
			//	モーダルで表示
			DoModal();
		}
	}

	//	戻値を返す
	return( m_nRet );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	チェックボックスクリック（印字する）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgKaikakekin::OnBnClickedPrintCheck()
{
	m_vecRangai[m_CurrentPage - 1].isPrint = m_chkPrint.GetCheck();
	CheckPrintStatus();
}

//**************************************************
//	印字するチェックボックス確認
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgKaikakekin::CheckPrintStatus()
{
	//	印字するにチェックあり？
	if ( m_vecRangai[m_CurrentPage - 1].isPrint )	SetAllControl( TRUE );	//	全てのコントロール表示
	else											SetAllControl( FALSE );	//	全てのコントロール非表示
}

//**************************************************
//	全てのコントロールのenable/disenableの設定
//	【引数】	fFlag	…	表示フラグ（TRUE：表示する／FALSE：表示しない）
//	【戻値】	なし
//**************************************************
void CdlgKaikakekin::SetAllControl( BOOL fFlag )
{
// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	return;
// midori 160612 add <--

	// ダイアグラム状態設定
	m_diagInput.EnableWindow( fFlag );

	// ダイアグラム背景色設定
	if( fFlag == FALSE )		SetAllBackColor( GetSysColor( COLOR_BTNSHADOW ));	// 入力不可のときはグレーアウト
	else						SetAllBackColor( RGB(255, 255, 255) );

	// コンボボックス状態設定
	//m_cmbPrint.EnableWindow( fFlag );

	// ページ関連コントロール状態設定
	OnCbnSelchangePrintComboSub(1);
	//OnBnClickedRadioCommonSub();
}

//**************************************************
//	入力ダイアグラムの背景色の変更
//	【引数】	nIndex	…	インデックス
//	【戻値】	0			成功
//				-1			失敗
//**************************************************
int CdlgKaikakekin::SetAllBackColor( COLORREF nColor )
{
	int					nRet = -1;
	DIAGRAM_ATTRIBUTE	udAttr;		//	属性構造体

	for(int i=0; i<DK_INPUTINDEX_LAST+1; i++){
		switch ( i ){
			// 表示
			case 2:
			case 3:
			case 5:
			case 6:
			case 9:
			case 10:
			case 12:
			case 13:
				// 属性構造体初期化
				InitAttr( &udAttr );

				// 属性情報取得
				m_diagInput.GetAttribute( i, ( LPUNKNOWN )&udAttr );	

				// 色情報セット
				udAttr.attr_bcolor  = nColor;

				// 属性情報セット
				m_diagInput.SetAttribute( i, ( LPUNKNOWN )&udAttr, TRUE );
				break;
			// 入力
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
				// 属性構造体初期化
				InitAttr( &udAttr );

				// 属性情報取得
				m_diagInput.GetAttribute( i, ( LPUNKNOWN )&udAttr );	

				// 色情報セット
				udAttr.attr_bcolor  = nColor;
				udAttr.attr_frcolor  = nColor;

				// 属性情報セット
				m_diagInput.SetAttribute( i, ( LPUNKNOWN )&udAttr, TRUE );
				break;
		}
	}

	return nRet;
}
void CdlgKaikakekin::OnCbnSelchangePrintCombo()
{
	OnCbnSelchangePrintComboSub(0);
}

void CdlgKaikakekin::OnCbnSelchangePrintComboSub(BOOL mode)
{
	BOOL state = FALSE;
	int cur_sel = m_cmbPrint.GetCurSel();

	if(cur_sel == 2)	state = TRUE;	// 全ページ
	else				state = FALSE;

	SetPrintTypeControlls(state);

	if(mode == 1)	return;

	// 「全ページ」以外なら強制的にページ共通出力
	if(state == FALSE){
		m_rdoCommon.SetCheck(TRUE);
		m_rdoIndiv.SetCheck(FALSE);

		if(m_CurrentPage == 1)	return;

		UpdateVectorAll();
		MovePage(1);
	}
}

void CdlgKaikakekin::SetPrintTypeControlls(BOOL fFlag)
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

void CdlgKaikakekin::SetPageControlls(BOOL fFlag)
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

void CdlgKaikakekin::OnBnClickedRadioCommon()
{
	OnBnClickedRadioCommonSub();

	UpdateVectorAll();

	MovePage(1);
}

void CdlgKaikakekin::OnBnClickedRadioCommonSub()
{
	if(m_rdoCommon.GetCheck())	SetPageControlls(FALSE);
	else						SetPageControlls(TRUE);
}

void CdlgKaikakekin::OnBnClickedRadioIndiv()
{
	if(m_rdoIndiv.GetCheck())	SetPageControlls(TRUE);
	else						SetPageControlls(FALSE);
}

int CdlgKaikakekin::SetToolBar()
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

void CdlgKaikakekin::OnToolButtonF7()
{
	if(m_ToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F7) == 0)		return;

	// 確認ダイアログ
	if( ICSMessageBox( _T("現在表示している欄外ページを削除します。\n\n削除してもよろしいですか？"), MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2 ) != IDOK ){
		return;
	}

	// 現在表示中のイテレータを取得
	vector<RangaiKaikake>::iterator it = m_vecRangai.begin() + m_CurrentPage - 1;
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

// 修正No.157633_元号日付 add -->
	if(bG_Kanso == TRUE)	{
		for(int ii = 0; ii < 5; ii++)	{
			m_OldDate[ii] = 0;		// 日付データ入力更新チェック用の値
			m_OldGengo[ii] = 0;		// 日付(元号番号)データ入力更新チェック用の値
		}
		m_GenListSw = 0;			// EditOFFに入ってきたカウント
	}
// 修正No.157633_元号日付 add <--

	Disp(m_CurrentPage);

	SetMoveButtonState();

	SetToolBarState();

// 修正No.157633_元号日付 add -->
	if(m_chkPrint.GetCheck() != FALSE && m_DataKakutei == FALSE)	{
		PostMessage(WM_USER_FOCUSSET2, 0, 0);
	}
// 修正No.157633_元号日付 add <--
}

void CdlgKaikakekin::OnToolButtonF9()
{
	if(m_ToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F9) == 0)		return;

	UpdateVectorAll();

	int MaxPageTemp = m_MaxPage + 1;

	RangaiKaikake rk;
	// 末尾に追加
	m_vecRangai.push_back(rk);

	m_MaxPage = m_vecRangai.size();
	m_CurrentPage = m_MaxPage;

// 修正No.157633_元号日付 add -->
	if(bG_Kanso == TRUE)	{
		for(int ii = 0; ii < 5; ii++)	{
			m_OldDate[ii] = 0;		// 日付データ入力更新チェック用の値
			m_OldGengo[ii] = 0;		// 日付(元号番号)データ入力更新チェック用の値
		}
		m_GenListSw = 0;			// EditOFFに入ってきたカウント
	}
// 修正No.157633_元号日付 add <--

	Disp(m_MaxPage);

	SetMoveButtonState();

	SetToolBarState();

	m_diagInput.SetPosition( DK_INPUTINDEX_FIRST );

// 修正No.157633_元号日付 add -->
	if(m_chkPrint.GetCheck() != FALSE && m_DataKakutei == FALSE)	{
		PostMessage(WM_USER_FOCUSSET2, 0, 0);
	}
// 修正No.157633_元号日付 add <--
}

BOOL CdlgKaikakekin::OnCommand(WPARAM wParam, LPARAM lParam)
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

void CdlgKaikakekin::OnBnClickedButtonPrev()
{
	if(m_btnMovePrev.IsWindowEnabled() == FALSE)	return;
	int old_page = m_CurrentPage;

	// 入力データをベクタに反映
	UpdateVectorAll();

	m_CurrentPage -= 1;
	if(m_CurrentPage < 1)	m_CurrentPage = 1;

	if(old_page == m_CurrentPage)	return;

	Disp(m_CurrentPage);

// 修正No.157633_元号日付 add -->
	if(bG_Kanso == TRUE)	{
		for(int ii = 0; ii < 5; ii++)	{
			m_OldDate[ii] = 0;		// 日付データ入力更新チェック用の値
			m_OldGengo[ii] = 0;		// 日付(元号番号)データ入力更新チェック用の値
		}
		m_GenListSw = 0;			// EditOFFに入ってきたカウント
	}
// 修正No.157633_元号日付 add <--

	SetMoveButtonState();

	// フォーカスが飛ぶのを防ぐ
//	if(m_btnMovePrev.IsWindowEnabled() == FALSE){	// このif文を入れてしまうと、1p印字しない 2p印字しないで開いて、F9頁追加し、印字する状態でPageUpで2pに戻った時灰色のダイアグラムインデックスにフォーカスが入ってしまう
		SetFocus();
//	}
}

void CdlgKaikakekin::OnBnClickedButtonNext()
{
	if(m_btnMoveNext.IsWindowEnabled() == FALSE)	return;
	int old_page = m_CurrentPage;

	// 入力データをベクタに反映
	UpdateVectorAll();

	m_CurrentPage += 1;
	if(m_CurrentPage > m_MaxPage)	m_CurrentPage = m_MaxPage;

	if(old_page == m_CurrentPage)	return;

	Disp(m_CurrentPage);

// 修正No.157633_元号日付 add -->
	if(bG_Kanso == TRUE)	{
		for(int ii = 0; ii < 5; ii++)	{
			m_OldDate[ii] = 0;		// 日付データ入力更新チェック用の値
			m_OldGengo[ii] = 0;		// 日付(元号番号)データ入力更新チェック用の値
		}
		m_GenListSw = 0;			// EditOFFに入ってきたカウント
	}
// 修正No.157633_元号日付 add <--

	SetMoveButtonState();

	// フォーカスが飛ぶのを防ぐ
//	if(m_btnMoveNext.IsWindowEnabled() == FALSE){	// このif文を入れてしまうと、1p印字しない 2p印字しないで開いて、F9頁追加し、印字する状態でPageUpで2pに戻った時灰色のダイアグラムインデックスにフォーカスが入ってしまう
		SetFocus();
//	}
}

void CdlgKaikakekin::Disp(int page)
{
// 修正No.157856 add -->
	int			num = 0;
// 修正No.157856 add <--

	// ダイアグラム
	RangaiKaikake rk = m_vecRangai[page - 1];
	for(vector<RANGAI_KAIKAKE_DATA>::iterator it_rkd = rk.vec_rkd.begin(); it_rkd != rk.vec_rkd.end(); it_rkd++){
		//	インデックスを取得
		int nIndex = DK_INPUTINDEX_FIRST + (( it_rkd->Row - 1 ) * 2 );
		//　オープン時の日付値を保持
		m_bSpDate[it_rkd->Row - 1] = it_rkd->SpData;
		//	日付設定
		SetDate( &m_diagInput, nIndex, it_rkd->SpData );
		//	金額設定
		SetMoney( &m_diagInput, ( nIndex + 1 ), it_rkd->Val );

// 修正No.157856 add -->
		// 日付チェック用
		m_bReDate[num] = 0;
		m_bReDate[num] = m_bSpDate[it_rkd->Row - 1];
		num++;
// 修正No.157856 add <--
	}

	// チェックボックス
	BOOL fFlag = FALSE;
	if ( m_vecRangai[page - 1].isPrint )	fFlag = TRUE;
	else									fFlag = FALSE;
	m_chkPrint.SetCheck( fFlag );

	// ダイアグラム状態設定
// midori 160612 cor -->
//	m_diagInput.EnableWindow( fFlag );
//	if( fFlag == FALSE )		SetAllBackColor( GetSysColor( COLOR_BTNSHADOW ));	// 入力不可のときはグレーアウト
//	else						SetAllBackColor( RGB(255, 255, 255) );
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		m_diagInput.EnableWindow( fFlag );
		if( fFlag == FALSE )		SetAllBackColor( GetSysColor( COLOR_BTNSHADOW ));	// 入力不可のときはグレーアウト
		else						SetAllBackColor( RGB(255, 255, 255) );
	}
// midori 160612 cor <--

	// ページ
	DispPage(page);
}

void CdlgKaikakekin::DispPage(int page)
{
	CString str_page;
	str_page.Format("%d", page);
	m_txtCurrent.SetWindowTextA(str_page);

	str_page.Format("%d", m_MaxPage);
	m_txtMax.SetWindowTextA(str_page);
}

void CdlgKaikakekin::AddVec(int page, CdbUc092Kaikakekin2& mfcRec)
{	
	// ベクタに登録
	RangaiKaikake rk(mfcRec);
	m_vecRangai.push_back(rk);
}

int CdlgKaikakekin::Write()
{
	if(m_pDB == NULL)	return -1;
	CdbUc092Kaikakekin2	mfcRec(m_pDB);

	// 入力データをベクタに反映
	UpdateVectorAll();

	// ベクタの値をDBに反映
	int page = 1;
	for(vector<RangaiKaikake>::iterator it = m_vecRangai.begin(); it != m_vecRangai.end(); it++, page++){
		mfcRec.Init(page);
		UpdateRecord(mfcRec, it, page);
	}

	// レコード数より実データ数が少ない場合、レコードを削除
	if(m_MaxPageDB > m_MaxPage)		mfcRec.DeletePage(m_MaxPage + 1);

	return DB_ERR_OK;
}

int CdlgKaikakekin::UpdateRecord(CdbUc092Kaikakekin2& db, vector<RangaiKaikake>::iterator it, int page)
{
	vector<RANGAI_KAIKAKE_DATA> vec = it->vec_rkd;
	for(vector<RANGAI_KAIKAKE_DATA>::iterator it_rkd = vec.begin(); it_rkd != vec.end(); it_rkd++, db.MoveNext()){
		db.Edit();

		db.m_NumPage = page;
		db.m_NumRow	 = it_rkd->Row;
		db.m_SpDate	 = it_rkd->SpData;
		db.m_Val	 = it_rkd->Val;

		// 日付か金額のどちらかにデータが存在すれば、データ行とする
		if(( db.m_SpDate != 0 ) || ( db.m_Val.IsEmpty() == FALSE ))		db.m_FgFunc = 1;
		else															db.m_FgFunc = 0;

		// 各レコードに印字するかどうかを持つ
		if(it->isPrint) db.m_FgShow = 0;
		else			db.m_FgShow = ID_FGSHOW_KAIKAKEKIN;

		// 1ページ目のみにコンボボックスの値を持つ
		if(page == 1){
			CConvOutRangai cr;
			db.m_OutRangai = cr.ToDB(m_cmbPrint.GetCurSel(), m_rdoIndiv.GetCheck());
		}

		db.Update();
	}

	return DB_ERR_OK;
}

void CdlgKaikakekin::UpdateVector(int index)
{
	int row = m_mapIndex[index];
	long date = GetDate(&m_diagInput, index, row);
	CString val = GetMoney(&m_diagInput, (index + 1));

	RANGAI_KAIKAKE_DATA rkd(row, date, val);	
	m_vecRangai[m_CurrentPage - 1].vec_rkd[row - 1] = rkd;
}

void CdlgKaikakekin::UpdateVectorAll()
{
	int row = 0;
	for(vector<RANGAI_KAIKAKE_DATA>::iterator it = m_vecRangai[m_CurrentPage - 1].vec_rkd.begin(); it != m_vecRangai[m_CurrentPage - 1].vec_rkd.end(); it++, row++){
		UpdateVector(DK_INPUTINDEX_FIRST + (row * 2));
	}
	BOOL isPrint = m_chkPrint.GetCheck();
	m_vecRangai[m_CurrentPage - 1].isPrint = isPrint;
}

void CdlgKaikakekin::Read()
{
	if(m_pDB == NULL)	return;
	CdbUc092Kaikakekin2	mfcRec(m_pDB);

	mfcRec.Init();
	m_MaxPageDB = mfcRec.GetNumPage();
	// Read時点ではDBの最大ページイコールメモリの最大値ページ
	// 後にセットするが、SetAllControl()でm_MaxPageが参照される為ここでプリセットする
	m_MaxPage = m_MaxPageDB;

	for(int i=0; i<m_MaxPageDB; i++){
		if(mfcRec.Init(i+1) != DB_ERR_OK)	return;
		if(mfcRec.IsEOF()){
			mfcRec.Fin();
			return;
		}

		if(i==0){
			//	チェックボックス
			//BOOL fFlag = FALSE;
			//if ( mfcRec.m_FgShow == ID_FGSHOW_KAIKAKEKIN )	fFlag = FALSE;
			//else											fFlag = TRUE;
			//m_chkPrint.SetCheck( fFlag );

			//　コンボボックス
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

			// コントロールのenable/disanableの設定
			//SetAllControl( fFlag );
		}

		// ベクタに追加
		AddVec(i+1, mfcRec);
	}
	mfcRec.Fin();

	m_MaxPage = m_vecRangai.size();

	// 棚卸資産の内訳書のページ数が欄外登録のページ数より多い場合、カレントページを1ページ目にセット
	if(m_MaxPage < m_CurrentPage)	m_CurrentPage = 1;
}

void CdlgKaikakekin::SetToolBarState()
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

void CdlgKaikakekin::SetMoveButtonState()
{
	SetMovePrevButtonState();
	SetMoveNextButtonState();
}

// ページ戻りボタン状態設定
void CdlgKaikakekin::SetMovePrevButtonState()
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
void CdlgKaikakekin::SetMoveNextButtonState()
{
	BOOL state = FALSE;
	//if(m_rdoCommon.IsWindowEnabled() == FALSE)	state = FALSE;
	//else{
		if(m_CurrentPage == m_MaxPage)	state = FALSE;
		else							state = TRUE;
	//}

	m_btnMoveNext.EnableWindow(state);
}

void CdlgKaikakekin::MovePage(int page)
{
	m_CurrentPage = page;

	Disp(m_CurrentPage);
  
	SetMoveButtonState();

	if(m_rdoCommon.GetCheck())	SetPageControlls(FALSE);
	else						SetPageControlls(TRUE);
}

void CdlgKaikakekin::AddPage()
{
	RangaiKaikake rk;
	// 末尾に追加
	m_vecRangai.push_back(rk);

	m_MaxPage = m_vecRangai.size();
}

void CdlgKaikakekin::SyncPage()
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

// 修正No.157633_元号日付 add -->
// --------------------------------------------------------------------------------------------------------
//	「支払確定年月日」列なら入力された日付をチェック
// --------------------------------------------------------------------------------------------------------
//	【引数】	short index	インデックス値
//				short sign	呼ばれた処理	0:PreTranslateMessage
//											1:EditOFFYoushikidiag1
//											2:TerminationYoushikidiag1
// --------------------------------------------------------------------------------------------------------
void CdlgKaikakekin::Date_ChkAndSet(CICSDiag* pDiag, short index, int col, short sign)
{
	short			sv_sel=0;
	int				sv_index=0;
	int				day=0,gen=0;
	DIAGRAM_DATA	udData;
	CdateConvert	cDC;			// 日付変換クラス

	if(bG_Kanso != TRUE)									return;
	if(m_DataKakutei == TRUE)								return;
	if(col != 14 && col != 16 && col != 18 && col != 20)	return;

	if(m_EditSign == -1)									return;

	// 「支払確定年月日」列なら入力された日付を取得

	// DIAGRAM_DATA構造体初期化
	InitDiag( &udData );

	//	データを取得
	pDiag->GetData( index, ( LPUNKNOWN )&udData );

	gen = udData.data_day[0];
	day = cDC.Convert(udData.data_day[0], udData.data_day[1], udData.data_day[2], udData.data_day[3]);

	// EditOFFYoushikidiag1
	if(sign == 1)	{
		if(col == 14)	{		// 支払確定年月日1
			if(m_OldDate[0] > 0 || m_OldGengo[0] > 0)	{
				// 初期化
				m_clsFunc.DiagInit( &udData );

				if(m_GenListSw == 0 && m_OldDate[0] > 0 && day == 0)	{
					// 元号リストが表示されたことによりクリアされるので、
					// 保存している日付を再セット
					m_GenListSw++;

					cDC.Convert( m_OldDate[0], udData.data_day, DC_DATE_GWMD );
					if(cDC.m_nDate != 0)	pDiag->SetData(index, ( LPUNKNOWN )&udData);
					else					pDiag->DataClear(index, TRUE);

				}
				else if(m_GenListSw > 0)	{
					if(gen == 0 || m_EditSign == 0)	{
						m_OldDate[0] = 0;
						m_OldGengo[0] = 0;
						m_GenListSw = 0;

						pDiag->DataClear(index, TRUE);
					}
					else if(m_OldDate[0] != day)	{
						m_OldDate[0] = 0;
						m_OldGengo[0] = 0;
						m_GenListSw = 0;

						// 日付、もしくは元号が変更された
						cDC.Convert(day, udData.data_day, DC_DATE_GWMD);
						if(cDC.m_nDate != 0)	pDiag->SetData(index, (LPUNKNOWN)&udData);
						else					pDiag->DataClear(index, TRUE);
					}
				}
			}
		}
		else if(col == 16)	{		// 支払確定年月日2
			if(m_OldDate[1] > 0 || m_OldGengo[1] > 0)	{
				// 初期化
				m_clsFunc.DiagInit( &udData );

				if(m_GenListSw == 0 && m_OldDate[1] > 0 && day == 0)	{
					// 元号リストが表示されたことにより日付がクリアされるので、
					// 保存している日付を再セット
					m_GenListSw++;

					cDC.Convert( m_OldDate[1], udData.data_day, DC_DATE_GWMD );
					if(cDC.m_nDate != 0)	pDiag->SetData(index, ( LPUNKNOWN )&udData);
					else					pDiag->DataClear(index, TRUE);
				}
				else if(m_GenListSw > 0)	{
					if(gen == 0 || m_EditSign == 0)	{
						m_OldDate[1] = 0;
						m_OldGengo[1] = 0;
						m_GenListSw = 0;

						pDiag->DataClear(index, TRUE);
					}
					else if(m_OldDate[1] != day)	{
						m_OldDate[1] = 0;
						m_OldGengo[1] = 0;
						m_GenListSw = 0;

						// 日付もしくは元号が変更された
						cDC.Convert(day, udData.data_day, DC_DATE_GWMD);
						if(cDC.m_nDate != 0)	pDiag->SetData(index, (LPUNKNOWN)&udData);
						else					pDiag->DataClear(index, TRUE);
					}
				}
			}
		}
		else if(col == 18)	{		// 支払確定年月日3
			if(m_OldDate[2] > 0 || m_OldGengo[2] > 0)	{
				// 初期化
				m_clsFunc.DiagInit( &udData );

				if(m_GenListSw == 0 && m_OldDate[2] > 0 && day == 0)	{
					// 元号リストが表示されたことにより日付がクリアされるので、
					// 保存している日付を再セット
					m_GenListSw++;

					cDC.Convert( m_OldDate[2], udData.data_day, DC_DATE_GWMD );
					if(cDC.m_nDate != 0)	pDiag->SetData(index, ( LPUNKNOWN )&udData);
					else					pDiag->DataClear(index, TRUE);
				}
				else if(m_GenListSw > 0)	{
					if(gen == 0 || m_EditSign == 0)	{
						m_OldDate[2] = 0;
						m_OldGengo[2] = 0;
						m_GenListSw = 0;

						pDiag->DataClear(index, TRUE);
					}
					else if(m_OldDate[2] != day)	{
						m_OldDate[2] = 0;
						m_OldGengo[2] = 0;
						m_GenListSw = 0;

						// 日付、もしくは元号が変更された
						cDC.Convert(day, udData.data_day, DC_DATE_GWMD);
						if(cDC.m_nDate != 0)	pDiag->SetData(index, (LPUNKNOWN)&udData);
						else					pDiag->DataClear(index, TRUE);
					}
				}
			}
		}
		else if(col == 20)	{		// 支払確定年月日4
			if(m_OldDate[3] > 0 || m_OldGengo[3] > 0)	{
				// 初期化
				m_clsFunc.DiagInit( &udData );

				if(m_GenListSw == 0 && m_OldDate[3] > 0 && day == 0)	{
					// 元号リストが表示されたことにより日付がクリアされるので、
					// 保存している日付を再セット
					m_GenListSw++;

					cDC.Convert( m_OldDate[3], udData.data_day, DC_DATE_GWMD );
					if(cDC.m_nDate != 0)	pDiag->SetData(index, ( LPUNKNOWN )&udData);
					else					pDiag->DataClear(index, TRUE);
				}
				else if(m_GenListSw > 0)	{
					if(gen == 0 || m_EditSign == 0)	{
						m_OldDate[3] = 0;
						m_OldGengo[3] = 0;
						m_GenListSw = 0;

						pDiag->DataClear(index, TRUE);
					}
					else if(m_OldDate[3] != day)	{
						m_OldDate[3] = 0;
						m_OldGengo[3] = 0;
						m_GenListSw = 0;

						// 日付、もしくは元号が変更された
						cDC.Convert(day, udData.data_day, DC_DATE_GWMD);
						if(cDC.m_nDate != 0)	pDiag->SetData(index, (LPUNKNOWN)&udData);
						else					pDiag->DataClear(index, TRUE);
					}
				}
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
LRESULT CdlgKaikakekin::OnUserFocusset2(WPARAM wParam, LPARAM lParam)
{
	// 初回画面表示の時に、日付で下矢印キーを押下した時に日付がクリアされてしまうのを避けるために、
	// 一旦別のカーソルにセットしてから元の初期位置に戻す。
	m_diagInput.SetPosition( DK_INPUTINDEX_LAST );
	m_diagInput.SetPosition( DK_INPUTINDEX_FIRST );
	
	return(0);
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void CdlgKaikakekin::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	int pos = 0;

	switch (nState) {
	case WA_INACTIVE:
		pos = m_diagInput.GetPosition();
		// pos(14、16、18、20):年月日入力 
		if(bG_Kanso == TRUE && (pos == 14 || pos == 16 || pos == 18 || pos == 20))	{
			if(m_EditSign == 1)	{
				m_EditSign = -1;
				m_GenListSw = 0;
				if(pos == 14) {
					m_OldDate[0] = 0;
					m_OldGengo[0] = 0;
				}
				else if(pos == 16) {
					m_OldDate[1] = 0;
					m_OldGengo[1] = 0;
				}
				else if(pos == 18) {
					m_OldDate[2] = 0;
					m_OldGengo[2] = 0;
				}
				else {
					m_OldDate[3] = 0;
					m_OldGengo[3] = 0;
				}
				m_diagInput.SetPosition(pos);
			}
		}
		break;
	}

	ICSDialog::OnActivate(nState, pWndOther, bMinimized);
}
// 修正No.157633_元号日付 add <--
