// dlgIkkatuHenkou.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgIkkatuHenkou.h"


// CdlgIkkatuHenkou ダイアログ

IMPLEMENT_DYNAMIC(CdlgIkkatuHenkou, ICSDialog)

CdlgIkkatuHenkou::CdlgIkkatuHenkou(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgIkkatuHenkou::IDD, pParent)
{

}

CdlgIkkatuHenkou::~CdlgIkkatuHenkou()
{
}

void CdlgIkkatuHenkou::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_ZEROCHK_BUTTON, m_btnZeroChk);
	DDX_Control(pDX, ID_NULLCHK_BUTTON, m_btnNullChk);
	DDX_Control(pDX, ID_ZEROPRCHK_BUTTON, m_btnZeroPrChk);
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CUSTOM_REPORT_IKKATU_HENKOU, m_report);
}


BEGIN_MESSAGE_MAP(CdlgIkkatuHenkou, ICSDialog)
	ON_BN_CLICKED(ID_ZEROCHK_BUTTON, &CdlgIkkatuHenkou::OnBnClickedZerochkButton)
	ON_BN_CLICKED(ID_NULLCHK_BUTTON, &CdlgIkkatuHenkou::OnBnClickedNullchkButton)
	ON_BN_CLICKED(ID_ZEROPRCHK_BUTTON, &CdlgIkkatuHenkou::OnBnClickedZeroprchkButton)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgIkkatuHenkou::OnBnClickedOkButton)

	ON_NOTIFY(ICS_NM_REPORT_CHECKED, IDC_CUSTOM_REPORT_IKKATU_HENKOU, OnNMReportCheckedCUSTOM_REPORT_IKKATU_HENKOU)
END_MESSAGE_MAP()


// CdlgIkkatuHenkou メッセージ ハンドラ

//+++++++++++++++++++++++++++++++++++++++++++
// ダイアログ起動時のイベント
//+++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgIkkatuHenkou::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	InitReportHeader();

	CreateIkkatuHenkouVec();

	AddRecords();

	SetData();

	m_report.SetFocus();
	m_report.SetFocusedRow(0);

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//**************************************************
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgIkkatuHenkou::InitReportHeader()
{
	int				iHeight;
	ICSReportColumn	*_pRepClm;

	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);

	_pRepClm = m_report.AddColumn(new ICSReportColumn(0, _T("様式番号"), 10));
	_pRepClm = m_report.AddColumn(new ICSReportColumn(1, _T("様式名称"), 45));
	_pRepClm = m_report.AddColumn(new ICSReportColumn(2, _T("０円データを\r\n出力しない"), 15));
	_pRepClm->SetAlignment( DT_WORDBREAK );	// アライメントの設定
	_pRepClm = m_report.AddColumn(new ICSReportColumn(3, _T("空欄データを\r\n出力しない"), 15));
	_pRepClm->SetAlignment( DT_WORDBREAK );	// アライメントの設定
	_pRepClm = m_report.AddColumn(new ICSReportColumn(4, _T("０円を印字する"), 15));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	// フォント設定
	CFont*		pFont;
	LOGFONT		font = {0};

	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_report.SetTextFont(font);
	m_report.SetHeaderFont(font);

	// 高さの変更
	iHeight = m_report.GetHeaderHeight();
	m_report.SetHeaderHeight( iHeight + iHeight / 2 );
}

//**************************************************
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	TRUE:成功
//				FALSE:失敗
//**************************************************
int CdlgIkkatuHenkou::AddRecords()
{
	for(vector<IKKATU_HENKOU_DATA>::iterator it = m_vecIkkatuHenkouData.begin(); it != m_vecIkkatuHenkouData.end(); it++){
		ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
		pRecord->AddItem( new ICSReportRecordItemText(it->TitleNo));	// 様式番号
		pRecord->AddItem( new ICSReportRecordItemText(it->Title));		// 様式名称
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// ０円ﾃﾞｰﾀを出力しない
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 空欄ﾃﾞｰﾀを出力しない
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// ０円を印字する
	}
	// 行の高さ
	m_report.SetItemHeight(20);

	return TRUE;
}

//**************************************************
//	レコードのチェックボックスにデータセット
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgIkkatuHenkou::SetData()
{
	int			row=0;

	for(vector<IKKATU_HENKOU_DATA>::iterator it = m_vecIkkatuHenkouData.begin(); it != m_vecIkkatuHenkouData.end(); it++, row++){
		m_report.SetChecked(row, ih_0henkou,(*it).is0_henkou);
		m_report.SetChecked(row, ih_null_henkou,(*it).isNULL_henkou);
		m_report.SetChecked(row, ih_0prhenkou,(*it).is0pr_henkou);

		if((*it).is0_henkou == TRUE || m_nOutKingaku != 0)	{
			m_report.SetCellBackColor(row,ih_0prhenkou,GetSysColor( COLOR_BTNFACE ),TRUE);
			m_report.AllowEditItem(row, ih_0prhenkou, FALSE);
		}
		else	{
			m_report.SetCellBackColor(row,ih_0prhenkou,RGB(255, 255, 255),TRUE);
			m_report.AllowEditItem(row, ih_0prhenkou, TRUE);
		}
	}
	// 共通印刷設定の「金額欄を空欄で出力する」にチェックが付いている場合は
	// 全行の「金額０円を印字する」が選択不可になる
	if(m_nOutKingaku != 0)	{
		m_btnZeroPrChk.EnableWindow(FALSE);
	}
}

void CdlgIkkatuHenkou::CreateIkkatuHenkouVec()
{
	CdbUcInfSub	mfcRec(m_pDB);
	mfcRec.InitOrder();
	mfcRec.MoveFirst();
	while(!mfcRec.IsEOF()){
		IKKATU_HENKOU_DATA	ikkatu_henkou_data;
		ikkatu_henkou_data.FormSeq = mfcRec.m_FormSeq;
		ikkatu_henkou_data.TitleNo = mfcRec.m_TitleNo + mfcRec.m_TitleNo2;
		ikkatu_henkou_data.Title = mfcRec.m_Title;

		CCtrlSubGeneralVar cs( mfcRec );

		//************************************
		// 「金額０円データを出力しない」
		//************************************
		if(cs.bPrintZeroData)	ikkatu_henkou_data.is0_henkou = TRUE;
		else					ikkatu_henkou_data.is0_henkou = FALSE;

		//************************************
		// 「金額空欄データを出力しない」
		//************************************
		if(cs.bPrintNullData)	ikkatu_henkou_data.isNULL_henkou = TRUE;
		else					ikkatu_henkou_data.isNULL_henkou = FALSE;

		//************************************
		// 「金額０円を印字する」（0でOFF　0以外でON）
		//************************************
		if(cs.bPrintHideZero)	ikkatu_henkou_data.is0pr_henkou = FALSE;
		else					ikkatu_henkou_data.is0pr_henkou = TRUE;

		// ベクタの最後尾に追加
		m_vecIkkatuHenkouData.push_back(ikkatu_henkou_data);
		// 次の様式へ
		mfcRec.MoveNext();
	}
}

void CdlgIkkatuHenkou::OnNMReportCheckedCUSTOM_REPORT_IKKATU_HENKOU(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//チェックされたカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//チェックされた行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//チェックされたアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();
	BOOL	bCheck	=	pItem->IsChecked();	//チェック状況

	if(indexColumn == ih_0henkou)	{
		if(bCheck == TRUE || m_nOutKingaku != 0)	{
			m_report.SetCellBackColor(indexRow,ih_0prhenkou,GetSysColor( COLOR_BTNFACE ),TRUE);
			m_report.AllowEditItem(indexRow, ih_0prhenkou, FALSE);
		}
		else	{
			m_report.SetCellBackColor(indexRow,ih_0prhenkou,RGB(255, 255, 255),TRUE);
			m_report.AllowEditItem(indexRow, ih_0prhenkou, TRUE);
		}
		m_report.RedrawControl();
	}
}

void CdlgIkkatuHenkou::UpdateStateFromKey(int key)
{
	int		indexClm;
	int		sw=0;

	if(m_clsFunc.GetFocusControlID(this) != IDC_CUSTOM_REPORT_ZERO_CLEAR)	return;

	ICSReportSelectedRows* s_rows = m_report.GetSelectedRows();
	if(s_rows == NULL)	return;
	ICSReportRow* r_row = s_rows->GetAt(0);
	if(r_row == NULL)	return;
	int indexRow = r_row->GetIndex();
	ICSReportRecordItem	*pItem	= m_report.GetActiveItem();

	switch(key)	{

		case VK_SPACE:
			if(pItem != NULL){
				indexClm = pItem->GetIndex();
				if(indexClm == ih_0henkou || indexClm == ih_null_henkou || indexClm == ih_0prhenkou){
					sw=0;
					if(indexClm == ih_0prhenkou)	{
						if(m_report.IsChecked(indexRow,ih_0henkou) == TRUE)	{
							sw=1;
						}
					}
					if(sw == 0)	{
						BOOL check2 = !m_report.IsChecked(indexRow, indexClm);
						m_report.SetChecked(indexRow, indexClm, check2);
						m_report.AllowEditItem( indexRow, indexClm, TRUE );	// この3行を入れないとVK_SPACE後Editが抜けてしまう

						if(indexClm == ih_0henkou)	{
							if(check2 == TRUE || m_nOutKingaku != 0)	{
								m_report.SetCellBackColor(indexRow,ih_0prhenkou,GetSysColor( COLOR_BTNFACE ),TRUE);
								m_report.AllowEditItem(indexRow, ih_0prhenkou, FALSE);
							}
							else	{
								m_report.SetCellBackColor(indexRow,ih_0prhenkou,RGB(255, 255, 255),TRUE);
								m_report.AllowEditItem(indexRow, ih_0prhenkou, TRUE);
							}
						}
						m_report.Populate();
						m_report.EditItem( indexRow, indexClm, TRUE );
					}
				}
			}
			break;

		case VK_RETURN:
			if(pItem != NULL){	// エディットが入っている→次のカラムにエディット
				int indexClm = pItem->GetIndex();
				if(indexClm == ih_0henkou)	{
					indexClm = ih_null_henkou;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
				else if(indexClm == ih_null_henkou)	{
// midori 152169 del -->
					//indexClm = ih_0prhenkou;
					//m_report.EditItem(indexRow, indexClm, TRUE);
// midori 152169 del <--
// midori 152169 del -->
					// 0円データを出力しないにチェック有り
					if(m_report.IsChecked(indexRow,ih_0henkou) == TRUE)	{
						MoveDown(indexRow);
						ICSReportRecords * records = m_report.GetRecords();
						if(indexRow+1<m_report.GetRecords()->GetCount())	m_report.EditItem( indexRow + 1, ih_0henkou, TRUE );
					}
					else	{
						indexClm = ih_0prhenkou;
						m_report.EditItem(indexRow, indexClm, TRUE);
					}
// midori 152169 del <--
				}
				else if(indexClm == ih_0prhenkou)	{
					MoveDown(indexRow);
					ICSReportRecords * records = m_report.GetRecords();
					int max_cnt = records->GetCount();
					if(indexRow+1<max_cnt)		m_report.EditItem( indexRow + 1, ih_0henkou, TRUE );
				}
			}
			else{	// エディットが入っていない→選択カラムにエディット
				m_report.EditItem( indexRow, ih_0henkou, TRUE );
			}
			break;
	}
}

void CdlgIkkatuHenkou::UpdateZeroClearData()
{
	ICSReportRecords * records = m_report.GetRecords();
	if(records == NULL)	return;
	int max_cnt = records->GetCount();

	for(int row=0; row<max_cnt; row++){
		m_vecIkkatuHenkouData[row].is0_henkou = m_report.IsChecked(row, ih_0henkou);
		m_vecIkkatuHenkouData[row].isNULL_henkou = m_report.IsChecked(row, ih_null_henkou);
		m_vecIkkatuHenkouData[row].is0pr_henkou = m_report.IsChecked(row, ih_0prhenkou);
	}
}

void CdlgIkkatuHenkou::SetGrid()
{
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
	arryGridColumn.Add(2);
	arryGridColumn.Add(3);

	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));

	m_report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}

void CdlgIkkatuHenkou::MoveDown(int cur_row)
{
	int cnt = 0;
	for(vector<IKKATU_HENKOU_DATA>::iterator it = m_vecIkkatuHenkouData.begin();it != m_vecIkkatuHenkouData.end(); it++, cnt++){
		if(cnt < cur_row)				continue;

		m_report.SetFocusedRow(cnt+1);
		break;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgIkkatuHenkou::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	int	nID;

	//	フォーカスのあるコントロールのID取得
	nID = m_clsFunc.GetFocusControlID( this );

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		//	パラメータで分岐
		switch( pMsg->wParam ){

		//	F2キーが押された
		case VK_F2:

			//	F2キーをShift + Tabイベントに書き換え
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return( 1 );									//	抜ける
			break;
		case VK_SPACE:
		case VK_RETURN:
			if(nID == IDC_CUSTOM_REPORT_IKKATU_HENKOU){
				UpdateStateFromKey((int)pMsg->wParam);
				return 1;
			}
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（0円データ全選択/解除）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgIkkatuHenkou::OnBnClickedZerochkButton()
{
	int	row = 0;

	m_clsFunc.ChangeReportColumnAll(&m_report, ih_0henkou);
	for(vector<IKKATU_HENKOU_DATA>::iterator it = m_vecIkkatuHenkouData.begin();it != m_vecIkkatuHenkouData.end(); it++, row++){
		if(m_report.IsChecked(row,ih_0henkou) == TRUE || m_nOutKingaku != 0)	{
			m_report.SetCellBackColor(row,ih_0prhenkou,GetSysColor( COLOR_BTNFACE ),TRUE);
			m_report.AllowEditItem(row, ih_0prhenkou, FALSE);
		}
		else	{
			m_report.SetCellBackColor(row,ih_0prhenkou,RGB(255, 255, 255),TRUE);
			m_report.AllowEditItem(row, ih_0prhenkou, TRUE);
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（空欄データ全選択/解除）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgIkkatuHenkou::OnBnClickedNullchkButton()
{
	m_clsFunc.ChangeReportColumnAll(&m_report, ih_null_henkou);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（0円印字全選択/解除）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgIkkatuHenkou::OnBnClickedZeroprchkButton()
{
// midori 152169 del -->
	//int	row = 0;

	//m_clsFunc.ChangeReportColumnAll(&m_report, ih_0prhenkou);
	//// 「0円ﾃﾞｰﾀを出力しない」にチェックが付いている場合、0円印字全選択/解除のチェックをOFFにする
	//for(vector<IKKATU_HENKOU_DATA>::iterator it = m_vecIkkatuHenkouData.begin(); it != m_vecIkkatuHenkouData.end(); it++, row++)	{

	//	if(m_report.IsChecked(row, ih_0henkou) == TRUE)	{
	//		m_report.SetChecked(row, ih_0prhenkou, FALSE);
	//	}

	//}
// midori 152169 del <--
// midori 152169 add -->
	int ii=0;
	int	max=0;
	BOOL flg = FALSE;

	max = m_report.GetRows()->GetCount();

	// チェックが入っている数取得
	flg = TRUE;
	for(ii=0; ii<max; ii++){
		if(m_report.IsChecked( ii, ih_0henkou ) == TRUE)	continue;
		if(m_report.IsChecked( ii, ih_0prhenkou ) == TRUE)	{
			// チェックが1件でもあれば全てオフ
			flg=FALSE;
			break;
		}
	}
	for(ii=0; ii<max; ii++){
		if(m_report.IsChecked( ii, ih_0henkou ) == TRUE)	continue;
		m_report.SetChecked(ii, ih_0prhenkou, flg);
	}

	m_report.Populate();
// midori 152169 add <--
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgIkkatuHenkou::OnBnClickedOkButton()
{
	// レポートコントロールの状態をベクタに更新
	UpdateZeroClearData();
	
	// チェック結果を保持
	SetCheckData();

	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//**************************************************
//	チェックボックスの選択結果を格納
//	【引数】	なし
//	【戻値】	TRUE	…	成功
//				FALSE	…	失敗
//**************************************************
BOOL CdlgIkkatuHenkou::SetCheckData()
{
	int			intFormSeq=0;
	int			st=0,old_st=0;
	BOOL		old_state=FALSE;
	CdbUcInfSub mfcSub(m_pDB);

	// FormOrder昇順でクエリ
	mfcSub.InitOrder();
	mfcSub.MoveFirst();

	// 全帳表分ループを回し、その中でベクタに登録されている様式のみチェックボックスの値を更新する
	vector<IKKATU_HENKOU_DATA>::iterator it = m_vecIkkatuHenkouData.begin();
	while(!mfcSub.IsEOF()){
		if(it == m_vecIkkatuHenkouData.end())	break;

		// 汎用変数コントロールクラス
		CCtrlSubGeneralVar cs( mfcSub );

		// 金額０円データを出力しない
		st = it->is0_henkou;
		if(cs.bPrintZeroData)	old_st = TRUE;
		else					old_st = FALSE;
		if(st != old_st)	{
			mfcSub.Edit();
			cs.bPrintZeroData = it->is0_henkou;
			cs.Set(mfcSub);
			mfcSub.Update();
			m_isChangePageNum = TRUE;
			m_isYousiki[mfcSub.m_FormOrder-1] = TRUE;
		}

		// 金額空欄データを出力しない
		st = it->isNULL_henkou;
		if(cs.bPrintNullData)	old_st = TRUE;
		else					old_st = FALSE;
		if(st != old_st)	{
			mfcSub.Edit();
			cs.bPrintNullData = it->isNULL_henkou;
			cs.Set(mfcSub);
			mfcSub.Update();
			m_isChangePageNum = TRUE;
			m_isYousiki[mfcSub.m_FormOrder-1] = TRUE;
		}

		// 金額０円を印字する(ONでFALSE OFFでTRUE)
		st = it->is0pr_henkou;
		old_state = BST_CHECKED;
		if(cs.bPrintHideZero)	old_state = BST_UNCHECKED;
		if(st != old_state)	{
			mfcSub.Edit();
			if(it->is0pr_henkou)	cs.bPrintHideZero = FALSE;
			else					cs.bPrintHideZero = TRUE;
			cs.Set(mfcSub);
			mfcSub.Update();
			m_isChangePageNum = TRUE;
			m_isYousiki[mfcSub.m_FormOrder-1] = TRUE;
		}

		it++;
		mfcSub.MoveNext();
	}
	mfcSub.Fin();

	return TRUE;
}

//**************************************************
//	ダイアログ表示
//	【引数】	intFormSeq		…	ダイアログを呼び出した際のカレント帳表
//				pDB				…	データベースハンドル
//				intOutKingaku	…	金額欄を空欄で出力するサイン
//	【戻値】	ID_DLG_OK		…	印刷ボタンが押された
//				ID_DLG_CANCEL	…	キャンセルボタンが押された
//**************************************************
int CdlgIkkatuHenkou::ShowDialog( BOOL& isChange, BOOL isYousiki[], int intFormSeq, CDatabase* pDB, int intOutKingaku )
{
	int		ii=0;

	m_nRet = ID_DLG_CANCEL;	//	戻値初期化
	m_nFormSeq = ID_FORMNO_011;
	m_isChangePageNum = FALSE;
	for(ii=0;ii<UC_ID_FORMNO_MAX;ii++)	{
		m_isYousiki[ii] = isYousiki[ii];
	}
	if(pDB != NULL)	{
		try	{
			if(pDB->IsOpen())	{
				m_pDB = pDB;					// データベースハンドル取得
				m_nFormSeq = intFormSeq;		// 様式番号取得
				m_nOutKingaku = intOutKingaku;	// 金額欄を空欄で出力するサイン取得
				DoModal();						// モーダルで表示
				if(m_nRet != ID_DLG_CANCEL){
					isChange = m_isChangePageNum;
					for(ii=0;ii<UC_ID_FORMNO_MAX;ii++)	{
						isYousiki[ii] = m_isYousiki[ii];
					}
				}
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return( m_nRet );
}
