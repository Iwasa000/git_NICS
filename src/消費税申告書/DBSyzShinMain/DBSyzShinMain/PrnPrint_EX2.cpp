// PrnPrint.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "PrnReportDlg.h"
#include "PrnPrint_EX2.h"

#include "PrnPrintIdx_EX2.h"
	
extern PRNEXSUB_INF									m_PrnEXSub_KAZ;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_KAZ;

extern PRNEXSUB_INF									m_PrnEXSub_ZEI;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_ZEI;

// CPrnPrint_EX2 ダイアログ

IMPLEMENT_DYNAMIC(CPrnPrint_EX2, ICSDialog)

CPrnPrint_EX2::CPrnPrint_EX2(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnPrint_EX2::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_swBeForked(0)
	, m_PrselState(ID_SEP_PRINT_SEL_STATE)
	, m_isNoneMTForm ( FALSE )
{
// 24/03/22_背景色 add -->
	m_pParent = pParent;
// 24/03/22_背景色 add <--
}

CPrnPrint_EX2::~CPrnPrint_EX2()
{
}

void CPrnPrint_EX2::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Diag1);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Diag2);
	DDX_Control(pDX, IDC_CHECK_ALLOUT, m_AlloutCheck);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
}


BEGIN_MESSAGE_MAP(CPrnPrint_EX2, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrnPrint_EX2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPrnPrint_EX2::OnBnClickedButton2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ALLOUT, &CPrnPrint_EX2::OnBnClickedCheckAllout)
	ON_BN_CLICKED(IDC_RADIO1,&CPrnPrint_EX2::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2,&CPrnPrint_EX2::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON4, &CPrnPrint_EX2::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPrnPrint_EX2::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CPrnPrint_EX2::OnBnClickedButton3)
END_MESSAGE_MAP()


// CPrnPrint_EX2 メッセージ ハンドラ

BOOL CPrnPrint_EX2::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	
	Init();
	m_SelSw = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CPrnPrint_EX2::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->wParam == VK_F10 ) 
		return TRUE;
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 )	return TRUE;
		if( pMsg->wParam == VK_ESCAPE ) {
			CWnd*	p1	=	GetParent();
			CWnd*	p2	=	p1->GetParent();
			((CPrnReportDlg*)p2)->OnBnClickedCancel();
			return TRUE;
		}

		//////////
		if (pMsg->wParam == VK_F6) {
			GetInputItemData(m_Diag2.GetPosition());
			OnBnClickedButton4();
			return TRUE;
		}

		if (pMsg->wParam == VK_F7) {
			GetInputItemData(m_Diag2.GetPosition());
			OnBnClickedButton3();
			return TRUE;
		}

		if (pMsg->wParam == VK_F8) {
			GetInputItemData(m_Diag2.GetPosition());
			OnBnClickedButton5();
			return TRUE;
		}

		if( pMsg->wParam == VK_F9 ) {
			OnBnClickedButton1();
			return TRUE;
		}

		if( pMsg->wParam == VK_F11) {
//			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F11);
			return TRUE;
		}
		if( pMsg->wParam == VK_F12 ) {
			GetInputItemData(m_Diag2.GetPosition());
			OnBnClickedButton2();
			return TRUE;
		}
		if( pMsg->wParam == VK_F5 ) {
			GetInputItemData(m_Diag2.GetPosition());
			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F5);		
			return TRUE;
		}
		if( (pMsg->wParam == VK_RETURN) || 
			(pMsg->wParam == VK_TAB && !(::GetKeyState(VK_SHIFT))) ) {
			if( GetFocus()->m_hWnd == m_Radio1.m_hWnd || 
				GetFocus()->m_hWnd == m_Radio2.m_hWnd )	{
				DIAGRAM_ATTRIBUTE	da;
				// 選択可であれば提出用カラー/モノクロにフォーカスをセットする
				diag_getattr(IDC_ICSDIAG8CTRL2,COMBO_TEI,&da,CIcsdiagctrl);
				if(da.attr_bcolor == BC_WHITE)	{
					// 提出用カラー/モノクロのコンボへ移動
					m_Diag2.SetPosition(COMBO_TEI);
				}
				else	{
					// 申告書本表のチェックへ移動
					m_Diag2.SetPosition(CHEK01_1);
				}
				return(TRUE);
			}
		}
		if(pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)	{
			if(m_Diag2.GetPosition() == COMBO_TEI || m_Diag2.GetPosition() == COMBO_HIK)	{

				DIAGRAM_DATA	dd;
				m_Diag2.GetData(m_Diag2.GetPosition(),(LPUNKNOWN)&dd);
				ComboSelIcsdiag8ctrl2Ex(m_Diag2.GetPosition(),&dd);

				if(m_Diag2.GetPosition() == COMBO_TEI &&  pMsg->wParam == VK_RIGHT)	{
					m_Diag2.SetPosition(COMBO_HIK);
				}
				if(m_Diag2.GetPosition() == COMBO_HIK &&  pMsg->wParam == VK_LEFT)	{
					m_Diag2.SetPosition(COMBO_TEI);
				}
				return(TRUE);
			}
		}
// 160117 add -->
		if(pMsg->wParam == VK_DELETE)	{
			// 法人税と同じようにDeleteキーは反応しないようにする
			return(TRUE);
		}
// 160117 add <--
// 依頼No.971
		if(pMsg->wParam == VK_END && m_swBeForked)	{
			GetInputItemData(m_Diag2.GetPosition());
			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_END);
			return(TRUE);
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CPrnPrint_EX2::Init()
{
	char			buf[512] = {0};
	DIAGRAM_DATA	DiagData;

	memset( buf, '\0', sizeof(buf) );
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		strcpy_s( buf, sizeof(buf), _T("一般用") );
	}
	else{
		strcpy_s( buf, sizeof(buf), _T("簡易用") );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 0, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( buf, '\0', sizeof(buf) );
	if( m_pSnHeadData->IsSoleProprietor() ){
		strcpy_s( buf, sizeof(buf), _T("個人事業者") );
	}
	else{
		strcpy_s( buf, sizeof(buf), _T("法人事業者") );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( buf, '\0', sizeof(buf) );
	switch( m_pSnHeadData->Sn_SKKBN ) {
		case 1 :
			if( m_pSnHeadData->Sn_Syuusei&0x02 ){
				if( m_pSnHeadData->Sn_Syuusei&0x04 ){
					strcpy_s( buf, sizeof(buf), _T("更正の申出") );
				}
				else{
					strcpy_s( buf, sizeof(buf), _T("更正の請求") );
				}
			}
			else{
				strcpy_s( buf, sizeof(buf), _T("確定申告") );
			}
			break;
		case 2 :
			strcpy_s( buf, sizeof( buf ), _T("中間申告") );	
			break;
		case 3 :
			strcpy_s( buf, sizeof( buf ), _T("修正確定申告") );
			break;
		default:
			strcpy_s( buf, sizeof( buf ), _T("修正中間申告") );	
			break;
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 2, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( buf, '\0', sizeof(buf) );	

	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			strcpy_s( buf, sizeof(buf), _T("個別対応方式") );
		}
		else{
			strcpy_s( buf, sizeof(buf), _T("比例配分方式") );
		}
	}
	else{
		if( m_pSnHeadData->Sn_TOKUR ){	// '14.06.15
			strcpy_s( buf, sizeof(buf), _T("特例適用有") );
		}
		else{
			strcpy_s( buf, sizeof(buf), _T("特例適用無") );
		}
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( buf, '\0', sizeof(buf) );
	if( m_pSnHeadData->SVmzsw == 1 ){
		strcpy_s( buf, sizeof(buf), _T("経過措置有") );
	}
	else{
		strcpy_s( buf, sizeof(buf), _T("経過措置無") );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	//ChangeColor( IDC_ICSDIAG8CTRL2, 0, 0 );		// 表題
	// タイトルの背景色
	ChangeColor(IDC_ICSDIAG8CTRL2,TITLE_01,0);
	ChangeColor(IDC_ICSDIAG8CTRL2,TITLE_02,0);
	ChangeColor(IDC_ICSDIAG8CTRL2,TITLE_07,0);
	ChangeColor(IDC_ICSDIAG8CTRL2,TITLE_08,0);

	// ------------------------------------------------------------
	// 中央表示
	// ------------------------------------------------------------
	DIAGRAM_ATTRIBUTE	dattr;
	int					ii=0;
	//// コンボボックス
	//for(ii=COMBO_TEI; ii<=COMBO_HIK; ii++)	{
	//	diag_getattr(IDC_ICSDIAG8CTRL2,ii,&dattr,CIcsdiagctrl);
	//	dattr.attr_align = DT_CENTER;
	//	diag_setattr(IDC_ICSDIAG8CTRL2,ii,&dattr,TRUE,CIcsdiagctrl);
	//}
	// チェックボックス
	for(ii=HP_TEI_CHK; ii<=KZ_HIK_CHK; ii+=2)	{
		diag_getattr(IDC_ICSDIAG8CTRL2,ii,&dattr,CIcsdiagctrl);
		dattr.attr_align = DT_CENTER;
		diag_setattr(IDC_ICSDIAG8CTRL2,ii,&dattr,TRUE,CIcsdiagctrl);
	}

	//DIAGRAM_DATA	DD;
	//DD.data_imgdata = 0;
	//DD.text_combo.Empty();
	//if( m_pPrintCmInfo->isColorPrn ){
	//	// カラープリンター
	//	// 提出用
	//	DD.text_combo =  _T("ＫＳＫ用（ＯＣＲ　用紙）\r\n");
	//	DD.text_combo += _T("提 出 用（カラー　白紙）\r\n");
	//	DD.text_combo += _T("提 出 用（モノクロ白紙）");
	//	m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
	//	// 控用
	//	DD.data_imgdata = 0;
	//	DD.text_combo.Empty();
	////	DD.text_combo =  _T("ＫＳＫ用（ＯＣＲ　用紙）\r\n");
	//	DD.text_combo += _T("控　　用（カラー　白紙）\r\n");
	//	DD.text_combo += _T("控　　用（モノクロ白紙）");
	//	m_Diag2.SetData( COMB02, (LPUNKNOWN)&DD );
	//}
	//else{
	//	// モノクロプリンター
	//	// 提出用
	//	DD.text_combo =  _T("ＫＳＫ用（ＯＣＲ　用紙）\r\n");
	//	DD.text_combo += _T("提 出 用（モノクロ白紙）");
	//	m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
	//	// 控用
	//	DD.text_combo.Empty();
	//	DD.text_combo += _T("控　　用（モノクロ白紙）");
	//	m_Diag2.SetData( COMB02, (LPUNKNOWN)&DD );
	//	ChangeColor( IDC_ICSDIAG8CTRL2, COMB02, 2 );
	//	m_Diag2.ModifyItem( COMB02, DIAG_MDFY_READONLY );
	//}

// 22-0720 del -->
	//// モノクロプリンターが選択されていて
	//if(m_pPrintCmInfo->isColorPrn == 0)	{
	//	// カラーのビットがONになっている場合はモノクロのビットに書き換える
	//	if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x01)	{
	//		m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x01;
	//	}
	//	if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x02)	{
	//		m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x02;
	//	}
	//}
// 22-0720 del <--

	m_Radio1.SetCheck(1);
	m_Radio2.SetCheck(0);
	// 中間申告第26号様式は白紙・ＯＣＲの選択不可にする
	if(m_pSnHeadData->IsMiddleProvisional())	{
		m_Radio1.EnableWindow(FALSE);
		m_Radio2.EnableWindow(FALSE);
	}

	if( m_swBeForked ){
		if( !(m_pSnHeadData->Sn_TYPE&0x01) ) {
//			DIAGRAM_DATA	DD;
//			DD.data_imgdata = 0;
//			DD.data_combo = 1;
//			m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
//			m_pSnHeadData->Sn_TYPE &= 0xfc;
//			m_pSnHeadData->Sn_TYPE |= 0x01;
//// 改良依頼 20-1013,19-1184 修正 -->
//			// 印刷の用紙設定を「提出用（カラー  白紙）」にする
//			// カラー
//			m_pSnHeadData->Sn_TYPE |= 0x02;
//// 改良依頼 20-1013,19-1184 修正 <--
			m_Radio1.SetCheck(1);
			m_Radio2.SetCheck(0);
			m_pSnHeadData->Sn_TYPE &= 0xfc;
			m_pSnHeadData->Sn_TYPE |= 0x01;
//// 970 add -->
//			// 申告書本表(課税標準額等の内訳書)にチェックが付いていて
//			// 提出用、控用のどちらにもチェックが付いていなければ
//			// 提出用のみONにする
//			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x04)	{
//				if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x08) && 
//					!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x10) )	{
//					m_pSnHeadData->Sn_PrintOutSgn2 |= 0x08;
//				}
//			}
//			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x20)	{
//				if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x40) && 
//					!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x80) )	{
//					m_pSnHeadData->Sn_PrintOutSgn2 |= 0x40;
//				}
//			}
//// 970 add <--
		}
// 970 add -->
		// 一括印刷の場合、ＯＣＲ用紙・白紙の選択を非表示にする
		m_Radio1.EnableWindow(FALSE);
		m_Radio1.ShowWindow(SW_HIDE);
		m_Radio2.EnableWindow(FALSE);
		m_Radio2.ShowWindow(SW_HIDE);
// 970 add <--
	}

	if(m_pSnHeadData->Sn_TYPE & 0x01) {
		// 申告書本表(課税標準額等の内訳書)にチェックが付いていて
		// 提出用、控用のどちらにもチェックが付いていなければ
		// 提出用のみONにする
		// 依頼No.160994
		// →下記は元々、一括印刷時のみの処理であったが
		// 　会社情報登録・修正・削除で課税方式（原則⇔課税）を切り替えた後にも
		// 　同じことが起きていたため、常にチェックするようにした
		if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x04)	{
			if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x08) && 
				!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x10) )	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x08;
			}
		}
		if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x20)	{
			if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x40) && 
				!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x80) )	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x40;
			}
		}
	}

	//if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
	//	DD.data_imgdata = 0;
	//	DD.text_check = _T("付表５[控除対象仕入税額の計算表]");
	//	m_Diag2.SetData( CHEK03, (LPUNKNOWN)&DD );
	//	DD.data_imgdata = 0;
	//	DD.text_check = _T("付表４[旧・新税率別、消費税額計算表]");
	//	m_Diag2.SetData( CHEK04, (LPUNKNOWN)&DD );
	//	DD.data_imgdata = 0;
	//	DD.text_check = _T("付表５－(２)[控除対象仕入税額等の計算表](1/2)");
	//	m_Diag2.SetData( CHEK05, (LPUNKNOWN)&DD );
	//}

	GetDlgItem(IDC_STATIC_INCOME)->SetWindowText( " " );
// 修正No.410 del -->
	//if( *m_pShinInfo->pSgSpc ){
	//	if( m_pSnHeadData->Sn_Sign2 & 0x02 ){
	//		CString	Mes;
	//		Mes  = _T("特定収入割合が５％以下です。この場合、課税仕入等の税額の\n");
	//		Mes += _T("調整は不要のため、申告書・付表には計上しません。");
	//		GetDlgItem(IDC_STATIC_INCOME)->SetWindowText( Mes );
	//	}
	//}
	//else{
	//	if( m_pSnHeadData->IsSoleProprietor() ){
	//		CString	Mes;
	//		Mes  = _T("※国税庁から個人事業者の欄外プレプリント印字は記載不要との指導がありましたので、\n");
	//		Mes += _T("欄外プレプリント印字の出力には対応しておりません。");
	//		GetDlgItem(IDC_STATIC_INCOME)->SetWindowText( Mes );
	//		if (m_pSnHeadData->m_DispTabSgn & 0x2000) { //２割特例計算を使用する
	//			GetDlgItem(IDC_STATIC_INCOME)->SetWindowText(" ");
	//		}
	//	}
	//}
// 修正No.410 del <--

	if( m_pPrintCmInfo->OP_PSEL&0x04 ){
		if( m_pPrintCmInfo->OP_PSEL&0x02 ){
			m_PrselState = ID_ALL_PRINT_SEL_STATE;
			m_AlloutCheck.SetCheck(BST_CHECKED);
		}
		else{
			m_PrselState = ID_SEP_PRINT_SEL_STATE;
			m_AlloutCheck.SetCheck(BST_UNCHECKED);
		}
		if( m_pPrintCmInfo->OP_PSEL&0x01 ){
			m_AlloutCheck.EnableWindow(FALSE);
		}
		m_pPrintCmInfo->OP_PSEL &= ~0x04;
	}
	else if( m_pPrintCmInfo->OP_PSEL&0x01 ){
		m_pPrintCmInfo->OP_PSEL |= 0x02;
		m_PrselState = ID_ALL_PRINT_SEL_STATE;
		m_AlloutCheck.SetCheck(BST_CHECKED);
		m_AlloutCheck.EnableWindow(FALSE);
	}
	else{
		m_pPrintCmInfo->OP_PSEL &= ~0x02;
		m_PrselState = ID_SEP_PRINT_SEL_STATE;
		m_AlloutCheck.SetCheck(BST_UNCHECKED);
	}

// 160121 del -->
	//if( m_pSnHeadData->Sn_TYPE&0x01 ){
// 160121 del <--
// 160121 add -->
	// 修正依頼No.160121 予定(26号)も「全表出力可」は選択不可にする
	if((m_pSnHeadData->Sn_TYPE&0x01) && !(m_pSnHeadData->IsMiddleProvisional()))	{
// 160121 add <--
		m_AlloutCheck.EnableWindow( TRUE );
	}
	else{
		m_AlloutCheck.EnableWindow( FALSE );
	}


	//----> 20230616
	// 2割特例で経過措置の場合は簡易課税の旧税率の帳票に置き換える
	DIAGRAM_DATA data;
	data.data_imgdata = NULL;

	if( m_pSnHeadData->SVmzsw ){
		// 1-1 → 4-1
		data.text_check.Format( "付表４－１[税率別消費税額計算表兼地方消費税の課税標準となる消費税額計算表]" );
		m_Diag2.SetData( CHEK04_1, (LPUNKNOWN)&data );
		
		// 1-2 → 4-2
		data.text_check.Format( "付表４－２" );
		m_Diag2.SetData( CHEK04_2, (LPUNKNOWN)&data );

		// 1-3 → 5-1
		data.text_check.Format( "付表５－１[控除対象仕入税額等の計算表]" );
		m_Diag2.SetData( CHEK04_3, (LPUNKNOWN)&data );

		// 2-1 → 5-1(2)
		data.text_check.Format( "付表５－１（２）" );
		m_Diag2.SetData( CHEK03_1, (LPUNKNOWN)&data );

		// 2-2 → 5-2
		data.text_check.Format( "付表５－２" );
		m_Diag2.SetData( CHEK03_2, (LPUNKNOWN)&data );

		// 2-3 → 5-2(2)
		data.text_check.Format( "付表５－２（２）" );
		m_Diag2.SetData( CHEK03_3, (LPUNKNOWN)&data );
	}
	//<---- 20230616

	//----->インボイス施工日以降は準確定の付表６を付表７へ名称変更('23.07.10 Add)
// 24/03/18_税務代理書面 cor -->
	//long eymd = 0;
	//if (!(m_pSnHeadData->Sn_SKKBN % 2)) {
	//	eymd = m_pSnHeadData->Sn_MDAYE;
	//}
	//else {
	//	eymd = m_pSnHeadData->Sn_KDAYE;
	//}

	//if (eymd >= ICS_SH_INVOICE_DAY) {
// -----------------------------
	m_Eymd = 0;
	if (!(m_pSnHeadData->Sn_SKKBN % 2)) {
		m_Eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		m_Eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if (m_Eymd >= ICS_SH_INVOICE_DAY) {
// 24/03/18_税務代理書面 cor <--
		data.text_check.Format("付表７[死亡した事業者の消費税及び地方消費税の確定申告明細書]");
		m_Diag2.SetData(CHEK20, (LPUNKNOWN)&data);
	}
	//<-----


	// 入力項目設定
	SetInputItem();

	// 環境共通設定の背景色を付ける
	ICSColorInfo	info;
	int				rv=0;
	rv = ((CDBSyzShinMainApp*)AfxGetApp())->GetStanderdColor(info);
	if(rv == 0 && info.m_swOnOff == 1)	{
		m_Diag1.SetBackColor(info.ViewColor());
		m_Diag2.SetBackColor(info.ViewColor());
	}

	// 中間申告第26号様式チェック
	if( m_pSnHeadData->IsMiddleProvisional() ){
		diag_setposition( IDC_ICSDIAG8CTRL2, CHEK06, CIcsdiagctrl );
		m_pSnHeadData->Sn_TYPE = 0x01;
	}
	else{
		diag_setposition( IDC_ICSDIAG8CTRL2, CHEK01_1, CIcsdiagctrl );
	}
	//m_Diag2.SetPosition(COMB01);
// 従前から初期のフォーカス位置は「印刷[F5]」ボタンにあるので
// 上記のdiag_setposition、m_Diag2.SetPositionはきいていない
// フォーカス位置を変えたければPostMessageで処理が必要？

	return 0;
}

//-----------------------------------------------------------------------------
// 初期情報セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		pZmSub		：	公益クラス
//		pShinInfo	：	共通情報
//		pPrintCmInfo：	印刷情報
//		pPRtable	：	部数を考慮した出力帳票
//		pPGtable	：	ページ番号
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CPrnPrint_EX2::InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, 
	SH_PRINT_CMINFO *pPrintCmInfo, CArray<char>* pPRtable, CArray<int>* pPGtable, CArray<int>* pPRtable1 )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pPrintCmInfo );
	if( pPrintCmInfo == NULL ){
		return -1;
	}
	m_pPrintCmInfo = pPrintCmInfo;

	if(pPRtable == NULL)	{
		return -1;
	}
	m_pPRtableEx = pPRtable;

	if(pPGtable == NULL)	{
		return -1;
	}
	m_pPGtable = pPGtable;

	if(pPRtable1 == NULL)	{
		return -1;
	}
	m_pPRtableEx1 = pPRtable1;

	CH31HyoView	*pH31View = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( H31_HONPYO_WND_IDX ) );
	m_isNoneMTForm = pH31View->IsNoneMTForm();

	return 0;
}

// -----------------------------------------------------------------------------
// 入力項目設定
// -----------------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// -----------------------------------------------------------------------------
void CPrnPrint_EX2::SetInputItem( void )
{
	SH_ITEMINDEX	*pPrnTbl;
	int				ii=0,jj=0,flg2=0;
	
	if (m_pSnHeadData->IsMiddleProvisional()) {
		pPrnTbl = &PrnTbl1[0];
	}
	else {
		if (!(m_pSnHeadData->Sn_TYPE & 0x01)) {
			pPrnTbl = &PrnTbl2[0];
		}
		else {
			pPrnTbl = &PrnTbl3[0];
		}
	}
	
	int cnt = 0;
	while( 1 ){
		if( (pPrnTbl+cnt)->Index == 0 ){
			break;
		}
		int Flg = 0;
		if( m_pSnHeadData->SVmzsw ){
			Flg = (pPrnTbl+cnt)->UnConnect;
		}
		else{
			Flg = (pPrnTbl+cnt)->Connect;
		}
		//int hide = 0;
		switch( Flg ){
			case 1 :	// 本表・付表・第26号様式
				Flg = 0;
				if ( ( ( pPrnTbl + cnt )->Index == COMBO_TEI) || ( ( pPrnTbl + cnt )->Index == COMBO_HIK ) ||
					 ( ( pPrnTbl + cnt )->Index == CHEK06	) || ( ( pPrnTbl + cnt )->Index == CHEK18	 ) ||
					 ( ( pPrnTbl + cnt )->Index == CHEK01_1 ) ) {
					Flg = 1;
				}
				//----> 20230616
				if( m_pSnHeadData->SVmzsw ){
					if( ( pPrnTbl + cnt )->Index == CHEK04_1 || ( pPrnTbl + cnt )->Index == CHEK04_2 || ( pPrnTbl + cnt )->Index == CHEK04_3 ||
						( pPrnTbl + cnt )->Index == CHEK03_1 || ( pPrnTbl + cnt )->Index == CHEK03_2 || ( pPrnTbl + cnt )->Index == CHEK03_3 ){
						Flg = 1;
					}
				}
				//<---- 20230616
				break;
			case 2 :	// 仕入控除税額に関する明細書
			case 3 :	// 仕入控除税額に関する明細書（次葉）
			case 4 :	// 消費税の還付申告に関する明細書
				Flg = 0;
				break;
			case 5 :	// 更正の請求書
				if (m_pSnHeadData->Sn_Syuusei & 0x02) {
					Flg = 1;
				}
				else {
					Flg = 0;
				}
				break;

			case 6:	// 課税取引金額計算表
				if(m_pSnHeadData->IsSoleProprietor() != FALSE)	{
					Flg = 1;
				}
				else	{
					Flg = 0;
				}
				break;

			case 7:	// 特定収入計算表
				Flg = 0;
				break;

			case 8:	// 税務代理書面
// 24/03/18_税務代理書面 cor -->
				//Flg = 1;
// -----------------------------
// 24/03/26_税務代理書面 cor -->
				//if(m_Eymd >= R06_ZEIMUDAIRI)	Flg = 1;
				//else							Flg = 0;
// -----------------------------
// 24/04/01_168704 cor -->
				//Flg = ((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud();
// -----------------------
				if(m_pParent == NULL)	{
					Flg = 1;
				}
				else	{
// 24/07/10_個別タブから出力 cor -->
					//Flg = ((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud();
// ---------------------------------
					if( ((CPrnReportDlg*)m_pParent)->ZeitempEnableChk() > 0 &&
						((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud() == TRUE) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
// 24/07/10_個別タブから出力 cor <--
				}
// 24/04/01_168704 cor <--
// 24/03/26_税務代理書面 cor <--
// 24/03/18_税務代理書面 cor <--
				break;

			//case 6 :	// 課税取引金額計算表　事業所得用
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
			//			Flg = 0;	// 事業別売上高[13'09.19]
			//		}
			//		else {
			//			if (m_pZmSub->zvol->ind_type & 0x01) {
			//				Flg = 1;
			//			}
			//			else {
			//				Flg = 0;
			//			}
			//		}
			//	}
			//	break;
			//case 7 :	// 課税取引金額計算表　農業所得用
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
			//			Flg = 0;	// 事業別売上高[13'09.19]
			//		}
			//		else {
			//			if (m_pZmSub->zvol->ind_type & 0x04) {
			//				Flg = 1;

			//			}
			//			else {
			//				Flg = 0;
			//			}
			//		}
			//	}
			//	break;
			//case 8 :	// 課税取引金額計算表　不動産所得用
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
			//			Flg = 0;	// 事業別売上高[13'09.19]
			//		}
			//		else {
			//			if (m_pZmSub->zvol->ind_type & 0x02) {
			//				Flg = 1;
			//			}
			//			else {
			//				Flg = 0;
			//			}
			//		}
			//	}
			//	break;
			//case 9 :	// 課税売上高計算表
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		Flg = 1;
			//	}
			//	break;
			//case 11:	// 特定収入計算表
			//case 12:	// 事業別売上高[13'09.19] 
			//	Flg = 0;
			//	break;

			case 13:	// 特定課税仕入れがある場合の課税標準額等の内訳書()
				if( m_pPrintCmInfo->Shrevtype < ID_ICSSH_TKKZSIIRE_REVTYPE ){
					Flg = 0;
				}
				/*
				else if( m_pSnHeadData->IsNeedTkkzDisp() ){
					Flg = 1;
				}
				else{
					Flg = 0;
				}*/
				break;
			case 14:	// 付表６
				if ((m_pSnHeadData->IsSoleProprietor()) && (m_pSnHeadData->Sn_Syuusei & 0x10)) {
					Flg = 1;
				}
				else {
					Flg = 0;
				}
				break;
			case 15:	// 特例1
			case 16:	// 特例2
			case 17:	// 特例3
				Flg = 0;
				break;
			case 18:	// 付表６（税率別消費税額計算表）
				if( m_pSnHeadData->SVmzsw ){
					Flg = 0;
				}
				else{
					Flg = 1;
				}
				break;
			default:	// 入力不可
				Flg = 0;
				//if ( m_isNoneMTForm ) {
				//	if ( ( ( pPrnTbl + cnt )->Index == CHEK03_1 ) || ( ( pPrnTbl + cnt )->Index == CHEK03_2 ) || ( ( pPrnTbl + cnt )->Index == CHEK04_1 ) || ( ( pPrnTbl + cnt )->Index == CHEK04_2 ) ) {
				//		hide = 1;
				//	}
				//}
				//else {
				//	if ( ( ( pPrnTbl + cnt )->Index == CHEK03_3 ) || ( ( pPrnTbl + cnt )->Index == CHEK04_3 ) ) {
				//		hide = 1;
				//	}
				//}
				break;
		}

		//if( Flg ){
		//	// 入力可
		//	ChangeColor( IDC_ICSDIAG8CTRL2, (pPrnTbl+cnt)->Index, 1 );
		//	m_Diag2.ModifyItem( (pPrnTbl+cnt)->Index, DIAG_MDFY_EDIT );
		//	SetInputItemData( (pPrnTbl+cnt)->Index );
		//}
		//else{
		//	// 入力不可
		//	ChangeColor( IDC_ICSDIAG8CTRL2, (pPrnTbl+cnt)->Index, 2 );
		//	DIAGRAM_DATA dd;
		//	dd.data_imgdata = NULL;
		//	dd.data_check = 0;
		//	m_Diag2.SetData( (pPrnTbl+cnt)->Index, (LPUNKNOWN)&dd);
		//	GetInputItemData( (pPrnTbl+cnt)->Index );
		//	m_Diag2.ModifyItem( (pPrnTbl+cnt)->Index, DIAG_MDFY_READONLY );

		//	if ( hide ) {
		//		m_Diag2.ModifyItem ( (pPrnTbl + cnt)->Index, DIAG_MDFY_TRANSPARENT );
		//	}
		//}
		if((pPrnTbl+cnt)->Index == COMBO_TEI || (pPrnTbl+cnt)->Index == COMBO_HIK)	{
// 22-0720 del -->
			//// モノクロプリンター
			//if(m_pPrintCmInfo->isColorPrn == 0)	{
			//	// コンボを提出用モノクロ、控用モノクロにして選択不可にする
			//	DIAGRAM_DATA	dd;
			//	int				ii=0;
			//	for(ii=COMBO_TEI; ii<=COMBO_HIK; ii++)	{
			//		m_Diag2.GetData(ii,(LPUNKNOWN)&dd);
			//		dd.data_combo = 1;
			//		m_Diag2.SetData(ii,(LPUNKNOWN)&dd);
			//		ChangeColor(IDC_ICSDIAG8CTRL2,ii,2);
			//		m_Diag2.ModifyItem(ii,DIAG_MDFY_READONLY);
			//	}
			//}
			//else	{
// 22-0720 del <--
				if(Flg)	{
					// 入力可
					ChangeColor(IDC_ICSDIAG8CTRL2,(pPrnTbl+cnt)->Index,1);
					m_Diag2.ModifyItem((pPrnTbl+cnt)->Index,DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					ChangeColor(IDC_ICSDIAG8CTRL2,(pPrnTbl+cnt)->Index,2);
					m_Diag2.ModifyItem((pPrnTbl+cnt)->Index,DIAG_MDFY_READONLY);
				}
				// ＯＣＲ用紙で起動したときにデフォルトのカラーが表示されるのも
				// おかしいので、白紙で選択していたときの状態が表示されるようにする
				SetInputItemData((pPrnTbl+cnt)->Index);
// 22-0720 del -->
			//}
// 22-0720 del <--
		}
		else	{
			for(ii=0; ; ii++)	{
				if(LstBusuTbl2[ii].idx[0] == -1)	break;
				if(LstBusuTbl2[ii].idx[0] == (pPrnTbl+cnt)->Index)	{
					for(jj=0; ; jj++)	{
						if(LstBusuTbl2[ii].idx[jj] == -1)	{
							break;
						}
						flg2=Flg;
						if(!(m_pSnHeadData->Sn_TYPE&0x01))	{
							// ＯＣＲの場合、カラー・モノクロ、部数の指定は不可
							if(jj != 0)	flg2=0;
						}
						//if(m_pPrintCmInfo->isColorPrn == 0)	{
						//	// モノクロプリンターの場合、カラーの指定は不可
						//	if( LstBusuTbl2[ii].idx[jj] == HP_TEI_COLOR_CHK || 
						//		LstBusuTbl2[ii].idx[jj] == HP_TEI_COLOR_BSU || 
						//		LstBusuTbl2[ii].idx[jj] == HP_HIK_COLOR_CHK || 
						//		LstBusuTbl2[ii].idx[jj] == HP_HIK_COLOR_BSU || 
						//		LstBusuTbl2[ii].idx[jj] == KZ_TEI_COLOR_CHK || 
						//		LstBusuTbl2[ii].idx[jj] == KZ_TEI_COLOR_BSU || 
						//		LstBusuTbl2[ii].idx[jj] == KZ_HIK_COLOR_CHK || 
						//		LstBusuTbl2[ii].idx[jj] == KZ_HIK_COLOR_BSU )	{
						//		flg2 = 0;
						//	}
						//}
						if(flg2)	{
							// 入力可
							ChangeColor(IDC_ICSDIAG8CTRL2,LstBusuTbl2[ii].idx[jj],1);
							m_Diag2.ModifyItem(LstBusuTbl2[ii].idx[jj],DIAG_MDFY_EDIT);
							SetInputItemData(LstBusuTbl2[ii].idx[jj]);

// 24/07/10_個別タブから出力 add -->
							// 課税取引金額計算表、特定収入計算表、税務代理書面の部数
							if(LstBusuTbl2[ii].idx[jj] == CHEK12_BUSU || LstBusuTbl2[ii].idx[jj] == CHEK13_BUSU || LstBusuTbl2[ii].idx[jj] == CHEK14_BUSU)	{
								ChangeColor(IDC_ICSDIAG8CTRL2, LstBusuTbl2[ii].idx[jj], 2);
								DIAGRAM_DATA	dd;
								dd.data_imgdata = NULL;
								dd.data_check = 0;
								m_Diag2.SetData(LstBusuTbl2[ii].idx[jj], (LPUNKNOWN)&dd);
								GetInputItemData(LstBusuTbl2[ii].idx[jj]);
								m_Diag2.ModifyItem(LstBusuTbl2[ii].idx[jj], DIAG_MDFY_READONLY);
							}
// 24/07/10_個別タブから出力 add <--
						}
						else	{
							// 入力不可
							ChangeColor(IDC_ICSDIAG8CTRL2,LstBusuTbl2[ii].idx[jj],2);
							DIAGRAM_DATA	dd;
							dd.data_imgdata = NULL;
							dd.data_check = 0;
// 24/07/10_個別タブから出力 cor -->
							//if(IsBusu(LstBusuTbl2[ii].idx[jj]) == TRUE)	{
// ---------------------------------
							if(IsBusu(LstBusuTbl2[ii].idx[jj]) == TRUE &&
								(LstBusuTbl2[ii].idx[jj] != CHEK12_BUSU && LstBusuTbl2[ii].idx[jj] != CHEK13_BUSU && LstBusuTbl2[ii].idx[jj] != CHEK14_BUSU))	{
// 24/07/10_個別タブから出力 cor <--
								dd.data_edit.Format(_T("%d"),m_pPrintCmInfo->busu[GetBusuNo(LstBusuTbl2[ii].idx[jj])]);
							}
							m_Diag2.SetData(LstBusuTbl2[ii].idx[jj],(LPUNKNOWN)&dd);
							GetInputItemData(LstBusuTbl2[ii].idx[jj]);
							m_Diag2.ModifyItem(LstBusuTbl2[ii].idx[jj],DIAG_MDFY_READONLY);
						}
					}
				}
			}
		}
		(pPrnTbl+cnt)->AllUnConnect = Flg;
		cnt++;
	}
	m_Diag2.Refresh();


	//20240312　ボタンの制御
	Button3Control();
	Button4Control();
	Button5Control();

}

//-----------------------------------------------------------------------------
// 入力項目設定（サブ）
//-----------------------------------------------------------------------------
int CPrnPrint_EX2::SetInputItemData( short idx )
{
	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;
	dd.data_check = 0;
	dd.data_combo = 0;
	dd.data_edit = _T("");

	// 白紙
	if(m_pSnHeadData->Sn_TYPE&0x01)	{
		m_Radio1.SetCheck(1);
		m_Radio2.SetCheck(0);
	}
	// ＯＣＲ用紙
	else	{
		m_Radio1.SetCheck(0);
		m_Radio2.SetCheck(1);
	}

	switch( idx ){

		case COMBO_TEI:	// 提出用カラー/モノクロ
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x01)	{
				// カラー
				dd.data_combo = 0;
			}
			else	{
				// モノクロ
				dd.data_combo = 1;
			}
			break;
		case COMBO_HIK:	// 控　用カラー/モノクロ
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x02)	{
				// カラー
				dd.data_combo = 0;
			}
			else	{
				// モノクロ
				dd.data_combo = 1;
			}
			break;

		case CHEK01_1:	// 申告書本表
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x04)	{
				dd.data_check = 1;
			}
			break;
		case HP_TEI_CHK:			// 提出用	チェック
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x08)	{
				dd.data_check = 1;
			}
			break;
		case HP_HIK_CHK:			// 控　用	チェック
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x10)	{
				dd.data_check = 1;
			}
			break;

		case CHEK01_2:	// 課税標準額等の内訳書
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x20)	{
				dd.data_check = 1;
			}
			break;	
		case KZ_TEI_CHK:			// 提出用	チェック
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x40)	{
				dd.data_check = 1;
			}
			break;
		case KZ_HIK_CHK:			// 控　用	チェック
			if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x80)	{
				dd.data_check = 1;
			}
			break;

		case CHEK03_1:	// 付表２－１
			//if(m_isNoneMTForm)	{
			//	dd.data_check = 0;
			//}
			//else	{
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x40)	{
					dd.data_check = 1;
				}
			//}
			break;
		case CHEK03_2:	// 付表２－２
			//if(m_isNoneMTForm)	{
			//	dd.data_check = 0;
			//}
			//else	{
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x80)	{
					dd.data_check = 1;
				}
			//}
			break;
		case CHEK03_3:	// 付表２－３
			//if(m_isNoneMTForm == FALSE)	{
			//	dd.data_check = 0;
			//}
			//else	{
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x8000000)	{
					dd.data_check = 1;
				}
			//}
			break;
		case CHEK04_1:	// 付表１－１
			//if(m_isNoneMTForm)	{
			//	dd.data_check = 0;
			//}
			//else	{
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x10)	{
					dd.data_check = 1;
				}
			//}
			break;
		case CHEK04_2:	// 付表１－２
			//if(m_isNoneMTForm)	{
			//	dd.data_check = 0;
			//}
			//else	{
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x20)	{
					dd.data_check = 1;
				}
			//}
			break;
		case CHEK04_3:	// 付表１－３
			//if(m_isNoneMTForm == FALSE)	{
			//	dd.data_check = 0;
			//}
			//else	{
				if(m_pSnHeadData->Sn_PrintOutSgn & 0x4000000)	{
					dd.data_check = 1;
				}
			//}
			break;
		case CHEK06:	// 第２６号様式
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x800)	{
				dd.data_check = 1;	
			}
			break;
		case CHEK09:	// 消費税の還付申告に関する明細書（表面）
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x2000)	{
				dd.data_check = 1;
			}
			break;
		case CHEK10:	// 消費税の還付申告に関する明細書（裏面）
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x4000)	{
				dd.data_check = 1;	
			}
			break;
		case CHEK11:	// 更正の請求書
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x8000)	{
				dd.data_check = 1;
			}
			break;
		//case CHEK12:	// 課税取引金額計算表　事業所得用
		//	if(m_pSnHeadData->Sn_PrintOutSgn & 0x200000)	{
		//		dd.data_check = 1;
		//	}
		//	break;
		//case CHEK13:	// 課税取引金額計算表　農業所得用 
		//	if(m_pSnHeadData->Sn_PrintOutSgn & 0x800000)	{
		//		dd.data_check = 1;
		//	}
		//	break;
		//case CHEK14:	// 課税取引金額計算表　不動産所得用
		//	if(m_pSnHeadData->Sn_PrintOutSgn & 0x400000)	{
		//		dd.data_check = 1;
		//	}
		//	break;
		case CHEK12:	// 課税取引金額計算表
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x40000000)	{
				dd.data_check = 1;
			}
			break;
		case CHEK14:	// 税務代理書面
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x80000000)	{
				dd.data_check = 1;
			}
			break;
		case CHEK13:	// 特定収入計算表
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x1000000)	{
				dd.data_check = 1;
			}
			break;
		case CHEK18:	// 第２６号様式Ａ４ヨコ控用
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x1000)	{
				dd.data_check = 1;
			}
			break;
		case CHEK25:	// 付表６（税率別消費税額計算表）
			if (m_pSnHeadData->Sn_PrintOutSgn & 0x20000000) {
				dd.data_check = 1;
			}
			break;
		case CHEK20:	// 付表６・付表７
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x400)	{
				dd.data_check = 1;
			}
			break;
		case CHEK22:	// 特例１
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x10000)	{
				dd.data_check = 1;
			}
			break;
		case CHEK23:	// 特例２
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x20000)	{
				dd.data_check = 1;
			}
			break;
		case CHEK24:	// 特例３
			if(m_pSnHeadData->Sn_PrintOutSgn & 0x40000)	{
				dd.data_check = 1;
			}
			break;

		// 部数
		case HP_TEI_BSU:
		case HP_HIK_BSU:
		case KZ_TEI_BSU:
		case KZ_HIK_BSU:
		case CHEK04_1_BUSU:
		case CHEK04_2_BUSU:
		case CHEK04_3_BUSU:
		case CHEK03_1_BUSU:
		case CHEK03_2_BUSU:
		case CHEK03_3_BUSU:
		case CHEK25_BUSU:
		case CHEK20_BUSU:
		case CHEK06_BUSU:
		case CHEK18_BUSU:
		case CHEK09_BUSU:
		case CHEK10_BUSU:
		case CHEK11_BUSU:
		case CHEK22_BUSU:
		case CHEK23_BUSU:
		case CHEK24_BUSU:
// 24/07/10_個別タブから出力 del -->
		//case CHEK12_BUSU:
		//case CHEK13_BUSU:
		//case CHEK14_BUSU:
// 24/07/10_個別タブから出力 del <--
		//case CHEK15_BUSU:
		//case CHEK16_BUSU:
		//case CHEK17_BUSU:
			dd.data_edit.Format(_T("%d"),m_pPrintCmInfo->busu[GetBusuNo(idx)]);
			break;

		default:
			break;
	}
	m_Diag2.SetData(idx,(LPUNKNOWN)&dd);

	return dd.data_check;
}

//-----------------------------------------------------------------------------
// 入力項目取得
//-----------------------------------------------------------------------------
// 引数	idx	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CPrnPrint_EX2::GetInputItemData( short idx )
{
	DIAGRAM_DATA	dd;
	DIAGRAM_DATA	dd2;
	dd.data_imgdata = NULL;
	m_Diag2.GetData(idx,(LPUNKNOWN)&dd);

	switch( idx )	{

		case CHEK01_1:	// 申告書本表
			m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x04;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x04;
			}
			break;
		case HP_TEI_CHK:	// 提出用	チェック
			m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x08;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x08;
			}
			break;
		case HP_HIK_CHK:	// 控　用	チェック
			m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x10;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x10;
			}
			break;

		case CHEK01_2:	// 課税標準額等の内訳書
			m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x20;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x20;
			}
			break;
		case KZ_TEI_CHK:	// 提出用	チェック
			m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x40;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x40;
			}
			break;
		case KZ_HIK_CHK:	// 控　用	チェック
			m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x80;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x80;
			}
			break;

		case CHEK03_1:	// 付表２－１
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x40;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x40;
			}
			break;
		case CHEK03_2:	// 付表２－２
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x80;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x80;
			}
			break;
		case CHEK03_3:	// 付表２－３
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x8000000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x8000000;
			}
			break;
		case CHEK04_1:	// 付表１－１
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x10;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x10;
			}
			break;
		case CHEK04_2:	// 付表１－２
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x20;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x20;
			}
			break;
		case CHEK04_3:	// 付表１－３
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x4000000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x4000000;
			}
			break;
		case CHEK06:	// 第２６号様式
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x800;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x800;
			}
			break;
		case CHEK09:	// 還付申告に関する明細書（表面）
			if(m_pSnHeadData->IsUseTaxRefund() == FALSE)	{
				break;	
			}
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x2000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x2000;
			}
			break;
		case CHEK10:	// 還付申告に関する明細書（裏面）
			if(m_pSnHeadData->IsUseTaxRefund() == FALSE)	{
				break;
			}
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x4000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x4000;
			}
		 	break;
		case CHEK11:	// 更正の請求書
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x8000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x8000;
			}
			break;
		//case CHEK12:	// 課税取引金額計算表　事業所得用
		//	m_pSnHeadData->Sn_PrintOutSgn &= ~0x200000;
		//	if(dd.data_check)	{
		//		m_pSnHeadData->Sn_PrintOutSgn |= 0x200000;
		//	}
		//	break;
		//case CHEK13:	// 課税取引金額計算表　農業所得用
		//	m_pSnHeadData->Sn_PrintOutSgn &= ~0x800000;
		//	if(dd.data_check)	{
		//		m_pSnHeadData->Sn_PrintOutSgn |= 0x800000;
		//	}
		// 	break;
		//case CHEK14:	// 課税取引金額計算表　不動産所得用
		//	m_pSnHeadData->Sn_PrintOutSgn &= ~0x400000;
		//	if(dd.data_check)	{
		//		m_pSnHeadData->Sn_PrintOutSgn |= 0x400000;
		//	}
		// 	break;
		case CHEK12:	// 課税取引金額計算表
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x40000000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x40000000;
			}
		 	break;
		case CHEK14:	// 税務代理書面
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x80000000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x80000000;
			}
		 	break;
		case CHEK13:	// 特定収入計算表
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x1000000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x1000000;
			}
			break;
		case CHEK18:	// 第２６号様式Ａ４ヨコ控用
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x1000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x1000;
			}
			break;
		case CHEK25:	// 付表６（税率別消費税額計算表）
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x20000000;
			if (dd.data_check) {
				m_pSnHeadData->Sn_PrintOutSgn |= 0x20000000;
			}
			break;
		case CHEK20:	// 付表６・付表７
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x400;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x400;
			}
			break;
		case CHEK22:	// 特例１
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x10000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x10000;
			}
			break;
		case CHEK23:	//　特例２
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x20000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x20000;
			}
			break;
		case CHEK24:	// 特例３
			m_pSnHeadData->Sn_PrintOutSgn &= ~0x40000;
			if(dd.data_check)	{
				m_pSnHeadData->Sn_PrintOutSgn |= 0x40000;
			}
			break;

		// 部数
		case HP_TEI_BSU:
		case HP_HIK_BSU:
		case KZ_TEI_BSU:
		case KZ_HIK_BSU:
		case CHEK04_1_BUSU:
		case CHEK04_2_BUSU:
		case CHEK04_3_BUSU:
		case CHEK03_1_BUSU:
		case CHEK03_2_BUSU:
		case CHEK03_3_BUSU:
		case CHEK25_BUSU:
		case CHEK20_BUSU:
		case CHEK06_BUSU:
		case CHEK18_BUSU:
		case CHEK09_BUSU:
		case CHEK10_BUSU:
		case CHEK11_BUSU:
		case CHEK22_BUSU:
		case CHEK23_BUSU:
		case CHEK24_BUSU:
// 24/07/10_個別タブから出力 del -->
		//case CHEK12_BUSU:
		//case CHEK13_BUSU:
		//case CHEK14_BUSU:
// 24/07/10_個別タブから出力 del <--
		//case CHEK15_BUSU:
		//case CHEK16_BUSU:
		//case CHEK17_BUSU:
// 160117 add -->
			// 1-99以外は受け付けないようにする
			if(_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99)	{
				dd.data_edit=_T("1");
				m_Diag2.SetData(idx,(LPUNKNOWN)&dd);
			}
// 160117 add <--
// 160122 del -->
			//if(IsCheckOn(idx) == TRUE)	{
// 160122 del <--
				m_pPrintCmInfo->busu[GetBusuNo(idx)] = _tstoi(dd.data_edit);
// 160122 del -->
			//}
// 160122 del <--
			break;

		default:
			break;
	}

	return dd.data_check;
}

//-----------------------------------------------------------------------------
// 印刷用情報のセット
//-----------------------------------------------------------------------------
//void CPrnPrint_EX2::SetPrintInfo()		// 24/07/10_個別タブから出力 del
void CPrnPrint_EX2::SetPrintInfo(int seltb)	// 24/07/10_個別タブから出力 add
{
	// 初期設定
	char	work[6] = {0};
	memset( m_pPrintCmInfo->PRtable, '\0', sizeof(m_pPrintCmInfo->PRtable) );
	m_pPRtableEx->RemoveAll();
	m_pPGtable->RemoveAll();
	m_pPRtableEx1->RemoveAll();
	m_pPrintCmInfo->PRtype = 0x00;

	BOOL syzSign = FALSE;
	long eymd = 0;

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

	if( m_pSnHeadData->Sn_TYPE&0x01 ){
		m_pPrintCmInfo->PRtype = (char)0x80;
	}

	SH_ITEMINDEX	*pPrnTbl;

	if( m_pSnHeadData->IsMiddleProvisional() ){
		pPrnTbl = &PrnTbl1[0];
	}
	else{
		if( !(m_pSnHeadData->Sn_TYPE&0x01) ){
			pPrnTbl = &PrnTbl2[0];
		}
		else{
			pPrnTbl = &PrnTbl3[0];
		}
	}

	int cnt = 0;
	int	max=GetMaxBusu();
	int	cur=0;
	for(cur=1; cur<=max; cur++)	{
		cnt=0;
		while( 1 ){
			if( (pPrnTbl+cnt)->Index == 0 ){
				break;
			}

			int Flg = 0;
			if( m_pSnHeadData->SVmzsw ){
				Flg = (pPrnTbl+cnt)->UnConnect;
			}
			else{
				Flg = (pPrnTbl+cnt)->Connect;
			}
		
// 24/07/10_個別タブから出力 add -->
			// 【引数】	seltb：0x00=通常(出力タブで選択されたもの)、0x01=課税取引金額計算表タブ、0x02=特定収入計算表タブ、0x04=税務代理書面タブ
			// Flg	6=課税取引金額計算表、7=特定収入計算表、8=税務代理書面
			if(seltb != 0x00)	{
				int	sw = 0;
				if(seltb == 0x01 && Flg != 6)		sw = 1;
				else if(seltb == 0x02 && Flg != 7)	sw = 1;
				else if(seltb == 0x04 && Flg != 8)	sw = 1;

				if(sw == 1)	{
					cnt++;
					continue;
				}
			}
// 24/07/10_個別タブから出力 add <--

			switch( Flg ){
				case 1 :	// 本表・付表・第26号様式
					Flg = 1;
					//if ( ( ( pPrnTbl + cnt )->Index == CHEK04_1 ) || ( ( pPrnTbl + cnt )->Index == CHEK03_1 )
					//	|| ( ( pPrnTbl + cnt )->Index == CHEK04_2 ) || ( ( pPrnTbl + cnt )->Index == CHEK03_2 ) ) {
					//	if ( m_isNoneMTForm ) {
					//		Flg = 0;
					//	}
					//}
					//else if ( ( ( pPrnTbl + cnt )->Index == CHEK04_3 ) || ( ( pPrnTbl + cnt )->Index == CHEK03_3 ) ) {
					//	if ( m_isNoneMTForm == FALSE ) {
					//		Flg = 0;
					//	}
					//}
					break;
				case 2 :	// 仕入控除税額に関する明細書
					if( m_pSnHeadData->IsUseTaxRefund() ){
						Flg = 0;
					}
					else{
						Flg = 1;
					}
					break;
				case 3 :	// 仕入控除税額に関する明細書（次葉）
					if( m_pSnHeadData->IsUseTaxRefund() ){
						Flg = 0;
					}
					else{
						ASSERT( !_T("有り得ない！") );
					}
					break;
				case 4 :	// 消費税の還付申告に関する明細書
					if( m_pSnHeadData->IsUseTaxRefund() ){
						Flg = 1;
					}
					else{
						Flg = 0;
					}
					break;
				case 5 :	// 更正の請求書
					if( m_pSnHeadData->Sn_Syuusei & 0x02 ){
						Flg = 1;
					}
					else{
						Flg = 0;
					}
					break;

				case 6:	// 課税取引金額計算表
					if(m_pSnHeadData->IsSoleProprietor() != FALSE)	{
						Flg = 1;
					}
					else	{
						Flg = 0;
					}
					break;

				case 7:	// 特定収入計算表
					Flg = 0;
					break;

				case 8:	// 税務代理書面
// 24/03/18_税務代理書面 cor -->
					//Flg = 1;
// -----------------------------
// 24/03/26_税務代理書面 cor -->
					//if(m_Eymd >= R06_ZEIMUDAIRI)	Flg = 1;
					//else							Flg = 0;
// -----------------------------
// 24/04/01_168704 cor -->
					//Flg = ((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud();
// -----------------------
					if(m_pParent == NULL)	{
						Flg = 1;
					}
					else	{
// 24/07/10_個別タブから出力 cor -->
						//Flg = ((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud();
// ---------------------------------
						if( ((CPrnReportDlg*)m_pParent)->ZeitempEnableChk() > 0 &&
							((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud() == TRUE) {
							Flg = 1;
						}
						else {
							Flg = 0;
						}
// 24/07/10_個別タブから出力 cor <--
					}
// 24/04/01_168704 cor <--
// 24/03/26_税務代理書面 cor <--
// 24/03/18_税務代理書面 cor <--
					break;

				//case 6 :	// 課税取引金額計算表　事業所得用
				//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
				//		Flg = 0;
				//	}
				//	else {
				//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
				//			Flg = 0;	// 事業別売上高
				//		}
				//		else {
				//			if (m_pZmSub->zvol->ind_type & 0x01) {
				//				Flg = 1;
				//			}
				//			else {
				//				Flg = 0;
				//			}
				//		}
				//	}
				//	break;
				//case 7 :	// 課税取引金額計算表　農業所得用
				//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
				//		Flg = 0;
				//	}
				//	else {
				//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
				//			Flg = 0;	// 事業別売上高
				//		}
				//		else {
				//			if (m_pZmSub->zvol->ind_type & 0x04) {
				//				Flg = 1;
				//			}
				//			else {
				//				Flg = 0;
				//			}
				//		}
				//	}
				//	break;
				//case 8 :	// 課税取引金額計算表　不動産所得用
				//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
				//		Flg = 0;
				//	}
				//	else {
				//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
				//			Flg = 0;	// 事業別売上高
				//		}
				//		else {
				//			if (m_pZmSub->zvol->ind_type & 0x02) {
				//				Flg = 1;
				//			}
				//			else {
				//				Flg = 0;
				//			}
				//		}
				//	}
				//	break;
				//case 9 :	// 課税売上高計算表
				//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
				//		Flg = 0;
				//	}
				//	else {
				//		Flg = 1;
				//	}
				//	break;
				//case 10:	// 課税仕入高計算表
				//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
				//		Flg = 0;
				//	}
				//	else {
				//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
				//			Flg = 0;	// 事業別売上高[13'09.19]
				//		}
				//		else {
				//			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				//				Flg = 1;
				//			}
				//			else {
				//				Flg = 0;
				//			}
				//		}
				//	}
				//	break;
				//case 11:	// 特定収入計算表
				//	if( *m_pShinInfo->pSgSpc ){
				//		if( m_PrselState == ID_ALL_PRINT_SEL_STATE ){
				//			Flg = 1;
				//		}
				//		else{
				//			if( m_pSnHeadData->Sn_Sign2 & 0x02 ){
				//				Flg = 0;
				//			}
				//			else{
				//				Flg = 1;
				//			}
				//		}
				//	}
				//	else{
				//		Flg = 0;
				//	}
				//	break;
				//case 12:	// 事業別売上高
				//	if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
				//		Flg = 0;
				//	}
				//	else{
				//		if( m_Util.IsUnConnectKaniMst( m_pZmSub->zvol) ){
				//			Flg = 1;	// 事業別売上高
				//		}
				//		else{
				//			Flg = 0;
				//		}
				//	}
				//	break;

				case 13:	// 特定課税仕入れがある場合の課税標準額等の内訳書()
					if( m_pPrintCmInfo->Shrevtype < ID_ICSSH_TKKZSIIRE_REVTYPE ){
						Flg = 0;
					}
					break;
				case 14:	//付表６
					if((m_pSnHeadData->IsSoleProprietor())&&(m_pSnHeadData->Sn_Syuusei&0x10)){
						Flg = 1;
					}else{
						Flg = 0;
					}
					break;
				case 15:
					if ( ( m_pSnHeadData->m_DispTabSgn & 0x80 ) &&  ( m_pSnHeadData->m_DispTabSgn & 0x10 ) ) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
					break;
				case 16:
					if ( ( m_pSnHeadData->m_DispTabSgn & 0x80 ) && ( m_pSnHeadData->m_DispTabSgn & 0x20 ) ) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
					break;
				case 17:
					if ( ( m_pSnHeadData->m_DispTabSgn & 0x80 ) && ( m_pSnHeadData->m_DispTabSgn & 0x40 ) ) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
					break;
				case 18:	// 付表６（税率別消費税額計算表）
					Flg = 1;
					break;
				default:	// 入力不可
					Flg = 0;
					break;
			}

			char	moji[64]={0};
			CArithEx	Arith;
			Arith.l_defn( 0x16 );

			if(Flg)	{
				// 入力可
				switch((pPrnTbl+cnt)->Index)	{

					case CHEK01_1:	// 申告書本表
						if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x04)	{
							// 白紙
							if(m_pSnHeadData->Sn_TYPE & 0x01)	{
								// 提出用カラー
// 22-0720 del -->
								//if(m_pPrintCmInfo->isColorPrn != 0)	{
// 22-0720 del <--
									if( (m_pSnHeadData->Sn_PrintOutSgn2 & 0x01) && 
										(m_pSnHeadData->Sn_PrintOutSgn2 & 0x08) )	{
										//m_pPrintCmInfo->PRtable[Put] = 0x01;
										BusuCheckAndPut(0x01,cur);
									}
// 22-0720 del -->
								//}
// 22-0720 del <--
								// 提出用モノクロ
								if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x01) && 
									(m_pSnHeadData->Sn_PrintOutSgn2 & 0x08) )	{
									//m_pPrintCmInfo->PRtable[Put] = 0x03;
									BusuCheckAndPut(0x03,cur);
								}
								// 控用カラー
// 22-0720 del -->
								//if(m_pPrintCmInfo->isColorPrn != 0)	{
// 22-0720 del <--
									if( (m_pSnHeadData->Sn_PrintOutSgn2 & 0x02) && 
										(m_pSnHeadData->Sn_PrintOutSgn2 & 0x10) )	{
										//m_pPrintCmInfo->PRtable[Put] = 0x02;
										BusuCheckAndPut(0x02,cur);
									}
// 22-0720 del -->
								//}
// 22-0720 del <--
								// 控用モノクロ
								if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x02) && 
									(m_pSnHeadData->Sn_PrintOutSgn2 & 0x10) )	{
									//m_pPrintCmInfo->PRtable[Put] = 0x04;
									BusuCheckAndPut(0x04,cur);
								}
							}
							// ＯＣＲ
							else	{
								//m_pPrintCmInfo->PRtable[Put] = 0x01;
								BusuCheckAndPut(0x01,cur);
							}
						}
						break;	

					case CHEK01_2:	// 特定課税仕入れがある場合の課税標準額等の内訳書
						if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x20)	{
							// 白紙
							if(m_pSnHeadData->Sn_TYPE & 0x01)	{
								// 提出用カラー
// 22-0720 del -->
								//if(m_pPrintCmInfo->isColorPrn != 0)	{
// 22-0720 del <--
									if( (m_pSnHeadData->Sn_PrintOutSgn2 & 0x01) && 
										(m_pSnHeadData->Sn_PrintOutSgn2 & 0x40) )	{
										//m_pPrintCmInfo->PRtable[Put] = 0x57;
										BusuCheckAndPut(0x57,cur);
									}
// 22-0720 del -->
								//}
// 22-0720 del <--
								// 提出用モノクロ
								if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x01) && 
									(m_pSnHeadData->Sn_PrintOutSgn2 & 0x40))	{
									//m_pPrintCmInfo->PRtable[Put] = 0x05;
									BusuCheckAndPut(0x05,cur);
								}
								// 控用カラー
// 22-0720 del -->
								//if(m_pPrintCmInfo->isColorPrn != 0)	{
// 22-0720 del <--
									if( (m_pSnHeadData->Sn_PrintOutSgn2 & 0x02) && 
										(m_pSnHeadData->Sn_PrintOutSgn2 & 0x80))	{
										//m_pPrintCmInfo->PRtable[Put] = 0x58;
										BusuCheckAndPut(0x58,cur);
									}
// 22-0720 del -->
								//}
// 22-0720 del <--
								// 控用モノクロ
								if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x02) && 
									(m_pSnHeadData->Sn_PrintOutSgn2 & 0x80))	{
									//m_pPrintCmInfo->PRtable[Put] = 0x06;
									BusuCheckAndPut(0x06,cur);
								}
							}
							// ＯＣＲ
							else	{
								//m_pPrintCmInfo->PRtable[Put] = 0x57;
								BusuCheckAndPut(0x57,cur);
							}
						}
						break;	

					case CHEK03_1:	// 付表２－１
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x40)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x72;
							BusuCheckAndPut(0x72,cur);
						}
						break;
					case CHEK03_2:	// 付表２－２
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x80)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x73;
							BusuCheckAndPut(0x73,cur);
						}
						break;
					case CHEK03_3:	// 付表２－３
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x8000000)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x18;
							BusuCheckAndPut(0x18,cur);
						}
						break;
					case CHEK04_1:	// 付表１－１
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x10)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x70;
							BusuCheckAndPut(0x70,cur);
						}
						break;
					case CHEK04_2:	// 付表１－２
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x20)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x71;
							BusuCheckAndPut(0x71,cur);
						}
						break;
					case CHEK04_3:	// 付表１－３
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x4000000)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x17;
							BusuCheckAndPut(0x17,cur);
						}
						break;
					case CHEK06:	// 第２６号様式
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x800)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x50;
							BusuCheckAndPut(0x50,cur);
						}
						break;
					case CHEK09:	// 消費税の還付申告に関する明細書（表面）
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x2000)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x07;
							BusuCheckAndPut(0x07,cur);
						}
						break;
					case CHEK10:	// 消費税の還付申告に関する明細書（裏面）
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x4000)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x08;
							BusuCheckAndPut(0x08,cur);
						}
						break;
					case CHEK11:	// 更正の請求書
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x8000)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x09;
							BusuCheckAndPut(0x09,cur);
						}
						break;
					//case CHEK12:	// 課税取引金額計算表　事業所得用
					//	if(m_pSnHeadData->Sn_PrintOutSgn & 0x200000)	{
					//		//m_pPrintCmInfo->PRtable[Put] = 0x12;
					//		BusuCheckAndPut(0x12,cur);
					//	}
					//	break;
					//case CHEK13:	// 課税取引金額計算表　農業所得用  
					//	if(m_pSnHeadData->Sn_PrintOutSgn & 0x800000)	{
					//		//m_pPrintCmInfo->PRtable[Put] = 0x14;
					//		BusuCheckAndPut(0x14,cur);
					//	}
					//	break;
					//case CHEK14:	// 課税取引金額計算表　不動産所得用 
					//	if(m_pSnHeadData->Sn_PrintOutSgn & 0x400000)	{
					//		//m_pPrintCmInfo->PRtable[Put] = 0x13;
					//		BusuCheckAndPut(0x13,cur);
					//	}
					//	break;
					case CHEK12:	// 課税取引金額計算表
						//if(m_pSnHeadData->Sn_PrintOutSgn & 0x40000000)	{					// 24/07/10_個別タブから出力 del
						if((m_pSnHeadData->Sn_PrintOutSgn & 0x40000000) || (seltb & 0x01))	{	// 24/07/10_個別タブから出力 add
							//m_pPrintCmInfo->PRtable[Put] = 0x10;
							//BusuCheckAndPut(0x10,cur);
							if(!m_Util.IsUnConnectKaniMst(m_pZmSub->zvol))	{
								if(m_pZmSub->zvol->ind_type & 0x01)	{
									// 課税取引金額計算表　事業所得用 
									BusuCheckAndPut3(0x12,cur);
								}
								if(m_pZmSub->zvol->ind_type & 0x04)	{
									// 課税取引金額計算表　農業所得用 
									BusuCheckAndPut3(0x14,cur);
								}
								//if(m_pZmSub->zvol->ind_type & 0x04)	{	// 修正No.168687 del
								if(m_pZmSub->zvol->ind_type & 0x02)	{		// 修正No.168687 add
									// 課税取引金額計算表　不動産所得用 
									BusuCheckAndPut3(0x13,cur);
								}
							}
							// 課税売上高計算表
							BusuCheckAndPut3(0x10,cur);
							if(!m_Util.IsUnConnectKaniMst(m_pZmSub->zvol))	{
								if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
									// 課税仕入高計算表
									BusuCheckAndPut3(0x11,cur);
								}
							}
						}
						break;
					case CHEK14:	// 税務代理書面
						//if(m_pSnHeadData->Sn_PrintOutSgn & 0x80000000)	{					// 24/07/10_個別タブから出力 del
						if((m_pSnHeadData->Sn_PrintOutSgn & 0x80000000) || (seltb & 0x04))	{	// 24/07/10_個別タブから出力 add
// 24/03/18_税務代理書面 cor -->
							////m_pPrintCmInfo->PRtable[Put] = 0x11;
							//BusuCheckAndPut(0x11,cur);
// -----------------------------
							// 税務代理権限証書
							BusuCheckAndPut4(0x80,cur);
							// 税理士法第33条の2第1項に規定する添付書面（第1面
							BusuCheckAndPut4(0x81,cur);
							// 税理士法第33条の2第1項に規定する添付書面（第2面）
							BusuCheckAndPut4(0x82,cur);
							// 税理士法第33条の2第1項に規定する添付書面（第3面）
							BusuCheckAndPut4(0x83,cur);
							// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
							BusuCheckAndPut4(0x84,cur);
							// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
							BusuCheckAndPut4(0x85,cur);
							// 税理士法第33条の2第2項に規定する添付書面（第1面）
							BusuCheckAndPut4(0x86,cur);
							// 税理士法第33条の2第2項に規定する添付書面（第2面）
							BusuCheckAndPut4(0x87,cur);
							// 税理士法第33条の2第2項に規定する添付書面（第3面）
							BusuCheckAndPut4(0x88,cur);
							// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
							BusuCheckAndPut4(0x89,cur);
							// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
							BusuCheckAndPut4(0x90,cur);
// 24/03/18_税務代理書面 cor <--
						}
						break;
					case CHEK13:	// 特定収入計算書
						//if(m_pSnHeadData->Sn_PrintOutSgn & 0x1000000)	{						// 24/07/10_個別タブから出力 del
						if((m_pSnHeadData->Sn_PrintOutSgn & 0x1000000) || (seltb & 0x02))	{	// 24/07/10_個別タブから出力 add
							// 計算表１
							//m_pPrintCmInfo->PRtable[Put] = 0x40;
							BusuCheckAndPut(0x40,cur);
							// 計算表２（１）
							CTaxationList31View *pTaxlist31View = NULL;
							pTaxlist31View = ( CTaxationList31View* ) ( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 4 ) );
							if ( pTaxlist31View->GetMxList21_31() ){
//								m_pPrintCmInfo->SpcTp1 = Put+1;
								m_pPrintCmInfo->SpcMx1 = pTaxlist31View->GetMxList21_31();
								for ( int i = 0; i != m_pPrintCmInfo->SpcMx1; i++ ) {
									//m_pPrintCmInfo->PRtable[Put] = 0x41;
									BusuCheckAndPut(0x41,cur,i);
								}
							}

							if ( m_pSnHeadData->IsKobetuSiireAnbun() ) {
								if ( !pTaxlist31View->IsRatioOver95() || m_pSnHeadData->IsUseSemiUriRatio() ) {
									if( syzSign ) {
										// 計算表２（２）表面
										if ( pTaxlist31View->GetMxList22_31() ){
//											m_pPrintCmInfo->SpcTp2_5per = Put+1;
											m_pPrintCmInfo->SpcMx2_5per = pTaxlist31View->GetMxList22_31();
											for ( int i = 0; i != m_pPrintCmInfo->SpcMx2_5per; i++ ) {
												//m_pPrintCmInfo->PRtable[Put] = 0x42;
												BusuCheckAndPut(0x42,cur,i);
											}
											pTaxlist31View->MakePageManage22_31();
										}
									}
								
									// 計算表２（２）裏面
									if ( pTaxlist31View->GetMxList22_2_31() ){
//										m_pPrintCmInfo->SpcTp2_8per = Put+1;
										m_pPrintCmInfo->SpcMx2_8per = pTaxlist31View->GetMxList22_2_31();
										for ( int i = 0; i != m_pPrintCmInfo->SpcMx2_8per; i++ ) {
											//m_pPrintCmInfo->PRtable[Put] = 0x56;
											BusuCheckAndPut(0x56,cur,i);
										}
										pTaxlist31View->MakePageManage22_2_31();
									}
								}
							}

							// 計算表３４
							//m_pPrintCmInfo->PRtable[Put] = 0x60;
							BusuCheckAndPut(0x60,cur);

							// 計算表５（１）（２）（３）
							//m_pPrintCmInfo->PRtable[Put] = 0x61;
							BusuCheckAndPut(0x61,cur);
						}
						break;
					case CHEK18:	// 第２６号様式Ａ4ヨコ控用
						if(m_pSnHeadData->Sn_PrintOutSgn & 0x1000)	{
							//m_pPrintCmInfo->PRtable[Put] = 0x51;
							BusuCheckAndPut(0x51,cur);
						}
						break;
					case CHEK25: // 付表６（税率別消費税額計算表）
						if (m_pSnHeadData->Sn_PrintOutSgn & 0x20000000) {
							//m_pPrintCmInfo->PRtable[Put] = 0x79;
							BusuCheckAndPut(0x79, cur);
						}
						break;
					case CHEK20:	// 付表６・付表７
						if(m_pSnHeadData->Sn_Syuusei & 0x10)	{
							CH31HyoView			*pH31View = NULL;
							CH26HyoView			*pH26View = NULL;
							if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
								pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
							}
							else {
								pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(2));
							}
							if( m_pSnHeadData->Sn_PrintOutSgn&0x400 ){
								//m_pPrintCmInfo->Fhy6Tp = Put+1;
								if (pH31View) {
									m_pPrintCmInfo->Fhy6Mx = pH31View->GetMxListFhyo6();
								}
								else {
									m_pPrintCmInfo->Fhy6Mx = pH26View->GetMxListFhyo60();
								}
								for( int i=0; i!=m_pPrintCmInfo->Fhy6Mx; i++ ){
									//m_pPrintCmInfo->PRtable[Put] = 0x16;
									BusuCheckAndPut(0x16,cur,i);
								}
							}
						}
						break;
					case CHEK22: //特例１
					case CHEK23: //特例２
					case CHEK24: //特例３
						if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
							CH31HyoView	*pH31View = NULL;
							pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
							int idx = (pPrnTbl + cnt)->Index - CHEK22;
							int Sign[3] = { m_pSnHeadData->Sn_PrintOutSgn&0x10000, 
											m_pSnHeadData->Sn_PrintOutSgn&0x20000,
											m_pSnHeadData->Sn_PrintOutSgn&0x40000 };
							int* pTokurei_Tp[3] = { &m_pPrintCmInfo->Tokurei_Uri1Tp, 
													&m_pPrintCmInfo->Tokurei_Uri2Tp,
													&m_pPrintCmInfo->Tokurei_ShiireTp};
							int* pTokurei_Mx[3] = { &m_pPrintCmInfo->Tokurei_Uri1Mx,
													&m_pPrintCmInfo->Tokurei_Uri2Mx,
													&m_pPrintCmInfo->Tokurei_ShiireMx };
							unsigned char Table[3] = { 0x76, 0x77, 0x78 };
							if (pH31View) {
								if ( Sign[idx] ) {
//									*pTokurei_Tp[idx] = Put+1;
									*pTokurei_Mx[idx] = pH31View->GetPageNumTokurei(idx);
									for (int i = 0; i != *pTokurei_Mx[idx]; i++) {
										//m_pPrintCmInfo->PRtable[Put] = Table[idx];
										BusuCheckAndPut(Table[idx],cur,i);
									}
								}
							}
						}
						break;

					default: 
						break;
				}
			}
			cnt++;
		}
		if(cur == 1)	{
			m_pPrintCmInfo->PgMx2 = m_pPRtableEx->GetCount();
		}
	}

	m_pPrintCmInfo->PgMx = m_pPRtableEx->GetCount();
}

BEGIN_EVENTSINK_MAP(CPrnPrint_EX2, ICSDialog)
ON_EVENT(CPrnPrint_EX2, IDC_ICSDIAG8CTRL2, 2, CPrnPrint_EX2::EditOFFIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CPrnPrint_EX2, IDC_ICSDIAG8CTRL2, 1, CPrnPrint_EX2::EditONIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CPrnPrint_EX2, IDC_ICSDIAG8CTRL2, 3, CPrnPrint_EX2::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CPrnPrint_EX2, IDC_ICSDIAG8CTRL2, 9, CPrnPrint_EX2::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
ON_EVENT(CPrnPrint_EX2, IDC_ICSDIAG8CTRL2, 8, CPrnPrint_EX2::ComboSelIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CPrnPrint_EX2::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	GetInputItemData( index );
}

void CPrnPrint_EX2::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CPrnPrint_EX2::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	// キーの動作は6種類
	// Tab、Enter		Z順に移動
	// Shift+Tab、F2	Z順の逆に移動
	// →				右方向のみ移動
	// ←				左方向のみ移動
	// ↓				グループ内で下方向のみ移動
	// ↑				グループ内で上方向のみ移動
	int		xx=0,yy=0;
	short	sw=0;
	short	wTerm=0;
	char	VK_FLG=0;

	VK_FLG = (char)::GetKeyState(VK_SHIFT);
	if(nChar == VK_TAB)	{
		if(VK_FLG & 0x80)		wTerm = VK_F2;
		else					wTerm = VK_RETURN;
	}
	else						wTerm = nChar;

	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	if(index == COMBO_TEI)	{

		m_Diag2.GetData(index,(LPUNKNOWN)&dd);
		ComboSelIcsdiag8ctrl2Ex(index,&dd);

		// 最終のインデックスに移動させる
		if(wTerm == VK_F2)	{
			// インデックスを申告書本表のチェックに書き換える
			index=CHEK01_1;
		}
		//else if(wTerm == VK_RETURN || wTerm == VK_RIGHT)	{
		// VK_RIGHTはPreTranslateMessageで処理する
		else if(wTerm == VK_RETURN)	{
			m_Diag2.SetPosition(COMBO_HIK);
			return;
		}
	}
	else if(index == COMBO_HIK)	{

		m_Diag2.GetData(index,(LPUNKNOWN)&dd);
		ComboSelIcsdiag8ctrl2Ex(index,&dd);

		//if(wTerm == VK_F2 || wTerm == VK_LEFT)	{
		// VK_LEFTはPreTranslateMessageで処理する
		if(wTerm == VK_F2)	{
			m_Diag2.SetPosition(COMBO_TEI);
			return;
		}
		else if(wTerm == VK_RETURN)	{
			m_Diag2.SetPosition(CHEK01_1);
			return;
		}
	}
	else if(index == CHEK01_1 || (index >= HP_TEI_CHK && index <= HP_HIK_BSU))	{
		diag_getattr(IDC_ICSDIAG8CTRL2,COMBO_TEI,&da,CIcsdiagctrl);
		if(da.attr_bcolor == BC_WHITE)	{
			if(index == CHEK01_1 && wTerm == VK_F2)	{
				m_Diag2.SetPosition(COMBO_HIK);
				return;
			}
			if((index == HP_TEI_CHK || index == HP_TEI_BSU) && wTerm == VK_UP)	{
				m_Diag2.SetPosition(COMBO_TEI);
				return;
			}
			if((index == HP_HIK_CHK || index == HP_HIK_BSU) && wTerm == VK_UP)	{
				m_Diag2.SetPosition(COMBO_HIK);
				return;
			}
		}
	}

	// ポジションの2次元配列から現在の場所を探す
	sw=0;
	for(xx=0; xx<10; xx++)	{
		for(yy=0; yy<30; yy++)	{
			if(CursorPos2[yy][xx] == index)	{
				sw=1;
				break;
			}
		}
		if(sw != 0)	break;
	}
	if(sw == 0)	{
		return;	// ありえない
	}

//	DIAGRAM_ATTRIBUTE	da;
	short				npos=-1;

	switch(wTerm)	{
		case VK_RETURN:
		case VK_F2:
			while(1)	{
				if(wTerm == VK_RETURN)	{
					xx++;
					if(xx >= 10)	{
						xx=0;
						yy++;
					}
					if(yy >= 30)	{
						yy=0;
					}
				}
				if(wTerm == VK_F2)		{
					xx--;
					if(xx < 0)	{
						xx=9;
						yy--;
					}
					if(yy < 0)	{
						yy=29;
					}
				}
				if(CursorPos2[yy][xx] == -1)	{
					continue;
				}
				diag_getattr(IDC_ICSDIAG8CTRL2,CursorPos2[yy][xx],&da,CIcsdiagctrl);
				if(da.attr_bcolor == BC_GRAY)	{
					continue;
				}
// 24/03/25_168696 cor -->
				//if(da.attr_bcolor == BC_WHITE)	{
// -----------------------
				if(da.attr_bcolor == BC_WHITE || da.attr_bcolor == BC_GREEN_L)	{
// 24/03/25_168696 cor <--
					npos = CursorPos2[yy][xx];
					break;
				}
			}
			break;
		case VK_RIGHT:
		case VK_LEFT:
			while(1)	{
				if(wTerm == VK_RIGHT)	xx++;
				if(wTerm == VK_LEFT)	xx--;
				if(CursorPos2[yy][xx] == -1)	{
					break;
				}
				diag_getattr(IDC_ICSDIAG8CTRL2,CursorPos2[yy][xx],&da,CIcsdiagctrl);
				if(da.attr_bcolor == BC_GRAY)	{
					continue;
				}
// 24/03/25_168696 cor -->
				//if(da.attr_bcolor == BC_WHITE)	{
// -----------------------
				if(da.attr_bcolor == BC_WHITE || da.attr_bcolor == BC_GREEN_L)	{
// 24/03/25_168696 cor <--
					npos = CursorPos2[yy][xx];
					break;
				}
			}
			break;
		case VK_DOWN:
		case VK_UP:
			while(1)	{
				if(wTerm == VK_DOWN)	yy++;
				if(wTerm == VK_UP)		yy--;
				if(CursorPos2[yy][xx] == -1)	{
// 24/03/25_168696 cor -->
					//break;
// -----------------------
					if(yy == 0)	break;
					else		continue;
// 24/03/25_168696 cor <--
				}
				diag_getattr(IDC_ICSDIAG8CTRL2,CursorPos2[yy][xx],&da,CIcsdiagctrl);
				if(da.attr_bcolor == BC_GRAY)	{
					continue;
				}
// 24/03/25_168696 cor -->
				//if(da.attr_bcolor == BC_WHITE)	{
// -----------------------
				if(da.attr_bcolor == BC_WHITE || da.attr_bcolor == BC_GREEN_L)	{
// 24/03/25_168696 cor <--
					npos = CursorPos2[yy][xx];
					break;
				}
			}
			break;
	}
	if(npos != -1)	{
		// 最終のインデックスでEnterキーを押下した場合
		if(wTerm == VK_RETURN && npos == CHEK01_1)	{
			// 選択可であれば提出用カラー/モノクロにフォーカスをセットする
			diag_getattr(IDC_ICSDIAG8CTRL2,COMBO_TEI,&da,CIcsdiagctrl);
// 24/03/25_168696 cor -->
			//if(da.attr_bcolor == BC_WHITE)	{
// -----------------------
			if(da.attr_bcolor == BC_WHITE || da.attr_bcolor == BC_GREEN_L)	{
// 24/03/25_168696 cor <--
				npos = COMBO_TEI;
			}
		}
		m_Diag2.SetPosition(npos);
	}
}

void CPrnPrint_EX2::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	GetInputItemData( index );
	// 白紙
	if(m_pSnHeadData->Sn_TYPE & 0x01)	{
		int				ii=0,jj=0,idx=0,sw=0,chk[3];
		DIAGRAM_DATA	dd;
		// 申告書本表 or 課税標準額等の内訳書
		// いずれかのチェックをON/OFFにした場合
		// 提出用、控用それぞれのカラー・モノクロすべてを同じ状態にする
		if(index == CHEK01_1 || index == CHEK01_2)	{
			m_Diag2.GetData(index,(LPUNKNOWN)&dd);
			for(int ii=0; ii<2; ii++)	{
				if(index == CHEK01_1)	{
					if(ii == 0)	idx=HP_TEI_CHK;
					else		idx=HP_HIK_CHK;
				}
				if(index == CHEK01_2)	{
					if(ii == 0)	idx=KZ_TEI_CHK;
					else		idx=KZ_HIK_CHK;
				}
				m_Diag2.SetData(idx,(LPUNKNOWN)&dd);
				GetInputItemData(idx);
			}
		}
		// 申告書本表 or 課税標準額等の内訳書それぞれの
		// 提出用、控用（カラー・モノクロ）いずれかのチェックをON/OFFにした場合
		// 一つでもチェックが付いていれば申告書本表（課税標準額等の内訳書）のチェックをON
		// すべてのチェックが外れていれば申告書本表（課税標準額等の内訳書）のチェックをOFF
		// にする
		for(ii=0; ii<2; ii++)	{
			// 申告書本表グループ
			if(ii == 0)	{
				chk[0]=CHEK01_1;
				chk[1]=HP_TEI_CHK;
				chk[2]=HP_HIK_CHK;
			}
			// 課税標準額等の内訳書グループ
			else	{
				chk[0]=CHEK01_2;
				chk[1]=KZ_TEI_CHK;
				chk[2]=KZ_HIK_CHK;
			}
			if(index == chk[1] || index == chk[2])	{
				for(int jj=0; jj<2; jj++)	{
					idx=chk[jj+1];
					m_Diag2.GetData(idx,(LPUNKNOWN)&dd);
					if(dd.data_check != 0)	sw++;
				}
				m_Diag2.GetData(chk[0],(LPUNKNOWN)&dd);
				if(sw != 0)	{
					if(dd.data_check == 0)	{
						dd.data_check = 1;
						m_Diag2.SetData(chk[0],(LPUNKNOWN)&dd);
						GetInputItemData(chk[0]);
					}
				}
				else	{
					if(dd.data_check != 0)	{
						dd.data_check = 0;
						m_Diag2.SetData(chk[0],(LPUNKNOWN)&dd);
						GetInputItemData(chk[0]);
					}
				}
			}
		}
// 24/03/22_背景色 add -->
		// 課税取引金額計算、特定収入計算表、税務代理書面 の背景色を変更
		if(index == CHEK12 || index == CHEK13 || index == CHEK14)	{
			DIAGRAM_DATA *pdata = (DIAGRAM_DATA*)data;
			BackColorControl(index,pdata);
		}
// 24/03/22_背景色 add <--
	}
}

void CPrnPrint_EX2::ComboSelIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA *pdata = (DIAGRAM_DATA*)data;

	ComboSelIcsdiag8ctrl2Ex(index,pdata);
}

void CPrnPrint_EX2::ComboSelIcsdiag8ctrl2Ex( short pIndex, DIAGRAM_DATA* pData )
{
	DIAGRAM_DATA dd;

	// 提出用カラー/モノクロ
	if(pIndex == COMBO_TEI)	{
		m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x01;
		// 提出用カラー
		if(pData->data_combo == 0)	{
			m_pSnHeadData->Sn_PrintOutSgn2 |= 0x01;
		}
	}
	// 控　用カラー/モノクロ
	if(pIndex == COMBO_HIK)	{
		m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x02;
		// 控　用カラー
		if(pData->data_combo == 0)	{
			m_pSnHeadData->Sn_PrintOutSgn2 |= 0x02;
		}
	}
}

//-----------------------------------------------------------------------------
// 全選択＆全解除
//-----------------------------------------------------------------------------
// 【引　数】	int		0:全選択＆全解除　1:チェックOFFが1箇所でもあるか調べる
// 【戻り値】	BOOL	チェックOFFが1箇所でもあればFALSE
//-----------------------------------------------------------------------------
BOOL CPrnPrint_EX2::ChoiceItem( int pSw )
{
	BOOL			brv=TRUE;
	SH_ITEMINDEX	*pPrnTbl;
	short			idx=0;

	if(pSw == 0)	{
		// フォーカスOFF
		idx = ((CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2))->GetPosition();
		diag_deledit(IDC_ICSDIAG8CTRL2,CIcsdiagctrl);
	}

	if( m_pSnHeadData->IsMiddleProvisional() ){
		pPrnTbl = &PrnTbl1[0];
	}
	else{
		if( !(m_pSnHeadData->Sn_TYPE&0x01) ){
			pPrnTbl = &PrnTbl2[0];
		}
		else{
			pPrnTbl = &PrnTbl3[0];
		}
	}
	//if( m_SelSw ){
	//	m_SelSw = 0;
	//}
	//else{
	//	m_SelSw = 1;
	//}

	int cnt = 0;
	while( 1 ){
		if( (pPrnTbl+cnt)->Index == 0 ){
			break;
		}
		if((pPrnTbl+cnt)->Index == COMBO_TEI || (pPrnTbl+cnt)->Index == COMBO_HIK)	{
			cnt++;
			continue;
		}
		//if( ((pPrnTbl+cnt)->Index==COMB01) || ((pPrnTbl+cnt)->Index==COMB02) ){
		//	cnt++;
		//	continue;
		//}
		int Flg = 0;
		if( m_pSnHeadData->SVmzsw ){
			Flg = (pPrnTbl+cnt)->UnConnect;
		}
		else{
			Flg = (pPrnTbl+cnt)->Connect;
		}

		switch( Flg ){
			case 1 :	// 本表・付表・第26号様式
				Flg = 1;
				if( !m_pSnHeadData->SVmzsw ){	// 20230616
					if ( ( ( pPrnTbl + cnt )->Index == CHEK04_1 ) || ( ( pPrnTbl + cnt )->Index == CHEK03_1 )
						   || ( ( pPrnTbl + cnt )->Index == CHEK04_2 ) || ( ( pPrnTbl + cnt )->Index == CHEK03_2 ) ) {
							Flg = 0;
					}
					else if ( ( ( pPrnTbl + cnt )->Index == CHEK04_3 ) || ( ( pPrnTbl + cnt )->Index == CHEK03_3 ) ) {
							Flg = 0;
					}
				}
//--> '15.02.04 CUT START
//				else{//2/2を無効に
//					if( ((pPrnTbl+cnt)->Index == CHEK20) && !(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) ){
//						Flg = 0;
//					}
//				}
//<-- '15.02.04 CUT END
				break;
			case 2 :	// 仕入控除税額に関する明細書
			case 3 :	// 仕入控除税額に関する明細書（次葉）
			case 4 :	// 消費税の還付申告に関する明細書
				Flg = 0;
				break;
			case 5 :	// 更正の請求書
				if (m_pSnHeadData->Sn_Syuusei & 0x02) {
					Flg = 1;
				}
				else {
					Flg = 0;
				}
				break;

			case 6:	// 課税取引金額計算表
				if(m_pSnHeadData->IsSoleProprietor() != FALSE)	{
					Flg = 1;
				}
				else	{
					Flg = 0;
				}
				break;

			case 7:	// 特定収入計算表
				Flg = 0;
				break;

			case 8:	// 税務代理書面
// 24/03/18_税務代理書面 cor -->
				//Flg = 1;
// -----------------------------
// 24/03/26_税務代理書面 cor -->
				//if(m_Eymd >= R06_ZEIMUDAIRI)	Flg = 1;
				//else							Flg = 0;
// -----------------------------
// 24/04/01_168704 cor -->
				//Flg = ((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud();
// -----------------------
				if(m_pParent == NULL)	{
					Flg = 1;
				}
				else	{
// 24/07/10_個別タブから出力 cor -->
					//Flg = ((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud();
// ---------------------------------
					if( ((CPrnReportDlg*)m_pParent)->ZeitempEnableChk() > 0 &&
						((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud() == TRUE) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
// 24/07/10_個別タブから出力 cor <--
				}
// 24/04/01_168704 cor <--
// 24/03/26_税務代理書面 cor <--
// 24/03/18_税務代理書面 cor <--
				break;

			//case 6 :	// 課税取引金額計算表　事業所得用
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
			//			Flg = 0;	// 事業別売上高[13'09.19]
			//		}
			//		else {
			//			if (m_pZmSub->zvol->ind_type & 0x01) {
			//				Flg = 1;
			//			}
			//			else {
			//				Flg = 0;
			//			}
			//		}
			//	}
			//	break;
			//case 7 :	// 課税取引金額計算表　農業所得用
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
			//			Flg = 0;	// 事業別売上高[13'09.19]
			//		}
			//		else {
			//			if (m_pZmSub->zvol->ind_type & 0x04) {
			//				Flg = 1;
			//			}
			//			else {
			//				Flg = 0;
			//			}
			//		}
			//	}
			//	break;
			//case 8 :	// 課税取引金額計算表　不動産所得用
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		if (m_Util.IsUnConnectKaniMst(m_pZmSub->zvol)) {
			//			Flg = 0;	// 事業別売上高[13'09.19]
			//		}
			//		else {
			//			if (m_pZmSub->zvol->ind_type & 0x02) {
			//				Flg = 1;
			//			}
			//			else {
			//				Flg = 0;
			//			}
			//		}
			//	}
			//	break;
			//case 9 :	// 課税売上高計算表
			//	if (m_pSnHeadData->IsSoleProprietor() == FALSE) {
			//		Flg = 0;
			//	}
			//	else {
			//		Flg = 1;
			//	}
			//	break;
			//case 10:	// 課税仕入高計算表
			//	//2割特例適用時は簡易課税と同じ扱いの為、課税仕入高計算表は表示しない
			//case 11:	// 特定収入計算表
			//case 12:	// 事業別売上高[13'09.19] 
			//	Flg = 0;
			//	break;

			case 13:	// 特定課税仕入れがある場合の課税標準額等の内訳書()
				if( m_pPrintCmInfo->Shrevtype < ID_ICSSH_TKKZSIIRE_REVTYPE ){
					Flg = 0;
				}
				/*
				else if( m_pSnHeadData->IsNeedTkkzDisp() ){
					Flg = 1;
				}
				else{
					Flg = 0;
				}*/
				break;
			case 14:	// 付表６
				if ((m_pSnHeadData->IsSoleProprietor()) && (m_pSnHeadData->Sn_Syuusei & 0x10)) {
					Flg = 1;
				}
				else {
					Flg = 0;
				}
				break;
			case 15:
			case 16:
			case 17:
				Flg = 0;
				break;
			case 18:	// 付表６（税率別消費税額計算表）
				//----> 20230616
				if( m_pSnHeadData->SVmzsw ){
					Flg = 0;
				}
				//<---- 20230616
				else{
					Flg = 1;
				}
				break;
			default:	// 入力不可
				Flg = 0;
				break;
		}
		if( Flg ){
			DIAGRAM_DATA dd;
			if(pSw == 0)	{
				dd.data_imgdata = NULL;
				if( m_SelSw ){
					dd.data_check = 1;
				}
				else{
					dd.data_check = 0;
				}
				m_Diag2.SetData( (pPrnTbl+cnt)->Index, (LPUNKNOWN)&dd );
				GetInputItemData( (pPrnTbl+cnt)->Index );
				// 申告書本表 or 課税標準額等の内訳書
				if((pPrnTbl+cnt)->Index == CHEK01_1 || (pPrnTbl+cnt)->Index == CHEK01_2)	{
					// 白紙
					if(m_pSnHeadData->Sn_TYPE & 0x01)	{
						int	ii=0,idx=0;
						for(ii=0; ii<2; ii++)	{
							if((pPrnTbl+cnt)->Index == CHEK01_1)	{
								if(ii == 0)	{
									idx=HP_TEI_CHK;
								}
								else	{
									idx=HP_HIK_CHK;
								}
							}
							if((pPrnTbl+cnt)->Index == CHEK01_2)	{
								if(ii == 0)	{
									idx=KZ_TEI_CHK;
								}
								else	{
									idx=KZ_HIK_CHK;
								}
							}
							dd.data_imgdata = NULL;
							if(m_SelSw)	dd.data_check = 1;
							else		dd.data_check = 0;
							m_Diag2.SetData(idx,(LPUNKNOWN)&dd);
							GetInputItemData(idx);
						}
					}
				}
			}
			else	{
				m_Diag2.GetData((pPrnTbl+cnt)->Index,(LPUNKNOWN)&dd);
				if(dd.data_check == 0)	{
					brv=FALSE;
					break;
				}
			}
		}
		cnt++;
	}
	if(pSw == 0)	{
		// フォーカスON
		// 修正依頼No.160285
		// 全表出力可のチェックOFFによってフォーカスのあった箇所が
		// 入力不可になっている場合、先頭にフォーカスをセットする
		DIAGRAM_ATTRIBUTE	da;
		diag_getattr(IDC_ICSDIAG8CTRL2,idx,&da,CIcsdiagctrl);
		if(da.attr_bcolor == BC_WHITE)	{
			diag_setposition(IDC_ICSDIAG8CTRL2,idx,CIcsdiagctrl);
		}
		else	{
			diag_getattr(IDC_ICSDIAG8CTRL2,COMBO_TEI,&da,CIcsdiagctrl);
			if(da.attr_bcolor == BC_WHITE)	{
				diag_setposition(IDC_ICSDIAG8CTRL2,COMBO_TEI,CIcsdiagctrl);
			}
			else	{
				diag_setposition(IDC_ICSDIAG8CTRL2,CHEK01_1,CIcsdiagctrl);
			}
		}
	}

	return(brv);
}

//-----------------------------------------------------------------------------
// 入力項目色変更
//-----------------------------------------------------------------------------
// 引数	id		：
//		index	：
//		sign	：
//-----------------------------------------------------------------------------
void CPrnPrint_EX2::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :
			DA.attr_bcolor = BC_WHITE;
// 24/03/25_168691 add -->
			if(index == CHEK12 || index == CHEK13 || index == CHEK14)	{
				DIAGRAM_DATA		dd;
				m_Diag2.GetData(index,(LPUNKNOWN)&dd);
				if(dd.data_check != 0)	{
					int	sw=0;
					if(index == CHEK12)			sw=0;
					else if(index == CHEK13)	sw=1;
					else						sw=2;
// 24/04/01_168704 cor -->
					//bool	brv = ((CPrnReportDlg*)m_pParent)->IsAllCheckOn(sw);
// -----------------------
					bool	brv = true;
					if(m_pParent != NULL)	{
						brv = ((CPrnReportDlg*)m_pParent)->IsAllCheckOn(sw);
					}
// 24/04/01_168704 cor <--
					if(brv == true)	{
						DA.attr_bcolor = BC_WHITE;
					}
					else	{
						DA.attr_bcolor = BC_GREEN_L;
					}
				}
			}
// 24/03/25_168691 add <--
			break;		// 入力不可	（白色）
		case 2 :
			DA.attr_bcolor = BC_GRAY;		
			break;		// ロック	（灰色）
		case 3 :
			DA.attr_bcolor = BC_BLUE;	
			break;		// 特殊色①	（青色）
		case 4 :
			DA.attr_bcolor = BC_GREEN;	
			break;		// 特殊色②	（緑色）	
		case 5 :
			DA.attr_bcolor = BC_GREEN_L;
			break;		// 特殊色③	（薄緑色）	 
		default: 
			DA.attr_bcolor = BC_CREAM;	
			break;		// 実額入力	（クリーム色） 
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}


//F9 全選択・解除
void CPrnPrint_EX2::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if(ChoiceItem(1) == FALSE)	{
		// チェックOFFが1箇所でもあれば全選択
		m_SelSw = 1;
	}
	else	{
		// すべてチェックONなら全解除
		m_SelSw = 0;
	}
	ChoiceItem(0);

// 24/03/25_168691 add -->
	// 課税取引金額計算、特定収入計算表、税務代理書面 の背景色を変更
	if(GetDlgItem(IDC_BUTTON4)->IsWindowEnabled() == TRUE && GetDlgItem(IDC_BUTTON4)->IsWindowVisible() == TRUE)	{
		BackColorControl(CHEK12);
	}
	if(GetDlgItem(IDC_BUTTON3)->IsWindowEnabled() == TRUE && GetDlgItem(IDC_BUTTON3)->IsWindowVisible() == TRUE)	{
		BackColorControl(CHEK13);
	}
	if(GetDlgItem(IDC_BUTTON5)->IsWindowEnabled() == TRUE && GetDlgItem(IDC_BUTTON5)->IsWindowVisible() == TRUE)	{
		BackColorControl(CHEK14);
	}
// 24/03/25_168691 add <--
}

//F12 次画面へ
void CPrnPrint_EX2::OnBnClickedButton2()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F12);
}



void CPrnPrint_EX2::OnBnClickedButton3()
{
	int tab_num = 1;

	CWnd*	p1 = GetParent();
	CWnd*	p2 = p1->GetParent();
	if ((((CPrnReportDlg*)p2)->m_pPrnTokS) == NULL) return;

	if( GetDlgItem(IDC_BUTTON3)->IsWindowEnabled() != TRUE || 
		GetDlgItem(IDC_BUTTON3)->IsWindowVisible() != TRUE )	{
		return;
	}

	if ((((CPrnReportDlg*)p2)->m_pPrnKazS) != NULL) tab_num++;

	((CPrnReportDlg*)p2)->m_PrnTab.SetCurSel(tab_num);
	((CPrnReportDlg*)p2)->SetFocusTab(2);//特定収入

	return;
}

void CPrnPrint_EX2::OnBnClickedButton4()
{
	int tab_num = 1;
	CWnd*	p1 = GetParent();
	CWnd*	p2 = p1->GetParent();

	if ((((CPrnReportDlg*)p2)->m_pPrnKazS) == NULL) return;

	if( GetDlgItem(IDC_BUTTON4)->IsWindowEnabled() != TRUE || 
		GetDlgItem(IDC_BUTTON4)->IsWindowVisible() != TRUE )	{
		return;
	}

	((CPrnReportDlg*)p2)->m_PrnTab.SetCurSel(tab_num);
	((CPrnReportDlg*)p2)->SetFocusTab(1);//課税取引
}


void CPrnPrint_EX2::OnBnClickedButton5()
{
	int tab_num = 1;
	CWnd*	p1 = GetParent();
	CWnd*	p2 = p1->GetParent();

	if( GetDlgItem(IDC_BUTTON5)->IsWindowEnabled() != TRUE || 
		GetDlgItem(IDC_BUTTON5)->IsWindowVisible() != TRUE )	{
		return;
	}

	if ((((CPrnReportDlg*)p2)->m_pPrnKazS) != NULL) tab_num++;
	if ((((CPrnReportDlg*)p2)->m_pPrnTokS) != NULL) tab_num++;

	((CPrnReportDlg*)p2)->m_PrnTab.SetCurSel(tab_num);
	((CPrnReportDlg*)p2)->SetFocusTab(3);//添付書面
}



// ボタン制御
void CPrnPrint_EX2::Button3Control(void)
{
	//BOOL			syzSgn = FALSE;
	//long			eymd = 0;
	//DIAGRAM_DATA	dd;

	//// 非営利法人以外は必要ないので非表示にしてリターン
	//if (((m_pZmSub->zvol->apno & 0xf0) <= 0x30) || (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {
	//	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);

	//	ChangeColor(IDC_ICSDIAG8CTRL2, CHEK13, 2);
	//	m_Diag2.ModifyItem(CHEK13, DIAG_MDFY_READONLY);
	//	ChangeColor(IDC_ICSDIAG8CTRL2, CHEK13_BUSU, 2);
	//	m_Diag2.ModifyItem(CHEK13_BUSU, DIAG_MDFY_READONLY);

	//	return;
	//}

	//GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_SHOW);

	//ChangeColor(IDC_ICSDIAG8CTRL2, CHEK13, 1);
	//m_Diag2.ModifyItem(CHEK13, DIAG_MDFY_EDIT);
	//ChangeColor(IDC_ICSDIAG8CTRL2, CHEK13_BUSU, 1);
	//m_Diag2.ModifyItem(CHEK13_BUSU, DIAG_MDFY_EDIT);


	//// 課税取引・旧税率ありなしの判定
	//if (!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	//else								eymd = m_pSnHeadData->Sn_KDAYE;
	//if (m_pSnHeadData->SVmzsw == 1 || eymd < TAX_R04_SDAY) {
	//	syzSgn = TRUE;	// 旧税率あり
	//}

	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
}
// 24/02/19wd_特定収入対応 add <--koko

void CPrnPrint_EX2::Button4Control(void)
{
	//BOOL			syzSgn = FALSE;
	//long			eymd = 0;
	//DIAGRAM_DATA	dd;

	////個人または個人事業者の場合
	//if (!(m_pSnHeadData->SVkojn & 0x01)) {
	//	GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_HIDE);

	//	ChangeColor(IDC_ICSDIAG8CTRL2, CHEK12, 1);
	//	m_Diag2.ModifyItem(CHEK12, DIAG_MDFY_EDIT);
	//	ChangeColor(IDC_ICSDIAG8CTRL2, CHEK12_BUSU, 1);
	//	m_Diag2.ModifyItem(CHEK12_BUSU, DIAG_MDFY_EDIT);

	//	return;
	//}

	//GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_SHOW);

	//ChangeColor(IDC_ICSDIAG8CTRL2, CHEK12, 1);
	//m_Diag2.ModifyItem(CHEK12, DIAG_MDFY_EDIT);
	//ChangeColor(IDC_ICSDIAG8CTRL2, CHEK12_BUSU, 1);
	//m_Diag2.ModifyItem(CHEK12_BUSU, DIAG_MDFY_EDIT);

// 修正No.168698 cor -->
	//if(m_pSnHeadData->IsSoleProprietor() == FALSE)	{
// ---------------------
	// 個人じゃない または 予定申告
	if(m_pSnHeadData->IsSoleProprietor() == FALSE || m_pSnHeadData->IsMiddleProvisional() == TRUE)	{
// 修正No.168698 cor <--
		GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_HIDE);
	}
	else	{
		GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_SHOW);
	}
}

void CPrnPrint_EX2::Button5Control(void)
{
// 24/03/18_税務代理書面 cor -->
	//GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);

	////ChangeColor(IDC_ICSDIAG8CTRL2, CHEK14, 1);
	////m_Diag2.ModifyItem(CHEK14, DIAG_MDFY_EDIT);
	////ChangeColor(IDC_ICSDIAG8CTRL2, CHEK14_BUSU, 1);
	//m_Diag2.ModifyItem(CHEK14_BUSU, DIAG_MDFY_EDIT);
// -----------------------------
// 修正No.168698 cor -->
	//if(m_Eymd >= R06_ZEIMUDAIRI)	GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
	//else							GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
// ---------------------
// 24/04/01_168704 cor -->
//// 24/03/26_税務代理書面 cor -->
//	//if(m_Eymd >= R06_ZEIMUDAIRI && m_pSnHeadData->IsMiddleProvisional() == FALSE)	{
//// -----------------------------
//	if(((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud() == TRUE && m_pSnHeadData->IsMiddleProvisional() == FALSE)	{
//// 24/03/26_税務代理書面 cor <--
//		GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
//	}
//	else	{
//		GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
//	}
//// 修正No.168698 cor <--
//// 24/03/18_税務代理書面 cor <--
// -----------------------
	if(m_pParent == NULL)	{
		GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
	}
	else	{
// 24/04/04_341 cor -->
		//if(((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud() == TRUE && m_pSnHeadData->IsMiddleProvisional() == FALSE)	{
// --------------------
		if(((CPrnReportDlg*)m_pParent)->m_pMainFrame->IsUsedZeimud() == TRUE)	{
// 24/04/04_341 cor <--
			GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
		}
		else	{
			GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
		}
	}
// 24/04/01_168704 cor <--
}

void CPrnPrint_EX2::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

void CPrnPrint_EX2::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnCancel();
}



HBRUSH CPrnPrint_EX2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if(GetDlgItem(IDC_STATIC_INCOME) == pWnd)
		pDC->SetTextColor(RGB(255,0,0));
	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CPrnPrint_EX2::OnBnClickedCheckAllout()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	int st = m_AlloutCheck.GetCheck();
	if( st == BST_CHECKED ){
		m_PrselState = ID_ALL_PRINT_SEL_STATE;
		m_pPrintCmInfo->OP_PSEL |= 0x02;
	}
	else if( st == BST_UNCHECKED ){
		m_PrselState = ID_SEP_PRINT_SEL_STATE;
		m_pPrintCmInfo->OP_PSEL &= ~0x02;
	}

	SetInputItem();

// 24/07/10_個別タブから出力 add -->
	// 特定収入計算表タブのSetInputItem関数を呼び出し
	((CPrnReportDlg*)m_pParent)->CallSetEnableInputItem();
// 24/07/10_個別タブから出力 add <--
}

// ラジオボタン（白紙に印刷する）をクリック
void CPrnPrint_EX2::OnBnClickedRadio1()
{
// 160103 add -->
	if(!(m_pSnHeadData->Sn_TYPE & 0x01))	{
		// ＯＣＲ用紙に印刷するから変更された場合

		// 申告書本表、課税標準額等の内訳書はチェックが付いていれば
		// 直前のチェック状態に戻す
		if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x04)	{
			// 白紙のときにチェックが付いていなければ「提出用」のみONにする
			if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x08) && 
				!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x10) )	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x08;
			}
		}
		if(m_pSnHeadData->Sn_PrintOutSgn2 & 0x20)	{
			if( !(m_pSnHeadData->Sn_PrintOutSgn2 & 0x40) && 
				!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x80) )	{
				m_pSnHeadData->Sn_PrintOutSgn2 |= 0x40;
			}
		}
	}
// 160103 add <--
	m_pSnHeadData->Sn_TYPE &= 0xfc;
	m_pSnHeadData->Sn_TYPE |= 0x01;
	// 全表出力可チェックを選択可にする
// 160121 del -->
	//m_AlloutCheck.EnableWindow(TRUE);
// 160121 del <--
// 160121 add -->
	if(!(m_pSnHeadData->IsMiddleProvisional()))	{
		m_AlloutCheck.EnableWindow(TRUE);
	}
// 160121 add <--
	// 入力項目設定
	SetInputItem();
	// 
	// 選択可であれば提出用カラー/モノクロにフォーカスをセットする
	DIAGRAM_ATTRIBUTE	da;
	diag_getattr(IDC_ICSDIAG8CTRL2,COMBO_TEI,&da,CIcsdiagctrl);
	if(da.attr_bcolor == BC_WHITE)	{
		m_Diag2.SetPosition(COMBO_TEI);
	}
	else	{
		m_Diag2.SetPosition(CHEK01_1);
	}
}

// ラジオボタン（ＯＣＲ用紙に印刷する）をクリック
void CPrnPrint_EX2::OnBnClickedRadio2()
{
	if(m_swBeForked)	{
		ICSMessageBox(_T("一括印刷処理ではＫＳＫ用（ＯＣＲ用紙）への印刷は出来ません。"));
		m_Radio1.SetCheck(1);
		m_Radio2.SetCheck(0);
		m_Radio1.SetFocus();
		return;
	}
	m_pSnHeadData->Sn_TYPE &= 0xfc;
	// 全表出力可チェックを選択不可にする
	m_AlloutCheck.EnableWindow(FALSE);
	// 入力項目設定
	SetInputItem();
	// 
	m_Diag2.SetPosition(CHEK01_1);
}

//-----------------------------------------------------------------------------
// 引数のインデックス番号が部数であるかを返す
//-----------------------------------------------------------------------------
// 引　数	int		：	インデックス番号
// 返送値	BOOL	：	TRUE:部数である
//-----------------------------------------------------------------------------
BOOL CPrnPrint_EX2::IsBusu( int pIdx )
{
	int	ii=0;
	BOOL	brv=FALSE;

	for(ii=0; ; ii++)	{
		if(LstBusuIdxGen2[ii].dsp_no == -1)	{
			break;
		}
		if(LstBusuIdxGen2[ii].dsp_no == pIdx)	{
			brv=TRUE;
			break;
		}
	}
	return(brv);
}

//-----------------------------------------------------------------------------
// 引数に該当する部数の配列番号を返す
//-----------------------------------------------------------------------------
// 引　数	int		：	インデックス番号
// 返送値	int		：	部数の配列番号
//-----------------------------------------------------------------------------
int CPrnPrint_EX2::GetBusuNo( int pIdx )
{
	int	ii=0,ret=-1;

	for(ii=0; ; ii++)	{
		if(LstBusuIdxGen2[ii].dsp_no == -1)	{
			break;
		}
		if(LstBusuIdxGen2[ii].dsp_no == pIdx)	{
			ret=ii;
			break;
		}
	}
	return(ret);
}

//-----------------------------------------------------------------------------
// 部数のインデックスから該当する帳票にチェックが付いているかを調べる
//-----------------------------------------------------------------------------
// 引　数	int		：	インデックス番号
// 返送値	BOOL	：	TRUE=チェックが付いている
//-----------------------------------------------------------------------------
BOOL CPrnPrint_EX2::IsCheckOn( int pIdx )
{
	BOOL			brv=FALSE;
	int				ii=0,jj=0;
	DIAGRAM_DATA	dd;

	for(ii=0; ; ii++)	{
		if(LstBusuTbl2[ii].idx[0] == -1)	break;
		for(jj=1; ; jj++)	{
			if(LstBusuTbl2[ii].idx[jj] == -1)	{
				break;
			}
			if(LstBusuTbl2[ii].idx[jj] == pIdx)	{
				m_Diag2.GetData(LstBusuTbl2[ii].idx[0],(LPUNKNOWN)&dd);
				if(dd.data_check != 0)	{
					brv=TRUE;
				}
				break;
			}
		}
	}
	return(brv);
}

//-----------------------------------------------------------------------------
// 指定されている部数のうち一番大きい部数を返す
//-----------------------------------------------------------------------------
// 引　数	なし
// 返送値	int		：	最大部数
//-----------------------------------------------------------------------------
int CPrnPrint_EX2::GetMaxBusu( void )
{
	int				ret=0;
	int				cnt=0;
	int				ii=0,jj=0,kk=0;
	//DIAGRAM_DATA	dd;
	SH_ITEMINDEX	*pPrnTbl;

	// 予定26号様式
	if(m_pSnHeadData->IsMiddleProvisional())	{
		pPrnTbl = &PrnTbl1[0];
	}
	else{
		// ＯＣＲ
		if(!(m_pSnHeadData->Sn_TYPE&0x01))	{
			// 部数指定は不可のため "1" を返す
			return(1);
		}
		else{
			pPrnTbl = &PrnTbl3[0];
		}
	}
	while(1)	{
		if((pPrnTbl+cnt)->Index == 0)	{
			break;
		}
		//m_Diag2.GetData((pPrnTbl+cnt)->Index,(LPUNKNOWN)&dd);
		//if(dd.data_check != 0)	{
			for(ii=0; ; ii++)	{
				if(LstBusuTbl2[ii].idx[0] == -1)	{
					break;
				}
				if(LstBusuTbl2[ii].idx[0] == (pPrnTbl+cnt)->Index)	{
					for(jj=1; ; jj++)	{
						if(LstBusuTbl2[ii].idx[jj] == -1)	{
							break;
						}
						for(kk=0; ; kk++)	{
							if(LstBusuIdxGen2[kk].dsp_no == -1)	{
								break;
							}
							if(LstBusuIdxGen2[kk].dsp_no == LstBusuTbl2[ii].idx[jj])	{
// 24/07/10_個別タブから出力 cor -->
								////m_Diag2.GetData(LstBusuIdxGen2[kk].dsp_no,(LPUNKNOWN)&dd);
								////if(ret < _tstoi(dd.data_edit))	{
								////	ret = _tstoi(dd.data_edit);
								////}
								//if(ret < m_pPrintCmInfo->busu[GetBusuNo(LstBusuTbl2[ii].idx[jj])])	{
								//	ret = m_pPrintCmInfo->busu[GetBusuNo(LstBusuTbl2[ii].idx[jj])];
								//}
// ---------------------------------
								if(LstBusuTbl2[ii].idx[jj] == CHEK12_BUSU)	{		// 課税取引金額計算表の部数
									::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
									for(int ii=0; ii<5; ii++)	{
										// CMapから個別指定情報を取り出し
										::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
										m_pMapPrnEXSub_KAZ.Lookup(ii, m_PrnEXSub_KAZ);
										if(m_PrnEXSub_KAZ.check == 1 && ret < m_PrnEXSub_KAZ.busu)	{
											ret = m_PrnEXSub_KAZ.busu;
										}
									}
								}
								else if(LstBusuTbl2[ii].idx[jj] == CHEK14_BUSU)	{	// 税務代理書面の部数
									::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
									for(int ii=0; ii<11; ii++)	{
										// CMapから個別指定情報を取り出し
										::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
										m_pMapPrnEXSub_ZEI.Lookup(ii, m_PrnEXSub_ZEI);
										if(m_PrnEXSub_ZEI.check == 1 && ret < m_PrnEXSub_ZEI.busu)	{
											ret = m_PrnEXSub_ZEI.busu;
										}
									}
								}
								else	{	// 課税取引金額計算表、特定収入計算表、税務代理書面以外
									if(ret < m_pPrintCmInfo->busu[GetBusuNo(LstBusuTbl2[ii].idx[jj])]) {
										ret = m_pPrintCmInfo->busu[GetBusuNo(LstBusuTbl2[ii].idx[jj])];
									}
								}
// 24/07/10_個別タブから出力 cor <--
								break;
							}
						}
					}
				}
			}
		//}
		cnt++;
	}

	return(ret);
}

//--------------------------------------------------------------------------------------------
// 帳票番号から画面の部数インデックスを割り出し、部数が指定の数字以上かをチェックする
// 指定の数字以上であれば帳票番号と部数を書き込む
//--------------------------------------------------------------------------------------------
// 引　数	unsigned char	：	帳票番号
//			int				：	指定の数字
//			int				：	ページ番号
// 返送値	BOOL			：	TRUE=指定の数字以上
//-----------------------------------------------------------------------------
void CPrnPrint_EX2::BusuCheckAndPut( unsigned char pPno, int pSno, int pPage/*=1*/ )
{
	bool			brv=false;
	bool			rsw=false;
	int				ii=0,jj=0;
	DIAGRAM_DATA	dd;

	// ＯＣＲ用紙は部数指定不可のため "TRUE" を返す
	if(!(m_pSnHeadData->Sn_TYPE&0x01))	{
		brv = true;
	}
	else	{
		for(ii=0; ; ii++)	{
			if(LstBusuIdxGen2[ii].dsp_no == -1)	{
				break;
			}
			for(jj=0; ; jj++)	{
				if(LstBusuIdxGen2[ii].prt_no[jj] == -1)	{
					break;
				}
				if(LstBusuIdxGen2[ii].prt_no[jj] == (int)pPno)	{
					if(m_pPrintCmInfo->busu[GetBusuNo(LstBusuIdxGen2[ii].dsp_no)] >= pSno)	{
						brv = true;
					}
					rsw = true;
					break;
				}
				if(rsw == true)	{
					break;
				}
			}
		}
	}
	//return(brv);
	if(brv == true)	{
		m_pPRtableEx->Add(pPno);
		m_pPGtable->Add(pPage);
		if(pSno == 1)	{
			m_pPRtableEx1->Add(pPno);
		}
	}
}

//--------------------------------------------------------------------------------------------
// 課税取引金額計算表
//--------------------------------------------------------------------------------------------
// 引　数	unsigned char	：	帳票番号
//			int				：	指定の数字
//			int				：	ページ番号
// 返送値	なし
//-----------------------------------------------------------------------------
void CPrnPrint_EX2::BusuCheckAndPut3( unsigned char pPno, int pSno )
{
	int			key=0;

	// 課税売上高計算表
	if(pPno == 0x10)		key=3;
	// 課税仕入高計算表 
	else if(pPno == 0x11)	key=4;
	// 課税取引金額計算表　事業所得用 
	else if(pPno == 0x12)	key=0;
	// 課税取引金額計算表　不動産所得用 
	else if(pPno == 0x13)	key=2;
	// 課税取引金額計算表　農業所得用
	else if(pPno == 0x14)	key=1;
	// 事業別売上高
	else if(pPno == 0x52)	key=4;

	// CMapに保存された
	::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
	m_pMapPrnEXSub_KAZ.Lookup(key, m_PrnEXSub_KAZ);
	if(m_PrnEXSub_KAZ.check != 0)	{
// 24/03/22_411 cor -->
		//m_pPRtableEx->Add(pPno);
		//if(pSno == 1) {
		//	m_pPRtableEx1->Add(pPno);
		//}
// --------------------
		// 部数のチェック
// 24/07/10_個別タブから出力 cor -->
		//int				ii=0,jj=0;
		//bool			rsw=false;
		//bool			brv=false;
		//for(ii=0; ; ii++)	{
		//	if(LstBusuIdxGen2[ii].dsp_no == -1)	{
		//		break;
		//	}
		//	for(jj=0; ; jj++)	{
		//		if(LstBusuIdxGen2[ii].prt_no[jj] == -1)	{
		//			break;
		//		}
		//		if(LstBusuIdxGen2[ii].prt_no[jj] == (int)pPno)	{
		//			if(m_pPrintCmInfo->busu[GetBusuNo(LstBusuIdxGen2[ii].dsp_no)] >= pSno)	{
		//				brv = true;
		//			}
		//			rsw = true;
		//			break;
		//		}
		//		if(rsw == true)	{
		//			break;
		//		}
		//	}
		//}
// ---------------------------------
		bool			brv=false;
		if(m_PrnEXSub_KAZ.busu >= pSno)	{
			brv = true;
		}
// 24/07/10_個別タブから出力 cor <--

		if(brv == true)	{
			m_pPRtableEx->Add(pPno);
// 24/04/03_開発員3 add -->
			m_pPGtable->Add(0);
// 24/04/03_開発員3 add <--
			if(pSno == 1) {
				m_pPRtableEx1->Add(pPno);
			}
		}
// 24/03/22_411 cor <--
	}
}

// 24/03/18_税務代理書面 add -->
//-----------------------------------------------------------------------------
// 税務代理書面
//-----------------------------------------------------------------------------
// 引　数	unsigned char	：	帳票番号
//			int				：	指定の数字
//			int				：	ページ番号
// 返送値	なし
//-----------------------------------------------------------------------------
void CPrnPrint_EX2::BusuCheckAndPut4( unsigned char pPno, int pSno )
{
	int			key=0;

// 24/04/03_checkon2 cor -->
	//// 税務代理権限証書
	//if(pPno == 0x80)		key=0;
	//// 税理士法第33条の2第1項に規定する添付書面（第1面
	//else if(pPno == 0x81)	key=1;
	//// 税理士法第33条の2第1項に規定する添付書面（第2面）
	//else if(pPno == 0x82)	key=2;
	//// 税理士法第33条の2第1項に規定する添付書面（第3面）
	//else if(pPno == 0x83)	key=3;
	//// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
	//else if(pPno == 0x84)	key=4;
	//// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
	//else if(pPno == 0x85)	key=5;
	//// 税理士法第33条の2第2項に規定する添付書面（第1面）
	//else if(pPno == 0x86)	key=6;
	//// 税理士法第33条の2第2項に規定する添付書面（第2面）
	//else if(pPno == 0x87)	key=7;
	//// 税理士法第33条の2第2項に規定する添付書面（第3面）
	//else if(pPno == 0x88)	key=8;
	//// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
	//else if(pPno == 0x89)	key=9;
	//// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
	//else if(pPno == 0x90)	key=10;
// -------------------------
	int									yflg=0,yno=0;
	int									flg=0;
	CArray<TAXAGENT_STFF,TAXAGENT_STFF>	car;
	// 呼び出し元業務
	if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yflg=1;	// 消費税申告書（非連動）
	else										yflg=0;	// 消費税申告書

	// 税務代理権限証書
	if(pPno == 0x80)	{
		yno = 0x01;
		key=0;
	}
	// 税理士法第33条の2第1項に規定する添付書面（第1面
	else if(pPno == 0x81)	{
		yno = 0x02;
		key=1;
	}
	// 税理士法第33条の2第1項に規定する添付書面（第2面）
	else if(pPno == 0x82)	{
		yno = 0x04;
		key=2;
	}
	// 税理士法第33条の2第1項に規定する添付書面（第3面）
	else if(pPno == 0x83)	{
		yno = 0x08;
		key=3;
	}
	// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
	else if(pPno == 0x84)	{
		yno = 0x10;
		key=4;
	}
	// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
	else if(pPno == 0x85)	{
		yno = 0x20;
		key=5;
	}
	// 税理士法第33条の2第2項に規定する添付書面（第1面）
	else if(pPno == 0x86)	{
		yno = 0x40;
		key=6;
	}
	// 税理士法第33条の2第2項に規定する添付書面（第2面）
	else if(pPno == 0x87)	{
		yno = 0x80;
		key=7;
	}
	// 税理士法第33条の2第2項に規定する添付書面（第3面）
	else if(pPno == 0x88)	{
		yno = 0x100;
		key=8;
	}
	// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
	else if(pPno == 0x89)	{
		yno = 0x200;
		key=9;
	}
	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
	else if(pPno == 0x90)	{
		yno = 0x400;
		key=10;
	}
	// 選択不可（背景グレー）の帳票も内部的にチェックがONになっている場合が
	// あるのでデータが無ければ書き込まないようにする
	if(ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this) == 0)	{
		return;
	}
// 24/04/03_checkon2 cor <--

	// CMapに保存された
	::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
	m_pMapPrnEXSub_ZEI.Lookup(key, m_PrnEXSub_ZEI);
	if(m_PrnEXSub_ZEI.check != 0)	{
// 24/03/22_411 cor -->
		//m_pPRtableEx->Add(pPno);
		//if(pSno == 1) {
		//	m_pPRtableEx1->Add(pPno);
		//}
// --------------------
		// 部数のチェック
// 24/07/10_個別タブから出力 cor -->
		//int				ii=0,jj=0;
		//bool			rsw=false;
		//bool			brv=false;
		//for(ii=0; ; ii++)	{
		//	if(LstBusuIdxGen2[ii].dsp_no == -1)	{
		//		break;
		//	}
		//	for(jj=0; ; jj++)	{
		//		if(LstBusuIdxGen2[ii].prt_no[jj] == -1)	{
		//			break;
		//		}
		//		if(LstBusuIdxGen2[ii].prt_no[jj] == (int)pPno)	{
		//			if(m_pPrintCmInfo->busu[GetBusuNo(LstBusuIdxGen2[ii].dsp_no)] >= pSno)	{
		//				brv = true;
		//			}
		//			rsw = true;
		//			break;
		//		}
		//		if(rsw == true)	{
		//			break;
		//		}
		//	}
		//}
// ---------------------------------
		bool			brv=false;
		if(m_PrnEXSub_ZEI.busu >= pSno)	{
			brv = true;
		}
// 24/07/10_個別タブから出力 cor <--

		if(brv == true)	{
			m_pPRtableEx->Add(pPno);
// 24/04/03_開発員3 add -->
			m_pPGtable->Add(0);
// 24/04/03_開発員3 add <--
			if(pSno == 1) {
				m_pPRtableEx1->Add(pPno);
			}
		}
// 24/03/22_411 cor <--
	}
}
// 24/03/18_税務代理書面 add <--

// 24/03/22_背景色 add -->
// ---------------------------------------------------------------------
// 課税取引金額計算、特定収入計算表、税務代理書面 の背景色を変更
// ---------------------------------------------------------------------
// 【引　数】	short			インデックス
//				DIAGRAM_DATA*	ダイアグラム情報
// 【戻り値】	なし
// ---------------------------------------------------------------------
// 24/03/25_168691 cor -->
//void CPrnPrint_EX2::BackColorControl( short pInd, DIAGRAM_DATA* pData )
// -----------------------
void CPrnPrint_EX2::BackColorControl( short pInd, DIAGRAM_DATA* pData/*=NULL*/ )
// 24/03/25_168691 cor <--
{
	DIAGRAM_ATTRIBUTE	da;
	int					sw=0;
	bool				brv=true;

	diag_getattr(IDC_ICSDIAG8CTRL2,pInd,&da,CIcsdiagctrl);
// 24/03/26_168701 add -->
	if(da.attr_bcolor == BC_GRAY)	{
		return;
	}
// 24/03/26_168701 add <--
	// チェックが外れた場合は背景色を元に戻す
// 24/03/25_168691 cor -->
	//if(pData->data_check == 0)	{
// -----------------------
	short	chk=0;
	if(pData != NULL)	{
		chk = pData->data_check;
	}
	else	{
		DIAGRAM_DATA		dd;
		m_Diag2.GetData(pInd,(LPUNKNOWN)&dd);
		chk = dd.data_check;
	}
	if(chk == 0)	{
// 24/03/25_168691 cor <--
		da.attr_bcolor = BC_WHITE;
	}
	// チェックが付いた場合、選択可能なチェックがすべてＯＮか調べる
	else	{
		if(pInd == CHEK12)		sw=0;
		else if(pInd == CHEK13)	sw=1;
		else					sw=2;
// 24/04/03_checkon cor -->
		//if(m_pParent != NULL)	{
		//	brv = ((CPrnReportDlg*)m_pParent)->IsAllCheckOn(sw);
		//}
		//if(brv == true)	{
		//	da.attr_bcolor = BC_WHITE;
		//}
		//else	{
		//	da.attr_bcolor = BC_GREEN_L;
		//}
// ------------------------
		// 申告書出力設定でチェックをOFF→ONにした場合は
		// 個別設定のチェックをすべてONにする
		if(m_pParent != NULL)	{
			((CPrnReportDlg*)m_pParent)->AllCheckOn(sw);
			brv = ((CPrnReportDlg*)m_pParent)->IsAllCheckOn(sw);
		}
		if(brv == true)	{
			da.attr_bcolor = BC_WHITE;
		}
		else	{
			da.attr_bcolor = BC_GREEN_L;
		}
// 24/04/03_checkon cor <--
	}
	diag_setattr(IDC_ICSDIAG8CTRL2,pInd,&da,TRUE,CIcsdiagctrl);
}

// --------------------------------------------------------------------------------
// 課税取引金額計算、特定収入計算表、税務代理書面 の背景色を変更
// （各個別設定タブから呼ばれる）
// --------------------------------------------------------------------------------
// 【引　数】	int		0:課税取引金額計算表　1:特定収入計算表　2:税務代理書面
// 【戻り値】	なし
// --------------------------------------------------------------------------------
void CPrnPrint_EX2::BackColorControl2( int pSw )
{
	DIAGRAM_DATA		dd;
	DIAGRAM_ATTRIBUTE	da;
	int					sw=0;
	short				ind=0;
	bool				brv=true;

	dd.data_imgdata = NULL;
	if(pSw == 0)		ind=CHEK12;
	else if(pSw == 1)	ind=CHEK13;
	else				ind=CHEK14;
	m_Diag2.GetData(ind,(LPUNKNOWN)&dd);
	diag_getattr(IDC_ICSDIAG8CTRL2,ind,&da,CIcsdiagctrl);
	if(da.attr_bcolor != BC_GRAY)	{
		// チェックが外れている場合は背景色を元に戻す
		if(dd.data_check == 0)	{
			da.attr_bcolor = BC_WHITE;
		}
		// チェックが付いている場合、選択可能なチェックがすべてＯＮか調べる
		else	{
// 24/04/01_168704 cor -->
			//brv = ((CPrnReportDlg*)m_pParent)->IsAllCheckOn(pSw);
// -----------------------
			if(m_pParent != NULL)	{
				brv = ((CPrnReportDlg*)m_pParent)->IsAllCheckOn(pSw);
			}
// 24/04/01_168704 cor <--
			if(brv == true)	{
				da.attr_bcolor = BC_WHITE;
			}
			else	{
				da.attr_bcolor = BC_GREEN_L;
			}
		}
		diag_setattr(IDC_ICSDIAG8CTRL2,ind,&da,TRUE,CIcsdiagctrl);
	}
}
// 24/03/22_背景色 add <--
