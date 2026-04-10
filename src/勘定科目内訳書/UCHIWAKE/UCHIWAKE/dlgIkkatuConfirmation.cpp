// midori 20/10/08u del -->
// IDC_MSG_DISP3を画面から削除
// 画面の高さ、コントロールの位置を調整
// midori 20/10/08u del <--

// midori 190505 add -->
// CdlgIkkatuConfirmation.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgIkkatuConfirmation.h"

extern BOOL	bG_Kanso;

// midori 20/10/08u del -->
//extern int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec);
// midori 20/10/08u del <--

// CdlgIkkatuConfirmation ダイアログ

IMPLEMENT_DYNAMIC(CdlgIkkatuConfirmation, ICSDialog)

CdlgIkkatuConfirmation::CdlgIkkatuConfirmation(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgIkkatuConfirmation::IDD, pParent)
{

}

CdlgIkkatuConfirmation::~CdlgIkkatuConfirmation()
{
}

void CdlgIkkatuConfirmation::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_REPORT_IKKATU_CONF, m_report);
	DDX_Control(pDX, IDC_MSG_DISP1,m_Disp1);
// midori 156907 add -->
	DDX_Control(pDX, IDC_MSG_DISP2,m_Disp2);
// midori 20/10/08u del -->
	//DDX_Control(pDX, IDC_MSG_DISP3,m_Disp3);
// midori 20/10/08u del <--
	DDX_Control(pDX, IDC_MSG_DISP4,m_Disp4);
// midori 156907 add <--
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOk);
	DDX_Control(pDX, ID_CANCEL_BUTTON, m_btnCan);
}

BEGIN_MESSAGE_MAP(CdlgIkkatuConfirmation, ICSDialog)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_CUSTOM_REPORT_IKKATU_CONF, OnNMReportEditkeydownCUSTOM_REPORT_IKKATUCONF)
	ON_NOTIFY(ICS_NM_REPORT_EDITKILLFOCUS, IDC_CUSTOM_REPORT_IKKATU_CONF, OnNMReportEditkillfocusCUSTOM_REPORT_IKKATUCONF)
	ON_NOTIFY(ICS_NM_REPORT_VALUECHANGED, IDC_CUSTOM_REPORT_IKKATU_CONF, OnNMReportValuechangedCUSTOM_REPORT_IKKATUCONF)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgIkkatuConfirmation::OnBnClickedOkButton)
	ON_BN_CLICKED(ID_CANCEL_BUTTON, &CdlgIkkatuConfirmation::OnBnClickedCancelButton)
END_MESSAGE_MAP()

// CdlgIkkatuConfirmation メッセージ ハンドラー

// --------------------------------------------------------------------------------------------------------------------
//	ダイアログ表示
// --------------------------------------------------------------------------------------------------------------------
//	【引数】	pDB				…	データベースハンドル
//				pvecPrintAll	…	印刷データ
//	【戻値】	ID_DLG_OK		…	はいボタンが押された
// --------------------------------------------------------------------------------------------------------------------
int CdlgIkkatuConfirmation::ShowDialog( CDatabase* pDB, vector<PRINT_ALL_DATA>* pvecPrintAll)
{
	m_nRet = ID_DLG_CANCEL;	//	戻値初期化

	if(pDB != NULL)	{
		try	{
			if(pDB->IsOpen())	{
				m_pDB			= pDB;			// データベースハンドル取得
				m_vecPrintAll	= pvecPrintAll;

				DoModal();						// モーダルで表示
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return(m_nRet);
}

// --------------------------------------------------------------------------------------------------------------------
//	ダイアログ起動時のイベント
// --------------------------------------------------------------------------------------------------------------------
BOOL CdlgIkkatuConfirmation::OnInitDialog()
{
	ICSDialog::OnInitDialog();

// midori 156907 add -->
	ICSColorInfo	icsColInfo;
	COLORREF		color;

	// 背景色を設定
	color = GetSysColor(COLOR_BTNFACE);
	if(((ICSWinApp *)AfxGetApp())->GetStanderdColor(icsColInfo) == 0)	{
		if(icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON)	{
			color = icsColInfo.ViewColor();
		}
	}
	m_Disp1.SetBackColor(color);
	m_Disp2.SetBackColor(color);
// midori 20/10/08u del -->
	//m_Disp3.SetBackColor(color);
// midori 20/10/08u del <--
	m_Disp4.SetBackColor(color);
// midori 156907 add <--

	// レポートコントロールの初期化
	InitReportHeader();

	// レポートコントロールにレコードを追加
	AddRecords();

	// レポートのチェックボックスにデータをセット
	SetData();

	m_report.SetFocus();
	m_report.SetFocusedRow(0);

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// --------------------------------------------------------------------------------------------------------------------
//	メッセージ処理
// --------------------------------------------------------------------------------------------------------------------
BOOL CdlgIkkatuConfirmation::PreTranslateMessage(MSG* pMsg)
{
	int		nID=0;
	CWnd*	pControl = this->GetFocus();

	if(pControl != NULL)	{
		// フォーカスのあるコントロールのID取得
		nID = m_clsFunc.GetFocusControlID(this);

		if(nID == IDC_CUSTOM_REPORT_IKKATU_CONF)	{
// midori 20/10/08u del -->
//// midori 156948 add -->
//			int						row=0;
//			ICSReportSelectedRows*	pSelRows=NULL;
//			ICSReportRow*			pRow=NULL;
//			IKKATU_CONF_DATA		ikt_cnf_dt;
//
//			// 選択行インデックスの取得
//			pSelRows	= m_report.GetSelectedRows();
//			pRow		= pSelRows->GetAt(0);
//			row			= pRow->GetIndex();
//			// 現在選択中の行によって、ガイドメッセージの表示を切り替える
//			m_mapIkkatuConfData.Lookup(row, ikt_cnf_dt);
//			vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), ikt_cnf_dt.FormSeq);
//			if(it->sortEdit != 0)	m_Disp4.ShowWindow(SW_SHOW);
//			else					m_Disp4.ShowWindow(SW_HIDE);
//// midori 156948 add <--
// midori 20/10/08u del <--

			// キーが押された？
			if(pMsg->message == WM_KEYDOWN)	{
				//	パラメータで分岐
				switch(pMsg->wParam)	{
					//case VK_SPACE:
					case VK_TAB:
					case VK_RETURN:
						UpdateStateFromKey((int)pMsg->wParam);
						return(TRUE);
						break;
				}
			}
		}
		// キーが押された？
		if(pMsg->message == WM_KEYDOWN)	{
			//	パラメータで分岐
			switch(pMsg->wParam)	{
				case VK_F2:			//	F2キーが押された
					//	F2キーをShift + Tabイベントに書き換え
					keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
					keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
					keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
					keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
					return(TRUE);									//	抜ける
					break;

				case VK_RETURN:
					if(nID == ID_OK_BUTTON)	{
						OnBnClickedOkButton();
						return(TRUE);
					}
					else if(nID == ID_CANCEL_BUTTON)	{
						OnBnClickedCancelButton();
						return(TRUE);
					}
					break;

				case VK_ESCAPE:
					OnBnClickedCancelButton();
					return(TRUE);
					break;

				case VK_F5:
					OnBnClickedOkButton();
					return(TRUE);
					break;

				// コンボボックスのエディット中に←→で要素が消えないように
				case VK_RIGHT:
				case VK_LEFT:
				case VK_DELETE:
					return(TRUE);
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// --------------------------------------------------------------------------------------------------------------------
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::InitReportHeader(void)
{
	CFont*				pFont;
	LOGFONT				font={0};
	ICSReportColumn		*_pRepClm;

	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);

	_pRepClm = m_report.AddColumn(new ICSReportColumn(0, _T("様式番号"), 9));
// midori 156906 del -->
	//_pRepClm = m_report.AddColumn(new ICSReportColumn(1, _T("様式名称"), 53));
	//_pRepClm = m_report.AddColumn(new ICSReportColumn(2, _T("出力方法"), 22));
// midori 156906 del <--
// midori 156906 add -->
	_pRepClm = m_report.AddColumn(new ICSReportColumn(1, _T("様式名称"), 49));
	_pRepClm = m_report.AddColumn(new ICSReportColumn(2, _T("出力方法"), 26));
// midori 156906 add <--
	_pRepClm = m_report.AddColumn(new ICSReportColumn(3, _T("開始頁"), 8));
	_pRepClm = m_report.AddColumn(new ICSReportColumn(4, _T("終了頁"), 8));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetReportGrid();						// グリッド線の設定

	// フォント設定
	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_report.SetTextFont(font);
	m_report.SetHeaderFont(font);
}

// --------------------------------------------------------------------------------------------------------------------
//	レポートコントロール　グリッド線の設定
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::SetReportGrid(void)
{
	CDWordArray		arryGridColumn;
	CDWordArray		arrayGridColor;

	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
	arryGridColumn.Add(2);
	arryGridColumn.Add(3);
	arryGridColumn.Add(4);

	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));

	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}

// --------------------------------------------------------------------------------------------------------------------
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	なし
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::AddRecords(void)
{
	int				ii=0;
	int				max=0;
	int				fseq=0;
// midori 20/10/08u del -->
//// midori 156909 add -->
//	int				sign=0;
//// midori 156909 add <--
// midori 20/10/08u del <--
	CString			start=_T("");
	CString			end=_T("");
	CString			max_end=_T("");
	CStringArray	strArray;

	ii=0;
// midori 20/10/08u del -->
//// midori 156909 add -->
//	sign = 0;
//// midori 156909 add <--
// midori 20/10/08u del <--
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll->begin(); it != m_vecPrintAll->end(); it++)	{
		if(it->isSelect2 == TRUE && it->SortChk == 1)	{
			// 出力方法のコンボボックスの選択
			strArray.RemoveAll();
// midori 20/10/08u del -->
//// midori 156906 del -->
//			//strArray.Add(_T("詰める"));
//			//strArray.Add(_T("画面の改頁を保つ"));
//			//if(it->sortEdit != 0)	strArray.Add(_T("第１ソート項目で改頁"));
//// midori 156906 del <--
//// midori 156906 add -->
//			strArray.Add(_T("改頁･空行を詰める"));
//			strArray.Add(_T("改頁･空行を保持する"));
//			if(it->sortEdit != 0)	{
//				strArray.Add(_T("第１ソート項目で改頁する"));
//// midori 156909 add -->
//				sign = 1;
//// midori 156909 add <--
//			}
//// midori 156906 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
			strArray.Add(_T("改頁･空行を詰める"));
			strArray.Add(_T("第１ソート項目で改頁する"));
// midori 20/10/08u add <--

			ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
			pRecord->AddItem(new ICSReportRecordItemText(it->TitleNo));											// 様式番号
			pRecord->AddItem(new ICSReportRecordItemText(it->Title));											// 様式名称
			pRecord->AddItem(new ICSReportRecordItemComboBox(0, ICS_CBS_DROPDOWNLIST, &strArray));				// 出力方法
			start.Empty();
			end.Empty();
			max_end.Empty();
			if(it->isKEnable == TRUE)	{
				start.Format(_T("%d"),it->inputStart);
				end.Format(_T("%d"),it->inputEnd);
				max_end.Format(_T("%d"),it->end);

				if(it->formType == ID_FORMTYPE_NORMAL)	{
					fseq = 0;
					if(it->FormSeq == ID_FORMNO_042)		fseq = ID_FORMNO_041;
					else if(it->FormSeq == ID_FORMNO_102)	fseq = ID_FORMNO_101;
					else if(it->FormSeq == ID_FORMNO_142)	fseq = ID_FORMNO_141;
					else if(it->FormSeq == ID_FORMNO_152)	fseq = ID_FORMNO_151;
					else if(it->FormSeq == ID_FORMNO_153)	fseq = ID_FORMNO_151;
					else if(it->FormSeq == ID_FORMNO_162)	fseq = ID_FORMNO_161;
					vector<PRINT_ALL_DATA>::iterator it2 = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), fseq);
					if(it2 != m_vecPrintAll->end())	{
						start.Format(_T("%d"),it2->inputStart);
						end.Format(_T("%d"),it2->inputEnd);
						max_end.Format(_T("%d"),it2->end);
					}
				}
			}
			pRecord->AddItem(new ICSReportRecordItemText(start, ICS_EDS_NUMERIC, TRUE, 1, 4, ICS_EDS_RIGHT));	// 開始頁
			pRecord->AddItem(new ICSReportRecordItemText(end, ICS_EDS_NUMERIC, TRUE, 1, 4, ICS_EDS_RIGHT));		// 終了頁

			// 初期状態をMapに保存
			IKKATU_CONF_DATA	ikt_cnf_dt;
			ikt_cnf_dt.FormSeq = it->FormSeq;
			ikt_cnf_dt.cmb1 = it->inputCmb;
			ikt_cnf_dt.start = _tstoi(start);
			ikt_cnf_dt.end = _tstoi(end);
			ikt_cnf_dt.max_end = _tstoi(max_end);
			m_mapIkkatuConfData.SetAt(ii, ikt_cnf_dt);
			ii++;
		}
	}

	m_report.AllowEdit(TRUE);
	m_report.AllowEditColumn(ih_youshiki_no, FALSE);		// 様式番号 Edit不可
	m_report.AllowEditColumn(ih_youshiki_name, FALSE);		// 様式名称 Edit不可

	// 行の高さ
	m_report.SetItemHeight(20);

	// セットした情報でレポートを描画する
	m_report.Populate();

// midori 20/10/08u del -->
//// midori 156909 add -->
//	// ガイド表示の制御
//	// 「第１ソート項目で改頁する」の選択肢を表示する様式が一つも無かった場合は、ガイドも非表示にする
//	if(sign == 0)	{
//		m_Disp4.ShowWindow(SW_HIDE);
//	}
//	else	{
//		m_Disp4.ShowWindow(SW_SHOW);
//	}
//// midori 156909 add <--
// midori 20/10/08u del <--
}

// --------------------------------------------------------------------------------------------------------------------
//	レポートのチェックボックスにデータをセット
//	【引数】	なし
//	【戻値】	なし
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::SetData(void)
{
	int			ii=0;
	int			sel=0;

	ii=0;
	for(vector<PRINT_ALL_DATA>::iterator it = m_vecPrintAll->begin(); it != m_vecPrintAll->end(); it++)	{
		if(it->isSelect2 == TRUE && it->SortChk == 1)	{
			// 出力方法
			sel = it->inputCmb;
			m_report.SetCurSel(ii, ih_Combo, sel);

			// 開始頁/終了頁の入力制御
			if(it->isKEnable == FALSE)	{
				m_report.AllowEditItem(ii, ih_start, FALSE);
				m_report.AllowEditItem(ii, ih_end, FALSE);
				m_report.SetCellBackColor(ii, ih_start, GetSysColor( COLOR_BTNFACE ), FALSE);
				m_report.SetCellBackColor(ii, ih_end, GetSysColor( COLOR_BTNFACE ), FALSE);
			}

			ii++;
		}
	}

	m_report.RedrawControl();
}

// --------------------------------------------------------------------------------------------------------------------
//	レポート	Keydownコントロール
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::OnNMReportEditkeydownCUSTOM_REPORT_IKKATUCONF(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM*	pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//ターミネーションキーコード
	UINT						nChar		=	pItemNotify->nChar;

	UpdateStateFromKey(nChar);
}

// --------------------------------------------------------------------------------------------------------------------
//	キー操作
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::UpdateStateFromKey(int key)
{
	ICSReportSelectedRows*	pSelRows=NULL;
	ICSReportRow*			pRow=NULL;
	ICSReportRecordItem*	pItem=NULL;
	int						indexRow=0;
	int						indexClm=0;
	int						sw=0;
	int						rowmax=0;
// midori 156948 add -->
	int						row=0;
	IKKATU_CONF_DATA		ikt_cnf_dt2;
// midori 156948 add <--

	// 選択行インデックスの取得
	pSelRows	= m_report.GetSelectedRows();
	if(pSelRows == NULL)	return;

	pRow		= pSelRows->GetAt(0);
	if(pRow == NULL)		return;

	indexRow	= pRow->GetIndex();
// midori 156948 add -->
	row = indexRow;
// midori 156948 add <--

	// 選択カラムインデックスの取得
	pItem		= m_report.GetActiveItem();

	// レポートの行数を取得
	rowmax		= m_report.GetRows()->GetCount();

	switch(key)	{
		case VK_RETURN:
			if(pItem != NULL)	{
				indexClm = pItem->GetIndex();
				if(indexClm == ih_Combo)	{		// 「出力方法」→「開始頁」
					IKKATU_CONF_DATA	ikt_cnf_dt;
					m_mapIkkatuConfData.Lookup(indexRow, ikt_cnf_dt);
					vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), ikt_cnf_dt.FormSeq);
					if(it != m_vecPrintAll->end())	{
						if(it->isKEnable == FALSE)	{
							if(indexRow != (rowmax-1))	{	// 最終行以外
								m_report.MoveDown(FALSE, FALSE);
								m_report.EditItem(indexRow+1, ih_Combo, TRUE);
// midori 156948 add -->
								row = indexRow + 1;
// midori 156948 add <--
							}
							else{	// 最終行　→ＯＫボタンへ
								m_btnOk.SetFocus();
							}
						}
						else	{
							m_report.EditItem(indexRow, ih_start, TRUE);
						}
					}
				}
				else if(indexClm == ih_start)	{	// 「開始頁」→「終了頁」
					m_report.EditItem(indexRow, ih_end, TRUE);
				}
				else if(indexClm == ih_end)	{		// 「終了頁」→次の行へ
					if(indexRow != (rowmax-1))	{	// 最終行以外
						m_report.MoveDown(FALSE, FALSE);
						m_report.EditItem(indexRow+1, ih_Combo, TRUE);
// midori 156948 add -->
						row = indexRow + 1;
// midori 156948 add <--
					}
					else	{	// 最終行　→ＯＫボタンへ
						m_btnOk.SetFocus();
					}
				}
			}
			else{
				m_report.EditItem(indexRow, ih_Combo, TRUE);
			}
			break;

		case VK_TAB:
			char	cShift = ( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得
			// SHIFTキーが押されてる？
			if( cShift & 0x80 )	PrevDlgCtrl();
			else				NextDlgCtrl();
			break;
	}

// midori 20/10/08u del -->
//// midori 156948 add -->
//	// 選択中の行によって、ガイドメッセージの表示を切り替える
//	m_mapIkkatuConfData.Lookup(row, ikt_cnf_dt2);
//	vector<PRINT_ALL_DATA>::iterator it2 = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), ikt_cnf_dt2.FormSeq);
//	if(it2->sortEdit != 0)	m_Disp4.ShowWindow(SW_SHOW);
//	else					m_Disp4.ShowWindow(SW_HIDE);
//// midori 156948 add <--
// midori 20/10/08u del <--
}

// --------------------------------------------------------------------------------------------------------------------
//	レポート	ValueChangedコントロール
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::OnNMReportValuechangedCUSTOM_REPORT_IKKATUCONF(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM*	pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*			pColumn		=	pItemNotify->pColumn;
	int							indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*				pRow		=	pItemNotify->pRow;
	int							indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*		pItem		=	pItemNotify->pItem;
	int							indexItem	=	pItem->GetIndex();
	//変更のあった位置
	POINT						pt			=	pItemNotify->pt;

	int							sel=0;
	IKKATU_CONF_DATA			ikt_cnf_dt;

	if(!(indexColumn == ih_Combo))	return;

	// 変更された場合、開始頁と終了頁を再セット
	if(CheckComboDt(indexRow, indexColumn) != 0)	{
		return;
	}
}

// --------------------------------------------------------------------------------------------------------------------
// 出力方法コンボボックスが変更された場合、開始頁と終了頁を再セット
// --------------------------------------------------------------------------------------------------------------------
int CdlgIkkatuConfirmation::CheckComboDt(int row, int clm)
{
	int					sel1=0,sel2=0;
	int					nPageMax=0;
	int					nRangaiMax=0;
	int					formseq=0;
	int					formseq2=0;
	int					formtype=0;
	int					sel_sb=0;
	int					page=0;
	int					ii=0;
	int					jj=0;
	int					idx=0;
	int					sign=0;
	CString				cst=_T("");
	IKKATU_CONF_DATA	ikt_cnf_dt;
	CprtJobFunction		prtJob;
	CdbUcInfSub			mfcRec(m_pDB);

	// 画面で選択された値を取得
	sel1 = m_report.GetCurSel(row, clm);

	// チェック用の値を取得
	m_mapIkkatuConfData.Lookup(row, ikt_cnf_dt);
	sel2 = ikt_cnf_dt.cmb1;

	// 変更された
	if(sel1 != sel2)	{
		formseq = ikt_cnf_dt.FormSeq;
		if(mfcRec.RequeryFormSeq(formseq) == DB_ERR_OK)	{
			if(!(mfcRec.IsEOF()))	{
				vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), formseq);
				if(it == m_vecPrintAll->end())	{
					return(-1);
				}
				formtype = it->formType;

				sign = 0;
				if(it->isKEnable == FALSE)	{
					sign = 1;
				}
				if(sign == 0)	{
					// 最大ページ数を再取得
					nPageMax = prtJob.GetDbDataPageAll(formseq, m_pDB, prtJob.GetOutZero(mfcRec), -1, 0, sel1);

					// 欄外登録の複数ページ対応により、欄外登録のページ数も考慮する
					if(formseq == ID_FORMNO_091)	{
						CdbUc092Kaikakekin2		mfcRec2(m_pDB);
						nRangaiMax = mfcRec2.GetPrintPageNum(prtJob.GetOutZero(mfcRec));
						if(nRangaiMax > nPageMax)	nPageMax = nRangaiMax;
					}

					// 出力形式「通常」
					if(formtype == ID_FORMTYPE_NORMAL)	{
						for(ii=0;ii<2;ii++) {
							formseq2 = 0;
							switch(formseq) {
							case ID_FORMNO_041:
								if(ii==0)	formseq2 = ID_FORMNO_042;
								break;
							case ID_FORMNO_042:
								if(ii==0)	formseq2 = ID_FORMNO_041;
								break;
							case ID_FORMNO_101:
								if(ii==0)	formseq2 = ID_FORMNO_102;
								break;
							case ID_FORMNO_102:
								if(ii==0)	formseq2 = ID_FORMNO_101;
								break;
							case ID_FORMNO_141:
								if(ii==0)	formseq2 = ID_FORMNO_142;
								break;
							case ID_FORMNO_142:
								if(ii==0)	formseq2 = ID_FORMNO_141;
								break;
							case ID_FORMNO_151:
								if(ii==0)	formseq2 = ID_FORMNO_152;
								else		formseq2 = ID_FORMNO_153;
								break;
							case ID_FORMNO_152:
								if(ii==0)	formseq2 = ID_FORMNO_151;
								else		formseq2 = ID_FORMNO_153;
								break;
							case ID_FORMNO_153:
								if(ii==0)	formseq2 = ID_FORMNO_151;
								else		formseq2 = ID_FORMNO_152;
								break;
							case ID_FORMNO_161:
								if(ii==0)	formseq2 = ID_FORMNO_162;
								break;
							case ID_FORMNO_162:
								if(ii==0)	formseq2 = ID_FORMNO_161;
								break;
							}
							if(formseq2 == 0) break;

							// －２、－３の最大ページ数も再取得
							vector<PRINT_ALL_DATA>::iterator it2 = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), formseq2);
							if(it2->isSelect2 == TRUE && it2->SortChk == 1)	{
								// 確認ダイアログに上がっている場合は、該当行のマップからデータを取得
								IKKATU_CONF_DATA	iktcnfdt_sb;
								for(jj=0; jj<(int)m_mapIkkatuConfData.GetCount(); jj++)	{
									m_mapIkkatuConfData.Lookup(jj, iktcnfdt_sb);
									if(iktcnfdt_sb.FormSeq == formseq2)	{
										sel_sb = iktcnfdt_sb.cmb1;
										break;
									}
								}
							}
							else	{
								// 確認ダイアログに上がっていない場合は、ベクターから出力方法を取得
								sel_sb = it2->inputCmb;
							}
							mfcRec.RequeryFormSeq(formseq2);
							page = prtJob.GetDbDataPageAll(formseq2, m_pDB, prtJob.GetOutZero(mfcRec), -1, 0, sel_sb);
							if(page > nPageMax)	nPageMax = page;
						}

					}

					// マップに上書き
					ikt_cnf_dt.cmb1 = sel1;
					ikt_cnf_dt.start = 1;
					ikt_cnf_dt.end = nPageMax;
					ikt_cnf_dt.max_end = nPageMax;
					m_mapIkkatuConfData.SetAt(row, ikt_cnf_dt);

					// 画面再描画
					m_report.SetCurSel(row, ih_Combo, ikt_cnf_dt.cmb1);
					cst.Format(_T("%d"),ikt_cnf_dt.start);
					m_report.SetCaption(row, ih_start, cst);
					cst.Format(_T("%d"),ikt_cnf_dt.end);
					m_report.SetCaption(row, ih_end, cst);
					m_report.RedrawControl();
				}
				else	{
					// 最大ページ数を再取得
					nPageMax = prtJob.GetDbDataPageAll(formseq, m_pDB, prtJob.GetOutZero(mfcRec), -1, 0, sel1);

					// 一旦選択行のマップに上書き
					ikt_cnf_dt.cmb1 = sel1;
					ikt_cnf_dt.start = 1;
					ikt_cnf_dt.end = nPageMax;
					ikt_cnf_dt.max_end = nPageMax;
					m_mapIkkatuConfData.SetAt(row, ikt_cnf_dt);

					idx = -1;
					for(ii=0;ii<2;ii++) {
						formseq2 = 0;
						switch(formseq) {
						case ID_FORMNO_042:
							if(ii==0)	formseq2 = ID_FORMNO_041;
							break;
						case ID_FORMNO_102:
							if(ii==0)	formseq2 = ID_FORMNO_101;
							break;
						case ID_FORMNO_142:
							if(ii==0)	formseq2 = ID_FORMNO_141;
							break;
						case ID_FORMNO_152:
							if(ii==0)	formseq2 = ID_FORMNO_151;
							else		formseq2 = ID_FORMNO_153;
							break;
						case ID_FORMNO_153:
							if(ii==0)	formseq2 = ID_FORMNO_151;
							else		formseq2 = ID_FORMNO_152;
							break;
						case ID_FORMNO_162:
							if(ii==0)	formseq2 = ID_FORMNO_161;
							break;
						}
						if(formseq2 == 0) break;

						// －２、－３の最大ページ数も再取得
						vector<PRINT_ALL_DATA>::iterator it2 = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), formseq2);
						if(it2->isSelect2 == TRUE && it2->SortChk == 1)	{
							// 確認ダイアログに上がっている場合は、該当行のマップからデータを取得
							IKKATU_CONF_DATA	iktcnfdt_sb;
							for(jj=0; jj<(int)m_mapIkkatuConfData.GetCount(); jj++)	{
								m_mapIkkatuConfData.Lookup(jj, iktcnfdt_sb);
								if(iktcnfdt_sb.FormSeq == formseq2)	{
									sel_sb = iktcnfdt_sb.cmb1;
									if(idx == -1)	idx = jj;
									break;
								}
							}
						}
						else	{
							// 確認ダイアログに上がっていない場合は、ベクターから出力方法を取得
							sel_sb = it2->inputCmb;
						}
						mfcRec.RequeryFormSeq(formseq2);
						page = prtJob.GetDbDataPageAll(formseq2, m_pDB, prtJob.GetOutZero(mfcRec), -1, 0, sel_sb);
						if(page > nPageMax)	nPageMax = page;
					}

					// ----

					// マップに上書き
					IKKATU_CONF_DATA	dt_sb;
					m_mapIkkatuConfData.Lookup(idx, dt_sb);
					dt_sb.start = 1;
					dt_sb.end = nPageMax;
					dt_sb.max_end = nPageMax;
					m_mapIkkatuConfData.SetAt(idx, dt_sb);

					// 画面再描画
					m_report.SetCurSel(idx, ih_Combo, dt_sb.cmb1);
					cst.Format(_T("%d"),dt_sb.start);
					m_report.SetCaption(idx, ih_start, cst);
					cst.Format(_T("%d"),dt_sb.end);
					m_report.SetCaption(idx, ih_end, cst);
					// 
					m_report.RedrawControl();
				}

			}
			mfcRec.Fin();
		}
	}

	return(0);
}

// --------------------------------------------------------------------------------------------------------------------
//	レポート	KillFocusコントロール
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::OnNMReportEditkillfocusCUSTOM_REPORT_IKKATUCONF(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM*	pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*			pColumn		=	pItemNotify->pColumn;
	int							indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*				pRow		=	pItemNotify->pRow;
	int							indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*		pItem		=	pItemNotify->pItem;
	int							indexItem	=	pItem->GetIndex();

	CString						strPage = _T("");
	IKKATU_CONF_DATA			ikt_cnf_dt;

	if(!(indexColumn == ih_start || indexColumn == ih_end || indexColumn == ih_Combo))	return;

	if(indexColumn == ih_Combo)	{
		// 出力方法
		// 変更された場合、開始頁と終了頁を再セット
		if(CheckComboDt(indexRow, indexColumn) != 0)	{
			return;
		}
	}
	else if(indexColumn == ih_start || indexColumn == ih_end)	{
		// 開始頁 / 終了頁
		// 頁数をチェック
		// 不正な頁数が入力されたらセットし直す
		if(CheckPageNum(indexRow, indexColumn) != 0)	{
			return;
		}

		// チェック用の変数に入力された頁数を保存
		m_mapIkkatuConfData.Lookup(indexRow, ikt_cnf_dt);
		if(indexColumn == ih_start)	{
			strPage = m_report.GetCaption(indexRow, ih_start);
			ikt_cnf_dt.start = _tstoi(strPage);
		}
		if(indexColumn == ih_end)	{
			strPage = m_report.GetCaption(indexRow, ih_end);
			ikt_cnf_dt.end = _tstoi(strPage);
		}
		m_mapIkkatuConfData.SetAt(indexRow, ikt_cnf_dt);
	}
}

// --------------------------------------------------------------------------------------------------------------------
//	入力されたページ数が範囲内かどうか全列チェック
// --------------------------------------------------------------------------------------------------------------------
int CdlgIkkatuConfirmation::CheckPageNum_All(void)
{
	int					ret=0;
	int					max_cnt=0;
	int					row=0;
	int					nP1=0,nP2=0,nP3=0;
	CString				strPage=_T("");
	ICSReportRecords*	records;

	records = m_report.GetRecords();
	if(records == NULL)	return(-1);

	ret = 0;
	max_cnt = records->GetCount();
	for(row=0; row<max_cnt; row++)	{
		// 開始頁 ページ数チェック
		if(CheckPageNum(row, ih_start) != 0)	{
			ret = -1;
			break;
		}
		// 終了頁 ページ数チェック
		if(CheckPageNum(row, ih_end) != 0)	{
			ret = -1;
			break;
		}
	}

	return(ret);
}

// --------------------------------------------------------------------------------------------------------------------
//	入力されたページ数が範囲内かどうかチェック
// --------------------------------------------------------------------------------------------------------------------
int CdlgIkkatuConfirmation::CheckPageNum(int row, int clm)
{
	int					page=0, pmax=0;
	int					nP1=0,nP2=0;
	CString				strPage=_T("");

	// 最大頁数を取得
	IKKATU_CONF_DATA	ikt_cnf_dt;
	m_mapIkkatuConfData.Lookup(row, ikt_cnf_dt);

	vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), ikt_cnf_dt.FormSeq);

	// 開始頁/終了頁が有効の列のみチェックする
	if(it->isKEnable == FALSE)	return(0);

	pmax = ikt_cnf_dt.max_end;

	// 開始頁、終了頁とも範囲を超えていないか
	strPage = m_report.GetCaption(row, clm);
	page = _tstoi(strPage);

	if(page < 1 || page > pmax)	{
		strPage.Empty();
		if(clm == ih_start)	{
			// 保存していた値を再セット
			strPage.Format(_T("%d"), ikt_cnf_dt.start);
			m_report.SetCaption(row, clm, strPage);
		}
		if(clm == ih_end)	{
			strPage.Format(_T("%d"), ikt_cnf_dt.end);
			m_report.SetCaption(row, clm, strPage);
		}
		m_report.RedrawControl();
		return(-1);
	}

	// ----

	// 開始頁をチェック
	if(clm == ih_start)	{
		strPage = m_report.GetCaption(row, clm);
		nP1 = _tstoi(strPage);

		strPage = m_report.GetCaption(row, ih_end);
		nP2 = _tstoi(strPage);

		// 開始頁＞終了頁になっている
		if(nP1 > nP2)	{
			// 終了頁に開始頁をセット
			ikt_cnf_dt.end = nP1;
			m_mapIkkatuConfData.SetAt(row, ikt_cnf_dt);
			strPage.Empty();
			strPage.Format(_T("%d"), nP1);
			m_report.SetCaption(row, ih_end, strPage);
			m_report.RedrawControl();
		}
	}
	// 終了頁をチェック
	if(clm == ih_end)	{
		strPage = m_report.GetCaption(row, ih_start);
		nP1 = _tstoi(strPage);

		strPage = m_report.GetCaption(row, clm);
		nP2 = _tstoi(strPage);

		// 開始頁＞終了頁になっている
		if(nP1 > nP2)	{
			strPage.Empty();
			strPage.Format(_T("%d"), ikt_cnf_dt.end);
			m_report.SetCaption(row, clm, strPage);
			m_report.RedrawControl();
			return(-1);
		}
	}

	// 入力値を再セット
	strPage.Empty();
	strPage.Format(_T("%d"), page);
	m_report.SetCaption(row, clm, strPage);

	return(0);
}

// --------------------------------------------------------------------------------------------------------------------
//	レポートの状態をマップ→ベクターに更新
// --------------------------------------------------------------------------------------------------------------------
//	【引数】	なし
//	【返送値】	なし
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::UpdateIkkatuConfMap(void)
{
	int					max_cnt=0;
	int					ii=0;
	int					flg1=0;
	int					fseq=0;
	int					fseq2=0;
	int					ftype=0;
	CString				st=_T(""),ed=_T(""),max_end=_T("");
// No.200601 del -->
	//ICSReportRecords*	records;
// No.200601 del <--
	IKKATU_CONF_DATA	ikkatu_conf_data;

	max_cnt = (int)m_mapIkkatuConfData.GetCount();

	for(ii=0; ii<max_cnt; ii++)	{
		m_mapIkkatuConfData.Lookup(ii, ikkatu_conf_data);

		fseq = ikkatu_conf_data.FormSeq;

		vector<PRINT_ALL_DATA>::iterator it = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), fseq);
		if(it != m_vecPrintAll->end())	{
			ftype = it->formType;
		}

		// 0:個別タイプ または 通常タイプで２様式以上のセットではない または ２様式以上の先頭
		// 1:通常タイプで２様式以上の -2 または -3 で、-1 が確認ダイアログに表示されていない
		// 2:通常タイプで２様式以上の -2 または -3 で、-1 が確認ダイアログに表示されている
		flg1 = 0;
		if((fseq == ID_FORMNO_042 || fseq == ID_FORMNO_102 ||
			fseq == ID_FORMNO_142 || fseq == ID_FORMNO_152 ||
			fseq == ID_FORMNO_153 || fseq == ID_FORMNO_162) && ftype == ID_FORMTYPE_NORMAL)	{
			// -2,-3 様式の場合、確認ダイアログに -1 様式が表示されていなければ
			if(it->isKEnable == TRUE)	{
				flg1 = 1;
			}
			else	{
				flg1 = 2;
			}
		}
		// 0:it にすべての情報を書き込む
		if(flg1 == 0)	{
			it->inputCmb = ikkatu_conf_data.cmb1;
			st.Empty();
			ed.Empty();
			max_end.Empty();
			st.Format(_T("%d"),ikkatu_conf_data.start);
			ed.Format(_T("%d"),ikkatu_conf_data.end);
			max_end.Format(_T("%d"),ikkatu_conf_data.max_end);
			it->inputStart = _tstoi(st);
			it->inputEnd = _tstoi(ed);
			it->end = _tstoi(max_end);
		}
		// 1:it に出力方法を書き込む、-1 の it に頁を書き込む
		else if(flg1 == 1)	{
			it->inputCmb = ikkatu_conf_data.cmb1;
			if(fseq == ID_FORMNO_042)							fseq = ID_FORMNO_041;
			if(fseq == ID_FORMNO_102)							fseq = ID_FORMNO_101;
			if(fseq == ID_FORMNO_152 || fseq == ID_FORMNO_153)	fseq = ID_FORMNO_151;
			if(fseq == ID_FORMNO_142)							fseq = ID_FORMNO_141;
			if(fseq == ID_FORMNO_162)							fseq = ID_FORMNO_161;
			it = find(m_vecPrintAll->begin(), m_vecPrintAll->end(), fseq);
			st.Empty();
			ed.Empty();
			max_end.Empty();
			st.Format(_T("%d"),ikkatu_conf_data.start);
			ed.Format(_T("%d"),ikkatu_conf_data.end);
			max_end.Format(_T("%d"),ikkatu_conf_data.max_end);
			it->inputStart = _tstoi(st);
			it->inputEnd = _tstoi(ed);
			it->end = _tstoi(max_end);
		}
		// 2:it に出力方法のみを書き込む
		else	{
			it->inputCmb = ikkatu_conf_data.cmb1;
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------
//	ボタンクリック（OKボタン）
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::OnBnClickedOkButton()
{
	m_nRet = ID_DLG_OK;

	// レポートのKillFocusでチェックしているが、念のためここでもページ数を確認
	if(CheckPageNum_All() != 0)	{
		ICSMessageBox(_T("出力データがありません。"), MB_OK, 0, 0, this);
		return;
	}

	// レポートの状態をマップ→ベクターに更新
	UpdateIkkatuConfMap();

	ICSDialog::OnOK();
}

// --------------------------------------------------------------------------------------------------------------------
//	ボタンクリック（キャンセルボタン）
// --------------------------------------------------------------------------------------------------------------------
void CdlgIkkatuConfirmation::OnBnClickedCancelButton()
{
	m_nRet = ID_DLG_CANCEL;

	ICSDialog::OnCancel();
}
// midori 190505 add <--
