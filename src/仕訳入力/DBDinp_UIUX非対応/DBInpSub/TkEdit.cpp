// TkEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "TkEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTkEdit

extern
int get_strcnt(char* str, int byte);

// ボタンの状態変更
#define BTN_STATE( id, sw )\
		if( m_pParent && id ) {\
			((CButton*)m_pParent->GetDlgItem( id ))->SetState( sw );\
		}\


// 文字位置(全角も半角も1文字として扱う)からバイト数を返送
//	文字位置( 0文字目から始まる)
//
int scnt_bytecnt( int strPos, char* string )
{
	int len, i, scnt, rval, st;
	len = strlen( string );

	scnt = 0;
	rval = -1;

	for( i = 0; i <= len; ) {
		st = IsKindOfLetter( string, i );

		if( strPos == scnt ) {
			rval = i;
			break;
		}

		scnt++;
		switch( st ) {
		case 1:		// ASCII 文字
			i++;
			break;
		case 2:		// MB文字1バイト目
			i += 2;
			break;
		default:
			if( string[i] == '\0' )
				i++;
		}
	}

	return rval;
}

// バイト数から文字位置(全角も半角も1文字として扱う)を返送
//	文字位置( 0文字目から始まる)
//
int bytecnt_scnt( int bytecnt, char* string )
{
	int len, i, scnt, rval, st;
	len = strlen( string );

	scnt = 0;
	rval = -1;

	for( i = 0; i <= len; ) {
		st = IsKindOfLetter( string, i );

		if( i == bytecnt ) {
			rval = scnt;
			break;
		}

		scnt++;
		switch( st ) {
		case 1:		// ASCII 文字
			i++;
			break;
		case 2:		// MB文字1バイト目
			i += 2;
			break;
		default:
			if( string[i] == '\0' )
				i++;
		}
	}

	return rval;
}



CTkEdit::CTkEdit()
{
	m_pParent = NULL;
	::ZeroMemory( m_BtnID, sizeof(UINT)*TKBTN_MAX );
	m_DspID = 0;

	::ZeroMemory( m_AnsBuf, sizeof(m_AnsBuf));

	// OS が XP以降か
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	GetVersionEx(&osvi);
	bIsWindowsXPorLater = 
	   ( (osvi.dwMajorVersion > 5) ||
		 (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1) );
}

CTkEdit::~CTkEdit()
{
}


// 摘要文字列セット
// InitCtrl() より前に 呼ぶ
//		int sel1	
//		int sel2	
//
BOOL CTkEdit::OnInitial( int pos, char *tky, int tkyLen, int sel1, int sel2 )
{
int lng = strlen( tky );

	if( !lng )
		return FALSE;

	m_DICTKY.startSel	= sel1;
	m_DICTKY.endSel		= sel2;

	// 文字列
	strcpy_s( m_DICTKY.tky, sizeof m_DICTKY.tky, tky );

	// 開始ポジション（０～）
	if( sel1 < sel2 ) {	//文字列選択時
		if( sel1 < lng )
			m_DICTKY.Def_nStart = bytecnt_scnt( sel1, m_DICTKY.tky );
		else
			m_DICTKY.Def_nStart = 0;
	}
	else {
		if( pos < lng )
			m_DICTKY.Def_nStart = bytecnt_scnt( pos, m_DICTKY.tky );
		else
			m_DICTKY.Def_nStart = 0;
	}

	int nDIC_Max;
	nDIC_Max = tkyLen;

	// 最大文字数
	if( lng < nDIC_Max/*_DIC_MAX*/ )
		m_DIC_MAX = lng;
	else
		m_DIC_MAX = nDIC_Max/*_DIC_MAX*/;

	return TRUE;
}


// コントロールの初期化
//  ダイアログでは OnInitDialog() で CALL
//		CWnd* pwnd .... 親ウィンドウ
//		UINT* btnID ... ボタンコントロールID
//					    [0] + , [1] - [2] ←, [3] →, [4] Home
//		UINT dspID .... 文字数表示コントロールID
//
BOOL CTkEdit::InitCtrl( CWnd* pwnd, UINT* btnID, UINT dspID )
{
	CEdit::SetReadOnly( TRUE );	// Ｒｅａｄ Ｏｎｌｙ
	CEdit::SetWindowText( m_DICTKY.tky );

	// 文字数を取得( byte 数ではない )
	m_DICTKY.length = CEdit::LineLength();
//	m_DICTKY.length = strlen( m_DICTKY.tky );
	
	int sb, eb, tmp, endpos;

	if( m_DICTKY.startSel < m_DICTKY.endSel ) {
		sb = m_DICTKY.startSel;
		// 文字列として正しいバイト数
		int cnt = (m_DICTKY.endSel - m_DICTKY.startSel);
		if( cnt > m_DIC_MAX )	cnt = m_DIC_MAX;
	
		tmp = get_strcnt( &m_DICTKY.tky[m_DICTKY.startSel], cnt );
		eb = sb + tmp;
		endpos = bytecnt_scnt( eb, m_DICTKY.tky );

		if( endpos > m_DICTKY.length )	endpos = m_DICTKY.length;

		m_DICTKY.nStart = bytecnt_scnt( m_DICTKY.startSel, m_DICTKY.tky );
		m_DICTKY.nEnd	= endpos;

	}
	else {
		sb = scnt_bytecnt( m_DICTKY.Def_nStart, m_DICTKY.tky );
		// 文字列として正しいバイト数
		tmp = get_strcnt( &m_DICTKY.tky[sb], m_DIC_MAX );
		eb = sb + tmp;
		endpos = bytecnt_scnt( eb, m_DICTKY.tky );

		if( endpos > m_DICTKY.length )	endpos = m_DICTKY.length;

		m_DICTKY.nStart = m_DICTKY.Def_nStart;
		m_DICTKY.nEnd = endpos;

	}

	m_pParent = pwnd;
	memcpy( m_BtnID, btnID, sizeof(UINT)*TKBTN_MAX );

	m_DspID = dspID;

	CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd );
	indicator();

ASSERT( m_pParent != NULL );

	return TRUE;
}

// ← キーが押された
void CTkEdit::LeftFunc()
{
	int st, sps, eps, sb, eb, si, ei;
	int nStart, nEnd;

	sps = eps = 0;

	if( m_DICTKY.nStart > 0 )
	{
		sb = scnt_bytecnt( (m_DICTKY.nStart-1), m_DICTKY.tky );

		st = IsKindOfLetter( m_DICTKY.tky, sb );

		if( st == 1 ) {			// ASCII 文字
			sps = 1;
		}
		else if( st == 2 ) {	// MB文字の1ﾊﾞｲﾄ目
			sps = 2;
		}
		// 選択最終ライン文字のタイプ
		eb = scnt_bytecnt( (m_DICTKY.nEnd-1), m_DICTKY.tky );
		st = IsKindOfLetter( m_DICTKY.tky, eb );
		switch( st ) {
		case 1:		// ASCII 文字
			eps = 1;	break;
		case 2:		// MB文字2バイト目
			eps = 2;	break;
		}

		if( sps ) {
			sb = scnt_bytecnt( m_DICTKY.nStart, m_DICTKY.tky );
			eb = scnt_bytecnt( m_DICTKY.nEnd, m_DICTKY.tky );

			si = (sb - sps);
			ei = eb /*(eb - eps)*/;

			if( (ei-si) > m_DIC_MAX ) {
				int tmp = get_strcnt( &m_DICTKY.tky[si], m_DIC_MAX );
				ei = si + tmp;
			}

			nStart	= bytecnt_scnt( si, m_DICTKY.tky );
			nEnd	= bytecnt_scnt( ei, m_DICTKY.tky );

			CTkEdit::SetSel( nStart, nEnd );
			CTkEdit::GetSel( m_DICTKY.nStart, m_DICTKY.nEnd );

/*
			nStart = m_DICTKY.nStart - sps;
			if( m_DICTKY.nEnd >= m_DICTKY.length )	// 最終位置で左が押された
				nEnd = m_DICTKY.nEnd;
			else
				nEnd = m_DICTKY.nEnd - eps;

			sb = scnt_bytecnt( nStart, m_DICTKY.tky );
			eb = scnt_bytecnt( nEnd, m_DICTKY.tky );
			if( (eb - sb) > m_DIC_MAX ) {
				int tmp = get_strcnt( m_DICTKY.tky, m_DIC_MAX );
				eb = sb + tmp;

				nStart	= bytecnt_scnt( sb, m_DICTKY.tky );
				nEnd	= bytecnt_scnt( eb, m_DICTKY.tky );
			}

			CTkEdit::SetSel( nStart, nEnd );
			CTkEdit::GetSel( m_DICTKY.nStart, m_DICTKY.nEnd );
*/

		}
		
		indicator();
	}

	CEdit::SetFocus();
}

// → キーが押された
void CTkEdit::RightFunc()
{
	int /*st, */sps, eps, sb, eb;
	int nStart, nEnd;
	sps = eps = 1;

#ifdef DB_OLD_CLOSE
	st = IsKindOfLetter( m_DICTKY.tky, m_DICTKY.nStart );

	if( st == 2 ) {		// MB 文字 1ﾊﾞｲﾄ目
		if( m_DICTKY.nStart + 2 < m_DICTKY.length )
			sps = 2;

	}
	else if( st == 1 ) {	// ASCII文字
		if( (m_DICTKY.nStart+1) < m_DICTKY.length )
			sps = 1;
	}

	// 選択最終ライン文字のタイプ
	if( m_DICTKY.nEnd < m_DICTKY.length ) {
		st = IsKindOfLetter( m_DICTKY.tky, m_DICTKY.nEnd );
		switch( st ) {
		case 1:		// ASCII 文字
			eps = 1;	break;
		case 2:		// MB文字１バイト目
			eps = 2;	break;
		}
	}
#endif

	if( sps ) {
		nStart = m_DICTKY.nStart + sps;
		nEnd = m_DICTKY.nEnd + eps;
		// 最後の文字は、必ず選択するように
		if( nStart == m_DICTKY.length ) {
			nStart = m_DICTKY.nStart;
			nEnd = m_DICTKY.nEnd;
		}

		sb = scnt_bytecnt( nStart, m_DICTKY.tky );
		eb = scnt_bytecnt( nEnd, m_DICTKY.tky );

		if( (eb-sb) > m_DIC_MAX ) {
			int tmp = get_strcnt( &m_DICTKY.tky[sb], m_DIC_MAX );
			eb = sb + tmp;
			nEnd = bytecnt_scnt( eb, m_DICTKY.tky );
		}
		CTkEdit::SetSel( nStart, nEnd );
		CTkEdit::GetSel( m_DICTKY.nStart, m_DICTKY.nEnd );
	}

	indicator();
	CEdit::SetFocus();
}

// ＋ が押された
void CTkEdit::AddFunc()
{
	int st, sb, eb;

	sb = scnt_bytecnt( m_DICTKY.nStart, m_DICTKY.tky );
	eb = scnt_bytecnt( m_DICTKY.nEnd, m_DICTKY.tky );

	if( (eb - sb) < m_DIC_MAX ) {

		st = IsKindOfLetter( m_DICTKY.tky, eb );

		if( st == 2 ) {		// MB 文字 1ﾊﾞｲﾄ目
			if( (eb + 2 - sb) <= m_DIC_MAX )
			{
				m_DICTKY.nEnd = bytecnt_scnt( eb+2, m_DICTKY.tky );

				CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd );
			}
		}
		else if( st == 1 ) {	// ASCII文字
			if( (eb + 1 - sb) <= m_DIC_MAX  )
			{
				m_DICTKY.nEnd = bytecnt_scnt( eb+1, m_DICTKY.tky );

				CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd );
			}
		}

		indicator();
	}

	CEdit::SetFocus();
}


// － が押された
void CTkEdit::SubFunc()
{
	int /*st,*/ start, end;

	CTkEdit::GetSel(start, end);

	if( m_DICTKY.nStart < m_DICTKY.nEnd - 1 ) {

		m_DICTKY.nEnd = end;

#ifdef DB_OLD_CLOSE
		st = IsKindOfLetter( m_DICTKY.tky, m_DICTKY.nEnd -1 );

		if( st == 3 ) {		// MB 文字 1ﾊﾞｲﾄ目
			if( m_DICTKY.nStart < m_DICTKY.nEnd - 2 ) {
				if( IsKindOfLetter( m_DICTKY.tky, m_DICTKY.nEnd -2 ) == 2 )
				{
					CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd -=2 );
				}
			}
		}
		else if( st == 1 ) {	// ASCII文字
			CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd -=1 );
		}
#endif

		CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd -=1 );

		indicator();
	}
	
	CEdit::SetFocus();
	
}

// Home が押された
void CTkEdit::HomeFunc()
{
	CEdit *pedit = (CEdit*)GetDlgItem( IDC_EDIT1 );
	int sb, eb;
	int tmp, endpos;

	m_DICTKY.nStart = m_DICTKY.Def_nStart;

	if( m_DICTKY.startSel < m_DICTKY.endSel ) {
		sb = m_DICTKY.startSel;
		// 文字列として正しいバイト数
		int cnt = (m_DICTKY.endSel - m_DICTKY.startSel);
		if( cnt > m_DIC_MAX )	cnt = m_DIC_MAX;
	
		tmp = get_strcnt( &m_DICTKY.tky[m_DICTKY.startSel], cnt );
		eb = sb + tmp;
		endpos = bytecnt_scnt( eb, m_DICTKY.tky );

	}
	else {
		sb = scnt_bytecnt( m_DICTKY.nStart, m_DICTKY.tky );
		tmp = get_strcnt( &m_DICTKY.tky[sb], m_DIC_MAX );

		eb = sb + tmp;
		endpos =  bytecnt_scnt( eb, m_DICTKY.tky );
	}

	if( endpos > m_DICTKY.length )	endpos = m_DICTKY.length;
	m_DICTKY.nEnd	= endpos;

	CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd );

	indicator();

	CEdit::SetFocus();
}

// シフトチェック
BOOL CTkEdit::isSHIFT()
{
	return( GetKeyState( VK_SHIFT ) < 0 );
}

//
// 選択文字数表示
void CTkEdit::indicator()
{
CEdit* pedit = (CEdit*)GetDlgItem( IDC_EDIT1 );
int nStart, nEnd, zen, han, st, sb, eb;
char buf[30];

	CTkEdit::GetSel( nStart, nEnd );

	sb = scnt_bytecnt( nStart, m_DICTKY.tky );
	eb = scnt_bytecnt( nEnd, m_DICTKY.tky );
	
	for( zen = han = 0; sb < eb;  ) {
		st = IsKindOfLetter( m_DICTKY.tky, sb );
		switch( st ) {
		case 1:		// ASCII文字
			sb++;
			han++;		break;
		case 2:		// MB文字 1バイト目
			sb += 2;
			zen++;		break;
		default:
			sb = eb;	// ループを抜けるため(それ以外)
			break;
		}
	}
	sprintf_s( buf, sizeof buf,  "全)%2d 半)%2d 文字", zen, han );

	if( m_DspID ) {
		if( m_pParent ) {
			m_pParent->GetDlgItem( m_DspID )->SetWindowText( buf );
		}
	}
}


BEGIN_MESSAGE_MAP(CTkEdit, CEdit)
	//{{AFX_MSG_MAP(CTkEdit)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTkEdit メッセージ ハンドラ

void CTkEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	m_bFocus = FALSE;
	if( m_pParent && m_DspID ) {
		CWnd* pwnd;
		pwnd = m_pParent->GetDlgItem(m_DspID);
		pwnd->Invalidate();
		pwnd->UpdateWindow();
	}
}


void CTkEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	m_bFocus = TRUE;
	if( m_pParent && m_DspID ) {
		CWnd* pwnd;
		pwnd = m_pParent->GetDlgItem(m_DspID);
		pwnd->Invalidate();
		pwnd->UpdateWindow();
	}

	CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd );
/*
	int nStart, nEnd;
	CTkEdit::GetSel( nStart, nEnd );
TRACE( "CTkEdit::OnSetFocus %d, %d\n", nStart, nEnd );
*/
}


void CTkEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( nChar )
	{
	case VK_ADD:	// + keyin
		BTN_STATE( m_BtnID[2], 1 );
		AddFunc();
		return;
	case VK_SUBTRACT:	// - keyin
	case VK_BACK:
		BTN_STATE( m_BtnID[3], 1 );
		SubFunc();
		return;
	case VK_LEFT:	// ← keyin
		if( !isSHIFT() )
		{
			BTN_STATE( m_BtnID[0], 1 );
			LeftFunc();
		}
		else
		{
			BTN_STATE( m_BtnID[3], 1 );	// shift が押されていたら - keyin
			SubFunc();
		}
		return;
	case VK_RIGHT:	// → keyin
		if( !isSHIFT() )
		{
			BTN_STATE( m_BtnID[1], 1 );
			RightFunc();
		}
		else
		{
			BTN_STATE( m_BtnID[2], 1 );	// shift が押されていたら + keyin
			AddFunc();
		}
		return;
	case VK_HOME:	// Home keyin
		BTN_STATE( m_BtnID[4], 1 );
		HomeFunc();
		return;
//	case VK_TAB:
//	case VK_END:
//		return 0;
	case VK_UP:
		return ;
	case VK_DOWN:
		if( m_pParent )
			((CDialog*)m_pParent)->NextDlgCtrl();
		return;
	default:
		break;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CTkEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( nChar )
	{
	case VK_ADD:	// + keyin
		BTN_STATE( m_BtnID[2], 0 );
		return;
	case VK_SUBTRACT:	// - keyin
	case VK_BACK:
		BTN_STATE( m_BtnID[3], 0 );
		return;
	case VK_LEFT:	// ← keyin
		BTN_STATE( m_BtnID[0], 0 );	// left button
		BTN_STATE( m_BtnID[3], 0 );	// sub button
		return;
	case VK_RIGHT:	// → keyin
		BTN_STATE( m_BtnID[1], 0 );	// right button
		BTN_STATE( m_BtnID[2], 0 );	// add button
		return;
	case VK_HOME:	// Home keyin
		BTN_STATE( m_BtnID[4], 0 );
		return;
	}
	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CTkEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
int nStart, nEnd, st;
int	sb, eb, tmp;

	CTkEdit::GetSel( nStart, nEnd );
	sb = scnt_bytecnt( nStart, m_DICTKY.tky );
	eb = scnt_bytecnt( nEnd, m_DICTKY.tky );
	
	if( eb - sb > m_DIC_MAX ) {
		tmp = get_strcnt( &m_DICTKY.tky[sb], m_DIC_MAX );
		eb = sb + tmp;
	}
	else if( eb - sb <= 0 )
	{
		int lng;
		lng = strlen( m_DICTKY.tky );

		if( eb < lng ) {
			st = IsKindOfLetter( m_DICTKY.tky, eb );
			switch( st ) {
			case 1:	// ASCII
				eb += 1;	break;
			case 2:	// MB-1
				eb += 2;	break;
			}				
		}
		else {
			st = IsKindOfLetter( m_DICTKY.tky, eb-1 );
			switch( st ) {
			case 1:	// ASCII
				sb = eb - 1;	break;
			case 3:	// MB-2
				sb = eb - 2;	break;
			}
		}
	}

	nStart	= bytecnt_scnt( sb, m_DICTKY.tky );
	nEnd	= bytecnt_scnt( eb, m_DICTKY.tky );

	CTkEdit::SetSel( nStart, nEnd );
	CTkEdit::GetSel( m_DICTKY.nStart, m_DICTKY.nEnd );

	indicator();

	CEdit::OnLButtonUp(nFlags, point);
}


// 登録摘要取り出し
void CTkEdit::get_dictky( char *buf, int bufsiz )
{
int nStart, nEnd;

	CTkEdit::GetSel( nStart, nEnd );
//	memmove( m_AnsBuf, &m_DICTKY.tky[nStart], nEnd - nStart );

	int ln, sofs, eofs;
	ln = strlen( m_DICTKY.tky );

	sofs = scnt_bytecnt( nStart, m_DICTKY.tky );
	eofs = scnt_bytecnt( nEnd, m_DICTKY.tky );

	memmove( m_AnsBuf, &m_DICTKY.tky[sofs], eofs - sofs );

	m_AnsBuf[ eofs - sofs ] = '\0';

	strcpy_s( buf, bufsiz, m_AnsBuf );
}

// キーの調整
BOOL CTkEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( pMsg->message == WM_KEYDOWN ) {
		// Return -> Tab
		long key;
		key = pMsg->wParam;
		if( key == VK_TAB && is_SHIFT() )	key = VK_F2;

		switch( key ) {
		case VK_RETURN:
		case VK_TAB:
			if( m_pParent )
				((CDialog*)m_pParent)->NextDlgCtrl();

			return TRUE;
			break;
		case VK_F2:
			if( m_pParent )
				((CDialog*)m_pParent)->PrevDlgCtrl();

			return TRUE;
			break;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}


// 摘要 選択 の Update
void CTkEdit::UpdateSelect()
{
	CTkEdit::SetSel( m_DICTKY.nStart, m_DICTKY.nEnd );
	indicator();
}


// OS によって、文字数かバイト数かが異なるため、
// 基本は、文字数単位で処理している。
//
void CTkEdit::GetSel(int& nStartChar, int& nEndChar)
{
	int start, end;

	CEdit::GetSel( start, end );

	// XP 以前は バイト単位
	if( ! bIsWindowsXPorLater ) {
		nStartChar	= bytecnt_scnt( start, m_DICTKY.tky );
		nEndChar	= bytecnt_scnt( end, m_DICTKY.tky );
	}
	else {
		nStartChar	= start;
		nEndChar	= end;
	}
}


void CTkEdit::SetSel( int nStartChar, int nEndChar, BOOL bNoScroll /*= FALSE*/ )
{
	int start, end;

	// XP 以前は バイト単位
	if( ! bIsWindowsXPorLater ) {
		start	= scnt_bytecnt( nStartChar, m_DICTKY.tky );
		end		= scnt_bytecnt( nEndChar, m_DICTKY.tky );
	}
	else {
		start	= nStartChar;
		end		= nEndChar;
	}

	CEdit::SetSel( start, end, bNoScroll );
}
