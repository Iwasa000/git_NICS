// CKoujoSelect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "H31HyoView.h"
#include "CKoujoSelect.h"
#include "KoujoSelectIdx.h"


// CKoujoSelect ダイアログ

IMPLEMENT_DYNAMIC(CKoujoSelect, ICSDialog)

CKoujoSelect::CKoujoSelect(CWnd* pParent /*=nullptr*/)
	: ICSDialog(CKoujoSelect::IDD, pParent)
	, m_pParent ( pParent )
{
	m_selIndex = 0;
}

CKoujoSelect::~CKoujoSelect()
{
}

void CKoujoSelect::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_KselDiag);
}


BEGIN_MESSAGE_MAP(CKoujoSelect, ICSDialog)
	ON_BN_CLICKED(IDCANCEL, &CKoujoSelect::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CKoujoSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// CKoujoSelect メッセージ ハンドラー

//==========================================================
// 初期情報のセット
//----------------------------------------------------------
// 引数
//		pSnHeadData			:		ヘッダー情報
//		pSyzSyukei			:		集計クラス
//		pArith				:		倍長演算
//----------------------------------------------------------
// 返送値
//			0				:		正常終了
//			-1				:		エラー
//==========================================================
int CKoujoSelect::InitInfo ( CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, CArithEx *pArith )
{
	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}

	m_pSnHeadData = pSnHeadData;
	m_pSyzSyukei = pSyzSyukei;
	m_pArith = pArith;

	return 0;
}

//==========================================================
// 選択された項目の取得
//----------------------------------------------------------
// 引数
//			getId			:		項目ID
//----------------------------------------------------------
// 返送値
//			0				:		正常終了
//			-1				:		エラー
//==========================================================
int CKoujoSelect::GetSelectData ( int &getId )
{
	int id = 0;

	switch ( m_selIndex ) {
	case ID_KSELECT20:
		id = ID_F520;
		break;
	case ID_KSELECT21:
		id = ID_F521;
		break;
	case ID_KSELECT22:
		id = ID_F522;
		break;
	case ID_KSELECT23:
		id = ID_F523;
		break;
	case ID_KSELECT24:
		id = ID_F524;
		break;
	case ID_KSELECT25:
		id = ID_F525;
		break;
	case ID_KSELECT26:
		id = ID_F526;
		break;
	case ID_KSELECT27:
		id = ID_F527;
		break;
	case ID_KSELECT28:
		id = ID_F528;
		break;
	case ID_KSELECT29:
		id = ID_F529;
		break;
	case ID_KSELECT30:
		id = ID_F530;
		break;
	case ID_KSELECT31:
		id = ID_F531;
		break;
	case ID_KSELECT32:
		id = ID_F532;
		break;
	case ID_KSELECT33:
		id = ID_F533;
		break;
	case ID_KSELECT34:
		id = ID_F534;
		break;
	case ID_KSELECT35:
		id = ID_F535;
		break;
	case ID_KSELECT36:
		id = ID_F536;
		break;
	default:
		id = -1;
		break;
	}

	if ( id != -1 ) {
		getId = id;
	}
	else {
		return -1;
	}
	   
	return 0;
}

BOOL CKoujoSelect::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	InitView();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//==========================================================
// 画面初期化
//----------------------------------------------------------
// 返送値
//			0				:		正常終了
//			-1				:		エラー
//==========================================================
int CKoujoSelect::InitView()
{
	int id = 0, st = 0;
	int index = 0;

	InitDiagAttr();
	st = GetSelectID ( id );
	if ( ( st != -1 ) && ( id != 0 ) ) {
		index = id;
	}
	else {
		index = ID_KSELECT20;
	}

	m_KselDiag.SetPosition ( index );
	return 0;
}

//==========================================================
// ダイアグラム初期化
//----------------------------------------------------------
// 返送値
//			0				:		正常終了
//			-1				:		エラー
//==========================================================
int CKoujoSelect::InitDiagAttr()
{
	int cnt = 0;
	int Flg = 0;
	while ( 1 ) {
		if ( KSELECT[cnt].Index == 0 ) {
			break;
		}

		Flg = 0;
		if ( m_pSnHeadData->Sn_Sign4 & 0x01 ) {
			// 確定時は不可
			Flg = KSELECT[cnt].AllUnConnect;
		}
		else {
			if ( KSELECT[cnt].Index == ID_KSELECT20 ) {
				Flg = KSELECT[cnt].UnConnect;
			}
			else if ( CheckUriageWariai ( KSELECT[cnt].Index ) == 1 ) {
				Flg = KSELECT[cnt].UnConnect;
			}
			else {
				Flg = 0;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, KSELECT[cnt].Index, Flg, m_pSnHeadData->Sn_Sign4, KSELECT[cnt].Connect );
		cnt++;
	}

	return 0;
}

//==========================================================
// 売上割合チェック
//----------------------------------------------------------
// 引数
//			index			:		インデックス
//----------------------------------------------------------
// 返送値
//			0				:		75%未満
//			1				:		75%以上
//==========================================================
int CKoujoSelect::CheckUriageWariai ( short index )
{
	int ret = 0;

	int sw = 0;
	int id1 = 0, id2 = 0;

	unsigned short uWari = 0, tmpWari1 = 0, tmpWari2 = 0;

	switch ( index ) {
	case ID_KSELECT21:
		sw = 1;
		break;
	case ID_KSELECT22:
		id1 = DAIICHISHU;
		id2 = DAINISHU;
		break;
	case ID_KSELECT23:
		id1 = DAIICHISHU;
		id2 = DAISANSHU;
		break;
	case ID_KSELECT24:
		id1 = DAIICHISHU;
		id2 = DAIYONSHU;
		break;
	case ID_KSELECT25:
		id1 = DAIICHISHU;
		id2 = DAIGOSHU;
		break;
	case ID_KSELECT26:
		id1 = DAIICHISHU;
		id2 = DAIROKUSHU;
		break;
	case ID_KSELECT27:
		id1 = DAINISHU;
		id2 = DAISANSHU;
		break;
	case ID_KSELECT28:
		id1 = DAINISHU;
		id2 = DAIYONSHU;
		break;
	case ID_KSELECT29:
		id1 = DAINISHU;
		id2 = DAIGOSHU;
		break;
	case ID_KSELECT30:
		id1 = DAINISHU;
		id2 = DAIROKUSHU;
		break;
	case ID_KSELECT31:
		id1 = DAISANSHU;
		id2 = DAIYONSHU;
		break;
	case ID_KSELECT32:
		id1 = DAISANSHU;
		id2 = DAIGOSHU;
		break;
	case ID_KSELECT33:
		id1 = DAISANSHU;
		id2 = DAIROKUSHU;
		break;
	case ID_KSELECT34:
		id1 = DAIYONSHU;
		id2 = DAIGOSHU;
		break;
	case ID_KSELECT35:
		id1 = DAIYONSHU;
		id2 = DAIROKUSHU;
		break;
	case ID_KSELECT36:
		id1 = DAIGOSHU;
		id2 = DAIROKUSHU;
		break;
	default:
		break;
	}

	if ( sw ) {
		// 単一業種
		for ( int i = DAIICHISHU; i <= DAIROKUSHU; i++ ) {
			uWari = 0;
			GetUriageWariai ( i, &uWari );
			uWari /= 10;

			if ( uWari >= URIAGEWARIAI ) {
				ret = 1;
				break;
			}
		}
	}
	else if ( ( id1 != 0 ) && ( id2 != 0 ) ) {
		uWari = 0;

		tmpWari1 = 0;
		GetUriageWariai ( id1, &tmpWari1 );

		tmpWari2 = 0;
		GetUriageWariai ( id2, &tmpWari2 );

		if ( ( tmpWari1 != 0 ) && ( tmpWari2 != 0 ) ) {
			uWari = ( tmpWari1 / 10 ) + ( tmpWari2 / 10 );
			if ( uWari >= URIAGEWARIAI ) {
				ret = 1;
			}
		}
	}
	else {
		ret = 0;
	}

	return ret;
}

//==========================================================
// 売上割合取得
//----------------------------------------------------------
// 引数
//			id				:		事業種
//			pWari			:		取得した売上割合
//----------------------------------------------------------
// 返送値
//			0				:		正常終了
//			-1				:		エラー
//==========================================================
int CKoujoSelect::GetUriageWariai ( int id, unsigned short *pWari )
{
	ASSERT ( pWari );
	if ( pWari == NULL ) {
		return -1;
	}

	int ret = 0;
	CString tag, tmpTag;

	switch ( id ) {
	case DAIICHISHU:
		tag = _T( "CTD00130" );
		break;
	case DAINISHU:
		tag = _T( "CTD00200" );
		break;
	case DAISANSHU:
		tag = _T( "CTD00270" );
		break;
	case DAIYONSHU:
		tag = _T( "CTD00340" );
		break;
	case DAIGOSHU:
		tag = _T( "CTD00410" );
		break;
	case DAIROKUSHU:
		tag = _T( "CTD00480" );
		break;
	default:
		break;
	}

	*pWari = 0;
	if ( !tag.IsEmpty() ) {
		tmpTag.Empty();
		( ( CH31HyoView* )m_pParent )->GetPercentage ( tag, tmpTag, pWari );
		ret = 0;
	}
	else {
		ret = -1;
	}

	return ret;
}

//==========================================================
// ラジオボタンのセット
//----------------------------------------------------------
// 引数
//			index			:		インデックス
//==========================================================
void CKoujoSelect::SetRadioButton ( short index )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	m_KselDiag.DeleteInput();

	for ( int i = ID_KSELECT20; i <= ID_KSELECT36; i++ ) {
		if ( index != i ) {
			// ラジオボタンOFF
			DiagData.data_check = 0;
			m_KselDiag.SetData ( i, ( struct IUnknown* )&DiagData );
		}
	}

	DiagData.data_check = 1;
	m_KselDiag.SetData ( index, ( struct IUnknown* )&DiagData );

	m_selIndex = index;
}

BEGIN_EVENTSINK_MAP(CKoujoSelect, ICSDialog)
ON_EVENT(CKoujoSelect, IDC_ICSDIAG8CTRL1, 3, CKoujoSelect::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CKoujoSelect, IDC_ICSDIAG8CTRL1, 1, CKoujoSelect::EditONIcsdiag8ctrl1, VTS_I2)
END_EVENTSINK_MAP()


void CKoujoSelect::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	short wTerm = 0;
	char VK_FLG = 0x00;
	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
	if ( nChar == VK_TAB ) {
		if ( VK_FLG & 0x80 ) {
			wTerm = VK_TAB;
		}
		else {
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_TAB ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_DOWN ) ) {
		m_KselDiag.SetNextPosition();
	}
	else if ( ( wTerm == VK_LEFT ) || ( wTerm == VK_UP ) ) {
		m_KselDiag.SetPrevPosition();
	}
}


void CKoujoSelect::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if ( m_selIndex != index ) {
		SetRadioButton ( index );
		m_KselDiag.SetPosition ( index );
	}
}


void CKoujoSelect::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	OnCancel();
}


void CKoujoSelect::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int pos = 0;

	CString msg;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	pos = m_KselDiag.GetPosition();
	GetSelectMessage ( pos, msg );

	msg += _T( "\nを選択しますか？" );
	if ( ICSMessageBox ( msg, ( MB_YESNO | MB_DEFBUTTON2 ), 0, 0 ) == IDNO ) {
		return;
	}

	OnOK();
}

//==========================================================
// 選択された項目のメッセージ取得
//----------------------------------------------------------
// 引数
//			index			:		インデックス
//			msg				:		メッセージ
//----------------------------------------------------------
// 返送値
//			0				:		正常終了
//			-1				:		エラー
//==========================================================
int CKoujoSelect::GetSelectMessage ( short index, CString &msg )
{
	CString tmpMsg;

	switch ( index ) {
	case ID_KSELECT20:
		tmpMsg = _T( "原則計算を適用する場合" );
		break;
	case ID_KSELECT21:
		tmpMsg = _T( "特例計算を適用する場合\n1種類の事業で75%以上" );
		break;
	case ID_KSELECT22:
	case ID_KSELECT23:
	case ID_KSELECT24:
	case ID_KSELECT25:
	case ID_KSELECT26:
	case ID_KSELECT27:
	case ID_KSELECT28:
	case ID_KSELECT29:
	case ID_KSELECT30:
	case ID_KSELECT31:
	case ID_KSELECT32:
	case ID_KSELECT33:
	case ID_KSELECT34:
	case ID_KSELECT35:
	case ID_KSELECT36:
		tmpMsg = _T( "特例計算を適用する場合\n2種類の事業で75%以上" );
		if ( index == ID_KSELECT22 ) {
			tmpMsg += _T( "\n第一種事業及び第二種事業" );
		}
		else if ( index == ID_KSELECT23 ) {
			tmpMsg += _T( "\n第一種事業及び第三種事業" );
		}
		else if ( index == ID_KSELECT24 ) {
			tmpMsg += _T( "\n第一種事業及び第四種事業" );
		}
		else if ( index == ID_KSELECT25 ) {
			tmpMsg += _T( "\n第一種事業及び第五種事業" );
		}
		else if ( index == ID_KSELECT26 ) {
			tmpMsg += _T( "\n第一種事業及び第六種事業" );
		}
		else if ( index == ID_KSELECT27 ) {
			tmpMsg += _T( "\n第二種事業及び第三種事業" );
		}
		else if ( index == ID_KSELECT28 ) {
			tmpMsg += _T( "\n第二種事業及び第四種事業" );
		}
		else if ( index == ID_KSELECT29 ) {
			tmpMsg += _T( "\n第二種事業及び第五種事業" );
		}
		else if ( index == ID_KSELECT30 ) {
			tmpMsg += _T( "\n第二種事業及び第六種事業" );
		}
		else if ( index == ID_KSELECT31 ) {
			tmpMsg += _T( "\n第三種事業及び第四種事業" );
		}
		else if ( index == ID_KSELECT32 ) {
			tmpMsg += _T( "\n第三種事業及び第五種事業" );
		}
		else if ( index == ID_KSELECT33 ) {
			tmpMsg += _T( "\n第三種事業及び第六種事業" );
		}
		else if ( index == ID_KSELECT34 ) {
			tmpMsg += _T( "\n第四種事業及び第五種事業" );
		}
		else if ( index == ID_KSELECT35 ) {
			tmpMsg += _T( "\n第四種事業及び第六種事業" );
		}
		else if ( index == ID_KSELECT36 ) {
			tmpMsg += _T( "\n第五種事業及び第六種事業" );
		}
		break;
	default:
		break;
	}

	if ( tmpMsg.IsEmpty() ) {
		return -1;
	}
	else {
		msg = tmpMsg;
	}

	return 0;
}

//==========================================================
// 選択中の項目ID取得
//----------------------------------------------------------
// 引数
//			getID			:		項目ID
//----------------------------------------------------------
// 返送値
//			0				:		正常終了
//			-1				:		エラー
//==========================================================
int CKoujoSelect::GetSelectID ( int &getID )
{
	int ret = 0;
	int tmpIdx = 0;
	MoneyBasejagArray money;
	char buf[32] = { 0 };
	int code = 0, typeCnt = 0;
	bool errFlg = false;

	money = m_pSyzSyukei->GetShinkokuData ( _T( "35F000" ) );
	m_pArith->l_print ( buf, money[0][0].arith, _T( "SSS") );
	typeCnt = atoi ( buf );

	memset ( buf, '\0', sizeof ( buf ) );
	money = m_pSyzSyukei->GetShinkokuData ( _T( "362006" ) );
	m_pArith->l_print ( buf, money[0][0].arith, _T( "SSSS") );
	code = atoi ( buf );

	switch ( code ) {
	case ID_F521:
		tmpIdx = ID_KSELECT21;
		break;
	case ID_F522:
		tmpIdx = ID_KSELECT22;
		break;
	case ID_F523:
		tmpIdx = ID_KSELECT23;
		break;
	case ID_F524:
		tmpIdx = ID_KSELECT24;
		break;
	case ID_F525:
		tmpIdx = ID_KSELECT25;
		break;
	case ID_F526:
		tmpIdx = ID_KSELECT26;
		break;
	case ID_F527:
		tmpIdx = ID_KSELECT27;
		break;
	case ID_F528:
		tmpIdx = ID_KSELECT28;
		break;
	case ID_F529:
		tmpIdx = ID_KSELECT29;
		break;
	case ID_F530:
		tmpIdx = ID_KSELECT30;
		break;
	case ID_F531:
		tmpIdx = ID_KSELECT31;
		break;
	case ID_F532:
		tmpIdx = ID_KSELECT32;
		break;
	case ID_F533:
		tmpIdx = ID_KSELECT33;
		break;
	case ID_F534:
		tmpIdx = ID_KSELECT34;
		break;
	case ID_F535:
		tmpIdx = ID_KSELECT35;
		break;
	case ID_F536:
		tmpIdx = ID_KSELECT36;
		break;
	default:
		if ( typeCnt >= 2 ) {
			tmpIdx = ID_KSELECT20;
		}
		else {
			tmpIdx = -1;
		}
		break;
	}

	getID = 0;
	if ( tmpIdx != -1 ) {
		getID = tmpIdx;
	}
	else {
		return -1;
	}

	return 0;
}