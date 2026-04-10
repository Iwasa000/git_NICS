// TaxationListView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "TaxationListView.h"

#include "ShinJigyo.h"
#include "ShinNougyou.h"
#include "ShinFudousan.h"
#include "ShinBuy.h"
#include "ShinSell.h"
#include "ShinKniinpDlg.h"

#include "TaxtionSelect.h"
//2016.06.22 INSERT START
#include "KoketsuMeisyo.h"
//2016.06.22 INSERT END

//----->経過措置用'14.08.07
#include "ShinBuyKeika.h"
#include "ShinJigyoKeika.h"//'14.08.11
#include "ShinFudousanKeika.h"//'14.08.11
#include "ShinNougyouKeika.h"//'14.08.11
#include "ShinSellKeika.h"//'14.08.11
#include "ShinKniinpKeika.h"//'14.10.08
//<----------------------

// 特定収入	('15.02.16)
#include "SpecPassOn.h"
#include "SpecPassOff.h"
//2015.03.05 INSERT START
#include "SpecPassOffEx.h"
//2015.03.05 INSERT END

//2016.10.20 INSERT START
#include "ShinKniinp6syuDlg.h"
#include "ShinKniinpKeika6syu.h"//'14.10.08
//2016.10.20 INSERT END

//2017.03.01 INSERT START
#include "ShinSpecific1EX.h"
#include "ShinSpecific1EXkeika.h"
//2017.03.01 INSERT END

extern	HINSTANCE g_hInstance;

// CTaxationListView

IMPLEMENT_DYNCREATE(CTaxationListView, ICSFormView)

CTaxationListView::CTaxationListView()
	: CBaseTaxHyoView(CTaxationListView::IDD)
	, m_pShinJigyo(NULL)
	, m_pShinFudousan(NULL)
	, m_pShinNougyou(NULL)
	, m_pShinBuy(NULL)
	, m_pShinSell(NULL)
	, m_pShinKniinp(NULL)
//2016.10.20 INSERT START
	, m_pShinKniinp6syu(NULL)
//2016.10.20 INSERT END
	, m_pTaxListData(NULL)
	, m_SnSeq(-1)
	, m_SvSnSeq(-1)//'14.09.01
	, m_pSnHeadData(NULL)
	, SHIN_TABMAX(0)
	, m_pShinInfo(NULL)
	, m_pZmSub(NULL)
	, m_pShzCalc(NULL)
	, m_pVprn(NULL)
	, m_pPset(NULL)
	, m_pFirstPage(NULL)				//'14.08.01
	, m_bShowed(FALSE)					//'14.08.05
	, m_pShinBuyKeika(NULL)				//'14.08.07
	, m_pShinFudousanKeika(NULL)		//'14.08.11
	, m_pShinNougyouKeika(NULL)			//'14.08.11
	, m_pShinJigyoKeika(NULL)			//'14.08.11
	, m_pShinSellKeika(NULL)			//'14.08.11
	, m_pShinKniinpKeika(NULL)			//'14.10.08
//2016.10.20 INSERT START
	, m_pShinKniinpKeika6syu(NULL)
//2016.10.20 INSERT END
	, m_pSyzSyukei(NULL)				//'14.08.08
	, m_nowTabIdx(0)
//--> '15.02.06 INS START
	, m_pSpcListData(NULL)
	, m_pShinSpecific1(NULL)
	, m_pShinSpecific21(NULL)
	, m_pShinSpecific22per8(NULL)
	, m_pShinSpecific22per5(NULL)
	, m_pShinSpecific34(NULL)
	, m_pShinSpecific51per8(NULL)
	, m_pShinSpecific51per5(NULL)
	, m_pShinSpecific52per8(NULL)
	, m_pShinSpecific52per5(NULL)
	, m_pShinSpecific53per8(NULL)
	, m_pShinSpecific53per5(NULL)
	, m_pShinSpecific6(NULL)
	, m_SelType(ID_ICSSH_NOLIST_TYPE)
//<-- '15.02.06 INS END
	, m_isLinkingSpcToHonpyo(FALSE)	// '15.04.29
//2016.10.20 INSERT START
	, m_SixKindFlg(FALSE)
//2016.10.20 INSERT END
//2017.03.01 INSERT START
	, m_pShinSpecific1EX(NULL)
	, m_pShinSpecific1EXkeika(NULL)
	, m_pShinSpecific21EX2(NULL)
	, m_pShinSpecific22keika(NULL)
	, m_pShinSpecific51keika(NULL)
	, m_pShinSpecific52keika(NULL)
	, m_pShinSpecific53keika(NULL)
//2017.03.01 INSERT END
	, m_JcContinuePrint(-1)
{
	m_Arith.l_defn( 0x16 );
	memset( m_ListType, '\0', sizeof(m_ListType) );
}

CTaxationListView::~CTaxationListView()
{
//--> '15.02.06 INS START
	if( m_pSpcListData ){
		delete m_pSpcListData;
		m_pSpcListData = NULL;
	}
//<-- '15.02.06 INS END
}

void CTaxationListView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_MainTab);
}

BEGIN_MESSAGE_MAP(CTaxationListView, ICSFormView)
	ON_COMMAND(ID_BUTTON_F2, OnButtonF2)//'14.08.05
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_BUTTON_F7, OnButtonF7)
	ON_COMMAND(ID_BUTTON_F8, OnButtonF8)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, OnTcnSelchangeMainTab)
	ON_COMMAND(ID_BUTTON_F9, &CTaxationListView::OnButtonF9)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CTaxationListView::OnUpdateButtonF9)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_KOMONSAKI_INF, &CTaxationListView::OnUpdateKomonsakiInf)//'14.08.05
	ON_COMMAND(ID_BUTTON_F12, OnButtonF12)//'14.08.08
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, &CTaxationListView::OnUpdateButtonF12)	//'14.08.08
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, &CTaxationListView::OnUpdateButtonF8)	//'14.10.08
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CTaxationListView::OnUpdateButtonF7)	//'14.10.08
	ON_MESSAGE( WM_MYMESSAGE, OnMyMessage )
	ON_MESSAGE(WM_USER_SHINTAB_INIT, OnUserShintabInit)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)										// '15.03.26
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, &CTaxationListView::OnUpdateButtonF11)	// '15.03.26
END_MESSAGE_MAP()


// CTaxationListView 診断

#ifdef _DEBUG
void CTaxationListView::AssertValid() const
{
	ICSFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTaxationListView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}
#endif
#endif //_DEBUG


//-->追加 harino ('14.08.08)
LRESULT CTaxationListView::OnMyMessage( WPARAM wParam, LPARAM lParam )
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	//yoshida150311
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){//課税取引
		SetItemPosition( m_MainTab.GetCurSel() );
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){//特定収入
		SetItemPositionEx( m_MainTab.GetCurSel() );
	}

	//yoshida150311
//	SetItemPosition( m_MainTab.GetCurSel() );
	  
	AfxSetResourceHandle( hInst );

	return 0;
}

//-->追加 harino ('14.08.08)

// CTaxationListView メッセージ ハンドラ

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	公益クラスのポインタ
//		pShinInfo	：	共通情報のポインタ
//		pSnHeadData	：	消費税ヘッダ情報のポインタ
//		pSyzSyukei	：	消費税集計クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
//2016.10.20 UPDATE START
//int CTaxationListView::InitView( CDBNpSub *pZmsub, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei )
int CTaxationListView::InitView( CDBNpSub *pZmsub, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, BOOL SixKindFlg  )
//2016.10.20 UPDATE END
{
//2016.10.20 INSERT START
	m_SixKindFlg = SixKindFlg;
//2016.10.20 INSERT END

	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		return -1;
	}
	m_pZmSub = pZmsub;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT(pSyzSyukei);
	if( pSyzSyukei == NULL ){
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

	// 内部タブの初期化
	InitTab(TRUE);//'14.08.05

	return 0;
}

//-----------------------------------------------------------------------------
// 内部タブの初期化
//-----------------------------------------------------------------------------.
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::InitTab( BOOL bDelete )
{
	//------->'14.08.05
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

//-- '15.02.06 --
//	int rt = InitTabSub( bDelete );
//---------------
//2016.02.23 INSERT START
	this->LockWindowUpdate();
//2016.02.23 INSERT END
	int rt = InitTabMain( bDelete );
//2016.02.23 INSERT START
	this->UnlockWindowUpdate();
//2016.02.23 INSERT END
//---------------

	AfxSetResourceHandle( hInst );

	return rt;
	//<----------------
}

//--> '15.02.06 INS START
//-----------------------------------------------------------------------------
// 内部タブの初期化（メイン）
//-----------------------------------------------------------------------------
// 引数	bDelete	：	内部を消去するかどうか
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::InitTabMain( BOOL bDelete )
{
	int rt = 0;

	ASSERT( m_pSnHeadData );
	if( m_pSnHeadData == NULL ){
		return -1;
	}

	if( m_Util.IsBasicMst(m_pZmSub->zvol) ){
		m_SelType = ID_ICSSH_TAXLIST_TYPE;
		rt = InitTabSub( bDelete );
	}
	else if( m_Util.IsSpcMaster(m_pZmSub) ){
		m_SelType = ID_ICSSH_SPCLIST_TYPE;
		rt = InitSpecificTab( bDelete );
	}

	return rt;
}
//<-- '15.02.06 INS END

//------>'14.08.05
//-----------------------------------------------------------------------------
// 内部タブの初期化
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::InitTabSub( BOOL bDelete )
{
	if( bDelete ){
		// 内部クラスを破棄？
		DeleteInsideDlg();
		// 全削除
		m_MainTab.DeleteAllItems();
	}
	
	SHIN_TABMAX = 0;

	ICSDialog	*ptmpTab[16] = {0};

	SH_TABMNG	shTabmng;//'14.08.01


	int tabIdx = 0;

	for( int i=0; i<ICSSH_TAXLIST_TAB_MAX; i++ ){
		m_ListType[i] = 0;
	}

	// 帳票データ
	if( !m_pTaxListData ){
		if( (m_pTaxListData = new CH26TaxListData()) == NULL ){
			return -1;
		}
		if( m_pTaxListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}

	// 集計クラスの初期化
	if( !m_pShzCalc ){
		if( (m_pShzCalc = new CShzCalc()) == NULL ){
			return -1;
		}
		m_pShzCalc->InitShzCalc( &m_pTaxListData, &m_pSnHeadData, m_pZmSub );
//		m_pShzCalc->InitShzCalc( m_pTaxListData, m_pSnHeadData, m_pZmSub );
//2016.10.20 INSERT START
		m_pShzCalc->m_SixKindFlg = m_SixKindFlg;
//2016.10.20 INSERT END
	}

	

	//'14.08.07
	// 経過措置対象の課税資産の譲渡等あり
	//++++++++++++++++++++++++++++++++++++++ 経過措置対象の課税資産の譲渡等あり++++++++++++++++++++++++++++++++++++++++++++++//
	if( m_pSnHeadData->SVmzsw == 1 ){
		//	pSyzShin->KniFlg = 0x00;									// 非連動ｄｂで簡易課税サイン [13'09.18]

		if( m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ){
	//		pSyzShin->KniFlg |= 0x01;								// 非連動ｄｂで簡易課税　D0:ON [13'09.18]

			// 非連動ｄｂで簡易課税
			//タブの順番変更'14.09.01
			//事業別売上高
//2016.10.20 INSERT START
			if( m_SixKindFlg == TRUE ){
				if( !m_pShinKniinpKeika6syu ){
					m_pShinKniinpKeika6syu = new CShinKniinpKeika6syu(this);
					m_pShinKniinpKeika6syu->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
					m_pShinKniinpKeika6syu->Create( IDD_DIALOG_JIGYO_SELL_KEIKA_6SYU, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinKniinpKeika6syu;
				m_ListType[tabIdx] = ID_ICSSH_KEI_6SYU_BUSINESS_SALES_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("事業別売上高"), m_pShinKniinpKeika6syu->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_KEI_6SYU_BUSINESS_SALES_LIST_TAB;
				shTabmng.pWnd = m_pShinKniinpKeika6syu;
			}
			else{
//2016.10.20 INSERT END
			if( !m_pShinKniinpKeika ){
				m_pShinKniinpKeika = new CShinKniinpKeika(this);
				m_pShinKniinpKeika->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//				m_pShinKniinpKeika->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
				m_pShinKniinpKeika->Create( IDD_DIALOG_JIGYO_SELL_KEIKA, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinKniinpKeika;
			m_ListType[tabIdx] = ID_ICSSH_KEI_BUSINESS_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T("事業別売上高"), m_pShinKniinpKeika->m_hWnd );//'14.08.06
			tabIdx++;
			SHIN_TABMAX++;

			//'14.08.01
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_KEI_BUSINESS_SALES_LIST_TAB;
			shTabmng.pWnd = m_pShinKniinpKeika;
//2016.10.20 INSERT START
			}
//2016.10.20 INSERT END
			m_TabMng.Add( shTabmng );


			// 課税売上高 '14.08.11
			if( !m_pShinSellKeika ){
				m_pShinSellKeika = new CShinSellKeika(this);
				m_pShinSellKeika->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//				m_pShinSellKeika->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
				m_pShinSellKeika->Create( IDD_DIALOG_SELL_KEIKA, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSellKeika;
			m_ListType[tabIdx] = ID_ICSSH_KEI_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T("課税売上高"), m_pShinSellKeika->m_hWnd );//'14.08.06
			tabIdx++;
			SHIN_TABMAX++;

			//'14.08.01
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_KEI_SALES_LIST_TAB;
			shTabmng.pWnd = m_pShinSellKeika;
			m_TabMng.Add( shTabmng );


			


		}
		else{
			//タブの順番変更'14.09.01

			//'14.08.11
			if( m_pZmSub->zvol->ind_type & 0x01 ){	// 一般
				if( !m_pShinJigyoKeika ){
					m_pShinJigyoKeika = new CShinJigyoKeika(this);
					m_pShinJigyoKeika->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinJigyoKeika->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinJigyoKeika->Create( IDD_DIALOG_JIGYO_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinJigyoKeika;
				m_ListType[tabIdx] = ID_ICSSH_KEI_GENERAL_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("事業所得用"), m_pShinJigyoKeika->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_KEI_GENERAL_LIST_TAB;
				shTabmng.pWnd = m_pShinJigyoKeika;
				m_TabMng.Add( shTabmng );

			}

			//'14.08.11
			if( m_pZmSub->zvol->ind_type&0x02 ){	// 不動産
				if( !m_pShinFudousanKeika ){
					m_pShinFudousanKeika = new CShinFudousanKeika(this);
					m_pShinFudousanKeika->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinFudousanKeika->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinFudousanKeika->Create( IDD_DIALOG_FUDOUSAN_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinFudousanKeika;
				m_ListType[tabIdx] = ID_ICSSH_KEI_REALESTATE_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("不動産所得用"), m_pShinFudousanKeika->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_KEI_REALESTATE_LIST_TAB;
				shTabmng.pWnd = m_pShinFudousanKeika;
				m_TabMng.Add( shTabmng );

			}

			//'14.08.11
			if( m_pZmSub->zvol->ind_type&0x04 ){	// 農業
				if( !m_pShinNougyouKeika ){
					m_pShinNougyouKeika = new CShinNougyouKeika(this);
					m_pShinNougyouKeika->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinNougyouKeika->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinNougyouKeika->Create( IDD_DIALOG_NOUGYOU_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinNougyouKeika;
				m_ListType[tabIdx] = ID_ICSSH_KEI_AGRICULTUTRE_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("農業所得用"), m_pShinNougyouKeika->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_KEI_AGRICULTUTRE_LIST_TAB;
				shTabmng.pWnd = m_pShinNougyouKeika;
				m_TabMng.Add( shTabmng );

			}

			// 課税売上高
			//'14.08.11
			if( !m_pShinSellKeika ){
				m_pShinSellKeika = new CShinSellKeika(this);
				m_pShinSellKeika->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//				m_pShinSellKeika->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
				m_pShinSellKeika->Create( IDD_DIALOG_SELL_KEIKA, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSellKeika;
			m_ListType[tabIdx] = ID_ICSSH_KEI_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T("課税売上高"), m_pShinSellKeika->m_hWnd );//'14.08.06
			tabIdx++;
			SHIN_TABMAX++;

			//'14.08.01
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_KEI_SALES_LIST_TAB;
			shTabmng.pWnd = m_pShinSellKeika;
			m_TabMng.Add( shTabmng );


			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI ){
	
				//経過措置用課税仕入高
				if( !m_pShinBuyKeika ){
					m_pShinBuyKeika = new CShinBuyKeika(this);
					m_pShinBuyKeika->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinBuyKeika->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinBuyKeika->Create( IDD_DIALOG_BUY_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinBuyKeika;
				m_ListType[tabIdx] = ID_ICSSH_KEI_STOCK_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("課税仕入高"), m_pShinBuyKeika->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_KEI_STOCK_LIST_TAB;
				shTabmng.pWnd = m_pShinBuyKeika;
				m_TabMng.Add( shTabmng );

			}
			
			
			
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//



	//++++++++++++++++++++++++++++++++++++++ 経過措置対象の課税資産の譲渡等なし++++++++++++++++++++++++++++++++++++++++++++++//

	else{
	//	pSyzShin->KniFlg = 0x00;									// 非連動ｄｂで簡易課税サイン [13'09.18]

		if( m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ){
	//		pSyzShin->KniFlg |= 0x01;								// 非連動ｄｂで簡易課税　D0:ON [13'09.18]


			// 非連動ｄｂで簡易課税
			//タブの順番変更'14.09.01
			//事業別売上高
//2016.10.20 INSERT START
			if( m_SixKindFlg == TRUE ){
				if( !m_pShinKniinp6syu ){
					m_pShinKniinp6syu = new CShinKniinp6syuDlg(this);
					m_pShinKniinp6syu->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
					m_pShinKniinp6syu->Create( IDD_DIALOG_JIGYO_SELL_6SYU, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinKniinp6syu;
				m_ListType[tabIdx] = ID_ICSSH_6SYU_BUSINESS_SALES_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("事業別売上高"), m_pShinKniinp6syu->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_6SYU_BUSINESS_SALES_LIST_TAB;
				shTabmng.pWnd = m_pShinKniinp6syu;
			}else{
//2016.10.20 INSERT END
			if( !m_pShinKniinp ){
				m_pShinKniinp = new CShinKniinpDlg(this);
				m_pShinKniinp->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//				m_pShinKniinp->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
				m_pShinKniinp->Create( IDD_DIALOG_JIGYO_SELL, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinKniinp;
			m_ListType[tabIdx] = ID_ICSSH_BUSINESS_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T("事業別売上高"), m_pShinKniinp->m_hWnd );//'14.08.06
			tabIdx++;
			SHIN_TABMAX++;

			//'14.08.01
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_BUSINESS_SALES_LIST_TAB;
			shTabmng.pWnd = m_pShinKniinp;
//2016.10.20 INSERT START
			}
//2016.10.20 INSERT END
			m_TabMng.Add( shTabmng );


			// 課税売上高
			if( !m_pShinSell ){
				m_pShinSell = new CShinSell(this);
				m_pShinSell->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//				m_pShinSell->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
				m_pShinSell->Create( IDD_DIALOG_SELL, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSell;
			m_ListType[tabIdx] = ID_ICSSH_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T("課税売上高"), m_pShinSell->m_hWnd );//'14.08.06
			tabIdx++;
			SHIN_TABMAX++;

			//'14.08.01
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_SALES_LIST_TAB;
			shTabmng.pWnd = m_pShinSell;
			m_TabMng.Add( shTabmng );


		}
		else{
			//タブの順番変更'14.09.01

			//'14.08.01
//			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//			shTabmng.tabtype = ID_ICSSH_SALES_LIST_TAB;
//			shTabmng.pWnd = m_pShinSell;
//			m_TabMng.Add( shTabmng );

			if( m_pZmSub->zvol->ind_type & 0x01 ){	// 一般
				if( !m_pShinJigyo ){
					m_pShinJigyo = new CShinJigyo(this);
					m_pShinJigyo->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinJigyo->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinJigyo->Create( IDD_DIALOG_JIGYO, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinJigyo;
				m_ListType[tabIdx] = ID_ICSSH_GENERAL_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("事業所得用"), m_pShinJigyo->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_GENERAL_LIST_TAB;
				shTabmng.pWnd = m_pShinJigyo;
				m_TabMng.Add( shTabmng );

			}

			if( m_pZmSub->zvol->ind_type&0x02 ){	// 不動産
				if( !m_pShinFudousan ){
					m_pShinFudousan = new CShinFudousan(this);
					m_pShinFudousan->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinFudousan->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinFudousan->Create( IDD_DIALOG_FUDOUSAN, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinFudousan;
				m_ListType[tabIdx] = ID_ICSSH_REALESTATE_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("不動産所得用"), m_pShinFudousan->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_REALESTATE_LIST_TAB;
				shTabmng.pWnd = m_pShinFudousan;
				m_TabMng.Add( shTabmng );

			}

			if( m_pZmSub->zvol->ind_type&0x04 ){	// 農業
				if( !m_pShinNougyou ){
					m_pShinNougyou = new CShinNougyou(this);
					m_pShinNougyou->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinNougyou->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinNougyou->Create( IDD_DIALOG_NOUGYOU, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinNougyou;
				m_ListType[tabIdx] = ID_ICSSH_AGRICULTUTRE_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("農業所得用"), m_pShinNougyou->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;

				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_AGRICULTUTRE_LIST_TAB;
				shTabmng.pWnd = m_pShinNougyou;
				m_TabMng.Add( shTabmng );

//2016.10.20 INSERT START
				if( m_pSnHeadData->SVmzsw == 1 ){
					m_pShzCalc->Shz_KeiAllCalc(3);
				}else{
					m_pShzCalc->Shz_AllCalc(3);
				}
//2016.10.20 INSERT END

			}

			// 課税売上高
			if( !m_pShinSell ){
				m_pShinSell = new CShinSell(this);
				m_pShinSell->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//				m_pShinSell->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
				m_pShinSell->Create( IDD_DIALOG_SELL, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSell;
			m_ListType[tabIdx] = ID_ICSSH_SALES_LIST_TAB;
			m_MainTab.InsertItem( tabIdx, _T("課税売上高"), m_pShinSell->m_hWnd );//'14.08.06
			tabIdx++;
			SHIN_TABMAX++;

			//'14.08.01
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_SALES_LIST_TAB;
			shTabmng.pWnd = m_pShinSell;
			m_TabMng.Add( shTabmng );


			
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI ){
		
				if( !m_pShinBuy ){
					m_pShinBuy = new CShinBuy(this);
					m_pShinBuy->InitInfo( &m_pTaxListData, &m_pSnHeadData, m_pZmSub, &m_Arith , &m_pShzCalc );//'14.08.06
//					m_pShinBuy->InitInfo( m_pTaxListData, m_pSnHeadData, m_pZmSub, &m_Arith , m_pShzCalc );//'14.08.06
					m_pShinBuy->Create( IDD_DIALOG_BUY, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinBuy;
				m_ListType[tabIdx] = ID_ICSSH_STOCK_LIST_TAB;
				m_MainTab.InsertItem( tabIdx, _T("課税仕入高"), m_pShinBuy->m_hWnd );//'14.08.06
				tabIdx++;
				SHIN_TABMAX++;
		
				//'14.08.01
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_STOCK_LIST_TAB;
				shTabmng.pWnd = m_pShinBuy;
				m_TabMng.Add( shTabmng );

			}

		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	}

//2016.02.23 INSERT START
	for( int tab_cnt = 0; tab_cnt<tabIdx; tab_cnt++){
		((CSyzBaseDlg *)ptmpTab[tab_cnt])->Static_Update( m_pSnHeadData->Sn_Sign4 );
	}
//2016.02.23 INSERT END

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
//	m_MainTab.SetCurSel( firstPage );
//	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );


	//------>'14.08.01
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
	//<---------------------

	return 0;


}

//-----------------------------------------------------------------------------
// タブ内のダイアログを全消去
//-----------------------------------------------------------------------------
void CTaxationListView::DeleteInsideDlg()
{
	// ビュー内部からフォーカスを外す！
	m_MainTab.SetFocus();

	// 課税仕入高
	if( m_pShinBuy ){
		m_pShinBuy->DestroyWindow();
		delete m_pShinBuy;
		m_pShinBuy = NULL;	
	}

	// 課税売上高
	if( m_pShinSell ){
		m_pShinSell->DestroyWindow();
		delete m_pShinSell;
		m_pShinSell = NULL;	
	}

	// 非連動ｄｂで簡易課税
	// 事業別売上高
	if( m_pShinKniinp ){
		m_pShinKniinp->DestroyWindow();
		delete m_pShinKniinp;
		m_pShinKniinp = NULL;	
	}
//2016.10.20 INSERT START
	// 事業別売上高
	if( m_pShinKniinp6syu ){
		m_pShinKniinp6syu->DestroyWindow();
		delete m_pShinKniinp6syu;
		m_pShinKniinp6syu = NULL;	
	}
//2016.10.20 INSERT END

	// 農業所得用
	if( m_pShinNougyou ){
		m_pShinNougyou->DestroyWindow();
		delete m_pShinNougyou;
		m_pShinNougyou = NULL;	
	}

	// 不動産所得用
	if( m_pShinFudousan ){
		m_pShinFudousan->DestroyWindow();
		delete m_pShinFudousan;
		m_pShinFudousan = NULL;	
	}

	// 事業所得用(一般)
	if( m_pShinJigyo ){
		m_pShinJigyo->DestroyWindow();
		delete m_pShinJigyo;
		m_pShinJigyo = NULL;	
	}

	//経過措置の関係'14.08.07-------->
	// 課税仕入(経過措置用)'14.08.07
	if(m_pShinBuyKeika){
		m_pShinBuyKeika->DestroyWindow();
		delete m_pShinBuyKeika;
		m_pShinBuyKeika = NULL;	
	}

	// 不動産(経過措置用)'14.08.11
	if(m_pShinFudousanKeika){
		m_pShinFudousanKeika->DestroyWindow();
		delete m_pShinFudousanKeika;
		m_pShinFudousanKeika = NULL;	
	}

	// 農業(経過措置用)'14.08.11
	if(m_pShinNougyouKeika){
		m_pShinNougyouKeika->DestroyWindow();
		delete m_pShinNougyouKeika;
		m_pShinNougyouKeika = NULL;	
	}

	// 事業(経過措置用)'14.08.11
	if(m_pShinJigyoKeika){
		m_pShinJigyoKeika->DestroyWindow();
		delete m_pShinJigyoKeika;
		m_pShinJigyoKeika = NULL;	
	}

	// 課税売上(経過措置用)'14.08.11
	if(m_pShinSellKeika){
		m_pShinSellKeika->DestroyWindow();
		delete m_pShinSellKeika;
		m_pShinSellKeika = NULL;	
	}

	// 非連動ｄｂで簡易課税
	// 事業別売上高(経過措置用)
	if( m_pShinKniinpKeika ){
		m_pShinKniinpKeika->DestroyWindow();
		delete m_pShinKniinpKeika;
		m_pShinKniinpKeika = NULL;	
	}
//2016.10.20 INSERT START
	if( m_pShinKniinpKeika6syu ){
		m_pShinKniinpKeika6syu->DestroyWindow();
		delete m_pShinKniinpKeika6syu;
		m_pShinKniinpKeika6syu = NULL;	
	}
//2016.10.20 INSERT END

	//<-----------------------------


	m_TabMng.RemoveAll();
	m_pFirstPage = NULL;


	return;
}
//<--------------

#ifdef _20150209_EXTEND_
//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::EndJob()
{
	//== 最終データ書込み ==
	if( WriteData(TRUE) ){
		return -1;
	}

	if( m_pShzCalc ){
		delete m_pShzCalc;
		m_pShzCalc = NULL;
	}

	DeleteInsideDlg();

	return 0;
}
#else
//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::EndJob()
{
	//== 最終データ書込み ==
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( WriteData(TRUE) ){
			return -1;
		}

		if( m_pShzCalc ){
			delete m_pShzCalc;
			m_pShzCalc = NULL;
		}

		DeleteInsideDlg();
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		if( WriteSpcData(TRUE) ){
			return -1;
		}

		DeleteInsideSpcDlg();
	}

	return 0;
}
#endif

// --> 20191112　必要ないタイミングで保存処理走ってしまい、31の保存データに26のデータを上書きしてしまっていたため追加
//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::EndJob( int wflg )
{
	//== 最終データ書込み ==
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( WriteData(TRUE) ){
			return -1;
		}

		if( m_pShzCalc ){
			delete m_pShzCalc;
			m_pShzCalc = NULL;
		}

		DeleteInsideDlg();
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
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

//-----------------------------------------------------------------------------
// データ書込み
//-----------------------------------------------------------------------------
// 引数	isEnd	：	最終データ？
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::WriteData( BOOL isEnd )
{ 
	if( m_pTaxListData ){
		if( m_pTaxListData->SetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pTaxListData;
			m_pTaxListData = NULL;
		}
	}

//--> '15.03.23 INS START
	if( m_pSpcListData ){
		if( m_pSpcListData->SetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pSpcListData;
			m_pSpcListData = NULL;
		}
	}
//<-- '15.03.23 INS END

	return 0;
}

void CTaxationListView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();

	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	SetResize( IDC_MAIN_TAB, ICS_RESIZE_BOTTOM );

}

//-----------------------------------------------------------------------------
// 各表の更新後の再表示
//-----------------------------------------------------------------------------
// 引数	selno	：	該当表番号
//-----------------------------------------------------------------------------
void CTaxationListView::IndicateTasList( int selno )
{
//	int page = SHIN_TABMAX - (selno+1);

	CSyzBaseDlg	*pBaseList = NULL;

//	switch( m_ListType[ page ] ){
	switch( m_ListType[ selno ] ){
		case ID_ICSSH_GENERAL_LIST_TAB :		// 一般
			pBaseList = m_pShinJigyo;
			break;
		case ID_ICSSH_REALESTATE_LIST_TAB :		// 不動産
			pBaseList = m_pShinFudousan;
			break;
		case ID_ICSSH_AGRICULTUTRE_LIST_TAB :	// 農業
			pBaseList = m_pShinNougyou;
			break;
		case ID_ICSSH_SALES_LIST_TAB :			// 課税売上高
			pBaseList = m_pShinSell;
			break;
		case ID_ICSSH_STOCK_LIST_TAB :			// 課税仕入高
			pBaseList = m_pShinBuy;
			break;
		case ID_ICSSH_BUSINESS_SALES_LIST_TAB :	// 事業別売上高
			pBaseList = m_pShinKniinp;
			break;

		//----------------------->'14.08.18
		case ID_ICSSH_KEI_GENERAL_LIST_TAB :		// 一般（経過措置用）
			pBaseList = m_pShinJigyoKeika;
			break;
		case ID_ICSSH_KEI_REALESTATE_LIST_TAB :		// 不動産（経過措置用）
			pBaseList = m_pShinFudousanKeika;
			break;
		case ID_ICSSH_KEI_AGRICULTUTRE_LIST_TAB :	// 農業（経過措置用）
			pBaseList = m_pShinNougyouKeika;
			break;
		case ID_ICSSH_KEI_SALES_LIST_TAB :			// 課税売上高（経過措置用）
			pBaseList = m_pShinSellKeika;
			break;
		case ID_ICSSH_KEI_STOCK_LIST_TAB :			// 課税仕入高（経過措置用）
			pBaseList = m_pShinBuyKeika;
			break;
		case ID_ICSSH_KEI_BUSINESS_SALES_LIST_TAB :	// 事業別売上高
			pBaseList = m_pShinKniinpKeika;
			break;
		//<------------------------------
//2016.10.20 INSERT START
		case ID_ICSSH_6SYU_BUSINESS_SALES_LIST_TAB :	// 事業別売上高
			pBaseList = m_pShinKniinp6syu;
			break;
		case ID_ICSSH_KEI_6SYU_BUSINESS_SALES_LIST_TAB :	// 事業別売上高
			pBaseList = m_pShinKniinpKeika6syu;
			break;
//2016.10.20 INSERT END

		default:
			break;
	}

	if( pBaseList ){
		pBaseList->DispList( 0 );
//		pBaseList->SetRedrawScroll( 0 );
//2016.06.22 INSERT START
		pBaseList->SetFocusRange();
//2016.06.22 INSERT END
	}
}

//-----------------------------------------------------------------------------
// 各表の更新後のフォーカスセット('16.12.26)
//-----------------------------------------------------------------------------
// 引数	selno	：	該当表番号
//-----------------------------------------------------------------------------
void CTaxationListView::IndicateItemPosition( int selno )
{
	CSyzBaseDlg	*pBaseList = NULL;

	switch( m_ListType[ selno ] ){
		case ID_ICSSH_GENERAL_LIST_TAB :					// 一般
			pBaseList = m_pShinJigyo;
			break;
		case ID_ICSSH_REALESTATE_LIST_TAB :					// 不動産
			pBaseList = m_pShinFudousan;
			break;
		case ID_ICSSH_AGRICULTUTRE_LIST_TAB :				// 農業
			pBaseList = m_pShinNougyou;
			break;
		case ID_ICSSH_SALES_LIST_TAB :						// 課税売上高
			pBaseList = m_pShinSell;
			break;
		case ID_ICSSH_STOCK_LIST_TAB :						// 課税仕入高
			pBaseList = m_pShinBuy;
			break;
		case ID_ICSSH_BUSINESS_SALES_LIST_TAB :				// 事業別売上高
			pBaseList = m_pShinKniinp;
			break;
		case ID_ICSSH_KEI_GENERAL_LIST_TAB :				// 一般（経過措置用）
			pBaseList = m_pShinJigyoKeika;
			break;
		case ID_ICSSH_KEI_REALESTATE_LIST_TAB :				// 不動産（経過措置用）
			pBaseList = m_pShinFudousanKeika;
			break;
		case ID_ICSSH_KEI_AGRICULTUTRE_LIST_TAB :			// 農業（経過措置用）
			pBaseList = m_pShinNougyouKeika;
			break;
		case ID_ICSSH_KEI_SALES_LIST_TAB :					// 課税売上高（経過措置用）
			pBaseList = m_pShinSellKeika;
			break;
		case ID_ICSSH_KEI_STOCK_LIST_TAB :					// 課税仕入高（経過措置用）
			pBaseList = m_pShinBuyKeika;
			break;
		case ID_ICSSH_KEI_BUSINESS_SALES_LIST_TAB :			// 事業別売上高
			pBaseList = m_pShinKniinpKeika;
			break;
		case ID_ICSSH_6SYU_BUSINESS_SALES_LIST_TAB :		// 事業別売上高
			pBaseList = m_pShinKniinp6syu;
			break;
		case ID_ICSSH_KEI_6SYU_BUSINESS_SALES_LIST_TAB :	// 事業別売上高
			pBaseList = m_pShinKniinpKeika6syu;
			break;

		default:
			break;
	}

	if( pBaseList ){
		pBaseList->SetItemPosition();
	}
}

void CTaxationListView::OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result)
{
	int selno = m_MainTab.GetCurSel();

//-- '15.02.09 --
//	if( m_nowTabIdx != selno ){
//		// 各表の更新処理後の再表示
//		IndicateTasList( selno );
//		SetItemPosition( selno );
//
//		m_nowTabIdx = selno;
//	}else{
//		SetItemPosition( selno );
//	}
//---------------
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( m_nowTabIdx != selno ){
			// 各表の更新処理後の再表示
			IndicateTasList( selno );
			SetItemPosition( selno );

			m_nowTabIdx = selno;
		}else{
			SetItemPosition( selno );
		}
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		ReInitSpcDisp( selno );
		SetItemPositionEx( selno );//yoshida150311

	}
//---------------
}

//------------>F2ボタン対応//'14.08.05
void CTaxationListView::OnButtonF2()
{
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		if( m_TabMng[selno].pWnd ){
			m_TabMng[selno].pWnd->PostMessage( WM_KEYDOWN, VK_F2 );
		}
	}
	
}
//<-------------------------------

void CTaxationListView::OnButtonF4()
{
	if( m_pShinInfo && m_pShinInfo->pBaseView ){
		// データ更新
		m_pShinInfo->pBaseView->PostMessage( WM_COMMAND, ID_BUTTON_F4 );
	}
}

void CTaxationListView::OnButtonF5()
{
	if( m_pShinInfo && m_pShinInfo->pBaseView ){
		// データ更新
		m_pShinInfo->pBaseView->PostMessage( WM_COMMAND, ID_BUTTON_F5 );
	}
}

void CTaxationListView::OnButtonF7()
{
//-- '15.02.09 --
//	if( m_pShinInfo && m_pShinInfo->pBaseView ){
//		CString	str;
//		int		ans;
//		// 処理確認
//		str = _T("課税取引金額計算表のデータがクリアされます。\n");
//		str +=_T("処理を継続しますか？");
//		ans = ICSMessageBox( str, (MB_YESNO | MB_DEFBUTTON2) );
//		if( ans == IDNO ){
//			return;
//		}
//
//		if( m_pShzCalc ){
//			m_pShzCalc->Shz_DelData();
//		}
//
//		IndicateTasList( m_MainTab.GetCurSel() );
//	}
//---------------
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( m_pShinInfo && m_pShinInfo->pBaseView ){
			CString	str;
			int		ans;
			// 処理確認
			str = _T("課税取引金額計算表のデータがクリアされます。\n");
			str +=_T("処理を継続しますか？");
			ans = ICSMessageBox( str, (MB_YESNO | MB_DEFBUTTON2) );
			if( ans == IDNO ){
				return;
			}

			if( m_pShzCalc ){
				m_pShzCalc->Shz_DelData();
			}
			IndicateTasList( m_MainTab.GetCurSel() );
		}
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
	}
//---------------
}

void CTaxationListView::OnButtonF8()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	// 非連動ｄｂ
	CString	str;
	str  = "消費税申告書へ転記しますか？\n";
	if( ICSMessageBox( str, MB_YESNO|MB_DEFBUTTON2 ) != IDYES )	return;
	if( m_pShzCalc->Shz_ReportPost() == -1 )	return;
	str  = "消費税申告書への転記が終了しました。\n\n";
	str += "申告書・付表の画面を開いて転記した金額を確認して下さい。\n";
	str += "また不足する項目については手入力して下さい。\n";
	ICSMessageBox( str );

	//----------->メインにメッセージを投げて本表データに転記
	m_pShinInfo->pBaseView->SendMessage( WM_USER_TENKI , (WPARAM)0 );
	//<------------------------------------------------------

	//----> 再集計
//	m_pShinInfo->pParent->SendMessage( WM_USER_RECALQ, (WPARAM)0 );
	//<-------------------------

	
	
}


void CTaxationListView::OnUpdateButtonF8(CCmdUI *pCmdUI)
{

	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。

	int	Flg = 0;

	ICSToolBar* pBar = ((ICSMainFrame *)m_pShinInfo->pParent)->GetToolBar( 1 );

	if( pBar ){
		char*		p;
		p = _T("F8 申告書転記");
		
		pBar->SetButtonTooltip( 6, p );
		pBar->SetButtonText( 6, p );

//-- '15.04.15 --		
//		if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){// 非連動ｄｂ
//			if( m_pSnHeadData->Sn_Sign4 & 0x01)	Flg = 0;
//			else								Flg = 1;
//		}
//---------------
		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
//2016.06.22 UPDATE START
//			if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){// 非連動ｄｂ
			if(( m_Util.IsUnConnectMst(m_pZmSub->zvol)) ||( m_pSnHeadData->Sn_Sign4&0x80 )){// 非連動ｄｂ or 非連動にチェック
//2016.06.22 UPDATE END
				if( m_pSnHeadData->Sn_Sign4 & 0x01)	Flg = 0;
				else								Flg = 1;
			}
		}
		else{
			Flg = 0;
		}
//---------------
	}

	pCmdUI->Enable( Flg );

}

void CTaxationListView::OnUpdateButtonF7(CCmdUI *pCmdUI)
{

	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	int	Flg = 0;

	//yoshida150325
	 if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){//特定収入では”初期化”ボタン非表示に
		pCmdUI->Enable( FALSE );
	 }
	 else{
		if( /*pSyzShin->IsNewMaster() &&*/ (m_pSnHeadData->Sn_Sign4 & 0x01) == 0x00 )	Flg = 1;
		if( Flg )		pCmdUI->Enable( TRUE );
		else			pCmdUI->Enable( FALSE );
	 }

//	 if( /*pSyzShin->IsNewMaster() &&*/ (m_pSnHeadData->Sn_Sign4 & 0x01) == 0x00 )	Flg = 1;
//		if( Flg )		pCmdUI->Enable( TRUE );
//		else			pCmdUI->Enable( FALSE );

}


//-----------------------------------------------------------------------------
// 経理方式の切替
//-----------------------------------------------------------------------------
// 引数	pKHstax			：	変更後の経理方式
//-----------------------------------------------------------------------------
// 返送値	IDOK		：	OK 終了
//			IDCANCEL	：	CANCEL 終了
//-----------------------------------------------------------------------------
int CTaxationListView::ChgTaxationSystem( int *pKHstax )
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
//	pSyzShin->curset.Sn_Sign2 &= 0xf7;
//	if( m_pTaxListData->Head.KHstax )	pSyzShin->curset.Sn_Sign2 |= 0x08;

	// 各表の更新処理
	if( m_pSnHeadData->SVmzsw == 1 ){
		m_pShzCalc->Shz_KeiAllCalc();
	}else{
		m_pShzCalc->Shz_AllCalc();
	}
	// 各表の再表示
	IndicateTasList( m_MainTab.GetCurSel() );

	return IDOK;
}

//2016.06.22 INSERT START
int CTaxationListView::ChgKoketsuMeisyoGet( int *pKoketsu )
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

	return IDOK;
}
//2016.06.22 INSERT END

void CTaxationListView::OnButtonF9()
{

	//--------->'14.08.05
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if( m_pSnHeadData == NULL ){
		return;
	}
	else if( (m_pShinInfo==NULL) || (m_pShinInfo->pBaseView==NULL) || (m_pShinInfo->pToolBar==NULL) ){
		return;
	}

	// 入力データ保存
	unsigned char	svSn_Sign4 = m_pSnHeadData->Sn_Sign4;

	if( m_pSnHeadData->DoUnlock(m_pZmSub, this) == IDNO ){
		return;
	}

	m_pShinInfo->pParent->PostMessage( WM_USER_UPDATE_INDICATE, 0 );
	m_pShinInfo->pBaseView->SendMessage( WM_USER_UNLOCK, (WPARAM)0 );

	if( (svSn_Sign4!=m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4&0x01) ){
//-- '15.02.09 --
//		WriteData( FALSE );
////		WriteData( FALSE, TRUE );
//---------------
		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
			WriteData( FALSE );
		}
		else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
			WriteSpcData( FALSE );
		}
//---------------
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
//-- '15.02.09 --
//		WriteData( FALSE );
////		WriteData( FALSE, FALSE );
//---------------
		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
			WriteData( FALSE );
		}
		else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
			WriteSpcData( FALSE );
		}
//---------------
	}

	if( m_SnSeq == 0 ){	// 現申告
		// 再集計
		//計算０％か１００％か選択するダイアログが２回出てしまっていた。集計処理は必要ないのでカットyoshida150421
//		m_pShinInfo->pParent->SendMessage( WM_USER_RECALQ, (WPARAM)0 );
	}

	// DIAGの属性変更
//	ChangeDiagAttr();

	//DBsyz26の初期化関数を呼ぶ。ロックした時に本表などがロックかかってなかった為
	m_pShinInfo->pBaseView->SendMessage( WM_USER_SYZINIT , (WPARAM)0 );


	//タブ初期化
	InitTab( TRUE );


	m_MainTab.SetCurSel( selno );
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );

	//yoshida150311
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){//課税取引
		SetItemPosition( selno );
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){//特定収入
		SetItemPositionEx( selno );
	}

//	SetItemPosition( selno );

	//<-----------------------------
}


//-----------------------------------------------------------------------------
// DIAGの属性変更
//-----------------------------------------------------------------------------
/*void CH26HyoView::ChangeDiagAttr()
{
	if( m_pIppan ){
		m_pIppan->ChgDiagAttr();
	}
	if( m_pKani ){
		m_pKani->ChgDiagAttr();
	}
}*/



void CTaxationListView::OnUpdateButtonF9(CCmdUI *pCmdUI)
{
//	CMainFrame* frm = (CMainFrame *)GetParentOwner();
//	frm->UpdateLockIndicator(pSyzShin->Snd.Sn_Sign4&0x01);

	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_pSnHeadData == NULL ){
		return;
	}
	else if( (m_pShinInfo==NULL) || (m_pShinInfo->pParent==NULL) ){
		return;
	}

	ICSToolBar* pBar = ((ICSMainFrame *)m_pShinInfo->pParent)->GetToolBar( 1 );
	if( pBar ){
		char*		p;
		if( m_pSnHeadData->Sn_Sign4&0x01 ){
			p = _T("F9 ロック解除");
		}
		else{
			p = _T("F9 入力ロック");
		}
		pBar->SetButtonTooltip( 7, p );
		pBar->SetButtonText( 7, p );
		return;
	}
}

//'14.08.01
void CTaxationListView::OnSize(UINT nType, int cx, int cy)
{
	ICSFormView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。

	if( m_TabMng.GetCount() && m_pFirstPage ){

		RECT rect = { 0 };
		m_pFirstPage->GetClientRect( &rect );

		int max = (int)m_TabMng.GetCount();
		for( int i=0; i<max; i++ ){
			if( m_TabMng[i].pWnd ){
				m_TabMng[i].pWnd->MoveWindow( &rect );
			}

		}
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

}

//キー対応'14.08.01
BOOL CTaxationListView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			if( m_pShinInfo && m_pShinInfo->pParent ){
//--> '15.07.31 INS START
				m_pShinInfo->pParent->SendMessage( WM_USER_SET_ENDSTART, 0 );
//<-- '15.07.31 INS END
				m_pShinInfo->pParent->PostMessage( WM_CLOSE, 0 );
				return TRUE;
			}
		}
		else if( pMsg->wParam == VK_RETURN ){
			if( m_TabMng.GetCount() ){

				//yoshida150311
				if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){//課税取引
					SetItemPosition( m_MainTab.GetCurSel() );
				}
				else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){//特定収入
					SetItemPositionEx( m_MainTab.GetCurSel() );
				}

			//	SetItemPosition( m_MainTab.GetCurSel() );
				return TRUE;
			}
		}
	}
	else if( (pMsg->message==WM_MOUSEWHEEL) ){
		if( pMsg->hwnd == GetSafeHwnd() ){//今処理しているウィンドウハンドルとviewのウィンドウハンドル
			CWnd* pWnd = WindowFromPoint( pMsg->pt );
			if( pWnd && IsChild(pWnd) ){
				pMsg->hwnd = pWnd->GetSafeHwnd();
			}
		}
	}

	return ICSFormView::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// 顧問先情報の取り込み
//-----------------------------------------------------------------------------
void CTaxationListView::OnUpdateKomonsakiInf(CCmdUI *pCmdUI)
{
	//オプションの顧問先情報の取込は無効にする
	pCmdUI->Enable( FALSE );
	
}

#ifdef _20150209_EXTEND_
//'14.08.05
//-----------------------------------------------------------------------------
// 全画面：画面表示 再初期化
//-----------------------------------------------------------------------------
// 引数	isCompChange	：	強制再初期化？
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CTaxationListView::ReInitDisp( BOOL isCompChange )
{

	BOOL	bRecreateTab = FALSE;
	if( isCompChange ){
		bRecreateTab = TRUE;
	}

	if( bRecreateTab ){
		InitTab( TRUE );
		//-->追加 harino ('14.08.08)
		PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセット
		//<--追加 harino ('14.08.08)
	}
	else {

		// 課税仕入高
		if( m_pShinBuy ){
			m_pShinBuy->Init();
		}

		// 課税売上高
		if( m_pShinSell ){
			m_pShinSell->Init();
		}

		// 非連動ｄｂで簡易課税
		// 事業別売上高
		if( m_pShinKniinp ){
			m_pShinKniinp->Init();
		}

		// 農業所得用
		if( m_pShinNougyou ){
			m_pShinNougyou->Init();
		}

		// 不動産所得用
		if( m_pShinFudousan ){
			m_pShinFudousan->Init();
		}

		// 事業所得用(一般)
		if( m_pShinJigyo ){
			m_pShinJigyo->Init();
		}

		//経過措置の関係-------->
		// 課税仕入(経過措置用)
		if(m_pShinBuyKeika){
			m_pShinBuyKeika->Init();
		}

		// 不動産(経過措置用)
		if(m_pShinFudousanKeika){
			m_pShinFudousanKeika->Init();
		}

		// 農業(経過措置用)
		if(m_pShinNougyouKeika){
			m_pShinNougyouKeika->Init();
		}

		// 事業(経過措置用)
		if(m_pShinJigyoKeika){
			m_pShinJigyoKeika->Init();
		}

		// 課税売上(経過措置用)
		if(m_pShinSellKeika){
			m_pShinSellKeika->Init();
		}

		// 非連動ｄｂで簡易課税
		// 事業別売上高
		if(m_pShinKniinpKeika){
			m_pShinKniinpKeika->Init();
		}
		
		PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセットのタイミングをPostMessageに変更してずらす
	}

	return 0;
}
#else
//-----------------------------------------------------------------------------
// 全画面：画面表示 再初期化
//-----------------------------------------------------------------------------
// 引数	isCompChange	：	強制再初期化？
//		isResetFocus	：	フォーカスの再セットを行うか？
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
//2016.10.20 UPDATE START
//int CTaxationListView::ReInitDisp( BOOL isCompChange, BOOL isResetFocus/*=TRUE*/ )
int CTaxationListView::ReInitDisp( BOOL isCompChange, BOOL SixKindFlg, BOOL isResetFocus/*=TRUE*/ )
//2016.10.20 UPDATE END
{
//2016.10.20 INSERT START
	BOOL	svSixKindFlg = m_SixKindFlg;

	m_SixKindFlg = SixKindFlg;
//2016.10.20 INSERT END

	BOOL	bRecreateTab = FALSE;
	if( isCompChange ){
		bRecreateTab = TRUE;
	}
//2016.10.20 INSERT START
	else{
		if( svSixKindFlg != SixKindFlg ){
			bRecreateTab = TRUE;
		}
	}
//2016.10.20 INSERT END

	if( bRecreateTab ){
		InitTab( TRUE );
		//-->追加 harino ('14.08.08)
		if( isResetFocus ){
			PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセット
		}
		//<--追加 harino ('14.08.08)
	}
	else{
		if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
			ReInitTaxDisp();
		}
		else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
			ReInitSpcDisp();
		}
		if( isResetFocus ){
			PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセットタイミングをずらす
		}
	}

	return 0;
}
#endif

//--> '15.02.09 INS START
//-----------------------------------------------------------------------------
// 全画面：画面表示 課税取引金額計算表 再初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::ReInitTaxDisp()
{
	// 課税仕入高
	if( m_pShinBuy ){
		m_pShinBuy->Init();
	}

	// 課税売上高
	if( m_pShinSell ){
		m_pShinSell->Init();
	}

	// 非連動ｄｂで簡易課税
	// 事業別売上高
	if( m_pShinKniinp ){
		m_pShinKniinp->Init();
	}
//2016.10.20 INSERT START
	if( m_pShinKniinp6syu ){
		m_pShinKniinp6syu->Init();
	}
//2016.10.20 INSERT END
	// 農業所得用
	if( m_pShinNougyou ){
		m_pShinNougyou->Init();
	}

	// 不動産所得用
	if( m_pShinFudousan ){
		m_pShinFudousan->Init();
	}

	// 事業所得用(一般)
	if( m_pShinJigyo ){
		m_pShinJigyo->Init();
	}

	//経過措置の関係-------->
	// 課税仕入(経過措置用)
	if(m_pShinBuyKeika){
		m_pShinBuyKeika->Init();
	}

	// 不動産(経過措置用)
	if(m_pShinFudousanKeika){
		m_pShinFudousanKeika->Init();
	}

	// 農業(経過措置用)
	if(m_pShinNougyouKeika){
		m_pShinNougyouKeika->Init();
	}

	// 事業(経過措置用)
	if(m_pShinJigyoKeika){
		m_pShinJigyoKeika->Init();
	}

	// 課税売上(経過措置用)
	if(m_pShinSellKeika){
		m_pShinSellKeika->Init();
	}

	// 非連動ｄｂで簡易課税
	// 事業別売上高
	if(m_pShinKniinpKeika){
		m_pShinKniinpKeika->Init();
	}
//2016.10.20 INSERT START
	if(m_pShinKniinpKeika6syu){
		m_pShinKniinpKeika6syu->Init();
	}
//2016.10.20 INSERT END
	return 0;
}
//--> '15.02.09 INS END

//-----------------------------------------------------------------------------
// 現在の入力位置にフォーカスセット
//-----------------------------------------------------------------------------
// 引数	IsInEnd	：	終了処理中？
//-----------------------------------------------------------------------------
void CTaxationListView::SetNowPosition( BOOL IsInEnd/*=FALSE*/ )
{
	//使われてないが一応対応入れておくyoshida150311
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){//課税取引
		SetItemPosition( m_MainTab.GetCurSel() );
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){//特定収入
		SetItemPositionEx( m_MainTab.GetCurSel(), IsInEnd );
	}

//	SetItemPosition( m_MainTab.GetCurSel() );
}


//-------------->F12キー対応 '14.08.08
// 次表
void CTaxationListView::OnButtonF12()
{
	PageChange(1);	// 1:次へ
	return;
}

void CTaxationListView::OnUpdateButtonF12(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_MainTab.GetItemCount() == 1 )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( TRUE );
}

//-----------------------------------------------------------------------------
// ページ切替
//-----------------------------------------------------------------------------
// 引数	move	：	1:順送り、-1:逆送り
//-----------------------------------------------------------------------------
void CTaxationListView::PageChange(int move)
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

//	// 各表の更新前処理
//	UpdateSheet( selno );

	// 各表の更新処理後の再表示
	m_MainTab.SetCurSel( movesel );
//	IndicateSheet( movesel );
//	SetItemPosition( movesel );

//	m_nowTabIdx = movesel;
}
//<---------------------

//-----------------------------------------------------------------------------
// フォーカスセット
//-----------------------------------------------------------------------------
// 引数	selno	：	セットするタブNO
//-----------------------------------------------------------------------------
void CTaxationListView::SetItemPosition( int selno )
{
	if( (int)m_TabMng.GetCount() <= selno ){
		return;
	}

	CSyzBaseDlg	*pBaseList = NULL;


	switch( m_TabMng[selno].tabtype ){
		case ID_ICSSH_GENERAL_LIST_TAB :		// 一般
			if( m_pShinJigyo ){
				pBaseList = m_pShinJigyo;
			}
			break;
		case ID_ICSSH_REALESTATE_LIST_TAB :		// 不動産
			if( m_pShinFudousan ){
				pBaseList = m_pShinFudousan;
			}
			break;
		case ID_ICSSH_AGRICULTUTRE_LIST_TAB :	// 農業
			if( m_pShinNougyou ){
				pBaseList = m_pShinNougyou;
			}
			break;
		case ID_ICSSH_SALES_LIST_TAB :			// 課税売上高
			if( m_pShinSell ){
				pBaseList = m_pShinSell;
			}
			break;
		case ID_ICSSH_STOCK_LIST_TAB :			// 課税仕入高
			if( m_pShinBuy ){
				pBaseList = m_pShinBuy;
			}
			break;
		case ID_ICSSH_BUSINESS_SALES_LIST_TAB :	// 事業別売上高
			if( m_pShinKniinp ){
				pBaseList = m_pShinKniinp;
			}
			
			break;


		//----------------------->'14.08.18
		case ID_ICSSH_KEI_GENERAL_LIST_TAB :		// 一般（経過措置用）
			if( m_pShinJigyoKeika ){
				pBaseList = m_pShinJigyoKeika;
			}
			break;
		case ID_ICSSH_KEI_REALESTATE_LIST_TAB :		// 不動産（経過措置用）
			if( m_pShinFudousanKeika ){
				pBaseList = m_pShinFudousanKeika;
			}
			break;
		case ID_ICSSH_KEI_AGRICULTUTRE_LIST_TAB :	// 農業（経過措置用）
			if( m_pShinNougyouKeika ){
				pBaseList = m_pShinNougyouKeika;
			}
			break;
		case ID_ICSSH_KEI_SALES_LIST_TAB :			// 課税売上高（経過措置用）
			if( m_pShinSellKeika ){
				pBaseList = m_pShinSellKeika;
			}
			break;
		case ID_ICSSH_KEI_STOCK_LIST_TAB :			// 課税仕入高（経過措置用）
			if( m_pShinBuyKeika ){
				pBaseList = m_pShinBuyKeika;
			}
			break;
		case ID_ICSSH_KEI_BUSINESS_SALES_LIST_TAB :	// 事業別売上高（経過措置用）
			if( m_pShinKniinpKeika ){
				pBaseList = m_pShinKniinpKeika;
			}
			break;
		//<------------------------------
//2016.10.20 INSERT START
		case ID_ICSSH_6SYU_BUSINESS_SALES_LIST_TAB :	// 事業別売上高
			if( m_pShinKniinp6syu ){
				pBaseList = m_pShinKniinp6syu;
			}
			break;
		case ID_ICSSH_KEI_6SYU_BUSINESS_SALES_LIST_TAB :	// 事業別売上高（経過措置用）
			if( m_pShinKniinpKeika6syu ){
				pBaseList = m_pShinKniinpKeika6syu;
			}
			break;
//2016.10.20 INSERT END

		default:
			break;
	}

	if( pBaseList ){
		pBaseList->SetItemPosition( );
	}

}

//-----------------------------------------------------------------------------
// 課税取引金額計算書との連動（'14.08.08）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::ConnectedTaxList()
{

	if( m_pTaxListData ){
		if( m_pTaxListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}

//		m_pTaxListData->ConnectZmdata( m_pZmSub );
		ASSERT( m_pShinInfo );
		if( m_pShinInfo == NULL ){
			return -1;
		}
		//m_pTaxListData->ConnectZmdata( m_pZmSub, m_pSyzSyukei, m_pShinInfo->s_tno, m_pShinInfo->d_tno, m_pSnHeadData->SVmzsw==1 ? TRUE:FALSE );
		m_pTaxListData->ConnectZmdata ( m_pZmSub, m_pSyzSyukei, m_pShinInfo->s_tno, m_pShinInfo->d_tno, ( m_pSnHeadData->SVmzsw == 1 ? TRUE : FALSE ), m_pSnHeadData );

		if( m_pTaxListData->SetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入計算表との連動（'15.02.13）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::ConnectedSpcList()
{
	if( m_pSpcListData ){
//		if( m_pSpcListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
//			return -1;
//		}

		ASSERT( m_pShinInfo );
		if( m_pShinInfo == NULL ){
			return -1;
		}
		m_pSpcListData->InitSpData( m_pZmSub );
//--> '15.04.02 INS START
		if( m_pSpcListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
//<-- '15.04.02 INS END
		m_pSpcListData->CalqForUriRatio( m_pZmSub, m_pSyzSyukei, m_pSnHeadData, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
		m_pSpcListData->SPassOffCalqMain( m_pSnHeadData );

		// サインON
		char	WORK0[MONY_BUF_SIZE]={0};
		m_pSnHeadData->Sn_Sign2 &= 0xfd;
		m_Arith.l_input( WORK0, _T("50") );
		if( m_Arith.l_cmp(m_pSpcListData->stval.SpTswr, WORK0) > 0 ){
			// 金額転記
			SetHonpyoMony();
		}
		else{
			m_pSnHeadData->Sn_Sign2 |= 0x02;
		}

		if( m_pSpcListData->SetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}
	return 0;
}

//---------->'14.09.01
//-----------------------------------------------------------------------------
// 履歴シーケンス切替
//-----------------------------------------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::ChangeSnSeq( int snseq )
{
	if( m_SvSnSeq == snseq ){
		return 0;
	}

//-- '15.02.09 --
//	// 既存データの書込み
//	if( WriteData(TRUE) ){
////	if( WriteData(TRUE, FALSE) ){
//		return -1;
//	}
//
//	// 新履歴番号セット
//	SetSnSeq( snseq );
//
//	// 新規データの取得
//	// 帳票データ
//	if( !m_pTaxListData ){
//		if( (m_pTaxListData = new CH26TaxListData()) == NULL ){
//			return -1;
//		}
//		if( m_pTaxListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
//			return -1;
//		}
//	}
//
////	ReGetData();
//---------------
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		// 既存データの書込み
		if( WriteData(TRUE) ){
			return -1;
		}
		// 新履歴番号セット
		SetSnSeq( snseq );

		// 新規データの取得
		// 帳票データ
		if( !m_pTaxListData ){
			if( (m_pTaxListData = new CH26TaxListData()) == NULL ){
				return -1;
			}
			if( m_pTaxListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		// 既存データの書込み
		if( WriteSpcData(TRUE) ){
			return -1;
		}
		// 新履歴番号セット
		SetSnSeq( snseq );

		// 帳票データ
		if( !m_pSpcListData ){
			if( (m_pSpcListData = new CH26SpcListData(m_pSnHeadData->SVmzsw ? TRUE:FALSE)) == NULL ){
				return -1;
			}
			if( m_pSpcListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}
//---------------

	return 0;
}

//-----------------------------------------------------------------------------
// 履歴シーケンスセット
//-----------------------------------------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//-----------------------------------------------------------------------------
void CTaxationListView::SetSnSeq( int snseq )
{
	m_SnSeq = snseq;
	m_SvSnSeq = snseq;
}

#ifdef _20150512_EXTEND_
//-----------------------------------------------------------------------------
// 現データを消去して、データの再読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::ReReadData()
{
	// 帳票データ
	if( m_pTaxListData ){
		delete m_pTaxListData;
		m_pTaxListData = NULL;

		if( (m_pTaxListData = new CH26TaxListData()) == NULL ){
			return -1;
		}
		if( m_pTaxListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}

	return 0;
}
#else
//-----------------------------------------------------------------------------
// 現データを消去して、データの再読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::ReReadData()
{
	// 帳票データ
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( m_pTaxListData ){
			delete m_pTaxListData;
			m_pTaxListData = NULL;

			if( (m_pTaxListData = new CH26TaxListData()) == NULL ){
				return -1;
			}
			if( m_pTaxListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		if( m_pSpcListData ){
			delete m_pSpcListData;
			m_pSpcListData = NULL;

			ASSERT(m_pSnHeadData);
			if( m_pSnHeadData == NULL ){
				return -1;
			}
			if( (m_pSpcListData = new CH26SpcListData(m_pSnHeadData->SVmzsw ? TRUE:FALSE)) == NULL ){
				return -1;
			}
			if( m_pSpcListData->InitSpData(m_pZmSub) ){
				return -1;
			}
			if( m_pSpcListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
		}
	}

	return 0;
}
#endif

//<-----------------------------------

//'14.10.08
//-----------------------------------------------------------------------------
// 転記データ取得　（本表に転記するデータを取得）
//-----------------------------------------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//-----------------------------------------------------------------------------
void CTaxationListView::GetTenkiData( SH_TENKIDATA* tenki_data )
{
	memmove(tenki_data , &m_pShzCalc->tenkidata , sizeof(*tenki_data));
}


// 手入力サインのクリア
int CTaxationListView::ClearManualInputSign()
{
	int rt = 0;

	if( m_pShinBuyKeika ){
		m_pShinBuyKeika->ClearManualInputSign();
	}
	if( m_pShinSellKeika ){
		m_pShinSellKeika->ClearManualInputSign();
	}
	if( m_pShinJigyoKeika ){
		m_pShinJigyoKeika->ClearManualInputSign();
	}
	if( m_pShinFudousanKeika ){
		m_pShinFudousanKeika->ClearManualInputSign();
	}
	if( m_pShinNougyouKeika ){
		m_pShinNougyouKeika->ClearManualInputSign();
	}
//	if( m_pShinKniinpKeika ){
//		m_pShinKniinpKeika->ClearManualInputSign();
//	}


	if( m_pShinBuy ){
		m_pShinBuy->ClearManualInputSign();
	}
	if( m_pShinSell ){
		m_pShinSell->ClearManualInputSign();
	}
	if( m_pShinJigyo ){
		m_pShinJigyo->ClearManualInputSign();
	}
	if( m_pShinFudousan ){
		m_pShinFudousan->ClearManualInputSign();
	}
	if( m_pShinNougyou ){
		m_pShinNougyou->ClearManualInputSign();
	}
//	if( m_pShinKniinp ){
//		m_pShinKniinp->ClearManualInputSign();
//	}

	return rt;
}

//-----------------------------------------------------------------------------
// 金額の再取得
//-----------------------------------------------------------------------------
// 引数	isResetFocus	：	フォーカスの再セットを行うかどうか
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::ReGetCalqedMony( BOOL isResetFocus )
{
	//再表示
//-- '15.03.26 --
//	ReInitDisp(FALSE);
//---------------
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		ReInitDisp( FALSE, isResetFocus );
	}
	else if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
		ReInitDisp( TRUE, isResetFocus );
	}
//---------------

	return 0;

	
}

//---------->'15.01.09
//-----------------------------------------------------------------------------
// 過年度プロ呼出し後にデータを吸い上げる
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::GetPastData()
{
	// 帳票データ
	if( !m_pTaxListData ){
		if( (m_pTaxListData = new CH26TaxListData()) == NULL ){
			return -1;
		}
	}
	if( m_pTaxListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
		return -1;
	}
	return 0;
}
//<-----------------------

//-----------------------------------------------------------------------------
// 消費税簡易計算表　ダイアログ表示（経過措置あり）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::SpecPassOnDlg()
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	double	x, y;
	CSpecPassOn	dlg(this);
	dlg.Init( &m_pSnHeadData, &m_pSpcListData, &m_Arith );
	GetRate( x, y );
	if( y > 1.15 ){
		y = 1.15;
	}
	dlg.DoModal();

	AfxSetResourceHandle( hInst );

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税簡易計算表　ダイアログ表示（経過措置なし）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::SpecPassOffDlg()
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	double	x, y;
	CSpecPassOff	dlg(this);
	dlg.Init( &m_pSnHeadData, &m_pSpcListData, &m_Arith );
	GetRate( x, y );
	if( y > 1.15 ){
		y = 1.15;
	}
	dlg.DoModal();

	AfxSetResourceHandle( hInst );

	return 0;
}

//2015.03.05 INSERT START
//-----------------------------------------------------------------------------
// 消費税簡易計算表　ダイアログ表示（経過措置なし）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::SpecPassOffExDlg()
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	double	x, y;
	CSpecPassOffEx	dlg(this);
	dlg.Init( &m_pSnHeadData, &m_pSpcListData, &m_Arith );
	GetRate( x, y );
	if( y > 1.15 ){
		y = 1.15;
	}
	dlg.DoModal();

	AfxSetResourceHandle( hInst );

	return 0;
}
//2015.03.05 INSERT END

//-----------------------------------------------------------------------------
// 再初期化
//-----------------------------------------------------------------------------
LRESULT CTaxationListView::OnUserShintabInit( WPARAM wParam, LPARAM lParam ) 
{
	InitTabMain( TRUE );//yoshida150321
//	InitTabMain( FALSE );

	return 0;
}

//-----------------------------------------------------------------------------
// 入力ロック切替時の再表示 ('16.07.22)
//-----------------------------------------------------------------------------
void CTaxationListView::ChgLockDisp()
{
	int max = (int)m_TabMng.GetCount();
	for( int i=0; i<max; i++ ){
		if( m_TabMng[i].pWnd ){
			((CSyzBaseDlg *)m_TabMng[i].pWnd)->Static_Update( m_pSnHeadData->Sn_Sign4 );
		}
	}
}

//2017.12.04 INSERT START
BOOL CTaxationListView::IsEmpty_SonekiSyunyu()
{
	BOOL ret = TRUE;

	if((m_pSnHeadData->Sn_SKKBN==0x02) && (m_pSnHeadData->Sn_Syuusei&0x08)){ //予定（26号）は判定しない
		return TRUE;
	}

	if( m_pTaxListData != NULL ){
		KKVAL*	pU = (*m_pTaxListData).NRec.KUval;

		if( m_Arith.l_test((pU+9)->KKval1) != 0 ){
			ret = FALSE;
		}
	}

	return ret;
}

BOOL CTaxationListView::IsEmpty_SonekiShiireKeihi()
{
	BOOL ret = TRUE;

	if((m_pSnHeadData->Sn_SKKBN==0x02) && (m_pSnHeadData->Sn_Syuusei&0x08)){ //予定（26号）は判定しない
		return TRUE;
	}

	if( m_pTaxListData != NULL ){
		KKVAL*	pS = (*m_pTaxListData).NRec.KSval;

		if( m_Arith.l_test((pS+9)->KKval1) != 0 ){
			ret = FALSE;
		}
	}

	return ret;
}
//2017.12.04 INSERT END

#ifdef _20190628_EXTEND_
//-----------------------------------------------------------------------------
// 平成継続出力？
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//-----------------------------------------------------------------------------
bool CTaxationListView::IsJcContinuePrint()
{
	if( m_JcContinuePrint == -1 ){
		m_JcContinuePrint = 0;
		if( m_Util.IsJcContinuePrint(m_pSnHeadData) ){
			m_JcContinuePrint = 1;
		}
	}

	if( m_JcContinuePrint == 1 ){
		return true;
	}
	else{
		return false;
	}
}
#else
// この対応で、m_JcContinuePrintは不要になりましたが、変数としては残しています。
//-----------------------------------------------------------------------------
// 平成継続出力？
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//-----------------------------------------------------------------------------
bool CTaxationListView::IsJcContinuePrint()
{
	if( m_Util.IsJcContinuePrint(m_pSnHeadData) ){
		return true;
	}
	return false;
}
#endif
