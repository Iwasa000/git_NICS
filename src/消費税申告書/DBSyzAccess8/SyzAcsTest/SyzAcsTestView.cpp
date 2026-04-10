// SyzAcsTestView.cpp : CSyzAcsTestView クラスの実装
//

#include "stdafx.h"
#include "SyzAcsTest.h"

#include "MainFrm.h"
#include "SyzAcsTestDoc.h"
#include "SyzAcsTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSyzAcsTestView

IMPLEMENT_DYNCREATE(CSyzAcsTestView, ICSFormView)

BEGIN_MESSAGE_MAP(CSyzAcsTestView, ICSFormView)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, ICSFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, ICSFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CSyzAcsTestView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSyzAcsTestView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSyzAcsTestView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSyzAcsTestView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSyzAcsTestView::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSyzAcsTestView::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSyzAcsTestView::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSyzAcsTestView::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CSyzAcsTestView::OnBnClickedButton9)
END_MESSAGE_MAP()


// CSyzAcsTestView コンストラクション/デストラクション

CSyzAcsTestView::CSyzAcsTestView()
	: ICSFormView(CSyzAcsTestView::IDD)
{
	// TODO: 構築コードをここに追加します。
}

CSyzAcsTestView::~CSyzAcsTestView()
{
}

void CSyzAcsTestView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
}

BOOL CSyzAcsTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return ICSFormView::PreCreateWindow(cs);
}

void CSyzAcsTestView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();
}


// CSyzAcsTestView 診断

#ifdef _DEBUG
void CSyzAcsTestView::AssertValid() const
{
	ICSFormView::AssertValid();
}

void CSyzAcsTestView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}

CSyzAcsTestDoc* CSyzAcsTestView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSyzAcsTestDoc)));
	return (CSyzAcsTestDoc*)m_pDocument;
}
#endif //_DEBUG

// CSyzAcsTestViewクラスの印刷

BOOL CSyzAcsTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CSyzAcsTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CSyzAcsTestView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	ICSFormView::OnPrepareDC(pDC, pInfo);
}

void CSyzAcsTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CSyzAcsTestView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: コントロールを印刷するコードを追加してください。
}

// CSyzAcsTestView メッセージ ハンドラ
void CSyzAcsTestView::OnButtonF4()
{
	ICSFormView::OnButtonF4();
}

void CSyzAcsTestView::OnButtonF5()
{
	ICSFormView::OnFilePrint();
}


void CSyzAcsTestView::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT( pMainFrame );
	ASSERT( pMainFrame->m_pDBSyzAccess );
	ASSERT( pMainFrame->m_pZmsub );

	if( pMainFrame->m_pDBSyzAccess->OpenShLstVal(pMainFrame->m_pZmsub->m_database) ){
		ICSMessageBox( pMainFrame->m_pDBSyzAccess->m_ErrMsg );
		return;
	}
}

void CSyzAcsTestView::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT( pMainFrame );
	ASSERT( pMainFrame->m_pDBSyzAccess );

	pMainFrame->m_pDBSyzAccess->CloseShLstVal();
}

void CSyzAcsTestView::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT( pMainFrame );
	ASSERT( pMainFrame->m_pDBSyzAccess );
	ASSERT( pMainFrame->m_pZmsub );

	if( pMainFrame->m_pDBSyzAccess->OpenShLstRate(pMainFrame->m_pZmsub->m_database) ){
		ICSMessageBox( pMainFrame->m_pDBSyzAccess->m_ErrMsg );
		return;
	}
}

void CSyzAcsTestView::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT( pMainFrame );
	ASSERT( pMainFrame->m_pDBSyzAccess );

	pMainFrame->m_pDBSyzAccess->CloseShLstRate();
}


void CSyzAcsTestView::OnBnClickedButton5()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainFrame);
	ASSERT(pMainFrame->m_pDBSyzAccess);

	DbgViewTrace(_T("【DEBUG】%d %s()\n"), __LINE__, __FUNCTION__);
	pMainFrame->m_pDBSyzAccess->SzsyksubOpen(pMainFrame->m_pZmsub->m_database);
    
}


void CSyzAcsTestView::OnBnClickedButton6()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainFrame);
	ASSERT(pMainFrame->m_pDBSyzAccess);

	DbgViewTrace(_T("【DEBUG】%d %s()\n"), __LINE__, __FUNCTION__);
	pMainFrame->m_pDBSyzAccess->SzsyksubClose();
}


void CSyzAcsTestView::OnBnClickedButton7()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainFrame);
	ASSERT(pMainFrame->m_pDBSyzAccess);

	DbgViewTrace(_T("【DEBUG】%d %s()\n"), __LINE__, __FUNCTION__);

    CArray<ZeiCalqEachMonth, ZeiCalqEachMonth&> Array;
	unsigned char Sn_TUMIAG = 0x02;
	pMainFrame->m_pDBSyzAccess->GetSyzZeiCalqForEachMonth2(pMainFrame->m_pZmsub, &Array, Sn_TUMIAG);

	ZeiCalqEachMonth ArrayItem;
    int ArrayCount = Array.GetCount();
    for(int i = 0; i < ArrayCount; i++){
        ArrayItem = Array.GetAt(i);
		DbgViewTrace(_T("【DEBUG】%d %s() month_info_ofs:%02d zeicalq_uri:%d zeicalq_sire:%d\n"),
			__LINE__, __FUNCTION__, 
			ArrayItem.month_info_ofs, 
			ArrayItem.zeicalq_uri, 
			ArrayItem.zeicalq_sire);
	}
    
}

void CSyzAcsTestView::OnBnClickedButton8()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainFrame);
	ASSERT(pMainFrame->m_pDBSyzAccess);

	long smd = 20240701;
	long emd = 20240720;
    bool bMixUriage, bMixSire;
    int rt = pMainFrame->m_pDBSyzAccess->CheckSyzZeiCalqSireKonzai(pMainFrame->m_pZmsub, smd, emd, &bMixUriage, &bMixSire);

	DbgViewTrace(_T("【DEBUG】%d %s() rt:%d 期間smd:%d-emd:%d bMixUriage:%d bMixSire:%d\n"), 
		__LINE__, __FUNCTION__, rt, smd, emd, bMixUriage, bMixSire);
}


void CSyzAcsTestView::OnBnClickedButton9()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CMainFrame	*pMainFrame = NULL;
	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainFrame);
	ASSERT(pMainFrame->m_pDBSyzAccess);

	bool bOver = false;
	long smd, emd;
	char syukei_money[6] = { 0 };

	smd = 20230401;
	emd = 20240331;

	bOver = pMainFrame->m_pDBSyzAccess->IsOver5okuYen(pMainFrame->m_pZmsub, smd, emd, &syukei_money[0]);

	CArithEx ArithEx;
	ArithEx.l_defn(0x16);
	char buf[256] = { 0 };

	DbgViewTrace(_T("【DEBUG】%d %s() bOver:%d sykeimoney:%s\n"), __LINE__, __FUNCTION__, bOver, ArithEx.l_print(buf, &syukei_money[0], _T("s,sss,sss,sss,ss9")));

}
