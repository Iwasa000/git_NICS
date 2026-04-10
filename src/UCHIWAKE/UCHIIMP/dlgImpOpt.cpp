// dlgImpOpt.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"       // メイン シンボル
#include "dlgImpOpt.h"
#include "impUc000Common.h"
// midori 190802 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc052Tanaoroshi2.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc092Kaikakekin2.h"

extern	BOOL	bG_Kanso;
// midori 190802 add <--
// 登録番号（法人番号）_23/11/21 add -->
extern	BOOL	bG_InvNo;
// 登録番号（法人番号）_23/11/21 add <--

// 24/01/16_ScoreLink add -->
BOOL			bG_ScoreLink;
// 24/01/16_ScoreLink add <--

// CdlgImpOpt ダイアログ

IMPLEMENT_DYNAMIC(CdlgImpOpt, ICSDialog)

CdlgImpOpt::CdlgImpOpt(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgImpOpt::IDD, pParent)
{
	m_nRet			= ID_DLG_CANCEL;
// midori 170504 del -->
//	m_stateTrans	= 0;
//	m_stateImpOut	= 0x003fffff;			// デフォルトで全てON
//	m_stateExpOut	= 0x003fffff;			// デフォルトで全てON
// midori 170504 del <--
// midori 170504 add -->
	m_stateTrans[0]		= 0;
	m_stateImpOut[0]	= 0x003fffff;			// デフォルトで全てON
	m_stateExpOut[0]	= 0x003fffff;			// デフォルトで全てON
	m_stateTrans[1]		= 0;
	m_stateImpOut[1]	= 0x000fffff;			// デフォルトで全てON
	m_stateExpOut[1]	= 0x000fffff;			// デフォルトで全てON
	m_stateTrans[2]		= 0;
	m_stateImpOut[2]	= 0x000fffff;			// デフォルトで全てON
	m_stateExpOut[2]	= 0x000fffff;			// デフォルトで全てON
// midori 170504 add <--
}

CdlgImpOpt::~CdlgImpOpt()
{
}

void CdlgImpOpt::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_report1);
	DDX_Control(pDX, IDC_BUTTON_ALLON, m_btnAllOn);
	DDX_Control(pDX, IDC_BUTTON_ALLOFF, m_btnAllOff);
	DDX_Control(pDX, IDC_BUTTON_IMP_START, m_btnImpStart);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_CHG_TRANS, m_btnChgTrans);
	DDX_Control(pDX, IDC_STATIC_OPT_MESSAGE, m_stcOptMessage);
// 登録番号（法人番号）_23/11/21 add -->
	DDX_Control(pDX, IDC_STATIC_OPT_MESSAGE2, m_stcOptMessage2);
	DDX_Control(pDX, IDC_STATIC_OPT_MESSAGE3, m_stcOptMessage3);
// 登録番号（法人番号）_23/11/21 add <--
}


BEGIN_MESSAGE_MAP(CdlgImpOpt, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALLON, &CdlgImpOpt::OnBnClickedButtonAllon)
	ON_BN_CLICKED(IDC_BUTTON_ALLOFF, &CdlgImpOpt::OnBnClickedButtonAlloff)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CdlgImpOpt::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_IMP_START, &CdlgImpOpt::OnBnClickedButtonImpStart)
	ON_NOTIFY(ICS_NM_REPORT_CHECKED, IDC_CUSTOM1, OnNMReportCheckedCUSTOM1)
	ON_BN_CLICKED(IDC_BUTTON_CHG_TRANS, &CdlgImpOpt::OnBnClickedButtonChgTrans)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_CUSTOM1, OnNMReportEditkeydownCUSTOM1)
END_MESSAGE_MAP()


// CdlgImpOpt メッセージ ハンドラ

BOOL CdlgImpOpt::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	//--------------------------------------------------
	// 表示文言設定
	//--------------------------------------------------
	CString		strTitle		= _T("");		// タイトル
	CString		strMsg			= _T("");		// メッセージ
// 登録番号（法人番号）_23/11/21 add -->
	CString		strMsg2			= _T("");		// メッセージ
	CString		strMsg3			= _T("");		// メッセージ
// 登録番号（法人番号）_23/11/21 add <--
	CString		strBtnMsg		= _T("");		// ボタンキャプション

	// 文字列生成
	if (m_fImport == TRUE) {
		// インポート
		strTitle	= IMP_DLGMSG_IMPORT_OPTION_TITLE;
		strMsg		= IMP_DLGMSG_IMPORT_ATTENTION;
		strBtnMsg	= IMP_DLGMSG_IMPORT_BUTTON;
	}
	else {
		// エクスポート
		strTitle	= IMP_DLGMSG_EXPORT_OPTION_TITLE;
		strMsg		= IMP_DLGMSG_EXPORT_ATTENTION;
// 登録番号（法人番号）_23/11/21 add -->
		if(bG_InvNo == TRUE)	{
			strMsg2		= _T("※令和6年3月1日以後終了事業年度分「登録番号(法人番号)欄の追加」の様式でエクスポートします。");
// 231222_168484 cor -->
			//strMsg3		= _T("　対応前のプログラムでインポートすることはできませんのでご注意ください。");
// ---------------------
			strMsg3		= _T("　対応前のプログラムで、正常にインポートすることはできませんのでご注意ください。");
// 231222_168484 cor <--
		}
// 登録番号（法人番号）_23/11/21 add <--
		strBtnMsg	= IMP_DLGMSG_EXPORT_BUTTON;
		m_btnChgTrans.ShowWindow( FALSE );
	}

	// 文字列→ダイアログにセット
	SetWindowText(strTitle);										// タイトル
	m_stcOptMessage.UpdateCaption(strMsg, NULL);					// メッセージ
// 登録番号（法人番号）_23/11/21 add -->
	m_stcOptMessage2.UpdateCaption(strMsg2, NULL);					// メッセージ
	m_stcOptMessage3.UpdateCaption(strMsg3, NULL);					// メッセージ
// 登録番号（法人番号）_23/11/21 add <--
	m_btnImpStart.SetWindowTextA(strBtnMsg);						// インポート/エクスポート開始ボタン

	//カラム設定
	MakeReportControl();

	//データ挿入
	CsvDataAdd();

	// 初期状態の選択状態をレコードの先頭に
	m_report1.SetSelectedRows(0, 0);

	// フォーカスをレポートコントロールにあてる
	m_report1.MoveDown(FALSE, FALSE);
	
// 24/01/16_ScoreLink add -->
	if(bG_ScoreLink == TRUE)	{
		PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_IMP_START,BN_CLICKED),0);
	}
// 24/01/16_ScoreLink add <--

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

int CdlgImpOpt::MakeReportControl()
{
	ICSReportColumns	*m_pClms	= NULL;
	ICSReportColumn		*m_pClm		= NULL;

	m_report1.Resize( TRUE, ICSResizeDefault );
	m_report1.RemoveAllRecords();
	m_report1.RemoveColumn(-1);
	m_report1.Populate();

	if( m_fImport != FALSE){
		m_report1.AddColumn(new ICSReportColumn(0, NULL, 6  , true,  0, TRUE, TRUE, DT_CENTER));
		m_report1.AddColumn(new ICSReportColumn(1, NULL, 13 , true,  1, TRUE, TRUE, DT_CENTER));
		m_report1.AddColumn(new ICSReportColumn(2, NULL, 10 , true,  3, TRUE, TRUE, DT_CENTER));
		m_report1.AddColumn(new ICSReportColumn(3, NULL, 75 , true,  2, TRUE, TRUE, DT_CENTER));
	}
	else{
		m_report1.AddColumn(new ICSReportColumn(0, NULL, 6  , true,  0, TRUE, TRUE, DT_CENTER));
		m_report1.AddColumn(new ICSReportColumn(1, NULL, 10 , true,  3, TRUE, TRUE, DT_CENTER));
		m_report1.AddColumn(new ICSReportColumn(2, NULL, 75 , true,  2, TRUE, TRUE, DT_CENTER));
	}

	m_report1.SetSortable ( FALSE );			// ソート禁止
	m_report1.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report1.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report1.SetMultipleSelection( FALSE );	// 複数行選択禁止

	m_pClms	= m_report1.GetColumns();

	if( m_fImport != FALSE ){
		for ( int i = 0; i < 4; i++ ) {
			m_pClm = m_pClms->GetAt( i );	

			m_pClm->SetCaption ( UcImpOptClm[i] );
		}
	}
	else{
		for ( int i = 0; i < 3; i++ ) {
			m_pClm = m_pClms->GetAt( i );	

			m_pClm->SetCaption ( UcImpOptClmExp[i] );
		}
	}

	return 0;
}

int CdlgImpOpt::CsvDataAdd()
{
	CStringArray			strArray;		strArray.RemoveAll();
	CStringArray			strArray142;	strArray142.RemoveAll();
	ICSReportRecordItem		*pItem		= NULL;
	int						bitStateTrans[32] = {'\0'};
// midori 170504 add -->
	int						j=0,cnt=0;
// midori 170504 add <--
// midori 190802 add -->
	int						ii = 0;
	int						jj = 0;
	CdbUcInfSub				mfcRec(m_pZmSub->m_database);
	CUcFunctionCommon		clsFunc;		//	共通関数クラス
	CString					cst = _T("");
// midori 190802 add <--
	// 転送方式文言セット
	for(int i=0; i<TRANS_NUM; i++){
		strArray.Add( UcTransMode[i] );
	}

	//「上書きのみ」の文言セット
	strArray142.Add("上書きのみ");

	// カラムにデータセット
	for(int i=0; i<GetSheetNum(); i++){
		ICSReportRecord*  pRecord  =  m_report1.AddRecord( new ICSReportRecord() );

		if( m_fImport != FALSE ){
			pItem  =  pRecord->AddItem( new ICSReportRecordItemCheckBox());	
			//⑭－２：人件費の内訳書は上書きのみ
			if( m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[16]) != 0 ){
				pItem  =  pRecord->AddItem( new ICSReportRecordItemComboBox( 0, ICS_CBS_DROPDOWNLIST, &strArray, 0, ICS_EDS_ASCII, FALSE, TRUE, 0, ICS_EDS_LEFT, FALSE, 25));
			}
			else{
				pItem  =  pRecord->AddItem( new ICSReportRecordItemComboBox( 0, ICS_CBS_DROPDOWNLIST, &strArray142, 0, ICS_EDS_ASCII, FALSE, TRUE, 0, ICS_EDS_LEFT, FALSE, 25));
			}
			pItem  =  pRecord->AddItem( new ICSReportRecordItemText( m_datImp.GetAt(i).SheetNo ));
			pItem  =  pRecord->AddItem( new ICSReportRecordItemText( m_datImp.GetAt(i).SheetName ));
		}
		else{
			pItem  =  pRecord->AddItem( new ICSReportRecordItemCheckBox());	
			pItem  =  pRecord->AddItem( new ICSReportRecordItemText( m_datImp.GetAt(i).SheetNo ));
			pItem  =  pRecord->AddItem( new ICSReportRecordItemText( m_datImp.GetAt(i).SheetName ));
		}
	}	

	//カラムの属性設定
	m_report1.AllowEdit( TRUE );
	m_report1.AllowEditColumn( m_clmYoushikiNo, FALSE );
	m_report1.AllowEditColumn( m_clmYoushikiName, FALSE );


	//更新
	m_report1.Populate();

// 24/01/16_ScoreLink cor -->
	//// owntbの値取得
	//GetOwnTblInfo();
// --------------------------
	char	bf[128]={0};
	memset(bf,0,sizeof(bf));
	UCOM_GetString(_T("SCORELINK依頼ID"),bf);
	bG_ScoreLink = FALSE;
	if(strlen(bf) > 0)	{
		bG_ScoreLink = TRUE;
	}	
	// ScoreLinkからの起動でない場合
	if(bG_ScoreLink == FALSE)	{
		// owntbの値取得
		GetOwnTblInfo();
	}
// 24/01/16_ScoreLink cor <--

// midori 190802 add -->
	// Export処理の場合、入力データがなければ出力のチェックをOFFにする
// 24/01/16_ScoreLink cor -->
	//if( m_fImport == FALSE ){
// --------------------------
	// ScoreLinkからの起動時はチェックを付けておく
	if(m_fImport == FALSE && bG_ScoreLink == FALSE)	{
// 24/01/16_ScoreLink cor <--
		mfcRec.InitOrder();
		mfcRec.MoveFirst();
		while(!mfcRec.IsEOF()){
			CdbUc000Common* rsData = clsFunc.CreateDbObject(mfcRec.m_FormSeq, m_pZmSub->m_database);
			if(rsData != NULL){
				BOOL hasData = FALSE;
				if( rsData->GetCountDataRecord(1) != 0 ){
					hasData = TRUE;
				}
				// 棚卸資産の内訳書、買掛金の内訳書は欄外登録の内容があるかどうかでも判断を行う
				if((mfcRec.m_FormSeq == ID_FORMNO_051 && bG_Kanso == FALSE) || mfcRec.m_FormSeq == ID_FORMNO_091)	{
					rsData->Fin();
					delete rsData;
					if(mfcRec.m_FormSeq == ID_FORMNO_051)	{
						rsData = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(m_pZmSub->m_database));
					}
					else	{
						rsData = (CdbUc000Common*)(new CdbUc092Kaikakekin2(m_pZmSub->m_database));
					}
					if(rsData->GetPrintPageNum() != 0)	{
						hasData = TRUE;
					}
				}
				rsData->Fin();
				// 入力データなし
				if(hasData == FALSE) {
					cst = mfcRec.m_TitleNo + mfcRec.m_TitleNo2;
					for(ii=0; ii<SHEET_NUM; ii++){
						if(m_datImp.GetAt(ii).SheetNo.Compare(cst) == 0) {
							for(jj=0; j<SHEET_NUM; jj++){
								int retComp = m_datImp.GetAt(ii).SheetNo.Compare(UcSheetNumber[jj]);
								if( retComp == 0 ){
									break;
								}
							}
							if(jj < 22)			cnt=0;
							else if(jj<42)		cnt=1;
							else				cnt=2;
							m_stateExpOut[cnt] &= ~m_datImp.GetAt(ii).bitTarget;
							break;
						}
					}
				}
			}
			delete rsData;		
			mfcRec.MoveNext();
		}
	}
// midori 190802 add <--

	// 出力状態、転送方式の復帰
	for(int i=0; i<GetSheetNum(); i++){
// midori 170504 del -->
		//for(int j=0; j<SHEET_NUM; j++){
		//	int retComp = m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[j] );
		//	if( retComp == 0 ){
		//		break;
		//	}
		//}		

		//if( m_fImport != FALSE ){
		//	// 出力状態
		//	if( m_stateImpOut & m_datImp.GetAt(i).bitTarget ){
		//		m_report1.SetChecked(i, m_clmCheck, TRUE);
		//	}
		//	else{
		//		m_report1.SetChecked(i, m_clmCheck, FALSE);
		//	}
		//	// 転送方式
		//	if( m_stateTrans & m_datImp.GetAt(i).bitTarget ){
		//		m_report1.SetCurSel(i, 1, 1);
		//	}
		//	else{				
		//		m_report1.SetCurSel(i, 1, 0);
		//	}
		//}
		//else{
		//	// 出力状態
		//	if( m_stateExpOut & m_datImp.GetAt(i).bitTarget ){
		//		m_report1.SetChecked(i, m_clmCheck, TRUE);
		//	}
		//	else{
		//		m_report1.SetChecked(i, m_clmCheck, FALSE);
		//	}
		//}
// midori 170504 del <--
// midori 170504 add -->
		for(j=0; j<SHEET_NUM; j++){
			int retComp = m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[j] );
			if( retComp == 0 ){
				break;
			}
		}

		if(j < 22)			cnt=0;
		else if(j<42)		cnt=1;
		else				cnt=2;

		if( m_fImport != FALSE ){
			// 出力状態
			if( m_stateImpOut[cnt] & m_datImp.GetAt(i).bitTarget ){
				m_report1.SetChecked(i, m_clmCheck, TRUE);
			}
			else{
				m_report1.SetChecked(i, m_clmCheck, FALSE);
			}
			// 転送方式
			if( m_stateTrans[cnt] & m_datImp.GetAt(i).bitTarget ){
				m_report1.SetCurSel(i, 1, 1);
			}
			else{
				m_report1.SetCurSel(i, 1, 0);
			}
		}
		else{
			// 出力状態
			if( m_stateExpOut[cnt] & m_datImp.GetAt(i).bitTarget ){
				m_report1.SetChecked(i, m_clmCheck, TRUE);
			}
			else{
				m_report1.SetChecked(i, m_clmCheck, FALSE);
			}
		}
// midori 170504 add <--
	}

	// チェックボックスがOFFの状態でコンボボックスに入力が入らないようにする
	if( m_fImport != FALSE ){
		for(int i=0; i<(int)m_datImp.GetCount(); i++){
			if( m_report1.IsChecked(i, m_clmCheck) == FALSE ){
				m_report1.AllowEditItem(i, m_clmTrans, FALSE);
			}
			else{
				// ⑭－２を入力不可にする
				if( m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[16]) == 0){
					m_report1.AllowEditItem( i, m_clmTrans, FALSE );
				}
			}
		}
	}

	return 0;
}

int CdlgImpOpt::CheckBoxControl(BOOL state)
{
	ICSReportRows	*pRows	=	NULL;
	int				cntRows =	0;

	pRows = m_report1.GetRows();
	cntRows = pRows->GetCount();

	for(int i=0; i<cntRows; i++){
		m_report1.SetChecked( i, m_clmCheck, state );
		// ⑭－２を入力不可にする(なぜかほかの場所では反映されなかったので)
		if( m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[16]) == 0){
			m_report1.AllowEditItem( i, m_clmTrans, FALSE );
		}
	}

	return 0;
}

void CdlgImpOpt::OnBnClickedButtonAllon()
{
	if( m_fImport != FALSE ){
		for(int i=0; i<GetSheetNum(); i++){
			m_report1.AllowEditItem( i, 1, TRUE );
		}
	}

	CheckBoxControl( TRUE );

	m_report1.Populate();
}

void CdlgImpOpt::OnBnClickedButtonAlloff()
{
	if( m_fImport != FALSE ){
		for(int i=0; i<GetSheetNum(); i++){
			m_report1.AllowEditItem( i, 1, FALSE );
		}
	}

	CheckBoxControl( FALSE );

	m_report1.Populate();
}

void CdlgImpOpt::OnBnClickedButtonImpStart()
{
	int cntFalse = 0;
	for(int i=0; i<GetSheetNum(); i++){
		if( m_report1.IsChecked(i, m_clmCheck ) == FALSE){
			cntFalse++;
		}
	}

	if( GetSheetNum() == cntFalse ){
// midori 155282 del -->
		//::ICSMessageBox( "出力状態が選択されていません。", 0, 0, 0, this );
// midori 155282 del <--
// midori 155282 add -->
	// インポート
// midori 155315 del -->
	//if (m_fImport == TRUE)		::ICSMessageBox( "取り込みを行うCSVファイルが選択されていません。", 0, 0, 0, this );
// midori 155315 del <--
// midori 155315 add -->
	if (m_fImport == TRUE)		::ICSMessageBox( "取込するCSVファイルが選択されていません。", 0, 0, 0, this );
// midori 155315 add <--
	// エクスポート
	else						::ICSMessageBox( "出力するCSVファイルが選択されていません。", 0, 0, 0, this );
// midori 155282 add <--
		return;
	}

	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

void CdlgImpOpt::OnBnClickedButtonCancel()
{
	m_nRet = ID_DLG_CANCEL;

	ICSDialog::OnCancel();
}

int CdlgImpOpt::ShowDialog(CDBZmSub* pZmSub, BOOL fImport, CArray<ImpOpt, ImpOpt&> &arrImpStct)
{
	//引数→メンバ変数
	m_fImport = fImport;
	m_datImp.Copy( arrImpStct );
	m_pZmSub = pZmSub;

	if( m_fImport != FALSE ){
		m_clmCheck			= 0;
		m_clmTrans			= 1;
		m_clmYoushikiNo		= 2;
		m_clmYoushikiName	= 3;
		m_nClm				= 4;
	}
	else{
		m_clmCheck			= 0;
		m_clmTrans			= -1;
		m_clmYoushikiNo		= 1;
		m_clmYoushikiName	= 2;
		m_nClm				= 3;
	}

	// モーダルでダイアログを表示
	DoModal();

	// [OK]ボタンが押されたらフォルダパスを返す
	if (m_nRet == ID_DLG_OK) {
		//チェック状態を調べる
		GetCheckComboState();

// 24/01/16_ScoreLink cor -->
		//// owntbに書き込み
		//SetOwnTblInfo();
// --------------------------
		// ScoreLinkからの呼び出しでない場合
		if(bG_ScoreLink == FALSE)	{
			// owntbに書き込み
			SetOwnTblInfo();
		}
// 24/01/16_ScoreLink cor <--

		// 呼び出し元に返す
		arrImpStct.Copy( m_datImp );
	}

	return m_nRet;
}

int CdlgImpOpt::GetSheetNum()
{
	return (int)m_datImp.GetCount();
}

void CdlgImpOpt::GetCheckComboState()
{
	for(int i=0; i<GetSheetNum(); i++){
		m_datImp.GetAt(i).chkState = m_report1.IsChecked( i, CLM_CHKBOX );
		if( m_fImport != FALSE ){
			m_datImp.GetAt(i).cmbTrans = m_report1.GetCurSel( i, CLM_CMBBOX );
		}
	}
}
void CdlgImpOpt::OnNMReportCheckedCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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
	//チェックされた位置
	POINT pt	=	pItemNotify->pt;

	if( m_fImport != FALSE ){
		if( bCheck == FALSE ){
			m_report1.AllowEditItem( indexRow, 1, FALSE );
		}
		else{
			m_report1.AllowEditItem( indexRow, 1, TRUE );

			//⑭－２：人件費の内訳書は上書き固定なため，入力を常に不許可にする
			CString strCurIndex = m_report1.GetCaption(indexRow, 2);
			if( strCurIndex.Compare( UcSheetNumber[16] ) == 0 ){
				m_report1.AllowEditItem( indexRow, 1, FALSE );
			}
		}
	}
}

// 
// 非表示 
//
void CdlgImpOpt::OnBnClickedButtonChgTrans()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int						indexRow	= 0;
	ICSReportRow*			pRow		= NULL;
	ICSReportSelectedRows*	pSelRows	= NULL;

	// 選択されている行を取得
	pSelRows = m_report1.GetSelectedRows();
	for(int i=0; i<pSelRows->GetCount(); i++){
		// 選択行(ICSReportRowクラス)を順次取得
		pRow = pSelRows->GetAt(i);

		if( pRow == NULL ){
			return;
		}
		// 選択行のインデックス取得
		indexRow = pRow->GetIndex();

		if( m_report1.IsChecked( indexRow, m_clmCheck ) != FALSE ){
			if( m_report1.GetCurSel( indexRow, 1 ) == TRANS_OVERWRITE ){		// 上書きの場合
				// ⑭－２：人件費の内訳書の場合は何もしない
				if( m_report1.GetCaption( indexRow, 2 ).Compare(UcSheetNumber[16] ) != 0 ){			
					m_report1.SetCurSel( indexRow, 1, 1 );
				}
			}
			else{	// 追加の場合
				m_report1.SetCurSel( indexRow, 1, 0 );
			}
		}
	}	

	// フォーカスをレポートコントロールに戻す
	m_report1.SetFocus();
	
	// 選択状態を元に戻す
	for(int i=0; i<pSelRows->GetCount(); i++){
		pRow = pSelRows->GetAt(i);

		if( pRow = NULL)
			return;

		pSelRows->Select( pRow );
	}	
}

int CdlgImpOpt::SetOwnTblInfo()
{
	int			nRet =	0;
	CString		strFilter;	strFilter.Empty();
	
	strFilter.Format( _T("apl_name = '%s' AND itm_name = '%s'"),
						OWNTB_APL_NAME,
						OWNTB_ITM_NAME);

	if(m_pZmSub->owntb == NULL){
		if( m_pZmSub->OwnTblOpen( strFilter ) != 0 ){
			return -1;
		}
	}

	// 初めての実行ならOwn Info Record 新規追加
	if ( m_pZmSub->owntb->st == -1 ) {
		m_pZmSub->owntb->AddNew();
		m_pZmSub->owntb->apl_name	=	OWNTB_APL_NAME;			// アプリケーション名
		m_pZmSub->owntb->itm_name	=	OWNTB_ITM_NAME;			// アイテム名
		m_pZmSub->owntb->u_id		=	OWNTB_USER_ID;			// ユーザーＩＤ

// midori 170504 del -->
		//// インポートエクスポートで分けると新規レコード追加時に初期値が入らないので廃止
		////if( m_fImport != FALSE ){
		//	m_pZmSub->owntb->vl[0] = UpdateState(m_stateTrans, TYPE_TRANS);
		//	m_pZmSub->owntb->vl[1] = UpdateState(m_stateImpOut, TYPE_OUT);
		////}
		////else{
		//	m_pZmSub->owntb->vl[2] = UpdateState(m_stateExpOut, TYPE_OUT);
		////}
// midori 170504 del <--
// midori 170504 add -->
		m_pZmSub->owntb->vl[0] = UpdateState(m_stateTrans[0], TYPE_TRANS,0);
		m_pZmSub->owntb->vl[1] = UpdateState(m_stateImpOut[0], TYPE_OUT,0);
		m_pZmSub->owntb->vl[2] = UpdateState(m_stateExpOut[0], TYPE_OUT,0);
		m_pZmSub->owntb->vl[3] = UpdateState(m_stateTrans[1], TYPE_TRANS,1);
		m_pZmSub->owntb->vl[4] = UpdateState(m_stateImpOut[1], TYPE_OUT,1);
		m_pZmSub->owntb->vl[5] = UpdateState(m_stateExpOut[1], TYPE_OUT,1);
		m_pZmSub->owntb->vl[6] = UpdateState(m_stateTrans[2], TYPE_TRANS,2);
		m_pZmSub->owntb->vl[7] = UpdateState(m_stateImpOut[2], TYPE_OUT,2);
		m_pZmSub->owntb->vl[8] = UpdateState(m_stateExpOut[2], TYPE_OUT,2);
// midori 170504 add <--
	}
	// Own Info Record 取得
	else {
		m_pZmSub->owntb->Edit();
// midori 170504 del -->
		//if( m_fImport != FALSE ){
		//	m_pZmSub->owntb->vl[0] = UpdateState(m_stateTrans, TYPE_TRANS);
		//	m_pZmSub->owntb->vl[1] = UpdateState(m_stateImpOut, TYPE_OUT);
		//}
		//else{
		//	m_pZmSub->owntb->vl[2] = UpdateState(m_stateExpOut, TYPE_OUT);
		//}
// midori 170504 del <--
// midori 170504 add -->
		if( m_fImport != FALSE ){
			// ① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
			m_pZmSub->owntb->vl[0] = UpdateState(m_stateTrans[0], TYPE_TRANS,0);
			m_pZmSub->owntb->vl[1] = UpdateState(m_stateImpOut[0], TYPE_OUT,0);
			// ⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
			m_pZmSub->owntb->vl[3] = UpdateState(m_stateTrans[1], TYPE_TRANS,1);
			m_pZmSub->owntb->vl[4] = UpdateState(m_stateImpOut[1], TYPE_OUT,1);
			// ⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
			m_pZmSub->owntb->vl[6] = UpdateState(m_stateTrans[2], TYPE_TRANS,2);
			m_pZmSub->owntb->vl[7] = UpdateState(m_stateImpOut[2], TYPE_OUT,2);
		}
		else{
			// ① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
			m_pZmSub->owntb->vl[2] = UpdateState(m_stateExpOut[0], TYPE_OUT,0);
			// ⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
			m_pZmSub->owntb->vl[5] = UpdateState(m_stateExpOut[1], TYPE_OUT,1);
			// ⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
			m_pZmSub->owntb->vl[8] = UpdateState(m_stateExpOut[2], TYPE_OUT,2);
		}
// midori 170504 add <--
	}
	// レコード更新
	m_pZmSub->owntb->Update();

	// レコードクローズ
	m_pZmSub->OwnTblClose();


	return	nRet;
}

int CdlgImpOpt::GetOwnTblInfo()
{
	CString		strFilter;
	strFilter.Format(  _T("apl_name = '%s' AND itm_name = '%s'"),
						OWNTB_APL_NAME,
						OWNTB_ITM_NAME );

	if( m_pZmSub->owntb == NULL ) {
		if( m_pZmSub->OwnTblOpen() == -1 )
			return -1;
	}

	// Own Info Record 新規追加
	if ( m_pZmSub->owntb->Requery( strFilter ) == -1 ) {
		m_pZmSub->owntb->AddNew();
		m_pZmSub->owntb->apl_name	=	OWNTB_APL_NAME;		// アプリケーション名
		m_pZmSub->owntb->itm_name	=	OWNTB_ITM_NAME;		// アイテム名
		m_pZmSub->owntb->u_id		=	OWNTB_USER_ID;		// ユーザーＩＤ

// midori 170504 del -->
		////if( m_fImport != FALSE ){
		//	m_pZmSub->owntb->vl[0]		=	m_stateTrans;		// 転送方式
		//	m_pZmSub->owntb->vl[1]		=	m_stateImpOut;		// インポート出力状態
		////}
		////else{
		//	m_pZmSub->owntb->vl[2]		=	m_stateExpOut;		// エクスポート出力状態
		////}
// midori 170504 del <--
// midori 170504 add -->
		// ① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
		m_pZmSub->owntb->vl[0]		=	m_stateTrans[0];		// 転送方式
		m_pZmSub->owntb->vl[1]		=	m_stateImpOut[0];		// インポート出力状態
		m_pZmSub->owntb->vl[2]		=	m_stateExpOut[0];		// エクスポート出力状態
		// ⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
		m_pZmSub->owntb->vl[3]		=	m_stateTrans[1];		// 転送方式
		m_pZmSub->owntb->vl[4]		=	m_stateImpOut[1];		// インポート出力状態
		m_pZmSub->owntb->vl[5]		=	m_stateExpOut[1];		// エクスポート出力状態
		// ⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
		m_pZmSub->owntb->vl[6]		=	m_stateTrans[2];		// 転送方式
		m_pZmSub->owntb->vl[7]		=	m_stateImpOut[2];		// インポート出力状態
		m_pZmSub->owntb->vl[8]		=	m_stateExpOut[2];		// エクスポート出力状態
// midori 170504 add <--

		m_pZmSub->owntb->Update();
		if ( m_pZmSub->owntb->Requery( strFilter ) == -1 )
			return	-1;
	}
	// Own Info Record 取得
	else {
// midori 170504 del -->
		//if( m_fImport != FALSE ){
		//	m_stateTrans = (int)m_pZmSub->owntb->vl[0];			// 転送方式
		//	m_stateImpOut = (int)m_pZmSub->owntb->vl[1];		// インポート出力状態
		//}
		//else{
		//	m_stateExpOut = (int)m_pZmSub->owntb->vl[2];		// エクスポート出力状態
		//}
// midori 170504 del <--
// midori 170504 add -->
		if( m_fImport != FALSE ){
			// ① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
			m_stateTrans[0] = (int)m_pZmSub->owntb->vl[0];		// 転送方式
			m_stateImpOut[0] = (int)m_pZmSub->owntb->vl[1];		// インポート出力状態
			// ⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
			m_stateTrans[1] = (int)m_pZmSub->owntb->vl[3];		// 転送方式
			m_stateImpOut[1] = (int)m_pZmSub->owntb->vl[4];		// インポート出力状態
			// ⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
			m_stateTrans[2] = (int)m_pZmSub->owntb->vl[6];		// 転送方式
			m_stateImpOut[2] = (int)m_pZmSub->owntb->vl[7];		// インポート出力状態
// midori 152193 add -->
			// 既にテーブルが作られている場合、vl[3]、vl[4]、vl[6]、vl[7]は初期値-1が入ってるため、
			// -1の場合は各項目にデフォルト値を代入する
			// ⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
			if(m_stateTrans[1] == -1)		m_stateTrans[1] = 0;
			if(m_stateImpOut[1] == -1)		m_stateImpOut[1] = 0x000fffff;
			// ⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
			if(m_stateTrans[2] == -1)		m_stateTrans[2] = 0;
			if(m_stateImpOut[2] == -1)		m_stateImpOut[2] = 0x000fffff;
// midori 152193 add <--
		}
		else{
			// ① 預貯金等の内訳書～⑯－２ 雑損失等の内訳書
			m_stateExpOut[0] = (int)m_pZmSub->owntb->vl[2];		// エクスポート出力状態
			// ⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
			m_stateExpOut[1] = (int)m_pZmSub->owntb->vl[5];		// エクスポート出力状態
			// ⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
			m_stateExpOut[2] = (int)m_pZmSub->owntb->vl[8];		// エクスポート出力状態
// midori 152193 add -->
			// 既にテーブルが作られている場合、vl[3]、vl[4]、vl[6]、vl[7]は初期値-1が入ってるため、
			// -1の場合は各項目にデフォルト値を代入する
			// ⑰－１ その他科目の内訳書①－１～⑰－２０ その他科目の内訳書①－２０
			if(m_stateExpOut[1] == -1)	m_stateExpOut[1] = 0x000fffff;
			// ⑱－１ その他科目の内訳書②－１～⑱－２０ その他科目の内訳書②－２０
			if(m_stateExpOut[2] == -1)	m_stateExpOut[2] = 0x000fffff;
// midori 152193 add <--
		}
// midori 170504 add <--
	}

	// レコードクローズ
	m_pZmSub->OwnTblClose();

	return 0;
}

// midori 170504 del -->
//int CdlgImpOpt::UpdateState(int state, int type)
// midori 170504 del <--
// midori 170504 add -->
int CdlgImpOpt::UpdateState(int state, int type, int psw)
// midori 170504 add <--
{
	for(int i=0; i<(int)m_datImp.GetCount(); i++){
		int nVal = -1;
		int check = -1;
// midori 170504 add -->
		int	sw=0;

		if(psw == 0)	{
			sw=0;
			for(int jj=0;jj<=21;jj++)	{
				if(m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[jj]) == 0)	{
					sw=1;
					break;
				}
			}
			if(sw == 0)	{
				continue;
			}
		}
		else if(psw == 1)	{
			sw=0;
			for(int jj=22;jj<=41;jj++)	{
				if(m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[jj]) == 0)	{
					sw=1;
					break;
				}
			}
			if(sw == 0)	{
				continue;
			}
		}
		else if(psw == 2)	{
			sw=0;
			for(int jj=42;jj<=61;jj++)	{
				if(m_datImp.GetAt(i).SheetNo.Compare(UcSheetNumber[jj]) == 0)	{
					sw=1;
					break;
				}
			}
			if(sw == 0)	{
				continue;
			}
		}
// midori 170504 add <--

		switch(type){
			case TYPE_TRANS:
				nVal	= m_datImp.GetAt(i).cmbTrans;
				check	= TRANS_ADD;
				break;
			case TYPE_OUT:
				nVal	= m_datImp.GetAt(i).chkState;
				check	= TRUE;
				break;
		}

		if( nVal == check ){
			state |= m_datImp.GetAt(i).bitTarget;
		}
		else{
			state &= ~m_datImp.GetAt(i).bitTarget;
		}
	}

	return state;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgImpOpt::PreTranslateMessage(MSG* pMsg)
{
	ICSReportRecordItem		*pItem1		= NULL;
	ICSReportRecordItem		*pItem2		= NULL;
	ICSReportRow			*pRow		= NULL;
	ICSReportSelectedRows	*pSelRows	= NULL;
	int indexRow;
	int indexClm;
	CString strCaption;

	// ダイアログにフォーカスがない場合は処理しない
	// （他のウィンドウに移った時など）
	if( GetFocus() != NULL){
		if( pMsg->message == WM_KEYDOWN ) {
			// レポートコントロールにフォーカスが当たっているときのみチェック
			// ※他のコントロールはICSモジュールを使用しているため、
			// 　下記の機能は実装されている
			if( GetFocus()->GetDlgCtrlID() == m_report1.GetDlgCtrlID() ){
				// キーが押された
				if( pMsg->message == WM_KEYDOWN ) {
					switch (pMsg->wParam) {
					case VK_RETURN:		// Enterキー
						// 選択行インデックスの取得
						pSelRows	= m_report1.GetSelectedRows();
						pRow		= pSelRows->GetAt(0);
						indexRow	= pRow->GetIndex();

						pItem1 = m_report1.GetActiveItem();
						if( pItem1 != NULL ){
							indexClm = pItem1->GetIndex();
							if( indexClm == m_clmCheck ){	// フォーカスがチェックボックス
								if( m_fImport != FALSE ){	// インポート
									if( m_report1.IsChecked( indexRow, indexClm) != FALSE ){	// ON→同じ行のコンボボックスへフォーカス
										m_report1.EditItem( indexRow, m_clmTrans, TRUE );
									}
									else{	// OFF→次の行のチェックボックスへフォーカス
										m_report1.MoveDown( FALSE, FALSE );
										pSelRows = m_report1.GetSelectedRows();
										pRow = pSelRows->GetAt(0);	// 選択行は1行のみなので先頭要素を抜き出す
										indexRow = pRow->GetIndex();
										m_report1.EditItem( indexRow, m_clmCheck, TRUE );
									}
								}
								else{	// エクスポート 転送方式がないので、次のチェックボックスに移動
									m_report1.MoveDown( FALSE, FALSE );
									pSelRows = m_report1.GetSelectedRows();
									pRow = pSelRows->GetAt(0);	// 選択行は1行のみなので先頭要素を抜き出す
									indexRow = pRow->GetIndex();
									m_report1.EditItem( indexRow, m_clmCheck, TRUE );
								}
							}
							else{	// フォーカスがコンボボックス
								// ターミネーションの発生で制御している
								// ⑭－２はエディットが入らず、ターミネーションが発生しないので処理追加
								m_report1.MoveDown( FALSE, FALSE );
								pSelRows = m_report1.GetSelectedRows();
								pRow = pSelRows->GetAt(0);	// 選択行は1行のみなので先頭要素を抜き出す
								indexRow = pRow->GetIndex();
								m_report1.EditItem( indexRow, m_clmCheck, TRUE );
							}
						}
						else{
							m_report1.EditItem( indexRow, m_clmCheck, TRUE );
						}
						return TRUE;
					case VK_SPACE:		// Spaceキー
						// 選択行インデックスの取得
						pSelRows	= m_report1.GetSelectedRows();
						pRow		= pSelRows->GetAt(0);
						indexRow	= pRow->GetIndex();

						// 選択カラムインデックスの取得
						pItem1 = m_report1.GetActiveItem();
						if( pItem1 != NULL ){
							indexClm = pItem1->GetIndex();
							if( indexClm == m_clmCheck ){
								if( m_report1.IsChecked(indexRow, indexClm ) != FALSE ){
									m_report1.SetChecked(indexRow, indexClm, FALSE );
									if( m_fImport != FALSE )
										m_report1.AllowEditItem( indexRow, m_clmTrans, FALSE );	// コンボボックスの入力付加
								}
								else{
									m_report1.SetChecked(indexRow, indexClm, TRUE );
									if( m_fImport != FALSE ){
										strCaption = m_report1.GetCaption( indexRow, m_clmYoushikiNo );	//⑭－２：人件費の内訳書は上書き固定なため，入力を常に不許可にする
										if( strCaption.Compare( UcSheetNumber[16] ) != 0 ){
											m_report1.AllowEditItem( indexRow, m_clmTrans, TRUE );
										}
									}
								}
								m_report1.Populate();
								m_report1.EditItem( indexRow, indexClm, TRUE );
							}
						}
						return TRUE;
					}
				}
			}
			else{
				// キーが押された
				if( pMsg->message == WM_KEYDOWN ) {
					switch (pMsg->wParam) {
						// コンボボックスのエディット中に←→で要素が消えないように
						case VK_RIGHT:
						case VK_LEFT:
						case VK_DELETE:
							return TRUE;
					}
				}
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
void CdlgImpOpt::OnNMReportEditkeydownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();
	//ターミネーションキーコード
	UINT	nChar	=	pItemNotify->nChar;

	ICSReportRecords	*pRecords = NULL;
	int nRecords = 0;

	pRecords = m_report1.GetRecords();
	nRecords = pRecords->GetCount();

	// コンボボックスがエディット状態のときにEnterキーが押されると、
	// 次のレコードのチェックボックスに入力をいれる
	switch( nChar ){
		case VK_RETURN:
			if( (indexRow+1) < nRecords ){
				m_report1.EditItem( indexRow+1, indexItem-1, TRUE );
			}
			break;
	}
}

