// dlgTorihikiAmount2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgTorihikiAmount2.h"

extern	BOOL	bG_Kanso;

// CdlgTorihikiAmount2 ダイアログ

IMPLEMENT_DYNAMIC(CdlgTorihikiAmount2, ICSDialog)

CdlgTorihikiAmount2::CdlgTorihikiAmount2(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_TORIHIKI_AMOUNT2, pParent)
{

}

CdlgTorihikiAmount2::~CdlgTorihikiAmount2()
{
}

void CdlgTorihikiAmount2::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1_REPORT, m_report);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_ICSDIAG_BC, m_DiagBC);
}

BEGIN_MESSAGE_MAP(CdlgTorihikiAmount2, ICSDialog)
	ON_BN_CLICKED(IDOK, &CdlgTorihikiAmount2::OnBnClickedOk)
END_MESSAGE_MAP()

// CdlgTorihikiAmount2 メッセージ ハンドラー

// 158125 del -->
//// 158065,158066 del -->
////// 158057 del -->
//////int CdlgTorihikiAmount2::ShowDialog( CDatabase* pDB, int pFormSeq )
////// 158057 del <--
////// 158057 add -->
////int CdlgTorihikiAmount2::ShowDialog( CDatabase* pDB, int pFormSeq, IkkatuValHandler& IkkatuVal )
////// 158057 add <--
//// 158065,158066 del <--
//// 158065,158066 add -->
//int CdlgTorihikiAmount2::ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, IkkatuValHandler& IkkatuVal )
//// 158065,158066 add <--
// 158125 del <--
// 158125 add -->
int CdlgTorihikiAmount2::ShowDialog( CDatabase* pDB, int pFormSeq, CString pIkkatuVal, IkkatuValHandler& IkkatuVal, int pZero )
// 158125 add <--
{
	m_pDB		= pDB;
	m_FormSeq	= pFormSeq;
// 158065,158066 add -->
	m_IkkatuVal	= pIkkatuVal;
// 158065,158066 add <--
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

void CdlgTorihikiAmount2::OnBnClickedOk()
{
	m_nRet = ID_DLG_OK;

	OnCancel();
}

BOOL CdlgTorihikiAmount2::OnInitDialog()
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
void CdlgTorihikiAmount2::InitReportHeader()
{
	m_report.RemoveAllRecords();
	m_report.RemoveColumn(-1);
	
	m_report.AddColumn(new ICSReportColumn(0, NULL, 5));
	m_report.AddColumn(new ICSReportColumn(1, NULL, 4));
	m_report.AddColumn(new ICSReportColumn(2, NULL, 3));

	m_report.SetSortable ( FALSE );			// ソート禁止
	m_report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	// 縦線(赤)
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255,0,0));
	arrayGridColor.Add(RGB(255,0,0));
	m_report.SetVerGrid(&arryGridColumn,&arrayGridColor,RGS_SOLID);

	ICSReportColumns	*m_pClms = m_report.GetColumns();
	CString				title=_T("");
	for(int i = 0; i < 3; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		if(i == 0)		title = _T("取引先");
		else if(i == 1)	title = _T("科目");
		else			title = _T("金額");
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
////	取引先－科目の総額を取得するSQL文を作成
////	【引数】	CDatabase*		データベース操作クラス
////				int				様式番号
////	【戻値】	CString			作成したSQL文
////**************************************************************
//CString CdlgTorihikiAmount2::MakeSql( CDatabase* pDB, int pFormSeq )
//{
//	CString				cs=_T("");
//	CString				tbl=_T("");
//	CUcFunctionCommon	ufc;
//	int					is50ont=0,is50onk=0,dummy=0;
//
//	if(pFormSeq == ID_FORMNO_021)	tbl = _T("uc_021_uketoritegata");
//	else							tbl = _T("uc_081_siharaitegata");
//
//	ufc.IsSort(pFormSeq,pDB,&dummy,&is50ont);
//	is50onk = ufc.IsKamokuSort(pFormSeq,pDB);
//
//	cs.Format(_T("select AdOrder,AdName1,AdName2,a.KnOrder,a.KnName,sum(Val) as amount,b.IkkatuVal from %s a"),tbl);
//	cs += _T(" left join uc_lst_kamoku b on a.KnSeq=b.KnSeq");
//	cs.Format(cs + _T(" where (FgShow=%d or FgShow=%d) and FgFunc=%d and a.KnOrder!=0"),ID_FGSHOW_OFF,ID_FGSHOW_IKKATU,ID_FGFUNC_DATA);
//	cs += _T(" group by AdOrder,AdName1,AdName2,AdKana,a.KnOrder,a.KnName,a.KnKana,b.IkkatuVal");
//	// 取引先
//	if(is50ont == 0)	{
//		// 番号順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdOrder ASC,");
//		cs += _T("(a.AdName1 + a.AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName2 COLLATE Japanese_CS_AS_KS_WS ASC");
//	}
//	else	{
//		// 50音順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdKana COLLATE Japanese_CS_AS_KS_WS ASC, a.AdOrder ASC");
//	}
//	// 科目
//	if(is50onk == 0)	{
//		// 番号順
//		cs += _T(",a.KnOrder ASC");
//	}
//	else	{
//		// 50音順
//		cs += _T(",a.KnKana COLLATE Japanese_CS_AS_KS_WS ASC, a.KnOrder ASC");
//	}
//	return(cs);
//}
// 158063 del <--

//**************************************************
//	レポートコントロールにレコードを追加
//	【引数】	なし
//	【戻値】	TRUE:成功
//				FALSE:失敗
//**************************************************
void CdlgTorihikiAmount2::AddRecords()
{
	CRecordset			rs(m_pDB);
	CUcFunctionCommon	ufc;
	CString				cs=_T("");
	CString				dt[7];
	CString				val=_T("");
	CString				save=_T("");
	CString				tbl=_T("");
	int					is50ont=0,is50onk=0,dummy=0;
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

	rs.Open(CRecordset::forwardOnly,ufc.MakeAmountSql2(m_pDB,m_FormSeq));
	while(!rs.IsEOF())	{
		for(scnt=0; scnt<7; scnt++)	{
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
		cs = dt[1]+dt[2];
		if(cs == save)	cs=_T("");
		pRecord->AddItem(new ICSReportRecordItemText(cs));
		save = dt[1]+dt[2];
		// 科目名称
// 158065,158066 del -->
		//pRecord->AddItem(new ICSReportRecordItemText(dt[4]));
// 158065,158066 del <--
// 158065,158066 add -->
		if(_tstoi(dt[3]) == 0)	cs = _T("【科目未入力】");
		else					cs = dt[4];
		pRecord->AddItem(new ICSReportRecordItemText(cs));
// 158065,158066 add <--
		// 金額
		dt[5].Trim();
		l_input(vl1,(char*)dt[5].GetBuffer());
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
		val.Format("%s", bf);
		val.TrimLeft();
		pRecord->AddItem(new ICSReportRecordItemText(val,ICS_EDS_NUMERIC,TRUE,1,len,ICS_EDS_RIGHT,TRUE));
		// 一括金額より小さい科目の背景色を変える
// 158057 add -->
		order = _tstoi(dt[3]);
// 158065,158066 del -->
		//dt[6] = m_IkkatuValHandler.GetIkkatuVal(order);
// 158065,158066 del <--
// 158065,158066 add -->
		if(order > 0)	dt[6] = m_IkkatuValHandler.GetIkkatuVal(order);
		else			dt[6] = m_IkkatuVal;
// 158065,158066 add <--
// 158057 add <--
		dt[6].Trim();
		l_input(vl2,(char*)dt[6].GetBuffer());
// 158125 del -->
		//if(l_cmp(vl1,vl2) < 0)	{
// 158125 del <--
// 158125 add -->
		if((l_cmp(vl1,vl2) < 0) && (m_Zero == 0 || l_test(vl1) != 0))	{
// 158125 add <--
			//m_report.SetCellBackColor(index,1,RGB(255,0,0),TRUE);
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

	// 一括金額より小さい取引先－科目があれば背景色を変える
	// (一括金額[自動]と同じ背景色)
	COLORREF	color = COLOR_IKKATUAUTO;
	max = (int)iktind.GetCount();
	if(max > 0)	{
		for(index=0; index<max; index++)	{
			m_report.SetCellBackColor(iktind.GetAt(index),1,color,TRUE);
			m_report.SetCellBackColor(iktind.GetAt(index),2,color,TRUE);
		}
		m_report.RedrawControl();
	}

}

// 158063 del -->
////**************************************************************
////	取引先－科目の総額をマップに書き込む（ソート処理から呼ばれる）
////	【引数】	CDatabase*		データベース操作クラス
////				int				様式番号
////	【戻値】	なし
////**************************************************************
//void CdlgTorihikiAmount2::MakeMap( CDatabase* pDB, int pFormSeq )
//{
//	CRecordset			rs(pDB);
//	CString				dt[7];
//	AMOUNTREC2			rec;
//	short				scnt=0;
//	int					cnt=0;
//
//	m_AmountMap.RemoveAll();
//
//	rs.Open(CRecordset::forwardOnly,MakeSql(pDB,pFormSeq));
//	while(!rs.IsEOF())	{
//		for(scnt=0; scnt<7; scnt++)	{
//			rs.GetFieldValue(scnt,dt[scnt]);
//		}
//		// 取引先コード
//		dt[0].Trim();
//		rec.tcode = _tstoi(dt[0]);
//		// 科目コード
//		dt[3].Trim();
//		rec.kcode = _tstoi(dt[3]);
//		// 取引先名称
//		dt[1].TrimRight();
//		dt[2].TrimRight();
//		rec.tname = dt[1] + dt[2];
//		// 総額
//		dt[5].Trim();
//		rec.valu = dt[5];
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
////	取引先－科目の総額をマップから読み込む（ソート処理から呼ばれる）
////	【引数】	int			取引先コード
////				CString		取引先名称
////				int			科目コード
////	【戻値】	CString		総額
////**************************************************************
//CString CdlgTorihikiAmount2::GetAmount( int pTcode, CString pTname, int pKcode )
//{
//	int				ii=0,max=0;
//	CString			cs=_T("");
//	AMOUNTREC2		rec;
//
//	max = (int)m_AmountMap.GetCount();
//	for(ii=0; ii<max; ii++)	{
//		m_AmountMap.Lookup(ii,rec);
//		if(rec.tcode == pTcode && rec.tname == pTname && rec.kcode == pKcode)	{
//			cs = rec.valu;
//			break;
//		}
//	}
//	return(cs);
//}
// 158063 del <--
