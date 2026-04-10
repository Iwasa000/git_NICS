#include "stdafx.h"
#include "resource.h"
#include "TaxationList31View.h"

#include "ShinSpecific1_31.h"
#include "ShinSpecific1_31Ex.h"
#include "ShinSpecific2_31.h"
#include "ShinSpecific2_31Ex.h"
#include "ShinSpecific22_31.h"
#include "ShinSpecific22_2_31.h"
#include "ShinSpecific22_2_31Ex.h"
#include "ShinSpecific34_31.h"
#include "ShinSpecific51_31.h"
#include "ShinSpecific51_31Ex.h"
#include "ShinSpecific51_31Ex2.h" // 20231124
#include "ShinSpecific52_31.h"
#include "ShinSpecific52_31Ex.h"
#include "ShinSpecific52_31Ex2.h" // 20231128
#include "ShinSpecific53_31.h"
#include "ShinSpecific53_31Ex.h"
#include "ShinSpecific53_31Ex2.h" // 20231128
#include "ShinSpecific52_1_31.h" // 20231128
#include "ShinSpecific52_2_31.h" // 20231128
#include "ShinSpecific52_31_31.h" // 20231128
#include "ShinSpecific52_32_31.h" // 20231128
#include "ShinSpecific52_33_31.h" // 20231128
#include "ShinSpecific52_41_31.h" // 20231128
#include "ShinSpecific52_42_31.h" // 20231128
#include "ShinSpecific52_43_31.h" // 20231128


//==========================================
// 特定収入タブの初期化
//------------------------------------------
// 引数
//		bDelete		:		内部を消去するか
//------------------------------------------
// 返送値
//		0			:		正常終了
//==========================================
int CTaxationList31View::InitSpecificTab ( BOOL bDelete )
{
	if ( bDelete ) {
		// 内部クラス破棄
		DeleteInsideSpcDlg();
		// 全削除
		m_MainTab.DeleteAllItems();
	}

	SHIN_TABMAX = 0;
	ICSDialog *ptmpTab[16] = { 0 };
	SH_TABMNG sh_Tabmng;
	int tabIdx = 0;

	long eymd = 0;
	BOOL syzSign = FALSE;

	memset( m_ListType, '\0', sizeof ( m_ListType ) );

	// 令和3年12月31日以降の課税期間
	if( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		eymd = m_pSnHeadData->Sn_KDAYE;
	}

	if( ( m_pSnHeadData->SVmzsw == 1 ) || ( eymd < R03YEAR ) ) {
		// 旧税率ありもしくは令和3年12月1日より前
		syzSign = TRUE;
	}

	// 帳票データ
	if ( !m_pSpcListData ) {
		if ( ( m_pSpcListData = new CH31SpcListData ( m_pSnHeadData->SVmzsw ? TRUE : FALSE ) ) == NULL ) {
			return -1;
		}

		if ( m_pSpcListData->InitSpData ( m_pZmsub ) ) {
			return -1;
		}

		if ( m_pSpcListData->GetData ( m_pZmsub, m_pSnHeadData, m_SnSeq ) ) {
			return -1;
		}
	}

	// 計算表１
	if( syzSign ) {
		// 旧税率あり
		memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
		if ( !m_pShinSpecific131 ) {
			m_pShinSpecific131 = new CShinSpecific1_31 ( this );
			m_pShinSpecific131->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
			m_pShinSpecific131->Create ( IDD_DIALOG_K1_31, &m_MainTab );
		}

		ptmpTab[tabIdx] = m_pShinSpecific131;
		m_MainTab.InsertItem ( tabIdx, _T( "計算表１" ), m_pShinSpecific131->m_hWnd );
		sh_Tabmng.pWnd = m_pShinSpecific131;

		sh_Tabmng.tabtype = ID_ICSSH_SPC31_1_TAB;
		m_TabMng.Add ( sh_Tabmng );

		m_ListType[tabIdx] = ID_ICSSH_SPC31_1_TAB;
		tabIdx++;
	}
	else {
		// 旧税率なし
		memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
		if( !m_pShinSpecific131Ex ) {
			m_pShinSpecific131Ex = new CShinSpecific1_31Ex( this );
			m_pShinSpecific131Ex->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith );
			m_pShinSpecific131Ex->Create( IDD_DIALOG_K1_31_ex, &m_MainTab );
		}
		ptmpTab[tabIdx] = m_pShinSpecific131Ex;
		m_MainTab.InsertItem( tabIdx, _T( "計算表１" ), m_pShinSpecific131Ex->m_hWnd );
		sh_Tabmng.pWnd = m_pShinSpecific131Ex;

		sh_Tabmng.tabtype = ID_ICSSH_SPC31_1_TAB;
		m_TabMng.Add( sh_Tabmng );

		m_ListType[tabIdx] = ID_ICSSH_SPC31_1_TAB;
		tabIdx++;
	}

	
	// 特定収入計算表２（１）
	if( syzSign ) {
		// 旧税率あり
		memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
		if ( !m_pShinSpecific231 ) {
			m_pShinSpecific231 = new CShinSpecific2_31 ( this );
			m_pShinSpecific231->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
			m_pShinSpecific231->Create ( IDD_DIALOG_K21_31, &m_MainTab );
		}

		ptmpTab[tabIdx] = m_pShinSpecific231;
		m_MainTab.InsertItem ( tabIdx, _T( "計算表２(1)" ), m_pShinSpecific231->m_hWnd );
		sh_Tabmng.pWnd = m_pShinSpecific231;

		sh_Tabmng.tabtype = ID_ICSSH_SPC31_2_TAB;
		m_TabMng.Add ( sh_Tabmng );

		m_ListType[tabIdx] = ID_ICSSH_SPC31_2_TAB;
		tabIdx++;
	}
	else {
		// 旧税率なし
		memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
		if( !m_pShinSpecific231Ex ) {
			m_pShinSpecific231Ex = new CShinSpecific2_31Ex( this );
			m_pShinSpecific231Ex->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith );
			m_pShinSpecific231Ex->Create( IDD_DIALOG_K21_31_ex, &m_MainTab );
		}

		ptmpTab[tabIdx] = m_pShinSpecific231Ex;
		m_MainTab.InsertItem( tabIdx, _T( "計算表２(1)" ), m_pShinSpecific231Ex->m_hWnd );
		sh_Tabmng.pWnd = m_pShinSpecific231Ex;

		sh_Tabmng.tabtype = ID_ICSSH_SPC31_2_TAB;
		m_TabMng.Add( sh_Tabmng );

		m_ListType[tabIdx] = ID_ICSSH_SPC31_2_TAB;
		tabIdx++;
	}

	// 課税売上割合95%未満かつ個別対応方式
	// 課税売上割合に準ずる割合を使用
	//if ( !( m_pSpcListData->IsRatioOver95 ( m_pSnHeadData ) ) && ( m_pSnHeadData->IsKobetuSiireAnbun() ) ) {
	if ( ( m_pSnHeadData->IsKobetuSiireAnbun() ) && ( !( m_pSpcListData->IsRatioOver95 ( m_pSnHeadData ) )|| ( m_pSnHeadData->IsUseSemiUriRatio() == TRUE ) ) ) {
		if( syzSign ) {
			// 特定収入計算表２（２） 表面
			memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
			if ( !m_pShinSpecific2231 ) {
				m_pShinSpecific2231 = new CShinSpecific22_31 ( this );
				m_pShinSpecific2231->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
				m_pShinSpecific2231->Create ( IDD_DIALOG_K22_31, &m_MainTab );
			}

			ptmpTab[tabIdx] = m_pShinSpecific2231;
			m_MainTab.InsertItem ( tabIdx, _T( "計算表２(2) 表面" ), m_pShinSpecific2231->m_hWnd );
			sh_Tabmng.pWnd = m_pShinSpecific2231;
	
			sh_Tabmng.tabtype = ID_ICSSH_SPC31_22_TAB;
			m_TabMng.Add ( sh_Tabmng );

			m_ListType[tabIdx] = ID_ICSSH_SPC31_22_TAB;
			tabIdx++;

			// 特定収入計算表２（２） 裏面
			memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
			if ( !m_pShinSpecific22_2_31 ) {
				m_pShinSpecific22_2_31 = new CShinSpecific22_2_31 ( this );
				m_pShinSpecific22_2_31->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
				m_pShinSpecific22_2_31->Create ( IDD_DIALOG_K22_2_31, &m_MainTab );
			}

			ptmpTab[tabIdx] = m_pShinSpecific22_2_31;
			m_MainTab.InsertItem ( tabIdx, _T( "計算表２(2) 裏面" ), m_pShinSpecific22_2_31->m_hWnd );
			sh_Tabmng.pWnd = m_pShinSpecific22_2_31;

			sh_Tabmng.tabtype = ID_ICSSH_SPC31_22_2_TAB;
			m_TabMng.Add ( sh_Tabmng );

			m_ListType[tabIdx] = ID_ICSSH_SPC31_22_2_TAB;
			tabIdx++;
		}
		else {
			// 旧税率なし
			// 特定収入計算表２（２）
			memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
			if( !m_pShinSpecific22_2_31Ex ) {
				m_pShinSpecific22_2_31Ex = new CShinSpecific22_2_31Ex( this );
				m_pShinSpecific22_2_31Ex->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
				m_pShinSpecific22_2_31Ex->Create( IDD_DIALOG_K22_2_31_ex, &m_MainTab );
			}

			ptmpTab[tabIdx] = m_pShinSpecific22_2_31Ex;
			m_MainTab.InsertItem( tabIdx, _T( "計算表２(2)" ), m_pShinSpecific22_2_31Ex->m_hWnd );
			sh_Tabmng.pWnd = m_pShinSpecific22_2_31Ex;

			sh_Tabmng.tabtype = ID_ICSSH_SPC31_22_2_TAB;
			m_TabMng.Add( sh_Tabmng );

			m_ListType[tabIdx] = ID_ICSSH_SPC31_22_2_TAB;
			tabIdx++;
		}
	}

	// 特定収入計算表３
	memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
	if ( !m_pShinSpecific3431 ) {
		m_pShinSpecific3431 = new CShinSpecific34_31 ( this );
		m_pShinSpecific3431->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
		m_pShinSpecific3431->Create ( IDD_DIALOG_K34_31, &m_MainTab );
	}

	ptmpTab[tabIdx] = m_pShinSpecific3431;
	m_MainTab.InsertItem ( tabIdx, _T( "計算表３・４" ), m_pShinSpecific3431->m_hWnd );
	sh_Tabmng.pWnd = m_pShinSpecific3431;

	sh_Tabmng.tabtype = ID_ICSSH_SPC31_34_TAB;
	m_TabMng.Add ( sh_Tabmng );

	m_ListType[tabIdx] = ID_ICSSH_SPC31_34_TAB;
	tabIdx++;

	//if ( m_pSpcListData->IsRatioOver95 ( m_pSnHeadData ) ) {
	// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
	if ( ( m_pSpcListData->IsRatioOver95 ( m_pSnHeadData ) ) && ( m_pSnHeadData->IsUseSemiUriRatio() == FALSE ) ) {
		// 課税売上割合95%以上
		// 特定収入計算表５（１）
		if( syzSign ) {
			// 旧税率あり
			memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
			if ( !m_pShinSpecific5131 ) {
				m_pShinSpecific5131 = new CShinSpecific51_31 ( this );
				m_pShinSpecific5131->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
				m_pShinSpecific5131->Create ( IDD_DIALOG_K51_31, &m_MainTab );
			}

			ptmpTab[tabIdx] = m_pShinSpecific5131;
			m_MainTab.InsertItem ( tabIdx, _T( "計算表５(1)" ), m_pShinSpecific5131->m_hWnd );
			sh_Tabmng.pWnd = m_pShinSpecific5131;

			sh_Tabmng.tabtype = ID_ICSSH_SPC31_51_TAB;
			m_TabMng.Add ( sh_Tabmng );

			m_ListType[tabIdx] = ID_ICSSH_SPC31_51_TAB;
			tabIdx++;
		}
		else {
			//----> 20231124
			if( eymd >= KIKAN_START_INV80){
				// インボイス改正版
				memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
				if( !m_pShinSpecific5131Ex2 ) {
					m_pShinSpecific5131Ex2 = new CShinSpecific51_31Ex2( this );
					m_pShinSpecific5131Ex2->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
					m_pShinSpecific5131Ex2->Create( IDD_DIALOG_K51_31_ex2, &m_MainTab );
				}

				ptmpTab[tabIdx] = m_pShinSpecific5131Ex2;
				m_MainTab.InsertItem( tabIdx, _T( "計算表５(1)" ), m_pShinSpecific5131Ex2->m_hWnd );
				sh_Tabmng.pWnd = m_pShinSpecific5131Ex2;

				sh_Tabmng.tabtype = ID_ICSSH_SPC31_51_TAB;
				m_TabMng.Add( sh_Tabmng );

				m_ListType[tabIdx] = ID_ICSSH_SPC31_51_TAB;
				tabIdx++;			
			}
			else{
			//<---- 20231124
				// 旧税率なし
				memset( &sh_Tabmng, '\0', sizeof( SH_TABMNG ) );
				if( !m_pShinSpecific5131Ex ) {
					m_pShinSpecific5131Ex = new CShinSpecific51_31Ex( this );
					m_pShinSpecific5131Ex->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
					m_pShinSpecific5131Ex->Create( IDD_DIALOG_K51_31_ex, &m_MainTab );
				}

				ptmpTab[tabIdx] = m_pShinSpecific5131Ex;
				m_MainTab.InsertItem( tabIdx, _T( "計算表５(1)" ), m_pShinSpecific5131Ex->m_hWnd );
				sh_Tabmng.pWnd = m_pShinSpecific5131Ex;

				sh_Tabmng.tabtype = ID_ICSSH_SPC31_51_TAB;
				m_TabMng.Add( sh_Tabmng );

				m_ListType[tabIdx] = ID_ICSSH_SPC31_51_TAB;
				tabIdx++;
			}
		}
	}
	else {
		// 課税売上割合95%未満
		if ( m_pSnHeadData->IsKobetuSiireAnbun() ) {
			// 個別対応
			// 特定収入計算表５（２）
			if( syzSign ) {
				// 旧税率あり
				memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
				if ( !m_pShinSpecific5231 ) {
					m_pShinSpecific5231 = new CShinSpecific52_31 ( this );
					m_pShinSpecific5231->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
					m_pShinSpecific5231->Create ( IDD_DIALOG_K52_31, &m_MainTab );
				}

				ptmpTab[tabIdx] = m_pShinSpecific5231;
				m_MainTab.InsertItem ( tabIdx, _T( "計算表５(2)" ), m_pShinSpecific5231->m_hWnd );
				sh_Tabmng.pWnd = m_pShinSpecific5231;

				sh_Tabmng.tabtype = ID_ICSSH_SPC31_52_TAB;
				m_TabMng.Add ( sh_Tabmng );

				m_ListType[tabIdx] = ID_ICSSH_SPC31_52_TAB;
				tabIdx++;
			}
			else {
				//----> 20231128
				if (eymd >= KIKAN_START_INV80) {
					// インボイス改正版
					memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
					if (!m_pShinSpecific5231Ex2) {
						m_pShinSpecific5231Ex2 = new CShinSpecific52_31Ex2(this);
						m_pShinSpecific5231Ex2->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
						m_pShinSpecific5231Ex2->Create(IDD_DIALOG_K52_31_ex2, &m_MainTab);
					}

					ptmpTab[tabIdx] = m_pShinSpecific5231Ex2;
					m_MainTab.InsertItem(tabIdx, _T("計算表５(2)"), m_pShinSpecific5231Ex2->m_hWnd);
					sh_Tabmng.pWnd = m_pShinSpecific5231Ex2;

					sh_Tabmng.tabtype = ID_ICSSH_SPC31_52_TAB;
					m_TabMng.Add(sh_Tabmng);

					m_ListType[tabIdx] = ID_ICSSH_SPC31_52_TAB;
					tabIdx++;
				}
				else {
				//<---- 20231128
					// 旧税率なし
					memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
					if (!m_pShinSpecific5231Ex) {
						m_pShinSpecific5231Ex = new CShinSpecific52_31Ex(this);
						m_pShinSpecific5231Ex->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
						m_pShinSpecific5231Ex->Create(IDD_DIALOG_K52_31_ex, &m_MainTab);
					}

					ptmpTab[tabIdx] = m_pShinSpecific5231Ex;
					m_MainTab.InsertItem(tabIdx, _T("計算表５(2)"), m_pShinSpecific5231Ex->m_hWnd);
					sh_Tabmng.pWnd = m_pShinSpecific5231Ex;

					sh_Tabmng.tabtype = ID_ICSSH_SPC31_52_TAB;
					m_TabMng.Add(sh_Tabmng);

					m_ListType[tabIdx] = ID_ICSSH_SPC31_52_TAB;
					tabIdx++;
				}
			}
		}
		else {
			// 比例配分
			// 特定収入計算表５（３）
			if( syzSign ) {
				// 旧税率あり
				memset ( &sh_Tabmng, '\0', sizeof ( SH_TABMNG ) );
				if ( !m_pShinSpecific5331 ) {
					m_pShinSpecific5331 = new CShinSpecific53_31 ( this );
					m_pShinSpecific5331->InitInfo ( &m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
					m_pShinSpecific5331->Create ( IDD_DIALOG_K53_31, &m_MainTab );
				}

				ptmpTab[tabIdx] = m_pShinSpecific5331;
				m_MainTab.InsertItem ( tabIdx, _T( "計算表５(3)" ), m_pShinSpecific5331->m_hWnd );
				sh_Tabmng.pWnd = m_pShinSpecific5331;

				sh_Tabmng.tabtype = ID_ICSSH_SPC31_53_TAB;
				m_TabMng.Add ( sh_Tabmng );

				m_ListType[tabIdx] = ID_ICSSH_SPC31_53_TAB;
				tabIdx++;
			}
			else {
				//----> 20231128
				if (eymd >= KIKAN_START_INV80) {
					// インボイス改正版
					memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
					if (!m_pShinSpecific5331Ex2) {
						m_pShinSpecific5331Ex2 = new CShinSpecific53_31Ex2(this);
						m_pShinSpecific5331Ex2->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
						m_pShinSpecific5331Ex2->Create(IDD_DIALOG_K53_31_ex2, &m_MainTab);
					}

					ptmpTab[tabIdx] = m_pShinSpecific5331Ex2;
					m_MainTab.InsertItem(tabIdx, _T("計算表５(3)"), m_pShinSpecific5331Ex2->m_hWnd);
					sh_Tabmng.pWnd = m_pShinSpecific5331Ex2;

					sh_Tabmng.tabtype = ID_ICSSH_SPC31_53_TAB;
					m_TabMng.Add(sh_Tabmng);

					m_ListType[tabIdx] = ID_ICSSH_SPC31_53_TAB;
					tabIdx++;
				}
				else {
					//<---- 20231128
					// 旧税率なし
					memset(&sh_Tabmng, '\0', sizeof(SH_TABMNG));
					if (!m_pShinSpecific5331Ex) {
						m_pShinSpecific5331Ex = new CShinSpecific53_31Ex(this);
						m_pShinSpecific5331Ex->InitInfo(&m_pSpcListData, &m_pSnHeadData, m_pZmsub, &m_Arith);
						m_pShinSpecific5331Ex->Create(IDD_DIALOG_K53_31_ex, &m_MainTab);
					}

					ptmpTab[tabIdx] = m_pShinSpecific5331Ex;
					m_MainTab.InsertItem(tabIdx, _T("計算表５(3)"), m_pShinSpecific5331Ex->m_hWnd);
					sh_Tabmng.pWnd = m_pShinSpecific5331Ex;

					sh_Tabmng.tabtype = ID_ICSSH_SPC31_53_TAB;
					m_TabMng.Add(sh_Tabmng);

					m_ListType[tabIdx] = ID_ICSSH_SPC31_53_TAB;
					tabIdx++;
				}
			}
		}
	}

	for ( int tab_cnt = 0; tab_cnt < tabIdx; tab_cnt++ ) {
		( ( CSyzBaseDlg* )ptmpTab[tab_cnt])->Static_Update ( m_pSnHeadData->Sn_Sign4 );
	}

	// 先頭ページ保存
	m_pFirstPage = ptmpTab[0];	

	int i = 0;
	RECT rect = { 0 };
	ptmpTab[0]->GetClientRect ( &rect );
	for ( i = 0; i != tabIdx; i++ ) {
		ptmpTab[i]->MoveWindow ( &rect );
	}
	RECT rect1, rect2;
	CWnd *pWnd = ( CWnd* )&m_MainTab;
	pWnd->GetClientRect ( &rect1 );			// ｸﾗｲｱﾝﾄ領域の取得
	m_MainTab.AdjustRect ( FALSE, &rect1 );	// ﾀﾌﾞｺﾝﾄﾛｰﾙの表示領域の計算
	rect2.left   = rect1.left;
	rect2.top    = rect1.top;
	rect2.bottom = rect1.bottom;
	rect2.right  = rect1.right;
	for ( i = 0; i != tabIdx; i++ ) {
		if ( ptmpTab[i] ) {
			// ダイアログの移動
			ptmpTab[i]->MoveWindow ( &rect2 );
		}
	}

	m_MainTab.ModifyStyle ( TCS_SINGLELINE, TCS_MULTILINE );

/*	SetItemPositionEx ( 0 );
	ReInitSpcDisp ( 0 );*/
	
	return 0;
}

//-----------------------------------------------------------------------------
// タブ内のダイアログを全消去(特定収入)
//-----------------------------------------------------------------------------
void CTaxationList31View::DeleteInsideSpcDlg()
{
	// ビュー内部からフォーカスを外す！
	m_MainTab.SetFocus();

	// 特定収入計算表１
	if ( m_pShinSpecific131 ) {
		m_pShinSpecific131->DestroyWindow();
		delete m_pShinSpecific131;
		m_pShinSpecific131 = NULL;	
	}
	if( m_pShinSpecific131Ex ) {
		m_pShinSpecific131Ex->DestroyWindow();
		delete m_pShinSpecific131Ex;
		m_pShinSpecific131Ex = NULL;	
	}

	// 特定収入計算表２（１）
	if ( m_pShinSpecific231 ) {
		m_pShinSpecific231->DestroyWindow();
		delete m_pShinSpecific231;
		m_pShinSpecific231 = NULL;	
	}
	if( m_pShinSpecific231Ex ) {
		m_pShinSpecific231Ex->DestroyWindow();
		delete m_pShinSpecific231Ex;
		m_pShinSpecific231Ex = NULL;	
	}

	// 特定収入計算表２（２）表面
	if ( m_pShinSpecific2231 ) {
		m_pShinSpecific2231->DestroyWindow();
		delete m_pShinSpecific2231;
		m_pShinSpecific2231 = NULL;	
	}

	// 特定収入計算表２（２）裏面
	if ( m_pShinSpecific22_2_31 ) {
		m_pShinSpecific22_2_31->DestroyWindow();
		delete m_pShinSpecific22_2_31;
		m_pShinSpecific22_2_31 = NULL;	
	}
	if ( m_pShinSpecific22_2_31Ex ) {
		m_pShinSpecific22_2_31Ex->DestroyWindow();
		delete m_pShinSpecific22_2_31Ex;
		m_pShinSpecific22_2_31Ex = NULL;	
	}

	// 特定収入計算表３
	if ( m_pShinSpecific3431 ) {
		m_pShinSpecific3431->DestroyWindow();
		delete m_pShinSpecific3431;
		m_pShinSpecific3431 = NULL;	
	}

	// 特定収入計算表５（１）
	if ( m_pShinSpecific5131 ) {
		m_pShinSpecific5131->DestroyWindow();
		delete m_pShinSpecific5131;
		m_pShinSpecific5131 = NULL;	
	}
	if ( m_pShinSpecific5131Ex ) {
		m_pShinSpecific5131Ex->DestroyWindow();
		delete m_pShinSpecific5131Ex;
		m_pShinSpecific5131Ex = NULL;	
	}
	//----> 20231124
	if ( m_pShinSpecific5131Ex2 ) {
		m_pShinSpecific5131Ex2->DestroyWindow();
		delete m_pShinSpecific5131Ex2;
		m_pShinSpecific5131Ex2 = NULL;	
	}
	//<---- 20231124

	// 特定収入計算表５（２）
	if ( m_pShinSpecific5231 ) {
		m_pShinSpecific5231->DestroyWindow();
		delete m_pShinSpecific5231;
		m_pShinSpecific5231 = NULL;	
	}
	if ( m_pShinSpecific5231Ex ) {
		m_pShinSpecific5231Ex->DestroyWindow();
		delete m_pShinSpecific5231Ex;
		m_pShinSpecific5231Ex = NULL;	
	}

	if (m_pShinSpecific5231Ex2) {
		m_pShinSpecific5231Ex2->DestroyWindow();
		delete m_pShinSpecific5231Ex2;
		m_pShinSpecific5231Ex2 = NULL;
	}
	// 特定収入計算表５（３）
	if ( m_pShinSpecific5331 ) {
		m_pShinSpecific5331->DestroyWindow();
		delete m_pShinSpecific5331;
		m_pShinSpecific5331 = NULL;	
	}
	if( m_pShinSpecific5331Ex ) {
		m_pShinSpecific5331Ex->DestroyWindow();
		delete m_pShinSpecific5331Ex;
		m_pShinSpecific5331Ex = NULL;	
	}
	if (m_pShinSpecific5331Ex2) {
		m_pShinSpecific5331Ex2->DestroyWindow();
		delete m_pShinSpecific5331Ex2;
		m_pShinSpecific5331Ex2 = NULL;
	}

	//----> 20231128
	if (m_pShinSpecific52131) {
		m_pShinSpecific52131->DestroyWindow();
		delete m_pShinSpecific52131;
		m_pShinSpecific52131 = NULL;
	}

	if (m_pShinSpecific52231) {
		m_pShinSpecific52231->DestroyWindow();
		delete m_pShinSpecific52231;
		m_pShinSpecific52231 = NULL;
	}
	if (m_pShinSpecific523131) {
		m_pShinSpecific523131->DestroyWindow();
		delete m_pShinSpecific523131;
		m_pShinSpecific523131 = NULL;
	}
	if (m_pShinSpecific523231) {
		m_pShinSpecific523231->DestroyWindow();
		delete m_pShinSpecific523231;
		m_pShinSpecific523231 = NULL;
	}
	if (m_pShinSpecific523331) {
		m_pShinSpecific523331->DestroyWindow();
		delete m_pShinSpecific523331;
		m_pShinSpecific523331 = NULL;
	}
	if (m_pShinSpecific524131) {
		m_pShinSpecific524131->DestroyWindow();
		delete m_pShinSpecific524131;
		m_pShinSpecific524131 = NULL;
	}
	if (m_pShinSpecific524231) {
		m_pShinSpecific524231->DestroyWindow();
		delete m_pShinSpecific524231;
		m_pShinSpecific524231 = NULL;
	}
	if (m_pShinSpecific524331) {
		m_pShinSpecific524331->DestroyWindow();
		delete m_pShinSpecific524331;
		m_pShinSpecific524331 = NULL;
	}
	//<---- 20231128
	

	if ( m_pSpcListData ) {
		delete m_pSpcListData;
		m_pSpcListData = NULL;
	}

	m_TabMng.RemoveAll();
	m_pFirstPage = NULL;
	memset ( m_ListType, '\0', sizeof ( m_ListType ) );
}

//==================================================
// フォーカスセット
//--------------------------------------------------
// 引数	
//		selno		：		選択帳票
//		IsInEnd		：		終了処理中？
//==================================================
void CTaxationList31View::SetItemPositionEx ( int selno )
{
	// 閾値チェック
	int cnt = ( int )m_TabMng.GetCount();
	if ( ( selno < 0) || ( cnt <= selno ) ) {
		return;
	}

	if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_1_TAB ){
		if ( m_pShinSpecific131 ) {
			( ( CShinSpecific1_31* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if (m_pShinSpecific131Ex) {
			((CShinSpecific1_31Ex*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_2_TAB ){
		if ( m_pShinSpecific231 ) {
			( ( CShinSpecific2_31* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if (m_pShinSpecific231Ex) {
			((CShinSpecific2_31Ex*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_22_TAB ){
		if ( m_pShinSpecific2231 ) {
			( ( CShinSpecific22_31* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_22_2_TAB ){
		if ( m_pShinSpecific22_2_31 ) {
			( ( CShinSpecific22_2_31* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if (m_pShinSpecific22_2_31Ex) {
			((CShinSpecific22_2_31Ex*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_34_TAB ){
		if ( m_pShinSpecific3431 ) {
			( ( CShinSpecific34_31* )m_TabMng[selno].pWnd )->SetItemPosition ( 0 );
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_51_TAB ){
		if ( m_pShinSpecific5131 ) {
			( ( CShinSpecific51_31* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if ( m_pShinSpecific5131Ex ) {
			( ( CShinSpecific51_31Ex* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		//----> 20231124
		if ( m_pShinSpecific5131Ex2 ) {
			( ( CShinSpecific51_31Ex2* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		//<---- 20231124
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_TAB ){
		if ( m_pShinSpecific5231 ) {
			( ( CShinSpecific52_31* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if ( m_pShinSpecific5231Ex ) {
			( ( CShinSpecific52_31Ex* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if (m_pShinSpecific5231Ex2) {
			((CShinSpecific52_31Ex2*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_53_TAB ){
		if ( m_pShinSpecific5331 ) {
			( ( CShinSpecific53_31* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if ( m_pShinSpecific5331Ex ) {
			( ( CShinSpecific53_31Ex* )m_TabMng[selno].pWnd )->SetItemPosition();
		}
		if (m_pShinSpecific5331Ex2) {
			((CShinSpecific53_31Ex2*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	//----> 20231128
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_1_31TAB) {
		if (m_pShinSpecific52131) {
			((CShinSpecific52_1_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_2_31TAB) {
		if (m_pShinSpecific52231) {
			((CShinSpecific52_2_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_31_31TAB) {
		if (m_pShinSpecific523131) {
			((CShinSpecific52_31_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_32_31TAB) {
		if (m_pShinSpecific523231) {
			((CShinSpecific52_32_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_33_31TAB) {
		if (m_pShinSpecific523331) {
			((CShinSpecific52_33_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_41_31TAB) {
		if (m_pShinSpecific524131) {
			((CShinSpecific52_41_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_42_31TAB) {
		if (m_pShinSpecific524231) {
			((CShinSpecific52_42_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_43_31TAB) {
		if (m_pShinSpecific524331) {
			((CShinSpecific52_43_31*)m_TabMng[selno].pWnd)->SetItemPosition();
		}
	}

	//<---- 20231128
}

//==================================================
// 全画面：画面表示 特定収入計算表 再初期化
//--------------------------------------------------
// 引数
//		selno		：		選択番号
//--------------------------------------------------
// 返送値
//			0		：		正常終了
//==================================================
int CTaxationList31View::ReInitSpcDisp( int selno )
{
	// 閾値チェック
	int cnt = ( int )m_TabMng.GetCount();
	if ( ( selno < 0 ) || ( cnt <= selno ) ){
		return 0;
	}
	if ( m_TabMng[selno].pWnd == NULL ) {
		return 0;
	}


	if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_1_TAB ) {
		if ( m_pShinSpecific131 ) {
			( ( CShinSpecific1_31* )m_TabMng[selno].pWnd )->SpecificInit1();
		}
		if ( m_pShinSpecific131Ex ) {
			( ( CShinSpecific1_31Ex* )m_TabMng[selno].pWnd )->SpecificInit1Ex();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_2_TAB ) {
		if ( m_pShinSpecific231 ) {
			( ( CShinSpecific2_31* )m_TabMng[selno].pWnd )->SpecificInit21();
		}
		if ( m_pShinSpecific231Ex ) {
			( ( CShinSpecific2_31Ex* )m_TabMng[selno].pWnd )->SpecificInit21Ex();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_22_TAB ) {
		if ( m_pShinSpecific2231 ) {
			( ( CShinSpecific22_31* )m_TabMng[selno].pWnd )->SpecificInit22();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_22_2_TAB ) {
		if ( m_pShinSpecific22_2_31 ) {
			( ( CShinSpecific22_2_31* )m_TabMng[selno].pWnd )->SpecificInit22_2();
		}
		if ( m_pShinSpecific22_2_31Ex ) {
			( ( CShinSpecific22_2_31Ex* )m_TabMng[selno].pWnd )->SpecificInit22_2Ex();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_34_TAB ) {
		if ( m_pShinSpecific3431 ) {
			( ( CShinSpecific34_31* )m_TabMng[selno].pWnd )->SpecificInit34();
		}
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_51_TAB ) {
		if ( m_pShinSpecific5131 ) {
			( ( CShinSpecific51_31* )m_TabMng[selno].pWnd )->SpecificInit51();
		}
		if ( m_pShinSpecific5131Ex ) {
			( ( CShinSpecific51_31Ex* )m_TabMng[selno].pWnd )->SpecificInit51Ex();
		}
		//----> 20231124
		if ( m_pShinSpecific5131Ex2 ) {
			( ( CShinSpecific51_31Ex2* )m_TabMng[selno].pWnd )->SpecificInit51Ex2();
		}
		//<---- 20231124
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_TAB ) {
		if ( m_pShinSpecific5231 ) {
			( ( CShinSpecific52_31* )m_TabMng[selno].pWnd )->SpecificInit52();
		}
		if ( m_pShinSpecific5231Ex ) {
			( ( CShinSpecific52_31Ex* )m_TabMng[selno].pWnd )->SpecificInit52Ex();
		}
		//----> 20231128
		if (m_pShinSpecific5231Ex2) {
			((CShinSpecific52_31Ex2*)m_TabMng[selno].pWnd)->SpecificInit52Ex2();
		}
		//<---- 20231128
	}
	else if ( m_TabMng[selno].tabtype == ID_ICSSH_SPC31_53_TAB ) {
		if ( m_pShinSpecific5331 ) {
			( ( CShinSpecific53_31* )m_TabMng[selno].pWnd )->SpecificInit53();
		}
		if ( m_pShinSpecific5331Ex ) {
			( ( CShinSpecific53_31Ex* )m_TabMng[selno].pWnd )->SpecificInit53Ex();
		}
		//----> 20231128
		if (m_pShinSpecific5331Ex2) {
			((CShinSpecific53_31Ex2*)m_TabMng[selno].pWnd)->SpecificInit53Ex2();
		}
		//<---- 20231128
	}
	//----> 20231128
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_1_31TAB) {
		if (m_pShinSpecific52131) {
			((CShinSpecific52_1_31*)m_TabMng[selno].pWnd)->SpecificInit52_1_31();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_2_31TAB) {
		if (m_pShinSpecific52231) {
			((CShinSpecific52_2_31*)m_TabMng[selno].pWnd)->SpecificInit52_2_31();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_31_31TAB) {
		if (m_pShinSpecific523131) {
			((CShinSpecific52_31_31*)m_TabMng[selno].pWnd)->SpecificInit52_31_31();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_32_31TAB) {
		if (m_pShinSpecific523231) {
			((CShinSpecific52_32_31*)m_TabMng[selno].pWnd)->SpecificInit52_32_31();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_33_31TAB) {
		if (m_pShinSpecific523331) {
			((CShinSpecific52_33_31*)m_TabMng[selno].pWnd)->SpecificInit52_33_31();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_41_31TAB) {
		if (m_pShinSpecific524131) {
			((CShinSpecific52_41_31*)m_TabMng[selno].pWnd)->SpecificInit52_41_31();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_42_31TAB) {
		if (m_pShinSpecific524231) {
			((CShinSpecific52_42_31*)m_TabMng[selno].pWnd)->SpecificInit52_42_31();
		}
	}
	else if (m_TabMng[selno].tabtype == ID_ICSSH_SPC31_52_43_31TAB) {
		if (m_pShinSpecific524331) {
			((CShinSpecific52_43_31*)m_TabMng[selno].pWnd)->SpecificInit52_43_31();
		}
	}
	//<---- 20231128
	return 0;
}

//==================================================
// 全画面：画面表示 特定収入計算表 再初期化
//--------------------------------------------------
// 返送値
//			0		：		正常終了
//==================================================
int CTaxationList31View::ReInitSpcDisp()
{
	if ( m_pShinSpecific131 ) {
		m_pShinSpecific131->SpecificInit1();
	}
	if ( m_pShinSpecific131Ex ) {
		m_pShinSpecific131Ex->SpecificInit1Ex();
	}

	if ( m_pShinSpecific231 ) {
		m_pShinSpecific231->SpecificInit21();
	}
	if ( m_pShinSpecific231Ex ) {
		m_pShinSpecific231Ex->SpecificInit21Ex();
	}

	if ( m_pShinSpecific2231 ) {
		m_pShinSpecific2231->SpecificInit22();
	}

	if ( m_pShinSpecific22_2_31 ) {
		m_pShinSpecific22_2_31->SpecificInit22_2();
	}
	if ( m_pShinSpecific22_2_31Ex ) {
		m_pShinSpecific22_2_31Ex->SpecificInit22_2Ex();
	}

	if ( m_pShinSpecific3431 ) {
		m_pShinSpecific3431->SpecificInit34();
	}

	if ( m_pShinSpecific5131 ) {
		m_pShinSpecific5131->SpecificInit51();
	}
	if ( m_pShinSpecific5131Ex ) {
		m_pShinSpecific5131Ex->SpecificInit51Ex();
	}
	//----> 20231124
	if ( m_pShinSpecific5131Ex2 ) {
		m_pShinSpecific5131Ex2->SpecificInit51Ex2();
	}
	//<---- 20231124

	if ( m_pShinSpecific5231 ) {
		m_pShinSpecific5231->SpecificInit52();
	}
	if ( m_pShinSpecific5231Ex ) {
		m_pShinSpecific5231Ex->SpecificInit52Ex();
	}
	//----> 20231128
	if (m_pShinSpecific5231Ex2) {
		m_pShinSpecific5231Ex2->SpecificInit52Ex2();
	}
	//<---- 20231128

	if ( m_pShinSpecific5331 ) {
		m_pShinSpecific5331->SpecificInit53();
	}
	if ( m_pShinSpecific5331Ex ) {
		m_pShinSpecific5331Ex->SpecificInit53Ex();
	}
	//----> 20231128
	if (m_pShinSpecific5331Ex2) {
		m_pShinSpecific5331Ex2->SpecificInit53Ex2();
	}
	//<---- 20231128
	//----> 20231128
	if (m_pShinSpecific52131) {
		m_pShinSpecific52131->SpecificInit52_1_31();
	}
	if (m_pShinSpecific52231) {
		m_pShinSpecific52231->SpecificInit52_2_31();
	}

	if (m_pShinSpecific523131) {
		m_pShinSpecific523131->SpecificInit52_31_31();
	}

	if (m_pShinSpecific523231) {
		m_pShinSpecific523231->SpecificInit52_32_31();
	}
	
	if (m_pShinSpecific523331) {
		m_pShinSpecific523331->SpecificInit52_33_31();
	}
	if (m_pShinSpecific524131) {
		m_pShinSpecific524131->SpecificInit52_41_31();
	}

	if (m_pShinSpecific524231) {
		m_pShinSpecific524231->SpecificInit52_42_31();
	}

	if (m_pShinSpecific524331) {
		m_pShinSpecific524331->SpecificInit52_43_31();
	}


	//<---- 20231128
	return 0;
}


//=============================
// 本表データへの書込み
//=============================
int CTaxationList31View::SetHonpyoMony()
{
	// 書込みデータ
	SPCPOST_DATA	postData = { 0 };

	memmove ( postData.Sn_202_23B, m_pSpcListData->stval.SpSiz4, sizeof ( postData.Sn_202_23B ) );
	memmove ( postData.Sn_202_23C, m_pSpcListData->stval.SpSiz63, sizeof ( postData.Sn_202_23C ) );
	memmove ( postData.Sn_201_23D, m_pSpcListData->stval.SpSiz624, sizeof ( postData.Sn_201_23D ) );
	memmove ( postData.Sn_201_23E, m_pSpcListData->stval.SpSiz78, sizeof ( postData.Sn_201_23E ) );

	postData.Sn_Sign2 = m_pSnHeadData->Sn_Sign2;

	m_isLinkingSpcToHonpyo = TRUE;

	m_pShinInfo->pParent->SendMessage ( WM_USER_SPCPOST_RECALQ31, ( WPARAM )&postData );

	m_isLinkingSpcToHonpyo = FALSE;

	return 0;
}

//-----------------------------------------------------------------------------
// 本表データとの連動
//-----------------------------------------------------------------------------
// 引数	IsChgDoSpc	：	特定収入を加味するかどうかに変更有？
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CTaxationList31View::LinkHonpyoData ( BOOL IsChgDoSpc )
{
	char WORK0[MONY_BUF_SIZE] = { 0 };

	m_pSnHeadData->Sn_Sign2 &= 0xfd;
	m_Arith.l_input ( WORK0, _T( "50" ) );
	if ( m_Arith.l_cmp ( m_pSpcListData->stval.SpTswr, WORK0 ) <= 0 ) {
		m_pSnHeadData->Sn_Sign2 |= 0x02;
		if ( IsChgDoSpc ) {
			SetHonpyoMony();
		}
	}
	else {
		SetHonpyoMony();
	}

	return 0;
}

//======================================
// 課税売上割合95%以上？
//======================================
BOOL CTaxationList31View::IsRatioOver95()
{
	BOOL ret = FALSE;

	ret = m_pSpcListData->IsRatioOver95 ( m_pSnHeadData );

	return ret;
}

//-----------------------------------------------------------------------------
// データ書込み(特定収入)
//-----------------------------------------------------------------------------
// 引数	isEnd	：	最終データ？
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationList31View::WriteSpcData( BOOL isEnd )
{ 
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

//-----------------------------------------------------------------------------
// 特定収入の再計算
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationList31View::RecalqSpc ( SYZTOSPC_DATA *pSyzToSpc )
{
	if ( m_pSpcListData == NULL ) {
		return -1;
	}

	// 控除対象仕入税額＋課税売上の取込
	if ( m_pSpcListData->SetSkjUriMony ( pSyzToSpc, m_pSnHeadData ) ) {
		return -1;
	}

	BOOL	isChgRatioOver95 = FALSE;

	//今回表示するタブを判定し、現在表示されているタブと比較し、違っていれば再作成フラグを立てる
	// 課税売上割合95%未満かつ、課税売上割合に準ずる割合を使用
	//if ( m_pSpcListData->IsRatioOver95 ( m_pSnHeadData ) == FALSE ) {
	if ( ( m_pSpcListData->IsRatioOver95 ( m_pSnHeadData ) == FALSE ) || ( m_pSnHeadData->IsUseSemiUriRatio() == TRUE ) ) {
		//課税売上９５％未満の場合
		if ( m_pSnHeadData->IsKobetuSiireAnbun() ) {
			// 計算表５（２）
			//if ( m_pShinSpecific5231 == NULL ) {
			if( ( m_pShinSpecific5231 == NULL ) && ( m_pShinSpecific5231Ex == NULL ) && (m_pShinSpecific5231Ex2 == NULL)) {// 20231128
				isChgRatioOver95 = TRUE;
			}
		}
		else {
			// 計算表５（３）
			//if ( m_pShinSpecific5331 == NULL ) {
			if( ( m_pShinSpecific5331 == NULL ) && ( m_pShinSpecific5331Ex == NULL ) && (m_pShinSpecific5331Ex2 == NULL)) {// 20231128
				isChgRatioOver95 = TRUE;
			}
		}
	}
	else {
		//課税売上９５％以上の場合
		 // 計算表５（１）
		//if ( m_pShinSpecific5131 == NULL ) {
		//if( ( m_pShinSpecific5131 == NULL ) && ( m_pShinSpecific5131Ex == NULL ) ) {
		if( ( m_pShinSpecific5131 == NULL ) && ( m_pShinSpecific5131Ex == NULL ) && ( m_pShinSpecific5131Ex2 == NULL ) ) { // 20231124
			isChgRatioOver95 = TRUE;
		}
	}

	if ( m_isLinkingSpcToHonpyo == TRUE ) {
		// 特定収入発信で金額を更新する時には、特定収入計算表の５シリーズの再作成はあり得ないから！
		isChgRatioOver95 = FALSE;
	}

	if ( isChgRatioOver95 ) {
		BeginWaitCursor();
		WriteSpcData ( FALSE );
		//InitSpecificTab ( TRUE );
		InitTab ( TRUE );
		EndWaitCursor();
	}

	// 控除対象仕入税額以降の計算
	m_pSpcListData->SPassOffCalqMain ( m_pSnHeadData );

	// 特定収入を加味した控除対象仕入税額の取得
	if ( m_pSpcListData->GetSkjMonyAfterSpc ( pSyzToSpc ) ) {
		return -1;
	}

	return 0;
}


//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CTaxationList31View::IsManualInputSign()
{
	BOOL ret = FALSE;
	BOOL syzSign = FALSE;
	long eymd = 0;

	if ( m_Util.IsSpcMaster ( m_pZmsub ) == FALSE ) {
		return ret;
	}

	// 令和3年12月31日以降の課税期間
	if( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		eymd = m_pSnHeadData->Sn_KDAYE;
	}

	if( ( m_pSnHeadData->SVmzsw == 1 ) || ( eymd < R03YEAR ) ) {
		syzSign = TRUE;
	}

	if ( !( m_pSnHeadData->Sn_Sign4 & 0x80 ) || ( m_pSnHeadData->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( m_pSnHeadData->IsKobetuSiireAnbun() ) {
			if( syzSign ) {
				if ( m_pSpcListData->stval.SpUwSgn52[0] & 0x0f ) {
					ret = TRUE;
				}
			}
			else {
				if ( m_pSpcListData->stval.SpUwSgn52[0] & 0x0c ) {
					ret = TRUE;
				}
			}
		}
		else {
			if( syzSign ) {
				if ( m_pSpcListData->stval.SpUwSgn53[0] & 0x0f ) {
					ret = TRUE;
				}
			}
			else {
				if ( m_pSpcListData->stval.SpUwSgn53[0] & 0x0c ) {
					ret = TRUE;
				}
			}
		}
	}

	return ret;
}

// 24/02/29wd_特定収入対応 add -->
//=========================================================
// 手入力項目あり？	特定収入計算表　5-2用
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CTaxationList31View::IsManualInputSign52(void)
{
	BOOL		ret = FALSE;
	int			ii=0, sel=0;
	TSREC52		TSrec52;

	if(m_Util.IsSpcMaster(m_pZmsub) == FALSE) {
		return ret;
	}

// 修正No.168715 add -->
	long		eymd = 0;
	// 令和3年12月31日以降の課税期間
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
// 24/04/19特定収入追加 cor -->
	//if((m_pSnHeadData->SVmzsw == 1) || (eymd < R03YEAR))	{
	//	// 旧税率ありもしくは令和3年12月1日より前はチェックしない
// ----------------------------
	if(eymd < R03YEAR)	{
		// 令和3年12月1日より前はチェックしない
// 24/04/19特定収入追加 cor <--
		return ret;
	}
// 修正No.168715 add <--

// 修正No.168719_2 add -->
	// 予定納税の時もチェックしない
	if(m_pSnHeadData->IsMiddleProvisional())	{	// 予定26号様式
		return ret;
	}
// 修正No.168719_2 add <--

	if( (m_pSnHeadData->Sn_Sign4 & 0x01) )	{
		// 入力ロック中はチェックしない
		return ret;
	}
	else	{
		// 計算表５－２の実額サインチェック
		for(ii=0; ii<m_pSpcListData->TsTbl52.tp_rnum; ii++)	{
			if(m_pSpcListData->m_TblHdl.th_rget(&TSrec52, &m_pSpcListData->TsTbl52, ii))	{
				break;
			}
			sel = GetSpecListType(ii);
			if(sel == 0x01)	{			// 5-2(2)
				if((TSrec52.ts522.msgn[0]&0xff) != 0x00) {
					ret = TRUE;
					break;
				}
			}
			else if(sel == 0x02)	{	// 5-2(3)
				if((TSrec52.ts5231.msgn[0]&0xff) != 0x00) {
					ret = TRUE;
					break;
				}
				if((TSrec52.ts5232.msgn[0]&0xff) != 0x00) {
					ret = TRUE;
					break;
				}
				if((TSrec52.ts5233.msgn[0]&0xff) != 0x00) {
					ret = TRUE;
					break;
				}
			}
			else if(sel == 0x04)	{	// 5-2(4)
				if((TSrec52.ts5241.msgn[0]&0xff) != 0x00) {
					ret = TRUE;
					break;
				}
				if((TSrec52.ts5242.msgn[0]&0xff) != 0x00) {
					ret = TRUE;
					break;
				}
				if((TSrec52.ts5243.msgn[0]&0xff) != 0x00) {
					ret = TRUE;
					break;
				}
			}
		}
	}

	return ret;
}
// 24/02/29wd_特定収入対応 add <--

//// 修正No.168722 add -->
//void CTaxationList31View::IndicateTasListSpc(int selno)
//{
//	CSpcBaseDlg *pBaseList = NULL;
//
//	pBaseList = GetBaseListSpc(m_ListType[selno]);
//	if(pBaseList) {
//		//pBaseList->DispList(0);
//		pBaseList->SetFocusRange();
//	}
//}
//
//CSpcBaseDlg* CTaxationList31View::GetBaseListSpc(int no)
//{
//	CSpcBaseDlg *pBaseList = NULL;
//
//	switch(no) {
//		case ID_ICSSH_SPC31_1_TAB:			// 特定収入計算表１
//			if(m_pShinSpecific131 != 0) {
//				pBaseList = m_pShinSpecific131;
//			}
//			else if(m_pShinSpecific131Ex != 0) {
//				pBaseList = m_pShinSpecific131Ex;
//			}
//			break;
//
//		case ID_ICSSH_SPC31_2_TAB:			// 特定収入計算表２（１）
//			break;
//		case ID_ICSSH_SPC31_22_TAB:			// 特定収入計算表２（２）表面
//			break;
//		case ID_ICSSH_SPC31_22_2_TAB:		// 特定収入計算表２（２）裏面
//			break;
//		case ID_ICSSH_SPC31_34_TAB:			// 特定収入計算表３
//			break;
//		case ID_ICSSH_SPC31_51_TAB:			// 特定収入計算表５（１）
//			break;
//		case ID_ICSSH_SPC31_52_TAB:			// 特定収入計算表５（２）
//			break;
//		case ID_ICSSH_SPC31_53_TAB:			// 特定収入計算表５（３）
//			break;
//
//
//			break;
//		default:
//			break;
//
//	}
//
//	return pBaseList;
//}
//// 修正No.168722 add <--
