// SiwkDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbdinp.h"

#include "DBDinpRec.h"
#include "DataListCtrl.h"

#include "MstRw.h"

#include "MsgDlg.h"

#include "SiwkDlg.h"
#include "input1.h"
#include "external.h"

#include "DinpSub.h"

#include "RpKeyDlg.h"
extern CRpkeyDlg*	pRpkeyDlg;

#include "Bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル
extern CBit	SYZMDFY_BIT;	//消費税仕訳ビットテーブル

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ScanView.cpp
extern
int SearchForIkat( int first, CWnd* pWnd, CMsgDlg* pMsgDlg, BOOL* bCancel );
extern
int GetDtListColumn(int nLine, CArray<DT_COLUMN, DT_COLUMN&>& clm, int inv/* = 0*/);

/////////////////////////////////////////////////////////////////////////////
// CSiwkDlg ダイアログ


CSiwkDlg::CSiwkDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSiwkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSiwkDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_ID = CSiwkDlg::IDD;

	m_bShow = FALSE;
	m_bData = FALSE;
	m_bRepeat = FALSE;

	IsCorrect() = FALSE;

	m_bSwkDel = FALSE;
}


void CSiwkDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSiwkDlg)
                // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	DDX_Control(pDX, IDC_BTN_IKATU, m_Ikat);
	DDX_Control(pDX, IDC_BTN_OK, m_Modify);
	DDX_Control(pDX, IDC_BTN_NOMDFY, m_NoMdfy);
	DDX_Control(pDX, IDC_BTN_END, m_End);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MOTO_SWK, m_DtMoto);
	DDX_Control(pDX, IDC_NEW_SWK,  m_DtNew);
}


BEGIN_MESSAGE_MAP(CSiwkDlg, ICSDialog)
	//{{AFX_MSG_MAP(CSiwkDlg)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_IKATU, OnBtnIkatu)
	ON_BN_CLICKED(IDC_BTN_NOMDFY, OnBtnNomdfy)
	ON_BN_CLICKED(IDC_BTN_END, OnBtnEnd)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_STARTSIWAKE, OnStartSiwake)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSiwkDlg メッセージ ハンドラ

BOOL CSiwkDlg::OnInitDialog() 
{
//	ControlInit( 480, 216, -1 );

	ICSDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	SetListColumn( 2 );

/*
	DWORD style;
	style = m_DtMoto.GetStyle();
	
	if( Voln1->apno == 0x20 ) 
		style |= DS_KOJI_USE;
	else						
		style &= ~DS_KOJI_USE;

	m_DtMoto.SetStyle( style );
	m_DtNew.SetStyle( style );
*/
	m_DtMoto.SetReadOnly( TRUE );
	m_DtNew.SetReadOnly( TRUE );

	DT_ITEMMAX im;
	im.im_seq = 6;
	im.im_denp = 6;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);
	im.im_sno = Voln1->snocol;

	m_DtMoto.SetImax( &im );
	m_DtNew.SetImax( &im );
//	m_DtMoto.SetLines(3);
//	m_DtNew.SetLines(3);

	m_DtMoto.SetConfirmMode( Confirm_Mode() );
	m_DtNew.SetConfirmMode( Confirm_Mode() );

	m_DtMoto.EnforceLayout();
	m_DtNew.EnforceLayout();

	//m_DtMoto.EnableWindow(FALSE);
	//m_DtNew.EnableWindow(FALSE);

	ICSDialog::OnInitDialogEX();

	if( m_bSwkDel ) {
		SetWindowText( "取消仕訳の確認" );

		GetDlgItem(IDC_STATIC1)->SetWindowText( "取消前の仕訳" );
		GetDlgItem(IDC_STATIC7)->SetWindowText( "取消後の仕訳" );

		GetDlgItem(IDC_BTN_IKATU)->SetWindowText( "一括取消[F6]" );
		GetDlgItem(IDC_BTN_OK)->SetWindowText( "取消する[F12]" );
		GetDlgItem(IDC_BTN_NOMDFY)->SetWindowText( "取消しない[F8]" );
	}

	c_set(IDC_BTN_OK);

	return FALSE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CSiwkDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	ICSDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( bShow == TRUE && ! m_bShow ) {
		m_bShow = TRUE;
		PostMessage(WM_STARTSIWAKE, 0, 0);	
	}
}

// 仕訳修正開始
LRESULT CSiwkDlg::OnStartSiwake(WPARAM wParam, LPARAM lParam)
{
	// 対象仕訳があるかどうかは、このダイアログを出す前に、チェックしている
	if( pDBzm->dbdata->st == -1 ) {
		ICSMessageBox( "該当の仕訳がありません。", 0,0,0,this);
		EndDialog(IDCANCEL);
		return 0;
	}

	m_bData = TRUE;
	NextDataSet( 0, NULL );
	
	return 0;
}


// 次ﾃﾞｰﾀ検索 and ﾃﾞｰﾀｾｯﾄ
void CSiwkDlg::NextDataSet( int nxt_sg, CMsgDlg* msgdlg )
{
	// 次仕訳を読み込む
	BOOL bCancel;

	if( nxt_sg ) {
		int st = SearchForIkat( 0, this, msgdlg, &bCancel );

		if( bCancel ) {
			// キャンセル
			m_bRepeat = FALSE;
		}
		else {
			if( st > 0 ) {
				pDBzm->GetCDBData( &m_Data );
			}
			else {
				m_bData = FALSE;
				SetBtnCondition(FALSE, 0);
			}
		}
	}
	else {
		pDBzm->GetCDBData( &m_Data );
	}

	if( m_bData ) {
		MakeDspData(MDFY_BEFORE, &m_Data );
		// 次ﾃﾞｰﾀをｾｯﾄ
		m_MdfyData = m_Data;
		// 修正後の仕訳ｾｯﾄ
		pRpkeyDlg->MakeMdfySiwake(&m_MdfyData);

		MakeDspData(MDFY_AFTER, &m_MdfyData );
	}
}

// データを表示する
void CSiwkDlg::MakeDspData(int sgn, CDBINPDataRec* pdt )
{
	CDataListCtrl* pLst;
	struct _DATA_LINE dline;
	DTLIST_DATA* pLdata;
	pLdata = &dline._XXDTA;

	pLst = (sgn == MDFY_BEFORE) ? &m_DtMoto : &m_DtNew;

	if( sgn == MDFY_BEFORE )
		DB_SyIniz( pdt );

	pLdata->dd_syz_str = sy_line_txt( pdt/*, NULL, INP_mode*/ );

	inpline_inz( &dline );

	dline._XXDTA.dd_seq = pdt->m_seq;
	_recdsp( pdt, &dline, FALSE, TRUE );

	BOOL bChange;
	BOOL bSyzChange = FALSE;
	BOOL bZeiChange = FALSE;

	for( int pn = DATA_SEQ; pn <= DATA_DOCEVI; ++pn ) {
	
		if( sgn == MDFY_AFTER ) {
			// 色の変更

			bChange = FALSE;

			if( pn == DATA_ZEI ) {
				if( memcmp( m_Data.m_zei, m_MdfyData.m_zei, sizeof m_Data.m_zei ) != 0 ) {
					bChange = TRUE;
					bZeiChange = TRUE;
				}
			}
			else {
				bChange = pRpkeyDlg->ReplaceChk(pn);

				if( pn == DATA_SYZ ) {
					bSyzChange = bChange;

					if( !bSyzChange ) {	//日付で自動的に税率が変わる場合も色付け
						if( pDBzm->km_syattr(m_MdfyData.m_dbt) != 0 || pDBzm->km_syattr(m_MdfyData.m_cre) != 0 ) {
							if( m_Data.m_ddate != m_MdfyData.m_ddate && m_Data.m_dsign[1] != m_MdfyData.m_dsign[1] ) {
								bSyzChange = TRUE;
							}
						}
					}
				}
			}
			//変動事由が変更になっている場合。
			if( pn == DATA_HJIYU ) {
				if( ! bChange ) {
					if( m_Data.m_hjiyu != m_MdfyData.m_hjiyu )
						bChange = TRUE;
				}
			}

			if( bChange ) {
				pLdata->dd_ctbl[pn].back = RGB_YELLOW1;
				pLdata->dd_ctbl[pn].text = RGB_BLACK;
			}
		//	else {
		//		pLdata->dd_ctbl[pn].back = RGB_WHITE;
		//		pLdata->dd_ctbl[pn].text = RGB_BLACK;
		//	}
		}
	}
#ifdef CLOSE
	for( int pn = DATA_SEQ; pn <= DATA_DOCEVI; ++pn ) {
	
		if( sgn == MDFY_AFTER ) {
			// 色の変更
			if( pRpkeyDlg->ReplaceChk(pn) ) {
				pLdata->dd_ctbl[pn].back = RGB_YELLOW1;
				pLdata->dd_ctbl[pn].text = RGB_BLACK;
			}
		//	else {
		//		pLdata->dd_ctbl[pn].back = RGB_WHITE;
		//		pLdata->dd_ctbl[pn].text = RGB_BLACK;
		//	}
		}
	}
#endif

	dline._XXDTA.dd_seq = pdt->m_seq;
	_recdsp( pdt, &dline, FALSE, TRUE );

	if( bSyzChange ) {
		pLdata->dd_ctbl[DATA_SYZ].back = RGB_YELLOW1;
		pLdata->dd_ctbl[DATA_SYZ].text = RGB_BLACK;
	}
	//免税・インボイス
	if( sgn == MDFY_AFTER ) {
		if( bInvMaster && bInvUse ) {
			if( (m_Data.m_dsign[6] & 0x20) != (m_MdfyData.m_dsign[6] & 0x20) ) {
				pLdata->dd_ctbl[DATA_USER2].back = RGB_YELLOW1;
				pLdata->dd_ctbl[DATA_USER2].text = RGB_BLACK;
			}
			if( (m_Data.m_dsign[6] & 0xC0) != (m_MdfyData.m_dsign[6] & 0xC0) ) {
				pLdata->dd_ctbl[DATA_SYZ].back = RGB_YELLOW1;
				pLdata->dd_ctbl[DATA_SYZ].text = RGB_BLACK;
			}
		}

		if( bInvMaster && bInvUse && bInvDisp ) {
			CString sinv1, sinv2;

			sinv1 = m_Data.m_invno;
			sinv2 = m_MdfyData.m_invno;

			if( sinv1.CompareNoCase( sinv2 ) != 0 ) {
				pLdata->dd_ctbl[DATA_USER3].back = RGB_YELLOW1;
				pLdata->dd_ctbl[DATA_USER3].text = RGB_BLACK;
			}
		}

		if( bZeiChange ) {
			pLdata->dd_ctbl[DATA_ZEI].back = RGB_YELLOW1;
			pLdata->dd_ctbl[DATA_ZEI].text = RGB_BLACK;
		}
	}

	pLdata->dd_tek.image = 0;
	pLdata->dd_tek.width = 0;
	pLdata->dd_tek.height = 0;
	if( dline.ImgSEQ ) {
		// イメージファイル
//		CString filter;
//		filter.Format( "iseq = '%d'", dtline->ImgSEQ );
//		if( pDBzm->image->Requery( filter ) == 0 ) {
		if( pDBzm->image->SetAbsolutePosition( dline.ImgSEQ ) == 0 ) {
			IMREC* pIMREC = pDBzm->image->DecodeImage();
			if( pIMREC != NULL ) {
				pLdata->dd_tek.width = pIMREC->dotxs;
				pLdata->dd_tek.height = pIMREC->dotys;
				pLdata->dd_tek.image = (char*)pIMREC->imdata;
			}
			else {
				pLdata->dd_tek.tekiyo	= Image;
				pLdata->dd_tek.width	= 0;
				pLdata->dd_tek.height	= 0;
				pLdata->dd_tek.image	= NULL;
			}
		}
	}

	pLst->SetDtListData( 0, pLdata );
}


void CSiwkDlg::OnBtnOk() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bData ) return;

	CMsgDlg* msgdlg = NULL;
	
	if( m_bRepeat ) {
		// 全ﾎﾞﾀﾝをDisable
		SetBtnCondition(FALSE, 1);

		DWORD ids;
		ids = (m_bSwkDel) ? IDS_MSG_STRING4 : IDS_MSG_STRING2;

		if( (msgdlg = new CMsgDlg(ids) ) == NULL ) {
			EndDialog(-1);
			return;
		}
		double x,y;
		x = y = 1.0;
//		GetRate(x,y);
TRACE("ikat rate %g, %g", x, y);
		msgdlg->SetResizeRate(x,y);
		msgdlg->Create(this);
	}

	MSG msg;

	do {
		// ﾒｯｾｰｼﾞ処理
		while( ::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		// 繰り返し処理のｷｬﾝｾﾙﾎﾞﾀﾝが押された場合のﾁｪｯｸ
		if( m_bRepeat ) {
			if( msgdlg->CheckCancelButton() ) {
				m_bRepeat = FALSE;
				break;
			}
		}

		// 仕訳修正
		if( IsNewDataWrite( &m_MdfyData, &m_Data ) || isdiff_data( &m_MdfyData, &m_Data ) ) {
			int ret;

			ret = pDBzm->DB_DataCorrect( &m_MdfyData );

			if( ret == ERR ) {
				// ｴﾗｰ
				if( msgdlg ) {
					msgdlg->DestroyWindow();
					delete msgdlg;
					msgdlg = NULL;
				}
				EndDialog(-1);
				return;
			}
			else if( ret == -2 ) {
				int st = myICSMessageBox( (LPCTSTR)pDBzm->GetZmErrMsg() );
			
				pDBzm->GetCDBData( &m_MdfyData );
				// 修正はしていないが再表示のため
				IsCorrect() = TRUE;
			}
			else {
				MODEFY_BIT.onbitn( m_MdfyData.m_seq - 1);
				if( isSyzdiff_data( &m_MdfyData, &m_Data ) ) {
					SYZMDFY_BIT.onbitn( m_MdfyData.m_seq - 1);
				}
				IsCorrect() = TRUE;
			}

			int rno;
			rno = m_MdfyData.m_seq;
			--rno;
		}
		// 次ﾃﾞｰﾀｾｯﾄ
		NextDataSet( 1, msgdlg );

//		Sleep(100);

	} while( m_bRepeat && m_bData );

	// 繰り返し終了ﾀﾞｲｱﾛｸﾞの破棄
	if( msgdlg ) {
		// ﾎﾞﾀﾝをﾃﾞｰﾀの有無によって状態変更
		SetBtnCondition(m_bData, 0);

		msgdlg->DestroyWindow();
		delete msgdlg;
		msgdlg = NULL;
	}
}

//
//	修正データを書き込むか？ 確定にする場合は、無条件に書き込む
//
BOOL CSiwkDlg::IsNewDataWrite( CDBINPDataRec *newdata, CDBINPDataRec *moto ) 
{
	BOOL bRet = FALSE;

	if( (newdata->m_dsign[7] & 0x10) ) {
		if( ! (moto->m_dsign[7] & 0x10) ) {
			bRet = TRUE;
		}
	}

	return bRet;
}



// 一括修正ボタンが押された
void CSiwkDlg::OnBtnIkatu() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_bData && !m_bRepeat ) {
		CString msg;
		if( m_bSwkDel )	msg = "仕訳の一括取消を行いますか？";
		else			msg = "仕訳の一括修正を行いますか？";

		if( ICSMessageBox( msg, MB_YESNO, 0,0,this) == IDYES ) {
			m_bRepeat = TRUE;
			PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_OK, BN_CLICKED),
							(LPARAM)GetDlgItem(IDC_BTN_OK)->m_hWnd );
		}
	}	
}

void CSiwkDlg::OnBtnNomdfy() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bData || m_bRepeat ) return;

	NextDataSet( 1, NULL );	
}

void CSiwkDlg::OnBtnEnd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EndDialog(IDCANCEL);
}


/*=============================================
	ﾎﾞﾀﾝの状態ｾｯﾄ
	BOOL flg .... TRUE = Enable, 
				  FALSE = Disable
	int all ..... 0 -> END ﾎﾞﾀﾝは除く(ﾃﾞｰﾀ有り無し時)
				  1 -> 全てのﾎﾞﾀﾝ
  ==============================================*/
void CSiwkDlg::SetBtnCondition(BOOL flg, int all)
{
	if( !all ) {
		CWnd* pEnd;
		pEnd = GetDlgItem(IDC_BTN_END);
		if( pEnd->IsWindowEnabled() == FALSE )
			pEnd->EnableWindow(TRUE);

		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd ) {
			int nID = pwnd->GetDlgCtrlID();
			if( nID == IDC_BTN_OK || nID == IDC_BTN_NOMDFY || nID == IDC_BTN_IKATU) {
				GotoDlgCtrl(GetDlgItem(IDC_BTN_END));
			}
		}
	}

	GetDlgItem(IDC_BTN_IKATU)->EnableWindow(flg);
	GetDlgItem(IDC_BTN_OK)->EnableWindow(flg);
	GetDlgItem(IDC_BTN_NOMDFY)->EnableWindow(flg);
	if( all )
		GetDlgItem(IDC_BTN_END)->EnableWindow(flg);
}


BOOL CSiwkDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_END ) {
		//	PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_END, BN_CLICKED),
		//					(LPARAM)GetDlgItem(IDC_BTN_END)->m_hWnd );
			GetDlgItem(IDC_BTN_END)->SendMessage(WM_LBUTTONDOWN, 0,0 );
			GetDlgItem(IDC_BTN_END)->SendMessage(WM_LBUTTONUP, 0,0 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_F12 && m_bData && !m_bRepeat ) {
		//	SendMessage(WM_COMMAND, MAKELONG(IDC_BTN_OK, BN_CLICKED),
		//					(LPARAM)GetDlgItem(IDC_BTN_OK)->m_hWnd );
			GetDlgItem(IDC_BTN_OK)->SendMessage(WM_LBUTTONDOWN, 0,0 );
			GetDlgItem(IDC_BTN_OK)->SendMessage(WM_LBUTTONUP, 0,0 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_F8 && m_bData && !m_bRepeat ) {
		//	SendMessage(WM_COMMAND, MAKELONG(IDC_BTN_NOMDFY, BN_CLICKED),
		//					(LPARAM)GetDlgItem(IDC_BTN_NOMDFY)->m_hWnd );
			GetDlgItem(IDC_BTN_NOMDFY)->SendMessage(WM_LBUTTONDOWN, 0,0 );
			GetDlgItem(IDC_BTN_NOMDFY)->SendMessage(WM_LBUTTONUP, 0,0 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_F6 && m_bData && !m_bRepeat ) {
		//	SendMessage(WM_COMMAND, MAKELONG(IDC_BTN_IKATU, BN_CLICKED),
		//					(LPARAM)GetDlgItem(IDC_BTN_IKATU)->m_hWnd );
			GetDlgItem(IDC_BTN_IKATU)->SendMessage(WM_LBUTTONDOWN, 0,0 );
			GetDlgItem(IDC_BTN_IKATU)->SendMessage(WM_LBUTTONUP, 0,0 );
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


////////////////////////////////////////////////////////////////////////////////////////////
// 表示行設定
//
#include "DtClmItem.h"

//extern
//DT_CLMITEM*	GetDtListColumn( int type, int& clmax );


// line は 表示行数(現在は 使用していない)
//
void CSiwkDlg::SetListColumn( int line )
{
	CArray<DT_COLUMN, DT_COLUMN&> clm;
	int cnt = GetDtListColumn(2, clm, 1);

	m_DtMoto.SetItemColumn( &clm[0], cnt );
	m_DtNew.SetItemColumn( &clm[0], cnt );;
}



