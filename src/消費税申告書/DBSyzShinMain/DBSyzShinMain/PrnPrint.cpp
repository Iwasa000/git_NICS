// PrnPrint.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "PrnReportDlg.h"
#include "PrnPrint.h"

#include "PrnPrintIdx.h"

// CPrnPrint ダイアログ

IMPLEMENT_DYNAMIC(CPrnPrint, ICSDialog)

CPrnPrint::CPrnPrint(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnPrint::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_swBeForked(0)
	, m_PrselState(ID_SEP_PRINT_SEL_STATE)
{

}

CPrnPrint::~CPrnPrint()
{
}

void CPrnPrint::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Diag2);
	DDX_Control(pDX, IDC_CHECK_ALLOUT, m_AlloutCheck);
}


BEGIN_MESSAGE_MAP(CPrnPrint, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrnPrint::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPrnPrint::OnBnClickedButton2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ALLOUT, &CPrnPrint::OnBnClickedCheckAllout)
END_MESSAGE_MAP()


// CPrnPrint メッセージ ハンドラ

BOOL CPrnPrint::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	
	Init();
	m_SelSw = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CPrnPrint::PreTranslateMessage(MSG* pMsg)
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
		if( pMsg->wParam == VK_F9 ) {
			OnBnClickedButton1();
			return TRUE;
		}

		if( pMsg->wParam == VK_F11) {
//			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F11);
			return TRUE;
		}
		if( pMsg->wParam == VK_F12 ) {
			OnBnClickedButton2();
			return TRUE;
		}
		if( pMsg->wParam == VK_F5 ) {
			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F5);		
			return TRUE;
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CPrnPrint::Init()
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

	ChangeColor( IDC_ICSDIAG8CTRL2, 0, 0 );		// 表題
	DIAGRAM_DATA	DD;
	DD.data_imgdata = 0;
	DD.text_combo.Empty();
	if( m_pPrintCmInfo->isColorPrn ){
		// カラープリンター
		// 提出用
		DD.text_combo =  _T("ＫＳＫ用（ＯＣＲ　用紙）\r\n");
		DD.text_combo += _T("提 出 用（カラー　白紙）\r\n");
		DD.text_combo += _T("提 出 用（モノクロ白紙）");
		m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
		// 控用
		DD.data_imgdata = 0;
		DD.text_combo.Empty();
	//	DD.text_combo =  _T("ＫＳＫ用（ＯＣＲ　用紙）\r\n");
		DD.text_combo += _T("控　　用（カラー　白紙）\r\n");
		DD.text_combo += _T("控　　用（モノクロ白紙）");
		m_Diag2.SetData( COMB02, (LPUNKNOWN)&DD );
	}
	else{
		// モノクロプリンター
		// 提出用
		DD.text_combo =  _T("ＫＳＫ用（ＯＣＲ　用紙）\r\n");
		DD.text_combo += _T("提 出 用（モノクロ白紙）");
		m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
		// 控用
		DD.text_combo.Empty();
		DD.text_combo += _T("控　　用（モノクロ白紙）");
		m_Diag2.SetData( COMB02, (LPUNKNOWN)&DD );
		ChangeColor( IDC_ICSDIAG8CTRL2, COMB02, 2 );
		m_Diag2.ModifyItem( COMB02, DIAG_MDFY_READONLY );
	}

	if( m_swBeForked ){
		if( !(m_pSnHeadData->Sn_TYPE&0x01) ) {
			DIAGRAM_DATA	DD;
			DD.data_imgdata = 0;
			DD.data_combo = 1;
			m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
			m_pSnHeadData->Sn_TYPE &= 0xfc;
			m_pSnHeadData->Sn_TYPE |= 0x01;

// 改良依頼 20-1013,19-1184 修正 -->
			// 印刷の用紙設定を「提出用（カラー  白紙）」にする
			// カラー
			m_pSnHeadData->Sn_TYPE |= 0x02;
// 改良依頼 20-1013,19-1184 修正 <--
		}
	}

	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
		DD.data_imgdata = 0;
		DD.text_check = _T("付表５[控除対象仕入税額の計算表]");
		m_Diag2.SetData( CHEK03, (LPUNKNOWN)&DD );
		DD.data_imgdata = 0;
		DD.text_check = _T("付表４[旧・新税率別、消費税額計算表]");
		m_Diag2.SetData( CHEK04, (LPUNKNOWN)&DD );
		DD.data_imgdata = 0;
		DD.text_check = _T("付表５－(２)[控除対象仕入税額等の計算表](1/2)");
		m_Diag2.SetData( CHEK05, (LPUNKNOWN)&DD );
	}
	else{
//--> '15.02.04 CUT START
//		DD.text_check = _T("");
//		DD.data_imgdata = 0;
//		m_Diag2.ModifyItem( CHEK20, DIAG_MDFY_READONLY );
//		m_Diag2.SetData( CHEK20, (LPUNKNOWN)&DD );
//<-- '15.02.04 CUT END
	}

	GetDlgItem(IDC_STATIC_INCOME)->SetWindowText( " " );
//-- '15.04.04 --
//	if( m_pShinInfo->sgSpc ){
//---------------
	if( *m_pShinInfo->pSgSpc ){
//---------------
		if( m_pSnHeadData->Sn_Sign2 & 0x02 ){
			CString	Mes;
			Mes  = _T("特定収入割合が５％以下です。この場合、課税仕入等の税額の\n");
			Mes += _T("調整は不要のため、申告書・付表には計上しません。");
			GetDlgItem(IDC_STATIC_INCOME)->SetWindowText( Mes );
		}
	}
	else{
		if( m_pSnHeadData->IsSoleProprietor() ){
			CString	Mes;
			Mes  = _T("※国税庁から個人事業者の欄外プレプリント印字は記載不要との指導がありましたので、\n");
			Mes += _T("欄外プレプリント印字の出力には対応しておりません。");
			GetDlgItem(IDC_STATIC_INCOME)->SetWindowText( Mes );
		}
	}

	// 特定収入
//	pSyzShin->m_SpcCt1 = 0;							// 計算表21
//	pSyzShin->m_SpcMx1 = 0;							// 〃 
//	pSyzShin->m_SpcTp1 = 0;							// 〃
//	pSyzShin->m_SpcCt2 = 0;							// 計算表22
//	pSyzShin->m_SpcMx2 = 0;							// 〃 
//	pSyzShin->m_SpcTp2 = 0;							// 〃

//--> '15.03.18 INS START
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
//<-- '15.03.18 INS END
//--> '15.02.24 INS START
	else if( m_pPrintCmInfo->OP_PSEL&0x01 ){
		m_pPrintCmInfo->OP_PSEL |= 0x02;
		m_PrselState = ID_ALL_PRINT_SEL_STATE;
		m_AlloutCheck.SetCheck(BST_CHECKED);
		m_AlloutCheck.EnableWindow(FALSE);
	}
	else{
//-- 出力指定を記憶 --
//		if( m_pPrintCmInfo->OP_PSEL&0x02 ){
//			m_PrselState = ID_ALL_PRINT_SEL_STATE;
//			m_AlloutCheck.SetCheck(BST_CHECKED);
//		}
//		else{
//			m_PrselState = ID_SEP_PRINT_SEL_STATE;
//			m_AlloutCheck.SetCheck(BST_UNCHECKED);
//		}
//-- 出力指定を記憶しない --
		m_pPrintCmInfo->OP_PSEL &= ~0x02;
		m_PrselState = ID_SEP_PRINT_SEL_STATE;
		m_AlloutCheck.SetCheck(BST_UNCHECKED);
	}
//<-- '15.02.24 INS END

//--> '15.03.07 INS START
	if( m_pSnHeadData->Sn_TYPE&0x01 ){
		m_AlloutCheck.EnableWindow( TRUE );
	}
	else{
		m_AlloutCheck.EnableWindow( FALSE );
	}
//<-- '15.03.07 INS END

	// 入力項目設定
	SetInputItem();

	// 中間申告第26号様式チェック
	if( m_pSnHeadData->IsMiddleProvisional() ){
		diag_setposition( IDC_ICSDIAG8CTRL2, CHEK06, CIcsdiagctrl );
//★//[13'06.04]///
		m_pSnHeadData->Sn_TYPE = 0x01;
///////////////////
	}
	else{
//-- '15.09.26 --
//		diag_setposition( IDC_ICSDIAG8CTRL2, CHEK01, CIcsdiagctrl );
//---------------
		diag_setposition( IDC_ICSDIAG8CTRL2, CHEK01_1, CIcsdiagctrl );
//---------------
	}
	m_Diag2.SetPosition(COMB01);

	return 0;
}

//-----------------------------------------------------------------------------
// 初期情報セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		pZmSub		：	公益クラス
//		pShinInfo	：	共通情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CPrnPrint::InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo )
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

	return 0;
}

//-----------------------------------------------------------------------------
// 入力項目設定
//-----------------------------------------------------------------------------
void CPrnPrint::SetInputItem()
{
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
		switch( Flg ){
			case 1 :	// 本表・付表・第26号様式
				Flg = 1;
//-- '15.02.04 --
//				if( (pPrnTbl+cnt)->Index == CHEK20 ){
//					if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
//						Flg = 1;
//					}
//					else{
//						Flg = 0;
//					}
//				}
//				else{
//					if( ((pPrnTbl+cnt)->Index==COMB02) && (m_pPrintCmInfo->isColorPrn==FALSE) ){
//						Flg = 0;
//					}
//				}
//---------------
				if( ((pPrnTbl+cnt)->Index==COMB02) && (m_pPrintCmInfo->isColorPrn==FALSE) ){
					Flg = 0;
				}
//---------------
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
/*- '14.01.28 -*/
//					if( pSyzShin->m_Skjrnum ){
//						Flg = 1;
//					}
//					else{
//						Flg = 0;
//					}
/*--------------*/
					Flg = 0;
/*--------------*/
				}
				break;
			case 4 :	// 消費税の還付申告に関する明細書
				if( m_pSnHeadData->IsUseTaxRefund() ){
//-- '15.02.24 --
//					Flg = 1;
//---------------
					if( m_PrselState == ID_ALL_PRINT_SEL_STATE ){
						Flg = 1;
					}
					else{
						if( m_pPrintCmInfo->OP_PSEL&0x01 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
//---------------
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
			case 6 :	// 課税売上高計算表
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					Flg = 1;
//					Flg = 0;
				}
				break;
			case 7 :	// 課税仕入高計算表
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
							Flg = 1;
//							Flg = 0;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 8 :	// 課税取引金額計算表（事業所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x01 ){
							Flg = 1;
//							Flg = 0;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 9 :	// 課税取引金額計算表（不動産所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x02 ){
							Flg = 1;
//							Flg = 0;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 10:	// 課税取引金額計算表（農業所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x04 ){
							Flg = 1;
//							Flg = 0;

						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 11:	// 特定収入計算表
//-- '15.04.04 --
//				if( m_pShinInfo->sgSpc ){
//---------------
				if( *m_pShinInfo->pSgSpc ){
//---------------
//-- '15.03.07 --
//					if( m_pSnHeadData->Sn_Sign2&0x02 ){
//						Flg = 0;
//					}
//					else{
//						Flg = 1;
//					}
//---------------
					if( m_PrselState == ID_ALL_PRINT_SEL_STATE ){
						Flg = 1;
					}
					else{
						if( m_pSnHeadData->Sn_Sign2&0x02 ){
							Flg = 0;
						}
						else{
							Flg = 1;
						}
					}
//---------------
				}
				else{
					Flg = 0;
				}
				break;
			case 12:	// 事業別売上高[13'09.19] 
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 1;	// 事業別売上高[13'09.19]
					}
					else{
						Flg = 0;
					}
				}
				break;
//--> '15.09.26 INS START
			case 13:	// 特定課税仕入れがある場合の課税標準額等の内訳書()
				if( m_pPrintCmInfo->Shrevtype < ID_ICSSH_TKKZSIIRE_REVTYPE ){
					Flg = 0;
				}
				else if( m_pSnHeadData->IsNeedTkkzDisp() ){
					Flg = 1;
				}
				else{
					Flg = 0;
				}
				break;
//<-- '15.09.26 INS END
//2018.03.13 INSERT START
			case 14: //付表６
				if((m_pSnHeadData->IsSoleProprietor())&&(m_pSnHeadData->Sn_Syuusei&0x10)){
					Flg = 1;
				}else{
					Flg = 0;
				}
				break;
//2018.03.13 INSERT END
			default:	// 入力不可
				Flg = 0;
				break;
		}

		if( Flg ){
			// 入力可
			ChangeColor( IDC_ICSDIAG8CTRL2, (pPrnTbl+cnt)->Index, 1 );
			m_Diag2.ModifyItem( (pPrnTbl+cnt)->Index, DIAG_MDFY_EDIT );
			SetInputItemData( (pPrnTbl+cnt)->Index );
		}
		else{
			// 入力不可
			ChangeColor( IDC_ICSDIAG8CTRL2, (pPrnTbl+cnt)->Index, 2 );
			DIAGRAM_DATA dd;
			dd.data_imgdata = NULL;
			dd.data_check = 0;
			m_Diag2.SetData( (pPrnTbl+cnt)->Index, (LPUNKNOWN)&dd);
			GetInputItemData( (pPrnTbl+cnt)->Index );
			m_Diag2.ModifyItem( (pPrnTbl+cnt)->Index, DIAG_MDFY_READONLY );
		}
		(pPrnTbl+cnt)->AllUnConnect = Flg;
		cnt++;
	}
	m_Diag2.Refresh();

}

//-----------------------------------------------------------------------------
// 入力項目設定（サブ）
//-----------------------------------------------------------------------------
int CPrnPrint::SetInputItemData( short idx )
{
	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;
	dd.data_check = 0;
	dd.data_combo = 0;

	switch( idx ){
//-- '15.09.26 --
//		case CHEK01	:
//---------------
		case CHEK01_1:	// 申告書本表（提出用）
//---------------
			if( m_pSnHeadData->Sn_SPECIAL&0x02 ){
				dd.data_check = 1;
			}
			break;
		case COMB01:	// 申告書本表（提出用）
			if( m_pPrintCmInfo->isColorPrn ){
				// カラープリンター
				if( m_pSnHeadData->Sn_TYPE & 0x01 ){
					if( m_pSnHeadData->Sn_TYPE & 0x02 ){
						dd.data_combo = 1;
					}
					else{
						dd.data_combo = 2;
					}
				}
				else{
					dd.data_combo = 0;
				}
			  }
			  else{
			 	// モノクロプリンター
				  if( m_pSnHeadData->Sn_TYPE&0x01 ){
					  dd.data_combo = 1;
				  }
				  else{
					  dd.data_combo = 0;
				  }
			  }
			  break;

//--> '15.09.26 INS START
		case CHEK01_2:	// 特定課税仕入れがある場合の課税標準額等の内訳書
			if( m_pSnHeadData->Sn_Sign3&0x40 ){
				dd.data_check = 1;
			}
			break;	
//<-- '15.09.26 INS END

//-- '15.09.26 --
//		case CHEK02	:
//---------------
		case CHEK02_1:	// 申告書本表（控用）
//---------------
			if( m_pSnHeadData->Sn_SPECIAL&0x04 ){
				dd.data_check = 1;
			}
			break;

		case COMB02 :	// 申告書本表（控用）
			if( m_pPrintCmInfo->isColorPrn ){	
				if( m_pSnHeadData->Sn_TYPE&0x04 ){
					dd.data_combo = 0;
				}
				else{
					dd.data_combo = 1;
				}
			}
			break;

//--> '15.09.26 INS START
		case CHEK02_2 :	// 特定課税仕入れがある場合の課税標準額等の内訳書（控用）
			if( m_pSnHeadData->Sn_Sign3&0x80 ){
				dd.data_check = 1;
			}
			break;
//<-- '15.09.26 INS END

		case CHEK03	: // 付表２・付表５
			if( m_pSnHeadData->Sn_SPECIAL&0x08 ){
				dd.data_check = 1;	
			}
			break;
		case CHEK04	: // 付表１・付表４
			if( m_pSnHeadData->Sn_SPECIAL&0x10 ){
				dd.data_check = 1;
			}
			break;  
		case CHEK05	: // 付表２－２・付表５－２
			if( m_pSnHeadData->Sn_SPECIAL&0x20 ){
				dd.data_check = 1;
			}
			break;  
		case CHEK06	: // 第２６号様式
			if( m_pSnHeadData->Sn_SPECIAL&0x40 ){
				dd.data_check = 1;	
			}
			break;  
/*		case CHEK07	: // 仕入控除税額に関する明細書
			if( m_pSnHeadData->Sn_SPECIAL&0x80 ){
				dd.data_check = 1;	
			}
			break;  
		case CHEK08:// 仕入控除税額に関する明細書（次葉）
			if( m_pSnHeadData->Sn_Sign1&0x01 ){
				dd.data_check = 1;
			}
			break;  
*/		case CHEK09	: // 消費税の還付申告に関する明細書（表面）
			if( m_pSnHeadData->Sn_Sign1&0x02 ){
				dd.data_check = 1;
			}
			break;
		case CHEK10	: // 消費税の還付申告に関する明細書（裏面）
			if( m_pSnHeadData->Sn_Sign1&0x04 ){
				dd.data_check = 1;	
			}
			break;  
		case CHEK11	: // 更正の請求書
			if( m_pSnHeadData->Sn_Sign1&0x10 ){
				dd.data_check = 1;
			}
			break;
		case CHEK12	: // 課税売上高計算表 
			if( m_pSnHeadData->Sn_Sign1&0x20 ){
				dd.data_check = 1;
			}
			break; 
		case CHEK13	: // 課税仕入高計算表  
			if( m_pSnHeadData->Sn_Sign1&0x40 ){
				dd.data_check = 1;	
			}
			break;  
		case CHEK14	: // 課税取引金額計算表　事業所得用
			if( m_pSnHeadData->Sn_Sign1&0x80 ){
				dd.data_check = 1;	
			}
			break;   
		case CHEK15	: // 課税取引金額計算表　不動産所得用
			if( m_pSnHeadData->Sn_Sign3&0x01 ){
				dd.data_check = 1;
			}
			break;  
		case CHEK16	: // 課税取引金額計算表　農業所得用
			if( m_pSnHeadData->Sn_Sign3&0x02 ){
				dd.data_check = 1;
			}
			break;   
		case CHEK17	: // 特定収入計算表
			if( m_pSnHeadData->Sn_Sign3&0x04 ){
				dd.data_check = 1;	
			}
			break; 
		case CHEK18	: // 第２６号様式Ａ４ヨコ控用
			if( m_pSnHeadData->Sn_Sign3&0x08 ){
				dd.data_check = 1;	
			}
			break;  
		case CHEK19 : // 事業別売上高[13'09.19]
			if( m_pSnHeadData->Sn_Sign3&0x10 ){
				dd.data_check = 1;	
			}
			break; 
//--> '15.02.04 CUT START
//		case CHEK20 : 
//			if( m_pSnHeadData->Sn_Sign3&0x20 ){
//				dd.data_check = 1;	
//			}
//			break;  // 付表５－２－２
//<-- '15.02.04 CUT END
//2018.03.13 INSERT START
		case CHEK20 : 
			if( m_pSnHeadData->Sn_Sign4&0x08 ){
				dd.data_check = 1;
			}
			break;
//2018.03.13 INSERT END
		default		: 
			break;
	}
	m_Diag2.SetData( idx, (LPUNKNOWN)&dd);

	return dd.data_check;
}

//-----------------------------------------------------------------------------
// 入力項目取得
//-----------------------------------------------------------------------------
// 引数	idx	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CPrnPrint::GetInputItemData( short idx )
{
	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;
	m_Diag2.GetData( idx, (LPUNKNOWN)&dd);

	switch( idx ){
//-- '15.09.26 --
//		case CHEK01	:
//---------------
		case CHEK01_1 :
//---------------
			m_pSnHeadData->Sn_SPECIAL &= 0xfd;
			if( dd.data_check ){
				m_pSnHeadData->Sn_SPECIAL |= 0x02;
			}
			break;

//--> '15.09.26 INS START
		case CHEK01_2 :
			m_pSnHeadData->Sn_Sign3 &= ~0x40;
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign3 |= 0x40;
			}
			break;
//<-- '15.09.26 INS END

//-- '15.09.26 --
//		case CHEK02	:
//---------------
		case CHEK02_1 :
//---------------
			m_pSnHeadData->Sn_SPECIAL &= 0xfb;
			if( dd.data_check ){
				m_pSnHeadData->Sn_SPECIAL |= 0x04;
			}
			break;

//--> '15.09.26 INS START
		case CHEK02_2 :
			m_pSnHeadData->Sn_Sign3 &= ~0x80;
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign3 |= 0x80;
			}
			break;
//<-- '15.09.26 INS END

		case CHEK03	:
			m_pSnHeadData->Sn_SPECIAL &= 0xf7;
			if( dd.data_check ){
				m_pSnHeadData->Sn_SPECIAL |= 0x08;
			}
			break;
		case CHEK04	:
			m_pSnHeadData->Sn_SPECIAL &= 0xef;
			if( dd.data_check ){
				m_pSnHeadData->Sn_SPECIAL |= 0x10;
			}
			break;
		case CHEK05	:	
			m_pSnHeadData->Sn_SPECIAL &= 0xdf;
			if( dd.data_check ){
				m_pSnHeadData->Sn_SPECIAL |= 0x20;
			}
			break;
		case CHEK06	:	
			m_pSnHeadData->Sn_SPECIAL &= 0xbf;
			if( dd.data_check ){
				m_pSnHeadData->Sn_SPECIAL |= 0x40;
			}
			break;
/*		case CHEK07	:	
			if( m_pSnHeadData->IsUseTaxRefund() ){
				break;	
			}
			m_pSnHeadData->Sn_SPECIAL &= 0x7f;	
			if( dd.data_check ){
				m_pSnHeadData->Sn_SPECIAL |= 0x80;	
			}
			break;
		case CHEK08	:
			if( m_pSnHeadData->IsUseTaxRefund() ){
				break;	
			}
			m_pSnHeadData->Sn_Sign1 &= 0xfe;	
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign1 |= 0x01;	
			}
			break;
*/		case CHEK09	:	
			if( m_pSnHeadData->IsUseTaxRefund() == FALSE ){
				break;	
			}
			m_pSnHeadData->Sn_Sign1 &= 0xfd;	
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign1 |= 0x02;	
			}
			break;	
		case CHEK10	:	
			if( m_pSnHeadData->IsUseTaxRefund() == FALSE ){
				break;
			}
		 	m_pSnHeadData->Sn_Sign1 &= 0xfb; 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign1 |= 0x04;
			}
		 	break;
		case CHEK11	:
			m_pSnHeadData->Sn_Sign1 &= 0xef;
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign1 |= 0x10;
			}
			break;
		case CHEK12	:	
			m_pSnHeadData->Sn_Sign1 &= 0xdf;
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign1 |= 0x20;
			}
			break;
		case CHEK13	: 	
			m_pSnHeadData->Sn_Sign1 &= 0xbf; 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign1 |= 0x40;
			}
		 	break;
		case CHEK14	:	
			m_pSnHeadData->Sn_Sign1 &= 0x7f;	 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign1 |= 0x80;
			}
		 	break;
		case CHEK15	:	
			m_pSnHeadData->Sn_Sign3 &= 0xfe; 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign3 |= 0x01;
			}
		 	break;	
		case CHEK16	:
			m_pSnHeadData->Sn_Sign3  &= 0xfd; 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign3 |= 0x02;
			}
		 	break;
		case CHEK17	:
			m_pSnHeadData->Sn_Sign3  &= 0xfb; 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign3 |= 0x04;
			}
			break;
		case CHEK18	:	
			m_pSnHeadData->Sn_Sign3  &= 0xf7; 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign3 |= 0x08;
			}
			break;
		case CHEK19	:
			m_pSnHeadData->Sn_Sign3  &= 0xef; 
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign3 |= 0x10;
			}
			break;
//--> '15.02.04 CUT START
//		case CHEK20	:
//			m_pSnHeadData->Sn_Sign3  &= 0xdf; 
//			if( dd.data_check ){
//				m_pSnHeadData->Sn_Sign3 |= 0x20;
//			}
//			break;// 付表５－２－２
//<-- '15.02.04 CUT END
//2018.03.13 INSERT START
		case CHEK20 :
			m_pSnHeadData->Sn_Sign4  &= 0xf7;
			if( dd.data_check ){
				m_pSnHeadData->Sn_Sign4 |= 0x08;
			}
			break;
//2018.03.13 INSERT END
		default		:
			break;
	}

	return dd.data_check;
}

//-----------------------------------------------------------------------------
// 印刷用情報のセット
//-----------------------------------------------------------------------------
int CPrnPrint::SetPrintInfo()
{
	// 初期設定
	char	work[6] = {0};
	memset( m_pPrintCmInfo->PRtable, '\0', sizeof(m_pPrintCmInfo->PRtable) );
	m_pPrintCmInfo->PRtype = 0x00;

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
	int Put = 0;
	while( 1 ){
		if( (pPrnTbl+cnt)->Index == 0 ){
			break;
		}
		if( ((pPrnTbl+cnt)->Index==COMB01) || ((pPrnTbl+cnt)->Index==COMB02) ){
			cnt++;
			continue;
		}

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
				if( ((pPrnTbl+cnt)->Index==COMB02) && (m_pPrintCmInfo->isColorPrn==0) ){
					Flg = 0;
				}
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
/*- '14.02.02 -*/
//					if( pSyzShin->m_Skjrnum ){
//						Flg = 1;
//					}
//					else{
//						Flg = 0;
//					}
/*-------------*/
					ASSERT( !_T("有り得ない！") );
/*-------------*/
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
			case 6 :	// 課税売上高計算表
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					Flg = 1;
				}
				break;
			case 7 :	// 課税仕入高計算表
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst( m_pZmSub->zvol) ){
//					if( pSyzShin->KniFlg&0x01 ){
//					if( 0 ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 8 :	// 課税取引金額計算表（事業所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst( m_pZmSub->zvol) ){
//					if( pSyzShin->KniFlg&0x01 ){
//					if( 0 ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x01 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 9 :	// 課税取引金額計算表（不動産所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst( m_pZmSub->zvol) ){
//					if( pSyzShin->KniFlg&0x01 ){
//					if( 0 ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x02 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 10:	// 課税取引金額計算表（農業所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst( m_pZmSub->zvol) ){
//					if( pSyzShin->KniFlg&0x01 ){
//					if( 0 ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x04 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 11:	// 特定収入計算表
//-- '15.04.04 --
//				if( m_pShinInfo->sgSpc ){
//---------------
				if( *m_pShinInfo->pSgSpc ){
//---------------
//-- '15.03.07 --
//					if( m_pSnHeadData->Sn_Sign2 & 0x02){
//						Flg = 0;
//					}
//					else{
//						Flg = 1;
//					}
//---------------
					if( m_PrselState == ID_ALL_PRINT_SEL_STATE ){
						Flg = 1;
					}
					else{
						if( m_pSnHeadData->Sn_Sign2&0x02 ){
							Flg = 0;
						}
						else{
							Flg = 1;
						}
					}
//---------------
				}
				else{
					Flg = 0;
				}
				break;
			case 12:	// 事業別売上高[13'09.19] 
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst( m_pZmSub->zvol) ){
//					if( pSyzShin->KniFlg&0x01 ){
//					if( 0 ){
						Flg = 1;	// 事業別売上高[13'09.19]
					}
					else{
						Flg = 0;
					}
				}
				break;
//--> '15.09.26 INS START
			case 13:	// 特定課税仕入れがある場合の課税標準額等の内訳書()
				if( m_pPrintCmInfo->Shrevtype < ID_ICSSH_TKKZSIIRE_REVTYPE ){
					Flg = 0;
				}
				else if( m_pSnHeadData->IsNeedTkkzDisp() ){
					Flg = 1;
				}
				else{
					Flg = 0;
				}
				break;
//<-- '15.09.26 INS END
//2018.03.13 INSERT START
			case 14:	//付表６
				if((m_pSnHeadData->IsSoleProprietor())&&(m_pSnHeadData->Sn_Syuusei&0x10)){
					Flg = 1;
				}else{
					Flg = 0;
				}
				break;
//2018.03.13 INSERT END
			default:	// 入力不可
				Flg = 0;
				break;
		}

//--> '15.03.07 INS START
		char	moji[64]={0};
		CArithEx	Arith;
		Arith.l_defn( 0x16 );
//<-- '15.03.07 INS END

		if( Flg ){
			// 入力可
			switch( (pPrnTbl+cnt)->Index ){
//-- '15.09.26 --
//				case CHEK01	:	// 申告書本表
//---------------
				case CHEK01_1:	// 申告書本表
//---------------
					if( m_pSnHeadData->Sn_SPECIAL&0x02 ){
						m_pPrintCmInfo->PRtable[Put] = 0x01;
						Put++;
//--> '15.09.26 CUT START
////2015.08.04 INSERT START(DEBUG)
//						if( m_pPrintCmInfo->Shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//							m_pPrintCmInfo->PRtable[Put] = 0x57;
//							Put++;
//						}
////2015.08.04 INSERT END(DEBUG)
//<-- '15.09.26 CUT END
					}
					break;	

//--> '15.09.26 INS START
				case CHEK01_2:	// 特定課税仕入れがある場合の課税標準額等の内訳書
					if( m_pSnHeadData->Sn_Sign3&0x40 ){
						m_pPrintCmInfo->PRtable[Put] = 0x57;
						Put++;
					}
					break;	
//<-- '15.09.26 INS END

//-- '15.09.26 --
//				case CHEK02	:	// 申告書本表
//---------------
				case CHEK02_1:	// 申告書控用
//---------------
					if( m_pSnHeadData->Sn_SPECIAL&0x04 ){
						m_pPrintCmInfo->PRtable[Put] = 0x02;
						Put++;
					}
					break;

//--> '15.09.26 INS START
				case CHEK02_2:	// 特定課税仕入れがある場合の課税標準額等の内訳書(控用)
					if( m_pSnHeadData->Sn_Sign3&0x80 ){
						m_pPrintCmInfo->PRtable[Put] = 0x58;
						Put++;
					}
					break;	
//<-- '15.09.26 INS END

				case CHEK03	: 	// 付表２・付表５
					if( m_pSnHeadData->Sn_SPECIAL&0x08 ){ 
						m_pPrintCmInfo->PRtable[Put] = 0x04;
						Put++;
					}	
					break;
				case CHEK04	: 	// 付表１・付表４
					if( m_pSnHeadData->Sn_SPECIAL&0x10 ){
						m_pPrintCmInfo->PRtable[Put] = 0x03; 
						Put++;
					}	
					break;  
				case CHEK05	: 	// 付表２－２・付表５－２
					if( m_pSnHeadData->Sn_SPECIAL&0x20 ){
						m_pPrintCmInfo->PRtable[Put] = 0x05; 
						Put++;
					}	
					break;  
				case CHEK06	: 	// 第２６号様式
					if( m_pSnHeadData->Sn_SPECIAL&0x40 ){
						m_pPrintCmInfo->PRtable[Put] = 0x50;
						Put++;	
					}	
					break;	
/*				case CHEK07	: 	// 仕入控除税額に関する明細書
					if( m_pSnHeadData->Sn_SPECIAL&0x80 ){
						m_pPrintCmInfo->PRtable[Put] = 0x06;
						Put++;	
					}	
					break;	
				case CHEK08	: 	// 仕入控除税額に関する明細書（次葉）
					if( m_pSnHeadData->Sn_Sign1 & 0x01 ){													
/*- '14.02.02 -*/
//						if( m_pSnHeadData->IsUseTaxRefund() == FALSE ){
//							pSyzShin->PRtable[Put] = 0x20;
//							Put++; 
//							pSyzShin->m_TopPage = Put;
//							pSyzShin->m_MaxPage = ( pSyzShin->m_Skjrnum / 27 );
//							if( pSyzShin->m_Skjrnum % 27 ){
//								pSyzShin->m_MaxPage++;
//							}
//							for( int i = 1; i != pSyzShin->m_MaxPage; i++ ) {
//								pSyzShin->PRtable[Put] = 0x20;
//								Put++;
//							}
//						}
/*-------------*/
//						ASSERT( !_T("有り得ない！") );
/*-------------*/
/*				  	}
				  	break;
*/				case CHEK09	: // 消費税の還付申告に関する明細書（表面）
					if( m_pSnHeadData->Sn_Sign1 & 0x02 ){ 
						m_pPrintCmInfo->PRtable[Put] = 0x07; 
						Put++;
					}	
					break;  
				case CHEK10	: // 消費税の還付申告に関する明細書（裏面）
					if( m_pSnHeadData->Sn_Sign1 & 0x04 ){
						m_pPrintCmInfo->PRtable[Put] = 0x08; 
						Put++; 
					}	
					break;  
				case CHEK11	: // 更正の請求書
					if( m_pSnHeadData->Sn_Sign1 & 0x10 ){
						m_pPrintCmInfo->PRtable[Put] = 0x09; 
						Put++; 
					}	
					break;	
				case CHEK12	: // 課税売上高計算表
					if( m_pSnHeadData->Sn_Sign1 & 0x20 ){
						m_pPrintCmInfo->PRtable[Put] = 0x10; 
						Put++;
					}	
					break;  
				case CHEK13	: 	// 課税仕入高計算表 
					if( m_pSnHeadData->Sn_Sign1 & 0x40 ){ 
						m_pPrintCmInfo->PRtable[Put] = 0x11; 
						Put++;
					}	
					break;  
				case CHEK14	: // 課税取引金額計算表　事業所得用 
					if( m_pSnHeadData->Sn_Sign1 & 0x80 ){
						m_pPrintCmInfo->PRtable[Put] = 0x12;
						Put++;
					}	
					break;
				case CHEK15	:	// 課税取引金額計算表　不動産所得用
					if( m_pSnHeadData->Sn_Sign3 & 0x01 ){
						m_pPrintCmInfo->PRtable[Put] = 0x13;
						Put++; 
					}	
					break;
				case CHEK16	: 	// 課税取引金額計算表　農業所得用 
					if( m_pSnHeadData->Sn_Sign3 & 0x02 ){ 
						m_pPrintCmInfo->PRtable[Put] = 0x14;
						Put++; 
					}	
					break;

				case CHEK17	: // 特定収入計算書
					if( m_pSnHeadData->Sn_Sign3 & 0x04 ){													
						// 計算表10
						m_pPrintCmInfo->PRtable[Put] = 0x40; 
						Put++;
						// 計算表21（次葉）
//2015.03.12 DELETE START
						//m_pPrintCmInfo->PRtable[Put] = 0x41;
						//Put++;
//2015.03.12 DELETE START
/*						if( pSyzShin->TsTbl.tp_rnum ){
							pSyzShin->m_SpcTp1 = Put;
							pSyzShin->m_SpcMx1 = ( pSyzShin->TsTbl.tp_rnum / 16 );
							if( pSyzShin->TsTbl.tp_rnum % 16 ){
								pSyzShin->m_SpcMx1++;
							}
							for( int i=1; i!=pSyzShin->m_SpcMx1; i++ ){
								m_pPrintCmInfo->PRtable[Put] = 0x41;
								Put++;
							}
						}*/
//2015.03.12 INSERT START
						CTaxationListView	*pTaxlistView = NULL;
						pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(3));

//2017.03.01 INSERT START
						if( pTaxlistView->IsKeikaType_Keisansyo() == TRUE ){
							if( pTaxlistView->GetMxList21Ex2() ){
								m_pPrintCmInfo->SpcTp1 = Put+1;

								m_pPrintCmInfo->SpcMx1 = pTaxlistView->GetMxList21Ex2();
								for( int i=0; i!=m_pPrintCmInfo->SpcMx1; i++ ){
									m_pPrintCmInfo->PRtable[Put] = 0x41;
									Put++;
								}
							}

						}else{
//2017.03.01 INSERT END
						if( pTaxlistView->GetMxList21Ex() ){
							m_pPrintCmInfo->SpcTp1 = Put+1;

							m_pPrintCmInfo->SpcMx1 = pTaxlistView->GetMxList21Ex();
							for( int i=0; i!=m_pPrintCmInfo->SpcMx1; i++ ){
								m_pPrintCmInfo->PRtable[Put] = 0x41;
								Put++;
							}
						}
//2017.03.01 INSERT START
						}
//2017.03.01 INSERT END
//2015.03.12 INSERT END
						if( m_pSnHeadData->IsKobetuSiireAnbun() ){
							// 計算表22（次葉）
//2015.03.12 UPDATE START
//							m_pPrintCmInfo->PRtable[Put] = 0x42;
//							Put++;	
///*							if( pSyzShin->TsTbl.tp_rnum ){
//								pSyzShin->m_SpcTp2 = Put;
//								pSyzShin->m_SpcMx2 = ( pSyzShin->TsTbl.tp_rnum / 16 );
//								if( pSyzShin->TsTbl.tp_rnum % 16 ){
//									pSyzShin->m_SpcMx2++;
//								}
//								for( int i=1; i!=pSyzShin->m_SpcMx2; i++ ){
//									m_pPrintCmInfo->PRtable[Put] = 0x42;
//									Put++;
//								}
//							}*/
							CTaxationListView	*pTaxlistView = NULL;
							pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(3));

							//m_pPrintCmInfo->PRtable[Put] = 0x56;
							//Put++;

//--- '15.04.25 ---
//							if( pTaxlistView->GetMxList22_8per() ){
//								m_pPrintCmInfo->SpcTp2_8per = Put+1;
//
//								m_pPrintCmInfo->SpcMx2_8per = pTaxlistView->GetMxList22_8per();
//								for( int i=0; i!=m_pPrintCmInfo->SpcMx2_8per; i++ ){
//									m_pPrintCmInfo->PRtable[Put] = 0x56;
//									Put++;
//								}
//							}
//
//							pTaxlistView->MakePageManage22_8per();
//
//							if(m_pSnHeadData->SVmzsw == 0x01){
//								//m_pPrintCmInfo->PRtable[Put] = 0x42;
//								//Put++;
//
//								if( pTaxlistView->GetMxList22_5per() ){
//									m_pPrintCmInfo->SpcTp2_5per = Put+1;
//
//									m_pPrintCmInfo->SpcMx2_5per = pTaxlistView->GetMxList22_5per();
//									for( int i=0; i!=m_pPrintCmInfo->SpcMx2_5per; i++ ){
//										m_pPrintCmInfo->PRtable[Put] = 0x42;
//										Put++;
//									}
//								}
//
//								pTaxlistView->MakePageManage22_5per();
//							}
//-----------------
							// 印刷の順番を5%→8%に変更

//2017.03.01 INSERT START
							if((pTaxlistView->IsKeikaType_Keisansyo() == TRUE)&&(m_pSnHeadData->SVmzsw == 0x01)){
								if( pTaxlistView->GetMxList22_keika() ){
									m_pPrintCmInfo->SpcTp2_keika = Put+1;

									m_pPrintCmInfo->SpcMx2_keika = pTaxlistView->GetMxList22_keika();
									for( int i=0; i!=m_pPrintCmInfo->SpcMx2_keika; i++ ){
										m_pPrintCmInfo->PRtable[Put] = 0x59;
										Put++;
									}
									pTaxlistView->MakePageManage22_keika();
								}
							}else{
//2017.03.01 INSERT END
							if(m_pSnHeadData->SVmzsw == 0x01){
								if( pTaxlistView->GetMxList22_5per() ){
									m_pPrintCmInfo->SpcTp2_5per = Put+1;

									m_pPrintCmInfo->SpcMx2_5per = pTaxlistView->GetMxList22_5per();
									for( int i=0; i!=m_pPrintCmInfo->SpcMx2_5per; i++ ){
										m_pPrintCmInfo->PRtable[Put] = 0x42;
										Put++;
									}
								}
								pTaxlistView->MakePageManage22_5per();
							}

							if( pTaxlistView->GetMxList22_8per() ){
								m_pPrintCmInfo->SpcTp2_8per = Put+1;

								m_pPrintCmInfo->SpcMx2_8per = pTaxlistView->GetMxList22_8per();
								for( int i=0; i!=m_pPrintCmInfo->SpcMx2_8per; i++ ){
									m_pPrintCmInfo->PRtable[Put] = 0x56;
									Put++;
								}
							}
							pTaxlistView->MakePageManage22_8per();
//2017.03.01 INSERT START
							}
//2017.03.01 INSERT END
//-----------------
//2015.03.12 INSERT END
						}
						// 計算表34
						m_pPrintCmInfo->PRtable[Put] = 0x43;
						Put++;

						wsprintf( moji, _T("%d"), m_pSnHeadData->m_UriRatioBorder );
						memset( work, '\0', sizeof(work) );
						Arith.l_input( work, moji );
//						if( Arith.l_cmp(pSyzShin->stval.SpUwri, work) >= 0 ){			// 課税売上割合９５％以上
//2015.03.12 UPDATE START
//						if( m_pSnHeadData->IsRatioOver95() ){							// 課税売上割合９５％以上	これで問題なし？？？？？
							//m_pPrintCmInfo->PRtable[Put] = 0x44;
							//Put++;

						if( pTaxlistView->IsRatioOver95() ){							// 課税売上割合９５％以上
							// 計算表51
//2017.03.01 UPDATE START
							//if(m_pSnHeadData->SVmzsw == 0x01){
							//	m_pPrintCmInfo->PRtable[Put] = 0x44;
							//	Put++;
							//}
							//m_pPrintCmInfo->PRtable[Put] = 0x53;
							//Put++;
							if((pTaxlistView->IsKeikaType_Keisansyo() == TRUE)&&(m_pSnHeadData->SVmzsw == 0x01)){
								m_pPrintCmInfo->PRtable[Put] = 0x60;
								Put++;
							}else{
								if(m_pSnHeadData->SVmzsw == 0x01){
									m_pPrintCmInfo->PRtable[Put] = 0x44;
									Put++;
								}
								m_pPrintCmInfo->PRtable[Put] = 0x53;
								Put++;
							}
//2017.03.01 UPDATE END
//2015.03.12 UPDATE END
						}
						else{															// 課税売上割合９５％未満
							if( m_pSnHeadData->IsKobetuSiireAnbun() == FALSE ){			// 比例配分
								// 計算表53
//2015.03.12 UPDATE START
								//m_pPrintCmInfo->PRtable[Put] = 0x45;
								//Put++;
//2017.03.01 UPDATE START
								//if(m_pSnHeadData->SVmzsw == 0x01){
								//	m_pPrintCmInfo->PRtable[Put] = 0x46;
								//	Put++;
								//}
								//m_pPrintCmInfo->PRtable[Put] = 0x55;
								//Put++;
								if((pTaxlistView->IsKeikaType_Keisansyo() == TRUE)&&(m_pSnHeadData->SVmzsw == 0x01)){
									m_pPrintCmInfo->PRtable[Put] = 0x62;
									Put++;
								}else{
									if(m_pSnHeadData->SVmzsw == 0x01){
										m_pPrintCmInfo->PRtable[Put] = 0x46;
										Put++;
									}
									m_pPrintCmInfo->PRtable[Put] = 0x55;
									Put++;
								}
//2017.03.01 UPDATE END
//2015.03.12 UPDATE END
							}
							else{														// 個別対応
								// 計算表52
//2015.03.12 UPDATE START
								//m_pPrintCmInfo->PRtable[Put] = 0x46;
								//Put++;											
//2017.03.01 UPDATE START
								//if(m_pSnHeadData->SVmzsw == 0x01){
								//	m_pPrintCmInfo->PRtable[Put] = 0x45;
								//	Put++;											
								//}
								//m_pPrintCmInfo->PRtable[Put] = 0x54;
								//Put++;
								if((pTaxlistView->IsKeikaType_Keisansyo() == TRUE)&&(m_pSnHeadData->SVmzsw == 0x01)){
									m_pPrintCmInfo->PRtable[Put] = 0x61;
									Put++;
								}else{
									if(m_pSnHeadData->SVmzsw == 0x01){
										m_pPrintCmInfo->PRtable[Put] = 0x45;
										Put++;											
									}
									m_pPrintCmInfo->PRtable[Put] = 0x54;
									Put++;
								}
//2017.03.01 UPDATE END
//2015.03.12 UPDATE END
							}	
						}
//--> '15.03.23 CUT START
//						// 経過措置チェック
//						if( !(m_pSnHeadData->SVmzsw & 0x01) ){
//							break;
//						}
//						// 計算表60
//						m_pPrintCmInfo->PRtable[Put] = 0x47;
//						Put++;
//<-- '15.03.23 CUT END
					}
					break;

				case CHEK18	: 	// 第２６号様式Ａ4ヨコ控用
					if( m_pSnHeadData->Sn_Sign3 & 0x08 ){ 
						m_pPrintCmInfo->PRtable[Put] = 0x51; 
						Put++; 
					}	
					break;
				case CHEK19	: // 事業別売上高
					if( m_pSnHeadData->Sn_Sign3 & 0x10 ){ 
						m_pPrintCmInfo->PRtable[Put] = 0x52;
						Put++;
					}	
					break;
//--> '15.02.04 CUT START
//				case CHEK20	: // 付表５－２－２
//					if( m_pSnHeadData->Sn_Sign3 & 0x20 ){ 
//						m_pPrintCmInfo->PRtable[Put] = 0x15;
//						Put++;
//					}	
//					break;
//<-- '15.02.04 CUT END
//2018.03.13 INSERT START
				case CHEK20 : // 付表６
					if( m_pSnHeadData->Sn_Syuusei & 0x10 ){													

						CH26HyoView	*pH26View = NULL;
						pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 2 ));

						if( m_pSnHeadData->Sn_Sign4 & 0x08 ){
							m_pPrintCmInfo->Fhy6Tp = Put+1;
							
							m_pPrintCmInfo->Fhy6Mx = pH26View->GetMxListFhyo60();
							for( int i=0; i!=m_pPrintCmInfo->Fhy6Mx; i++ ){
								m_pPrintCmInfo->PRtable[Put] = 0x16;
								Put++;
							}
						}
					}
					break;
//2018.03.13 INSERT END
				default		: 
					break;
			}
		}
		cnt++;
	}
	m_pPrintCmInfo->PgMx = Put;
//	pSyzShin->m_CntPage = 0;

	char	Chk = 0x00;
	char	Dmm = 0x00;
	char	Xymd[4] = { 0x09, 0x04, 0x01, 0x00 };

	if( m_pSnHeadData->IsMiddleProvisional() == FALSE ){

		// 消費税集計
/*		if( Dmm = (pSyzShin->N_ver&0xff) ){		// バージョンチェック
			switch( m_pSnHeadData->Sn_SKKBN&0xff ){
				case 1 : case 3 : case 5 :	// 確定・確定修正・更正の請求
					if( l_jsgn( pSyzShin->Snh.Sn_KDAYE, Xymd, 3 ) < 0 ){
						Dmm = 0; // H9.4ﾌｸﾑ
					}
					break;
				default:				//中間・中間修正
					if( l_jsgn( pSyzShin->Snh.Sn_MDAYE, Xymd, 3 ) < 0 ){
						Dmm = 0; // H9.4ﾌｸﾑ
					}
					break;
			}
		}
		switch( Dmm ){
			case 1 :
				if( !m_pSnHeadData->SVmzsw ){
					Dmm = 2;
				}
				break;
			case 2 :	
				if( m_pSnHeadData->SVmzsw ){
					Dmm = 1;
				}
				break;
			default:
				break;
		}
		int StTs = pSyzShin->stm_pcalq( Dmm, pSyzShin->N_ver );
		if( StTs ){
			ICSMessageBox( _T("消費税集計中に障害が発生し集計できませんでした。"), MB_OK, 0, 0, this  );
		}*/
	}

	return Put;
}

BEGIN_EVENTSINK_MAP(CPrnPrint, ICSDialog)
ON_EVENT(CPrnPrint, IDC_ICSDIAG8CTRL2, 2, CPrnPrint::EditOFFIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CPrnPrint, IDC_ICSDIAG8CTRL2, 1, CPrnPrint::EditONIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CPrnPrint, IDC_ICSDIAG8CTRL2, 3, CPrnPrint::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CPrnPrint, IDC_ICSDIAG8CTRL2, 9, CPrnPrint::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
ON_EVENT(CPrnPrint, IDC_ICSDIAG8CTRL2, 8, CPrnPrint::ComboSelIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CPrnPrint::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	GetInputItemData( index );
}

void CPrnPrint::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CPrnPrint::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char )::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}
	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
		m_Diag2.SetNextPosition();
	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Diag2.SetPrevPosition();
	}
}

void CPrnPrint::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	GetInputItemData( index );
}

void CPrnPrint::ComboSelIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA *pdata = (DIAGRAM_DATA*)data;

	if( index == COMB01 ){
		if( m_swBeForked ){
			if( !pdata->data_combo ){
				ICSMessageBox( "一括印刷処理ではＫＳＫ用（ＯＣＲ用紙）への印刷は出来ません。" );
				DIAGRAM_DATA	DD;
				DD.data_imgdata = 0;
				DD.data_combo = 1;
				m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
//				m_pSnHeadData->Sn_TYPE &= 0xfc;
//				m_pSnHeadData->Sn_TYPE |= 0x01;
				return;
			}
		}
		m_pSnHeadData->Sn_TYPE &= 0xfc;
		if( pdata->data_combo ){
			m_pSnHeadData->Sn_TYPE |= 0x01;
		}
		if( m_pPrintCmInfo->isColorPrn ){
			// カラープリンター
			if( pdata->data_combo == 1 ){
				m_pSnHeadData->Sn_TYPE |= 0x02;
			}
		}
//--> '15.03.07 INS START
		if( m_pSnHeadData->Sn_TYPE&0x01 ){
			m_AlloutCheck.EnableWindow( TRUE );
		}
		else{
			m_AlloutCheck.EnableWindow( FALSE );
		}
//<-- '15.03.07 INS END
		// 入力項目設定
		SetInputItem();
		DIAGRAM_DATA	DD;
		DD.data_imgdata = 0;
		DD.data_combo = pdata->data_combo;
		m_Diag2.SetData( COMB01, (LPUNKNOWN)&DD );
	}
	else if( index == COMB02 ){
		m_pSnHeadData->Sn_TYPE &= 0xfb;
		if( m_pPrintCmInfo->isColorPrn ){
			// カラープリンター
			if( pdata->data_combo == 0 ){
				m_pSnHeadData->Sn_TYPE |= 0x04;
			}
		}
		// 入力項目設定
		if( !m_swBeForked ){
			SetInputItem();
		}
		DIAGRAM_DATA	DD;
		DD.data_imgdata = 0;
		DD.data_combo = pdata->data_combo;
		m_Diag2.SetData( COMB02, (LPUNKNOWN)&DD );
	}
}

//-----------------------------------------------------------------------------
// 全選択＆全解除
//-----------------------------------------------------------------------------
void CPrnPrint::ChoiceItem()
{
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
	if( m_SelSw ){
		m_SelSw = 0;
	}
	else{
		m_SelSw = 1;
	}

	int cnt = 0;
	while( 1 ){
		if( (pPrnTbl+cnt)->Index == 0 ){
			break;
		}
		if( ((pPrnTbl+cnt)->Index==COMB01) || ((pPrnTbl+cnt)->Index==COMB02) ){
			cnt++;
			continue;
		}
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
				if( ((pPrnTbl+cnt)->Index==COMB02) && (m_pPrintCmInfo->isColorPrn==0) ){
					Flg = 0;
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
/*- '14.01.28 -*/
//					if( pSyzShin->m_Skjrnum ){
//						Flg = 1;
//					}
//					else{
//						Flg = 0;
//					}
/*--------------*/
					Flg = 0;
/*--------------*/
				}
				break;
			case 4 :	// 消費税の還付申告に関する明細書
				if( m_pSnHeadData->IsUseTaxRefund() ){
//-- '15.02.24 --
//					Flg = 1;
//---------------
					if( m_PrselState == ID_ALL_PRINT_SEL_STATE ){
						Flg = 1;
					}
					else{
						if( m_pPrintCmInfo->OP_PSEL&0x01 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
//---------------
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
			case 6 :	// 課税売上高計算表
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					Flg = 1;
				}
				break;
			case 7 :	// 課税仕入高計算表
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 8 :	// 課税取引金額計算表（事業所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x01 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 9 :	// 課税取引金額計算表（不動産所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x02 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 10:	// 課税取引金額計算表（農業所得）
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 0;	// 事業別売上高[13'09.19]
					}
					else{
						if( m_pZmSub->zvol->ind_type & 0x04 ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
				}
				break;
			case 11:	// 特定収入計算表
//-- '15.04.04 --
//				if( m_pShinInfo->sgSpc ){
//---------------
				if( *m_pShinInfo->pSgSpc ){
//---------------
//-- '15.03.07 --
//					if( m_pSnHeadData->Sn_Sign2 & 0x02){
//						Flg = 0;
//					}
//					else{
//						Flg = 1;
//					}
//---------------
					if( m_PrselState == ID_ALL_PRINT_SEL_STATE ){
						Flg = 1;
					}
					else{
						if( m_pSnHeadData->Sn_Sign2 & 0x02){
							Flg = 0;
						}
						else{
							Flg = 1;
						}
					}
//---------------
				}
				else{
					Flg = 0;
				}
				break;
			case 12:	// 事業別売上高[13'09.19] 
				if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
					Flg = 0;
				}
				else{
					if( m_Util.IsUnConnectKaniMst(m_pZmSub->zvol) ){
						Flg = 1;	// 事業別売上高[13'09.19]
					}
					else{
						Flg = 0;
					}
				}
				break;
//--> '15.09.26 INS START
			case 13:	// 特定課税仕入れがある場合の課税標準額等の内訳書()
				if( m_pPrintCmInfo->Shrevtype < ID_ICSSH_TKKZSIIRE_REVTYPE ){
					Flg = 0;
				}
				else if( m_pSnHeadData->IsNeedTkkzDisp() ){
					Flg = 1;
				}
				else{
					Flg = 0;
				}
				break;
//2018.03.13 INSERT START
			case 14: //付表６
				if((m_pSnHeadData->IsSoleProprietor())&&(m_pSnHeadData->Sn_Syuusei&0x10)){
					Flg = 1;
				}else{
					Flg = 0;
				}
				break;
//2018.03.13 INSERT END
//<-- '15.09.26 INS END
			default:	// 入力不可
				Flg = 0;
				break;
		}
		if( Flg ){
			DIAGRAM_DATA dd;
			dd.data_imgdata = NULL;
			if( m_SelSw ){
				dd.data_check = 1;
			}
			else{
				dd.data_check = 0;
			}
			m_Diag2.SetData( (pPrnTbl+cnt)->Index, (LPUNKNOWN)&dd );
			GetInputItemData( (pPrnTbl+cnt)->Index );
		}
		cnt++;
	}
}

//-----------------------------------------------------------------------------
// 入力項目色変更
//-----------------------------------------------------------------------------
// 引数	id		：
//		index	：
//		sign	：
//-----------------------------------------------------------------------------
void CPrnPrint::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :
			DA.attr_bcolor = BC_WHITE;	
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
void CPrnPrint::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ChoiceItem();
}

//F12 次画面へ
void CPrnPrint::OnBnClickedButton2()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F12);
}

void CPrnPrint::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

void CPrnPrint::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnCancel();
}



HBRUSH CPrnPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if(GetDlgItem(IDC_STATIC_INCOME) == pWnd)
		pDC->SetTextColor(RGB(255,0,0));
	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CPrnPrint::OnBnClickedCheckAllout()
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
}
