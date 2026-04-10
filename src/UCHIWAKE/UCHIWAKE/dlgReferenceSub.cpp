// インボイス登録番号追加対応_23/11/06 add -->
//--------------------------------------------------
//	dlgReferenceSub.cpp
//
//	2023.11.14～
//	インボイス登録番号追加版に変更
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgreferencesub.h"
#include "CharController.h"
#include "frmUc000Common.h"

IMPLEMENT_DYNAMIC(CdlgReferenceSub, ICSDialog)

extern	BOOL	bG_Kanso;
extern	BOOL	bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）

CSyzInvoice		pSyzInvoice;

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgReferenceSub::CdlgReferenceSub(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgReferenceSub::IDD, pParent)
{
	m_pOwner = pParent;
}

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgReferenceSub::CdlgReferenceSub( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
	m_pOwner = pParent;
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgReferenceSub::~CdlgReferenceSub()
{
// 修正No.168444 add -->
	// テーブルクローズ
	if(bG_InvNo == TRUE)	{	// 修正No.168522 add
		if(opsw[7] == 1)	pRfZmSub->SiwakeTaioTekiyoClose();	opsw[7] = 0;
		if(opsw[6] == 1)	pRfZmSub->KanaTekiyoClose();		opsw[6] = 0;
		if(opsw[5] == 1)	pRfZmSub->RensoTekiyoClose();		opsw[5] = 0;
		if(opsw[4] == 1)	pRfZmSub->BmnameClose();			opsw[4] = 0;
		if(opsw[3] == 1)	pRfZmSub->EdabanClose();			opsw[3] = 0;
		if(opsw[2] == 1)	pRfZmSub->KamokuMeisyoClose();		opsw[2] = 0;
		if(opsw[1] == 1)	pRfZmSub->KamokuMeisyoFree();		opsw[1] = 0;
		if(opsw[0] == 1)	pRfZmSub->VolumeClose();			opsw[0] = 0;
		if(pRfZmSub) {
			pRfZmSub = NULL;
			delete pRfZmSub;
		}
	}
// 修正No.168444 add <--
}

void CdlgReferenceSub::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_DIAG, m_diagInput);
	DDX_Control(pDX, ID_SEARCH, m_btnSearch);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CdlgReferenceSub, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()													// 修正No.168444 add
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgReferenceSub::OnBnClickedOk)
	ON_BN_CLICKED(ID_SEARCH, &CdlgReferenceSub::OnBnClickedSearch)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgReferenceSub, ICSDialog)
	ON_EVENT(CdlgReferenceSub, IDC_INPUT_DIAG, 3, CdlgReferenceSub::TerminationInputDiag, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CdlgReferenceSub, IDC_INPUT_DIAG, 11, CdlgReferenceSub::ReadStringIcsdiag8Kanasearch, VTS_I2 VTS_BSTR)
	ON_EVENT(CdlgReferenceSub, IDC_INPUT_DIAG, 2, CdlgReferenceSub::EditOFFInputDiag, VTS_I2 )
	ON_EVENT(CdlgReferenceSub, IDC_INPUT_DIAG, 1, CdlgReferenceSub::EditONInputDiag, VTS_I2 )				// 修正No.168365,168404 add 
	ON_EVENT(CdlgReferenceSub, IDC_INPUT_DIAG, 8, CdlgReferenceSub::ComboSelInputDiag, VTS_I2 VTS_UNKNOWN)	// 修正No.168377 add 

END_EVENTSINK_MAP()

void CdlgReferenceSub::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

HBRUSH CdlgReferenceSub::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReferenceSub::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 背景色取得
	ICSColorInfo icsColInfo;
	m_ViewColor = GetSysColor(COLOR_BTNFACE);
	if(((ICSWinApp *)AfxGetApp())->GetStanderdColor(icsColInfo) == 0) {
		if(icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON) {
			m_ViewColor = icsColInfo.ViewColor();
		}
	}

	if(bG_InvNo == TRUE) {
		InitZmSelSub();
	}

	//	ディスプレイ初期化
	InitDisplay(m_nType, m_szTitle, m_szOrder, m_szName1, m_szName2, m_szAdd1, m_szAdd2, m_szKana1, m_szKana2, m_szInvno);
	// リサイズ
	OnInitDialogEX();

	//	戻値を返す
	return(TRUE);
}

// 修正No.168444 add -->
void CdlgReferenceSub::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	switch(nState) {
		case	WA_ACTIVE:			// アクティブ化
		case	WA_CLICKACTIVE:		// マウスクリックでアクティブ
			if(bG_InvNo == TRUE && pRfZmSel && pRfZmSel->IsDisplay() != FALSE) {
				// 登録番号ダイアログを更新
				pRfZmSel->ZmselUpdate();
			}
			break;
	}

	ICSDialog::OnActivate(nState, pWndOther, bMinimized);
}
// 修正No.168444 add <--

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（住所検索/登録番号一覧ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub::OnBnClickedSearch()
{
	//	銀行／住所検索／登録番号一覧表示
	ShowSearchDialog(m_nType, 0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub::OnBnClickedOk()
{
	int		nRet;		//	戻値

	//	入力データチェック
	nRet = CheckInputData(m_nType, m_szName1, m_szName2, m_szAdd1, m_szAdd2, m_szKana1, m_szKana2, m_szInvno, m_nGrSeq, m_nSeq);

	//	戻値で分岐
	switch(nRet) {
		//	編集あり（重複なし）
		case 1:
			//	戻値をOKに設定
			m_nRet = ID_DLG_OK;
			break;

		//	編集あり（重複あり）
		case 2:
			ICSMessageBox("既に登録されています。", MB_OK, 0, 0, this);		//	警告メッセージ
			return;															//	抜ける
			break;

		//	入力なし
		case 3:
			ICSMessageBox("入力されていません。", MB_OK, 0, 0, this);		//	警告メッセージ
			return;															//	抜ける
			break;

		// 戻す（電子申告文字数チェックメッセージで「いいえ(N)」を選択）
		case 4:
			return;															//	抜ける

		//	その他（編集なし）
		default:
			break;
	}

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージを処理する前に発生するイベント
//	【引数】	pMsg	…	MSG構造体のポインタ
//	【戻値】	0		…	メッセージを通常の方法で処理
//				0以外	…	メッセージ処理を終了
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReferenceSub::PreTranslateMessage(MSG* pMsg)
{
	char	cShift = 0;		//	SHIFTキー状態
	int		nID = 0;		//	ID
	BOOL	bFlag = FALSE;
	CString	cst = _T("");

	nID = GetFocusControlID(this);			//	現在フォーカスのあるコントロールID取得

	//	検索ボタンの名称を変更
	if(bG_InvNo == TRUE && m_nType == ID_DLGTYPE_ADDRESS)	{
		if(nID != ID_SEARCH && nID != ID_OK_BUTTON && nID != IDCANCEL)	{
			if(m_diagInput.GetPosition() == ID_COL_ADDI_INVNOTSEL || m_diagInput.GetPosition() == ID_COL_ADDI_INVNO)	{
				m_btnSearch.SetWindowText("登録番号一覧");

// 修正No.168377 add -->
// 修正No.168444 del -->
				//if(m_clsFunc.DiagGetCombo(&m_diagInput, ID_COL_ADDI_INVNOTSEL) == 0) {
// 修正No.168444 del <--
// 修正No.168444 add -->
				// 非連動dbは科目設定を開けないため、摘要に登録番号を入力することはないので登録番号選択は表示しない。
				// また、会社登録で「インボイス登録番号の表示」を「表示しない」に設定している場合も出さない。
				if( ((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 0 && !pRfZmSub->zvol->s_sgn8
				   && m_clsFunc.DiagGetCombo(&m_diagInput, ID_COL_ADDI_INVNOTSEL) == 0) {
// 修正No.168444 add <--
					m_btnSearch.EnableWindow(TRUE);
				}
				else {
					m_btnSearch.EnableWindow(FALSE);
				}
// 修正No.168377 add <--
			}
			else {
				m_btnSearch.SetWindowText("住所検索[HOME]");
// 修正No.168377 add -->
				m_btnSearch.EnableWindow(TRUE);
// 修正No.168377 add <--
			}
		}
	}

	//	キーダウンメッセージ？
	if(pMsg->message == WM_KEYDOWN) {
		//	パラメータで分岐
		switch(pMsg->wParam) {
			//	「Tab」キーが押された
			case VK_TAB:
				cShift = (char)GetKeyState(VK_SHIFT);	//	SHIFTキー状態取得

				//	SHIFTキーが押されてる？
				if(cShift & 0x80) {
					//	検索ボタンにフォーカスあり？
					if(nID == ID_OK_BUTTON) {
						//	参照型で分岐
						switch(m_nType) {
							//	金融機関
							case ID_DLGTYPE_BANK:
								//	金融機関用ｶﾅ名テキストに移動
								m_diagInput.SetPosition(ID_COL_BANK_KANA2);
								return(1);
								break;
								//	取引先
							case ID_DLGTYPE_ADDRESS:
								//	取引先用ｶﾅテキストに移動
								if(bG_InvNo == TRUE)	m_diagInput.SetPosition(ID_COL_ADDI_KANA1);
								else					m_diagInput.SetPosition(ID_COL_ADD_KANA1);
								return(1);
								break;
								//	科目
							case ID_DLGTYPE_KAMOKU:
								//	科目用ｶﾅテキストに移動
								m_diagInput.SetPosition(ID_COL_KAMOKU_KANA1);
								return(1);
								break;
						}
					}
				}
				//	SHIFTキーが押されてない
				else {
					//	検索ボタンにフォーカスあり？
					if(nID == ID_SEARCH) {
						//	参照型で分岐
						switch(m_nType) {
							//	金融機関
							case ID_DLGTYPE_BANK:
								//	金融機関テキストに移動
								m_diagInput.SetPosition(ID_COL_BANK_NAME1);
								return(1);
								break;
							//	取引先
							case ID_DLGTYPE_ADDRESS:
								//	名称テキストに移動
								if(bG_InvNo == TRUE)	m_diagInput.SetPosition(ID_COL_ADDI_INVNOTSEL);
								else					m_diagInput.SetPosition(ID_COL_ADD_NAME1);
								return(1);
								break;
						}
					}
					//	キャンセルボタンにフォーカスあり
					else if(nID == IDCANCEL) {
						//	科目？
						if(m_nType == ID_DLGTYPE_KAMOKU) {
							m_diagInput.SetPosition(ID_COL_KAMOKU_NAME1);	//	名称テキストに移動
							return(1);
						}
					}
				}
				break;

			//	「F2」キー押した
			case VK_F2:
				keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
				keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
				keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
				return(1);
				break;

			//	「F7」ボタン押した
			case VK_F7:
				//	金融機関？
				if(m_nType == ID_DLGTYPE_BANK) {
					//	検索ボタン起動
					PostMessage(WM_COMMAND, ID_SEARCH);
				}
				break;

			//	「HOME」ボタン押した
			case VK_HOME:
				//	取引先？
				if(m_nType == ID_DLGTYPE_ADDRESS) {
// 修正No.168362 del -->
					//if(!(bG_InvNo == TRUE && (m_diagInput.GetPosition() == ID_COL_ADDI_INVNOTSEL || m_diagInput.GetPosition() == ID_COL_ADDI_INVNO))) {
					//	//	検索ボタン起動
					//	PostMessage(WM_COMMAND, ID_SEARCH);
					//}
// 修正No.168362 del <--
// 修正No.168362 add -->
					if(bG_InvNo == TRUE && (m_diagInput.GetPosition() == ID_COL_ADDI_INVNOTSEL || m_diagInput.GetPosition() == ID_COL_ADDI_INVNO)) {
						// ---- 登録番号一覧関連 ----->
						if(pRfZmSel && pRfZmSel->IsDisplay() != FALSE) {
							pRfZmSel->PageChange();
							return(TRUE);
						}
						// <--- 登録番号一覧関連 ------
					}
					else {
						//	検索ボタン起動
						PostMessage(WM_COMMAND, ID_SEARCH);
					}
// 修正No.168362 add <--
				}
				break;

			//	↑キーが押された
			case VK_UP:
			//	←キーが押された
			case VK_LEFT:
				//	OKボタンにフォーカスあり？
				if(nID == ID_OK_BUTTON) {
					keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
					keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
					keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
					keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
					return(1);
				}
				break;

			//	↓キーが押された
			case VK_DOWN:
			//	→キーが押された
			case VK_RIGHT:
				//	参照型で分岐
				switch(m_nType) {
					//	金融機関
					case ID_DLGTYPE_BANK:
					//	取引先
					case ID_DLGTYPE_ADDRESS:
						//	検索ボタンにフォーカスあり？
						if(nID == ID_SEARCH) {
							bFlag = TRUE;
						}
						break;

					//	科目
					case ID_DLGTYPE_KAMOKU:
						//	キャンセルボタンにフォーカスあり？
						if(nID == IDCANCEL) {
							bFlag = TRUE;
						}
						break;
				}

				//	フラグオン？
				if(bFlag == TRUE) {
					keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
					keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
					return(1);
				}
				break;

// 修正No.168362 add -->
			// PageUp，PageDownキー
			case VK_PRIOR:
			case VK_NEXT:
				// ---- 登録番号一覧関連 ----->
				if(bG_InvNo == TRUE)	{
					if(pRfZmSel && pRfZmSel->IsDisplay() != FALSE) {
						if(pMsg->wParam == VK_NEXT) {
							pRfZmSel->PageDown();
						}
						else if(pMsg->wParam == VK_PRIOR) {
							pRfZmSel->PageUp();
						}
						return(TRUE);
					}
				}
				// ---- 登録番号一覧関連 <-----
				break;
// 修正No.168362 add <--

			// ESCキーが押された
			case VK_ESCAPE:
				// ---- 登録番号一覧関連 ----->
				if(bG_InvNo == TRUE)	{	// 修正No.168404 add
					if(pRfZmSel && pRfZmSel->IsDisplay() != FALSE) {
						pRfZmSel->DialogOFF();
						return(TRUE);
					}
				}							// 修正No.168404 add
				// ---- 登録番号一覧関連 <-----

				ICSDialog::OnCancel();
				return(1);

				break;

			//	その他
			default:
				break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	入力状態でキーが押された場合のイベント（ICSDiagコントロール）
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReferenceSub::TerminationInputDiag(short index, short nChar, short length, LPUNKNOWN data)
{
	char			cShift = 0;			//	SHIFTキー状態
// 修正No.168425,168443 del -->
	//int				ret=0;
	//CString			szInput,szInputc;
	//CString			cst, cst1, cst2, cst3, sErr;
	//CString			cc, str1, str2, str3;
	//DIAGRAM_DATA	diadata;			// ICSDiag構造体
	//ZmselDBData		tmpzd;				// 修正No.168361,168369,168371,168378 add
// 修正No.168425,168443 del <--

	//	仮想キーコードで分岐
	switch(nChar) {
		//	「Tab」キーが押された
		case VK_TAB:
		//	「Enter」キーが押された
		case VK_RETURN:
// 修正No.168425,168443 del -->
			//// インボイス登録番号使用マスターで参照型が取引先の時
			//if(bG_InvNo == TRUE && m_nType == ID_DLGTYPE_ADDRESS)	{
			//	// 登録番号にカーソルがある場合、登録番号のチェックディジットを行う
			//	if(index == ID_COL_ADDI_INVNO)	{
			//		szInput.Empty();
			//		szInputc.Empty();
			//		cst.Empty();
			//		// 登録番号のデータ取得
			//		cst = m_clsFunc.DiagGetString(&m_diagInput, index);
			//		if(cst.IsEmpty() == FALSE)	{
			//			if(m_clsFunc.DiagGetCombo(&m_diagInput, ID_COL_ADDI_INVNOTSEL) == 0)	{
			//				szInput.Format(_T("T%s"), cst);
			//				szInputc = szInput;					// チェックディジット用
			//			}
			//			else	{
			//				szInput.Format(_T(" %s"), cst);
			//				szInputc.Format(_T("T%s"), cst);	// チェックディジット用
			//			}
			//		}

			//		if(szInput.IsEmpty() == FALSE)	{
			//			//ret = ((CfrmUc000Common *)m_pOwner)->GetBillingProviderHojinInfo(m_pDB, m_nGrSeq, szInput, &cst1, &cst2, &cst3, &sErr, 1);
			//			//if(ret >= 0)	{	//	正常終了（データ取得済み）
			//			//	// 「T」を表示
			//			//	if(ret == 0)	DiagSetDataDisp(ID_COL_ADDI_INVNO_T, "T");
			//			//	else			DiagSetDataDisp(ID_COL_ADDI_INVNO_T, " ");

			//			//	// 名称を表示
			//			//	str1 = str2 = str3 = _T("");
			//			//	cc.Format(_T("%s"), cst1);
			//			//	cc.TrimRight();
			//			//	if(cc.GetLength() > 20)	{
			//			//		str2 = m_clsFunc.GetSpritString(cc, &str1, 20);
			//			//		if(str2.GetLength() > 20)	{
			//			//			cc = str2;
			//			//			str3 = m_clsFunc.GetSpritString(cc, &str2, 20);
			//			//		}
			//			//	}
			//			//	else	{
			//			//		str1 = cc;
			//			//	}
			//			//	cst1.Empty();
			//			//	if(str2.IsEmpty() == FALSE)		cst1 = str1 + _T("\r\n") + str2;
			//			//	else							cst1 = str1;
			//			//	m_clsFunc.DiagInit(&diadata);
			//			//	diadata.data_edit.Format(_T("%s"), cst1);
			//			//	m_diagInput.SetData(ID_COL_ADDI_NAME1, (LPUNKNOWN)&diadata);
			//			//	// 所在地
			//			//	m_clsFunc.DiagInit(&diadata);
			//			//	diadata.data_edit.Format(_T("%s"), cst2);
			//			//	m_diagInput.SetData(ID_COL_ADDI_ADD1, (LPUNKNOWN)&diadata);
			//			//	m_clsFunc.DiagInit(&diadata);
			//			//	diadata.data_edit.Format(_T("%s"), cst3);
			//			//	m_diagInput.SetData(ID_COL_ADDI_ADD2, (LPUNKNOWN)&diadata);
			//			//}
			//			//else	{
			//			//	ICSMessageBox(sErr, MB_ICONEXCLAMATION | MB_OK, 0, 0, this);
			//			//	// 登録番号にカーソルを戻す
			//			//	m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
			//			//	break;
			//			//}

			//			if(szInputc.GetLength() < 14) {
			//				DIAGRAM_DATA	ddata;
			//				ddata.data_combo = 0;
			//				m_diagInput.SetData(ID_COL_ADDI_INVNOTSEL, (LPUNKNOWN)&ddata);
			//				ddata.data_edit.Empty();
			//				m_diagInput.DataClear(ID_COL_ADDI_INVNO, TRUE);

			//				// エラーメッセージを表示
			//				cst  = _T("登録番号（法人番号）の文字数に誤りがあります。\r\n");
			//				cst += _T("登録番号（法人番号）を再入力してください。");
			//				ICSMessageBox(cst, MB_ICONSTOP | MB_OK, 0, 0, this);
			//				m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
			//				break;
			//			}

			//			// 指定された登録番号が正しいかどうかチェック
			//			if(pSyzInvoice.CheckInvoiceNumber(szInputc) == -1) {
			//				////	登録番号（T選択）
			//				//DiagSetCombo(ID_COL_ADDI_INVNOTSEL, szInput);
			//				//// 登録番号
			//				//((CfrmUc000Common *)m_pOwner)->InvoNoSplit(szInput, &cst1, &cst2);
			//				//DiagSetDataEdit(ID_COL_ADDI_INVNO, cst2);
			//				DIAGRAM_DATA	ddata;
			//				ddata.data_combo = 0;
			//				m_diagInput.SetData(ID_COL_ADDI_INVNOTSEL, (LPUNKNOWN)&ddata);
			//				ddata.data_edit.Empty();
			//				m_diagInput.DataClear(ID_COL_ADDI_INVNO, TRUE);

			//				// エラーメッセージを表示
			//				//cst  = _T("登録番号（法人番号）に誤りがあります。\r\n誤った番号は保存されません。\r\n\r\n");	// 修正No.168378 del
			//				cst  = _T("登録番号（法人番号）に誤りがあります。\r\n");										// 修正No.168378 add
			//				cst += _T("登録番号（法人番号）を再入力してください。");
			//				ICSMessageBox(cst, MB_ICONSTOP | MB_OK, 0, 0, this);
			//				m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
			//				break;
			//			}
			//		}
			//		else	{
			//			//m_clsFunc.DiagInit(&diadata);
			//			//diadata.data_disp.Format(_T(" "));
			//			//m_diagInput.SetData(ID_COL_ADDI_INVNO_T, (LPUNKNOWN)&diadata);
			//			DiagSetDataDisp(ID_COL_ADDI_INVNO_T, " ");
			//		}
			//	}
			//}
// 修正No.168425,168443 del <--

			cShift = (char)GetKeyState(VK_SHIFT);				//	SHIFTキー状態取得

			//	SHIFTキー押されてて且つ「Tab」キーが押されてる？
			if((cShift & 0x80) && (nChar == VK_TAB)) {
				//	参照型で分岐
				switch(m_nType) {
					//	金融機関
					case ID_DLGTYPE_BANK:
						//	オフセットが3？
						if(index == ID_COL_BANK_NAME1) {
							GetDlgItem(ID_SEARCH)->SetFocus();	//	銀行検索にフォーカス移動
						}
						//	オフセットが3以外
						else {
							m_diagInput.SetPrevPosition();		//	前のテキストにフォーカス移動
						}
						break;

					//	取引先
					case ID_DLGTYPE_ADDRESS:
						if(bG_InvNo == FALSE && index == ID_COL_ADD_NAME1)	{		//	オフセットが8？
							GetDlgItem(ID_SEARCH)->SetFocus();	//	住所検索にフォーカス移動
						}
						else if(bG_InvNo == TRUE && index == ID_COL_ADDI_INVNOTSEL)	{	//	オフセットが26？
							GetDlgItem(ID_SEARCH)->SetFocus();	//	住所検索にフォーカス移動
						}
						else	{													//	オフセットが8と26以外
							m_diagInput.SetPrevPosition();		//	前のテキストにフォーカス移動
						}
						break;

					//	科目
					case ID_DLGTYPE_KAMOKU:
						if(index == ID_COL_KAMOKU_KANA1) {
							m_diagInput.SetPrevPosition();		//	前のテキストにフォーカス移動
						}
						else {
							GetDlgItem(IDCANCEL)->SetFocus();	//	キャンセルボタンにフォーカス移動
						}
						break;
				}

			}
			//	SHIFTキー押されてない又は「Enter」キー押された
			else	{
				//	参照型で分岐
				switch(m_nType)		{
					//	金融機関
					case ID_DLGTYPE_BANK:
						//	オフセットが20？
						if(index == ID_COL_BANK_KANA2) {
							GetDlgItem(ID_OK_BUTTON)->SetFocus();		//	OKボタンにフォーカス移動
						}
						else {
							m_diagInput.SetNextPosition();				//	次のテキストにフォーカス移動
						}
						break;

					//	取引先
					case ID_DLGTYPE_ADDRESS:
						if(bG_InvNo == FALSE && index == ID_COL_ADD_KANA1)	{		//	オフセットが16？
							GetDlgItem(ID_OK_BUTTON)->SetFocus();		//	OKボタンにフォーカス移動
						}
						else if(bG_InvNo == TRUE && index == ID_COL_ADDI_KANA1)	{	//	オフセットが32？
							GetDlgItem(ID_OK_BUTTON)->SetFocus();		//	OKボタンにフォーカス移動
						}
						else	{
							m_diagInput.SetNextPosition();				//	次のテキストにフォーカス移動
						}
						break;

					//	科目
					case ID_DLGTYPE_KAMOKU:
						if(index == ID_COL_KAMOKU_KANA1) {
							GetDlgItem(ID_OK_BUTTON)->SetFocus();		//	OKボタンにフォーカス移動
						}
						else {
							m_diagInput.SetNextPosition();				//	次のテキストにフォーカス移動
						}
						break;
				}
			}
			break;

		//	「Delete」キーが押された
		case VK_DELETE:
			if( !(m_nType == ID_DLGTYPE_ADDRESS && bG_InvNo == TRUE && index == ID_COL_ADDI_INVNOTSEL)) {
				//	データクリア
				m_diagInput.DataClear(index, TRUE);
			}
			break;

			//	その他
		default:
			break;
	}
}

//**************************************************
//	背景色の変更
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub::DiagSetBackColor(short nIndex)
{
	DIAGRAM_ATTRIBUTE	udAtt;		//	属性構造体

	//	初期化
	InitAttr(&udAtt);
	//	属性を取得
	m_diagInput.GetAttribute(nIndex, (LPUNKNOWN)&udAtt);
	//	背景色を変更
	udAtt.attr_bcolor = m_ViewColor;
	//	属性を設定
	m_diagInput.SetAttribute(nIndex, (LPUNKNOWN)&udAtt, TRUE);
}

//**************************************************
//	表示データセット
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub::DiagSetDataDisp(short nIndex, CString szTitle)
{
	DIAGRAM_DATA		udData;		//	データ構造体

	//	初期化
	m_clsFunc.DiagInit(&udData);
	//	データ取得
	m_diagInput.GetData(nIndex, (LPUNKNOWN)&udData);
	//	表示データセット
	udData.data_disp = szTitle;
	//	データ設定
	m_diagInput.SetData(nIndex, (LPUNKNOWN)&udData);
}

//**************************************************
//	セルを非表示
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub::DiagSetEnabled(short nIndex)
{
	//	入力不可に設定（※非表示にしても入力受付してしまうため）
	m_diagInput.ModifyItem(nIndex, DIAG_MDFY_READONLY);
	//	非表示に設定
	m_diagInput.ModifyItem(nIndex, DIAG_MDFY_TRANSPARENT);
}

//**************************************************
//	入力データセット
//	【引数】	nIndex	…	セルインデックス
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub::DiagSetDataEdit(short nIndex, CString szData)
{
	DIAGRAM_DATA		udData;		//	データ構造体

	//	初期化
	m_clsFunc.DiagInit(&udData);
	//	データ取得
	m_diagInput.GetData(nIndex, (LPUNKNOWN)&udData);
	//	入力データセット
	udData.data_edit = szData;
	//	データ設定
	m_diagInput.SetData(nIndex, (LPUNKNOWN)&udData);
}

//********************************************************************************
//	ComboBoxへ設定（登録番号のデータによってComboBoxのインデックスに変更してセット）
//		IN		CICSDiag*		対象コントロール
//				CString			登録番号　先頭文字を見て選択肢を設定する
//				long			科目順序番号
//		RET		なし
//********************************************************************************
void CdlgReferenceSub::DiagSetCombo(int intIndex, CString strData)
{
	DIAGRAM_DATA	udData;			// ICSDiag構造体
	short			sel = 0;
	CString			cs1, cs2;

	// 一番左端の文字が「T」か「空白」かそれ以外か
	cs1 = strData;
	cs2 = cs1.Left(1);
	if(strcmp(cs2, _T("T")) == 0)		sel = 0;
	else if(strcmp(cs2, _T(" ")) == 0)	sel = 1;
	else								sel = 0;

	// 初期化
	m_clsFunc.DiagInit(&udData);

	m_diagInput.GetData(intIndex, (LPUNKNOWN)&udData);

	udData.data_combo = sel;

	m_diagInput.SetData(intIndex, (LPUNKNOWN)&udData);
}

//**************************************************
//	最大入力文字数設定（ANK文字の入力有効も同時に行う）
//	【引数】	nIndex		…	セルインデックス
//				nMax		…	最大入力文字数
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub::DiagSetInputMax(short nIndex, int nMax)
{
	DIAGRAM_ATTRIBUTE	udAtt;		//	属性構造体

	//	初期化
	InitAttr(&udAtt);
	//	属性を取得
	m_diagInput.GetAttribute(nIndex, (LPUNKNOWN)&udAtt);
	//	文字列数
	udAtt.attr_inpmax = nMax;
	// 属性にANK文字の入力有効指定
	udAtt.attr_editattr = DIAG_EATTR_ZENHAN;
	//	属性を設定
	m_diagInput.SetAttribute(nIndex, (LPUNKNOWN)&udAtt, TRUE);
}

//**************************************************
//	ディスプレイ初期化
//	【引数】	nType			…	参照型
//				szTitle			…	ダイアログタイトル
//				szOrder			…	順序番号
//				szName1			…	名称1
//				szName2			…	名称2
//				szAdd1			…	住所1
//				szAdd2			…	住所2
//				szKana1			…　ｶﾅ文字1
//				szKana2			…　ｶﾅ文字2
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub::InitDisplay(EnumIdDlgType nType, CString szTitle,
								   CString szOrder, CString szName1,
								   CString szName2, CString szAdd1,
								   CString szAdd2, CString szKana1,
								   CString szKana2, CString szInvno)
{
	int				nCnt;			//	カウント用
	CString			cs1, cs2;
	CString			sTitle;			// 改良No.23-0513 add
	DIAGRAM_DATA	diadata;		// ICSDIAG構造体（T選択）

	//	ダイアログのタイトルを設定
	this->SetWindowText(szTitle);

	// 背景色を指定
	m_diagInput.SetBackColor(m_ViewColor);

	DiagSetBackColor(0);								//	背景色を変更
	DiagSetDataDisp(0, "番号　　　　：");				//	番号ラベル表示
	DiagSetBackColor(1);								//	順序番号の背景色変更
	DiagSetDataDisp(1, szOrder);						//	順序番号データセット

	//	参照型で分岐
	switch(m_nType) {
		//	金融機関
		case ID_DLGTYPE_BANK:
			//	セル6～12を非表示に設定
			for(nCnt = 6; nCnt <= 12; nCnt++) {
				DiagSetEnabled(nCnt);
			}
			//	セル15～18を非表示に設定
			for(nCnt = 15; nCnt <= 18; nCnt++) {
				DiagSetEnabled(nCnt);
			}
			//	セル21～32を非表示に設定
			for(nCnt = 21; nCnt <= 32; nCnt++) {
				DiagSetEnabled(nCnt);
			}

			DiagSetBackColor(ID_COL_BANK_NAME1_T);						//	背景色を変更
			DiagSetDataDisp(ID_COL_BANK_NAME1_T, "金融機関名　：");		//	金融機関名ラベル表示
			DiagSetDataEdit(ID_COL_BANK_NAME1, szName1);				//	金融機関名データセット
			DiagSetInputMax(ID_COL_BANK_NAME1, 10);						//	金融機関名の入力文字数設定
			DiagSetBackColor(ID_COL_BANK_NAME2_T);						//	背景色を変更
			DiagSetDataDisp(ID_COL_BANK_NAME2_T, "支店名　　　：");		//	支店名ラベル表示
			DiagSetDataEdit(ID_COL_BANK_NAME2, szName2);				//	支店名データセット
			DiagSetInputMax(ID_COL_BANK_NAME2, 10);						//	支店名の入力文字数設定
			DiagSetBackColor(ID_COL_BANK_KANA1_T);						//	背景色を変更
			DiagSetDataDisp(ID_COL_BANK_KANA1_T, "金融機関ｶﾅ名：");		//	ｶﾅ名ラベル表示
			DiagSetDataEdit(ID_COL_BANK_KANA1, szKana1);				//	金融機関ｶﾅ名データセット
			DiagSetInputMax(ID_COL_BANK_KANA1, 6);						//	ｶﾅ名の入力文字数設定
			DiagSetBackColor(ID_COL_BANK_KANA2_T);						//	背景色を変更
			DiagSetDataDisp(ID_COL_BANK_KANA2_T, "支店名ｶﾅ名　：");		//	ｶﾅ名ラベル表示
			DiagSetDataEdit(ID_COL_BANK_KANA2, szKana2);				//	支店名ｶﾅ名データセット
			DiagSetInputMax(ID_COL_BANK_KANA2, 6);						//	ｶﾅ名の入力文字数設定
			m_diagInput.SetPosition(ID_COL_BANK_NAME1);					//	初期フォーカス位置に移動
			m_btnSearch.ShowWindow(SW_SHOW);							//	検索ボタン有効
			m_btnSearch.SetWindowText("銀行検索[F7]");					//	検索ボタンに名称設定
			break;

		//	取引先
		case ID_DLGTYPE_ADDRESS:
			if(bG_InvNo == FALSE)	{
				//	セル2～5を非表示に設定
				for(nCnt = 2; nCnt <= 5; nCnt++) {
					DiagSetEnabled(nCnt);
				}
				//　セル13,14を非表示に設定
				for(nCnt = 13; nCnt <= 14; nCnt++) {
					DiagSetEnabled(nCnt);
				}
				//　セル17～20を非表示に設定
				for(nCnt = 17; nCnt <= 20; nCnt++) {
					DiagSetEnabled(nCnt);
				}
				//	セル21～32を非表示に設定
				for(nCnt = 21; nCnt <= 32; nCnt++) {
					DiagSetEnabled(nCnt);
				}

				DiagSetBackColor(ID_COL_ADD_NAME1_T);										//	背景色を変更
				DiagSetDataDisp(ID_COL_ADD_NAME1_T, "名称　　　　：");						//	名称ラベル表示
				DiagSetDataEdit(ID_COL_ADD_NAME1, m_clsFunc.StrDocking(szName1, szName2));	//	名称データセット
				DiagSetInputMax(ID_COL_ADD_NAME1, 20);										//	名称の入力文字数設定
				DiagSetBackColor(ID_COL_ADD_ADD_T);											//	背景色を変更
				DiagSetDataDisp(ID_COL_ADD_ADD_T, "所在地　　　：");						//	所在地ラベル表示
				DiagSetDataEdit(ID_COL_ADD_ADD1, szAdd1);									//	所在地１データセット
				DiagSetInputMax(ID_COL_ADD_ADD1, 20);										//	所在地１の入力文字数設定
				DiagSetDataEdit(ID_COL_ADD_ADD2, szAdd2);									//	所在地２データセット
				DiagSetInputMax(ID_COL_ADD_ADD2, 20);										//	所在地２の入力文字数設定
				DiagSetBackColor(ID_COL_ADD_KANA1_T);										//	背景色を変更
				DiagSetDataDisp(ID_COL_ADD_KANA1_T, "ｶﾅ名　　　　：");						//	ｶﾅ名ラベル表示
				DiagSetDataEdit(ID_COL_ADD_KANA1, szKana1);									//	ｶﾅ名データセット
				DiagSetInputMax(ID_COL_ADD_KANA1, 6);										//	ｶﾅ名の入力文字数設定
				m_diagInput.SetPosition(ID_COL_ADD_NAME1);									//	初期フォーカス位置に移動
				m_btnSearch.ShowWindow(TRUE);												//	検索ボタン有効
				m_btnSearch.SetWindowText("住所検索[HOME]");								//	検索ボタンに名称設定
			}
			else	{
				//	セル2～20を非表示に設定
				for(nCnt = 2; nCnt <= 20; nCnt++) {
					DiagSetEnabled(nCnt);
				}

				// 登録番号のT選択に選択肢を追加
				m_clsFunc.DiagInit(&diadata);
				diadata.text_combo = _T("T\r\n \r\n");
				m_diagInput.SetData(ID_COL_ADDI_INVNOTSEL, (LPUNKNOWN)&diadata);

				DiagSetBackColor(ID_COL_ADDI_INVNO_T);										//	背景色を変更
				DiagSetDataDisp(ID_COL_ADDI_INVNO_T, "登録番号\r\n(法人番号)　：");			//	登録番号ラベル表示
				DiagSetCombo(ID_COL_ADDI_INVNOTSEL, szInvno);								//	登録番号（T選択）
				((CfrmUc000Common *)m_pOwner)->InvoNoSplit(szInvno, &cs1, &cs2);
				DiagSetDataEdit(ID_COL_ADDI_INVNO, cs2);									//	登録番号データセット
				DiagSetInputMax(ID_COL_ADDI_INVNO, 13);										//	登録番号の入力文字数設定
				DiagSetBackColor(ID_COL_ADDI_NAME1_T);										//	背景色を変更
				DiagSetDataDisp(ID_COL_ADDI_NAME1_T,  "名称　　　　：");					//	名称ラベル表示
				DiagSetDataEdit(ID_COL_ADDI_NAME1, m_clsFunc.StrDocking(szName1, szName2));	//	名称データセット
				DiagSetInputMax(ID_COL_ADDI_NAME1, 20);										//	名称の入力文字数設定
				DiagSetBackColor(ID_COL_ADDI_ADD_T);										//	背景色を変更
				DiagSetDataDisp(ID_COL_ADDI_ADD_T,    "所在地　　　：");					//	所在地ラベル表示
				DiagSetDataEdit(ID_COL_ADDI_ADD1, szAdd1);									//	所在地１データセット
				DiagSetInputMax(ID_COL_ADDI_ADD1, 20);										//	所在地１の入力文字数設定
				DiagSetDataEdit(ID_COL_ADDI_ADD2, szAdd2);									//	所在地２データセット
				DiagSetInputMax(ID_COL_ADDI_ADD2, 20);										//	所在地２の入力文字数設定
				DiagSetBackColor(ID_COL_ADDI_KANA1_T);										//	背景色を変更
				DiagSetDataDisp(ID_COL_ADDI_KANA1_T,  "ｶﾅ名　　　　：");					//	ｶﾅ名ラベル表示
				DiagSetDataEdit(ID_COL_ADDI_KANA1, szKana1);								//	ｶﾅ名データセット
				DiagSetInputMax(ID_COL_ADDI_KANA1, 6);										//	ｶﾅ名の入力文字数設定
				// ※名称→登録番号の順にフォーカスを順にセットしています。
				// そうしないと何故か登録番号でひらがな入力が可能になってしますため。
				m_diagInput.SetPosition(ID_COL_ADDI_ADD1);									//	初期フォーカス位置に移動	// 修正No.168541,168543 add
				m_diagInput.SetPosition(ID_COL_ADDI_INVNOTSEL);								//	初期フォーカス位置に移動
				m_btnSearch.ShowWindow(SW_SHOW);											//	検索ボタン有効
				m_btnSearch.SetWindowText("法人検索[HOME]");								//	検索ボタンに名称設定
			}
			break;

		//	科目
		case ID_DLGTYPE_KAMOKU:
			//	セル4～9を非表示に設定
			for(nCnt = 4; nCnt <= 12; nCnt++) {
				DiagSetEnabled(nCnt);
			}
			//　セル13～16を非表示に設定
			for(nCnt = 13; nCnt <= 16; nCnt++) {
				DiagSetEnabled(nCnt);
			}
			//　セル19,20を非表示に設定
			for(nCnt = 19; nCnt <= 20; nCnt++) {
				DiagSetEnabled(nCnt);
			}
			//	セル21～32を非表示に設定
			for(nCnt = 21; nCnt <= 32; nCnt++) {
				DiagSetEnabled(nCnt);
			}

			DiagSetBackColor(ID_COL_KAMOKU_NAME1_T);						//	背景色を変更
// 改良No.23-0513 cor -->
			//DiagSetDataDisp(ID_COL_KAMOKU_NAME1_T, "科目　　　　：");		//	名称ラベル表示
// ----------------------
			sTitle.Empty();
			if(m_nGrSeq == ID_FORMNO_011 || m_nGrSeq == ID_FORMNO_102)	{
				sTitle = _T("種類　　　　：");
			}
			else if(m_nGrSeq == ID_FORMNO_061 || m_nGrSeq == ID_FORMNO_121 || m_nGrSeq == ID_FORMNO_151)	{
				sTitle = _T("区分　　　　：");
			}
			else	{
				sTitle = _T("科目　　　　：");
			}
			DiagSetDataDisp(ID_COL_KAMOKU_NAME1_T, sTitle);					//	名称ラベル表示
// 改良No.23-0513 cor <--
			DiagSetDataEdit(ID_COL_KAMOKU_NAME1, szName1);					//	名称データセット
			DiagSetInputMax(ID_COL_KAMOKU_NAME1, m_nInputLen);				//	名称の入力文字数設定
			DiagSetBackColor(ID_COL_KAMOKU_KANA1_T);						//	背景色を変更
			DiagSetDataDisp(ID_COL_KAMOKU_KANA1_T, "ｶﾅ名　　　　：");		//	ｶﾅ名ラベル表示
			DiagSetDataEdit(ID_COL_KAMOKU_KANA1, szKana1);					//	金融機関ｶﾅ名データセット
			DiagSetInputMax(ID_COL_KAMOKU_KANA1, 6);						//	ｶﾅ名の入力文字数設定
			m_diagInput.SetPosition(ID_COL_KAMOKU_NAME1);					//	初期フォーカス位置に移動
			m_btnSearch.ShowWindow(SW_HIDE);								//	検索ボタン無効
			break;
	}
}

//**************************************************
//	入力データチェック
//	【引数】	nType		…	参照型
//				szName1		…	名称1
//				szName2		…	名称2
//				szAdd1		…	住所1
//				szAdd2		…	住所2
//				szKana1		…	カナ1
//				szKana2		…	カナ2
//				nGrSeq		…	グループ番号
//				nSeq		…	シーケンス番号
//	【戻値】	0			…	編集なし
//				1			…	編集あり（重複なし）
//				2			…	編集あり（重複あり）
//				3			…	入力なし
//**************************************************
int CdlgReferenceSub::CheckInputData(EnumIdDlgType nType,
									 CString szName1, CString szName2,
									 CString szAdd1, CString szAdd2,
									 CString szKana1, CString szKana2, CString szInvno, int nGrSeq, int nSeq)
{
	int			nRet = 0;	//	戻値
	int			nCnt;		//	カウント用
	CString		szInput[6];	//	入力文字列
	size_t		len = 0;
	CString		msg = _T("");
	CString		cst = _T("");

	// 初期化
	for(nCnt=0; nCnt<5; nCnt++) {
		szInput[nCnt].Empty();
	}

	CStringArray	strArray;	// 文字列分割した場合のバッファ

CString cc;
	// 参照型で分岐
	switch(m_nType) {
		//	金融機関
		case ID_DLGTYPE_BANK:
			szInput[0] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_BANK_NAME1));	//	データ取得
			szInput[1] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_BANK_NAME2));	//	データ取得
			szInput[2] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_BANK_KANA1));	//	データ取得
			szInput[3] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_BANK_KANA2));	//	データ取得

			// 入力なし？
			if((szInput[0] == "") && (szInput[1] == "")) {
				nRet = 3;
				break;
			}
			// 入力内容と登録内容が変わった？
			if((szInput[0].Compare(szName1) != 0) ||
			   (szInput[1].Compare(szName2) != 0) ||
			   (szInput[2].Compare(szKana1) != 0) ||
			   (szInput[3].Compare(szKana2) != 0)) {
				// 重複してる？
				if(CheckRepeatBank(szInput[0], szInput[1], szInput[2], szInput[3]) == TRUE) {
					nRet = 2;
					break;
				}
			}

			if(bG_Kanso == TRUE) {
				// 電子帳票の文字数チェックを行う
				// 銀行名
				cst.Empty();
				cst = szInput[0].GetBuffer();
				((CfrmUc000Common *)m_pOwner)->ReplaceHankakuKana(cst);
				len = _tcsclen(cst.GetBuffer());
				if(len > UC_BANK_LEN) {
					msg.Empty();
					msg.Format(_T("金融機関名が電子申告の文字数(%d文字)を超えています。\r\nこのまま登録してもよろしいですか？"), UC_BANK_LEN);
					if(ICSMessageBox(msg, MB_YESNO | MB_ICONQUESTION) == IDNO) {
						m_diagInput.SetPosition(ID_COL_BANK_NAME1);	//	金融機関テキストに移動
						nRet = 4;
						break;
					}
				}
				else {
					// 支店名
					cst.Empty();
					cst = szInput[1].GetBuffer();
					((CfrmUc000Common *)m_pOwner)->ReplaceHankakuKana(cst);
					len = _tcsclen(cst.GetBuffer());
					if(len > UC_BANK_LEN) {
						msg.Empty();
						msg.Format(_T("支店名が電子申告の文字数(%d文字)を超えています。\r\nこのまま登録してもよろしいですか？"), UC_BANK_LEN);
						if(ICSMessageBox(msg, MB_YESNO | MB_ICONQUESTION) == IDNO) {
							m_diagInput.SetPosition(ID_COL_BANK_NAME2);	//	支店名テキストに移動
							nRet = 4;
							break;
						}
					}
				}
			}
			// 登録可能
			m_RetName1 = szInput[0];	//	返却名称1設定
			m_RetName2 = szInput[1];	//	返却名称2設定
			m_RetKana1 = szInput[2];	//　返却ｶﾅ1設定
			m_RetKana2 = szInput[3];	//　返却ｶﾅ2設定
			nRet = 1;					//	戻値を編集ありに設定
			break;

		//	取引先
		case ID_DLGTYPE_ADDRESS:
			if(bG_InvNo == FALSE) {
				m_clsFunc.StrDivision(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADD_NAME1), &strArray, 2, TRUE, TRUE);
				szInput[0] = strArray.GetAt(0);
				szInput[1] = strArray.GetAt(1);
				szInput[2] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADD_ADD1));	//	データ取得
				szInput[3] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADD_ADD2));	//	データ取得
				szInput[4] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADD_KANA1));	//	データ取得

				//	入力なし？
				if((szInput[0] == "") && (szInput[1] == "") && (szInput[2] == "") && (szInput[3] == "")) {
					nRet = 3;
				}
				//	入力内容と登録内容が変わった？
				else if((szInput[0].Compare(szName1) != 0) ||
						(szInput[1].Compare(szName2) != 0) ||
						(szInput[2].Compare(szAdd1) != 0) ||
						(szInput[3].Compare(szAdd2) != 0) ||
						(szInput[4].Compare(szKana1) != 0)) {

					// 重複してる？
					if(CheckRepeatAddress(szInput[0], szInput[1], szInput[2], szInput[3], szInput[4], "", nGrSeq) == TRUE) {
						nRet = 2;
						break;
					}

					m_RetName1 = szInput[0];	//	返却名称1設定
					m_RetName2 = szInput[1];	//	返却名称2設定
					m_RetAdd1 = szInput[2];		//	返却住所1設定
					m_RetAdd2 = szInput[3];		//	返却住所2設定
					m_RetKana1 = szInput[4];	//	返却ｶﾅ1設定
					nRet = 1;					//	戻値を編集ありに設定
				}
			}
			else {
				m_clsFunc.StrDivision(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_NAME1), &strArray, 2, TRUE, TRUE);
				szInput[0] = strArray.GetAt(0);
				szInput[1] = strArray.GetAt(1);
				szInput[2] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_ADD1));		//	データ取得
				szInput[3] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_ADD2));		//	データ取得
				szInput[4] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_KANA1));		//	データ取得
				cst.Empty();
				cst = m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_INVNO);											//	データ取得
				if(cst.IsEmpty() == FALSE)	{
					if(m_clsFunc.DiagGetCombo(&m_diagInput, ID_COL_ADDI_INVNOTSEL) == 0)	{
						szInput[5].Format(_T("T%s"), cst);
					}
					else	{
						szInput[5].Format(_T(" %s"), cst);
					}
				}

				//	入力なし？
				if((szInput[0] == "") && (szInput[1] == "") && (szInput[2] == "") && (szInput[3] == "") && (szInput[5] == "")) {
					nRet = 3;
				}
				//	入力内容と登録内容が変わった？
				else if((szInput[0].Compare(szName1) != 0) || (szInput[1].Compare(szName2) != 0) ||
						(szInput[2].Compare(szAdd1) != 0) || (szInput[3].Compare(szAdd2) != 0) ||
						(szInput[4].Compare(szKana1) != 0) || (szInput[5].Compare(szInvno) != 0)) {

					// 重複してる？
					if(CheckRepeatAddress(szInput[0], szInput[1], szInput[2], szInput[3], szInput[4], szInput[5], nGrSeq) == TRUE) {
						nRet = 2;
						break;
					}

					m_RetName1 = szInput[0];	//	返却名称1設定
					m_RetName2 = szInput[1];	//	返却名称2設定
					m_RetAdd1 = szInput[2];		//	返却住所1設定
					m_RetAdd2 = szInput[3];		//	返却住所2設定
					m_RetKana1 = szInput[4];	//	返却ｶﾅ1設定
					m_RetInvno = szInput[5];	//	返却登録番号設定
					nRet = 1;					//	戻値を編集ありに設定
				}
			}

			break;

		//	科目
		case ID_DLGTYPE_KAMOKU:
			szInput[0] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_KAMOKU_NAME1));	//	データ取得
			szInput[1] = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_KAMOKU_KANA1));	//	データ取得

			//	入力なし？
			if(szInput[0] == "") {
				nRet = 3;
				break;
			}
			//	入力内容と登録内容が変わった？
			if( (szInput[0].Compare(szName1) != 0) ||
				(szInput[1].Compare(szKana1) != 0)) {
				//	重複してる？
				if(CheckRepeatKamoku(szInput[0], szInput[1], nGrSeq) == TRUE) {
					nRet = 2;
					break;
				}
			}
			if(bG_Kanso == TRUE) {
// 改良No.21-0086,21-0529 cor -->
				//if(!(nGrSeq == ID_FORMNO_021 || nGrSeq == ID_FORMNO_111 || (nGrSeq >= ID_FORMNO_171 && nGrSeq <= ID_FORMNO_1720) || (nGrSeq >= ID_FORMNO_181 && nGrSeq <= ID_FORMNO_1820))) {
// ------------------------------
				if(!(nGrSeq == ID_FORMNO_021 || nGrSeq == ID_FORMNO_081 || nGrSeq == ID_FORMNO_111 || (nGrSeq >= ID_FORMNO_171 && nGrSeq <= ID_FORMNO_1720) || (nGrSeq >= ID_FORMNO_181 && nGrSeq <= ID_FORMNO_1820))) {
// 改良No.21-0086,21-0529 cor <--
					cst.Empty();
					cst = szInput[0].GetBuffer();
					((CfrmUc000Common *)m_pOwner)->ReplaceHankakuKana(cst);
					len = _tcsclen(cst.GetBuffer());
					if(len > UC_KAMOKU_LEN) {
						msg.Empty();
						msg.Format(_T("科目が電子申告の文字数(%d文字)を超えています。\r\nこのまま登録してもよろしいですか？"), UC_KAMOKU_LEN);
						if(ICSMessageBox(msg, MB_YESNO | MB_ICONQUESTION) == IDNO) {
							m_diagInput.SetPosition(ID_COL_KAMOKU_NAME1);	//	科目名テキストに移動
							nRet = 4;
							break;
						}
					}
				}
			}
			// 登録可能
			m_RetName1 = szInput[0];	//	返却名称1設定
			m_RetKana1 = szInput[1];	//　返却ｶﾅ名称1設定
			nRet = 1;					//	戻値を編集ありに設定
			break;
	}

	//	戻値を返す
	return(nRet);
}

//**************************************************
//	銀行／住所検索ダイアログ表示
//	【引数】	nType	…	参照型
//				nButn	…	0:住所検索/法人番号検索、1;登録番号一覧（※現時点未使用）
//	【戻値】	なし
//**************************************************
void CdlgReferenceSub::ShowSearchDialog(EnumIdDlgType nType, int nBtn)
{
	ICSBank				clsBank;		//	銀行クラス
	UCHAR				cBkCode[2];		//	銀行コード
	UCHAR				cBcCode[2];		//	支店コード
	CString				szBuf1;			//	バッファ１
	CString				szBuf2;			//	バッファ２
	CdlgAddressSearch	clsAdd(this);	//	住所検索クラス
	int					nID = 0;		//	ID
	int					sign = 0;		//	インボイス登録番号版で取引先の時　0:住所検索、1:法人番号一覧、2:登録番号一覧 を表示

	HJN_REC_NEW			hjnRec;
	CString				strErr;
	CStringArray		strArray;		// 文字列分割した場合のバッファ
	CString				cst,cs1,cs2, cs3;;
	char				buf[256];
	DIAGRAM_DATA		diadata;		// ICSDIAG構造体（T選択）
	CUcFunctionCommon	ufc;

	//	初期化
	szBuf1.Empty();
	szBuf2.Empty();

	// 参照型で分岐
	switch(nType) {
		case ID_DLGTYPE_BANK:			// 金融機関
			// 初期化
			memset(cBkCode, 0xFF, sizeof(cBkCode));
			memset(cBcCode, 0xFF, sizeof(cBcCode));

			// 銀行検索ダイアログを呼び出しOKボタンで終了？
			if(clsBank.BankSelect(cBkCode, cBcCode, &szBuf1, &szBuf2, 0, this) == TRUE) {
				// 銀行検索から名称を持ってくると、頭に半角スペースが入っている時があるので、頭のスペースをカット
				// （リリース時のみ現象が確認できた。デバックではなぜか現象出ない。）
				szBuf1.TrimLeft();
				szBuf2.TrimLeft();

				//	金融機関名セルにデータセット
				DiagSetDataEdit(ID_COL_BANK_NAME1, szBuf1);
				//	支店名セルにデータセット
				DiagSetDataEdit(ID_COL_BANK_NAME2, szBuf2);

				// 銀行カナ、支店カナも取得
				BankKanaSelect(cBkCode, cBcCode, &szBuf1, &szBuf2);
				//	金融機関カナ名セルにデータセット
				DiagSetDataEdit(ID_COL_BANK_KANA1, szBuf1);
				//	支店カナ名セルにデータセット
				DiagSetDataEdit(ID_COL_BANK_KANA2, szBuf2);
			}
			//	OKボタンにフォーカス移動
			m_btnOK.SetFocus();
			break;

		case ID_DLGTYPE_ADDRESS:		//	取引先
			sign = 0;
			nID = GetFocusControlID(this);			//	現在フォーカスのあるコントロールID取得
			if(bG_InvNo == TRUE && nID != ID_OK_BUTTON && nID != IDCANCEL)	{
				if(m_diagInput.GetPosition() == ID_COL_ADDI_INVNOTSEL || m_diagInput.GetPosition() == ID_COL_ADDI_INVNO)	{
					//if(nBtn == 0)	sign = 1;	// 法人番号検索
					//else			sign = 2;	// 登録番号一覧
					sign = 2;	// 登録番号一覧
				}
			}

			if(sign == 0)	{		// 住所検索
				if(clsAdd.ShowDialog(40, 40) == ID_DLG_OK) {	// 住所検索ダイアログでOKが押された？
// 修正No.168403 add -->
					if(bG_InvNo == TRUE)	{
						// 所在地セルにデータセット
						DiagSetDataEdit(ID_COL_ADDI_ADD1, clsAdd.m_AdAdd1);
						DiagSetDataEdit(ID_COL_ADDI_ADD2, clsAdd.m_AdAdd2);
						// 所在地セルにフォーカス移動
						if(clsAdd.m_AdAdd2 == "") {
							// セットするセルは住所欄(上段)
							m_diagInput.SetPosition(ID_COL_ADDI_ADD1);
						}
						else {
							// セットするセルは住所欄(下段)
							m_diagInput.SetPosition(ID_COL_ADDI_ADD2);
						}
					}
					else	{
// 修正No.168403 add <--
						// 所在地セルにデータセット
						DiagSetDataEdit(ID_COL_ADD_ADD1, clsAdd.m_AdAdd1);
						DiagSetDataEdit(ID_COL_ADD_ADD2, clsAdd.m_AdAdd2);
						// 所在地セルにフォーカス移動
						if(clsAdd.m_AdAdd2 == "") {
							// セットするセルは住所欄(上段)
							m_diagInput.SetPosition(ID_COL_ADD_ADD1);
						}
						else {
							// セットするセルは住所欄(下段)
							m_diagInput.SetPosition(ID_COL_ADD_ADD2);
						}
// 修正No.168403 add -->
					}
// 修正No.168403 add <--
				}
			}
			else if(sign == 1)	{	// 法人番号検索
				// ※現時点では法人検索からの呼び出しはなくなったが、今後必要となったときのためにロジックを残す
				memset(&hjnRec, 0, sizeof(HJN_REC_NEW));

				// もし名称等が入力されていたらhjnRecにセット
				// 名称
				m_clsFunc.StrDivision(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_NAME1), &strArray, 2, TRUE, TRUE);
				if(strArray.GetAt(0).IsEmpty() == FALSE || strArray.GetAt(1).IsEmpty() == FALSE) {
					memset(buf, 0, sizeof(buf));
					sprintf_s(buf, _T("%s%s"), strArray.GetAt(0), strArray.GetAt(1));
					memcpy(hjnRec.wHjnName, buf, strlen(buf));
				}
				// 住所（上段）
				cst = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_ADD1));		//	データ取得
				if(cst.IsEmpty() == FALSE) {
					memset(buf, 0, sizeof(buf));
					sprintf_s(buf, _T("%s"), cst);
					memcpy(hjnRec.wAdrs1, buf, strlen(buf));
				}
				// 住所（下段）
				cst = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_ADD2));		//	データ取得
				if(cst.IsEmpty() == FALSE) {
					memset(buf, 0, sizeof(buf));
					sprintf_s(buf, _T("%s"), cst);
					memcpy(hjnRec.wAdrs2, buf, strlen(buf));
				}
				// カナ
				cst = m_clsFunc.DeleteRightSpace(m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_KANA1));		//	データ取得
				if(cst.IsEmpty() == FALSE) {
					memset(buf, 0, sizeof(buf));
					sprintf_s(buf, _T("%s"), cst);
					memcpy(hjnRec.wHjnKana, buf, strlen(buf));
				}
				// 登録番号
				cst = m_clsFunc.DiagGetString(&m_diagInput, ID_COL_ADDI_INVNO);									//	データ取得
				if(cst.IsEmpty() == FALSE) {
					memset(buf, 0, sizeof(buf));
					sprintf_s(buf, _T("%s"), cst);
					memcpy(hjnRec.wHjnKana, buf, 13);
				}
				// 取込み対象とする項目
				hjnRec.wDDispInfo |= BIT_D0;	// D0 法人番号
				hjnRec.wDDispInfo |= BIT_D2;	// D2 法人名称
				hjnRec.wDDispInfo |= BIT_D3;	// D3 法人名称カナ
				hjnRec.wDDispInfo |= BIT_D4;	// D4 住所上下段ともに

				// 法人情報取得、ダイアログ呼び出し
				if(_GetHjnInfo(0x00, hjnRec, strErr, this) == 0)	{
					// データをセット
					// 登録番号(T選択)
					m_clsFunc.DiagInit(&diadata);
					cst.Format(_T("T%s"), hjnRec.wHjnNo1);			// ※ここではあえてTを選択させるようにする
					DiagSetCombo(ID_COL_ADDI_INVNOTSEL, cst);
					// 登録番号
					((CfrmUc000Common *)m_pOwner)->InvoNoSplit(cst, &cs1, &cs2);
					DiagSetDataEdit(ID_COL_ADDI_INVNO, cs2);

					m_diagInput.Refresh();
				}

				m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
			}
			else	{				// 登録番号一覧
				//int			opsw[8];					// 修正No.168444 del
				int			index = ID_COL_ADDI_INVNO;
				CRect		diagrect,zmselrect;
				RECT		hr;								// 修正No.168525 add
				CICSDiag*	diag = &m_diagInput;
				LONG		sWidth;

				if(pRfZmSel) {
					if(pRfZmSel->IsDisplay() != FALSE)	{
						pRfZmSel->DialogOFF();
					}

// 修正No.168444 del -->
					//for(int ii=0; ii<8; ii++)	{
					//	opsw[ii] = 0;
					//}

					//CDBZmSub* pRfZmSub = NULL;
					//pRfZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pRfZmSub;

					//// 必要なテーブルオープン
					//if(pRfZmSub->zvol == NULL) {
					//	pRfZmSub->VolumeOpen();			opsw[0] = 1;
					//}
					//if(pRfZmSub->pKnrec == NULL) {
					//	pRfZmSub->KamokuMeisyoRead();		opsw[1] = 1;
					//}
					//if(pRfZmSub->knrec == NULL) {
					//	pRfZmSub->KamokuMeisyoOpen();		opsw[2] = 1;
					//}
					//if(pRfZmSub->ezrec == NULL) {
					//	pRfZmSub->EdabanOpen();			opsw[3] = 1;
					//}
					//if(pRfZmSub->bmname == NULL) {
					//	pRfZmSub->BmnameOpen();			opsw[4] = 1;
					//}
					//if(pRfZmSub->rtrec == NULL) {
					//	pRfZmSub->RensoTekiyoOpen();		opsw[5] = 1;
					//}
					//if(pRfZmSub->tkrec == NULL) {
					//	pRfZmSub->KanaTekiyoOpen();		opsw[6] = 1;
					//}
					//if(pRfZmSub->strec == NULL) {
					//	pRfZmSub->SiwakeTaioTekiyoOpen();	opsw[7] = 1;
					//}
// 修正No.168444 del <--

					// 非連動dbは科目設定を開けないため、摘要に登録番号を入力することはないので登録番号選択は表示しない。
					// また、会社登録で「インボイス登録番号の表示」を「表示しない」に設定している場合も出さない。
					if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 0 && !pRfZmSub->zvol->s_sgn8 ){
						// 位置調整
						diag->GetRect(index,(LPUNKNOWN)&diagrect);
						ScreenToClient(&diagrect);
						pRfZmSel->GetScreenRect(zmselrect);

// 修正No.168525 del -->
						//// 上下
						//if((diagrect.top - zmselrect.Height() - 10) > 0)	{
						//	zmselrect.top = diagrect.top - 10;
						//}
						//else	{
						//	zmselrect.top = diagrect.bottom + zmselrect.Height() + 10;
						//}
						//// 左右
						//sWidth = zmselrect.Width();
						//if((sWidth / 2) > diagrect.right)	{
						//	// マルチ右など
						//	zmselrect.left = diagrect.left - (sWidth / 2);
						//}
						//else	{
						//	// 基本
						//	zmselrect.left = 10;
						//}
// 修正No.168525 del -->
// 修正No.168525 add -->
						// 幅と高さ
						int		wid = zmselrect.Width();
						int		hei = zmselrect.Height();

						// 編集ダイアログのサイズを取得
						m_diagInput.GetWindowRect(&hr);
						ScreenToClient(&hr);

						// 上下の位置
						zmselrect.bottom = diagrect.top - 10;
						zmselrect.top = zmselrect.bottom - hei;
						// 左右の位置
						//zmselrect.left = diagrect.left - (wid / 2);
						if((hr.right - hr.left) > wid) {
							zmselrect.left = hr.left + (((hr.right - hr.left) - wid) / 2);
						}
						else {
							zmselrect.left = hr.left;
						}

						zmselrect.right = zmselrect.left + wid;
// 修正No.168525 add -->

						pRfZmSel->DispCloseButton(1);

						try	{
							//pRfZmSel->DialogON(ZSEL_INVNO, ZSEL_TOPLEFT, &zmselrect);	// 修正No.168525 del
							pRfZmSel->DialogON(ZSEL_INVNO, ZSEL_CLIRECT, &zmselrect);	// 修正No.168525 add
						}
						catch(CDBException *e)	{
							ICSMessageBox(e->m_strError,MB_ICONSTOP);
							e->Delete();
						}
					}

// 修正No.168444 del -->
					//// テーブルクローズ
					//if(opsw[0] == 1)	pRfZmSub->VolumeClose();				opsw[0] = 0;
					//if(opsw[1] == 1)	pRfZmSub->KamokuMeisyoFree();			opsw[1] = 0;
					//if(opsw[2] == 1)	pRfZmSub->KamokuMeisyoClose();		opsw[2] = 0;
					//if(opsw[3] == 1)	pRfZmSub->EdabanClose();				opsw[3] = 0;
					//if(opsw[4] == 1)	pRfZmSub->BmnameClose();				opsw[4] = 0;
					//if(opsw[5] == 1)	pRfZmSub->RensoTekiyoClose();			opsw[5] = 0;
					//if(opsw[6] == 1)	pRfZmSub->KanaTekiyoClose();			opsw[6] = 0;
					//if(opsw[7] == 1)	pRfZmSub->SiwakeTaioTekiyoClose();	opsw[7] = 0;
// 修正No.168444 del <--
				}
				else	{
					if(pRfZmSel && pRfZmSel->IsDisplay() != FALSE)	{
						pRfZmSel->DialogOFF();
					}
				}

				m_diagInput.SetPosition(ID_COL_ADDI_INVNO);	// 修正No.168424 add
			}
			break;
	}
}

//**************************************************************
// DIAGRAM_ATTRIBUTE構造体初期化関数
//【引数】	*diadata	…	初期化構造体のハンドル
//【戻値】	なし
//***************************************************************
void CdlgReferenceSub::InitAttr( DIAGRAM_ATTRIBUTE *diaatt ){

	// 初期化
	memset(&diaatt->attr_bcolor, '\0', sizeof(diaatt->attr_bcolor));
	memset(&diaatt->attr_fcolor, '\0', sizeof(diaatt->attr_fcolor));
	memset(&diaatt->attr_frcolor, '\0', sizeof(diaatt->attr_frcolor));
	diaatt->attr_ofstx		= 0;
	diaatt->attr_ofsty		= 0;
	diaatt->attr_align		= 0;
	diaatt->attr_linestyle	= 0;
	diaatt->attr_linewidth	= 0;
	memset(&diaatt->attr_lfcolor, '\0', sizeof(diaatt->attr_lfcolor));
	memset(&diaatt->attr_lbcolor, '\0', sizeof(diaatt->attr_lbcolor));
	diaatt->attr_rframe		= 0;
	diaatt->attr_rwidth		= 0;
	diaatt->attr_rheight	= 0;
	diaatt->attr_inpkind	= 0;
	diaatt->attr_inpmax		= 0;
	diaatt->attr_linemax	= 0;
	diaatt->attr_frctmax	= 0;
	diaatt->attr_editkind	= 0;
	diaatt->attr_editattr	= 0;
	diaatt->attr_editformat.Empty();
	diaatt->attr_dayattr	= 0;

}

//**************************************************
//	改行文字列削除
//	【引数】	szBuf	…	文字列
//	【戻値】	改行文字列削除文字列
//**************************************************
CString CdlgReferenceSub::DelCrLfString(CString szBuf)
{
	CString		szRet;		//	戻値
	CString		szCrLf;		//	改行文字列
	int			nPos = -1;	//	改行位置
	int			nLen = -1;	//	文字列長
	int			nCrLf = -1;	//	改行の文字列長

	//	初期化
	szRet.Empty();
	szCrLf.Empty();
	//	改行文字列作成
	szCrLf = '\r';
	szCrLf += '\n';

	//	改行文字列検索
	nPos = szBuf.Find(szCrLf);

	//	改行あり？
	if(nPos >= 0) {
		nLen = szBuf.GetLength();						//	文字列長取得
		nCrLf = szCrLf.GetLength();						//	改行の文字列長取得
		szRet = szBuf.Left(nPos);						//	改行文字までの文字列取得
		szRet += szBuf.Right(nLen - (nPos + nCrLf));	//	改行文字列以降の文字列取得
	}
	//	改行なし
	else {
		//	引数を戻値にセット
		szRet = szBuf;
	}

	//	戻値を返す
	return(szRet);
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CdlgReferenceSub::GetFocusControlID(CWnd* pParent /*=NULL*/)
{
	int		nRet = 0;			//	戻値
	CWnd*	pControl = NULL;	//	コントロール

	//	親ハンドルあり？
	if(pParent != NULL) {
		//	フォーカスのあるコントロールを取得
		pControl = pParent->GetFocus();

		//	コントロール取得成功？
		if(pControl != NULL) {
			//	コントロールID取得
			nRet = pControl->GetDlgCtrlID();
		}
	}

	//	戻値を返す
	return(nRet);
}

//**************************************************
//	重複確認（金融機関）
//	【引数】	szName1		…	銀行名
//				szName2		…	支店名
//				szKana2		…	銀行名ｶﾅ
//				szKana2		…	支店名ｶﾅ
//	【戻値】	TRUE		…	重複あり
//				FALSE		…	重複なし
//**************************************************
BOOL CdlgReferenceSub::CheckRepeatBank(CString szName1, CString szName2, CString szKana1, CString szKana2)
{
	BOOL				fRet = FALSE;		//	戻値
	CdbUcLstBank		mfcRec(m_pDB);	//	金融機関名称テーブル

	//	重複確認
	if(mfcRec.CheckRepeat(szName1, szName2, szKana1, szKana2) == DB_ERR_OK) {
		//	重複あり？
		if(!mfcRec.IsEOF()) {
			//	重複あり！
			fRet = TRUE;
		}
		//	閉じる
		mfcRec.Fin();
	}

	//	戻値を返す
	return(fRet);
}

//**************************************************
//	重複確認（取引先）
//	【引数】	szName1		…	名称１
//				szName2		…	名称２
//				szAdd1		…	住所１
//				szAdd2		…	住所２
//				szKana		…	ｶﾅ１
//				nGrSeq		…	グループ番号
//	【戻値】	TRUE		…	重複あり
//				FALSE		…	重複なし
//**************************************************
BOOL CdlgReferenceSub::CheckRepeatAddress(CString szName1, CString szName2, CString szAdd1, CString szAdd2, CString szKana, CString szInvno, int nGrSeq)
{
	int					ret = 0;
	BOOL				fRet = FALSE;		//	戻値
	CString				cst = _T("");
	CdbUcLstAddress		mfcRec(m_pDB);	//	取引先名称テーブル

	//	重複確認
	//if(mfcRec.CheckRepeat(szName1, szName2, szAdd1, szAdd2, nGrSeq, TRUE, szKana) == DB_ERR_OK) {
	if(bG_InvNo == TRUE) {
		if(szInvno.IsEmpty() == FALSE)	cst = szInvno;
		else							cst = _T("-1");
		ret = mfcRec.CheckRepeat_Inv(szName1, szName2, szAdd1, szAdd2, cst, nGrSeq, TRUE, szKana);
	}
	else	{
		ret = mfcRec.CheckRepeat(szName1, szName2, szAdd1, szAdd2, nGrSeq, TRUE, szKana);
	}
	if(ret == DB_ERR_OK)	{
		//	重複あり？
		if(!mfcRec.IsEOF()) {
			//	重複あり！
			fRet = TRUE;

			//	閉じる
			mfcRec.Fin();

			//	戻値を返す
			return(fRet);
		}
	}

	//	閉じる
	mfcRec.Fin();

	//	戻値を返す
	return(fRet);
}

//**************************************************
//	重複確認（科目）
//	【引数】	szName1		…	科目名称
//				szName2		…　科目ｶﾅ
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	TRUE		…	重複あり
//				FALSE		…	重複なし
//**************************************************
BOOL CdlgReferenceSub::CheckRepeatKamoku(CString szName1, CString szName2, int nFormSeq)
{
	BOOL				fRet = FALSE;		//	戻値
	CdbUcLstKamoku		mfcRec(m_pDB);	//	科目名称テーブル

	//	重複確認
	if(mfcRec.CheckRepeat(nFormSeq, szName1, szName2) == DB_ERR_OK) {
		//	重複あり？
		if(!mfcRec.IsEOF()) {
			//	重複あり！
			fRet = TRUE;
		}
		//	閉じる
		mfcRec.Fin();
	}

	//	戻値を返す
	return(fRet);
}

//***************************************************************
//	ダイアログ表示
//	【引数】	nType			…	参照型
//				nMode			…	動作モード（追加/挿入/編集）
//				szTitle			…	ダイアログタイトル
//				nOrder			…	順序番号
//				szName1			…	名称1
//				szName2			…	名称2
//				szAdd1			…	住所1
//				szAdd2			…	住所2
//				szInvno			…	登録番号
//				nGrSeq			…	グループ番号(取引先)
//				nSeq			…	シーケンス場合
//				nInputLen		…	入力文字数（※科目参照のみ）
//	【戻値】	ID_DLG_OK		…	OKボタン押された
//				ID_DLG_CANCEL	…	キャンセルボタン押された
//***************************************************************
int CdlgReferenceSub::ShowDialog( CDatabase* pDB, EnumIdDlgType nType /*=ID_DLGTYPE_BANK*/, 
								  EnumIdDlgRefMode nMode /*=ID_DLGREF_APPEND*/, 
								  CString szTitle /*=""*/, long nOrder /*=1*/, 
								  CString szName1 /*=""*/, CString szName2 /*=""*/, 
								  CString szAdd1 /*=""*/, CString szAdd2 /*=""*/,
								  CString szKana1 /*=""*/, CString szKana2/*=""*/, CString szInvno/*=""*/,
								  int nInputLen /*=DR_INPUTLENGTH_NORMAL*/, int nGrSeq /*=0*/, int nSeq /*=0*/ )
{
	char	szBuf[8];			//	バッファ

	//	バッファ初期化
	ZeroMemory( szBuf, sizeof( szBuf ) );
	//	順序番号を文字列に変換
	_itoa_s( nOrder, szBuf, 10 );	

	m_nRet = ID_DLG_CANCEL;		//	戻値を初期化
	m_RetName1.Empty();			//	返却名称1初期化
	m_RetName2.Empty();			//	返却名称2初期化
	m_RetAdd1.Empty();			//	返却住所1初期化
	m_RetAdd2.Empty();			//	返却住所2初期化
	m_RetKana1.Empty();			//  返却ｶﾅ1初期化
	m_RetKana2.Empty();			//  返却ｶﾅ2初期化
	m_RetInvno.Empty();			//  返却登録番号初期化

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){
		//	データベース開いてる？
		if ( pDB->IsOpen() ){
			m_pDB = pDB;
			m_nType = nType;			//	参照型取得
			m_nMode = nMode;			//	動作モード取得
			m_szTitle = szTitle;		//	ダイアログタイトル取得
			m_szOrder = szBuf;			//	順序番号取得
			m_szName1 = szName1;		//	名称1取得
			m_szName2 = szName2;		//	名称2取得
			m_szAdd1 = szAdd1;			//	住所1取得
			m_szAdd2 = szAdd2;			//	住所2取得
			m_szKana1 = szKana1;		//  ｶﾅ文字1取得
			m_szKana2 = szKana2;		//  ｶﾅ文字1取得
			m_szInvno = szInvno;		//  登録番号取得
			m_nInputLen = nInputLen;	//	入力文字数（※科目参照ダイアログのみ）
			m_nGrSeq = nGrSeq;			//	グループ番号（科目はフォーム番号）
			m_nSeq = nSeq;				//	シーケンス番号
			DoModal();					//	モーダルで表示
		}
	}
	
	//	戻値を返す
	return( m_nRet );
}

void CdlgReferenceSub::ReadStringIcsdiag8Kanasearch(short index, LPCTSTR kana)
{
	int nBaseIndex = 0;
	CString strKana;

	switch(m_nType) {
		case ID_DLGTYPE_BANK:		// 金融機関
			if(index == ID_COL_BANK_NAME1)		nBaseIndex = ID_COL_BANK_KANA1;
			else if(index == ID_COL_BANK_NAME2)	nBaseIndex = ID_COL_BANK_KANA2;
			break;
		case ID_DLGTYPE_ADDRESS:	// 取引先
// 修正No.168403 del -->
			//nBaseIndex = ID_COL_ADD_KANA1;
// 修正No.168403 del <--
// 修正No.168403 add -->
			if(bG_InvNo == TRUE)	nBaseIndex = ID_COL_ADDI_KANA1;
			else					nBaseIndex = ID_COL_ADD_KANA1;
// 修正No.168403 add <--
			break;
		case ID_DLGTYPE_KAMOKU:		// 科目
			nBaseIndex = ID_COL_KAMOKU_KANA1;
			break;
	}

	strKana = m_clsFunc.DiagGetString(&m_diagInput, nBaseIndex);
	if(!strKana.IsEmpty())	return;

	CharController cc;

	cc.Divide(kana, strKana, 6/*文字*/ / 2);
	switch(m_nType) {
		case ID_DLGTYPE_BANK:		// 金融機関
			if(index == ID_COL_BANK_NAME1)		DiagSetDataEdit(ID_COL_BANK_KANA1, strKana);		//	金融機関ｶﾅ名データセット
			else if(index == ID_COL_BANK_NAME2)	DiagSetDataEdit(ID_COL_BANK_KANA2, strKana);		//	支店名ｶﾅ名データセット
			break;
		case ID_DLGTYPE_ADDRESS:	// 取引先
// 修正No.168403 del -->
			//DiagSetDataEdit(ID_COL_ADD_KANA1, strKana);			//	ｶﾅ名ラベル表示
// 修正No.168403 del <--
// 修正No.168403 add -->
			//	ｶﾅ名ラベル表示
			if(bG_InvNo == TRUE)	DiagSetDataEdit(ID_COL_ADDI_KANA1, strKana);
			else					DiagSetDataEdit(ID_COL_ADD_KANA1, strKana);
// 修正No.168403 add <--
			break;
		case ID_DLGTYPE_KAMOKU:		// 科目
			DiagSetDataEdit(ID_COL_KAMOKU_KANA1, strKana);		//	ｶﾅ名ラベル表示
			break;
	}
}

//**************************************************
// 銀行カナ、支店カナを取得
//**************************************************
void CdlgReferenceSub::BankKanaSelect(UCHAR* pBkCode, UCHAR* pBcCode, CString* pBuf1, CString* pBuf2)
{
	int			ival = 0;
	int			cnt = 0, sw = 0;
	int			bno = 0, brnum = 0, brofs = 0;
	char		sdev[16], mdev[16];
	char		bf[128];
	UCHAR		bbf[128];
	UCHAR		cbf[128];
	CString		kana1 = _T(""), kana2 = _T("");
	ICSBank		clsBank;					//	銀行クラス
	_BANK_REC	brec;
	_BRAN_REC	brrec;

	memset(sdev, 0, 16);
	memset(mdev, 0, 16);
	GCOM_GetString(_T("SystemDrive"), sdev);
	PCOM_GetString(_T("MasterDevice"), mdev);
	if(clsBank.InitBankFile(sdev[0], mdev[0], TRUE) == FALSE) {
		ICSExit(0, _T("銀行名称ファイルがオープンできません。"));
		return;
	}
	clsBank.AccessBankFile(FALSE);

	// 銀行コード
	memset(bbf, 0, 128);
	memcpy(bbf, pBkCode, 128);
	// 支店コード
	memset(cbf, 0, 128);
	memcpy(cbf, pBcCode, 128);
	// 返送用変数をクリア
	pBuf1->Empty();
	pBuf2->Empty();

	bno = clsBank.GetBank(bbf);
	if(bno >= 0) {
		// 銀行情報を取得
		brec = clsBank.m_BnkRec[bno];

		// 銀行カナ
		memset(bf, 0, 128);
		memcpy(bf, brec.BANK_KAN, 6);
		pBuf1->Format(_T("%s"), bf);

		// 支店情報を取得
		brnum = clsBank.GetBranchNum(bbf);
		brofs = clsBank.GetBranch(bbf);
		sw = 0;
		for(cnt = brofs; cnt < brnum + brofs; cnt++) {
			brrec = clsBank.m_BrnRec[cnt];
			if(((brrec.BRAN_CODE[0] & 0xff) == (cbf[0] & 0xff)) && ((brrec.BRAN_CODE[1] & 0xff) == (cbf[1] & 0xff))) {
				sw = 1;
				break;
			}
		}
		if(sw == 1) {
			memset(bf, 0, 128);
			memcpy(bf, brrec.BRAN_KAN, 6);
			pBuf2->Format(_T("%s"), bf);
		}
	}
}

void CdlgReferenceSub::EditOFFInputDiag(short index)
{
// 修正No.168365,168404 del -->
//	// 登録番号一覧が表示されていたら閉じる
//	if(pRfZmSel && index != ID_COL_ADDI_INVNOTSEL && index != ID_COL_ADDI_INVNO) {
//		if(pRfZmSel->IsDisplay() != FALSE) {
//			pRfZmSel->DialogOFF();
//		}
//	}
// 修正No.168365,168404 del <--

// 修正No.168425,168443 add -->
	// インボイス登録番号使用マスターで参照型が取引先の時
	if(bG_InvNo == TRUE && m_nType == ID_DLGTYPE_ADDRESS)	{
		if(index == ID_COL_ADDI_INVNO) {
			int				ret=0;
			CString			szInput,szInputc;
			CString			cst, cst1, cst2, cst3, sErr;
			CString			cc, str1, str2, str3;
			DIAGRAM_DATA	diadata;			// ICSDiag構造体
			ZmselDBData		tmpzd;				// 修正No.168361,168369,168371,168378 add

			// 登録番号にカーソルがある場合、登録番号のチェックディジットを行う
			if(index == ID_COL_ADDI_INVNO)	{
				szInput.Empty();
				szInputc.Empty();
				cst.Empty();
				// 登録番号のデータ取得
				cst = m_clsFunc.DiagGetString(&m_diagInput, index);
				if(cst.IsEmpty() == FALSE)	{
					if(m_clsFunc.DiagGetCombo(&m_diagInput, ID_COL_ADDI_INVNOTSEL) == 0)	{
						szInput.Format(_T("T%s"), cst);
						szInputc = szInput;					// チェックディジット用
					}
					else	{
						szInput.Format(_T(" %s"), cst);
						szInputc.Format(_T("T%s"), cst);	// チェックディジット用
					}
				}

// 修正No.168361,168369,168371,168378 del -->
				//if(szInput.IsEmpty() == FALSE)	{
				//	//ret = ((CfrmUc000Common *)m_pOwner)->GetBillingProviderHojinInfo(m_pDB, m_nGrSeq, szInput, &cst1, &cst2, &cst3, &sErr, 1);
				//	//if(ret >= 0)	{	//	正常終了（データ取得済み）
				//	//	// 「T」を表示
				//	//	if(ret == 0)	DiagSetDataDisp(ID_COL_ADDI_INVNO_T, "T");
				//	//	else			DiagSetDataDisp(ID_COL_ADDI_INVNO_T, " ");

				//	//	// 名称を表示
				//	//	str1 = str2 = str3 = _T("");
				//	//	cc.Format(_T("%s"), cst1);
				//	//	cc.TrimRight();
				//	//	if(cc.GetLength() > 20)	{
				//	//		str2 = m_clsFunc.GetSpritString(cc, &str1, 20);
				//	//		if(str2.GetLength() > 20)	{
				//	//			cc = str2;
				//	//			str3 = m_clsFunc.GetSpritString(cc, &str2, 20);
				//	//		}
				//	//	}
				//	//	else	{
				//	//		str1 = cc;
				//	//	}
				//	//	cst1.Empty();
				//	//	if(str2.IsEmpty() == FALSE)		cst1 = str1 + _T("\r\n") + str2;
				//	//	else							cst1 = str1;
				//	//	m_clsFunc.DiagInit(&diadata);
				//	//	diadata.data_edit.Format(_T("%s"), cst1);
				//	//	m_diagInput.SetData(ID_COL_ADDI_NAME1, (LPUNKNOWN)&diadata);
				//	//	// 所在地
				//	//	m_clsFunc.DiagInit(&diadata);
				//	//	diadata.data_edit.Format(_T("%s"), cst2);
				//	//	m_diagInput.SetData(ID_COL_ADDI_ADD1, (LPUNKNOWN)&diadata);
				//	//	m_clsFunc.DiagInit(&diadata);
				//	//	diadata.data_edit.Format(_T("%s"), cst3);
				//	//	m_diagInput.SetData(ID_COL_ADDI_ADD2, (LPUNKNOWN)&diadata);
				//	//}
				//	//else	{
				//	//	ICSMessageBox(sErr, MB_ICONEXCLAMATION | MB_OK, 0, 0, this);
				//	//	// 登録番号にカーソルを戻す
				//	//	m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
				//	//	break;
				//	//}

				//	if(szInputc.GetLength() < 14) {
				//		DIAGRAM_DATA	ddata;
				//		ddata.data_combo = 0;
				//		m_diagInput.SetData(ID_COL_ADDI_INVNOTSEL, (LPUNKNOWN)&ddata);
				//		ddata.data_edit.Empty();
				//		m_diagInput.DataClear(ID_COL_ADDI_INVNO, TRUE);

				//		// エラーメッセージを表示
				//		cst  = _T("登録番号（法人番号）の文字数に誤りがあります。\r\n");
				//		cst += _T("登録番号（法人番号）を再入力してください。");
				//		ICSMessageBox(cst, MB_ICONSTOP | MB_OK, 0, 0, this);
				//		m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
				//		break;
				//	}

				//	// 指定された登録番号が正しいかどうかチェック
				//	if(pSyzInvoice.CheckInvoiceNumber(szInputc) == -1) {
				//		////	登録番号（T選択）
				//		//DiagSetCombo(ID_COL_ADDI_INVNOTSEL, szInput);
				//		//// 登録番号
				//		//((CfrmUc000Common *)m_pOwner)->InvoNoSplit(szInput, &cst1, &cst2);
				//		//DiagSetDataEdit(ID_COL_ADDI_INVNO, cst2);
				//		DIAGRAM_DATA	ddata;
				//		ddata.data_combo = 0;
				//		m_diagInput.SetData(ID_COL_ADDI_INVNOTSEL, (LPUNKNOWN)&ddata);
				//		ddata.data_edit.Empty();
				//		m_diagInput.DataClear(ID_COL_ADDI_INVNO, TRUE);

				//		// エラーメッセージを表示
				//		//cst  = _T("登録番号（法人番号）に誤りがあります。\r\n誤った番号は保存されません。\r\n\r\n");	// 修正No.168378 del
				//		cst  = _T("登録番号（法人番号）に誤りがあります。\r\n");										// 修正No.168378 add
				//		cst += _T("登録番号（法人番号）を再入力してください。");
				//		ICSMessageBox(cst, MB_ICONSTOP | MB_OK, 0, 0, this);
				//		m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
				//		break;
				//	}
				//}
				//else	{
				//	//m_clsFunc.DiagInit(&diadata);
				//	//diadata.data_disp.Format(_T(" "));
				//	//m_diagInput.SetData(ID_COL_ADDI_INVNO_T, (LPUNKNOWN)&diadata);
				//	DiagSetDataDisp(ID_COL_ADDI_INVNO_T, " ");
				//}
// 修正No.168361,168369,168371,168378 del <--
// 修正No.168361,168369,168371,168378 add -->
				if(cst.GetLength() > 0)	{

// 修正No.168443 add -->
					CWnd *pWnd1 = AfxGetMainWnd();
					if(pWnd1 != NULL) {
						CWnd *pWnd2 = pWnd1->GetFocus();
						if((pWnd2 != NULL && pWnd2 != &m_diagInput) || (pWnd2 == NULL)) {
							if(pSyzInvoice.CheckInvoiceNumber(szInputc) == -1) {
								m_diagInput.DataClear(ID_COL_ADDI_INVNO, TRUE);
								return;
							}
						}
					}
// 修正No.168443 add <--

					if(pRfZmSel && pRfZmSel->IsDisplay() != FALSE && cst.GetLength() < 3) {
						// 2桁の場合は科目選択から
						int no = atoi(cst);
						pRfZmSel->InputData(1, no, -1, ZSEL_INVNO);
						pRfZmSel->ResultData(&tmpzd);
						if(tmpzd.errflg != -1) {
							DIAGRAM_DATA	ddata;
							// T選択
							m_clsFunc.DiagInit(&ddata);
							ddata.data_combo = 0;
							m_diagInput.SetData(ID_COL_ADDI_INVNOTSEL, (LPUNKNOWN)&ddata);
							// 登録番号
							m_clsFunc.DiagInit(&ddata);
							ddata.data_edit.Format(tmpzd.sel_name);
							m_diagInput.SetData(ID_COL_ADDI_INVNO, (LPUNKNOWN)&ddata);
							// 再描画
							m_diagInput.Refresh();

							// 名称にフォーカスを移動
							m_diagInput.SetPosition(ID_COL_ADDI_NAME1);

							return;
						}
					}
					else {
						// ３桁以上は手入力なので桁数チェック
						if(szInputc.GetLength() < 14) {
// 修正No.168443 del -->
							//// 手前で入力していた番号に戻す
							//if(m_InvnoSave.IsEmpty() == FALSE) {
							//	DiagSetDataEdit(ID_COL_ADDI_INVNO, m_InvnoSave);
							//	m_diagInput.Refresh();
							//}
							//else {
							//	m_diagInput.DataClear(ID_COL_ADDI_INVNO, TRUE);
							//}
// 修正No.168443 del <--

							// エラーメッセージを表示
							cst = _T("登録番号（法人番号）の文字数に誤りがあります。\r\n");
							cst += _T("登録番号（法人番号）を再入力してください。");
							ICSMessageBox(cst, MB_ICONSTOP | MB_OK, 0, 0, this);
							//　フォーカスを戻す
							m_diagInput.SetPosition(ID_COL_ADDI_INVNO);

							return;
						}
					}

					// 指定された登録番号が正しいかどうかチェック
					if(pSyzInvoice.CheckInvoiceNumber(szInputc) == -1) {
// 修正No.168443 del -->
						//// 手前で入力していた番号に戻す
						//if(m_InvnoSave.IsEmpty() == FALSE) {
						//	DiagSetDataEdit(ID_COL_ADDI_INVNO, m_InvnoSave);
						//	m_diagInput.Refresh();
						//}
						//else {
						//	m_diagInput.DataClear(ID_COL_ADDI_INVNO, TRUE);
						//}
// 修正No.168443 del <--

						// エラーメッセージを表示
						cst  = _T("登録番号（法人番号）に誤りがあります。\r\n");
						cst += _T("登録番号（法人番号）を再入力してください。");
						ICSMessageBox(cst, MB_ICONSTOP | MB_OK, 0, 0, this);
						// フォーカスを戻す
						m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
					}
				}
// 修正No.168361,168369,168371,168378 add <--
			}
		}
	}
// 修正No.168425,168443 add <--
}

// 修正No.168365,168404 add -->
void CdlgReferenceSub::EditONInputDiag(short index)
{
	//if(bG_InvNo == TRUE)	{
	if(bG_InvNo == TRUE && m_nType == ID_DLGTYPE_ADDRESS)	{
		// 登録番号一覧が表示されていたら閉じる
		if(pRfZmSel && index != ID_COL_ADDI_INVNOTSEL && index != ID_COL_ADDI_INVNO) {
			if(pRfZmSel->IsDisplay() != FALSE) {
				pRfZmSel->DialogOFF();
			}
		}
	}
}
// 修正No.168365,168404 add <--

// 修正No.168377 add -->
void CdlgReferenceSub::ComboSelInputDiag(short index, LPUNKNOWN data)
{
	if(bG_InvNo == TRUE)	{
		if(m_clsFunc.DiagGetCombo(&m_diagInput, ID_COL_ADDI_INVNOTSEL) != 0)	{
			// 登録番号一覧が表示されていたら閉じる
			if(pRfZmSel && pRfZmSel->IsDisplay() != FALSE) {
				pRfZmSel->DialogOFF();
			}
		}
	}
}
// 修正No.168377 add <--

void CdlgReferenceSub::InitZmSelSub(void)
{
// 修正No.168444 del -->
	//int		opsw[8];
// 修正No.168444 del <--

	for(int ii=0; ii<8; ii++)	{
		opsw[ii] = 0;
	}

	pRfZmSel = new CZmselDB();
	if(pRfZmSel != NULL) {
		//CDBZmSub* pRfZmSub = NULL;	// 修正No.168444 del
		pRfZmSub = NULL;				// 修正No.168444 add
		pRfZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
		pRfZmSel->SetDialogResize(FALSE);	//科目選択を小さく表示

		// 必要なテーブルオープン
		if(pRfZmSub->zvol == NULL) {
			pRfZmSub->VolumeOpen();				opsw[0] = 1;
		}
		if(pRfZmSub->pKnrec == NULL) {
			pRfZmSub->KamokuMeisyoRead();		opsw[1] = 1;
		}
		if(pRfZmSub->knrec == NULL) {
			pRfZmSub->KamokuMeisyoOpen();		opsw[2] = 1;
		}
		if(pRfZmSub->ezrec == NULL) {
			pRfZmSub->EdabanOpen();				opsw[3] = 1;
		}
		if(pRfZmSub->bmname == NULL) {
			pRfZmSub->BmnameOpen();				opsw[4] = 1;
		}
		if(pRfZmSub->rtrec == NULL) {
			pRfZmSub->RensoTekiyoOpen();		opsw[5] = 1;
		}
		if(pRfZmSub->tkrec == NULL) {
			pRfZmSub->KanaTekiyoOpen();			opsw[6] = 1;
		}
		if(pRfZmSub->strec == NULL) {
			pRfZmSub->SiwakeTaioTekiyoOpen();	opsw[7] = 1;
		}

		// クラスモジュールの初期設定
		pRfZmSel->SetPara(pRfZmSub, this, NULL, callback2);
		
// 修正No.168444 del -->
		//// テーブルクローズ
		//if(opsw[0] == 1)	pRfZmSub->VolumeClose();				opsw[0] = 0;
		//if(opsw[1] == 1)	pRfZmSub->KamokuMeisyoFree();			opsw[1] = 0;
		//if(opsw[2] == 1)	pRfZmSub->KamokuMeisyoClose();		opsw[2] = 0;
		//if(opsw[3] == 1)	pRfZmSub->EdabanClose();				opsw[3] = 0;
		//if(opsw[4] == 1)	pRfZmSub->BmnameClose();				opsw[4] = 0;
		//if(opsw[5] == 1)	pRfZmSub->RensoTekiyoClose();			opsw[5] = 0;
		//if(opsw[6] == 1)	pRfZmSub->KanaTekiyoClose();			opsw[6] = 0;
		//if(opsw[7] == 1)	pRfZmSub->SiwakeTaioTekiyoClose();	opsw[7] = 0;
// 修正No.168444 del <--
	}
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void CALLBACK callback2(UINT n, void* p, CWnd* pwnd)
{
	//CdlgReferenceSub* pBaseDlg = (CdlgReferenceSub*)pwnd;

	if(n == IDOK)	{
		ZmselDBData* zd = (ZmselDBData*)p;
		
		if(zd->selmode == ZSEL_INVNO)	{
			if(strlen(zd->sel_name) > 0)	{
				CString				strInvoNo, strHjnName;
				CdlgReferenceSub*	pBaseDlg = (CdlgReferenceSub*)pwnd;

				strInvoNo.Format(_T("%s"), zd->sel_name);	// 登録番号
				//strHjnName.Format(_T("%s"), zd->tkstr);	// 摘要名称
				strHjnName.Empty();							// ※摘要名称は取得しない
				// 取得したデータを各様式の画面に表示
				pBaseDlg->DiagSetHjnData(strInvoNo, strHjnName);
			}
		}
	}
}

void CdlgReferenceSub::DiagSetHjnData(CString strInvoNo, CString strHjnName)
{
	CString				cst,cs1,cs2, cs3;;
	DIAGRAM_DATA		diadata;		// ICSDIAG構造体（T選択）
	CUcFunctionCommon	ufc;

	// データをセット
	// 登録番号(T選択)
	m_clsFunc.DiagInit(&diadata);
	cst.Format(_T("T%s"), strInvoNo);
	DiagSetCombo(ID_COL_ADDI_INVNOTSEL, cst);
	// 登録番号
	((CfrmUc000Common *)m_pOwner)->InvoNoSplit(cst, &cs1, &cs2);
	DiagSetDataEdit(ID_COL_ADDI_INVNO, cs2);

	m_diagInput.Refresh();

	// 登録番号一覧が表示されていたら閉じる
	if(pRfZmSel && pRfZmSel->IsDisplay() != FALSE) {
		pRfZmSel->DialogOFF();
	}

	m_diagInput.SetPosition(ID_COL_ADDI_INVNO);
}
// インボイス登録番号追加対応_23/11/06 add <--
