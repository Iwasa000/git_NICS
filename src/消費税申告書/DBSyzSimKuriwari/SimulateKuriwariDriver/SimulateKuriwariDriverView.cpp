// ICS Application1View.cpp : CSimulateKuriwariDriverView クラスの実装
//

#include "stdafx.h"
#include "SimulateKuriwariDriver.h"

#include "MainFrm.h"
#include "SimulateKuriwariDriverDoc.h"
#include "SimulateKuriwariDriverView.h"

#include "../SimulateKuriwari/SimulateKuriwari.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimulateKuriwariDriverView

IMPLEMENT_DYNCREATE(CSimulateKuriwariDriverView, ICSFormView)

BEGIN_MESSAGE_MAP(CSimulateKuriwariDriverView, ICSFormView)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, ICSFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, ICSFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CSimulateKuriwariDriverView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSimulateKuriwariDriverView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSimulateKuriwariDriverView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSimulateKuriwariDriverView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSimulateKuriwariDriverView::OnBnClickedButton5)
END_MESSAGE_MAP()


// CSimulateKuriwariDriverView コンストラクション/デストラクション

CSimulateKuriwariDriverView::CSimulateKuriwariDriverView()
	: ICSFormView(CSimulateKuriwariDriverView::IDD)
{
	// TODO: 構築コードをここに追加します。
}

CSimulateKuriwariDriverView::~CSimulateKuriwariDriverView()
{
}

void CSimulateKuriwariDriverView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
}

BOOL CSimulateKuriwariDriverView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return ICSFormView::PreCreateWindow(cs);
}

void CSimulateKuriwariDriverView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();
}


// CSimulateKuriwariDriverView 診断

#ifdef _DEBUG
void CSimulateKuriwariDriverView::AssertValid() const
{
	ICSFormView::AssertValid();
}

void CSimulateKuriwariDriverView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}

CSimulateKuriwariDriverDoc* CSimulateKuriwariDriverView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimulateKuriwariDriverDoc)));
	return (CSimulateKuriwariDriverDoc*)m_pDocument;
}
#endif //_DEBUG

// CSimulateKuriwariDriverViewクラスの印刷

BOOL CSimulateKuriwariDriverView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CSimulateKuriwariDriverView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CSimulateKuriwariDriverView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	ICSFormView::OnPrepareDC(pDC, pInfo);
}

void CSimulateKuriwariDriverView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CSimulateKuriwariDriverView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: コントロールを印刷するコードを追加してください。
}

// CSimulateKuriwariDriverView メッセージ ハンドラ
void CSimulateKuriwariDriverView::OnButtonF4()
{
	ICSFormView::OnButtonF4();
}

void CSimulateKuriwariDriverView::OnButtonF5()
{
	ICSFormView::OnFilePrint();
}



void CSimulateKuriwariDriverView::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	CDBZmSub* pZmSub = ((CMainFrame*)AfxGetMainWnd())->m_Zmsub;
	CSimKuriwari SimK(pZmSub);

    SimK.CreateDlg(this);
}


void CSimulateKuriwariDriverView::OnBnClickedButton2()
{

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CSimKuriwari SimK(((CMainFrame*)AfxGetMainWnd())->m_Zmsub);

	SIMKURIWARIZREC Rec;

	SimK.SyukeiKuriwari10Eigyobi(&Rec);
	//SimK.SyukeiKuriwari10Eigyobi(&Rec, 20191001, 20191031);
	//SimK.SyukeiKuriwari10Eigyobi(&Rec, 20191101, 20191131);
  
}


void CSimulateKuriwariDriverView::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CSimKuriwari SimK(((CMainFrame*)AfxGetMainWnd())->m_Zmsub);

	SIMKSIREWARIZREC Rec;
	int IsNeedTkkz = 0;	// ０固定（特定課税仕入れなし）
	SimK.SyukeiKsirewari(&Rec, IsNeedTkkz);
	//SimK.SyukeiKsirewari(&Rec, 20191001, 20191031);
	//SimK.SyukeiKsDEirewari(&Rec, 20191101, 20191131);

}


void CSimulateKuriwariDriverView::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CSimKuriwari SimK(((CMainFrame*)AfxGetMainWnd())->m_Zmsub);

	SIMKURIWARIZREC Rec;
	SimK.ReadDlgSyukeiData(&Rec);

}


void CSimulateKuriwariDriverView::OnBnClickedButton5()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDBZmSub* pZmSub = ((CMainFrame*)AfxGetMainWnd())->m_Zmsub;
	CSimKuriwari SimK(pZmSub);

	SIMKSIREWARIZREC sirewariRec;
	int IsNeedTkkz = 0;	// ０固定（特定課税仕入れなし）
	SimK.SyukeiKsirewari(&sirewariRec, IsNeedTkkz);

	SIMKURIWARIZREC uriwariRec;
	SimK.SyukeiKuriwari10Eigyobi(&uriwariRec);

	SimK.CreateSyukeiDlg(this, 0, 0);

	//SimK.CreateSyukeiDlg(this, 20191001, 20191031);
	//SimK.CreateSyukeiDlg(this, 20191001, 20191010);

}

