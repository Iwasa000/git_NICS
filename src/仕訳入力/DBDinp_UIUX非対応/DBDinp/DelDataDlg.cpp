// DelDataDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "DataListCtrl.h"
#include "MsgDlg.h"

#include "DelDataDlg.h"

#include "input1.h"
#include "external.h"

#include "MstRw.h"

#include "DinpSub.h"

#include "Bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル

extern
int SearchForDelDataBack( int first, CWnd* pWnd, CMsgDlg* pMsgDlg, BOOL* bCancel );
extern
int GetDtListColumn(int nLine, CArray<DT_COLUMN, DT_COLUMN&>& clm, int inv/* = 0*/);


IMPLEMENT_DYNAMIC(CDelDataDlg, ICSDialog)

CDelDataDlg::CDelDataDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CDelDataDlg::IDD, pParent)
{
	m_bShow = FALSE;
	m_bData = FALSE;
	m_bRepeat = FALSE;

	m_DataPos = 0;
	IsDataBack() = FALSE;
}

CDelDataDlg::~CDelDataDlg()
{
}

void CDelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_IKATU, m_Ikat);
	DDX_Control(pDX, IDC_BTN_OK, m_Modify);
	DDX_Control(pDX, IDC_BTN_NOMDFY, m_NoMdfy);
	DDX_Control(pDX, IDC_BTN_END, m_End);
	DDX_Control(pDX, IDC_DEL_SWK, m_DtData);
}


BEGIN_MESSAGE_MAP(CDelDataDlg, ICSDialog)
	
	ON_MESSAGE( WM_STARTSIWAKE, OnStartSiwake)
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_BTN_IKATU, &CDelDataDlg::OnBnClickedBtnIkatu)
	ON_BN_CLICKED(IDC_BTN_END, &CDelDataDlg::OnBnClickedBtnEnd)
	ON_BN_CLICKED(IDC_BTN_NOMDFY, &CDelDataDlg::OnBnClickedBtnNomdfy)
	ON_BN_CLICKED(IDC_BTN_OK, &CDelDataDlg::OnBnClickedBtnOk)
END_MESSAGE_MAP()

//
// CDelDataDlg メッセージ ハンドラ
//
BOOL CDelDataDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
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
	m_DtData.SetReadOnly( TRUE );

	DT_ITEMMAX im;
	im.im_seq = 6;
	im.im_denp = 6;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);
	im.im_sno= Voln1->snocol;

	m_DtData.SetImax( &im );
	m_DtData.SetConfirmMode( Confirm_Mode() );

	m_DtData.EnforceLayout();

	m_DtData.EnableWindow(FALSE);

	ICSDialog::OnInitDialogEX();

	c_set(IDC_BTN_OK);

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDelDataDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ICSDialog::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( bShow == TRUE && ! m_bShow ) {
		m_bShow = TRUE;
		PostMessage(WM_STARTSIWAKE, 0, 0);
	}
}


LRESULT CDelDataDlg::OnStartSiwake(WPARAM wParam, LPARAM lParam)
{
	// 対象仕訳があるかどうかは、このダイアログを出す前に、チェックしている
	if( pDBzm->dbdata->st == -1 ) {
		ICSMessageBox( "該当の取消仕訳がありません。", 0,0,0,this);
		EndDialog(IDCANCEL);
		return 0;
	}

	m_bData = TRUE;
	NextDataSet( 0, NULL );
	
	return 0;
}



// 次ﾃﾞｰﾀ検索 and ﾃﾞｰﾀｾｯﾄ
void CDelDataDlg::NextDataSet( int nxt_sg, CMsgDlg* msgdlg )
{
	// 次仕訳を読み込む
	BOOL bCancel;

	if( nxt_sg ) {
		int st = SearchForDelDataBack( 0, this, msgdlg, &bCancel );

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
		// 次ﾃﾞｰﾀをｾｯﾄ
		MakeDspData( &m_Data );
	}
}

// データを表示する
void CDelDataDlg::MakeDspData( CDBINPDataRec* pdt )
{
	CDataListCtrl* pLst;
	struct _DATA_LINE dline;
	DTLIST_DATA* pLdata;
	pLdata = &dline._XXDTA;

	pLst = &m_DtData;

	DB_SyIniz( pdt );
	pLdata->dd_syz_str = sy_line_txt( pdt/*, NULL, INP_mode*/ );
		
	inpline_inz( &dline );

	dline._XXDTA.dd_seq = pdt->m_seq;
	_recdsp( pdt, &dline, FALSE, TRUE );

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
				pLdata->dd_tek.width	= pIMREC->dotxs;
				pLdata->dd_tek.height	= pIMREC->dotys;
				pLdata->dd_tek.image	= (char*)pIMREC->imdata;
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


/*=============================================
	ﾎﾞﾀﾝの状態ｾｯﾄ
	BOOL flg .... TRUE = Enable, 
				  FALSE = Disable
	int all ..... 0 -> END ﾎﾞﾀﾝは除く(ﾃﾞｰﾀ有り無し時)
				  1 -> 全てのﾎﾞﾀﾝ
  ==============================================*/
void CDelDataDlg::SetBtnCondition(BOOL flg, int all)
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


////////////////////////////////////////////////////////////////////////////////////////////
// 表示行設定
//
#include "DtClmItem.h"

//extern
//DT_CLMITEM*	GetDtListColumn( int type, int& clmax );

// line は 表示行数(現在は 使用していない)
//
void CDelDataDlg::SetListColumn( int line )
{
	CArray<DT_COLUMN, DT_COLUMN&> clm;
	int cnt = GetDtListColumn(2, clm, 1);

	m_DtData.SetItemColumn( &clm[0], cnt );
}


// 一括復帰
void CDelDataDlg::OnBnClickedBtnIkatu()
{
	if( m_bData && !m_bRepeat ) {
		if( ICSMessageBox("取消仕訳の一括復帰を行いますか？", MB_YESNO, 0,0,this) == IDYES ) {
			m_bRepeat = TRUE;
			PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_OK, BN_CLICKED),
							(LPARAM)GetDlgItem(IDC_BTN_OK)->m_hWnd );
		}
	}	
}

// 個別復帰
void CDelDataDlg::OnBnClickedBtnOk()
{
	if( !m_bData ) return;

	CMsgDlg* msgdlg = NULL;
	
	if( m_bRepeat ) {
		// 全ﾎﾞﾀﾝをDisable
		SetBtnCondition(FALSE, 1);

		if( (msgdlg = new CMsgDlg(IDS_MSG_STRING3) ) == NULL ) {
			EndDialog(-1);
			return;
		}
//		double x,y;
//		x = y = 1.0;
//		GetRate(x,y);
//TRACE("ikat rate %g, %g", x, y);
//		msgdlg->SetResizeRate(x,y);
		msgdlg->Create(this);
	}

	MSG msg;

	BOOL bEnaModify = TRUE;
	if( ! IsModifyConfirmData() || IsEnkakuUser() ) {	// 確定仕訳修正不可・遠隔ユーザの場合
		bEnaModify = FALSE;
	}

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

		BOOL bHukki = TRUE;

		if( ! bEnaModify ) {	// 確定仕訳は修正不可
			if( _tline_chk( &m_Data.m_dsign ) != 0 || IsJzSansyo() ) {
				bHukki = FALSE;
			}
		}

		if( bHukki && (m_Data.m_dsign[0] & 0x01) ) {
			// 取消復帰
			m_Data.m_dsign[0] &= ~0x01;

			if( pDBzm->DB_DataCorrect( &m_Data ) == ERR ) {
				// ｴﾗｰ
				if( msgdlg ) {
					msgdlg->DestroyWindow();
					delete msgdlg;
					msgdlg = NULL;
				}
				EndDialog(-1);
				return;
			}
			//諸口・現金貸借
			syog_genk( &m_Data, _ADD );

			MODEFY_BIT.onbitn( m_Data.m_seq - 1);

			IsDataBack() = TRUE;
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

		// 最終ﾃﾞｰﾀを再表示する
		if( pDBzm->dbdata->seq > 0 )
			pDBzm->GetCDBData( &m_Data );

		MakeDspData( &m_Data );
	}
}

// 次の仕訳へ
void CDelDataDlg::OnBnClickedBtnNomdfy()
{
	if( !m_bData || m_bRepeat ) return;

	NextDataSet( 1, NULL );	
}

// 終了
void CDelDataDlg::OnBnClickedBtnEnd()
{
	EndDialog(IDOK);
}



BOOL CDelDataDlg::PreTranslateMessage(MSG* pMsg)
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
