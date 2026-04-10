// KpubDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "KpubDlg.h"


// CKpubDlg ダイアログ

IMPLEMENT_DYNAMIC(CKpubDlg, ICSDialog)

CKpubDlg::CKpubDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CKpubDlg::IDD, pParent)
	, m_pZm(NULL)
	, m_jgykcd(0)
	, m_knrkcd(0)
{

}

CKpubDlg::~CKpubDlg()
{
}

void CKpubDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_JIGYO, m_JigyoCmb);
	DDX_Control(pDX, IDC_COMBO_KANRI, m_KanriCmb);
}


BEGIN_MESSAGE_MAP(CKpubDlg, ICSDialog)
	ON_BN_CLICKED(IDOK, &CKpubDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKpubDlg メッセージ ハンドラ

BOOL CKpubDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	if( InitDisp() ){
		return FALSE;
	}

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*====================================================================================
	画面初期化
  ------------------------------------------------------------------------------------
	返送値	0	：	正常終了
			-1	：	エラー
  ====================================================================================*/
int CKpubDlg::InitDisp()
{
	// 事業費・管理費大分類名称の読込＆セット
	if( ReadJigyoKanriMeisyo() ){
		return -1;
	}

	return 0;
}

/*====================================================================================
	事業費・管理費大分類名称の読込
  ------------------------------------------------------------------------------------
	返送値	0	：	正常終了
			-1	：	エラー
  ====================================================================================*/
int CKpubDlg::ReadJigyoKanriMeisyo()
{
	ASSERT( m_pZm );

	if( m_pZm->pGkrec == NULL ){
		if( m_pZm->GokeiMeisyoRead() == -1 ){
			return -1;
		}
	}

	int	jgyidx=0, inijgyidx=0;
	int knridx=0, iniknridx=0;

	// 全出力を追加
	m_JigyoCmb.AddString( "全　　事　　業　　費" );
	m_JigyoCmb.SetItemData( jgyidx, 0xffffffff );
	if( m_jgykcd == 0xffffffff ){
		inijgyidx = jgyidx;
	}
	jgyidx++;

	m_KanriCmb.AddString( "全　　管　　理　　費" );
	m_KanriCmb.SetItemData( knridx, 0xffffffff );
	if( m_knrkcd == 0xffffffff ){
		iniknridx = knridx;
	}
	knridx++;

	CString str;

	for( int i=0; i<m_pZm->gkn_info->reccount; i++ ){
		if( !((m_pZm->pGkrec+i)->gkcod&0xffffff00) ){
			if( (m_pZm->pGkrec+i)->gkbcod == 0x09 ){		// 事業費
				str.Format( "%-24.24s", (m_pZm->pGkrec+i)->gknam );
				m_JigyoCmb.AddString( (LPCTSTR)str );
				m_JigyoCmb.SetItemData( jgyidx, (m_pZm->pGkrec+i)->gkcod );
				if( m_jgykcd == (m_pZm->pGkrec+i)->gkcod ){
					inijgyidx = jgyidx;
				}
				jgyidx++;
			}
			else if( (m_pZm->pGkrec+i)->gkbcod == 0x0a ){	// 管理費
				str.Format( "%-24.24s", (m_pZm->pGkrec+i)->gknam );
				m_KanriCmb.AddString( (LPCTSTR)str );
				m_KanriCmb.SetItemData( knridx, (m_pZm->pGkrec+i)->gkcod );
				if( m_knrkcd == (m_pZm->pGkrec+i)->gkcod ){
					iniknridx = knridx;
				}
				knridx++;
			}
			else if( (m_pZm->pGkrec+i)->gkbcod > 0x0a ){
				break;
			}
		}
	}

	m_JigyoCmb.SetCurSel( inijgyidx );
	m_KanriCmb.SetCurSel( iniknridx );

	if( jgyidx <= 2 ){
		m_JigyoCmb.EnableWindow( FALSE );
	}
	if( knridx <= 2 ){
		m_KanriCmb.EnableWindow( FALSE );
	}

	return 0;
}

/*====================================================================================
	選択事業費＆管理費の取得
  ------------------------------------------------------------------------------------
	引数	pJgykcd	：	事業費コード取得先
			pKnrkcd	：	管理費コード取得先
  ------------------------------------------------------------------------------------
	返送値	0		：	正常終了
			-1		：	エラー
  ====================================================================================*/
int CKpubDlg::GetSelJigyoKanri( unsigned long *pJgykcd, unsigned long *pKnrkcd )
{
	ASSERT( pJgykcd );
	ASSERT( pKnrkcd );

	*pJgykcd = m_jgykcd;
	*pKnrkcd = m_knrkcd;

	return 0;
}

/*====================================================================================
	初期カーソル科目を設定
  ------------------------------------------------------------------------------------
	引数	jgykcd	：	事業費コード
			knrkcd	：	管理費コード
  ====================================================================================*/
void CKpubDlg::SetInitKamoku( unsigned long jgykcd, unsigned long knrkcd )
{
	m_jgykcd = jgykcd;
	m_knrkcd = knrkcd;
}

void CKpubDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	// 事業費
	m_jgykcd = (long)m_JigyoCmb.GetItemData( m_JigyoCmb.GetCurSel() );
	// 管理費
	m_knrkcd = (long)m_KanriCmb.GetItemData( m_KanriCmb.GetCurSel() );

	ICSDialog::OnOK();
}

BOOL CKpubDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			PostMessage( WM_COMMAND, IDOK );
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
