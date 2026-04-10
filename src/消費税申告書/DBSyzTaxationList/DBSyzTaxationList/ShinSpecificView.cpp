// ShinSpecificView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "TaxationListView.h"

#include "ShinSpecific1.h"
#include "ShinSpecific21EX.h"
#include "ShinSpecific22.h"
#include "ShinSpecific34.h"
#include "ShinSpecific51.h"
#include "ShinSpecific52.h"
#include "ShinSpecific53.h"
#include "ShinSpecific6.h"

//2017.03.01 INSERT START
#include "ShinSpecific1EX.h"
#include "ShinSpecific1EXkeika.h"
#include "ShinSpecific21EX2.h"
#include "ShinSpecific22keika.h"
#include "ShinSpecific51keika.h"
#include "ShinSpecific52keika.h"
#include "ShinSpecific53keika.h"
//2017.03.01 INSERT END

//-----------------------------------------------------------------------------
// 特定収入タブの初期化
//-----------------------------------------------------------------------------
// 引数	bDelete	：	内部を消去するかどうか
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::InitSpecificTab( BOOL bDelete )
{
	int		i;
	char	PW0[6]={0};

	if( bDelete ){
		// 内部クラスを破棄？
		DeleteInsideSpcDlg();
		// 全削除
		m_MainTab.DeleteAllItems();
	}
	memset( m_ListType, '\0', sizeof(m_ListType) );

	// 帳票データ
	if( !m_pSpcListData ){
		if( (m_pSpcListData = new CH26SpcListData(m_pSnHeadData->SVmzsw ? TRUE:FALSE)) == NULL ){
			return -1;
		}
		if( m_pSpcListData->InitSpData(m_pZmSub) ){
			return -1;
		}
		//------>yoshida150309
		if( m_pSpcListData->GetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		//<-------------------
	}

	int			tabIdx = 0;
	ICSDialog	*ptmpTab[16] = {0};
	SH_TABMNG	shTabmng;

//2017.03.01 UPDATE START
//	// 計算表１
//	if( !m_pShinSpecific1 ){
//		m_pShinSpecific1 = new CShinSpecific1(this);
//		m_pShinSpecific1->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//		m_pShinSpecific1->Create( IDD_DIALOG_K1, &m_MainTab );
//	}
//	ptmpTab[tabIdx] = m_pShinSpecific1;
//	m_ListType[tabIdx] = ID_ICSSH_SPC_1_TAB;
//	m_MainTab.InsertItem( tabIdx, _T("計算表１"), m_pShinSpecific1->m_hWnd );
//	tabIdx++;
//
//	memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//	shTabmng.tabtype = ID_ICSSH_SPC_1_TAB;
//	shTabmng.pWnd = m_pShinSpecific1;
//	m_TabMng.Add( shTabmng );
//
//	// 計算表２－１
//	if( !m_pShinSpecific21 ){
//		m_pShinSpecific21 = new CShinSpecific21EX(this);
//		m_pShinSpecific21->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//		m_pShinSpecific21->Create( IDD_DIALOG_K21_EX, &m_MainTab );
//	}
//	ptmpTab[tabIdx] = m_pShinSpecific21;
//	m_ListType[tabIdx] = ID_ICSSH_SPC_2_1_TAB;
//	m_MainTab.InsertItem( tabIdx, _T("計算表２(1)"), m_pShinSpecific21->m_hWnd );
//	tabIdx++;
//
//	memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//	shTabmng.tabtype = ID_ICSSH_SPC_2_1_TAB;
//	shTabmng.pWnd = m_pShinSpecific21;
//	m_TabMng.Add( shTabmng );
//
//	if( m_pSnHeadData->IsKobetuSiireAnbun() ){
//		// 計算表２－２(5%)
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			if( !m_pShinSpecific22per5 ){
//				m_pShinSpecific22per5 = new CShinSpecific22(ID_ICSSH_SPC_5PER, this);
//				m_pShinSpecific22per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//				m_pShinSpecific22per5->Create( IDD_DIALOG_K22, &m_MainTab );
//			}
//			ptmpTab[tabIdx] = m_pShinSpecific22per5;
//			m_ListType[tabIdx] = ID_ICSSH_SPC_2_2_TAB;
//			m_MainTab.InsertItem( tabIdx, _T("計算表２(2) (5%)"), m_pShinSpecific22per5->m_hWnd );
//			tabIdx++;
//
//			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//			shTabmng.tabtype = ID_ICSSH_SPC_2_2_TAB;
//			shTabmng.pWnd = m_pShinSpecific22per5;
//			m_TabMng.Add( shTabmng );
//		}
//		// 計算表２－２(8%)
//		if( !m_pShinSpecific22per8 ){
//			m_pShinSpecific22per8 = new CShinSpecific22(ID_ICSSH_SPC_8PER, this);
//			m_pShinSpecific22per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//			m_pShinSpecific22per8->Create( IDD_DIALOG_K22, &m_MainTab );
//		}
//		ptmpTab[tabIdx] = m_pShinSpecific22per8;
//		m_ListType[tabIdx] = ID_ICSSH_SPC_2_2_TAB;
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			m_MainTab.InsertItem( tabIdx, _T("計算表２(2) (8%)"), m_pShinSpecific22per8->m_hWnd );
//		}
//		else{
//			m_MainTab.InsertItem( tabIdx, _T("計算表２(2)"), m_pShinSpecific22per8->m_hWnd );
//		}
//		tabIdx++;
//
//		memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//		shTabmng.tabtype = ID_ICSSH_SPC_2_2_TAB;
//		shTabmng.pWnd = m_pShinSpecific22per8;
//		m_TabMng.Add( shTabmng );
//	}
//
//	// 計算表３・４
//	if( !m_pShinSpecific34 ){
//		m_pShinSpecific34 = new CShinSpecific34(this);
//		m_pShinSpecific34->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//		m_pShinSpecific34->Create( IDD_DIALOG_K34, &m_MainTab );
//	}
//	ptmpTab[tabIdx] = m_pShinSpecific34;
//	m_ListType[tabIdx] = ID_ICSSH_SPC_3or4_TAB;
//	m_MainTab.InsertItem( tabIdx, _T("計算表３・４"), m_pShinSpecific34->m_hWnd );
//	tabIdx++;
//
//	memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//	shTabmng.tabtype = ID_ICSSH_SPC_3or4_TAB;
//	shTabmng.pWnd = m_pShinSpecific34;
//	m_TabMng.Add( shTabmng );
//
////	if( 1/*課税売上高95％以上*/ ){
////	if( m_pSnHeadData->IsRatioOver95() ){
//	if( m_pSpcListData->IsRatioOver95(m_pSnHeadData) ){
//
//		// 計算表５－１(5%)
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			if( !m_pShinSpecific51per5 ){
//				m_pShinSpecific51per5 = new CShinSpecific51(ID_ICSSH_SPC_5PER, this);
//				m_pShinSpecific51per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//				m_pShinSpecific51per5->Create( IDD_DIALOG_K51, &m_MainTab );
//			}
//			ptmpTab[tabIdx] = m_pShinSpecific51per5;
//			m_ListType[tabIdx] = ID_ICSSH_SPC_5_1_TAB;
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(1) (5%)"), m_pShinSpecific51per5->m_hWnd );
//			tabIdx++;
//
//			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//			shTabmng.tabtype = ID_ICSSH_SPC_5_1_TAB;
//			shTabmng.pWnd = m_pShinSpecific51per5;
//			m_TabMng.Add( shTabmng );
//		}
//		// 計算表５－１(8%)
//		if( !m_pShinSpecific51per8 ){
//			m_pShinSpecific51per8 = new CShinSpecific51(ID_ICSSH_SPC_8PER, this);
//			m_pShinSpecific51per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//			m_pShinSpecific51per8->Create( IDD_DIALOG_K51, &m_MainTab );
//		}
//		ptmpTab[tabIdx] = m_pShinSpecific51per8;
//		m_ListType[tabIdx] = ID_ICSSH_SPC_5_1_TAB;
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(1) (8%)"), m_pShinSpecific51per8->m_hWnd );
//		}
//		else{
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(1)"), m_pShinSpecific51per8->m_hWnd );
//		}
//		tabIdx++;
//
//		memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//		shTabmng.tabtype = ID_ICSSH_SPC_5_1_TAB;
//		shTabmng.pWnd = m_pShinSpecific51per8;
//		m_TabMng.Add( shTabmng );
//	}
//
////	if( m_pSnHeadData->IsKobetuSiireAnbun() && 1/*課税売上高９５％未満*/ ){
//	if( m_pSnHeadData->IsKobetuSiireAnbun() && (m_pSpcListData->IsRatioOver95(m_pSnHeadData)==FALSE) ){
//		// 計算表５－２(5%)
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			if( !m_pShinSpecific52per5 ){
//				m_pShinSpecific52per5 = new CShinSpecific52(ID_ICSSH_SPC_5PER, this);
//				m_pShinSpecific52per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//				m_pShinSpecific52per5->Create( IDD_DIALOG_K52, &m_MainTab );
//			}
//			ptmpTab[tabIdx] = m_pShinSpecific52per5;
//			m_ListType[tabIdx] = ID_ICSSH_SPC_5_2_TAB;
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(2) (5%)"), m_pShinSpecific52per5->m_hWnd );
//			tabIdx++;
//
//			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//			shTabmng.tabtype = ID_ICSSH_SPC_5_2_TAB;
//			shTabmng.pWnd = m_pShinSpecific52per5;
//			m_TabMng.Add( shTabmng );
//		}
//		// 計算表５－２(8%)
//		if( !m_pShinSpecific52per8 ){
//			m_pShinSpecific52per8 = new CShinSpecific52(ID_ICSSH_SPC_8PER, this);
//			m_pShinSpecific52per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//			m_pShinSpecific52per8->Create( IDD_DIALOG_K52, &m_MainTab );
//		}
//		ptmpTab[tabIdx] = m_pShinSpecific52per8;
//		m_ListType[tabIdx] = ID_ICSSH_SPC_5_2_TAB;
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(2) (8%)"), m_pShinSpecific52per8->m_hWnd );
//		}
//		else{
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(2)"), m_pShinSpecific52per8->m_hWnd );
//		}
//		tabIdx++;
//
//		memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//		shTabmng.tabtype = ID_ICSSH_SPC_5_2_TAB;
//		shTabmng.pWnd = m_pShinSpecific52per8;
//		m_TabMng.Add( shTabmng );
//	}
//
////	if( (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) && 1/*課税売上高９５％未満*/ ){
//	if( (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) && (m_pSpcListData->IsRatioOver95(m_pSnHeadData)==FALSE) ){
//		// 計算表５－３(5%)
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			if( !m_pShinSpecific53per5 ){
//				m_pShinSpecific53per5 = new CShinSpecific53(ID_ICSSH_SPC_5PER, this);
//				m_pShinSpecific53per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//				m_pShinSpecific53per5->Create( IDD_DIALOG_K53, &m_MainTab );
//			}
//			ptmpTab[tabIdx] = m_pShinSpecific53per5;
//			m_ListType[tabIdx] = ID_ICSSH_SPC_5_3_TAB;
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(3) (5%)"), m_pShinSpecific53per5->m_hWnd );
//			tabIdx++;
//
//			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//			shTabmng.tabtype = ID_ICSSH_SPC_5_3_TAB;
//			shTabmng.pWnd = m_pShinSpecific53per5;
//			m_TabMng.Add( shTabmng );
//		}
//		// 計算表５－３(8%)
//		if( !m_pShinSpecific53per8 ){
//			m_pShinSpecific53per8 = new CShinSpecific53(ID_ICSSH_SPC_8PER, this);
//			m_pShinSpecific53per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//			m_pShinSpecific53per8->Create( IDD_DIALOG_K53, &m_MainTab );
//		}
//		ptmpTab[tabIdx] = m_pShinSpecific53per8;
//		m_ListType[tabIdx] = ID_ICSSH_SPC_5_3_TAB;
//		if( m_pSnHeadData->SVmzsw == 1 ){
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(3) (8%)"), m_pShinSpecific53per8->m_hWnd );
//		}
//		else{
//			m_MainTab.InsertItem( tabIdx, _T("計算表５(3)"), m_pShinSpecific53per8->m_hWnd );
//		}
//		tabIdx++;
//
//		memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//		shTabmng.tabtype = ID_ICSSH_SPC_5_3_TAB;
//		shTabmng.pWnd = m_pShinSpecific53per8;
//		m_TabMng.Add( shTabmng );
//	}

	// 計算表１
	memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
	if( IsKeikaType_Keisansyo() == TRUE ){
		if( m_pSnHeadData->SVmzsw == 1 ){
			if( !m_pShinSpecific1EXkeika ){
				m_pShinSpecific1EXkeika = new CShinSpecific1EXkeika(this);
				m_pShinSpecific1EXkeika->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
				m_pShinSpecific1EXkeika->Create( IDD_DIALOG_K1_EX_KEIKA, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSpecific1EXkeika;
			m_MainTab.InsertItem( tabIdx, _T("計算表１"), m_pShinSpecific1EXkeika->m_hWnd );

			shTabmng.pWnd = m_pShinSpecific1EXkeika;
		}else{
			if( !m_pShinSpecific1EX ){
				m_pShinSpecific1EX = new CShinSpecific1EX(this);
				m_pShinSpecific1EX->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
				m_pShinSpecific1EX->Create( IDD_DIALOG_K1_EX, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSpecific1EX;
			m_MainTab.InsertItem( tabIdx, _T("計算表１"), m_pShinSpecific1EX->m_hWnd );

			shTabmng.pWnd = m_pShinSpecific1EX;
		}
	}else{
		if( !m_pShinSpecific1 ){
			m_pShinSpecific1 = new CShinSpecific1(this);
			m_pShinSpecific1->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
			m_pShinSpecific1->Create( IDD_DIALOG_K1, &m_MainTab );
		}
		ptmpTab[tabIdx] = m_pShinSpecific1;
		m_MainTab.InsertItem( tabIdx, _T("計算表１"), m_pShinSpecific1->m_hWnd );

		shTabmng.pWnd = m_pShinSpecific1;
	}
	shTabmng.tabtype = ID_ICSSH_SPC_1_TAB;
	m_TabMng.Add( shTabmng );

	m_ListType[tabIdx] = ID_ICSSH_SPC_1_TAB;
	tabIdx++;

	// 計算表２－１
	memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
	if(( IsKeikaType_Keisansyo() == TRUE )&&( m_pSnHeadData->SVmzsw != 1 )){
		if( !m_pShinSpecific21EX2 ){
			m_pShinSpecific21EX2 = new CShinSpecific21EX2(this);
			m_pShinSpecific21EX2->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
			m_pShinSpecific21EX2->Create( IDD_DIALOG_K21_EX2, &m_MainTab );
		}
		ptmpTab[tabIdx] = m_pShinSpecific21EX2;
		m_MainTab.InsertItem( tabIdx, _T("計算表２(1)"), m_pShinSpecific21EX2->m_hWnd );

		shTabmng.pWnd = m_pShinSpecific21EX2;
	}else{
		if( !m_pShinSpecific21 ){
			m_pShinSpecific21 = new CShinSpecific21EX(this);
			m_pShinSpecific21->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
			m_pShinSpecific21->Create( IDD_DIALOG_K21_EX, &m_MainTab );
		}
		ptmpTab[tabIdx] = m_pShinSpecific21;
		m_MainTab.InsertItem( tabIdx, _T("計算表２(1)"), m_pShinSpecific21->m_hWnd );

		shTabmng.pWnd = m_pShinSpecific21;
	}

	shTabmng.tabtype = ID_ICSSH_SPC_2_1_TAB;
	m_TabMng.Add( shTabmng );

	m_ListType[tabIdx] = ID_ICSSH_SPC_2_1_TAB;
	tabIdx++;

	// 計算表２－２
	if( m_pSnHeadData->IsKobetuSiireAnbun() ){
		if( IsKeikaType_Keisansyo() == TRUE ){
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific22keika ){
					m_pShinSpecific22keika = new CShinSpecific22keika(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific22keika->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific22keika->Create( IDD_DIALOG_K22_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific22keika;
				m_ListType[tabIdx] = ID_ICSSH_SPC_2_2_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表２(2)"), m_pShinSpecific22keika->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_2_2_TAB;
				shTabmng.pWnd = m_pShinSpecific22keika;
				m_TabMng.Add( shTabmng );
			}else{
				if( !m_pShinSpecific22per8 ){
					m_pShinSpecific22per8 = new CShinSpecific22(ID_ICSSH_SPC_8PER, this);
					m_pShinSpecific22per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific22per8->Create( IDD_DIALOG_K22, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific22per8;
				m_ListType[tabIdx] = ID_ICSSH_SPC_2_2_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表２(2)"), m_pShinSpecific22per8->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_2_2_TAB;
				shTabmng.pWnd = m_pShinSpecific22per8;
				m_TabMng.Add( shTabmng );
			}
		}else{
			// 計算表２－２(5%)
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific22per5 ){
					m_pShinSpecific22per5 = new CShinSpecific22(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific22per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific22per5->Create( IDD_DIALOG_K22, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific22per5;
				m_ListType[tabIdx] = ID_ICSSH_SPC_2_2_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表２(2) (5%)"), m_pShinSpecific22per5->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_2_2_TAB;
				shTabmng.pWnd = m_pShinSpecific22per5;
				m_TabMng.Add( shTabmng );
			}
			// 計算表２－２(8%)
			if( !m_pShinSpecific22per8 ){
				m_pShinSpecific22per8 = new CShinSpecific22(ID_ICSSH_SPC_8PER, this);
				m_pShinSpecific22per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
				m_pShinSpecific22per8->Create( IDD_DIALOG_K22, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSpecific22per8;
			m_ListType[tabIdx] = ID_ICSSH_SPC_2_2_TAB;
			if( m_pSnHeadData->SVmzsw == 1 ){
				m_MainTab.InsertItem( tabIdx, _T("計算表２(2) (8%)"), m_pShinSpecific22per8->m_hWnd );
			}
			else{
				m_MainTab.InsertItem( tabIdx, _T("計算表２(2)"), m_pShinSpecific22per8->m_hWnd );
			}
			tabIdx++;

			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_SPC_2_2_TAB;
			shTabmng.pWnd = m_pShinSpecific22per8;
			m_TabMng.Add( shTabmng );
		}
	}

	// 計算表３・４
	if( !m_pShinSpecific34 ){
		m_pShinSpecific34 = new CShinSpecific34(this);
		m_pShinSpecific34->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
		m_pShinSpecific34->Create( IDD_DIALOG_K34, &m_MainTab );
	}
	ptmpTab[tabIdx] = m_pShinSpecific34;
	m_ListType[tabIdx] = ID_ICSSH_SPC_3or4_TAB;
	m_MainTab.InsertItem( tabIdx, _T("計算表３・４"), m_pShinSpecific34->m_hWnd );
	tabIdx++;

	memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
	shTabmng.tabtype = ID_ICSSH_SPC_3or4_TAB;
	shTabmng.pWnd = m_pShinSpecific34;
	m_TabMng.Add( shTabmng );

//	if( 1/*課税売上高95％以上*/ ){
//	if( m_pSnHeadData->IsRatioOver95() ){
	if( m_pSpcListData->IsRatioOver95(m_pSnHeadData) ){
		if( IsKeikaType_Keisansyo() == TRUE ){
			// 計算表５－１
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific51keika ){
					m_pShinSpecific51keika = new CShinSpecific51keika(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific51keika->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific51keika->Create( IDD_DIALOG_K51_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific51keika;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_1_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(1)"), m_pShinSpecific51keika->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_1_TAB;
				shTabmng.pWnd = m_pShinSpecific51keika;
				m_TabMng.Add( shTabmng );
			}else{
			// 計算表５－１(8%)
				if( !m_pShinSpecific51per8 ){
					m_pShinSpecific51per8 = new CShinSpecific51(ID_ICSSH_SPC_8PER, this);
					m_pShinSpecific51per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific51per8->Create( IDD_DIALOG_K51, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific51per8;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_1_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(1)"), m_pShinSpecific51per8->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_1_TAB;
				shTabmng.pWnd = m_pShinSpecific51per8;
				m_TabMng.Add( shTabmng );
			}
		}else{
			// 計算表５－１(5%)
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific51per5 ){
					m_pShinSpecific51per5 = new CShinSpecific51(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific51per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific51per5->Create( IDD_DIALOG_K51, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific51per5;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_1_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(1) (5%)"), m_pShinSpecific51per5->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_1_TAB;
				shTabmng.pWnd = m_pShinSpecific51per5;
				m_TabMng.Add( shTabmng );
			}
			// 計算表５－１(8%)
			if( !m_pShinSpecific51per8 ){
				m_pShinSpecific51per8 = new CShinSpecific51(ID_ICSSH_SPC_8PER, this);
				m_pShinSpecific51per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
				m_pShinSpecific51per8->Create( IDD_DIALOG_K51, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSpecific51per8;
			m_ListType[tabIdx] = ID_ICSSH_SPC_5_1_TAB;
			if( m_pSnHeadData->SVmzsw == 1 ){
				m_MainTab.InsertItem( tabIdx, _T("計算表５(1) (8%)"), m_pShinSpecific51per8->m_hWnd );
			}
			else{
				m_MainTab.InsertItem( tabIdx, _T("計算表５(1)"), m_pShinSpecific51per8->m_hWnd );
			}
			tabIdx++;

			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_SPC_5_1_TAB;
			shTabmng.pWnd = m_pShinSpecific51per8;
			m_TabMng.Add( shTabmng );
		}
	}

//	if( m_pSnHeadData->IsKobetuSiireAnbun() && 1/*課税売上高９５％未満*/ ){
	if( m_pSnHeadData->IsKobetuSiireAnbun() && (m_pSpcListData->IsRatioOver95(m_pSnHeadData)==FALSE) ){
		if( IsKeikaType_Keisansyo() == TRUE ){
			// 計算表５－２
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific52keika ){
					m_pShinSpecific52keika = new CShinSpecific52keika(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific52keika->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific52keika->Create( IDD_DIALOG_K52_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific52keika;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_2_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(2)"), m_pShinSpecific52keika->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_2_TAB;
				shTabmng.pWnd = m_pShinSpecific52keika;
				m_TabMng.Add( shTabmng );
			}else{
				// 計算表５－２(8%)
				if( !m_pShinSpecific52per8 ){
					m_pShinSpecific52per8 = new CShinSpecific52(ID_ICSSH_SPC_8PER, this);
					m_pShinSpecific52per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific52per8->Create( IDD_DIALOG_K52, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific52per8;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_2_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(2)"), m_pShinSpecific52per8->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_2_TAB;
				shTabmng.pWnd = m_pShinSpecific52per8;
				m_TabMng.Add( shTabmng );
			}
		}else{
			// 計算表５－２(5%)
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific52per5 ){
					m_pShinSpecific52per5 = new CShinSpecific52(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific52per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific52per5->Create( IDD_DIALOG_K52, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific52per5;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_2_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(2) (5%)"), m_pShinSpecific52per5->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_2_TAB;
				shTabmng.pWnd = m_pShinSpecific52per5;
				m_TabMng.Add( shTabmng );
			}
			// 計算表５－２(8%)
			if( !m_pShinSpecific52per8 ){
				m_pShinSpecific52per8 = new CShinSpecific52(ID_ICSSH_SPC_8PER, this);
				m_pShinSpecific52per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
				m_pShinSpecific52per8->Create( IDD_DIALOG_K52, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSpecific52per8;
			m_ListType[tabIdx] = ID_ICSSH_SPC_5_2_TAB;
			if( m_pSnHeadData->SVmzsw == 1 ){
				m_MainTab.InsertItem( tabIdx, _T("計算表５(2) (8%)"), m_pShinSpecific52per8->m_hWnd );
			}
			else{
				m_MainTab.InsertItem( tabIdx, _T("計算表５(2)"), m_pShinSpecific52per8->m_hWnd );
			}
			tabIdx++;

			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_SPC_5_2_TAB;
			shTabmng.pWnd = m_pShinSpecific52per8;
			m_TabMng.Add( shTabmng );
		}
	}

//	if( (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) && 1/*課税売上高９５％未満*/ ){
	if( (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) && (m_pSpcListData->IsRatioOver95(m_pSnHeadData)==FALSE) ){
		if( IsKeikaType_Keisansyo() == TRUE ){
			// 計算表５－３
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific53keika ){
					m_pShinSpecific53keika = new CShinSpecific53keika(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific53keika->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific53keika->Create( IDD_DIALOG_K53_KEIKA, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific53keika;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_3_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(3)"), m_pShinSpecific53keika->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_3_TAB;
				shTabmng.pWnd = m_pShinSpecific53keika;
				m_TabMng.Add( shTabmng );
			}else{
			// 計算表５－３(8%)
				if( !m_pShinSpecific53per8 ){
					m_pShinSpecific53per8 = new CShinSpecific53(ID_ICSSH_SPC_8PER, this);
					m_pShinSpecific53per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific53per8->Create( IDD_DIALOG_K53, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific53per8;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_3_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(3)"), m_pShinSpecific53per8->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_3_TAB;
				shTabmng.pWnd = m_pShinSpecific53per8;
				m_TabMng.Add( shTabmng );
			}
		}else{
			// 計算表５－３(5%)
			if( m_pSnHeadData->SVmzsw == 1 ){
				if( !m_pShinSpecific53per5 ){
					m_pShinSpecific53per5 = new CShinSpecific53(ID_ICSSH_SPC_5PER, this);
					m_pShinSpecific53per5->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
					m_pShinSpecific53per5->Create( IDD_DIALOG_K53, &m_MainTab );
				}
				ptmpTab[tabIdx] = m_pShinSpecific53per5;
				m_ListType[tabIdx] = ID_ICSSH_SPC_5_3_TAB;
				m_MainTab.InsertItem( tabIdx, _T("計算表５(3) (5%)"), m_pShinSpecific53per5->m_hWnd );
				tabIdx++;

				memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
				shTabmng.tabtype = ID_ICSSH_SPC_5_3_TAB;
				shTabmng.pWnd = m_pShinSpecific53per5;
				m_TabMng.Add( shTabmng );
			}
			// 計算表５－３(8%)
			if( !m_pShinSpecific53per8 ){
				m_pShinSpecific53per8 = new CShinSpecific53(ID_ICSSH_SPC_8PER, this);
				m_pShinSpecific53per8->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
				m_pShinSpecific53per8->Create( IDD_DIALOG_K53, &m_MainTab );
			}
			ptmpTab[tabIdx] = m_pShinSpecific53per8;
			m_ListType[tabIdx] = ID_ICSSH_SPC_5_3_TAB;
			if( m_pSnHeadData->SVmzsw == 1 ){
				m_MainTab.InsertItem( tabIdx, _T("計算表５(3) (8%)"), m_pShinSpecific53per8->m_hWnd );
			}
			else{
				m_MainTab.InsertItem( tabIdx, _T("計算表５(3)"), m_pShinSpecific53per8->m_hWnd );
			}
			tabIdx++;

			memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
			shTabmng.tabtype = ID_ICSSH_SPC_5_3_TAB;
			shTabmng.pWnd = m_pShinSpecific53per8;
			m_TabMng.Add( shTabmng );
		}
	}
//2017.03.01 UPDATE END

//--> '15.03.07 CUT_START
//	if( m_pSnHeadData->SVmzsw ){
//		// 計算表６
//		if( !m_pShinSpecific6 ){
//			m_pShinSpecific6 = new CShinSpecific6(this);
//			m_pShinSpecific6->InitInfo( &m_pSpcListData, &m_pSnHeadData, m_pZmSub, &m_Arith );
//			m_pShinSpecific6->Create( IDD_DIALOG_K6, &m_MainTab );
//		}
//		ptmpTab[tabIdx] = m_pShinSpecific6;
//		m_ListType[tabIdx] = ID_ICSSH_SPC_6_TAB;
//		m_MainTab.InsertItem( tabIdx, _T("計算表６"), m_pShinSpecific6->m_hWnd );
//		tabIdx++;
//
//		memset( &shTabmng, '\0', sizeof(SH_TABMNG) );
//		shTabmng.tabtype = ID_ICSSH_SPC_6_TAB;
//		shTabmng.pWnd = m_pShinSpecific6;
//		m_TabMng.Add( shTabmng );
//	}
//<-- '15.03.07 CUT_END

//2016.02.23 INSERT START
	for( int tab_cnt = 0; tab_cnt<tabIdx; tab_cnt++){
		((CSyzBaseDlg *)ptmpTab[tab_cnt])->Static_Update( m_pSnHeadData->Sn_Sign4 );
	}
//2016.02.23 INSERT END

	// 先頭ページ保存
	m_pFirstPage = ptmpTab[0];	

	RECT rect = {0};
	ptmpTab[0]->GetClientRect( &rect );
	for( i=0; i!=tabIdx; i++ ){
		ptmpTab[i]->MoveWindow( &rect );
	}
	RECT rect1, rect2;
	CWnd *pWnd = (CWnd *)&m_MainTab;
	pWnd->GetClientRect( &rect1 );			// ｸﾗｲｱﾝﾄ領域の取得
	m_MainTab.AdjustRect( FALSE, &rect1 );	// ﾀﾌﾞｺﾝﾄﾛｰﾙの表示領域の計算
	rect2.left   = rect1.left;
	rect2.top    = rect1.top;
	rect2.bottom = rect1.bottom;
	rect2.right  = rect1.right;
	for( i=0; i!=tabIdx; i++ ){
		if( ptmpTab[i] ){
			// ダイアログの移動
			ptmpTab[i]->MoveWindow( &rect2 );
		}
	}
	// 初期ページ設定
//-- NakaG_TEST
//	m_MainTab.SetCurSel( m_TabNoControl );
//--
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );
//	SetIemPositionEx( m_TabNoControl );
	SetItemPositionEx( 0 );//yoshida150311
//	SetIemPositionEx( 0 );
//-- NakaG_TEST
	// 各表の更新処理後の再表示
//--
	// 各表の更新処理後の再表示
	ReInitSpcDisp( 0 );

	return 0;
}

//-----------------------------------------------------------------------------
// タブ内のダイアログを全消去(特定収入)
//-----------------------------------------------------------------------------
void CTaxationListView::DeleteInsideSpcDlg()
{
	// ビュー内部からフォーカスを外す！
	m_MainTab.SetFocus();

	// 特定収入計算表１
	if( m_pShinSpecific1 ){
		m_pShinSpecific1->DestroyWindow();
		delete m_pShinSpecific1;
		m_pShinSpecific1 = NULL;	
	}

	// 特定収入計算表２－１
	if( m_pShinSpecific21 ){
		m_pShinSpecific21->DestroyWindow();
		delete m_pShinSpecific21;
		m_pShinSpecific21 = NULL;	
	}

	// 特定収入計算表２－２
	if( m_pShinSpecific22per8 ){
		m_pShinSpecific22per8->DestroyWindow();
		delete m_pShinSpecific22per8;
		m_pShinSpecific22per8 = NULL;	
	}
	if( m_pShinSpecific22per5 ){
		m_pShinSpecific22per5->DestroyWindow();
		delete m_pShinSpecific22per5;
		m_pShinSpecific22per5 = NULL;	
	}

	// 特定収入計算表３、４
	if( m_pShinSpecific34 ){
		m_pShinSpecific34->DestroyWindow();
		delete m_pShinSpecific34;
		m_pShinSpecific34 = NULL;	
	}

	// 特定収入計算表５－１
	if( m_pShinSpecific51per8 ){
		m_pShinSpecific51per8->DestroyWindow();
		delete m_pShinSpecific51per8;
		m_pShinSpecific51per8 = NULL;	
	}
	if( m_pShinSpecific51per5 ){
		m_pShinSpecific51per5->DestroyWindow();
		delete m_pShinSpecific51per5;
		m_pShinSpecific51per5 = NULL;	
	}

	// 特定収入計算表５－２
	if( m_pShinSpecific52per8 ){
		m_pShinSpecific52per8->DestroyWindow();
		delete m_pShinSpecific52per8;
		m_pShinSpecific52per8 = NULL;	
	}
	if( m_pShinSpecific52per5 ){
		m_pShinSpecific52per5->DestroyWindow();
		delete m_pShinSpecific52per5;
		m_pShinSpecific52per5 = NULL;	
	}

	// 特定収入計算表５－３
	if( m_pShinSpecific53per8 ){
		m_pShinSpecific53per8->DestroyWindow();
		delete m_pShinSpecific53per8;
		m_pShinSpecific53per8 = NULL;	
	}
	if( m_pShinSpecific53per5 ){
		m_pShinSpecific53per5->DestroyWindow();
		delete m_pShinSpecific53per5;
		m_pShinSpecific53per5 = NULL;	
	}

	// 特定収入計算表６
	if( m_pShinSpecific6 ){
		m_pShinSpecific6->DestroyWindow();
		delete m_pShinSpecific6;
		m_pShinSpecific6 = NULL;	
	}

//2017.03.01 INSERT START
	if( m_pShinSpecific1EX ){
		m_pShinSpecific1EX->DestroyWindow();
		delete m_pShinSpecific1EX;
		m_pShinSpecific1EX = NULL;
	}

	if( m_pShinSpecific1EXkeika ){
		m_pShinSpecific1EXkeika->DestroyWindow();
		delete m_pShinSpecific1EXkeika;
		m_pShinSpecific1EXkeika = NULL;
	}

	if( m_pShinSpecific21EX2 ){
		m_pShinSpecific21EX2->DestroyWindow();
		delete m_pShinSpecific21EX2;
		m_pShinSpecific21EX2 = NULL;
	}

	if( m_pShinSpecific22keika ){
		m_pShinSpecific22keika->DestroyWindow();
		delete m_pShinSpecific22keika;
		m_pShinSpecific22keika = NULL;
	}

	if( m_pShinSpecific51keika ){
		m_pShinSpecific51keika->DestroyWindow();
		delete m_pShinSpecific51keika;
		m_pShinSpecific51keika = NULL;
	}

	if( m_pShinSpecific52keika ){
		m_pShinSpecific52keika->DestroyWindow();
		delete m_pShinSpecific52keika;
		m_pShinSpecific52keika = NULL;
	}

	if( m_pShinSpecific53keika ){
		m_pShinSpecific53keika->DestroyWindow();
		delete m_pShinSpecific53keika;
		m_pShinSpecific53keika = NULL;
	}
//2017.03.01 INSERT END
	// データ削除
	if( m_pSpcListData ){
		delete m_pSpcListData;
		m_pSpcListData = NULL;
	}

	m_TabMng.RemoveAll();
	m_pFirstPage = NULL;
	memset( m_ListType, '\0', sizeof(m_ListType) );

	return;
}

//-----------------------------------------------------------------------------
// フォーカスセット
//-----------------------------------------------------------------------------
// 引数	selno	：	選択帳票
//		IsInEnd	：	終了処理中？
//-----------------------------------------------------------------------------
void CTaxationListView::SetItemPositionEx( int selno, BOOL IsInEnd/*=FALSE*/ )//yoshida150311
//void CTaxationListView::SetIemPositionEx( int selno )
{
	// 閾値チェック
	int cnt = (int)m_TabMng.GetCount();
	if( (selno<0) || (cnt<=selno) ){
		return;
	}

//2017.03.01 UPDATE START
//	if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_1_TAB ){
//		((CShinSpecific1*)m_TabMng[selno].pWnd)->SetItemPosition(  );//yoshida150310
////		((CShinSpecific1*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_1_TAB ){
//		((CShinSpecific21EX*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );//yoshida150311
////		((CShinSpecific21EX*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_2_TAB ){
//		((CShinSpecific22*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );//yoshida150316
////		((CShinSpecific22*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_3or4_TAB ){
//		((CShinSpecific34*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_1_TAB ){
//		((CShinSpecific51*)m_TabMng[selno].pWnd)->SetItemPosition( );//yoshida150316
////		((CShinSpecific51*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_2_TAB ){
//		((CShinSpecific52*)m_TabMng[selno].pWnd)->SetItemPosition( );//yoshida150316
////		((CShinSpecific52*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_3_TAB ){
//		((CShinSpecific53*)m_TabMng[selno].pWnd)->SetItemPosition( );//yoshida150316
////		((CShinSpecific53*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}
//	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_6_TAB ){
//		((CShinSpecific6*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
//	}

	if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_1_TAB ){
		if( m_pShinSpecific1EX ){
			((CShinSpecific1EX*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );
		}else if( m_pShinSpecific1EXkeika ){
			((CShinSpecific1EXkeika*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );
		}else{
			((CShinSpecific1*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_1_TAB ){
		if(  m_pShinSpecific21EX2 ){
			((CShinSpecific21EX2*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );
		}else{
			((CShinSpecific21EX*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_2_TAB ){
		if(  m_pShinSpecific22keika ){
			((CShinSpecific22keika*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );
		}else{
			((CShinSpecific22*)m_TabMng[selno].pWnd)->SetItemPosition( IsInEnd );
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_3or4_TAB ){
		((CShinSpecific34*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_1_TAB ){
		if( m_pShinSpecific51keika ){
			((CShinSpecific51keika*)m_TabMng[selno].pWnd)->SetItemPosition( );
		}else{
			((CShinSpecific51*)m_TabMng[selno].pWnd)->SetItemPosition( );
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_2_TAB ){
		if( m_pShinSpecific52keika ){
			((CShinSpecific52keika*)m_TabMng[selno].pWnd)->SetItemPosition( );
		}else{
			((CShinSpecific52*)m_TabMng[selno].pWnd)->SetItemPosition( );
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_3_TAB ){
		if( m_pShinSpecific53keika ){
			((CShinSpecific53keika*)m_TabMng[selno].pWnd)->SetItemPosition( );
		}else{
			((CShinSpecific53*)m_TabMng[selno].pWnd)->SetItemPosition( );
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_6_TAB ){
		((CShinSpecific6*)m_TabMng[selno].pWnd)->SetItemPosition( 0 );
	}

//2017.03.01 UPDATE END
}

//-----------------------------------------------------------------------------
// データ書込み(特定収入)
//-----------------------------------------------------------------------------
// 引数	isEnd	：	最終データ？
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::WriteSpcData( BOOL isEnd )
{ 
	if( m_pSpcListData ){
		//------------>yoshida150309
		if( m_pSpcListData->SetData(m_pZmSub, m_pSnHeadData, m_SnSeq) ){
			return -1;
		}
		//<-------------------------
		if( isEnd ){
			delete m_pSpcListData;
			m_pSpcListData = NULL;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 全画面：画面表示 特定収入計算表 再初期化
//-----------------------------------------------------------------------------
// 引数	selno	：	選択番号
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::ReInitSpcDisp( int selno )
{
	// 閾値チェック
	int cnt = (int)m_TabMng.GetCount();
	if( (selno<0) || (cnt<=selno) ){
		return 0;
	}
	if( m_TabMng[selno].pWnd == NULL ){
		return 0;
	}

//2017.03.01 UPDATE START
	//if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_1_TAB ){
	//	((CShinSpecific1*)m_TabMng[selno].pWnd)->SpecificInit1();
	//}
	//else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_1_TAB ){
	//	((CShinSpecific21EX*)m_TabMng[selno].pWnd)->SpecificInit21();
	//}
	//else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_2_TAB ){
	//	((CShinSpecific22*)m_TabMng[selno].pWnd)->SpecificInit22();
	//}
	//else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_3or4_TAB ){
	//	((CShinSpecific34*)m_TabMng[selno].pWnd)->SpecificInit34();
	//}
	//else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_1_TAB ){
	//	((CShinSpecific51*)m_TabMng[selno].pWnd)->SpecificInit51();
	//}
	//else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_2_TAB ){
	//	((CShinSpecific52*)m_TabMng[selno].pWnd)->SpecificInit52();
	//}
	//else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_3_TAB ){
	//	((CShinSpecific53*)m_TabMng[selno].pWnd)->SpecificInit53();
	//}
	//else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_6_TAB ){
	//	((CShinSpecific6*)m_TabMng[selno].pWnd)->SpecificInit6();
	//}

	if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_1_TAB ){
		if( m_pShinSpecific1EX ){
			((CShinSpecific1EX*)m_TabMng[selno].pWnd)->SpecificInit1();
		}else if( m_pShinSpecific1EXkeika ){
			((CShinSpecific1EXkeika*)m_TabMng[selno].pWnd)->SpecificInit1();
		}else{
			((CShinSpecific1*)m_TabMng[selno].pWnd)->SpecificInit1();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_1_TAB ){
		if( m_pShinSpecific21EX2 ){
			((CShinSpecific21EX2*)m_TabMng[selno].pWnd)->SpecificInit21();
		}else{
			((CShinSpecific21EX*)m_TabMng[selno].pWnd)->SpecificInit21();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_2_2_TAB ){
		if( m_pShinSpecific22keika ){
			((CShinSpecific22keika*)m_TabMng[selno].pWnd)->SpecificInit22();
		}else{
			((CShinSpecific22*)m_TabMng[selno].pWnd)->SpecificInit22();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_3or4_TAB ){
		((CShinSpecific34*)m_TabMng[selno].pWnd)->SpecificInit34();
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_1_TAB ){
		if( m_pShinSpecific51keika ){
			((CShinSpecific51keika*)m_TabMng[selno].pWnd)->SpecificInit51();
		}else{
			((CShinSpecific51*)m_TabMng[selno].pWnd)->SpecificInit51();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_2_TAB ){
		if( m_pShinSpecific52keika ){
			((CShinSpecific52keika*)m_TabMng[selno].pWnd)->SpecificInit52();
		}else{
			((CShinSpecific52*)m_TabMng[selno].pWnd)->SpecificInit52();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_5_3_TAB ){
		if( m_pShinSpecific53keika ){
			((CShinSpecific53keika*)m_TabMng[selno].pWnd)->SpecificInit53();
		}else{
			((CShinSpecific53*)m_TabMng[selno].pWnd)->SpecificInit53();
		}
	}
	else if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_6_TAB ){
		((CShinSpecific6*)m_TabMng[selno].pWnd)->SpecificInit6();
	}
//2017.03.01 UPDATE END

	return 0;
}

//-----------------------------------------------------------------------------
// 全画面：画面表示 特定収入計算表 再初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::ReInitSpcDisp()
{
	// テスト的に作成！！！

	if( m_pShinSpecific1 ){
		m_pShinSpecific1->SpecificInit1();
	}
	if( m_pShinSpecific21 ){
		m_pShinSpecific21->SpecificInit21();
	}
	if( m_pShinSpecific22per8 ){
		m_pShinSpecific22per8->SpecificInit22();
	}
	if( m_pShinSpecific22per5 ){
		m_pShinSpecific22per5->SpecificInit22();
	}
	if( m_pShinSpecific34 ){
		m_pShinSpecific34->SpecificInit34();
	}
	if( m_pShinSpecific51per8 ){
		m_pShinSpecific51per8->SpecificInit51();
	}
	if( m_pShinSpecific51per5 ){
		m_pShinSpecific51per5->SpecificInit51();
	}
	if( m_pShinSpecific52per8 ){
		m_pShinSpecific52per8->SpecificInit52();
	}
	if( m_pShinSpecific52per5 ){
		m_pShinSpecific52per5->SpecificInit52();
	}
	if( m_pShinSpecific53per8 ){
		m_pShinSpecific53per8->SpecificInit53();
	}
	if( m_pShinSpecific53per5 ){
		m_pShinSpecific53per5->SpecificInit53();
	}
	if( m_pShinSpecific6 ){
		m_pShinSpecific6->SpecificInit6();
	}
//2017.03.01 INSERT START
	if( m_pShinSpecific1EX ){
		m_pShinSpecific1EX->SpecificInit1();
	}
	if( m_pShinSpecific1EXkeika ){
		m_pShinSpecific1EXkeika->SpecificInit1();
	}
	if( m_pShinSpecific21EX2 ){
		m_pShinSpecific21EX2->SpecificInit21();
	}
	if( m_pShinSpecific22keika ){
		m_pShinSpecific22keika->SpecificInit22();
	}
	if( m_pShinSpecific51keika ){
		m_pShinSpecific51keika->SpecificInit51();
	}
	if( m_pShinSpecific52keika ){
		m_pShinSpecific52keika->SpecificInit52();
	}
	if( m_pShinSpecific53keika ){
		m_pShinSpecific53keika->SpecificInit53();
	}
//2017.03.01 INSERT END
	return 0;
}

//-----------------------------------------------------------------------------
// 本表データとの連動（'15.03.09）
//-----------------------------------------------------------------------------
// 引数	IsChgDoSpc	：	特定収入を加味するかどうかに変更有？
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::LinkHonpyoData( BOOL IsChgDoSpc )
{
//2017.03.01 DELETE START
//	if( m_pSnHeadData->Sn_Sign4&0x80 ){
//2017.03.01 DELETE END
		char	WORK0[MONY_BUF_SIZE]={0};

		m_pSnHeadData->Sn_Sign2 &= 0xfd;
		m_Arith.l_input( WORK0, _T("50") );
		if( m_Arith.l_cmp(m_pSpcListData->stval.SpTswr, WORK0) <= 0 ){
			m_pSnHeadData->Sn_Sign2 |= 0x02;
//--> '15.04.28 INS START
			if( IsChgDoSpc ){
				SetHonpyoMony();
			}
//<-- '15.04.28 INS END
		}
		else{
			SetHonpyoMony();
		}
//2017.03.01 DELETE START
//	}
//2017.03.01 DELETE END
	return 0;
}

//-----------------------------------------------------------------------------
// 本表データへの書き込み（'15.03.09）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::SetHonpyoMony()
{
	// 書込みデータ
	SPCPOST_DATA	postData={0};

	// 書込みデータ作成
	memmove( postData.Sn_2F21A, m_pSpcListData->stval.SpSiz3, sizeof(postData.Sn_2F21A) );
	memmove( postData.Sn_2F21B, m_pSpcListData->stval.SpSiz4, sizeof(postData.Sn_2F21B) );
	memmove( postData.Sn_2F21C, m_pSpcListData->stval.SpSiz63, sizeof(postData.Sn_2F21C) );

//--> '15.04.28 INS START
	postData.Sn_Sign2 = m_pSnHeadData->Sn_Sign2;
//<-- '15.04.28 INS END

//--> '15.04.29 INS START
	m_isLinkingSpcToHonpyo = TRUE;
//<-- '15.04.29 INS END

	//ここの処理でフォーカスがセットされない。処理方法をやめる。yoshida150313
	m_pShinInfo->pParent->SendMessage( WM_USER_SPCPOST_RECALQ, (WPARAM)&postData );

//--> '15.04.29 INS START
	m_isLinkingSpcToHonpyo = FALSE;
//<-- '15.04.29 INS END

	//yoshida150313

//	SetItemPositionEx(1);


	return 0;
}

//2015.03.05 INSERT START
// 計算表２（1）　ページ数取得
int CTaxationListView::GetMxList21Ex()
{
	int ret = 0;
	TSREC TSrec;

	if( m_pSpcListData->TsTbl.tp_rnum ){

		int line_cnt;
		for( line_cnt = m_pSpcListData->TsTbl.tp_rnum; line_cnt > 0; line_cnt-- ){
			if(m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, line_cnt-1 )){
				return -1;
			}

			BOOL sgn = FALSE;
			char tmp[6]={'\0'};

			//内容
			if( strlen(TSrec.TsName) != 0 ){
				sgn = TRUE;
			}

			//資産の譲渡等の対価以外の収入
			if(m_Arith.l_test(TSrec.TsJsyu) != 0 ){
				sgn = TRUE;
			}

			//左のうち特定収入
			memset(tmp,'\0',sizeof(tmp));
			m_Arith.l_add( tmp, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );
			if(m_Arith.l_test(tmp) != 0 ){
				sgn = TRUE;
			}

			//Ａのうち課税仕入れ等～　８％
			if( m_pSnHeadData->IsKobetuSiireAnbun() == TRUE ){
				if(m_Arith.l_test(TSrec.TsDtl[1].TsSsyu) != 0 ){
					sgn = TRUE;
				}
			}else{
				if(m_Arith.l_test(TSrec.TsDtl[1].TsTsyu) != 0 ){
					sgn = TRUE;
				}
			}

			//Ａのうち課税仕入れ等～　５％
			if( m_pSnHeadData->IsKobetuSiireAnbun() == TRUE ){
				if(m_Arith.l_test(TSrec.TsDtl[0].TsSsyu) != 0 ){
					sgn = TRUE;
				}
			}else{
				if(m_Arith.l_test(TSrec.TsDtl[0].TsTsyu) != 0 ){
					sgn = TRUE;
				}
			}

			//Ａ－Ｂの金額～
			memset(tmp,'\0',sizeof(tmp));
			m_Arith.l_add( tmp, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
			if(m_Arith.l_test(tmp) != 0 ){
				sgn = TRUE;
			}

			if( sgn == TRUE ){
				break;
			}
		}

//		ret = ( m_pSpcListData->TsTbl.tp_rnum / LINES_PER_PAGE_21 );
		ret = ( line_cnt / LINES_PER_PAGE_21 );

//		if( m_pSpcListData->TsTbl.tp_rnum % LINES_PER_PAGE_21 ){
		if( line_cnt % LINES_PER_PAGE_21 ){
			ret++;
		}
	}

	//if( ret == 0 ){
	//	ret = 1; //データが無くても白紙ページ出力するため
	//}

	return ret;
}

// 計算表２（2）８％　ページ数取得
int CTaxationListView::GetMxList22_8per()
{
	int		ret = 0;
	TSREC	TSrec;
	int		line_cnt = 0;

	if( m_pSpcListData->TsTbl.tp_rnum ){

		if( m_pShinSpecific22per8 ){
			for(int rec_cnt = 0; rec_cnt < m_pSpcListData->TsTbl.tp_rnum; rec_cnt++ ){

				if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rec_cnt ) ){
					return -1;
				}

				if( m_pShinSpecific22per8->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}else{
					line_cnt++;
				}
			}
		}

//		ret = ( m_pSpcListData->TsTbl.tp_rnum / LINES_PER_PAGE );
		ret = ( line_cnt / LINES_PER_PAGE_22 );

//		if( m_pSpcListData->TsTbl.tp_rnum % LINES_PER_PAGE ){
		if( line_cnt % LINES_PER_PAGE_22 ){
			ret++;
		}
	}

	//if( ret == 0 ){
	//	ret = 1; //データが無くても白紙ページ出力するため
	//}

	return ret;
}

// 計算表２（2）５％　ページ数取得
int CTaxationListView::GetMxList22_5per()
{
	int ret = 0;
	TSREC	TSrec;
	int		line_cnt = 0;

	if( m_pSpcListData->TsTbl.tp_rnum ){

		if( m_pShinSpecific22per5 ){
			for(int rec_cnt = 0; rec_cnt < m_pSpcListData->TsTbl.tp_rnum; rec_cnt++ ){

				if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rec_cnt ) ){
					return -1;
				}

				if( m_pShinSpecific22per5->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}else{
					line_cnt++;
				}
			}
		}

//		ret = ( m_pSpcListData->TsTbl.tp_rnum / LINES_PER_PAGE );
		ret = ( line_cnt / LINES_PER_PAGE_22 );

//		if( m_pSpcListData->TsTbl.tp_rnum % LINES_PER_PAGE ){
		if( line_cnt % LINES_PER_PAGE_22 ){
			ret++;
		}
	}

	//if( ret == 0 ){
	//	ret = 1; //データが無くても白紙ページ出力するため
	//}

	return ret;
}

// 計算表２（2）８％　ページの先頭のレコード番号配列作成
int CTaxationListView::MakePageManage22_8per()
{
	TSREC	TSrec;
	int		line_cnt = 0;

	m_PageManage_22per8.RemoveAll();

	if( m_pSpcListData->TsTbl.tp_rnum ){

		if( m_pShinSpecific22per8 ){
			for(int rec_cnt = 0; rec_cnt < m_pSpcListData->TsTbl.tp_rnum; rec_cnt++ ){

				if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rec_cnt ) ){
					return -1;
				}

				if( m_pShinSpecific22per8->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}

				if(( line_cnt % LINES_PER_PAGE_22 ) == 0){
					m_PageManage_22per8.Add( rec_cnt ); //ページ先頭のレコード番号を記憶
				}

				line_cnt++;
			}
		}
	}

	return 0;
}
// 計算表２（2）５％　ページの先頭のレコード番号配列作成
int CTaxationListView::MakePageManage22_5per()
{
	TSREC	TSrec;
	int		line_cnt = 0;

	m_PageManage_22per5.RemoveAll();

	if( m_pSpcListData->TsTbl.tp_rnum ){

		if( m_pShinSpecific22per5 ){
			for(int rec_cnt = 0; rec_cnt < m_pSpcListData->TsTbl.tp_rnum; rec_cnt++ ){

				if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rec_cnt ) ){
					return -1;
				}

				if( m_pShinSpecific22per5->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}

				if(( line_cnt % LINES_PER_PAGE_22 ) == 0){
					m_PageManage_22per5.Add( rec_cnt ); //ページ先頭のレコード番号を記憶
				}

				line_cnt++;
			}
		}
	}

	return 0;
}

//2017.03.01 INSERT START
// 計算表２（1）　ページ数取得
int CTaxationListView::GetMxList21Ex2()
{
	int ret = 0;
	TSREC TSrec;

	if( m_pSpcListData->TsTbl.tp_rnum ){

		int line_cnt;
		for( line_cnt = m_pSpcListData->TsTbl.tp_rnum; line_cnt > 0; line_cnt-- ){
			if(m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, line_cnt-1 )){
				return -1;
			}

			BOOL sgn = FALSE;
			char tmp[6]={'\0'};

			//内容
			if( strlen(TSrec.TsName) != 0 ){
				sgn = TRUE;
			}

			//資産の譲渡等の対価以外の収入
			if(m_Arith.l_test(TSrec.TsJsyu) != 0 ){
				sgn = TRUE;
			}

			//左のうち特定収入
			if(m_Arith.l_test(TSrec.TsDtl[1].TsGsyu) != 0 ){
				sgn = TRUE;
			}

			//Ａのうち課税仕入れ等～　８％
			if( m_pSnHeadData->IsKobetuSiireAnbun() == TRUE ){
				if(m_Arith.l_test(TSrec.TsDtl[1].TsSsyu) != 0 ){
					sgn = TRUE;
				}
			}else{
				if(m_Arith.l_test(TSrec.TsDtl[1].TsTsyu) != 0 ){
					sgn = TRUE;
				}
			}

			//Ａ－Ｂの金額～
			if(m_Arith.l_test(TSrec.TsDtl[1].TsFsyu) != 0 ){
				sgn = TRUE;
			}

			if( sgn == TRUE ){
				break;
			}
		}

		ret = ( line_cnt / LINES_PER_PAGE_21 );

		if( line_cnt % LINES_PER_PAGE_21 ){
			ret++;
		}
	}

	return ret;
}

// 計算表２（2）経過措置　ページ数取得
int CTaxationListView::GetMxList22_keika()
{
	int		ret = 0;
	TSREC	TSrec;
	int		line_cnt = 0;

	if( m_pSpcListData->TsTbl.tp_rnum ){

		if( m_pShinSpecific22keika ){
			for(int rec_cnt = 0; rec_cnt < m_pSpcListData->TsTbl.tp_rnum; rec_cnt++ ){

				if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rec_cnt ) ){
					return -1;
				}

				if( m_pShinSpecific22keika->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}else{
					line_cnt++;
				}
			}
		}

		ret = ( line_cnt / LINES_PER_PAGE_22 );

		if( line_cnt % LINES_PER_PAGE_22 ){
			ret++;
		}
	}

	return ret;
}



// 計算表２（2）経過措置　ページの先頭のレコード番号配列作成
int CTaxationListView::MakePageManage22_keika()
{
	TSREC	TSrec;
	int		line_cnt = 0;

	m_PageManage_22keika.RemoveAll();

	if( m_pSpcListData->TsTbl.tp_rnum ){

		if( m_pShinSpecific22keika ){
			for(int rec_cnt = 0; rec_cnt < m_pSpcListData->TsTbl.tp_rnum; rec_cnt++ ){

				if( m_pSpcListData->m_TblHdl.th_rget( &TSrec, &m_pSpcListData->TsTbl, rec_cnt ) ){
					return -1;
				}

				if( m_pShinSpecific22keika->IsNowZrtData( &TSrec ) == FALSE ){
					continue;
				}

				if(( line_cnt % LINES_PER_PAGE_22 ) == 0){
					m_PageManage_22keika.Add( rec_cnt ); //ページ先頭のレコード番号を記憶
				}

				line_cnt++;
			}
		}
	}

	return 0;
}
// 計算書を新様式(国からの正式発表の形式)で出力するか
BOOL CTaxationListView::IsKeikaType_Keisansyo()
{
	BOOL ret = FALSE;

	if( m_pSnHeadData->Sn_KDAYS >= KEISANSYO_KEIKA ){ //課税期間が28年4月以降開始は新形式で出力
		ret = TRUE;
	}
	
	return ret;
}

//2017.03.01 INSERT END
BOOL CTaxationListView::IsRatioOver95()
{
	BOOL ret;

	ret =  m_pSpcListData->IsRatioOver95(m_pSnHeadData);

	return ret;
}
//2015.03.05 INSERT END

// データ集計
void CTaxationListView::OnButtonF11()
{
	if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		return;
	}

	int selno = m_MainTab.GetCurSel();

//2017.03.01 INSERT START
	if( m_TabMng[selno].tabtype == ID_ICSSH_SPC_1_TAB ){
		if( m_pShinSpecific1EX ){
			m_pShinSpecific1EX->GetDiagData();
		}else if( m_pShinSpecific1EXkeika ){
			m_pShinSpecific1EXkeika->GetDiagData();
		}
	}
	WriteSpcData( FALSE );
//2017.03.01 INSERT END

	if( m_pShinInfo && m_pShinInfo->pParent ){
		// 再集計
		m_pShinInfo->pParent->SendMessage( WM_USER_RECALQ, (WPARAM)1 );
	}
	// 再表示
	ReInitSpcDisp();
	m_MainTab.SetCurSel( selno );
	m_MainTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );
	SetItemPositionEx( selno );
}

// 集計ボタン
void CTaxationListView::OnUpdateButtonF11(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL	bON = FALSE;

	if( m_SelType == ID_ICSSH_SPCLIST_TYPE ){
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
//--> '15.06.01 INS START
	else if( m_SelType == ID_ICSSH_TAXLIST_TYPE ){
		if( pCmdUI ){
			pCmdUI->Enable( FALSE );
		}
	}
//<-- '15.06.01 INS END
}

//-----------------------------------------------------------------------------
// 特定収入の再計算('15.04.03)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationListView::RecalqSpc( SYZTOSPC_DATA *pSyzToSpc )
{
	if( m_pSpcListData == NULL ){
		return -1;
	}

	BOOL	isChgRatioOver95 = FALSE;


	//---->yoshida150421
	//今回表示するタブを判定し、現在表示されているタブと比較し、違っていれば再作成フラグを立てる
	if( m_pSpcListData->IsRatioOver95(m_pSnHeadData) == FALSE ){//課税売上９５％未満の場合
		if( m_pSnHeadData->IsKobetuSiireAnbun()){
			//計算表５－２を表示する
//2017.03.01 UPDATE START
//			if( m_pShinSpecific52per8 == NULL ){
			if(( m_pShinSpecific52per8 == NULL )&&(( m_pShinSpecific52keika == NULL ))){
//2017.03.01 UPDATE END
				isChgRatioOver95 = TRUE;
			}
		}
		else{
			//計算表５－３を表示する
//2017.03.01 UPDATE START
//			if( m_pShinSpecific53per8 == NULL ){
			if(( m_pShinSpecific53per8 == NULL )&&(( m_pShinSpecific53keika == NULL ))){
//2017.03.01 UPDATE END
				isChgRatioOver95 = TRUE;
			}
		}
	}
	else{//課税売上９５％以上の場合
		//計算表５－１を表示する
//2017.03.01 UPDATE START
//		if( m_pShinSpecific51per8 == NULL ){
		if(( m_pShinSpecific51per8 == NULL )&&(( m_pShinSpecific51keika == NULL ))){
//2017.03.01 UPDATE END
			isChgRatioOver95 = TRUE;
		}
	}
	//<------------------

//--> '15.04.29 INS START
	if( m_isLinkingSpcToHonpyo == TRUE ){
		// 特定収入発信で金額を更新する時には、特定収入計算表の５シリーズの再作成はあり得ないから！
		isChgRatioOver95 = FALSE;
	}
//<-- '15.04.29 INS END


	//<-----------------
	//---->元々の処理yoshida150421
	/*
	if( ((m_pSpcListData->IsRatioOver95(m_pSnHeadData)==TRUE) && ((pSyzToSpc->IsRatioOver95&0x01)==0x00)) ||
		((m_pSpcListData->IsRatioOver95(m_pSnHeadData)==FALSE) && ((pSyzToSpc->IsRatioOver95&0x01)==0x01)) ){
		// タブ再作成
		isChgRatioOver95 = TRUE;
	}
	*/
	//<-------------


	// 控除対象仕入税額＋課税売上の取込
	if( m_pSpcListData->SetSkjUriMony(pSyzToSpc) ){
		return -1;
	}

	if( isChgRatioOver95 ){
		BeginWaitCursor();
		WriteSpcData( FALSE );

		// リソースの切り替えから行う 19.10.29
		//InitSpecificTab( TRUE );
		InitTab ( TRUE );

		EndWaitCursor();
	}

	// 控除対象仕入税額以降の計算
	m_pSpcListData->SPassOffCalqMain( m_pSnHeadData );

	// 特定収入を加味した控除対象仕入税額の取得
	if( m_pSpcListData->GetSkjMonyAfterSpc(pSyzToSpc) ){
		return -1;
	}

	return 0;
}
