// 改良No.22-0404,22-0812 -->
//リストのコントロールを ListInp から ICSReport に変更するため、従来のソースを
//IDD_DIALOG_REFERENCE_OLD、dlgReference_Old.cpp、dlgReference_Old.h で保存

//--------------------------------------------------
//	dlgReference_Old.cpp
//
//	2006.02.21～	
//--------------------------------------------------

// midori 161106 cor -->
// ※リソースプロパティの Visible を FALSE から TRUE に変更
// midori 161106 cor <--

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgReference_Old.h"
// midori 152165_2 add -->
#include "frmUc000Common.h"
// midori 152165_2 add <--

// midori 161109 add -->
BOOL	g_TabSw99;
// midori 161109 add <--

// CdlgReference_Old ダイアログ

IMPLEMENT_DYNAMIC(CdlgReference_Old, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgReference_Old::CdlgReference_Old(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgReference_Old::IDD, pParent)
{
// midori 152165_2 add -->
	m_pOwner = pParent;
// midori 152165_2 add <--
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgReference_Old::CdlgReference_Old( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
// midori 152165_2 add -->
	m_pOwner = pParent;
// midori 152165_2 add <--
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgReference_Old::~CdlgReference_Old()
{
}

void CdlgReference_Old::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REFERENCE_LIST, m_lstReference);
	DDX_Control(pDX, IDC_ENTRY_LABEL, m_lblEntry);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_TORIHIKICHANGE, m_cmbTorihikiChange);
	DDX_Control(pDX, IDC_ICSDIAG8_KANASEARCH, m_DiagKanaSearch);
	DDX_Control(pDX, IDC_CHECK_REG_AUTO, m_chkRegAuto);
}


BEGIN_MESSAGE_MAP(CdlgReference_Old, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgReference_Old::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TORIHIKICHANGE, &CdlgReference_Old::OnCbnSelchangeComboTorihikichange)
	ON_BN_CLICKED(IDC_CHECK_REG_AUTO, &CdlgReference_Old::OnBnClickedCheckRegAuto)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgReference_Old, ICSDialog)
	ON_EVENT(CdlgReference_Old, IDC_REFERENCE_LIST, 7, CdlgReference_Old::LButtonDblClickReferenceList, VTS_I2)
	ON_EVENT(CdlgReference_Old, IDC_REFERENCE_LIST, 10, CdlgReference_Old::SelChangeReferenceList, VTS_I2)
	ON_EVENT(CdlgReference_Old, IDC_REFERENCE_LIST, 6, CdlgReference_Old::LButtonClickReferenceList, VTS_I2)
	ON_EVENT(CdlgReference_Old, IDC_REFERENCE_LIST, 9, CdlgReference_Old::FocusedReferenceList, VTS_NONE)
	ON_EVENT(CdlgReference_Old, IDC_ICSDIAG8_KANASEARCH, 3, CdlgReference_Old::TerminationKanaSearchDiag, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
// midori 161109 add -->
	ON_EVENT(CdlgReference_Old, IDC_ICSDIAG8_KANASEARCH, 8, CdlgReference_Old::ComboSelKanaSearchDiag, VTS_I2 VTS_UNKNOWN)
// midori 161109 add <--
// midori M-16113002 add -->
	ON_EVENT(CdlgReference_Old, IDC_ICSDIAG8_KANASEARCH, 2, CdlgReference_Old::EditOFFIcsdiag8Kanasearch, VTS_I2)
// midori M-16113002 add <--
// midori 152165_2 add -->
	ON_EVENT(CdlgReference_Old, IDC_ICSDIAG8_KANASEARCH, 1, CdlgReference_Old::EditONIcsdiag8Kanasearch, VTS_I2)
// midori 152165_2 add <--
END_EVENTSINK_MAP()

void CdlgReference_Old::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgReference_Old::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReference_Old::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	//	ツールバーの設定成功？
	if ( SetToolBar() == 0 ){
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_FAKE, FALSE );
	}

// midori 161109 add -->
	// 財務dbクラス
	m_pZmSub	= ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

// midori 152135,152136 del -->
//	// 表示順の選択をowntbからゲット
//	Owntb_DataGet();
// midori 152135,152136 del <--
// midori 161109 add <--

	//	帳表で選択されている項目のシーケンス番号取得
	m_nItemSeq = GetItemSeq();
	//	リスト初期化
	InitList( m_nType );
// midori 161109 del -->
	////	リストデータ取得
	//GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq );
	////	初期位置移動
	//InitMovePosition( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nItemSeq );
// midori 161109 del <--
// midori 161109 add -->
// midori 152745 del -->
//	//	リストデータ取得
//	GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort );
//	//	初期位置移動
//	InitMovePosition( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_nItemSeq );
// midori 152745 del <--
// midori 152745 add -->
	//	リストデータ取得
	GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum] );
	//	初期位置移動
	InitMovePosition( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_nItemSeq );
// midori 152745 add <--
// midori 161109 add <--
	//　リスト以外のコントロールの初期化
	InitOtherControls( m_nType, m_nGrSeq);
	// リサイズ
	OnInitDialogEX();
// midori 161106 add -->
	// コンボボックスで「新規作成」が選択されて呼び出された場合は、追加ダイアログを呼び出す
	if(m_F9OnSw == TRUE)	{
		PostMessage( WM_COMMAND, ID_TOOL_BUTTON_F9 );
	}
// midori 161106 add <--
	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ウィンドウメッセージをフィルタリング
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReference_Old::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	long	nID = 0;	//	ID番号
	int		nCtrID = GetFocusControlID(this);
// midori 152165_2 add -->
	int		sw=0;
// midori 152165_2 add <--
	char	cShift = ( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得

	//	キーダウンメッセージ？
	if ( pMsg->message == WM_KEYDOWN ){

// midori 152165_2 add -->
		sw = 1;
		if(nCtrID == IDC_COMBO_TORIHIKICHANGE)	sw = 0;
		if(nCtrID == IDC_REFERENCE_LIST)		sw = 0;
		if(nCtrID == IDC_CHECK_REG_AUTO)		sw = 0;
		if(nCtrID == ID_OK_BUTTON)				sw = 0;
		if(nCtrID == IDCANCEL)					sw = 0;
		if(sw == 1)	{
			switch(pMsg->wParam)	{
			// 表示順コンボボックス上で←・→キーを押下した場合
			// 他と同じように選択値を変更せずフォーカス移動する
			case VK_LEFT:
			case VK_RIGHT:
				if(sw == 1)	{
					if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT)	{
						m_DiagKanaSearch.ComboDropDown(ID_DIAG_INDEX_INPUT_SORT,FALSE);
						((CfrmUc000Common *)m_pOwner)->CmnDiagSetCombo(&m_DiagKanaSearch,ID_DIAG_INDEX_INPUT_SORT,m_OldCombo);
						if(pMsg->wParam == VK_LEFT)		pMsg->wParam = VK_F2;
						if(pMsg->wParam == VK_RIGHT)	pMsg->wParam = VK_TAB;
					}
				}
				break;
			case VK_TAB:
// midori 152165_2 add -->
				// 下記不具合の対応
				// ①表示順のコンボにフォーカスがある状態で他のアプリに行き戻ってくる
				// ②その後、↓↑キーでコンボを変更してもコンボ自身の値が切り替わらない
				// 　(リストの並びは変わっている)
				// TabキーをENTERキーに置き換える
				if(!(cShift & 0x80))	{
					pMsg->wParam = VK_RETURN;
				}
				break;
			}
		}
// midori 152165_2 add <--

		//	パラメータで分岐
		switch( pMsg->wParam ){
		
		//	Enterキー
		case VK_RETURN:
			//	OnOk起動
//			nID = IDOK;
			
			//	リストに居る？
			if ( GetFocusControlID( this ) == IDC_REFERENCE_LIST ){
				//keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				//keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				m_btnOK.SetFocus();
				return 1;
			}
// midori M-16113003 add -->
			else if ( GetFocusControlID( this ) == IDC_COMBO_TORIHIKICHANGE ){
// midori 152165 cor -->
//				FocusSet(0);
//				ReDrawRowColor(1);
// ---------------------
				m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SORT );
// midori 152165 cor <--
				return 1;
			}
// midori M-16113003 add <--
			break;
		//　Tabキー
		case VK_TAB:
// midori 161109 add -->
			g_TabSw99 = FALSE;
// midori 161109 add <--
			switch( nCtrID ){
// midori 152165 add -->
				case ID_OK_BUTTON:
					//	Tab
					if ( !(cShift & 0x80) ){
						m_btnCancel.SetFocus();
						return(1);
					}
					break;
// midori 152165 add <--
				case IDCANCEL:
					//	SHIFTキーが押されてる？
					if ( !(cShift & 0x80) ){
						if( m_nType == ID_DLGTYPE_ADDRESS ){
							m_cmbTorihikiChange.SetFocus();
						}
						else{
// midori 161109 del -->
//							m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
// midori 161109 del <--
// midori 161109 add -->
							m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SORT );
// midori 161109 add <--
						}
						return(1);
					}
// midori M-16113003 add -->
					else	{
						if(m_lstReference.GetCount() <= 0)	{
							if(m_chkRegAuto.IsWindowVisible() == FALSE)	{
								FocusSet(1);
								return(1);
							}
						}
					}
// midori M-16113003 add <--
					break;

				case IDC_COMBO_TORIHIKICHANGE:
					//	SHIFTキーが押されてる？
					if ( cShift & 0x80 ){
						m_btnCancel.SetFocus();
					}
					else{
// midori 161109 del -->
//						m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
// midori 161109 del <--
// midori 161109 add -->
						m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SORT );
// midori 161109 add <--
					}
					return(1);
					break;
// midori M-16113003 add -->
				case IDC_CHECK_REG_AUTO:
					//	SHIFTキーが押されてる？
					if ( cShift & 0x80 ){
						FocusSet(1);
						return(1);
					}
					break;
// midori M-16113003 add <--

				case IDC_REFERENCE_LIST:
					//	SHIFTキーが押されてる？
					if ( cShift & 0x80 ){
						if( m_nType == ID_DLGTYPE_BANK ){
							m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SUB );
						}
						else{
							m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
						}
						//return(1);
					}
					break;

				case IDC_ICSDIAG8_KANASEARCH:		// ｶﾅ検索ダイアログ
						//	SHIFTキーが押されてる？
					if ( cShift & 0x80 ){
// midori 161109 del -->
						//if( m_nType == ID_DLGTYPE_BANK ){	// 検索ボックス2つ
						//	if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN )
						//		m_btnCancel.SetFocus();
						//	else
						//		m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
						//}
						//else{	// 検索ボックス1つ
						//	m_btnCancel.SetFocus();
						//}
// midori 161109 del <--
// midori 161109 add -->
						if( m_nType == ID_DLGTYPE_BANK ){	// 検索ボックス2つ
							if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN )	{
								m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SORT );
								g_TabSw99 = TRUE;
							}
							else if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SUB )	{
								m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
							}
							else	{
								m_btnCancel.SetFocus();
							}
						}
						else{	// 検索ボックス1つ
							if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN )	{
								m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SORT );
								g_TabSw99 = TRUE;
							}
							else	{
								m_btnCancel.SetFocus();
							}
						}
// midori 161109 add <--
						//return(1);
					}
					else{
// midori 161109 del -->
						//if( m_nType == ID_DLGTYPE_BANK ){	// 検索ボックス2つ
						//	if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN )
						//		m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SUB );
						//	else
						//		m_lstReference.SetFocus();
						//}
						//else{	// 検索ボックス1つ
						//	m_lstReference.SetFocus();
						//}
// midori 161109 del <--
// midori 161109 add -->
						if( m_nType == ID_DLGTYPE_BANK ){	// 検索ボックス2つ
							if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT )	{
								m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
							}
							else if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN )	{
								m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SUB );
							}
							else	{
								FocusSet(0);
							}
						}
						else{	// 検索ボックス1つ
							if( m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT )	{
								m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
							}
							else	{
								FocusSet(0);
							}
						}
// midori 161109 add <--
					}
					break;
			}
			break;
		//	Escキー
		case VK_ESCAPE:
			//	OnCancel起動
			nID = IDCANCEL;
			break;
		//	F2キー
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );
			break;
		//	F6キー
		case VK_F6:
			nID = ID_TOOL_BUTTON_F6;
			break;
		//	F7キー
		case VK_F7:
			nID = ID_TOOL_BUTTON_F7;
			break;
		//	F8キー
		case VK_F8:
			nID = ID_TOOL_BUTTON_F8;
			break;
		//	F9キー
		case VK_F9:
			nID = ID_TOOL_BUTTON_F9;
			break;
		//	F11キー
		case VK_F11:
			nID = ID_TOOL_BUTTON_F11;
			break;
		//	F12キー
		case VK_F12:
			nID = ID_TOOL_BUTTON_F12;
			break;
		//	PageUp
		case VK_PRIOR:
		//	PageDown
		case VK_NEXT:
			//	現在行確認
			CheckNowRowPosition();
			break;
		//	その他キー
		default:
			break;
		}
	}

	//	CICSListInpのスクロールイベントが取得出来なかったため
	//	解決方法が見つかるまで暫定処理として
	//	マウス移動イベントで現在行の確認処理を実装
	else if (( pMsg->message == WM_NCMOUSEMOVE ) || ( pMsg->message == WM_MOUSEMOVE )){
		//	現在行確認
		CheckNowRowPosition();
	}

	//	ID番号あり？
	if ( nID != 0 ){
		//	メッセージ投げる
		PostMessage( WM_COMMAND, nID );
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ マップを処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReference_Old::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
	//	パラメータで分岐
	switch( wParam ){
	//	[F2]
	case ID_TOOL_BUTTON_F2:
		PostMessage( WM_KEYDOWN, VK_F2 );
		return( 1 );
		break;
	//	[F6]編集
	case ID_TOOL_BUTTON_F6:
		OnToolButtonF6();
		break;
	//	[F7]削除
	case ID_TOOL_BUTTON_F7:
		OnToolButtonF7();
		break;
	//	[F8]挿入
	case ID_TOOL_BUTTON_F8:
		OnToolButtonF8();
		break;
	//	[F9]追加
	case ID_TOOL_BUTTON_F9:
		OnToolButtonF9();
		break;
	//	[F11]１行上へ
	case ID_TOOL_BUTTON_F11:
		OnToolButtonF11();
		break;
	//	[F12]１行下へ
	case ID_TOOL_BUTTON_F12:
		OnToolButtonF12();
		break;
	}
	return ICSDialog::OnCommand(wParam, lParam);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストクリック（ダブル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::LButtonDblClickReferenceList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	//	選択ボタンクリックイベント起動
	OnBnClickedOk();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（選択ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	//	OKボタン有効？
	if ( m_btnOK.IsWindowEnabled() == TRUE ){

		//	選択されたリストのレコード取得成功？
		if ( GetSelectRecord( m_pDB, m_nType, m_nFormSeq, m_nGrSeq ) == 0 ){
			//	戻値に選択ボタンが押されたをセット
			m_nRet = ID_DLG_OK;
		}

// midori 152135,152136 del -->
//// midori 161109 add -->
//		// 表示順の選択をowntbにセット
//		Owntb_DataSet();
//// midori 161109 add <--
// midori 152135,152136 del <--

		ICSDialog::OnOK();
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	セルが移動した
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::SelChangeReferenceList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	//	現在行確認
	CheckNowRowPosition();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	リストクリック（シングル）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::LButtonClickReferenceList(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	//	現在行確認
	CheckNowRowPosition();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	フォーカス（来た）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::FocusedReferenceList()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
	//	前回選択j行あり？
	if ( m_nOldRow >= 0 ){
		//	行の背景色を更新
// midori M-16113003 del -->
//		ReDrawRowColor();
// midori M-16113003 del <--
// midori M-16113003 add -->
		ReDrawRowColor(0);
// midori M-16113003 add <--
	}
}

//**************************************************
//	F6（編集）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::OnToolButtonF6() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	

	//	ボタン有効？
	if ( m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled( ID_TOOL_BUTTON_F6 ) != 0 ){
		//	レコードの編集あり？
// midori 152745 del -->
//// midori 161109 del -->
////		if ( SetRecordEdit( m_pDB, m_nType, m_nFormSeq, m_nGrSeq ) != 0 ){
//// midori 161109 del <--
//// midori 161109 add -->
//		if ( SetRecordEdit( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort ) != 0 ){
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
		if ( SetRecordEdit( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum] ) != 0 ){
// midori 152745 add <--
			//	更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

//**************************************************
//	F7（削除）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::OnToolButtonF7()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	int			nRet;		//	戻値
	CString		szMessage;	//	メッセージ

	//	初期化
	szMessage.Empty(); 

	//	ボタン有効？
	if ( m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled( ID_TOOL_BUTTON_F7 ) != 0 ){
		
		//	メッセージ設定
		szMessage = "削除してもよろしいですか？";
		
		//	種別で分岐
		switch( m_nType ){
		//	金融機関
		case ID_DLGTYPE_BANK:
		//	取引先
		case ID_DLGTYPE_ADDRESS:
			szMessage += "\r\n既に使用している場合、全帳表にて削除項目が空欄で表示されます。";
			break;

		//	科目
		case ID_DLGTYPE_KAMOKU:
			break;
		}

		//	削除確認OK？
		if ( ICSMessageBox( szMessage, MB_OKCANCEL | MB_DEFBUTTON2, 0, 0, this ) == IDOK ){
			
			//	レコードの削除
// midori 152745 del -->
//// midori 161109 del -->
////			nRet = SetRecordDelete( m_pDB, m_nType, m_nFormSeq, m_nGrSeq );
//// midori 161109 del <--
//// midori 161109 add -->
//			nRet = SetRecordDelete( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort );
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
			nRet = SetRecordDelete( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum] );
// midori 152745 add <--
			
			//	レコードの削除あり？
			if(nRet >= 0) {
				//	更新フラグをオン
				m_fUpdate = TRUE;
			}
			//	デフォルト科目？
			else if(nRet == -2) {
				//	削除不可をアナウンス
				//ICSMessageBox( _T( "規定科目なので削除できません。" ), MB_OK, 0, 0, this );
				ICSMessageBox( _T( "規定科目のため削除できません。" ), MB_OK, 0, 0, this );
			}
			else if(nRet == -3) {
				//	削除不可をアナウンス
				ICSMessageBox(_T("財務連動の対象となっているため、削除できません。"), MB_OK, 0, 0, this);
			}
		}
	}
}

//**************************************************
//	F8（挿入）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::OnToolButtonF8()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	//	ボタン有効？
	if ( m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled( ID_TOOL_BUTTON_F8 ) != 0 ){
		//	レコードの挿入あり？
// midori 152745 del -->
//// midori 161109 del -->
////		if ( SetRecordInsert( m_pDB, m_nType, m_nFormSeq, m_nGrSeq ) != 0 ){
//// midori 161109 del <--
//// midori 161109 add -->
//		if ( SetRecordInsert( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort ) != 0 ){
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
		if ( SetRecordInsert( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum] ) != 0 ){
// midori 152745 add <--
			//	更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

//**************************************************
//	F9（追加）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::OnToolButtonF9()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	//	ボタン有効？
	if ( m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled( ID_TOOL_BUTTON_F9 ) != 0 ){
		//	レコードの追加あり？
// midori 152745 del -->
//// midori 161109 del -->
////		if ( SetRecordInsert( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, TRUE ) != 0 ){
//// midori 161109 del <--
//// midori 161109 add -->
//		if ( SetRecordInsert( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, TRUE ) != 0 ){
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
		if ( SetRecordInsert( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], TRUE ) != 0 ){
// midori 152745 add <--
			//	更新フラグをオン
			m_fUpdate = TRUE;
			// フォーカスをリストコントロールへ
			m_lstReference.SetFocus();
		}
// midori 152201 add -->
		// コンボボックスで「新規作成」が選択されて呼び出された場合は、ダイアログを閉じる
		if(m_F9OnSw == TRUE)	{
			// データベースの更新有り
			if(m_fUpdate == TRUE)	{
				OnBnClickedOk();
			}
			else	{
				m_nRet = ID_DLG_CANCEL;
				EndDialog(ID_DLG_CANCEL);
			}
		}
// midori 152201 add <--
	}
}

//**************************************************
//	F11（１行上へ）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::OnToolButtonF11()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	//	ボタン有効？
	if ( m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled( ID_TOOL_BUTTON_F11 ) != 0 ){
		//	レコードを一行上へ移動した？
// midori 152745 del -->
//// midori 161109 del -->
////		if ( SetRecordUpDown( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, -1 ) != 0 ){
//// midori 161109 del <--
//// midori 161109 add -->
//		if ( SetRecordUpDown( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, -1 ) != 0 ){
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
		if ( SetRecordUpDown( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], -1 ) != 0 ){
// midori 152745 add <--
			//	更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

//**************************************************
//	F12（１行下へ）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::OnToolButtonF12()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	//	ボタン有効？
	if ( m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled( ID_TOOL_BUTTON_F12 ) != 0 ){
		//	レコードを一行下へ移動した？
// midori 152745 del -->
//// midori 161109 del -->
////		if ( SetRecordUpDown( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, 1 ) != 0 ){
//// midori 161109 del <--
//// midori 161109 add -->
//		if ( SetRecordUpDown( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, 1 ) != 0 ){
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
		if ( SetRecordUpDown( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], 1 ) != 0 ){
// midori 152745 add <--
			//	更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

//**************************************************
// ツールバーの設定
//	【引数】	なし
//	【戻値】	0		…	成功
//				0以外	…	失敗
//**************************************************
int CdlgReference_Old::SetToolBar()
{
	CRect	ClientStart;	//	クライアント開始矩形
	CRect	ClientNow;		//	クライアント現在矩形
	CPoint	Offset;			//	オフセット位置
	CRect	Child;			//	子ウィンドウ矩形
	CWnd*	pChild = NULL;	//	子ウィンドウハンドル
	CRect	Window;			//	ウィンドウ矩形
	
	//	ツールバー作成
	if ( m_tbrToolBar.Create( this ) == 0 ){
		//	エラー
		return( -1 );
	}
	
	//	ビットマップ読み込み
	if ( m_tbrToolBar.LoadToolBar( IDR_REFERENCE_BAR ) == 0 ){
		//	エラー
		return( -2 );
	}
	
	//------------------------------
	//	スタイル設定
	//		CBRS_TOOLTIPS	…	ツールチップ表示
	//		CBRS_FLYBY		…	メッセージテキストの更新
	//		CBRS_BORDER_ANY	…	フレームを任意に設定
	//------------------------------
	m_tbrToolBar.SetBarStyle( m_tbrToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_BORDER_ANY );
	//	拡張スタイルをフラットツールバーに設定
	m_tbrToolBar.ModifyStyle( 0, 0x0800 );
/*
	//	クライアント領域の矩形情報取得
	GetClientRect( ClientStart );
	//	クライアント領域での再配置/サイズ変更
	RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, ClientNow );	
	//	オフセット位置を取得
	Offset.x = ClientNow.left - ClientStart.left;
	Offset.y = ClientNow.top - ClientStart.top;
	
	//	最初の子ウィンドウ取得
	pChild = GetWindow( GW_CHILD );
	
	//	ハンドルがヌルになるまでループ
	while ( pChild != NULL ){

		//	矩形情報取得
		pChild->GetWindowRect( Child );
		//	スクリーン座標からクライアント座標に置き換え
		ScreenToClient( Child );
		//	指定オフセット分移動
		Child.OffsetRect( Offset );
		//	位置と寸法の変更
		pChild->MoveWindow( Child, FALSE );
		//	次のウィンドウ取得
		pChild = pChild->GetNextWindow();
	}
	
	//	ウィンドウの矩形情報取得
	GetWindowRect( Window );
	//	ウィンドウの右下隅の位置を修正
	Window.right += ClientStart.Width() - ClientNow.Width();
	Window.bottom += ClientStart.Height() - ClientNow.Height();
	//	位置と寸法の変更
	MoveWindow( Window, FALSE );
*/
	//	クライアント領域での再配置/サイズ変更
	RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0 );
	
	//	成功
	return( 0 );
}

//**************************************************
// リスト初期化
//	【引数】	nType	…	参照型
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::InitList( EnumIdDlgType nType )
{
	int					nCnt;			//	カウント用
// midori 161109 del -->
//	LINP_COLUMN			udColumn[6];	//	カラム構造体
//	LINP_VCOLORINFO		udGrid[6];		//	縦グリッド色構造体
// midori 161109 del <--
// midori 161109 add -->
	LINP_COLUMN			udColumn[7];	//	カラム構造体
	LINP_VCOLORINFO		udGrid[7];		//	縦グリッド色構造体
// midori 161109 add <--
	int					nColumnCnt = 0;	//	カラムカウント数
	LPSTR				szTitle;		//	タイトル
	
	//------------------------------
	//	0カラム目（非表示）
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
	udColumn[nColumnCnt].clm_max = 4;															//	入力最大桁数
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;						//	属性（※フォーカス移動のため読み取り属性を外す）
	udColumn[nColumnCnt].clm_width = 0;															//	幅（DLU）
	udColumn[nColumnCnt].clm_title = _T( "シーケンス番号" );									//	タイトル
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );									//	背景色
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );									//	文字色
	//	カラムカウント数インクリメント
	nColumnCnt++;

	//------------------------------
	//	1カラム目
	//------------------------------
	udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
	udColumn[nColumnCnt].clm_max = 4;															//	入力最大桁数
	udColumn[nColumnCnt].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;	//	属性
	udColumn[nColumnCnt].clm_width = 32;														//	幅（DLU）
	udColumn[nColumnCnt].clm_title = _T( "番号" );												//	タイトル
	udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );									//	背景色
	udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );									//	文字色
	//	カラムカウント数インクリメント
	nColumnCnt++;

	//	参照型で分岐	
	switch( nType ){
	//++++++++++++++++++++++++++++++
	//	金融機関
	//++++++++++++++++++++++++++++++
	case ID_DLGTYPE_BANK:

		//	最大カラム数
// midori 161109 del -->
//		m_nColumnMax = 4;
// midori 161109 del <--
// midori 161109 add -->
		m_nColumnMax = 6;
// midori 161109 add <--
		//	タイトル
		szTitle = "金融機関";

		//------------------------------
		//	2カラム目　金融機関名
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
		udColumn[nColumnCnt].clm_width = ( 12 * 8 ) + 8;
		udColumn[nColumnCnt].clm_title = _T( "金融機関名" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;

// midori 161109 add -->
		//------------------------------
		//	3カラム目　カナ
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_ZENHAN | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
		udColumn[nColumnCnt].clm_width = ( 3 * 8 ) + 16;
		udColumn[nColumnCnt].clm_title = _T( "カナ" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;
// midori 161109 add <--

		//------------------------------
		//	4カラム目　支店名
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
// midori 161109 del -->
//		udColumn[nColumnCnt].clm_width = ( 12 * 8 ) + 8 + 8;
// midori 161109 del <--
// midori 161109 add -->
		udColumn[nColumnCnt].clm_width = ( 12 * 8 ) + 8;
// midori 161109 add <--
		udColumn[nColumnCnt].clm_title = _T( "支店名" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;

// midori 161109 add -->
		//------------------------------
		//	5カラム目　カナ
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_ZENHAN | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
		udColumn[nColumnCnt].clm_width = ( 3 * 8 ) + 16;
		udColumn[nColumnCnt].clm_title = _T( "カナ" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;
// midori 161109 add <--

		break;

	//++++++++++++++++++++++++++++++
	//	取引先
	//++++++++++++++++++++++++++++++
	case ID_DLGTYPE_ADDRESS:
				
		//	最大カラム数
// midori 161109 del -->
//		m_nColumnMax = 6;
// midori 161109 del <--
// midori 161109 add -->
		m_nColumnMax = 7;
// midori 161109 add <--

		//	タイトル
		szTitle = "取引先";
		
		////	グループ番号
		//switch( m_nGrSeq ){
		////	（売）
		//case ID_ADDRESSGR_URI:
		//	szTitle = "取引先（得意先）";
		//	break;
		////	（買）
		//case ID_ADDRESSGR_KAI:
		//	szTitle = "取引先（仕入先）";
		//	break;
		////	その他
		//default:
		//	szTitle = "取引先";
		//	break;
		//}

		//------------------------------
		//	2カラム目　名称（上段）
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
// midori 152132 del -->
//// midori 161109 del -->
////		udColumn[nColumnCnt].clm_width = ( 10 * 8 ) + 8;
//// midori 161109 del <--
//// midori 161109 add -->
//		udColumn[nColumnCnt].clm_width = ( 10 * 8 ) + 44;
//// midori 161109 add <--
// midori 152132 del <--
// midori 152132 add -->
		udColumn[nColumnCnt].clm_width = ( 10 * 8 ) + 8;
// midori 152132 add <--
		udColumn[nColumnCnt].clm_title = _T( "名称（上段）" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;

		//------------------------------
		//	3カラム目　名称（下段）
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
// midori 152132 del -->
//// midori 161109 del -->
////		udColumn[nColumnCnt].clm_width = ( 10 * 8 ) + 8;
//// midori 161109 del <--
//// midori 161109 add -->
//		udColumn[nColumnCnt].clm_width = ( 10 * 8 ) + 44;
//// midori 161109 add <--
// midori 152132 del <--
// midori 152132 add -->
		udColumn[nColumnCnt].clm_width = ( 10 * 8 ) + 8;
// midori 152132 add <--
		udColumn[nColumnCnt].clm_title = _T( "名称（下段）" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;

// midori 161109 add -->
		//------------------------------
		//	4カラム目　カナ
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_ZENHAN | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
// midori 152132 del -->
//		udColumn[nColumnCnt].clm_width = ( 3 * 8 ) + 16;
// midori 152132 del <--
// midori 152132 add -->
		udColumn[nColumnCnt].clm_width = ( 3 * 8 ) + 8;
// midori 152132 add <--
		udColumn[nColumnCnt].clm_title = _T( "カナ" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;
// midori 161109 add <--

		//------------------------------
		//	4カラム目 所在地（上段）
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 20;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
		udColumn[nColumnCnt].clm_width = ( 21 * 8 ) + 8;
		udColumn[nColumnCnt].clm_title = _T( "所在地（上段）" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;
		//------------------------------
		//	5カラム目　所在地（下段）
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 20;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
		udColumn[nColumnCnt].clm_width = ( 21 * 8 ) + 8;
		udColumn[nColumnCnt].clm_title = _T( "所在地（下段）" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;
		break;
		
	//++++++++++++++++++++++++++++++
	//	科目
	//++++++++++++++++++++++++++++++
	case ID_DLGTYPE_KAMOKU:

		//	最大カラム数
// midori 161109 del -->
//		m_nColumnMax = 4;
// midori 161109 del <--
// midori 161109 add -->
		m_nColumnMax = 5;
// midori 161109 add <--
		//	タイトル
		szTitle = "科目";

		//------------------------------
		//	2カラム目　名称
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
// midori 161109 del -->
//		udColumn[nColumnCnt].clm_width = ( 25 * 8 ) + 8;
// midori 161109 del <--
// midori 161109 add -->
		udColumn[nColumnCnt].clm_width = ( 25 * 8 ) + 48;
// midori 161109 add <--
		udColumn[nColumnCnt].clm_title = _T( "名称" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;

// midori 161109 add -->
		//------------------------------
		//	3カラム目　カナ
		//------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;
		udColumn[nColumnCnt].clm_max = 10;
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_KANJI | LINP_ATTR_LEFT | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
		udColumn[nColumnCnt].clm_width = ( 3 * 8 ) + 16;
		udColumn[nColumnCnt].clm_title = _T( "カナ" );
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );
		//	カラムカウント数インクリメント
		nColumnCnt++;
// midori 161109 add <--

		//-----------------------------------------
		//	4カラム目（デフォルトフラグ・非表示）
		//-----------------------------------------
		udColumn[nColumnCnt].clm_kind = LINP_KIND_STRING;											//	入力種別
		udColumn[nColumnCnt].clm_max = 1;															//	入力最大桁数
		udColumn[nColumnCnt].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;	//	属性
		udColumn[nColumnCnt].clm_width = 0;															//	幅（DLU）
		udColumn[nColumnCnt].clm_title = _T( "デフォルトフラグ" );									//	タイトル
		udColumn[nColumnCnt].clm_tbcolor = RGB( 0x00, 0x80, 0x00 );									//	背景色
		udColumn[nColumnCnt].clm_ttcolor = RGB( 0xFF, 0xFF, 0xFF );									//	文字色
		//	カラムカウント数インクリメント
		nColumnCnt++;
		break;	
	}
	
	//	タイトル設定
	m_lblEntry.SetWindowText( szTitle );
	//	カラムの設定
	m_lstReference.SetColumn( m_nColumnMax, ( LPUNKNOWN )udColumn );
	//	横グリッド線設定
	m_lstReference.SetHorzGrid( 5, RGB( 0x00, 0xFF, 0xFF), LINP_GRID_SOLID );

	//	縦グリッド線数文ループ
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){
		udGrid[nCnt].pos = nCnt;						//	縦グリッド線を引くカラムのオフセット
		udGrid[nCnt].color = RGB( 0xFF, 0x00, 0x00 );	//	色
		udGrid[nCnt].type = LINP_GRID_SOLID;			//	線種
	}
	
	//	縦グリッド線設定
	m_lstReference.SetVartGrid( m_nColumnMax, ( LPUNKNOWN )udGrid );
}

//**************************************************
//	リストデータ取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nType		…	参照型
//				nGrSeq		…　グループ番号
// midori 161109 add -->
//				nSort		…	表示順
// midori 161109 add <--
//				strFilterKana	…　ｶﾅ検索用フィルタ
//	【戻値】	なし
//**************************************************
// midori 161109 del -->
//void CdlgReference_Old::GetListData( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, 
//								int nGrSeq, CString strFilterKana1/*=NULL*/, CString strFilterKana2/*=NULL*/ )
// midori 161109 del <--
// midori 161109 add -->
void CdlgReference_Old::GetListData( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
								CString strFilterKana1/*=NULL*/, CString strFilterKana2/*=NULL*/ )
// midori 161109 add <--
{
	long	nRet = 0;	//	戻値

	//	参照型で分岐
// midori 161109 del -->
	//switch( nType ){
	////	金融機関
	//case ID_DLGTYPE_BANK:
	//	//	金融機関アイテム取得
	//	nRet = GetKinyuukikanItem( pDB, strFilterKana1, strFilterKana2 );
	//	break;
	////	取引先
	//case ID_DLGTYPE_ADDRESS:
	//	//	取引先アイテム取得
	//	nRet = GetTorihikisakiItem( pDB, nGrSeq, strFilterKana1 );
	//	break;
	////	科目
	//case ID_DLGTYPE_KAMOKU:
	//	//	科目アイテム取得
	//	nRet = GetKamokuItem( pDB, nFormSeq, strFilterKana1 );
	//	break;
	//}
// midori 161109 del <--
// midori 161109 add -->
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
		//	金融機関アイテム取得
		nRet = GetKinyuukikanItem( pDB, nSort, strFilterKana1, strFilterKana2 );
		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:
		//	取引先アイテム取得
		nRet = GetTorihikisakiItem( pDB, nGrSeq, nSort, strFilterKana1 );
		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:
		//	科目アイテム取得
		nRet = GetKamokuItem( pDB, nFormSeq, nSort, strFilterKana1 );
		break;
	}
// midori 161109 add <--
	
	//	戻値で分岐
	switch( nRet ){
	//	0行
	case 0:
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F8, FALSE );	//	[F8]挿入	×
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, FALSE );	//	[F7]削除	×
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F6, FALSE );	//	[F6]編集		×
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, FALSE );//	[F11]一行上へ	×
		m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, FALSE );//	[F12]一行下へ	×
		m_btnOK.EnableWindow( FALSE );											//	OKボタン		×
		break;
	//	1行
	case 1:
// midori 161109 del -->
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F8, TRUE );	//	[F8]挿入	○
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, TRUE );	//	[F7]削除	○
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F6, TRUE );	//	[F6]編集		○
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, FALSE );//	[F11]一行上へ	×
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, FALSE );//	[F12]一行下へ	×
// midori 161109 del <--
// midori 161109 add -->
		// 番号順
		if(nSort == 0)	{
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F8, TRUE );		//	[F8]挿入		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, TRUE );		//	[F7]削除		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F6, TRUE );		//	[F6]編集		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, FALSE );	//	[F11]一行上へ	×
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, FALSE );	//	[F12]一行下へ	×
		}
		// 50音順
		else	{
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F8, FALSE );		//	[F8]挿入		×
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, TRUE );		//	[F7]削除		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F6, TRUE );		//	[F6]編集		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, FALSE );	//	[F11]一行上へ	×
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, FALSE );	//	[F12]一行下へ	×
		}
// midori 161109 add <--
		m_btnOK.EnableWindow( TRUE );											//	OKボタン		○
		break;
	//	1行以上
	default:
// midori 161109 del -->
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F8, TRUE );	//	[F8]挿入	○
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, TRUE );	//	[F7]削除	○
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F6, TRUE );	//	[F6]編集		○
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, TRUE );	//	[F11]一行上へ	○
		//m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, TRUE );	//	[F12]一行下へ	○
// midori 161109 del <--
// midori 161109 add -->
		// 番号順
		if(nSort == 0)	{
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F8, TRUE );		//	[F8]挿入		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, TRUE );		//	[F7]削除		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F6, TRUE );		//	[F6]編集		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, TRUE );		//	[F11]一行上へ	○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, TRUE );		//	[F12]一行下へ	○
		}
		// 50音順
		else	{
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F8, FALSE );		//	[F8]挿入		×
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F7, TRUE );		//	[F7]削除		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F6, TRUE );		//	[F6]編集		○
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, FALSE );	//	[F11]一行上へ	×
			m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, FALSE );	//	[F12]一行下へ	×
		}
// midori 161109 add <--
		m_btnOK.EnableWindow( TRUE );											//	OKボタン		○
		break;
	}
}

//**************************************************
//	リストを除くコントロールの初期化
//	【引数】	nType		…	参照型
//				nGrSeq		…　グループ番号
//	【戻値】	なし
//**************************************************
int CdlgReference_Old::InitOtherControls( EnumIdDlgType nType, int nGrSeq)
{
	//	グループ番号
	switch( nGrSeq ){
		//	（売）
		case ID_ADDRESSGR_URI:
			m_cmbTorihikiChange.SetCurSel( 1 );	//　コンボボックスに得意先をセット
			break;
			//	（買）
		case ID_ADDRESSGR_KAI:
			m_cmbTorihikiChange.SetCurSel( 0 );	//　コンボボックスに仕入先をセット
			break;
	}

	//　参照型が取引先以外は表示しない
	if( nType == ID_DLGTYPE_ADDRESS){
		m_cmbTorihikiChange.ShowWindow( TRUE );
	}
	else{
		m_cmbTorihikiChange.ShowWindow( FALSE );
	}

	// カナ検索ダイアグラム初期化
	SetInitKanaSearchCtrl( nType );

	// 「取引先、金融機関を自動で登録する」チェックボックス初期化
// midori 152165 del -->
	//if( nType == ID_DLGTYPE_BANK || nType == ID_DLGTYPE_BANK_REFERENCE || nType == ID_DLGTYPE_ADDRESS || nType == ID_DLGTYPE_ADDRESS_REFERENCE){
	//	BOOL state = BST_UNCHECKED;
	//	if(m_isRegAuto)	state = BST_CHECKED;
	//	m_chkRegAuto.SetCheck(state);
	//}
	//else m_chkRegAuto.ShowWindow(FALSE);
// midori 152165 del <--
// midori 152165 add -->
	if( nType == ID_DLGTYPE_BANK || nType == ID_DLGTYPE_BANK_REFERENCE || nType == ID_DLGTYPE_ADDRESS || nType == ID_DLGTYPE_ADDRESS_REFERENCE){
		BOOL state = BST_UNCHECKED;
		if(m_isRegAuto)	state = BST_CHECKED;
		m_chkRegAuto.SetCheck(state);
		m_chkRegAuto.EnableWindow(TRUE);
		m_chkRegAuto.ShowWindow(SW_SHOW);
	}
	else	{
		m_chkRegAuto.EnableWindow(FALSE);
		m_chkRegAuto.ShowWindow(SW_HIDE);
	}
// midori 152165 add <--

	return 0;
}

//**************************************************
//	金融機関アイテム取得
//	【引数】	pDB				…	データベースハンドル
// midori 161109 add -->
//				nSort			…	表示順
// midori 161109 add <--
//				strFilter2　	…　ｶﾅ検索用フィルタ（金融機関名）
//				strFilter1　	…　ｶﾅ検索用フィルタ（支店名）
//	【戻値】	行数
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::GetKinyuukikanItem( CDatabase* pDB, CString strFilter1, CString strFilter2 )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::GetKinyuukikanItem( CDatabase* pDB, int nSort, CString strFilter1, CString strFilter2 )
// midori 161109 add <--
{
	CdbUcLstBank	mfcRec( pDB );		//	uc_lst_bankテーブルクラス
// midori 161109 del -->
//	LINP_DATA		udData[4];			//	リストデータ構造体
// midori 161109 del <--
// midori 161109 add -->
	LINP_DATA		udData[6];			//	リストデータ構造体
// midori 161109 add <--
	int				nCnt;				//	カウント用
	char			szBuf[8];			//	文字列変換用バッファ
	long			nLine = 0;			//	行管理用
	
	//	リストデータ初期化
// midori 161109 del -->
//	for( nCnt = 0; nCnt < 4; nCnt++ ){
// midori 161109 del <--
// midori 161109 add -->
	for( nCnt = 0; nCnt < 6; nCnt++ ){
// midori 161109 add <--
		InitLinpData( &udData[nCnt] );
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}
	
	//	リストクリア
	m_lstReference.RemoveAll();

	try{
		
		//	初期化成功？
// midori 161109 del -->
//		if ( mfcRec.Init() == DB_ERR_OK ){
// midori 161109 del <--
// midori 161109 add -->
		if ( mfcRec.Init_M(nSort) == DB_ERR_OK ){
// midori 161109 add <--
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();		//	レコードを先頭に移動
				
				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
					BOOL bContinue1 = FALSE, bContinue2 = FALSE;
					// ｶﾅ検索が入力されている場合、フィルタリング
					if( !(strFilter1.IsEmpty()) ){
						CString buff1 = mfcRec.m_BkKana1;
						buff1 = buff1.Left( strFilter1.GetLength() );
						if( buff1.Compare( strFilter1 ) != 0 ){
							bContinue1 = TRUE;
						}
					}

					if( !(strFilter2.IsEmpty()) ){
						CString buff2 = mfcRec.m_BkKana2;
						buff2 = buff2.Left( strFilter2.GetLength() );
						if( buff2.Compare( strFilter2 ) != 0 ){
							bContinue2 = TRUE;
						}
					}

					if( bContinue1 != FALSE || bContinue2 != FALSE ){
						mfcRec.MoveNext();
						continue;
					}
					
					//	リストに1行追加
					m_lstReference.AddNewLine( TRUE );

					//	シーケンス番号を設定（非表示項目）
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec.m_Seq, szBuf, 10 );
					udData[0].dat_string = szBuf;
					m_lstReference.SetColumnData( ( short )nLine, 0, ( LPUNKNOWN )udData, TRUE );

					//	順序番号を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec.m_OrderNum, szBuf, 10 );
					udData[1].dat_string = szBuf;
					m_lstReference.SetColumnData( ( short )nLine, 1, ( LPUNKNOWN )udData, TRUE );
					
					//	金融機関名を設定
					udData[2].dat_string = mfcRec.m_BkName1;
					m_lstReference.SetColumnData( ( short )nLine, 2, ( LPUNKNOWN )udData, TRUE );
					
// midori 161109 del -->
//					//	支店名を設定
//					udData[3].dat_string = mfcRec.m_BkName2;
//					m_lstReference.SetColumnData( ( short )nLine, 3, ( LPUNKNOWN )udData, TRUE );
// midori 161109 del <--
// midori 161109 add -->
					//	カナを設定
					udData[3].dat_string = mfcRec.m_BkKana1;
					m_lstReference.SetColumnData( ( short )nLine, 3, ( LPUNKNOWN )udData, TRUE );

					//	支店名を設定
					udData[4].dat_string = mfcRec.m_BkName2;
					m_lstReference.SetColumnData( ( short )nLine, 4, ( LPUNKNOWN )udData, TRUE );

					//	カナを設定
					udData[5].dat_string = mfcRec.m_BkKana2;
					m_lstReference.SetColumnData( ( short )nLine, 5, ( LPUNKNOWN )udData, TRUE );
// midori 161109 add <--
					
					mfcRec.MoveNext();	//	次のレコードに移動
					nLine++;			//	行をインクリメント
				}	
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
	return( nLine );
}

//**************************************************
//	取引先アイテム取得
//	【引数】	pDB			…	データベースハンドル
//				nGrSeq		…	グループ番号
// midori 161109 add -->
//				nSort		…	表示順
// midori 161109 add <--
//				strFilter　	…　ｶﾅ検索用フィルタ
//	【戻値】	行数
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::GetTorihikisakiItem( CDatabase* pDB, int nGrSeq, CString strFilter )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::GetTorihikisakiItem( CDatabase* pDB, int nGrSeq, int nSort, CString strFilter )
// midori 161109 add <--
{
	CdbUcLstAddress		mfcRec( pDB );		//	uc_lst_addressテーブルクラス
// midori 161109 del -->
//	LINP_DATA			udData[6];			//	リストデータ構造体
// midori 161109 del <--
// midori 161109 add -->
	LINP_DATA			udData[7];			//	リストデータ構造体
// midori 161109 add <--
	int					nCnt;				//	カウント用
	char				szBuf[8];			//	文字列変換用バッファ
	long				nLine = 0;			//	行管理用
	
	//	リストデータ初期化
// midori 161109 del -->
//	for( nCnt = 0; nCnt < 6; nCnt++ ){
// midori 161109 del <--
// midori 161109 add -->
	for( nCnt = 0; nCnt < 7; nCnt++ ){
// midori 161109 add <--
		InitLinpData( &udData[nCnt] );
		udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
		udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
		udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
	}
	
	//	リストクリア
	m_lstReference.RemoveAll();

	try{
		
		//	初期化成功？
// midori 161109 del -->
//		if ( mfcRec.Init( nGrSeq ) == DB_ERR_OK ){
// midori 161109 del <--
// midori 161109 add -->
		if ( mfcRec.Init_M( nGrSeq, nSort ) == DB_ERR_OK ){
// midori 161109 add <--
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();		//	レコードを先頭に移動
				
				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
					// ｶﾅ検索が入力されている場合、フィルタリング
					if( !(strFilter.IsEmpty()) ){
						CString buff = mfcRec.m_AdKana;
						buff = buff.Left( strFilter.GetLength() );
						if( buff.Compare( strFilter ) != 0 ){
							mfcRec.MoveNext();
							continue;
						}
					}
					
					//	リストに1行追加
					m_lstReference.AddNewLine( TRUE );

					//	シーケンス番号を設定（非表示項目）
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec.m_AdSeq, szBuf, 10 );
					udData[0].dat_string = szBuf;
					m_lstReference.SetColumnData( ( short )nLine, 0, ( LPUNKNOWN )udData, TRUE );

					//	順序番号を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec.m_OrderNum, szBuf, 10 );
					udData[1].dat_string = szBuf;
					m_lstReference.SetColumnData( ( short )nLine, 1, ( LPUNKNOWN )udData, TRUE );
					
					//	取引先名を設定
					udData[2].dat_string = mfcRec.m_AdName1;
					m_lstReference.SetColumnData( ( short )nLine, 2, ( LPUNKNOWN )udData, TRUE );

					//	取引先名を設定
					udData[3].dat_string = mfcRec.m_AdName2;
					m_lstReference.SetColumnData( ( short )nLine, 3, ( LPUNKNOWN )udData, TRUE );
					
// midori 161109 del -->
//					//	住所を設定
//					udData[4].dat_string = mfcRec.m_AdAdd1;
//					m_lstReference.SetColumnData( ( short )nLine, 4, ( LPUNKNOWN )udData, TRUE );
//
//					//	住所を設定
//					udData[5].dat_string = mfcRec.m_AdAdd2;
//					m_lstReference.SetColumnData( ( short )nLine, 5, ( LPUNKNOWN )udData, TRUE );
// midori 161109 del <--
// midori 161109 add -->
					//	カナを設定
					udData[4].dat_string = mfcRec.m_AdKana;
					m_lstReference.SetColumnData( ( short )nLine, 4, ( LPUNKNOWN )udData, TRUE );

					//	住所を設定
					udData[5].dat_string = mfcRec.m_AdAdd1;
					m_lstReference.SetColumnData( ( short )nLine, 5, ( LPUNKNOWN )udData, TRUE );

					//	住所を設定
					udData[6].dat_string = mfcRec.m_AdAdd2;
					m_lstReference.SetColumnData( ( short )nLine, 6, ( LPUNKNOWN )udData, TRUE );
// midori 161109 add <--
					
					mfcRec.MoveNext();	//	次のレコードに移動
					nLine++;			//	行をインクリメント
				}	
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
	return( nLine );
}

//**************************************************
//	科目アイテム取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
// midori 161109 add -->
//				nSort		…	表示順
// midori 161109 add <--
//				strFilter　	…　ｶﾅ検索用フィルタ
//	【戻値】	行数
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::GetKamokuItem( CDatabase* pDB, short nFormSeq, CString strFilter )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::GetKamokuItem( CDatabase* pDB, short nFormSeq, int nSort, CString strFilter )
// midori 161109 add <--
{
	CdbUcLstKamoku	mfcRec( pDB );		//	uc_lst_kamokuテーブルクラス
// midori 161109 del -->
//	LINP_DATA		udData[4];			//	リストデータ構造体
// midori 161109 del <--
// midori 161109 add -->
	LINP_DATA		udData[5];			//	リストデータ構造体
// midori 161109 add <--
	int				nCnt;				//	カウント用
	char			szBuf[8];			//	文字列変換用バッファ
	long			nLine = 0;			//	行管理用
	
	//	リストデータ初期化
// midori 161109 del -->
//	for( nCnt = 0; nCnt < 4; nCnt++ ){
// midori 161109 del <--
// midori 161109 add -->
	for( nCnt = 0; nCnt < 5; nCnt++ ){
// midori 161109 add <--
		InitLinpData( &udData[nCnt] );
	}
	
	//	リストクリア
	m_lstReference.RemoveAll();

	try{
		
		//	初期化成功？
// midori 161109 del -->
//		if ( mfcRec.Init( nFormSeq ) == DB_ERR_OK ){
// midori 161109 del <--
// midori 161109 add -->
		if ( mfcRec.Init_M( nFormSeq, nSort ) == DB_ERR_OK ){
// midori 161109 add <--
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();		//	レコードを先頭に移動
				
				//	レコード終端までループ
				while( !mfcRec.IsEOF() ){
					// ｶﾅ検索が入力されている場合、フィルタリング
					if( !(strFilter.IsEmpty()) ){
						CString buff = mfcRec.m_KnKana;
						buff = buff.Left( strFilter.GetLength() );
						if( buff.Compare( strFilter ) != 0 ){
							mfcRec.MoveNext();
							continue;
						}
					}
					
					//	リストに1行追加
					m_lstReference.AddNewLine( TRUE );
					
					//	デフォルト科目？
					if ( mfcRec.m_FgDft == 1 ){
						
// midori 161109 del -->
//						for( nCnt = 0; nCnt < 4; nCnt++ ){
// midori 161109 del <--
// midori 161109 add -->
						for( nCnt = 0; nCnt < 5; nCnt++ ){
// midori 161109 add <--
							//	背景色変更
							udData[nCnt].dat_fc = PALETTERGB( 0x00, 0x00, 0x00 );	//	文字色
							udData[nCnt].dat_bc = PALETTERGB( 0xA6, 0XCA, 0xF0 );	//	背景色
							udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	属性
						}
					}
					else{
						//	背景色初期化
// midori 161109 del -->
//						for( nCnt = 0; nCnt < 4; nCnt++ ){
// midori 161109 del <--
// midori 161109 add -->
						for( nCnt = 0; nCnt < 5; nCnt++ ){
// midori 161109 add <--
							udData[nCnt].dat_attr = LINP_DISP_COLOR;				//	データラインの背景色と文字色を任意で指定
							udData[nCnt].dat_fc = GetSysColor( COLOR_WINDOWTEXT );	//	文字色
							udData[nCnt].dat_bc = GetSysColor( COLOR_WINDOW );		//	背景色
						}
					}

					//	科目シーケンス番号を設定（非表示項目）
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec.m_KnSeq, szBuf, 10 );
					udData[0].dat_string = szBuf;
					m_lstReference.SetColumnData( ( short )nLine, 0, ( LPUNKNOWN )udData, TRUE );

					//	順序番号を設定
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec.m_KnOrder, szBuf, 10 );
					udData[1].dat_string = szBuf;
					m_lstReference.SetColumnData( ( short )nLine, 1, ( LPUNKNOWN )udData, TRUE );
					
					//	科目名を設定
					udData[2].dat_string = mfcRec.m_KnName;
					m_lstReference.SetColumnData( ( short )nLine, 2, ( LPUNKNOWN )udData, TRUE );
					
// midori 161109 del -->
					////	デフォルトフラグを設定（非表示項目）
					//ZeroMemory( szBuf, sizeof( szBuf ) );
					//_itoa_s( mfcRec.m_FgDft, szBuf, 10 );
					//udData[3].dat_string = szBuf;
					//m_lstReference.SetColumnData( ( short )nLine, 3, ( LPUNKNOWN )udData, TRUE );
// midori 161109 del <--
// midori 161109 add -->
					//	カナを設定
					udData[3].dat_string = mfcRec.m_KnKana;
					m_lstReference.SetColumnData( ( short )nLine, 3, ( LPUNKNOWN )udData, TRUE );

					//	デフォルトフラグを設定（非表示項目）
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec.m_FgDft, szBuf, 10 );
					udData[4].dat_string = szBuf;
					m_lstReference.SetColumnData( ( short )nLine, 4, ( LPUNKNOWN )udData, TRUE );
// midori 161109 add <--

					mfcRec.MoveNext();	//	次のレコードに移動
					nLine++;			//	行をインクリメント
				}	
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
	return( nLine );
}

//**************************************************
//	レコードの編集
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
// midori 161109 add -->
//				nSort			…	表示順
// midori 161109 add <--
//				fSearch			…	銀行検索フラグ（TRUE：銀行検索する／FALSE：自分で編集する）
//	【戻値】	0				…	編集なし
//				0以外			…	編集あり
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::SetRecordEdit( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, BOOL fSearch /*=FALSE*/ )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::SetRecordEdit( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, BOOL fSearch /*=FALSE*/ )
// midori 161109 add <--
{
	long	nRet = 0;		//	戻値
	long	nTopIndex = 0;	//	トップインデックス
	
	//	参照型で分岐
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
		//	金融機関アイテム編集
		nRet = EditKinyuukikan( pDB, fSearch );
		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:
		//	取引先アイテム編集
		nRet = EditTorihikisaki( pDB, nGrSeq, fSearch );
		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:
		//	科目アイテム編集
		nRet = EditKamoku( pDB, nFormSeq );
		break;
	}
	
	//	編集された？
	if ( nRet != 0 ){
		//	前回選択行をクリア
		m_nOldRow = -1;
		//	リストのトップインデックスを保持
		nTopIndex = m_lstReference.GetTopIndex();
		//	リストの更新
// midori 161109 del -->
//		GetListData( pDB, nType, nFormSeq, nGrSeq, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 del <--
// midori 161109 add -->
		GetListData( pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 add <--
		//	位置移動
		MovePosition( nTopIndex, ( nRet -1 ) );
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	金融機関のレコード編集
//	【引数】	pDB				…	データベースハンドル
//				fSearch			…	銀行検索フラグ（TRUE：銀行検索する／FALSE：自分で編集する）
//	【戻値】	0				…	編集なし
//				0以外			…	編集した順序番号
//**************************************************
long CdlgReference_Old::EditKinyuukikan( CDatabase* pDB, BOOL fSearch /*=FALSE*/)
{
	long					nRet = 0;			//	戻値
	long					nIndex = 0;			//	リストインデックス
	//CdlgReferenceSub		dlgSub( this );		//	参照サブダイアログクラス
	CdlgReferenceSub_Old	dlgSub( this );		//	参照サブダイアログクラス
	CdbUcLstBank			mfcRec( pDB );		//	uc_lst_bankテーブルクラス
	long					nSeq = 0;			//	シーケンス番号
	_RD_BANK_OLD			udBank;				//	金融機関構造体
	int						nEdit = 1;			//	編集フラグ（0：編集した／0以外：未編集）
	ICSBank					clsBank;			//	銀行検索クラス
	UCHAR					cBkCode[2];			//	銀行コード
	UCHAR					cBcCode[2];			//	支店コード
	CString					szBuf[2];			//	バッファ
	
	//	リストのインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号を取得
	nSeq = GetSeq( nIndex );
	
	//	金融機関のレコード取得失敗？
	if ( GetKinnyuukikanRecord( pDB, nSeq, &udBank ) != 0 )
	{
		//	失敗
		return( nRet );
	}
	
	//	自分で編集する？
	if ( fSearch == FALSE ){

		//	変更あり？
		if ( dlgSub.ShowDialog( pDB, ID_DLGTYPE_BANK, ID_DLGREF_UPDATE, 
								"[F6]編集", udBank.m_OrderNum, 
								udBank.m_BkName1, udBank.m_BkName2, "", "", udBank.m_BkKana1, udBank.m_BkKana2 ) == ID_DLG_OK ){
			//	戻値を構造体に反映
			udBank.m_BkName1 = dlgSub.m_RetName1;
			udBank.m_BkName2 = dlgSub.m_RetName2;
			udBank.m_BkKana1 = dlgSub.m_RetKana1;
			udBank.m_BkKana2 = dlgSub.m_RetKana2;
			//	編集した
			nEdit = 0;
		}
	}
	//	銀行検索する
	else{
		
		//	初期化
		memset( cBkCode, 0xFF, sizeof( cBkCode ) );
		memset( cBcCode, 0xFF, sizeof( cBcCode ) );
		szBuf[0].Empty();
		szBuf[1].Empty();
		
		//	銀行検索してOKボタンが押された？
		if ( clsBank.BankSelect( cBkCode, cBcCode, &szBuf[0], &szBuf[1], 0, this ) == TRUE ){
			//	戻値を構造体に反映
			udBank.m_BkName1 = szBuf[0];
			udBank.m_BkName2 = szBuf[1];
			//	編集した
			nEdit = 0;
		}
	}
	
	//	編集した？
	if ( nEdit == 0 ){
		
		try{
			//	初期化成功？
			if ( mfcRec.RequerySeq( nSeq ) == DB_ERR_OK ){
				
				//	レコードあり？
				if ( !( mfcRec.IsEOF() ) ){
					
					mfcRec.MoveFirst();						//	レコードを先頭に移動
					mfcRec.Edit();							//	レコード編集開始
					mfcRec.m_BkName1 = udBank.m_BkName1;	//	金融機関名変更
					mfcRec.m_BkName2 = udBank.m_BkName2;	//	支店名変更
					mfcRec.m_BkKana1 = udBank.m_BkKana1;	//	金融機関ｶﾅ変更
					mfcRec.m_BkKana2 = udBank.m_BkKana2;	//	支店名ｶﾅ変更
					mfcRec.Update();						//	レコード更新
					//nRet = udBank.m_OrderNum;				//	戻値を順序番号に設定
					nRet = nIndex + 1;						//	戻値を順序番号に設定
				}
				//	レコード閉じる
				mfcRec.Fin();							
			}
		}
		catch(...){
			
			//	レコード開いている？
			if ( mfcRec.IsOpen() ){
				//	レコード閉じる
				mfcRec.Fin();	
			}
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	取引先のレコード編集
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
//				fSearch			…
//	【戻値】	0				…	編集なし
//				0以外			…	編集した順序番号
//**************************************************
long CdlgReference_Old::EditTorihikisaki( CDatabase* pDB, int nGrSeq, BOOL fSearch /*=FALSE*/ )
{
	long					nRet = 0;			//	戻値
	long					nIndex = 0;			//	リストインデックス
	//CdlgReferenceSub		dlgSub( this );		//	参照サブダイアログクラス
	CdlgReferenceSub_Old	dlgSub( this );		//	参照サブダイアログクラス
	CdbUcLstAddress			mfcRec( pDB );		//	uc_lst_addressテーブルクラス
	long					nSeq = 0;			//	シーケンス番号
	_RD_ADDRESS_OLD			udAddress;			//	取引先構造体
	int						nEdit = 1;			//	編集フラグ（0：編集した／0以外：未編集）
	CString					szBuf[2];			//	バッファ
	CdlgAddressSearch		clsAdd( this );		//	住所検索クラス
	
	//	リストのインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号を取得
	nSeq = GetSeq( nIndex );
	
	//	取引先のレコード取得失敗？
	if ( GetTorihikisakiRecord( pDB, nGrSeq, nSeq, &udAddress ) != 0 )
	{
		//	失敗
		return( nRet );
	}
	
	//	自分で編集する？
	if ( fSearch == FALSE ){

		//	変更あり？
		if ( dlgSub.ShowDialog( pDB, ID_DLGTYPE_ADDRESS, ID_DLGREF_UPDATE, 
								"[F6]編集", udAddress.m_OrderNum, 
								udAddress.m_AdName1, udAddress.m_AdName2, 
								udAddress.m_AdAdd1, udAddress.m_AdAdd2, udAddress.m_AdKana, "", 0, nGrSeq ) == ID_DLG_OK ){
			//	戻値を構造体に反映
			udAddress.m_AdName1 = dlgSub.m_RetName1;
			udAddress.m_AdName2 = dlgSub.m_RetName2;
			udAddress.m_AdAdd1 = dlgSub.m_RetAdd1;
			udAddress.m_AdAdd2 = dlgSub.m_RetAdd2;
			udAddress.m_AdKana = dlgSub.m_RetKana1;
			//	編集した
			nEdit = 0;
		}
	}
	//	住所検索あり
	else{
		
		szBuf[0].Empty();
		szBuf[1].Empty();
		
		//	住所検索ダイアログでOKが押された？
		if ( clsAdd.ShowDialog( 40, 40 ) == ID_DLG_OK ){
			//	戻値を構造体に反映
			udAddress.m_AdAdd1 = clsAdd.m_AdAdd1;
			udAddress.m_AdAdd2 = clsAdd.m_AdAdd2;
			//	編集した
			nEdit = 0;
		}
	}
	
	//	編集した？
	if ( nEdit == 0 ){

		try{
			//	初期化成功？
			if ( mfcRec.RequeryAdSeq( nGrSeq, nSeq ) == DB_ERR_OK ){
				
				//	レコードあり？
				if ( !( mfcRec.IsEOF() ) ){
					
					mfcRec.MoveFirst();						//	レコードを先頭に移動
					mfcRec.Edit();							//	レコード編集開始
					mfcRec.m_AdName1 = udAddress.m_AdName1;	//	名称変更
					mfcRec.m_AdName2 = udAddress.m_AdName2;	//	名称変更
					mfcRec.m_AdAdd1 = udAddress.m_AdAdd1;	//	所在地変更
					mfcRec.m_AdAdd2 = udAddress.m_AdAdd2;	//	所在地変更
					mfcRec.m_AdKana = udAddress.m_AdKana;	//	ｶﾅ変更
					mfcRec.Update();						//	レコード更新
					//nRet = udAddress.m_OrderNum;			//	戻値を順序番号に設定
					nRet = nIndex + 1;						//	戻値を順序番号に設定
				}
				//	レコード閉じる
				mfcRec.Fin();							
			}
		}
		catch(...){
			
			//	レコード開いている？
			if ( mfcRec.IsOpen() ){
				//	レコード閉じる
				mfcRec.Fin();	
			}
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	科目のレコード編集
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	編集なし
//				0以外		…	編集した順序番号
//**************************************************
long CdlgReference_Old::EditKamoku( CDatabase* pDB, short nFormSeq )
{
	long					nRet = 0;			//	戻値
	long					nIndex = 0;			//	リストインデックス
	//CdlgReferenceSub		dlgSub( this );		//	参照サブダイアログクラス
	CdlgReferenceSub_Old	dlgSub( this );		//	参照サブダイアログクラス
	CdbUcLstKamoku			mfcRec( pDB );		//	uc_lst_kamokuテーブルクラス
	long					nKnSeq = 0;			//	シーケンス番号
	_RD_KAMOKU_OLD			udKamoku;			//	科目構造体
	int						nInputLen;			//	入力文字数

	//	リストのインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号を取得
	nKnSeq = GetSeq( nIndex );
	
	//	科目のレコード取得失敗？
	if ( GetKamokuRecord( pDB, nFormSeq, nKnSeq, &udKamoku ) != 0 )
	{
		//	失敗
		return( nRet );
	}
	
	switch( nFormSeq ){

	//	⑩-2.源泉所得税預り金の内訳
	case ID_FORMNO_102:
		nInputLen = DR_INPUTLENGTH_GENSEN;
		break;
	//	⑫.土地の売上高等の内訳書
	case ID_FORMNO_121:
		nInputLen = DR_INPUTLENGTH_TOTIURI;
		break;
	//	⑮-1.地代家賃の内訳書
	case ID_FORMNO_151:
		nInputLen = DR_INPUTLENGTH_TIDAI;
		break;
	default:
		// 入力文字数を８文字に設定
		nInputLen = DR_INPUTLENGTH_NORMAL;
		break;
	}	

	//	変更あり？
	if ( dlgSub.ShowDialog( pDB, ID_DLGTYPE_KAMOKU, ID_DLGREF_UPDATE, 
							"[F6]編集", udKamoku.m_KnOrder, 
							udKamoku.m_KnName, "", "", "" , udKamoku.m_KnKana, "", nInputLen, nFormSeq ) == ID_DLG_OK ){
		
		try{
			//	初期化成功？
			if ( mfcRec.RequeryKnSeq( nFormSeq, nKnSeq ) == DB_ERR_OK ){
				
				//	レコードあり？
				if ( !( mfcRec.IsEOF() ) ){
					
					mfcRec.MoveFirst();						//	レコードを先頭に移動
					mfcRec.Edit();							//	レコード編集開始
					mfcRec.m_KnName = dlgSub.m_RetName1;	//	科目名変更
					mfcRec.m_KnKana = dlgSub.m_RetKana1;	//  科目ｶﾅ変更
					mfcRec.Update();						//	レコード更新
					//nRet = udKamoku.m_KnOrder;				//	戻値を順序番号に設定
					nRet = nIndex + 1;				//	戻値を順序番号に設定
				}
				//	レコード閉じる
				mfcRec.Fin();							
			}
		}
		catch(...){
			
			//	レコード開いている？
			if ( mfcRec.IsOpen() ){
				//	レコード閉じる
				mfcRec.Fin();	
			}
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	レコードの挿入（追加）
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nType			…	参照型
// midori 161109 add -->
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
// midori 161109 add <--
//				fAddLine		…	追加フラグ（TRUE：追加／FALSE：挿入）
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入あり
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::SetRecordInsert( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, BOOL fAddLine )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::SetRecordInsert( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, BOOL fAddLine )
// midori 161109 add <--
{
	long		nRet = 0;		//	戻値
	long		nTopIndex = 0;	//	トップインデックス
	CString		szTitle;		//	タイトル文字列
	
	//	初期化
	szTitle.Empty();

	//	追加フラグで分岐
	switch( fAddLine){
	//	追加
	case TRUE:
		szTitle = "[F9]追加";		
		break;
	//	挿入
	case FALSE:
		szTitle = "[F8]挿入";
		break;
	}

	//	参照型で分岐
// midori 161109 del -->
	//switch( nType ){
	////	金融機関
	//case ID_DLGTYPE_BANK:
	//	//	金融機関のレコード挿入（追加）		
	//	nRet = InsertKinyuukikan( pDB, szTitle, fAddLine );
	//	break;
	////	取引先
	//case ID_DLGTYPE_ADDRESS:
	//	//	取引先のレコード挿入（追加）		
	//	nRet = InsertTorihikisaki( pDB, nGrSeq, szTitle, fAddLine );
	//	break;
	////	科目
	//case ID_DLGTYPE_KAMOKU:
	//	//	科目のレコード挿入（追加）		
	//	nRet = InsertKamoku( pDB, nFormSeq, szTitle, fAddLine );
	//	break;
	//}
// midori 161109 del <--
// midori 161109 add -->
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
		//	金融機関のレコード挿入（追加）
		nRet = InsertKinyuukikan( pDB, nSort, szTitle, fAddLine );
		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:
		//	取引先のレコード挿入（追加）
		nRet = InsertTorihikisaki( pDB, nGrSeq, nSort, szTitle, fAddLine );
		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:
		//	科目のレコード挿入（追加）
		nRet = InsertKamoku( pDB, nFormSeq, nSort, szTitle, fAddLine );
		break;
	}
// midori 161109 add <--
	
	//	挿入された？
	if ( nRet != 0 ){
		//	前回選択行をクリア
		m_nOldRow = -1;
		//	リストのトップインデックスを保持
		nTopIndex = m_lstReference.GetTopIndex();
		//	リストの更新
// midori 161109 del -->
//		GetListData( pDB, nType, nFormSeq, nGrSeq, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 del <--
// midori 161109 add -->
		GetListData( pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 add <--
		//	位置移動
// midori 161109 del -->
//		MovePosition( nTopIndex, ( nRet -1 ) );
// midori 161109 del <--
// midori 161109 add -->
		MovePosition2( nRet );
// midori 161109 add <--
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	金融機関のレコード挿入（追加）
//	【引数】	pDB				…	データベースハンドル
// midori 161109 add -->
//				nSort			…	表示順
// midori 161109 add <--
//				szTitle			…	ダイアログタイトル
//				fAddLine		…	追加フラグ
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入した順序番号
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::InsertKinyuukikan( CDatabase* pDB, CString szTitle,  BOOL fAddLine )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::InsertKinyuukikan( CDatabase* pDB, int nSort, CString szTitle,  BOOL fAddLine )
// midori 161109 add <--
{
	long					nRet = 0;			//	戻値初期化
	long					nIndex = 0;			//	リストインデックス
	//CdlgReferenceSub		dlgSub( this );		//	参照サブダイアログクラス
	CdlgReferenceSub_Old	dlgSub( this );		//	参照サブダイアログクラス
	CdbUcLstBank			mfcRec( pDB );		//	uc_lst_bankテーブルクラス	
	long					nOrderNum = 0;		//	順序番号（数値）
	long					nSeq = 0;			//	シーケンス番号
	_RD_BANK_OLD			udBank;				//	金融機関構造体
	EnumIdDlgRefMode		nMode;				//	動作モード
	int						nIdxOffset = 1;		//　フォーカスのオフセット

	//	追加フラグで分岐
	switch( fAddLine){
	//	追加
	case TRUE:
// midori 161109 del -->
//		nIndex = m_lstReference.GetCount() - 1;		//	インデックス算出
// midori 161109 del <--
// midori 161109 add -->
		nIndex = GetListIndex2(pDB, ID_DLGTYPE_BANK, -1, -1, nSort);		//	インデックス算出
// midori 161109 add <--
		nMode = ID_DLGREF_APPEND;
		nIdxOffset++;
		break;
	//	挿入
	case FALSE:
		nIndex = GetListIndex();					//	賞味のインデックス取得
		nMode = ID_DLGREF_INSERT;
		break;
	}

	//	シーケンス番号取得
	nSeq = GetSeq( nIndex );
	//	金融機関レコード取得
	GetKinnyuukikanRecord( pDB, nSeq, &udBank );
	//	順序番号を取得
	nOrderNum = udBank.m_OrderNum;

	//	追加？
	if ( fAddLine == TRUE ){
		//	順序番号をインクリメント
		nOrderNum++;
	}

	//	変更あり？
	if ( dlgSub.ShowDialog( pDB, ID_DLGTYPE_BANK, nMode, szTitle, nOrderNum, "", "" ) == ID_DLG_OK ){
		
		try{
			
			//	順序番号振り直し成功？
			if ( mfcRec.ReNumberOrder( nOrderNum, 1 ) == DB_ERR_OK ){
				
				//	新規行追加成功？
				if ( mfcRec.CreateNewRecord() == DB_ERR_OK ){
					
					//	レコードあり？
					if ( !( mfcRec.IsEOF() ) ){
						mfcRec.MoveFirst();						//	レコードを先頭に移動
						mfcRec.Edit();							//	レコード編集開始
						mfcRec.m_OrderNum = nOrderNum;			//	順序番号設定
						mfcRec.m_BkName1 = dlgSub.m_RetName1;	//	金融機関名設定
						mfcRec.m_BkName2 = dlgSub.m_RetName2;	//	支店名設定
						mfcRec.m_BkKana1 = dlgSub.m_RetKana1;	//	金融機関名ｶﾅ設定
						mfcRec.m_BkKana2 = dlgSub.m_RetKana2;	//	支店名ｶﾅ設定
						mfcRec.m_FgDel = 0;						//	削除フラグ設定
						mfcRec.Update();						//	レコード更新
					}
					// NULLの場合、ここで''（未入力）に更新
					mfcRec.UpdateStrFieldAllForNull();
					//	レコード閉じる
					mfcRec.Fin();
					//	戻値に順序番号を設定
// midori 161109 del -->
					//nRet = nOrderNum;
					//nRet = nIndex + nIdxOffset;					// 現在選択中の行の1つ下を対象とするため
// midori 161109 del <--
// midori 161109 add -->
					nRet = nOrderNum;
// midori 161109 add <--
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
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	取引先のレコード挿入（追加）
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
// midori 161109 add -->
//				nSort			…	表示順
// midori 161109 add <--
//				szTitle			…	ダイアログタイトル
//				fAddLine		…	追加フラグ
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入した順序番号
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::InsertTorihikisaki( CDatabase* pDB, int nGrSeq, CString szTitle,  BOOL fAddLine )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::InsertTorihikisaki( CDatabase* pDB, int nGrSeq, int nSort, CString szTitle,  BOOL fAddLine )
// midori 161109 add <--
{
	long					nRet = 0;			//	戻値初期化
	long					nIndex = 0;			//	リストインデックス
	//CdlgReferenceSub		dlgSub( this );		//	参照サブダイアログクラス
	CdlgReferenceSub_Old	dlgSub( this );		//	参照サブダイアログクラス
	CdbUcLstAddress			mfcRec( pDB );		//	uc_lst_addressテーブルクラス	
	long					nOrderNum = 0;		//	順序番号（数値）
	long					nSeq = 0;			//	シーケンス番号
	_RD_ADDRESS_OLD			udAddress;			//	取引先構造体
	EnumIdDlgRefMode		nMode;				//	動作モード
	int						nIdxOffset = 1;		//　フォーカスのオフセット

	//	追加フラグで分岐
	switch( fAddLine){
	//	追加
	case TRUE:
// midori 161109 del -->
//		nIndex = m_lstReference.GetCount() - 1;		//	インデックス算出
// midori 161109 del <--
// midori 161109 add -->
		nIndex = GetListIndex2(pDB, ID_DLGTYPE_ADDRESS, nGrSeq, -1, nSort);		//	インデックス算出
// midori 161109 add <--
		nMode = ID_DLGREF_APPEND;
		nIdxOffset++;
		break;
	//	挿入
	case FALSE:
		nIndex = GetListIndex();					//	賞味のインデックス取得
		nMode = ID_DLGREF_INSERT;
		break;
	}

	//	シーケンス番号取得
	nSeq = GetSeq( nIndex );
	//	取引先レコード取得
	GetTorihikisakiRecord( pDB, nGrSeq, nSeq, &udAddress );
	//	順序番号を取得
	nOrderNum = udAddress.m_OrderNum;

	//	追加？
	if ( fAddLine == TRUE ){
		//	順序番号をインクリメント
		nOrderNum++;
	}

	//	変更あり？
	if ( dlgSub.ShowDialog( pDB, ID_DLGTYPE_ADDRESS, nMode, szTitle, nOrderNum, "", "", "", "", "", "", 0,  nGrSeq ) == ID_DLG_OK ){
		
		try{
			
			//	順序番号振り直し成功？
			if ( mfcRec.ReNumberOrder( nGrSeq, nOrderNum, 1 ) == DB_ERR_OK ){
				
				//	新規行追加成功？
				if ( mfcRec.CreateNewRecord( nGrSeq ) == DB_ERR_OK ){
					
					//	レコードあり？
					if ( !( mfcRec.IsEOF() ) ){
						mfcRec.MoveFirst();						//	レコードを先頭に移動
						mfcRec.Edit();							//	レコード編集開始
						mfcRec.m_OrderNum = nOrderNum;			//	順序番号設定
						mfcRec.m_AdName1 = dlgSub.m_RetName1;	//	名称設定
						mfcRec.m_AdName2 = dlgSub.m_RetName2;	//	名称設定
						mfcRec.m_AdAdd1 = dlgSub.m_RetAdd1;		//	所在地設定
						mfcRec.m_AdAdd2 = dlgSub.m_RetAdd2;		//	所在地設定
						mfcRec.m_AdKana = dlgSub.m_RetKana1;	//  科目ｶﾅ名設定
						mfcRec.m_FgDel = 0;						//	削除フラグ設定
						mfcRec.Update();						//	レコード更新
					}
					// NULLの場合、ここで''（未入力）に更新
					mfcRec.UpdateStrFieldAllForNull();
					//	レコード閉じる
					mfcRec.Fin();
					//	戻値に順序番号を設定
// midori 161109 del -->
					////nRet = nOrderNum;
					//nRet = nIndex + nIdxOffset;					// 現在選択中の行の1つ下を対象とするため
// midori 161109 del <--
// midori 161109 add -->
					nRet = nOrderNum;								// 現在選択中の行の1つ下を対象とするため
// midori 161109 add <--
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
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	科目のレコード挿入（追加）
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
// midori 161109 add -->
//				nSort			…	表示順
// midori 161109 add <--
//				szTitle			…	ダイアログタイトル
//				fAddLine		…	追加フラグ
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入した順序番号
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::InsertKamoku( CDatabase* pDB, short nFormSeq, CString szTitle, BOOL fAddLine )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::InsertKamoku( CDatabase* pDB, short nFormSeq, int nSort, CString szTitle, BOOL fAddLine )
// midori 161109 add <--
{
	long					nRet = 0;			//	戻値初期化
	long					nIndex = 0;			//	リストインデックス
	//CdlgReferenceSub		dlgSub( this );		//	参照サブダイアログクラス
	CdlgReferenceSub_Old	dlgSub( this );		//	参照サブダイアログクラス
	CdbUcLstKamoku			mfcRec( pDB );		//	uc_lst_kamokuテーブルクラス	
	long					nKnOrder = 0;		//	順序番号（数値）
	long					nKnSeq = 0;			//	シーケンス番号
	_RD_KAMOKU_OLD			udKamoku;			//	科目構造体
	int						nInputLen;			//	入力文字数
	EnumIdDlgRefMode		nMode;				//	動作モード
	int						nIdxOffset = 1;		//　フォーカスのオフセット

	//	追加フラグで分岐
	switch( fAddLine){
	//	追加
	case TRUE:
// midori 161109 del -->
//		nIndex = m_lstReference.GetCount() - 1;		//	インデックス算出
// midori 161109 del <--
// midori 161109 add -->
		nIndex = GetListIndex2(pDB, ID_DLGTYPE_KAMOKU, -1, nFormSeq, nSort);		//	インデックス算出
// midori 161109 add <--
		nMode = ID_DLGREF_APPEND;
		nIdxOffset++;
		break;
	//	挿入
	case FALSE:
		nIndex = GetListIndex();					//	賞味のインデックス取得
		nMode = ID_DLGREF_INSERT;
		break;
	}

	//	シーケンス番号取得
	nKnSeq = GetSeq( nIndex );
	//	科目レコード取得
	GetKamokuRecord( pDB, nFormSeq, nKnSeq, &udKamoku );
	//	順序番号を取得
	nKnOrder = udKamoku.m_KnOrder;

	//	追加？
	if ( fAddLine == TRUE ){
		//	順序番号をインクリメント
		nKnOrder++;
	}
	
	switch( nFormSeq ){

	//	⑩-2.源泉所得税預り金の内訳
	case ID_FORMNO_102:
		nInputLen = DR_INPUTLENGTH_GENSEN;
		break;
	//	⑫.土地の売上高等の内訳書
	case ID_FORMNO_121:
		nInputLen = DR_INPUTLENGTH_TOTIURI;
		break;
	//	⑮-1.地代家賃の内訳書
	case ID_FORMNO_151:
		nInputLen = DR_INPUTLENGTH_TIDAI;
		break;
	default:
		// 入力文字数を８文字に設定
		nInputLen = DR_INPUTLENGTH_NORMAL;
		break;
	}	

	//	変更あり？
	if ( dlgSub.ShowDialog( pDB, ID_DLGTYPE_KAMOKU, nMode, szTitle, nKnOrder, "", "", "", "","","",  nInputLen, nFormSeq ) == ID_DLG_OK ){

		try{

			//	順序番号振り直し成功？
			if ( mfcRec.ReNumberOrder( nFormSeq, nKnOrder, 1 ) == DB_ERR_OK ){
				
				//	新規行追加成功？
				if ( mfcRec.CreateNewRecord( nFormSeq ) == DB_ERR_OK ){

					//	レコードあり？
					if ( !( mfcRec.IsEOF() ) ){
						mfcRec.MoveFirst();						//	レコードを先頭に移動
						mfcRec.Edit();							//	レコード編集開始
						mfcRec.m_FormSeq = nFormSeq;			//	様式シーケンス番号設定
						mfcRec.m_KnOrder = nKnOrder;			//	順序番号設定
						mfcRec.m_KnCode = _T("");				//	科目コード設定
						mfcRec.m_KnName = dlgSub.m_RetName1;	//	科目名称設定
						mfcRec.m_FgDel = 0;						//	削除フラグ設定
						mfcRec.m_FgDft = 0;						//	デフォルト値フラグ設定
						mfcRec.m_KnKana = dlgSub.m_RetKana1;	//  科目ｶﾅ名設定
// midori 160610 add -->
						mfcRec.m_PrSign = 0;					//	科目出力サイン
// midori 160610 add <--
						mfcRec.Update();						//	レコード更新
					}
					//	レコード閉じる
					mfcRec.Fin();
// midori 161109 del -->
					////	戻値に順序番号を設定
					////nRet = nKnOrder;
					//nRet = nIndex + nIdxOffset;					// 現在選択中の行の1つ下を対象とするため
// midori 161109 del <--
// midori 161109 add -->
					nRet = nKnOrder;							// 現在選択中の行の1つ下を対象とするため
// midori 161109 add <--
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
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	レコードの削除
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
// midori 161109 add -->
//				nSort			…	表示順
// midori 161109 add <--
//	【戻値】	0				…	編集なし
//				0以外			…	編集あり
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::SetRecordDelete( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::SetRecordDelete( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort )
// midori 161109 add <--
{
	long	nRet = -1;		//	戻値
	long	nTopIndex = 0;	//	トップインデックス

	//	参照型で分岐
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
		//	金融機関のレコード削除
		nRet = DeleteKinyuukikan( pDB );
		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:
		//	取引先のレコード削除
		nRet = DeleteTorihikisaki( pDB, nGrSeq );
		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:
		//	科目のレコード削除
		nRet = DeleteKamoku( pDB, nFormSeq );
		break;
	}
	
	//	削除された？
	if ( nRet >= 0 ){
		//	前回選択行をクリア
		m_nOldRow = -1;
		//	リストのトップインデックスを保持
		nTopIndex = m_lstReference.GetTopIndex();
		//	リストの更新
// midori 161109 del -->
//		GetListData( pDB, nType, nFormSeq, nGrSeq, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 del <--
// midori 161109 add -->
		GetListData( pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 add <--
		//	位置移動
		MovePosition( nTopIndex, ( nRet -1 ) );
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	取引先のレコード削除
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
//	【戻値】	-1				…	削除なし
//				0以上			…	削除した順序番号
//**************************************************
long CdlgReference_Old::DeleteTorihikisaki( CDatabase* pDB, int nGrSeq )
{
	long					nRet = -1;			//	戻値
	long					nIndex = 0;			//	リストインデックス
// midori 153570 add ------------------------------------------------------------------------------------------------------------------->
	int						cnt=0;
// midori 153570 add <-------------------------------------------------------------------------------------------------------------------
	CdbUcLstAddress			mfcRec( pDB );		//	uc_lst_addressテーブルクラス
	long					nSeq = 0;			//	シーケンス番号
	long					nOrderNum = 0;		//	順序番号
	_RD_ADDRESS_OLD			udAddress;			//	取引先構造体

	//	リストのインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号を取得
	nSeq = GetSeq( nIndex );
	
	//	取引先のレコード取得失敗？
	if ( GetTorihikisakiRecord( pDB, nGrSeq, nSeq, &udAddress ) != 0 )
	{
		//	失敗
		return( nRet );
	}	

	//	順序番号取得
	nOrderNum = udAddress.m_OrderNum;

	try{
		//	初期化成功？
		if ( mfcRec.RequeryAdSeq( nGrSeq, nSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				mfcRec.MoveFirst();						//	レコードを先頭に移動
				mfcRec.Delete();						//	削除 				
			}
			//	レコード閉じる
			mfcRec.Fin();
			
// midori 153570 del ------------------------------------------------------------------------------------------------------------------->
//			//	リスト登録数が1件以上？
//			if ( ( m_lstReference.GetCount() - 1 ) > 0 ){
// midori 153570 del <-------------------------------------------------------------------------------------------------------------------
// midori 153570 add ------------------------------------------------------------------------------------------------------------------->
			// データベースの登録件数を取得
			cnt = 0;
			if(mfcRec.Init(nGrSeq) == DB_ERR_OK)	{
				cnt = mfcRec.GetRecordCount();
				mfcRec.Fin();
			}
			if(cnt > 0)	{
// midori 153570 add <-------------------------------------------------------------------------------------------------------------------

				//	順序番号振り直し成功？
				if ( mfcRec.ReNumberOrder( nGrSeq, nOrderNum, -1 ) == DB_ERR_OK ){
					//	レコード閉じる
					mfcRec.Fin();
				}
				
				//	順序番号が最終行？
				if ( nOrderNum == m_lstReference.GetCount() ){
					//	順序番号デクリメント
					nOrderNum--;
				}
			}
			//	戻値を順序番号に設定
			//nRet = nOrderNum;
			nRet = nIndex + 1;
		}
	}
	catch(...){
		
		//	レコード開いている？
		if ( mfcRec.IsOpen() ){
			mfcRec.Fin();	//	レコード閉じる
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	金融機関のレコード削除
//	【引数】	pDB				…	データベースハンドル
//	【戻値】	-1				…	削除なし
//				0以上			…	削除した順序番号
//**************************************************
long CdlgReference_Old::DeleteKinyuukikan( CDatabase* pDB )
{
	long					nRet = -1;			//	戻値
	long					nIndex = 0;			//	リストインデックス
// midori 153570 add ------------------------------------------------------------------------------------------------------------------->
	int						cnt=0;
// midori 153570 add <-------------------------------------------------------------------------------------------------------------------
	CdbUcLstBank			mfcRec( pDB );		//	uc_lst_bankテーブルクラス
	long					nSeq = 0;			//	シーケンス番号
	long					nOrderNum = 0;		//	順序番号
	_RD_BANK_OLD			udBank;				//	金融機関構造体

	//	リストのインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号を取得
	nSeq = GetSeq( nIndex );
	
	//	金融機関のレコード取得失敗？
	if ( GetKinnyuukikanRecord( pDB, nSeq, &udBank ) != 0 )
	{
		//	失敗
		return( nRet );
	}	

	//	順序番号取得
	nOrderNum = udBank.m_OrderNum;

	try{
		//	初期化成功？
		if ( mfcRec.RequerySeq( nSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				mfcRec.MoveFirst();						//	レコードを先頭に移動
				mfcRec.Delete();						//	削除
			}
			//	レコード閉じる
			mfcRec.Fin();
			
// midori 153570 del ------------------------------------------------------------------------------------------------------------------->
//			//	リスト登録数が1件以上？
//			if ( ( m_lstReference.GetCount() - 1 ) > 0 ){
// midori 153570 del <-------------------------------------------------------------------------------------------------------------------
// midori 153570 add ------------------------------------------------------------------------------------------------------------------->
			// データベースの登録件数を取得
			cnt = 0;
			if(mfcRec.Init() == DB_ERR_OK)	{
				cnt = mfcRec.GetRecordCount();
				mfcRec.Fin();
			}
			if(cnt > 0)	{
// midori 153570 add <-------------------------------------------------------------------------------------------------------------------

				//	順序番号振り直し成功？
				if ( mfcRec.ReNumberOrder( nOrderNum, -1 ) == DB_ERR_OK ){
					//	レコード閉じる
					mfcRec.Fin();
				}
				
				//	順序番号が最終行？
				if ( nOrderNum == m_lstReference.GetCount() ){
					//	順序番号デクリメント
					nOrderNum--;
				}
			}
			//	戻値を順序番号に設定
			//nRet = nOrderNum;
			nRet = nIndex + 1;
		}
	}
	catch(...){
		
		//	レコード開いている？
		if ( mfcRec.IsOpen() ){
			mfcRec.Fin();	//	レコード閉じる
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	科目のレコード削除
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	-1				…	削除失敗
//				-2				…	デフォルト科目で削除不可
//				0以上			…	削除した順序番号
//**************************************************
long CdlgReference_Old::DeleteKamoku( CDatabase* pDB, short nFormSeq )
{
	long					nRet = -1;			//	戻値
	long					nIndex = 0;			//	リストインデックス
// midori 153570 add ------------------------------------------------------------------------------------------------------------------->
	int						cnt=0;
// midori 153570 add <-------------------------------------------------------------------------------------------------------------------
	CdbUcLstKamoku			mfcRec( pDB );		//	uc_lst_kamokuテーブルクラス
	CdbUcRenKamoku			mfcZmKmk( pDB );	//	uc_ren_kamokuテーブルクラス
	long					nKnSeq = 0;			//	シーケンス番号
	long					nKnOrder = 0;		//	順序番号
	_RD_KAMOKU_OLD			udKamoku;			//	科目構造体
	BOOL					fDelete = TRUE;		//	削除フラグ
	BOOL					zDelete = TRUE;	//	財務連動登録済フラグ
	//	リストのインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号を取得
	nKnSeq = GetSeq( nIndex );
	
	//	科目のレコード取得失敗？
	if ( GetKamokuRecord( pDB, nFormSeq, nKnSeq, &udKamoku ) != 0 )
	{
		//	失敗
		return( nRet );
	}	

	//	順序番号取得
	nKnOrder = udKamoku.m_KnOrder;

	try{
		//	初期化成功？
		if ( mfcRec.RequeryKnSeq( nFormSeq, nKnSeq ) == DB_ERR_OK ){
			
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				
				mfcRec.MoveFirst();			//	レコードを先頭に移動

				if ( mfcRec.m_FgDft == 0 ){

					// 財務連動登録済みかどうか確認
					if ( mfcZmKmk.RequeryKnSeq( nKnSeq ) == DB_ERR_OK ){
						// レコードあり?
						if ( !( mfcZmKmk.IsEOF() ) ){
							zDelete = FALSE;
						}
						else{
							mfcRec.Delete();		//	削除
						}
						// レコード閉じる
						mfcZmKmk.Fin();
					}

				}
				else{
					//	削除フラグを未削除に設定
					fDelete = FALSE;		
				}
			}
			//	レコード閉じる
			mfcRec.Fin();
			
			//	削除完了？
			if ( fDelete == TRUE ){
				// 財務連動登録済み?(削除は実行されていない?)
				if ( zDelete == TRUE ){
// midori 153570 del ------------------------------------------------------------------------------------------------------------------->
//					//	リスト登録数が1件以上？
//					if ( ( m_lstReference.GetCount() - 1 ) > 0 ){
// midori 153570 del <-------------------------------------------------------------------------------------------------------------------
// midori 153570 add ------------------------------------------------------------------------------------------------------------------->
					// データベースの登録件数を取得
					cnt = 0;
					if(mfcRec.Init(nFormSeq) == DB_ERR_OK)	{
						cnt = mfcRec.GetRecordCount();
						mfcRec.Fin();
					}
					if(cnt > 0)	{
// midori 153570 add <-------------------------------------------------------------------------------------------------------------------

						//	順序番号振り直し成功？
						if ( mfcRec.ReNumberOrder( nFormSeq, nKnOrder, -1 ) == DB_ERR_OK ){
							//	レコード閉じる
							mfcRec.Fin();
						}

						//	順序番号が最終行？
						if ( nKnOrder == m_lstReference.GetCount() ){
							//	順序番号デクリメント
							nKnOrder--;
						}
					}
					//	戻値を順序番号に設定
					//nRet = nKnOrder;
					nRet = nIndex + 1;
				}
				else{
					nRet = -3;
				}
			}
			else{
				nRet = -2;
			}
		}
	}
	catch(...){
		
		//	レコード開いている？
		if ( mfcRec.IsOpen() ){
			mfcRec.Fin();	//	レコード閉じる
		}

	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	レコードの上下移動
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
// midori 161109 add -->
//				nSort			…	表示順
// midori 161109 add <--
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動あり
//**************************************************
// midori 161109 del -->
//long CdlgReference_Old::SetRecordUpDown( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nUpDown )
// midori 161109 del <--
// midori 161109 add -->
long CdlgReference_Old::SetRecordUpDown( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, int nUpDown )
// midori 161109 add <--
{
	long	nRet = 0;		//	戻値
	long	nTopIndex = 0;	//	トップインデックス

	//	参照型で分岐
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
		//	金融機関のレコード上下移動
		nRet = UpDownKinyuukikan( pDB, nUpDown );
		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:
		//	取引先のレコード上下移動
		nRet = UpDownTorihikisaki( pDB, nGrSeq, nUpDown );
		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:
		//	科目のレコード上下移動
		nRet = UpDownKamoku( pDB, nFormSeq, nUpDown );
		break;
	}
	
	//	移動された？
	if ( nRet != 0 ){

		//	リストのトップインデックスを保持
		nTopIndex = m_lstReference.GetTopIndex();
		//	リストの更新
// midori 161109 del -->
		//GetListData( pDB, nType, nFormSeq, nGrSeq, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
		//	m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 del <--
// midori 161109 add -->
		GetListData( pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
			m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 161109 add <--
		//	位置移動
		MovePosition( nTopIndex, ( nRet -1 ) );
	}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	金融機関のレコード上下移動
//	【引数】	pDB				…	データベースハンドル
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動した順序番号
//**************************************************
long CdlgReference_Old::UpDownKinyuukikan( CDatabase* pDB, int nUpDown )
{
	long					nRet = 0;			//	戻値
	long					nIndex[2];			//	リストインデックス
	CdbUcLstBank			mfcRecFrom( pDB );	//	uc_lst_bankテーブルクラス
	CdbUcLstBank			mfcRecTo( pDB );	//	uc_lst_bankテーブルクラス
	long					nSeq[2];			//	シーケンス番号
	long					nOrderNum = 0;		//	順序番号	
	long					nMax;				//	最大行数
	
	//	リストインデックス取得
	nIndex[0] = GetListIndex();
	//	移動先リストインデックス算出
	nIndex[1] = nIndex[0] + nUpDown;
	//	最大行数取得
	nMax = m_lstReference.GetCount() - 1;
	
	//	移動先が範囲外？
	if (( nIndex[1] < 0 ) || ( nIndex[1] > nMax )){
		//	範囲外
		return( nRet );
	}

	//	シーケンス番号を取得
	nSeq[0] = GetSeq( nIndex[0] );
	nSeq[1] = GetSeq( nIndex[1] );
	
	try{
		
		//	指定シーケンス番号のレコード取得成功？（移動元）
		if ( mfcRecFrom.RequerySeq( nSeq[0] ) == DB_ERR_OK ){
			
			//	レコードあり？（移動元）
			if ( !( mfcRecFrom.IsEOF() ) ){
				
				//	指定シーケンス番号のレコード取得成功？（移動先）
				if ( mfcRecTo.RequerySeq( nSeq[1] ) == DB_ERR_OK ){
					
					//	レコードあり？（移動先）
					if ( !( mfcRecTo.IsEOF() ) ){

						//	レコードを先頭に移動
						mfcRecFrom.MoveFirst();							
						mfcRecTo.MoveFirst();
						//	レコード編集開始
						mfcRecFrom.Edit();
						mfcRecTo.Edit();
						//	バッファに移動元レコードを一時保存
						nOrderNum = mfcRecFrom.m_OrderNum;
						//	移動元レコードに移動先レコードの順序番号を設定
						mfcRecFrom.m_OrderNum = mfcRecTo.m_OrderNum;
						//	移動先レコードに一時保存しておいた順序番号を設定
						mfcRecTo.m_OrderNum = nOrderNum;
						//	更新
						mfcRecFrom.Update();
						mfcRecTo.Update();
						//	移動元の順序番号を戻値にセット
						//nRet = mfcRecFrom.m_OrderNum;
						nRet = nIndex[1] + 1;
					}
					//	レコード閉じる（移動先）
					mfcRecTo.Fin();
				}
			}
			//	レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
	}
	catch(...){

		//	レコード開いている？（移動元）
		if ( mfcRecFrom.IsOpen() ){
			//	レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
		//	レコード開いている？（移動先）
		if ( mfcRecTo.IsOpen() ){
			//	レコード閉じる（移動先）
			mfcRecTo.Fin();
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	取引先のレコード上下移動
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動した順序番号
//**************************************************
long CdlgReference_Old::UpDownTorihikisaki( CDatabase* pDB, int nGrSeq, int nUpDown )
{
	long					nRet = 0;			//	戻値
	long					nIndex[2];			//	リストインデックス
	CdbUcLstAddress			mfcRecFrom( pDB );	//	uc_lst_addressテーブルクラス
	CdbUcLstAddress			mfcRecTo( pDB );	//	uc_lst_addressテーブルクラス
	long					nSeq[2];			//	シーケンス番号
	long					nOrderNum = 0;		//	順序番号	
	long					nMax;				//	最大行数
	
	//	リストインデックス取得
	nIndex[0] = GetListIndex();
	//	移動先リストインデックス算出
	nIndex[1] = nIndex[0] + nUpDown;
	//	最大行数取得
	nMax = m_lstReference.GetCount() - 1;
	
	//	移動先が範囲外？
	if (( nIndex[1] < 0 ) || ( nIndex[1] > nMax )){
		//	範囲外
		return( nRet );
	}

	//	シーケンス番号を取得
	nSeq[0] = GetSeq( nIndex[0] );
	nSeq[1] = GetSeq( nIndex[1] );
	
	try{
		
		//	指定シーケンス番号のレコード取得成功？（移動元）
		if ( mfcRecFrom.RequeryAdSeq( nGrSeq, nSeq[0] ) == DB_ERR_OK ){
			
			//	レコードあり？（移動元）
			if ( !( mfcRecFrom.IsEOF() ) ){
				
				//	指定シーケンス番号のレコード取得成功？（移動先）
				if ( mfcRecTo.RequeryAdSeq( nGrSeq, nSeq[1] ) == DB_ERR_OK ){
					
					//	レコードあり？（移動先）
					if ( !( mfcRecTo.IsEOF() ) ){

						//	レコードを先頭に移動
						mfcRecFrom.MoveFirst();							
						mfcRecTo.MoveFirst();
						//	レコード編集開始
						mfcRecFrom.Edit();
						mfcRecTo.Edit();
						//	バッファに移動元レコードを一時保存
						nOrderNum = mfcRecFrom.m_OrderNum;
						//	移動元レコードに移動先レコードの順序番号を設定
						mfcRecFrom.m_OrderNum = mfcRecTo.m_OrderNum;
						//	移動先レコードに一時保存しておいた順序番号を設定
						mfcRecTo.m_OrderNum = nOrderNum;
						//	更新
						mfcRecFrom.Update();
						mfcRecTo.Update();
						//	移動元の順序番号を戻値にセット
						//nRet = mfcRecFrom.m_OrderNum;
						nRet = nIndex[1] + 1;
					}
					//	レコード閉じる（移動先）
					mfcRecTo.Fin();
				}
			}
			//	レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
	}
	catch(...){

		//	レコード開いている？（移動元）
		if ( mfcRecFrom.IsOpen() ){
			//	レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
		//	レコード開いている？（移動先）
		if ( mfcRecTo.IsOpen() ){
			//	レコード閉じる（移動先）
			mfcRecTo.Fin();
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	科目のレコード上下移動
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動した順序番号
//**************************************************
long CdlgReference_Old::UpDownKamoku( CDatabase* pDB, short nFormSeq, int nUpDown )
{
	long					nRet = 0;			//	戻値
	long					nIndex[2];			//	リストインデックス
	CdbUcLstKamoku			mfcRecFrom( pDB );	//	uc_lst_kamokuテーブルクラス
	CdbUcLstKamoku			mfcRecTo( pDB );	//	uc_lst_kamokuテーブルクラス
	long					nKnSeq[2];			//	シーケンス番号
	long					nKnOrder = 0;		//	順序番号	
	long					nMax;				//	最大行数
	
	//	リストインデックス取得
	nIndex[0] = GetListIndex();
	//	移動先リストインデックス算出
 	nIndex[1] = nIndex[0] + nUpDown;
	//	最大行数取得
	nMax = m_lstReference.GetCount() - 1;
	
	//	移動先が範囲外？
	if (( nIndex[1] < 0 ) || ( nIndex[1] > nMax )){
		//	範囲外
		return( nRet );
	}

	//	シーケンス番号を取得
	nKnSeq[0] = GetSeq( nIndex[0] );
	nKnSeq[1] = GetSeq( nIndex[1] );
	
	try{
		
		//	指定シーケンス番号のレコード取得成功？（移動元）
		if ( mfcRecFrom.RequeryKnSeq( nFormSeq, nKnSeq[0] ) == DB_ERR_OK ){
			
			//	レコードあり？（移動元）
			if ( !( mfcRecFrom.IsEOF() ) ){
				
				//	指定シーケンス番号のレコード取得成功？（移動先）
				if ( mfcRecTo.RequeryKnSeq( nFormSeq, nKnSeq[1] ) == DB_ERR_OK ){
					
					//	レコードあり？（移動先）
					if ( !( mfcRecTo.IsEOF() ) ){

						//	レコードを先頭に移動
						mfcRecFrom.MoveFirst();							
						mfcRecTo.MoveFirst();
						//	レコード編集開始
						mfcRecFrom.Edit();
						mfcRecTo.Edit();
						//	バッファに移動元レコードを一時保存
						nKnOrder = mfcRecFrom.m_KnOrder;
						//	移動元レコードに移動先レコードの順序番号を設定
						mfcRecFrom.m_KnOrder = mfcRecTo.m_KnOrder;
						//	移動先レコードに一時保存しておいた順序番号を設定
						mfcRecTo.m_KnOrder = nKnOrder;
						//	更新
						mfcRecFrom.Update();
						mfcRecTo.Update();
						//	移動元の順序番号を戻値にセット
						//nRet = mfcRecFrom.m_KnOrder;
						nRet = nIndex[1] + 1;
					}
					//	レコード閉じる（移動先）
					mfcRecTo.Fin();
				}
			}
			//	レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
	}
	catch(...){

		//	レコード開いている？（移動元）
		if ( mfcRecFrom.IsOpen() ){
			//	レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
		//	レコード開いている？（移動先）
		if ( mfcRecTo.IsOpen() ){
			//	レコード閉じる（移動先）
			mfcRecTo.Fin();
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	選択されたリストのレコード取得
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
//	【戻値】	0				…	成功
//				0以外			…	失敗
//**************************************************
int CdlgReference_Old::GetSelectRecord( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq )
{
	int		nRet = 1;		//	戻値
	int		nIndex = 0;		//	リストインデックス
	long	nSeq = 0;		//	シーケンス番号
	
	//	リストインデックス取得
	nIndex = GetListIndex();
	//	シーケンス番号取得
	nSeq = GetSeq( nIndex );

	//	参照型で分岐
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
		//	金融機関のレコード取得
		nRet = GetKinnyuukikanRecord( pDB, nSeq, &m_udBank );
		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:
		//	取引先のレコード取得
		nRet = GetTorihikisakiRecord( pDB, nGrSeq, nSeq, &m_udAddress );
		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:
		//	科目のレコード取得
		nRet = GetKamokuRecord( pDB, nFormSeq, nSeq, &m_udKamoku );
		break;
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	賞味のリストインデックス取得
//	【引数】	なし
//	【戻値】	賞味のインデックス
//**************************************************
long CdlgReference_Old::GetListIndex()
{
	long	nRet = 0;		//	戻値
	long	nIndex = 0;		//	頁内でのインデックス
	long	nTop = 0;		//	頁のトップインデックス
	
	//	頁のトップインデックス取得
	nTop = m_lstReference.GetTopIndex();
	//	頁内でのインデックス取得
	nIndex = m_lstReference.GetIndex();
	//	賞味のインデックス取得
	nRet = nTop + nIndex;

	//	戻値を返す
	return( nRet );
}

// midori 161109 add -->
//**************************************************
//	最大番号を持っているリストインデックスを取得
//	【引数】	pDB			…	データベースハンドル
//				pType		…	参照型
//				pGrSeq		…	グループ番号
//				pFormSeq	…	様式シーケンス番号
//				pSort		…	表示順
//	【戻値】	賞味のインデックス
//**************************************************
long CdlgReference_Old::GetListIndex2( CDatabase* pDB, int pType, int pGrSeq, short pFormSeq, int pSort )
{
	int					cnt=0,max=0;
	long				nRet=0;				//	戻値
	long				nOrderNum=0;		//	順序番号
	long				nOrderNumSv=0;		//	順序番号セーブ
	long				nSeq = 0;			//	シーケンス番号
	_RD_BANK_OLD		udBank;				//	金融機関構造体
	_RD_ADDRESS_OLD		udAddress;			//	取引先構造体
	_RD_KAMOKU_OLD		udKamoku;			//	科目構造体

	if(pSort == 0)	{
		nRet = m_lstReference.GetCount() - 1;
	}
	else	{
		max = (int)m_lstReference.GetCount();

		nOrderNumSv = -1;
		for(cnt=0; cnt<max; cnt++)	{
			//	シーケンス番号取得
			nSeq = GetSeq( cnt );
			if(pType == ID_DLGTYPE_BANK)	{
				//	金融機関レコード取得
				GetKinnyuukikanRecord( pDB, nSeq, &udBank );
				//	順序番号を取得
				nOrderNum = udBank.m_OrderNum;
			}
			else if(pType == ID_DLGTYPE_ADDRESS)	{
				//	取引先のレコード取得
				GetTorihikisakiRecord( pDB, pGrSeq, nSeq, &udAddress );
				nOrderNum = udAddress.m_OrderNum;
			}
			else if(pType == ID_DLGTYPE_KAMOKU)		{
				//	科目のレコード取得
				GetKamokuRecord( pDB, pFormSeq, nSeq, &udKamoku );
				nOrderNum = udKamoku.m_KnOrder;
			}
			
			if(nOrderNumSv < nOrderNum)	{
				nRet = cnt;
				nOrderNumSv = nOrderNum;
			}
		}
	}

	//	戻値を返す
	return( nRet );
}
// midori 161109 add <--

//**************************************************
//	位置移動
//	【引数】	nTop	…	トップインデックス（※4/28未使用）
//				nIndex	…	賞味のインデックス
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::MovePosition( long nTop, long nIndex )
{
	//	インデックスが範囲外？
	if (( nIndex < 0 ) || ( nIndex > ( m_lstReference.GetCount() - 1 ) )){
		//	先頭にする
		nIndex = 0;
	}

	//	カーソル移動
	m_lstReference.SetInputPositionEX( ( short )nIndex, 0 );
	//	現在行確認
	CheckNowRowPosition();
}

// midori 161109 add -->
//**************************************************
//	位置移動（Order番号から検索）
//	【引数】	nOrder	…	順序番号
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::MovePosition2( int nOrder )
{
	int			ii=0;			// ループカウンタ
	int			max=0;			// 最大行数
	int			seq=0;			// 順序番号
	int			row=0;			// カーソルセットポジション
	LINP_DATA	udData;			//	リストデータ構造体
	
	max = (int)m_lstReference.GetCount();

	row = 0;
	for(ii=0;ii<max;ii++)	{
		//	初期化
		InitLinpData( &udData );
		//	指定カラムのデータを取得
		m_lstReference.GetColumnData( ii, 1, ( LPUNKNOWN )&udData );
		//	シーケンス番号を数値に変換
		seq = atoi( udData.dat_string );
		if(seq == nOrder)	{
			row = ii;
		}
	}
	//	カーソル移動
	m_lstReference.SetInputPositionEX( ( short )row, 0 );

	//	現在行確認
	CheckNowRowPosition();
}
// midori 161109 add <--

//**************************************************
//	金融機関のレコード取得
//	【引数】	pDB			…	データベースハンドル
//				nSeq		…	シーケンス番号
//				pudBank		…	金融機関構造体
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgReference_Old::GetKinnyuukikanRecord( CDatabase* pDB, long nSeq, _RD_BANK_OLD* pudBank )
{
	int				nRet = 1;		//	戻値
	CdbUcLstBank	mfcRec( pDB );	//	uc_lst_bankテーブルクラス
	
	//	初期化
	pudBank->m_Seq = 0;
	pudBank->m_OrderNum = 0;
	pudBank->m_BkName1.Empty();
	pudBank->m_BkName2.Empty();
	pudBank->m_FgDel = 0;
	pudBank->m_BkKana1.Empty();
	pudBank->m_BkKana2.Empty();

	try{

		//	指定シーケンス番号のレコード取得成功？
		if ( mfcRec.RequerySeq( nSeq ) == DB_ERR_OK ){

			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();							//	レコードを先頭に移動
				pudBank->m_Seq = mfcRec.m_Seq;				//	シーケンス番号取得
				pudBank->m_OrderNum = mfcRec.m_OrderNum;	//	順序番号取得
				pudBank->m_BkName1 = mfcRec.m_BkName1;		//	金融機関名取得
				pudBank->m_BkName2 = mfcRec.m_BkName2;		//	支店名取得
				pudBank->m_FgDel = mfcRec.m_FgDel;			//	削除フラグ取得
				pudBank->m_BkKana1 = mfcRec.m_BkKana1;		//  金融機関ｶﾅ取得
				pudBank->m_BkKana2 = mfcRec.m_BkKana2;		//  支店ｶﾅ取得
				nRet = 0;									//	戻値を成功に設定
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
//	取引先のレコード取得
//	【引数】	pDB			…	データベースハンドル
//				nGrSeq		…	グループ番号
//				nAdSeq		…	シーケンス番号
//				pudAddress	…	取引先構造体
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgReference_Old::GetTorihikisakiRecord( CDatabase* pDB, int nGrSeq, long nAdSeq, _RD_ADDRESS_OLD* pudAddress )
{
	int					nRet = 1;		//	戻値
	CdbUcLstAddress		mfcRec( pDB );	//	uc_lst_addressテーブルクラス
	
	//	初期化
	pudAddress->m_AdSeq = 0;
	pudAddress->m_GrSeq = 0;
	pudAddress->m_OrderNum = 0;
	pudAddress->m_AdName1.Empty();
	pudAddress->m_AdName2.Empty();
	pudAddress->m_AdAdd1.Empty();
	pudAddress->m_AdAdd2.Empty();
	pudAddress->m_FgDel = 0;
	pudAddress->m_AdKana.Empty();

	try{

		//	指定シーケンス番号のレコード取得成功？
		if ( mfcRec.RequeryAdSeq( nGrSeq, nAdSeq ) == DB_ERR_OK ){

			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();							//	レコードを先頭に移動
				pudAddress->m_AdSeq = mfcRec.m_AdSeq;			//	シーケンス番号取得
				pudAddress->m_GrSeq = mfcRec.m_GrSeq;		//	グループ番号取得
				pudAddress->m_OrderNum = mfcRec.m_OrderNum;	//	順序番号取得
				pudAddress->m_AdName1 = mfcRec.m_AdName1;	//	名称取得
				pudAddress->m_AdName2 = mfcRec.m_AdName2;	//	名称取得
				pudAddress->m_AdAdd1 = mfcRec.m_AdAdd1;		//	所在地取得
				pudAddress->m_AdAdd2 = mfcRec.m_AdAdd2;		//	所在地取得
				pudAddress->m_FgDel = mfcRec.m_FgDel;		//	削除フラグ取得
				pudAddress->m_AdKana = mfcRec.m_AdKana;		//	ｶﾅ取得
				nRet = 0;									//	戻値を成功に設定
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
//	科目のレコード取得
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nKnSeq		…	シーケンス番号
//				pudBank		…	金融機関構造体
//	【戻値】	0			…	成功
//				0以外		…	失敗
//**************************************************
int CdlgReference_Old::GetKamokuRecord( CDatabase* pDB, short nFormSeq, long nKnSeq, _RD_KAMOKU_OLD* pudKamoku )
{
	int				nRet = 1;		//	戻値
	CdbUcLstKamoku	mfcRec( pDB );	//	uc_lst_bankテーブルクラス
	
	//	初期化
	pudKamoku->m_FormSeq = 0;
	pudKamoku->m_KnSeq = 0;
	pudKamoku->m_KnOrder = 0;
	pudKamoku->m_KnCode.Empty();
	pudKamoku->m_KnName.Empty();
	pudKamoku->m_FgDel = 0;
	pudKamoku->m_FgDft = 0;
	pudKamoku->m_KnKana.Empty();

	try{

		//	指定シーケンス番号のレコード取得成功？
		if ( mfcRec.RequeryKnSeq( nFormSeq, nKnSeq ) == DB_ERR_OK ){

			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){

				mfcRec.MoveFirst();							//	レコードを先頭に移動
				pudKamoku->m_FormSeq = mfcRec.m_FormSeq;	//	シーケンス番号取得
				pudKamoku->m_KnSeq	= mfcRec.m_KnSeq;		//	シーケンス番号取得
				pudKamoku->m_KnOrder = mfcRec.m_KnOrder;	//	順序番号取得
				pudKamoku->m_KnCode	= mfcRec.m_KnCode;		//	金融機関名取得
				pudKamoku->m_KnName	= mfcRec.m_KnName;		//	支店名取得
				pudKamoku->m_FgDel	= mfcRec.m_FgDel;		//	削除フラグ取得
				pudKamoku->m_FgDft	= mfcRec.m_FgDft;		//	削除フラグ取得
				pudKamoku->m_KnKana = mfcRec.m_KnKana;		//  ｶﾅ取得
				nRet = 0;									//	戻値を成功に設定
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
//	リストからシーケンス番号取得
//	【引数】	nIndex	…	リストインデックス
//	【戻値】	0以外	…	シーケンス番号
//				0		…	失敗
//**************************************************
long CdlgReference_Old::GetSeq( int nIndex )
{
	
	long		nRet = 0;			//	戻値
	LINP_DATA	udData;				//	リストデータ構造体
	
	//	初期化
	InitLinpData( &udData );
	
	//	指定カラムのデータを取得
	m_lstReference.GetColumnData( nIndex, 0, ( LPUNKNOWN )&udData );
	//	シーケンス番号を数値に変換
	nRet = atoi( udData.dat_string );
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	初期位置移動
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nType			…	参照型
// midori 161109 add -->
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
// midori 161109 add <--
//				nItemSeq		…	帳表で選択されている項目のシーケンス番号
//	【戻値】	なし
//**************************************************
// midori 161109 del -->
//void CdlgReference_Old::InitMovePosition( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, long nItemSeq )
// midori 161109 del <--
// midori 161109 add -->
void CdlgReference_Old::InitMovePosition( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, long nItemSeq )
// midori 161109 add <--
{
	_RD_BANK_OLD	udBank;		//	金融機関	
	_RD_ADDRESS_OLD	udAddress;	//	取引先
	_RD_KAMOKU_OLD	udKamoku;	//	科目
	long			nPos = 0;	//	位置
// midori 161109 add -->
	int				cnt=0,max=0;
	LINP_DATA		udData;				//	リストデータ構造体
// midori 161109 add <--

	//	参照型で分岐
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
		//	金融機関のレコード取得
		if ( GetKinnyuukikanRecord( pDB, nItemSeq, &udBank ) == 0 ){
			//	順序番号取得
			nPos = udBank.m_OrderNum;
		}

		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:

		//	取引先のレコード取得
		if ( GetTorihikisakiRecord( pDB, nGrSeq, nItemSeq, &udAddress ) == 0 ){
			//	順序番号取得
			nPos = udAddress.m_OrderNum;
		}
		break;

		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:

		//	科目のレコード取得
		if ( GetKamokuRecord( pDB, nFormSeq, nItemSeq, &udKamoku ) == 0 ){
			//	順序番号取得
			nPos = udKamoku.m_KnOrder;
		}
		break;
	}
	
// midori 161109 add -->
	// 50音順の時はインデックス番号を再取得
	if(nSort == 1)	{
		max = (int)m_lstReference.GetCount();
		for(cnt=0; cnt<max; cnt++)	{
			m_lstReference.GetColumnData( cnt, 1, ( LPUNKNOWN )&udData );
			if(nPos == _tstoi(udData.dat_string))	{
				nPos = cnt+1;
				break;
			}
		}
	}
// midori 161109 add <--

	//	位置移動
	MovePosition( 0, nPos - 1 );
}

//**************************************************
//	LINP_DATA構造体初期化
//	【引数】	pData	…	LINP_DATA構造体ポインタ
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::InitLinpData( LINP_DATA* pData )
{
	pData->dat_attr = 0;
	pData->dat_bc = 0;
	ZeroMemory( pData->dat_bmn, sizeof( pData->dat_bmn ) );
	ZeroMemory( pData->dat_day, sizeof( pData->dat_day ) );
	pData->dat_dpn = 0;
	ZeroMemory( pData->dat_eda, sizeof( pData->dat_eda ) );
	pData->dat_fc = 0;
	pData->dat_initflg = 0;
	pData->dat_kamoku.kamoku_code = 0;  
	pData->dat_kamoku.kamoku_edaban = 0;
	ZeroMemory( pData->dat_kamoku.kamoku_name, sizeof( pData->dat_kamoku.kamoku_name ) );
	pData->dat_kamoku.kamoku_num = 0;
	ZeroMemory( pData->dat_numeric, sizeof( pData->dat_numeric ) );
	pData->dat_sel.sel_data = 0;
	pData->dat_sel.sel_num = 0;
	pData->dat_sel.sel_rslt = 0;
	pData->dat_seq = 0;
	pData->dat_string.Empty();
	ZeroMemory( pData->dat_tek.tek_data, sizeof( pData->dat_tek.tek_data ) );
	pData->dat_tek.tek_height = 0;
	pData->dat_tek.tek_image = NULL;
	pData->dat_tek.tek_width = 0;
	ZeroMemory( pData->dat_val, sizeof( pData->dat_val ) );
}

//**************************************************
//	現在行確認
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::CheckNowRowPosition()
{
	long	nIndex;		//	インデックス
	long	nMax;		//	リスト最大登録数
	BOOL	bFlag[2];	//	フラグ

	//	賞味のインデックス取得
	nIndex = GetListIndex();
	//	リスト最大登録数
	nMax = m_lstReference.GetCount();
	
	//	フラグ初期化
	bFlag[0] = TRUE;
	bFlag[1] = TRUE;
	
	//	リスト最大登録数が2行以下？
	if ( nMax < 2 ){
		bFlag[0] = FALSE;
		bFlag[1] = FALSE;
	}
	//	インデックスが先頭？
	else if ( nIndex == 0 ){
		bFlag[0] = FALSE;
	}
	//	インデックスが最終行？
	else if ( nIndex == ( nMax - 1 ) ){
		bFlag[1] = FALSE;
	}

// midori 152745 del -->
//// midori 161109 add -->
//	// 表示順が50音の場合は常にFALSE
//	if(m_nSort == 1)	{
//		bFlag[0] = FALSE;
//		bFlag[1] = FALSE;
//	}
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
	// 表示順が50音の場合は常にFALSE
	if(m_nSort[m_nSortNum] == 1)	{
		bFlag[0] = FALSE;
		bFlag[1] = FALSE;
	}
// midori 152745 add <--

	//	[F5][F6]キーの有効／無効を設定
	m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F11, bFlag[0] );		//	[F11]一行上へ
	m_tbrToolBar.GetToolBarCtrl().EnableButton( ID_TOOL_BUTTON_F12, bFlag[1] );		//	[F12]一行下へ
	
	//	行の背景色を更新
// midori M-16113003 del -->
//	ReDrawRowColor();
// midori M-16113003 del <--
// midori M-16113003 add -->
	ReDrawRowColor(0);
// midori M-16113003 add <--
	//	前回選択行取得
	m_nOldRow = nIndex;
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				nFont	…	文字色
//				nBack	…	背景色
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::SetColor( int nIndex, COLORREF nFont, COLORREF nBack )
{
	LINP_DATA	udData[32];		//	リストデータ構造体
	int			nCnt;			//	カウント用
	
	//	リストデータ構造体初期化
	for( nCnt = 0; nCnt < 32; nCnt++ ){
		InitLinpData( &udData[nCnt] );
	}

	//	現在選択されている行のデータを取得
	m_lstReference.GetData( nIndex, ( LPUNKNOWN )udData );
	
	//	カラム最大数分ループ
	for( nCnt = 0; nCnt < m_nColumnMax; nCnt++ ){ 
		udData[nCnt].dat_bc = nBack;	//	背景色を変更
		udData[nCnt].dat_fc = nFont;	//	文字色を変更
	}
	
	//	現在選択されている行のデータを設定
	m_lstReference.SetData( nIndex, ( LPUNKNOWN )udData, TRUE );
}

// midori M-16113003 del -->
////**************************************************
////	行の背景色を更新
////	【引数】	なし
////	【戻値】	なし
////**************************************************
//void CdlgReference_Old::ReDrawRowColor()
// midori M-16113003 del <--
// midori M-16113003 add -->
//**************************************************
//	行の背景色を更新
//	【引数】	int pSw		1:コンボボックスでEnter押下時
//	【戻値】	なし
//**************************************************
void CdlgReference_Old::ReDrawRowColor(int pSw)
// midori M-16113003 add <--
{
	long	nIndex;		//	インデックス
	long	nMax;		//	リスト最大登録数
	int		nFont;
	int		nBack;
	
	//	賞味のインデックス取得
	nIndex = GetListIndex();
	//	リスト最大登録数
	nMax = m_lstReference.GetCount();
	
	//	前回選択行と現在選択行が違う？
// midori M-16113003 del -->
//	if ( nIndex != m_nOldRow ){
// midori M-16113003 del <--
// midori M-16113003 add -->
	if ( nIndex != m_nOldRow || pSw == 1){
// midori M-16113003 add <--
		
		//	前回選択行あり？
		if ( m_nOldRow >= 0 ){
			
			//	科目で且つデフォルト項目？
			if (( m_nType == ID_DLGTYPE_KAMOKU ) && ( GetFgDft( m_nOldRow ) == 1 )){
				nFont = PALETTERGB( 0x00, 0x00, 0x00 );
				nBack = PALETTERGB( 0xA6, 0XCA, 0xF0 );
			}
			//	以外
			else{
				nFont = GetSysColor( COLOR_WINDOWTEXT );
				nBack = GetSysColor( COLOR_WINDOW );
			}
			
			//	前回選択行が範囲内？
			if (( m_nOldRow >= 0 ) && ( m_nOldRow < nMax )){
				//	前回選択行をクリア
				SetColor( m_nOldRow, nFont, nBack );
			}
		}
		
		//	現在選択行が範囲内？
		if (( nIndex >= 0 ) && ( nIndex < nMax )){
			//	現在選択行を選択色に変更
			SetColor( nIndex, GetSysColor( COLOR_WINDOWTEXT ), RGB( 127, 127, 127 ) );
		}
	}
}

//**************************************************
//	リストからデフォルトフラグ取得
//	【引数】	nIndex	…	リストインデックス
//	【戻値】	0以外	…	デフォルトフラグ
//				0		…	失敗
//**************************************************
long CdlgReference_Old::GetFgDft( int nIndex )
{
	
	long		nRet = 0;			//	戻値
	LINP_DATA	udData;				//	リストデータ構造体

	//	初期化
	InitLinpData( &udData );

	//	指定カラムのデータを取得
// midori 161109 del -->
//	m_lstReference.GetColumnData( nIndex, 3, ( LPUNKNOWN )&udData );
// midori 161109 del <--
// midori 161109 add -->
	m_lstReference.GetColumnData( nIndex, 4, ( LPUNKNOWN )&udData );
// midori 161109 add <--
	//	シーケンス番号を数値に変換
	nRet = atoi( udData.dat_string );
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	帳表で選択されている項目のシーケンス番号取得
//	（※参照のみ＝自動登録しない場合でも選択項目をインバース表示するため追加）
//	【引数】	なし
//	【戻値】	帳表で選択されている項目のシーケンス番号
//**************************************************
int CdlgReference_Old::GetItemSeq()
{
	int				nRet = 0;			//	戻値

	//	参照型で分岐
	switch( m_nType ){
	//	金融機関（参照のみ＝自動登録しない）
	case ID_DLGTYPE_BANK_REFERENCE:

		//	金融機関のシーケンス番号検索
		nRet = SearchBankItemSeq();
		//	参照型を通常に戻す
		m_nType = ID_DLGTYPE_BANK;
		break;

	//	取引先（参照のみ＝自動登録しない）
	case ID_DLGTYPE_ADDRESS_REFERENCE:
		
		//	取引先のシーケンス番号検索
		nRet = SearchAddressItemSeq();
		//	参照型を通常に戻す
		m_nType = ID_DLGTYPE_ADDRESS;
		break;

	//	上記以外
	default:
		nRet = m_nItemSeq;
		break;
	}
	
	//	戻値を返す
	return( nRet ); 
}

//**************************************************
//	金融機関のシーケンス番号検索
//	【引数】	なし
//	【戻値】	シーケンス番号
//**************************************************
int CdlgReference_Old::SearchBankItemSeq()
{
	int				nRet = 0;			//	戻値
	CdbUcLstBank	mfcRec( m_pDB );	//	金融機関名称テーブル

	//	重複確認
	if ( mfcRec.CheckRepeat( m_udReference.szName1, m_udReference.szName2 ) == DB_ERR_OK ){
		
		//	重複あり？
		if ( !mfcRec.IsEOF() ){
			//	シーケンス番号取得
			nRet = mfcRec.m_Seq; 
		}
	}
	
	//	閉じる
	mfcRec.Fin();

	//	戻値を返す
	return( nRet ); 
}

//**************************************************
//	取引先のシーケンス番号検索
//	【引数】	なし
//	【戻値】	シーケンス番号
//**************************************************
int CdlgReference_Old::SearchAddressItemSeq()
{
	int				nRet = 0;			//	戻値
	CdbUcLstAddress	mfcRec( m_pDB );	//	取引先名称テーブル

	//	重複確認
	if ( mfcRec.CheckRepeat( m_udReference.szName1, m_udReference.szName2, m_udReference.szAdd1, m_udReference.szAdd2, m_nGrSeq ) == DB_ERR_OK ){
		
		//	重複あり？
		if ( !mfcRec.IsEOF() ){
			//	シーケンス番号取得
			nRet = mfcRec.m_AdSeq; 
		}
	}
	
	//	閉じる
	mfcRec.Fin();

	//	戻値を返す
	return( nRet ); 
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgReference_Old::GetFocusControlID( CWnd* pParent /*=NULL*/)
{
	int		nRet = 0;			//	戻値
	CWnd*	pControl = NULL;	//	コントロール
	char	lpClassName[128];
	
	//	親ハンドルあり？
	if ( pParent != NULL ){

		//	フォーカスのあるコントロールを取得
		pControl = pParent->GetFocus();
		
		//	コントロール取得成功？
		if ( pControl != NULL ){
			// フォーカスを持っているウィンドウがICSDiagの場合、ICSDiag自体のIDではなくICSDiag::EditのIDを取得してしまうので、
			// Editがフォーカスを持っているときはICSDiagがフォーカスを持っていると判定する
			GetClassName(GetFocus()->GetSafeHwnd(), lpClassName, sizeof(lpClassName));
			if( strcmp( lpClassName, _T("Edit")) == 0 ){
				//	コントロールID取得
				nRet = m_DiagKanaSearch.GetDlgCtrlID();
			}
			else{
				//	コントロールID取得
				nRet = pControl->GetDlgCtrlID();
			}
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	ダイアログ表示
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
//				nItemSeq		…	帳表で選択されている項目のシーケンス番号
//				szName1			…	名称１（※銀行名）
//				szName2			…	名称２（※支店名）
//				szAdd1			…	所在地１
//				szAdd1			…	所在地２
//	【戻値】	ID_DLG_OK		…	指定終了押した
//				ID_DLG_CANCEL	…	キャンセル押した
//**************************************************
// midori 152135,152136 del -->
//// midori 161106 del -->
////int CdlgReference_Old::ShowDialog( BOOL& isRegAuto, CDatabase* pDB /*=NULL*/, EnumIdDlgType nType /*=ID_DLGTYPE_BANK*/,
////							   short nFormSeq /*=ID_FORMNO_011*/, int nGrSeq /*=0*/,
////							   long nItemSeq /*=0*/, REFERENCE_INFO* pudReference /*=0*/)
//// midori 161106 del <--
//// midori 161106 add -->
//int CdlgReference_Old::ShowDialog( BOOL& isRegAuto, CDatabase* pDB /*=NULL*/, EnumIdDlgType nType /*=ID_DLGTYPE_BANK*/,
//							   short nFormSeq /*=ID_FORMNO_011*/, int nGrSeq /*=0*/,
//							   long nItemSeq /*=0*/, REFERENCE_INFO* pudReference /*=0*/, BOOL pF9OnSw /*=FALSE*/)
//// midori 161106 add <--
// midori 152135,152136 del <--
// midori 152745 del -->
//// midori 152135,152136 add -->
//int CdlgReference_Old::ShowDialog( BOOL& isRegAuto, int& pSort, CDatabase* pDB /*=NULL*/, EnumIdDlgType nType /*=ID_DLGTYPE_BANK*/,
//							   short nFormSeq /*=ID_FORMNO_011*/, int nGrSeq /*=0*/,
//							   long nItemSeq /*=0*/, REFERENCE_INFO* pudReference /*=0*/, BOOL pF9OnSw /*=FALSE*/)
//// midori 152135,152136 add <--
// midori 152745 del <--
// midori 152745 add -->
int CdlgReference_Old::ShowDialog( BOOL& isRegAuto, int pSort[], CDatabase* pDB /*=NULL*/, EnumIdDlgType nType /*=ID_DLGTYPE_BANK*/,
							   short nFormSeq /*=ID_FORMNO_011*/, int nGrSeq /*=0*/,
							   long nItemSeq /*=0*/, REFERENCE_INFO* pudReference /*=0*/, BOOL pF9OnSw /*=FALSE*/)
// midori 152745 add <--
{
	m_nRet = ID_DLG_CANCEL;		//	戻値初期化
	m_fUpdate = FALSE;			//	更新フラグ初期化
	m_nOldRow = -1;				//	前回選択行初期化

	//	初期化
	m_udReference.szName1.Empty();
	m_udReference.szName2.Empty();
	m_udReference.szAdd1.Empty();
	m_udReference.szAdd2.Empty();
	m_isRegAuto = FALSE;
// midori 152745 del -->
//// midori 152135,152136 add -->
//	m_nSort = 0;
//// midori 152135,152136 add <--
// midori 152745 del <--
// midori 152745 add -->
	m_nSort[0] = 0;
	m_nSort[1] = 0;
	m_nSortNum = 0;
// midori 152745 add <--
	
	//	参照型で分岐
	switch( nType ){
	//	金融機関
	case ID_DLGTYPE_BANK:
	case ID_DLGTYPE_BANK_REFERENCE:
		//	初期化
		m_udBank.m_Seq = 0;				//	シーケンス番号
		m_udBank.m_OrderNum = 0;		//	順序番号
		m_udBank.m_BkName1.Empty();		//	金融機関名
		m_udBank.m_BkName2.Empty();		//	支店名
		m_udBank.m_FgDel = 0;			//	削除フラグ
		break;
	//	取引先
	case ID_DLGTYPE_ADDRESS:
	case ID_DLGTYPE_ADDRESS_REFERENCE:
		//	初期化
		m_udAddress.m_AdSeq = 0;		//	シーケンス番号
		m_udAddress.m_GrSeq = 0;		//	グループ番号
		m_udAddress.m_OrderNum = 0;		//	順序番号
		m_udAddress.m_AdName1.Empty();	//	取引先名1
		m_udAddress.m_AdName2.Empty();	//	取引先名2
		m_udAddress.m_AdAdd1.Empty();	//	住所1
		m_udAddress.m_AdAdd2.Empty();	//	住所2
		m_udAddress.m_FgDel = 0;		//	削除フラグ
		break;
	//	科目
	case ID_DLGTYPE_KAMOKU:
		//	初期化
		m_udKamoku.m_FormSeq = 0;		//	様式シーケンス番号
		m_udKamoku.m_KnSeq = 0;			//	科目シーケンス番号
		m_udKamoku.m_KnOrder = 0;		//	順序番号
		m_udKamoku.m_KnCode.Empty();	//	科目コード
		m_udKamoku.m_KnName.Empty();	//	科目名
		m_udKamoku.m_FgDel = 0;			//	削除フラグ
		m_udKamoku.m_FgDft = 0;			//	デフォルトフラグ
		break;
	}

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;				//	データベースハンドル取得
				m_nType = nType;			//	参照型取得
				m_nFormSeq = nFormSeq;		//	様式シーケンス番号取得
				m_nItemSeq = nItemSeq;		//	帳表で選択されている項目のシーケンス番号
				m_nGrSeq = nGrSeq;			//	グループ番号取得
				m_isRegAuto = isRegAuto;	//  「取引先、金融機関を自動で登録する」
// midori 161106 add -->
				m_F9OnSw = pF9OnSw;			//	ｺﾝﾎﾞﾎﾞｯｸｽで「新規作成」が選択されて呼び出された
// midori 161106 add <--
// midori 152745 del -->
//// midori 152135,152136 add -->
//				m_nSort = pSort;			// 表示順
//// midori 152135,152136 add <--
//// midori 152165_2 add -->
//				m_OldCombo = m_nSort;
//// midori 152165_2 add <--
// midori 152745 del <--
// midori 152745 add -->
				m_nSort[0] = pSort[0];			// 表示順
				m_nSort[1] = pSort[1];			// 表示順
				m_nSortNum = 0;
				if(nType == ID_DLGTYPE_ADDRESS || nType == ID_DLGTYPE_ADDRESS_REFERENCE)	{
					if(m_nGrSeq == ID_ADDRESSGR_URI)	{
						m_nSortNum = 1;
					}
				}
// midori 152745 add <--
				//	検索用文字列取得
				m_udReference.szName1 = pudReference->szName1;
				m_udReference.szName2 = pudReference->szName2;
				m_udReference.szAdd1 = pudReference->szAdd1;
				m_udReference.szAdd2 = pudReference->szAdd2;

				DoModal();					//	モーダルで表示

			}
		}
		catch(...){
		}
	}

// midori 152745 del -->
//// midori 152135,152136 add -->
//	pSort = m_nSort;
//// midori 152135,152136 add <--
// midori 152745 del <--
// midori 152745 add -->
	pSort[0] = m_nSort[0];
	pSort[1] = m_nSort[1];
// midori 152745 add <--
	isRegAuto = m_isRegAuto;

	//	戻値を返す
	return( m_nRet );
}

//*************************************************************************
//	参照型が取引先の場合、コンボボックスによって得意先と仕入先を切り替える
//	【引数】	なし
//	【戻値】	なし
//*************************************************************************
void CdlgReference_Old::OnCbnSelchangeComboTorihikichange()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
// midori 152745 add -->
	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体
// midori 152745 add <--

	switch( m_cmbTorihikiChange.GetCurSel() ){
		case 0:
			m_nGrSeq = ID_ADDRESSGR_KAI;
			break;
		case 1:
			m_nGrSeq = ID_ADDRESSGR_URI;
			break;
	}
	// カナ検索ダイアログ初期化
// midori 152745 add -->
	m_nSortNum = m_cmbTorihikiChange.GetCurSel();
	m_clsFunc.DiagInit(&udData);
	m_DiagKanaSearch.GetData(ID_DIAG_INDEX_INPUT_SORT,(LPUNKNOWN)&udData);
	udData.data_combo = m_nSort[m_nSortNum];
	m_DiagKanaSearch.SetData(ID_DIAG_INDEX_INPUT_SORT,(LPUNKNOWN)&udData);
// midori 152745 add <--
// midori 152164 add -->
	m_Kana1 = _T("");
// midori 152164 add <--
	m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN, ID_DIAGDATA_EDIT, _T(""));

	// 表示切替
// midori 152745 del -->
//// midori 161109 del -->
////	GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
//// midori 161109 del <--
//// midori 161109 add -->
//	GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
	GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
	// 得意先と仕入先を切り替えた時、リストからインバースが消えていたので下記を追加
	ReDrawRowColor(1);
// midori 152745 add <--
}

//*************************************************************************
//	カナ検索ダイアグラムを参照型によって文言の変更、表示／非表示を切り替える
//	【引数】	参照型
//	【戻値】	TRUE/FALSE
//*************************************************************************
BOOL CdlgReference_Old::SetInitKanaSearchCtrl(EnumIdDlgType nType)
{
	int nRet = FALSE;
	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体

	// 背景色取得
	ICSColorInfo icsColInfo;
	m_ViewColor = GetSysColor( COLOR_BTNFACE );
	if ( ((ICSWinApp *)AfxGetApp())->GetStanderdColor( icsColInfo ) == 0 ){
		if ( icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON ){
			m_ViewColor = icsColInfo.ViewColor();
		}
	}
	// 背景色セット
	m_DiagKanaSearch.SetBackColor( m_ViewColor );

	// 文字列の背景セット
// midori 161109 add -->
	m_DiagKanaSearch.GetAttribute( ID_DIAG_INDEX_LABEL_SORT , (LPUNKNOWN)&diaatt );
	diaatt.attr_bcolor = m_ViewColor;
	m_DiagKanaSearch.SetAttribute( ID_DIAG_INDEX_LABEL_SORT , (LPUNKNOWN)&diaatt , TRUE );
// midori 161109 add <--

	m_DiagKanaSearch.GetAttribute( ID_DIAG_INDEX_LABEL_MAIN , (LPUNKNOWN)&diaatt );
	diaatt.attr_bcolor = m_ViewColor;
	m_DiagKanaSearch.SetAttribute( ID_DIAG_INDEX_LABEL_MAIN , (LPUNKNOWN)&diaatt , TRUE );

	m_DiagKanaSearch.GetAttribute( ID_DIAG_INDEX_LABEL_SUB , (LPUNKNOWN)&diaatt );
	diaatt.attr_bcolor = m_ViewColor;
	m_DiagKanaSearch.SetAttribute( ID_DIAG_INDEX_LABEL_SUB , (LPUNKNOWN)&diaatt , TRUE );

// midori 161109 add -->
	// 表示順の設定
	m_clsFunc.DiagInit( &udData );
	m_DiagKanaSearch.GetData( ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData );
// midori 152745 del -->
//	udData.data_combo = m_nSort;
// midori 152745 del <--
// midori 152745 add -->
	udData.data_combo = m_nSort[m_nSortNum];
// midori 152745 add <--
	m_DiagKanaSearch.SetData( ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData );
// midori 161109 add <--

	// 文字列セット＆表示/非表示切替
	switch( nType ){
		case ID_DLGTYPE_BANK :
		case ID_DLGTYPE_BANK_REFERENCE:
// midori 161109 del -->
			//m_clsFunc.DiagSetString( &m_DiagKanaSearch, 0, ID_DIAGDATA_DISP, _T("金融機関ｶﾅ検索") );
			//m_clsFunc.DiagSetString( &m_DiagKanaSearch, 2, ID_DIAGDATA_DISP, _T("支店名ｶﾅ検索") );

			//m_DiagKanaSearch.ModifyItem( 2, DIAG_MDFY_OPAQUE );
			//m_DiagKanaSearch.ModifyItem( 3, DIAG_MDFY_OPAQUE );
// midori 161109 del <--
// midori 161109 add -->
			m_clsFunc.DiagSetString( &m_DiagKanaSearch, 2, ID_DIAGDATA_DISP, _T("金融機関ｶﾅ検索") );
			m_clsFunc.DiagSetString( &m_DiagKanaSearch, 4, ID_DIAGDATA_DISP, _T("支店名ｶﾅ検索") );

			m_DiagKanaSearch.ModifyItem( 4, DIAG_MDFY_OPAQUE );
			m_DiagKanaSearch.ModifyItem( 5, DIAG_MDFY_OPAQUE );
// midori 161109 add <--
			break;

		case ID_DLGTYPE_ADDRESS:
		case ID_DLGTYPE_ADDRESS_REFERENCE:
// midori 161109 del -->
//			m_clsFunc.DiagSetString( &m_DiagKanaSearch, 0, ID_DIAGDATA_DISP, _T("取引先ｶﾅ検索") );
//
//			m_DiagKanaSearch.ModifyItem( 2, DIAG_MDFY_TRANSPARENT );
//			m_DiagKanaSearch.ModifyItem( 3, DIAG_MDFY_TRANSPARENT );
// midori 161109 del <--
// midori 161109 add -->
			m_clsFunc.DiagSetString( &m_DiagKanaSearch, 2, ID_DIAGDATA_DISP, _T("取引先ｶﾅ検索") );

			m_DiagKanaSearch.ModifyItem( 4, DIAG_MDFY_TRANSPARENT );
			m_DiagKanaSearch.ModifyItem( 5, DIAG_MDFY_TRANSPARENT );
// midori 161109 add <--
			break;

		case ID_DLGTYPE_KAMOKU:
// midori 161109 del -->
//			m_clsFunc.DiagSetString( &m_DiagKanaSearch, 0, ID_DIAGDATA_DISP, _T("科目名ｶﾅ検索") );
//
//			m_DiagKanaSearch.ModifyItem( 2, DIAG_MDFY_TRANSPARENT );
//			m_DiagKanaSearch.ModifyItem( 3, DIAG_MDFY_TRANSPARENT );
// midori 161109 del <--
// midori 161109 add -->
			m_clsFunc.DiagSetString( &m_DiagKanaSearch, 2, ID_DIAGDATA_DISP, _T("科目名ｶﾅ検索") );

			m_DiagKanaSearch.ModifyItem( 4, DIAG_MDFY_TRANSPARENT );
			m_DiagKanaSearch.ModifyItem( 5, DIAG_MDFY_TRANSPARENT );
// midori 161109 add <--
			break;
	}

	return nRet;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント（ICSDiagコントロール）
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::TerminationKanaSearchDiag(short index, short nChar, short length, LPUNKNOWN data)
{
	char cShift = ( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得

	//	仮想キーコードで分岐
// midori 152165 del -->
//	switch( nChar ){
//		//	「Tab」キーが押された
//// midori 161109 del -->
//	//case VK_TAB:
//	//	//	SHIFTキーが押されてる？
//	//	if ( cShift & 0x80 ){
//	//		if( index == ID_DIAG_INDEX_INPUT_MAIN ){
//	//			if( m_nType == ID_DLGTYPE_ADDRESS ){
//	//				m_cmbTorihikiChange.SetFocus();
//	//			}
//	//			else{
//	//				m_btnCancel.SetFocus();
//	//			}
//	//		}
//	//		else{
//	//			m_DiagKanaSearch.SetPrevPosition();			//	前のテキストにフォーカス移動
//	//		}
//	//	}
//	//	else{
//	//		if( m_nType == ID_DLGTYPE_BANK ){
//	//			if( index == ID_DIAG_INDEX_INPUT_SUB ){
//	//				m_lstReference.SetFocus();
//	//			}
//	//			else{
//	//				m_DiagKanaSearch.SetNextPosition();			//	次のテキストにフォーカス移動
//	//			}
//	//		}
//	//		else{
//	//			m_lstReference.SetFocus();
//	//		}
//	//	}
//	//	break;
//// midori 161109 del <--
//// midori 161109 add -->
//	case VK_TAB:
//		if( index == ID_DIAG_INDEX_INPUT_SORT ){
//			// 表示順コンボボックスが変更された場合にのみ
//			if(g_TabSw99 == FALSE)	{
//				//	SHIFTキーが押されてる？
//				if ( cShift & 0x80 ){
//					if( m_nType == ID_DLGTYPE_ADDRESS ){
//						m_cmbTorihikiChange.SetFocus();
//					}
//					else{
//						m_btnCancel.SetFocus();
//					}
//				}
//				else{
//					m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
//				}
//			}
//			if(g_TabSw99 == TRUE)	g_TabSw99 = FALSE;
//
//		}
//		break;
//// midori 161109 add <--
//	//	「Enter」キーが押された
//	case VK_RETURN:
//		// 表示切替
//// midori 161109 del -->
//		//GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
//		//	m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
//// midori 161109 del <--
//// midori 161109 add -->
//		if( index == ID_DIAG_INDEX_INPUT_SORT ){
//			FocusSet(0);
//			ReDrawRowColor(1);
//		}
//		else if( index == ID_DIAG_INDEX_INPUT_MAIN ){
//			if(m_Kana1 != m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ))	{
//				GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
//					m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
//			}
//			m_Kana1 = m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN );
//		}
//		else if( index == ID_DIAG_INDEX_INPUT_SUB ){
//			if(m_Kana2 != m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ))	{
//				GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
//					m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
//			}
//			m_Kana2 = m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB );
//		}
//// midori 161109 add <--
//		break;
//	//	その他
//	default:
//		break;
//	}
// midori 152165 del <--
// midori 152165 add -->
	switch( nChar ){
		//	「Tab」キーが押された
	case VK_TAB:
		if( index == ID_DIAG_INDEX_INPUT_SORT ){
			// 表示順コンボボックスが変更された場合にのみ
			if(g_TabSw99 == FALSE)	{
				//	SHIFTキーが押されてる？
				if ( cShift & 0x80 ){
					if( m_nType == ID_DLGTYPE_ADDRESS ){
						m_cmbTorihikiChange.SetFocus();
					}
					else{
						m_btnCancel.SetFocus();
					}
				}
				else{
					m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
				}
			}
			if(g_TabSw99 == TRUE)	g_TabSw99 = FALSE;

		}
		break;
	//	「Enter」キーが押された
	case VK_RETURN:
		// 表示切替
		if( index == ID_DIAG_INDEX_INPUT_SORT ){
			m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
		}
		else if( index == ID_DIAG_INDEX_INPUT_MAIN ){
			if(m_Kana1 != m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ))	{
// midori 152745 del -->
//				GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
//					m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 del <--
// midori 152745 add -->
				GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
					m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 add <--
			}
			m_Kana1 = m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN );

			if( m_nType == ID_DLGTYPE_BANK ){	// 検索ボックス2つ
				m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SUB );
			}
			else	{
				FocusSet(0);
				ReDrawRowColor(1);
			}
		}
		else if( index == ID_DIAG_INDEX_INPUT_SUB ){
			if(m_Kana2 != m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ))	{
// midori 152745 del -->
//				GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
//					m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 del <--
// midori 152745 add -->
				GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
					m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 add <--
			}
			m_Kana2 = m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB );
			FocusSet(0);
			ReDrawRowColor(1);
		}
		break;
	//	その他
	default:
		break;
	}
// midori 152165 add <--
}
void CdlgReference_Old::OnBnClickedCheckRegAuto()
{
	int state = m_chkRegAuto.GetCheck();
	if(state == BST_CHECKED)	m_isRegAuto = TRUE;
	else						m_isRegAuto = FALSE;
}

// midori 161109 add -->
//++++++++++++++++++++++++++++++++++++++++++++++++++
//	コンボボックスが変更された場合のイベント（ICSDiagコントロール）
//	【引数】	index	…	コントロール内のオフセット
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::ComboSelKanaSearchDiag(short index, LPUNKNOWN data)
{
	//int					nTopIndex=0;
	//DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体

	// 表示順コンボボックスが変更された場合にのみ
	if(index == ID_DIAG_INDEX_INPUT_SORT)	{
		KanaSearch_ComboSelChenge(index);
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	コンボボックスのデータが変わったかどうか
//	【引数】	index	…	コントロール内のオフセット
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference_Old::KanaSearch_ComboSelChenge(short index)
{
	short				sign=-1;
	int					nTopIndex=0;
	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体

	udData.data_combo = -1;
	m_DiagKanaSearch.GetData(index, (LPUNKNOWN)&udData);
	sign = udData.data_combo;

	if(sign == -1)	{	return;	}

// midori 152745 del -->
//	if(m_nSort != sign)	{
//		m_nSort = sign;
// midori 152745 del <--
// midori 152745 add -->
	if(m_nSort[m_nSortNum] != sign)	{
		m_nSort[m_nSortNum] = sign;
// midori 152745 add <--

		// 表示順：番号順
		// 表示順：50音順
		//	前回選択行をクリア
		m_nOldRow = -1;
		//	リストのトップインデックスを保持
		nTopIndex = m_lstReference.GetTopIndex();
		//	リストの更新
// midori 152745 del -->
//		GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 152745 del <--
// midori 152745 add -->
		GetListData( m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN), m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ) );
// midori 152745 add <--
		//	位置移動
		//MovePosition( nTopIndex, ( nRet -1 ) );
		MovePosition( nTopIndex, 0 );
// midori 152165_2 add -->
		// 下記不具合の対応
		// ①表示順のコンボにフォーカスがある状態で他のアプリに行き戻ってくる
		// ②その後、↓↑キーでコンボを変更してもコンボ自身の値が切り替わらない
		// 　(リストの並びは変わっている)
		m_DiagKanaSearch.RedrawWindow();
// midori 152165_2 add <--
	}
}

// midori 152135,152136 del -->
////++++++++++++++++++++++++++++++++++++++++++++++++++
////	Owntbからデータを取得
////++++++++++++++++++++++++++++++++++++++++++++++++++
//int CdlgReference_Old::Owntb_DataGet(void)
//{
//	int			flg=0;
//	int			sort=0;
//	CString		strSQL=_T("");
//	CString		strItmName=_T("");
//
//	// SQL文の作成
//	strSQL.Empty();
//	strItmName.Format(_T("option_sort"));
//	strSQL.Format(_T("apl_name = '%s' AND itm_name = '%s' AND itm_seq = 0 AND u_id = %d"), OWNTB_APLNAME, strItmName, TG_COMMON_USER_ID);
//
//	flg = 0;
//	// テーブルオープンチェック
//	if(m_pZmSub->owntb == NULL)	{
//		if(m_pZmSub->OwnTblOpen(strSQL) != 0)	{
//			return(-1);
//		}
//		flg = 1;
//	}
//	else	{
//		m_pZmSub->owntb->Requery(strSQL);
//	}
//	// 初めて実行？
//	if(m_pZmSub->owntb->st == -1)	{
//		// 新規作成
//		m_pZmSub->owntb->AddNew();
//
//		m_pZmSub->owntb->apl_name = OWNTB_APLNAME;
//		m_pZmSub->owntb->itm_name = strItmName;
//		m_pZmSub->owntb->itm_seq = 0;
//		m_pZmSub->owntb->u_id = TG_COMMON_USER_ID;
//		m_pZmSub->owntb->vl[0] = 0;
//		m_pZmSub->owntb->vl[1] = 0;
//		m_pZmSub->owntb->vl[2] = 0;
//
//		// 更新
//		m_pZmSub->owntb->Update();
//
//		sort = 0;
//	}
//	else	{
//		// 金融機関
//		if(m_nType == ID_DLGTYPE_BANK)	{
//			sort = m_pZmSub->owntb->vl[0];
//		}
//		// 取引先
//		else if(m_nType == ID_DLGTYPE_ADDRESS)	{
//			sort = m_pZmSub->owntb->vl[1];
//		}
//		// 科目
//		else if(m_nType == ID_DLGTYPE_KAMOKU)	{
//			sort = m_pZmSub->owntb->vl[2];
//		}
//	}
//
//	m_nSort = sort;
//// midori M-16113002 add -->
//	// カナ入力の初期化
//	m_Kana1 = _T("");
//	m_Kana2 = _T("");
//// midori M-16113002 add <--
//
//	// テーブルクローズ
//	if(flg == 1)	{
//		m_pZmSub->OwnTblClose();
//	}
//
//	return(0);
//}
//
////++++++++++++++++++++++++++++++++++++++++++++++++++
////	Owntbにデータをセット
////++++++++++++++++++++++++++++++++++++++++++++++++++
//int CdlgReference_Old::Owntb_DataSet(void)
//{
//	int			flg=0;
//	int			sort=0;
//	CString		strSQL=_T("");
//	CString		strItmName=_T("");
//
//	sort = m_nSort;
//
//	// SQL文の作成
//	strSQL.Empty();
//	strItmName.Format(_T("option_sort"));
//	strSQL.Format(_T("apl_name = '%s' AND itm_name = '%s' AND itm_seq = 0 AND u_id = %d"), OWNTB_APLNAME, strItmName, TG_COMMON_USER_ID);
//
//	flg = 0;
//	// テーブルオープンチェック
//	if(m_pZmSub->owntb == NULL)	{
//		if(m_pZmSub->OwnTblOpen(strSQL) != 0)	{
//			return(-1);
//		}
//		flg = 1;
//	}
//	else	{
//		m_pZmSub->owntb->Requery(strSQL);
//	}
//	// 初めて実行？
//	if(m_pZmSub->owntb->st != -1)	{
//		// 編集
//		m_pZmSub->owntb->Edit();
//
//		// 金融機関
//		if(m_nType == ID_DLGTYPE_BANK)	{
//			m_pZmSub->owntb->vl[0] = sort;
//		}
//		// 取引先
//		else if(m_nType == ID_DLGTYPE_ADDRESS)	{
//			m_pZmSub->owntb->vl[1] = sort;
//		}
//		// 科目
//		else if(m_nType == ID_DLGTYPE_KAMOKU)	{
//			m_pZmSub->owntb->vl[2] = sort;
//		}
//
//		// 更新
//		m_pZmSub->owntb->Update();
//	}
//
//	// テーブルクローズ
//	if(flg == 1)	{
//		m_pZmSub->OwnTblClose();
//	}
//
//	return(0);
//}
// midori 152135,152136 del <--
// midori 161109 add <--

// midori M-16113003 add -->
void CdlgReference_Old::FocusSet(int pSw)
{
	if(pSw == 0)	{
		if(m_lstReference.GetCount() > 0)	{
			m_lstReference.SetFocus();
		}
		else								{
			if(m_chkRegAuto.IsWindowVisible() == FALSE)	m_btnCancel.SetFocus();
			else										m_chkRegAuto.SetFocus();
		}
	}
	else	{
		if(m_lstReference.GetCount() > 0)	{
			m_lstReference.SetFocus();
		}
		else								{
			if( m_nType == ID_DLGTYPE_BANK ){	// 検索ボックス2つ
				m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_SUB );
			}
			else	{
				m_DiagKanaSearch.SetPosition( ID_DIAG_INDEX_INPUT_MAIN );
			}
		}
	}
}
// midori M-16113003 add <--

// midori M-16113002 add -->
void CdlgReference_Old::EditOFFIcsdiag8Kanasearch(short index)
{
	if( index == ID_DIAG_INDEX_INPUT_SORT )	{
		KanaSearch_ComboSelChenge(index);
	}
	else if( index == ID_DIAG_INDEX_INPUT_MAIN )	{
		if(m_Kana1 != m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ))	{
// midori 152745 del -->
			//GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
			//	m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 del <--
// midori 152745 add -->
			GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
				m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 add <--
		}
		m_Kana1 = m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN );
	}
	else if( index == ID_DIAG_INDEX_INPUT_SUB )	{
		if(m_Kana2 != m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ))	{
// midori 152745 del -->
			//GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort, m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
			//	m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 del <--
// midori 152745 add -->
			GetListData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN ), 
				m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB ));
// midori 152745 add <--
		}
		m_Kana2 = m_clsFunc.DiagGetString( &m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB );
	}
}
// midori M-16113002 add <--

// midori 152165_2 add -->
void CdlgReference_Old::EditONIcsdiag8Kanasearch(short index)
{
	DIAGRAM_DATA diadata;

	if(index == ID_DIAG_INDEX_INPUT_SORT)	{
		m_DiagKanaSearch.GetData(index,(LPUNKNOWN)&diadata);
		m_OldCombo = diadata.data_combo;
	}
}
// midori 152165_2 add <--

// 改良No.22-0404,22-0812 <--
