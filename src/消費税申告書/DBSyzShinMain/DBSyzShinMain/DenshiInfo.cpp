// DenshiInfo.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "DenshiInfo.h"


// CDenshiInfo ダイアログ

IMPLEMENT_DYNAMIC(CDenshiInfo, ICSDialog)

CDenshiInfo::CDenshiInfo(CWnd* pParent /*=NULL*/)
	: ICSDialog(CDenshiInfo::IDD, pParent)
	, m_pSnHeadData(NULL)
{
	memset( m_Sn_DIADR, '\0', sizeof(m_Sn_DIADR) );
	memset( m_Sn_DIZIP, '\0', sizeof(m_Sn_DIZIP) );
	memset( m_Sn_DITELNO1, '\0', sizeof(m_Sn_DITELNO1) );
	memset( m_Sn_DITELNO2, '\0', sizeof(m_Sn_DITELNO2) );
	memset( m_Sn_DITELNO3, '\0', sizeof(m_Sn_DITELNO3) );
	memset( m_Sn_KEIRI, '\0', sizeof(m_Sn_KEIRI) );
}

CDenshiInfo::~CDenshiInfo()
{
}

void CDenshiInfo::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_denshi_diag1);
}


BEGIN_MESSAGE_MAP(CDenshiInfo, ICSDialog)
	ON_BN_CLICKED(IDCANCEL, &CDenshiInfo::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDenshiInfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CDenshiInfo メッセージ ハンドラ

BOOL CDenshiInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN )	{
		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 )	return TRUE;

		if( pMsg->wParam == VK_F5 ){
			c_set( IDOK );
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

void CDenshiInfo::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

void CDenshiInfo::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// データの保存
	WriteData();

	OnOK();
}

BEGIN_EVENTSINK_MAP(CDenshiInfo, ICSDialog)
	ON_EVENT(CDenshiInfo, IDC_ICSDIAG8CTRL1, 3, CDenshiInfo::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CDenshiInfo, IDC_ICSDIAG8CTRL1, 2, CDenshiInfo::EditOFFIcsdiag8ctrl1, VTS_I2)
END_EVENTSINK_MAP()

void CDenshiInfo::InitInfo( CSnHeadData *pSnHeadData )
{
	m_pSnHeadData = pSnHeadData;
}

void CDenshiInfo::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	char	buf[512] = {0} , buff[64] = {0};
	int		len = 0;
	CString	ZipBf, AddBf;
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

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){
		case ID_YUBIN:
			if( wTerm == VK_HOME ){
				m_denshi_diag1.GetData( ID_YUBIN, (struct IUnknown *)&DiagData );
				len = DiagData.data_edit.GetLength();
				memset( buf, '\0', sizeof(buf) );
				memmove( buf, DiagData.data_edit, len );
				memmove( m_Sn_DIZIP, buf, 8 );
				ZipBf =  m_Sn_DIZIP;
				if( ZipToAddress(ZipBf, AddBf) == TRUE ){
					//代表者住所　上段
					memset( buf, '\0', sizeof(buf) );
					len = AddBf.GetLength();
					memmove( buf, AddBf, len );
					memmove( m_Sn_DIADR, buf, 40 );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , buf, 40 );
					DiagData.data_edit = buff;
					m_denshi_diag1.SetData( ID_JUSYO1 , (struct IUnknown *)&DiagData );
					//郵便番号
					len = ZipBf.GetLength();
					memset( buf, '\0', sizeof(buf) );
					memmove( buf, ZipBf, len );
					memmove( m_Sn_DIZIP, buf, 8 );
					DiagData.data_edit = buf;
					m_denshi_diag1.SetData( ID_YUBIN, (struct IUnknown *)&DiagData );
					m_denshi_diag1.SetPosition( ID_JUSYO1 );
				}else{
					m_denshi_diag1.SetPosition( ID_YUBIN );
				}
			}
			break;
		case ID_JUSYO1:
			if( wTerm == VK_HOME ){
				ZipBf = m_Sn_DIZIP;
				if( ZipToAddress(ZipBf, AddBf) == TRUE ){
					memset( buf, '\0', sizeof(buf) );
					len = AddBf.GetLength();
					memmove( buf, AddBf, len );
					memmove( m_Sn_DIADR, buf, 40 );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , buf, 40 );
					DiagData.data_edit = buff;
					m_denshi_diag1.SetData( ID_JUSYO1, (struct IUnknown *)&DiagData );
				}
				m_denshi_diag1.SetPosition( ID_JUSYO1 );
			}
			break;
		case ID_JUSYO2:
			if( wTerm == VK_HOME ){
				ZipBf = m_Sn_DIZIP;
				if( ZipToAddress(ZipBf, AddBf) == TRUE ){
					memset( buf, '\0', sizeof(buf) );
					len = AddBf.GetLength();
					memmove( buf, AddBf, len );
					memmove( m_Sn_DIADR, buf, 40 );
					memset( buff, '\0', sizeof(buff) );
					memmove( buff , buf, 40 );
					DiagData.data_edit = buff;
					m_denshi_diag1.SetData( ID_JUSYO2, (struct IUnknown *)&DiagData );
				}
				m_denshi_diag1.SetPosition( ID_JUSYO2 );
			}
			break;
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT)|| (wTerm==VK_DOWN) ){
		switch( index ){
			case ID_YUBIN:
			case ID_JUSYO1:
			case ID_JUSYO2:
			case ID_DY_TEL1:
			case ID_DY_TEL2:
			case ID_DY_TEL3:
				m_denshi_diag1.SetNextPosition();
				break;
			case ID_KEIRI:
				GetDlgItem( IDOK )->SetFocus();
				break;
		}
	}

	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_UP) || (wTerm==VK_F2) ){
		switch( index ){
			case ID_YUBIN:
				GetDlgItem( IDCANCEL )->SetFocus();
				break;
			case ID_JUSYO1:
			case ID_JUSYO2:
			case ID_DY_TEL1:
			case ID_DY_TEL2:
			case ID_DY_TEL3:
			case ID_KEIRI:
				m_denshi_diag1.SetPrevPosition();
				break;
		}
	}
}

BOOL CDenshiInfo::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	ReadData();
	InitDisp();

	ICSDialog::OnInitDialogEX();

//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// データ読み込み
//-----------------------------------------------------------------------------
void CDenshiInfo::ReadData()
{
	memmove( m_Sn_DIADR, m_pSnHeadData->Sn_DIADR, sizeof(m_Sn_DIADR) );
	memmove( m_Sn_DIZIP, m_pSnHeadData->Sn_DIZIP, sizeof(m_Sn_DIZIP) );
	memmove( m_Sn_DITELNO1, m_pSnHeadData->Sn_DITELNO1, sizeof(m_Sn_DITELNO1) );
	memmove( m_Sn_DITELNO2, m_pSnHeadData->Sn_DITELNO2, sizeof(m_Sn_DITELNO2) );
	memmove( m_Sn_DITELNO3, m_pSnHeadData->Sn_DITELNO3, sizeof(m_Sn_DITELNO3) );
	memmove( m_Sn_KEIRI, m_pSnHeadData->Sn_KEIRI, sizeof(m_Sn_KEIRI) );
}

//-----------------------------------------------------------------------------
// データ書込み
//-----------------------------------------------------------------------------
void CDenshiInfo::WriteData()
{
	memmove( m_pSnHeadData->Sn_DIADR, m_Sn_DIADR, sizeof(m_Sn_DIADR) );
	memmove( m_pSnHeadData->Sn_DIZIP, m_Sn_DIZIP, sizeof(m_Sn_DIZIP) );
	memmove( m_pSnHeadData->Sn_DITELNO1, m_Sn_DITELNO1, sizeof(m_Sn_DITELNO1) );
	memmove( m_pSnHeadData->Sn_DITELNO2, m_Sn_DITELNO2, sizeof(m_Sn_DITELNO2) );
	memmove( m_pSnHeadData->Sn_DITELNO3, m_Sn_DITELNO3, sizeof(m_Sn_DITELNO3) );
	memmove( m_pSnHeadData->Sn_KEIRI, m_Sn_KEIRI, sizeof(m_Sn_KEIRI) );
}

//-----------------------------------------------------------------------------
// 初期表示
//-----------------------------------------------------------------------------
void CDenshiInfo::InitDisp()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char			buf[512] = {0};

	//== ANK混じり ==
	// 納税地　住所(上)
	m_denshi_diag1.GetAttribute( ID_JUSYO1, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_denshi_diag1.SetAttribute( ID_JUSYO1, (LPUNKNOWN)&DA, TRUE );
	// 納税地　住所(上)
	m_denshi_diag1.GetAttribute( ID_JUSYO2, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_denshi_diag1.SetAttribute( ID_JUSYO2, (LPUNKNOWN)&DA, TRUE );
	// 経理担当者
	m_denshi_diag1.GetAttribute( ID_KEIRI, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_denshi_diag1.SetAttribute( ID_KEIRI, (LPUNKNOWN)&DA, TRUE );

	// 納税地　郵便番号
	memset( buf, '\0', sizeof( buf ) );
	memmove( buf, m_Sn_DIZIP, 8 );
	DiagData.data_edit = buf;
	m_denshi_diag1.SetData( ID_YUBIN, (struct IUnknown *)&DiagData );
	// 納税地　住所(上)
	memset( buf, '\0', sizeof( buf ) );
	int len = m_Util.MixSpaceCutLength( (char *)m_Sn_DIADR, 40 );
	memmove( buf, m_Sn_DIADR, len );
	DiagData.data_edit = buf;
	m_denshi_diag1.SetData( ID_JUSYO1, (struct IUnknown *)&DiagData );
	// 納税地　住所(下)
	memset( buf, '\0', sizeof( buf ) );
	len = m_Util.MixSpaceCutLength( (char *)&m_Sn_DIADR[40], 40 );
	memmove( buf, &m_Sn_DIADR[40], len );
	DiagData.data_edit = buf;
	m_denshi_diag1.SetData( ID_JUSYO2, (struct IUnknown *)&DiagData );
	// 電話番号(市外)
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, m_Sn_DITELNO1, sizeof(m_Sn_DITELNO1) );
	DiagData.data_edit = buf;
	m_denshi_diag1.SetData( ID_DY_TEL1, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, m_Sn_DITELNO2, sizeof(m_Sn_DITELNO2) );
	DiagData.data_edit = buf;
	m_denshi_diag1.SetData( ID_DY_TEL2, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, m_Sn_DITELNO3, sizeof(m_Sn_DITELNO3) );
	DiagData.data_edit = buf;
	m_denshi_diag1.SetData( ID_DY_TEL3, (struct IUnknown *)&DiagData );
	// 経理担当者
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)m_Sn_KEIRI, sizeof(m_Sn_KEIRI) );
	memmove( buf, m_Sn_KEIRI, len );
	DiagData.data_edit = buf;
	m_denshi_diag1.SetData( ID_KEIRI, (struct IUnknown *)&DiagData );

	m_denshi_diag1.SetPosition( ID_YUBIN );
}

void CDenshiInfo::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	DiagData;
	char			buf[256] = {0};
	int				len = 0;
	char			tmpData = 0;

	// データの取得
	m_Util.DiagOcxIniz(&DiagData);
	m_denshi_diag1.GetData( index, (struct IUnknown *)&DiagData );

	switch( index ){
		case ID_YUBIN :
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, DiagData.data_edit, len );
			memmove( m_Sn_DIZIP, buf, 8 );
			break;
		case ID_JUSYO1 :
			memset( buf, '\0', sizeof( buf ) );
			len = DiagData.data_edit.GetLength();
			memmove( buf, DiagData.data_edit, len );
			memmove( m_Sn_DIADR, buf, 40 );
			break;
		case ID_JUSYO2 :
			memset( buf, '\0', sizeof( buf ) );
			len = DiagData.data_edit.GetLength();
			memmove( buf, DiagData.data_edit, len );
			memmove( &m_Sn_DIADR[40], buf, 40 );
			break;
		case ID_DY_TEL1 :
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, DiagData.data_edit, len );
			memmove( m_Sn_DITELNO1, buf, 6 );
			break;
		case ID_DY_TEL2 :
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, DiagData.data_edit, len );
			memmove( m_Sn_DITELNO2, buf, 4 );
			break;
		case ID_DY_TEL3 :
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, DiagData.data_edit, len );
			memmove( m_Sn_DITELNO3, buf, 4 );
			break;
		case ID_KEIRI :
			len = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, DiagData.data_edit, len );
			memmove( m_Sn_KEIRI, buf, 40 );
			break;
		default	:
			break;
	}
}
