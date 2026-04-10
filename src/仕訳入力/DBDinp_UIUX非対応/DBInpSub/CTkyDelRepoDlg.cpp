// CTkyDelRepoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "CTkyDelRepoDlg.h"
#include "DBInpSubFunc.h"


// CTkyDelRepoDlg ダイアログ

IMPLEMENT_DYNAMIC(CTkyDelRepoDlg, ICSDialog)

CTkyDelRepoDlg::CTkyDelRepoDlg(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DLG_DELTKY, pParent)
{
	m_bBmn = m_bSyusi = FALSE;
}

CTkyDelRepoDlg::~CTkyDelRepoDlg()
{
}

void CTkyDelRepoDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_Report);
}


BEGIN_MESSAGE_MAP(CTkyDelRepoDlg, ICSDialog)
END_MESSAGE_MAP()


// CTkyDelRepoDlg メッセージ ハンドラー


BOOL CTkyDelRepoDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください

	ICSDialog::OnInitDialogEX();

	GetDlgItem(IDC_STATIC_TXT)->SetWindowText(m_str);
	((CStatic*)GetDlgItem(IDC_ICON_STATIC))->SetIcon(LoadIcon(NULL, MAKEINTRESOURCE(IDI_EXCLAMATION)));

	KamokuReportDisp();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CTkyDelRepoDlg::GetBtnLogFont(LOGFONT* plf)
{
	CFont* pFont;
	pFont = GetDlgItem(IDOK)->GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	memcpy(plf, &lf, sizeof LOGFONT);
}


void CTkyDelRepoDlg::InitTkyDelReport()
{
	// リポートコントロール設定
	//リサイズ設定はカラム設定より前に行う必要がある
	m_Report.RemoveAllRecords();
	m_Report.RemoveColumn(-1);
	m_Report.Resize(TRUE, ICSResizeDefault);

	CString str1, str2;

	int cnt = m_pKmAry->GetCount();

	//部門・収支があるか？
	for( int n = 0; n < cnt; n++ ) {
		if( (*m_pKmAry)[n].ezbmn != -1 ) {
			m_bBmn = TRUE;
		}
		if( (*m_pKmAry)[n].type != 0 ) {
			m_bSyusi = TRUE;
		}
	}

	int clm = 0;

	CRect	repRect;
	m_Report.GetWindowRect(&repRect);
	int width = repRect.Width();

	const int bmnCnt = 8;
	const int kcodeCnt = 10;
	const int knamCnt = 20;
	const int edaCnt = 6;

	int divCnt = kcodeCnt + knamCnt + edaCnt;
	int clWidth;

	//カラムの設定
	if( m_bBmn == TRUE ) {
		divCnt += bmnCnt;
		clWidth = (width / divCnt) * bmnCnt;
		m_Report.AddColumn(new ICSReportColumn(clm++, _T("部門"), clWidth, TRUE, ICS_REPORT_NOICON, FALSE));
	}
	clWidth = (width / divCnt) * kcodeCnt;
	m_Report.AddColumn(new ICSReportColumn(clm++, _T("科目コード"), clWidth, TRUE, ICS_REPORT_NOICON, FALSE));

	clWidth = (width / divCnt) * knamCnt;
	m_Report.AddColumn(new ICSReportColumn(clm++, _T("科目名称"), clWidth, TRUE, ICS_REPORT_NOICON, FALSE));

	clWidth = (width / divCnt) * edaCnt;
	m_Report.AddColumn(new ICSReportColumn(clm++, _T("枝番"), clWidth, TRUE, ICS_REPORT_NOICON, FALSE));

	m_Report.SetGridStyle(REPORT_GRID_VERTICAL, RGS_SMALLDOT);
	m_Report.SetGridColor(REPORT_GRID_VERTICAL, RGB(255,0,0));

	m_Report.SetGridColor(REPORT_GRID_HORIZONTAL, CL_GRAY);
//	m_Report.SetGridPerCount(REPORT_GRID_HORIZONTAL, 5, CL_BLUE, RGS_SOLID);

	COLORREF colorRow;
	colorRow = GetSysColor(COLOR_HIGHLIGHT);
	m_Report.ColorInactive(colorRow);

	LOGFONT lf;
	GetBtnLogFont(&lf);

	m_Report.SetTextFont(lf);
	m_Report.SetHeaderFont(lf);
	int ht = abs(lf.lfHeight);
	int itemH = ht;

	ht += (ht / 2);
	m_Report.SetHeaderHeight(ht);

	m_Report.SetItemHeight(itemH + (itemH / 2));

	m_Report.AllowEdit(FALSE);

	m_Report.SetSortable(FALSE);
	m_Report.SetDraggable(FALSE);
	m_Report.SetRemovable(FALSE);

}


// 科目リストの表示
void CTkyDelRepoDlg::KamokuReportDisp()
{
	// Report イニシャライズ
	InitTkyDelReport();

	int ClmCount = m_Report.GetColumns()->GetCount();
	CString filter;
	// 現在コードを保存

	CString str, strNum;
	ICSReportRecord*			pRecord;
	ICSReportRecordItem*		pItem;
	ICSReportRecordItemText*	pTextItm;
	ICS_REPORT_EDITPARA		ep1;

	ep1.kind = ICS_EDS_NUMERIC | ICS_EDS_ALPHABET | ICS_EDS_KANA | ICS_EDS_ZENKAKU | ICS_EDS_ASCII;;
	ep1.inverse = TRUE;			//フォーカス時インバース
	ep1.cmove = TRUE;			//カーソル移動許可/不許可

	ep1.lenMax = 20;			//入力最大バイト数

	//CL_WHITE
	//RGB_YELLOW1;

	COLORREF	back;
	back = CL_WHITE;

	//科目枝番→部門枝番→収支枝番→収支部門枝番 に行が変われば、背景色を変える。
	int prev_bmn = -1, prev_type = -1;

	for( int i = 0; i < (int)m_pKmAry->GetCount(); i++ ) {
#ifdef CLOSE //背景色変更は品質依頼よりやめる
		if( i == 0 ) {
			prev_bmn = (*m_pKmAry)[i].ezbmn;
			prev_type = (*m_pKmAry)[i].type;
		}
		else {
			int chg_col = 0;
			if( m_bBmn ) {
				if( prev_bmn != -1 ) {
					if( (*m_pKmAry)[i].ezbmn == -1 ) {
						chg_col++;
						prev_bmn = -1;
					}
				}
				else {
					if( (*m_pKmAry)[i].ezbmn != -1 ) {
						prev_bmn = (*m_pKmAry)[i].ezbmn;
						chg_col++;
					}
				}
			}
			if( !chg_col ) {
				if( prev_type != (*m_pKmAry)[i].type ) {
					prev_type = (*m_pKmAry)[i].type;
					chg_col++;
				}
			}
			if( chg_col ) {
				if( back == CL_WHITE )	back = RGB_YELLOW1;
				else {
					back = CL_WHITE;
				}
			}
		}
#endif

		pRecord = m_Report.AddRecord(new ICSReportRecord());

		ep1.lenMax = m_pZm->zvol->bmcol;	//入力最大バイト数
		ep1.vector = ICS_EDS_CENTER;	//入力方向
		ep1.conma = FALSE;			//入力時カンマ編集
		ep1.format = "";			//書式フォーマット(倍長のみ有効)
		ep1.hitem = 0;				//コンボボックスリストアイテムの高さ

		char tmp[64];
		::ZeroMemory(tmp, sizeof tmp);

		if( m_bBmn ) {
			if( (*m_pKmAry)[i].ezbmn == -1 ) {
				for( int i = 0; i < m_pZm->zvol->bmcol; i++ ) {
					tmp[i] = '-';
				}
			}
			else {
				set_codestr(tmp, sizeof tmp, (*m_pKmAry)[i].ezbmn, m_pZm->zvol->bmcol);
			}
			pTextItm = (ICSReportRecordItemText*)pRecord->AddItem(new ICSReportRecordItemText(tmp, &ep1));
			pTextItm->SetCaption(tmp);
			pTextItm->SetBackgroundColor(back);
			pTextItm->SetTextColor(CL_BLACK);
		}

		ep1.lenMax = 20;

		/* 科目コード */
		str.Empty();
		if( (*m_pKmAry)[i].knicod != -1 ) {
			str.Format("%04d", (*m_pKmAry)[i].knicod);
		}
		pItem = pRecord->AddItem(new ICSReportRecordItemText(str, &ep1));
		((ICSReportRecordItemText*)pItem)->EnableDelete(0);
		((ICSReportRecordItemText*)pItem)->SetDisplayString(str);
		pItem->SetCaption(str);
		pItem->SetBackgroundColor(back);
		pItem->SetTextColor(CL_BLACK);
		/* 科目 */
		if( m_bSyusi ) {
			if( (*m_pKmAry)[i].type ) {
				str.Format("[収]%s", (*m_pKmAry)[i].knnam);
			}
			else {
				str.Format("%s", (*m_pKmAry)[i].knnam);
			}
		}
		else {
			str = (*m_pKmAry)[i].knnam;
		}
		pItem = pRecord->AddItem(new ICSReportRecordItemText(str, &ep1));
		((ICSReportRecordItemText*)pItem)->SetDisplayString(str);
		pItem->SetCaption(str);
		((ICSReportRecordItemText*)pItem)->EnableDelete(0);
		pItem->SetBackgroundColor(back);
		pItem->SetTextColor(CL_BLACK);
		/* 枝番コード */
		set_codestr(tmp, sizeof tmp, (*m_pKmAry)[i].ezecd, m_pZm->zvol->edcol);
		pItem = pRecord->AddItem(new ICSReportRecordItemText(tmp, &ep1));
		pItem->SetCaption(tmp);
		((ICSReportRecordItemText*)pItem)->EnableDelete(0);
		pItem->SetBackgroundColor(back);
		pItem->SetTextColor(CL_BLACK);

	}

	m_Report.Populate();
}

