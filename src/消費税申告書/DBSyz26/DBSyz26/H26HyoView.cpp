// H26HyoView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "H26HyoView.h"

#include "ShinIppan.h"
#include "ShinKani.h"
#include "ShinKaniEX.h"
#include "ShinFhyo40.h"
#include "ShinFhyo40EX.h"
#include "ShinFhyo52.h"
#include "ShinFhyo52EX.h"
#include "ShinFhyo522.h"
#include "ShinFhyo522EX.h"
#include "ShinFhyo50.h"
#include "ShinFhyo50EX.h"
//2018.03.13 INSERT START
#include "ShinFhyo60.h"
//2018.03.13 INSERT END
#include "ShinFhyo10.h"
#include "ShinFhyo22.h"
#include "ShinFhyo20.h"
//2015.07.02 INSERT START(DEBUG)
#include "ShinFhyo10EX.h"
#include "ShinFhyo22EX.h"
#include "ShinFhyo20EX.h"
//2015.07.02 INSERT END(DEBUG)
//----->特定課税仕入対応yoshida150713
#include "ShinKaniEX2.h"
#include "ShinIppanEX.h"
#include "ShinChskEX.h"
#include "ShinHjnKskEX.h"
#include "ShinKjnKskEX.h"
#include "ShinHjnKskEX.h"
#include "ShinKjnKskEX.h"
#include "TKUtiwake.h"
//<--------------------------------




#include ".\SyzrefundDB\ShinRefundKjn01.h"
#include ".\SyzrefundDB\ShinRefundKjn02.h"
#include ".\SyzrefundDB\ShinRefundHjn01.h"
#include ".\SyzrefundDB\ShinRefundHjn02.h"

#include "ShinHjnKsk.h"
#include "ShinKjnKsk.h"

#include "ShinChsk.h"
#include "ShinKskRnd.h"

// CH26HyoView

extern	HINSTANCE g_hInstance;


IMPLEMENT_DYNCREATE(CH26HyoView, ICSFormView)

CH26HyoView::CH26HyoView()
	: CBaseHyoView(CH26HyoView::IDD)
	, m_pZmSub(NULL)
	, m_pIppan(NULL)
	, m_pKani(NULL)
	, m_pKaniEX(NULL)
	, m_pFhyo40(NULL)
	, m_pFhyo40EX(NULL)
	, m_pFhyo52(NULL)
	, m_pFhyo52EX(NULL)
	, m_pFhyo522(NULL)
	, m_pFhyo522EX(NULL)
	, m_pFhyo50(NULL)
	, m_pFhyo50EX(NULL)
	, m_pFhyo10(NULL)
	, m_pFhyo22(NULL)
	, m_pFhyo20(NULL)
	, m_pRefundKjn01(NULL)
	, m_pRefundKjn02(NULL)
	, m_pRefundHjn01(NULL)
	, m_pRefundHjn02(NULL)
	, m_pHjnKsk(NULL)
	, m_pKjnKsk(NULL)
	, m_pChsk(NULL)
	//----->特定課税仕入対応yoshida150713
	, m_pChskEX(NULL)//yoshida150805
	, m_pIppanEX(NULL)
	, m_pKaniEX2(NULL)
	, m_pFhyo10EX(NULL)
	, m_pFhyo20EX(NULL)
	, m_pFhyo22EX(NULL)
	, m_pHjnKskEX(NULL)
	, m_pKjnKskEX(NULL)
	, m_pTKUtiwake(NULL)
	//<------------------------------
	, m_pShinInfo(NULL)
	, m_pSnHonpyoData(NULL)
	, m_pSnFhyo20Data(NULL)
	, m_pSnFhyo22Data(NULL)
	, m_pSnFhyo50Data(NULL)
	, m_pSnFhyo52Data(NULL)
	, m_pSnFhyo10Data(NULL)
	, m_pSnFhyo40Data(NULL)
	, m_pSnTanaData(NULL)
	, m_pSnRefundData(NULL)
	, m_pSnChskData(NULL)
	, m_pSnKskData(NULL)
	, m_SnSeq(-1)
	, m_SvSnSeq(-1)
	, m_pEselect(NULL)
	, m_pIsOpenEselect(NULL)
	, m_pSyzSyukei(NULL)
	, m_pVprn(NULL)
	, m_pPset(NULL)
	, m_pPrintCmInfo(NULL)
	, m_pTblhdl(NULL)
	, m_pSkjCnvtbl(NULL)
	, m_pFirstPage(NULL)
	, m_bShowed(FALSE)
	, m_bNeedFocusSet(FALSE)
	, m_nowTabIdx(0)
	, m_pSubSyzSyukei(NULL)
	, m_SixKindFlg(FALSE)				// [Y]150106
	//----->特定課税仕入対応yoshida150713
//	, m_TokuteiFlg(FALSE)
	//<------------------------------
	, m_ShRevType(ID_ICSSH_NO_REVTYPE)	// '15.08.26
//2016.02.23 INSERT START
	, m_Syokai_NukiKomi(FALSE)
//2016.02.23 INSERT END
//2018.03.13 INSERT START
	, m_pFhyo60(NULL)
	, m_pSnFhyo60Data(NULL)
//2018.03.13 INSERT END
	, m_isEndStart(FALSE)
//2018.08.21 INSERT START
	, m_NoHistory_browse(FALSE)
//2018.08.21 INSERT END
	, m_JcContinuePrint(-1)
	, m_isInited(false)
{
}

CH26HyoView::~CH26HyoView()
{
}

void CH26HyoView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_MainTab);
}

BEGIN_MESSAGE_MAP(CH26HyoView, ICSFormView)
	ON_MESSAGE(WM_USER_RECALQ_TKKZ, OnUserRecalqTkkz)	// '15.08.21
	ON_COMMAND(ID_BUTTON_F2, OnButtonF2)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_BUTTON_F7, OnButtonF7)
	ON_NOTIFY(TCN_SELCHANGING, IDC_MAIN_TAB, OnTcnSelchangingMainTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, OnTcnSelchangeMainTab)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CH26HyoView::OnUpdateButtonF9)
	ON_COMMAND(ID_BUTTON_F9, &CH26HyoView::OnButtonF9)
	ON_COMMAND(ID_BUTTON_F6, OnButtonF6)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F6, &CH26HyoView::OnUpdateButtonF6)
	ON_COMMAND(ID_ZEIRISHI_INF, &CH26HyoView::OnZeirishiInf)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, &CH26HyoView::OnUpdateButtonF11)
	ON_UPDATE_COMMAND_UI(ID_ZEIRISHI_INF, &CH26HyoView::OnUpdateZeirishiInf)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_KOMONSAKI_INF, &CH26HyoView::OnUpdateKomonsakiInf)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CH26HyoView::OnUpdateButtonF7)
	ON_COMMAND(ID_BUTTON_F8, &CH26HyoView::OnButtonF8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, &CH26HyoView::OnUpdateButtonF8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F4, &CH26HyoView::OnUpdateButtonF4)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CH26HyoView::OnUpdateButtonF5)
	ON_COMMAND(ID_BUTTON_F12, OnButtonF12)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, &CH26HyoView::OnUpdateButtonF12)
	ON_MESSAGE( WM_MYMESSAGE, OnMyMessage )
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CH26HyoView 診断

#ifdef _DEBUG
void CH26HyoView::AssertValid() const
{
	ICSFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CH26HyoView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}
#endif
#endif //_DEBUG


//-->追加 harino ('14.03.05)
LRESULT CH26HyoView::OnMyMessage( WPARAM wParam, LPARAM lParam )
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

//-->追加 harino ('14.03.05)

// CH26HyoView メッセージ ハンドラ

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
int CH26HyoView::InitView( CDBNpSub *pZmsub, CSyzSyukei *pSyzSyukei, CSyzSyukei *pSubSyzSyukei, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl , BOOL SixKindFlg )//[Y]150107
//int CH26HyoView::InitView( CDBNpSub *pZmsub, CSyzSyukei *pSyzSyukei, CSyzSyukei *pSubSyzSyukei, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl )
{
	m_isInited = false;

	m_SixKindFlg = SixKindFlg;//[Y]150107

	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		return -1;
	}
	m_pZmSub = pZmsub;

	ASSERT( pSyzSyukei );
	if( pSyzSyukei == NULL ){
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

//	ASSERT( pSubSyzSyukei );
//	if( pSubSyzSyukei == NULL ){
//		return -1;
//	}
	m_pSubSyzSyukei = pSubSyzSyukei;

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

	ASSERT( pTblhdl );
	if( pTblhdl == NULL ){
		return -1;
	}
	m_pTblhdl = pTblhdl;

	ASSERT( pSkjCnvtbl );
	if( pSkjCnvtbl == NULL ){
		return -1;
	}
	m_pSkjCnvtbl = pSkjCnvtbl;

	// 倍長演算
	m_Arith.l_defn( 0x16 );

	GetClientRect( vwRect );

	// データ変換
	BOOL isNeedCnv = IsNeedOldDataCnv();

	// 内部タブの初期化
/*- '14.05.13 -*/
//	if( InitTab(TRUE) ){
//		m_ErrMsg = _T("本表・付表画面の内部作成に失敗しました。");
//		return -1;
//	}
/*-------------*/
	if( isNeedCnv || IsNeedInitNendo() ){
		// 年度が必要な年度？
		if( InitTab(TRUE) ){
			m_ErrMsg = _T("本表・付表画面の内部作成に失敗しました。");
			return -1;
		}
	}
/*-------------*/

	// データ変換
	if( isNeedCnv ){
		MakeSnkData();
		GetPastData();
	}

//2016.02.23 INSERT START
	if( m_pZmSub->szvol->SVsasw&0x01 ){
		m_Syokai_NukiKomi = FALSE;
	}else{
		m_Syokai_NukiKomi = TRUE;
	}
//2016.02.23 INSERT END

	m_isInited = true;

	return 0;
}

//-----------------------------------------------------------------------------
// 旧データ読込み必要性のチェック
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	必要性あり
//			FALSE	：	必要性無し
//-----------------------------------------------------------------------------
BOOL CH26HyoView::IsNeedOldDataCnv()
{
	if( m_pZmSub ){
		if( m_pZmSub->szsnk == NULL ){
			if( m_pZmSub->SzsnkOpen() ){
				return FALSE;
			}
		}
		else{
			m_pZmSub->szsnk->Requery( _T("Sn_SEQ = 0") );
		}
	}
	if( m_pZmSub->szsnk->st == -1 ){
		return FALSE;
	}

	CDBSyzAccess	DBSyzAccess;
	if( DBSyzAccess.OpenShLstVal(m_pZmSub->m_database) ){
		return FALSE;
	}
	int st = DBSyzAccess.m_pSH_LST_VAL->st;

	DBSyzAccess.CloseShLstVal();

	if( st == -1 ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 申告書用データ作成
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH26HyoView::MakeSnkData()
{
	// 本表
	if( m_pSnHonpyoData ){

		//---------->yoshida150810
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq, 0x01 );
//---------------
			((CH28SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq, 0x01 );
//---------------
		}
		else{
			if(m_SixKindFlg == TRUE){
				((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq, 0x01 );
			}
			else{
				m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq, 0x01 );
			}
		}
		//------------------------
		/*
		//--->[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq, 0x01 );
		}
		else{
			m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq, 0x01 );
		}
		//<----------------
		*/
		//<-----------------------
	}
	// 付表２
	if( m_pSnFhyo20Data ){
		//---->yoshida150810
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo20Data*)m_pSnFhyo20Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo20Data->SetData( m_pZmSub, m_SnSeq );
		}
		//------------------
//		m_pSnFhyo20Data->SetData( m_pZmSub, m_SnSeq );
		//<-----------------
	}
	if( m_pSnFhyo22Data ){
		//---->yoshida150810
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo20Data*)m_pSnFhyo22Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo22Data->SetData( m_pZmSub, m_SnSeq );
		}
		//-------------------
		//m_pSnFhyo22Data->SetData( m_pZmSub, m_SnSeq );
		//<-------------------
	}
	// 付表１
	if( m_pSnFhyo10Data ){
		//---->yoshida150810
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo10Data->SetData( m_pZmSub, m_SnSeq );
		}

		//-----------------
		//m_pSnFhyo10Data->SetData( m_pZmSub, m_SnSeq );
		//<----------------
	}
	// 付表５
	if( m_pSnFhyo50Data ){
		//--->[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo50Data*)m_pSnFhyo50Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo50Data->SetData( m_pZmSub, m_SnSeq );
		}
		//<----------------
	}
	if( m_pSnFhyo52Data ){
		//--->[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo50Data*)m_pSnFhyo52Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo52Data->SetData( m_pZmSub, m_SnSeq );
		}
		//<---------------
	}
	// 付表４
	if( m_pSnFhyo40Data ){
		//--->[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo10Data*)m_pSnFhyo40Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo40Data->SetData( m_pZmSub, m_SnSeq );
		}
		//<-----------------
	}
	// 棚卸データ
	if( m_pSnTanaData ){
		m_pSnTanaData->SetData( m_pZmSub, m_SnSeq );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// タブ内のダイアログを全消去
//-----------------------------------------------------------------------------
void CH26HyoView::DeleteInsideDlg()
{
	if( m_pHjnKsk ){
		m_pHjnKsk->SetStartEnd( TRUE );
	}
	if( m_pKjnKsk ){
		m_pKjnKsk->SetStartEnd( TRUE );
	}
//2016.02.22 INSERT START
	if( m_pHjnKskEX ){
		m_pHjnKskEX->SetStartEnd( TRUE );
	}
	if( m_pKjnKskEX ){
		m_pKjnKskEX->SetStartEnd( TRUE );
	}
//2016.02.22 INSERT END

	// ビュー内部からフォーカスを外す！
	m_MainTab.SetFocus();

	if( m_pIppan ){
		m_pIppan->EndJob();
		m_pIppan->DestroyWindow();
		delete m_pIppan;
		m_pIppan = NULL;
	}
	if( m_pKani ){
		m_pKani->EndJob();
		m_pKani->DestroyWindow();
		delete m_pKani;
		m_pKani = NULL;
	}
	if( m_pFhyo40 ){
		m_pFhyo40->DestroyWindow();
		delete m_pFhyo40;
		m_pFhyo40 = NULL;
		if( m_pSnFhyo40Data ){
			delete m_pSnFhyo40Data;
			m_pSnFhyo40Data = NULL;
		}
	}
	if( m_pFhyo52 ){
		m_pFhyo52->DestroyWindow();
		delete m_pFhyo52;
		m_pFhyo52 = NULL;
	}

//------->[Y]141212
	if( m_pKaniEX ){
		m_pKaniEX->EndJob();
		m_pKaniEX->DestroyWindow();
		delete m_pKaniEX;
		m_pKaniEX = NULL;
	}
	if( m_pFhyo40EX ){
		m_pFhyo40EX->DestroyWindow();
		delete m_pFhyo40EX;
		m_pFhyo40EX = NULL;
		if( m_pSnFhyo40Data ){
			delete m_pSnFhyo40Data;
			m_pSnFhyo40Data = NULL;
		}
	}
	if( m_pFhyo52EX ){
		m_pFhyo52EX->DestroyWindow();
		delete m_pFhyo52EX;
		m_pFhyo52EX = NULL;
	}
//<--------------------

	//[Y]
	// 表の有無に関係なく、データがあれば破棄するように('14.04.25)
	if( m_pSnFhyo52Data ){
		delete m_pSnFhyo52Data;
		m_pSnFhyo52Data = NULL;
	}

	if( m_pFhyo522 ){
		m_pFhyo522->DestroyWindow();
		delete m_pFhyo522;
		m_pFhyo522 = NULL;
	}
	if( m_pFhyo50 ){
		m_pFhyo50->DestroyWindow();
		delete m_pFhyo50;
		m_pFhyo50 = NULL;
	}
//------->[Y]141212
	if( m_pFhyo522EX ){
		m_pFhyo522EX->DestroyWindow();
		delete m_pFhyo522EX;
		m_pFhyo522EX = NULL;
	}
	if( m_pFhyo50EX ){
		m_pFhyo50EX->DestroyWindow();
		delete m_pFhyo50EX;
		m_pFhyo50EX = NULL;
	}
//<--------------------

//2018.03.13 INSERT START
	if( m_pFhyo60 ){
		m_pFhyo60->DestroyWindow();
		delete m_pFhyo60;
		m_pFhyo60 = NULL;
	}
//2018.03.13 INSERT END
	//[Y]
	// 表の有無に関係なく、データがあれば破棄するように('14.04.25)
	if(m_pSnFhyo50Data){
		delete m_pSnFhyo50Data;
		m_pSnFhyo50Data = NULL;
	}

	if( m_pFhyo10 ){
		m_pFhyo10->DestroyWindow();
		delete m_pFhyo10;
		m_pFhyo10 = NULL;
		if( m_pSnFhyo10Data ){
			delete m_pSnFhyo10Data;
			m_pSnFhyo10Data = NULL;
		}
	}
	if( m_pFhyo22 ){
		m_pFhyo22->DestroyWindow();
		delete m_pFhyo22;
		m_pFhyo22 = NULL;
	}
	//[Y]
	// 表の有無に関係なく、データがあれば破棄するように('14.04.25)
	if(m_pSnFhyo22Data){
		delete m_pSnFhyo22Data;
		m_pSnFhyo22Data = NULL;
	}
	
	//[Y]
	if(m_pSnTanaData){
		delete	m_pSnTanaData;
		m_pSnTanaData = NULL;
	}

	if( m_pFhyo20 ){
		m_pFhyo20->DestroyWindow();
		delete m_pFhyo20;
		m_pFhyo20 = NULL;
	}
	//[Y]
	// 表の有無に関係なく、データがあれば破棄するように('14.04.25)
	if(m_pSnFhyo20Data){
		delete m_pSnFhyo20Data;
		m_pSnFhyo20Data = NULL;
	}

//--> '15.02.20 INS START
	if( m_pSnHonpyoData ){
		delete m_pSnHonpyoData;
		m_pSnHonpyoData = NULL;
	}
//<-- '15.02.20 INS END

	if( m_pRefundKjn01 ){
		m_pRefundKjn01->EndJob();
		m_pRefundKjn01->DestroyWindow();
		delete m_pRefundKjn01;
		m_pRefundKjn01 = NULL;
	}
	if( m_pRefundKjn02 ){
		m_pRefundKjn02->EndJob();
//--> '15.02.18 INS START
		m_pRefundKjn02->pSnHeadData = NULL;
		m_pRefundKjn02->pUtil = NULL;
		m_pRefundKjn02->pZmSub = NULL;
//<-- '15.02.18 INS END
		m_pRefundKjn02->DestroyWindow();
		delete m_pRefundKjn02;
		m_pRefundKjn02 = NULL;
	}
	if( m_pRefundHjn01 ){
		m_pRefundHjn01->EndJob();
		m_pRefundHjn01->DestroyWindow();
		delete m_pRefundHjn01;
		m_pRefundHjn01 = NULL;
	}
	if( m_pRefundHjn02 ){
		m_pRefundHjn02->EndJob();
//--> '15.02.18 INS START
		m_pRefundHjn02->pSnHeadData = NULL;
		m_pRefundHjn02->pUtil = NULL;
		m_pRefundHjn02->pZmSub = NULL;
//<-- '15.02.18 INS END
		m_pRefundHjn02->DestroyWindow();
		delete m_pRefundHjn02;
		m_pRefundHjn02 = NULL;
	}
	if( m_pChsk ){
		m_pChsk->DestroyWindow();
		delete m_pChsk;
		m_pChsk = NULL;
	}
	if( m_pHjnKsk ){
		m_pHjnKsk->DestroyWindow();
		delete m_pHjnKsk;
		m_pHjnKsk = NULL;
	}
	if( m_pKjnKsk ){
		m_pKjnKsk->DestroyWindow();
		delete m_pKjnKsk;
		m_pKjnKsk = NULL;
	}


	//------>特定課税仕入対応yoshida150713
	if( m_pChskEX ){
		m_pChskEX->DestroyWindow();
		delete m_pChskEX;
		m_pChskEX = NULL;
	}

	if( m_pIppanEX ){//一般本表
		m_pIppanEX->DestroyWindow();
		delete m_pIppanEX;
		m_pIppanEX = NULL;
	}

	if( m_pKaniEX2 ){ //簡易本表
//		m_pKaniEX2->EndJob();
		m_pKaniEX2->DestroyWindow();
		delete m_pKaniEX2;
		m_pKaniEX2 = NULL;
	}

	if( m_pFhyo10EX ){//付表１
		m_pFhyo10EX->DestroyWindow();
		delete m_pFhyo10EX;
		m_pFhyo10EX = NULL;
		if( m_pSnFhyo10Data ){
			delete m_pSnFhyo10Data;
			m_pSnFhyo10Data = NULL;
		}
	}

	if( m_pFhyo20EX ){//付表２
		m_pFhyo20EX->DestroyWindow();
		delete m_pFhyo20EX;
		m_pFhyo20EX = NULL;
	}

	if( m_pFhyo22EX ){//付表２－２
		m_pFhyo22EX->DestroyWindow();
		delete m_pFhyo22EX;
		m_pFhyo22EX = NULL;
	}

	if( m_pHjnKskEX ){
		m_pHjnKskEX->DestroyWindow();
		delete m_pHjnKskEX;
		m_pHjnKskEX = NULL;
	}
	if( m_pKjnKskEX ){
		m_pKjnKskEX->DestroyWindow();
		delete m_pKjnKskEX;
		m_pKjnKskEX = NULL;
	}

	if( m_pTKUtiwake ){//特定課税仕入新表
		m_pTKUtiwake->EndJob();
		m_pTKUtiwake->DestroyWindow();
		delete m_pTKUtiwake;
		m_pTKUtiwake = NULL;
	}
	//<-------------------------------

//2018.03.13 INSERT START
	if( m_pFhyo60 ){// 付表６
		m_pFhyo60->DestroyWindow();
		delete m_pFhyo60;
		m_pFhyo60 = NULL;
	}
//2018.03.13 INSERT END



	m_TabMng.RemoveAll();
	m_pFirstPage = NULL;
}

//-----------------------------------------------------------------------------
// 内部タブの初期化
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::InitTab( BOOL bDelete )
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );
	m_bNeedFocusSet = TRUE;

	// 令和プレプリント情報の初期化
	InitReiwaPrnt();
	m_JcContinuePrint = -1;

//2016.02.23 INSERT START
	this->LockWindowUpdate();
//2016.02.23 INSERT END
	int rt = InitTabSub( bDelete );
//2016.02.23 INSERT START
	this->UnlockWindowUpdate();
//2016.02.23 INSERT END

	m_bNeedFocusSet = FALSE;
	AfxSetResourceHandle( hInst );

	return rt;
}

//-----------------------------------------------------------------------------
// 内部タブデータの再取得
//-----------------------------------------------------------------------------
int CH26HyoView::ReGetData()
{
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第26号様式）
		// 中間予定申告データ
		if( !m_pSnChskData ){
			m_pSnChskData = new CH26SnChskData();
			if( m_pSnChskData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
	}
	else{
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
		BOOL	isTransitionalMeasure = FALSE;
		if( m_pSnHeadData->SVmzsw == 1 ){
			isTransitionalMeasure = TRUE;
		}

		if( KzHoushiki == ID_ICSSH_KANNI ){
			// 本表データ読込みクラス
			if( !m_pSnHonpyoData ){

				//----->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//					m_pSnHonpyoData = new CH27SnHonpyoData();
//					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//---------------
					m_pSnHonpyoData = new CH28SnHonpyoData();
					if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
//---------------
				}
				else{
					if(m_SixKindFlg == TRUE){
						m_pSnHonpyoData = new CH27SnHonpyoData();
						if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnHonpyoData = new CH26SnHonpyoData();
						if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
				}

				//--------------------
				/*
				if(m_SixKindFlg == TRUE){
					m_pSnHonpyoData = new CH27SnHonpyoData();
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnHonpyoData = new CH26SnHonpyoData();
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				*/
				//<-------------------
			}
			// 付表４データ読込みクラス
			if( !m_pSnFhyo40Data ){

				//---->[Y]150106
				if(m_SixKindFlg == TRUE){
					m_pSnFhyo40Data = new CH27SnFhyo10Data(isTransitionalMeasure);
					if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnFhyo40Data = new CH26SnFhyo10Data(isTransitionalMeasure);
					if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//<--------------------
			}

			// 経過措置対象の課税資産の譲渡等あり
			if( m_pSnHeadData->SVmzsw == 1 ){
				// データ読込みクラス
				if( !m_pSnFhyo52Data ){
					//---->[Y]150106
					if(m_SixKindFlg == TRUE){
						m_pSnFhyo52Data = new CH27SnFhyo50Data(TRUE);
						if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo52Data = new CH26SnFhyo50Data(TRUE);
						if( m_pSnFhyo52Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//<-------------------
				}
			}
			// 経過措置対象の課税資産の譲渡等なし
			else{
				// データ読込みクラス
				if( !m_pSnFhyo50Data ){
					//---->[Y]150106
					if(m_SixKindFlg == TRUE){
						m_pSnFhyo50Data = new CH27SnFhyo50Data(FALSE);
						if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo50Data = new CH26SnFhyo50Data(FALSE);
						if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//<----------------
				}
			}
		}
		else if( KzHoushiki == ID_ICSSH_GENSOKU ){
			// データ読込みクラス
//2015.08.04 INSERT START
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( !m_pSnFhyo10Data ){
					m_pSnFhyo10Data = new CH28SnFhyo10Data(isTransitionalMeasure);
					if( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->GetData(m_pZmSub, m_SnSeq) ){//yoshida150811
						return -1;
					}
				}
			}else{
//2015.08.04 INSERT END
				if( !m_pSnFhyo10Data ){
					m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
					if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
//2015.08.04 INSERT START
			}
//2015.08.04 INSERT END
			// データ読込みクラス

			//------->yoshida150811
			if( !m_pSnHonpyoData ){
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//					m_pSnHonpyoData = new CH27SnHonpyoData();
//					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//---------------
					m_pSnHonpyoData = new CH28SnHonpyoData();
					if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
//---------------
				}
				else{
					m_pSnHonpyoData = new CH26SnHonpyoData();
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			//---------------------
			/*
			if( !m_pSnHonpyoData ){
				m_pSnHonpyoData = new CH26SnHonpyoData();
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			*/
			//<---------------------

			// 経過措置対象の課税資産の譲渡等あり
			if( m_pSnHeadData->SVmzsw == 1 ){
				// データ読込みクラス
//2015.08.04 INSERT START
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( !m_pSnFhyo22Data ){
						m_pSnFhyo22Data = new CH28SnFhyo20Data(TRUE);
						if( ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetData(m_pZmSub, m_SnSeq) ){//yoshida150811
							return -1;
						}
					}
				}else{
//2015.08.04 INSERT END
					if( !m_pSnFhyo22Data ){
						m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE);
						if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
//2015.08.04 INSERT START
				}
//2015.08.04 INSERT END
				if( !m_pSnTanaData ){
					m_pSnTanaData = new CH26SnTanaData(TRUE);
					if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			// 経過措置対象の課税資産の譲渡等なし
			else{
				// データ読込みクラス
//2015.08.04 INSERT START
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( !m_pSnFhyo20Data ){
						m_pSnFhyo20Data = new CH28SnFhyo20Data(FALSE);
						if( ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetData(m_pZmSub, m_SnSeq) ){//yoshida150811
							return -1;
						}
					}
				}else{
//2015.08.04 INSERT END
					if( !m_pSnFhyo20Data ){
						m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE);
						if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
//2015.08.04 INSERT START
				}
//2015.08.04 INSERT END
				if( !m_pSnTanaData ){
					m_pSnTanaData = new CH26SnTanaData(FALSE);
					if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
		}

		// ('14.05.16)-------------------------------------------------------
		if( !m_pSnRefundData ){
			m_pSnRefundData = new CH26SnRefundData();
			if( m_pSnRefundData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
				return -1;
			}
			// 情報設定
			ResetRefundData();
		}
		//-------------------------------------------------------------------
//2018.03.13 INSERT START
		if( m_pSnHeadData->Sn_Syuusei&0x10 ){
			if( !m_pSnFhyo60Data ){
				m_pSnFhyo60Data = new CH28SnFhyo60Data();
				if( m_pSnFhyo60Data->GetData( m_pZmSub, m_SnSeq ) ){
					return -1;
				}
			}
		}
//2018.03.13 INSERT END
	}

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
int CH26HyoView::InitTabSub( BOOL bDelete )
{
	if( bDelete ){
		// 内部クラスを破棄？
		DeleteInsideDlg();
		// 全削除
		m_MainTab.DeleteAllItems();
	}

	ICSDialog	*ptmpTab[16] = {0};
	BOOL		isPluralZei	= FALSE;
	SH_TABMNG	shTabmng;
	int			firstPage = 0;

	int	tabIdx = 0;
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第26号様式）
		// 本表データ読込みクラス
		if( !m_pSnHonpyoData ){
//-- '15.02.27 --
//			m_pSnHonpyoData = new CH26SnHonpyoData();
//			if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//---------------

			//------------->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//				m_pSnHonpyoData = new CH27SnHonpyoData();
//				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//---------------
				m_pSnHonpyoData = new CH28SnHonpyoData();
				if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
//---------------
			}
			else{
				if( m_SixKindFlg == TRUE ){
					m_pSnHonpyoData = new CH27SnHonpyoData();
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnHonpyoData = new CH26SnHonpyoData();
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			//----------------------------
			/*
			if( m_SixKindFlg == TRUE ){
				m_pSnHonpyoData = new CH27SnHonpyoData();
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				m_pSnHonpyoData = new CH26SnHonpyoData();
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			*/
			//<---------------------------
//---------------
			// 集計済みデータのセット
/*- '14.06.30 -*/
//			if( m_pSnHeadData->IsReadCalqedData() ){
//				m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
//			}
/*-------------*/
		}
		// 中間予定申告データ
		if( !m_pSnChskData ){
			m_pSnChskData = new CH26SnChskData();
			if( m_pSnChskData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}

		//------------->特定課税仕入対応yoshida150805
//		if( m_TokuteiFlg == TRUE){
//-- '15.11.26 --
//		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE){
//---------------
		if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
			//---------------
			if( !m_pChskEX ){
				m_pChskEX = new CShinChskEX(this);
				m_pChskEX->InitInfo( m_pSnHeadData, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo );
				m_pChskEX->Create( IDD_DIALOG_CHSK_EX, &m_MainTab );
			}
			m_MainTab.InsertItem( tabIdx, _T("中間申告(第26号)"), m_pChskEX->m_hWnd );
			ptmpTab[tabIdx] = m_pChskEX;
			tabIdx++;
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_CHSK_TAB;
			shTabmng.pWnd = m_pChskEX;
			m_TabMng.Add( shTabmng );
		}
		else{
		//<-------------------------------------------
			if( !m_pChsk ){
				m_pChsk = new CShinChsk(this);
				m_pChsk->InitInfo( m_pSnHeadData, m_pSnChskData, m_pSnHonpyoData, &m_Arith, m_pShinInfo );
				m_pChsk->Create( IDD_DIALOG_CHSK, &m_MainTab );
			}
			m_MainTab.InsertItem( tabIdx, _T("中間申告(第26号)"), m_pChsk->m_hWnd );
			ptmpTab[tabIdx] = m_pChsk;
			tabIdx++;
			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_CHSK_TAB;
			shTabmng.pWnd = m_pChsk;
			m_TabMng.Add( shTabmng );
		}
	}
	else{
		// 課税処理
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
//		m_pSnHeadData->SVmzsw &= ~0x01;
//		m_pSnHeadData->SVmzsw |= 0x01;

		BOOL	isTransitionalMeasure = FALSE;
/*- '14.03.19 -*/
		if( m_pSnHeadData->SVmzsw == 1 ){
			isTransitionalMeasure = TRUE;
		}
/*-------------*/
//		isTransitionalMeasure = TRUE;
/*-------------*/

		if( KzHoushiki == ID_ICSSH_KANNI ){
			// 本表データ読込みクラス

			//------>[Y]
			if( m_pSnHonpyoData == NULL ){

				//------------->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//					m_pSnHonpyoData = new CH27SnHonpyoData();
//					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//---------------
					m_pSnHonpyoData = new CH28SnHonpyoData();
					if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
//---------------
				}
				else{
					if(m_SixKindFlg == TRUE){
						m_pSnHonpyoData = new CH27SnHonpyoData();
						if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnHonpyoData = new CH26SnHonpyoData();
						if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
				}
				//---------------------------
				/*
				//---->[Y]150106
				if(m_SixKindFlg == TRUE){
					m_pSnHonpyoData = new CH27SnHonpyoData();
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnHonpyoData = new CH26SnHonpyoData();
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				*/
				//<-----------------------------
			}


//			if( !m_pSnHonpyoData ){
//				m_pSnHonpyoData = new CH26SnHonpyoData();
//				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//				// 集計済みデータのセット
///*- '14.06.30 -*/
////				if( m_pSnHeadData->IsReadCalqedData() ){
////					m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
////				}
///*-------------*/
//			}

			//<---------------------

			// 付表４データ読込みクラス

			//------>[Y]
			if( !m_pSnFhyo40Data ){
				//---->[Y]150106
				if(m_SixKindFlg == TRUE){
					m_pSnFhyo40Data = new CH27SnFhyo10Data(isTransitionalMeasure);
					if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnFhyo40Data = new CH26SnFhyo10Data(isTransitionalMeasure);
					if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//<---------------
			}

//			if( !m_pSnFhyo40Data ){
//				m_pSnFhyo40Data = new CH26SnFhyo10Data(isTransitionalMeasure);
//				if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//				// 集計済みデータのセット
///*- '14.06.30 -*/
////				if( m_pSnHeadData->IsReadCalqedData() ){
////					m_pSnFhyo40Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
////				}
////				if( isPluralZei ){
//////					m_pSnHeadData->SVmzsw |= 0x01;
////				}
///*-------------*/
//			}

			//<---------------

			// 経過措置対象の課税資産の譲渡等あり
			if( m_pSnHeadData->SVmzsw == 1 ){

				//----->[Y]
				// データ読込みクラス
				if( !m_pSnFhyo52Data ){
					//---->[Y]150106
					if(m_SixKindFlg == TRUE){
						m_pSnFhyo52Data = new CH27SnFhyo50Data(TRUE);
						if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo52Data = new CH26SnFhyo50Data(TRUE);
						if( m_pSnFhyo52Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
				}

//				// データ読込みクラス
//				if( !m_pSnFhyo52Data ){
//					m_pSnFhyo52Data = new CH26SnFhyo50Data(TRUE);
//					if( m_pSnFhyo52Data->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//					// 集計済みデータのセット
///*- '14.06.30 -*/
////					if( m_pSnHeadData->IsReadCalqedData() ){
////						m_pSnFhyo52Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
////					}
///*-------------*/
//				}

				//<------------


				//[Y]141212
				//---->
				if(m_SixKindFlg == TRUE){
					if( !m_pFhyo40EX ){
						m_pFhyo40EX = new CShinFhyo40EX(this);
	//					m_pFhyo40EX->InitInfo( (CH27SnFhyo10Data**)&m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo , (CH27SnHonpyoData**)&m_pSnHonpyoData );//yooshida140311
						m_pFhyo40EX->InitInfo( &m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo , &m_pSnHonpyoData );//yooshida140311
	//					m_pFhyo40->InitInfo( m_pSnFhyo40Data, m_pSnHeadData, &m_Arith, m_pShinInfo );
						m_pFhyo40EX->Create( IDD_DIALOG_FHY40_EX, &m_MainTab );
					}

					m_MainTab.InsertItem( tabIdx, _T("付表４"), m_pFhyo40EX->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo40EX;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO40_TAB;
					shTabmng.pWnd = m_pFhyo40EX;
					m_TabMng.Add( shTabmng );

				}
				else{
					if( !m_pFhyo40 ){
						m_pFhyo40 = new CShinFhyo40(this);
						m_pFhyo40->InitInfo( &m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo , &m_pSnHonpyoData );
						m_pFhyo40->Create( IDD_DIALOG_FHY40, &m_MainTab );
					}
					m_MainTab.InsertItem( tabIdx, _T("付表４"), m_pFhyo40->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo40;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO40_TAB;
					shTabmng.pWnd = m_pFhyo40;
					m_TabMng.Add( shTabmng );
				}


//				if( !m_pFhyo40 ){
//					m_pFhyo40 = new CShinFhyo40(this);
//					m_pFhyo40->InitInfo( &m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo , &m_pSnHonpyoData );//yooshida140311
//					m_pFhyo40->InitInfo( m_pSnFhyo40Data, m_pSnHeadData, &m_Arith, m_pShinInfo );
//					m_pFhyo40->Create( IDD_DIALOG_FHY40, &m_MainTab );
//				}
				

				//m_MainTab.InsertItem( tabIdx, _T("付表４"), m_pFhyo40->m_hWnd );
				//ptmpTab[tabIdx] = m_pFhyo40;
				//tabIdx++;
				//memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				//shTabmng.tabtype = ID_ICSSH_FHYO40_TAB;
				//shTabmng.pWnd = m_pFhyo40;
				//m_TabMng.Add( shTabmng );

				//<----------


				//[Y]141212
				//---->
				if(m_SixKindFlg == TRUE){
					if( !m_pFhyo52EX ){
						m_pFhyo52EX = new CShinFhyo52EX(this);
	//					m_pFhyo52EX->InitInfo( &m_pSnFhyo52Data, &m_pSnFhyo40Data, &m_pSnHonpyoData, &m_pSnHeadData, &m_Arith, m_pShinInfo, m_pZmSub, m_pSubSyzSyukei );
						m_pFhyo52EX->InitInfo( (CH27SnFhyo50Data**)&m_pSnFhyo52Data, (CH27SnFhyo10Data**)&m_pSnFhyo40Data, (CH27SnHonpyoData**)&m_pSnHonpyoData, &m_pSnHeadData, &m_Arith, m_pShinInfo, m_pZmSub, m_pSubSyzSyukei );
						m_pFhyo52EX->Create( IDD_DIALOG_FHY52_EX, &m_MainTab );
						if( m_pFhyo40EX ){
							m_pFhyo52EX->ReWriteOnlySkjData();
							m_pFhyo40EX->disp_by_recalq();
						}
					}

					m_MainTab.InsertItem( tabIdx, _T("付表５－(２) 1/2"), m_pFhyo52EX->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo52EX;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO52_1_TAB;
					shTabmng.pWnd = m_pFhyo52EX;
					m_TabMng.Add( shTabmng );
				}
				else{
					//[Y]141212
					if( !m_pFhyo52 ){
						m_pFhyo52 = new CShinFhyo52(this);
						m_pFhyo52->InitInfo( &m_pSnFhyo52Data, &m_pSnFhyo40Data, &m_pSnHonpyoData, &m_pSnHeadData, &m_Arith, m_pShinInfo, m_pZmSub, m_pSubSyzSyukei );
						m_pFhyo52->Create( IDD_DIALOG_FHY52, &m_MainTab );
						if( m_pFhyo40 ){
							m_pFhyo52->ReWriteOnlySkjData();
							m_pFhyo40->disp_by_recalq();
						}
					}
					m_MainTab.InsertItem( tabIdx, _T("付表５－(２) 1/2"), m_pFhyo52->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo52;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO52_1_TAB;
					shTabmng.pWnd = m_pFhyo52;
					m_TabMng.Add( shTabmng );
				}

				//<-----------------------

				//[Y]141212
				//------------->
				if(m_SixKindFlg == TRUE){
					if( !m_pFhyo522EX ){
						m_pFhyo522EX = new CShinFhyo522EX(this);
						m_pFhyo522EX->InitInfo( (CH27SnFhyo50Data**)&m_pSnFhyo52Data, (CH27SnFhyo10Data**)&m_pSnFhyo40Data, (CH27SnHonpyoData**)&m_pSnHonpyoData, &m_pSnHeadData, &m_Arith, m_pShinInfo, m_pFhyo52EX, m_pZmSub );
						m_pFhyo522EX->Create( IDD_DIALOG_FHY522_EX, &m_MainTab );
					}
					m_MainTab.InsertItem( tabIdx, _T("付表５－(２) 2/2"), m_pFhyo522EX->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo522EX;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO52_2_TAB;
					shTabmng.pWnd = m_pFhyo522EX;
					m_TabMng.Add( shTabmng );
				}
				else{
					//[Y]141212
					if( !m_pFhyo522 ){
						m_pFhyo522 = new CShinFhyo522(this);
						m_pFhyo522->InitInfo( &m_pSnFhyo52Data, &m_pSnFhyo40Data, &m_pSnHonpyoData, &m_pSnHeadData, &m_Arith, m_pShinInfo, m_pFhyo52, m_pZmSub );
						m_pFhyo522->Create( IDD_DIALOG_FHY522, &m_MainTab );
					}
					m_MainTab.InsertItem( tabIdx, _T("付表５－(２) 2/2"), m_pFhyo522->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo522;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO52_2_TAB;
					shTabmng.pWnd = m_pFhyo522;
					m_TabMng.Add( shTabmng );
				}

				//<-------------

			}
			// 経過措置対象の課税資産の譲渡等なし
			else{


				//----->[Y]141212
				if( !m_pSnFhyo50Data ){
					//---->[Y]150106
					if(m_SixKindFlg == TRUE){
						m_pSnFhyo50Data = new CH27SnFhyo50Data(FALSE);
						 
						if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->GetData(m_pZmSub, m_SnSeq) ){//[Y]150115
//						if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo50Data = new CH26SnFhyo50Data(FALSE);
						if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//<-----------------

					// 集計済みデータのセット
				}

				// データ読込みクラス
//				if( !m_pSnFhyo50Data ){
//					m_pSnFhyo50Data = new CH26SnFhyo50Data(FALSE);
//					if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//					// 集計済みデータのセット
///*- '14.06.30 -*/
////					if( m_pSnHeadData->IsReadCalqedData() ){
////						m_pSnFhyo50Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
////					}
///*-------------*/
//				}

				//<---------------------------


				if(m_SixKindFlg == TRUE){
					if( !m_pFhyo50EX ){
						m_pFhyo50EX = new CShinFhyo50EX(this);
						m_pFhyo50EX->InitInfo( (CH27SnFhyo50Data**)&m_pSnFhyo50Data, (CH27SnFhyo10Data**)&m_pSnFhyo40Data, &m_pSnHeadData, (CH27SnHonpyoData**)&m_pSnHonpyoData, &m_Arith, m_pZmSub, m_pShinInfo );
						m_pFhyo50EX->Create( IDD_DIALOG_FHY50_EX, &m_MainTab );
					}
		
					if( !m_pFhyo40EX ){
						m_pFhyo40EX = new CShinFhyo40EX(this);
						m_pFhyo40EX->InitInfo( &m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo , &m_pSnHonpyoData );
						m_pFhyo40EX->Create( IDD_DIALOG_FHY40_EX, &m_MainTab );
					}

					m_MainTab.InsertItem( tabIdx, _T("付表５"), m_pFhyo50EX->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo50EX;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO50_TAB;
					shTabmng.pWnd = m_pFhyo50EX;
					m_TabMng.Add( shTabmng );

				}
				else{
					if( !m_pFhyo50 ){
						m_pFhyo50 = new CShinFhyo50(this);
						m_pFhyo50->InitInfo( &m_pSnFhyo50Data, &m_pSnFhyo40Data, &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pZmSub, m_pShinInfo );
						m_pFhyo50->Create( IDD_DIALOG_FHY50, &m_MainTab );
					}

					if( !m_pFhyo40 ){
						m_pFhyo40 = new CShinFhyo40(this);
						m_pFhyo40->InitInfo( &m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo , &m_pSnHonpyoData );
						m_pFhyo40->Create( IDD_DIALOG_FHY40, &m_MainTab );
					}
						
					m_MainTab.InsertItem( tabIdx, _T("付表５"), m_pFhyo50->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo50;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO50_TAB;
					shTabmng.pWnd = m_pFhyo50;
					m_TabMng.Add( shTabmng );

				}
			}

			// 帳票ダイアログ

			
			//------------->特定課税仕入対応yoshida150805
//			if( m_TokuteiFlg == TRUE){
			if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
				if( !m_pKaniEX2 ){
					m_pKaniEX2 = new CShinKaniEX2(this);
					m_pKaniEX2->InitInfo( (CH28SnHonpyoData**)&m_pSnHonpyoData, (CH27SnFhyo10Data**)&m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
					m_pKaniEX2->Create( IDD_DIALOG_KANI_EX2, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T("簡易本表"), m_pKaniEX2->m_hWnd );
				ptmpTab[tabIdx] = m_pKaniEX2;
				tabIdx++;
				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_KANI_TAB;
				shTabmng.pWnd = m_pKaniEX2;
				m_TabMng.Add( shTabmng );

			}
			else{
			//<-----------------------------------------\
				//----->[Y]141212
				if(m_SixKindFlg == TRUE){
					if( !m_pKaniEX ){
						m_pKaniEX = new CShinKaniEX(this);
						m_pKaniEX->InitInfo( (CH27SnHonpyoData**)&m_pSnHonpyoData, (CH27SnFhyo10Data**)&m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
						m_pKaniEX->Create( IDD_DIALOG_KANI_EX, &m_MainTab );
					}
					m_MainTab.InsertItem( tabIdx, _T("簡易本表"), m_pKaniEX->m_hWnd );
					ptmpTab[tabIdx] = m_pKaniEX;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_KANI_TAB;
					shTabmng.pWnd = m_pKaniEX;
					m_TabMng.Add( shTabmng );
				}
				else{
					if( !m_pKani ){
						m_pKani = new CShinKani(this);
						m_pKani->InitInfo( &m_pSnHonpyoData, &m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
						m_pKani->Create( IDD_DIALOG_KANI, &m_MainTab );
					}
					m_MainTab.InsertItem( tabIdx, _T("簡易本表"), m_pKani->m_hWnd );
					ptmpTab[tabIdx] = m_pKani;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_KANI_TAB;
					shTabmng.pWnd = m_pKani;
					m_TabMng.Add( shTabmng );
				}
			}

			//[Y]141212
			//if( !m_pKani ){
			//	m_pKani = new CShinKani(this);
			//	m_pKani->InitInfo( &m_pSnHonpyoData, &m_pSnFhyo40Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
			//	m_pKani->Create( IDD_DIALOG_KANI, &m_MainTab );
			//}
			//m_MainTab.InsertItem( tabIdx, _T("簡易本表"), m_pKani->m_hWnd );
			//ptmpTab[tabIdx] = m_pKani;
			//tabIdx++;
			//memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			//shTabmng.tabtype = ID_ICSSH_KANI_TAB;
			//shTabmng.pWnd = m_pKani;
			//m_TabMng.Add( shTabmng );

			//<-----------------

		}
		else if( KzHoushiki == ID_ICSSH_GENSOKU ){
			// データ読込みクラス
			if( !m_pSnFhyo10Data ){

				//-------->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					m_pSnFhyo10Data = new CH28SnFhyo10Data(isTransitionalMeasure);
					if( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}

				}
				else{
					m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
					if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//--------------------------
				/*
				m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
				if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<---------------------------

				// 集計済みデータのセット
/*- '14.06.30 -*/
//				if( m_pSnHeadData->IsReadCalqedData() ){
//					m_pSnFhyo10Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
//				}
//				if( isPluralZei ){
////					m_pSnHeadData->SVmzsw |= 0x01;
//				}
/*-------------*/
			}

			// 原則課税
			// データ読込みクラス
			if( !m_pSnHonpyoData ){
				//-------->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//					m_pSnHonpyoData = new CH27SnHonpyoData();
//					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//---------------
					m_pSnHonpyoData = new CH28SnHonpyoData();
					if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
//---------------
				}
				else{
					m_pSnHonpyoData = new CH26SnHonpyoData();
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//----------------------
				/*
				m_pSnHonpyoData = new CH26SnHonpyoData();
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<----------------------

				// 集計済みデータのセット
/*- '14.06.30 -*/
//				if( m_pSnHeadData->IsReadCalqedData() ){
//					m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
//				}
/*-------------*/
			}

			// 経過措置対象の課税資産の譲渡等あり
			if( m_pSnHeadData->SVmzsw == 1 ){
				// データ読込みクラス
				if( !m_pSnFhyo22Data ){
					//-------->yoshida150811
//					if( m_TokuteiFlg == TRUE ){
					if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
						m_pSnFhyo22Data = new CH28SnFhyo20Data(TRUE);
						if( ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE);
						if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//-------------------------
					/*
					m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE);
					if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
					*/
					//<---------------------------

					// 集計済みデータのセット
/*- '14.06.30 -*/
//					if( m_pSnHeadData->IsReadCalqedData() ){
//						m_pSnFhyo22Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
//					}
//					if( isPluralZei ){
////						m_pSnHeadData->SVmzsw |= 0x01;
//					}
/*-------------*/
				}
				if( !m_pSnTanaData ){
					m_pSnTanaData = new CH26SnTanaData(TRUE);
					if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				

				//------------->特定課税仕入対応yoshida150713
//				if( m_TokuteiFlg == TRUE){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( !m_pFhyo22EX ){
						m_pFhyo22EX = new CShinFhyo22EX(this);
						m_pFhyo22EX->InitInfo( (CH28SnFhyo20Data**)&m_pSnFhyo22Data, &m_pSnHeadData, &m_pSnTanaData, (CH28SnFhyo10Data**)&m_pSnFhyo10Data, (CH28SnHonpyoData**)&m_pSnHonpyoData, &m_Arith, m_pShinInfo );
						m_pFhyo22EX->Create( IDD_DIALOG_FHY22_EX, &m_MainTab );
					}
				}
				else{
					if( !m_pFhyo22 ){
						m_pFhyo22 = new CShinFhyo22(this);
						m_pFhyo22->InitInfo( &m_pSnFhyo22Data, &m_pSnHeadData, &m_pSnTanaData, &m_pSnFhyo10Data, &m_pSnHonpyoData, &m_Arith, m_pShinInfo );
						m_pFhyo22->Create( IDD_DIALOG_FHY22, &m_MainTab );
					}
				}
				//----------------------------------------
				
				

				//------------->特定課税仕入対応yoshida150713
//				if( m_TokuteiFlg == TRUE){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					m_MainTab.InsertItem( tabIdx, _T("付表２－(２)"), m_pFhyo22EX->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo22EX;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO22_TAB;
					shTabmng.pWnd = m_pFhyo22EX;
				}
				else{
					m_MainTab.InsertItem( tabIdx, _T("付表２－(２)"), m_pFhyo22->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo22;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO22_TAB;
					shTabmng.pWnd = m_pFhyo22;
				}
				//----------------------------------------
				//<-------------------------------------------

				m_TabMng.Add( shTabmng );

				
				//------------->特定課税仕入対応yoshida150713
//				if( m_TokuteiFlg == TRUE){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( !m_pFhyo10EX ){
						m_pFhyo10EX = new CShinFhyo10EX(this);
						m_pFhyo10EX->InitInfo( (CH28SnFhyo10Data**)&m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, (CH28SnHonpyoData**)&m_pSnHonpyoData );//[Y]
						m_pFhyo10EX->Create( IDD_DIALOG_FHY10_EX, &m_MainTab );
					}
				}
				else{
					if( !m_pFhyo10 ){
						m_pFhyo10 = new CShinFhyo10(this);
						m_pFhyo10->InitInfo( &m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnHonpyoData );//[Y]
						m_pFhyo10->Create( IDD_DIALOG_FHY10, &m_MainTab );
					}
				}
				//---------------------------------------
					

				//------------->特定課税仕入対応yoshida150713
//				if( m_TokuteiFlg == TRUE){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					m_MainTab.InsertItem( tabIdx, _T("付表１"), m_pFhyo10EX->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo10EX;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO10_TAB;
					shTabmng.pWnd = m_pFhyo10EX;
				}
				else{
					m_MainTab.InsertItem( tabIdx, _T("付表１"), m_pFhyo10->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo10;
					tabIdx++;
					memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
					shTabmng.tabtype = ID_ICSSH_FHYO10_TAB;
					shTabmng.pWnd = m_pFhyo10;
				}
				//---------------------------------------
				//<---------------------------------------

				m_TabMng.Add( shTabmng );
			}
			// 経過措置対象の課税資産の譲渡等なし
			else{
				

				//------------->特定課税仕入対応yoshida150713
//				if( m_TokuteiFlg == TRUE){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( !m_pFhyo10EX ){
						m_pFhyo10EX = new CShinFhyo10EX(this);
						m_pFhyo10EX->InitInfo( (CH28SnFhyo10Data**)&m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, (CH28SnHonpyoData**)&m_pSnHonpyoData );//[Y]
						m_pFhyo10EX->Create( IDD_DIALOG_FHY10_EX, &m_MainTab );
					}
				}
				else{
					if( !m_pFhyo10 ){
						m_pFhyo10 = new CShinFhyo10(this);
						m_pFhyo10->InitInfo( &m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnHonpyoData );//[Y]
						m_pFhyo10->Create( IDD_DIALOG_FHY10, &m_MainTab );
					}
				}
				//---------------------------------------
				//m_pFhyo10 = new CShinFhyo10(this);
				//m_pFhyo10->InitInfo( &m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnHonpyoData );//[Y]
				//m_pFhyo10->Create( IDD_DIALOG_FHY10, &m_MainTab );
				//<---------------------------------------

				

				// データ読込みクラス
				if( !m_pSnFhyo20Data ){
					//-------->yoshida150811
//					if( m_TokuteiFlg == TRUE ){
					if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
						m_pSnFhyo20Data = new CH28SnFhyo20Data(FALSE);
						if( ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE);
						if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//-----------------------
					/*
					m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE);
					if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
					*/
					//<------------------------
				}
				if( !m_pSnTanaData ){
					m_pSnTanaData = new CH26SnTanaData(FALSE);
					if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}

				

				//------------->特定課税仕入対応yoshida150713
//				if( m_TokuteiFlg == TRUE){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( !m_pFhyo20EX ){
						m_pFhyo20EX = new CShinFhyo20EX(this);
						m_pFhyo20EX->InitInfo( (CH28SnFhyo20Data**)&m_pSnFhyo20Data, &m_pSnHeadData, &m_pSnTanaData, (CH28SnFhyo10Data**)&m_pSnFhyo10Data, (CH28SnHonpyoData**)&m_pSnHonpyoData, &m_Arith, m_pShinInfo );
						m_pFhyo20EX->Create( IDD_DIALOG_FHY20_EX, &m_MainTab );
					}
				}
				else{
					if( !m_pFhyo20 ){
						m_pFhyo20 = new CShinFhyo20(this);
						m_pFhyo20->InitInfo( &m_pSnFhyo20Data, &m_pSnHeadData, &m_pSnTanaData, &m_pSnFhyo10Data, &m_pSnHonpyoData, &m_Arith, m_pShinInfo );
						m_pFhyo20->Create( IDD_DIALOG_FHY20, &m_MainTab );
					}
				}
				//---------------------------------------
				
				//------------->特定課税仕入対応yoshida150713
//				if( m_TokuteiFlg == TRUE){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					m_MainTab.InsertItem( tabIdx, _T("付表２"), m_pFhyo20EX->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo20EX;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO20_TAB;
					shTabmng.pWnd = m_pFhyo20EX;
				}
				else{
					m_MainTab.InsertItem( tabIdx, _T("付表２"), m_pFhyo20->m_hWnd );
					ptmpTab[tabIdx] = m_pFhyo20;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_FHYO20_TAB;
					shTabmng.pWnd = m_pFhyo20;
				}
				//----------------------------------------
				//<---------------------------------------

				m_TabMng.Add( shTabmng );
			}

//			if( m_TokuteiFlg == TRUE){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( !m_pIppanEX ){
					m_pIppanEX = new CShinIppanEX(this);
					m_pIppanEX->InitInfo(  (CH28SnHonpyoData**)&m_pSnHonpyoData, (CH28SnFhyo10Data**)&m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
					m_pIppanEX->Create( IDD_DIALOG_IPPAN_EX, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T("一般本表"), m_pIppanEX->m_hWnd );
				ptmpTab[tabIdx] = m_pIppanEX;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_IPPAN_TAB;
				shTabmng.pWnd = m_pIppanEX;

			}
			else{
				// 帳票ダイアログ
				if( !m_pIppan ){
					m_pIppan = new CShinIppan(this);
					m_pIppan->InitInfo( &m_pSnHonpyoData, &m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
					m_pIppan->Create( IDD_DIALOG_IPPAN, &m_MainTab );
				}
				//---------------------------------------

				/*
				m_pIppan = new CShinIppan(this);
				m_pIppan->InitInfo( &m_pSnHonpyoData, &m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
				m_pIppan->Create( IDD_DIALOG_IPPAN, &m_MainTab );
				*/
				//<---------------------------------------

				m_MainTab.InsertItem( tabIdx, _T("一般本表"), m_pIppan->m_hWnd );
				ptmpTab[tabIdx] = m_pIppan;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_IPPAN_TAB;
				shTabmng.pWnd = m_pIppan;

			}

			/*
			m_MainTab.InsertItem( tabIdx, _T("一般本表"), m_pIppan->m_hWnd );
			ptmpTab[tabIdx] = m_pIppan;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_IPPAN_TAB;
			shTabmng.pWnd = m_pIppan;
			*/

			m_TabMng.Add( shTabmng );

			//------------>特定課税仕入対応yoshida150713
			//新表表示対応	
			if( m_pSnHeadData->m_DispTabSgn & 0x01){
				if( !m_pTKUtiwake ){
					m_pTKUtiwake = new CTKUtiwake(this);
					m_pTKUtiwake->InitInfo( (CH28SnHonpyoData**)&m_pSnHonpyoData, (CH28SnFhyo10Data**)&m_pSnFhyo10Data, &m_pSnHeadData, &m_Arith, m_pShinInfo );
					m_pTKUtiwake->Create( IDD_DIALOG_TKUTIWAKE, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T("特定課税仕入"), m_pTKUtiwake->m_hWnd );
				ptmpTab[tabIdx] = m_pTKUtiwake;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_TKTEI_TAB;
				shTabmng.pWnd = m_pTKUtiwake;

				m_TabMng.Add( shTabmng );
			}
			//<--------------------------------------
		}

//2018.03.13 INSERT START
		//準確定or準確定の修正・更正
		if( m_pSnHeadData->Sn_Syuusei&0x10 ){

			if( !m_pSnFhyo60Data ){
				m_pSnFhyo60Data = new CH28SnFhyo60Data();
				if( m_pSnFhyo60Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}

			if( !m_pFhyo60 ){
				m_pFhyo60 = new CShinFhyo60(this);
				m_pFhyo60->InitInfo( &m_pSnFhyo60Data, &m_pSnFhyo10Data, &m_pSnFhyo40Data, &m_pSnHeadData, &m_pSnHonpyoData, &m_Arith, m_pZmSub, m_pShinInfo );
				m_pFhyo60->Create( IDD_DIALOG_FHY60, &m_MainTab );
			}
			m_MainTab.InsertItem( tabIdx, _T("付表６"), m_pFhyo60->m_hWnd );
			ptmpTab[tabIdx] = m_pFhyo60;
			tabIdx++;
			shTabmng.tabtype = ID_ICSSH_FHYO60_TAB;
			shTabmng.pWnd = m_pFhyo60;
			m_TabMng.Add( shTabmng );			
		}
//2018.03.13 INSERT END


		// 還付
		if( m_pSnHeadData->IsUseTaxRefund() ){

			if( !m_pSnRefundData ){
				m_pSnRefundData = new CH26SnRefundData();
				if( m_pSnRefundData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
					m_pSnRefundData->DeleteAllData( m_pZmSub, -1 );
					return -1;
				}
			}

			// 情報設定
			ResetRefundData();

			// 個人
			if( m_pSnHeadData->IsSoleProprietor() ){
				if( !m_pRefundKjn01 ){
					m_pRefundKjn01 = new CShinRefundKjn01(this);
					m_pRefundKjn01->Create( IDD_SYZREFUND_KJ1, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T("還付付表1(個人)"), m_pRefundKjn01->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundKjn01;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_KJ_01_TAB;
				shTabmng.pWnd = m_pRefundKjn01;
				m_TabMng.Add( shTabmng );

				if( !m_pRefundKjn02 ){
					m_pRefundKjn02 = new CShinRefundKjn02(this);
					m_pRefundKjn02->Create( IDD_SYZREFUND_KJ2, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T("還付付表2(個人)"), m_pRefundKjn02->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundKjn02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_KJ_02_TAB;
				shTabmng.pWnd = m_pRefundKjn02;
				m_TabMng.Add( shTabmng );
			}
			// 法人
			else{
				if( !m_pRefundHjn01 ){
					m_pRefundHjn01 = new CShinRefundHjn01(this);
					m_pRefundHjn01->Create( IDD_SYZREFUND_HJ1, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T("還付付表1(法人)"), m_pRefundHjn01->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundHjn01;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_HJ_01_TAB;
				shTabmng.pWnd = m_pRefundHjn01;
				m_TabMng.Add( shTabmng );

				if( !m_pRefundHjn02 ){
					m_pRefundHjn02 = new CShinRefundHjn02(this);
					m_pRefundHjn02->Create( IDD_SYZREFUND_HJ2, &m_MainTab );
				}
				m_MainTab.InsertItem( tabIdx, _T("還付付表2(法人)"), m_pRefundHjn02->m_hWnd );
				ptmpTab[tabIdx] = m_pRefundHjn02;
				tabIdx++;
				shTabmng.tabtype = ID_ICSSH_REFUND_HJ_02_TAB;
				shTabmng.pWnd = m_pRefundHjn02;
				m_TabMng.Add( shTabmng );
			}
		}
		else{
			// 個人
			if( m_pSnHeadData->IsSoleProprietor() ){
			}
			// 法人
			else{
			}
		}

		// 更正の請求
		if( m_pSnHeadData->Sn_Syuusei&0x02 ){
			if( !m_pSnKskData ){
				m_pSnKskData = new CH26SnKskData();
				if( m_pSnKskData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
					return -1;
				}
			}
			CString	ttlStr;
			// 個人
			if( m_pSnHeadData->IsSoleProprietor() ){

				//------------>特定課税仕入対応yoshida150805
//				if( m_TokuteiFlg == TRUE){
//2016.02.22 DELETE START
//				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//2016.02.22 DELETE END
					if( !m_pKjnKskEX ){
						m_pKjnKskEX = new CShinKjnKskEX(this);
						m_pKjnKskEX->InitInfo( &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData );
						m_pKjnKskEX->Create( IDD_DIALOG_KJINKSK_EX, &m_MainTab );
//2016.02.22 INSERT START
						m_pKjnKskEX->Init(); //ライセンスが無い場合、マイナンバー入力エリアの表示がおかしくなる。何故かここで再度処理すると直るため、苦肉の策。
//2016.02.22 INSERT END
					}
					if( m_pSnHeadData->Sn_Syuusei&0x04 ){
						ttlStr = _T("更正の申出(個人)");
					}
					else{
						ttlStr = _T("更正の請求(個人)");
					}
					m_MainTab.InsertItem( tabIdx, ttlStr, m_pKjnKskEX->m_hWnd );
					ptmpTab[tabIdx] = m_pKjnKskEX;
					firstPage = tabIdx;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_KSK_KJ_TAB;
					shTabmng.pWnd = m_pKjnKskEX;
					m_TabMng.Add( shTabmng );
//2016.02.22 DELETE START
				//}
				//else{
				////<-----------------------------------------
				//	if( !m_pKjnKsk ){
				//		m_pKjnKsk = new CShinKjnKsk(this);
				//		m_pKjnKsk->InitInfo( &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData );
				//		m_pKjnKsk->Create( IDD_DIALOG_KJINKSK, &m_MainTab );
				//	}
				//	if( m_pSnHeadData->Sn_Syuusei&0x04 ){
				//		ttlStr = _T("更正の申出(個人)");
				//	}
				//	else{
				//		ttlStr = _T("更正の請求(個人)");
				//	}
				//	m_MainTab.InsertItem( tabIdx, ttlStr, m_pKjnKsk->m_hWnd );
				//	ptmpTab[tabIdx] = m_pKjnKsk;
				//	firstPage = tabIdx;
				//	tabIdx++;
				//	shTabmng.tabtype = ID_ICSSH_KSK_KJ_TAB;
				//	shTabmng.pWnd = m_pKjnKsk;
				//	m_TabMng.Add( shTabmng );
				//}
//2016.02.22 DELETE END
			}
			// 法人
			else{
				//------------>特定課税仕入対応yoshida150805
//				if( m_TokuteiFlg == TRUE){
//2016.02.22 DELETE START
//				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//2016.02.22 DELETE END
					if( !m_pHjnKskEX ){
						m_pHjnKskEX = new CShinHjnKskEX(this);
						m_pHjnKskEX->InitInfo( &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData );
						m_pHjnKskEX->Create( IDD_DIALOG_HJINKSK_EX, &m_MainTab );
					}
					if( m_pSnHeadData->Sn_Syuusei&0x04 ){
						ttlStr = _T("更正の申出(法人)");
					}
					else{
						ttlStr = _T("更正の請求(法人)");
					}
					m_MainTab.InsertItem( tabIdx, ttlStr, m_pHjnKskEX->m_hWnd );
					ptmpTab[tabIdx] = m_pHjnKskEX;
					firstPage = tabIdx;
					tabIdx++;
					shTabmng.tabtype = ID_ICSSH_KSK_HJ_TAB;
					shTabmng.pWnd = m_pHjnKskEX;
					m_TabMng.Add( shTabmng );
//2016.02.22 DELETE START
				//}
				//else{
				////<-----------------------------------------
				//	if( !m_pHjnKsk ){
				//		m_pHjnKsk = new CShinHjnKsk(this);
				//		m_pHjnKsk->InitInfo( &m_pSnHeadData, &m_Arith, m_pShinInfo, &m_pSnKskData );
				//		m_pHjnKsk->Create( IDD_DIALOG_HJINKSK, &m_MainTab );
				//	}
				//	if( m_pSnHeadData->Sn_Syuusei&0x04 ){
				//		ttlStr = _T("更正の申出(法人)");
				//	}
				//	else{
				//		ttlStr = _T("更正の請求(法人)");
				//	}
				//	m_MainTab.InsertItem( tabIdx, ttlStr, m_pHjnKsk->m_hWnd );
				//	ptmpTab[tabIdx] = m_pHjnKsk;
				//	firstPage = tabIdx;
				//	tabIdx++;
				//	shTabmng.tabtype = ID_ICSSH_KSK_HJ_TAB;
				//	shTabmng.pWnd = m_pHjnKsk;
				//	m_TabMng.Add( shTabmng );
				//}
//2016.02.22 DELETE END
			}
		}
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
	m_MainTab.SetCurSel( firstPage );
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );
//	SetIemPositionEx( m_TabNoControl );
//	m_TabSign = 1;
//	if( pSyzShin->IntViewSw ){
//		m_TabSheet.PostMessageA( WM_KEYDOWN, VK_RETURN );
//	}

//	FitView( m_MainTab );
	
	//画面サイズ変更
	//------>kanemura140312
//	CRect	tabRect;
//	m_MainTab.GetClientRect(tabRect);
//	tabRect.bottom += vwRect.bottom - tabRect.bottom;
//	m_MainTab.MoveWindow( tabRect );
//	SetScrollPos(SB_HORZ,0,1);
	//<--------------------

	//------>kanemura140312
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
//	SetScrollPos(SB_HORZ,0,1);
	//<---------------------

	//-->変更 harino ('14.03.06)
	//SetItemPosition( m_MainTab.GetCurSel() );
	//PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセットのタイミングをPostMessageに変更してずらす	// 削除 harino ('14.03.08)
	//<--変更 harino ('14.03.06)

	return 0;
}

void CH26HyoView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();

	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
}

BOOL CH26HyoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	return ICSFormView::PreCreateWindow(cs);
}

void CH26HyoView::PreSubclassWindow()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	ICSFormView::PreSubclassWindow();
}

//-----------------------------------------------------------------------------
// 各表の更新処理
//-----------------------------------------------------------------------------
void CH26HyoView::UpdateSheetFromOuter()
{
	UpdateSheet( m_MainTab.GetCurSel() );

	// 還付データ書込み
	if( m_pSnRefundData ){
		m_pSnRefundData->SetData( m_pZmSub, m_pSnHeadData, m_SnSeq );
	}
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
int CH26HyoView::WriteData( BOOL isEnd, BOOL isGetData )
{
	// 一般本表
	if( m_pSnHonpyoData ){
/*- '14.03.10 -*/
//		if( isEnd == FALSE ){
/*-------------*/
		if( isGetData ){
/*-------------*/
			if( m_bShowed ){
				if( m_pIppan ){
					m_pIppan->WriteData();
				}
				//---->yoshida150811
				if( m_pIppanEX ){
					m_pIppanEX->WriteData();
				}
				//<-----------------
				if( m_pKani ){
					m_pKani->WriteData();
				}
//--> '15.01.05 INS START
				if( m_pKaniEX ){
					m_pKaniEX->GetDiagData();
					m_pKaniEX->WriteData();
				}
//<-- '15.01.05 INS END
				//---->yoshida150811
				if( m_pKaniEX2 ){
					m_pKaniEX2->GetDiagData();
					m_pKaniEX2->WriteData();
				}
				//<-----------------
//--> '15.08.21 INS START
				if( m_pTKUtiwake ){
//					m_pTKUtiwake->GetDiagData();
					m_pTKUtiwake->WriteData();
				}
//<-- '15.08.21 INS END
			}
		}

		//---->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//			if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//---------------
			if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
//---------------
		}
		else{
			if( m_SixKindFlg ){
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnHonpyoData->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
		}
		//------------------
		/*
		if( m_SixKindFlg ){
			if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( m_pSnHonpyoData->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		*/
		//<------------------

		if( isEnd ){
			delete m_pSnHonpyoData;
			m_pSnHonpyoData = NULL;
		}
	}
	// 付表１データ
	if( m_pSnFhyo10Data ){
/*- '14.03.10 -*/
//		if( isEnd == FALSE ){
/*-------------*/
		if( isGetData ){
/*-------------*/
			if( m_bShowed ){
				if( m_pFhyo10 ){
					m_pFhyo10->WriteData();
				}
				//---->yoshida150811
				if( m_pFhyo10EX ){
					m_pFhyo10EX->WriteData();
				}
				//<-----------------
			}
		}

		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( m_pSnFhyo10Data->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//-------------------
		/*
		if( m_pSnFhyo10Data->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
		*/
		//<-------------------

		if( isEnd ){
			delete m_pSnFhyo10Data;
			m_pSnFhyo10Data = NULL;
		}
	}
	
	if( m_pSnHeadData->SVmzsw == 0 ){
		// 付表２－２データ
		if( m_pSnFhyo22Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo22 ){
						m_pFhyo22->WriteData( 1 );
					}
					//---->yoshida150811
					if( m_pFhyo22EX ){
						m_pFhyo22EX->WriteData( 1 );
					}
					//<-----------------
				}
			}

			//----->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo22Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//--------------------
			/*
			if( m_pSnFhyo22Data->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
			*/
			//<--------------------
			if( isEnd ){
				delete m_pSnFhyo22Data;
				m_pSnFhyo22Data = NULL;
			}
		}
		// 付表２データ
		if( m_pSnFhyo20Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo20 ){
						m_pFhyo20->WriteData( 1 );
					}
					//---->yoshida150811
					if( m_pFhyo20EX ){
						m_pFhyo20EX->WriteData( 1 );
					}
					//<-----------------
				}
			}
			//----->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo20Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//-------------------
			/*
			if( m_pSnFhyo20Data->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
			*/
			//<------------------
			if( isEnd ){
				delete m_pSnFhyo20Data;
				m_pSnFhyo20Data = NULL;
			}
		}
	}
	else{
		// 付表２データ
		if( m_pSnFhyo20Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo20 ){
						m_pFhyo20->WriteData( 1 );
					}
					//---->yoshida150811
					if( m_pFhyo20EX ){
						m_pFhyo20EX->WriteData( 1 );
					}
					//<-----------------
				}
			}
			//----->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo20Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//-------------------
			/*
			if( m_pSnFhyo20Data->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
			*/
			//<-------------------
			if( isEnd ){
				delete m_pSnFhyo20Data;
				m_pSnFhyo20Data = NULL;
			}
		}
		// 付表２－２データ
		if( m_pSnFhyo22Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo22 ){
						m_pFhyo22->WriteData( 1 );
					}
					//---->yoshida150811
					if( m_pFhyo22EX ){
						m_pFhyo22EX->WriteData( 1 );
					}
					//<-----------------
				}
			}
			//----->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo22Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//--------------------
			/*
			if( m_pSnFhyo22Data->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
			*/
			//<--------------------
			if( isEnd ){
				delete m_pSnFhyo22Data;
				m_pSnFhyo22Data = NULL;
			}
		}
	}

	// 付表４データ
	if( m_pSnFhyo40Data ){
/*- '14.03.10 -*/
//		if( isEnd == FALSE ){
/*-------------*/
		if( isGetData ){
/*-------------*/
			if( m_bShowed ){
				if( m_pFhyo40 ){
					m_pFhyo40->WriteData();
				}
//--> '15.02.23 INS START
				if( m_pFhyo40EX ){
					m_pFhyo40EX->WriteData();
				}
//<-- '15.02.23 INS END
			}
		}
		//--->[Y]150106
		if(m_SixKindFlg == TRUE){
			if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( m_pSnFhyo40Data->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//<-------------------
		if( isEnd ){
			delete m_pSnFhyo40Data;
			m_pSnFhyo40Data = NULL;
		}
	}

	if( m_pSnHeadData->SVmzsw == 0 ){
		// 付表５－２データ
		if( m_pSnFhyo52Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo52 ){
						m_pFhyo52->WriteData();
					}
					if( m_pFhyo522 ){
						m_pFhyo522->WriteData();
					}
//--> '15.01.05 INS START
					if( m_pFhyo52EX ){
						m_pFhyo52EX->WriteData();
					}
					if( m_pFhyo522EX ){
						m_pFhyo522EX->WriteData();
					}
//<-- '15.01.05 INS END
				}
			}
//- '15.01.05 -
//			if( m_pSnFhyo52Data->SetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//-------------
			if( m_SixKindFlg ){
				if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo52Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
//-------------
			if( isEnd ){
				delete m_pSnFhyo52Data;
				m_pSnFhyo52Data = NULL;
			}
		}
		// 付表５データ
		if( m_pSnFhyo50Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo50 ){
						m_pFhyo50->WriteData();
					}
//--> '15.01.05 INS START
					if( m_pFhyo50EX ){
						m_pFhyo50EX->WriteData();
					}
//<-- '15.01.05 INS END
				}
			}
//- '15.01.05 -
//			if( m_pSnFhyo50Data->SetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//-------------
			if( m_SixKindFlg ){
				if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo50Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
//-------------
			if( isEnd ){
				delete m_pSnFhyo50Data;
				m_pSnFhyo50Data = NULL;
			}
		}
	}
	else{
		// 付表５データ
		if( m_pSnFhyo50Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo50 ){
						m_pFhyo50->WriteData();
					}
//--> '15.01.05 INS START
					if( m_pFhyo50EX ){
						m_pFhyo50EX->WriteData();
					}
//<-- '15.01.05 INS END
				}
			}
//- '15.01.05 -
//			if( m_pSnFhyo50Data->SetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//-------------
			if( m_SixKindFlg ){
				if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo50Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
//-------------
			if( isEnd ){
				delete m_pSnFhyo50Data;
				m_pSnFhyo50Data = NULL;
			}
		}
		// 付表５－２データ
		if( m_pSnFhyo52Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo52 ){
						m_pFhyo52->WriteData();
					}
					if( m_pFhyo522 ){
						m_pFhyo522->WriteData();
					}
//--> '15.01.05 INS START
					if( m_pFhyo52EX ){
						m_pFhyo52EX->WriteData();
					}
					if( m_pFhyo522EX ){
						m_pFhyo522EX->WriteData();
					}
//<-- '15.01.05 INS END
				}
			}
//- '15.01.05 -
//			if( m_pSnFhyo52Data->SetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//-------------
			if( m_SixKindFlg ){
				if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnFhyo52Data->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
//-------------
			if( isEnd ){
				delete m_pSnFhyo52Data;
				m_pSnFhyo52Data = NULL;
			}
		}
	}
//2018.03.13 INSERT START
	if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
		if( m_pSnFhyo60Data ){
			if( isGetData ){
				if( m_bShowed ){
					if( m_pFhyo60 ){
						m_pFhyo60->WriteData();
					}
				}
			}
			
			if( m_pSnFhyo60Data->SetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}

			if( isEnd ){
				delete m_pSnFhyo60Data;
				m_pSnFhyo60Data = NULL;
			}
		}
	}
//2018.03.13 INSERT END

	// 棚卸調整データ
	if( m_pSnTanaData ){
		if( m_pSnTanaData->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pSnTanaData;
			m_pSnTanaData = NULL;
		}
	}

	// 還付申告データ
	if( m_pSnRefundData ){
		if( m_pSnRefundData->SetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pSnRefundData;
			m_pSnRefundData = NULL;
		}
	}
	// 更正の請求データ
	if( m_pSnKskData ){
		if( m_pSnKskData->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pSnKskData;
			m_pSnKskData = NULL;
		}
	}

	// 予定納税
	if( m_pSnChskData ){
		if( m_pSnChskData->SetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
		if( isEnd ){
			delete m_pSnChskData;
			m_pSnChskData = NULL;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 部分データ書込み
//-----------------------------------------------------------------------------
// 引数	flg	：	書込み対象
//-----------------------------------------------------------------------------
int CH26HyoView::WriteSepData( int flg )
{
	if( flg&0x01 ){	// 本表データ
		if( m_pSnHonpyoData ){

			//----->yoshida150810
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
//---------------
				if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
//---------------
					return -1;
				}
			}
			else{
				if(m_SixKindFlg == TRUE){
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( m_pSnHonpyoData->SetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			//-------------------
			/*
			//--->[Y]150106
			if(m_SixKindFlg == TRUE){
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( m_pSnHonpyoData->SetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//<-----------------
			*/
			//<--------------------
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 部分データ読込み
//-----------------------------------------------------------------------------
// 引数	flg	：	読込み対象
//-----------------------------------------------------------------------------
int CH26HyoView::ReadSepData( int flg )
{
	if( flg&0x01 ){	// 本表データ
		//----->yoshida150810
		if( m_pSnHonpyoData ){
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//---------------
				if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//---------------
					return -1;
				}
			}
			else{
				if(m_SixKindFlg == TRUE){
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}

			}
		}
		//-------------------
		/*
		if( m_pSnHonpyoData ){
			if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		*/
		//<-------------------
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
// 引数		wflg	：	（未使用）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::EndJob( int wflg )
{
	//== 最終データ書込み ==
	if( WriteData(TRUE, FALSE) ){
		return -1;
	}

	//== データ ==
	if( m_pSnHonpyoData ){
		delete m_pSnHonpyoData;
		m_pSnHonpyoData = NULL;
	}

	//== 画面 ==
	DeleteInsideDlg();

	return 0;
}


void CH26HyoView::OnButtonF2()
{
	//-->追加 harino ('14.03.03)
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		//if( (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_KJ_01_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_KJ_02_TAB) ||
		//	(m_TabMng[selno].tabtype==ID_ICSSH_REFUND_HJ_01_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_HJ_02_TAB) ){
			if( m_TabMng[selno].pWnd ){
				m_TabMng[selno].pWnd->PostMessage( WM_KEYDOWN, VK_F2 );
			}
		//}
	}
	//<--追加 harino ('14.03.03)
}


#ifdef	_CUT

//	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
//
//		
//	}
//	else{
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			if( m_MainTab.GetCurSel() == 0 ){//付表１
//				
//			}
//		}
//
//	}
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		if( (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_KJ_01_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_KJ_02_TAB) ||
			(m_TabMng[selno].tabtype==ID_ICSSH_REFUND_HJ_01_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_HJ_02_TAB) ){
			if( m_TabMng[selno].pWnd ){

				//[Y]
			//	SetItemPosition( m_MainTab.GetCurSel() );

				//[Y]
				// 還付申告ページのみうまくいかない。。。
			//	CWnd *pNowWnd = m_TabMng[selno].pWnd->GetFocus();
			//	if( pNowWnd ){
			//		pNowWnd->PostMessageA( WM_KEYDOWN, VK_F2 );
			//	}
				
			}
		}
		else if( (m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) ){
			;
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_CHSK_TAB ){
			;
		}
		else{
			if( m_TabMng[selno].pWnd ){

				//[Y]
				SetItemPosition( m_MainTab.GetCurSel() );

				//[Y]
				/*
				CWnd *pNowWnd = m_TabMng[selno].pWnd->GetFocus();
				if( pNowWnd ){
					pNowWnd->PostMessageA( WM_KEYDOWN, VK_F2 );
				}
				*/

			}
		}

	}

}
//<-----------------
#endif


void CH26HyoView::OnButtonF4()
{
	if( m_pShinInfo && m_pShinInfo->pBaseView ){

//2018.03.13 INSERT START
		if( Message_MyNumber_error() == -1 ){
			return;
		}
		if( Message_MyNumber_overlap() == -1 ){
			return;
		}
		EditOff_Fuhyo6();
		if( Message_SozokuWariai() == -1 ){
			return;
		}
//2018.03.13 INSERT END

		// データ更新
		UpdateSheet( m_MainTab.GetCurSel() );

//--> '15.09.09 INS START
		int st = IsAbleTkKz();
		if( st == -1 ){
			// 選択を変更する
			ChangeTkkzUse(1, 2);
			return;
		}
		else if( st == -2 ){
			if( ChkZkTkkzReturn(1,2) == 0 ){
				return;
			}
		}
//<-- '15.09.09 INS END

		m_pShinInfo->pBaseView->PostMessage( WM_COMMAND, ID_BUTTON_F4 );
	}
}

void CH26HyoView::OnButtonF5()
{

	if( m_pShinInfo && m_pShinInfo->pBaseView ){

//2018.03.13 INSERT START
		if( Message_MyNumber_error() == -1 ){
			return;
		}
		if( Message_MyNumber_overlap() == -1 ){
			return;
		}
		EditOff_Fuhyo6();
		if( Message_SozokuWariai() == -1 ){
			return;
		}
//2018.03.13 INSERT END

		// データ更新
		UpdateSheet( m_MainTab.GetCurSel() );

//--> '15.09.09 INS START
//--- '15.11.18 ---
//		int st = IsAbleTkKz();
//		if( st == -1 ){
//			// 選択を変更する
//			ChangeTkkzUse(1, 1);
//			return;
//		}
//		else if( st == -2 ){
//			if( ChkZkTkkzReturn(1,1) == 0 ){
//				return;
//			}
//		}
//-----------------
		if( (m_pVprn==NULL) || 
			((m_pVprn&&(m_pVprn->IsPrintFromPreview()==FALSE)) && (m_pVprn&&(m_pVprn->PreviewOnPrintdlg()==0))) ){
			int st = IsAbleTkKz();
			if( st == -1 ){
				// 選択を変更する
				ChangeTkkzUse(1, 1);
				return;
			}
			else if( st == -2 ){
				if( ChkZkTkkzReturn(1,1) == 0 ){
					return;
				}
			}
		}
//-----------------
//<-- '15.09.09 INS END

		m_pShinInfo->pBaseView->PostMessage( WM_COMMAND, ID_BUTTON_F5 );
	}
}

void CH26HyoView::OnButtonF7()
{
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		if( m_TabMng[selno].tabtype == ID_ICSSH_REFUND_KJ_01_TAB ){
			if( m_pRefundKjn01 ){
				m_pRefundKjn01->VolRecClear();
				if( m_pRefundKjn02 ){
					m_pRefundKjn02->SetDispPosition( 2 );
				}
				m_pRefundKjn01->SetDispPosition( 0 );
			}
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_REFUND_HJ_01_TAB ){
			if( m_pRefundHjn01 ){
				m_pRefundHjn01->VolRecClear();
				if( m_pRefundHjn02 ){
					m_pRefundHjn02->SetDispPosition( 3 );
				}
				m_pRefundHjn01->SetDispPosition( 1 );
			}
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB ){
			if( m_pHjnKsk ){
				// 中身未実装
				m_pHjnKsk->ClearKskHjn();
			}
			//-------->yoshida150810
			else if( m_pHjnKskEX ){
				// 中身未実装
				m_pHjnKskEX->ClearKskHjn();
			}
			//<---------------------

		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB ){
			if( m_pKjnKsk ){
				// 中身未実装
				m_pKjnKsk->ClearKskKjn();
			}
			//-------->yoshida150810
			else if( m_pKjnKskEX ){
				// 中身未実装
				m_pKjnKskEX->ClearKskKjn();
			}
			//<---------------------
		}
	}
}

void CH26HyoView::OnUpdateButtonF7(CCmdUI *pCmdUI)
{
	BOOL	bOn = FALSE;

	int selno = m_MainTab.GetCurSel();

	if( !(m_pSnHeadData && m_pSnHeadData->Sn_Sign4&0x01) ){
		if( (selno!=-1) && (selno<(int)m_TabMng.GetCount()) ){
			if( (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_KJ_01_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_REFUND_HJ_01_TAB) ||
				(m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) ){

				bOn = TRUE;
			}
		}
	}
	pCmdUI->Enable( bOn );
}

BOOL CH26HyoView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
//--> '15.11.17 INS START
			WPARAM	wParam = 0;
//<-- '15.11.17 INS END

//--- '16.01.07 ---
//			if( m_pShinInfo && m_pShinInfo->pParent ){
////--> '15.09.09 INS START
//				int st = IsAbleTkKz();
//				if( st == -1 ){
//					// 選択を変更する
//					ChangeTkkzUse(1,3);
//					return TRUE;
//				}
//				else if( st == -2 ){
//					if( ChkZkTkkzReturn(1,3) == 0 ){
//						return TRUE;
//					}
////--> '15.11.17 INS START
//					wParam = 1;	// 正常な特定課税仕入
////<-- '15.11.17 INS END
//				}
////<-- '15.09.09 INS END
//
////--> '15.07.31 INS START
//				m_pShinInfo->pParent->SendMessage( WM_USER_SET_ENDSTART, wParam );
////<-- '15.07.31 INS END
//				m_pShinInfo->pParent->PostMessage( WM_CLOSE, 0 );
//				return TRUE;
//			}
//-----------------
			if( m_pShinInfo && m_pShinInfo->pParent ){
				if( (m_pSnHeadData!=NULL) && m_pSnHeadData->IsMiddleProvisional() ){
					// 中間申告は正常な特定課税仕入として扱う
					wParam = 1;
				}
				else{
					int st = IsAbleTkKz();
					if( st == -1 ){
						// 選択を変更する
						ChangeTkkzUse(1,3);
						return TRUE;
					}
					else if( st == -2 ){
						if( ChkZkTkkzReturn(1,3) == 0 ){
							return TRUE;
						}
						wParam = 1;	// 正常な特定課税仕入
					}
				}

				m_pShinInfo->pParent->SendMessage( WM_USER_SET_ENDSTART, wParam );
				m_pShinInfo->pParent->PostMessage( WM_CLOSE, 0 );
				return TRUE;
			}
//-----------------
		}
		else if( pMsg->wParam == VK_RETURN ){
			if( m_TabMng.GetCount() ){
				SetItemPosition( m_MainTab.GetCurSel() );
				return TRUE;
			}
		}
	}
	else if( (pMsg->message==WM_MOUSEWHEEL) ){
		if( pMsg->hwnd == GetSafeHwnd() ){
			CWnd* pWnd = WindowFromPoint( pMsg->pt );
			if( pWnd && IsChild(pWnd) ){
				pMsg->hwnd = pWnd->GetSafeHwnd();
			}
		}
		//-->削除 harino ('14.03.07)
		//else{
		//	CWnd* pWnd = WindowFromPoint( pMsg->pt );
		//	if( pWnd && IsChild(pWnd) ){
		//		pWnd->SetFocus();
		//	}
		//}
		//<--削除 harino ('14.03.07)
	}

	return ICSFormView::PreTranslateMessage(pMsg);
}

void CH26HyoView::OnTcnSelchangingMainTab(NMHDR * pNotifyStruct, LRESULT * result)
{

	int selno = m_MainTab.GetCurSel();
//	m_TabNoControl = selno;

//2018.03.13 INSERT START
	int tabno;
	for( tabno = 0; tabno < m_TabMng.GetCount(); tabno++ ){
		if( m_TabMng[tabno].tabtype == ID_ICSSH_FHYO60_TAB ){
			break;
		}
	}
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
//2018.03.13 INSERT END

	UpdateSheet( selno );
}

//-----------------------------------------------------------------------------
// 各表の更新処理
//-----------------------------------------------------------------------------
// 引数	selno	：	タブインデックス
//-----------------------------------------------------------------------------
//2017.03.01 UPDATE START
//void CH26HyoView::UpdateSheet( int selno )
void CH26HyoView::UpdateSheet( int selno, int get_sgn )
//2017.03.01 UPDATE END
{
	if( selno == -1 ){
		return;
	}

	if( m_pSnHeadData->IsMiddleProvisional() == FALSE ){
		// 経理処理
		if( m_pSnHeadData->IsUseTaxRefund() ){		// 消費税の還付申告に関する明細書
			if( (int)m_TabMng.GetCount() <= selno ){
				return;
			}

			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
				if( m_pSnHeadData->SVmzsw == 1 ){

					// 経過措置対象の課税資産の譲渡等あり
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO40_TAB : // 付表４
							if( m_pFhyo40 ){
//-- '15.02.24 --
//								m_pFhyo40->GetAllDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo40->GetAllDiagData();
								}
//---------------
								m_pFhyo40->f4calq_all(1 , 1);//[Y]
								m_pFhyo40->WriteData();
							}

							//[Y]150119
							if( m_pFhyo40EX ){
//-- '15.02.24 --
//								m_pFhyo40EX->GetAllDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo40EX->GetAllDiagData();
								}
//---------------
								m_pFhyo40EX->f4calq_all(1 , 1);
								m_pFhyo40EX->WriteData();
							}

//-- '15.01.27 --
//							if( m_pKani ){
//								m_pKani->calq_by_OtherChange( 1 );
//							}
//							//[Y]150119
//							if( m_pKaniEX ){
//								m_pKaniEX->calq_by_OtherChange( 1 );
//							}
//
//							if( m_pFhyo52 ){
//								m_pFhyo52->calq_by_OtherChange(0);
//							}
//							//[Y]150119
//							if( m_pFhyo52EX ){
//								m_pFhyo52EX->calq_by_OtherChange( 1 );
//							}
//---------------
							if( m_pFhyo52 ){
								m_pFhyo52->calq_by_OtherChange(0);
							}
							//[Y]150119
							if( m_pFhyo52EX ){
//-- '15.02.23 --
//								m_pFhyo52EX->calq_by_OtherChange( 1 );
//---------------
								m_pFhyo52EX->calq_by_OtherChange(0);
//---------------
							}
							if( m_pFhyo40 ){
								m_pFhyo40->calq_by_OtherChange();
							}
//--> '15.02.23 INS START
							if( m_pFhyo40EX ){
								m_pFhyo40EX->calq_by_OtherChange();
							}
//<-- '15.02.23 INS END
							if( m_pKani ){
								m_pKani->calq_by_OtherChange( 1 );
							}
							//[Y]150119
							if( m_pKaniEX ){
								m_pKaniEX->calq_by_OtherChange( 1 );
							}
//---------------
							//----->yoshida150811
							if( m_pKaniEX2 ){
								m_pKaniEX2->calq_by_OtherChange( 1 );
							}
							//<------------------

							break;

						case ID_ICSSH_FHYO52_1_TAB : // 付表５－２
							if( m_pFhyo52 ){
								// 中身実装なし
//								((CShinFhy52*)ShinTab[1])->f52data_all();
//								m_pFhyo52->WriteData();
								m_pFhyo52->calq_by_OtherChange(0);
//								m_pFhyo52->WriteData();
							}

							//[Y]150119
							if( m_pFhyo52EX ){
								m_pFhyo52EX->calq_by_OtherChange(0);
							}

							if( m_pFhyo40 ){
								m_pFhyo40->calq_by_OtherChange();
							}

							//[Y]150119
							if( m_pFhyo40EX ){
								m_pFhyo40EX->calq_by_OtherChange();
							}

							if( m_pKani ){
								m_pKani->calq_by_OtherChange( 1 );
							}

							//[Y]150119
							if( m_pKaniEX ){
								m_pKaniEX->calq_by_OtherChange( 1 );
							}

							//----->yoshida150811
							if( m_pKaniEX2 ){
								m_pKaniEX2->calq_by_OtherChange( 1 );
							}
							//<------------------

							break;

						case ID_ICSSH_FHYO52_2_TAB: // 付表５－２（２）
							if( m_pFhyo52 ){
								m_pFhyo52->calq_by_OtherChange(0);
							}

							//[Y]150119
							if( m_pFhyo52EX ){
								m_pFhyo52EX->calq_by_OtherChange(0);
							}

//--> '15.01.27 INS START
							if( m_pFhyo40 ){
								m_pFhyo40->calq_by_OtherChange();
							}
							if( m_pFhyo40EX ){
								m_pFhyo40EX->calq_by_OtherChange();
							}
							if( m_pKani ){
								m_pKani->calq_by_OtherChange( 1 );
							}
							if( m_pKaniEX ){
								m_pKaniEX->calq_by_OtherChange( 1 );
							}
//<-- '15.01.27 INS END
							//----->yoshida150811
							if( m_pKaniEX2 ){
								m_pKaniEX2->calq_by_OtherChange( 1 );
							}
							//<------------------
							break;

//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->GetAllDiagData();
								}

								m_pFhyo60->WriteData();

								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->MoveFocus_FromMyNum();
								}
							}
							break;
//2018.03.13 INSERT END
						case ID_ICSSH_KANI_TAB : // 本表
							if( m_pFhyo52 ){
								m_pFhyo52->calq_by_OtherChange(0);
							}
							//[Y]150119
							if( m_pFhyo52EX ){
								m_pFhyo52EX->calq_by_OtherChange(0);
							}

							if( m_pKani ){
//-- '15.02.24 --
//								m_pKani->GetDiagData();
//---------------
								// 自身が表示されている時のみデータ取得
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pKani->GetDiagData();
								}
//---------------
								m_pKani->calqx_all( 6, 1 );
								m_pKani->WriteData();
								
							}

							//[Y]150119
							if( m_pKaniEX ){
//-- '15.02.24 --
//								m_pKaniEX->GetDiagData();
//---------------
								// 自身が表示されている時のみデータ取得
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pKaniEX->GetDiagData();
								}
//---------------
								m_pKaniEX->calqx_all( 6, 1 );
								m_pKaniEX->WriteData();
								
							}

							//------>yoshida150811
							if( m_pKaniEX2 ){
								// 自身が表示されている時のみデータ取得
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END

									m_pKaniEX2->GetDiagData();
								}
								m_pKaniEX2->calqx_all( 6, 1 );
								m_pKaniEX2->WriteData();
							}
							//<-------------------


							// 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetTitle();
									m_pRefundKjn01->SetDispPosition(0);
								}
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetTitle();
									m_pRefundHjn01->SetDispPosition(1);
								}
							}
							break;
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetDispPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetDispPosition(3);
								}
							}
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装無し
//									((CShinKjnKsk*)ShinTab[5])->GetAllData();
								}
								//---->yoshida150811
								if( m_pKjnKskEX ){
								}
								//<----------------

							}
							else{
								if( m_pHjnKsk ){
									// 中身実装無し
//									((CShinHjnKsk*)ShinTab[5])->GetAllData();
								}
								//---->yoshida150811
								if( m_pHjnKskEX ){
								}
								//<----------------
							}
							break;												
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO50_TAB : // 付表５
							if( m_pFhyo50 ){
//-- '15.02.24 --
//								m_pFhyo50->GetAllDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo50->GetAllDiagData();
								}
//---------------
								m_pFhyo50->f5calq_all( 1 );
								m_pFhyo50->WriteData();
							}

							//[Y]150119
							if( m_pFhyo50EX ){
//-- '15.02.24 --
//								m_pFhyo50EX->GetAllDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo50EX->GetAllDiagData();
								}
//---------------
								m_pFhyo50EX->f5calq_all( 1 );
								m_pFhyo50EX->WriteData();
							}

							// 付表４のクラス作成
							if(m_pFhyo40){
								m_pFhyo40->calq_by_OtherChange();
							}
							//[Y]150119
							if(m_pFhyo40EX){
								m_pFhyo40EX->calq_by_OtherChange();
							}

							if( m_pKani ){
								m_pKani->calq_by_OtherChange( 1 );
							}

							//[Y]150119
							if( m_pKaniEX ){
								m_pKaniEX->calq_by_OtherChange( 1 );
							}

							//---->yoshida150811
							if( m_pKaniEX2 ){
								m_pKaniEX2->calq_by_OtherChange( 1 );
							}
							//<----------------


							break;
//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->GetAllDiagData();
								}

								m_pFhyo60->WriteData();

								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->MoveFocus_FromMyNum();
								}
							}
							break;
//2018.03.13 INSERT END
						case ID_ICSSH_KANI_TAB : //	本表
							if( m_pKani ){
//-- '15.02.24 --
//								m_pKani->GetDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pKani->GetDiagData();
								}
//---------------
								m_pKani->calqx_all( 6, 1 );
								m_pKani->WriteData();
							}

							//[Y]150119
							if( m_pKaniEX ){
//-- '15.02.24 --
//								m_pKaniEX->GetDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pKaniEX->GetDiagData();
								}
//---------------
								m_pKaniEX->calqx_all( 6, 1 );
								m_pKaniEX->WriteData();
							}

							//--------->yoshida150811
							if( m_pKaniEX2 ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pKaniEX2->GetDiagData();
								}
								m_pKaniEX2->calqx_all( 6, 1 );
								m_pKaniEX2->WriteData();
							}
							//<----------------------


							// 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetTitle();
									m_pRefundKjn01->SetDispPosition(0);
								}
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetTitle();
									m_pRefundHjn01->SetDispPosition(1);
								}
							}
							break;

						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetDispPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetDispPosition(3);
								}
							}
							break;
/*

							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetDispPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetDispPosition(3);
								}
							}
							break;
*/							
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装無し
//									((CShinKjnKsk*)ShinTab[4])->GetAllData();
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									// 中身実装無し
//									((CShinHjnKsk*)ShinTab[4])->GetAllData();
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
								}
								//<------------------
							}
							break;
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
			}
			else{	// 原則課税
				
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO10_TAB : // 付表１
							if( m_pFhyo10 ){
//-- '15.02.24 --
//								m_pFhyo10->GetAllDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo10->GetAllDiagData();
								}
//---------------
								m_pFhyo10->f1calq_all( 1, 0 );
								m_pFhyo10->WriteData();
							}

							//------>yoshida150811
							if( m_pFhyo10EX ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
//2015.11.11 INSERT START
									m_pFhyo10EX->SetEditFlg();
//2015.11.11 INSERT END
									m_pFhyo10EX->GetAllDiagData();
								}
								m_pFhyo10EX->f1calq_all( 1, 0 );
								m_pFhyo10EX->WriteData();
							}
							//<-------------------

							if( m_pIppan ){
								m_pIppan->calq_by_OtherChange( 0 );
							}

							//------>yoshida150811
							if( m_pIppanEX ){
								m_pIppanEX->calq_by_OtherChange( 0 );
							}
							//<-------------------

//--> '15.02.03 INS START
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo22Data, m_ShRevType );
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->calq_by_OtherChange( 2, m_pSnFhyo22Data, m_ShRevType );
								}
							}
//<-- '15.02.03 INS END
							break;

						case ID_ICSSH_FHYO22_TAB : // 付表２－２
							if( m_pFhyo22 ){
//-- '15.02.24 --
//								m_pFhyo22->GetAllDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo22->GetAllDiagData();
								}
//---------------
								m_pFhyo22->f22calq_all( 1 );
								m_pFhyo22->WriteData( 1 );
							}

							//------>yoshida150811
							if( m_pFhyo22EX ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo22EX->GetAllDiagData();
								}
								m_pFhyo22EX->f22calq_all( 1 );
								m_pFhyo22EX->WriteData( 1 );
							}
							//<-------------------

							if( m_pFhyo10 ){
								m_pFhyo10->calq_by_OtherChange( 1 );
							}
							//------>yoshida150811
							if( m_pFhyo10EX ){
								m_pFhyo10EX->calq_by_OtherChange( 1 );
							}
							//<-------------------

							if( m_pIppan ){
								m_pIppan->calq_by_OtherChange( 0 );
							}
							//------>yoshida150811
							if( m_pIppanEX ){
								m_pIppanEX->calq_by_OtherChange( 0 );
							}
							//<-------------------
//--> '15.02.03 INS START
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo22Data, m_ShRevType );
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->calq_by_OtherChange( 2, m_pSnFhyo22Data, m_ShRevType );
								}
							}
//<-- '15.02.03 INS END
							break;
//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->GetAllDiagData();
								}

								m_pFhyo60->WriteData();

								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->MoveFocus_FromMyNum();
								}
							}
							break;
//2018.03.13 INSERT END
						case ID_ICSSH_IPPAN_TAB : // 本表
							if( m_pIppan ){
//-- '15.02.24 --
//								m_pIppan->GetDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pIppan->GetDiagData();
								}
//---------------
								m_pIppan->calqx_all( 6, 1 );
								m_pIppan->WriteData();
							}

							//--------->yoshida150811
							if( m_pIppanEX ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pIppanEX->GetDiagData();
								}
								m_pIppanEX->calqx_all( 6, 1 );
								m_pIppanEX->WriteData();
							}
							//<----------------------

							// 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetTitle();
									m_pRefundKjn01->SetDispPosition(0);
								}
//--> '15.02.03 INS START
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo22Data, m_ShRevType );
								}
//<-- '15.02.03 INS END
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetTitle();
									m_pRefundHjn01->SetDispPosition(1);
								}
//--> '15.02.03 INS START
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->calq_by_OtherChange( 2, m_pSnFhyo22Data, m_ShRevType );
								}
//<-- '15.02.03 INS END
							}
							break;

//--> '15.08.20 INS START
						case ID_ICSSH_TKTEI_TAB :
							if( m_pTKUtiwake ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pTKUtiwake->GetDiagData();
								}
//								m_pTKUtiwake->calqx_all( 6, 1 );
								m_pTKUtiwake->WriteData();
							}
							break;
//<-- '15.08.20 INS END
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
/*							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetTitle();
									m_pRefundKjn02->SetDispPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetTitle();
									m_pRefundHjn02->SetDispPosition(3);
								}
							}
							break;
*/						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装なし
//									((CShinKjnKsk*)ShinTab[5])->GetAllData();
								}
								//--------->yoshida150811
								if( m_pKjnKskEX ){
									
								}
								//<----------------------
							}
							else{
								if( m_pHjnKsk ){
									// 中身実装なし
//									((CShinHjnKsk*)ShinTab[5])->GetAllData();
								}
								//--------->yoshida150811
								if( m_pHjnKskEX ){
									
								}
								//<----------------------
							}
							break;							
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO20_TAB : // 付表２
							if( m_pFhyo20 ){
//-- '15.02.24 --
//								m_pFhyo20->GetAllDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo20->GetAllDiagData();
								}
//---------------
								m_pFhyo20->f2calq_all( 1 );
								m_pFhyo20->WriteData( 1 );
							}

							//-------------->yoshida150811
							if( m_pFhyo20EX ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pFhyo20EX->GetAllDiagData();
								}
								m_pFhyo20EX->f2calq_all( 1 );
								m_pFhyo20EX->WriteData( 1 );
							}
							//<-------------------

							// 付表１のクラス作成
//							pShinFhy10	= new CShinFhy10;
							if(m_pFhyo10){
								m_pFhyo10->calq_by_OtherChange( 1 );
							}
							//-------------->yoshida150811
							if(m_pFhyo10EX){
								m_pFhyo10EX->calq_by_OtherChange( 1 );
							}
							//<--------------------

//							delete( pShinFhy10 );
							if( m_pIppan ){
								m_pIppan->calq_by_OtherChange( 0 );
							}

							//-------------->yoshida150811
							if( m_pIppanEX ){
								m_pIppanEX->calq_by_OtherChange( 0 );
							}
							//<---------------------------

//--> '15.02.03 INS START
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo20Data, m_ShRevType );
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->calq_by_OtherChange( 2, m_pSnFhyo20Data, m_ShRevType );
								}
							}
//<-- '15.02.03 INS END
							break;
//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->GetAllDiagData();
								}

								m_pFhyo60->WriteData();

								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
									m_pFhyo60->MoveFocus_FromMyNum();
								}
							}
							break;
//2018.03.13 INSERT END
						case ID_ICSSH_IPPAN_TAB : // 本表
							if( m_pIppan ){
//-- '15.02.24 --
//								m_pIppan->GetDiagData();
//---------------
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pIppan->GetDiagData();
								}
//---------------
								m_pIppan->calqx_all( 6, 1 );
								m_pIppan->WriteData();
							}
							//-------------->yoshida150811
							if( m_pIppanEX ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pIppanEX->GetDiagData();
								}
								m_pIppanEX->calqx_all( 6, 1 );
								m_pIppanEX->WriteData();
							}
							//<---------------------------

							// 付表１のクラス作成＆計算
//							pShinFhy10	= new CShinFhy10;
							if(m_pFhyo10){
								m_pFhyo10->calq_by_OtherChange( 1 );
							}

							//-------------->yoshida150811
							if(m_pFhyo10EX){
								m_pFhyo10EX->calq_by_OtherChange( 1 );
							}
							//<---------------------------

//							delete( pShinFhy10 );
							// 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetTitle();
									m_pRefundKjn01->SetDispPosition(0);
								}
//--> '15.02.03 INS START
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo20Data, m_ShRevType );
								}
//<-- '15.02.03 INS END
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetTitle();
									m_pRefundHjn01->SetDispPosition(1);
								}
//--> '15.02.03 INS START
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->calq_by_OtherChange( 2, m_pSnFhyo20Data, m_ShRevType );
								}
//<-- '15.02.03 INS END
							}
							break;
//--> '15.08.20 INS START
						case ID_ICSSH_TKTEI_TAB :
							if( m_pTKUtiwake ){
//2017.03.01 UPDATE START
//								if( m_MainTab.GetCurSel() == selno ){
								if(( m_MainTab.GetCurSel() == selno )&&( get_sgn == 0 )){
//2017.03.01 UPDATE END
									m_pTKUtiwake->GetDiagData();
								}
//								m_pTKUtiwake->calqx_all( 6, 1 );
								m_pTKUtiwake->WriteData();
							}
							break;
//<-- '15.08.20 INS END
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
/*							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetDispPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetDispPosition(3);
								}
							}
*/							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装なし
//									((CShinKjnKsk*)ShinTab[4])->GetAllData();
								}
								//--------->yoshida150811
								if( m_pKjnKskEX ){

								}
								//<----------------------
							}
							else{
								if( m_pHjnKsk ){
									// 中身実装なし
//									((CShinHjnKsk*)ShinTab[4])->GetAllData();
								}
								//--------->yoshida150811
								if( m_pHjnKskEX ){

								}
								//<----------------------
							}
							break;						
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
			}
		}
		// 仕入控除税額に関する明細書は無いのでカット！
/*		else{							// 仕入控除税額に関する明細書

			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
				// 簡易課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( selno ){
						case 0 : // 付表４
							if( m_pFhyo40 ){
								m_pFhyo40->GetAllDiagData();
								m_pFhyo40->WriteData();
							}
							if( m_pKani ){
								m_pKani->calq_by_OtherChange( 1 );
							}
							break;

						case 1 : // 付表５－２
							if( m_pFhyo52 ){
								// 中身実装なし
//								((CShinFhy52*)ShinTab[1])->f52data_all();
								m_pFhyo52->calq_by_OtherChange( 0 );
								m_pFhyo52->WriteData();
							}
							if( m_pFhyo40 ){
								m_pFhyo40->calq_by_OtherChange();
							}
							if( m_pKani ){
								m_pKani->calq_by_OtherChange( 1 );
							}
							break;
						case 2 :	// 付表５－２（２）
							break;
						case 3 :	// 本表
							if( m_pKani ){
								m_pKani->GetDiagData();
								m_pKani->calqx_all( 6, 1 );
								m_pKani->WriteData();
							}
//							break;
//						case 4 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
//								((CShinKjnSkj*)ShinTab[3])->GetAllData();
							}
							else{
//								((CShinHjnSkj*)ShinTab[3])->GetAllData();
							}
							break;
						case 4 : // 仕入控除
							break;
						case 5 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){ 
								break;	
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装無し
//									((CShinKjnKsk*)ShinTab[4])->GetAllData();
								}
							}
							else{
								if( m_pHjnKsk ){
									// 中身実装無し
//									((CShinHjnKsk*)ShinTab[4])->GetAllData();
								}
							}
							break;												
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( selno ){
						case 0 : // 付表５
							if( m_pFhyo50 ){
								// 中身実装無し
//								((CShinFhy50*)ShinTab[0])->f5data_all();
								m_pFhyo50->f5calq_all( 1 );
								m_pFhyo50->WriteData();
							}
							// 付表１のクラス作成
//							pShinFhy40	= new CShinFhy40;
//							pShinFhy40->fh4_calq();
//							delete( pShinFhy40 );
							if( m_pKani ){
								m_pKani->calq_by_OtherChange( 1 );
							}
							break;

						case 1 : //	本表
							if( m_pKani ){
								m_pKani->GetDiagData();
								m_pKani->calqx_all( 6, 1 );
								m_pKani->WriteData();
							}
//							break;
//						case 2 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
//								((CShinKjnSkj*)ShinTab[2])->GetAllData();
							}
							else{
//								((CShinHjnSkj*)ShinTab[2])->GetAllData();
							}
							break;
						case 2 : // 仕入控除
							break;
						case 3 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装無し
//									((CShinKjnKsk*)ShinTab[3])->GetAllData();
								}
							}
							else{
								if( m_pHjnKsk ){
									// 中身実装無し
//									((CShinHjnKsk*)ShinTab[3])->GetAllData();
								}
							}
							break;												
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
			}
			else{
				// 一般課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( selno ) {
						case 0 : // 付表１
							if( m_pFhyo10 ){
								m_pFhyo10->GetAllDiagData();
								m_pFhyo10->f1calq_all( 1, 0 );
								m_pFhyo10->WriteData();
							}
							if( m_pIppan ){
								m_pIppan->calq_by_OtherChange( 0 );
							}
							break;
						case 1 : // 付表２－２
							if( m_pFhyo22 ){
								m_pFhyo22->GetAllDiagData();
								m_pFhyo22->WriteData( 1 );
							}
							if( m_pFhyo10 ){
								m_pFhyo10->calq_by_OtherChange( 1 );
							}
							if( m_pIppan ){
								m_pIppan->calq_by_OtherChange( 0 );
							}
							break;
						case 2 : // 本表
							if( m_pIppan ){
								m_pIppan->GetDiagData();
								m_pIppan->calqx_all( 6, 1 );
								m_pIppan->WriteData();
							}
//							break;
//						case 3 : // 仕入控除
							if(  m_pSnHeadData->IsSoleProprietor() == TRUE ){
//								((CShinKjnSkj*)ShinTab[3])->GetAllData();
							}
							else{
//								((CShinHjnSkj*)ShinTab[3])->GetAllData();
							}
							break;
						case 3 : // 仕入控除
							break;
						case 4 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装無し
//									((CShinKjnKsk*)ShinTab[4])->GetAllData();
								}
							}
							else{
								if( m_pHjnKsk ){
									// 中身実装無し
//									((CShinHjnKsk*)ShinTab[4])->GetAllData();
								}
							}
							break;							
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( selno ) {
						case 0 : // 付表２
							if( m_pFhyo20 ){
								m_pFhyo20->GetAllDiagData();
								m_pFhyo20->f2calq_all( 1 );
								m_pFhyo20->WriteData( 1 );
							}
//							// 付表１のクラス作成
//							pShinFhy10	= new CShinFhy10;
//							pShinFhy10->fh1_calq( 1 );
//							delete( pShinFhy10 );
							if( m_pIppan ){
								m_pIppan->calq_by_OtherChange( 0 );
							}
							break;
						case 1 : // 本表
							if( m_pIppan ){
								m_pIppan->GetDiagData();
								m_pIppan->calqx_all( 6, 1 );
								m_pIppan->WriteData();
							}
							// 付表１のクラス作成＆計算
//							pShinFhy10	= new CShinFhy10;
//							pShinFhy10->fh1_calq( 1 );
//							delete( pShinFhy10 );
							// 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
//								((CShinKjnSkj*)ShinTab[2])->GetAllData();
							}
							else{
//								((CShinHjnSkj*)ShinTab[2])->GetAllData();
							}
							break;
						case 2 : // 仕入控除
							break;
						case 3 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
								if( m_pKjnKsk ){
									// 中身実装なし
//									((CShinKjnKsk*)ShinTab[3])->GetAllData();
								}
							}
							else{
								if( m_pHjnKsk ){
									// 中身実装なし
//									((CShinHjnKsk*)ShinTab[3])->GetAllData();
								}
							}
							break;						
						default:
							ASSERT( !_T("UpdateSheet_selno_ERROR") );
							break;
					}
				}
			}
		}*/
	}
}

void CH26HyoView::OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result)
{

	int selno = m_MainTab.GetCurSel();

	if( m_nowTabIdx != selno ){

//2018.08.21 INSERT START
		if( m_TabMng.GetCount() ){
			if(m_TabMng[selno].tabtype == ID_ICSSH_FHYO60_TAB){
				if( IsExist_MyNumber_Fuhyo6()&&!m_NoHistory_browse){
					WriteNoHistory(0);
					m_NoHistory_browse = TRUE;
				}			
			}
		}
//2018.08.21 INSERT END
//2018.03.13 INSERT START
		int tabno;
		for( tabno = 0; tabno < m_TabMng.GetCount(); tabno++ ){
			if( m_TabMng[tabno].tabtype == ID_ICSSH_FHYO60_TAB ){
				break;
			}
		}
		if( selno != tabno ){
			if( tabno < m_TabMng.GetCount() ){
				if( m_pFhyo60->MyNumber_CheckFromDisp(1) < 0 ){
					m_MainTab.SetCurSel( tabno );
					m_nowTabIdx = tabno;

					return;
				}
				if( m_pFhyo60->MyNumberCheck(1) == -1 ){
					m_MainTab.SetCurSel( tabno );
					m_nowTabIdx = tabno;

					return;
				}
				if( m_pFhyo60->SozokubunCheck(1) == -1 ){
					m_MainTab.SetCurSel( tabno );
					m_nowTabIdx = tabno;

					return;
				}
			}
		}
//2018.03.13 INSERT END

		// 各表の更新処理後の再表示
		IndicateSheet( selno );
		SetItemPosition( selno );

		m_nowTabIdx = selno;
	}else{
		SetItemPosition( selno );
	}
}

// 各表の更新処理後の再表示
void CH26HyoView::IndicateSheet( int selno )
{
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第２６号様式）
//		((CShinChsk*)ShinTab[selno])->ShinChskInitialize();
		if( m_pChsk ){
			m_pChsk->Init();
		}
		//------>yoshida150811
		if( m_pChskEX ){
			m_pChskEX->Init();
		}
		//<-------------------

	}
	else{
		if( m_pSnHeadData->IsUseTaxRefund() ){	// 消費税の還付申告に関する明細書

			if( (int)m_TabMng.GetCount() <= selno ){
				return;
			}

			// 経理処理
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
				// 簡易課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO40_TAB : // 付表４
							if( m_pFhyo40 ){
								m_pFhyo40->Init();
							}
							//[Y]150119
							if( m_pFhyo40EX ){
								m_pFhyo40EX->Init();
							}
							break;

						case ID_ICSSH_FHYO52_1_TAB : // 付表５－２
							if( m_pFhyo52 ){
								m_pFhyo52->Init();
							}
							//[Y]150119
							if( m_pFhyo52EX ){
								m_pFhyo52EX->Init();
							}
							break;

						case ID_ICSSH_FHYO52_2_TAB : // 付表５－２
							if( m_pFhyo522 ){
								m_pFhyo522->Init();
							}
							//[Y]150119
							if( m_pFhyo522EX ){
								m_pFhyo522EX->Init();
							}

							break;

						case ID_ICSSH_KANI_TAB : // 本表
							if( m_pKani ){
								m_pKani->Init();
							}
							//[Y]150119
							if( m_pKaniEX ){
								m_pKaniEX->Init();
							}
							//------>yoshida150811
							if( m_pKaniEX2 ){
								m_pKaniEX2->Init();
							}
							//<-------------------

							break;
//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								m_pFhyo60->Init();
							}
							break;
//2018.03.13 INSERT END
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
								//-------->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->DispKsiKjn();
								}
								//<---------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
								//-------->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->DispKsiHjn();
								}
								//<---------------------
							}
							break;					
						default:
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO50_TAB : // 付表５
							if( m_pFhyo50 ){
								m_pFhyo50->Init();
							}
							//[Y]150119
							if( m_pFhyo50EX ){
								m_pFhyo50EX->Init();
							}
							break;
						case ID_ICSSH_KANI_TAB : // 本表
							if( m_pKani ){
								m_pKani->Init();
							}
							//[Y]150119
							if( m_pKaniEX ){
								m_pKaniEX->Init();
							}
							//-------->yoshida150811
							if( m_pKaniEX2 ){
								m_pKaniEX2->Init();
							}
							//<--------------------
							break;
//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								m_pFhyo60->Init();
							}
							break;
//2018.03.13 INSERT END
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->DispKsiKjn();
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->DispKsiHjn();
								}
								//<------------------
							}
							break;						
						default:
							break;
					}
				}
			}
			else{
				// 一般課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO10_TAB : // 付表１
							if( m_pFhyo10 ){
								m_pFhyo10->Init();
							}
							//----->yoshida150811
							if( m_pFhyo10EX ){
								m_pFhyo10EX->Init();
							}
							//<------------------
							break;
						case ID_ICSSH_FHYO22_TAB : // 付表２－２
							if( m_pFhyo22 ){
								m_pFhyo22->Init();
							}
							//----->yoshida150811
							if( m_pFhyo22EX ){
								m_pFhyo22EX->Init();
							}
							//<------------------
							break;
						case ID_ICSSH_IPPAN_TAB : // 本表
							if( m_pIppan ){
								m_pIppan->Init();
							}
							//----->yoshida150811
							if( m_pIppanEX ){
								m_pIppanEX->Init();
							}
							//<------------------
							break;
//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								m_pFhyo60->Init();
							}
							break;
//2018.03.13 INSERT END
//--> '15.08.20 INS START
						case ID_ICSSH_TKTEI_TAB :
							if( m_pTKUtiwake ){
								m_pTKUtiwake->Init();
							}
							break;
//<-- '15.08.20 INS END
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->DispKsiKjn();
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->DispKsiHjn();
								}
								//<------------------
							}
							break;						
						default:
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO20_TAB : // 付表２
							if( m_pFhyo20 ){
								m_pFhyo20->Init();
							}
							//----->yoshida150811
							if( m_pFhyo20EX ){
								m_pFhyo20EX->Init();
							}
							//<------------------
							break;
						case ID_ICSSH_IPPAN_TAB : // 本表
							if( m_pIppan ){
								m_pIppan->Init();
							}
							//----->yoshida150811
							if( m_pIppanEX ){
								m_pIppanEX->Init();
							}
							//<------------------
							break;
//2018.03.13 INSERT START
						case ID_ICSSH_FHYO60_TAB : // 付表６
							if( m_pFhyo60 ){
								m_pFhyo60->Init();
							}
							break;
//2018.03.13 INSERT END
//--> '15.08.20 INS START
						case ID_ICSSH_TKTEI_TAB :
							if( m_pTKUtiwake ){
								m_pTKUtiwake->Init();
							}
							break;
//<-- '15.08.20 INS END
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->DispKsiKjn();
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->DispKsiHjn();
								}
								//<------------------
							}
							break;					
						default:
							break;
					}
				}
			}
		}
		
		// 仕入控除税額に関する明細書は無いのでカット

/*		else{							// 仕入控除税額に関する明細書
			// 経理処理
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
				// 簡易課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( selno ){
						case 0 : // 付表４
							if( m_pFhyo40 ){
								m_pFhyo40->Init();
							}
							break;
						case 1 : // 付表５－２
							if( m_pFhyo52 ){
								m_pFhyo52->Init();
							}
							break;
						case 2 : // 付表５－２（２）
							if( m_pFhyo522 ){
								m_pFhyo522->Init();
							}
							break;
						case 3 : // 本表
							if( m_pKani ){
								m_pKani->Init();
							}
							break;
						case 4 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[3])->ShinKjnSkjInitialize();
							}
							else{
//								((CShinHjnSkj*)ShinTab[3])->ShinHjnSkjInitialize();
							}
							break;
						case 5 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
							}
							break;
						default: // 付表
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( selno ){
						case 0 : // 付表５
							if( m_pFhyo50 ){
								m_pFhyo50->Init();
							}
							break;
						case 1 : // 本表
							if( m_pKani ){
								m_pKani->Init();
							}
							break;
						case 2 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[2])->ShinKjnSkjInitialize();
							}
							else{
//								((CShinHjnSkj*)ShinTab[2])->ShinHjnSkjInitialize();
							}
							break;
						case 3 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
							}
							break;						
						default:
							break;
					}
				}
			}
			else{
				// 一般課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( selno ){
						case 0 : // 付表１
							if( m_pFhyo10 ){
								m_pFhyo10->Init();
							}
							break;
						case 1 : // 付表２－２
							if( m_pFhyo22 ){
								m_pFhyo22->Init();
							}
							break;
						case 2 : // 本表
							if( m_pIppan ){
								m_pIppan->Init();
							}
							break;
						case 3 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[3])->ShinKjnSkjInitialize();
							}
							else{
//								((CShinHjnSkj*)ShinTab[3])->ShinHjnSkjInitialize();
							}
							break;
						case 4 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
							}
							break;						
						default: // 付表１
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( selno ){
						case 0 : // 付表２
							if( m_pFhyo20 ){
								m_pFhyo20->Init();
							}
							break;
						case 1 : // 本表
							if( m_pIppan ){
								m_pIppan->Init();
							}
							break;
						case 2 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[2])->ShinKjnSkjInitialize();
							}
							else{
//								((CShinHjnSkj*)ShinTab[2])->ShinHjnSkjInitialize();
							}
							break;
						case 3 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->DispKsiKjn();
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->DispKsiHjn();
								}
							}
							break;					
						default:
							break;
					}
				}
			}
		}*/
	}
}

//-----------------------------------------------------------------------------
// 初期入力位置にフォーカスセット
//-----------------------------------------------------------------------------
// 引数	selno	：	セットするタブNO
//-----------------------------------------------------------------------------
void CH26HyoView::SetItemPosition( int selno )
{
//--> '15.02.20 INS START
	if( (m_pSnHeadData==NULL) || (m_pZmSub==NULL) ){
		return;
	}
//<-- '15.02.20 INS END

	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第２６号様式）
		if( m_pChsk ){
			m_pChsk->SetFocus();
			m_pChsk->SetItemPosition(0);
		}
		//----->yoshida150811
		if( m_pChskEX ){
			m_pChskEX->SetFocus();
			m_pChskEX->SetItemPosition(0);
		}
		//<------------------
	}
	else{
		if( (selno==-1) || ((int)m_TabMng.GetCount()<=selno) ){
			return;
		}

		// 経理処理
		if( m_pSnHeadData->IsUseTaxRefund() ){		// 消費税の還付申告に関する明細書
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
				// 簡易課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO40_TAB : // 付表４
							if( m_pFhyo40 ){
								m_pFhyo40->SetItemPosition();
//								((CShinFhy40*)ShinTab[selno])->SetItemPosition(0);
							}
							if(m_pFhyo40EX){//[Y]150105
								m_pFhyo40EX->SetItemPosition();
							}
							break;
						case ID_ICSSH_FHYO52_1_TAB : // 付表５ー２
							if( m_pFhyo52 ){
//								m_MainTab.SetFocus();
//								m_pFhyo52->GotoDlgCtrl( &m_pFhyo52->m_Fhyo52Diag02 );
								m_pFhyo52->SetItemPosition();
//								((CShinFhy52*)ShinTab[selno])->SetItemPosition(0);
							}
							if( m_pFhyo52EX ){//[Y]150105
								m_pFhyo52EX->SetItemPosition();
							}
							break;
						case ID_ICSSH_FHYO52_2_TAB : // 付表５ー２（２）
							if( m_pFhyo522 ){
								m_pFhyo522->SetItemPosition();
							}
							if( m_pFhyo522EX ){//[Y]150105
								m_pFhyo522EX->SetItemPosition();
							}
							break;
						case ID_ICSSH_KANI_TAB : // 本表
							if( m_pKani ){
								m_pKani->SetItemPosition( 0 );
							}
							if( m_pKaniEX ){//[Y]150105
								m_pKaniEX->SetItemPosition( 0 );
							}
							//----->yoshida150811
							if( m_pKaniEX2 ){//[Y]150105
								m_pKaniEX2->SetItemPosition( 0 );
							}
							//<------------------
							break;
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetInitPosition(0);
								}
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetInitPosition(1);
								}
							}
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetInitPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetInitPosition(3);
								}
							}
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->SetItemPosition( 0 );
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->SetItemPosition( 0 );
								}
								//<-------------------
							}
							break;					
						default:
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO50_TAB : // 付表５
							if( m_pFhyo50 ){
								m_pFhyo50->SetItemPosition();
							}
							if( m_pFhyo50EX ){//[Y]150105
								m_pFhyo50EX->SetItemPosition();
							}
							break;
						case ID_ICSSH_KANI_TAB : // 本表
							if( m_pKani ){
								m_pKani->SetItemPosition( 0 );
							}
							if( m_pKaniEX ){//[Y]150105
								m_pKaniEX->SetItemPosition( 0 );
							}
							//----->yoshida150811
							if( m_pKaniEX2 ){
								m_pKaniEX2->SetItemPosition( 0 );
							}
							//<------------------
							break;
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetInitPosition(0);
								}
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetInitPosition(1);
								}
							}
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetInitPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetInitPosition(3);
								}
							}
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->SetItemPosition( 0 );
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->SetItemPosition( 0 );
								}
								//<------------------
							}
							break;						
						default:
							break;
					}
				}
			}
			else{
				// 一般課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO10_TAB: // 付表１
							// 付表１　再描画
							/*if( m_pFhyo22 ){	// テスト削除 ('14.03.06)
								m_pFhyo22->SetItemPosition();
//								((CShinFhy22*)ShinTab[selno])->fh22_calq();
							}*/
							if( m_pFhyo10 ){
								//m_pFhyo10->Init();	// フォーカスセットのみの関数に置き換え harino ('14.03.12)
								m_pFhyo10->SetItemPosition();
//								((CShinFhy10*)ShinTab[selno])->SetItemPosition(0);
							}
							//----->yoshida150811
							if( m_pFhyo10EX ){
								m_pFhyo10EX->SetItemPosition();
							}
							//<------------------

							break;
						case ID_ICSSH_FHYO22_TAB : // 付表２－２
							if( m_pFhyo22 ){
								m_pFhyo22->SetItemPosition();
//								((CShinFhy22*)ShinTab[selno])->SetItemPosition(0);
							}
							//----->yoshida150811
							if( m_pFhyo22EX ){
								m_pFhyo22EX->SetItemPosition();
							}
							//<------------------
							break;
						case ID_ICSSH_IPPAN_TAB : // 本表
							if( m_pIppan ){
								m_pIppan->SetItemPosition();
							}
							//----->yoshida150811
							if( m_pIppanEX ){
								m_pIppanEX->SetItemPosition();
							}
							//<------------------
							break;
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetInitPosition(0);
								}
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetInitPosition(1);
								}
							}
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetInitPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetInitPosition(3);
								}
							}
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->SetItemPosition( 0 );
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->SetItemPosition( 0 );
								}
								//<------------------
							}
							break;
//--> '15.08.28 INS START
						case ID_ICSSH_TKTEI_TAB : // 特定課税仕入
							if( m_pTKUtiwake ){
								m_pTKUtiwake->SetItemPosition();
							}
							break;
//<-- '15.08.28 INS END
						default:
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( m_TabMng[selno].tabtype ){
						case ID_ICSSH_FHYO20_TAB : // 付表２
							if( m_pFhyo20 ){
								m_pFhyo20->SetItemPosition();
//								((CShinFhy20*)ShinTab[selno])->SetItemPosition(0);
							}
							//----->yoshida150811
							if( m_pFhyo20EX ){
								m_pFhyo20EX->SetItemPosition();
							}
							//<-----------------
							break;
						case ID_ICSSH_IPPAN_TAB : // 本表
							if( m_pIppan ){
								m_pIppan->SetItemPosition();
							}
							//----->yoshida150811
							if( m_pIppanEX ){
								m_pIppanEX->SetItemPosition();
							}
							//<------------------
							break;
						case ID_ICSSH_REFUND_KJ_01_TAB : // 還付付表①
						case ID_ICSSH_REFUND_HJ_01_TAB : // 還付付表①
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn01 ){
									m_pRefundKjn01->SetInitPosition(0);
								}
							}
							else{
								if( m_pRefundHjn01 ){
									m_pRefundHjn01->SetInitPosition(1);
								}
							}
							break;
						case ID_ICSSH_REFUND_KJ_02_TAB : // 還付付表②
						case ID_ICSSH_REFUND_HJ_02_TAB : // 還付付表②
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pRefundKjn02 ){
									m_pRefundKjn02->SetInitPosition(2);
								}
							}
							else{
								if( m_pRefundHjn02 ){
									m_pRefundHjn02->SetInitPosition(3);
								}
							}
							break;
						case ID_ICSSH_KSK_HJ_TAB : // 更正の請求
						case ID_ICSSH_KSK_KJ_TAB : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei&0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pKjnKskEX ){
									m_pKjnKskEX->SetItemPosition( 0 );
								}
								//<------------------
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
								//----->yoshida150811
								if( m_pHjnKskEX ){
									m_pHjnKskEX->SetItemPosition( 0 );
								}
								//<------------------
							}
							break;					
//--> '15.08.28 INS START
						case ID_ICSSH_TKTEI_TAB : // 特定課税仕入
							if( m_pTKUtiwake ){
								m_pTKUtiwake->SetItemPosition();
							}
							break;
//<-- '15.08.28 INS END
						default :
							break;
					}
				}
			}
//2018.03.13 INSERT START
			if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO60_TAB ){
				if( m_pSnHeadData->IsSoleProprietor() ){
					if( m_pFhyo60 ){
						m_pFhyo60->SetItemPosition();
					}
				}
			}
//2018.03.13 INSERT END
		}

		// 仕入控除税額に関する明細書は無いのでカット
/*		else{							// 仕入控除税額に関する明細書
			// 経理処理
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
				// 簡易課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( selno ){
						case 0 : // 付表４
							if( m_pFhyo40 ){
								m_pFhyo40->SetItemPosition();
//								((CShinFhy40*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 1 : // 付表５ー２
							if( m_pFhyo52 ){
								m_pFhyo52->SetItemPosition();
//								((CShinFhy52*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 2 : // 付表５ー２（２）
							if( m_pFhyo522 ){
								m_pFhyo522->SetItemPosition();
//								((CShinFhy52*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 3 : // 本表
							if( m_pKani ){
								m_pKani->SetItemPosition( 0 );
							}
							break;
						case 4 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							else{
//								((CShinHjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 5 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
							}
							break;					
						default:
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( selno ) {
						case 0 : // 付表５
							if( m_pFhyo50 ){
								m_pFhyo50->SetItemPosition();
//								((CShinFhy50*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 1 : // 本表
							if( m_pKani ){
								m_pKani->SetItemPosition( 0 );
							}
							break;
						case 2 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							else{
//								((CShinHjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 3 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
							}
							break;						
						default :
							break;
					}
				}
			}
			else{
				// 一般課税
				if( m_pSnHeadData->SVmzsw == 1 ){
					// 経過措置対象の課税資産の譲渡等あり
					switch( selno ){
						case 0 : // 付表１
							// 付表１　再描画
							if( m_pFhyo22 ){
//								((CShinFhy22*)ShinTab[selno])->fh22_calq();
							}
							if( m_pFhyo10 ){
								m_pFhyo10->Init();
//								((CShinFhy10*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 1 : // 付表２－２
							if( m_pFhyo22 ){
								m_pFhyo22->SetItemPosition();
//								((CShinFhy22*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 2 : // 本表
							if( m_pIppan ){
								m_pIppan->SetItemPosition();
							}
							break;
						case 3 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							else{
//								((CShinHjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 4 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
							}
							break;
						default: // 付表１
							break;
					}
				}
				else{
					// 経過措置対象の課税資産の譲渡等無し
					switch( selno ){
						case 0 : // 付表２
							if( m_pFhyo20 ){
								m_pFhyo20->SetItemPosition();
//								((CShinFhy20*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 1 : // 本表
							if( m_pIppan ){
								m_pIppan->SetItemPosition();
							}
							break;
						case 2 : // 仕入控除
							if( m_pSnHeadData->IsSoleProprietor() ){
//								((CShinKjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							else{
//								((CShinHjnSkj*)ShinTab[selno])->SetItemPosition(0);
							}
							break;
						case 3 : // 更正の請求
							if( !(m_pSnHeadData->Sn_Syuusei & 0x02) ){
								break;
							}
							if( m_pSnHeadData->IsSoleProprietor() ){
								if( m_pKjnKsk ){
									m_pKjnKsk->SetItemPosition( 0 );
								}
							}
							else{
								if( m_pHjnKsk ){
									m_pHjnKsk->SetItemPosition( 0 );
								}
							}
							break;					
						default: // 付表２
							break;
					}
				}
			}
		}*/
	}
}

void CH26HyoView::OnUpdateButtonF9(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。

/*- '14.02.11 -*/
//	if( m_pSnHeadData == NULL ){
//		return;
//	}
//	else if( (m_pShinInfo==NULL) || (m_pShinInfo->pToolBar==NULL) ){
//		return;
//	}
//	char*		p;
//	if( m_pSnHeadData->Sn_Sign4&0x01 ){
//		p = _T("F9 ロック解除");
//	}
//	else{
//		p = _T("F9 入力ロック");
//	}
//	m_pShinInfo->pToolBar->SetButtonTooltip( 7, p );
//	m_pShinInfo->pToolBar->SetButtonText( 7, p );
/*-------------*/
	// 上のやり方だと、うまく変わらないので変更！
	if( m_pSnHeadData == NULL ){
		return;
	}
	else if( (m_pShinInfo==NULL) || (m_pShinInfo->pParent==NULL) ){
		return;
	}

	ICSToolBar* pBar = ((ICSMainFrame *)m_pShinInfo->pParent)->GetToolBar( 1 );
	if( pBar ){
		char*		p = NULL;
		char*		pDescription = NULL;
		if( m_pSnHeadData->Sn_Sign4&0x01 ){
			p = _T("F9 ロック解除");
			pDescription = _T("入力ロックを解除します");
		}
		else{
			p = _T("F9 入力ロック");
			pDescription = _T("入力をロックします");
		}
		pBar->SetButtonTooltip( 7, p );
		pBar->SetButtonText( 7, p );
		pBar->SetButtonDescription( 7, pDescription );
		return;
	}
/*-------------*/
}

// 欄外印字
void CH26HyoView::OnButtonF6()
{
	int selno = m_MainTab.GetCurSel();

	if( m_pSnHeadData->IsMiddleProvisional() == FALSE ){
//2016.02.23 INSERT START
		//if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
//2016.02.23 INSERT END
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
			if( m_TabMng[selno].tabtype==ID_ICSSH_KANI_TAB ){
				if( m_pKani ){
					m_pKani->PerPrint();
				}
//--> '15.02.23 INS START
				else if( m_pKaniEX ){
					m_pKaniEX->PerPrint();
				}
//<-- '15.02.23 INS END

				//------>yoshida150810
				else if( m_pKaniEX2 ){
					m_pKaniEX2->PerPrint();
				}
				//<-------------------

			}
		}
		else{
			if( m_TabMng[selno].tabtype==ID_ICSSH_IPPAN_TAB ){
				if( m_pIppan ){
					m_pIppan->PerPrint();
				}
				//------>yoshida150810
				else if( m_pIppanEX ){
					m_pIppanEX->PerPrint();
				}
				//<-------------------
			}

		}
//2016.02.23 INSERT START
		//}else{
		//	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
		//		if( m_TabMng[selno].tabtype==ID_ICSSH_KANI_TAB ){
		//			if( m_pKani ){
		//				m_pKani->PerPrint2();
		//			}
		//			else if( m_pKaniEX ){
		//				m_pKaniEX->PerPrint2();
		//			}
		//			else if( m_pKaniEX2 ){
		//				m_pKaniEX2->PerPrint2();
		//			}
		//		}
		//	}
		//	else{
		//		if( m_TabMng[selno].tabtype==ID_ICSSH_IPPAN_TAB ){
		//			if( m_pIppan ){
		//				m_pIppan->PerPrint2();
		//			}
		//			else if( m_pIppanEX ){
		//				m_pIppanEX->PerPrint2();
		//			}
		//		}
		//	}
		//}
//2016.02.23 INSERT END
	}
	SetItemPosition( selno );//[Y]
}

void CH26HyoView::OnButtonF9()
{

//2018.03.13 INSERT START
	if( Message_MyNumber_error() == -1 ){
		return;
	}
	if( Message_MyNumber_overlap() == -1 ){
		return;
	}
	EditOff_Fuhyo6();
	if( Message_SozokuWariai() == -1 ){
		return;
	}
//2018.03.13 INSERT END

//2016.02.22 INSERT START
	m_MainTab.SetFocus();
//2016.02.22 INSERT END

	InputRock(0);//関数化（別箇所から同じ処理呼ぶ為）'14.10.08


	/*'14.10.08
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
		WriteData( FALSE, TRUE );
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
	UpdateSheet( selno );
	if( (svSn_Sign4!=m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4&0x01) ){
		WriteData( FALSE, FALSE );
	}

	if( m_SnSeq == 0 ){	// 現申告
		// 再集計
		m_pShinInfo->pParent->SendMessage( WM_USER_RECALQ, (WPARAM)0 );
	}

	// DIAGの属性変更
	ChangeDiagAttr();

	// 全削除ではなく、一部削除では駄目？？？
	InitTab( TRUE );

	m_MainTab.SetCurSel( selno );
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );
	SetItemPosition( selno );
	*/

}

//-----------------------------------------------------------------------------
// DIAGの属性変更
//-----------------------------------------------------------------------------
void CH26HyoView::ChangeDiagAttr()
{
	if( m_pIppan ){
		m_pIppan->ChgDiagAttr();
	}
	if( m_pKani ){
		m_pKani->ChgDiagAttr();
	}
//--> '15.02.23 INS START
	if( m_pKaniEX ){
		m_pKaniEX->ChgDiagAttr();
	}
//<-- '15.02.23 INS END
	//----->yoshida150810
	if( m_pIppanEX ){
		m_pIppanEX->ChgDiagAttr();
	}
	if( m_pKaniEX2 ){
		m_pKaniEX2->ChgDiagAttr();
	}
	//<-----------------

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
int CH26HyoView::GetMony( CString &tag, CString &chtag, char *pArith )
{
	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}

	if( (tag==_T("AAF00000")) || (tag==_T("AAF00010")) || (tag==_T("AAF00020")) || (tag==_T("AAF00030")) || (tag == _T("AAG00140")) || (tag == _T("AAG00150")) ||
		(tag==_T("AAG00160")) || (tag == _T("ABB00180")) || (tag == _T("ABB00010")) || (tag == _T("AAB00010")) || (tag == _T("AAB00180")) ){
		if( m_pSnHonpyoData == NULL ){
//-- '15.02.20 --
//			if( (m_pSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
//				return -1;
//			}
//			if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//---------------

			//-------->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//				if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
//					return -1;
//				}
//				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//---------------
				if( (m_pSnHonpyoData = new CH28SnHonpyoData()) == NULL ){
					return -1;
				}
				if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
//---------------
			}
			else{
				if( m_SixKindFlg ){
					if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
						return -1;
					}
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( (m_pSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
						return -1;
					}
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			//-------------------------
			/*
			if( m_SixKindFlg ){
				if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
					return -1;
				}
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( (m_pSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
					return -1;
				}
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			*/
			//<-------------------------

//---------------
		}
	}
	else if( tag == _T("Sn_KTNFZ") ){
		if( m_pSnChskData == NULL ){
			if( (m_pSnChskData = new CH26SnChskData()) == NULL ){
				return -1;
			}
			if( m_pSnChskData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
	}
	else if( (tag==_T("AEB00030")) || (tag==_T("AEB00040")) || (tag==_T("AEB00050")) ){
		if( m_pSnHeadData->SVmzsw ){
			//--------->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( m_pSnFhyo22Data == NULL ){
					if( (m_pSnFhyo22Data = new CH28SnFhyo20Data(TRUE)) == NULL ){
						return -1;
					}
					if( ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			else{
				if( m_pSnFhyo22Data == NULL ){
					if( (m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			//-----------------------
			/*
			if( m_pSnFhyo22Data == NULL ){
				if( (m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE)) == NULL ){
					return -1;
				}
				if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			*/
			//<----------------------
		}
		else{
			if( m_pSnFhyo20Data == NULL ){
				//--------->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( (m_pSnFhyo20Data = new CH28SnFhyo20Data(FALSE)) == NULL ){
						return -1;
					}
					if( ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}

				}
				else{
					if( (m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//-----------------------
				/*
				if( (m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE)) == NULL ){
					return -1;
				}
				if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<------------------------
			}
		}
	}

	if( tag == _T("AAF00000") ){
		memmove( pArith, m_pSnHonpyoData->Sn_ZZURI, MONY_BUF_SIZE );
	}
	else if( tag == _T("AAF00010") ){
		memmove( pArith, m_pSnHonpyoData->Sn_ZURI, MONY_BUF_SIZE );
	}
	else if( tag == _T("AAF00020") ){
		memmove( pArith, m_pSnHonpyoData->Sn_ZZNOFZ, MONY_BUF_SIZE );
	}
	else if( tag == _T("AAF00030") ){
		memmove( pArith, m_pSnHonpyoData->Sn_ZNOFZ, MONY_BUF_SIZE );
	}
	else if( tag == _T("AAG00140") ){
		memmove( pArith, m_pSnHonpyoData->Sn_TS_TDM1, MONY_BUF_SIZE );
	}
	else if( tag == _T("AAG00150") ){
		memmove( pArith, m_pSnHonpyoData->Sn_TS_SYT1, MONY_BUF_SIZE );
	}
	else if( tag == _T("AAG00160") ){
		memmove( pArith, m_pSnHonpyoData->Sn_TS_SYT2, MONY_BUF_SIZE );
	}
	else if( tag == _T("ABB00180") ){
		memmove( pArith, m_pSnHonpyoData->Sn_ZZURI, MONY_BUF_SIZE );
	}
//--> '15.02.25 INS START
	else if( (tag==_T("ABB00010")) || (tag==_T("AAB00010")) ){
		memmove( pArith, m_pSnHonpyoData->Sn_KSTD, MONY_BUF_SIZE );
	}
	else if( (tag==_T("ABB00170")) || (tag==_T("AAB00180")) ){
		memmove( pArith, m_pSnHonpyoData->Sn_KZURI, MONY_BUF_SIZE );
	}
	else if( (tag==_T("ABB00110")) || (tag==_T("AAB00110")) ){
		memmove( pArith, m_pSnHonpyoData->Sn_TYNOFZ, MONY_BUF_SIZE );
	}
	else if( tag == _T("Sn_TTYWAR") ){
		memmove( pArith, m_pSnHonpyoData->Sn_TTYWAR, MONY_BUF_SIZE );
	}
//<-- '15.02.25 INS END
	else if( tag == _T("AEB00030") ){
		if( m_pSnFhyo22Data ){
			memmove( pArith, m_pSnFhyo22Data->Sn_2F1A, MONY_BUF_SIZE );
		}
		else if( m_pSnFhyo20Data ){
			memmove( pArith, m_pSnFhyo20Data->Sn_2F1A, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AEB00040") ){
		if( m_pSnFhyo22Data ){
			memmove( pArith, m_pSnFhyo22Data->Sn_2F1B, MONY_BUF_SIZE );
		}
		else if( m_pSnFhyo20Data ){
			memmove( pArith, m_pSnFhyo20Data->Sn_2F1B, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AEB00050") ){
		if( m_pSnFhyo22Data ){
			memmove( pArith, m_pSnFhyo22Data->Sn_2F1C, MONY_BUF_SIZE );
		}
		else if( m_pSnFhyo20Data ){
			memmove( pArith, m_pSnFhyo20Data->Sn_2F1C, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AAC00130") ){
		// 集計により求める！
		char	val[MONY_BUF_SIZE]={0};
		SH_ANS	ANS[30] = {0};
		SH_ANS	JANS[10] = {0};
		CalqSyzForKskSub( (void*)ANS, (void*)JANS, val );
		memmove( pArith, val, MONY_BUF_SIZE );
	}
	else if( tag == _T("Sn_KTNFZ") ){
		if( m_pSnChskData ){
			memmove( pArith, m_pSnChskData->Sn_KTNFZ, MONY_BUF_SIZE );
		}
	}
//--> '14.06.27 INS START
	else if( tag == _T("AYB00040") ){
		if( m_pSnTanaData ){
			memmove( pArith, m_pSnTanaData->Sn_menzz[0], MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00100") ){
		if( m_pSnTanaData ){
			memmove( pArith, m_pSnTanaData->Sn_mnz40[0], MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00160") ){
		if( m_pSnTanaData ){
			memmove( pArith, m_pSnTanaData->Sn_mnz63[0], MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00060") ){
		if( m_pSnTanaData ){
			memmove( pArith, m_pSnTanaData->Sn_menzz[2], MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00120") ){
		if( m_pSnTanaData ){
			memmove( pArith, m_pSnTanaData->Sn_mnz40[2], MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AYB00180") ){
		if( m_pSnTanaData ){
			memmove( pArith, m_pSnTanaData->Sn_mnz63[2], MONY_BUF_SIZE );
		}
	}
//<-- '14.06.27 INS END

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
int CH26HyoView::SetMony( CString &tag, CString &chtag, char *pArith )
{
//--> '15.04.14 INS START
	if( (tag==_T("AAF00000")) || (tag==_T("AAF00010")) || (tag==_T("AAF00020")) || (tag==_T("AAF00030")) || (tag == _T("AAG00140")) || (tag == _T("AAG00150")) ||
		(tag==_T("AAG00160")) || (tag == _T("ABB00180")) || (tag == _T("ABB00010")) || (tag == _T("AAB00010")) || (tag == _T("AAB00180")) ){
		if( m_pSnHonpyoData == NULL ){

			//------>yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//				if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
//					return -1;
//				}
//				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//---------------
				if( (m_pSnHonpyoData = new CH28SnHonpyoData()) == NULL ){
					return -1;
				}
				if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
//---------------
			}
			else{
				if( m_SixKindFlg ){
					if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
						return -1;
					}
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( (m_pSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
						return -1;
					}
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}

			//-------------------
			/*
			if( m_SixKindFlg ){
				if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
					return -1;
				}
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( (m_pSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
					return -1;
				}
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			*/
			//<-----------------
		}
	}
	else if( tag == _T("Sn_KTNFZ") ){
		if( m_pSnChskData == NULL ){
			if( (m_pSnChskData = new CH26SnChskData()) == NULL ){
				return -1;
			}
			if( m_pSnChskData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
	}
	else if( (tag==_T("AEB00030")) || (tag==_T("AEB00040")) || (tag==_T("AEB00050")) || (tag==_T("AEC00090")) || (tag==_T("AEC00040")) ||
		(tag==_T("AEC00030")) || (tag==_T("AEC00080")) || (tag==_T("AEG00020")) || (tag==_T("AEG00030")) || (tag==_T("AEG00040")) ){
		if( m_pSnHeadData->SVmzsw ){
			if( m_pSnFhyo22Data == NULL ){
				//------>yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( (m_pSnFhyo22Data = new CH28SnFhyo20Data(TRUE)) == NULL ){
						return -1;
					}
					if( ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}

				}
				else{
					if( (m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//-------------------
				/*
				if( (m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE)) == NULL ){
					return -1;
				}
				if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<-----------------
			}
		}
		else{
			if( m_pSnFhyo20Data == NULL ){
				//------>yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( (m_pSnFhyo20Data = new CH28SnFhyo20Data(FALSE)) == NULL ){
						return -1;
					}
					if( ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}

				}
				else{
					if( (m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//---------------------
				/*
				if( (m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE)) == NULL ){
					return -1;
				}
				if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<---------------------
			}
		}
	}
//-- '15.11.14 --
//	else if( (tag==_T("ACB00030")) || (tag==_T("ACC00030")) || (tag==_T("ACE00090")) || (tag==_T("ACB00020")) || (tag==_T("ACC00020")) ||
//		(tag==_T("ACE00080")) ){
//---------------
	else if( (tag==_T("ACB00030")) || (tag==_T("ACC00030")) || (tag==_T("ACE00090")) || (tag==_T("ACB00020")) || (tag==_T("ACC00020")) ||
		(tag==_T("ACE00080")) || (tag==_T("ACB00080")) || (tag==_T("ACE00240")) || (tag==_T("ACB00070")) || (tag==_T("ACE00230")) ){
//---------------

//-- '15.05.25 --
//		// 付表４
//		if( m_pSnFhyo40Data == NULL ){
//			if(m_SixKindFlg == TRUE){
//				if( (m_pSnFhyo40Data = new CH27SnFhyo10Data(TRUE)) == NULL ){
//					return -1;
//				}
//				if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//			}
//			else{
//				if( (m_pSnFhyo40Data = new CH26SnFhyo10Data(TRUE)) == NULL ){
//					return -1;
//				}
//				if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//			}
//		}
//---------------
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){		// 簡易課税
			// 付表４
			if( m_pSnFhyo40Data == NULL ){
				if(m_SixKindFlg == TRUE){
					if( (m_pSnFhyo40Data = new CH27SnFhyo10Data(TRUE)) == NULL ){
						return -1;
					}
					if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( (m_pSnFhyo40Data = new CH26SnFhyo10Data(TRUE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
		}
		else if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){	// 原則課税
			// 付表１
			if( m_pSnFhyo10Data == NULL ){
				//------>yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( (m_pSnFhyo10Data = new CH28SnFhyo10Data(TRUE)) == NULL ){
						return -1;
					}
					if( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( (m_pSnFhyo10Data = new CH26SnFhyo10Data(TRUE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//---------------------
				/*
				if( (m_pSnFhyo10Data = new CH26SnFhyo10Data(TRUE)) == NULL ){
					return -1;
				}
				if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<----------------------
			}
		}
//---------------
	}
	else if( (tag==_T("AHG00110")) || (tag==_T("AHG00170")) || (tag==_T("AHG00230")) || (tag==_T("AHG00290")) || (tag==_T("AHG00350")) ||
//2016.10.20 INSERT START
		(tag==_T("AHG00410")) || (tag==_T("AHG00400")) ||
//2016.10.20 INSERT END
//--> '16.12.27 INSERT START
		(tag==_T("AHG01730")) ||
//<-- '16.12.27 INSERT END
		(tag==_T("AHG00100")) || (tag==_T("AHG00160")) || (tag==_T("AHG00220")) || (tag==_T("AHG00280")) || (tag==_T("AHG00340")) ||
		(tag==_T("AHG00430")) || (tag==_T("AHG00480")) || (tag==_T("AHG00530")) || (tag==_T("AHG00580")) || (tag==_T("AHG00630")) ||
		(tag==_T("AHG00680")) ){

		// 付表５
		if( m_pSnHeadData->SVmzsw ){
			if( m_pSnFhyo52Data == NULL ){
				if( m_SixKindFlg ){
					if( (m_pSnFhyo52Data = new CH27SnFhyo50Data(TRUE)) == NULL ){
						return -1;
					}
					if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( (m_pSnFhyo52Data = new CH26SnFhyo50Data(TRUE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo52Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
		}
		else{
			if( m_pSnFhyo50Data == NULL ){
				if( m_SixKindFlg ){
					if( (m_pSnFhyo50Data = new CH27SnFhyo50Data(FALSE)) == NULL ){
						return -1;
					}
					if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					if( (m_pSnFhyo50Data = new CH26SnFhyo50Data(FALSE)) == NULL ){
						return -1;
					}
					if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
		}
	}
//---> '15.05.25 CUT START
//	else if( (tag==_T("ACB00030")) || (tag==_T("ACC00030")) || (tag==_T("ACE00090")) || (tag==_T("ACB00020")) || (tag==_T("ACC00020")) ||
//		(tag==_T("ACE00080")) ){
//
//		// 付表１
//		if( m_pSnFhyo10Data == NULL ){
//			if( (m_pSnFhyo10Data = new CH26SnFhyo10Data(TRUE)) == NULL ){
//				return -1;
//			}
//			if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//		}
//	}
//<--- '15.05.25 CUT END
//<-- '15.04.14 INS END

	if( tag == _T("AAF00000") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_ZZURI, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AAF00010") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_ZURI, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AAF00020") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_ZZNOFZ, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AAF00030") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_ZNOFZ, pArith, MONY_BUF_SIZE );
			m_pSnHonpyoData->Sn_ZNOsw = atoi( chtag);
		}
	}
	else if( tag == _T("AAG00140") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_TS_TDM1, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AAG00150") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_TS_SYT1, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("AAG00160") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_TS_SYT2, pArith, MONY_BUF_SIZE );
		}
	}
	else if( (tag==_T("AAB00110")) || (tag==_T("ABB00110")) ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_TYNOFZ, pArith, MONY_BUF_SIZE );
		}
	}
	else if( tag == _T("Sn_TTYWAR") ){
		if( m_pSnHonpyoData ){
			memmove( m_pSnHonpyoData->Sn_TTYWAR, pArith, MONY_BUF_SIZE );
		}
	}

	
	//--->'14.10.08
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){//簡易課税
		if( tag == _T("ACB00030") ){
			if( m_pSnFhyo40Data ){
				memmove( m_pSnFhyo40Data->Sn_1F1C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACC00030") ){
			if( m_pSnFhyo40Data ){
				memmove( m_pSnFhyo40Data->Sn_1F2C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACE00090") ){
			if( m_pSnFhyo40Data ){
				memmove( m_pSnFhyo40Data->Sn_1F5C, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("ACB00020") ){
			if( m_pSnFhyo40Data ){
				memmove( m_pSnFhyo40Data->Sn_1F1B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACC00020") ){
			if( m_pSnFhyo40Data ){
				memmove( m_pSnFhyo40Data->Sn_1F2B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACE00080") ){
			if( m_pSnFhyo40Data ){
				memmove( m_pSnFhyo40Data->Sn_1F5B, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("AHG00110") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FURI1C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI1C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00170") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FURI2C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI2C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00230") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FURI3C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI3C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00290") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FURI4C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI4C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00350") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FURI5C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI5C, pArith, MONY_BUF_SIZE );
			}
		}
//2016.10.20 INSERT START
		else if( tag == _T("AHG00410") ){
			if( m_pSnFhyo50Data ){
				memmove( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->Sn_5FURI6C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->Sn_5FURI6C, pArith, MONY_BUF_SIZE );
			}
		}
//2016.10.20 INSERT END
//--> '16.12.27 INS START
		else if( tag == _T("AHG01730") ){
			if( m_pSnFhyo50Data ){
				memmove( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->Sn_5FUZT6, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->Sn_5FUZT6, pArith, MONY_BUF_SIZE );
			}
		}
//<-- '16.12.27 INS END
		else if( tag == _T("AHG00100") ){
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI1B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00160") ){
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI2B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00220") ){
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI3B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00280") ){
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI4B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00340") ){
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FURI5B, pArith, MONY_BUF_SIZE );
			}
		}
//2016.10.20 INSERT START
		else if( tag == _T("AHG00400") ){
			if( m_pSnFhyo52Data ){
				memmove( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->Sn_5FURI6B, pArith, MONY_BUF_SIZE );
			}
		}
//2016.10.20 INSERT END

		else if( tag == _T("ABB00170") ){
			if( m_pSnHonpyoData ){
				memmove( m_pSnHonpyoData->Sn_KZURI, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("AHG00430") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FUZTT, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FUZTT, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00480") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FUZT1, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FUZT1, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AHG00530") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FUZT2, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FUZT2, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("AHG00580") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FUZT3, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FUZT3, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("AHG00630") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FUZT4, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FUZT4, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("AHG00680") ){
			if( m_pSnFhyo50Data ){
				memmove( m_pSnFhyo50Data->Sn_5FUZT5, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo52Data ){
				memmove( m_pSnFhyo52Data->Sn_5FUZT5, pArith, MONY_BUF_SIZE );
			}
		}

	}
	else{//原則課税
		if( tag == _T("ACB00030") ){
			if( m_pSnFhyo10Data ){
				memmove( m_pSnFhyo10Data->Sn_1F1C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACC00030") ){
			if( m_pSnFhyo10Data ){
				memmove( m_pSnFhyo10Data->Sn_1F2C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACE00090") ){
			if( m_pSnFhyo10Data ){
				memmove( m_pSnFhyo10Data->Sn_1F5C, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("ACB00020") ){
			if( m_pSnFhyo10Data ){
				memmove( m_pSnFhyo10Data->Sn_1F1B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACC00020") ){
			if( m_pSnFhyo10Data ){
				memmove( m_pSnFhyo10Data->Sn_1F2B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACE00080") ){
			if( m_pSnFhyo10Data ){
				memmove( m_pSnFhyo10Data->Sn_1F5B, pArith, MONY_BUF_SIZE );
			}
		}		

		else if( tag == _T("AEB00050") ){
			if( m_pSnFhyo20Data ){
				memmove( m_pSnFhyo20Data->Sn_2F1C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo22Data ){
				memmove( m_pSnFhyo22Data->Sn_2F1C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AEC00040") ){
			if( m_pSnFhyo20Data ){
				memmove( m_pSnFhyo20Data->Sn_2F8C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo22Data ){
				memmove( m_pSnFhyo22Data->Sn_2F8C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AEC00090") ){
			if( m_pSnFhyo20Data ){
				memmove( m_pSnFhyo20Data->Sn_2F9C, pArith, MONY_BUF_SIZE );
			}
			if( m_pSnFhyo22Data ){
				memmove( m_pSnFhyo22Data->Sn_2F9C, pArith, MONY_BUF_SIZE );
			}
		}

		else if( tag == _T("AEB00040") ){
			if( m_pSnFhyo22Data ){
				memmove( m_pSnFhyo22Data->Sn_2F1B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AEC00030") ){
			if( m_pSnFhyo22Data ){
				memmove( m_pSnFhyo22Data->Sn_2F8B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AEC00080") ){
			if( m_pSnFhyo22Data ){
				memmove( m_pSnFhyo22Data->Sn_2F9B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("AEG00020") ){
//-- '15.11.14 --
//			if( m_pSnFhyo20Data ){
//				memmove( m_pSnFhyo20Data->Sn_2F21A, pArith, MONY_BUF_SIZE );
//			}
//			if( m_pSnFhyo22Data ){
//				memmove( m_pSnFhyo22Data->Sn_2F21A, pArith, MONY_BUF_SIZE );
//			}
//---------------
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( m_pSnFhyo20Data ){
					CH28SnFhyo20Data *pH28SnFhyo20Data = (CH28SnFhyo20Data*)m_pSnFhyo20Data;
					memmove( pH28SnFhyo20Data->Sn_2F22A, pArith, MONY_BUF_SIZE );
				}
				if( m_pSnFhyo22Data ){
					CH28SnFhyo20Data *pH28SnFhyo22Data = (CH28SnFhyo20Data*)m_pSnFhyo22Data;
					memmove( pH28SnFhyo22Data->Sn_2F22A, pArith, MONY_BUF_SIZE );
				}
			}
			else{
				if( m_pSnFhyo20Data ){
					memmove( m_pSnFhyo20Data->Sn_2F21A, pArith, MONY_BUF_SIZE );
				}
				if( m_pSnFhyo22Data ){
					memmove( m_pSnFhyo22Data->Sn_2F21A, pArith, MONY_BUF_SIZE );
				}
			}
//---------------
		}
		else if( tag == _T("AEG00030") ){
//-- '15.11.14 --
//			if( m_pSnFhyo20Data ){
//				memmove( m_pSnFhyo20Data->Sn_2F21B, pArith, MONY_BUF_SIZE );
//			}
//			if( m_pSnFhyo22Data ){
//				memmove( m_pSnFhyo22Data->Sn_2F21B, pArith, MONY_BUF_SIZE );
//			}
//---------------
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( m_pSnFhyo20Data ){
					CH28SnFhyo20Data *pH28SnFhyo20Data = (CH28SnFhyo20Data*)m_pSnFhyo20Data;
					memmove( pH28SnFhyo20Data->Sn_2F22B, pArith, MONY_BUF_SIZE );
				}
				if( m_pSnFhyo22Data ){
					CH28SnFhyo20Data *pH28SnFhyo22Data = (CH28SnFhyo20Data*)m_pSnFhyo22Data;
					memmove( pH28SnFhyo22Data->Sn_2F22B, pArith, MONY_BUF_SIZE );
				}
			}
			else{
				if( m_pSnFhyo20Data ){
					memmove( m_pSnFhyo20Data->Sn_2F21B, pArith, MONY_BUF_SIZE );
				}
				if( m_pSnFhyo22Data ){
					memmove( m_pSnFhyo22Data->Sn_2F21B, pArith, MONY_BUF_SIZE );
				}
			}
//---------------
		}
		else if( tag == _T("AEG00040") ){

//-- '15.11.14 --
//			if( m_pSnFhyo20Data ){
//				memmove( m_pSnFhyo20Data->Sn_2F21C, pArith, MONY_BUF_SIZE );
////-- '15.05.20 --
////				m_pSnFhyo20Data->Sn_2F21sw |= atoi(chtag);
////---------------
//				int	svSn_2F21sw = m_pSnFhyo20Data->Sn_2F21sw;
//				m_pSnFhyo20Data->Sn_2F21sw &= ~0x111;
//				m_pSnFhyo20Data->Sn_2F21sw |= atoi(chtag);
//
//				if( svSn_2F21sw != m_pSnFhyo20Data->Sn_2F21sw ){
//					// 異なる時には帳票内データを変更する
//					if( m_pFhyo20 ){
//						m_pFhyo20->SetCalqSw( m_pSnFhyo20Data->Sn_2F21sw );
//					}
//				}
////---------------
//			}
//			if( m_pSnFhyo22Data ){
//				memmove( m_pSnFhyo22Data->Sn_2F21C, pArith, MONY_BUF_SIZE );
////-- '15.05.20 --
////				m_pSnFhyo22Data->Sn_2F21sw |= atoi(chtag);
////---------------
//				int	svSn_2F21sw = m_pSnFhyo22Data->Sn_2F21sw;
//				m_pSnFhyo22Data->Sn_2F21sw &= ~0x111;
//				m_pSnFhyo22Data->Sn_2F21sw |= atoi(chtag);
//
//				if( svSn_2F21sw != m_pSnFhyo22Data->Sn_2F21sw ){
//					// 異なる時には帳票内データを変更する
//					if( m_pFhyo22 ){
//						m_pFhyo22->SetCalqSw( m_pSnFhyo22Data->Sn_2F21sw );
//					}
//				}
////---------------
//			}
//---------------
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( m_pSnFhyo20Data ){
					CH28SnFhyo20Data *pH28SnFhyo20Data = (CH28SnFhyo20Data*)m_pSnFhyo20Data;

					memmove( pH28SnFhyo20Data->Sn_2F22C, pArith, MONY_BUF_SIZE );
					int	svSn_2F22sw = pH28SnFhyo20Data->Sn_2F22sw;
					pH28SnFhyo20Data->Sn_2F22sw &= ~0x111;
					pH28SnFhyo20Data->Sn_2F22sw |= atoi(chtag);

					if( svSn_2F22sw != pH28SnFhyo20Data->Sn_2F22sw ){
						// 異なる時には帳票内データを変更する
						if( m_pFhyo20EX ){
							m_pFhyo20EX->SetCalqSw( pH28SnFhyo20Data->Sn_2F22sw );
						}
					}
				}
				if( m_pSnFhyo22Data ){
					CH28SnFhyo20Data *pH28SnFhyo22Data = (CH28SnFhyo20Data*)m_pSnFhyo22Data;

					memmove( pH28SnFhyo22Data->Sn_2F22C, pArith, MONY_BUF_SIZE );
					int	svSn_2F22sw = pH28SnFhyo22Data->Sn_2F22sw;
					pH28SnFhyo22Data->Sn_2F22sw &= ~0x111;
					pH28SnFhyo22Data->Sn_2F22sw |= atoi(chtag);

					if( svSn_2F22sw != pH28SnFhyo22Data->Sn_2F22sw ){
						// 異なる時には帳票内データを変更する
						if( m_pFhyo22EX ){
							m_pFhyo22EX->SetCalqSw( pH28SnFhyo22Data->Sn_2F22sw );
						}
					}
				}
			}
			else{
				if( m_pSnFhyo20Data ){
					memmove( m_pSnFhyo20Data->Sn_2F21C, pArith, MONY_BUF_SIZE );
					int	svSn_2F21sw = m_pSnFhyo20Data->Sn_2F21sw;
					m_pSnFhyo20Data->Sn_2F21sw &= ~0x111;
					m_pSnFhyo20Data->Sn_2F21sw |= atoi(chtag);

					if( svSn_2F21sw != m_pSnFhyo20Data->Sn_2F21sw ){
						// 異なる時には帳票内データを変更する
						if( m_pFhyo20 ){
							m_pFhyo20->SetCalqSw( m_pSnFhyo20Data->Sn_2F21sw );
						}
					}
				}
				if( m_pSnFhyo22Data ){
					memmove( m_pSnFhyo22Data->Sn_2F21C, pArith, MONY_BUF_SIZE );
					int	svSn_2F21sw = m_pSnFhyo22Data->Sn_2F21sw;
					m_pSnFhyo22Data->Sn_2F21sw &= ~0x111;
					m_pSnFhyo22Data->Sn_2F21sw |= atoi(chtag);

					if( svSn_2F21sw != m_pSnFhyo22Data->Sn_2F21sw ){
						// 異なる時には帳票内データを変更する
						if( m_pFhyo22 ){
							m_pFhyo22->SetCalqSw( m_pSnFhyo22Data->Sn_2F21sw );
						}
					}
				}
			}
//---------------
		}
//--> '15.11.14 INS START
		else if( tag == _T("ACB00080") ){
			if( m_pSnFhyo10Data && (m_ShRevType>=ID_ICSSH_TKKZSIIRE_REVTYPE) ){
				memmove( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->Sn_1F1_1C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACE00240") ){
			if( m_pSnFhyo10Data && (m_ShRevType>=ID_ICSSH_TKKZSIIRE_REVTYPE) ){
				memmove( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->Sn_1F5_1C, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACB00070") ){
			if( m_pSnFhyo10Data && (m_ShRevType>=ID_ICSSH_TKKZSIIRE_REVTYPE) ){
				memmove( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->Sn_1F1_1B, pArith, MONY_BUF_SIZE );
			}
		}
		else if( tag == _T("ACE00230") ){
			if( m_pSnFhyo10Data && (m_ShRevType>=ID_ICSSH_TKKZSIIRE_REVTYPE) ){
				memmove( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->Sn_1F5_1B, pArith, MONY_BUF_SIZE );
			}
		}
//<-- '15.11.14 INS END
	}
	//<----------

	return 0;
}

//-----------------------------------------------------------------------------
// 集計済み金額の取得	('14.06.30)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::GetCalqedMony()
{
	if( m_pSnHeadData ){
		if( m_pSnHeadData->IsReadCalqedData() == FALSE ){
			return 0;
		}
	}
	BOOL	isPluralZei = FALSE;

	if( m_pSnHeadData ){
		m_pSnHeadData->SetCalqedData( m_pSyzSyukei );
	}
	if( m_pSnTanaData ){
		m_pSnTanaData->SetCalqedData( m_pSyzSyukei );
		m_pSnTanaData->SetData( m_pZmSub, m_SnSeq );
	}

	if( m_pSnHonpyoData ){

		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );
//---------------
			((CH28SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );
//---------------
		}
		else{
			if(m_SixKindFlg == TRUE){
				((CH27SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );
			}
			else{
				m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
			}
		}
		//-------------------
		/*
		//[Y]150119
		if(m_SixKindFlg == TRUE){
			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );
		}
		else{
			m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
		}
		*/
		//<-------------------

//		m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );

		// 申告区分によるデータの再変更
		if( !(m_pSnHeadData->Sn_SKKBN%2) ){
			m_Arith.l_clear( m_pSnHonpyoData->Sn_TYNOFZ );
			m_Arith.l_clear( m_pSnHonpyoData->Sn_TTYWAR );
		}
		if( m_pSnHeadData->Sn_SKKBN <= 2 ){
			m_Arith.l_clear( m_pSnHonpyoData->Sn_KAKTIZ );
			m_Arith.l_clear( m_pSnHonpyoData->Sn_TKAKTZ );
		}

		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
//---------------
			((CH28SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
//---------------
		}
		else{
			if(m_SixKindFlg == TRUE){
				((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
			}
			else{
				m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
			}
		}

		//-------------------
		/*
		//------>[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
		}
		//<-------------------
		*/
		//<------------------
	}
	if( m_pSnFhyo10Data ){
		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->SetCalqedData( m_pSyzSyukei, isPluralZei );
//			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo10Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
			m_pSnFhyo10Data->SetData( m_pZmSub, m_SnSeq );
		}
		//-------------------
		/*
		m_pSnFhyo10Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
		m_pSnFhyo10Data->SetData( m_pZmSub, m_SnSeq );
		*/
		//<------------------
	}
	if( m_pSnFhyo20Data ){
		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo20Data*)m_pSnFhyo20Data)->SetCalqedData( m_pSyzSyukei, isPluralZei );
			((CH28SnFhyo20Data*)m_pSnFhyo20Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo20Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
			m_pSnFhyo20Data->SetData( m_pZmSub, m_SnSeq );
		}
		//--------------------
		/*
		m_pSnFhyo20Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
		m_pSnFhyo20Data->SetData( m_pZmSub, m_SnSeq );
		*/
		//<-------------------
	}
	if( m_pSnFhyo22Data ){
		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo20Data*)m_pSnFhyo22Data)->SetCalqedData( m_pSyzSyukei, isPluralZei );
			((CH28SnFhyo20Data*)m_pSnFhyo22Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo22Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
			m_pSnFhyo22Data->SetData( m_pZmSub, m_SnSeq );
		}
		//-------------------
		/*
		m_pSnFhyo22Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
		m_pSnFhyo22Data->SetData( m_pZmSub, m_SnSeq );
		*/
		//<------------------
	}
	if( m_pSnFhyo40Data ){
//		m_pSnFhyo40Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
		//------>[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo10Data*)m_pSnFhyo40Data)->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );//[Y]150119
			((CH27SnFhyo10Data*)m_pSnFhyo40Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo40Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );//[Y]150119
			m_pSnFhyo40Data->SetData( m_pZmSub, m_SnSeq );
		}
		//<-------------------
	}
	if( m_pSnFhyo50Data ){
//		m_pSnFhyo50Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
		//------>[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo50Data*)m_pSnFhyo50Data)->SetCalqedData( m_pSyzSyukei, isPluralZei );//[Y]150119
			((CH27SnFhyo50Data*)m_pSnFhyo50Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo50Data->SetCalqedData( m_pSyzSyukei, isPluralZei );//[Y]150119
			m_pSnFhyo50Data->SetData( m_pZmSub, m_SnSeq );
		}
		//<------------------
	}
	if( m_pSnFhyo52Data ){
//		m_pSnFhyo52Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
		//------>[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo50Data*)m_pSnFhyo52Data)->SetCalqedData( m_pSyzSyukei, isPluralZei );//[Y]150119
			((CH27SnFhyo50Data*)m_pSnFhyo52Data)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo52Data->SetCalqedData( m_pSyzSyukei, isPluralZei );//[Y]150119
			m_pSnFhyo52Data->SetData( m_pZmSub, m_SnSeq );
		}
		//<-------------------
	}

	if( m_pSnHonpyoData ){
		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );
//			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
//---------------
			((CH28SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );
			((CH28SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
//---------------
		}
		else{
			if(m_SixKindFlg == TRUE){
				((CH27SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );
				((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
			}
			else{
				m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
				m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
			}
		}
		//--------------------
		/*
		//------>[Y]150106
		if(m_SixKindFlg == TRUE){
			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetCalqedData( m_pZmSub, m_pSyzSyukei );//[Y]150119
			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );//[Y]150119
			m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
		}
		//<-------------------
		*/
		//<--------------------
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 金額の再取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::ReGetCalqedMony()
{
//	if( ((Snh.Sn_SKKBN) == 0x01 || ((Snh.Sn_SKKBN) == 0x02 && m_Middle == 0 ) || (Snh.Sn_SKKBN) == 0x03 || (Snh.Sn_SKKBN) == 0x04 ) && (Snd.Sn_Sign4&0x81) == 0x00 ){
//	}
	if( m_pSnHeadData->IsReadCalqedData() == FALSE ){
		return 0;
	}

/*- '14.06.30 -*/
//	BOOL	isPluralZei = FALSE;
//
//	if( m_pSnHeadData ){
//		m_pSnHeadData->SetCalqedData( m_pSyzSyukei );
//	}
//	if( m_pSnTanaData ){
//		m_pSnTanaData->SetCalqedData( m_pSyzSyukei );
//		m_pSnTanaData->SetData( m_pZmSub, m_SnSeq );
//	}
//
//	if( m_pSnHonpyoData ){
//		m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
//		// 申告区分によるデータの再変更
//		if( !(m_pSnHeadData->Sn_SKKBN%2) ){
//			m_Arith.l_clear( m_pSnHonpyoData->Sn_TYNOFZ );
//			m_Arith.l_clear( m_pSnHonpyoData->Sn_TTYWAR );
//		}
//		if( m_pSnHeadData->Sn_SKKBN <= 2 ){
//			m_Arith.l_clear( m_pSnHonpyoData->Sn_KAKTIZ );
//			m_Arith.l_clear( m_pSnHonpyoData->Sn_TKAKTZ );
//		}
//
//		m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
//	}
//	if( m_pSnFhyo10Data ){
//		m_pSnFhyo10Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
//		m_pSnFhyo10Data->SetData( m_pZmSub, m_SnSeq );
//	}
//	if( m_pSnFhyo20Data ){
//		m_pSnFhyo20Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
//		m_pSnFhyo20Data->SetData( m_pZmSub, m_SnSeq );
//	}
//	if( m_pSnFhyo22Data ){
//		m_pSnFhyo22Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
//		m_pSnFhyo22Data->SetData( m_pZmSub, m_SnSeq );
//	}
//	if( m_pSnFhyo40Data ){
//		m_pSnFhyo40Data->SetCalqedData( m_pZmSub, m_pSyzSyukei, isPluralZei );
//		m_pSnFhyo40Data->SetData( m_pZmSub, m_SnSeq );
//	}
//	if( m_pSnFhyo50Data ){
//		m_pSnFhyo50Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
//		m_pSnFhyo50Data->SetData( m_pZmSub, m_SnSeq );
//	}
//	if( m_pSnFhyo52Data ){
//		m_pSnFhyo52Data->SetCalqedData( m_pSyzSyukei, isPluralZei );
//		m_pSnFhyo52Data->SetData( m_pZmSub, m_SnSeq );
//	}
//
//	if( m_pSnHonpyoData ){
//		m_pSnHonpyoData->SetCalqedData( m_pZmSub, m_pSyzSyukei );
//		m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
//	}
//
//	if( isPluralZei ){
////		m_pSnHeadData->SVmzsw |= 0x01;
//	}
/*-------------*/
	GetCalqedMony();
/*-------------*/

	// 表示更新
	if( m_pFhyo40 ){
		m_pFhyo40->Init();
	}
	if( m_pFhyo50 ){
		m_pFhyo50->Init();
	}
	if( m_pFhyo52 ){
		m_pFhyo52->Init();
	}
	if( m_pFhyo522 ){
		m_pFhyo522->Init();
	}
	if( m_pKani ){
		m_pKani->Init();
	}

//--> '15.02.18 INS START
	if( m_pFhyo40EX ){
		m_pFhyo40EX->Init();
	}
	if( m_pFhyo50EX ){
		m_pFhyo50EX->Init();
	}
	if( m_pFhyo52EX ){
		m_pFhyo52EX->Init();
	}
	if( m_pFhyo522EX ){
		m_pFhyo522EX->Init();
	}
	if( m_pKaniEX ){
		m_pKaniEX->Init();
	}
//<-- '15.02.18 INS END

	//---->yoshida150810
	if( m_pKaniEX2 ){
		m_pKaniEX2->Init();
	}
	//<-----------------

	if( m_pFhyo10 ){
		m_pFhyo10->Init();
	}
	if( m_pFhyo20 ){
		m_pFhyo20->Init();
	}
	if( m_pFhyo22 ){
		m_pFhyo22->Init();
	}
	if( m_pIppan ){
		m_pIppan->Init();
	}

	//---->yoshida150810
	if( m_pFhyo10EX ){
		m_pFhyo10EX->Init();
	}
	if( m_pFhyo20EX ){
		m_pFhyo20EX->Init();
	}
	if( m_pFhyo22EX ){
		m_pFhyo22EX->Init();
	}
	if( m_pIppanEX ){
		m_pIppanEX->Init();
	}
	if( m_pTKUtiwake ){
		m_pTKUtiwake->Init();
	}
	//<-----------------


//--> '14.04.25 INS START
	// 還付
	if( m_pRefundKjn01 ){
		m_pRefundKjn01->ReUpdateData();
	}
	if( m_pRefundKjn02 ){
		m_pRefundKjn02->ReUpdateData();
	}
	if( m_pRefundHjn01 ){
		m_pRefundHjn01->ReUpdateData();
	}
	if( m_pRefundHjn02 ){
		m_pRefundHjn02->ReUpdateData();
	}
//<-- '14.04.25 INS END

//2018.03.13 UPDATE START
	if( m_pFhyo60 ){
		m_pFhyo60->Init();
	}
//2018.03.13 UPDATE END
	return 0;
}

void CH26HyoView::OnZeirishiInf()
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_pShinInfo && m_pShinInfo->pParent ){
		int ret = (int)m_pShinInfo->pParent->SendMessage( WM_USER_READ_ZEIRISHI, 0 );
		if( ret == 2 ){//２はキャンセル
			return;
		}
//2016.10.20 UPDATE START
//		int selno = m_MainTab.GetCurSel();
//		if( m_TabMng[selno].tabtype==ID_ICSSH_KANI_TAB ){
//			if( m_pKani ){
//				m_pKani->Init();
//			}
////--> '15.02.23 INS START
//			else if( m_pKaniEX ){
//				m_pKaniEX->Init();
//			}
////<-- '15.02.23 INS END
//			//-------->yoshida150810
//			else if( m_pKaniEX2 ){
//				m_pKaniEX2->Init();
//			}
//			//<---------------------
//
//		}
//		else if( m_TabMng[selno].tabtype==ID_ICSSH_IPPAN_TAB ){
//			if( m_pIppan ){
//				m_pIppan->Init();
//			}
//			//-------->yoshida150810
//			else if( m_pIppanEX ){
//				m_pIppanEX->Init();
//			}
//			//<--------------------
//		}
//		else if( m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB ){
//			if( m_pHjnKsk ){
//				m_pHjnKsk->DispKsiHjn();
//			}
//			//-------->yoshida150810
//			else if( m_pHjnKskEX ){
//				m_pHjnKskEX->Init();
//			}
//			//<--------------------
//		}
//		else if( m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB ){
//			if( m_pKjnKsk ){
//				m_pKjnKsk->DispKsiKjn();
//			}
//			//-------->yoshida150810
//			else if( m_pKjnKskEX ){
//				m_pKjnKskEX->Init();
//			}
//			//<--------------------
//		}
//		else if( m_TabMng[selno].tabtype==ID_ICSSH_CHSK_TAB ){
//			if( m_pChsk ){
//				m_pChsk->Init();
//			}
//			//-------->yoshida150810
//			else if( m_pChskEX ){
//				m_pChskEX->Init();
//			}
//			//<--------------------
//		}

		ReDispHonpyoUpper();

	}
//2016.10.20 UPDATE END
}

void CH26HyoView::OnUpdateZeirishiInf(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL	bEnable = FALSE;
	int selno = m_MainTab.GetCurSel();

	if( (selno!=-1) && (selno<(int)m_TabMng.GetCount()) ){
		if( (m_TabMng[selno].tabtype==ID_ICSSH_KANI_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_IPPAN_TAB) ||
			(m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) ||
			(m_TabMng[selno].tabtype==ID_ICSSH_CHSK_TAB) ){

			if( !(m_pSnHeadData->Sn_Sign4&0x01) ){	// 入力ロック時はDisable harino ('14.03.12)
				if( m_SnSeq == 0 ){
					bEnable = TRUE;
				}
			}
		}
	}

	if( pCmdUI ){
		pCmdUI->Enable( bEnable );
	}
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
//-- '15.09.30 --
//int CH26HyoView::ConnectedSkj()
//---------------
int CH26HyoView::ConnectedSkj( MoneyBasejagArray &MoneyBaseArray, bool isUpdDisp )
//---------------
{
	if( m_pSnRefundData ){
		m_pSnRefundData->GetData( m_pZmSub, m_pSnHeadData, m_SnSeq );
//-- '15.08.27 --
//		m_pSnRefundData->ConnectZmdata( m_pZmSub, m_pSnHeadData, m_pSyzSyukei, m_pTblhdl, m_pSkjCnvtbl );
//---------------
		m_pSnRefundData->ConnectZmdata( m_pZmSub, m_pSnHeadData, m_pSyzSyukei, m_pTblhdl, m_pSkjCnvtbl, m_ShRevType, MoneyBaseArray );
//---------------
		m_pSnRefundData->SetData( m_pZmSub, m_pSnHeadData, m_SnSeq );
	}

	return 0;
}

//2016.02.23 INSERT START
//-----------------------------------------------------------------------------
// 仕入金額等の明細のクリア
//-----------------------------------------------------------------------------
// 引数			：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::ClearShiire()
{
	if( m_pSnRefundData ){
		m_pSnRefundData->ClearData_Shiire( m_pZmSub, m_pSnHeadData, m_SnSeq );
	}
	return 0;
}
//2016.02.23 INSERT END

//----------------------------------------------------------------------------
// 更正の請求用　申告書金額計算
//----------------------------------------------------------------------------
// 引数	mode	：
//----------------------------------------------------------------------------
// 返送値
//----------------------------------------------------------------------------
int CH26HyoView::CalqSyzForKsk( int mode )
{
	char	val[MONY_BUF_SIZE]={0};

//--> '15.03.16 INS START
	if( ReadDataForCalq() ){
		return -1;
	}
//<-- '15.03.16 INS END

	SH_ANS	ANS[30] = {0};
	SH_ANS	JANS[10] = {0};
	CalqSyzForKskSub( (void*)ANS, (void*)JANS, val );

	if( !m_pSnKskData ){
		m_pSnKskData = new CH26SnKskData();
		if( m_pSnKskData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}
	m_pSnKskData->SetCalqedData( mode, ANS );

	return 0;
}

//-----------------------------------------------------------------------------
// 更正の請求用　申告書金額計算（実部）
//-----------------------------------------------------------------------------
// 引数	pCalqedMony1	：	集計後金額の保存先１
//		pCalqedMony2	：	集計後金額の保存先２
//		Zei				：	税額
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//-----------------------------------------------------------------------------
int CH26HyoView::CalqSyzForKskSub_H25( void *pCalqedMony1, void *pCalqedMony2, char *Zei )
{
	// 申告書金額の保存先
	ASSERT( pCalqedMony1 );
	ASSERT( pCalqedMony2 );
	SH_ANS	*ANS = (SH_ANS*)pCalqedMony1;
	SH_ANS	*JANS = (SH_ANS*)pCalqedMony2;
	
	// 計算結果バッファ
	char 	a[6], b[6], c[6];
	char	vers, N_v;
	m_Arith.l_defn( 0x16 );
	
	// バージョンチェック
	vers = N_v = 0x02;
	if( m_pSnHeadData->SVmzsw ){
		vers = 1;
	}
	// 初期設定
	memset( Zei, '\0', 6 );
	// 中間申告２６様式チェック
	if( !((m_pSnHeadData->Sn_SKKBN&0xff) % 2) ){
		int cnt = 0;
		if( m_pSnHeadData->Sn_MCOUNT >= 3 ){
			cnt = m_pSnHeadData->Sn_JCOUNT - 1;
		}
		if( (m_pSnHeadData->Sn_SKKBN&0xff) == 0x02 )	{	// 中間
//			if(m_pSnHeadData->SVtmth[ cnt ] & 0x01) {
//				memmove( Zei, m_pSnChskData->Sn_KTNFZ, 6 );
//				return;
//			}
		}
	}
	// 通常申告
//	memset( &ANS, '\0', sizeof( struct _ANS )*30 );
//	memset( &JANS, '\0', sizeof( struct _ANS )*10 );
	// 課税標準額
	// 普通乗用自動車 ６％分
	memmove( a, m_pSnHeadData->Sn_CK60, 6 );
	m_Util.l_calq( (char *)JANS[0].val, (char *)a, 0 );						// １０００円未満　切り捨て
	
	/* 普通乗用自動車 ４．５％分 */
	memmove( a, m_pSnHeadData->Sn_CK45, 6 );
	m_Util.l_calq( (char *)JANS[1].val, (char *)a, 0 );						// １０００円未満　切り捨て
	
	// 消費税額
	// 普通乗用自動車
	// 6%
	memmove( JANS[2].val, m_pSnHeadData->Sn_CZ60, 6 );
	
	// 4.5%
	memmove( JANS[3].val, m_pSnHeadData->Sn_CZ45, 6 );
	
	CH26SnFhyo10Data	*pSnFhyo10Data = NULL;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		pSnFhyo10Data = m_pSnFhyo10Data;
	}
	else{
		pSnFhyo10Data = m_pSnFhyo40Data;
	}

	switch( vers&0x0f ) {
		case 1 :	// 経過措置対象
			// 課税標準額		
			memmove( a, pSnFhyo10Data->Sn_1F1B, 6 );						// ４％分
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );				// １０００円未満　切り捨て
			
			memmove( a, pSnFhyo10Data->Sn_1F1A, 6 );						// ３％分
			m_Util.l_calq( (char *)JANS[5].val, (char *)a, 0 );				// １０００円未満　切り捨て
			
			m_Arith.l_add( ANS[0].val, JANS[4].val, JANS[5].val );
			
			// 消費税額
			memmove( JANS[6].val, pSnFhyo10Data->Sn_1F2B, 6 );					// ４％分
			
			memmove( JANS[7].val, pSnFhyo10Data->Sn_1F2A, 6 );					// ３％分
			
			m_Arith.l_add( ANS[1].val, JANS[6].val, JANS[7].val );
			break;

		case 2 :	// 経過措置対象外
			// 課税標準額
			memmove( a, m_pSnHonpyoData->Sn_KSTD, 6 );
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );				// １０００円未満　切り捨て
			
			// 消費税額
			memmove( JANS[6].val, m_pSnHonpyoData->Sn_SYTX, 6 );
			
			memmove( ANS[0].val, JANS[4].val, 6 );
			memmove( ANS[1].val, JANS[6].val, 6 );
			break;
		default:	// 旧バージョンマスター
			// 普通乗用自動車
/*			memmove( a, Snd->Sn_KSTDM, 6 );
			m_Util.l_calq( (char *)ANS[0].val, (char *)a, 0 );					// １０００円未満　切り捨て
			
			// その他の課税分
			memmove( a, Snd->Sn_KSTD, 6 );
			m_Util.l_calq( (char *)ANS[1].val, (char *)a, 0 );					// １０００円未満　切り捨て
			
			// 計 (1 + 2)
			m_Arith.l_add( ANS[2].val, ANS[0].val, ANS[1].val );
			
			// 消費税額
			memmove( ANS[3].val, Snd->Sn_SYTXM, 6 );
			
			// その他の課税分
			memmove( ANS[4].val, Snd->Sn_SYTX, 6 );
			
			// 計 (4 + 5)
			m_Arith.l_add( ANS[5].val, ANS[3].val, ANS[4].val );*/
			break;
	}
	
	// 簡易 -> 貸倒回収に係る or 税額一般 -> 控除過大調整税額
	if( vers ){
		memmove( ANS[2].val, m_pSnHonpyoData->Sn_KJADJZ, 6 );
	}
	else {
		memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
		memmove( c, m_pSnTanaData->Sn_MENZZ, 6 );
		if( m_pSnTanaData->Sn_MENZsw & 0x01 ){
			if( m_Arith.l_test( c ) ){
				m_Arith.l_neg( c );
			}
		}
		m_Arith.l_add( a, a, c );
		memmove( b, m_pSnHonpyoData->Sn_KJADJZ, 6 );
		if( m_Arith.l_test( a ) < 0 ) {
				m_Arith.l_neg( a );
				m_Arith.l_add( ANS[6].val, b, a );
		}
		else{
			memmove( ANS[6].val, b, 6 );
		}
	}
	
	// 控除税額
	// 控除対象仕入税額
	memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
	if( ! vers ){
		m_Arith.l_add( a, a, c );
	}
	if( m_Arith.l_test( a ) < 0 ){
		m_Arith.l_clear( (vers ? ANS[3].val : ANS[7].val) );
	}
	else{
		memmove( (vers ? ANS[3].val : ANS[7].val), a, 6 );
	}
	
	// 返還等対価の税額
	memmove( (vers ? ANS[4].val : ANS[8].val), m_pSnHonpyoData->Sn_HKANZ, 6 );
	
	// 貸倒れに係る税額
	memmove( (vers ? ANS[5].val : ANS[9].val), m_pSnHonpyoData->Sn_KSITZ, 6 );
	
	// 控除税額小計 vers ? (4 + 5 + 6) : (8 + 9 + 10)
	if( vers ) {
		m_Arith.l_add( ANS[6].val, ANS[3].val, ANS[4].val );
		m_Arith.l_add( ANS[6].val, ANS[6].val, ANS[5].val );
	}
	else {
		m_Arith.l_add( ANS[10].val, ANS[7].val,  ANS[8].val );
		m_Arith.l_add( ANS[10].val, ANS[10].val, ANS[9].val );
	}
	
	// 限界控除税額
	memmove( (vers ? JANS[8].val : ANS[13].val), m_pSnHonpyoData->Sn_GENKAI, 6 );
	
	switch( vers&0x0f ) {
	// 控除不足還付税額 (7 - 2 - 3) ･ 差引税額 (2 + 3 - 7)
		case 1 :	if( m_Arith.l_test( pSnFhyo10Data->Sn_1F10T ) < 0 ) {
						if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ) {					// 中間申告以外
							memmove( ANS[7].val, pSnFhyo10Data->Sn_1F10T, 6 );
							m_Arith.l_neg( ANS[7].val );
						}
					}
					else {
						if( m_Arith.l_test( m_pSnHonpyoData->Sn_GENKAI ) ){
//							memmove( a, Snd->Sn_3F3,   6 );
						}
						else{
							memmove( a, pSnFhyo10Data->Sn_1F10T, 6 );
						}
						m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );		//1 00円未満切り捨て
					}
					break;
		case 2 :	m_Arith.l_add( a, ANS[1].val, ANS[2].val );
					m_Arith.l_sub( a, a, ANS[6].val );
					// 限界控除前の税額がマイナスの時
					if( m_Arith.l_test( a ) < 0 ) {
						if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ) {					// 中間申告以外
							m_Arith.l_neg( a );
							memmove( ANS[7].val, a, 6 );
						}
					}
					else{
						m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );	// 100円未満切り捨て
					}
					break;
	
				// 限界控除前の税額 (6 + 7 - 11)
		default:
/*			m_Arith.l_add( a, ANS[5].val, ANS[6].val );
			m_Arith.l_sub( a, a, ANS[10].val );
			if( m_Arith.l_test( a ) < 0 ){
				m_Arith.l_clear( ANS[11].val );
			}
			else{
				memmove( ANS[11].val, a, 6 );
			}
			
		// 控除不足還付税額 (11 - 6 - 7)
			m_Arith.l_sub( a, ANS[10].val, ANS[5].val );
			m_Arith.l_sub( a, a, ANS[6].val );
			// 中間申告以外
			if( (m_Arith.l_test( a ) < 0 ) || ! (Snh->Sn_SKKBN % 2) )
					m_Arith.l_clear( ANS[12].val );
			else	memmove( ANS[12].val, a, 6 );
			
		// 差引税額 (12 - 14)
			if( ! m_Arith.l_test( ANS[12].val ) )
					m_Arith.l_sub( ANS[14].val, ANS[11].val, ANS[13].val );
			else	m_Arith.l_clear( ANS[14].val );
			
			// １００円未満　切り捨て
			if( m_Arith.l_test( ANS[14].val ) )
					p_calq( (char *)ANS[14].val, (char *)ANS[14].val, 0x10 );*/
			break;
	}
	
	// 中間納付税額
	memmove( a, m_pSnHonpyoData->Sn_TYNOFZ, 6 );
	m_Util.l_calq( (char *)(vers ? ANS[9].val : ANS[15].val), (char *)a, 0x10 );	// 100円未満切り捨て
	
	// 納付税額 (15 - 16) ･ 中間納付還付税額
	if( vers ) {
		m_Arith.l_sub( a, ANS[8].val, ANS[9].val );
		m_Util.l_calq( (char *)a, (char *)a, 0x10 );								// 100円未満切り捨て
		if( m_Arith.l_test( a ) < 0 ) {
			m_Arith.l_neg( a );
			memmove( ANS[11].val, a, 6 );
		}
		else{
			memmove( ANS[10].val, a, 6 );
		}
	}
	else {
		if( ! m_Arith.l_test( ANS[12].val ) ) {
			m_Arith.l_sub( a, ANS[14].val, ANS[15].val );
			if( l_test( a ) < 0 ){
				m_Arith.l_clear( ANS[16].val );
			}
			else{
				m_Util.l_calq( (char *)ANS[16].val, (char *)a, 0x10 );			// 100円未満　切り捨て
			}
		}
		else{
			m_Arith.l_clear( ANS[16].val );
		}
		
		if( m_Arith.l_test( ANS[12].val ) ){
			memmove( ANS[17].val, ANS[15].val, 6 );
		}
		else {
			m_Arith.l_sub( a, ANS[15].val, ANS[14].val );
			if( m_Arith.l_test( a ) < 0 ){
				m_Arith.l_clear( ANS[17].val );
			}
			else{
				memmove( ANS[17].val, a, 6 );
			}
		}
	}
	
	memmove( m_pSnHonpyoData->Sn_EDNOFZ, (vers ? ANS[10].val : ANS[16].val), 6 );
	
	// 修正申告
	if( m_pSnHeadData->Sn_SKKBN == 3 || m_pSnHeadData->Sn_SKKBN == 4 ) {
		// 既確定税額
		memmove( (vers ? ANS[12].val : ANS[18].val), m_pSnHonpyoData->Sn_KAKTIZ, 6 );
		
		if( vers ) {
			if( m_Arith.l_test( ANS[10].val ) > 0 )		// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[10].val, ANS[12].val );
			else {
				memmove( a, ANS[11].val, 6 );
				m_Arith.l_neg( a );
				m_Arith.l_sub( a, a, ANS[12].val );
			}
			m_Arith.l_sub( a, a, ANS[7].val );
		}
		else {
			// 差引納付税額 (17 - 19)
			if( m_Arith.l_test( ANS[12].val ) ) {
				m_Arith.l_add( a, ANS[12].val, ANS[17].val );
				m_Arith.l_add( a, a, ANS[18].val );
				m_Arith.l_neg( a );
			}
			else {
				if( m_Arith.l_test( ANS[16].val ) )
					m_Arith.l_sub( a, ANS[16].val, ANS[18].val );
				else {
					m_Arith.l_add( a, ANS[17].val, ANS[18].val );
					m_Arith.l_neg( a );
				}
			}
		}
		// １００円未満　切り捨て
		m_Util.l_calq( (char *)(vers ? ANS[13].val : ANS[19].val), (char *)a, 0x10 );
	}
#ifdef _CUT_SHOHI_	
	// 課税資産の譲渡等の対価の額
	memmove( (vers ? ANS[14].val : ANS[20].val), Snd->Sn_KZURI, 6 );
	if( Kani > 1 )	 // 基準期間の課税売上高
		memmove( (vers ? ANS[15].val : ANS[21].val), Snd->Sn_ZZURI, 6 );
	else			 // 資産の譲渡等の対価の額
		memmove( (vers ? ANS[15].val : ANS[21].val), Snd->Sn_SOURI, 6 );
#endif	
	if( ! vers )	return 0;
	
	// 地方消費税の計算
	// 控除不足還付税額
	if( ! ((m_pSnHeadData->Sn_SKKBN&0xff) % 2) && N_v != 2 ) ;	// 中間且改正日前開始
	else {
		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ) {	// 中間申告以外
			if( vers == 1 ){
				memmove( ANS[16].val, pSnFhyo10Data->Sn_1F8B, 6 );
			}
			else{
				memmove( ANS[16].val, ANS[7].val, 	6 );
			}
		}
		
		// 差引税額
		if( m_Arith.l_test( pSnFhyo10Data->Sn_1F10T ) < 0 && ! ((m_pSnHeadData->Sn_SKKBN&0xff) % 2) ) ;
		else {		// 還付　中間申告以外
			if( vers == 1 ) {
				if( m_Arith.l_test( m_pSnHonpyoData->Sn_GENKAI ) ){
//					memmove( a, Snd->Sn_3F6,   6 );
				}
				else{
					memmove( a, pSnFhyo10Data->Sn_1F9B, 6 );
				}
			}
			else{
				memmove( a, ANS[8].val, 6 );
			}
			m_Util.l_calq( (char *)ANS[17].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
		
		// 譲渡割額還付額 ･ 譲渡割額納税額
		m_Arith.l_input( b, "250" );
		if( m_Arith.l_test( ANS[16].val ) )
			m_Arith.l_mul100( (unsigned char *)ANS[18].val, (unsigned char *)ANS[16].val, (unsigned char *)b, 1, 0 );
		if( m_Arith.l_test( ANS[17].val ) ) {
			m_Arith.l_mul100( (unsigned char *)a, (unsigned char *)ANS[17].val, (unsigned char *)b, 1, 0 );
			m_Util.l_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
		
		// 中間納付譲渡割額
		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 )	// 中間申告以外
			m_Util.l_calq( (char *)ANS[20].val, (char *)m_pSnHonpyoData->Sn_TTYWAR, 0x10 );	// 100円未満切り捨て
		
		// 納付譲渡割額 (20 - 21) ･ 中間還付譲渡割額 (21 - 20)
		m_Arith.l_sub( a, ANS[19].val, ANS[20].val );
		if ( m_Arith.l_test( a ) < 0 ) {
			m_Arith.l_neg( a );
			m_Util.l_calq( (char *)ANS[22].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
		else{
			m_Util.l_calq( (char *)ANS[21].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
		
	// 修正申告
		if( m_pSnHeadData->Sn_SKKBN == 3 || m_pSnHeadData->Sn_SKKBN == 4 ) {
			// 既確定税額
			memmove( ANS[23].val, m_pSnHonpyoData->Sn_TKAKTZ, 6 );
			
			// 差引納付譲渡割額
			if( m_Arith.l_test( ANS[21].val ) > 0 )	// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[21].val, ANS[23].val );
			else {
				m_Arith.l_add( a, ANS[18].val, ANS[22].val );
				if( m_Arith.l_test( a ) ){
					m_Arith.l_neg( a );
				}
				m_Arith.l_sub( a, a, ANS[23].val );
			}
			m_Util.l_calq( (char *)ANS[24].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
	}
	
	// 消費税及び地方消費税の合計（納付又は還付）税額
	switch( m_pSnHeadData->Sn_SKKBN&0xff ) {
		case 1 : case 2 :		// 確定・中間申告
			m_Arith.l_add( a, ANS[10].val, ANS[21].val );
			m_Arith.l_add( b, ANS[7].val,  ANS[11].val );
			m_Arith.l_add( b, b, ANS[18].val );
			m_Arith.l_add( b, b, ANS[22].val );
			m_Arith.l_sub( ANS[25].val, a, b );
			break;
		default	:				// 修正申告
			m_Arith.l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}
	memmove( Zei, ANS[25].val, 6 );

	return 0;
}

//-----------------------------------------------------------------------------
// 更正の請求用　申告書金額計算（実部）
//-----------------------------------------------------------------------------
// 引数	pCalqedMony1	：	集計後金額の保存先１
//		pCalqedMony2	：	集計後金額の保存先２
//		Zei				：	税額
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//-----------------------------------------------------------------------------
int CH26HyoView::CalqSyzForKskSub( void *pCalqedMony1, void *pCalqedMony2, char *Zei )
{
	// 疑問？？
	//  この時点で集計して再取得する事に意味はあるのか？

	// 計算結果バッファ
	char 	a[MONY_BUF_SIZE]={0}, b[MONY_BUF_SIZE]={0}, c[MONY_BUF_SIZE]={0}, d[MONY_BUF_SIZE]={0};
	char	vers, N_v;

	// 申告書金額の保存先
	ASSERT( pCalqedMony1 );
	ASSERT( pCalqedMony2 );
	SH_ANS	*ANS = (SH_ANS*)pCalqedMony1;
	SH_ANS	*JANS = (SH_ANS*)pCalqedMony2;
	
	// バージョンチェック
	vers = N_v = 0x02;
	if( m_pSnHeadData->SVmzsw ){
		vers = 1;
	}

//--> '14.09.09 INS START
	BOOL	isH25Syz = FALSE;
	if( m_pSnHeadData->Sn_KDAYE < 20140401 ){
		return CalqSyzForKskSub_H25( pCalqedMony1, pCalqedMony2, Zei );
	}
//<-- '14.09.09 INS END

	// 初期設定
	memset( Zei, '\0', 6 );
	// 中間申告２６様式チェック
	if( !((m_pSnHeadData->Sn_SKKBN&0xff) % 2) ){
		int cnt = 0;
		if( m_pSnHeadData->Sn_MCOUNT >= 3 ){
			cnt = m_pSnHeadData->Sn_JCOUNT - 1;
		}
		if( (m_pSnHeadData->Sn_SKKBN&0xff) == 0x02 ){	// 中間
//			if( m_pSnHeadData->SVtmth[ cnt ] & 0x01 ){
//				memmove( Zei, m_pSnChskData->Sn_KTNFZ, 6 );
//				return 0;
//			}
		}
	}

	// 通常申告
	memset( ANS, '\0', sizeof(SH_ANS)*30 );
	memset( JANS, '\0', sizeof(SH_ANS)*10 );

	// 課税標準額
	// 普通乗用自動車 ６％分
	memmove( a, m_pSnHeadData->Sn_CK60, 6 );
//	p_calq( (char *)JANS[0].val, (char *)a, 0 );						// １０００円未満　切り捨て
	m_Util.l_calq( (char *)JANS[0].val, (char *)a, 0 );						// １０００円未満　切り捨て
	
	// 普通乗用自動車 ４．５％分
	memmove( a, m_pSnHeadData->Sn_CK45, 6 );
//	p_calq( (char *)JANS[1].val, (char *)a, 0 );						// １０００円未満　切り捨て
	m_Util.l_calq( (char *)JANS[1].val, (char *)a, 0 );						// １０００円未満　切り捨て
	
	// 消費税額
	// 普通乗用自動車
	// 6%
	memmove( JANS[2].val, m_pSnHeadData->Sn_CZ60, 6 );
	
	// 4.5%
	memmove( JANS[3].val, m_pSnHeadData->Sn_CZ45, 6 );

	CH26SnFhyo10Data	*pSnFhyo10Data = NULL;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		pSnFhyo10Data = m_pSnFhyo10Data;
	}
	else{
		pSnFhyo10Data = m_pSnFhyo40Data;
	}
	
	switch( vers&0x0f ){
		case 1 :	// 経過措置対象
			// 課税標準額		

			// ４％分
			memmove( a, pSnFhyo10Data->Sn_1F1B, 6 );
//			p_calq( (char *)JANS[4].val, (char *)a, 0 );				// １０００円未満　切り捨て
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );			// １０００円未満　切り捨て

			// ３％分
			memmove( a, pSnFhyo10Data->Sn_1F1A, 6 );
//			p_calq( (char *)JANS[5].val, (char *)a, 0 );				// １０００円未満　切り捨て
			m_Util.l_calq( (char *)JANS[5].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// ６．３％分
			memmove( a, pSnFhyo10Data->Sn_1F1C, 6 );				
			m_Util.l_calq( (char *)JANS[6].val, (char *)a, 0 );			// １０００円未満　切り捨て

			m_Arith.l_add( ANS[0].val, JANS[4].val, JANS[5].val );
			m_Arith.l_add( ANS[0].val, ANS[0].val, JANS[6].val );
			
			// 消費税額
			memmove( JANS[7].val, pSnFhyo10Data->Sn_1F2A, 6 );		// ３％分
			memmove( JANS[8].val, pSnFhyo10Data->Sn_1F2B, 6 );		// ４％分
			memmove( JANS[9].val, pSnFhyo10Data->Sn_1F2C, 6 );		// ６．３％分
			
			m_Arith.l_add( ANS[1].val, JANS[7].val, JANS[8].val );
			m_Arith.l_add( ANS[1].val, ANS[1].val, JANS[9].val );

			break;

		case 2 :	// 経過措置対象外
			// 課税標準額
			memmove( a, m_pSnHonpyoData->Sn_KSTD, 6 );
//			p_calq( (char *)JANS[4].val, (char *)a, 0 );				// １０００円未満　切り捨て
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// 消費税額
			memmove( JANS[6].val, m_pSnHonpyoData->Sn_SYTX, 6 );
			
			memmove( ANS[0].val, JANS[4].val, 6 );
			memmove( ANS[1].val, JANS[6].val, 6 );
			break;

		default:	// 旧バージョンマスター
/*			// 普通乗用自動車
			memmove( a, Snd->Sn_KSTDM, 6 );
			p_calq( (char *)ANS[0].val, (char *)a, 0 );					// １０００円未満　切り捨て
			
			// その他の課税分
			memmove( a, Snd->Sn_KSTD, 6 );
			p_calq( (char *)ANS[1].val, (char *)a, 0 );					// １０００円未満　切り捨て
			
			// 計 (1 + 2)
			l_add( ANS[2].val, ANS[0].val, ANS[1].val );
			
			// 消費税額
			memmove( ANS[3].val, Snd->Sn_SYTXM, 6 );
			
			// その他の課税分
			memmove( ANS[4].val, Snd->Sn_SYTX, 6 );
			
			// 計 (4 + 5)
			l_add( ANS[5].val, ANS[3].val, ANS[4].val );*/
			break;
	}
	
	// 簡易 -> 貸倒回収に係る or 税額一般 -> 控除過大調整税額
	if( vers ){
		memmove( ANS[2].val, m_pSnHonpyoData->Sn_KJADJZ, 6 );
	}
	else{
		memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
		memmove( c, m_pSnTanaData->Sn_MENZZ, 6 );
		if( m_pSnTanaData->Sn_MENZsw & 0x01 ){
			if( m_Arith.l_test( c ) ){
				m_Arith.l_neg( c );
			}
		}
		m_Arith.l_add( a, a, c );
		memmove( b, m_pSnHonpyoData->Sn_KJADJZ, 6 );
		if( m_Arith.l_test( a ) < 0 ) {
			m_Arith.l_neg( a );
			m_Arith.l_add( ANS[6].val, b, a );
		}
		else{
			memmove( ANS[6].val, b, 6 );
		}
	}
	
	// 控除税額
	// 控除対象仕入税額
	memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
	if( !vers ){
		m_Arith.l_add( a, a, c );
	}
	if( m_Arith.l_test( a ) < 0 ){
		m_Arith.l_clear( (vers ? ANS[3].val : ANS[7].val) );
	}
	else{
		memmove( (vers ? ANS[3].val : ANS[7].val), a, 6 );
	}
	
	// 返還等対価の税額
	memmove( (vers ? ANS[4].val : ANS[8].val), m_pSnHonpyoData->Sn_HKANZ, 6 );
	
	// 貸倒れに係る税額
	memmove( (vers ? ANS[5].val : ANS[9].val), m_pSnHonpyoData->Sn_KSITZ, 6 );
	
	// 控除税額小計 vers ? (4 + 5 + 6) : (8 + 9 + 10)
	if( vers ) {
		m_Arith.l_add( ANS[6].val, ANS[3].val, ANS[4].val );
		m_Arith.l_add( ANS[6].val, ANS[6].val, ANS[5].val );
	}
	else{
		m_Arith.l_add( ANS[10].val, ANS[7].val,  ANS[8].val );
		m_Arith.l_add( ANS[10].val, ANS[10].val, ANS[9].val );
	}
	
	// 限界控除税額
	memmove( (vers ? JANS[8].val : ANS[13].val), m_pSnHonpyoData->Sn_GENKAI, 6 );
	
	switch( vers&0x0f ){
		// 控除不足還付税額 (7 - 2 - 3) ･ 差引税額 (2 + 3 - 7)
		case 1 :
			if( m_Arith.l_test(pSnFhyo10Data->Sn_1F10T) < 0 ){
				if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){					// 中間申告以外
					memmove( ANS[7].val, pSnFhyo10Data->Sn_1F10T, 6 );
					m_Arith.l_neg( ANS[7].val );
				}
			}
			else{
				if( m_Arith.l_test(m_pSnHonpyoData->Sn_GENKAI) ){
//					memmove( a, Snd->Sn_3F3,   6 );
				}
				else{
					memmove( a, pSnFhyo10Data->Sn_1F10T, 6 );
				}
//				p_calq( (char *)ANS[8].val, (char *)a, 0x10 );		//1 00円未満切り捨て
				m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );		//1 00円未満切り捨て
			}
			break;
		case 2 :
			m_Arith.l_add( a, ANS[1].val, ANS[2].val );
			m_Arith.l_sub( a, a, ANS[6].val );
			// 限界控除前の税額がマイナスの時
			if( m_Arith.l_test(a) < 0 ){
				if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){					// 中間申告以外
					m_Arith.l_neg( a );
					memmove( ANS[7].val, a, 6 );
				}
			}
			else{
//				p_calq( (char *)ANS[8].val, (char *)a, 0x10 );	// 100円未満切り捨て
				m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );	// 100円未満切り捨て
			}
			break;
	
		default:	
			// 限界控除前の税額 (6 + 7 - 11)
/*			l_add( a, ANS[5].val, ANS[6].val );
			l_sub( a, a, ANS[10].val );
			if( l_test( a ) < 0 )	l_clear( ANS[11].val );
			else					memmove( ANS[11].val, a, 6 );
			
			// 控除不足還付税額 (11 - 6 - 7)
			l_sub( a, ANS[10].val, ANS[5].val );
			l_sub( a, a, ANS[6].val );
			// 中間申告以外
			if( (l_test( a ) < 0 ) || ! (Snh->Sn_SKKBN % 2) )
					l_clear( ANS[12].val );
			else	memmove( ANS[12].val, a, 6 );
			
			// 差引税額 (12 - 14)
			if( ! l_test( ANS[12].val ) )
					l_sub( ANS[14].val, ANS[11].val, ANS[13].val );
			else	l_clear( ANS[14].val );
			
			// １００円未満　切り捨て
			if( l_test( ANS[14].val ) )
					p_calq( (char *)ANS[14].val, (char *)ANS[14].val, 0x10 );*/
			break;
	}
	
	// 中間納付税額
	memmove( a, m_pSnHonpyoData->Sn_TYNOFZ, 6 );
//	p_calq( (char *)(vers ? ANS[9].val : ANS[15].val), (char *)a, 0x10 );	// 100円未満切り捨て
	m_Util.l_calq( (char *)(vers ? ANS[9].val : ANS[15].val), (char *)a, 0x10 );	// 100円未満切り捨て
	
	// 納付税額 (15 - 16) ･ 中間納付還付税額
	if( vers ){
		m_Arith.l_sub( a, ANS[8].val, ANS[9].val );
//		p_calq( (char *)a, (char *)a, 0x10 );								// 100円未満切り捨て
		m_Util.l_calq( (char *)a, (char *)a, 0x10 );						// 100円未満切り捨て
		if( m_Arith.l_test( a ) < 0 ) {
			m_Arith.l_neg( a );
			memmove( ANS[11].val, a, 6 );
		}
		else{
			memmove( ANS[10].val, a, 6 );
		}
	}
	else {
		if( !m_Arith.l_test( ANS[12].val ) ){
			m_Arith.l_sub( a, ANS[14].val, ANS[15].val );
			if( m_Arith.l_test( a ) < 0 ){
				m_Arith.l_clear( ANS[16].val );
			}
			else{
//				p_calq( (char *)ANS[16].val, (char *)a, 0x10 );				// 100円未満　切り捨て
				m_Util.l_calq( (char *)ANS[16].val, (char *)a, 0x10 );		// 100円未満　切り捨て
			}
		}
		else{
			m_Arith.l_clear( ANS[16].val );
		}
		
		if( m_Arith.l_test( ANS[12].val ) ){
			memmove( ANS[17].val, ANS[15].val, 6 );
		}
		else{
			m_Arith.l_sub( a, ANS[15].val, ANS[14].val );
			if( m_Arith.l_test( a ) < 0 ){
				m_Arith.l_clear( ANS[17].val );
			}
			else{
				memmove( ANS[17].val, a, 6 );
			}
		}
	}
	
	memmove( m_pSnHonpyoData->Sn_EDNOFZ, (vers ? ANS[10].val : ANS[16].val), 6 );
	
	// 修正申告
	if( (m_pSnHeadData->Sn_SKKBN==3) || (m_pSnHeadData->Sn_SKKBN==4) ){
		// 既確定税額
		memmove( (vers ? ANS[12].val : ANS[18].val), m_pSnHonpyoData->Sn_KAKTIZ, 6 );
		
		if( vers ){
			if( m_Arith.l_test( ANS[10].val ) > 0 ){		// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[10].val, ANS[12].val );
			}
			else{
				memmove( a, ANS[11].val, 6 );
				m_Arith.l_neg( a );
				m_Arith.l_sub( a, a, ANS[12].val );
			}
			m_Arith.l_sub( a, a, ANS[7].val );
		}
		else{
			// 差引納付税額 (17 - 19)
			if( m_Arith.l_test( ANS[12].val ) ){
				m_Arith.l_add( a, ANS[12].val, ANS[17].val );
				m_Arith.l_add( a, a, ANS[18].val );
				m_Arith.l_neg( a );
			}
			else{
				if( m_Arith.l_test( ANS[16].val ) ){
					m_Arith.l_sub( a, ANS[16].val, ANS[18].val );
				}
				else{
					m_Arith.l_add( a, ANS[17].val, ANS[18].val );
					m_Arith.l_neg( a );
				}
			}
		}
		// １００円未満　切り捨て
//		p_calq( (char *)(vers ? ANS[13].val : ANS[19].val), (char *)a, 0x10 );
		m_Util.l_calq( (char *)(vers ? ANS[13].val : ANS[19].val), (char *)a, 0x10 );
	}
#ifdef _CUT_SHOHI_	
	// 課税資産の譲渡等の対価の額
	memmove( (vers ? ANS[14].val : ANS[20].val), Snd->Sn_KZURI, 6 );
	if( Kani > 1 )	 // 基準期間の課税売上高
		memmove( (vers ? ANS[15].val : ANS[21].val), Snd->Sn_ZZURI, 6 );
	else			 // 資産の譲渡等の対価の額
		memmove( (vers ? ANS[15].val : ANS[21].val), Snd->Sn_SOURI, 6 );
#endif	
	if( !vers ){
		return 0;
	}
	
	// 地方消費税の計算
	// 控除不足還付税額
//	if( !((m_pSnHeadData->Sn_SKKBN&0xff)%2) && (N_v!=2) ){
	if( 0 ){
		;	// 中間且改正日前開始
	}
	else{

/*- '14.09.02 -*/
//		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
//			if( vers == 1 ){
//				m_Arith.l_add( ANS[16].val, pSnFhyo10Data->Sn_1F8B, pSnFhyo10Data->Sn_1F8C );
//			}
//			else{
//				memmove( ANS[16].val, ANS[7].val, 	6 );
//			}
//		}
//		
//		// 差引税額
//		if( (m_Arith.l_test(pSnFhyo10Data->Sn_1F10T)<0) && ! ((m_pSnHeadData->Sn_SKKBN&0xff) % 2) ){
//			;
//		}
//		else {		// 還付　中間申告以外
//			if( vers == 1 ){
//				if( m_Arith.l_test(m_pSnHonpyoData->Sn_GENKAI) ){
//					;
//				}
//				else{
//					m_Arith.l_add( a, pSnFhyo10Data->Sn_1F9A, pSnFhyo10Data->Sn_1F9B );
//					m_Arith.l_add( a, a, pSnFhyo10Data->Sn_1F9C );
//				}
//			}
//			else{
//				memmove( a, ANS[8].val, 6 );
//			}
//			m_Util.l_calq( (char *)ANS[17].val, (char *)a, 0x10 );		// １００円未満　切り捨て
//		}
/*-------------*/
		if( isH25Syz == FALSE ){
			char	tmpMony[6]={0}, tmpMony2[6]={0};
			if( vers == 1 ){
				m_Arith.l_add( tmpMony, pSnFhyo10Data->Sn_1F8B, pSnFhyo10Data->Sn_1F8C );
				m_Arith.l_add( tmpMony2, pSnFhyo10Data->Sn_1F9B, pSnFhyo10Data->Sn_1F9C );
				m_Arith.l_sub( tmpMony, tmpMony2, tmpMony );
				if( m_Arith.l_test(tmpMony) < 0 ){
					m_Arith.l_neg( tmpMony );
				}
				else{
					m_Arith.l_clear( tmpMony );
				}
			}
			else{
				memmove( tmpMony, ANS[7].val, 6 );
			}
			if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
				memmove( ANS[16].val, tmpMony, 6 );
			}

			// 差引税額
			if( m_Arith.l_test(ANS[16].val) ){
				;
			}
			else{		// 還付　中間申告以外
				if( vers == 1 ){
					if( m_Arith.l_test(m_pSnHonpyoData->Sn_GENKAI) ){
						;
					}
					else{
						m_Arith.l_add( tmpMony, pSnFhyo10Data->Sn_1F8B, pSnFhyo10Data->Sn_1F8C );
						m_Arith.l_add( tmpMony2, pSnFhyo10Data->Sn_1F9B, pSnFhyo10Data->Sn_1F9C );
						m_Arith.l_sub( tmpMony, tmpMony2, tmpMony );
						if( m_Arith.l_test(tmpMony) > 0 ){
							m_Util.l_calq( (char *)ANS[17].val, (char *)tmpMony, 0x10 );
						}
					}
				}
				else{
					memmove( a, ANS[8].val, 6 );
					m_Util.l_calq( (char *)ANS[17].val, (char *)a, 0x10 );
				}
			}
		}
		else{
		}
/*-------------*/
		
		// 譲渡割額還付額 ･ 譲渡割額納税額
/*- '14.03.08 -*/
//		m_Arith.l_input( b, "250" );
//		if( m_Arith.l_test( ANS[16].val ) ){
//			m_Arith.l_mul100( (unsigned char *)ANS[18].val, (unsigned char *)ANS[16].val, (unsigned char *)b, 1, 0 );
//		}
//		if( m_Arith.l_test( ANS[17].val ) ){
//			m_Arith.l_mul100( (unsigned char *)a, (unsigned char *)ANS[17].val, (unsigned char *)b, 1, 0 );
////			p_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
//			m_Util.l_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
//		}
/*-------------*/
		if( vers == 1 ){	// 経過措置対象

			memset( a, '\0', sizeof(a) );
			memset( b, '\0', sizeof(b) );
			memset( c, '\0', sizeof(c) );
			memset( d, '\0', sizeof(d) );

			m_Util.percent( a, (char*)pSnFhyo10Data->Sn_1F8B, 7, 25, 0 );
			m_Util.percent( b, (char*)pSnFhyo10Data->Sn_1F8C, 8, 17, 0 );
			m_Util.percent( c, (char*)pSnFhyo10Data->Sn_1F9B, 7, 25, 0 );
			m_Util.percent( d, (char*)pSnFhyo10Data->Sn_1F9C, 8, 17, 0 );

			// 15-14
			char	sagaku1[6]={0}, sagaku2[6]={0};
			m_Arith.l_sub( sagaku1, c, a );
			m_Arith.l_sub( sagaku2, d, b );

			// 16
			char	goukei[6]={0};
			m_Arith.l_add( goukei, sagaku1, sagaku2 );
			if( m_Arith.l_test(goukei) > 0 ){
				m_Util.l_calq( (char *)ANS[19].val, (char *)goukei, 0x10 );	// １００円未満　切り捨て
			}
			else{
				m_Arith.l_neg( goukei );
				memmove( ANS[18].val, goukei, 6 );
			}
		}
		else{				// 経過措置対象外
			if( m_Arith.l_test(ANS[16].val) ){
				m_Util.percent( (char*)ANS[18].val, (char*)ANS[16].val, 8, 17, 0 );
			}
			if( m_Arith.l_test( ANS[17].val ) ){
				m_Util.percent( a, (char*)ANS[17].val, 8, 17, 0 );
				m_Util.l_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
			}
		}
/*-------------*/
		
		// 中間納付譲渡割額
		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
//			p_calq( (char *)ANS[20].val, (char *)m_pSnHonpyoData->Sn_TTYWAR, 0x10 );	// 100円未満切り捨て
			m_Util.l_calq( (char *)ANS[20].val, (char *)m_pSnHonpyoData->Sn_TTYWAR, 0x10 );	// 100円未満切り捨て
		}
		
		// 納付譲渡割額 (20 - 21) ･ 中間還付譲渡割額 (21 - 20)
		m_Arith.l_sub( a, ANS[19].val, ANS[20].val );
		if( m_Arith.l_test(a) < 0 ){
			m_Arith.l_neg( a );
//			p_calq( (char *)ANS[22].val, (char *)a, 0x10 );		// １００円未満　切り捨て
			m_Util.l_calq( (char *)ANS[22].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
		else{
//			p_calq( (char *)ANS[21].val, (char *)a, 0x10 );	// １００円未満　切り捨て
			m_Util.l_calq( (char *)ANS[21].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
		
		// 修正申告
		if( (m_pSnHeadData->Sn_SKKBN==3) || (m_pSnHeadData->Sn_SKKBN==4) ){
			// 既確定税額
			memmove( ANS[23].val, m_pSnHonpyoData->Sn_TKAKTZ, 6 );
			
			// 差引納付譲渡割額
			if( m_Arith.l_test(ANS[21].val) > 0 ){	// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[21].val, ANS[23].val );
			}
			else{
				m_Arith.l_add( a, ANS[18].val, ANS[22].val );
				if( m_Arith.l_test(a) ){
					m_Arith.l_neg( a );
				}
				m_Arith.l_sub( a, a, ANS[23].val );
			}
//			p_calq( (char *)ANS[24].val, (char *)a, 0x10 );		// １００円未満　切り捨て
			m_Util.l_calq( (char *)ANS[24].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
	}
	
	// 消費税及び地方消費税の合計（納付又は還付）税額
	switch( m_pSnHeadData->Sn_SKKBN&0xff ) {
		case 1 :	// 確定・中間申告
		case 2 :		
			m_Arith.l_add( a, ANS[10].val, ANS[21].val );
			m_Arith.l_add( b, ANS[7].val,  ANS[11].val );
			m_Arith.l_add( b, b, ANS[18].val );
			m_Arith.l_add( b, b, ANS[22].val );
			m_Arith.l_sub( ANS[25].val, a, b );
			break;
		default	:				// 修正申告
			m_Arith.l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}
	memmove( Zei, ANS[25].val, 6 );
	
	return 0;
}

//-----------------------------------------------------------------------------
// データ読み込み（金額集計用）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::ReadDataForCalq()
{
	// データ読み込み
	if( !m_pSnHonpyoData ){
		if( m_SixKindFlg == TRUE ){
//-- '15.08.19 --
//			m_pSnHonpyoData = new CH27SnHonpyoData();
//			if( m_pSnHonpyoData ){
//				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//			}
//			else{
//				return -1;
//			}
//---------------
			m_pSnHonpyoData = new CH28SnHonpyoData();
			if( m_pSnHonpyoData ){
				if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				return -1;
			}
//---------------
		}
		else{
			m_pSnHonpyoData = new CH26SnHonpyoData();
			if( m_pSnHonpyoData ){
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				return -1;
			}
		}
	}

	BOOL	isTransitionalMeasure = FALSE;
	if( m_pSnHeadData->SVmzsw == 1 ){
		isTransitionalMeasure = TRUE;
	}
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		if( !m_pSnFhyo10Data ){

			//------------>yoshida150810
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				m_pSnFhyo10Data = new CH28SnFhyo10Data(isTransitionalMeasure);
				if( m_pSnFhyo10Data ){
					if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					return -1;
				}
			}
			else{
				m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
				if( m_pSnFhyo10Data ){
					if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					return -1;
				}
			}
			//--------------------------
			/*
			m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
			if( m_pSnFhyo10Data ){
				if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				return -1;
			}
			*/
			//<---------------------------
		}
		if( !m_pSnTanaData ){
			m_pSnTanaData = new CH26SnTanaData(isTransitionalMeasure);
			if( m_pSnTanaData ){
				if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				return -1;
			}
		}
	}
	else{
		if( !m_pSnFhyo40Data ){
			m_pSnFhyo40Data = new CH26SnFhyo10Data(isTransitionalMeasure);
			if( m_pSnFhyo40Data ){
				if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				return -1;
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本表の計算(1)～(26)迄
// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備
//-----------------------------------------------------------------------------
// 引数	type	：	呼出年度種別(0:平成25年版, 1:平成26年版)
//-----------------------------------------------------------------------------
void CH26HyoView::AlreadyDecisionCalc( int type )
{
//--> '15.03.16 INS START
	if( ReadDataForCalq() ){
		return;
	}
//<-- '15.03.16 INS END

	if( type == 0 ){
		AlreadyDecisionCalcH25();
	}
	else if( type == 1 ){
		AlreadyDecisionCalcH26();
	}
}

//-----------------------------------------------------------------------------
//★//[13'05.21]///
// 本表の計算(1)～(26)迄
// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CH26HyoView::AlreadyDecisionCalcH25()
{
	char vers = 0x02;
	if( m_pSnHeadData->SVmzsw ){
		vers = 1;
	}

	SH_ANS	ANS[30]={0}, JANS[20]={0};
	char 	a[6], b[6], c[6], WORK[6];
	
	memset( &ANS, '\0', sizeof(ANS) );
	memset( &JANS, '\0', sizeof(JANS) );
	
	// 課税標準額
	// 普通乗用自動車 ６％分
	memmove( a, m_pSnHeadData->Sn_CK60, 6 );
	m_Util.l_calq( (char *)JANS[0].val, (char *)a, 0 );		// １０００円未満　切り捨て
	
	// 普通乗用自動車 ４．５％分
	memmove( a, m_pSnHeadData->Sn_CK45, 6 );
	m_Util.l_calq( (char *)JANS[1].val, (char *)a, 0 );		// １０００円未満　切り捨て
	
	// 消費税額
	// 普通乗用自動車
	// 6%
	memmove( JANS[2].val, m_pSnHeadData->Sn_CZ60, 6 );
	// 4.5%
	memmove( JANS[3].val, m_pSnHeadData->Sn_CZ45, 6 );

	CH26SnFhyo10Data	*pSnFhyo10Data = NULL;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		pSnFhyo10Data = m_pSnFhyo10Data;
	}
	else{
		pSnFhyo10Data = m_pSnFhyo40Data;
	}

	switch( vers&0x0f ){
		case 1 :	// 経過措置対象
			// 課税標準額

			// ４％分
			memmove( a, pSnFhyo10Data->Sn_1F1B, 6 );
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );		// １０００円未満　切り捨て
			
			// ３％分
			memmove( a, pSnFhyo10Data->Sn_1F1A, 6 );				
			m_Util.l_calq( (char *)JANS[5].val, (char *)a, 0 );		// １０００円未満　切り捨て

/*- '14.05.16 -*/
//			// ６．３％分
//			memmove( a, pSnFhyo10Data->Sn_1F1C, 6 );				
//			m_Util.l_calq( (char *)JANS[6].val, (char *)a, 0 );		// １０００円未満　切り捨て
/*-------------*/

			m_Arith.l_add( ANS[0].val, JANS[4].val, JANS[5].val );
			m_Arith.l_add( ANS[0].val, ANS[0].val, JANS[6].val );
			
			// 消費税額
			memmove( JANS[7].val, pSnFhyo10Data->Sn_1F2A, 6 );		// ３％分
			memmove( JANS[8].val, pSnFhyo10Data->Sn_1F2B, 6 );		// ４％分
/*- '14.05.16 -*/
//			memmove( JANS[9].val, pSnFhyo10Data->Sn_1F2C, 6 );		// ６．３％分
/*-------------*/
			
			m_Arith.l_add( ANS[1].val, JANS[7].val, JANS[8].val );
			m_Arith.l_add( ANS[1].val, ANS[1].val, JANS[9].val );
			break;

		case 2 :	// 経過措置対象外
			// 課税標準額
			memmove( a, m_pSnHonpyoData->Sn_KSTD, 6 );
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );		// １０００円未満　切り捨て
			
			// 消費税額
			memmove( JANS[6].val, m_pSnHonpyoData->Sn_SYTX, 6 );
			
			memmove( ANS[0].val, JANS[4].val, 6 );
			memmove( ANS[1].val, JANS[6].val, 6 );
			break;

		default:	// 旧バージョンマスター
/*			// 普通乗用自動車
			memmove( a, Snd.Sn_KSTDM, 6 );
			p_calq( (char *)ANS[0].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// その他の課税分
			memmove( a, Snd.Sn_KSTD, 6 );
			p_calq( (char *)ANS[1].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// 計 (1 + 2)
			l_add( ANS[2].val, ANS[0].val, ANS[1].val );
			
			// 消費税額
			memmove( ANS[3].val, Snd.Sn_SYTXM, 6 );
			
			// その他の課税分
			memmove( ANS[4].val, Snd.Sn_SYTX, 6 );
			
			// 計 (4 + 5)
			l_add( ANS[5].val, ANS[3].val, ANS[4].val );*/
			break;
	}
	
	// 簡易 -> 貸倒回収に係る or 税額一般 -> 控除過大調整税額
	if( vers ){
		memmove( ANS[2].val, m_pSnHonpyoData->Sn_KJADJZ, 6 );
	}
	else {
		memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
		memmove( c, m_pSnTanaData->Sn_MENZZ, 6 );
		if( m_pSnTanaData->Sn_MENZsw & 0x01 ){
			if( m_Arith.l_test(c) ){
				m_Arith.l_neg(c);
			}
		}
		m_Arith.l_add( a, a, c );
		memmove( b, m_pSnHonpyoData->Sn_KJADJZ, 6 );
		if( m_Arith.l_test(a) < 0 ){
			m_Arith.l_neg( a );
			m_Arith.l_add( ANS[6].val, b, a );
		}
		else{
			memmove( ANS[6].val, b, 6 );
		}
	}
	
	// 控除税額
	// 控除対象仕入税額
	memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
	if( !vers ){
		m_Arith.l_add( a, a, c );
	}
	if( m_Arith.l_test(a) < 0 ){
		m_Arith.l_clear( (vers ? ANS[3].val : ANS[7].val) );
	}
	else{
		memmove( (vers ? ANS[3].val : ANS[7].val), a, 6 );
	}
	
	// 返還等対価の税額
	memmove( (vers ? ANS[4].val : ANS[8].val), m_pSnHonpyoData->Sn_HKANZ, 6 );
	
	// 貸倒れに係る税額
	memmove( (vers ? ANS[5].val : ANS[9].val), m_pSnHonpyoData->Sn_KSITZ, 6 );
	
	// 控除税額小計 vers ? (4 + 5 + 6) : (8 + 9 + 10)
	if( vers ){
		m_Arith.l_add( ANS[6].val, ANS[3].val, ANS[4].val );
		m_Arith.l_add( ANS[6].val, ANS[6].val, ANS[5].val );
	}
	else{
		m_Arith.l_add( ANS[10].val, ANS[7].val,  ANS[8].val );
		m_Arith.l_add( ANS[10].val, ANS[10].val, ANS[9].val );
	}
	
	// 限界控除税額
	memmove( (vers ? JANS[10].val : ANS[13].val), m_pSnHonpyoData->Sn_GENKAI, 6 );
	
	switch( vers&0x0f ){
		// 控除不足還付税額 (7 - 2 - 3) ･ 差引税額 (2 + 3 - 7)
		case 1 :	
			if( m_Arith.l_test(pSnFhyo10Data->Sn_1F10T) < 0 ){
				if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
					memmove( ANS[7].val, pSnFhyo10Data->Sn_1F10T, 6 );
					m_Arith.l_neg( ANS[7].val );
				}
			}
			else{
				if( m_Arith.l_test( m_pSnHonpyoData->Sn_GENKAI ) ){
					// 有り得ない！
//					memmove( a, Snd.Sn_3F3,   6 );
				}
				else{
					memmove( a, pSnFhyo10Data->Sn_1F10T, 6 );
				}
				m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );	//100円未満切り捨て
			}
			break;
		case 2 :
			m_Arith.l_add( a, ANS[1].val, ANS[2].val );
			m_Arith.l_sub( a, a, ANS[6].val );
			// 限界控除前の税額がマイナスの時
			if( m_Arith.l_test(a) < 0 ){
				if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
					m_Arith.l_neg( a );
					memmove( ANS[7].val, a, 6 );
				}
			}
			else{
				m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );	// 100円未満切り捨て
			}
			break;
	
				// 限界控除前の税額 (6 + 7 - 11)
		default:	
/*			l_add( a, ANS[5].val, ANS[6].val );
			l_sub( a, a, ANS[10].val );
			if( l_test( a ) < 0 )	l_clear( ANS[11].val );
			else					memmove( ANS[11].val, a, 6 );
					
			// 控除不足還付税額 (11 - 6 - 7)
			l_sub( a, ANS[10].val, ANS[5].val );
			l_sub( a, a, ANS[6].val );
			// 中間申告以外
			if( (l_test( a ) < 0 ) || ! (Snh.Sn_SKKBN % 2) )
					l_clear( ANS[12].val );
			else	memmove( ANS[12].val, a, 6 );
					
			// 差引税額 (12 - 14)
			if( ! l_test( ANS[12].val ) )
					l_sub( ANS[14].val, ANS[11].val, ANS[13].val );
			else	l_clear( ANS[14].val );
				
			// １００円未満　切り捨て
			if( l_test( ANS[14].val ) )
					p_calq( (char *)ANS[14].val, (char *)ANS[14].val, 0x10 );*/
			break;
	}
	
	// 中間納付税額
	memmove( a, m_pSnHonpyoData->Sn_TYNOFZ, 6 );
	m_Util.l_calq( (char *)(vers ? ANS[9].val : ANS[15].val), (char *)a, 0x10 );	// 100円未満切り捨て
	
	// 納付税額 (15 - 16) ･ 中間納付還付税額
	if( vers ){
		m_Arith.l_sub( a, ANS[8].val, ANS[9].val );
		m_Util.l_calq( (char *)a, (char *)a, 0x10 );	// 100円未満切り捨て
		if( m_Arith.l_test( a ) < 0 ){
			m_Arith.l_neg( a );
			memmove( ANS[11].val, a, 6 );
		}
		else{
			memmove( ANS[10].val, a, 6 );
		}
	}
	else {
		if( !m_Arith.l_test( ANS[12].val ) ) {
			m_Arith.l_sub( a, ANS[14].val, ANS[15].val );
			if( m_Arith.l_test(a) < 0 ){
				m_Arith.l_clear( ANS[16].val );
			}
			else{
				m_Util.l_calq( (char *)ANS[16].val, (char *)a, 0x10 );	// 100円未満切り捨て
			}
		}
		else{
			m_Arith.l_clear( ANS[16].val );
		}
		
		if( m_Arith.l_test(ANS[12].val) ){
			memmove( ANS[17].val, ANS[15].val, 6 );
		}
		else{
			m_Arith.l_sub( a, ANS[15].val, ANS[14].val );
			if( m_Arith.l_test(a) < 0 ){
				m_Arith.l_clear( ANS[17].val );
			}
			else{
				memmove( ANS[17].val, a, 6 );
			}
		}
	}
	
	memmove( m_pSnHonpyoData->Sn_EDNOFZ, (vers ? ANS[10].val : ANS[16].val), 6 );
	
	// 修正申告
	if( (m_pSnHeadData->Sn_SKKBN==3) || (m_pSnHeadData->Sn_SKKBN==4) ){
		// 既確定税額
		memmove( (vers ? ANS[12].val : ANS[18].val), m_pSnHonpyoData->Sn_KAKTIZ, 6 );
		
		if( vers ){
			if( m_Arith.l_test( ANS[10].val ) > 0 ){		// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[10].val, ANS[12].val );
			}
			else{
				memmove( a, ANS[11].val, 6 );
				m_Arith.l_neg( a );
				m_Arith.l_sub( a, a, ANS[12].val );
			}
			m_Arith.l_sub( a, a, ANS[7].val );
		}
		else {
			// 差引納付税額 (17 - 19)
			if( m_Arith.l_test( ANS[12].val ) ) {
				m_Arith.l_add( a, ANS[12].val, ANS[17].val );
				m_Arith.l_add( a, a, ANS[18].val );
				m_Arith.l_neg( a );
			}
			else {
				if( m_Arith.l_test( ANS[16].val ) ){
					m_Arith.l_sub( a, ANS[16].val, ANS[18].val );
				}
				else{
					m_Arith.l_add( a, ANS[17].val, ANS[18].val );
					m_Arith.l_neg( a );
				}
			}
		}
		// １００円未満　切り捨て
		m_Util.l_calq( (char *)(vers ? ANS[13].val : ANS[19].val), (char *)a, 0x10 );
	}
	
	// 課税資産の譲渡等の対価の額
	memmove( (vers ? ANS[14].val : ANS[20].val), m_pSnHonpyoData->Sn_KZURI, 6 );
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){	 // 基準期間の課税売上高
		memmove( (vers ? ANS[15].val : ANS[21].val), m_pSnHonpyoData->Sn_ZZURI, 6 );
	}
	else{			 // 資産の譲渡等の対価の額
		memmove( (vers ? ANS[15].val : ANS[21].val), m_pSnHonpyoData->Sn_SOURI, 6 );
	}
	
	if( !vers ){
		return;
	}
	
	// 地方消費税の計算
	// 控除不足還付税額
//	if( ! ((Snh.Sn_SKKBN&0xff) % 2) && N_ver != 2 ){
	if( 0 ){
		;	// 中間且改正日前開始
	}
	else{
		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ) {	// 中間申告以外
			if( vers == 1 ){
				memmove( ANS[16].val, pSnFhyo10Data->Sn_1F8B, 6 );
			}
			else{
				memmove( ANS[16].val, ANS[7].val, 	6 );
			}
		}
		
		// 差引税額
		if( ((vers==1)&&(m_Arith.l_test(pSnFhyo10Data->Sn_1F10T)<0)) && !((m_pSnHeadData->Sn_SKKBN&0xff) % 2) ){
			;
		}
		else {		// 還付　中間申告以外
			if( vers == 1 ){
				if( m_Arith.l_test( m_pSnHonpyoData->Sn_GENKAI ) ){
					// 有り得ないはず
//					memmove( a, Snd.Sn_3F6,   6 );
				}
				else{
					memmove( a, pSnFhyo10Data->Sn_1F9B, 6 );
				}
			}
			else{
				memmove( a, ANS[8].val, 6 );
			}
			m_Util.l_calq( (char *)ANS[17].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
		
		// 譲渡割額還付額 ･ 譲渡割額納税額
/*- '14.03.07 -*/
		m_Arith.l_input( b, "250" );
		if( m_Arith.l_test( ANS[16].val ) ){
			m_Arith.l_mul100( (unsigned char *)ANS[18].val, (unsigned char *)ANS[16].val, (unsigned char *)b, 1, 0 );
		}
		if( m_Arith.l_test( ANS[17].val ) ){
			m_Arith.l_mul100( (unsigned char *)a, (unsigned char *)ANS[17].val, (unsigned char *)b, 1, 0 );
			m_Util.l_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
/*-------------*/
/*		if( vers == 1 ){	// 経過措置対象

			memset( a, '\0', sizeof(a) );
			memset( b, '\0', sizeof(b) );
			memset( c, '\0', sizeof(c) );
			memset( d, '\0', sizeof(d) );

			m_Util.percent( a, (char*)pSnFhyo10Data->Sn_1F8B, 7, 25, 0 );
			m_Util.percent( b, (char*)pSnFhyo10Data->Sn_1F8C, 8, 17, 0 );
			m_Util.percent( c, (char*)pSnFhyo10Data->Sn_1F9B, 7, 25, 0 );
			m_Util.percent( d, (char*)pSnFhyo10Data->Sn_1F9C, 8, 17, 0 );

			// 15-14
			char	sagaku1[6]={0}, sagaku2[6]={0};
			m_Arith.l_sub( sagaku1, c, a );
			m_Arith.l_sub( sagaku2, d, b );

			// 16
			char	goukei[6]={0};
			m_Arith.l_add( goukei, sagaku1, sagaku2 );
			if( m_Arith.l_test(goukei) > 0 ){
				m_Util.l_calq( (char *)ANS[19].val, (char *)goukei, 0x10 );	// １００円未満　切り捨て
			}
			else{
				if( m_pSnHeadData->Sn_SKKBN != 4 ){
					m_Arith.l_neg( goukei );
					memmove( ANS[18].val, goukei, 6 );
				}
			}
		}
		else{				// 経過措置対象外
			if( m_Arith.l_test(ANS[16].val) ){
				m_Util.percent( (char*)ANS[18].val, (char*)ANS[16].val, 8, 17, 0 );
			}
			if( m_Arith.l_test( ANS[17].val ) ){
				m_Util.percent( a, (char*)ANS[17].val, 8, 17, 0 );
				m_Util.l_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
			}
		}*/
/*-------------*/
		
		// 中間納付譲渡割額
		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
			m_Util.l_calq( (char *)ANS[20].val, (char *)m_pSnHonpyoData->Sn_TTYWAR, 0x10 );	// 100円未満切り捨て
		}
		
		// 納付譲渡割額 (20 - 21) ･ 中間還付譲渡割額 (21 - 20)
		m_Arith.l_sub( a, ANS[19].val, ANS[20].val );
		if( m_Arith.l_test(a) < 0 ){
			m_Arith.l_neg( a );
			m_Util.l_calq( (char *)ANS[22].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
		else{
			m_Util.l_calq( (char *)ANS[21].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
		
		// 修正申告
		if( (m_pSnHeadData->Sn_SKKBN==3) || (m_pSnHeadData->Sn_SKKBN==4) ){
			// 既確定税額
			memmove( ANS[23].val, m_pSnHonpyoData->Sn_TKAKTZ, 6 );
			
			// 差引納付譲渡割額
			if( m_Arith.l_test( ANS[21].val ) > 0 ){	// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[21].val, ANS[23].val );
			}
			else{
				m_Arith.l_add( a, ANS[18].val, ANS[22].val );
				if( m_Arith.l_test( a ) ){
					m_Arith.l_neg( a );
				}
				m_Arith.l_sub( a, a, ANS[23].val );
			}
			m_Util.l_calq( (char *)ANS[24].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
	}
	
	// 消費税及び地方消費税の合計（納付又は還付）税額
	switch( m_pSnHeadData->Sn_SKKBN&0xff ){
		case 1 : case 2 :		// 確定・中間申告
			m_Arith.l_add( a, ANS[10].val, ANS[21].val );
			m_Arith.l_add( b, ANS[7].val,  ANS[11].val );
			m_Arith.l_add( b, b, ANS[18].val );
			m_Arith.l_add( b, b, ANS[22].val );
			m_Arith.l_sub( ANS[25].val, a, b );
			break;
		default	:				// 修正申告
			m_Arith.l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}

	// 国税の既確定税額の算出
	if( m_Arith.l_test( ANS[7].val ) > 0 ){
		m_Arith.l_neg( ANS[7].val );
	}
	m_Arith.l_add( WORK, ANS[7].val, ANS[8].val );
	m_Arith.l_sub( WORK, WORK, ANS[9].val );
	memmove( m_pSnHonpyoData->Sn_KAKTIZ, WORK, 6 );

	// 地方税の既確定譲渡割額の算出
	if( m_Arith.l_test( ANS[18].val ) > 0 ){
		m_Arith.l_neg( ANS[18].val );
	}
	m_Arith.l_add( WORK, ANS[18].val, ANS[19].val );
	m_Arith.l_sub( WORK, WORK, ANS[20].val );
	memmove( m_pSnHonpyoData->Sn_TKAKTZ, WORK, 6 );

	if( m_pZmSub ){
		CString	szFilter;
		szFilter.Format( _T("Sn_SEQ=%d"), m_SnSeq );
		if( m_pZmSub->szsnk == NULL ){
			m_pZmSub->SzsnkOpen( szFilter );
		}
		else{
			m_pZmSub->szsnk->Requery( szFilter );
		}
		if( m_pZmSub->szsnk->st != -1 ){
			char	monyBuf1[32]={0}, monyBuf2[32]={0};
			m_pZmSub->szsnk->Edit();
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_KAKTIZ, m_pSnHonpyoData->Sn_KAKTIZ );
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_TKAKTZ, m_pSnHonpyoData->Sn_TKAKTZ );
			m_pZmSub->szsnk->Update();
		}
	}
}

//-----------------------------------------------------------------------------
//★//[13'05.21]///
// 本表の計算(1)～(26)迄
// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CH26HyoView::AlreadyDecisionCalcH26()
{
	// バージョンチェック
/*	char	vers = N_ver;
	switch( vers ) {
		case 1 :	if( ! TBhead.SVmzsw )	vers = 2;	break;
		case 2 :	if(   TBhead.SVmzsw )	vers = 1;	break;
		default:	break;
	}*/

	char vers = 0x02;
	if( m_pSnHeadData->SVmzsw ){
		vers = 1;
	}

	SH_ANS	ANS[30]={0}, JANS[20]={0};

	char 	a[6], b[6], c[6], d[6], WORK[6];
	
//	l_defn( 0x16 );
	memset( &ANS, '\0', sizeof(ANS) );
	memset( &JANS, '\0', sizeof(JANS) );
	
	// 課税標準額
	// 普通乗用自動車 ６％分
	memmove( a, m_pSnHeadData->Sn_CK60, 6 );
//	p_calq( (char *)JANS[0].val, (char *)a, 0 );				// １０００円未満　切り捨て
	m_Util.l_calq( (char *)JANS[0].val, (char *)a, 0 );				// １０００円未満　切り捨て
	
	// 普通乗用自動車 ４．５％分
	memmove( a, m_pSnHeadData->Sn_CK45, 6 );
//	p_calq( (char *)JANS[1].val, (char *)a, 0 );				// １０００円未満　切り捨て
	m_Util.l_calq( (char *)JANS[1].val, (char *)a, 0 );				// １０００円未満　切り捨て
	
	// 消費税額
	// 普通乗用自動車
	// 6%
	memmove( JANS[2].val, m_pSnHeadData->Sn_CZ60, 6 );
	// 4.5%
	memmove( JANS[3].val, m_pSnHeadData->Sn_CZ45, 6 );


	CH26SnFhyo10Data	*pSnFhyo10Data = NULL;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		pSnFhyo10Data = m_pSnFhyo10Data;
	}
	else{
		pSnFhyo10Data = m_pSnFhyo40Data;
	}

	switch( vers&0x0f ){
		case 1 :	// 経過措置対象
			// 課税標準額

			// ４％分
			memmove( a, pSnFhyo10Data->Sn_1F1B, 6 );
//			p_calq( (char *)JANS[4].val, (char *)a, 0 );				// １０００円未満　切り捨て
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// ３％分
			memmove( a, pSnFhyo10Data->Sn_1F1A, 6 );				
//			p_calq( (char *)JANS[5].val, (char *)a, 0 );				// １０００円未満　切り捨て
			m_Util.l_calq( (char *)JANS[5].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// ６．３％分
			memmove( a, pSnFhyo10Data->Sn_1F1C, 6 );				
			m_Util.l_calq( (char *)JANS[6].val, (char *)a, 0 );			// １０００円未満　切り捨て

			m_Arith.l_add( ANS[0].val, JANS[4].val, JANS[5].val );
			m_Arith.l_add( ANS[0].val, ANS[0].val, JANS[6].val );
			
			// 消費税額
			memmove( JANS[7].val, pSnFhyo10Data->Sn_1F2A, 6 );			// ３％分
			memmove( JANS[8].val, pSnFhyo10Data->Sn_1F2B, 6 );			// ４％分
			memmove( JANS[9].val, pSnFhyo10Data->Sn_1F2C, 6 );			// ６．３％分
			
			m_Arith.l_add( ANS[1].val, JANS[7].val, JANS[8].val );
			m_Arith.l_add( ANS[1].val, ANS[1].val, JANS[9].val );
			break;

		case 2 :	// 経過措置対象外
			// 課税標準額
			memmove( a, m_pSnHonpyoData->Sn_KSTD, 6 );
//			p_calq( (char *)JANS[4].val, (char *)a, 0 );				// １０００円未満　切り捨て
			m_Util.l_calq( (char *)JANS[4].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// 消費税額
			memmove( JANS[6].val, m_pSnHonpyoData->Sn_SYTX, 6 );
			
			memmove( ANS[0].val, JANS[4].val, 6 );
			memmove( ANS[1].val, JANS[6].val, 6 );
			break;

		default:	// 旧バージョンマスター
/*			// 普通乗用自動車
			memmove( a, Snd.Sn_KSTDM, 6 );
			p_calq( (char *)ANS[0].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// その他の課税分
			memmove( a, Snd.Sn_KSTD, 6 );
			p_calq( (char *)ANS[1].val, (char *)a, 0 );			// １０００円未満　切り捨て
			
			// 計 (1 + 2)
			l_add( ANS[2].val, ANS[0].val, ANS[1].val );
			
			// 消費税額
			memmove( ANS[3].val, Snd.Sn_SYTXM, 6 );
			
			// その他の課税分
			memmove( ANS[4].val, Snd.Sn_SYTX, 6 );
			
			// 計 (4 + 5)
			l_add( ANS[5].val, ANS[3].val, ANS[4].val );*/
			break;
	}
	
	// 簡易 -> 貸倒回収に係る or 税額一般 -> 控除過大調整税額
	if( vers ){
		memmove( ANS[2].val, m_pSnHonpyoData->Sn_KJADJZ, 6 );
	}
	else {
		memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
		memmove( c, m_pSnTanaData->Sn_MENZZ, 6 );
		if( m_pSnTanaData->Sn_MENZsw & 0x01 ){
			if( m_Arith.l_test(c) ){
				m_Arith.l_neg(c);
			}
		}
		m_Arith.l_add( a, a, c );
		memmove( b, m_pSnHonpyoData->Sn_KJADJZ, 6 );
		if( m_Arith.l_test(a) < 0 ){
			m_Arith.l_neg( a );
			m_Arith.l_add( ANS[6].val, b, a );
		}
		else{
			memmove( ANS[6].val, b, 6 );
		}
	}
	
	// 控除税額
	// 控除対象仕入税額
	memmove( a, m_pSnHonpyoData->Sn_SIREZ, 6 );
	if( !vers ){
		m_Arith.l_add( a, a, c );
	}
	if( m_Arith.l_test(a) < 0 ){
		m_Arith.l_clear( (vers ? ANS[3].val : ANS[7].val) );
	}
	else{
		memmove( (vers ? ANS[3].val : ANS[7].val), a, 6 );
	}
	
	// 返還等対価の税額
	memmove( (vers ? ANS[4].val : ANS[8].val), m_pSnHonpyoData->Sn_HKANZ, 6 );
	
	// 貸倒れに係る税額
	memmove( (vers ? ANS[5].val : ANS[9].val), m_pSnHonpyoData->Sn_KSITZ, 6 );
	
	// 控除税額小計 vers ? (4 + 5 + 6) : (8 + 9 + 10)
	if( vers ){
		m_Arith.l_add( ANS[6].val, ANS[3].val, ANS[4].val );
		m_Arith.l_add( ANS[6].val, ANS[6].val, ANS[5].val );
	}
	else{
		m_Arith.l_add( ANS[10].val, ANS[7].val,  ANS[8].val );
		m_Arith.l_add( ANS[10].val, ANS[10].val, ANS[9].val );
	}
	
	// 限界控除税額
	memmove( (vers ? JANS[10].val : ANS[13].val), m_pSnHonpyoData->Sn_GENKAI, 6 );
	
	switch( vers&0x0f ){
		// 控除不足還付税額 (7 - 2 - 3) ･ 差引税額 (2 + 3 - 7)
		case 1 :	
			if( m_Arith.l_test(pSnFhyo10Data->Sn_1F10T) < 0 ){
				if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
					memmove( ANS[7].val, pSnFhyo10Data->Sn_1F10T, 6 );
					m_Arith.l_neg( ANS[7].val );
				}
			}
			else{
				if( m_Arith.l_test( m_pSnHonpyoData->Sn_GENKAI ) ){
					// 有り得ない！
//					memmove( a, Snd.Sn_3F3,   6 );
				}
				else{
					memmove( a, pSnFhyo10Data->Sn_1F10T, 6 );
				}
//				p_calq( (char *)ANS[8].val, (char *)a, 0x10 );	//1 00円未満切り捨て
				m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );	//1 00円未満切り捨て
			}
			break;
		case 2 :
			m_Arith.l_add( a, ANS[1].val, ANS[2].val );
			m_Arith.l_sub( a, a, ANS[6].val );
			// 限界控除前の税額がマイナスの時
			if( m_Arith.l_test(a) < 0 ){
				if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
					m_Arith.l_neg( a );
					memmove( ANS[7].val, a, 6 );
				}
			}
			else{
//				p_calq( (char *)ANS[8].val, (char *)a, 0x10 );	// 100円未満切り捨て
				m_Util.l_calq( (char *)ANS[8].val, (char *)a, 0x10 );	// 100円未満切り捨て
			}
			break;
	
				// 限界控除前の税額 (6 + 7 - 11)
		default:	
/*			l_add( a, ANS[5].val, ANS[6].val );
			l_sub( a, a, ANS[10].val );
			if( l_test( a ) < 0 )	l_clear( ANS[11].val );
			else					memmove( ANS[11].val, a, 6 );
					
			// 控除不足還付税額 (11 - 6 - 7)
			l_sub( a, ANS[10].val, ANS[5].val );
			l_sub( a, a, ANS[6].val );
			// 中間申告以外
			if( (l_test( a ) < 0 ) || ! (Snh.Sn_SKKBN % 2) )
					l_clear( ANS[12].val );
			else	memmove( ANS[12].val, a, 6 );
					
			// 差引税額 (12 - 14)
			if( ! l_test( ANS[12].val ) )
					l_sub( ANS[14].val, ANS[11].val, ANS[13].val );
			else	l_clear( ANS[14].val );
				
			// １００円未満　切り捨て
			if( l_test( ANS[14].val ) )
					p_calq( (char *)ANS[14].val, (char *)ANS[14].val, 0x10 );*/
			break;
	}
	
	// 中間納付税額
	memmove( a, m_pSnHonpyoData->Sn_TYNOFZ, 6 );
//	p_calq( (char *)(vers ? ANS[9].val : ANS[15].val), (char *)a, 0x10 );	// 100円未満切り捨て
	m_Util.l_calq( (char *)(vers ? ANS[9].val : ANS[15].val), (char *)a, 0x10 );	// 100円未満切り捨て
	
	// 納付税額 (15 - 16) ･ 中間納付還付税額
	if( vers ){
		m_Arith.l_sub( a, ANS[8].val, ANS[9].val );
//		p_calq( (char *)a, (char *)a, 0x10 );			// 100円未満切り捨て
		m_Util.l_calq( (char *)a, (char *)a, 0x10 );			// 100円未満切り捨て
		if( m_Arith.l_test( a ) < 0 ){
			m_Arith.l_neg( a );
			memmove( ANS[11].val, a, 6 );
		}
		else{
			memmove( ANS[10].val, a, 6 );
		}
	}
	else {
		if( !m_Arith.l_test( ANS[12].val ) ) {
			m_Arith.l_sub( a, ANS[14].val, ANS[15].val );
			if( m_Arith.l_test(a) < 0 ){
				m_Arith.l_clear( ANS[16].val );
			}
			else{
//				p_calq( (char *)ANS[16].val, (char *)a, 0x10 );		// 100円未満　切り捨て
				m_Util.l_calq( (char *)ANS[16].val, (char *)a, 0x10 );		// 100円未満　切り捨て
			}
		}
		else{
			m_Arith.l_clear( ANS[16].val );
		}
		
		if( m_Arith.l_test(ANS[12].val) ){
			memmove( ANS[17].val, ANS[15].val, 6 );
		}
		else{
			m_Arith.l_sub( a, ANS[15].val, ANS[14].val );
			if( m_Arith.l_test(a) < 0 ){
				m_Arith.l_clear( ANS[17].val );
			}
			else{
				memmove( ANS[17].val, a, 6 );
			}
		}
	}
	
	memmove( m_pSnHonpyoData->Sn_EDNOFZ, (vers ? ANS[10].val : ANS[16].val), 6 );
	
	// 修正申告
	if( (m_pSnHeadData->Sn_SKKBN==3) || (m_pSnHeadData->Sn_SKKBN==4) ){
		// 既確定税額
		memmove( (vers ? ANS[12].val : ANS[18].val), m_pSnHonpyoData->Sn_KAKTIZ, 6 );
		
		if( vers ){
			if( m_Arith.l_test( ANS[10].val ) > 0 ){		// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[10].val, ANS[12].val );
			}
			else{
				memmove( a, ANS[11].val, 6 );
				m_Arith.l_neg( a );
				m_Arith.l_sub( a, a, ANS[12].val );
			}
			m_Arith.l_sub( a, a, ANS[7].val );
		}
		else {
			// 差引納付税額 (17 - 19)
			if( m_Arith.l_test( ANS[12].val ) ) {
				m_Arith.l_add( a, ANS[12].val, ANS[17].val );
				m_Arith.l_add( a, a, ANS[18].val );
				m_Arith.l_neg( a );
			}
			else {
				if( m_Arith.l_test( ANS[16].val ) ){
					m_Arith.l_sub( a, ANS[16].val, ANS[18].val );
				}
				else{
					m_Arith.l_add( a, ANS[17].val, ANS[18].val );
					m_Arith.l_neg( a );
				}
			}
		}
		// １００円未満　切り捨て
//		p_calq( (char *)(vers ? ANS[13].val : ANS[19].val), (char *)a, 0x10 );
		m_Util.l_calq( (char *)(vers ? ANS[13].val : ANS[19].val), (char *)a, 0x10 );
	}
	
	// 課税資産の譲渡等の対価の額
	memmove( (vers ? ANS[14].val : ANS[20].val), m_pSnHonpyoData->Sn_KZURI, 6 );
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){	 // 基準期間の課税売上高
		memmove( (vers ? ANS[15].val : ANS[21].val), m_pSnHonpyoData->Sn_ZZURI, 6 );
	}
	else{			 // 資産の譲渡等の対価の額
		memmove( (vers ? ANS[15].val : ANS[21].val), m_pSnHonpyoData->Sn_SOURI, 6 );
	}
	
	if( !vers ){
		return;
	}
	
	// 地方消費税の計算
	// 控除不足還付税額
//	if( ! ((Snh.Sn_SKKBN&0xff) % 2) && N_ver != 2 ){
	if( 0 ){
		;	// 中間且改正日前開始
	}
	else{
		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ) {	// 中間申告以外
			if( vers == 1 ){
//				memmove( ANS[16].val, pSnFhyo10Data->Sn_1F8C, 6 );
				m_Arith.l_add( ANS[16].val, pSnFhyo10Data->Sn_1F8B, pSnFhyo10Data->Sn_1F8C );
			}
			else{
				memmove( ANS[16].val, ANS[7].val, 	6 );
			}
		}
		
		// 差引税額
		if( ((vers==1)&&(m_Arith.l_test(pSnFhyo10Data->Sn_1F10T)<0)) && !((m_pSnHeadData->Sn_SKKBN&0xff) % 2) ){
			;
		}
		else {		// 還付　中間申告以外
			if( vers == 1 ){
				if( m_Arith.l_test( m_pSnHonpyoData->Sn_GENKAI ) ){
					// 有り得ないはず
//					memmove( a, Snd.Sn_3F6,   6 );
				}
				else{
//					memmove( a, pSnFhyo10Data->Sn_1F9C, 6 );
					m_Arith.l_add( a, pSnFhyo10Data->Sn_1F9A, pSnFhyo10Data->Sn_1F9B );
					m_Arith.l_add( a, a, pSnFhyo10Data->Sn_1F9C );
				}
			}
			else{
				memmove( a, ANS[8].val, 6 );
			}
//			p_calq( (char *)ANS[17].val, (char *)a, 0x10 );		// １００円未満　切り捨て
			m_Util.l_calq( (char *)ANS[17].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
		
		// 譲渡割額還付額 ･ 譲渡割額納税額
/*- '14.03.07 -*/
//		m_Arith.l_input( b, "250" );
//		if( m_Arith.l_test( ANS[16].val ) ){
//			m_Arith.l_mul100( (unsigned char *)ANS[18].val, (unsigned char *)ANS[16].val, (unsigned char *)b, 1, 0 );
//		}
//		if( m_Arith.l_test( ANS[17].val ) ){
//			m_Arith.l_mul100( (unsigned char *)a, (unsigned char *)ANS[17].val, (unsigned char *)b, 1, 0 );
////			p_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
//			m_Util.l_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
//		}
/*-------------*/
		if( vers == 1 ){	// 経過措置対象

			memset( a, '\0', sizeof(a) );
			memset( b, '\0', sizeof(b) );
			memset( c, '\0', sizeof(c) );
			memset( d, '\0', sizeof(d) );

			m_Util.percent( a, (char*)pSnFhyo10Data->Sn_1F8B, 7, 25, 0 );
			m_Util.percent( b, (char*)pSnFhyo10Data->Sn_1F8C, 8, 17, 0 );
			m_Util.percent( c, (char*)pSnFhyo10Data->Sn_1F9B, 7, 25, 0 );
			m_Util.percent( d, (char*)pSnFhyo10Data->Sn_1F9C, 8, 17, 0 );

			// 15-14
			char	sagaku1[6]={0}, sagaku2[6]={0};
			m_Arith.l_sub( sagaku1, c, a );
			m_Arith.l_sub( sagaku2, d, b );

			// 16
			char	goukei[6]={0};
			m_Arith.l_add( goukei, sagaku1, sagaku2 );
			if( m_Arith.l_test(goukei) > 0 ){
				m_Util.l_calq( (char *)ANS[19].val, (char *)goukei, 0x10 );	// １００円未満　切り捨て
			}
			else{
				if( m_pSnHeadData->Sn_SKKBN != 4 ){
					m_Arith.l_neg( goukei );
					memmove( ANS[18].val, goukei, 6 );
				}
			}
		}
		else{				// 経過措置対象外
			if( m_Arith.l_test(ANS[16].val) ){
				m_Util.percent( (char*)ANS[18].val, (char*)ANS[16].val, 8, 17, 0 );
			}
			if( m_Arith.l_test( ANS[17].val ) ){
				m_Util.percent( a, (char*)ANS[17].val, 8, 17, 0 );
				m_Util.l_calq( (char *)ANS[19].val, (char *)a, 0x10 );	// １００円未満　切り捨て
			}
		}
/*-------------*/
		
		// 中間納付譲渡割額
		if( (m_pSnHeadData->Sn_SKKBN&0xff) % 2 ){	// 中間申告以外
//			p_calq( (char *)ANS[20].val, (char *)Snd.Sn_TTYWAR, 0x10 );	// 100円未満切り捨て
			m_Util.l_calq( (char *)ANS[20].val, (char *)m_pSnHonpyoData->Sn_TTYWAR, 0x10 );	// 100円未満切り捨て
		}
		
		// 納付譲渡割額 (20 - 21) ･ 中間還付譲渡割額 (21 - 20)
		m_Arith.l_sub( a, ANS[19].val, ANS[20].val );
		if( m_Arith.l_test(a) < 0 ){
			m_Arith.l_neg( a );
//			p_calq( (char *)ANS[22].val, (char *)a, 0x10 );		// １００円未満　切り捨て
			m_Util.l_calq( (char *)ANS[22].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
		else{
//			p_calq( (char *)ANS[21].val, (char *)a, 0x10 );	// １００円未満　切り捨て
			m_Util.l_calq( (char *)ANS[21].val, (char *)a, 0x10 );	// １００円未満　切り捨て
		}
		
		// 修正申告
		if( (m_pSnHeadData->Sn_SKKBN==3) || (m_pSnHeadData->Sn_SKKBN==4) ){
			// 既確定税額
			memmove( ANS[23].val, m_pSnHonpyoData->Sn_TKAKTZ, 6 );
			
			// 差引納付譲渡割額
			if( m_Arith.l_test( ANS[21].val ) > 0 ){	// 納付税額が有る場合
				m_Arith.l_sub( a, ANS[21].val, ANS[23].val );
			}
			else{
				m_Arith.l_add( a, ANS[18].val, ANS[22].val );
				if( m_Arith.l_test( a ) ){
					m_Arith.l_neg( a );
				}
				m_Arith.l_sub( a, a, ANS[23].val );
			}
//			p_calq( (char *)ANS[24].val, (char *)a, 0x10 );		// １００円未満　切り捨て
			m_Util.l_calq( (char *)ANS[24].val, (char *)a, 0x10 );		// １００円未満　切り捨て
		}
	}
	
	// 消費税及び地方消費税の合計（納付又は還付）税額
	switch( m_pSnHeadData->Sn_SKKBN&0xff ){
		case 1 : case 2 :		// 確定・中間申告
			m_Arith.l_add( a, ANS[10].val, ANS[21].val );
			m_Arith.l_add( b, ANS[7].val,  ANS[11].val );
			m_Arith.l_add( b, b, ANS[18].val );
			m_Arith.l_add( b, b, ANS[22].val );
			m_Arith.l_sub( ANS[25].val, a, b );
			break;
		default	:				// 修正申告
			m_Arith.l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}

	// 国税の既確定税額の算出
	if( m_Arith.l_test( ANS[7].val ) > 0 ){
		m_Arith.l_neg( ANS[7].val );
	}
	m_Arith.l_add( WORK, ANS[7].val, ANS[8].val );
	m_Arith.l_sub( WORK, WORK, ANS[9].val );
	memmove( m_pSnHonpyoData->Sn_KAKTIZ, WORK, 6 );

	// 地方税の既確定譲渡割額の算出
	if( m_Arith.l_test( ANS[18].val ) > 0 ){
		m_Arith.l_neg( ANS[18].val );
	}
	m_Arith.l_add( WORK, ANS[18].val, ANS[19].val );
	m_Arith.l_sub( WORK, WORK, ANS[20].val );
	memmove( m_pSnHonpyoData->Sn_TKAKTZ, WORK, 6 );

//--> '14.05.15 INS START
	if( m_pZmSub ){
		CString	szFilter;
		szFilter.Format( _T("Sn_SEQ=%d"), m_SnSeq );
		if( m_pZmSub->szsnk == NULL ){
			m_pZmSub->SzsnkOpen( szFilter );
		}
		else{
			m_pZmSub->szsnk->Requery( szFilter );
		}
		if( m_pZmSub->szsnk->st != -1 ){
			char	monyBuf1[32]={0}, monyBuf2[32]={0};
			m_pZmSub->szsnk->Edit();
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_KAKTIZ, m_pSnHonpyoData->Sn_KAKTIZ );
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_TKAKTZ, m_pSnHonpyoData->Sn_TKAKTZ );
			m_pZmSub->szsnk->Update();
		}
	}
//<-- '14.05.15 INS END
}

//-----------------------------------------------------------------------------
// 棚卸調整額の連動
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::CalqStatckSyz()
{
//--> '14.06.28 INS START
	if( m_pSnTanaData == NULL ){
		if( m_pSnHeadData->IsMiddleProvisional() == FALSE ){
			EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
			if( KzHoushiki == ID_ICSSH_GENSOKU ){
				if( (m_pSnTanaData = new CH26SnTanaData(m_pSnHeadData->SVmzsw ? TRUE:FALSE)) != NULL ){
					if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
		}
	}
//<-- '14.06.28 INS END

	if( m_pSnTanaData ){
//2016.02.23 UPDATE START
//		m_pSnTanaData->ConnectZmdata( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );

		if( m_Syokai_NukiKomi == TRUE ){
			m_pSnTanaData->ConnectZmdata( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
			m_Syokai_NukiKomi = FALSE;
		}else{
			unsigned char tmp_sgn = m_pSnHeadData->SVsign;
			m_pSnTanaData->ConnectZmdata( m_pZmSub, m_pSnHeadData, m_pSyzSyukei );
			m_pSnHeadData->SVsign = tmp_sgn;
		}
//2016.02.23 UPDATE END

//--> '14.06.26 INS START
		if( (m_pSnTanaData->Sn_MENZsw&0x10) && m_pSnHeadData->IsKobetuSiireAnbun() ){
//-- '16.05.30 --
//			if( m_pSyzSyukei && (m_pSnTanaData->Sn_tansw&0x01)==0x00 ){
//				char	mony[6]={0};
//				// 課税売上げに要する
//				CString	tag, chtag;
//				MoneyBasejagArray	shinkoku;
//				memset( mony, '\0', sizeof(mony) );
//				shinkoku = m_pSyzSyukei->GetShinkokuData( "021401" );
//				tag = _T("AYB00040");
//				GetMony( tag, chtag, mony );
//				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
//					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
//				}
//				else{
//					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
//				}
//				m_pSyzSyukei->SetShinkokuData( "021401", mony );
//
//				memset( mony, '\0', sizeof(mony) );
//				shinkoku = m_pSyzSyukei->GetShinkokuData( "021402" );
//				tag = _T("AYB00100");
//				GetMony( tag, chtag, mony );
//				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
//					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
//				}
//				else{
//					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
//				}
//				m_pSyzSyukei->SetShinkokuData( "021402", mony );
//
//				memset( mony, '\0', sizeof(mony) );
//				shinkoku = m_pSyzSyukei->GetShinkokuData( "121403" );
//				tag = _T("AYB00160");
//				GetMony( tag, chtag, mony );
//				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
//					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
//				}
//				else{
//					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
//				}
//				m_pSyzSyukei->SetShinkokuData( "121403", mony );
//
//				// 共通に要する
//				memset( mony, '\0', sizeof(mony) );
//				shinkoku = m_pSyzSyukei->GetShinkokuData( "021501" );
//				tag = _T("AYB00060");
//				GetMony( tag, chtag, mony );
//				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
//					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
//				}
//				else{
//					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
//				}
//				m_pSyzSyukei->SetShinkokuData( "021501", mony );
//
//				memset( mony, '\0', sizeof(mony) );
//				shinkoku = m_pSyzSyukei->GetShinkokuData( "021502" );
//				tag = _T("AYB00120");
//				GetMony( tag, chtag, mony );
//				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
//					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
//				}
//				else{
//					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
//				}
//				m_pSyzSyukei->SetShinkokuData( "021502", mony );
//
//				memset( mony, '\0', sizeof(mony) );
//				shinkoku = m_pSyzSyukei->GetShinkokuData( "121503" );
//				tag = _T("AYB00180");
//				GetMony( tag, chtag, mony );
//				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
//					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
//				}
//				else{
//					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
//				}
//				m_pSyzSyukei->SetShinkokuData( "121503", mony );
//			}
//---------------	
			static	char	*H28_FHYO2_SIIRE_CODE[] = { _T("221601"), _T("221602"), _T("221603"), _T("221701"), _T("221702"), _T("221703") };
			static	char	*H26_FHYO2_SIIRE_CODE[] = { _T("021401"), _T("021402"), _T("121403"), _T("021501"), _T("021502"), _T("121503") };

			if( m_pSyzSyukei && (m_pSnTanaData->Sn_tansw&0x01)==0x00 ){

				char	**SIIRE_CODE;
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					SIIRE_CODE =H28_FHYO2_SIIRE_CODE ;
				}
				else{
					SIIRE_CODE = H26_FHYO2_SIIRE_CODE;
				}

				char	mony[6]={0};
				// 課税売上げに要する
				CString	tag, chtag;
				MoneyBasejagArray	shinkoku;
				memset( mony, '\0', sizeof(mony) );
				shinkoku = m_pSyzSyukei->GetShinkokuData( SIIRE_CODE[0] );
				tag = _T("AYB00040");
				GetMony( tag, chtag, mony );
				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
				}
				else{
					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
				}
				m_pSyzSyukei->SetShinkokuData( SIIRE_CODE[0], mony );

				memset( mony, '\0', sizeof(mony) );
				shinkoku = m_pSyzSyukei->GetShinkokuData( SIIRE_CODE[1] );
				tag = _T("AYB00100");
				GetMony( tag, chtag, mony );
				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
				}
				else{
					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
				}
				m_pSyzSyukei->SetShinkokuData( SIIRE_CODE[1], mony );

				memset( mony, '\0', sizeof(mony) );
				shinkoku = m_pSyzSyukei->GetShinkokuData( SIIRE_CODE[2] );
				tag = _T("AYB00160");
				GetMony( tag, chtag, mony );
				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
				}
				else{
					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
				}
				m_pSyzSyukei->SetShinkokuData( SIIRE_CODE[2], mony );

				// 共通に要する
				memset( mony, '\0', sizeof(mony) );
				shinkoku = m_pSyzSyukei->GetShinkokuData( SIIRE_CODE[3] );
				tag = _T("AYB00060");
				GetMony( tag, chtag, mony );
				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
				}
				else{
					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
				}
				m_pSyzSyukei->SetShinkokuData( SIIRE_CODE[3], mony );

				memset( mony, '\0', sizeof(mony) );
				shinkoku = m_pSyzSyukei->GetShinkokuData( SIIRE_CODE[4] );
				tag = _T("AYB00120");
				GetMony( tag, chtag, mony );
				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
				}
				else{
					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
				}
				m_pSyzSyukei->SetShinkokuData( SIIRE_CODE[4], mony );

				memset( mony, '\0', sizeof(mony) );
				shinkoku = m_pSyzSyukei->GetShinkokuData( SIIRE_CODE[5] );
				tag = _T("AYB00180");
				GetMony( tag, chtag, mony );
				if( m_pSnTanaData->Sn_MENZsw&0x01 ){
					m_Arith.l_sub( mony, shinkoku[0][0].arith, mony );
				}
				else{
					m_Arith.l_add( mony, shinkoku[0][0].arith, mony );
				}
				m_pSyzSyukei->SetShinkokuData( SIIRE_CODE[5], mony );
			}
//---------------
		}
//<-- '14.06.26 INS END
	}

	return 0;
}

//-----------------------------------------------------------------------------
// タブタイトルの切替
//-----------------------------------------------------------------------------
// 引数	type	：	
//-----------------------------------------------------------------------------
void CH26HyoView::ChengTabTitle( char type )
{
	CString	title;
	if( type&0x04 ){
		title = _T("更正の申出");
	}
	else{
		title = _T("更正の請求");
	}
	if( m_pSnHeadData->IsSoleProprietor() ){
		title += _T("(個人)");
	}
	else{
		title += _T("(法人)");
	}

	int max = (int)m_TabMng.GetCount();
	for( int i=0; i<max; i++ ){
		if( (m_TabMng[i].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[i].tabtype==ID_ICSSH_KSK_KJ_TAB) ){
			m_MainTab.SetCaption( i, title );
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
int CH26HyoView::RenewSyzShin( SH_PASTINFO *pPastInfo )
{
	if( m_pSnChskData == NULL ){
		m_pSnChskData = new CH26SnChskData();
		if( m_pSnChskData->GetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
	}

//--> '17.06.07 INS START
	if( m_pSnHonpyoData == NULL ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( (m_pSnHonpyoData = new CH28SnHonpyoData()) == NULL ){
				return -1;
			}
			if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( m_SixKindFlg ){
				if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
					return -1;
				}
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( (m_pSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
					return -1;
				}
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
		}
	}
//<-- '17.06.07 INS END

//--> '14.05.15 INS START
	CString	szFilter;
	szFilter.Format( _T("Sn_SEQ=%d"), m_SnSeq );
	if( m_pZmSub->szsnk ){
		m_pZmSub->szsnk->Requery( szFilter );
	}
	else{
		m_pZmSub->SzsnkOpen( szFilter );
	}
//<-- '14.05.15 INS END


	ASSERT( pPastInfo );
	if( pPastInfo == NULL ){
		return -1;
	}
	ASSERT( m_pSnHeadData );
	if( m_pSnHeadData == NULL ){
		return -1;
	}
	ASSERT( m_pSnHonpyoData );
	if( m_pSnHonpyoData == NULL ){
		return -1;
	}

	char	MYmds[4] = {0};
	char	MYmde[4] = {0};
	char	Date[4] = {0};

	int Msno = 0;
	int Meno = 0;
	int McNt = 0;
	char	SKKBN = pPastInfo->SKKBN;
	char	MCOUNT = pPastInfo->MCOUNT;

	switch( m_pSnHeadData->Sn_MCOUNT ){
		case 3 :
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
	if(	(m_pSnHeadData->Sn_SKKBN%2) ){
		m_Arith.l_add( m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnHonpyoData->Sn_KZURI );
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			// 非課税輸出売上を課税売上より差引く
			if( m_pSnFhyo20Data ){
				m_Arith.l_sub( m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo20Data->Sn_2F3C );
			}
			else if( m_pSnFhyo22Data ){
				m_Arith.l_sub( m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo22Data->Sn_2F3C );
			}
		}
	}
	
	// 前回中間申告  中間納付 <-- 最終納付
	if( !(SKKBN%2) ){
		// 課税売上高を累計
		m_Arith.l_add( m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnHonpyoData->Sn_KZURI );
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			// 非課税輸出売上を課税売上より差引く
			if( m_pSnFhyo20Data ){
				m_Arith.l_sub( m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo20Data->Sn_2F3C );
			}
			else if( m_pSnFhyo22Data ){
				m_Arith.l_sub( m_pSnHeadData->Sn_WURI, m_pSnHeadData->Sn_WURI, m_pSnFhyo22Data->Sn_2F3C );
			}
		}
	}


	m_Arith.l_clear( m_pSnHonpyoData->Sn_KSTDM );							//  課税標準額（自動車）
	m_Arith.l_clear( m_pSnHeadData->Sn_CK45 );								//			〃			(4.5%)
	m_Arith.l_clear( m_pSnHeadData->Sn_CK60 );								//			〃			( 6% )
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KSTD );							//  　　〃　　（その他）
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TIKAM );							//  対価の返還等をした金額（自動車）
	m_Arith.l_clear( m_pSnHeadData->Sn_CH45 );								//				〃					(4.5%)
	m_Arith.l_clear( m_pSnHeadData->Sn_CH60 );								//				〃					( 6% )
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TIKA );							//  　　　　　〃　　　　　（その他）
	m_Arith.l_clear( m_pSnHonpyoData->Sn_SYTXM );							//  消費税額（自動車）
	m_Arith.l_clear( m_pSnHeadData->Sn_CZ45 );								//		　〃			(4.5%)
	m_Arith.l_clear( m_pSnHeadData->Sn_CZ60 );								//		　〃			( 6% )
	m_Arith.l_clear( m_pSnHonpyoData->Sn_SYTX );							//  　　〃　（その他）
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KJADJZ );							//  控除過大調整税額
	m_Arith.l_clear( m_pSnHonpyoData->Sn_SIREZ );							//  控除対象仕入税額
	if( m_pSnTanaData ){
		m_pSnTanaData->Sn_MENZsw = 0x00;
		m_Arith.l_clear( m_pSnTanaData->Sn_MENZK );							//	免税 控除対象仕入額
		m_Arith.l_clear( m_pSnTanaData->Sn_MENZZ );							//	免税 控除対象仕入税額
		m_Arith.l_clear( m_pSnTanaData->Sn_MNK40 );							//	免税 控除対象仕入額(5%分)
		m_Arith.l_clear( m_pSnTanaData->Sn_MNZ40 );							//	免税 控除対象仕入税額(5%分)	
		m_Arith.l_clear( m_pSnTanaData->Sn_MNK63 );							//[Y]
        m_Arith.l_clear( m_pSnTanaData->Sn_MNZ63 );							//[Y]
	}
	m_Arith.l_clear( m_pSnHonpyoData->Sn_HKANZ );							//  返還等対価に係る税額
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KSITZ );							//  貸倒れに係る税額

	if( m_pSnHeadData->IsKakuteiShinkoku() && m_pSnHeadData->IsKikanTansyuku() ){
		m_Arith.l_clear( m_pSnHonpyoData->Sn_TYNOFZ );						//  中間納付税額
		m_Arith.l_clear( m_pSnHonpyoData->Sn_TTYWAR );						//  中間納付税額		(地方消費税用)
	}

/*- '14.05.15 -*/
//	if( ( (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && pPastInfo->MINASHI && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
//		( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
//		( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) ) ){
/*-------------*/
//2018.03.13 UPDATE START
//	if( ( (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && pPastInfo->MINASHI && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
//		( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
//		( (pPastInfo->SKKBN==4/*中間修正*/) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN) ) || 
//		( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) ) ){

	if( ( ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) && pPastInfo->MINASHI && ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku() ) || 
		( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku() ) || 
		( (pPastInfo->SKKBN==4/*中間修正*/) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN) ) || 
		( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) ) ){
//2018.03.13 UPDATE END
/*-------------*/
		m_Arith.l_clear( m_pSnHonpyoData->Sn_KAKTIZ );						//  既確定税額
		m_Arith.l_clear( m_pSnHonpyoData->Sn_TKAKTZ );						//	地方消費税　既確定譲渡割額
	}

	// 前回確定申告
	if( SKKBN%2 ){
		m_Arith.l_clear( m_pSnHeadData->Sn_TYNF1 );							//	中間納付税額　１回目
		m_Arith.l_clear( m_pSnHeadData->Sn_TYNF2 );							//	中間納付税額　２回目
		m_Arith.l_clear( m_pSnHeadData->Sn_TYNF3 );							//	中間納付税額　３回目
		m_Arith.l_clear( m_pSnHeadData->Sn_TYNF4 );							//	中間納付税額　４回目
		m_Arith.l_clear( m_pSnHeadData->Sn_TTYNF1 );						//	中間納付税額　１回目(地方消費税用)
		m_Arith.l_clear( m_pSnHeadData->Sn_TTYNF2 );						//	中間納付税額　２回目(地方消費税用)
		m_Arith.l_clear( m_pSnHeadData->Sn_TTYNF3 );						//	中間納付税額　３回目(地方消費税用)
		m_Arith.l_clear( m_pSnHeadData->Sn_TTYNF4 );						//	中間納付税額　４回目(地方消費税用)
				 
		for( int i=0; i<11; i++ ){
			memset( m_pSnHeadData->Sn_TYNF[i], '\0', 6 );
			memset( m_pSnHeadData->Sn_TTNF[i], '\0', 6 );
		}

//		memset( TBhead.SVtmth, '\0', 12 );
//		m_Middle = 0;
		
//->shimizu外に出す
//		m_Arith.l_clear( m_pSnChskData->Sn_KTNFZ );							// 消費税及び地方消費税の合計納付税額
//		m_pSnChskData->Sn_BSMON = 0x00;										// 月換算	分子
//		m_pSnChskData->Sn_BBMON = 0x00;										// 〃　　　 分母
//<-
	}
//->shimizu
	if( m_pSnChskData ){
		m_Arith.l_clear( m_pSnChskData->Sn_KTNFZ );							// 消費税及び地方消費税の合計納付税額
		m_pSnChskData->Sn_BSMON = 0x00;										// 月換算	分子
		m_pSnChskData->Sn_BBMON = 0x00;										// 〃　　　 分母
	}
//<-
	m_Arith.l_clear( m_pSnHonpyoData->Sn_EDNOFZ );							//  最終納付税額
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KZURI );							//  課税売上高
	m_Arith.l_clear( m_pSnHonpyoData->Sn_SOURI );							//  総売上高
	
	m_pSnHeadData->Sn_TOKUR = 0;											//	特例計算適用ＳＷ
	m_pSnHonpyoData->Sn_UP1 = 0;											//	第１種事業　売上割合
	m_pSnHonpyoData->Sn_UP2 = 0;											//	第２種事業　売上割合
	m_pSnHonpyoData->Sn_UP3 = 0;											//	第３種事業　売上割合
	m_pSnHonpyoData->Sn_UP4 = 0;											//	第４種事業　売上割合
	m_pSnHonpyoData->Sn_UP5 = 0;											//	第５種事業　売上割合
//-- '15.02.23 --
//	((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_UP6 = 0;						//	第６種事業　売上割合[Y]
//---------------
	if( m_SixKindFlg ){
		((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_UP6 = 0;					//	第６種事業　売上割合[Y]
	}
//---------------
//--> '15.08.19 INS START
//	if( m_TokuteiFlg ){
	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6 );	//	第６種事業　課税売上
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_KSJT3 );	//　課税資産の譲渡等の対価の額
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_KSJT4 );
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_KSJT63 );
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_KSJTKEI );
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_TKST63 );	// 特定課税仕入に係る支払対価の額
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_TKSTKEI );
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_URHKNZ );	// 売上の返還等対価に係る税額
		m_Arith.l_clear( ((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_TKHKNZ );	// 特定課税仕入の返還等対価に係る税額
	}
//<-- '15.08.19 INS END
	m_pSnHeadData->Sn_TUMIAG = 0;											//	規則22条 1項適用ＳＷ
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KURI1 );							//	第１種事業　課税売上
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KURI2 );							//	第２種事業　課税売上
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KURI3 );							//	第３種事業　課税売上
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KURI4 );							//	第４種事業　課税売上
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KURI5 );							//	第５種事業　課税売上
//-- '15.02.23 --
//	m_Arith.l_clear( ((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6 );		//	第６種事業　課税売上[Y]
//---------------

	m_Arith.l_clear( m_pSnHeadData->Sn_KURIA );								//	事業別課税売上高合計
	m_Arith.l_clear( m_pSnHonpyoData->Sn_GENKAI );							//	限界控除税額
	m_Arith.l_clear( m_pSnHonpyoData->Sn_WGENKA );							//	限界控除税額(手入力額)
	
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TEDNOF );							//	地方消費税　最終納付税額

	m_pSnHeadData->SVzchg = 0;												//	免税⇔課税切り替えサイン
//	m_pSnHeadData->SV_5sw = 0;												//	第５種事業に係る経過措置に関するｓｗ
	m_pSnHeadData->SVmzsw = 0;												//	複数税率処理ｓｗ
//	curset.SVmzsw = 0;
	m_Arith.l_clear( m_pSnHeadData->SVtval1 );								//	調整課税仕入等に係る棚卸資産額
	m_Arith.l_clear( m_pSnHeadData->SVtval2 );								//	調整課税仕入等に係る棚卸消費税額

//--> '14.05.15 INS START
	if( m_pZmSub->szsnk->st != -1 ){
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
		if( m_pSnHeadData->IsKakuteiShinkoku() && m_pSnHeadData->IsKikanTansyuku() ){
			m_pZmSub->szsnk->Sn_TYNOFZ = _T("0");
			m_pZmSub->szsnk->Sn_TTYWAR = _T("0");
		}

/*- '14.05.15 -*/
//		if( ( (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && pPastInfo->MINASHI && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
//			( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
//			( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) ) ){
/*-------------*/
//2018.03.13 UPDATE START
		//if( ( (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && pPastInfo->MINASHI && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
		//	( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) && m_pSnHeadData->IsKikanTansyuku() ) || 
		//	( (pPastInfo->SKKBN==4/*中間修正*/) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN) ) || 
		//	( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI) ) ){

		if( ( ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) && pPastInfo->MINASHI && ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku() ) || 
			( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) && m_pSnHeadData->IsKikanTansyuku() ) || 
			( (pPastInfo->SKKBN==4/*中間修正*/) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN) ) || 
			( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && ((m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI)||(m_pSnHeadData->GetKubun()==ID_ICSSH_JUNKAKUTEI)) ) ){
//2018.03.13 UPDATE END
/*-------------*/

			m_pZmSub->szsnk->Sn_KAKTIZ = _T("0");
			m_pZmSub->szsnk->Sn_TKAKTZ = _T("0");
		}

		// 前回確定申告
		if( SKKBN%2 ){
			for(int i = 0; i < 11; i++ ) {
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
//<-- '14.05.15 INS END
	
	if( m_pSnFhyo10Data ){													//	付表　集計エリア
		//------>yoshida1500811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->ClearDataByRenew( m_pZmSub, 0 );
		}
		else{
			m_pSnFhyo10Data->ClearDataByRenew( m_pZmSub, 0 );
		}
		//----------------------
//		m_pSnFhyo10Data->ClearDataByRenew( m_pZmSub, 0 );
		//<---------------------
	}
	if( m_pSnFhyo40Data ){

		//[Y]150119
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo10Data*)m_pSnFhyo40Data)->ClearDataByRenew( m_pZmSub, 0 );
		}
		else{
			m_pSnFhyo40Data->ClearDataByRenew( m_pZmSub, 0 );
		}

//		m_pSnFhyo40Data->ClearDataByRenew( m_pZmSub, 0 );
	}
	if( m_pSnFhyo20Data ){
		//------>yoshida1500811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//2018.03.13 UPDATE START
//			if( (pPastInfo->SKKBN==ID_ICSSH_KAKUTEI) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
			if( ((pPastInfo->SKKBN==ID_ICSSH_KAKUTEI)||(pPastInfo->SKKBN==ID_ICSSH_JUNKAKUTEI)) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
//2018.03.13 UPDATE END
				((CH28SnFhyo20Data*)m_pSnFhyo20Data)->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			else if( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN_SYUUSEI) ){ //前回が中間で今回が中間修正の場合
				((CH28SnFhyo20Data*)m_pSnFhyo20Data)->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			else{
				((CH28SnFhyo20Data*)m_pSnFhyo20Data)->ClearDataByRenew( m_pZmSub, 0 , 0 );
			}
		}
		else{
//2018.03.13 UPDATE START
//			if( (pPastInfo->SKKBN==ID_ICSSH_KAKUTEI) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
			if( ((pPastInfo->SKKBN==ID_ICSSH_KAKUTEI)||(pPastInfo->SKKBN==ID_ICSSH_JUNKAKUTEI)) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
//2018.03.13 UPDATE END
				m_pSnFhyo20Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			else if( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN_SYUUSEI) ){ //前回が中間で今回が中間修正の場合
				m_pSnFhyo20Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			else{
				m_pSnFhyo20Data->ClearDataByRenew( m_pZmSub, 0 , 0 );
			}
		}
		//---------------------
		/*
		if( (pPastInfo->SKKBN==ID_ICSSH_KAKUTEI) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
			m_pSnFhyo20Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
		}
		else if( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN_SYUUSEI) ){ //前回が中間で今回が中間修正の場合
			m_pSnFhyo20Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
		}
		else{
			m_pSnFhyo20Data->ClearDataByRenew( m_pZmSub, 0 , 0 );
		}
		*/
		//<----------------------
	}
	if( m_pSnFhyo22Data ){
		//------>yoshida1500811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//2018.03.13 UPDATE START
//			if( (pPastInfo->SKKBN==ID_ICSSH_KAKUTEI) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
			if( ((pPastInfo->SKKBN==ID_ICSSH_KAKUTEI)||(pPastInfo->SKKBN==ID_ICSSH_JUNKAKUTEI)) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
//2018.03.13 UPDATE END
				((CH28SnFhyo20Data*)m_pSnFhyo22Data)->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			if( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN_SYUUSEI) ){ //前回が中間で今回が中間修正の場合
				((CH28SnFhyo20Data*)m_pSnFhyo22Data)->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			else{
				((CH28SnFhyo20Data*)m_pSnFhyo22Data)->ClearDataByRenew( m_pZmSub, 0 , 0 );
			}

		}
		else{
//2018.03.13 UPDATE START
//			if( (pPastInfo->SKKBN==ID_ICSSH_KAKUTEI) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
			if( ((pPastInfo->SKKBN==ID_ICSSH_KAKUTEI)||(pPastInfo->SKKBN==ID_ICSSH_JUNKAKUTEI)) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
//2018.03.13 UPDATE END
				m_pSnFhyo22Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			if( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN_SYUUSEI) ){ //前回が中間で今回が中間修正の場合
				m_pSnFhyo22Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
			}
			else{
				m_pSnFhyo22Data->ClearDataByRenew( m_pZmSub, 0 , 0 );
			}
		}
		//----------------------
		/*
		if( (pPastInfo->SKKBN==ID_ICSSH_KAKUTEI) && (m_pSnHeadData->GetKubun()==ID_ICSSH_KAKUTEI_SYUUSEI) ){ //前回が確定で今回が確定修正の場合
			m_pSnFhyo22Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
		}
		if( (pPastInfo->SKKBN==ID_ICSSH_TYUUKAN) && (m_pSnHeadData->GetKubun()==ID_ICSSH_TYUUKAN_SYUUSEI) ){ //前回が中間で今回が中間修正の場合
			m_pSnFhyo22Data->ClearDataByRenew( m_pZmSub, 0 , 0x01 );
		}
		else{
			m_pSnFhyo22Data->ClearDataByRenew( m_pZmSub, 0 , 0 );
		}
		*/
		//<----------------------
	}
	if( m_pSnFhyo50Data ){
		//[Y]150119
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo50Data*)m_pSnFhyo50Data)->ClearDataByRenew( m_pZmSub, 0 );
		}
		else{
			m_pSnFhyo50Data->ClearDataByRenew( m_pZmSub, 0 );
		}
	//	m_pSnFhyo50Data->ClearDataByRenew( m_pZmSub, 0 );
	}
	if( m_pSnFhyo52Data ){
		//[Y]150119
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo50Data*)m_pSnFhyo52Data)->ClearDataByRenew( m_pZmSub, 0 );
		}
		else{
			m_pSnFhyo52Data->ClearDataByRenew( m_pZmSub, 0 );
		}
//		m_pSnFhyo52Data->ClearDataByRenew( m_pZmSub, 0 );
	}
//2018.03.13 INSERT START
	if( m_pSnFhyo60Data ){
//		m_pSnFhyo60Data->ClearDataByRenew( m_pZmSub, 0 ); データ引継のため
	}
//2018.03.13 INSERT END
	
	// 個別対応時の棚卸調整額
	if( m_pSnTanaData ){
		m_pSnTanaData->Sn_tansw = 0x00;
		for( int i=0; i!=3; i++ ){
			memset( m_pSnTanaData->Sn_mnk63[i], '\0', 6 );//[Y]
			memset( m_pSnTanaData->Sn_mnz63[i], '\0', 6 );//[Y]
			memset( m_pSnTanaData->Sn_mnk40[i], '\0', 6 );
			memset( m_pSnTanaData->Sn_mnz40[i], '\0', 6 );
			memset( m_pSnTanaData->Sn_menzk[i], '\0', 6 );
			memset( m_pSnTanaData->Sn_menzz[i], '\0', 6 );
		}
	}
	/*1・2の内訳3行目*/
//	m_pSnHonpyoData->Sn_TS_KUBUN1 = 0;
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TS_TDM1 );
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TS_SYT1 );
	/*17・18の内訳3行目*/
//	m_pSnHonpyoData->Sn_TS_KUBUN1 = 0;
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TS_SYT2 );

	// データ更新

	//------>yoshida1500811
//	if( m_TokuteiFlg == TRUE ){
	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//		((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
//---------------
		((CH28SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
//---------------
	}
	else{
		if(m_SixKindFlg == TRUE){
			((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
		}
	}
	//------------------------
	/*
	//------>[Y]150106
	if(m_SixKindFlg == TRUE){
		((CH27SnHonpyoData*)m_pSnHonpyoData)->SetData( m_pZmSub, m_SnSeq );
	}
	else{
		m_pSnHonpyoData->SetData( m_pZmSub, m_SnSeq );
	}
	//<------------------
	*/
	//<-------------------------

	m_pSnChskData->SetData( m_pZmSub, m_SnSeq );

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
//------>特定課税仕入対応yoshida150713
int CH26HyoView::ReInitDisp( BOOL isCompChange, BOOL SixKindFlg/*=FALSE*/, EnumIdIcsShRevType shrevtype/*=ID_ICSSH_NO_REVTYPE*/ )
//--------------------------------
//int CH26HyoView::ReInitDisp( BOOL isCompChange , BOOL SixKindFlg )
//<-------------------------------
//int CH26HyoView::ReInitDisp( BOOL isCompChange )
{
//--> '15.02.19 INS START
	BOOL	svSixKindFlg = m_SixKindFlg;
//<-- '15.02.19 INS END

	m_SixKindFlg = SixKindFlg;	//[Y]150106

//------>特定課税仕入対応yoshida150713
	EnumIdIcsShRevType	svShrevtype = m_ShRevType;
	m_ShRevType = shrevtype;
//<-------------------------------


	BOOL	bRecreateTab = FALSE;
	if( isCompChange ){
		bRecreateTab = TRUE;
	}
	else{
		if( m_pSnHeadData ){
			if( m_pSnHeadData->SVmzsw&0x01 ){
//-- '15.02.18 --
//				if( (m_pFhyo52==NULL) && (m_pFhyo22==NULL) ){
//					bRecreateTab = TRUE;
//				}
//---------------

				//----->yoshida150811
				if( (m_pFhyo52==NULL) && (m_pFhyo22==NULL) && (m_pFhyo52EX==NULL) && (m_pFhyo22EX==NULL) ){
					bRecreateTab = TRUE;
				}
				//-------------------
				/*
				if( (m_pFhyo52==NULL) && (m_pFhyo22==NULL) && (m_pFhyo52EX==NULL) ){
					bRecreateTab = TRUE;
				}*/
				//--------------------

//---------------
			}
			else{
//-- '15.02.18 --
//				if( m_pFhyo52 || m_pFhyo22 ){
//					bRecreateTab = TRUE;
//				}
//---------------
				//----->yoshida150811
				if( m_pFhyo52 || m_pFhyo22 || m_pFhyo52EX || m_pFhyo22EX ){
					bRecreateTab = TRUE;
				}
				//-------------------
				/*
				if( m_pFhyo52 || m_pFhyo22 || m_pFhyo52EX ){
					bRecreateTab = TRUE;
				}
				*/
				//<-------------------
//---------------
			}
		}
//--> '15.02.19 INS START
		if( svSixKindFlg != SixKindFlg ){
			bRecreateTab = TRUE;
		}
//<-- '15.02.19 INS END

		//------>特定課税仕入対応yoshida150713
		if( svShrevtype != shrevtype ){
			bRecreateTab = TRUE;
		}
		//<------------------------------


	}

	if( bRecreateTab ){
		InitTab( TRUE );
		//-->追加 harino ('14.03.05)
		PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセット
		//<--追加 harino ('14.03.05)
	}
	else{
		if( m_pFhyo40 ){
			m_pFhyo40->Init();
		}
		if( m_pFhyo52 ){
			m_pFhyo52->Init();
		}
		if( m_pFhyo522 ){
			m_pFhyo522->Init();
		}
		if( m_pFhyo50 ){
			m_pFhyo50->Init();
		}
//--> '15.02.18 INS START
		if( m_pFhyo40EX ){
			m_pFhyo40EX->Init();
		}
		if( m_pFhyo52EX ){
			m_pFhyo52EX->Init();
		}
		if( m_pFhyo522EX ){
			m_pFhyo522EX->Init();
		}
		if( m_pFhyo50EX ){
			m_pFhyo50EX->Init();
		}
//<-- '15.02.18 INS END

//--> '14.05.20 MOVE START
		if( m_pFhyo22 ){
			m_pFhyo22->Init();
			m_pFhyo22->WriteData( 1 );
		}
		//----->yoshida150811
		if( m_pFhyo22EX ){
			m_pFhyo22EX->Init();
			m_pFhyo22EX->WriteData( 1 );
		}
		//<------------------
		if( m_pFhyo20 ){
			m_pFhyo20->Init();
			m_pFhyo20->WriteData( 1 );
		}
		//----->yoshida150811
		if( m_pFhyo20EX ){
			m_pFhyo20EX->Init();
			m_pFhyo20EX->WriteData( 1 );
		}
		//<------------------
//<-- '14.05.20 MOVE END
		if( m_pFhyo10 ){
			m_pFhyo10->Init();
		}
		//----->yoshida150811
		if( m_pFhyo10EX ){
			m_pFhyo10EX->Init();
		}
		//<------------------

	
//--> '14.05.20 MOVE START
//		if( m_pFhyo22 ){
//			m_pFhyo22->Init();
//		}
//		if( m_pFhyo20 ){
//			m_pFhyo20->Init();
//		}
//<-- '14.05.20 MOVE END

		if( m_pIppan ){
//--> '14.04.18 INS START
			CalqFromIpaHonpyo();
//<-- '14.04.18 INS END
			m_pIppan->Init();
		}

		//----->yoshida150811
		if( m_pIppanEX ){		
			CalqFromIpaHonpyo();
			m_pIppanEX->Init();
		}
		//<------------------

		if( m_pKani ){
//--> '14.04.18 INS START
			CalqFromKanHonpyo();
//<-- '14.04.18 INS END
			m_pKani->Init();
		}

//--> '15.02.18 INS START
		if( m_pKaniEX ){
			CalqFromKanHonpyo();
			m_pKaniEX->Init();
		}
//<-- '15.02.18 INS END

		//----->yoshida150811
		if( m_pKaniEX2 ){		
			CalqFromKanHonpyo();
			m_pKaniEX2->Init();
		}
		//<-------------------

		if( m_pHjnKsk ){
			m_pHjnKsk->Init();
		}
		//----->yoshida150811
		if( m_pHjnKskEX ){
			m_pHjnKskEX->Init();
		}
		//<-------------------
		if( m_pKjnKsk ){
			m_pKjnKsk->Init();
		}
		//----->yoshida150811
		if( m_pKjnKskEX ){
			m_pKjnKskEX->Init();
		}
		//<-------------------
		if( m_pChsk ){
			m_pChsk->Init();
		}
		//----->yoshida150811
		if( m_pChskEX ){
			m_pChskEX->Init();
		}
		//<-------------------
//2016.06.22 INSERT START
		if( m_pRefundKjn02&&m_pSnFhyo20Data ){
			m_pRefundKjn02->calq_by_OtherChange( 3, m_pSnFhyo20Data, m_ShRevType );
		}
//2016.06.22 INSERT END

//2018.03.13 INSERT START
		if( m_pFhyo60 ){
			m_pFhyo60->Init();
		}
//2018.03.13 INSERT END

		//-->変更 harino ('14.03.05)
		//SetItemPosition( m_MainTab.GetCurSel() );
		PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセットのタイミングをPostMessageに変更してずらす
		//<--変更 harino ('14.03.05)
	}

	return 0;
}

//2016.10.20 INSERT START
void CH26HyoView::ReDispHonpyoUpper()
{
	if( m_pIppan ){
		m_pIppan->DispIppanUpper();
	}

	if( m_pIppanEX ){		
		m_pIppanEX->DispIppanUpper();
	}

	if( m_pKani ){
		m_pKani->DispInitKaniUpper();
	}

	if( m_pKaniEX ){
		m_pKaniEX->DispInitKaniUpper();
	}

	if( m_pKaniEX2 ){		
		m_pKaniEX2->DispInitKaniUpper();
	}

	if( m_pChsk ){
		m_pChsk->InitDisp();
	}

	if( m_pChskEX ){
		m_pChskEX->InitDisp();
	}

	if( m_pKjnKsk ){
		m_pKjnKsk->DispKsiKjnDigZi();
	}
	if( m_pKjnKskEX ){
		m_pKjnKskEX->DispKsiKjnDigZi();
	}
	if( m_pHjnKsk ){
		m_pHjnKsk->DispKsiHjnDigZi();
	}
	if( m_pHjnKskEX ){
		m_pHjnKskEX->DispKsiHjnDigZi();
	}


	PostMessage( WM_MYMESSAGE, 0, 0 );	// フォーカスセットのタイミングをPostMessageに変更してずらす

	return;
}
//2016.10.20 INSERT END

//-----------------------------------------------------------------------------
// 現在の入力位置にフォーカスセット harino ('14.03.11)
//-----------------------------------------------------------------------------
void CH26HyoView::SetNowPosition()
{
	SetItemPosition( m_MainTab.GetCurSel() );
}

//-----------------------------------------------------------------------------
// 本表から他表への計算
//-----------------------------------------------------------------------------
void CH26HyoView::CalqFromKanHonpyo()
{
	if( !m_pSnHeadData->SVmzsw ){
		if( m_pFhyo50 ){
			m_pFhyo50->calq_by_OtherChange();
		}
//--> '15.02.23 INS START
		if( m_pFhyo50EX ){
			m_pFhyo50EX->calq_by_OtherChange();
		}
//<-- '15.02.23 INS END
	}
	else{
		if( m_pFhyo52 ){
			m_pFhyo52->calq_by_OtherChange(1);
		}
		if( m_pFhyo40 ){
			m_pFhyo40->calq_by_OtherChange();
		}
//--> '15.02.23 INS START
		if( m_pFhyo52EX ){
			m_pFhyo52EX->calq_by_OtherChange(1);
		}
		if( m_pFhyo40EX ){
			m_pFhyo40EX->calq_by_OtherChange();
		}
//<-- '15.02.23 INS END
	}
}

//-----------------------------------------------------------------------------
// 本表から他表への計算 ('14.05.20)
//-----------------------------------------------------------------------------
void CH26HyoView::CalqFromIpaHonpyo()
{
	if( !m_pSnHeadData->SVmzsw ){
		if( m_pFhyo10 ){
			m_pFhyo10->calq_by_OtherChange( 1 );
		}
		//------>yoshida150810
		if( m_pFhyo10EX ){
			m_pFhyo10EX->calq_by_OtherChange( 1 );
		}
		//<-------------------
	}
	else{
		if( m_pFhyo22 ){
			m_pFhyo22->calq_by_OtherChange();
		}
		//------>yoshida150810
		if( m_pFhyo22EX ){
			m_pFhyo22EX->calq_by_OtherChange();
		}
		//<-------------------
		if( m_pFhyo10 ){
			m_pFhyo10->calq_by_OtherChange( 1 );
		}
		//------>yoshida150810
		if( m_pFhyo10EX ){
			m_pFhyo10EX->calq_by_OtherChange( 1 );
		}
		//<-------------------
	}
}

//-----------------------------------------------------------------------------
// 付表5-2(2)の表示呼出
//-----------------------------------------------------------------------------
void CH26HyoView::DispFhyo522()
{
	if( m_pFhyo522 ){
		// 控除対象仕入税額の計算式区分の表示
		m_pFhyo522->f52dsp_tk();
	}
}

void CH26HyoView::OnSize(UINT nType, int cx, int cy)
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

//		FitView( m_MainTab );

		//------>kanemura140312
/*		GetClientRect( vwRect );
		CRect	tabRect;
		m_MainTab.GetClientRect(tabRect);
		tabRect.bottom += vwRect.bottom - tabRect.bottom;
		m_MainTab.MoveWindow( tabRect );
		SetScrollPos(SB_HORZ,0,1);
*/
		//<--------------------

		//------>kanemura140312
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
//		SetScrollPos(SB_HORZ,0,1);
		//<---------------------


		//-->テスト削除 harino ('14.03.04)
		//SetItemPosition( m_MainTab.GetCurSel() );
		//<--テスト削除 harino ('14.03.04)
	}
}

void CH26HyoView::OnUpdateButtonF6(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( pCmdUI ){
		BOOL	bOn = FALSE;
		if( m_pSnHeadData ){
			int selno = m_MainTab.GetCurSel();
			if( (selno!=-1) && (selno<(int)m_TabMng.GetCount()) ){
				if( (m_TabMng[selno].tabtype==ID_ICSSH_IPPAN_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KANI_TAB) ){
//2016.02.23 UPDATE START
					if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
						bOn = TRUE;
					}

					//bOn = TRUE;
//2016.02.23 UPDATE END
				}
			}
		}
		pCmdUI->Enable( bOn );
	}

//2016.02.23 INSERT START
	//ICSToolBar* pBar = ((ICSMainFrame *)m_pShinInfo->pParent)->GetToolBar( 1 );
	//if( pBar ){
	//	char*		p = NULL;
	//	char*		pDescription = NULL;
	//	if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
	//		p = _T("F6 欄外印字");
	//		pDescription = _T("欄外印字を登録します");
	//	}
	//	else{
	//		p = _T("F6 欄外メモ");
	//		pDescription = _T("欄外メモを登録します");
	//	}
	//	pBar->SetButtonTooltip( 4, p );
	//	pBar->SetButtonText( 4, p );
	//	pBar->SetButtonDescription( 4, pDescription );
	//	return;
	//}
//2016.02.23 INSERT END
}

// データ集計
void CH26HyoView::OnButtonF11()
{
//2018.03.13 INSERT START
	int tabno;
	for( tabno = 0; tabno < m_TabMng.GetCount(); tabno++ ){
		if( m_TabMng[tabno].tabtype == ID_ICSSH_FHYO60_TAB ){
			break;
		}
	}
	if( tabno < m_TabMng.GetCount() ){
		if( Message_MyNumber_error() == -1 ){
			m_MainTab.SetCurSel( tabno );
			return;
		}
		if( Message_MyNumber_overlap() == -1 ){
			m_MainTab.SetCurSel( tabno );
			return;
		}
		EditOff_Fuhyo6();
		if( Message_SozokuWariai() == -1 ){
			m_MainTab.SetCurSel( tabno );
			return;
		}
	}
//2018.03.13 INSERT END
	UpdateSheetFromOuter();
	int selno = m_MainTab.GetCurSel();
	if( m_pShinInfo && m_pShinInfo->pParent ){
		// 再集計
		m_pShinInfo->pParent->SendMessage( WM_USER_RECALQ, (WPARAM)1 );
	}
	// 再表示
//-- '15.03.03 --
//	ReInitDisp( TRUE );
//---------------
//--- '15.08.27 ---
//	ReInitDisp( TRUE, m_SixKindFlg );
//----------------
//2016.02.23 INSERT START
	this->LockWindowUpdate();
//2016.02.23 INSERT END
	ReInitDisp( TRUE, m_SixKindFlg, m_ShRevType );
//2016.02.23 INSERT START
	this->UnlockWindowUpdate();
//2016.02.23 INSERT END
//-----------------
//---------------
	m_MainTab.SetCurSel( selno );
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );
	SetItemPosition( selno );
}

void CH26HyoView::OnUpdateButtonF11(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL	bON = FALSE;
	if( pCmdUI ){
		if( m_pSnHeadData ){
			if( (m_pSnHeadData->Sn_Sign4&0x81) == 0 ){
				bON = TRUE;
			}
			if( m_pSnHeadData->IsMiddleProvisional() ){
				bON = FALSE;
			}
			if( bON ){
				if( m_SnSeq ){
					bON = FALSE;
				}
			}
		}
		pCmdUI->Enable( bON );
	}
}


// 次表
void CH26HyoView::OnButtonF12()
{
	PageChange(1);	// 1:次へ
	return;
}

void CH26HyoView::OnUpdateButtonF12(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_MainTab.GetItemCount() == 1 )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( TRUE );
}

//-----------------------------------------------------------------------------
// 表示帳票の最終データ取得
//-----------------------------------------------------------------------------
void CH26HyoView::GetDispHyoData()
{
	int selno = m_MainTab.GetCurSel();
	int max = (int)m_TabMng.GetCount();
	if( (selno>=max) || (selno==-1) ){
		return;
	}
	if( m_bShowed == FALSE ){
		return;
	}

	if( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN_TAB ){
		if( m_pIppan ){
			m_pIppan->GetDiagData();
			m_pIppan->WriteData();
		}

		//------>yoshida150810
		if( m_pIppanEX ){
			m_pIppanEX->GetDiagData();
			m_pIppanEX->WriteData();
		}
		//<-------------------

	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_KANI_TAB ){
		if( m_pKani ){
			m_pKani->GetDiagData();
			m_pKani->WriteData();
		}
//--> '15.01.05 INS START
		if( m_pKaniEX ){
			m_pKaniEX->GetDiagData();
			m_pKaniEX->WriteData();
		}
//<-- '15.01.05 INS END
		//------>yoshida150810
		if( m_pKaniEX2 ){
			m_pKaniEX2->GetDiagData();
			m_pKaniEX2->WriteData();
		}
		//<--------------------
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO10_TAB ){
		if( m_pFhyo10 ){
			m_pFhyo10->GetAllDiagData();
			m_pFhyo10->WriteData();
		}
		//------>yoshida150810
		if( m_pFhyo10EX ){
			m_pFhyo10EX->GetAllDiagData();
			m_pFhyo10EX->WriteData();
		}
		//<-------------------
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO20_TAB ){
		if( m_pFhyo20 ){
			m_pFhyo20->GetAllDiagData();
			m_pFhyo20->WriteData( 1 );
		}
		//------>yoshida150810
		if( m_pFhyo20EX ){
			m_pFhyo20EX->GetAllDiagData();
			m_pFhyo20EX->WriteData( 1 );
		}
		//<-------------------
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO22_TAB ){
		if( m_pFhyo22 ){
			m_pFhyo22->GetAllDiagData();
			m_pFhyo22->WriteData( 1 );
		}
		//------>yoshida150810
		if( m_pFhyo22EX ){
			m_pFhyo22EX->GetAllDiagData();
			m_pFhyo22EX->WriteData( 1 );
		}
		//<-------------------
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO40_TAB ){
		if( m_pFhyo40 ){
			m_pFhyo40->GetAllDiagData();
			m_pFhyo40->WriteData();
		}
//--> '15.01.05 INS START
		if( m_pFhyo40EX ){
			m_pFhyo40EX->GetAllDiagData();
			m_pFhyo40EX->WriteData();
		}
//<-- '15.01.05 INS END
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO50_TAB ){
		if( m_pFhyo50 ){
			m_pFhyo50->GetAllDiagData();
			m_pFhyo50->WriteData();
		}
//--> '15.01.05 INS START
		if( m_pFhyo50EX ){
			m_pFhyo50EX->GetAllDiagData();
			m_pFhyo50EX->WriteData();
		}
//--> '15.01.05 INS END
	}
/*- '14.06.28 -*/
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO52_1_TAB ){
//		if( m_pFhyo52 ){
////			m_pFhyo52->GetAllDiagData();
//		}
//		if( m_pFhyo522 ){
////			m_pFhyo522->GetAllDiagData();
//		}
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO52_2_TAB ){
//		if( m_pFhyo52 ){
////			m_pFhyo52->GetAllDiagData();
//		}
//		if( m_pFhyo522 ){
////			m_pFhyo522->GetAllDiagData();
//		}
//	}
/*-------------*/
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO52_1_TAB ){
		if( m_pFhyo52 ){
			m_pFhyo52->WriteData();
		}
//--> '15.01.05 INS START
		if( m_pFhyo52EX ){
			m_pFhyo52EX->WriteData();
		}
//--> '15.01.05 INS END
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO52_2_TAB ){
		if( m_pFhyo52 ){
			m_pFhyo52->WriteData();
		}
//--> '15.01.05 INS START
		if( m_pFhyo52EX ){
			m_pFhyo52EX->WriteData();
		}
//--> '15.01.05 INS END
	}
//2018.03.13 INSERT START
	else if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO60_TAB ){
		if( m_pFhyo60 ){
			m_pFhyo60->WriteData();
		}
	}
//2018.03.13 INSERT END
/*-------------*/
	else if( m_TabMng[selno].tabtype == ID_ICSSH_REFUND_KJ_01_TAB ){
		if( m_pRefundKjn01 ){
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_REFUND_KJ_02_TAB ){
		if( m_pRefundKjn02 ){
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_REFUND_HJ_01_TAB ){
		if( m_pRefundHjn01 ){
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_REFUND_HJ_02_TAB ){
		if( m_pRefundHjn02 ){
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB ){
		if( m_pHjnKsk ){
		}
		//----->yoshida150810
		if( m_pHjnKskEX ){
		}
		//<------------------
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB ){
		if( m_pKjnKsk ){
		}
		//----->yoshida150810
		if( m_pKjnKskEX ){
		}
		//<------------------
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_CHSK_TAB ){
		if( m_pChsk ){
		}
		//----->yoshida150810
		if( m_pChskEX ){
		}
		//<------------------
	}
}

//-----------------------------------------------------------------------------
// 過年度プロ呼出し後にデータを吸い上げる
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH26HyoView::GetPastData()
{
	// データの有無チェック
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第26号様式）
		// 本表データ読込みクラス
		if( !m_pSnHonpyoData ){

			//----->yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//				m_pSnHonpyoData = new CH27SnHonpyoData();
//				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//					return -1;
//				}
//---------------
				m_pSnHonpyoData = new CH28SnHonpyoData();
				if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
//---------------
			}
			else{
				m_pSnHonpyoData = new CH26SnHonpyoData();
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//--------------------
			/*
			m_pSnHonpyoData = new CH26SnHonpyoData();
			if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
			*/
			//<--------------------
		}
		// 中間予定申告データ
		if( !m_pSnChskData ){
			m_pSnChskData = new CH26SnChskData();
			if( m_pSnChskData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
	}
	else{
		// 課税処理
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );

		BOOL	isTransitionalMeasure = FALSE;
		if( m_pSnHeadData->SVmzsw == 1 ){
			isTransitionalMeasure = TRUE;
		}

		if( KzHoushiki == ID_ICSSH_KANNI ){
			// 本表データ読込みクラス
			if( !m_pSnHonpyoData ){

				//---->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//					m_pSnHonpyoData = new CH27SnHonpyoData();
//					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//---------------
					m_pSnHonpyoData = new CH28SnHonpyoData();
					if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
//---------------
				}
				else{
					if(m_SixKindFlg == TRUE){
						m_pSnHonpyoData = new CH27SnHonpyoData();
						if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnHonpyoData = new CH26SnHonpyoData();
						if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
				}
				//--------------------
				/*
				//---->[Y]150106
				if(m_SixKindFlg == TRUE){
					m_pSnHonpyoData = new CH27SnHonpyoData();
					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnHonpyoData = new CH26SnHonpyoData();
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//<-------------------
				*/
				//<--------------------
			}
			// 付表４データ読込みクラス
			if( !m_pSnFhyo40Data ){
				//---->[Y]150106
				if(m_SixKindFlg == TRUE){
					m_pSnFhyo40Data = new CH27SnFhyo10Data(isTransitionalMeasure);
					if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnFhyo40Data = new CH26SnFhyo10Data(isTransitionalMeasure);
					if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//<-----------------
			}
			// 経過措置対象の課税資産の譲渡等あり
			if( m_pSnHeadData->SVmzsw == 1 ){
				// データ読込みクラス
				if( !m_pSnFhyo52Data ){
					//---->[Y]150106
					if(m_SixKindFlg == TRUE){
						m_pSnFhyo52Data = new CH27SnFhyo50Data(TRUE);
						if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo52Data = new CH26SnFhyo50Data(TRUE);
						if( m_pSnFhyo52Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//<------------------
				}
			}
			// 経過措置対象の課税資産の譲渡等なし
			else{
				// データ読込みクラス
				if( !m_pSnFhyo50Data ){
					//---->[Y]150106
					if(m_SixKindFlg == TRUE){
						m_pSnFhyo50Data = new CH27SnFhyo50Data(FALSE);
						if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo50Data = new CH26SnFhyo50Data(FALSE);
						if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//<-------------------
				}
			}
		}
		else if( KzHoushiki == ID_ICSSH_GENSOKU ){
			// データ読込みクラス
			if( !m_pSnFhyo10Data ){
				//---->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					m_pSnFhyo10Data = new CH28SnFhyo10Data(isTransitionalMeasure);
					if( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
					if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//------------------
				/*
				m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
				if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<------------------
			}

			// 原則課税
			// データ読込みクラス
			if( !m_pSnHonpyoData ){
				//---->yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//					m_pSnHonpyoData = new CH27SnHonpyoData();
//					if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//						return -1;
//					}
//---------------
					m_pSnHonpyoData = new CH28SnHonpyoData();
					if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
//---------------
				}
				else{
					m_pSnHonpyoData = new CH26SnHonpyoData();
					if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//-------------------
				/*
				m_pSnHonpyoData = new CH26SnHonpyoData();
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<-------------------
			}

			// 経過措置対象の課税資産の譲渡等あり
			if( m_pSnHeadData->SVmzsw == 1 ){
				// データ読込みクラス
				if( !m_pSnFhyo22Data ){
					//---->yoshida150811
//					if( m_TokuteiFlg == TRUE ){
					if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
						m_pSnFhyo22Data = new CH28SnFhyo20Data(TRUE);
						if( ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE);
						if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//------------------
					/*
					m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE);
					if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
					*/
					//<------------------
				}
				if( !m_pSnTanaData ){
					m_pSnTanaData = new CH26SnTanaData(TRUE);
					if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			// 経過措置対象の課税資産の譲渡等なし
			else{
				// データ読込みクラス
				if( !m_pSnFhyo20Data ){
					//---->yoshida150811
//					if( m_TokuteiFlg == TRUE ){
					if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
						m_pSnFhyo20Data = new CH28SnFhyo20Data(FALSE);
						if( ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					else{
						m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE);
						if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
							return -1;
						}
					}
					//-----------------
					/*
					m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE);
					if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
					*/
					//<----------------
				}
				if( !m_pSnTanaData ){
					m_pSnTanaData = new CH26SnTanaData(FALSE);
					if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
		}

		// 還付
		if( m_pSnHeadData->IsUseTaxRefund() ){
			if( !m_pSnRefundData ){
				m_pSnRefundData = new CH26SnRefundData();
				if( m_pSnRefundData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
					m_pSnRefundData->DeleteAllData( m_pZmSub, -1 );
					return -1;
				}
				// 情報再設定
				ResetRefundData();
			}
		}

		// 更正の請求
		if( m_pSnHeadData->Sn_Syuusei&0x02 ){
			if( !m_pSnKskData ){
				m_pSnKskData = new CH26SnKskData();
				if( m_pSnKskData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
					return -1;
				}
			}
		}
	}

	// 本表
	//---->yoshida150811
//	if( m_TokuteiFlg == TRUE ){
	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
		if( m_pSnHonpyoData ){
			m_pSnHonpyoData->GetPastData( m_pZmSub, m_SnSeq );
//-- '15.08.19 --
//			((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData( m_pZmSub, m_SnSeq );
//---------------
			((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData( m_pZmSub, m_SnSeq );
//---------------
		}
	}
	else{
		if( m_SixKindFlg == TRUE ){
			if( m_pSnHonpyoData ){
				m_pSnHonpyoData->GetPastData( m_pZmSub, m_SnSeq );
				((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData( m_pZmSub, m_SnSeq );
			}
		}
		else{
			if( m_pSnHonpyoData ){
				m_pSnHonpyoData->GetPastData( m_pZmSub, m_SnSeq );
				m_pSnHonpyoData->GetData( m_pZmSub, m_SnSeq );
			}
		}
	}
	//-------------------
	/*
	if( m_SixKindFlg == TRUE ){
		if( m_pSnHonpyoData ){
			m_pSnHonpyoData->GetPastData( m_pZmSub, m_SnSeq );
			((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData( m_pZmSub, m_SnSeq );
		}
	}
	else{
		if( m_pSnHonpyoData ){
			m_pSnHonpyoData->GetPastData( m_pZmSub, m_SnSeq );
			m_pSnHonpyoData->GetData( m_pZmSub, m_SnSeq );
		}
	}
	*/
	//<-------------------

	// 付表２
	//--------->yoshida150811
//	if( m_TokuteiFlg == TRUE ){
	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
		if( m_pSnFhyo20Data ){
			((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetPastData( m_pZmSub, m_SnSeq );
			((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetData( m_pZmSub, m_SnSeq );
		}
		if( m_pSnFhyo22Data ){
			((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetPastData( m_pZmSub, m_SnSeq );
			((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetData( m_pZmSub, m_SnSeq );
		}
		// 付表１
		if( m_pSnFhyo10Data ){
			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->GetPastData( m_pZmSub, m_SnSeq );
			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->GetData( m_pZmSub, m_SnSeq );
		}
	}
	else{
		if( m_pSnFhyo20Data ){
			m_pSnFhyo20Data->GetPastData( m_pZmSub, m_SnSeq );
			m_pSnFhyo20Data->GetData( m_pZmSub, m_SnSeq );
		}
		if( m_pSnFhyo22Data ){
			m_pSnFhyo22Data->GetPastData( m_pZmSub, m_SnSeq );
			m_pSnFhyo22Data->GetData( m_pZmSub, m_SnSeq );
		}
		// 付表１
		if( m_pSnFhyo10Data ){
			m_pSnFhyo10Data->GetPastData( m_pZmSub, m_SnSeq );
			m_pSnFhyo10Data->GetData( m_pZmSub, m_SnSeq );
		}
	}
	//-----------------------
	/*
	if( m_pSnFhyo20Data ){
		m_pSnFhyo20Data->GetPastData( m_pZmSub, m_SnSeq );
		m_pSnFhyo20Data->GetData( m_pZmSub, m_SnSeq );
	}
	if( m_pSnFhyo22Data ){
		m_pSnFhyo22Data->GetPastData( m_pZmSub, m_SnSeq );
		m_pSnFhyo22Data->GetData( m_pZmSub, m_SnSeq );
	}
	// 付表１
	if( m_pSnFhyo10Data ){
		m_pSnFhyo10Data->GetPastData( m_pZmSub, m_SnSeq );
		m_pSnFhyo10Data->GetData( m_pZmSub, m_SnSeq );
	}
	*/
	//<-----------------------

	// 付表５
	if( m_pSnFhyo50Data ){
		//----->[Y]150106
		if(m_SixKindFlg == TRUE){
			m_pSnFhyo50Data->GetPastData( m_pZmSub, m_SnSeq );
			((CH27SnFhyo50Data*)m_pSnFhyo50Data)->GetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo50Data->GetPastData( m_pZmSub, m_SnSeq );
			m_pSnFhyo50Data->GetData( m_pZmSub, m_SnSeq );
		}
		//<------------------
	}
	if( m_pSnFhyo52Data ){
		if(m_SixKindFlg == TRUE){
			m_pSnFhyo52Data->GetPastData( m_pZmSub, m_SnSeq );
			((CH27SnFhyo50Data*)m_pSnFhyo52Data)->GetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo52Data->GetPastData( m_pZmSub, m_SnSeq );
			m_pSnFhyo52Data->GetData( m_pZmSub, m_SnSeq );
		}
	}
	// 付表４
	if( m_pSnFhyo40Data ){
		//----->[Y]150106
		if(m_SixKindFlg == TRUE){
			m_pSnFhyo40Data->GetPastData( m_pZmSub, m_SnSeq );
			((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData( m_pZmSub, m_SnSeq );
		}
		else{
			m_pSnFhyo40Data->GetPastData( m_pZmSub, m_SnSeq );
			m_pSnFhyo40Data->GetData( m_pZmSub, m_SnSeq );
		}
		//<-------------------
	}
	// 棚卸データ
	if( m_pSnTanaData ){
		m_pSnTanaData->GetPastData( m_pZmSub, m_SnSeq );
		m_pSnTanaData->GetData( m_pZmSub, m_SnSeq );
	}
	// 予定申告データ
	if( m_pSnChskData ){
		m_pSnChskData->GetData( m_pZmSub, m_SnSeq );
	}
	// 本表
	if( m_pSnHeadData ){
		m_pSnHeadData->GetData( m_pZmSub, m_SnSeq );
	}
	// 還付申告
	if( m_pSnRefundData ){
		m_pSnRefundData->GetData( m_pZmSub, m_pSnHeadData, m_SnSeq );
	}
	// 更正の請求
	if( m_pSnKskData ){
		m_pSnKskData->GetData( m_pZmSub, m_pSnHeadData, m_SnSeq );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 顧問先情報の取り込み
//-----------------------------------------------------------------------------
void CH26HyoView::OnUpdateKomonsakiInf(CCmdUI *pCmdUI)
{
	int	Flg = 0;
	int selno = m_MainTab.GetCurSel();

	if( (selno!=-1) && m_pSnHeadData ){
		if( !(m_pSnHeadData->Sn_Sign4&0x01) ){
			if( m_pSnHeadData->IsMiddleProvisional() ){
				Flg = 3;
			}
			else if( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN_TAB ){
				Flg = 2;
			}
			else if( m_TabMng[selno].tabtype == ID_ICSSH_KANI_TAB ){
				Flg = 2;
			}
		}
		if( Flg ){
			pCmdUI->Enable( TRUE );
		}
		else{
			pCmdUI->Enable( FALSE );
		}
	}
//--> '15.03.03 CUT START
//	if( Flg == 3 ){
//		if( m_pChsk ){
//			m_pChsk->Init();
//		}
//	}
//<-- '15.03.03 CUT END
}

void CH26HyoView::OnButtonF8()
{
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		if( (m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) ){
			// 更正の請求連動ダイアログ
			CShinKskRnd	SKRDialog;
			SKRDialog.InitInfo( m_pSnKskData );
			SKRDialog.m_Rsw = 1;
			SKRDialog.m_Ksw = 0;

			HINSTANCE svhInstResource = AfxGetResourceHandle();
			AfxSetResourceHandle( g_hInstance );

			int rt = (int)SKRDialog.DoModal();

			AfxSetResourceHandle( svhInstResource );

			if( rt != IDOK ){
				return;
			}

			int	Sign = SKRDialog.m_Rsw;
			int Bank = SKRDialog.m_Ksw;

			// 申告金額の連動
			CalqSyzForKsk( Sign );
			// 還付先金融機関等情報の連動
			if( Bank ){
				if( m_pSnKskData ){
					m_pSnKskData->SetReaskFinancial( m_pSnHeadData );
				}
			}

			// 再表示
			if( (m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) && m_pHjnKsk ){
				m_pHjnKsk->DispKsiHjn( 1 );
			}
			else if( (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) && m_pKjnKsk ){
				m_pKjnKsk->DispKsiKjn( 1 );
			}
			//------>yoshida150810
			else if( (m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) && m_pHjnKskEX ){
				m_pHjnKskEX->DispKsiHjn( 1 );
				m_pHjnKskEX->SetItemPosition ( 0 );
			}
			else if( (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) && m_pKjnKskEX ){
				m_pKjnKskEX->DispKsiKjn( 1 );
				m_pKjnKskEX->SetItemPosition ( 0 );
			}
			//<-------------------

		}
	}
}

void CH26HyoView::OnUpdateButtonF8(CCmdUI *pCmdUI)
{
	BOOL	bOn = FALSE;

	if( !(m_pSnHeadData->Sn_Sign4&0x01) ){
		int selno = m_MainTab.GetCurSel();
		if( (selno!=-1) && (selno<(int)m_TabMng.GetCount()) ){
			if( (m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) ){
				bOn = TRUE;
			}
		}
	}
	pCmdUI->Enable( bOn );
}

void CH26HyoView::OnUpdateButtonF4(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	int	Flag = 1;
/*	
	if( m_pMainFrame ){
		if( m_pMainFrame->IsAblePrint() ){
			Flag = 1;
		}
	}
*/	if( m_pShinInfo->isDotPrn ){
		Flag = 0;
	}
	pCmdUI->Enable( Flag ? TRUE : FALSE );
}

void CH26HyoView::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	int	Flag = 1;
/*	
	if( m_pMainFrame ){
		if( m_pMainFrame->IsAblePrint() ){
			Flag = 1;
		}
	}
*/
	if( m_pShinInfo->isDotPrn ){
		Flag = 0;
	}
	pCmdUI->Enable( Flag ? TRUE : FALSE );
}
//-----------------------------------------------------------------------------
// 全表の更新処理
//-----------------------------------------------------------------------------
//2017.03.01 UPDATE START
//void CH26HyoView::UpdateAllSheet()
void CH26HyoView::UpdateAllSheet( int sgn )
//2017.03.01 UPDATE END
{
	int cnt = (int)m_TabMng.GetCount();

//2017.03.01 UPDATE START
//	UpdateSheet( m_MainTab.GetCurSel() );//現在の表を先に更新しておく[S]
//	for( int i=0; i<cnt; i++ ){
//		UpdateSheet( i );
//	}

	if( sgn == 0 ){
		UpdateSheet( m_MainTab.GetCurSel() );//現在の表を先に更新しておく[S]
		for( int i=0; i<cnt; i++ ){
			UpdateSheet( i, 0 );
		}
	}else{
		for( int i=0; i<cnt; i++ ){
			UpdateSheet( i ,1 );
		}
	}
//2017.03.01 UPDATE END

}

//-----------------------------------------------------------------------------
//[Y] 選択表より前までの表の更新処理
//-----------------------------------------------------------------------------
void CH26HyoView::UpdateUntilSheet()
{

	int cnt = m_MainTab.GetCurSel();

	for( int i=0; i<cnt; i++ ){
		UpdateSheet( i );
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
int CH26HyoView::ChangeSnSeq( int snseq )
{
	if( m_SvSnSeq == snseq ){
		return 0;
	}

	// 既存データの書込み
	if( WriteData(TRUE, FALSE) ){
		return -1;
	}

	// 新履歴番号セット
	SetSnSeq( snseq );

	// 新規データの取得
	ReGetData();

	return 0;
}

//-----------------------------------------------------------------------------
// 履歴シーケンスセット
//-----------------------------------------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//-----------------------------------------------------------------------------
void CH26HyoView::SetSnSeq( int snseq )
{
	m_SnSeq = snseq;
	m_SvSnSeq = snseq;
}

//-----------------------------------------------------------------------------
// 現申告データを消去して、データの再読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::ReReadData()
{
	BOOL	isTransitionalMeasure = FALSE;
	if( m_pSnHeadData->SVmzsw == 1 ){
		isTransitionalMeasure = TRUE;
	}

	if( m_pSnHonpyoData ){
		delete m_pSnHonpyoData;
		m_pSnHonpyoData = NULL;

		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//			if( (m_pSnHonpyoData = new CH27SnHonpyoData) == NULL ){
//				return -1;
//			}
//			if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//---------------
			if( (m_pSnHonpyoData = new CH28SnHonpyoData) == NULL ){
				return -1;
			}
			if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
//---------------
		}
		else{
			if( m_SixKindFlg == TRUE ){
				if( (m_pSnHonpyoData = new CH27SnHonpyoData) == NULL ){
					return -1;
				}
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				if( (m_pSnHonpyoData = new CH26SnHonpyoData) == NULL ){
					return -1;
				}
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
		}
		//--------------------
		/*
		//----->[Y]150106
		if( m_SixKindFlg == TRUE ){
			if( (m_pSnHonpyoData = new CH27SnHonpyoData) == NULL ){
				return -1;
			}
			if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( (m_pSnHonpyoData = new CH26SnHonpyoData) == NULL ){
				return -1;
			}
			if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//<-------------------
		*/
		//<--------------------
	}
	if( m_pSnFhyo10Data ){
		delete m_pSnFhyo10Data;
		m_pSnFhyo10Data = NULL;

		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( (m_pSnFhyo10Data = new CH28SnFhyo10Data(isTransitionalMeasure)) == NULL ){
				return -1;
			}
			if( ((CH28SnFhyo10Data*)m_pSnFhyo10Data)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( (m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure)) == NULL ){
				return -1;
			}
			if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//--------------------
		/*
		if( (m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure)) == NULL ){
			return -1;
		}
		if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
		*/
		//<-------------------
	}
	if( m_pSnFhyo20Data ){
		delete m_pSnFhyo20Data;
		m_pSnFhyo20Data = NULL;

		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( (m_pSnFhyo20Data = new CH28SnFhyo20Data(FALSE)) == NULL ){
				return -1;
			}
			if(  ((CH28SnFhyo20Data*)m_pSnFhyo20Data)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( (m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE)) == NULL ){
				return -1;
			}
			if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//-------------------
		/*
		if( (m_pSnFhyo20Data = new CH26SnFhyo20Data(FALSE)) == NULL ){
			return -1;
		}
		if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
		*/
		//<------------------
	}
	if( m_pSnFhyo22Data ){
		delete m_pSnFhyo22Data;
		m_pSnFhyo22Data = NULL;

		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( (m_pSnFhyo22Data = new CH28SnFhyo20Data(TRUE)) == NULL ){
				return -1;
			}
			if(  ((CH28SnFhyo20Data*)m_pSnFhyo22Data)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( (m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE)) == NULL ){
				return -1;
			}
			if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//-------------------
		/*
		if( (m_pSnFhyo22Data = new CH26SnFhyo20Data(TRUE)) == NULL ){
			return -1;
		}
		if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
		*/
		//<-------------------
	}
	if( m_pSnTanaData ){
		delete m_pSnTanaData;
		m_pSnTanaData = NULL;

		if( (m_pSnTanaData = new CH26SnTanaData(m_pSnHeadData->SVmzsw)) == NULL ){
			return -1;
		}
		if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
	}
	if( m_pSnFhyo40Data ){
		delete m_pSnFhyo40Data;
		m_pSnFhyo40Data = NULL;

		//----->[Y]150106
		if(m_SixKindFlg == TRUE){
			if( (m_pSnFhyo40Data = new CH27SnFhyo10Data(isTransitionalMeasure)) == NULL ){
				return -1;
			}
			if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( (m_pSnFhyo40Data = new CH26SnFhyo10Data(isTransitionalMeasure)) == NULL ){
				return -1;
			}
			if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//<---------------------
	}
	if( m_pSnFhyo50Data ){
		delete m_pSnFhyo50Data;
		m_pSnFhyo50Data = NULL;

		//----->[Y]150106
		if(m_SixKindFlg == TRUE){
			if( (m_pSnFhyo50Data = new CH27SnFhyo50Data(FALSE)) == NULL ){
				return -1;
			}
			if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( (m_pSnFhyo50Data = new CH26SnFhyo50Data(FALSE)) == NULL ){
				return -1;
			}
			if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//<-----------------
	}
	if( m_pSnFhyo52Data ){
		delete m_pSnFhyo52Data;
		m_pSnFhyo52Data = NULL;

		//----->[Y]150106
		if(m_SixKindFlg == TRUE){
			if( (m_pSnFhyo52Data = new CH27SnFhyo50Data(TRUE)) == NULL ){
				return -1;
			}
			if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			if( (m_pSnFhyo52Data = new CH26SnFhyo50Data(TRUE)) == NULL ){
				return -1;
			}
			if( m_pSnFhyo52Data->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//<------------------
	}
//2018.03.13 INSERT START
	if( m_pSnFhyo60Data ){
		delete m_pSnFhyo60Data;
		m_pSnFhyo60Data = NULL;

		if( (m_pSnFhyo60Data = new CH28SnFhyo60Data()) == NULL ){
			return -1;
		}
		if( m_pSnFhyo60Data->GetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
	}
//2018.03.13 INSERT END
	if( m_pSnRefundData ){
		delete m_pSnRefundData;
		m_pSnRefundData = NULL;

		if( (m_pSnRefundData = new CH26SnRefundData) == NULL ){
			return -1;
		}
		if( m_pSnRefundData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		ResetRefundData();
	}
	if( m_pSnChskData ){
		delete m_pSnChskData;
		m_pSnChskData = NULL;

		if( (m_pSnChskData = new CH26SnChskData) == NULL ){
			return -1;
		}
		if( m_pSnChskData->GetData(m_pZmSub, m_SnSeq) ){
			return -1;
		}
	}
	if( m_pSnKskData ){
		delete m_pSnKskData;
		m_pSnKskData = NULL;

		if( (m_pSnKskData = new CH26SnKskData) == NULL ){
			return -1;
		}
		if( m_pSnKskData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 手入力サインのクリア
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH26HyoView::ClearManualInputSign( BOOL isChgKzritu )
{
	if( m_pSnFhyo50Data ){
		//---->[Y]150119
		if(m_SixKindFlg == TRUE){
			 ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->ClearManualInputSign( isChgKzritu );
		}
		else{
			m_pSnFhyo50Data->ClearManualInputSign( isChgKzritu );
		}
		//<---------------
//		m_pSnFhyo50Data->ClearManualInputSign( isChgKzritu );
	}
	if( m_pSnFhyo52Data ){
		//---->[Y]150119
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo50Data*)m_pSnFhyo52Data)->ClearManualInputSign( isChgKzritu );
		}
		else{
			m_pSnFhyo52Data->ClearManualInputSign( isChgKzritu );
		}
		//<----------------
//		m_pSnFhyo52Data->ClearManualInputSign( isChgKzritu );
	}
	if( m_pSnFhyo40Data ){
		//---->[Y]150119
		if(m_SixKindFlg == TRUE){
			((CH27SnFhyo10Data*)m_pSnFhyo40Data)->ClearManualInputSign( isChgKzritu );
		}
		else{
			m_pSnFhyo40Data->ClearManualInputSign( isChgKzritu );
		}
		//<----------------
//		m_pSnFhyo40Data->ClearManualInputSign( isChgKzritu );
	}

	if( m_pSnFhyo20Data ){
		//----->yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo20Data*)m_pSnFhyo20Data)->ClearManualInputSign( isChgKzritu );
		}
		else{
			m_pSnFhyo20Data->ClearManualInputSign( isChgKzritu );
		}
		//--------------------------
		//m_pSnFhyo20Data->ClearManualInputSign( isChgKzritu );
		//<-------------------------
	}
	if( m_pSnFhyo22Data ){
//-- '15.08.24 --
//		m_pSnFhyo22Data->ClearManualInputSign( isChgKzritu );
//---------------
//		if( m_TokuteiFlg ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo20Data*)m_pSnFhyo22Data)->ClearManualInputSign( isChgKzritu );
		}
		else{
			m_pSnFhyo22Data->ClearManualInputSign( isChgKzritu );
		}
//---------------
	}
	if( m_pSnFhyo10Data ){
//-- '15.08.24 --
//		m_pSnFhyo10Data->ClearManualInputSign( isChgKzritu );
//---------------
//		if( m_TokuteiFlg ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnFhyo10Data*)m_pSnFhyo10Data)->ClearManualInputSign( isChgKzritu );
		}
		else{
			m_pSnFhyo10Data->ClearManualInputSign( isChgKzritu );
		}
//---------------
	}

	if( m_pSnHonpyoData ){
//-- '15.08.24 --
//		//---->[Y]150119
//		if(m_SixKindFlg == TRUE){
//			((CH27SnHonpyoData*)m_pSnHonpyoData)->ClearManualInputSign();
//		}
//		else{
//			m_pSnHonpyoData->ClearManualInputSign();
//		}
//		//<----------------
////		m_pSnHonpyoData->ClearManualInputSign();
//---------------
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			((CH28SnHonpyoData*)m_pSnHonpyoData)->ClearManualInputSign();
		}
		else if( m_SixKindFlg == TRUE ){
			((CH27SnHonpyoData*)m_pSnHonpyoData)->ClearManualInputSign();
		}
		else{
			m_pSnHonpyoData->ClearManualInputSign();
		}
//---------------
	}

	if( m_pSnHeadData->IsSoleProprietor() ){
		if( m_pRefundKjn02 ){
			m_pRefundKjn02->refundvol.KPSstx &= ~0x01;
		}
	}
	else{
		if( m_pRefundHjn02 ){
			m_pRefundHjn02->refundvol.KPSstx &= ~0x01;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 次回申告データ作成に必要になるデータの読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::ReadDataForNextShinkoku()
{
	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );

	if( m_pSnHonpyoData == NULL ){

		//------>yoshida150811
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-- '15.08.19 --
//			m_pSnHonpyoData = new CH27SnHonpyoData();
//			if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
//				return -1;
//			}
//---------------
			m_pSnHonpyoData = new CH28SnHonpyoData();
			if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
//---------------
		}
		else{
			if(m_SixKindFlg == TRUE){
				m_pSnHonpyoData = new CH27SnHonpyoData();
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				m_pSnHonpyoData = new CH26SnHonpyoData();
				if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
		}
		//----------------------
		/*
		//----->[Y]150106
		if(m_SixKindFlg == TRUE){
			m_pSnHonpyoData = new CH27SnHonpyoData();
			if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		else{
			m_pSnHonpyoData = new CH26SnHonpyoData();
			if( m_pSnHonpyoData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
		//<-------------------
		*/
		//<---------------------
	}

	BOOL	isTransitionalMeasure = FALSE;
	if( m_pSnHeadData ){
		if( m_pSnHeadData->SVmzsw ){
			isTransitionalMeasure = TRUE;
		}
	}

	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		if( m_pSnFhyo10Data == NULL ){

			//------>yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				m_pSnFhyo10Data = new CH28SnFhyo10Data(isTransitionalMeasure);
				if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
				if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//--------------------
			/*
			m_pSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure);
			if( m_pSnFhyo10Data->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
			*/
			//<-------------------
		}

		if( isTransitionalMeasure ){
			if( m_pSnFhyo22Data == NULL ){

				//------>yoshida150811
//				if( m_TokuteiFlg == TRUE ){
				if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					m_pSnFhyo22Data = new CH28SnFhyo20Data(isTransitionalMeasure);
					if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnFhyo22Data = new CH26SnFhyo20Data(isTransitionalMeasure);
					if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//--------------------
				/*
				m_pSnFhyo22Data = new CH26SnFhyo20Data(isTransitionalMeasure);
				if( m_pSnFhyo22Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
				*/
				//<--------------------
			}
		}
		else{
			//------>yoshida150811
//			if( m_TokuteiFlg == TRUE ){
			if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
				if( m_pSnFhyo20Data == NULL ){
					m_pSnFhyo20Data = new CH28SnFhyo20Data(isTransitionalMeasure);
					if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			else{
				if( m_pSnFhyo20Data == NULL ){
					m_pSnFhyo20Data = new CH26SnFhyo20Data(isTransitionalMeasure);
					if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
			}
			//--------------------
			/*
			if( m_pSnFhyo20Data == NULL ){
				m_pSnFhyo20Data = new CH26SnFhyo20Data(isTransitionalMeasure);
				if( m_pSnFhyo20Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			*/
			//<-------------------
		}

		if( m_pSnTanaData == NULL ){
			m_pSnTanaData = new CH26SnTanaData(isTransitionalMeasure);
			if( m_pSnTanaData->GetData(m_pZmSub, m_SnSeq) ){
				return -1;
			}
		}
	}
	else{
		if( m_pSnFhyo40Data == NULL ){
			//----->[Y]150106
			if(m_SixKindFlg == TRUE){
				m_pSnFhyo40Data = new CH27SnFhyo10Data(isTransitionalMeasure);
				if( ((CH27SnFhyo10Data*)m_pSnFhyo40Data)->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			else{
				m_pSnFhyo40Data = new CH26SnFhyo10Data(isTransitionalMeasure);
				if( m_pSnFhyo40Data->GetData(m_pZmSub, m_SnSeq) ){
					return -1;
				}
			}
			//<--------------------
		}

		if( isTransitionalMeasure ){
			if( m_pSnFhyo52Data == NULL ){
				//----->[Y]150106
				if(m_SixKindFlg == TRUE){
					m_pSnFhyo52Data = new CH27SnFhyo50Data(isTransitionalMeasure);
					if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnFhyo52Data = new CH26SnFhyo50Data(isTransitionalMeasure);
					if( m_pSnFhyo52Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//<-----------------
			}
		}
		else{
			if( m_pSnFhyo50Data == NULL ){
				//----->[Y]150106
				if(m_SixKindFlg == TRUE){
					m_pSnFhyo50Data = new CH27SnFhyo50Data(isTransitionalMeasure);
					if( ((CH27SnFhyo50Data*)m_pSnFhyo50Data)->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				else{
					m_pSnFhyo50Data = new CH26SnFhyo50Data(isTransitionalMeasure);
					if( m_pSnFhyo50Data->GetData(m_pZmSub, m_SnSeq) ){
						return -1;
					}
				}
				//<-----------------
			}
		}
	}
//2018.03.13 INSERT START
	if( m_pSnHeadData->Sn_Syuusei&0x10 ){
		if( m_pSnFhyo60Data == NULL ){
			m_pSnFhyo60Data = new CH28SnFhyo60Data();
			if( m_pSnFhyo60Data->GetData( m_pZmSub, m_SnSeq ) ){
				return -1;
			}
		}
	}
//2018.03.13 INSERT END

	return 0;
}


//-----------------------------------------------------------------------------
// ページ切替
//-----------------------------------------------------------------------------
// 引数	move	：	1:順送り、-1:逆送り
//-----------------------------------------------------------------------------
void CH26HyoView::PageChange(int move)
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

//2018.03.21 INSERT START
	if( m_TabMng.GetCount() ){
		if( m_TabMng[selno].tabtype == ID_ICSSH_FHYO60_TAB ){
			if( IsExist_MyNumber_Fuhyo6()&&!m_NoHistory_browse){
				WriteNoHistory(0);
				m_NoHistory_browse = TRUE;
			}
		}
	}
//2018.03.21 INSERT END

//	IndicateSheet( movesel );
//	SetItemPosition( movesel );

//	m_nowTabIdx = movesel;
}

//-----------------------------------------------------------------------------
// 予定26号の前課税期間のセット
//-----------------------------------------------------------------------------
//void CH26HyoView::ZenkiDate_Chsk( char* ZenkSymd , char* ZenkEymd ){
void CH26HyoView::ZenkiDate_Chsk(char* ZenkSymd , char* ZenkEymd, int ZenkSg, int ZenkEg ){
	if( m_pSnChskData ){
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 0, ZenkSymd[0], ZenkSg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 1, ZenkSymd[1], ZenkSg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_FROM, 2, ZenkSymd[2], ZenkSg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO,   0, ZenkEymd[0], ZenkEg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO,   1, ZenkEymd[1], ZenkEg );
		m_pSnChskData->SetYmdDataGen( ID_ZENKI_KAZEIKIKAN_TO,   2, ZenkEymd[2], ZenkEg );
	}
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
int CH26HyoView::GetLastYearTax( CDBNpSub *pBfNpSub, char *pArith )
{
	ASSERT( pBfNpSub );
	ASSERT( pBfNpSub->szvol );
	ASSERT( pArith );

	char	tmpMony[6]={0};

	BOOL	isTransitionalMeasure = FALSE;
	if( pBfNpSub->szvol->SVmzsw ){
		isTransitionalMeasure = TRUE;
	}

	if( isTransitionalMeasure ){

		//------------------------------------------------>yoshida150810
//		if( m_TokuteiFlg == TRUE ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			CH28SnFhyo10Data	*pZnSnFhyo10Data = NULL;
			if( (pZnSnFhyo10Data = new CH28SnFhyo10Data(isTransitionalMeasure)) == NULL ){
				return -1;
			}
			pZnSnFhyo10Data->GetData( pBfNpSub, 0 );
			memmove( tmpMony, pZnSnFhyo10Data->Sn_1F10T, sizeof(tmpMony) );

			if( m_Arith.l_test(tmpMony) > 0 ){
				m_Util.l_calq( tmpMony, tmpMony, 0x10 );
			}

			if( pZnSnFhyo10Data ){
				delete pZnSnFhyo10Data;
				pZnSnFhyo10Data = NULL;
			}
		}
		else{
			CH26SnFhyo10Data	*pZnSnFhyo10Data = NULL;
			if( (pZnSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure)) == NULL ){
				return -1;
			}
			pZnSnFhyo10Data->GetData( pBfNpSub, 0 );
			memmove( tmpMony, pZnSnFhyo10Data->Sn_1F10T, sizeof(tmpMony) );

			if( m_Arith.l_test(tmpMony) > 0 ){
				m_Util.l_calq( tmpMony, tmpMony, 0x10 );
			}

			if( pZnSnFhyo10Data ){
				delete pZnSnFhyo10Data;
				pZnSnFhyo10Data = NULL;
			}

		}
		//--------------------------------------------------------------
		/*
		// 付表1 or 4 合計差引税額(⑩)
		CH26SnFhyo10Data	*pZnSnFhyo10Data = NULL;
		if( (pZnSnFhyo10Data = new CH26SnFhyo10Data(isTransitionalMeasure)) == NULL ){
			return -1;
		}
		pZnSnFhyo10Data->GetData( pBfNpSub, 0 );
		memmove( tmpMony, pZnSnFhyo10Data->Sn_1F10T, sizeof(tmpMony) );

		if( m_Arith.l_test(tmpMony) > 0 ){
			m_Util.l_calq( tmpMony, tmpMony, 0x10 );
		}

		if( pZnSnFhyo10Data ){
			delete pZnSnFhyo10Data;
			pZnSnFhyo10Data = NULL;
		}
		*/
		//<-----------------------------------------------------------------
	}
	else{
		// 本表から計算 差引税額(⑨)
		//----->[Y]150106
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			CH28SnHonpyoData	*pZnSnHonpyoData = NULL;
			if( (pZnSnHonpyoData = new CH28SnHonpyoData()) == NULL ){
				return -1;
			}
			pZnSnHonpyoData->GetData( pBfNpSub, 0 );

			m_Arith.l_add( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SYTX );	// ＋消費税額
			m_Arith.l_add( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KJADJZ );	// ＋控除過大調整税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SIREZ );	// －控除対象仕入税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_HKANZ );	// －返還等対価に係る税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KSITZ );	// －貸倒れに係る税額
			if( m_Arith.l_test(tmpMony) > 0 ){
				m_Util.l_calq( tmpMony, tmpMony, 0x10 );
			}
			if( pZnSnHonpyoData ){
				delete pZnSnHonpyoData;
				pZnSnHonpyoData = NULL;
			}
		}
		else if(m_SixKindFlg == TRUE){
			CH27SnHonpyoData	*pZnSnHonpyoData = NULL;
			if( (pZnSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
				return -1;
			}
			pZnSnHonpyoData->GetData( pBfNpSub, 0 );

			m_Arith.l_add( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SYTX );	// ＋消費税額
			m_Arith.l_add( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KJADJZ );	// ＋控除過大調整税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SIREZ );	// －控除対象仕入税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_HKANZ );	// －返還等対価に係る税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KSITZ );	// －貸倒れに係る税額

			if( m_Arith.l_test(tmpMony) > 0 ){
				m_Util.l_calq( tmpMony, tmpMony, 0x10 );
			}

			if( pZnSnHonpyoData ){
				delete pZnSnHonpyoData;
				pZnSnHonpyoData = NULL;
			}
		}
		else{
			CH26SnHonpyoData	*pZnSnHonpyoData = NULL;
			if( (pZnSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
				return -1;
			}
			pZnSnHonpyoData->GetData( pBfNpSub, 0 );
		
			m_Arith.l_add( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SYTX );	// ＋消費税額
			m_Arith.l_add( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KJADJZ );	// ＋控除過大調整税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_SIREZ );	// －控除対象仕入税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_HKANZ );	// －返還等対価に係る税額
			m_Arith.l_sub( tmpMony, tmpMony, pZnSnHonpyoData->Sn_KSITZ );	// －貸倒れに係る税額

			if( m_Arith.l_test(tmpMony) > 0 ){
				m_Util.l_calq( tmpMony, tmpMony, 0x10 );
			}

			if( pZnSnHonpyoData ){
				delete pZnSnHonpyoData;
				pZnSnHonpyoData = NULL;
			}
		}
		//<------------------
	}

	memmove( pArith, tmpMony, sizeof(tmpMony) );

	return 0;
}

//-----------------------------------------------------------------------------
// 表示中頁の更新処理後の再表示
//-----------------------------------------------------------------------------
void CH26HyoView::IndicateSheet()
{
	IndicateSheet( m_MainTab.GetCurSel() );
}
//-----------------------------------------------------------------------------
// タブへフォーカスセット
//-----------------------------------------------------------------------------

void CH26HyoView::EndJobFocus()
{
	m_MainTab.SetFocus();
}

//-----------------------------------------------------------------------------
// 初期化が必要な年度？('14.05.13)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	初期化が必要
//			FALSE	：	初期化が不要
//-----------------------------------------------------------------------------
BOOL CH26HyoView::IsNeedInitNendo()
{
	BOOL	bRt = FALSE;

	if( m_pZmSub->zvol->ss_ymd >= 20140401 ){
		bRt = TRUE;
	}
	else if( m_pZmSub->zvol->ee_ymd < 20140401 ){
		bRt = FALSE;
	}
	else{
		if( m_pSnHeadData->Sn_SKKBN%2 ){
			// 確定
			if( m_pSnHeadData->Sn_KDAYE >= 20140401 ){
				bRt = TRUE;
			}
		}
		else{
			// 中間
			if( m_pSnHeadData->Sn_MDAYE >= 20140401 ){
				bRt = TRUE;
			}
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 還付申告用データ再設定 ('14.05.17)
//-----------------------------------------------------------------------------
void CH26HyoView::ResetRefundData()
{
	// 情報設定
	if( m_pSnHeadData ){
		CRefundBaseDialog::pSnHeadData = m_pSnHeadData;
	}
	if( m_pSnRefundData ){
		CRefundBaseDialog::pSnRefundData = m_pSnRefundData;
		CRefundBaseDialog::KAZEIU_REC = &m_pSnRefundData->RefundJot[0];
		CRefundBaseDialog::YUNYUT_REC = &m_pSnRefundData->RefundExp[0];
		CRefundBaseDialog::SHIIRE_REC = &m_pSnRefundData->RefundStk[0];
		CRefundBaseDialog::TANAOR_REC = &m_pSnRefundData->RefundTna[0];
		CRefundBaseDialog::KOTEIS_REC = &m_pSnRefundData->RefundKot[0];
	}
//--> '15.02.03 INS START
	CRefundBaseDialog::pUtil = &m_Util;
	CRefundBaseDialog::pZmSub = m_pZmSub;
//<-- '15.02.03 INS END
//--> '15.02.03 INS START
	CDiagControlBase::pUtil = &m_Util;
	CDiagControlBase::pZmSub = m_pZmSub;
//<-- '15.02.03 INS END
}



//-----------------------------------------------------------------------------
//	入力ロック処理
//-----------------------------------------------------------------------------
// 引数	mode		：	０：本表・付表から呼び出し　１：課税取引計算表から呼び出し
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
void CH26HyoView::InputRock(int mode)
{

	// TODO: ここにコマンド ハンドラ コードを追加します。

	if( m_pSnHeadData == NULL ){
		return;
	}
	else if( (m_pShinInfo==NULL) || (m_pShinInfo->pBaseView==NULL) || (m_pShinInfo->pToolBar==NULL) ){
		return;
	}

	// 入力データ保存
	unsigned char	svSn_Sign4 = m_pSnHeadData->Sn_Sign4;

	if(mode == 0){
		if( m_pSnHeadData->DoUnlock(m_pZmSub, this) == IDNO ){
			return;
		}
	}

	m_pShinInfo->pParent->PostMessage( WM_USER_UPDATE_INDICATE, 0 );
	m_pShinInfo->pBaseView->SendMessage( WM_USER_UNLOCK, (WPARAM)0 );

//--> '16.11.22 INS START
	if( (m_SnSeq>0) ){
		// ロック解除
		if( !(m_pSnHeadData->Sn_Sign4&0x01) ){
			m_pSnHeadData->Sn_EXP100 &= ~0x01;
		}
	}
//<-- '16.11.22 INS END

	if(mode == 1){
		if( m_pSnHeadData->Sn_Sign4 & 0x01 ){
			WriteData( FALSE, TRUE );
		}
	}
	else{
		if( (svSn_Sign4!=m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4&0x01) ){
//--> '17.02.09 INS START
			UpdateAllSheet();
//<-- '17.02.09 INS END
			WriteData( FALSE, TRUE );
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
	UpdateSheet( selno );

	if(mode == 1){
		if( m_pSnHeadData->Sn_Sign4 & 0x01 ){
			WriteData( FALSE, FALSE );
		}
	}
	else{
		if( (svSn_Sign4!=m_pSnHeadData->Sn_Sign4) && (m_pSnHeadData->Sn_Sign4&0x01) ){
			WriteData( FALSE, FALSE );
		}
	}

	if( m_SnSeq == 0 ){	// 現申告
		// 再集計
		m_pShinInfo->pParent->SendMessage( WM_USER_RECALQ, (WPARAM)0 );
	}

	// DIAGの属性変更
	ChangeDiagAttr();	// '15.08.28：下で全削除するのに必要？？？？

	// 全削除ではなく、一部削除では駄目？？？
	InitTab( TRUE );
//--> '16.07.22 INS START
	if( mode == 0 ){
		m_pShinInfo->pParent->SendMessage( WM_USER_CHG_LOCK_DISP );
	}
//<-- '16.07.22 INS END

	m_MainTab.SetCurSel( selno );
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );
	SetItemPosition( selno );

	RedrawInputLock();

	return;
}

//-----------------------------------------------------------------------------
// 入力ロックコントロールの再描画	('17.02.27)
//-----------------------------------------------------------------------------
void CH26HyoView::RedrawInputLock()
{
	int selno = m_MainTab.GetCurSel();
	if( (m_TabMng[selno].tabtype==ID_ICSSH_IPPAN_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KANI_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_FHYO10_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_FHYO20_TAB) ||
		(m_TabMng[selno].tabtype==ID_ICSSH_FHYO22_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_FHYO40_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_FHYO50_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_FHYO52_1_TAB) || 
//2018.03.13 UPDATE START
//		(m_TabMng[selno].tabtype==ID_ICSSH_FHYO52_2_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_TKTEI_TAB) ){
		(m_TabMng[selno].tabtype==ID_ICSSH_FHYO52_2_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_FHYO60_TAB) || 
		(m_TabMng[selno].tabtype==ID_ICSSH_KSK_HJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_KSK_KJ_TAB) || (m_TabMng[selno].tabtype==ID_ICSSH_TKTEI_TAB) ){
//2018.03.13 UPDATE END

		if( m_TabMng[selno].pWnd ){
			((CSyzBaseDlg*)m_TabMng[selno].pWnd)->RedrawInputLock();
		}
	}
}

//-----------------------------------------------------------------------------
// 簡易課税で横計ではない例外計算が必要？	('14.10.17)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	必要
//			FALSE	：	不要
//-----------------------------------------------------------------------------
BOOL CH26HyoView::IsNeedExpCalq()
{
	BOOL	bRt = FALSE;

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
	if( KzHoushiki != ID_ICSSH_KANNI ){
		return bRt;
	}
	if( m_pSnHeadData->SVmzsw == 0 ){
		return bRt;
	}

	if( m_pSnFhyo40Data && m_pSnFhyo52Data ){
		if( m_pSnFhyo40Data->IsNeedExpCalq(m_pZmSub) ){
			bRt = TRUE;
		}
//-- '15.05.07 --
//		else if( m_pSnFhyo52Data->IsNeedExpCalq() ){
//			bRt = TRUE;
//		}
//---------------
		if( m_SixKindFlg ){
			if( ((CH27SnFhyo50Data*)m_pSnFhyo52Data)->IsNeedExpCalq() ){
				bRt = TRUE;
			}
		}
		else{
			if( m_pSnFhyo52Data->IsNeedExpCalq() ){
				bRt = TRUE;
			}
		}
//---------------
	}

	return bRt;
}

//--> '15.01.14 INS START
//-----------------------------------------------------------------------------
// 本表のフォーカス位置を顧問先情報と関係の無い位置にずらす
//-----------------------------------------------------------------------------
void CH26HyoView::ShiftHonpyoFocus()
{
	int sel = m_MainTab.GetCurSel();

	if( sel < (int)m_TabMng.GetCount() ){
		if( m_TabMng[sel].tabtype == ID_ICSSH_IPPAN_TAB ){
			if( m_pIppan ){
				m_pIppan->ShiftHonpyoFocus();
			}
			//---->yoshida150810
			else if( m_pIppanEX ){
				m_pIppanEX->ShiftHonpyoFocus();
			}
			//<-----------------
		}
		else if( m_TabMng[sel].tabtype == ID_ICSSH_KANI_TAB ){
			if( m_pKani ){
			}
			else if( m_pKaniEX ){
			}
			//---->yoshida150810
			else if( m_pKaniEX2 ){
			}
			//<------------------
		}
	}
}
//<-- '15.01.14 INS END

//-----------------------------------------------------------------------------
// 既確定税額のクリア('15.01.28)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::ClearAlreadyDecision()
{
	m_Arith.l_clear( m_pSnHonpyoData->Sn_KAKTIZ );
	m_Arith.l_clear( m_pSnHonpyoData->Sn_TKAKTZ );

	if( m_pZmSub ){
		CString	szFilter;
		szFilter.Format( _T("Sn_SEQ=%d"), m_SnSeq );
		if( m_pZmSub->szsnk == NULL ){
			m_pZmSub->SzsnkOpen( szFilter );
		}
		else{
			m_pZmSub->szsnk->Requery( szFilter );
		}
		if( m_pZmSub->szsnk->st != -1 ){
			char	monyBuf1[32]={0}, monyBuf2[32]={0};
			m_pZmSub->szsnk->Edit();
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_KAKTIZ, m_pSnHonpyoData->Sn_KAKTIZ );
			m_Util.val_to_asci( &m_pZmSub->szsnk->Sn_TKAKTZ, m_pSnHonpyoData->Sn_TKAKTZ );
			m_pZmSub->szsnk->Update();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 税務署入力にフォーカス誘導('15.02.03)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::GuideTaxOfficeInput()
{
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第26号様式）
		if( m_pChsk ){
			m_pChsk->SetTaxOfficeInput();
		}
		//----yoshida150810
		else if( m_pChskEX ){
			m_pChskEX->SetTaxOfficeInput();
		}
		//<----------------
	}
	else{
		// 課税処理
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
		if( KzHoushiki == ID_ICSSH_KANNI ){
			int max = (int)m_TabMng.GetCount();
			int page = -1;
			for( int i=0; i<max; i++ ){
				if( m_TabMng[i].tabtype == ID_ICSSH_KANI_TAB ){
					page = i;
					break;
				}
			}

			if( page != -1 ){
				m_MainTab.SetCurSel( page );
				if( m_pKani ){
					m_pKani->SetTaxOfficeInput();
				}
				if( m_pKaniEX ){
					m_pKaniEX->SetTaxOfficeInput();
				}
				//----yoshida150810
				if( m_pKaniEX2 ){
					m_pKaniEX2->SetTaxOfficeInput();
				}
				//<----------------
			}
		}
		else if( KzHoushiki == ID_ICSSH_GENSOKU ){
			int max = (int)m_TabMng.GetCount();
			int page = -1;
			for( int i=0; i<max; i++ ){
				if( m_TabMng[i].tabtype == ID_ICSSH_IPPAN_TAB ){
					page = i;
					break;
				}
			}

			if( page != -1 ){
				m_MainTab.SetCurSel( page );
				if( m_pIppan ){
					m_pIppan->SetTaxOfficeInput();
				}
				//----yoshida150810
				if( m_pIppanEX ){
					m_pIppanEX->SetTaxOfficeInput();
				}
				//<----------------
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入計算表ダイアログ表示('15.02.16)
//-----------------------------------------------------------------------------
// 引数	type	：	表示ダイアログタイプ(0:経過措置なし, 1:経過措置あり)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH26HyoView::SpecPassDlg( int type )
{
	if( m_pShinInfo && m_pShinInfo->pParent ){
		m_pShinInfo && m_pShinInfo->pParent->SendMessage( WM_USER_DISP_SPECPASS, type );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 還付申告？('15.02.24)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	還付申告
//			FALSE	：	還付申告でない
//-----------------------------------------------------------------------------
BOOL CH26HyoView::IsKanpuShinkoku()
{
	BOOL	bRt = FALSE;

	ASSERT( m_pSnHeadData );
	if( m_pSnHeadData->IsMiddleProvisional() || m_pSnHeadData->IsTyukanShinkoku() ){
		;
	}
	else{
		// 通常集計では、原則課税でしか金額は上がってこない
		// 簡易課税では、非連動時に金額が上がってくるのを想定
		char	mony[MONY_BUF_SIZE] = {0};
		m_Arith.l_add( mony, m_pSnHonpyoData->Sn_SIREZ, m_pSnHonpyoData->Sn_HKANZ );
		m_Arith.l_add( mony, mony, m_pSnHonpyoData->Sn_KSITZ );
		m_Arith.l_sub( mony, mony, m_pSnHonpyoData->Sn_SYTX );
		m_Arith.l_sub( mony, mony, m_pSnHonpyoData->Sn_KJADJZ );
		if( m_Arith.l_test(mony) > 0 ){
			bRt = TRUE;
		}
	}

	return bRt;
}

//2016.06.22 INSERT START
//-----------------------------------------------------------------------------
// 還付申告？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	還付申告
//			FALSE	：	還付申告でない
//-----------------------------------------------------------------------------
BOOL CH26HyoView::IsKanpuShinkoku2()
{
	BOOL	bRt = FALSE;

	ASSERT( m_pSnHeadData );
	if( m_pSnHeadData->IsMiddleProvisional() || m_pSnHeadData->IsTyukanShinkoku() ){
		;
	}
	else{
		if( m_pIppan ){
			if( m_pIppan->IsKanpu() == TRUE ){
				bRt = TRUE;
			}
		}
		if( m_pIppanEX ){
			if( m_pIppanEX->IsKanpu() == TRUE ){
				bRt = TRUE;
			}
		}
		if( m_pKani ){
			if( m_pKani->IsKanpu() == TRUE ){
				bRt = TRUE;
			}
		}
		if( m_pKaniEX ){
			if( m_pKaniEX->IsKanpu() == TRUE ){
				bRt = TRUE;
			}
		}
		if( m_pKaniEX2 ){
			if( m_pKaniEX2->IsKanpu() == TRUE ){
				bRt = TRUE;
			}
		}
	}

	return bRt;
}

//2016.06.22 INSERT END

//-----------------------------------------------------------------------------
// 入力位置を保存 ('15.03.02)
//-----------------------------------------------------------------------------
void CH26HyoView::SaveInputPosition()
{
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		if( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN_TAB ){
			if( m_pIppan ){
				m_pIppan->SaveInputPosition();
			}
			//----yoshida150810
			else if( m_pIppanEX ){
				m_pIppanEX->SaveInputPosition();
			}
			//<----------------
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_KANI_TAB ){
			if( m_pKani ){
				m_pKani->SaveInputPosition();
			}
			else if( m_pKaniEX ){
				m_pKaniEX->SaveInputPosition();
			}
			//----yoshida150810
			else if( m_pKaniEX2 ){
				m_pKaniEX2->SaveInputPosition();
			}
			//<----------------
		}
	}
}

//-----------------------------------------------------------------------------
// 入力位置を復元 ('15.03.02)
//-----------------------------------------------------------------------------
void CH26HyoView::RestoreInputPosition()
{
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		if( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN_TAB ){
			if( m_pIppan ){
				m_pIppan->RestoreInputPosition();
			}
			//----yoshida150810
			else if( m_pIppanEX ){
				m_pIppanEX->RestoreInputPosition();
			}
			//<----------------
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_KANI_TAB ){
			if( m_pKani ){
				m_pKani->RestoreInputPosition();
			}
			else if( m_pKaniEX ){
				m_pKaniEX->RestoreInputPosition();
			}
			//----yoshida150810
			else if( m_pKaniEX2 ){
				m_pKaniEX2->RestoreInputPosition();
			}
			//<----------------
		}
	}
}

//-----------------------------------------------------------------------------
// 終了時のフォーカス移動 ('15.03.02)
//-----------------------------------------------------------------------------
void CH26HyoView::MoveFocusInEnd()
{
	int selno = m_MainTab.GetCurSel();
	if( (selno!=-1) && (selno<(int)m_TabMng.GetCount()) ){
		if( m_TabMng[selno].tabtype == ID_ICSSH_IPPAN_TAB ){
			if( m_pIppan ){
				m_pIppan->MoveFocusInEnd();
			}
			//----yoshida150810
			else if( m_pIppanEX ){
				m_pIppanEX->MoveFocusInEnd();
			}
			//<----------------
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_KANI_TAB ){
			if( m_pKani ){
				m_pKani->MoveFocusInEnd();
			}
			else if( m_pKaniEX ){
				m_pKaniEX->MoveFocusInEnd();
			}
			//----yoshida150810
			else if( m_pKaniEX2 ){
				m_pKaniEX2->MoveFocusInEnd();
			}
			//<----------------
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_CHSK_TAB ){
			if( m_pChsk ){
				m_pChsk->MoveFocusInEnd();
			}
//--> '16.01.07 INS START
			if( m_pChskEX ){
				m_pChskEX->MoveFocusInEnd();
			}
//<-- '16.01.07 INS END
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_KSK_HJ_TAB ){
			if( m_pHjnKsk ){
				m_pHjnKsk->MoveFocusInEnd();
			}
			if( m_pHjnKskEX ){
				m_pHjnKskEX->MoveFocusInEnd();
			}
		}
		else if( m_TabMng[selno].tabtype == ID_ICSSH_KSK_KJ_TAB ){
			if( m_pKjnKsk ){
				m_pKjnKsk->MoveFocusInEnd();
			}
			if( m_pKjnKskEX ){
				m_pKjnKskEX->MoveFocusInEnd();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 特定収入計算表 金額連動？ ('15.03.02)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	金額連動中
//			FALSE	：	金額連動無し
//-----------------------------------------------------------------------------
BOOL CH26HyoView::IsSpcDataLinked()
{
	BOOL	bRt = FALSE;

	if( m_pSnHeadData && m_pZmSub ){
		bRt = m_pSnHeadData->IsSpcDataLinked(m_pZmSub);
	}

	return bRt;
}

#ifdef _20150403_TEST_SV_
//-----------------------------------------------------------------------------
// 控除対象仕入税額の再計算
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::RecalqSkj()
{
	if( (m_pShinInfo==NULL) || (m_pShinInfo->pParent==NULL) ){
		return -1;
	}

	SYZTOSPC_DATA	SyzToSpc;
	memset( &SyzToSpc, '\0', sizeof(SYZTOSPC_DATA) );

	CH26SnFhyo20Data	*pTmpSnFhyo20Data = NULL;
	if( m_pSnHeadData->SVmzsw == 1 ){
		pTmpSnFhyo20Data = m_pSnFhyo22Data;
	}
	else{
		pTmpSnFhyo20Data = m_pSnFhyo20Data;
	}

	if( m_pSnHeadData->IsRatioOver95() ){
		//== 計算表５（１） ==
		char	F29A[MONY_BUF_SIZE]={0}, F29B[MONY_BUF_SIZE]={0}, F29C[MONY_BUF_SIZE]={0};
		char	F211A[MONY_BUF_SIZE]={0}, F211B[MONY_BUF_SIZE]={0}, F211C[MONY_BUF_SIZE]={0};
		char	F212A[MONY_BUF_SIZE]={0}, F212B[MONY_BUF_SIZE]={0}, F212C[MONY_BUF_SIZE]={0};

		// 課税仕入れに係る消費税
		memmove( F29A, pTmpSnFhyo20Data->Sn_2F9A, MONY_BUF_SIZE );
		memmove( F29B, pTmpSnFhyo20Data->Sn_2F9B, MONY_BUF_SIZE );
		memmove( F29C, pTmpSnFhyo20Data->Sn_2F9C, MONY_BUF_SIZE );

		// 課税貨物に係る消費税額
		memmove( F211A, pTmpSnFhyo20Data->Sn_2F9A, MONY_BUF_SIZE );
		memmove( F211B, pTmpSnFhyo20Data->Sn_2F9B, MONY_BUF_SIZE );
		memmove( F211C, pTmpSnFhyo20Data->Sn_2F9C, MONY_BUF_SIZE );

		// 棚卸調整
		if( m_pSnTanaData->Sn_MENZsw&0x10 ){
			if( m_pSnHeadData->IsKobetuSiireAnbun() ){	//個別対応時
				char	WORK0[MONY_BUF_SIZE] = {0};
				m_Arith.l_add( F212A, m_pSnTanaData->Sn_menzz[0], m_pSnTanaData->Sn_menzz[1] );
				m_Arith.l_add( F212A, F212A, m_pSnTanaData->Sn_menzz[2] );
				m_Arith.l_add( F212B, m_pSnTanaData->Sn_mnz40[0], m_pSnTanaData->Sn_mnz40[1] );
				m_Arith.l_add( F212B, F212B, m_pSnTanaData->Sn_mnz40[2] );
				m_Arith.l_add( F212C, m_pSnTanaData->Sn_mnz63[0], m_pSnTanaData->Sn_mnz63[1] );
				m_Arith.l_add( F212C, F212C, m_pSnTanaData->Sn_mnz63[2] );
			}
			else{										//比例配分
				memmove( F212A, m_pSnTanaData->Sn_MENZZ, MONY_BUF_SIZE );
				memmove( F212B, m_pSnTanaData->Sn_MNZ40, MONY_BUF_SIZE );
				memmove( F212C, m_pSnTanaData->Sn_MNZ63, MONY_BUF_SIZE );
			}

			if( m_pSnTanaData->Sn_MENZsw & 0x01 ){
				m_Arith.l_neg( F212A );
				m_Arith.l_neg( F212B );
				m_Arith.l_neg( F212C );
			}
		}
		else{
			memset( F212A, '\0', MONY_BUF_SIZE );
			memset( F212B, '\0', MONY_BUF_SIZE );
			memset( F212C, '\0', MONY_BUF_SIZE );
		}

		m_Arith.l_add( SyzToSpc.SpKgz3, F29A, F211A );
		m_Arith.l_add( SyzToSpc.SpKgz3, SyzToSpc.SpKgz3, F212A );
		m_Arith.l_add( SyzToSpc.SpKgz4, F29B, F211B );
		m_Arith.l_add( SyzToSpc.SpKgz4, SyzToSpc.SpKgz4, F212B );
		m_Arith.l_add( SyzToSpc.SpKgz63, F29C, F211C );
		m_Arith.l_add( SyzToSpc.SpKgz63, SyzToSpc.SpKgz63, F212C );
	}
	if( m_pSnHeadData->IsKobetuSiireAnbun() && (m_pSnHeadData->IsRatioOver95()==FALSE) ){
		//== 計算表５（２） ==
		memmove( SyzToSpc.SpKgz3, pTmpSnFhyo20Data->Sn_2F17A, MONY_BUF_SIZE );
		memmove( SyzToSpc.SpKgz4, pTmpSnFhyo20Data->Sn_2F17B, MONY_BUF_SIZE );
		memmove( SyzToSpc.SpKgz63, pTmpSnFhyo20Data->Sn_2F17C, MONY_BUF_SIZE );
	}
	if( (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) && (m_pSnHeadData->IsRatioOver95()==FALSE) ){
		//== 計算表５（３） ==
		memmove( SyzToSpc.SpKgz3, pTmpSnFhyo20Data->Sn_2F18A, MONY_BUF_SIZE );
		memmove( SyzToSpc.SpKgz4, pTmpSnFhyo20Data->Sn_2F18B, MONY_BUF_SIZE );
		memmove( SyzToSpc.SpKgz63, pTmpSnFhyo20Data->Sn_2F18C, MONY_BUF_SIZE );
	}

	m_pShinInfo->pParent->PostMessage( WM_USER_SKJPOST_RECALQ, 0 );

	return 0;
}
#else
//-----------------------------------------------------------------------------
// 控除対象仕入税額の再計算
//-----------------------------------------------------------------------------
// 引数	pSyzToSpc	：	再計算前の控除対象仕入税額
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::RecalqSkj( SYZTOSPC_DATA *pSyzToSpc )
{
	ASSERT( pSyzToSpc );
	if( pSyzToSpc == NULL ){
		return -1;
	}

	if( (m_pShinInfo==NULL) || (m_pShinInfo->pParent==NULL) ){
		return -1;
	}

	SYZTOSPC_DATA	SyzToSpc;
	memset( &SyzToSpc, '\0', sizeof(SYZTOSPC_DATA) );

	m_pShinInfo->pParent->SendMessage( WM_USER_SKJPOST_RECALQ, (WPARAM)pSyzToSpc );

	return 0;
}
#endif

LRESULT CH26HyoView::OnUserRecalqTkkz( WPARAM wParam, LPARAM lParam ) 
{
	if( m_pIppanEX ){
		m_pIppanEX->calq_by_OtherChange( 0, TRUE );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 改正様式をセットする ('15.08.27)
//-----------------------------------------------------------------------------
// 引数	shrevtype	：	セットする改正様式
//-----------------------------------------------------------------------------
void CH26HyoView::SetShRevType( EnumIdIcsShRevType shrevtype )
{
	m_ShRevType = shrevtype;
}

//-----------------------------------------------------------------------------
// 特定課税仕入可能？('15.09.09)
//-----------------------------------------------------------------------------
// 返送値	1	：	特定課税仕入を行っている状態で、特定課税仕入の処理可能
//			0	：	特定課税仕入を行っていない
//			-1	：	特定課税仕入を行っている状態で、特定課税仕入の処理不可能
//			-2	：	特定課税仕入を行っている状態で、通常は特定課税仕入の処理不可能だが
//					前期以前の返還である可能性があるので注意喚起して処理
//-----------------------------------------------------------------------------
int CH26HyoView::IsAbleTkKz()
{
	if( m_pSnHeadData == NULL ){
		ASSERT( !_T("特定課税仕入可能チェックのエラー") );
		return 0;
	}

	int nRt = 0;

	// 非連動？
	if( !(m_pSnHeadData->Sn_Sign4&0x80) ){
		;
	}
	// 特定課税仕入ON？
	else if( m_pSnHeadData->IsNeedTkkzDisp() == FALSE ){
		;
	}
	// 中間申告（予定２６号）？
	else if( m_pSnHeadData->IsMiddleProvisional() ){
		;
	}
	else{
		// データ更新
		int selno = m_MainTab.GetCurSel();
		if( m_pSnHeadData->SVmzsw == 1 ){
			if( ((m_TabMng[selno].tabtype==ID_ICSSH_FHYO10_TAB) && (m_pFhyo10EX!=NULL)) ||
				((m_TabMng[selno].tabtype==ID_ICSSH_FHYO22_TAB) && (m_pFhyo22EX!=NULL)) ){
				UpdateSheet( selno );
			}
		}
		else{
			if( ((m_TabMng[selno].tabtype==ID_ICSSH_TKTEI_TAB) && (m_pTKUtiwake!=NULL)) ||
				((m_TabMng[selno].tabtype==ID_ICSSH_FHYO20_TAB) && (m_pFhyo20EX!=NULL)) ){
				UpdateSheet( selno );
			}
		}

		// 判定
		if( IsRatio95Over() ){
			// 特定課税仕入の返還無し？
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( m_Arith.l_test(((CH28SnFhyo10Data*)m_pSnFhyo10Data)->Sn_1F5_2C) == 0 ){
					nRt = -1;
				}
				else{
					nRt = -2;
				}
			}
			else{
				if( m_Arith.l_test(((CH28SnHonpyoData*)m_pSnHonpyoData)->Sn_TKHKNZ) == 0 ){
					nRt = -1;
				}
				else{
					nRt = -2;
				}
			}
		}
		else{
			nRt = 1;
		}
	}

	return nRt;
}

//-----------------------------------------------------------------------------
// 特定課税仕入を『有』から『無し』へ('15.09.09)
//-----------------------------------------------------------------------------
// 引数	srctab		：	呼び出し元ページ（0:基本情報, 1:本表・付表, 2:課売・特定収入）
//		calltype	：	何の処理で呼び出しているか（1:印刷, 2:プレビュー, 3:処理終了）
//-----------------------------------------------------------------------------
// 返送値	1		：	正常終了(ページ移動無し)
//			2		：	正常終了(ページ移動有り)
//-----------------------------------------------------------------------------
int CH26HyoView::ChangeTkkzUse( int srctab, int calltype )
{
//-- '15.11.27 --
//	int st = ICSMessageBox( _T("課税売上割合が95％を上回っています。特定課税仕入の入力は必要有りません。\n特定課税仕入を無しにしてよろしいですか？"), (MB_YESNO|MB_DEFBUTTON2), 0, 0, this );
//---------------
	int st = ICSMessageBox( _T("課税売上割合が95％以上です。特定課税仕入は、なかったものとされます。\n特定課税仕入を無しにしてよろしいですか？"), (MB_YESNO|MB_DEFBUTTON2), 0, 0, this );
//---------------
	if( st == IDNO ){
		return 1;
	}

	// 特定課税仕入を強制的に無しに！
	if( (m_pShinInfo==NULL) || (m_pShinInfo->pParent==NULL) ){
		// 処理不可
		return 1;
	}

	// 金額のクリア
	if( m_pFhyo20EX ){
		m_pFhyo20EX->ClearTkkzMony();
	}
	if( m_pFhyo22EX ){
		m_pFhyo22EX->ClearTkkzMony();
	}
	if( m_pFhyo10EX ){
		m_pFhyo10EX->ClearTkkzMony();
	}
//--> '15.12.03 INS START -> 保留。
	if( m_pTKUtiwake ){
		m_pTKUtiwake->ClearTkkzMony();
	}
//<-- '15.12.03 INS END

//	UpdateAllSheet();

	m_pShinInfo->pParent->PostMessage( WM_USER_RESET_TKKZ, 0 );

	return 2;
}

//-----------------------------------------------------------------------------
// 正常な特定課税仕入(前期以前の返還)かを確認('15.10.06)
//-----------------------------------------------------------------------------
// 引数	srctab		：	呼び出し元ページ（0:基本情報, 1:本表・付表, 2:課売・特定収入）
//		calltype	：	何の処理で呼び出しているか（1:印刷, 2:プレビュー, 3:処理終了）
//-----------------------------------------------------------------------------
// 返送値	1		：	処理継続
//			0		：	処理中断
//-----------------------------------------------------------------------------
int CH26HyoView::ChkZkTkkzReturn( int srctab, int calltype )
{
//2015.11.11 UPDATE START
//	int st = ICSMessageBox( _T("課税売上割合が95％を上回っていて、かつ特定課税仕入の返還等対価に係る税額に金額の発生があります。\n\n特定課税仕入の返還等対価に係る税額が、前課税期間以前の課税期間において申告された特定課税仕入れに係る対価の\n返還を受けたのであれば問題はありません。\n\n処理を継続してよろしいですか？"), (MB_YESNO|MB_DEFBUTTON2), 0, 0, this );
	int st = ICSMessageBox( _T("課税売上割合が95％を上回っていて、かつ特定課税仕入の返還等対価に係る税額に金額の発生があります。\n\n特定課税仕入の返還等対価に係る税額が、前課税期間以前の課税期間において申告された特定課税仕入に係る対価の\n返還を受けたのであれば問題はありません。\n\n処理を継続してよろしいですか？"), (MB_YESNO|MB_DEFBUTTON2), 0, 0, this );
//2015.11.11 UPDATE START
	if( st == IDNO ){
		return 0;
	}

	return 1;
}

//-----------------------------------------------------------------------------
// 特定課税仕入と課税売上割合のチェック('15.09.10)
//-----------------------------------------------------------------------------
// 引数	srctab		：	呼び出し元ページ（0:基本情報, 1:本表・付表, 2:課売・特定収入）
//		calltype	：	何の処理で呼び出しているか（1:印刷, 2:プレビュー, 3:処理終了）
//-----------------------------------------------------------------------------
// 返送値	0		：	処理継続可能
//			-1		：	処理継続不可
//-----------------------------------------------------------------------------
int CH26HyoView::ChkTkkzAndKzwari( int srctab, int calltype )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CH26HyoView::ChkTkkzAndKzwari_START\n");
	DbgViewTrace(hoge);
#endif

	int nRt = 0;

	// チェック対象の環境？
//-- '15.11.14 --
//	if( IsAbleTkKz() == -1 ){
//		int st = ChangeTkkzUse( srctab, calltype );
//		if( (srctab==1) && (calltype==3) ){
//			if( st == 1 ){
//				// タブ移動無し
//				SetItemPosition( m_MainTab.GetCurSel() );
//			}
//		}
//		nRt = -1;
//	}
//---------------
	int aSt = IsAbleTkKz();
	if( aSt == -1 ){
		int st = ChangeTkkzUse( srctab, calltype );
		if( (srctab==1) && (calltype==3) ){
			if( st == 1 ){
				// タブ移動無し
				SetItemPosition( m_MainTab.GetCurSel() );
			}
		}
		nRt = -1;
	}
	else if( aSt == -2 ){
		if( ChkZkTkkzReturn(srctab, calltype) == 0 ){
			nRt = -1;
		}
	}
//---------------

#ifdef _DBG_VIEW_START_
	hoge.Format("CH26HyoView::ChkTkkzAndKzwari_END\n");
	DbgViewTrace(hoge);
#endif

	return nRt;
}

//-----------------------------------------------------------------------------
// 課税売上割合が95%以上？('15.09.10)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	95%以上
//			FALSE	：	95%未満
//-----------------------------------------------------------------------------
BOOL CH26HyoView::IsRatio95Over()
{
	BOOL bRt = FALSE;

	if( m_pFhyo22EX ){
		if( m_pFhyo22EX->GetCalqedKzwari() >= 9500 ){
			bRt = TRUE;
		}
	}
	else if( m_pFhyo20EX ){
		if( m_pFhyo20EX->GetCalqedKzwari() >= 9500 ){
			bRt = TRUE;
		}
	}

	return bRt;
}

void CH26HyoView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	int selno = m_MainTab.GetCurSel();
	if( selno < (int)m_TabMng.GetCount() ){
		switch( m_TabMng[selno].tabtype ){
			case ID_ICSSH_IPPAN_TAB:
				if( m_pIppanEX ){
					m_pIppanEX->MoveDiagMNEdit();
				}
				break;
			case ID_ICSSH_KANI_TAB:
				if( m_pKaniEX2 ){
					m_pKaniEX2->MoveDiagMNEdit();
				}
				break;
			case ID_ICSSH_KSK_HJ_TAB:
				if( m_pHjnKskEX ){
					m_pHjnKskEX->MoveDiagMNEdit();
				}
				break;
			case ID_ICSSH_KSK_KJ_TAB:
				if( m_pKjnKskEX ){
					m_pKjnKskEX->MoveDiagMNEdit();
				}
				break;
			case ID_ICSSH_CHSK_TAB:
				if( m_pChskEX ){
					m_pChskEX->MoveDiagMNEdit();
				}
				break;
			default:
				break;
		}
	}
	ICSFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

//2016.06.22 INSERT START
int CH26HyoView::Message_BlankKinyu_Kanpu()
{
	char tmp_char1[128]={0},tmp_char2[128]={0},tmp_char3[128]={0},tmp_char4[128]={0},tmp_char5[128]={0};
	CString	msg;
	BOOL blank1 = FALSE,blank2 = FALSE,blank3 = FALSE,blank4 = FALSE,blank5 = FALSE; //空白判定

	CRefundBaseDialog *base = NULL;

	if( m_pRefundKjn01 ){
		base = m_pRefundKjn01;
	}else if( m_pRefundHjn01 ){
		base = m_pRefundHjn01;
	}

	if( base ){

		memmove( tmp_char1, base->refundvol.KPEbnk, sizeof( base->refundvol.KPEbnk ));	//銀行名称
		memmove( tmp_char2, base->refundvol.KPEstn, sizeof( base->refundvol.KPEstn ));	//支店名称
		memmove( tmp_char3, base->refundvol.KPEykn, sizeof( base->refundvol.KPEykn ));	//預金種別
		if( base->refundvol.KPEbdm ){ //ゆうちょ
			memmove( tmp_char4, base->refundvol.KPEyn1, sizeof( base->refundvol.KPEyn1 ));//口座番号１
			memmove( tmp_char5, base->refundvol.KPEyn2, sizeof( base->refundvol.KPEyn2 ));//口座番号２
		}else{ //銀行
			l_unpac( tmp_char4, base->refundvol.KPEkoz, 10 );//口座番号
		}


		if( m_Util.MixSpaceCutLength(tmp_char1, 20) == 0 ){	//銀行名称
			blank1 = TRUE;
		}
		if( m_Util.MixSpaceCutLength(tmp_char2, 20) == 0 ){	//支店名称
			if(( base->refundvol.KPEssg != 0 )&&( base->refundvol.KPEssg != 2 )){ //本店、本所は空白OK
				blank2 = TRUE;
			}
		}
		if( m_Util.MixSpaceCutLength(tmp_char3, 8) == 0 ){	//預金種別
			blank3 = TRUE;
		}
		if( base->refundvol.KPEbdm ){ //ゆうちょ
			if( m_Util.MixSpaceCutLength(tmp_char4, 5) == 0 ){//口座番号１
				blank4 = TRUE;
			}
			if( m_Util.MixSpaceCutLength(tmp_char5, 8) == 0 ){//口座番号２
				blank5 = TRUE;
			}
		}else{ //銀行
			if( m_Util.MixSpaceCutLength(tmp_char4, 10) == 0 ){//口座番号
				blank4 = TRUE;
			}
		}


		if( base->refundvol.KPEbdm ){ //ゆうちょ
			if( blank1&&blank2&&blank3&&blank4&&blank5){
				msg = _T("還付申告ですが、輸出取引等に利用する主な金融機関（還付付表１）が登録されていません！");
				msg += _T("\n輸出取引等に利用する主な金融機関を入力しますか？");
			}else 
			if( blank1||blank2||blank3||blank4||blank5){
				msg = _T("輸出取引等に利用する主な金融機関（還付付表１）の登録において、以下の内容を再確認してください。\n");
				if(blank1){
					msg += _T("\n郵便局名（入力されていません）");
				}
				if(blank2){
					msg += _T("\n支店名（入力されていません）");
				}
				if(blank3){
					msg += _T("\n預金種別（入力されていません）");
				}
				if(blank4||blank5){
					msg += _T("\nゆうちょ銀行の貯金記号番号（入力されていません）");
				}
				msg += _T("\n\n輸出取引等に利用する主な金融機関を入力しますか？");
			}
		}else{
			if( blank1&&blank2&&blank3&&blank4){
				msg = _T("還付申告ですが、輸出取引等に利用する主な金融機関（還付付表１）が登録されていません！");
				msg += _T("\n輸出取引等に利用する主な金融機関を入力しますか？");
			}else
			if( blank1||blank2||blank3||blank4){
				msg = _T("輸出取引等に利用する主な金融機関（還付付表１）の登録において、以下の内容を再確認してください。\n");
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
				msg += _T("\n\n輸出取引等に利用する主な金融機関を入力しますか？");
			}
		}
		if( msg.IsEmpty() == FALSE ){
			return ICSMessageBox(msg, MB_YESNO, 0);
		}
	}
	
	return 0;
}

int CH26HyoView::Message_BlankKinyu_Kousei()
{

	if( m_pHjnKsk || m_pKjnKsk || m_pHjnKskEX || m_pKjnKskEX ){


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
//2016.06.22 INSERT END
//2018.03.13 INSERT START
void CH26HyoView::EditOff_Fuhyo6()
{
	if( m_pFhyo60 ){
		m_pFhyo60->OnEditOFFCallBack( m_pFhyo60->m_Focus );
	}
}

int CH26HyoView::Message_NofuKanpuGokei()
{
	CString msg;

	if( m_pFhyo60 ){
		int ret = m_pFhyo60->NofuKanpuGokeiCheck();
		if( ret ){
			msg += "付表６\n";
			msg += "下記の金額が一致していません。金額を確認してください。\n\n";
			if( ret&0x01 ){
				msg += "⑨の各人の納付税額(消費税)の合計金額と②の納める消費税\n";
			}
			if( ret&0x02 ){
				msg += "⑩の各人の納付税額(地方消費税)の合計金額と③の納める地方消費税\n";
			}
			if( ret&0x04 ){
				msg += "⑫の各人の還付税額(消費税)の合計金額と⑤の還付される消費税\n";
			}
			if( ret&0x08 ){
				msg += "⑬の各人の還付税額(地方消費税)の合計金額と⑥の還付される地方消費税\n";
			}

			if( msg.IsEmpty() == FALSE ){
				return ICSMessageBox(msg, MB_YESNO, 0);
			}
		}
	}

	return 0;
}

int CH26HyoView::Message_MyNumber_error()
{
	if( m_pFhyo60 ){
		int ret = m_pFhyo60->MyNumber_CheckFromDisp();
		if( ret < 0 ){
			//ICSMessageBox( "付表６　相続人等に関する事項\n個人番号に不備があります。", MB_ICONSTOP );
			//return ret;
			return -1;
		}		
	}

	return 0;
}

int CH26HyoView::Message_MyNumber_overlap()
{
	if( m_pFhyo60 ){
		int ret = m_pFhyo60->MyNumberCheck();
		if( ret == -1 ){
			//ICSMessageBox( "付表６　相続人等に関する事項\n個人番号が重複しています。", MB_ICONSTOP );
			return ret;
		}		
	}

	return 0;
}

int CH26HyoView::Message_SozokuWariai()
{
	if( m_pFhyo60 ){
		int ret = m_pFhyo60->SozokubunCheck();
		if( ret == -1 ){
			//ICSMessageBox( "付表６　相続人等に関する事項\n承継割合の合計が『 1 』ではありません。", MB_ICONSTOP );
			return ret;
		}
	}

	return 0;
}


//指定された地点から、次の改行までの位置を検索する(改行コードの前までのバイト数を返す)
int CH26HyoView::Getindex_toNewLineCode( int start, int *end, CString str )
{
	int ret = -1;
	char code1 = '\r';
	char code2 = '\n';
	*end = str.GetLength();

	for( int idx = start; idx < str.GetLength(); idx++ ){
		if( str.Mid( idx, 1 ) == '\0' ){
			*end = idx-start;
			ret = -1;
			break;
		}

		if( str.Mid( idx, 1 ) == code1 ){
			if( str.Mid( idx+1, 1 ) == code2 ){
				*end = idx-start;
				ret = 0;
				break;
			}
		}
	}

	return ret;
}

//2018.03.13 INSERT END

//-----------------------------------------------------------------------------
// 還付金融機関入力にフォーカス誘導('16.12.26)
//-----------------------------------------------------------------------------
// 引数	dsInput	：	移動先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::GuideKanpuKinyu( int dsInput )
{
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第26号様式）
		return 0;
	}
	else{
		if( dsInput == ID_SYZ_MOVE_TO_HONPYO_BANK ){
			EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
			if( KzHoushiki == ID_ICSSH_KANNI ){
				int max = (int)m_TabMng.GetCount();
				int page = -1;
				for( int i=0; i<max; i++ ){
					if( m_TabMng[i].tabtype == ID_ICSSH_KANI_TAB ){
						page = i;
						break;
					}
				}

				if( page != -1 ){
					m_MainTab.SetCurSel( page );
					if( m_pKani ){
						m_pKani->SetKanpuKinyuInput();
					}
					if( m_pKaniEX ){
						m_pKaniEX->SetKanpuKinyuInput();
					}
					if( m_pKaniEX2 ){
						m_pKaniEX2->SetKanpuKinyuInput();
					}
				}
			}
			else if( KzHoushiki == ID_ICSSH_GENSOKU ){
				int max = (int)m_TabMng.GetCount();
				int page = -1;
				for( int i=0; i<max; i++ ){
					if( m_TabMng[i].tabtype == ID_ICSSH_IPPAN_TAB ){
						page = i;
						break;
					}
				}

				if( page != -1 ){
					m_MainTab.SetCurSel( page );
					if( m_pIppan ){
						m_pIppan->SetKanpuKinyuInput();
					}
					if( m_pIppanEX ){
						m_pIppanEX->SetKanpuKinyuInput();
					}
				}
			}
		}
		else if( dsInput == ID_SYZ_MOVE_TO_KOUSEI_BANK ){
			if( m_pSnHeadData->Sn_Syuusei&0x02 ){
				int setTabtype = ID_ICSSH_KSK_HJ_TAB;
				if( m_pSnHeadData->IsSoleProprietor() ){
					setTabtype = ID_ICSSH_KSK_KJ_TAB;
				}

				int max = (int)m_TabMng.GetCount();
				int page = -1;
				for( int i=0; i<max; i++ ){
					if( m_TabMng[i].tabtype == setTabtype ){
						page = i;
						break;
					}
				}

				if( page != -1 ){
					m_MainTab.SetCurSel( page );
					if( m_pSnHeadData->IsSoleProprietor() ){
						if( m_pKjnKsk ){
							m_pKjnKsk->SetKanpuKinyuInput();
						}
						if( m_pKjnKskEX ){
							m_pKjnKskEX->SetKanpuKinyuInput();
						}
					}
					else{
						if( m_pHjnKsk ){
							m_pHjnKsk->SetKanpuKinyuInput();
						}
						if( m_pHjnKskEX ){
							m_pHjnKskEX->SetKanpuKinyuInput();
						}
					}
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 課税取引金額計算書から転記時のフォーカス移動('16.12.27)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::MoveFocusInTaxTenki()
{
	int max = (int)m_TabMng.GetCount();
	int page = -1;
	for( int i=0; i<max; i++ ){
		if( m_TabMng[i].tabtype == ID_ICSSH_FHYO50_TAB ){
			page = i;
			break;
		}
	}

	if( page != -1 ){
		if( m_pFhyo50 ){
			m_pFhyo50->MoveFocusInTaxTenki();
		}
		if( m_pFhyo50EX ){
			m_pFhyo50EX->MoveFocusInTaxTenki();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 予定申告の月数クリア('17.02.09)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::ClearYoteiMonth()
{
	if( m_pSnChskData ){
		m_pSnChskData->Sn_BBMON = 0;
		m_pSnChskData->Sn_BSMON = 0;
	}

	if( m_pZmSub ){
		CString	szFilter;
		szFilter.Format( _T("Sn_SEQ=%d"), m_SnSeq );
		if( m_pZmSub->szsnk == NULL ){
			m_pZmSub->SzsnkOpen( szFilter );
		}
		else{
			m_pZmSub->szsnk->Requery( szFilter );
		}
		if( m_pZmSub->szsnk->st != -1 ){
			m_pZmSub->szsnk->Edit();
			m_pZmSub->szsnk->Sn_BBMON = 0;
			m_pZmSub->szsnk->Sn_BSMON = 0;
			m_pZmSub->szsnk->Update();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 終了処理フラグの切り替え('18.06.22)
//-----------------------------------------------------------------------------
// 引数	flg	：	セットするフラグ
//-----------------------------------------------------------------------------
void CH26HyoView::SetEndStartFlg( BOOL flg )
{
	m_isEndStart = flg;
}

//2018.08.21 INSERT START
int CH26HyoView::IsExist_MyNumber_Fuhyo6()
{
	int ret = 0;

	if( m_pFhyo60 ){
		if( m_pFhyo60->MyNumber_IsAllEmpty() == TRUE ){
			ret = 0;
		}else{
			ret = 1;
		}
	}

	return ret;
}

/////////////////////////////////////////////////////////
//処理概要：マイナンバーの履歴を書き込む(GetData()からコピー！！)
//
//引数　　：write_sgn  0…閲覧　1…出力
/////////////////////////////////////////////////////////
void CH26HyoView::WriteNoHistory(int write_sgn)
{
	// 履歴情報作成
	NOHISTTBL nhtbl;
	NTBMOD8_InitNoHistoryTbl( 0, &nhtbl );

	nhtbl.mst_code = m_pZmSub->zvol->v_cod;
	nhtbl.mst_apno = m_pZmSub->zvol->apno;

	memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
	memmove( nhtbl.tgt_char, m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );

	CString mst_nam_str,kessan_str;
	
	mst_nam_str = nhtbl.mst_nam;
	mst_nam_str = mst_nam_str.TrimRight();
	m_pSnHeadData->GetString_KessanKikan( &kessan_str );
	strcpy_s( nhtbl.mst_nam, sizeof(nhtbl.mst_nam), mst_nam_str + kessan_str );

	strcpy_s( nhtbl.gy_nam, sizeof(nhtbl.gy_nam), _T("消費税申告書") );

	switch( write_sgn ){
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
	nhtblary.Add( nhtbl );

	// 履歴テーブルにセット
	NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );

}
//2018.08.21 INSERT END

//-----------------------------------------------------------------------------
// 送付不要をカットする様式？
//-----------------------------------------------------------------------------
// 返送値	true	：	送付不要をカット
//			false	：	送付不要をカットしない
//-----------------------------------------------------------------------------
bool CH26HyoView::IsNoDeliForm()
{
	bool	bRt = false;

	ASSERT( m_pSnHeadData );
	if( m_pSnHeadData->IsSoleProprietor() ){
		ASSERT( m_pZmSub );
		ASSERT( m_pZmSub->zvol );
		EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			if( (m_ShRevType>=ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->Sn_KDAYS>=ICS_SH_MYNUMBER_REFORM_DAY) ){
				bRt = true;
			}
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){
			if( (m_ShRevType>=ID_ICSSH_MYNUMBER_REVTYPE) || (m_SixKindFlg && (m_pSnHeadData->IsPrintWithMynoHonpyo())) ){
				if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ){
					bRt = true;
				}
			}
		}
	}

	return bRt;
}

#ifdef _20190626_EXTEND_
//-----------------------------------------------------------------------------
// 平成継続出力？
//-----------------------------------------------------------------------------
// 引数	IsJudgeKjn	：	個人事業かどうかを判断基準とするかどうか
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//-----------------------------------------------------------------------------
bool CH26HyoView::IsJcContinuePrint( bool IsJudgeKjn )
{
	if( m_JcContinuePrint == -1 ){
		m_JcContinuePrint = 0;
		if( m_Util.IsJcContinuePrint(m_pSnHeadData, IsJudgeKjn) ){
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
//-----------------------------------------------------------------------------
// 平成継続出力？
//-----------------------------------------------------------------------------
// 引数	IsJudgeKjn	：	個人事業かどうかを判断基準とするかどうか
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//-----------------------------------------------------------------------------
bool CH26HyoView::IsJcContinuePrint(bool IsJudgeKjn)
{
	// 課税期間の末日等、変動する要素が加味されるようになったので、その都度判定するように変更
	if( m_Util.IsJcContinuePrint(m_pSnHeadData, IsJudgeKjn) ){
		return true;
	}
	else{
		return false;
	}
}
#endif

//-----------------------------------------------------------------------------
// 平成･令和の元号の選択可・不可
//-----------------------------------------------------------------------------
// 引数	IsJudgeKjn	：	個人事業かどうかを判断基準とするかどうか
//-----------------------------------------------------------------------------
// 返送値	true	：	選択可
//			false	：	選択不可
//-----------------------------------------------------------------------------
bool CH26HyoView::IsSwitch_HeiseiReiwa( bool IsJudgeKojin )
{
	bool ret = true;

	if(m_Util.GetPrePrintGengo(m_pSnHeadData, IsJudgeKojin) != 2){
		ret = false;
	}

	return ret;
}

//-----------------------------------------------------------------------------
// 令和プレプリント情報の初期化
//-----------------------------------------------------------------------------
void CH26HyoView::InitReiwaPrnt()
{
	m_isReiwaPrnt = false;
	if( m_Util.GetPrePrintGengo(m_pSnHeadData) == 2 ){
		m_isReiwaPrnt = true;
	}
}

//-----------------------------------------------------------------------------
// 令和プレプリント？
//-----------------------------------------------------------------------------
// 返送値	true	：	令和プレプリント
//			false	：	平成プレプリント
//-----------------------------------------------------------------------------
bool CH26HyoView::IsReiwaPrnt()
{
	return m_isReiwaPrnt;
}

//-----------------------------------------------------------------------------
// データの引き継ぎ
//-----------------------------------------------------------------------------
// 引数	srcVer		：	引き継ぎ元バージョン
//		shRevType	：	改正様式タイプ
//		sixKindFlg	：	簡易課税第六種以降？
//		snSeq		：	履歴シーケンス
//		pZmsub		：	公益クラス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26HyoView::TakeOverData( int srcVer, EnumIdIcsShRevType shRevType, BOOL sixKindFlg, int snSeq, CDBNpSub *pZmsub )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CH26HyoView::TakeOverData_START\n");
	DbgViewTrace(hoge);
#endif

	if( m_pSnHonpyoData == NULL ){
		// このデータの読み込み部分が各所に散らばっているので、一か所にまとめたい
		if( shRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( (m_pSnHonpyoData = new CH28SnHonpyoData()) == NULL ){
				return -1;
			}
			if( ((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(pZmsub, snSeq) ){
				return -1;
			}
		}
		else{
			if( sixKindFlg ){
				if( (m_pSnHonpyoData = new CH27SnHonpyoData()) == NULL ){
					return -1;
				}
				if( ((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(pZmsub, snSeq) ){
					return -1;
				}
			}
			else{
				if( (m_pSnHonpyoData = new CH26SnHonpyoData()) == NULL ){
					return -1;
				}
				if( m_pSnHonpyoData->GetData(pZmsub, snSeq) ){
					return -1;
				}
			}
		}
	}

	//'19.11.07
	else {
		if (shRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) {
			if (((CH28SnHonpyoData*)m_pSnHonpyoData)->GetData(pZmsub, snSeq)) {
				return -1;
			}
		}
		else {
			if (sixKindFlg) {
				if (((CH27SnHonpyoData*)m_pSnHonpyoData)->GetData(pZmsub, snSeq)) {
					return -1;
				}
			}
			else {
				if (m_pSnHonpyoData->GetData(pZmsub, snSeq)) {
					return -1;
				}
			}
		}
	}


	if( m_pSnHonpyoData ){
		m_pSnHonpyoData->TakeOverData(pZmsub, snSeq, m_pSnHeadData, srcVer);
		m_pSnHonpyoData->SetData(pZmsub, snSeq);
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("CH26HyoView::TakeOverData_END\n");
	DbgViewTrace(hoge);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化完了フラグの切り替え
//-----------------------------------------------------------------------------
// 引数	flg	：	セットするフラグ
//-----------------------------------------------------------------------------
void CH26HyoView::SetInitedFlg(bool flg)
{
	m_isInited = flg;
}


//==================================================
// 更正の請求書の提出用　元号番号取得
//--------------------------------------------------
// 返送値	元号番号
//==================================================
int CH26HyoView::GetGengoNo_Ksk()
{
	int gengoNo = 0;
	int sgn = 0;

	if ( !m_pSnKskData ) {
		m_pSnKskData = new CH26SnKskData();
		if ( m_pSnKskData->GetData ( m_pZmSub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}
		sgn = 1;
	}

	gengoNo = m_pSnKskData->KsData.KSdata[0];
	if ( ( gengoNo == 0 ) || IsReiwaPrnt() ) {
		gengoNo = ID_ICSSH_REIWA_GENGO;
	}

	if ( sgn ) {
		delete m_pSnKskData;
		m_pSnKskData = NULL;
	}

	return gengoNo;
}

bool CH26HyoView::CheckInvno( int sgn )
{
	return true;
}