// 改良No.22-0404,22-0812 add -->
// dlgReferenceDelChk1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgReferenceDelChk1.h"
#include "frmUc000Common.h"

// 修正No.162348 add -->
// ユーザーメッセージWPARAM
#define			UM1_POPULATE	0
#define			UM1_REPINIZ		1
// 修正No.162348 add <--

// CdlgReferenceDelChk1 ダイアログ

IMPLEMENT_DYNAMIC(CdlgReferenceDelChk1, ICSDialog)

CdlgReferenceDelChk1::CdlgReferenceDelChk1(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgReferenceDelChk1::IDD, pParent)
{
	m_pOwner = pParent;
}

CdlgReferenceDelChk1::CdlgReferenceDelChk1(UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
	m_pOwner = pParent;
}

CdlgReferenceDelChk1::~CdlgReferenceDelChk1()
{
}

void CdlgReferenceDelChk1::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_STC1, m_stcMsg1);
	DDX_Control(pDX, IDC_MSG_STC2, m_stcMsg2);
	DDX_Control(pDX, IDC_MSG_STC3, m_stcMsg3);
	DDX_Control(pDX, IDC_MSG_STC4, m_stcMsg4);
	DDX_Control(pDX, IDC_MSG_STC5, m_stcMsg5);
	DDX_Control(pDX, IDC_MSG_STC6, m_stcMsg6);
	DDX_Control(pDX, IDC_MSG_STC7, m_stcMsg7);		// 修正No.162360,162361 add
	DDX_Control(pDX, IDC_MSG_STC8, m_stcMsg8);		// 修正No.162360,162361 add
	DDX_Control(pDX, IDC_MSG_STC9, m_stcMsg9);		// 修正No.162389 add
	DDX_Control(pDX, IDC_MSG_STC10, m_stcMsg10);	// 修正No.162435 add
	DDX_Control(pDX, IDC_REPORT_CSTM, m_report);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CdlgReferenceDelChk1, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgReferenceDelChk1::OnBnClickedOk)
	ON_MESSAGE(WM_USER_1, OnUser1Message)	// 修正No.162348 add
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgReferenceDelChk1, ICSDialog)
END_EVENTSINK_MAP()

// CdlgReferenceDelChk1 メッセージ ハンドラー

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
void CdlgReferenceDelChk1::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
HBRUSH CdlgReferenceDelChk1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//------------------------------------------------------------------------------------------------------
//	ダイアログ表示
//------------------------------------------------------------------------------------------------------
BOOL CdlgReferenceDelChk1::ShowDialog(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq,
//									  CString sName1, CString sName2, CString sName3, CArray<int,int> &cDelFormNo)					// 修正No.162360,162361 del
									  CString sName1, CString sName2, CString sName3, CString sName4, CArray<int,int> &cDelFormNo)	// 修正No.162360,162361 add
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
				m_Name3 = sName3;				// 表示名称３
				m_Name4 = sName4;				// 表示名称４				// 修正No.162360,162361 add
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
BOOL CdlgReferenceDelChk1::OnInitDialog()
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
// 修正No.162360,162361,162389,162435 del -->
//void CdlgReferenceDelChk1::DispInit(void)
//{
//	CString			nam1,nam2,nam3;
//	CString			cs1,cs2,cs3,cs4,cs5,cs6;
//
//	// ダイアログのタイトルを設定
//	this->SetWindowText(_T("[F7]削除"));
//
//	// メッセージを作成
//	if(m_nType == ID_DLGTYPE_BANK)	{	// 金融機関
//		// 金融機関名
//		nam1.Format(m_Name1);
//		nam1.TrimRight();
//		cs1.Format(_T("金融機関［%s］"), nam1);
//		// 支店名
//		nam2.Format(m_Name2);
//		nam2.TrimRight();
//		cs2.Format(_T("支 店 名［%s］"), nam2);
//		// 残りメッセージ
//		cs3.Format(_T("は、下記の様式で使用されています。"));
//		cs4.Format(_T("削除してもよろしいですか？"));
//		//cs5.Format(_T("※削除した場合は下記の様式から削除され、空欄で表示されます。"));		// 改良No.22-0404,22-0812_230414 del
//		cs5.Format(_T("※削除した場合は下記の様式から削除され、帳表上空欄で表示されます。"));	// 改良No.22-0404,22-0812_230414 add
//		cs6.Format(_T(""));
//	}
//	else	{							// 取引先
//		// 名称
//		nam1.Format(m_Name1);
//		nam1.TrimRight();
//		if(m_nGrSeq == ID_ADDRESSGR_URI)	cs1 = _T("取引先(得意先)");
//		else								cs1 = _T("取引先(仕入先)");
//		cs1.Format(cs1 + _T("名称［%s］"), nam1);
//		// 住所(上段)
//		nam2.Format(m_Name2);
//		nam2.TrimRight();
//		cs2.Format(_T("所在地［%s］"), nam2);
//
//		// 住所(下段)
//		nam3.Format(m_Name3);
//		nam3.TrimRight();
//		// 住所(下段) 有り無しでメッセージの場所を変更
//		if(nam3.IsEmpty() == FALSE)	{
//			cs3.Format(_T("　　　［%s］"), nam3);
//			// 残りメッセージ
//			cs4.Format(_T("は、下記の様式で使用されています。"));
//			cs5.Format(_T("削除してもよろしいですか？"));
//			//cs6.Format(_T("※削除した場合は下記の様式から削除され、空欄で表示されます。"));		// 改良No.22-0404,22-0812_230414 del
//			cs6.Format(_T("※削除した場合は下記の様式から削除され、帳表上空欄で表示されます。"));	// 改良No.22-0404,22-0812_230414 add
//		}
//		else	{
//			// 残りメッセージ
//			cs3.Format(_T("は、下記の様式で使用されています。"));
//			cs4.Format(_T("削除してもよろしいですか？"));
//			//cs5.Format(_T("※削除した場合は下記の様式から削除され、空欄で表示されます。"));		// 改良No.22-0404,22-0812_230414 del
//			cs5.Format(_T("※削除した場合は下記の様式から削除され、帳表上空欄で表示されます。"));	// 改良No.22-0404,22-0812_230414 add
//			cs6.Format(_T(""));
//		}
//	}
//
//	m_stcMsg1.SetWindowTextA(cs1);
//	m_stcMsg2.SetWindowTextA(cs2);
//	m_stcMsg3.SetWindowTextA(cs3);
//	m_stcMsg4.SetWindowTextA(cs4);
//	m_stcMsg5.SetWindowTextA(cs5);
//	m_stcMsg6.SetWindowTextA(cs6);
//}
// 修正No.162360,162361,162389,162435 del <--
// 修正No.162360,162361,162389,162435 add -->
void CdlgReferenceDelChk1::DispInit(void)
{
	int				cnt;
	CString			nam1,nam2,nam3,nam4;
	CString			cst[10];

	// ダイアログのタイトルを設定
	this->SetWindowText(_T("[F7]削除"));

	for(int ii=0; ii<10; ii++)	{
		cst[ii].Empty();
	}

	cnt = 0;
	// メッセージを作成
	if(m_nGrSeq == ID_ADDRESSGR_URI)	cst[cnt] = _T("取引先(得意先)");
	else								cst[cnt] = _T("取引先(仕入先)");
	cnt++;

	// 名称(上段)
	nam1.Format(m_Name1);
	nam1.TrimRight();
	//cst[cnt].Format(_T("名　称［%s］"), nam1);
	cst[cnt].Format(_T("名　称(上段)：%s"), nam1);
	cnt++;
	// 名称(下段)
	nam2.Format(m_Name2);
	nam2.TrimRight();
	//if(nam2.IsEmpty() == FALSE) {
	//cst[cnt].Format(_T("　　　［%s］"), nam2);
	cst[cnt].Format(_T("　　　(下段)：%s"), nam2);
	cnt++;
	//}

	// 住所(上段)
	nam3.Format(m_Name3);
	nam3.TrimRight();
	cst[cnt].Format(_T("所在地(上段)：%s"), nam3);
	cnt++;
	// 住所(下段) 
	nam4.Format(m_Name4);
	nam4.TrimRight();
	//if(nam4.IsEmpty() == FALSE) {
	cst[cnt].Format(_T("　　　(下段)：%s"), nam4);
	cnt++;
	//}

	// 残りメッセージ
	cst[cnt].Format(_T("は、下記の様式で使用されています。"));
	cnt++;
	cst[cnt].Format(_T("削除してもよろしいですか？"));
	cnt++;
	cnt++;
	cst[cnt].Format(_T("※削除した場合は下記の様式及び保管参照からも削除され、"));
	cnt++;
	cst[cnt].Format(_T("　該当項目は空欄で表示されます。"));
	cnt++;

	m_stcMsg1.SetWindowTextA(cst[0]);
	m_stcMsg2.SetWindowTextA(cst[1]);
	m_stcMsg3.SetWindowTextA(cst[2]);
	m_stcMsg4.SetWindowTextA(cst[3]);
	m_stcMsg5.SetWindowTextA(cst[4]);
	m_stcMsg6.SetWindowTextA(cst[5]);
	m_stcMsg7.SetWindowTextA(cst[6]);
	m_stcMsg8.SetWindowTextA(cst[7]);
	m_stcMsg9.SetWindowTextA(cst[8]);
	m_stcMsg10.SetWindowTextA(cst[9]);
}
// 修正No.162360,162361,162389,162435 add <--

//------------------------------------------------------------------------------------------------------
//	レポートの初期化
//------------------------------------------------------------------------------------------------------
int CdlgReferenceDelChk1::ReportInit(void)
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
void CdlgReferenceDelChk1::SetReportGrid(void)
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
LRESULT CdlgReferenceDelChk1::OnUser1Message(WPARAM wParam, LPARAM lParam)
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
int CdlgReferenceDelChk1::ReportSet(void)
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
BOOL CdlgReferenceDelChk1::PreTranslateMessage(MSG* pMsg)
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
void CdlgReferenceDelChk1::OnBnClickedOk()
{
	m_nRet = ID_DLG_OK;
	ICSDialog::OnOK();
}

//------------------------------------------------------------------------------------------------------
//	OnOkイベント
//------------------------------------------------------------------------------------------------------
void CdlgReferenceDelChk1::OnOK()
{
}

// 改良No.22-0404,22-0812 add <--
