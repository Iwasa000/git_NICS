// 改良No.22-0404,22-0812 add -->
// 修正No.162360,162361 add -->
// dlgReferenceDelChk2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgReferenceDelChk2.h"
#include "frmUc000Common.h"

// 修正No.162348 add -->
// ユーザーメッセージWPARAM
#define			UM1_POPULATE	0
#define			UM1_REPINIZ		1
// 修正No.162348 add <--

// CdlgReferenceDelChk2 ダイアログ

IMPLEMENT_DYNAMIC(CdlgReferenceDelChk2, ICSDialog)

CdlgReferenceDelChk2::CdlgReferenceDelChk2(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgReferenceDelChk2::IDD, pParent)
{
	m_pOwner = pParent;
}

CdlgReferenceDelChk2::CdlgReferenceDelChk2(UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
	m_pOwner = pParent;
}

CdlgReferenceDelChk2::~CdlgReferenceDelChk2()
{
}

void CdlgReferenceDelChk2::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_STC1, m_stcMsg1);
	DDX_Control(pDX, IDC_MSG_STC2, m_stcMsg2);
	DDX_Control(pDX, IDC_MSG_STC3, m_stcMsg3);
	DDX_Control(pDX, IDC_MSG_STC4, m_stcMsg4);
	DDX_Control(pDX, IDC_MSG_STC5, m_stcMsg5);
	DDX_Control(pDX, IDC_MSG_STC6, m_stcMsg6);
	DDX_Control(pDX, IDC_MSG_STC7, m_stcMsg7);
	DDX_Control(pDX, IDC_REPORT_CSTM, m_report);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CdlgReferenceDelChk2, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgReferenceDelChk2::OnBnClickedOk)
	ON_MESSAGE(WM_USER_1, OnUser1Message)	// 修正No.162348 add
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgReferenceDelChk2, ICSDialog)
END_EVENTSINK_MAP()


// CdlgReferenceDelChk2 メッセージ ハンドラー

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
void CdlgReferenceDelChk2::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
HBRUSH CdlgReferenceDelChk2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//------------------------------------------------------------------------------------------------------
//	ダイアログ表示
//------------------------------------------------------------------------------------------------------
BOOL CdlgReferenceDelChk2::ShowDialog(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq,
									  CString sName1, CString sName2, CArray<int,int> &cDelFormNo)
{
	m_nRet = ID_DLG_CANCEL;		//	戻値初期化

	// データベースハンドルがヌルじゃない？
	if(pDB != NULL)	{
		try	{
			// データベース開いてる？
			if(pDB->IsOpen())	{
				m_pDB = pDB;					// データベースハンドル取得
				m_nType = nType;				// 参照型取得
				m_nFormSeq = nFormSeq;			// 様式シーケンス番号取得
				m_nGrSeq = nGrSeq;				// グループ番号取得
				m_Name1 = sName1;				// 表示名称１
				m_Name2 = sName2;				// 表示名称２
				m_DelFormNo.Copy(cDelFormNo);	// 使用されている様式番号

				DoModal();					//	モーダルで表示
			}
		}
		catch(...)	{
		}
	}

	// 戻値を返す
	return(m_nRet);
}

//------------------------------------------------------------------------------------------------------
//	ダイアログ初期化
//------------------------------------------------------------------------------------------------------
BOOL CdlgReferenceDelChk2::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 財務dbクラス
	m_pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	// メッセージの作成
	DispInit();

	// レポートの初期化
	ReportInit();

	// レポートの表示
	ReportSet();

	// リサイズ
	//OnInitDialogEX();
	ICSDialog::OnInitDialogEX();

	// フォーカスを一旦レポートコントロールにあてる
	m_report.SetFocus();
	m_report.SetFocusedRow(0);

	// フォーカスをキャンセルボタンに移動
	m_btnCancel.SetFocus();
	c_set(IDCANCEL);

	// 戻値を返す
	//return(TRUE);
	return(FALSE);
}

//------------------------------------------------------------------------------------------------------
//	メッセージの作成
//------------------------------------------------------------------------------------------------------
void CdlgReferenceDelChk2::DispInit(void)
{
	int				cnt;
	CString			nam1,nam2,nam3,nam4;
	CString			cst[9];

	// ダイアログのタイトルを設定
	this->SetWindowText(_T("[F7]削除"));

	for(int ii=0; ii<9; ii++)	{
		cst[ii].Empty();
	}

	cnt = 0;
	// メッセージを作成
	cst[cnt] = _T("金融機関");
	cnt++;

// 修正No.162389,162435 del -->
	//// 金融機関名
	//nam1.Format(m_Name1);
	//nam1.TrimRight();
	//cst[cnt].Format(_T("金融機関名［%s］"), nam1);
	//cnt++;

	//// 支店名
	//nam2.Format(m_Name2);
	//nam2.TrimRight();
	//cst[cnt].Format(_T("支　店　名［%s］"), nam2);
	//cnt++;

	//// 残りメッセージ
	//cst[cnt].Format(_T("は、下記の様式で使用されています。"));
	//cnt++;
	//cst[cnt].Format(_T("削除してもよろしいですか？"));
	//cnt++;
	//cst[cnt].Format(_T("※削除した場合は下記の様式から削除され、帳表上空欄で表示されます。"));
	//cnt++;
// 修正No.162389,162435 del <--
// 修正No.162389,162435 add -->
	// 金融機関名
	nam1.Format(m_Name1);
	nam1.TrimRight();
	cst[cnt].Format(_T("金融機関名：%s"), nam1);
	cnt++;

	// 支店名
	nam2.Format(m_Name2);
	nam2.TrimRight();
	cst[cnt].Format(_T("支　店　名：%s"), nam2);
	cnt++;

	// 残りメッセージ
	cst[cnt].Format(_T("は、下記の様式で使用されています。"));
	cnt++;
	cst[cnt].Format(_T("削除してもよろしいですか？"));
	cnt++;

	cst[cnt].Format(_T("※削除した場合は下記の様式及び保管参照からも削除され、"));
	cnt++;
	cst[cnt].Format(_T("　該当項目は空欄で表示されます。"));
// 修正No.162389,162435 add <--

	m_stcMsg1.SetWindowTextA(cst[0]);
	m_stcMsg2.SetWindowTextA(cst[1]);
	m_stcMsg3.SetWindowTextA(cst[2]);
	m_stcMsg4.SetWindowTextA(cst[3]);
	m_stcMsg5.SetWindowTextA(cst[4]);
	m_stcMsg6.SetWindowTextA(cst[5]);
	m_stcMsg7.SetWindowTextA(cst[6]);
}

//------------------------------------------------------------------------------------------------------
//	レポートの初期化
//------------------------------------------------------------------------------------------------------
int CdlgReferenceDelChk2::ReportInit(void)
{
	CFont*				pFont;
	LOGFONT				font={0};
	ICSReportColumn		*_pRepClm;

	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);

	_pRepClm = m_report.AddColumn(new ICSReportColumn(0, _T("様式番号"), 16));
	_pRepClm = m_report.AddColumn(new ICSReportColumn(1, _T("様式名称"), 84));

	_pRepClm->AllowRemove(FALSE);			// 修正No.162348 add

	// ドラッグによるカラムの削除不可
	m_report.SetSortable(FALSE);			// ソート禁止
	m_report.SetDraggable(FALSE);			// ヘッダードラッグ禁止
	m_report.SetRemovable(FALSE);			// ヘッダー削除禁止
	m_report.SetMultipleSelection(FALSE);	// 複数行選択禁止

	SetReportGrid();						// グリッド線の設定

// 修正No.162348 del -->
	//// フォント設定
	//pFont = this->GetFont();
	//pFont->GetLogFont(&font);
	//m_report.SetTextFont(font);
	//m_report.SetHeaderFont(font);
// 修正No.162348 del <--
// 修正No.162348 add -->
	// セットした情報でコントロールを描画する
	m_report.Populate();

	// フォント設定
	PostMessage(WM_USER_1, UM1_REPINIZ);
// 修正No.162348 add <--

	return(0);
}

//------------------------------------------------------------------------------------------------------
//	レポート　グリッド線の設定
//------------------------------------------------------------------------------------------------------
void CdlgReferenceDelChk2::SetReportGrid(void)
{
	CDWordArray		arryGridColumn;
	CDWordArray		arrayGridColor;

	arryGridColumn.Add(0);
	arryGridColumn.Add(1);

	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));

	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}

// 修正No.162348 add -->
//------------------------------------------------------------------------------------------------------
//	フォントの設定
//------------------------------------------------------------------------------------------------------
LRESULT CdlgReferenceDelChk2::OnUser1Message(WPARAM wParam, LPARAM lParam)
{
	WORD	MsgID = LOWORD(wParam);
	WORD	WndID = HIWORD(wParam);
	WORD	PAR1 = LOWORD(lParam);
	WORD	PAR2 = HIWORD(lParam);
	
	if(MsgID == UM1_POPULATE)	{
		// セットした情報でコントロールを描画する。
		m_report.Populate();
	}
	else if(MsgID == UM1_REPINIZ)	{
		CFont*	listfont;
		LOGFONT	LogFont;
		CRect	rect;
		CWnd*	ctlwnd = m_pWnd->GetDlgItem(IDC_STC99);

		// フォントの設定
		listfont = ctlwnd->GetFont();
		listfont->GetLogFont(&LogFont);
		//LogFont.lfHeight = LogFont.lfHeight * 108 / 100;
		//LogFont.lfWidth  = LogFont.lfWidth  * 108 / 100;
		//LogFont.lfWeight = LogFont.lfWeight * 100 / 100;
		m_report.SetTextFont(LogFont);
		listfont->GetLogFont(&LogFont);
		//LogFont.lfHeight = LogFont.lfHeight * 108 / 100;
		//LogFont.lfWidth  = LogFont.lfWidth  * 108 / 100;
		//LogFont.lfWeight = LogFont.lfWeight * 100 / 100;
		m_report.SetHeaderFont(LogFont);

		// タイトルの高さを設定
		ctlwnd->GetWindowRect(rect);
		m_report.SetHeaderHeight(rect.Height());
		m_report.SetItemHeight(rect.Height());

		// セットした情報でコントロールを描画する
		m_report.Populate();
	}

	return(0);
}
// 修正No.162348 add <--

//------------------------------------------------------------------------------------------------------
//	レポートの表示
//------------------------------------------------------------------------------------------------------
int CdlgReferenceDelChk2::ReportSet(void)
{
	int						ii=0;
	int						max=0;
	int						nID;
	CString					strData1,strData2;
	CdbUcInfSub				rsSub(m_pDB);
	ICSReportRecordItem		*pItem = NULL;

	max = m_DelFormNo.GetCount();

	for(ii=0; ii<max; ii++)		{
		nID = m_DelFormNo.GetAt(ii);

		if(rsSub.RequeryFormSeq(nID) == DB_ERR_OK)	{
			if(!rsSub.IsEOF())	{
				strData1 = rsSub.m_TitleNo + rsSub.m_TitleNo2;	// 様式番号
				strData2 = rsSub.m_Title;						// 様式名称
			}
		}
		rsSub.Fin();

		ICSReportRecord*	pRecord = m_report.AddRecord(new ICSReportRecord());
		pItem = pRecord->AddItem(new ICSReportRecordItemText(strData1));
		pItem = pRecord->AddItem(new ICSReportRecordItemText(strData2));
	}

	m_report.Populate();

	return(0);
}

//------------------------------------------------------------------------------------------------------
//	ウィンドウメッセージ
//------------------------------------------------------------------------------------------------------
BOOL CdlgReferenceDelChk2::PreTranslateMessage(MSG* pMsg)
{
	int		sw = 0;
	int		nID = 0;
	char	cShift = (char)GetKeyState(VK_SHIFT);	// SHIFTキー状態取得
	CWnd*	pControl = NULL;						// コントロール

	//	フォーカスのあるコントロールのID取得
	pControl = this->GetFocus();
	if(pControl != NULL)	{
		nID = pControl->GetDlgCtrlID();
	}

	// キーダウンメッセージ？
	if(pMsg->message == WM_KEYDOWN)	{
		// パラメータで分岐
		switch(pMsg->wParam)	{
			case VK_RETURN:			// Enterキー
				break;

			case VK_TAB:			// Tabキー
				break;

			case VK_ESCAPE:			// Escキー
				if(m_report.HasFocus() == TRUE)	{
					// OnCancel起動
					PostMessage(WM_COMMAND, IDCANCEL);
					sw = 1;
				}
				break;

			case VK_F2:				//	F2キー
				keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
				sw = 1;
				break;

			default:
				break;
		}
	}

	if(sw == 1)		return(1);
	return ICSDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------------------------------
//	ボタンクリック（はいボタン）
//------------------------------------------------------------------------------------------------------
void CdlgReferenceDelChk2::OnBnClickedOk()
{
	m_nRet = ID_DLG_OK;
	ICSDialog::OnOK();
}

//------------------------------------------------------------------------------------------------------
//	OnOkイベント
//------------------------------------------------------------------------------------------------------
void CdlgReferenceDelChk2::OnOK()
{
}
// 修正No.162360,162361 add <--
// 改良No.22-0404,22-0812 add <--
