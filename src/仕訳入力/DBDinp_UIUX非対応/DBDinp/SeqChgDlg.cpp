// SeqChgDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "SeqChgdlg.h"

#include "input1.h"
#include "external.h"



// CSeqChgDlg ダイアログ

#define SC_SEQINP_PN			0
#define SC_MOVE_PN				1
#define SC_CANCEL_PN			2



IMPLEMENT_DYNAMIC(CSeqChgDlg, ICSDialog)

CSeqChgDlg::CSeqChgDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSeqChgDlg::IDD, pParent)
{
	m_BaseSeq = -1;
}

CSeqChgDlg::~CSeqChgDlg()
{
}

void CSeqChgDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSeqChgDlg, ICSDialog)
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus )
END_MESSAGE_MAP()


// CSeqChgDlg メッセージ ハンドラ

BOOL CSeqChgDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	ICSDialog::OnInitDialogEX();

	SeqInfoSet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}



void CSeqChgDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	VARIANT var;
	char	data[32];
	int seq = -1;
	CString msg;

	DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( var.pbVal != NULL )	{
		int len = strlen( (char*)var.pbVal );
		strncpy_s( data, sizeof data, (char*)var.pbVal, len );

		seq = atoi( data );

		int chk_dacnt = pDBzm->m_dacnt;
		// 最大登録数のSEQ+1 の場合は、＋１を許可する。(元に戻す機能として)

		// SEQ が最後の登録か？
		for( int i = 0; i < m_sCnt; i++ ) {
			if( m_SeqAry[i] == chk_dacnt ) {
				chk_dacnt += 1;
			}
		}

		if( seq > chk_dacnt ) {
			msg.Format( "登録仕訳数を超えた仕訳番号です！" );
			ICSMessageBox( msg, 0,0,0, this );
			c_set( IDC_ICSDBEDTCTRL1 );
			return;
		}
		if( seq <= 0 ) {
			msg.Format( "入力された仕訳番号が不正です！\r\n確認してください。" );
			ICSMessageBox( msg, 0,0,0, this );
			c_set( IDC_ICSDBEDTCTRL1 );
			return;
		}

		for( int i = 0; i < m_sCnt; i++ ) {
			if( m_SeqAry[i] == seq ) {
				msg.Format( "選択された仕訳と同じ番号[%d]には、移動することはできません！", seq );
				ICSMessageBox( msg, 0,0,0, this );
				c_set( IDC_ICSDBEDTCTRL1 );
				return;
			}
		}

		for( int i = 0; i < m_sCnt; i++ ) {
			if( pDBzm->DBDATA_SetPosition( m_SeqAry[i] ) == 0 ) {

				int ret = pDBzm->dbdata->DataCorrect( seq );
				// 追加修正サイン
				pDBzm->IsModify() = TRUE;

				if( ret == -1 ) {
					ICSExit( 0, "仕訳データの移動の修正ができません！" );
					return;
				}
//msg.Format( "DataCorrect ---- 挿入元= %d, dbdata->seq = %d\n", seq, pDBzm->dbdata->seq );
//AfxMessageBox(msg);
			}
			else {
				ICSExit( 0, "仕訳データを取得できません！" );
				return;
			}
		}
	}
	else {
		msg.Format( "仕訳番号が入力されていません！" );
		ICSMessageBox( msg );
		c_set( IDC_ICSDBEDTCTRL1 );
		return;

	}

	m_BaseSeq = seq;

	ICSDialog::OnOK();
}


extern void MakeShortString(CDC* pDC, CString &cs, int nColumnLen, int nOffset);



void CSeqChgDlg::SeqInfoSet()
{
	int max_count = m_sCnt;	// 登録数

	CString msg;
	char	bf[20];
	int sg = 0;

	for( int i = 0; i < max_count; i++ ) {
		if( sg )	sprintf_s( bf, sizeof bf, ",%d", m_SeqAry[i]  );
		else		sprintf_s( bf, sizeof bf, "%d", m_SeqAry[i] );
		sg++;
		msg += bf;
	}

	CWnd* pwnd;
	pwnd = GetDlgItem(IDC_STATIC1);

	if( pwnd ) {
		CClientDC dc( pwnd );
		CFont* font = pwnd->GetFont();
		dc.SelectObject( font );
		
		CRect rect;
		pwnd->GetWindowRect( &rect );
		ScreenToClient( rect );

		int width = rect.Width()/2;

		MakeShortString( &dc, msg, width, 0 ); 

		CString newMsg;
		newMsg.Format( "選択された仕訳[ %s ]を", msg );

		pwnd->SetWindowText( newMsg );
	}

}


BEGIN_EVENTSINK_MAP(CSeqChgDlg, ICSDialog)
ON_EVENT(CSeqChgDlg, IDC_ICSDBEDTCTRL1, 1, CSeqChgDlg::TerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CSeqChgDlg::TerminationIcsdbedtctrl1(long nChar, long inplen, long kst)
{
	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	char tmp[64];
	VARIANT var;

	if( nChar == VK_DELETE )
	{
		::ZeroMemory( tmp, sizeof tmp );
		var.pbVal = (BYTE*)tmp;
		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
	{
		set_focus( SC_MOVE_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 || nChar == VK_UP || nChar == VK_DOWN )
	{
		set_focus( SC_CANCEL_PN );
	}
	else
		Buzzer();

}

void CSeqChgDlg::set_focus( int pn )
{
	if( pn >= 0 )
	{
		PostMessage(WM_RETROFOCUS, pn);
	}
}


LRESULT CSeqChgDlg::RetroFocus( WPARAM wParam, LPARAM lParam )
{
	switch( wParam ) {
	case SC_SEQINP_PN:
		c_set( IDC_ICSDBEDTCTRL1 );
		break;
	case SC_MOVE_PN:
		c_set( IDOK );
		break;
	case SC_CANCEL_PN:
		c_set( IDCANCEL );
		break;
	}

	return 1;
}


BOOL CSeqChgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_ESCAPE ) {
			EndDialog(IDCANCEL);
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
