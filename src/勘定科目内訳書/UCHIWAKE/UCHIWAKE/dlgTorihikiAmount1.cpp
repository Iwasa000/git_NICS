// dlgTorihikiAmount1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgTorihikiAmount1.h"

extern	BOOL	bG_Kanso;

// CdlgTorihikiAmount1 ダイアログ

IMPLEMENT_DYNAMIC(CdlgTorihikiAmount1, ICSDialog)

CdlgTorihikiAmount1::CdlgTorihikiAmount1(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_TORIHIKI_AMOUNT1, pParent)
{

}

CdlgTorihikiAmount1::~CdlgTorihikiAmount1()
{
}

void CdlgTorihikiAmount1::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1_REPORT, m_report);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_ICSDIAG_BC, m_DiagBC);
}

BEGIN_MESSAGE_MAP(CdlgTorihikiAmount1, ICSDialog)
	ON_BN_CLICKED(IDOK, &CdlgTorihikiAmount1::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CdlgTorihikiAmount1 メッセージ ハンドラー

// 158125 del -->
//// 158057 del -->
////int CdlgTorihikiAmount1::ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv )
//// 158057 del <--
//// 158057 add -->
//int CdlgTorihikiAmount1::ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv, IkkatuValHandler& IkkatuVal )
//// 158057 add <--
// 158125 del <--
// 158125 add -->
int CdlgTorihikiAmount1::ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, int pUseAdv, IkkatuValHandler& IkkatuVal, int pZero )
// 158125 add <--
{
	m_pDB		= pDB;
	m_FormSeq	= pFormSeq;
	m_IkkatuVal	= pIkkatuVal;
	m_UseAdv	= pUseAdv;
// 158057 add -->
	m_IkkatuValHandler = IkkatuVal;
// 158057 add <--
// 158125 add -->
	m_Zero		= pZero;
// 158125 add <--

	// ダイアログ表示
	DoModal();

	return DB_ERR_OK;
}

void CdlgTorihikiAmount1::OnBnClickedOk()
{
	m_nRet = ID_DLG_OK;

	OnCancel();
}

BOOL CdlgTorihikiAmount1::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 倍長フィールド指定
	l_defn(0x16);

	DIAGRAM_ATTRIBUTE	diaatt;
	ICSColorInfo		ic;
	int					rv=((CUCHIWAKEApp *)AfxGetApp())->GetStanderdColor(ic);

	if(rv == 0 && ic.m_swOnOff == 1)	{
		m_DiagBC.SetBackColor(ic.ViewColor());
		m_DiagBC.GetAttribute(2,(LPUNKNOWN)&diaatt);
		diaatt.attr_bcolor = ic.ViewColor();
		m_DiagBC.SetAttribute(2,(LPUNKNOWN)&diaatt,TRUE);
	}
	m_DiagBC.GetAttribute(0,(LPUNKNOWN)&diaatt);
	diaatt.attr_bcolor = COLOR_IKKATUAUTO;
	m_DiagBC.SetAttribute(0,(LPUNKNOWN)&diaatt,TRUE);

	// レポートコントロールヘッダ設定
	InitReportHeader();
	// レコード追加
	AddRecords();
	// 最初のフォーカスセット
	m_report.SetFocus();
	// 最大行数取得
	//ICSReportRecords* rrs = m_report.GetRecords();
	//if(rrs != NULL)	m_RowMax = rrs->GetCount();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//**************************************************
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgTorihikiAmount1::InitReportHeader()
{
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
	m_report.AddColumn(new ICSReportColumn(0, NULL, 5));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 3));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	// 縦線(赤)
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255,0,0));
	m_report.SetVerGrid(&arryGridColumn,&arrayGridColor,RGS_SOLID);

//	ICSReportColumns *m_pClms = m_report.GetColumns();
//	int cnt = m_IkkatuValHandler.GetHeaderNum();
//	for(int i = 0; i < cnt; i++) {
//		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
//		m_pClm->SetCaption(m_IkkatuValHandler.GetHeader(i));
//	}

	ICSReportColumns	*m_pClms = m_report.GetColumns();
	CString				title=_T("");
	for(int i = 0; i < 2; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		if(i == 0)	title = _T("取引先");
		else		title = _T("金額");
		m_pClm->SetCaption(title);
	}

	// フォント設定
	CFont*		pFont;
	LOGFONT		font = {0};

	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_report.SetTextFont(font);
	m_report.SetHeaderFont(font);
}

// 158063 del -->
////**************************************************************
////	取引先の総額を取得するSQL文を作成
////	【引数】	CDatabase*		データベース操作クラス
////				int				様式番号
////	【戻値】	CString			作成したSQL文
////**************************************************************
//CString CdlgTorihikiAmount1::MakeSql( CDatabase* pDB, int pFormSeq )
//{
//	CString				cs=_T("");
//	CString				tbl=_T("");
//	CUcFunctionCommon	ufc;
//	int					is50ont=0,dummy=0;
//
//	if(pFormSeq == ID_FORMNO_021)	tbl = _T("uc_021_uketoritegata");
//	else							tbl = _T("uc_081_siharaitegata");
//
//	ufc.IsSort(m_FormSeq,pDB,&dummy,&is50ont);
//
//	cs.Format(_T("select a.AdOrder,a.AdName1,a.AdName2,sum(Val) as amount,b.IkkatuVal from %s a"),tbl);
//	cs += _T(" left join uc_lst_address b on a.AdSeq=b.AdSeq");
//	cs.Format(cs + _T(" where (FgShow=%d or FgShow=%d) and FgFunc=%d"),ID_FGSHOW_OFF,ID_FGSHOW_IKKATU,ID_FGFUNC_DATA);
//	cs += _T(" group by a.AdOrder,a.AdName1,a.AdName2,a.AdKana,b.IkkatuVal");
//	if(is50ont == 0)	{
//		// 番号順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdOrder ASC,");
//		cs += _T("(a.AdName1 + a.AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName2 COLLATE Japanese_CS_AS_KS_WS ASC");
//	}
//	else	{
//		// 50音順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdKana COLLATE Japanese_CS_AS_KS_WS ASC, a.AdOrder ASC");
//	}
//
//	return(cs);
//}
// 158063 del <--

//**************************************************************
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	TRUE:成功
//				FALSE:失敗
//**************************************************************
void CdlgTorihikiAmount1::AddRecords()
{
	CRecordset			rs(m_pDB);
	CUcFunctionCommon	ufc;
	CString				dt[5];
	CString				val=_T("");
	int					len=0;
	int					index=0,max=0;
// 158057 add -->
	long				order=0;
// 158057 add <--
	short				scnt=0;
	char				vl1[6]={0},vl2[6]={0},bf[32]={0};
	CArray<int,int>		iktind;
// 158138 add -->
	CdbUcInfMain		mfcRecMain(m_pDB);
	BOOL				regautoref=TRUE;

	mfcRecMain.Init();
	CCtrlMainGeneralVar	cm(mfcRecMain);
	regautoref = cm.bRegAutoRef;
	mfcRecMain.Fin();
// 158138 add <--

	iktind.RemoveAll();

	l_input(vl2,m_IkkatuVal.GetBuffer());

// 158063 del -->
	//rs.Open(CRecordset::forwardOnly,MakeSql(m_pDB,m_FormSeq));
// 158063 del <--
// 158063 add -->
	rs.Open(CRecordset::forwardOnly,ufc.MakeAmountSql1(m_pDB,m_FormSeq));
// 158063 add <--
	while(!rs.IsEOF())	{
		for(scnt=0; scnt<5; scnt++)	{
			rs.GetFieldValue(scnt,dt[scnt]);
		}
		ICSReportRecord*  pRecord = m_report.AddRecord(new ICSReportRecord());
		// 取引先名称
// 158138 del -->
//// 158065,158066 add -->
//		if(_tstoi(dt[0]) == 0)	{
//			dt[1] = _T("【取引先未入力】");
//		}
//// 158065,158066 add <--
// 158138 del <--
// 158138 add -->
		if(regautoref == TRUE)	{
			if(_tstoi(dt[0]) == 0)	{
				dt[1] = _T("【取引先未入力】");
			}
		}
		else	{
			if(dt[1].IsEmpty() == TRUE && dt[2].IsEmpty() == TRUE)	{
				dt[1] = _T("【取引先未入力】");
			}
		}
// 158138 add <--
		pRecord->AddItem(new ICSReportRecordItemText(dt[1]+dt[2]));
		// 金額
		dt[3].Trim();
		l_input(vl1,(char*)dt[3].GetBuffer());
		memset(bf,0,sizeof(bf));
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
					l_print(bf,vl1,FMT12);
					len=12;
					break;
				default:				// それ以外の様式
					// 明細13桁
					l_print(bf,vl1,FMT13);
					len=13;
					break;
			}
		}
		else	{
			len=12;
			l_print(bf,vl1,FMT12);
		}
		val.Format("%s",bf);
		val.TrimLeft();
		pRecord->AddItem(new ICSReportRecordItemText(val,ICS_EDS_NUMERIC,TRUE,1,len,ICS_EDS_RIGHT,TRUE));
		// 一括金額より小さい科目の背景色を変える
		if(m_FormSeq == ID_FORMNO_081 && m_UseAdv != 0)	{
// 158057 add -->
			order = _tstoi(dt[0]);
			dt[4] = m_IkkatuValHandler.GetIkkatuVal(order);
// 158057 add <--
			dt[4].Trim();
			if(dt[4].IsEmpty() == FALSE)	{
				l_input(vl2,(char*)dt[4].GetBuffer());
			}
		}
// 158125 del -->
		//if(l_cmp(vl1,vl2) < 0)	{
// 158125 del <--
// 158125 add -->
		if((l_cmp(vl1,vl2) < 0) && (m_Zero == 0 || l_test(vl1) != 0))	{
// 158125 add <--
			iktind.Add(index);
		}
		// 次のレコードへ
		rs.MoveNext();
		index++;
	}
	rs.Close();

	// ここ以降でないと、効かない
	m_report.AllowEdit(TRUE);						// 全て編集許可にしたあと
	m_report.AllowEditColumn(0, FALSE);				// Edit不可に
	m_report.AllowEditColumn(1, FALSE);				// Edit不可に

	// 行の高さ
	m_report.SetItemHeight(20);

	// 一括金額より小さい取引先があれば背景色を変える
	// (一括金額[自動]と同じ背景色)
	COLORREF	color = COLOR_IKKATUAUTO;
	max = (int)iktind.GetCount();
	if(max > 0)	{
		for(index=0; index<max; index++)	{
			m_report.SetCellBackColor(iktind.GetAt(index),0,color,TRUE);
			m_report.SetCellBackColor(iktind.GetAt(index),1,color,TRUE);
		}
		m_report.RedrawControl();
	}
}

// 158063 del -->
////**************************************************************
////	取引先の総額をマップに書き込む（ソート処理から呼ばれる）
////	【引数】	CDatabase*		データベース操作クラス
////				int				様式番号
////	【戻値】	なし
////**************************************************************
//void CdlgTorihikiAmount1::MakeMap( CDatabase* pDB, int pFormSeq )
//{
//	CRecordset			rs(pDB);
//	CString				dt[5];
//	AMOUNTREC1			rec;
//	short				scnt=0;
//	int					cnt=0;
//
//	m_AmountMap.RemoveAll();
//
//	rs.Open(CRecordset::forwardOnly,MakeSql(pDB,pFormSeq));
//	while(!rs.IsEOF())	{
//		for(scnt=0; scnt<5; scnt++)	{
//			rs.GetFieldValue(scnt,dt[scnt]);
//		}
//		// 取引先コード
//		dt[0].Trim();
//		rec.code = _tstoi(dt[0]);
//		// 取引先名称
//		dt[1].TrimRight();
//		dt[2].TrimRight();
//		rec.name = dt[1] + dt[2];
//		// 総額
//		dt[3].Trim();
//		rec.valu = dt[3];
//		// 書き込み
//		m_AmountMap.SetAt(cnt,rec);
//		cnt++;
//		// 次のレコードへ
//		rs.MoveNext();
//	}
//	rs.Close();
//}
//
////**************************************************************
////	取引先の総額をマップから読み込む（ソート処理から呼ばれる）
////	【引数】	int			取引先コード
////				CString		取引先名称
////	【戻値】	CString		総額
////**************************************************************
//CString CdlgTorihikiAmount1::GetAmount( int pCode, CString pName )
//{
//	int				ii=0,max=0;
//	CString			cs=_T("");
//	AMOUNTREC1		rec;
//
//	max = (int)m_AmountMap.GetCount();
//	for(ii=0; ii<max; ii++)	{
//		m_AmountMap.Lookup(ii,rec);
//		if(rec.code == pCode && rec.name == pName)	{
//			cs = rec.valu;
//			break;
//		}
//	}
//	return(cs);
//}
// 158063 del <--
