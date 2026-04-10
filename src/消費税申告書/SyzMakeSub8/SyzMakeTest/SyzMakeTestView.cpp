// SyzMakeTestView.cpp : CSyzMakeTestView クラスの実装
//

#include "stdafx.h"
#include "SyzMakeTest.h"

#include "MainFrm.h"
#include "SyzMakeTestDoc.h"
#include "SyzMakeTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSyzMakeTestView

IMPLEMENT_DYNCREATE(CSyzMakeTestView, ICSFormView)

BEGIN_MESSAGE_MAP(CSyzMakeTestView, ICSFormView)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, ICSFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, ICSFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CSyzMakeTestView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSyzMakeTestView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSyzMakeTestView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSyzMakeTestView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSyzMakeTestView::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSyzMakeTestView::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSyzMakeTestView::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSyzMakeTestView::OnBnClickedButton8)
END_MESSAGE_MAP()


// CSyzMakeTestView コンストラクション/デストラクション

CSyzMakeTestView::CSyzMakeTestView()
	: ICSFormView(CSyzMakeTestView::IDD)
{
	// TODO: 構築コードをここに追加します。
}

CSyzMakeTestView::~CSyzMakeTestView()
{
}

void CSyzMakeTestView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ApnoCmb);
	DDX_Control(pDX, IDC_EDIT1, m_CodeEdit);
	DDX_Control(pDX, IDC_EDIT2, m_KisyuEdit);
	DDX_Control(pDX, IDC_EDIT3, m_KimatsuEdit);
}

BOOL CSyzMakeTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return ICSFormView::PreCreateWindow(cs);
}

void CSyzMakeTestView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();
}


// CSyzMakeTestView 診断

#ifdef _DEBUG
void CSyzMakeTestView::AssertValid() const
{
	ICSFormView::AssertValid();
}

void CSyzMakeTestView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}

CSyzMakeTestDoc* CSyzMakeTestView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSyzMakeTestDoc)));
	return (CSyzMakeTestDoc*)m_pDocument;
}
#endif //_DEBUG

// CSyzMakeTestViewクラスの印刷

BOOL CSyzMakeTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CSyzMakeTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CSyzMakeTestView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	ICSFormView::OnPrepareDC(pDC, pInfo);
}

void CSyzMakeTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CSyzMakeTestView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: コントロールを印刷するコードを追加してください。
}

// CSyzMakeTestView メッセージ ハンドラ
void CSyzMakeTestView::OnButtonF4()
{
	ICSFormView::OnButtonF4();
}

void CSyzMakeTestView::OnButtonF5()
{
	ICSFormView::OnFilePrint();
}

void CSyzMakeTestView::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	SYZMAKE_MKCOINFO	mkcoinfo;
	memset( &mkcoinfo, '\0', sizeof(SYZMAKE_MKCOINFO) );

	PCOM_GetString( _T("SelectSqlServer"), mkcoinfo.dbSvrName );
	GCOM_GetString( _T("SystemDatabase"), mkcoinfo.dbSysDbName );
	PCOM_GetString( _T("ProgramDevice"), mkcoinfo.pdev );
	PCOM_GetString( _T("ExecGrJobs"), mkcoinfo.grno, mkcoinfo.jobno );

	if( GetInfoFromControl(mkcoinfo) ){
		return;
	}
	wsprintf( mkcoinfo.c_name, _T("消費税（テスト作成）") );
	wsprintf( mkcoinfo.c_kana, _T("シ") );

	// 消費税申告書（非連動）
	mkcoinfo.sub_sw |= 0x8000;
	mkcoinfo.sub_sw |= 0x10000;

	// 単体マスター？
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		mkcoinfo.mkOpt1 |= 0x01;
	}

	CSyzMakeSub	syzmk;
//	if( syzmk.MakeDBMaster(mkcoinfo, this) ){
	if( syzmk.MakeDBMaster(mkcoinfo, NULL) ){
		ICSMessageBox( syzmk.GetErrorMsg() );
	}

	AfxMessageBox( _T("作成完了！") );
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CSyzMakeTestView::InitApl()
{
	InitDisp();

	return 0;
}

//-----------------------------------------------------------------------------
// 画面初期化
//-----------------------------------------------------------------------------
void CSyzMakeTestView::InitDisp()
{
	// 業種
	m_ApnoCmb.SetCurSel( 0 );

	// 期首年月日
	m_KisyuEdit.SetWindowText( _T("20130101") );
	// 期末年月日
	m_KimatsuEdit.SetWindowText( _T("20131231") );
}

//-----------------------------------------------------------------------------
// コントロールから会社情報の取得
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	情報のセット先
//-----------------------------------------------------------------------------
int CSyzMakeTestView::GetInfoFromControl( SYZMAKE_MKCOINFO &mkcoinfo )
{
	// 業種
	int sel = m_ApnoCmb.GetCurSel();
	if( sel == 0 ){			// 法人
		mkcoinfo.apno = 0x00;
	}
	else if( sel == 1 ){	// 個人
		mkcoinfo.apno = 0x10;
	}
	else if( sel == 2 ){	// 建設
		mkcoinfo.apno = 0x20;
	}
	else if( sel == 3 ){	// 運送
		mkcoinfo.apno = 0x01;
	}
	else if( sel == 4 ){	// 病院
		mkcoinfo.apno = 0x30;
	}

	// 会社コード
	CString	str;
	m_CodeEdit.GetWindowText( str );
	if( str.GetLength() == 0 ){
		AfxMessageBox( _T("会社コード未設定") );
		return -1;
	}
	mkcoinfo.cocode = atoi( str );

	// 期首年月日
	m_KisyuEdit.GetWindowText( str );
	if( str.GetLength() == 0 ){
		AfxMessageBox( _T("期首年月日未設定") );
		return -1;
	}
	mkcoinfo.ss_ymd = atoi( str );

	// 期末年月日
	m_KimatsuEdit.GetWindowText( str );
	if( str.GetLength() == 0 ){
		AfxMessageBox( _T("期末年月日未設定") );
		return -1;
	}
	mkcoinfo.ee_ymd = atoi( str );

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税チェック
//-----------------------------------------------------------------------------
void CSyzMakeTestView::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	CoSel		m_cosel;	// 会社確認
	switch( m_cosel.co_ok() ){
		case	IDOK:
			break;
		case	IDCANCEL:
			AfxMessageBox( _T("処理を中止します。") );
			return;
			break;
		default:
			break;
	}

	char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
	n_apno = atoi( apno );
	n_cocode = atoi( cocode );
	n_kikan = atoi( kikan );

	CDBZmSub	*m_pZmsub = NULL;
	if( (m_pZmsub = new CDBZmSub()) == NULL ){
		AfxMessageBox( _T("エラー") );
		return;
	}
	m_pZmsub->ExitMode( 1 );

	if( m_pZmsub->SetMasterDB( 2, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
		AfxMessageBox( _T("エラー") );
		if( m_pZmsub ){
			delete m_pZmsub;
			m_pZmsub = NULL;
		}
	}

	SYZ_CHKINFO	chkinfo = { 0 };
	CString		str;
	CSyzMakeSub	syzmk;

	if( syzmk.CheckWrongData( m_pZmsub, 20140401, 20140430 ) ){
		AfxMessageBox( syzmk.GetErrorMsg() );
	}

	int			st = 0;
	if( (st=syzmk.CheckSyzState(m_pZmsub, str, chkinfo)) == -1 ){
		AfxMessageBox( syzmk.GetErrorMsg() );
		if( m_pZmsub ){
			delete m_pZmsub;
			m_pZmsub = NULL;
		}
		return;
	}
	else if( st == 1 ){
		AfxMessageBox( str );
	}

	if( m_pZmsub ){
		delete m_pZmsub;
		m_pZmsub = NULL;
	}

	AfxMessageBox( _T("正常終了") );
}

void CSyzMakeTestView::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SYZMAKE_MKCOINFO	mkcoinfo;
	memset( &mkcoinfo, '\0', sizeof(SYZMAKE_MKCOINFO) );

	PCOM_GetString( _T("SelectSqlServer"), mkcoinfo.dbSvrName );
	GCOM_GetString( _T("SystemDatabase"), mkcoinfo.dbSysDbName );
	PCOM_GetString( _T("ProgramDevice"), mkcoinfo.pdev );
	PCOM_GetString( _T("ExecGrJobs"), mkcoinfo.grno, mkcoinfo.jobno );

	if( GetInfoFromControl(mkcoinfo) ){
		return;
	}
	wsprintf( mkcoinfo.c_name, _T("消費税（テスト作成）") );
	wsprintf( mkcoinfo.c_kana, _T("シ") );

	CSyzMakeSub	syzmk;
//	if( syzmk.MakeDBMaster(mkcoinfo, this) ){
	if( syzmk.DeleteDBMaster(mkcoinfo) ){
		ICSMessageBox( syzmk.GetErrorMsg() );
	}
}

void CSyzMakeTestView::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	CoSel		m_cosel;	// 会社確認
	switch( m_cosel.co_ok() ){
		case	IDOK:
			break;
		case	IDCANCEL:
			AfxMessageBox( _T("処理を中止します。") );
			return;
			break;
		default:
			break;
	}

	char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
	n_apno = atoi( apno );
	n_cocode = atoi( cocode );
	n_kikan = atoi( kikan );

	CDBZmSub	*m_pZmsub = NULL;
	if( (m_pZmsub = new CDBZmSub()) == NULL ){
		AfxMessageBox( _T("エラー") );
		return;
	}
	m_pZmsub->ExitMode( 1 );

	if( m_pZmsub->SetMasterDB( 2, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
		ICSMessageBox( m_pZmsub->GetZmErrMsg() );
		if( m_pZmsub ){
			delete m_pZmsub;
			m_pZmsub = NULL;
		}
		return;
	}

	// ボリュームラベル
	if( m_pZmsub->zvol == NULL ){
		if( m_pZmsub->VolumeOpen() == -1 ){
			ICSMessageBox( m_pZmsub->GetZmErrMsg() );
			if( m_pZmsub ){
				delete m_pZmsub;
				m_pZmsub = NULL;
			}
			return;
		}
	}

	// 消費税集計
	if( m_pZmsub->szsyk == NULL ){
		if( m_pZmsub->SzsykOpen() == -1 ){
			ICSMessageBox( m_pZmsub->GetZmErrMsg() );
			if( m_pZmsub ){
				delete m_pZmsub;
				m_pZmsub = NULL;
			}
			return;
		}
	}

	if( m_pZmsub ){
		delete m_pZmsub;
		m_pZmsub = NULL;
	}
}

void CSyzMakeTestView::OnBnClickedButton5()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	COPAK pak;
	pak.sgn0 |= 0x14;
	pak.pksw |= 0x1;
	pak.cod[0] = pak.cod[1] = pak.cod[2] = pak.cod[3] = 0xff;
	pak.apno = 0xffff;
	pak.subs = 0x20000;
	pak.sgn4 |= 0x20;	// SUBマスターのみでも表示

	CoSel		m_cosel;	// 会社確認
	switch( m_cosel.co_ok(pak) ){
		case	IDOK:
			break;
		case	IDCANCEL:
			AfxMessageBox( _T("処理を中止します。") );
			return;
			break;
		default:
			break;
	}

	char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
	n_apno = atoi( apno );
	n_cocode = atoi( cocode );
	n_kikan = atoi( kikan );

	CDBZmSub	*m_pZmsub = NULL;
	if( (m_pZmsub = new CDBZmSub()) == NULL ){
		AfxMessageBox( _T("エラー") );
		return;
	}
	m_pZmsub->ExitMode( 1 );

	if( m_pZmsub->SetMasterDB( 2, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
		ICSMessageBox( m_pZmsub->GetZmErrMsg() );
		if( m_pZmsub ){
			delete m_pZmsub;
			m_pZmsub = NULL;
		}
		return;
	}

	// ボリュームラベル
	if( m_pZmsub->zvol == NULL ){
		if( m_pZmsub->VolumeOpen() == -1 ){
			ICSMessageBox( m_pZmsub->GetZmErrMsg() );
			if( m_pZmsub ){
				delete m_pZmsub;
				m_pZmsub = NULL;
			}
			return;
		}
	}

	CSyzMakeSub	syzmk;
	if( syzmk.RepairZmonthInfo(m_pZmsub) ){
		ICSMessageBox( syzmk.GetErrorMsg() );
	}

	if( m_pZmsub ){
		delete m_pZmsub;
		m_pZmsub = NULL;
	}
}

void CSyzMakeTestView::OnBnClickedButton6()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SYZMAKE_MKCOINFO	mkcoinfo;
	memset( &mkcoinfo, '\0', sizeof(SYZMAKE_MKCOINFO) );

	PCOM_GetString( _T("SelectSqlServer"), mkcoinfo.dbSvrName );
	GCOM_GetString( _T("SystemDatabase"), mkcoinfo.dbSysDbName );
	PCOM_GetString( _T("ProgramDevice"), mkcoinfo.pdev );
	PCOM_GetString( _T("ExecGrJobs"), mkcoinfo.grno, mkcoinfo.jobno );

	if( GetInfoFromControl(mkcoinfo) ){
		return;
	}
	wsprintf( mkcoinfo.c_name, _T("消費税（テスト作成）") );
	wsprintf( mkcoinfo.c_kana, _T("シ") );

	// 消費税申告書（非連動）
	mkcoinfo.sub_sw |= 0x20000;

	// 単体マスター？
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		mkcoinfo.mkOpt1 |= 0x01;
	}

	CSyzMakeSub	syzmk;
	if( syzmk.ReMakeZvolume(mkcoinfo) ){
		ICSMessageBox( syzmk.GetErrorMsg() );
	}
}


void CSyzMakeTestView::OnBnClickedButton7()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CoSel		m_cosel;	// 会社確認
	switch( m_cosel.co_ok() ){
	case	IDOK:
		break;
	case	IDCANCEL:
		AfxMessageBox(_T("処理を中止します。"));
		return;
		break;
	default:
		break;
	}

	char	sqlsvr[128]={ 0 }, apno[32]={ 0 }, cocode[32]={ 0 }, kikan[32]={ 0 };
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString("SELECTCODSN", sqlsvr, apno, cocode, kikan);
	n_apno = atoi(apno);
	n_cocode = atoi(cocode);
	n_kikan = atoi(kikan);

	CDBZmSub	*m_pZmsub = NULL;
	if( (m_pZmsub = new CDBZmSub()) == NULL ){
		AfxMessageBox(_T("エラー"));
		return;
	}
	m_pZmsub->ExitMode(1);

	if( m_pZmsub->SetMasterDB(3, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
		ICSMessageBox(m_pZmsub->GetZmErrMsg());
		if( m_pZmsub ){
			delete m_pZmsub;
			m_pZmsub = NULL;
		}
		return;
	}

	// ボリュームラベル
	if( m_pZmsub->zvol == NULL ){
		if( m_pZmsub->VolumeOpen() == -1 ){
			ICSMessageBox(m_pZmsub->GetZmErrMsg());
			if( m_pZmsub ){
				delete m_pZmsub;
				m_pZmsub = NULL;
			}
			return;
		}
	}

	CSyzMakeSub	syzmk;
	if( syzmk.CheckSepParaData(m_pZmsub, 20191001, 20191031) ){
		ICSMessageBox(syzmk.GetErrorMsg());
	}

	if( m_pZmsub ){
		delete m_pZmsub;
		m_pZmsub = NULL;
	}
}

void CSyzMakeTestView::OnBnClickedButton8()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	char	sqlsvr[128]={ 0 }, apno[32]={ 0 }, cocode[32]={ 0 }, kikan[32]={ 0 };
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString("SELECTCODSN", sqlsvr, apno, cocode, kikan);
	n_apno = atoi(apno);
	n_cocode = atoi(cocode);
	n_kikan = atoi(kikan);

	CDBZmSub	*m_pZmsub = NULL;
	if( (m_pZmsub = new CDBZmSub()) == NULL ){
		AfxMessageBox(_T("エラー"));
		return;
	}
	m_pZmsub->ExitMode(1);

	if( m_pZmsub->SetMasterDB(3, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
		ICSMessageBox(m_pZmsub->GetZmErrMsg());
		if( m_pZmsub ){
			delete m_pZmsub;
			m_pZmsub = NULL;
		}
		return;
	}

	// ボリュームラベル
	if( m_pZmsub->zvol == NULL ){
		if( m_pZmsub->VolumeOpen() == -1 ){
			ICSMessageBox(m_pZmsub->GetZmErrMsg());
			if( m_pZmsub ){
				delete m_pZmsub;
				m_pZmsub = NULL;
			}
			return;
		}
	}

	CSyzMakeSub	syzmk;
	if( syzmk.CheckWrongMzSrData(m_pZmsub) ){
		ICSMessageBox(syzmk.GetErrorMsg());
	}

	if( m_pZmsub ){
		delete m_pZmsub;
		m_pZmsub = NULL;
	}
}
