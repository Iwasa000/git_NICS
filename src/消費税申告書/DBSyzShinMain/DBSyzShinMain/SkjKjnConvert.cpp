// SkjKjnConvert.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

//#include "MainFrm.h"
//#include "DBSyzShinDoc.h"
//#include "DBSyzShinView.h"

#include "SkjKjnConvert.h"
#include "Gyousyu.h"

//#include "ShinSkjInit.h"

//static	SKJCNV	SkjCnv;
#define		S_CLASS_RGB		RGB( 255, 255, 220 )

static	char SKJ_NAME[][64] = {
	"01 仕入金額 ① 事業　所得",
	"02 必要経費 ② 事業　所得",
	"03 固定資産 ③ 事業　所得",
	"05 必要経費 ⑤ 不動産所得",
	"06 固定資産 ⑥ 不動産所得",
	"08 仕入金額 ⑧ その他所得",
	"09 必要経費 ⑨ その他所得",
	"10 固定資産 ⑩ その他所得",
	"00 集　計　外　科　目　×",
	"　",
	0
};

static	char	m_HKJSAVE[10240];			// 1024 → 10240 [09'07.13]
//////////////////

// メインビューの財務クラスを参照
//CDBSyzShinView *CSkjKjnConvert::pSyzShin;


// CSkjKjnConvert ダイアログ

IMPLEMENT_DYNAMIC(CSkjKjnConvert, ICSDialog)

CSkjKjnConvert::CSkjKjnConvert(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSkjKjnConvert::IDD, pParent)
	, m_pZmSub(NULL)
	, m_pSnHeadData(NULL)
	, m_pTblhdl(NULL)
	, m_pSkjCnvtbl(NULL)
{
}

CSkjKjnConvert::~CSkjKjnConvert()
{
}

void CSkjKjnConvert::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_Report);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CSkjKjnConvert, ICSDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_KANA, &CSkjKjnConvert::OnBnClickedButtonKana)
	ON_LBN_DBLCLK(IDC_LIST1, &CSkjKjnConvert::OnLbnDblclkList1)
	ON_NOTIFY(ICS_NM_REPORT_LBUTTONDOWN, IDC_CUSTOM1, OnNMReportLbuttondownCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_VALUECHANGED, IDC_CUSTOM1, OnNMReportValuechangedCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_CUSTOM1, OnNMReportEditkeydownCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_EDITSETFOCUS, IDC_CUSTOM1, OnNMReportEditsetfocusCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_EDITKILLFOCUS, IDC_CUSTOM1, OnNMReportEditkillfocusCUSTOM1)
	ON_BN_CLICKED(IDCANCEL, &CSkjKjnConvert::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSkjKjnConvert::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, &CSkjKjnConvert::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CSkjKjnConvert::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CSkjKjnConvert::OnBnClickedButton4)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pZmSub		：	公益クラス
//		pSnHeadData	：	消費税ヘッダ情報
//		pTblHdl		：	テーブルハンドリング
//		pSkjCnvtbl	：	仕入控除用テーブル
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSkjKjnConvert::InitInfo( CDBNpSub *pZmSub, CSnHeadData *pSnHeadData, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl )
{
	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pTblhdl );
	if( pTblhdl == NULL ){
		return -1;
	}
	m_pTblhdl = pTblhdl;

	ASSERT( pSkjCnvtbl );
	if( pSkjCnvtbl == NULL ){
		return -1;
	}
	m_pSkjCnvtbl = pSkjCnvtbl;

	return 0;
}


// CSkjKjnConvert メッセージ ハンドラ

BOOL CSkjKjnConvert::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 集計先項目の表示
	m_ListBox.ResetContent();
	for( int i = 0; SKJ_NAME[i][0]; i++ ) {
		m_ListBox.AddString( SKJ_NAME[i] );
		CString	Str;
		Str.Format( "%s", SKJ_NAME[i] );
		m_ListBox.SetItemData( i, atoi( Str.Left( 2 )) );
	}

	CRect rect;
	m_Report.GetClientRect( &rect );
	m_Report.RemoveColumn( -1 );

//★//[12'03.12]///
	// キャプション変更
	CString	SkjKjnCaption;
	if( m_pSnHeadData->IsUseTaxRefund() )	SkjKjnCaption = _T("還付申告明細用連動情報登録");
	else						SkjKjnCaption = _T("仕入控除税額明細用連動情報登録");
	SetWindowText( SkjKjnCaption );
///////////////////

	int m_width;
	CSize	szScreen;
	if( GetScreenSize()->cx > 1024 )	m_width = rect.Width()-42;
	else								m_width = rect.Width()-130;
	m_width = m_width / 3;
	m_Report.AddColumn( new ICSReportColumn(0, _T( "科　目　名　称"), m_width + (m_width/2),TRUE, ICS_REPORT_NOICON , FALSE) );
	m_Report.AddColumn( new ICSReportColumn(1, _T( " "), m_width / 2,TRUE, ICS_REPORT_NOICON , FALSE) );
	m_Report.AddColumn( new ICSReportColumn(2, _T( "集 計 先 項 目"), m_width + m_width,TRUE, ICS_REPORT_NOICON , FALSE, TRUE,DT_RIGHT) );
	m_Report.SetGridStyle( TRUE, (ICSReportGridStyle)RGS_SMALLDOT );	//選択オフセット=	RGS_NOLINES or RGS_SMALLDOT or RGS_LARGEDOT or RGS_DASH or RGS_SOLID
	m_Report.SetGridColor(TRUE, RGB( 0xff, 0, 0 ));
	m_Report.SetDraggable( FALSE );
	m_Report.SetRemovable( FALSE );
	m_Report.SetSortable( FALSE );

	CFont *pFont = m_ListBox.GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_Report.SetTextFont( lf );

	InizDataList = TRUE;
//2016.02.23 INSERT START
	ICSDialog::OnInitDialogEX();
//2016.02.23 INSERT END
	SetListColm();

//2016.02.23 DELETE START
//	ICSDialog::OnInitDialogEX();
//2016.02.23 DELETE END
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CSkjKjnConvert::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
	if( m_ListBox ) {
		CFont *pFont = m_ListBox.GetFont();
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		m_Report.SetTextFont( lf );
	}
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CSkjKjnConvert::OnBnClickedButtonKana()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CWnd *pWnd = GetDlgItem( IDC_BUTTON_KANA );
	RECT rc;
	if( pWnd == NULL ) return;
	pWnd->GetWindowRect( &rc );

	SH_SKJ_CNV	skjcnv;

	KSEL_PAR	Kpar;

	ZeroMemory( &Kpar, sizeof( KSEL_PAR ));
	Kpar.ksPsgn		= KSP_RIGHTTOP;		// ポジション・サイン
	Kpar.ksFsgn		= KSF_ABSDISP;		// ファンクション・サイン
	Kpar.ksLnum		= 10;				// 表示科目数
	Kpar.ksKana;						// 呼び出し科目の頭文字（ASCII）
	Kpar.ksKrng;						// 対象科目（０：全科目／１：貸借／２：損益／他：（下位：開始／上位：終了））
	Kpar.ksPos.x	= rc.right;
	Kpar.ksPos.y	= rc.bottom;		// ダイアログ表示ポジション（スクリーン座標）
	Kpar.ksCwnd;						// ポジションチェック対象ウィンドウ
	Kpar.ksStat;						// ステータス・フラグ
	Kpar.ksFhwd;						// フォントサイズ設定対象ウィンドウハンドル（'98.9.28）
	CString cmd;
	cmd  = _T( "(kncod >= '02000000' and kncod <= '03000000') or " );
	cmd += _T( "(kncod >= '09000000' and kncod <= '0A000000') or " );
	cmd += _T( "(kncod >= '0C020000' and kncod <  '0C030000') or " );
	cmd += _T( "(kncod >= '0B020000' and kncod <  '0B030000') or " );
	cmd += _T( "(kncod >= '0F000000' and kncod <= '0FFFFFFF') " );
//2016.02.23 INSERT START
	cmd += _T( "or (((cast(cast(knsgn as binary(1)) as int) & 0xff) = 0x01) and ((cast(cast(knsgn as binary(2)) as int) & 0xff) = 0x00)) " );
//2016.02.23 INSERT END

	m_pZmSub->knrec->Requery( cmd, 1 );
	ICSMainFrame *pICSFrm = ((ICSMainFrame *)AfxGetMainWnd());
	pICSFrm->PopupCloseState( 2 );
	if( Kmk50onSelect( m_pZmSub, &Kpar, this ) != FALSE ) {
		GotoDlgCtrl( &m_Report );
		if( Kpar.ksStat == 0 ) {
			char	codbf[10];
			memset( codbf, '\0', sizeof( codbf ) );
			memmove( codbf, m_pZmSub->knrec->kncod, 8 );
			_strupr_s( codbf, sizeof( codbf ) );
			int	rno = 0;
			int	flg = 0;
			while( 1 ) {
				if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, rno ) ) break;
				if( !strncmp( codbf, (char *)skjcnv.SKJCod, 8 ) )	{
					m_Report.EditItem( rno, 1, TRUE );
					flg = 1;
					break;
				}
				rno++;
			}
			if( !flg )		GotoDlgCtrl( &m_Report );	
		}
	}
	else	GotoDlgCtrl( &m_Report );
	m_pZmSub->knrec->Requery( "", 1 );
	pICSFrm->PopupCloseState( 0 );
}

void CSkjKjnConvert::OnLbnDblclkList1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( Inp_l == 0 && Inp_c == 0 ) return;
	int cur = m_ListBox.GetCurSel();

	int r = ERR;
	r = (int)m_ListBox.GetItemData( cur );

	SH_SKJ_CNV	skjcnv;

	if( r != ERR ) {
		CString	Str;
		// 現在ポジションのゲット
		if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;
		skjcnv.KJNCod = r;
		if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;
		// 集計項目番号
		switch( skjcnv.KJNCod ) {
			case 1 :	Str.Format( "%2.2s", SKJ_NAME[0] );	r = 0;	break;
			case 2 :	Str.Format( "%2.2s", SKJ_NAME[1] );	r = 1;	break;
			case 3 :	Str.Format( "%2.2s", SKJ_NAME[2] );	r = 2;	break;
			case 5 :	Str.Format( "%2.2s", SKJ_NAME[3] );	r = 3;	break;
			case 6 :	Str.Format( "%2.2s", SKJ_NAME[4] );	r = 4;	break;
			case 8 :	Str.Format( "%2.2s", SKJ_NAME[5] );	r = 5;	break;
			case 9 :	Str.Format( "%2.2s", SKJ_NAME[6] );	r = 6;	break;
			case 10:	Str.Format( "%2.2s", SKJ_NAME[7] );	r = 7;	break;
			default:	Str.Format( "%2.2s", SKJ_NAME[8] );	r = 8;	break;
		}
		m_Report.SetCaption( Inp_l, 1, Str );

		Str.Format( "%s", &SKJ_NAME[r][3] );
		m_Report.SetCaption( Inp_l, 2, Str );
	}

	//データ設定の反映　再描画
	m_Report.Populate();
	m_Report.EditItem( Inp_l, 1, TRUE );

}

void CSkjKjnConvert::OnNMReportLbuttondownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//クリックされたカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//クリックされた行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//クリックされたアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();
	//クリックされた位置
	POINT pt	=	pItemNotify->pt;

	CString str;
	Inp_l = indexRow;
	Inp_c = indexColumn;

	if( Inp_c <= 0 ) {
		Inp_l = Inp_c = 0;
		return;
	}

	if( Inp_c != 1 ) return;
	
	SH_SKJ_CNV	skjcnv;
	if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;
	int r = GetKjnCodName( skjcnv.KJNCod );
	m_ListBox.SetCurSel( r );
//	
	if( r != -1 ) {
		str.Format( "%2.2s", SKJ_NAME[r] );
	}
	else str =  _T( "――" );
	m_Report.SetCaption( Inp_l, Inp_c, str );

	if( r != -1 ) {
		str.Format( "%s", &SKJ_NAME[r][3] );
	}
	else str =  _T( "―――――――――" );
	m_Report.SetCaption( Inp_l, Inp_c+1, str );
//
}

void CSkjKjnConvert::OnNMReportValuechangedCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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
	//変更のあった位置
	POINT pt	=	pItemNotify->pt;
}

void CSkjKjnConvert::OnNMReportEditkeydownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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

	m_Report.EditItem( indexRow, indexColumn, FALSE );
	SetSnameString( indexRow, indexColumn );

	int lc = m_Report.GetRows()->GetCount();
	switch( nChar ) {

		case	VK_RIGHT:
		case	VK_RETURN:
						if( (indexRow+1) < m_Report.GetRows()->GetCount() ) {
							m_Report.EditItem( indexRow+1, indexColumn, TRUE );

						}
						break;
		case	VK_LEFT:
						break;
		case	VK_UP:
						if( indexRow > 0 ) {
							m_Report.EditItem( indexRow-1, indexColumn, TRUE );
						}
						break;

		case	VK_NEXT:
						break;

		case	VK_PRIOR:
						break;

		case	VK_DELETE:
						break;
	
							
	}


}

void CSkjKjnConvert::OnNMReportEditsetfocusCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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

	CString str;
	Inp_l = indexRow;
	Inp_c = indexColumn;

	if( Inp_c <= 0 ) {
		Inp_l = Inp_c = 0;
		return;
	}

	if( Inp_c != 1 ) return;

	SH_SKJ_CNV	skjcnv;
	if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;

	int r = GetKjnCodName( skjcnv.KJNCod );
	m_ListBox.SetCurSel( r );
//
	if( r != -1 ) {
		str.Format( "%2.2s", SKJ_NAME[r] );
	}
	else str =  _T( "――" );
	m_Report.SetCaption( Inp_l, Inp_c, str );

	if( r != -1 ) {
		str.Format( "%s", &SKJ_NAME[r][3] );
	}
	else str =  _T( "―――――――――" );
	m_Report.SetCaption( Inp_l, Inp_c+1, str );
//
}

void CSkjKjnConvert::OnNMReportEditkillfocusCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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
}


//////////////////
// 関数定義領域 //
//////////////////

// 変換情報の表示
void CSkjKjnConvert::SetListColm()
{

	memset( m_HKJSAVE, '\0', sizeof( m_HKJSAVE ) );

	//仮想モードOFF
	m_Report.SetVirtualMode( NULL, 0 );

	//データ削除
	ICSReportRecords	*pRecords	=	m_Report.GetRecords();
	pRecords->RemoveAll();
	//データセット
	CString	str;
	ICSReportRecordItem*	pItem;

	int rc = m_pSkjCnvtbl->tp_rnum;
	
	int enableColor = RGB( 0xa0, 0xa0, 0xa0 );
	CString		strName;
	char		kkbf[128];
	int wno = 0;
	int	rno = 0;
	SH_SKJ_CNV	skjcnv;
	while( 1 ) {
		if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, rno ) ) break;
		// 保存
		m_HKJSAVE[rno] = skjcnv.KJNCod;

		// 科目名称の取得
		memset( kkbf, '\0', sizeof( kkbf ) );
		if( m_Util.GetKnrecName( m_pZmSub, (char *)skjcnv.SKJCod, kkbf ) ){
			ICSReportRecord*	pRecord	=	m_Report.AddRecord( new ICSReportRecord() );
			// 科目名称
			strName.Format( "  %s", kkbf );
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName ) );
			m_Report.SetCaption( rno, 0, strName );
			// 集計項目番号
			switch( skjcnv.KJNCod ) {
				case 1 :	strName.Format( "%2.2s", SKJ_NAME[0] );	wno = 0;	break;
				case 2 :	strName.Format( "%2.2s", SKJ_NAME[1] );	wno = 1;	break;
				case 3 :	strName.Format( "%2.2s", SKJ_NAME[2] );	wno = 2;	break;
				case 5 :	strName.Format( "%2.2s", SKJ_NAME[3] );	wno = 3;	break;
				case 6 :	strName.Format( "%2.2s", SKJ_NAME[4] );	wno = 4;	break;
				case 8 :	strName.Format( "%2.2s", SKJ_NAME[5] );	wno = 5;	break;
				case 9 :	strName.Format( "%2.2s", SKJ_NAME[6] );	wno = 6;	break;
				case 10:	strName.Format( "%2.2s", SKJ_NAME[7] );	wno = 7;	break;
				default:	strName.Format( "%2.2s", SKJ_NAME[8] );	wno = 8;	break;
			}
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName, ICS_EDS_NUMERIC, TRUE, FALSE, 4, ICS_EDS_RIGHT ));
			m_Report.SetCaption( rno, 1, strName );
			((ICSReportRecordItemText *)pItem)->EnableDelete(0);
			// 集計科目名称
			strName.Format( "%s", &SKJ_NAME[wno][3] );
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName, ICS_EDS_ZENKAKU, TRUE, FALSE, 4 ));
			m_Report.SetCaption( rno, 2, strName );
			((ICSReportRecordItemText *)pItem)->EnableDelete(0);
		}
		rno++;
	}
	m_Report.AllowEdit( TRUE );
	m_Report.AllowEditColumn( 0, FALSE );
	m_Report.AllowEditColumn( 1, TRUE  );
	m_Report.AllowEditColumn( 2, FALSE  );

	//データ設定の反映　再描画
	m_Report.Populate();
	//一行目へフォーカス
	m_Report.SetFocus();
	m_Report.SetFocusedRow( m_Report.GetRows()->GetAt( 0 ) );

	for( int i=0; i < rc; i++ ) {
		m_Report.SetCellBackColor( i, 0, S_CLASS_RGB );
	}

}

//　集計項目番号の取得
int CSkjKjnConvert::GetKjnCodName( int kjncod )
{
	CString findstr;
	findstr.Format( "%02d", kjncod );
	int r = m_ListBox.FindString( -1, findstr );

	return r;

}

// ポジションセット
void CSkjKjnConvert::SetSnameString( int indexRow, int indexColumn )
{

	Inp_l = indexRow;
	Inp_c = indexColumn;
			
	SH_SKJ_CNV	skjcnv;
	if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;


	CString str;
	str.Format("%s", m_Report.GetCaption( Inp_l, Inp_c ));

	int d = atoi( str );
	if( d > 9999 ) d = d / 10000;

	int r = -1;
	CString strName;

	int cur = ERR;
	if( d >= 0 ) {
		cur = GetKjnCodName( d );
		m_ListBox.SetCurSel( cur );

		r = (int)m_ListBox.GetItemData( cur );

		if( r != -1 ) {
			if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;
			skjcnv.KJNCod = r;
			if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;
		}
	}

	r = GetKjnCodName( skjcnv.KJNCod );
	m_ListBox.SetCurSel( r );
	m_ListBox.GetItemData( r );

	strName.Format( "%2.2s", SKJ_NAME[r] );
	m_Report.SetCaption( Inp_l, Inp_c, strName );

	strName.Format( "%s", &SKJ_NAME[r][3] );
	m_Report.SetCaption( Inp_l, Inp_c+1, strName );
}

BOOL CSkjKjnConvert::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CWnd *pWnd = GetFocus();
	if( pMsg->message == WM_KEYDOWN ) {

//★★★//[11'02.15]
		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 )	return TRUE;
///////////////////	

		if( pMsg->wParam == VK_RETURN ) {
			if( pWnd == &m_Report ) {
				if( m_Report.GetRows()->GetCount() > 0 ) {
					ICSReportColumns *pClmns = m_Report.GetColumns();
					ICSReportColumn *pClmn = pClmns->GetAt( 1 );

					if( pClmn->IsEditable() != FALSE ) {
						int indexRow = m_Report.GetFocusedRow()->GetIndex();
						m_Report.EditItem( indexRow, 1, TRUE );
					}
				}
				return TRUE;
			}
		}
/////////[07'06.29]
		if( pMsg->wParam == VK_F3 ) {
			ICSDialog::EndDialog( -1 );
			return TRUE;
		}
		if( pMsg->wParam == VK_F4 ) {
			ICSDialog::EndDialog( -2 );
			return TRUE;
		}
		if( pMsg->wParam == VK_F5 ) {
			ICSDialog::EndDialog( -3 );
			return TRUE;
		}
///////////////////
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

void CSkjKjnConvert::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int r = 0;
	SH_SKJ_CNV	skjcnv;
	while( 1 ) {
		if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, r ) )	break;
		skjcnv.KJNCod = m_HKJSAVE[r];
		if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, r ) )	continue;
		r++;
	}
	ICSDialog::OnCancel();
}

void CSkjKjnConvert::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	ICSDialog::OnOK();
}


void CSkjKjnConvert::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	ICSDialog::EndDialog( -2 );
}

void CSkjKjnConvert::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ICSDialog::EndDialog( -3 );
}

void CSkjKjnConvert::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
/////////[08'10.24]
//	CString	Mesg;
//	Mesg.Empty();
//	Mesg = _T("仕入控除税額明細用連動情報を初期化しますか？	");
//	if( ICSMessageBox( Mesg, MB_YESNO | MB_DEFBUTTON2, 0 ) != IDYES )	{
//		m_Report.SetFocus();
//		return;
//	}
///////////////////
	int			Gflg = 0;
	CGyousyu	GyousyuDlg;
	GyousyuDlg.m_Gyousyu = 1;
//	GyousyuDlg.m_Refund = pSyzShin->m_TaxRefund;
	if( GyousyuDlg.DoModal() != IDOK )	{
		m_Report.SetFocus();
		return;
	}
	Gflg = GyousyuDlg.m_Gyousyu;
///////////////////

	memset( m_HKJSAVE, '\0', sizeof( m_HKJSAVE ) );

	//仮想モードOFF
	m_Report.SetVirtualMode( NULL, 0 );

	//データ削除
	ICSReportRecords	*pRecords	=	m_Report.GetRecords();
	pRecords->RemoveAll();
	//データセット
	CString	str;
	ICSReportRecordItem*	pItem;

	int rc = m_pSkjCnvtbl->tp_rnum;
	
	int enableColor = RGB( 0xa0, 0xa0, 0xa0 );

	CString		strName;
	char		kkbf[128], codbf[128];
	int wno = 0;
	int	rno = 0;
	SH_SKJ_CNV	skjcnv;
	while( 1 ) {
		if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, rno ) ) break;
		// 保存
		m_HKJSAVE[rno] = skjcnv.HJNCod;
		// 大文字変換
		memset( codbf, '\0', sizeof( codbf ) );
		memmove( codbf, skjcnv.SKJCod, 8 );
		_strupr_s( codbf, sizeof( codbf ) );

//2016.02.23 INSERT START
		skjcnv.HJNCod = 0;
		skjcnv.KJNCod = 0;
//2016.02.23 INSERT END

		// 固定資産
		if( !strncmp( codbf, "02", 2 )	)	{
			switch( Gflg ) {
				case 2 :	skjcnv.KJNCod = 6;		break;
				case 3 :	skjcnv.KJNCod = 10;		break;
				default:	skjcnv.KJNCod = 3;		break;
			}
		}
		// 繰延資産
		if( !strncmp( codbf, "03", 2 )	)	{
			switch( Gflg ) {
				case 2 :	skjcnv.KJNCod = 6;		break;
				case 3 :	skjcnv.KJNCod = 10;		break;
				default:	skjcnv.KJNCod = 3;		break;
			}
		}
		// 仕入
		if( !strncmp( codbf, "09", 2 )	)	{
			switch( Gflg ) {
				case 2 :	skjcnv.KJNCod = 5;		break;
				case 3 :	skjcnv.KJNCod = 8;		break;
				default:	skjcnv.KJNCod = 1;		break;
			}
		}
		// 経費
		if( !strncmp( codbf, "0A", 2 )	)	{
			switch( Gflg ) {
				case 2 :	skjcnv.KJNCod = 5;		break;
				case 3 :	skjcnv.KJNCod = 9;		break;
				default:	skjcnv.KJNCod = 2;		break;
			}
		}
		// 営業外費用
		if( !strncmp( codbf, "0B02", 4 ))	{
			switch( Gflg ) {
				case 2 :	skjcnv.KJNCod = 5;		break;
				case 3 :	skjcnv.KJNCod = 9;		break;
				default:	skjcnv.KJNCod = 2;		break;
			}
		}
		// 特別損失
		if( !strncmp( codbf, "0C02", 4 ))	{
			switch( Gflg ) {
				case 2 :	skjcnv.KJNCod = 5;		break;
				case 3 :	skjcnv.KJNCod = 9;		break;
				default:	skjcnv.KJNCod = 2;		break;
			}
		}
		// 製造原価
		if( !strncmp( codbf, "0F", 2 )	)	{
			switch( Gflg ) {
				case 2 :	skjcnv.KJNCod = 5;		break;
				case 3 :	skjcnv.KJNCod = 8;		break;
				default:	skjcnv.KJNCod = 1;		break;
			}
		}
		// 棚卸科目は含まない。
		if( !strncmp( codbf, "090101", 6 )	||	// 期首商品製品棚卸
			!strncmp( codbf, "090401", 6 )	||	// 期末商品税品棚卸
			!strncmp( codbf, "0F0101", 6 )	||	// 期首材料
			!strncmp( codbf, "0F0105", 6 ) ) {	// 期末材料
			skjcnv.KJNCod = 0;			// ×
		}
		if( m_pZmSub->zvol->apno == 0x01 )	{	// 運送業
			if( !strncmp( codbf, "0F0A01", 6 )	||	// 期首仕掛品
				!strncmp( codbf, "0F0A02", 6 ) ) {	// 期末仕掛品
				skjcnv.KJNCod = 0;		// ×
			}
		}
		else	{					// 運送業以外
			if( !strncmp( codbf, "0F0501", 6 )	||	// 期首仕掛品
				!strncmp( codbf, "0F0502", 6 ) ) {	// 期末仕掛品
				skjcnv.KJNCod = 0;		// ×
			}
		}
		// 集計外科目の追加
		if(  !strncmp( codbf, "020110", 6 ) ||											// 減価償却累計額
			(!strncmp( codbf, "020115", 6 ) && (m_pZmSub->zvol->apno != 0x10) ) ||		// 減損損失累計額
			(!strncmp( codbf, "020306", 6 ) && (m_pZmSub->zvol->apno == 0x00) ) ||		// 繰延税金資産
			(!strncmp( codbf, "020306", 6 ) && (m_pZmSub->zvol->apno == 0x01) ) ||		// 繰延税金資産
			(!strncmp( codbf, "020306", 6 ) && (m_pZmSub->zvol->apno == 0x20) ) ||		// 繰延税金資産
			(!strncmp( codbf, "020307", 6 ) && (m_pZmSub->zvol->apno != 0x30) ) ||		// 繰延消費税等
			(!strncmp( codbf, "02030C", 6 ) && (m_pZmSub->zvol->apno == 0x30) ) ||		// 繰延税金資産
			(!strncmp( codbf, "02030D", 6 ) && (m_pZmSub->zvol->apno == 0x30) ) ||		// 繰延消費税等
			(!strncmp( codbf, "020319", 6 ) && (m_pZmSub->zvol->apno != 0x10) ) )	{	// 貸倒引当金
			skjcnv.KJNCod = 0;		// ×
		}
/////////[08'01.09]
		if(	(!strncmp( codbf, "020319", 6 ) && (m_pZmSub->zvol->apno == 0x10) ) ||		// 
			(!strncmp( codbf, "0301", 4 )	&& (m_pZmSub->zvol->apno == 0x10) ) )	{	// 
			skjcnv.KJNCod = 0;		// ×
		}
///////////////////

		if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, rno ) )	{
			CString	Mesg;
			Mesg.Empty();
			if( m_pSnHeadData->IsUseTaxRefund() )
					Mesg = _T("還付明細用連動情報を初期化中に障害が発生しました。\n処理を中断します。");
			else	Mesg = _T("仕入控除税額明細用連動情報を初期化中に障害が発生しました。\n処理を中断します。");
			OnBnClickedCancel();
			return;
		}
		// 保存
		m_HKJSAVE[rno] = skjcnv.KJNCod;

		// 科目名称の取得
		memset( kkbf, '\0', sizeof( kkbf ) );
		if( m_Util.GetKnrecName( m_pZmSub, (char *)skjcnv.SKJCod, kkbf ) ) {
			ICSReportRecord*	pRecord	=	m_Report.AddRecord( new ICSReportRecord() );
			// 科目名称
			strName.Format( "  %s", kkbf );
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName ) );
			m_Report.SetCaption( rno, 0, strName );
			// 集計項目番号
			switch( skjcnv.KJNCod ) {
				case 1 :	strName.Format( "%2.2s", SKJ_NAME[0] );	wno = 0;	break;
				case 2 :	strName.Format( "%2.2s", SKJ_NAME[1] );	wno = 1;	break;
				case 3 :	strName.Format( "%2.2s", SKJ_NAME[2] );	wno = 2;	break;
				case 5 :	strName.Format( "%2.2s", SKJ_NAME[3] );	wno = 3;	break;
				case 6 :	strName.Format( "%2.2s", SKJ_NAME[4] );	wno = 4;	break;
				case 8 :	strName.Format( "%2.2s", SKJ_NAME[5] );	wno = 5;	break;
				case 9 :	strName.Format( "%2.2s", SKJ_NAME[6] );	wno = 6;	break;
				case 10:	strName.Format( "%2.2s", SKJ_NAME[7] );	wno = 7;	break;
				default:	strName.Format( "%2.2s", SKJ_NAME[8] );	wno = 8;	break;
			}
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName, ICS_EDS_NUMERIC, TRUE, FALSE, 4, ICS_EDS_RIGHT ));
			m_Report.SetCaption( rno, 1, strName );
			((ICSReportRecordItemText *)pItem)->EnableDelete(0);
			// 集計科目名称
			strName.Format( "%s", &SKJ_NAME[wno][3] );
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName, ICS_EDS_ZENKAKU, TRUE, FALSE, 4 ));
			m_Report.SetCaption( rno, 2, strName );
			((ICSReportRecordItemText *)pItem)->EnableDelete(0);
		}
		rno++;
	}
	m_Report.AllowEdit( TRUE );
	m_Report.AllowEditColumn( 0, FALSE );
	m_Report.AllowEditColumn( 1, TRUE  );
	m_Report.AllowEditColumn( 2, FALSE  );

	//データ設定の反映　再描画
	m_Report.Populate();
	//一行目へフォーカス
	m_Report.SetFocus();
	m_Report.SetFocusedRow( m_Report.GetRows()->GetAt( 0 ) );

	for( int i=0; i < rc; i++ ) {
		m_Report.SetCellBackColor( i, 0, S_CLASS_RGB );
	}
}
