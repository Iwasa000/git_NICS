// ShinHjnKskEX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinHjnKskEX.h"
#include "H31HyoView.h"

#include "ShinHjnKskEXIdx.h"


// CShinHjnKskEX ダイアログ

IMPLEMENT_DYNAMIC(CShinHjnKskEX, CSyzBaseDlg)

CShinHjnKskEX::CShinHjnKskEX(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinHjnKskEX::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pSnKskData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_Clm(0)
	, m_Idx(0)
	, m_pParent(pParent)
{
	m_curwnd = -1;
}

CShinHjnKskEX::~CShinHjnKskEX()
{
}

void CShinHjnKskEX::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL10, m_dig1);
	DDX_Control(pDX, IDC_ICSDIAGCTRL20, m_dig2);
	DDX_Control(pDX, IDC_ICSDIAGCTRL30, m_dig3);
	DDX_Control(pDX, IDC_ICSDIAGCTRL40, m_dig4);
	DDX_Control(pDX, IDC_ICSDIAGCTRL50, m_dig5);

	DDX_Control(pDX, IDC_EDIT_MYNUM1, m_edit_mynum1);
	DDX_Control(pDX, IDC_EDIT_MYNUM2, m_edit_mynum2);
	DDX_Control(pDX, IDC_EDIT_MYNUM3, m_edit_mynum3);
	DDX_Control(pDX, IDC_EDIT_MYNUM4, m_edit_mynum4);
}


BEGIN_MESSAGE_MAP(CShinHjnKskEX, CSyzBaseDlg)
	ON_COMMAND(ID_BUTTON_F2, &CShinHjnKskEX::OnButtonF2)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShinHjnKskEX メッセージ ハンドラ

BOOL CShinHjnKskEX::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
//	m_pBottomDiag = &m_dig4;
//	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAGCTRL40);
	m_BottomIdx = 19;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSnKskData		：	更正の請求データ
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinHjnKskEX::InitInfo( CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH31SnKskData **pSnKskData )
{
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

	ASSERT( pSnKskData );
	if( pSnKskData == NULL ){
		return -1;
	}
	m_pSnKskData = pSnKskData;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CShinHjnKskEX::Init()
{
	InitDiagAttr();

	/*
	//処理カット
	// 平成23年10月2日より前の場合に許可
	char	yymmdd[3]={0};
	(*m_pSnHeadData)->GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	if( m_pArith->l_jsgn(yymmdd, AMENDH, 3) >= 0 ){
		m_dig5.ModifyItem( DLG0CMB, DIAG_MDFY_TRANSPARENT );
		m_dig5.ModifyItem( DLG0CMB-1, DIAG_MDFY_TRANSPARENT );
	}
	else{
		DIAGRAM_DATA	data;
		if( ((*m_pSnHeadData)->Sn_Syuusei&0x06) == 0x02 ){
			data.data_combo = 0;
		}
		else{
			data.data_combo = 1;
		}
		diag_setdata( IDC_ICSDIAGCTRL50, DLG0CMB, &data, CIcsdiagctrl );
	}
	*/

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinHjnKskEX::InitDiagAttr()
{
	
	DIAGRAM_ATTRIBUTE data;
	int i;
	short dg1[] = { DLG1_AD1, DLG1_AD2, DLG1_HNM, DLG1_DNM, -1 };
	short dg3[] = { DLG3REA, DLG3DOC, DLG3BNM, DLG3SNM, DLG3DNM, DLG3PNM, -1 };

	for( i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_dig1.GetAttribute( dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_dig1.SetAttribute( dg1[i], (LPUNKNOWN)&data, 1 );
	}

	for( i=0;; i++ ){
		if( dg3[i] == -1 ){
			break;
		}
		// ank対応
		m_dig3.GetAttribute( dg3[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_dig3.SetAttribute( dg3[i], (LPUNKNOWN)&data, 1 );
	}


	// 法人番号
	int kubun;
	HWND edit[10] = {NULL};
	int diag_index[2][10] = {{-1},-1};

	kubun = MN_HOJIN;

	edit[0] = m_edit_mynum1.GetSafeHwnd();
	edit[1] = m_edit_mynum2.GetSafeHwnd();
	edit[2] = m_edit_mynum3.GetSafeHwnd();
	edit[3] = m_edit_mynum4.GetSafeHwnd();

	diag_index[0][0] = HJNKSK_NUM_HOJIN1;		
	diag_index[0][1] = HJNKSK_NUM_HOJIN2;
	diag_index[0][2] = HJNKSK_NUM_HOJIN3;
	diag_index[0][3] = HJNKSK_NUM_HOJIN4;
	diag_index[0][4] = -1;

	m_ctrl_mynum.Init( this, kubun , (*m_pSnHeadData)->m_apno, TRUE, edit, m_dig1.GetSafeHwnd(), diag_index, MN_NO_PICKUP_DIAGDATA );

	int ret;
	CString str_myno;

	(*m_pSnHeadData)->GetMyNumber(str_myno);

	ret = m_ctrl_mynum.SetData(str_myno);

	MoveDiagMNEdit();

	m_ctrl_mynum.SetHeightMargin(MN_HEIGHT_CENTER);
	m_ctrl_mynum.EnableWindow(FALSE);
	m_ctrl_mynum.ShowWindow(SW_SHOW);

	DispKsiHjn( 0 );

	int	cnt;

	cnt = 0;
	while( 1 ) {
		if( HjnKsk1[cnt].Index == 0 )	break;
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = HjnKsk1[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = HjnKsk1[cnt].UnConnect;
			}
			else{
				Flg = HjnKsk1[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL10, HjnKsk1[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, HjnKsk1[cnt].Connect );

		if( HjnKsk1[cnt].Index == DLG1_HKN || HjnKsk1[cnt].Index == DLG1_DKN )
			m_dig1.ModifyItem( HjnKsk1[cnt].Index, ( Flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY ) | DIAG_MODIFY_IMEKANA );
		cnt++;
	}

	cnt = 0;
	while( 1 ) {
		if( HjnKsk2[cnt].Index == 0 )	break;
		int	Flg;
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = HjnKsk2[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = HjnKsk2[cnt].UnConnect;
			}
			else{
				Flg = HjnKsk2[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL20, HjnKsk2[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, HjnKsk2[cnt].Connect );
		cnt++;
	}

	cnt = 0;
	while( 1 ) {
		if( HjnKsk3[cnt].Index == 0 )	break;
		int	Flg;
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 )	Flg = HjnKsk3[cnt].AllUnConnect;
		else	{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 )	Flg = HjnKsk3[cnt].UnConnect;
			else								Flg = HjnKsk3[cnt].Connect;
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL30, HjnKsk3[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, HjnKsk3[cnt].Connect );
		cnt++;
	}

	// 平成23年10月2日より前の場合に許可
	char	yymmdd[3]={0};
	(*m_pSnHeadData)->GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	if( m_pArith->l_jsgn(yymmdd, AMENDH, 3) < 0 ){
		cnt = 0;
		while( 1 ){
			if( HjnKsk5[cnt].Index == 0 )	break;
			int	Flg;
			if( (*m_pSnHeadData)->Sn_Sign4&0x01 )	Flg = HjnKsk5[cnt].AllUnConnect;
			else	{
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 )	Flg = HjnKsk5[cnt].UnConnect;
				else								Flg = HjnKsk5[cnt].Connect;
			}
			ATRIB_MOD( IDC_ICSDIAGCTRL50, HjnKsk5[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, HjnKsk5[cnt].Connect );
			cnt++;
		}
	}

	m_dig1.EnableDelete(0);
	m_dig2.EnableDelete(0);
	m_dig3.EnableDelete(0);
	

	return 0;
}


//-----------------------------------------------------------------------------
// 画面出力
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CShinHjnKskEX::DispKsiHjn( int mode )
{
	
	m_dig1.OffControl( TRUE );
	m_dig2.OffControl( TRUE );
	m_dig3.OffControl( TRUE );

	DIAGRAM_DATA	data;
	int				i, ln;
	char			buf[512]={0};

	for( i=DLG1_TAX; i<=DLG1_HNO; i++ ){
		m_Util.DiagOcxIniz( &data );
		::ZeroMemory( buf, sizeof( buf ));
		diag_clear( IDC_ICSDIAGCTRL10, i, TRUE, CIcsdiagctrl );
		switch( i ){
		case DLG1_TAX:	// 税務署長
			memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, sizeof((*m_pSnHeadData)->Sn_ZMSYO) );
			data.data_edit = buf;
			break;
		case DLG1_ZIP:	// 納税地〒
			memmove( buf, (*m_pSnHeadData)->Sn_ZIP, sizeof((*m_pSnHeadData)->Sn_ZIP));
			data.data_edit = buf;
			break;
		case DLG1_AD1:	// 納税地１
			ln = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_NOZEI, 40 );
			memmove( buf, &(*m_pSnHeadData)->Sn_NOZEI[0], ln );
			data.data_edit = buf;
			break;
		case DLG1_AD2:	// 納税地２
			ln = m_Util.MixSpaceCutLength( (char *)&(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
			memmove( buf, &(*m_pSnHeadData)->Sn_NOZEI[40], ln );
			data.data_edit = buf;
			break;
		case DLG1_TL1:	// 電話番号
			memmove( buf, (*m_pSnHeadData)->Sn_TELNO1, 6 );
			data.data_edit = buf;
			break;
		case DLG1_TL2:	// 電話番号
			memmove( buf, (*m_pSnHeadData)->Sn_TELNO2, 4 );
			data.data_edit = buf;
			break;
		case DLG1_TL3:	// 電話番号
			memmove( buf, (*m_pSnHeadData)->Sn_TELNO3, 4 );
			data.data_edit = buf;
			break;
		case DLG1_HKN:	// 法人名フリガナ
			memmove( buf, (*m_pSnHeadData)->Sn_COFRI, sizeof( (*m_pSnHeadData)->Sn_COFRI ));
			data.data_edit = buf;
			break;
		case DLG1_HNM:	// 法人名
			memmove( buf, (*m_pSnHeadData)->Sn_CONAM, 40 );
			data.data_edit = buf;
			break;
		case DLG1_DKN:	// 代表者名フリガナ
			memmove( buf, (*m_pSnHeadData)->Sn_DIFRI, sizeof( (*m_pSnHeadData)->Sn_DIFRI ));
			data.data_edit = buf;
			break;
		case DLG1_DNM:	// 代表者名
			memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, 40 );
			data.data_edit = buf;
			break;

		case DLG1_HNO:	// 法人番号

			int ret;
			CString str_myno;
			(*m_pSnHeadData)->GetMyNumber(str_myno);
			ret = m_ctrl_mynum.SetData(str_myno);
			continue;

		}
		diag_setdata( IDC_ICSDIAGCTRL10, i, &data, CIcsdiagctrl );
	}
	for( i = DLG1SDT; i <= DLG1EDT; i++ ){
		m_Util.DiagOcxIniz( &data );
		::ZeroMemory( buf, sizeof( buf ));
		diag_clear( IDC_ICSDIAGCTRL10, i, TRUE, CIcsdiagctrl );

		char	yymmdd[3]={0};
		int		gengo = 0;
		int		gengoIdx = 0;

		switch( i ){
			case DLG1SDT:	// 課税期間　自
				memset( yymmdd, '\0', sizeof(yymmdd) );
				(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
				memmove( data.data_day, yymmdd, 3 );
				gengoIdx =DLG1SDG;
				break;
			case DLG1EDT:	// 課税期間　至
				memset( yymmdd, '\0', sizeof(yymmdd) );
				(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
				memmove( data.data_day, yymmdd, 3 );
				gengoIdx =DLG1EDG;
				break;
		}
		diag_setdata( IDC_ICSDIAGCTRL10, i, &data, CIcsdiagctrl );

		// 元号
		char	tmpbuf[512] = { 0 };
		if( gengo==ID_ICSSH_REIWA_GENGO  ) {
			sprintf_s ( tmpbuf, sizeof ( tmpbuf ), _T( "%s" ), SYZ31_REIWA_G_STR );
		}
		else {
			sprintf_s ( tmpbuf, sizeof ( tmpbuf ), _T( "%s" ), SYZ31_HEISEI_G_STR );
		}

		diag_clear ( IDC_ICSDIAGCTRL10, gengoIdx, TRUE, CIcsdiagctrl );
		m_Util.DiagOcxIniz ( &data );
		::ZeroMemory ( buf, sizeof ( buf ) );

		if ( gengoIdx == DLG1SDG ) {
			sprintf_s ( buf, sizeof ( buf ), _T( "自 %s" ), tmpbuf );
		}
		else if ( gengoIdx == DLG1EDG ) {
			sprintf_s ( buf, sizeof ( buf ), _T( "至 %s" ), tmpbuf );
		}
		data.data_disp = buf;
		diag_setdata ( IDC_ICSDIAGCTRL10, gengoIdx, &data, CIcsdiagctrl );
	}

	DispKsiHjnDig1();
	DispKsiHjnDig2();
	DispKsiHjnDig3();
	DispKsiHjnDigZi();

	m_dig1.OffControl( FALSE );
	m_dig2.OffControl( FALSE );
	m_dig3.OffControl( FALSE );

}

//-----------------------------------------------------------------------------
// データ表示
//-----------------------------------------------------------------------------
void CShinHjnKskEX::DispKsiHjnDig1()
{
	DIAGRAM_DATA	data;
	int		i, ln;
	char	buf[512]={0};

	for( i = DLG1_TAX; i <= DLG1_DNM; i++ ){
		m_Util.DiagOcxIniz( &data );
		if(i != DLG1KSW && i != DLG1HDT )
			diag_clear( IDC_ICSDIAGCTRL10, i, TRUE, CIcsdiagctrl );
		::ZeroMemory( buf, sizeof( buf ));
		switch( i ){
		case DLG1_TAX:	// 税務署長
			memmove( buf, (*m_pSnHeadData)->Sn_ZMSYO, 12 );
			data.data_edit = buf;
			break;
		case DLG1_ZIP:	// 納税地〒
			memmove( buf, (*m_pSnHeadData)->Sn_ZIP, sizeof((*m_pSnHeadData)->Sn_ZIP));
			data.data_edit = buf;
			break;
		case DLG1_AD1:	// 納税地１
			ln = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_NOZEI, 40 );
			memmove( buf, &(*m_pSnHeadData)->Sn_NOZEI[0], ln );
			data.data_edit = buf;
			break;
		case DLG1_AD2:	// 納税地２
			ln = m_Util.MixSpaceCutLength( (char *)&(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
			memmove( buf, &(*m_pSnHeadData)->Sn_NOZEI[40], ln );
			data.data_edit = buf;
			break;
		case DLG1_TL1:	// 電話番号
			memmove( buf, (*m_pSnHeadData)->Sn_TELNO1, 6 );
			data.data_edit = buf;
			break;
		case DLG1_TL2:	// 電話番号
			memmove( buf, (*m_pSnHeadData)->Sn_TELNO2, 4 );
			data.data_edit = buf;
			break;
		case DLG1_TL3:	// 電話番号
			memmove( buf, (*m_pSnHeadData)->Sn_TELNO3, 4 );
			data.data_edit = buf;
			break;
		case DLG1_HKN:	// 法人名フリガナ
			memmove( buf, (*m_pSnHeadData)->Sn_COFRI, sizeof( (*m_pSnHeadData)->Sn_COFRI ));
			data.data_edit = buf;
			break;
		case DLG1_HNM:	// 法人名
			memmove( buf, (*m_pSnHeadData)->Sn_CONAM, 40 );
			data.data_edit = buf;
			break;
		case DLG1_DKN:	// 代表者名フリガナ
			memmove( buf, (*m_pSnHeadData)->Sn_DIFRI, sizeof( (*m_pSnHeadData)->Sn_DIFRI ));
			data.data_edit = buf;
			break;
		case DLG1_DNM:	// 代表者名
			memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, 40 );
			data.data_edit = buf;
			break;
		}
		diag_setdata( IDC_ICSDIAGCTRL10, i, &data, CIcsdiagctrl );
	}

	char	yymmdd[3]={0};
	for( i = DLG1TDT; i <= DLG1KSW; i++ ){
		m_Util.DiagOcxIniz( &data );
		if(i != DLG1KSW && i != DLG1HDT ){
			diag_clear( IDC_ICSDIAGCTRL10, i, TRUE, CIcsdiagctrl );
		}
		switch( i ){
			case DLG1TDT:	// 提出年月日
				memmove( data.data_day, &(*m_pSnKskData)->KsData.KSdata[1], 3 );
				break;
			case DLG1HDT:	// 適用法令の区分
				data.data_combo = (*m_pSnKskData)->KsData.KSthsw;
				break;
			case DLG1KDT:	// 更正決定通知書日付
				//memmove( data.data_day, &(*m_pSnKskData)->KsData.KSktdt[0], 4 );	
				if ( (*m_pSnKskData)->KsData.KSktdt[0] == 0 ) {
					continue;
				}
				else {
					memmove( data.data_day, &(*m_pSnKskData)->KsData.KSktdt[0], 4 );	
				}
				break;
			case DLG1KSW:	// 申告、更正、決定
				data.data_combo = (*m_pSnKskData)->KsData.KSktsw;	
				break;
		}
		diag_setdata( IDC_ICSDIAGCTRL10, i, &data, CIcsdiagctrl );
	}

}

//-----------------------------------------------------------------------------
// データ表示
//-----------------------------------------------------------------------------
void CShinHjnKskEX::DispKsiHjnDig2()
{
	DIAGRAM_DATA	data;
	int		i, idx;

	for( i = 0; i < DLG2LIN; i++ ){	// この請求前の金額
		idx = DLG2VSS+i;
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL20, idx, TRUE, CIcsdiagctrl );
		memmove( data.data_val, (*m_pSnKskData)->KsData.KSval[i].KVsval, 6 );
		diag_setdata( IDC_ICSDIAGCTRL20, idx, &data, CIcsdiagctrl );
	}
	for( i = 0; i < DLG2LIN; i++ ){	// 更正の請求金額
		idx = DLG2VKS+i;
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL20, idx+i, TRUE, CIcsdiagctrl );
		memmove( data.data_val, (*m_pSnKskData)->KsData.KSval[i].KVkval, 6 );
		diag_setdata( IDC_ICSDIAGCTRL20, idx, &data, CIcsdiagctrl );
	}
}

//-----------------------------------------------------------------------------
// データ表示
//-----------------------------------------------------------------------------
void CShinHjnKskEX::DispKsiHjnDig3()
{
	DIAGRAM_DATA	data;
	int		i;

	for( i = DLG3REA; i <= DLG3PNM; i++ ){
		m_Util.DiagOcxIniz( &data );
		if(i == DLG3BSW || i == DLG3BKN || i == DLG3SKN);
		else	diag_clear( IDC_ICSDIAGCTRL30, i, TRUE, CIcsdiagctrl );
		switch( i ){
		case DLG3REA:	// 更正の請求をする理由等
			m_Util.LineDataGet( data.data_edit, (*m_pSnKskData)->KsData.KSreas, 40*2, 3 );	
			break;
		case DLG3SDT:	// 修正申告提出年月日
			//memmove( data.data_day, &(*m_pSnKskData)->KsData.KSstdt[0], 4 );
			if ( (*m_pSnKskData)->KsData.KSstdt[0] == 0 ) {
				continue;
			}
			else {
				memmove( data.data_day, &(*m_pSnKskData)->KsData.KSstdt[0], 4 );
			}
			break;
		case DLG3JDT:	// 更正決定通知書受理年月日
			//memmove( data.data_day, &(*m_pSnKskData)->KsData.KSjrdt[0], 4 );	
			if ( ( *m_pSnKskData )->KsData.KSjrdt[0] == 0 ) {
				continue;
			}
			else {
				memmove( data.data_day, &(*m_pSnKskData)->KsData.KSjrdt[0], 4 );
			}
			break;
		case DLG3DOC:	// 添付書類
			m_Util.LineDataGet( data.data_edit, (*m_pSnKskData)->KsData.KSdocu, 15*2, 2 );	
			break;
		case DLG3BSW:	// 金融機関
			data.data_combo = (*m_pSnKskData)->KsData.KSbpsw;	
			break;
		case DLG3BNM:	// 銀行等名称
			data.data_edit = (*m_pSnKskData)->KsData.KSbank.Bname;
			break;
		case DLG3BKN:	// 銀行等区分
			data.data_combo = (*m_pSnKskData)->KsData.KSbank.Bkbn;
			break;
		case DLG3SNM:	// 本支店名称
			data.data_edit = (*m_pSnKskData)->KsData.KSbank.Sname;	
			break;
		case DLG3SKN:	// 本支店区分
			data.data_combo = (*m_pSnKskData)->KsData.KSbank.Skbn;	
			break;
		case DLG3DNM:	// 預金
			data.data_edit = (*m_pSnKskData)->KsData.KSbank.Dname;	
			break;
		case DLG3BAC:	// 口座番号
			data.data_edit = (*m_pSnKskData)->KsData.KSbank.Accnt;	
			break;
		case DLG3AC1:	// 口座番号１
			data.data_edit = (*m_pSnKskData)->KsData.KSpost.Acnt1;
			break;
		case DLG3AC2:	// 口座番号２
			data.data_edit = (*m_pSnKskData)->KsData.KSpost.Acnt2;
			break;
		case DLG3PNM:	// 郵便局名称
			data.data_edit = (*m_pSnKskData)->KsData.KSpost.Pname;
			break;
		}
		diag_setdata( IDC_ICSDIAGCTRL30, i, &data, CIcsdiagctrl );
	}
}

//-----------------------------------------------------------------------------
// データ表示
//-----------------------------------------------------------------------------
void CShinHjnKskEX::DispKsiHjnDigZi()
{
	DIAGRAM_DATA	data;
	char	buf[256] = {0};

	// 税理士署名
	diag_clear( IDC_ICSDIAGCTRL40, DLG4ZNM, TRUE, CIcsdiagctrl );
	// 税理士情報の画面表示は全て表示する（印刷情報と連動しない）
//	if( pSyzShin->OfSw ) {
	if( m_pShinInfo->isExistOffice ){
		m_Util.DiagOcxIniz( &data );
		::ZeroMemory( buf, sizeof( buf ));
		sprintf_s( buf, sizeof( buf ), "%-40.40s　%-40.40s", m_pShinInfo->pO_DATA->o_ofcnam, m_pShinInfo->pZ_DATA->z_zeinam);
		data.data_disp = buf;
		diag_setdata( IDC_ICSDIAGCTRL40, DLG4ZNM, &data, CIcsdiagctrl );
	}
}


//-----------------------------------------------------------------------------
// データセット
//-----------------------------------------------------------------------------
// 引数
// 
// 
// 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShinHjnKskEX::DataSetDig1( long index, short leng, DIAGRAM_DATA *data, int mode )
{
	if( index < 0 ){
		return 0;
	}
	else if( data == NULL ){
		return 0;
	}

	if( !mode ){
		char	Str[128];
		int		Length;

		int				tmpGengo = 0;
		unsigned char	tmpCnvWaYmd[4] = {0};

		switch( index ){
			case DLG1_TAX:	// 税務署長
				memset( Str, '\0', sizeof( Str ) );
				memmove( Str, (*m_pSnHeadData)->Sn_ZMSYO, 12 );
				Length = (int)data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_ZMSYO, '\0', 12 );
				memmove( (*m_pSnHeadData)->Sn_ZMSYO, data->data_edit, Length );
				if( Length ){
					char	name1[20], name2[20];
					memset( name1, '\0', sizeof( name1 ) );
					memset( name2, '\0', sizeof( name2 ) );
					memmove( name1, (*m_pSnHeadData)->Sn_ZMSYO, 12 );
					memmove( name2, Str, 12 );
					if( Esel_CompareTaxOffice( name1, name2 ) )	{
						memset( (*m_pSnHeadData)->Sn_TAXNO, '\0', 5 );
						// 税務署コードの取得
						if( *m_pShinInfo->pIsOpenEselect ){
							if( GetTaxationOffice() ){
								break;
							}
						}
					}
				}
				break;
			case DLG1_ZIP:	// 納税地〒
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_ZIP, '\0', 10 );
				memmove( (*m_pSnHeadData)->Sn_ZIP, data->data_edit, Length );
				break;
			case DLG1_AD1:	// 納税地１
				Length = (int) data->data_edit.GetLength();
				memset( &(*m_pSnHeadData)->Sn_NOZEI[0], '\0', 40 );
				memmove( &(*m_pSnHeadData)->Sn_NOZEI[0], data->data_edit, Length );
				break;
			case DLG1_AD2:	// 納税地２
				Length = (int) data->data_edit.GetLength();
				memset( &(*m_pSnHeadData)->Sn_NOZEI[40], '\0', 40 );
				memmove( &(*m_pSnHeadData)->Sn_NOZEI[40], data->data_edit, Length );
				break;
			case DLG1_TL1:	// 電話番号
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_TELNO1, '\0', 6 );
				memmove( (*m_pSnHeadData)->Sn_TELNO1, data->data_edit, Length );
				break;
			case DLG1_TL2:	// 電話番号
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_TELNO2, '\0', 4 );
				memmove( (*m_pSnHeadData)->Sn_TELNO2, data->data_edit, Length );
				break;
			case DLG1_TL3:	// 電話番号
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_TELNO3, '\0', 4 );
				memmove( (*m_pSnHeadData)->Sn_TELNO3, data->data_edit, Length );
				break;
			case DLG1_HKN:	// 法人名フリガナ
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_COFRI, '\0', 80 );
				memmove( (*m_pSnHeadData)->Sn_COFRI, data->data_edit, Length );
				break;
			case DLG1_HNM:	// 法人名
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_CONAM, '\0', 40 );
				memmove( (*m_pSnHeadData)->Sn_CONAM, data->data_edit, Length );
//				memmove( (*m_pSnHeadData)->Sn_CONAM, data->data_edit, 40 );
				break;
			case DLG1_DKN:	// 代表者名フリガナ
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_DIFRI, '\0', 80 );
				memmove( (*m_pSnHeadData)->Sn_DIFRI, data->data_edit, Length );
				break;
			case DLG1_DNM:	// 代表者名
				Length = (int) data->data_edit.GetLength();
				memset( (*m_pSnHeadData)->Sn_DIHYO, '\0', 40 );
				memmove( (*m_pSnHeadData)->Sn_DIHYO, data->data_edit, Length );
				break;
			case DLG1TDT:	// 提出年月日
				//(*m_pSnKskData)->KsData.KSdata[0] = ID_ICSSH_REIWA_GENGO;//令和固定
				//tmpGengo = ID_ICSSH_REIWA_GENGO;
				memmove( tmpCnvWaYmd, data->data_day, sizeof(tmpCnvWaYmd) );
				if ( ( tmpCnvWaYmd[0] != 0xff ) && ( tmpCnvWaYmd[1] != 0xff ) && ( tmpCnvWaYmd[2] != 0xff ) ) {
					(*m_pSnKskData)->KsData.KSdata[0] = ID_ICSSH_REIWA_GENGO;//令和固定
					tmpGengo = ID_ICSSH_REIWA_GENGO;
					if( m_Util.IsValidReiwaYmd(tmpGengo, tmpCnvWaYmd[0], tmpCnvWaYmd[1], tmpCnvWaYmd[2]) == false ){
						tmpGengo = 0;
						memset(tmpCnvWaYmd, '\0', sizeof(tmpCnvWaYmd));
					}
					memmove(&(*m_pSnKskData)->KsData.KSdata[1], tmpCnvWaYmd, 3);
				}
				break;
			case DLG1KDT:	// 更正決定通知書日付
//				(*m_pSnKskData)->KsData.KSktdt[0] = 4;
				//memmove( &(*m_pSnKskData)->KsData.KSktdt[0], data->data_day, 4 );
				if ( data->data_day[0] == 0 ) {
					::ZeroMemory ( ( *m_pSnKskData )->KsData.KSktdt, sizeof ( ( *m_pSnKskData )->KsData.KSktdt ) );
				}
				else if ( data->data_day[0] != -1 ) {
					memmove ( &( *m_pSnKskData )->KsData.KSktdt[0], data->data_day, 4 );
				}
				break;
			case DLG1HDT:	// 適用法令の区分
				(*m_pSnKskData)->KsData.KSthsw = data->data_combo;	
				break;
			case DLG1KSW:	// 申告、更正、決定
				(*m_pSnKskData)->KsData.KSktsw = data->data_combo;
				break;
		}
	}
	else{
		switch( index ){
			case DLG1_TAX:	// 税務署長
				::ZeroMemory( (*m_pSnHeadData)->Sn_ZMSYO, 12 );
				break;
			case DLG1_ZIP:	// 納税地〒
				::ZeroMemory( (*m_pSnHeadData)->Sn_ZIP, sizeof((*m_pSnHeadData)->Sn_ZIP));
				break;
			case DLG1_AD1:	// 納税地１
				::ZeroMemory( &(*m_pSnHeadData)->Sn_NOZEI[0], 40 );
				break;
			case DLG1_AD2:	// 納税地２
				::ZeroMemory( &(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
				break;
			case DLG1_TL1:	// 電話番号
				::ZeroMemory( (*m_pSnHeadData)->Sn_TELNO1, 6 );
				break;
			case DLG1_TL2:	// 電話番号
				::ZeroMemory( (*m_pSnHeadData)->Sn_TELNO2, 4 );
				break;
			case DLG1_TL3:	// 電話番号
				::ZeroMemory( (*m_pSnHeadData)->Sn_TELNO3, 4 );
				break;
			case DLG1_HKN:	// 法人名フリガナ
				::ZeroMemory( (*m_pSnHeadData)->Sn_COFRI, sizeof( (*m_pSnHeadData)->Sn_COFRI ));
				break;
			case DLG1_HNM:	// 法人名
				::ZeroMemory( (*m_pSnHeadData)->Sn_CONAM, 40 );
				break;
			case DLG1_DKN:	// 代表者名フリガナ
				::ZeroMemory( (*m_pSnHeadData)->Sn_DIFRI, sizeof( (*m_pSnHeadData)->Sn_DIFRI ));
				break;
			case DLG1_DNM:	// 代表者名
				::ZeroMemory( (*m_pSnHeadData)->Sn_DIHYO, 40 );
				break;
			case DLG1TDT:	// 提出年月日
				::ZeroMemory( (*m_pSnKskData)->KsData.KSdata, sizeof((*m_pSnKskData)->KsData.KSdata));
				break;
			case DLG1KDT:	// 更正決定通知書日付
				::ZeroMemory( (*m_pSnKskData)->KsData.KSktdt, sizeof((*m_pSnKskData)->KsData.KSktdt));	
				break;
		}
	}
	DispKsiHjnDig1();

	return 0;
}


//-----------------------------------------------------------------------------
// 税務署コードの取得
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CShinHjnKskEX::GetTaxationOffice()
{
	DIAGRAM_DATA	dgda;
	char	Number[20], SName[64], DName[64], TName[64];
	int		Length, Status, index;
	char	Str[256] = {0};

	// 税務署名
	memset( SName, '\0', sizeof( SName ) );
	memset( DName, '\0', sizeof( DName ) );
	memmove( SName, (*m_pSnHeadData)->Sn_ZMSYO, 12 );
	// 税務署番号
	memset( Number, '\0', sizeof( Number ) );
	memmove( Number, (*m_pSnHeadData)->Sn_TAXNO, 5 );
	// 都道府県名取得
	memset( TName, '\0', sizeof( TName ) );
	// 電子申告チェック
	if( (Number[0]&0xff) != 'F' ){
		// 漢字スペースカットで税務署名が有り、税務署番号が無い時のチェック
		Length = m_Util.MojiSearch( DName, SName, 12 );
		if( Length && (strlen(Number)==0) ){
			Status = Esel_TaxOfficeFromName( DName, Number, TName );
			if( Status == 0 ){
				memmove( (*m_pSnHeadData)->Sn_TAXNO, Number, 5 );		// 税務署コード取得
			}
			else{
				CString Mes;
				if( Status == -1 ){
					Mes  = _T("入力された税務署名に該当する税務署が見つかりませんでした。\n");
					Mes += _T("電子申告を行う場合は、正確な提出先税務署を設定する必要があります。\n\n");
					Mes += _T("税務署選択を行いますか？");
				}
				else{
					Mes  = _T("入力された税務署名と類似した税務署名が確認できました。\n");
					Mes += _T("電子申告を行う場合は、正確な提出先税務署を設定する必要があります。\n\n");
					Mes += _T("税務署選択を行いますか？");
				}
				Status = ICSMessageBox( Mes, MB_YESNO, 0 );
				if( Status == IDYES ){
					memset( m_pShinInfo->pEselect, '\0', sizeof(ESELECT) );
					m_pShinInfo->pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
					memmove( m_pShinInfo->pEselect->nm_office, DName, Length );
					if( Esel_SelectTaxOffice(m_pShinInfo->pEselect) == IDOK ){
						memmove( (*m_pSnHeadData)->Sn_TAXNO, m_pShinInfo->pEselect->cd_office, 5 );
						memset( Str, '\0', sizeof( Str ) );
						Length = (int)strlen( m_pShinInfo->pEselect->nm_office );
						memmove( Str, m_pShinInfo->pEselect->nm_office, Length );
						memmove( (*m_pSnHeadData)->Sn_ZMSYO, Str, 12 );
						// 税務署名
						memset( Str, '\0', sizeof( Str ) );
						Length = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_ZMSYO, 12 );
						memmove( Str, (*m_pSnHeadData)->Sn_ZMSYO, Length );
						index = DLG1_TAX;
						diag_clear( IDC_ICSDIAGCTRL10, index, TRUE, CIcsdiagctrl );
						m_Util.DiagOcxIniz( &dgda );
						dgda.data_edit = Str;
						m_dig1.OffControl( TRUE );
						diag_setdata( IDC_ICSDIAGCTRL10, index, &dgda, CIcsdiagctrl );
						m_dig1.OffControl( FALSE );
					}
					else{
						memset( (*m_pSnHeadData)->Sn_TAXNO, 'F', 5 );
					}
				}
				else{
					memset( (*m_pSnHeadData)->Sn_TAXNO, 'F', 5 );
				}
			}
		}
	}
	return( Status );
}


//-----------------------------------------------------------------------------
// データセット
//-----------------------------------------------------------------------------
// 引数	index	：
//		leng	：
//		data	：
//		mode	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShinHjnKskEX::DataSetDig2( long index, short leng, DIAGRAM_DATA *data, int mode )
{
	if( index < 0 ){
		return 0;
	}
	else if( data == NULL ){
		return 0;
	}

	int		lin, clm;
	char	val[MONY_BUF_SIZE] = {0};

	if( index < DLG2VKS ){
		lin = index-DLG2VSS;
		clm = 0;
	}
	else{
		lin = index-DLG2VKS;
		clm = 1;
	}

	KSIVAL*	pVal = &(*m_pSnKskData)->KsData.KSval[lin];
	if( !mode ){
		memmove( val, data->data_val, 6 );
		switch( lin ){
			case 0:
				m_Util.l_calq( (char *)val, (char *)val, 0 );		
				break;
			case 9: 
			case 13:
			case 15:
			case 16:
				m_Util.l_calq( (char *)val, (char *)val, 0x10 );	
				break;
		}
		if( !clm ){		// この請求前の金額
			if( leng ){
				memmove( pVal->KVsval, val, 6 );
				pVal->KVvflg |= 0x01;
			}
		}
		else{			// 更正の請求金額
			if( leng ){
				memmove( pVal->KVkval, val, 6 );
				pVal->KVvflg |= 0x02;
			}
		}
	}
	else{
		if( !clm ){		// この請求前の金額
			::ZeroMemory( pVal->KVsval, 6 );
			pVal->KVvflg &= 0xfe;
		}
		else{			// 更正の請求金額
			::ZeroMemory( pVal->KVkval, 6 );
			pVal->KVvflg &= 0xfd;
		}
	}
	(*m_pSnKskData)->CalqBillVal( (*m_pSnHeadData) );
	DispKsiHjnDig2();

	return 0;
}

//-----------------------------------------------------------------------------
// データセット
//-----------------------------------------------------------------------------
// 引数	index
//		leng
//		data
//		mode
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShinHjnKskEX::DataSetDig3(long index, short leng, DIAGRAM_DATA *data, int mode)
{
	if( index < 0 ){
		return 0;
	}
	else if( data == NULL ){
		return 0;
	}

	char	dbuf[1024];
	char	*buff;
	int		len;

	len = data->data_edit.GetLength();
	memset( dbuf, '\0', sizeof( dbuf ));
	buff = (char *)((LPCTSTR)(data->data_edit));
	memmove( dbuf, buff, len );

	if( !mode ){
		switch( index ){
			case DLG3REA:	// 更正の請求をする理由等
				::ZeroMemory( (*m_pSnKskData)->KsData.KSreas, sizeof( (*m_pSnKskData)->KsData.KSreas ));
				m_Util.LineDataSet( (*m_pSnKskData)->KsData.KSreas, dbuf, 40*2, 3 );
				break;
			case DLG3SDT:	// 修正申告提出年月日
//				(*m_pSnKskData)->KsData.KSstdt[0] = 4;
				if ( data->data_day[0] == 0 ) {
					::ZeroMemory ( ( *m_pSnKskData )->KsData.KSstdt, sizeof ( ( *m_pSnKskData )->KsData.KSstdt ) );
				}
				else if ( data->data_day[0] != -1 ) { 
					memmove ( &(*m_pSnKskData)->KsData.KSstdt[0], data->data_day, 4 );
				}
				break;
			case DLG3JDT:	// 更正決定通知書受理年月日
//				(*m_pSnKskData)->KsData.KSjrdt[0] = 4;
				if ( data->data_day[0] == 0 ) {
					::ZeroMemory ( ( *m_pSnKskData )->KsData.KSjrdt, sizeof ( ( *m_pSnKskData )->KsData.KSjrdt ) );
				}
				else if ( data->data_day[0] != -1 ) { 
					memmove ( &( *m_pSnKskData )->KsData.KSjrdt[0], data->data_day, 4 );
				}
				break;
			case DLG3DOC:	// 添付書類
				::ZeroMemory( (*m_pSnKskData)->KsData.KSdocu, sizeof( (*m_pSnKskData)->KsData.KSdocu ));
				m_Util.LineDataSet( (*m_pSnKskData)->KsData.KSdocu, dbuf, 15*2, 2 );
				break;
			case DLG3BSW:	// 金融機関
				(*m_pSnKskData)->KsData.KSbpsw = data->data_combo;
				break;
			case DLG3BNM:	// 銀行等名称
				memcpy( (*m_pSnKskData)->KsData.KSbank.Bname, dbuf, 20 );
				break;
			case DLG3BKN:	// 銀行等区分
				(*m_pSnKskData)->KsData.KSbank.Bkbn = data->data_combo;	
				break;
			case DLG3SNM:	// 本支店名称
				memcpy( (*m_pSnKskData)->KsData.KSbank.Sname, dbuf, 20 );	
				break;
			case DLG3SKN:	// 本支店区分
				(*m_pSnKskData)->KsData.KSbank.Skbn = data->data_combo;	
				break;
			case DLG3DNM:	// 預金
				memcpy( (*m_pSnKskData)->KsData.KSbank.Dname, dbuf, 12 );	
				break;
			case DLG3BAC:	// 口座番号
				memcpy( (*m_pSnKskData)->KsData.KSbank.Accnt, dbuf, 10 );	
				break;
			case DLG3AC1:	// 口座番号１
				memcpy( (*m_pSnKskData)->KsData.KSpost.Acnt1, dbuf, 5 );	
				break;
			case DLG3AC2:	// 口座番号２
				memcpy( (*m_pSnKskData)->KsData.KSpost.Acnt2, dbuf, 8 );	
				break;
			case DLG3PNM:	// 郵便局名称
				memcpy( (*m_pSnKskData)->KsData.KSpost.Pname, dbuf, 16 );
				break;
		}
	}
	else{
		switch( index ){
		case DLG3REA:	// 更正の請求をする理由等
			::ZeroMemory( (*m_pSnKskData)->KsData.KSreas, sizeof( (*m_pSnKskData)->KsData.KSreas ));
			break;
		case DLG3SDT:	// 修正申告提出年月日
			::ZeroMemory( (*m_pSnKskData)->KsData.KSstdt, sizeof( (*m_pSnKskData)->KsData.KSstdt ));	
			break;
		case DLG3JDT:	// 更正決定通知書受理年月日
			::ZeroMemory( (*m_pSnKskData)->KsData.KSjrdt, sizeof( (*m_pSnKskData)->KsData.KSjrdt ));
			break;
		case DLG3DOC:	// 添付書類
			::ZeroMemory( (*m_pSnKskData)->KsData.KSdocu, sizeof( (*m_pSnKskData)->KsData.KSdocu ));	
			break;
		case DLG3BNM:	// 銀行等名称
			::ZeroMemory( (*m_pSnKskData)->KsData.KSbank.Bname, sizeof( (*m_pSnKskData)->KsData.KSbank.Bname ));	
			break;
		case DLG3SNM:	// 本支店名称
			::ZeroMemory( (*m_pSnKskData)->KsData.KSbank.Sname, sizeof( (*m_pSnKskData)->KsData.KSbank.Sname ));
			break;
		case DLG3DNM:	// 預金
			::ZeroMemory( (*m_pSnKskData)->KsData.KSbank.Dname, sizeof( (*m_pSnKskData)->KsData.KSbank.Dname ));
			break;
		case DLG3BAC:	// 口座番号
			::ZeroMemory( (*m_pSnKskData)->KsData.KSbank.Accnt, sizeof( (*m_pSnKskData)->KsData.KSbank.Accnt ));
			break;
		case DLG3AC1:	// 口座番号１
			::ZeroMemory( (*m_pSnKskData)->KsData.KSpost.Acnt1, sizeof( (*m_pSnKskData)->KsData.KSpost.Acnt1 ));
			break;
		case DLG3AC2:	// 口座番号２
			::ZeroMemory( (*m_pSnKskData)->KsData.KSpost.Acnt2, sizeof( (*m_pSnKskData)->KsData.KSpost.Acnt2 ));
			break;
		case DLG3PNM:	// 郵便局名称
			::ZeroMemory( (*m_pSnKskData)->KsData.KSpost.Pname, sizeof( (*m_pSnKskData)->KsData.KSpost.Pname ));
			break;
		}
	}
	DispKsiHjnDig3();

	return 0;
}

//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index
//		flg
//		nChar
//		mode
//-----------------------------------------------------------------------------
int CShinHjnKskEX::MoveCurselDiag1( short index, UINT flg, short nChar, int mode )
{
	short	xpos = index;
	int		sw = 0, cmb = 0;

	char VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );

	if( flg&REPLY_STAY ){
		diag_setposition( IDC_ICSDIAGCTRL10, xpos, CIcsdiagctrl );	
		return 1;
	}

	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			nChar = VK_LEFT;
		}
		else{
			nChar = VK_RIGHT;
		}
	}
	else{
		if( !mode && (nChar!=VK_RETURN) ){
			switch( index ){
				case DLG1KSW:
					cmb = 1;
					break;
				case DLG1HDT:
					cmb = 1;	
					break;	// 適用法令の区分
			}
		}
	}

	switch( nChar ){
		case VK_RETURN:
		case VK_RIGHT: 
		case VK_DOWN:
		case VK_F3:
			flg = REPLY_NEXT;
			if( index == DLG1TDT ){
				xpos = DLG1_TAX;
				flg = 0;
			}
			if( index == DLG1_DNM ){
				xpos = DLG1HDT;
				flg = 0;
			}
			if( index == DLG1KSW ){
				flg = REPLY_STAY;
				if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){
					sw = 4;
				}
				else{
					sw = 2;
				}
			}
			break;
		case VK_LEFT:
		case VK_UP:
		case VK_F2:
			flg = REPLY_BACK;
			if( index == DLG1TDT ){
				flg = REPLY_STAY;
				sw = 3;
			}
			if( index == DLG1HDT ) {
				xpos = DLG1_DNM;
				flg = 0;
			}
			if( index == DLG1_TAX ) {
				xpos = DLG1TDT;
				flg = 0;
			}
			break;
	}
	if( cmb ){

		int ret = 1;
		switch( nChar ){
			// 矢印制御	
			case VK_UP:
			case VK_DOWN:
				m_dig1.ComboDropDown( index, TRUE );
				break;
			
			case VK_RETURN:
			case VK_RIGHT:
			case VK_F3:
			// 次項目
				if(index == DLG1HDT){// 適用法令の区分
					m_dig1.SetPosition(DLG1KDT);//更正決定通知書日付へ	
				}
				if(index == DLG1KSW){//申告、更正、決定
					if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//連動の場合
						m_dig3.SetPosition(DLG3REA);// 更正の請求をする理由等へ	
						ret = 3;
					}
					else{//非連動の場合
						m_dig2.SetPosition(DLG2VSS);//更正決定通知書日付へ	
						ret = 2;
					}
				}
				break;
			
			case VK_TAB:
			case VK_LEFT:
			case VK_F2:
			// 前項目へ
				if(index == DLG1HDT){// 適用法令の区分
					m_dig1.SetPosition(DLG1_DNM);//代表者名へ
				}
				if(index == DLG1KSW){//申告、更正、決定
					m_dig1.SetPosition(DLG1KDT);//更正決定通知書日付へ
				}
				break;
		}
		return ret;
	}

	if( sw == 2 ){
		diag_setposition( IDC_ICSDIAGCTRL20, DLG2VSS, CIcsdiagctrl );
		return 2;
	}
	else if( sw == 3 ){
		diag_setposition( IDC_ICSDIAGCTRL30, DLG3PNM, CIcsdiagctrl );
		return 3;
	}
	else if( sw == 4 ){
		diag_setposition( IDC_ICSDIAGCTRL30, DLG3REA, CIcsdiagctrl );
		return 3;
	}
	else{
		if( flg & REPLY_NEXT ){
			diag_setnext( IDC_ICSDIAGCTRL10, CIcsdiagctrl );
		}
		else if( flg & REPLY_BACK ){
			diag_setprev( IDC_ICSDIAGCTRL10, CIcsdiagctrl );
		}
		else{
			diag_setposition( IDC_ICSDIAGCTRL10, xpos, CIcsdiagctrl );
		}
	}

	return 1;
}


//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index
//		flg
//		nChar
//		mode
//-----------------------------------------------------------------------------
int CShinHjnKskEX::MoveCurselDiag2( short index, UINT flg, short nChar, int mode )
{
	short	xpos = index;
	int		sw = 0;

	char VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );

	if( flg&REPLY_STAY ){
		diag_setposition( IDC_ICSDIAGCTRL20, xpos, CIcsdiagctrl );
		return 2;
	}

	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			nChar = VK_UP;
		}
		else{
			nChar = VK_DOWN;
		}
	}

	int		lin, clm;
	if( index < DLG2VKS ){
		lin = index-DLG2VSS;
		clm = 0;
	}
	else{
		lin = index-DLG2VKS;	
		clm = 1;
	}

	switch( nChar ){
		case VK_RETURN:
		case VK_DOWN:
		case VK_F3:
			flg = REPLY_NEXT;
			if( index == DLG2VKE ){
				flg = REPLY_STAY;	
				sw = 3;
			}
			break;
		case VK_UP:
		case VK_F2:
			flg = REPLY_BACK;
			if( index == DLG2VSS ){
				flg = REPLY_STAY;
				sw = 1;
			}
			break;
		case VK_RIGHT:
			flg = REPLY_STAY;

			if( index == DLG2KSE ){
				diag_setposition( IDC_ICSDIAGCTRL20, DLG2MDS, CIcsdiagctrl );
				return 2;
			}
			else if(index == DLG2MDE ){
				diag_setposition( IDC_ICSDIAGCTRL20, DLG2LKS, CIcsdiagctrl );
				return 2;
			}
			else if(index == DLG2VKE ){
				flg = REPLY_STAY;
				sw = 3;
			}	
			else{
				if( !clm ){
					xpos += DLG2LIN;
				}
				else{
					xpos = xpos - DLG2LIN + 1;
				}
			}
			
			break;
		case VK_LEFT:
			flg = REPLY_STAY;
			if( index == DLG2VSS ){
				flg = REPLY_STAY;
				sw = 1;
			}
			else if( index == DLG2MDS ){
				diag_setposition( IDC_ICSDIAGCTRL20, DLG2KSE, CIcsdiagctrl );
				return 2;
			}
			else if(index == DLG2LKS ){
				diag_setposition( IDC_ICSDIAGCTRL20, DLG2MDE, CIcsdiagctrl );
				return 2;
			}
			else{
				if( clm ){
					xpos -= DLG2LIN;
				}
				else{
					xpos = xpos + DLG2LIN - 1;
				}
			}
			
			break;
	}

	if( sw == 1 ){
		diag_setposition( IDC_ICSDIAGCTRL10, DLG1KSW, CIcsdiagctrl );
		return 1;
	}
	else if( sw == 3 ){
		diag_setposition( IDC_ICSDIAGCTRL30, DLG3REA, CIcsdiagctrl );
		return 3;
	}
	else{
		if( flg & REPLY_NEXT ){
			diag_setnext( IDC_ICSDIAGCTRL20, CIcsdiagctrl );
		}
		else if( flg & REPLY_BACK ){
			diag_setprev( IDC_ICSDIAGCTRL20, CIcsdiagctrl );
		}
		else{
			diag_setposition( IDC_ICSDIAGCTRL20, xpos, CIcsdiagctrl );
		}
	}

	return 2;
}

//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index
//		flg
//		nChar
//		mode
//-----------------------------------------------------------------------------
int CShinHjnKskEX::MoveCurselDiag3( short index, UINT flg, short nChar, int mode )
{
	short	xpos = index;
	int		sw = 0, cmb = 0;

	char VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );

	if( flg&REPLY_STAY ){
		diag_setposition( IDC_ICSDIAGCTRL30, xpos, CIcsdiagctrl );
		return 3;
	}

	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			nChar = VK_LEFT;
		}
		else{
			nChar = VK_RIGHT;
		}
	}
	else{
		if( !mode){
			switch( index ){
				case DLG3BSW: case DLG3BKN: case DLG3SKN:
					cmb = 1;
					break;
			}
		}
	}

	switch( nChar ){
		case VK_RETURN:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_F3:
			flg = REPLY_NEXT;
			switch( index ){
			case DLG3PNM:
				flg = REPLY_STAY;
				sw = 1;
				break;
			}
			break;
		case VK_LEFT:
		case VK_UP:
		case VK_F2:
			flg = REPLY_BACK;
			switch( index ){
				case DLG3REA:
					flg = REPLY_STAY;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//連動の場合
						sw = 4;
					}
					else{//非連動の場合
						sw = 2;
					}
					break;
			}
			break;
	}
	if( cmb ){
		switch( nChar ){
			// 矢印制御	
			case VK_UP:
			case VK_DOWN:
				m_dig3.ComboDropDown( index, TRUE );		
				break;
			
			case VK_RETURN:
			case VK_RIGHT:
			case VK_F3:
				m_dig3.SetNextPosition();
				break;
			
			case VK_TAB:
			case VK_LEFT:
			case VK_F2:
			// 前項目へ
				m_dig3.SetPrevPosition();
				break;
		}
		return 3;
	}

	if( sw == 1 ){
		char	yymmdd[3]={0};
		(*m_pSnHeadData)->GetYmdData( ID_ICSSH_KESSAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
		if( m_pArith->l_jsgn(yymmdd, AMENDH, 3) >= 0 ){
			diag_setposition( IDC_ICSDIAGCTRL10, DLG1TDT, CIcsdiagctrl );
			return 1;
		}
		else{
			CRect	rc;
			this->GetClientRect( &rc );
			this->InvalidateRect( &rc );
			SetRedrawScroll( 0 );
		//	diag_setposition( IDC_ICSDIAGCTRL50, DLG0CMB, CIcsdiagctrl );
			return 5;
		}
	}
	else if( sw == 2 ){
		diag_setposition( IDC_ICSDIAGCTRL20, DLG2VKE, CIcsdiagctrl );
		return 2;
	}
	else if( sw == 4 ){
		diag_setposition( IDC_ICSDIAGCTRL10, DLG1KSW, CIcsdiagctrl );
		return 1;
	}
	else{
		if( flg & REPLY_NEXT ){
			diag_setnext( IDC_ICSDIAGCTRL30, CIcsdiagctrl );
		}
		else if( flg & REPLY_BACK ){
			diag_setprev( IDC_ICSDIAGCTRL30, CIcsdiagctrl );
		}
		else{
			diag_setposition( IDC_ICSDIAGCTRL30, xpos, CIcsdiagctrl );
		}
	}

	return 3;
}

//-----------------------------------------------------------------------------
// 初期化（データクリア）
//-----------------------------------------------------------------------------
void CShinHjnKskEX::ClearKskHjn()
{
	CString	str;
	int		ans;

	// 処理確認
	str = _T("更正の請求書のデータがクリアされます。\n");
	str +=_T("処理を継続しますか？");
	ans = ICSMessageBox( str, (MB_YESNO | MB_DEFBUTTON2) );
	if( ans == IDNO ){
		return;
	}

	(*m_pSnKskData)->ClearData();

	DispKsiHjn( 1 );
}

//-----------------------------------------------------------------------------
// 画面スクロール処理
//-----------------------------------------------------------------------------
// 引数	id		：
//		index	：
//-----------------------------------------------------------------------------
void CShinHjnKskEX::ChkScroll( WORD id, short index )
{
	RECT	drect, wrect;
	long	sz, cnt;
	int		bpos;
	unsigned int spos = 0;
	DIAGRAM_ATTRIBUTE	dattr;	

	((CWnd*)this)->GetWindowRect( &wrect );
	((CIcsdiagctrl*)GetDlgItem( id ))->GetRect( index, (LPUNKNOWN)&drect );
	sz = (drect.bottom - drect.top)*2;
	bpos = GetScrollPos( SB_VERT );
	cnt = (wrect.bottom-wrect.top) /2L;
	if( wrect.bottom <= (drect.bottom+sz) ) {	//現在のポジションが画面の下にある時
		cnt = (drect.bottom+sz) - cnt;
		spos = (int)cnt + bpos;
	}
	else if( wrect.top >= (drect.top-sz) ) {	//現在のポジションが画面の上にある時
		cnt -= (drect.top-sz);
		spos = bpos - (int)cnt;
	}
	if( spos ){
		SetRedrawScroll( spos );
	}
}


//-----------------------------------------------------------------------------
// ポジションセット
//-----------------------------------------------------------------------------
// 引数	IndexNo	：	セット先インデックス
//-----------------------------------------------------------------------------
void CShinHjnKskEX::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = DLG1TDT;
	}

	int index;
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL10 ) {
		if( (index = m_dig1.GetPosition()) != -1) {
			m_dig1.SetPosition(index);
			m_dig1.SetFocus();
			return;
		}
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL20 ) {
		if( (index = m_dig2.GetPosition()) != -1) {
			m_dig2.SetPosition(index);
			m_dig2.SetFocus();
			return;
		}
	}
	else if( m_curwnd == IDC_ICSDIAGCTRL30 ) {
		if( (index = m_dig3.GetPosition()) != -1) {
			m_dig3.SetPosition(index);
			m_dig3.SetFocus();
			return;
		}
	}

	m_dig2.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット 
	m_dig1.SetFocus();

	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	m_dig1.SetPosition( Index );

	//CRect	rc;
	//this->GetClientRect( &rc );
	//this->InvalidateRect( &rc );
	//SetRedrawScroll( 0 );
}


void CShinHjnKskEX::MoveDiagMNEdit()
{
	if( m_pParent && m_pParent->GetSafeHwnd() ){
		m_ctrl_mynum.MoveDiagMNEdit(HJNKSK_NUM_HOJIN1,FALSE);
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			m_ctrl_mynum.SetDisableColor( BC_GRAY );
		}
		else{
			m_ctrl_mynum.SetDisableColor( BC_WHITE );
		}
	}
}

//-----------------------------------------------------------------------------
// マイナンバーコントロールの表示を消す
//-----------------------------------------------------------------------------
void CShinHjnKskEX::HideMNEdit()
{
	m_edit_mynum1.EnableWindow( FALSE );
	m_edit_mynum2.EnableWindow( FALSE );
	m_edit_mynum3.EnableWindow( FALSE );
	m_edit_mynum4.EnableWindow( FALSE );
	m_edit_mynum1.ShowWindow( SW_HIDE );
	m_edit_mynum2.ShowWindow( SW_HIDE );
	m_edit_mynum3.ShowWindow( SW_HIDE );
	m_edit_mynum4.ShowWindow( SW_HIDE );
}


//-----------------------------------------------------------------------------
// 還付金融機関入力にフォーカスセット('16.12.26)
//-----------------------------------------------------------------------------
void CShinHjnKskEX::SetKanpuKinyuInput()
{
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	m_dig3.SetPosition( DLG3BNM );

	// 再描画
	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );

	ChkScroll( IDC_ICSDIAGCTRL30, DLG3BNM );
}

//-----------------------------------------------------------------------------
// 終了時のフォーカス移動 ('16.12.26)
//-----------------------------------------------------------------------------
void CShinHjnKskEX::MoveFocusInEnd()
{
	// データを確定させる為にフォーカス移動
	if( m_curwnd == IDC_ICSDIAGCTRL30 ){
		if( m_dig3.GetPosition() == DLG3BSW ){
			m_dig3.SetPosition( DLG3BNM );
		}
	}
}



BEGIN_EVENTSINK_MAP(CShinHjnKskEX, CSyzBaseDlg)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL10, 3, CShinHjnKskEX::TerminationIcsdiagctrl10, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL10, 1, CShinHjnKskEX::EditONIcsdiagctrl10, VTS_I2)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL10, 2, CShinHjnKskEX::EditOFFIcsdiagctrl10, VTS_I2)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL10, 8, CShinHjnKskEX::ComboSelIcsdiagctrl10, VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL20, 3, CShinHjnKskEX::TerminationIcsdiagctrl20, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL20, 1, CShinHjnKskEX::EditONIcsdiagctrl20, VTS_I2)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL20, 2, CShinHjnKskEX::EditOFFIcsdiagctrl20, VTS_I2)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL30, 3, CShinHjnKskEX::TerminationIcsdiagctrl30, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL30, 1, CShinHjnKskEX::EditONIcsdiagctrl30, VTS_I2)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL30, 2, CShinHjnKskEX::EditOFFIcsdiagctrl30, VTS_I2)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL30, 8, CShinHjnKskEX::ComboSelIcsdiagctrl30, VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL50, 8, CShinHjnKskEX::ComboSelIcsdiagctrl50, VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinHjnKskEX, IDC_ICSDIAGCTRL50, 3, CShinHjnKskEX::TerminationIcsdiagctrl50, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinHjnKskEX::TerminationIcsdiagctrl10(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}
	if( m_IsStartEnd ){
		return;
	}

	char	Str[128] = {0} , buff[64] = {0};
	int		Length;
	CString	ZipBf, AddBf;

	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.MoveDiagData( &xdata, idata );
	xdata.data_imgdata = NULL;

	m_dig1.OffControl( TRUE );
	switch( nChar ){
		case VK_RETURN:
			DataSetDig1( index, length, &xdata, 0 );
			break;
		case VK_DELETE:
			DataSetDig1( index, length, &xdata, -1 );
			flg = REPLY_STAY;
			break;
		case VK_HOME :
			if( index == DLG1_TAX ){
				if( *(m_pShinInfo->pIsOpenEselect) ){
					memset( m_pShinInfo->pEselect, '\0', sizeof(ESELECT) );
					m_pShinInfo->pEselect->cd_kind = ESELECT_KIND_ZEIMUSYO;
					if( ((*m_pSnHeadData)->Sn_TAXNO[0]&0xff) != 0x00 && ((*m_pSnHeadData)->Sn_TAXNO[0]&0xff) != 'F' )
										memmove( m_pShinInfo->pEselect->cd_office, (*m_pSnHeadData)->Sn_TAXNO, 5 );
					if( Esel_SelectTaxOffice( m_pShinInfo->pEselect ) == IDOK ){
						memmove( (*m_pSnHeadData)->Sn_TAXNO, m_pShinInfo->pEselect->cd_office, 5 );
						memset( Str, '\0', sizeof( Str ) );
						kspclr( Str, 6 );
						Length = (int)strlen( m_pShinInfo->pEselect->nm_office );
						memmove( Str, m_pShinInfo->pEselect->nm_office, Length );
						memmove( (*m_pSnHeadData)->Sn_ZMSYO, Str, 12 );
						// 税務署名
						memset( Str, '\0', sizeof( Str ) );
						Length = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_ZMSYO, 12 );
						memmove( Str, (*m_pSnHeadData)->Sn_ZMSYO, Length );
						index = DLG1_TAX;
						diag_clear( IDC_ICSDIAGCTRL10, index, TRUE, CIcsdiagctrl );
						m_Util.DiagOcxIniz( &xdata );
						xdata.data_edit = Str;
						m_dig1.OffControl( TRUE );
						diag_setdata( IDC_ICSDIAGCTRL10, index, &xdata, CIcsdiagctrl );
						m_dig1.OffControl( FALSE );
					}
				}
				DataSetDig1( index, length, &xdata, 0 );
			}
			if( index == DLG1_ZIP )	{	// 納税地〒
				diag_getdata( IDC_ICSDIAGCTRL10, index, &xdata, CIcsdiagctrl );
				int len = xdata.data_edit.GetLength();
				memset( buff, '\0', sizeof(buff) );
				memmove( buff, xdata.data_edit, len );
				memmove( (*m_pSnHeadData)->Sn_ZIP, buff, len );
				ZipBf =  (*m_pSnHeadData)->Sn_ZIP;
				if( ZipToAddress( ZipBf, AddBf ) == TRUE )	{
					// 納税地　住所(上)
					memset( Str, '\0', sizeof( Str ) );
					Length = AddBf.GetLength();
					memmove( Str, AddBf, Length );
					memmove( (*m_pSnHeadData)->Sn_NOZEI, Str, 40 );
					m_Util.DiagOcxIniz( &xdata );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , Str, 40 );
					xdata.data_edit = buff;
					m_dig1.SetData( DLG1_AD1, (struct IUnknown *)&xdata );
					// 納税地　郵便番号
					Length = ZipBf.GetLength();
					memset( Str, '\0', sizeof( Str ) );
					memmove( Str, ZipBf, Length );
					memmove( (*m_pSnHeadData)->Sn_ZIP, Str, 8 );
					m_Util.DiagOcxIniz( &xdata );
					xdata.data_edit = Str;
					m_dig1.SetData( DLG1_ZIP, (struct IUnknown *)&xdata );
				}
			}
			if( index == DLG1_AD1 )	{	// 納税地１
				ZipBf =  (*m_pSnHeadData)->Sn_ZIP;
				if( ZipToAddress( ZipBf, AddBf ) == TRUE )	{
					// 納税地　住所(上)
					memset( Str, '\0', sizeof( Str ) );
					Length = AddBf.GetLength();
					memmove( Str, AddBf, Length );
					memmove( (*m_pSnHeadData)->Sn_NOZEI, Str, 40 );
					m_Util.DiagOcxIniz( &xdata );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , Str, 40 );
					xdata.data_edit = buff;
					m_dig1.SetData( DLG1_AD1, (struct IUnknown *)&xdata );
				}
			}
			if( index == DLG1_AD2 )	{	// 納税地２
				ZipBf =  (*m_pSnHeadData)->Sn_ZIP;
				if( ZipToAddress( ZipBf, AddBf ) == TRUE )	{
					// 納税地　住所(下)
					memset( Str, '\0', sizeof( Str ) );
					Length = AddBf.GetLength();
					memmove( Str, AddBf, Length );
					memmove( &(*m_pSnHeadData)->Sn_NOZEI[40], Str, 40 );
					m_Util.DiagOcxIniz( &xdata );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , Str, 40 );
					xdata.data_edit = buff;
					m_dig1.SetData( DLG1_AD2, (struct IUnknown *)&xdata );
				}
			}
			break;
	}
	m_dig1.OffControl( FALSE );
	int nowdiag = MoveCurselDiag1( index, flg, nChar );
	int nowpos;	// 移動先または自身のポジション
	if( nowdiag == 1 ) {
		nowpos = m_dig1.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL10, nowpos);
	}
	else if( nowdiag == 2 ) {
		nowpos = m_dig2.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL20, nowpos);
	}
	else if( nowdiag == 3 ) {
		nowpos = m_dig3.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL30, nowpos);
	}
}

void CShinHjnKskEX::EditONIcsdiagctrl10(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}
	m_Clm = 0;
	m_Idx = index;

	m_curwnd = IDC_ICSDIAGCTRL10;
}

void CShinHjnKskEX::EditOFFIcsdiagctrl10(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}

	DIAGRAM_DATA	data;
	m_dig1.OffControl( TRUE );
	m_Util.DiagOcxIniz(&data);
	diag_getdata( IDC_ICSDIAGCTRL10, index, &data, CIcsdiagctrl );
	int	st = DataSetDig1( index, data.data_check, &data, 0 );

	//--->yoshida190814
	if (index != DLG1KDT) {//元号選択ダイアログ
		diag_deledit(IDC_ICSDIAGCTRL10, CIcsdiagctrl);
	}
	//<----

	m_dig1.OffControl( FALSE );
}

void CShinHjnKskEX::ComboSelIcsdiagctrl10(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}

	DIAGRAM_DATA	xdata;
	m_dig1.OffControl( TRUE );
	diag_deledit( IDC_ICSDIAGCTRL10, CIcsdiagctrl );
	diag_getdata( IDC_ICSDIAGCTRL10, index, &xdata, CIcsdiagctrl );
	DataSetDig1( index, 0, &xdata, 0 );
	diag_setposition( IDC_ICSDIAGCTRL10, index, CIcsdiagctrl );
	m_dig1.OffControl( FALSE );
}

void CShinHjnKskEX::TerminationIcsdiagctrl20(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}
	if( m_IsStartEnd ){
		return;
	}

	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.MoveDiagData( &xdata, idata );
	xdata.data_imgdata = NULL;

	m_dig2.OffControl( TRUE );
	switch( nChar ){
	case VK_RETURN:
		DataSetDig2( index, length, &xdata, 0 );
		break;
	case VK_DELETE:
		DataSetDig2( index, length, &xdata, -1 );
		flg = REPLY_STAY;
		break;
	}
	m_dig2.OffControl( FALSE );
	int nowdiag = MoveCurselDiag2( index, flg, nChar );
	int nowpos;	// 移動先または自身のポジション
	if( nowdiag == 1 ) {
		nowpos = m_dig1.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL10, nowpos);
	}
	else if( nowdiag == 2 ) {
		nowpos = m_dig2.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL20, nowpos);
	}
	else if( nowdiag == 3 ) {
		nowpos = m_dig3.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL30, nowpos);
	}
}

void CShinHjnKskEX::EditONIcsdiagctrl20(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}
	m_Clm = 1;
	m_Idx = index;

	m_curwnd = IDC_ICSDIAGCTRL20;

}

void CShinHjnKskEX::EditOFFIcsdiagctrl20(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}
	DIAGRAM_DATA	data;
	m_dig2.OffControl( TRUE );
	m_Util.DiagOcxIniz(&data);
	diag_getdata( IDC_ICSDIAGCTRL20, index, &data, CIcsdiagctrl );
	int	st = DataSetDig2( index, data.data_check, &data, 0 );
	diag_deledit( IDC_ICSDIAGCTRL20, CIcsdiagctrl );
	m_dig2.OffControl( FALSE );
}

void CShinHjnKskEX::TerminationIcsdiagctrl30(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}
	if( m_IsStartEnd ){
		return;
	}

	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.MoveDiagData( &xdata, idata );
	xdata.data_imgdata = NULL;

	m_dig3.OffControl( TRUE );
	switch( nChar ){
	case VK_RETURN:
		DataSetDig3( index, length, &xdata, 0 );
		break;
	case VK_DELETE:
		DataSetDig3( index, length, &xdata, -1 );
		flg = REPLY_STAY;
		break;
	}
	m_dig3.OffControl( FALSE );
	int nowdiag = MoveCurselDiag3( index, flg, nChar );
	int nowpos;	// 移動先または自身のポジション
	if( nowdiag == 1 ) {
		nowpos = m_dig1.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL10, nowpos);
	}
	else if( nowdiag == 2 ) {
		nowpos = m_dig2.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL20, nowpos);
	}
	else if( nowdiag == 3 ) {
		nowpos = m_dig3.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL30, nowpos);
	}
	else if( nowdiag == 5 ) {
		nowpos = m_dig5.GetPosition();
		ChkScroll(IDC_ICSDIAGCTRL50, nowpos);
	}
}

void CShinHjnKskEX::EditONIcsdiagctrl30(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}
	m_Clm = 2;
	m_Idx = index;

	m_curwnd = IDC_ICSDIAGCTRL30;

}

void CShinHjnKskEX::EditOFFIcsdiagctrl30(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}
	DIAGRAM_DATA	data;
	m_Util.DiagOcxIniz(&data);
	diag_getdata( IDC_ICSDIAGCTRL30, index, &data, CIcsdiagctrl );
	int	st = DataSetDig3( index, data.data_check, &data, 0 );
}

void CShinHjnKskEX::ComboSelIcsdiagctrl30(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_IsStartEnd ){
		return;
	}
	DIAGRAM_DATA	xdata;
	m_dig3.OffControl( TRUE );
	diag_deledit( IDC_ICSDIAGCTRL10, CIcsdiagctrl );
	diag_getdata( IDC_ICSDIAGCTRL30, index, &xdata, CIcsdiagctrl );
	DataSetDig3( index, 0, &xdata, 0 );
	diag_setposition( IDC_ICSDIAGCTRL30, index, CIcsdiagctrl );
	m_dig3.OffControl( FALSE );
}

void CShinHjnKskEX::ComboSelIcsdiagctrl50(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	return;

	/*
	//処理カット。
	if( m_IsStartEnd ){
		return;
	}
	DIAGRAM_DATA	dataCombo;
	diag_getdata( IDC_ICSDIAGCTRL50, DLG0CMB, &dataCombo, CIcsdiagctrl );
	(*m_pSnHeadData)->Sn_Syuusei &= 0xfb;
	if( dataCombo.data_combo == 1 ){
		(*m_pSnHeadData)->Sn_Syuusei |= 0x04;
	}
	CWnd*	p1	=	GetParent();
	CWnd*	p2	=	p1->GetParent();
	
	((CH31HyoView*)m_pParent)->ChengTabTitle( (*m_pSnHeadData)->Sn_Syuusei );
	if( (*m_pSnHeadData)->Sn_Syuusei&0x04 ) {
		CString	Mes;
		Mes  = _T("更正の申出書の入力は、更正の請求書と同じ画面を使用します。\n");
		Mes += _T("「請求」の表現は、「申出」に読み替えて入力ください。\n\n");
		Mes += _T("印刷は、更正の申出書に必要な項目のみを、更正の申出書に正しく印刷します。");
		ICSMessageBox( Mes, MB_OK, 0, 0, this  ); 
	}
	diag_setposition( IDC_ICSDIAGCTRL10, DLG1TDT, CIcsdiagctrl );
	*/
}

void CShinHjnKskEX::TerminationIcsdiagctrl50(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}
	if( m_IsStartEnd ){
		return;
	}

	//switch( index ){
	//		default		:	
	//			break;
	//}
}




BOOL CShinHjnKskEX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_F2 ){
			OnButtonF2();
			return TRUE;
		}

		if( pMsg->wParam == VK_END ){
			

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
			
		}

		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAGCTRL10 ) {
					TerminationIcsdiagctrl10( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				else if( m_curwnd == IDC_ICSDIAGCTRL20 ) {
					TerminationIcsdiagctrl20( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				else if( m_curwnd == IDC_ICSDIAGCTRL30 ) {
					TerminationIcsdiagctrl30( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}


		if( pMsg->wParam == VK_F7 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F7 );
			}
			return TRUE;
		}
		if( pMsg->wParam == VK_F8 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F8 );
			}
			return TRUE;
		}
		
		//コンボボックスにフォーカスがある場合の対応
		if( m_curwnd == IDC_ICSDIAGCTRL10 || m_curwnd == IDC_ICSDIAGCTRL20 || m_curwnd == IDC_ICSDIAGCTRL30) {
			if( pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_RETURN ){
				DIAGRAM_ATTRIBUTE        dattr;
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getattr( m_curwnd, result, &dattr, CIcsdiagctrl );
				
				if( dattr.attr_inpkind == 1 ) {  //コンボボックスにカーソルがある場合はリストを閉じる
					DIAGRAM_DATA   dd;
					m_Util.DiagOcxIniz(&dd);
					diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );

					diag_cbdrop( m_curwnd, result, FALSE, CIcsdiagctrl );
					if(m_curwnd == IDC_ICSDIAGCTRL10){
						TerminationIcsdiagctrl10( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
					}
					else if(m_curwnd == IDC_ICSDIAGCTRL20){
						TerminationIcsdiagctrl20( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
					}
					else if(m_curwnd == IDC_ICSDIAGCTRL30){
						TerminationIcsdiagctrl30( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
					}

					return TRUE;
				}

			}
		}


		// 元号選択にフォーカスがある場合の対応
		//--->yoshida190814
		if (m_curwnd == IDC_ICSDIAGCTRL10) {
			DIAGRAM_ATTRIBUTE dattr;
			short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();

			if (result == DLG1KDT) {
				//yoshida190814
				//元号選択箇所では左右キーを無効化
				if ((pMsg->wParam == VK_LEFT) || (pMsg->wParam == VK_RIGHT)) {
					return TRUE;
				}

				// 他の業務と統一するためカット
				// Backspaceの動作をDeleteと同じ動作にする
				//if (pMsg->wParam == VK_BACK) {
				//	pMsg->wParam = VK_DELETE;
				//}


			}
		}
		else if (m_curwnd == IDC_ICSDIAGCTRL30) {
			DIAGRAM_ATTRIBUTE dattr;
			short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();

			if (result == DLG3SDT || result == DLG3JDT) {
				//yoshida190814
				//元号選択箇所では左右キーを無効化
				if ((pMsg->wParam == VK_LEFT) || (pMsg->wParam == VK_RIGHT)) {
					return TRUE;
				}

				// 他の業務と統一するためカット
				// Backspaceの動作をDeleteと同じ動作にする
				//if (pMsg->wParam == VK_BACK) {
				//	pMsg->wParam = VK_DELETE;
				//}

			}
		}
		//<-----

	}


	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


void CShinHjnKskEX::OnButtonF2()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	switch( m_Clm ){
		case 0:	
			MoveCurselDiag1( m_Idx, 0, VK_LEFT, 1 );	
			break;
		case 1:	
			MoveCurselDiag2( m_Idx, 0, VK_UP, 1 );	
			break;
		case 2:
			MoveCurselDiag3( m_Idx, 0, VK_LEFT, 1 );
			break;
	}
}

void CShinHjnKskEX::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	MoveDiagMNEdit();

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CShinHjnKskEX::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	MoveDiagMNEdit();

	CSyzBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CShinHjnKskEX::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSyzBaseDlg::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	MoveDiagMNEdit();
}

void CShinHjnKskEX::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	MoveDiagMNEdit();
}
