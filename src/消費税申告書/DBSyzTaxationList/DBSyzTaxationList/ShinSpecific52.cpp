// ShinSpecific52.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific52.h"
#include "ShinSpecific52Idx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 18;
const	int	StartIndex = 48;

// CShinSpecific52 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific52, CSpcBaseDlg)

CShinSpecific52::CShinSpecific52(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific52::IDD, pParent)
	,m_wPos(0)
	,m_cmPos(0)
	,m_ScW(0)
	,m_zeiType(ID_ICSSH_SPC_8PER)
{
}

CShinSpecific52::CShinSpecific52(int zeitype, CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific52::IDD, pParent)
	,m_wPos(0)
	,m_cmPos(0)
	,m_ScW(0)
	,m_zeiType(zeitype)
{
}

CShinSpecific52::~CShinSpecific52()
{
}

void CShinSpecific52::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific52);
}


BEGIN_MESSAGE_MAP(CShinSpecific52, CSpcBaseDlg)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShinSpecific52 メッセージ ハンドラ

BOOL CShinSpecific52::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit52();

	CSpcBaseDlg::OnInitDialogEX();

	m_initfg = 1;

	// 確定時は全入力不可
	int Flg = 0;
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		Flg = 0;
	}
	else{
		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
			Flg = 1;
		}
	}
//--> '15.04.02 CUT START
//	if( Flg ){
//		// 実額入力
//		Color_Change( IDC_ICSDIAG8CTRL1, ID520, 0 );
//		m_Specific52.ModifyItem( ID520, DIAG_MDFY_EDIT );
//	}
//	else{
//		// 入力不可
//		Color_Change( IDC_ICSDIAG8CTRL1, ID520, 2 );
//		m_Specific52.ModifyItem( ID520, DIAG_MDFY_READONLY );
//	}
//<-- '15.04.02 CUT END

//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific52::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//----->yoshida150310
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
	//<-------------------

	/*yoshida150310

	if( pMsg->message == WM_KEYDOWN ){

		if( GetCombineKeyDown(VK_SHIFT) && (VK_F1<=pMsg->wParam) && (pMsg->wParam<=VK_F12) ){
			return TRUE;
		}

		if( (pMsg->wParam==VK_NEXT) || (pMsg->wParam==VK_PRIOR) ){
			SCROLLINFO ScrollInfo;
			int	c = GetScrollPos( SB_VERT );
			GetScrollInfo( SB_VERT, &ScrollInfo );
			if( pMsg->wParam == VK_NEXT ){
				sykscroll( c+ScrollInfo.nPage );
			}
			if( pMsg->wParam == VK_PRIOR ){
				sykscroll( c-ScrollInfo.nPage );
			}
			return 1;
		}
		if( pMsg->wParam == VK_END ){
			// 更新
			if( m_pParent ){
				//yoshida150310
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
//				m_pParent->PostMessageA( WM_CLOSE, 0 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_ESCAPE ){
			return 1;
		}
		if( pMsg->wParam == VK_F5 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F5 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_F4 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F4 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_F9 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F9 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_F11 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F11 );
			}
			return 1;
		}
	}
	return CSpcBaseDlg::PreTranslateMessage(pMsg);
	*/
}

//-----------------------------------------------------------------------------
// 再描画
//-----------------------------------------------------------------------------
void CShinSpecific52::sykscroll( int mpos )
{
	int		smax;

	smax = GetScrollLimit( SB_VERT );
	if( mpos < 0 ){
		mpos = 0;
	}
	if( mpos > smax ){
		mpos = smax;
	}
	WPARAM wParam;
	wParam = mpos << 16;
	wParam |= (int)SB_THUMBTRACK;
	PostMessage( WM_VSCROLL, wParam );
}

//----->yoshida150316
/*
//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific52::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndex;
	}

	m_Specific52.SetFocus();
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	if( !((*m_pSnHeadData)->SVmzsw == 1) ){
		m_Specific52.SetPosition( Index );
	}

	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );

	sykscroll( 0 );
}
*/
//<------------------------------------------------

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific52::Color_Change( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :		// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2 : 		// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;	
			break;
		case 3 : 		// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;		
			break;
		case 4 :		// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;	
			break;
		case 5 : 		// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default: 		// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;	
			break;
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}*/

//-----------------------------------------------------------------------------
// 初期設定
//-----------------------------------------------------------------------------
void CShinSpecific52::SpecificInit52( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		DiagData.data_disp = _T("６．３\n\n１０８");
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		DiagData.data_disp = _T("４\n\n１０５");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	diag_setdata( IDC_ICSDIAG8CTRL1, 9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	
//--> '15.04.14 INS START
//2017.03.01 UPDATE START
//	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//		DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等(税率6.3％)にのみ\n　使途が特定されている特定収入　(計算表２(2)（8%）⑰Ｅ)");//yoshida150505
////		DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等(税率6.3％)にのみ使途が特定されている\n　特定収入　(計算表２(2)（8%）⑰Ｅ)");//yoshida150429
////		DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等にのみ使途が特定されている\n　特定収入　（計算表２(2)（8%）⑰のＥ）");
//	}
//	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等(税率4％)にのみ\n　使途が特定されている特定収入　(計算表２(2)（5%）⑰Ｅ)");//yoshida150505
////		DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等(税率4％)にのみ使途が特定されている\n　特定収入　(計算表２(2)（5%）⑰Ｅ)");//yoshida150429
////		DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等にのみ使途が特定されている\n　特定収入　（計算表２(2)（5%）⑰のＥ）");
//	}
//	diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//
//	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//		DiagData.data_disp = _T("　課税・非課税売上げに共通して要する課税仕入れ等(税率6.3％)に\n　のみ使途が特定されている特定収入(計算表２(2)（8%）⑰Ｆ)");//yoshida150429
////		DiagData.data_disp = _T("　課税・非課税売上げに共通して要する課税仕入れ等にのみ使途が\n　特定されている特定収入（計算表２(2)（8%）⑰のＦ）");
//	}
//	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		DiagData.data_disp = _T("　課税・非課税売上げに共通して要する課税仕入れ等(税率4％)に\n　のみ使途が特定されている特定収入(計算表２(2)（5%）⑰Ｆ)");//yoshida150429
////		DiagData.data_disp = _T("　課税・非課税売上げに共通して要する課税仕入れ等にのみ使途が\n　特定されている特定収入（計算表２(2)（5%）⑰のＦ）");
//	}
//	diag_setdata( IDC_ICSDIAG8CTRL1, 7, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等にのみ\n　使途が特定されている特定収入　(計算表２(2)⑰のＥ)");
	}else{
		if( m_zeiType == ID_ICSSH_SPC_8PER ){
			DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等(税率6.3％)にのみ\n　使途が特定されている特定収入　(計算表２(2)（8%）⑰Ｅ)");
		}
		else if( m_zeiType == ID_ICSSH_SPC_5PER ){
			DiagData.data_disp = _T("　課税売上げにのみ要する課税仕入れ等(税率4％)にのみ\n　使途が特定されている特定収入　(計算表２(2)（5%）⑰Ｅ)");
		}
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		DiagData.data_disp = _T("　課税・非課税売上げに共通して要する課税仕入れ等に\n　のみ使途が特定されている特定収入(計算表２(2)⑰のＦ)");
	}else{
		if( m_zeiType == ID_ICSSH_SPC_8PER ){
			DiagData.data_disp = _T("　課税・非課税売上げに共通して要する課税仕入れ等(税率6.3％)に\n　のみ使途が特定されている特定収入(計算表２(2)（8%）⑰Ｆ)");
		}
		else if( m_zeiType == ID_ICSSH_SPC_5PER ){
			DiagData.data_disp = _T("　課税・非課税売上げに共通して要する課税仕入れ等(税率4％)に\n　のみ使途が特定されている特定収入(計算表２(2)（5%）⑰Ｆ)");
		}
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 7, (struct IUnknown *)&DiagData, CIcsdiagctrl );

//2017.03.01 UPDATE END
//<-- '15.04.14 INS END

	SpecificDisp52();

	IndexControl();//yoshida150313

	SetItemPosition();//yoshida150316
//	SetItemPosition(0);
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific52::SpecificDisp52( void )
{
	char			val[MONY_BUF_SIZE]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 経過措置チェック
//--> '15.03.26 CUT START
//	if( (*m_pSnHeadData)->SVmzsw == 1 ){
//
//		Color_Change( IDC_ICSDIAG8CTRL1, ID520 , 0 );//色変更yoshida150313
//		diag_modify( IDC_ICSDIAG8CTRL1, ID520, DIAG_MDFY_EDIT , CIcsdiagctrl );//旧は入力可能だったのであけたyoshida150313
//
////		Color_Change( IDC_ICSDIAG8CTRL1, ID520 , 1 );
////		diag_modify( IDC_ICSDIAG8CTRL1, ID520, DIAG_MDFY_READONLY, CIcsdiagctrl );
//	}
//	else{
//		// 確定時は全入力不可
//		if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
//			Color_Change( IDC_ICSDIAG8CTRL1, ID520 , 0 );
//			diag_modify( IDC_ICSDIAG8CTRL1, ID520, DIAG_MDFY_EDIT, CIcsdiagctrl );
//		}
//	}
//<-- '15.03.26 CUT END

	// 1. 調整前の仕入れ等の税額の合計								[SpKgzg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, sizeof(DiagData.data_val) );
	}
	else{
		ASSERT( !_T("不正税率") );
		memset( DiagData.data_val, '\0', sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID520, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 2. 課税仕入れ等のみに使途が特定されている特定収入			[SpTsyu]
//-- '15.04.25 --
//	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu63, sizeof(DiagData.data_val) );
//	}
//	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu4, sizeof(DiagData.data_val) );
//	}
//---------------
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSsyu63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSsyu4, sizeof(DiagData.data_val) );
	}
//---------------
	diag_setdata( IDC_ICSDIAG8CTRL1, ID521, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 3. ②×６．３／１０８										[SpTszg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID522, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 4. 課税・非課税売上げに共通して使途が特定されている特定収入	[SpKsyu]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsyu63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsyu4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID523, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5. ④×４／１０５											[SpZszg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID524, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5. 課税売上割合	分子										[SpKuws]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKuws, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID525, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 6. 課税売上割合　分母										[SpKuwb]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID526, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 7. ⑤×⑥													[SpKszg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKszg63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKszg4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID527, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 8. ③＋⑦													[SpGszg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID528, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 9. ①－⑧													[SpKgzg - SpKgzg]
	memset( val, '\0', sizeof(val) );
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpGszg63 );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpGszg4 );
	}
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID529, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 10. 調整割合		分子										[SpFsyu]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID52A, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 10. 調整割合		分母										[SpTuwb + SpFsyu]
	memset( val, '\0', sizeof(val) );
	m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID52B, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 11. ⑨×⑩													[SpTwag]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID52C, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 12. 特定収入に係る課税仕入れ等の税額							[SpGszg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, sizeof(DiagData.data_val) );
		m_pArith->l_add( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, (*m_pSpcListData)->stval.SpTwag63 );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, sizeof(DiagData.data_val) );
		m_pArith->l_add( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, (*m_pSpcListData)->stval.SpTwag4 );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID52D, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 13. 控除対象仕入税額											[SpSizg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID52E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

// 再計算
void CShinSpecific52::Specific52_Calq( void )
{
	// 特定収入に係る課税仕入れ等の税額計算
//-- '15.03.07 --
//	if( (*m_pSnHeadData)->SVmzsw == 1 ){
//		(*m_pSpcListData)->SPassOnCalqEx( (*m_pSnHeadData) );
//	}
//	else{
//		(*m_pSpcListData)->SPassOffCalqEx( (*m_pSnHeadData) );
//	}
//---------------
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain( *m_pSnHeadData );
	BOOL	IsChgDoSpc = FALSE;
	if( (svSn_Sign2&0x02) != ((*m_pSnHeadData)->Sn_Sign2&0x02) ){
		IsChgDoSpc = TRUE;
	}
	((CTaxationListView*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
//---------------

	SpecificDisp52();
}


HBRUSH CShinSpecific52::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific52::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( pScrollBar ){
		return;
	}
	SCROLLINFO SI;
	GetScrollInfo( SB_VERT, &SI );
	int nowPos = m_wPos;

	switch( nSBCode ){
		case SB_LINEUP:
		case SB_PAGEUP:
			if( nSBCode == SB_LINEUP ){
				m_wPos -= m_ScW;
			}
			else{
				m_wPos -= SI.nPage;
			}
			if( m_wPos <= 0 ){
				m_wPos = 0;
			}
			break;
		case SB_LINEDOWN:
		case SB_PAGEDOWN:
			if( nSBCode == SB_LINEDOWN ){
				m_wPos += m_ScW;
			}
			else{
				m_wPos += SI.nPage;
			}
			if( m_wPos >= SI.nMax-(int)SI.nPage ){
				m_wPos = SI.nMax-SI.nPage;
			}
			break;
		case SB_THUMBTRACK:
			m_wPos = nPos;
			break;
	}
	if( (nSBCode==SB_LINEDOWN) || (nSBCode==SB_LINEUP) || (nSBCode==SB_PAGEDOWN) ||
			(nSBCode==SB_PAGEUP) || (nSBCode==SB_THUMBTRACK) ){
		ScrollWindow( 0, nowPos-m_wPos );
		SetScrollPos( SB_VERT, m_wPos );
		m_cmPos = m_wPos;
	}

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CShinSpecific52::OnSize(UINT nType, int cx, int cy)
{
	if( m_initfg ){
		SetScrollPos( SB_VERT, 0, TRUE );
		ScrollWindow( 0, m_cmPos, NULL );
		m_cmPos = 0;
	}

	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_initfg ){
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect( &a );
		m_Specific52.GetRect( BottomIndex, (LPUNKNOWN)&b );
		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE )
			ScrollInfo.nPos = 0;
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom + 30) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}
}

BEGIN_EVENTSINK_MAP(CShinSpecific52, CSpcBaseDlg)
	ON_EVENT(CShinSpecific52, IDC_ICSDIAG8CTRL1, 1, CShinSpecific52::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52, IDC_ICSDIAG8CTRL1, 2, CShinSpecific52::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52, IDC_ICSDIAG8CTRL1, 3, CShinSpecific52::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific52::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinSpecific52::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//--> '15.04.29 INS START
	// 計算表5(2)で編集を無くしたのでカット
	return;
//<-- '15.04.29 INS END

	ASSERT( m_pSpcListData );
	if( m_pSpcListData == NULL ){
		return;
	}
	ASSERT( *m_pSpcListData );
	if( *m_pSpcListData == NULL ){
		return;
	}
//--> '15.03.18 INS START
	if( IsWindowVisible() == FALSE ){
		return;
	}
//<-- '15.03.18 INS END

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){
		case ID520	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( m_zeiType == ID_ICSSH_SPC_8PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
			}
			else if( m_zeiType == ID_ICSSH_SPC_5PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			}
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz63 );
			break;
		default     :
			break;
	}
	// 再計算
	Specific52_Calq();
}

void CShinSpecific52::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//--> '15.04.29 INS START
	// 計算表5(2)で編集を無くしたのでカット
	return;
//<-- '15.04.29 INS END

	char	VK_FLG;
	short	wTerm;

	if( nChar == 0 ){
		return;
	}

	// SHIFT+TABをTABに、TABをENTERに変換
	VK_FLG = 0;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG&0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	switch( index ){
		case ID520 :
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( m_zeiType == ID_ICSSH_SPC_8PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
			}
			else if( m_zeiType == ID_ICSSH_SPC_5PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			}
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz63 );
			// 再計算
			Specific52_Calq();
			break;
		default :
			break;
	}
}

void CShinSpecific52::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific52::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}


//---------------->yoshida150313

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CShinSpecific52::IndexControl( void )
{
	int	Flg = 0;
//--> '15.03.26 CUT START
//	// 確定時は全入力不可
//	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
//		Flg = 0;
//	}
//	else{
//		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
//			Flg = 1;
//		}
//		else{
//			Flg = 0;
//		}
//	}
//<-- '15.03.26 CUT END
//--> '15.04.02 CUT START
//	ATRIB_MOD( IDC_ICSDIAG8CTRL1, ID520, Flg, (*m_pSnHeadData)->Sn_Sign4, Flg );
//<-- '15.04.02 CUT END
}


//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific52::SetItemPosition( )
{


	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific52.GetPosition()) != -1) {
			m_Specific52.SetPosition(index);
			m_Specific52.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific52.SetPosition(ID520);
	}
	else{
	}
	SetRedrawScroll( 0 );


}

//<------------------------------