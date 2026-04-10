// TaxationList31View.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "TaxationList31View.h"

#include "ShinJigyo31.h"
// kasai add -->
#include "ShinJigyo31Ex.h"
// kasai add <--
// 2023/07/11 h.wada add -> 
#include "ShinJigyo31Ex2.h"
// h.wada add <-
// 2023/07/14 tanaka add -> 
#include "ShinNougyou31Ex2.h"
// tanaka add <-
#include "ShinFudousan31.h"
#include "ShinNougyou31.h"
// 211129_h add -->
#include "ShinFudousan31Ex.h"
#include "ShinFudousan31Ex2.h"
#include "ShinNougyou31Ex.h"
// 211129_h add <--
#include "ShinSell31.h"
#include "ShinBuy31.h"
// kasai add -->
#include "ShinSell31Ex.h"
#include "ShinBuy31Ex.h"
// kasai add <--
// tanaka add -->
#include "ShinSell31Ex2.h"
#include "ShinBuy31Ex2.h"
// tanaka add <--
#include "ShinKniinp31.h"
// 211201_h add -->
#include "ShinKniinp31Ex.h"
// 211201_h add <--
#include "TaxtionSelect.h"
#include "KoketsuMeisyo.h"
#include "KoketsuKingaku.h"

// 特定収入
#include "SpecPassOffEx.h"
#include "SpecPassOffEx2.h"		// 24/02/19wd_特定収入対応 add

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include "afxdialogex.h"

extern HINSTANCE g_hInstance;



// CTaxationList31View ダイアログ

IMPLEMENT_DYNCREATE(CTaxationList31View, ICSFormView)

CTaxationList31View::CTaxationList31View()
	: CBaseTaxHyoView(CTaxationList31View::IDD)
	, m_pShinJigyo31 ( NULL )
	// kasai add -->
	, m_pShinJigyo31Ex ( NULL )
	// kasai add <--
	// 2023/07/11 h.wada add -> 
	, m_pShinJigyo31Ex2 ( NULL )
    // h.wada add <-
	, m_pShinFudousan31 ( NULL)
	, m_pShinNougyou31 ( NULL )
// 211129_h add -->
	, m_pShinFudousan31Ex ( NULL)
	, m_pShinNougyou31Ex ( NULL )
// 211129_h add <--
// 2023/07/14 tanaka add -> 
	, m_pShinNougyou31Ex2(NULL)
	, m_pShinFudousan31Ex2(NULL)
// tanaka add <-
	, m_pShinSell31 ( NULL )
	, m_pShinBuy31 ( NULL )
	// kasai add -->
	, m_pShinSell31Ex ( NULL )
	, m_pShinBuy31Ex ( NULL )
	// kasai add <--
	// tanaka add -->
	, m_pShinSell31Ex2( NULL )
	, m_pShinBuy31Ex2( NULL )
	// tanaka add <--
	, m_pShinKniinp31( NULL )
// 211201_h add -->
	, m_pShinKniinp31Ex( NULL )
// 211201_h add <--
	, m_pSnHeadData ( NULL )
	, m_pTaxListData( NULL )
	, m_pShzCalc( NULL )
	, m_pZmsub ( NULL )
	, m_SnSeq ( -1 )
	, m_SvSnSeq ( -1 )
	, m_nowTabIdx(0)
	, m_pFirstPage(NULL)
	, m_pShinSpecific131 ( NULL )
	, m_pShinSpecific131Ex( NULL )
	, m_pShinSpecific231 ( NULL )
	, m_pShinSpecific231Ex( NULL )
	, m_pShinSpecific2231 ( NULL )
	, m_pShinSpecific22_2_31 ( NULL )
	, m_pShinSpecific22_2_31Ex( NULL )
	, m_pShinSpecific3431 ( NULL )
	, m_pShinSpecific5131 ( NULL )
	, m_pShinSpecific5131Ex( NULL )
	, m_pShinSpecific5131Ex2( NULL ) // 20231124
	, m_pShinSpecific5231 ( NULL )
	, m_pShinSpecific5231Ex( NULL )
	, m_pShinSpecific5231Ex2( NULL ) // 20231128
	, m_pShinSpecific5331 ( NULL )
	, m_pShinSpecific5331Ex( NULL )
	, m_pShinSpecific5331Ex2( NULL )// 20231128
	, m_pShinSpecific52131(NULL)// 20231128
	, m_pShinSpecific52231(NULL)// 20231128
	, m_pShinSpecific523131(NULL)// 20231128
	, m_pShinSpecific523231(NULL)// 20231128
	, m_pShinSpecific523331(NULL)// 20231128
	, m_pShinSpecific524131(NULL)// 20231128
	, m_pShinSpecific524231(NULL)// 20231128
	, m_pShinSpecific524331(NULL)// 20231128
	, m_pSpcListData ( NULL )
	, m_isLinkingSpcToHonpyo ( FALSE )
	, m_SixKindFlg ( TRUE )
	, m_DispTabSgn( 0 )
{
	m_Arith.l_defn( 0x16 );
	memset( m_ListType, '\0', sizeof(m_ListType) );
}

CTaxationList31View::~CTaxationList31View()
{
}

void CTaxationList31View::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_MainTab);
}


BEGIN_MESSAGE_MAP(CTaxationList31View, ICSFormView)
	ON_COMMAND(ID_BUTTON_F2, OnButtonF2)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_BUTTON_F7, OnButtonF7)
	ON_COMMAND(ID_BUTTON_F8, OnButtonF8)
	ON_COMMAND(ID_BUTTON_F9, &CTaxationList31View::OnButtonF9)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)
	ON_COMMAND(ID_BUTTON_F12, OnButtonF12)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CTaxationList31View::OnUpdateButtonF7)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, &CTaxationList31View::OnUpdateButtonF8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CTaxationList31View::OnUpdateButtonF9)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, &CTaxationList31View::OnUpdateButtonF11)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, &CTaxationList31View::OnUpdateButtonF12)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, OnTcnSelchangeMainTab)
	ON_MESSAGE(WM_USER_SHINTAB_INIT31, OnUserShintabInit)
	ON_UPDATE_COMMAND_UI(ID_KOMONSAKI_INF, &CTaxationList31View::OnUpdateKomonsakiInf)
	ON_MESSAGE( WM_MYMESSAGE31, OnMyMessage )
	ON_WM_SIZE()
END_MESSAGE_MAP()


#ifdef _DEBUG
void CTaxationList31View::AssertValid() const
{
	ICSFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTaxationList31View::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CTaxationList31View メッセージ ハンドラー

void CTaxationList31View::OnButtonF2()
{
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		if( m_TabMng[selno].pWnd ){
			m_TabMng[selno].pWnd->PostMessage( WM_KEYDOWN, VK_F2 );
		}
	}
}

void CTaxationList31View::OnButtonF4()
{
	if( m_pShinInfo && m_pShinInfo->pBaseView ){
		// データ更新
		m_pShinInfo->pBaseView->PostMessage( WM_COMMAND, ID_BUTTON_F4 );
	}
}

void CTaxationList31View::OnButtonF5()
{
	if( m_pShinInfo && m_pShinInfo->pBaseView ){
		// データ更新
		m_pShinInfo->pBaseView->PostMessage( WM_COMMAND, ID_BUTTON_F5 );
	}
}

void CTaxationList31View::OnButtonF7()
{
// 修正No.168722 cor -->
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( m_pShinInfo && m_pShinInfo->pBaseView ){
			CString	str;
			int		ans;
			// 処理確認
			str  = _T("課税取引金額計算表のデータがクリアされます。\n");
			str += _T("処理を継続しますか？");
			ans = ICSMessageBox( str, (MB_YESNO | MB_DEFBUTTON2) );
			if( ans == IDNO ){
				return;
			}
			if(m_pShzCalc)	{
				m_pShzCalc->Shz_DelData();
			}
			IndicateTasList( m_MainTab.GetCurSel() );
		}
	}
// ---------------------
	//if(m_SelType == ID_ICSSH_TAXLIST_TYPE || m_SelType == ID_ICSSH_SPCLIST_TYPE) {
	//	if(m_pShinInfo && m_pShinInfo->pBaseView) {
	//		CString	str;
	//		int		ans;
	//		int		rendo;

	//		// 処理確認
	//		if(m_SelType == ID_ICSSH_TAXLIST_TYPE)	{
	//			// 課税取引金額計算表
	//			str = _T("課税取引金額計算表のデータがクリアされます。\n");
	//			str += _T("処理を継続しますか？");
	//		}
	//		else	{
	//			// 特定収入計算表
	//			str = _T("特定収入計算表のデータがクリアされます。\n");
	//			if((m_pSnHeadData->Sn_Sign4 & 0x80) || (m_Util.IsUnConnectMst(m_pZmsub->zvol)))	{	// 非連動
	//				rendo = 1;
	//			}
	//			else	{
	//				rendo = 0;
	//				str += _T("※連動されている金額はクリアされません。\n");
	//			}
	//			str += _T("処理を継続しますか？");
	//		}
	//		ans = ICSMessageBox(str, (MB_YESNO | MB_DEFBUTTON2));
	//		if(ans == IDNO) {
	//			return;
	//		}

	//		if(m_SelType == ID_ICSSH_TAXLIST_TYPE) {
	//			if(m_pShzCalc) {
	//				m_pShzCalc->Shz_DelData();
	//			}
	//			IndicateTasList(m_MainTab.GetCurSel());
	//		}
	//		else {
	//			if(m_pSpcListData) {
	//				m_pSpcListData->Spc_DelData(rendo);
	//			}
	//			IndicateTasListSpc(m_MainTab.GetCurSel());
	//		}
	//		
	//	}
	//}
// 修正No.168722 cor <--
}

void CTaxationList31View::OnButtonF8()
{
	CString str;
	str = _T( "消費税申告書へ転記しますか？\n" );
	if( ICSMessageBox ( str, ( MB_YESNO | MB_DEFBUTTON2 ) ) != IDYES ){
		return;
	}

	if ( m_pShzCalc->Shz_ReportPost() == -1 ) {
		return;
	}

	str = _T( "消費税申告書への転記が終了しました。\n\n" );
	str += _T( "申告書・付表の画面を開いて転記した金額を確認して下さい。\n" );
	str += _T( "また不足する項目については手入力して下さい。\n" );
	ICSMessageBox ( str );

	// 本表に転記
	m_pShinInfo->pBaseView->PostMessageA ( WM_USER_TENKI31, ( WPARAM )0 );
}

void CTaxationList31View::OnButtonF9()
{

	if( m_pSnHeadData == NULL ){
		return;
	}
	else if( (m_pShinInfo==NULL) || (m_pShinInfo->pBaseView==NULL) || (m_pShinInfo->pToolBar==NULL) ){
		return;
	}

	// 入力データ保存
	unsigned char	svSn_Sign4 = m_pSnHeadData->Sn_Sign4;

	if( m_pSnHeadData->DoUnlock(m_pZmsub, this) == IDNO ){
		return;
	}

	m_pShinInfo->pParent->PostMessage( WM_USER_UPDATE_INDICATE31, 0 );
	m_pShinInfo->pBaseView->SendMessage( WM_USER_UNLOCK31, (WPARAM)0 );

	if( (svSn_Sign4!=m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4&0x01) ){

		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
			WriteData( FALSE , FALSE );
		}
		else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
			WriteSpcData( FALSE );
		}

	}

	char*		p;
	if( m_pSnHeadData->Sn_Sign4&0x01 ){
		p = _T("F9 ロック解除");
	}
	else{
		p = _T("F9 入力ロック");
	}
	m_pShinInfo->pToolBar->SetButtonTooltip( 7, p );
	m_pShinInfo->pToolBar->SetButtonText( 7, p );

	int	selno = m_MainTab.GetCurSel();
	if( (svSn_Sign4!=m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4&0x01) ){
		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
			WriteData( FALSE , FALSE );
		}
		else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
			WriteSpcData( FALSE );
		}

	}

	m_pShinInfo->pBaseView->SendMessage ( WM_USER_SYZINIT31, ( WPARAM )0 );

	// タブ初期化
	InitTab ( TRUE );

	m_MainTab.SetCurSel ( selno );
	m_MainTab.ModifyStyle ( TCS_SINGLELINE, TCS_MULTILINE );

	if ( m_SelType == ID_ICSSH_TAXLIST_TYPE ) {
		SetItemPosition ( selno );
	}
	else if ( m_SelType == ID_ICSSH_SPCLIST_TYPE ) {
		SetItemPositionEx ( selno );

	}

}

void CTaxationList31View::OnButtonF11()
{
	if ( m_SelType == ID_ICSSH_TAXLIST_TYPE ) {
		return;
	}
	//特定収入時のみ
	int selno = m_MainTab.GetCurSel();

	WriteSpcData ( FALSE );

	if ( m_pShinInfo && m_pShinInfo->pParent ){
		// 再集計
		m_pShinInfo->pParent->SendMessage ( WM_USER_RECALQ31, ( WPARAM )1 );
	}
	// 再表示
	ReInitSpcDisp();
	m_MainTab.SetCurSel ( selno );
	m_MainTab.ModifyStyle ( TCS_SINGLELINE, TCS_MULTILINE );
	SetItemPositionEx ( selno );
}

void CTaxationList31View::OnButtonF12()
{
	PageChange(1);	// 1:次へ
}

void CTaxationList31View::OnUpdateButtonF7(CCmdUI *pCmdUI)
{
	int Flg = 0;
// 修正No.168722 del -->
	if ( m_SelType == ID_ICSSH_SPCLIST_TYPE ) {
		// 特定収入では初期化ボタンを非表示に
		pCmdUI->Enable ( FALSE );
	}
	else {
// 修正No.168722 del <--
		if ( ( m_pSnHeadData->Sn_Sign4 & 0x01 ) == 0x00 ) {
			Flg = 1;
		}

		if ( Flg ) {
			pCmdUI->Enable ( TRUE );
		}
		else {
			pCmdUI->Enable ( FALSE );
		}
// 修正No.168722 del -->
	}
// 修正No.168722 del <--
}

void CTaxationList31View::OnUpdateButtonF8(CCmdUI *pCmdUI)
{
	if (m_pSnHeadData == NULL) {
		return;
	}
	else if ((m_pShinInfo == NULL) || (m_pShinInfo->pParent == NULL)) {
		return;
	}



	int Flg = 0;
	ICSToolBar *pBar = ( ( ICSMainFrame* )m_pShinInfo->pParent )->GetToolBar ( 1 );

	if ( pBar ) {
		char *p;
		p = _T( "F8 申告書転記" );
		
		pBar->SetButtonTooltip ( 6, p );
		pBar->SetButtonText ( 6, p );

		if ( m_SelType == ID_ICSSH_TAXLIST_TYPE ) {
			//yoshida190824
			//特例使用時は申告書転記ボタンは無効化
			if (m_pSnHeadData->m_DispTabSgn & 0x80) {//特例使用時無効化
				Flg = 0;
			}
			else {
				if ((m_Util.IsUnConnectMst(m_pZmsub->zvol)) || (m_pSnHeadData->Sn_Sign4 & 0x80)) {
					if (m_pSnHeadData->Sn_Sign4 & 0x01) {
						Flg = 0;
					}
					else {
						Flg = 1;
					}
				}
				else {
					Flg = 0;
				}
			}
		}
	}


	pCmdUI->Enable ( Flg );
}

void CTaxationList31View::OnUpdateButtonF9(CCmdUI *pCmdUI)
{
	if ( m_pSnHeadData == NULL ) {
		return;
	}
	else if ( ( m_pShinInfo == NULL ) || ( m_pShinInfo->pParent == NULL ) ) {
		return;
	}

	ICSToolBar *pBar = ( ( ICSMainFrame* )m_pShinInfo->pParent )->GetToolBar ( 1 );
	if ( pBar ) {
		char *p;
		if ( m_pSnHeadData->Sn_Sign4 & 0x01 ) {
			p = _T( "F9 ロック解除" );
		}
		else {
			p = _T( "F9 入力ロック" );
		}

		pBar->SetButtonTooltip ( 7, p );
		pBar->SetButtonText ( 7, p );

		return;
	}
}

void CTaxationList31View::OnUpdateButtonF11(CCmdUI *pCmdUI)
{
	BOOL bON = FALSE;

	if ( m_SelType == ID_ICSSH_SPCLIST_TYPE ) {
		if ( pCmdUI ) {
			if ( m_pSnHeadData ) {
				if ( ( m_pSnHeadData->Sn_Sign4 & 0x81 ) == 0x00 ) {
					bON = TRUE;
				}
				if ( m_pSnHeadData->IsMiddleProvisional() ) {
					bON = FALSE;
				}
				if ( bON ) {
					if ( m_SnSeq ) {
						bON = FALSE;
					}
				}
			}
			pCmdUI->Enable ( bON );
		}
	}
	else if ( m_SelType == ID_ICSSH_TAXLIST_TYPE ) {
		if ( pCmdUI ) {
			pCmdUI->Enable ( FALSE );
		}
	}
}

void CTaxationList31View::OnUpdateButtonF12(CCmdUI *pCmdUI)
{
	if ( m_MainTab.GetItemCount() == 1 ) {
		pCmdUI->Enable( FALSE );
	}
	else {
		pCmdUI->Enable( TRUE );
	}
}

void CTaxationList31View::OnSize(UINT nType, int cx, int cy)
{

	this->LockWindowUpdate();

	ICSFormView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。

	if( m_TabMng.GetCount() && m_pFirstPage ){

//		RECT rect = { 0 };
//		m_pFirstPage->GetClientRect( &rect );

		int max = (int)m_TabMng.GetCount();
//		for( int i=0; i<max; i++ ){
//			if( m_TabMng[i].pWnd ){
//				m_TabMng[i].pWnd->MoveWindow( &rect );
//			}

//		}
		RECT rect1, rect2;
		CWnd *pWnd = (CWnd *)&m_MainTab;
		pWnd->GetClientRect( &rect1 );					// ｸﾗｲｱﾝﾄ領域の取得
		m_MainTab.AdjustRect( FALSE, &rect1 );			// ﾀﾌﾞｺﾝﾄﾛｰﾙの表示領域の計算
		rect2.left   = rect1.left;
		rect2.top    = rect1.top;
		rect2.bottom = rect1.bottom;
		rect2.right  = rect1.right;
		for( int i=0; i<max; i++ ){
			if( m_TabMng[i].pWnd ){
				m_TabMng[i].pWnd->MoveWindow( &rect2 );	// ダイアログの移動
			}
		}

		GetClientRect( vwRect );
		CRect	tabRect;
		m_MainTab.GetClientRect(tabRect);

		SCROLLINFO	siV = {0}, siH = {0};
		siV.cbSize = sizeof(SCROLLINFO);
		GetScrollInfo(SB_VERT, &siV );
		tabRect.top = tabRect.top - siV.nPos;
		if( vwRect.Height() < siV.nMax ) {
			tabRect.bottom = siV.nMax;
		}
		else {
			tabRect.bottom = vwRect.bottom;
		}

		GetScrollInfo(SB_HORZ, &siH );
		tabRect.left -= siH.nPos;
		tabRect.right -= siH.nPos;

		m_MainTab.MoveWindow( tabRect );

	}

	this->UnlockWindowUpdate();
}

//==========================================
// 顧問先情報の取り込み
//==========================================
void CTaxationList31View::OnUpdateKomonsakiInf(CCmdUI *pCmdUI)
{
	//オプションの顧問先情報の取込は無効にする
	pCmdUI->Enable ( FALSE );
}

//==========================================
// 再初期化
//==========================================
LRESULT CTaxationList31View::OnUserShintabInit( WPARAM wParam, LPARAM lParam ) 
{
	InitTabMain ( TRUE );
	return 0;
}

LRESULT CTaxationList31View::OnMyMessage( WPARAM wParam, LPARAM lParam )
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){//課税取引
		SetItemPosition( m_MainTab.GetCurSel() );
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){//特定収入
		SetItemPositionEx( m_MainTab.GetCurSel() );
	}

	AfxSetResourceHandle( hInst );

	return 0;
}

void CTaxationList31View::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();

	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	SetResize( IDC_MAIN_TAB, ICS_RESIZE_BOTTOM );
}

void CTaxationList31View::OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result)
{
	int selno = m_MainTab.GetCurSel();

	if ( m_SelType == ID_ICSSH_TAXLIST_TYPE ) {
		if ( m_nowTabIdx != selno ) {
			IndicateTasList ( selno );
			SetItemPosition ( selno );

			m_nowTabIdx = selno;
		}
		else {
			SetItemPosition ( selno );
		}
	}
	else if ( m_SelType == ID_ICSSH_SPCLIST_TYPE ) {
		ReInitSpcDisp ( selno );
		SetItemPositionEx ( selno );
	}
}

BOOL CTaxationList31View::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			if ( m_pShinInfo && m_pShinInfo->pParent ) {
				m_pShinInfo->pParent->SendMessage ( WM_USER_SET_ENDSTART31, 0 );
				m_pShinInfo->pParent->PostMessageA ( WM_CLOSE, 0 );
				return TRUE;
			}
		}
		else if ( pMsg->wParam == VK_RETURN ) {
			if ( m_TabMng.GetCount() ) {
				if ( m_SelType == ID_ICSSH_TAXLIST_TYPE ) {
					SetItemPosition ( m_MainTab.GetCurSel() );
				}
				else if ( m_SelType == ID_ICSSH_SPCLIST_TYPE ) {
					SetItemPositionEx ( m_MainTab.GetCurSel() );
				}

				return TRUE;
			}
		}
	}
	else if ( pMsg->message == WM_MOUSEWHEEL ) {
		if ( pMsg->hwnd == GetSafeHwnd() ) {
			CWnd *pWnd = WindowFromPoint ( pMsg->pt );
			if ( pWnd && IsChild ( pWnd ) ) {
				pMsg->hwnd = pWnd->GetSafeHwnd();
			}
		}
	}

	return ICSFormView::PreTranslateMessage(pMsg);
}

//==========================================
// 初期化
//------------------------------------------
// 引数
//		pZmSub		:		公益クラス
//		pShinInfo	:		共通情報
//		pSnHeadData	:		消費税ヘッダ
//		pSyzSyukei	:		集計クラス
//------------------------------------------
// 返送値
//		0			:		正常終了
//==========================================
int CTaxationList31View::InitView ( CDBNpSub *pZmsub, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei )
{

	ASSERT ( pZmsub );
	if ( pZmsub == NULL ) {
		return -1;
	}
	m_pZmsub = pZmsub;

	ASSERT ( pShinInfo );
	if ( pShinInfo == NULL ) {
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

	// 内部タブの初期化
	InitTab ( TRUE );

	return 0;
}

//==========================================
// 内部タブの初期化
//------------------------------------------
// 返送値
//		0			:		正常終了
//==========================================
int CTaxationList31View::InitTab ( BOOL bDelete )
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle ( g_hInstance );

	// コンバージョンが必要かどうか
	//BOOL IsNeedCnv = m_pTaxListData->IsNeedOldDataCnv( m_pZmsub, m_SnSeq );
	BOOL IsNeedCnv = TRUE;
	if ( m_pSnHeadData->Sn_GenericSgn & 0x02 ) {
		IsNeedCnv = FALSE;
	}
	BOOL IsNeedCnvInv = TRUE;
	if (m_pSnHeadData->Sn_GenericSgn & 0x2000) {//230919
		IsNeedCnvInv = FALSE;
	}

	this->LockWindowUpdate();
	int ret = InitTabMain ( bDelete );
	this->UnlockWindowUpdate();

	AfxSetResourceHandle ( hInst );

	// コンバージョン
	if( IsNeedCnv ){
		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
			m_pTaxListData->GetPastData( m_pZmsub, m_pSnHeadData, m_SnSeq );
			m_pTaxListData->SetData( m_pZmsub, m_pSnHeadData, m_SnSeq );
			ReInitTaxDisp();
		}
		else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
			;	// 特定収入計算表ではデータの引継ぎは必要ない
		}
		IsNeedCnv = FALSE;
		// コンバージョンサインON
		m_pSnHeadData->Sn_GenericSgn |= 0x02;
	}

	if (IsNeedCnvInv) {//230919
		long	eymd = 0;
		if (!(m_pSnHeadData->Sn_SKKBN % 2)) eymd = m_pSnHeadData->Sn_MDAYE;
		else								eymd = m_pSnHeadData->Sn_KDAYE;

		// SetData2への移行後であればコンバージョン
	//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
	//	if (eymd >= TAX_R05_EDAY) {
		if (eymd >= ICS_SH_INVOICE_DAY) {
			if (m_SelType == ID_ICSSH_TAXLIST_TYPE) {
				m_pTaxListData->GetPastData2(m_pZmsub, m_pSnHeadData, m_SnSeq);
				m_pTaxListData->SetData2(m_pZmsub, m_pSnHeadData, m_SnSeq);
				ReInitTaxDisp();
			}
			else if (m_SelType == ID_ICSSH_SPCLIST_TYPE) {
				;	// 特定収入計算表
			}
			IsNeedCnvInv = FALSE;
			// コンバージョンサインON
			m_pSnHeadData->Sn_GenericSgn |= 0x2000;
		}
	}

	return ret;
}

//==========================================
// 内部タブの初期化 ( メイン )
//------------------------------------------
// 引数
//		bDelete		:		内部を消去するか
//------------------------------------------
// 返送値
//		0			:		正常終了
//==========================================
int CTaxationList31View::InitTabMain ( BOOL bDelete )
{
	int ret = 0;

	ASSERT ( m_pSnHeadData );
	if ( m_pSnHeadData == NULL ) {
		return -1;
	}

	if ( m_Util.IsBasicMst ( m_pZmsub->zvol ) ) {
		m_SelType = ID_ICSSH_TAXLIST_TYPE;
		ret = InitTabSub ( bDelete );
	}
	else if ( m_Util.IsSpcMaster ( m_pZmsub ) ) {
		m_SelType = ID_ICSSH_SPCLIST_TYPE;
		ret = InitSpecificTab ( bDelete );
	}

	return ret;
}

//==========================================
// 内部タブの初期化
//------------------------------------------
// 引数
//		bDelete		:		内部を消去するか
//------------------------------------------
// 返送値
//		0			:		正常終了
//==========================================
int CTaxationList31View::InitTabSub ( BOOL bDelete )
{
	if ( bDelete ) {
		// 内部クラス破棄
		DeleteInsideDlg();
		// 全削除
		m_MainTab.DeleteAllItems();
	}

	SHIN_TABMAX = 0;
	ICSDialog *ptmpTab[16] = { 0 };
	SH_TABMNG sh_Tabmng;
	int tabIdx = 0;

	for ( int idx = 0; idx < ICSSH_TAXLIST31_TAB_MAX; idx++ ) {
		m_ListType[idx] = 0;
	}

	// kasai add -->
	BOOL syzSgn = FALSE;
// 211203h del -->
	//if( m_pSnHeadData->SVmzsw == 1 ) {
// 211203h del <--
// 211203h add -->
	long	eymd=0;
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	{
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else	{
		eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if( m_pSnHeadData->SVmzsw == 1 || eymd < TAX_R04_SDAY ) {
// 211203h add <--
		// 旧税率あり
		syzSgn = TRUE;
	}
	// kasai add <--

	// 2023/07/13 h.wada add -->
	BYTE syzSgn2 = 0x00; //D0:令和5年10月1日以降に終了する課税期間用の帳票を使用する
	//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
	//if ( eymd >= TAX_R05_EDAY) {
	if (eymd >= ICS_SH_INVOICE_DAY) {
		syzSgn2 = 0x01;
	}
	// h.wada <--


	// 帳票データ
	if( !m_pTaxListData ){
		if( (m_pTaxListData = new CH31TaxListData()) == NULL ){
			return -1;
		}
		if( m_pTaxListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}

	// 集計クラスの初期化
	if( !m_pShzCalc ){
		if( (m_pShzCalc = new CShzTax31Calc()) == NULL ){
			return -1;
		}
		m_pShzCalc->InitShzCalc( &m_pTaxListData, &m_pSnHeadData, m_pZmsub );
		m_pShzCalc->m_SixKindFlg = m_SixKindFlg;
	}
	
	// 経過措置対象の課税資産の譲渡等あり
	if ( m_Util.IsUnConnectMst ( m_pZmsub->zvol ) && ( m_Util.GetKazeihoushiki ( m_pZmsub->zvol ) == ID_ICSSH_KANNI ) ) {
		// 事業別売上高
// 211201_h del -->
		//if( !m_pShinKniinp31 ){
		//	m_pShinKniinp31 = new CShinKniinp31( this );
		//	m_pShinKniinp31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
		//	m_pShinKniinp31->Create( IDD_DIALOG_JIGYO_SELL31, &m_MainTab );
		//}
		//ptmpTab[tabIdx] = m_pShinKniinp31;
		//m_ListType[tabIdx] = ID_ICSSH_31_BUSINESS_SALES_LIST_TAB;
		//m_MainTab.InsertItem( tabIdx, _T( "事業別売上高" ), m_pShinKniinp31->m_hWnd );
// 211201_h del <--
// 211201_h add -->
		if( syzSgn ){
			// 旧税率あり
			if( !m_pShinKniinp31 ){
				m_pShinKniinp31 = new CShinKniinp31( this );
				m_pShinKniinp31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinKniinp31->Create( IDD_DIALOG_JIGYO_SELL31, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinKniinp31;
			m_ListType[tabIdx] = ID_ICSSH_31_BUSINESS_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T( "事業別売上高" ), m_pShinKniinp31->m_hWnd );
		}
		else	{
			// 旧税率なし
			if( !m_pShinKniinp31Ex ){
				m_pShinKniinp31Ex = new CShinKniinp31Ex( this );
				m_pShinKniinp31Ex->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinKniinp31Ex->Create( IDD_DIALOG_JIGYO_SELL31_ex, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinKniinp31Ex;
			m_ListType[tabIdx] = ID_ICSSH_31_BUSINESS_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T( "事業別売上高" ), m_pShinKniinp31Ex->m_hWnd );
		}
// 211201_h add <--
		tabIdx++;
		SHIN_TABMAX++;

		memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
		sh_Tabmng.tabtype = ID_ICSSH_31_BUSINESS_SALES_LIST_TAB;
// 211201_h del -->
		//sh_Tabmng.pWnd = m_pShinKniinp31;
// 211201_h del <--
// 211201_h add -->
		if(m_pShinKniinp31Ex != 0)	{
			sh_Tabmng.pWnd = m_pShinKniinp31Ex;
		}
		else	{
			sh_Tabmng.pWnd = m_pShinKniinp31;
		}
// 211201_h add <--
		m_TabMng.Add( sh_Tabmng );
		// 課税売上高
// kasai del -->
		/*if( !m_pShinSell31 ){
			m_pShinSell31 = new CShinSell31( this );
			m_pShinSell31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
			m_pShinSell31->Create( IDD_DIALOG_SELL31, &m_MainTab );
		}
		ptmpTab[tabIdx] = m_pShinSell31;
		m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
		m_MainTab.InsertItem( tabIdx, _T( "課税売上高" ), m_pShinSell31->m_hWnd );
		tabIdx++;
		SHIN_TABMAX++;

		memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
		sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
		sh_Tabmng.pWnd = m_pShinSell31;
		m_TabMng.Add( sh_Tabmng );*/
// kasai del <--
		if (syzSgn2 & 0x01) {
			if (!m_pShinSell31Ex2) {
				m_pShinSell31Ex2 = new CShinSell31Ex2(this);
				m_pShinSell31Ex2->InitInfo(&m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith, &m_pShzCalc);
				m_pShinSell31Ex2->Create(IDD_DIALOG_SELL31_ex2, &m_MainTab);
			}
			ptmpTab[tabIdx] = m_pShinSell31Ex2;
			m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
			m_MainTab.InsertItem(tabIdx, _T("課税売上高"), m_pShinSell31Ex2->m_hWnd);
			tabIdx++;
			SHIN_TABMAX++;

			memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
			sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
			sh_Tabmng.pWnd = m_pShinSell31Ex2;
			m_TabMng.Add(sh_Tabmng);
		}
		else {

// kasai add -->
		if( syzSgn ){
			// 旧税率あり
			if( !m_pShinSell31 ){
				m_pShinSell31 = new CShinSell31( this );
				m_pShinSell31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinSell31->Create( IDD_DIALOG_SELL31, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSell31;
			m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T( "課税売上高" ), m_pShinSell31->m_hWnd );
			tabIdx++;
			SHIN_TABMAX++;

			memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
			sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
			sh_Tabmng.pWnd = m_pShinSell31;
			m_TabMng.Add( sh_Tabmng );
		}
		else{
			// 旧税率なし
			if( !m_pShinSell31Ex ){
				m_pShinSell31Ex = new CShinSell31Ex( this );
				m_pShinSell31Ex->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinSell31Ex->Create( IDD_DIALOG_SELL31_ex, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSell31Ex;
			m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T( "課税売上高" ), m_pShinSell31Ex->m_hWnd );
			tabIdx++;
			SHIN_TABMAX++;

			memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
			sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
			sh_Tabmng.pWnd = m_pShinSell31Ex;
			m_TabMng.Add( sh_Tabmng );
		}
	}	
// kasai add <--
	}
	else {
		if ( m_pZmsub->zvol->ind_type & 0x01 ) {	// 一般

// kasai del -->
			// 旧税率あり
			/*if ( !m_pShinJigyo31 ) {
				m_pShinJigyo31 = new CShinJigyo31 ( this );
				m_pShinJigyo31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinJigyo31->Create ( IDD_DIALOG_JIGYO31, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinJigyo31;
			m_ListType[tabIdx] = ID_ICSSH_31_GENERAL_LIST_TAB;
			m_MainTab.InsertItem ( tabIdx, _T( "事業所得用" ), m_pShinJigyo31->m_hWnd );*/
// kasai del <--



// 2023/07/11 h.wada change --> 
			if (syzSgn2 & 0x01) {
				if (!m_pShinJigyo31Ex2) {
					m_pShinJigyo31Ex2 = new CShinJigyo31Ex2(this);
					m_pShinJigyo31Ex2->InitInfo(&m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith, &m_pShzCalc);
					m_pShinJigyo31Ex2->Create(IDD_DIALOG_JIGYO31_ex2, &m_MainTab);
				}
				ptmpTab[tabIdx] = m_pShinJigyo31Ex2;
				m_ListType[tabIdx] = ID_ICSSH_31_GENERAL_LIST_TAB;
				m_MainTab.InsertItem(tabIdx, _T("事業所得用"), m_pShinJigyo31Ex2->m_hWnd);
			}
			else {
// kasai add -->
			if( syzSgn ){
				// 旧税率あり
				if ( !m_pShinJigyo31 ) {
					m_pShinJigyo31 = new CShinJigyo31 ( this );
					m_pShinJigyo31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
					m_pShinJigyo31->Create ( IDD_DIALOG_JIGYO31, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinJigyo31;
				m_ListType[tabIdx] = ID_ICSSH_31_GENERAL_LIST_TAB;
				m_MainTab.InsertItem ( tabIdx, _T( "事業所得用" ), m_pShinJigyo31->m_hWnd );
			}
			else{
				// 旧税率なし
				if ( !m_pShinJigyo31Ex ) {
					m_pShinJigyo31Ex = new CShinJigyo31Ex ( this );
					m_pShinJigyo31Ex->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
					m_pShinJigyo31Ex->Create ( IDD_DIALOG_JIGYO31_ex, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinJigyo31Ex;
				m_ListType[tabIdx] = ID_ICSSH_31_GENERAL_LIST_TAB;
				m_MainTab.InsertItem ( tabIdx, _T( "事業所得用" ), m_pShinJigyo31Ex->m_hWnd );
			}
// kasai add <--
			}
// h.wada change <--
			tabIdx++;
			SHIN_TABMAX++;

			memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
			sh_Tabmng.tabtype = ID_ICSSH_31_GENERAL_LIST_TAB;
// kasai del -->
			//sh_Tabmng.pWnd = m_pShinJigyo31;
// kasai del <--
// 2023/07/11 h.wada change --> 
			if (m_pShinJigyo31Ex2 != 0) {
				sh_Tabmng.pWnd = m_pShinJigyo31Ex2;
			}
// kasai add -->
			else if(m_pShinJigyo31Ex != 0)	{
				sh_Tabmng.pWnd = m_pShinJigyo31Ex;
			}
			else	{
				sh_Tabmng.pWnd = m_pShinJigyo31;
			}
// kasai add <--
//  h.wada change <--
			m_TabMng.Add ( sh_Tabmng );
		}

		if ( m_pZmsub->zvol->ind_type & 0x04 ) {	// 農業
// 211129_h del -->
			//if ( !m_pShinNougyou31 ) {
			//	m_pShinNougyou31 = new CShinNougyou31 ( this );
			//	m_pShinNougyou31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
			//	m_pShinNougyou31->Create ( IDD_DIALOG_NOUGYOU31, &m_MainTab );
			//}
			//ptmpTab[tabIdx] = m_pShinNougyou31;
			//m_ListType[tabIdx] = ID_ICSSH_31_AGRICULTUTRE_LIST_TAB;
			//m_MainTab.InsertItem ( tabIdx, _T( "農業所得用" ), m_pShinNougyou31->m_hWnd );
// 211129_h del <--
// 211129_h add -->
			if (syzSgn2 & 0x01) {
				if (!m_pShinNougyou31Ex2) {
					m_pShinNougyou31Ex2 = new CShinNougyou31Ex2(this);
					m_pShinNougyou31Ex2->InitInfo(&m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith, &m_pShzCalc);
					m_pShinNougyou31Ex2->Create(IDD_DIALOG_NOUGYOU31_ex2, &m_MainTab);
				}
				ptmpTab[tabIdx] = m_pShinNougyou31Ex2;
				m_ListType[tabIdx] = ID_ICSSH_31_AGRICULTUTRE_LIST_TAB;
				m_MainTab.InsertItem(tabIdx, _T("農業所得用"), m_pShinNougyou31Ex2->m_hWnd);
			}
			else {

			if( syzSgn )	{
				// 旧税率あり
				if ( !m_pShinNougyou31 ) {
					m_pShinNougyou31 = new CShinNougyou31 ( this );
					m_pShinNougyou31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
					m_pShinNougyou31->Create ( IDD_DIALOG_NOUGYOU31, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinNougyou31;
				m_ListType[tabIdx] = ID_ICSSH_31_AGRICULTUTRE_LIST_TAB;
				m_MainTab.InsertItem ( tabIdx, _T( "農業所得用" ), m_pShinNougyou31->m_hWnd );
			}
			else	{
				// 旧税率なし
				if ( !m_pShinNougyou31Ex ) {
					m_pShinNougyou31Ex = new CShinNougyou31Ex ( this );
					m_pShinNougyou31Ex->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
					m_pShinNougyou31Ex->Create ( IDD_DIALOG_NOUGYOU31_ex, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinNougyou31Ex;
				m_ListType[tabIdx] = ID_ICSSH_31_AGRICULTUTRE_LIST_TAB;
				m_MainTab.InsertItem ( tabIdx, _T( "農業所得用" ), m_pShinNougyou31Ex->m_hWnd );
			}
		}	
// 211129_h add <--
			tabIdx++;
			SHIN_TABMAX++;

			memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
			sh_Tabmng.tabtype = ID_ICSSH_31_AGRICULTUTRE_LIST_TAB;
// 211129_h del -->
			//sh_Tabmng.pWnd = m_pShinNougyou31;
// 211129_h del <--

// 2023/07/14 tanaka change --> 
			if (m_pShinNougyou31Ex2 != 0) {
				sh_Tabmng.pWnd = m_pShinNougyou31Ex2;
			}
// 211129_h add -->
			else if(m_pShinNougyou31Ex != 0)	{
				sh_Tabmng.pWnd = m_pShinNougyou31Ex;
			}
			else	{
				sh_Tabmng.pWnd = m_pShinNougyou31;
			}
// 211129_h add <--
// 2023/07/14 tanaka change <--
			m_TabMng.Add ( sh_Tabmng );
		}

		if ( m_pZmsub->zvol->ind_type & 0x02 ) {	// 不動産
// 211129_h del -->
			//if ( !m_pShinFudousan31 ) {
			//	m_pShinFudousan31 = new CShinFudousan31 ( this );
			//	m_pShinFudousan31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
			//	m_pShinFudousan31->Create ( IDD_DIALOG_FUDOUSAN31, &m_MainTab );
			//}
			//ptmpTab[tabIdx] = m_pShinFudousan31;
			//m_ListType[tabIdx] = ID_ICSSH_31_REALESTATE_LIST_TAB;
			//m_MainTab.InsertItem ( tabIdx, _T( "不動産所得用" ), m_pShinFudousan31->m_hWnd );
// 211129_h del <--
// 211129_h add -->
			if (syzSgn2 & 0x01) {
				if (!m_pShinFudousan31Ex2) {
					m_pShinFudousan31Ex2 = new CShinFudousan31Ex2(this);
					m_pShinFudousan31Ex2->InitInfo(&m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith, &m_pShzCalc);
					m_pShinFudousan31Ex2->Create(IDD_DIALOG_FUDOUSAN31_ex2, &m_MainTab);
				}
				ptmpTab[tabIdx] = m_pShinFudousan31Ex2;
				m_ListType[tabIdx] = ID_ICSSH_31_REALESTATE_LIST_TAB;
				m_MainTab.InsertItem(tabIdx, _T("不動産所得用"), m_pShinFudousan31Ex2->m_hWnd);
			}
			else {

			if( syzSgn )	{
				// 旧税率あり
				if ( !m_pShinFudousan31 ) {
					m_pShinFudousan31 = new CShinFudousan31 ( this );
					m_pShinFudousan31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
					m_pShinFudousan31->Create ( IDD_DIALOG_FUDOUSAN31, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinFudousan31;
				m_ListType[tabIdx] = ID_ICSSH_31_REALESTATE_LIST_TAB;
				m_MainTab.InsertItem ( tabIdx, _T( "不動産所得用" ), m_pShinFudousan31->m_hWnd );
			}
			else	{
				// 旧税率なし
				if ( !m_pShinFudousan31Ex ) {
					m_pShinFudousan31Ex = new CShinFudousan31Ex ( this );
					m_pShinFudousan31Ex->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
					m_pShinFudousan31Ex->Create ( IDD_DIALOG_FUDOUSAN31_ex, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinFudousan31Ex;
				m_ListType[tabIdx] = ID_ICSSH_31_REALESTATE_LIST_TAB;
				m_MainTab.InsertItem ( tabIdx, _T( "不動産所得用" ), m_pShinFudousan31Ex->m_hWnd );
			}
		}	
// 211129_h add <--
			tabIdx++;
			SHIN_TABMAX++;

			memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
			sh_Tabmng.tabtype = ID_ICSSH_31_REALESTATE_LIST_TAB;
// 211129_h del -->
			//sh_Tabmng.pWnd = m_pShinFudousan31;
// 211129_h del <--
// 211129_h add -->
			if (m_pShinFudousan31Ex2 != 0) {
				sh_Tabmng.pWnd = m_pShinFudousan31Ex2;
			}
			if(m_pShinFudousan31Ex != 0)	{
				sh_Tabmng.pWnd = m_pShinFudousan31Ex;
			}
			else	{
				sh_Tabmng.pWnd = m_pShinFudousan31;
			}
// 211129_h add <--
			m_TabMng.Add ( sh_Tabmng );
		}

		// 課税売上高
// kasai del -->		
		/*if( !m_pShinSell31 ){
			m_pShinSell31 = new CShinSell31( this );
			m_pShinSell31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
			m_pShinSell31->Create( IDD_DIALOG_SELL31, &m_MainTab );
		}
		ptmpTab[tabIdx] = m_pShinSell31;
		m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
		m_MainTab.InsertItem( tabIdx, _T( "課税売上高" ), m_pShinSell31->m_hWnd );
		tabIdx++;
		SHIN_TABMAX++;

		memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
		sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
		sh_Tabmng.pWnd = m_pShinSell31;
		m_TabMng.Add( sh_Tabmng );*/
// kasai del <--

		if (syzSgn2 & 0x01) {
			if (!m_pShinSell31Ex2) {
				m_pShinSell31Ex2 = new CShinSell31Ex2(this);
				m_pShinSell31Ex2->InitInfo(&m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith, &m_pShzCalc);
				m_pShinSell31Ex2->Create(IDD_DIALOG_SELL31_ex2, &m_MainTab);
			}
			ptmpTab[tabIdx] = m_pShinSell31Ex2;
			m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
			m_MainTab.InsertItem(tabIdx, _T("課税売上高"), m_pShinSell31Ex2->m_hWnd);
			tabIdx++;
			SHIN_TABMAX++;

			memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
			sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
			sh_Tabmng.pWnd = m_pShinSell31Ex2;
			m_TabMng.Add(sh_Tabmng);
		}
		else {

// kasai add -->
		if( syzSgn ){
			// 旧税率あり
			if( !m_pShinSell31 ){
				m_pShinSell31 = new CShinSell31( this );
				m_pShinSell31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinSell31->Create( IDD_DIALOG_SELL31, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSell31;
			m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T( "課税売上高" ), m_pShinSell31->m_hWnd );
			tabIdx++;
			SHIN_TABMAX++;

			memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
			sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
			sh_Tabmng.pWnd = m_pShinSell31;
			m_TabMng.Add( sh_Tabmng );
		}
		else{
			// 旧税率なし
			if( !m_pShinSell31Ex ){
				m_pShinSell31Ex = new CShinSell31Ex( this );
				m_pShinSell31Ex->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinSell31Ex->Create( IDD_DIALOG_SELL31_ex, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSell31Ex;
			m_ListType[tabIdx] = ID_ICSSH_31_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T( "課税売上高" ), m_pShinSell31Ex->m_hWnd );
			tabIdx++;
			SHIN_TABMAX++;

			memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
			sh_Tabmng.tabtype = ID_ICSSH_31_SALES_LIST_TAB;
			sh_Tabmng.pWnd = m_pShinSell31Ex;
			m_TabMng.Add( sh_Tabmng );
		}
// kasai add <--
}

		if( m_Util.GetKazeihoushiki(m_pZmsub->zvol) != ID_ICSSH_KANNI ){
			
			// 課税仕入
// kasai del -->
			/*if (!m_pShinBuy31) {
				m_pShinBuy31 = new CShinBuy31( this );
				m_pShinBuy31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
				m_pShinBuy31->Create(IDD_DIALOG_BUY31, &m_MainTab);
			}
			ptmpTab[tabIdx] = m_pShinBuy31;
			m_ListType[tabIdx] = ID_ICSSH_31_STOCK_LIST_TAB;
			m_MainTab.InsertItem(tabIdx, _T("課税仕入高"), m_pShinBuy31->m_hWnd);
			tabIdx++;
			SHIN_TABMAX++;

			memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
			sh_Tabmng.tabtype = ID_ICSSH_31_STOCK_LIST_TAB;
			sh_Tabmng.pWnd = m_pShinBuy31;
			m_TabMng.Add(sh_Tabmng);*/
// kasai del <--

			// 原則でも2割特例を適用する場合は簡易と同じ扱いになるため、課税仕入高は表示しない
			long emd = 0;
			if( !( m_pSnHeadData->Sn_SKKBN % 2 )) {
				// 中間申告
				emd = m_pSnHeadData->Sn_MDAYE;
			}
			else {
				// 確定申告
				emd = m_pSnHeadData->Sn_KDAYE;
			}
			
			if( !(emd >= ICS_SH_INVOICE_DAY && m_pSnHeadData->m_DispTabSgn & 0x2000) ){
// kasai add -->
				if (syzSgn2 & 0x01) {
					if (!m_pShinBuy31Ex2) {
						m_pShinBuy31Ex2 = new CShinBuy31Ex2(this);
						m_pShinBuy31Ex2->InitInfo(&m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith, &m_pShzCalc);
						m_pShinBuy31Ex2->Create(IDD_DIALOG_BUY31_ex2, &m_MainTab);
					}
					ptmpTab[tabIdx] = m_pShinBuy31Ex2;
					m_ListType[tabIdx] = ID_ICSSH_31_STOCK_LIST_TAB;
					m_MainTab.InsertItem(tabIdx, _T("課税仕入高"), m_pShinBuy31Ex2->m_hWnd);
					tabIdx++;
					SHIN_TABMAX++;

					memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
					sh_Tabmng.tabtype = ID_ICSSH_31_STOCK_LIST_TAB;
					sh_Tabmng.pWnd = m_pShinBuy31Ex2;
					m_TabMng.Add(sh_Tabmng);
				}
				else {

				if( syzSgn ){
					// 旧税率あり
					if (!m_pShinBuy31) {
						m_pShinBuy31 = new CShinBuy31( this );
						m_pShinBuy31->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
						m_pShinBuy31->Create(IDD_DIALOG_BUY31, &m_MainTab);
					}
					ptmpTab[tabIdx] = m_pShinBuy31;
					m_ListType[tabIdx] = ID_ICSSH_31_STOCK_LIST_TAB;
					m_MainTab.InsertItem(tabIdx, _T("課税仕入高"), m_pShinBuy31->m_hWnd);
					tabIdx++;
					SHIN_TABMAX++;

					memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
					sh_Tabmng.tabtype = ID_ICSSH_31_STOCK_LIST_TAB;
					sh_Tabmng.pWnd = m_pShinBuy31;
					m_TabMng.Add(sh_Tabmng);
				}
				else{
					// 旧税率なし
					if (!m_pShinBuy31Ex) {
						m_pShinBuy31Ex = new CShinBuy31Ex( this );
						m_pShinBuy31Ex->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmsub, &m_Arith , &m_pShzCalc );
						m_pShinBuy31Ex->Create(IDD_DIALOG_BUY31_ex, &m_MainTab);
					}
					ptmpTab[tabIdx] = m_pShinBuy31Ex;
					m_ListType[tabIdx] = ID_ICSSH_31_STOCK_LIST_TAB;
					m_MainTab.InsertItem(tabIdx, _T("課税仕入高"), m_pShinBuy31Ex->m_hWnd);
					tabIdx++;
					SHIN_TABMAX++;

					memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
					sh_Tabmng.tabtype = ID_ICSSH_31_STOCK_LIST_TAB;
					sh_Tabmng.pWnd = m_pShinBuy31Ex;
					m_TabMng.Add(sh_Tabmng);
				}
// kasai add <--
			}
			}
		}
	}

	for ( int tabcnt = 0; tabcnt < tabIdx; tabcnt++ ) {
		( ( CSyzBaseDlg* )ptmpTab[tabcnt] )->Static_Update ( m_pSnHeadData->Sn_Sign4 );
	}

	// 先頭ページ保存
	m_pFirstPage = ptmpTab[0];

	RECT rect = { 0 };
	ptmpTab[0]->GetClientRect ( &rect );
	for ( int idx = 0; idx < tabIdx; idx++ ) {
		ptmpTab[idx]->MoveWindow ( &rect );
	}

	RECT rect1, rect2;
	CWnd *pWnd = ( CWnd* )&m_MainTab;
	m_MainTab.AdjustRect ( FALSE, &rect1 );
	rect2.left   = rect1.left;
	rect2.top    = rect1.top;
	rect2.bottom = rect1.bottom;
	rect2.right  = rect1.right;
	for ( int idx = 0; idx < tabIdx; idx++ ) {
		if ( ptmpTab[idx] ) {
			ptmpTab[idx]->MoveWindow ( &rect2 );
		}
	}

	GetClientRect ( vwRect );
	CRect tabRect;
	m_MainTab.GetClientRect ( tabRect );

	SCROLLINFO siV = { 0 },
			   siH = { 0 };
	GetScrollInfo ( SB_VERT, &siV );
	tabRect.top = tabRect.top - siV.nPos;
	if ( vwRect.Height() < siV.nMax ) {
		tabRect.bottom = siV.nMax;
	}
	else {
		tabRect.bottom = vwRect.bottom;
	}

	GetScrollInfo ( SB_HORZ, &siH );
	tabRect.left -= siH.nPos;
	tabRect.right -= siH.nPos;

	m_MainTab.MoveWindow ( tabRect );

	//なぜか描画されないので、とりあえず応急処置清水後で修正
	if( m_MainTab.GetItemCount() >= 2 ){
		m_MainTab.SetCurSel(1);
	}
	m_MainTab.SetCurSel(0);
	////////////////////////////////////////////

	m_DispTabSgn = m_pSnHeadData->m_DispTabSgn;	// 基本情報が変更されたかどうかのチェックのため保存

	return 0;
}

//==========================================
// タブ内のダイアログを全消去
//==========================================
void CTaxationList31View::DeleteInsideDlg()
{
	// ビュー内部からフォーカスを外す
	m_MainTab.SetFocus();

	// 事業所得用
	if ( m_pShinJigyo31 ) {
		m_pShinJigyo31->DestroyWindow();
		delete m_pShinJigyo31;
		m_pShinJigyo31 = NULL;
	}
	// kasai add -->
	// 事業所得用　旧税率なし
	if ( m_pShinJigyo31Ex ) {
		m_pShinJigyo31Ex->DestroyWindow();
		delete m_pShinJigyo31Ex;
		m_pShinJigyo31Ex = NULL;
	}
	// kasai add <--
	// 事業所得用 令和5年10月1日以後終了する課税期間
	// 2023/07/11 h.wada add -> 
	if (m_pShinJigyo31Ex2) {
		m_pShinJigyo31Ex2->DestroyWindow();
		delete m_pShinJigyo31Ex2;
		m_pShinJigyo31Ex2 = NULL;
	}
	// h.wada add <-

	// 不動産所得用
// 211129_h add -->
	if ( m_pShinFudousan31Ex ) {
		m_pShinFudousan31Ex->DestroyWindow();
		delete m_pShinFudousan31Ex;
		m_pShinFudousan31Ex = NULL;
	}
// 211129_h add <--
	if ( m_pShinFudousan31 ) {
		m_pShinFudousan31->DestroyWindow();
		delete m_pShinFudousan31;
		m_pShinFudousan31 = NULL;
	}
	if (m_pShinFudousan31Ex2) {
		m_pShinFudousan31Ex2->DestroyWindow();
		delete m_pShinFudousan31Ex2;
		m_pShinFudousan31Ex2 = NULL;
	}

	// 農業所得用
// 2023/07/14 tanaka add --> 
	//令和5年10月1日以後終了する課税期間
	if (m_pShinNougyou31Ex2) {
		m_pShinNougyou31Ex2->DestroyWindow();
		delete m_pShinNougyou31Ex2;
		m_pShinNougyou31Ex2 = NULL;
	}
// 2023/07/14 tanaka add <--
// 211129_h add -->
	if ( m_pShinNougyou31Ex ) {
		m_pShinNougyou31Ex->DestroyWindow();
		delete m_pShinNougyou31Ex;
		m_pShinNougyou31Ex = NULL;
	}
// 211129_h add <--
	if ( m_pShinNougyou31 ) {
		m_pShinNougyou31->DestroyWindow();
		delete m_pShinNougyou31;
		m_pShinNougyou31 = NULL;
	}

	// 課税売上高
	if( m_pShinSell31 ){
		m_pShinSell31->DestroyWindow();
		delete m_pShinSell31;
		m_pShinSell31 = NULL;
	}
	// kasai add -->
	if( m_pShinSell31Ex ){
		m_pShinSell31Ex->DestroyWindow();
		delete m_pShinSell31Ex;
		m_pShinSell31Ex = NULL;
	}
	// kasai add <--
	// tanaka add -->
	if (m_pShinSell31Ex2) {
		m_pShinSell31Ex2->DestroyWindow();
		delete m_pShinSell31Ex2;
		m_pShinSell31Ex2 = NULL;
	}
	// tanaka add <--

	// 課税仕入高
	if( m_pShinBuy31 ){
		m_pShinBuy31->DestroyWindow();
		delete m_pShinBuy31;
		m_pShinBuy31 = NULL;
	}
	// kasai add -->
	if( m_pShinBuy31Ex ){
		m_pShinBuy31Ex->DestroyWindow();
		delete m_pShinBuy31Ex;
		m_pShinBuy31Ex = NULL;
	}
	// kasai add <--
	// tanaka add -->
	if (m_pShinBuy31Ex2) {
		m_pShinBuy31Ex2->DestroyWindow();
		delete m_pShinBuy31Ex2;
		m_pShinBuy31Ex2 = NULL;
	}
	// tanaka add <--

	//事業別売上
// 211201_h add -->
	if( m_pShinKniinp31Ex ){
		m_pShinKniinp31Ex->DestroyWindow();
		delete m_pShinKniinp31Ex;
		m_pShinKniinp31Ex = NULL;
	}
// 211201_h add <--
	if( m_pShinKniinp31 ){
		m_pShinKniinp31->DestroyWindow();
		delete m_pShinKniinp31;
		m_pShinKniinp31 = NULL;
	}
	
	m_TabMng.RemoveAll();
	m_pFirstPage = NULL;

}

//==========================================
// 履歴シーケンスのセット
//==========================================
void CTaxationList31View::SetSnSeq ( int snseq )
{
	m_SnSeq = snseq;
	m_SvSnSeq = snseq;
}

//==========================================
// 履歴シーケンスの切り替え
//------------------------------------------
// 引数
//		snseq		:		履歴シーケンス
//------------------------------------------
// 返送値
//		0			:		正常終了
//==========================================
int CTaxationList31View::ChangeSnSeq( int snseq )
{

	if( m_SvSnSeq == snseq ){
		return 0;
	}

	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		// 既存データ書き込み
		if( WriteData( TRUE , FALSE ) ){
			return -1;
		}

		// 新履歴番号のセット
		SetSnSeq( snseq );

		// 帳票データ
		if( !m_pTaxListData ){
			if( (m_pTaxListData = new CH31TaxListData()) == NULL ){
				return -1;
			}
			if( m_pTaxListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		// 既存データ書き込み
		if( WriteSpcData( TRUE ) ){
			return -1;
		}

		// 新履歴番号のセット
		SetSnSeq( snseq );

		// 帳票データ
		if( !m_pSpcListData ){
			if( (m_pSpcListData = new CH31SpcListData(m_pSnHeadData->SVmzsw ? TRUE:FALSE)) == NULL ){
				return -1;
			}
			if( m_pSpcListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}

	return 0;
}

//==========================================
// 終了処理
//==========================================
int CTaxationList31View::EndJob()
{
	// 最終データ書き込み
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE){
		if( WriteData( TRUE , FALSE) ){
			return -1;
		}
		if( m_pShzCalc ){
			delete m_pShzCalc;
			m_pShzCalc = NULL;
		}
		DeleteInsideDlg();
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE){
		if( WriteSpcData(TRUE) ){
			return -1;
		}

		DeleteInsideSpcDlg();
	}

	return 0;
}

// --> 20191112　必要ないタイミングで保存処理走ってしまっていたため追加
//==========================================
// 終了処理
//==========================================
int CTaxationList31View::EndJob( int wflg )
{
	// 最終データ書き込み
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE){
		if( WriteData( TRUE , FALSE) ){
			return -1;
		}
		if( m_pShzCalc ){
			delete m_pShzCalc;
			m_pShzCalc = NULL;
		}
		DeleteInsideDlg();
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE){
		if( wflg ){
			if( WriteSpcData(TRUE) ){
				return -1;
			}
		}

		DeleteInsideSpcDlg();
	}

	return 0;
}
// <-- 20191112

//==========================================
// データ書き込み
//------------------------------------------
// 引数
// 引数	isEnd		：	最終データ？
//		isGetData	：	データを取得するかどうか
//------------------------------------------
// 返送値
//		0			:		正常終了
//==========================================
int CTaxationList31View::WriteData( BOOL isEnd, BOOL isGetData )
{ 

	if( m_pTaxListData ){
		if( m_pTaxListData->SetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		if( isGetData ){
			if( m_pTaxListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
		if( isEnd ){
			delete m_pTaxListData;
			m_pTaxListData = NULL;
		}
	}
	if( m_pSpcListData ){
		if( m_pSpcListData->SetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		if( isGetData ){
			if( m_pSpcListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
		if( isEnd ){
			delete m_pSpcListData;
			m_pSpcListData = NULL;
		}
	}
	return 0;
}

// 2019/10/29
// 引数が2つあるWriteDataやと特定収入のタブ切替の保存のときに31と26の切り替えがうまくいかず、
// 26のWriteDataがちゃんと流れていなかったため、引数が1つの関数を追加。
//-----------------------------------------------------------------------------
// データ書込み
//-----------------------------------------------------------------------------
// 引数	isEnd	：	最終データ？
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationList31View::WriteData( BOOL isEnd )
{ 
	if( m_pTaxListData ){
		if( m_pTaxListData->SetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pTaxListData;
			m_pTaxListData = NULL;
		}
	}

	if( m_pSpcListData ){
		if( m_pSpcListData->SetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pSpcListData;
			m_pSpcListData = NULL;
		}
	}

	return 0;
}

//==========================================
// ページ切替 	ZIM15_SyzTaxationList31d.dll!CH31SpcListData::SetData(CDBNpSub * pDBNpSub, CSnHeadData * pSnHeadData, int sn_seq) 行 1857	C++

//------------------------------------------
// 引数
//		move		:		1:順送り -1:逆戻り
//==========================================
void CTaxationList31View::PageChange( int move )
{
	int selno = m_MainTab.GetCurSel();

	// 移動後のタブ
	int movesel;
	if (move == 1) {
		if (selno == (m_MainTab.GetItemCount() - 1))
			movesel = 0;
		else
			movesel = selno + move;
	}
	else if (move == -1) {
		if (selno == 0)
			movesel = (m_MainTab.GetItemCount() - 1);
		else
			movesel = selno + move;
	}
	else return;


	// 各表の更新処理後の再表示
	m_MainTab.SetCurSel(movesel);
}

CSyzBaseDlg* CTaxationList31View::GetBaseList( int no ){

	CSyzBaseDlg *pBaseList = NULL;

	switch( no ){
		case ID_ICSSH_31_GENERAL_LIST_TAB:
			// kasai del -->
			//pBaseList = m_pShinJigyo31;
			// kasai del <--
			// 2023/07/11 h.wada change -> 
			if (m_pShinJigyo31Ex2 != 0) {
				pBaseList = m_pShinJigyo31Ex2;
			}
			// kasai add -->
			else if( m_pShinJigyo31Ex != 0 ){
				pBaseList = m_pShinJigyo31Ex;
			}
			else{
				pBaseList = m_pShinJigyo31;
			}
			// kasai add <--
			// h.wada change <-
			break;
		case ID_ICSSH_31_REALESTATE_LIST_TAB:
// 211129_h del -->
			//pBaseList = m_pShinFudousan31;
// 211129_h del <--
// 211129_h add -->
			if (m_pShinFudousan31Ex2 != 0) {
				pBaseList = m_pShinFudousan31Ex2;
			}
			//if(m_pShinFudousan31Ex != 0)	{		// 230801Aw del
			else if(m_pShinFudousan31Ex != 0)	{	// 230801Aw add
				// 新税率のみ → 新様式
				pBaseList = m_pShinFudousan31Ex;
			}
			else	{
				pBaseList = m_pShinFudousan31;
			}
// 211129_h add <--
			break;
		case ID_ICSSH_31_AGRICULTUTRE_LIST_TAB:
// 211129_h del -->
			//pBaseList = m_pShinNougyou31;
// 211129_h del <--
// 2023/07/14 tanaka change ->
			if (m_pShinNougyou31Ex2 != 0) {
				pBaseList = m_pShinNougyou31Ex2;
			}
// 211129_h add -->
			else if( m_pShinNougyou31Ex != 0 )	{
				// 新税率のみ → 新様式
				pBaseList = m_pShinNougyou31Ex;
			}
			else	{
				pBaseList = m_pShinNougyou31;
			}
// 211129_h add <--
// 2023/07/14 tanaka change <--
			break;
		case ID_ICSSH_31_SALES_LIST_TAB:
// kasai del -->
			//pBaseList = m_pShinSell31;
// kasai del <--
// kasai add -->
			if ( m_pShinSell31Ex2 != 0 ) {
				pBaseList = m_pShinSell31Ex2;
			}
			else if( m_pShinSell31Ex != 0 ){
				pBaseList = m_pShinSell31Ex;
			}
			else{
				pBaseList = m_pShinSell31;
			}
// kasai add <--
			break;
		case ID_ICSSH_31_STOCK_LIST_TAB:
// kasai del -->
			//pBaseList = m_pShinBuy31;
// kasai del <--
// kasai add -->
			if ( m_pShinBuy31Ex2 != 0 ) {
				pBaseList = m_pShinBuy31Ex2;
			}
			else if( m_pShinBuy31Ex != 0 ){
				pBaseList = m_pShinBuy31Ex;
			}
			else{
				pBaseList = m_pShinBuy31;
			}
// kasai add <--
			break;
		case ID_ICSSH_31_BUSINESS_SALES_LIST_TAB:
// 211201_h del -->
			//pBaseList = m_pShinKniinp31;
// 211201_h del <--
// 211201_h add -->
			if(m_pShinKniinp31Ex != 0)	{
				pBaseList = m_pShinKniinp31Ex;
			}
			else	{
				pBaseList = m_pShinKniinp31;
			}
// 211201_h add <--
			break;
		default:
			break;
	}

	return pBaseList;
}

//==========================================
// 各表の更新後の再表示
//-----------------------------------
// 引数	selno	：	該当表番号
//==========================================
void CTaxationList31View::IndicateTasList(int selno)
{
	CSyzBaseDlg *pBaseList = NULL;

	pBaseList = GetBaseList(m_ListType[selno]);
	if(pBaseList) {
		pBaseList->DispList(0);
		pBaseList->SetFocusRange();
	}
}

//==========================================
// 各表の更新後のフォーカスセット
//------------------------------------------
// 引数	selno	：	該当表番号
//==========================================
void CTaxationList31View::IndicateItemPosition(int selno)
{
	CSyzBaseDlg *pBaseList = NULL;
	pBaseList = GetBaseList( m_ListType[selno] );

	if( pBaseList ){
		pBaseList->SetItemPosition();
	}
}

// 修正No.168690 add -->
//==========================================
// 更新後、今選択中のタブ以外の画面を再描画
//------------------------------------------
// 引数	selno	：	該当表番号
//==========================================
void CTaxationList31View::DiagReSetPosition(void)
{
	int			ii;
	CSyzBaseDlg	*pBaseList = NULL;

	// 今選択中のタブ以外の画面をrefreshする
	for(ii=0; ii<m_MainTab.GetItemCount(); ii++)	{
		if(ii != m_MainTab.GetCurSel())	{
			pBaseList = GetBaseList(m_ListType[ii]);
			if(pBaseList)	{
				pBaseList->DiagReSetPosition();
			}
		}
	}
}
// 修正No.168690 add <--

//==========================================
// 経理方式の切替
//------------------------------------------
// 引数	pKHstax			：	変更後の経理方式
//------------------------------------------
// 返送値	IDOK		：	OK 終了
//			IDCANCEL	：	CANCEL 終了
//==========================================
int CTaxationList31View::ChgTaxationSystem(int *pKHstax)
{
	
	ASSERT( m_pTaxListData );

	HINSTANCE svhInstResource = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	CTaxtionSelect	TsDlg;
	TsDlg.m_KHstax = m_pTaxListData->Head.KHstax;

	int st = (int)TsDlg.DoModal();

	AfxSetResourceHandle( svhInstResource );

	if( st != IDOK ){
		return IDCANCEL;
	}
	m_pTaxListData->Head.KHstax = TsDlg.m_KHstax;

	m_pSnHeadData->Sn_Sign2 &= 0xf7;
	if( m_pTaxListData->Head.KHstax ){
		m_pSnHeadData->Sn_Sign2 |= 0x08;
	}

	// 履歴分の更新
	*pKHstax = TsDlg.m_KHstax;

	// 各表の更新処理
	m_pShzCalc->Shz_31AllCalc();
	// 各表の再表示
	IndicateTasList( m_MainTab.GetCurSel() );

	SetItemPosition(m_MainTab.GetCurSel());

	return 0;
}

//==========================================
// 全画面：画面表示 再初期化
//------------------------------------------
// 引数	isCompChange	：	強制再初期化？
//		isResetFocus	：	フォーカスの再セットを行うか？
//------------------------------------------
// 返送値
//==========================================
int CTaxationList31View::ReInitDisp(BOOL isCompChange, BOOL SixKindFlg, BOOL isResetFocus/*=TRUE*/)
{
	BOOL svSixKindFlg = m_SixKindFlg;

	m_SixKindFlg = SixKindFlg;

	BOOL	RecreataTab = FALSE;
	if( isCompChange ){
		RecreataTab = TRUE;
	}
//	else {
//		if( svSixKindFlg != SixKindFlg ){
//			RecreataTab = TRUE;
//		}
//	}

	if( (m_DispTabSgn & 0x2000) != (m_pSnHeadData->m_DispTabSgn & 0x2000) ){
		RecreataTab = TRUE;
	}
	m_DispTabSgn = m_pSnHeadData->m_DispTabSgn;

	if( RecreataTab ){
		InitTab(TRUE);
		if( isResetFocus ){
			PostMessage( WM_MYMESSAGE31, 0, 0 );
		}
	}
	else {
		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
			ReInitTaxDisp();
		}
		else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
			ReInitSpcDisp();
		}
		if( isResetFocus){
			PostMessage( WM_MYMESSAGE31, 0, 0 );
		}
	}


	return 0;	
}

//==========================================
// 全画面：画面表示 課税取引金額計算表 再初期化
//------------------------------------------
// 返送値	0	：	正常終了
//==========================================
int CTaxationList31View::ReInitTaxDisp()
{
// kasai del -->
	/*if( m_pShinJigyo31 ){
		m_pShinJigyo31->Init();
	}*/
// kasai del <--
// 2023/07/11 h.wada change --> 
	if ( m_pShinJigyo31Ex2 ) {
		m_pShinJigyo31Ex2->Init();
	}
// kasai add -->
	else if( m_pShinJigyo31Ex ){
		m_pShinJigyo31Ex->Init();
	}
	else if( m_pShinJigyo31 ){
		m_pShinJigyo31->Init();
	}
// kasai add <--
// h.wada change <--
// 211129_h del -->
	//if( m_pShinFudousan31 ){
	//	m_pShinFudousan31->Init();
	//}
	//if( m_pShinNougyou31 ){
	//	m_pShinNougyou31->Init();
	//}
// 211129_h del <--
// 211129_h add -->
	// 不動産
	if ( m_pShinFudousan31Ex2 ) {
		m_pShinFudousan31Ex2->Init();
	}
	else if( m_pShinFudousan31Ex ){
		m_pShinFudousan31Ex->Init();
	}
	else if( m_pShinFudousan31 )	{
		m_pShinFudousan31->Init();
	}
	// 農業
// 2023/07/14 tanaka change --> 
	if ( m_pShinNougyou31Ex2 ) {
		m_pShinNougyou31Ex2->Init();
	}
	else if( m_pShinNougyou31Ex ){
		m_pShinNougyou31Ex->Init();
	}
	else if( m_pShinNougyou31 )	{
		m_pShinNougyou31->Init();
	}
// 2023/07/14 tanaka change <-- 
// 211129_h add <--
// kasai del -->
	/*if( m_pShinSell31 ){
		m_pShinSell31->Init();
	}
	if( m_pShinBuy31 ){
		m_pShinBuy31->Init();
	}*/
// kasai del <--
// kasai add -->
	if ( m_pShinSell31Ex2 ) {
		m_pShinSell31Ex2->Init();
	}
	else if( m_pShinSell31Ex ){
		m_pShinSell31Ex->Init();
	}
	else if( m_pShinSell31 ){
		m_pShinSell31->Init();
	}

	if ( m_pShinBuy31Ex2 ) {
		m_pShinBuy31Ex2->Init();
	}
	else if( m_pShinBuy31Ex ){
		m_pShinBuy31Ex->Init();
	}
	if( m_pShinBuy31 ){
		m_pShinBuy31->Init();
	}
// kasai add <--
// 211201_h del -->
	//if( m_pShinKniinp31 ){
	//	m_pShinKniinp31->Init();
	//}
// 211201_h del <--
// 211201_h add -->
	if( m_pShinKniinp31Ex ){
		m_pShinKniinp31Ex->Init();
	}
	else if( m_pShinKniinp31 ){
		m_pShinKniinp31->Init();
	}
// 211201_h add <--

	return 0;
}

//==========================================
// 現在の入力位置にフォーカスセット
//------------------------------------------
// 引数	IsInEnd	：	終了処理中？
//==========================================
void CTaxationList31View::SetNowPosition(BOOL IsInEnd/*=FALSE*/)
{
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		SetItemPosition( m_MainTab.GetCurSel() );
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		SetItemPositionEx(m_MainTab.GetCurSel());
	}
}

//==========================================
// フォーカスセット
//------------------------------------------
// 引数	selno	：	セットするタブNO
//==========================================
void CTaxationList31View::SetItemPosition(int selno)
{
	if( (int)m_TabMng.GetCount() <= selno ){
		return;
	}

	CSyzBaseDlg		*pBaseList = NULL;

	pBaseList = GetBaseList( m_TabMng[selno].tabtype );

	if( pBaseList ){
		m_MainTab.SetFocus();
		pBaseList->SetItemPosition();
	}
}

//==========================================
// 課税取引金額計算書との連動
//------------------------------------------
// 返送値	0	：	正常終了
//==========================================
int CTaxationList31View::ConnectedTaxList()
{
	
	if( m_pTaxListData ){
		if( m_pTaxListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}

//		m_pTaxListData->ConnectZmdata( m_pZmSub );
		ASSERT( m_pShinInfo );
		if( m_pShinInfo == NULL ){
			return -1;
		}
		//m_pTaxListData->ConnectZmdata( m_pZmsub, m_pSyzSyukei, m_pShinInfo->s_tno, m_pShinInfo->d_tno, m_pSnHeadData->SVmzsw==1 ? TRUE:FALSE );
		m_pTaxListData->ConnectZmdata ( m_pZmsub, m_pSyzSyukei, m_pShinInfo->s_tno, m_pShinInfo->d_tno, ( m_pSnHeadData->SVmzsw == 1 ? TRUE : FALSE ), m_pSnHeadData );

		if( m_pTaxListData->SetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}
	return 0;
}

//==========================================
// 特定収入計算表との連動
//------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//==========================================
int CTaxationList31View::ConnectedSpcList()
{
	if ( m_pSpcListData ) {
		ASSERT ( m_pShinInfo );
		if ( m_pShinInfo == NULL ) {
			return -1;
		}

		m_pSpcListData->InitSpData ( m_pZmsub );
		if ( m_pSpcListData->GetData ( m_pZmsub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}

		m_pSpcListData->CalqForUriRatio ( m_pZmsub, m_pSyzSyukei, m_pSnHeadData, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
		m_pSpcListData->SPassOffCalqMain ( m_pSnHeadData );

		// サインON
		char	WORK0[MONY_BUF_SIZE] = { 0 };
		m_pSnHeadData->Sn_Sign2 &= 0xfd;
		m_Arith.l_input ( WORK0, _T( "50" ) );
		if ( m_Arith.l_cmp ( m_pSpcListData->stval.SpTswr, WORK0 ) > 0 ) {
			// 金額転記
			SetHonpyoMony();
		}
		else {
			m_pSnHeadData->Sn_Sign2 |= 0x02;
		}

		if ( m_pSpcListData->SetData ( m_pZmsub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}
	}

	return 0;
}

//==========================================
// 現データを消去して、データの再読込み
//------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//==========================================
int CTaxationList31View::ReReadData()
{
// 帳票データ
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( m_pTaxListData ){
			delete m_pTaxListData;
			m_pTaxListData = NULL;

			if( (m_pTaxListData = new CH31TaxListData()) == NULL ){
				return -1;
			}
			if( m_pTaxListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		//特定収入
		if( m_pSpcListData ){
			delete m_pSpcListData;
			m_pSpcListData = NULL;

			ASSERT(m_pSnHeadData);
			if( m_pSnHeadData == NULL ){
				return -1;
			}
			if( (m_pSpcListData = new CH31SpcListData(m_pSnHeadData->SVmzsw ? TRUE:FALSE)) == NULL ){
				return -1;
			}
			if( m_pSpcListData->InitSpData(m_pZmsub) ){
				return -1;
			}
			if( m_pSpcListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}

	return 0;
}

//==========================================
// 転記データ取得　（本表に転記するデータを取得）
//------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//==========================================
void CTaxationList31View::GetTenkiData(SH_TENKIDATA31* tenki_data)
{
	memmove(tenki_data , &m_pShzCalc->tenkidata , sizeof(*tenki_data));
}

//==========================================
// 転記データ取得　（本表に転記するデータを取得）
//------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//==========================================
void CTaxationList31View::GetTenkiData(SH_INVOICE_TENKIDATA31* inv_tenki_data)	//課税期間末日が令和５年10月1日以降時に使用
{
	memmove(inv_tenki_data, &m_pShzCalc->inv_tenkidata, sizeof(*inv_tenki_data));
}

// 手入力サインのクリア
int CTaxationList31View::ClearManualInputSign()
{
	//設定の変更や、履歴表示するだけで手入力サインが落ちる。残す方向にしたので処理をカット　'20.01.15
	return 0;

	/*
	int rt = 0;

	if( m_pShinBuy31 ){
		m_pShinBuy31->ClearManualInputSign();
	}
	if( m_pShinSell31 ){
		m_pShinSell31->ClearManualInputSign();
	}
	if( m_pShinJigyo31 ){
		m_pShinJigyo31->ClearManualInputSign();
	}
	if( m_pShinFudousan31 ){
		m_pShinFudousan31->ClearManualInputSign();
	}
	if( m_pShinNougyou31 ){
		m_pShinNougyou31->ClearManualInputSign();
	}
	if( m_pShinKniinp31 ){
		m_pShinKniinp31->ClearManualInputSign();
	}

	return rt;
	*/
}

//==========================================
// 金額の再取得
//------------------------------------------
// 引数	isResetFocus	：	フォーカスの再セットを行うかどうか
//------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//==========================================
int CTaxationList31View::ReGetCalqedMony(BOOL isResetFocus)
{
	if ( m_SelType == ID_ICSSH_TAXLIST_TYPE ) {
		ReInitDisp ( TRUE, isResetFocus );
	}
	else if ( m_SelType == ID_ICSSH_SPCLIST_TYPE ) {
		ReInitDisp ( TRUE, isResetFocus );
	}

	return 0;
}

//==========================================
// 過年度プロ呼出し後にデータを吸い上げる
//------------------------------------------
// 返送値	0		：	正常終了
//==========================================
int CTaxationList31View::GetPastData()
{
	// 帳票データ
	if( !m_pTaxListData ){
		if( (m_pTaxListData = new CH31TaxListData()) == NULL ){
			return -1;
		}
	}
	if( m_pTaxListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
	//if( m_pTaxListData->GetPastData(m_pZmsub, m_pSnHeadData, m_SnSeq) ){
		return -1;
	}

	return 0;
}

//==========================================
// 消費税簡易計算表　ダイアログ表示（経過措置なし）
//------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//==========================================
int CTaxationList31View::SpecPassOffExDlg()
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle ( g_hInstance );

// 24/02/19wd_特定収入対応 cor -->
	//double	x, y;
	//CSpecPassOffEx	dlg(this);
	//dlg.Init ( &m_pSnHeadData, &m_pSpcListData, &m_Arith );
	//GetRate ( x, y );
	//if ( y > 1.15 ){
	//	y = 1.15;
	//}
	//dlg.DoModal();
// ---------------------------------------
	long			eymd=0;
	long			flg;			// 24/02/27wd_特定収入対応 add
	double			x, y;
	CSpecPassOffEx	dlg(this);
	CSpecPassOffEx2	dlg2(this);

	// 令和3年12月31日以降の課税期間
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;

	// 令和5年10月1日以前の時は従来通り
	if(eymd < ICS_SH_INVOICE_DAY)	{		// ICS_SH_INVOICE_DAY=20231001 インボイス制度施工日
		dlg.Init(&m_pSnHeadData, &m_pSpcListData, &m_Arith);
		GetRate(x, y);
		if(y > 1.15) {
			y = 1.15;
		}
		dlg.DoModal();
	}
	else	{
		dlg2.Init(&m_pSnHeadData, &m_pSpcListData, &m_Arith);
		GetRate(x, y);
		if(y > 1.15) {
			y = 1.15;
		}

		// 画面の入力制御を確認
		IsSpecificEnable(&flg);		
		if((flg & 0x0010))		dlg2.m_ListNo = 0;
		else if((flg & 0x0020))	dlg2.m_ListNo = 1;
		else if((flg & 0x0040))	dlg2.m_ListNo = 2;
		else					dlg2.m_ListNo = -1;

		dlg2.DoModal();	
	}
// 24/02/19wd_特定収入対応 cor <--

	AfxSetResourceHandle ( hInst );

	return 0;
}


//==========================================
// 入力ロック切替時の再表示
//==========================================
void CTaxationList31View::ChgLockDisp()
{
	int max = ( int )m_TabMng.GetCount();
	for ( int idx = 0; idx < max; idx++ ) {
		if ( m_TabMng[idx].pWnd ){
			( ( CSyzBaseDlg* )m_TabMng[idx].pWnd )->Static_Update ( m_pSnHeadData->Sn_Sign4 );
		}
	}
}

BOOL CTaxationList31View::IsEmpty_SonekiSyunyu()
{
	BOOL ret = TRUE;

	if((m_pSnHeadData->Sn_SKKBN==0x02) && (m_pSnHeadData->Sn_Syuusei&0x08)){ //予定（26号）は判定しない
		return TRUE;
	}

	if( m_pTaxListData != NULL ){
		KKVAL*	pU = (*m_pTaxListData).NRec.KUval;

		if( m_Arith.l_test((pU+5)->KKval1) != 0 ){
			ret = FALSE;
		}
	}

	return ret;
}

BOOL CTaxationList31View::IsEmpty_SonekiShiireKeihi()
{
	BOOL ret = TRUE;

	if((m_pSnHeadData->Sn_SKKBN==0x02) && (m_pSnHeadData->Sn_Syuusei&0x08)){ //予定（26号）は判定しない
		return TRUE;
	}

	if( m_pTaxListData != NULL ){
		KKVAL*	pS = (*m_pTaxListData).NRec.KSval;

		if( m_Arith.l_test((pS+5)->KKval1) != 0 ){
			ret = FALSE;
		}
	}

	return ret;
}


//==========================================
// 平成継続出力？
//------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//==========================================
BOOL CTaxationList31View::IsJcContinuePrint()
{
	//if ( m_Util.IsJcContinuePrint ( m_pSnHeadData ) ) {//清水後でOPEN
	//	return true;
	//}

	return false;

}

void CTaxationList31View::GetDispHyoData(){

	int selno = m_MainTab.GetCurSel();
	int max = (int)m_TabMng.GetCount();
	if( (selno>=max) || (selno==-1) ){
		return;
	}
	if( m_bShowed == FALSE ){
		return;
	}

	CSyzBaseDlg *pBaseList = NULL;

	pBaseList = GetBaseList( m_TabMng[selno].tabtype );
//	pBaseList->GetDiagData();
//	pBaseList->WriteData();

}
int CTaxationList31View::ChgKoketsuMeisyoGet( int *pKoketsu )
{
	ASSERT( m_pTaxListData );

	HINSTANCE svhInstResource = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	CKoketsuMeisyo KmDlg;
	KmDlg.m_KHkoke = m_pTaxListData->Head.KHkoke;

	int st = (int)KmDlg.DoModal();

	AfxSetResourceHandle( svhInstResource );

	if( st != IDOK ){
		return IDCANCEL;
	}
	m_pTaxListData->Head.KHkoke = KmDlg.m_KHkoke;

	if( m_pTaxListData->Head.KHkoke ){
		m_pSnHeadData->Sn_Sign4 |= 0x02;
	}else{
		m_pSnHeadData->Sn_Sign4 &= ~0x02;
	}

	*pKoketsu = KmDlg.m_KHkoke;

	// 各表の再表示
	IndicateTasList( m_MainTab.GetCurSel() );
	if( !m_pTaxListData->Head.KHkoke ){
		IndicateItemPosition( m_MainTab.GetCurSel() );
	}
// 修正No.168690 add -->
	else	{
		DiagReSetPosition();
	}
// 修正No.168690 add <--

	return IDOK;
}

//========================================================================
// 個人決算書金額の取得
//------------------------------------------------------------------------
// 返送値
//			0			:			正常終了
//			-1			:			エラー
//========================================================================
int CTaxationList31View::GetKoketsuKingaku()
{
	CString msg;

	msg.Format( _T( "課税取引金額計算表の初期化を行い、個人決算書から決算額を取得します。\r\nよろしいですか？" ));
	if( m_pTaxListData->Head.KHkoke ) {
		msg += _T( "\r\n\r\n※「個人決算書名称の取得」は、しないに変更されます。" );
	}

	if( ICSMessageBox( msg, ( MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION )) == IDNO ) {
		return 0;
	}

	CKoketsuKingaku koketsuKingaku( m_pZmsub );

	if( koketsuKingaku.DoModal() == IDOK ) {
		int idx = 0;
		koketsuKingaku.GetKoketsuKingauItem( idx );
		if( idx != 0 ) {
			if( StzRendo( idx ) == 0 ) {
				ICSMessageBox( _T( "個人決算書の金額を取得しました。\r\n金額を確認して下さい" ) );
				InitTab( TRUE );
			}
			else {
				ICSMessageBox( _T( "個人決算書の金額の取得に失敗しました。" ) );
				return -1;
			}
		}
	}

	return 0;
}

// 24/02/19wd_特定収入対応 add -->
// -------------------------------------------------------------------------------------------
//	特定収入計算表 最大頁数を返す
// -------------------------------------------------------------------------------------------
int CTaxationList31View::GetSpecListMaxPage(void)
{
	return(m_pSpcListData->TsTbl52.tp_rnum);
}

// 24/02/26_特定収入対応 add -->
// -------------------------------------------------------------------------------------------
//	特定収入計算表 指定された頁の選択帳票を返す
// -------------------------------------------------------------------------------------------
int CTaxationList31View::GetSpecListType( int pPage )
{
	int			rv=0;
	TSREC52		tsr52;

	m_pSpcListData->m_TblHdl.th_rget(&tsr52,&m_pSpcListData->TsTbl52,pPage);
	return(tsr52.ts521.sel);
}
// 24/02/26_特定収入対応 add <--

// 240308_h_160357 add -->
// -------------------------------------------------------------------------------------------
//	特定収入計算表 指定された頁の帳票にデータが入力されているか
//	【引　数】	int		頁
//				int		帳票種類
//							0:5-2(1)
//							1:5-2(2)
//							2:5-2(3)-1	3:5-2(3)-2	4:5-2(3)-3
//							5:5-2(3)-1	6:5-2(3)-2	7:5-2(3)-3
//	【返送値】	TRUE:データが入力されている		FALSE:データが入力されていない
// -------------------------------------------------------------------------------------------
BOOL CTaxationList31View::IsGetSpecListData( int pPage, int pList )
{
	BOOL		brv=TRUE;
	TSREC52		tsr52;
	char		bf[128]={0};
	int			def=m_Arith.l_retn();

	m_Arith.l_defn(0x16);

	m_pSpcListData->m_TblHdl.th_rget(&tsr52,&m_pSpcListData->TsTbl52,pPage);
	switch(pList)	{
		case	0:	// 5-2(1)
			memset(bf,0,sizeof(bf));
			memcpy(bf,tsr52.ts521.name,sizeof(tsr52.ts521.name));
			if(strlen(bf) > 0)	break;
			memset(bf,0,sizeof(bf));
			memcpy(bf,tsr52.ts521.kikans,sizeof(tsr52.ts521.kikans));
			if(atoi(bf) > 0)	break;
			memset(bf,0,sizeof(bf));
			memcpy(bf,tsr52.ts521.kikane,sizeof(tsr52.ts521.kikane));
			if(atoi(bf) > 0)	break;
			if(m_Arith.l_test(tsr52.ts521.val1) != 0)	break;
			if(m_Arith.l_test(tsr52.ts521.val2) != 0)	break;
			if(m_Arith.l_test(tsr52.ts521.val3) != 0)	break;
			brv=FALSE;
			break;
		case	1:	// 5-2(2)
			if(m_Arith.l_test(tsr52.ts522.val011) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val012) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val021) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val022) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val031) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val041) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val052) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val062) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val071) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val072) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val081) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val091) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val102) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val112) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val121) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val122) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val131) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val132) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val141) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val151) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val162) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val172) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val181) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val182) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val191) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val192) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val201) != 0)	break;
			if(m_Arith.l_test(tsr52.ts522.val201) != 0)	break;
			brv=FALSE;
			break;
		case	2:	// 5-2(3)-1
			if(m_Arith.l_test(tsr52.ts5231.val011) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val021) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val031) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val041) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val052) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val062) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val072) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val082) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val091) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val092) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val101) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val102) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val111) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val112) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val121) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val122) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val131) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val132) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val141) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val142) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val151) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val152) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5231.val090) != 0)	break;
			brv=FALSE;
			break;
		case	3:	// 5-2(3)-2
			if(m_Arith.l_test(tsr52.ts5232.val011) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val021) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val031) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val041) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val052) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val062) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val072) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val082) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val091) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val092) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val101) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val102) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val111) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val112) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val121) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val122) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val131) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val132) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val141) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val142) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val151) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val152) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5232.val090) != 0)	break;
			brv=FALSE;
			break;
		case	4:	// 5-2(3)-3
			if(m_Arith.l_test(tsr52.ts5233.val011) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val021) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val031) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val041) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val052) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val062) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val072) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val082) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val091) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val092) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val101) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val102) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val111) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val112) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val121) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val122) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val131) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val132) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val141) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val142) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val151) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val152) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5233.val090) != 0)	break;
			brv=FALSE;
			break;
		case	5:	// 5-2(4)-1
			if(m_Arith.l_test(tsr52.ts5241.val011) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val021) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val032) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val042) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val051) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val052) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val061) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val062) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val071) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val072) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val081) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val082) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val091) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val092) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val101) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val102) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5241.val050) != 0)	break;
			brv=FALSE;
			break;
		case	6:	// 5-2(4)-2
			if(m_Arith.l_test(tsr52.ts5242.val011) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val021) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val032) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val042) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val051) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val052) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val061) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val062) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val071) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val072) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val081) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val082) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val091) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val092) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val101) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val102) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5242.val050) != 0)	break;
			brv=FALSE;
			break;
		default:	// 5-2(4)-3
			if(m_Arith.l_test(tsr52.ts5243.val011) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val021) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val032) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val042) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val051) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val052) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val061) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val062) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val071) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val072) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val081) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val082) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val091) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val092) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val101) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val102) != 0)	break;
			if(m_Arith.l_test(tsr52.ts5243.val050) != 0)	break;
			brv=FALSE;
			break;
	}
	m_Arith.l_defn(def);

	return(brv);
}
// 240308_h_160357 add <--

// -------------------------------------------------------------------------------------------
// 特定収入計算表 入力制限の制御を返す
// -------------------------------------------------------------------------------------------
int CTaxationList31View::IsSpecificEnable(long *pFlg)
{
	long	eymd = 0;
	BOOL	syzSign = FALSE;

	//memset(m_ListType, '\0', sizeof(m_ListType));

	// 令和3年12月31日以降の課税期間
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	{
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else	{
		eymd = m_pSnHeadData->Sn_KDAYE;
	}

// 24/04/19特定収入追加 cor -->
	//if((m_pSnHeadData->SVmzsw == 1) || (eymd < R03YEAR))	{
// ----------------------------
	if(eymd < R03YEAR)	{
// 24/04/19特定収入追加 cor <--
		// 旧税率ありもしくは令和3年12月1日より前
		syzSign = TRUE;
	}

	// 帳票データ
	if(!m_pSpcListData) {
		if((m_pSpcListData = new CH31SpcListData(m_pSnHeadData->SVmzsw ? TRUE : FALSE)) == NULL) {
			return(-1);
		}

		if(m_pSpcListData->InitSpData(m_pZmsub)) {
			return(-1);
		}

		if(m_pSpcListData->GetData(m_pZmsub, m_pSnHeadData, m_SnSeq)) {
			return(-1);
		}
	}

	// 初期化
	*pFlg = 0x0000;

// 修正No.168719 add -->
	// 予定納税の時は選択できないのでここでリターン
	if(m_pSnHeadData->IsMiddleProvisional()) {	// 予定26号様式
		return(0);
	}
// 修正No.168719 add <--

// 修正No.168713 cor -->
//	// 計算表１
//	if(syzSign == FALSE)	{	// 旧税率なし
//		*pFlg |= 0x0001;
//	}
//
//	// 計算表２（１）
//	if(syzSign == FALSE)	{	// 旧税率なし
//		*pFlg |= 0x0002;
//	}
//
//	// 計算表２（２）
//	// 課税売上割合95%未満かつ個別対応方式
//	// 課税売上割合に準ずる割合を使用
//	if((m_pSnHeadData->IsKobetuSiireAnbun()) && (!(m_pSpcListData->IsRatioOver95(m_pSnHeadData)) || (m_pSnHeadData->IsUseSemiUriRatio() == TRUE))) {
//		if(syzSign == FALSE)	{	// 旧税率なし
//			*pFlg |= 0x0004;
//		}
//	}
//
//	// 計算表３、４
//	*pFlg |= 0x0008;
//
//	// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
//	if((m_pSpcListData->IsRatioOver95(m_pSnHeadData)) && (m_pSnHeadData->IsUseSemiUriRatio() == FALSE)) {
//		// 課税売上割合95%以上
//		// 計算表５（１）
//		if(syzSign == FALSE)	{	// 旧税率なし
//			*pFlg |= 0x0010;
//		}
//	}
//	else {
//		// 課税売上割合95%未満
//		if(m_pSnHeadData->IsKobetuSiireAnbun()) {
//			// 個別対応
//			// 計算表５（２）
//			if(syzSign == FALSE)	{	// 旧税率なし
//				*pFlg |= 0x0020;
//			}
//		}
//		else	{
//			// 比例配分
//			// 計算表５（３）
//			if(syzSign == FALSE)	{	// 旧税率なし
//				*pFlg |= 0x0040;
//			}
//		}
//	}
//
//// 24/03/14_税務代理書面 cor -->
//	//// 計算表５－２（１）
//	//*pFlg |= 0x0080;
//	//// 計算表５－２（２）
//	//*pFlg |= 0x0100;
//	//// 計算表５－２（３）－１
//	//*pFlg |= 0x0200;
//	//// 計算表５－２（３）－２
//	//*pFlg |= 0x0400;
//	//// 計算表５－２（３）－３
//	//if(eymd > KIKAN_END_INV80)	*pFlg |= 0x0800;
//	//// 計算表５－２（４）－１
//	//*pFlg |= 0x1000;
//	//// 計算表５－２（４）－２
//	//*pFlg |= 0x2000;
//	//// 計算表５－２（４）－３
//	//if(eymd > KIKAN_END_INV80)	*pFlg |= 0x4000;
//// -----------------------------
//	// ICS_SH_INVOICE_DAY＝2023/10/01、 KIKAN_END_INV80＝2026/09/30
//	// 計算表５－２（１）
//	if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0080;
//	// 計算表５－２（２）
//	if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0100;
//	// 計算表５－２（３）－１
//	if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0200;
//	// 計算表５－２（３）－２
//	if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0400;
//	// 計算表５－２（３）－３
//	if(eymd > KIKAN_END_INV80)		*pFlg |= 0x0800;
//	// 計算表５－２（４）－１
//	if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x1000;
//	// 計算表５－２（４）－２
//	if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x2000;
//	// 計算表５－２（４）－３
//	if(eymd > KIKAN_END_INV80)		*pFlg |= 0x4000;
//// 24/03/14_税務代理書面 cor <--
// ---------------------
	// 計算表１
	*pFlg |= 0x0001;

	// 計算表２（１）
	*pFlg |= 0x0002;

	// 計算表２（２）
	// 課税売上割合95%未満かつ個別対応方式
	// 課税売上割合に準ずる割合を使用
	if((m_pSnHeadData->IsKobetuSiireAnbun()) && (!(m_pSpcListData->IsRatioOver95(m_pSnHeadData)) || (m_pSnHeadData->IsUseSemiUriRatio() == TRUE))) {
		*pFlg |= 0x0004;
	}

	// 計算表３、４
	*pFlg |= 0x0008;

	// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
	if((m_pSpcListData->IsRatioOver95(m_pSnHeadData)) && (m_pSnHeadData->IsUseSemiUriRatio() == FALSE)) {
		// 課税売上割合95%以上
		// 計算表５（１）
		*pFlg |= 0x0010;
	}
	else {
		// 課税売上割合95%未満
		if(m_pSnHeadData->IsKobetuSiireAnbun())	{	// 個別対応
			// 計算表５（２）
			*pFlg |= 0x0020;
		}
		else	{									// 比例配分
			// 計算表５（３）
			*pFlg |= 0x0040;
		}
	}

	if(syzSign == FALSE)	{
		// ICS_SH_INVOICE_DAY＝2023/10/01、 KIKAN_END_INV80＝2026/09/30
		// 計算表５－２（１）
		if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0080;
		// 計算表５－２（２）
		if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0100;
		// 計算表５－２（３）－１
		if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0200;
		// 計算表５－２（３）－２
		if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x0400;
		// 計算表５－２（３）－３
		if(eymd > KIKAN_END_INV80)		*pFlg |= 0x0800;
		// 計算表５－２（４）－１
		if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x1000;
		// 計算表５－２（４）－２
		if(eymd > ICS_SH_INVOICE_DAY)	*pFlg |= 0x2000;
		// 計算表５－２（４）－３
		if(eymd > KIKAN_END_INV80)		*pFlg |= 0x4000;
	}
// 修正No.168713 cor <--

	return(0);
}
// 24/02/19wd_特定収入対応 add <--
