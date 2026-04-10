// midori 180101 add -->
// dlgPrintHyoshi.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgPrintHyoshi.h"

// 色
const	COLORREF	BLACK_CLR = RGB( 0x00, 0x00, 0x00 );
// midori 153784 add -->
extern	int			m_OtherSign;
// midori 153784 add <--

// CdlgPrintHyoshi ダイアログ

IMPLEMENT_DYNAMIC(CdlgPrintHyoshi, ICSDialog)

// -----------------------------------------------------------------------------
//	標準コンストラクタ
// -----------------------------------------------------------------------------
CdlgPrintHyoshi::CdlgPrintHyoshi(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgPrintHyoshi::IDD, pParent)
{
	// 財務dbクラス
	m_pZmSub	= ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
// midori 180104 add -->
	m_ChgFlg = 0;
// midori 180104 add <--
// midori 191201 add -->
// midori 151730,151263 del -->
	//m_ChkReki = 0;
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	m_apno = 0;
	m_ver = 0;
// midori 151730,151263 add <--
	m_RekiSave = 0;
	m_RekiSave2 = 0;
// midori 191201 add <--
}

// -----------------------------------------------------------------------------
//	デストラクタ
// -----------------------------------------------------------------------------
CdlgPrintHyoshi::~CdlgPrintHyoshi()
{
}

void CdlgPrintHyoshi::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HSDE010, m_HSDE010);
	DDX_Control(pDX, IDC_HSCK010, m_HSCK010);
	DDX_Control(pDX, IDC_HSDE020, m_HSDE020);
	DDX_Control(pDX, IDC_HSDE021, m_HSDE021);
	DDX_Control(pDX, IDC_HSDE022, m_HSDE022);
	DDX_Control(pDX, IDC_HSDE023, m_HSDE023);
	DDX_Control(pDX, IDC_HSDE024, m_HSDE024);
	DDX_Control(pDX, IDC_HSDE025, m_HSDE025);
	DDX_Control(pDX, IDC_HSCK020, m_HSCK020);
	DDX_Control(pDX, IDC_HSDE030, m_HSDE030);
	DDX_Control(pDX, IDC_HSDE040, m_HSDE040);
	DDX_Control(pDX, IDC_HSDE041, m_HSDE041);
	DDX_Control(pDX, IDC_HSCB050, m_HSCB050);
// midori 181101 add -->
	DDX_Control(pDX, IDC_HSCB020, m_HSCB020);
	DDX_Control(pDX, IDC_HSCB030, m_HSCB030);
// midori 181101 add <--
	DDX_Control(pDX, ID_HOME_BUTTON, m_btnHome);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CdlgPrintHyoshi, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgPrintHyoshi::OnBnClickedOK)
	ON_BN_CLICKED(ID_HOME_BUTTON, &CdlgPrintHyoshi::OnBnClickedHomeButton)
	ON_CBN_SELCHANGE(IDC_HSCB050, &CdlgPrintHyoshi::OnCbnSelchangeHscb050)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgPrintHyoshi, ICSDialog)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE010, 1, CdlgPrintHyoshi::TerminationHsde010, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE020, 1, CdlgPrintHyoshi::TerminationHsde020, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE021, 1, CdlgPrintHyoshi::TerminationHsde021, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE022, 1, CdlgPrintHyoshi::TerminationHsde022, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE023, 1, CdlgPrintHyoshi::TerminationHsde023, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE024, 1, CdlgPrintHyoshi::TerminationHsde024, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE025, 1, CdlgPrintHyoshi::TerminationHsde025, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE030, 1, CdlgPrintHyoshi::TerminationHsde030, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE040, 1, CdlgPrintHyoshi::TerminationHsde040, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPrintHyoshi, IDC_HSDE041, 1, CdlgPrintHyoshi::TerminationHsde041, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


// CdlgPrintHyoshi メッセージ ハンドラ

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
HBRUSH CdlgPrintHyoshi::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

// -----------------------------------------------------------------------------
//	ダイアログ表示
// -----------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//	【戻値】	ID_DLG_OK		…	OKボタンが押された
//				ID_DLG_CANCEL	…	キャンセルボタンが押された
// -----------------------------------------------------------------------------
int CdlgPrintHyoshi::ShowDialog( CDatabase* pDB )
{
	m_nRet = ID_DLG_CANCEL;		//	戻値初期化

	// データベースハンドルがヌルじゃない？
	if(pDB != NULL)	{
		try	{
			// データベース開いてる？
			if(pDB->IsOpen())	{
				m_pDB = pDB;	//	データベースハンドル取得
				DoModal();		//	モーダルで表示
// midori 180104 add -->
				// OKボタン押下時
				if(m_nRet == ID_DLG_OK)	{
					// 過年度マスターでなく、ダイアログの項目に変更あり
					if( (((CUCHIWAKEApp*)AfxGetApp())->IsKanendoMaster()==FALSE) && m_ChgFlg ){
						if(PutCoinfo() == -1)	{
							// エラー
							m_nRet = ID_DLG_CANCEL;
						}
					}
				}
// midori 180104 add <--
			}
		}
		catch(...)	{
		}
	}

	//	戻値を返す
	return( m_nRet );
}

// -----------------------------------------------------------------------------
//	ダイアログ初期化
// -----------------------------------------------------------------------------
BOOL CdlgPrintHyoshi::OnInitDialog()
{
	// 2010/07/XX 一括印刷対応　ログ
	const CString	FN = _T("CdlgPrintHyoshi::GetPageData()");
	CString			strLog;

	DbgWriteLog( FN, _T("START") );

	ICSDialog::OnInitDialog();

	// 初期設定
	if(InitDisp() != 0)	{
		return(FALSE);
	}

	// リサイズ
	OnInitDialogEX();

	// 2010/07/XX 一括印刷対応　ログ
	DbgWriteLog( FN, _T("END") );

	// 初期フォーカスセット
	m_HSDE010.SetFocus();

	//	戻値を返す
	return( TRUE );
}

// -----------------------------------------------------------------------------
// メッセージ処理
// -----------------------------------------------------------------------------
BOOL CdlgPrintHyoshi::PreTranslateMessage(MSG* pMsg)
{
	int		sw=0;
	CWnd*	pWnd = NULL;

	pWnd = GetFocus();
	sw = 0;
	// キーが押された？
	if( pMsg->message == WM_KEYDOWN )	{
		//	パラメータで分岐
		switch( pMsg->wParam )	{

		case	VK_RETURN:
			if( (pWnd == GetDlgItem(ID_HOME_BUTTON)) )	{
				PostMessage( WM_COMMAND, ID_HOME_BUTTON );
				sw = 1;
			}
			else if( (pWnd == GetDlgItem(ID_OK_BUTTON)))	{
				OnBnClickedOK();
				sw = 1;
			}
			else if( (pWnd == GetDlgItem(IDC_HSCK010)) || (pWnd == GetDlgItem(IDC_HSCK020)) )	{
				NextDlgCtrl();
				sw = 1;
			}
			break;

		case	VK_TAB:
			//if( (pWnd == GetDlgItem(IDC_HSCK010)) || (pWnd == GetDlgItem(IDC_HSCK020)) ||
			//	(pWnd == GetDlgItem(ID_HOME_BUTTON)) || (pWnd == GetDlgItem(ID_OK_BUTTON)) )	{
				if( GetKeyState(VK_SHIFT)&0x8000 ){
					PrevDlgCtrl();
				}
				else{
					NextDlgCtrl();
				}
				sw = 1;
			//}
			break;

		case VK_F2:				//	F2キーが押された
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			sw = 1;
			break;

		case VK_ESCAPE:
			ICSDialog::OnCancel();
			sw = 1;
			break;

		case	VK_HOME:
			if( (pWnd->m_hWnd == m_HSDE040.m_hWnd) || (pWnd->m_hWnd == m_HSDE041.m_hWnd) )	{
				PostMessage( WM_COMMAND, ID_HOME_BUTTON );
				sw = 1;
			}
			break;

		}
	}

	if(sw == 1)	return(1);

	return ICSDialog::PreTranslateMessage(pMsg);
}

// -----------------------------------------------------------------------------
//	初期設定
// -----------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	int		0:正常終了、-1:エラー終了
// -----------------------------------------------------------------------------
int CdlgPrintHyoshi::InitDisp(void)
{
// midori 180104 del -->
	//// 文字入力 Deleteキー
	//m_HSDE030.EnableDelete( 0 );
	//m_HSDE040.EnableDelete( 0 );
	//m_HSDE041.EnableDelete( 0 );
// midori 180104 del <--

	// フレーム
	m_HSDE010.SetFrame( -1, BLACK_CLR );
	m_HSDE020.SetFrame( -1, BLACK_CLR );
	m_HSDE021.SetFrame( -1, BLACK_CLR );
	m_HSDE022.SetFrame( -1, BLACK_CLR );
	m_HSDE023.SetFrame( -1, BLACK_CLR );
	m_HSDE024.SetFrame( -1, BLACK_CLR );
	m_HSDE025.SetFrame( -1, BLACK_CLR );
	m_HSDE030.SetFrame( -1, BLACK_CLR );
	m_HSDE040.SetFrame( -1, BLACK_CLR );
	m_HSDE041.SetFrame( -1, BLACK_CLR );

	// ------------------------------------

	// 共通情報
	m_PrtInfCommon.KikanStart		= NULL;
	m_PrtInfCommon.KikanEnd			= NULL;
// midori 153812 del ------------------>
//	m_PrtInfCommon.KikanEndPage		= NULL;
// midori 153812 del <------------------
	m_PrtInfCommon.Ki				= NULL;
	m_PrtInfCommon.OutKi			= NULL;
	m_PrtInfCommon.OutCoName		= NULL;
	m_PrtInfCommon.OutKikan			= NULL;
	m_PrtInfCommon.OutKingaku		= NULL;
	m_PrtInfCommon.OutPage			= NULL;
	m_PrtInfCommon.CoRekinen		= NULL;
	m_PrtInfCommon.CoName.Empty();
	m_PrtInfCommon.CoAddr1.Empty();
	m_PrtInfCommon.CoAddr2.Empty();
	m_PrtInfCommon.OutGenzai		= NULL;
// midori 181101 add -->
	m_PrtInfCommon.StartGen			= NULL;
	m_PrtInfCommon.EndGen			= NULL;
// midori 181101 add <--
// midori 200101 add -->
	m_PrtInfCommon.OutGengo			= NULL;
// midori 200101 add <--

// midori 180104 add -->
	m_CoName.Empty();	// 起動時の会社名
	m_CoAddr1.Empty();	// 　〃　の住所（上段）
	m_CoAddr2.Empty();	// 　〃　の住所（下段）
// midori 180104 add <--

// midori 151730,151263 add -->
	// アプリケーションNoとマスターバージョンを取得
	m_pZmSub->VolumeOpen();
	m_apno = m_pZmSub->zvol->apno;
	m_ver = m_pZmSub->zvol->m_ver;
	m_pZmSub->VolumeClose();
// midori 151730,151263 add <--

	// データ読込み
	if(ReadData() != 0)	{
		return(-1);
	}

// midori 180104 add -->
	if(m_PrtInfCommon.CoName.IsEmpty() == FALSE)	m_CoName	= m_PrtInfCommon.CoName;
	if(m_PrtInfCommon.CoAddr1.IsEmpty() == FALSE)	m_CoAddr1	= m_PrtInfCommon.CoAddr1;
	if(m_PrtInfCommon.CoAddr2.IsEmpty() == FALSE)	m_CoAddr2	= m_PrtInfCommon.CoAddr2;
// midori 180104 add <--

	// 暦年表示の設定により、年入力の桁数を変更
	// 0:和暦 1:西暦
	if(m_PrtInfCommon.CoRekinen == 0)	{
		m_HSDE020.SetMaxval(_T("99"));
		m_HSDE023.SetMaxval(_T("99"));
	}
	else	{
		m_HSDE020.SetMaxval(_T("9999"));
		m_HSDE023.SetMaxval(_T("9999"));
	}

	// 画面表示
	SetData();

	return(0);
}

// -----------------------------------------------------------------------------
//	データ読込み
// -----------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	int		0:正常終了、-1:エラー終了
// -----------------------------------------------------------------------------
int CdlgPrintHyoshi::ReadData(void)
{
	int				st=0;
	int				symd=0,eymd=0;
// midori 151730,151263 del -->
//// midori 191201 add -->
//	int				nRekinen=0;
//	int				nver=0;
//// midori 191201 add <--
//// midori 153813 add ------------------>
//	BYTE			apno = 0x00;
//// midori 153813 add <------------------
// midori 151730,151263 del <--
	CString			strQuery="";
	CString			strDate="";
	CdbBase			rc( m_pDB );
	CDBVariant		var;
	CdateConvert	dateCvt;
	CDBPrtbl		pt;
	CdbUcInfMain*	pfcRec;				// uc_inf_mainテーブルクラス
	CVolDateDB		icsVolDate;			// DB期間モジュール

// midori 153382 del -->
	//// 決算情報を取得 -----------------------------------------------------------------
	//// 決算書データからデータを取得（一旦、内訳書で使用するすべての項目を取得する）
	//try	{
	//	pt.SetMasterDB( m_pDB );
	//	st = pt.PrfkinOpen("pkofs = 0");		// 当期のみ参照
	//	if(st != 0)	{
	//		return(-1);
	//	}

	//	// -----------------------------------------------------------------------------
	//	// 以下の情報は、決算書ではなく会社情報から連動する
	//	// -----------------------------------------------------------------------------
	//	m_pZmSub->VolumeOpen();
	//	icsVolDate.db_datecnv(m_pZmSub->zvol->ss_ymd,&symd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
	//	icsVolDate.db_datecnv(m_pZmSub->zvol->ee_ymd,&eymd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
	//	m_PrtInfCommon.KikanStart	= symd;		// 決算期間（自）
	//	m_PrtInfCommon.KikanEnd		= eymd;		// 決算表示期間（至）
	//	m_PrtInfCommon.CoRekinen	= 0;		// 暦年表示サイン
	//	m_pZmSub->VolumeClose();

	//	// 期
	//	m_PrtInfCommon.Ki			= pt.prfkin->pkdkin;
	//	// 期出力サイン
	//	if( (pt.prfkin->pkmsw & BIT_D1) != 0 )		m_PrtInfCommon.OutKi = 1;
	//	else										m_PrtInfCommon.OutKi = 0;
	//	//
	//	pt.PrfkinClose();
	//}
	//catch( ... )	{
	//	TRACE( "\n# ERROR: CdlgPrintHyoshi::ReadData()_1\n" );
	//	if( pt.PrfkinOpen() )	{
	//		pt.PrfkinClose();
	//		return(-1);
	//	}
	//}
	//// Owntbテーブルをオープン
	//m_pZmSub->OwnTblOpen();
	//strQuery.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq = 0"), "UCHIWAKE", "HyoshiData");
	//m_pZmSub->owntb->Requery(strQuery);
	//if(m_pZmSub->owntb->GetRecordCount() > 0)	{
	//	// Owntbテーブルからデータを取得
	//	m_PrtInfCommon.KikanStart		= m_pZmSub->owntb->vl[0];			// 決算期間（自）
	//	m_PrtInfCommon.KikanEnd			= m_pZmSub->owntb->vl[1];			// 決算期間（至）
	//	m_PrtInfCommon.CoRekinen		= m_pZmSub->owntb->vl[2];			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
	//}
	//// Owntbテーブルをクローズ
	//m_pZmSub->OwnTblClose();
// midori 153382 del <--
// midori 153382 add -->
	// 決算情報を取得 -----------------------------------------------------------------
	// 決算書データからデータを取得（一旦、内訳書で使用するすべての項目を取得する）
	try	{
		// 決算書情報のテーブルオープン
		pt.SetMasterDB( m_pDB );
		st = pt.PrfkinOpen("pkofs = 0");		// 当期のみ参照
		if(st != 0)	{
			return(-1);
		}
		// Owntbテーブルをオープン
		m_pZmSub->OwnTblOpen();
		strQuery.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq = 0"), "UCHIWAKE", "HyoshiData");
		m_pZmSub->owntb->Requery(strQuery);

// midori 155882 add -->
		m_clsFunc.YmdChg(m_pZmSub,&pt);
// midori 155882 add <--

		// -----------------------------------------------------------------------------

		// 以下の情報は、決算書ではなく会社情報から連動する
// midori 151730,151263 del -->
//		m_pZmSub->VolumeOpen();
//// midori 153812 del ------------------>
////		icsVolDate.db_datecnv(m_pZmSub->zvol->ss_ymd,&symd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
////		icsVolDate.db_datecnv(m_pZmSub->zvol->ee_ymd,&eymd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
////		m_PrtInfCommon.KikanStart	= symd;		// 決算期間（自）
////		m_PrtInfCommon.KikanEnd		= eymd;		// 決算期間（至）
////		m_PrtInfCommon.CoRekinen	= 0;		// 暦年表示サイン
//// midori 153812 del <------------------
//// midori 153813 add ------------------>
//		apno = m_pZmSub->zvol->apno;
//// midori 153813 add <------------------
//// midori 191201 add -->
//		nver = m_pZmSub->zvol->m_ver;
//// midori 191201 add <--
//		m_pZmSub->VolumeClose();
// midori 151730,151263 del <--

		// 「指定期間と連動させる」ONの時
// midori 191201 del -->
		//if((pt.prfkin->pkmsw & BIT_D3) != 0)	{
// midori 191201 del <--
// midori 191201 add -->
// midori 151730,151263 del -->
		//m_ChkReki = m_clsFunc.GetRekinen(m_pZmSub->m_database,apno,nver,&nRekinen);
// midori 151730,151263 del <--
		if((pt.prfkin->pkmsw & BIT_D3) != 0) {
// midori 191201 add <--
			// 期
			m_PrtInfCommon.Ki			= pt.prfkin->pkdkin;

			if( (m_pZmSub->owntb->vl[0] == 0 || m_pZmSub->owntb->vl[1] == 0)
// midori 181101 del -->
//			 || (m_pZmSub->owntb->vl[2] != pt.prfkin->pksdy || m_pZmSub->owntb->vl[3] != pt.prfkin->pkedy))	{
// midori 181101 del <--
// midori 181101 add -->
			 || (m_pZmSub->owntb->vl[6] != pt.prfkin->pkHyGen1 || m_pZmSub->owntb->vl[2] != pt.prfkin->pksdy
			 || m_pZmSub->owntb->vl[7] != pt.prfkin->pkHyGen2 || m_pZmSub->owntb->vl[3] != pt.prfkin->pkedy))	{
// midori 181101 add <--
				// 決算期間（自）
// midori 181101 add -->
				 m_PrtInfCommon.StartGen = pt.prfkin->pkHyGen1;
// midori 181101 add <--
				m_PrtInfCommon.KikanStart	= pt.prfkin->pksdy;
				// 決算期間（至）
// midori 181101 add -->
				 m_PrtInfCommon.EndGen = pt.prfkin->pkHyGen2;
// midori 181101 add <--
				m_PrtInfCommon.KikanEnd		= pt.prfkin->pkedy;
			}
			else	{
				// 決算期間（自）
// midori 181101 add -->
				 m_PrtInfCommon.StartGen = m_pZmSub->owntb->vl[4];
// midori 181101 add <--
				m_PrtInfCommon.KikanStart	= m_pZmSub->owntb->vl[0];
				// 決算期間（至）
// midori 181101 add -->
				 m_PrtInfCommon.EndGen = m_pZmSub->owntb->vl[5];
// midori 181101 add <--
				m_PrtInfCommon.KikanEnd		= m_pZmSub->owntb->vl[1];
			}

			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
// midori 151730,151263 del -->
//// midori 153813 del ------------------>
////			if(pt.prfkin->pkmsw&0x04)				m_PrtInfCommon.CoRekinen = 1;
////			else									m_PrtInfCommon.CoRekinen = 0;
//// midori 153813 del <------------------
//// midori 153813 add ------------------>
//			// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は和暦固定にする
//			if(apno == 0x50 || apno == 0x51 || apno == 0x52 || apno == 0x58 || apno == 0x59)	{
//				m_PrtInfCommon.CoRekinen = 0;
//			}
//			else	{
//				if(pt.prfkin->pkmsw&0x04)				m_PrtInfCommon.CoRekinen = 1;
//				else									m_PrtInfCommon.CoRekinen = 0;
//			}
//// midori 153813 add <------------------
// midori 151730,151263 del <--
// midori 151730,151263 add -->
			if(pt.prfkin->pkmsw&0x04)				m_PrtInfCommon.CoRekinen = 1;
			else									m_PrtInfCommon.CoRekinen = 0;
// midori 151730,151263 add <--
			// 期出力サイン
			if((pt.prfkin->pkmsw & BIT_D1) != 0)	m_PrtInfCommon.OutKi = 1;
			else									m_PrtInfCommon.OutKi = 0;
		}
		// 「指定期間と連動させる」OFFの時
		else	{
			// 期
			m_PrtInfCommon.Ki			= pt.prfkin->pkdkin;
			// 決算表示期間（自）
// midori 181101 add -->
			m_PrtInfCommon.StartGen		= pt.prfkin->pkHyGen1;
// midori 181101 add <--
			m_PrtInfCommon.KikanStart	= pt.prfkin->pksdy;
			// 決算表示期間（至）
// midori 181101 add -->
			m_PrtInfCommon.EndGen		= pt.prfkin->pkHyGen2;
// midori 181101 add <--
			m_PrtInfCommon.KikanEnd		= pt.prfkin->pkedy;
			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
// midori 153813 del ------------------>
//			if(pt.prfkin->pkmsw&0x04)	m_PrtInfCommon.CoRekinen = 1;
//			else						m_PrtInfCommon.CoRekinen = 0;
// midori 153813 del <------------------
// midori 153813 add ------------------>
// midori 191201 del -->
			//// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は和暦固定にする
			//if(apno == 0x50 || apno == 0x51 || apno == 0x52 || apno == 0x58 || apno == 0x59)	{
			//	m_PrtInfCommon.CoRekinen = 0;
// midori 191201 del <--
// midori 191201 add -->
// midori 151730,151263 del -->
			//// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は和暦固定にする
			//if(apno == 0x50 || apno == 0x51 || apno == 0x52 || apno == 0x58 || apno == 0x59)	{
			//	// 下記の場合は和暦固定にする
			//	// ・公益または社会福祉法人の旧会計基準
			//	// ・暦年表示未対応
			//	if(m_ChkReki == 1) {
			//		m_PrtInfCommon.CoRekinen = nRekinen;
			//	}
			//	else {
			//		m_PrtInfCommon.CoRekinen = 0;
			//	}
			//}
// midori 151730,151263 del <--
// midori 151730,151263 add -->
			if(isHIEIRI(m_apno)) {
				// 下記の場合は和暦固定にする
				// ・公益
				// ・学校法人の旧会計基準
				// ・社会福祉法人の旧会計基準
				if(isKYUKAIKEI(m_apno,m_ver)) {
					m_PrtInfCommon.CoRekinen = 0;
				}
				else {
					m_PrtInfCommon.CoRekinen = m_clsFunc.GetRekinen(m_pZmSub);
				}
			}
// midori 151730,151263 add <--
// midori 191201 add <--
			else	{
				if(pt.prfkin->pkmsw&0x04)	m_PrtInfCommon.CoRekinen = 1;
				else						m_PrtInfCommon.CoRekinen = 0;
			}
// midori 153813 add <------------------
			// 期出力サイン
			if( (pt.prfkin->pkmsw & BIT_D1) != 0 )		m_PrtInfCommon.OutKi = 1;
			else										m_PrtInfCommon.OutKi = 0;
		}

		// -----------------------------------------------------------------------------

		// Owntbテーブルをクローズ
		m_pZmSub->OwnTblClose();
		// 決算情報のテーブルをクローズ
		pt.PrfkinClose();
	}
	catch( ... )	{
		TRACE( "\n# ERROR: CdlgPrintHyoshi::ReadData()_1\n" );
		if( pt.PrfkinOpen() )	{
			pt.PrfkinClose();
			return(-1);
		}
	}
// midori 153382 add <--

	// 会社情報を取得 -----------------------------------------------------------------
	try	{
		strQuery = _T( "SELECT cpyg, cpua, cpda FROM [cpdata] WHERE cpkofs = 0" );
		if ( !rc.OpenEx( NULL, strQuery, "cpdata" ) )	{
			return(-1);
		}

		if( !rc.IsEOF() )	{
			// 会社名
			rc.GetFieldValue( _T( "cpyg" ), var );
			if( var.m_dwType != DBVT_NULL )	{
				m_PrtInfCommon.CoName = *var.m_pstring;
				m_PrtInfCommon.CoName.TrimRight();
			}

			// 住所(上段)
			rc.GetFieldValue( _T( "cpua" ), var );
			if( var.m_dwType != DBVT_NULL ){
				m_PrtInfCommon.CoAddr1 = *var.m_pstring;
				m_PrtInfCommon.CoAddr1.TrimRight();
			}

			// 住所(下段)
			rc.GetFieldValue( _T( "cpda" ), var );
			if( var.m_dwType != DBVT_NULL ){
				m_PrtInfCommon.CoAddr2 = *var.m_pstring;
				m_PrtInfCommon.CoAddr2.TrimRight();
			}
		}
		rc.Close();
	}
	catch( ... )	{
		TRACE( "\n# ERROR: CdlgPrintHyoshi::ReadData()_2\n" );
		if( rc.IsOpen() )	{
			rc.Close();
			return(-1);
		}
	}

	// uc_inf_mainテーブルクラス ------------------------------------------------------
	pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
	if(pfcRec->Init() == DB_ERR_OK)	{
		if(!(pfcRec->IsEOF()))	{
			pfcRec->MoveFirst();
			if(pfcRec->m_GeneralVar[2] & BIT_D2)	m_PrtInfCommon.OutGenzai = 1;
			else									m_PrtInfCommon.OutGenzai = 0;
		}
	}
	pfcRec->Fin();
	delete pfcRec;

	// --------------------------------------------------------------------------------

	return(0);
}

// -----------------------------------------------------------------------------
//	画面表示
// -----------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::SetData(void)
{
// midori UC_0026 del -->
//// midori 181101 add -->
//	int			no=0;			// アプリケーションNo
//// midori 181101 add <--
// midori UC_0026 del <--
	int			yy=0,mm=0,dd=0;
	int			temp=0;
// midori 151730,151263 del -->
//// midori 191201 add -->
//	int			nRekinen=0;
//// midori 191201 add <--
// midori 151730,151263 del <--
	char		buf[256]={0};
	char		str[256]={0};
	VARIANT		Val;
	
	// 期
	memset(str, 0, sizeof(str));
	if(m_PrtInfCommon.Ki > 0)	wsprintf(str, _T("%d"), m_PrtInfCommon.Ki);
	else						memset(str, '\0', sizeof(str));
	Val.pbVal = (UCHAR*)str;
	m_HSDE010.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

	// 期を出力する
	if(m_PrtInfCommon.OutKi == 1)	m_HSCK010.SetCheck(1);	// チェックＯＮ
	else							m_HSCK010.SetCheck(0);	// チェックＯＦＦ

	// 暦年表示		0:和暦 1:西暦
	m_HSCB050.SetCurSel(m_PrtInfCommon.CoRekinen);
// midori 151730,151263 del -->
//// midori 191201 add -->
//	if(m_ChkReki == 1) {
//		m_RekiSave = m_PrtInfCommon.CoRekinen;
//	}
//	m_RekiSave2 = m_PrtInfCommon.CoRekinen;
//// midori 191201 add <--
// midori 151730,151263 del <--
// midori 153813 add ------------------>
	m_pZmSub->VolumeOpen();

// midori 151730,151263 add -->
	m_RekiSave = m_PrtInfCommon.CoRekinen;
	m_RekiSave2 = m_PrtInfCommon.CoRekinen;
// midori 151730,151263 add <--

// midori 151730,151263 del -->
//// midori 191201 del -->
//	//// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は和暦固定にする
//	//if(m_pZmSub->zvol->apno == 0x50 || m_pZmSub->zvol->apno == 0x51 || m_pZmSub->zvol->apno == 0x52 ||
//	//	m_pZmSub->zvol->apno == 0x58 || m_pZmSub->zvol->apno == 0x59)	{
//	//	m_HSCB050.EnableWindow(FALSE);
//	//}
//// midori 191201 del <--
//// midori 191201 add -->
//	// 非営利法人
//	if(m_pZmSub->zvol->apno == 0x50 || m_pZmSub->zvol->apno == 0x51 || m_pZmSub->zvol->apno == 0x52 ||
//		m_pZmSub->zvol->apno == 0x58 || m_pZmSub->zvol->apno == 0x59)	{
//		// 下記の場合は和暦固定にする
//		// ・公益または社会福祉法人の旧会計基準
//		// ・暦年表示未対応
//		if(m_ChkReki == 0) {
//			m_HSCB050.EnableWindow(FALSE);
//		}
//	}
//// midori 191201 add <--
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	// 公益又は社会福祉法人の旧会計又は学校法人の旧会計の場合、暦年表示を無効にする
	if(isKYUKAIKEI(m_apno,m_ver))	{
		m_HSCB050.EnableWindow(FALSE);
	}
// midori 151730,151263 add <--

	m_pZmSub->VolumeClose();
// midori 153813 add <------------------

// midori 181101 add -->
    if( IsValidKaigen() ) {
        CString strGengo;
		db_vd_NoToStrGen(GENGO_NEXT_HEISEI,strGengo);
        m_HSCB020.InsertString(-1, strGengo);
        m_HSCB030.InsertString(-1, strGengo);
    }
// midori 181101 add <--

	// 決算表示期間（自）
	temp = m_PrtInfCommon.KikanStart;
	yy = temp / 10000;
	mm = temp % 10000 / 100;
	dd = temp % 100;
// midori 181101 add -->
	// 元号
	// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合
// midori UC_0026 del -->
	//no = ((CUCHIWAKEApp*)AfxGetApp())->m_APNO;
	//if(no == 0x50 || no == 0x51 || no == 0x52 ||
	//	no == 0x58 || no == 0x59)	{
	//	m_HSCB020.SetCurSel( m_PrtInfCommon.StartGen );
	//}
	//else {
	//	if( m_PrtInfCommon.StartGen ) {
	//		m_HSCB020.SetCurSel( m_PrtInfCommon.StartGen - GENGO_OFFSET );
	//	}
	//	else {
	//		m_HSCB020.SetCurSel( -1 );
	//	}
	//}
// midori UC_0026 del <--
// midori UC_0026 add -->
	if( m_PrtInfCommon.StartGen ) {
		m_HSCB020.SetCurSel( m_PrtInfCommon.StartGen - GENGO_OFFSET );
	}
	else {
		m_HSCB020.SetCurSel( -1 );
	}
// midori UC_0026 add <--
	// 0:和暦 1:西暦
	if(m_PrtInfCommon.CoRekinen == 0)	{
		m_HSCB020.EnableWindow(TRUE);
	}
	else {
		m_HSCB020.EnableWindow(FALSE);
	}
// midori 181101 add <--
	// 開始年
	memset(str, 0, sizeof(str));
	if(yy > 0)	wsprintf(str, _T("%d"), yy);
	else		memset(str, '\0', sizeof(str));
	Val.pbVal = (UCHAR*)str;
	m_HSDE020.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	// 開始月
	memset(str, 0, sizeof(str));
	if(mm > 0)	wsprintf(str, _T("%2d"),mm);
	else		memset(str, '\0', sizeof(str));
	Val.pbVal = (UCHAR*)str;
	m_HSDE021.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	// 開始日
	memset(str, 0, sizeof(str));
	if(dd > 0)	wsprintf(str, _T("%2d"),dd);
	else		memset(str, '\0', sizeof(str));
	Val.pbVal = (UCHAR*)str;
	m_HSDE022.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

	// 決算表示期間（至）
	temp = m_PrtInfCommon.KikanEnd;
	yy = temp / 10000;
	mm = temp % 10000 / 100;
	dd = temp % 100;
// midori 181101 add -->
	// 元号
	// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合
// midori UC_0026 del -->
	//if(no == 0x50 || no == 0x51 || no == 0x52 ||
	//	no == 0x58 || no == 0x59)	{
	//	m_HSCB030.SetCurSel( m_PrtInfCommon.EndGen );
	//}
	//else {
	//	if( m_PrtInfCommon.EndGen ) {
	//		m_HSCB030.SetCurSel( m_PrtInfCommon.EndGen - GENGO_OFFSET );
	//	}
	//	else {
	//		m_HSCB030.SetCurSel( -1 );
	//	}
	//}
// midori UC_0026 del <--
// midori UC_0026 add -->
	if( m_PrtInfCommon.EndGen ) {
		m_HSCB030.SetCurSel( m_PrtInfCommon.EndGen - GENGO_OFFSET );
	}
	else {
		m_HSCB030.SetCurSel( -1 );
	}
// midori UC_0026 add <--
	// 0:和暦 1:西暦
	if(m_PrtInfCommon.CoRekinen == 0)	{
		m_HSCB030.EnableWindow(TRUE);
	}
	else {
		m_HSCB030.EnableWindow(FALSE);
	}
// midori 181101 add <--
	// 終了年
	memset(str, 0, sizeof(str));
	if(yy > 0)	wsprintf(str, _T("%d"), yy);
	else		memset(str, '\0', sizeof(str));
	Val.pbVal = (UCHAR*)str;
	m_HSDE023.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	// 終了月
	memset(str, 0, sizeof(str));
	if(mm > 0)	wsprintf(str, _T("%2d"), mm);
	else		memset(str, '\0', sizeof(str));
	Val.pbVal = (UCHAR*)str;
	m_HSDE024.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	// 終了日
	memset(str, 0, sizeof(str));
	if(dd > 0)	wsprintf(str, _T("%2d"), dd);
	else		memset(str, '\0', sizeof(str));
	Val.pbVal = (UCHAR*)str;
	m_HSDE025.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

	// 「現在」を出力する
	// 期を出力する
	if(m_PrtInfCommon.OutGenzai == 1)	m_HSCK020.SetCheck(1);	// チェックＯＮ
	else								m_HSCK020.SetCheck(0);	// チェックＯＦＦ
	
	// 会社名
	memset(str, 0, sizeof(str));
	memset(buf, 0, sizeof(buf));
	wsprintf(buf, _T("%s"), m_PrtInfCommon.CoName.GetBuffer() );
	LstKjsCut(buf, str, sizeof(buf));
	Val.pbVal = (UCHAR*)str;
	m_HSDE030.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

	// 住所(上段)
	memset(str, 0, sizeof(str));
	memset(buf, 0, sizeof(buf));
	wsprintf(buf, _T("%s"), m_PrtInfCommon.CoAddr1.GetBuffer() );
	LstKjsCut(buf, str, sizeof(buf));
	Val.pbVal = (UCHAR*)str;
	m_HSDE040.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

	// 住所(下段)
	memset(str, 0, sizeof(str));
	memset(buf, 0, sizeof(buf));
	wsprintf(buf, _T("%s"), m_PrtInfCommon.CoAddr2.GetBuffer() );
	LstKjsCut(buf, str, sizeof(buf));
	Val.pbVal = (UCHAR*)str;
	m_HSDE041.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
}

// -----------------------------------------------------------------------------
//	文字列の後ろからあるだけスペースカット
// -----------------------------------------------------------------------------
//	【引数】	sbuf	変換元格納バッファ
//				dbuf	変換先格納バッファ
//				kjmx	変換元格納バッファの最大バイト数
// -----------------------------------------------------------------------------
//	【戻値】	-1：エラー、-1以外：文字列長
// -----------------------------------------------------------------------------
int CdlgPrintHyoshi::LstKjsCut( char sbuf[], char dbuf[], int kjmx )
{
	int	ii=0, max=0;

	// 文字列長チェック
	for(ii=kjmx-1, max=kjmx; ii>=0; )	{
		if( !sbuf[ii] )	{
			ii--;
			max--;
			continue;
		}
		else if( (ii>=1) && (sbuf[ii] == (char)0x40) && (sbuf[ii-1] == (char)0x81) )	{
			// 全角ＳＰ
			max -= 2;
			ii  -= 2;
		}
		else if( sbuf[ii] == (char)0x20 )	{
			// 半角ＳＰ
			max--;
			ii--;
		}
		else{
			break;
		}
	}
	if( max < 0 )	{
		max = 0;
	}

	// 編集後の格納バッファへ転送
	memset(dbuf, 0, kjmx);
	if( max <= kjmx )	{
		memmove( dbuf, sbuf, max );
		return(max);
	}
	else	{
		return(-1);
	}
}

// -----------------------------------------------------------------------------
//	期	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde010(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE010);
}

// -----------------------------------------------------------------------------
//	自 開始年	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde020(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE020);
}

// -----------------------------------------------------------------------------
//	自 開始月	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde021(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE021);
}

// -----------------------------------------------------------------------------
//	自 開始日	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde022(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE022);
}

// -----------------------------------------------------------------------------
//	至 開始年	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde023(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE023);
}

// -----------------------------------------------------------------------------
//	至 開始月	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde024(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE024);
}

// -----------------------------------------------------------------------------
//	至 開始日	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde025(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE025);
}

// -----------------------------------------------------------------------------
//	会社名	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde030(long nChar, long inplen, long kst)
{
	MoveCursel(nChar, inplen, kst, IDC_HSDE030);
}

// -----------------------------------------------------------------------------
//	住所(上段)	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde040(long nChar, long inplen, long kst)
{
	if(nChar == VK_HOME)	{
		PostMessage(WM_COMMAND, ID_HOME_BUTTON);
	}
	else	{
		MoveCursel(nChar, inplen, kst, IDC_HSDE040);
	}
}

// -----------------------------------------------------------------------------
//	住所(下段)	Terminationイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::TerminationHsde041(long nChar, long inplen, long kst)
{
	if(nChar == VK_HOME)	{
		PostMessage(WM_COMMAND, ID_HOME_BUTTON);
	}
	else	{
		MoveCursel(nChar, inplen, kst, IDC_HSDE041);
	}
}

//-----------------------------------------------------------------------------
// 移動
//-----------------------------------------------------------------------------
void CdlgPrintHyoshi::MoveCursel( long nChar, long inplen, long kst, int nID )
{
// ICSDBEditのTABキーは、PreTranslateMessage で処理する
// → フォーカスインでTerminationが発生して、次のコントロールへ移動してしまうため
	if(nChar == VK_RETURN)	{
		if( kst&ICSDBEDT_KST_SHIFT )	{
			PrevDlgCtrl();
		}
		else	{
			NextDlgCtrl();
		}
	}
	else if(nChar == VK_F2)	{
		PrevDlgCtrl();
	}
}

// -----------------------------------------------------------------------------
//	暦年表示	セルチェンジイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::OnCbnSelchangeHscb050()
{
	int			sel=0;
	int			len=0;
// midori 191201 add -->
	int			ymds=0;
	int			ymde=0;
	int			rymd=0;
	int			gengos=0;
	int			gengoe=0;
// midori 191201 add <--
	char		bf1[128],bf2[128];
	VARIANT		Val;

// midori 191201 del -->
//	sel = m_HSCB050.GetCurSel();
//	// 0:和暦 1:西暦
//	if(sel == 0)	{
//		m_HSDE020.SetMaxval(_T("99"));
//		m_HSDE023.SetMaxval(_T("99"));
//
//		// 年の表示を切替え
//		// 開始年
//		m_HSDE020.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
//		len = (int)strlen((char*)Val.pbVal);
//		if(len > 2)	{
//			memset(bf1,0,128);
//			memset(bf2,0,128);
//			wsprintf(bf1,_T("%s"),Val.pbVal);
//			memcpy(bf2,&bf1[len-2],2);
//			Val.pbVal = (UCHAR*)bf2;
//			m_HSDE020.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
//		}
//		// 終了年
//		m_HSDE023.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
//		len = (int)strlen((char*)Val.pbVal);
//		if(len > 2)	{
//			memset(bf1,0,128);
//			memset(bf2,0,128);
//			wsprintf(bf1,_T("%s"),Val.pbVal);
//			memcpy(bf2,&bf1[len-2],2);
//			Val.pbVal = (UCHAR*)bf2;
//			m_HSDE023.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
//		}
//// midori 181101 add -->
//		m_HSCB020.EnableWindow(TRUE);
//		m_HSCB030.EnableWindow(TRUE);
//// midori 181101 add <--
//	}
//	else	{
//		m_HSDE020.SetMaxval(_T("9999"));
//		m_HSDE023.SetMaxval(_T("9999"));
//// midori 181101 add -->
//		m_HSCB020.EnableWindow(FALSE);
//		m_HSCB030.EnableWindow(FALSE);
//// midori 181101 add <--
//	}
// midori 191201 del <--
// midori 191201 add -->
	sel = m_HSCB050.GetCurSel();
	// 前回と同じ選択の場合処理を行わない
// midori 151730,151263 del -->
	//if(m_RekiSave2 == sel) return;
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	if(m_RekiSave == sel) return;
// midori 151730,151263 add <--

// midori 151730,151263 del -->
	//// 非営利法人の暦年表示対応済みの場合は
	//// 西暦⇔和暦の切り替え時、年号と年の変換処理を行う
	//if(m_ChkReki == 1) {
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	// 非営利法人の新会計基準の場合は
	// 西暦⇔和暦の切り替え時、年号と年の変換処理を行う
	if(isHIEIRI(m_apno)) {
// midori 151730,151263 add <--
		// 0:和暦 1:西暦
		if(sel == 0)	{
			m_HSDE020.SetMaxval(_T("99"));
			m_HSDE023.SetMaxval(_T("99"));
			m_HSCB020.EnableWindow(TRUE);
			m_HSCB030.EnableWindow(TRUE);
		}
		else {
			m_HSDE020.SetMaxval(_T("9999"));
			m_HSDE023.SetMaxval(_T("9999"));
			m_HSCB020.EnableWindow(FALSE);
			m_HSCB030.EnableWindow(FALSE);
		}

		// 開始年月日
		if( m_HSCB020.GetCurSel() >= 0 ) {
			gengos = m_HSCB020.GetCurSel() + GENGO_OFFSET;
		}
		else {
			gengos = 0;
		}
		// 開始年
		m_HSDE020.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		rymd = atoi((char*)Val.pbVal);
		ymds = rymd * 10000;
		// 開始月
		m_HSDE021.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		rymd = atoi((char*)Val.pbVal);
		ymds += rymd * 100;
		// 開始日
		m_HSDE022.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		rymd = atoi((char*)Val.pbVal);
		ymds += rymd;

		// 終了年月日
		if( m_HSCB030.GetCurSel() >= 0 ) {
			gengoe = m_HSCB030.GetCurSel() + GENGO_OFFSET;
		}
		else {
			gengoe = 0;
		}
		// 終了年
		m_HSDE023.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		rymd = atoi((char*)Val.pbVal);
		ymde = rymd * 10000;
		// 終了月
		m_HSDE024.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		rymd = atoi((char*)Val.pbVal);
		ymde += rymd * 100;
		// 終了日
		m_HSDE025.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		rymd = atoi((char*)Val.pbVal);
		ymde += rymd;

		// 開始
		rymd = ymds;
		m_clsFunc.GetYmd(sel,ymds,&gengos,&rymd);
		m_HSCB020.SetCurSel((gengos - GENGO_OFFSET));
		// 開始年
		memset(bf1,0,128);
		if((rymd / 10000) > 0)				wsprintf(bf1,_T("%d"),(rymd / 10000));
		else								memset(bf1, '\0', 128);
		Val.pbVal = (UCHAR*)bf1;
		m_HSDE020.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		// 開始月
		memset(bf1,0,128);
		if(((rymd % 10000) / 100) > 0)		wsprintf(bf1,_T("%2d"),((rymd % 10000) / 100));
		else								memset(bf1, '\0', 128);
		Val.pbVal = (UCHAR*)bf1;
		m_HSDE021.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		// 開始日
		memset(bf1,0,128);
		if(rymd % 100 > 0)					wsprintf(bf1,_T("%2d"),(rymd % 100));
		else								memset(bf1, '\0', 128);
		Val.pbVal = (UCHAR*)bf1;
		m_HSDE022.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		// 終了
		rymd = ymde;
		m_clsFunc.GetYmd(sel,ymde,&gengoe,&rymd);
		m_HSCB030.SetCurSel((gengoe - GENGO_OFFSET));
		// 終了年
		memset(bf1,0,128);
		if((rymd / 10000) > 0)				wsprintf(bf1,_T("%d"),(rymd / 10000));
		else								memset(bf1, '\0', 128);
		Val.pbVal = (UCHAR*)bf1;
		m_HSDE023.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		// 終了月
		memset(bf1,0,128);
		if(((rymd % 10000) / 100) > 0)		wsprintf(bf1,_T("%2d"),((rymd % 10000) / 100));
		else								memset(bf1, '\0', 128);
		Val.pbVal = (UCHAR*)bf1;
		m_HSDE024.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		// 終了日
		memset(bf1,0,128);
		if(rymd % 100 > 0)					wsprintf(bf1,_T("%2d"),(rymd % 100));
		else								memset(bf1, '\0', 128);
		Val.pbVal = (UCHAR*)bf1;
		m_HSDE025.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	}
	// 財務処理dbの場合は
	// 西暦⇔和暦の切り替え時、年号と年の変換処理を行わず
	// 西暦→和暦の時、年が2桁になる用に年をカットする
	else {
		// 0:和暦 1:西暦
		if(sel == 0)	{
			m_HSDE020.SetMaxval(_T("99"));
			m_HSDE023.SetMaxval(_T("99"));

			// 年の表示を切替え
			// 開始年
			m_HSDE020.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
			len = (int)strlen((char*)Val.pbVal);
			if(len > 2)	{
				memset(bf1,0,128);
				memset(bf2,0,128);
				wsprintf(bf1,_T("%s"),Val.pbVal);
				memcpy(bf2,&bf1[len-2],2);
				Val.pbVal = (UCHAR*)bf2;
				m_HSDE020.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
			}
			// 終了年
			m_HSDE023.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
			len = (int)strlen((char*)Val.pbVal);
			if(len > 2)	{
				memset(bf1,0,128);
				memset(bf2,0,128);
				wsprintf(bf1,_T("%s"),Val.pbVal);
				memcpy(bf2,&bf1[len-2],2);
				Val.pbVal = (UCHAR*)bf2;
				m_HSDE023.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
			}
			m_HSCB020.EnableWindow(TRUE);
			m_HSCB030.EnableWindow(TRUE);
		}
		else	{
			m_HSDE020.SetMaxval(_T("9999"));
			m_HSDE023.SetMaxval(_T("9999"));
			m_HSCB020.EnableWindow(FALSE);
			m_HSCB030.EnableWindow(FALSE);
		}
	}
// midori 151730,151263 del -->
	//m_RekiSave2 = sel;
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	m_RekiSave = sel;
// midori 151730,151263 add <--
// midori 191201 add <--
}


// -----------------------------------------------------------------------------
//	[住所検索]ボタン クリック（OK）
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::OnBnClickedHomeButton()
{
	int			slen=0;
	int			len=0;
	char		bf1[128]={0},bf2[128]={0};
	char		str[256]={0};
	CString		cStrZip="", cStrAddress="";
	VARIANT		Val;

	if( ZipToAddress(cStrZip, cStrAddress, this) == TRUE )	{
		// エラー処理はキャンセルと同一の流れに入るので、処理なし！

		if( (slen = cStrAddress.GetLength()) > 40 )	{
			// 分割！
			len = 40;
			// 2 = マルチバイト文字の第１バイト
			if(IsKindOfLetter(cStrAddress.GetBuffer(), 39) == 2)	{
				len--;
			}
			memset(bf1,0,sizeof(bf1));
			memset(bf2,0,sizeof(bf2));
			wsprintf(bf1, _T("%s"), cStrAddress.GetBuffer(len) );
			wsprintf(bf2, _T("%s"), cStrAddress.Right(slen-len).GetBuffer() );
		}
		else{
			memset(bf1,0,sizeof(bf1));
			wsprintf(bf1, _T("%s"), cStrAddress.GetBuffer() );
			memset(bf2,'\0',sizeof(bf2));
		}

		// その値でデータ再セット

		// 住所（上段）
		memset(str, '\0', sizeof(str));
		LstKjsCut(bf1, str, sizeof(bf1));
		Val.pbVal = (UCHAR*)str;
		m_HSDE040.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

		// 住所（下段）
		memset(str, '\0', sizeof(str));
		LstKjsCut(bf2, str, sizeof(bf2));
		Val.pbVal = (UCHAR*)str;
		m_HSDE041.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	}
}

// -----------------------------------------------------------------------------
//	[OK]ボタン クリック（OK）
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::OnBnClickedOK()
{
	// 画面取得
	GetData();

	// データ書込み
	if(WriteData() != 0)	{
		return;
	}

	m_nRet = ID_DLG_OK;
// midori 153784 add -->
	m_OtherSign = 1;
// midori 153784 add <--
	ICSDialog::OnOK();
}

// -----------------------------------------------------------------------------
//	OnOkイベント
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
}

// -----------------------------------------------------------------------------
//	画面取得
// -----------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------
void CdlgPrintHyoshi::GetData(void)
{
// midori UC_0026 del -->
//// midori 181101 add -->
//	int			no=0;	// アプリケーションNo
//// midori 181101 add <--
// midori UC_0026 del <--

	int			ymd=0, temp=0;
	VARIANT		Val;

	// 期
	m_HSDE010.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	m_PrtInfCommon.Ki =  atoi((char*)Val.pbVal);

	// 期を出力する
	if(m_HSCK010.GetCheck() == 1)	m_PrtInfCommon.OutKi = 1;
	else							m_PrtInfCommon.OutKi = 0;

	// 暦年表示
	m_PrtInfCommon.CoRekinen = m_HSCB050.GetCurSel();

	// 決算表示期間（自）
	ymd = 0;
// midori UC_0026 del -->
//// midori 181101 add -->
//	// 元号
//	no = ((CUCHIWAKEApp*)AfxGetApp())->m_APNO;
//	// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合
//	if(no == 0x50 || no == 0x51 || no == 0x52 ||
//		no == 0x58 || no == 0x59)	{
//		m_PrtInfCommon.StartGen = m_HSCB020.GetCurSel();
//	}
//	else {
//		if( m_HSCB020.GetCurSel() >= 0 ) {
//			m_PrtInfCommon.StartGen = m_HSCB020.GetCurSel() + GENGO_OFFSET;
//		}
//		else {
//			m_PrtInfCommon.StartGen = 0;
//		}
//	}
//// midori 181101 add <--
// midori UC_0026 del <--
// midori UC_0026 add -->
	if( m_HSCB020.GetCurSel() >= 0 ) {
		m_PrtInfCommon.StartGen = m_HSCB020.GetCurSel() + GENGO_OFFSET;
	}
	else {
		m_PrtInfCommon.StartGen = 0;
	}
// midori UC_0026 add <--
	// 開始年
	m_HSDE020.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	temp = atoi((char*)Val.pbVal);
	ymd = temp * 10000;
	// 開始月
	m_HSDE021.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	temp = atoi((char*)Val.pbVal);
	ymd += temp * 100;
	// 開始日
	m_HSDE022.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	temp = atoi((char*)Val.pbVal);
	ymd += temp;
	m_PrtInfCommon.KikanStart = ymd;

	// 決算表示期間（至）
	ymd = 0;
// midori UC_0026 del -->
//// midori 181101 add -->
//	// 元号
//	// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合
//	if(no == 0x50 || no == 0x51 || no == 0x52 ||
//		no == 0x58 || no == 0x59)	{
//		m_PrtInfCommon.EndGen = m_HSCB030.GetCurSel();
//	}
//	else {
//		if( m_HSCB030.GetCurSel() >= 0 ) {
//			m_PrtInfCommon.EndGen = m_HSCB030.GetCurSel() + GENGO_OFFSET;
//		}
//		else {
//			m_PrtInfCommon.EndGen = 0;
//		}
//	}
//// midori 181101 add <--
// midori UC_0026 del <--
// midori UC_0026 add -->
	if( m_HSCB030.GetCurSel() >= 0 ) {
		m_PrtInfCommon.EndGen = m_HSCB030.GetCurSel() + GENGO_OFFSET;
	}
	else {
		m_PrtInfCommon.EndGen = 0;
	}
// midori UC_0026 add <--
	// 終了年
	m_HSDE023.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	temp = atoi((char*)Val.pbVal);
	ymd = temp * 10000;
	// 終了月
	m_HSDE024.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	temp = atoi((char*)Val.pbVal);
	ymd += temp * 100;
	// 終了日
	m_HSDE025.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	temp = atoi((char*)Val.pbVal);
	ymd += temp;
	m_PrtInfCommon.KikanEnd = ymd;

	// 「現在」を出力する
	if(m_HSCK020.GetCheck() == 1)	m_PrtInfCommon.OutGenzai = 1;
	else							m_PrtInfCommon.OutGenzai = 0;

	// 会社名
	m_HSDE030.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	m_PrtInfCommon.CoName.Format(_T("%s"), Val.pbVal);

	// 住所(上段)
	m_HSDE040.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	m_PrtInfCommon.CoAddr1.Format(_T("%s"), Val.pbVal);

	// 住所(下段)
	m_HSDE041.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	m_PrtInfCommon.CoAddr2.Format(_T("%s"), Val.pbVal);
}

// -----------------------------------------------------------------------------
//	データ書込み
// -----------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	int		0:正常終了、-1:エラー終了
// -----------------------------------------------------------------------------
int CdlgPrintHyoshi::WriteData(void)
{
	int				st=0;
	int				ii=0;
// midori 153382 add -->
	int				symd=0,eymd=0;
// midori 153382 add <--
// midori 153812 add ------------------>
	int				pksdy=0;
	int				pkedy=0;
// midori 181101 add -->
	int				gen = 0;
	int				gen1=0;
	int				gen2=0;
// midori 181101 add <--
// midori 191201 add -->
	BYTE			apno=0x00;
// midori 191201 add <--
// midori 153812 add <------------------
	BYTE			pkmsw=0;
// midori 153812 add <------------------
	CString			strQuery="";
	CString			strDate="";
	CdbBase			rc( m_pDB );
	CDBVariant		var;
	CdateConvert	dateCvt;
	CDBPrtbl		pt;
	CdbUcInfMain*	pfcRec;				// uc_inf_mainテーブルクラス
// midori 153382 add -->
	CVolDateDB		icsVolDate;			// DB期間モジュール
// midori 153382 add <--

// midori 153812 del ------------------>
//// midori 153382 del -->
//	//// 決算情報を保存 -----------------------------------------------------------------
//	//// 決算書データに保存
//	//try	{
//	//	pt.SetMasterDB( m_pDB );
//	//	st = pt.PrfkinOpen("pkofs = 0");		// 当期のみ参照
//	//	if(st != 0)		return(-1);
//	//	// 編集
//	//	pt.prfkin->Edit();
//	//	// 期
//	//	pt.prfkin->pkdkin				= m_PrtInfCommon.Ki;
//	//	// 期出力サイン
//	//	if(m_PrtInfCommon.OutKi == 1)	pt.prfkin->pkmsw |= BIT_D1;
//	//	else							pt.prfkin->pkmsw &= ~BIT_D1;
//	//	// 上書き
//	//	pt.prfkin->Update();
//	//	//
//	//	pt.PrfkinClose();
//	//}
//	//catch( ... )	{
//	//	TRACE( "\n# ERROR: CdlgPrintHyoshi::ReadData()_1\n" );
//	//	if( pt.PrfkinOpen() )	{
//	//		pt.PrfkinClose();
//	//		return(-1);
//	//	}
//	//}
//	//// Owntbテーブルに保存
//	//// Owntbテーブルをオープン
//	//m_pZmSub->OwnTblOpen();
//	//strQuery.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq = 0"), "UCHIWAKE", "HyoshiData");
//	//m_pZmSub->owntb->Requery(strQuery);
//	//if(m_pZmSub->owntb->GetRecordCount() > 0)	{
//	//	// 編集
//	//	m_pZmSub->owntb->Edit();
//	//}
//	//else	{
//	//	// 新規
//	//	m_pZmSub->owntb->AddNew();
//	//	m_pZmSub->owntb->apl_name	= "UCHIWAKE";
//	//	m_pZmSub->owntb->itm_name	= "HyoshiData";
//	//	m_pZmSub->owntb->itm_seq	= 0;
//	//	m_pZmSub->owntb->u_id		= 0;
//	//	for(ii=0;ii<20;ii++)	{
//	//		m_pZmSub->owntb->vl[ii]	= 0;
//	//		m_pZmSub->owntb->str[ii].Empty();
//	//		m_pZmSub->owntb->code[ii].Empty();
//	//	}
//	//}
//	//// 決算期間（自）
//	//m_pZmSub->owntb->vl[0] = m_PrtInfCommon.KikanStart;
//	//// 決算期間（至）
//	//m_pZmSub->owntb->vl[1] = m_PrtInfCommon.KikanEnd;
//	//// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
//	//m_pZmSub->owntb->vl[2] = m_PrtInfCommon.CoRekinen;
//	//// 更新
//	//m_pZmSub->owntb->Update();
//	//// Owntbテーブルをクローズ
//	//m_pZmSub->OwnTblClose();
//// midori 153382 del <--
//// midori 153382 add -->
//	// 決算情報を保存 -----------------------------------------------------------------
//	// 決算書データに保存
//	try	{
//		// 決算書情報テーブルをオープン
//		pt.SetMasterDB( m_pDB );
//		st = pt.PrfkinOpen("pkofs = 0");		// 当期のみ参照
//		if(st != 0)		return(-1);
//		// 編集
//		pt.prfkin->Edit();
//
//		// Owntbテーブルをオープン
//		m_pZmSub->OwnTblOpen();
//		strQuery.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq = 0"), "UCHIWAKE", "HyoshiData");
//		m_pZmSub->owntb->Requery(strQuery);
//		if(m_pZmSub->owntb->GetRecordCount() > 0)	{
//			// 編集
//			m_pZmSub->owntb->Edit();
//		}
//		else	{
//			// 新規
//			m_pZmSub->owntb->AddNew();
//			m_pZmSub->owntb->apl_name	= "UCHIWAKE";
//			m_pZmSub->owntb->itm_name	= "HyoshiData";
//			m_pZmSub->owntb->itm_seq	= 0;
//			m_pZmSub->owntb->u_id		= 0;
//			for(ii=0;ii<20;ii++)	{
//				m_pZmSub->owntb->vl[ii]	= 0;
//				m_pZmSub->owntb->str[ii].Empty();
//				m_pZmSub->owntb->code[ii].Empty();
//			}
//		}
//
//		// ----------------------------------------------------------------------------
//
//		// 「指定期間と連動させる」ONの時
//		if((pt.prfkin->pkmsw & BIT_D3) != 0)	{
//			// 期
//			pt.prfkin->pkdkin					= m_PrtInfCommon.Ki;
//
//			// 決算期間（自）
//			m_pZmSub->owntb->vl[0]				= m_PrtInfCommon.KikanStart;
//			// 決算期間（至）
//			m_pZmSub->owntb->vl[1]				= m_PrtInfCommon.KikanEnd;
//
//			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
//			// 西暦
//			if(m_PrtInfCommon.CoRekinen == 1)	{
//				if((pt.prfkin->pkmsw & 0x04) == 0)	{
//					// 決算書側が和暦のため、年を和暦から西暦に変換して書き込む
//					icsVolDate.db_datecnv(pt.prfkin->pksdy,&symd,1,0);		// 1:和暦→西暦へ変換，0:年月日より取得
//					icsVolDate.db_datecnv(pt.prfkin->pkedy,&eymd,1,0);		// 1:和暦→西暦へ変換，0:年月日より取得
//					pt.prfkin->pksdy = symd;
//					pt.prfkin->pkedy = eymd;
//				}
//				pt.prfkin->pkmsw |= 0x04;
//			}
//			// 和暦
//			else								{
//				if((pt.prfkin->pkmsw & 0x04) != 0)	{
//					// 決算書側が西暦のため、年を西暦から和暦に変換して書き込む
//					icsVolDate.db_datecnv(pt.prfkin->pksdy,&symd,0,0);		// 0:西暦→和暦へ変換，0:年月日より取得
//					icsVolDate.db_datecnv(pt.prfkin->pkedy,&eymd,0,0);		// 0:西暦→和暦へ変換，0:年月日より取得
//					pt.prfkin->pksdy = symd;
//					pt.prfkin->pkedy = eymd;
//				}
//				pt.prfkin->pkmsw &= ~0x04;
//			}
//
//			// 期出力サイン
//			if(m_PrtInfCommon.OutKi == 1)		pt.prfkin->pkmsw |= BIT_D1;
//			else								pt.prfkin->pkmsw &= ~BIT_D1;
//		}
//		else	{
//			// 期
//			pt.prfkin->pkdkin					= m_PrtInfCommon.Ki;
//			// 決算期間（自）
//			pt.prfkin->pksdy					= m_PrtInfCommon.KikanStart;
//			// 決算期間（至）
//			pt.prfkin->pkedy					= m_PrtInfCommon.KikanEnd;
//			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
//			if(m_PrtInfCommon.CoRekinen == 1)	pt.prfkin->pkmsw |= 0x04;
//			else								pt.prfkin->pkmsw &= ~0x04;
//			// 期出力サイン
//			if(m_PrtInfCommon.OutKi == 1)		pt.prfkin->pkmsw |= BIT_D1;
//			else								pt.prfkin->pkmsw &= ~BIT_D1;
//
//			// 保存用 決算期間（自）
//			m_pZmSub->owntb->vl[0] = 0;
//			// 保存用 決算期間（至）
//			m_pZmSub->owntb->vl[1] = 0;
//		}
//		
//		// 比較用 決算期間(自)
//		m_pZmSub->owntb->vl[2] = pt.prfkin->pksdy;
//		// 比較用 決算期間(至)
//		m_pZmSub->owntb->vl[3] = pt.prfkin->pkedy;
//
//		// 決算書テーブル上書き
//		pt.prfkin->Update();
//		// owntbテーブル更新
//		m_pZmSub->owntb->Update();
//
//		// ----------------------------------------------------------------------------
//		// Owntbテーブルをクローズ
//		m_pZmSub->OwnTblClose();
//		// 決算書テーブルをクローズ
//		pt.PrfkinClose();
//
//	}
//	catch( ... )	{
//		TRACE( "\n# ERROR: CdlgPrintHyoshi::ReadData()_1\n" );
//		if( pt.PrfkinOpen() )	{
//			pt.PrfkinClose();
//			return(-1);
//		}
//	}
//// midori 153382 add <--
// midori 153812 del <------------------
// midori 153812 add ------------------>
	// 決算情報を保存 -----------------------------------------------------------------
	// 決算書データに保存
	try	{
		// 決算書情報テーブルをオープン
		pt.SetMasterDB( m_pDB );
		st = pt.PrfkinOpen("pkofs = 0");		// 当期のみ参照
		if(st != 0)		return(-1);
		// 編集
		pt.prfkin->Edit();

		// ----------------------------------------------------------------------------

		// 「指定期間と連動させる」ONの時
// midori 191201 del -->
		//if((pt.prfkin->pkmsw & BIT_D3) != 0)	{
// midori 191201 del <--
// midori 191201 add -->
		m_pZmSub->VolumeOpen();
		apno = m_pZmSub->zvol->apno;
		m_pZmSub->VolumeClose();
		if((pt.prfkin->pkmsw & BIT_D3) != 0)	{
// midori 191201 add <--
			// 期
			pt.prfkin->pkdkin					= m_PrtInfCommon.Ki;

// midori 181101 del -->
			//// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
			//// 西暦
			//if(m_PrtInfCommon.CoRekinen == 1)	{
			//	if((pt.prfkin->pkmsw & 0x04) == 0)	{
			//		// 決算書側が和暦のため、年を和暦から西暦に変換して書き込む
			//		icsVolDate.db_datecnv(pt.prfkin->pksdy,&symd,1,0);		// 1:和暦→西暦へ変換，0:年月日より取得
			//		icsVolDate.db_datecnv(pt.prfkin->pkedy,&eymd,1,0);		// 1:和暦→西暦へ変換，0:年月日より取得
			//		pt.prfkin->pksdy = symd;
			//		pt.prfkin->pkedy = eymd;
			//	}
			//	pt.prfkin->pkmsw |= 0x04;
			//}
			//// 和暦
			//else								{
			//	if((pt.prfkin->pkmsw & 0x04) != 0)	{
			//		// 決算書側が西暦のため、年を西暦から和暦に変換して書き込む
			//		icsVolDate.db_datecnv(pt.prfkin->pksdy,&symd,0,0);		// 0:西暦→和暦へ変換，0:年月日より取得
			//		icsVolDate.db_datecnv(pt.prfkin->pkedy,&eymd,0,0);		// 0:西暦→和暦へ変換，0:年月日より取得
			//		pt.prfkin->pksdy = symd;
			//		pt.prfkin->pkedy = eymd;
			//	}
			//	pt.prfkin->pkmsw &= ~0x04;
			//}
// midori 181101 del <--
// midori 181101 add -->
			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
			// 西暦
			if(m_PrtInfCommon.CoRekinen == 1)	{
				if((pt.prfkin->pkmsw & 0x04) == 0)	{
					// 決算書側が和暦のため、年を和暦から西暦に変換して書き込む
					icsVolDate.db_datecnvGen(pt.prfkin->pkHyGen1,pt.prfkin->pksdy,&gen,&symd,1,0);		// 1:和暦→西暦へ変換，0:年月日より取得
					icsVolDate.db_datecnvGen(pt.prfkin->pkHyGen2,pt.prfkin->pkedy,&gen,&eymd,1,0);		// 1:和暦→西暦へ変換，0:年月日より取得
					pt.prfkin->pksdy = symd;
					pt.prfkin->pkedy = eymd;
				}
				pt.prfkin->pkmsw |= 0x04;
			}
			// 和暦
			else								{
				if((pt.prfkin->pkmsw & 0x04) != 0)	{
					// 決算書側が西暦のため、年を西暦から和暦に変換して書き込む
					icsVolDate.db_datecnvGen(0,pt.prfkin->pksdy,&pt.prfkin->pkHyGen1,&symd,0,0);		// 0:西暦→和暦へ変換，0:年月日より取得
					icsVolDate.db_datecnvGen(0,pt.prfkin->pkedy,&pt.prfkin->pkHyGen2,&eymd,0,0);		// 0:西暦→和暦へ変換，0:年月日より取得
					pt.prfkin->pksdy = symd;
					pt.prfkin->pkedy = eymd;
				}
				pt.prfkin->pkmsw &= ~0x04;
			}
// midori 181101 add <--

			// 期出力サイン
			if(m_PrtInfCommon.OutKi == 1)		pt.prfkin->pkmsw |= BIT_D1;
			else								pt.prfkin->pkmsw &= ~BIT_D1;
		}
		else	{
			// 期
			pt.prfkin->pkdkin					= m_PrtInfCommon.Ki;
			// 決算期間（自）
// midori 181101 add -->
			pt.prfkin->pkHyGen1					= m_PrtInfCommon.StartGen;
// midori 181101 add <--
			pt.prfkin->pksdy					= m_PrtInfCommon.KikanStart;
			// 決算期間（至）
// midori 181101 add -->
			pt.prfkin->pkHyGen2					= m_PrtInfCommon.EndGen;
// midori 181101 add <--
			pt.prfkin->pkedy					= m_PrtInfCommon.KikanEnd;
// midori 191201 del -->
			//// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
			//if(m_PrtInfCommon.CoRekinen == 1)	pt.prfkin->pkmsw |= 0x04;
			//else								pt.prfkin->pkmsw &= ~0x04;
// midori 191201 del <--
// midori 191201 add -->
// midori 151730,151263 del -->
			//// 非営利法人の暦年表示対応後の場合は暦年表示を
			//// psfvolテーブルに書き込むためここでの書き込みは不要
			//if( m_ChkReki == 0) {
// midori 151730,151263 del <--
// midori 151730,151263 add -->
			// 非営利法人の場合は暦年表示を
			// rekinenテーブルに書き込むためここでの書き込みは不要
			if( !isHIEIRI(m_apno) ) {
// midori 151730,151263 add <--
				// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
				if(m_PrtInfCommon.CoRekinen == 1)	pt.prfkin->pkmsw |= 0x04;
				else								pt.prfkin->pkmsw &= ~0x04;
			}
// midori 191201 add <--
			// 期出力サイン
			if(m_PrtInfCommon.OutKi == 1)		pt.prfkin->pkmsw |= BIT_D1;
			else								pt.prfkin->pkmsw &= ~BIT_D1;
		}

		pkmsw = pt.prfkin->pkmsw;
		pksdy = pt.prfkin->pksdy;
		pkedy = pt.prfkin->pkedy;

// midori 181101 add -->
		gen1 = pt.prfkin->pkHyGen1;
		gen2 = pt.prfkin->pkHyGen2;
// midori 181101 add <--

		// 決算書テーブル上書き
		pt.prfkin->Update();

// midori 154704 add -->
// midori 151730,151263 del -->
//// midori 191201 add -->
//		// 非営利法人かつ暦年表示対応済みの場合、暦年表示の保存場所が異なるため、
//		// すべての期に書き込む対応は不要
//		if(m_ChkReki == 0) {
//// midori 191201 add <--
// midori 151730,151263 del <--
// midori 151730,151263 add -->
		// 非営利法人の場合、暦年表示の保存場所が異なるため、
		// すべての期に書き込む対応は不要
		if(!isHIEIRI(m_apno)) {
// midori 151730,151263 add <--
			// 暦年表示(西暦・和暦)のみすべての期に書き込む
			if(pt.prfkin->Requery() == -1)	{
				if(pt.prfkin->IsNodata() == FALSE)	{
					ICSMessageBox(pt.GetLastErrMsg(),MB_ICONSTOP);
					return(-1);
				}
			}
			else	{
				for(ii=0; ; ii++)	{
					if(ii == 0)	{
						if((pt.prfkin->MoveFirst() == -1) || (pt.prfkin->st == -1))	{
							break;
						}
					}
					else	{
						if((pt.prfkin->MoveNext() == -1) || (pt.prfkin->st == -1))	{
							break;
						}
					}
					pt.prfkin->Edit();
					pt.prfkin->pkmsw &= ~BIT_D1;
					pt.prfkin->pkmsw &= ~0x04;
					pt.prfkin->pkmsw |= pkmsw;
					if(pt.prfkin->Update() == -1)	{
						ICSMessageBox(pt.GetLastErrMsg(),MB_ICONSTOP);
						return(-1);
					}
				}
			}
// midori 154704 add <--
// midori 191201 add -->
		}
// midori 191201 add <--
		// 決算書テーブルをクローズ
		pt.PrfkinClose();

		// Owntbテーブルをオープン
		m_pZmSub->OwnTblOpen();
		strQuery.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq = 0"), "UCHIWAKE", "HyoshiData");
		m_pZmSub->owntb->Requery(strQuery);
		if(m_pZmSub->owntb->GetRecordCount() > 0)	{
			// 編集
			m_pZmSub->owntb->Edit();
		}
		else	{
			// 新規
			m_pZmSub->owntb->AddNew();
			m_pZmSub->owntb->apl_name	= "UCHIWAKE";
			m_pZmSub->owntb->itm_name	= "HyoshiData";
			m_pZmSub->owntb->itm_seq	= 0;
			m_pZmSub->owntb->u_id		= 0;
			for(ii=0;ii<20;ii++)	{
				m_pZmSub->owntb->vl[ii]	= 0;
				m_pZmSub->owntb->str[ii].Empty();
				m_pZmSub->owntb->code[ii].Empty();
			}
		}

		// 「指定期間と連動させる」ONの時
		if((pkmsw & BIT_D3) != 0)	{
			// 決算期間（自）
// midori 181101 add -->
			m_pZmSub->owntb->vl[4]				= m_PrtInfCommon.StartGen;
// midori 181101 add <--
			m_pZmSub->owntb->vl[0]				= m_PrtInfCommon.KikanStart;
			// 決算期間（至）
// midori 181101 add -->
			m_pZmSub->owntb->vl[5]				= m_PrtInfCommon.EndGen;
// midori 181101 add <--
			m_pZmSub->owntb->vl[1]				= m_PrtInfCommon.KikanEnd;
		}
		else	{
			// 保存用 決算期間（自）
// midori 181101 add -->
			m_pZmSub->owntb->vl[4]				= 0;
// midori 181101 add <--
			m_pZmSub->owntb->vl[0] = 0;
			// 保存用 決算期間（至）
// midori 181101 add -->
			m_pZmSub->owntb->vl[5]				= 0;
// midori 181101 add <--
			m_pZmSub->owntb->vl[1] = 0;
		}
		// 比較用 決算期間(自)
		m_pZmSub->owntb->vl[2] = pksdy;
// midori 181101 add -->
		// 比較用 元号
		m_pZmSub->owntb->vl[6] = gen1;
// midori 181101 add <--
		// 比較用 決算期間(至)
		m_pZmSub->owntb->vl[3] = pkedy;
// midori 181101 add -->
		// 比較用 元号
		m_pZmSub->owntb->vl[7] = gen2;
// midori 181101 add <--

		// owntbテーブル更新
		m_pZmSub->owntb->Update();

		// ----------------------------------------------------------------------------
		// Owntbテーブルをクローズ
		m_pZmSub->OwnTblClose();

// midori 191201 add -->
// midori 151730,151263 del -->
		//// 非営利法人の暦年表示対応の場合はpsfvolテーブルに書き込みを行う
		//if(m_ChkReki == 1) {
		//	strQuery.Empty();
		//	switch(apno) {
		//	// 公益Ⅲ
		//	case 0x51:
		//		strQuery.Format(_T("update psfvol set vl16 = %d where aplno = 0 and itmno = 12"),m_PrtInfCommon.CoRekinen);
		//		break;
		//	// 社会福祉法人
		//	case 0x52:
		//		strQuery.Format(_T("update psfvol set vl17 = %d where aplno = 0 and itmno = 12"),m_PrtInfCommon.CoRekinen);
		//		break;
		//	// 学校法人
		//	case 0x58:
		//		strQuery.Format(_T("update psfvol set vl16 = %d where aplno = 1 and itmno = 12"),m_PrtInfCommon.CoRekinen);
		//		break;
		//	// 宗教法人
		//	case 0x59:
		//		strQuery.Format(_T("update psfvol set vl20 = %d where aplno = 0 and itmno = 11"),m_PrtInfCommon.CoRekinen);
		//		break;
		//	}
		//	m_pZmSub->m_database->ExecuteSQL(strQuery);
// midori 151730,151263 del <--
// midori 151730,151263 add -->
		// 非営利法人の場合はrekinenテーブルに暦年表示の書き込みを行う
		if(isHIEIRI(m_apno) && !isKYUKAIKEI(m_apno,m_ver)) {
			//------------------------------
			//	暦年表示の選択値保存
			//------------------------------
			m_pZmSub->NpRekinenOpen();
			m_pZmSub->m_pRekinen->EditEx();
			m_pZmSub->m_pRekinen->m_rsel	= m_PrtInfCommon.CoRekinen;				//暦年表示
			m_pZmSub->m_pRekinen->UpdateEx();
			m_pZmSub->NpRekinenClose();
// midori 151730,151263 add <--
		}
// midori 191201 add <--

	}
	catch( ... )	{
		TRACE( "\n# ERROR: CdlgPrintHyoshi::ReadData()_1\n" );
		if( pt.PrfkinOpen() )	{
			pt.PrfkinClose();
			return(-1);
		}
	}

// midori 153812 add <------------------

	// 会社情報を保存 -----------------------------------------------------------------
	try	{
		strQuery = _T( "SELECT * FROM [cpdata] WHERE cpkofs = 0" );
		if ( !rc.OpenEx( NULL, strQuery, "cpdata" ) )	{
			return(-1);
		}
		rc.Close();

		// SQLコマンド作成
		//strQuery.Format(_T("UPDATE [cpdata] SET cpyg='%s', cpua='%s', cpda='%s' "), 
		//	m_PrtInfCommon.CoName, m_PrtInfCommon.CoAddr1, m_PrtInfCommon.CoAddr2);
		CString	c_nam=m_PrtInfCommon.CoName;
		CString	c_ad1=m_PrtInfCommon.CoAddr1;
		CString	c_ad2=m_PrtInfCommon.CoAddr2;
		c_nam.Replace( _T("'"), _T("''") );	// 修正依頼No.161058
		c_ad1.Replace( _T("'"), _T("''") );	// 修正依頼No.161060
		c_ad2.Replace( _T("'"), _T("''") );	// 修正依頼No.161060
		strQuery.Format(_T("UPDATE [cpdata] SET cpyg='%s',cpua='%s',cpda='%s'"),c_nam,c_ad1,c_ad2);
		strQuery += _T("WHERE cpkofs = 0");

		// SQL文を実行
		m_pZmSub->m_database->ExecuteSQL(strQuery);
	}
	catch( ... )	{
		TRACE( "\n# ERROR: CdlgPrintHyoshi::WriteData()_2\n" );
		if( rc.IsOpen() )	{
			rc.Close();
			return(-1);
		}
	}

	// uc_inf_mainテーブルクラス ------------------------------------------------------
	pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
	if(pfcRec->Init() == DB_ERR_OK)	{
		pfcRec->Edit();
		if(m_PrtInfCommon.OutGenzai == 1)	pfcRec->m_GeneralVar[2] |= BIT_D2;
		else								pfcRec->m_GeneralVar[2] &= ~BIT_D2;
		pfcRec->Update();
	}
	pfcRec->Fin();
	delete pfcRec;

	// --------------------------------------------------------------------------------

// midori 180104 add -->
	// --------------------------------------------------------------------------------
	// 下記の項目が変更されていれば顧問先情報を更新する
	// --------------------------------------------------------------------------------
	// 会社名
	if(CmpStrWithoutBspace(&m_PrtInfCommon.CoName,m_CoName.GetBuffer()))	{
		m_ChgFlg |= 0x01;
	}
	// 住所上段
	if(CmpStrWithoutBspace(&m_PrtInfCommon.CoAddr1,m_CoAddr1.GetBuffer()))	{
		m_ChgFlg |= 0x02;
	}
	// 住所下段
	if(CmpStrWithoutBspace(&m_PrtInfCommon.CoAddr2,m_CoAddr2.GetBuffer()))	{
		m_ChgFlg |= 0x04;
	}
// midori 180104 add <--

// midori 151730,151263 del -->
//// midori 191201 add -->
//	// 非営利法人かつ暦年表示対応済みで、暦年表示が変更された場合
//	if(m_ChkReki == 1 && m_RekiSave != m_PrtInfCommon.CoRekinen)	{
//		m_clsFunc.YmdConvert(m_pZmSub,m_PrtInfCommon.CoRekinen);
//	}
//// midori 191201 add <--
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	// 財務処理dbまたは、非営利法人の旧会計基準マスター以外の
	if(!isKYUKAIKEI(m_apno,m_ver)) {
		// 暦年表示の選択に変更有り
		if(m_RekiSave2 != m_PrtInfCommon.CoRekinen)	{
			// 非営利法人
			if(isHIEIRI(m_apno)) {
				m_clsFunc.YmdConvert(m_pZmSub,m_PrtInfCommon.CoRekinen);
// midori 151258,151262 add -->
				// 学校法人の場合、「消費支出準備金　会計年度」の更新を行う
				if(apno == 0x58) {
					m_clsFunc.YmdConvert2(m_pZmSub,m_PrtInfCommon.CoRekinen);
				}
// midori 151258,151262 add <--
			}
			// 財務処理db
			else {
				m_clsFunc.YmdConvert3(m_pZmSub,m_PrtInfCommon.CoRekinen);
			}
		}
	}
// midori 151730,151263 add <--

	return(0);
}
// midori 180101 add <--

// midori 180104 add -->
// -----------------------------------------------------------------------------
// ２つの文字列の比較
// -----------------------------------------------------------------------------
//	【引数】	CString *	比較対象１
//				CString *	比較対象２
//	【戻値】	0	：	等しい
//				>0	：	pCStr1 は pStr2 より大きい
//				<0	：	pCStr1 は pStr2 より小さい
// -----------------------------------------------------------------------------
int CdlgPrintHyoshi::CmpStrWithoutBspace( CString *pCStr1, char *pStr2 )
{
	ASSERT( pCStr1 );
	ASSERT( pStr2 );

	char	buff1[128], buff2[128];

	// 対象１
	memset( buff1, '\0', sizeof(buff1) );
	wsprintf( buff1, _T("%s"), pCStr1->GetBuffer() );

	// 対象２
	memset( buff2, '\0', sizeof(buff2) );
	wsprintf( buff2, _T("%s"), pStr2 );

	return CompareStringSJIS( buff1, buff2 );
}

// -----------------------------------------------------------------------------
//	顧問先情報への書き込み
// -----------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	0:正常終了　-1:エラー
// -----------------------------------------------------------------------------
int CdlgPrintHyoshi::PutCoinfo( void )
{
	CCoinfCtl	coinfctl;

	// zvolumeテーブル参照
	if (m_pZmSub->VolumeOpen() != 0) {
		// エラー
		m_clsFunc.ZmSubErrMsg( m_pZmSub );
		return(-1);
	}

	char	sdev[16]={0}, tmpdev[16]={0};
	PCOM_GetString( _T("MasterDevice"), tmpdev );
	wsprintf( sdev, _T("%s:"), tmpdev );

	if( coinfctl.OpenFile(3, sdev) == FALSE ){
		// 環境によっては、新総合会社情報ファイルが存在しないことがあるので！
		return(0);
	}

	// 会社コード
	unsigned char	cocode[4];
	int_bcd( cocode, m_pZmSub->zvol->v_cod, 4 );

	int st = 0;
	if( (st=coinfctl.IsRegistMcode(1, m_pZmSub->zvol->apno, 0, cocode, NULL)) == 1 ){

		// 該当のものが法人or個人？
		CINF_REC	cinfrec;
		CTBL_PAR	ctblpar;
		memset( &cinfrec, '\0', sizeof(CINF_REC) );
		memset( &ctblpar, '\0', sizeof(CTBL_PAR) );
		ctblpar.tpDtyp = 1;
		ctblpar.tpMtyp = 0;
		ctblpar.tpApno = m_pZmSub->zvol->apno;
		memmove( ctblpar.tpCcod, cocode, sizeof(cocode) );

		if( coinfctl.FindRecFromMcod(0, &cinfrec, &ctblpar) == 1 ){

			// 会社情報へ書き込み
			NCOF_PAR	NCpar = {0};
			CINF_REC	CoRec = {0};
			memset( &NCpar, '\0', sizeof(NCOF_PAR) );
			memset( &CoRec, '\0', sizeof(CINF_REC) );
			
			NCpar.cpPare = this;
			NCpar.cpDtyp = 1;
			NCpar.cpMtyp = 0;
			NCpar.cpApno = m_pZmSub->zvol->apno;
			memmove( NCpar.cpCode, cocode, 4 );

			// 書き込み用
			char	buf[128], wrtbuf[128];

			// 会社名
			if( m_ChgFlg&0x01 ){
				memset( buf, '\0', sizeof(buf) );
				memset( wrtbuf, '\0', sizeof(wrtbuf) );
//				wsprintf( buf, _T("%s"), m_pDbprtbl->cpdata->cpyg.GetBuffer() );
				wsprintf( buf, _T("%s"), m_PrtInfCommon.CoName.GetBuffer() );
				LstKjsCut( buf, wrtbuf, sizeof(buf) );
				if( ctblpar.tpHorK == 0 ){
					memmove( CoRec.ciName, wrtbuf, sizeof(CoRec.ciName) );
					NCpar.cpEflg[0] |= 0x00000004;
				}
// midori 7 del -->
//				else if( ctblpar.tpHorK == 1 ){
//					// 36バイトにカット
//					short	rt = 0;
//					if( strlen(wrtbuf) > 36 ){
//						if( (rt=IsKindOfLetter(wrtbuf, 35)) == 2 ){
//							wrtbuf[35] = '\0';
//						}
//						else{
//							wrtbuf[36] = '\0';
//						}
//					}
//					memmove( CoRec.ciYagou, wrtbuf, sizeof(CoRec.ciYagou) );
//					NCpar.cpEflg[0] |= 0x00000080;
//				}
// midori 7 del <--
			}

			// 住所上段
			if( m_ChgFlg&0x02 ){
				memset( buf, '\0', sizeof(buf) );
				memset( wrtbuf, '\0', sizeof(wrtbuf) );
//				wsprintf( buf, _T("%s"), m_pDbprtbl->cpdata->cpua.GetBuffer() );
				wsprintf( buf, _T("%s"), m_PrtInfCommon.CoAddr1.GetBuffer() );
				LstKjsCut( buf, wrtbuf, sizeof(buf) );
				memmove( CoRec.ciAdrs1, wrtbuf, sizeof(CoRec.ciAdrs1) );
				NCpar.cpEflg[0] |= 0x00001000;

// 21/08/24 顧問先情報への書き込み対応 add -->
				//「所在地を納税地とする」のチェックがONを示している時には、
				// 住所情報を納税地（cinfrec.ciTppAd1）にも書き込む。
				if(cinfrec.ciNzsw1 == 1)	{
					memset(buf, '\0', sizeof(buf));
					memset(wrtbuf, '\0', sizeof(wrtbuf));
					wsprintf(buf, _T("%s"), m_PrtInfCommon.CoAddr1.GetBuffer());
					LstKjsCut(buf, wrtbuf, sizeof(buf));
					memmove(CoRec.ciTppAd1, wrtbuf, sizeof(CoRec.ciTppAd1));
					NCpar.cpEflg[1] |= 0x00000800;
				}
// 21/08/24 顧問先情報への書き込み対応 add <--
			}

			// 住所下段
			if( m_ChgFlg&0x04 ){
				memset( buf, '\0', sizeof(buf) );
				memset( wrtbuf, '\0', sizeof(wrtbuf) );
//				wsprintf( buf, _T("%s"), m_pDbprtbl->cpdata->cpda.GetBuffer() );
				wsprintf( buf, _T("%s"), m_PrtInfCommon.CoAddr2.GetBuffer() );
				LstKjsCut( buf, wrtbuf, sizeof(buf) );
				memmove( CoRec.ciAdrs2, wrtbuf, sizeof(CoRec.ciAdrs2) );
				NCpar.cpEflg[0] |= 0x00004000;

// 21/08/24 顧問先情報への書き込み対応 add -->
				//「所在地を納税地とする」のチェックがONを示している時には、
				// 住所情報を納税地（cinfrec.ciTppAd2）にも書き込む。
				if(cinfrec.ciNzsw1 == 1)	{
					memset(buf, '\0', sizeof(buf));
					memset(wrtbuf, '\0', sizeof(wrtbuf));
					wsprintf(buf, _T("%s"), m_PrtInfCommon.CoAddr2.GetBuffer());
					LstKjsCut(buf, wrtbuf, sizeof(buf));
					memmove(CoRec.ciTppAd2, wrtbuf, sizeof(CoRec.ciTppAd2));
					NCpar.cpEflg[1] |= 0x00002000;
				}
// 21/08/24 顧問先情報への書き込み対応 add <--
			}

			// 実書込み
// midori 7 del -->
//			coinfctl.PutCoinf( 0, &CoRec, &NCpar );
// midori 7 del <--
// midori 7 add -->
			if(NCpar.cpEflg[0] != 0)	{
				coinfctl.SetMstDat(&CoRec, KMNCTL_GKIND_KNJ);	// 顧問先情報表示対応 add
				coinfctl.PutCoinf( 0, &CoRec, &NCpar );
			}
// midori 7 add <--
		}
	}

	coinfctl.CloseFile();

	m_pZmSub->VolumeClose();

	return 0;
}
// midori 180104 add <--
