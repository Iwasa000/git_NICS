// dlgZeroMoneyClear.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgZeroMoneyClear.h"


// CdlgZeroMoneyClear ダイアログ

IMPLEMENT_DYNAMIC(CdlgZeroMoneyClear, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgZeroMoneyClear::CdlgZeroMoneyClear(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgZeroMoneyClear::IDD, pParent)
{
	// 初期化
	for( int i = 0; i < UC_ID_FORMNO_MAX; i++ ){
		ZeroMemory( &m_ClearData[i], sizeof(CLEAR_DATA) );
	}
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgZeroMoneyClear::~CdlgZeroMoneyClear()
{
}

void CdlgZeroMoneyClear::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);

	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_HYOCHK_BUTTON, m_btnHyoChk);
	DDX_Control(pDX, ID_CLEARZEROCHK_BUTTON, m_btnZeroChk);
	DDX_Control(pDX, ID_CLEARNULLCHK_BUTTON, m_btnNullChk);
// midori 151394 cor -->
//	DDX_Control(pDX, IDC_CUSTOM_REPORT_ZERO_MOVE, m_report);
// ---------------------
	DDX_Control(pDX, IDC_CUSTOM_REPORT_ZERO_CLEAR, m_report);
// midori 151394 cor <--
}


BEGIN_MESSAGE_MAP(CdlgZeroMoneyClear, ICSDialog)
	ON_BN_CLICKED(ID_HYOCHK_BUTTON, &CdlgZeroMoneyClear::OnBnClickedHyochkButton)
	ON_BN_CLICKED(ID_CLEARZEROCHK_BUTTON, &CdlgZeroMoneyClear::OnBnClickedZerochkButton)
	ON_BN_CLICKED(ID_CLEARNULLCHK_BUTTON, &CdlgZeroMoneyClear::OnBnClickedNullchkButton)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgZeroMoneyClear::OnBnClickedOkButton)

	ON_NOTIFY(ICS_NM_REPORT_CHECKED, IDC_CUSTOM_REPORT_ZERO_CLEAR, OnNMReportCheckedCUSTOM_REPORT_ZERO_CLEAR)
END_MESSAGE_MAP()


// CdlgZeroMoneyClear メッセージ ハンドラ

//+++++++++++++++++++++++++++++++++++++++++++
// ダイアログ起動時のイベント
//+++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgZeroMoneyClear::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	InitReportHeader();

	CreateZeroClearVec();

	AddRecords();

	SetData();

	m_report.SetFocus();
	m_report.SetFocusedRow(0);

	// リサイズ
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（帳表全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyClear::OnBnClickedHyochkButton()
{
	int	row = 0;

	m_clsFunc.ChangeReportColumnAll(&m_report, zc_select);
	for(vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin();it != m_vecZeroClearData.end(); it++, row++){
// midori 151393 cor -->
//		if((*it).isEnable == FALSE)	continue;
// ---------------------
		if((*it).isEnable == FALSE)	{
			// レコードが入力不可の場合、チェックをOFFにする
			m_report.SetChecked(row, zc_select, FALSE);
			continue;
		}
// midori 151393 cor <--
		BOOL isCheck = m_report.IsChecked(row, zc_select);
		SetCheckBoxState(row, isCheck);
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（０円移動全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyClear::OnBnClickedZerochkButton()
{
	int	row = 0;

	m_clsFunc.ChangeReportColumnAll(&m_report, zc_0clear);
	for(vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin();it != m_vecZeroClearData.end(); it++, row++){
// midori 151393 add -->
		// レコードが入力不可又は選択にチェックが付いていない場合、０クリア全選択/解除ボタンのチェックをOFFにする
		if((*it).isEnable == FALSE || m_report.IsChecked(row, zc_select) == FALSE)	m_report.SetChecked(row, zc_0clear, FALSE);
// midori 151393 add <--
		if(m_report.IsChecked(row,zc_0clear) == TRUE)	{
			m_report.SetChecked(row,zc_null_clear,FALSE);
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（空欄移動全選択/解除ボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyClear::OnBnClickedNullchkButton()
{
	int	row = 0;

	m_clsFunc.ChangeReportColumnAll(&m_report, zc_null_clear);
	for(vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin();it != m_vecZeroClearData.end(); it++, row++){
// midori 151393 add -->
		// レコードが入力不可又は選択にチェックが付いていない場合、空欄全選択/解除ボタンのチェックをOFFにする
		if((*it).isEnable == FALSE || m_report.IsChecked(row, zc_select) == FALSE)	m_report.SetChecked(row, zc_null_clear, FALSE);
// midori 151393 add <--
		if(m_report.IsChecked(row,zc_null_clear) == TRUE)	{
			m_report.SetChecked(row,zc_0clear,FALSE);
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyClear::OnBnClickedOkButton()
{
	// レポートコントロールの状態をベクタに更新
	UpdateZeroClearData();

	// 選択カラムが1つ以上チェックされてる？
	if(GetSelectCheckNum() < 1){
		ICSMessageBox( _T("様式が１件も選択されていません。"), MB_OK, 0, 0, this );
		m_report.SetFocus();
		return;
	}
	
	// チェック結果を保持
	SetCheckData();

	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgZeroMoneyClear::OnOK()
{
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgZeroMoneyClear::PreTranslateMessage(MSG* pMsg)
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
			if(nID == IDC_CUSTOM_REPORT_ZERO_CLEAR){
				UpdateStateFromKey((int)pMsg->wParam);
				return 1;
			}
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
//	指定帳表データ有無チェック処理
//	【引数】	intFormSeq	…	帳表FormSeq番号
//	【戻値】	TRUE		…	空行/頁計/累計以外の行あり
//				FALSE		…	空行/頁計/累計行のみ
//**************************************************
BOOL CdlgZeroMoneyClear::CheckYousikiData( int intFormSeq )
{
	CdbUc000Common* rsData = NULL;

	// ＤＢオブジェクト取得
	rsData = m_clsFunc.CreateDbObject(intFormSeq, m_pDB);

	// データ取得
	if ( rsData->GetCountDataRecord(1) == 0 ){
		rsData->Fin();
		delete rsData;

		return FALSE;
	}

	rsData->Fin();
	delete rsData;

	return TRUE;
}

//**************************************************
//	チェックボックスの選択結果を格納
//	【引数】	なし
//	【戻値】	TRUE	…	成功
//				FALSE	…	失敗
//**************************************************
BOOL CdlgZeroMoneyClear::SetCheckData()
{
	int intFormSeq = 0;
	int cnt = 0;
	CdbUcInfSub mfcSub(m_pDB);

	// FormOrder昇順でクエリ
	mfcSub.InitOrder();
	mfcSub.MoveFirst();

	// 全帳表分ループを回し、その中でベクタに登録されている様式のみチェックボックスの値を更新する
	vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin();
	while(!mfcSub.IsEOF()){
		if(it == m_vecZeroClearData.end())	break;

		// 様式シーケンスは必ずセット
		m_ClearData[cnt].intFormSeq = mfcSub.m_FormSeq;

		if(m_ClearData[cnt].intFormSeq == (*it).FormSeq){
			if((*it).is_select != FALSE && ((*it).isEnable != FALSE)){
				m_ClearData[cnt].blnZeroFg = (*it).is0_clear;
				m_ClearData[cnt].blnNullFg = (*it).isNULL_clear;
			}
			it++;
		}

		cnt++;
		mfcSub.MoveNext();
	}
	mfcSub.Fin();

	return TRUE;
}

//**************************************************
//	ダイアログ表示
//	【引数】	intFormSeq		…	ダイアログを呼び出した際のカレント帳表
//				pDB				…	データベースハンドル
//	【戻値】	ID_DLG_OK		…	印刷ボタンが押された
//				ID_DLG_CANCEL	…	キャンセルボタンが押された
//**************************************************
int CdlgZeroMoneyClear::ShowDialog( int intFormSeq, CDatabase* pDB )
{
	m_nRet = ID_DLG_CANCEL;	//	戻値初期化
	m_nFormSeq = ID_FORMNO_011;

	//	データベースハンドルがヌルじゃない？
	if ( pDB != NULL ){

		try{
			//	データベース開いてる？
			if ( pDB->IsOpen() ){
				m_pDB = pDB;	//	データベースハンドル取得
				m_nFormSeq = intFormSeq;		//	様式番号取得
				DoModal();		//	モーダルで表示
			}
		}
		catch(...){
		}
	}

	//	戻値を返す
	return( m_nRet );
}

//**************************************************
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgZeroMoneyClear::InitReportHeader()
{
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
	m_report.AddColumn(new ICSReportColumn(0, NULL, 1));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 2));
	m_report.AddColumn(new ICSReportColumn(2, NULL, 10));
	m_report.AddColumn(new ICSReportColumn(3, NULL, 4));
	m_report.AddColumn(new ICSReportColumn(4, NULL, 4));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	ICSReportColumns *m_pClms = m_report.GetColumns();
	int cnt = sizeof(strZeroClearHeader) / sizeof(strZeroClearHeader[0]);
	for(int i = 0; i < cnt; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		m_pClm->SetCaption(strZeroClearHeader[i]);
	}

	// フォント設定
	CFont*		pFont;
	LOGFONT		font = {0};

	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_report.SetTextFont(font);
	m_report.SetHeaderFont(font);
}

//**************************************************
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	TRUE:成功
//				FALSE:失敗
//**************************************************
int CdlgZeroMoneyClear::AddRecords()
{
	for(vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin(); it != m_vecZeroClearData.end(); it++){
		ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 選択
		pRecord->AddItem( new ICSReportRecordItemText(it->TitleNo));	// 様式番号
		pRecord->AddItem( new ICSReportRecordItemText(it->Title));		// 様式名称
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 金額等を０クリアする
		pRecord->AddItem( new ICSReportRecordItemCheckBox());			// 金額等を空欄にする
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
void CdlgZeroMoneyClear::SetData()
{
	int row = 0;
	for(vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin(); it != m_vecZeroClearData.end(); it++, row++){
		// データのない行は背景色も更新
		if((*it).isEnable == FALSE){
			SetCheckBoxState(row, FALSE);
			// 背景色更新
			SetColor(row, (*it).isEnable);
			// データがない行は「選択」カラムも選択不可
			m_report.AllowEditItem(row, zc_select, FALSE);
		}
		else{
			BOOL isFromFormSeq = FALSE;	// 現在入力中の様式かどうかかどうか
			if(m_nFormSeq == (*it).FormSeq)		isFromFormSeq = TRUE;
			m_report.SetChecked(row, zc_select,isFromFormSeq);
			SetCheckBoxState(row, isFromFormSeq);	// 入力中の様式のみ、チェック可
		}
// midori 151393 cor -->
//		m_report.SetChecked(row, zc_0clear,TRUE);
//		m_report.SetChecked(row,zc_null_clear,FALSE);
// ---------------------
		// チェックの付いている様式のみ金額等を０クリアするにチェックを付ける
		if(m_report.IsChecked(row,zc_select) == TRUE)	{
			m_report.SetChecked(row, zc_0clear,TRUE);
		}
// midori 151393 cor <--
	}
}

void CdlgZeroMoneyClear::CreateZeroClearVec()
{
	CdbUcInfSub	mfcRec(m_pDB);
	mfcRec.InitOrder();
	mfcRec.MoveFirst();
	while(!mfcRec.IsEOF()){
		ZERO_CLEAR_DATA	zero_clear_data;
		zero_clear_data.FormSeq = mfcRec.m_FormSeq;
		zero_clear_data.TitleNo = mfcRec.m_TitleNo + mfcRec.m_TitleNo2;
		zero_clear_data.Title	= mfcRec.m_Title;

		BOOL hasData = CheckYousikiData(mfcRec.m_FormSeq);
		if(hasData != NULL){	// データがなければ選択不可
			zero_clear_data.isEnable = TRUE;
		}

		// ベクタの最後尾に追加
		m_vecZeroClearData.push_back(zero_clear_data);
		// 次の様式へ
		mfcRec.MoveNext();
	}
}

void CdlgZeroMoneyClear::SetCheckBoxState(int indexRow, BOOL state)
{
	m_report.AllowEditItem(indexRow, zc_0clear, state);
	m_report.AllowEditItem(indexRow, zc_null_clear, state);
}

void CdlgZeroMoneyClear::OnNMReportCheckedCUSTOM_REPORT_ZERO_CLEAR(NMHDR * pNotifyStruct, LRESULT * result)
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

	if(indexColumn != zc_select && indexColumn != zc_0clear && indexColumn != zc_null_clear)	{
		return;
	}
	// 選択
	if(indexColumn == zc_select)	{
		// チェックボックス更新
		SetCheckBoxState(indexRow, bCheck);
	}
	// 金額等を０クリアする
	else if(indexColumn == zc_0clear)	{
		m_report.SetChecked(indexRow,zc_null_clear,FALSE);
	}
	// 金額等を空欄にする
	else if(indexColumn == zc_null_clear)	{
		m_report.SetChecked(indexRow,zc_0clear,FALSE);
	}

	m_report.RedrawControl();
}

//**************************************************
//	色を設定
//	【引数】	nIndex	…	インデックス
//				check	…	選択チェックボックスの状態
//	【戻値】	なし
//**************************************************
void CdlgZeroMoneyClear::SetColor( int nIndex, BOOL check)
{
	if(check == FALSE)	m_report.SetRowBackColor(nIndex, GetSysColor( COLOR_BTNFACE ), FALSE);
}

void CdlgZeroMoneyClear::UpdateStateFromKey(int key)
{
	if(m_clsFunc.GetFocusControlID(this) != IDC_CUSTOM_REPORT_ZERO_CLEAR)	return;

	ICSReportSelectedRows* s_rows = m_report.GetSelectedRows();
	if(s_rows == NULL)	return;
	ICSReportRow* r_row = s_rows->GetAt(0);
	if(r_row == NULL)	return;
	int indexRow = r_row->GetIndex();
	BOOL check = m_report.IsChecked(indexRow, zc_select);
	ICSReportRecordItem	*pItem	= m_report.GetActiveItem();

	switch(key){
		case VK_SPACE:
			if(IsExistData(indexRow) == FALSE)	return;	// データのない様式はチェックを入れさせない

			// 選択カラムの状態反転
			check = !check;
			if(pItem == NULL){
				m_report.SetChecked(indexRow, zc_select, check);

				// チェックボックス更新
				SetCheckBoxState(indexRow, check);
				// 背景色更新
				//SetColor(indexRow, check);
				m_report.RedrawControl();
			}
			else{
				int indexClm = pItem->GetIndex();
				if(indexClm == zc_select){
					m_report.SetChecked(indexRow, zc_select, check);
					// チェックボックス更新
					SetCheckBoxState(indexRow, check);
					// 背景色更新
					//SetColor(indexRow, check);
					m_report.RedrawControl();
				}
				else if(indexClm == zc_0clear || indexClm == zc_null_clear){

					BOOL check2 = !m_report.IsChecked(indexRow, indexClm);
					m_report.SetChecked(indexRow, indexClm, check2);
// midori 152387 add -->
					if(indexClm == zc_0clear)	{
						if(check2 == TRUE)	{
							m_report.SetChecked(indexRow,zc_null_clear,FALSE);
						}
					}
					if(indexClm == zc_null_clear)	{
						if(check2 == TRUE)	{
							m_report.SetChecked(indexRow,zc_0clear,FALSE);
						}
					}
// midori 152387 add <--
					m_report.AllowEditItem( indexRow, indexClm, TRUE );	// この3行を入れないとVK_SPACE後Editが抜けてしまう
					m_report.Populate();
					m_report.EditItem( indexRow, indexClm, TRUE );
				}
			}
			break;

		case VK_RETURN:
			if(check == FALSE){
				MoveDown(indexRow);
				return;
			}
			
			if(pItem != NULL){	// エディットが入っている→次のカラムにエディット
				int indexClm = pItem->GetIndex();
				if(indexClm == zc_select){
					indexClm = zc_0clear;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
				else if(indexClm == zc_0clear){
					indexClm = zc_null_clear;
					m_report.EditItem(indexRow, indexClm, TRUE);
				}
				else if(indexClm == zc_null_clear){	// 空欄カラムなら次の行にエディット
					MoveDown(indexRow);
					ICSReportRecords * records = m_report.GetRecords();
					int max_cnt = records->GetCount();
					if(indexRow+1<max_cnt)		m_report.EditItem( indexRow + 1, zc_select, TRUE );
				}
			}
			else{	// エディットが入っていない→選択カラムにエディット
				m_report.EditItem( indexRow, zc_select, TRUE );
			}
	}
}

void CdlgZeroMoneyClear::UpdateZeroClearData()
{
	ICSReportRecords * records = m_report.GetRecords();
	if(records == NULL)	return;
	int max_cnt = records->GetCount();

	for(int row=0; row<max_cnt; row++){
		m_vecZeroClearData[row].is_select = m_report.IsChecked(row, zc_select);
		m_vecZeroClearData[row].is0_clear = m_report.IsChecked(row, zc_0clear);
		m_vecZeroClearData[row].isNULL_clear = m_report.IsChecked(row, zc_null_clear);
	}
}

int CdlgZeroMoneyClear::GetSelectCheckNum()
{
	int cnt = 0;
	for(vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin();it != m_vecZeroClearData.end(); it++){
		if(it->is_select != FALSE){
			cnt++;
		}
	}
	return cnt;
}

void CdlgZeroMoneyClear::SetGrid()
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

BOOL CdlgZeroMoneyClear::IsExistData(int row)
{
	try{
		ZERO_CLEAR_DATA zmd = m_vecZeroClearData.at(row);
		return zmd.isEnable;
	}
	catch(...){
		return FALSE;
	}

	return FALSE;
}

void CdlgZeroMoneyClear::MoveDown(int cur_row)
{
	int cnt = 0;
	for(vector<ZERO_CLEAR_DATA>::iterator it = m_vecZeroClearData.begin();it != m_vecZeroClearData.end(); it++, cnt++){
		if(cnt < cur_row)				continue;
		if(IsExistData(cnt+1) == FALSE)	continue;

		m_report.SetFocusedRow(cnt+1);
		break;
	}
}
