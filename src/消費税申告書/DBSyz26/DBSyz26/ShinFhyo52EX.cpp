// ShinFhyo52.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo52EX.h"
#include "ShinFhyo52EXIdx.h"

#define	SIROIROCOLOR	PALETTERGB( 255, 251, 240 )

// CShinFhyo52EX ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo52EX, CSyzBaseDlg)

CShinFhyo52EX::CShinFhyo52EX(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo52EX::IDD, pParent)
	, m_pSnFhyo52Data(NULL)
	, m_pSnFhyo40Data(NULL)
	, m_pSnHonpyoData(NULL)
	, m_pSnHeadData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_typeCnt(0)
	, m_FrSw(0)
	, m_Tmp(0)
	, m_DpSw(0)
	, m_CQsw(0)
	, m_pParent(pParent)
	, m_Szsw(0)
	, m_SBsw(0)
	, m_tkSw(0)
	, m_pSubSyzSyukei(NULL)
	, m_dspSkjSw(0)
	, m_SvSzsw(-1)
{
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

	memset( F526T, '\0', sizeof(F521T) );
	memset( F521A, '\0', sizeof(F521A) );
	memset( F521B, '\0', sizeof(F521B) );
	memset( F521C, '\0', sizeof(F521C) );

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

	memset( F5210T, '\0', sizeof(F5210T) );
	memset( F5210A, '\0', sizeof(F5210A) );
	memset( F5210B, '\0', sizeof(F5210B) );
	memset( F5210C, '\0', sizeof(F5210C) );

	memset( F5211T, '\0', sizeof(F5211T) );
	memset( F5211A, '\0', sizeof(F5211A) );
	memset( F5211B, '\0', sizeof(F5211B) );
	memset( F5211C, '\0', sizeof(F5211C) );

	memset( F5212T, '\0', sizeof(F5212T) );
	memset( F5212A, '\0', sizeof(F5212A) );
	memset( F5212B, '\0', sizeof(F5212B) );
	memset( F5212C, '\0', sizeof(F5212C) );

	memset( F5213T, '\0', sizeof(F5213T) );
	memset( F5213A, '\0', sizeof(F5213A) );
	memset( F5213B, '\0', sizeof(F5213B) );
	memset( F5213C, '\0', sizeof(F5213C) );


	memset( F5214T, '\0', sizeof(F5214T) );
	memset( F5214A, '\0', sizeof(F5214A) );
	memset( F5214B, '\0', sizeof(F5214B) );
	memset( F5214C, '\0', sizeof(F5214C) );

	memset( F5215T, '\0', sizeof(F5215T) );
	memset( F5215A, '\0', sizeof(F5215A) );
	memset( F5215B, '\0', sizeof(F5215B) );
	memset( F5215C, '\0', sizeof(F5215C) );

	memset( F5216T, '\0', sizeof(F5216T) );
	memset( F5216A, '\0', sizeof(F5216A) );
	memset( F5216B, '\0', sizeof(F5216B) );
	memset( F5216C, '\0', sizeof(F5216C) );

	memset( F5217T, '\0', sizeof(F5217T) );
	memset( F5217A, '\0', sizeof(F5217A) );
	memset( F5217B, '\0', sizeof(F5217B) );
	memset( F5217C, '\0', sizeof(F5217C) );

	memset( F5218T, '\0', sizeof(F5218T) );
	memset( F5218A, '\0', sizeof(F5218A) );
	memset( F5218B, '\0', sizeof(F5218B) );
	memset( F5218C, '\0', sizeof(F5218C) );

	memset( F5219T, '\0', sizeof(F5219T) );
	memset( F5219A, '\0', sizeof(F5219A) );
	memset( F5219B, '\0', sizeof(F5219B) );
	memset( F5219C, '\0', sizeof(F5219C) );

	memset( F5220T, '\0', sizeof(F5220T) );
	memset( F5220A, '\0', sizeof(F5220A) );
	memset( F5220B, '\0', sizeof(F5220B) );
	memset( F5220C, '\0', sizeof(F5220C) );

	memset( F5221T, '\0', sizeof(F5221T) );
	memset( F5221A, '\0', sizeof(F5221A) );
	memset( F5221B, '\0', sizeof(F5221B) );
	memset( F5221C, '\0', sizeof(F5221C) );

	memset( F5222T, '\0', sizeof(F5222T) );
	memset( F5222A, '\0', sizeof(F5222A) );
	memset( F5222B, '\0', sizeof(F5222B) );
	memset( F5222C, '\0', sizeof(F5222C) );

	memset( F5223T, '\0', sizeof(F5223T) );
	memset( F5223A, '\0', sizeof(F5223A) );
	memset( F5223B, '\0', sizeof(F5223B) );
	memset( F5223C, '\0', sizeof(F5223C) );

	memset( F5224T, '\0', sizeof(F5224T) );
	memset( F5224A, '\0', sizeof(F5224A) );
	memset( F5224B, '\0', sizeof(F5224B) );
	memset( F5224C, '\0', sizeof(F5224C) );

	memset( F5225T, '\0', sizeof(F5225T) );
	memset( F5225A, '\0', sizeof(F5225A) );
	memset( F5225B, '\0', sizeof(F5225B) );
	memset( F5225C, '\0', sizeof(F5225C) );

	memset( F5226T, '\0', sizeof(F5226T) );
	memset( F5226A, '\0', sizeof(F5226A) );
	memset( F5226B, '\0', sizeof(F5226B) );
	memset( F5226C, '\0', sizeof(F5226C) );

	memset( F5227T, '\0', sizeof(F5227T) );
	memset( F5227A, '\0', sizeof(F5227A) );
	memset( F5227B, '\0', sizeof(F5227B) );
	memset( F5227C, '\0', sizeof(F5227C) );

	memset( F5228T, '\0', sizeof(F5228T) );
	memset( F5228A, '\0', sizeof(F5228A) );
	memset( F5228B, '\0', sizeof(F5228B) );
	memset( F5228C, '\0', sizeof(F5228C) );

	memset( F5229T, '\0', sizeof(F5229T) );
	memset( F5229A, '\0', sizeof(F5229A) );
	memset( F5229B, '\0', sizeof(F5229B) );
	memset( F5229C, '\0', sizeof(F5229C) );

	memset( F5230T, '\0', sizeof(F5230T) );
	memset( F5230A, '\0', sizeof(F5230A) );
	memset( F5230B, '\0', sizeof(F5230B) );
	memset( F5230C, '\0', sizeof(F5230C) );

	memset( F5231T, '\0', sizeof(F5231T) );
	memset( F5231A, '\0', sizeof(F5231A) );
	memset( F5231B, '\0', sizeof(F5231B) );
	memset( F5231C, '\0', sizeof(F5231C) );

	memset( F5232T, '\0', sizeof(F5232T) );
	memset( F5232A, '\0', sizeof(F5232A) );
	memset( F5232B, '\0', sizeof(F5232B) );
	memset( F5232C, '\0', sizeof(F5232C) );

	memset( F5233T, '\0', sizeof(F5233T) );
	memset( F5233A, '\0', sizeof(F5233A) );
	memset( F5233B, '\0', sizeof(F5233B) );
	memset( F5233C, '\0', sizeof(F5233C) );

	memset( F5234T, '\0', sizeof(F5234T) );
	memset( F5234A, '\0', sizeof(F5234A) );
	memset( F5234B, '\0', sizeof(F5234B) );
	memset( F5234C, '\0', sizeof(F5234C) );

	memset( F5235T, '\0', sizeof(F5235T) );
	memset( F5235A, '\0', sizeof(F5235A) );
	memset( F5235B, '\0', sizeof(F5235B) );
	memset( F5235C, '\0', sizeof(F5235C) );

	memset( F5236T, '\0', sizeof(F5236T) );
	memset( F5236A, '\0', sizeof(F5236A) );
	memset( F5236B, '\0', sizeof(F5236B) );
	memset( F5236C, '\0', sizeof(F5236C) );

	memset( F5237T, '\0', sizeof(F5237T) );
	memset( F5237A, '\0', sizeof(F5237A) );
	memset( F5237B, '\0', sizeof(F5237B) );
	memset( F5237C, '\0', sizeof(F5237C) );


	F520c = F521c = F522c = F523c = F524c = F525c = F526c = 0;
	F5213c = F5214c = F5215c = F5216c = F5217c = F5218c = F5219c = 0;
	F5220c = F5221c = 0;
	F5222c = F5223c = F5224c = F5225c = F5226c = F5227c = F5228c = F5229c = F5230c = F5231c = F5232c = F5233c = F5234c = F5235c = F5236c = 0;

	m_curwnd = -1;

//--> '14.06.10 INS START
	memset( &m_Fh5Mony, '\0', sizeof(SH_FH5_MONY) );
//<-- '14.06.10 INS END
}

CShinFhyo52EX::~CShinFhyo52EX()
{
}

void CShinFhyo52EX::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo52Diag01);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo52Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo52EX, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFhyo52EX メッセージ ハンドラ

BOOL CShinFhyo52EX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			WriteData();
//			CWnd*	p1	=	GetParent();
//			CWnd*	p2	=	p1->GetParent();
//			((CSyzShinSheet*)p2)->Fhyo4_calq();
//			((CSyzShinSheet*)p2)->Kanik_calq( 1 );
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
				return TRUE;
			}
		}
		//<--追加 harino ('14.03.04)
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

BOOL CShinFhyo52EX::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo52Diag02;
	m_BottomIdx = 176;

	// 金額フィールドの連結
	ConnectMonyField();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 金額フィールドの連結	('14.06.10)
//-----------------------------------------------------------------------------
void CShinFhyo52EX::ConnectMonyField()
{
	memset( &m_Fh5Mony, '\0', sizeof(SH_FH5_MONY) );

	m_Fh5Mony.pF5213[0] = F5213T;
	m_Fh5Mony.pF5213[1] = F5213A;
	m_Fh5Mony.pF5213[2] = F5213B;
	m_Fh5Mony.pF5213[3] = F5213C;

	m_Fh5Mony.pF5214[0] = F5214T;
	m_Fh5Mony.pF5214[1] = F5214A;
	m_Fh5Mony.pF5214[2] = F5214B;
	m_Fh5Mony.pF5214[3] = F5214C;

	m_Fh5Mony.pF5215[0] = F5215T;
	m_Fh5Mony.pF5215[1] = F5215A;
	m_Fh5Mony.pF5215[2] = F5215B;
	m_Fh5Mony.pF5215[3] = F5215C;

	m_Fh5Mony.pF5216[0] = F5216T;
	m_Fh5Mony.pF5216[1] = F5216A;
	m_Fh5Mony.pF5216[2] = F5216B;
	m_Fh5Mony.pF5216[3] = F5216C;

	m_Fh5Mony.pF5217[0] = F5217T;
	m_Fh5Mony.pF5217[1] = F5217A;
	m_Fh5Mony.pF5217[2] = F5217B;
	m_Fh5Mony.pF5217[3] = F5217C;

	m_Fh5Mony.pF5218[0] = F5218T;
	m_Fh5Mony.pF5218[1] = F5218A;
	m_Fh5Mony.pF5218[2] = F5218B;
	m_Fh5Mony.pF5218[3] = F5218C;

	m_Fh5Mony.pF5219[0] = F5219T;
	m_Fh5Mony.pF5219[1] = F5219A;
	m_Fh5Mony.pF5219[2] = F5219B;
	m_Fh5Mony.pF5219[3] = F5219C;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo52EX::Init()
{
//--> '14.05.16 INS START
	if( ((*m_pSnFhyo52Data)==NULL) || ((*m_pSnFhyo40Data)==NULL) || ((*m_pSnHonpyoData)==NULL) ){
		return -1;
	}
//<-- '14.05.16 INS END

	ReadData( 1 );
//--> '15.04.27 INS START
	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){
		// 連動時
		m_CQsw &= 0x0f;
	}
//<-- '15.04.27 INS END
	m_FrSw = 1;

	m_Tmp = 0;
	f52calq_1();				// 課税標準額消費税額
	f52calq_2();				// 貸倒回収消費税額
	f52calq_3();				// 売上対価の返還等消費税額
	f52calq_4();				// （１）＋（２）－（３）
	f52calq_gsubtotal();
	f52calq_5();
	if( m_typeCnt > 1 ){
		f52calq_ku();			// 事業別課税売上高の計算
		f52calq_gks();			// 課税売上に係る税額計
	}
	else{
		m_Tmp = 1;
	}

	InitDiagAttr();

	// 画面表示
	DispFh52();

	//-->削除 harino ('14.03.06)
	//if( m_pSnHeadData->Sn_Sign4&0x80 ){
	//	diag_setposition( IDC_ICSDIAGCTRL2, ID527A, CIcsdiagctrl );
	//}
	//<--削除 harino ('14.03.06)

 	return 0;
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnFhyo52Data	：	付表５－２データ読込みクラス
//		pSnFhyo40Data	：	付表４データ読込みクラス
//		pSnHonpyoData	：	本表データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pZmSub			：	公益クラス
//		pSubSyzSyukei	：	集計クラス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo52EX::InitInfo( CH27SnFhyo50Data **pSnFhyo52Data, CH27SnFhyo10Data **pSnFhyo40Data, CH27SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CDBNpSub *pZmSub, CSyzSyukei *pSubSyzSyukei )
//int CShinFhyo52EX::InitInfo( CH26SnFhyo50Data **pSnFhyo52Data, CH26SnFhyo10Data **pSnFhyo40Data, CH26SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CDBNpSub *pZmSub, CSyzSyukei *pSubSyzSyukei )
{
	ASSERT( pSnFhyo52Data );
	if( pSnFhyo52Data == NULL ){
		return -1;
	}
	m_pSnFhyo52Data = pSnFhyo52Data;

	ASSERT( pSnFhyo40Data );
	if( pSnFhyo40Data == NULL ){
		return -1;
	}
	m_pSnFhyo40Data = pSnFhyo40Data;

	ASSERT( pSnHonpyoData );
	if( pSnHonpyoData == NULL ){
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

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

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

//	ASSERT( pSubSyzSyukei );
//	if( pSubSyzSyukei == NULL ){
//		return -1;
//	}
	m_pSubSyzSyukei = pSubSyzSyukei;

	return 0;
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数	upck	：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo52EX::ReadData( int upck )
{
	// 課税標準額に対する消費税額
	m_pArith->l_add( F521T, (*m_pSnFhyo40Data)->Sn_1F2A, (*m_pSnFhyo40Data)->Sn_1F2B );
	m_pArith->l_add( F521T, F521T, (*m_pSnFhyo40Data)->Sn_1F2C );
	memmove( F521A, (*m_pSnFhyo40Data)->Sn_1F2A, MONY_BUF_SIZE );
	memmove( F521B, (*m_pSnFhyo40Data)->Sn_1F2B, MONY_BUF_SIZE );
	memmove( F521C, (*m_pSnFhyo40Data)->Sn_1F2C, MONY_BUF_SIZE );

	// 貸倒回収に対する消費税額
	m_pArith->l_add( F522T, (*m_pSnFhyo40Data)->Sn_1F3A, (*m_pSnFhyo40Data)->Sn_1F3B );
	m_pArith->l_add( F522T, F522T, (*m_pSnFhyo40Data)->Sn_1F3C );
	memmove( F522A, (*m_pSnFhyo40Data)->Sn_1F3A, MONY_BUF_SIZE );
	memmove( F522B, (*m_pSnFhyo40Data)->Sn_1F3B, MONY_BUF_SIZE );
	memmove( F522C, (*m_pSnFhyo40Data)->Sn_1F3C, MONY_BUF_SIZE );

	// 売上対価の返還等に係る消費税額
	m_pArith->l_add( F523T, (*m_pSnFhyo40Data)->Sn_1F5A, (*m_pSnFhyo40Data)->Sn_1F5B );
	m_pArith->l_add( F523T, F523T, (*m_pSnFhyo40Data)->Sn_1F5C );
	memmove( F523A, (*m_pSnFhyo40Data)->Sn_1F5A, MONY_BUF_SIZE );
	memmove( F523B, (*m_pSnFhyo40Data)->Sn_1F5B, MONY_BUF_SIZE );
	memmove( F523C, (*m_pSnFhyo40Data)->Sn_1F5C, MONY_BUF_SIZE );

	m_pArith->l_add( F524S, F521T, F522T );
	m_pArith->l_sub( F524S, F524S, F523T );

	// 事業区分別の合計額
	//[Y]
	F520c = (*m_pSnFhyo52Data)->Sn_5FURITsw;
	if(F520c&0x08){//手入力されている場合
		memmove( F526A, (*m_pSnFhyo52Data)->Sn_5FURITA, MONY_BUF_SIZE );
	}
	if(F520c&0x080){//手入力されている場合
		memmove( F526B, (*m_pSnFhyo52Data)->Sn_5FURITB, MONY_BUF_SIZE );
	}
	if(F520c&0x0800){//手入力されている場合
		memmove( F526C, (*m_pSnFhyo52Data)->Sn_5FURITC, MONY_BUF_SIZE );
	}

	memmove( F526T, (*m_pSnFhyo52Data)->Sn_5FURIK, MONY_BUF_SIZE );
//	if( m_pArith->l_cmp(F526T, m_pSnHeadData->Sn_KURIA) ){
//		memmove( F526T, m_pSnHeadData->Sn_KURIA, MONY_BUF_SIZE );
//	}
//	memmove( F526B,  pSyzShin->Snd.Sn_5FURIT, MONY_BUF_SIZE );
	memmove( F5213T, (*m_pSnFhyo52Data)->Sn_5FUZTT, MONY_BUF_SIZE );
	memmove( F5213A, (*m_pSnFhyo52Data)->Sn_5FUZKTA, MONY_BUF_SIZE );
	memmove( F5213B, (*m_pSnFhyo52Data)->Sn_5FUZKTB, MONY_BUF_SIZE );
	memmove( F5213C, (*m_pSnFhyo52Data)->Sn_5FUZKTC, MONY_BUF_SIZE );

	// 第一種事業課税売上高
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI1) ){
		m_typeCnt++;
	}
	memmove( F527T,  (*m_pSnHonpyoData)->Sn_KURI1, MONY_BUF_SIZE );
	memmove( F527A,  (*m_pSnFhyo52Data)->Sn_5FURI1A, MONY_BUF_SIZE );
	memmove( F527B,  (*m_pSnFhyo52Data)->Sn_5FURI1B, MONY_BUF_SIZE );
	memmove( F527C,  (*m_pSnFhyo52Data)->Sn_5FURI1C, MONY_BUF_SIZE );
	F521UPb = (*m_pSnFhyo52Data)->Sn_5FUP1;
	memmove( F5214T, (*m_pSnFhyo52Data)->Sn_5FUZT1, MONY_BUF_SIZE );
	memmove( F5214A, (*m_pSnFhyo52Data)->Sn_5FUZK1A, MONY_BUF_SIZE );
	memmove( F5214B, (*m_pSnFhyo52Data)->Sn_5FUZK1B, MONY_BUF_SIZE );
	memmove( F5214C, (*m_pSnFhyo52Data)->Sn_5FUZK1C, MONY_BUF_SIZE );

	F5214c = (*m_pSnFhyo52Data)->Sn_5FUZK1sw;//[Y]
//--> '14.05.17 INS START
	F521c |= ((*m_pSnFhyo52Data)->Sn_5FUZK1sw&0x111);
//<-- '14.05.17 INS END

	// 第二種事業課税売上高
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI2) ){
		m_typeCnt++;
	}
	memmove( F528T, (*m_pSnHonpyoData)->Sn_KURI2, MONY_BUF_SIZE );
	memmove( F528A, (*m_pSnFhyo52Data)->Sn_5FURI2A, MONY_BUF_SIZE );
	memmove( F528B, (*m_pSnFhyo52Data)->Sn_5FURI2B, MONY_BUF_SIZE );
	memmove( F528C, (*m_pSnFhyo52Data)->Sn_5FURI2C, MONY_BUF_SIZE );
	F522UPb = (*m_pSnFhyo52Data)->Sn_5FUP2;
	memmove( F5215T, (*m_pSnFhyo52Data)->Sn_5FUZT2, MONY_BUF_SIZE );
	memmove( F5215A, (*m_pSnFhyo52Data)->Sn_5FUZK2A, MONY_BUF_SIZE );
	memmove( F5215B, (*m_pSnFhyo52Data)->Sn_5FUZK2B, MONY_BUF_SIZE );
	memmove( F5215C, (*m_pSnFhyo52Data)->Sn_5FUZK2C, MONY_BUF_SIZE );

	F5215c = (*m_pSnFhyo52Data)->Sn_5FUZK2sw;//[Y]
//--> '14.05.17 INS START
	F522c |= ((*m_pSnFhyo52Data)->Sn_5FUZK2sw&0x111);
//<-- '14.05.17 INS END

	// 第三種事業課税売上高
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI3) ){
		m_typeCnt++;
	}
	memmove( F529T, (*m_pSnHonpyoData)->Sn_KURI3, MONY_BUF_SIZE );
	memmove( F529A, (*m_pSnFhyo52Data)->Sn_5FURI3A, MONY_BUF_SIZE );
	memmove( F529B, (*m_pSnFhyo52Data)->Sn_5FURI3B, MONY_BUF_SIZE );
	memmove( F529C, (*m_pSnFhyo52Data)->Sn_5FURI3C, MONY_BUF_SIZE );
	F523UPb = (*m_pSnFhyo52Data)->Sn_5FUP3;
	memmove( F5216T, (*m_pSnFhyo52Data)->Sn_5FUZT3, MONY_BUF_SIZE );
	memmove( F5216A, (*m_pSnFhyo52Data)->Sn_5FUZK3A, MONY_BUF_SIZE );
	memmove( F5216B, (*m_pSnFhyo52Data)->Sn_5FUZK3B, MONY_BUF_SIZE );
	memmove( F5216C, (*m_pSnFhyo52Data)->Sn_5FUZK3C, MONY_BUF_SIZE );

	F5216c = (*m_pSnFhyo52Data)->Sn_5FUZK3sw;//[Y]
//--> '14.05.17 INS START
	F523c |= ((*m_pSnFhyo52Data)->Sn_5FUZK3sw&0x111);
//<-- '14.05.17 INS END

	// 第四種事業課税売上高
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI4) ){
		m_typeCnt++;
	}
	memmove( F5210T, (*m_pSnHonpyoData)->Sn_KURI4, MONY_BUF_SIZE );
	memmove( F5210A, (*m_pSnFhyo52Data)->Sn_5FURI4A, MONY_BUF_SIZE );
	memmove( F5210B, (*m_pSnFhyo52Data)->Sn_5FURI4B, MONY_BUF_SIZE );
	memmove( F5210C, (*m_pSnFhyo52Data)->Sn_5FURI4C, MONY_BUF_SIZE );
	F524UPb = (*m_pSnFhyo52Data)->Sn_5FUP4;
	memmove( F5217T, (*m_pSnFhyo52Data)->Sn_5FUZT4, MONY_BUF_SIZE );
	memmove( F5217A, (*m_pSnFhyo52Data)->Sn_5FUZK4A, MONY_BUF_SIZE );
	memmove( F5217B, (*m_pSnFhyo52Data)->Sn_5FUZK4B, MONY_BUF_SIZE );
	memmove( F5217C, (*m_pSnFhyo52Data)->Sn_5FUZK4C, MONY_BUF_SIZE );

	F5217c = (*m_pSnFhyo52Data)->Sn_5FUZK4sw;//[Y]
//--> '14.05.17 INS START
	F524c |= ((*m_pSnFhyo52Data)->Sn_5FUZK4sw&0x111);
//<-- '14.05.17 INS END

	// 第五種事業課税売上高
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI5) ){
		m_typeCnt++;
	}
	memmove( F5211T, (*m_pSnHonpyoData)->Sn_KURI5, MONY_BUF_SIZE );
	memmove( F5211A, (*m_pSnFhyo52Data)->Sn_5FURI5A, MONY_BUF_SIZE );
	memmove( F5211B, (*m_pSnFhyo52Data)->Sn_5FURI5B, MONY_BUF_SIZE );
	memmove( F5211C, (*m_pSnFhyo52Data)->Sn_5FURI5C, MONY_BUF_SIZE );
	F525UPb = (*m_pSnFhyo52Data)->Sn_5FUP5;
	memmove( F5218T, (*m_pSnFhyo52Data)->Sn_5FUZT5, MONY_BUF_SIZE );
	memmove( F5218A, (*m_pSnFhyo52Data)->Sn_5FUZK5A, MONY_BUF_SIZE );
	memmove( F5218B, (*m_pSnFhyo52Data)->Sn_5FUZK5B, MONY_BUF_SIZE );
	memmove( F5218C, (*m_pSnFhyo52Data)->Sn_5FUZK5C, MONY_BUF_SIZE );

	F5218c = (*m_pSnFhyo52Data)->Sn_5FUZK5sw;//[Y]
//--> '14.05.17 INS START
	F525c |= ((*m_pSnFhyo52Data)->Sn_5FUZK5sw&0x111);
//<-- '14.05.17 INS END

	// 第六種事業課税売上高
	if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_KURI6) ){
		m_typeCnt++;
	}
	memmove( F5212T, (*m_pSnHonpyoData)->Sn_KURI6, MONY_BUF_SIZE );
	memmove( F5212A, (*m_pSnFhyo52Data)->Sn_5FURI6A, MONY_BUF_SIZE );
	memmove( F5212B, (*m_pSnFhyo52Data)->Sn_5FURI6B, MONY_BUF_SIZE );
	memmove( F5212C, (*m_pSnFhyo52Data)->Sn_5FURI6C, MONY_BUF_SIZE );
	F526UPb = (*m_pSnFhyo52Data)->Sn_5FUP6;
	memmove( F5219T, (*m_pSnFhyo52Data)->Sn_5FUZT6, MONY_BUF_SIZE );
	memmove( F5219A, (*m_pSnFhyo52Data)->Sn_5FUZK6A, MONY_BUF_SIZE );
	memmove( F5219B, (*m_pSnFhyo52Data)->Sn_5FUZK6B, MONY_BUF_SIZE );
	memmove( F5219C, (*m_pSnFhyo52Data)->Sn_5FUZK6C, MONY_BUF_SIZE );

	F5219c = (*m_pSnFhyo52Data)->Sn_5FUZK6sw;//[Y]
//--> '14.05.17 INS START
	F526c |= ((*m_pSnFhyo52Data)->Sn_5FUZK6sw&0x111);
//<-- '14.05.17 INS END

	// 事業区分別の合計額
	memmove( F526S, F526T, MONY_BUF_SIZE );	// 合計
	memmove( F526SS,F526B, MONY_BUF_SIZE );	// 4%

	// 原則計算を適用する場合の控除対象仕入税額
	memmove( F5220T, (*m_pSnFhyo52Data)->Sn_5FSZTG, MONY_BUF_SIZE );
	memmove( F5220A, (*m_pSnFhyo52Data)->Sn_5FSZKGA, MONY_BUF_SIZE );
	memmove( F5220B, (*m_pSnFhyo52Data)->Sn_5FSZKGB, MONY_BUF_SIZE );
	memmove( F5220C, (*m_pSnFhyo52Data)->Sn_5FSZKGC, MONY_BUF_SIZE );

	// 特例計算を適用する場合の控除対象仕入税額
	F5221c = (*m_pSnFhyo52Data)->Sn_5FSZT21sw;
	if( (*m_pSnHeadData)->IsDataConnected() ){
		memmove( F5221T, (*m_pSnFhyo52Data)->Sn_5FSZTT, MONY_BUF_SIZE );
		memmove( F5221A, (*m_pSnFhyo52Data)->Sn_5FSZKTA, MONY_BUF_SIZE );
		memmove( F5221B, (*m_pSnFhyo52Data)->Sn_5FSZKTB, MONY_BUF_SIZE );
		memmove( F5221C, (*m_pSnFhyo52Data)->Sn_5FSZKTC, MONY_BUF_SIZE );
	}
	else if( (*m_pSnFhyo52Data)->Sn_5FSZsw ){
		memmove( F5221T, (*m_pSnFhyo52Data)->Sn_5FSZTT, MONY_BUF_SIZE );
		memmove( F5221A, (*m_pSnFhyo52Data)->Sn_5FSZKTA, MONY_BUF_SIZE );
		memmove( F5221B, (*m_pSnFhyo52Data)->Sn_5FSZKTB, MONY_BUF_SIZE );
		memmove( F5221C, (*m_pSnFhyo52Data)->Sn_5FSZKTC, MONY_BUF_SIZE );
	}
	else{
/*- '14.03.03 -*/
//		memset( F5219T, '\0', MONY_BUF_SIZE );
//		memset( F5219A, '\0', MONY_BUF_SIZE );
//		memset( F5219B, '\0', MONY_BUF_SIZE );
//		memset( F5219C, '\0', MONY_BUF_SIZE );
/*-------------*/
		if( F5221c&0x08 ){
			memmove( F5221A, (*m_pSnFhyo52Data)->Sn_5FSZKTA, MONY_BUF_SIZE );
		}
		else{
			memset( F5221A, '\0', MONY_BUF_SIZE );
		}
		if( F5221c&0x80 ){
			memmove( F5221B, (*m_pSnFhyo52Data)->Sn_5FSZKTB, MONY_BUF_SIZE );
		}
		else{
			memset( F5221B, '\0', MONY_BUF_SIZE );
		}
		if( F5221c&0x800 ){
			memmove( F5221C, (*m_pSnFhyo52Data)->Sn_5FSZKTC, MONY_BUF_SIZE );
		}
		else{
			memset( F5221C, '\0', MONY_BUF_SIZE );
		}
		if( F5221c&0x888 ){
			memmove( F5221T, (*m_pSnFhyo52Data)->Sn_5FSZTT, MONY_BUF_SIZE );
		}
		else{
			memset( F5221T, '\0', MONY_BUF_SIZE );
		}
/*-------------*/
	}

	// 22
	memmove( F5222A, (*m_pSnFhyo52Data)->Sn_5FSZT22A, MONY_BUF_SIZE );
	memmove( F5222B, (*m_pSnFhyo52Data)->Sn_5FSZT22B, MONY_BUF_SIZE );
	memmove( F5222C, (*m_pSnFhyo52Data)->Sn_5FSZT22C, MONY_BUF_SIZE );
	memmove( F5222T, (*m_pSnFhyo52Data)->Sn_5FSZT22T, MONY_BUF_SIZE );
	F5222c = (*m_pSnFhyo52Data)->Sn_5FSZT22sw;
	// 23
	memmove( F5223A, (*m_pSnFhyo52Data)->Sn_5FSZT23A, MONY_BUF_SIZE );
	memmove( F5223B, (*m_pSnFhyo52Data)->Sn_5FSZT23B, MONY_BUF_SIZE );
	memmove( F5223C, (*m_pSnFhyo52Data)->Sn_5FSZT23C, MONY_BUF_SIZE );
	memmove( F5223T, (*m_pSnFhyo52Data)->Sn_5FSZT23T, MONY_BUF_SIZE );
	F5223c = (*m_pSnFhyo52Data)->Sn_5FSZT23sw;
	// 24
	memmove( F5224A, (*m_pSnFhyo52Data)->Sn_5FSZT24A, MONY_BUF_SIZE );
	memmove( F5224B, (*m_pSnFhyo52Data)->Sn_5FSZT24B, MONY_BUF_SIZE );
	memmove( F5224C, (*m_pSnFhyo52Data)->Sn_5FSZT24C, MONY_BUF_SIZE );
	memmove( F5224T, (*m_pSnFhyo52Data)->Sn_5FSZT24T, MONY_BUF_SIZE );
	F5224c = (*m_pSnFhyo52Data)->Sn_5FSZT24sw;
	// 25
	memmove( F5225A, (*m_pSnFhyo52Data)->Sn_5FSZT25A, MONY_BUF_SIZE );
	memmove( F5225B, (*m_pSnFhyo52Data)->Sn_5FSZT25B, MONY_BUF_SIZE );
	memmove( F5225C, (*m_pSnFhyo52Data)->Sn_5FSZT25C, MONY_BUF_SIZE );
	memmove( F5225T, (*m_pSnFhyo52Data)->Sn_5FSZT25T, MONY_BUF_SIZE );
	F5225c = (*m_pSnFhyo52Data)->Sn_5FSZT25sw;
	// 26
	memmove( F5226A, (*m_pSnFhyo52Data)->Sn_5FSZT26A, MONY_BUF_SIZE );
	memmove( F5226B, (*m_pSnFhyo52Data)->Sn_5FSZT26B, MONY_BUF_SIZE );
	memmove( F5226C, (*m_pSnFhyo52Data)->Sn_5FSZT26C, MONY_BUF_SIZE );
	memmove( F5226T, (*m_pSnFhyo52Data)->Sn_5FSZT26T, MONY_BUF_SIZE );
	F5226c = (*m_pSnFhyo52Data)->Sn_5FSZT26sw;
	// 27
	memmove( F5227A, (*m_pSnFhyo52Data)->Sn_5FSZT27A, MONY_BUF_SIZE );
	memmove( F5227B, (*m_pSnFhyo52Data)->Sn_5FSZT27B, MONY_BUF_SIZE );
	memmove( F5227C, (*m_pSnFhyo52Data)->Sn_5FSZT27C, MONY_BUF_SIZE );
	memmove( F5227T, (*m_pSnFhyo52Data)->Sn_5FSZT27T, MONY_BUF_SIZE );
	F5227c = (*m_pSnFhyo52Data)->Sn_5FSZT27sw;
	// 28
	memmove( F5228A, (*m_pSnFhyo52Data)->Sn_5FSZT28A, MONY_BUF_SIZE );
	memmove( F5228B, (*m_pSnFhyo52Data)->Sn_5FSZT28B, MONY_BUF_SIZE );
	memmove( F5228C, (*m_pSnFhyo52Data)->Sn_5FSZT28C, MONY_BUF_SIZE );
	memmove( F5228T, (*m_pSnFhyo52Data)->Sn_5FSZT28T, MONY_BUF_SIZE );
	F5228c = (*m_pSnFhyo52Data)->Sn_5FSZT28sw;
	// 29
	memmove( F5229A, (*m_pSnFhyo52Data)->Sn_5FSZT29A, MONY_BUF_SIZE );
	memmove( F5229B, (*m_pSnFhyo52Data)->Sn_5FSZT29B, MONY_BUF_SIZE );
	memmove( F5229C, (*m_pSnFhyo52Data)->Sn_5FSZT29C, MONY_BUF_SIZE );
	memmove( F5229T, (*m_pSnFhyo52Data)->Sn_5FSZT29T, MONY_BUF_SIZE );
	F5229c = (*m_pSnFhyo52Data)->Sn_5FSZT29sw;
	// 30
	memmove( F5230A, (*m_pSnFhyo52Data)->Sn_5FSZT30A, MONY_BUF_SIZE );
	memmove( F5230B, (*m_pSnFhyo52Data)->Sn_5FSZT30B, MONY_BUF_SIZE );
	memmove( F5230C, (*m_pSnFhyo52Data)->Sn_5FSZT30C, MONY_BUF_SIZE );
	memmove( F5230T, (*m_pSnFhyo52Data)->Sn_5FSZT30T, MONY_BUF_SIZE );
	F5230c = (*m_pSnFhyo52Data)->Sn_5FSZT30sw;
	// 31
	memmove( F5231A, (*m_pSnFhyo52Data)->Sn_5FSZT31A, MONY_BUF_SIZE );
	memmove( F5231B, (*m_pSnFhyo52Data)->Sn_5FSZT31B, MONY_BUF_SIZE );
	memmove( F5231C, (*m_pSnFhyo52Data)->Sn_5FSZT31C, MONY_BUF_SIZE );
	memmove( F5231T, (*m_pSnFhyo52Data)->Sn_5FSZT31T, MONY_BUF_SIZE );
	F5231c = (*m_pSnFhyo52Data)->Sn_5FSZT31sw;
	// 32
	memmove( F5232A, (*m_pSnFhyo52Data)->Sn_5FSZT32A, MONY_BUF_SIZE );
	memmove( F5232B, (*m_pSnFhyo52Data)->Sn_5FSZT32B, MONY_BUF_SIZE );
	memmove( F5232C, (*m_pSnFhyo52Data)->Sn_5FSZT32C, MONY_BUF_SIZE );
	memmove( F5232T, (*m_pSnFhyo52Data)->Sn_5FSZT32T, MONY_BUF_SIZE );
	F5232c = (*m_pSnFhyo52Data)->Sn_5FSZT32sw;
	// 33
	memmove( F5233A, (*m_pSnFhyo52Data)->Sn_5FSZT33A, MONY_BUF_SIZE );
	memmove( F5233B, (*m_pSnFhyo52Data)->Sn_5FSZT33B, MONY_BUF_SIZE );
	memmove( F5233C, (*m_pSnFhyo52Data)->Sn_5FSZT33C, MONY_BUF_SIZE );
	memmove( F5233T, (*m_pSnFhyo52Data)->Sn_5FSZT33T, MONY_BUF_SIZE );
	F5233c = (*m_pSnFhyo52Data)->Sn_5FSZT33sw;
	// 34
	memmove( F5234A, (*m_pSnFhyo52Data)->Sn_5FSZT34A, MONY_BUF_SIZE );
	memmove( F5234B, (*m_pSnFhyo52Data)->Sn_5FSZT34B, MONY_BUF_SIZE );
	memmove( F5234C, (*m_pSnFhyo52Data)->Sn_5FSZT34C, MONY_BUF_SIZE );
	memmove( F5234T, (*m_pSnFhyo52Data)->Sn_5FSZT34T, MONY_BUF_SIZE );
	F5234c = (*m_pSnFhyo52Data)->Sn_5FSZT34sw;
	// 35
	memmove( F5235A, (*m_pSnFhyo52Data)->Sn_5FSZT35A, MONY_BUF_SIZE );
	memmove( F5235B, (*m_pSnFhyo52Data)->Sn_5FSZT35B, MONY_BUF_SIZE );
	memmove( F5235C, (*m_pSnFhyo52Data)->Sn_5FSZT35C, MONY_BUF_SIZE );
	memmove( F5235T, (*m_pSnFhyo52Data)->Sn_5FSZT35T, MONY_BUF_SIZE );
	F5235c = (*m_pSnFhyo52Data)->Sn_5FSZT35sw;
	// 36
	memmove( F5236A, (*m_pSnFhyo52Data)->Sn_5FSZT36A, MONY_BUF_SIZE );
	memmove( F5236B, (*m_pSnFhyo52Data)->Sn_5FSZT36B, MONY_BUF_SIZE );
	memmove( F5236C, (*m_pSnFhyo52Data)->Sn_5FSZT36C, MONY_BUF_SIZE );
	memmove( F5236T, (*m_pSnFhyo52Data)->Sn_5FSZT36T, MONY_BUF_SIZE );
	F5236c = (*m_pSnFhyo52Data)->Sn_5FSZT36sw;


	// １種類の事業の専業者の場合
	// 特例計算適用ＳＷ
	m_Szsw = (*m_pSnFhyo52Data)->Sn_5FSZsw;
	m_SBsw = (*m_pSnFhyo52Data)->Sn_5FSBsw;
	m_CQsw = (*m_pSnFhyo52Data)->Sn_5FCQsw;
	F5220c = (*m_pSnFhyo52Data)->Sn_5FGKsw;

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo52EX::WriteData()
{
	char	WORK0[MONY_BUF_SIZE]={0};
	char	WORK1[MONY_BUF_SIZE]={0};

	// 入力直後の特例計算サイン
//	Tokurei52();

/*- '14.03.12_CUT -*/
//	// 0 = 経過措置無し
//	// 課税標準額に対する消費税額
//	memmove( (*m_pSnFhyo40Data)->Sn_1F4A, F521A, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo40Data)->Sn_1F4B, F521B, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F521C, MONY_BUF_SIZE );
//
//	// 貸倒回収に対する消費税額
//	memmove( (*m_pSnFhyo40Data)->Sn_1F3A, F522A, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo40Data)->Sn_1F3B, F522B, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo40Data)->Sn_1F3C, F522C, MONY_BUF_SIZE );
//
//	// 売上対価の返還等に係る消費税額
////	l_sub( pSyzShin->Snd.Sn_1F9A, F523T, F523B );
//	memmove( (*m_pSnFhyo40Data)->Sn_1F5A, F523A, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo40Data)->Sn_1F5B, F523B, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo40Data)->Sn_1F5C, F523C, MONY_BUF_SIZE );*/
////	memmove( (*m_pSnFhyo40Data)->Sn_1F9C, F523T, MONY_BUF_SIZE );
//	m_pArith->l_add( WORK0, F521T, F522T );
//	m_pArith->l_sub( WORK0, WORK0, F523T );
/*- '14.03.12_CUT -*/

	// 事業区分別の合計額
	memmove( (*m_pSnHeadData)->Sn_KURIA,  F526T,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURIK, F526T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURITA, F526A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURITB, F526B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURITC, F526C, MONY_BUF_SIZE );

	(*m_pSnFhyo52Data)->Sn_5FURITsw = F520c;//[Y]

	// 第一種事業課税売上高
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI1A, F527A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI1B, F527B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI1C, F527C, MONY_BUF_SIZE );
	memmove( (*m_pSnHonpyoData)->Sn_KURI1,  F527T,  MONY_BUF_SIZE  );
	(*m_pSnFhyo52Data)->Sn_5FUP1 = F521UPb;

	memmove( (*m_pSnFhyo52Data)->Sn_5FUZT1, F5214T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK1A, F5214A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK1B, F5214B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK1C, F5214C, MONY_BUF_SIZE );

	(*m_pSnFhyo52Data)->Sn_5FUZK1sw = F5214c;//[Y]


	// 第二種事業課税売上高
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI2A, F528A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI2B, F528B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI2C, F528C, MONY_BUF_SIZE );
	memmove( (*m_pSnHonpyoData)->Sn_KURI2,  F528T,  MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FUP2 = F522UPb;

	memmove( (*m_pSnFhyo52Data)->Sn_5FUZT2, F5215T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK2A, F5215A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK2B, F5215B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK2C, F5215C, MONY_BUF_SIZE );

	(*m_pSnFhyo52Data)->Sn_5FUZK2sw = F5215c;//[Y]

	// 第三種事業課税売上高
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI3A, F529A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI3B, F529B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI3C, F529C, MONY_BUF_SIZE );
	memmove( (*m_pSnHonpyoData)->Sn_KURI3,  F529T,  MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FUP3 = F523UPb;

	memmove( (*m_pSnFhyo52Data)->Sn_5FUZT3, F5216T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK3A, F5216A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK3B, F5216B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK3C, F5216C, MONY_BUF_SIZE );

	(*m_pSnFhyo52Data)->Sn_5FUZK3sw = F5216c;//[Y]

	// 第四種事業課税売上高
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI4A, F5210A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI4B, F5210B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI4C, F5210C, MONY_BUF_SIZE );
	memmove( (*m_pSnHonpyoData)->Sn_KURI4,  F5210T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FUP4 = F524UPb;

	memmove( (*m_pSnFhyo52Data)->Sn_5FUZT4, F5217T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK4A, F5217A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK4B, F5217B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK4C, F5217C, MONY_BUF_SIZE );

	(*m_pSnFhyo52Data)->Sn_5FUZK4sw = F5217c;//[Y]

	// 第五種事業課税売上高
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI5A, F5211A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI5B, F5211B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI5C, F5211C, MONY_BUF_SIZE );
	memmove( (*m_pSnHonpyoData)->Sn_KURI5,  F5211T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FUP5 = F525UPb;

	memmove( (*m_pSnFhyo52Data)->Sn_5FUZT5, F5218T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK5A, F5218A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK5B, F5218B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK5C, F5218C, MONY_BUF_SIZE );

	(*m_pSnFhyo52Data)->Sn_5FUZK5sw = F5218c;//[Y]

	// 第六種事業課税売上高
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI6A, F5212A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI6B, F5212B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FURI6C, F5212C, MONY_BUF_SIZE );
	memmove( (*m_pSnHonpyoData)->Sn_KURI6,  F5212T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FUP6 = F526UPb;

	memmove( (*m_pSnFhyo52Data)->Sn_5FUZT6, F5219T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK6A, F5219A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK6B, F5219B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZK6C, F5219C, MONY_BUF_SIZE );

	(*m_pSnFhyo52Data)->Sn_5FUZK6sw = F5219c;//[Y]


	memmove( (*m_pSnFhyo52Data)->Sn_5FUZKTA, F5213A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZKTB, F5213B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZKTC, F5213C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FUZTT, F5213T, MONY_BUF_SIZE );

//
	
	// 原則計算を適用する場合の控除対象仕入税額
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZTG, F5220T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZKGA, F5220A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZKGB, F5220B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZKGC, F5220C, MONY_BUF_SIZE );

	// 特例計算を適用する場合の控除対象仕入税額
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZTT, F5221T, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZKTA, F5221A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZKTB, F5221B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZKTC, F5221C, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT21sw = F5221c;

	// １種類の事業の専業者の場合
	memset( (*m_pSnFhyo40Data)->Sn_1F4A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo40Data)->Sn_1F4B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo40Data)->Sn_1F4C, '\0', MONY_BUF_SIZE );
//--> '14.10.17 INS START
	memset( (*m_pSnFhyo40Data)->Sn_1F4T, '\0', MONY_BUF_SIZE );
//<-- '14.10.17 INS END

	if( m_CQsw & 0x0f ){
		memmove( (*m_pSnFhyo40Data)->Sn_1F4A, F525A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4B, F525B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F525C, MONY_BUF_SIZE );
//--> '14.10.17 INS START
		memmove( (*m_pSnFhyo40Data)->Sn_1F4T, F525T, MONY_BUF_SIZE );
//<-- '14.10.17 INS END
// これはカット
//		memmove( (*m_pSnFhyo40Data)->Sn_1F8C, F525T, MONY_BUF_SIZE );
//		m_pArith->l_sub( (*m_pSnFhyo40Data)->Sn_1F8A, F525T, F525B );
	}
	else{
		memmove( (*m_pSnFhyo40Data)->Sn_1F4A, F5237A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4B, F5237B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F5237C, MONY_BUF_SIZE );
//--> '14.10.17 INS START
		memmove( (*m_pSnFhyo40Data)->Sn_1F4T, F5237T, MONY_BUF_SIZE );
//<-- '14.10.17 INS END
	}
// これはカット
/*	else{
		if( m_pArith->l_cmp(F5218C, F5219C) > 0 ){
			memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F5218C, MONY_BUF_SIZE );
		}
		else{
			memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F5219C, MONY_BUF_SIZE );
		}

//		if( !pSyzShin->Snh.Sn_TOKUR ){
		if( 0 ){
			memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F5218T, MONY_BUF_SIZE );
		}
		else {
			memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F5219T, MONY_BUF_SIZE );
		}
	}*/

	// 22
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT22A, F5222A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT22B, F5222B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT22C, F5222C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT22T, F5222T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT22sw = F5222c;
	// 23
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT23A, F5223A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT23B, F5223B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT23C, F5223C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT23T, F5223T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT23sw = F5223c;
	// 24
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT24A, F5224A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT24B, F5224B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT24C, F5224C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT24T, F5224T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT24sw = F5224c;
	// 25
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT25A, F5225A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT25B, F5225B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT25C, F5225C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT25T, F5225T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT25sw = F5225c;
	// 26
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT26A, F5226A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT26B, F5226B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT26C, F5226C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT26T, F5226T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT26sw = F5226c;
	// 27
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT27A, F5227A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT27B, F5227B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT27C, F5227C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT27T, F5227T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT27sw = F5227c;
	// 28
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT28A, F5228A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT28B, F5228B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT28C, F5228C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT28T, F5228T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT28sw = F5228c;
	// 29
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT29A, F5229A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT29B, F5229B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT29C, F5229C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT29T, F5229T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT29sw = F5229c;
	// 30
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT30A, F5230A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT30B, F5230B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT30C, F5230C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT30T, F5230T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT30sw = F5230c;
	// 31
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT31A, F5231A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT31B, F5231B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT31C, F5231C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT31T, F5231T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT31sw = F5231c;
	// 32
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT32A, F5232A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT32B, F5232B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT32C, F5232C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT32T, F5232T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT32sw = F5232c;
	// 33
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT33A, F5233A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT33B, F5233B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT33C, F5233C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT33T, F5233T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT33sw = F5233c;
	// 34
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT34A, F5234A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT34B, F5234B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT34C, F5234C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT34T, F5234T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT34sw = F5234c;
	// 35
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT35A, F5235A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT35B, F5235B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT35C, F5235C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT35T, F5235T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT35sw = F5235c;
	// 36
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT36A, F5236A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT36B, F5236B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT36C, F5236C, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo52Data)->Sn_5FSZT36T, F5236T, MONY_BUF_SIZE );
	(*m_pSnFhyo52Data)->Sn_5FSZT36sw = F5236c;


	// 特例計算適用ＳＷ
	(*m_pSnFhyo52Data)->Sn_5FSZsw = m_Szsw;

	(*m_pSnFhyo52Data)->Sn_5FSBsw = m_SBsw;
	(*m_pSnFhyo52Data)->Sn_5FCQsw = m_CQsw;
	(*m_pSnFhyo52Data)->Sn_5FGKsw = F5220c;

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo52EX::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo52[cnt].Index == 0 ){
			break;
		}
		
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo52[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo52[cnt].UnConnect;
			}
			else{
				Flg = Fhyo52[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL2, Fhyo52[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo52[cnt].Connect );
		cnt++;
	}

	m_Fhyo52Diag02.EnableDelete(0);//[Y]DELETEキー１回で削除モード設定

	return 0;
}

//-----------------------------------------------------------------------------
// 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo52EX::DispFh52()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	char	buf[512] = {0};

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]連動の場合手入力サイン初期化
		//手入力サイン初期化
		F520c = 0;
		F5213c = F5214c = F5215c = F5216c = F5217c = F5218c = F5219c = 0;
	}

	// 期間・氏名の表示
	char	yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff),(mm&0xff),(dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL1, 9, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff),(mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL1, 10, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( buf, '\0', sizeof(buf) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, 40 );
	}
	else{
		memmove( buf, (*m_pSnHeadData)->Sn_CONAM, 40 );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL1, 8, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税標準額に対する消費税額
	f52calq_1();
	// 貸倒回収額に対する消費税額
	f52calq_2();
	// 売上対価の返還等に係る消費税額
	f52calq_3();
	// 控除対象仕入税額計算の基礎となる消費税額
	f52calq_4();
	// 単一事業の場合の控除対象仕入税額
	f52calq_gsubtotal();
	f52calq_5();
	if( m_typeCnt > 1 ){		// 複数事業種選択時のみ表示
		// 課税売上高に係る消費税額の計算欄
		f52calq_ku();
		// 事業別課税売上割合の表示
		f52calq_uws();
		// 売上区分チェック
		f52_urikbn();
		// 原則計算を適用する場合
		f52calq_gn();
		// 特例計算を適用する場合
/*- '14.03.12 -*/
//		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
//			f52calq_tk_all();
//		}
//		else{
//			f52calq_tk();
//		}
/*-------------*/
		f52calq_tk_all();
/*-------------*/
	}
	else{
		// 課税売上高に係る消費税額の計算欄
		f52calq_ku();
		//	事業別課税売上割合の表示
		f52calq_uws();
	}

	f52calq_all( 1 );

	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE ){
		diag_clear( IDC_ICSDIAGCTRL1, 4, TRUE, CIcsdiagctrl );
		diag_getattr( IDC_ICSDIAGCTRL1, 4, &DA, CIcsdiagctrl );
		DA.attr_bcolor = SIROIROCOLOR;
		diag_setattr( IDC_ICSDIAGCTRL1, 4, &DA, FALSE, CIcsdiagctrl );
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) || ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

//	if( F5218c&0x0f ){
//		ChangeColor( IDC_ICSDIAGCTRL2, ID5218T, 4 );			//2 );
//	}
//	if( F5218c&0xf0 ){
//		ChangeColor( IDC_ICSDIAGCTRL2, ID5218B, 4 );			//2 );
//	}
}

//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinFhyo52EX::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	if( !m_DpSw ){	// 再集計のみ（表示無し）
		diag_getattr( id, index, &DA, CIcsdiagctrl );
		switch( sign ){
			case 1 :		// 入力不可	（白色）
				DA.attr_bcolor = BC_WHITE;
				break;
			case 2 :		// ロック	（灰色）
				DA.attr_bcolor = BC_GRAY;
				break;
			case 3 :		// 特殊色①	（青色）
				DA.attr_bcolor = BC_BLUE;
				break;		
			case 4 :		// 特殊色②	（緑色）
				DA.attr_bcolor = BC_GREEN;
				break;
			case 5 : 		// 特殊色③	（薄緑色）	 
				DA.attr_bcolor = BC_GREEN_L;
				break;
			default: 		// 実額入力	（クリーム色） 
				DA.attr_bcolor = BC_CREAM;
				break;
		}
		diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
	}
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	
//		Index	：	
//		Mony	：	
//-----------------------------------------------------------------------------
void CShinFhyo52EX::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){	// 再集計のみ（表示無し）
	
		switch( Index ) {
			case ID521T  :	//  1. 課税標準額に対する消費税額（期間計）
			case ID521B  :	//  1. 課税標準額に対する消費税額（4%分）
			case ID522T  :	//  2. 貸倒回収額に対する消費税額（期間計）
			case ID522B  :	//  2. 貸倒回収額に対する消費税額（4%分）
			case ID523T  : 	//  3. 売上対価の返還等に係る消費税額（期間計）
			case ID523B  :	//  3. 売上対価の返還等に係る消費税額（4%分）
			case ID524T  :	//  4. 控除対象仕入税額計算の為の消費税額（期間計）
			case ID524B  :	//  4. 控除対象仕入税額計算の為の消費税額（4%分）
			case ID525T  :	//	5. 単一事業の場合の控除対象仕入税額（期間計）
			case ID525B  :	//	5. 単一事業の場合の控除対象仕入税額（4%分）
				memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
				diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				break;

			//[Y]
	//		case ID5230T :	// 30.期間計
	//		case ID5230B :	// 30.4%分
	//			memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
	//			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//			break;
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
// 引数	Id		：	
//		Index	：	
//-----------------------------------------------------------------------------
void CShinFhyo52EX::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){	// 再集計のみ（表示無し）
	
		switch( Index ){
			case 163 :			// 課税期間　自
			case 164 :			// 課税期間　至
			case 165 :			// 氏名又は､名称
			case ID527TE :	//  7. 売上割合
			case ID528TE :	//  8. 売上割合
			case ID529TE :	//  9. 売上割合
			case ID5210TE:	// 10. 売上割合
			case ID5211TE:	// 11. 売上割合
			case ID5212TE:	// 12. 売上割合
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
// 売上割合表示
//-----------------------------------------------------------------------------
// 引数	Id		：	
//		Index	：	
//		pers	：	
//		sign	：	
//-----------------------------------------------------------------------------
void CShinFhyo52EX::dsp_uws( unsigned short Id, short Index, unsigned short pers, short sign )
{
	if( pers && (!m_DpSw) ){	// 再集計のみ（表示無し）
		char				buf[128] = {0};
		DIAGRAM_DATA		DiagData;
		DiagData.data_imgdata = NULL;

		if( sign ){
			sprintf_s( buf, sizeof(buf), _T("%3d.%01d"), (pers/10),  (pers%10) );
		}
		else	{
			sprintf_s( buf, sizeof(buf), _T("%3d.%02d"), (pers/100), (pers%100) );
		}
		DiagData.data_disp = (char *)buf;
		diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
	else{
		dsp_cls( Id, Index );
	}
}

//-----------------------------------------------------------------------------
// 連動情報の取得
//-----------------------------------------------------------------------------
// 引数	pLinkedInfo	：	連動させる情報
//-----------------------------------------------------------------------------
void CShinFhyo52EX::GetLinkedData( SH_52EX_LINKED_INFO *pLinkedInfo )
{
	ASSERT( pLinkedInfo );

	*(pLinkedInfo->ptypeCnt) = &m_typeCnt;
	*(pLinkedInfo->pCQsw) = &m_CQsw;
	*(pLinkedInfo->pSzsw) = &m_Szsw;
	*(pLinkedInfo->pSBsw) = &m_SBsw;
	*(pLinkedInfo->ptkSw) = &m_tkSw;
	*(pLinkedInfo->pdspSkjSw) = &m_dspSkjSw;	// '14.06.11
	
	*(pLinkedInfo->pF5220T) = F5220T;
	*(pLinkedInfo->pF5220A) = F5220A;
	*(pLinkedInfo->pF5220B) = F5220B;
	*(pLinkedInfo->pF5220C) = F5220C;
	*(pLinkedInfo->pF5220c) = &F5220c;

	*(pLinkedInfo->pF5221T) = F5221T;
	*(pLinkedInfo->pF5221A) = F5221A;
	*(pLinkedInfo->pF5221B) = F5221B;
	*(pLinkedInfo->pF5221C) = F5221C;
	*(pLinkedInfo->pF5221c) = &F5221c;

	*(pLinkedInfo->pF5222T) = F5222T;
	*(pLinkedInfo->pF5222A) = F5222A;
	*(pLinkedInfo->pF5222B) = F5222B;
	*(pLinkedInfo->pF5222C) = F5222C;
	*(pLinkedInfo->pF5222c) = &F5222c;

	*(pLinkedInfo->pF5223T) = F5223T;
	*(pLinkedInfo->pF5223A) = F5223A;
	*(pLinkedInfo->pF5223B) = F5223B;
	*(pLinkedInfo->pF5223C) = F5223C;
	*(pLinkedInfo->pF5223c) = &F5223c;

	*(pLinkedInfo->pF5224T) = F5224T;
	*(pLinkedInfo->pF5224A) = F5224A;
	*(pLinkedInfo->pF5224B) = F5224B;
	*(pLinkedInfo->pF5224C) = F5224C;
	*(pLinkedInfo->pF5224c) = &F5224c;

	*(pLinkedInfo->pF5225T) = F5225T;
	*(pLinkedInfo->pF5225A) = F5225A;
	*(pLinkedInfo->pF5225B) = F5225B;
	*(pLinkedInfo->pF5225C) = F5225C;
	*(pLinkedInfo->pF5225c) = &F5225c;

	*(pLinkedInfo->pF5226T) = F5226T;
	*(pLinkedInfo->pF5226A) = F5226A;
	*(pLinkedInfo->pF5226B) = F5226B;
	*(pLinkedInfo->pF5226C) = F5226C;
	*(pLinkedInfo->pF5226c) = &F5226c;

	*(pLinkedInfo->pF5227T) = F5227T;
	*(pLinkedInfo->pF5227A) = F5227A;
	*(pLinkedInfo->pF5227B) = F5227B;
	*(pLinkedInfo->pF5227C) = F5227C;
	*(pLinkedInfo->pF5227c) = &F5227c;

	*(pLinkedInfo->pF5228T) = F5228T;
	*(pLinkedInfo->pF5228A) = F5228A;
	*(pLinkedInfo->pF5228B) = F5228B;
	*(pLinkedInfo->pF5228C) = F5228C;
	*(pLinkedInfo->pF5228c) = &F5228c;

	*(pLinkedInfo->pF5229T) = F5229T;
	*(pLinkedInfo->pF5229A) = F5229A;
	*(pLinkedInfo->pF5229B) = F5229B;
	*(pLinkedInfo->pF5229C) = F5229C;
	*(pLinkedInfo->pF5229c) = &F5229c;

	*(pLinkedInfo->pF5230T) = F5230T;
	*(pLinkedInfo->pF5230A) = F5230A;
	*(pLinkedInfo->pF5230B) = F5230B;
	*(pLinkedInfo->pF5230C) = F5230C;
	*(pLinkedInfo->pF5230c) = &F5230c;

	*(pLinkedInfo->pF5231T) = F5231T;
	*(pLinkedInfo->pF5231A) = F5231A;
	*(pLinkedInfo->pF5231B) = F5231B;
	*(pLinkedInfo->pF5231C) = F5231C;
	*(pLinkedInfo->pF5231c) = &F5231c;

	*(pLinkedInfo->pF5232T) = F5232T;
	*(pLinkedInfo->pF5232A) = F5232A;
	*(pLinkedInfo->pF5232B) = F5232B;
	*(pLinkedInfo->pF5232C) = F5232C;
	*(pLinkedInfo->pF5232c) = &F5232c;

	*(pLinkedInfo->pF5233T) = F5233T;
	*(pLinkedInfo->pF5233A) = F5233A;
	*(pLinkedInfo->pF5233B) = F5233B;
	*(pLinkedInfo->pF5233C) = F5233C;
	*(pLinkedInfo->pF5233c) = &F5233c;

	*(pLinkedInfo->pF5234T) = F5234T;
	*(pLinkedInfo->pF5234A) = F5234A;
	*(pLinkedInfo->pF5234B) = F5234B;
	*(pLinkedInfo->pF5234C) = F5234C;
	*(pLinkedInfo->pF5234c) = &F5234c;

	*(pLinkedInfo->pF5235T) = F5235T;
	*(pLinkedInfo->pF5235A) = F5235A;
	*(pLinkedInfo->pF5235B) = F5235B;
	*(pLinkedInfo->pF5235C) = F5235C;
	*(pLinkedInfo->pF5235c) = &F5235c;

	*(pLinkedInfo->pF5236T) = F5236T;
	*(pLinkedInfo->pF5236A) = F5236A;
	*(pLinkedInfo->pF5236B) = F5236B;
	*(pLinkedInfo->pF5236C) = F5236C;
	*(pLinkedInfo->pF5236c) = &F5236c;

	*(pLinkedInfo->pF5237T) = F5237T;
	*(pLinkedInfo->pF5237A) = F5237A;
	*(pLinkedInfo->pF5237B) = F5237B;
	*(pLinkedInfo->pF5237C) = F5237C;
}

BEGIN_EVENTSINK_MAP(CShinFhyo52EX, CSyzBaseDlg)
	ON_EVENT(CShinFhyo52EX, IDC_ICSDIAGCTRL2, 1, CShinFhyo52EX::EditONIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo52EX, IDC_ICSDIAGCTRL2, 2, CShinFhyo52EX::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo52EX, IDC_ICSDIAGCTRL2, 3, CShinFhyo52EX::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo52EX::EditONIcsdiagctrl2(short index)
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
	//<--Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL2;
}

//-----------------------------------------------------------------------------
// ダイアグラムのインデックスから基準金額を取得する
//-----------------------------------------------------------------------------
// 引数	Idx		：	ダイアグラムのインデックス
//		pRtMony	：	該当カラムの金額
//-----------------------------------------------------------------------------
// 返送値	0	：	該当あり
//			-1	：	該当なし
//-----------------------------------------------------------------------------
int CShinFhyo52EX::GetBaseMonyByDiagIdx( short Idx, char **pRtMony )
{
	if( (Idx==ID5214A) || (Idx==ID5215A) || (Idx==ID5216A) || (Idx==ID5217A) || (Idx==ID5218A) || (Idx==ID5219A) ){
		*pRtMony = F526A;
	}
	else if( (Idx==ID5214B) || (Idx==ID5215B) || (Idx==ID5216B) || (Idx==ID5217B) || (Idx==ID5218B) || (Idx==ID5219B) ){
		*pRtMony = F526B;
	}
	else if( (Idx==ID5214C) || (Idx==ID5215C) || (Idx==ID5216C) || (Idx==ID5217C) || (Idx==ID5218C) || (Idx==ID5219C) ){
		*pRtMony = F526C;
	}
	else{
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラムのインデックスから関連項目を取得する
//-----------------------------------------------------------------------------
// 引数	Idx		：	ダイアグラムのインデックス
//		pRtMony	：	該当カラムの金額
//-----------------------------------------------------------------------------
// 返送値	0	：	該当あり
//			-1	：	該当なし
//-----------------------------------------------------------------------------
int CShinFhyo52EX::GetRelInfoByDiagIdx( short Idx, char **pRtMony )
{
	if( Idx == ID526A ){
		*pRtMony = F526A;
	}
	else if( Idx == ID526B ){
		*pRtMony = F526B;
	}
	else if( Idx == ID526C ){
		*pRtMony = F526C;
	}
	else if( Idx == ID5213A ){
		*pRtMony = F5213A;
	}
	else if( Idx == ID5213B ){
		*pRtMony = F5213B;
	}
	else if( Idx == ID5213C ){
		*pRtMony = F5213C;
	}
	else if( Idx == ID527A ){
		*pRtMony = F527A;
	}
	else if( Idx == ID527B ){
		*pRtMony = F527B;
	}
	else if( Idx == ID527C ){
		*pRtMony = F527C;
	}
	else if( Idx == ID5214A ){
		*pRtMony = F5214A;
	}
	else if( Idx == ID5214B ){
		*pRtMony = F5214B;
	}
	else if( Idx == ID5214C ){
		*pRtMony = F5214C;
	}
	else if( Idx == ID528A ){
		*pRtMony = F528A;
	}
	else if( Idx == ID528B ){
		*pRtMony = F528B;
	}
	else if( Idx == ID528C ){
		*pRtMony = F528C;
	}
	else if( Idx == ID5215A ){
		*pRtMony = F5215A;
	}
	else if( Idx == ID5215B ){
		*pRtMony = F5215B;
	}
	else if( Idx == ID5215C ){
		*pRtMony = F5215C;
	}
	else if( Idx == ID529A ){
		*pRtMony = F529A;
	}
	else if( Idx == ID529B ){
		*pRtMony = F529B;
	}
	else if( Idx == ID529C ){
		*pRtMony = F529C;
	}
	else if( Idx == ID5216A ){
		*pRtMony = F5216A;
	}
	else if( Idx == ID5216B ){
		*pRtMony = F5216B;
	}
	else if( Idx == ID5216C ){
		*pRtMony = F5216C;
	}
	else if( Idx == ID5210A ){
		*pRtMony = F5210A;
	}
	else if( Idx == ID5210B ){
		*pRtMony = F5210B;
	}
	else if( Idx == ID5210C ){
		*pRtMony = F5210C;
	}
	else if( Idx == ID5217A ){
		*pRtMony = F5217A;
	}
	else if( Idx == ID5217B ){
		*pRtMony = F5217B;
	}
	else if( Idx == ID5217C ){
		*pRtMony = F5217C;
	}
	else if( Idx == ID5211A ){
		*pRtMony = F5211A;
	}
	else if( Idx == ID5211B ){
		*pRtMony = F5211B;
	}
	else if( Idx == ID5211C ){
		*pRtMony = F5211C;
	}
	else if( Idx == ID5218A ){
		*pRtMony = F5218A;
	}
	else if( Idx == ID5218B ){
		*pRtMony = F5218B;
	}
	else if( Idx == ID5218C ){
		*pRtMony = F5218C;
	}
	else if( Idx == ID5212A ){
		*pRtMony = F5212A;
	}
	else if( Idx == ID5212B ){
		*pRtMony = F5212B;
	}
	else if( Idx == ID5212C ){
		*pRtMony = F5212C;
	}
	else if( Idx == ID5219A ){
		*pRtMony = F5219A;
	}
	else if( Idx == ID5219B ){
		*pRtMony = F5219B;
	}
	else if( Idx == ID5219C ){
		*pRtMony = F5219C;
	}
	else{
		return -1;
	}

	return 0;
}


void CShinFhyo52EX::EditOFFIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// [03'07.15]
	diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
/////////[08'11.07]
//	if( DiagData.data_check == 0 ) return;
//	else	m_Tmp = 0;	// 入力あり
///////////////////
	m_Tmp = 0;	// 入力あり
///////////////////

	char	*ptmpMony=NULL, *pBsMony=NULL;
	char	CkMy[MONY_BUF_SIZE]={0}, CkVl[MONY_BUF_SIZE]={0};

	switch( index ){

/*		case ID526T  :	//	6. 事業別課税売上高合計（期間計）
			memset( CkMy, '\0', 6 );
			memmove( CkMy, F526T, 6 );
			diag_getdata( IDC_ICSDIAGCTRL2, ID526T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, 6 ); 
			if( !l_test( CkMy ) && m_Tmp )	{
				f52calq_all( 1 );
				break;
			}
			if( l_cmp( F526T, CkMy ) )	F520c |= 0x01;
			else						break;		// 表示無し[08'11.07]
			memmove( F526T, CkMy, 6 );
			// 各入力の計算表示
			m_Tmp = 0x00;
			f52calq_all( 1 );
			break;
*/
		case ID526A  :	//	6. 事業別課税売上高合計（3%分）
		case ID526B  :	//	6. 事業別課税売上高合計（4%分）
		case ID526C  :	//	6. 事業別課税売上高合計（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				if( index == ID526A ){
					F520c |= 0x08;
				}
				else if( index == ID526B ){
					F520c |= 0x80;
				}
				else if( index == ID526C ){
					F520c |= 0x800;
				}
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			memmove( ptmpMony, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5212T :	// 12. 事業別課税売上に係る消費税額合計（期間計）
				memmove( CkMy, F5212T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5212T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( F5212T, DiagData.data_val, MONY_BUF_SIZE );
				if( l_cmp( CkMy, F5212T ) )	CQsw &= 0x0f;
				else						break;		// 表示無し[08'11.07]
				// 各入力の計算表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5213A :	// 13. 事業別課税売上に係る消費税額合計（3%分）
		case ID5213B :	// 13. 事業別課税売上に係る消費税額合計（4%分）
		case ID5213C :	// 13. 事業別課税売上に係る消費税額合計（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( ptmpMony, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			// 各入力の計算表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID527T	 :	//	7. 事業別課税売上高･第一種（期間計）
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F527T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID527T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F527T, CkMy ) )	{ F521c |= 0x01;	CQsw &= 0x0f; }
				else						break;		// 表示無し[08'11.07]
				memmove( F527T, CkMy, MONY_BUF_SIZE );
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;*/

		case ID527A  :	//	7. 事業別課税売上高･第一種（3%分）
		case ID527B  :	//	7. 事業別課税売上高･第一種（4%分）
		case ID527C  :	//	7. 事業別課税売上高･第一種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				if( index == ID527A ){
					F521c |= 0x01;
				}
				else if( index == ID527B ){
					F521c |= 0x10;
				}
				else if( index == ID527C ){
					F521c |= 0x100;	
				}
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5213T :	// 13. 事業別課税売上に係る消費税額･第一種（期間計）
				memmove( CkMy, F5213T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5213T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( F5213T, DiagData.data_val, MONY_BUF_SIZE );
				if( l_cmp( CkMy, F5213T ) )	CQsw &= 0x0f;
				else						break;		// 表示無し[08'11.07]
			// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5214A :	// 14. 事業別課税売上に係る消費税額･第一種（3%分）
		case ID5214B :	// 14. 事業別課税売上に係る消費税額･第一種（4%分）
		case ID5214C :	// 14. 事業別課税売上に係る消費税額･第一種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( ptmpMony, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(CkMy, ptmpMony) ){
				//[Y]
				if( GetBaseMonyByDiagIdx(index, &pBsMony) == 0 ){
					if( m_pArith->l_test(pBsMony) ){
						if( index == ID5214A ){
							F5214c |= 0x08;
						}
						else if( index == ID5214B ){
							F5214c |= 0x80;
						}
						else if( index == ID5214C ){
							F5214c |= 0x800;
						}
					}
				}

//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID528T  :	//	8. 事業別課税売上高･第二種（期間計）
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F528T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID528T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F528T, CkMy ) )	{ F522c |= 0x01;	CQsw &= 0x0f; }
				else						break;		// 表示無し[08'11.07]
				memmove( F528T, CkMy, MONY_BUF_SIZE );
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID528A  :	//	8. 事業別課税売上高･第二種（3%分）
		case ID528B  :	//	8. 事業別課税売上高･第二種（4%分）
		case ID528C  :	//	8. 事業別課税売上高･第二種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				if( index == ID528A ){
					F522c |= 0x01;
				}
				else if( index == ID528B ){
					F522c |= 0x10;
				}
				else if( index == ID528C ){
					F522c |= 0x100;
				}

//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5214T :	// 14. 事業別課税売上に係る消費税額･第二種（期間計）
				memmove( CkMy, F5214T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5214T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( F5214T, DiagData.data_val, MONY_BUF_SIZE );
				if( l_cmp( CkMy, F5214T ) )	CQsw &= 0x0f;
				else						break;		// 表示無し[08'11.07]
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5215A :	// 15. 事業別課税売上に係る消費税額･第二種（3%分）
		case ID5215B :	// 15. 事業別課税売上に係る消費税額･第二種（4%分）
		case ID5215C :	// 15. 事業別課税売上に係る消費税額･第二種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( ptmpMony, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(CkMy, ptmpMony) ){
				//[Y]
				if( GetBaseMonyByDiagIdx(index, &pBsMony) == 0 ){
					if( m_pArith->l_test(pBsMony) ){
						if( index == ID5215A ){
							F5215c |= 0x08;
						}
						else if( index == ID5215B ){
							F5215c |= 0x80;
						}
						else if( index == ID5215C ){
							F5215c |= 0x800;
						}
					}
				}
			
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID529T  :	//	9. 事業別課税売上高･第三種（期間計）
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F529T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID529T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F529T, CkMy ) )	{ F523c |= 0x01;	CQsw &= 0x0f; }
				else						break;		// 表示無し[08'11.07]
				memmove( F529T, CkMy, MONY_BUF_SIZE );
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID529A  :	//	9. 事業別課税売上高･第三種（3%分）
		case ID529B  :	//	9. 事業別課税売上高･第三種（4%分）
		case ID529C  :	//	9. 事業別課税売上高･第三種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				if( index == ID529A ){
					F523c |= 0x01;
				}
				else if( index == ID529B ){
					F523c |= 0x10;
				}
				else if( index == ID529C ){
					F523c |= 0x100;
				}
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5215T :	// 15. 事業別課税売上に係る消費税額･第三種（期間計）
				memmove( CkMy, F5215T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5215T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( F5215T, DiagData.data_val, MONY_BUF_SIZE );
				if( l_cmp( CkMy, F5215T ) )	CQsw &= 0x0f;
				else						break;		// 表示無し[08'11.07]
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5216A :	// 16. 事業別課税売上に係る消費税額･第三種（3%分）
		case ID5216B :	// 16. 事業別課税売上に係る消費税額･第三種（4%分）
		case ID5216C :	// 16. 事業別課税売上に係る消費税額･第三種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( ptmpMony, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(CkMy, ptmpMony) ){
				//[Y]
				if( GetBaseMonyByDiagIdx(index, &pBsMony) == 0 ){
					if( m_pArith->l_test(pBsMony) ){
						if( index == ID5216A ){
							F5216c |= 0x08;
						}
						else if( index == ID5216B ){
							F5216c |= 0x80;
						}
						else if( index == ID5216C ){
							F5216c |= 0x800;
						}
					}
				}
				
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5210T :	// 10. 事業別課税売上高･第四種（期間計）
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F5210T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5210T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F5210T, CkMy ) )	{ F524c |= 0x01;	CQsw &= 0x0f; }
				else						break;		// 表示無し[08'11.07]
				memmove( F5210T, CkMy, MONY_BUF_SIZE );
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5210A :	// 10. 事業別課税売上高･第四種（3%分）
		case ID5210B :	// 10. 事業別課税売上高･第四種（4%分）
		case ID5210C :	// 10. 事業別課税売上高･第四種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				if( index == ID5210A ){
					F524c |= 0x01;
				}
				else if( index == ID5210B ){
					F524c |= 0x10;
				}
				else if( index == ID5210C ){
					F524c |= 0x100;
				}
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5216T :	// 16. 事業別課税売上に係る消費税額･第四種（期間計）
				memmove( CkMy, F5216T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5216T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( F5216T, DiagData.data_val, MONY_BUF_SIZE );
				if( l_cmp( CkMy, F5216T ) )	CQsw &= 0x0f;
				else						break;		// 表示無し[08'11.07]
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5217A :	// 17. 事業別課税売上に係る消費税額･第四種（3%分）
		case ID5217B :	// 17. 事業別課税売上に係る消費税額･第四種（4%分）
		case ID5217C :	// 17. 事業別課税売上に係る消費税額･第四種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( ptmpMony, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(CkMy, ptmpMony) ){
				//[Y]
				if( GetBaseMonyByDiagIdx(index, &pBsMony) == 0 ){
					if( m_pArith->l_test(pBsMony) ){
						if( index == ID5217A ){
							F5217c |= 0x08;
						}
						else if( index == ID5217B ){
							F5217c |= 0x80;
						}
						else if( index == ID5217C ){
							F5217c |= 0x800;
						}
					}
				}


//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5211T :	// 11. 事業別課税売上高･第五種（期間計）
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F5211T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5211T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F5211T, CkMy ) )	{ F525c |= 0x01;	CQsw &= 0x0f; }
				else						break;		// 表示無し[08'11.07]
				memmove( F5211T, CkMy, MONY_BUF_SIZE );
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5211A :	// 11. 事業別課税売上高･第五種（3%分）
		case ID5211B :	// 11. 事業別課税売上高･第五種（4%分）
		case ID5211C :	// 11. 事業別課税売上高･第五種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				if( index == ID5211A ){
					F525c |= 0x01;
				}
				else if( index == ID5211B ){
					F525c |= 0x10;
				}
				else if( index == ID5211C ){
					F525c |= 0x100;
				}
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

/*		case ID5217T :	// 17. 事業別課税売上に係る消費税額･第五種（期間計）
				memmove( CkMy, F5217T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5217T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( F5217T, DiagData.data_val, MONY_BUF_SIZE );
				if( l_cmp( CkMy, F5217T ) )	CQsw &= 0x0f;
				else						break;		// 表示無し[08'11.07]
				// 各入力の計表示
				f52calq_all( 1 );
			// 入力直後の特例計算サイン
				Tokurei52();
			// 各入力の計算表示
				f52dsp_tk();
				break;
*/
		case ID5218A :	// 18. 事業別課税売上に係る消費税額･第五種（3%分）
		case ID5218B :	// 18. 事業別課税売上に係る消費税額･第五種（4%分）
		case ID5218C :	// 18. 事業別課税売上に係る消費税額･第五種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( ptmpMony, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(CkMy, ptmpMony) ){
				//[Y]
				if( GetBaseMonyByDiagIdx(index, &pBsMony) == 0 ){
					if( m_pArith->l_test(pBsMony) ){
						if( index == ID5218A ){
							F5218c |= 0x08;
						}
						else if( index == ID5218B ){
							F5218c |= 0x80;
						}
						else if( index == ID5218C ){
							F5218c |= 0x800;
						}
					}
				}
				
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

		case ID5212A :	// 12. 事業別課税売上高･第六種（3%分）
		case ID5212B :	// 12. 事業別課税売上高･第六種（4%分）
		case ID5212C :	// 12. 事業別課税売上高･第六種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				if( index == ID5212A ){
					F526c |= 0x01;
				}
				else if( index == ID5212B ){
					F526c |= 0x10;
				}
				else if( index == ID5212C ){
					F526c |= 0x100;
				}
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;

		case ID5219A :	// 19. 事業別課税売上に係る消費税額･第五種（3%分）
		case ID5219B :	// 19. 事業別課税売上に係る消費税額･第五種（4%分）
		case ID5219C :	// 19. 事業別課税売上に係る消費税額･第五種（6.3%分）
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( ptmpMony, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(CkMy, ptmpMony) ){
				//[Y]
				if( GetBaseMonyByDiagIdx(index, &pBsMony) == 0 ){
					if( m_pArith->l_test(pBsMony) ){
						if( index == ID5219A ){
							F5219c |= 0x08;
						}
						else if( index == ID5219B ){
							F5219c |= 0x80;
						}
						else if( index == ID5219C ){
							F5219c |= 0x800;
						}
					}
				}
				
//--> '14.03.02 CUT
//				m_CQsw &= 0x0f;
			}
			else{
				// 表示無し[08'11.07]
				break;
			}
			// 各入力の計表示
			f52calq_all( 1 );
			// 入力直後の特例計算サイン
			Tokurei52();
			// 各入力の計算表示
//			f52dsp_tk();
			break;


/*		case ID5218T :	// 18. 原則計算を適用する場合
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F5218T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5218T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F5218T, CkMy ) )	{
					F5218c |= 0x01;
					Color_Change( IDC_ICSDIAGCTRL2, ID5218T, 3 );			//2 );
					memmove( F5218T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				}
				else	memmove( F5218T, CkMy, MONY_BUF_SIZE );
				// 各入力の計表示
				f52dsp_tk();
				CQsw |= 0x10;
				break;

		case ID5218B :	// 18. 原則計算を適用する場合
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F5218B, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL2, ID5218B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F5218B, CkMy ) )	{
					F5218c |= 0x10;
					Color_Change( IDC_ICSDIAGCTRL2, ID5218B, 3 );			//2 );
				}
				memmove( F5218B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
				f52dsp_tk();
				CQsw |= 0x10;
				break;

		case ID5219T :	// 19. 特例適用･合計 仕入税額(期間計)
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 1 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5219T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 1 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5219B :	// 19. 特例適用･合計 仕入税額(4%分)	
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 1 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5219B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 1 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5220T :	// 20.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 2 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5220T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 2 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5220B :	// 20.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 2 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5220B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 2 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5221T :	// 21.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 3 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5221T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 3 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5221B :	// 21.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 3 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5221B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 3 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5222T :	// 22.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 4 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5222T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 4 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5222B :	// 22.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 4 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5222B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 4 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5223T :	// 23.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 5 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5223T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 5 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5223B :	// 23.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 5 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5223B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 5 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5224T :	// 24.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == MONY_BUF_SIZE )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5224T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 6 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5224B :	// 24.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == MONY_BUF_SIZE )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5224B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 6 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5225T :	// 25.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 7 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5225T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 7 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5225B :	// 25.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 7 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5225B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 7 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5226T :	// 26.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 8 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5226T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 8 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5226B :	// 26.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 8 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5226B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 8 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5227T :	// 27.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 9 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5227T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 9 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5227B :	// 27.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 9 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5227B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 9 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5228T :	// 28.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 10 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5228T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 10 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5228B :	// 28.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 10 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5228B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 10 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5229T :	// 29.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 11 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5229T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 11 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;

		case ID5229B :	// 29.
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SBsw&0xff) == 11 )	{
					memmove( CkMy, F5219B, MONY_BUF_SIZE );
					memmove( CkVl, F5219B, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL2, ID5229B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SBsw = ( char ) ( l_test( CkMy ) ? 11 : 0 );
					memmove( F5219B, CkMy, MONY_BUF_SIZE );
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;*/
		default :
				break;
	}
}

void CShinFhyo52EX::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}
	
	// Shift+TABをTABに、TABをENTERに返還
	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
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
		m_Fhyo52Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo52Diag02.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) ){
		m_Fhyo52Diag02.SetNextPosition();
	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Fhyo52Diag02.SetPrevPosition();
	}

	if( wTerm == VK_DELETE ){
		//[Y]
		switch( index ){
			case ID526A	:	//	6. 事業別課税売上高合計（3%分）
				F520c &= ~0x08;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID526A );
				break;

			case ID526B	:	//	6. 事業別課税売上高合計（4%分）
				F520c &= ~0x80;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID526B );
				break;

			case ID526C	:	//	6. 事業別課税売上高合計（6.3%分）
				F520c &= ~0x800;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID526C );
				break;

			case ID5214A :	// 14. 事業別課税売上に係る消費税額･第一種（3%分）
				F5214c &= ~0x08;
				F521c |= 0x01;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5214A );
				break;

			case ID5214B :	// 14. 事業別課税売上に係る消費税額･第一種（4%分）
				F5214c &= ~0x80;
				F521c |= 0x10;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5214B );
				break;

			case ID5214C :	// 14. 事業別課税売上に係る消費税額･第一種（6.3%分）
				F5214c &= ~0x800;
				F521c |= 0x100;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5214C );
				break;

			case ID5215A :	// 15. 事業別課税売上に係る消費税額･第二種（3%分）
				F5215c &= ~0x08;
				F522c |= 0x01;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5215A );
				break;

			case ID5215B :	// 15. 事業別課税売上に係る消費税額･第二種（4%分）
				F5215c &= ~0x80;
				F522c |= 0x10;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5215B );
				break;

			case ID5215C :	// 15. 事業別課税売上に係る消費税額･第二種（6.3%分）
				F5215c &= ~0x800;
				F522c |= 0x100;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5215C );
				break;

			case ID5216A :	// 16. 事業別課税売上に係る消費税額･第三種（3%分）
				F5216c &= ~0x08;
				F523c |= 0x01;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5216A );
				break;

			case ID5216B :	// 16. 事業別課税売上に係る消費税額･第三種（4%分）
				F5216c &= ~0x80;
				F523c |= 0x10;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5216B );
				break;

			case ID5216C :	// 16. 事業別課税売上に係る消費税額･第三種（6.3%分）
				F5216c &= ~0x800;
				F523c |= 0x100;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5216C );
				break;

			case ID5217A :	// 17. 事業別課税売上に係る消費税額･第四種（3%分）
				F5217c &= ~0x08;
				F524c |= 0x01;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5217A );
				break;

			case ID5217B :	// 17. 事業別課税売上に係る消費税額･第四種（4%分）
				F5217c &= ~0x80;
				F524c |= 0x10;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5217B );
				break;

			case ID5217C :	// 17. 事業別課税売上に係る消費税額･第四種（6.3%分）
				F5217c &= ~0x800;
				F524c |= 0x100;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5217C );
				break;

			case ID5218A :	// 18. 事業別課税売上に係る消費税額･第五種（3%分）
				F5218c &= ~0x08;
				F525c |= 0x01;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5218A );
				break;

			case ID5218B :	// 18. 事業別課税売上に係る消費税額･第五種（4%分）
				F5218c &= ~0x80;
				F525c |= 0x10;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5218B );
				break;

			case ID5218C :	// 18. 事業別課税売上に係る消費税額･第五種（6.3%分）
				F5218c &= ~0x800;
				F525c |= 0x100;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5218C );
				break;

			case ID5219A :	// 19. 事業別課税売上に係る消費税額･第六種（3%分）
				F5219c &= ~0x08;
				F526c |= 0x01;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5219A );
				break;

			case ID5219B :	// 19. 事業別課税売上に係る消費税額･第六種（4%分）
				F5219c &= ~0x80;
				F526c |= 0x10;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5219B );
				break;

			case ID5219C :	// 19. 事業別課税売上に係る消費税額･第六種（6.3%分）
				F5219c &= ~0x800;
				F526c |= 0x100;
				// 各入力の計算表示
				f52calq_all( 1 );
				m_Fhyo52Diag02.SetPosition( ID5219C );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL2 , index);//[Y]
				break;
		}
	}

	//-->Editonから移動 harino ('14.03.07)
	int nowpos = m_Fhyo52Diag02.GetPosition();	// 移動先または自身のポジション

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
int CShinFhyo52EX::UpDownFocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	for(i = 0 ; i < FCS_FHYO52CNT ; i++){
		if(Index == FCS_Fhyo52[i].IDNo){
			break;
		}
	}

	idx = FCS_Fhyo52[i].IDNo;//現在ポジションセット

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHYO52CNT ; j++){
			m_Fhyo52Diag02.GetAttribute(FCS_Fhyo52[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo52[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo52Diag02.GetAttribute(FCS_Fhyo52[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo52[j].IDNo;
				break;
			}
		}
	}

	return idx;
}



//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo52EX::SetItemPosition()
{
	int index;

	//-->追加 harino ('14.03.06)
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_Fhyo52Diag02.GetPosition();
		if( (index = m_Fhyo52Diag02.GetPosition()) != -1) {
			m_Fhyo52Diag02.SetPosition(index);
			m_Fhyo52Diag02.SetFocus();
			return;
		}
	}
	//<--追加 harino ('14.03.06)

//	int nowIdx = m_Fhyo52Diag02.GetPosition();
//	if( nowIdx == ID527A ){
//		return;
//	}

	//入力可能なインデックスを探す処理が必要？
	m_Fhyo52Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo52Diag02.SetFocus();
	m_Fhyo52Diag02.SetPosition( ID527A );
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額の再書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo52EX::ReWriteOnlySkjData()
{
	// １種類の事業の専業者の場合
	memset( (*m_pSnFhyo40Data)->Sn_1F4A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo40Data)->Sn_1F4B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo40Data)->Sn_1F4C, '\0', MONY_BUF_SIZE );

	if( m_CQsw & 0x0f ){
		memmove( (*m_pSnFhyo40Data)->Sn_1F4A, F525A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4B, F525B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F525C, MONY_BUF_SIZE );
	}
	else{
		memmove( (*m_pSnFhyo40Data)->Sn_1F4A, F5237A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4B, F5237B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F5237C, MONY_BUF_SIZE );
	}

	return 0;
}

