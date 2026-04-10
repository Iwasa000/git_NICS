// MiddlePyment.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#include "MiddlePymentRec.h"
#include "MiddlePyment.h"


// メインビューの財務クラスを参照
//CDBSyzShinView *CMiddlePyment::pSyzShin;
CDBSyzShinMainView *CMiddlePyment::pSyzShin;

// CMiddlePyment ダイアログ

IMPLEMENT_DYNAMIC(CMiddlePyment, ICSDialog)

CMiddlePyment::CMiddlePyment(CWnd* pParent /*=NULL*/)
: ICSDialog(CMiddlePyment::IDD, pParent) , arith(0x16), IsRireki(0)
{
	memset( Sn_ZZURI, '\0', sizeof(Sn_ZZURI) );
	memset( Sn_ZZNOFZ, '\0', sizeof(Sn_ZZNOFZ) );
	memset( Sn_ZURI, '\0', sizeof(Sn_ZURI) );
	memset( Sn_ZNOFZ, '\0', sizeof(Sn_ZNOFZ) );
	memset( Sn_KazeiUriage, '\0', MONY_BUF_SIZE );

	m_isSaveKzUri = false;
}

CMiddlePyment::~CMiddlePyment()
{
}

void CMiddlePyment::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_MPeyment);
}


BEGIN_MESSAGE_MAP(CMiddlePyment, ICSDialog)
	ON_BN_CLICKED(IDCANCEL, &CMiddlePyment::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMiddlePyment::OnBnClickedOk)
	ON_COMMAND(ID_OLDTYUKAN, &CMiddlePyment::OnOldtyukan)
END_MESSAGE_MAP()


// CMiddlePyment メッセージ ハンドラ
void SetAttribMod(CIcsdiagctrl& diag,int index,COLORREF color, int mdfy)
{
	CDiagAttribute DA;

	diag.GetAttribute( index, DA );
	DA.attr_bcolor = color;
	diag.SetAttribute( index, DA ,TRUE);
	diag.ModifyItem( index, mdfy );
}


BOOL CMiddlePyment::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	int itm_cnt = 2;
	//int line_max = 14;
	int line_max = 15;

	for(int line = 0;line < line_max; line++ ){
		for(int i = 0;i < itm_cnt;i++){

			int index = ( itm_cnt * line ) + i;

			int up	  = index - itm_cnt;
			int	down  = index + itm_cnt;

			int	left  = index - 1;
			int	right = index + 1;

			if( i == 0 && line == 0 ) left = MOVE_PREV;
			if( line == 0 )			  up   = MOVE_PREV;
			if( line == line_max - 1) down = MOVE_NEXT;
			if( line == line_max - 1 && i == itm_cnt - 1 ) 
									 right = MOVE_NEXT;

			CRect rec = CRect(left, up, right, down);
			PositionMap.SetAt( index , rec );
		}
	}
	
	Init(FALSE);

	ReadNoufu();

	//Init(FALSE);

	if( CheckOldTyukan() ){
		CMenu*	mn = GetMenu();
		mn->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
		DrawMenuBar();
	}
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CMiddlePyment::CheckOldTyukan()
{
	if( !MPrec.size() )
		return TRUE;

	if( !MPrec[0].edit ) return TRUE;

	vector<MPAYMENTVAL> rec = pSyzShin->OldKinfoRead();

	int flg = FALSE;
	for(UINT i = 0; i < rec.size() && MPrec[i].edit ; i++ ){
		if( arith.l_test(rec[i].KMPval) || arith.l_test(rec[i].TMPval) ){
			flg = TRUE;
			break;
		}
	}

	return !flg;
}

BOOL CMiddlePyment::Init(int oldread)
{
	MPrec = pSyzShin->MiddlePaymentRead(oldread);

	CDiagData DD;

	//int stofs = 4;
	//int kofs = 37;
	//int tofs = 49;

	int stofs = 6;
	int kofs = 40;
	int tofs = 52;


	for(UINT i = 0;i<MPrec.size();i++){
		DD.data_disp.Format("%s",!MPrec[i].edit ? GetMapSkbnName()[MPrec[i].kubun] : "履歴なし");
		m_MPeyment.SetData( i + kofs, DD );

		DD.data_disp = CTimeControl::GetDateStrPair(TimePair(CTimeControl::GetCTime(MPrec[i].smd),CTimeControl::GetCTime(MPrec[i].emd)),"%2d.%2d.%2d");
		m_MPeyment.SetData( i + tofs, DD );

		memmove( DD.data_val, MPrec[i].KMPval,6 );
		m_MPeyment.SetData( i * 2 + stofs, DD );
		if(!MPrec[i].edit)
			SetAttribMod( m_MPeyment, i * 2 + stofs,RGB(242,242,242), DIAG_MDFY_READONLY );

		memmove( DD.data_val, MPrec[i].TMPval,6 );
		m_MPeyment.SetData( i* 2 + 1 + stofs, DD );
		if(!MPrec[i].edit)
			SetAttribMod( m_MPeyment, i* 2 + 1 + stofs,RGB(242,242,242), DIAG_MDFY_READONLY );
	}

	for(UINT i = (UINT)MPrec.size();i<11;i++){
		SetAttribMod( m_MPeyment, i * 2 + stofs,RGB(242,242,242), DIAG_MDFY_READONLY );
		SetAttribMod( m_MPeyment, i* 2 + 1 + stofs,RGB(242,242,242), DIAG_MDFY_READONLY );
	}

	recalq();
	
	if((sn_sign4 & 0x01) == 0x00){//入力ロックされていない場合のみフォーカスセット
		m_MPeyment.ForceSetPosition(0);

		GetDlgItem(IDC_ICSDISP8CTRL2)->ShowWindow(SW_HIDE);

	}
	else{
		//for(int i = 0 ; i < 26 ; i++){
		for(int i = 0 ; i < 28 ; i++){
			SetAttribMod( m_MPeyment, i , RGB(242,242,242) , DIAG_MDFY_READONLY );
		}

		GetDlgItem(IDC_ICSDISP8CTRL2)->ShowWindow(SW_SHOW);

	}

	CString str;
	GetWindowText(str.GetBuffer(128),128);
	str.ReleaseBuffer();
	if( IsRireki ) str += CString(" <<履歴選択中>>");
	SetWindowText(str);	

	long diagFlg = 0;
	if( !pSyzShin->IsInvoiceMaster() ) {
		// 課税期間における課税売上高を非表示
		long diagCode[2] = { 4, 35 };
		for( auto code : diagCode ) {
			SetAttribMod( m_MPeyment, code, RGB( 242, 242, 242 ), DIAG_MDFY_TRANSPARENT );
		}
	}
	else {
		if( !pSyzShin->IsInputKazeiUriage() ) {
			// 課税期間における課税売上高を入力不可
			long diagCode = 0;
			diagCode = 4;
			SetAttribMod( m_MPeyment, diagCode, RGB( 242, 242, 242 ), DIAG_MDFY_READONLY );
		}
	
		m_isSaveKzUri = true;
	}

	// 課税期間における課税売上高の消費税額欄は不要だが、処理が2列ある前提で書かれているため、ソース上で非表示にする
	SetAttribMod( m_MPeyment, 5, RGB( 242, 242, 242 ), DIAG_MDFY_TRANSPARENT );

	return 0;
}

BOOL CMiddlePyment::ReadNoufu()
{
	ASSERT( pSyzShin );
	
	CDiagData DD;

	char	tmpMony[MONY_BUF_SIZE] = {0};
	CString	tag, chtag;

	// 10%改正対応
	//tag = _T("AAF00000");
	if ( m_ActViewVer == ID_VER_SYZ31 ) {
		tag = _T("AAM00030");
	}
	else {
		tag = _T("AAF00000");
	}

#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CMiddlePyment::ReadNoufu()__tag:%s\n", tag);
	DbgViewTrace(hoge);
#endif

	pSyzShin->GetMony( tag, chtag, (char*)Sn_ZZURI, m_ActViewVer );
	memmove( DD.data_val, Sn_ZZURI, 6 );
	m_MPeyment.SetData( 0, DD );

	tag = _T("AAF00020");
	pSyzShin->GetMony( tag, chtag, (char*)Sn_ZZNOFZ, m_ActViewVer );
	memmove( DD.data_val, Sn_ZZNOFZ, 6 );
	m_MPeyment.SetData( 1, DD );

	tag = _T("AAF00010");
	pSyzShin->GetMony( tag, chtag, (char*)Sn_ZURI, m_ActViewVer );
	memmove( DD.data_val, Sn_ZURI, 6 );
	m_MPeyment.SetData( 2, DD );

	tag = _T("AAF00030");
	pSyzShin->GetMony( tag, chtag, (char*)Sn_ZNOFZ, m_ActViewVer );
	memmove( DD.data_val, Sn_ZNOFZ, 6 );
	memmove(DD.data_val, Sn_ZNOFZ, 6 );
	m_MPeyment.SetData( 3, DD );

	if( m_isSaveKzUri ) {
		// 課税期間における課税売上高
		pSyzShin->GetKazeiUriagedaka( ( char* )Sn_KazeiUriage );
		memmove( DD.data_val, Sn_KazeiUriage, 6 );
		m_MPeyment.SetData( 4, DD );
	}

	if((sn_sign4 & 0x01) == 0x00){//入力ロックされていない場合のみフォーカスセット
		m_MPeyment.SetPosition( 0 );
	}

	return 0;
}

BOOL CMiddlePyment::UpdateNoufu()
{
	int chsign = 0;

	CDiagData DD;
	m_MPeyment.GetData( 0, DD );
	memmove( Sn_ZZURI,DD.data_val,6 );
	m_MPeyment.GetData( 1, DD );
	memmove( Sn_ZZNOFZ,DD.data_val,6 );
	m_MPeyment.GetData( 2, DD );
	memmove( Sn_ZURI,DD.data_val,6 );
	m_MPeyment.GetData( 3, DD );
	if( arith.l_cmp( Sn_ZNOFZ , DD.data_val) != 0){
		chsign = 1;
	}
	memmove( Sn_ZNOFZ,DD.data_val,6 );

	// 課税期間における課税売上高
	m_MPeyment.GetData( 4, DD );
	memmove( Sn_KazeiUriage, DD.data_val, MONY_BUF_SIZE );

	CString	tag, chtag;
	// 10%改正対応
	//tag = _T("AAF00000");
	if ( m_ActViewVer == ID_VER_SYZ31 ) {
		tag = _T("AAM00030");
	}
	else {
		tag = _T("AAF00000");
	}

#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CMiddlePyment::UpdateNoufu()__tag:%s\n", tag);
	DbgViewTrace(hoge);
#endif

	pSyzShin->SetMony( tag, chtag, (char*)Sn_ZZURI, m_ActViewVer );
	tag = _T("AAF00020");
	pSyzShin->SetMony( tag, chtag, (char*)Sn_ZZNOFZ, m_ActViewVer );
	tag = _T("AAF00010");
	pSyzShin->SetMony( tag, chtag, (char*)Sn_ZURI, m_ActViewVer );

	tag = _T("AAF00030");
	chtag = ( chsign ? "1" : "0" );
	pSyzShin->SetMony( tag, chtag, (char*)Sn_ZNOFZ, m_ActViewVer );

	// 課税期間における課税売上高
	if( m_isSaveKzUri ) {
		tag = pSyzShin->GetKazeiUriageCode();
		//if( !m_isSaveKzUri ) {
		//	memset( Sn_KazeiUriage, '\0', MONY_BUF_SIZE );
		//}
		pSyzShin->SetMony( tag, chtag, ( char* )Sn_KazeiUriage, m_ActViewVer );
	}

	if( pSyzShin->WriteNofu() ){
		return -1;
	}

	return 0;
}

BOOL CMiddlePyment::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN )	{
		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 )	return TRUE;

		if( pMsg->wParam == VK_F5 ){
			OnBnClickedOk();
			return TRUE;
		}
		if( pMsg->wParam == VK_ESCAPE ){
			OnBnClickedCancel();
			return TRUE;
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

void CMiddlePyment::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

void CMiddlePyment::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( UpdateNoufu() ){
		OnCancel();
	}

	CDiagData DD;
	//int stofs = 4;
	int stofs = 6;
	for(UINT i = 0;i<MPrec.size();i++){
		m_MPeyment.GetData( i * 2 + stofs, DD );
		memmove(MPrec[i].KMPval, DD.data_val, 6 );
		m_MPeyment.GetData( i* 2 + 1 + stofs, DD );
		memmove(MPrec[i].TMPval, DD.data_val, 6 );
	}
	pSyzShin->ShinKinfoWrite(MPrec);

	pSyzShin->CheckOver500Million();

	OnOK();
}

BEGIN_EVENTSINK_MAP(CMiddlePyment, ICSDialog)
	ON_EVENT(CMiddlePyment, IDC_ICSDIAG8CTRL1, 1, CMiddlePyment::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CMiddlePyment, IDC_ICSDIAG8CTRL1, 2, CMiddlePyment::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CMiddlePyment, IDC_ICSDIAG8CTRL1, 3, CMiddlePyment::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CMiddlePyment::EditONIcsdiag8ctrl1(short index)
{
}

void CMiddlePyment::EditOFFIcsdiag8ctrl1(short index)
{
}

int CMiddlePyment::SerchNextPos(int& index,RECT& move,LONG RECT::*ptr)
{
	CDiagAttribute DA;
	int st = TRUE;

	do{
		if(	st = PositionMap.Lookup(index,move) ) {
			if( move.*ptr < 0 ){ st = 0; break; }
			m_MPeyment.GetAttribute( (short)(move.*ptr), DA );
			index = move.*ptr;
			if( DA.attr_bcolor == RGB(242,242,242) ) continue;
			return 1;
			break;
		}
	}while(st);

	return 0;
}

short CheckInputKey(short nChar)
{
	if( !( nChar==VK_DOWN || nChar==VK_UP || nChar==VK_RIGHT ||
		   nChar==VK_LEFT || nChar==VK_RETURN || nChar==VK_TAB ||
		   nChar==VK_F2   || nChar==VK_F3 ) ) return 0;

	short key = nChar;

	int shift = ::GetKeyState(VK_SHIFT)&0x80;
	if( nChar == VK_F3 || nChar == VK_RETURN || ( nChar == VK_TAB && !shift ) )
		key = VK_RIGHT;
	if( nChar == VK_F2 || nChar == VK_TAB && shift )
		key = VK_LEFT;

	return key;
}

int CMiddlePyment::IsEditable(int index)
{
	CDiagAttribute DA;
	m_MPeyment.GetAttribute( index, DA );
	return DA.attr_inpkind == 0;
}

int CMiddlePyment::Termination(int index,short nChar)
{
	if(nChar == VK_DELETE){
		if(IsEditable(index)) m_MPeyment.DataClear(index,TRUE);
		return 0;
	}

	short key = CheckInputKey(nChar);
	if( !key ) return 0;

	int next = index;
	LONG RECT::*ptr;

	switch(key){
	case VK_UP:	   ptr = &RECT::top;    break;
	case VK_DOWN:  ptr = &RECT::bottom; break;
	case VK_LEFT:  ptr = &RECT::left;   break;
	case VK_RIGHT: ptr = &RECT::right;  break;
	}

	RECT move;
	if( SerchNextPos(next,move,ptr) )
		SetPosition(move,ptr);

	return 1;
}

void CMiddlePyment::SetPosition(CRect move,LONG RECT::*ptr)
{
	CDiagAttribute DA;
	int index = move.*ptr;
	m_MPeyment.GetAttribute(index,DA );

	if( DA.attr_bcolor ==  RGB(242,242,242) ){
		RECT move;
		if( !SerchNextPos(index,move,ptr) ) return;
	}
	m_MPeyment.ForceSetPosition(index);
}

void CMiddlePyment::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	Termination( index, nChar);
	recalq();
}

void CMiddlePyment::recalq()
{
	//int stofs = 4;
	int stofs = 6;
	int ttofs = 36;

	char KMPvalTotal[6] = {};
	char TMPvalTotal[6] = {};

	for(UINT i = 0;i<MPrec.size();i++){
		CDiagData DD;
		m_MPeyment.GetData( i * 2 + stofs, DD );
		arith.l_add(KMPvalTotal,KMPvalTotal,DD.data_val);

		m_MPeyment.GetData( i* 2 + 1 + stofs, DD );
		arith.l_add(TMPvalTotal,TMPvalTotal,DD.data_val);
	}

	CDiagData DD;
	memmove( DD.data_val, KMPvalTotal,6 );
	//m_MPeyment.SetData( 26, DD );
	m_MPeyment.SetData( 28, DD );
	memmove( DD.data_val, TMPvalTotal,6 );
	//m_MPeyment.SetData( 27, DD );
	m_MPeyment.SetData( 29, DD );
}

void CMiddlePyment::OnOldtyukan()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。

	CString mes;
	mes += "バージョンアップ前の中間納付額を履歴なし欄に取込みます。\n";
	mes += "\n";
	mes += "処理状況により、実際の納税額と異なる場合がありますので、\n";
	mes += "再取り込み後の中間納税額は必ずご確認ください。\n";

	ICSMessageBox(mes);
	Init(TRUE);
}