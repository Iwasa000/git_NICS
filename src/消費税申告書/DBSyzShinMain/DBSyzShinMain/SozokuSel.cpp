// CSozokuSel.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "SozokuSel.h"
#include "afxdialogex.h"
#include "resource.h"

// CSozokuSel ダイアログ

// 納付書・相続人対応のため追加

IMPLEMENT_DYNAMIC(CSozokuSel, ICSDialog)

CSozokuSel::CSozokuSel(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_SOZOKU_SEL, pParent)
{
	m_sozokumax		= 0;
	m_chgsel_sgn	= 0;
}

CSozokuSel::~CSozokuSel()
{
}

void CSozokuSel::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_report);
}


BEGIN_MESSAGE_MAP(CSozokuSel, ICSDialog)
	ON_BN_CLICKED(IDC_CHGSEL_BTN, &CSozokuSel::OnBnClickedChgselBtn)
	ON_BN_CLICKED(IDOK, &CSozokuSel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSozokuSel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSozokuSel メッセージ ハンドラー

BOOL CSozokuSel::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CreateReport();
	SetReportData();
	m_report.SetFocus();
	m_report.SetFocusedRow( 0 );

	ICSDialog::OnInitDialogEX();

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

int CSozokuSel::InitInfo( NOHFUSHO_INFO *nohfuinfo, NOHFUSHO_DATA *nohfudata, CArray<int> &SozokuNoAry, int nohfuinfomax )
{
	for( int i = 0; i < nohfuinfomax; i++ ){
		memset( &NOHFU_INFO[i], '\0', sizeof(NOHFU_INFO[i]) );
		memset( &NOHFU_DATA[i], '\0', sizeof(NOHFU_DATA[i]) );
		NOHFU_INFO[i] = nohfuinfo[i];
		NOHFU_DATA[i] = nohfudata[i];
	}

	m_SozokuNoAry.RemoveAll();
	m_SozokuNoAry.Copy( SozokuNoAry );

	m_sozokumax = nohfuinfomax;

	return 0;
}

int CSozokuSel::CreateReport()
{
	m_report.GetClientRect( m_rctRep );
	m_report.Resize( TRUE, ICSResizeDefault );

	// データ削除
	ICSReportRecords	*pRecords	=	m_report.GetRecords();
	pRecords->RemoveAll();

	m_report.RemoveColumn( -1 );

	// カラムの設定
	BOOL	bResize	=	TRUE;	// カラム幅　自動リサイズフラグTRUEの場合自動リサイズ
	m_report.AddColumn( new ICSReportColumn( 0, _T(""), m_rctRep.Width()/20*2, bResize, ICS_REPORT_NOICON , FALSE, TRUE, DT_LEFT) );
	m_report.AddColumn( new ICSReportColumn( 1, _T(""), m_rctRep.Width()/20*18, bResize, ICS_REPORT_NOICON , FALSE, TRUE, DT_LEFT) );

	// ヘッダの削除不可
	m_report.SetRemovable( FALSE );
	// ヘッダのソート不可
	m_report.SetSortable( FALSE );
	// ヘッダのドラッグ不可
	m_report.SetDraggable( FALSE );
	// シングルクリックでエディット不可
	m_report.EditOnClick( FALSE );
	// 複数行選択不可
	m_report.SetMultipleSelection( FALSE );

	// 垂直グリッド線の色指定
	ICSReportGridStyle	m_styleHorPer;
	m_styleHorPer = RGS_NOLINES;
	COLORREF black = RGB( 0, 0, 0 );
	m_report.SetGridPerCount( TRUE, 1, black, m_styleHorPer );

	// 水平グリッド線の色指定
	m_report.SetGridPerCount( FALSE, 1, black, m_styleHorPer );

	// ヘッダー非表示
	m_report.ShowHeader( FALSE );

	// フォント設定
	CFont* pFont = GetDlgItem( IDC_CHGSEL_BTN )->GetFont();
	LOGFONT lf;
	pFont->GetLogFont( &lf );
	m_report.SetTextFont( lf );
	m_report.SetHeaderFont( lf );

	return 0;
}

int CSozokuSel::SetReportData()
{
	ICSReportRecord			*pRecord = NULL;
	ICSReportRecordItem		*pItem = NULL;
	CString					str;

	m_report.RemoveAllRecords();

	for( int i = 0; i < m_sozokumax; i++ ){
		pRecord = m_report.AddRecord( new ICSReportRecord() );
		str.Format( "%s", NOHFU_INFO[i].NINFNAME2 );
		pItem = pRecord->AddItem( new ICSReportRecordItemCheckBoxEx( "", 0, ICS_CHECKS_CENTER ) );
		pItem = pRecord->AddItem( new ICSReportRecordItemText( str, ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII, TRUE, FALSE, 4, ICS_EDS_LEFT ));
	}

	m_report.Populate();
	m_report.AllowEdit( TRUE );
	m_report.AllowEditColumn( 0, TRUE );
	m_report.AllowEditColumn( 1, FALSE );

	return 0;
}

void CSozokuSel::OnBnClickedChgselBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ChangeSelect();
}

void CSozokuSel::ChangeSelect()
{
	int recmax = m_report.GetRows()->GetCount();
	for( int i = 0; i < recmax; i++ ){
		if( !m_chgsel_sgn ){
			// 全選択
			m_report.SetChecked( i, CLM_CHECK, TRUE );
		}
		else{
			// 全解除
			m_report.SetChecked( i, CLM_CHECK, FALSE );
		}
	}
	m_report.RedrawControl();

	m_chgsel_sgn = !m_chgsel_sgn;
}

void CSozokuSel::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	// 選択された相続人の情報をセット
	for( int i=0; i<40; i++ ){
		memset( SELECT_INFO, '\0', sizeof(SELECT_INFO) );
		memset( SELECT_DATA, '\0', sizeof(SELECT_DATA) );
	}
	m_SelectNoAry.RemoveAll();

	int cnt = 0;
	int recmax = m_report.GetRows()->GetCount();
	for( int i = 0; i < recmax; i++ ){
		if( m_report.IsChecked( i, CLM_CHECK ) == TRUE ){
			SELECT_INFO[cnt] = NOHFU_INFO[i];
			SELECT_DATA[cnt] = NOHFU_DATA[i];
			m_SelectNoAry.Add( m_SozokuNoAry.GetAt( i ) );
			cnt++;
		}
	}

	if( cnt <= 0 ){
		ICSMessageBox( "相続人が選択されていません。" );
		return;
	}

	ICSDialog::OnOK();
}

void CSozokuSel::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	ICSDialog::OnCancel();
}

BOOL CSozokuSel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CWnd *pWnd = GetFocus();
	if ( pMsg->message == WM_KEYDOWN )	{

		if( pMsg->wParam == VK_F12 ){
			ChangeSelect();
			return TRUE;
		}
		if( pMsg->wParam == VK_END ){
			return TRUE;
		}
		if( pMsg->wParam == VK_SPACE ){
			if( pWnd == &m_report ){
				int rowidx = m_report.GetFocusedRow()->GetIndex();
				if( m_report.IsChecked( rowidx, CLM_CHECK ) == FALSE ){
					m_report.SetChecked( rowidx, CLM_CHECK, TRUE );
				}
				else{
					m_report.SetChecked( rowidx, CLM_CHECK, FALSE );
				}
				m_report.RedrawControl();
			}
			if( pWnd->m_hWnd == GetDlgItem(IDC_CHGSEL_BTN)->m_hWnd ){
				ChangeSelect();
			}
			if( pWnd->m_hWnd == GetDlgItem(IDOK)->m_hWnd ){
				OnBnClickedOk();
			}
			if( pWnd->m_hWnd == GetDlgItem(IDCANCEL)->m_hWnd ){
				OnBnClickedCancel();
			}
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
