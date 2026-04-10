// ShinFhyo50.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo50.h"
#include "ShinFhyo50Idx.h"

#define	SIROIROCOLOR	PALETTERGB( 255, 251, 240 )

// CShinFhyo50 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo50, CSyzBaseDlg)

CShinFhyo50::CShinFhyo50(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo50::IDD, pParent)
	, m_typeCnt(0)
	, m_pParent(pParent)
	, m_FrSw(0)
	, m_DpSw(0)
	, m_Tmp(0)
	, m_pZmSub(NULL)
{
	memset( F51T, '\0', sizeof(F51T) );
	memset( F51A, '\0', sizeof(F51A) );
	memset( F51B, '\0', sizeof(F51B) );
	memset( F51C, '\0', sizeof(F51C) );

	memset( F52T, '\0', sizeof(F52T) );
	memset( F52A, '\0', sizeof(F52A) );
	memset( F52B, '\0', sizeof(F52B) );
	memset( F52C, '\0', sizeof(F52C) );

	memset( F53T, '\0', sizeof(F53T) );
	memset( F53A, '\0', sizeof(F53A) );
	memset( F53B, '\0', sizeof(F53B) );
	memset( F53C, '\0', sizeof(F53C) );

	memset( F54T, '\0', sizeof(F54T) );
	memset( F54A, '\0', sizeof(F54A) );
	memset( F54B, '\0', sizeof(F54B) );
	memset( F54C, '\0', sizeof(F54C) );
	memset( F54S, '\0', sizeof(F54S) );

	memset( F55T, '\0', sizeof(F55T) );
	memset( F55A, '\0', sizeof(F55A) );
	memset( F55B, '\0', sizeof(F55B) );
	memset( F55C, '\0', sizeof(F55C) );

	memset( F56T, '\0', sizeof(F56T) );
	memset( F56A, '\0', sizeof(F56A) );
	memset( F56B, '\0', sizeof(F56B) );
	memset( F56C, '\0', sizeof(F56C) );
	memset( F56S, '\0', sizeof(F56S) );
	memset( F56SS, '\0', sizeof(F56SS) );

	memset( F57T, '\0', sizeof(F57T) );
	memset( F57A, '\0', sizeof(F57A) );
	memset( F57B, '\0', sizeof(F57B) );
	memset( F57C, '\0', sizeof(F57C) );

	memset( F58T, '\0', sizeof(F58T) );
	memset( F58A, '\0', sizeof(F58A) );
	memset( F58B, '\0', sizeof(F58B) );
	memset( F58C, '\0', sizeof(F58C) );

	memset( F59T, '\0', sizeof(F59T) );
	memset( F59A, '\0', sizeof(F59A) );
	memset( F59B, '\0', sizeof(F59B) );
	memset( F59C, '\0', sizeof(F59C) );

	memset( F510T, '\0', sizeof(F510T) );
	memset( F510A, '\0', sizeof(F510A) );
	memset( F510B, '\0', sizeof(F510B) );
	memset( F510C, '\0', sizeof(F510C) );

	memset( F511T, '\0', sizeof(F511T) );
	memset( F511A, '\0', sizeof(F511A) );
	memset( F511B, '\0', sizeof(F511B) );
	memset( F511C, '\0', sizeof(F511C) );

	memset( F512T, '\0', sizeof(F512T) );
	memset( F512A, '\0', sizeof(F512A) );
	memset( F512B, '\0', sizeof(F512B) );
	memset( F512C, '\0', sizeof(F512C) );

	memset( F513T, '\0', sizeof(F513T) );
	memset( F513A, '\0', sizeof(F513A) );
	memset( F513B, '\0', sizeof(F513B) );
	memset( F513C, '\0', sizeof(F513C) );

	memset( F514T, '\0', sizeof(F514T) );
	memset( F514A, '\0', sizeof(F514A) );
	memset( F514B, '\0', sizeof(F514B) );
	memset( F514C, '\0', sizeof(F514C) );

	memset( F515T, '\0', sizeof(F515T) );
	memset( F515A, '\0', sizeof(F515A) );
	memset( F515B, '\0', sizeof(F515B) );
	memset( F515C, '\0', sizeof(F515C) );

	memset( F516T, '\0', sizeof(F516T) );
	memset( F516A, '\0', sizeof(F516A) );
	memset( F516B, '\0', sizeof(F516B) );
	memset( F516C, '\0', sizeof(F516C) );

	memset( F517T, '\0', sizeof(F517T) );
	memset( F517A, '\0', sizeof(F517A) );
	memset( F517B, '\0', sizeof(F517B) );
	memset( F517C, '\0', sizeof(F517C) );

	memset( F518T, '\0', sizeof(F518T) );
	memset( F518A, '\0', sizeof(F518A) );
	memset( F518B, '\0', sizeof(F518B) );
	memset( F518C, '\0', sizeof(F518C) );

	memset( F519T, '\0', sizeof(F519T) );
	memset( F519A, '\0', sizeof(F519A) );
	memset( F519B, '\0', sizeof(F519B) );
	memset( F519C, '\0', sizeof(F519C) );

	memset( F520T, '\0', sizeof(F520T) );
	memset( F520A, '\0', sizeof(F520A) );
	memset( F520B, '\0', sizeof(F520B) );
	memset( F520C, '\0', sizeof(F520C) );

	memset( F521T, '\0', sizeof(F521T) );
	memset( F521A, '\0', sizeof(F521A) );
	memset( F521B, '\0', sizeof(F521B) );
	memset( F521C, '\0', sizeof(F521C) );

	memset( F522T, '\0', sizeof(F522T) );
	memset( F522A, '\0', sizeof(F522A) );
	memset( F522B, '\0', sizeof(F522B) );
	memset( F522C, '\0', sizeof(F522C) );

	memset( F523T, '\0', sizeof(F523T) );
	memset( F523A, '\0', sizeof(F523A) );
	memset( F523B, '\0', sizeof(F523B) );
	memset( F523C, '\0', sizeof(F523C) );

	memset( F524T, '\0', sizeof(F524T) );
	memset( F524A, '\0', sizeof(F524A) );
	memset( F524B, '\0', sizeof(F524B) );
	memset( F524C, '\0', sizeof(F524C) );

	memset( F525T, '\0', sizeof(F525T) );
	memset( F525A, '\0', sizeof(F525A) );
	memset( F525B, '\0', sizeof(F525B) );
	memset( F525C, '\0', sizeof(F525C) );

	memset( F526T, '\0', sizeof(F526T) );
	memset( F526A, '\0', sizeof(F526A) );
	memset( F526B, '\0', sizeof(F526B) );
	memset( F526C, '\0', sizeof(F526C) );

	memset( F527T, '\0', sizeof(F527T) );
	memset( F527A, '\0', sizeof(F527A) );
	memset( F527B, '\0', sizeof(F527B) );
	memset( F527C, '\0', sizeof(F527C) );

	memset( F528T, '\0', sizeof(F528T) );
	memset( F528A, '\0', sizeof(F528A) );
	memset( F528B, '\0', sizeof(F528B) );
	memset( F528C, '\0', sizeof(F528C) );

	memset( F529T, '\0', sizeof(F529T) );
	memset( F529A, '\0', sizeof(F529A) );
	memset( F529B, '\0', sizeof(F529B) );
	memset( F529C, '\0', sizeof(F529C) );

	memset( F530T, '\0', sizeof(F530T) );
	memset( F530A, '\0', sizeof(F530A) );
	memset( F530B, '\0', sizeof(F530B) );
	memset( F530C, '\0', sizeof(F530C) );

	F51UPb = F52UPb = F53UPb = F54UPb = F55UPb = 0;
	SZsw = SBsw = CQsw = 0x00;

	F50c = F51c = F52c = F53c = F54c = F55c;
	F518sw = F519sw = F520sw = F521sw = F522sw = F523sw = F524sw = F525sw = F526sw = F527sw = F528sw = F529sw = 0;

	F56sw = 0;
	F512sw = F513sw = F514sw = F515sw = F516sw = F517sw = 0;

	m_curwnd = -1;
}

CShinFhyo50::~CShinFhyo50()
{
}

void CShinFhyo50::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo50Diag01);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo50Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo50, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFhyo50 メッセージ ハンドラ

BOOL CShinFhyo50::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo50Diag02;
	m_BottomIdx = 118;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnFhyo50Data	：	付表５データ読込みクラス
//		pSnFhyo40Data	：	付表４データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSnHonpyoData	：	本表データ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo50::InitInfo( CH26SnFhyo50Data **pSnFhyo50Data, CH26SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CH26SnHonpyoData **pSnHonpyoData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo )
{
	ASSERT( pSnFhyo50Data );
	if( pSnFhyo50Data == NULL ){
		return -1;
	}
	m_pSnFhyo50Data = pSnFhyo50Data;

	ASSERT( pSnFhyo40Data );
	if( pSnFhyo40Data == NULL ){
		return -1;
	}
	m_pSnFhyo40Data = pSnFhyo40Data;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSnHonpyoData );
	if( pSnHonpyoData == NULL ){
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo50::Init()
{
//--> '14.05.16 INS START
	if( ((*m_pSnFhyo50Data)==NULL) || ((*m_pSnFhyo40Data)==NULL) || ((*m_pSnHonpyoData)==NULL) ){
		return -1;
	}
//<-- '14.05.16 INS END

	m_DpSw = 0;
	CQsw = 0;
	m_typeCnt = 0;

	// ファイルからＤＡＴＡＲＥＡＤ
	ReadData( 1 );
//--> '15.04.27 INS START
	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){
		// 連動時
		CQsw &= 0x0f;
	}
//<-- '15.04.27 INS END
	m_FrSw = 1;

	m_Tmp = 0;
	f5calq_4();				// （１）＋（２）－（３）
	f5calq_5();				// 単一事業の場合
	if( m_typeCnt > 1 ) {
		f5calq_ku();		// 事業別課税売上高の計算
		f5calq_gts( 0 );	// 複数事業種選択時のみ
		// 入力直後の特例計算サイン[S]read時に特例計算のサインが立っていなかったため判定を追加
		Tokurei50();
	}
	else{
		m_Tmp = 1;
	}

	// 付表５
	DispFh5();

	InitDiagAttr();

	//-->削除 harino ('14.03.06)
	// 課税標準率Ｂ欄へ
	//if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
	//	diag_setposition( IDC_ICSDIAGCTRL2, ID51T, CIcsdiagctrl );
	//}
	//<--削除 harino ('14.03.06)

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo50::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo5[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo5[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo5[cnt].UnConnect;
			}
			else{
				Flg = Fhyo5[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL2, Fhyo5[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo5[cnt].Connect );
		cnt++;
	}

	// 確定時＋非連動時には変更しない
	if( (*m_pSnHeadData)->IsInUnconnectInput(m_pZmSub) == FALSE ){
		return 0;
	}

	// 手入力サインに合わせた色変更
	if( F56sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID56T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2,  ID56T, 0 );
	}
	if( F513sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID513T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2,  ID513T, 0 );
	}
	if( F514sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID514T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2,  ID514T, 0 );
	}
	if( F515sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID515T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2,  ID515T, 0 );
	}
	if( F516sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID516T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2,  ID516T, 0 );
	}
	if( F517sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID517T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2,  ID517T, 0 );
	}
	if( F518sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 0 );
	}

	int ChkFlg = 0x800;
	if( F519sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID519T, 3 );
	}
	else if( F520sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID520T, 3 );
	}
	else if( F521sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID521T, 3 );
	}
	else if( F522sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID522T, 3 );
	}
	else if( F523sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID523T, 3 );
	}
	else if( F524sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID524T, 3 );
	}
	else if( F525sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID525T, 3 );
	}
	else if( F526sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID526T, 3 );
	}
	else if( F527sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID527T, 3 );
	}
	else if( F528sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID528T, 3 );
	}
	else if( F529sw & ChkFlg ){
		ChangeColor( IDC_ICSDIAGCTRL2,  ID529T, 3 );
	}

	m_Fhyo50Diag02.EnableDelete( 0 );

	return 0;
}

BOOL CShinFhyo50::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			WriteData();

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
		//-->追加 harino ('14.03.04)
		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
					TerminationIcsdiagctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
		//<--追加 harino ('14.03.04)
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数	upck	：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo50::ReadData( int upck )
{
	m_typeCnt = 0;

	// 課税標準額に対する消費税額
	// 経過措置なし
/*	memset( m_pSnFhyo40Data->Sn_1F1A,  '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F1B,  '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F2A,  '\0', MONY_BUF_SIZE );
	m_pSnFhyo40Data->Sn_1F2sw &= ~0x00f;
	memset( m_pSnFhyo40Data->Sn_1F2B,  '\0', MONY_BUF_SIZE );
	m_pSnFhyo40Data->Sn_1F2sw &= ~0x0f0;
	memset( m_pSnFhyo40Data->Sn_1F3A,  '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F3B,  '\0', MONY_BUF_SIZE );

//	m_pArith->l_sub( pSyzShin->Snd.Sn_1F8C, pSyzShin->Snd.Sn_1F8C, pSyzShin->Snd.Sn_1F8A );
//	memset( pSyzShin->Snd.Sn_1F8A,  '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F4A,  '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F4B,  '\0', MONY_BUF_SIZE );

//	m_pArith->l_sub( pSyzShin->Snd.Sn_1F9C, pSyzShin->Snd.Sn_1F9C, pSyzShin->Snd.Sn_1F9A );
//	memset( pSyzShin->Snd.Sn_1F9A,  '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F5A,  '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F5B,  '\0', MONY_BUF_SIZE );

	memset( m_pSnFhyo40Data->Sn_1F6A, '\0', MONY_BUF_SIZE );
	memset( m_pSnFhyo40Data->Sn_1F6B, '\0', MONY_BUF_SIZE );
*/
//	m_pArith->l_add( F51T, pSyzShin->Snd.Sn_1F4B, pSyzShin->Snd.Sn_1F5A );
//	memmove( F51B,  pSyzShin->Snd.Sn_1F4B, MONY_BUF_SIZE );
	memmove( F51T,  (*m_pSnFhyo40Data)->Sn_1F2C, MONY_BUF_SIZE );
	memmove( F51C,  (*m_pSnFhyo40Data)->Sn_1F2C, MONY_BUF_SIZE );

	// 貸倒回収に対する消費税額
	memmove( F52T,  (*m_pSnFhyo40Data)->Sn_1F3C, MONY_BUF_SIZE );
	memmove( F52C,  (*m_pSnFhyo40Data)->Sn_1F3C, MONY_BUF_SIZE );
	// 売上対価の返還等に係る消費税額
	memmove( F53T, (*m_pSnFhyo40Data)->Sn_1F5C, MONY_BUF_SIZE );
	memmove( F53C, (*m_pSnFhyo40Data)->Sn_1F5C, MONY_BUF_SIZE );
	m_pArith->l_add( F54S, F51T, F52T );
	m_pArith->l_sub( F54S, F54S, F53T );

	// 第一種事業課税売上高
/*- '14.02.23 -*/
//	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI1) ){
//		m_typeCnt++;
//	}
//	memmove( F57T, (*m_pSnHonpyoData)->Sn_KURI1, MONY_BUF_SIZE );
//	memmove( F57A,  (*m_pSnFhyo50Data)->Sn_5FURI1A, MONY_BUF_SIZE );
//	memmove( F57B,  (*m_pSnFhyo50Data)->Sn_5FURI1B, MONY_BUF_SIZE );
//	memmove( F57C,  (*m_pSnFhyo50Data)->Sn_5FURI1C, MONY_BUF_SIZE );
/*-------------*/
	memset( F57A, '\0', MONY_BUF_SIZE );
	memset( F57B, '\0', MONY_BUF_SIZE );
	memmove( F57C,  (*m_pSnFhyo50Data)->Sn_5FURI1C, MONY_BUF_SIZE );
	memmove( F57T, F57C, MONY_BUF_SIZE );
	if( m_pArith->l_cmp((*m_pSnHonpyoData)->Sn_KURI1, F57T) ){
		memmove( (*m_pSnHonpyoData)->Sn_KURI1, F57T, MONY_BUF_SIZE );
	}
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI1) ){
		m_typeCnt++;
	}
/*-------------*/

	F51UPb = (*m_pSnFhyo50Data)->Sn_5FUP1;
	memmove( F513T, (*m_pSnFhyo50Data)->Sn_5FUZT1, MONY_BUF_SIZE );
	memmove( F513A, (*m_pSnFhyo50Data)->Sn_5FUZK1A, MONY_BUF_SIZE );
	memmove( F513B, (*m_pSnFhyo50Data)->Sn_5FUZK1B, MONY_BUF_SIZE );
	memmove( F513C, (*m_pSnFhyo50Data)->Sn_5FUZK1C, MONY_BUF_SIZE );
	F513sw = (*m_pSnFhyo50Data)->Sn_5FUZK1sw;

	// 第二種事業課税売上高
/*- '14.02.23 -*/
//	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI2) ){
//		m_typeCnt++;
//	}
//	memmove( F58T, (*m_pSnHonpyoData)->Sn_KURI2, MONY_BUF_SIZE );
//	memmove( F58A, (*m_pSnFhyo50Data)->Sn_5FURI2A, MONY_BUF_SIZE );
//	memmove( F58B, (*m_pSnFhyo50Data)->Sn_5FURI2B, MONY_BUF_SIZE );
//	memmove( F58C, (*m_pSnFhyo50Data)->Sn_5FURI2C, MONY_BUF_SIZE );
/*-------------*/
	memset( F58A, '\0', MONY_BUF_SIZE );
	memset( F58B, '\0', MONY_BUF_SIZE );
	memmove( F58C, (*m_pSnFhyo50Data)->Sn_5FURI2C, MONY_BUF_SIZE );
	memmove( F58T, F58C, MONY_BUF_SIZE );
	if( m_pArith->l_cmp((*m_pSnHonpyoData)->Sn_KURI2, F58T) ){
		memmove( (*m_pSnHonpyoData)->Sn_KURI2, F58T, MONY_BUF_SIZE );
	}
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI2) ){
		m_typeCnt++;
	}
/*-------------*/

	F52UPb = (*m_pSnFhyo50Data)->Sn_5FUP2;
	memmove( F514T, (*m_pSnFhyo50Data)->Sn_5FUZT2, MONY_BUF_SIZE );
	memmove( F514A, (*m_pSnFhyo50Data)->Sn_5FUZK2A, MONY_BUF_SIZE );
	memmove( F514B, (*m_pSnFhyo50Data)->Sn_5FUZK2B, MONY_BUF_SIZE );
	memmove( F514C, (*m_pSnFhyo50Data)->Sn_5FUZK2C, MONY_BUF_SIZE );
	F514sw = (*m_pSnFhyo50Data)->Sn_5FUZK2sw;

	// 第三種事業課税売上高
/*- '14.02.23 -*/
//	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI3) ){
//		m_typeCnt++;
//	}
//	memmove( F59T, (*m_pSnHonpyoData)->Sn_KURI3, MONY_BUF_SIZE );
//	memmove( F59A, (*m_pSnFhyo50Data)->Sn_5FURI3A, MONY_BUF_SIZE );
//	memmove( F59B, (*m_pSnFhyo50Data)->Sn_5FURI3B, MONY_BUF_SIZE );
//	memmove( F59C, (*m_pSnFhyo50Data)->Sn_5FURI3C, MONY_BUF_SIZE );
/*-------------*/
	memset( F59A, '\0', MONY_BUF_SIZE );
	memset( F59B, '\0', MONY_BUF_SIZE );
	memmove( F59C, (*m_pSnFhyo50Data)->Sn_5FURI3C, MONY_BUF_SIZE );
	memmove( F59T, F59C, MONY_BUF_SIZE );
	if( m_pArith->l_cmp((*m_pSnHonpyoData)->Sn_KURI3, F59T) ){
		memmove( (*m_pSnHonpyoData)->Sn_KURI3, F59C, MONY_BUF_SIZE );
	}
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI3) ){
		m_typeCnt++;
	}
/*-------------*/

	F53UPb = (*m_pSnFhyo50Data)->Sn_5FUP3;
	memmove( F515T, (*m_pSnFhyo50Data)->Sn_5FUZT3, MONY_BUF_SIZE );
	memmove( F515A, (*m_pSnFhyo50Data)->Sn_5FUZK3A, MONY_BUF_SIZE );
	memmove( F515B, (*m_pSnFhyo50Data)->Sn_5FUZK3B, MONY_BUF_SIZE );
	memmove( F515C, (*m_pSnFhyo50Data)->Sn_5FUZK3C, MONY_BUF_SIZE );
	F515sw = (*m_pSnFhyo50Data)->Sn_5FUZK3sw;

	// 第四種事業課税売上高
/*- '14.02.23 -*/
//	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI4) ){
//		m_typeCnt++;
//	}
//	memmove( F510T, (*m_pSnHonpyoData)->Sn_KURI4, MONY_BUF_SIZE );
//	memmove( F510A, (*m_pSnFhyo50Data)->Sn_5FURI4A, MONY_BUF_SIZE );
//	memmove( F510B, (*m_pSnFhyo50Data)->Sn_5FURI4B, MONY_BUF_SIZE );
//	memmove( F510C, (*m_pSnFhyo50Data)->Sn_5FURI4C, MONY_BUF_SIZE );
/*-------------*/
	memset( F510A, '\0', MONY_BUF_SIZE );
	memset( F510B, '\0', MONY_BUF_SIZE );
	memmove( F510C, (*m_pSnFhyo50Data)->Sn_5FURI4C, MONY_BUF_SIZE );
	memmove( F510T, F510C, MONY_BUF_SIZE );
	if( m_pArith->l_cmp((*m_pSnHonpyoData)->Sn_KURI4, F510T) ){
		memmove( (*m_pSnHonpyoData)->Sn_KURI4, F510T, MONY_BUF_SIZE );
	}
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI4) ){
		m_typeCnt++;
	}
/*-------------*/

	F54UPb = (*m_pSnFhyo50Data)->Sn_5FUP4;
	memmove( F516T, (*m_pSnFhyo50Data)->Sn_5FUZT4, MONY_BUF_SIZE );
	memmove( F516A, (*m_pSnFhyo50Data)->Sn_5FUZK4A, MONY_BUF_SIZE );
	memmove( F516B, (*m_pSnFhyo50Data)->Sn_5FUZK4B, MONY_BUF_SIZE );
	memmove( F516C, (*m_pSnFhyo50Data)->Sn_5FUZK4C, MONY_BUF_SIZE );
	F516sw = (*m_pSnFhyo50Data)->Sn_5FUZK4sw;
	
	// 第五種事業課税売上高
/*- '14.02.23 -*/
//	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI5) ){
//		m_typeCnt++;
//	}
//	memmove( F511T, (*m_pSnHonpyoData)->Sn_KURI5, MONY_BUF_SIZE );
//	memmove( F511A, (*m_pSnFhyo50Data)->Sn_5FURI5A, MONY_BUF_SIZE );
//	memmove( F511B, (*m_pSnFhyo50Data)->Sn_5FURI5B, MONY_BUF_SIZE );
//	memmove( F511C, (*m_pSnFhyo50Data)->Sn_5FURI5C, MONY_BUF_SIZE );
/*-------------*/
	memset( F511A, '\0', MONY_BUF_SIZE );
	memset( F511B, '\0', MONY_BUF_SIZE );
	memmove( F511C, (*m_pSnFhyo50Data)->Sn_5FURI5C, MONY_BUF_SIZE );
	memmove( F511T, F511C, MONY_BUF_SIZE );
	if( m_pArith->l_cmp((*m_pSnHonpyoData)->Sn_KURI5, F511T) ){
		memmove( (*m_pSnHonpyoData)->Sn_KURI5, F511C, MONY_BUF_SIZE );
	}
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI5) ){
		m_typeCnt++;
	}
/*-------------*/
	
	F55UPb = (*m_pSnFhyo50Data)->Sn_5FUP5;
	memmove( F517T, (*m_pSnFhyo50Data)->Sn_5FUZT5, MONY_BUF_SIZE );
	memmove( F517A, (*m_pSnFhyo50Data)->Sn_5FUZK5A, MONY_BUF_SIZE );
	memmove( F517B, (*m_pSnFhyo50Data)->Sn_5FUZK5B, MONY_BUF_SIZE );
	memmove( F517C, (*m_pSnFhyo50Data)->Sn_5FUZK5C, MONY_BUF_SIZE );
	F517sw = (*m_pSnFhyo50Data)->Sn_5FUZK5sw;

	// 事業区分別の合計額
	char	WORK0[MONY_BUF_SIZE]={0};
	m_pArith->l_add( WORK0, F57C, F58C );
	m_pArith->l_add( WORK0, WORK0, F59C );
	m_pArith->l_add( WORK0, WORK0, F510C );
	m_pArith->l_add( WORK0, WORK0, F511C );
//--> '15.02.10 INS START
	if( m_pArith->l_test(WORK0) < 0 ){
		m_pArith->l_clear( WORK0 );
	}
//<-- '15.02.10 INS END
	memset( F56A, '\0', MONY_BUF_SIZE );
	memset( F56B, '\0', MONY_BUF_SIZE );
	memmove( F56C, (*m_pSnFhyo50Data)->Sn_5FURITC, MONY_BUF_SIZE );
	F56sw = (*m_pSnFhyo50Data)->Sn_5FURITsw;
	if( !(F56sw&0x800) ){
		if( m_pArith->l_cmp(WORK0, F56C) ){
			memmove( F56C, WORK0, MONY_BUF_SIZE );
			memmove( (*m_pSnFhyo50Data)->Sn_5FURITC, F56C, MONY_BUF_SIZE );
		}
		memmove( F56T, F56C, MONY_BUF_SIZE );
		if( m_pArith->l_cmp( F56T, (*m_pSnHeadData)->Sn_KURIA ) ){
			memmove( (*m_pSnHeadData)->Sn_KURIA, F56T, MONY_BUF_SIZE );
		}
	}
	else{
		memmove( F56T, F56C, MONY_BUF_SIZE );
		if( m_pArith->l_cmp( F56T, (*m_pSnHeadData)->Sn_KURIA ) ){
			memmove( (*m_pSnHeadData)->Sn_KURIA, F56T, MONY_BUF_SIZE );
		}
	}

	memset( F512A, '\0', MONY_BUF_SIZE );
	memset( F512B, '\0', MONY_BUF_SIZE );
	memmove( F512C, (*m_pSnFhyo50Data)->Sn_5FUZKTC, MONY_BUF_SIZE );
	memmove( F512T, (*m_pSnFhyo50Data)->Sn_5FUZTT, MONY_BUF_SIZE );
	F512sw = (*m_pSnFhyo50Data)->Sn_5FUZKTsw;

	// 事業区分別の合計額
	memmove( F56S, F56T, MONY_BUF_SIZE );
	memmove( F56SS, F56B, MONY_BUF_SIZE );

	// 原則計算を適用する場合の控除対象仕入税額
	memmove( F518T, (*m_pSnFhyo50Data)->Sn_5FSZTG, MONY_BUF_SIZE );
	memmove( F518A, (*m_pSnFhyo50Data)->Sn_5FSZKGA, MONY_BUF_SIZE );
	memmove( F518B, (*m_pSnFhyo50Data)->Sn_5FSZKGB, MONY_BUF_SIZE );
	memmove( F518C, (*m_pSnFhyo50Data)->Sn_5FSZKGC, MONY_BUF_SIZE );

	// 特例計算を適用する場合の控除対象仕入税額
	if( (*m_pSnFhyo50Data)->Sn_5FSZsw ){
		memmove( F519T, (*m_pSnFhyo50Data)->Sn_5FSZTT, MONY_BUF_SIZE );
		memmove( F519A, (*m_pSnFhyo50Data)->Sn_5FSZKTA, MONY_BUF_SIZE );
		memmove( F519B, (*m_pSnFhyo50Data)->Sn_5FSZKTB, MONY_BUF_SIZE );
		memmove( F519C, (*m_pSnFhyo50Data)->Sn_5FSZKTC, MONY_BUF_SIZE );
	}
	else{
		memset( F519T, '\0', MONY_BUF_SIZE );
		memset( F519B, '\0', MONY_BUF_SIZE );
	}

	// １種類の事業の専業者の場合
	// 特例計算適用ＳＷ
	SZsw = (*m_pSnFhyo50Data)->Sn_5FSZsw;
	SBsw = (*m_pSnFhyo50Data)->Sn_5FSBsw;
	CQsw = (*m_pSnFhyo50Data)->Sn_5FCQsw;
	F518sw = (*m_pSnFhyo50Data)->Sn_5FGKsw;

	F519sw = (*m_pSnFhyo50Data)->Sn_5FSZT19sw;//0x0800
	F520sw = (*m_pSnFhyo50Data)->Sn_5FSZT20sw;
	F521sw = (*m_pSnFhyo50Data)->Sn_5FSZT21sw;
	F522sw = (*m_pSnFhyo50Data)->Sn_5FSZT22sw;
	F523sw = (*m_pSnFhyo50Data)->Sn_5FSZT23sw;
	F524sw = (*m_pSnFhyo50Data)->Sn_5FSZT24sw;
	F525sw = (*m_pSnFhyo50Data)->Sn_5FSZT25sw;
	F526sw = (*m_pSnFhyo50Data)->Sn_5FSZT26sw;
	F527sw = (*m_pSnFhyo50Data)->Sn_5FSZT27sw;
	F528sw = (*m_pSnFhyo50Data)->Sn_5FSZT28sw;
	F529sw = (*m_pSnFhyo50Data)->Sn_5FSZT29sw;

	memmove( F520T, (*m_pSnFhyo50Data)->Sn_5FSZT20C, MONY_BUF_SIZE );
	memmove( F521T, (*m_pSnFhyo50Data)->Sn_5FSZT21C, MONY_BUF_SIZE );
	memmove( F522T, (*m_pSnFhyo50Data)->Sn_5FSZT22C, MONY_BUF_SIZE );
	memmove( F523T, (*m_pSnFhyo50Data)->Sn_5FSZT23C, MONY_BUF_SIZE );
	memmove( F524T, (*m_pSnFhyo50Data)->Sn_5FSZT24C, MONY_BUF_SIZE );
	memmove( F525T, (*m_pSnFhyo50Data)->Sn_5FSZT25C, MONY_BUF_SIZE );
	memmove( F526T, (*m_pSnFhyo50Data)->Sn_5FSZT26C, MONY_BUF_SIZE );
	memmove( F527T, (*m_pSnFhyo50Data)->Sn_5FSZT27C, MONY_BUF_SIZE );
	memmove( F528T, (*m_pSnFhyo50Data)->Sn_5FSZT28C, MONY_BUF_SIZE );
	memmove( F529T, (*m_pSnFhyo50Data)->Sn_5FSZT29C, MONY_BUF_SIZE );

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo50::WriteData()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	// 入力直後の特例計算サイン
	Tokurei50();

/*- '14.03.12_CUT -*/
//	// 課税標準額に対する消費税額
//	memmove( (*m_pSnFhyo40Data)->Sn_1F2C, F51T,  6 );
//	// 貸倒回収に対する消費税額
//	memmove( (*m_pSnFhyo40Data)->Sn_1F3C, F52T,  6 );
//	// 売上対価の返還等に係る消費税額
//	memmove( (*m_pSnFhyo40Data)->Sn_1F5C, F53T,  6 );
////	memmove( (*m_pSnFhyo40Data)->Sn_1F9C, F53T, 6 );
//	m_pArith->l_add( WORK0, F51T,  F52T );
//	m_pArith->l_sub( WORK0, WORK0, F53T );
/*- '14.03.12_CUT -*/

	// 事業区分別の合計額	
	memmove( (*m_pSnHeadData)->Sn_KURIA,  F56T,  6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FURIK, F56T,  6 );
//	memmove( (*m_pSnFhyo50Data)->Sn_5FURITC, F56C,  6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FURITC, F56T,  6 );
	(*m_pSnFhyo50Data)->Sn_5FURITsw = F56sw;
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZTT, F512T, 6 );
//	memmove( (*m_pSnFhyo50Data)->Sn_5FUZKTC, F512C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZKTC, F512T, 6 );
	(*m_pSnFhyo50Data)->Sn_5FUZKTsw = F512sw;
	// 第一種事業課税売上高
	memmove( (*m_pSnHonpyoData)->Sn_KURI1,  F57T,  6  );
	memmove( (*m_pSnFhyo50Data)->Sn_5FURI1C,  F57T,  6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZT1, F513T, 6 );
	(*m_pSnFhyo50Data)->Sn_5FUZK1sw = F513sw;
//	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK1C, F513C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK1C, F513T, 6 );
	// 第二種事業課税売上高
	memmove( (*m_pSnHonpyoData)->Sn_KURI2,  F58T,  6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FURI2C,  F58T,  6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZT2, F514T, 6 );
	(*m_pSnFhyo50Data)->Sn_5FUZK2sw = F514sw;
//	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK2C, F514C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK2C, F514T, 6 );
	// 第三種事業課税売上高
	memmove( (*m_pSnHonpyoData)->Sn_KURI3,  F59T,  6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FURI3C,  F59T,  6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZT3, F515T, 6 );
	(*m_pSnFhyo50Data)->Sn_5FUZK3sw = F515sw;
//	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK3C, F515C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK3C, F515T, 6 );
	// 第四種事業課税売上高
	memmove( (*m_pSnHonpyoData)->Sn_KURI4,  F510T, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FURI4C, F510T, 6 );	
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZT4, F516T, 6 );
	(*m_pSnFhyo50Data)->Sn_5FUZK4sw = F516sw;
//	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK4C, F516C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK4C, F516T, 6 );
	// 五種事業課税売上高
	memmove( (*m_pSnHonpyoData)->Sn_KURI5,  F511T, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FURI5C, F511T, 6 );	
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZT5, F517T, 6 );
	(*m_pSnFhyo50Data)->Sn_5FUZK5sw = F517sw;
//	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK5C, F517C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FUZK5C, F517T, 6 );
	// 則計算を適用する場合の控除対象仕入税額
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZTG, F518T, 6 );
//	memmove( (*m_pSnFhyo50Data)->Sn_5FSZKGC, F518C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZKGC, F518T, 6 );
	// 例計算を適用する場合の控除対象仕入税額
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZTT, F519T, 6 );
//	memmove( (*m_pSnFhyo50Data)->Sn_5FSZKTC, F519C, 6 );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZKTC, F519T, 6 );
	// 種類の事業の専業者の場合
	memset( (*m_pSnFhyo40Data)->Sn_1F4A, '\0', 6 );
	memset( (*m_pSnFhyo40Data)->Sn_1F4A, '\0', 6 );
	memset( (*m_pSnFhyo40Data)->Sn_1F4C, '\0', 6 );

	if( CQsw & 0x0f ){
		memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F55T, 6 );
	}
	else {
		if( !(*m_pSnHeadData)->Sn_TOKUR ){
			memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F518T, 6 );
		}
		else {
			memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F530T, 6 );
		}
	}

	// 特例計算適用ＳＷ
	(*m_pSnFhyo50Data)->Sn_5FSZsw = SZsw;
	(*m_pSnFhyo50Data)->Sn_5FSBsw = SBsw;
	(*m_pSnFhyo50Data)->Sn_5FCQsw = CQsw;
	(*m_pSnFhyo50Data)->Sn_5FGKsw = F518sw;

	(*m_pSnFhyo50Data)->Sn_5FSZT19sw = F519sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT20sw = F520sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT21sw = F521sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT22sw = F522sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT23sw = F523sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT24sw = F524sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT25sw = F525sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT26sw = F526sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT27sw = F527sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT28sw = F528sw;
	(*m_pSnFhyo50Data)->Sn_5FSZT29sw = F529sw;

	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT20C, F520T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT21C, F521T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT22C, F522T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT23C, F523T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT24C, F524T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT25C, F525T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT26C, F526T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT27C, F527T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT28C, F528T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo50Data)->Sn_5FSZT29C, F529T, MONY_BUF_SIZE );

	return 0;
}

//-----------------------------------------------------------------------------
// 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo50::DispFh5()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for( int i=119; i<=156; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}

	// 期間・氏名の表示
	char	buf[512] = {0};
	char	yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof( buf ), _T("%02x.%02x.%02x"), (yy&0xff),(mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 119, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s( buf, sizeof( buf ), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 120, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( buf, '\0', sizeof(buf) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	}
	else{
		memmove( buf, (*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM) );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 121, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税標準額に対する消費税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID51T, F51T );
	// 貸倒回収額に対する消費税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID52T, F52T );
	// 売上対価の返還等に係る消費税額
	dsp_prs( IDC_ICSDIAGCTRL2, ID53T, F53T );
	// 控除対象仕入税額計算の基礎となる消費税額
	f5calq_4();
	// 単一事業の場合の控除対象仕入税額
	f5calq_5();
	if( m_typeCnt > 1 ){		// 複数事業種選択時のみ表示
		// 課税売上高に係る消費税額の計算欄
		f5calq_ku();
		//	事業別課税売上割合の表示
		f5calq_uws();

		// 原則計算を適用する場合
		f5calq_gn();
		// 特例計算を適用する場合
		f5calq_tk();	

		f5dsp_tk();
	}
	else{
		// 課税売上高に係る消費税額の計算欄
		f5calq_ku();
		//	事業別課税売上割合の表示
		f5calq_uws();
	}

	// 非連動ｄｂ
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE ){	
		diag_clear( IDC_ICSDIAGCTRL1, 3, TRUE, CIcsdiagctrl );
		diag_getattr( IDC_ICSDIAGCTRL1, 3, &DA, CIcsdiagctrl );
		DA.attr_bcolor = SIROIROCOLOR;
		diag_setattr( IDC_ICSDIAGCTRL1, 3, &DA, FALSE, CIcsdiagctrl );
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) || ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	ChangeColor( IDC_ICSDIAGCTRL2,  ID511T, 0 );
	ChangeColor( IDC_ICSDIAGCTRL2,  ID517T, 0 );
	diag_modify( IDC_ICSDIAGCTRL2, ID511T, DIAG_MDFY_EDIT, CIcsdiagctrl );
	diag_modify( IDC_ICSDIAGCTRL2, ID517T, DIAG_MDFY_EDIT, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo50::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){	// 再集計のみ（表示無し）
		switch( Index ){
			case ID54T	:		//  4.
			case ID55T	:		//  5.
			case ID530T	:		// 30.
				memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
				diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				break;
			default		:
				memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
				diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinFhyo50::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){	// 再集計のみ（表示無し）
		switch( Index ){
			case 119 :			// 課税期間　自
			case 120 :			// 課税期間　至
			case 121 :			// 氏名又は､名称
			case ID57TE	:		//  7. 売上割合
			case ID58TE	:		//  8. 売上割合
			case ID59TE	:		//  9. 売上割合
			case ID510TE:		// 10. 売上割合
			case ID511TE:		// 11. 売上割合	
				diag_clear( Id, Index, TRUE, CIcsdiagctrl );
				break;
			default :
				memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
				diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinFhyo50::ChangeColor( unsigned short id, short index, int sign )
{
	if( (*m_pSnHeadData)->IsInUnconnectInput(m_pZmSub) == FALSE ){
		return;
	}

	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :	// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;		
		case 2 :	// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;
			break;		
		case 3 :	// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;
			break;		
		case 4 :	// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5 :	// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:	// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;
			break;
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 売上割合表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Pers	：	表示割合
//		Sign	：	
//-----------------------------------------------------------------------------
void CShinFhyo50::dsp_uws( unsigned short Id, short Index, unsigned short Pers, short Sign )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char				buf[512] = {0};

	if( !m_DpSw ){
		if( Pers ){	// 再集計のみ（表示無し）
			memset( buf, '\0', sizeof(buf) );
			if( Sign ){
				sprintf_s( buf, sizeof(buf), _T("%3d.%01d"), Pers / 10,  Pers % 10 );
			}
			else	{
				sprintf_s( buf, sizeof(buf), _T("%3d.%02d"), Pers / 100, Pers % 100 );
			}
			DiagData.data_disp = (char *)buf;
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			dsp_cls( Id, Index );
		}
	}
}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinFhyo50::GetAllDiagData()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	m_Fhyo50Diag01.SetFocus();//★[13'12.03]無理やり
	
	//  1. 課税標準額に対する消費税額（期間計）
	diag_getdata( IDC_ICSDIAGCTRL2, ID51T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F51T, DiagData.data_val, MONY_BUF_SIZE ); 
	//  2. 貸倒回収額に対する消費税額（期間計）
	diag_getdata( IDC_ICSDIAGCTRL2, ID52T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F52T, DiagData.data_val, MONY_BUF_SIZE ); 
	//  3. 売上対価の返還等に係る消費税額（期間計）
	diag_getdata( IDC_ICSDIAGCTRL2, ID53T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F53T, DiagData.data_val, MONY_BUF_SIZE ); 
}

BEGIN_EVENTSINK_MAP(CShinFhyo50, CSyzBaseDlg)
	ON_EVENT(CShinFhyo50, IDC_ICSDIAGCTRL2, 1, CShinFhyo50::EditONIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo50, IDC_ICSDIAGCTRL2, 2, CShinFhyo50::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo50, IDC_ICSDIAGCTRL2, 3, CShinFhyo50::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo50::EditONIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rectA, rectB;
	//GetWindowRect( &rectA );
	//diag_getrect( IDC_ICSDIAGCTRL2, index, &rectB, CIcsdiagctrl );
	//if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	//}
	m_curwnd = IDC_ICSDIAGCTRL2;
}

void CShinFhyo50::EditOFFIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	CkMy[MONY_BUF_SIZE]={0}, CkVl[MONY_BUF_SIZE]={0};

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// [03'07.15]
	diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_Tmp = 0;	// 入力あり
	if( index >= ID518T && index <= ID529T){
		if(m_typeCnt == 1){
			dsp_cls( IDC_ICSDIAGCTRL2, index );
			return;
		}			
	}

	switch( index ) {

		case	ID51T	:	//  1. 課税標準額に対する消費税額（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F51T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID51T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F51T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( !m_pArith->l_cmp(F51T, CkMy) ){
				break;
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			break;

		case	ID52T	:	//  2. 貸倒回収額に対する消費税額（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F52T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID52T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F52T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( !m_pArith->l_cmp(F52T, CkMy) ){
				break;
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			break;

		case	ID53T	:	//  3. 売上対価の返還等に係る消費税額（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F53T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID53T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F53T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( !m_pArith->l_cmp(F53T, CkMy) ){
				break;
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			break;

		case	ID56T	:	//	6. 事業別課税売上高合計（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F56T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID56T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F56T, CkMy) ){
//				F50c |= 0x01;
				F512sw |= 0x100;
				F56sw |= 0x800;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			memmove( F56T, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f5calq_all( 1 );

			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID512T	:	// 12. 事業別課税売上に係る消費税額合計（期間計）
			memmove( CkMy, F512T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID512T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F512T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, F512T) ){
				CQsw &= 0x0f;
				F512sw |= 0x800;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			// 各入力の計算表示
			f5calq_all( 1 );

			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();

			break;

		case	ID57T	:	//	7. 事業別課税売上高･第一種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F57T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID57T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F57T, CkMy) ){
//				F51c |= 0x01;
				F513sw |= 0x100;
				CQsw &= 0x0f;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			memmove( F57T, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID513T	:	//	13. 事業別課税売上に係る消費税額･第一種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F513T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID513T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F513T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F513T, CkMy) ){
				CQsw &= 0x0f;
				F513sw |= 0x800;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID58T	:	//	8. 事業別課税売上高･第二種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F58T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID58T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F58T, CkMy) ){
//				F52c |= 0x01;
				F514sw |= 0x100;
				CQsw &= 0x0f;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			memmove( F58T, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID514T	:	// 14. 事業別課税売上に係る消費税額･第二種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F514T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID514T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F514T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F514T, CkMy) ){
				CQsw &= 0x0f;
				F514sw |= 0x800;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID59T	:	//	9. 事業別課税売上高･第三種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F59T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID59T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F59T, CkMy) ){
//				F53c |= 0x01;
				F515sw |= 0x100;
				CQsw &= 0x0f;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			memmove( F59T, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID515T	:	// 15. 事業別課税売上に係る消費税額･第三種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F515T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID515T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F515T, DiagData.data_val, 6 ); 
			if( m_pArith->l_cmp(F515T, CkMy) ){
				CQsw &= 0x0f;
				F515sw |= 0x800;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID510T	:	// 10. 事業別課税売上高･第四種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F510T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID510T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F510T, CkMy) ){
//				F54c |= 0x01;
				F516sw |= 0x100;
				CQsw &= 0x0f;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			memmove( F510T, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID516T	:	// 16. 事業別課税売上に係る消費税額･第四種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F516T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID516T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F516T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F516T, CkMy) ){
				CQsw &= 0x0f;
				F516sw |= 0x800;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID511T	:	// 11. 事業別課税売上高･第五種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F511T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID511T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F511T, CkMy) ){
//				F55c |= 0x01;
				F517sw |= 0x100;
				CQsw &= 0x0f;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			memmove( F511T, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID517T	:	// 17. 事業別課税売上に係る消費税額･第五種（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F517T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID517T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F517T, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F517T, CkMy) ){
				CQsw &= 0x0f;
				F517sw |= 0x800;
			}
			else{
				break;		// 表示無し[08'11.07]
			}
			// 各入力の計算表示
			f5calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			break;

		case	ID518T	:	// 18. 原則計算適用･合計 仕入税額（期間計）
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F518T, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID518T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F518T, CkMy) ){
//				F518c |= 0x01;
				if( m_pArith->l_test(F54S) >= 0 ){
					F518sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 3 );
					memmove( F518T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				else{
					dsp_cls( IDC_ICSDIAGCTRL2, index );
				}
			}
			else{
				memmove( F518T, CkMy, MONY_BUF_SIZE );
			}

			// 各入力の計算表示
			f5dsp_tk();
			CQsw |= 0x10;
			break;

		case	ID519T	:	// 19. 特例適用･合計 仕入税額(4%分)	
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 1 ){
				memmove( CkMy, F519T, MONY_BUF_SIZE );
				memmove( CkVl, F519T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID519T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){
				SZsw = ( char ) ( m_pArith->l_test(CkMy) ? 1 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F519sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID519T, 3 );
					memmove( F519T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;

		case	ID520T	:	// 20.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 2 ){
				memmove( CkMy, F520T, MONY_BUF_SIZE );
				memmove( CkVl, F520T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID520T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){
				SZsw = ( char ) ( m_pArith->l_test(CkMy) ? 2 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F520sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID520T, 3 );
					memmove( F520T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;
	
		case	ID521T	:	// 21.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 3 ){
				memmove( CkMy, F521T, MONY_BUF_SIZE );
				memmove( CkVl, F521T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID521T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 3 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F521sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID521T, 3 );
					memmove( F521T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;
	
		case	ID522T	:	// 22.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 4 ){
				memmove( CkMy, F522T, MONY_BUF_SIZE );
				memmove( CkVl, F522T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID522T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp( CkMy, CkVl ) ) {
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 4 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F522sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID522T, 3 );
					memmove( F522T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;
	
		case	ID523T	:	// 23.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 5 ){
				memmove( CkMy, F523T, MONY_BUF_SIZE );
				memmove( CkVl, F523T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID523T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 5 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F523sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID523T, 3 );
					memmove( F523T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;

		case	ID524T	:	// 24.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 6 )	{
				memmove( CkMy, F524T, MONY_BUF_SIZE );
				memmove( CkVl, F524T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID524T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp( CkMy, CkVl ) ) {
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 6 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F524sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID524T, 3 );
					memmove( F524T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;

		case	ID525T	:	// 25.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 7 ){
				memmove( CkMy, F525T, MONY_BUF_SIZE );
				memmove( CkVl, F525T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID525T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 7 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F525sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID525T, 3 );
					memmove( F525T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;

		case	ID526T	:	// 26.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 8 )	{
				memmove( CkMy, F526T, MONY_BUF_SIZE );
				memmove( CkVl, F526T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID526T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp( CkMy, CkVl ) ) {
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 8 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F526sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID526T, 3 );
					memmove( F526T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;

		case	ID527T	:	// 27.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 9 ){
				memmove( CkMy, F527T, MONY_BUF_SIZE );
				memmove( CkVl, F527T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID527T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 9 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F527sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID527T, 3 );
					memmove( F527T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;

		case	ID528T	:	// 28.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 10 ){
				memmove( CkMy, F528T, MONY_BUF_SIZE );
				memmove( CkVl, F528T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID528T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 10 : 0 );
				AllTkSwClear();
				if( m_pArith->l_test(F54S) >= 0 ){
					F528sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID528T, 3 );
					memmove( F528T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;

		case	ID529T	:	// 29.
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memset( CkVl, '\0', MONY_BUF_SIZE );
			if( (SZsw&0xff) == 11 )	{
				memmove( CkMy, F529T, MONY_BUF_SIZE );
				memmove( CkVl, F529T, MONY_BUF_SIZE );
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID529T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp( CkMy, CkVl ) ) {
				SZsw = ( char ) ( m_pArith->l_test( CkMy ) ? 11 : 0 );
				if( m_pArith->l_test(F54S) >= 0 ){
					AllTkSwClear();
					F529sw |= 0x800;
					ChangeColor( IDC_ICSDIAGCTRL2,  ID529T, 3 );
					memmove( F529T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
					Tokurei50();
				}
				// 各入力の計算表示
				f5dsp_tk();
				CQsw |= 0x10;
			}
			break;
			
		default:	
			break;
	}
}

void CShinFhyo50::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	if( nChar == 0 ){
		return;
	}
	
	char	VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	short	wTerm;
	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}

	int pos = 0;
	if( wTerm==VK_UP ){
		pos = UpDownFocusMoveCheck(index , 1);
		m_Fhyo50Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo50Diag02.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) ){
		m_Fhyo50Diag02.SetNextPosition();
	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Fhyo50Diag02.SetPrevPosition();
	}

	if( wTerm == VK_DELETE ){
		if( index == ID518T ){
			F518sw &= ~0x800;
			ChangeColor( IDC_ICSDIAGCTRL2, ID518T, 0 );
			memset( F518T, '\0', MONY_BUF_SIZE );
			CQsw &= 0x0f;
			f5calq_all( 3 );
//			// 入力直後の特例計算サイン
			Tokurei50();
			// 各入力の計算表示
			f5dsp_tk();
			m_Fhyo50Diag02.SetPosition( ID518T );
		}
		else if( index == ID56T ){
			F56sw &= ~0x800;
			f5calq_all( 3 );
			m_Fhyo50Diag02.SetPosition( ID56T );
		}
		else if( index == ID513T ){
			F513sw &= ~0x800;
			F513sw |= 0x100;
			f5calq_all( 3 );
			m_Fhyo50Diag02.SetPosition( ID513T );
		}
		else if( index == ID514T ){
			F514sw &= ~0x800;
			F514sw |= 0x100;
			f5calq_all( 3 );
			m_Fhyo50Diag02.SetPosition( ID514T );
		}
		else if( index == ID515T ){
			F515sw &= ~0x800;
			F515sw |= 0x100;
			f5calq_all( 3 );
			m_Fhyo50Diag02.SetPosition( ID515T );
		}
		else if( index == ID516T ){
			F516sw &= ~0x800;
			F516sw |= 0x100;
			f5calq_all( 3 );
			m_Fhyo50Diag02.SetPosition( ID516T );
		}
		else if( index == ID517T ){
			F517sw &= ~0x800;
			F517sw |= 0x100;
			f5calq_all( 3 );
			m_Fhyo50Diag02.SetPosition( ID517T );
		}
		else if( index >= ID519T && index <= ID529T ){
			if( TkSwCheck(index) == 1){
				AllTkSwClear();
				// 複数事業種選択時のみ表示
				CQsw &= 0x0f;
				// 複数事業種選択時のみ表示
//				if( m_typeCnt > 1 ){
					// 原則計算を適用する場合
					f5calq_gn();
					// 特例計算を適用する場合
					f5calq_tk();
					Tokurei50();
					f5dsp_tk();
//				}
			}		
			m_Fhyo50Diag02.SetPosition( index );
		}
		else{
			dsp_cls( IDC_ICSDIAGCTRL2, index );
		}

	}

	//-->Editonから移動 harino ('14.03.07)
	int nowpos = m_Fhyo50Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFhyo50::UpDownFocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	for(i = 0 ; i < FCS_FHYO5CNT ; i++){
		if(Index == FCS_Fhyo5[i].IDNo){
			break;
		}
	}

	idx = FCS_Fhyo5[i].IDNo;//現在ポジションセット

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHYO5CNT ; j++){
			m_Fhyo50Diag02.GetAttribute(FCS_Fhyo5[j].IDNo ,  (LPUNKNOWN)&DA);
			if( (DA.attr_bcolor==BC_CREAM) || (DA.attr_bcolor==BC_BLUE) ){
				idx = FCS_Fhyo5[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo50Diag02.GetAttribute(FCS_Fhyo5[j].IDNo ,  (LPUNKNOWN)&DA);
			if( (DA.attr_bcolor==BC_CREAM) || (DA.attr_bcolor==BC_BLUE) ){
				idx = FCS_Fhyo5[j].IDNo;
				break;
			}
		}
	}

	return idx;
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo50::SetItemPosition()
{
	int index;

	//-->追加 harino ('14.03.06)
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_Fhyo50Diag02.GetPosition();
		if( (index = m_Fhyo50Diag02.GetPosition()) != -1) {
			m_Fhyo50Diag02.SetPosition(index);
			m_Fhyo50Diag02.SetFocus();
			return;
		}
	}
	//<--追加 harino ('14.03.06)

	m_Fhyo50Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo50Diag02.SetFocus();

	//入力可能なインデックスを探す処理が必要？
	m_Fhyo50Diag02.SetPosition( ID57T );

}
void CShinFhyo50::AllTkSwClear(){

	int id = 0;
	int chkFlg = 0x800;
	if( (F519sw & chkFlg) ){
		F519sw = 0x00;
		id = ID519T;
	}
	else if( F520sw & chkFlg ){
		F520sw = 0x00;
		id = ID520T;
	}
	else if( F521sw & chkFlg ){
		F521sw = 0x00;
		id = ID521T;
	}
	else if( F522sw & chkFlg ){
		F522sw = 0x00;
		id = ID522T;
	}
	else if( F523sw & chkFlg ){
		F523sw = 0x00;
		id = ID523T;
	}
	else if( F524sw & chkFlg ){
		F524sw = 0x00;
		id = ID524T;
	}
	else if( F525sw & chkFlg ){
		F525sw = 0x00;
		id = ID525T;
	}
	else if( F526sw & chkFlg ){
		F526sw = 0x00;
		id = ID526T;
	}
	else if( F527sw & chkFlg ){
		F527sw = 0x00;
		id = ID527T;
	}
	else if( F528sw & chkFlg ){
		F528sw = 0x00;
		id = ID528T;
	}
	else if( F529sw & chkFlg ){
		F529sw = 0x00;
		id = ID529T;
	}
	
	if( id ) {
		ChangeColor( IDC_ICSDIAGCTRL2, id , 0 );
	}
}
int CShinFhyo50::TkSwCheck(int idx){

	int chkFlg = 0x800;
	int st = 0;

	switch( idx ){
		case ID519T:
			if( F519sw & chkFlg ){
				st = 1;
			}
			break;
		case ID520T:
			if( F520sw & chkFlg ){
				st = 1;
			}
			break;
		case ID521T:
			if( F521sw & chkFlg ){
				st = 1;
			}
			break;
		case ID522T:
			if( F522sw & chkFlg ){
				st = 1;
			}
			break;
		case ID523T:
			if( F523sw & chkFlg ){
				st = 1;
			}
			break;
		case ID524T:
			if( F524sw & chkFlg ){
				st = 1;
			}
			break;
		case ID525T:
			if( F525sw & chkFlg ){
				st = 1;
			}
			break;
		case ID526T:
			if( F526sw & chkFlg ){
				st = 1;
			}
			break;
		case ID527T:
			if( F527sw & chkFlg ){
				st = 1;
			}
			break;
		case ID528T:
			if( F528sw & chkFlg ){
				st = 1;
			}
			break;
		case ID529T:
			if( F529sw & chkFlg ){
				st = 1;
			}
			break;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 課税取引金額計算書から転記時のフォーカス移動
//-----------------------------------------------------------------------------
void CShinFhyo50::MoveFocusInTaxTenki()
{
	m_Fhyo50Diag02.SetPosition( ID56T );

	CQsw = 0;
	(*m_pSnFhyo50Data)->Sn_5FCQsw = CQsw;
}

