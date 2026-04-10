// SWKselDlg.cpp : インプリメンテーション ファイル
//
// 仕訳摘要の登録タイプを選択

#include "stdafx.h"
#include "resource.h"

#include "SWKselDlg.h"

#include "icsdisp.h"

#include "swktkydef.h"
#include "DBInpSubFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static
int	SetSwkRegSel( SWK_REGSEL* pSel, int max, BOOL bBoth, CDBZmSub* pZm );



/////////////////////////////////////////////////////////////////////////////
// CSWKselDlg ダイアログ

BOOL CSWKselDlg::bSwkDlgShow	=	FALSE;


CSWKselDlg::CSWKselDlg(CWnd* pParent /*=NULL*/, BOOL bDCBoth /*= FALSE*/ )
	: ICSDialog(CSWKselDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSWKselDlg)
	m_Radio = -1;
	//}}AFX_DATA_INIT

	m_bDCBoth = bDCBoth;
}


void CSWKselDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSWKselDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}


void CSWKselDlg::SetZmClass(CDBZmSub* pZm)
{
	m_pZm = pZm;
}


BEGIN_MESSAGE_MAP(CSWKselDlg, ICSDialog)
	//{{AFX_MSG_MAP(CSWKselDlg)
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSWKselDlg メッセージ ハンドラ

BOOL CSWKselDlg::OnInitDialog() 
{
//	ControlInit(255,140,-1);
	
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// 仕訳登録可能 数 取得
	int cnt = SetSwkRegSel( m_Rgsel, (sizeof(m_Rgsel)/sizeof(m_Rgsel[0])), m_bDCBoth, m_pZm );

	if( !cnt ) {
		EndDialog(IDCANCEL);
		return TRUE;
	}

	m_Radio = 0;
	UpdateData(FALSE);
	SetRadioString(cnt);

#ifdef CLOSE
	c_disp( IDC_ICSDISPCTRL1, 0, 1, "＊入力時の表示", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 2, 1, 
			"・仕訳摘要として登録した場合、その仕訳に対応した摘要として", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 3, 1, "　表示します。", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 4, 1, 
			"・単独科目摘要として登録した場合、借方・貸方の発生に関係なく", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 5, 1, "　摘要を表示します。", CICSDisp );

//	c_disp( IDC_ICSDISPCTRL1, 5, 1, 
//			"・単独科目設定にした場合、科目属性に従って借方・貸方のどちらかに", CICSDisp );
//	c_disp( IDC_ICSDISPCTRL1, 6, 1, "　科目を設定します。", CICSDisp );
#endif

	PostMessage( WM_KEYDOWN, VK_TAB );

	ICSDialog::OnInitDialogEX();

	c_disp( IDC_ICSDISPCTRL1, 0, 1, "＊入力時の表示", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 2, 1, 
			"・仕訳摘要として登録した場合、その仕訳に対応した摘要として", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 3, 1, "　表示します。", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 4, 1, 
			"・単独科目摘要として登録した場合、借方・貸方の発生に関係なく", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 5, 1, "　摘要を表示します。", CICSDisp );

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ラジオボタン 文字列
void CSWKselDlg::SetRadioString( int cnt )
{
	UINT rdID[] = {
		IDC_RADIO1, IDC_RADIO2, IDC_RADIO3,
		0
	};

	CString str;
	int endsw = 0;
	DBKNREC* pDeb, *pCre;

	for( int i = 0; rdID[i] != 0; i++ ) {
		switch( m_Rgsel[i].type ) {
		case RSW_SIWAKE:
			pDeb = DB_PjisToKnrec( m_Rgsel[i].debcode, m_pZm );
			pCre = DB_PjisToKnrec( m_Rgsel[i].crecode, m_pZm );
			str.Format( "(仕訳) %-14.14s ／ %-14.14s　の仕訳摘要", pDeb->knnam, pCre->knnam );
			break;
		case RSW_DEBONLY:
			pDeb = DB_PjisToKnrec( m_Rgsel[i].debcode, m_pZm );
			str.Format( "(借方) %-14.14s　の仕訳摘要", pDeb->knnam );
			break;
		case RSW_CREONLY:
			pCre = DB_PjisToKnrec( m_Rgsel[i].crecode, m_pZm );
			str.Format( "(貸方) %-14.14s　の仕訳摘要", pCre->knnam );
			break;
		case RSW_BOTH:	// (借貸) のときは 科目属性によって ｺｰﾄﾞの位置が違う
			if( ! m_Rgsel[i].debcode.IsEmpty() )
				pDeb = DB_PjisToKnrec( m_Rgsel[i].debcode, m_pZm );
			else
				pDeb = DB_PjisToKnrec( m_Rgsel[i].crecode, m_pZm );

			str.Format( "(単独) %-14.14s　の摘要", pDeb->knnam );
			break;
		default:
			endsw = 1;
			break;
		}

		if( !endsw ) {
			GetDlgItem( rdID[i] )->SetWindowText(str);
		}
		else
			GetDlgItem( rdID[i] )->ShowWindow(SW_HIDE);
	}
}


void CSWKselDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);

	m_Selected = m_Rgsel[m_Radio];
//	memcpy( &m_Selected, &m_Rgsel[m_Radio], sizeof(SWK_REGSEL));

	ICSDialog::OnOK();
}



// 仕訳摘要の登録 ｻｲﾝ を ｾｯﾄ
//	SWK_REGSEL* pSel .... バッファ
// 	int max ............. バッファ数
//	BOOL bBoth .......... 両方の登録ありか？
//	------------------------------------------
//	return	0 -------> 登録なし
//			0以外 ---> 登録数
static
int	SetSwkRegSel( SWK_REGSEL* pSel, int max, BOOL bBoth, CDBZmSub* pZm )
{
	int i;
	// 初期化
	for( i = 0; i < max; i++ ) {
		(pSel+i)->debcode = "";
		(pSel+i)->crecode = "";
		(pSel+i)->type = RSW_NONE;
	}

	TKY_REGPAR par = {0};
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	i = 0;
	// 借貸 あり
	if( par.tr_debt[0] && par.tr_cred[0] ) {
		(pSel+i)->debcode = par.tr_debt;
		(pSel+i)->crecode = par.tr_cred;
		(pSel+i)->type = RSW_SIWAKE;
		i++;
	}

	// 借方
	if( par.tr_debt[0] ) {
#ifdef CLOSE // 当面は 借貸 両方のみの登録をサポート
		(pSel+i)->debcode = par.tr_debt;
		(pSel+i)->crecode = 0;
		(pSel+i)->type = RSW_DEBONLY;
		i++;
		(pSel+i)->debcode = 0;
		(pSel+i)->crecode = par.tr_debt;
		(pSel+i)->type = RSW_CREONLY;
		i++;
#endif

		// 借貸 両方の時は 科目属性によってｾｯﾄする位置を買える
		if( bBoth ) {
			DBKNREC* pKn = DB_PjisToKnrec( par.tr_debt, pZm );
			if( !(pKn->knatt & 0x01) ) {
				(pSel+i)->debcode = par.tr_debt;
				(pSel+i)->crecode.Empty();
			}
			else {
				(pSel+i)->debcode.Empty();
				(pSel+i)->crecode = par.tr_debt;
			}

			(pSel+i)->type = RSW_BOTH;
			i++;
		}
	}
	// 貸方
	if( par.tr_cred[0] ) {
#ifdef CLOSE // 当面は 借貸 両方のみの登録をサポート
		(pSel+i)->debcode = par.tr_cred;
		(pSel+i)->crecode = 0;
		(pSel+i)->type = RSW_DEBONLY;
		i++;
		(pSel+i)->debcode = 0;
		(pSel+i)->crecode = par.tr_cred;
		(pSel+i)->type = RSW_CREONLY;
		i++;
#endif

		// 借貸 両方の時は 科目属性によってｾｯﾄする位置を買える
		if( bBoth ) {
			DBKNREC* pKn = DB_PjisToKnrec( par.tr_cred, pZm );
			if( !(pKn->knatt & 0x01) ) {
				(pSel+i)->debcode = par.tr_cred;
				(pSel+i)->crecode.Empty();
			}
			else {
				(pSel+i)->debcode.Empty();
				(pSel+i)->crecode = par.tr_cred;
			}

			(pSel+i)->type = RSW_BOTH;
			i++;
		}
	}

	return i;
}
