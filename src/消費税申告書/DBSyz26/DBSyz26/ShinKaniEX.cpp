// ShinKani.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKaniEX.h"
#include "ShinKaniEXIdx.h"

#include "PrePrint.h"
//2016.02.23 INSERT START
#include "PrePrint2.h"
//2016.02.23 INSERT END
//--> '15.01.27 INS START
#include "H26HyoView.h"
//<-- '15.01.27 INS END


// CShinKaniEX ダイアログ

IMPLEMENT_DYNAMIC(CShinKaniEX, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;

CShinKaniEX::CShinKaniEX(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinKaniEX::IDD, pParent)
	, m_pParent(pParent)
	, m_pSnHonpyoData(NULL)
	, m_pSnFhyo40Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_kanisw(0)
	, m_DpSw(0)
	, m_M1c(0x00)
	, m_curInPos(-1)
{
	memset( XM1, '\0', sizeof(XM1) );
	memset( XM2, '\0', sizeof(XM2) );
	memset( XM3, '\0', sizeof(XM3) );
	memset( XM4, '\0', sizeof(XM4) );
	memset( XM5, '\0', sizeof(XM5) );
	memset( XM6, '\0', sizeof(XM6) );
	memset( XM7, '\0', sizeof(XM7) );
	memset( XM8, '\0', sizeof(XM8) );
	memset( XM9, '\0', sizeof(XM9) );
	memset( XM10, '\0', sizeof(XM10) );
	memset( XM11, '\0', sizeof(XM11) );
	memset( XM12, '\0', sizeof(XM12) );
	memset( XM13, '\0', sizeof(XM13) );
	memset( XM14, '\0', sizeof(XM14) );
	memset( XM15, '\0', sizeof(XM15) );
	memset( SM15, '\0', sizeof(SM15) );
	memset( XM16, '\0', sizeof(XM16) );
	memset( XM17, '\0', sizeof(XM17) );
	memset( XM18, '\0', sizeof(XM18) );
	memset( XM19, '\0', sizeof(XM19) );
	memset( XM20, '\0', sizeof(XM20) );
	memset( XM21, '\0', sizeof(XM21) );
	memset( XM22, '\0', sizeof(XM22) );
	memset( XM23, '\0', sizeof(XM23) );
	memset( XM24, '\0', sizeof(XM24) );
	memset( XM25, '\0', sizeof(XM25) );
	memset( XM26, '\0', sizeof(XM26) );

	SW_WARIFU = SW_NOBEBA = SW_KOUJIS = SW_GENKIN = SW_KISK22 = 0;

	m_curwnd = -1;
}

CShinKaniEX::~CShinKaniEX()
{
}

void CShinKaniEX::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_KaniDiag02);
	DDX_Control(pDX, IDC_ICSDIAGCTRL3, m_KaniDiag03);
	DDX_Control(pDX, IDC_ICSDIAGCTRL4, m_KaniDiag04);
}


BEGIN_MESSAGE_MAP(CShinKaniEX, CSyzBaseDlg)
	ON_BN_CLICKED(IDC_BUTTON1, &CShinKaniEX::OnBnClickedButton1)
END_MESSAGE_MAP()


// CShinKaniEX メッセージ ハンドラ

BOOL CShinKaniEX::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// ダイアグラム初期設定
	ChgDiagAttr();


	// 基底クラスの初期化
	m_pBottomDiag = &m_KaniDiag04;
	m_BottomIdx = 126;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinKaniEX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){

//--> '15.03.02 INS START
			// フォーカス移動
			MoveFocusInEnd();
//<-- '15.03.02 INS END

			// 本表　簡易更新
			GetDiagData();
			calqx_all( 6, 1 );
			WriteData();
//			((CMainFrame *)AfxGetMainWnd())->PostMessageA( WM_CLOSE, 0 );

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
				else if( m_curwnd == IDC_ICSDIAGCTRL3 ) {
					TerminationIcsdiagctrl3( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				else if( m_curwnd == IDC_ICSDIAGCTRL4 ) {
					TerminationIcsdiagctrl4( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}

		//[Y]コンボボックスにフォーカスがある場合の対応
		if( m_curwnd == IDC_ICSDIAGCTRL2 || m_curwnd == IDC_ICSDIAGCTRL3 || m_curwnd == IDC_ICSDIAGCTRL4) {
			if( pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_RETURN ){
				DIAGRAM_ATTRIBUTE        dattr;
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getattr( m_curwnd, result, &dattr, CIcsdiagctrl );
				if( dattr.attr_inpkind == 1 ) {  //コンボボックスにカーソルがある場合はリストを閉じる
					DIAGRAM_DATA   dd;
					m_Util.DiagOcxIniz(&dd);
					diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
					diag_cbdrop( m_curwnd, result, FALSE, CIcsdiagctrl );
					if(m_curwnd == IDC_ICSDIAGCTRL4){
						TerminationIcsdiagctrl4( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
					}
					return TRUE;
				}

			}
		}
		

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnHonpyoData	：	本表データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSnFhyo40Data	：	付表４データ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinKaniEX::InitInfo( CH27SnHonpyoData **pSnHonpyoData, CH27SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo )
//int CShinKaniEX::InitInfo( CH26SnHonpyoData **pSnHonpyoData, CH26SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo )
{
	ASSERT( pSnHonpyoData );
	if( pSnHonpyoData == NULL ){
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT( pSnFhyo40Data );
	if( pSnFhyo40Data == NULL ){
		return -1;
	}
	m_pSnFhyo40Data = pSnFhyo40Data;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKaniEX::Init()
{
	// ダイアグラム初期属性セット
	InitDiagAttr();

	DispInitKani();

//	ChgDiagAttr();//[Y]一般はinitdialogでしているので簡易もそこに移動。（入力サイン関係に関わってくる為）

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKaniEX::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE	DA;

	// ANK交じり対応
	// 納税地　住所(上)
	m_KaniDiag02.GetAttribute( ID2030, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag02.SetAttribute( ID2030, (LPUNKNOWN)&DA, TRUE );
	// 納税地　住所(下)
	m_KaniDiag02.GetAttribute( ID2031, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag02.SetAttribute( ID2031, (LPUNKNOWN)&DA, TRUE );
	// 氏名・屋号
	m_KaniDiag02.GetAttribute( ID2051, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag02.SetAttribute( ID2051, (LPUNKNOWN)&DA, TRUE );
	// 被合併名
	m_KaniDiag02.GetAttribute( ID2052, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag02.SetAttribute( ID2052, (LPUNKNOWN)&DA, TRUE );
	// 代表者氏名
	m_KaniDiag02.GetAttribute( ID2061, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag02.SetAttribute( ID2061, (LPUNKNOWN)&DA, TRUE );
	// 経理担当者
	m_KaniDiag02.GetAttribute( ID2070, (LPUNKNOWN)&DA);
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag02.SetAttribute( ID2070, (LPUNKNOWN)&DA, TRUE );


	// ANK混じり設定
	// 銀行名
	m_KaniDiag04.GetAttribute( ID4210, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag04.SetAttribute( ID4210, (LPUNKNOWN)&DA, FALSE );
	// 銀行支店名
	m_KaniDiag04.GetAttribute( ID4211, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag04.SetAttribute( ID4211, (LPUNKNOWN)&DA, FALSE );
	// 郵便局名
	m_KaniDiag04.GetAttribute( ID4312, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_KaniDiag04.SetAttribute( ID4312, (LPUNKNOWN)&DA, FALSE );

	m_KaniDiag02.EnableDelete(0);//[Y]DELETEキー１回で削除モード設定
	m_KaniDiag03.EnableDelete(0);//[Y]DELETEキー１回で削除モード設定
	m_KaniDiag04.EnableDelete(0);//[Y]DELETEキー１回で削除モード設定

	return 0;
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKaniEX::ReadData()
{
	char	WORK0[MONY_BUF_SIZE],
			WORK1[MONY_BUF_SIZE],
			WORK2[MONY_BUF_SIZE];

	m_Util.l_calq( (char*)WORK0, (char *)(*m_pSnFhyo40Data)->Sn_1F1A, 0 );
	m_Util.l_calq( (char*)WORK1, (char *)(*m_pSnFhyo40Data)->Sn_1F1B, 0 );
	m_Util.l_calq( (char*)WORK2, (char *)(*m_pSnFhyo40Data)->Sn_1F1C, 0 );

	if( (*m_pSnHeadData)->SVmzsw ){
		// 課税標準額
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSTD, WORK0, WORK1 );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSTD, (*m_pSnHonpyoData)->Sn_KSTD, WORK2 );
		// 消費税額
		if(!((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){//消費税額手入力されていない場合のみ[Y]
			m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnFhyo40Data)->Sn_1F2A, (*m_pSnFhyo40Data)->Sn_1F2B );
			m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnFhyo40Data)->Sn_1F2C );
		}
		// 貸倒回収
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnFhyo40Data)->Sn_1F3A, (*m_pSnFhyo40Data)->Sn_1F3B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnFhyo40Data)->Sn_1F3C );
		// 控除対象仕入税額
//-- '14.10.17 --
//		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnFhyo40Data)->Sn_1F4A, (*m_pSnFhyo40Data)->Sn_1F4B );
//		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnFhyo40Data)->Sn_1F4C );
//---------------
		m_pArith->l_clear( (*m_pSnHonpyoData)->Sn_SIREZ );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnFhyo40Data)->Sn_1F4T );
//---------------
		// 変換等対価にかかる税額
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo40Data)->Sn_1F5A, (*m_pSnFhyo40Data)->Sn_1F5B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo40Data)->Sn_1F5C );
		// 貸倒に係る税額
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnFhyo40Data)->Sn_1F6A, (*m_pSnFhyo40Data)->Sn_1F6B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnFhyo40Data)->Sn_1F6C );
	}
	else{
		memmove( (*m_pSnHonpyoData)->Sn_KSTD, WORK2, MONY_BUF_SIZE );
		if( !((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){//消費税額手入力されていない場合のみ[Y]
			memmove( (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnFhyo40Data)->Sn_1F2C, MONY_BUF_SIZE );
		}
		memmove( (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnFhyo40Data)->Sn_1F3C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnFhyo40Data)->Sn_1F4C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo40Data)->Sn_1F5C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnFhyo40Data)->Sn_1F6C, MONY_BUF_SIZE );
	}

	// 1.課税標準額
	memmove( XM1, (*m_pSnHonpyoData)->Sn_KSTD, MONY_BUF_SIZE );
	// 2.消費税額
	memmove( XM2, (*m_pSnHonpyoData)->Sn_SYTX, MONY_BUF_SIZE );
	m_M1c = (*m_pSnHonpyoData)->Sn_SYTsw;//[Y]

	// 3.控除過大調整税額
	memmove( XM3, (*m_pSnHonpyoData)->Sn_KJADJZ, MONY_BUF_SIZE );
	// 4.控除対象仕入税額
	memmove( XM4, (*m_pSnHonpyoData)->Sn_SIREZ, MONY_BUF_SIZE );
	// 5.返還等対価に係る税額
	memmove( XM5, (*m_pSnHonpyoData)->Sn_HKANZ, MONY_BUF_SIZE );
	// 6.貸倒に係る税額
	memmove( XM6, (*m_pSnHonpyoData)->Sn_KSITZ, MONY_BUF_SIZE );
	// 限界控除税額
	// 10.中間納付税額
	if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
		memset( (*m_pSnHonpyoData)->Sn_TYNOFZ, '\0', MONY_BUF_SIZE );
	}

	memmove( XM10, (*m_pSnHonpyoData)->Sn_TYNOFZ, MONY_BUF_SIZE );
	// 13.既確定税額
	memmove( XM13, (*m_pSnHonpyoData)->Sn_KAKTIZ, MONY_BUF_SIZE );	
	// 課税売上高
	memmove( XM15, (*m_pSnHonpyoData)->Sn_KZURI, MONY_BUF_SIZE );
	memmove( SM15, (*m_pSnHonpyoData)->Sn_KZURI, MONY_BUF_SIZE );
	if( !m_kanisw ){	// 総売上高
		memmove( XM16, (*m_pSnHonpyoData)->Sn_SOURI, MONY_BUF_SIZE );
	}
	else{			// 前々回基準期間の課税売上高
		memmove( XM16, (*m_pSnHonpyoData)->Sn_ZZURI, MONY_BUF_SIZE );
	}
	// 21.中間納付譲渡割額
	if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
		memset( (*m_pSnHonpyoData)->Sn_TTYWAR, '\0', MONY_BUF_SIZE );
	}

	memmove( XM21, (*m_pSnHonpyoData)->Sn_TTYWAR, MONY_BUF_SIZE );
	// 24.既確定税額
	memmove( XM24, (*m_pSnHonpyoData)->Sn_TKAKTZ, MONY_BUF_SIZE );

	SW_WARIFU = (int)((*m_pSnHeadData)->Sn_KAPPU&0xff);	// 割賦基準の適用
	SW_NOBEBA = (int)((*m_pSnHeadData)->Sn_NOBE&0xff);		// 延払基準の適用
	SW_KOUJIS = (int)((*m_pSnHeadData)->Sn_KOJI&0xff);		// 工事進行基準の適用
	SW_GENKIN = (int)((*m_pSnHeadData)->Sn_GENKN&0xff);	// 現金主義会計の適用
	SW_KISK22 = (int)((*m_pSnHeadData)->Sn_TUMIAG&0x01);	// 規則22条１項の適用

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKaniEX::WriteData()
{
	char	WORK0[MONY_BUF_SIZE],
			WORK1[MONY_BUF_SIZE];

	(*m_pSnHeadData)->Sn_KAPPU = (char) SW_WARIFU;
	(*m_pSnHeadData)->Sn_NOBE  = (char) SW_NOBEBA;
	(*m_pSnHeadData)->Sn_KOJI  = (char) SW_KOUJIS;
	(*m_pSnHeadData)->Sn_GENKN = (char) SW_GENKIN;
	(*m_pSnHeadData)->Sn_TUMIAG &= 0xfe;
	if( SW_KISK22 ){
		(*m_pSnHeadData)->Sn_TUMIAG |= 0x01;
	}

	// 1.課税標準額
	memmove( (*m_pSnHonpyoData)->Sn_KSTD, XM1, MONY_BUF_SIZE );
	// 2.消費税額
	memmove( (*m_pSnHonpyoData)->Sn_SYTX, XM2, MONY_BUF_SIZE );
	//付表５、（付表４1F2C）の金額も同時にセットする
//	memmove( (*m_pSnFhyo40Data)->Sn_1F2C , (*m_pSnHonpyoData)->Sn_SYTX , MONY_BUF_SIZE );

	(*m_pSnHonpyoData)->Sn_SYTsw = m_M1c;
	// 3.控除過大調整税額
	memmove( (*m_pSnHonpyoData)->Sn_KJADJZ, XM3, MONY_BUF_SIZE );
	// 4.控除対象仕入額
	memmove( (*m_pSnHonpyoData)->Sn_SIREZ, XM4, MONY_BUF_SIZE );
	// 5.返還等対価に係る税額
	memmove( (*m_pSnHonpyoData)->Sn_HKANZ, XM5, MONY_BUF_SIZE );
	// 6.貸倒れに係る税額
	memmove( (*m_pSnHonpyoData)->Sn_KSITZ, XM6, MONY_BUF_SIZE );
	// 10.中間納付税額
	memmove( (*m_pSnHonpyoData)->Sn_TYNOFZ, XM10, MONY_BUF_SIZE );
	// 13.既確定税額
	memmove( (*m_pSnHonpyoData)->Sn_KAKTIZ, XM13, MONY_BUF_SIZE );
///////////////////	
//	// 限界控除適用上限チェック
//	if( pSyzShin->N_ver != 2 && (l_cmp( SM15, XM15 ) || l_cmp( pSyzShin->Snd.Sn_KZURI, XM15 )) )
//												Kanigenup_( XM15 );
///////////////////
	// 課税売上高
	memmove( (*m_pSnHonpyoData)->Sn_KZURI,  XM15, MONY_BUF_SIZE );
	
	// 非課税売上額
	if( ! m_kanisw ) {
		// 総売上高
		memmove( (*m_pSnHonpyoData)->Sn_SOURI, XM16, MONY_BUF_SIZE );
		memset( WORK0, '\0', MONY_BUF_SIZE );	memset( WORK1, '\0', MONY_BUF_SIZE );
		// 課税売上高割合
		if( !((*m_pSnHeadData)->Sn_UPERS & 0x10 ) )	{
			char	tmpbuf[64]={0};
			sprintf_s( tmpbuf, _T("%d"), (*m_pSnHeadData)->m_UriRatioBorder );
			m_pArith->l_input( WORK1, tmpbuf );
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)(*m_pSnHonpyoData)->Sn_KZURI, (unsigned char *)(*m_pSnHonpyoData)->Sn_SOURI, 2, 0 );
			// 売上割合９５％以上の場合の仕入控除方式の出力ＳＷ
			(*m_pSnHeadData)->Sn_UPERS &= 0xfe;
			if( m_pArith->l_cmp(WORK0, WORK1) < 0 ){
				(*m_pSnHeadData)->Sn_UPERS |= 0x01;	// 95%未満
			}
		}
	}
	else{	// 基準期間の課税売上高
		memmove( (*m_pSnHonpyoData)->Sn_ZZURI, XM16, MONY_BUF_SIZE );
	}
	// 最終納付税額・書き込み
	memmove( (*m_pSnHonpyoData)->Sn_EDNOFZ, XM11, MONY_BUF_SIZE );

	// 地方消費税
	// 21.中間納付譲渡割額
	memmove( (*m_pSnHonpyoData)->Sn_TTYWAR, XM21, MONY_BUF_SIZE );
	// 24.既確定税額
	memmove( (*m_pSnHonpyoData)->Sn_TKAKTZ, XM24, MONY_BUF_SIZE );
	// 地方消費税　最終納付譲渡割額・書き込み
	memmove( (*m_pSnHonpyoData)->Sn_TEDNOF, XM22, MONY_BUF_SIZE );

	// 年３回中間申告時の各回納付税額・書き込み 
	switch( (*m_pSnHeadData)->Sn_MCOUNT ){
		case 3 :	// 年３回
			switch( (*m_pSnHeadData)->Sn_JCOUNT ){
				case 1 :
//					memmove( pSyzShin->Snd.Sn_TYNF1,  XM11, MONY_BUF_SIZE );
//					memmove( pSyzShin->Snd.Sn_TTYNF1, XM22, MONY_BUF_SIZE );
					break;
				case 2 :
//					memmove( pSyzShin->Snd.Sn_TYNF2,  XM11, MONY_BUF_SIZE );
//					memmove( pSyzShin->Snd.Sn_TTYNF2, XM22, MONY_BUF_SIZE );
					break;
				case 3 :
//					memmove( pSyzShin->Snd.Sn_TYNF3,  XM11, MONY_BUF_SIZE );
//					memmove( pSyzShin->Snd.Sn_TTYNF3, XM22, MONY_BUF_SIZE );
					break;
				case 4 :
//					memmove( pSyzShin->Snd.Sn_TYNF4,  XM11, MONY_BUF_SIZE );
//					memmove( pSyzShin->Snd.Sn_TTYNF4, XM22, MONY_BUF_SIZE );
					break;
				default:
					break;
			}
			break;
		case 11:	// 年11回
//			memmove( pSyzShin->Snd.Sn_TYNF[ (*m_pSnHonpyoData)->Sn_JCOUNT - 1 ], XM11, MONY_BUF_SIZE );
//			memmove( pSyzShin->Snd.Sn_TTNF[ (*m_pSnHonpyoData)->Sn_JCOUNT - 1 ], XM22, MONY_BUF_SIZE );
			break;
		default:	// 年１回
			break;
	}
	
	if( !(*m_pSnHeadData)->SVmzsw ){		// 経過措置無し
		memmove( (*m_pSnFhyo40Data)->Sn_1F1C,	(*m_pSnHonpyoData)->Sn_KSTD,   MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F2C,	(*m_pSnHonpyoData)->Sn_SYTX,   MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F3C, (*m_pSnHonpyoData)->Sn_KJADJZ, MONY_BUF_SIZE );
//		memmove( (*m_pSnFhyo40Data)->Sn_1F8B, (*m_pSnHonpyoData)->Sn_SIREZ,  MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4C, (*m_pSnHonpyoData)->Sn_SIREZ,  MONY_BUF_SIZE );
//		memmove( (*m_pSnFhyo40Data)->Sn_1F9B, (*m_pSnHonpyoData)->Sn_HKANZ,  MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F5C, (*m_pSnHonpyoData)->Sn_HKANZ,  MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F6C, (*m_pSnHonpyoData)->Sn_KSITZ,  MONY_BUF_SIZE );
	}
	else {
		if( ! m_pArith->l_test( (*m_pSnHonpyoData)->Sn_KSTD ) ){
			memset( (*m_pSnFhyo40Data)->Sn_1F1A, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F1B, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F1C, '\0', MONY_BUF_SIZE );
		}
		if( ! m_pArith->l_test( (*m_pSnHonpyoData)->Sn_SYTX ) ){
			memset( (*m_pSnFhyo40Data)->Sn_1F2A, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F2B, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F2C, '\0', MONY_BUF_SIZE );
		}
		if( ! m_pArith->l_test( (*m_pSnHonpyoData)->Sn_KJADJZ ) ) {
			memset( (*m_pSnFhyo40Data)->Sn_1F3A, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F3B, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F3C, '\0', MONY_BUF_SIZE );
		}
//		memmove( (*m_pSnFhyo40Data)->Sn_1F8C, (*m_pSnHonpyoData)->Sn_SIREZ, MONY_BUF_SIZE );
		if( ! m_pArith->l_test( (*m_pSnHonpyoData)->Sn_SIREZ ) ) {
//-- '15.01.27 --
//			memset( (*m_pSnFhyo40Data)->Sn_1F4A, '\0', MONY_BUF_SIZE );
//			memset( (*m_pSnFhyo40Data)->Sn_1F4B, '\0', MONY_BUF_SIZE );
//			memset( (*m_pSnFhyo40Data)->Sn_1F4C, '\0', MONY_BUF_SIZE );
//---------------
			if( judge_skj_calq() == 0 ){
				memset( (*m_pSnFhyo40Data)->Sn_1F4A, '\0', MONY_BUF_SIZE );
				memset( (*m_pSnFhyo40Data)->Sn_1F4B, '\0', MONY_BUF_SIZE );
				memset( (*m_pSnFhyo40Data)->Sn_1F4C, '\0', MONY_BUF_SIZE );
			}
//---------------
		}
//		memmove( pSyzShin->Snd.Sn_1F9C, pSyzShin->Snd.Sn_HKANZ, MONY_BUF_SIZE );
		if( ! m_pArith->l_test( (*m_pSnHonpyoData)->Sn_HKANZ ) ) {
			memset( (*m_pSnFhyo40Data)->Sn_1F5A, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F5B, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F5C, '\0', MONY_BUF_SIZE );
		}
		if( ! m_pArith->l_test( (*m_pSnHonpyoData)->Sn_KSITZ ) ) {
			memset( (*m_pSnFhyo40Data)->Sn_1F6A, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F6B, '\0', MONY_BUF_SIZE );
			memset( (*m_pSnFhyo40Data)->Sn_1F6C, '\0', MONY_BUF_SIZE );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 初期データ表示
//-----------------------------------------------------------------------------
void CShinKaniEX::DispInitKani()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			buf[512] = {0};

	// 作成年月日
	char	yy=0, mm=0, dd=0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_PRESENT_DATE, &yy, &mm, &dd );
	if( (yy&0xff) ){
		DiagData.data_day[0] = yy;
		m_KaniDiag02.SetData( ID2000, (struct IUnknown *)&DiagData );
	}
	if( (mm&0xff) ) {
		DiagData.data_day[0] = mm;
		m_KaniDiag02.SetData( ID2001, (struct IUnknown *)&DiagData );
	}
	if( (dd&0xff) ) {
		DiagData.data_day[0] = dd;
		m_KaniDiag02.SetData( ID2002, (struct IUnknown *)&DiagData );
	}
	// 税務署名
	memset( buf, '\0', sizeof(buf) );
	int len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
	memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2010, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	if( (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!=0x00) && (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!='F') ){
		sprintf_s( buf, sizeof(buf), _T("署番号 %-5.5s"), (*m_pSnHeadData)->Sn_TAXNO );
	}
	else{
		sprintf_s( buf, sizeof(buf), _T("署番号未登録") );
	}
	DiagData.data_disp = buf;
	m_KaniDiag02.SetData( ID2011, (struct IUnknown *)&DiagData );
	// 納税地　郵便番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_ZIP, 8 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2020, (struct IUnknown *)&DiagData );
	// 納税地　住所(上)
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_NOZEI, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_NOZEI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2030, (struct IUnknown *)&DiagData );
	// 納税地　住所(下)
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)&(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
	memmove( buf, &(*m_pSnHeadData)->Sn_NOZEI[40], len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2031, (struct IUnknown *)&DiagData );
	// 納税地　電話番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO1, 6 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2040, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO2, 4 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2041, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO3, 4 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2042, (struct IUnknown *)&DiagData );
	// 氏名・屋号　フリガナ
	memset( buf, '\0', sizeof(buf) );
//2016.10.20 UPDATE START
//	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_COFRI, 68 );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_COFRI, 80 );
//2016.10.20 UPDATE END
	memmove( buf, (*m_pSnHeadData)->Sn_COFRI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2050, (struct IUnknown *)&DiagData );
	// 氏名・屋号
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_CONAM, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_CONAM, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2051, (struct IUnknown *)&DiagData );
	// 被合併名
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_GPNAM, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_GPNAM, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2052, (struct IUnknown *)&DiagData );
	// 代表者氏名　フリガナ
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIFRI, 68 );
	memmove( buf, (*m_pSnHeadData)->Sn_DIFRI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2060, (struct IUnknown *)&DiagData );
	// 代表者氏名
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIHYO, 40 );
	memmove( buf,(*m_pSnHeadData)->Sn_DIHYO, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2061, (struct IUnknown *)&DiagData );
	// 経理担当者
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_KEIRI, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_KEIRI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2070, (struct IUnknown *)&DiagData );

	//== 税務署処理･整理欄 ==
	// 一連番号
	if( ((*m_pSnHeadData)->Sn_ITIREN[0]&0xff) != 0xff ) {
		memset( buf, '\0', sizeof(buf) );
		m_pArith->l_unpac( buf, (*m_pSnHeadData)->Sn_ITIREN, 8 );
		DiagData.data_edit = buf;
		m_KaniDiag02.SetData( ID2080, (struct IUnknown *)&DiagData );
	}
	// 送付　要・不要
	DiagData.data_check = (short)( (*m_pSnHeadData)->Sn_ZeiHou&0x04 );
	m_KaniDiag02.SetData( ID2081, (struct IUnknown *)&DiagData );
	// 所管
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_SYOKN, 3 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2082, (struct IUnknown *)&DiagData );
	// 整理番号
	if( ((*m_pSnHeadData)->Sn_SEIRI[0]&0xff) != 0xff ) {
		memset( buf, '\0', sizeof(buf) );
		m_pArith->l_unpac( buf, (*m_pSnHeadData)->Sn_SEIRI, 8 );
		DiagData.data_edit = buf;
		m_KaniDiag02.SetData( ID2083, (struct IUnknown *)&DiagData );
	}
	// 課税期間
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){ 
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2100, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2101, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2102, (struct IUnknown *)&DiagData );

	}
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2110, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf)  );
		sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2111, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2112, (struct IUnknown *)&DiagData );
	}
	// 申告区分
	memset( buf, '\0', sizeof(buf) );
	switch( (*m_pSnHeadData)->Sn_SKKBN ) {
		case 1 :
//2018.03.13 UPDATE START
//			memmove( buf, _T("　確定　"), 8 );
			if( (*m_pSnHeadData)->Sn_Syuusei&0x10 ){
					memmove( buf, _T(" 準確定 "), 8 );
			}else{
					memmove( buf, _T("　確定　"), 8 );
			}
//2018.03.13 UPDATE END
			break;
		case 2 :
			memmove( buf, _T("　中間　"), 8 );
			break;
		case 3 :
			memmove( buf, _T("修正確定"), 8 );
			break;
		default:
			memmove( buf, _T("修正中間"), 8 );
			break;
	}
	DiagData.data_disp = buf;
	m_KaniDiag02.SetData( ID2120, (struct IUnknown *)&DiagData );

	if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) )	{
		// 中間期間
		yy = mm = dd = 0;
		(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd );
		if( (yy&0xff) && (mm&0xff) && (dd&0xff) ) { 
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2130, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2131, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2132, (struct IUnknown *)&DiagData );
		}

		yy = mm = dd = 0;
		(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd );
		if( (yy&0xff) && (mm&0xff) && (dd&0xff) ) { 
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2140, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2141, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2142, (struct IUnknown *)&DiagData );
		}
	}

	//== 金額＆付記事項等画面 ==
	// 簡易課税
	if( (m_kanisw = (int )((*m_pSnHeadData)->m_s_sgn2&0x0f)) == 1 ){
		m_kanisw = 0;	// 業種区分
	}
	// データの読込み
	ReadData();
	// 画面出力
	DispKani();

	// 納税署整理番号
	char	cbuf[128]={0};
	if( ((*m_pSnHeadData)->Sn_ZSEIRI[0]&0xff) != 0xff ) {
		if( ((*m_pSnHeadData)->Sn_ZSEIRI[3]&0x0f) != 0x0f ){	// 8桁入力済み
			m_pArith->l_unpac( cbuf, (*m_pSnHeadData)->Sn_ZSEIRI, 8 );
			if( cbuf[0] != 0x30 ){							// 先頭が0以外は後1桁カット
				cbuf[7] = '\0';
			}
			else{
				int ItCt;
				for( ItCt = 0; ItCt < 7; ++ItCt ){
					cbuf[ItCt] = cbuf[ItCt+1];
				}
				cbuf[ItCt] = '\0';
			}
			memset( (*m_pSnHeadData)->Sn_ZSEIRI, '\0', 4 );
			m_pArith->l_pack( (*m_pSnHeadData)->Sn_ZSEIRI, cbuf, 7 );
		}
		memset( buf, '\0', sizeof(buf) );
		m_pArith->l_unpac( buf, (*m_pSnHeadData)->Sn_ZSEIRI, 8 );
		DiagData.data_edit = buf;
		m_KaniDiag04.SetData( ID4RNO, (struct IUnknown *)&DiagData );
	}
}

//2016.10.20 INSERT START
//-----------------------------------------------------------------------------
// 初期データ表示
//-----------------------------------------------------------------------------
void CShinKaniEX::DispInitKaniUpper()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			buf[512] = {0};
	char			cbuf[128]={0};

	// 作成年月日
	char	yy=0, mm=0, dd=0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_PRESENT_DATE, &yy, &mm, &dd );
	if( (yy&0xff) ){
		DiagData.data_day[0] = yy;
		m_KaniDiag02.SetData( ID2000, (struct IUnknown *)&DiagData );
	}
	if( (mm&0xff) ) {
		DiagData.data_day[0] = mm;
		m_KaniDiag02.SetData( ID2001, (struct IUnknown *)&DiagData );
	}
	if( (dd&0xff) ) {
		DiagData.data_day[0] = dd;
		m_KaniDiag02.SetData( ID2002, (struct IUnknown *)&DiagData );
	}
	// 税務署名
	memset( buf, '\0', sizeof(buf) );
	int len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
	memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2010, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	if( (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!=0x00) && (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!='F') ){
		sprintf_s( buf, sizeof(buf), _T("署番号 %-5.5s"), (*m_pSnHeadData)->Sn_TAXNO );
	}
	else{
		sprintf_s( buf, sizeof(buf), _T("署番号未登録") );
	}
	DiagData.data_disp = buf;
	m_KaniDiag02.SetData( ID2011, (struct IUnknown *)&DiagData );
	// 納税地　郵便番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_ZIP, 8 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2020, (struct IUnknown *)&DiagData );
	// 納税地　住所(上)
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_NOZEI, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_NOZEI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2030, (struct IUnknown *)&DiagData );
	// 納税地　住所(下)
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)&(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
	memmove( buf, &(*m_pSnHeadData)->Sn_NOZEI[40], len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2031, (struct IUnknown *)&DiagData );
	// 納税地　電話番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO1, 6 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2040, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO2, 4 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2041, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO3, 4 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2042, (struct IUnknown *)&DiagData );
	// 氏名・屋号　フリガナ
	memset( buf, '\0', sizeof(buf) );
//2016.10.20 UPDATE START
//	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_COFRI, 68 );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_COFRI, 80 );
//2016.10.20 UPDATE END
	memmove( buf, (*m_pSnHeadData)->Sn_COFRI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2050, (struct IUnknown *)&DiagData );
	// 氏名・屋号
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_CONAM, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_CONAM, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2051, (struct IUnknown *)&DiagData );
	// 被合併名
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_GPNAM, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_GPNAM, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2052, (struct IUnknown *)&DiagData );
	// 代表者氏名　フリガナ
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIFRI, 68 );
	memmove( buf, (*m_pSnHeadData)->Sn_DIFRI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2060, (struct IUnknown *)&DiagData );
	// 代表者氏名
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIHYO, 40 );
	memmove( buf,(*m_pSnHeadData)->Sn_DIHYO, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2061, (struct IUnknown *)&DiagData );
	// 経理担当者
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_KEIRI, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_KEIRI, len );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2070, (struct IUnknown *)&DiagData );

	//== 税務署処理･整理欄 ==
	// 一連番号
	if( ((*m_pSnHeadData)->Sn_ITIREN[0]&0xff) != 0xff ) {
		memset( buf, '\0', sizeof(buf) );
		m_pArith->l_unpac( buf, (*m_pSnHeadData)->Sn_ITIREN, 8 );
		DiagData.data_edit = buf;
		m_KaniDiag02.SetData( ID2080, (struct IUnknown *)&DiagData );
	}
	// 送付　要・不要
	DiagData.data_check = (short)( (*m_pSnHeadData)->Sn_ZeiHou&0x04 );
	m_KaniDiag02.SetData( ID2081, (struct IUnknown *)&DiagData );
	// 所管
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_SYOKN, 3 );
	DiagData.data_edit = buf;
	m_KaniDiag02.SetData( ID2082, (struct IUnknown *)&DiagData );
	// 整理番号
	if( ((*m_pSnHeadData)->Sn_SEIRI[0]&0xff) != 0xff ) {
		memset( buf, '\0', sizeof(buf) );
		m_pArith->l_unpac( buf, (*m_pSnHeadData)->Sn_SEIRI, 8 );
		DiagData.data_edit = buf;
		m_KaniDiag02.SetData( ID2083, (struct IUnknown *)&DiagData );
	}
	// 課税期間
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){ 
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2100, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2101, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2102, (struct IUnknown *)&DiagData );

	}
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2110, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf)  );
		sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2111, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
		DiagData.data_disp = buf;
		m_KaniDiag02.SetData( ID2112, (struct IUnknown *)&DiagData );
	}
	// 申告区分
	memset( buf, '\0', sizeof(buf) );
	switch( (*m_pSnHeadData)->Sn_SKKBN ) {
		case 1 :
			memmove( buf, _T("　確定　"), 8 );
			break;
		case 2 :
			memmove( buf, _T("　中間　"), 8 );
			break;
		case 3 :
			memmove( buf, _T("修正確定"), 8 );
			break;
		default:
			memmove( buf, _T("修正中間"), 8 );
			break;
	}
	DiagData.data_disp = buf;
	m_KaniDiag02.SetData( ID2120, (struct IUnknown *)&DiagData );

	if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) )	{
		// 中間期間
		yy = mm = dd = 0;
		(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd );
		if( (yy&0xff) && (mm&0xff) && (dd&0xff) ) { 
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2130, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2131, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2132, (struct IUnknown *)&DiagData );
		}

		yy = mm = dd = 0;
		(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd );
		if( (yy&0xff) && (mm&0xff) && (dd&0xff) ) { 
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2140, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2141, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2142, (struct IUnknown *)&DiagData );
		}
	}

	// 金融機関 名
	memset( buf, '\0', sizeof(buf) );
	int KjLen = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_BANK, 20 );
	memmove( buf, (*m_pSnHeadData)->Sn_BANK, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4210, (struct IUnknown *)&DiagData );

	// 銀行・金庫等
	DiagData.data_combo = (short )((*m_pSnHeadData)->Sn_FINANCE&0x0f);
	m_KaniDiag04.SetData( ID4212, (struct IUnknown *)&DiagData );

	// 金融機関 支店名
	memset( buf, '\0', sizeof(buf) );
	KjLen = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_SITEN, 20 );
	memmove( buf, (*m_pSnHeadData)->Sn_SITEN, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4211, (struct IUnknown *)&DiagData );

	// 本社・支社等
	DiagData.data_combo = (short )((*m_pSnHeadData)->Sn_CHAIN&0x0f);
	m_KaniDiag04.SetData( ID4213, (struct IUnknown *)&DiagData );

	// 金融機関 預金種別
	memset( buf, '\0', sizeof(buf) );
	KjLen = kjlen( (*m_pSnHeadData)->Sn_YOKIN, 8 );
	memmove( buf, (*m_pSnHeadData)->Sn_YOKIN, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4214, (struct IUnknown *)&DiagData );

	// 金融機関 口座番号
	if( ((*m_pSnHeadData)->Sn_KOZA[0]&0xff) != 0xff ) {
		memmove( cbuf, (*m_pSnHeadData)->Sn_KOZA, 5 );
		l_unpac( buf, cbuf, 10 );
		DiagData.data_edit = (char *)buf;
		m_KaniDiag04.SetData( ID4215, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4215 );
	}

	// 郵便局 名
	memset( buf, '\0', sizeof(buf) );
	KjLen = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_YUBIN, 16 );
	memmove( buf, (*m_pSnHeadData)->Sn_YUBIN, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData(  ID4312, (struct IUnknown *)&DiagData );

	// 銀行？郵便局
	if( (*m_pSnHeadData)->Sn_BYsw ){
		DiagData.data_combo = 1;
	}
	else{
		DiagData.data_combo = 0;
	}
	m_KaniDiag04.SetData(  ID4410, (struct IUnknown *)&DiagData );
	
	// 郵便局 預金番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_YUNUM1, 5 );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4310, (struct IUnknown *)&DiagData );

	// 郵便局 預金番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_YUNUM2, 8 );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4311, (struct IUnknown *)&DiagData );

	// 税理士情報の画面表示は全て表示する（印刷情報と連動しない）
	if( m_pShinInfo->isExistOffice ){
		int	i, j, k;
		// 事務所名又は法人名
		memset( buf, '\0', sizeof(buf));
		memcpy( buf, m_pShinInfo->pO_DATA->o_ofcnam, sizeof( m_pShinInfo->pO_DATA->o_ofcnam ));
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4ZI0, (struct IUnknown *)&DiagData );
		// 担当税理士氏名
		memset( buf, '\0', sizeof(buf));
		if( (i = (int)strlen(  m_pShinInfo->pZ_DATA->z_zeinam )) > 40 )	i = 40;
		memcpy( buf, m_pShinInfo->pZ_DATA->z_zeinam, i );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4ZI1, (struct IUnknown *)&DiagData );
		// 電話番号
		memset( buf, '\0', sizeof(buf) );
		for( j = 0; j < 18; ++j )	buf[j] = 0x20;
		for( j = k = 0; j < 30 || m_pShinInfo->pO_DATA->o_telno[j]; ++j ) {
			if( m_pShinInfo->pO_DATA->o_telno[j] == '-' || m_pShinInfo->pO_DATA->o_telno[j] == 'ｰ' ) {
				switch( k ) {
					case 0 :
						k = ( (j < 6) ? j : 6 );
						memmove( &buf[6-k], &m_pShinInfo->pO_DATA->o_telno[0], k );
						k = 1;	i = j + 1;
						continue;
					case 1 :
						if( (j-i) < 5 )	k = (j - i);
						else			k = 5;
						memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[i],   k );
						memmove( &buf[13], &m_pShinInfo->pO_DATA->o_telno[++j], 5 );
						k = 2;
						break;
					default:	break;
				}
				if( k > 1 )	break;
			}
		}
		if( ! k )	memmove( buf, &m_pShinInfo->pO_DATA->o_telno[0], (j > 18 ? 18 : j) );
		else	{
			if( k == 1 ) {
				memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[i], 4 );
			}
		}
		DiagData.data_disp = (char *)buf;
		m_KaniDiag04.SetData(  ID4ZI2, (struct IUnknown *)&DiagData );

	}
	else	{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4ZI0 );
		dsp_cls( IDC_ICSDIAGCTRL4, ID4ZI1 );
		dsp_cls( IDC_ICSDIAGCTRL4, ID4ZI2 );
	}
}
//2016.10.20 INSERT END

//-----------------------------------------------------------------------------
// 本表：金額欄表示
//-----------------------------------------------------------------------------
void CShinKaniEX::DispKani()
{
	char	Mny[MONY_BUF_SIZE];
	DIAGRAM_ATTRIBUTE	DA;

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]連動の場合手入力サイン初期化
		//手入力サイン初期化
		m_M1c = 0;
	}


	// 画面初期化
	for( int i=ID3XM1; i<=ID3XM26; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL3, (short )i );
	}

	// １．課税標準額
	calqx_1();
	// ２．消費税額
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM2, XM2 );
	// ３．控除過大調整税額 or 貸倒回収に係る税額
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM3, XM3 );
	// ４．控除対象仕入税額
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM4, XM4 );
	// ５．返還等対価に係る税額
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM5, XM5 );
	// ６．貸倒に係る税額
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM6, XM6 );
	// ７．控除税額小計
	calqx_7( 0 );
	// ８．控除不足還付税額 ･ ９．差引税額
	calqx_89( 0 );
	//１０．中間納付税額
	calqx_10();
	//１１．納付税額・１２．中間納付還付税額
	calqx_1112( 0 );
	//１３．既確定税額
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM13, XM13 );
	//１４．差引納付税額
	calqx_14( 0 );
	//１５．課税売上高
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM15, XM15 );
	// １６．総売上高 ･ 基準期間の課税売上
	dsp_prs( IDC_ICSDIAGCTRL3, ID3XM16, XM16 );
	// 地方消費税
	// 中間且改正日前開始は地方消費税入力無し
//	if( ! (((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) && pSyzShin->N_ver != 2 ) ;
	if( 0 ){
		;
	}
	else{
		// １７．控除不足還付税額
		calqx_17( 0 );
		//１８．差引税額
		calqx_18( 0 );
		//１９．譲渡割額 還付額･納税額	
		calqx_1920( 0 );
		//２１．中間納付譲渡割額
		calqx_21();
		//２２．納付譲渡割額・２３．中間還付譲渡割額
		calqx_2223( 0 );
		//２４．既確定譲渡割額
		dsp_prs( IDC_ICSDIAGCTRL3, ID3XM24, XM24 );
		//２５．差引納付譲渡割額
		calqx_25( 0 );
	}
	//２６．消費税及び地方消費税の合計税額
	calqx_26( 0 );

//★//[12'05.31]///
// 税額計算　入力切替え
//	Kani_Inp( 0 );
///////////////////

	//== 付記・参考 ==
	for( int i=ID4000; i<=ID4ZI2; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL4, (short )i );
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 適用ＳＷのＳＥＴ
	SW_WARIFU = (int)((*m_pSnHeadData)->Sn_KAPPU&0xff);	// 割賦基準の適用
	if( SW_WARIFU ){
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4000, (struct IUnknown *)&DiagData );
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4001, (struct IUnknown *)&DiagData );
	}
	else{
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4000, (struct IUnknown *)&DiagData );
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4001, (struct IUnknown *)&DiagData );
	}

	SW_NOBEBA = (int)((*m_pSnHeadData)->Sn_NOBE&0xff);	// 延払基準の適用
	if( SW_NOBEBA ) {
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4010, (struct IUnknown *)&DiagData );
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4011, (struct IUnknown *)&DiagData );
	}
	else{
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4010, (struct IUnknown *)&DiagData );
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4011, (struct IUnknown *)&DiagData );
	}

	SW_KOUJIS = (int)((*m_pSnHeadData)->Sn_KOJI&0xff);	// 工事進行基準の適用
	if( SW_KOUJIS ) {
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4020, (struct IUnknown *)&DiagData );
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4021, (struct IUnknown *)&DiagData );
	}
	else{
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4020, (struct IUnknown *)&DiagData );
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4021, (struct IUnknown *)&DiagData );
	}

	SW_GENKIN = (int)((*m_pSnHeadData)->Sn_GENKN&0xff);	// 現金主義会計の適用
	if( SW_GENKIN ) {
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4030, (struct IUnknown *)&DiagData );
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4031, (struct IUnknown *)&DiagData );
	}
	else	{
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4030, (struct IUnknown *)&DiagData );
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4031, (struct IUnknown *)&DiagData );
	}

	SW_KISK22 = (int)((*m_pSnHeadData)->Sn_TUMIAG&0x01);	// 規則22条１項の適用
	if( SW_KISK22 ){
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4040, (struct IUnknown *)&DiagData );
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4041, (struct IUnknown *)&DiagData );
	}
	else	{
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4040, (struct IUnknown *)&DiagData );
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4041, (struct IUnknown *)&DiagData );
	}

	char			buf[512]={0};
	char			cbuf[128]={0};
	unsigned char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0};
	char			MNY[MONY_BUF_SIZE]={0};

	// 課税方式区分の表示
	memset( Mny, '\0', MONY_BUF_SIZE );
	memset( buf, '\0', sizeof(buf) );
	// 第一種事業課税売上
	m_Util.l_calq( Mny, ( char *)&(*m_pSnHonpyoData)->Sn_KURI1[0], 2 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4100, Mny );
//-- '15.02.10 --
//	if( m_pArith->l_test(Mny) || (*m_pSnHonpyoData)->Sn_UP1 ){
//---------------
	if( ((m_pArith->l_test((*m_pSnHeadData)->Sn_KURIA)>0)&&(m_pArith->l_test(Mny)>0)) || (*m_pSnHonpyoData)->Sn_UP1 ){
//---------------
		sprintf_s( buf, sizeof(buf), _T("(%3d.%01d%%)"), (*m_pSnHonpyoData)->Sn_UP1 / 10, (*m_pSnHonpyoData)->Sn_UP1 % 10 );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4101, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4101 );
	}
	
	// 第二種事業課税売上
	m_Util.l_calq( Mny, (char *)&(*m_pSnHonpyoData)->Sn_KURI2[0], 2 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4110, Mny );
//-- '15.02.10 --
//	if( m_pArith->l_test(Mny) || (*m_pSnHonpyoData)->Sn_UP2 ) {
//---------------
	if( ((m_pArith->l_test((*m_pSnHeadData)->Sn_KURIA)>0)&&(m_pArith->l_test(Mny)>0)) || (*m_pSnHonpyoData)->Sn_UP2 ) {
//---------------
		sprintf_s( buf, sizeof(buf), _T("(%3d.%01d%%)"), (*m_pSnHonpyoData)->Sn_UP2 / 10, (*m_pSnHonpyoData)->Sn_UP2 % 10 );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4111, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4111 );
	}

	// 第三種事業課税売上
	m_Util.l_calq( Mny, (char *)&(*m_pSnHonpyoData)->Sn_KURI3[0], 2 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4120, Mny );
//-- '15.02.10 --
//	if( m_pArith->l_test(Mny) || (*m_pSnHonpyoData)->Sn_UP3 ){
//---------------
	if( ((m_pArith->l_test((*m_pSnHeadData)->Sn_KURIA)>0)&&(m_pArith->l_test(Mny)>0)) || (*m_pSnHonpyoData)->Sn_UP3 ){
//---------------
		sprintf_s( buf, sizeof(buf), _T("(%3d.%01d%%)"), (*m_pSnHonpyoData)->Sn_UP3 / 10, (*m_pSnHonpyoData)->Sn_UP3 % 10 );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4121, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4121 );
	}

	// 第四種事業課税売上
	m_Util.l_calq( Mny, (char *)&(*m_pSnHonpyoData)->Sn_KURI4[0], 2 );
	dsp_prs( IDC_ICSDIAGCTRL4,ID4130, Mny );
//-- '15.02.10 --
//	if( m_pArith->l_test(Mny) || (*m_pSnHonpyoData)->Sn_UP4 ) {
//---------------
	if( ((m_pArith->l_test((*m_pSnHeadData)->Sn_KURIA)>0)&&(m_pArith->l_test(Mny)>0)) || (*m_pSnHonpyoData)->Sn_UP4 ) {
//---------------
		sprintf_s( buf, sizeof(buf), _T("(%3d.%01d%%)"), (*m_pSnHonpyoData)->Sn_UP4 / 10, (*m_pSnHonpyoData)->Sn_UP4 % 10 );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4131, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4131 );
	}
//	if( (pSyzShin->Ver&0xff) == 0x02 ){
//	if( 1 ){
	// 第五種事業課税売上
	m_Util.l_calq( Mny, (char *)&(*m_pSnHonpyoData)->Sn_KURI5[0], 2 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4140, Mny );
//-- '15.02.10 --
//	if( m_pArith->l_test(Mny) || (*m_pSnHonpyoData)->Sn_UP5 ){
//---------------
	if( ((m_pArith->l_test((*m_pSnHeadData)->Sn_KURIA)>0)&&(m_pArith->l_test(Mny)>0)) || (*m_pSnHonpyoData)->Sn_UP5 ){
//---------------
		sprintf_s( buf, sizeof(buf), _T("(%3d.%01d%%)"), (*m_pSnHonpyoData)->Sn_UP5 / 10, (*m_pSnHonpyoData)->Sn_UP5 % 10 );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4141, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4141 );
	}

	//[Y]
	// 第六種事業課税売上
	m_Util.l_calq( Mny, (char *)&(*m_pSnHonpyoData)->Sn_KURI6[0], 2 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4150, Mny );
//-- '15.05.12 --
//	if( m_pArith->l_test(Mny) || (*m_pSnHonpyoData)->Sn_UP6 ){
//---------------
	if( ((m_pArith->l_test((*m_pSnHeadData)->Sn_KURIA)>0)&&(m_pArith->l_test(Mny)>0)) || (*m_pSnHonpyoData)->Sn_UP6 ){
//---------------
		sprintf_s( buf, sizeof(buf), _T("(%3d.%01d%%)"), (*m_pSnHonpyoData)->Sn_UP6 / 10, (*m_pSnHonpyoData)->Sn_UP6 % 10 );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4151, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4151 );
	}

	//[Y]
	// 合計
//	m_Util.l_calq( Mny, (char *)&(*m_pSnHeadData)->Sn_KURIA[0], 2 );
//	dsp_prs( IDC_ICSDIAGCTRL4, ID4150, Mny );

	// 課税標準額
	memset( WORK0, '\0', MONY_BUF_SIZE );
	memset( WORK1, '\0', MONY_BUF_SIZE );
	memset( WORK2, '\0', MONY_BUF_SIZE );
	m_Util.l_calq( (char *)WORK0, (char *)(*m_pSnFhyo40Data)->Sn_1F1A, 0 );
	m_Util.l_calq( (char *)WORK1, (char *)(*m_pSnFhyo40Data)->Sn_1F1B, 0 );
	m_Util.l_calq( (char *)WORK2, (char *)(*m_pSnFhyo40Data)->Sn_1F1C, 0 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4KM1, (char *)WORK0 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4KM2, (char *)WORK1 );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4KM3, (char *)WORK2 );

	// 仕入控除方式の表示	簡易課税
	if( (int)((*m_pSnHeadData)->Sn_TOKUR&0xff) ){
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4160, (struct IUnknown *)&DiagData );
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4161, (struct IUnknown *)&DiagData );
	}
	else{
		DiagData.data_check = 0;
		m_KaniDiag04.SetData( ID4160, (struct IUnknown *)&DiagData );
		DiagData.data_check = 1;
		m_KaniDiag04.SetData( ID4161, (struct IUnknown *)&DiagData );
	}

/*- '14.02.15_CUT -*/
//	// 自動車　課税標準額　表示 6.0%
////★//[13'04.01]///
////	strcpy_s( buf, sizeof(buf), "６．０％" );
////	DiagData.data_disp = (char *)buf;
////	m_KaniDiag04.SetData(  ID4KN3, (struct IUnknown *)&DiagData );
/////////////////////
//	m_pArith->l_input( M1000, _T("1000") );
//	m_pArith->l_div( MNY,(char *)(*m_pSnHeadData)->Sn_CK60, M1000 ); 
//	dsp_prs( IDC_ICSDIAGCTRL4, ID4KM3, MNY );
/*-----------------*/

	// 消費税額
	memset( WORK0, '\0', MONY_BUF_SIZE );
	memset( WORK1, '\0', MONY_BUF_SIZE );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4SM1, (char *)(*m_pSnFhyo40Data)->Sn_1F2A );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4SM2, (char *)(*m_pSnFhyo40Data)->Sn_1F2B );
	dsp_prs( IDC_ICSDIAGCTRL4, ID4SN3, (char *)(*m_pSnFhyo40Data)->Sn_1F2C );

	if( (*m_pSnHeadData)->SVmzsw == 1 ){// 経過措置対象の課税資産の譲渡等あり
		m_pArith->l_sub( Mny , (*m_pSnFhyo40Data)->Sn_1F9B , (*m_pSnFhyo40Data)->Sn_1F8B );
		if( m_pArith->l_test( XM17 ) > 0){//還付のとき
			if( !((*m_pSnHeadData)->Sn_SKKBN%2) ){
				memset( Mny , '\0' , MONY_BUF_SIZE);
			}
			else{
/*- '14.06.03_CUT -*/
//				m_pArith->l_neg ( Mny );
/*-----------------*/
			}
		}
		if( !m_pArith->l_test( XM17 ) && !m_pArith->l_test( XM18 ) ){
			memset( Mny , '\0' , MONY_BUF_SIZE);
		}
		dsp_prs( IDC_ICSDIAGCTRL4, ID4TSM1, Mny );
		// 地方消費税の課税標準となる消費税額6.3%
		memset( Mny , '\0' , MONY_BUF_SIZE);
		m_pArith->l_sub( Mny , (*m_pSnFhyo40Data)->Sn_1F9C , (*m_pSnFhyo40Data)->Sn_1F8C );
		if( m_pArith->l_test( XM17 ) > 0){//還付のとき
			if( !((*m_pSnHeadData)->Sn_SKKBN%2) ){
				memset( Mny , '\0' , MONY_BUF_SIZE);
			}
			else{
/*- '14.06.03_CUT -*/
//				m_pArith->l_neg ( Mny );
/*-----------------*/
			}
		}
		if( !m_pArith->l_test( XM17 ) && !m_pArith->l_test( XM18 ) ){
			memset( Mny , '\0' , MONY_BUF_SIZE);
		}
		dsp_prs( IDC_ICSDIAGCTRL4, ID4TSM2, Mny );
	}
	else{
		if( m_pArith->l_test( XM17 ) > 0){
			memmove( Mny , XM17 , MONY_BUF_SIZE );
//--> '14.06.03 INS START
			m_pArith->l_neg( Mny );
//<-- '14.06.03 INS END
		}
		else{
			memmove( Mny , XM18 , MONY_BUF_SIZE );
		}
		dsp_prs( IDC_ICSDIAGCTRL4, ID4TSM2, Mny );
	}



/*	// 地方消費税の課税標準となる消費税額	（表示のみ）
	if( m_pArith->l_test((*m_pSnFhyo40Data)->Sn_1F10T) < 0 ){//合計差引税額がマイナスの場合は(11)-(12)の額を表示する

		if( (*m_pSnHeadData)->SVmzsw == 1 ){// 経過措置対象の課税資産の譲渡等あり
			// 地方消費税の課税標準となる消費税額4%
			memset( Mny , '\0' , MONY_BUF_SIZE);
			m_pArith->l_sub( Mny , (*m_pSnFhyo40Data)->Sn_1F8B , (*m_pSnFhyo40Data)->Sn_1F9B );
			dsp_prs( IDC_ICSDIAGCTRL4, ID4TSM1, Mny );
		}

		// 地方消費税の課税標準となる消費税額6.3%
		memset( Mny , '\0' , MONY_BUF_SIZE);
		m_pArith->l_sub( Mny , (*m_pSnFhyo40Data)->Sn_1F8C , (*m_pSnFhyo40Data)->Sn_1F9C );
		dsp_prs( IDC_ICSDIAGCTRL4, ID4TSM2, Mny );	

	}
	else{//合計差引税額がマイナスの場合は(12)-(11)の額を表示する

		if( (*m_pSnHeadData)->SVmzsw == 1 ){// 経過措置対象の課税資産の譲渡等あり
			// 地方消費税の課税標準となる消費税額4%
			memset( Mny , '\0' , MONY_BUF_SIZE);
			m_pArith->l_sub( Mny , (*m_pSnFhyo40Data)->Sn_1F9B , (*m_pSnFhyo40Data)->Sn_1F8B );
			dsp_prs( IDC_ICSDIAGCTRL4, ID4TSM1, Mny );
		}

		// 地方消費税の課税標準となる消費税額6.3%
		memset( Mny , '\0' , MONY_BUF_SIZE);
		m_pArith->l_sub( Mny , (*m_pSnFhyo40Data)->Sn_1F9C , (*m_pSnFhyo40Data)->Sn_1F8C );
		dsp_prs( IDC_ICSDIAGCTRL4, ID4TSM2, Mny );	

	}
*/
	memset( Mny , '\0' , MONY_BUF_SIZE);


/*- '14.02.16_CUT -*/
//	// 自動車　消費税額　　表示 6.0%
////★//[13'04.01]///
////	strcpy_s( buf, sizeof(buf), "　　　％" );
////	DiagData.data_disp = (char *)buf;
////	m_KaniDiag04.SetData(  ID4SN3, (struct IUnknown *)&DiagData );
/////////////////////
////	dsp_prs( IDC_ICSDIAGCTRL4, ID4SM3, (char *)pSyzShin->Snd.Sn_CZ60 );
/*-----------------*/

	// 金融機関 名
	memset( buf, '\0', sizeof(buf) );
	int KjLen = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_BANK, 20 );
	memmove( buf, (*m_pSnHeadData)->Sn_BANK, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4210, (struct IUnknown *)&DiagData );

	// 銀行・金庫等
	DiagData.data_combo = (short )((*m_pSnHeadData)->Sn_FINANCE&0x0f);
	m_KaniDiag04.SetData( ID4212, (struct IUnknown *)&DiagData );

	// 金融機関 支店名
	memset( buf, '\0', sizeof(buf) );
	KjLen = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_SITEN, 20 );
	memmove( buf, (*m_pSnHeadData)->Sn_SITEN, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4211, (struct IUnknown *)&DiagData );

	// 本社・支社等
	DiagData.data_combo = (short )((*m_pSnHeadData)->Sn_CHAIN&0x0f);
	m_KaniDiag04.SetData( ID4213, (struct IUnknown *)&DiagData );

	// 金融機関 預金種別
	memset( buf, '\0', sizeof(buf) );
	KjLen = kjlen( (*m_pSnHeadData)->Sn_YOKIN, 8 );
	memmove( buf, (*m_pSnHeadData)->Sn_YOKIN, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4214, (struct IUnknown *)&DiagData );

	// 金融機関 口座番号
	if( ((*m_pSnHeadData)->Sn_KOZA[0]&0xff) != 0xff ) {
		memmove( cbuf, (*m_pSnHeadData)->Sn_KOZA, 5 );
		l_unpac( buf, cbuf, 10 );
		DiagData.data_edit = (char *)buf;
		m_KaniDiag04.SetData( ID4215, (struct IUnknown *)&DiagData );
	}
	else{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4215 );
	}

	// 郵便局 名
	memset( buf, '\0', sizeof(buf) );
	KjLen = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_YUBIN, 16 );
	memmove( buf, (*m_pSnHeadData)->Sn_YUBIN, KjLen );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData(  ID4312, (struct IUnknown *)&DiagData );

	// 銀行？郵便局
	if( (*m_pSnHeadData)->Sn_BYsw ){
		DiagData.data_combo = 1;
	}
	else{
		DiagData.data_combo = 0;
	}
	m_KaniDiag04.SetData(  ID4410, (struct IUnknown *)&DiagData );
	
	// 郵便局 預金番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_YUNUM1, 5 );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4310, (struct IUnknown *)&DiagData );

	// 郵便局 預金番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_YUNUM2, 8 );
	DiagData.data_edit = (char *)buf;
	m_KaniDiag04.SetData( ID4311, (struct IUnknown *)&DiagData );

	// 税務署整理欄
	memset( cbuf, '\0', sizeof(cbuf) );
	if( ((*m_pSnHeadData)->Sn_ZSEIRI[0]&0xff) != 0xff ) {
		if( ((*m_pSnHeadData)->Sn_ZSEIRI[3]&0x0f) != 0x0f ) {	//8桁入力済み
			m_pArith->l_unpac( cbuf, (*m_pSnHeadData)->Sn_ZSEIRI, 8 );
			if( cbuf[0] != 0x30 ){	// 先頭が0以外は後1桁カット
				cbuf[7] = '\0';
			}
			else{
				int itct;
				for( itct=0; itct<7; ++itct ){
					cbuf[itct] = cbuf[itct+1];
				}
				cbuf[itct] = '\0';
			}
			memset( (*m_pSnHeadData)->Sn_ZSEIRI, '\0', 4 );
			m_pArith->l_pack( (*m_pSnHeadData)->Sn_ZSEIRI, cbuf, 7 );
		}
		memset( cbuf, '\0', sizeof(cbuf) );
		m_pArith->l_unpac( cbuf, (*m_pSnHeadData)->Sn_ZSEIRI, 8 );  
		DiagData.data_edit = (char *)cbuf;//[Y]
//		DiagData.data_disp = (char *)cbuf;
		m_KaniDiag04.SetData(  ID4RNO, (struct IUnknown *)&DiagData );
	}

	// 税理士情報の画面表示は全て表示する（印刷情報と連動しない）
	if( m_pShinInfo->isExistOffice ){
		int	i, j, k;
		// 事務所名又は法人名
		memset( buf, '\0', sizeof(buf));
		memcpy( buf, m_pShinInfo->pO_DATA->o_ofcnam, sizeof( m_pShinInfo->pO_DATA->o_ofcnam ));
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4ZI0, (struct IUnknown *)&DiagData );
		// 担当税理士氏名
		memset( buf, '\0', sizeof(buf));
		if( (i = (int)strlen(  m_pShinInfo->pZ_DATA->z_zeinam )) > 40 )	i = 40;
		memcpy( buf, m_pShinInfo->pZ_DATA->z_zeinam, i );
		DiagData.data_disp = buf;
		m_KaniDiag04.SetData( ID4ZI1, (struct IUnknown *)&DiagData );
		// 電話番号
		memset( buf, '\0', sizeof(buf) );
		for( j = 0; j < 18; ++j )	buf[j] = 0x20;
		for( j = k = 0; j < 30 || m_pShinInfo->pO_DATA->o_telno[j]; ++j ) {
			if( m_pShinInfo->pO_DATA->o_telno[j] == '-' || m_pShinInfo->pO_DATA->o_telno[j] == 'ｰ' ) {
				switch( k ) {
					case 0 :
						k = ( (j < 6) ? j : 6 );
						memmove( &buf[6-k], &m_pShinInfo->pO_DATA->o_telno[0], k );
						k = 1;	i = j + 1;
						continue;
					case 1 :
						if( (j-i) < 5 )	k = (j - i);
						else			k = 5;
						memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[i],   k );
						memmove( &buf[13], &m_pShinInfo->pO_DATA->o_telno[++j], 5 );
						k = 2;
						break;
					default:	break;
				}
				if( k > 1 )	break;
			}
		}
		if( ! k )	memmove( buf, &m_pShinInfo->pO_DATA->o_telno[0], (j > 18 ? 18 : j) );
		else	{
			if( k == 1 ) {
				memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[i], 4 );
			}
		}
		DiagData.data_disp = (char *)buf;
		m_KaniDiag04.SetData(  ID4ZI2, (struct IUnknown *)&DiagData );

	}
	else	{
		dsp_cls( IDC_ICSDIAGCTRL4, ID4ZI0 );
		dsp_cls( IDC_ICSDIAGCTRL4, ID4ZI1 );
		dsp_cls( IDC_ICSDIAGCTRL4, ID4ZI2 );
	}

	// 税理士法第３０条の書面提出有り
	DiagData.data_check = (short)( (*m_pSnHeadData)->Sn_ZeiHou&0x01 );
	m_KaniDiag04.SetData( ID4ZI3, (struct IUnknown *)&DiagData );
	// 税理士法第３３条の２の書面提出有り
	DiagData.data_check = (short)( (*m_pSnHeadData)->Sn_ZeiHou&0x02 );
	m_KaniDiag04.SetData( ID4ZI4, (struct IUnknown *)&DiagData );
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinKaniEX::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			MNY[MONY_BUF_SIZE] = {0};
	char			M1000[MONY_BUF_SIZE] = {0};

	if( !m_DpSw ){
		if( Id == IDC_ICSDIAGCTRL3 ){	// 税額計算
			switch( Index ){

				//[Y]
				case ID3XM2	:	//  2. 消費税額計
					memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
					m_KaniDiag03.SetData( Index, (struct IUnknown *)&DiagData );

					if(  (*m_pSnHeadData)->SVmzsw != 1 && ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]経過措置がない場合のみかつ非連動の場合のみ
						if(!(m_M1c&0x08) ){//自動計算の場合
							ChangeColor( IDC_ICSDIAGCTRL3, ID3XM2, 0 );
						}
						else{//手入力の場合
							ChangeColor( IDC_ICSDIAGCTRL3, ID3XM2, 3 );
						}
					}

					break;

				case ID3XM1	:	//  1. 課税標準額
		//		case ID3XM2	:	//  2. 消費税額計
				case ID3XM3	:	//  3. 控除過大調整税額
				case ID3XM4	:	//  4. 控除対象仕入税額
				case ID3XM5	:	//  5. 返還等対価の税額
				case ID3XM6	:	//  6. 貸倒れに係る税額
				case ID3XM10:	// 10. 中間納付税額
				case ID3XM13:	// 13. 既確定税額
				case ID3XM15:	// 15. 課税売上高
				case ID3XM16:	// 16. 総売上高 or 基準期間の課税売上高
				case ID3XM21:	// 21. 中間納付譲渡割額	
				case ID3XM24:	// 24. 既確定譲渡割額
					memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
					m_KaniDiag03.SetData( Index, (struct IUnknown *)&DiagData );
					break;
				default		:
					memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
					m_KaniDiag03.SetData( Index, (struct IUnknown *)&DiagData );
					break;
			}
		}
		else{						// 付記・参考
			switch( Index ){			
//				case ID4KM3 :	// 課税標準額?%
//				case ID4SM3 :	// 消費税額?%
//					memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
//					m_KaniDiag04.SetData( Index, (struct IUnknown *)&DiagData );
//					break;
				case ID4100 :	// 第１個所
				case ID4110	:	// 第２種
				case ID4120	:	// 第３種
				case ID4130	:	// 第４種
				case ID4140	:	// 第５種
				case ID4150	:	// 合計
				case ID4KM1	:	// 課税標準額4%
				case ID4KM2	:	// 課税標準額3%
				case ID4KM3	:	// 課税標準額6.3%
					m_pArith->l_input( M1000, "1000" );
					m_pArith->l_div( MNY, Mony, M1000 );
					memmove( DiagData.data_val, MNY, MONY_BUF_SIZE );
					m_KaniDiag04.SetData( Index, (struct IUnknown *)&DiagData );
					break;
				default :
					memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
					m_KaniDiag04.SetData( Index, (struct IUnknown *)&DiagData );
					break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinKaniEX::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){
		//[Y]
		if( Id == IDC_ICSDIAGCTRL2 ){	// 基本情報
			char	tmpData = 0;//[Y]
			char	buf[512] = {0};

			switch( Index ){
				case ID2000 :	// 提出年
					//[Y]
					(*m_pSnHeadData)->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 0, tmpData );
					DiagData.data_day[0] = 0x00;
					m_KaniDiag02.SetData( ID2000, (struct IUnknown *)&DiagData );
					break;

				case ID2001 :	// 提出月
					//[Y]
					(*m_pSnHeadData)->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 1, tmpData );
					DiagData.data_day[0] = 0x00;
					m_KaniDiag02.SetData( ID2001, (struct IUnknown *)&DiagData );
					break;

				case ID2002 :	// 提出日
					//[Y]
					(*m_pSnHeadData)->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 2, tmpData );
					DiagData.data_day[0] = 0x00;
					m_KaniDiag02.SetData( ID2002, (struct IUnknown *)&DiagData );
					break;

				case ID2010 :	// 税務署名
				case ID2020 :	// 郵便番号
				case ID2030 :	// 納税地　上段
				case ID2031 :	// 〃　　　下段
				case ID2040 :	// 電話番号　市外
				case ID2041 :	// 〃　　　　市内
				case ID2042 :	// 〃		 局番
				case ID2050 :	// 名称又は屋号		（フリガナ）
				case ID2051 :	// 名称又は屋号
				case ID2052 :	// 被合併名
				case ID2060 :	// 代書者又は氏名	（フリガナ）
				case ID2061 :	// 代書者又は氏名
				case ID2070 :	// 経理担当者氏名
					m_KaniDiag02.DataClear( Index, TRUE );
					break;

				case ID2080 :	// 一連番号
					memset( (*m_pSnHeadData)->Sn_ITIREN, '\0', sizeof((*m_pSnHeadData)->Sn_ITIREN) );
					(*m_pSnHeadData)->Sn_ITIREN[0] = 0xff;
				//	memset( buf, '\0', sizeof(buf) );
				//	DiagData.data_edit = buf;
					m_KaniDiag02.DataClear( Index, TRUE );
					break;

				case ID2081 :	// 送付　要・不要
					break;
				case ID2082 :	// 所管
					memset( (*m_pSnHeadData)->Sn_SYOKN, '\0', 3 );
				//	memset( buf, '\0', sizeof(buf) );
				//	DiagData.data_edit = buf;
					m_KaniDiag02.DataClear( Index, TRUE );	
					break;

				case ID2083 :	// 整理番号
					memset( (*m_pSnHeadData)->Sn_SEIRI, '\0', sizeof((*m_pSnHeadData)->Sn_SEIRI) );
					(*m_pSnHeadData)->Sn_SEIRI[0] = 0xff;
			//		memset( buf, '\0', sizeof(buf) );
			//		DiagData.data_edit = buf;
					m_KaniDiag02.DataClear( Index, TRUE );	


				default:
					m_KaniDiag02.DataClear( Index, TRUE );
					break;
			}
		}
		else if( Id == IDC_ICSDIAGCTRL3 ){	//[Y] 税額計算
			switch( Index ){
				case ID3XM1	:	//  1. 課税標準額
				case ID3XM2 :	//  2. 消費税額計
				case ID3XM3	:	//  3. 控除過大調整税額
				case ID3XM4 :	//  4. 控除対象仕入税額
				case ID3XM5	:	//  5. 返還等対価の税額
				case ID3XM6	:	//  6. 貸倒れに係る税額
				case ID3XM7	:	//  7. 控除税額小計
				case ID3XM8 :	//  8. 控除不足還付税額
				case ID3XM9	:	//  9. (2+3-7) 差引税額
				case ID3XM10:	// 10. 中間納付税額
				case ID3XM11:	// 11. (9-10) 納付税額
				case ID3XM12:	// 12. 中間納付還元税額
				case ID3XM13:	// 13. 既確定税額
				case ID3XM14:	// 14. 差引納付税額
				case ID3XM15:	// 15. 課税売上高
				case ID3XM16:	// 16. 総売上高 or 基準期間の課税売上高
				case ID3XM17:	// 17. 控除不足還付税額
				case ID3XM18:	// 18. 差引税額
				case ID3XM19:	// 19. (17*25%) 還付額
				case ID3XM20:	// 20. (18*25%) 納税額
				case ID3XM21:	// 21. 中間納付譲渡割額
				case ID3XM22:	// 22. 納付譲渡割額
				case ID3XM23:	// 23. 中間還付譲渡割額
				case ID3XM24:	// 24. 既確定譲渡割額
				case ID3XM25:	// 25. 差引納付譲渡割額
				case ID3XM26:	// 26. 消費税･地方消費税合計
					memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
					m_KaniDiag03.SetData( Index, (struct IUnknown *)&DiagData );
					break;
				default		:
					m_KaniDiag03.DataClear( Index, TRUE );
					break;
			}
		}
		else{							// 付記・参考
			switch( Index ){			
				case ID4100 :	// 第１個所
				case ID4110	:	// 第２種
				case ID4120	:	// 第３種
				case ID4130	:	// 第４種
				case ID4140	:	// 第５種
				case ID4150	:	// 第６種
				case ID4KM1	:	// 課税標準額4%
				case ID4KM2	:	// 課税標準額3%
				case ID4KM3 :	// 課税標準額?%
				case ID4SM1 :	// 消費税額4%
				case ID4SM2	:	// 消費税額3%
//				case ID4SM3	:	// 消費税額?%
					memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
					m_KaniDiag04.SetData( Index, (struct IUnknown *)&DiagData );
					break;
				case ID4000	:	// 割賦基準の適用 
				case ID4001	:	// 割賦基準の適用 
				case ID4010	:	// 延払基準の適用
				case ID4011	:	// 延払基準の適用
				case ID4020	:	// 工事進行基準の適用
				case ID4021	:	// 工事進行基準の適用
				case ID4030	:	// 現金主義会計の適用
				case ID4031	:	// 現金主義会計の適用
				case ID4040	:	// 規則22条1項の適用
				case ID4041	:	// 規則22条1項の適用
				case ID4160	:	// 特例計算適用
				case ID4161	:	// 特例計算適用
				case ID4212 :	// 銀行・金庫等
				case ID4213 :	// 本社・支社等
				case ID4410 :	// 銀行・郵便局
				case ID4ZI3 :	// 税理士　第３０条
				case ID4ZI4 :	// 〃	　 第３０条の２
					// コンボボックス　動作無し
					break;
				default :
					m_KaniDiag04.DataClear( Index, TRUE );
					break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ダイアグラム初期設定
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinKaniEX::ChgDiagAttr()
{
	int	cnt;
	cnt = 0;
	while( 1 ){
		if( Kani1[cnt].Index == 0 ){
			break;
		}
		int	Flg;

		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Kani1[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Kani1[cnt].UnConnect;
			}
			else{
				Flg = Kani1[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL2, Kani1[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Kani1[cnt].Connect );

		if( (Kani1[cnt].Index==ID2050) || (Kani1[cnt].Index==ID2060) ){
			m_KaniDiag02.ModifyItem( Kani1[cnt].Index,( Flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY ) | DIAG_MODIFY_IMEKANA );
		}

		cnt++;
	}
	
	cnt = 0;
	while( 1 ){
		if( Kani2[cnt].Index == 0 ){
			break;
		}

		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Kani2[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Kani2[cnt].UnConnect;
			}
			else{
				Flg = Kani2[cnt].Connect;
			}
			if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
				if( (Kani2[cnt].Index==ID3XM10) || (Kani2[cnt].Index==ID3XM21) ){
					Flg = 0;
				}
			}
			if( (Kani2[cnt].Index==ID3XM13) || (Kani2[cnt].Index==ID3XM24) ){
				if( Flg == 1 ){
					// 修正確定・修正中間
					if( ((*m_pSnHeadData)->Sn_SKKBN==0x03) || ((*m_pSnHeadData)->Sn_SKKBN==0x04) ){
						;
					}
					else{
						Flg = 0;
					}
				}
				else{
					Flg = 0;
				}
			}
		}

		int flg_ex = 0;	
		if( Flg == 2 ){
			if( (*m_pSnHeadData)->SVmzsw == 0 ){	// 経過措置なし
				flg_ex = TRUE;
			}
			else{								// 経過措置あり
				flg_ex = FALSE;
			}
		}
		else{
			if( Flg ){
				flg_ex = TRUE;
			}
			else{
				flg_ex = FALSE;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL3, Kani2[cnt].Index, flg_ex, (*m_pSnHeadData)->Sn_Sign4, Kani2[cnt].Connect );
		cnt++;
	}

	cnt = 0;
	while( 1 ){
		if( Kani3[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Kani3[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Kani3[cnt].UnConnect;
			}
			else{
				Flg = Kani3[cnt].Connect;
			}
		}
	
		ATRIB_MOD( IDC_ICSDIAGCTRL4, Kani3[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Kani3[cnt].Connect );
		cnt++;
	}
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );
	}
	else{
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( TRUE );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CShinKaniEX::EndJob()
{
	// エディット中だと正しく終了しない為
}

BEGIN_EVENTSINK_MAP(CShinKaniEX, CSyzBaseDlg)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL2, 1, CShinKaniEX::EditONIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL2, 2, CShinKaniEX::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL2, 3, CShinKaniEX::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL3, 1, CShinKaniEX::EditONIcsdiagctrl3, VTS_I2)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL3, 2, CShinKaniEX::EditOFFIcsdiagctrl3, VTS_I2)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL3, 3, CShinKaniEX::TerminationIcsdiagctrl3, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL4, 1, CShinKaniEX::EditONIcsdiagctrl4, VTS_I2)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL4, 2, CShinKaniEX::EditOFFIcsdiagctrl4, VTS_I2)
	ON_EVENT(CShinKaniEX, IDC_ICSDIAGCTRL4, 3, CShinKaniEX::TerminationIcsdiagctrl4, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinKaniEX::EditONIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rectA, rectB;
	//GetWindowRect( &rectA );
	//m_KaniDiag02.GetRect( index, (LPUNKNOWN)&rectB );
	//if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	//}
	//<--Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL2;//[Y]
}

void CShinKaniEX::EditOFFIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	char			buf[512] = {0};
	int				len;
	DIAGRAM_DATA	DiagData, DiagDataYY, DiagDataMM, DiagDataDD;
	DiagData.data_imgdata = NULL;
	char			tmpYmd[4] ={ 0 };

	// データの取得
	m_Util.DiagOcxIniz(&DiagData);
	m_KaniDiag02.GetData( index, (struct IUnknown *)&DiagData );
	//[Y]
	char	yymmdd[3]={0};
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2] );

	switch( index ){
		case ID2000 :	// 提出年
			if( DiagData.data_day[0] != 0 ){
				m_Util.DiagOcxIniz(&DiagDataMM);
				m_KaniDiag02.GetData(ID2001, (struct IUnknown *)&DiagDataMM);
				m_Util.DiagOcxIniz(&DiagDataDD);
				m_KaniDiag02.GetData(ID2002, (struct IUnknown *)&DiagDataDD);

				(*m_pSnHeadData)->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 0/*年*/, DiagData.data_day[0], DiagDataMM.data_day[0], DiagDataDD.data_day[0], 0 );
				(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &tmpYmd[0], &tmpYmd[1], &tmpYmd[2]);
				if( DiagDataMM.data_day[0] ){
					if( DiagData.data_day[0] != tmpYmd[0] ){
						DiagData.data_day[0] = tmpYmd[0];
						m_KaniDiag02.SetData(index, (struct IUnknown *)&DiagData);
					}
				}
			}
			else{//[Y]
				DiagData.data_day[0] = yymmdd[0];
				m_KaniDiag02.SetData( index, (struct IUnknown *)&DiagData );
			}
			break;
		case ID2001 :	// 提出月
			if( DiagData.data_day[0] != 0x00 ){
				m_Util.DiagOcxIniz(&DiagDataYY);
				m_KaniDiag02.GetData(ID2000, (struct IUnknown *)&DiagDataYY);
				m_Util.DiagOcxIniz(&DiagDataDD);
				m_KaniDiag02.GetData(ID2002, (struct IUnknown *)&DiagDataDD);

				(*m_pSnHeadData)->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 1/*月*/, DiagDataYY.data_day[0], DiagData.data_day[0], DiagDataDD.data_day[0], 0 );
				(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &tmpYmd[0], &tmpYmd[1], &tmpYmd[2]);
				if( DiagDataYY.data_day[0] != tmpYmd[0] ){
					DiagDataYY.data_day[0] = tmpYmd[0];
					m_KaniDiag02.SetData(ID2000, (struct IUnknown *)&DiagDataYY);
				}
			}
			else{//[Y]
				DiagData.data_day[0] = yymmdd[1];		
				m_KaniDiag02.SetData( index, (struct IUnknown *)&DiagData );
			}
			break;
		case ID2002 :	// 提出日
			if( DiagData.data_day[0] != 0x00 ){
				m_Util.DiagOcxIniz(&DiagDataYY);
				m_KaniDiag02.GetData(ID2000, (struct IUnknown *)&DiagDataYY);
				m_Util.DiagOcxIniz(&DiagDataMM);
				m_KaniDiag02.GetData(ID2001, (struct IUnknown *)&DiagDataMM);

				(*m_pSnHeadData)->SetYmdDataGen( ID_ICSSH_PRESENT_DATE, 2/*日*/, DiagDataYY.data_day[0], DiagDataMM.data_day[0], DiagData.data_day[0], 0 );
				(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &tmpYmd[0], &tmpYmd[1], &tmpYmd[2]);
				if( DiagDataYY.data_day[0] != tmpYmd[0] ){
					DiagDataYY.data_day[0] = tmpYmd[0];
					m_KaniDiag02.SetData(ID2000, (struct IUnknown *)&DiagDataYY);
				}
			}
			else{//[Y]
				DiagData.data_day[0] = yymmdd[2];		
				m_KaniDiag02.SetData( index, (struct IUnknown *)&DiagData );
			}
			break;
		case ID2010 :	// 税務署名
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
			len = (int)DiagData.data_edit.GetLength();
			memset( (*m_pSnHeadData)->Sn_ZMSYO, '\0', 12 );
			memmove( (*m_pSnHeadData)->Sn_ZMSYO, DiagData.data_edit, len );
			if( len ){
				char	name1[20]={0}, name2[20]={0};
				memmove( name1, (*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
				memmove( name2, buf, 12 );
				if( Esel_CompareTaxOffice( name1, name2 ) ){
					memset( (*m_pSnHeadData)->Sn_TAXNO, '\0', sizeof((*m_pSnHeadData)->Sn_TAXNO) );
					// 税務署コードの取得
					if( *(m_pShinInfo->pIsOpenEselect) == TRUE ){
						if( (*m_pSnHeadData)->GetTaxationOffice(this, m_pShinInfo->pEselect) ){
							// 税務署コード
							memset( buf, '\0', sizeof(buf) );
							if( (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!=0x00) && (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!='F') ){
								sprintf_s( buf, sizeof(buf), "署番号 %-5.5s", (*m_pSnHeadData)->Sn_TAXNO );
							}
							else{
								sprintf_s( buf, sizeof(buf), "署番号未登録" );
							}
							DiagData.data_disp = buf;
							m_KaniDiag02.SetData( ID2011, (struct IUnknown *)&DiagData );
							break;
						}
						memset( buf, '\0', sizeof(buf) );
						len = m_Util.MixSpaceCutLength( (char*)(*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
						memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, len );
						DiagData.data_edit = buf;
						m_KaniDiag02.SetData( ID2010, (struct IUnknown *)&DiagData );
					}
				}
				// 税務署コード
				memset( buf, '\0', sizeof(buf) );
				if( (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!=0x00) && (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!='F') ){
					sprintf_s( buf, sizeof(buf), "署番号 %-5.5s", (*m_pSnHeadData)->Sn_TAXNO );
				}
				else{
					sprintf_s( buf, sizeof(buf), "署番号未登録" );
				}
				DiagData.data_disp = buf;
				m_KaniDiag02.SetData( ID2011, (struct IUnknown *)&DiagData );
			}
			else{
				memset( (*m_pSnHeadData)->Sn_TAXNO, '\0', sizeof((*m_pSnHeadData)->Sn_TAXNO) );
				DiagData.data_disp = _T("署番号未登録");
				m_KaniDiag02.SetData( ID2011, (struct IUnknown *)&DiagData );
			}
			break;
		case ID2020 :	// 郵便番号
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_ZIP, buf, 8 );
			break;
		case ID2030 :	// 納税地　上段
			memset( buf, '\0', sizeof(buf) );
			len = DiagData.data_edit.GetLength();
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_NOZEI, buf, 40 );
			break;
		case ID2031 :	// 〃　　　下段
			memset( buf, '\0', sizeof(buf) );
			len = DiagData.data_edit.GetLength();
			memmove( buf, DiagData.data_edit, len );
			memmove( &(*m_pSnHeadData)->Sn_NOZEI[40], buf, 40 );
			break;
		case ID2040 :	// 電話番号　市外
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_TELNO1, buf, 6 );
			break;
		case ID2041 :	// 〃　　　　市内
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_TELNO2, buf, 4 );
			break;
		case ID2042 :	// 〃		 局番
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_TELNO3, buf, 4 );
			break;
		case ID2050 :	// 名称又は屋号		（フリガナ）
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
//2016.10.20 UPDATE START
//			memmove( (*m_pSnHeadData)->Sn_COFRI, buf, 34*2 );
			memmove( (*m_pSnHeadData)->Sn_COFRI, buf, 40*2 );
//2016.10.20 UPDATE END
			break;
		case ID2051 :	// 名称又は屋号
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_CONAM, buf, 40 );
			break;
		case ID2052 :	// 被合併名
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_GPNAM, buf, 40 );
			break;
		case ID2060 :	// 代書者又は氏名	（フリガナ）
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_DIFRI, buf, 34*2 );
			break;
		case ID2061 :	// 代書者又は氏名
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_DIHYO, buf, 40 );
			break;
		case ID2070 :	// 経理担当者氏名
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_KEIRI, buf, 40 );
			break;
		case ID2080 :	// 一連番号
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			m_pArith->l_pack( (*m_pSnHeadData)->Sn_ITIREN, buf, 8 );
			break;
		case ID2081 :	// 送付　要・不要
			(*m_pSnHeadData)->Sn_ZeiHou &= 0xfb;
			if( DiagData.data_check ){
				(*m_pSnHeadData)->Sn_ZeiHou |= 0x04;
			}
			break;
		case ID2082 :	// 所管
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			memmove( (*m_pSnHeadData)->Sn_SYOKN, buf, 3 );
			break;
		case ID2083 :	// 整理番号
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, DiagData.data_edit, len );
			m_pArith->l_pack( (*m_pSnHeadData)->Sn_SEIRI, buf, 8 );
			break;
		default     :
			break;
	}
}

void CShinKaniEX::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//	char	Str[128] = {0};
	char buff[64] = {0};
	int		len = 0;
	CString	ZipBf, AddBf;
	
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	if( nChar == 0 ){
		return;
	}

	char	buf[256]={0};
	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char )::GetKeyState( VK_SHIFT );
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

	//[Y]
	switch( index ){
		case ID2000 :	// 提出年
		case ID2001 :	// 提出月
		case ID2002 :	// 提出日
			break;

		case ID2010 :	// 税務署名
			if( wTerm == VK_HOME ){
				if( *(m_pShinInfo->pIsOpenEselect) ){
					memset( m_pShinInfo->pEselect, '\0', sizeof(ESELECT) );
					m_pShinInfo->pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
					if( (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!=0x00) && (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!='F') ){
						memmove( m_pShinInfo->pEselect->cd_office, (*m_pSnHeadData)->Sn_TAXNO, sizeof((*m_pSnHeadData)->Sn_TAXNO) );
					}
					m_Util.DiagOcxIniz(&DiagData);
					m_KaniDiag02.GetData( index, (struct IUnknown *)&DiagData );
					len = (int)DiagData.data_edit.GetLength();
					memset( (*m_pSnHeadData)->Sn_ZMSYO, '\0', sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
					memmove( (*m_pSnHeadData)->Sn_ZMSYO, DiagData.data_edit, len );
					if( Esel_SelectTaxOffice(m_pShinInfo->pEselect) == IDOK ){
						memmove( (*m_pSnHeadData)->Sn_TAXNO, m_pShinInfo->pEselect->cd_office, sizeof((*m_pSnHeadData)->Sn_TAXNO) );
						memset( buf, '\0', sizeof(buf) );
						kspclr( buf, 6 );
						len = (int)strlen( m_pShinInfo->pEselect->nm_office );
						memmove( buf, m_pShinInfo->pEselect->nm_office, len );
						memmove( (*m_pSnHeadData)->Sn_ZMSYO, buf, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
						// 税務署名
						memset( buf, '\0', sizeof(buf) );
						len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
						memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, len );
						DiagData.data_edit = buf;
						m_KaniDiag02.SetData( ID2010, (struct IUnknown *)&DiagData );
						memset( buf, '\0', sizeof(buf) );
						sprintf_s( buf, sizeof(buf), _T("署番号 %-5.5s"), (*m_pSnHeadData)->Sn_TAXNO );
						DiagData.data_disp = buf;
						m_KaniDiag02.SetData( ID2011, (struct IUnknown *)&DiagData );
						m_KaniDiag02.SetPosition( ID2020 );
					}
					else{
						m_KaniDiag02.SetPosition( ID2010 );
					}
				}
			}
			break;//[Y]

		case ID2020 :	// 郵便番号
			if( wTerm == VK_HOME ){
//--> '15.05.11 INS START
				m_KaniDiag02.GetData( ID2020, (struct IUnknown *)&DiagData );
				len = DiagData.data_edit.GetLength();
				memset( buf, '\0', sizeof(buf) );
				memmove( buf, DiagData.data_edit, len );
				memmove( (*m_pSnHeadData)->Sn_ZIP, buf, 8 );
				ZipBf =  (*m_pSnHeadData)->Sn_ZIP;
//<-- '15.05.11 INS END
				ZipBf =  (*m_pSnHeadData)->Sn_ZIP;
				if( ZipToAddress(ZipBf, AddBf) == TRUE ){
					// 納税地　住所(上)
					memset( buf, '\0', sizeof(buf) );
					len = AddBf.GetLength();
					memmove( buf, AddBf, len );
					memmove( (*m_pSnHeadData)->Sn_NOZEI, buf, 40 );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , buf, 40 );
					DiagData.data_edit = buff;
					m_KaniDiag02.SetData( ID2030, (struct IUnknown *)&DiagData );
					// 納税地　郵便番号
					len = ZipBf.GetLength();
					memset( buf, '\0', sizeof(buf) );
					memmove( buf, ZipBf, len );
					memmove( (*m_pSnHeadData)->Sn_ZIP, buf, 8 );
					DiagData.data_edit = buf;
					m_KaniDiag02.SetData( ID2020, (struct IUnknown *)&DiagData );
					m_KaniDiag02.SetPosition( ID2030 );
				}
				else{
					m_KaniDiag02.SetPosition( ID2020 );
				}
			}
			break;

		case ID2030 :	// 納税地　上段
			if( wTerm == VK_HOME ){
				ZipBf =  (*m_pSnHeadData)->Sn_ZIP;
				if( ZipToAddress(ZipBf, AddBf) == TRUE ){
					memset( buf, '\0', sizeof(buf) );
					len = AddBf.GetLength();
					memmove( buf, AddBf, len );
					memmove( (*m_pSnHeadData)->Sn_NOZEI, buf, 40 );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , buf, 40 );
					DiagData.data_edit = buff;
					m_KaniDiag02.SetData( ID2030, (struct IUnknown *)&DiagData );
				}
				m_KaniDiag02.SetPosition( ID2030 );
			}
			break;

		case ID2031 :	// 〃　　　下段
			if( wTerm == VK_HOME ){
				ZipBf =  (*m_pSnHeadData)->Sn_ZIP;
				if( ZipToAddress(ZipBf, AddBf) == TRUE ){
					memset( buf, '\0', sizeof(buf) );
					len = AddBf.GetLength();
					memmove( buf, AddBf, len );
					memmove( &(*m_pSnHeadData)->Sn_NOZEI[40], buf, 40 );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , buf , 40 ); 
					DiagData.data_edit = buff;
					m_KaniDiag02.SetData( ID2031, (struct IUnknown *)&DiagData );
				}
				m_KaniDiag02.SetPosition( ID2031 );
			}
			break;

		case ID2040 :	// 電話番号　市外
			break;
		case ID2041 :	// 〃　　　　市内
			break;
		case ID2042 :	// 〃		 局番
			break;
		case ID2050 :	// 名称又は屋号		（フリガナ）
			break;
		case ID2051 :	// 名称又は屋号
			break;
		case ID2052 :	// 被合併名
			break;
		case ID2060 :	// 代書者又は氏名	（フリガナ）
			break;
		case ID2061 :	// 代書者又は氏名
			break;
		case ID2070 :	// 経理担当者氏名
			break;
		case ID2080 :	// 一連番号	
			break;
		case ID2081 :	// 送付　要・不要
			break;
		case ID2082 :	// 所管		
			break;
		case ID2083 :	// 整理番号
			break;
		default     :
			break;

	}
	if( wTerm == VK_DELETE ){
		dsp_cls(IDC_ICSDIAGCTRL2 , index);
	}

	// 次項目
	int nowdiag = 2;
	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
		if( index == ID2083 ){
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){
					// 確定
//					if( pSyzShin->N_ver == 2 && !(*m_pSnHeadData)->SVmzsw ){
					if( !(*m_pSnHeadData)->SVmzsw ){
						// 経過措置無し
						m_KaniDiag03.SetPosition( ID3XM1 );	// 課税標準額
						nowdiag = 3;
					}
					else{
						// 経過措置有り
						m_KaniDiag03.SetPosition( ID3XM10 );	// 中間納付税額
						nowdiag = 3;
					}
				}
				else{
					// 中間
//					if( pSyzShin->N_ver == 2 && !(*m_pSnHeadData)->SVmzsw ) {
					if( !(*m_pSnHeadData)->SVmzsw ){
						// 経過措置無し
						m_KaniDiag03.SetPosition( ID3XM1 );	// 課税標準額
						nowdiag = 3;
					}
					else{
					// 経過措置有り
						if( (*m_pSnHeadData)->IsRevisedTaxReturn() ){
							m_KaniDiag03.SetPosition( ID3XM13 );	// 既確定税額
							nowdiag = 3;
						}
						else{
							if( SW_WARIFU ){
								m_KaniDiag04.SetPosition( ID4000 );
								nowdiag = 4;
							}
							else{
								m_KaniDiag04.SetPosition( ID4001 );
								nowdiag = 4;
							}
						}	
					}
				}
			}
			else{
				if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){
					// 確定
					// 経過措置有り
					m_KaniDiag03.SetPosition( ID3XM10 );	// 中間納付税額
					nowdiag = 3;
				}
				else{
					// 中間
					// 経過措置有り
					if( (*m_pSnHeadData)->IsRevisedTaxReturn() ){
						m_KaniDiag03.SetPosition( ID3XM13 );	// 既確定税額
						nowdiag = 3;
					}
					else{
						//[Y]中間の場合
						if( ((*m_pSnHeadData)->Sn_SKKBN&0x02) == 0x02 ){	
							if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//連動の場合
								m_KaniDiag03.SetPosition(ID3XM16);
								nowdiag = 3;
							}	
						}
						else{
							if( SW_WARIFU ){
								m_KaniDiag04.SetPosition( ID4000 );
								nowdiag = 4;
							}
							else{
								m_KaniDiag04.SetPosition( ID4001 );
								nowdiag = 4;
							}
						}
					}	
				}
			}
		}
		else{
			m_KaniDiag02.SetNextPosition();
		}
	}

	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
		if( index == ID2000 ){
			m_KaniDiag04.SetPosition( ID4ZI4 );
			nowdiag = 4;
		}
		else{
			m_KaniDiag02.SetPrevPosition();
		}
	}

	//[Y]
	if( (wTerm==VK_DELETE) ){
		dsp_cls(IDC_ICSDIAGCTRL2 , index);
		return;
	}

	//-->Editonから移動 harino ('14.03.07)
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	int nowpos;	// 移動先または自身のポジション
	if( nowdiag == 2 ) {
		nowpos = m_KaniDiag02.GetPosition();
		m_KaniDiag02.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	else if( nowdiag == 3 ) {
		nowpos = m_KaniDiag03.GetPosition();
		m_KaniDiag03.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	else if( nowdiag == 4 ) {
		nowpos = m_KaniDiag04.GetPosition();
		m_KaniDiag04.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	if ( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

void CShinKaniEX::EditONIcsdiagctrl3(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rectA, rectB;
	//GetWindowRect( &rectA );
	//m_KaniDiag03.GetRect( index, (LPUNKNOWN)&rectB );
	//if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	//}
	//-->Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL3;//[Y]
}

void CShinKaniEX::EditOFFIcsdiagctrl3(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	CkMy[MONY_BUF_SIZE];

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// データの読み込み
	m_KaniDiag03.GetData( index, (struct IUnknown *)&DiagData );
	switch( index ) {
		case ID3XM1	:	//  1. 課税標準額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, XM1, MONY_BUF_SIZE );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(XM1, CkMy) ){//[Y]
				m_M1c |= 0x01;
			}
//			m_M1c = ( l_cmp( XM1, CkMy ) ? 1 : 0 );
			memmove( XM1, CkMy, MONY_BUF_SIZE );
			if( m_M1c ) {
				// 各入力の計算表示
				calqx_all( 1, 0 );
				WriteData();
				calqx_otherlists();
				ReadData();
				DispKani();
			}
			else{
				// 各入力の計算表示
				calqx_all( 1, 0 );
			}
			break;
		case ID3XM2	:	//  2. 消費税額計
			//[Y]
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, XM2, MONY_BUF_SIZE );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(XM2, CkMy) ){
				m_M1c |= 0x08;
				m_M1c &= ~0x01;
			}
			memmove( XM2, CkMy, MONY_BUF_SIZE ); 

		//	memmove( XM2, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 2, 0 );
			WriteData();

			calqx_otherlists();
			ReadData();
			DispKani();
			break;
		case ID3XM3	:	//  3. 控除過大調整税額
			memmove( XM3, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 3, 0 );
			WriteData();

			calqx_otherlists();
			ReadData();
			DispKani();
			break;
		case ID3XM4	:	//  4. 控除対象仕入税額
			memmove( XM4, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 4, 0 );
			break;
		case ID3XM5	:	//  5. 返還等対価の税額
			memmove( XM5, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 5, 0 );
			WriteData();
			calqx_otherlists();
			ReadData();
			DispKani();
			break;
		case ID3XM6	:	//  6. 貸倒れに係る税額
			memmove( XM6, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 6, 0 );
			WriteData();
			calqx_otherlists();
			ReadData();
			DispKani();
			break;
		case ID3XM10:	// 10. 中間納付税額
			memmove( XM10, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 7, 0 );
			break;
		case ID3XM13:	// 13. 既確定税額
			memmove( XM13, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 8, 0 );
			break;
		case ID3XM15:	// 15. 課税売上高
			memmove( XM15, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 9, 0 );
			break;
		case ID3XM16:	// 16. 総売上高 or 基準期間の課税売上高
			memmove( XM16, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 10, 0 );
			break;
		case ID3XM21:	// 21. 中間納付譲渡割額	
			memmove( XM21, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 11, 0 );
			break;
		case ID3XM24:	// 24. 既確定譲渡割額
			memmove( XM24, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 12, 0 );
			break;
		default :
			break;
	}	
}

void CShinKaniEX::TerminationIcsdiagctrl3(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	if( nChar == 0 ){
		return;
	}

	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char )::GetKeyState( VK_SHIFT );
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

	// 次項目
	int nowdiag = 3;
	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
		if( (*m_pSnHeadData)->IsRevisedTaxReturn() ){
			if( index == ID3XM24 ){
				if( SW_WARIFU ){
					m_KaniDiag04.SetPosition( ID4000 );
					nowdiag = 4;
				}
				else{
					m_KaniDiag04.SetPosition( ID4001 );
					nowdiag = 4;
				}
			}
			else{
				m_KaniDiag03.SetNextPosition();
			}
		}
		else{
			if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){
				int Id;
				Id = ID3XM21;
				if( index == Id ){
					if( SW_WARIFU ){
						m_KaniDiag04.SetPosition( ID4000 );
						nowdiag = 4;
					}
					else{
						m_KaniDiag04.SetPosition( ID4001 );
						nowdiag = 4;
					}
				}
				else{
					m_KaniDiag03.SetNextPosition();
				}
			}
			else{
				if( index == ID3XM16 ){
					//[Y]
					if( SW_WARIFU ){
						m_KaniDiag04.SetPosition( ID4000 );
						nowdiag = 4;
					}
					else{
						m_KaniDiag04.SetPosition( ID4001 );
						nowdiag = 4;
					}
				}
				else{
					m_KaniDiag03.SetNextPosition();
				}
			}
		}
	}
	// 前項目
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
			if( (*m_pSnHeadData)->SVmzsw == 1 ){//[Y]経過措置がある場合
				if( index == ID3XM10 ){
					m_KaniDiag02.SetPosition( ID2083 );		// 整理番号
					nowdiag = 2;
				}
				else{
					m_KaniDiag03.SetPrevPosition();			// 戻り
				}
			}
			else{
				if( index == ID3XM1 ){
					m_KaniDiag02.SetPosition( ID2083 );		// 整理番号
					nowdiag = 2;
				}
				else{
					m_KaniDiag03.SetPrevPosition();			// 戻り
				}
			}
		}
		else{
			if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){
				if( index == ID3XM10 ){
					m_KaniDiag02.SetPosition( ID2083 );		// 整理番号
					nowdiag = 2;
				}
				else{
					m_KaniDiag03.SetPrevPosition();			// 戻り	
				}
			}
			else{
				//[Y]
				if( ((*m_pSnHeadData)->Sn_SKKBN&0x02) == 0x02 ){
					if( index == ID3XM16 ){
						m_KaniDiag02.SetPosition( ID2083 );		// 整理番号
						nowdiag = 2;
					}
					else{
						m_KaniDiag03.SetPrevPosition();			// 戻り	
					}
				}
				else{
					if( index == ID3XM13 ){
						m_KaniDiag02.SetPosition( ID2083 );		// 整理番号
						nowdiag = 2;
					}
					else{
						m_KaniDiag03.SetPrevPosition();			// 戻り	
					}
				}
			}
		}
	}

	//[Y]
	if( wTerm == VK_DELETE ){
		switch( index ){
			case ID3XM2	:	//  2. 消費税額計
				m_M1c &= ~0x08;
				m_M1c |= 0x01;
				// 各入力の計算表示
				calqx_all( 2, 0 );
				m_KaniDiag03.SetPosition( ID3XM2 );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL3 , index);//[Y]
				break;
		}
	}

	//-->Editonから移動 harino ('14.03.07)
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	int nowpos;	// 移動先または自身のポジション
	if( nowdiag == 2 ) {
		nowpos = m_KaniDiag02.GetPosition();
		m_KaniDiag02.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	else if( nowdiag == 3 ) {
		nowpos = m_KaniDiag03.GetPosition();
		m_KaniDiag03.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	else if( nowdiag == 4 ) {
		nowpos = m_KaniDiag04.GetPosition();
		m_KaniDiag04.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	if ( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

void CShinKaniEX::EditONIcsdiagctrl4(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rectA, rectB;
	//GetWindowRect( &rectA );
	//m_KaniDiag04.GetRect( index, (LPUNKNOWN)&rectB );
	//if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	//}
	//<--Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL4;//[Y]

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){
		case ID4000	:	// 割賦基準の適用
			if( !SW_WARIFU ){
				SW_WARIFU = 1;	// 割賦基準の適用
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4000, (struct IUnknown *)&DiagData );
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4001, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4000 );
			}
			break;
		case ID4001	:	// 割賦基準の適用
			if( SW_WARIFU ){
				SW_WARIFU = 0;	// 割賦基準の適用
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4000, (struct IUnknown *)&DiagData );
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4001, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4001 );
			}
			break;
		case ID4010	:	// 延払基準の適用
			if( !SW_NOBEBA ){
				SW_NOBEBA = 1;	// 延払基準の適用
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4010, (struct IUnknown *)&DiagData );
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4011, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4010 );
			}
			break;
		case ID4011	:	// 延払基準の適用
			if( SW_NOBEBA ){
				SW_NOBEBA = 0;	// 延払基準の適用
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4010, (struct IUnknown *)&DiagData );
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4011, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4011 );
			}
			break;
		case ID4020	:	// 工事進行基準の適用
			if( !SW_KOUJIS ){
				SW_KOUJIS = 1;	// 工事進行基準の適用
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4020, (struct IUnknown *)&DiagData );
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4021, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4020 );
			}
			break;
		case ID4021	:	// 工事進行基準の適用
			if( SW_KOUJIS ){
				SW_KOUJIS = 0;	// 工事進行基準の適用
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4020, (struct IUnknown *)&DiagData );
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4021, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4021 );
			}
			break;
		case ID4030	:	// 現金主義会計の適用
			if( !SW_GENKIN ){
				SW_GENKIN = 1;	// 現金主義会計の適用
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4030, (struct IUnknown *)&DiagData );
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4031, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4030 );
			}
			break;
		case ID4031 :	// 現金主義会計の適用
			if( SW_GENKIN ){
				SW_GENKIN = 0;	// 現金主義会計の適用
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4030, (struct IUnknown *)&DiagData );
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4031, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4031);
			}
			break;
		case ID4040	:	// 規則22条1項の適用
			if( !SW_KISK22 ){
				SW_KISK22 = 1;	// 規則22条1項の適用
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4040, (struct IUnknown *)&DiagData );
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4041, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4040 );
			}
			break;
		case ID4041	:	// 規則22条1項の適用
			if( SW_KISK22 ){
				SW_KISK22 = 0;	// 規則22条1項の適用
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4040, (struct IUnknown *)&DiagData );
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4041, (struct IUnknown *)&DiagData );
				m_KaniDiag04.SetPosition( ID4041 );
			}
			break;
		case	ID4160	:	// 特例計算適用
			if( !(*m_pSnHeadData)->Sn_TOKUR ){
				(*m_pSnHeadData)->Sn_TOKUR = 1;
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4160, (struct IUnknown *)&DiagData );
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4161, (struct IUnknown *)&DiagData );
				// 各入力の計算表示
				calqx_all( 1, 0 );
				WriteData();
				calqx_otherlists();
				ReadData();
				DispKani();
				if( !(*m_pSnHeadData)->Sn_TOKUR ){
					m_KaniDiag04.SetPosition( ID4161 );
				}
			}
			break;
		case	ID4161	:	// 特例計算適用
			if( (*m_pSnHeadData)->Sn_TOKUR ){
				(*m_pSnHeadData)->Sn_TOKUR = 0;
				DiagData.data_check = 0;
				m_KaniDiag04.SetData( ID4160, (struct IUnknown *)&DiagData );
				m_KaniDiag04.DeleteInput();
				DiagData.data_check = 1;
				m_KaniDiag04.SetData( ID4161, (struct IUnknown *)&DiagData );
				// 各入力の計算表示
				calqx_all( 1, 0 );
				WriteData();
				calqx_otherlists();
				ReadData();
				DispKani();
				if( (*m_pSnHeadData)->Sn_TOKUR ){
					m_KaniDiag04.SetPosition( ID4160 );
				}
			}
			break;
		case ID4ZI3 :	// 税理士法第３０条
			(*m_pSnHeadData)->Sn_ZeiHou &= 0xfe;
			m_KaniDiag04.GetData( ID4ZI3, (struct IUnknown *)&DiagData );
			if( DiagData.data_check ){
				(*m_pSnHeadData)->Sn_ZeiHou |= 0x01;
			}
			break;
		case ID4ZI4 :	// 税理士法第３３条の２
			(*m_pSnHeadData)->Sn_ZeiHou &= 0xfd;
			m_KaniDiag04.GetData( ID4ZI4, (struct IUnknown *)&DiagData );
			if( DiagData.data_check ){
				(*m_pSnHeadData)->Sn_ZeiHou |= 0x02;
			}
			break;
		default		:
			break;
	}
}

void CShinKaniEX::EditOFFIcsdiagctrl4(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			buf[512] = {0};
	int				len;

	switch( index ){

		case ID4000	:	// 割賦基準の適用
			break;
		case ID4001	:	// 割賦基準の適用
			break;
		case ID4010	:	// 延払基準の適用
			break;
		case ID4011	:	// 延払基準の適用
			break;
		case ID4020	:	// 工事進行基準の適用
			break;
		case ID4021	:	// 工事進行基準の適用
			break;
		case ID4030	:	// 現金主義会計の適用
			break;
		case ID4031 :	// 現金主義会計の適用
			break;
		case ID4040	:	// 規則22条1項の適用
			break;
		case ID4041	:	// 規則22条1項の適用
			break;
		case ID4160	:	// 特例計算適用
			break;
		case ID4161	:	// 特例計算適用
			break;
/*- CUT!! -*/
//		case ID4KM3	:	// 他 課税標準額
//			m_KaniDiag04.GetData( ID4KM3, (struct IUnknown *)&DiagData );
//			m_pArith->l_input( M1000, "1000" );
//			m_pArith->l_mul( pSyzShin->Snd.Sn_CK60, DiagData.data_val, M1000);
//			break;
//		case ID4SM3	:	// 他 消費税額
//			m_KaniDiag04.GetData( ID4SM3, (struct IUnknown *)&DiagData );
//			memmove( pSyzShin->Snd.Sn_CZ60, DiagData.data_val, 6 ); 
//			break;
/*---------*/
		case ID4210	:	// 金融機関 名
			memset( buf, '\0', sizeof(buf) );
			m_KaniDiag04.GetData( ID4210, (struct IUnknown *)&DiagData );
			len = (int)DiagData.data_edit.GetLength();
			if( len ){
				memmove( buf, DiagData.data_edit, len );
			}
			memmove( (*m_pSnHeadData)->Sn_BANK, buf, 20 );
			break;
		case ID4212 :	// 銀行・金庫等
			m_KaniDiag04.GetData( ID4212, (struct IUnknown *)&DiagData );
			(*m_pSnHeadData)->Sn_FINANCE = (char )DiagData.data_combo;
			break;
		case ID4211	:	// 金融機関 支店名
			memset( buf, '\0', sizeof(buf) );
			m_KaniDiag04.GetData( ID4211, (struct IUnknown *)&DiagData );
			len = (int)DiagData.data_edit.GetLength();
			if( len ){
				memmove( buf, DiagData.data_edit, len ); 
			}
			memmove( (*m_pSnHeadData)->Sn_SITEN, buf, 20 );
			break;
		case ID4213 :	// 本社・支社等
			m_KaniDiag04.GetData( ID4213, (struct IUnknown *)&DiagData );
			(*m_pSnHeadData)->Sn_CHAIN = (char )DiagData.data_combo;
			break;
		case ID4214	:	// 金融機関 預金種類
			memset( buf, '\0', sizeof(buf) );
			kspclr( buf, 4 );
			m_KaniDiag04.GetData( ID4214, (struct IUnknown *)&DiagData );
			len = (int)strlen( DiagData.data_edit );
			if( len ){
				memmove( buf, DiagData.data_edit, len ); 
			}
			memmove( (*m_pSnHeadData)->Sn_YOKIN, buf, 8 );
			break;
		case ID4215	:	// 金融機関 口座番号
			memset( buf, '\0', sizeof(buf) );
			memset( buf, 0x20, 10 );
			m_KaniDiag04.GetData( ID4215, (struct IUnknown *)&DiagData );
			len = (int)strlen( DiagData.data_edit );
			if( len ){
				memmove( &buf[10-len], DiagData.data_edit, len );
			}
			m_pArith->l_pack( (*m_pSnHeadData)->Sn_KOZA, buf, 10 );
			break;
		case ID4310	:	// 郵便局
			memset( buf, '\0', sizeof(buf) );
			memset( buf, 0x20, 5 );
			m_KaniDiag04.GetData( ID4310, (struct IUnknown *)&DiagData );
			len = (int)strlen( DiagData.data_edit );
			if( len ){
				memmove( &buf[5-len], DiagData.data_edit, len );
			}
			memmove( (*m_pSnHeadData)->Sn_YUNUM1, buf, 5 );
			break;
		case ID4311	:	// 郵便局
			memset( buf, '\0', sizeof(buf) );
			memset( buf, 0x20, 8 );
			m_KaniDiag04.GetData( ID4311, (struct IUnknown *)&DiagData );
			len = (int)strlen( DiagData.data_edit );
			if( len ){
				memmove( &buf[8-len], DiagData.data_edit, len );
			}
			memmove( (*m_pSnHeadData)->Sn_YUNUM2, buf, 8 );
		case ID4312	:	// 郵便局
			memset( buf, '\0', sizeof(buf) );
			m_KaniDiag04.GetData( ID4312, (struct IUnknown *)&DiagData );
			len = (int)DiagData.data_edit.GetLength();
			if( len ){
				memmove( buf, DiagData.data_edit, len );
			}
			memmove( (*m_pSnHeadData)->Sn_YUBIN, buf, 16 );
			break;
		case ID4410	:	// 郵便局
			m_KaniDiag04.GetData( ID4410, (struct IUnknown *)&DiagData );
			if( DiagData.data_combo ){
				(*m_pSnHeadData)->Sn_BYsw = 1;
			}
			else{
				(*m_pSnHeadData)->Sn_BYsw = 0;
			}
			break;
		case ID4RNO :	// 税務署整理欄
			m_KaniDiag04.GetData( ID4RNO, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			len = DiagData.data_edit.GetLength();
			memmove( buf, DiagData.data_edit, len );
			m_pArith->l_pack( (*m_pSnHeadData)->Sn_ZSEIRI, buf, 8 );
			break;
		case ID4ZI3 :	// 税理士法第３０条
			(*m_pSnHeadData)->Sn_ZeiHou &= 0xfe;
			m_KaniDiag04.GetData( ID4ZI3, (struct IUnknown *)&DiagData );
			if( DiagData.data_check ){
				(*m_pSnHeadData)->Sn_ZeiHou |= 0x01;
			}
			break;
		case ID4ZI4 :	// 税理士法第３３条の２
			(*m_pSnHeadData)->Sn_ZeiHou &= 0xfd;
			m_KaniDiag04.GetData( ID4ZI4, (struct IUnknown *)&DiagData );
			if( DiagData.data_check ){
				(*m_pSnHeadData)->Sn_ZeiHou |= 0x02;
			}
			break;
		default		:
			break;
	}	
}

void CShinKaniEX::TerminationIcsdiagctrl4(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	char			buf[512]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char )::GetKeyState( VK_SHIFT );
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

	//[Y]
	if( (wTerm==VK_DELETE) ){
		dsp_cls(IDC_ICSDIAGCTRL4 , index);
		return;
	}

	int nowdiag = 4;
	switch( index ){

		case ID4000	:	// 割賦基準の適用
		case ID4001	:	// 割賦基準の適用
			if((wTerm==VK_LEFT) || (wTerm==VK_RIGHT) ){
				if( SW_WARIFU ){
					SW_WARIFU = 0;	// 割賦基準の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4001, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4000, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4001 );
				}
				else{
					SW_WARIFU = 1;	// 割賦基準の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4000, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4001, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4000 );
				}
				break;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
				if( SW_NOBEBA ){
					m_KaniDiag04.SetPosition( ID4010 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4011 );
				}
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
					// 中間申告
					if( (*m_pSnHeadData)->IsRevisedTaxReturn() ){
						m_KaniDiag03.SetPosition( ID3XM24 );						// 中間修正	既確定譲渡割額	(24)
						nowdiag = 3;
					}
					else{
						//[Y]
						m_KaniDiag03.SetPosition( ID3XM16 );						//  6. 貸倒れに係る税額
						nowdiag = 3;
					}
				}
				else{
					// 確定
					if( (*m_pSnHeadData)->IsRevisedTaxReturn() ){
						m_KaniDiag03.SetPosition( ID3XM24 );						// 確定修正	既確定譲渡割額	(24)
						nowdiag = 3;
					}
					else{
						m_KaniDiag03.SetPosition( ID3XM21 );						// 確定		中間納付譲渡割額(21)
						nowdiag = 3;
					}
				}
			}
			break;
		case ID4010	:	// 延払基準の適用
		case ID4011	:	// 延払基準の適用
			if( (wTerm==VK_LEFT) || (wTerm==VK_RIGHT) ){
				if( SW_NOBEBA ){
					SW_NOBEBA = 0;	// 延払基準の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4011, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4010, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4011 );
				}
				else{
					SW_NOBEBA = 1;	// 延払基準の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4010, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4011, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4010 );
				}
				break;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
				if( SW_KOUJIS ){
					m_KaniDiag04.SetPosition( ID4020 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4021 );
				}
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				if( SW_WARIFU ){
					m_KaniDiag04.SetPosition( ID4000 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4001 );
				}
			}
			break;
		case ID4020	:	// 工事進行基準の適用
		case ID4021	:	// 工事進行基準の適用
			if( (wTerm==VK_LEFT) || (wTerm==VK_RIGHT) ){
				if( SW_KOUJIS ){
					SW_KOUJIS = 0;	// 工事進行基準の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4021, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4020, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4021 );
				}
				else{
					SW_KOUJIS = 1;	// 工事進行基準の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4020, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4021, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4020 );
				}
				break;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
				if( SW_GENKIN ){
					m_KaniDiag04.SetPosition( ID4030 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4031 );
				}
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				if( SW_NOBEBA ){
					m_KaniDiag04.SetPosition( ID4010 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4011 );
				}
			}
			break;
		case ID4030	:	// 現金主義会計の適用
		case ID4031	:	// 現金主義会計の適用
			if( (wTerm==VK_LEFT) || (wTerm==VK_RIGHT) ){
				if( SW_GENKIN ){
					SW_GENKIN = 0;	// 現金主義会計の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4031, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4030, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4031 );
				}
				else{
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4030, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4031, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4030 );
				}
				break;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4210 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				if( SW_KOUJIS ){
					m_KaniDiag04.SetPosition( ID4020 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4021 );
				}
			}
			break;
		case ID4040	:	// 規則22条1項の適用
		case ID4041	:	// 規則22条1項の適用
			if( (wTerm==VK_LEFT) || (wTerm==VK_RIGHT) ){
				if( SW_KISK22 ){
					SW_KISK22 = 0;	// 規則22条1項の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4041, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4040, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4041 );
				}
				else{
					SW_KISK22 = 1;	// 規則22条1項の適用
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4040, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4041, (struct IUnknown *)&DiagData );
					m_KaniDiag04.SetPosition( ID4040 );
				}
				break;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
				if( SW_KISK22 ){
					m_KaniDiag04.SetPosition( ID4160 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4161 );
				}
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				if( SW_GENKIN ){
					m_KaniDiag04.SetPosition( ID4030 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4031 );
				}
			}
			break;
		case	ID4160	:	// 特例計算適用
		case	ID4161	:	// 特例計算適用
			if( (wTerm==VK_LEFT) || (wTerm==VK_RIGHT) ){
				if( (*m_pSnHeadData)->Sn_TOKUR ){
					(*m_pSnHeadData)->Sn_TOKUR = 0x00;
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4161, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4160, (struct IUnknown *)&DiagData );
				}
				else{
					(*m_pSnHeadData)->Sn_TOKUR = 0x01;
					DiagData.data_check = 1;
					m_KaniDiag04.SetData( ID4160, (struct IUnknown *)&DiagData );
					DiagData.data_check = 0;
					m_KaniDiag04.SetData( ID4161, (struct IUnknown *)&DiagData );
				}
				// 各入力の計算表示
				calqx_all( 1, 0 );
				WriteData();
				calqx_otherlists();
				ReadData();
				DispKani();
				if( (*m_pSnHeadData)->Sn_TOKUR ){
					m_KaniDiag04.SetPosition( ID4160 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4161 );
				}
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4KM3 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				if( SW_KISK22 ){
					m_KaniDiag04.SetPosition( ID4040 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4041 );
				}
			}
			break;
//		case ID4KM3	:	// 自動車　課税標準額　表示 6.0%
//			// 次項目
//			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
//				m_KaniDiag04.SetPosition( ID4SM3 );
//			}
//			// 前項目へ
//			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//				if( SW_GENKIN ){
//				if( 0 ){
//					m_KaniDiag04.SetPosition( ID4030 );
//				}
//				else{
//					m_KaniDiag04.SetPosition( ID4031 );
//				}
//			}
//			break;
//		case ID4SM3	:	// 自動車　消費税額　　表示 6.0%
//			// 次項目
//			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
//				m_KaniDiag04.SetPosition( ID4210 );
//			}
//			// 前項目へ
//			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//				m_KaniDiag04.SetPosition(ID4KM3 );
//			}
//			break;
		case ID4210	:	// 金融機関 名
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4212 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
/*- '14.01.20 -*/
//				m_KaniDiag04.SetPosition( ID4SM3 );
/*-------------*/
				if( SW_GENKIN ){
					m_KaniDiag04.SetPosition( ID4030 );
				}
				else{
					m_KaniDiag04.SetPosition( ID4031 );
				}
/*-------------*/
			}
			break;
		case ID4212	:	// 銀行・金庫等
			// 矢印制御
			if( wTerm == VK_DOWN ){
				m_KaniDiag04.ComboDropDown( ID4212, TRUE );
				return;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) ){
				m_KaniDiag04.SetPosition( ID4211 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
				m_KaniDiag04.SetPosition(ID4210 );
			}
			break;
		case ID4211	:	// 金融機関 支店名
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4213 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition(ID4212 );
			}
			break;
		case ID4213	:	// 本店・支店等
			// 矢印制御
			if( wTerm == VK_DOWN ){
				m_KaniDiag04.ComboDropDown( ID4213, TRUE );
				return;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) ){
				m_KaniDiag04.SetPosition( ID4214 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
				m_KaniDiag04.SetPosition(ID4211 );
			}
			break;
		case ID4214	:	// 金融機関 預金種名
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4215 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition(ID4213 );
			}
			break;
		case ID4215	:	// 金融機関 口座番号
			if( wTerm == VK_DELETE ){
				memset( (*m_pSnHeadData)->Sn_KOZA, 0xff, 8 );
				memset( buf, '\0', sizeof(buf) );
				memset( buf, 0x20, 10 );
				dsp_cls( IDC_ICSDIAGCTRL4, ID4215 );
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4310 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition(ID4214);
			}
			break;
		case ID4310	:	// 郵便局
			if( wTerm == VK_DELETE ){
				memset( (*m_pSnHeadData)->Sn_YUNUM1, 0x20, 5 );
				dsp_cls( IDC_ICSDIAGCTRL4, ID4310 );
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4311 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition( ID4215 );
			}
			break;
		case ID4311	:	// 郵便局
			if( wTerm == VK_DELETE ) {
				memset( (*m_pSnHeadData)->Sn_YUNUM2, 0x20, 8 );
				dsp_cls( IDC_ICSDIAGCTRL4, ID4311 );
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4312 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition( ID4310 );
			}
			break;
		case ID4312	:	// 郵便局
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4410 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition( ID4311 );
			}
			break;
		case ID4410	:	// 銀行or郵便局
			// 矢印制御
			if( wTerm == VK_DOWN ){
				m_KaniDiag04.ComboDropDown( ID4410, TRUE );
				return;
			}
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) ){
				m_KaniDiag04.SetPosition( ID4RNO );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
				m_KaniDiag04.SetPosition( ID4312 );
			}
			break;
		case ID4RNO	:	// 税務署処理欄
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4ZI3 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition( ID4410 );
			}
			break;
		case ID4ZI3 :	// 税理士法第３０条
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag04.SetPosition( ID4ZI4 );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition( ID4RNO );
			}
			break;
		case ID4ZI4 :	// 税理士法第３３条の２
			// 次項目
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) || (wTerm==VK_DOWN) ){
				m_KaniDiag02.SetPosition( ID2000 );
				nowdiag = 2;
			}	
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
				m_KaniDiag04.SetPosition(ID4ZI3 );
			}
			break;
		default		:
			break;
	}

	//-->Editonから移動 harino ('14.03.07)
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	int nowpos;	// 移動先または自身のポジション
	if( nowdiag == 2 ) {
		nowpos = m_KaniDiag02.GetPosition();
		m_KaniDiag02.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	else if( nowdiag == 3 ) {
		nowpos = m_KaniDiag03.GetPosition();
		m_KaniDiag03.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	else if( nowdiag == 4 ) {
		nowpos = m_KaniDiag04.GetPosition();
		m_KaniDiag04.GetRect( nowpos, (LPUNKNOWN)&rectB );
	}
	if ( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

//-----------------------------------------------------------------------------
// 最終データ読込み
//-----------------------------------------------------------------------------
void CShinKaniEX::GetDiagData()
{
	if( m_KaniDiag03.GetPosition() == ID3XM1 ){
		m_KaniDiag02.SetFocus();
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	//  1. 課税標準額
	m_KaniDiag03.GetData( ID3XM1, (struct IUnknown *)&DiagData );
	memmove(  XM1, DiagData.data_val, MONY_BUF_SIZE ); 
	//  2. 消費税額計
	m_KaniDiag03.GetData( ID3XM2, (struct IUnknown *)&DiagData );
	memmove( XM2, DiagData.data_val, MONY_BUF_SIZE ); 
	//  3. 控除過大調整税額
	m_KaniDiag03.GetData( ID3XM3, (struct IUnknown *)&DiagData );
	memmove( XM3, DiagData.data_val, MONY_BUF_SIZE ); 
	//  4. 控除対象仕入税額
	m_KaniDiag03.GetData( ID3XM4, (struct IUnknown *)&DiagData );
	memmove( XM4, DiagData.data_val, MONY_BUF_SIZE ); 
	//  5. 返還等対価の税額
	m_KaniDiag03.GetData( ID3XM5, (struct IUnknown *)&DiagData );
	memmove( XM5, DiagData.data_val, MONY_BUF_SIZE ); 
	//  6. 貸倒れに係る税額
	m_KaniDiag03.GetData( ID3XM6, (struct IUnknown *)&DiagData );
	memmove( XM6, DiagData.data_val, MONY_BUF_SIZE ); 
	// 10. 中間納付税額
	m_KaniDiag03.GetData( ID3XM10, (struct IUnknown *)&DiagData );
	memmove( XM10, DiagData.data_val, MONY_BUF_SIZE ); 
	// 13. 既確定税額
	m_KaniDiag03.GetData( ID3XM13, (struct IUnknown *)&DiagData );
	memmove( XM13, DiagData.data_val, MONY_BUF_SIZE ); 
	// 15. 課税売上高
	m_KaniDiag03.GetData( ID3XM15, (struct IUnknown *)&DiagData );
	memmove( XM15, DiagData.data_val, MONY_BUF_SIZE ); 
	// 16. 総売上高 or 基準期間の課税売上高
	m_KaniDiag03.GetData( ID3XM16, (struct IUnknown *)&DiagData );
	memmove( XM16, DiagData.data_val, MONY_BUF_SIZE ); 
	// 21. 中間納付譲渡割額	
	m_KaniDiag03.GetData( ID3XM21, (struct IUnknown *)&DiagData );
	memmove( XM21, DiagData.data_val, MONY_BUF_SIZE ); 
	// 24. 既確定譲渡割額
	m_KaniDiag03.GetData( ID3XM24, (struct IUnknown *)&DiagData );
	memmove( XM24, DiagData.data_val, MONY_BUF_SIZE ); 

/*- '14.10.25_CUT -*/
//	// 自動車　課税標準額　表示 6.0%
//	char	M1000[MONY_BUF_SIZE] = {0};
//	m_KaniDiag04.GetData( ID4KM3, (struct IUnknown *)&DiagData );
//	m_pArith->l_input( M1000, "1000" );
////	m_pArith->l_mul( pSyzShin->Snd.Sn_CK60, DiagData.data_val, M1000 ); 
//	// 自動車　消費税額　　表示 6.0%
////	m_KaniDiag04.GetData( ID4SM3, (struct IUnknown *)&DiagData );
////	memmove( pSyzShin->Snd.Sn_CZ60, DiagData.data_val, MONY_BUF_SIZE ); 
/*------------------*/
}

void CShinKaniEX::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	char			buf[512] = {0};
	int				len;
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	if( *m_pShinInfo->pIsOpenEselect ){
		memset( m_pShinInfo->pEselect, '\0', sizeof(ESELECT) );
		m_pShinInfo->pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
		if( (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!=0x00) && (((*m_pSnHeadData)->Sn_TAXNO[0]&0xff)!='F') ){
			memmove( m_pShinInfo->pEselect->cd_office, (*m_pSnHeadData)->Sn_TAXNO, sizeof((*m_pSnHeadData)->Sn_TAXNO) );
		}
		
		if( Esel_SelectTaxOffice(m_pShinInfo->pEselect) == IDOK ){
			memmove( (*m_pSnHeadData)->Sn_TAXNO, m_pShinInfo->pEselect->cd_office, 5 );
			memset( buf, '\0', sizeof(buf) );
			len = (int)strlen( m_pShinInfo->pEselect->nm_office );
			memmove( buf, m_pShinInfo->pEselect->nm_office, len );
			memmove( (*m_pSnHeadData)->Sn_ZMSYO, buf, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
			// 税務署名
			len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
			memset( buf, '\0', sizeof(buf) );
			memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, len );
			DiagData.data_edit = buf;
			m_KaniDiag02.SetData( ID2010, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), "署番号 %-5.5s", (*m_pSnHeadData)->Sn_TAXNO );
			DiagData.data_disp = buf;
			m_KaniDiag02.SetData( ID2011, (struct IUnknown *)&DiagData );
		}
	}
}

//-----------------------------------------------------------------------------
// 他表からの計算
//-----------------------------------------------------------------------------
// 引数	ksw	：	簡易課税切替スイッチ
//-----------------------------------------------------------------------------
//Kanisyo_calq
void CShinKaniEX::calq_by_OtherChange( int ksw )
{
	m_DpSw = -1;
	m_kanisw = ksw;
	ReadData();
	calqx_all( 6, 1 );
	WriteData();
	m_DpSw = 0;
}

//-----------------------------------------------------------------------------
// 入力位置のセット
//-----------------------------------------------------------------------------
// 引数	no	：	
//-----------------------------------------------------------------------------
void CShinKaniEX::SetItemPosition(int no )
{
	int index;

	//-->追加 harino ('14.03.06)
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		if( (index = m_KaniDiag02.GetPosition()) != -1) {
			m_KaniDiag02.SetPosition(index);
			m_KaniDiag02.SetFocus();
			return;
		}
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL3 ) {
		if( (index = m_KaniDiag03.GetPosition()) != -1) {
			m_KaniDiag03.SetPosition(index);
			m_KaniDiag03.SetFocus();
			return;
		}
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL4 ) {
		if( (index = m_KaniDiag04.GetPosition()) != -1) {
			m_KaniDiag04.SetPosition(index);
			m_KaniDiag04.SetFocus();
			return;
		}
	}
	//<--追加 harino ('14.03.06)

	m_KaniDiag03.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_KaniDiag02.SetFocus();

	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		return;
	}

	m_KaniDiag02.SetPosition( 71 );

	CRect	rc;
	GetClientRect( &rc );
	InvalidateRect( &rc );
	SetRedrawScroll( 0 );
}

//-----------------------------------------------------------------------------
//[Y] 欄外印字
//-----------------------------------------------------------------------------
void CShinKaniEX::PerPrint()
{
	// 欄外印字
	CPrePrint PPDialog;
	PPDialog.InitInfo( (*m_pSnHeadData) );

	HINSTANCE svhInstResource = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );
	PPDialog.DoModal();
	AfxSetResourceHandle( svhInstResource );
}
//2016.02.23 INSERT START
//-----------------------------------------------------------------------------
// 欄外メモ
//-----------------------------------------------------------------------------
void CShinKaniEX::PerPrint2()
{
	// 欄外印字
	CPrePrint2 PPDialog;
	PPDialog.InitInfo( (*m_pSnHeadData) );

	HINSTANCE svhInstResource = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );
	PPDialog.DoModal();
	AfxSetResourceHandle( svhInstResource );
}
//2016.02.23 INSERT END

//-----------------------------------------------------------------------------
//[Y] 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinKaniEX::ChangeColor( unsigned short id, short Index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, Index, &DA, CIcsdiagctrl );
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
	diag_setattr( id, Index, &DA, FALSE, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算方法の判定	('15.01.27)
//-----------------------------------------------------------------------------
// 返送値	1	：	D列は別計算
//			0	：	D列はA+B+C
//-----------------------------------------------------------------------------
int CShinKaniEX::judge_skj_calq()
{
	if( m_pParent ){
		if( ((CH26HyoView*)m_pParent)->IsNeedExpCalq() ){
			return 1;
		}
	}

	return 0;
}

//2016.06.22 INSERT START
BOOL CShinKaniEX::IsKanpu()
{
	if( m_pArith->l_test( XM26 ) < 0 ){
		return TRUE;
	}

	return FALSE;
}
//2016.06.22 INSERT END

//-----------------------------------------------------------------------------
// 税務署入力にフォーカスセット('15.02.03)
//-----------------------------------------------------------------------------
void CShinKaniEX::SetTaxOfficeInput()
{
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		return;
	}

	m_KaniDiag02.SetPosition( 74 );

	CRect	rc;
	GetClientRect( &rc );
	InvalidateRect( &rc );
	SetRedrawScroll( 0 );
}

//-----------------------------------------------------------------------------
// 入力位置を保存 ('15.03.02)
//-----------------------------------------------------------------------------
void CShinKaniEX::SaveInputPosition()
{
	m_curInPos = -1;

	int index;
	if( m_curwnd == IDC_ICSDIAGCTRL2 ){
		if( (index=m_KaniDiag02.GetPosition()) != -1 ){
			m_curInPos = index;
		}
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL3 ){
		if( (index=m_KaniDiag03.GetPosition()) != -1 ){
			m_curInPos = index;
		}
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL4 ){
		if( (index=m_KaniDiag04.GetPosition()) != -1 ){
			m_curInPos = index;
		}
	}

	if( m_curInPos != -1 ){
		m_KaniDiag02.SetPosition( 71 );
	}
}

//-----------------------------------------------------------------------------
// 入力位置を復元 ('15.03.02)
//-----------------------------------------------------------------------------
void CShinKaniEX::RestoreInputPosition()
{
	if( m_curInPos == -1 ){
		return;
	}
	
	if( m_curwnd == IDC_ICSDIAGCTRL2 ){
		m_KaniDiag02.SetPosition(m_curInPos);
		m_KaniDiag02.SetFocus();
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL3 ){
		m_KaniDiag03.SetPosition(m_curInPos);
		m_KaniDiag03.SetFocus();
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL4 ){
		m_KaniDiag04.SetPosition(m_curInPos);
		m_KaniDiag04.SetFocus();
	}
	m_curInPos = -1;
}

//-----------------------------------------------------------------------------
// 終了時のフォーカス移動 ('15.03.02)
//-----------------------------------------------------------------------------
void CShinKaniEX::MoveFocusInEnd()
{
	// データを確定させる為にフォーカス移動
	if( m_curwnd == IDC_ICSDIAGCTRL2 ){
		if( m_KaniDiag02.GetPosition() == ID2010 ){
			m_KaniDiag02.SetPosition( ID2000 );
		}
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL4 ){
		if( m_KaniDiag04.GetPosition() == ID4410 ){
			m_KaniDiag04.SetPosition( ID4RNO );
		}
	}
}

//-----------------------------------------------------------------------------
// 還付金融機関入力にフォーカスセット('16.12.26)
//-----------------------------------------------------------------------------
void CShinKaniEX::SetKanpuKinyuInput()
{
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		return;
	}

	m_KaniDiag04.SetPosition( ID4210 );

	CRect	rc;
	GetClientRect( &rc );
	InvalidateRect( &rc );
//	SetRedrawScroll( 0 );

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	int nowpos;	// 移動先または自身のポジション
	nowpos = m_KaniDiag04.GetPosition();
	m_KaniDiag04.GetRect( nowpos, (LPUNKNOWN)&rectB );

	if ( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}
