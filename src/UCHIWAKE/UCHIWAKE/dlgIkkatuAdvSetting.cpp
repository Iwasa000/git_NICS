// dlgIkkatuAdvSetting.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgIkkatuAdvSetting.h"

// No.200903 add -->
extern	BOOL	bG_Kanso;
// No.200903 add <--

// CdlgIkkatuAdvSetting ダイアログ

IMPLEMENT_DYNAMIC(CdlgIkkatuAdvSetting, ICSDialog)

CdlgIkkatuAdvSetting::CdlgIkkatuAdvSetting(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgIkkatuAdvSetting::IDD, pParent),m_nRet(ID_DLG_CANCEL),m_RowMax(0)
{

}

CdlgIkkatuAdvSetting::~CdlgIkkatuAdvSetting()
{
}

void CdlgIkkatuAdvSetting::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1_IKK_ADV_REPORT, m_report);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CdlgIkkatuAdvSetting, ICSDialog)
	ON_BN_CLICKED(IDOK, &CdlgIkkatuAdvSetting::OnBnClickedOk)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_CUSTOM1_IKK_ADV_REPORT, OnNMReportEditkeydownCUSTOM1_IKK_ADV_REPORT)
END_MESSAGE_MAP()


// CdlgIkkatuAdvSetting メッセージ ハンドラ

// No.200903 del -->
//int CdlgIkkatuAdvSetting::ShowDialog(IkkatuValHandler& IkkatuVal, BOOL& isChange)
// No.200903 del <--
// No.200903 add -->
int CdlgIkkatuAdvSetting::ShowDialog(IkkatuValHandler& IkkatuVal, BOOL& isChange, int pFormSeq)
// No.200903 add <--
{
	// 呼び出し元から変数引継
	m_IkkatuValHandler = IkkatuVal;
// No.200903 add -->
	m_FormSeq = pFormSeq;
// No.200903 add <--

	// ダイアログ表示
	DoModal();

	// OKボタンが押下されたなら、変更を呼び出し元へ返す
	if(m_nRet == ID_DLG_OK){
		if(IkkatuVal.GetMap() != m_IkkatuValHandler.GetMap()){
			isChange = TRUE;
			IkkatuVal = m_IkkatuValHandler;
		}
	}

	return DB_ERR_OK;
}
void CdlgIkkatuAdvSetting::OnBnClickedOk()
{
	m_nRet = ID_DLG_OK;

	ICSReportRecords* rrs = m_report.GetRecords();
	int record_num = rrs->GetCount();
	map<long, IKKATU_ADV_DATA>::iterator it = m_IkkatuValHandler.GetMap().begin();
	for(int row=0; row<record_num; row++, it++){
		if(it != m_IkkatuValHandler.GetMap().end()){
			// 入力された一括金額をセット
			(*it).second.ikkatu_val = m_report.GetCaption(row, ias_val);
// 20-0873 add -->
			// Deleteキーで消去した場合、０円と判定されない不具合対応
			if((*it).second.ikkatu_val.IsEmpty() == TRUE)	{
				(*it).second.ikkatu_val = _T("0");
			}
// 20-0873 add <--
		}
	}

	OnOK();
}

BOOL CdlgIkkatuAdvSetting::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 倍長フィールド指定
	l_defn( 0x16 );

	// レポートコントロールヘッダ設定
	InitReportHeader();
	// レコード追加
	AddRecords();
	// 最初のフォーカスセット
	m_report.SetFocus();
	// 最大行数取得
	ICSReportRecords* rrs = m_report.GetRecords();
	if(rrs != NULL)	m_RowMax = rrs->GetCount();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//**************************************************
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgIkkatuAdvSetting::InitReportHeader()
{
	//m_report.Resize( TRUE, ICSResizeDefault );
	
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
	m_report.AddColumn(new ICSReportColumn(0, NULL, 5));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 3));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

// 20-0873 add -->
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255,0,0));
	m_report.SetVerGrid(&arryGridColumn,&arrayGridColor,RGS_SOLID);
// 20-0873 add <--

	ICSReportColumns *m_pClms = m_report.GetColumns();
	int cnt = m_IkkatuValHandler.GetHeaderNum();
	for(int i = 0; i < cnt; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		m_pClm->SetCaption(m_IkkatuValHandler.GetHeader(i));
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
void CdlgIkkatuAdvSetting::AddRecords()
{
// No.200903 add -->
	int		len=0;
// No.200903 add <--

	map<long, IKKATU_ADV_DATA> IkkatuVal = m_IkkatuValHandler.GetMap();
	for(map<long, IKKATU_ADV_DATA>::iterator it = IkkatuVal.begin(); it != IkkatuVal.end(); it++){
		ICSReportRecord*  pRecord  =  m_report.AddRecord(new ICSReportRecord());
		pRecord->AddItem( new ICSReportRecordItemText((*it).second.name));	// 科目名称

		// 金額にカンマをつける
		char buf[6] = {0}, buf2[32] = {0};
		l_input(buf, (char*)((LPCTSTR)(*it).second.ikkatu_val));
// No.200903 del -->
		//l_print( buf2, buf, FMT12 );
// No.200903 del <--
// No.200903 add -->
		if(bG_Kanso == TRUE)	{
			switch(m_FormSeq)	{
				case ID_FORMNO_041:		// ④-1仮払金（前渡金）の内訳書
				case ID_FORMNO_042:		// ④-2貸付金及び受取利息の内訳書
				case ID_FORMNO_061:		// ⑥  有価証券の内訳書
				case ID_FORMNO_071:		// ⑦  固定資産の内訳書
				case ID_FORMNO_101:		// ⑩-1仮受金の内訳書
				case ID_FORMNO_111:		// ⑪  借入金及び支払利子の内訳書
				case ID_FORMNO_131:		// ⑬  売上高等の事業所別内訳書
				case ID_FORMNO_151:		// ⑮-1地代家賃の内訳書
				case ID_FORMNO_152:		// ⑮-2権利金等の期中支払の内訳書
				case ID_FORMNO_153:		// ⑮-3工業所有権等の使用料の内訳書
				case ID_FORMNO_161:		// ⑯-1雑益等の内訳書
				case ID_FORMNO_162:		// ⑯-2雑損失等の内訳書
					// 明細12桁
					l_print(buf2, buf, FMT12);
					len=12;
					break;

				default:				// それ以外の様式
					// 明細13桁
					l_print(buf2, buf, FMT13);
					len=13;
					break;
			}
		}
		else	{
			len=12;
			l_print( buf2, buf, FMT12 );
		}
// No.200903 add <--

		CString val;
		val.Format("%s", buf2);
		val.TrimLeft();
// No.200903 del -->
		//pRecord->AddItem( new ICSReportRecordItemText(val, ICS_EDS_NUMERIC, TRUE, 1, 12, ICS_EDS_RIGHT, TRUE));	// 一括金額
// No.200903 del <--
// No.200903 add -->
		pRecord->AddItem( new ICSReportRecordItemText(val, ICS_EDS_NUMERIC, TRUE, 1, len, ICS_EDS_RIGHT, TRUE));	// 一括金額
// No.200903 add <--
	}

	// ここ以降でないと、効かない
	m_report.AllowEdit(TRUE);						// 全て編集許可にしたあと
	m_report.AllowEditColumn(ias_name, FALSE);		// 科目名称はEdit不可に

	// 行の高さ
	m_report.SetItemHeight(20);
}
BOOL CdlgIkkatuAdvSetting::PreTranslateMessage(MSG* pMsg)
{
	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		switch( pMsg->wParam ){
			case VK_RETURN:
				if(m_clsFunc.GetFocusControlID(this) == IDC_CUSTOM1_IKK_ADV_REPORT){	// エディットを1行目に入れる
					m_report.EditItem(0, ias_val, TRUE);
					return 1;
				}
				break;
			case VK_DECIMAL:		// テンキーの「.」
			case VK_OEM_PERIOD:		// .（かなの「る」）
			case VK_SUBTRACT:		// テンキーの「-」
			case VK_OEM_MINUS:		// -（かなの「ほ」）
				return 1;
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

void CdlgIkkatuAdvSetting::OnNMReportEditkeydownCUSTOM1_IKK_ADV_REPORT(NMHDR * pNotifyStruct, LRESULT * result)
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

	// キー操作へ
	BindKey(indexRow, nChar);
}

void CdlgIkkatuAdvSetting::BindKey(int row, int key)
{
	switch(key){
		// 次の行へ or 前の行へ
		case VK_RETURN:{
			char cShift =( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得
			int dist = row;
			if(cShift & 0x80){	// 前の行へ
				if(row != 0)	dist--;			// 1行目ならその場に留まる それ以外は前の行に移動
			}
			else{				// 次の行へ
				// 最終行ならOKボタンへ
				if(m_RowMax == dist + 1){
					NextDlgCtrl();
					return;
				}
				// それ以外は次の行に移動
				dist++;
			}
			m_report.EditItem(dist, ias_val, TRUE);	
			break;}
		// 次の行へ
		case VK_DOWN:
			m_report.EditItem(row + 1, ias_val, TRUE);
			break;
		// 前の行へ
		case VK_UP:
			m_report.EditItem(row - 1, ias_val, TRUE);
			break;
		// 次のコントロールへ or 前のコントロールへ
		case VK_TAB:{
			char cShift =( char )GetKeyState( VK_SHIFT );	//	SHIFTキー状態取得
			if(cShift & 0x80)	PrevDlgCtrl();
			else				NextDlgCtrl();
			break;}
	}
}