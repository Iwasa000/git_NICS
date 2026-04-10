// H31HyoView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "H31HyoView.h"

#include "ShinFhyo1_01.h"
#include "ShinFhyo1_02.h"
#include "ShinFhyo1_03.h"
#include "ShinFhyo2_01.h"
#include "ShinFhyo2_11.h"
#include "ShinFhyo2_11Ex.h"
#include "ShinFhyo2_02.h"
#include "ShinFhyo2_12.h"
#include "ShinFhyo2_12Ex.h"
#include "ShinFhyo2_03.h"
#include "ShinFhyo2_03Ex.h"
#include "ShinFhyo4_01.h"
#include "ShinFhyo4_02.h"
#include "ShinFhyo4_03.h"
#include "ShinFhyo5_01.h"
#include "ShinFhyo5_012.h"
#include "ShinFhyo5_02.h"
#include "ShinFhyo5_022.h"
#include "ShinFhyo5_03.h"
#include "ShinFhyo5_032.h"
#include "ShinFhyo6.h"
#include "ShinFhyo6_2wari.h"	//('23.05.01 Add)
#include "ShinIppan01.h"
#include "ShinIppan01EX.h"
#include "ShinIppan02.h"
#include "ShinIppan01EX2.h"		//('23.05.01 Add)
#include "ShinKani01.h"
#include "ShinKani01EX.h"
#include "ShinKani01EX2.h"		//('23.05.01 Add)
#include "ShinChskEX.h"
#include "ShinKjnKskEX.h"
#include "ShinHjnKskEX.h"
#include "ShinKjnKskEX2.h"
#include "ShinHjnKskEX2.h"
#include ".\SyzrefundDB\ShinRefundKjn01.h"
#include ".\SyzrefundDB\ShinRefundKjn02.h"
#include ".\SyzrefundDB\ShinRefundKjn02EX.h"	// 20230704
#include ".\SyzrefundDB\ShinRefundHjn01.h"
#include ".\SyzrefundDB\ShinRefundHjn02.h"
#include ".\SyzrefundDB\ShinRefundHjn02EX.h"	// 20230704

#include "ShinKSisanUri.h"
#include "ShinKsisan_Uri2.h"
#include "ShinKshiire.h"	

#include "ShinKskRnd.h"
#include "ShinKskRnd2.h"

//#include "CKoujoSelect.h"

//特例適用期間
#define SIIRETOKUREIDAY 20200930
#define URIAGETOKUREIDAY 20230930

// CH31HyoView

extern	HINSTANCE g_hInstance;


IMPLEMENT_DYNCREATE(CH31HyoView, ICSFormView)

CH31HyoView::CH31HyoView()
	: CBaseHyoView(CH31HyoView::IDD)
	, m_pFhyo1_01(NULL)
	, m_pFhyo1_02(NULL)
	, m_pFhyo1_03(NULL)
	, m_pFhyo2_01(NULL)
	, m_pFhyo2_11(NULL)
	, m_pFhyo2_11Ex(NULL)
	, m_pFhyo2_02(NULL)
	, m_pFhyo2_12(NULL)
	, m_pFhyo2_12Ex(NULL)
	, m_pFhyo2_03(NULL)
	, m_pFhyo2_03Ex(NULL)
	, m_pFhyo4_01(NULL)
	, m_pFhyo4_02(NULL)
	, m_pFhyo4_03(NULL)
	, m_pFhyo5_01(NULL)
	, m_pFhyo5_012(NULL)
	, m_pFhyo5_02(NULL)
	, m_pFhyo5_022(NULL)
	, m_pFhyo5_03(NULL)
	, m_pFhyo5_032(NULL)
	, m_pFhyo6(NULL)
	, m_pFhyo6_01(NULL)			//('23.05.01 Add)
	, m_pShinIppan01(NULL)
	, m_pShinIppan01EX(NULL)
	, m_pShinIppan01EX2(NULL)	//('23.05.01 Add)
	, m_pShinIppan02(NULL)
	, m_pShinKani01(NULL)
	, m_pShinKani01EX(NULL)
	, m_pShinKani01EX2(NULL)	//('23.05.01 Add)
	, m_pRefundKjn01(NULL)
	, m_pRefundKjn02(NULL)
	, m_pRefundKjn02EX(NULL)	// 20230704
	, m_pRefundHjn01(NULL)
	, m_pRefundHjn02(NULL)
	, m_pRefundHjn02EX(NULL)	// 20230704
	, m_pChskEX(NULL)
	, m_pHjnKskEX(NULL)
	, m_pKjnKskEX(NULL)
	, m_pHjnKskEX2(NULL)
	, m_pKjnKskEX2(NULL)
	//---------------------->
	, m_pSnHonpyoData(NULL)
	, m_pSnHonpyo2Data(NULL)
	, m_pSnFhyo1_01Data(NULL)
	, m_pSnFhyo1_02Data(NULL)
	, m_pSnFhyo2_01Data(NULL)
	, m_pSnFhyo2_02Data(NULL)
	, m_pSnTanaData(NULL)
	, m_pSnFhyo4_01Data ( NULL )
	, m_pSnFhyo4_02Data ( NULL )
	, m_pSnFhyo5_01Data(NULL)
	, m_pSnFhyo5_012Data(NULL)
	, m_pSnFhyo5_02Data(NULL)
	, m_pSnFhyo5_022Data(NULL)
	, m_pSnFhyo6Data(NULL)
	, m_pSnFhyo6_01Data(NULL)	//('23.05.01 Add)
	, m_pSnRefundData(NULL)
	, m_pSnChskData(NULL)
	, m_pSnKskData(NULL)
	, m_SnSeq(-1)
	//<----------------------
	, m_pSyzSyukei(NULL)
	, m_pFirstPage(NULL)
	, m_SixKindFlg(TRUE)
	, m_bShowed(FALSE)
	, m_nowTabIdx(0)
	, m_dspSkjSw(0)
	, m_tkSw(0)
	, m_typeCnt(0)
	, m_NoHistory_browse(FALSE)
	, m_pKsisanuri(NULL)
	, m_pKsisanuri2(NULL)
	, m_pKshiire(NULL)
	, m_ShRevType ( ID_ICSSH_NO_REVTYPE )
	, m_DispTabSgn(0)
	, m_pVprn ( NULL )
	, m_pTblhdl(NULL)
	, m_pSkjCnvtbl(NULL)
	, m_Syokai_NukiKomi(FALSE)
	, m_Uri1MxPage(1)
	, m_Uri2MxPage(1)
    , m_ShiireMxPage(1)
	, m_isInited(false)
{
}

CH31HyoView::~CH31HyoView()
{
}

void CH31HyoView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_MainTab);
}

BEGIN_MESSAGE_MAP(CH31HyoView, ICSFormView)
	ON_NOTIFY(TCN_SELCHANGING, IDC_MAIN_TAB, OnTcnSelchangingMainTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, OnTcnSelchangeMainTab)

	ON_COMMAND(ID_BUTTON_F2, OnButtonF2)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_BUTTON_F6, OnButtonF6)
	ON_COMMAND(ID_BUTTON_F7, OnButtonF7)
	ON_COMMAND(ID_BUTTON_F8, &CH31HyoView::OnButtonF8)
	ON_COMMAND(ID_BUTTON_F9, &CH31HyoView::OnButtonF9)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)
	ON_COMMAND(ID_BUTTON_F12, OnButtonF12)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F4, &CH31HyoView::OnUpdateButtonF4)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CH31HyoView::OnUpdateButtonF5)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F6, &CH31HyoView::OnUpdateButtonF6)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CH31HyoView::OnUpdateButtonF7)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, &CH31HyoView::OnUpdateButtonF8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CH31HyoView::OnUpdateButtonF9)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, &CH31HyoView::OnUpdateButtonF11)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, &CH31HyoView::OnUpdateButtonF12)

	ON_UPDATE_COMMAND_UI(ID_KOMONSAKI_INF, &CH31HyoView::OnUpdateKomonsakiInf)
	ON_UPDATE_COMMAND_UI(ID_ZEIRISHI_INF, &CH31HyoView::OnUpdateZeirishiInf)
	ON_COMMAND(ID_ZEIRISHI_INF, &CH31HyoView::OnZeirishiInf)

	ON_WM_SIZE()
	ON_WM_VSCROLL()

	ON_MESSAGE( WM_MYMESSAGE, OnMyMessage )
END_MESSAGE_MAP()


// CH31HyoView 診断

#ifdef _DEBUG
void CH31HyoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CH31HyoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


LRESULT CH31HyoView::OnMyMessage( WPARAM wParam, LPARAM lParam )
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	int selno = m_MainTab.GetCurSel();
	if( selno != -1 ){
		SetItemPosition( selno );
	}

	AfxSetResourceHandle( hInst );

	return 0;
}

// CH31HyoView メッセージ ハンドラ

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 引数	pZmsub			：	公益クラスのポインタ
//		pSyzSyukei		：	消費税集計クラスのポインタ
//		pSubSyzSyukei	：	消費税集計クラス（サブ集計）のポインタ
//		pShinInfo		：	共通情報のポインタ
//		pSnHeadData		：	消費税ヘッダ情報
//		pTblhdl			：	テーブルハンドリング
//		pSkjCnvtbl		：	仕入控除用テーブル
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
//--->yoshida190627
int CH31HyoView::InitView(CDBNpSub *pZmsub, CSyzSyukei *pSyzSyukei, CSnHeadData *pSnHeadData, SYC_SHININFO *pShinInfo, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl)
//------
//int CH31HyoView::InitView(  CDBNpSub *pZmsub , CSyzSyukei *pSyzSyukei , CSnHeadData *pSnHeadData , SYC_SHININFO *pShinInfo)
//<------
{
	m_isInited = false;
	
	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		return -1;
	}
	m_pZmSub = pZmsub;


	ASSERT(pSyzSyukei);
	if (pSyzSyukei == NULL) {
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;


	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;


	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	//---->yoshida190627
	ASSERT(pTblhdl);
	if (pTblhdl == NULL) {
		return -1;
	}
	m_pTblhdl = pTblhdl;

	ASSERT(pSkjCnvtbl);
	if (pSkjCnvtbl == NULL) {
		return -1;
	}
	m_pSkjCnvtbl = pSkjCnvtbl;
	//<-------------------
	
	GetClientRect( vwRect );

	m_Arith.l_defn(0x16);

	m_PastYoteiSgn = FALSE;		//[23-0203]('24.07.23 Add)
	
	if( InitTab(TRUE) ){
		//m_ErrMsg = _T("本表・付表画面の内部作成に失敗しました。");
		return -1;
	}

	//yoshida190627
	if (m_pZmSub->szvol->SVsasw & 0x01) {
		m_Syokai_NukiKomi = FALSE;
	}
	else {
		m_Syokai_NukiKomi = TRUE;
	}

	m_isInited = true;

	return 0;
}


//-----------------------------------------------------------------------------
// 内部タブの初期化
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::InitTab( BOOL bDelete )
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	// コンバージョン
	BOOL IsNeedCnv = IsNeedOldDataCnv();

	this->LockWindowUpdate();

	//int rt = InitTabSub( bDelete );
	int rt = 0;
	if ( !( m_pSnHeadData->m_DispTabSgn & 0x1000 ) ) {
		rt = InitOldFormTab ( bDelete );
	}
	else {
		rt = InitTabSub ( bDelete );
	}

	this->UnlockWindowUpdate();

	AfxSetResourceHandle( hInst );

	if ( IsNeedCnv ) {
		GetPastData();
	}

	return rt;
}


//-----------------------------------------------------------------------------
// 内部タブの初期化
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::InitTabSub( BOOL bDelete )
{
	if (bDelete) {
		// 内部クラスを破棄？
		DeleteInsideDlg();
		// 全削除
		m_MainTab.DeleteAllItems();
	}

	ICSDialog	*ptmpTab[16] = {0};
	SH_TABMNG	shTabmng;
	int			firstPage = 0;

	int	tabIdx = 0;
	bool isRefundDialog = false;	// 還付付表作成ダイアログ

	BOOL	isTransitionalMeasure = FALSE;

	// 入力ロックを解除すると非連動サインと経過措置サインがクリアされるため、タブを作成するタイミングでもサインをセットする
	// 非連動サイン ('19.05.24)harino
	char flg[MONY_BUF_SIZE] = { 0 };
	if (m_pSnHeadData->Sn_Sign4 & 0x80) m_Arith.l_input(flg, "1");
	m_pSyzSyukei->SetShinkokuData(_T("FFFFFC"), flg);


	// ---->経過措置サイン yoshida200110
	// 出荷延期。出荷日決まり次第、計算テーブルと一緒に対応をあける。	'20.01.22 出荷
	char keikaflg[MONY_BUF_SIZE] = { 0 };
	if (m_pSnHeadData->SVmzsw == 1) m_Arith.l_input(keikaflg, "1");//経過措置あり
	m_pSyzSyukei->SetShinkokuData(_T("FFFFFB"), keikaflg);
	//<-----

	// 令和2年4月1日以降の課税期間であれば、納税者有利になるよう計算
	long eymd = 0;
	char adv[MONY_BUF_SIZE] = { 0 };
	if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if ( eymd >= TAXADVANTAGE ) {
		m_Arith.l_input ( adv, _T( "1" ) );
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF4000" ), adv );

	// 令和2年8月1日以降の課税期間または、修正申告・更正の請求であれば、簡易課税の計算を修正
	memset ( adv, '\0', sizeof ( adv ) );
	if ( eymd >= TAXMODIFY ) {
		m_Arith.l_input ( adv, _T( "1" ) );
	}
	else {
		if ( ( ( m_pSnHeadData->Sn_SKKBN == 3 ) || ( m_pSnHeadData->Sn_SKKBN == 4 ) ) || ( m_pSnHeadData->Sn_Syuusei & 0x02 ) ) {
			m_Arith.l_input ( adv, _T( "1" ) );
		}
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF5000" ), adv );

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(m_pZmSub->zvol);

	// 新様式？
	memset ( adv, '\0', sizeof ( adv ) );
	if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
		if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
			if ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) {
				m_Arith.l_input ( adv, _T( "1" ) );
			}
		}
		else {
			m_Arith.l_input ( adv, _T( "1" ) );
		}
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF6000" ), adv );

	// 集計フラグのセット
	SetSyukeiFlg();

	// 更正の請求/新様式対応
	// 令和4年12月31日以後の課税期間の場合は、更正の請求の「この請求前の金額」列をカット
	// 「この請求前の既確定税額」項目の追加
	BOOL bKskModify = FALSE;
	if( eymd >= KSKMODIFY ){
		bKskModify = TRUE;
	}

	memset(&shTabmng, '\0', sizeof(SH_TABMNG));
	if (m_pSnHeadData->IsMiddleProvisional()) {	// 前年実績による中間申告（第26号様式）
		// 本表データ
		m_pSnHonpyoData = new CH31SnHonpyoData();
		if (m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}

		// 中間予定申告データ
		if (!m_pSnChskData) {
			m_pSnChskData = new CH31SnChskData();
			if (m_pSnChskData->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		//										予定申告（２６号）										   //
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		if (!m_pChskEX) {
			m_pChskEX = new CShinChskEX(this);
			m_pChskEX->InitInfo(m_pSnHeadData, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo);
			m_pChskEX->Create(IDD_DIALOG_CHSK_EX, &m_MainTab);
		}
		m_MainTab.InsertItem(tabIdx, _T("中間申告(第26号)"), m_pChskEX->m_hWnd);
		ptmpTab[tabIdx] = m_pChskEX;
		tabIdx++;
		shTabmng.tabtype = ID_ICSSH_CHSK_TAB;
		shTabmng.pWnd = m_pChskEX;
		m_TabMng.Add(shTabmng);
	}
	else {
		//EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(m_pZmSub->zvol);

		// 本表データ
		m_pSnHonpyoData = new CH31SnHonpyoData();
		if (m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}

		m_pSnHonpyo2Data = new CH31SnHonpyo2Data();
		if (m_pSnHonpyo2Data->GetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}

		//特例チェックONの場合、特例計算帳票表示
		//基本情報画面の特例チェックをまず判断する。チェック無い場合は表示しない
		if (m_pSnHeadData->m_DispTabSgn & 0x80) {
			if (m_pSnHeadData->m_DispTabSgn & 0x10) {//
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//			第５-(１)様式 課税試算の譲渡等の対価の額の計算表（軽減売上割合１０営業日）			   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if (!m_pKsisanuri) {
					m_pKsisanuri = new CShinKSisanUri(this);
					m_pKsisanuri->InitInfo(&m_pSnHeadData, m_pShinInfo, &m_Arith, m_pZmSub, &m_pSyzSyukei, m_SnSeq);
					m_pKsisanuri->Create(IDD_DIALOG_KSISAN_URI, &m_MainTab);
				}
				m_MainTab.InsertItem(tabIdx, _T("第５-(１)号様式"), m_pKsisanuri->m_hWnd);
				ptmpTab[tabIdx] = m_pKsisanuri;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSISANURI_TAB;
				shTabmng.pWnd = m_pKsisanuri;
				m_TabMng.Add(shTabmng);

			}

			if (m_pSnHeadData->m_DispTabSgn & 0x20) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//			第５-(２)様式 課税資産の譲渡等の対価の額の計算表（小売等軽減仕入割合）				　 //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if (!m_pKsisanuri2) {
					m_pKsisanuri2 = new CShinKsisan_Uri2(this);
					m_pKsisanuri2->InitInfo(&m_pSnHeadData, m_pShinInfo, &m_Arith, m_pZmSub, &m_pSyzSyukei, m_SnSeq);
					m_pKsisanuri2->Create(IDD_DIALOG_KSISAN_URI2, &m_MainTab);
				}
				m_MainTab.InsertItem(tabIdx, _T("第５-(２)号様式"), m_pKsisanuri2->m_hWnd);
				ptmpTab[tabIdx] = m_pKsisanuri2;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSISANURI2_TAB;
				shTabmng.pWnd = m_pKsisanuri2;
				m_TabMng.Add(shTabmng);
			}

			if ( m_pSnHeadData->m_DispTabSgn & 0x40 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//						第５-(３)様式 課税仕入れ等の税額の計算表								   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if (!m_pKshiire) {
					m_pKshiire = new CShinKshiire(this);
					m_pKshiire->InitInfo(&m_pSnHeadData, m_pShinInfo, &m_Arith, m_pZmSub, &m_pSyzSyukei, m_SnSeq);
					m_pKshiire->Create(IDD_DIALOG_KSHIIRE, &m_MainTab);
				}
				m_MainTab.InsertItem(tabIdx, _T("第５-(３)号様式"), m_pKshiire->m_hWnd);
				ptmpTab[tabIdx] = m_pKshiire;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSHIIRE_TAB;
				shTabmng.pWnd = m_pKshiire;
				m_TabMng.Add(shTabmng);
			}
		}

		if (KzHoushiki == ID_ICSSH_KANNI) {	// 簡易課税

			// 付表４データ
			m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(TRUE);
			if (m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
			m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data(TRUE);
			if (m_pSnFhyo4_02Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}

			// 付表５データ
			m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(TRUE);
			if (m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
			m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data(TRUE);
			if (m_pSnFhyo5_012Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
			m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data(TRUE);
			if (m_pSnFhyo5_02Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
			m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data(TRUE);
			if (m_pSnFhyo5_022Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
//----->('23.05.01 Add)
			m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(TRUE);
			if (m_pSnFhyo6_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
//<-----

			// --> 191217
			// 簡易本表：入力ロックして業務を終了すると4の項目に15の金額が上書きされていた不具合を修正
			// 修正申告の時に本表26の金額に100円未満の金額が表示されていた不具合を修正
			if( !(m_pSnHeadData->Sn_GenericSgn & 0x04) ){
				m_pSnHeadData->Sn_GenericSgn |= 0x04;

				if( m_pSnHeadData->Sn_Sign4 & 0x01 ){
					memmove( m_pSnHonpyoData->Sn_SIREZ, m_pSnFhyo4_01Data->Sn_F401_4F, MONY_BUF_SIZE );

					if (m_pSnHeadData->Sn_SKKBN == 3 || m_pSnHeadData->Sn_SKKBN == 4){	// 修正申告
						m_Arith.l_add( m_pSnHonpyoData->Sn_SYOTSG, m_pSnHonpyoData->Sn_SHN, m_pSnHonpyoData->Sn_SHNJ );
					}
				}
			}
			// <-- 191217
			if ((eymd < ICS_SH_INVOICE_DAY) || ((m_pSnHeadData->m_DispTabSgn & 0x2000) == 0)) {	//２割特例を適用しない場合
				// 経過措置対象の課税資産の譲渡等あり
				if (m_pSnHeadData->SVmzsw == 1) {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表４－０２											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo4_02) {
						m_pFhyo4_02 = new CShinFhyo4_02(this);
						m_pFhyo4_02->InitInfo(&m_pSnFhyo4_02Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
						m_pFhyo4_02->Create(IDD_DIALOG_FHY4_02, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表４－２"), m_pFhyo4_02->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo4_02;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO4_02_TAB;
					shTabmng.pWnd = m_pFhyo4_02;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表４－０１											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo4_01) {
						m_pFhyo4_01 = new CShinFhyo4_01(this);
						m_pFhyo4_01->InitInfo(&m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
						m_pFhyo4_01->Create(IDD_DIALOG_FHY4_01, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表４－１"), m_pFhyo4_01->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo4_01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO4_01_TAB;
					shTabmng.pWnd = m_pFhyo4_01;
					m_TabMng.Add(shTabmng);
				}
				else {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表４－０３											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo4_03) {
						m_pFhyo4_03 = new CShinFhyo4_03(this);
						m_pFhyo4_03->InitInfo(&m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith);
						m_pFhyo4_03->Create(IDD_DIALOG_FHY4_03, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表４－３"), m_pFhyo4_03->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo4_03;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO4_03_TAB;
					shTabmng.pWnd = m_pFhyo4_03;
					m_TabMng.Add(shTabmng);
				}


				// 経過措置対象の課税資産の譲渡等あり
				if (m_pSnHeadData->SVmzsw == 1) {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０２											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_02) {
						m_pFhyo5_02 = new CShinFhyo5_02(this);
						m_pFhyo5_02->InitInfo(&m_pSnFhyo5_02Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith/*, m_pShinInfo*/, m_pZmSub, &m_pSyzSyukei);
						m_pFhyo5_02->Create(IDD_DIALOG_FHY5_02, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－２"), m_pFhyo5_02->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_02;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_02_TAB;
					shTabmng.pWnd = m_pFhyo5_02;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０２(2)											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_022) {
						m_pFhyo5_022 = new CShinFhyo5_022(this);
						m_pFhyo5_022->InitInfo(&m_pSnFhyo5_022Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith/*, m_pShinInfo*/, &m_pSyzSyukei);
						m_pFhyo5_022->Create(IDD_DIALOG_FHY5_022, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－２(2)"), m_pFhyo5_022->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_022;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_022_TAB;
					shTabmng.pWnd = m_pFhyo5_022;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０１											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_01) {
						m_pFhyo5_01 = new CShinFhyo5_01(this);
						m_pFhyo5_01->InitInfo(&m_pSnFhyo5_01Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith, &m_pSyzSyukei/*, m_pShinInfo*/);
						m_pFhyo5_01->Create(IDD_DIALOG_FHY5_01, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－１"), m_pFhyo5_01->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_01_TAB;
					shTabmng.pWnd = m_pFhyo5_01;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０１(2)											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_012) {
						m_pFhyo5_012 = new CShinFhyo5_012(this);
						m_pFhyo5_012->InitInfo(&m_pSnFhyo5_012Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_pSyzSyukei, &m_Arith/*, m_pShinInfo*/);
						m_pFhyo5_012->Create(IDD_DIALOG_FHY5_012, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－１(2)"), m_pFhyo5_012->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_012;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_012_TAB;
					shTabmng.pWnd = m_pFhyo5_012;
					m_TabMng.Add(shTabmng);
				}
				else {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０３											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_03) {
						m_pFhyo5_03 = new CShinFhyo5_03(this);
						m_pFhyo5_03->InitInfo(&m_pSnFhyo5_01Data, &m_pSnHeadData, &m_Arith, &m_pSyzSyukei);
						m_pFhyo5_03->Create(IDD_DIALOG_FHY5_03, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－３"), m_pFhyo5_03->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_03;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_03_TAB;
					shTabmng.pWnd = m_pFhyo5_03;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０３(2)											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_032) {
						m_pFhyo5_032 = new CShinFhyo5_032(this);
						m_pFhyo5_032->InitInfo(&m_pSnFhyo5_012Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith);
						m_pFhyo5_032->Create(IDD_DIALOG_FHY5_032, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－３(2)"), m_pFhyo5_032->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_032;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_032_TAB;
					shTabmng.pWnd = m_pFhyo5_032;
					m_TabMng.Add(shTabmng);
				}
			}
			else {	//２割特例を適用する(「２割特例を設定する」ON かつ 課税期間＞20231001 )場合

				//----> 20230616
				// 経過措置対象の課税資産の譲渡等あり
				if (m_pSnHeadData->SVmzsw == 1) {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表４－０２											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo4_02) {
						m_pFhyo4_02 = new CShinFhyo4_02(this);
						m_pFhyo4_02->InitInfo(&m_pSnFhyo4_02Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
						m_pFhyo4_02->Create(IDD_DIALOG_FHY4_02, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表４－２"), m_pFhyo4_02->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo4_02;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO4_02_TAB;
					shTabmng.pWnd = m_pFhyo4_02;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表４－０１											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo4_01) {
						m_pFhyo4_01 = new CShinFhyo4_01(this);
						m_pFhyo4_01->InitInfo(&m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
						m_pFhyo4_01->Create(IDD_DIALOG_FHY4_01, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表４－１"), m_pFhyo4_01->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo4_01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO4_01_TAB;
					shTabmng.pWnd = m_pFhyo4_01;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０２											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_02) {
						m_pFhyo5_02 = new CShinFhyo5_02(this);
						m_pFhyo5_02->InitInfo(&m_pSnFhyo5_02Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith/*, m_pShinInfo*/, m_pZmSub, &m_pSyzSyukei);
						m_pFhyo5_02->Create(IDD_DIALOG_FHY5_02, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－２"), m_pFhyo5_02->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_02;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_02_TAB;
					shTabmng.pWnd = m_pFhyo5_02;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０２(2)											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_022) {
						m_pFhyo5_022 = new CShinFhyo5_022(this);
						m_pFhyo5_022->InitInfo(&m_pSnFhyo5_022Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith/*, m_pShinInfo*/, &m_pSyzSyukei);
						m_pFhyo5_022->Create(IDD_DIALOG_FHY5_022, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－２(2)"), m_pFhyo5_022->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_022;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_022_TAB;
					shTabmng.pWnd = m_pFhyo5_022;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０１											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_01) {
						m_pFhyo5_01 = new CShinFhyo5_01(this);
						m_pFhyo5_01->InitInfo(&m_pSnFhyo5_01Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith, &m_pSyzSyukei/*, m_pShinInfo*/);
						m_pFhyo5_01->Create(IDD_DIALOG_FHY5_01, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－１"), m_pFhyo5_01->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_01_TAB;
					shTabmng.pWnd = m_pFhyo5_01;
					m_TabMng.Add(shTabmng);

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表５－０１(2)											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo5_012) {
						m_pFhyo5_012 = new CShinFhyo5_012(this);
						m_pFhyo5_012->InitInfo(&m_pSnFhyo5_012Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_pSyzSyukei, &m_Arith/*, m_pShinInfo*/);
						m_pFhyo5_012->Create(IDD_DIALOG_FHY5_012, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表５－１(2)"), m_pFhyo5_012->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo5_012;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO5_012_TAB;
					shTabmng.pWnd = m_pFhyo5_012;
					m_TabMng.Add(shTabmng);				
				}
				//<---- 20230616
				else{
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表６（２割特例）										   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo6_01) {
						m_pFhyo6_01 = new CShinFhyo6_2wari(this);
						m_pFhyo6_01->InitInfo(&m_pSnFhyo6_01Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith);
						m_pFhyo6_01->Create(IDD_DIALOG_FHY6_2WARI, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表６"), m_pFhyo6_01->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo6_01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO6_01_TAB;
					shTabmng.pWnd = m_pFhyo6_01;
					m_TabMng.Add(shTabmng);
				}
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										簡易本表												   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//------->２割特例用の簡易本表を追加('23.05.01 Add)
			if ( eymd >= ICS_SH_INVOICE_DAY ) {
				// 令和5年10月1日以降の課税期間かつ、「２割特例計算を使用する」チェックがON
				if (!m_pShinKani01EX2) {
					m_pShinKani01EX2 = new CShinKani01EX2(this);
					m_pShinKani01EX2->InitInfo(&m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei);
					m_pShinKani01EX2->Create(IDD_DIALOG_KANIEX2, &m_MainTab);
				}
				m_MainTab.InsertItem(tabIdx, _T("簡易本表"), m_pShinKani01EX2->m_hWnd);
				ptmpTab[tabIdx] = m_pShinKani01EX2;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KANI01_TAB;
				shTabmng.pWnd = m_pShinKani01EX2;
				m_TabMng.Add(shTabmng);
			}
//<-------
			else if(( eymd >= R04REFORM ) && ( m_pSnHeadData->IsSoleProprietor() )) {
				// 令和4年4月1日以降の課税期間かつ、個人業種
				if( !m_pShinKani01EX ) {
					m_pShinKani01EX = new CShinKani01EX(this);
					m_pShinKani01EX->InitInfo( &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei );
					m_pShinKani01EX->Create( IDD_DIALOG_KANIEX, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T( "簡易本表" ), m_pShinKani01EX->m_hWnd );
				ptmpTab[tabIdx] = m_pShinKani01EX;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KANI01_TAB;
				shTabmng.pWnd = m_pShinKani01EX;
				m_TabMng.Add( shTabmng );
			}
			else {
				if (!m_pShinKani01) {
					m_pShinKani01 = new CShinKani01(this);
					m_pShinKani01->InitInfo( &m_pSnHeadData,/* m_pSnChskData, */&m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei );
					m_pShinKani01->Create(IDD_DIALOG_KANI, &m_MainTab);
				}
				m_MainTab.InsertItem(tabIdx, _T("簡易本表"), m_pShinKani01->m_hWnd);
				ptmpTab[tabIdx] = m_pShinKani01;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KANI01_TAB;
				shTabmng.pWnd = m_pShinKani01;
				m_TabMng.Add(shTabmng);
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//								課税標準額等の内訳書											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pShinIppan02 ) {
				m_pShinIppan02 = new CShinIppan02 ( this );
				m_pShinIppan02->InitInfo ( &m_pSnHeadData, &m_pSnHonpyo2Data, &m_Arith, m_pShinInfo, m_pZmSub, &m_pSyzSyukei );
				m_pShinIppan02->Create ( IDD_DIALOG_IPPAN02, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "課税標準額等の内訳書" ), m_pShinIppan02->m_hWnd );
//			m_MainTab.InsertItem ( tabIdx, _T( "簡易本表２" ), m_pShinIppan02->m_hWnd );
			ptmpTab[tabIdx] = m_pShinIppan02;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_IPPAN02_TAB;
			shTabmng.pWnd = m_pShinIppan02;
			m_TabMng.Add(shTabmng);
		}
		else {	// 原則課税

			// 付表１データ
			m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data(TRUE);
			if (m_pSnFhyo1_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
			m_pSnFhyo1_02Data = new CH31SnFhyo1_02Data(TRUE);
			if (m_pSnFhyo1_02Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}

			// 付表２データ
			m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data(TRUE);
			if (m_pSnFhyo2_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
			m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data(TRUE);
			if (m_pSnFhyo2_02Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}

	//----->('23.05.01 Add) 　
			// 付表６データ
			m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(TRUE);
			if (m_pSnFhyo6_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
	//<-----

			//----> 20230616
			if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
				// 付表４データ
				m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(TRUE);
				if (m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
				m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data(TRUE);
				if (m_pSnFhyo4_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}

				// 付表５データ
				m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(TRUE);
				if (m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
				m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data(TRUE);
				if (m_pSnFhyo5_012Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
				m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data(TRUE);
				if (m_pSnFhyo5_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
				m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data(TRUE);
				if (m_pSnFhyo5_022Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
			//<---- 20230616

			// 棚卸データ
			m_pSnTanaData = new CH31SnTanaData(TRUE);
			if (m_pSnTanaData->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}

			// --> 191217
			// 簡易本表：入力ロックして業務を終了すると4の項目に15の金額が上書きされていた不具合を修正
			// 修正申告の時に本表26の金額に100円未満の金額が表示されていた不具合を修正
			if( !(m_pSnHeadData->Sn_GenericSgn & 0x04) ){
				m_pSnHeadData->Sn_GenericSgn |= 0x04;
				if( m_pSnHeadData->Sn_Sign4 & 0x01 ){
					if (m_pSnHeadData->Sn_SKKBN == 3 || m_pSnHeadData->Sn_SKKBN == 4){	// 修正申告
						m_Arith.l_add( m_pSnHonpyoData->Sn_SYOTSG, m_pSnHonpyoData->Sn_SHN, m_pSnHonpyoData->Sn_SHNJ );
					}
				}
			}
			// <-- 191217

			if( eymd < ICS_SH_INVOICE_DAY ){
				// インボイス期間外　既存の付表
				if (m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR) {
					// 経過措置対象の課税資産の譲渡等あり
					if (m_pSnHeadData->SVmzsw == 1) {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表２－０２											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo2_12) {
							m_pFhyo2_12 = new CShinFhyo2_12(this);
							m_pFhyo2_12->InitInfo(&m_pSnFhyo2_02Data, &m_pSnHeadData, &m_pSnTanaData, &m_Arith, m_pShinInfo, &m_pSyzSyukei);
							m_pFhyo2_12->Create(IDD_DIALOG_FHY2_12, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表２－２"), m_pFhyo2_12->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo2_12;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO2_02_TAB;
						shTabmng.pWnd = m_pFhyo2_12;
						m_TabMng.Add(shTabmng);

						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表２－０１											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo2_11) {
							m_pFhyo2_11 = new CShinFhyo2_11(this);
							m_pFhyo2_11->InitInfo(&m_pSnFhyo2_01Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnTanaData, &m_pSyzSyukei);
							m_pFhyo2_11->Create(IDD_DIALOG_FHY2_11, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表２－１"), m_pFhyo2_11->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo2_11;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO2_01_TAB;
						shTabmng.pWnd = m_pFhyo2_11;
						m_TabMng.Add(shTabmng);
					}
					else {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表２－０３											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo2_03) {
							m_pFhyo2_03 = new CShinFhyo2_03(this);
							m_pFhyo2_03->InitInfo(&m_pSnFhyo2_01Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnTanaData, &m_pSyzSyukei);
							m_pFhyo2_03->Create(IDD_DIALOG_FHY2_03, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表２－３"), m_pFhyo2_03->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo2_03;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO2_03_TAB;
						shTabmng.pWnd = m_pFhyo2_03;
						m_TabMng.Add(shTabmng);
					}
				}
				else {
					if (m_pSnHeadData->SVmzsw == 1) {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表２－０２											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo2_02) {
							m_pFhyo2_02 = new CShinFhyo2_02(this);
							m_pFhyo2_02->InitInfo(&m_pSnFhyo2_02Data, &m_pSnHeadData, &m_pSnTanaData, &m_Arith, m_pShinInfo, &m_pSyzSyukei/*, m_pSnChskData, m_pSnHonpyoData*/);
							m_pFhyo2_02->Create(IDD_DIALOG_FHY2_02, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表２－２"), m_pFhyo2_02->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo2_02;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO2_02_TAB;
						shTabmng.pWnd = m_pFhyo2_02;
						m_TabMng.Add(shTabmng);
					}

					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表２－０１											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo2_01) {
						m_pFhyo2_01 = new CShinFhyo2_01(this);
						m_pFhyo2_01->InitInfo(&m_pSnFhyo2_01Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnTanaData, &m_pSyzSyukei/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith*/);
						m_pFhyo2_01->Create(IDD_DIALOG_FHY2_01, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表２－１"), m_pFhyo2_01->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo2_01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO2_01_TAB;
					shTabmng.pWnd = m_pFhyo2_01;
					m_TabMng.Add(shTabmng);
				}


				// 経過措置対象の課税資産の譲渡等あり
				if (m_pSnHeadData->SVmzsw == 1) {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表１－０２											　 //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo1_02) {
						m_pFhyo1_02 = new CShinFhyo1_02(this);
						m_pFhyo1_02->InitInfo(&m_pSnFhyo1_02Data, &m_pSnHeadData, m_pShinInfo, &m_Arith, &m_pSyzSyukei/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo */);
						m_pFhyo1_02->Create(IDD_DIALOG_FHY1_02, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表１－２"), m_pFhyo1_02->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo1_02;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO1_02_TAB;
					shTabmng.pWnd = m_pFhyo1_02;
					m_TabMng.Add(shTabmng);
				}

				if ((m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR) && (m_pSnHeadData->SVmzsw == 0)) {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表１－０３											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo1_03) {
						m_pFhyo1_03 = new CShinFhyo1_03(this);
						m_pFhyo1_03->InitInfo(&m_pSnFhyo1_01Data, &m_pSnHeadData, m_pShinInfo, &m_Arith, &m_pSyzSyukei);
						m_pFhyo1_03->Create(IDD_DIALOG_FHY1_03, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表１－３"), m_pFhyo1_03->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo1_03;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO1_03_TAB;
					shTabmng.pWnd = m_pFhyo1_03;
					m_TabMng.Add(shTabmng);
				}
				else {
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					//										付表１－０１											   //
					//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
					if (!m_pFhyo1_01) {
						m_pFhyo1_01 = new CShinFhyo1_01(this);
						m_pFhyo1_01->InitInfo(&m_pSnFhyo1_01Data, &m_pSnHeadData, m_pShinInfo, &m_Arith, &m_pSyzSyukei/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
						m_pFhyo1_01->Create(IDD_DIALOG_FHY1_01, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("付表１－１"), m_pFhyo1_01->m_hWnd);
					ptmpTab[tabIdx] = m_pFhyo1_01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO1_01_TAB;
					shTabmng.pWnd = m_pFhyo1_01;
					m_TabMng.Add(shTabmng);
				}
			}
			else{
				// インボイス期間内

				if( m_pSnHeadData->m_DispTabSgn & 0x2000 ){

					if( m_pSnHeadData->SVmzsw ){
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表４－０２											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo4_02) {
							m_pFhyo4_02 = new CShinFhyo4_02(this);
							m_pFhyo4_02->InitInfo(&m_pSnFhyo4_02Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
							m_pFhyo4_02->Create(IDD_DIALOG_FHY4_02, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表４－２"), m_pFhyo4_02->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo4_02;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO4_02_TAB;
						shTabmng.pWnd = m_pFhyo4_02;
						m_TabMng.Add(shTabmng);

						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表４－０１											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo4_01) {
							m_pFhyo4_01 = new CShinFhyo4_01(this);
							m_pFhyo4_01->InitInfo(&m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
							m_pFhyo4_01->Create(IDD_DIALOG_FHY4_01, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表４－１"), m_pFhyo4_01->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo4_01;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO4_01_TAB;
						shTabmng.pWnd = m_pFhyo4_01;
						m_TabMng.Add(shTabmng);

						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表５－０２											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo5_02) {
							m_pFhyo5_02 = new CShinFhyo5_02(this);
							m_pFhyo5_02->InitInfo(&m_pSnFhyo5_02Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith/*, m_pShinInfo*/, m_pZmSub, &m_pSyzSyukei);
							m_pFhyo5_02->Create(IDD_DIALOG_FHY5_02, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表５－２"), m_pFhyo5_02->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo5_02;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO5_02_TAB;
						shTabmng.pWnd = m_pFhyo5_02;
						m_TabMng.Add(shTabmng);

						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表５－０２(2)											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo5_022) {
							m_pFhyo5_022 = new CShinFhyo5_022(this);
							m_pFhyo5_022->InitInfo(&m_pSnFhyo5_022Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith/*, m_pShinInfo*/, &m_pSyzSyukei);
							m_pFhyo5_022->Create(IDD_DIALOG_FHY5_022, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表５－２(2)"), m_pFhyo5_022->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo5_022;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO5_022_TAB;
						shTabmng.pWnd = m_pFhyo5_022;
						m_TabMng.Add(shTabmng);

						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表５－０１											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo5_01) {
							m_pFhyo5_01 = new CShinFhyo5_01(this);
							m_pFhyo5_01->InitInfo(&m_pSnFhyo5_01Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_Arith, &m_pSyzSyukei/*, m_pShinInfo*/);
							m_pFhyo5_01->Create(IDD_DIALOG_FHY5_01, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表５－１"), m_pFhyo5_01->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo5_01;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO5_01_TAB;
						shTabmng.pWnd = m_pFhyo5_01;
						m_TabMng.Add(shTabmng);

						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表５－０１(2)											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo5_012) {
							m_pFhyo5_012 = new CShinFhyo5_012(this);
							m_pFhyo5_012->InitInfo(&m_pSnFhyo5_012Data, &m_pSnHeadData/*, m_pSnChskData, m_pSnHonpyoData*/, &m_pSyzSyukei, &m_Arith/*, m_pShinInfo*/);
							m_pFhyo5_012->Create(IDD_DIALOG_FHY5_012, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表５－１(2)"), m_pFhyo5_012->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo5_012;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO5_012_TAB;
						shTabmng.pWnd = m_pFhyo5_012;
						m_TabMng.Add(shTabmng);
					}
					else{
						// チェックあり　付表6のみ
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表６（２割特例）										   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo6_01) {
							m_pFhyo6_01 = new CShinFhyo6_2wari(this);
							m_pFhyo6_01->InitInfo(&m_pSnFhyo6_01Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith);
							m_pFhyo6_01->Create(IDD_DIALOG_FHY6_2WARI, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表６"), m_pFhyo6_01->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo6_01;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO6_01_TAB;
						shTabmng.pWnd = m_pFhyo6_01;
						m_TabMng.Add(shTabmng);
					}
				}
				else{
					// チェックなし　付表2（インボイス対応）と付表1

					// 経過措置対象の課税資産の譲渡等あり
					if (m_pSnHeadData->SVmzsw == 1) {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表２－０２											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if ( !m_pFhyo2_12Ex ) {
							m_pFhyo2_12Ex = new CShinFhyo2_12Ex ( this );
							m_pFhyo2_12Ex->InitInfo ( &m_pSnFhyo2_02Data, &m_pSnHeadData, &m_pSnTanaData, &m_Arith, m_pShinInfo, &m_pSyzSyukei );
							m_pFhyo2_12Ex->Create ( IDD_DIALOG_FHY2_12EX, &m_MainTab );
						}
						m_MainTab.InsertItem ( tabIdx, _T( "付表２－２" ), m_pFhyo2_12Ex->m_hWnd );
						ptmpTab[tabIdx] = m_pFhyo2_12Ex;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO2_02_TAB;
						shTabmng.pWnd = m_pFhyo2_12Ex;
						m_TabMng.Add ( shTabmng );

						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表２－０１											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if ( !m_pFhyo2_11Ex ) {
							m_pFhyo2_11Ex = new CShinFhyo2_11Ex ( this );
							m_pFhyo2_11Ex->InitInfo ( &m_pSnFhyo2_01Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnTanaData, &m_pSyzSyukei );
							m_pFhyo2_11Ex->Create ( IDD_DIALOG_FHY2_11EX, &m_MainTab );
						}
						m_MainTab.InsertItem ( tabIdx, _T( "付表２－１" ), m_pFhyo2_11Ex->m_hWnd );
						ptmpTab[tabIdx] = m_pFhyo2_11Ex;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO2_01_TAB;
						shTabmng.pWnd = m_pFhyo2_11Ex;
						m_TabMng.Add ( shTabmng );
					}
					else {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表２－０３											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if ( !m_pFhyo2_03Ex ) {
							m_pFhyo2_03Ex = new CShinFhyo2_03Ex ( this );
							m_pFhyo2_03Ex->InitInfo ( &m_pSnFhyo2_01Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnTanaData, &m_pSyzSyukei );
							m_pFhyo2_03Ex->Create ( IDD_DIALOG_FHY2_03EX, &m_MainTab );
						}
						m_MainTab.InsertItem ( tabIdx, _T( "付表２－３" ), m_pFhyo2_03Ex->m_hWnd );
						ptmpTab[tabIdx] = m_pFhyo2_03Ex;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO2_03_TAB;
						shTabmng.pWnd = m_pFhyo2_03Ex;
						m_TabMng.Add ( shTabmng );
					}

					//	// 経過措置対象の課税資産の譲渡等あり
					if (m_pSnHeadData->SVmzsw == 1) {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表１－０２											　 //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo1_02) {
							m_pFhyo1_02 = new CShinFhyo1_02(this);
							m_pFhyo1_02->InitInfo(&m_pSnFhyo1_02Data, &m_pSnHeadData, m_pShinInfo, &m_Arith, &m_pSyzSyukei/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo */);
							m_pFhyo1_02->Create(IDD_DIALOG_FHY1_02, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表１－２"), m_pFhyo1_02->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo1_02;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO1_02_TAB;
						shTabmng.pWnd = m_pFhyo1_02;
						m_TabMng.Add(shTabmng);
					}

					if ((m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR) && (m_pSnHeadData->SVmzsw == 0)) {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表１－０３											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo1_03) {
							m_pFhyo1_03 = new CShinFhyo1_03(this);
							m_pFhyo1_03->InitInfo(&m_pSnFhyo1_01Data, &m_pSnHeadData, m_pShinInfo, &m_Arith, &m_pSyzSyukei);
							m_pFhyo1_03->Create(IDD_DIALOG_FHY1_03, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表１－３"), m_pFhyo1_03->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo1_03;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO1_03_TAB;
						shTabmng.pWnd = m_pFhyo1_03;
						m_TabMng.Add(shTabmng);
					}
					else {
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						//										付表１－０１											   //
						//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
						if (!m_pFhyo1_01) {
							m_pFhyo1_01 = new CShinFhyo1_01(this);
							m_pFhyo1_01->InitInfo(&m_pSnFhyo1_01Data, &m_pSnHeadData, m_pShinInfo, &m_Arith, &m_pSyzSyukei/*, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo*/);
							m_pFhyo1_01->Create(IDD_DIALOG_FHY1_01, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("付表１－１"), m_pFhyo1_01->m_hWnd);
						ptmpTab[tabIdx] = m_pFhyo1_01;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_FHYO1_01_TAB;
						shTabmng.pWnd = m_pFhyo1_01;
						m_TabMng.Add(shTabmng);
					}
				}
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										一般本表１												   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//------->２割特例用の本表を追加('23.05.01 Add)
			if ( eymd >= ICS_SH_INVOICE_DAY ) {
				// 令和5年10月1日以降の課税期間かつ、「２割特例計算を使用する」チェックがON
				if (!m_pShinIppan01EX2) {
					m_pShinIppan01EX2 = new CShinIppan01EX2(this);
					m_pShinIppan01EX2->InitInfo(&m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei);
					m_pShinIppan01EX2->Create(IDD_DIALOG_IPPAN01EX2, &m_MainTab);
				}
				m_MainTab.InsertItem(tabIdx, _T("一般本表"), m_pShinIppan01EX2->m_hWnd);
				ptmpTab[tabIdx] = m_pShinIppan01EX2;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_IPPAN01_TAB;
				shTabmng.pWnd = m_pShinIppan01EX2;
				m_TabMng.Add(shTabmng);
			}
//<-------
			else if(( eymd >= R04REFORM ) && ( m_pSnHeadData->IsSoleProprietor() )) {
				// 令和4年4月1日以降の課税期間かつ、個人業種
				if( !m_pShinIppan01EX ) {
					m_pShinIppan01EX = new CShinIppan01EX( this );
					m_pShinIppan01EX->InitInfo( &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei );
					m_pShinIppan01EX->Create( IDD_DIALOG_IPPAN01EX, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T( "一般本表" ), m_pShinIppan01EX->m_hWnd );
				ptmpTab[tabIdx] = m_pShinIppan01EX;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_IPPAN01_TAB;
				shTabmng.pWnd = m_pShinIppan01EX;
				m_TabMng.Add( shTabmng );
			}
			else {
				if (!m_pShinIppan01) {
					m_pShinIppan01 = new CShinIppan01(this);
					m_pShinIppan01->InitInfo(&m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei);
					m_pShinIppan01->Create(IDD_DIALOG_IPPAN01, &m_MainTab);
				}
				m_MainTab.InsertItem(tabIdx, _T("一般本表"), m_pShinIppan01->m_hWnd);
	//			m_MainTab.InsertItem(tabIdx, _T("一般本表１"), m_pShinIppan01->m_hWnd);
				ptmpTab[tabIdx] = m_pShinIppan01;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_IPPAN01_TAB;
				shTabmng.pWnd = m_pShinIppan01;
				m_TabMng.Add(shTabmng);
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//								課税標準額等の内訳書											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if (!m_pShinIppan02) {
				m_pShinIppan02 = new CShinIppan02(this);
				m_pShinIppan02->InitInfo( &m_pSnHeadData, /*m_pSnChskData,*/ &m_pSnHonpyo2Data, &m_Arith, m_pShinInfo, m_pZmSub, &m_pSyzSyukei );
				m_pShinIppan02->Create(IDD_DIALOG_IPPAN02, &m_MainTab);
			}
			m_MainTab.InsertItem(tabIdx, _T("課税標準額等の内訳書"), m_pShinIppan02->m_hWnd);
//			m_MainTab.InsertItem(tabIdx, _T("一般本表２"), m_pShinIppan02->m_hWnd);
			ptmpTab[tabIdx] = m_pShinIppan02;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_IPPAN02_TAB;
			shTabmng.pWnd = m_pShinIppan02;
			m_TabMng.Add(shTabmng);
		}

//		if ((eymd < ICS_SH_INVOICE_DAY) || ((m_pSnHeadData->m_DispTabSgn & 0x2000) == 0)) {	//２割特例を適用しない場合
		//準確定or準確定の修正・更正
		if (m_pSnHeadData->Sn_Syuusei & 0x10) {
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										付表６													   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if (!m_pSnFhyo6Data) {
				m_pSnFhyo6Data = new CH31SnFhyo6Data();
				if (m_pSnFhyo6Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}

			if (!m_pFhyo6) {
				m_pFhyo6 = new CShinFhyo6(this);
				m_pFhyo6->InitInfo(&m_pSnFhyo6Data, &m_pSnFhyo1_01Data, &m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pZmSub, m_pShinInfo, m_SnSeq);
				//m_pFhyo6->InitInfo( &m_pSnFhyo6Data, &m_pSnFhyo1_01Data, &m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pZmSub, m_pShinInfo );
				//m_pFhyo6->InitInfo(&m_pSnFhyo6Data, &m_pSnFhyo1_01Data, &m_pSnFhyo4_01Data, &m_pSnHeadData, NULL, m_pZmSub, m_pShinInfo);
				m_pFhyo6->Create(IDD_DIALOG_FHY6, &m_MainTab);
			}
			//----->インボイス施工日以降は付表７に名称変更('23.07.10 cor)
			if ( eymd >= ICS_SH_INVOICE_DAY ) {
				m_MainTab.InsertItem( tabIdx, _T("付表７"), m_pFhyo6->m_hWnd );
			}
			else {
				m_MainTab.InsertItem( tabIdx, _T("付表６"), m_pFhyo6->m_hWnd );
			}
			//<-----
			ptmpTab[tabIdx] = m_pFhyo6;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO6_TAB;
			shTabmng.pWnd = m_pFhyo6;
			m_TabMng.Add(shTabmng);
		}

		// 還付
//		if ((eymd < ICS_SH_INVOICE_DAY) || ((m_pSnHeadData->m_DispTabSgn & 0x2000) == 0)) {	//２割特例を適用しない場合
		if ( (m_pSnHeadData->m_DispTabSgn & 0x2000) == 0 ){	// 20230704
			if (m_pSnHeadData->IsUseTaxRefund()) {
				if (!m_pSnRefundData) {
					m_pSnRefundData = new CH31SnRefundData();
					if (m_pSnRefundData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq)) {
						m_pSnRefundData->DeleteAllData(m_pZmSub, -1);
						return -1;
					}
				}

				isRefundDialog = true;

				// 情報設定
				ResetRefundData();

				// 個人
				if (m_pSnHeadData->IsSoleProprietor()) {
					if (!m_pRefundKjn01) {
						m_pRefundKjn01 = new CShinRefundKjn01(this);
						m_pRefundKjn01->Create(IDD_SYZREFUND_KJ1, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("還付付表1(個人)"), m_pRefundKjn01->m_hWnd);
					ptmpTab[tabIdx] = m_pRefundKjn01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_REFUND_KJ_01_TAB;
					shTabmng.pWnd = m_pRefundKjn01;
					m_TabMng.Add(shTabmng);

					//----> 20230704
					if( eymd >= ICS_SH_INVOICE_DAY ){
						if (!m_pRefundKjn02EX) {
							m_pRefundKjn02EX = new CShinRefundKjn02EX(this);
							m_pRefundKjn02EX->Create(IDD_SYZREFUND_KJ2_EX, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("還付付表2(個人)"), m_pRefundKjn02EX->m_hWnd);
						ptmpTab[tabIdx] = m_pRefundKjn02EX;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_REFUND_KJ_02_TAB;
						shTabmng.pWnd = m_pRefundKjn02EX;
						m_TabMng.Add(shTabmng);						
					}
					//<---- 20230704
					else{
						if (!m_pRefundKjn02) {
							m_pRefundKjn02 = new CShinRefundKjn02(this);
							m_pRefundKjn02->Create(IDD_SYZREFUND_KJ2, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("還付付表2(個人)"), m_pRefundKjn02->m_hWnd);
						ptmpTab[tabIdx] = m_pRefundKjn02;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_REFUND_KJ_02_TAB;
						shTabmng.pWnd = m_pRefundKjn02;
						m_TabMng.Add(shTabmng);
					}
				}
				// 法人
				else {
					if (!m_pRefundHjn01) {
						m_pRefundHjn01 = new CShinRefundHjn01(this);
						m_pRefundHjn01->Create(IDD_SYZREFUND_HJ1, &m_MainTab);
					}
					m_MainTab.InsertItem(tabIdx, _T("還付付表1(法人)"), m_pRefundHjn01->m_hWnd);
					ptmpTab[tabIdx] = m_pRefundHjn01;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_REFUND_HJ_01_TAB;
					shTabmng.pWnd = m_pRefundHjn01;
					m_TabMng.Add(shTabmng);

					//----> 20230704
					if( eymd >= ICS_SH_INVOICE_DAY ){
						if (!m_pRefundHjn02EX) {
							m_pRefundHjn02EX = new CShinRefundHjn02EX(this);
							m_pRefundHjn02EX->Create(IDD_SYZREFUND_HJ2_EX, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("還付付表2(法人)"), m_pRefundHjn02EX->m_hWnd);
						ptmpTab[tabIdx] = m_pRefundHjn02EX;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_REFUND_HJ_02_TAB;
						shTabmng.pWnd = m_pRefundHjn02EX;
						m_TabMng.Add(shTabmng);
					}
					//<---- 20230704
					else{
						if (!m_pRefundHjn02) {
							m_pRefundHjn02 = new CShinRefundHjn02(this);
							m_pRefundHjn02->Create(IDD_SYZREFUND_HJ2, &m_MainTab);
						}
						m_MainTab.InsertItem(tabIdx, _T("還付付表2(法人)"), m_pRefundHjn02->m_hWnd);
						ptmpTab[tabIdx] = m_pRefundHjn02;
						tabIdx++;
						shTabmng.tabtype = ID_ICSSH_REFUND_HJ_02_TAB;
						shTabmng.pWnd = m_pRefundHjn02;
						m_TabMng.Add(shTabmng);
					}
				}
			}
		}

		// 更正の請求
		if (m_pSnHeadData->Sn_Syuusei & 0x02) {
			if (!m_pSnKskData) {
				m_pSnKskData = new CH31SnKskData();
				if (m_pSnKskData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq)) {
					return -1;
				}
			}

			CString	ttlStr;
			// 個人
			if (m_pSnHeadData->IsSoleProprietor()) {
				if (bKskModify != FALSE) {	// 更正の請求/新様式対応
					if (!m_pKjnKskEX2) {
						m_pKjnKskEX2 = new CShinKjnKskEX2(this);
						m_pKjnKskEX2->InitInfo(&m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData);
						m_pKjnKskEX2->Create(IDD_DIALOG_KJINKSK_EX2, &m_MainTab);
					}
					if (m_pSnHeadData->Sn_Syuusei & 0x04) {
						ttlStr = _T("更正の申出(個人)");
					}
					else {
						ttlStr = _T("更正の請求(個人)");
					}
					m_MainTab.InsertItem(tabIdx, ttlStr, m_pKjnKskEX2->m_hWnd);
					ptmpTab[tabIdx] = m_pKjnKskEX2;
					firstPage = tabIdx;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_KSK_KJ_TAB;
					shTabmng.pWnd = m_pKjnKskEX2;
				}
				else {
					if (!m_pKjnKskEX) {
						m_pKjnKskEX = new CShinKjnKskEX(this);
						m_pKjnKskEX->InitInfo(&m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData);
						m_pKjnKskEX->Create(IDD_DIALOG_KJINKSK_EX, &m_MainTab);
					}
					if (m_pSnHeadData->Sn_Syuusei & 0x04) {
						ttlStr = _T("更正の申出(個人)");
					}
					else {
						ttlStr = _T("更正の請求(個人)");
					}
					m_MainTab.InsertItem(tabIdx, ttlStr, m_pKjnKskEX->m_hWnd);
					ptmpTab[tabIdx] = m_pKjnKskEX;
					firstPage = tabIdx;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_KSK_KJ_TAB;
					shTabmng.pWnd = m_pKjnKskEX;
				}
				m_TabMng.Add(shTabmng);
			}
			// 法人
			else {
				if (bKskModify != FALSE) {	// 更正の請求/新様式対応
					if (!m_pHjnKskEX2) {
						m_pHjnKskEX2 = new CShinHjnKskEX2(this);
						m_pHjnKskEX2->InitInfo(&m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData);
						m_pHjnKskEX2->Create(IDD_DIALOG_HJINKSK_EX2, &m_MainTab);
					}
					if (m_pSnHeadData->Sn_Syuusei & 0x04) {
						ttlStr = _T("更正の申出(法人)");
					}
					else {
						ttlStr = _T("更正の請求(法人)");
					}
					m_MainTab.InsertItem(tabIdx, ttlStr, m_pHjnKskEX2->m_hWnd);
					ptmpTab[tabIdx] = m_pHjnKskEX2;
					firstPage = tabIdx;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_KSK_HJ_TAB;
					shTabmng.pWnd = m_pHjnKskEX2;
				}
				else {
					if (!m_pHjnKskEX) {
						m_pHjnKskEX = new CShinHjnKskEX(this);
						m_pHjnKskEX->InitInfo(&m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData);
						m_pHjnKskEX->Create(IDD_DIALOG_HJINKSK_EX, &m_MainTab);
					}
					if (m_pSnHeadData->Sn_Syuusei & 0x04) {
						ttlStr = _T("更正の申出(法人)");
					}
					else {
						ttlStr = _T("更正の請求(法人)");
					}
					m_MainTab.InsertItem(tabIdx, ttlStr, m_pHjnKskEX->m_hWnd);
					ptmpTab[tabIdx] = m_pHjnKskEX;
					firstPage = tabIdx;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_KSK_HJ_TAB;
					shTabmng.pWnd = m_pHjnKskEX;
				}
				m_TabMng.Add(shTabmng);
			}
		}
	}

	// 各表の入力ロックを描画
	for (int tab_cnt = 0; tab_cnt < tabIdx; tab_cnt++) {
		((CSyzBaseDlg *)ptmpTab[tab_cnt])->Static_Update(m_pSnHeadData->Sn_Sign4);
	}

	// 先頭ページ保存
	m_pFirstPage = ptmpTab[0];	

	RECT rect = {0};
	ptmpTab[0]->GetClientRect( &rect );
	for( int i=0; i<tabIdx; i++ ){
		ptmpTab[i]->MoveWindow( &rect );
	}
	RECT rect1, rect2;
	CWnd *pWnd = (CWnd *)&m_MainTab;
	pWnd->GetClientRect( &rect1 );				// ｸﾗｲｱﾝﾄ領域の取得
	m_MainTab.AdjustRect( FALSE, &rect1 );		// ﾀﾌﾞｺﾝﾄﾛｰﾙの表示領域の計算
	rect2.left   = rect1.left;
	rect2.top    = rect1.top;
	rect2.bottom = rect1.bottom;
	rect2.right  = rect1.right;
	for( int i=0; i<tabIdx; i++ ){
		if( ptmpTab[i] ){
			ptmpTab[i]->MoveWindow( &rect2 );	// ダイアログの移動
		}
	}

	// 初期ページ設定
	m_MainTab.SetCurSel( firstPage );
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );

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

	//tabRect.bottom += vwRect.bottom - tabRect.bottom;
	m_MainTab.MoveWindow( tabRect );

	if( isRefundDialog ) {
		// ZmSelダイアログの作成
		if( m_pRefundHjn02EX ) {
			m_pRefundHjn02EX->InitZmSel();
		}

		if( m_pRefundKjn02EX ) {
			m_pRefundKjn02EX->InitZmSel();
		}
	}

	m_DispTabSgn = m_pSnHeadData->m_DispTabSgn;	// タブ生成時の表示サインを保存（基本情報変更時の判定に使用するため）

	return 0;
}


//-----------------------------------------------------------------------------
// タブ内のダイアログを全消去
//-----------------------------------------------------------------------------
void CH31HyoView::DeleteInsideDlg()
{
	// ビュー内部からフォーカスを外す
	m_MainTab.SetFocus();

	if( m_pFhyo1_01 ){
		m_pFhyo1_01->DestroyWindow();
		delete m_pFhyo1_01;
		m_pFhyo1_01 = NULL;	
	}
	if( m_pFhyo1_02 ){
		m_pFhyo1_02->DestroyWindow();
		delete m_pFhyo1_02;
		m_pFhyo1_02 = NULL;
	}
	if ( m_pFhyo1_03 ) {
		m_pFhyo1_03->DestroyWindow();
		delete m_pFhyo1_03;
		m_pFhyo1_03 = NULL;	
	}
	if( m_pFhyo2_01 ){
		m_pFhyo2_01->DestroyWindow();
		delete m_pFhyo2_01;
		m_pFhyo2_01 = NULL;
	}
	if ( m_pFhyo2_11 ) {
		m_pFhyo2_11->DestroyWindow();
		delete m_pFhyo2_11;
		m_pFhyo2_11 = NULL;
	}
	//---->20230120
	if ( m_pFhyo2_11Ex ) {
		m_pFhyo2_11Ex->DestroyWindow();
		delete m_pFhyo2_11Ex;
		m_pFhyo2_11Ex = NULL;
	}
	//<----20230120
	if( m_pFhyo2_02 ){
		m_pFhyo2_02->DestroyWindow();
		delete m_pFhyo2_02;
		m_pFhyo2_02 = NULL;
	}
	if ( m_pFhyo2_12 ) {
		m_pFhyo2_12->DestroyWindow();
		delete m_pFhyo2_12;
		m_pFhyo2_12 = NULL;
	}
	//---->20230120
	if ( m_pFhyo2_12Ex ) {
		m_pFhyo2_12Ex->DestroyWindow();
		delete m_pFhyo2_12Ex;
		m_pFhyo2_12Ex = NULL;
	}
	//<----20230120
	if ( m_pFhyo2_03 ) {
		m_pFhyo2_03->DestroyWindow();
		delete m_pFhyo2_03;
		m_pFhyo2_03 = NULL;
	}
	//---->20230120
	if ( m_pFhyo2_03Ex ) {
		m_pFhyo2_03Ex->DestroyWindow();
		delete m_pFhyo2_03Ex;
		m_pFhyo2_03Ex = NULL;
	}
	//<----20230120
	if( m_pFhyo4_01 ){
		m_pFhyo4_01->DestroyWindow();
		delete m_pFhyo4_01;
		m_pFhyo4_01 = NULL;
	}
	if( m_pFhyo4_02 ){
		m_pFhyo4_02->DestroyWindow();
		delete m_pFhyo4_02;
		m_pFhyo4_02 = NULL;
	}
	if ( m_pFhyo4_03 ) {
		m_pFhyo4_03->DestroyWindow();
		delete m_pFhyo4_03;
		m_pFhyo4_03 = NULL;
	}
	if( m_pFhyo5_01 ){
		m_pFhyo5_01->DestroyWindow();
		delete m_pFhyo5_01;
		m_pFhyo5_01 = NULL;
	}
	if( m_pFhyo5_012 ){
		m_pFhyo5_012->DestroyWindow();
		delete m_pFhyo5_012;
		m_pFhyo5_012 = NULL;
	}
	if( m_pFhyo5_02 ){
		m_pFhyo5_02->DestroyWindow();
		delete m_pFhyo5_02;
		m_pFhyo5_02 = NULL;
	}
	if( m_pFhyo5_022 ){
		m_pFhyo5_022->DestroyWindow();
		delete m_pFhyo5_022;
		m_pFhyo5_022 = NULL;
	}
	if ( m_pFhyo5_03 ){
		m_pFhyo5_03->DestroyWindow();
		delete m_pFhyo5_03;
		m_pFhyo5_03 = NULL;
	}
	if ( m_pFhyo5_032 ) {
		m_pFhyo5_032->DestroyWindow();
		delete m_pFhyo5_032;
		m_pFhyo5_032 = NULL;
	}
	if( m_pFhyo6 ){
		m_pFhyo6->DestroyWindow();
		delete m_pFhyo6;
		m_pFhyo6 = NULL;
	}
//----->('23.05.01 Add)
	if (m_pFhyo6_01) {
		m_pFhyo6_01->DestroyWindow();
		delete m_pFhyo6_01;
		m_pFhyo6_01 = NULL;
	}
//<-----
	if( m_pShinIppan01 ){
		m_pShinIppan01->DestroyWindow();
		delete m_pShinIppan01;
		m_pShinIppan01 = NULL;
	}
	if( m_pShinIppan01EX ){
		m_pShinIppan01EX->DestroyWindow();
		delete m_pShinIppan01EX;
		m_pShinIppan01EX = NULL;
	}
//----->('23.05.01 Add)
	if (m_pShinIppan01EX2) {
		m_pShinIppan01EX2->DestroyWindow();
		delete m_pShinIppan01EX2;
		m_pShinIppan01EX2 = NULL;
	}
//<-----
	if( m_pShinIppan02 ){
		m_pShinIppan02->DestroyWindow();
		delete m_pShinIppan02;
		m_pShinIppan02 = NULL;
	}
	if( m_pShinKani01 ){
		m_pShinKani01->DestroyWindow();
		delete m_pShinKani01;
		m_pShinKani01 = NULL;
	}
	if( m_pShinKani01EX ){
		m_pShinKani01EX->DestroyWindow();
		delete m_pShinKani01EX;
		m_pShinKani01EX = NULL;
	}
//----->('23.05.01 Add)
	if (m_pShinKani01EX2) {
		m_pShinKani01EX2->DestroyWindow();
		delete m_pShinKani01EX2;
		m_pShinKani01EX2 = NULL;
	}
//<-----
	if (m_pRefundKjn01) {
		m_pRefundKjn01->DestroyWindow();
		delete m_pRefundKjn01;
		m_pRefundKjn01 = NULL;
	}
	if (m_pRefundKjn02) {
		m_pRefundKjn02->DestroyWindow();
		delete m_pRefundKjn02;
		m_pRefundKjn02 = NULL;
	}
	//----> 20230704
	if (m_pRefundKjn02EX) {
		m_pRefundKjn02EX->DestroyWindow();
		delete m_pRefundKjn02EX;
		m_pRefundKjn02EX = NULL;
	}
	//<---- 20230704
	if (m_pRefundHjn01) {
		m_pRefundHjn01->DestroyWindow();
		delete m_pRefundHjn01;
		m_pRefundHjn01 = NULL;
	}
	if (m_pRefundHjn02) {
		m_pRefundHjn02->DestroyWindow();
		delete m_pRefundHjn02;
		m_pRefundHjn02 = NULL;
	}
	//----> 20230704
	if (m_pRefundHjn02EX) {
		m_pRefundHjn02EX->DestroyWindow();
		delete m_pRefundHjn02EX;
		m_pRefundHjn02EX = NULL;
	}
	//<---- 20230704
	if (m_pChskEX) {
		m_pChskEX->DestroyWindow();
		delete m_pChskEX;
		m_pChskEX = NULL;
	}
	if (m_pHjnKskEX) {
		m_pHjnKskEX->DestroyWindow();
		delete m_pHjnKskEX;
		m_pHjnKskEX = NULL;
	}
	if (m_pKjnKskEX) {
		m_pKjnKskEX->DestroyWindow();
		delete m_pKjnKskEX;
		m_pKjnKskEX = NULL;
	}
	// 更正の請求/新様式対応
	if (m_pHjnKskEX2) {
		m_pHjnKskEX2->DestroyWindow();
		delete m_pHjnKskEX2;
		m_pHjnKskEX2 = NULL;
	}
	if (m_pKjnKskEX2) {
		m_pKjnKskEX2->DestroyWindow();
		delete m_pKjnKskEX2;
		m_pKjnKskEX2 = NULL;
	}

	
	if (m_pKsisanuri) {
		m_pKsisanuri->DestroyWindow();
		delete m_pKsisanuri;
		m_pKsisanuri = NULL;
	}
	if (m_pKsisanuri2) {
		m_pKsisanuri2->DestroyWindow();
		delete m_pKsisanuri2;
		m_pKsisanuri2 = NULL;
	}
	if (m_pKshiire) {
		m_pKshiire->DestroyWindow();
		delete m_pKshiire;
		m_pKshiire = NULL;
	}													
	
	if (m_pSnHonpyoData) {
		delete m_pSnHonpyoData;
		m_pSnHonpyoData = NULL;
	}
	if (m_pSnHonpyo2Data) {
		delete m_pSnHonpyo2Data;
		m_pSnHonpyo2Data = NULL;
	}
	if (m_pSnFhyo1_01Data) {
		delete m_pSnFhyo1_01Data;
		m_pSnFhyo1_01Data = NULL;
	}
	if (m_pSnFhyo1_02Data) {
		delete m_pSnFhyo1_02Data;
		m_pSnFhyo1_02Data = NULL;
	}
	if (m_pSnFhyo2_01Data) {
		delete m_pSnFhyo2_01Data;
		m_pSnFhyo2_01Data = NULL;
	}
	if (m_pSnFhyo2_02Data) {
		delete m_pSnFhyo2_02Data;
		m_pSnFhyo2_02Data = NULL;
	}
	if (m_pSnTanaData) {
		delete m_pSnTanaData;
		m_pSnTanaData = NULL;
	}
	if (m_pSnFhyo4_01Data) {
		delete m_pSnFhyo4_01Data;
		m_pSnFhyo4_01Data = NULL;
	}
	if (m_pSnFhyo4_02Data) {
		delete m_pSnFhyo4_02Data;
		m_pSnFhyo4_02Data = NULL;
	}
	if (m_pSnFhyo5_01Data) {
		delete m_pSnFhyo5_01Data;
		m_pSnFhyo5_01Data = NULL;
	}
	if (m_pSnFhyo5_012Data) {
		delete m_pSnFhyo5_012Data;
		m_pSnFhyo5_012Data = NULL;
	}
	if (m_pSnFhyo5_02Data) {
		delete m_pSnFhyo5_02Data;
		m_pSnFhyo5_02Data = NULL;
	}
	if (m_pSnFhyo5_022Data) {
		delete m_pSnFhyo5_022Data;
		m_pSnFhyo5_022Data = NULL;
	}
//----->('3.05.01 Add)
	if (m_pSnFhyo6_01Data) {
		delete m_pSnFhyo6_01Data;
		m_pSnFhyo6_01Data = NULL;
	}
//<-----
	//if (m_pSnFhyo6Data) {
	//	delete m_pSnFhyo6Data;
	//	m_pSnFhyo6Data = NULL;
	//}
	//if (m_pSnRefundData) {
	//	delete m_pSnRefundData;
	//	m_pSnRefundData = NULL;
	//}
	if (m_pSnChskData) {
		delete m_pSnChskData;
		m_pSnChskData = NULL;
	}
	//if (m_pSnKskData) {
	//	delete m_pSnKskData;
	//	m_pSnKskData = NULL;
	//}

	m_TabMng.RemoveAll();
	m_pFirstPage = NULL;
}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
// 引数		wflg	：	書き込みあり
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::EndJob( int wflg )
{
	//== 最終データ書込み ==
	if (wflg) {
		if (WriteData(TRUE, FALSE)) {
			return -1;
		}
	}

	//== データ ==
	if( m_pSnHonpyoData ){
		delete m_pSnHonpyoData;
		m_pSnHonpyoData = NULL;
	}

	// 還付申告データ
	if ( m_pSnRefundData ) {
		delete m_pSnRefundData;
		m_pSnRefundData = NULL;
	}

	//== 画面 ==
	DeleteInsideDlg();

	return 0;
}

void CH31HyoView::OnButtonF2()
{
	int selno = m_MainTab.GetCurSel();
	if ( selno < ( int )m_TabMng.GetCount() ) {
		if ( m_TabMng[selno].pWnd ) {
			m_TabMng[selno].pWnd->PostMessage ( WM_KEYDOWN, VK_F2 );
		}
	}
}
void CH31HyoView::OnButtonF4()
{
	if( m_pShinInfo && m_pShinInfo->pBaseView ){

		if (Message_MyNumber_error() == -1) {
			return;
		}
		if (Message_MyNumber_overlap() == -1) {
			return;
		}
		EditOff_Fuhyo6();
		if (Message_SozokuWariai() == -1) {
			return;
		}

		CheckInvno( 2 );

		// データ更新
		UpdateSheet(m_MainTab.GetCurSel());

		int st = IsAbleTkKz();
		if (st == -1) {
			// 選択を変更する
			ChangeTkkzUse(1, 2);
			return;
		}
		else if (st == -2) {
			if (ChkZkTkkzReturn(1, 2) == 0) {
				return;
			}
		}

		m_pShinInfo->pBaseView->PostMessage( WM_COMMAND, ID_BUTTON_F4 );
	}
}
void CH31HyoView::OnButtonF5()
{
	if (m_pShinInfo && m_pShinInfo->pBaseView) {

		if (Message_MyNumber_error() == -1) {
			return;
		}
		if (Message_MyNumber_overlap() == -1) {
			return;
		}EditOff_Fuhyo6();
		if (Message_SozokuWariai() == -1) {
			return;
		}

		CheckInvno( 2 );

		// データ更新
		UpdateSheet(m_MainTab.GetCurSel());

		if ((m_pVprn == NULL) ||
			((m_pVprn && (m_pVprn->IsPrintFromPreview() == FALSE)) && (m_pVprn && (m_pVprn->PreviewOnPrintdlg() == 0)))) {
			int st = IsAbleTkKz();
			if (st == -1) {
				// 選択を変更する
				ChangeTkkzUse(1, 1);
				return;
			}
			else if (st == -2) {
				if (ChkZkTkkzReturn(1, 1) == 0) {
					return;
				}
			}
		}

		m_pShinInfo->pBaseView->PostMessage(WM_COMMAND, ID_BUTTON_F5);
	}
}
void CH31HyoView::OnButtonF6()
{
	int selno = m_MainTab.GetCurSel();

	if ( m_pSnHeadData->IsMiddleProvisional() == FALSE ) {
		if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) {
			if ( m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB ) {
				if ( m_pShinKani01 ) {
					m_pShinKani01->PrePrint();
				}
				if ( m_pShinKani01EX ) {
					m_pShinKani01EX->PrePrint();
				}
				//----->('23.05.01 Add)
				if ( m_pShinKani01EX2 ) {
					m_pShinKani01EX2->PrePrint();
				}
				//<-----
			}
		}
		else {
			if ( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB ) {
				if ( m_pShinIppan01 ) {
					m_pShinIppan01->PrePrint();
				}
				if ( m_pShinIppan01EX ) {
					m_pShinIppan01EX->PrePrint();
				}
				//----->('23.05.01 Add)
				if (m_pShinIppan01EX2) {
					m_pShinIppan01EX2->PrePrint();
				}
				//<-----
			}
		}
	}
}
void CH31HyoView::OnButtonF7()
{
	int selno = m_MainTab.GetCurSel();
	if ( selno < (int)m_TabMng.GetCount() ) {
		if ( m_TabMng[selno].tabtype == ID_ICSSH_REFUND_HJ_01_TAB ) {		// 還付　法人
			if ( m_pRefundHjn01 ) {
				m_pRefundHjn01->VolRecClear();
				if ( m_pRefundHjn02 ) {
					m_pRefundHjn02->SetDispPosition( 2 );
				}
				//----> 20230704
				if ( m_pRefundHjn02EX ) {
					m_pRefundHjn02EX->SetDispPosition( 2 );
				}
				//<---- 20230704
				m_pRefundHjn01->SetDispPosition( 0 );
			}
		}
		//else if (m_TabMng[selno].tabtype == ID_ICSSH_REFUND_KJ_02_TAB) {	// 還付　個人
		else if (m_TabMng[selno].tabtype == ID_ICSSH_REFUND_KJ_01_TAB) {	// 還付　個人
			if ( m_pRefundKjn01 ) {
				m_pRefundKjn01->VolRecClear();
				if ( m_pRefundKjn02 ) {
					m_pRefundKjn02->SetDispPosition( 3 );
				}
				//----> 20230704
				if ( m_pRefundKjn02EX ) {
					m_pRefundKjn02EX->SetDispPosition( 3 );
				}
				//<---- 20230704
				m_pRefundKjn01->SetDispPosition( 1 );
			}
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB) {			// 更正の請求　法人
			if ( m_pHjnKskEX ) {
				m_pHjnKskEX->ClearKskHjn();
				m_pHjnKskEX->SetItemPosition( 0 );
			}
			else if ( m_pHjnKskEX2 ) {	// 更正の請求/新様式対応
				m_pHjnKskEX2->ClearKskHjn();
				m_pHjnKskEX2->SetItemPosition( 0 );
			}
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB) {			// 更正の請求　個人
			if ( m_pKjnKskEX ) {
				m_pKjnKskEX->ClearKskKjn();
				m_pKjnKskEX->SetItemPosition( 0 );
			}
			else if ( m_pKjnKskEX2 ) {	// 更正の請求/新様式対応
				m_pKjnKskEX2->ClearKskKjn();
				m_pKjnKskEX2->SetItemPosition( 0 );
			}
		}
	}
}
void CH31HyoView::OnButtonF8()
{
	int selno = m_MainTab.GetCurSel();
	if (selno < (int)m_TabMng.GetCount()) {
		if ((m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB)) {

			// 更正の請求連動ダイアログ
			if( m_pHjnKskEX || m_pKjnKskEX ){
				CShinKskRnd	SKRDialog;
				SKRDialog.InitInfo(m_pSnKskData);
				SKRDialog.m_Rsw = 1;
				SKRDialog.m_Ksw = 0;

				HINSTANCE svhInstResource = AfxGetResourceHandle();
				AfxSetResourceHandle(g_hInstance);

				int rt = (int)SKRDialog.DoModal();

				AfxSetResourceHandle(svhInstResource);

				if (rt != IDOK) {
					return;
				}

				int	Sign = SKRDialog.m_Rsw;
				int Bank = SKRDialog.m_Ksw;

				// 申告金額の連動
				CalqSyzForKsk(Sign);
				
				// 還付先金融機関等情報の連動
				if (Bank) {
					if (m_pSnKskData) {
						// 関数の中で公金受取サインを更新しているため、元に戻す
						int sv_GenericSgn = m_pSnHeadData->Sn_GenericSgn;
						m_pSnKskData->SetReaskFinancial(m_pSnHeadData);
						m_pSnHeadData->Sn_GenericSgn = sv_GenericSgn;
					}
				}
			}
			else if( m_pHjnKskEX2 || m_pKjnKskEX2 ){	// 更正の請求/新様式対応
				CShinKskRnd2	SKRDialog2;
				SKRDialog2.InitInfo(m_pSnKskData, m_pSnHeadData);
				SKRDialog2.m_Rsw = 1;
				SKRDialog2.m_Ksw = 0;

				HINSTANCE svhInstResource = AfxGetResourceHandle();
				AfxSetResourceHandle(g_hInstance);

				int rt = (int)SKRDialog2.DoModal();

				AfxSetResourceHandle(svhInstResource);

				// 金融機関のどこかにフォーカスが入っているとそこだけデータが入らない場合があるため、
				// フォーカスが入っている場合は「公金受取口座を利用する」に移動させておく。
				if( m_pKjnKskEX2 ){
					m_pKjnKskEX2->SetFocusKoukinUketoriChk();
				}

				if (rt != IDOK) {
					return;
				}

				int	Sign = SKRDialog2.m_Rsw;
				int Bank = SKRDialog2.m_Ksw;

				// 申告金額の連動
				if( m_pSnHeadData->Sn_Sign4 & 0x80 ){	// 金額の転記は非連動のときのみ
					CalqSyzForKsk(Sign);
				}
				// 還付先金融機関等情報の連動
				if (Bank) {
					if (m_pSnKskData) {
						m_pSnKskData->SetReaskFinancial(m_pSnHeadData);
					}
				}
			}

			// 再表示
			if ((m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB) /*&& m_pHjnKskEX*/) {
				if ( m_pHjnKskEX ) {
					m_pHjnKskEX->DispKsiHjn(1);
					m_pHjnKskEX->SetItemPosition ( 0 );
				}
				else if ( m_pHjnKskEX2 ) {
					m_pHjnKskEX2->DispKsiHjn(1);	// 更正の請求/新様式対応
					m_pHjnKskEX2->SetItemPosition ( 0 );
				}
			}
			else if ((m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB) /*&& m_pKjnKskEX*/) {
				if( m_pKjnKskEX ){
					m_pKjnKskEX->DispKsiKjn(1);
					m_pKjnKskEX->SetItemPosition ( 0 );
				}
				else if( m_pKjnKskEX2 ){
					m_pKjnKskEX2->DispKsiKjn(1);	// 更正の請求/新様式対応
					m_pKjnKskEX2->SetItemPosition ( 0 );
				}
			}
		}
	}
}
void CH31HyoView::OnButtonF9()
{
	if (Message_MyNumber_error() == -1) {
		return;
	}
	if (Message_MyNumber_overlap() == -1) {
		return;
	}
	EditOff_Fuhyo6();
	if (Message_SozokuWariai() == -1) {
		return;
	}

	CheckInvno( 2 );

	InputRock(0);
}
void CH31HyoView::OnButtonF11()
{
	int tabno = 0;
	for ( tabno = 0; tabno < m_TabMng.GetCount(); tabno++ ) {
		if ( m_TabMng[tabno].tabtype == ID_ICSSH_FHYO6_TAB ) {
			break;
		}
	}

	if ( tabno < m_TabMng.GetCount() ) {
		if ( Message_MyNumber_error() == -1 ) {
			m_MainTab.SetCurSel ( tabno );
			return;
		}

		if ( Message_MyNumber_overlap() == -1 ) {
			m_MainTab.SetCurSel ( tabno );
			return;
		}

		EditOff_Fuhyo6();
		if ( Message_SozokuWariai() == -1 ) {
			m_MainTab.SetCurSel ( tabno );
			return;
		}
	}

	CheckInvno( 2 );

	UpdateSheetFromOuter();
	int selno = m_MainTab.GetCurSel();
	if ( m_pShinInfo && m_pShinInfo->pParent ) {
		// 再集計
		m_pShinInfo->pParent->SendMessage ( WM_USER_RECALQ, ( WPARAM )1 );
	}

	this->LockWindowUpdate();
	ReInitDisp ( TRUE, m_SixKindFlg, m_ShRevType );
	this->UnlockWindowUpdate();

	m_MainTab.SetCurSel ( selno );
	m_MainTab.ModifyStyle ( TCS_SINGLELINE, TCS_MULTILINE );
	SetItemPosition ( selno );
}
void CH31HyoView::OnButtonF12()
{
	CheckInvno( 2 );

	MoveKanpuFocus();
	PageChange(1);	// 1:次へ
	return;
}
void CH31HyoView::OnUpdateButtonF4(CCmdUI *pCmdUI)
{
	int	Flag = 1;
	if (m_pShinInfo->isDotPrn) {
		Flag = 0;
	}
	pCmdUI->Enable(Flag ? TRUE : FALSE);
}
void CH31HyoView::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	int	Flag = 1;
	if (m_pShinInfo->isDotPrn) {
		Flag = 0;
	}
	pCmdUI->Enable(Flag ? TRUE : FALSE);
}
void CH31HyoView::OnUpdateButtonF6(CCmdUI *pCmdUI)
{
	if (pCmdUI) {
		BOOL	bOn = FALSE;
		if (m_pSnHeadData && !(m_pSnHeadData->Sn_Sign4 & 0x01)) {
			int selno = m_MainTab.GetCurSel();
			if ((selno != -1) && (selno < (int)m_TabMng.GetCount())) {
				if ((m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB)) {
					if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
						bOn = TRUE;
					}

				}
			}
		}
		pCmdUI->Enable(bOn);
	}
}
void CH31HyoView::OnUpdateButtonF7(CCmdUI *pCmdUI)
{
	BOOL	bOn = FALSE;

	int selno = m_MainTab.GetCurSel();

	if (!(m_pSnHeadData && m_pSnHeadData->Sn_Sign4 & 0x01)) {
		if ((selno != -1) && (selno < (int)m_TabMng.GetCount())) {
			if ((m_TabMng[selno].tabtype == ID_ICSSH_REFUND_KJ_01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_REFUND_HJ_01_TAB) ||
				(m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB)) {

				bOn = TRUE;
			}
			// 還付、更生の請求を TRUE にする
		}
	}
	pCmdUI->Enable(bOn);
}
void CH31HyoView::OnUpdateButtonF8(CCmdUI *pCmdUI)
{
	BOOL	bOn = FALSE;

	if (!(m_pSnHeadData->Sn_Sign4 & 0x01)) {
		int selno = m_MainTab.GetCurSel();
		if ((selno != -1) && (selno < (int)m_TabMng.GetCount())) {
			if ((m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB)) {
				bOn = TRUE;
			}
		}
	}
	pCmdUI->Enable(bOn);
}
void CH31HyoView::OnUpdateButtonF9(CCmdUI *pCmdUI)
{
	if (m_pSnHeadData == NULL) {
		return;
	}
	else if ((m_pShinInfo == NULL) || (m_pShinInfo->pParent == NULL)) {
		return;
	}

	ICSToolBar* pBar = ((ICSMainFrame *)m_pShinInfo->pParent)->GetToolBar(1);
	if (pBar) {
		char*		p = NULL;
		char*		pDescription = NULL;
		if (m_pSnHeadData->Sn_Sign4 & 0x01) {
			p = _T("F9 ロック解除");
			pDescription = _T("入力ロックを解除します");
		}
		else {
			p = _T("F9 入力ロック");
			pDescription = _T("入力をロックします");
		}
		pBar->SetButtonTooltip(7, p);
		pBar->SetButtonText(7, p);
		pBar->SetButtonDescription(7, pDescription);
		return;
	}
}
void CH31HyoView::OnUpdateButtonF11(CCmdUI *pCmdUI)
{
	BOOL	bON = FALSE;
	if (pCmdUI) {
		if (m_pSnHeadData) {
			if ((m_pSnHeadData->Sn_Sign4 & 0x81) == 0) {
				bON = TRUE;
			}
			if (m_pSnHeadData->IsMiddleProvisional()) {
				bON = FALSE;
			}
			if (bON) {
				if (m_SnSeq) {
					bON = FALSE;
				}
			}
		}
		pCmdUI->Enable(bON);
	}
}
void CH31HyoView::OnUpdateButtonF12(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if (m_MainTab.GetItemCount() == 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CH31HyoView::OnSize(UINT nType, int cx, int cy)
{

	this->LockWindowUpdate();

	ICSFormView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_TabMng.GetCount() && m_pFirstPage ){

		int max = (int)m_TabMng.GetCount();

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


void CH31HyoView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		switch( m_TabMng[selno].tabtype ){
			case ID_ICSSH_IPPAN01_TAB:
				if(m_pShinIppan01){
					m_pShinIppan01->MoveDiagMNEdit();
				}
				if( m_pShinIppan01EX ) {
					m_pShinIppan01EX->MoveDiagMNEdit();
				}
				//----->('23.05.01 Add)
				if (m_pShinIppan01EX2) {
					m_pShinIppan01EX2->MoveDiagMNEdit();
				}
				//<-----
				break;
			case ID_ICSSH_KANI01_TAB:
				if(m_pShinKani01){
					m_pShinKani01->MoveDiagMNEdit();
				}
				if( m_pShinKani01EX ){
					m_pShinKani01EX->MoveDiagMNEdit();
				}
				//----->('23.05.01 Add)
				if ( m_pShinKani01EX2 ) {
					m_pShinKani01EX2->MoveDiagMNEdit();
				}
				//<-----
				break;
			case ID_ICSSH_KSK_HJ_TAB:
				if( m_pHjnKskEX ){
					m_pHjnKskEX->MoveDiagMNEdit();
				}
				else if( m_pHjnKskEX2 ){	// 更正の請求/新様式対応
					m_pHjnKskEX2->MoveDiagMNEdit();
				}
				break;
			case ID_ICSSH_KSK_KJ_TAB:
				if( m_pKjnKskEX ){
					m_pKjnKskEX->MoveDiagMNEdit();
				}
				else if( m_pKjnKskEX2 ){	// 更正の請求/新様式対応
					m_pKjnKskEX2->MoveDiagMNEdit();
				}
				break;
			case ID_ICSSH_CHSK_TAB:
				if( m_pChskEX ){
					m_pChskEX->MoveDiagMNEdit();
				}
				break;
            case ID_ICSSH_KSISANURI_TAB:
				if (m_pKsisanuri) {
					m_pKsisanuri->MoveWindow_JigyoKubunBar(true);
				}
                break;
			case ID_ICSSH_KSISANURI2_TAB:
				if (m_pKsisanuri2) {
					m_pKsisanuri2->MoveWindow_JigyoKubunBar(true);
				}
				break;
			case ID_ICSSH_KSHIIRE_TAB:
				if (m_pKshiire) {
					m_pKshiire->MoveWindow_JigyoKubunBar(true);
				}
				break;
			default:
				break;
		}
	}

	ICSFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CH31HyoView::OnTcnSelchangingMainTab(NMHDR * pNotifyStruct, LRESULT * result)
{

	int selno = m_MainTab.GetCurSel();
	int tabno;
	for( tabno = 0; tabno < m_TabMng.GetCount(); tabno++ ){

		if( m_TabMng[tabno].tabtype == ID_ICSSH_FHYO6_TAB ){
			if( selno == tabno ){
				if( tabno < m_TabMng.GetCount() ){
					if( Message_MyNumber_error() < 0 ){
						m_MainTab.SetCurSel( tabno );
						m_nowTabIdx = tabno;
						return;
					}
					if( Message_MyNumber_overlap() == -1 ){
						m_MainTab.SetCurSel( tabno );
						m_nowTabIdx = tabno;
						return;
					}
					EditOff_Fuhyo6();
					if( Message_SozokuWariai() == -1 ){
						m_MainTab.SetCurSel( tabno );
						m_nowTabIdx = tabno;
						return;
					}
				}
			}
		}
		if( m_TabMng[tabno].tabtype == ID_ICSSH_REFUND_HJ_02_TAB ||
			m_TabMng[tabno].tabtype == ID_ICSSH_REFUND_KJ_02_TAB ){
			if( selno == tabno ){	
				CheckInvno( 2 );
			}
		}
	}
	/*if( selno == tabno ){
		if( tabno < m_TabMng.GetCount() ){
			if( Message_MyNumber_error() < 0 ){
				m_MainTab.SetCurSel( tabno );
				m_nowTabIdx = tabno;

				return;
			}
			if( Message_MyNumber_overlap() == -1 ){
				m_MainTab.SetCurSel( tabno );
				m_nowTabIdx = tabno;

				return;
			}
			EditOff_Fuhyo6();
			if( Message_SozokuWariai() == -1 ){
				m_MainTab.SetCurSel( tabno );
				m_nowTabIdx = tabno;

				return;
			}
		}
	}*/

	UpdateSheet( selno );
}

void CH31HyoView::UpdateSheet(int selno, int get_sgn)
{

	if (selno == -1) {
		return;
	}

	BOOL getdata = FALSE;
	if ((m_MainTab.GetCurSel() == selno) && (get_sgn == 0)) {
		getdata = TRUE;
	}

	//計算はまだ考慮していない
	if (m_pSnHeadData->IsMiddleProvisional() == FALSE) {
		if ((int)m_TabMng.GetCount() <= selno) {
			return;
		}

		if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) { //簡易
			// 経過措置対象の課税資産の譲渡等あり
			if (m_pSnHeadData->SVmzsw == 1) {
				if (m_pFhyo4_02) {
					if (getdata) {
						m_pFhyo4_02->GetAllDiagData();
					}
					m_pFhyo4_02->MemoryReget();
					m_pFhyo4_02->WriteData();
				}
				
				if (m_pFhyo4_01) {
					if (getdata) {
						m_pFhyo4_01->GetAllDiagData();
					}
					m_pFhyo4_01->MemoryReget();
					m_pFhyo4_01->WriteData();
				}

				if (m_pFhyo5_02) {
					m_pFhyo5_02->MemoryReget();
					m_pFhyo5_02->WriteData();
				}

				if (m_pFhyo5_022) {
					m_pFhyo5_022->Disp_f5022();
					m_pFhyo5_022->MemoryReget();
					m_pFhyo5_022->WriteData();
				}
				
				if (m_pFhyo5_01) {
					m_pFhyo5_01->MemoryReget();
					m_pFhyo5_01->WriteData();
				}
				
				if (m_pFhyo5_012) {
					m_pFhyo5_012->Disp_f5012();
					m_pFhyo5_012->MemoryReget();
					m_pFhyo5_012->WriteData();
				}
				//----->('23.05.01 Add)
				if (m_pFhyo6_01) {
					m_pFhyo6_01->MemoryReget();
					m_pFhyo6_01->WriteData();
				}
				//<-----
				if (m_pShinKani01) {
					m_pShinKani01->MemoryReget();
					m_pShinKani01->GetFhyoData();
					m_pShinKani01->WriteData();
				}

				if( m_pShinKani01EX ) {
					m_pShinKani01EX->MemoryReget();
					m_pShinKani01EX->GetFhyoData();
					m_pShinKani01EX->WriteData();
				}
				//----->('23.05.01 Add)
				if (m_pShinKani01EX2) {
					m_pShinKani01EX2->MemoryReget();
					m_pShinKani01EX2->GetFhyoData();
					m_pShinKani01EX2->WriteData();
				}
				//<-----
				if (m_pShinIppan02) {
					m_pShinIppan02->GetFhyoData();
					m_pShinIppan02->WriteData();
				}

				if( m_pSnHeadData->IsSoleProprietor() ){
					if( m_pRefundKjn01 ){
						m_pRefundKjn01->SetTitle();
						m_pRefundKjn01->SetDispPosition(0);
					}

					if( m_pRefundKjn02 ){
						m_pRefundKjn02->SetDispPosition(2);
					}

					//----> 20230704
					if( m_pRefundKjn02EX ){
						m_pRefundKjn02EX->SetDispPosition(2);
					}
					//<---- 20230704
				}
				else{
					if( m_pRefundHjn01 ){
						m_pRefundHjn01->SetTitle();
						m_pRefundHjn01->SetDispPosition(1);
					}

					if( m_pRefundHjn02 ){
						m_pRefundHjn02->SetDispPosition(3);
					}

					//----> 20230704
					if( m_pRefundHjn02EX ){
						m_pRefundHjn02EX->SetDispPosition(3);
					}
					//<---- 20230704
				}
			}
			else {
				if (m_pFhyo4_01) {
					if (getdata) {
						m_pFhyo4_01->GetAllDiagData();
					}
					m_pFhyo4_01->MemoryReget();
					m_pFhyo4_01->WriteData();
				}
				
				if (m_pFhyo5_01) {
					m_pFhyo5_01->MemoryReget();
					m_pFhyo5_01->WriteData();
				}
				
				if (m_pFhyo5_012) {
					m_pFhyo5_012->Disp_f5012();
					m_pFhyo5_012->MemoryReget();
					m_pFhyo5_012->WriteData();
				}
				
				if ( m_pFhyo4_03 ) {
					if ( getdata ) {
						m_pFhyo4_03->GetAllDiagData();
					}
					m_pFhyo4_03->MemoryReget();
					m_pFhyo4_03->WriteData();
				}

				if ( m_pFhyo5_03 ) {
					m_pFhyo5_03->MemoryReget();
					m_pFhyo5_03->WriteData();
				}

				if ( m_pFhyo5_032 ) {
					m_pFhyo5_032->DispFh5_032();
					m_pFhyo5_032->MemoryReget();
					m_pFhyo5_032->WriteData();
				}
				//----->('23.05.01 Add)
				if ( m_pFhyo6_01 ) {
					m_pFhyo6_01->MemoryReget();
					m_pFhyo6_01->WriteData();
				}
				//<-----
				if (m_pShinKani01) {
					m_pShinKani01->MemoryReget();
					m_pShinKani01->GetFhyoData();
					m_pShinKani01->WriteData();
				}

				if( m_pShinKani01EX ) {
					m_pShinKani01EX->MemoryReget();
					m_pShinKani01EX->GetFhyoData();
					m_pShinKani01EX->WriteData();
				}
				//----->('23.05.01 Add)
				if (m_pShinKani01EX2) {
					m_pShinKani01EX2->MemoryReget();
					m_pShinKani01EX2->GetFhyoData();
					m_pShinKani01EX2->WriteData();
				}
				//<-----
				if (m_pShinIppan02) {
					m_pShinIppan02->GetFhyoData();
					m_pShinIppan02->WriteData();
				}

				if( m_pSnHeadData->IsSoleProprietor() ){
					if( m_pRefundKjn01 ){
						m_pRefundKjn01->SetTitle();
						m_pRefundKjn01->SetDispPosition(0);
					}

					if( m_pRefundKjn02 ){
						m_pRefundKjn02->SetDispPosition(2);
					}

					//----> 20230704
					if( m_pRefundKjn02EX ){
						m_pRefundKjn02EX->SetDispPosition(2);
					}
					//<---- 20230704
				}
				else{
					if( m_pRefundHjn01 ){
						m_pRefundHjn01->SetTitle();
						m_pRefundHjn01->SetDispPosition(1);
					}

					if( m_pRefundHjn02 ){
						m_pRefundHjn02->SetDispPosition(3);
					}

					//----> 20230704
					if( m_pRefundHjn02EX ){
						m_pRefundHjn02EX->SetDispPosition(3);
					}
					//<---- 20230704
				}
			}
		}
		else { //原則
			// 経過措置対象の課税資産の譲渡等あり
			if (m_pSnHeadData->SVmzsw == 1) {

				//----> 20230616
				if( m_pSnHeadData->m_DispTabSgn & 0x2000 ){
					if (m_pFhyo4_02) {
						if (getdata) {
							m_pFhyo4_02->GetAllDiagData();
						}
						m_pFhyo4_02->MemoryReget();
						m_pFhyo4_02->WriteData();
					}
				
					if (m_pFhyo4_01) {
						if (getdata) {
							m_pFhyo4_01->GetAllDiagData();
						}
						m_pFhyo4_01->MemoryReget();
						m_pFhyo4_01->WriteData();
					}

					if (m_pFhyo5_02) {
						m_pFhyo5_02->MemoryReget();
						m_pFhyo5_02->WriteData();
					}

					if (m_pFhyo5_022) {
						m_pFhyo5_022->Disp_f5022();
						m_pFhyo5_022->MemoryReget();
						m_pFhyo5_022->WriteData();
					}
				
					if (m_pFhyo5_01) {
						m_pFhyo5_01->MemoryReget();
						m_pFhyo5_01->WriteData();
					}
				
					if (m_pFhyo5_012) {
						m_pFhyo5_012->Disp_f5012();
						m_pFhyo5_012->MemoryReget();
						m_pFhyo5_012->WriteData();
					}
				}
				//<---- 20230616
				else{
					if (m_pFhyo2_02) {
						m_pFhyo2_02->MemoryReget();
						m_pFhyo2_02->WriteData(0);
					}

					if (m_pFhyo2_01) {
						m_pFhyo2_01->MemoryReget();
						m_pFhyo2_01->WriteData(0);
					}

					if (m_pFhyo2_12) {
						m_pFhyo2_12->MemoryReget();
						m_pFhyo2_12->WriteData();
					}

					//---->20230120
					if (m_pFhyo2_12Ex) {
						m_pFhyo2_12Ex->MemoryReget();
						m_pFhyo2_12Ex->WriteData();
					}
					//<----20230120

					if ( m_pFhyo2_11 ) {
						m_pFhyo2_11->MemoryReget();
						m_pFhyo2_11->WriteData();
					}

					//---->20230120
					if ( m_pFhyo2_11Ex ) {
						m_pFhyo2_11Ex->MemoryReget();
						m_pFhyo2_11Ex->WriteData();
					}
					//<----20230120

					if (m_pFhyo1_02) {
						m_pFhyo1_02->MemoryReget();
						m_pFhyo1_02->WriteData();
					}
				
					if (m_pFhyo1_01) {
						m_pFhyo1_01->MemoryReget();
						m_pFhyo1_01->WriteData();
					}
					//----->('23.05.01 Add)
					if (m_pFhyo6_01) {
						m_pFhyo6_01->MemoryReget();
						m_pFhyo6_01->WriteData();
					}
					//<-----
				}

				if (m_pShinIppan01) {
					m_pShinIppan01->MemoryReget();
					m_pShinIppan01->WriteData();
				}

				if( m_pShinIppan01EX ) {
					m_pShinIppan01EX->MemoryReget();
					m_pShinIppan01EX->WriteData();
				}
				//----->('23.05.01 Add)
				if( m_pShinIppan01EX2 ) {
					m_pShinIppan01EX2->MemoryReget();
					m_pShinIppan01EX2->WriteData();
				}
				//<-----
				if (m_pShinIppan02) {
					m_pShinIppan02->GetFhyoData();
					m_pShinIppan02->WriteData();
				}

				if( m_pSnHeadData->IsSoleProprietor() ){
					if( m_pRefundKjn01 ){
						m_pRefundKjn01->SetTitle();
						m_pRefundKjn01->SetDispPosition(0);
					}

					if( m_pRefundKjn02 ){
						if ((m_pSnHeadData->m_DispTabSgn & 0xc0) == 0xc0)	// 特例の金額を集計
							m_pSnRefundData->ConnectSkjZeigaku( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
						m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo2_01Data, m_ShRevType );
					}

					if( m_pRefundKjn02EX ){
						m_pRefundKjn02EX->calq_by_OtherChange( 3, m_pSnFhyo2_01Data, m_ShRevType );
					}
				}
				else{
					if( m_pRefundHjn01 ){
						m_pRefundHjn01->SetTitle();
						m_pRefundHjn01->SetDispPosition(1);
					}

					if( m_pRefundHjn02 ){
						if ((m_pSnHeadData->m_DispTabSgn & 0xc0) == 0xc0)	// 特例の金額を集計
							m_pSnRefundData->ConnectSkjZeigaku( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
						m_pRefundHjn02->calq_by_OtherChange( 2, m_pSnFhyo2_01Data, m_ShRevType );
					}

					if( m_pRefundHjn02EX ){
						m_pRefundHjn02EX->calq_by_OtherChange( 2, m_pSnFhyo2_01Data, m_ShRevType );
					}
				}
			}
			else {
				if (m_pFhyo2_01) {
					m_pFhyo2_01->MemoryReget();
					m_pFhyo2_01->WriteData(0);
				}

				if ( m_pFhyo2_11 ) {
					m_pFhyo2_11->MemoryReget();
					m_pFhyo2_11->WriteData();
				}

				//---->20230120
				if ( m_pFhyo2_11Ex ) {
					m_pFhyo2_11Ex->MemoryReget();
					m_pFhyo2_11Ex->WriteData();
				}
				//<----20230120
		
				if (m_pFhyo1_01) {
					m_pFhyo1_01->MemoryReget();
					m_pFhyo1_01->WriteData();
				}

				if ( m_pFhyo2_03 ) {
					m_pFhyo2_03->MemoryReget();
					m_pFhyo2_03->WriteData();
				}

				//---->20230120
				if ( m_pFhyo2_03Ex ) {
					m_pFhyo2_03Ex->MemoryReget();
					m_pFhyo2_03Ex->WriteData();
				}
				//<----20230120

				if ( m_pFhyo1_03 ) {
					m_pFhyo1_03->MemoryReget();
					m_pFhyo1_03->WriteData();
				}
				//----->('23.05.01 Add)
				if (m_pFhyo6_01) {
					m_pFhyo6_01->MemoryReget();
					m_pFhyo6_01->WriteData();
				}
				//<-----				
				if (m_pShinIppan01) {
					m_pShinIppan01->MemoryReget();
					m_pShinIppan01->WriteData();
				}

				if( m_pShinIppan01EX ) {
					m_pShinIppan01EX->MemoryReget();
					m_pShinIppan01EX->WriteData();
				}
				//----->('23.05.01 Add)
				if( m_pShinIppan01EX2) {
					m_pShinIppan01EX2->MemoryReget();
					m_pShinIppan01EX2->WriteData();
				}
				//<-----
				if (m_pShinIppan02) {
					m_pShinIppan02->GetFhyoData();
					m_pShinIppan02->WriteData();
				}

				if( m_pSnHeadData->IsSoleProprietor() ){
					if( m_pRefundKjn01 ){
						m_pRefundKjn01->SetTitle();
						m_pRefundKjn01->SetDispPosition(0);
					}

					if( m_pRefundKjn02 ){
						if ((m_pSnHeadData->m_DispTabSgn & 0xc0) == 0xc0)	// 特例の金額を集計
							m_pSnRefundData->ConnectSkjZeigaku( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
						m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo2_01Data, m_ShRevType );
					}

					if( m_pRefundKjn02EX ){
						m_pRefundKjn02EX->calq_by_OtherChange( 3, m_pSnFhyo2_01Data, m_ShRevType );
					}
				}
				else{
					if( m_pRefundHjn01 ){
						m_pRefundHjn01->SetTitle();
						m_pRefundHjn01->SetDispPosition(1);
					}

					if( m_pRefundHjn02 ){
						if ((m_pSnHeadData->m_DispTabSgn & 0xc0) == 0xc0)	// 特例の金額を集計
							m_pSnRefundData->ConnectSkjZeigaku( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
						m_pRefundHjn02->calq_by_OtherChange( 2, m_pSnFhyo2_01Data, m_ShRevType );
					}

					if( m_pRefundHjn02EX ){
						m_pRefundHjn02EX->calq_by_OtherChange( 2, m_pSnFhyo2_01Data, m_ShRevType );
					}
				}
			}

		}

		/*if (m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB) { // 付表６
			if (m_pFhyo6) {
				if ((m_MainTab.GetCurSel() == selno) && (get_sgn == 0)) {
					m_pFhyo6->GetAllDiagData();
				}

				m_pFhyo6->WriteData();

				if ((m_MainTab.GetCurSel() == selno) && (get_sgn == 0)) {
					m_pFhyo6->MoveFocus_FromMyNum();
				}
			}
		}*/


		//----->yoshida200114
		//申告書画面立ち上げ状態で仕訳入力。その後申告書戻り、再計算かかり基本画面戻る。付表を表示し、付表６表示せずプレビュ―すると
		//ここのGetAllDiagDataで画面データ（更新前の情報）が書き込まれてしまっている。
		//付表６画面立ち上げ時しか取得しないよう修正
		if (m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB) { // 付表６
			if (m_pFhyo6) {
				if ((m_MainTab.GetCurSel() == selno) && (get_sgn == 0)) {
					m_pFhyo6->GetAllDiagData();
				}
				m_pFhyo6->WriteData();

				if ((m_MainTab.GetCurSel() == selno) && (get_sgn == 0)) {
					m_pFhyo6->MoveFocus_FromMyNum();
				}
			}
		}
		//------
//		if (m_pFhyo6) {
//			m_pFhyo6->GetAllDiagData();
//			m_pFhyo6->WriteData();
//			m_pFhyo6->MoveFocus_FromMyNum();
//		}
		//<-------

		//if (m_TabMng[selno].tabtype == ID_ICSSH_KSISANURI_TAB) {
		if ( m_pKsisanuri ) {
			m_pKsisanuri->m_pSimKuriwari->EndDlg(IDCANCEL);
			m_pKsisanuri->GetAllDiagData();
			m_pKsisanuri->WriteData(m_SnSeq, TOKUREI_TBL_VERSION);
            m_Uri1MxPage = m_pKsisanuri->GetMxPrtPage();
            if( m_pKshiire ) {
                m_pKshiire->SetSyukeiOnRendo(1);
			}
		}

		//else if (m_TabMng[selno].tabtype == ID_ICSSH_KSISANURI2_TAB) {
		if (m_pKsisanuri2) {
			m_pKsisanuri2->GetAllDiagData();
			m_pKsisanuri2->WriteData(m_SnSeq, TOKUREI_TBL_VERSION);
			m_Uri2MxPage = m_pKsisanuri2->GetMxPrtPage();
		}
		
		//else if (m_TabMng[selno].tabtype == ID_ICSSH_KSHIIRE_TAB) {
		if ( m_pKshiire ) {
			m_pKshiire->GetAllDiagData();
			m_pKshiire->WriteData(m_SnSeq, TOKUREI_TBL_VERSION);
			m_ShiireMxPage = m_pKshiire->GetMxPrtPage();
		}

	}

}

void CH31HyoView::OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result)
{

	int selno = m_MainTab.GetCurSel();

	if( m_nowTabIdx != selno ){

		if( m_TabMng.GetCount() ){
			if(m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB){
				if( IsExist_MyNumber_Fuhyo6()&&!m_NoHistory_browse){
					WriteNoHistory(0);
					m_NoHistory_browse = TRUE;
				}			
			}
		}
		int tabno;
		for( tabno = 0; tabno < m_TabMng.GetCount(); tabno++ ){
			if( m_TabMng[tabno].tabtype == ID_ICSSH_FHYO6_TAB ){
				break;
			}
		}
		if( selno != tabno ){
			if( tabno < m_TabMng.GetCount() ){
				if(m_pFhyo6->MyNumber_CheckFromDisp(1) < 0 ){
					m_MainTab.SetCurSel( tabno );
					m_nowTabIdx = tabno;

					return;
				}
				if(m_pFhyo6->MyNumberCheck(1) == -1 ){
					m_MainTab.SetCurSel( tabno );
					m_nowTabIdx = tabno;

					return;
				}
				if( m_pFhyo6->SozokubunCheck(1) == -1 ){
					m_MainTab.SetCurSel( tabno );
					m_nowTabIdx = tabno;

					return;
				}
			}
		}

		// 各表の更新処理後の再表示
		IndicateSheet( selno );
		SetItemPosition( selno );

		m_nowTabIdx = selno;
	}else{
		SetItemPosition( selno );
	}
}



BOOL CH31HyoView::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {
			WPARAM	wParam = 0;

			if (m_pShinInfo && m_pShinInfo->pParent) {
				if ((m_pSnHeadData != NULL) && m_pSnHeadData->IsMiddleProvisional()) {
					// 中間申告は正常な特定課税仕入として扱う
					wParam = 1;
				}
				else {
					int st = IsAbleTkKz();
					if (st == -1) {
						// 選択を変更する
						ChangeTkkzUse(1, 3);
						return TRUE;
					}
					else if (st == -2) {
						if (ChkZkTkkzReturn(1, 3) == 0) {
							return TRUE;
						}
						wParam = 1;	// 正常な特定課税仕入
					}
					
				}

				m_pShinInfo->pParent->SendMessage(WM_USER_SET_ENDSTART, wParam);
				m_pShinInfo->pParent->PostMessage(WM_CLOSE, 0);
				return TRUE;
			}
		}
		else if (pMsg->wParam == VK_RETURN) {
			if (m_TabMng.GetCount()) {
				SetItemPosition(m_MainTab.GetCurSel());
				return TRUE;
			}
		}
	}
	else if ((pMsg->message == WM_MOUSEWHEEL)) {
		if (pMsg->hwnd == GetSafeHwnd()) {
			CWnd* pWnd = WindowFromPoint(pMsg->pt);
			if (pWnd && IsChild(pWnd)) {
				pMsg->hwnd = pWnd->GetSafeHwnd();
			}
		}
	}

	return ICSFormView::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// 特定課税仕入れが可能か？
//-----------------------------------------------------------------------------
// 返送値	1	：	特定課税仕入を行っている状態で、特定課税仕入の処理可能
//			0	：	特定課税仕入を行っていない
//			-1	：	特定課税仕入を行っている状態で、特定課税仕入の処理不可能
//			-2	：	特定課税仕入を行っている状態で、通常は特定課税仕入の処理不可能だが
//					前期以前の返還である可能性があるので注意喚起して処理
//-----------------------------------------------------------------------------
int CH31HyoView::IsAbleTkKz()
{
	if ( m_pSnHeadData == NULL) {
		ASSERT( !_T("特定課税仕入可能チェックのエラー") );
		return 0;
	}

	int nRt = 0;

	// 非連動？
	if ( !(m_pSnHeadData->Sn_Sign4 & 0x80) ) {
		;
	}
	// 特定課税仕入れがON？
	else if ( m_pSnHeadData->IsNeedTkkzDisp() == FALSE  ) {
		;
	}
	// 中間申告？
	else if (m_pSnHeadData->IsMiddleProvisional()) {
		;
	}
	else {
		int selno = m_MainTab.GetCurSel();
		if ( m_pSnHeadData->SVmzsw == 1 ) {	// 経過措置あり
			if ((m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_02_TAB) && (m_pSnFhyo1_02Data != NULL) ||
				(m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_02_TAB) && (m_pSnFhyo2_02Data != NULL) ||
				(m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_01_TAB) && (m_pSnFhyo1_01Data != NULL) || 
				(m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_01_TAB) && (m_pSnFhyo2_01Data != NULL)) {
				UpdateSheet(selno);
			}
		}
		else {	// 経過措置なし
			if ((m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_01_TAB) && (m_pSnFhyo1_01Data != NULL) ||
				(m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_01_TAB) && (m_pSnFhyo2_01Data != NULL) ||
			    ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_03_TAB ) && ( m_pSnFhyo1_01Data != NULL ) ||
				( m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_03_TAB ) && ( m_pSnFhyo2_01Data != NULL ) ) {
				UpdateSheet ( selno );
			}
		}

		// 判定
		if (IsRatio95Over()) {	// 課税売上割合が95%以上か？
			// 特定課税仕入の返還無し？
			//-->yoshida190723
			if (m_pSnHeadData->SVmzsw == 1) {//経過措置あり
				if (m_Arith.l_test(((CH31SnFhyo1_02Data*)m_pSnFhyo1_02Data)->Sn_F102_5_2C) == 0 && 
					m_Arith.l_test(((CH31SnFhyo1_01Data*)m_pSnFhyo1_01Data)->Sn_F101_5_2E) == 0) {
					nRt = -1;
				}
				else {
					nRt = -2;
				}
			}
			//<--
			else{
				if( m_Arith.l_test(((CH31SnFhyo1_01Data*)m_pSnFhyo1_01Data)->Sn_F101_5_2E) == 0 ){
					nRt = -1;
				}
				else{
					nRt = -2;
				}
			}
		}
		else {
			nRt = 1;
		}
	}

	return nRt;
}

//-----------------------------------------------------------------------------
// 特定課税仕入れを「あり」から「なし」へ
//-----------------------------------------------------------------------------
// 引数	srctab		：	呼び出し元ページ（0:基本情報, 1:本表・付表, 2:課売・特定収入）
//		calltype	：　その処理から呼び出してるか？（1:印刷, 2:プレビュー, 3:処理終了）
//-----------------------------------------------------------------------------
// 返送値	1		：　正常終了（ページ移動なし）
//			2		：　正常終了（ページ移動あり）
//-----------------------------------------------------------------------------
int CH31HyoView::ChangeTkkzUse(int srctab, int calltype)
{
	int st = ICSMessageBox(_T("課税売上割合が95％以上です。特定課税仕入は、なかったものとされます。\n特定課税仕入を無しにしてよろしいですか？"), (MB_YESNO | MB_DEFBUTTON2), 0, 0, this);

	if ( st == IDNO ) {
		return 1;
	}

	// 特定課税仕入れを強制的になしにする <-- ?
	if ( (m_pShinInfo == NULL) || (m_pShinInfo->pParent == NULL) ) {
		return 1;
	}

	// 金額のクリア
	if ( m_pFhyo1_01 ) {	// 付表1－1
		m_pFhyo1_01->ClearTkkzMony();
	}
	if ( m_pFhyo1_02 ) {	// 付表1－2
		m_pFhyo1_02->ClearTkkzMony();
	}
	if ( m_pFhyo1_03 ) {	// 付表1－3
		m_pFhyo1_03->ClearTkkzMony();
	}

	m_pShinInfo->pParent->PostMessage( WM_USER_RESET_TKKZ, 0 );

	return 0;
}

//-----------------------------------------------------------------------------
// 履歴シーケンスセット
//-----------------------------------------------------------------------------
// 引数	srctab		：	呼び出し元ページ（0:基本情報, 1:本表・付表, 2:課売・特定収入）
//		calltype	：	何の処理で呼び出しているか（1:印刷, 2:プレビュー, 3:処理終了）
//-----------------------------------------------------------------------------
// 返送値	1		：	処理継続
//			0		：	処理中断
//-----------------------------------------------------------------------------
int CH31HyoView::ChkZkTkkzReturn(int srctab, int calltype)
{
	int st = ICSMessageBox(_T("課税売上割合が95％を上回っていて、かつ特定課税仕入の返還等対価に係る税額に金額の発生があります。\n\n"
							  "特定課税仕入の返還等対価に係る税額が、前課税期間以前の課税期間において申告された特定課税仕入に係る対価の\n返還を受けたのであれば問題はありません。\n\n"
							  "処理を継続してよろしいですか？"), (MB_YESNO | MB_DEFBUTTON2), 0, 0, this);


	if (st == IDNO) {
		return 0;
	}

	return 1;
}



//-----------------------------------------------------------------------------
// 履歴シーケンスセット
//-----------------------------------------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//-----------------------------------------------------------------------------
void CH31HyoView::SetSnSeq( int snseq )
{
	m_SnSeq = snseq;
	m_SvSnSeq = snseq;
}


//-----------------------------------------------------------------------------
// 表示帳票の最終データ取得
//-----------------------------------------------------------------------------
void CH31HyoView::GetDispHyoData()
{	
	// ●特定課税仕入　未対応
	int selno = m_MainTab.GetCurSel();
	int max = (int)m_TabMng.GetCount();
	if( (selno>=max) || (selno==-1) ){
		return;
	}
	if( m_bShowed == FALSE ){
		return;
	}

	int pas = 0;	// 引数不明（仮）

	if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_01_TAB ){
		if( m_pFhyo1_01 ){
			m_pFhyo1_01->GetAllDiagData();
			m_pFhyo1_01->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_02_TAB ){
		if( m_pFhyo1_02 ){
			m_pFhyo1_02->GetAllDiagData();
			m_pFhyo1_02->WriteData();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_03_TAB ) {
		if ( m_pFhyo1_03 ) {
			m_pFhyo1_03->GetAllDiagData();
			m_pFhyo1_03->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_01_TAB ){
		if( m_pFhyo2_01 ){
			m_pFhyo2_01->GetAllDiagData();
			m_pFhyo2_01->WriteData( pas );
		}

		if ( m_pFhyo2_11 ){
			m_pFhyo2_11->GetAllDiagData();
			m_pFhyo2_11->WriteData();
		}

		//---->20230120
		if ( m_pFhyo2_11Ex ){
			m_pFhyo2_11Ex->GetAllDiagData();
			m_pFhyo2_11Ex->WriteData();
		}
		//<----20230120
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_02_TAB ){
		if( m_pFhyo2_02 ){
			m_pFhyo2_02->GetAllDiagData();
			m_pFhyo2_02->WriteData( pas );
		}

		if( m_pFhyo2_12 ){
			m_pFhyo2_12->GetAllDiagData();
			m_pFhyo2_12->WriteData();
		}

		//---->20230120
		if( m_pFhyo2_12Ex ){
			m_pFhyo2_12Ex->GetAllDiagData();
			m_pFhyo2_12Ex->WriteData();
		}
		//<----20230120
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_03_TAB ) {
		if ( m_pFhyo2_03 ) {
			m_pFhyo2_03->GetAllDiagData();
			m_pFhyo2_03->WriteData();
		}

		//---->20230120
		if ( m_pFhyo2_03Ex ) {
			m_pFhyo2_03Ex->GetAllDiagData();
			m_pFhyo2_03Ex->WriteData();
		}
		//<----20230120
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_01_TAB ){
		if( m_pFhyo4_01 ){
			m_pFhyo4_01->GetAllDiagData();
			m_pFhyo4_01->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_02_TAB ){
		if( m_pFhyo4_02 ){
			m_pFhyo4_02->GetAllDiagData();
			m_pFhyo4_02->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_03_TAB ) {
		if ( m_pFhyo4_03 ) {
			m_pFhyo4_03->GetAllDiagData();
			m_pFhyo4_03->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_01_TAB ){
		if( m_pFhyo5_01 ){
			m_pFhyo5_01->GetAllDiagData();
			m_pFhyo5_01->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_012_TAB ){
		if( m_pFhyo5_012 ){
			m_pFhyo5_012->GetAllDiagData();
			m_pFhyo5_012->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_02_TAB ){
		if( m_pFhyo5_02 ){
			m_pFhyo5_02->GetAllDiagData();
			m_pFhyo5_02->WriteData();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_022_TAB ){
		if( m_pFhyo5_022 ){
			m_pFhyo5_022->GetAllDiagData();
			m_pFhyo5_022->WriteData();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_03_TAB ) {
		if ( m_pFhyo5_03 ){
			m_pFhyo5_03->GetAllDiagData();
			m_pFhyo5_03->WriteData();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_032_TAB ) {
		if ( m_pFhyo5_032 ) {
			m_pFhyo5_032->GetAllDiagData();
			m_pFhyo5_032->WriteData();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB) {
		if (m_pFhyo6) {
			m_pFhyo6->WriteData();
		}
	}
//----->('23.05.01 Add)
	else if (m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_01_TAB) {
		if (m_pFhyo6_01) {
			m_pFhyo6_01->GetAllDiagData();
			m_pFhyo6_01->WriteData();
		}
	}
//<-----
	else if( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB ){
		if( m_pShinIppan01 ){
			m_pShinIppan01->GetDiagData();
			m_pShinIppan01->WriteData();
		}
		if( m_pShinIppan01EX ) {
			m_pShinIppan01EX->GetDiagData();
			m_pShinIppan01EX->WriteData();
		}
		//----->('23.05.01 Add)
		if( m_pShinIppan01EX2 ) {
			m_pShinIppan01EX2->GetDiagData();
			m_pShinIppan01EX2->WriteData();
		}
		//<-----
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB ){
		if( m_pShinKani01 ){
			m_pShinKani01->GetDiagData();
			m_pShinKani01->WriteData();
		}
		if( m_pShinKani01EX ) {
			m_pShinKani01EX->GetDiagData();
			m_pShinKani01EX->WriteData();
		}
		//----->('23.05.01 Add)
		if (m_pShinKani01EX2) {
			m_pShinKani01EX2->GetDiagData();
			m_pShinKani01EX2->WriteData();
		}
		//<-----
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN02_TAB ){
		if( m_pShinIppan02 ){
			m_pShinIppan02->WriteData();
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// データ書込み
//-----------------------------------------------------------------------------
// 引数	isEnd		：	最終データ？
//		isGetData	：	データを取得するかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::WriteData( BOOL isEnd, BOOL isGetData )
{
	// ●特定課税仕入　未対応
	int pas = 0;	// 引数不明（仮）

	// 一般本表
	if( m_pSnHonpyoData ){
		if( isGetData ){
			if( m_bShowed ){
				if( m_pShinIppan01 ){
					m_pShinIppan01->WriteData();
				}
				if( m_pShinIppan01EX ) {
					m_pShinIppan01EX->WriteData();
				}
				//----->('23.05.01 Add)
				if( m_pShinIppan01EX2 ) {
					m_pShinIppan01EX2->WriteData();
				}
				//<-----

				if( m_pShinKani01 ){
					m_pShinKani01->WriteData();
				}
				if( m_pShinKani01EX ) {
					m_pShinKani01EX->WriteData();
				}
				//----->('23.05.01 Add)
				if (m_pShinKani01EX2) {
					m_pShinKani01EX2->WriteData();
				}
				//<-----
			}
		}

		if( m_pSnHonpyoData->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}

		if( isEnd ){
			delete m_pSnHonpyoData;
			m_pSnHonpyoData = NULL;
		}
	}

	// 一般本表2
	if( m_pSnHonpyo2Data ){
		if( isGetData ){
			if( m_bShowed ){
				if( m_pShinIppan02 ){
					m_pShinIppan02->WriteData();
				}
			}
		}

		if( m_pSnHonpyo2Data->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}

		if( isEnd ){
			delete m_pSnHonpyo2Data;
			m_pSnHonpyo2Data = NULL;
		}
	}

	// 付表１データ
	if( m_pSnFhyo1_01Data ){
		if( isGetData ){
			if( m_bShowed ){
				if( m_pFhyo1_01 ){
					m_pFhyo1_01->WriteData();
				}
				if( m_pFhyo1_02 ){
					m_pFhyo1_02->WriteData();
				}
				if ( m_pFhyo1_03 ) {
					m_pFhyo1_03->WriteData();
				}
			}
		}

		if( m_pSnFhyo1_01Data->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}

		if( isEnd ){
			delete m_pSnFhyo1_01Data;
			m_pSnFhyo1_01Data = NULL;
		}
	}

	// 付表1-2データ
	if ( m_pSnFhyo1_02Data ) {

		if ( m_pSnFhyo1_02Data->SetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		if ( isEnd ) {
			delete m_pSnFhyo1_02Data;
			m_pSnFhyo1_02Data = NULL;
		}
	}

	// 付表２データ
	if( m_pSnFhyo2_01Data ){
		if( isGetData ){
			if( m_bShowed ){
				if( m_pFhyo2_02 ){
					m_pFhyo2_02->WriteData( pas );
				}
				if( m_pFhyo2_01 ){
					m_pFhyo2_01->WriteData( pas );
				}

				if( m_pFhyo2_12 ){
					m_pFhyo2_12->WriteData();
				}
				if( m_pFhyo2_11 ){
					m_pFhyo2_11->WriteData();
				}
				if ( m_pFhyo2_03 ) {
					m_pFhyo2_03->WriteData();
				}

				//---->20230120
				if( m_pFhyo2_12Ex ){
					m_pFhyo2_12Ex->WriteData();
				}
				if( m_pFhyo2_11Ex ){
					m_pFhyo2_11Ex->WriteData();
				}
				if ( m_pFhyo2_03Ex ) {
					m_pFhyo2_03Ex->WriteData();
				}
				//<----20230120
			}
		}

		if( m_pSnFhyo2_01Data->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}

		if( isEnd ){
			delete m_pSnFhyo2_01Data;
			m_pSnFhyo2_01Data = NULL;
		}
	}

	if ( m_pSnFhyo2_02Data ) {
		if( m_pSnFhyo2_02Data->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}

		if ( isEnd ) {
			delete m_pSnFhyo2_02Data;
			m_pSnFhyo2_02Data = NULL;
		}
	}

	// 付表４データ
	if( m_pSnFhyo4_01Data ){
		if( isGetData ){
			if( m_bShowed ){
				if( m_pFhyo4_01 ){
					m_pFhyo4_01->WriteData();
				}
				if( m_pFhyo4_02 ){
					m_pFhyo4_02->WriteData();
				}
				if ( m_pFhyo4_03 ) {
					m_pFhyo4_03->WriteData();
				}
			}
		}

		if( m_pSnFhyo4_01Data->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}

		if( isEnd ){
			delete m_pSnFhyo4_01Data;
			m_pSnFhyo4_01Data = NULL;
		}
	}

	// 付表4-2
	if ( m_pSnFhyo4_02Data ) {
		if( m_pSnFhyo4_02Data->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}

		if ( isEnd ) {
			delete m_pSnFhyo4_02Data;
			m_pSnFhyo4_02Data = NULL;
		}
	}

	// 付表5-1(1)
	if ( m_pSnFhyo5_01Data ) {
		if ( m_pSnFhyo5_01Data->SetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		if ( isEnd ) {
			delete m_pSnFhyo5_01Data;
			m_pSnFhyo5_01Data = NULL;
		}
	}

	// 付表5-1(2)
	if ( m_pSnFhyo5_012Data ) {
		if ( m_pSnFhyo5_012Data->SetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		if ( isEnd ) {
			delete m_pSnFhyo5_012Data;
			m_pSnFhyo5_012Data = NULL;
		}
	}

	// 付表5-2
	if ( m_pSnFhyo5_02Data ) {
		if ( m_pSnFhyo5_02Data->SetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		if ( isEnd ) {
			delete m_pSnFhyo5_02Data;
			m_pSnFhyo5_02Data = NULL;
		}
	}

	// 付表5-2(2)
	if ( m_pSnFhyo5_022Data ) {
		if ( m_pSnFhyo5_022Data->SetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		if ( isEnd ) {
			delete m_pSnFhyo5_022Data;
			m_pSnFhyo5_022Data = NULL;
		}
	}

	// 付表６
	if (m_pSnHeadData->IsSoleProprietor() == TRUE) {
		if (m_pSnFhyo6Data) {
			if (isGetData) {
				if (m_bShowed) {
					if (m_pFhyo6) {	
						m_pFhyo6->WriteData();
					}
				}
			}

			if (m_pSnFhyo6Data->SetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}

			if (isEnd) {
				delete m_pSnFhyo6Data;
				m_pSnFhyo6Data = NULL;
			}
		}
	}

//----->('23.05.01 Add)
	//付表６(２割特例)
	if (m_pSnFhyo6_01Data) {
		if (m_pSnFhyo6_01Data->SetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}

		if (isEnd) {
			delete m_pSnFhyo6_01Data;
			m_pSnFhyo6_01Data = NULL;
		}
	}
//<-----

	// 還付申告データ
	if (m_pSnRefundData) {
		if (m_pSnRefundData->SetData(m_pZmSub, m_pSnHeadData, m_SnSeq)) {
			return -1;
		}
		if (isEnd) {
			delete m_pSnRefundData;
			m_pSnRefundData = NULL;
		}
	}

	// 更正の請求データ
	if ( m_pSnKskData ) {
		if (m_pSnKskData->SetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}
		if ( isEnd ) {
			delete m_pSnKskData;
			m_pSnKskData = NULL;
		}
	}

	// 予定納税
	if (m_pSnChskData) {
		if (m_pSnChskData->SetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}
		if (isEnd) {
			delete m_pSnChskData;
			m_pSnChskData = NULL;
		}
	}

	// 棚卸データ
	if( m_pSnTanaData ){
		if( m_pSnTanaData->SetData( m_pZmSub, m_SnSeq ) ){
			return -1;
		}
		if( isEnd ){
			delete m_pSnTanaData;
			m_pSnTanaData = NULL;
		}
	}
	   
	return 0;
}


//-----------------------------------------------------------------------------
// 部分データ書込み
//-----------------------------------------------------------------------------
// 引数	flg	：	書込み対象
//-----------------------------------------------------------------------------
int CH31HyoView::WriteSepData( int flg )
{
	if ( flg & 0x01 ) {	// 本表データ
		m_pSnHonpyoData->SetData ( m_pZmSub, m_SnSeq );
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 部分データ読込み
//-----------------------------------------------------------------------------
// 引数	flg	：	読込み対象
//-----------------------------------------------------------------------------
int CH31HyoView::ReadSepData( int flg )
{
	if ( flg & 0x01 ) {	// 本表データ
		m_pSnHonpyoData->GetData ( m_pZmSub, m_SnSeq );
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 全画面：画面表示 再初期化
//-----------------------------------------------------------------------------
// 引数	isCompChange	：	強制再初期化？
//		SixKindFlg		：	簡易六趣判定
//		TokuteiFlg		：	改正様式
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH31HyoView::ReInitDisp( BOOL isCompChange, BOOL SixKindFlg/*=FALSE*/, EnumIdIcsShRevType shrevtype/*=ID_ICSSH_NO_REVTYPE*/ )
{
	BOOL	bRecreateTab = FALSE;

	BOOL svSizKindFlg = m_SixKindFlg;
	m_SixKindFlg = SixKindFlg;

	EnumIdIcsShRevType svShrevtype = m_ShRevType;
	m_ShRevType = shrevtype;

	long svDispTabSgn = m_DispTabSgn;
	m_DispTabSgn = m_pSnHeadData->m_DispTabSgn;

	if (isCompChange) {
		bRecreateTab = TRUE;
	}
	else {
		if (m_pSnHeadData) {
			if (m_pSnHeadData->SVmzsw & 0x01) {
				//if ((m_pFhyo5_02 == NULL) && (m_pFhyo5_022 == NULL) && (m_pFhyo4_02 == NULL) && (m_pFhyo2_02 == NULL) && (m_pFhyo1_02 == NULL)) {
				//	bRecreateTab = TRUE;
				//}
				if ( IsNoneMTForm() ) {
					//if ( ( m_pFhyo5_02 == NULL ) && ( m_pFhyo5_022 == NULL ) && ( m_pFhyo4_02 == NULL ) && ( m_pFhyo2_12 == NULL ) && ( m_pFhyo1_02 == NULL ) ) {
					//	bRecreateTab = TRUE;
					//}
					//---->20230120
					if ( ( m_pFhyo5_02 == NULL ) && ( m_pFhyo5_022 == NULL ) && ( m_pFhyo4_02 == NULL ) && ( m_pFhyo2_12 == NULL ) && ( m_pFhyo2_12Ex == NULL ) && ( m_pFhyo1_02 == NULL ) ) {
						bRecreateTab = TRUE;
					}
					//<----20230120
				}
				else {
					if ( ( m_pFhyo5_02 == NULL ) && ( m_pFhyo5_022 == NULL ) && ( m_pFhyo4_02 == NULL ) && ( m_pFhyo2_02 == NULL ) && ( m_pFhyo1_02 == NULL ) ) {
						bRecreateTab = TRUE;
					}
				}
			}
			else {
				//if (m_pFhyo5_02 || m_pFhyo5_022 || m_pFhyo4_02 || m_pFhyo2_02 || m_pFhyo1_02) {
				//	bRecreateTab = TRUE;
				//}
				if ( IsNoneMTForm() ) {
					//if ( m_pFhyo5_02 || m_pFhyo5_022 || m_pFhyo4_02 || m_pFhyo2_12 || m_pFhyo1_02 ) {
					//	bRecreateTab = TRUE;
					//}
					//---->20230120
					if ( m_pFhyo5_02 || m_pFhyo5_022 || m_pFhyo4_02 || m_pFhyo2_12 || m_pFhyo2_12Ex || m_pFhyo1_02 ) {
						bRecreateTab = TRUE;
					}
					//<----20230120
				}
				else {
					if ( m_pFhyo5_02 || m_pFhyo5_022 || m_pFhyo4_02 || m_pFhyo2_02 || m_pFhyo1_02 ) {
						bRecreateTab = TRUE;
					}
				}
			}
		}
	}

	// 10%の期間であれば簡易課税第六種は必ず存在するのでチェックしない
	//if ( svSizKindFlg != m_SixKindFlg ) {
	//	bRecreateTab = TRUE;
	//}

	if ( svShrevtype != m_ShRevType ) {
		bRecreateTab = TRUE;
	}

	if ((svDispTabSgn & 0xf0) != (m_DispTabSgn & 0xf0)) {
		bRecreateTab = TRUE;
	}
	else if ( ( svDispTabSgn & 0x1000 ) != ( m_DispTabSgn & 0x1000 ) ) {
		bRecreateTab = TRUE;
	}

	if( m_pSnHeadData->m_DispTabSgn & 0x80 ){
		long symd = 0, eymd = 0;

		// 特例期間チェック
		//if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		//	symd = m_pSnHeadData->Sn_MDAYS;
		//}
		//else {
		//	symd = m_pSnHeadData->Sn_KDAYS;
		//}
		GetKazeiKikan( symd, eymd );

		if ( ( m_DispTabSgn & 0x40 ) && ( symd > SIIRETOKUREIDAY ) ) {
			bRecreateTab = TRUE;
			m_pSnHeadData->m_DispTabSgn &= ~0x40;
		}

		if ( ( ( m_DispTabSgn & 0x10 ) || ( m_DispTabSgn & 0x20 ) ) && ( symd > URIAGETOKUREIDAY ) ) {		// 20231025 9月30日をまたぐ間は適用可能なため、開始日が超えているかどうかで判定する
		//if ( ( ( m_DispTabSgn & 0x10 ) || ( m_DispTabSgn & 0x20 ) ) && ( eymd > URIAGETOKUREIDAY ) ) {	// 20231025 del
			bRecreateTab = TRUE;
			m_pSnHeadData->m_DispTabSgn &= ~0x70;
		}
	}

	if(( svDispTabSgn & 0x2000 ) != ( m_DispTabSgn & 0x2000 )) {
		bRecreateTab = TRUE;
	}

	// ２割特例期間チェック
	if( IsNiwariTokurei() ) {
		long symd = 0, eymd = 0;

		GetKazeiKikan( symd, eymd );
		if(( m_DispTabSgn & 0x2000 ) && ( symd > INVOICE_2WARI_EDAY )) {
			bRecreateTab = TRUE;
			m_pSnHeadData->m_DispTabSgn &= ~0x2000;
		}
	}

	// 非連動サイン ('19.05.24)harino
	char flg[MONY_BUF_SIZE] = { 0 };
	if (m_pSnHeadData->Sn_Sign4 & 0x80) m_Arith.l_input(flg, "1");
	m_pSyzSyukei->SetShinkokuData(_T("FFFFFC"), flg);


	// ---->経過措置サイン yoshida200110
	// 出荷延期。出荷日決まり次第、計算テーブルと一緒に対応をあける。	'20.01.22 出荷
	char keikaflg[MONY_BUF_SIZE] = { 0 };
	if (m_pSnHeadData->SVmzsw == 1) m_Arith.l_input(keikaflg, "1");//経過措置あり
	m_pSyzSyukei->SetShinkokuData(_T("FFFFFB"), keikaflg);
	//<-----

	// 令和2年4月1日以降の課税期間であれば、納税者有利になるよう計算
	long eymd = 0;
	char adv[MONY_BUF_SIZE] = { 0 };
	if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if ( eymd >= TAXADVANTAGE ) {
		m_Arith.l_input ( adv, _T( "1" ) );
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF4000" ), adv );

	// 令和2年8月1日以降の課税期間または、修正申告・更正の請求であれば、簡易課税の計算を修正
	memset ( adv, '\0', sizeof ( adv ) );
	if ( eymd >= TAXMODIFY ) {
		m_Arith.l_input ( adv, _T( "1" ) );
	}
	else {
		if ( ( ( m_pSnHeadData->Sn_SKKBN == 3 ) || ( m_pSnHeadData->Sn_SKKBN == 4 ) ) || ( m_pSnHeadData->Sn_Syuusei & 0x02 ) ) {
			m_Arith.l_input ( adv, _T( "1" ) );
		}
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF5000" ), adv );

	// 新様式？
	memset ( adv, '\0', sizeof ( adv ) );
	if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(m_pZmSub->zvol);
		if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
			if ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) {
				m_Arith.l_input ( adv, _T( "1" ) );
			}
		}
		else {
			m_Arith.l_input ( adv, _T( "1" ) );
		}
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF6000" ), adv );

	// 集計フラグのセット
	SetSyukeiFlg();


	if (bRecreateTab) {
		InitTab(TRUE);
		PostMessage(WM_MYMESSAGE, 0, 0);	// フォーカスセット
	}
	else {

		//---->yoshida190821
		//場所移動
		if (m_pKsisanuri) {
			m_pKsisanuri->Init();
		}

		if (m_pKsisanuri2) {
			m_pKsisanuri2->Init();
		}

		if (m_pKshiire) {
			m_pKshiire->Init();
		}
		//<------

		if ( m_pSnHeadData->SVmzsw == 1 ) {
			// --> '20.01.30 課税取引転記後、付表4に付表5の金額が渡せてなかった為、先に集計モジュールに付表5の金額を渡す。
			if( m_pFhyo5_02 ){
				m_pFhyo5_02->ReadData();
				m_pFhyo5_02->ReadData2();
			}
			if( m_pFhyo5_022 ){
				m_pFhyo5_022->ReadData();
				m_pFhyo5_022->ReadData2();
			}
			if( m_pFhyo5_01 ){
				m_pFhyo5_01->ReadData();
				m_pFhyo5_01->ReadData2();
			}
			if( m_pFhyo5_012 ){
				m_pFhyo5_012->ReadData();
				m_pFhyo5_012->ReadData2();
			}
			// <-- '20.01.30


			if (m_pFhyo4_02) {
				m_pFhyo4_02->Init();
				m_pFhyo4_02->WriteData();
			}

			if (m_pFhyo4_01) {
				m_pFhyo4_01->Init();
				m_pFhyo4_01->WriteData();
			}

			if (m_pFhyo5_02) {
				m_pFhyo5_02->Init();
				m_pFhyo5_02->WriteData();
			}
			if (m_pFhyo5_022) {
				m_pFhyo5_022->Init();
				m_pFhyo5_022->WriteData();
			}

			if (m_pFhyo5_01) {
				m_pFhyo5_01->Init();
				m_pFhyo5_01->WriteData();
			}
			if (m_pFhyo5_012) {
				m_pFhyo5_012->Init();
				m_pFhyo5_012->WriteData();
			}
		

			if (m_pFhyo2_02) {
				// '20.01.28  課税取引計算表からF8申告書転記後、付表を開かずにプレビューを行うと
				// 付表2-2の4・7・8の金額が以前の金額のままになっていたため、先に付表2-1のReadDataを呼ぶ。
				if( m_pFhyo2_01 ){
					m_pFhyo2_01->ReadData(0);
					m_pFhyo2_01->ReadData2();
				}

				m_pFhyo2_02->Init();
				m_pFhyo2_02->WriteData(1);
			}

			if (m_pFhyo2_01) {
				m_pFhyo2_01->Init();
				m_pFhyo2_01->WriteData(1);
			}

			if ( m_pFhyo2_12 ) {
				if ( m_pFhyo2_11 ) {
					m_pFhyo2_11->ReadData();
					m_pFhyo2_11->ReadData2();
				}

				m_pFhyo2_12->Init();
				m_pFhyo2_12->WriteData();
			}

			if ( m_pFhyo2_11 ) {
				m_pFhyo2_11->Init();
				m_pFhyo2_11->WriteData();
			}

			//---->20230120
			if ( m_pFhyo2_12Ex ) {
				if ( m_pFhyo2_11Ex ) {
					m_pFhyo2_11Ex->ReadData();
					m_pFhyo2_11Ex->ReadData2();
				}

				m_pFhyo2_12Ex->Init();
				m_pFhyo2_12Ex->WriteData();
			}

			if ( m_pFhyo2_11Ex ) {
				m_pFhyo2_11Ex->Init();
				m_pFhyo2_11Ex->WriteData();
			}
			//<----20230120

			if (m_pFhyo1_02) {
				m_pFhyo1_02->Init();
				m_pFhyo1_02->WriteData();
			}

			if (m_pFhyo1_01) {
				m_pFhyo1_01->Init();
				m_pFhyo1_01->WriteData();
			}
		
		}
		else {
			if ( m_pFhyo5_03 ) {
				m_pFhyo5_03->ReadData();
				m_pFhyo5_03->ReadData2();
			}
			if ( m_pFhyo5_032 ){
				m_pFhyo5_032->ReadData();
				m_pFhyo5_032->ReadData2();
			}

			if ( m_pFhyo4_03 ) {
				m_pFhyo4_03->Init();
				m_pFhyo4_03->WriteData();
			}

			if ( m_pFhyo5_03 ) {
				m_pFhyo5_03->Init();
				m_pFhyo5_03->WriteData();
			}
			if ( m_pFhyo5_032 ) {
				m_pFhyo5_032->Init();
				m_pFhyo5_032->WriteData();
			}

			if (m_pFhyo4_01) {
				m_pFhyo4_01->Init();
				m_pFhyo4_01->WriteData();
			}

			if (m_pFhyo5_01) {
				m_pFhyo5_01->Init();
				m_pFhyo5_01->WriteData();
			}
			if (m_pFhyo5_012) {
				m_pFhyo5_012->Init();
				m_pFhyo5_012->WriteData();
			}

			if (m_pFhyo2_01) {
				m_pFhyo2_01->Init();
				m_pFhyo2_01->WriteData(1);
			}
			if ( m_pFhyo2_03 ) {
				m_pFhyo2_03->Init();
				m_pFhyo2_03->WriteData();
			}
			//---->20230120
			if ( m_pFhyo2_03Ex ) {
				m_pFhyo2_03Ex->Init();
				m_pFhyo2_03Ex->WriteData();
			}
			//<----20230120

			if (m_pFhyo1_01) {
				m_pFhyo1_01->Init();
				m_pFhyo1_01->WriteData();
			}
			if ( m_pFhyo1_03 ) {
				m_pFhyo1_03->Init();
				m_pFhyo1_03->WriteData();
			}

			// 付表6（2割特例）
			if( m_pFhyo6_01 ){
				m_pFhyo6_01->Init();
				m_pFhyo6_01->WriteData();
			}
		}

		if (m_pShinIppan01) {
			m_pShinIppan01->Init();
			m_pShinIppan01->WriteData();	// '20.01.24 仕訳編集後、付表６の再集計がうまくいかないことがあったため　kasai
		}
		
		if ( m_pShinIppan01EX ) {
			m_pShinIppan01EX->Init();
			m_pShinIppan01EX->WriteData();
		}
		//----->('23.05.01 Add)
		if ( m_pShinIppan01EX2 ) {
			m_pShinIppan01EX2->Init();
			m_pShinIppan01EX2->WriteData();
		}
		//<-----
		if (m_pShinIppan02) {
			m_pShinIppan02->Init();
			m_pShinIppan02->WriteData();
		}
		if (m_pShinKani01) {
			m_pShinKani01->Init();
			m_pShinKani01->WriteData();		// '20.01.24 仕訳編集後、付表６の再集計がうまくいかないことがあったため　kasai
		}

		if( m_pShinKani01EX ) {
			m_pShinKani01EX->Init();
			m_pShinKani01EX->WriteData();
		}
		//----->('23.05.01 Add)
		if (m_pShinKani01EX2) {
			m_pShinKani01EX2->Init();
			m_pShinKani01EX2->WriteData();
		}
		//<-----

		if( m_pFhyo6 ){
			m_pFhyo6->Init();	// '20.01.24 対応漏れのため追加　kasai
		}

		if (m_pChskEX) {
			m_pChskEX->Init();
		}
		if (m_pHjnKskEX) {
			// '20.01.27　仕訳入力と同時に起動している状態で仕訳編集を行っても、本表の金額が反映されていなかったため修正 kasai
			if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
				CalqSyzForKsk(1);
			}
			m_pHjnKskEX->Init();
		}
		else if (m_pHjnKskEX2) {	// 更正の請求/新様式対応
			if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
				CalqSyzForKsk(1);
			}
			m_pHjnKskEX2->Init();
		}
		if (m_pKjnKskEX) {
			// '20.01.27　仕訳入力と同時に起動している状態で仕訳編集を行っても、本表の金額が反映されていなかったため修正 kasai
			if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
				CalqSyzForKsk(1);
			}
			m_pKjnKskEX->Init();
		}
		else if (m_pKjnKskEX2) {	// 更正の請求/新様式対応
			if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
				CalqSyzForKsk(1);
			}
			m_pKjnKskEX2->Init();
		}

		if ( m_pRefundKjn02 && m_pSnFhyo2_01Data ) {
			if ((m_pSnHeadData->m_DispTabSgn & 0xc0) == 0xc0)	// 特例の金額を集計
				m_pSnRefundData->ConnectSkjZeigaku( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
			m_pRefundKjn02->calq_by_OtherChange ( 3, m_pSnFhyo2_01Data, m_ShRevType );
		}

		if ( m_pRefundKjn02EX && m_pSnFhyo2_01Data ) {
			m_pRefundKjn02EX->calq_by_OtherChange ( 3, m_pSnFhyo2_01Data, m_ShRevType );
		}


		//---->yoshida190821
		//特例の計算値が本表に反映されていない。
		//場所移動
		/*
		if ( m_pKsisanuri ) {
			m_pKsisanuri->Init();
		}

		if ( m_pKsisanuri2 ) {
			m_pKsisanuri2->Init();
		}

		if ( m_pKshiire ) {
			m_pKshiire->Init();
		}
		*/
		//<-----

		PostMessage ( WM_MYMESSAGE, 0, 0 );
	}

	return 0;
}


void CH31HyoView::ReDispHonpyoUpper()
{
	if ( m_pShinIppan01 ) {
		m_pShinIppan01->DispIppanUpper();
	}

	if( m_pShinIppan01EX ) {
		m_pShinIppan01EX->DispIppanUpper();
	}
	//----->('23.05.01 Add)
	if( m_pShinIppan01EX2 ) {
		m_pShinIppan01EX2->DispIppanUpper();
	}
	//<-----
	if ( m_pShinIppan02 ) {
		m_pShinIppan02->DispIppan2Upper();
	}

	if ( m_pShinKani01 ) {
		m_pShinKani01->DispInitKaniUpper();
	}

	if( m_pShinKani01EX ) {
		m_pShinKani01EX->DispInitKaniUpper();
	}
	//----->('23.05.01 Add)
	if (m_pShinKani01EX2) {
		m_pShinKani01EX2->DispInitKaniUpper();
	}
	//<-----
	if( m_pHjnKskEX ){
		m_pHjnKskEX->DispKsiHjnDigZi();
	}
	else if( m_pHjnKskEX2 ){	// 更正の請求/新様式対応
		m_pHjnKskEX2->DispKsiHjnDigZi();
	}

	if( m_pKjnKskEX ){
		m_pKjnKskEX->DispKsiKjnDigZi();
	}
	else if( m_pKjnKskEX2 ){	// 更正の請求/新様式対応
		m_pKjnKskEX2->DispKsiKjnDigZi();
	}

	if ( m_pChskEX ) {
		m_pChskEX->InitDisp();
	}

	PostMessage ( WM_MYMESSAGE, 0, 0 );

	return;
}


//-----------------------------------------------------------------------------
// 現在の入力位置にフォーカスセット
//-----------------------------------------------------------------------------
void CH31HyoView::SetNowPosition()
{

	SetItemPosition(m_MainTab.GetCurSel());
	
	return;
}


//-----------------------------------------------------------------------------
// 過年度プロ呼出し後にデータを吸い上げる
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH31HyoView::GetPastData()
{
	// ヘッダー情報
	if ( m_pSnHeadData ) {
		m_pSnHeadData->GetData ( m_pZmSub, m_SnSeq );
		// パートナーズデータ取込業務で旧税率が無かった場合も、この処理が原因で旧税率ありになっていた。
		// 旧プロからの引継ぎはもう必要のない処理なのでカットする。
		//m_pSnHeadData->SVmzsw |= 0x01;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );
	if ( KzHoushiki == ID_ICSSH_GENSOKU ) {	// 原則課税
		// 付表２－２
		if ( m_pSnFhyo2_02Data ) {
			if ( m_pSnFhyo2_02Data->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pFhyo2_02 ) {
			m_pFhyo2_02->Init();
		}

		// 付表２－１
		if ( m_pSnFhyo2_01Data) {
			if ( m_pSnFhyo2_01Data->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pFhyo2_01 ) {
			m_pFhyo2_01->Init();
		}

		// 付表１－２
		if ( m_pSnFhyo1_02Data ) {
			if ( m_pSnFhyo1_02Data->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pFhyo1_02 ) {
			m_pFhyo1_02->Init();
		}

		// 一般本表
		if ( m_pSnHonpyoData ) {
			if ( m_pSnHonpyoData->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pShinIppan01 ) {
			m_pShinIppan01->Init();
		}

		if( m_pShinIppan01EX ) {
			m_pShinIppan01EX->Init();
		}
		//----->('23.05.01 Add)
		if( m_pShinIppan01EX2 ) {
			m_pShinIppan01EX2->Init();
		}
		//<-----
	}
	else {	// 簡易課税
		// 付表５－２（１）
		if ( m_pSnFhyo5_02Data ) {
			if ( m_pSnFhyo5_02Data->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pFhyo5_02 ) {
			m_pFhyo5_02->Init();
		}

		// 付表５－２（２）
		if ( m_pSnFhyo5_022Data ) {
			if ( m_pSnFhyo5_022Data->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pFhyo5_022 ) {
			m_pFhyo5_022->Init();
		}

		// 付表４－２
		if ( m_pSnFhyo4_02Data ) {
			if ( m_pSnFhyo4_02Data->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pFhyo4_02 ) {
			m_pFhyo4_02->Init();
		}

		// 簡易本表
		if ( m_pSnHonpyoData ) {
			if ( m_pSnHonpyoData->GetPastData ( m_pZmSub, m_SnSeq, m_pSnHeadData ) == -1 ) {
				return -1;
			}
		}
		if ( m_pShinKani01 ) {
			m_pShinKani01->Init();
		}

		if( m_pShinKani01EX ) {
			m_pShinKani01EX->Init();
		}
		//----->('23.05.01 Add)
		if (m_pShinKani01EX2) {
			m_pShinKani01EX2->Init();
		}
		//<-----
	}

	// コンバージョンサインON
	m_pSnHeadData->Sn_GenericSgn |= 0x01;

	return 0;
}

//==============================================
// 旧データ読込み必要性のチェック
//----------------------------------------------
// 返送値	
//			TRUE	：	必要性あり
//			FALSE	：	必要性無し
//==============================================
BOOL CH31HyoView::IsNeedOldDataCnv()
{
/*	if ( m_pZmSub ) {
		if ( m_pZmSub->szsnk == NULL ) {
			if ( m_pZmSub->SzsnkOpen() ) {
				return FALSE;
			}
		}
		else {
			m_pZmSub->szsnk->Requery ( _T("Sn_SEQ = 0") );
		}
	}
	if ( m_pZmSub->szsnk->st == -1 ) {
		return FALSE;
	}

	CDBSyzAccess	DBSyzAccess;
	if ( DBSyzAccess.OpenShLstVal ( m_pZmSub->m_database, _T( "version = 2" ) ) ) {
		return FALSE;
	}
	int st = DBSyzAccess.m_pSH_LST_VAL->st;

	DBSyzAccess.CloseShLstVal();

	if ( st == -1 ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
*/

	if ( m_pSnHeadData->Sn_GenericSgn & 0x01 ) {
		// コンバージョン済み
		return FALSE;
	}
	else {
		return TRUE;
	}
}


//-----------------------------------------------------------------------------
// 履歴シーケンス切替
//-----------------------------------------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ChangeSnSeq( int snseq )
{
	if ( m_SvSnSeq == snseq ) {
		return 0;
	}

	// 既存データの書込み
	if ( WriteData ( TRUE, FALSE ) ) {
		return -1;
	}

	// 新履歴番号セット
	SetSnSeq ( snseq );

	// 新規データの取得
	ReGetData();

	return 0;
}


//-----------------------------------------------------------------------------
// 現申告データを消去して、データの再読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ReReadData()
{
	BOOL isTransitionalMeasure = FALSE;
	if ( m_pSnHeadData->SVmzsw == 1 ) {
		isTransitionalMeasure = TRUE;
	}

	if ( m_pSnHonpyoData ) {
		delete m_pSnHonpyoData;
		m_pSnHonpyoData = NULL;

		if ( ( m_pSnHonpyoData = new CH31SnHonpyoData() ) == NULL ) {
			return -1;
		}
		if ( m_pSnHonpyoData->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnHonpyo2Data ) {
		delete m_pSnHonpyo2Data;
		m_pSnHonpyo2Data = NULL;

		if ( ( m_pSnHonpyo2Data = new CH31SnHonpyo2Data() ) == NULL ) {
			return -1;
		}
		if ( m_pSnHonpyo2Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo1_01Data ) {
		delete m_pSnFhyo1_01Data;
		m_pSnFhyo1_01Data = NULL;

		if ( ( m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data ( isTransitionalMeasure) ) == NULL ) {
			return -1;
		}
		if ( m_pSnFhyo1_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo1_02Data ) {
		delete m_pSnFhyo1_02Data;
			m_pSnFhyo1_02Data = NULL;

			if ( ( m_pSnFhyo1_02Data = new CH31SnFhyo1_02Data ( isTransitionalMeasure) ) == NULL) {
				return -1;
			}
		if ( m_pSnFhyo1_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo2_01Data ) {
		delete m_pSnFhyo2_01Data;
			m_pSnFhyo2_01Data = NULL;

			if ( ( m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data ( isTransitionalMeasure) ) == NULL) {
				return -1;
			}
		if ( m_pSnFhyo2_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo2_02Data ) {
		delete m_pSnFhyo2_02Data;
		m_pSnFhyo2_02Data = NULL;

		if ( ( m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data ( isTransitionalMeasure) ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo2_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo4_01Data ) {
		delete m_pSnFhyo4_01Data;
		m_pSnFhyo4_01Data = NULL;

		if ( ( m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data ( isTransitionalMeasure) ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo4_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo4_02Data ) {
		delete m_pSnFhyo4_02Data;
		m_pSnFhyo4_02Data = NULL;

		if ( ( m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data ( isTransitionalMeasure) ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo4_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo5_01Data ) {
		delete m_pSnFhyo5_01Data;
		m_pSnFhyo5_01Data = NULL;

		if ( ( m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data ( isTransitionalMeasure) ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo5_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo5_012Data ) {
		delete m_pSnFhyo5_012Data;
		m_pSnFhyo5_012Data = NULL;

		if ( ( m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data ( isTransitionalMeasure) ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo5_012Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo5_02Data ) {
		delete m_pSnFhyo5_02Data;
		m_pSnFhyo5_02Data = NULL;

		if ( ( m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data ( isTransitionalMeasure) ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo5_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo5_022Data ) {
		delete m_pSnFhyo5_022Data;
		m_pSnFhyo5_022Data = NULL;

		if ( ( m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data ( isTransitionalMeasure) ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo5_022Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnFhyo6Data ) {
		delete m_pSnFhyo6Data;
		m_pSnFhyo6Data = NULL;

		if ( ( m_pSnFhyo6Data = new CH31SnFhyo6Data() ) == NULL) {
			return -1;
		}
		if ( m_pSnFhyo6Data->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}
	if (m_pSnFhyo6_01Data) {
		delete m_pSnFhyo6_01Data;
		m_pSnFhyo6_01Data = NULL;

		if ((m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(isTransitionalMeasure)) == NULL) {
			return -1;
		}
		if (m_pSnFhyo6_01Data->GetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}
	}

	if ( m_pSnRefundData ) {
		delete m_pSnRefundData;
		m_pSnRefundData = NULL;

		if ( ( m_pSnRefundData = new CH31SnRefundData() ) == NULL) {
			return -1;
		}
		if ( m_pSnRefundData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}
		ResetRefundData();
	}

	if ( m_pSnChskData ) {
		delete m_pSnChskData;
		m_pSnChskData = NULL;

		if ( ( m_pSnChskData = new CH31SnChskData() ) == NULL) {
			return -1;
		}
		if ( m_pSnChskData->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}
	}

	if ( m_pSnKskData ) {
		delete m_pSnKskData;
		m_pSnKskData = NULL;

		if ( ( m_pSnKskData = new CH31SnKskData() ) == NULL) {
			return -1;
		}
		if ( m_pSnKskData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}
	}

	if( m_pSnTanaData ){
		delete m_pSnTanaData;
		m_pSnTanaData = NULL;

		if( ( m_pSnTanaData = new CH31SnTanaData( isTransitionalMeasure ) ) == NULL ){
			return -1;
		}
		if( m_pSnTanaData->GetData ( m_pZmSub, m_SnSeq ) ){
			
		}
	}

	if( m_pSnFhyo6_01Data ){
		delete m_pSnFhyo6_01Data;
		m_pSnFhyo6_01Data = NULL;

		if( ( m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData( isTransitionalMeasure ) ) == NULL ){
			return -1;
		}
		if( m_pSnFhyo6_01Data->GetData ( m_pZmSub, m_SnSeq ) ){
			
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 手入力サインのクリア
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH31HyoView::ClearManualInputSign( BOOL isChgKzritu )
{
	if ( m_pSnFhyo1_01Data ) {
		m_pSnFhyo1_01Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo1_02Data ) {
		m_pSnFhyo1_02Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo2_01Data ) {
		m_pSnFhyo2_01Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo2_02Data ) {
		m_pSnFhyo2_02Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo4_01Data ) {
		m_pSnFhyo4_01Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo4_02Data ) {
		m_pSnFhyo4_02Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo5_01Data ) {
		m_pSnFhyo5_01Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo5_012Data ) {
		m_pSnFhyo5_012Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo5_02Data ) {
		m_pSnFhyo5_02Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo5_022Data ) {
		m_pSnFhyo5_022Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnFhyo6_01Data ) {
		m_pSnFhyo6_01Data->ClearManualInputSign ( isChgKzritu );
	}

	if ( m_pSnHeadData->IsSoleProprietor() ) {
		if ( m_pRefundKjn02 ){
			m_pRefundKjn02->refundvol.KPSstx &= ~0x01;
		}

		//----> 20230704
		if ( m_pRefundKjn02EX ){
			m_pRefundKjn02EX->refundvol.KPSstx &= ~0x01;
		}
		//<---- 20230704
	}
	else {
		if ( m_pRefundHjn02 ) {
			m_pRefundHjn02->refundvol.KPSstx &= ~0x01;
		}

		//----> 20230704
		if ( m_pRefundHjn02EX ) {
			m_pRefundHjn02EX->refundvol.KPSstx &= ~0x01;
		}
		//<---- 20230704
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 付表5-1・付表5-2　手入力サインのクリア '20.01.29
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH31HyoView::ClearManualInputSign_Fhyo5()
{
	if( m_pSnFhyo5_01Data ){
		m_pSnFhyo5_01Data->Sn_F501_6sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_14sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_15sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_16sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_17sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_18sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_19sw = 0;
	}

	if( m_pSnFhyo5_02Data ){
		m_pSnFhyo5_02Data->Sn_F502_6sw &= ~0x800;
		m_pSnFhyo5_02Data->Sn_F502_14sw &= ~0x800;
		m_pSnFhyo5_02Data->Sn_F502_15sw &= ~0x800;
		m_pSnFhyo5_02Data->Sn_F502_16sw &= ~0x800;
		m_pSnFhyo5_02Data->Sn_F502_17sw &= ~0x800;
		m_pSnFhyo5_02Data->Sn_F502_18sw &= ~0x800;
		m_pSnFhyo5_02Data->Sn_F502_19sw &= ~0x800;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 次回申告データ作成に必要になるデータの読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ReadDataForNextShinkoku()
{

	return 0;
}


//-----------------------------------------------------------------------------
// ページ切替
//-----------------------------------------------------------------------------
// 引数	move	：	1:順送り、-1:逆送り
//-----------------------------------------------------------------------------
void CH31HyoView::PageChange(int move)
{
	int selno = m_MainTab.GetCurSel();

	// 移動後のタブ
	int movesel;
	if( move == 1 ) {
		if( selno == (m_MainTab.GetItemCount() - 1) )
			movesel = 0;
		else
			movesel = selno + move;
	}
	else if( move == -1 ) {
		if( selno == 0 )
			movesel = (m_MainTab.GetItemCount() - 1);
		else
			movesel = selno + move;
	}
	else return;

	// 各表の更新処理後の再表示
	m_MainTab.SetCurSel( movesel );

	if( m_TabMng.GetCount() ){
		if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB ){
			if( IsExist_MyNumber_Fuhyo6()&&!m_NoHistory_browse){
				WriteNoHistory(0);
				m_NoHistory_browse = TRUE;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// 金額取得
//-----------------------------------------------------------------------------
// 引数	tag		：
//		chtag	：
//		pArith	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CH31HyoView::GetMony( CString &tag, CString &chtag, char *pArith )
{
	ASSERT(pArith);
	if (pArith == NULL) {
		return -1;
	}

	char flg = 0x00;
	
	if( (tag == _T("AAF00000")) || (tag == _T("ABI00180")) || (tag == _T("AAF00010")) || (tag == _T("AAF00020")) || (tag == _T("AAF00030")) ||	// 本表
		(tag == _T("AAJ00110")) || (tag == _T("AAM00030")) || (tag == _T("ABI00010")) || (tag == _T("ABI00170")) ||
		( tag == _T( "FFF00000" ))){
		if (m_pSnHonpyoData == NULL) {
			if ((m_pSnHonpyoData = new CH31SnHonpyoData()) == NULL) {
				return -1;
			}
			if (m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}

			flg |= 0x01;
		}
	}

	else if ( tag == _T("Sn_KTNFZ") ) {	// 予定申告
		if ( m_pSnChskData == NULL ) {
			if ( (m_pSnChskData = new CH31SnChskData()) == NULL ) {
				return -1;
			}
			if ( m_pSnChskData->GetData( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}

			flg |= 0x02;
		}
	}
	else if ((tag == _T("CRB00020")) || (tag == _T("CRB00030")) || (tag == _T("CRB00040")) || (tag == _T("CRB00050")) || (tag == _T("CRB00060")) ||
			(tag == _T("CRE00030")) || (tag == _T("CRE00040")) || (tag == _T("CRE00080")) || (tag == _T("CRE00090")) ||	// 付表2-1　課税売上額
		    (tag == _T("AEB00030")) || (tag == _T("AEB00040")) || (tag == _T("AEB00045")) || (tag == _T("AEB00050"))) {	// 付表2-2　課税売上額

		if (m_pSnHeadData->SVmzsw) {
			if (m_pSnFhyo2_02Data == NULL) {
				if ((m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo2_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}

				flg |= 0x04;
			}

			if (m_pSnFhyo2_01Data == NULL) {
				if ((m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data(FALSE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo2_01Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}

				flg |= 0x08;
			}
		}
		else {
			if (m_pSnFhyo2_01Data == NULL) {
				if ((m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data(FALSE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo2_01Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}

				flg |= 0x08;
			}
		}
	}
	else if ( ( tag == _T( "BCD00060" ) ) || ( tag == _T( "BCD00110" ) ) || ( tag == _T( "BCD00170" ) ) || ( tag == _T( "BCD00230" ) ) || ( tag == _T( "BCD00290" ) ) || ( tag == _T( "BCD00350" ) ) || ( tag == _T( "BCD00720" ) ) ||
			  ( tag == _T( "CTD00060" ) ) || ( tag == _T( "CTD00120" ) ) || ( tag == _T( "CTD00190" ) ) || ( tag == _T( "CTD00260" ) ) || ( tag == _T( "CTD00330" ) ) || ( tag == _T( "CTD00400" ) ) || ( tag == _T( "CTD00470" ) ) ) { 
		if ( m_pSnHeadData->SVmzsw ) {
			if ( m_pSnFhyo5_02Data == NULL ) {
				if ( ( m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data ( TRUE ) ) == NULL ) {
					return -1;
				}

				if ( m_pSnFhyo5_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
					return -1;
				}

				flg |= 0x10;
			}
		}
		else {
			if ( m_pSnFhyo5_01Data == NULL ) {
				if ( ( m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data ( TRUE ) ) == NULL ) {
					return -1;
				}

				if ( m_pSnFhyo5_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
					return -1;
				}

				flg |= 0x20;
			}
		}
	}

	// 本表
	if ((tag == _T("AAM00030")) || (tag == _T("ABI00180"))) {	// 基準期間の課税売上高
		if ( m_pSnHonpyoData ) {
			memmove(pArith, m_pSnHonpyoData->Sn_ZZURI, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AAF00010")) {	// 前年の課税売上高
		if ( m_pSnHonpyoData ) {
			memmove(pArith, m_pSnHonpyoData->Sn_ZURI, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AAF00020")) {	// 前々回丈夫税額
		if ( m_pSnHonpyoData ) {
			memmove(pArith, m_pSnHonpyoData->Sn_ZZNOFZ, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AAF00030")) {	// 前回納付税額
		if ( m_pSnHonpyoData ) {
			memmove(pArith, m_pSnHonpyoData->Sn_ZNOFZ, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AAJ00110") || (tag == _T("ABI00110"))) {	// 中間納付税額
		if (m_pSnHonpyoData) {
			memmove(pArith, m_pSnHonpyoData->Sn_TYNOFZ, MONY_BUF_SIZE);
		}
	}
	//else if (tag == _T("Sn_TTYWAR")) {	// 中間納付譲渡割額
	else if (tag == _T("AAK00070") || (tag == _T("ABJ00070"))) {	// 中間納付税額（地方消費税）
		if (m_pSnHonpyoData) {
			memmove(pArith, m_pSnHonpyoData->Sn_TTYWAR, MONY_BUF_SIZE);
		}
	}
	else if( tag == _T("AAK00130") ){	// 消費税及び地方消費税の合計(納付又は還付)税額
		if( m_pSnHonpyoData ) {
			memmove(pArith, m_pSnHonpyoData->Sn_SYOTSG, MONY_BUF_SIZE);
		}
	}

	//yoshida190730
	else if (tag == _T("AAJ00010") || tag == _T("ABI00010")) {	// 課税標準額
		if (m_pSnHonpyoData) {
			memmove(pArith, m_pSnHonpyoData->Sn_KSTD, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("ABI00170")) {	// この課税期間の課税売上高
		if (m_pSnHonpyoData) {
			memmove(pArith, m_pSnHonpyoData->Sn_KZURI, MONY_BUF_SIZE);
		}
	}
	else if( tag == _T( "FFF00000" )) {
		// この課税期間における課税売上高
		if( m_pSnHonpyoData ) {
			memmove( pArith, m_pSnHonpyoData->Sn_KazeiUriage, MONY_BUF_SIZE );
		}
	}


	// 予定申告
	else if (tag == _T("Sn_KTNFZ")) {
		if ( m_pSnChskData ) {
			memmove(pArith, m_pSnChskData->Sn_KTNFZ, MONY_BUF_SIZE);
		}
	}

	// 付表2-1
	else if (tag == _T("CRB00020")) {
		if ( m_pSnFhyo2_01Data ) {
			memmove(pArith, m_pSnFhyo2_01Data->Sn_F201_1X, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRB00030")) {
		if ( m_pSnFhyo2_01Data ) {
			memmove(pArith, m_pSnFhyo2_01Data->Sn_F201_1D, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRB00040")) {
		if ( m_pSnFhyo2_01Data ) {
			memmove(pArith, m_pSnFhyo2_01Data->Sn_F201_1E, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRB00050")) {
		if ( m_pSnFhyo2_01Data ) {
			memmove(pArith, m_pSnFhyo2_01Data->Sn_F201_1F, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRB00060")) {   // 免税売上高 /* 2020.02.18. Furukawa */
		if ( m_pSnFhyo2_01Data ) {
			memmove(pArith, m_pSnFhyo2_01Data->Sn_F201_2F, MONY_BUF_SIZE);
		}
	}
	// 付表2-2
	else if (tag == _T("AEB00030")) {
		if ( m_pSnFhyo2_02Data ) {
			memmove(pArith, m_pSnFhyo2_02Data->Sn_F202_1A, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AEB00040")) {
		if ( m_pSnFhyo2_02Data ) {
			memmove(pArith, m_pSnFhyo2_02Data->Sn_F202_1B, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AEB00045")) {
		if ( m_pSnFhyo2_02Data ) {
			memmove(pArith, m_pSnFhyo2_02Data->Sn_F202_1C, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AEB00050")) {
		if ( m_pSnFhyo2_02Data ) {
			memmove(pArith, m_pSnFhyo2_02Data->Sn_F202_1X, MONY_BUF_SIZE);
		}
	}
	// 付表5-2
	else if ( tag == _T( "BCD00060" ) ) {
		if ( m_pSnFhyo5_02Data ) {
			memmove ( pArith, m_pSnFhyo5_02Data->Sn_F502_6X, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "BCD00110" ) ) {
		if ( m_pSnFhyo5_02Data ) {
			memmove ( pArith, m_pSnFhyo5_02Data->Sn_F502_7X, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "BCD00170" ) ) {
		if ( m_pSnFhyo5_02Data ) {
			memmove ( pArith, m_pSnFhyo5_02Data->Sn_F502_8X, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "BCD00230" ) ) {
		if ( m_pSnFhyo5_02Data ) {
			memmove ( pArith, m_pSnFhyo5_02Data->Sn_F502_9X, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "BCD00290" ) ) {
		if ( m_pSnFhyo5_02Data ) {
			memmove ( pArith, m_pSnFhyo5_02Data->Sn_F502_10X, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "BCD00350" ) ) {
		if ( m_pSnFhyo5_02Data ) {
			memmove ( pArith, m_pSnFhyo5_02Data->Sn_F502_11X, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "BCD00720" ) ) {
		if ( m_pSnFhyo5_02Data ) {
			memmove ( pArith, m_pSnFhyo5_02Data->Sn_F502_12C, MONY_BUF_SIZE );
		}
	}
	// 付表5-1
	else if ( tag == _T( "CTD00060" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			memmove ( pArith, m_pSnFhyo5_01Data->Sn_F501_6F, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "CTD00120" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			memmove ( pArith, m_pSnFhyo5_01Data->Sn_F501_7F, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "CTD00190" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			memmove ( pArith, m_pSnFhyo5_01Data->Sn_F501_8F, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "CTD00260" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			memmove ( pArith, m_pSnFhyo5_01Data->Sn_F501_9F, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "CTD00330" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			memmove ( pArith, m_pSnFhyo5_01Data->Sn_F501_10F, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "CTD00400" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			memmove ( pArith, m_pSnFhyo5_01Data->Sn_F501_11F, MONY_BUF_SIZE );
		}
	}
	else if ( tag == _T( "CTD00470" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			memmove ( pArith, m_pSnFhyo5_01Data->Sn_F501_12F, MONY_BUF_SIZE );
		}
	}

	// 当関数でメモリを確保し、確保したメモリを破棄していないと
	// 非連動時にデータベースに0が書き込まれていた
	if ( flg & 0x01 ) {
		if ( m_pSnHonpyoData ) {
			delete m_pSnHonpyoData;
			m_pSnHonpyoData = NULL;
		}
	}

	if ( flg & 0x02 ) {
		if ( m_pSnChskData ) {
			delete m_pSnChskData;
			m_pSnChskData = NULL;
		}
	}

	if ( flg & 0x04 ) {
		if ( m_pSnFhyo2_02Data ) {
			delete m_pSnFhyo2_02Data;
			m_pSnFhyo2_02Data = NULL;
		}
	}

	if ( flg & 0x08 ) {
		if ( m_pSnFhyo2_01Data ) {
			delete m_pSnFhyo2_01Data;
			m_pSnFhyo2_01Data = NULL;
		}
	}

	if ( flg & 0x10 ) {
		if ( m_pSnFhyo5_02Data ) {
			delete m_pSnFhyo5_02Data;
			m_pSnFhyo5_02Data = NULL;
		}
	}

	if ( flg & 0x20 ) {
		if ( m_pSnFhyo5_01Data ) {
			delete m_pSnFhyo5_01Data;
			m_pSnFhyo5_01Data = NULL;
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 金額セット
//-----------------------------------------------------------------------------
// 引数	tag		：
//		chtag	：
//		pArith	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CH31HyoView::SetMony(CString &tag, CString &chtag, char *pArith)
{

	if ((tag == _T("AAM00030")) || (tag == _T("ABI00180")) || (tag == _T("AAF00010")) || (tag == _T("AAF00020")) || (tag == _T("AAF00030")) ||	// 本表
	    (tag == _T("AAJ00110")) || (tag == _T("ABI00110")) || (tag == _T("AAK00070")) || (tag == _T("ABJ00070")) ||	// 22-0985  中間納付税額・中間納付譲渡割額が漏れていたため追加
		(tag == _T("FFF00000")) || (tag == _T("ABI00170")) ){	// 本表
		if (m_pSnHonpyoData == NULL) {
			if ((m_pSnHonpyoData = new CH31SnHonpyoData()) == NULL) {
				return -1;
			}
			if (m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}		
		}
	}

	else if (tag == _T("Sn_KTNFZ")) {	// 予定申告
		if (m_pSnChskData == NULL) {
			if ((m_pSnChskData = new CH31SnChskData()) == NULL) {
				return -1;
			}
			if (m_pSnChskData->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}
	}
	else if ((tag == _T("CRB00020")) || (tag == _T("CRB00030")) || (tag == _T("CRB00040")) || (tag == _T("CRB00050")) || 	// 付表2-1
		(tag == _T("CRE00360")) || (tag == _T("CRE00370")) || (tag == _T("CRE00410")) || (tag == _T("CRE00420")) ||	//('23.09.14 Tanaka Add)
		(tag == _T("CRI00030")) || (tag == _T("CRI00040")) || (tag == _T("CRI00080")) || (tag == _T("CRI00090")) ||	// 付表2-1 ( 特定収入計算表対応 )
		(tag == _T("AEB00030")) || (tag == _T("AEB00040")) || (tag == _T("AEB00045")) || (tag == _T("AEB00050")) || (tag == _T("AEC00035")) || (tag == _T("AEC00075"))) {	// 付表2-2
		if (m_pSnHeadData->SVmzsw) {
			if (m_pSnFhyo2_02Data == NULL) {
				if ((m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo2_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
		}
		else {
			if (m_pSnFhyo2_01Data == NULL) {
				if ((m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo2_01Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
		}
	}
	
	else if ((tag == _T("CQC00040")) || (tag == _T("CQD00030")) || (tag == _T("CQF00090")) || (tag == _T("CQC00030")) ||	// 付表1-1 課税標準額C・消費税額C・返還等対価に係る税額C・課税標準額B・
		(tag == _T("CQD00020")) || (tag == _T("CQF00080")) ||																//		   消費税額B・返還等対価に係る税額B・課税資産の譲渡等対価の額C
		(tag == _T("CQF00030")) || (tag == _T("CQF00040")) ||																// 付表1-1 ( 特定収入計算表対応 )
		(tag == _T("CQF00140")) || (tag == _T("CQF00150")) ||  
		(tag == _T("BAB00030")) || (tag == _T("BAC00030")) || (tag == _T("BAE00090")) || (tag == _T("BAB00020")) ||	// 付表1-2 
		(tag == _T("BAC00020")) || (tag == _T("BAE00080")) || (tag == _T("BAM00040")) || (tag == _T("BAE00250")) ){


		if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {	// 原則課税
			if (m_pSnFhyo1_01Data == NULL) {
				if ((m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo1_01Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
			if (m_pSnFhyo1_02Data == NULL) {
				if ((m_pSnFhyo1_02Data = new CH31SnFhyo1_02Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo1_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
		}
	}

	else if ((tag == _T("CTD00100")) || (tag == _T("CTD00170")) || (tag == _T("CTD00240")) || (tag == _T("CTD00310")) || (tag == _T("CTD00380")) || (tag == _T("CTD00450")) ||	// 付表5-1
			 (tag == _T("CTD00090")) || (tag == _T("CTD00160")) || (tag == _T("CTD00230")) || (tag == _T("CTD00300")) || (tag == _T("CTD00370")) || (tag == _T("CTD00440")) ||

			 (tag == _T("BCD00100")) || (tag == _T("BCD00160")) || (tag == _T("BCD00220")) || (tag == _T("BCD00280")) || (tag == _T("BCD00340")) || (tag == _T("BCD00710")) ||// 付表5-2
			 (tag == _T("BCD00090")) || (tag == _T("BCD00150")) || (tag == _T("BCD00210")) || (tag == _T("BCD00270")) || (tag == _T("BCD00330")) || (tag == _T("BCD00700"))) {


		if (m_pSnFhyo5_01Data == NULL) {
			if ((m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}
		if (m_pSnFhyo5_02Data == NULL) {
			if ((m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo5_02Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}
	}

	//yoshida190730
	else if ((tag == _T("CSC00020")) || (tag == _T("CSC00030")) || (tag == _T("CSD00020")) || (tag == _T("CSD00030")) || (tag == _T("CSF00080")) || (tag == _T("CSF00090")) ||// 付表4-1
		  (tag == _T("BBL00030")) || (tag == _T("BBC00030")) || (tag == _T("BBE00090")) ) {	// 付表4-2
		if (m_pSnHeadData->SVmzsw) {
			if (m_pSnFhyo4_02Data == NULL) {
				if ((m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo4_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
		}
		else {
			if (m_pSnFhyo4_01Data == NULL) {
				if ((m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
		}
	}

	// 付表6（2割特例）
	else if( (tag == _T("AYB00020")) || (tag == _T("AYB00030")) || (tag == _T("AYB00100")) || (tag == _T("AYB00110")) || (tag == _T("AYB00180")) || (tag == _T("AYB00190")) ){
		if( m_pSnFhyo6_01Data == NULL ){
			if( (m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(TRUE)) == NULL ){
				return -1;
			}
			if( m_pSnFhyo6_01Data->GetData( m_pZmSub, m_SnSeq ) ){
				return -1;
			}
		}
	}


	// 本表
	if ((tag == _T("AAM00030")) || (tag == _T("ABI00180"))) {	// 原則・簡易
		if (m_pSnHonpyoData) {
			memmove(m_pSnHonpyoData->Sn_ZZURI, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AAF00010")) {
		if (m_pSnHonpyoData) {
			memmove(m_pSnHonpyoData->Sn_ZURI, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AAF00020")) {
		if (m_pSnHonpyoData) {
			memmove(m_pSnHonpyoData->Sn_ZZNOFZ, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AAF00030")) {
		if (m_pSnHonpyoData) {
			memmove(m_pSnHonpyoData->Sn_ZNOFZ, pArith, MONY_BUF_SIZE);
			m_pSnHonpyoData->Sn_ZNOsw = atoi(chtag);//19.11.07
		}
	}
	else if ((tag == _T("AAJ00110")) || (tag == _T("ABI00110"))) {	// 中間納付税額
		if (m_pSnHonpyoData) {
			memmove(m_pSnHonpyoData->Sn_TYNOFZ, pArith, MONY_BUF_SIZE);
		}
	}
	else if ((tag == _T("AAK00070")) || (tag == _T("ABJ00070"))) {	// 中間納付譲渡割額（地方消費税）
		if (m_pSnHonpyoData) {
			memmove(m_pSnHonpyoData->Sn_TTYWAR, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("ABI00170")) {	// この課税期間の課税売上高
		if (m_pSnHonpyoData) {
			memmove(m_pSnHonpyoData->Sn_KZURI, pArith, MONY_BUF_SIZE);
		}
	}
	else if( tag == _T( "FFF00000" ) ) {
		// この課税期間における課税売上高
		if( m_pSnHonpyoData ) {
			memmove( m_pSnHonpyoData->Sn_KazeiUriage, pArith, MONY_BUF_SIZE );
		}
	}

	// 予定申告
	else if (tag == _T("Sn_KTNFZ")) {
		if (m_pSnChskData) {
			memmove(m_pSnChskData->Sn_KTNFZ, pArith, MONY_BUF_SIZE);
		}
	}

	// 付表２
	else if (tag == _T("CRB00020")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_1X, pArith, MONY_BUF_SIZE);
		}
	}		
	else if (tag == _T("CRB00030")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_1D, pArith, MONY_BUF_SIZE);
		}
	}	
	else if (tag == _T("CRB00040")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_1E, pArith, MONY_BUF_SIZE);
		}
	}	
	else if (tag == _T("CRB00050")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_1F, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRI00030")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_23D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRI00040")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_23E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRI00080")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_24D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CRI00090")) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_24E, pArith, MONY_BUF_SIZE);
		}
	}
	else if ( tag == _T( "CRE00030" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_9D, pArith, MONY_BUF_SIZE);
		}
	}
	else if ( tag == _T( "CRE00040" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_9E, pArith, MONY_BUF_SIZE);
		}
	}
	else if ( tag == _T( "CRE00080" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_10D, pArith, MONY_BUF_SIZE);
			if ( m_Arith.l_test ( m_pSnFhyo2_01Data->Sn_F201_10D ) ) {
				m_pSnFhyo2_01Data->Sn_F201_10sw |= 0x80;
			}
		}
	}
	else if ( tag == _T( "CRE00090" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_10E, pArith, MONY_BUF_SIZE);
			if ( m_Arith.l_test ( m_pSnFhyo2_01Data->Sn_F201_10E ) ) {
				m_pSnFhyo2_01Data->Sn_F201_10sw |= 0x800;
			}
		}
	}
//----->('23.09.14 Tanaka cor タグ名を正式なものに変更)
	// インボイス_23/07/31_higuchi add -->
//	else if ( tag == _T( "CRE2" ) ) {
	else if ( tag == _T( "CRE00360" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_27D, pArith, MONY_BUF_SIZE);
		}
	}
//	else if ( tag == _T( "CRE3" ) ) {
	else if ( tag == _T( "CRE00370" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_27E, pArith, MONY_BUF_SIZE);
		}
	}
//	else if ( tag == _T( "CRE6" ) ) {
	else if ( tag == _T( "CRE00410" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_28D, pArith, MONY_BUF_SIZE);
			if ( m_Arith.l_test ( m_pSnFhyo2_01Data->Sn_F201_28D ) ) {
				m_pSnFhyo2_01Data->Sn_F201_28sw |= 0x80;
			}
		}
	}
//	else if ( tag == _T( "CRE7" ) ) {
	else if ( tag == _T( "CRE00420" ) ) {
		if (m_pSnFhyo2_01Data) {
			memmove(m_pSnFhyo2_01Data->Sn_F201_28E, pArith, MONY_BUF_SIZE);
			if ( m_Arith.l_test ( m_pSnFhyo2_01Data->Sn_F201_28E ) ) {
				m_pSnFhyo2_01Data->Sn_F201_28sw |= 0x800;
			}
		}
	}
	// インボイス_23/07/31_higuchi add <--
//<-----('23.09.14 Tanaka cor タグ名を正式なものに変更)
	else if (tag == _T("AEB00030")) {
		if (m_pSnFhyo2_02Data) {
			memmove(m_pSnFhyo2_02Data->Sn_F202_1A, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AEB00040")) {
		if (m_pSnFhyo2_02Data) {
			memmove(m_pSnFhyo2_02Data->Sn_F202_1B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AEB00045")) {
		if (m_pSnFhyo2_02Data) {
			memmove(m_pSnFhyo2_02Data->Sn_F202_1C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("AEB00050")) {
		if (m_pSnFhyo2_02Data) {
			memmove(m_pSnFhyo2_02Data->Sn_F202_1X, pArith, MONY_BUF_SIZE);
		}
	}
	else if ( tag == _T( "AEC00035" ) ) {
		if (m_pSnFhyo2_02Data) {
			memmove(m_pSnFhyo2_02Data->Sn_F202_9C, pArith, MONY_BUF_SIZE);
		}
	}
	else if ( tag == _T( "AEC00075" ) ) {
		if (m_pSnFhyo2_02Data) {
			memmove(m_pSnFhyo2_02Data->Sn_F202_10C, pArith, MONY_BUF_SIZE);
			if ( m_Arith.l_test ( m_pSnFhyo2_02Data->Sn_F202_10C ) ) {
				m_pSnFhyo2_02Data->Sn_F202_10sw |= 0x800;
			}
		}
	}
	// 付表１
	else if (tag == _T("CQC00040")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_1_1E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CQD00030")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_2E, pArith, MONY_BUF_SIZE);
			m_pSnFhyo1_01Data->Sn_F101_2c &= ~0x800;//金額転記後は手入力サインOFFに
		}
	}
	else if (tag == _T("CQF00090")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_5E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CQC00030")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_1_1D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CQD00020")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_2D, pArith, MONY_BUF_SIZE);
			m_pSnFhyo1_01Data->Sn_F101_2c &= ~0x80;//金額転記後は手入力サインOFFに

		}
	}
	else if (tag == _T("CQF00080")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_5D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CQF00030")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_4D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CQF00040")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_4E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CQF00140")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_5_1D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CQF00150")) {
		if (m_pSnFhyo1_01Data) {
			memmove(m_pSnFhyo1_01Data->Sn_F101_5_1E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BAB00030")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_1C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BAC00030")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_2C, pArith, MONY_BUF_SIZE);
			m_pSnFhyo1_02Data->Sn_F102_2c &= ~0x800;//金額転記後は手入力サインOFFに
		}
	}
	else if (tag == _T("BAE00090")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_5C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BAB00020")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_1B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BAC00020")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_2B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BAE00080")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_5B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BAM00040")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_1_1C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BAE00250")) {
		if (m_pSnFhyo1_02Data) {
			memmove(m_pSnFhyo1_02Data->Sn_F102_5_1C, pArith, MONY_BUF_SIZE);
		}
	}
	// 付表５－１
	else if (tag == _T("CTD00100")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_7E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00170")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_8E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00240")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_9E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00310")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_10E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00380")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_11E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00450")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_12E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00090")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_7D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00160")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_8D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00230")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_9D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00300")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_10D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00370")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_11D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CTD00440")) {
		if (m_pSnFhyo5_01Data) {
			memmove(m_pSnFhyo5_01Data->Sn_F501_12D, pArith, MONY_BUF_SIZE);
		}
	}

	// 付表５－２
	else if (tag == _T("BCD00100")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_7C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00160")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_8C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00220")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_9C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00280")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_10C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00340")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_11C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00710")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_12C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00090")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_7B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00150")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_8B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00210")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_9B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00270")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_10B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00330")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_11B, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BCD00700")) {
		if (m_pSnFhyo5_02Data) {
			memmove(m_pSnFhyo5_02Data->Sn_F502_12B, pArith, MONY_BUF_SIZE);
		}
	}

	//yoshida190730
	//付表４－１
	else if (tag == _T("CSC00020")) {
		if (m_pSnFhyo4_01Data) {
			memmove(m_pSnFhyo4_01Data->Sn_F401_1_1D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CSC00030")) {
		if (m_pSnFhyo4_01Data) {
			memmove(m_pSnFhyo4_01Data->Sn_F401_1_1E, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CSD00020")) {
		if (m_pSnFhyo4_01Data) {
			memmove(m_pSnFhyo4_01Data->Sn_F401_2D, pArith, MONY_BUF_SIZE);
			m_pSnFhyo4_01Data->Sn_F401_2sw &= ~0x80;//金額転記後は手入力サインOFFに

		}
	}
	else if (tag == _T("CSD00030")) {
		if (m_pSnFhyo4_01Data) {
			memmove(m_pSnFhyo4_01Data->Sn_F401_2E, pArith, MONY_BUF_SIZE);
			m_pSnFhyo4_01Data->Sn_F401_2sw &= ~0x800;//金額転記後は手入力サインOFFに
		}
	}
	else if (tag == _T("CSF00080")) {
		if (m_pSnFhyo4_01Data) {
			memmove(m_pSnFhyo4_01Data->Sn_F401_5D, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("CSF00090")) {
		if (m_pSnFhyo4_01Data) {
			memmove(m_pSnFhyo4_01Data->Sn_F401_5E, pArith, MONY_BUF_SIZE);
		}
	}

	//付表４－２
	else if (tag == _T("BBL00030")) {
		if (m_pSnFhyo4_02Data) {
			memmove(m_pSnFhyo4_02Data->Sn_F402_1_1C, pArith, MONY_BUF_SIZE);
		}
	}
	else if (tag == _T("BBC00030")) {
		if (m_pSnFhyo4_02Data) {
			memmove(m_pSnFhyo4_02Data->Sn_F402_2C, pArith, MONY_BUF_SIZE);
			m_pSnFhyo4_02Data->Sn_F402_2sw &= ~0x800;//金額転記後は手入力サインOFFに
		}
	}
	else if (tag == _T("BBE00090")) {
		if (m_pSnFhyo4_02Data) {
			memmove(m_pSnFhyo4_02Data->Sn_F402_5C, pArith, MONY_BUF_SIZE);
		}
	}

	// 付表6（2割特例）
	else if( tag == _T("AYB00020") ){
		if( m_pSnFhyo6_01Data ){
			memmove( m_pSnFhyo6_01Data->Sn_F601_1A, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00030") ){
		if( m_pSnFhyo6_01Data ){
			memmove( m_pSnFhyo6_01Data->Sn_F601_1B, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00100") ){
		if( m_pSnFhyo6_01Data ){
			memmove( m_pSnFhyo6_01Data->Sn_F601_3A, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00110") ){
		if( m_pSnFhyo6_01Data ){
			memmove( m_pSnFhyo6_01Data->Sn_F601_3B, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00180") ){
		if( m_pSnFhyo6_01Data ){
			memmove( m_pSnFhyo6_01Data->Sn_F601_5A, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00190") ){
		if( m_pSnFhyo6_01Data ){
			memmove( m_pSnFhyo6_01Data->Sn_F601_5B, pArith, MONY_BUF_SIZE );
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 金額の再取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ReGetCalqedMony()
{
	if (m_pSnHeadData->IsReadCalqedData() == FALSE) {
		return 0;
	}

	GetCalqedMony();

	// メモリ再確保
	ReMemory();

	// 表示更新

	// 20230616
	// 原則で2割経過(非連動)→通常経過(連動)→2割経過(非連動)などすると、クラス破棄する前にここを通ってしまい
	// 付表4・5で手入力した金額がMemoryRegetで上書きされてしまうため、原則・簡易で区切る
	if( m_Util.GetKazeihoushiki( m_pZmSub->zvol ) == ID_ICSSH_KANNI ){

		if ( m_pSnHeadData->SVmzsw == 1 ) {
			if (m_pFhyo4_01) {
				m_pFhyo4_01->Init();
			}
			if (m_pFhyo4_02) {
				m_pFhyo4_02->Init();
			}
			if (m_pFhyo5_01) {
				m_pFhyo5_01->Init();
			}
			if (m_pFhyo5_012) {
				m_pFhyo5_012->Init();
			}
			if (m_pFhyo5_02) {
				m_pFhyo5_02->Init();
			}
			if (m_pFhyo5_022) {
				m_pFhyo5_022->Init();
			}
		}
		else {
			if (m_pFhyo4_01) {
				m_pFhyo4_01->Init();
			}
			if (m_pFhyo5_01) {
				m_pFhyo5_01->Init();
			}
			if (m_pFhyo5_012) {
				m_pFhyo5_012->Init();
			}

			if ( m_pFhyo4_03 ) {
				m_pFhyo4_03->Init();
			}
			if ( m_pFhyo5_03 ) {
				m_pFhyo5_03->Init();
			}
			if ( m_pFhyo5_032 ) {
				m_pFhyo5_032->Init();
			}
			//----->('23.05.01 Add)
			if ( m_pFhyo6_01 ) {
				m_pFhyo6_01->Init();
			}
			//<-----
		}
		if (m_pShinKani01) {
			m_pShinKani01->Init();
		}

		if( m_pShinKani01EX ) {
			m_pShinKani01EX->Init();
		}
		//----->('23.05.01 Add)
		if (m_pShinKani01EX2) {
			m_pShinKani01EX2->Init();
		}
		//<-----
	}
	else{
		if ( m_pSnHeadData->SVmzsw == 1 ) {

			//---->20230616
			if( m_pSnHeadData->m_DispTabSgn & 0x2000 ){
				if (m_pFhyo4_01) {
					m_pFhyo4_01->Init();
				}
				if (m_pFhyo4_02) {
					m_pFhyo4_02->Init();
				}
				if (m_pFhyo5_01) {
					m_pFhyo5_01->Init();
				}
				if (m_pFhyo5_012) {
					m_pFhyo5_012->Init();
				}
				if (m_pFhyo5_02) {
					m_pFhyo5_02->Init();
				}
				if (m_pFhyo5_022) {
					m_pFhyo5_022->Init();
				}
			}
			//<----20230616
			else{
				if (m_pFhyo1_01) {
					m_pFhyo1_01->Init();
				}
				if (m_pFhyo1_02) {
					m_pFhyo1_02->Init();
				}

				//---->20230120
				if ( m_pFhyo2_11Ex ) {
					m_pFhyo2_11Ex->Init();
				}
				if ( m_pFhyo2_12Ex ) {
					m_pFhyo2_12Ex->Init();
				}
				//<----20230120
				if ( m_pFhyo2_11 ) {
					m_pFhyo2_11->Init();
				}
				if ( m_pFhyo2_12 ) {
					m_pFhyo2_12->Init();
				}
				if (m_pFhyo2_01) {
					m_pFhyo2_01->Init();
				}
				if (m_pFhyo2_02) {
					m_pFhyo2_02->Init();
				}
			}
		}
		else {
			if ( m_pFhyo1_01 ) {
				m_pFhyo1_01->Init();
			}
			if ( m_pFhyo1_03 ) {
				m_pFhyo1_03->Init();
			}
		
			if ( m_pFhyo2_01 ) {
				m_pFhyo2_01->Init();
			}
			if ( m_pFhyo2_11 ) {
				m_pFhyo2_11->Init();
			}
			//---->20230120
			if ( m_pFhyo2_11Ex ) {
				m_pFhyo2_11Ex->Init();
			}
			//<----20230120
			if ( m_pFhyo2_03 ) {
				m_pFhyo2_03->Init();
			}
			//---->20230120
			if ( m_pFhyo2_03Ex ) {
				m_pFhyo2_03Ex->Init();
			}
			//<----20230120
			//----->('23.05.01 Add)
			if ( m_pFhyo6_01 ) {
				m_pFhyo6_01->Init();
			}
			//<-----
		}
		if (m_pShinIppan01) {
			m_pShinIppan01->Init();
		}

		if( m_pShinIppan01EX ) {
			m_pShinIppan01EX->Init();
		}
		//----->('23.05.01 Add)
		if( m_pShinIppan01EX2 ) {
			m_pShinIppan01EX2->Init();
		}
		//<-----
		if (m_pShinIppan02) {
			m_pShinIppan02->Init();
		}
	}

	// 還付
	if (m_pRefundKjn01) {
		m_pRefundKjn01->ReUpdateData();
	}
	if (m_pRefundKjn02) {
		m_pRefundKjn02->ReUpdateData();
	}
	//----> 20230704
	if (m_pRefundKjn02EX) {
		m_pRefundKjn02EX->ReUpdateData();
	}
	//<---- 20230704
	if (m_pRefundHjn01) {
		m_pRefundHjn01->ReUpdateData();
	}
	if (m_pRefundHjn02) {
		m_pRefundHjn02->ReUpdateData();
	}
	//----> 20230704
	if (m_pRefundHjn02EX) {
		m_pRefundHjn02EX->ReUpdateData();
	}
	//<---- 20230704
	
	if (m_pFhyo6) {
		m_pFhyo6->Init();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 仕入控除税額のみの財務連動
//-----------------------------------------------------------------------------
// 引数	isUpdDisp		：	画面の更新をするかどうか
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ConnectedSkjZeigaku( bool isUpdDisp )
{
	if( m_pSnRefundData ){
//		m_pSnRefundData->GetData( m_pZmSub, m_pSnHeadData, m_SnSeq );
		m_pSnRefundData->ConnectSkjZeigaku( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
//		m_pSnRefundData->SetData( m_pZmSub, m_pSnHeadData, m_SnSeq );

		if( isUpdDisp ){
			if( m_pRefundKjn02 ){
				m_pRefundKjn02->ReUpdateData();
			}
			//----> 20230704
			if( m_pRefundKjn02EX ){
				m_pRefundKjn02EX->ReUpdateData();
			}
			//<---- 20230704
			if( m_pRefundHjn02 ){
				m_pRefundHjn02->ReUpdateData();
			}
			//----> 20230704
			if( m_pRefundHjn02EX ){
				m_pRefundHjn02EX->ReUpdateData();
			}
			//<---- 20230704
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 仕入控除税額の財務連動
//-----------------------------------------------------------------------------
// 引数	MoneyBaseArray	：	集計済 試算表データ
//		isUpdDisp		：	画面の更新をするかどうか
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ConnectedSkj( MoneyBasejagArray &MoneyBaseArray, bool isUpdDisp )
{

	if ( m_pSnRefundData ) {

		m_pSnRefundData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq );
		//m_pSnRefundData->ConnectZmdata();

		//-->yoshida190627
		m_pSnRefundData->ConnectZmdata(m_pZmSub, m_pSnHeadData, m_pSyzSyukei, m_pTblhdl, m_pSkjCnvtbl, m_ShRevType, MoneyBaseArray);
		//<------

		m_pSnRefundData->SetData ( m_pZmSub, m_pSnHeadData, m_SnSeq );

		if( isUpdDisp ){
			if( m_pRefundKjn01 ) {
				m_pRefundKjn01->ReUpdateData();
			}
			if( m_pRefundKjn02 ) {
				m_pRefundKjn02->ReUpdateData();
			}
			//----> 20230704
			if( m_pRefundKjn02EX ) {
				m_pRefundKjn02EX->ReUpdateData();
			}
			//<---- 20230704
			if( m_pRefundHjn01 ) {
				m_pRefundHjn01->ReUpdateData();
			}
			if( m_pRefundHjn02 ) {
				m_pRefundHjn02->ReUpdateData();
			}
			//----> 20230704
			if( m_pRefundHjn02EX ) {
				m_pRefundHjn02EX->ReUpdateData();
			}
			//<---- 20230704
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 仕入金額等の明細のクリア
//-----------------------------------------------------------------------------
// 引数			：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ClearShiire()
{
	if ( m_pSnRefundData ) {
		m_pSnRefundData->ClearData_Shiire ( m_pZmSub, m_pSnHeadData, m_SnSeq );
	}

	return 0;
}


//----------------------------------------------------------------------------
// 更正の請求用　申告書金額計算
//----------------------------------------------------------------------------
// 引数	mode	：
//----------------------------------------------------------------------------
// 返送値
//----------------------------------------------------------------------------
int CH31HyoView::CalqSyzForKsk( int mode )
{
	char val[MONY_BUF_SIZE] = { 0 };

	if ( ReadDataForCalq() ) {
		return -1;
	}

	SH_ANS ANS[30] = { 0 };
	SH_ANS JANS[30] = { 0 };
	CalqSyzForKskSub ( ( void* )ANS, ( void* )JANS, val );

	if ( !m_pSnKskData ) {
		m_pSnKskData = new CH31SnKskData();
		if ( m_pSnKskData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}
	}

	// 更正の請求/新様式対応
	long eymd = 0;
	if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if( eymd >= KSKMODIFY ){
		m_pSnKskData->SetCalqedData2 ( mode, ANS );
	}
	else{
		m_pSnKskData->SetCalqedData ( mode, ANS );
	}

	return 0;
}

//----------------------------------------------------------------------------
// 更正の請求用　更正→修正に進んだ場合の既確定税額のセット
//----------------------------------------------------------------------------
// 引数	mode	：
//----------------------------------------------------------------------------
// 返送値
//----------------------------------------------------------------------------
int CH31HyoView::CalqSyzFromKsk()
{
	if ( !m_pSnKskData ) {
		m_pSnKskData = new CH31SnKskData();
		if ( m_pSnKskData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}
	}

	CArithEx arith( 0x16 );
	KSIVAL *pKval1 = NULL, *pKval2 = NULL;

	// 既確定税額
	pKval1 = &m_pSnKskData->KsData.KSval[10];		// 納付税額
	if( arith.l_test( pKval1->KVkval ) > 0 ){
		memmove( m_pSnHonpyoData->Sn_KAKTIZ, pKval1->KVkval, sizeof(m_pSnHonpyoData->Sn_KAKTIZ) );
	}
	else{
		pKval1 = &m_pSnKskData->KsData.KSval[7];	// 控除不足還付税額
		pKval2 = &m_pSnKskData->KsData.KSval[11];	// 中間納付還付税額
		arith.l_add( m_pSnHonpyoData->Sn_KAKTIZ, pKval1->KVkval, pKval2->KVkval );
		arith.l_neg( m_pSnHonpyoData->Sn_KAKTIZ );
	}

	// 既確定譲渡割額
	pKval1 = &m_pSnKskData->KsData.KSval[17];		// 納付譲渡割額
	if( arith.l_test( pKval1->KVkval ) > 0 ){
		memmove( m_pSnHonpyoData->Sn_TKAKTZ, pKval1->KVkval, sizeof(m_pSnHonpyoData->Sn_TKAKTZ) );
	}
	else{
		pKval1 = &m_pSnKskData->KsData.KSval[14];	// 譲渡割額還付額
		pKval2 = &m_pSnKskData->KsData.KSval[18];	// 中間納付還付譲渡割額
		arith.l_add( m_pSnHonpyoData->Sn_TKAKTZ, pKval1->KVkval, pKval2->KVkval );
		arith.l_neg( m_pSnHonpyoData->Sn_TKAKTZ );
	}

	return 0;
}


//-----------------------------------------------------------------------------
// タブタイトルの切替
//-----------------------------------------------------------------------------
// 引数	type	：	
//-----------------------------------------------------------------------------
void CH31HyoView::ChengTabTitle( char type )
{
	CString title;
	if ( type & 0x04 ) {
		title = _T( "更正の申出" );
	}
	else {
		title = _T( "更正の請求" );
	}

	if ( m_pSnHeadData->IsSoleProprietor() ) {
		title += _T( "(個人)" );
	}
	else {
		title += _T( "(法人)" );
	}

	int max = ( int )m_TabMng.GetCount();
	for ( int i = 0; i < max; i++ ) {
		if ( ( m_TabMng[i].tabtype == ID_ICSSH_KSK_HJ_TAB ) || ( m_TabMng[i].tabtype == ID_ICSSH_KSK_KJ_TAB ) ) {
			m_MainTab.SetCaption ( i, title );
		}
	}
}


//-----------------------------------------------------------------------------
// 中間及び期間短縮の更新
//-----------------------------------------------------------------------------
// 引数	pPastInfo	：	以前の情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::RenewSyzShin( SH_PASTINFO *pPastInfo )
{
	if (m_pSnChskData == NULL) {
		if ((m_pSnChskData = new CH31SnChskData()) == NULL) {
			return -1;
		}
		if (m_pSnChskData->GetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}
	}

	if (m_pSnHonpyoData == NULL) {
		if ((m_pSnHonpyoData = new CH31SnHonpyoData()) == NULL) {
			return -1;
		}
		if (m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}
	}

	CString	szFilter;
	szFilter.Format(_T("Sn_SEQ=%d"), m_SnSeq);
	if (m_pZmSub->szsnk) {
		m_pZmSub->szsnk->Requery(szFilter);
	}
	else {
		m_pZmSub->SzsnkOpen(szFilter);
	}

	ASSERT(pPastInfo);
	if (pPastInfo == NULL) {
		return -1;
	}
	ASSERT(m_pSnHeadData);
	if (m_pSnHeadData == NULL) {
		return -1;
	}
	ASSERT(m_pSnHonpyoData);
	if (m_pSnHonpyoData == NULL) {
		return -1;
	}

	char	MYmds[4] = { 0 };
	char	MYmde[4] = { 0 };
	char	Date[4] = { 0 };

	int Msno = 0;
	int Meno = 0;
	int McNt = 0;
	char	SKKBN = pPastInfo->SKKBN;
	char	MCOUNT = pPastInfo->MCOUNT;

	switch (m_pSnHeadData->Sn_MCOUNT) {
	case 3:
		McNt = 3;
		break;
	case 11:
		McNt = 11;
		break;
	default:
		McNt = 1;
		break;
	}

	//確定申告
	if ((m_pSnHeadData->Sn_SKKBN % 2)) {
		m_Arith.l_add(m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnHonpyoData->Sn_KZURI);
		if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
			/*あとで対応
			// 非課税輸出売上を課税売上より差引く
			if (m_pSnFhyo20Data) {
				m_Arith.l_sub(m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo20Data->Sn_2F3C);
			}
			else if (m_pSnFhyo22Data) {
				m_Arith.l_sub(m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo22Data->Sn_2F3C);
			}
			*/
		}
	}

	// 前回中間申告  中間納付 <-- 最終納付
	if (!(SKKBN % 2)) {
		// 課税売上高を累計
		m_Arith.l_add(m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnHonpyoData->Sn_KZURI);
		if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
			// 非課税輸出売上を課税売上より差引く
			/*あとで対応
			if (m_pSnFhyo20Data) {
				m_Arith.l_sub(m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo20Data->Sn_2F3C);
			}
			else if (m_pSnFhyo22Data) {
				m_Arith.l_sub(m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo22Data->Sn_2F3C);
			}
			*/
		}
	}

	m_Arith.l_clear(m_pSnHonpyoData->Sn_KSTDM);							//  課税標準額（自動車）
	m_Arith.l_clear(m_pSnHeadData->Sn_CK45);								//			〃			(4.5%)
	m_Arith.l_clear(m_pSnHeadData->Sn_CK60);								//			〃			( 6% )
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KSTD);							//  　　〃　　（その他）
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TIKAM);							//  対価の返還等をした金額（自動車）
	m_Arith.l_clear(m_pSnHeadData->Sn_CH45);								//				〃					(4.5%)
	m_Arith.l_clear(m_pSnHeadData->Sn_CH60);								//				〃					( 6% )
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TIKA);							//  　　　　　〃　　　　　（その他）
	m_Arith.l_clear(m_pSnHonpyoData->Sn_SYTXM);							//  消費税額（自動車）
	m_Arith.l_clear(m_pSnHeadData->Sn_CZ45);								//		　〃			(4.5%)
	m_Arith.l_clear(m_pSnHeadData->Sn_CZ60);								//		　〃			( 6% )
	m_Arith.l_clear(m_pSnHonpyoData->Sn_SYTX);							//  　　〃　（その他）
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KJADJZ);							//  控除過大調整税額
	m_Arith.l_clear(m_pSnHonpyoData->Sn_SIREZ);							//  控除対象仕入税額

	if (m_pSnTanaData) {
		m_pSnTanaData->Sn_MENZsw = 0x00;
		m_Arith.l_clear(m_pSnTanaData->Sn_MENZK);							//	免税 控除対象仕入額
		m_Arith.l_clear(m_pSnTanaData->Sn_MENZZ);							//	免税 控除対象仕入税額
		m_Arith.l_clear(m_pSnTanaData->Sn_MNK40);							//	免税 控除対象仕入額(5%分)
		m_Arith.l_clear(m_pSnTanaData->Sn_MNZ40);							//	免税 控除対象仕入税額(5%分)	
		m_Arith.l_clear(m_pSnTanaData->Sn_MNK63);							//
		m_Arith.l_clear(m_pSnTanaData->Sn_MNZ63);							//

		m_Arith.l_clear(m_pSnTanaData->Sn_MNK624);							//
		m_Arith.l_clear(m_pSnTanaData->Sn_MNZ624);							//
		m_Arith.l_clear(m_pSnTanaData->Sn_MNK78);							//
		m_Arith.l_clear(m_pSnTanaData->Sn_MNZ78);							//
	}
	m_Arith.l_clear(m_pSnHonpyoData->Sn_HKANZ);							//  返還等対価に係る税額
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KSITZ);							//  貸倒れに係る税額

	if (m_pSnHeadData->IsKakuteiShinkoku() && m_pSnHeadData->IsKikanTansyuku()) {
		m_Arith.l_clear(m_pSnHonpyoData->Sn_TYNOFZ);						//  中間納付税額
		m_Arith.l_clear(m_pSnHonpyoData->Sn_TTYWAR);						//  中間納付税額		(地方消費税用)
	}

	if ((((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)) && pPastInfo->MINASHI && ((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku()) ||
		((pPastInfo->SKKBN == ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku()) ||
		((pPastInfo->SKKBN == 4/*中間修正*/) && (m_pSnHeadData->GetKubun() == ID_ICSSH_TYUUKAN)) ||
		((pPastInfo->SKKBN == ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)))) {
		m_Arith.l_clear(m_pSnHonpyoData->Sn_KAKTIZ);						//  既確定税額
		m_Arith.l_clear(m_pSnHonpyoData->Sn_TKAKTZ);						//	地方消費税　既確定譲渡割額
	}

	// 前回確定申告
	if (SKKBN % 2) {
		m_Arith.l_clear(m_pSnHeadData->Sn_TYNF1);							//	中間納付税額　１回目
		m_Arith.l_clear(m_pSnHeadData->Sn_TYNF2);							//	中間納付税額　２回目
		m_Arith.l_clear(m_pSnHeadData->Sn_TYNF3);							//	中間納付税額　３回目
		m_Arith.l_clear(m_pSnHeadData->Sn_TYNF4);							//	中間納付税額　４回目
		m_Arith.l_clear(m_pSnHeadData->Sn_TTYNF1);							//	中間納付税額　１回目(地方消費税用)
		m_Arith.l_clear(m_pSnHeadData->Sn_TTYNF2);							//	中間納付税額　２回目(地方消費税用)
		m_Arith.l_clear(m_pSnHeadData->Sn_TTYNF3);							//	中間納付税額　３回目(地方消費税用)
		m_Arith.l_clear(m_pSnHeadData->Sn_TTYNF4);							//	中間納付税額　４回目(地方消費税用)

		for (int i = 0; i < 11; i++) {
			memset(m_pSnHeadData->Sn_TYNF[i], '\0', 6);
			memset(m_pSnHeadData->Sn_TTNF[i], '\0', 6);
		}
	}

	if (m_pSnChskData) {
		m_Arith.l_clear(m_pSnChskData->Sn_KTNFZ);							// 消費税及び地方消費税の合計納付税額
		m_pSnChskData->Sn_BSMON = 0x00;										// 月換算	分子
		m_pSnChskData->Sn_BBMON = 0x00;										// 〃　　　 分母
	}
	m_Arith.l_clear(m_pSnHonpyoData->Sn_EDNOFZ);							//  最終納付税額
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KZURI);								//  課税売上高
	m_Arith.l_clear(m_pSnHonpyoData->Sn_SOURI);								//  総売上高

	m_pSnHeadData->Sn_TOKUR = 0;											//	特例計算適用ＳＷ
	m_pSnHonpyoData->Sn_UP1 = 0;											//	第１種事業　売上割合
	m_pSnHonpyoData->Sn_UP2 = 0;											//	第２種事業　売上割合
	m_pSnHonpyoData->Sn_UP3 = 0;											//	第３種事業　売上割合
	m_pSnHonpyoData->Sn_UP4 = 0;											//	第４種事業　売上割合
	m_pSnHonpyoData->Sn_UP5 = 0;											//	第５種事業　売上割合
	m_pSnHonpyoData->Sn_UP6 = 0;											//	第６種事業　売上割合
	
	
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KURI6);								//	第６種事業　課税売上
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KSJT3);								//　課税資産の譲渡等の対価の額
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KSJT4);
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KSJT63);
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KSJTKEI);
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TKST63);							// 特定課税仕入に係る支払対価の額
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TKSTKEI);
	m_Arith.l_clear(m_pSnHonpyoData->Sn_URHKNZ);							// 売上の返還等対価に係る税額
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TKHKNZ);							// 特定課税仕入の返還等対価に係る税額
	

	m_pSnHeadData->Sn_TUMIAG = 0;											//	規則22条 1項適用ＳＷ
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KURI1);								//	第１種事業　課税売上
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KURI2);								//	第２種事業　課税売上
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KURI3);								//	第３種事業　課税売上
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KURI4);								//	第４種事業　課税売上
	m_Arith.l_clear(m_pSnHonpyoData->Sn_KURI5);								//	第５種事業　課税売上

	m_Arith.l_clear(m_pSnHeadData->Sn_KURIA);								//	事業別課税売上高合計
	m_Arith.l_clear(m_pSnHonpyoData->Sn_GENKAI);							//	限界控除税額
	m_Arith.l_clear(m_pSnHonpyoData->Sn_WGENKA);							//	限界控除税額(手入力額)

	m_Arith.l_clear(m_pSnHonpyoData->Sn_TEDNOF);							//	地方消費税　最終納付税額

	m_pSnHeadData->SVzchg = 0;												//	免税⇔課税切り替えサイン
	m_pSnHeadData->SVmzsw = 0;												//	複数税率処理ｓｗ
	m_Arith.l_clear(m_pSnHeadData->SVtval1);								//	調整課税仕入等に係る棚卸資産額
	m_Arith.l_clear(m_pSnHeadData->SVtval2);								//	調整課税仕入等に係る棚卸消費税額

	if (m_pZmSub->szsnk->st != -1) {
		m_pZmSub->szsnk->Edit();
		m_pZmSub->szsnk->Sn_KSTDM = _T("0");
		m_pZmSub->szsnk->Sn_CK45 = _T("0");
		m_pZmSub->szsnk->Sn_CK60 = _T("0");
		m_pZmSub->szsnk->Sn_KSTD = _T("0");
		m_pZmSub->szsnk->Sn_TIKAM = _T("0");
		m_pZmSub->szsnk->Sn_CH45 = _T("0");
		m_pZmSub->szsnk->Sn_CH60 = _T("0");
		m_pZmSub->szsnk->Sn_TIKA = _T("0");
		m_pZmSub->szsnk->Sn_SYTXM = _T("0");
		m_pZmSub->szsnk->Sn_CZ45 = _T("0");
		m_pZmSub->szsnk->Sn_CZ60 = _T("0");
		m_pZmSub->szsnk->Sn_SYTX = _T("0");
		m_pZmSub->szsnk->Sn_KJADJZ = _T("0");
		m_pZmSub->szsnk->Sn_SIREZ = _T("0");
		m_pZmSub->szsnk->Sn_MENZsw = 0;
		m_pZmSub->szsnk->Sn_MENZK = _T("0");
		m_pZmSub->szsnk->Sn_MENZZ = _T("0");
		m_pZmSub->szsnk->Sn_MNK40 = _T("0");
		m_pZmSub->szsnk->Sn_MNZ40 = _T("0");
		m_pZmSub->szsnk->Sn_HKANZ = _T("0");
		m_pZmSub->szsnk->Sn_KSITZ = _T("0");
		if (m_pSnHeadData->IsKakuteiShinkoku() && m_pSnHeadData->IsKikanTansyuku()) {
			m_pZmSub->szsnk->Sn_TYNOFZ = _T("0");
			m_pZmSub->szsnk->Sn_TTYWAR = _T("0");
		}

		if ((((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)) && pPastInfo->MINASHI && ((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku()) ||
			((pPastInfo->SKKBN == ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku()) ||
			((pPastInfo->SKKBN == 4/*中間修正*/) && (m_pSnHeadData->GetKubun() == ID_ICSSH_TYUUKAN)) ||
			((pPastInfo->SKKBN == ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI) || (m_pSnHeadData->GetKubun() == ID_ICSSH_JUNKAKUTEI)))) {

			m_pZmSub->szsnk->Sn_KAKTIZ = _T("0");
			m_pZmSub->szsnk->Sn_TKAKTZ = _T("0");
		}

		// 前回確定申告
		if (SKKBN % 2) {
			for (int i = 0; i < 11; i++) {
				m_pZmSub->szsnk->Sn_TYNF[i] = _T("0");
				m_pZmSub->szsnk->Sn_TTYNF[i] = _T("0");
			}
		}
		m_pZmSub->szsnk->Sn_KTNFZ = _T("0");
		m_pZmSub->szsnk->Sn_BSMON = 0;
		m_pZmSub->szsnk->Sn_BBMON = 0;
		m_pZmSub->szsnk->Sn_EDNOFZ = _T("0");
		m_pZmSub->szsnk->Sn_KZURI = _T("0");
		m_pZmSub->szsnk->Sn_SOURI = _T("0");

		m_pZmSub->szsnk->Sn_TOKUR = 0;
		m_pZmSub->szsnk->Sn_UP[0] = 0;
		m_pZmSub->szsnk->Sn_UP[1] = 0;
		m_pZmSub->szsnk->Sn_UP[2] = 0;
		m_pZmSub->szsnk->Sn_UP[3] = 0;
		m_pZmSub->szsnk->Sn_UP[4] = 0;
		m_pZmSub->szsnk->Sn_TUMIAG = 0;

		m_pZmSub->szsnk->Sn_KURI[0] = _T("0");
		m_pZmSub->szsnk->Sn_KURI[1] = _T("0");
		m_pZmSub->szsnk->Sn_KURI[2] = _T("0");
		m_pZmSub->szsnk->Sn_KURI[3] = _T("0");
		m_pZmSub->szsnk->Sn_KURI[4] = _T("0");
		m_pZmSub->szsnk->Sn_TEDNOF = _T("0");

		m_pZmSub->szsnk->Sn_1F1B = _T("0");
		m_pZmSub->szsnk->Sn_1F2A = _T("0");
		m_pZmSub->szsnk->Sn_1F4B = _T("0");
		m_pZmSub->szsnk->Sn_1F5A = _T("0");
		m_pZmSub->szsnk->Sn_1F7A = _T("0");
		m_pZmSub->szsnk->Sn_1F7B = _T("0");
		m_pZmSub->szsnk->Sn_1F8A = _T("0");
		m_pZmSub->szsnk->Sn_1F8B = _T("0");
		m_pZmSub->szsnk->Sn_1F8C = _T("0");
		m_pZmSub->szsnk->Sn_1F9A = _T("0");
		m_pZmSub->szsnk->Sn_1F9B = _T("0");
		m_pZmSub->szsnk->Sn_1F9C = _T("0");
		m_pZmSub->szsnk->Sn_1F10A = _T("0");
		m_pZmSub->szsnk->Sn_1F10B = _T("0");
		m_pZmSub->szsnk->Sn_1F12B = _T("0");
		m_pZmSub->szsnk->Sn_1F13B = _T("0");
		m_pZmSub->szsnk->Sn_1F14C = _T("0");

		m_pZmSub->szsnk->Sn_5FURIK = _T("0");
		m_pZmSub->szsnk->Sn_5FURIT = _T("0");
		m_pZmSub->szsnk->Sn_5FURI[0] = _T("0");
		m_pZmSub->szsnk->Sn_5FURI[1] = _T("0");
		m_pZmSub->szsnk->Sn_5FURI[2] = _T("0");
		m_pZmSub->szsnk->Sn_5FURI[3] = _T("0");
		m_pZmSub->szsnk->Sn_5FURI[4] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZTT = _T("0");
		m_pZmSub->szsnk->Sn_5FUZT[0] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZT[1] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZT[2] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZT[3] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZT[4] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZKT = _T("0");
		m_pZmSub->szsnk->Sn_5FUZK[0] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZK[1] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZK[2] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZK[3] = _T("0");
		m_pZmSub->szsnk->Sn_5FUZK[4] = _T("0");

		m_pZmSub->szsnk->Sn_5FSZTG = _T("0");
		m_pZmSub->szsnk->Sn_5FSZTT = _T("0");
		m_pZmSub->szsnk->Sn_5FSZKG = _T("0");
		m_pZmSub->szsnk->Sn_5FSZKT = _T("0");
		m_pZmSub->szsnk->Sn_5Fdm1 = _T("0");
		m_pZmSub->szsnk->Sn_5Fdm2 = _T("0");
		m_pZmSub->szsnk->Sn_5Fdm3 = _T("0");
		m_pZmSub->szsnk->Sn_5Fdm4 = _T("0");

		m_pZmSub->szsnk->Sn_5FSZsw = 0;
		m_pZmSub->szsnk->Sn_5FSBsw = 0;
		m_pZmSub->szsnk->Sn_5FCQsw = 0;
		m_pZmSub->szsnk->Sn_5FGKsw = 0;
		m_pZmSub->szsnk->Sn_2F21sw = 0;

		m_pZmSub->szsnk->Sn_5FUP[0] = 0;
		m_pZmSub->szsnk->Sn_5FUP[1] = 0;
		m_pZmSub->szsnk->Sn_5FUP[2] = 0;
		m_pZmSub->szsnk->Sn_5FUP[3] = 0;
		m_pZmSub->szsnk->Sn_5FUP[4] = 0;

		m_pZmSub->szsnk->Sn_2F17A = _T("0");
		m_pZmSub->szsnk->Sn_2F17B = _T("0");
		m_pZmSub->szsnk->Sn_2F18A = _T("0");
		m_pZmSub->szsnk->Sn_2F18B = _T("0");
		m_pZmSub->szsnk->Sn_2F21A = _T("0");
		m_pZmSub->szsnk->Sn_2F21B = _T("0");

		m_pZmSub->szsnk->Update();
	}

	//	付表　集計エリア
	if (m_pSnFhyo1_01Data) {													
		m_pSnFhyo1_01Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo1_02Data) {
		m_pSnFhyo1_02Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo2_01Data) {
		if( ((pPastInfo->SKKBN==ID_ICSSH_KAKUTEI)||(pPastInfo->SKKBN==ID_ICSSH_JUNKAKUTEI)) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){
			m_pSnFhyo2_01Data->ClearDataByRenew(m_pZmSub, 0, 0x01);
		}
		else if( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN_SYUUSEI) ){
			m_pSnFhyo2_01Data->ClearDataByRenew(m_pZmSub, 0, 0x01);
		}
		else{
			m_pSnFhyo2_01Data->ClearDataByRenew(m_pZmSub, 0, 0x00);
		}
	}
	if (m_pSnFhyo2_02Data) {
		//m_pSnFhyo2_02Data->ClearDataByRenew(m_pZmSub, 0);
		if( (( pPastInfo->SKKBN == ID_ICSSH_KAKUTEI )||( pPastInfo->SKKBN == ID_ICSSH_JUNKAKUTEI )) && ( m_pSnHeadData->GetKubun() == ID_ICSSH_KAKUTEI_SYUUSEI ) ) {
			m_pSnFhyo2_02Data->ClearDataByRenew( m_pZmSub, 0, 0x01 );
		}
		else if(( pPastInfo->SKKBN == ID_ICSSH_TYUUKAN ) && ( m_pSnHeadData->GetKubun() == ID_ICSSH_TYUUKAN_SYUUSEI )) {
			m_pSnFhyo2_02Data->ClearDataByRenew( m_pZmSub, 0, 0x01 );
		}
		else{
			m_pSnFhyo2_02Data->ClearDataByRenew( m_pZmSub, 0, 0x00 );
		}
	}
	if (m_pSnFhyo4_01Data) {
		m_pSnFhyo4_01Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo4_02Data) {
		m_pSnFhyo4_02Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo5_01Data) {
		m_pSnFhyo5_01Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo5_012Data) {
		m_pSnFhyo5_012Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo5_02Data) {
		m_pSnFhyo5_02Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo5_022Data) {
		m_pSnFhyo5_022Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if (m_pSnFhyo6Data) {
		//m_pSnFhyo6Data->ClearDataByRenew(m_pZmSub, 0);
	}
	if ( m_pSnFhyo6_01Data ) {
		m_pSnFhyo6_01Data->ClearDataByRenew(m_pZmSub, 0);
	}

	// 個別対応時の棚卸調整額
	if (m_pSnTanaData) {
		m_pSnTanaData->Sn_tansw = 0x00;
		for (int i = 0; i != 3; i++) {
			memset(m_pSnTanaData->Sn_mnk63[i], '\0', 6);
			memset(m_pSnTanaData->Sn_mnz63[i], '\0', 6);
			memset(m_pSnTanaData->Sn_mnk40[i], '\0', 6);
			memset(m_pSnTanaData->Sn_mnz40[i], '\0', 6);
			memset(m_pSnTanaData->Sn_menzk[i], '\0', 6);
			memset(m_pSnTanaData->Sn_menzz[i], '\0', 6);

			memset(m_pSnTanaData->Sn_mnk624[i], '\0', 6);
			memset(m_pSnTanaData->Sn_mnz624[i], '\0', 6);
			memset(m_pSnTanaData->Sn_mnk78[i], '\0', 6);
			memset(m_pSnTanaData->Sn_mnz78[i], '\0', 6);
		}
	}

	/*1・2の内訳3行目*/
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TS_TDM1);
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TS_SYT1);
	/*17・18の内訳3行目*/
	m_Arith.l_clear(m_pSnHonpyoData->Sn_TS_SYT2);


	// データ更新
	m_pSnHonpyoData->SetData(m_pZmSub, m_SnSeq);

	m_pSnChskData->SetData(m_pZmSub, m_SnSeq);


	// テーブルバージョンアップ
	if(m_pKsisanuri){
		m_pKsisanuri->ClearData(true);
		//m_pKsisanuri->WriteData(m_SnSeq, 2);	
		m_pKsisanuri->WriteData(m_SnSeq, TOKUREI_TBL_VERSION);	
	}
	if(m_pKsisanuri2){
		m_pKsisanuri2->ClearData(true);
		//m_pKsisanuri2->WriteData(m_SnSeq, 2);	
		m_pKsisanuri2->WriteData(m_SnSeq, TOKUREI_TBL_VERSION);	
	}
	if(m_pKshiire){
		m_pKshiire->ClearData();
		//m_pKshiire->WriteData(m_SnSeq, 2);
		m_pKshiire->WriteData(m_SnSeq, TOKUREI_TBL_VERSION);
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 本表の計算
//-----------------------------------------------------------------------------
// 引数	type	：	呼出年度種別
//-----------------------------------------------------------------------------
void CH31HyoView::AlreadyDecisionCalc( int type )
{
	if ( ReadDataForCalq() ) {
	}

	AlreadyDecisionCalc31();
}


//-----------------------------------------------------------------------------
// 棚卸調整額の連動
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::CalqStatckSyz()
{
	if ( m_pSnTanaData == NULL ) {
		if ( m_pSnHeadData->IsMiddleProvisional() == FALSE ) {
			EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );
			if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
				if ( ( m_pSnTanaData = new CH31SnTanaData ( m_pSnHeadData->SVmzsw ? TRUE : FALSE ) ) != NULL ) {
					if ( m_pSnTanaData->GetData ( m_pZmSub, m_SnSeq ) ) {
						return -1;
					}
				}
			}
		}
	}

	if (m_pSnTanaData) {
		
		if (m_Syokai_NukiKomi == TRUE) {
			m_pSnTanaData->ConnectZmdata(m_pZmSub, m_pSnHeadData, m_pSyzSyukei);
			m_Syokai_NukiKomi = FALSE;
		}
		else {
			unsigned char tmp_sgn = m_pSnHeadData->SVsign;
			m_pSnTanaData->ConnectZmdata(m_pZmSub, m_pSnHeadData, m_pSyzSyukei);
			m_pSnHeadData->SVsign = tmp_sgn;
		}

		return 0;	// 金額集計はTanaDataAddで行う
		
		if ((m_pSnTanaData->Sn_MENZsw & 0x10) && m_pSnHeadData->IsKobetuSiireAnbun()) {

			//付表２－２と付表２－１
			static	char	*H32_FHYO2_SIIRE_CODE[] = { _T("321701"), _T("321702"), _T("321703"), _T("321801"), _T("321802"), _T("321803"), _T("324202"), _T("324203"),  _T("321802"), _T("321803") };
			

			if (m_pSyzSyukei && (m_pSnTanaData->Sn_tansw & 0x01) == 0x00) {

				char	**SIIRE_CODE;

				SIIRE_CODE = H32_FHYO2_SIIRE_CODE;
				

				char	mony[6] = { 0 };

				//＝＝＝＝付表２－２＝＝＝＝＝//
				// 課税売上げに要する
				CString	tag, chtag;
				MoneyBasejagArray	shinkoku;
				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[0]);
				tag = _T("AEE00030");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[0], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[1]);
				tag = _T("AEE00040");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[1], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[2]);
				tag = _T("AEE00045");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[2], mony);

				// 共通に要する
				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[3]);
				tag = _T("AEE00070");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[3], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[4]);
				tag = _T("AEE00080");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[4], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[5]);
				tag = _T("AEE00085");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[5], mony);

				//＝＝＝＝付表２－１＝＝＝＝＝//

				// 課税売上げに要する
				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[6]);
				tag = _T("CRG00040");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[6], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[7]);
				tag = _T("CRG00050");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[7], mony);


				// 共通に要する
				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[8]);
				tag = _T("CRG00090");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[8], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[9]);
				tag = _T("CRG00100");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[9], mony);
			}

			//ここを全部３１の形にかえる
			/*
			static	char	*H28_FHYO2_SIIRE_CODE[] = { _T("221601"), _T("221602"), _T("221603"), _T("221701"), _T("221702"), _T("221703") };
			static	char	*H26_FHYO2_SIIRE_CODE[] = { _T("021401"), _T("021402"), _T("121403"), _T("021501"), _T("021502"), _T("121503") };

			if (m_pSyzSyukei && (m_pSnTanaData->Sn_tansw & 0x01) == 0x00) {

				char	**SIIRE_CODE;
				if (m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) {
					SIIRE_CODE = H28_FHYO2_SIIRE_CODE;
				}
				else {
					SIIRE_CODE = H26_FHYO2_SIIRE_CODE;
				}

				char	mony[6] = { 0 };
				// 課税売上げに要する
				CString	tag, chtag;
				MoneyBasejagArray	shinkoku;
				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[0]);
				tag = _T("AYB00040");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[0], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[1]);
				tag = _T("AYB00100");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[1], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[2]);
				tag = _T("AYB00160");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[2], mony);

				// 共通に要する
				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[3]);
				tag = _T("AYB00060");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[3], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[4]);
				tag = _T("AYB00120");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[4], mony);

				memset(mony, '\0', sizeof(mony));
				shinkoku = m_pSyzSyukei->GetShinkokuData(SIIRE_CODE[5]);
				tag = _T("AYB00180");
				GetMony(tag, chtag, mony);
				if (m_pSnTanaData->Sn_MENZsw & 0x01) {
					m_Arith.l_sub(mony, shinkoku[0][0].arith, mony);
				}
				else {
					m_Arith.l_add(mony, shinkoku[0][0].arith, mony);
				}
				m_pSyzSyukei->SetShinkokuData(SIIRE_CODE[5], mony);
			}
		*/	
		}
		
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 前年の消費税額取得（過年度マスターから）
//-----------------------------------------------------------------------------
// 引数	pBfNpSub	：	過年度マスターに接続済の非営利クラス
//		pArith		：	消費税額の保存先
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::GetLastYearTax( CDBNpSub *pBfNpSub, char *pArith )
{

	char	tmpMony[6] = { 0 };

	// 本表から計算 差引税額(⑨)
	//計算する必要あるか確認。

	if ( pBfNpSub->szsnk->Sn_KDAYE >= ICS_SH_10PER_REFORM_DAY ) {
		CH31SnHonpyoData *pZnSnHonpyoData = NULL;
		if ( ( pZnSnHonpyoData = new CH31SnHonpyoData() ) == NULL ) {
			return -1;
		}
		pZnSnHonpyoData->GetData ( pBfNpSub, 0 );

		memmove ( tmpMony, pZnSnHonpyoData->Sn_SHZ, MONY_BUF_SIZE );

		if ( pZnSnHonpyoData ) {
			delete pZnSnHonpyoData;
			pZnSnHonpyoData = NULL;
		}
	}
	else {
		CH28SnHonpyoData *pZnSnHonpyoData = NULL;
		if ( ( pZnSnHonpyoData = new CH28SnHonpyoData() ) == NULL ) {
			return -1;
		}
		pZnSnHonpyoData->GetData ( pBfNpSub, 0 );

		m_Arith.l_add ( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SYTX );	// ＋消費税額
		m_Arith.l_add ( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KJADJZ );	// ＋控除過大調整税額
		m_Arith.l_sub ( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SIREZ );	// －控除対象仕入税額
		m_Arith.l_sub ( tmpMony, tmpMony, pZnSnHonpyoData->Sn_HKANZ );	// －返還等対価に係る税額
		m_Arith.l_sub ( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KSITZ );	// －貸倒れに係る税額

		if ( pZnSnHonpyoData ) {
			delete pZnSnHonpyoData;
			pZnSnHonpyoData = NULL;
		}

	}

	if ( m_Arith.l_test ( tmpMony ) > 0 ) {
		m_Util.l_calq ( tmpMony, tmpMony, 0x10 );
	}
	memmove ( pArith, tmpMony, sizeof ( tmpMony ) );

	return 0;
}


//-----------------------------------------------------------------------------
// 集計済み金額の取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::GetCalqedMony()
{
	if (m_pSnHeadData) {
		if (m_pSnHeadData->IsReadCalqedData() == FALSE) {
			return 0;
		}
	}
	BOOL	isPluralZei = FALSE;

	if (m_pSnHeadData) {
		m_pSnHeadData->SetCalqedData(m_pSyzSyukei);
	}
	if (m_pSnTanaData) {
		m_pSnTanaData->SetCalqedData(m_pSyzSyukei);
		m_pSnTanaData->SetData(m_pZmSub, m_SnSeq);
	}

	//yoshida191001
	//再集計ボタンにて、中間納付税額等が消えてる。ここの処理必要と播野さんより聞き、処理復活
	if (m_pSnHonpyoData) {
//		m_pSnHonpyoData->SetCalqedData(m_pZmSub, m_pSyzSyukei);
		// 申告区分によるデータの再変更
		if (!(m_pSnHeadData->Sn_SKKBN % 2)) {
			m_Arith.l_clear(m_pSnHonpyoData->Sn_TYNOFZ);
			m_Arith.l_clear(m_pSnHonpyoData->Sn_TTYWAR);
		}
		if (m_pSnHeadData->Sn_SKKBN <= 2) {
			m_Arith.l_clear(m_pSnHonpyoData->Sn_KAKTIZ);
			m_Arith.l_clear(m_pSnHonpyoData->Sn_TKAKTZ);
		}
		m_pSnHonpyoData->SetData(m_pZmSub, m_SnSeq);
	}

	// 20230616
	// 原則で2割経過(非連動)→通常経過(連動)→2割経過(非連動)などすると、クラス破棄する前にここを通ってしまい
	// 付表4・5で手入力した金額が連動金額に上書きされてしまうため、原則・簡易で区切る
	if( m_Util.GetKazeihoushiki( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ){

		//----> 20230616
		if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
			if (m_pSnFhyo4_01Data) {
				m_pSnFhyo4_01Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo4_01Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo4_02Data) {
				m_pSnFhyo4_02Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo4_02Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo5_01Data) {
				m_pSnFhyo5_01Data->SetCalqedData(m_pSyzSyukei, isPluralZei, m_pSnHeadData);
				m_pSnFhyo5_01Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo5_012Data) {
				m_pSnFhyo5_012Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo5_012Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo5_02Data) {
				m_pSnFhyo5_02Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo5_02Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo5_022Data) {
				m_pSnFhyo5_022Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo5_022Data->SetData(m_pZmSub, m_SnSeq);
			}
		}
		//<---- 20230616
		else{
			if (m_pSnFhyo1_01Data) {
				m_pSnFhyo1_01Data->SetCalqed1_01Data(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo1_01Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo1_02Data) {
				m_pSnFhyo1_02Data->SetCalqed1_02Data(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo1_02Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo2_01Data) {
				m_pSnFhyo2_01Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo2_01Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo2_02Data) {
				m_pSnFhyo2_02Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo2_02Data->SetData(m_pZmSub, m_SnSeq);
			}

			if (m_pSnFhyo6_01Data) {
				m_pSnFhyo6_01Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
				m_pSnFhyo6_01Data->SetData(m_pZmSub, m_SnSeq);
			}
		}
	}
	else{
		if (m_pSnFhyo4_01Data) {
			m_pSnFhyo4_01Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
			m_pSnFhyo4_01Data->SetData(m_pZmSub, m_SnSeq);
		}

		if (m_pSnFhyo4_02Data) {
			m_pSnFhyo4_02Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
			m_pSnFhyo4_02Data->SetData(m_pZmSub, m_SnSeq);
		}

		if (m_pSnFhyo5_01Data) {
			m_pSnFhyo5_01Data->SetCalqedData(m_pSyzSyukei, isPluralZei, m_pSnHeadData);
			m_pSnFhyo5_01Data->SetData(m_pZmSub, m_SnSeq);
		}

		if (m_pSnFhyo5_012Data) {
			m_pSnFhyo5_012Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
			m_pSnFhyo5_012Data->SetData(m_pZmSub, m_SnSeq);
		}

		if (m_pSnFhyo5_02Data) {
			m_pSnFhyo5_02Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
			m_pSnFhyo5_02Data->SetData(m_pZmSub, m_SnSeq);
		}
		if (m_pSnFhyo5_022Data) {
			m_pSnFhyo5_022Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
			m_pSnFhyo5_022Data->SetData(m_pZmSub, m_SnSeq);
		}
		
		if (m_pSnFhyo6_01Data) {
			m_pSnFhyo6_01Data->SetCalqedData(m_pSyzSyukei, isPluralZei);
			m_pSnFhyo6_01Data->SetData(m_pZmSub, m_SnSeq);
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 既確定税額のクリア
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ClearAlreadyDecision()
{
	
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KAKTIZ );
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TKAKTZ );

	if (m_pZmSub) {
		CString filter;
		filter.Format("Sn_SEQ = %d", m_SnSeq);

		if (m_pZmSub->szsnk == NULL) {
			m_pZmSub->SzsnkOpen(filter);
		}
		else {
			m_pZmSub->szsnk->Requery(filter);
		}
		if ( m_pZmSub->szsnk->st != -1 ) {
			m_pZmSub->szsnk->Edit();
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_KAKTIZ, m_pSnHonpyoData->Sn_KAKTIZ );
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_TKAKTZ, m_pSnHonpyoData->Sn_TKAKTZ );
			m_pZmSub->szsnk->Update();
		}
	}
	

	return 0;
}


//-----------------------------------------------------------------------------
// 各表の更新処理
//-----------------------------------------------------------------------------
void CH31HyoView::UpdateSheetFromOuter()
{
	UpdateSheet ( m_MainTab.GetCurSel() );

	// 還付データ書き込み
	if ( m_pSnRefundData ) {
		m_pSnRefundData->SetData ( m_pZmSub, m_pSnHeadData, m_SnSeq );
	}
}


//-----------------------------------------------------------------------------
// 全表の更新処理
//-----------------------------------------------------------------------------
void CH31HyoView::UpdateAllSheet( int sgn )
{
	int cnt = (int)m_TabMng.GetCount();

	if (sgn == 0) {
		//UpdateSheet(m_MainTab.GetCurSel());//現在の表を先に更新しておく[S]
		//for (int i = 0; i < cnt; i++) {
		//	UpdateSheet(i, 0);
		//}
		UpdateSheet(0, 0);
	}
	else {
		//for (int i = 0; i < cnt; i++) {
		//	UpdateSheet(i, 1);
		//}
		if( m_pShinIppan01 ){
			m_pShinIppan01->ReadData2();
		}
		if( m_pShinIppan01EX ){
			m_pShinIppan01EX->ReadData2();
		}
		//----->('23.05.01 Add)
		if( m_pShinIppan01EX2 ) {
			m_pShinIppan01EX2->ReadData2();
		}
		//<-----
		if( m_pShinKani01 ){
			m_pShinKani01->ReadData2();
		}
		if( m_pShinKani01EX ){
			m_pShinKani01EX->ReadData2();
		}
		//----->('23.05.01 Add)
		if (m_pShinKani01EX2) {
			m_pShinKani01EX2->ReadData2();
		}
		//<-----

		UpdateSheet(0, 1);
	}

}


//-----------------------------------------------------------------------------
// 選択表より前までの表の更新処理
//-----------------------------------------------------------------------------
void CH31HyoView::UpdateUntilSheet()
{
	int cnt = m_MainTab.GetCurSel();

	//for (int i = 0; i < cnt; i++ ) {
	//	UpdateSheet( i );
	//}

	UpdateSheet( 0 );

}


//-----------------------------------------------------------------------------
// 予定26号の前課税期間のセット
//-----------------------------------------------------------------------------
void CH31HyoView::ZenkiDate_Chsk(char* ZenkSymd, char* ZenkEymd, int ZenkSg, int ZenkEg)
//void CH31HyoView::ZenkiDate_Chsk( char* ZenkSymd , char* ZenkEymd )
{
	if ( m_pSnChskData ) {
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 0, ZenkSymd[0], ZenkSg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 1, ZenkSymd[1], ZenkSg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 2, ZenkSymd[2], ZenkSg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO,   0, ZenkEymd[0], ZenkEg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO,   1, ZenkEymd[1], ZenkEg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO,   2, ZenkEymd[2], ZenkEg );
	}
}


//-----------------------------------------------------------------------------
// 表示中頁の更新処理後の再表示
//-----------------------------------------------------------------------------
void CH31HyoView::IndicateSheet()
{
	IndicateSheet( m_MainTab.GetCurSel() );
}


//-----------------------------------------------------------------------------
// タブへフォーカスセット
//-----------------------------------------------------------------------------
void CH31HyoView::EndJobFocus()
{
	m_MainTab.SetFocus();
}


//-----------------------------------------------------------------------------
//	入力ロック処理
//-----------------------------------------------------------------------------
// 引数	mode		：	０：本表・付表から呼び出し　１：課税取引計算表から呼び出し
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
void CH31HyoView::InputRock(int mode)
{
	if (m_pSnHeadData == NULL) {
		return;
	}
	else if ((m_pShinInfo == NULL) || (m_pShinInfo->pBaseView == NULL) || (m_pShinInfo->pToolBar == NULL)) {
		return;
	}

	// 入力データ保存
	unsigned char	svSn_Sign4 = m_pSnHeadData->Sn_Sign4;

	if (mode == 0) {

		//---->yoshida191216
		UpdateAllSheet();
		WriteData(FALSE, TRUE);
		//<------------------

		if (m_pSnHeadData->DoUnlock(m_pZmSub, this) == IDNO) {
			return;
		}
	}

	m_pShinInfo->pParent->PostMessage(WM_USER_UPDATE_INDICATE, 0);
	m_pShinInfo->pBaseView->SendMessage(WM_USER_UNLOCK, (WPARAM)0);

	if ((m_SnSeq > 0)) {
		// ロック解除
		if (!(m_pSnHeadData->Sn_Sign4 & 0x01)) {
			m_pSnHeadData->Sn_EXP100 &= ~0x01;
		}
	}

	if (mode == 1) {
		if (m_pSnHeadData->Sn_Sign4 & 0x01) {
			WriteData(FALSE, TRUE);
		}
	}
//	else {
//		//---->yoshida191216 コメント化
//		if ((svSn_Sign4 != m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4 & 0x01)) {
//			UpdateAllSheet();
//			WriteData(FALSE, TRUE);
//		}
//		//<---------------------
//	}

	char* p = NULL;
	if ( m_pSnHeadData->Sn_Sign4 & 0x01 ) {
		p = _T( "F9 ロック解除");
	}
	else {
		p = _T( "F9 入力ロック" );
	}
	m_pShinInfo->pToolBar->SetButtonTooltip ( 7, p );
	m_pShinInfo->pToolBar->SetButtonText ( 7, p );

	int	selno = m_MainTab.GetCurSel();
//	UpdateSheet(selno);

	if (mode == 1) {
		if (m_pSnHeadData->Sn_Sign4 & 0x01) {
			WriteData(FALSE, FALSE);
		}
	}
	else {
//		if ((svSn_Sign4 != m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4 & 0x01)) {
//			//---->yoshida191216 コメント化
//			WriteData(FALSE, FALSE);
//			//<---------
//		}
	}

	if (m_SnSeq == 0) {	// 現申告
		// 再集計
		m_pShinInfo->pParent->SendMessage(WM_USER_RECALQ, (WPARAM)0);
	}

	// DIAGの属性変更
	ChangeDiagAttr();

	InitTab(TRUE);
	if (mode == 0) {
		m_pShinInfo->pParent->SendMessage(WM_USER_CHG_LOCK_DISP);
	}

	m_MainTab.SetCurSel(selno);
	m_MainTab.ModifyStyle(TCS_SINGLELINE, TCS_MULTILINE);
	SetItemPosition(selno);

	RedrawInputLock();
	
}

//-----------------------------------------------------------------------------
// 簡易課税で横計ではない例外計算が必要？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	必要
//			FALSE	：	不要
//-----------------------------------------------------------------------------
BOOL CH31HyoView::IsNeedExpCalq()
{

	BOOL	bRt = FALSE;

	/*
	EnumIdIcsShKazeihoushiki	KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
	if ( KzHoushiki != ID_ICSSH_KANNI ) {
		return bRt;
	}
	if ( m_pSnHeadData->SVmzsw == 0 ) {
		return bRt;
	}

	if ( m_pSnFhyo4_01Data && m_pSnFhyo5_012Data ) {
		if ( m_pSnFhyo4_01Data-> ) {	// 関数未作成
			
		}
	}
	*/

	return bRt;
}

//-----------------------------------------------------------------------------
// 税務署入力にフォーカス誘導
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::GuideTaxOfficeInput()
{
	if ( m_pSnHeadData->IsMiddleProvisional() ) {
		if ( m_pChskEX ) {
			m_pChskEX->SetTaxOfficeInput();
		}
	}
	else {
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );

		if ( KzHoushiki == ID_ICSSH_KANNI ) {
			int max = (int)m_TabMng.GetCount();
			int page = -1;
			for (int i = 0; i < max; i++) {
				if (m_TabMng[i].tabtype == ID_ICSSH_KANI01_TAB) {
					page = i;
					break;
				}
			}

			if ( page != -1 ) {
				m_MainTab.SetCurSel( page );
				if ( m_pShinKani01 ) {
					m_pShinKani01->SetTaxOfficeInput();
				}
				if( m_pShinKani01EX ) {
					m_pShinKani01EX->SetTaxOfficeInput();
				}
				//----->('23.05.01 Add)
				if (m_pShinKani01EX2) {
					m_pShinKani01EX2->SetTaxOfficeInput();
				}
				//<-----
			}
		}
		else if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
			int max = (int)m_TabMng.GetCount();
			int page = -1;
			for (int i = 0; i < max; i++) {
				if (m_TabMng[i].tabtype == ID_ICSSH_IPPAN01_TAB) {
					page = i;
					break;
				}
			}

			if ( page != -1 ) {
				m_MainTab.SetCurSel( page );
				if ( m_pShinIppan01 ) {
					m_pShinIppan01->SetTaxOfficeInput();
				}
				if( m_pShinIppan01EX ) {
					m_pShinIppan01EX->SetTaxOfficeInput();
				}
				//----->('23.05.01 Add)
				if( m_pShinIppan01EX2 ) {
					m_pShinIppan01EX2->SetTaxOfficeInput();
				}
				//<-----
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 還付申告？('15.02.24)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	還付申告
//			FALSE	：	還付申告でない
//-----------------------------------------------------------------------------
BOOL CH31HyoView::IsKanpuShinkoku()
{
	BOOL	bRt = FALSE;

	ASSERT( m_pSnHeadData );
	if ( (m_pSnHeadData->IsMiddleProvisional()) || (m_pSnHeadData->IsTyukanShinkoku()) ) {
		;
	}
	else {
		char	mony[MONY_BUF_SIZE];
		m_Arith.l_add( mony, m_pSnHonpyoData->Sn_SIREZ, m_pSnHonpyoData->Sn_HKANZ );
		m_Arith.l_add(mony, mony, m_pSnHonpyoData->Sn_KSITZ );
		m_Arith.l_sub(mony, mony, m_pSnHonpyoData->Sn_SYTX );
		m_Arith.l_sub(mony, mony, m_pSnHonpyoData->Sn_KJADJZ );
		if ( m_Arith.l_test( mony ) > 0 ) {
			bRt = TRUE;
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 還付申告？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	還付申告
//			FALSE	：	還付申告でない
//-----------------------------------------------------------------------------
BOOL CH31HyoView::IsKanpuShinkoku2()
{
	BOOL	bRt = FALSE;

	ASSERT( m_pSnHeadData );
	if ( (m_pSnHeadData->IsMiddleProvisional()) || (m_pSnHeadData->IsTyukanShinkoku()) ) {
		;
	}
	else {
		if (m_pShinIppan01) {
			if (m_pShinIppan01->IsKanpu() == TRUE) {
				bRt = TRUE;
			}
		}
		if( m_pShinIppan01EX ) {
			if( m_pShinIppan01EX->IsKanpu() == TRUE ) {
				bRt = TRUE;
			}
		}
		//----->('23.05.01 Add)
		if ( m_pShinIppan01EX2 ) {
			if ( m_pShinIppan01EX2->IsKanpu() == TRUE ) {
				bRt = TRUE;
			}
		}
		//<-----
		if (m_pShinKani01){
			if (m_pShinKani01->IsKanpu() == TRUE) {
				bRt = TRUE;
			}
		}
		if( m_pShinKani01EX ) {
			if( m_pShinKani01EX->IsKanpu() == TRUE ) {
				bRt = TRUE;
			}
		}
		//----->('23.05.01 Add)
		if ( m_pShinKani01EX2 ) {
			if ( m_pShinKani01EX2->IsKanpu() == TRUE ) {
				bRt = TRUE;
			}
		}
		//<-----
	}


	return bRt;
}


//-----------------------------------------------------------------------------
// 出力用税理士名の作成('15.02.27)
//-----------------------------------------------------------------------------
// 引数	pOutBuf	：	出力用バッファ
//		len		：	出力用バッファ長
//		Honpyo	：	1：本表出力	/ 0：更正の請求・予定26号
//-----------------------------------------------------------------------------
int CH31HyoView::MakeOutZeirishiName( char *pOutBuf, int len, int Honpyo )
{
	const int ZNAME_BUF_LEN = 40;
	int tmplen = 0;
	char zbuf[128] = { 0 };
	int rt = 0;

	if ( m_pPrintCmInfo->OPA == 2 ) {
		if ( ( tmplen = ( int )strlen ( m_pShinInfo->pO_DATA->o_zeinam ) ) > ZNAME_BUF_LEN ) {
			tmplen = ZNAME_BUF_LEN;
		}
		memmove ( zbuf, m_pShinInfo->pO_DATA->o_zeinam, tmplen );
	}
	else {
		if ( m_pPrintCmInfo->OPA ) {
			// 税理士法人名
			if ( ( tmplen = ( int )strlen ( m_pShinInfo->pO_DATA->o_ofcnam ) ) > ZNAME_BUF_LEN ) {
				tmplen = ZNAME_BUF_LEN;
			}
			memmove ( zbuf, m_pShinInfo->pO_DATA->o_ofcnam, tmplen );
		}
		else {
			// 税理士名
			if ( ( tmplen = ( int )strlen ( m_pShinInfo->pO_DATA->o_zeinam ) ) > ZNAME_BUF_LEN ) {
				tmplen = ZNAME_BUF_LEN;
			}
			memmove ( zbuf, m_pShinInfo->pO_DATA->o_zeinam, tmplen );
		}
	}

	if ( m_pPrintCmInfo->zr_kbn == 2 ) {
		if ( m_pSnHeadData->m_ZroutSgn & 0x01 ) {	// 税理士区分を出力
			CString kbName;
			GetZeirishiKubunName ( m_pPrintCmInfo->zr_kbn, kbName );
			CString outZname;
			outZname.Format ( _T( "%s %s" ), kbName, zbuf );
			// 本表印刷時、税理士区分+税理士名が14文字を超える場合は直接受任を税理士名の下に表示させる
			int len = outZname.GetLength();
			if( Honpyo && (outZname.GetLength() > 27) ){
				rt = 1;
			}
			else {
				if ( m_pSnHeadData->m_ZroutSgn & 0x02 ) {	// 直接受任を出力
					outZname += _T( "（直接受任）" );
				}
			}
			memset ( pOutBuf, '\0', len );
			wsprintf ( pOutBuf, _T( "%s" ), outZname.GetBuffer() );
		}
		else {
			wsprintf ( pOutBuf, _T( "%s" ), zbuf );
		}
	}
	else {
		wsprintf ( pOutBuf, _T( "%s" ), zbuf );
	}

	return rt;
}


//-----------------------------------------------------------------------------
// 入力位置を保存
//-----------------------------------------------------------------------------
void CH31HyoView::SaveInputPosition()
{
	int selno = m_MainTab.GetCurSel();
	if (selno < (int)m_TabMng.GetCount()) {
		if (m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB) {
			if (m_pShinIppan01) {
				m_pShinIppan01->SaveInputPosition();
			}
			if( m_pShinIppan01EX ) {
				m_pShinIppan01EX->SaveInputPosition();
			}
			//----->('23.05.01 Add)
			if( m_pShinIppan01EX2 ) {
				m_pShinIppan01EX2->SaveInputPosition();
			}
			//<-----
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB) {
			if (m_pShinKani01) {
				m_pShinKani01->SaveInputPosition();
			}
			if( m_pShinKani01EX ) {
				m_pShinKani01EX->SaveInputPosition();
			}
			//----->('23.05.01 Add)
			if( m_pShinKani01EX2 ) {
				m_pShinKani01EX2->SaveInputPosition();
			}
			//<-----
		}
	}

}


//-----------------------------------------------------------------------------
// 入力位置を復元
//-----------------------------------------------------------------------------
void CH31HyoView::RestoreInputPosition()
{
	int selno = m_MainTab.GetCurSel();
	if (selno < (int)m_TabMng.GetCount()) {
		if (m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB) {
			if (m_pShinIppan01) {
				m_pShinIppan01->RestoreInputPosition();
			}
			if( m_pShinIppan01EX ) {
				m_pShinIppan01EX->RestoreInputPosition();
			}
			//----->('23.05.01 Add)
			if( m_pShinIppan01EX2 ) {
				m_pShinIppan01EX2->RestoreInputPosition();
			}
			//<-----
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB) {
			if (m_pShinKani01) {
				m_pShinKani01->RestoreInputPosition();
			}
			if( m_pShinKani01EX ) {
				m_pShinKani01EX->RestoreInputPosition();
			}
			//----->('23.05.01 Add)
			if( m_pShinKani01EX2 ) {
				m_pShinKani01EX2->RestoreInputPosition();
			}
			//<-----
		}
	}

}


//-----------------------------------------------------------------------------
// 終了時のフォーカス移動
//-----------------------------------------------------------------------------
void CH31HyoView::MoveFocusInEnd()
{
	int selno = m_MainTab.GetCurSel();
	if ((selno != -1) && (selno < (int)m_TabMng.GetCount())) {
		if (m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB) {
			if (m_pShinIppan01) {
				m_pShinIppan01->MoveFocusInEnd();
			}
			if( m_pShinIppan01EX ) {
				m_pShinIppan01EX->MoveFocusInEnd();
			}
			//----->('23.05.01 Add)
			if( m_pShinIppan01EX2 ) {
				m_pShinIppan01EX2->MoveFocusInEnd();
			}
			//<-----
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB) {
			if (m_pShinKani01) {
				m_pShinKani01->MoveFocusInEnd();
			}
			if( m_pShinKani01EX ) {
				m_pShinKani01EX->MoveFocusInEnd();
			}
			//----->('23.05.01 Add)
			if( m_pShinKani01EX2 ) {
				m_pShinKani01EX2->MoveFocusInEnd();
			}
			//<-----
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_CHSK_TAB) {
			if (m_pChskEX) {
				m_pChskEX->MoveFocusInEnd();
			}
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB) {
			if (m_pHjnKskEX) {
				m_pHjnKskEX->MoveFocusInEnd();
			}
			else if (m_pHjnKskEX2) {	// 更正の請求/新様式対応
				m_pHjnKskEX2->MoveFocusInEnd();
			}
		}
		else if (m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB) {
			if (m_pKjnKskEX) {
				m_pKjnKskEX->MoveFocusInEnd();
			}
			else if (m_pKjnKskEX2) {	// 更正の請求/新様式対応
				m_pKjnKskEX2->MoveFocusInEnd();
			}
		}
	}

}




// 各表の更新処理後の再表示
void CH31HyoView::IndicateSheet( int selno )
{

	if (m_pSnHeadData->IsMiddleProvisional()) {	// 前年実績による中間申告（第２６号様式）
		if ( m_pChskEX ) {
			m_pChskEX->Init();
		}
	}
	else {
		if ((int)m_TabMng.GetCount() <= selno) {
			return;
		}

		switch (m_TabMng[selno].tabtype) {
		case ID_ICSSH_FHYO1_01_TAB:
			if (m_pFhyo1_01) {
				m_pFhyo1_01->MemoryReget();
				m_pFhyo1_01->DispFh1_01();
			}
			break;
		case ID_ICSSH_FHYO1_02_TAB:
			if (m_pFhyo1_02) {
				m_pFhyo1_02->MemoryReget();
				m_pFhyo1_02->DispFh1_02();
			}
			break;
		case ID_ICSSH_FHYO1_03_TAB:
			if ( m_pFhyo1_03 ) {
				m_pFhyo1_03->MemoryReget();
				m_pFhyo1_03->DispFh1_03();
			}
			break;
		case ID_ICSSH_FHYO2_01_TAB:
			if (m_pFhyo2_01) {
				//m_pFhyo2_01->Init();
				m_pFhyo2_01->MemoryReget();
				m_pFhyo2_01->DispFh2_01();
			}

			if ( m_pFhyo2_11 ) {
				m_pFhyo2_11->MemoryReget();
				m_pFhyo2_11->DispFh2_11();
			}

			//---->20230120
			if ( m_pFhyo2_11Ex ) {
				m_pFhyo2_11Ex->MemoryReget();
				m_pFhyo2_11Ex->DispFh2_11();
			}
			//<----20230120
			break;
		case ID_ICSSH_FHYO2_02_TAB:
			if (m_pFhyo2_02) {
				//m_pFhyo2_02->Init();
				m_pFhyo2_02->MemoryReget();
				m_pFhyo2_02->DispFh2_02();
			}

			if ( m_pFhyo2_12 ) {
				m_pFhyo2_12->MemoryReget();
				m_pFhyo2_12->DispFh2_12();
			}

			//---->20230120
			if ( m_pFhyo2_12Ex ) {
				m_pFhyo2_12Ex->MemoryReget();
				m_pFhyo2_12Ex->DispFh2_12();
			}
			//<----20230120
			break;
		case ID_ICSSH_FHYO2_03_TAB:
			if ( m_pFhyo2_03 ) {
				m_pFhyo2_03->MemoryReget();
				m_pFhyo2_03->DispFh2_03();
			}

			//---->20230120
			if ( m_pFhyo2_03Ex ) {
				m_pFhyo2_03Ex->MemoryReget();
				m_pFhyo2_03Ex->DispFh2_03();
			}
			//<----20230120
			break;
		case ID_ICSSH_FHYO4_01_TAB:
			if ( m_pFhyo4_01 ) {
				//m_pFhyo4_01->Init();
				m_pFhyo4_01->MemoryReget();
				m_pFhyo4_01->DispFh4_01();
			}
			break;
		case ID_ICSSH_FHYO4_02_TAB:
			if ( m_pFhyo4_02 ) {
				//m_pFhyo4_02->Init();
				m_pFhyo4_02->MemoryReget();
				m_pFhyo4_02->DispFh402();
			}
			break;
		case ID_ICSSH_FHYO4_03_TAB:
			if ( m_pFhyo4_03 ) {
				m_pFhyo4_03->MemoryReget();
				m_pFhyo4_03->DispFh4_03();
			}
			break;
		case ID_ICSSH_FHYO5_01_TAB:
			if ( m_pFhyo5_01 ) {
				m_pFhyo5_01->MemoryReget();
				m_pFhyo5_01->DispFh501();
			}
			break;
		case ID_ICSSH_FHYO5_012_TAB:
			if ( m_pFhyo5_012 ) {
				m_pFhyo5_012->MemoryReget();
				m_pFhyo5_012->Disp_f5012();
			}
			break;
		case ID_ICSSH_FHYO5_02_TAB:
			if ( m_pFhyo5_02 ) {
				m_pFhyo5_02->MemoryReget();
				m_pFhyo5_02->DispFh5_02();
			}
			break;
		case ID_ICSSH_FHYO5_022_TAB:
			if ( m_pFhyo5_022 ) {
				m_pFhyo5_022->MemoryReget();
				m_pFhyo5_022->Disp_f5022();
			}
			break;
		case ID_ICSSH_FHYO5_03_TAB:
			if ( m_pFhyo5_03) {
				m_pFhyo5_03->MemoryReget();
				m_pFhyo5_03->DispFh5_03();
			}
			break;
		case ID_ICSSH_FHYO5_032_TAB:
			if ( m_pFhyo5_032 ) {
				m_pFhyo5_032->MemoryReget();
				m_pFhyo5_032->DispFh5_032();
			}
			break;
		//--->付表６(２割特例) ('23.05.01 Add)
		case ID_ICSSH_FHYO6_01_TAB:
			if ( m_pFhyo6_01 ) {
				m_pFhyo6_01->MemoryReget();
				m_pFhyo6_01->DispFh6_01();
			}
			break;
		//<---
		case ID_ICSSH_IPPAN01_TAB:
			if ( m_pShinIppan01 ) {
				m_pShinIppan01->MemoryReget();
				m_pShinIppan01->Disp_Ippandata ( 0 );
				m_pShinIppan01->Disp_FhyoData();
			}
			if( m_pShinIppan01EX ) {
				m_pShinIppan01EX->MemoryReget();
				m_pShinIppan01EX->Disp_Ippandata ( 0 );
				m_pShinIppan01EX->Disp_FhyoData();
			}
			//----->('23.05.01 Add)
			if( m_pShinIppan01EX2 ) {
				m_pShinIppan01EX2->MemoryReget();
				m_pShinIppan01EX2->Disp_Ippandata(0);
				m_pShinIppan01EX2->Disp_FhyoData();
			}
			//<-----
			break;
		case ID_ICSSH_IPPAN02_TAB:
			if ( m_pShinIppan02 ) {
				m_pShinIppan02->GetFhyoData();
				m_pShinIppan02->SetDiagData();
				m_pShinIppan02->DispIppan2Upper();
			}
			break;
		case ID_ICSSH_KANI01_TAB:
			if ( m_pShinKani01 ) {
				m_pShinKani01->MemoryReget();
				m_pShinKani01->GetFhyoData();
				m_pShinKani01->Disp_Kanidata ( 0 );
				m_pShinKani01->Disp_FhyoData();
			}
			if ( m_pShinKani01EX ) {
				m_pShinKani01EX->MemoryReget();
				m_pShinKani01EX->GetFhyoData();
				m_pShinKani01EX->Disp_Kanidata ( 0 );
				m_pShinKani01EX->Disp_FhyoData();
			}
			//----->('23.05.01 Add)
			if ( m_pShinKani01EX2 ) {
				m_pShinKani01EX2->MemoryReget();
				m_pShinKani01EX2->GetFhyoData();
				m_pShinKani01EX2->Disp_Kanidata(0);
				m_pShinKani01EX2->Disp_FhyoData();
			}
			//<-----
			break;
		case ID_ICSSH_FHYO6_TAB:
			if ( m_pFhyo6 ) {
				m_pFhyo6->Init();
			}
			break;
		case ID_ICSSH_KSISANURI_TAB:
			if ( m_pKsisanuri ) {
				m_pKsisanuri->SetName();
			}
			break;
		case ID_ICSSH_KSISANURI2_TAB:
			if ( m_pKsisanuri2 ) {
				m_pKsisanuri2->SetName();
			}
			break;
		case ID_ICSSH_KSHIIRE_TAB:
			if ( m_pKshiire ) {
				m_pKshiire->SetName();
			}
			break;
		case ID_ICSSH_KSK_HJ_TAB:
			if ( m_pHjnKskEX ) {
				// '20.01.27 kasai 更正の請求に本表の金額が反映されておらず、「正当とする額」が正しく集計されていなかったため修正。
				if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
					CalqSyzForKsk(1);
				}
				m_pHjnKskEX->DispKsiHjn();
			}
			else if ( m_pHjnKskEX2 ) {	// 更正の請求/新様式対応
				if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
					CalqSyzForKsk(1);
				}
				m_pHjnKskEX2->DispKsiHjn();
			}
			break;
		case ID_ICSSH_KSK_KJ_TAB:
			if ( m_pKjnKskEX ) {
				// '20.01.27 kasai 更正の請求に本表の金額が反映されておらず、「正当とする額」が正しく集計されていなかったため修正
				if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
					CalqSyzForKsk(1);
				}
				m_pKjnKskEX->DispKsiKjn();
			}
			else if ( m_pKjnKskEX2 ) {	// 更正の請求/新様式対応
				if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
					CalqSyzForKsk(1);
				}
				m_pKjnKskEX2->DispKsiKjn();
			}
			break;
		default:
			break;
		}
	}

}

	// 改行コードの検索
int CH31HyoView::Getindex_toNewLineCode( int start, int *end, CString str )
{
	int ret = -1;
	char code1 = '\r';
	char code2 = '\n';
	*end = str.GetLength();

	for (int idx = start; idx < str.GetLength(); idx++) {
		if (str.Mid(idx, 1) == '\0') {
			*end = idx - start;
			ret = -1;
			break;
		}

		if (str.Mid(idx, 1) == code1) {
			if (str.Mid(idx + 1, 1) == code2) {
				*end = idx - start;
				ret = 0;
				break;
			}
		}
	}

	return ret;
}

//======================================
// 全角・半角交じり判定
//--------------------------------------
// 引数
//		dmmy	:	文字列
//		num		:	折り返しバイト数
//--------------------------------------
// 返送値
//		0		:	正常終了
//======================================
int CH31HyoView::Strchek ( char *dmmy, int num )
{
	int i = 0, k = 0;
	for ( i = 0 ; i < num ; ) {
		if ( *dmmy == NULL){
			// 途中終わり　そこまでのバイト数を返す
			return i;
		}

		k = IsKindOfLetter ( ( const char* )dmmy, 0 );
		if ( k == 2 ) {
			dmmy += 2;
			i += 2;

			if ( i == num ) {
				return num;
			}
			else if ( i > num ) {
				return num - 1;
			}
			continue;
		}
		else { 
			//k == 1 と　エラー　は１つ移動
			i += 1;
			dmmy += 1;
			if ( i == num ) {
				return num;
			}
			continue;
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// DIAGの属性変更
//-----------------------------------------------------------------------------
void CH31HyoView::ChangeDiagAttr()
{
	// 31テスト削除
	/*if (m_pShinIppan01) {
		m_pShinIppan01->ChgDiagAttr();
	}
	if (m_pShinKani01) {
		m_pShinKani01->ChgDiagAttr();
	}*/
	
	/*
	if (m_pKaniEX) {
		m_pKaniEX->ChgDiagAttr();
	}

	if (m_pIppanEX) {
		m_pIppanEX->ChgDiagAttr();
	}
	if (m_pKaniEX2) {
		m_pKaniEX2->ChgDiagAttr();
	}
	*/

}

//-----------------------------------------------------------------------------
// 入力ロックコントロールの再描画	('17.02.27)
//-----------------------------------------------------------------------------
void CH31HyoView::RedrawInputLock()
{
	int selno = m_MainTab.GetCurSel();
/*	if ((m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_IPPAN02_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB) || 
		(m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_02_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_02_TAB) || 
		(m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_02_TAB) || 
		(m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_012_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_02_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_022_TAB) || 
		(m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB)) {*/
	if ( ( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB )  || ( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN02_TAB )   || ( m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB )   || 
		 ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_01_TAB ) || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_02_TAB )  || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO1_03_TAB ) ||
		 ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_01_TAB ) || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_02_TAB )  || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO2_03_TAB ) ||
		 ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_01_TAB ) || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_02_TAB )  || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO4_03_TAB ) ||
		 ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_01_TAB ) || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_012_TAB ) ||
	 	 ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_02_TAB ) || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_022_TAB ) ||
		 ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_03_TAB ) || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO5_032_TAB ) ||
		 ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB )    || ( m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_01_TAB ) ) {

		if (m_TabMng[selno].pWnd) {
			((CSyzBaseDlg*)m_TabMng[selno].pWnd)->RedrawInputLock();
		}
	}
}


//-----------------------------------------------------------------------------
// 還付申告用データ再設定 ('14.05.17)
//-----------------------------------------------------------------------------
void CH31HyoView::ResetRefundData()
{
	// 情報設定
	if (m_pSnHeadData) {
		CRefundBaseDialog::pSnHeadData = m_pSnHeadData;
	}
	if (m_pSnRefundData) {
		CRefundBaseDialog::pSnRefundData = m_pSnRefundData;
		CRefundBaseDialog::KAZEIU_REC = &m_pSnRefundData->RefundJot[0];
		CRefundBaseDialog::YUNYUT_REC = &m_pSnRefundData->RefundExp[0];
		CRefundBaseDialog::SHIIRE_REC = &m_pSnRefundData->RefundStk[0];
		CRefundBaseDialog::TANAOR_REC = &m_pSnRefundData->RefundTna[0];
		CRefundBaseDialog::KOTEIS_REC = &m_pSnRefundData->RefundKot[0];
	}

	CRefundBaseDialog::pUtil = &m_Util;
	CRefundBaseDialog::pZmSub = m_pZmSub;
	CDiagControlBase::pUtil = &m_Util;
	CDiagControlBase::pZmSub = m_pZmSub;
}


int CH31HyoView::f501_jigyo()
{
	if (m_pSnHonpyoData == NULL) {
		return -1;
	}

	m_typeCnt = 0;

	if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1)) {
		m_typeCnt++;
	}
	if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2)) {
		m_typeCnt++;
	}
	if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3)) {
		m_typeCnt++;
	}
	if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4)) {
		m_typeCnt++;
	}
	if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5)) {
		m_typeCnt++;
	}
	if (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI6)) {
		m_typeCnt++;
	}

	return 0;
}

int CH31HyoView::f501_urikbn()
{
	//暫定対応

	if (m_pSnFhyo5_01Data == NULL) {
		return -1;
	}

	int ret = 0;

	if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_7F)) {
		ret |= 0x01;
	}
	if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_8F)) {
		ret |= 0x02;
	}
	if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_9F)) {
		ret |= 0x04;
	}
	if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_10F)) {
		ret |= 0x08;
	}
	if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_11F)) {
		ret |= 0x10;
	}
	if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_12F)) {
		ret |= 0x20;
	}

	m_tkSw = ret;

	return 0;
}

int  CH31HyoView::f501_calq_tk_for_disp()
{
	char	max_val[MONY_BUF_SIZE], val[MONY_BUF_SIZE];
	int		maxSign = 0;

	memset(max_val, '\0', MONY_BUF_SIZE);

	for (int jigyo_cnt = 1; jigyo_cnt < 16; jigyo_cnt++) {
		switch (jigyo_cnt) {
		case 1:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_22F, MONY_BUF_SIZE);
			break;
		case 2:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_23F, MONY_BUF_SIZE);
			break;
		case 3:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_24F, MONY_BUF_SIZE);
			break;
		case 4:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_25F, MONY_BUF_SIZE);
			break;
		case 5:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_26F, MONY_BUF_SIZE);
			break;
		case 6:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_27F, MONY_BUF_SIZE);
			break;
		case 7:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_28F, MONY_BUF_SIZE);
			break;
		case 8:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_29F, MONY_BUF_SIZE);
			break;
		case 9:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_30F, MONY_BUF_SIZE);
			break;
		case 10:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_31F, MONY_BUF_SIZE);
			break;
		case 11:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_32F, MONY_BUF_SIZE);
			break;
		case 12:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_33F, MONY_BUF_SIZE);
			break;
		case 13:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_34F, MONY_BUF_SIZE);
			break;
		case 14:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_35F, MONY_BUF_SIZE);
			break;
		case 15:
			memmove(val, m_pSnFhyo5_012Data->Sn_F5012_36F, MONY_BUF_SIZE);
			break;
		default:
			break;
		}


		if (m_Arith.l_cmp(max_val, val) < 0) {
			memmove(max_val, val, MONY_BUF_SIZE);
			maxSign = jigyo_cnt;
		}
	}

	memmove(val, m_pSnFhyo5_012Data->Sn_F5012_21F, MONY_BUF_SIZE);
	if (m_Arith.l_cmp(max_val, val) < 0) {
		memmove(max_val, val, MONY_BUF_SIZE);
		maxSign = 0;
	}

	m_dspSkjSw = 0;
	if (m_typeCnt > 1) {
		if (m_Arith.l_test(m_pSnFhyo5_012Data->Sn_F5012_21X) || m_Arith.l_test(m_pSnFhyo5_012Data->Sn_F5012_21D) || m_Arith.l_test(m_pSnFhyo5_012Data->Sn_F5012_21E)) {
			m_dspSkjSw |= 0x01;
		}
		if (m_Arith.l_test(max_val)) {
			long	onbit = 0x02 << maxSign;
			m_dspSkjSw |= onbit;
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 初期入力位置にフォーカスセット
//-----------------------------------------------------------------------------
// 引数	selno	：	セットするタブNO
//-----------------------------------------------------------------------------
void CH31HyoView::SetItemPosition(int selno)
{
	if ((m_pSnHeadData == NULL) || (m_pZmSub == NULL)) {
		return;
	}
	

	if (m_pSnHeadData->IsMiddleProvisional()) {	// 前年実績による中間申告（第２６号様式）
		
		if (m_pChskEX) {
			m_pChskEX->SetFocus();
			m_pChskEX->SetItemPosition(0);
		}
	}

	else {
		if ((selno == -1) || ((int)m_TabMng.GetCount() <= selno)) {
			return;
		}

		// 経理処理
		if (m_pSnHeadData->IsUseTaxRefund()) {		// 消費税の還付申告に関する明細書
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
				// 簡易課税
				if (m_pSnHeadData->SVmzsw == 1) {
					// 経過措置対象の課税資産の譲渡等あり
					switch (m_TabMng[selno].tabtype) {
					case ID_ICSSH_FHYO4_01_TAB: // 付表４－１
						if (m_pFhyo4_01) {
							m_pFhyo4_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO4_02_TAB: // 付表４－２
						if (m_pFhyo4_02) {
							m_pFhyo4_02->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_01_TAB: // 付表５ー１
						if (m_pFhyo5_01) {
							m_pFhyo5_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_012_TAB: // 付表５ー１（２）
						if (m_pFhyo5_012) {
							m_pFhyo5_012->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_02_TAB: // 付表５ー２（２）
						if (m_pFhyo5_02) {
							m_pFhyo5_02->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_022_TAB: // 付表５ー２（２）
						if (m_pFhyo5_022) {
							m_pFhyo5_022->SetItemPosition();
						}
						break;
					//----->('23.05.01 Add)
					case ID_ICSSH_FHYO6_01_TAB:
						if (m_pFhyo6_01) {
							m_pFhyo6_01->SetItemPosition();
						}
						break;
					//<-----
					case ID_ICSSH_KANI01_TAB: // 簡易本表
						if (m_pShinKani01) {
							m_pShinKani01->SetItemPosition();
						}
						if( m_pShinKani01EX ) {
							m_pShinKani01EX->SetItemPosition();
						}
						//----->('23.05.01 Add)
						if( m_pShinKani01EX2 ) {
							m_pShinKani01EX2->SetItemPosition();
						}
						//<-----
						break;

					case ID_ICSSH_IPPAN02_TAB: // 一般本表（２）
						if (m_pShinIppan02) {
							m_pShinIppan02->SetItemPosition();
						}
						break;

					case ID_ICSSH_REFUND_KJ_01_TAB: // 還付付表①
					case ID_ICSSH_REFUND_HJ_01_TAB: // 還付付表①
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn01) {
								m_pRefundKjn01->SetInitPosition(0);
							}
						}
						else {
							if (m_pRefundHjn01) {
								m_pRefundHjn01->SetInitPosition(1);
							}
						}
						break;
					case ID_ICSSH_REFUND_KJ_02_TAB: // 還付付表②
					case ID_ICSSH_REFUND_HJ_02_TAB: // 還付付表②
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn02) {
								m_pRefundKjn02->SetInitPosition(2);
							}
							//----> 20230704
							if (m_pRefundKjn02EX) {
								m_pRefundKjn02EX->SetInitPosition(2);
							}
							//<---- 20230704
						}
						else {
							if (m_pRefundHjn02) {
								m_pRefundHjn02->SetInitPosition(3);
							}
							//----> 20230704
							if (m_pRefundHjn02EX) {
								m_pRefundHjn02EX->SetInitPosition(3);
							}
							//<---- 20230704
						}
						break;
					case ID_ICSSH_KSK_HJ_TAB: // 更正の請求
					case ID_ICSSH_KSK_KJ_TAB: // 更正の請求
						if (!(m_pSnHeadData->Sn_Syuusei & 0x02)) {
							break;
						}
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pKjnKskEX) {
								m_pKjnKskEX->SetItemPosition(0);
							}
							else if (m_pKjnKskEX2) {	// 更正の請求/新様式対応
								m_pKjnKskEX2->SetItemPosition(0);
							}
						}
						else {
							if (m_pHjnKskEX) {
								m_pHjnKskEX->SetItemPosition(0);
							}
							else if (m_pHjnKskEX2) {	// 更正の請求/新様式対応
								m_pHjnKskEX2->SetItemPosition(0);
							}
						}
						break;
					case ID_ICSSH_KSISANURI_TAB:
						if( m_pKsisanuri ){
							m_pKsisanuri->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSISANURI2_TAB:
						if( m_pKsisanuri2 ){
							m_pKsisanuri2->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSHIIRE_TAB:
						if( m_pKshiire ){
							m_pKshiire->SetItemPosition();
						}
						break;
					default:
						break;
					}
				}
				else {
					// 経過措置対象の課税資産の譲渡等無し
					switch (m_TabMng[selno].tabtype) {

					case ID_ICSSH_FHYO4_01_TAB: // 付表４－１
						if (m_pFhyo4_01) {
							m_pFhyo4_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_01_TAB: // 付表５ー１
						if (m_pFhyo5_01) {
							m_pFhyo5_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_012_TAB: // 付表５ー１（２）
						if (m_pFhyo5_012) {
							m_pFhyo5_012->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO4_03_TAB: // 付表４－３
						if ( m_pFhyo4_03 ) {
							m_pFhyo4_03->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_03_TAB: // 付表５ー３
						if ( m_pFhyo5_03 ) {
							m_pFhyo5_03->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_032_TAB: // 付表５ー３（２）
						if ( m_pFhyo5_032 ) {
							m_pFhyo5_032->SetItemPosition();
						}
						break;
					//----->('23.05.01 Add)
					case ID_ICSSH_FHYO6_01_TAB:	//付表６(２割特例)
						if ( m_pFhyo6_01) {
							m_pFhyo6_01->SetItemPosition();
						}
						break;
					//<-----


					case ID_ICSSH_KANI01_TAB: // 簡易本表
						if (m_pShinKani01) {
							m_pShinKani01->SetItemPosition();
						}
						if( m_pShinKani01EX ) {
							m_pShinKani01EX->SetItemPosition();
						}
						//----->('23.05.01 Add)
						if( m_pShinKani01EX2 ) {
							m_pShinKani01EX2->SetItemPosition();
						}
						//<-----
						break;
					case ID_ICSSH_IPPAN02_TAB: // 一般本表（２）
						if (m_pShinIppan02) {
							m_pShinIppan02->SetItemPosition();
						}
						break;


					case ID_ICSSH_REFUND_KJ_01_TAB: // 還付付表①
					case ID_ICSSH_REFUND_HJ_01_TAB: // 還付付表①
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn01) {
								m_pRefundKjn01->SetInitPosition(0);
							}
						}
						else {
							if (m_pRefundHjn01) {
								m_pRefundHjn01->SetInitPosition(1);
							}
						}
						break;
					case ID_ICSSH_REFUND_KJ_02_TAB: // 還付付表②
					case ID_ICSSH_REFUND_HJ_02_TAB: // 還付付表②
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn02) {
								m_pRefundKjn02->SetInitPosition(2);
							}
							//----> 20230704
							if (m_pRefundKjn02EX) {
								m_pRefundKjn02EX->SetInitPosition(2);
							}
							//<---- 20230704
						}
						else {
							if (m_pRefundHjn02) {
								m_pRefundHjn02->SetInitPosition(3);
							}
							//----> 20230704
							if (m_pRefundHjn02EX) {
								m_pRefundHjn02EX->SetInitPosition(3);
							}
							//<---- 20230704
						}
						break;
					case ID_ICSSH_KSK_HJ_TAB: // 更正の請求
					case ID_ICSSH_KSK_KJ_TAB: // 更正の請求
						if (!(m_pSnHeadData->Sn_Syuusei & 0x02)) {
							break;
						}
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pKjnKskEX) {
								m_pKjnKskEX->SetItemPosition(0);
							}
							else if (m_pKjnKskEX2) {	// 更正の請求/新様式対応
								m_pKjnKskEX2->SetItemPosition(0);
							}
						}
						else {
							if (m_pHjnKskEX) {
								m_pHjnKskEX->SetItemPosition(0);
							}
							else if (m_pHjnKskEX2) {	// 更正の請求/新様式対応
								m_pHjnKskEX2->SetItemPosition(0);
							}
						}
						break;
					case ID_ICSSH_KSISANURI_TAB:
						if( m_pKsisanuri ){
							m_pKsisanuri->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSISANURI2_TAB:
						if( m_pKsisanuri2 ){
							m_pKsisanuri2->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSHIIRE_TAB:
						if( m_pKshiire ){
							m_pKshiire->SetItemPosition();
						}
						break;
					default:
						break;
					}
				}
			}
			else {
				// 一般課税
				if (m_pSnHeadData->SVmzsw == 1) {
					// 経過措置対象の課税資産の譲渡等あり
					switch (m_TabMng[selno].tabtype) {
					case ID_ICSSH_FHYO1_01_TAB: // 付表１－１
						if (m_pFhyo1_01) {
							m_pFhyo1_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO1_02_TAB: // 付表１－２
						if (m_pFhyo1_02) {
							m_pFhyo1_02->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO2_01_TAB: // 付表２－１
						if (m_pFhyo2_01) {
							m_pFhyo2_01->SetItemPosition();
						}

						if ( m_pFhyo2_11 ) {
							m_pFhyo2_11->SetItemPosition();
						}

						//---->20230120
						if ( m_pFhyo2_11Ex ) {
							m_pFhyo2_11Ex->SetItemPosition();
						}
						//<----20230120
						break;

					case ID_ICSSH_FHYO2_02_TAB: // 付表２－２
						if (m_pFhyo2_02) {
							m_pFhyo2_02->SetItemPosition();	
						}

						if ( m_pFhyo2_12 ) {
							m_pFhyo2_12->SetItemPosition();	
						}

						//---->20230120
						if ( m_pFhyo2_12Ex ) {
							m_pFhyo2_12Ex->SetItemPosition();	
						}
						//<----20230120
						break;
					//----->('23.05.01 Add)
					case ID_ICSSH_FHYO6_01_TAB:	//付表６(２割特例)
						if( m_pFhyo6_01 ) {
							m_pFhyo6_01->SetItemPosition();
						}
						break;
					//<-----
					case ID_ICSSH_IPPAN01_TAB: // 一般本表
						if (m_pShinIppan01) {
							m_pShinIppan01->SetItemPosition();
						}
						if( m_pShinIppan01EX ) {
							m_pShinIppan01EX->SetItemPosition();
						}
						//----->('23.05.01 Add)
						if( m_pShinIppan01EX2 ) {
							m_pShinIppan01EX2->SetItemPosition();
						}
						//<-----
						break;

					case ID_ICSSH_IPPAN02_TAB: // 一般本表（２）
						if (m_pShinIppan02) {
							m_pShinIppan02->SetItemPosition();
						}
						break;

					case ID_ICSSH_REFUND_KJ_01_TAB: // 還付付表①
					case ID_ICSSH_REFUND_HJ_01_TAB: // 還付付表①
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn01) {
								m_pRefundKjn01->SetInitPosition(0);
							}
						}
						else {
							if (m_pRefundHjn01) {
								m_pRefundHjn01->SetInitPosition(1);
							}
						}
						break;
					case ID_ICSSH_REFUND_KJ_02_TAB: // 還付付表②
					case ID_ICSSH_REFUND_HJ_02_TAB: // 還付付表②
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn02) {
								m_pRefundKjn02->SetInitPosition(2);
							}
							//----> 20230704
							if (m_pRefundKjn02EX) {
								m_pRefundKjn02EX->SetInitPosition(2);
							}
							//<---- 20230704
						}
						else {
							if (m_pRefundHjn02) {
								m_pRefundHjn02->SetInitPosition(3);
							}
							//----> 20230704
							if (m_pRefundHjn02EX) {
								m_pRefundHjn02EX->SetInitPosition(3);
							}
							//<---- 20230704
						}
						break;
					case ID_ICSSH_KSK_HJ_TAB: // 更正の請求
					case ID_ICSSH_KSK_KJ_TAB: // 更正の請求
						if (!(m_pSnHeadData->Sn_Syuusei & 0x02)) {
							break;
						}
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pKjnKskEX) {
								m_pKjnKskEX->SetItemPosition(0);
							}
							else if (m_pKjnKskEX2) {	// 更正の請求/新様式対応
								m_pKjnKskEX2->SetItemPosition(0);
							}
						}
						else {
							if (m_pHjnKskEX) {
								m_pHjnKskEX->SetItemPosition(0);
							}
							else if (m_pHjnKskEX2) {	// 更正の請求/新様式対応
								m_pHjnKskEX2->SetItemPosition(0);
							}
						}
						break;
						
					//まだ未作成
					//case ID_ICSSH_TKTEI_TAB: // 特定課税仕入
					//	if (m_pTKUtiwake) {
					//		m_pTKUtiwake->SetItemPosition();
					//	}
					//	break;						
					case ID_ICSSH_KSISANURI_TAB:
						if( m_pKsisanuri ){
							m_pKsisanuri->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSISANURI2_TAB:
						if( m_pKsisanuri2 ){
							m_pKsisanuri2->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSHIIRE_TAB:
						if( m_pKshiire ){
							m_pKshiire->SetItemPosition();
						}
						break;
					//----> 20230616
					case ID_ICSSH_FHYO4_01_TAB: // 付表４－１
						if (m_pFhyo4_01) {
							m_pFhyo4_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO4_02_TAB: // 付表４－２
						if (m_pFhyo4_02) {
							m_pFhyo4_02->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_01_TAB: // 付表５ー１
						if (m_pFhyo5_01) {
							m_pFhyo5_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_012_TAB: // 付表５ー１（２）
						if (m_pFhyo5_012) {
							m_pFhyo5_012->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_02_TAB: // 付表５ー２
						if (m_pFhyo5_02) {
							m_pFhyo5_02->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO5_022_TAB: // 付表５ー２（２）
						if (m_pFhyo5_022) {
							m_pFhyo5_022->SetItemPosition();
						}
						break;
					//<---- 20230616
					default:
						break;
					}
				}
				else {
					// 経過措置対象の課税資産の譲渡等無し
					switch (m_TabMng[selno].tabtype) {
					
					case ID_ICSSH_FHYO1_01_TAB: // 付表１－１
						if (m_pFhyo1_01) {
							m_pFhyo1_01->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO2_01_TAB: // 付表２－１
						if (m_pFhyo2_01) {
							m_pFhyo2_01->SetItemPosition();
						}

						if ( m_pFhyo2_11 ) {
							m_pFhyo2_11->SetItemPosition();
						}

						//---->20230120
						if ( m_pFhyo2_11Ex ) {
							m_pFhyo2_11Ex->SetItemPosition();
						}
						//<----20230120
						break;
					case ID_ICSSH_FHYO1_03_TAB: // 付表１－３
						if ( m_pFhyo1_03 ) {
							m_pFhyo1_03->SetItemPosition();
						}
						break;

					case ID_ICSSH_FHYO2_03_TAB: // 付表２－３
						if ( m_pFhyo2_03 ) {
							m_pFhyo2_03->SetItemPosition();
						}

						//---->20230120
						if ( m_pFhyo2_03Ex ) {
							m_pFhyo2_03Ex->SetItemPosition();
						}
						//<----20230120
						break;
					//----->('23.05.01 Add)
					case ID_ICSSH_FHYO6_01_TAB:	//付表６(２割特例)
						if( m_pFhyo6_01 ) {
							m_pFhyo6_01->SetItemPosition();
						}
						break;
					//<-----
					case ID_ICSSH_IPPAN01_TAB: // 一般本表
						if (m_pShinIppan01) {
							m_pShinIppan01->SetItemPosition();
						}
						if( m_pShinIppan01EX ) {
							m_pShinIppan01EX->SetItemPosition();
						}
						//----->('23.05.01 Add)
						if( m_pShinIppan01EX2 ) {
							m_pShinIppan01EX2->SetItemPosition();
						}
						//<-----
						break;

					case ID_ICSSH_IPPAN02_TAB: // 一般本表（２）
						if (m_pShinIppan02) {
							m_pShinIppan02->SetItemPosition();
						}
						break;

					case ID_ICSSH_REFUND_KJ_01_TAB: // 還付付表①
					case ID_ICSSH_REFUND_HJ_01_TAB: // 還付付表①
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn01) {
								m_pRefundKjn01->SetInitPosition(0);
							}
						}
						else {
							if (m_pRefundHjn01) {
								m_pRefundHjn01->SetInitPosition(1);
							}
						}
						break;
					case ID_ICSSH_REFUND_KJ_02_TAB: // 還付付表②
					case ID_ICSSH_REFUND_HJ_02_TAB: // 還付付表②
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pRefundKjn02) {
								m_pRefundKjn02->SetInitPosition(2);
							}
							//----> 20230704
							if (m_pRefundKjn02EX) {
								m_pRefundKjn02EX->SetInitPosition(2);
							}
							//<---- 20230704
						}
						else {
							if (m_pRefundHjn02) {
								m_pRefundHjn02->SetInitPosition(3);
							}
							//----> 20230704
							if (m_pRefundHjn02EX) {
								m_pRefundHjn02EX->SetInitPosition(3);
							}
							//<---- 20230704
						}
						break;
					case ID_ICSSH_KSK_HJ_TAB: // 更正の請求
					case ID_ICSSH_KSK_KJ_TAB: // 更正の請求
						if (!(m_pSnHeadData->Sn_Syuusei & 0x02)) {
							break;
						}
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (m_pKjnKskEX) {
								m_pKjnKskEX->SetItemPosition(0);
							}
							else if (m_pKjnKskEX2) {	// 更正の請求/新様式対応
								m_pKjnKskEX2->SetItemPosition(0);
							}
						}
						else {
							if (m_pHjnKskEX) {
								m_pHjnKskEX->SetItemPosition(0);
							}
							else if (m_pHjnKskEX2) {	// 更正の請求/新様式対応
								m_pHjnKskEX2->SetItemPosition(0);
							}
						}
						break;
						
					//まだ未作成
					//case ID_ICSSH_TKTEI_TAB: // 特定課税仕入
					//	if (m_pTKUtiwake) {
					//		m_pTKUtiwake->SetItemPosition();
					//	}
					//	break;
						//<-- '15.08.28 INS END
					case ID_ICSSH_KSISANURI_TAB:
						if( m_pKsisanuri ){
							m_pKsisanuri->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSISANURI2_TAB:
						if( m_pKsisanuri2 ){
							m_pKsisanuri2->SetItemPosition();
						}
						break;
					case ID_ICSSH_KSHIIRE_TAB:
						if( m_pKshiire ){
							m_pKshiire->SetItemPosition();
						}
						break;
					default:
						break;
					}
				}
			}
			
			if (m_TabMng[selno].tabtype == ID_ICSSH_FHYO6_TAB) {	//付表６
				if (m_pSnHeadData->IsSoleProprietor()) {
					if (m_pFhyo6) {
						m_pFhyo6->SetItemPosition();
					}
				}
			}
		}

	}
}

/////////////////////////////////////////////////////////
//処理概要：マイナンバーの履歴を書き込む(GetData()からコピー！！)
//
//引数　　：write_sgn  0…閲覧　1…出力
/////////////////////////////////////////////////////////
void CH31HyoView::WriteNoHistory(int write_sgn)
{
	// 履歴情報作成
	NOHISTTBL nhtbl;
	NTBMOD8_InitNoHistoryTbl(0, &nhtbl);

	nhtbl.mst_code = m_pZmSub->zvol->v_cod;
	nhtbl.mst_apno = m_pZmSub->zvol->apno;

	memmove(nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()));
	memmove(nhtbl.tgt_char, m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO));

	CString mst_nam_str, kessan_str;

	mst_nam_str = nhtbl.mst_nam;
	mst_nam_str = mst_nam_str.TrimRight();
	m_pSnHeadData->GetString_KessanKikan(&kessan_str);
	strcpy_s(nhtbl.mst_nam, sizeof(nhtbl.mst_nam), mst_nam_str + kessan_str);

	strcpy_s(nhtbl.gy_nam, sizeof(nhtbl.gy_nam), _T("消費税申告書"));

	switch (write_sgn) {
	case 0: //閲覧
		nhtbl.user_play = NTBMOD_USER_PLAY_BROWSE | NTBMOD_USER_PLAY_APPLI;
		break;
	case 1: //出力
		nhtbl.user_play = NTBMOD_USER_PLAY_PRINT | NTBMOD_USER_PLAY_APPLI;
		break;
	default:
		return;
		break;
	}

	CNoHistoryTblArray nhtblary;
	nhtblary.Add(nhtbl);

	// 履歴テーブルにセット
	NTBMOD8_SetNoHistoryTbl(0, &nhtblary);

}

//-----------------------------------------------------------------------------
// 顧問先情報の取り込み
//-----------------------------------------------------------------------------
void CH31HyoView::OnUpdateKomonsakiInf(CCmdUI *pCmdUI)
{
	int	Flg = 0;
	int selno = m_MainTab.GetCurSel();

	if ((selno != -1) && m_pSnHeadData) {
		if (!(m_pSnHeadData->Sn_Sign4 & 0x01)) {
			if (m_pSnHeadData->IsMiddleProvisional()) {
				Flg = 1;
			}
			else if (m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB) {
				Flg = 1;
			}
			else if (m_TabMng[selno].tabtype == ID_ICSSH_IPPAN02_TAB) {
				Flg = 1;
			}
			else if (m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB) {
				Flg = 1;
			}
		}
		if (Flg) {
			pCmdUI->Enable(TRUE);
		}
		else {
			pCmdUI->Enable(FALSE);
		}
	}

}


void CH31HyoView::OnUpdateZeirishiInf(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL	bEnable = FALSE;
	int selno = m_MainTab.GetCurSel();

	if ((selno != -1) && (selno < (int)m_TabMng.GetCount())) {
		if ((m_TabMng[selno].tabtype == ID_ICSSH_KANI01_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_IPPAN01_TAB) ||
			(m_TabMng[selno].tabtype == ID_ICSSH_IPPAN02_TAB)|| (m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB)  ||
			(m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB) || (m_TabMng[selno].tabtype == ID_ICSSH_CHSK_TAB)) {

			if (!(m_pSnHeadData->Sn_Sign4 & 0x01)) {	// 入力ロック時はDisable harino ('14.03.12)
				if (m_SnSeq == 0) {
					bEnable = TRUE;
				}
			}
		}
	}

	if (pCmdUI) {
		pCmdUI->Enable(bEnable);
	}
}

void CH31HyoView::OnZeirishiInf()
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if (m_pShinInfo && m_pShinInfo->pParent) {
		int ret = (int)m_pShinInfo->pParent->SendMessage(WM_USER_READ_ZEIRISHI, 0);
		if (ret == 2) {//２はキャンセル
			return;
		}

		ReDispHonpyoUpper();

	}
}

//-----------------------------------------------------------------------------
// 不要になった帳票のデータをクリア
//-----------------------------------------------------------------------------
void CH31HyoView::ClearHyoData()
{
	if (m_pSnHeadData->SVmzsw == 0) {
		if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
			ClearHyoData42();
			ClearHyoData52();
			ClearHyoData522();
		}
		else {
			ClearHyoData12();
			ClearHyoData22();

			//----> 20230616 いらないかもしれないが念のためクリアしておく
			if( m_pSnHeadData->m_DispTabSgn & 0x2000 ){
				ClearHyoData42();
				ClearHyoData52();
				ClearHyoData522();
			}
			//<---- 20230616
		}
	}
}

//==================================
// データクリア　付表１－２
//==================================
void CH31HyoView::ClearHyoData12()
{
	char mony[MONY_BUF_SIZE];
	memset( mony, '\0', MONY_BUF_SIZE );

	//	m_pSyzSyukei->SetShinkokuData(_T("310101"), mony);
	//	m_pSyzSyukei->SetShinkokuData(_T("310102"), mony);
	//	m_pSyzSyukei->SetShinkokuData(_T("310103"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310201"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310202"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310203"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310303"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310401"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310402"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310403"), mony);
	//m_pSyzSyukei->SetShinkokuData(_T("310602"), mony);	// 特定収入計算表からの転記金額をクリア  '20.02.17 付表2-2の「23」の金額が付表1-2に集計されない場合があったため閉じる
	//m_pSyzSyukei->SetShinkokuData(_T("310603"), mony);	// 特定収入計算表からの転記金額をクリア  '20.02.17 付表2-2の「23」の金額が付表1-2に集計されない場合があったため閉じる
	m_pSyzSyukei->SetShinkokuData(_T("310701"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310702"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310703"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310801"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310802"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310803"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("310903"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("311001"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("311002"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("311003"), mony);
}

//==================================
// データクリア　付表２－２
//==================================
void CH31HyoView::ClearHyoData22()
{
	char	mony[6];
	memset( mony, '\0', 6 );

	m_pSyzSyukei->SetShinkokuData(_T("320101"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("320102"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("320103"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("320901"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("320902"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("320903"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321001"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321002"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321003"), mony);

	//--->yoshida190613
	m_pSyzSyukei->SetShinkokuData(_T("321101"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321102"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321103"), mony);

	m_pSyzSyukei->SetShinkokuData(_T("321201"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321202"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321203"), mony);

	m_pSyzSyukei->SetShinkokuData(_T("321301"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321302"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321303"), mony);

	m_pSyzSyukei->SetShinkokuData(_T("321401"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321402"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321403"), mony);

	//m_pSyzSyukei->SetShinkokuData(_T("321501"), mony);
	//m_pSyzSyukei->SetShinkokuData(_T("321502"), mony);
	//m_pSyzSyukei->SetShinkokuData(_T("321503"), mony);
	//<--------------

	m_pSyzSyukei->SetShinkokuData(_T("321701"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321702"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321703"), mony);

	m_pSyzSyukei->SetShinkokuData(_T("321801"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321802"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321803"), mony);

	m_pSyzSyukei->SetShinkokuData(_T("321901"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321902"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("321903"), mony);

	m_pSyzSyukei->SetShinkokuData(_T("322101"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322102"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322103"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322201"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322202"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322203"), mony);

	//  特定収入計算表からの転記金額をクリアするため、コメントを外す
	m_pSyzSyukei->SetShinkokuData(_T("322301"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322302"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322303"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322401"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322402"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322403"), mony);

	m_pSyzSyukei->SetShinkokuData(_T("322501"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322502"), mony);
	m_pSyzSyukei->SetShinkokuData(_T("322503"), mony);

	m_pSyzSyukei->SetShinkokuData ( _T( "326101" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "326102" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "326103" ), mony );

	//----> 20230120
	m_pSyzSyukei->SetShinkokuData ( _T( "328101" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "328102" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "328103" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "328201" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "328202" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "328203" ), mony );
	//<---- 20230120
}

//==================================
// データクリア　付表４－２
//==================================
void CH31HyoView::ClearHyoData42()
{
	char mony[MONY_BUF_SIZE] = { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );

	m_pSyzSyukei->SetShinkokuData ( _T( "310101" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310102" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310103" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310201" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310202" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310203" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "322501" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "322502" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "322503" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310701" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310702" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "310703" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "311001" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "311002" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "311003" ), mony );

	m_pSyzSyukei->SetShinkokuData ( _T( "340101" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340102" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340103" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340201" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340202" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340203" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340301" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340302" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340303" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340401" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340402" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340403" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340601" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340602" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340603" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340701" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340702" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340703" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340901" ), mony );	// 191205
	m_pSyzSyukei->SetShinkokuData ( _T( "340902" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "340903" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "341001" ), mony );	// 191205
	m_pSyzSyukei->SetShinkokuData ( _T( "341002" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "341003" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "341502" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "341503" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "341602" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "341603" ), mony );
}

//==================================
// データクリア　付表５－２
//==================================
void CH31HyoView::ClearHyoData52()
{
	char mony[MONY_BUF_SIZE] = { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );

	m_pSyzSyukei->SetShinkokuData ( _T( "350601" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350602" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350603" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350701" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350702" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350703" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350801" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350802" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350803" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350901" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350902" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "350903" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351001" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351002" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351003" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351101" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351102" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351103" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351201" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351202" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351203" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351401" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351402" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351403" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351501" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351502" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351503" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351601" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351602" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351603" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351701" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351702" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351703" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351801" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351802" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351803" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351901" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351902" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "351903" ), mony );
}

//==================================
// データクリア　付表５－２（２）
//==================================
void CH31HyoView::ClearHyoData522()
{
	char mony[MONY_BUF_SIZE] = { 0 };
	memset ( mony, '\0', MONY_BUF_SIZE );

	m_pSyzSyukei->SetShinkokuData ( _T( "352001" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352002" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352003" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352101" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352102" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352103" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352201" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352202" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352203" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352301" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352302" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352303" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352401" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352402" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352403" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352501" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352502" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352503" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352601" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352602" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352603" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352701" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352702" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352703" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352801" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352802" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352803" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352901" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352902" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "352903" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353001" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353002" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353003" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353101" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353102" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353103" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353201" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353202" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353203" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353301" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353302" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353303" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353401" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353402" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353403" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353501" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353502" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353503" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353601" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353602" ), mony );
	m_pSyzSyukei->SetShinkokuData ( _T( "353603" ), mony );
}



//=========================================
// 改正様式をセットする
//-----------------------------------------
// 引数
//		shrevtype	:	セットする改正様式
//=========================================
void CH31HyoView::SetShRevType ( EnumIdIcsShRevType shrevtype )
{
	m_ShRevType = shrevtype;
}

//==================================================
// 送付不要をカットする様式か
//--------------------------------------------------
// 返送値
//		true		:		送付不要をカット
//		false		:		送付不要をカットしない
//==================================================
bool CH31HyoView::IsNoDeliForm()
{
	bool bRt = false;

	ASSERT ( m_pSnHeadData );
	if ( m_pSnHeadData->IsSoleProprietor() ) {
		ASSERT ( m_pZmSub );
		ASSERT ( m_pZmSub->zvol );

		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );
		if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
			if ( ( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ) && ( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ) ) {
				bRt = true;
			}
		}
		else if ( KzHoushiki == ID_ICSSH_KANNI ) {
			if ( ( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ) || ( m_SixKindFlg && ( m_pSnHeadData->IsPrintWithMynoHonpyo() ) ) ) {
				if ( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ) {
					bRt = true;
				}
			}
		}
	}

	return bRt;
}
//===============================================
// 内部タブデータの再取得
//===============================================
int CH31HyoView::ReGetData()
{
	if ( m_pSnHeadData->IsMiddleProvisional() ) {	// 前年実績による中間申告（第26号様式）
		// 中間予定申告データ
		if ( !m_pSnChskData ) {
			m_pSnChskData = new CH31SnChskData();
			if (m_pSnChskData->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}
	}
	else {
		
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
		//BOOL	isTransitionalMeasure = FALSE;
		//if ( m_pSnHeadData->SVmzsw == 1 ){
		//	isTransitionalMeasure = TRUE;
		//}

		if ( KzHoushiki == ID_ICSSH_KANNI ) {
			// 本表データクラス
			if ( !m_pSnHonpyoData ) {
				m_pSnHonpyoData = new CH31SnHonpyoData();
				if( m_pSnHonpyoData->GetData( m_pZmSub, m_SnSeq ) ){
					return -1;
				}
			}
			if ( !m_pSnHonpyo2Data ) {
				m_pSnHonpyo2Data = new CH31SnHonpyo2Data();
				if( m_pSnHonpyo2Data->GetData( m_pZmSub, m_SnSeq ) ){
					return -1;
				}
			}

			if( m_pSnHeadData->SVmzsw == 1 ){	// 経過措置あり
				// 付表４－２
				if (!m_pSnFhyo4_02Data) {
					m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data(TRUE);
					if( m_pSnFhyo4_02Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}

				// 付表４－１
				if (!m_pSnFhyo4_01Data) {
					m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(FALSE);
					if( m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}

				// 付表５－２
				if (!m_pSnFhyo5_02Data) {
					m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data(TRUE);
					if( m_pSnFhyo5_02Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				if (!m_pSnFhyo5_022Data) {
					m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data(TRUE);
					if( m_pSnFhyo5_022Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}

				// 付表５－１
				if (!m_pSnFhyo5_01Data) {
					m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(FALSE);
					if( m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}

				if (!m_pSnFhyo5_012Data) {
					m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data(FALSE);
					if( m_pSnFhyo5_012Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			else {	// 経過措置なし
				// 付表４
				if (!m_pSnFhyo4_01Data) {
					m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(FALSE);
					if( m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				// 付表５
				if (!m_pSnFhyo5_01Data) {
					m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(FALSE);
					if( m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				if (!m_pSnFhyo5_012Data) {
					m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data(FALSE);
					if( m_pSnFhyo5_012Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
		}
		else if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
			// 本表データクラス
			if ( !m_pSnHonpyoData ) {
				m_pSnHonpyoData = new CH31SnHonpyoData();
				if( m_pSnHonpyoData->GetData( m_pZmSub, m_SnSeq ) ){
					return -1;
				}
			}
			if ( !m_pSnHonpyo2Data ) {
				m_pSnHonpyo2Data = new CH31SnHonpyo2Data();
				if( m_pSnHonpyo2Data->GetData( m_pZmSub, m_SnSeq ) ){
					return -1;
				}
			}

			if( m_pSnHeadData->SVmzsw == 1 ){	// 経過措置あり

				//----> 20230616
				if( m_pSnHeadData->m_DispTabSgn & 0x2000 ){
					// 付表４－２
					if (!m_pSnFhyo4_02Data) {
						m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data(TRUE);
						if( m_pSnFhyo4_02Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}

					// 付表４－１
					if (!m_pSnFhyo4_01Data) {
						m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(FALSE);
						if( m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}

					// 付表５－２
					if (!m_pSnFhyo5_02Data) {
						m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data(TRUE);
						if( m_pSnFhyo5_02Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					if (!m_pSnFhyo5_022Data) {
						m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data(TRUE);
						if( m_pSnFhyo5_022Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}

					// 付表５－１
					if (!m_pSnFhyo5_01Data) {
						m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(FALSE);
						if( m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}

					if (!m_pSnFhyo5_012Data) {
						m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data(FALSE);
						if( m_pSnFhyo5_012Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
				}
				//<---- 20230616
				else{
					// 付表１－２
					if (!m_pSnFhyo1_02Data) {
						m_pSnFhyo1_02Data = new CH31SnFhyo1_02Data(TRUE);
						if (m_pSnFhyo1_02Data->GetData(m_pZmSub, m_SnSeq)) {
							return -1;
						}
					}

					// 付表１－１
					if ( !m_pSnFhyo1_01Data ) {
						m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data(FALSE);
						if( m_pSnFhyo1_01Data->GetData( m_pZmSub, m_SnSeq ) ){
							return -1;
						}
					}

					// 付表２－２
					if (!m_pSnFhyo2_02Data) {
						m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data(TRUE);
						if (m_pSnFhyo2_02Data->GetData(m_pZmSub, m_SnSeq)) {
							return -1;
						}
					}

					// 付表２－１
					if ( !m_pSnFhyo2_01Data ) {
						m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data(FALSE);
						if( m_pSnFhyo2_01Data->GetData( m_pZmSub, m_SnSeq ) ){
							return -1;
						}
					}

					// 棚卸
					if (!m_pSnTanaData) {
						m_pSnTanaData = new CH31SnTanaData(TRUE);
						if (m_pSnTanaData->GetData(m_pZmSub, m_SnSeq)) {
							return -1;
						}
					}
				}
			}
			else {	// 経過措置なし
				// 付表１
				if ( !m_pSnFhyo1_01Data ) {
					m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data(FALSE);
					if( m_pSnFhyo1_01Data->GetData( m_pZmSub, m_SnSeq ) ){
						return -1;
					}
				}
				// 付表２
				if ( !m_pSnFhyo2_01Data ) {
					m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data(FALSE);
					if( m_pSnFhyo2_01Data->GetData( m_pZmSub, m_SnSeq ) ){
						return -1;
					}
				}
				// 棚卸
				if (!m_pSnTanaData) {
					m_pSnTanaData = new CH31SnTanaData(FALSE);
					if (m_pSnTanaData->GetData(m_pZmSub, m_SnSeq)) {
						return -1;
					}
				}
			}
		}
	}

	// 還付
	if ( !m_pSnRefundData ) {
		m_pSnRefundData = new CH31SnRefundData();
		if (m_pSnRefundData->GetData( m_pZmSub, m_pSnHeadData, m_SnSeq )) {
			return -1;
		}
		// 情報設定
		ResetRefundData();
	}
	// 付表６
	if (m_pSnHeadData->Sn_Syuusei & 0x10) {
		if ( !m_pSnFhyo6Data ) {
			m_pSnFhyo6Data = new CH31SnFhyo6Data();
			if ( m_pSnFhyo6Data->GetData( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
		}
	}
	
	//２割特例
	if (m_pSnHeadData->m_DispTabSgn & 0x2000) {
		if (!m_pSnFhyo6_01Data) {
			//2023.6.30現在、付表6において旧税率有無で処理を分けている部分はないため、FALSEを渡す
			m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(FALSE);
			if (m_pSnFhyo6_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}
	}


	return 0;
}

//===============================================
// 更正の請求用 申告書金額計算 (実部)
//-----------------------------------------------
// 引数
//		pCalqedMony1	:	集計後金額の保存先1
//		pCalqedMony2	:	集計後金額の保存先2
//		Zei				:	税額
//-----------------------------------------------
// 返送値
//		0				:	正常終了
//===============================================
int CH31HyoView::CalqSyzForKskSub ( void *pCalqedMony1, void *pCalqedMony2, char *Zei )
{
	SH_ANS	*ANS = (SH_ANS*)pCalqedMony1;

	memset( ANS, '\0', sizeof(SH_ANS)*30 );

	int Kazeihoushiki = m_Util.GetKazeihoushiki(m_pZmSub->zvol);

	memmove( ANS[0].val, m_pSnHonpyoData->Sn_KSTD, 6 );
	memmove( ANS[1].val, m_pSnHonpyoData->Sn_SYTX, 6 );
	memmove( ANS[2].val, m_pSnHonpyoData->Sn_KJADJZ, 6 );
	memmove( ANS[3].val, m_pSnHonpyoData->Sn_SIREZ, 6 );
	memmove( ANS[4].val, m_pSnHonpyoData->Sn_HKANZ, 6 );
	memmove( ANS[5].val, m_pSnHonpyoData->Sn_KSITZ, 6 );
	memmove( ANS[6].val, m_pSnHonpyoData->Sn_KJZSK, 6 );
	memmove( ANS[7].val, m_pSnHonpyoData->Sn_KJHKZ, 6 );
	memmove( ANS[8].val, m_pSnHonpyoData->Sn_SHZ, 6 );
	memmove( ANS[9].val, m_pSnHonpyoData->Sn_TYNOFZ, 6 );
	memmove( ANS[10].val, m_pSnHonpyoData->Sn_EDNOFZ, 6 );
	memmove( ANS[11].val, m_pSnHonpyoData->Sn_TYNKNP, 6 );
	memmove( ANS[12].val, m_pSnHonpyoData->Sn_KAKTIZ, 6 );
	memmove( ANS[13].val, m_pSnHonpyoData->Sn_SHN, 6 );
	memmove( ANS[14].val, m_pSnHonpyoData->Sn_KZURI, 6 );
	if(Kazeihoushiki == ID_ICSSH_GENSOKU)
		memmove( ANS[15].val, m_pSnHonpyoData->Sn_SOURI, 6 );
	else if(Kazeihoushiki == ID_ICSSH_KANNI)
		memmove( ANS[15].val, m_pSnHonpyoData->Sn_ZZURI, 6 );
	memmove( ANS[16].val, m_pSnHonpyoData->Sn_TKJHKZ, 6 );
	memmove( ANS[17].val, m_pSnHonpyoData->Sn_TSHZ, 6 );
	memmove( ANS[18].val, m_pSnHonpyoData->Sn_KNP, 6 );
	memmove( ANS[19].val, m_pSnHonpyoData->Sn_NOZ, 6 );
	memmove( ANS[20].val, m_pSnHonpyoData->Sn_TTYWAR, 6 );
	memmove( ANS[21].val, m_pSnHonpyoData->Sn_TEDNOF, 6 );
	memmove( ANS[22].val, m_pSnHonpyoData->Sn_TYNKNPJ, 6 );
	memmove( ANS[23].val, m_pSnHonpyoData->Sn_TKAKTZ, 6 );
	memmove( ANS[24].val, m_pSnHonpyoData->Sn_SHNJ, 6 );
	memmove( ANS[25].val, m_pSnHonpyoData->Sn_SYOTSG, 6 );

	return 0;
}

//===============================================
// データ読込み(金額集計用)
//===============================================
int CH31HyoView::ReadDataForCalq()
{
	if (!m_pSnHonpyoData) {
		m_pSnHonpyoData = new CH31SnHonpyoData();
		if (m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq)) {
			return -1;
		}
	}

	return 0;
}

//===============================================
// 本表の計算
//===============================================
void CH31HyoView::AlreadyDecisionCalc31()
{
	//yoshida190619
	//既確定税額の次回申告の為の準備

	// 国税の既確定税額の算出
	if (m_Arith.l_test(m_pSnHonpyoData->Sn_EDNOFZ) > 0) {//納付金額ある場合
		memmove(m_pSnHonpyoData->Sn_KAKTIZ, m_pSnHonpyoData->Sn_EDNOFZ, 6);
	}
	else {//還付
		//memmove(m_pSnHonpyoData->Sn_KAKTIZ, m_pSnHonpyoData->Sn_TYNKNP, 6);
		m_Arith.l_add(m_pSnHonpyoData->Sn_KAKTIZ, m_pSnHonpyoData->Sn_KJHKZ, m_pSnHonpyoData->Sn_TYNKNP);
		m_Arith.l_neg(m_pSnHonpyoData->Sn_KAKTIZ);
	}

	// 地方税の既確定譲渡割額の算出
	if (m_Arith.l_test(m_pSnHonpyoData->Sn_TEDNOF) > 0) {//納付金額ある場合
		memmove(m_pSnHonpyoData->Sn_TKAKTZ, m_pSnHonpyoData->Sn_TEDNOF, 6);
	}
	else {//還付
		//memmove(m_pSnHonpyoData->Sn_TKAKTZ, m_pSnHonpyoData->Sn_TYNKNPJ, 6);
		m_Arith.l_add(m_pSnHonpyoData->Sn_TKAKTZ, m_pSnHonpyoData->Sn_KNP, m_pSnHonpyoData->Sn_TYNKNPJ);
		m_Arith.l_neg(m_pSnHonpyoData->Sn_TKAKTZ);
	}



	/*if (m_pZmSub) {
		CString	szFilter;
		szFilter.Format(_T("Sn_SEQ=%d"), m_SnSeq);
		if (m_pZmSub->szsnk == NULL) {
			m_pZmSub->SzsnkOpen(szFilter);
		}
		else {
			m_pZmSub->szsnk->Requery(szFilter);
		}
		if (m_pZmSub->szsnk->st != -1) {
			char	monyBuf1[32] = { 0 }, monyBuf2[32] = { 0 };
			m_pZmSub->szsnk->Edit();
			m_Util.val_to_asci(&m_pZmSub->szsnk->Sn_KAKTIZ, m_pSnHonpyoData->Sn_KAKTIZ);
			m_Util.val_to_asci(&m_pZmSub->szsnk->Sn_TKAKTZ, m_pSnHonpyoData->Sn_TKAKTZ);
			m_pZmSub->szsnk->Update();
		}
	}*/

}

//===============================================
// メモリ確保
//===============================================
void CH31HyoView::ReMemory()
{
	// 課税方式チェック
	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );

	if (KzHoushiki == ID_ICSSH_KANNI) {
		// 簡易課税

		// 付表４－１データ
		if ( m_pSnFhyo4_01Data == NULL ) {
			m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data ( TRUE );
		}

		if ( m_pSnHeadData->SVmzsw ==1 ) {
			// 付表４－２データ
			if ( m_pSnFhyo4_02Data == NULL ) {
				m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data ( TRUE );
			}
		}

		// 付表５－１データ
		if ( m_pSnFhyo5_01Data == NULL ) {
			m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data ( TRUE );
		}

		// 付表５－１（２）データ
		if ( m_pSnFhyo5_012Data == NULL ) {
			m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data ( TRUE );
		}

		if ( m_pSnHeadData->SVmzsw ==1 ) {
			// 付表５－２データ
			if ( m_pSnFhyo5_02Data == NULL ) {
				m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data ( TRUE );
			}

			// 付表５－２（２）データ
			if ( m_pSnFhyo5_022Data == NULL ) {
				m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data ( TRUE );
			}
		}

		// 付表６データ(２割特例)
		if ( m_pSnHeadData->m_DispTabSgn & 0x2000 ) {
			if ( m_pSnFhyo6_01Data == NULL ) {
				m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(TRUE);
			}
		}
	}
	else {
		// 原則課税

		//----> 20230616
		if( m_pSnHeadData->m_DispTabSgn & 0x2000 & m_pSnHeadData->SVmzsw ){
			// 付表４－１データ
			if ( m_pSnFhyo4_01Data == NULL ) {
				m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data ( TRUE );
			}

			// 付表４－２データ
			if ( m_pSnFhyo4_02Data == NULL ) {
				m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data ( TRUE );
			}

			// 付表５－１データ
			if ( m_pSnFhyo5_01Data == NULL ) {
				m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data ( TRUE );
			}

			// 付表５－１(２)データ
			if ( m_pSnFhyo5_012Data == NULL ) {
				m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data ( TRUE );
			}

			// 付表５－２データ
			if ( m_pSnFhyo5_02Data == NULL ) {
				m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data ( TRUE );
			}

			// 付表５－２(２)データ
			if ( m_pSnFhyo5_022Data == NULL ) {
				m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data ( TRUE );
			}
		}
		//<---- 20230616
		else{
			// 付表１－１データ
			if ( m_pSnFhyo1_01Data == NULL ) {
				m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data ( TRUE );
			}

			if ( m_pSnHeadData->SVmzsw ==1 ) {
				// 付表１－２データ
				if ( m_pSnFhyo1_02Data == NULL ) {
					m_pSnFhyo1_02Data = new CH31SnFhyo1_02Data ( TRUE );
				}
			}

			// 付表２－１データ
			if ( m_pSnFhyo2_01Data == NULL ) {
				m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data ( TRUE );
			}

			if ( m_pSnHeadData->SVmzsw ==1 ) {
				// 付表２－２データ
				if ( m_pSnFhyo2_02Data == NULL ) {
					m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data ( TRUE );
				}
			}
		}

		// 付表６データ(２割特例)
		if ( m_pSnHeadData->m_DispTabSgn & 0x2000 ) {
			if ( m_pSnFhyo6_01Data == NULL ) {
				m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(TRUE);
			}
		}

		// 棚卸データ
		if ( m_pSnTanaData == NULL ) {
			m_pSnTanaData = new CH31SnTanaData ( TRUE );
		}
	}

	if( m_pSnHeadData->IsMiddleProvisional() == FALSE ){
		// 課税標準額等の内訳書
		if( m_pSnHonpyo2Data == NULL ){
			m_pSnHonpyo2Data = new CH31SnHonpyo2Data();
		}
	}
}

//===========================================
// 還付金融機関入力にフォーカス誘導
//-------------------------------------------
// 引数
//		dsInput		:		移動先
//-------------------------------------------
// 返送値
//			0		:		正常終了
//			-1		:		エラー
//============================================
int CH31HyoView::GuideKanpuKinyu ( int dsInput )
{
	if ( m_pSnHeadData->IsMiddleProvisional() ) {	// 前年実績による中間申告(第26号様式)
		return 0;
	}
	else {
		if ( dsInput == ID_SYZ_MOVE_TO_HONPYO_BANK ) {
			EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );
			if ( KzHoushiki == ID_ICSSH_KANNI ) {
				int max = ( int )m_TabMng.GetCount();
				int page = -1;
				for ( int idx = 0; idx < max; idx++ ) {
					if ( m_TabMng[idx].tabtype == ID_ICSSH_KANI01_TAB ) {
						page = idx;
						break;
					}
				}

				if ( page != -1 ) {
					m_MainTab.SetCurSel ( page );

					if ( m_pShinKani01 ) {
						m_pShinKani01->SetKanpuKinyuInput();
					}
					if( m_pShinKani01EX ) {
						m_pShinKani01EX->SetKanpuKinyuInput();
					}
					//----->('23.05.01 Add)
					if( m_pShinKani01EX2 ) {
						m_pShinKani01EX2->SetKanpuKinyuInput();
					}
					//<-----
				}
			}
			else if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
				int max = ( int )m_TabMng.GetCount();
				int page = -1;
				for ( int idx = 0; idx < max; idx++ ) {
					if ( m_TabMng[idx].tabtype == ID_ICSSH_IPPAN01_TAB ) {
						page = idx;
						break;
					}
				}

				if ( page != -1 ) {
					m_MainTab.SetCurSel ( page );

					if ( m_pShinIppan01 ) {
						m_pShinIppan01->SetKanpuKinyuInput();
					}
					if( m_pShinIppan01EX ) {
						m_pShinIppan01EX->SetKanpuKinyuInput();
					}
					//----->('23.05.01 Add)
					if( m_pShinIppan01EX2 ) {
						m_pShinIppan01EX2->SetKanpuKinyuInput();
					}
					//<-----
				}
			}
		}
		else if ( dsInput == ID_SYZ_MOVE_TO_KOUSEI_BANK ) {
			if ( m_pSnHeadData->Sn_Syuusei & 0x02 ) {
				int setTabtype = ID_ICSSH_KSK_HJ_TAB;
				if ( m_pSnHeadData->IsSoleProprietor() ) {
					setTabtype = ID_ICSSH_KSK_KJ_TAB;
				}

				int max = ( int )m_TabMng.GetCount();
				int page = -1;
				for ( int idx = 0; idx < max; idx++ ) {
					if ( m_TabMng[idx].tabtype == setTabtype ) {
						page = idx;
						break;
					}
				}

				if ( page != -1 ) {
					m_MainTab.SetCurSel ( page );
					if ( m_pSnHeadData->IsSoleProprietor() ) {
						if ( m_pKjnKskEX ) {
							m_pKjnKskEX->SetKanpuKinyuInput();
						}
						else if ( m_pKjnKskEX2 ) {	// 更正の請求/新様式対応
							m_pKjnKskEX2->SetKanpuKinyuInput();
						}
					}
					else {
						if ( m_pHjnKskEX ) {
							m_pHjnKskEX->SetKanpuKinyuInput();
						}
						else if ( m_pHjnKskEX2 ) {	// 更正の請求/新様式対応
							m_pHjnKskEX2->SetKanpuKinyuInput();
						}
					}
				}
			}
		}
	}
	return 0;
}


//-----------------------------------------------------------------------------
// 特定課税仕入と課税売上割合のチェック
//-----------------------------------------------------------------------------
// 引数	srctab		：	呼び出し元ページ（0:基本情報, 1:本表・付表, 2:課売・特定収入）
//		calltype	：	何の処理で呼び出しているか（1:印刷, 2:プレビュー, 3:処理終了）
//-----------------------------------------------------------------------------
// 返送値	0		：	処理継続可能
//			-1		：	処理継続不可
//-----------------------------------------------------------------------------
int CH31HyoView::ChkTkkzAndKzwari(int srctab, int calltype)
{
	int nRt = 0;

	int aSt = IsAbleTkKz();
	if (aSt == -1) {
		int st = ChangeTkkzUse(srctab, calltype);
		if ((srctab == 1) && (calltype == 3)) {
			if (st == 1) {
				// タブ移動無し
				SetItemPosition(m_MainTab.GetCurSel());
			}
		}
		nRt = -1;
	}
	else if (aSt == -2) {
		if (ChkZkTkkzReturn(srctab, calltype) == 0) {
			nRt = -1;
		}
	}


	return nRt;
}

//-----------------------------------------------------------------------------
// 課税売上割合が95%以上？('15.09.10)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	95%以上
//			FALSE	：	95%未満
//-----------------------------------------------------------------------------
BOOL CH31HyoView::IsRatio95Over()
{
	BOOL bRt = FALSE;

	/*if (m_pSnFhyo2_01Data) {
		if (m_pSnFhyo2_01Data->Sn_F201_8EX >= 9500) {
			bRt = TRUE;
		}
	}
	else if (m_pSnFhyo2_02Data) {
		if (m_pSnFhyo2_02Data->Sn_F202_8EX >= 9500) {
			bRt = TRUE;
		}
	}*/

	if (m_pFhyo2_01) {
		if( m_pFhyo2_01->GetCalqedKzwari() >= 9500 ){
			bRt = TRUE;
		}
	}
	if ( m_pFhyo2_11 ) {
		if ( m_pFhyo2_11->GetCalqedKzwari() >= 9500 ) {
			bRt = TRUE;
		}
	}
	if ( m_pFhyo2_03 ) {
		if ( m_pFhyo2_03->GetCalqedKzwari() >= 9500 ) {
			bRt = TRUE;
		}
	}

	//---->20230120
	if ( m_pFhyo2_11Ex ) {
		if ( m_pFhyo2_11Ex->GetCalqedKzwari() >= 9500 ) {
			bRt = TRUE;
		}
	}
	if ( m_pFhyo2_03Ex ) {
		if ( m_pFhyo2_03Ex->GetCalqedKzwari() >= 9500 ) {
			bRt = TRUE;
		}
	}
	//<----20230120

	return bRt;
}

//-----------------------------------------------------------------------------
// データの引き継ぎ
//-----------------------------------------------------------------------------
// 引数	srcVer	：	引き継ぎ元バージョン
//		snSeq	：	履歴シーケンス
//		pZmsub	：	公益クラス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::TakeOverData(int srcVer, int snSeq, CDBNpSub *pZmsub)
{
	if( m_pSnHonpyoData ==NULL ){
		if( (m_pSnHonpyoData = new CH31SnHonpyoData()) == NULL ){
			return -1;
		}
		if( m_pSnHonpyoData->GetData(pZmsub, snSeq) ) {
			return -1;
		}
	}
	//'19.11.07
	else {
		if (m_pSnHonpyoData->GetData(pZmsub, snSeq)) {
			return -1;
		}
	}

	if( m_pSnHonpyoData ) {
		m_pSnHonpyoData->TakeOverData(pZmsub, snSeq, m_pSnHeadData, srcVer);
		m_pSnHonpyoData->SetData(pZmsub, snSeq);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 初期化完了フラグの切り替え
//-----------------------------------------------------------------------------
// 引数	flg	：	セットするフラグ
//-----------------------------------------------------------------------------
void CH31HyoView::SetInitedFlg(bool flg)
{
	m_isInited = flg;
}



//-----------------------------------------------------------------------------
// 課税取引金額計算書から転記時のフォーカス移動
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::MoveFocusInTaxTenki()
{
	int max = ( int )m_TabMng.GetCount();
	int page = -1;

	for ( int i = 0; i < max; i++ ) {
		if ( ( m_TabMng[i].tabtype == ID_ICSSH_FHYO4_01_TAB ) || ( m_TabMng[i].tabtype == ID_ICSSH_FHYO4_03_TAB ) ) {
			page = i;
			break;
		}
	}

	if ( page != -1 ) {
		if( m_pFhyo4_01 ) {
			m_pFhyo4_01->MoveFocusInTaxTenki();
		}
		if ( m_pFhyo4_03 ) {
			m_pFhyo4_03->MoveFocusInTaxTenki();
		}
	}

	return 0;
}

int CH31HyoView::Message_BlankKinyu_Kousei()
{
	long smd = 0, emd = 0;
	GetKazeiKikan( smd, emd );
	if( emd >= R04REFORM && m_pSnHeadData->Sn_GenericSgn & 0x1000 ){
		return 0;
	}

	//if( m_pHjnKskEX || m_pKjnKskEX ){
	if( m_pHjnKskEX || m_pKjnKskEX || m_pHjnKskEX2 || m_pKjnKskEX2 ){	// 更正の請求/新様式対応


		char tmp_char1[128]={0},tmp_char2[128]={0},tmp_char3[128]={0},tmp_char4[128]={0},tmp_char5[128]={0};
		CString	msg;
		BOOL blank1 = FALSE,blank2 = FALSE,blank3 = FALSE,blank4 = FALSE,blank5 = FALSE; //空白判定

		if( !m_pSnKskData->KsData.KSbpsw ){	// 銀行

			// 銀行等名称
			memmove( tmp_char1, m_pSnKskData->KsData.KSbank.Bname, sizeof( m_pSnKskData->KsData.KSbank.Bname ));
			if( m_Util.MixSpaceCutLength(tmp_char1, 20) == 0 ){
				blank1 = TRUE;
			}
			// 本支店名称
			memmove( tmp_char2, m_pSnKskData->KsData.KSbank.Sname, sizeof( m_pSnKskData->KsData.KSbank.Sname ));
			if( m_Util.MixSpaceCutLength(tmp_char2, 20) == 0 ){
				if(( m_pSnKskData->KsData.KSbank.Skbn != 1 )&&( m_pSnKskData->KsData.KSbank.Skbn != 3 )){ //本店、本所以外
					blank2 = TRUE;
				}
			}
			// 預金
			memmove( tmp_char3, m_pSnKskData->KsData.KSbank.Dname, sizeof( m_pSnKskData->KsData.KSbank.Dname ));
			if( m_Util.MixSpaceCutLength(tmp_char3, 12) == 0 ){
				blank3 = TRUE;
			}
			// 口座番号
			memmove( tmp_char4, m_pSnKskData->KsData.KSbank.Accnt, sizeof( m_pSnKskData->KsData.KSbank.Accnt ));
			if( m_Util.MixSpaceCutLength(tmp_char4, 10) == 0 ){
				blank4 = TRUE;
			}

			if( blank1&&blank2&&blank3&&blank4){
				msg = _T("還付申告ですが、還付される税金の受取場所（更正の請求）が登録されていません！");
				msg += _T("\n還付される税金の受取場所を入力しますか？");
			}else
			if( blank1||blank2||blank3||blank4){
				msg = _T("還付される税金の受取場所（更正の請求）の登録において、以下の内容を再確認してください。\n");
				if(blank1){
					msg += _T("\n銀行名（入力されていません）");
				}
				if(blank2){
					msg += _T("\n支店名（入力されていません）");
				}
				if(blank3){
					msg += _T("\n預金種別（入力されていません）");
				}
				if(blank4){
					msg += _T("\n口座番号（入力されていません）");
				}

				msg += _T("\n\n還付される税金の受取場所を入力しますか？");
			}

		}else{ //郵便貯金
			memmove( tmp_char1, m_pSnKskData->KsData.KSpost.Acnt1, sizeof(m_pSnKskData->KsData.KSpost.Acnt1) );
			memmove( tmp_char2, m_pSnKskData->KsData.KSpost.Acnt2, sizeof(m_pSnKskData->KsData.KSpost.Acnt2) );
			memmove( tmp_char3, m_pSnKskData->KsData.KSpost.Pname, sizeof(m_pSnKskData->KsData.KSpost.Pname) );

			if( m_Util.MixSpaceCutLength(tmp_char1, 5 ) == 0 ){
				blank1 = TRUE;
			}
			if( m_Util.MixSpaceCutLength(tmp_char2, 8 ) == 0 ){
				blank2 = TRUE;
			}
			if( m_Util.MixSpaceCutLength(tmp_char3, 16 ) == 0 ){
				blank3 = TRUE;
			}

			if((blank1||blank2)&&blank3){ //「ゆうちょ銀行の貯金記号番号」と「郵便局名等」のいずれかの入力があればメッセージを出さない
				msg = _T("還付申告ですが、還付される税金の受取場所（更正の請求）が登録されていません！");
				msg += _T("\n還付される税金の受取場所を入力しますか？");
			}
		}

		if( msg.IsEmpty() == FALSE ){
			return ICSMessageBox(msg, MB_YESNO, 0);
		}

	}
	
	return 0;
}

void CH31HyoView::EditOff_Fuhyo6()
{
	if ( m_pFhyo6 ) {
		m_pFhyo6->OnEditOFFCallBack ( m_pFhyo6->m_Focus );
	}
}

//=================================
// 付表６ 金額チェック
//=================================
int CH31HyoView::Message_NofuKanpuGokei()
{
	CString msg;

	if ( m_pFhyo6 ) {
		int ret = m_pFhyo6->NofuKanpuGokeiCheck();
		if ( ret ) {
			//msg += _T( "付表６\n" );
			long	symd=0,eymd=0;
			GetKazeiKikan(symd,eymd);
			// インボイス施工日以降は付表７に名称変更（24/05/07_依頼No.168738）
			if(eymd >= ICS_SH_INVOICE_DAY)	{
				msg += _T( "付表７\n" );
			}
			else	{
				msg += _T( "付表６\n" );
			}
			msg += _T( "下記の金額が一致していません。金額を確認してください。\n\n" );
			if ( ret & 0x01 ) {
				msg += _T( "⑨の各人の納付税額(消費税)の合計金額と②の納める消費税\n" );
			}
			if ( ret & 0x02 ) {
				msg += _T( "⑩の各人の納付税額(地方消費税)の合計金額と③の納める地方消費税\n" );
			}
			if ( ret & 0x04 ) {
				msg += _T( "⑫の各人の還付税額(消費税)の合計金額と⑤の還付される消費税\n" );
			}
			if ( ret & 0x08 ) {
				msg += _T( "⑬の各人の還付税額(地方消費税)の合計金額と⑥の還付される地方消費税\n" );
			}

			if ( msg.IsEmpty() == FALSE ) {
				return ICSMessageBox ( msg, MB_YESNO, 0 );
			}
		}
	}

	return 0;
}

//================================
// 付表６ マイナンバーチェック
//================================
int CH31HyoView::Message_MyNumber_error()
{
	if ( m_pFhyo6 ) {
		int ret = m_pFhyo6->MyNumber_CheckFromDisp();
		if ( ret < 0 ) {
			return -1;
		}		
	}

	return 0;
}

//================================
// 付表６ マイナンバーチェック
//================================
int CH31HyoView::Message_MyNumber_overlap()
{
	if ( m_pFhyo6 ) {
		int ret = m_pFhyo6->MyNumberCheck();
		if ( ret == -1 ) {
			return ret;
		}		
	}

	return 0;
}

//================================
// 付表６ 相続割合チェック
//================================
int CH31HyoView::Message_SozokuWariai()
{
	if ( m_pFhyo6 ) {
		int ret = m_pFhyo6->SozokubunCheck();
		if ( ret == -1 ) {
			return ret;
		}
	}

	return 0;
}

//---------------------------------------------
// 付表６ 相続人情報の取得
//---------------------------------------------
// 引数		nohfuinfo	:	相続人情報
//---------------------------------------------
// 返送値	0			:	正常終了
//			-1			:	エラー
//---------------------------------------------
int CH31HyoView::GetSozokuInfo( NOHFUSHO_INFO *nohfuinfo, NOHFUSHO_DATA *nohfudata, CArray<int> *SozokuNoAry )
{
	int ret = 0;

	if( m_pFhyo6 ){
		ret = m_pFhyo6->GetSozokuInfo( nohfuinfo, nohfudata, SozokuNoAry );
	}

	return ret;
}

//yoshida190909
//-----------------------------------------------------------------------------
// 手入力サイン　データベース書き込み関数
//-----------------------------------------------------------------------------
// 引数	tag		：タグID　（未使用。後にID単位で書き込む用に作成しておく）
//		mode	：処理モード（未使用）
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CH31HyoView::WriteDBManualInputSign(CString &tag, int mode)
{
	// 金額がある場合のみ手入力サインON
	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {	// 原則課税
		if (m_pSnHeadData->SVmzsw) {

			//----> 20230616
			if( m_pSnHeadData->m_DispTabSgn & 0x2000 ){

				if (m_pSnFhyo4_02Data == NULL) {
					if ((m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data(TRUE)) == NULL) {
						return -1;
					}
					if (m_pSnFhyo4_02Data->GetData(m_pZmSub, m_SnSeq)) {
						return -1;
					}
				}

				//---->19.10.23
				//付表５消費税額も連動から非連動切替で金額かわることがあるので手入力サインをONにさせる。
				if (m_pSnFhyo5_02Data == NULL) {
					if ((m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data(TRUE)) == NULL) {
						return -1;
					}
					if (m_pSnFhyo5_02Data->GetData(m_pZmSub, m_SnSeq)) {
						return -1;
					}
				}
				//<-------------

				if (m_pSnFhyo4_02Data) {
					//m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x888;
					// 消費税額
					if (m_Arith.l_test(m_pSnFhyo4_02Data->Sn_F402_2A)) {
						m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x08;
					}

					if (m_Arith.l_test(m_pSnFhyo4_02Data->Sn_F402_2B)) {
						m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x80;
					}

					if (m_Arith.l_test(m_pSnFhyo4_02Data->Sn_F402_2C)) {
						m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x800;
					}
					m_pSnFhyo4_02Data->SetData(m_pZmSub, m_SnSeq);
				}

				//---->19.10.23
				if (m_pSnFhyo5_02Data) {
					// 事業区分別　消費税額  2割特例時は第二種のみ
					if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15A)) {
						m_pSnFhyo5_02Data->Sn_F502_15sw |= 0x08;
					}
					if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15B)) {
						m_pSnFhyo5_02Data->Sn_F502_15sw |= 0x80;
					}
					if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15C)) {
						m_pSnFhyo5_02Data->Sn_F502_15sw |= 0x800;
					}
					m_pSnFhyo5_02Data->SetData(m_pZmSub, m_SnSeq);
				}
				//<------------

				if (m_pSnFhyo4_01Data == NULL) {
					if ((m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(TRUE)) == NULL) {
						return -1;
					}
					if (m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq)) {
						return -1;
					}
				}
				//---->19.10.23
				//付表５消費税額も連動から非連動切替で金額かわることがあるので手入力サインをONにさせる。
				if (m_pSnFhyo5_01Data == NULL) {
					if ((m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(TRUE)) == NULL) {
						return -1;
					}
					if (m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq)) {
						return -1;
					}
				}
				//<-------------

				if (m_pSnFhyo4_01Data) {
					//m_pSnFhyo4_01Data->Sn_F401_2sw |= 0x880;
					// 消費税額
					if (m_Arith.l_test(m_pSnFhyo4_01Data->Sn_F401_2D)) {
						m_pSnFhyo4_01Data->Sn_F401_2sw |= 0x80;
					}

					if (m_Arith.l_test(m_pSnFhyo4_01Data->Sn_F401_2E)) {
						m_pSnFhyo4_01Data->Sn_F401_2sw |= 0x800;
					}
					m_pSnFhyo4_01Data->SetData(m_pZmSub, m_SnSeq);
				}

				//---->19.10.23
				if (m_pSnFhyo5_01Data) {
					// 事業区分別　消費税額
					//第二種
					if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15D)) {
						m_pSnFhyo5_01Data->Sn_F501_15sw |= 0x80;
					}
					if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15E)) {
						m_pSnFhyo5_01Data->Sn_F501_15sw |= 0x800;
					}
					m_pSnFhyo5_01Data->SetData(m_pZmSub, m_SnSeq);
				}
			}
			//<---- 20230616
			else{
				if (m_pSnFhyo2_02Data == NULL) {
					if ((m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data(TRUE)) == NULL) {
						return -1;
					}
					if (m_pSnFhyo2_02Data->GetData(m_pZmSub, m_SnSeq)) {
						return -1;
					}
				}

				if (m_pSnFhyo1_02Data == NULL) {
					if ((m_pSnFhyo1_02Data = new CH31SnFhyo1_02Data(TRUE)) == NULL) {
						return -1;
					}
					if (m_pSnFhyo1_02Data->GetData(m_pZmSub, m_SnSeq)) {
						return -1;
					}
				}

				// 付表２－１、２－２の「課税仕入れに係る消費税額」の部分。手入力サインON
				if (m_pSnFhyo2_02Data) {
					//m_pSnFhyo2_02Data->Sn_F202_10sw |= 0x888;
					if ( m_Arith.l_test ( m_pSnFhyo2_02Data->Sn_F202_10A ) ) {
						m_pSnFhyo2_02Data->Sn_F202_10sw |= 0x08;
					}

					if ( m_Arith.l_test ( m_pSnFhyo2_02Data->Sn_F202_10B ) ) {
						m_pSnFhyo2_02Data->Sn_F202_10sw |= 0x80;
					}

					if ( m_Arith.l_test ( m_pSnFhyo2_02Data->Sn_F202_10C ) ) {
						m_pSnFhyo2_02Data->Sn_F202_10sw |= 0x800;
					}

					//----> 20230120
					if( m_Arith.l_test( m_pSnFhyo2_02Data->Sn_F202_28A ) ){
						m_pSnFhyo2_02Data->Sn_F202_28sw |= 0x08;
					}
					if( m_Arith.l_test( m_pSnFhyo2_02Data->Sn_F202_28B ) ){
						m_pSnFhyo2_02Data->Sn_F202_28sw |= 0x80;
					}
					if( m_Arith.l_test( m_pSnFhyo2_02Data->Sn_F202_28C ) ){
						m_pSnFhyo2_02Data->Sn_F202_28sw |= 0x800;
					}
					//<---- 20230120

					// 特定課税仕入れに係る消費税額
					if ( m_Arith.l_test ( m_pSnFhyo2_02Data->Sn_F202_12C ) ) {
						m_pSnFhyo2_02Data->Sn_F202_12sw |= 0x800;
					}

					m_pSnFhyo2_02Data->SetData(m_pZmSub, m_SnSeq);
				}
			}

			if (m_pSnFhyo1_02Data) {
				//m_pSnFhyo1_02Data->Sn_F102_2c |= 0x888;

				// 消費税額
				if ( m_Arith.l_test ( m_pSnFhyo1_02Data->Sn_F102_2A ) ) {
					m_pSnFhyo1_02Data->Sn_F102_2c |= 0x08;
				}

				if ( m_Arith.l_test ( m_pSnFhyo1_02Data->Sn_F102_2B ) ) {
					m_pSnFhyo1_02Data->Sn_F102_2c |= 0x80;
				}

				if ( m_Arith.l_test ( m_pSnFhyo1_02Data->Sn_F102_2C ) ) {
					m_pSnFhyo1_02Data->Sn_F102_2c |= 0x800;
				}

				m_pSnFhyo1_02Data->SetData(m_pZmSub, m_SnSeq);
			}
		}

		if (m_pSnFhyo2_01Data == NULL) {
			if ((m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo2_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}

		if (m_pSnFhyo1_01Data == NULL) {
			if ((m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo1_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}

		if (m_pSnFhyo6_01Data == NULL) {
			if ((m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo6_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}


		if (m_pSnFhyo2_01Data) {
			//m_pSnFhyo2_01Data->Sn_F201_10sw |= 0x880;
			// 課税仕入れに係る消費税額
			if ( m_Arith.l_test ( m_pSnFhyo2_01Data->Sn_F201_10D ) ) {
				m_pSnFhyo2_01Data->Sn_F201_10sw |= 0x80;
			}

			if ( m_Arith.l_test ( m_pSnFhyo2_01Data->Sn_F201_10E ) ) {
				m_pSnFhyo2_01Data->Sn_F201_10sw |= 0x800;
			}

			//----> 20230120
			if( m_Arith.l_test( m_pSnFhyo2_01Data->Sn_F201_28D ) ){
				m_pSnFhyo2_01Data->Sn_F201_28sw |= 0x80;
			}
			if( m_Arith.l_test( m_pSnFhyo2_01Data->Sn_F201_28E ) ){
				m_pSnFhyo2_01Data->Sn_F201_28sw |= 0x800;
			}
			//<---- 20230120

			// 特定課税仕入れに係る消費税額
			if ( m_Arith.l_test ( m_pSnFhyo2_01Data->Sn_F201_12E ) ) {
				m_pSnFhyo2_01Data->Sn_F201_12sw |= 0x800;
			}

			m_pSnFhyo2_01Data->SetData(m_pZmSub, m_SnSeq);
		}

		if (m_pSnFhyo1_01Data) {
			//m_pSnFhyo1_01Data->Sn_F101_2c |= 0x880;
			// 消費税額
			if ( m_Arith.l_test ( m_pSnFhyo1_01Data->Sn_F101_2D ) ) {
				m_pSnFhyo1_01Data->Sn_F101_2c |= 0x80;
			}

			if ( m_Arith.l_test ( m_pSnFhyo1_01Data->Sn_F101_2E ) ) {
				m_pSnFhyo1_01Data->Sn_F101_2c |= 0x800;
			}

			m_pSnFhyo1_01Data->SetData(m_pZmSub, m_SnSeq);
		}
		if (m_pSnFhyo6_01Data) {

			if (m_Arith.l_test(m_pSnFhyo6_01Data->Sn_F601_3A)) {
				m_pSnFhyo6_01Data->Sn_F601_3sw |= 0x80;
			}

			if (m_Arith.l_test(m_pSnFhyo6_01Data->Sn_F601_3B)) {
				m_pSnFhyo6_01Data->Sn_F601_3sw |= 0x800;
			}

			m_pSnFhyo6_01Data->SetData(m_pZmSub, m_SnSeq);
		}


	}
	else {//簡易課税

		if (m_pSnHeadData->SVmzsw) {
			if (m_pSnFhyo4_02Data == NULL) {
				if ((m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo4_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}

			//---->19.10.23
			//付表５消費税額も連動から非連動切替で金額かわることがあるので手入力サインをONにさせる。
			if (m_pSnFhyo5_02Data == NULL) {
				if ((m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data(TRUE)) == NULL) {
					return -1;
				}
				if (m_pSnFhyo5_02Data->GetData(m_pZmSub, m_SnSeq)) {
					return -1;
				}
			}
			//<-------------



			if (m_pSnFhyo4_02Data) {
				//m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x888;
				// 消費税額
				if (m_Arith.l_test(m_pSnFhyo4_02Data->Sn_F402_2A)) {
					m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x08;
				}

				if (m_Arith.l_test(m_pSnFhyo4_02Data->Sn_F402_2B)) {
					m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x80;
				}

				if (m_Arith.l_test(m_pSnFhyo4_02Data->Sn_F402_2C)) {
					m_pSnFhyo4_02Data->Sn_F402_2sw |= 0x800;
				}

				m_pSnFhyo4_02Data->SetData(m_pZmSub, m_SnSeq);
			}

			//---->19.10.23
			if (m_pSnFhyo5_02Data) {
				// 事業区分別　消費税額
				//第一種
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14A)) {
					m_pSnFhyo5_02Data->Sn_F502_14sw |= 0x08;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14B)) {
					m_pSnFhyo5_02Data->Sn_F502_14sw |= 0x80;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_14C)) {
					m_pSnFhyo5_02Data->Sn_F502_14sw |= 0x800;
				}

				//第二種
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15A)) {
					m_pSnFhyo5_02Data->Sn_F502_15sw |= 0x08;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15B)) {
					m_pSnFhyo5_02Data->Sn_F502_15sw |= 0x80;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_15C)) {
					m_pSnFhyo5_02Data->Sn_F502_15sw |= 0x800;
				}

				//第三種
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16A)) {
					m_pSnFhyo5_02Data->Sn_F502_16sw |= 0x08;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16B)) {
					m_pSnFhyo5_02Data->Sn_F502_16sw |= 0x80;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_16C)) {
					m_pSnFhyo5_02Data->Sn_F502_16sw |= 0x800;
				}

				//第四種
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17A)) {
					m_pSnFhyo5_02Data->Sn_F502_17sw |= 0x08;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17B)) {
					m_pSnFhyo5_02Data->Sn_F502_17sw |= 0x80;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_17C)) {
					m_pSnFhyo5_02Data->Sn_F502_17sw |= 0x800;
				}

				//第五種
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18A)) {
					m_pSnFhyo5_02Data->Sn_F502_18sw |= 0x08;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18B)) {
					m_pSnFhyo5_02Data->Sn_F502_18sw |= 0x80;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_18C)) {
					m_pSnFhyo5_02Data->Sn_F502_18sw |= 0x800;
				}

				//第六種
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19A)) {
					m_pSnFhyo5_02Data->Sn_F502_19sw |= 0x08;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19B)) {
					m_pSnFhyo5_02Data->Sn_F502_19sw |= 0x80;
				}
				if (m_Arith.l_test(m_pSnFhyo5_02Data->Sn_F502_19C)) {
					m_pSnFhyo5_02Data->Sn_F502_19sw |= 0x800;
				}

				m_pSnFhyo5_02Data->SetData(m_pZmSub, m_SnSeq);

			}
			//<------------

		}

		if (m_pSnFhyo4_01Data == NULL) {
			if ((m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo4_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}
		//---->19.10.23
		//付表５消費税額も連動から非連動切替で金額かわることがあるので手入力サインをONにさせる。
		if (m_pSnFhyo5_01Data == NULL) {
			if ((m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo5_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}
		//<-------------

		if (m_pSnFhyo6_01Data == NULL) {
			if ((m_pSnFhyo6_01Data = new CH31SnFhyo6_2wariData(TRUE)) == NULL) {
				return -1;
			}
			if (m_pSnFhyo6_01Data->GetData(m_pZmSub, m_SnSeq)) {
				return -1;
			}
		}


		if (m_pSnFhyo4_01Data) {
			//m_pSnFhyo4_01Data->Sn_F401_2sw |= 0x880;
			// 消費税額
			if (m_Arith.l_test(m_pSnFhyo4_01Data->Sn_F401_2D)) {
				m_pSnFhyo4_01Data->Sn_F401_2sw |= 0x80;
			}

			if (m_Arith.l_test(m_pSnFhyo4_01Data->Sn_F401_2E)) {
				m_pSnFhyo4_01Data->Sn_F401_2sw |= 0x800;
			}

			m_pSnFhyo4_01Data->SetData(m_pZmSub, m_SnSeq);
		}

		//---->19.10.23
		if (m_pSnFhyo5_01Data) {
			// 事業区分別　消費税額
			//第一種
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14D)) {
				m_pSnFhyo5_01Data->Sn_F501_14sw |= 0x80;
			}
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_14E)) {
				m_pSnFhyo5_01Data->Sn_F501_14sw |= 0x800;
			}


			//第二種
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15D)) {
				m_pSnFhyo5_01Data->Sn_F501_15sw |= 0x80;
			}
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_15E)) {
				m_pSnFhyo5_01Data->Sn_F501_15sw |= 0x800;
			}

			//第三種
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16D)) {
				m_pSnFhyo5_01Data->Sn_F501_16sw |= 0x80;
			}
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_16E)) {
				m_pSnFhyo5_01Data->Sn_F501_16sw |= 0x800;
			}

			//第四種
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17D)) {
				m_pSnFhyo5_01Data->Sn_F501_17sw |= 0x80;
			}
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_17E)) {
				m_pSnFhyo5_01Data->Sn_F501_17sw |= 0x800;
			}

			//第五種
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18D)) {
				m_pSnFhyo5_01Data->Sn_F501_18sw |= 0x80;
			}
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_18E)) {
				m_pSnFhyo5_01Data->Sn_F501_18sw |= 0x800;
			}

			//第六種
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19D)) {
				m_pSnFhyo5_01Data->Sn_F501_19sw |= 0x80;
			}
			if (m_Arith.l_test(m_pSnFhyo5_01Data->Sn_F501_19E)) {
				m_pSnFhyo5_01Data->Sn_F501_19sw |= 0x800;
			}

			m_pSnFhyo5_01Data->SetData(m_pZmSub, m_SnSeq);
		}

		if (m_pSnFhyo6_01Data) {

			if (m_Arith.l_test(m_pSnFhyo6_01Data->Sn_F601_3A)) {
				m_pSnFhyo6_01Data->Sn_F601_3sw |= 0x80;
			}

			if (m_Arith.l_test(m_pSnFhyo6_01Data->Sn_F601_3B)) {
				m_pSnFhyo6_01Data->Sn_F601_3sw |= 0x800;
			}

			m_pSnFhyo6_01Data->SetData(m_pZmSub, m_SnSeq);
		}
	
	}
	return 0;
}

//======================================
// 特定収入計算表ダイアログ表示
//--------------------------------------
// 引数
//		type	:	0 = 経過措置なし
//					1 = 経過措置あり
//======================================
int CH31HyoView::SpecPassDlg ( int type )
{
	if ( m_pShinInfo && m_pShinInfo->pParent ) {
		m_pShinInfo->pParent->SendMessage ( WM_USER_DISP_SPECPASS, type );
	}

	return 0;
}

//=======================================
// 控除対象仕入税額の再計算
//=======================================
int CH31HyoView::RecalqSkj ( SYZTOSPC_DATA *pSyzToSpc )
{
	ASSERT ( pSyzToSpc );
	if ( pSyzToSpc == NULL ) {
		return -1;
	}

	if ( ( m_pShinInfo == NULL ) || ( m_pShinInfo->pParent == NULL ) ) {
		return -1;
	}

	m_pShinInfo->pParent->SendMessage ( WM_USER_SKJPOST_RECALQ, ( WPARAM )pSyzToSpc );

	return 0;
}



//'19.10.28
//-----------------------------------------------------------------------------
// 予定申告の月数クリア('17.02.09)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31HyoView::ClearYoteiMonth()
{
	if (m_pSnChskData) {
		m_pSnChskData->Sn_BBMON = 0;
		m_pSnChskData->Sn_BSMON = 0;
	}

	if (m_pZmSub) {
		CString	szFilter;
		szFilter.Format(_T("Sn_SEQ=%d"), m_SnSeq);
		if (m_pZmSub->szsnk == NULL) {
			m_pZmSub->SzsnkOpen(szFilter);
		}
		else {
			m_pZmSub->szsnk->Requery(szFilter);
		}
		if (m_pZmSub->szsnk->st != -1) {
			m_pZmSub->szsnk->Edit();
			m_pZmSub->szsnk->Sn_BBMON = 0;
			m_pZmSub->szsnk->Sn_BSMON = 0;
			m_pZmSub->szsnk->Update();
		}
	}

	return 0;
}

//=====================================
// 付表６　マイナンバーあり
//-------------------------------------
// 返送値
//		0		:		なし
//		1		:		あり
//=====================================
int CH31HyoView::IsExist_MyNumber_Fuhyo6()
{
	int ret = 0;

	if ( m_pFhyo6 ) {
		if ( m_pFhyo6->MyNumber_IsAllEmpty() == TRUE ) {
			ret = 0;
		}
		else{
			ret = 1;
		}
	}

	return ret;
}

//=========================================================
// 実額入力項目あり？
//---------------------------------------------------------
// 返送値
//		TRUE		:		実額入力項目あり
//		FALSE		:		実額入力項目なし
//=========================================================
BOOL CH31HyoView::IsManualInputSign()
{
	BOOL ret = FALSE;

	if ( ( ( m_pSnHeadData->Sn_Sign4 & 0x80 ) == 0x00 ) || ( m_pSnHeadData->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );

	if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
		// 原則課税

		// 付表1-1
		if ( m_pFhyo1_01 ) {
			if ( m_pFhyo1_01->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表1-2
		if ( m_pFhyo1_02 ) {
			if ( m_pFhyo1_02->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表1-3
		if ( m_pFhyo1_03 ) {
			if ( m_pFhyo1_03->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表2-1
		if ( m_pFhyo2_01 ) {
			if ( m_pFhyo2_01->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表2-2
		if ( m_pFhyo2_02 ) {
			if ( m_pFhyo2_02->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表2-3
		if ( m_pFhyo2_03 ) {
			if ( m_pFhyo2_03->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		//---->20230120
		// 付表2-3
		if ( m_pFhyo2_03Ex ) {
			if ( m_pFhyo2_03Ex->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}
		//<----20230120

		// 付表2-1
		if ( m_pFhyo2_11 ) {
			if ( m_pFhyo2_11->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表2-2
		if ( m_pFhyo2_12 ) {
			if ( m_pFhyo2_12->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		//---->20230120
		// 付表2-1
		if ( m_pFhyo2_11Ex ) {
			if ( m_pFhyo2_11Ex->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表2-2
		if ( m_pFhyo2_12Ex ) {
			if ( m_pFhyo2_12Ex->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}
		//<----20230120
		//----->('23.05.01 Add)
		// 付表6(２割特例)
		if ( m_pFhyo6_01 ) {
			if (m_pFhyo6_01->IsManualInputSign() == TRUE) {
				ret = TRUE;
			}
		}
		//<-----

		//----> 20230616
		if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
			// 付表4-1
			if ( m_pFhyo4_01 ) {
				if ( m_pFhyo4_01->IsManualInputSign() == TRUE ) {
					ret = TRUE;
				}
			}

			// 付表4-2
			if ( m_pFhyo4_02 ) {
				if ( m_pFhyo4_02->IsManualInputSign() == TRUE ) {
					ret = TRUE;
				}
			}

			// 付表5-1
			if ( m_pFhyo5_01 ) {
				if ( m_pFhyo5_01->IsManualInputSign() == TRUE ) {
					ret = TRUE;
				}
			}

			// 付表5-1(2)
			if ( m_pFhyo5_012 ) {
				if ( m_pFhyo5_012->IsManualInputSign() == TRUE ) {
					ret = TRUE;
				}
			}

			// 付表5-2
			if ( m_pFhyo5_02 ) {
				if ( m_pFhyo5_02->IsManualInputSign() == TRUE ) {
					ret = TRUE;
				}
			}

			// 付表5-2(2)
			if ( m_pFhyo5_022 ) {
				if ( m_pFhyo5_022->IsManualInputSign() == TRUE ) {
					ret = TRUE;
				}
			}
		}
		//<---- 20230616
	}
	else {
		// 簡易課税
		
		
		// 付表4-1
		if ( m_pFhyo4_01 ) {
			if ( m_pFhyo4_01->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表4-2
		if ( m_pFhyo4_02 ) {
			if ( m_pFhyo4_02->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表4-3
		if ( m_pFhyo4_03 ) {
			if ( m_pFhyo4_03->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表5-1
		if ( m_pFhyo5_01 ) {
			if ( m_pFhyo5_01->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表5-1(2)
		if ( m_pFhyo5_012 ) {
			if ( m_pFhyo5_012->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表5-2
		if ( m_pFhyo5_02 ) {
			if ( m_pFhyo5_02->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表5-2(2)
		if ( m_pFhyo5_022 ) {
			if ( m_pFhyo5_022->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表5-3
		if ( m_pFhyo5_03 ) {
			if ( m_pFhyo5_03->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}

		// 付表5-3(2)
		if ( m_pFhyo5_032 ) {
			if ( m_pFhyo5_032->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}
		//----->('23.05.01 Add)
		// 付表6(２割特例)
		if ( m_pFhyo6_01 ) {
			if ( m_pFhyo6_01->IsManualInputSign() == TRUE) {
				ret = TRUE;
			}
		}
		//<-----
	}

	// 付表6
	if ( ( m_pSnHeadData->IsSoleProprietor() ) && ( m_pSnHeadData->Sn_Syuusei & 0x10 ) ) {
		if ( m_pFhyo6 ) {
			if ( m_pFhyo6->IsManualInputSign() == TRUE ) {
				ret = TRUE;
			}
		}
	}

	return ret;
}

//=========================================================
// 新様式 ( 旧税率を適用した取引なし )表示？
//---------------------------------------------------------
// 返送値
//		TRUE		:		新様式
//		FALSE		:		旧様式
//=========================================================
BOOL CH31HyoView::IsNoneMTForm()
{
	BOOL ret = FALSE;

	int cnt = m_MainTab.GetItemCount();
	if ( cnt > 0 ) {
		for ( int i = 0; i < cnt; i++ ) {
			if ( ( m_TabMng[i].tabtype == ID_ICSSH_FHYO1_03_TAB ) || ( m_TabMng[i].tabtype == ID_ICSSH_FHYO2_03_TAB ) ||
				 ( m_TabMng[i].tabtype == ID_ICSSH_FHYO4_03_TAB ) || ( m_TabMng[i].tabtype == ID_ICSSH_FHYO5_03_TAB ) || ( m_TabMng[i].tabtype == ID_ICSSH_FHYO5_032_TAB ) ||
				 ( m_TabMng[i].tabtype == ID_ICSSH_FHYO6_01_TAB ) ) {
				ret = TRUE;
			}
		}
	}

	return ret;
}

int CH31HyoView::InitOldFormTab ( BOOL bDelete )
{
	if ( bDelete ) {
		// 内部クラスを破棄？
		DeleteInsideDlg();
		// 全削除
		m_MainTab.DeleteAllItems();
	}

	ICSDialog	*ptmpTab[16] = {0};
	SH_TABMNG	shTabmng;
	int			firstPage = 0;

	int	tabIdx = 0;


	BOOL	isTransitionalMeasure = FALSE;

	// 非連動サイン
	char flg[MONY_BUF_SIZE] = { 0 };
	if ( m_pSnHeadData->Sn_Sign4 & 0x80 ) {
		m_Arith.l_input ( flg, _T( "1" ) );
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FFFFFC" ), flg );


	// 経過措置サイン
	char keikaflg[MONY_BUF_SIZE] = { 0 };
	if ( m_pSnHeadData->SVmzsw == 1 ) {
		m_Arith.l_input ( keikaflg, _T( "1" ) );
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FFFFFB" ), keikaflg );

	// 令和2年4月1日以降の課税期間であれば、納税者有利になるよう計算
	long eymd = 0;
	char adv[MONY_BUF_SIZE] = { 0 };
	if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if ( eymd >= TAXADVANTAGE ) {
		m_Arith.l_input ( adv, _T( "1" ) );
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF4000" ), adv );

	// 令和2年8月1日以降の課税期間または、修正申告・更正の請求であれば、簡易課税の計算を修正
	memset ( adv, '\0', sizeof ( adv ) );
	if ( eymd >= TAXMODIFY ) {
		m_Arith.l_input ( adv, _T( "1" ) );
	}
	else {
		if ( ( ( m_pSnHeadData->Sn_SKKBN == 3 ) || ( m_pSnHeadData->Sn_SKKBN == 4 ) ) || ( m_pSnHeadData->Sn_Syuusei & 0x02 ) ) {
			m_Arith.l_input ( adv, _T( "1" ) );
		}
	}
	m_pSyzSyukei->SetShinkokuData ( _T( "FF5000" ), adv );

	// 旧様式
	memset ( adv, '\0', sizeof ( adv ) );
	m_pSyzSyukei->SetShinkokuData ( _T( "FF6000" ), adv );

	memset ( &shTabmng, '\0', sizeof ( SH_TABMNG ) );
	if ( m_pSnHeadData->IsMiddleProvisional() ) {	// 前年実績による中間申告 ( 第26号様式 )
		// 本表データ
		m_pSnHonpyoData = new CH31SnHonpyoData();
		if ( m_pSnHonpyoData->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		// 中間予定申告データ
		if ( !m_pSnChskData ) {
			m_pSnChskData = new CH31SnChskData();
			if ( m_pSnChskData->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		//										予定申告（２６号）										   //
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		if ( !m_pChskEX ) {
			m_pChskEX = new CShinChskEX ( this );
			m_pChskEX->InitInfo ( m_pSnHeadData, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo );
			m_pChskEX->Create ( IDD_DIALOG_CHSK_EX, &m_MainTab );
		}
		m_MainTab.InsertItem ( tabIdx, _T( "中間申告(第26号)" ), m_pChskEX->m_hWnd );
		ptmpTab[tabIdx] = m_pChskEX;
		tabIdx++;
		shTabmng.tabtype = ID_ICSSH_CHSK_TAB;
		shTabmng.pWnd = m_pChskEX;
		m_TabMng.Add ( shTabmng );
	}
	else {
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );

		// 本表データ
		m_pSnHonpyoData = new CH31SnHonpyoData();
		if ( m_pSnHonpyoData->GetData ( m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		m_pSnHonpyo2Data = new CH31SnHonpyo2Data();
		if ( m_pSnHonpyo2Data->GetData(m_pZmSub, m_SnSeq ) ) {
			return -1;
		}

		//特例チェックONの場合、特例計算帳票表示
		//基本情報画面の特例チェックをまず判断する。チェック無い場合は表示しない
		if ( m_pSnHeadData->m_DispTabSgn & 0x80 ) {
			if ( m_pSnHeadData->m_DispTabSgn & 0x10 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//			第５-(１)様式 課税試算の譲渡等の対価の額の計算表（軽減売上割合１０営業日）			   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pKsisanuri ) {
					m_pKsisanuri = new CShinKSisanUri ( this );
					m_pKsisanuri->InitInfo ( &m_pSnHeadData, m_pShinInfo, &m_Arith, m_pZmSub, &m_pSyzSyukei, m_SnSeq );
					m_pKsisanuri->Create ( IDD_DIALOG_KSISAN_URI, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "第５-(１)号様式" ), m_pKsisanuri->m_hWnd );
				ptmpTab[tabIdx] = m_pKsisanuri;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSISANURI_TAB;
				shTabmng.pWnd = m_pKsisanuri;
				m_TabMng.Add ( shTabmng );
			}

			if ( m_pSnHeadData->m_DispTabSgn & 0x20 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//			第５-(２)様式 課税資産の譲渡等の対価の額の計算表（小売等軽減仕入割合）				　 //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pKsisanuri2 ) {
					m_pKsisanuri2 = new CShinKsisan_Uri2 ( this );
					m_pKsisanuri2->InitInfo ( &m_pSnHeadData, m_pShinInfo, &m_Arith, m_pZmSub, &m_pSyzSyukei, m_SnSeq );
					m_pKsisanuri2->Create ( IDD_DIALOG_KSISAN_URI2, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "第５-(２)号様式" ), m_pKsisanuri2->m_hWnd );
				ptmpTab[tabIdx] = m_pKsisanuri2;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSISANURI2_TAB;
				shTabmng.pWnd = m_pKsisanuri2;
				m_TabMng.Add ( shTabmng );
			}

			if ( m_pSnHeadData->m_DispTabSgn & 0x40 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//						第５-(３)様式 課税仕入れ等の税額の計算表								   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pKshiire ) {
					m_pKshiire = new CShinKshiire ( this );
					m_pKshiire->InitInfo ( &m_pSnHeadData, m_pShinInfo, &m_Arith, m_pZmSub, &m_pSyzSyukei, m_SnSeq );
					m_pKshiire->Create ( IDD_DIALOG_KSHIIRE, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "第５-(３)号様式" ), m_pKshiire->m_hWnd );
				ptmpTab[tabIdx] = m_pKshiire;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSHIIRE_TAB;
				shTabmng.pWnd = m_pKshiire;
				m_TabMng.Add ( shTabmng );
			}
		}

		if ( KzHoushiki == ID_ICSSH_KANNI ) {	// 簡易課税

			// 付表４データ
			m_pSnFhyo4_01Data = new CH31SnFhyo4_01Data ( TRUE );
			if (m_pSnFhyo4_01Data->GetData ( m_pZmSub, m_SnSeq )) {
				return -1;
			}
			m_pSnFhyo4_02Data = new CH31SnFhyo4_02Data ( TRUE );
			if ( m_pSnFhyo4_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}

			// 付表５データ
			m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data(TRUE);
			if ( m_pSnFhyo5_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
			m_pSnFhyo5_012Data = new CH31SnFhyo5_012Data ( TRUE );
			if ( m_pSnFhyo5_012Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
			m_pSnFhyo5_02Data = new CH31SnFhyo5_02Data ( TRUE );
			if ( m_pSnFhyo5_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
			m_pSnFhyo5_022Data = new CH31SnFhyo5_022Data ( TRUE );
			if ( m_pSnFhyo5_022Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}

			// 経過措置対象の課税資産の譲渡等あり
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//										付表４－０２											   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pFhyo4_02 ) {
					m_pFhyo4_02 = new CShinFhyo4_02 ( this );
					m_pFhyo4_02->InitInfo ( &m_pSnFhyo4_02Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith );
					m_pFhyo4_02->Create(IDD_DIALOG_FHY4_02, &m_MainTab);
				}
				m_MainTab.InsertItem ( tabIdx, _T( "付表４－２" ), m_pFhyo4_02->m_hWnd );
				ptmpTab[tabIdx] = m_pFhyo4_02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_FHYO4_02_TAB;
				shTabmng.pWnd = m_pFhyo4_02;
				m_TabMng.Add ( shTabmng );
			}


			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										付表４－０１											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pFhyo4_01 ) {
				m_pFhyo4_01 = new CShinFhyo4_01 ( this );
				m_pFhyo4_01->InitInfo ( &m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith );
				m_pFhyo4_01->Create ( IDD_DIALOG_FHY4_01, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "付表４－１" ), m_pFhyo4_01->m_hWnd );
			ptmpTab[tabIdx] = m_pFhyo4_01;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO4_01_TAB;
			shTabmng.pWnd = m_pFhyo4_01;
			m_TabMng.Add ( shTabmng );


			// 経過措置対象の課税資産の譲渡等あり
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//										付表５－０２											   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pFhyo5_02 ) {
					m_pFhyo5_02 = new CShinFhyo5_02 ( this );
					m_pFhyo5_02->InitInfo ( &m_pSnFhyo5_02Data, &m_pSnHeadData, &m_Arith, m_pZmSub, &m_pSyzSyukei );
					m_pFhyo5_02->Create ( IDD_DIALOG_FHY5_02, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "付表５－２" ), m_pFhyo5_02->m_hWnd );
				ptmpTab[tabIdx] = m_pFhyo5_02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_FHYO5_02_TAB;
				shTabmng.pWnd = m_pFhyo5_02;
				m_TabMng.Add ( shTabmng );

				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//										付表５－０２(2)											   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pFhyo5_022 ) {
					m_pFhyo5_022 = new CShinFhyo5_022 ( this );
					m_pFhyo5_022->InitInfo ( &m_pSnFhyo5_022Data, &m_pSnHeadData, &m_Arith, &m_pSyzSyukei );
					m_pFhyo5_022->Create ( IDD_DIALOG_FHY5_022, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "付表５－２(2)" ), m_pFhyo5_022->m_hWnd );
				ptmpTab[tabIdx] = m_pFhyo5_022;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_FHYO5_022_TAB;
				shTabmng.pWnd = m_pFhyo5_022;
				m_TabMng.Add ( shTabmng );
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										付表５－０１											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pFhyo5_01 ) {
				m_pFhyo5_01 = new CShinFhyo5_01 ( this );
				m_pFhyo5_01->InitInfo ( &m_pSnFhyo5_01Data, &m_pSnHeadData, &m_Arith, &m_pSyzSyukei );
				m_pFhyo5_01->Create ( IDD_DIALOG_FHY5_01, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "付表５－１" ), m_pFhyo5_01->m_hWnd );
			ptmpTab[tabIdx] = m_pFhyo5_01;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO5_01_TAB;
			shTabmng.pWnd = m_pFhyo5_01;
			m_TabMng.Add ( shTabmng );

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										付表５－０１(2)											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pFhyo5_012 ) {
				m_pFhyo5_012 = new CShinFhyo5_012 ( this );
				m_pFhyo5_012->InitInfo ( &m_pSnFhyo5_012Data, &m_pSnHeadData, &m_pSyzSyukei, &m_Arith );
				m_pFhyo5_012->Create ( IDD_DIALOG_FHY5_012, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "付表５－１(2)" ), m_pFhyo5_012->m_hWnd );
			ptmpTab[tabIdx] = m_pFhyo5_012;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO5_012_TAB;
			shTabmng.pWnd = m_pFhyo5_012;
			m_TabMng.Add ( shTabmng );

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										簡易本表												   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pShinKani01 ) {
				m_pShinKani01 = new CShinKani01 ( this );
				m_pShinKani01->InitInfo ( &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei );
				m_pShinKani01->Create ( IDD_DIALOG_KANI, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "簡易本表" ), m_pShinKani01->m_hWnd );
			ptmpTab[tabIdx] = m_pShinKani01;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_KANI01_TAB;
			shTabmng.pWnd = m_pShinKani01;
			m_TabMng.Add ( shTabmng );

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//								課税標準額等の内訳書											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pShinIppan02 ) {
				m_pShinIppan02 = new CShinIppan02 ( this );
				m_pShinIppan02->InitInfo ( &m_pSnHeadData, &m_pSnHonpyo2Data, &m_Arith, m_pShinInfo, m_pZmSub, &m_pSyzSyukei );
				m_pShinIppan02->Create ( IDD_DIALOG_IPPAN02, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "課税標準額等の内訳書" ), m_pShinIppan02->m_hWnd );
			ptmpTab[tabIdx] = m_pShinIppan02;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_IPPAN02_TAB;
			shTabmng.pWnd = m_pShinIppan02;
			m_TabMng.Add ( shTabmng );
		}
		else {	// 原則課税

			// 付表１データ
			m_pSnFhyo1_01Data = new CH31SnFhyo1_01Data ( TRUE );
			if (m_pSnFhyo1_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
			m_pSnFhyo1_02Data = new CH31SnFhyo1_02Data ( TRUE );
			if ( m_pSnFhyo1_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}

			// 付表２データ
			m_pSnFhyo2_01Data = new CH31SnFhyo2_01Data ( TRUE );
			if ( m_pSnFhyo2_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
			m_pSnFhyo2_02Data = new CH31SnFhyo2_02Data ( TRUE );
			if ( m_pSnFhyo2_02Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}

			// 棚卸データ
			m_pSnTanaData = new CH31SnTanaData ( TRUE );
			if ( m_pSnTanaData->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}


			if ( m_pSnHeadData->SVmzsw == 1 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//										付表２－０２											   //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pFhyo2_02 ) {
					m_pFhyo2_02 = new CShinFhyo2_02 ( this );
					m_pFhyo2_02->InitInfo ( &m_pSnFhyo2_02Data, &m_pSnHeadData, &m_pSnTanaData, &m_Arith, m_pShinInfo, &m_pSyzSyukei );
					m_pFhyo2_02->Create ( IDD_DIALOG_FHY2_02, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "付表２－２" ), m_pFhyo2_02->m_hWnd );
				ptmpTab[tabIdx] = m_pFhyo2_02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_FHYO2_02_TAB;
				shTabmng.pWnd = m_pFhyo2_02;
				m_TabMng.Add ( shTabmng );
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										付表２－０１											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pFhyo2_01 ) {
				m_pFhyo2_01 = new CShinFhyo2_01 ( this );
				m_pFhyo2_01->InitInfo ( &m_pSnFhyo2_01Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnTanaData, &m_pSyzSyukei );
				m_pFhyo2_01->Create ( IDD_DIALOG_FHY2_01, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "付表２－１" ), m_pFhyo2_01->m_hWnd );
			ptmpTab[tabIdx] = m_pFhyo2_01;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO2_01_TAB;
			shTabmng.pWnd = m_pFhyo2_01;
			m_TabMng.Add ( shTabmng );

			// 経過措置対象の課税資産の譲渡等あり
			if ( m_pSnHeadData->SVmzsw == 1 ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				//										付表１－０２											　 //
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
				if ( !m_pFhyo1_02 ) {
					m_pFhyo1_02 = new CShinFhyo1_02 ( this );
					m_pFhyo1_02->InitInfo ( &m_pSnFhyo1_02Data, &m_pSnHeadData, m_pShinInfo, &m_Arith, &m_pSyzSyukei );
					m_pFhyo1_02->Create ( IDD_DIALOG_FHY1_02, &m_MainTab);
				}
				m_MainTab.InsertItem ( tabIdx, _T( "付表１－２" ), m_pFhyo1_02->m_hWnd );
				ptmpTab[tabIdx] = m_pFhyo1_02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_FHYO1_02_TAB;
				shTabmng.pWnd = m_pFhyo1_02;
				m_TabMng.Add ( shTabmng );
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										付表１－０１											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pFhyo1_01 ) {
				m_pFhyo1_01 = new CShinFhyo1_01 ( this );
				m_pFhyo1_01->InitInfo ( &m_pSnFhyo1_01Data, &m_pSnHeadData, m_pShinInfo , &m_Arith, &m_pSyzSyukei );
				m_pFhyo1_01->Create ( IDD_DIALOG_FHY1_01, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "付表１－１" ), m_pFhyo1_01->m_hWnd );
			ptmpTab[tabIdx] = m_pFhyo1_01;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO1_01_TAB;
			shTabmng.pWnd = m_pFhyo1_01;
			m_TabMng.Add ( shTabmng );

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										一般本表１												   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pShinIppan01 ) {
				m_pShinIppan01 = new CShinIppan01 ( this );
				m_pShinIppan01->InitInfo ( &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pShinInfo, &m_pSyzSyukei );
				m_pShinIppan01->Create ( IDD_DIALOG_IPPAN01, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "一般本表" ), m_pShinIppan01->m_hWnd );
			ptmpTab[tabIdx] = m_pShinIppan01;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_IPPAN01_TAB;
			shTabmng.pWnd = m_pShinIppan01;
			m_TabMng.Add ( shTabmng );

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//								課税標準額等の内訳書											   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pShinIppan02 ) {
				m_pShinIppan02 = new CShinIppan02 ( this );
				m_pShinIppan02->InitInfo( &m_pSnHeadData, &m_pSnHonpyo2Data, &m_Arith, m_pShinInfo, m_pZmSub, &m_pSyzSyukei );
				m_pShinIppan02->Create ( IDD_DIALOG_IPPAN02, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "課税標準額等の内訳書" ), m_pShinIppan02->m_hWnd );
			ptmpTab[tabIdx] = m_pShinIppan02;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_IPPAN02_TAB;
			shTabmng.pWnd = m_pShinIppan02;
			m_TabMng.Add ( shTabmng );
		}

		//準確定or準確定の修正・更正
		if ( m_pSnHeadData->Sn_Syuusei & 0x10 ) {
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//										付表６													   //
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
			if ( !m_pSnFhyo6Data ) {
				m_pSnFhyo6Data = new CH31SnFhyo6Data();
				if ( m_pSnFhyo6Data->GetData ( m_pZmSub, m_SnSeq ) ) {
					return -1;
				}
			}

			if ( !m_pFhyo6 ) {
				m_pFhyo6 = new CShinFhyo6 ( this );
				m_pFhyo6->InitInfo ( &m_pSnFhyo6Data, &m_pSnFhyo1_01Data, &m_pSnFhyo4_01Data, &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pZmSub, m_pShinInfo, m_SnSeq );
				m_pFhyo6->Create ( IDD_DIALOG_FHY6, &m_MainTab );
			}
			m_MainTab.InsertItem ( tabIdx, _T( "付表６" ), m_pFhyo6->m_hWnd );
			ptmpTab[tabIdx] = m_pFhyo6;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO6_TAB;
			shTabmng.pWnd = m_pFhyo6;
			m_TabMng.Add ( shTabmng );
		}

		// 還付
		if ( m_pSnHeadData->IsUseTaxRefund() ) {
			if ( !m_pSnRefundData ) {
				m_pSnRefundData = new CH31SnRefundData();
				if ( m_pSnRefundData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq ) ) {
					m_pSnRefundData->DeleteAllData ( m_pZmSub, -1 );
					return -1;
				}
			}

			// 情報設定
			ResetRefundData();

			// 個人
			if ( m_pSnHeadData->IsSoleProprietor() ) {
				if ( !m_pRefundKjn01 ) {
					m_pRefundKjn01 = new CShinRefundKjn01 ( this );
					m_pRefundKjn01->Create ( IDD_SYZREFUND_KJ1, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "還付付表1(個人)" ), m_pRefundKjn01->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundKjn01;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_KJ_01_TAB;
				shTabmng.pWnd = m_pRefundKjn01;
				m_TabMng.Add ( shTabmng );

				if ( !m_pRefundKjn02 ) {
					m_pRefundKjn02 = new CShinRefundKjn02 ( this );
					m_pRefundKjn02->Create ( IDD_SYZREFUND_KJ2, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "還付付表2(個人)" ), m_pRefundKjn02->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundKjn02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_KJ_02_TAB;
				shTabmng.pWnd = m_pRefundKjn02;
				m_TabMng.Add ( shTabmng );
			}
			// 法人
			else {
				if ( !m_pRefundHjn01 ) {
					m_pRefundHjn01 = new CShinRefundHjn01 ( this );
					m_pRefundHjn01->Create ( IDD_SYZREFUND_HJ1, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "還付付表1(法人)" ), m_pRefundHjn01->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundHjn01;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_HJ_01_TAB;
				shTabmng.pWnd = m_pRefundHjn01;
				m_TabMng.Add ( shTabmng );

				if ( !m_pRefundHjn02 ) {
					m_pRefundHjn02 = new CShinRefundHjn02 ( this );
					m_pRefundHjn02->Create ( IDD_SYZREFUND_HJ2, &m_MainTab );
				}
				m_MainTab.InsertItem ( tabIdx, _T( "還付付表2(法人)" ), m_pRefundHjn02->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundHjn02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_HJ_02_TAB;
				shTabmng.pWnd = m_pRefundHjn02;
				m_TabMng.Add ( shTabmng );
			}
		}

		// 更正の請求
		if ( m_pSnHeadData->Sn_Syuusei & 0x02 ) {
			if ( !m_pSnKskData ) {
				m_pSnKskData = new CH31SnKskData();
				if ( m_pSnKskData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq ) ) {
					return -1;
				}
			}

			CString	ttlStr;
			// 個人
			if ( m_pSnHeadData->IsSoleProprietor() ) {
				if ( !m_pKjnKskEX ) {
					m_pKjnKskEX = new CShinKjnKskEX ( this );
					m_pKjnKskEX->InitInfo ( &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData );
					m_pKjnKskEX->Create ( IDD_DIALOG_KJINKSK_EX, &m_MainTab );
				}
				if ( m_pSnHeadData->Sn_Syuusei & 0x04 ) {
					ttlStr = _T( "更正の申出(個人)" );
				}
				else {
					ttlStr = _T( "更正の請求(個人)" );
				}
				m_MainTab.InsertItem ( tabIdx, ttlStr, m_pKjnKskEX->m_hWnd );
				ptmpTab[tabIdx] = m_pKjnKskEX;
				firstPage = tabIdx;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSK_KJ_TAB;
				shTabmng.pWnd = m_pKjnKskEX;
				m_TabMng.Add ( shTabmng );
			}
			// 法人
			else {
				if ( !m_pHjnKskEX ) {
					m_pHjnKskEX = new CShinHjnKskEX ( this );
					m_pHjnKskEX->InitInfo ( &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData );
					m_pHjnKskEX->Create ( IDD_DIALOG_HJINKSK_EX, &m_MainTab);
				}
				if ( m_pSnHeadData->Sn_Syuusei & 0x04 ) {
					ttlStr = _T( "更正の申出(法人)" );
				}
				else {
					ttlStr = _T( "更正の請求(法人)" );
				}
				m_MainTab.InsertItem ( tabIdx, ttlStr, m_pHjnKskEX->m_hWnd );
				ptmpTab[tabIdx] = m_pHjnKskEX;
				firstPage = tabIdx;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_KSK_HJ_TAB;
				shTabmng.pWnd = m_pHjnKskEX;
				m_TabMng.Add ( shTabmng );
			}
		}
	}

	// 各表の入力ロックを描画
	for ( int tab_cnt = 0; tab_cnt < tabIdx; tab_cnt++ ) {
		( ( CSyzBaseDlg* )ptmpTab[tab_cnt] )->Static_Update ( m_pSnHeadData->Sn_Sign4 );
	}

	// 先頭ページ保存
	m_pFirstPage = ptmpTab[0];	

	RECT rect = { 0 };
	ptmpTab[0]->GetClientRect ( &rect );
	for ( int i = 0; i < tabIdx; i++ ) {
		ptmpTab[i]->MoveWindow ( &rect );
	}
	RECT rect1, rect2;
	CWnd *pWnd = ( CWnd* )&m_MainTab;
	pWnd->GetClientRect ( &rect1 );				// ｸﾗｲｱﾝﾄ領域の取得
	m_MainTab.AdjustRect ( FALSE, &rect1 );		// ﾀﾌﾞｺﾝﾄﾛｰﾙの表示領域の計算
	rect2.left   = rect1.left;
	rect2.top    = rect1.top;
	rect2.bottom = rect1.bottom;
	rect2.right  = rect1.right;
	for ( int i = 0; i < tabIdx; i++ ) {
		if ( ptmpTab[i] ) {
			ptmpTab[i]->MoveWindow ( &rect2 );	// ダイアログの移動
		}
	}

	// 初期ページ設定
	m_MainTab.SetCurSel ( firstPage );
	m_MainTab.ModifyStyle ( TCS_SINGLELINE, TCS_MULTILINE );

	GetClientRect ( vwRect );
	CRect tabRect;
	m_MainTab.GetClientRect ( tabRect );

	SCROLLINFO siV = { 0 }, siH = { 0 };
	siV.cbSize = sizeof ( SCROLLINFO );
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

	m_DispTabSgn = m_pSnHeadData->m_DispTabSgn;

	return 0;
}

//====================================================================
// 売上割合取得
//--------------------------------------------------------------------
// 引数
//		tag			:		タグ
//		chtag		:		未使用
//		pWari		:		売上割合
//--------------------------------------------------------------------
// 返送値
//		0			:		正常終了
//		-1			:		エラー
//====================================================================
int CH31HyoView::GetPercentage ( CString &tag, CString &chtag, unsigned short *pWari )
{
	ASSERT ( pWari );
	if ( pWari == NULL ) {
		return -1;
	}

	unsigned short tmpWari = 0;
	bool memoryFlg = false;

	if ( ( tag == _T( "CTD00130" ) ) || ( tag == _T( "CTD00200" ) ) || ( tag == _T( "CTD00270" ) ) || ( tag == _T( "CTD00340" ) ) || ( tag == _T( "CTD00410" ) ) || ( tag == _T( "CTD00480" ) ) ) {
		if ( m_pSnFhyo5_01Data == NULL ) {
			if ( ( m_pSnFhyo5_01Data = new CH31SnFhyo5_01Data ( TRUE ) ) == NULL ) {
				return -1;
			}

			if ( m_pSnFhyo5_01Data->GetData ( m_pZmSub, m_SnSeq ) ) {
				return -1;
			}
			memoryFlg = true;
		}
	}

	// 付表5-1
	if ( tag == _T( "CTD00130" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			tmpWari = m_pSnFhyo5_01Data->Sn_F501_UP1;
		}
	}
	else if ( tag == _T( "CTD00200" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			tmpWari = m_pSnFhyo5_01Data->Sn_F501_UP2;
		}
	}
	else if ( tag == _T( "CTD00270" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			tmpWari = m_pSnFhyo5_01Data->Sn_F501_UP3;
		}
	}
	else if ( tag == _T( "CTD00340" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			tmpWari = m_pSnFhyo5_01Data->Sn_F501_UP4;
		}
	}
	else if ( tag == _T( "CTD00410" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			tmpWari = m_pSnFhyo5_01Data->Sn_F501_UP5;
		}
	}
	else if ( tag == _T( "CTD00480" ) ) {
		if ( m_pSnFhyo5_01Data ) {
			tmpWari = m_pSnFhyo5_01Data->Sn_F501_UP6;
		}
	}

	*pWari = 0;
	if ( tmpWari != 0 ) {
		*pWari = tmpWari;
	}

	if ( memoryFlg ) {
		delete m_pSnFhyo5_01Data;
		m_pSnFhyo5_01Data = NULL;
	}

	return 0;
}

//====================================================================
// 売上割合取得
//--------------------------------------------------------------------
// 引数
//		type		:		0 = 付表5-1を取得
//							2 = 付表5-3を取得
//		id			:		項目ID
//		pWari		:		売上割合
//		pArith		:		合計金額
//--------------------------------------------------------------------
// 返送値
//		0			:		正常終了
//		-1			:		エラー
//====================================================================
int CH31HyoView::GetPercentage2( int id, unsigned short *pWari, char *pArith )
{
	ASSERT ( pWari );
	ASSERT ( pArith );
	if(  ( pWari == NULL ) || ( pArith == NULL ) ) {
		return -1;
	}

	int st = 0;
	unsigned short tmpWari = 0;
	char tmpArith[MONY_BUF_SIZE] = { 0 };

	if( m_pFhyo5_03 ) {
		st = m_pFhyo5_03->GetPercentage( id, &tmpWari, tmpArith );
	}

	if( m_pFhyo5_01 ) {
		st = m_pFhyo5_01->GetPercentage( id, &tmpWari, tmpArith );
	}

	*pWari = 0;
	memset( pArith, '\0', MONY_BUF_SIZE );

	if( !st ) {
		*pWari = tmpWari;
		memmove( pArith, tmpArith, MONY_BUF_SIZE );
	}
	else {
		return -1;
	}

	return 0;
}

//====================================================================
// 付表5 手入力サイン取得
//--------------------------------------------------------------------
// 引数
//		type		:		0 = 付表5-1を取得, 1 = 付表5-2を取得
//		id			:		項目ID
//		inputSign	:		取得した手入力サイン
//--------------------------------------------------------------------
// 返送値
//		0			:		正常終了
//		-1			:		エラー
//====================================================================
int CH31HyoView::GetInputSign ( int type, int id, int &inputSign )
{
	int ret = 0, st = 0;
	int tmpSign = 0;
	bool errFlg = false;

	inputSign = 0;

	if( type ) {
		if( m_pSnHeadData->SVmzsw == 1 ) {
			if( m_pFhyo5_022 ) {
				st = m_pFhyo5_022->GetInputSign_F522( id, tmpSign );
			}
		}
	}
	else {
		if( m_pFhyo5_012 ) {
			st = m_pFhyo5_012->GetInputSign_F512( id, tmpSign );
		}
	}

	if( st != -1 ) {
		inputSign = tmpSign;
		ret = 0;
	}
	else {
		ret = -1;
	}

	return ret;
}

//====================================================================
// 付表5 控除対象仕入税額の出力をセット
//====================================================================
void CH31HyoView::SetPintArea()
{
	// 簡易課税
	if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) {
		if ( m_pSnHeadData->Sn_GenericSgn & 0x40 ) {
			if ( m_pFhyo5_012 ) {
				m_pFhyo5_012->SetPrintArea();
				m_pFhyo5_012->WriteData();
			}

			if ( m_pFhyo5_022 ) {
				m_pFhyo5_022->SetPrintArea();
				m_pFhyo5_022->WriteData();
			}

			if ( m_pFhyo5_032 ) {
				m_pFhyo5_032->SetPrintArea();
				m_pFhyo5_032->WriteData();
			}
		}
		else {
			if ( m_pFhyo5_012 ) {
				m_pFhyo5_012->AllPrint();
				m_pFhyo5_012->WriteData();
			}

			if ( m_pFhyo5_022 ) {
				m_pFhyo5_022->AllPrint();
				m_pFhyo5_022->WriteData();
			}

			if ( m_pFhyo5_032 ) {
				m_pFhyo5_032->AllPrint();
				m_pFhyo5_032->WriteData();
			}
		}
	}
}

//======================================================
// 課税期間の取得
//------------------------------------------------------
// 引数
//		symd		:		課税期間（自）
//		eymd		:		課税期間（至）
//======================================================
void CH31HyoView::GetKazeiKikan( long &symd, long &eymd )
{
	// 課税期間の取得処理が散らばっているため関数化
	long tmpSymd = 0, tmpEymd = 0;

	// 初期化
	symd = 0;
	eymd = 0;

	if( !( m_pSnHeadData->Sn_SKKBN % 2 )) {
		// 中間申告
		tmpSymd = m_pSnHeadData->Sn_MDAYS;
		tmpEymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		// 確定申告
		tmpSymd = m_pSnHeadData->Sn_KDAYS;
		tmpEymd = m_pSnHeadData->Sn_KDAYE;
	}

	symd = tmpSymd;
	eymd = tmpEymd;
}

//======================================================
// 課税期間に免税事業者の期間が含まれている？
//------------------------------------------------------
// 返送値
//		true		:		含まれている
//		false		:		含まれていない
//======================================================
bool CH31HyoView::IsIncludeMenzeiKikan()
{
	// 課税期間の末日が、令和5年10月1日を超えていない場合は含まない
	long symd = 0, eymd = 0;
	GetKazeiKikan( symd, eymd );
	if( eymd < ICS_SH_INVOICE_DAY ) {
		return false;
	}

	// 課税開始日が未設定
	long kazeiKaishibi = 0;
	kazeiKaishibi = m_pZmSub->zvol->to_kz_ymd;
	if( kazeiKaishibi == 0 ) {
		return false;
	}

	// 課税事業者になった日が課税期間の開始日よりも後
	if( kazeiKaishibi <= symd ) {
		return false;
	}

	return true;
}

//======================================================
// ２割特例使用中？
//------------------------------------------------------
// 返送値
//		true		:		２割特例を使用中
//		false		:		２割特例を未使用
//======================================================
bool CH31HyoView::IsNiwariTokurei()
{
	bool bRet = false;

	if( m_pSnHeadData->m_DispTabSgn & BIT_NIWARITOKUREI ) {
		bRet = true;
	}
	else {
		bRet = false;
	}

	return bRet;
}

//================================================================
// 課税期間に免税の期間が含まれていれば集計を行い結果を取得する
//----------------------------------------------------------------
// 引数
//		symd			:			集計開始日
//		eymd			:			集計終了日
//		pArith			:			集計結果
//================================================================
void CH31HyoView::MenzeiKikanSyukei( long symd, long eymd, char *pArith )
{
	ASSERT( pArith );
	if( pArith == nullptr ) {
		return;
	}

	memset( pArith, '\0', MONY_BUF_SIZE );

	// 集計
	m_DBSyzAccess.IsOver5okuYen( m_pZmSub, symd, eymd, pArith );
}

//================================================================
// 課税開始日以降の課税売上高の集計を行う
//----------------------------------------------------------------
// 引数
//		pArith			:			集計結果
//================================================================
void CH31HyoView::KazeiUriageSyukei( char *pArith )
{
	/*
	//--> 20240226 del KazeiUriageSyukeiImpl(char *pArith, bool isNiwari, int mode)へ移動
	ASSERT( pArith );
	if( pArith == nullptr ) {
		return;
	}

	memset( pArith, '\0', MONY_BUF_SIZE );

	MoneyBasejagArray money;
	char val[MONY_BUF_SIZE] = { 0 };
	CString code;

	if( m_Util.GetKazeihoushiki( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
		// 原則課税
		if( IsNiwariTokurei() ){
			code.Format( _T( "331500" ));
		}
		else {
			code.Format( _T( "301500" ));
		}
	}
	else {
		// 簡易課税
		code.Format( _T( "331500" ));
	}

	money = m_pSyzSyukei->GetShinkokuData( code );
	memmove( pArith, money[0][0].arith, MONY_BUF_SIZE );
	//<-- 20240226 del
	*/
	KazeiUriageSyukeiImpl(pArith, false, 0);
}

//----------------------------------------------------------------> 20240226 add
//================================================================
// 課税開始日以降の課税売上高の集計を行う
//----------------------------------------------------------------
// 引数
//		pArith			:			集計結果
//      isNiwari        :           2割特例適用時にtrue
//----------------------------------------------------------------
// 備考
//      修正依頼168613関係、20240226追加
//　　　KazeiUriageSyukei(char *pArith)はm_pSnHeadData->m_DispTabSgnを参照して2割特例適用中かを判定するが
//      「2割特例を適用する」チェックボックス変更時にはcursetのみを更新し、HeadDataは本表・付表タブに移動するまで更新しないので
//      フラグを受け取るバージョンの関数を追加。
//================================================================
void CH31HyoView::KazeiUriageSyukei(char *pArith, bool isNiwari)
{
	KazeiUriageSyukeiImpl(pArith, isNiwari, 1);
}

//================================================================
// 課税開始日以降の課税売上高の集計を行う関数
// KazeiUriageSyukei(char*),KazeiUriageSyukei(char*, bool)の実装
//----------------------------------------------------------------
// 引数
//		pArith			:			集計結果
//      isNiwari        :           2割特例適用時にtrue
//      mode            :           0       ：引数isNiwariを無視してIsNiwariTokurei()で判定を行う
//                                  1       ：引数isNiwariで判定を行う
//                                  上記以外：1と同様
//----------------------------------------------------------------
// 備考
//      修正依頼168613関係の対応でKazeiUriageSyukei(char*, bool)を追加した際に処理を統合
//================================================================
void CH31HyoView::KazeiUriageSyukeiImpl(char *pArith, bool isNiwari, int mode)
{
	ASSERT(pArith);
	if (pArith == nullptr) {
		return;
	}

	memset(pArith, '\0', MONY_BUF_SIZE);

	MoneyBasejagArray money;
	char val[MONY_BUF_SIZE] = { 0 };
	CString code;

	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
		// 原則課税
		/*
		//--> 20240226 del
		if (IsNiwariTokurei()) {
			code.Format(_T("331500"));
		}
		else {
			code.Format(_T("301500"));
		}
		//--> 20240226 del
		*/
		//--> 20240226 add
		if (mode == 0) {
			(IsNiwariTokurei()) ? code.Format(_T("331500")) : code.Format(_T("301500"));
		}
		else {
			(isNiwari) ? code.Format(_T("331500")) : code.Format(_T("301500"));
		}
		//<-- 20240226 add
	}
	else {
		// 簡易課税
		code.Format(_T("331500"));
	}

	money = m_pSyzSyukei->GetShinkokuData(code);
	memmove(pArith, money[0][0].arith, MONY_BUF_SIZE);

}
//<---------------------------------------------------------------- 20240226 add

//================================================================
// 還付付表２フォーカス移動
//================================================================
void CH31HyoView::MoveKanpuFocus()
{
	int selNo = 0;
	selNo = m_MainTab.GetCurSel();
	if(( m_TabMng[selNo].tabtype == ID_ICSSH_REFUND_KJ_02_TAB ) || ( m_TabMng[selNo].tabtype == ID_ICSSH_REFUND_HJ_02_TAB )) {
		if( m_pRefundKjn02EX ) {
			m_pRefundKjn02EX->MoveFocus();
		}

		if( m_pRefundHjn02EX ) {
			m_pRefundHjn02EX->MoveFocus();
		}
	}
}

//================================================================
// 集計フラグのセット
//================================================================
void CH31HyoView::SetSyukeiFlg()
{
	// 集計フラグのセット処理が増えているため関数化する
	char syukeiFlg[MONY_BUF_SIZE] = { 0 };

	// ２割特例
	memset( syukeiFlg, '\0', MONY_BUF_SIZE );
	if( IsNiwariTokurei() ) {
		m_Arith.l_input( syukeiFlg, _T( "1" ));
	}
	m_pSyzSyukei->SetShinkokuData( _T( "FF0004" ), syukeiFlg );

	//-->20240307 h.wada add
	// 修正依頼168651 令和5年10月1日以降の課税期間でのみ適格請求発行事業者からの課税仕入の集計額を税額に含める
	long symd = 0, eymd = 0;
	GetKazeiKikan( symd, eymd );
	memset(syukeiFlg, '\0', MONY_BUF_SIZE);
	if (eymd >= ICS_SH_INVOICE_DAY) {
		m_Arith.l_input(syukeiFlg, _T("1"));
	}
	m_pSyzSyukei->SetShinkokuData(_T("FF7000"), syukeiFlg);
	//<--20240307 h.wada add

}

/* // 付表5 控除対象仕入税額の選択をICSP連携専用の機能とするため一時的にカット
//====================================================================
// 付表5 表示中？
//--------------------------------------------------------------------
// 返送値
//		0			:		付表5以外
//		1			:		付表5
//====================================================================
int CH31HyoView::IsFhyo5()
{
	int ret = 0;
	int selNo = 0;
	if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) {
		selNo = m_MainTab.GetCurSel();
		if ( ( m_TabMng[selNo].tabtype == ID_ICSSH_FHYO5_012_TAB ) || ( m_TabMng[selNo].tabtype == ID_ICSSH_FHYO5_032_TAB ) ) {
			ret = 1;
		}
	}
	else {
		// 簡易課税以外
		ret = 0;
	}

	return ret;
}


//====================================================================
// 付表5 控除対象仕入税額の選択
//--------------------------------------------------------------------
// 返送値
//		0			:		正常終了
//		-1			:		エラー
//====================================================================
int CH31HyoView::SelectFhyo5()
{
	CString msg;
	CKoujoSelect kSelect ( this );
	kSelect.InitInfo ( m_pSnHeadData, m_pSyzSyukei, &m_Arith );

	if ( !( m_pSnHeadData->Sn_GenericSgn & 0x08 ) ) {
		msg.Format ( _T( "控除対象仕入税額を選択しますか？" ) );

		if ( ICSMessageBox ( msg, ( MB_YESNO | MB_DEFBUTTON2 ), 0, 0 ) == IDNO ) {
			return 0;
		}

		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle ( g_hInstance );
		if ( kSelect.DoModal() == IDOK ) {
			m_pSnHeadData->Sn_GenericSgn |= 0x08;
			int getId = 0;
			kSelect.GetSelectData ( getId );

			if ( m_pFhyo5_012 ) {
				// 一旦リセットしておく
				m_pFhyo5_012->AutoKjCode();
				m_pFhyo5_012->SetSelectCode ( getId );
				m_pFhyo5_012->ReSyukei();

				if ( m_pFhyo5_022 ) {
					m_pFhyo5_022->AutoKjCode();
					m_pFhyo5_022->SetSelectCode ( getId );
					m_pFhyo5_022->ReSyukei();
				}
			}
			else if ( m_pFhyo5_032 ) {
				// 一旦リセットしておく
				m_pFhyo5_032->AutoKjCode();
				m_pFhyo5_032->SetSelectCode ( getId );
				m_pFhyo5_032->ReSyukei();
			}
		}
		AfxSetResourceHandle ( svhInstResource );
	}
	else {
		msg.Format ( _T( "控除対象仕入税額を自動で集計しますか？" ) );
		if ( ICSMessageBox ( msg, ( MB_YESNO | MB_DEFBUTTON2 ), 0, 0 ) == IDYES ) {
			m_pSnHeadData->Sn_GenericSgn &= ~0x08;
			if ( m_pFhyo5_012 ) {
				m_pFhyo5_012->AutoKjCode();
				m_pFhyo5_012->ReSyukei();
				if ( m_pFhyo5_022 ) {
					m_pFhyo5_022->AutoKjCode();
					m_pFhyo5_022->ReSyukei();
				}
			}
			else if ( m_pFhyo5_032 ) {
				m_pFhyo5_032->AutoKjCode();
				m_pFhyo5_032->ReSyukei();
			}
		}
	}

	SetItemPosition ( m_MainTab.GetCurSel() );

	return 0;
}
*/

//====================================================================
// 令和5年9月30日をまたぐ期間か  20231025
//--------------------------------------------------------------------
// 返送値
//		true		:		またぐ期間
//		false		:		またがない期間
//====================================================================
bool CH31HyoView::IsCrossOverR050930()
{
	bool rt = false;

	long smd = 0, emd = 0;
	GetKazeiKikan( smd, emd );

	if( smd < URIAGETOKUREIDAY && emd > URIAGETOKUREIDAY ){
		rt = true;
	}

	return rt;
}

//====================================================================
// 還付付表2　インボイス登録番号のチェック
//--------------------------------------------------------------------
// 引数
//		sgn			:		0　科目選択をマウスクリックした場合
//					:　		1　diagに入力した場合
//					:		2　F4プレビューやタブ移動など
//					:		3　処理終了
//--------------------------------------------------------------------
// 返送値
//		true		:		問題なし
//		false		:		不正な登録番号あり
//====================================================================
bool CH31HyoView::CheckInvno( int sgn )
{
	if ( m_pRefundHjn02EX ) {
		return m_pRefundHjn02EX->CheckInvno( sgn );	
	}
	if ( m_pRefundKjn02EX ) {
		return m_pRefundKjn02EX->CheckInvno( sgn );	
	}
	return true;
}

//-----------------------------------------------------------------------------
// 付表5-1　手入力サインのクリア '24.03.06
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH31HyoView::ClearManualInputSign_Fhyo51()
{
	if (m_pSnFhyo5_01Data) {
		m_pSnFhyo5_01Data->Sn_F501_6sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_14sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_15sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_16sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_17sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_18sw = 0;
		m_pSnFhyo5_01Data->Sn_F501_19sw = 0;
	}

	return 0;
}

// 24/04/01_303 add -->
//-----------------------------------------------------------------------------
// 更正の請求の「提出年月日」を取得する
//-----------------------------------------------------------------------------
// 返送値	：	提出年月日
//-----------------------------------------------------------------------------
int CH31HyoView::Get_KouseiTymd( void )
{
	CVolDateDB		vd;
	unsigned char	ymd[8]={0};
	unsigned char	ryy[4]={0};
	char			aymd[16]={0};
	int				gen=0;
	int				date=0;

	memcpy(ymd,&m_pSnKskData->KsData.KSdata[1],3);
	if((ymd[0]&0xff) == 0x00 && (ymd[1]&0xff) == 0x00 && (ymd[2]&0xff) == 0x00)	{
		return(0);
	}
	vd.db_datecnvGen(m_pSnKskData->KsData.KSdata[0],ymd,&gen,ryy,1,0);
	ymd[0] = ryy[0];
	l_unpac(aymd,ymd,6);
	date = atoi(aymd);
	return(date + 20000000);
}
// 24/04/01_303 add <--

//-----------------------------------------------------------------------------
// 前回申告区分：予定２６号のサインを返す　　　'24.07.23
//-----------------------------------------------------------------------------
BOOL CH31HyoView::GetPastYoteiSgn()
{
	return	m_PastYoteiSgn;
}

//-----------------------------------------------------------------------------
// 前回申告区分：予定２６号のサインを切替える　　'24.07.23
//-----------------------------------------------------------------------------
void CH31HyoView::ChgPastYoteiSgn( BOOL sign )
{
	if (sign)	m_PastYoteiSgn = TRUE;
	else		m_PastYoteiSgn = FALSE;
}