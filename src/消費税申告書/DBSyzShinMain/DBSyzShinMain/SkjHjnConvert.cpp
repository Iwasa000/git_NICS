// SkjHjnConvert.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

//#include "MainFrm.h"
//#include "DBSyzShinDoc.h"
//#include "DBSyzShinView.h"

#include "SkjHjnConvert.h"

//static	SKJCNV	SkjCnv;

#define		S_CLASS_RGB		RGB( 255, 255, 220 )

static	char SKJ_NAME[][64] = {
	"01 商　品　仕　入　等　①",
	"02 販売費・一般管理費　②",
	"03 営　業　外　費　用　③",
	"04 そ　　　の　　　他　④",
	"06 固 　定  　資 　産　⑥",
	"07 繰 　延  　資 　産　⑦",
	"00 集　計　外　科　目　×",
	"　",
	0
};
//★//[12'01.17]///
static	char REF_NAME[][64] = {
	"01 商　品　仕　入　等　①",
	"02 販売費・一般管理費　②",
	"03 営　業　外　費　用　③",
	"04 そ　　　の　　　他　④",
	"06 固 　定  　資 　産　⑥",
	"07 繰 　延  　資 　産　⑦",
	"08 そ　　　の　　　他　⑧",
	"00 集　計　外　科　目　×",
	"　",
	0
};
///////////////////

/////////[07'06.28]
static	char	m_HKJSAVE[10240];			// 1024 → 10240 [09'07.13]
//////////////////

// メインビューの財務クラスを参照
//CDBSyzShinView *CSkjHjnConvert::pSyzShin;

// CSkjHjnConvert ダイアログ

IMPLEMENT_DYNAMIC(CSkjHjnConvert, ICSDialog)

CSkjHjnConvert::CSkjHjnConvert(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSkjHjnConvert::IDD, pParent)
	, m_pZmSub(NULL)
	, m_pSnHeadData(NULL)
{
	InizDataList = FALSE;
	Inp_l = Inp_c = 0;
	memset( m_HKJSAVE, '\0', sizeof( m_HKJSAVE ) );
}

CSkjHjnConvert::~CSkjHjnConvert()
{
}

void CSkjHjnConvert::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_Report);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CSkjHjnConvert, ICSDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_KANA, &CSkjHjnConvert::OnBnClickedButtonKana)
	ON_LBN_DBLCLK(IDC_LIST1, &CSkjHjnConvert::OnLbnDblclkList1)
	ON_NOTIFY(ICS_NM_REPORT_LBUTTONDOWN, IDC_CUSTOM1, OnNMReportLbuttondownCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_VALUECHANGED, IDC_CUSTOM1, OnNMReportValuechangedCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_CUSTOM1, OnNMReportEditkeydownCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_EDITSETFOCUS, IDC_CUSTOM1, OnNMReportEditsetfocusCUSTOM1)
	ON_NOTIFY(ICS_NM_REPORT_EDITKILLFOCUS, IDC_CUSTOM1, OnNMReportEditkillfocusCUSTOM1)
	ON_BN_CLICKED(IDOK, &CSkjHjnConvert::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSkjHjnConvert::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CSkjHjnConvert::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSkjHjnConvert::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSkjHjnConvert::OnBnClickedButton4)
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
int CSkjHjnConvert::InitInfo( CDBNpSub *pZmSub, CSnHeadData *pSnHeadData, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl )
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

// CSkjHjnConvert メッセージ ハンドラ

BOOL CSkjHjnConvert::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// TISW修正[22-0676] 2026/03/19 START
	if (m_isSkjUsrUnconfirmed)
	{
		// 設定を有効にする利用者が未確定の場合は登録不可とする
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	// TISW修正[22-0676] 2026/03/19 END

	// 集計先項目の表示
	m_ListBox.ResetContent();

	if( m_pSnHeadData->IsUseTaxRefund() ){
		for( int i = 0; REF_NAME[i][0]; i++ ) {
			m_ListBox.AddString( REF_NAME[i] );
			CString	Str;
			Str.Format( "%s", REF_NAME[i] );
			m_ListBox.SetItemData( i, atoi( Str.Left( 2 )) );
		}
	}
	else{
		for( int i = 0; SKJ_NAME[i][0]; i++ ) {
			m_ListBox.AddString( SKJ_NAME[i] );
			CString	Str;
			Str.Format( "%s", SKJ_NAME[i] );
			m_ListBox.SetItemData( i, atoi( Str.Left( 2 )) );
		}
	}

	CRect rect;
	m_Report.GetClientRect( &rect );
	m_Report.RemoveColumn( -1 );

	// キャプション変更
	CString	SkjHjnCaption;
	switch( m_pZmSub->zvol->apno ) {
		case 0x50 : SkjHjnCaption = _T("【公益用】");			break;
		case 0x51 : SkjHjnCaption = _T("【公益Ⅲ用】");			break;
		case 0x52 : SkjHjnCaption = _T("【社会福祉用】");		break;
		default   :	SkjHjnCaption = _T("【法人用】");			break;
	}
	if( m_pSnHeadData->IsUseTaxRefund() )	SkjHjnCaption += _T("還付申告明細用連動情報登録");
	else						SkjHjnCaption += _T("仕入控除税額明細用連動情報登録");
	SetWindowText( SkjHjnCaption );

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

void CSkjHjnConvert::OnSize(UINT nType, int cx, int cy)
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

// 科目検索
void CSkjHjnConvert::OnBnClickedButtonKana()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CWnd *pWnd = GetDlgItem( IDC_BUTTON_KANA );
	RECT rc;
	if( pWnd == NULL ) return;
	pWnd->GetWindowRect( &rc );

	KSEL_PAR	Kpar;

	ZeroMemory( &Kpar, sizeof( KSEL_PAR ));
	Kpar.ksPsgn		= KSP_RIGHTTOP;		// ポジション・サイン
	Kpar.ksFsgn		= KSF_ABSDISP;		// ファンクション・サイン
	Kpar.ksLnum		= 10;				// 表示科目数
	Kpar.ksKana;		// 呼び出し科目の頭文字（ASCII）
	Kpar.ksKrng;		// 対象科目（０：全科目／１：貸借／２：損益／他：（下位：開始／上位：終了））
	Kpar.ksPos.x	= rc.right;
	Kpar.ksPos.y	= rc.bottom;			// ダイアログ表示ポジション（スクリーン座標）
	Kpar.ksCwnd;		// ポジションチェック対象ウィンドウ
	Kpar.ksStat;		// ステータス・フラグ
	Kpar.ksFhwd;		// フォントサイズ設定対象ウィンドウハンドル（'98.9.28）
	CString cmd;

	switch( m_pZmSub->zvol->apno ) {
		case 0x50 : // 公益
			cmd  = _T( "(kncod >= '01050100' and kncod <= '010501FF') or " );
			cmd += _T( "(kncod >= '02000000' and kncod <  '03000000') or " );
			cmd += _T( "(kncod >= '09000000' and kncod <  '0B0101FF') or " );
			cmd += _T( "(kncod >= '0D000000' and kncod <  '0E000000') or " );
			cmd += _T( "(kncod >= '11000000' and kncod <  '13000000') or " );
			cmd += _T( "(kncod >= '80000000' and kncod <  '90000000')" );
			break;
		case 0x51 : // 公益Ⅲ
			cmd  = _T( "(kncod >= '01050100' and kncod <= '010501FF') or " );
			cmd += _T( "(kncod >= '02000000' and kncod <  '03000000') or " );
			cmd += _T( "(kncod >= '09000000' and kncod <  '0C000000') or " );
			cmd += _T( "(kncod >= '0C020000' and kncod <  '0C030000') or " );
			cmd += _T( "(kncod >= '0C040000' and kncod <  '0C050000') or " );
			cmd += _T( "(kncod >= '0C060000' and kncod <  '0C070000') or " );
			cmd += _T( "(kncod >= '0E010000' and kncod <  '0F000000') or " );
			cmd += _T( "(kncod >= '12000000' and kncod <  '14000000') or " );
			cmd += _T( "(kncod >= '15020000' and kncod <  '16000000') or " );
			cmd += _T( "(kncod >= '80000000' and kncod <  '90000000')" );
			break;
		case 0x52 : // 社会福祉
//★//[12'03.02]///
			if( m_pZmSub->zvol->m_ver == 0x11 ) {
				cmd  = _T( "(kncod >= '01040100' and kncod <= '010401FF') or " );
				cmd += _T( "(kncod >= '02000000' and kncod <  '02FFFFFF') or " );
				cmd += _T( "(kncod >= '19000000' and kncod <  '19FFFFFF') or " );
				cmd += _T( "(kncod >= '1B000000' and kncod <  '1BFFFFFF') or " );
				cmd += _T( "(kncod >= '1B050000' and kncod <  '1BFFFFFF') or " );
				cmd += _T( "(kncod >= '21010000' and kncod <  '2102FFFF') or " );
				cmd += _T( "(kncod >= '21030000' and kncod <  '2105FFFF') or " );
				cmd += _T( "(kncod >= '21060000' and kncod <  '2109FFFF') or " );
				cmd += _T( "(kncod >= '22000000' and kncod <  '2202FFFF') or " );
				cmd += _T( "(kncod >= '23010000' and kncod <  '0301FFFF') or " );
				cmd += _T( "(kncod >= '23020000' and kncod <  '2304FFFF') or " );
				cmd += _T( "(kncod >= '23050000' and kncod <  '2305FFFF') or " );
				cmd += _T( "(kncod >= '2B010000' and kncod <  '2B03FFFF') or " );
				cmd += _T( "(kncod >= '2B040100' and kncod <  '2B0401FF') or " );
				cmd += _T( "(kncod >= '2B040200' and kncod <  '2B0403FF') or " );
				cmd += _T( "(kncod >= '2B040400' and kncod <  '2B0405FF') or " );
				cmd += _T( "(kncod >= '2B040600' and kncod <  '2B0702FF') or " );
				cmd += _T( "(kncod >= '2B080100' and kncod <  '2B0801FF') or " );
				cmd += _T( "(kncod >= '2B090100' and kncod <  '2B0A01FF') or " );
				cmd += _T( "(kncod >= '2B0B0000' and kncod <  '2B0BFFFF') or " );
				cmd += _T( "(kncod >= '2B0C0000' and kncod <  '2B14FFFF')" );
			}
			else	{
				cmd  = _T( "(kncod >= '01040100' and kncod <= '010401FF') or " );
				cmd += _T( "(kncod >= '02000000' and kncod <  '03000000') or " );
				cmd += _T( "(kncod >= '09000000' and kncod <  '0A000000') or " );
				cmd += _T( "(kncod >= '0B000000' and kncod <  '0C000000') or " );
				cmd += _T( "(kncod >= '0D000000' and kncod <  '0E000000') or " );
				cmd += _T( "(kncod >= '0F000000' and kncod <  '10000000')" );
			}
///////////////////
			break;
		default   :	// 標準・運送・建設・病院
			cmd  = _T( "(kncod >= '01050100' and kncod <= '010501FF') or " );
			cmd += _T( "(kncod >= '02000000' and kncod <  '04000000') or " );
			cmd += _T( "(kncod >= '09000000' and kncod <  '0B000000') or " );
			cmd += _T( "(kncod >= '0B020000' and kncod <  '0C000000') or " );
			cmd += _T( "(kncod >= '0C020000' and kncod <  '0D000000') or " );
			cmd += _T( "(kncod >= '0F000000' and kncod <= '0FFFFFFF') " );
			break;
	}

//2016.02.23 INSERT START
	cmd += _T( "or (((cast(cast(knsgn as binary(1)) as int) & 0xff) = 0x01) and ((cast(cast(knsgn as binary(2)) as int) & 0xff) = 0x00)) " );
//2016.02.23 INSERT END

	SH_SKJ_CNV	skjcnv;

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

//////////////////
// 関数定義領域 //
/////////////////

// 変換情報の表示
void CSkjHjnConvert::SetListColm()
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
	
	SH_SKJ_CNV	skjcnv;

	int enableColor = RGB( 0xa0, 0xa0, 0xa0 );
	CString		strName;
	char		kkbf[128];
	int wno = 0;
	int	rno = 0;
	while( 1 ) {
		if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, rno ) ) break;
		// 保存
		m_HKJSAVE[rno] = skjcnv.HJNCod;

		// 科目名称の取得
		memset( kkbf, '\0', sizeof( kkbf ) );
		if( m_Util.GetKnrecName( m_pZmSub, (char *)skjcnv.SKJCod, kkbf ) ) {
			ICSReportRecord*	pRecord	=	m_Report.AddRecord( new ICSReportRecord() );
			// 科目名称
			strName.Format( "  %s", kkbf );
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName ) );
			m_Report.SetCaption( rno, 0, strName );
			// 集計項目番号
//★//[12'01.17]///
			if( m_pSnHeadData->IsUseTaxRefund() ) {
				switch( skjcnv.HJNCod ) {
					case 1 :	strName.Format( "%2.2s", REF_NAME[0] );	wno = 0;	break;
					case 2 :	strName.Format( "%2.2s", REF_NAME[1] );	wno = 1;	break;
					case 3 :	strName.Format( "%2.2s", REF_NAME[2] );	wno = 2;	break;
					case 4 :	strName.Format( "%2.2s", REF_NAME[3] );	wno = 3;	break;
					case 6 :	strName.Format( "%2.2s", REF_NAME[4] );	wno = 4;	break;
					case 7 :	strName.Format( "%2.2s", REF_NAME[5] );	wno = 5;	break;
					case 8 :	strName.Format( "%2.2s", REF_NAME[6] );	wno = 6;	break;
					default:	strName.Format( "%2.2s", REF_NAME[7] );	wno = 7;	break;
				}
			}
			else	{
				switch( skjcnv.HJNCod ) {
					case 1 :	strName.Format( "%2.2s", SKJ_NAME[0] );	wno = 0;	break;
					case 2 :	strName.Format( "%2.2s", SKJ_NAME[1] );	wno = 1;	break;
					case 3 :	strName.Format( "%2.2s", SKJ_NAME[2] );	wno = 2;	break;
					case 4 :	strName.Format( "%2.2s", SKJ_NAME[3] );	wno = 3;	break;
					case 6 :	strName.Format( "%2.2s", SKJ_NAME[4] );	wno = 4;	break;
					case 7 :	strName.Format( "%2.2s", SKJ_NAME[5] );	wno = 5;	break;
					default:	strName.Format( "%2.2s", SKJ_NAME[6] );	wno = 6;	break;
				}
			}
///////////////////
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName, ICS_EDS_NUMERIC, TRUE, FALSE, 4, ICS_EDS_RIGHT ));
			m_Report.SetCaption( rno, 1, strName );
			((ICSReportRecordItemText *)pItem)->EnableDelete(0);
			// 集計科目名称
//★//[12'01.17]///
			if( m_pSnHeadData->IsUseTaxRefund() ) strName.Format( "%s", &REF_NAME[wno][3] );
			else						strName.Format( "%s", &SKJ_NAME[wno][3] );
///////////////////
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
int CSkjHjnConvert::GetKjnCodName( int kjncod )
{
	CString findstr;
	findstr.Format( "%02d", kjncod );
	int r = m_ListBox.FindString( -1, findstr );

	return r;

}

// ポジションセット
void CSkjHjnConvert::SetSnameString( int indexRow, int indexColumn )
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
			skjcnv.HJNCod = r;
			if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;
		}
	}

	r = GetKjnCodName( skjcnv.HJNCod );
	m_ListBox.SetCurSel( r );
	m_ListBox.GetItemData( r );


//★//[12'01.17]///
	if( m_pSnHeadData->IsUseTaxRefund() )	{
		strName.Format( "%2.2s", REF_NAME[r] );
		m_Report.SetCaption( Inp_l, Inp_c, strName );

		strName.Format( "%s", &REF_NAME[r][3] );
		m_Report.SetCaption( Inp_l, Inp_c+1, strName );
	}
	else	{
		strName.Format( "%2.2s", SKJ_NAME[r] );
		m_Report.SetCaption( Inp_l, Inp_c, strName );

		strName.Format( "%s", &SKJ_NAME[r][3] );
		m_Report.SetCaption( Inp_l, Inp_c+1, strName );
	}
///////////////////
}

void CSkjHjnConvert::OnLbnDblclkList1()
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
		skjcnv.HJNCod = r;
		if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, Inp_l ) )	return;
		// 集計項目番号
//★//[12'01.17]///
		if( m_pSnHeadData->IsUseTaxRefund() )	{
			switch( skjcnv.HJNCod ) {
				case 1 :	Str.Format( "%2.2s", REF_NAME[0] );	r = 0; break;
				case 2 :	Str.Format( "%2.2s", REF_NAME[1] );	r = 1;	break;
				case 3 :	Str.Format( "%2.2s", REF_NAME[2] );	r = 2;	break;
				case 4 :	Str.Format( "%2.2s", REF_NAME[3] );	r = 3;	break;
				case 6 :	Str.Format( "%2.2s", REF_NAME[4] );	r = 4;	break;
				case 7 :	Str.Format( "%2.2s", REF_NAME[5] );	r = 5;	break;
				case 8 :	Str.Format( "%2.2s", REF_NAME[6] );	r = 6;	break;
				default:	Str.Format( "%2.2s", REF_NAME[7] );	r = 7;	break;
			}
			m_Report.SetCaption( Inp_l, 1, Str );

			Str.Format( "%s", &REF_NAME[r][3] );
			m_Report.SetCaption( Inp_l, 2, Str );
		}
		else	{
			switch( skjcnv.HJNCod ) {
				case 1 :	Str.Format( "%2.2s", SKJ_NAME[0] );	r = 0; break;
				case 2 :	Str.Format( "%2.2s", SKJ_NAME[1] );	r = 1;	break;
				case 3 :	Str.Format( "%2.2s", SKJ_NAME[2] );	r = 2;	break;
				case 4 :	Str.Format( "%2.2s", SKJ_NAME[3] );	r = 3;	break;
				case 6 :	Str.Format( "%2.2s", SKJ_NAME[4] );	r = 4;	break;
				case 7 :	Str.Format( "%2.2s", SKJ_NAME[5] );	r = 5;	break;
				default:	Str.Format( "%2.2s", SKJ_NAME[6] );	r = 6;	break;
			}
			m_Report.SetCaption( Inp_l, 1, Str );

			Str.Format( "%s", &SKJ_NAME[r][3] );
			m_Report.SetCaption( Inp_l, 2, Str );
		}
///////////////////
	}

	//データ設定の反映　再描画
	m_Report.Populate();
	m_Report.EditItem( Inp_l, 1, TRUE );

}

/////////[07'06.28]
///////////////////
void CSkjHjnConvert::OnNMReportLbuttondownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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
	int r = GetKjnCodName( skjcnv.HJNCod );
	m_ListBox.SetCurSel( r );
//★//[12'01.17]///
	if( m_pSnHeadData->IsUseTaxRefund() )	{
		if( r != -1 ) {
			str.Format( "%2.2s", REF_NAME[r] );
		}
		else str =  _T( "――" );
		m_Report.SetCaption( Inp_l, Inp_c, str );

		if( r != -1 ) {
			str.Format( "%s", &REF_NAME[r][3] );
		}
		else str =  _T( "―――――――――" );
		m_Report.SetCaption( Inp_l, Inp_c+1, str );

	}
	else	{
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
	}
///////////////////
}

void CSkjHjnConvert::OnNMReportValuechangedCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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

void CSkjHjnConvert::OnNMReportEditkeydownCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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

void CSkjHjnConvert::OnNMReportEditsetfocusCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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

	int r = GetKjnCodName( skjcnv.HJNCod );
	m_ListBox.SetCurSel( r );
//★//[12'01.17]///
	if( m_pSnHeadData->IsUseTaxRefund() )	{
		if( r != -1 ) {
			str.Format( "%2.2s", REF_NAME[r] );
		}
		else str =  _T( "――" );
		m_Report.SetCaption( Inp_l, Inp_c, str );

		if( r != -1 ) {
			str.Format( "%s", &REF_NAME[r][3] );
		}
		else str =  _T( "―――――――――" );
		m_Report.SetCaption( Inp_l, Inp_c+1, str );

	}
	else	{
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
	}
///////////////////
}

void CSkjHjnConvert::OnNMReportEditkillfocusCUSTOM1(NMHDR * pNotifyStruct, LRESULT * result)
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

void CSkjHjnConvert::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	ICSDialog::OnOK();
}

void CSkjHjnConvert::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int r = 0;
	SH_SKJ_CNV	skjcnv;
	while( 1 ) {
		if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, r ) )	break;
		skjcnv.HJNCod = m_HKJSAVE[r];
		if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, r ) )	continue;
		r++;
	}
	ICSDialog::OnCancel();
}

BOOL CSkjHjnConvert::PreTranslateMessage(MSG* pMsg)
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

void CSkjHjnConvert::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	ICSDialog::EndDialog( -2 );
}

void CSkjHjnConvert::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	ICSDialog::EndDialog( -3 );
}

// 初期設定
void CSkjHjnConvert::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	CString	Mesg;
	Mesg.Empty();
	if( m_pSnHeadData->IsUseTaxRefund() )
		Mesg = _T("還付明細用連動情報を初期化しますか？	");
	else	Mesg = _T("仕入控除税額明細用連動情報を初期化しますか？	");
	if( ICSMessageBox( Mesg, MB_YESNO | MB_DEFBUTTON2, 0, 0, this ) != IDYES )	{
		m_Report.SetFocus();
//		m_Report.SetFocusedRow( m_Report.GetRows()->GetAt( 0 ) );
		return;
	}

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

	DBKNREC		*pKnrec;
	SH_SKJ_CNV	skjcnv;
	CString		strName;
	char		kkbf[128], codbf[128];
	int wno = 0;
	int	rno = 0;
	while( 1 ) {
		if( m_pTblhdl->th_rget( &skjcnv, m_pSkjCnvtbl, rno ) ){
			break;
		}

		// 科目名称の読み込み
		pKnrec = GetKnrec( skjcnv.SKJCod );
		if( pKnrec == NULL ){
			Mesg.Empty();
			Mesg = _T("科目名称読み込み中に障害が発生しました。\n処理を中断します。");
			ICSMessageBox( Mesg, MB_OK, 0, 0, this );
			OnBnClickedCancel();
			return;
		}

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
		if( (m_pZmSub->zvol->apno&0xf0) > 0x30 )	{
			// 非営利法人
/////////[10'06.29]
//			// 有価証券
//			if( !strncmp( codbf, "01", 2 ) )	{
//				skjcnv.HJNCod = 6;		// ⑥
//			}
//			// 固定資産
//			if( !strncmp( codbf, "02", 2 )	)	{
//				skjcnv.HJNCod = 6;		// ⑥
//			}
//			// 繰延資産
//			if( !strncmp( codbf, "04", 2 )	)	{
//				skjcnv.HJNCod = 0;		// ×
//			}
//			// 仕入
//			if( !strncmp( codbf, "0903", 4 )	)	{
//				skjcnv.HJNCod = 1;		// ①
//			}
//			// 経費
//			if( !strncmp( codbf, "0A", 2 )	)	{
//				skjcnv.HJNCod = 2;		// ②
//			}
//			// 他会計繰出額
//			if( !strncmp( codbf, "0B", 2 )	)	{
//				skjcnv.HJNCod = 2;		// ②
//			}
//			// 一般正味財産　評価損
//			if( !strncmp( codbf, "0D", 2 )	)	{
//				skjcnv.HJNCod = 3;		// ③
//			}
//			// 指定正味財産　評価損
//			if( !strncmp( codbf, "11", 2 )	)	{
//				skjcnv.HJNCod = 4;		// ④
//			}
//			// 指定正味財産　一般振替
//			if( !strncmp( codbf, "12", 2 )	)	{
//				skjcnv.HJNCod = 0;		// ×
//			}
///////////////////
			skjcnv.HJNCod = 0;
			skjcnv.KJNCod = 0;
			switch( m_pZmSub->zvol->apno ) {
				case 0x51 :	// 公益Ⅲ
					if( !strncmp( codbf, "0105", 4 )	)	{
						if( m_pSnHeadData->IsUseTaxRefund() )	skjcnv.HJNCod = 8;		// ⑧
						else						skjcnv.HJNCod = 6;		// ⑥
					}
					if( !strncmp( codbf, "02", 2 )	)	{
						if( !strncmp( codbf, "0201", 4 )	||
							!strncmp( codbf, "0202", 4 )	||
							!strncmp( codbf, "02031", 5 )	||
							!strncmp( codbf, "02050A", 6 )	||
							!strncmp( codbf, "02050B", 6 )	||
							!strncmp( codbf, "02051", 5 ) )
								skjcnv.HJNCod = 0;
						else	skjcnv.HJNCod = 6;
					}
					if( !strncmp( codbf, "09", 2 ) || 
						!strncmp( codbf, "0A", 2 ) || 
						!strncmp( codbf, "8", 1 ) )	{
//						if( knrec.Kn_sz1 == 0x04 )	skjcnv.HJNCod = 1;
						if( pKnrec->knsgn[0] == 0x04 )	skjcnv.HJNCod = 1;
//						if( knrec.Kn_sz1 == 0x08 )	skjcnv.HJNCod = 2;
						if( pKnrec->knsgn[0] == 0x08 )	skjcnv.HJNCod = 2;
					}
					if( !strncmp( codbf, "0B", 2 ) )	{
						skjcnv.HJNCod = 0;
					}
					if( !strncmp( codbf, "0C02", 4 ) ||
						!strncmp( codbf, "0C04", 4 ) ||	
						!strncmp( codbf, "0C06", 4 ) )	{
						skjcnv.HJNCod = 3;
					}
					if( !strncmp( codbf, "0E", 2 )	)	{
						skjcnv.HJNCod = 3;
					}
					if( !strncmp( codbf, "12", 2 ) ||
						!strncmp( codbf, "13", 2 )	)	{
						skjcnv.HJNCod = 4;
					}
					if( !strncmp( codbf, "1502", 4 )	)	{
						skjcnv.HJNCod = 0;
					}
					break;
					
				case 0x52 :	// 社会福祉
//★//[12'03.02]///
					if( m_pZmSub->zvol->m_ver == 0x11 ) {
						if( !strncmp( codbf, "0104", 4 )	)	{		// 有価証券
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "02", 2 )	)	{			//基財・土地	～	長期繰延税資産
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "19", 2 )	)	{			// 役員報酬		～	基金繰入額（事
							skjcnv.HJNCod = 2;		// ②販売費・一般管理費
						}
						if( !strncmp( codbf, "1907", 4 )	)	{			// 事)国補取崩額
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "1B", 2 )	)	{			// 支払利息		～	雑支出
							skjcnv.HJNCod = 3;		// ②販売費・一般管理費
						}
						if( !strncmp( codbf, "1D", 2 )	)	{			// １号基本組入額　～　他特別損失
							skjcnv.HJNCod = 3;		// ②販売費・一般管理費
						}
						if( !strncmp( codbf, "1D04", 4 )	)	{			// 特)国補取崩額
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "21", 2 ) )	{			// 期首製品棚卸高　事)	～	期末材料棚卸高
							skjcnv.HJNCod = 1;		// ①商品仕入等
						}
						if( !strncmp( codbf, "210101", 6 ) ||
							!strncmp( codbf, "210201", 6 ) ||
							!strncmp( codbf, "210203", 6 ) ||
							!strncmp( codbf, "210510", 6 ) ||
							!strncmp( codbf, "210601", 6 ) ||
							!strncmp( codbf, "210701", 6 ) ||
							!strncmp( codbf, "210901", 6 ) )	{			// 利用者賃金	～	雑費
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "22", 2 ) )	{			// 特）利用者賃金	～	雑費
							skjcnv.HJNCod = 2;		// ②販売費・一般管理費
						}
						if( !strncmp( codbf, "20211", 4 ) )	{			// 特）利用者賃金	～	雑費
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "23", 2 ) )	{			// 材料仕入高
							skjcnv.HJNCod = 1;		// ①商品仕入等
						}
						if( !strncmp( codbf, "230411", 6 ) ||
							!strncmp( codbf, "2305", 4 ) )	{		// 利用者工賃	～	雑費
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "2B01", 4 ) ||
							!strncmp( codbf, "2B02", 4 ) ||
							!strncmp( codbf, "2B03", 4 ) )	{		// 管理費返還	～	設備償還金
							skjcnv.HJNCod = 4;		// ④その他
						}
						if( !strncmp( codbf, "2B04", 4 ) ||
							!strncmp( codbf, "2B05", 4 ) ||
							!strncmp( codbf, "2B06", 4 ) ||
							!strncmp( codbf, "2B07", 4 ) ||
							!strncmp( codbf, "2B08", 4 ) ||
							!strncmp( codbf, "2B0A", 4 ) ||
							!strncmp( codbf, "2B0C", 4 ) ||
							!strncmp( codbf, "2B0D", 4 ) ||
							!strncmp( codbf, "2B0E", 4 ) ||
							!strncmp( codbf, "2B0F", 4 ) ||
							!strncmp( codbf, "2B10", 4 ) ||
							!strncmp( codbf, "2B11", 4 ) ||
							!strncmp( codbf, "2B12", 4 ) ||
							!strncmp( codbf, "2B13", 4 ) ||
							!strncmp( codbf, "2B14", 4 ) )	{
							skjcnv.HJNCod = 6;		// ⑥固定資産
						}
						// 有価証券
						if( !strncmp( codbf, "2B09", 4 ) ||
							!strncmp( codbf, "2B0B", 4 ) )	{
							skjcnv.HJNCod = 8;		// ⑧その他
						}
						if( !strncmp( codbf, "2C", 2 )	)	{			// 有価証券
							skjcnv.HJNCod = 0;		// ①商品仕入等
						}
					}
					else	{
						if( !strncmp( codbf, "0104", 4 ) )	{
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "02", 2 ) )	{
							skjcnv.HJNCod = 0;
						}
						if( !strncmp( codbf, "09", 2 ) )	{
							skjcnv.HJNCod = 2;
						}
						if( !strncmp( codbf, "0B", 2 ) )	{
							skjcnv.HJNCod = 3;
						}
						if( !strncmp( codbf, "0D", 2 ) )	{
							skjcnv.HJNCod = 4;
						}
						if( !strncmp( codbf, "0F", 2 )	)	{
							if( !strncmp( codbf, "0F01", 4 ) ) skjcnv.HJNCod = 4;
							else	if( !strncmp( codbf, "0F1", 3 ) ) skjcnv.HJNCod = 0;
							else	skjcnv.HJNCod = 6;
						}
					}
/////////////////////
					break;
//★//[12'12.13]///
				case 0x58 :
					// 固定資産の範囲
					if( !strncmp( codbf, "0201", 4 ) || !strncmp( codbf, "0202", 4 ) ) {
						if( !strncmp( codbf, "02022", 5 ) )	
								skjcnv.HJNCod = 0;
						else	skjcnv.HJNCod = 6;
					}
					if( !strncmp( codbf, "0305", 4 ) )	{
						if( m_pSnHeadData->IsUseTaxRefund() )	skjcnv.HJNCod = 8;		// ⑧
						else				skjcnv.HJNCod = 6;		// ⑥
					}
					if( !strncmp( codbf, "09", 2 ) || 
						!strncmp( codbf, "0A", 2 ) )	{
//						if( knrec.Kn_sz1 == 0x08 )	skjcnv.HJNCod = 2;
						if( pKnrec->knsgn[0] == 0x08 )	skjcnv.HJNCod = 2;
					}
					break;
///////////////////

//★//[11'08.03]
				case 0x59 :
					if( !strncmp( codbf, "02", 2 )	)	{
						skjcnv.HJNCod = 0;
					}
					if( !strncmp( codbf, "03", 2 )	)	{
						skjcnv.HJNCod = 0;
					}
					if( !strncmp( codbf, "0A", 2 )	)	{
						skjcnv.HJNCod = 2;
					}
					if( !strncmp( codbf, "0A0B01", 6 )	)	{
						if( m_pSnHeadData->IsUseTaxRefund() )	skjcnv.HJNCod = 8;		// ⑧
					}
					if( !strncmp( codbf, "0B01", 4 )	||
						!strncmp( codbf, "0B02", 4 )	||
						!strncmp( codbf, "0B03", 4 )	||
						!strncmp( codbf, "0B04", 4 )	||
						!strncmp( codbf, "0B05", 4 )	||
						!strncmp( codbf, "0B06", 4 )	||
						!strncmp( codbf, "0B09", 4 )	)	{
//★//[11'11.09]///
//						skjcnv.HJNCod = 1;
///////////////////
						skjcnv.HJNCod = 6;
///////////////////
					}
					break;
///////////////////

				default   :	// 公益
					if( !strncmp( codbf, "0105", 4 )	)	{
						if( m_pSnHeadData->IsUseTaxRefund() )	skjcnv.HJNCod = 8;		// ⑧
						else						skjcnv.HJNCod = 6;		// ⑥
					}
					if( !strncmp( codbf, "02", 2 )	)	{
						if( !strncmp( codbf, "0201", 4 )	||
							!strncmp( codbf, "0202", 4 )	||
							!strncmp( codbf, "02031", 5 )	||
							!strncmp( codbf, "02050A", 6 )	||
							!strncmp( codbf, "02050B", 6 )	||
							!strncmp( codbf, "02051", 5 ) )
								skjcnv.HJNCod = 0;
						else	skjcnv.HJNCod = 6;
					}
					if( !strncmp( codbf, "09", 2 ) || 
						!strncmp( codbf, "0A", 2 ) || 
						!strncmp( codbf, "8", 1 ) || 
						!strncmp( codbf, "0B", 2 ) )	{
//						if( knrec.Kn_sz1 == 0x04 )	skjcnv.HJNCod = 1;
						if( pKnrec->knsgn[0] == 0x04 )	skjcnv.HJNCod = 1;
//						if( knrec.Kn_sz1 == 0x08 )	skjcnv.HJNCod = 2;
						if( pKnrec->knsgn[0] == 0x08 )	skjcnv.HJNCod = 2;
					}
					if( !strncmp( codbf, "0D", 2 )	)	{
						skjcnv.HJNCod = 3;
					}
					if( !strncmp( codbf, "11", 2 ) ||
						!strncmp( codbf, "12", 2 )	)	{
						skjcnv.HJNCod = 4;
					}
					break;
			}
///////////////////
		}
		else	{
			// 有価証券
//			if( !strncmp(codbf, "01", 2) && knrec.Kn_sz1 == 0x11 )	{
			if( !strncmp(codbf, "01", 2) && (pKnrec->knsgn[0]==0x11) ){
				if( m_pSnHeadData->IsUseTaxRefund() )	skjcnv.HJNCod = 8;		// ⑧	
				else						skjcnv.HJNCod = 6;		// ⑥
			}
			// 固定資産
			if( !strncmp( codbf, "02", 2 )	)	{
				skjcnv.HJNCod = 6;		// ⑥
			}
			// 繰延資産
			if( !strncmp( codbf, "03", 2 )	)	{
				skjcnv.HJNCod = 7;		// ⑦
			}
			// 仕入
			if( !strncmp( codbf, "09", 2 )	)	{
				skjcnv.HJNCod = 1;		// ①
			}
			// 経費
			if( !strncmp( codbf, "0A", 2 )	)	{
				skjcnv.HJNCod = 2;		// ②
			}
			// 営業外費用
			if( !strncmp( codbf, "0B02", 4 ))	{
				skjcnv.HJNCod = 3;		// ③
			}
			// 特別損失
			if( !strncmp( codbf, "0C02", 4 ))	{
				skjcnv.HJNCod = 4;		// ④
			}
			// 製造原価
			if( !strncmp( codbf, "0F", 2 )	)	{
				skjcnv.HJNCod = 1;		// ①
			}
			// 棚卸科目は含まない。
			if( !strncmp( codbf, "090101", 6 )	||	// 期首商品製品棚卸
				!strncmp( codbf, "090401", 6 )	||	// 期末商品税品棚卸
				!strncmp( codbf, "0F0101", 6 )	||	// 期首材料
				!strncmp( codbf, "0F0105", 6 ) ) {	// 期末材料
				skjcnv.HJNCod = 0;			// ×
			}
			if( m_pZmSub->zvol->apno == 0x01 )	{	// 運送業
				if( !strncmp( codbf, "0F0A01", 6 )	||	// 期首仕掛品
					!strncmp( codbf, "0F0A02", 6 ) ) {	// 期末仕掛品
					skjcnv.HJNCod = 0;		// ×
				}
			}
			else	{					// 運送業以外
				if( !strncmp( codbf, "0F0501", 6 )	||	// 期首仕掛品
					!strncmp( codbf, "0F0502", 6 ) ) {	// 期末仕掛品
					skjcnv.HJNCod = 0;		// ×
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
				skjcnv.HJNCod = 0;		// ×
			}
		}

		if( m_pTblhdl->th_rput( &skjcnv, m_pSkjCnvtbl, rno ) )	{
			Mesg.Empty();
			Mesg = _T("還付明細用連動情報を初期化中に障害が発生しました。\n処理を中断します。");
			OnBnClickedCancel();
			return;
		}

		// 科目名称の取得
		memset( kkbf, '\0', sizeof( kkbf ) );
		if( !strncmp( (char *)skjcnv.SKJCod, "09020401", 8 ) )	{
			int i = 0;
		}
		if( m_Util.GetKnrecName( m_pZmSub, (char *)skjcnv.SKJCod, kkbf ) ) {
			ICSReportRecord*	pRecord	=	m_Report.AddRecord( new ICSReportRecord() );
			// 科目名称
			strName.Format( "  %s", kkbf );
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName ) );
			m_Report.SetCaption( rno, 0, strName );

TRACE("\n%8.8s[%20.20s]",skjcnv.SKJCod, strName );

			// 集計項目番号
//★//[12'01.17]///
			if( m_pSnHeadData->IsUseTaxRefund() ) {
				switch( skjcnv.HJNCod ) {
					case 1 :	strName.Format( "%2.2s", REF_NAME[0] );	wno = 0;	break;
					case 2 :	strName.Format( "%2.2s", REF_NAME[1] );	wno = 1;	break;
					case 3 :	strName.Format( "%2.2s", REF_NAME[2] );	wno = 2;	break;
					case 4 :	strName.Format( "%2.2s", REF_NAME[3] );	wno = 3;	break;
					case 6 :	strName.Format( "%2.2s", REF_NAME[4] );	wno = 4;	break;
					case 7 :	strName.Format( "%2.2s", REF_NAME[5] );	wno = 5;	break;
					case 8 :	strName.Format( "%2.2s", REF_NAME[6] );	wno = 6;	break;
					default:	strName.Format( "%2.2s", REF_NAME[7] );	wno = 7;	break;
				}
			}
			else	{
				switch( skjcnv.HJNCod ) {
					case 1 :	strName.Format( "%2.2s", SKJ_NAME[0] );	wno = 0;	break;
					case 2 :	strName.Format( "%2.2s", SKJ_NAME[1] );	wno = 1;	break;
					case 3 :	strName.Format( "%2.2s", SKJ_NAME[2] );	wno = 2;	break;
					case 4 :	strName.Format( "%2.2s", SKJ_NAME[3] );	wno = 3;	break;
					case 6 :	strName.Format( "%2.2s", SKJ_NAME[4] );	wno = 4;	break;
					case 7 :	strName.Format( "%2.2s", SKJ_NAME[5] );	wno = 5;	break;
					default:	strName.Format( "%2.2s", SKJ_NAME[6] );	wno = 6;	break;
				}
			}
///////////////////
			pItem	=	pRecord->AddItem( new ICSReportRecordItemText( strName, ICS_EDS_NUMERIC, TRUE, FALSE, 4, ICS_EDS_RIGHT ));
			m_Report.SetCaption( rno, 1, strName );
			((ICSReportRecordItemText *)pItem)->EnableDelete(0);
			// 集計科目名称
//★//[12'01.17]///
			if( m_pSnHeadData->IsUseTaxRefund() )	strName.Format( "%s", &REF_NAME[wno][3] );
			else						strName.Format( "%s", &SKJ_NAME[wno][3] );
///////////////////
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

//-----------------------------------------------------------------------------
// 科目名称レコードの取得
//-----------------------------------------------------------------------------
// 引数		pkcd		：	科目コード
//-----------------------------------------------------------------------------
// 返送値	NULL		：	該当無し
//			NULL以外	：	該当科目名称レコード
//-----------------------------------------------------------------------------
DBKNREC* CSkjHjnConvert::GetKnrec( unsigned char *pkcd )
{
	ASSERT( pkcd );
	if( pkcd == NULL ){
		return NULL;
	}

	if( m_pZmSub->pKnrec == NULL ){
		m_pZmSub->KamokuMeisyoRead();
	}

	unsigned long	tmpkcd = 0;
	m_Util.AscToBcd( (char*)&tmpkcd, (char*)pkcd, 8 );
	return m_pZmSub->PjisToKnrec( tmpkcd );
}


