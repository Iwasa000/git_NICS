#include "stdafx.h"
#include "ExEdit.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtTab
CExEdit::CExEdit()
{
	hIMC = NULL;

	Gyo_Max = 3;
	LineByte_Max = 10;
	str_z_edit = "";
	fst_kind = ZEN_ST;
}

CExEdit::~CExEdit()
{
}

void CExEdit::Make_Edit(int gyo,int line_byte,CString str,int kind)
{
	Gyo_Max = gyo;
	LineByte_Max = line_byte;
	str_z_edit = str;
	fst_kind = kind;
}

BEGIN_MESSAGE_MAP(CExEdit, CEdit)
	//{{AFX_MSG_MAP(CExtTab)
	//ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(EN_CHANGE, &CExEdit::OnEnChange)
	ON_WM_DRAWCLIPBOARD()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CExEdit::OnEnSetfocus)
//	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// キャプション描画
void CExEdit::DrawCaption(CDC *pDC, CRect rect,const COLORREF color,const CString& str)
{

}

BOOL CExEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	//if( Editsgn == 0 )	return( FALSE );

	if ( pMsg->message == WM_KEYDOWN) {
		switch ( pMsg->wParam ) {
				case VK_RETURN: 
					if(GetLineCount() >= Gyo_Max){//最大行数以上のエンターは不可
						return TRUE;
					}
					break;
				case VK_PROCESSKEY://ctr + z 用の保存
					GetWindowTextA(str_z_edit.GetBuffer(EX_EDIT_MAX),EX_EDIT_MAX);
					break;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CExEdit::OnEnChange()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CEdit::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。

	//// TODO:  ここにコントロール通知ハンドラ コードを追加してください。
	CString strText;
	int	start, end;

	CString strText_Set = "";
	CString strText_Set2 = "";
	int i,len, nLineCount = GetLineCount();

	CString lp_str = "";
	char moji[EX_EDIT_MAX];
	memset( moji, '\0', sizeof moji );

	GetWindowTextA(lp_str.GetBuffer(EX_EDIT_MAX),EX_EDIT_MAX);
	
	_tcscpy_s(moji,lp_str);

	len = (int)strlen(lp_str);

	int pos_pl = 0;		//改行のコードを\rから\r\nに変更しているので、カーソル位置もその分ずらす用
	int ent_cnt = 0;	//改行数
	int moji_cnt = 0;	//一行の文字数
	int ent_del_flg = 0;	//書かれている文字に対して、追加で文字を記入し、行最大文字数を超えた場合、次の改行を消す為のフラグ

	GetSel(start,end);


	for(i = 0;i < EX_EDIT_MAX;i++){
		if(moji[i] == '\0'){break;}

		//IsKindOfLetter
		//１：ASCII文字
		//２：マルチバイト文字の第一バイト
		//３：マルチバイト文字の第二バイト
		//０：上記以外
		if( (IsKindOfLetter(moji,i) == 3) || (IsKindOfLetter(moji,i) == 1)){//カーソルが入れる場所の時のみ
			if(moji_cnt < LineByte_Max){//この後はかならず文字は来ないので、今見ている文字は次ループで数える事になる
				start--;
				if(start < 0){start = 0;}
			}
		}


		if(moji[i] == '\r' && moji[i + 1] != '\n' ){
			moji_cnt = 0;

			ent_cnt++;
			if(ent_cnt >= Gyo_Max){moji[i] = '\0';break;}//MAX行まで

			//\n入れる分ずらす
			memmove(&moji[i + 2],&moji[i + 1],EX_EDIT_MAX - (i + 2));
			//\r を　\r\nと変換しなければ改行されない
			moji[i++] = '\r';moji[i] = '\n';

			pos_pl += 1;
		}else if(moji[i] == '\r' || moji[i] == '\n'  ){
			if(moji[i] == '\r' && moji[i + 1] == '\n' ){//\rではなく、\r\nがきている時は何もしない=普通の改行処理

				if(ent_del_flg == 1){

					memmove(&moji[i],&moji[i + 2],EX_EDIT_MAX - (i + 2));
					ent_del_flg = 0;
					i--;

					if(start > 0){	pos_pl -= 2;}

				}else{
					start -= 2;
					if(start < 0){start = 0;}
					moji_cnt = 0;
					ent_cnt++;
					if(ent_cnt >= Gyo_Max){moji[i] = '\0';break;}//MAX行まで
				}
			}
		}else{
			moji_cnt++;
			
			if(moji_cnt > LineByte_Max || (moji_cnt == LineByte_Max && IsKindOfLetter(moji,i) == 2)){//強制的に改行（一行の最大文字数）
				//新たに\r\nを入れる為、２つずらす（MAX行の場合はそこで終了＝\0）
				memmove(&moji[i + 2],&moji[i],EX_EDIT_MAX - (i + 2));

				ent_cnt++;
				if(ent_cnt >= Gyo_Max){moji[i] = '\0';break;}//MAX行まで

				//\r を　\r\nと変換しなければ改行されない
				moji[i++] = '\r';moji[i] = '\n';

				moji_cnt = 0;
				if(start > 0){pos_pl += 2;}
				ent_del_flg = 1;
			}
		}
	}
	
	strText_Set.Format("%s",moji);
		
	GetSel(start,end);

	start = start + pos_pl;
	end = end + pos_pl;

	SetWindowTextA(strText_Set);
	SetSel(start,end);
}

void CExEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CString str;
	int	start, end,i;
	char moji[EX_EDIT_MAX];
	memset( moji, '\0', sizeof moji );

	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( nChar == 0x1a ) {	//ctrl+z
		GetWindowTextA(str.GetBuffer(EX_EDIT_MAX),EX_EDIT_MAX);
		SetWindowTextA(str_z_edit);
		str_z_edit = str;
		SetSel(0,-1);//テキスト全選択
	}

	if( nChar == 0x01 ) {	//ctrl+a
		SetSel(0,-1);//テキスト全選択
	}

	if( nChar == VK_BACK ) {
		
		GetWindowTextA(str.GetBuffer(EX_EDIT_MAX),EX_EDIT_MAX);
		GetSel(start,end);
		_tcscpy_s(moji,str);
		if(start > 2 && start == end){//改行直後のカーソル位置の場合のバックスペース

			for(i = 0;i < EX_EDIT_MAX;i++){
				if(moji[i] == '\0'){break;}

				if( (IsKindOfLetter(moji,i) == 3) || (IsKindOfLetter(moji,i) == 1)){
					start--;
					if(start < 0){start = 0;}
				}
			}

			if(moji[i - 1] == '\n'){

				if(moji[i] == '\0'){//文字がその後ろにない場合は改行を消す
					memmove(&moji[i - 2],&moji[i],EX_EDIT_MAX - i);
					str.Format("%s",moji);
					SetWindowTextA(str);
				}
				SetSel(i - 2,i - 2);//文字が後ろにある場合は何も消さず、カーソル位置のみ変更
				return;
			}
		}

	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CExEdit::OnEnSetfocus()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	if(fst_kind == ZEN_ST){
		if( !hIMC ) {
			hIMC = ImmGetContext( m_hWnd );
			if( !hIMC )	return;
		}

		ImmSetOpenStatus( hIMC, TRUE );
		DWORD	conv, sent;
		ImmGetConversionStatus( hIMC, &conv, &sent );
		if( conv & IME_CMODE_ROMAN )
			conv	=	(IME_CMODE_JAPANESE | IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE);
		else
			conv	=	(IME_CMODE_JAPANESE | IME_CMODE_FULLSHAPE);
		ImmSetConversionStatus( hIMC, conv, IME_SMODE_AUTOMATIC );
	}
}

/*
void CExEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	HIMC	hlmc;
	hlmc = ImmGetContext(GetSafeHwnd());

	//IMEのクローズ
	if( hlmc ) {
		ImmSetOpenStatus( hlmc, FALSE );
		ImmReleaseContext( m_hWnd, hlmc );
	}
}
*/

void CExEdit::Killfocus()
{
	if( hIMC ) {
		::ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
		ImmSetOpenStatus( hIMC, FALSE );
		ImmReleaseContext( m_hWnd, hIMC );
		hIMC = NULL;
	}
}

int	CExEdit::GetLineOfs()
{
	int nIndex = LineIndex( -1 );

	int ln;
	for( ln =0; ln < Gyo_Max; ln++ ) {
		if( LineIndex ( ln ) == nIndex ) break;
	}
	return ln;
}

void CExEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	BOOL	CurPos = FALSE;
	int		lofs = GetLineOfs();

	if(( nChar == VK_UP ) || ( nChar == VK_DOWN )) {

		switch( nChar ) {

			case	VK_UP:
							if( !lofs ) CurPos = TRUE;
							else	break;
			case	VK_DOWN:
							if( CurPos != TRUE && ((lofs+1) == Gyo_Max) ) {
								CurPos = FALSE;
							}
							else if( CurPos == FALSE ) break;
			default:
							CWnd *pWnd;//, *p;
							pWnd = GetParent();
//							pWnd->PostMessage( WM_USER_SETINPUT, nChar );

//							p = pWnd->GetNextDlgTabItem( this, CurPos );
//							((CDialog *)pWnd)->GotoDlgCtrl( p );
							return;
		}
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
