// 改良No.22-0404,22-0812 add -->
//------------------------------------------------------------------------------------------------------------------------------------
//	dlgReference.cpp
//
//	2006.02.21～
//	2023.01.13～	リスト変更 ListInp→ICSReport
//------------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgReference.h"
#include "frmUc000Common.h"
#include "dlgReferenceDelChk1.h"
#include "dlgReferenceDelChk2.h"	// 修正No.162389 add

extern	BOOL	bG_InvNo;			// インボイス登録番号追加対応_23/11/06 add

// ユーザーメッセージWPARAM
#define			UM1_POPULATE	0
#define			UM1_REPINIZ		1

BOOL			g_TabSw;
BOOL			g_TabSw2;			// 修正No.162490 add

IMPLEMENT_DYNAMIC(CdlgReference, ICSDialog)

// -----------------------------------------------------------------------------------------------------------------------------------
//	標準コンストラクタ
// -----------------------------------------------------------------------------------------------------------------------------------
CdlgReference::CdlgReference(CWnd* pParent /*=NULL*/) : ICSDialog(CdlgReference::IDD, pParent)
{
	m_pOwner = pParent;
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	コンストラクタ
// -----------------------------------------------------------------------------------------------------------------------------------
CdlgReference::CdlgReference( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
	m_pOwner = pParent;
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	デストラクタ
// -----------------------------------------------------------------------------------------------------------------------------------
CdlgReference::~CdlgReference()
{
}

void CdlgReference::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENTRY_LABEL, m_lblEntry);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//DDX_Control(pDX, IDC_COMBO_TORIHIKICHANGE, m_cmbTorihikiChange);	// 修正No.162490 del
	DDX_Control(pDX, IDC_ICSDIAG8_KANASEARCH, m_DiagKanaSearch);
	DDX_Control(pDX, IDC_CHECK_REG_AUTO, m_chkRegAuto);
	//DDX_Control(pDX, IDC_REFERENCE_LIST, m_lstReference);
	DDX_Control(pDX, IDC_REFERENCE_REPORT, m_report);
	DDX_Control(pDX, IDC_STC_GUIDE, m_guide);				// 改良No.22-0404,22-0812_230414 add
}

BEGIN_MESSAGE_MAP(CdlgReference, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgReference::OnBnClickedOk)
	//ON_CBN_SELCHANGE(IDC_COMBO_TORIHIKICHANGE, &CdlgReference::OnCbnSelchangeComboTorihikichange)	// 修正No.162490 del
	ON_BN_CLICKED(IDC_CHECK_REG_AUTO, &CdlgReference::OnBnClickedCheckRegAuto)
	ON_MESSAGE(WM_USER_1, OnUser1Message)
	ON_MESSAGE(WM_USER_FOCUSSET, OnUserFocusset)
	ON_MESSAGE(WM_USER_FOCUSSET3, OnUserFocusset3)					// 修正_23/04/21 add
	ON_MESSAGE(WM_USER_SELECTCHECK, OnUserSelectCheck)				// 修正No.162394 add
	ON_NOTIFY(NM_DBLCLK, IDC_REFERENCE_REPORT, OnNMDblclkReport)
	ON_NOTIFY(NM_RDBLCLK, IDC_REFERENCE_REPORT, OnNMRdblclkReport)	// 修正No.162345
	//ON_CBN_SETFOCUS(IDC_COMBO_TORIHIKICHANGE, &CdlgReference::OnCbnSetfocusComboTorihikichange)	// 修正No.162437 add	// 修正No.162490 del
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CdlgReference, ICSDialog)
	ON_EVENT(CdlgReference, IDC_ICSDIAG8_KANASEARCH, 3, CdlgReference::TerminationKanaSearchDiag, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CdlgReference, IDC_ICSDIAG8_KANASEARCH, 8, CdlgReference::ComboSelKanaSearchDiag, VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CdlgReference, IDC_ICSDIAG8_KANASEARCH, 2, CdlgReference::EditOFFIcsdiag8Kanasearch, VTS_I2)
	ON_EVENT(CdlgReference, IDC_ICSDIAG8_KANASEARCH, 1, CdlgReference::EditONIcsdiag8Kanasearch, VTS_I2)
END_EVENTSINK_MAP()

// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
HBRUSH CdlgReference::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	ダイアログ表示
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	isRegAuto		…	TRUE=取引先、金融機関を自動で登録する
//				pSort			…	表示順 0=番号順、1=50音順
//				pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
//				nItemSeq		…	帳表で選択されている項目のシーケンス番号
//				pudReference	…	
//				pF9OnSw			…	TRUE=科目ｺﾝﾎﾞﾎﾞｯｸｽで「新規作成」が選択されて呼び出された
//	【戻値】	ID_DLG_OK		…	指定終了押した
//				ID_DLG_CANCEL	…	キャンセル押した
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::ShowDialog( BOOL& isRegAuto, int pSort[], CDatabase* pDB /*=NULL*/, EnumIdDlgType nType /*=ID_DLGTYPE_BANK*/,
							   short nFormSeq /*=ID_FORMNO_011*/, int nGrSeq /*=0*/,
							   long nItemSeq /*=0*/, REFERENCE_INFO* pudReference /*=0*/, BOOL pF9OnSw /*=FALSE*/)
{
	m_nRet = ID_DLG_CANCEL;		// 戻値初期化
	m_fUpdate = FALSE;			// 更新フラグ初期化
	m_nOldRow = -1;				// 前回選択行初期化

	// 初期化
	m_udReference.szName1.Empty();
	m_udReference.szName2.Empty();
	m_udReference.szAdd1.Empty();
	m_udReference.szAdd2.Empty();
	m_isRegAuto = FALSE;
	m_nSort[0] = 0;
	m_nSort[1] = 0;
	m_nSortNum = 0;
	
	// 参照型で分岐
	switch( nType )	{
		case ID_DLGTYPE_BANK:				// 金融機関
		case ID_DLGTYPE_BANK_REFERENCE:
			// 初期化
			m_udBank.m_Seq = 0;				// シーケンス番号
			m_udBank.m_OrderNum = 0;		// 順序番号
			m_udBank.m_BkName1.Empty();		// 金融機関名
			m_udBank.m_BkName2.Empty();		// 支店名
			m_udBank.m_FgDel = 0;			// 削除フラグ
			break;

		case ID_DLGTYPE_ADDRESS:			// 取引先
		case ID_DLGTYPE_ADDRESS_REFERENCE:
			// 初期化
			m_udAddress.m_AdSeq = 0;		// シーケンス番号
			m_udAddress.m_GrSeq = 0;		// グループ番号
			m_udAddress.m_OrderNum = 0;		// 順序番号
			m_udAddress.m_AdName1.Empty();	// 取引先名1
			m_udAddress.m_AdName2.Empty();	// 取引先名2
			m_udAddress.m_AdAdd1.Empty();	// 住所1
			m_udAddress.m_AdAdd2.Empty();	// 住所2
			m_udAddress.m_FgDel = 0;		// 削除フラグ
			m_udAddress.m_InvNo.Empty();	// 登録番号(法人番号)	// インボイス登録番号追加対応_23/11/06 add
			break;

		case ID_DLGTYPE_KAMOKU:				// 科目
			// 初期化
			m_udKamoku.m_FormSeq = 0;		// 様式シーケンス番号
			m_udKamoku.m_KnSeq = 0;			// 科目シーケンス番号
			m_udKamoku.m_KnOrder = 0;		// 順序番号
			m_udKamoku.m_KnCode.Empty();	// 科目コード
			m_udKamoku.m_KnName.Empty();	// 科目名
			m_udKamoku.m_FgDel = 0;			// 削除フラグ
			m_udKamoku.m_FgDft = 0;			// デフォルトフラグ
			break;
	}

	// データベースハンドルがヌルじゃない？
	if(pDB != NULL)		{
		try		{
			// データベース開いてる？
			if(pDB->IsOpen())	{
				m_pDB		= pDB;			// データベースハンドル取得
				m_nType		= nType;		// 参照型取得
				m_nFormSeq	= nFormSeq;		// 様式シーケンス番号取得
				m_nItemSeq	= nItemSeq;		// 帳表で選択されている項目のシーケンス番号
				m_nGrSeq	= nGrSeq;		// グループ番号取得
				m_isRegAuto	= isRegAuto;	// 「取引先、金融機関を自動で登録する」
				m_F9OnSw	= pF9OnSw;		// ｺﾝﾎﾞﾎﾞｯｸｽで「新規作成」が選択されて呼び出された
				m_nSort[0]	= pSort[0];		// 表示順
				m_nSort[1]	= pSort[1];		// 表示順
				m_nSortNum	= 0;
				if(nType == ID_DLGTYPE_ADDRESS || nType == ID_DLGTYPE_ADDRESS_REFERENCE)	{
					if(m_nGrSeq == ID_ADDRESSGR_URI)	{
						m_nSortNum = 1;
					}
				}
				// 検索用文字列取得
				m_udReference.szName1	= pudReference->szName1;
				m_udReference.szName2	= pudReference->szName2;
				m_udReference.szAdd1	= pudReference->szAdd1;
				m_udReference.szAdd2	= pudReference->szAdd2;

				DoModal();					// モーダルで表示

			}
		}
		catch(...)	{
		}
	}

	pSort[0] = m_nSort[0];
	pSort[1] = m_nSort[1];
	isRegAuto = m_isRegAuto;

	// 戻値を返す
	return(m_nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	ダイアログ初期化
// -----------------------------------------------------------------------------------------------------------------------------------
BOOL CdlgReference::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// ツールバーの設定成功？
	if(SetToolBar() == 0)	{
		m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_FAKE, FALSE);
	}

	// 財務dbクラス
	m_pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	// 帳表で選択されている項目のシーケンス番号取得
	m_nItemSeq = GetItemSeq();

	// リスト以外のコントロールの初期化
	InitOtherControls( m_nType, m_nGrSeq );

	// リサイズ
	OnInitDialogEX();

	m_report.ShowWindow(SW_SHOW);

	// レポート初期化
	InitReport(m_nType);
	// レポートデータ取得
	GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum]);
	// 初期位置移動
	InitMovePosition(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_nItemSeq);

	// コンボボックスで「新規作成」が選択されて呼び出された場合は、追加ダイアログを呼び出す
	if(m_F9OnSw == TRUE)	{
		PostMessage(WM_COMMAND, ID_TOOL_BUTTON_F9);
	}
	else	{
		PostMessage(WM_USER_FOCUSSET, 0, 0);
	}

	// 戻値を返す
	return( TRUE );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	ウィンドウメッセージをフィルタリング
// -----------------------------------------------------------------------------------------------------------------------------------
BOOL CdlgReference::PreTranslateMessage(MSG* pMsg)
{
	long					nID = 0;									// ID番号
	int						nCtrID = GetFocusControlID(this);
	int						sw=0;
	char					cShift = (char)GetKeyState(VK_SHIFT);		// SHIFTキー状態取得
	int						idx=0;
	ICSReportRow			*pRow = NULL;
	ICSReportSelectedRows*	pSelRows;	// 修正No.162394 add 
	POINT					pt;	// 修正No.162394 add 
	RECT					rt;	// 修正No.162394 add 

	// キーダウンメッセージ？
	if(pMsg->message == WM_KEYDOWN)	{
// 修正No.162394 add -->
		if(m_report.HasFocus())	{
			PostMessage(WM_USER_SELECTCHECK, 0, 0);
		}
// 修正No.162394 add <--

		sw = 1;
		//if(nCtrID == IDC_COMBO_TORIHIKICHANGE)	sw = 0;		// 修正No.162490 del
		if(m_report.HasFocus())					sw = 0;
		if(nCtrID == IDC_CHECK_REG_AUTO)		sw = 0;
		if(nCtrID == ID_OK_BUTTON)				sw = 0;
		if(nCtrID == IDCANCEL)					sw = 0;
		if(sw == 1)	{
			// カナ検索(IDC_ICSDIAG8_KANASEARCH)にフォーカスがある
			switch(pMsg->wParam)	{
				// カナ検索上で←・→キーを押下した場合
				// 他と同じように選択値を変更せずフォーカス移動する
				case VK_LEFT:
				case VK_RIGHT:
					if(sw == 1)	{
						if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT)	{
							m_DiagKanaSearch.ComboDropDown(ID_DIAG_INDEX_INPUT_SORT, FALSE);
							((CfrmUc000Common *)m_pOwner)->CmnDiagSetCombo(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SORT, m_OldCombo);
							if(pMsg->wParam == VK_LEFT)		pMsg->wParam = VK_F2;
							if(pMsg->wParam == VK_RIGHT)	pMsg->wParam = VK_TAB;
						}
// 修正No.162490 add -->
						else if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_KUBUN)	{
							m_DiagKanaSearch.ComboDropDown(ID_DIAG_INDEX_INPUT_KUBUN, FALSE);
							((CfrmUc000Common *)m_pOwner)->CmnDiagSetCombo(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_KUBUN, m_OldKCombo);
							if(pMsg->wParam == VK_LEFT)		pMsg->wParam = VK_F2;
							if(pMsg->wParam == VK_RIGHT)	pMsg->wParam = VK_TAB;
						}
// 修正No.162490 add <--
					}
					break;

				case VK_TAB:
					// 下記不具合の対応
					// ①表示順のコンボにフォーカスがある状態で他のアプリに行き戻ってくる
					// ②その後、↓↑キーでコンボを変更してもコンボ自身の値が切り替わらない
					// 　(リストの並びは変わっている)
					// TabキーをENTERキーに置き換える
					if( !(cShift & 0x80) )	{
						pMsg->wParam = VK_RETURN;
					}
					break;
			}
		}

		// パラメータで分岐
		switch(pMsg->wParam)	{
			case VK_RETURN:			// Enterキー
				// OnOk起動
				// リストに居る？
				if(m_report.HasFocus())	{	// レポートにいる
					m_btnOK.SetFocus();
					return(1);
				}
				else if(sw == 1)	{
					if( (m_nType == ID_DLGTYPE_BANK && m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SUB) ||
						(m_nType == ID_DLGTYPE_ADDRESS && m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN))	{
							if(m_DataLineMax <= 0)	{
								m_chkRegAuto.SetFocus();
							}
							else	{
// 修正No.162394 del -->
								//m_report.SetFocus();
								//pRow = m_report.GetFocusedRow();
								//if(pRow == NULL)	idx = 0;
								//else				idx = pRow->GetIndex();
								//m_report.SetFocusedRow(idx);
// 修正No.162394 del <--
// 修正No.162394 add -->
								// 選択行のカウントを常に確認。複数行選択されている場合は「選択ボタン」等をDisableにする
								pSelRows = m_report.GetSelectedRows();
								if(pSelRows != NULL)	{
									if(pSelRows->GetCount() > 1) {
										m_report.SetFocus();
										//m_btnOK.EnableWindow(FALSE);
									}
									else {
										m_report.SetFocus();
										pRow = m_report.GetFocusedRow();
										if(pRow == NULL)	idx = 0;
										else				idx = pRow->GetIndex();
										m_report.SetFocusedRow(idx);
										//m_btnOK.EnableWindow(TRUE);
									}
								}
								else	{
									m_report.SetFocus();
									m_report.SetFocusedRow(0);
									//m_btnOK.EnableWindow(FALSE);
								}
								SendMessage(WM_USER_SELECTCHECK, 0, 0);
// 修正No.162394 add <--
							}
							return(TRUE);
					}
				}
// 修正No.162490 del -->
				//else if(GetFocusControlID(this) == IDC_COMBO_TORIHIKICHANGE)	{
				//	m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
				//	return(1);
				//}
// 修正No.162490 del <--

				break;

			case VK_TAB:			// Tabキー
				g_TabSw = FALSE;
				g_TabSw2 = FALSE;	// 修正No.162490 add
				switch(nCtrID)	{
					case ID_OK_BUTTON:
						// SHIFTキーが押されてる？
						if( !(cShift & 0x80) )	{
							m_btnCancel.SetFocus();
							return(1);
						}
						break;

					case IDCANCEL:
						// SHIFTキーが押されてる？
						if( !(cShift & 0x80) )	{
							if(m_nType == ID_DLGTYPE_ADDRESS)	{
								//m_cmbTorihikiChange.SetFocus();							// 修正No.162490 del
								m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_KUBUN);	// 修正No.162490 add
							}
							else	{
								m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
							}
							return(1);
						}
						else	{
							if(m_DataLineMax <= 0)	{
								if(m_chkRegAuto.IsWindowVisible() == FALSE)	{
									FocusSet(1);
									return(1);
								}
							}
						}
						break;

// 修正No.162490 del -->
					//case IDC_COMBO_TORIHIKICHANGE:
					//	// SHIFTキーが押されてる？
					//	if( cShift & 0x80 )	{
					//		m_btnCancel.SetFocus();
					//	}
					//	else	{
					//		m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
					//	}
					//	return(1);
					//	break;
// 修正No.162490 del <--

					case IDC_CHECK_REG_AUTO:
						// SHIFTキーが押されてる？
						if(cShift & 0x80)	{
							FocusSet(1);
							return(1);
						}
						break;

					case IDC_REFERENCE_REPORT:
						// SHIFTキーが押されてる？
						if(cShift & 0x80)	{
							if(m_nType == ID_DLGTYPE_BANK)	{
								m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SUB);
							}
							else	{
								m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
							}
							return(1);
						}
						break;

					case IDC_ICSDIAG8_KANASEARCH:		// ｶﾅ検索ダイアログ
						// SHIFTキーが押されてる？
						if(cShift & 0x80)	{
							if(m_nType == ID_DLGTYPE_BANK)	{	// 検索ボックス2つ
								if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
									g_TabSw = TRUE;
								}
								else if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SUB)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
								}
// 修正No.162490 add -->
								else if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_KUBUN);
									g_TabSw2 = TRUE;
								}
// 修正No.162490 add <--
								else	{
									m_btnCancel.SetFocus();
								}
							}
							else	{	// 検索ボックス1つ
								if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
									g_TabSw = TRUE;
								}
// 修正No.162490 add -->
								else if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_KUBUN);
									g_TabSw2 = TRUE;
								}
// 修正No.162490 add <--
								else	{
									m_btnCancel.SetFocus();
								}
							}
						}
						else	{
							if(m_nType == ID_DLGTYPE_BANK)	{	// 検索ボックス2つ
								if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
								}
								else if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_MAIN)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SUB);
								}
// 修正No.162490 add -->
								else if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_KUBUN)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
								}
// 修正No.162490 add <--
								else	{
									FocusSet(0);
								}
							}
							else	{							// 検索ボックス1つ
								if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_SORT)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
								}
// 修正No.162490 add -->
								else if(m_DiagKanaSearch.GetPosition() == ID_DIAG_INDEX_INPUT_KUBUN)	{
									m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
								}
// 修正No.162490 add <--
								else	{
									FocusSet(0);
								}
							}
						}
						break;
				}
				break;

			case VK_ESCAPE:			// Escキー
				// OnCancel起動
				nID = IDCANCEL;
				break;

			case VK_F2:				// F2キー
// 修正No.162394 add -->
				if(m_nType != ID_DLGTYPE_KAMOKU && nCtrID == IDC_CHECK_REG_AUTO)	{
					pSelRows = m_report.GetSelectedRows();
					if(pSelRows != NULL) {
						if(pSelRows->GetCount() > 1) {
							m_report.SetFocus();
							//m_btnOK.EnableWindow(FALSE);
						}
						else {
							m_report.SetFocus();
							pRow = m_report.GetFocusedRow();
							if(pRow == NULL)	idx = 0;
							else				idx = pRow->GetIndex();
							m_report.SetFocusedRow(idx);
							//m_btnOK.EnableWindow(TRUE);
						}
					}
					else	{
						m_report.SetFocus();
						m_report.SetFocusedRow(0);
					}
					SendMessage(WM_USER_SELECTCHECK, 0, 0);
				}
				else	{
// 修正No.162394 add <--
					keybd_event(VK_SHIFT, 0, 0, 0);					// Shiftキー押している
					keybd_event(VK_TAB, 0, 0, 0);					// Tabキー押している
					keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		// Tabキー放した（※放さないと押っぱなしになる）
					keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	// Shiftキー放した（※放さないと押っぱなしになる）
// 修正No.162394 add -->
				}
// 修正No.162394 add <--
				return( 1 );
				break;

			case VK_F6:				// F6キー
				nID = ID_TOOL_BUTTON_F6;
				break;

			case VK_F7:				// F7キー
				nID = ID_TOOL_BUTTON_F7;
				break;

			case VK_F8:				// F8キー
				nID = ID_TOOL_BUTTON_F8;
				break;

			case VK_F9:				// F9キー
				nID = ID_TOOL_BUTTON_F9;
				break;

			case VK_F11:			// F11キー
				nID = ID_TOOL_BUTTON_F11;
				break;

			case VK_F12:			// F12キー
				nID = ID_TOOL_BUTTON_F12;
				break;

// 修正No.162343 del -->
			//case VK_PRIOR:			// PageUp
			//	// レポートにいる
			//	if(m_report.HasFocus())	m_report.MovePageUp(FALSE, FALSE);
			//	break;

			//case VK_NEXT:			// PageDown
			//	// レポートにいる
			//	if(m_report.HasFocus())	m_report.MovePageDown(FALSE, FALSE);
			//	break;
// 修正No.162343 del <--

// 修正_23/04/21 del -->
			//case VK_DOWN:			// ↓矢印キー
				//// レポートにいる時
				//if(m_report.HasFocus())		{
				//	pRow = m_report.GetFocusedRow();
				//	if(pRow != NULL)	{
				//		idx = pRow->GetIndex();
				//		if(idx + 1 >= m_DataLineMax)	{
				//			// データが登録されている行以上は進まないようにする
				//			return(1);
				//		}
				//	}
				//}
// 修正_23/04/21 del <--
// 修正_23/04/21 add -->
			case VK_UP:				// ↑矢印キー
				// レポートにいる時
				if(m_report.HasFocus())		{
					pRow = m_report.GetFocusedRow();
					if(pRow != NULL)	{
						idx = pRow->GetIndex();
						//if( pMsg->wParam == VK_UP && idx <= 0)	{	// 修正No.162502 del
						if(idx <= 0)	{								// 修正No.162502 add
							// データが登録されている行以上は進まないようにする
							return(1);
						}
					}

					// 現在行の確認
					CheckNowRowPosition(idx-1);
				}
				break;

			case VK_DOWN:			// ↓矢印キー
			case VK_RIGHT:			// →矢印キー	// 修正No.162502 add
				// レポートにいる時
				if(m_report.HasFocus())		{
					pRow = m_report.GetFocusedRow();
					if(pRow != NULL)	{
						idx = pRow->GetIndex();
						//if( (pMsg->wParam == VK_UP && idx <= 0)							// 修正No.162502 del
						// || (pMsg->wParam == VK_DOWN && (idx+1) >= m_DataLineMax) )	{	// 修正No.162502 del
						if((idx+1) >= m_DataLineMax)	{									// 修正No.162502 add
							// データが登録されている行以上は進まないようにする
							return(1);
						}
					}

					// 現在行の確認
					CheckNowRowPosition(idx+1);
				}
// 修正No.162490 add -->
				else if(nCtrID == IDCANCEL)	{
					if(m_nType == ID_DLGTYPE_ADDRESS) {
						m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_KUBUN);
					}
					else {
						m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
					}
					return(1);
				}
// 修正No.162490 add <--
// 修正_23/04/21 add <--
				break;

			case VK_END:			// 
			case VK_HOME:			// 
				return(TRUE);

			default:				// その他キー
				break;
		}
	}
// 修正No.162394 add -->
	// マウス左クリック
	else if(pMsg->message == WM_LBUTTONDOWN) {
		// 現在のカーソルポジションを取得する
		//POINT			pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		// レポートコントロール内でクリックされたかを調べる
		//RECT			rt;
		m_report.GetWindowRect(&rt);
		ScreenToClient(&rt);

		if(pt.x >= rt.left && pt.x <= rt.right && pt.y >= rt.top && pt.y <= rt.bottom)	{
			PostMessage(WM_USER_FOCUSSET3, 0, 0);

			PostMessage(WM_USER_SELECTCHECK, 1, 0);
		}
	}
// 修正No.162394 add <--

	// ID番号あり？
	if(nID != 0)	{
		// メッセージ投げる
		PostMessage(WM_COMMAND, nID);
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	フォーカスのあるコントロールのID取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::GetFocusControlID(CWnd* pParent /*=NULL*/)
{
	int		nRet = 0;			// 戻値
	CWnd*	pControl = NULL;	// コントロール
	char	lpClassName[128];

	// 親ハンドルあり？
	if(pParent != NULL)		{
		// フォーカスのあるコントロールを取得
		pControl = pParent->GetFocus();

		// コントロール取得成功？
		if(pControl != NULL)	{
			// フォーカスを持っているウィンドウがICSDiagの場合、ICSDiag自体のIDではなくICSDiag::EditのIDを取得してしまうので、
			// Editがフォーカスを持っているときはICSDiagがフォーカスを持っていると判定する
			GetClassName(GetFocus()->GetSafeHwnd(), lpClassName, sizeof(lpClassName));
			if(strcmp(lpClassName, _T("Edit")) == 0)	{
				// コントロールID取得
				nRet = m_DiagKanaSearch.GetDlgCtrlID();
			}
			else	{
				// コントロールID取得
				nRet = pControl->GetDlgCtrlID();
			}
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::FocusSet(int pSw)
{
	int						idx;
	ICSReportRow			*pRow;
	ICSReportSelectedRows*	pSelRows;	// 修正No.162394 add

	if(m_DataLineMax > 0)	{
// 修正No.162394 del -->
		//m_report.SetFocus();
		//pRow = m_report.GetFocusedRow();
		//if(pRow == NULL)	idx = 0;
		//else				idx = pRow->GetIndex();
		//m_report.SetFocusedRow(idx);
// 修正No.162394 del <--
// 修正No.162394 add -->
		// 選択行のカウントを常に確認。複数行選択されている場合は「選択ボタン」をDisableにする
		pSelRows = m_report.GetSelectedRows();
		if(pSelRows != NULL) {
			if(pSelRows->GetCount() > 1) {
				m_report.SetFocus();
				//m_btnOK.EnableWindow(FALSE);
			}
			else {
				m_report.SetFocus();
				pRow = m_report.GetFocusedRow();
				if(pRow == NULL)	idx = 0;
				else				idx = pRow->GetIndex();
				m_report.SetFocusedRow(idx);

				//m_btnOK.EnableWindow(TRUE);
			}
		}
		else {
			m_report.SetFocus();
			m_report.SetFocusedRow(0);
		}
		SendMessage(WM_USER_SELECTCHECK, 0, 0);
// 修正No.162394 add <--
	}
	else	{
		if(pSw == 0)	{
			if(m_chkRegAuto.IsWindowVisible() == FALSE)	m_btnCancel.SetFocus();
			else										m_chkRegAuto.SetFocus();
		}
		else	{
			if(m_nType == ID_DLGTYPE_BANK)	{	// 検索ボックス2つ
				m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SUB);
			}
			else	{
				m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	フォーカスセット（ダイアログ起動時用）
// -----------------------------------------------------------------------------------------------------------------------------------
LRESULT	CdlgReference::OnUserFocusset(WPARAM wParam, LPARAM lParam)
{
	ICSReportRow*	pRow;

	if(m_DataLineMax <= 0)	{
		m_btnCancel.SetFocus();
	}

	return(0);
}

// 修正_23/04/21 add -->
// -----------------------------------------------------------------------------------------------------------------------------------
//	フォーカスセット（マウスクリックした）
// -----------------------------------------------------------------------------------------------------------------------------------
LRESULT	CdlgReference::OnUserFocusset3(WPARAM wParam, LPARAM lParam)
{
	int				idx;
	ICSReportRow*	pRow;

	if(m_DataLineMax == 0) {
		m_report.SetFocus();
		m_report.SetFocusedRow(0);
	}
	else	{
		pRow = m_report.GetFocusedRow();
		if(pRow == NULL)	idx = 0;
		else				idx = pRow->GetIndex();
		if(m_DataLineMax <= idx)	{
			m_report.SetFocusedRow(m_DataLineMax - 1);
		}
	}

	return(0);
}
// 修正_23/04/21 add <--

// 修正No.162394 add -->
// -----------------------------------------------------------------------------------------------------------------------------------
//	レポートの選択管理
// -----------------------------------------------------------------------------------------------------------------------------------
LRESULT	CdlgReference::OnUserSelectCheck(WPARAM wParam, LPARAM lParam)
{
	int						ii=0;
	int						selcnt = 0;		// 修正No.162495 add
	long					nInx=0;
	BOOL					bsw[7] = {FALSE};
	ICSReportRows*			pRows;
	ICSReportSelectedRows*	pSelRows;

	for(ii=0; ii<7; ii++)	{
		bsw[ii] = FALSE;
	}

	// 選択行のカウントを常に確認。複数行選択されている場合は「選択ボタン」をDisableにする
	pRows = m_report.GetRows();
	if(pRows != NULL) {
		pSelRows = m_report.GetSelectedRows();
		if(pSelRows != NULL)	{
// 修正No.162495 del -->
			//if(pSelRows->GetCount() == 1)	{
// 修正No.162495 del <--
// 修正No.162495 add -->
			selcnt = pSelRows->GetCount();
			if(selcnt == 1)	{
// 修正No.162495 add <--
				if(m_DataLineMax == 0)	{
					bsw[0] = FALSE;									// F6編集
					bsw[1] = FALSE;									// F7削除
					bsw[2] = FALSE;									// F8挿入
					bsw[3] = TRUE;									// F9追加
					bsw[4] = FALSE;									// F11一行上へ
					bsw[5] = FALSE;									// F11一行下へ
					bsw[6] = FALSE;									// OKボタン

				}
				if(m_DataLineMax == 1)	{
					if(m_nSort[m_nSortNum] == 0)	{	// 番号順
						bsw[0] = TRUE;									// F6編集
						bsw[1] = TRUE;									// F7削除
						bsw[2] = TRUE;									// F8挿入
						bsw[3] = TRUE;									// F9追加
						bsw[4] = FALSE;									// F11一行上へ
						bsw[5] = FALSE;									// F11一行下へ
						bsw[6] = TRUE;									// OKボタン
					}
					else	{							// 50音順
						bsw[0] = TRUE;									// F6編集
						bsw[1] = TRUE;									// F7削除
						bsw[2] = FALSE;									// F8挿入
						bsw[3] = TRUE;									// F9追加
						bsw[4] = FALSE;									// F11一行上へ
						bsw[5] = FALSE;									// F11一行下へ
						bsw[6] = TRUE;									// OKボタン
					}
				}
				else if(m_DataLineMax > 1)	{
					nInx = GetListIndex();

					if(m_nSort[m_nSortNum] == 0)	{	// 番号順
						bsw[0] = TRUE;									// F6編集
						bsw[1] = TRUE;									// F8挿入
						bsw[2] = TRUE;									// F8挿入
						bsw[3] = TRUE;									// F9追加
						if(nInx == 0)	bsw[4] = FALSE;					// F11一行上へ
						else			bsw[4] = TRUE;					// F11一行上へ
						if((nInx+1) == m_DataLineMax)	bsw[5] = FALSE;	// F11一行下へ
						else							bsw[5] = TRUE;	// F11一行下へ
						bsw[6] = TRUE;									// OKボタン
					}
					else	{							// 50音順
						bsw[0] = TRUE;									// F6編集
						bsw[1] = TRUE;									// F8挿入
						bsw[2] = FALSE;									// F8挿入
						bsw[3] = TRUE;									// F9追加
						if(nInx == 0)	bsw[4] = FALSE;					// F11一行上へ
						else			bsw[4] = TRUE;					// F11一行上へ
						if((nInx+1) == m_DataLineMax)	bsw[5] = FALSE;	// F11一行下へ
						else							bsw[5] = TRUE;	// F11一行下へ
						bsw[6] = TRUE;									// OKボタン
					}
				}
			}
// 修正No.162495 add -->
			else if(selcnt == 0)	{
				bsw[0] = FALSE;									// F6編集
				bsw[1] = FALSE;									// F7削除
				bsw[2] = FALSE;									// F8挿入
				bsw[3] = FALSE;									// F9追加
				bsw[4] = FALSE;									// F11一行上へ
				bsw[5] = FALSE;									// F11一行下へ
				bsw[6] = FALSE;									// OKボタン
			}
// 修正No.162495 add <--
			else	{
				bsw[0] = FALSE;									// F6編集
				bsw[1] = TRUE;									// F7削除
				bsw[2] = FALSE;									// F8挿入
				bsw[3] = FALSE;									// F9追加
				bsw[4] = FALSE;									// F11一行上へ
				bsw[5] = FALSE;									// F11一行下へ
				bsw[6] = FALSE;									// OKボタン
			}
		}
	}

	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F6,  bsw[0]);		// [F6]編集
	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7,  bsw[1]);		// [F7]削除
	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F8,  bsw[2]);		// [F8]挿入
	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F9,  bsw[3]);		// [F9]追加
	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F11, bsw[4]);		// [F11]一行上へ
	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F12, bsw[5]);		// [F12]一行下へ
	m_btnOK.EnableWindow(bsw[6]);												// OKボタン

	return(0);
}
// 修正No.162394 add <--

// -----------------------------------------------------------------------------------------------------------------------------------
//	メッセージ マップを処理
// -----------------------------------------------------------------------------------------------------------------------------------
BOOL CdlgReference::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// パラメータで分岐
	switch(wParam)	{
		case ID_TOOL_BUTTON_F2:		// [F2]前項目
			PostMessage(WM_KEYDOWN, VK_F2);
			return(1);
			break;

		case ID_TOOL_BUTTON_F6:		// [F6]編集
			OnToolButtonF6();
			break;

		case ID_TOOL_BUTTON_F7:		// [F7]削除
			OnToolButtonF7();
			break;

		case ID_TOOL_BUTTON_F8:		// [F8]挿入
			OnToolButtonF8();
			break;

		case ID_TOOL_BUTTON_F9:		// [F9]追加
			OnToolButtonF9();
			break;

		case ID_TOOL_BUTTON_F11:	// [F11]１行上へ
			OnToolButtonF11();
			break;

		case ID_TOOL_BUTTON_F12:	// [F12]１行下へ
			OnToolButtonF12();
			break;
	}
	return ICSDialog::OnCommand(wParam, lParam);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	レポートコントロール	コントロール内でマウス左ボタンがダブルクリックされた
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnNMDblclkReport(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	if(pItemNotify == 0)	return;

// 修正_23/04/21 add -->
	//変更のあったカラム
	ICSReportColumn*	pColumn		= pItemNotify->pColumn;
	int					indexColumn	= pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*		pRow		= pItemNotify->pRow;
	int					indexRow	= pRow->GetIndex();

	if(indexRow >= m_DataLineMax)	{
		return;
	}
// 修正_23/04/21 add <--

	// 選択ボタンクリックイベント起動
	OnBnClickedOk();
}

// 修正No.162345 add -->
// -----------------------------------------------------------------------------------------------------------------------------------
//	レポートコントロール
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnNMRdblclkReport(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICSReportColumns*	pColumns = m_report.GetColumns();
	ICSReportColumn*	pColumn;

	// 列を非表示にする
	if(m_nType == ID_DLGTYPE_BANK) {
		//「シーケンス番号」
		pColumn = pColumns->GetAt(0);
		pColumn->SetVisible(FALSE);
	}
	else if(m_nType == ID_DLGTYPE_ADDRESS) {
		//「シーケンス番号」
		pColumn = pColumns->GetAt(0);
		pColumn->SetVisible(FALSE);
	}
	else if(m_nType == ID_DLGTYPE_KAMOKU) {
		//「シーケンス番号」
		pColumn = pColumns->GetAt(0);
		pColumn->SetVisible(FALSE);
		// 「デフォルトフラグ」
		pColumn = pColumns->GetAt(4);
		pColumn->SetVisible(FALSE);
	}
}
// 修正No.162345 add <--

// 修正No.162490 cor -->
//// -----------------------------------------------------------------------------------------------------------------------------------
////	参照型が取引先の場合、コンボボックスによって得意先と仕入先を切り替える
//// -----------------------------------------------------------------------------------------------------------------------------------
////	【引数】	なし
////	【戻値】	なし
//// -----------------------------------------------------------------------------------------------------------------------------------
//void CdlgReference::OnCbnSelchangeComboTorihikichange()
//{
//	int					nIndex;		// 修正No.162394 add
//	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体
//
//	switch(m_cmbTorihikiChange.GetCurSel())	{
//		case 0:		// 仕入先(買)
//			m_nGrSeq = ID_ADDRESSGR_KAI;
//			break;
//		case 1:		// 得意先(売)
//			m_nGrSeq = ID_ADDRESSGR_URI;
//			break;
//	}
//
//	// カナ検索ダイアログ初期化
//	m_nSortNum = m_cmbTorihikiChange.GetCurSel();
//	m_clsFunc.DiagInit(&udData);
//	m_DiagKanaSearch.GetData(ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData);
//	udData.data_combo = m_nSort[m_nSortNum];
//	m_DiagKanaSearch.SetData(ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData);
//	m_Kana1 = _T("");
//	m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN, ID_DIAGDATA_EDIT, _T(""));
//
//	// リストの更新/表示データの切替
//	GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
//				  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
//
//// 修正No.162436 del -->
////// 修正No.162394 add -->
////	// リストのトップ
////	nIndex = m_report.GetTopRowIndex();
////	// 位置移動
////	MovePosition(nIndex, 0);
////// 修正No.162394 add <--
//// 修正No.162436 del <--
//// 修正No.162436 add -->
//	m_report.SetFocusedRow(0);
//
//	// 現在行の確認
//	CheckNowRowPosition();
//// 修正No.162436 add <--
//}
// ---------------------
void CdlgReference::KanaSearch_ComboSelChengeTorihiki(short index)
{
	int					nIndex;
	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体

	// 仕入先/得意先コンボボックスの取得
	m_clsFunc.DiagInit(&udData);
	m_DiagKanaSearch.GetData(index, (LPUNKNOWN)&udData);
	m_nSortNum = udData.data_combo;
	switch(m_nSortNum)	{
		case 0:		// 仕入先(買)
			m_nGrSeq = ID_ADDRESSGR_KAI;
			break;
		case 1:		// 得意先(売)
			m_nGrSeq = ID_ADDRESSGR_URI;
			break;
	}

	// カナ検索ダイアログ初期化
	m_clsFunc.DiagInit(&udData);
	m_DiagKanaSearch.GetData(ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData);
	udData.data_combo = m_nSort[m_nSortNum];
	m_DiagKanaSearch.SetData(ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData);

	m_Kana1 = _T("");
	m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN, ID_DIAGDATA_EDIT, _T(""));

	// リストの更新/表示データの切替
	GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
				  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));

	m_report.SetFocusedRow(0);

	// 現在行の確認
	CheckNowRowPosition();
}
// 修正No.162490 cor <--

// -----------------------------------------------------------------------------------------------------------------------------------
//	カナ検索	入力状態でキーが押された場合のイベント（ICSDiagコントロール）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	index	…	コントロール内のオフセット
//				nCnar	…	仮想キーコード
//				length	…	入力されたデータ長
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::TerminationKanaSearchDiag(short index, short nChar, short length, LPUNKNOWN data)
{
	char		cShift = (char)GetKeyState(VK_SHIFT);	// SHIFTキー状態取得

	// 仮想キーコードで分岐
	switch(nChar)	{
		case VK_TAB:		// 「Tab」キーが押された
			if(index == ID_DIAG_INDEX_INPUT_SORT)	{
				// 表示順コンボボックスが変更された場合にのみ
				if(g_TabSw == FALSE)	{
					// SHIFTキーが押されてる？
					if(cShift & 0x80)	{
						if(m_nType == ID_DLGTYPE_ADDRESS)	{
							//m_cmbTorihikiChange.SetFocus();							// 修正No.162490 del
							m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_KUBUN);	// 修正No.162490 add
						}
						else	{
							m_btnCancel.SetFocus();
						}
					}
					else	{
						m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
					}
				}
				if(g_TabSw == TRUE)		g_TabSw = FALSE;
			}
// 修正No.162490 add -->
			else if(index == ID_DIAG_INDEX_INPUT_KUBUN) {
				// 表示順コンボボックスが変更された場合にのみ
				if(g_TabSw2 == FALSE)	{
					// SHIFTキーが押されてる？
					if(cShift & 0x80)	{
						m_btnCancel.SetFocus();
					}
					else	{
						m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
					}
				}
				if(g_TabSw2 == TRUE)	g_TabSw2 = FALSE;
			}
// 修正No.162490 add <--

			break;

		case VK_RETURN:		// 「Enter」キーが押された
			// 表示切替
			if(index == ID_DIAG_INDEX_INPUT_SORT)	{
				m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_MAIN);
			}
			else if(index == ID_DIAG_INDEX_INPUT_MAIN)	{
				if(m_Kana1 != m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN))	{
					// リストの更新
					GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
								  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
				}
				m_Kana1 = m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN);

				if(m_nType == ID_DLGTYPE_BANK)	{	// 検索ボックス2つ
					m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SUB);
				}
				else	{
					FocusSet(0);
				}
			}
			else if(index == ID_DIAG_INDEX_INPUT_SUB)	{
				if(m_Kana2 != m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB))	{
					// リストの更新
					GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
								  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
				}
				m_Kana2 = m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB);
				FocusSet(0);
			}
// 修正No.162490 add -->
			else if(index == ID_DIAG_INDEX_INPUT_KUBUN)	{
				m_DiagKanaSearch.SetPosition(ID_DIAG_INDEX_INPUT_SORT);
			}
// 修正No.162490 add <--
			break;

		default:		// その他
			break;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	カナ検索(表示順)	コンボボックスが変更された場合のイベント（ICSDiagコントロール）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	index	…	コントロール内のオフセット
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::ComboSelKanaSearchDiag(short index, LPUNKNOWN data)
{
	// 表示順コンボボックスが変更された場合にのみ
	if(index == ID_DIAG_INDEX_INPUT_SORT)	{
		KanaSearch_ComboSelChenge(index);
	}
// 修正No.162490 add -->
	else if(index == ID_DIAG_INDEX_INPUT_KUBUN) {
		KanaSearch_ComboSelChengeTorihiki(index);
	}
// 修正No.162490 add <--
}

// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::EditOFFIcsdiag8Kanasearch(short index)
{
	if(index == ID_DIAG_INDEX_INPUT_SORT)	{
		KanaSearch_ComboSelChenge(index);
	}
// 修正No.162490 add -->
	else if(index == ID_DIAG_INDEX_INPUT_KUBUN)	{
		KanaSearch_ComboSelChengeTorihiki(index);
	}
// 修正No.162490 add <--
	else if(index == ID_DIAG_INDEX_INPUT_MAIN)	{
		if(m_Kana1 != m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN))	{
			// リストの更新
			GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
						  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
		}
		m_Kana1 = m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN);
	}
	else if(index == ID_DIAG_INDEX_INPUT_SUB)	{
		if(m_Kana2 != m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB))	{
			// リストの更新
			GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
						  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
		}
		m_Kana2 = m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB);
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::EditONIcsdiag8Kanasearch(short index)
{
	DIAGRAM_DATA			diadata;
// 修正No.162437 add -->
	ICSReportRow*			pRow;
	ICSReportRows*			pRows;
	ICSReportSelectedRows*	pSelRows;
	long					nInx=0;

	switch(index)	{
		case ID_DIAG_INDEX_INPUT_SORT:
		case ID_DIAG_INDEX_INPUT_MAIN:
		case ID_DIAG_INDEX_INPUT_SUB:
			pRows = m_report.GetRows();
			if(pRows != NULL)	{
				pSelRows = m_report.GetSelectedRows();
				if(pSelRows != NULL)	{
					if(pSelRows->GetCount() > 1) {
						pRow = m_report.GetFocusedRow();
						nInx = pRow->GetIndex();
						m_report.SetFocusedRow(nInx);

						PostMessage(WM_USER_SELECTCHECK, 0, 0);
					}
				}
			}
			break;

		default:
			break;
	}
// 修正No.162437 add <--

	if(index == ID_DIAG_INDEX_INPUT_SORT)	{
		m_DiagKanaSearch.GetData(index,(LPUNKNOWN)&diadata);
		m_OldCombo = diadata.data_combo;
	}
// 修正No.162490 add -->
	else if(index == ID_DIAG_INDEX_INPUT_KUBUN)	{
		m_DiagKanaSearch.GetData(index,(LPUNKNOWN)&diadata);
		m_OldKCombo = diadata.data_combo;
	}
// 修正No.162490 add <--

}

// -----------------------------------------------------------------------------------------------------------------------------------
//	カナ検索(表示順)	コンボボックスのデータが変わったかどうか
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	index	…	コントロール内のオフセット
//				data	…	入力されたデータ（DIAGRAM_DATA構造体のポインタ）
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::KanaSearch_ComboSelChenge(short index)
{
	short				sign=-1;
	int					nTopIndex=0;
	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体

	udData.data_combo = -1;
	m_DiagKanaSearch.GetData(index, (LPUNKNOWN)&udData);
	sign = udData.data_combo;

	if(sign == -1)	{
		return;
	}

	if(m_nSort[m_nSortNum] != sign)	{
		m_nSort[m_nSortNum] = sign;

		// 表示順：番号順
		// 表示順：50音順
		// 前回選択行をクリア
		m_nOldRow = -1;
		// リストのトップ
		nTopIndex = m_report.GetTopRowIndex();
		// リストの更新
		GetReportData(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
					  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
// 修正No.162436 del -->
		//// 位置移動
		//MovePosition(nTopIndex, 0);
// 修正No.162436 del <--
// 修正No.162436 add -->
		m_report.SetFocusedRow(0);
		// 現在行の確認
		CheckNowRowPosition();
// 修正No.162436 add <--

		// 下記不具合の対応
		// ①表示順のコンボにフォーカスがある状態で他のアプリに行き戻ってくる
		// ②その後、↓↑キーでコンボを変更してもコンボ自身の値が切り替わらない
		//   (リストの並びは変わっている)
		m_DiagKanaSearch.RedrawWindow();
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
// 「取引先、金融機関を自動で登録する」	クリックイベント
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnBnClickedCheckRegAuto()
{
	int		state = 0;

	state = m_chkRegAuto.GetCheck();
	if(state == BST_CHECKED)	m_isRegAuto = TRUE;
	else						m_isRegAuto = FALSE;
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	ボタンクリック（選択ボタン）
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnBnClickedOk()
{
	// OKボタン有効？
	if(m_btnOK.IsWindowEnabled() == TRUE)	{
// 修正No.162394 del -->
		//if(m_DataLineMax > m_report.GetFocusedRow()->GetIndex())	{
// 修正No.162394 del <--
// 修正No.162394 add -->
		ICSReportRow	*pRow = NULL;

		// 頁内でのインデックス取得
		pRow = m_report.GetFocusedRow();
		if(pRow == NULL) return;

		if(m_DataLineMax > pRow->GetIndex())	{
// 修正No.162394 add <--
			// 選択されたリストのレコード取得成功？
			if(GetSelectRecord(m_pDB, m_nType, m_nFormSeq, m_nGrSeq) == 0) {
				// 戻値に選択ボタンが押されたをセット
				m_nRet = ID_DLG_OK;
			}

			ICSDialog::OnOK();
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	OnOkイベント
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnOK()
{
//	ICSDialog::OnOK();
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	F6（編集）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnToolButtonF6() 
{
	// ボタン有効？
	if(m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F6) != 0)	{
		// レコードの編集あり？
		if(SetRecordEdit(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum]) != 0)	{
			// 更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	F7（削除）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnToolButtonF7()
{
	int			nRet;		//	戻値
	CArray<long, long>		delseq;							// 選択された行のSEQNoをセット

	// ボタン有効？
	if(m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F7) == 0)	return;

	if(m_nType == ID_DLGTYPE_KAMOKU)	{		// 科目
		// 先に削除可能な科目かどうかチェックする。（規定科目、財務連動科目）
		// 削除可能な場合は、削除しようとしている科目が使用されていないかチェック
		nRet = DeleteCheckKamoku(m_pDB, m_nFormSeq, &delseq);
		if(nRet < 0)	{
			return;
		}
	}
	else if(m_nType == ID_DLGTYPE_BANK)		{	// 金融機関
		// 削除しようとしている金融機関が他の様式で使用されていないかチェック
		nRet = DeleteCheckKinyuukikan(m_pDB, m_nFormSeq, &delseq);
		if(nRet < 0)	{
			return;
		}
	}
	else if(m_nType == ID_DLGTYPE_ADDRESS)	{	// 取引先
		// 削除しようとしている取引先が他の様式で使用されていないかチェック
		nRet = DeleteCheckTorihikisaki(m_pDB, m_nFormSeq, m_nGrSeq, &delseq);
		if(nRet < 0)	{
			return;
		}
	}

	// レコードの削除
	nRet = SetRecordDelete(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], &delseq);
	// レコードの削除あり？
	if(nRet >= 0)	{
		// 更新フラグをオン
		m_fUpdate = TRUE;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	F8（挿入）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnToolButtonF8()
{
	// ボタン有効？
	if(m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F8) != 0)	{
		// レコードの挿入あり？
		if(SetRecordInsert(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum]) != 0)	{
			// 更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	F9（追加）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnToolButtonF9()
{
	// ボタン有効？
	if(m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F9) != 0)	{
		// レコードの追加あり？
		if(SetRecordInsert(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], TRUE) != 0)	{
			// 更新フラグをオン
			m_fUpdate = TRUE;
		}
		// コンボボックスで「新規作成」が選択されて呼び出された場合は、ダイアログを閉じる
		if(m_F9OnSw == TRUE)	{
			// データベースの更新有り
			if(m_fUpdate == TRUE)	{
				OnBnClickedOk();
			}
			else	{
				m_nRet = ID_DLG_CANCEL;
				EndDialog(ID_DLG_CANCEL);
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	F11（１行上へ）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnToolButtonF11()
{
	// ボタン有効？
	if(m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F11) != 0)	{
		// レコードを一行上へ移動した？
		if(SetRecordUpDown(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], -1) != 0)	{
			// 更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	F12（１行下へ）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::OnToolButtonF12()
{
	// ボタン有効？
	if(m_tbrToolBar.GetToolBarCtrl().IsButtonEnabled(ID_TOOL_BUTTON_F12) != 0)	{
		// レコードを一行下へ移動した？
		if(SetRecordUpDown(m_pDB, m_nType, m_nFormSeq, m_nGrSeq, m_nSort[m_nSortNum], 1) != 0)	{
			// 更新フラグをオン
			m_fUpdate = TRUE;
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	帳表で選択されている項目のシーケンス番号取得
//	（※参照のみ＝自動登録しない場合でも選択項目をインバース表示するため追加）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	帳表で選択されている項目のシーケンス番号
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::GetItemSeq(void)
{
	int				nRet = 0;			// 戻値

	// 参照型で分岐
	switch(m_nType)	{
		case ID_DLGTYPE_BANK_REFERENCE:		// 金融機関（参照のみ＝自動登録しない）
			// 金融機関のシーケンス番号検索
			nRet = SearchBankItemSeq();
			// 参照型を通常に戻す
			m_nType = ID_DLGTYPE_BANK;
			break;

		case ID_DLGTYPE_ADDRESS_REFERENCE:	// 取引先（参照のみ＝自動登録しない）
			// 取引先のシーケンス番号検索
			nRet = SearchAddressItemSeq();
			// 参照型を通常に戻す
			m_nType = ID_DLGTYPE_ADDRESS;
			break;

		default:							// 上記以外
			nRet = m_nItemSeq;
			break;
	}
	
	// 戻値を返す
	return( nRet ); 
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関のシーケンス番号検索
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	シーケンス番号
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::SearchBankItemSeq(void)
{
	int				nRet = 0;		// 戻値
	CdbUcLstBank	mfcRec(m_pDB);	// 金融機関名称テーブル

	// 重複確認
	if(mfcRec.CheckRepeat(m_udReference.szName1, m_udReference.szName2) == DB_ERR_OK)	{
		// 重複あり？
		if(!mfcRec.IsEOF())	{
			// シーケンス番号取得
			nRet = mfcRec.m_Seq;
		}
	}

	// 閉じる
	mfcRec.Fin();

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先のシーケンス番号検索
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	シーケンス番号
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::SearchAddressItemSeq(void)
{
	int				nRet = 0;			// 戻値
	CdbUcLstAddress	mfcRec(m_pDB);	// 取引先名称テーブル

	// 重複確認
	if(mfcRec.CheckRepeat(m_udReference.szName1, m_udReference.szName2, m_udReference.szAdd1, m_udReference.szAdd2, m_nGrSeq) == DB_ERR_OK)	{
		// 重複あり？
		if(!mfcRec.IsEOF())	{
			// シーケンス番号取得
			nRet = mfcRec.m_AdSeq;
		}
	}

	// 閉じる
	mfcRec.Fin();

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	ツールバーの設定
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	0		…	成功
//				0以外	…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::SetToolBar(void)
{
	CRect	ClientStart;	// クライアント開始矩形
	CRect	ClientNow;		// クライアント現在矩形
	CPoint	Offset;			// オフセット位置
	CRect	Child;			// 子ウィンドウ矩形
	CWnd*	pChild = NULL;	// 子ウィンドウハンドル
	CRect	Window;			// ウィンドウ矩形
	
	// ツールバー作成
	if(m_tbrToolBar.Create(this) == 0)	{
		// エラー
		return(-1);
	}
	
	// ビットマップ読み込み
	if(m_tbrToolBar.LoadToolBar(IDR_REFERENCE_BAR) == 0)	{
		// エラー
		return(-2);
	}
	
	//------------------------------
	//	スタイル設定
	//		CBRS_TOOLTIPS	…	ツールチップ表示
	//		CBRS_FLYBY		…	メッセージテキストの更新
	//		CBRS_BORDER_ANY	…	フレームを任意に設定
	//------------------------------
	m_tbrToolBar.SetBarStyle(m_tbrToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_BORDER_ANY);
	// 拡張スタイルをフラットツールバーに設定
	m_tbrToolBar.ModifyStyle(0, 0x0800);

	// クライアント領域での再配置/サイズ変更
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	
	// 成功
	return(0);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	レポートコントロール初期化
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	nType	…	参照型
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::InitReport( EnumIdDlgType nType )
{
	int					nCnt;			// カウント用
	int					nColumnCnt=0;	// カラムカウント数
	int					ii=0;
	CRect				rect;
	ICSReportColumn*	clm;
	ICSReportColumns*	clms;
	
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);

	m_report.GetWindowRect(rect);

	//------------------------------
	// 0カラム目（非表示）
	//------------------------------
	m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("シーケンス番号"), 0, FALSE, ICS_REPORT_NOICON, FALSE, FALSE, DT_CENTER));
	nColumnCnt++;	// カラムカウント数インクリメント

// 修正No.162345 del -->
	////------------------------------
	//// 1カラム目
	////------------------------------
	//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("番号"), rect.Width()*10/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
	//nColumnCnt++;	// カラムカウント数インクリメント
// 修正No.162345 del <--

	// 参照型で分岐
	switch(nType)	{
		//++++++++++++++++++++++++++++++
		case ID_DLGTYPE_BANK:		// 金融機関
			// 最大カラム数
			m_nColumnMax = 6;

// 修正No.162345 del -->
			////------------------------------
			//// 2カラム目　金融機関名
			////------------------------------
			//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("金融機関名"), rect.Width()*32/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			//nColumnCnt++;	// カラムカウント数インクリメント
			////------------------------------
			//// 3カラム目　カナ
			////------------------------------
			//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), (rect.Width()*12/100)-1, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			//nColumnCnt++;	// カラムカウント数インクリメント
			////------------------------------
			//// 4カラム目　支店名
			////------------------------------
			//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("支店名"), rect.Width()*32/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			//nColumnCnt++;	// カラムカウント数インクリメント
			////------------------------------
			//// 5カラム目　カナ
			////------------------------------
			//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), (rect.Width()*12/100)-1, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			//nColumnCnt++;	// カラムカウント数インクリメント
// 修正No.162345 del <--
// 修正No.162345 add -->
			//------------------------------
			// 1カラム目
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("番号"), rect.Width()*10/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;
			//------------------------------
			// 2カラム目　金融機関名
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("金融機関名"), rect.Width()*33/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;
			//------------------------------
			// 3カラム目　カナ
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), rect.Width()*12/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;
			//------------------------------
			// 4カラム目　支店名
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("支店名"), rect.Width()*33/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;
			//------------------------------
			// 5カラム目　カナ
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), rect.Width()*12/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;

			// 列を非表示にする
			clms = m_report.GetColumns();
			//「シーケンス番号」
			clm = clms->GetAt(0);
			clm->SetVisible(FALSE);

			clm->AllowRemove(FALSE);	// ドラッグによるカラムの削除不可
// 修正No.162345 add <--

			break;

		//++++++++++++++++++++++++++++++
		case ID_DLGTYPE_ADDRESS:	// 取引先
			if(bG_InvNo == FALSE)	{	// インボイス登録番号追加対応_23/11/06 add
				// 最大カラム数
				m_nColumnMax = 7;
// 修正No.162345 add -->
				//------------------------------
				// 1カラム目
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("番号"), rect.Width()*10/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
// 修正No.162345 add <--
				//------------------------------
				// 2カラム目　名称（上段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("名称（上段）"), rect.Width()*27/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 3カラム目　名称（下段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("名称（下段）"), rect.Width()*27/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 4カラム目　カナ
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), rect.Width()*10/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 5カラム目　所在地（上段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("所在地（上段）"), rect.Width()*54/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 6カラム目　所在地（下段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("所在地（下段）"), rect.Width()*54/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
// インボイス登録番号追加対応_23/11/06 add -->
			}
			else	{
				// 最大カラム数
				m_nColumnMax = 8;
				//------------------------------
				// 1カラム目
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("番号"), rect.Width()*6/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 2カラム目　登録番号
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("登録番号(法人番号)"), rect.Width()*24/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 3カラム目　名称（上段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("名称（上段）"), rect.Width()*26/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 4カラム目　名称（下段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("名称（下段）"), rect.Width()*26/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 5カラム目　カナ
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), rect.Width()*9/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 6カラム目　所在地（上段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("所在地（上段）"), rect.Width()*52/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
				//------------------------------
				// 7カラム目　所在地（下段）
				//------------------------------
				m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("所在地（下段）"), rect.Width()*52/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
				nColumnCnt++;
			}
// インボイス登録番号追加対応_23/11/06 add <--

// 修正No.162345 add -->
			// 列を非表示にする
			clms = m_report.GetColumns();
			//「シーケンス番号」
			clm = clms->GetAt(0);
			clm->SetVisible(FALSE);

			clm->AllowRemove(FALSE);	// ドラッグによるカラムの削除不可
// 修正No.162345 add <--

			break;

		//++++++++++++++++++++++++++++++
		case ID_DLGTYPE_KAMOKU:		// 科目
			// 最大カラム数
			m_nColumnMax = 5;

// 修正No.162345 del -->
			////------------------------------
			//// 2カラム目　名称
			////------------------------------
			//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("名称"), rect.Width()*73/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			//nColumnCnt++;	// カラムカウント数インクリメント
			////------------------------------
			//// 3カラム目　カナ
			////------------------------------
			//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), rect.Width()*14/100, FALSE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			//nColumnCnt++;	// カラムカウント数インクリメント
			////-----------------------------------------
			//// 4カラム目（デフォルトフラグ・非表示）
			////-----------------------------------------
			//m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("デフォルトフラグ"), 0, FALSE, ICS_REPORT_NOICON, FALSE, FALSE, DT_CENTER));
			//nColumnCnt++;
// 修正No.162345 del <--
// 修正No.162345 add -->
			//------------------------------
			// 1カラム目
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("番号"), rect.Width()*10/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;
			//------------------------------
			// 2カラム目　名称
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("名称"), rect.Width()*74/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;
			//------------------------------
			// 3カラム目　カナ
			//------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("カナ"), rect.Width()*16/100, TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_CENTER));
			nColumnCnt++;
			//-----------------------------------------
			// 4カラム目（デフォルトフラグ・非表示）
			//-----------------------------------------
			m_report.AddColumn(new ICSReportColumn(nColumnCnt, _T("デフォルトフラグ"), 0, TRUE, ICS_REPORT_NOICON, FALSE, FALSE, DT_CENTER));
			nColumnCnt++;

			// 列を非表示にする
			clms = m_report.GetColumns();
			//「シーケンス番号」
			clm = clms->GetAt(0);
			clm->SetVisible(FALSE);
			// 「デフォルトフラグ」
			clm = clms->GetAt(4);
			clm->SetVisible(FALSE);

			clm->AllowRemove(FALSE);	// ドラッグによるカラムの削除不可
// 修正No.162345 add <--

			break;
	}

	if(nType == ID_DLGTYPE_KAMOKU)	{
		m_report.SetMultipleSelection(FALSE);	// 複数行選択禁止
	}
	m_report.SetSortable(FALSE);				// ソート禁止
	m_report.SetDraggable(FALSE);				// ヘッダードラッグ禁止
	m_report.SetRemovable(FALSE);				// ヘッダー削除禁止

	// カラムのサイズを変更できるよう設定
	// ※変更後の幅は保存しない
	clms = m_report.GetColumns();
	for(ii=0; ii<m_nColumnMax; ii++)	{
		clm = clms->GetAt(ii);
		clm->EnableResize(TRUE);
	}

	// グリッド線の設定
	CDWordArray		arryGridColumn;
	CDWordArray		arrayGridColor;

	// 縦線
	for(ii=0; ii<m_nColumnMax; ii++)	{
		arryGridColumn.Add(ii);
		arrayGridColor.Add(RGB(255, 0, 0));
	}
	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);

	// 横線
	m_report.SetGridPerCount(FALSE, 5, RGB(0x00, 0xFF, 0xFF), RGS_SOLID);

	// レポート再描画
	m_report.Populate();

	// フォント設定
	PostMessage(WM_USER_1, UM1_REPINIZ);
}

// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
LRESULT	CdlgReference::OnUser1Message(WPARAM wParam, LPARAM lParam)
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
		CRect	rect1,rect2;
		CWnd*	ctlwnd1 = m_pWnd->GetDlgItem(IDC_STC_DMY1);
		CWnd*	ctlwnd2 = m_pWnd->GetDlgItem(IDC_STC_DMY2);

		// フォントの設定
		listfont = ctlwnd1->GetFont();
		listfont->GetLogFont(&LogFont);
		//LogFont.lfHeight = LogFont.lfHeight * 108 / 100;
		//LogFont.lfWidth  = LogFont.lfWidth  * 108 / 100;
		//LogFont.lfWeight = LogFont.lfWeight * 100 / 100;
		LogFont.lfHeight = LogFont.lfHeight * 102 / 100;
		LogFont.lfWidth  = LogFont.lfWidth  * 102 / 100;
		LogFont.lfWeight = LogFont.lfWeight * 100 / 100;
		m_report.SetTextFont(LogFont);
		listfont->GetLogFont(&LogFont);
		//LogFont.lfHeight = LogFont.lfHeight * 108 / 100;
		//LogFont.lfWidth  = LogFont.lfWidth  * 108 / 100;
		//LogFont.lfWeight = LogFont.lfWeight * 100 / 100;
		LogFont.lfHeight = LogFont.lfHeight * 102 / 100;
		LogFont.lfWidth  = LogFont.lfWidth  * 102 / 100;
		LogFont.lfWeight = LogFont.lfWeight * 100 / 100;
		m_report.SetHeaderFont(LogFont);

		// タイトルの高さを設定
		ctlwnd1->GetWindowRect(rect1);
		ctlwnd2->GetWindowRect(rect2);
		m_report.SetHeaderHeight(rect1.Height());
		m_report.SetItemHeight(rect2.Height());

		// セットした情報でコントロールを描画する
		m_report.Populate();
	}

	return(0);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	レポートデータ取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nType		…	参照型
//				nGrSeq		…　グループ番号
//				nSort		…	表示順
//				strFilterKana	…　ｶﾅ検索用フィルタ
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::GetReportData(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
								  CString strFilterKana1/*=NULL*/, CString strFilterKana2/*=NULL*/)
{
	long	nRet = 0;	// 戻値

	// 参照型で分岐
	switch(nType)	{
		case ID_DLGTYPE_BANK:		// 金融機関
			// 金融機関アイテム取得
			nRet = GetKinyuukikanItemRep(pDB, nSort, strFilterKana1, strFilterKana2);
			break;

		case ID_DLGTYPE_ADDRESS:	// 取引先
			// 取引先アイテム取得
			nRet = GetTorihikisakiItemRep(pDB, nGrSeq, nSort, strFilterKana1);
			break;

		case ID_DLGTYPE_KAMOKU:		// 科目
			// 科目アイテム取得
			nRet = GetKamokuItemRep(pDB, nFormSeq, nSort, strFilterKana1);
			break;
	}

	// 戻値で分岐
	switch(nRet)	{
		case 0:		// 0行
			m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F8, FALSE);			// [F8]挿入			×
			m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, FALSE);			// [F7]削除			×
			m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F6, FALSE);			// [F6]編集			×
			m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F11, FALSE);			// [F11]一行上へ	×
			m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F12, FALSE);			// [F12]一行下へ	×
			m_btnOK.EnableWindow(FALSE);													// OKボタン			×
			break;

		case 1:		// 1行
			// 番号順
			if(nSort == 0)	{
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F8, TRUE);		// [F8]挿入			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, TRUE);		// [F7]削除			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F6, TRUE);		// [F6]編集			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F11, FALSE);		// [F11]一行上へ	×
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F12, FALSE);		// [F12]一行下へ	×
			}
			// 50音順
			else	{
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F8, FALSE);		// [F8]挿入			×
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, TRUE);		// [F7]削除			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F6, TRUE);		// [F6]編集			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F11, FALSE);		// [F11]一行上へ	×
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F12, FALSE);		// [F12]一行下へ	×
			}
			m_btnOK.EnableWindow(TRUE);														// OKボタン			○
			break;

		default:	// 1行以上
			// 番号順
			if(nSort == 0)	{
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F8, TRUE);		// [F8]挿入			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, TRUE);		// [F7]削除			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F6, TRUE);		// [F6]編集			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F11, TRUE);		// [F11]一行上へ	○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F12, TRUE);		// [F12]一行下へ	○
			}
			// 50音順
			else	{
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F8, FALSE);		// [F8]挿入			×
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F7, TRUE);		// [F7]削除			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F6, TRUE);		// [F6]編集			○
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F11, FALSE);		// [F11]一行上へ	×
				m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F12, FALSE);		// [F12]一行下へ	×
			}
			m_btnOK.EnableWindow(TRUE);														// OKボタン			○
			break;
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関アイテム取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nSort			…	表示順
//				strFilter2　	…　ｶﾅ検索用フィルタ（金融機関名）
//				strFilter1　	…　ｶﾅ検索用フィルタ（支店名）
//	【戻値】	行数
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::GetKinyuukikanItemRep(CDatabase* pDB, int nSort, CString strFilter1, CString strFilter2)
{
	long					max = 0;
	long					nLine = 0;				// 行管理用
	long					kind = 0;
	char					szBuf[256];				// 文字列変換用バッファ
	BOOL					bContinue1 = FALSE;
	BOOL					bContinue2 = FALSE;
	CString					buff1 = _T("");
	CString					buff2 = _T("");
	CRect					rect,rect1,rect2;
	CWnd*					ctlwnd1 = m_pWnd->GetDlgItem(IDC_STC_DMY1);
	CWnd*					ctlwnd2 = m_pWnd->GetDlgItem(IDC_STC_DMY2);
	ICSReportRecord			*pRecord;
	ICSReportRecordItem		*pItem;
	CdbUcLstBank			mfcRec(pDB);			// uc_lst_bankテーブルクラス
	
	// レポートをクリア
	m_report.RemoveAllRecords();
	m_DataLineMax = 0;

	try	{
		// 初期化成功？
		if(mfcRec.Init_M(nSort) == DB_ERR_OK)	{
			// レコードあり？
			if( !( mfcRec.IsEOF() ) )	{
				mfcRec.MoveFirst();		// レコードを先頭に移動

				// レコード終端までループ
				while( !mfcRec.IsEOF() )	{
					bContinue1 = FALSE;
					bContinue2 = FALSE;
					// ｶﾅ検索が入力されている場合、フィルタリング
					if( !(strFilter1.IsEmpty()) )	{
						buff1 = mfcRec.m_BkKana1;
						buff1 = buff1.Left(strFilter1.GetLength());
						if(buff1.Compare(strFilter1) != 0)	{
							bContinue1 = TRUE;
						}
					}

					if( !(strFilter2.IsEmpty()) )	{
						buff2 = mfcRec.m_BkKana2;
						buff2 = buff2.Left(strFilter2.GetLength());
						if(buff2.Compare(strFilter2) != 0)	{
							bContinue2 = TRUE;
						}
					}

					if(bContinue1 != FALSE || bContinue2 != FALSE)	{
						mfcRec.MoveNext();
						continue;
					}
					
					pRecord = m_report.AddRecord(new ICSReportRecord());	//	新規行追加

					// シーケンス番号を設定（非表示項目）
					ZeroMemory(szBuf, sizeof(szBuf));
					_itoa_s(mfcRec.m_Seq, szBuf, 10);
					kind = ICS_EDS_NUMERIC;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));

					// 順序番号を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					_itoa_s(mfcRec.m_OrderNum, szBuf, 10);
					kind = ICS_EDS_NUMERIC;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));

					// 金融機関名を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_BkName1);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));

					// カナを設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_BkKana1);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));

					// 支店名を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_BkName2);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));

					// カナを設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_BkKana2);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));

					nLine++;			// 行をインクリメント
					mfcRec.MoveNext();	// 次のレコードに移動
				}
			}

			// レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...)	{
		// レコード開いてる？
		if(mfcRec.IsOpen())	{
			// レコード閉じる
			mfcRec.Fin();
		}
	}

	// セットした情報でレポートを描画する
	m_report.Populate();

	// データをセットした行数
	m_DataLineMax = nLine;

	// -----------------------------
	// 1画面に表示できる行数を取得する
	m_report.GetWindowRect(rect);		// レポートの高さ
	ctlwnd1->GetWindowRect(rect1);		// タイトル行の高さ
	ctlwnd2->GetWindowRect(rect2);		// データ行の高さ
	max = ((rect.Height() - rect1.Height()) / rect2.Height());
	if(max > nLine)	{
		for(int ii=nLine; ii<max; ii++)	{
			// 最大行数より少ない場合は、空行を追加する
			pRecord = m_report.AddRecord(new ICSReportRecord());	//	新規行追加
			ZeroMemory(szBuf, sizeof(szBuf));
			kind = ICS_EDS_NUMERIC | ICS_EDS_ALPHABET | ICS_EDS_KANA | ICS_EDS_ZENKAKU | ICS_EDS_ASCII;

			// シーケンス番号を設定（非表示項目）
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));
			// 順序番号を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));
			// 金融機関名を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));
			// カナを設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));
			// 支店名を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));
			// カナを設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));
		}
		// セットした情報でレポートを描画する
		m_report.Populate();
	}
	else	{
		max = nLine;
	}

	// -----------------------------
	// 全て編集不可
	m_report.AllowEdit(TRUE);
	for(int ii=0; ii<max; ii++)	{
		m_report.AllowEditItem(ii, 0, FALSE);	// シーケンス番号（非表示項目）
		m_report.AllowEditItem(ii, 1, FALSE);	// 順序番号
		m_report.AllowEditItem(ii, 2, FALSE);	// 金融機関名
		m_report.AllowEditItem(ii, 3, FALSE);	// カナ
		m_report.AllowEditItem(ii, 4, FALSE);	// 支店名
		m_report.AllowEditItem(ii, 5, FALSE);	// カナ
	}

	// 戻値を返す
	return(nLine);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先アイテム取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nGrSeq		…	グループ番号
//				nSort		…	表示順
//				strFilter　	…　ｶﾅ検索用フィルタ
//	【戻値】	行数
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::GetTorihikisakiItemRep(CDatabase* pDB, int nGrSeq, int nSort, CString strFilter)
{
	long					max;
	long					nLine = 0;			// 行管理用
	long					kind = 0;
	char					szBuf[256];			// 文字列変換用バッファ
	CString					buff = _T("");
	CRect					rect,rect1,rect2;
	CWnd*					ctlwnd1 = m_pWnd->GetDlgItem(IDC_STC_DMY1);
	CWnd*					ctlwnd2 = m_pWnd->GetDlgItem(IDC_STC_DMY2);
	ICSReportRecord			*pRecord;
	ICSReportRecordItem		*pItem;
	CdbUcLstAddress			mfcRec(pDB);		// uc_lst_addressテーブルクラス

	// レポートをクリア
	m_report.RemoveAllRecords();

	try	{
		// 初期化成功？
		if(mfcRec.Init_M(nGrSeq, nSort) == DB_ERR_OK)	{
			// レコードあり？
			if( !(mfcRec.IsEOF()) )	{
				mfcRec.MoveFirst();		// レコードを先頭に移動
				
				// レコード終端までループ
				while( !mfcRec.IsEOF() )	{
					// ｶﾅ検索が入力されている場合、フィルタリング
					if(!(strFilter.IsEmpty()))	{
						buff = mfcRec.m_AdKana;
						buff = buff.Left(strFilter.GetLength());
						if(buff.Compare(strFilter) != 0)	{
							mfcRec.MoveNext();
							continue;
						}
					}

					pRecord = m_report.AddRecord(new ICSReportRecord());	//	新規行追加

					// シーケンス番号を設定（非表示項目）
					ZeroMemory(szBuf, sizeof(szBuf));
					_itoa_s(mfcRec.m_AdSeq, szBuf, 10);
					kind = ICS_EDS_NUMERIC;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));

					// 順序番号を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					_itoa_s(mfcRec.m_OrderNum, szBuf, 10);
					kind = ICS_EDS_NUMERIC;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));
					
// インボイス登録番号追加対応_23/11/06 add -->
					// 登録番号(法人番号)を設定
					if(bG_InvNo == TRUE)	{
						ZeroMemory(szBuf, sizeof(szBuf));
						sprintf_s(szBuf, _T("%s"), mfcRec.m_InvNo);
						kind = ICS_EDS_NUMERIC | ICS_EDS_ALPHABET;
						pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 14, ICS_EDS_CENTER));
					}
// インボイス登録番号追加対応_23/11/06 add <--

					// 取引先名(上段)を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_AdName1);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 10, ICS_EDS_LEFT));

					// 取引先名(下段)を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_AdName2);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 10, ICS_EDS_LEFT));
					
					// カナを設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_AdKana);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));

					// 住所(上段)を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_AdAdd1);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));

					// 住所(下段)を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_AdAdd2);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));

					nLine++;			// 行をインクリメント
					mfcRec.MoveNext();	// 次のレコードに移動
				}	
			}
			// レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...)	{
		// レコード開いてる？
		if(mfcRec.IsOpen())	{
			// レコード閉じる
			mfcRec.Fin();
		}
	}

	// セットした情報でレポートを描画する
	m_report.Populate();

	// データをセットした行数
	m_DataLineMax = nLine;

	// -----------------------------
	// 1画面に表示できる行数を取得する
	m_report.GetWindowRect(rect);		// レポートの高さ
	ctlwnd1->GetWindowRect(rect1);		// タイトル行の高さ
	ctlwnd2->GetWindowRect(rect2);		// データ行の高さ
	max = ((rect.Height() - rect1.Height()) / rect2.Height()) - 1;	// ※横数クロールが表示されるため-1
	if(max > nLine)	{
		for(int ii=nLine; ii<max; ii++)	{
			// 最大行数より少ない場合は、空行を追加する
			pRecord = m_report.AddRecord(new ICSReportRecord());	//	新規行追加
			ZeroMemory(szBuf, sizeof(szBuf));
			kind = ICS_EDS_NUMERIC | ICS_EDS_ALPHABET | ICS_EDS_KANA | ICS_EDS_ZENKAKU | ICS_EDS_ASCII;

			// シーケンス番号を設定（非表示項目）
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));
			// 順序番号を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));
// インボイス登録番号追加対応_23/11/06 add -->
			// 登録番号(法人番号)を設定
			if(bG_InvNo == TRUE) {
				pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 14, ICS_EDS_CENTER));
			}
// インボイス登録番号追加対応_23/11/06 add <--
			// 取引先名(上段)を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 10, ICS_EDS_LEFT));
			// 取引先名(下段)を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 10, ICS_EDS_LEFT));
			// カナを設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));
			// 住所(上段)を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));
			// 住所(下段)を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 20, ICS_EDS_LEFT));
		}
		// セットした情報でレポートを描画する
		m_report.Populate();
	}
	else	{
		max = nLine;
	}

	// -----------------------------
	// 全て編集不可
	m_report.AllowEdit(TRUE);
	for(int ii=0; ii<max; ii++)	{
// インボイス登録番号追加対応_23/11/06 add -->
		if(bG_InvNo == FALSE) {
// インボイス登録番号追加対応_23/11/06 add <--
			m_report.AllowEditItem(ii, 0, FALSE);	// シーケンス番号（非表示項目）
			m_report.AllowEditItem(ii, 1, FALSE);	// 順序番号
			m_report.AllowEditItem(ii, 2, FALSE);	// 取引先名（上段）
			m_report.AllowEditItem(ii, 3, FALSE);	// 取引先名（下段）
			m_report.AllowEditItem(ii, 4, FALSE);	// カナ
			m_report.AllowEditItem(ii, 5, FALSE);	// 住所（上段）
			m_report.AllowEditItem(ii, 6, FALSE);	// 住所（下段）
// インボイス登録番号追加対応_23/11/06 add -->
		}
		else {
			m_report.AllowEditItem(ii, 0, FALSE);	// シーケンス番号（非表示項目）
			m_report.AllowEditItem(ii, 1, FALSE);	// 順序番号
			m_report.AllowEditItem(ii, 2, FALSE);	// 登録番号(法人番号)
			m_report.AllowEditItem(ii, 3, FALSE);	// 取引先名（上段）
			m_report.AllowEditItem(ii, 4, FALSE);	// 取引先名（下段）
			m_report.AllowEditItem(ii, 5, FALSE);	// カナ
			m_report.AllowEditItem(ii, 6, FALSE);	// 住所（上段）
			m_report.AllowEditItem(ii, 7, FALSE);	// 住所（下段）
		}
// インボイス登録番号追加対応_23/11/06 add <--
	}

	// 戻値を返す
	return(nLine);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	科目アイテム取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nSort		…	表示順
//				strFilter　	…　ｶﾅ検索用フィルタ
//	【戻値】	行数
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::GetKamokuItemRep(CDatabase* pDB, short nFormSeq, int nSort, CString strFilter)
{
	long					max=0;
	long					nLine = 0;			// 行管理用
	long					kind = 0;
	char					szBuf[256];			// 文字列変換用バッファ
	CString					buff = _T("");
	CRect					rect,rect1,rect2;
	CWnd*					ctlwnd1 = m_pWnd->GetDlgItem(IDC_STC_DMY1);
	CWnd*					ctlwnd2 = m_pWnd->GetDlgItem(IDC_STC_DMY2);
	ICSReportRecord			*pRecord;
	ICSReportRecordItem		*pItem;
	CdbUcLstKamoku			mfcRec(pDB);		// uc_lst_kamokuテーブルクラス

	// レポートをクリア
	m_report.RemoveAllRecords();

	try	{
		// 初期化成功？
		if(mfcRec.Init_M(nFormSeq, nSort) == DB_ERR_OK)	{
			// レコードあり？
			if( !(mfcRec.IsEOF()) )	{
				mfcRec.MoveFirst();		// レコードを先頭に移動
				
				// レコード終端までループ
				while( !mfcRec.IsEOF() )	{
					// ｶﾅ検索が入力されている場合、フィルタリング
					if( !(strFilter.IsEmpty()) )	{
						buff = mfcRec.m_KnKana;
						buff = buff.Left( strFilter.GetLength() );
						if( buff.Compare( strFilter ) != 0 )	{
							mfcRec.MoveNext();
							continue;
						}
					}

					pRecord = m_report.AddRecord(new ICSReportRecord());	//	新規行追加
				
					// 科目シーケンス番号を設定（非表示項目）
					ZeroMemory(szBuf, sizeof(szBuf));
					_itoa_s(mfcRec.m_KnSeq, szBuf, 10);
					kind = ICS_EDS_NUMERIC;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));

					// 順序番号を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					_itoa_s(mfcRec.m_KnOrder, szBuf, 10);
					kind = ICS_EDS_NUMERIC;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));

					// 科目名を設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_KnName);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 12, ICS_EDS_LEFT));

					// カナを設定
					ZeroMemory(szBuf, sizeof(szBuf));
					sprintf_s(szBuf,_T("%s"), mfcRec.m_KnKana);
					kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ASCII;
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));

					// デフォルトフラグを設定（非表示項目）
					ZeroMemory(szBuf, sizeof(szBuf));
					_itoa_s(mfcRec.m_FgDft, szBuf, 10);
					kind = ICS_EDS_NUMERIC;
					//pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 2, ICS_EDS_CENTER));
					pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 2));

					nLine++;			// 行をインクリメント
					mfcRec.MoveNext();	// 次のレコードに移動
				}	
			}
			// レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...)	{
		// レコード開いてる？
		if( mfcRec.IsOpen() )	{
			// レコード閉じる
			mfcRec.Fin();
		}
	}

	// セットした情報でレポートを描画する
	m_report.Populate();

	// データをセットした行数
	m_DataLineMax = nLine;

	// -----------------------------
	// 1画面に表示できる行数を取得する
	m_report.GetWindowRect(rect);	// レポートの高さ
	ctlwnd1->GetWindowRect(rect1);	// タイトル行の高さ
	ctlwnd2->GetWindowRect(rect2);	// データ行の高さ
	max = ((rect.Height() - rect1.Height()) / rect2.Height());
	if(max > nLine)	{
		for(int ii=nLine; ii<max; ii++)	{
			// 最大行数より少ない場合は、空行を追加する
			pRecord = m_report.AddRecord(new ICSReportRecord());	//	新規行追加
			ZeroMemory(szBuf, sizeof(szBuf));
			kind = ICS_EDS_NUMERIC | ICS_EDS_ALPHABET | ICS_EDS_KANA | ICS_EDS_ZENKAKU | ICS_EDS_ASCII;

			// 科目シーケンス番号を設定（非表示項目）
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));
			// 順序番号を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 4, ICS_EDS_CENTER));
			// 科目名を設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 12, ICS_EDS_LEFT));
			// カナを設定
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 6, ICS_EDS_LEFT));
			// デフォルトフラグを設定（非表示項目）
			wsprintf(szBuf, _T("%s"), _T("0"));
			pItem = pRecord->AddItem(new ICSReportRecordItemText(szBuf, kind, FALSE, FALSE, 2, ICS_EDS_CENTER));
		}
		// セットした情報でレポートを描画する
		m_report.Populate();
	}
	else	{
		max = nLine;
	}

	// -----------------------------
	// 全て編集不可
	m_report.AllowEdit(TRUE);
	for(int ii=0; ii<max; ii++)	{
		m_report.AllowEditItem(ii, 0, FALSE);	// シーケンス番号（非表示項目）
		m_report.AllowEditItem(ii, 1, FALSE);	// 順序番号
		m_report.AllowEditItem(ii, 2, FALSE);	// 科目名
		m_report.AllowEditItem(ii, 3, FALSE);	// カナ
		//m_report.AllowEditItem(ii, 4, FALSE);	// デフォルトフラグ（非表示項目）	// 修正No.162345 del

		buff = m_report.GetCaption(ii, 4);
		if(_tstoi(buff) == 1)	{
			// デフォルト科目の背景色
			m_report.SetRowBackColor(ii, RGB(0xA6, 0XCA, 0xF0), TRUE);
			m_report.SetRowTextColor(ii, RGB(0x00, 0x00, 0x00), TRUE);
		}
		else	{
			// 通常行の背景色と文字色
			m_report.SetRowBackColor(ii, GetSysColor(COLOR_WINDOW), TRUE);
			m_report.SetRowTextColor(ii, GetSysColor(COLOR_WINDOWTEXT), TRUE);
		}
	}

// 修正No.162345 del -->
	//// セットした情報でレポートを描画する
	//m_report.Populate();
// 修正No.162345 del <--

	// 戻値を返す
	return(nLine);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	リストを除くコントロールの初期化
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	nType		…	参照型
//				nGrSeq		…	グループ番号
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::InitOtherControls( EnumIdDlgType nType, int nGrSeq)
{
	LPSTR				szTitle;		// タイトル

	// タイトル設定
	switch(nType)	{
		case ID_DLGTYPE_BANK:
			szTitle = "金融機関";
			break;
		case ID_DLGTYPE_ADDRESS:
			szTitle = "取引先";
			break;
		case ID_DLGTYPE_KAMOKU:
// 改良No.23-0513 cor -->
			//szTitle = "科目";
// ----------------------
			if(m_nFormSeq == ID_FORMNO_011 || m_nFormSeq == ID_FORMNO_102)	{
				szTitle = "種類";
			}
			else if(m_nFormSeq == ID_FORMNO_061 || m_nFormSeq == ID_FORMNO_121 || m_nFormSeq == ID_FORMNO_151)	{
				szTitle = "区分";
			}
			else	{
				szTitle = "科目";
			}
// 改良No.23-0513 cor <--
			break;
		default:
			szTitle = "";
			break;
	}
	m_lblEntry.SetWindowText(szTitle);

// 改良No.22-0404,22-0812_230414 add -->
	// ガイドの表示設定
	if(nType == ID_DLGTYPE_KAMOKU)	m_guide.ShowWindow(SW_HIDE);
	else							m_guide.ShowWindow(SW_SHOW);
// 改良No.22-0404,22-0812_230414 add <--

// 修正No.162490 cor -->
	//// グループ番号
	//switch(nGrSeq)	{
	//	case ID_ADDRESSGR_URI:	// （売）
	//		m_cmbTorihikiChange.SetCurSel(1);	// コンボボックスに得意先をセット
	//		break;

	//	case ID_ADDRESSGR_KAI:	// （買）
	//		m_cmbTorihikiChange.SetCurSel(0);	// コンボボックスに仕入先をセット
	//		break;
	//}

	//// 参照型が取引先以外は表示しない
	//if(nType == ID_DLGTYPE_ADDRESS)	{
	//	m_cmbTorihikiChange.ShowWindow(SW_SHOW);
	//}
	//else	{
	//	m_cmbTorihikiChange.ShowWindow(SW_HIDE);
	//}
// ---------------------
	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体

	// グループ番号
	m_clsFunc.DiagInit(&udData);
	m_DiagKanaSearch.GetData(ID_DIAG_INDEX_INPUT_KUBUN, (LPUNKNOWN)&udData);

	if(nGrSeq == ID_ADDRESSGR_KAI)	{		// （買）
		udData.data_combo = 0;	// コンボボックスに仕入先をセット
	}
	else if(nGrSeq == ID_ADDRESSGR_URI)	{	// （売）
		udData.data_combo = 1;	// コンボボックスに得意先をセット
	}
	m_DiagKanaSearch.SetData(ID_DIAG_INDEX_INPUT_KUBUN, (LPUNKNOWN)&udData);

	// 参照型が取引先以外は表示しない
	if(nType == ID_DLGTYPE_ADDRESS)	{
		m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_INPUT_KUBUN, DIAG_MDFY_OPAQUE);
	}
	else	{
		m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_INPUT_KUBUN, DIAG_MDFY_TRANSPARENT);
	}
// 修正No.162490 cor <--

	// カナ検索ダイアグラム初期化
	SetInitKanaSearchCtrl(nType);

	// 「取引先、金融機関を自動で登録する」チェックボックス初期化
	if(nType == ID_DLGTYPE_BANK || nType == ID_DLGTYPE_BANK_REFERENCE || nType == ID_DLGTYPE_ADDRESS || nType == ID_DLGTYPE_ADDRESS_REFERENCE)	{
		BOOL	state = BST_UNCHECKED;
		if(m_isRegAuto)	state = BST_CHECKED;
		m_chkRegAuto.SetCheck(state);
		m_chkRegAuto.EnableWindow(TRUE);
		m_chkRegAuto.ShowWindow(SW_SHOW);
	}
	else	{
		m_chkRegAuto.EnableWindow(FALSE);
		m_chkRegAuto.ShowWindow(SW_HIDE);
	}

	return(0);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	カナ検索ダイアグラムを参照型によって文言の変更、表示／非表示を切り替える
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	参照型
//	【戻値】	TRUE/FALSE
// -----------------------------------------------------------------------------------------------------------------------------------
BOOL CdlgReference::SetInitKanaSearchCtrl(EnumIdDlgType nType)
{
	int					nRet = FALSE;
	CString				cst=_T("");	// 改良No.23-0513 add
	DIAGRAM_DATA		udData;		// DIAGRAM_DATA構造体
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体
	ICSColorInfo		icsColInfo;

	// 背景色取得
	m_ViewColor = GetSysColor(COLOR_BTNFACE);
	if(((ICSWinApp *)AfxGetApp())->GetStanderdColor(icsColInfo) == 0)	{
		if(icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON)	{
			m_ViewColor = icsColInfo.ViewColor();
		}
	}
	// 背景色セット
	m_DiagKanaSearch.SetBackColor(m_ViewColor);

	// 文字列の背景セット
	m_DiagKanaSearch.GetAttribute(ID_DIAG_INDEX_LABEL_SORT, (LPUNKNOWN)&diaatt);
	diaatt.attr_bcolor = m_ViewColor;
	m_DiagKanaSearch.SetAttribute(ID_DIAG_INDEX_LABEL_SORT, (LPUNKNOWN)&diaatt, TRUE);

	m_DiagKanaSearch.GetAttribute(ID_DIAG_INDEX_LABEL_MAIN, (LPUNKNOWN)&diaatt);
	diaatt.attr_bcolor = m_ViewColor;
	m_DiagKanaSearch.SetAttribute(ID_DIAG_INDEX_LABEL_MAIN, (LPUNKNOWN)&diaatt, TRUE);

	m_DiagKanaSearch.GetAttribute(ID_DIAG_INDEX_LABEL_SUB, (LPUNKNOWN)&diaatt);
	diaatt.attr_bcolor = m_ViewColor;
	m_DiagKanaSearch.SetAttribute(ID_DIAG_INDEX_LABEL_SUB, (LPUNKNOWN)&diaatt, TRUE);

	// 表示順の設定
	m_clsFunc.DiagInit(&udData);
	m_DiagKanaSearch.GetData(ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData);
	udData.data_combo = m_nSort[m_nSortNum];
	m_DiagKanaSearch.SetData(ID_DIAG_INDEX_INPUT_SORT, (LPUNKNOWN)&udData);

// 修正No.162490 cor -->
	//// 文字列セット＆表示/非表示切替
	//switch(nType)	{
	//	case ID_DLGTYPE_BANK:
	//	case ID_DLGTYPE_BANK_REFERENCE:
	//		m_clsFunc.DiagSetString(&m_DiagKanaSearch, 2, ID_DIAGDATA_DISP, _T("金融機関ｶﾅ検索"));
	//		m_clsFunc.DiagSetString(&m_DiagKanaSearch, 4, ID_DIAGDATA_DISP, _T("支店名ｶﾅ検索"));

	//		m_DiagKanaSearch.ModifyItem(4, DIAG_MDFY_OPAQUE);
	//		m_DiagKanaSearch.ModifyItem(5, DIAG_MDFY_OPAQUE);
	//		break;

	//	case ID_DLGTYPE_ADDRESS:
	//	case ID_DLGTYPE_ADDRESS_REFERENCE:
	//		m_clsFunc.DiagSetString(&m_DiagKanaSearch, 2, ID_DIAGDATA_DISP, _T("取引先ｶﾅ検索"));

	//		m_DiagKanaSearch.ModifyItem(4, DIAG_MDFY_TRANSPARENT);
	//		m_DiagKanaSearch.ModifyItem(5, DIAG_MDFY_TRANSPARENT);
	//		break;

	//	case ID_DLGTYPE_KAMOKU:
	//		m_clsFunc.DiagSetString(&m_DiagKanaSearch, 2, ID_DIAGDATA_DISP, _T("科目名ｶﾅ検索"));

	//		m_DiagKanaSearch.ModifyItem(4, DIAG_MDFY_TRANSPARENT);
	//		m_DiagKanaSearch.ModifyItem(5, DIAG_MDFY_TRANSPARENT);
	//		break;
	//}
// ---------------------
	// 文字列セット＆表示/非表示切替
	switch(nType)	{
		case ID_DLGTYPE_BANK:
		case ID_DLGTYPE_BANK_REFERENCE:
			m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_LABEL_MAIN, ID_DIAGDATA_DISP, _T("金融機関ｶﾅ検索"));
			m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_LABEL_SUB, ID_DIAGDATA_DISP, _T("支店名ｶﾅ検索"));

			m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_LABEL_SUB, DIAG_MDFY_OPAQUE);
			m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_INPUT_SUB, DIAG_MDFY_OPAQUE);
			break;

		case ID_DLGTYPE_ADDRESS:
		case ID_DLGTYPE_ADDRESS_REFERENCE:
			m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_LABEL_MAIN, ID_DIAGDATA_DISP, _T("取引先ｶﾅ検索"));

			m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_LABEL_SUB, DIAG_MDFY_TRANSPARENT);
			m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_INPUT_SUB, DIAG_MDFY_TRANSPARENT);
			break;

		case ID_DLGTYPE_KAMOKU:
// 改良No.23-0513 cor -->
			//m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_LABEL_MAIN, ID_DIAGDATA_DISP, _T("科目名ｶﾅ検索"));
// ----------------------
			cst.Empty();
			if(m_nFormSeq == ID_FORMNO_011 || m_nFormSeq == ID_FORMNO_102)	{
				cst = _T("種類名ｶﾅ検索");
			}
			else if(m_nFormSeq == ID_FORMNO_061 || m_nFormSeq == ID_FORMNO_121 || m_nFormSeq == ID_FORMNO_151)	{
				cst = _T("区分名ｶﾅ検索");
			}
			else	{
				cst = _T("科目名ｶﾅ検索");
			}
			m_clsFunc.DiagSetString(&m_DiagKanaSearch, ID_DIAG_INDEX_LABEL_MAIN, ID_DIAGDATA_DISP, cst);
// 改良No.23-0513 cor <--

			m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_LABEL_SUB, DIAG_MDFY_TRANSPARENT);
			m_DiagKanaSearch.ModifyItem(ID_DIAG_INDEX_INPUT_SUB, DIAG_MDFY_TRANSPARENT);
			break;
	}
// 修正No.162490 cor <--

	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	初期位置移動
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nType			…	参照型
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
//				nItemSeq		…	帳表で選択されている項目のシーケンス番号
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::InitMovePosition(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, long nItemSeq)
{
	_RD_BANK	udBank;				// 金融機関	
	_RD_ADDRESS	udAddress;			// 取引先
	_RD_KAMOKU	udKamoku;			// 科目
	long		nPos = 0;			// 位置
	int			cnt=0,max=0;
	CString		udData;				// データ

	// 参照型で分岐
	switch( nType )	{
		// 金融機関
		case ID_DLGTYPE_BANK:
			// 金融機関のレコード取得
			if( GetKinnyuukikanRecord( pDB, nItemSeq, &udBank ) == 0 )	{
				// 順序番号取得
				nPos = udBank.m_OrderNum;
			}
			break;

		// 取引先
		case ID_DLGTYPE_ADDRESS:
			// 取引先のレコード取得
			if( GetTorihikisakiRecord( pDB, nGrSeq, nItemSeq, &udAddress ) == 0 )	{
				// 順序番号取得
				nPos = udAddress.m_OrderNum;
			}
			break;

		// 科目
		case ID_DLGTYPE_KAMOKU:
			// 科目のレコード取得
			if( GetKamokuRecord( pDB, nFormSeq, nItemSeq, &udKamoku ) == 0 )	{
				// 順序番号取得
				nPos = udKamoku.m_KnOrder;
			}
			break;
	}
	
	// 50音順の時はインデックス番号を再取得
	if(nSort == 1)	{
		max = m_DataLineMax;
		for(cnt=0; cnt<max; cnt++)	{
			udData = m_report.GetCaption(cnt, 1);
			if(nPos == _tstoi(udData))	{
				nPos = cnt + 1;
				break;
			}
		}
	}

	// 位置移動
	MovePosition(0, nPos - 1);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	位置移動
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	nTop	…	トップインデックス（※4/28未使用）
//				nIndex	…	賞味のインデックス
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::MovePosition(long nTop, long nIndex)
{
	// インデックスが範囲外？
	if( (nIndex < 0) || (nIndex > (m_DataLineMax - 1)) )	{
		// 先頭にする
		nIndex = 0;
	}

	if(m_DataLineMax <= 0)	{
		m_btnCancel.SetFocus();
	}
	else	{
		// カーソル移動
		m_report.SetFocus();
		m_report.SetFocusedRow((int)nIndex);
	}

	// 現在行の確認
	CheckNowRowPosition();	// 修正_23/04/21 add
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	位置移動（Order番号から検索）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	nOrder	…	順序番号
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::MovePosition2( int nOrder )
{
	int			ii=0;			// ループカウンタ
	int			seq=0;			// 順序番号
	int			row=0;			// カーソルセットポジション
	long		max=0;			// 最大行数
	CString		udData;			// リストデータ構造体
	
	max = m_DataLineMax;

	row = 0;
	for(ii=0; ii<max; ii++)	{
		// 指定カラムのデータを取得
		udData = m_report.GetCaption(ii, 1); 

		// シーケンス番号を数値に変換
		seq = _tstoi(udData);
		if(seq == nOrder)	{
			row = ii;
		}
	}

	// カーソル移動
	m_report.SetFocus();
	m_report.SetFocusedRow((int)row);

	// 現在行の確認
	CheckNowRowPosition();	// 修正_23/04/21 add
}

// 修正_23/04/21 add -->
// -----------------------------------------------------------------------------------------------------------------------------------
//	現在行の確認
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::CheckNowRowPosition(long pIndx/*=-1*/)
{
	long	nIndex;		//	インデックス
	long	nMax=0;			// 最大行数
	BOOL	bFlag[2];	//	フラグ

	// 賞味のインデックス取得
	if(pIndx == -1)	nIndex = GetListIndex();
	else			nIndex = pIndx;

	// 最大行数
	nMax = m_DataLineMax;

	// フラグ初期化
	bFlag[0] = TRUE;
	bFlag[1] = TRUE;

	//	リスト最大登録数が2行以下？
	if(nMax < 2)	{
		bFlag[0] = FALSE;
		bFlag[1] = FALSE;
	}
	//	インデックスが先頭？
	else if(nIndex == 0)	{
		bFlag[0] = FALSE;
	}
	//	インデックスが最終行？
	else if(nIndex == (nMax - 1))	{
		bFlag[1] = FALSE;
	}

	// 表示順が50音の場合は常にFALSE
	if(m_nSort[m_nSortNum] == 1)	{
		bFlag[0] = FALSE;
		bFlag[1] = FALSE;
	}

	//	[F5][F6]キーの有効／無効を設定
	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F11, bFlag[0]);	// [F11]一行上へ
	m_tbrToolBar.GetToolBarCtrl().EnableButton(ID_TOOL_BUTTON_F12, bFlag[1]);	// [F12]一行下へ

	// 前回選択行取得
	m_nOldRow = nIndex;
}
// 修正_23/04/21 add <--

// -----------------------------------------------------------------------------------------------------------------------------------
//	賞味のリストインデックス取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【戻値】	賞味のインデックス
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::GetListIndex(void)
{
	long	nRet = 0;		// 戻値
	long	nIndex = 0;		// 頁内でのインデックス
	long	nTop = 0;		// 頁のトップインデックス
	
	ICSReportRow	*pRow = NULL;

	// 頁内でのインデックス取得
	pRow = m_report.GetFocusedRow();
	if(pRow != NULL)	{
		nIndex = pRow->GetIndex();
	}
	else	{
		nIndex = 0;
	}
	// 賞味のインデックス取得
	nRet = nTop + nIndex;

	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	最大番号を持っているリストインデックスを取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				pType		…	参照型
//				pGrSeq		…	グループ番号
//				pFormSeq	…	様式シーケンス番号
//				pSort		…	表示順
//	【戻値】	賞味のインデックス
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::GetListIndex2( CDatabase* pDB, int pType, int pGrSeq, short pFormSeq, int pSort )
{
	int					cnt=0,max=0;
	long				nRet=0;				// 戻値
	long				nOrderNum=0;		// 順序番号
	long				nOrderNumSv=0;		// 順序番号セーブ
	long				nSeq = 0;			// シーケンス番号
	_RD_BANK			udBank;				// 金融機関構造体
	_RD_ADDRESS			udAddress;			// 取引先構造体
	_RD_KAMOKU			udKamoku;			// 科目構造体

	if(pSort == 0)	{
		nRet = m_DataLineMax - 1;
	}
	else	{
		max = m_DataLineMax;

		nOrderNumSv = -1;
		for(cnt=0; cnt<max; cnt++)	{
			// シーケンス番号取得
			nSeq = GetSeq(cnt);
			if(pType == ID_DLGTYPE_BANK)	{			// 金融機関
				// レコード取得
				GetKinnyuukikanRecord(pDB, nSeq, &udBank);
				// 順序番号を取得
				nOrderNum = udBank.m_OrderNum;
			}
			else if(pType == ID_DLGTYPE_ADDRESS)	{	// 取引先
				// レコード取得
				GetTorihikisakiRecord(pDB, pGrSeq, nSeq, &udAddress);
				// 順序番号を取得
				nOrderNum = udAddress.m_OrderNum;
			}
			else if(pType == ID_DLGTYPE_KAMOKU)		{	// 科目
				// レコード取得
				GetKamokuRecord(pDB, pFormSeq, nSeq, &udKamoku);
				// 順序番号を取得
				nOrderNum = udKamoku.m_KnOrder;
			}
			
			if(nOrderNumSv < nOrderNum)	{
				nRet = cnt;
				nOrderNumSv = nOrderNum;
			}
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	リストからシーケンス番号取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	nIndex	…	リストインデックス
//	【戻値】	0以外	…	シーケンス番号
//				0		…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::GetSeq(int nIndex)
{
	long		nRet = 0;			// 戻値
	CString		udData;				//

	// 指定カラムのデータを取得
	udData = m_report.GetCaption(nIndex, 0);
	// シーケンス番号を数値に変換
	nRet = _tstoi(udData);
	
	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	レコードの編集
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
//				fSearch			…	銀行検索フラグ（TRUE：銀行検索する／FALSE：自分で編集する）
//	【戻値】	0				…	編集なし
//				0以外			…	編集あり
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::SetRecordEdit(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, BOOL fSearch /*=FALSE*/)
{
	long					nRet = 0;		// 戻値
	long					nTopIndex = 0;	// トップインデックス
// 改良No.22-0404,22-0812_230414 add -->
	int						nIndex;
	ICSReportRow*			pRow;
	ICSReportSelectedRows*	pSelRows;

	// 複数行が選択されていれば、単一行選択に戻す
	pSelRows = m_report.GetSelectedRows();
	if(pSelRows->GetCount() > 1)	{
		pRow = m_report.GetFocusedRow();
		nIndex = pRow->GetIndex();
		m_report.SetUnSelectedRows();
		m_report.SetSelectedRows(nIndex, nIndex);
	}
// 改良No.22-0404,22-0812_230414 add <--

	// 参照型で分岐
	switch(nType)	{
		case ID_DLGTYPE_BANK:		// 金融機関
			// 金融機関アイテム編集
			nRet = EditKinyuukikan(pDB, fSearch);
			break;

		case ID_DLGTYPE_ADDRESS:	// 取引先
			// 取引先アイテム編集
			nRet = EditTorihikisaki(pDB, nGrSeq, fSearch);
			break;

		case ID_DLGTYPE_KAMOKU:		// 科目
			// 科目アイテム編集
			nRet = EditKamoku(pDB, nFormSeq);
			break;
	}
	
	// 編集された？
	if(nRet != 0)	{
		// 前回選択行をクリア
		m_nOldRow = -1;
		// リストのトップ
		nTopIndex = m_report.GetTopRowIndex();
		// リストの更新
		GetReportData(pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
					  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
		// 位置移動
		MovePosition(nTopIndex, (nRet - 1));
	}
	
	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関のレコード編集
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				fSearch			…	銀行検索フラグ（TRUE：銀行検索する／FALSE：自分で編集する）
//	【戻値】	0				…	編集なし
//				0以外			…	編集した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::EditKinyuukikan(CDatabase* pDB, BOOL fSearch /*=FALSE*/)
{
	long					nRet = 0;			// 戻値
	long					nIndex = 0;			// リストインデックス
	CdlgReferenceSub		dlgSub(this);		// 参照サブダイアログクラス
	CdbUcLstBank			mfcRec(pDB);		// uc_lst_bankテーブルクラス
	long					nSeq = 0;			// シーケンス番号
	_RD_BANK				udBank;				// 金融機関構造体
	int						nEdit = 1;			// 編集フラグ（0：編集した／0以外：未編集）
	ICSBank					clsBank;			// 銀行検索クラス
	UCHAR					cBkCode[2];			// 銀行コード
	UCHAR					cBcCode[2];			// 支店コード
	CString					szBuf[2];			// バッファ

	// リストのインデックス取得
	nIndex = GetListIndex();
	// シーケンス番号を取得
	nSeq = GetSeq(nIndex);
	
	// 金融機関のレコード取得失敗？
	if(GetKinnyuukikanRecord(pDB, nSeq, &udBank) != 0)	{
		// 失敗
		return(nRet);
	}
	
	// 自分で編集する？
	if(fSearch == FALSE)	{
		// 変更あり？
// インボイス登録番号追加対応_23/11/06 del -->
		//if(dlgSub.ShowDialog(pDB, ID_DLGTYPE_BANK, ID_DLGREF_UPDATE, 
		//					 "[F6]編集", udBank.m_OrderNum, 
		//					 udBank.m_BkName1, udBank.m_BkName2, "", "", udBank.m_BkKana1, udBank.m_BkKana2, 0, 0, nSeq) == ID_DLG_OK)	{
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
		if(dlgSub.ShowDialog(pDB, ID_DLGTYPE_BANK, ID_DLGREF_UPDATE, 
							 "[F6]編集", udBank.m_OrderNum, 
							 udBank.m_BkName1, udBank.m_BkName2, "", "", udBank.m_BkKana1, udBank.m_BkKana2, "", 0, 0, nSeq) == ID_DLG_OK)	{
// インボイス登録番号追加対応_23/11/06 add <--
			// 戻値を構造体に反映
			udBank.m_BkName1 = dlgSub.m_RetName1;
			udBank.m_BkName2 = dlgSub.m_RetName2;
			udBank.m_BkKana1 = dlgSub.m_RetKana1;
			udBank.m_BkKana2 = dlgSub.m_RetKana2;
			// 編集した
			nEdit = 0;
		}
	}
	// 銀行検索する
	else	{
		// 初期化
		memset(cBkCode, 0xFF, sizeof(cBkCode));
		memset(cBcCode, 0xFF, sizeof(cBcCode));
		szBuf[0].Empty();
		szBuf[1].Empty();
		
		// 銀行検索してOKボタンが押された？
		if(clsBank.BankSelect(cBkCode, cBcCode, &szBuf[0], &szBuf[1], 0, this) == TRUE)	{
			// 戻値を構造体に反映
			udBank.m_BkName1 = szBuf[0];
			udBank.m_BkName2 = szBuf[1];
			// 編集した
			nEdit = 0;
		}
	}
	
	// 編集した？
	if(nEdit == 0)	{
		try	{
			// 初期化成功？
			if(mfcRec.RequerySeq(nSeq) == DB_ERR_OK)	{
				// レコードあり？
				if( !(mfcRec.IsEOF()) )	{
					mfcRec.MoveFirst();						// レコードを先頭に移動
					mfcRec.Edit();							// レコード編集開始
					mfcRec.m_BkName1 = udBank.m_BkName1;	// 金融機関名変更
					mfcRec.m_BkName2 = udBank.m_BkName2;	// 支店名変更
					mfcRec.m_BkKana1 = udBank.m_BkKana1;	// 金融機関ｶﾅ変更
					mfcRec.m_BkKana2 = udBank.m_BkKana2;	// 支店名ｶﾅ変更
					mfcRec.Update();						// レコード更新
					nRet = nIndex + 1;						// 戻値を順序番号に設定
				}
				// レコード閉じる
				mfcRec.Fin();
			}
		}
		catch(...)	{
			// レコード開いている？
			if(mfcRec.IsOpen())	{
				// レコード閉じる
				mfcRec.Fin();	
			}
		}
	}
	
	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先のレコード編集
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
//				fSearch			…
//	【戻値】	0				…	編集なし
//				0以外			…	編集した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::EditTorihikisaki(CDatabase* pDB, int nGrSeq, BOOL fSearch /*=FALSE*/)
{
	long					nRet = 0;			// 戻値
	long					nIndex = 0;			// リストインデックス
	CdlgReferenceSub		dlgSub(this);		// 参照サブダイアログクラス
	CdbUcLstAddress			mfcRec(pDB);		// uc_lst_addressテーブルクラス
	long					nSeq = 0;			// シーケンス番号
	_RD_ADDRESS				udAddress;			// 取引先構造体
	int						nEdit = 1;			// 編集フラグ（0：編集した／0以外：未編集）
	CString					szBuf[2];			// バッファ
	CdlgAddressSearch		clsAdd(this);		// 住所検索クラス
	
	// リストのインデックス取得
	nIndex = GetListIndex();
	// シーケンス番号を取得
	nSeq = GetSeq(nIndex);

	// 取引先のレコード取得失敗？
	if(GetTorihikisakiRecord(pDB, nGrSeq, nSeq, &udAddress) != 0)	{
		// 失敗
		return(nRet);
	}
	
	// 自分で編集する？
	if( fSearch == FALSE )	{
		// 変更あり？
// インボイス登録番号追加対応_23/11/06 del -->
		//if( dlgSub.ShowDialog( pDB, ID_DLGTYPE_ADDRESS, ID_DLGREF_UPDATE, 
		//						"[F6]編集", udAddress.m_OrderNum, 
		//						udAddress.m_AdName1, udAddress.m_AdName2, 
		//						udAddress.m_AdAdd1, udAddress.m_AdAdd2, udAddress.m_AdKana, "", 0, nGrSeq, nSeq) == ID_DLG_OK)	{
		//	// 戻値を構造体に反映
		//	udAddress.m_AdName1 = dlgSub.m_RetName1;
		//	udAddress.m_AdName2 = dlgSub.m_RetName2;
		//	udAddress.m_AdAdd1 = dlgSub.m_RetAdd1;
		//	udAddress.m_AdAdd2 = dlgSub.m_RetAdd2;
		//	udAddress.m_AdKana = dlgSub.m_RetKana1;
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->	
		if( dlgSub.ShowDialog( pDB, ID_DLGTYPE_ADDRESS, ID_DLGREF_UPDATE, 
								"[F6]編集", udAddress.m_OrderNum, 
								udAddress.m_AdName1, udAddress.m_AdName2, 
								udAddress.m_AdAdd1, udAddress.m_AdAdd2, udAddress.m_AdKana, "", udAddress.m_InvNo, 0, nGrSeq, nSeq) == ID_DLG_OK)	{
			// 戻値を構造体に反映
			udAddress.m_AdName1 = dlgSub.m_RetName1;
			udAddress.m_AdName2 = dlgSub.m_RetName2;
			udAddress.m_AdAdd1 = dlgSub.m_RetAdd1;
			udAddress.m_AdAdd2 = dlgSub.m_RetAdd2;
			udAddress.m_AdKana = dlgSub.m_RetKana1;
			udAddress.m_InvNo = dlgSub.m_RetInvno;
// インボイス登録番号追加対応_23/11/06 add <--
			// 編集した
			nEdit = 0;
		}
	}
	// 住所検索あり
	else	{
		szBuf[0].Empty();
		szBuf[1].Empty();
		
		// 住所検索ダイアログでOKが押された？
		if( clsAdd.ShowDialog( 40, 40 ) == ID_DLG_OK )	{
			// 戻値を構造体に反映
			udAddress.m_AdAdd1 = clsAdd.m_AdAdd1;
			udAddress.m_AdAdd2 = clsAdd.m_AdAdd2;
			// 編集した
			nEdit = 0;
		}
	}
	
	// 編集した？
	if( nEdit == 0 )	{
		try	{
			// 初期化成功？
			if( mfcRec.RequeryAdSeq( nGrSeq, nSeq ) == DB_ERR_OK )	{
				// レコードあり？
				if( !( mfcRec.IsEOF() ) )	{
					mfcRec.MoveFirst();						// レコードを先頭に移動
					mfcRec.Edit();							// レコード編集開始
					mfcRec.m_AdName1 = udAddress.m_AdName1;	// 名称変更
					mfcRec.m_AdName2 = udAddress.m_AdName2;	// 名称変更
					mfcRec.m_AdAdd1 = udAddress.m_AdAdd1;	// 所在地変更
					mfcRec.m_AdAdd2 = udAddress.m_AdAdd2;	// 所在地変更
					mfcRec.m_AdKana = udAddress.m_AdKana;	// ｶﾅ変更
					mfcRec.m_InvNo = udAddress.m_InvNo;		// 登録番号変更		インボイス登録番号追加対応_23/11/06 add
					mfcRec.Update();						// レコード更新
					nRet = nIndex + 1;						// 戻値を順序番号に設定
				}
				// レコード閉じる
				mfcRec.Fin();
			}
		}
		catch(...)	{
			// レコード開いている？
			if( mfcRec.IsOpen() )	{
				// レコード閉じる
				mfcRec.Fin();	
			}
		}
	}
	
	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	科目のレコード編集
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	編集なし
//				0以外		…	編集した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::EditKamoku( CDatabase* pDB, short nFormSeq )
{
	long					nRet = 0;			// 戻値
	long					nIndex = 0;			// リストインデックス
	CdlgReferenceSub		dlgSub( this );		// 参照サブダイアログクラス
	CdbUcLstKamoku			mfcRec( pDB );		// uc_lst_kamokuテーブルクラス
	long					nKnSeq = 0;			// シーケンス番号
	_RD_KAMOKU				udKamoku;			// 科目構造体
	int						nInputLen;			// 入力文字数

	// リストのインデックス取得
	nIndex = GetListIndex();
	// シーケンス番号を取得
	nKnSeq = GetSeq(nIndex);
	
	// 科目のレコード取得失敗？
	if( GetKamokuRecord( pDB, nFormSeq, nKnSeq, &udKamoku ) != 0 )	{
		// 失敗
		return( nRet );
	}
	
	switch( nFormSeq )	{
	// ⑩-2.源泉所得税預り金の内訳
	case ID_FORMNO_102:
		nInputLen = DR_INPUTLENGTH_GENSEN;
		break;
	// ⑫.土地の売上高等の内訳書
	case ID_FORMNO_121:
		nInputLen = DR_INPUTLENGTH_TOTIURI;
		break;
	// ⑮-1.地代家賃の内訳書
	case ID_FORMNO_151:
		nInputLen = DR_INPUTLENGTH_TIDAI;
		break;
	default:
		// 入力文字数を８文字に設定
		nInputLen = DR_INPUTLENGTH_NORMAL;
		break;
	}	

	// 変更あり？
// インボイス登録番号追加対応_23/11/06 del -->
	//if( dlgSub.ShowDialog( pDB, ID_DLGTYPE_KAMOKU, ID_DLGREF_UPDATE, 
	//						"[F6]編集", udKamoku.m_KnOrder, 
	//						udKamoku.m_KnName, "", "", "" , udKamoku.m_KnKana, "", nInputLen, nFormSeq, nKnSeq) == ID_DLG_OK)	{
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
	if( dlgSub.ShowDialog( pDB, ID_DLGTYPE_KAMOKU, ID_DLGREF_UPDATE, 
							"[F6]編集", udKamoku.m_KnOrder, 
							udKamoku.m_KnName, "", "", "" , udKamoku.m_KnKana, "", "", nInputLen, nFormSeq, nKnSeq) == ID_DLG_OK)	{
// インボイス登録番号追加対応_23/11/06 add <--
		try	{
			// 初期化成功？
			if( mfcRec.RequeryKnSeq( nFormSeq, nKnSeq ) == DB_ERR_OK )	{
				// レコードあり？
				if( !( mfcRec.IsEOF() ) )	{
					mfcRec.MoveFirst();						// レコードを先頭に移動
					mfcRec.Edit();							// レコード編集開始
					mfcRec.m_KnName = dlgSub.m_RetName1;	// 科目名変更
					mfcRec.m_KnKana = dlgSub.m_RetKana1;	// 科目ｶﾅ変更
					mfcRec.Update();						// レコード更新
					nRet = nIndex + 1;						// 戻値を順序番号に設定
				}
				// レコード閉じる
				mfcRec.Fin();
			}
		}
		catch(...)	{
			// レコード開いている？
			if( mfcRec.IsOpen() )	{
				// レコード閉じる
				mfcRec.Fin();	
			}
		}
	}

	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関を削除する前に、削除可能かどうかチェック
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				pDelseq		…	削除可能なSEQNo
//	【戻値】	-1			…	処理失敗、エラー
//				-2			…	他様式でも使用
//				-3			…	自様式のみ使用
//				-4			…	削除処理をキャンセル
//				0			…	ok→削除処理実行
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::DeleteCheckKinyuukikan(CDatabase* pDB, short nFormSeq, CArray<long, long>* pDelseq)
{
	long					nRet = 0;						// 戻値
	long					ret = 0;
	long					nIndex = 0;						// リストインデックス
	long					nSeq = 0;						// シーケンス番号
	int						ii = 0, jj = 0;
	int						cnt = 0;
	int						sw = 0;
	int						tblnamemax = 0;
	CArray<int, int>		delFormNo;						// テーブル名セット
	CString					strtblname[4] = { _T("") };		// 様式テーブル名
	CString					strSQL = _T("");
	CString					strData = _T("");
	CString					sMessage;
	_RD_BANK				udBank;							// 金融機関構造体
	CdbUcLstBank			mfcRec(pDB);					// uc_lst_bankテーブルクラス
	CRecordset				rs(pDB);
	CdlgReferenceDelChk2	dlg(this);
	ICSReportRow*			pRow;
	ICSReportSelectedRows*	pSelRows;

	pDelseq->RemoveAll();
	delFormNo.RemoveAll();

	// ①預貯金等の内訳書
	// ②受取手形の内訳書
	// ⑧支払手形の内訳書
	// ⑪借入金及び支払利子の内訳書
	strtblname[0] = STR_TBL_NAME_011;	// uc_011_yotyokin
	strtblname[1] = STR_TBL_NAME_021;	// uc_021_uketoritegata
	strtblname[2] = STR_TBL_NAME_081;	// uc_081_siharaitegata
	strtblname[3] = STR_TBL_NAME_111;	// uc_111_kariirekin
	tblnamemax = 4;

	cnt = 0;
	pSelRows = m_report.GetSelectedRows();
	for(ii=0; ii<pSelRows->GetCount(); ii++)	{
		// 選択行(ICSReportRowクラス)を順次取得
		pRow = pSelRows->GetAt(ii);
		if(pRow == NULL)	{
			break;
		}
		nIndex = pRow->GetIndex();
		nSeq = GetSeq(nIndex);
		if(nSeq <= 0)	continue;

		cnt++;	// 選択された行カウント

		// 金融機関のレコード取得失敗？
		if(GetKinnyuukikanRecord(pDB, nSeq, &udBank) != 0)	{
			// 失敗
			return(-1);
		}

		sw = 0;
		for(jj=0; jj<tblnamemax; jj++)	{
			strSQL.Format(_T("select count(Seq) from %s "), strtblname[jj]);
			// ②受取手形の内訳書
			if(jj == 1)	{
				strSQL.Format(strSQL + _T("where bkseq = %d or bk2seq = %d"), nSeq, nSeq);
			}
			// ⑪借入金及び支払利子の内訳書
			else if(jj == 3)	{
				strSQL.Format(strSQL + _T("where adseq = %d"), nSeq);
			}
			// ①預貯金等の内訳書
			// ⑧支払手形の内訳書
			else	{
				strSQL.Format(strSQL + _T("where bkseq = %d"), nSeq);
			}

			try		{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					strData.Empty();
					rs.GetFieldValue((short)0, strData);
					// 様式にデータあり
					if(_tstoi(strData) > 0)	{
						if(jj == 0)			delFormNo.Add(ID_FORMNO_011);
						else if(jj == 1)	delFormNo.Add(ID_FORMNO_021);
						else if(jj == 2)	delFormNo.Add(ID_FORMNO_081);
						else				delFormNo.Add(ID_FORMNO_111);
						sw = 1;
					}
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				return(-1);
			}
		}
		if(sw == 0)	{
			pDelseq->Add(nSeq);
		}
	}

	if(cnt > 1)	{
		// 選択されたデータが複数行の場合
		sMessage  = _T("選択された金融機関を削除してもよろしいですか？\r\n\r\n");
		sMessage += _T("※使用されている金融機関は削除されません。\r\n");
		//sMessage += _T("　削除したい場合は、個別選択で処理を実行してください。");		// 改良No.22-0404,22-0812_230426 del
		sMessage += _T("　削除したい場合は、個別に選択して処理を実行してください。");	// 改良No.22-0404,22-0812_230426 add
		if(ICSMessageBox(sMessage, MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
			nRet = -4;
		}
	}
	else if(cnt == 1)	{
		// 選択されたデータが１行の場合
		if((int)delFormNo.GetCount() > 0)	{
			// どこかの様式で使用している
			//ret = dlg.ShowDialog(pDB, ID_DLGTYPE_BANK, nFormSeq, -1, udBank.m_BkName1, udBank.m_BkName2, _T(""), delFormNo);	// 修正No.162360,162361 del
			ret = dlg.ShowDialog(pDB, ID_DLGTYPE_BANK, nFormSeq, -1, udBank.m_BkName1, udBank.m_BkName2, delFormNo);			// 修正No.162360,162361 add
			if(ret != ID_DLG_OK)	{
				nRet = -4;
			}
			else	{
				pDelseq->Add(nSeq);
			}
		}
		else	{
			// 確認メッセージを表示
			if(ICSMessageBox(_T("削除してもよろしいですか？"), MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
				nRet = -4;
			}
		}
	}
	else	{
		nRet = -1;
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先を削除する前に、削除可能かどうかチェック
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nGrSeq		…	グループ番号
//				pDelseq		…	削除可能なSEQNo
//	【戻値】	-1			…	処理失敗、エラー
//				-2			…	他様式でも使用
//				-3			…	自様式のみ使用
//				-4			…	削除処理をキャンセル
//				0			…	ok→削除処理実行
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::DeleteCheckTorihikisaki(CDatabase* pDB, short nFormSeq, int nGrSeq, CArray<long, long>* pDelseq)
{
	long					nRet = 0;						// 戻値
	long					ret = 0;
	long					nIndex = 0;						// リストインデックス
	long					nSeq = 0;						// シーケンス番号
	int						ii = 0, jj = 0;
	int						cnt = 0;
	int						sw = 0;
	int						tblnamemax = 0;
	CArray<int, int>		delFormNo;						// テーブル名セット
	CString					strtblname[9] = { _T("") };		// 様式テーブル名
	CString					strSQL = _T("");
	CString					strData = _T("");
	CString					sMessage = _T("");
	CString					cs1, cs2;
	_RD_ADDRESS				udAddress;						// 取引先構造体
	CdbUcLstAddress			mfcRec(pDB);					// uc_lst_addressテーブルクラス
	CRecordset				rs(pDB);
	CdlgReferenceDelChk1	dlg(this);
	ICSReportRow*			pRow;
	ICSReportSelectedRows*	pSelRows;

	pDelseq->RemoveAll();
	delFormNo.RemoveAll();

	// ②受取手形の内訳書
	// ③売掛金（未収入金）の内訳書
	// ④－１仮払金（前渡金）の内訳書
	// ④－２貸付金及び受取利息の内訳書
	// ⑧支払手形の内訳書
	// ⑨買掛金（未払金・未払費用）の内訳書
	// ⑩－１仮受金（前受金・預り金）の内訳書
	// ⑯－１雑益、雑損失等の内訳書
	// ⑯－２雑損失等の内訳書
	strtblname[0] = STR_TBL_NAME_021;	// uc_021_uketoritegata
	strtblname[1] = STR_TBL_NAME_031;	// uc_031_urikakekin
	strtblname[2] = STR_TBL_NAME_041;	// uc_041_karibarai
	strtblname[3] = STR_TBL_NAME_042;	// uc_042_kasituke
	strtblname[4] = STR_TBL_NAME_081;	// uc_081_siharaitegata
	strtblname[5] = STR_TBL_NAME_091;	// uc_091_kaikakekin
	strtblname[6] = STR_TBL_NAME_101;	// uc_101_kariukekin
	strtblname[7] = STR_TBL_NAME_161;	// uc_161_zatueki
	strtblname[8] = STR_TBL_NAME_162;	// uc_162_zatuson
	tblnamemax = 9;

	cnt = 0;
	// 選択されたシーケンス番号を取得
	pSelRows = m_report.GetSelectedRows();
	for(ii=0; ii<pSelRows->GetCount(); ii++)	{
		// 選択行(ICSReportRowクラス)を順次取得
		pRow = pSelRows->GetAt(ii);
		if(pRow == NULL)	{
			break;
		}
		nIndex = pRow->GetIndex();
		nSeq = GetSeq(nIndex);
		if(nSeq <= 0)	continue;

		cnt++;	// 選択された行カウント

		// 取引先のレコード取得失敗？
		if(GetTorihikisakiRecord(pDB, nGrSeq, nSeq, &udAddress) != 0)	{
			// 失敗
			return(-1);
		}

		sw = 0;
		for(jj=0; jj<tblnamemax; jj++)	{
			strSQL.Format(_T("select count(Seq) from %s "), strtblname[jj]);
			strSQL.Format(strSQL + _T("where AdSeq = %d"), nSeq);

			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF()) {
					strData.Empty();
					rs.GetFieldValue((short)0, strData);
					// 様式にデータあり
					if(_tstoi(strData) > 0)	{
						if(jj == 0)			delFormNo.Add(ID_FORMNO_021);
						else if(jj == 1)	delFormNo.Add(ID_FORMNO_031);
						else if(jj == 2)	delFormNo.Add(ID_FORMNO_041);
						else if(jj == 3)	delFormNo.Add(ID_FORMNO_042);
						else if(jj == 4)	delFormNo.Add(ID_FORMNO_081);
						else if(jj == 5)	delFormNo.Add(ID_FORMNO_091);
						else if(jj == 6)	delFormNo.Add(ID_FORMNO_101);
						else if(jj == 7)	delFormNo.Add(ID_FORMNO_161);
						else				delFormNo.Add(ID_FORMNO_162);
						sw = 1;
					}
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				return(-1);
			}
		}

		if(sw == 0)	{
			pDelseq->Add(nSeq);
		}
	}

	if(cnt > 1)	{
		// 選択されたデータが複数行の場合
		// 選択数が複数件の時
		sMessage = _T("選択された");
		if(nGrSeq == ID_ADDRESSGR_URI)	sMessage += _T("取引先(得意先) ");
		else							sMessage += _T("取引先(仕入先) ");
		sMessage += _T("を削除してもよろしいですか？\r\n\r\n");
		sMessage += _T("※使用されている取引先は削除されません。\r\n");
		//sMessage += _T("　削除したい場合は、個別選択で処理を実行してください。");		// 改良No.22-0404,22-0812_230426 del
		sMessage += _T("　削除したい場合は、個別に選択して処理を実行してください。");	// 改良No.22-0404,22-0812_230426 add
		if(ICSMessageBox(sMessage, MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
			nRet = -4;
		}
	}
	else if(cnt == 1)	{
		// 選択されたデータが１行の場合
		if((int)delFormNo.GetCount() > 0)	{
// 修正No.162360,162361 del -->
			//cs1 = udAddress.m_AdName1 + udAddress.m_AdName2;
			//ret = dlg.ShowDialog(pDB, ID_DLGTYPE_ADDRESS, nFormSeq, nGrSeq, cs1, udAddress.m_AdAdd1, udAddress.m_AdAdd2, delFormNo);
// 修正No.162360,162361 del <--
// 修正No.162360,162361 add -->
			ret = dlg.ShowDialog(pDB, ID_DLGTYPE_ADDRESS, nFormSeq, nGrSeq, udAddress.m_AdName1, udAddress.m_AdName2, udAddress.m_AdAdd1, udAddress.m_AdAdd2, delFormNo);
// 修正No.162360,162361 add <--
			if(ret != ID_DLG_OK)	{
				nRet = -4;
			}
			else	{
				pDelseq->Add(nSeq);
			}
		}
		else	{
			// 確認メッセージを表示
			if(ICSMessageBox(_T("削除してもよろしいですか？"), MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
				nRet = -4;
			}
		}
	}
	else	{
		nRet = -1;
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	科目を削除する前に、削除可能かどうかチェック
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				pDelseq			…	削除可能なSEQNo
//	【戻値】	-1				…	処理失敗、エラー
//				-2				…	デフォルト科目で削除不可
//				-3				…	財務連動科目で削除不可
//				-4				…	使用してた
//				-5				…	削除処理キャンセル
//				0				…	ok→削除処理実行
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::DeleteCheckKamoku(CDatabase* pDB, short nFormSeq, CArray<long, long>* pDelseq)
{
	int						ii = 0;
	int						cnt = 0;
	int						sw = 0;
	long					nRet = 0;				// 戻値
	long					nIndex = 0;				// リストインデックス
	long					nKnSeq = 0;				// シーケンス番号
	BOOL					fDelete = TRUE;			// 削除フラグ
	BOOL					zDelete = TRUE;			// 財務連動登録済フラグ
	CString					strtblname = _T("");
	CString					strSQL = _T("");
	CString					strData = _T("");
	CString					sMessage = _T("");
	_RD_KAMOKU				udKamoku;				// 科目構造体
	CdbUcLstKamoku			mfcRec(pDB);			// uc_lst_kamokuテーブルクラス
	CdbUcRenKamoku			mfcZmKmk(pDB);			// uc_ren_kamokuテーブルクラス
	CRecordset				rs(pDB);
	ICSReportRow*			pRow;
	ICSReportSelectedRows*	pSelRows;

	pDelseq->RemoveAll();

	cnt = 0;
	// 選択されたシーケンス番号を取得
	pSelRows = m_report.GetSelectedRows();
	for(ii=0; ii<pSelRows->GetCount(); ii++)	{
		// 選択行(ICSReportRowクラス)を順次取得
		pRow = pSelRows->GetAt(ii);
		if(pRow == NULL)	{
			break;
		}
		nIndex = pRow->GetIndex();
		nKnSeq = GetSeq(nIndex);
		if(nKnSeq <= 0)	continue;

		cnt++;	// 選択された行カウント

		// 科目のレコード取得失敗？
		if(GetKamokuRecord(pDB, nFormSeq, nKnSeq, &udKamoku) != 0)	{
			// 失敗
			return(-1);
		}

		sw = 0;
		try	{
			// 初期化成功？
			if(mfcRec.RequeryKnSeq(nFormSeq, nKnSeq) == DB_ERR_OK)	{
				// レコードあり？
				if( !(mfcRec.IsEOF()) )	{
					// レコードを先頭に移動
					mfcRec.MoveFirst();
					// デフォルト値
					if(mfcRec.m_FgDft == 0)	{
						// 財務連動登録済みかどうか確認
						if(mfcZmKmk.RequeryKnSeq(nKnSeq) == DB_ERR_OK)	{
							// レコードあり?
							if( !(mfcZmKmk.IsEOF()) )	{
								zDelete = FALSE;	// 削除フラグを未削除に設定
							}
							// レコード閉じる
							mfcZmKmk.Fin();
						}
					}
					else	{
						fDelete = FALSE;	// 削除フラグを未削除に設定
					}
				}
				// レコード閉じる
				mfcRec.Fin();

				// 削除ＯＫ？
				if(fDelete == FALSE)	{
					break;
				}
				else if(zDelete == FALSE)	{
					break;
				}
			}
		}
		catch(...)	{
			// レコード開いている？
			if(mfcRec.IsOpen())	{
				mfcRec.Fin();	// レコード閉じる
			}
			if(mfcZmKmk.IsOpen())	{
				mfcZmKmk.Fin();	// レコード閉じる
			}
		}

		//-------------------------------------------------------------------------------------
		// 基本科目と財務連動の対象科目でなかったら、使用しているかどうか確認する
		// テーブル名取得
		switch(nFormSeq)	{
			case ID_FORMNO_011:	strtblname = _T("uc_011_yotyokin");			break;	// KnSeq
			case ID_FORMNO_021:	strtblname = _T("uc_021_uketoritegata");	break;	// KnSeq
			case ID_FORMNO_031:	strtblname = _T("uc_031_urikakekin");		break;	// KnSeq
			case ID_FORMNO_041:	strtblname = _T("uc_041_karibarai");		break;	// KnSeq
			case ID_FORMNO_051:	strtblname = _T("uc_051_tanaoroshi");		break;	// KnSeq
			case ID_FORMNO_061:	strtblname = _T("uc_061_yuukasyouken");		break;	// KnSeq
// 改良No.21-0086,21-0529 add -->
			case ID_FORMNO_081:	strtblname = _T("uc_081_siharaitegata");	break;	// KnSeq
// 改良No.21-0086,21-0529 add <--
			case ID_FORMNO_091:	strtblname = _T("uc_091_kaikakekin");		break;	// KnSeq
			case ID_FORMNO_101:	strtblname = _T("uc_101_kariukekin");		break;	// KnSeq
			case ID_FORMNO_102:	strtblname = _T("uc_102_gensen");			break;	// KnSeq
			case ID_FORMNO_111:	strtblname = _T("uc_111_kariirekin");		break;	// KnSeq
			case ID_FORMNO_121:	strtblname = _T("uc_121_totiuriage");		break;	// KnSeq
			case ID_FORMNO_151:	strtblname = _T("uc_151_tidaiyatin");		break;	// KnSeq
			case ID_FORMNO_161:	strtblname = _T("uc_161_zatueki");			break;	// KnSeq
			case ID_FORMNO_162:	strtblname = _T("uc_162_zatuson");			break;	// KnSeq
			case ID_FORMNO_171:
			case ID_FORMNO_172:
			case ID_FORMNO_173:
			case ID_FORMNO_174:
			case ID_FORMNO_175:
			case ID_FORMNO_176:
			case ID_FORMNO_177:
			case ID_FORMNO_178:
			case ID_FORMNO_179:
			case ID_FORMNO_1710:
			case ID_FORMNO_1711:
			case ID_FORMNO_1712:
			case ID_FORMNO_1713:
			case ID_FORMNO_1714:
			case ID_FORMNO_1715:
			case ID_FORMNO_1716:
			case ID_FORMNO_1717:
			case ID_FORMNO_1718:
			case ID_FORMNO_1719:
			case ID_FORMNO_1720:
								strtblname = _T("uc_171_sonota1");			break;	// FormSeq,KnSeq
			case ID_FORMNO_181:
			case ID_FORMNO_182:
			case ID_FORMNO_183:
			case ID_FORMNO_184:
			case ID_FORMNO_185:
			case ID_FORMNO_186:
			case ID_FORMNO_187:
			case ID_FORMNO_188:
			case ID_FORMNO_189:
			case ID_FORMNO_1810:
			case ID_FORMNO_1811:
			case ID_FORMNO_1812:
			case ID_FORMNO_1813:
			case ID_FORMNO_1814:
			case ID_FORMNO_1815:
			case ID_FORMNO_1816:
			case ID_FORMNO_1817:
			case ID_FORMNO_1818:
			case ID_FORMNO_1819:
			case ID_FORMNO_1820:
								strtblname = _T("uc_181_sonota2");			break;	// FormSeq,KnSeq
			default:														break;
		}

		strSQL.Format(_T("select count(Seq) From %s where KnSeq = %d"), strtblname, nKnSeq);
		if(nFormSeq >= ID_FORMNO_171 && nFormSeq <= ID_FORMNO_1820)	{
			strSQL.Format(strSQL + _T(" and FormSeq = %d"), nFormSeq);
		}
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0, strData);
		rs.Close();
		if(_tstoi(strData) > 0)	{
			sw = 1;
		}
		pDelseq->Add(nKnSeq);
	}

	if(cnt <= 0) {
		return(-1);
	}

	//-------------------------------------------------------------------------------------
	nRet = 0;
	if(fDelete == FALSE)	{
// 改良No.23-0513 cor -->
		//sMessage = _T("規定科目のため削除できません。");
// ----------------------
		sMessage.Empty();
		if(m_nFormSeq == ID_FORMNO_011 || m_nFormSeq == ID_FORMNO_102)	{
			sMessage = _T("既定の種類は削除できません。");
		}
		else if(m_nFormSeq == ID_FORMNO_061 || m_nFormSeq == ID_FORMNO_121 || m_nFormSeq == ID_FORMNO_151)	{
			sMessage = _T("既定の区分は削除できません。");
		}
		else	{
			sMessage = _T("既定の科目は削除できません。");
		}
// 改良No.23-0513 cor <--
		ICSMessageBox(sMessage, MB_OK, 0, 0, this);
		nRet = -2;
	}
	else if(zDelete == FALSE)	{
		sMessage = _T("財務連動の対象となっているため、削除できません。");
		ICSMessageBox(sMessage, MB_OK, 0, 0, this);
		nRet = -3;
	}
	else if(sw == 1)	{
		sMessage.Empty();
// 改良No.23-0513 cor -->
		////sMessage.Format(_T("科目［%s］は使用されています。"), udKamoku.m_KnName);	// 修正No.162435 del
		//sMessage.Format(_T("科目：%s は使用されています。"), udKamoku.m_KnName);	// 修正No.162435 add
		//sMessage += _T("\r\n削除してもよろしいですか？");
		////sMessage += _T("\r\n※削除した場合は、様式からも削除され、空欄で表示されます。");			// 改良No.22-0404,22-0812_230414 del
		////sMessage += _T("\r\n※削除した場合は、様式からも削除され、帳表上空欄で表示されます。");	// 改良No.22-0404,22-0812_230414 add
		//sMessage += _T("\r\n\r\n※削除した場合は様式及び保管参照からも削除され、該当項目は空欄で表示されます。");	// 修正No.162389 add
// ----------------------
		if(m_nFormSeq == ID_FORMNO_011 || m_nFormSeq == ID_FORMNO_102)
			sMessage = _T("種類：");
		else if(m_nFormSeq == ID_FORMNO_061 || m_nFormSeq == ID_FORMNO_121 || m_nFormSeq == ID_FORMNO_151)
			sMessage = _T("区分：");
		else	
			sMessage = _T("科目：");
		sMessage += udKamoku.m_KnName;
		sMessage += _T("は使用されています。");
		sMessage += _T("\r\n削除してもよろしいですか？");
		sMessage += _T("\r\n\r\n※削除した場合は様式及び保管参照からも削除され、該当項目は空欄で表示されます。");
// 改良No.23-0513 cor <--
		if(ICSMessageBox(sMessage, MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
			nRet = -4;
		}
	}
	else	{
		// 確認メッセージを表示
		if(ICSMessageBox(_T("削除してもよろしいですか？"), MB_YESNO | MB_DEFBUTTON2, 0, 0, this) == IDNO)	{
			nRet = -4;
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	レコードの削除
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
//				pDelseq			…	選択されたSEQNo
//	【戻値】	0				…	編集なし
//				0以外			…	編集あり
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::SetRecordDelete(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, CArray<long, long>* pDelseq)
{
	long	nRet = -1;		// 戻値
	long	nTopIndex = 0;	// トップインデックス

	// 参照型で分岐
	switch(nType)	{
		case ID_DLGTYPE_BANK:		// 金融機関
			// 金融機関のレコード削除
			nRet = DeleteKinyuukikan(pDB, pDelseq);
			break;

		case ID_DLGTYPE_ADDRESS:	// 取引先
			// 取引先のレコード削除
			nRet = DeleteTorihikisaki(pDB, nGrSeq, pDelseq);
			break;

		case ID_DLGTYPE_KAMOKU:		// 科目
			// 科目のレコード削除
			nRet = DeleteKamoku(pDB, nFormSeq, pDelseq);
			break;
	}
	
	// 削除された？
	if(nRet >= 0)	{
		// 前回選択行をクリア
		m_nOldRow = -1;
		// リストのトップ
		nTopIndex = m_report.GetTopRowIndex();
		// リストの更新
		GetReportData(pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
					  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
		
		// 位置移動
		//MovePosition(nTopIndex, (nRet - 1));	// 修正No.162405 del
		MovePosition(nTopIndex, nRet);			// 修正No.162405 add

		PostMessage(WM_USER_SELECTCHECK, 0, 0);	// 修正No.162466 add
	}
	
	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関のレコード削除
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				pDelseq			…	選択されたSEQNo
//	【戻値】	-1				…	削除なし
//				0以上			…	削除した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::DeleteKinyuukikan(CDatabase* pDB, CArray<long, long>* pDelseq)
{
	long					nRet = -1;			// 戻値
	long					nIndex = 0;			// リストインデックス
	long					nSeq = 0;			// シーケンス番号
	long					nOrderNum = 0;		// 順序番号
	int						ii = 0;
	int						max = 0;
	int						cnt = 0;
	CdbUcLstBank			mfcRec(pDB);		// uc_lst_bankテーブルクラス
	_RD_BANK				udBank;				// 金融機関構造体

	// 削除する件数を取得
	max = (int)pDelseq->GetCount();

	for(ii=0; ii<max; ii++)	{
		// シーケンス番号を取得
		nSeq = pDelseq->GetAt(ii);

		// 金融機関のレコード取得失敗？
		if(GetKinnyuukikanRecord(pDB, nSeq, &udBank) != 0)	{
			// 失敗
			return(-1);
		}

		// 順序番号取得
		nOrderNum = udBank.m_OrderNum;
		if(ii == 0)	nIndex = nOrderNum - 1;

		try	{
			// 初期化成功？
			if(mfcRec.RequerySeq(nSeq) == DB_ERR_OK)	{
				// レコードあり？
				if(!(mfcRec.IsEOF()))	{
					mfcRec.MoveFirst();				// レコードを先頭に移動
					mfcRec.Delete();				// 削除
				}
				// レコード閉じる
				mfcRec.Fin();
			}

			cnt = 0;
			if(mfcRec.Init() == DB_ERR_OK)	{
				cnt = mfcRec.GetRecordCount();
				mfcRec.Fin();
			}
			if(cnt > 0)	{
				// 順序番号振り直し成功？
				if(mfcRec.ReNumberOrder(nOrderNum, -1) == DB_ERR_OK)	{
					// レコード閉じる
					mfcRec.Fin();
				}
			}

		}
		catch(...)	{
			// レコード開いている？
			if(mfcRec.IsOpen())	{
				mfcRec.Fin();	// レコード閉じる
			}
			return(-1);
		}
	}

	// 順序番号が最終行？
	if(nIndex == m_DataLineMax)	{
		nIndex--;
	}

	// 戻値を設定
	nRet = nIndex;

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先のレコード削除
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
//				pDelseq			…	選択されたSEQNo
//	【戻値】	-1				…	削除なし
//				0以上			…	削除した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::DeleteTorihikisaki(CDatabase* pDB, int nGrSeq, CArray<long, long>* pDelseq)
{
	long					nRet = -1;			// 戻値
	long					nIndex = 0;			// リストインデックス
	long					nSeq = 0;			// シーケンス番号
	long					nOrderNum = 0;		// 順序番号
	int						cnt = 0;
	int						ii = 0;
	int						max = 0;
	CdbUcLstAddress			mfcRec(pDB);		// uc_lst_addressテーブルクラス
	_RD_ADDRESS				udAddress;			// 取引先構造体

	max = (int)pDelseq->GetCount();

	for(ii=0; ii<max; ii++)	{
		// シーケンス番号を取得
		nSeq = pDelseq->GetAt(ii);

		// 取引先のレコード取得失敗？
		if(GetTorihikisakiRecord(pDB, nGrSeq, nSeq, &udAddress) != 0)	{
			// 失敗
			return(-1);
		}

		// 順序番号取得
		nOrderNum = udAddress.m_OrderNum;
		if(ii == 0)	nIndex = nOrderNum - 1;

		try	{
			// 初期化成功？
			if(mfcRec.RequeryAdSeq(nGrSeq, nSeq) == DB_ERR_OK)	{
				// レコードあり？
				if(!(mfcRec.IsEOF()))	{
					mfcRec.MoveFirst();			// レコードを先頭に移動
					mfcRec.Delete();			// 削除
				}
				// レコード閉じる
				mfcRec.Fin();
			}

			// データベースの登録件数を取得
			cnt = 0;
			if(mfcRec.Init(nGrSeq) == DB_ERR_OK)	{
				cnt = mfcRec.GetRecordCount();
				mfcRec.Fin();
			}
			if(cnt > 0)	{
				// 順序番号振り直し成功？
				if(mfcRec.ReNumberOrder(nGrSeq, nOrderNum, -1) == DB_ERR_OK)	{
					// レコード閉じる
					mfcRec.Fin();
				}
			}

		}
		catch(...)	{
			// レコード開いている？
			if(mfcRec.IsOpen())	{
				mfcRec.Fin();	// レコード閉じる
			}
			return(-1);
		}
	}

	// 順序番号が最終行？
	if(nIndex == m_DataLineMax)	{
		nIndex--;
	}

	// 戻値を順序番号に設定
	nRet = nIndex;

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	科目のレコード削除
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				pDelseq			…	選択されたSEQNo
//	【戻値】	-1				…	削除失敗
//				-2				…	デフォルト科目で削除不可
//				0以上			…	削除した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::DeleteKamoku(CDatabase* pDB, short nFormSeq, CArray<long, long>* pDelseq)
{
	long					nRet = -1;			// 戻値
	long					nIndex = 0;			// リストインデックス
	long					nKnSeq = 0;			// シーケンス番号
	long					nKnOrder = 0;		// 順序番号
	int						max = 0;
	int						ii = 0;
	int						cnt = 0;
	CdbUcLstKamoku			mfcRec(pDB);		// uc_lst_kamokuテーブルクラス
	CdbUcRenKamoku			mfcZmKmk(pDB);		// uc_ren_kamokuテーブルクラス
	_RD_KAMOKU				udKamoku;			// 科目構造体

	max = (int)pDelseq->GetCount();

	for(ii=0; ii<max; ii++)	{
		// シーケンス番号を取得
		nKnSeq = pDelseq->GetAt(ii);

		// 科目のレコード取得失敗？
		if(GetKamokuRecord(pDB, nFormSeq, nKnSeq, &udKamoku) != 0)	{
			// 失敗
			return(-1);
		}

		// 順序番号取得
		nKnOrder = udKamoku.m_KnOrder;
		nIndex = nKnOrder - 1;

		try		{
			// 初期化成功？
			if(mfcRec.RequeryKnSeq(nFormSeq, nKnSeq) == DB_ERR_OK)	{
				// レコードあり？
				if(!(mfcRec.IsEOF()))	{
					mfcRec.MoveFirst();		// レコードを先頭に移動
					mfcRec.Delete();		// 削除
				}
				// レコード閉じる
				mfcZmKmk.Fin();
			}

			// データベースの登録件数を取得
			cnt = 0;
			if(mfcRec.Init(nFormSeq) == DB_ERR_OK)	{
				cnt = mfcRec.GetRecordCount();
				mfcRec.Fin();
			}
			if(cnt > 0)	{
				// 順序番号振り直し成功？
				if(mfcRec.ReNumberOrder(nFormSeq, nKnOrder, -1) == DB_ERR_OK)	{
					// レコード閉じる
					mfcRec.Fin();
				}
			}
		}
		catch(...)	{
			// レコード開いている？
			if(mfcRec.IsOpen())	{
				mfcRec.Fin();	// レコード閉じる
			}
		}
	}

	// 順序番号が最終行？
	if(nIndex == m_DataLineMax)	{
		// 順序番号デクリメント
		nIndex--;
	}
	// 戻値を順序番号に設定
	nRet = nIndex;

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	レコードの挿入（追加）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nType			…	参照型
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
//				fAddLine		…	追加フラグ（TRUE：追加／FALSE：挿入）
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入あり
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::SetRecordInsert(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, BOOL fAddLine)
{
	long					nRet = 0;		// 戻値
	long					nTopIndex = 0;	// トップインデックス
	CString					szTitle;		// タイトル文字列
// 改良No.22-0404,22-0812_230414 add -->
	int						nIndex;
	ICSReportRow*			pRow;
	ICSReportSelectedRows*	pSelRows;

	// 複数行が選択されていれば、単一行選択に戻す
	pSelRows = m_report.GetSelectedRows();
	if(pSelRows->GetCount() > 1)	{
		pRow = m_report.GetFocusedRow();
		nIndex = pRow->GetIndex();
		m_report.SetUnSelectedRows();
		m_report.SetSelectedRows(nIndex, nIndex);
	}
// 改良No.22-0404,22-0812_230414 add <--

	// 初期化
	szTitle.Empty();
	// 追加フラグで分岐
	switch(fAddLine)	{
		// 追加
		case TRUE:
			szTitle = "[F9]追加";
			break;

		// 挿入
		case FALSE:
			szTitle = "[F8]挿入";
			break;
	}

	// 参照型で分岐
	switch(nType)	{
		// 金融機関
		case ID_DLGTYPE_BANK:
			// 金融機関のレコード挿入（追加）
			nRet = InsertKinyuukikan(pDB, nSort, szTitle, fAddLine);
			break;
		// 取引先
		case ID_DLGTYPE_ADDRESS:
			// 取引先のレコード挿入（追加）
			nRet = InsertTorihikisaki(pDB, nGrSeq, nSort, szTitle, fAddLine);
			break;
		// 科目
		case ID_DLGTYPE_KAMOKU:
			// 科目のレコード挿入（追加）
			nRet = InsertKamoku(pDB, nFormSeq, nSort, szTitle, fAddLine);
			break;
	}
	
	// 挿入された？
	if(nRet != 0)	{
		// 前回選択行をクリア
		m_nOldRow = -1;
		// リストのトップ
		nTopIndex = m_report.GetTopRowIndex();
		// リストの更新
		GetReportData(pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
					  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
		// 位置移動
		MovePosition2(nRet);
	}
	
	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関のレコード挿入（追加）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nSort			…	表示順
//				szTitle			…	ダイアログタイトル
//				fAddLine		…	追加フラグ
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::InsertKinyuukikan(CDatabase* pDB, int nSort, CString szTitle,  BOOL fAddLine)
{
	long					nRet = 0;			// 戻値初期化
	long					nIndex = 0;			// リストインデックス
	long					nOrderNum = 0;		// 順序番号（数値）
	long					nSeq = 0;			// シーケンス番号
	int						nIdxOffset = 1;		// フォーカスのオフセット
	CdlgReferenceSub		dlgSub(this);		// 参照サブダイアログクラス
	CdbUcLstBank			mfcRec(pDB);		// uc_lst_bankテーブルクラス	
	_RD_BANK				udBank;				// 金融機関構造体
	EnumIdDlgRefMode		nMode;				// 動作モード

	// 追加フラグで分岐
	switch(fAddLine)	{
		// 追加
		case TRUE:
			nIndex = GetListIndex2(pDB, ID_DLGTYPE_BANK, -1, -1, nSort);		// インデックス算出
			nMode = ID_DLGREF_APPEND;
			nIdxOffset++;
			break;

		// 挿入
		case FALSE:
			// 賞味のインデックス取得
			nIndex = GetListIndex();
			nMode = ID_DLGREF_INSERT;
			break;
	}

	// シーケンス番号取得
	nSeq = GetSeq(nIndex);
	// 金融機関レコード取得
	GetKinnyuukikanRecord(pDB, nSeq, &udBank);
	// 順序番号を取得
	nOrderNum = udBank.m_OrderNum;

	// 追加？
	if(fAddLine == TRUE)	{
		// 順序番号をインクリメント
		nOrderNum = GetMaxOrderNo(pDB, ID_DLGTYPE_BANK, -1, -1);		// 修正No.162491 add
		nOrderNum++;
	}

	// 変更あり？
	if(dlgSub.ShowDialog(pDB, ID_DLGTYPE_BANK, nMode, szTitle, nOrderNum, "", "") == ID_DLG_OK)	{
		try	{
			// 順序番号振り直し成功？
			if(mfcRec.ReNumberOrder(nOrderNum, 1) == DB_ERR_OK)	{
				// 新規行追加成功？
				if(mfcRec.CreateNewRecord() == DB_ERR_OK)	{
					// レコードあり？
					if(!(mfcRec.IsEOF()))	{
						mfcRec.MoveFirst();							// レコードを先頭に移動
						mfcRec.Edit();								// レコード編集開始

						mfcRec.m_OrderNum	= nOrderNum;			// 順序番号設定
						mfcRec.m_BkName1	= dlgSub.m_RetName1;	// 金融機関名設定
						mfcRec.m_BkName2	= dlgSub.m_RetName2;	// 支店名設定
						mfcRec.m_BkKana1	= dlgSub.m_RetKana1;	// 金融機関名ｶﾅ設定
						mfcRec.m_BkKana2	= dlgSub.m_RetKana2;	// 支店名ｶﾅ設定
						mfcRec.m_FgDel		= 0;					// 削除フラグ設定

						mfcRec.Update();							// レコード更新
					}
					// NULLの場合、ここで''（未入力）に更新
					mfcRec.UpdateStrFieldAllForNull();
					// レコード閉じる
					mfcRec.Fin();
					// 戻値に順序番号を設定
					nRet = nOrderNum;
				}
				// レコード閉じる
				mfcRec.Fin();
			}
		}
		catch(...)	{
			// レコード開いてる？
			if(mfcRec.IsOpen())	{
				// レコード閉じる
				mfcRec.Fin();
			}
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先のレコード挿入（追加）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
//				szTitle			…	ダイアログタイトル
//				fAddLine		…	追加フラグ
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::InsertTorihikisaki(CDatabase* pDB, int nGrSeq, int nSort, CString szTitle,  BOOL fAddLine)
{
	long					nRet = 0;			// 戻値初期化
	long					nIndex = 0;			// リストインデックス
	long					nOrderNum = 0;		// 順序番号（数値）
	long					nSeq = 0;			// シーケンス番号
	int						nIdxOffset = 1;		// フォーカスのオフセット
	CdlgReferenceSub		dlgSub( this );		// 参照サブダイアログクラス
	CdbUcLstAddress			mfcRec( pDB );		// uc_lst_addressテーブルクラス	
	_RD_ADDRESS				udAddress;			// 取引先構造体
	EnumIdDlgRefMode		nMode;				// 動作モード

	// 追加フラグで分岐
	switch(fAddLine)	{
		// 追加
		case TRUE:
			nIndex = GetListIndex2(pDB, ID_DLGTYPE_ADDRESS, nGrSeq, -1, nSort);		// インデックス算出
			nMode = ID_DLGREF_APPEND;
			nIdxOffset++;
			break;

		// 挿入
		case FALSE:
			// 賞味のインデックス取得
			nIndex = GetListIndex();
			nMode = ID_DLGREF_INSERT;
			break;
	}

	// シーケンス番号取得
	nSeq = GetSeq(nIndex);
	// 取引先レコード取得
	GetTorihikisakiRecord(pDB, nGrSeq, nSeq, &udAddress);
	// 順序番号を取得
	nOrderNum = udAddress.m_OrderNum;

	// 追加？
	if(fAddLine == TRUE)	{
		// 順序番号をインクリメント
		nOrderNum = GetMaxOrderNo(pDB, ID_DLGTYPE_ADDRESS, nGrSeq, -1);		// 修正No.162491 add
		nOrderNum++;
	}

	// 変更あり？
	//if(dlgSub.ShowDialog(pDB, ID_DLGTYPE_ADDRESS, nMode, szTitle, nOrderNum, "", "", "", "", "", "", 0, nGrSeq) == ID_DLG_OK)	{		// インボイス登録番号追加対応_23/11/06 del
	if(dlgSub.ShowDialog(pDB, ID_DLGTYPE_ADDRESS, nMode, szTitle, nOrderNum, "", "", "", "", "", "", "", 0, nGrSeq) == ID_DLG_OK)	{	// インボイス登録番号追加対応_23/11/06 add
		try	{
			// 順序番号振り直し成功？
			if(mfcRec.ReNumberOrder(nGrSeq, nOrderNum, 1) == DB_ERR_OK)	{
				// 新規行追加成功？
				if(mfcRec.CreateNewRecord(nGrSeq) == DB_ERR_OK)	{
					// レコードあり？
					if(!(mfcRec.IsEOF()))	{
						mfcRec.MoveFirst();							// レコードを先頭に移動
						mfcRec.Edit();								// レコード編集開始

						mfcRec.m_OrderNum	= nOrderNum;			// 順序番号設定
						mfcRec.m_AdName1	= dlgSub.m_RetName1;	// 名称設定
						mfcRec.m_AdName2	= dlgSub.m_RetName2;	// 名称設定
						mfcRec.m_AdAdd1		= dlgSub.m_RetAdd1;		// 所在地設定
						mfcRec.m_AdAdd2		= dlgSub.m_RetAdd2;		// 所在地設定
						mfcRec.m_AdKana		= dlgSub.m_RetKana1;	// 科目ｶﾅ名設定
						mfcRec.m_InvNo		= dlgSub.m_RetInvno;	// 登録番号設定		インボイス登録番号追加対応_23/11/06 add
						mfcRec.m_FgDel		= 0;					// 削除フラグ設定

						mfcRec.Update();							// レコード更新
					}
					// NULLの場合、ここで''（未入力）に更新
					mfcRec.UpdateStrFieldAllForNull();
					// レコード閉じる
					mfcRec.Fin();
					// 戻値に順序番号を設定
					nRet = nOrderNum;							// 現在選択中の行の1つ下を対象とするため
				}
				// レコード閉じる
				mfcRec.Fin();
			}
		}
		catch(...)	{
			// レコード開いてる？
			if(mfcRec.IsOpen())	{
				// レコード閉じる
				mfcRec.Fin();
			}
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	科目のレコード挿入（追加）
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nSort			…	表示順
//				szTitle			…	ダイアログタイトル
//				fAddLine		…	追加フラグ
//	【戻値】	0				…	挿入なし
//				0以外			…	挿入した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::InsertKamoku(CDatabase* pDB, short nFormSeq, int nSort, CString szTitle, BOOL fAddLine)
{
	long					nRet = 0;			// 戻値初期化
	long					nIndex = 0;			// リストインデックス
	long					nKnOrder = 0;		// 順序番号（数値）
	long					nKnSeq = 0;			// シーケンス番号
	int						nInputLen;			// 入力文字数
	int						nIdxOffset = 1;		// フォーカスのオフセット
	CdlgReferenceSub		dlgSub( this );		// 参照サブダイアログクラス
	CdbUcLstKamoku			mfcRec( pDB );		// uc_lst_kamokuテーブルクラス	
	_RD_KAMOKU				udKamoku;			// 科目構造体
	EnumIdDlgRefMode		nMode;				// 動作モード

	// 追加フラグで分岐
	switch(fAddLine)	{
		// 追加
		case TRUE:
			nIndex = GetListIndex2(pDB, ID_DLGTYPE_KAMOKU, -1, nFormSeq, nSort);		// インデックス算出
			nMode = ID_DLGREF_APPEND;
			nIdxOffset++;
			break;

		// 挿入
		case FALSE:
			// 賞味のインデックス取得
			nIndex = GetListIndex();
			nMode = ID_DLGREF_INSERT;
			break;
	}

	// シーケンス番号取得
	nKnSeq = GetSeq(nIndex);
	// 科目レコード取得
	GetKamokuRecord(pDB, nFormSeq, nKnSeq, &udKamoku);
	// 順序番号を取得
	nKnOrder = udKamoku.m_KnOrder;

	// 追加？
	if(fAddLine == TRUE)	{
		// 順序番号をインクリメント
		nKnOrder = GetMaxOrderNo(pDB, ID_DLGTYPE_KAMOKU, -1, nFormSeq);		// 修正No.162491 add
		nKnOrder++;
	}
	
	switch(nFormSeq)	{
		// ⑩-2.源泉所得税預り金の内訳
		case ID_FORMNO_102:
			nInputLen = DR_INPUTLENGTH_GENSEN;
			break;
		// ⑫.土地の売上高等の内訳書
		case ID_FORMNO_121:
			nInputLen = DR_INPUTLENGTH_TOTIURI;
			break;
		// ⑮-1.地代家賃の内訳書
		case ID_FORMNO_151:
			nInputLen = DR_INPUTLENGTH_TIDAI;
			break;
		default:
			// 入力文字数を８文字に設定
			nInputLen = DR_INPUTLENGTH_NORMAL;
			break;
	}

	// 変更あり？
	//if(dlgSub.ShowDialog(pDB, ID_DLGTYPE_KAMOKU, nMode, szTitle, nKnOrder, "", "", "", "", "", "", nInputLen, nFormSeq) == ID_DLG_OK)	{		// インボイス登録番号追加対応_23/11/06 del
	if(dlgSub.ShowDialog(pDB, ID_DLGTYPE_KAMOKU, nMode, szTitle, nKnOrder, "", "", "", "", "", "", "", nInputLen, nFormSeq) == ID_DLG_OK)	{	// インボイス登録番号追加対応_23/11/06 add
		try	{
			// 順序番号振り直し成功？
			if(mfcRec.ReNumberOrder(nFormSeq, nKnOrder, 1) == DB_ERR_OK)	{
				// 新規行追加成功？
				if(mfcRec.CreateNewRecord(nFormSeq) == DB_ERR_OK)	{
					// レコードあり？
					if(!(mfcRec.IsEOF()))	{
						mfcRec.MoveFirst();							// レコードを先頭に移動
						mfcRec.Edit();								// レコード編集開始

						mfcRec.m_FormSeq	= nFormSeq;				// 様式シーケンス番号設定
						mfcRec.m_KnOrder	= nKnOrder;				// 順序番号設定
						mfcRec.m_KnCode		= _T("");				// 科目コード設定
						mfcRec.m_KnName		= dlgSub.m_RetName1;	// 科目名称設定
						mfcRec.m_FgDel		= 0;					// 削除フラグ設定
						mfcRec.m_FgDft		= 0;					// デフォルト値フラグ設定
						mfcRec.m_KnKana		= dlgSub.m_RetKana1;	// 科目ｶﾅ名設定
						mfcRec.m_PrSign		= 0;					// 科目出力サイン

						mfcRec.Update();							// レコード更新
					}
					// レコード閉じる
					mfcRec.Fin();
					nRet = nKnOrder;							// 現在選択中の行の1つ下を対象とするため
				}
				// レコード閉じる
				mfcRec.Fin();
			}
		}
		catch(...)	{
			// レコード開いてる？
			if(mfcRec.IsOpen())	{
				// レコード閉じる
				mfcRec.Fin();
			}
		}
	}

	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	レコードの上下移動
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
//				nSort			…	表示順
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動あり
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::SetRecordUpDown(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort, int nUpDown)
{
	long	nRet = 0;		// 戻値
	long	nTopIndex = 0;	// トップインデックス
// 改良No.22-0404,22-0812_230414 add -->
	int						nIndex;
	ICSReportRow*			pRow;
	ICSReportSelectedRows*	pSelRows;

	// 複数行が選択されていれば、単一行選択に戻す
	pSelRows = m_report.GetSelectedRows();
	if(pSelRows->GetCount() > 1)	{
		pRow = m_report.GetFocusedRow();
		nIndex = pRow->GetIndex();
		m_report.SetUnSelectedRows();
		m_report.SetSelectedRows(nIndex, nIndex);
	}
// 改良No.22-0404,22-0812_230414 add <--

	// 参照型で分岐
	switch(nType)	{
		case ID_DLGTYPE_BANK:		// 金融機関
			// 金融機関のレコード上下移動
			nRet = UpDownKinyuukikan(pDB, nUpDown);
			break;

		case ID_DLGTYPE_ADDRESS:	// 取引先
			// 取引先のレコード上下移動
			nRet = UpDownTorihikisaki(pDB, nGrSeq, nUpDown);
			break;

		case ID_DLGTYPE_KAMOKU:		// 科目
			// 科目のレコード上下移動
			nRet = UpDownKamoku(pDB, nFormSeq, nUpDown);
			break;
	}
	
	// 移動された？
	if(nRet != 0)	{
		// リストのトップ
		nTopIndex = m_report.GetTopRowIndex();
		// リストの更新
		GetReportData(pDB, nType, nFormSeq, nGrSeq, nSort, m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_MAIN),
					  m_clsFunc.DiagGetString(&m_DiagKanaSearch, ID_DIAG_INDEX_INPUT_SUB));
		// 位置移動
		MovePosition(nTopIndex, (nRet -1));
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関のレコード上下移動
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::UpDownKinyuukikan(CDatabase* pDB, int nUpDown)
{
	long					nRet = 0;			// 戻値
	long					nIndex[2];			// リストインデックス
	CdbUcLstBank			mfcRecFrom( pDB );	// uc_lst_bankテーブルクラス
	CdbUcLstBank			mfcRecTo( pDB );	// uc_lst_bankテーブルクラス
	long					nSeq[2];			// シーケンス番号
	long					nOrderNum = 0;		// 順序番号	
	long					nMax;				// 最大行数
	
	// リストインデックス取得
	nIndex[0] = GetListIndex();
	// 移動先リストインデックス算出
	nIndex[1] = nIndex[0] + nUpDown;
	// 最大行数取得
	nMax = m_DataLineMax - 1;
	
	// 移動先が範囲外？
	if((nIndex[1] < 0) || (nIndex[1] > nMax))	{
		// 範囲外
		return(nRet);
	}

	// シーケンス番号を取得
	nSeq[0] = GetSeq(nIndex[0]);
	nSeq[1] = GetSeq(nIndex[1]);
	
	try	{
		// 指定シーケンス番号のレコード取得成功？（移動元）
		if( mfcRecFrom.RequerySeq( nSeq[0] ) == DB_ERR_OK )	{
			// レコードあり？（移動元）
			if( !( mfcRecFrom.IsEOF() ) )	{
				// 指定シーケンス番号のレコード取得成功？（移動先）
				if( mfcRecTo.RequerySeq( nSeq[1] ) == DB_ERR_OK )	{
					// レコードあり？（移動先）
					if( !( mfcRecTo.IsEOF() ) )	{
						// レコードを先頭に移動
						mfcRecFrom.MoveFirst();
						mfcRecTo.MoveFirst();
						// レコード編集開始
						mfcRecFrom.Edit();
						mfcRecTo.Edit();
						// バッファに移動元レコードを一時保存
						nOrderNum = mfcRecFrom.m_OrderNum;
						// 移動元レコードに移動先レコードの順序番号を設定
						mfcRecFrom.m_OrderNum = mfcRecTo.m_OrderNum;
						// 移動先レコードに一時保存しておいた順序番号を設定
						mfcRecTo.m_OrderNum = nOrderNum;
						// 更新
						mfcRecFrom.Update();
						mfcRecTo.Update();
						// 移動元の順序番号を戻値にセット
						nRet = nIndex[1] + 1;
					}
					// レコード閉じる（移動先）
					mfcRecTo.Fin();
				}
			}
			// レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
	}
	catch(...)	{
		// レコード開いている？（移動元）
		if( mfcRecFrom.IsOpen() )	{
			// レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
		// レコード開いている？（移動先）
		if( mfcRecTo.IsOpen() )	{
			// レコード閉じる（移動先）
			mfcRecTo.Fin();
		}
	}

	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先のレコード上下移動
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nGrSeq			…	グループ番号
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::UpDownTorihikisaki( CDatabase* pDB, int nGrSeq, int nUpDown )
{
	long					nRet = 0;			// 戻値
	long					nIndex[2];			// リストインデックス
	CdbUcLstAddress			mfcRecFrom( pDB );	// uc_lst_addressテーブルクラス
	CdbUcLstAddress			mfcRecTo( pDB );	// uc_lst_addressテーブルクラス
	long					nSeq[2];			// シーケンス番号
	long					nOrderNum = 0;		// 順序番号	
	long					nMax;				// 最大行数
	
	// リストインデックス取得
	nIndex[0] = GetListIndex();
	// 移動先リストインデックス算出
	nIndex[1] = nIndex[0] + nUpDown;
	// 最大行数取得
	nMax = m_DataLineMax - 1;
	
	// 移動先が範囲外？
	if(( nIndex[1] < 0 ) || ( nIndex[1] > nMax ))	{
		// 範囲外
		return( nRet );
	}

	// シーケンス番号を取得
	nSeq[0] = GetSeq( nIndex[0] );
	nSeq[1] = GetSeq( nIndex[1] );
	
	try	{
		// 指定シーケンス番号のレコード取得成功？（移動元）
		if( mfcRecFrom.RequeryAdSeq( nGrSeq, nSeq[0] ) == DB_ERR_OK )	{
			// レコードあり？（移動元）
			if( !( mfcRecFrom.IsEOF() ) )	{
				// 指定シーケンス番号のレコード取得成功？（移動先）
				if( mfcRecTo.RequeryAdSeq( nGrSeq, nSeq[1] ) == DB_ERR_OK )	{
					// レコードあり？（移動先）
					if( !( mfcRecTo.IsEOF() ) )	{
						// レコードを先頭に移動
						mfcRecFrom.MoveFirst();
						mfcRecTo.MoveFirst();
						// レコード編集開始
						mfcRecFrom.Edit();
						mfcRecTo.Edit();
						// バッファに移動元レコードを一時保存
						nOrderNum = mfcRecFrom.m_OrderNum;
						// 移動元レコードに移動先レコードの順序番号を設定
						mfcRecFrom.m_OrderNum = mfcRecTo.m_OrderNum;
						// 移動先レコードに一時保存しておいた順序番号を設定
						mfcRecTo.m_OrderNum = nOrderNum;
						// 更新
						mfcRecFrom.Update();
						mfcRecTo.Update();
						// 移動元の順序番号を戻値にセット
						nRet = nIndex[1] + 1;
					}
					// レコード閉じる（移動先）
					mfcRecTo.Fin();
				}
			}
			// レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
	}
	catch(...)	{
		// レコード開いている？（移動元）
		if( mfcRecFrom.IsOpen() )	{
			// レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
		// レコード開いている？（移動先）
		if( mfcRecTo.IsOpen() )	{
			// レコード閉じる（移動先）
			mfcRecTo.Fin();
		}
	}

	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	科目のレコード上下移動
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//				nUpDown			…	上下移動値（正：下方向／負：上方向）
//	【戻値】	0				…	移動なし
//				0以外			…	移動した順序番号
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::UpDownKamoku( CDatabase* pDB, short nFormSeq, int nUpDown )
{
	long					nRet = 0;			// 戻値
	long					nIndex[2];			// リストインデックス
	CdbUcLstKamoku			mfcRecFrom( pDB );	// uc_lst_kamokuテーブルクラス
	CdbUcLstKamoku			mfcRecTo( pDB );	// uc_lst_kamokuテーブルクラス
	long					nKnSeq[2];			// シーケンス番号
	long					nKnOrder = 0;		// 順序番号	
	long					nMax;				// 最大行数
	
	// リストインデックス取得
	nIndex[0] = GetListIndex();
	// 移動先リストインデックス算出
 	nIndex[1] = nIndex[0] + nUpDown;
	// 最大行数取得
	nMax = m_DataLineMax - 1;
	
	// 移動先が範囲外？
	if(( nIndex[1] < 0 ) || ( nIndex[1] > nMax ))	{
		// 範囲外
		return( nRet );
	}

	// シーケンス番号を取得
	nKnSeq[0] = GetSeq( nIndex[0] );
	nKnSeq[1] = GetSeq( nIndex[1] );
	
	try	{
		// 指定シーケンス番号のレコード取得成功？（移動元）
		if( mfcRecFrom.RequeryKnSeq( nFormSeq, nKnSeq[0] ) == DB_ERR_OK )	{
			// レコードあり？（移動元）
			if( !( mfcRecFrom.IsEOF() ) )	{
				// 指定シーケンス番号のレコード取得成功？（移動先）
				if( mfcRecTo.RequeryKnSeq( nFormSeq, nKnSeq[1] ) == DB_ERR_OK )	{
					// レコードあり？（移動先）
					if( !( mfcRecTo.IsEOF() ) )	{
						// レコードを先頭に移動
						mfcRecFrom.MoveFirst();
						mfcRecTo.MoveFirst();
						// レコード編集開始
						mfcRecFrom.Edit();
						mfcRecTo.Edit();
						// バッファに移動元レコードを一時保存
						nKnOrder = mfcRecFrom.m_KnOrder;
						// 移動元レコードに移動先レコードの順序番号を設定
						mfcRecFrom.m_KnOrder = mfcRecTo.m_KnOrder;
						// 移動先レコードに一時保存しておいた順序番号を設定
						mfcRecTo.m_KnOrder = nKnOrder;
						// 更新
						mfcRecFrom.Update();
						mfcRecTo.Update();
						// 移動元の順序番号を戻値にセット
						nRet = nIndex[1] + 1;
					}
					// レコード閉じる（移動先）
					mfcRecTo.Fin();
				}
			}
			// レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
	}
	catch(...)	{
		// レコード開いている？（移動元）
		if( mfcRecFrom.IsOpen() )	{
			// レコード閉じる（移動元）
			mfcRecFrom.Fin();
		}
		// レコード開いている？（移動先）
		if( mfcRecTo.IsOpen() )	{
			// レコード閉じる（移動先）
			mfcRecTo.Fin();
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	選択されたリストのレコード取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				nType			…	参照型
//				nFormSeq		…	様式シーケンス番号
//				nGrSeq			…	グループ番号
//	【戻値】	0				…	成功
//				0以外			…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::GetSelectRecord(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq)
{
	int						nRet = 1;		// 戻値
	int						nIndex = 0;		// リストインデックス
	long					nSeq = 0;		// シーケンス番号
	int						cnt;
	ICSReportRow*			pRow;
	ICSReportSelectedRows*	pSelRows;

	// 複数選択されている場合は、リターン
	cnt = 0;
	pSelRows = m_report.GetSelectedRows();
	for(int ii=0; ii<pSelRows->GetCount(); ii++)	{
		// 選択行(ICSReportRowクラス)を順次取得
		pRow = pSelRows->GetAt(ii);
		if(pRow == NULL)	{
			return(-1);
		}
		cnt++;
	}
	if(cnt <= 0 || cnt > 1) {
		return(-1);
	}

	// リストインデックス取得
	nIndex = GetListIndex();
	// シーケンス番号取得
	nSeq = GetSeq(nIndex);

	// 参照型で分岐
	switch(nType)	{
		case ID_DLGTYPE_BANK:		// 金融機関
			// レコード取得
			nRet = GetKinnyuukikanRecord(pDB, nSeq, &m_udBank);
			break;

		case ID_DLGTYPE_ADDRESS:	// 取引先
			// レコード取得
			nRet = GetTorihikisakiRecord(pDB, nGrSeq, nSeq, &m_udAddress);
			break;

		case ID_DLGTYPE_KAMOKU:		// 科目
			// レコード取得
			nRet = GetKamokuRecord(pDB, nFormSeq, nSeq, &m_udKamoku);
			break;
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	金融機関のレコード取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nSeq		…	シーケンス番号
//				pudBank		…	金融機関構造体
//	【戻値】	0			…	成功
//				0以外		…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::GetKinnyuukikanRecord(CDatabase* pDB, long nSeq, _RD_BANK* pudBank)
{
	int				nRet = 1;		// 戻値
	CdbUcLstBank	mfcRec(pDB);	// uc_lst_bankテーブルクラス
	
	// 初期化
	pudBank->m_Seq = 0;
	pudBank->m_OrderNum = 0;
	pudBank->m_BkName1.Empty();
	pudBank->m_BkName2.Empty();
	pudBank->m_FgDel = 0;
	pudBank->m_BkKana1.Empty();
	pudBank->m_BkKana2.Empty();

	try	{
		// 指定シーケンス番号のレコード取得成功？
		if(mfcRec.RequerySeq(nSeq) == DB_ERR_OK)	{
			// レコードあり？
			if( !(mfcRec.IsEOF()) )	{
				mfcRec.MoveFirst();							// レコードを先頭に移動
				pudBank->m_Seq		= mfcRec.m_Seq;			// シーケンス番号取得
				pudBank->m_OrderNum	= mfcRec.m_OrderNum;	// 順序番号取得
				pudBank->m_BkName1	= mfcRec.m_BkName1;		// 金融機関名取得
				pudBank->m_BkName2	= mfcRec.m_BkName2;		// 支店名取得
				pudBank->m_FgDel	= mfcRec.m_FgDel;		// 削除フラグ取得
				pudBank->m_BkKana1	= mfcRec.m_BkKana1;		// 金融機関ｶﾅ取得
				pudBank->m_BkKana2	= mfcRec.m_BkKana2;		// 支店ｶﾅ取得
				nRet = 0;									// 戻値を成功に設定
			}
			// レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...)	{
		// レコード開いてる？
		if(mfcRec.IsOpen())	{
			// レコード閉じる
			mfcRec.Fin();
		}
	}

	// 戻値を返す
	return( nRet );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	取引先のレコード取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nGrSeq		…	グループ番号
//				nAdSeq		…	シーケンス番号
//				pudAddress	…	取引先構造体
//	【戻値】	0			…	成功
//				0以外		…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::GetTorihikisakiRecord(CDatabase* pDB, int nGrSeq, long nAdSeq, _RD_ADDRESS* pudAddress)
{
	int					nRet = 1;		// 戻値
	CdbUcLstAddress		mfcRec(pDB);	// uc_lst_addressテーブルクラス
	
	// 初期化
	pudAddress->m_AdSeq = 0;
	pudAddress->m_GrSeq = 0;
	pudAddress->m_OrderNum = 0;
	pudAddress->m_AdName1.Empty();
	pudAddress->m_AdName2.Empty();
	pudAddress->m_AdAdd1.Empty();
	pudAddress->m_AdAdd2.Empty();
	pudAddress->m_FgDel = 0;
	pudAddress->m_AdKana.Empty();
	pudAddress->m_InvNo.Empty();	// インボイス登録番号追加対応_23/11/06 add

	try	{
		// 指定シーケンス番号のレコード取得成功？
		if(mfcRec.RequeryAdSeq(nGrSeq, nAdSeq) == DB_ERR_OK)	{
			// レコードあり？
			if(!(mfcRec.IsEOF()))	{
				mfcRec.MoveFirst();								// レコードを先頭に移動
				pudAddress->m_AdSeq		= mfcRec.m_AdSeq;		// シーケンス番号取得
				pudAddress->m_GrSeq		= mfcRec.m_GrSeq;		// グループ番号取得
				pudAddress->m_OrderNum	= mfcRec.m_OrderNum;	// 順序番号取得
				pudAddress->m_AdName1	= mfcRec.m_AdName1;		// 名称取得
				pudAddress->m_AdName2	= mfcRec.m_AdName2;		// 名称取得
				pudAddress->m_AdAdd1	= mfcRec.m_AdAdd1;		// 所在地取得
				pudAddress->m_AdAdd2	= mfcRec.m_AdAdd2;		// 所在地取得
				pudAddress->m_FgDel		= mfcRec.m_FgDel;		// 削除フラグ取得
				pudAddress->m_AdKana	= mfcRec.m_AdKana;		// ｶﾅ取得
				pudAddress->m_InvNo		= mfcRec.m_InvNo;		// 登録番号取得
				nRet = 0;										// 戻値を成功に設定
			}
			// レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...)	{
		// レコード開いてる？
		if(mfcRec.IsOpen()) {
			// レコード閉じる
			mfcRec.Fin();
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	科目のレコード取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				nKnSeq		…	シーケンス番号
//				pudBank		…	金融機関構造体
//	【戻値】	0			…	成功
//				0以外		…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::GetKamokuRecord(CDatabase* pDB, short nFormSeq, long nKnSeq, _RD_KAMOKU* pudKamoku)
{
	int				nRet = 1;		// 戻値
	CdbUcLstKamoku	mfcRec(pDB);	// uc_lst_bankテーブルクラス
	
	// 初期化
	pudKamoku->m_FormSeq = 0;
	pudKamoku->m_KnSeq = 0;
	pudKamoku->m_KnOrder = 0;
	pudKamoku->m_KnCode.Empty();
	pudKamoku->m_KnName.Empty();
	pudKamoku->m_FgDel = 0;
	pudKamoku->m_FgDft = 0;
	pudKamoku->m_KnKana.Empty();

	try	{
		// 指定シーケンス番号のレコード取得成功？
		if(mfcRec.RequeryKnSeq(nFormSeq, nKnSeq) == DB_ERR_OK)	{
			// レコードあり？
			if(!(mfcRec.IsEOF()))	{
				mfcRec.MoveFirst();								// レコードを先頭に移動
				pudKamoku->m_FormSeq	= mfcRec.m_FormSeq;		// シーケンス番号取得
				pudKamoku->m_KnSeq		= mfcRec.m_KnSeq;		// シーケンス番号取得
				pudKamoku->m_KnOrder	= mfcRec.m_KnOrder;		// 順序番号取得
				pudKamoku->m_KnCode		= mfcRec.m_KnCode;		// 金融機関名取得
				pudKamoku->m_KnName		= mfcRec.m_KnName;		// 支店名取得
				pudKamoku->m_FgDel		= mfcRec.m_FgDel;		// 削除フラグ取得
				pudKamoku->m_FgDft		= mfcRec.m_FgDft;		// 削除フラグ取得
				pudKamoku->m_KnKana		= mfcRec.m_KnKana;		// ｶﾅ取得
				nRet = 0;										// 戻値を成功に設定
			}
			// レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...)	{
		// レコード開いてる？
		if(mfcRec.IsOpen())	{
			// レコード閉じる
			mfcRec.Fin();
		}
	}

	// 戻値を返す
	return(nRet);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	LINP_DATA構造体初期化
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pData	…	LINP_DATA構造体ポインタ
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::InitLinpData( LINP_DATA* pData )
{
	pData->dat_attr = 0;
	pData->dat_bc = 0;
	ZeroMemory( pData->dat_bmn, sizeof( pData->dat_bmn ) );
	ZeroMemory( pData->dat_day, sizeof( pData->dat_day ) );
	pData->dat_dpn = 0;
	ZeroMemory( pData->dat_eda, sizeof( pData->dat_eda ) );
	pData->dat_fc = 0;
	pData->dat_initflg = 0;
	pData->dat_kamoku.kamoku_code = 0;  
	pData->dat_kamoku.kamoku_edaban = 0;
	ZeroMemory( pData->dat_kamoku.kamoku_name, sizeof( pData->dat_kamoku.kamoku_name ) );
	pData->dat_kamoku.kamoku_num = 0;
	ZeroMemory( pData->dat_numeric, sizeof( pData->dat_numeric ) );
	pData->dat_sel.sel_data = 0;
	pData->dat_sel.sel_num = 0;
	pData->dat_sel.sel_rslt = 0;
	pData->dat_seq = 0;
	pData->dat_string.Empty();
	ZeroMemory( pData->dat_tek.tek_data, sizeof( pData->dat_tek.tek_data ) );
	pData->dat_tek.tek_height = 0;
	pData->dat_tek.tek_image = NULL;
	pData->dat_tek.tek_width = 0;
	ZeroMemory( pData->dat_val, sizeof( pData->dat_val ) );
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	色を設定
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	nIndex	…	インデックス
//				nFont	…	文字色
//				nBack	…	背景色
//	【戻値】	なし
// -----------------------------------------------------------------------------------------------------------------------------------
void CdlgReference::SetColor(int nIndex, COLORREF nFont, COLORREF nBack)
{
	// 文字色
	m_report.SetRowTextColor(nIndex, nFont);
	// 背景色
	m_report.SetRowBackColor(nIndex, nBack);
}

// -----------------------------------------------------------------------------------------------------------------------------------
//	リストからデフォルトフラグ取得
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	nIndex	…	リストインデックス
//	【戻値】	0以外	…	デフォルトフラグ
//				0		…	失敗
// -----------------------------------------------------------------------------------------------------------------------------------
long CdlgReference::GetFgDft(int nIndex)
{
	long		nRet = 0;			// 戻値
	CString		cst=_T("");

	cst = m_report.GetCaption(nIndex, 4);
	nRet = _tstol(cst);

	// 戻値を返す
	return( nRet );
}
// 改良No.22-0404,22-0812 add <--

// 修正No.162490 del -->
//// 修正No.162437 add -->
//// -----------------------------------------------------------------------------------------------------------------------------------
////	取引先コンボボックス
//// -----------------------------------------------------------------------------------------------------------------------------------
//void CdlgReference::OnCbnSetfocusComboTorihikichange()
//{
//	long					nInx=0;
//	ICSReportRow*			pRow;
//	ICSReportRows*			pRows;
//	ICSReportSelectedRows*	pSelRows;
//
//	pRows = m_report.GetRows();
//	if(pRows != NULL)	{
//		pSelRows = m_report.GetSelectedRows();
//		if(pSelRows != NULL)	{
//			if(pSelRows->GetCount() > 1) {
//				pRow = m_report.GetFocusedRow();
//				nInx = pRow->GetIndex();
//				m_report.SetFocusedRow(nInx);
//
//				PostMessage(WM_USER_SELECTCHECK, 0, 0);
//			}
//		}
//	}
//}
//// 修正No.162437 add <--
// 修正No.162490 del <--

// 修正No.162491 add -->
// -----------------------------------------------------------------------------------------------------------------------------------
//	科目、金融機関、取引先のＭＡＸ番号を取得する
// -----------------------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB			…	データベースハンドル
//				pType		…	参照型
//				pGrSeq		…	グループ番号
//				pFormSeq	…	様式シーケンス番号
//	【戻値】	登録番号の最大値
// -----------------------------------------------------------------------------------------------------------------------------------
int CdlgReference::GetMaxOrderNo(CDatabase* pDB, int pType, int pGrSeq, short pFormSeq)
{
	int			rtNo=0;
	CString		strSQL=_T("");
	CString		strData=_T("");
	CRecordset	rs(pDB);

	switch(pType)	{
		case ID_DLGTYPE_KAMOKU:		// 科目
			strSQL.Format(_T("select max(KnOrder) from uc_lst_kamoku where FormSeq=%d"), pFormSeq);
			break;
		case ID_DLGTYPE_BANK:		// 金融機関
			strSQL.Format(_T("select max(OrderNum) from uc_lst_bank"));
			break;
		case ID_DLGTYPE_ADDRESS:	// 取引先
			strSQL.Format(_T("select max(OrderNum) from uc_lst_address where GrSeq=%d"), pGrSeq);
			break;
		default:
			break;
	}

	try		{
		// テーブルオープン
		rs.Open(CRecordset::forwardOnly, strSQL);
		if(!rs.IsEOF())	{
			strData.Empty();
			rs.GetFieldValue((short)0, strData);
		}
		rs.Close();
	}
	catch(CDBException *e)	{
		ICSMessageBox(e->m_strError);
		// 開放
		e->Delete();
		return(0);
	}

	rtNo = _tstoi(strData);

	return(rtNo);
}
// 修正No.162491 add <--
