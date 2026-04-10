//--------------------------------------------------
//	frmUc051Tanaorosi.cpp
//
//	2006.02.23～
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc051Tanaorosi.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

// 改良No.21-0086,21-0529 cor -->
//// No.200905 add -->
//extern	int	KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// No.200905 add <--
// ------------------------------
extern	int	KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--


IMPLEMENT_DYNCREATE(CfrmUc051Tanaorosi, CfrmUc000Common)

BEGIN_MESSAGE_MAP(CfrmUc051Tanaorosi, CfrmUc000Common)

	//	ファンクションキー
//	ON_COMMAND(ID_BUTTON_F1, OnButtonF1)			//	F1	：ヘルプ→一行上へ
	ON_COMMAND(ID_BUTTON_F2, OnButtonF2)			//	F2	：一行上へ→前項目
//	ON_COMMAND(ID_BUTTON_F3, OnButtonF3)			//	F3	：一行下へ
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)			//	F4	：プレビュ
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)			//	F5	：印刷
	ON_COMMAND(ID_BUTTON_F6, OnButtonF6)			//	F6	：項目複写
	ON_COMMAND(ID_BUTTON_F7, OnButtonF7)			//	F7	：削除
	ON_COMMAND(ID_BUTTON_F8, OnButtonF8)			//	F8	：挿入
	ON_COMMAND(ID_BUTTON_F9, OnButtonF9)			//	F9	：特殊行
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)			//	F11	：参照
	ON_COMMAND(ID_BUTTON_F12, OnButtonF12)			//	F12	：ソート
	ON_COMMAND(ID_BUTTON_HOME, OnButtonHome)		//	Home：住所検索
	ON_COMMAND( ID_BUTTON_INSERT, OnButtonInsert )	//　Ins	：様式選択
	//ON_COMMAND(ID_BUTTON_END, OnButtonEnd)		//	End	：処理終了
	
	//	メニュー
	ON_COMMAND(ID_PRINTSETUP_MENU, &CfrmUc051Tanaorosi::OnMenuPrintSetup)		//	印刷設定
	ON_COMMAND(ID_PRINTALL_MENU, &CfrmUc051Tanaorosi::OnMenuPrintAll)			//	一括印刷
//	ON_COMMAND(ID_OUTPUTFORM_MENU, &CfrmUc051Tanaorosi::OnMenuFormTypeSetup)	//	出力形式（※⑤棚卸資産には不要）
	ON_COMMAND( ID_SPECIALROW_MENU, &CfrmUc051Tanaorosi::OnMenuSpecialRow )		//	特殊行挿入
	ON_COMMAND( ID_SORT_MENU, &CfrmUc051Tanaorosi::OnMenuSort )					//	ソート
	ON_COMMAND(ID_IKKATUADD_MENU, &CfrmUc051Tanaorosi::OnIkkatuAddMenu)
	ON_COMMAND(ID_DISPDIALOG_MENU, &CfrmUc051Tanaorosi::OnMenuIkkatuKingaku)	//	一括金額
	ON_COMMAND(ID_ZEROMONEY_MENU, &CfrmUc051Tanaorosi::OnMenuZeroMoney)			//	一時保管
	ON_COMMAND(ID_ZEROMONEYMOVE_MENU, &CfrmUc051Tanaorosi::OnMenuZeroMoneyMove)	//	一括で一時保管へ移動
	ON_COMMAND(ID_AMOUTDIALOG_MENU, &CfrmUc051Tanaorosi::OnMenuOutKeiSetup)		//	計設定
// midori 160606 add -->
	ON_COMMAND(ID_CLEAR_MENU, &CfrmUc051Tanaorosi::OnMenuClear)					//	金額等数値0クリア
// midori 160606 add <--
	ON_COMMAND(ID_TITLEMENU, &CfrmUc051Tanaorosi::OnMenuTitleRestore)			//	タイトルを戻す
	ON_COMMAND(ID_USER_SETTING_MENU, &CfrmUc051Tanaorosi::OnMenuOptionSetting)	//　オプション設定
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc051Tanaorosi::OnMenuCopySettingInfo )	// 他マスターから設定情報のコピー
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc051Tanaorosi::OnMenuDeleteDataEachYoushiki )	// 様式別全データ削除

	//	頁操作系ボタン
	ON_BN_CLICKED(IDC_PAGEBACK_BUTTON1, &CfrmUc051Tanaorosi::OnPageBack)		//	前頁
	ON_BN_CLICKED(IDC_PAGENEXT_BUTTON1, &CfrmUc051Tanaorosi::OnPageNext)		//	次頁
	ON_BN_CLICKED(IDC_PAGENEW_BUTTON1, &CfrmUc051Tanaorosi::OnPageNew)			//	改頁
	ON_BN_CLICKED(IDC_PAGEINSERT_BUTTON1, &CfrmUc051Tanaorosi::OnPageInsert)	//	頁挿入	

	//	コピー／貼り付け系ボタン
	ON_BN_CLICKED(IDC_ROWCOPY_BUTTON1, &CfrmUc051Tanaorosi::OnRowCopy)			//	行コピー
	ON_BN_CLICKED(IDC_ROWPASTE_BUTTON1, &CfrmUc051Tanaorosi::OnRowPaste)		//	行貼付
	ON_BN_CLICKED(IDC_ROWINSERT_BUTTON1, &CfrmUc051Tanaorosi::OnRowInsert)		//	行挿入貼付
	ON_BN_CLICKED(IDC_MARGINAL_BUTTON1, &CfrmUc051Tanaorosi::OnBnClickedMarginalButton1)
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc051Tanaorosi::OnMoveYoushikiNext )	//　行挿入
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc051Tanaorosi::OnMoveYoushikiBack )			//　行削除
	
	//	ファンクションキー更新
//	ON_UPDATE_COMMAND_UI(ID_BUTTON_F1, &CfrmUc051Tanaorosi::OnUpdateButtonF1)	//	F1
//	ON_UPDATE_COMMAND_UI(ID_BUTTON_F3, &CfrmUc051Tanaorosi::OnUpdateButtonF3)	//	F3
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F6, &CfrmUc051Tanaorosi::OnUpdateButtonF6)	//	F6
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CfrmUc051Tanaorosi::OnUpdateButtonF7)	//	F7
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, &CfrmUc051Tanaorosi::OnUpdateButtonF8)	//	F8
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CfrmUc051Tanaorosi::OnUpdateButtonF9)	//	F9
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, &CfrmUc051Tanaorosi::OnUpdateButtonF11)	//	F11
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, &CfrmUc051Tanaorosi::OnUpdateButtonF12)	//	F12
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc051Tanaorosi::OnUpdateButtonHome ) //　HOME
	ON_UPDATE_COMMAND_UI(ID_IKKATUADD_MENU, &CfrmUc051Tanaorosi::OnUpdateIkkatuaddMenu)
	ON_UPDATE_COMMAND_UI(ID_DISPDIALOG_MENU, &CfrmUc051Tanaorosi::OnUpdateMenuIkkatuKingaku)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_SORT_MENU, &CfrmUc051Tanaorosi::OnUpdateMenuSort )
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc051Tanaorosi::OnUpdateMenuClear )
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI(ID_ZEROMONEY_MENU, &CfrmUc051Tanaorosi::OnUpdateMenuZeroMoney)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc051Tanaorosi::OnUpdateMenuZeroMoneyMove)
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI(ID_SPECIALROW_MENU, &CfrmUc051Tanaorosi::OnUpdateMenuSpecialRow)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc051Tanaorosi::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc051Tanaorosi::OnUpdateMenuDeleteDataEachYoushiki )
// midori 160612 add <--
	ON_MESSAGE(WM_USER_REDRAWVIEW, OnUserReDrawView)		// 修正No.162446 add
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CfrmUc051Tanaorosi, CfrmUc000Common)
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG1, 3, CfrmUc051Tanaorosi::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG1, 1, CfrmUc051Tanaorosi::EditONYoushikidiag1, VTS_I2)
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG1, 2, CfrmUc051Tanaorosi::EditOFFYoushikidiag1, VTS_I2)
// midori 151405 add -->
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG1, 12, CfrmUc051Tanaorosi::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG1, 8, CfrmUc051Tanaorosi::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG3, 3, CfrmUc051Tanaorosi::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG3, 1, CfrmUc051Tanaorosi::EditONYoushikidiag1, VTS_I2)
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG3, 2, CfrmUc051Tanaorosi::EditOFFYoushikidiag1, VTS_I2)
// midori 151405 add -->
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG3, 12, CfrmUc051Tanaorosi::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
ON_EVENT(CfrmUc051Tanaorosi, IDC_YOUSHIKIDIAG3, 8, CfrmUc051Tanaorosi::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

ON_EVENT(CfrmUc051Tanaorosi, IDC_TITLEDIAG1, 3, CfrmUc051Tanaorosi::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CfrmUc051Tanaorosi, IDC_TITLEDIAG1, 2, CfrmUc051Tanaorosi::EditOFFTitlediag1, VTS_I2)
ON_EVENT(CfrmUc051Tanaorosi, IDC_TITLEDIAG1, 1, CfrmUc051Tanaorosi::EditONTitlediag1, VTS_I2 )
ON_EVENT(CfrmUc051Tanaorosi, IDC_TITLEDIAG1, 6, CfrmUc051Tanaorosi::FocusTitlediag1, VTS_NONE)

ON_EVENT(CfrmUc051Tanaorosi, IDC_PAGEDIAG1, 3, CfrmUc051Tanaorosi::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CfrmUc051Tanaorosi, IDC_PAGEDIAG1, 2, CfrmUc051Tanaorosi::EditOFFPagediag1, VTS_I2)
ON_EVENT(CfrmUc051Tanaorosi, IDC_PAGEDIAG1, 1, CfrmUc051Tanaorosi::EditONPagediag1, VTS_I2 )
ON_EVENT(CfrmUc051Tanaorosi, IDC_PAGEDIAG1, 6, CfrmUc051Tanaorosi::FocusPagediag1, VTS_NONE)
END_EVENTSINK_MAP()

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CfrmUc051Tanaorosi::CfrmUc051Tanaorosi()
	: CfrmUc000Common(CfrmUc051Tanaorosi::IDD)
{
	//	データベースのハンドル参照
	m_pDB = ((CUCHIWAKEApp*)AfxGetApp())->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
// midori 152718 add -->
	m_pPageDiag	= &m_PageDiag;
// midori 152718 add <--
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CfrmUc051Tanaorosi::~CfrmUc051Tanaorosi()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc051Tanaorosi::~CfrmUc051Tanaorosi\n"));

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;
}

void CfrmUc051Tanaorosi::DoDataExchange(CDataExchange* pDX)
{
	CfrmUc000Common::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TITLEDIAG1, m_TitleDiag);
	DDX_Control(pDX, IDC_PAGEDIAG1, m_PageDiag);
	DDX_Control(pDX, IDC_NUMBER_DIAG1, m_NumberDiag);
	DDX_Control( pDX, IDC_YOUSHIKIDIAG1, m_Diag1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG3, m_Diag2 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI1, m_DiagKei1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI2, m_DiagKei2 );
	DDX_Control(pDX, IDC_MARGINAL_BUTTON1, m_btnMarjinal);
	DDX_Control(pDX, IDC_TOPICDIAG1, m_HeadDiag);
// midori 161108 add -->
	DDX_Control(pDX, IDC_SAVEDATA_DIAG1, m_SaveDataDiag);
// midori 161108 add <--
// 修正No.157626 add -->
	DDX_Control(pDX, IDC_ICSDISPGUIDE1, m_Guide1);
// 修正No.157626 add <--
}

//--------------------------------------------------
//	初期更新
//--------------------------------------------------
void CfrmUc051Tanaorosi::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if(m_InitialFlag == TRUE)	{
		// コンボボックスアイテムのリサイズ制御
		m_Diag1.ComboxFontMode(TRUE);
		m_Diag2.ComboxFontMode(TRUE);

		// 背景色を取得
		m_ViewColor = CmnGetBackColor();

// 修正No.158252 add -->
		// 欄外ガイドの背景色
		m_Guide1.SetBackColor(m_ViewColor);
		m_Guide1.ShowWindow(FALSE);
// 修正No.158252 add <--

		// リサイズ
		//SetResize( IDC_YOUSHIKIDIAG1 , ICS_RESIZE_BOTTOM );

		// 帳表表示処理OK?
		if(virStartJob() == TRUE)	{
			// 最小化／最大化ボックスを無効に設定
//			CmnEnableMinMaxBox( GetParent() );
		}
		else	{
			// カーソルフラグ=FALSE(この後フォーカスセット処理が走らないように)
			m_CursorSetFlag = FALSE;
		}
	}
}

// 要通知案件修正 21/07/09 add -->
void CfrmUc051Tanaorosi::virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame)
{
	int		index = 0;
	CWnd*	pWnd = this->GetFocus();

	if(pWnd != NULL)	{
		if(m_DataKakutei == FALSE)	{
			if(m_uInfo.intCurCol == ID_COL_051_KAMOKU)	{
				virKamokuAddCancel();
			}
		}
	}
}

void CfrmUc051Tanaorosi::virKamokuAddCancel()
{
	if(m_DataKakutei == TRUE)	return;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition(&m_uInfo.intCurRow,&m_uInfo.intCurCol,&m_uInfo.OldCol);
	// 科目列なら
	if(m_uInfo.intCurCol == ID_COL_051_KAMOKU && m_F9OnSw != TRUE)	{
		DIAGRAM_DATA	diadata;
		m_pDiag->GetData(m_uInfo.intCurCtlIndex,(LPUNKNOWN)&diadata);
		// 【新規作成】が選択されていれば
		if(diadata.data_combo > m_CmbCnt)	{
			// 前回の値に戻す
			CmnDiagSetCombo(m_pDiag, CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_051_KAMOKU), m_uInfo.OldCombo);
		}
	}
}
// 要通知案件修正 21/07/09 add <--

//--------------------------------------------------
//	ウィンドウメッセージをフィルタリング
//	【引数】	pMsg	…	MSG構造体のポインタ
//	【戻値】	0		…	メッセージを通常の方法で処理
//				0以外	…	メッセージ処理を終了
//--------------------------------------------------
BOOL CfrmUc051Tanaorosi::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	
//	int				intID;
	BOOL			blnMove = TRUE;		// カーソル移動可能/不可フラグ
	int				intOrgIndex;		// カーソル移動前のインデックス番号
	int				intMoveRow;			// カーソル移動先（行）
	int				intMoveCol;			// カーソル移動先（列）
// midori 160611 add -->
	int				rv=0;
// midori 160611 add <--
// midori 152364 add -->
	int				intRet=0;
// midori 152364 add <--
// midori 151405 add -->
	int				id=0;
// midori UC_0001 add -->
	int				nMaxDataCol=0;		// 1頁の最大行数
	int				intTotalType=0;		// 合計タイプ
// midori UC_0001 add <--
// midori 152134 17/10/07 add -->
	int				intCol=0;
	DIAGRAM_DATA	diadata;
// midori 152134 17/10/07 add <--
	CWnd* pWnd = this->GetFocus();

	if ( pWnd != NULL ){
		id = pWnd->GetDlgCtrlID();
		if(id == IDC_PAGEBACK_BUTTON1 || id == IDC_PAGENEXT_BUTTON1 || id == IDC_PAGENEW_BUTTON1 || 
			id == IDC_PAGEINSERT_BUTTON1 || id == IDC_ROWCOPY_BUTTON1 || id == IDC_ROWPASTE_BUTTON1 || 
			id == IDC_ROWINSERT_BUTTON1 || id == IDC_YOUSHIKIBACK_BUTTON1 || id == IDC_YOUSHIKINEXT_BUTTON1 || 
			id == IDC_MARGINAL_BUTTON1)	{
			nG_MScroll = 0;
		}
	}
// midori 151405 add <--

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		//	パラメータで分岐
		switch( pMsg->wParam ){
// midori 151405 del -->
//// midori 152364 add -->
//		// Enterキー
//		case VK_RETURN:
//			if(IsSpecialRow(m_ReadData[m_uInfo.intCurRow].m_FgFunc) == 1)	{
//				intRet = GetFocusPosition(VK_RETURN);
//				m_pDiag->SetPosition(intRet);
//				rv=1;
//			}
//			break;
//// midori 152364 add <--
// midori 151405 del <--
// midori 151405 add -->
		// Enterキー
		case VK_RETURN:
			if(nG_MScroll == 1)	{
				rv = CursorControl((short)pMsg->wParam,1);
			}
			else if(m_DataKakutei == FALSE && m_uInfo.intCurFocus == 1)	{
				if(IsSpecialRow(m_ReadData[m_uInfo.intCurRow].m_FgFunc) == 1)	{
					intRet = GetFocusPosition(VK_RETURN);
					m_pDiag->SetPosition(intRet);
					rv=1;
				}
			}
			break;
// midori 151405 add <--
		// PageUp，PageDownキー
		case VK_PRIOR:
		case VK_NEXT:
// midori 152390 add -->
			if(m_DataKakutei == FALSE)	{
// midori 152390 add <--
// midori 151405 del -->
				//// データ入力欄にフォーカスある場合のみ実行を許す
				//if (m_uInfo.intCurFocus != 1) {
				//	// 何もせずに抜ける
				//	return CfrmUc000Common::PreTranslateMessage(pMsg);
				//}
// midori 151405 del <--
// midori 151405 add -->
				if (m_uInfo.intCurFocus != 1 && nG_MScroll == 0) {
					// 何もせずに抜ける
					break;
				}
// midori 151405 add <--

// midori 160611 del -->
//				// ComboBoxではPageUp，PageDownキーを実行しない又は計行である又は特殊行である
//				if ((CmnGetControlCol() != ID_COL_051_KAMOKU) || (CmnCheckTotalRow() == TRUE )){
// midori 160611 del <--
				// カレントインデックスを保存
				intOrgIndex = m_pDiag->GetPosition();

				// カレント行を取得
				if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
					intMoveRow = CmnGetControlRow();		// データ行の場合
					intMoveCol = m_uInfo.intCurCol;
				}
				else if (m_uInfo.intCurCtlIndex >= m_uInfo.intT2StartIndex) {
					intMoveRow = m_uInfo.intRowMax;			// 合計行②の場合(24)
					intMoveCol = m_uInfo.intCursolDefPos;	// 移動先がデータ行になるため、Combo以外へ移動したい
				}
				else {
					intMoveRow = m_uInfo.intRowMax - 1;		// 合計行①の場合(23)
					intMoveCol = m_uInfo.intCursolDefPos;	// 移動先がデータ行になるため、Combo以外へ移動したい
				}

// midori 152134 17/10/07 add -->
				intCol = CmnGetControlCol();
				// 例外処理：移動前に【新規作成】が選ばれている場合は値を元に戻す
				if (intCol == ID_COL_051_KAMOKU) {
					m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
					if(diadata.data_combo > m_CmbCnt)	{
						m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
						CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
					}
				}
// midori 152134 17/10/07 add <--

				// Scroll＋フォーカス移動先を取得
				if (pMsg->wParam == VK_PRIOR) {
					// PageUpの場合：Scroll＋フォーカス移動（行－１０）
					m_pDiag->ScrollPos(SB_PAGEUP , 0);
					intMoveRow = intMoveRow - 10;
					if (intMoveRow < 1) {
						blnMove = FALSE;					// 移動しない
					}
				}
				else {
					// PageDownの場合：Scroll＋フォーカス移動（行＋１０）
					m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
					intMoveRow = intMoveRow + 10;
// midori UC_0001 del -->
//					if (intMoveRow > m_uInfo.intRowMax) {
// midori UC_0001 del <--
// midori UC_0001 add -->
					// カレント頁の合計行の種別を取得
					intTotalType = CmnGetControlTotalType();
					// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
					// 頁計と累計
					if(intTotalType == ID_OUTKEI_BOTH)	nMaxDataCol = m_uInfo.intRowMaxData - 1;
					else								nMaxDataCol = m_uInfo.intRowMaxData;

					if (intMoveRow > nMaxDataCol) {
// midori UC_0001 add <--
						blnMove = FALSE;					// 移動しない
					}
				}

				// カーソル移動処理
				if (blnMove == TRUE) {
					// 頁計/累計行の制御は、当処理で制御する
					CmnDiagSetFocus(m_pDiag, intMoveRow, intMoveCol);
				}
				else {
					// 移動しない場合でも同じ位置にカーソルを再設定
					m_pDiag->SetPosition(intOrgIndex);
				}
// midori 152134 17/10/07 del -->
//// midori 160611 cor -->
//				//}
//// ---------------------
//				if(CmnGetControlCol() == ID_COL_051_KAMOKU)	{
//					rv=1;
//				}
//// midori 160611 cor <--
// midori 152134 17/10/07 del <--
// midori 152134 17/10/07 add -->
				if(intCol == ID_COL_051_KAMOKU)	{
					rv=1;
				}
// midori 152134 17/10/07 add <--
// midori 152390 add -->
			}
			else	{
				// Scroll
				if (pMsg->wParam == VK_PRIOR)	m_pDiag->ScrollPos(SB_PAGEUP , 0);
				else							m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
			}
// midori 152390 add <--
			break;
/*
		// Tabキー（前頁ボタンで[Shift＋Tab]キー処理。これ以外は別のイベントで行う）
		case VK_TAB:
			// 現在フォーカスを持つコントロールのIDを取得
			intID = CmnGetDlgCtrlID();

			// 前頁ボタンで[Shift＋Tab]キーが押されたらカーソルセット
			if ((intID == IDC_PAGEBACK_BUTTON1) && (CmnCheckShiftKey() == TRUE)) {
				// データ部の最終行へ移動（頁計/累計行の制御を行う）
				CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_051_KAMOKU);
				return 1;
			}
			break;
*/
// midori 151405 add -->
		// カーソル移動キー
		case VK_TAB:
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			if(nG_MScroll == 1)	{
				rv = CursorControl((short)pMsg->wParam,1);
			}
// 修正No.158259 add -->
			else if(m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
				intCol = CmnGetControlCol();
				if(intCol == ID_COL_051_KAMOKU && (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT))	{
					rv = CursorControl((short)pMsg->wParam, 0);
				}
			}
// 修正No.158259 add <--
			break;
// midori 151405 add <--
		}
	}
// 要通知案件修正 21/07/09 add -->
	// マウス左クリック
	else if(pMsg->message == WM_LBUTTONDOWN)	{
		if(bG_Kanso == FALSE)	{	// 棚卸方法登録ボタンが有効なのは簡素化以前の様式のみ
			// 科目コンボボックスで【新規追加】が選択されたままタイトル部やページ番号部にカーソルがセットされた時、
			// コンボボックスの値を元に戻す
			int		id = -1;
			POINT	pt;
			RECT	rt;

			// 現在のカーソルポジションを取得する
			GetCursorPos(&pt);
			ScreenToClient(&pt);

			m_btnMarjinal.GetWindowRect(&rt);
			ScreenToClient(&rt);
			if(pt.x >= rt.left && pt.x <= rt.right && pt.y >= rt.top && pt.y <= rt.bottom)	{
				id = IDC_MARGINAL_BUTTON1;		// ⑤棚卸方法登録ボタン
			}
			if(id != -1) {
				CmnEventStartProc(id);
			}
		}
	}
// 要通知案件修正 21/07/09 add <--

// midori 160611 add -->
	if(rv == 1)	{
		return(TRUE);
	}
// midori 160611 add <--

	return CfrmUc000Common::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント（データ用）
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::TerminationYoushikidiag1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	// 現在の列番号を取得
	int	intCol = CmnGetControlCol();

	switch (nChar) {
	// Deleteキー
	case VK_DELETE:
		// 項目をクリア（ただしComboBoxはクリアしない）
		switch (m_ReadData[m_uInfo.intCurRow].m_FgFunc) {
		case ID_FGFUNC_SYOKEI:				// 小計
		case ID_FGFUNC_SYOKEINULL:			// 小計（空行）
		case ID_FGFUNC_CHUKEI:				// 中計
		case ID_FGFUNC_CHUKEINULL:			// 中計（空行）
		case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
		case ID_FGFUNC_PAGEKEI:				//	頁計
		case ID_FGFUNC_RUIKEI:				//	累計
			break;
		default:
			
			//	列で分岐
			switch( intCol ){

			//	科目
			case ID_COL_051_KAMOKU:
				break;

			//	その他
			default:
				//	データクリア
				m_pDiag->DataClear(m_uInfo.intCurCtlIndex, TRUE);

				//	「期末現在高」の列？
				if ( intCol == ID_COL_051_KIMATU ){
					//	一個前の文字列もクリア（この後、EditOFFイベント発生のため）
					m_uInfo.OldstrDiag = _T("");
					// 行・列取得
					CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );
					//	自動計算フラグを自動計算に設定
					m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_AUTO;
					//	背景色を変更
					virInitialControlFgInputCell( m_uInfo.intCurRow, m_ReadData[m_uInfo.intCurRow].m_FgFunc, m_ReadData[m_uInfo.intCurRow].m_FgInput );
				}
				break;
			}
			break;
		}

// midori 151405 del -->
	//// カーソル移動キー
	//case VK_TAB:
	//case VK_RETURN:
	//case VK_UP:
	//case VK_DOWN:
	//case VK_LEFT:
	//case VK_RIGHT:
	//	// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
	//	if ((nChar == VK_LEFT) || (nChar == VK_RIGHT)) {
	//		// ComboBoxは選択内容を確定させない
	//		if (intCol == ID_COL_051_KAMOKU) {
	//			m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
	//			CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
	//		}
	//	}

	//	// 期末現在高で値が入力された場合の金額確定処理
	//	//if( intCol == ID_COL_051_KIMATU ){		
	//	//	switch(nChar){
	//	//		// 行を跨がないカーソル移動
	//	//		case VK_TAB:
	//	//		case VK_RIGHT:
	//	//		case VK_LEFT:
	//	//		case VK_RETURN:
	//	//			int intRow = CmnGetControlRow();
	//	//			// 登録処理
	//	//			virRecBufSaveData(m_uInfo.intCurPage , intRow );
	//	//			break;
	//	//	}
	//	//}

	//	// フォーカス移動先を取得
	//	int intRet = GetFocusPosition(nChar);
	//	switch (intRet) {
	//	case -1:		// 先頭項目でShift+Tabキーのため、タイトルへ移動
	//	case -4:		// 先頭行で↑or先頭項目で←は、タイトルへ移動
	//		m_TitleDiag.SetPosition( 0 );
	//		break;
	//	case -2:	// 最終項目でTabキーのため、頁コントロールへ移動
	//		m_PageDiag.SetPosition(1);
	//		break;
	//	case -3:		// ComboBoxで↑↓キーのため何もしない
	//		break;
	//	default:		// フォーカス移動を行う
	//		m_pDiag->SetPosition(intRet);
	//		break;
	//	}

	//	// ポジション取得
	//	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	//	break;
// midori 151405 del <--
// midori 151405 add -->
	// カーソル移動キー
	case VK_TAB:
	case VK_RETURN:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		CursorControl(nChar,0);
		break;
// midori 151405 add <--
	}
}

// midori 151405 add -->
void CfrmUc051Tanaorosi::VScrollYoushikidiag1(short mode)
{
	if(mode == 8 && m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
// midori UC_0002 add -->
		virInputDecisionEditOFF();
// midori UC_0002 add <--
		nG_MScroll=1;
	}
}
// midori 151405 add <--

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	エディットオンイベント（データ用）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::EditONYoushikidiag1(short index)
{
	DIAGRAM_DATA diadata;

// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
// midori 151405 add <--
// midori 152134 add -->
	m_ComboSgn = 0;
// midori 152134 add <--

	// フォーカスフラグON
	m_uInfo.intCurFocus = 1;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

	// 「種類」列ならコンボボックスのオフセット値取得
	if ( m_uInfo.intCurCol == ID_COL_051_KAMOKU ){
		m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
// 要通知案件修正 21/07/09 del -->
		//m_uInfo.OldCombo = diadata.data_combo;
// 要通知案件修正 21/07/09 del <--
// 要通知案件修正 21/07/09 add -->
		if(diadata.data_combo <= m_CmbCnt)	{
			// 選択しているのが【新規作成】であれば保存しない
			// 1.キー操作で【新規作成】までもっていく。
			// 2.欄外の例えば今回追加したガイド等をクリックする（コンボは閉じるが【新規作成】のまま）
			// 3.ウインドウの右下をドラッグして少し小さくする（【新規作成】のまま）
			// 4.[Ins 様式切替] 等、EditOff が発生する動作を行う
			// 5.科目追加ダイアログが開かれていて閉じるともう一度表示される
			// ※3の操作でEditOnが発生して OldCombo に【新規作成】が保存される
			m_uInfo.OldCombo = diadata.data_combo;
		}
// 要通知案件修正 21/07/09 add <--
	}
	else{
		// 現在の入力値を取得
		m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );
	}

	// 行移動確認
	if ( /*( m_uInfo.OldRow != CmnGetControlRow() ) &&*/ ( m_uInfo.OldRow > 0 ) ){
		if ( m_uInfo.OldPage == m_uInfo.intCurPage && m_uInfo.OldPage > 0 ) {
//			if( CmnGetControlCol() != ID_COL_051_KIMATU ){
				if ( virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow ) == FUNCTION_UPDATE) {
					// レコード更新があったとき、再表示する

					// 計再計算
					CmnTblCalcKei();

					// 1頁再表示
					virUpdateControlTblData();

					// カーソル移動処理
					CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
				}
//			}
		}
	}

	// ボタン操作
	CmnChangeButtonEnable(1);
}

// データコントロールのロストフォーカス
void CfrmUc051Tanaorosi::EditOFFYoushikidiag1(short index)
{
	if( m_EndView != FALSE )	return;
	virInputDecisionEditOFF();

// midori 152134 del -->
//// midori 161106 add -->
//	ComboNew(index);
//// midori 161106 add <--
// midori 152134 del <--
// midori 152134 add -->
	if(m_uInfo.intCurCol == ID_COL_051_KAMOKU)	{
		if(m_ComboSgn != 1)	{
			ComboNew(index);
		}
	}
	m_ComboSgn = 0;
// midori 152134 add <--
}

//**************************************************
//	入力確定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virInputDecisionEditOFF()
{
	// フォーカスフラグOFF
	m_uInfo.intCurFocus = 0;
	
	//	期末現在高の値が範囲を超えてない？
	//	（※期末現在高の値が範囲を超えて"************"で表示されている場合、入力を確定してしまうと"0"になってしまう）
//	if ( CheckOutsideValue( m_pDiag, m_pDiag->GetPosition(), m_uInfo.intCurCol ) == FALSE ){
		//	入力項目を強制的に確定
		CmnDiagSetEnterReDraw( m_pDiag, m_pDiag->GetPosition() );
//	}

	// 自動一括集計行なら、変更できてしまうComboBoxの値を元に戻す
// No.200905 del -->
	//if (m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// No.200905 del <--
// No.200905 add -->
// 改良No.21-0086,21-0529 cor -->
	//if (KamokuRowEnableSgn(m_pDB,0) == 0 && m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// ------------------------------
	if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 0 && m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// 改良No.21-0086,21-0529 cor <--
// No.200905 add <--
		if (m_uInfo.intCurCol == ID_COL_051_KAMOKU) {
			//	前回の値に戻す
			CmnDiagSetCombo(m_pDiag, CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_051_KAMOKU), m_uInfo.OldCombo);
		}
	}

	//	自動計算フラグ確認
	CheckFgInput();
	//	自動計算結果を設定
	SetAutoCalc();

	// 行取得
	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
		m_uInfo.OldRow = CmnGetControlRow();
	}
	else{
		m_uInfo.OldRow = -1;
	}

	// 金額欄の入力が確定した時点で反映
//	if (  m_uInfo.OldRow > 0 ){
//		//if( CmnGetControlCol() == ID_COL_051_KIMATU ){
//			//if ( m_uInfo.OldPage == m_uInfo.intCurPage ) {
//				// カレント行登録＋再表示
//				virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow );
//			//}
//		//}
//	}

	// ページ番号取得
	m_uInfo.OldPage = m_uInfo.intCurPage;

	// ボタン操作
	CmnChangeButtonEnable(0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント（タイトル用）
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::TerminationTitlediag1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	// フォーカス移動
	switch (nChar) {
	case VK_RETURN:
		// １行目の左上へ移動
		CmnDiagSetFocus(m_pDiag, 1, 1);
		break;
	case VK_TAB:
		// Shiftキーの有無は？
		if (CmnCheckShiftKey() == FALSE) {
			// １行目の左上へ移動
			CmnDiagSetFocus(m_pDiag, 1, 1);
		}
		else {
			// 頁コントロールへ移動
			m_PageDiag.SetPosition(1);
		}
		break;
	case VK_UP:
		// 頁コントロールへ移動
		m_PageDiag.SetPosition(1);
		break;
	case VK_DOWN:
		// データ部の初期位置へ移動
		CmnDiagSetFocus(m_pDiag, 1, m_uInfo.intCursolDefPos);
		break;
	case VK_DELETE:
		// タイトル削除
		m_TitleDiag.DataClear( 0 , TRUE );
		break;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	エディットオンイベント（タイトル用）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::EditONTitlediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Titlefocus = 1;
// midori 151405 add <--
	// 1行登録
//	if( CmnGetControlCol() != ID_COL_051_KIMATU ){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_TitleDiag.SetPosition(0);
		}
//	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	エディットオフイベント（タイトル用）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::EditOFFTitlediag1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

// midori 151405 add -->
	m_Titlefocus = 0;
// midori 151405 add <--
	virUpdateTitle();
}

//	タイトル更新
void CfrmUc051Tanaorosi::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString(&m_TitleDiag, 0);
	CmnUcInfSubSetTitleInfo(TRUE);	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	フォーカスイベント（タイトル用）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::FocusTitlediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_TitleDiag.SetPosition(0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント（頁用）
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::TerminationPagediag1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	int nMaxDataCol = 0;
	// カレント頁の合計行の種別を取得
	int intTotalType = CmnGetControlTotalType();
	
	// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
	switch (intTotalType) {
	case ID_OUTKEI_BOTH:			// 頁計と累計
		nMaxDataCol = m_uInfo.intRowMaxData - 1;
		break;
	default:
		nMaxDataCol = m_uInfo.intRowMaxData;
		break;
	}

	// フォーカス移動
	switch (nChar) {
	case VK_RETURN:
		// Return：頁確定のため
		CmnDiagSetFocus(m_pDiag, 1, 1);
		break;
	case VK_TAB:
		// Tab/Shift+Tabにてフォーカス移動
		if (CmnCheckShiftKey() == FALSE) {
			// Tab：タイトルコントロールへ移動
// midori 160612 cor -->
			//m_TitleDiag.SetPosition(0);
// ---------------------
			if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
// midori 160612 cor <--
		}
		else {
			// Shift+Tab：データ部の最終行へ移動（頁計/累計行の制御を行う）
			//CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_051_KAMOKU);
			CmnDiagSetFocus(m_pDiag, nMaxDataCol, ID_COL_051_TEKIYOU);
		}
		break;
	case VK_DOWN:
		// タイトルコントロールへ移動
// midori 160612 cor -->
		//m_TitleDiag.SetPosition(0);
// ---------------------
		if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
// midori 160612 cor <--
		break;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	エディットオンイベント（頁用）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::EditONPagediag1(short index)
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Pagefocus = 1;
// midori 151405 add <--

	// 1行登録
//	if( CmnGetControlCol() != ID_COL_051_KIMATU ){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_PageDiag.SetPosition(1);
		}
//	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	エディットオフイベント（頁用）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::EditOFFPagediag1(short index)
{
// midori 155028 add -->
	// 会社切替時にメッセージを表示した場合、EditOffが走りDB書き込み時にエラーが発生していた
	if(m_EndView != FALSE)	return;
// midori 155028 add <--

	// 頁入力値を取得
	int	intTempPage = CmnDiagGetValue( &m_PageDiag, 1 );

// midori 151405 add -->
	m_Pagefocus = 0;
// midori 151405 add <--
	// 頁が変更された場合は画面更新が必要
	if ( m_uInfo.intCurPage != intTempPage ) {
		if ( ( intTempPage >= 1 ) && ( intTempPage <= m_uInfo.intMaxPage ) ) {
			// 範囲内：新ページに更新
			m_uInfo.intCurPage = intTempPage;

// 修正No.162446 del -->
			//// １頁の更新
			//virUpdateControlTblData();
			// フォーカスをデータ入力欄に
			////CmnDiagSetFocus( m_pDiag, 1, 1 );	// 修正No.162381,162382 del
			//PostMessage(WM_USER_FOCUSSET, 1, 1);	// 修正No.162381,162382 add
// 修正No.162446 del <--
// 修正No.162446 add -->
			m_ChangeFont = TRUE;

			// データを再描画してフォーカスをデータ入力欄に
			PostMessage(WM_USER_REDRAWVIEW, 0, 0);
// 修正No.162446 add <--

			return;
		}
	}

	// ページ情報のみ更新："0001"→"1"で表示したい
	UpdateControlPage();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	フォーカスイベント（頁用）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::FocusPagediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_PageDiag.SetPosition(1);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（棚卸方法登録） // 旧：欄外登録
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc051Tanaorosi::OnBnClickedMarginalButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	
	CdlgTanaoroshi		dlgTana( this );	//	欄外登録ダイアログクラス
	
	// 実行前の準備処理
	CmnEventStartProc(IDC_MARGINAL_BUTTON1);

	// 欄外登録関連のテーブルがオープン可かチェック
	if ( CheckTableMarginal() == TRUE ){
		//	欄外登録ダイアログ表示
// midori 160612 cor -->
//		dlgTana.ShowDialog( m_pDB, m_uInfo );
// ---------------------
		dlgTana.ShowDialog( m_pDB, m_uInfo, m_DataKakutei );
// midori 160612 cor <--
	}
}


#ifdef _DEBUG
void CfrmUc051Tanaorosi::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc051Tanaorosi::Dump(CDumpContext& dc) const
{
	CfrmUc000Common::Dump(dc);
}
#endif
#endif //_DEBUG

//**************************************************
//	初期処理
//	【引数】	なし
//	【戻値】	なし
//**************************************************
BOOL CfrmUc051Tanaorosi::virStartJob()
{
	//	共通データの初期値設定
	if ( CmnSetInitialUchiwakeInfo() != TRUE){	// 共通データ
		return FALSE;
	}

// midori UC_0038 add -->
	// 科目、銀行、取引先参照の番号が正しく連番になっているかチェック
	// 連番になっていない場合、修復処理を行う
	((CUCHIWAKEApp*)AfxGetApp())->OrderCheckEx(m_uInfo.intFormSeq);
// midori UC_0038 add <--

	//	帳表固有変数の初期値設定
	virSetInitialValue();

	// テーブルデータ無しなら１頁分のデータを作成
	CmnTblInitialMakeData();

	//	帳表タイトル情報の表示
	virUpdateControlTitle();

	// 頁の表示
	m_ChangeFont = TRUE;
	if ( virUpdateControlTblData() != TRUE ){
		return FALSE;
	}

	//	開始インデックスへ移動
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );

	// MainFrameからチェック状態を取得
//	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
//	m_chkYoushikiSkip.SetCheck( pFrame->GetMainYoushikiSkipMode() );

	// 様式送りボタンの制御
	UpdateYoushikiMoveButton( m_YoushikiSkip );

	return TRUE;
}

//**************************************************
//	共通データの初期値設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virSetInitialUchiwakeInfo()
{
	// 棚卸資産の固有データ
	m_uInfo.intRowMax = ID_ROWKOBETU_051;				//	１頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_051 - 1;		//	１頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_051;					//	１行内のコントロール個数	（預貯金=13）
	m_uInfo.intColMaxData = ID_COLMAXDATA_051;			//	１行内の列数				（預貯金=6）
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_051;	//	１行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_051;		//	合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_051;		//	合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_051_HINNMOKU;		// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_051_KAMOKU;		// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_051;					//	様式シーケンス番号
	m_uInfo.intFormNo = 5;								//	様式番号　　（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;								//	様式番号枝番（uc_inf_sub）
	
	m_uInfo.intRowNormal = ID_ROWNORMAL_051;			//	通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_051;			//	個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_051;	//	通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_051;	//	通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_051;	//	個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_051;	//	個別時：合計行②の先頭インデックス

	m_uInfo.intRowKei = ID_ROWKEI_051;				//	帳表固定の合計行数（1:合計あり，0:なし）
	m_uInfo.intKei1StartIndex = ID_COL_051_KEI1_MEISYO;	// 計ダイアグラム1行目の先頭インデックス
	m_uInfo.intKei2StartIndex = ID_COL_051_KEI2_MEISYO;	// 計ダイアグラム2行目の先頭インデックス


	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_051_MAX;		// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_051_FIELD1;		// 集計フィールド名称１

	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;			//	計設定ダイアログの選択可能なボタンの設定
}

//**************************************************
//	ICSDiagコントロールを通常／個別で切り替える
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virSetInitialDiag()
{	
	int		nIndex;

	// オブジェクト未生成時は処理を抜ける
	if ( (m_Diag1.m_hWnd == NULL) || (m_Diag2.m_hWnd == NULL) ){
		return;
	}

	//表示するダイアログの設定
	m_Diag		= CmnGetDiagType( &m_Diag1, &m_Diag2 );
	m_DiagKei	= CmnGetDiagType( &m_DiagKei1, &m_DiagKei2 );

	// 親クラスのダイアログにコピー
	m_pDiag		= m_Diag;
	m_pDiagKei	= m_DiagKei;

	// 入力不可行でのDeleteキー無効
	m_pDiag->EnableDelete(0);

	//	コンボボックスの属性取得
	nIndex = CmnGetControlIndex( 1, ID_COL_051_KAMOKU );
	m_pDiag->GetAttribute( nIndex, ( LPUNKNOWN )&m_uInfo.uComboAttr );
}

//**************************************************
//	帳表固有変数の初期値設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virSetInitialValue()
{
	//	ローカルの1ページ分のレコードバッファを初期化する
	RecBufClearAllData();
	// 行コピー用のローカルバッファをクリア
	RecBufClearData( &m_CopyData );
	//	コピーフラグ初期化
	m_nCopyFlag = 0;
	return;
}

//**************************************************
//	1レコード分のデータを初期化する
//	【引数】	pudRec			…	1レコード分の構造体
//	【戻値】	FUNCTION_OK		…	成功
//**************************************************
int CfrmUc051Tanaorosi::RecBufClearData( P_REC_UC_051_TANAOROSI pudRec )
{
	pudRec->m_Seq		= 0;				//	シーケンス番号	
	pudRec->m_NumPage	= 0;				//	頁番号
	pudRec->m_NumRow	= 0;				//	行番号
	pudRec->m_FgFunc	= 0;				//	特殊行フラグ
	pudRec->m_FgShow	= 0;				//	表示フラグ
	pudRec->m_KeiStr	= _T("");			//	計
	pudRec->m_NumGroup	= 0;				//	グループ番号

	pudRec->m_KnSeq		= 0;				//	科目（シーケンス番号）
	pudRec->m_KnOrder	= 0;				//	科目（並び順）
	pudRec->m_KnName	= _T("");			//	科目（名称）
	pudRec->m_HdName	= _T("");			//	品目
	pudRec->m_Suuryou	= _T("");			//	数量
	pudRec->m_Tanni		= _T("");			//	単位
	pudRec->m_Price		= _T("");			//	単価
	pudRec->m_Val		= _T("");			//	期末現在高
	pudRec->m_Teki		= _T("");			//	摘要
	pudRec->m_FgInput	= ID_AUTOCALC_AUTO;	//	自動計算フラグ
	pudRec->m_ShowKeiZero = 0;				//０円計表示フラグ
// midori 152137 add -->
	pudRec->m_KnKana	= _T("");			// 科目
// midori 152137 add <--

	//	戻値を返す
	return( FUNCTION_OK );
}

//**************************************************
//	ローカルの1ページ分のレコードバッファを初期化する
//	【引数】	なし
//	【戻値】	FUNCTION_OK		…	成功
//**************************************************
int CfrmUc051Tanaorosi::RecBufClearAllData()
{
	// 1ページ分のレコードバッファを初期化する
	// 1行目から、最終行(計行も含む)までクリア
	// 配列の添え字は、行番号をそのまま使用
	// 使用していないが配列[0]も初期化している
	for (int trgRow = 0; trgRow <= m_uInfo.intRowMax; trgRow++) {
		//	1レコード分のデータを初期化する
		RecBufClearData( &m_ReadData[trgRow] );
	}
	//	戻値を返す
	return( FUNCTION_OK );
}

//**************************************************
//	帳表タイトル情報の表示
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virUpdateControlTitle()
{
	DIAGRAM_ATTRIBUTE diaatt;

	// 帳表ナンバーの背景色設定
	CmnDiagSetAttrColor(&m_NumberDiag, 0, &m_ViewColor );

	// 帳表ナンバー表示
	CmnDiagSetString(&m_NumberDiag, 0, m_uInfo.strTitleNumber, 1);
	// 帳表タイトル表示
	CmnDiagSetString(&m_TitleDiag, 0, m_uInfo.strTitleName, 0);

/*
	// 帳表タイトル編集可能/不可
	BOOL bEnable = CmnCheckEnableTitleEdit();
	GetDlgItem(IDC_TITLEDIAG1)->EnableWindow(bEnable);
*/

	//	タイトル入力最大文字数設定
	CmnDiagSetAttrInpmax( &m_TitleDiag, 0, ID_TITLE_INPMAX );
	
	long	nMode;		//	入力不可モードフラグ

/*
	// 帳表タイトル編集可能/不可
	if ( CmnCheckEnableTitleEdit() == TRUE ){
		nMode = DIAG_MDFY_INPUT;	//	入力可
	} else {
		nMode = DIAG_MDFY_NOINPUT;	//	入力不可
	}
*/
	// 全ての様式において、編集可能
	nMode = DIAG_MDFY_INPUT;	//	入力可

	//	入力不可モード切り替え
	m_TitleDiag.ModifyItem( 0, nMode );

	// 帳表タイトル_半角/全角設定
	m_TitleDiag.GetAttribute( 0 , (LPUNKNOWN)&diaatt );
	diaatt.attr_editattr = diaatt.attr_editattr | DIAG_EATTR_ZENHAN;
	m_TitleDiag.SetAttribute( 0 , (LPUNKNOWN)&diaatt , TRUE );

// midori 161108 add -->
	// 保管ﾃﾞｰﾀ件数の背景色設定
	CmnDiagSetAttrColor(&m_SaveDataDiag, 0, &m_ViewColor);
// midori 161108 add <--

// midori 190301 add -->
	// ---------------------------------------------------
	// 棚卸方法登録ボタン（新様式：非表示、旧様式：表示）
	// ---------------------------------------------------
	if(bG_Kanso == TRUE)	{
		m_btnMarjinal.ShowWindow(SW_HIDE);
		m_btnMarjinal.EnableWindow(FALSE);
	}
	else	{
		m_btnMarjinal.ShowWindow(SW_SHOW);
		m_btnMarjinal.EnableWindow(TRUE);
	}
// midori 190301 add <--

// 修正No.158252 del -->
//// 修正No.157626 add -->
//	// 欄外ガイドの背景色
//	m_Guide1.SetBackColor(m_ViewColor);
//	m_Guide1.ShowWindow(FALSE);
//// 修正No.157626 add <--
// 修正No.158252 del <--
}

//**************************************************
//	帳表データの表示（１ページ分）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
BOOL CfrmUc051Tanaorosi::virUpdateControlTblData()
{
// midori 18-0887 del -->
//// midori 161108_2 add -->
//	int					hcnt=0;
//	CString				cs=_T("");
//// midori 161108_2 add <--
// midori 18-0887 del <--
// midori 18-0887 add -->
	int					inpsw=0;
	int					nCtlIndex=0;
	CString				szNow=_T("");
	CString				szTmp1=_T("");
	CString				szTmp2=_T("");
	CString				szTmp3=_T("");
// midori 18-0887 add <--

	// ダイアログ切替
	if( m_pDiag != NULL ){
		virSetInitialDiag();
	}

	CdbUc051Tanaoroshi	mfcRec( m_pDB );

	m_pDiag->DeleteInput();					//	入力ボックスの削除

	// 頁情報の表示
	UpdateControlPage();

	// コントロールのクリア＋初期設定
	if ( InitialControlDataArea() != TRUE ){	// データ行
		return FALSE;
	}
// midori 160612 del -->
	//InitialControlTotalArea();				// 頁計/累計行
	//m_pDiag->Refresh();						// 更新
// midori 160612 del <--

	//----------------------- １行データ表示 ------------------------------//

	// 倍長設定
	l_defn(0x16);

	// 指定ページ番号のデータを取得
	mfcRec.Init( m_uInfo.intCurPage );

	// 1ページ分のローカルバッファをクリア
	RecBufClearAllData();

	// 1ページ分のデータ1レコードづつ取得し、画面に表示
	while( !mfcRec.IsEOF()){		// ファイル終端まで
		// 1ページ分のローカルバッファにデータを格納
		RecBufSetData( &mfcRec );
		
		// 対象行金額欄の桁数を変更
		InitialControlSpecialRow( mfcRec.m_NumRow, mfcRec.m_FgFunc );

		// 累計/頁計の場合、特殊処理
		if ( ( mfcRec.m_FgFunc == ID_FGFUNC_RUIKEI ) || ( mfcRec.m_FgFunc == ID_FGFUNC_PAGEKEI ) ){
			// 累計/頁計の表示
			UpdateControlRowTotal( &mfcRec );
		}
		else{
			// データ行の表示（空行，特殊行を含む）
			UpdateControlRowData( &mfcRec );
		}

// midori 18-0887 add -->
		//	自動計算結果と期末現在高が同じ場合でも背景色が緑の場合はここで自動計算フラグをＯＦＦにする
		if((mfcRec.m_FgFunc == ID_FGFUNC_DATA) || (mfcRec.m_FgFunc == ID_FGFUNC_IKKATUMANUAL))	{
			//	期末現在高の入力値取得
			nCtlIndex = CmnGetControlIndex(mfcRec.m_NumRow,ID_COL_051_KIMATU);
			szNow = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	数量の入力値取得
			nCtlIndex = CmnGetControlIndex(mfcRec.m_NumRow,ID_COL_051_SUURYOU);
			szTmp1 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	単価の入力値取得
			nCtlIndex = CmnGetControlIndex(mfcRec.m_NumRow,ID_COL_051_TANNKA);
			szTmp2 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	数量×単価を計算（四捨五入）
// midori UC_0025 del -->
			//szTmp3 = GetMulVal(szTmp1,szTmp2);
// midori UC_0025 del <--
// midori UC_0025 add -->
			if(szTmp1 == "" || szTmp2 == "")	{
				szTmp3 = "";
			}
			else	{
				szTmp3 = GetMulVal(szTmp1,szTmp2);
			}
// midori UC_0025 add <--
			//	自動計算した結果と現在の入力値が違う？
			if(szNow != szTmp3 && szNow.IsEmpty() != TRUE)	{
				inpsw = ID_AUTOCALC_INPUT;
			}
			else	{
				inpsw = ID_AUTOCALC_AUTO;
			}
			if(inpsw != mfcRec.m_FgInput)	{
				// 自動計算をフラグを書き換える
				mfcRec.Edit();
				mfcRec.m_FgInput = inpsw;
				mfcRec.Update();
			}
		}
// midori 18-0887 add <--

		//	帳表コントロールの自動計算セル初期化
		virInitialControlFgInputCell( mfcRec.m_NumRow, mfcRec.m_FgFunc, mfcRec.m_FgInput );

// No.200601 add -->
		// 簡素化様式のみ
		if(bG_Kanso == TRUE) {
			// 電子申告の文字数を超えていれば、項目の文字色を変える
			// 科目
			CmnInitialControlFgStringCell( m_pDiag, mfcRec.m_NumRow, ID_COL_051_KAMOKU, mfcRec.m_FgFunc, UC_KAMOKU_LEN, mfcRec.m_KnName);
			// 品目
			CmnInitialControlFgStringCell( m_pDiag, mfcRec.m_NumRow, ID_COL_051_HINNMOKU, mfcRec.m_FgFunc, UC_051_HINMOKU_LEN, mfcRec.m_HdName);
			// 摘要
			CmnInitialControlFgStringCell( m_pDiag, mfcRec.m_NumRow, ID_COL_051_TEKIYOU, mfcRec.m_FgFunc, UC_051_TEKI_LEN, mfcRec.m_Teki);
		}
// No.200601 add <--

		//	次のレコードに移動
		mfcRec.MoveNext();
	}

	// 閉じる
	mfcRec.Fin();

// midori 160612 add -->
	InitialControlTotalArea();				// 頁計/累計行
// midori 160612 add <--

// midori 161108 add -->
	// 保管ﾃﾞｰﾀ件数の表示
	virUpdateControlTblData2();
// midori 161108 add <--

// No.200601 add -->
	// 簡素化様式のみ
	if(bG_Kanso == TRUE) {
		TitleColorChg();
	}
// No.200601 add <--

	// 画面更新
	m_pDiag->Refresh();
	m_pDiagKei->Refresh();

	return TRUE;
}

// midori 161108 add -->
//********************************************************************************
//	保管件数の表示
//********************************************************************************
BOOL CfrmUc051Tanaorosi::virUpdateControlTblData2()
{
	int					hcnt=0;
	CString				cs=_T("");

	hcnt=virGetDataTableObject()->GetCountFgShow(ID_FGSHOW_HOKAN);
	cs.Format(_T("保管%3d件"),hcnt);
	CmnDiagSetString(&m_SaveDataDiag,0,cs,1);

	return TRUE;
}
// midori 161108 add <--

//**************************************************
//	帳表ページ情報の表示
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::UpdateControlPage()
{
	BOOL			bBack = TRUE;
	BOOL			bNext = TRUE;

	// 最大頁取得
	m_uInfo.intMaxPage = CmnTblGetPageMax();

	// カレント頁番号のチェック
	if (m_uInfo.intCurPage > m_uInfo.intMaxPage) {
		m_uInfo.intCurPage = m_uInfo.intMaxPage;
	}

	// カレント頁/最大頁表示
	CmnDiagSetValue(&m_PageDiag, 1, m_uInfo.intCurPage, 0);
	CmnDiagSetValue(&m_PageDiag, 3, m_uInfo.intMaxPage, 1);

	// 前頁/次頁ボタンの制御
	CmnCheckEnableMoveButton(&bBack, &bNext);
// midori 155879 del -->
//	GetDlgItem(IDC_PAGEBACK_BUTTON1)->EnableWindow(bBack);		// 前頁ボタン
//	GetDlgItem(IDC_PAGENEXT_BUTTON1)->EnableWindow(bNext);		// 次頁ボタン
// midori 155879 del <--
// midori 155879 add -->
	PostMessage(WM_USER_CONTROLPAGE, bBack, bNext);
// midori 155879 add <--
}

//**************************************************
//	帳表コントロールの初期化（データ領域）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
BOOL CfrmUc051Tanaorosi::InitialControlDataArea()
{
	int			intMax;
	int			i;
// 修正No.157647 add -->
	int			len=0;
// 修正No.157647 add <--

	// 全データクリア
	m_pDiag->AllClear();
	m_pDiagKei->AllClear();

	// 各Diagに背景色を設定
	SetDiagBackColor();

	// 背景色クリア
	CmnDiagSetAttrColorAllClear(m_pDiag);

	// 列の属性により更にクリアと初期設定が必要
	intMax = m_uInfo.intT1StartIndex;
	for ( i = m_uInfo.intCtlStartIndex; i < intMax; i += m_uInfo.intColMax ) {
		// ComboBox：クリア
		CmnDiagSetString( m_pDiag, i + ID_COL_051_KAMOKU - 1, "", 0 );
		
		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_051_HINNMOKU    - 1, MAX_KETA_051_HINNMOKU );		//	品目
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_051_TANNI   - 1, MAX_KETA_051_TANNI );			//	単位
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_051_TEKIYOU    - 1, MAX_KETA_051_TEKIYOU );		//	摘要

// 修正No.157647 del -->
		//// 金額欄：桁数初期化（12桁）
		//CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_051_KIMATU - 1, ID_VAL_051_DATA );			//	期末現在高
// 修正No.157647 del <--
// 修正No.157647 add -->
		// 金額欄：桁数初期化
		if(bG_Kanso == TRUE)	len = ID_VAL_051_K_DATA;
		else					len = ID_VAL_051_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_051_KIMATU - 1, len );						//	期末現在高
// 修正No.157647 add <--
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_051_SUURYOU - 1 , 7, TRUE, TRUE, 2 );		//	数量
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_051_TANNKA - 1 , 8, TRUE, TRUE, 2 );		//	単価

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	// ComboBoxの初期設定
	if ( InitialControlComboBox() != TRUE ){
		return FALSE;
	}

	// 半角/全角設定
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_051_HINNMOKU );		//	品目
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_051_TEKIYOU );		//	摘要
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_051_TANNI );			//	単位

	return TRUE;
}

//**************************************************
//	帳表コントロールの初期化（ComboBox）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
BOOL CfrmUc051Tanaorosi::InitialControlComboBox()
{
	DIAGRAM_DATA	diadata;			// ICSDIAG構造体
	int				intTotalType;
	int				intMax;
	int				intIndex;
	int				i;
	int				nFontSize = 100;
	BOOL			bFlg = FALSE;	// フォントサイズ変更する？

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();
	
	// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
	switch (intTotalType) {
	case ID_OUTKEI_BOTH:			// 頁計と累計
		intMax = m_uInfo.intRowMaxData - 1;
		break;
	default:
		intMax = m_uInfo.intRowMaxData;
		break;
	}

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// ComboBoxの選択項目取得
	if ( CmnUcLstKamokuGetComboList( &diadata.text_combo, bFlg ) != TRUE ){
		return FALSE;
	}

	// 全行設定
	for (i = 1; i <= intMax; i++) {
		// 対象コントロールのインデックス番号を取得
		intIndex = CmnGetControlIndex(i, ID_COL_051_KAMOKU);

		// 初期設定（ドロップダウンON＋リスト情報をセット）
		m_pDiag->ModifyItem(intIndex, DIAG_MDFY_NODROPDOWN);
		m_pDiag->SetData(intIndex, (LPUNKNOWN)&diadata);

		// フォントサイズ設定
		if( m_EndView == FALSE && m_ChangeFont != FALSE && m_ImportMode == FALSE ){
			SetComboFontSize( m_pDiag, intIndex, bFlg );
		}
	}
	m_ChangeFont = FALSE;

	return TRUE;
}

//**************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		文言はデータに含まれているので、その内容を使用する
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::InitialControlTotalArea()
{
	int		nKeiType;			//	計の種別
	int		nDataRow;			//	データ行属性
	int		nDataRowPos;		//	データ行位置
	int		nKeiRow[2];			//	計行属性
	int		nCnt;				//	カウント
	int		nStart;				//	開始位置
// midori 152166 add -->
	int		intIndex=0;
	int		nSw=0;
// 修正No.157898 add -->
	int		flg2=0;
// 修正No.157898 add <--

	intIndex = CmnGetControlIndex(m_uInfo.intCurRow,m_uInfo.intCurCol);
// midori 152166 add <--

	// カレント頁の合計行の種別を取得
	nKeiType = CmnGetControlTotalType();
	
	nDataRowPos = m_uInfo.intRowMax - 2;

	//	計の種別で分岐
	switch ( nKeiType ){
	case ID_OUTKEI_OFF:				// なし
	case ID_OUTKEI_PAGEKEI:			// 頁計
	case ID_OUTKEI_RUIKEI:			// 累計
		//	データ行の設定
// midori 160612 cor -->
		//nDataRow = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
// ---------------------
// midori 152364 del -->
		//if(m_DataKakutei == FALSE)	nDataRow = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
// midori 152364 del <--
// midori 152364 add -->
		if(m_DataKakutei == FALSE)	{
			if(IsSpecialRow(m_ReadData[nDataRowPos+1].m_FgFunc) != 0)	{
				nDataRow = (DIAG_MDFY_NOINPUT | DIAG_MDFY_OPAQUE);
// 修正No.157898 add -->
				if(IsSpecialRow2(m_ReadData[nDataRowPos+1].m_FgFunc) != 0)	{
					flg2 = 1;
				}
// 修正No.157898 add <--
			}
			else	{
				nDataRow = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
			}
		}
// midori 152364 add <--
		else						nDataRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
// midori 160612 cor <--
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		//nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );

		// 28行目の合計行①
		nStart = m_uInfo.intT1StartIndex;
		for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
			m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[0] );
		}
		break;
	case ID_OUTKEI_BOTH:			// 頁計と累計
		//	データ行の設定
		nDataRow = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		//nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		break;
	}

	// 28行目のデータ行
	nStart = nDataRowPos * m_uInfo.intColMax;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++){
		m_pDiag->ModifyItem( nStart + nCnt, nDataRow );
// midori 152166 add -->
		if((intIndex == (nStart + nCnt)) && ((nDataRow & DIAG_MDFY_READONLY) != 0))	nSw=1;
// midori 152166 add <--
	}

// 修正No.157898 add -->
	if(flg2 == 1)	{
		// 一括自動行の科目を入力可能にする
		m_pDiag->ModifyItem( CmnGetControlIndex( (nDataRowPos+1), ID_COL_051_KAMOKU ), (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE) );
	}
// 修正No.157898 add <--

// midori 152166 add -->
	if(nSw != 0)	{
		// 現在のカーソルポジションが入力不可になった場合は先頭に戻す
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
	}
// midori 152166 add <--

	// 計専用ダイアグラムの金額欄を入力不可にする
	int intKeiRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
	m_pDiagKei->ModifyItem(ID_COL_051_KEI1_KIMATU, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_051_KEI2_KIMATU, intKeiRow);

	//// 28行目の合計行①
	//nStart = m_uInfo.intT1StartIndex;
	//for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
	//	m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[0] );
	//}
	//// 29行目の合計行②
	//nStart = m_uInfo.intT2StartIndex;
	//for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
	//	m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[1] );
	//}

	// 合計行の名称欄は、カーソルを飛ばしたいので入力許可する
	//switch ( nKeiType ) {
	//case ID_OUTKEI_OFF:
	//case ID_OUTKEI_PAGEKEI:
	//case ID_OUTKEI_RUIKEI:
	//	nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT | DIAG_MDFY_NOINPUT );
	//	nKeiRow[1] = ( DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE | DIAG_MDFY_NOINPUT );
	//	break;
	//case ID_OUTKEI_BOTH:
	//	nKeiRow[0] = ( DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE | DIAG_MDFY_NOINPUT );
	//	nKeiRow[1] = ( DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE | DIAG_MDFY_NOINPUT );
	//	break;
	//}
	
	// 入力可能（ただし、画面レイアウトで入力文字数"0"としているため、実際は入力できない）
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 1, ID_COL_051_KAMOKU ), nKeiRow[0] );
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 2, ID_COL_051_KAMOKU ), nKeiRow[1] );
	
	// 合計行の金額欄は、金額欄 + 1桁
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_051_KEI1_KIMATU, ID_VAL_051_TOTAL );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_051_KEI2_KIMATU, ID_VAL_051_TOTAL );
	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_051_KIMATU ), ID_VAL_051_TOTAL );
	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_051_KIMATU ), ID_VAL_051_TOTAL );
}

//**************************************************
//	ローカルのレコードバッファに、1行分のデータを格納する
//	【引数】	pmfcRec			…	レコードセット
//	【戻値】	FUNCTION_OK		…	成功
//**************************************************
int CfrmUc051Tanaorosi::RecBufSetData(CdbUc051Tanaoroshi* pmfcRec)
{
	// 1ページ分のレコードバッファの該当行にデータを格納する
	int	trgRow = pmfcRec->m_NumRow;	
	m_ReadData[trgRow].m_Seq		= pmfcRec->m_Seq;
	m_ReadData[trgRow].m_NumPage	= pmfcRec->m_NumPage;
	m_ReadData[trgRow].m_NumRow		= pmfcRec->m_NumRow;
	m_ReadData[trgRow].m_FgFunc		= pmfcRec->m_FgFunc;
	m_ReadData[trgRow].m_FgShow		= pmfcRec->m_FgShow;
	m_ReadData[trgRow].m_KeiStr		= pmfcRec->m_KeiStr;
	m_ReadData[trgRow].m_NumGroup	= pmfcRec->m_NumGroup;

	m_ReadData[trgRow].m_KnSeq		= pmfcRec->m_KnSeq;
	m_ReadData[trgRow].m_KnOrder	= pmfcRec->m_KnOrder;
	m_ReadData[trgRow].m_KnName		= pmfcRec->m_KnName;
	m_ReadData[trgRow].m_HdName		= pmfcRec->m_HdName;
	m_ReadData[trgRow].m_Suuryou	= pmfcRec->m_Suuryou;
	m_ReadData[trgRow].m_Tanni		= pmfcRec->m_Tanni;
	m_ReadData[trgRow].m_Price		= pmfcRec->m_Price;
	m_ReadData[trgRow].m_Val		= pmfcRec->m_Val;
	m_ReadData[trgRow].m_Teki		= pmfcRec->m_Teki;
	m_ReadData[trgRow].m_FgInput	= pmfcRec->m_FgInput;
// midori 152137 add -->
	m_ReadData[trgRow].m_KnKana		= pmfcRec->m_KnKana;	// 科目
// midori 152137 add <--
	
	//	戻値を返す
	return( FUNCTION_OK );
}

//**************************************************
//	帳表コントロールの特殊行初期化
//	【引数】	intRow		…	行番号
//				intFgFunc	…	特殊行フラグ（FgFunc）
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::InitialControlSpecialRow(int intRow, int intFgFunc)
{
	DIAGRAM_DATA	diadata;			// ICSDIAG構造体

	// 対象行の色を変更
	CmnDiagSetAttrSpecialRow(m_pDiag, intRow, intFgFunc);

	// 特殊行の金額欄は桁数変更 ･･･ 頁計/累計行は初期設定するのでここでは対象外
	switch (intFgFunc) {
	case ID_FGFUNC_SYOKEI:				// 小計
	case ID_FGFUNC_SYOKEINULL:			// 小計（空行）
	case ID_FGFUNC_CHUKEI:				// 中計
	case ID_FGFUNC_CHUKEINULL:			// 中計（空行）
	case ID_FGFUNC_IKKATUMANUAL:		// 一括集計金額行（手動）
	case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
		// 13桁を設定
		CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow , ID_COL_051_KIMATU), ID_VAL_051_TOTAL);
		break;
	}

	// 特殊行は入力不可とする
	switch (intFgFunc) {
	case ID_FGFUNC_SYOKEI:				// 小計
	case ID_FGFUNC_SYOKEINULL:			// 小計（空行）
	case ID_FGFUNC_CHUKEI:				// 中計
	case ID_FGFUNC_CHUKEINULL:			// 中計（空行）
	case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
		
		//	// 一括集計金額行（自動）以外？
		if (intFgFunc != ID_FGFUNC_IKKATUAUTO) {
			// 種類はComboBoxのため、DropDown不可とする
			m_pDiag->ModifyItem(CmnGetControlIndex(intRow, ID_COL_051_KAMOKU), DIAG_MDFY_NODROPDOWN);

			// ComboBoxの選択項目クリア
			m_clsFunc.DiagInit(&diadata);
			diadata.text_combo = _T("\r\n");
			m_pDiag->SetData(CmnGetControlIndex(intRow, ID_COL_051_KAMOKU), (LPUNKNOWN)&diadata);
		}

		//	入力不可モードの設定
// midori 160612 cor -->
		//SetInputMode( intRow, DIAG_MDFY_NOINPUT );
// ---------------------
		if(m_DataKakutei == FALSE)	SetInputMode(intRow,DIAG_MDFY_NOINPUT);
		else						SetInputMode(intRow,DIAG_MDFY_READONLY);
// midori 160612 cor <--

// No.200905 add -->
// 改良No.21-0086,21-0529 cor -->
		//if(KamokuRowEnableSgn(m_pDB,1) == 1)	{
// ------------------------------
		if(KamokuRowEnableSgn(m_pDB, 1, m_uInfo.intFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
			// 一括集計金額（自動）行の場合、科目のみ入力可能とする
			if (intFgFunc == ID_FGFUNC_IKKATUAUTO ){
				//	科目
				m_pDiag->ModifyItem( CmnGetControlIndex( intRow, ID_COL_051_KAMOKU ), DIAG_MDFY_INPUT );
			}
		}
// No.200905 add <--

		break;
	
	case ID_FGFUNC_RUIKEI:		//	頁計
	case ID_FGFUNC_PAGEKEI:		//	累計
		//	何もしない
		break;
	}
}

//**************************************************
//	帳表コントロールの自動計算セル初期化
//	【引数】	nRow		…	行番号
//				nFgFunc		…	特殊行フラグ
//				bFgInput	…	自動計算フラグ
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virInitialControlFgInputCell( int nRow, int nFgFunc, BYTE bFgInput )
{	
	int		nRowFirst;	//	行開始位置
	int		nBackPos;	//	背景位置
	int		nDataPos;	//	データ位置

	//	位置計算
	nRowFirst = ( nRow - 1 ) * m_uInfo.intColMax;
	nBackPos = nRowFirst + 4;
	nDataPos = nRowFirst + ( m_uInfo.intColMax - m_uInfo.intColMaxData ) + ( ID_COL_051_KIMATU - 1 );

	//	自動計算対象セルの属性設定を行う
	CmnDiagSetAttrFgInputCell( m_pDiag, nBackPos, nDataPos, nFgFunc, bFgInput );
}

//**************************************************
//	帳表データの１行表示（累計，頁計行）
//	【引数】	mfcRec		…	レコードセット
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::UpdateControlRowTotal( CdbUc051Tanaoroshi* mfcRec )
{
	int				intTotalType;
	int				intRowName = 0;			// 名称表示行（0:非表示）
	int				intRowKingaku = 0;		// 金額表示行（0:非表示）

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();

	switch (intTotalType) {
	case ID_OUTKEI_OFF:
		// カレント頁："なし"なら、合計行②に名称のみ表示
		if ((mfcRec->m_FgFunc == ID_FGFUNC_PAGEKEI) || (mfcRec->m_FgFunc == ID_FGFUNC_RUIKEI)) {
			intRowName = 2;
		}
		break;
	case ID_OUTKEI_PAGEKEI:
		// カレント頁："頁計"なら、合計行②に"頁計"を表示
		if (mfcRec->m_FgFunc == ID_FGFUNC_PAGEKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	case ID_OUTKEI_RUIKEI:
		// カレント頁："累計"なら、合計行②に"累計"を表示
		if (mfcRec->m_FgFunc == ID_FGFUNC_RUIKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	case ID_OUTKEI_BOTH:
		// カレント頁："頁計＋累計"なら、合計行①に"頁計"＋合計行②に"累計"を表示
		if (mfcRec->m_FgFunc == ID_FGFUNC_PAGEKEI) {
			intRowName = 1;
			intRowKingaku = 1;
		}
		else if (mfcRec->m_FgFunc == ID_FGFUNC_RUIKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	}

	// 頁計/累計の名称表示
	if (intRowName > 0) {
		//CmnDiagSetString(m_pDiag, CmnGetControlTotalIndex(intRowName, ID_COL_051_KAMOKU), mfcRec->m_KeiStr, 0);
		CmnDiagSetString( m_pDiagKei, CmnGetKeiDiagIndex( intRowName, ID_COL_051_KEI1_MEISYO ), mfcRec->m_KeiStr, 1 );
	}
	// 頁計/累計の金額表示
	if (intRowKingaku > 0) {
		//CmnDiagSetKingaku(m_pDiag, CmnGetControlTotalIndex(intRowKingaku, ID_COL_051_KIMATU), mfcRec->m_Val);
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowKingaku, ID_COL_051_KEI1_KIMATU ), mfcRec->m_Val );
	}
}

//**************************************************
//	帳表データの１行表示（空行，データ行，特殊行）
//	【引数】	mfcRec		…	レコードセット
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::UpdateControlRowData( CdbUc051Tanaoroshi* mfcRec )
{

	DIAGRAM_DATA		diadata;		// ICSDiag構造体
	int					intCell;		// 保存フィールド

	// 初期化
	m_clsFunc.DiagInit( &diadata );
	// 読込み対象セル取得
	intCell = CmnGetControlIndex( mfcRec->m_NumRow, ID_COL_051_KAMOKU );

	// 科目
	//	データ行？
	switch( mfcRec->m_FgFunc ){
	//------------------------------
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:
	//	一括金額（自動）
	case ID_FGFUNC_IKKATUAUTO:
	//------------------------------
		//	コンボの選択インデックス
// midori 152137 del -->
//		diadata.data_combo = (short)mfcRec->m_KnOrder;
// midori 152137 del <--
// midori 152137 add -->
		// KnOrder（科目順序）をキーにコンボボックスのインデックスを取得
		m_SortMap1.Lookup(mfcRec->m_KnOrder,diadata.data_combo);
// midori 152137 add <--
		break;
	//------------------------------
	//	小計
	case ID_FGFUNC_SYOKEI:
	//	小計（空行）
	case ID_FGFUNC_SYOKEINULL:
	//	中計
	case ID_FGFUNC_CHUKEI:
	//	中計計（空行）
	case ID_FGFUNC_CHUKEINULL:
	//------------------------------
		diadata.text_combo = mfcRec->m_KeiStr;
		break;
	default:
		break;
	}
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	//	品目
	//	データ行？
	switch( mfcRec->m_FgFunc ){
	//------------------------------
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:
	//	一括金額（自動）
	case ID_FGFUNC_IKKATUAUTO:
	//------------------------------
		diadata.data_edit = mfcRec->m_HdName;
		break;
	default:
		diadata.data_edit.Empty();
		break;
	}
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 数量
	if ( mfcRec->m_Suuryou != "" ){
		CmnChangeFieldValData( &diadata.data_val[0] , mfcRec->m_Suuryou );
		//diag_setdata( IDC_YOUSHIKIDIAG1,intCell ,&diadata, CICSDiag);
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else{
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 単位
	diadata.data_edit = mfcRec->m_Tanni; 
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;
	// 単価
	if ( mfcRec->m_Price != "" ){
		CmnChangeFieldValData( &diadata.data_val[0] , mfcRec->m_Price );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else{
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;
	// 金額
	if ( mfcRec->m_Val != "" ){
		CmnChangeFieldValData( &diadata.data_val[0] , mfcRec->m_Val );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else{
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;
	// 摘要
	diadata.data_edit = mfcRec->m_Teki;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
}

//**************************************************
//	預貯金テーブルのオブジェクトを生成し、ポインタを返す	
//	【引数】	なし
//	【戻値】	なし
//**************************************************
CdbUc000Common* CfrmUc051Tanaorosi::virGetDataTableObject()
{
	// 預貯金テーブルのオブジェクトを生成し、ポインタを返す	
	return (CdbUc000Common*)(new CdbUc051Tanaoroshi(m_pDB));
}

//**************************************************
//	指定された行のデータをテーブルに保存
//	【引数】	inPage				…	登録するレコードのページ番号
//				inRow				…	登録するレコードの行番号
//	【戻値】	FUNCTION_UPDATE		…	成功(更新あり)
//				FUNCTION_OK			…	成功(更新なし)
//				FUNCTION_NG			…	失敗
//**************************************************
int CfrmUc051Tanaorosi::virRecBufSaveData(int inPage, int inRow)
{
	REC_UC_051_TANAOROSI tmpSaveData;	// 画面データ1レコード格納用

	RecBufClearData( &tmpSaveData );	// レコードを初期化

	// 頁計/累計行は登録しない
	if ((m_ReadData[inRow].m_FgFunc == ID_FGFUNC_PAGEKEI) || (m_ReadData[inRow].m_FgFunc == ID_FGFUNC_RUIKEI)) {
		return FUNCTION_OK;
	}

	///////////////////////////////////////////////////////////////
	// コンポーネント(画面上の1レコード分の情報)を仮変数へ格納
	DIAGRAM_DATA	diadata;			// ICSDiag構造体
	int				intIndex;			// 対象行の各インデックス
	
	// 初期化
	m_clsFunc.DiagInit( &diadata );

	//	科目
	intIndex = CmnGetControlIndex( inRow , ID_COL_051_KAMOKU );
	m_pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
// midori 152137 del -->
//	tmpSaveData.m_KnOrder = diadata.data_combo;
// midori 152137 del <--
// midori 152137 add -->
	// コンボボックスの値をキーにKnOrder（科目順序）を取得
	m_SortMap2.Lookup(diadata.data_combo,tmpSaveData.m_KnOrder);
// midori 152137 add <--

	// 品目
	intIndex = CmnGetControlIndex( inRow , ID_COL_051_HINNMOKU );
	tmpSaveData.m_HdName = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 数量
	intIndex = CmnGetControlIndex( inRow , ID_COL_051_SUURYOU );
	tmpSaveData.m_Suuryou = CmnDiagGetKingaku( m_pDiag, intIndex );
	
	// 単位
	intIndex = CmnGetControlIndex( inRow , ID_COL_051_TANNI );
	tmpSaveData.m_Tanni = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 単価
	intIndex = CmnGetControlIndex( inRow , ID_COL_051_TANNKA );
	tmpSaveData.m_Price = CmnDiagGetKingaku( m_pDiag, intIndex );
	
	// 期末現在高格納
	tmpSaveData.m_Val = GetAutoCalc( inRow );
	
	//	自動計算フラグ
	tmpSaveData.m_FgInput = m_ReadData[inRow].m_FgInput;

	// 摘要格納
	intIndex = CmnGetControlIndex( inRow , ID_COL_051_TEKIYOU );
	tmpSaveData.m_Teki = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	int	intRet = RecBufSaveDataSub( inPage, inRow, &tmpSaveData );

	// 行登録した場合、必ず計再計算と1頁再表示を行う
	if (intRet == FUNCTION_UPDATE) {
		// 計再計算
		CmnTblCalcKei();

		// 1頁再表示
		virUpdateControlTblData();
	}

	return intRet;
}

//**************************************************
//	指定された行のデータをテーブルに保存(補助ルーチン)
//  ※ShowKeiZeroはUpdateCalcKeiで保存している
//	【引数】	inPage				…	登録するレコードのページ番号
//				inRow				…	登録するレコードの行番号
//				inSaveData			…	登録するデータレコードへのポインタ
//	【戻値】	FUNCTION_UPDATE		…	成功(更新あり)
//				FUNCTION_OK			…	成功(更新なし)
//				FUNCTION_NG			…	失敗
//**************************************************
int CfrmUc051Tanaorosi::RecBufSaveDataSub(int inPage, int inRow, P_REC_UC_051_TANAOROSI inSaveData)
{
	REC_UC_051_TANAOROSI	udTemp;		//	画面上のデータ格納用
	
	//	画面上のデータ格納用初期化
	RecBufClearData( &udTemp );

	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL	ufgUpData	= FALSE;		// 変更確認フラグ(全体用データ行/空行)
	BOOL	ufgUpToku	= FALSE;		// 変更確認フラグ(全体用特殊行)
	BOOL	ufgKn		= FALSE;		// 変更確認フラグ(科目)
	BOOL	ufgHdName	= FALSE;		// 変更確認フラグ(品目)
	BOOL	ufgSuuryou	= FALSE;		// 変更確認フラグ(数量)
	BOOL	ufgTanni	= FALSE;		// 変更確認フラグ(単位)
	BOOL	ufgPrice	= FALSE;		// 変更確認フラグ(単価)
	BOOL	ufgVal		= FALSE;		// 変更確認フラグ(金額)
	BOOL	ufgTeki		= FALSE;		// 変更確認フラグ(摘要)
	BOOL	ufgFunc		= FALSE;		// 変更確認フラグ(未使用？)

	// レコード更新したことを示すフラグ
	BOOL	ufgRsUpdate	= FALSE;
	// テーブルアクセスクラス
	CdbUc051Tanaoroshi	mfcRec( m_pDB );
	// 作業用変数へ 登録対象のフィールド値を代入
	udTemp = *inSaveData;
	// データ行か、空行かのチェック
	udTemp.m_FgFunc = m_ReadData[inRow].m_FgFunc;

	if ( udTemp.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)

// midori 153907 del -->
		//if (( udTemp.m_KnOrder == 0 ) &&
		//	//( udTemp.m_HdName == "" ) &&
		//	( m_clsFunc.CompareNoCRLF(udTemp.m_HdName, _T("")) == 0) &&
		//	( udTemp.m_Suuryou == "" ) &&
		//	( udTemp.m_Tanni == "" ) &&
		//	( udTemp.m_Price == "" ) &&
		//	( udTemp.m_Val == "" ) &&
		//	//( udTemp.m_Teki == "" )){
		//	( m_clsFunc.CompareNoCRLF(udTemp.m_Teki, _T("")) == 0 )){
// midori 153907 del <--
// midori 153907 add -->
		if (( udTemp.m_KnOrder == 0 ) &&
			( udTemp.m_HdName == "" ) &&
			( udTemp.m_Suuryou == "" ) &&
			( udTemp.m_Tanni == "" ) &&
			( udTemp.m_Price == "" ) &&
			( udTemp.m_Val == "" ) &&
			( udTemp.m_Teki == "" ))	{
// midori 153907 add <--

			// 空行なら
			udTemp.m_FgFunc = ID_FGFUNC_NULL;
		}
		else {
			// データが入っていれば
			udTemp.m_FgFunc = ID_FGFUNC_DATA;
		}
	}

	///////////////////////////////////////////////////////////////
	// 更新状況のチェック

	//	(1) 科目（Order番号管理になっている)
	if (udTemp.m_KnOrder != m_ReadData[inRow].m_KnOrder) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgKn		= TRUE;		// 科目用
	}

	//	(2) 品目
// midori 153907 del -->
	////if (udTemp.m_HdName != m_ReadData[inRow].m_HdName) {
	//if(m_clsFunc.CompareNoCRLF(udTemp.m_HdName, m_ReadData[inRow].m_HdName) != 0){
// midori 153907 del <--
// midori 153907 add -->
	if(udTemp.m_HdName != m_ReadData[inRow].m_HdName)	{
// midori 153907 add <--
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgUpToku	= TRUE;		// 全体(特殊行)
		ufgHdName	= TRUE;		// 品名用
	}
	
	//	(3)	数量
	if (udTemp.m_Suuryou != m_ReadData[inRow].m_Suuryou) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgSuuryou	= TRUE;		// 数量用
	}

	//	(4) 単位
	if (udTemp.m_Tanni != m_ReadData[inRow].m_Tanni) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgTanni	= TRUE;		// 単位用
	}

	//	(5)	単価
	if (udTemp.m_Price != m_ReadData[inRow].m_Price) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgPrice	= TRUE;		// 単価用
	}

	//	(6) 期末現在高
	if (udTemp.m_Val != m_ReadData[inRow].m_Val) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgVal		= TRUE;		// 金額用
	}

	//	(7) 摘要
// midori 153907 del -->
	////if (udTemp.m_Teki != m_ReadData[inRow].m_Teki) {
	//if(m_clsFunc.CompareNoCRLF(udTemp.m_Teki, m_ReadData[inRow].m_Teki) != 0){
// midori 153907 del <--
// midori 153907 add -->
	if(udTemp.m_Teki != m_ReadData[inRow].m_Teki)	{
// midori 153907 add <--
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgTeki		= TRUE;		// 摘要用
	}

	//	(8) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず) 
	if (m_ReadData[inRow].m_FgFunc <= ID_FGFUNC_DATA) {
		// 元が空行orデータ行の場合、入力内容により空行orデータ行となる
		// 元が特殊行の場合、FgFuncは更新しない
		if (udTemp.m_FgFunc != m_ReadData[inRow].m_FgFunc) {
			ufgUpData	= TRUE;		// 全体(データ行/空行)
			ufgFunc		= TRUE;		// ufgFunc用
		}
	}

	///////////////////////////////////////////////////////////////////
	// 1 レコード更新処理
	if (( udTemp.m_FgFunc  == ID_FGFUNC_DATA ) || ( udTemp.m_FgFunc == ID_FGFUNC_IKKATUMANUAL )) {	// データ行or一括集計金額行（手動）
		///////////////////////////////////////////////////////////////////
		// データ行or一括集計金額行（手動）の場合の処理

		// 種類について KnOrder に関連する、名称、シーケンスを取得
		if ((ufgKn == TRUE) && (udTemp.m_KnOrder > 0)) {
			// 科目インデックスがNULLなら登録しない
			CdbUcLstKamoku		rsKamo( m_pDB );
			int intRet = rsKamo.RequeryKnOrder(m_uInfo.intFormSeq, udTemp.m_KnOrder);
			if ( intRet != DB_ERR_OK ) {	// 失敗
				return FUNCTION_NG;
			}
			udTemp.m_KnSeq = rsKamo.m_KnSeq;
			udTemp.m_KnName = rsKamo.m_KnName;
// midori 152137 add -->
			udTemp.m_KnKana = rsKamo.m_KnKana;
// midori 152137 add <--

			rsKamo.Fin();
		}

		// 更新処理
		if (ufgUpData == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow(inPage, inRow);

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();
				
				//	科目変更あり？
				if (ufgKn == TRUE) {	
					mfcRec.m_KnSeq		= udTemp.m_KnSeq;
					mfcRec.m_KnName		= udTemp.m_KnName;
					mfcRec.m_KnOrder	= udTemp.m_KnOrder;
// midori 152137 add -->
					mfcRec.m_KnKana		= udTemp.m_KnKana;
// midori 152137 add <--
				}
				
				//	品目変更あり？
				if (ufgHdName == TRUE) {
					CStringArray strArray;
					m_clsFunc.StrDivision( udTemp.m_HdName, &strArray, 2, TRUE, TRUE );
					mfcRec.m_HdName		= m_clsFunc.StrDocking( &strArray );
				}
				
				//	数量変更あり？
				if (ufgSuuryou == TRUE) {
					mfcRec.m_Suuryou 	= udTemp.m_Suuryou;
				}

				//	単位変更あり？
				if (ufgTanni == TRUE) {
					mfcRec.m_Tanni		= udTemp.m_Tanni;
				}

				//	単価変更あり？
				if (ufgPrice == TRUE) {
					mfcRec.m_Price		= udTemp.m_Price;
				}

				//	期末現在高変更あり？
				if (ufgVal == TRUE) {
					mfcRec.m_Val		= udTemp.m_Val;
				}
				
				//	摘要変更あり？
				if (ufgTeki == TRUE) {
					CStringArray strArray;
					m_clsFunc.StrDivision( udTemp.m_Teki, &strArray, 2, TRUE, TRUE );
					mfcRec.m_Teki		= m_clsFunc.StrDocking( &strArray );
				}
				
				// 特殊行フラグは、強制的に書き込む
				mfcRec.m_FgFunc		= udTemp.m_FgFunc;
				mfcRec.m_FgInput	= udTemp.m_FgInput;

				mfcRec.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
// No.200905 add -->
	else if ( udTemp.m_FgFunc == ID_FGFUNC_IKKATUAUTO && ufgKn == TRUE ) {
// 改良No.21-0086,21-0529 cor -->
		//if(KamokuRowEnableSgn(m_pDB,0) == 1) {
// ------------------------------
		if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1) {
// 改良No.21-0086,21-0529 cor <--
			// 種類について KnOrder に関連する、名称、シーケンスを取得
			if (udTemp.m_KnOrder > 0) {
				// 科目インデックスがNULLなら登録しない
				CdbUcLstKamoku		rsKamo( m_pDB );
				int intRet = rsKamo.RequeryKnOrder(m_uInfo.intFormSeq, udTemp.m_KnOrder);
				if ( intRet != DB_ERR_OK ) {	// 失敗
					return FUNCTION_NG;
				}
				udTemp.m_KnSeq = rsKamo.m_KnSeq;
				udTemp.m_KnName = rsKamo.m_KnName;
				udTemp.m_KnKana = rsKamo.m_KnKana;

				rsKamo.Fin();
			}
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow( inPage, inRow );

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();

				//	科目
				mfcRec.m_KnSeq		= udTemp.m_KnSeq;
				mfcRec.m_KnName		= udTemp.m_KnName;
				mfcRec.m_KnOrder	= udTemp.m_KnOrder;
				mfcRec.m_KnKana		= udTemp.m_KnKana;
				mfcRec.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
// No.200905 add <--
	else if ( udTemp.m_FgFunc == ID_FGFUNC_NULL ) {	// 空行
		///////////////////////////////////////////////////////////////////
		// 空行の場合の処理

		// 更新処理
		if (ufgUpData == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow(inPage, inRow);

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();
				mfcRec.m_FgFunc		= udTemp.m_FgFunc;
				mfcRec.m_KeiStr		= _T("");
				mfcRec.m_NumGroup	= 0;
				mfcRec.m_KnSeq		= 0;
				mfcRec.m_KnOrder	= 0;
				mfcRec.m_KnName		= _T("");
				mfcRec.m_HdName		= _T("");
				mfcRec.m_Suuryou	= _T("");
				mfcRec.m_Tanni		= _T("");
				mfcRec.m_Price		= _T("");
				mfcRec.m_Val		= _T("");
				mfcRec.m_Teki		= _T("");
				mfcRec.m_FgInput	= ID_AUTOCALC_AUTO;
// midori 152137 add -->
				mfcRec.m_KnKana		= _T("");	// 科目
// midori 152137 add <--
				mfcRec.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
	else  {		// データ行、空行 ではない（特殊行)
		///////////////////////////////////////////////////////////////////
		// 特殊行の場合の処理

		// 更新処理
		if (ufgUpToku == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow(inPage, inRow);

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();
				mfcRec.m_HdName = udTemp.m_HdName; 
				mfcRec.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}


	///////////////////////////////////////////////////////////////////
	// 内部 1ページバッファも更新する
	int retFunc = FUNCTION_OK;				//戻り値：成功（変更なし)
	if (ufgRsUpdate == TRUE) {
		retFunc = FUNCTION_UPDATE;		//戻り値：成功（変更あり)
		if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
			RecBufSetData( &mfcRec );
		}
	}

	mfcRec.Fin();

	return retFunc;
}

//**************************************************
//	１つ前のデータ表示処理
//	【引数】	rsData	…	共通テーブルクラス
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virFncCellCopyProc( CdbUc000Common* rsData )
{
	CdbUc051Tanaoroshi*	rs;
	int					intCol;		// 現在の列

	rs = (CdbUc051Tanaoroshi*)rsData;

	// 列取得
	intCol = CmnGetControlCol();

	// 列によって処理を分岐
	switch( intCol ){

	// 科目
	case ID_COL_051_KAMOKU:
// midori 152137 del -->
//		CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_KnOrder );
// midori 152137 del <--
// midori 152137 add -->
		CmnDiagSetCombo2( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_KnOrder );
// midori 152137 add <--
		break;
	// 品目
	case ID_COL_051_HINNMOKU:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_HdName, 0 );
		break;
	// 数量
	case ID_COL_051_SUURYOU:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Suuryou );
		break;
	// 単位
	case ID_COL_051_TANNI:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Tanni, 0 );
		break;
	// 単価
	case ID_COL_051_TANNKA:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Price );
		break;
	// 期末現在高
	case ID_COL_051_KIMATU:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Val );
		break;
	// 摘要
	case ID_COL_051_TEKIYOU:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Teki, 0 );
		break;
	}

	// 画面更新
	m_pDiag->Refresh();
}

//********************************************************************************
//	一括集計金額行のデータ設定
//		IN		int					呼び出し元（0:ソート，1:一括金額参照，2:特殊行挿入）
//				CfrmUc000Common*	テーブル情報
//				CString				名称
//				CALCKEI_INFO		一括集計金額情報
//				SORTKAMOKU_INFO		科目情報
//		RET		なし
//********************************************************************************
void CfrmUc051Tanaorosi::virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, 
											  CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku)
{
	CdbUc051Tanaoroshi*	mfcRec = (CdbUc051Tanaoroshi*)rsData;

	switch (nType) {
	case	0:
		// ソートの一括集計処理 ･･･ 科目の無い帳表はいらない
		mfcRec->m_HdName = strName;					// 名称：品目
		mfcRec->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定

		mfcRec->m_KnSeq = uKamoku.intKnSeq;			// 科目情報
		mfcRec->m_KnOrder = uKamoku.intKnOrder;		// 科目情報
		mfcRec->m_KnName = uKamoku.strKnName;		// 科目情報
// midori 152137 add -->
		mfcRec->m_KnKana = uKamoku.strKnKana;		// 科目情報（カナ)
// midori 152137 add <--
		break;
	case	1:
		// 一括金額参照の戻す処理
		mfcRec->m_HdName = strName;					// 名称：品目
		mfcRec->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case	2:
		// 特殊行挿入の一括金額選択時
		mfcRec->m_HdName = strName;					// 名称：品目
		break;
	}
}

//**************************************************
//	ソート後に改頁挿入，小計行挿入処理
//	【引数】	pCdlgSort	…	ソートダイアログクラス
//				rsData		…	共通テーブルクラス
//	【戻値】	FUNCTION_OK	…	成功
//				FUNCTION_NG	…	失敗
//**************************************************
int CfrmUc051Tanaorosi::virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData)
{
	CdbUc051Tanaoroshi*	rs;
	CString				strFilter;
	CString				strSort;

	// ソート項目の再設定（科目）		→ 帳表により実行有無が異なる
	CdbUcLstKamoku	rsKmk( m_pDB );
//@	rsKmk.UpdateDataTable(TBL_NAME_051, m_uInfo.intFormSeq);
	rsKmk.UpdateDataTable(m_uInfo.intFormSeq);
	rsKmk.Fin();

	// ソート条件取得
	CmnTblSortSubGetSortParam(pCdlgSort, &strFilter, &strSort);

	rs = (CdbUc051Tanaoroshi*)rsData;

	// 対象フィールドがNULLならNULL文字列にする：文字列連結のソート時は必須
	rs->UpdateDataTableWork();

	// ソート実行
	return rs->RequerySortParameter(strFilter, strSort);
}

//**************************************************
//	行コピー処理
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virRowButtonCopyProc()
{
	// カレント行のデータを、コーピー用バッファに格納
	m_CopyData	= m_ReadData[m_uInfo.intCurRow];

	// コピーフラグON
	m_nCopyFlag	= 1;
}

//********************************************************************************
//	テーブルから科目情報を取得
//		IN		Cdb000Common* rsData	テーブル情報
//				SORTKAMOKU_INFO*		科目情報
//		RET		なし
//********************************************************************************
void CfrmUc051Tanaorosi::virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku)
{
	CdbUc051Tanaoroshi*	mfcRec = (CdbUc051Tanaoroshi*)rsData;

	// 本当ならGetFieldValue()で取得する予定だったが、時間も無いのであきらめた
	uKamoku->intKnSeq = mfcRec->m_KnSeq;
	uKamoku->intKnOrder = mfcRec->m_KnOrder;
	uKamoku->strKnName = mfcRec->m_KnName;
// midori 152137 add -->
	uKamoku->strKnKana = mfcRec->m_KnKana;
// midori 152137 add <--
}

//********************************************************************************
//	共通処理から帳表のカレント情報を取得
//		IN		CURRENT_INFO			取得する情報の格納領域
//		RET		なし
//********************************************************************************
void CfrmUc051Tanaorosi::virGetCurrentInfo(CURRENT_INFO* uCurInfo)
{
	uCurInfo->intFgFunc = m_ReadData[m_uInfo.intCurRow].m_FgFunc;
	uCurInfo->intNumGroup = m_ReadData[m_uInfo.intCurRow].m_NumGroup;
	uCurInfo->intCopyFlag = m_nCopyFlag;
	uCurInfo->intFgInput = m_ReadData[m_uInfo.intCurRow].m_FgInput;
}

//**************************************************
//	指定された行のデータをテーブルに保存(挿入貼付作業用)
//	【引数】	inPage				…	登録するレコードのページ番号
//				inRow				…	登録するレコードの行番号
//	【戻値】	FUNCTION_UPDATE		…	成功(更新あり)
//				FUNCTION_OK			…	成功(更新なし)
//				FUNCTION_NG			…	失敗
//**************************************************
int CfrmUc051Tanaorosi::virRecBufSaveDataForPasteInsert(int inPage, int inRow)
{
	RecBufClearData( &m_ReadData[inRow] );			// レコードを初期化
	m_ReadData[inRow].m_FgFunc	= ID_FGFUNC_DATA;	//データ行とする

	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	return	RecBufSaveDataSub( inPage, inRow, &m_CopyData );
}

//**************************************************
//	指定された行のデータをテーブルに保存(上書き貼付作業用)
//	【引数】	inPage				…	登録するレコードのページ番号
//				inRow				…	登録するレコードの行番号
//	【戻値】	FUNCTION_UPDATE		…	成功(更新あり)
//				FUNCTION_OK			…	成功(更新なし)
//				FUNCTION_NG			…	失敗
//**************************************************
int	CfrmUc051Tanaorosi::virRecBufSaveDataForPaste( int inPage, int inRow )
{
	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	return RecBufSaveDataSub( inPage, inRow, &m_CopyData );
}

//**************************************************
//	フォーカス移動処理（メイン）
//	【引数】	nChar		…	キーコード（VK_UP，VK_RIGHT，VK_DOWN，VK_LEFT，VK_RETURN，VK_TAB）
//	【戻値】	0以上		…	移動先コントロールのインデックス番号
//				-1			…	先頭項目で"Shift+Tab"のためタイトルへ移動
//				-2			…	最終項目で"Shift"のため別コントロールへ移動
//				-3			…	ComboBoxで"↑/↓"はキー操作を無視する
//				-4			…	先頭行で↑or先頭項目で←はタイトルへ移動
//**************************************************
int	CfrmUc051Tanaorosi::GetFocusPosition(int nChar)
{
	int		intRow;					// カレント行番号
	int		intCol;					// カレント列番号
	int		intVector[4];			// 矢印キーの移動先（0:↑，1:→，2:↓，3:←）
	int		intNext = 0;			// フォーカス移動先
	int		intRowChange = 0;		// 行をまたぐ場合のインデックス増減数
//	BOOL	fSpecial;				//	特殊行判断フラグ（TRUE：特殊行／FALSE：非特殊行）

	// カレント行＋列番号を取得
	CmnGetControlPosition(&intRow, &intCol);

	// 行をまたぐ場合のインデックス増減数（摘要→次の行の金融機関名）
	intRowChange = m_uInfo.intCtlStartIndex + 1;
	
	//	特殊行か判断
//	fSpecial = CheckSpecialRow();

	//	計行じゃない？
	if ( CmnCheckTotalRow() == FALSE ){
		
		// 「種類」はComboBoxのため、「↑，↓」キーではカーソルを移動しない（特殊行は除く）
//		if (( intCol == ID_COL_051_KAMOKU ) && ( fSpecial == FALSE )){
		if ( intCol == ID_COL_051_KAMOKU ){
		
			if ((nChar == VK_UP) || (nChar == VK_DOWN)) {
				// キー操作を無視する
				return -3;
			}
		}
	}

	//---------------------------------------------------------------- 
	//	矢印キーによる移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じなので省略
	//	（※特殊行は除く）
	//---------------------------------------------------------------- 
//	if (( intCol == ID_COL_051_KAMOKU ) && ( fSpecial == FALSE )){
	if ( intCol == ID_COL_051_KAMOKU ){
		// カレント列が「科目」の場合
		intVector[0] = m_uInfo.intCurCtlIndex;							// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
	else if ( intCol == ID_COL_051_SUURYOU ){
		// カレント列が「数量」の場合
		intVector[0] = m_uInfo.intCurCtlIndex + 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax + 1;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_051_TANNI ){
		// カレント列が「単位」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax - 1;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex - 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_051_TEKIYOU ){
		// カレント列が「摘要」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + intRowChange;			// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else{
		// その他の行の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}

	//---------------------------------------------------------------- 
	//	押下キーから移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じ
	//---------------------------------------------------------------- 
	switch (nChar) {
	case VK_UP:
		intNext = intVector[0];			// 「↑」キー押下時
		break;
	case VK_RIGHT:
		intNext = intVector[1];			// 「→」キー押下時
		break;
	case VK_DOWN:
		intNext = intVector[2];			// 「↓」キー押下時
		break;
	case VK_LEFT:
		intNext = intVector[3];			// 「←」キー押下時
		break;
	case VK_RETURN:	// Enterキーでカーソルを下に移動するモード対応
		if(m_ReturnMoveDown)	intNext = intVector[2];			//	Enterキーでカーソルを下に移動するチェック時は「↓」キーと同じ
		else					intNext = intVector[1];			// 「Return」キー押下時：「→」キーと同じ
		break;
	case VK_TAB:
		if (CmnCheckShiftKey() == FALSE) {
			intNext = intVector[1];		// 「Tab」キー押下時：「→」キーと同じ
		}
		else {
			// 例外処理：先頭項目で"Shift+Tab"は別コントロールへ移動
			// それ以外は「←」キーと同じ
			if ((intRow <= 1) && (intCol == ID_COL_051_KAMOKU)) {
				return -1;
			}
			intNext = intVector[3];		// 「Shift+Tab」キー押下時：「→」キーと同じ
		}
		break;
	}

	if (intNext >= 0) {
		// 当処理で合計行を考慮したカーソル移動先を確定させる
		intNext = GetTotalRowIndex(nChar, m_uInfo.intCurCtlIndex, intNext);
	}
	else {
		// コントロールのフィールド外（上へ）へはタイトルに移動する
		return -4;
	}

	return intNext;
}

//**************************************************
//	フォーカス移動処理（合計欄を考慮）
//	【引数】	nChar		…	キーコード（VK_UP，VK_RIGHT，VK_DOWN，VK_LEFT，VK_RETURN，VK_TAB）
//				intIndex	…	カレントのコントロールインデックス
//				intNext		…	合計欄を考慮していない移動先のインデックス番号
//	【戻値】	合計欄を考慮した移動先ののインデックス番号
//**************************************************
int CfrmUc051Tanaorosi::GetTotalRowIndex(int nCharOrg ,int intIndex ,int intNext)
{
	int		nCharTemp;				// キーコード（VK_RETURN，VK_TABを矢印キーに置き換え）
	int		nNowRow;				//	現在の行
	int		nNextRow;				//	移動先の行
	int		nBorder[3];				//	境界
	int		nRet;					//	戻値

	// キーコード置き換え（VK_RETURN，VK_TABを矢印キーに置き換え）
	switch( nCharOrg ){
	case VK_RETURN:			// Returnキーなら→キーと同じ
		nCharTemp = VK_RIGHT;
		break;
	case VK_TAB:			// Tabキーなら→キーと同じ，Shift+Tabなら←キーと同じ
		nCharTemp = VK_RIGHT;
		if ( CmnCheckShiftKey() == TRUE ) {
			nCharTemp = VK_LEFT;
		}
		break;
	default:
		nCharTemp = nCharOrg;
		break;
	}
	
	nNowRow = CmnGetControlRow( intIndex );	//	カレント行を取得
	nNextRow = CmnGetControlRow( intNext );	//	移動先行を取得（暫定）
	nRet = intNext;							//	次の移動先を戻値として取得
	
	//	現在の頁の計の種別で分岐
	switch( CmnGetControlTotalType() ){
	case ID_OUTKEI_OFF:			//	オフ
	case ID_OUTKEI_PAGEKEI:		//	頁計
	case ID_OUTKEI_RUIKEI:		//	累計
		
		//	現在行が計行？
		if ( intIndex >= m_uInfo.intT1StartIndex ){
			nNowRow = m_uInfo.intRowMax;
		}
		
		//	移動先行が計行？
		if ( intNext >= m_uInfo.intT1StartIndex ){
			nNextRow = m_uInfo.intRowMax;
		}

		nBorder[0] = m_uInfo.intRowMax - 1;		//	データ行
		nBorder[1] = m_uInfo.intRowMax;			//	頁計
		nBorder[2] = 0;							//	累計（なし）
		break;
	case ID_OUTKEI_BOTH:		//	頁計＆累計
		
		//	現在行が計２？
		if ( intIndex >= m_uInfo.intT2StartIndex ){
			nNowRow = m_uInfo.intRowMax;
		}
		//	現在行が計１？
		else if ( intIndex >= m_uInfo.intT1StartIndex ){
			nNowRow = m_uInfo.intRowMax - 1;
		}
		
		//	移動先行が計２？
		if ( intNext >= m_uInfo.intT2StartIndex ){
			nNextRow = m_uInfo.intRowMax;
		}
		//	移動先行が計１？
		else if ( intNext >= m_uInfo.intT1StartIndex ){
			nNextRow = m_uInfo.intRowMax - 1;
		}

		nBorder[0] = m_uInfo.intRowMax - 2;		//	データ行
		nBorder[1] = m_uInfo.intRowMax - 1;		//	頁計
		nBorder[2] = m_uInfo.intRowMax;			//	累計
		break;
	}

	//	現在、データ行に居る
	if ( nNowRow <= nBorder[0] ){
		
		//	データ行を超えた？
		if ( nNextRow > nBorder[0] ){
			//	計行に移動
			//nRet = CmnGetControlIndex( ( nBorder[1] + 1 ), ID_COL_051_KAMOKU );
			if (nCharOrg == VK_TAB) {
				return -2;
			}
		}
	}
	//	現在、頁計に居る
	//else if ( nNowRow == nBorder[1] ){

	//	// すでに合計行にいるので、"intNext"は無視する
	//	switch ( nCharTemp ){
	//	case VK_UP:			// ↑：23行目のデータ行の支店名項目へ移動
	//		nRet = CmnGetControlIndex( nBorder[0], ID_COL_051_HINNMOKU );
	//		break;
	//	case VK_LEFT:		// ←/Shift+Tab：23行目のデータ行の摘要項目へ移動
	//		nRet = CmnGetControlIndex( nBorder[0], ID_COL_051_TEKIYOU );
	//		break;
	//	default:			// →/↓/Return/Tab：25行目の合計行②へ移動
	//		
	//		//	累計なし？
	//		if ( nBorder[2] == 0 ){
	//			// 例外処理：最終項目で"Tab"は別コントロールへ移動
	//			// それ以外は「→」キーと同じ
	//			if (nCharOrg == VK_TAB) {
	//				return -2;
	//			}
	//		}
	//		//	累計あり
	//		else{
	//			nRet = CmnGetControlIndex( ( nBorder[2] + 1 ), ID_COL_051_KAMOKU );
	//		}
	//		break;
	//	}
	//}
	////	現在、累計に居る場合
	//else if ( nNowRow == nBorder[2] ){

	//	// すでに合計行にいるので、"intNext"は無視する
	//	switch ( nCharTemp ){
	//	case VK_UP:			// ↑：24行目の合計行① or 24行目のデータ行の支店名項目へ移動
	//	case VK_LEFT:		// ←/Shift+Tab：24行目の合計行① or 24行目のデータ行の摘要項目へ移動
	//		nRet = CmnGetControlIndex( ( nBorder[1] + 1 ), ID_COL_051_KAMOKU );
	//		break;
	//	default:			// →/↓：移動できない
	//		// 例外処理：最終項目で"Tab"は別コントロールへ移動
	//		// それ以外は「→」キーと同じ
	//		if (nCharOrg == VK_TAB) {
	//			return -2;
	//		}
	//		break;
	//	}
	//}
	
	//	戻値を返す
	return( nRet );
}

//**************************************************
//	自動計算フラグ確認
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::CheckFgInput()
{
	CString		szNow;	//	現在の入力値を保存
// midori 18-0887 add -->
	CString		szTmp1=_T("");
	CString		szTmp2=_T("");
	CString		szTmp3=_T("");
	int			nCtlIndex=0;
// midori 18-0887 add <--

	//	初期化
	szNow.Empty();
	
	//	特殊行フラグで分岐
	switch( m_ReadData[m_uInfo.intCurRow].m_FgFunc ){
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:
		// ポジション取得
		m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
		// 行・列取得
		CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

		// 「期末現在高」列？
		if ( m_uInfo.intCurCol == ID_COL_051_KIMATU ){

			// 空行の場合、自動計算フラグはOFF
			if ( CheckRowNullData() == TRUE ){
				//	自動計算フラグを自動計算に設定
				m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_AUTO;
				//	背景色を変更
				virInitialControlFgInputCell( m_uInfo.intCurRow, m_ReadData[m_uInfo.intCurRow].m_FgFunc, m_ReadData[m_uInfo.intCurRow].m_FgInput );
				return;
			}
			
			// 現在の入力値を取得
			szNow = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );

// midori 18-0887 del -->
			////	現在の入力値と前回の入力値が違う？
			//if ( szNow != m_uInfo.OldstrDiag ){
			//	//	自動計算フラグを手入力に設定
			//	m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_INPUT;
			//	//	背景色を変更
			//	virInitialControlFgInputCell( m_uInfo.intCurRow, m_ReadData[m_uInfo.intCurRow].m_FgFunc, m_ReadData[m_uInfo.intCurRow].m_FgInput );
			//}
// midori 18-0887 del <--
// midori 18-0887 add -->
			//	数量の入力値取得
			nCtlIndex = CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_051_SUURYOU);
			szTmp1 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	単価の入力値取得
			nCtlIndex = CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_051_TANNKA);
			szTmp2 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	数量×単価を計算（四捨五入）
// midori UC_0025 del -->
			//szTmp3 = GetMulVal(szTmp1,szTmp2);
// midori UC_0025 del <--
// midori UC_0025 add -->
			if(szTmp1 == "" || szTmp2 == "")	{
				szTmp3 = "";
			}
			else	{
				szTmp3 = GetMulVal(szTmp1,szTmp2);
			}
// midori UC_0025 add <--
			//	現在の入力値からカンマとスペースを除く
			szNow.Replace(_T(","),_T(""));
			szNow.TrimLeft();
			//	自動計算した結果と現在の入力値が違う？
			if(szNow != szTmp3 && szNow.IsEmpty() != TRUE)	{
				//	自動計算フラグを手入力に設定
				m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_INPUT;
			}
			else	{
				//	自動計算フラグを自動計算に設定
				m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_AUTO;
			}
			//	背景色を変更
			virInitialControlFgInputCell(m_uInfo.intCurRow,m_ReadData[m_uInfo.intCurRow].m_FgFunc,m_ReadData[m_uInfo.intCurRow].m_FgInput);
// midori 18-0887 add <--
		}	
		break;
	default:
		break;
	}
}

//**************************************************
//	自動計算結果を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::SetAutoCalc()
{
	int			nCtlIndex;	//	コントロールインデックス
	CString		szSuuryou;	//	数量
	CString		szPrice;	//	単価
	CString		szVal;		//	期末現在高

	//	初期化
	szSuuryou.Empty();
	szPrice.Empty();
	szVal.Empty();

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );
	
	//	特殊行フラグで分岐
	switch( m_ReadData[m_uInfo.intCurRow].m_FgFunc ){
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:

		//	列で分岐
		switch( m_uInfo.intCurCol ){
		//	数量
		case ID_COL_051_SUURYOU:
		//	単価
		case ID_COL_051_TANNKA:
		//	期末現在高
		case ID_COL_051_KIMATU:

			//	自動計算フラグ有効？
			if ( m_ReadData[m_uInfo.intCurRow].m_FgInput != ID_AUTOCALC_INPUT ){
				
				//	数量のコントロールインデックス取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_051_SUURYOU );
				//	数量の入力値取得
				szSuuryou = CmnDiagGetKingaku( m_pDiag, nCtlIndex );
				//	単価のコントロールインデックス取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_051_TANNKA );
				//	単価の入力値取得
				szPrice = CmnDiagGetKingaku( m_pDiag, nCtlIndex );
				
				//	期末現在高のコントロールインデックスを取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_051_KIMATU );

				//	数量と単価共に入力あり？
				if (( szSuuryou != "" ) && ( szPrice != "" )){
					//	数量×単価を計算（四捨五入）
					szVal = GetMulVal( szSuuryou, szPrice );
					//	ICS-Controlへ金額を設定
					CmnDiagSetKingaku( m_pDiag, nCtlIndex, szVal );
// midori 152392 add -->
					// 現在の入力値を取得
					m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, nCtlIndex );
// midori 152392 add <--
				}
				//	数量か単価どちらかが入力なし？
				else if (( szSuuryou == "" ) || ( szPrice == "" )){

					//	期末現在高をクリア
					m_pDiag->DataClear( nCtlIndex, TRUE );
				}

				// 更新
				m_pDiag->Refresh();
			}
			break;

		//	その他
		default:
			break;
		}
		break;

	//	その他
	default:
		break;
	}
}

//**************************************************
//	自動計算結果を取得
//	【引数】	なし
//	【戻値】	自動計算結果文字列
//**************************************************
CString CfrmUc051Tanaorosi::GetAutoCalc( int nRow )
{
	CString		szRet;		//	戻値
	int			nIndex;		//	インデックス
	CString		szSuuryou;	//	数量
	CString		szPrice;	//	単価
	BOOL		fAutoCalc;	//	自動計算したかフラグ（TRUE：自動計算した/FALSE：自動計算してない）

	//	初期化
	szRet.Empty();
	fAutoCalc = FALSE;
	
	//	特殊行フラグで分岐
	switch( m_ReadData[nRow].m_FgFunc ){
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:

		//	自動計算フラグ有効？
		if ( m_ReadData[nRow].m_FgInput != ID_AUTOCALC_INPUT ){

			//	数量のコントロールインデックス取得
			nIndex = CmnGetControlIndex( nRow, ID_COL_051_SUURYOU );
			//	数量の入力値取得
			szSuuryou = CmnDiagGetKingaku( m_pDiag, nIndex );
			//	単価のコントロールインデックス取得
			nIndex = CmnGetControlIndex( nRow, ID_COL_051_TANNKA );
			//	単価の入力値取得
			szPrice = CmnDiagGetKingaku( m_pDiag, nIndex );
			
			//	期末現在高のコントロールインデックスを取得
			nIndex = CmnGetControlIndex( nRow, ID_COL_051_KIMATU );

			//	数量と単価共に入力あり？
			if (( szSuuryou != "" ) && ( szPrice != "" )){
				//	数量×単価を計算（四捨五入）
				szRet = GetMulVal( szSuuryou, szPrice );
				fAutoCalc = TRUE;
			}
		}
		break;
	
	//	その他
	default:
		break;
	}

	//	自動計算してない？
	if ( fAutoCalc == FALSE ){
		nIndex = CmnGetControlIndex( nRow , ID_COL_051_KIMATU );
//		szRet = CheckOutsideValue( m_pDiag, nIndex );
		szRet = CmnDiagGetKingaku( m_pDiag, nIndex );
	}

	//	戻値を返す
	return( szRet );
}

//**************************************************
//	掛け算した値を取得
//	【引数】	szVal1		…	値1
//				szVal2		…	値2
//	【戻値】	掛け算した結果
//**************************************************
CString CfrmUc051Tanaorosi::GetMulVal( CString szVal1, CString szVal2 )
{
	CString		szRet;			//	戻値（文字列）
	char		szBuf1[16];		//	バッファ1（文字列）
	char		szBuf2[16];		//	バッファ2（文字列）
	char		szBuf3[16];		//	バッファ3（文字列）
	CString		szBuf4;			//	バッファ4（文字列）
	double		dBuf[4];		//	バッファ（実数）
	
	//	初期化
	szRet = _T("0");
	ZeroMemory( szBuf1, sizeof( szBuf1 ) );
	ZeroMemory( szBuf2, sizeof( szBuf2 ) );
	ZeroMemory( szBuf3, sizeof( szBuf3 ) );
	szBuf4.Empty();
	ZeroMemory( dBuf, sizeof( dBuf ) );

	//	値をバッファに変換
	strcpy_s( szBuf1, sizeof( szBuf1 ), szVal1 );
	strcpy_s( szBuf2, sizeof( szBuf2 ), szVal2 );
	
	//	実数に変換
	dBuf[0] = atof( szBuf1 );
	dBuf[1] = atof( szBuf2 );
	
	//	掛け算
	dBuf[2] = dBuf[0] * dBuf[1];

	// 「0 × 負の値」の計算だと「-0」という値が
	// 返ってくるため対処する
	if ( dBuf[2] == -0 ){
		dBuf[2] = 0;
	}
	
	//++++++++++++++++++++++++++++++
	//	四捨五入（少数点第一位）
	//++++++++++++++++++++++++++++++
	//	0より大きい？
	if ( dBuf[2] > 0 ){
		dBuf[2] += 5000;
	}
	//	0より小さい？
	else if ( dBuf[2] < 0 ){
		dBuf[2] -= 5000;
	}
	
	//	少数点第二位どうしの掛け算なので10000で割る
	dBuf[3] = dBuf[2] / 10000;
	
	//	値が範囲を超えた？
	if ( dBuf[3] > DB_DECIMAL_MAX ){
		//	値を最大値に設定
		dBuf[3] = DB_DECIMAL_MAX;
	}
	//	値が範囲未満？
	else if ( dBuf[3] < DB_DECIMAL_MIN ){
		//	値を最小値に設定
		dBuf[3] = DB_DECIMAL_MIN;
	}
	
	//	文字列に変換
	szBuf4.Format( "%f", dBuf[3] );
	//	少数点以下を切り落とす
	szRet = szBuf4.Left( szBuf4.Find(".") );

	//	戻値を返す
	return( szRet );
}

//********************************************************************************
//	【F9:参照】参照ダイアログで使用する種別を取得
//		IN		int				カレント列番号
//				int				取引先時に使用するグループ番号（更新項目）
//									ID_ADDRESSGR_NONE：	なし
//									ID_ADDRESSGR_URI：	売掛金
//									ID_ADDRESSGR_KAI：	買掛金
//				long			現在選択項目のシーケンス番号　（更新項目）
//				int*			住所１の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*			住所２の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*			住所１の列番号
//				int*			住所２の列番号
//		RET		int				参照型の種別
//									ID_DLGTYPE_BANK：	金融機関
//									ID_DLGTYPE_ADDRESS：取引先
//									ID_DLGTYPE_KAMOKU：	科目
//									ID_DLGTYPE_NONE：	参照不可
//********************************************************************************
int CfrmUc051Tanaorosi::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference)
{
	int				intRefType;

	switch (intCol) {

	// 種類
	case ID_COL_051_KAMOKU:
		intRefType = ID_DLGTYPE_KAMOKU;
		*intGrSeq = ID_ADDRESSGR_NONE;
		*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_KnSeq;
		break;

	// その他：参照対象外
	default:
		intRefType = ID_DLGTYPE_NONE;
		*intGrSeq = ID_ADDRESSGR_NONE;
		*lngItemSeq = 0;
		break;
	}
	return intRefType;
}

//********************************************************************************
//	【F9:参照】参照ダイアログで選択された内容を表示
//		IN		CdlgReference*	参照ダイアログ情報
//				int				参照ダイアログの種別
//				int				カレント列番号				
//		RET		なし
//********************************************************************************
void CfrmUc051Tanaorosi::virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol)
{
	switch (intRefType) {

	// 種類
	case ID_DLGTYPE_KAMOKU:
		// 選択内容を表示
// midori 152137 del -->
//		CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udKamoku.m_KnOrder );
// midori 152137 del <--
// midori 152137 add -->
		CmnDiagSetCombo2( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udKamoku.m_KnOrder );
// midori 152137 add <--
		break;

	default:
		return;
		break;
	}
	m_pDiag->Refresh();
}

//********************************************************************************
//	【F9:参照】参照ダイアログの操作によりテーブルを更新
//		IN		int				参照ダイアログの種別
//		RET		なし
//********************************************************************************
void CfrmUc051Tanaorosi::virFncReferenceUpdateTbl(int intRefType, int nRefMode/*=0*/)
{
	switch (intRefType) {
	
	// 科目
	case ID_DLGTYPE_KAMOKU:
		{
			// "uc_051_Tanaoroshi"の更新
			CdbUcLstKamoku	rsKmk( m_pDB );
//@			rsKmk.UpdateDataTable(TBL_NAME_051, m_uInfo.intFormSeq);
			rsKmk.UpdateDataTable(m_uInfo.intFormSeq, nRefMode);
			rsKmk.Fin();
			break;
		}
	default:
		break;
	}	
}

//**************************************************
//	「F6：項目複写」ボタンの監視
//	※自動計算対象列は項目複写しない対応
//	【引数】	bFncEnable	…	ボタンの有効／無効状態
//									TRUE	…	有効
//									FALSE	…	無効
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::virOnUpdateButtonF6(BOOL* bFncEnable)
{
/*
	int		nCol;	//	列格納
	
	//	現在の列取得
	nCol = CmnGetControlCol();
	
	//	ボタンが有効でかつ列が期末現在高？
	if (( *bFncEnable == TRUE ) && ( nCol == ID_COL_051_KIMATU )){
		//	項目複写しないから無効に設定
		*bFncEnable = FALSE;
	}
*/
}

//**************************************************
//	入力不可モードの設定/解除
//	【引数】	nRow	…	行
//				nMode	…	入力不可モードフラグ
//								DIAG_MDFY_NOINPUT	…	エディットアイテムで全てのWM_CHARを無効とする
//								DIAG_MDFY_INPUT		…	DIAG_MDFY_NOINPUTモード解除
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_051_KAMOKU ), nMode );		//	科目（※科目がエディットボックスに変更された際に必要）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_051_HINNMOKU ), nMode );		//	品目
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_051_SUURYOU ), nMode );		//	数量
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_051_TANNI ), nMode );			//	単位
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_051_TANNKA ), nMode );		//	単価
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_051_KIMATU ), nMode );		//	期末現在高
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_051_TEKIYOU ), nMode );		//	摘要
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc051Tanaorosi::SetDiagBackColor()
{
	// 入力画面の各Diagの背景色を共通設定色に合わせる
	m_PageDiag.SetBackColor( m_ViewColor );
	m_TitleDiag.SetBackColor( m_ViewColor );
	m_NumberDiag.SetBackColor( m_ViewColor );
	m_HeadDiag.SetBackColor( m_ViewColor );
	m_Diag->SetBackColor( m_ViewColor );
	m_DiagKei->SetBackColor( m_ViewColor );
// midori 161108 add -->
	m_SaveDataDiag.SetBackColor( m_ViewColor );
// midori 161108 add <--
}

//**************************************************
//	値の範囲外確認
//	【引数】	pDiag		…	DIAGRAM_DATA構造体
//				nIndex		…	インデックス
//	【戻値】	期末現在高
//**************************************************
/*CString CfrmUc051Tanaorosi::CheckOutsideValue( CICSDiag* pDiag, int nIndex )
{
	CString			szBuf;	//	バッファ
	CString			szRet;	//	戻値

	//	金額を取得
	szRet = CmnDiagGetKingaku( m_pDiag, nIndex );
	
	//	金額が0？
	if ( szRet.Trim() == "0" ){
		
		//	本当は値が0じゃない？
		if ( CheckOutsideValueSub( pDiag, nIndex ) == TRUE ){
			//	文字列に変換
			szBuf.Format( "%f", ( double )DB_DECIMAL_MAX );
			//	範囲外の値を設定
			szRet = szBuf.Left( szBuf.Find(".") );
		}
	}
	
	//	戻値を返す
	return ( szRet );
}*/

//**************************************************
//	値の範囲外確認
//	【引数】	pDiag		…	DIAGRAM_DATA構造体
//				nIndex		…	インデックス
//				nCol		…	列
//	【戻値】	TRUE		…	0じゃない
//				FALSE		…	0
//**************************************************
/*BOOL CfrmUc051Tanaorosi::CheckOutsideValue( CICSDiag* pDiag, int nIndex, int nCol )
{
	BOOL			fRet = FALSE;	//	
	CString			szRet;			//	戻値
	
	//	期末現在高？
	if ( nCol == ID_COL_051_KIMATU ){

		//	金額を取得
		szRet = CmnDiagGetKingaku( m_pDiag, nIndex );
		
		//	金額が0？
		if ( szRet.Trim() == "0" ){
			
			//	本当は値が0じゃない？
			fRet = CheckOutsideValueSub( pDiag, nIndex );
		}
	}
	
	//	戻値を返す
	return ( fRet );
}*/

//**************************************************
//	値が0か確認
//	【引数】	pDiag		…	DIAGRAM_DATA構造体
//				nIndex		…	インデックス
//	【戻値】	TRUE		…	0じゃない
//				FALSE		…	0
//**************************************************
/*BOOL CfrmUc051Tanaorosi::CheckOutsideValueSub( CICSDiag* pDiag, int nIndex )
{
	BOOL			fRet;
	DIAGRAM_DATA	udData;
	
	// 初期化
	fRet = FALSE;
	m_clsFunc.DiagInit( &udData );
	
	// 金額取得
	pDiag->GetData( nIndex , ( LPUNKNOWN )&udData );

	//	'*'あり？
	if ( udData.data_edit.Find( '*' ) != -1 ){
		fRet = TRUE;
	}
	
	//	戻値を返す
	return( fRet );
}*/

//**************************************************
//	対象行にデータが入っているかどうか判断(自動計算フラグON/OFFの判断に使用)
//	【引数】	なし
//	【戻値】	TRUE	…　空行
//				FALSE	…	空行ではない(対象行にデータあり)
//**************************************************
BOOL CfrmUc051Tanaorosi::CheckRowNullData()
{
	REC_UC_051_TANAOROSI tmpSaveData;	// 画面データ1レコード格納用

	RecBufClearData( &tmpSaveData );	// レコードを初期化

	///////////////////////////////////////////////////////////////
	// コンポーネント(画面上の1レコード分の情報)を仮変数へ格納
	DIAGRAM_DATA	diadata;			// ICSDiag構造体
	int				intIndex;			// 対象行の各インデックス
	
	// 初期化
	m_clsFunc.DiagInit( &diadata );

	//	科目
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow , ID_COL_051_KAMOKU );
	m_pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
	tmpSaveData.m_KnOrder = diadata.data_combo;

	// 品目
	CStringArray strArray;
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow , ID_COL_051_HINNMOKU );
	tmpSaveData.m_HdName = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 数量
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow , ID_COL_051_SUURYOU );
	tmpSaveData.m_Suuryou = CmnDiagGetKingaku( m_pDiag, intIndex );
	
	// 単位
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow , ID_COL_051_TANNI );
	tmpSaveData.m_Tanni = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 単価
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow , ID_COL_051_TANNKA );
	tmpSaveData.m_Price = CmnDiagGetKingaku( m_pDiag, intIndex );
	
	// 期末現在高格納
	tmpSaveData.m_Val = GetAutoCalc( m_uInfo.intCurRow );
	
	//	自動計算フラグ
	tmpSaveData.m_FgInput = m_ReadData[m_uInfo.intCurRow].m_FgInput;

	// 摘要格納
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow , ID_COL_051_TEKIYOU );
	tmpSaveData.m_Teki = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 空行なら自動計算OFF
	if (( tmpSaveData.m_KnOrder == 0 ) &&
		( tmpSaveData.m_HdName == "" ) &&
		( tmpSaveData.m_Suuryou == "" ) &&
		( tmpSaveData.m_Tanni == "" ) &&
		( tmpSaveData.m_Price == "" ) &&
		( tmpSaveData.m_Val == "" ) &&
		( tmpSaveData.m_Teki == "" )){

		// 空行なら
		return TRUE;
	}

	return FALSE;
}

//**************************************************
//	棚卸方法登録(OnBnClickedMarginalButton1)実行前に必要なテーブルがオープン可かチェックする
//	【引数】	なし
//	【戻値】	TRUE 　… オープン可
//				FALSE  … オープン不可
//**************************************************
BOOL CfrmUc051Tanaorosi::CheckTableMarginal(){

	// ソート項目情報リストテーブルのチェック
	CdbUc052Tanaoroshi2 mfcTana( m_pDB );
	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcTana ) != TRUE ){
		return FALSE;
	}

	return TRUE;
}


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Viewの終了処理
//		・デストラクタで行っていた処理をここに移動
//
//	IN		なし
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CfrmUc051Tanaorosi::virEndProc(void)
{
	TRACE(_T("***** CfrmUc051Tanaorosi::virEndProc\n"));

	// ↓この処理でコントロールが破棄されているが、当関数が実行される時点では、
	// 　そのコントロールを持つView自体は生きている。
	// 　このため、コントロールに対する処理（セル位置の取得）でエラーになってしまう。
	// 　もともとはデストラクタにあったため、コントロールを破棄した後、
	// 　すぐにView自体も殺していたので問題なかった。
	// 　処理自体も特に意味がないので、コメントアウトする。

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;

	// 親Viewの終了処理
	return CfrmUc000Common::virEndProc();
}

// midori 161106 add -->
void CfrmUc051Tanaorosi::ComboNew(short index)
{
	short			sv_sel=0;
// midori 152201 add -->
	short			sv_sel2=0;		// 変更前のコンボボックスの値を取得
// midori 152201 add <--
	int				sv_cnt=0;
	int				sv_index=0;
	int				intCol=0;
	DIAGRAM_DATA	diadata;

	m_F9OnSw = FALSE;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	sv_index = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );
	// 「種類」列ならコンボボックスのオフセット値取得
	if ( m_uInfo.intCurCol == ID_COL_051_KAMOKU ){
		m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );

		if(diadata.data_combo > m_CmbCnt)	{
			// 実際に登録された科目の件数と選択されたIndexが異なる場合は、「新規作成」が選択されたとする
			m_F9OnSw = TRUE;

			sv_sel = diadata.data_combo;
			sv_cnt = m_CmbCnt;
// midori 152201 add -->
			sv_sel2 = GetKnOrder(m_uInfo.intFormSeq,m_uInfo.intCurPage,m_uInfo.intCurRow);
// midori 152201 add <--

			// F9参照
			OnButtonF9();

			m_F9OnSw = FALSE;

			// 選択されたものを再取得
			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			// 何も変わってなかったらキャンセル
			if(sv_sel == diadata.data_combo && sv_cnt == m_CmbCnt)	{
// midori 152201 del -->
				//diadata.data_combo = 0;
				//m_uInfo.OldCombo = 0;
// midori 152201 del <--
// midori 152201 add -->
				// sv_sel2が新規追加の場合、クリアする
				if(sv_sel2 > m_CmbCnt)	sv_sel2 = 0;

				diadata.data_combo = sv_sel2;
				m_uInfo.OldCombo = sv_sel2;
// midori 152201 add <--
				m_pDiag->SetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			}
// midori 152134 del -->
//			m_pDiag->SetPosition( sv_index );
// midori 152134 del <--
// midori 152134 add -->
			PostMessage(WM_USER_FOCUSSET,0,0);
// midori 152134 add <--
		}
	}
}
// midori 161106 add <--

// midori 151405 add -->
int CfrmUc051Tanaorosi::CursorControl(short nChar, int sw)
{
	int				rv=0;
	int				intCol=0;
	int				pos=0;
// midori 152134 17/10/07 add -->
	DIAGRAM_DATA	diadata;
// midori 152134 17/10/07 add <--

	// 現在の列番号を取得
	intCol = CmnGetControlCol();

// 修正No.158296 del -->
//// midori 152134 17/10/07 add -->
//	// 例外処理：移動前に【新規作成】を選び、Tabキーを押下された場合は値を元に戻す
//	if(nChar == VK_TAB)	{
//		if (intCol == ID_COL_051_KAMOKU) {
//			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
//			if(diadata.data_combo > m_CmbCnt)	{
//				m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
//				CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
//			}
//		}
//	}
//// midori 152134 17/10/07 add <--
// 修正No.158296 del <--
// 修正No.158296 add -->
	// 例外処理：移動前に【新規作成】を選び、Tabキー、左矢印キー、右矢印キーを押下された場合は値を元に戻す
	if(nChar == VK_TAB || nChar == VK_LEFT || nChar == VK_RIGHT)	{
		if (intCol == ID_COL_051_KAMOKU) {
			int		sw = 0;
			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			if(diadata.data_combo > m_CmbCnt)	{
				sw = 1;
			}
			else if(diadata.data_combo != m_uInfo.OldCombo)	{
				if(nChar == VK_LEFT || nChar == VK_RIGHT)	{
					sw = 1;
				}
			}
			if(sw == 1)	{
				m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
				CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
			}
		}
	}
// 修正No.158296 add <--

// 修正No.158296 del -->
	//// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
	//if ((nChar == VK_LEFT) || (nChar == VK_RIGHT)) {
	//	// ComboBoxは選択内容を確定させない
	//	if (intCol == ID_COL_051_KAMOKU) {
	//		m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
	//		CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
	//	}
	//}
// 修正No.158296 del <--

	// フォーカス移動先を取得
	int intRet = GetFocusPosition(nChar);
	switch (intRet) {
	case -1:		// 先頭項目でShift+Tabキーのため、タイトルへ移動
	case -4:		// 先頭行で↑or先頭項目で←は、タイトルへ移動
		m_TitleDiag.SetPosition(0);
		rv = 1;
		break;
	case -2:		// 最終項目でTabキーのため、頁コントロールへ移動
		m_PageDiag.SetPosition(1);
		rv = 1;
		break;
	case -3:		// スクロールで無く、ComboBoxで↑↓キーを押された場合何もしない
		if(nG_MScroll == 1)	{
			pos = m_pDiag->GetPosition();
			m_pDiag->SetPosition(pos);
			rv = 1;
		}
		break;
	default:		// フォーカス移動を行う
		m_pDiag->SetPosition(intRet);
		rv = 1;
		break;
	}

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	if(sw == 1)	nG_MScroll = 0;

	return(rv);
}
// midori 151405 add <--

// midori 152134 add -->
void CfrmUc051Tanaorosi::ComboSelYoushikidiag1(short index, LPUNKNOWN data)
{
// 要通知案件修正 21/07/09 del -->
	//char	key=0x00;		// →矢印キー状態取得
	//char	key2=0x00;		// Enterキー状態取得

	//key = ( char )GetKeyState( VK_RIGHT );				//	→矢印キー状態取得
	//key2 = ( char )GetKeyState( VK_RETURN );			//	→Enterキー状態取得

	//m_ComboSgn = 0;
	//if((key & 0x80) != 0x80 && (key2 & 0x80) != 0x80)	{
	//	if(m_uInfo.intCurCol == ID_COL_051_KAMOKU)	{
	//		ComboNew(index);
	//		m_ComboSgn = 1;
	//	}
	//}
// 要通知案件修正 21/07/09 del <--
// 要通知案件修正 21/07/09 add -->
	char	key1 = 0x00;		// Enterキー状態取得
	char	key2 = 0x00;		// マウス左クリック状態取得
// 修正No.158262 add -->
	char	key3 = 0x00;		// 右矢印キー状態取得
// 修正No.158262 add <--

	key1 = (char)GetKeyState( VK_RETURN );			// Enterキー状態取得
	key2 = (char)GetAsyncKeyState(VK_LBUTTON);		// マウス左クリック状態取得
// 修正No.158262 add -->
	key3 = (char)GetKeyState( VK_RIGHT );			// 右矢印キー状態取得
// 修正No.158262 add <--

	m_ComboSgn = 0;
// 修正No.158262 add -->
	// 右矢印キーで来た時は抜ける
	if((key3 & 0x80) == 0x80)	{
		return;
	}
// 修正No.158262 add <--
	// マウスクリックされた時のみ
	// Enter押下の処理はEditOFFでしているのでここでは入れないようにする
	if( ((key1 & 0x80) != 0x80) && ((key2 & 0xff) == 0x01) )	{
		if(m_uInfo.intCurCol == ID_COL_051_KAMOKU) {
			ComboNew(index);
			m_ComboSgn = 1;
		}
	}
// 要通知案件修正 21/07/09 add <--
}
// midori 152134 add <--

// No.200601 add -->
// ------------------------------------------------------------------------------------------------------------------------
// 全頁の電子申告の桁数チェックを行い、タイトルの文字色を変更する
// ------------------------------------------------------------------------------------------------------------------------
void CfrmUc051Tanaorosi::TitleColorChg(void)
{
	int					nCharOver[4]={0};	// 電子申告の文字数を超えたかどうかのサイン		1:電子申告の文字数を超えた
											// 様式①	[0]:金融機関名(11文字)	[1]:支店名(11文字)
											//			[2]:種類(10文字)		[3]:摘要(30文字)
	CRecordset			rs(m_pDB);
	CString				strSQL=_T("");
	CString				strData=_T("");

	// 科目
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(KnName,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_051_tanaoroshi where FgShow=%d) tbl1"),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_051_tanaoroshi where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("KnName")),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[0] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[0] = 1;
	}

	// 品目
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(HdName,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_051_tanaoroshi where FgShow=%d) tbl1"),UC_051_HINMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_051_tanaoroshi where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("HdName")),UC_051_HINMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[1] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[1] = 1;
	}

	// 摘要
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(Teki,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_051_tanaoroshi where FgShow=%d) tbl1"),UC_051_TEKI_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_051_tanaoroshi where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("Teki")),UC_051_TEKI_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[2] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[2] = 1;
	}

	// 項目タイトル(ヘッダ部)の再設定
// 修正No.157626 del -->
	//CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科  目(10)"),nCharOver[0]);
	//CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("品  目(20)"),nCharOver[1]);
	//CmnHeadDiagSetTitle(&m_HeadDiag,6,_T("摘  要(30)"),nCharOver[2]);
// 修正No.157626 del <--
// 修正No.157626 add -->
	if(nCharOver[0] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科  目(10)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科　目"),0);

	if(nCharOver[1] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("品  目(20)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("品　目"),0);

	if(nCharOver[2] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,6,_T("摘  要(30)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,6,_T("摘　要"),0);

	// 欄外ガイドの表示
	if(nCharOver[0] == 1 || nCharOver[1] == 1 || nCharOver[2] == 1)	{
		m_Guide1.ShowWindow(TRUE);
	}
	else {
		m_Guide1.ShowWindow(FALSE);
	}
// 修正No.157626 add <--
}
// No.200601 add <--

// 修正No.162446 add -->
LRESULT CfrmUc051Tanaorosi::OnUserReDrawView(WPARAM wParam, LPARAM lParam)
{
	virUpdateControlTblData();
	CmnDiagSetFocus(m_pDiag, 1, 1);

	return(0);
}
// 修正No.162446 add <--
