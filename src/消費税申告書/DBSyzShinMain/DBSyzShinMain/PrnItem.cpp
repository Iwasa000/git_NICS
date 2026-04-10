// prnItem.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

//#include "MainFrm.h"
//#include "DBSyzShinDoc.h"
//#include "DBSyzShinView.h"

#include "PrnReportDlg.h"
#include "PrnItem.h"

// メインビューの財務クラスを参照
//CDBSyzShinView *CPrnItem::pSyzShin;

//static	int					SelSw = 0;	// 全選択／解除サイン

// CPrnItem ダイアログ

IMPLEMENT_DYNAMIC(CPrnItem, ICSDialog)

CPrnItem::CPrnItem(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnItem::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_SelSw(0)
{

}

CPrnItem::~CPrnItem()
{
}

void CPrnItem::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK50, m_Check50);
	DDX_Control(pDX, IDC_CHECK51, m_Check51);
	DDX_Control(pDX, ICS_CHECK52, m_Check52);
	DDX_Control(pDX, IDC_CHECK53, m_Check53);
	DDX_Control(pDX, IDC_CHECK54, m_Check54);
	DDX_Control(pDX, IDC_CHECK55, m_Check55);
	DDX_Control(pDX, IDC_CHECK56, m_Check56);
	DDX_Control(pDX, IDC_CHECK57, m_Check57);
	DDX_Control(pDX, IDC_CHECK58, m_Check58);
	DDX_Control(pDX, IDC_CHECK59, m_Check59);
	DDX_Control(pDX, IDC_CHECK60, m_Check60);
	DDX_Control(pDX, IDC_CHECK61, m_Check61);
	DDX_Control(pDX, IDC_CHECK90, m_Check90);
	DDX_Control(pDX, IDC_CHECK91, m_Check91);
	DDX_Control(pDX, IDC_CHECK70, m_Check70);
	DDX_Control(pDX, IDC_CHECK71, m_Check71);
	DDX_Control(pDX, IDC_CHECK72, m_Check72);
	DDX_Control(pDX, IDC_CHECK73, m_Check73);
	DDX_Control(pDX, IDC_CHECK80, m_Check80);
	DDX_Control(pDX, IDC_CHECK63, m_Check63);
	DDX_Control(pDX, IDC_CHECK74, m_Check74);
	DDX_Control(pDX, IDC_CHECK75, m_Check75);
	DDX_Control(pDX, IDC_CHECK62, m_Check62);
	DDX_Control(pDX, IDC_CHECK64, m_Check64);
	DDX_Control(pDX, IDC_CHECK65, m_Check65);
	DDX_Control(pDX, IDC_CHECK101, m_Check101);
	DDX_Control(pDX, IDC_CHECK66, m_Check66);
	DDX_Control(pDX, IDC_CHECK67, m_Check67);
	// 改良依頼 20-0352 修正 add -->
	DDX_Control(pDX, IDC_CHECK110, m_Check110);
	DDX_Control(pDX, IDC_CHECK111, m_Check111);
	DDX_Control(pDX, IDC_CHECK112, m_Check112);
	DDX_Control(pDX, IDC_CHECK113, m_Check113);
	// 改良依頼 20-0352 修正 add <--
	DDX_Control(pDX, IDC_CHECK68, m_Check68);
}


BEGIN_MESSAGE_MAP(CPrnItem, ICSDialog)
	ON_BN_CLICKED(IDC_CHECK50, &CPrnItem::OnBnClickedCheck50)
	ON_BN_CLICKED(IDC_CHECK51, &CPrnItem::OnBnClickedCheck51)
	ON_BN_CLICKED(ICS_CHECK52, &CPrnItem::OnBnClickedCheck52)
	ON_BN_CLICKED(IDC_CHECK53, &CPrnItem::OnBnClickedCheck53)
	ON_BN_CLICKED(IDC_CHECK54, &CPrnItem::OnBnClickedCheck54)
	ON_BN_CLICKED(IDC_CHECK55, &CPrnItem::OnBnClickedCheck55)
	ON_BN_CLICKED(IDC_CHECK56, &CPrnItem::OnBnClickedCheck56)
	ON_BN_CLICKED(IDC_CHECK57, &CPrnItem::OnBnClickedCheck57)
	ON_BN_CLICKED(IDC_CHECK58, &CPrnItem::OnBnClickedCheck58)
	ON_BN_CLICKED(IDC_CHECK59, &CPrnItem::OnBnClickedCheck59)
	ON_BN_CLICKED(IDC_CHECK60, &CPrnItem::OnBnClickedCheck60)
	ON_BN_CLICKED(IDC_CHECK61, &CPrnItem::OnBnClickedCheck61)
	ON_BN_CLICKED(IDC_CHECK90, &CPrnItem::OnBnClickedCheck90)
	ON_BN_CLICKED(IDC_CHECK91, &CPrnItem::OnBnClickedCheck91)
	ON_BN_CLICKED(IDC_CHECK70, &CPrnItem::OnBnClickedCheck70)
	ON_BN_CLICKED(IDC_CHECK71, &CPrnItem::OnBnClickedCheck71)
	ON_BN_CLICKED(IDC_CHECK72, &CPrnItem::OnBnClickedCheck72)
	ON_BN_CLICKED(IDC_CHECK73, &CPrnItem::OnBnClickedCheck73)
	ON_BN_CLICKED(IDC_CHECK80, &CPrnItem::OnBnClickedCheck80)
	ON_BN_CLICKED(IDC_BUTTON2, &CPrnItem::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrnItem::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CPrnItem::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK63, &CPrnItem::OnBnClickedCheck63)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK74, &CPrnItem::OnBnClickedCheck74)
	ON_BN_CLICKED(IDC_CHECK75, &CPrnItem::OnBnClickedCheck75)
	ON_BN_CLICKED(IDC_CHECK62, &CPrnItem::OnBnClickedCheck62)
	ON_BN_CLICKED(IDC_CHECK64, &CPrnItem::OnBnClickedCheck64)
	ON_BN_CLICKED(IDC_CHECK65, &CPrnItem::OnBnClickedCheck65)
	ON_BN_CLICKED(IDC_CHECK101, &CPrnItem::OnBnClickedCheck101)
	ON_BN_CLICKED(IDC_CHECK66, &CPrnItem::OnBnClickedCheck66)
	ON_BN_CLICKED(IDC_CHECK67, &CPrnItem::OnBnClickedCheck67)
// 改良依頼 20-0352 修正 add -->
	ON_BN_CLICKED(IDC_CHECK110, &CPrnItem::OnBnClickedCheck110)
	ON_BN_CLICKED(IDC_CHECK111, &CPrnItem::OnBnClickedCheck111)
	ON_BN_CLICKED(IDC_CHECK112, &CPrnItem::OnBnClickedCheck112)
	ON_BN_CLICKED(IDC_CHECK113, &CPrnItem::OnBnClickedCheck113)
// 改良依頼 20-0352 修正 add <--
ON_BN_CLICKED(IDC_CHECK68, &CPrnItem::OnBnClickedCheck68)
END_MESSAGE_MAP()


// CPrnItem メッセージ ハンドラ

BOOL CPrnItem::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	CDBSyzShinMainView	*pMainView = ( CDBSyzShinMainView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 0 ) );

//2016.02.23 INSERT START 暫定対応
			m_Check62.ShowWindow( SW_HIDE );						// 欄外メモ
//2016.02.23 INSERT END

	// TODO:  ここに初期化を追加してください
	// 個人を選択
//	pSyzShin->m_Kojin = (pSyzShin->Snh.Sn_KOJIN&0x01);									// 個人を選択時は選択可
//	if( (pSyzShin->Vol1.APNO&0xf0) == 0x10 )	pSyzShin->m_Kojin = 1;					// 個人業種マスターは選択可

//--> '17.03.28 INS START
	// 法人番号
	if( IsShowHojinNoChk() ){
		m_Check64.ShowWindow( SW_SHOW );
		m_Check64.EnableWindow( TRUE );
	}
	else{
		m_Check64.ShowWindow( SW_HIDE );
		m_Check64.EnableWindow( FALSE );
	}
//<-- '17.03.28 INS END


	// 申告書本表　出力項目設定
	if( m_pSnHeadData->Sn_TYPE&0x01 ){
		// ＯＣＲ用紙
		m_Check50.SetCheck( 1 );										// 課税期間、整理番号
		m_Check50.EnableWindow( FALSE );
		m_Check51.SetCheck( 1 );										// 中間納税額
		m_Check51.EnableWindow( FALSE );
		m_Check52.SetCheck( 1 );										// 申告区分（確定時のみ）
		m_Check52.EnableWindow( FALSE );
//--> '17.03.28 INS START
		if( IsShowHojinNoChk() ){										// 法人番号
			//m_Check64.SetCheck( 0 );
			m_Check64.SetCheck( 1 );
			m_Check64.EnableWindow( FALSE );
		}
//<-- '17.03.28 INS END
	}
	else{
		// ＯＣＲ印刷
		m_Check50.EnableWindow( TRUE );									// 課税期間、整理番号
		m_Check50.SetCheck( m_pPrintCmInfo->OP1 ? FALSE: TRUE );
		m_Check51.EnableWindow( TRUE );									// 中間納税額
		m_Check51.SetCheck( m_pPrintCmInfo->OPB ? FALSE: TRUE );
		m_Check52.EnableWindow( TRUE );									// 申告区分（確定時のみ）
		m_Check52.SetCheck( m_pPrintCmInfo->OPC ? FALSE: TRUE );
//--> '17.03.28 INS START
		if( IsShowHojinNoChk() ){										// 法人番号
			m_Check64.EnableWindow( TRUE );
			m_Check64.SetCheck( m_pPrintCmInfo->OPU ? 1 : 0 );
		}
//<-- '17.03.28 INS END
	}
	m_Check53.SetCheck( m_pPrintCmInfo->OP3 ? FALSE: TRUE );					// 納税地　住所・郵便番号
	m_Check54.SetCheck( m_pPrintCmInfo->OPE ? FALSE: TRUE );					// 納税地　電話番号
	m_Check55.SetCheck( m_pPrintCmInfo->OP4 ? FALSE: TRUE );					// 名称又は屋号
	m_Check56.SetCheck( m_pPrintCmInfo->OPF ? FALSE: TRUE );					// 名称又は屋号　フリガナ
	m_Check57.SetCheck( m_pPrintCmInfo->OP5 ? FALSE: TRUE );					// 代表者氏名又は氏名
	m_Check58.SetCheck( m_pPrintCmInfo->OPG ? FALSE: TRUE );					// 代表者氏名又は氏名　フリガナ
//-- '15.11.26 --
//	m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );					// 経理担当者氏名
//---------------
	// 経理担当者氏名
	if( m_pPrintCmInfo->Shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
		m_Check59.EnableWindow( FALSE );
		m_pPrintCmInfo->OP6 = 0;
	}
	else{
		m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );
	}
//---------------

	m_Check60.SetCheck( m_pPrintCmInfo->OPI ? TRUE: FALSE );					// 納付時　還付金融機関等
	if( m_pSnHeadData->IsSoleProprietor() ){
		m_Check61.SetCheck( 0 );										// 欄外印字項目
		m_Check61.EnableWindow( FALSE );
//2016.02.23 INSERT START
		//m_Check62.SetCheck( TRUE );										// 欄外メモ
		//m_Check62.SetCheck( m_pPrintCmInfo->OPT ? FALSE: TRUE );
//2016.02.23 INSERT END
	}
	else	{
		m_Check61.EnableWindow( TRUE );									// 欄外印字項目
		m_Check61.SetCheck( m_pPrintCmInfo->OP7 ? FALSE: TRUE );
//2016.02.23 INSERT START
		//m_Check62.SetCheck( 0 );										// 欄外メモ
		//m_Check62.EnableWindow( FALSE );
//2016.02.23 INSERT END
	}

//2018.03.13 INSERT START
	if( m_pSnHeadData->Sn_Syuusei&0x10 ){
		//m_Check65.EnableWindow( TRUE );									// 相続人等
		m_Check65.ShowWindow( SW_SHOW );
		m_Check65.SetCheck( m_pPrintCmInfo->OPV ? FALSE: TRUE );
	}else{
		m_Check65.SetCheck( 0 );
		//m_Check65.EnableWindow( FALSE );
		m_Check65.ShowWindow( SW_HIDE );
	}
//2018.03.13 INSERT END

	// 新型コロナウイルスによる申告・期限延長申請
//	if ( (  m_pZmSub->zvol->ss_ymd >= H31_SDAY ) && ( ( m_Util.IsBasicMst ( m_pZmSub->zvol ) ) || ( m_pSnHeadData->IsMiddleProvisional() ) ) ) {
	if ( (  m_pZmSub->zvol->ss_ymd >= H31_SDAY && m_pZmSub->zvol->ss_ymd < R05_SDAY ) &&
		 ( ( m_Util.IsBasicMst ( m_pZmSub->zvol ) ) || ( m_pSnHeadData->IsMiddleProvisional() ) ) ) {	// 20231117 決算期間の開始が令和6年1月1日以降の場合は非表示にする
		// 決算期間の期首が平成31年1月1日以降かつ個人業種または申告区分が予定申告
		m_Check66.ShowWindow ( SW_SHOW );
		long eymd = 0;
		if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
			eymd = m_pSnHeadData->Sn_MDAYE;
		}
		else {
			eymd = m_pSnHeadData->Sn_KDAYE;
		}
		if ( ( eymd >= ICS_SH_10PER_REFORM_DAY ) && ( ( m_pSnHeadData->Sn_Syuusei & 0x02 ) == 0x00 ) ) {
			// 課税期間の終了が2019/10/1以降かつ申告区分が更正の請求以外
			m_Check66.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x10 ) ? TRUE: FALSE );
			m_Check66.EnableWindow ( TRUE );
		}
		else {
			m_Check66.EnableWindow ( FALSE );
			m_Check66.SetCheck ( 0 );
			m_pSnHeadData->Sn_GenericSgn &= ~0x10;
		}
	}
	else {
		m_Check66.ShowWindow ( SW_HIDE );
		m_pSnHeadData->Sn_GenericSgn &= ~0x10;
	}

	bool showFlg = false;
	// 付表６様式選択
	long symd = 0, eymd = 0;
	if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
		symd = m_pSnHeadData->Sn_MDAYS;
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		symd = m_pSnHeadData->Sn_KDAYS;
		eymd = m_pSnHeadData->Sn_KDAYE;
	}

	if ( ( eymd >= ICS_SH_10PER_REFORM_DAY ) && ( symd < R03_SDAY ) ) {
		if ( ( m_pSnHeadData->IsSoleProprietor() ) && ( m_pSnHeadData->Sn_Syuusei & 0x10 ) ) {
			m_Check67.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x20 ) ? TRUE: FALSE );
			showFlg = true;
		}
		else {
			m_Check67.ShowWindow ( SW_HIDE );	
			//GetDlgItem ( IDC_STATIC_ITEM3 )->ShowWindow ( SW_HIDE );
			GetDlgItem ( IDC_STATIC_FHYO6 )->ShowWindow ( SW_HIDE );
		}
	}
	else {
		if ( symd >= R03_SDAY ) {
			m_pSnHeadData->Sn_GenericSgn &= ~0x20;
		}
		
		m_Check67.ShowWindow ( SW_HIDE );
		//GetDlgItem ( IDC_STATIC_ITEM3 )->ShowWindow ( SW_HIDE );
		GetDlgItem ( IDC_STATIC_FHYO6 )->ShowWindow ( SW_HIDE );
	}

	// 付表5 控除対象仕入税額を1項目のみ出力
	if ( ( pMainView->GetActViewVer() == ID_VER_SYZ31 ) && ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) ) {
		//----->('23.05.01 Add)	
		if ( ( eymd >= ICS_SH_INVOICE_DAY ) && ( m_pSnHeadData->m_DispTabSgn & 0x2000 ) ) { //２割特例計算を使用する
			m_Check68.SetCheck(FALSE);
			m_Check68.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_FHYO5)->ShowWindow(SW_HIDE);
		}
		else {
		//<-----
			m_Check68.SetCheck((m_pSnHeadData->Sn_GenericSgn & 0x40) ? TRUE : FALSE);
			showFlg = true;
		//----->('23.05.01 Add)
		}
		//<-----
	}
	else {
		m_Check68.SetCheck ( FALSE );
		m_Check68.ShowWindow ( SW_HIDE );
		GetDlgItem ( IDC_STATIC_FHYO5 )->ShowWindow ( SW_HIDE );
	}

	if ( showFlg ) {
		GetDlgItem ( IDC_STATIC_ITEM3 )->ShowWindow ( SW_SHOW );
	}
	else {
		GetDlgItem ( IDC_STATIC_ITEM3 )->ShowWindow ( SW_HIDE );
	}

	// 申告書（複写分兼控用）　出力項目設定
	m_Check90.EnableWindow( TRUE );										// 欄外に【控用】を印刷する
	m_Check90.SetCheck( m_pPrintCmInfo->OPD ? TRUE : FALSE );

	if( *m_pShinInfo->pProgress ){										// 電子申告済を印刷する（進捗管理）
		m_Check91.ShowWindow( SW_SHOW );
		if( m_pSnHeadData->Sn_Sign2 & 0x80 ){
			*m_pShinInfo->pFinishSw = 1;
		}
		else{
			*m_pShinInfo->pFinishSw = 0;
		}
		m_Check91.SetCheck( *m_pShinInfo->pFinishSw );
	}
	else{
		m_Check91.SetCheck( 0 );
		m_Check91.ShowWindow( SW_HIDE );
		m_pShinInfo->pFinishSw = 0;
		m_pSnHeadData->Sn_Sign2 &= 0x7f;
	}
	// 税理士欄　出力項目設定
	if( m_pShinInfo->pO_DATA->o_hojn&0x01 ){
		GetDlgItem(IDC_CHECK70)->SetWindowText("税理士法人名");
		GetDlgItem(IDC_CHECK71)->SetWindowText("税理士法人名を縮小して出力");
	}
	else{
		GetDlgItem(IDC_CHECK70)->SetWindowText("事務所名");
		GetDlgItem(IDC_CHECK71)->SetWindowText("事務所名を縮小して出力");
	}
//★//[13'02.12]///
//	m_Check70.SetCheck( pSyzShin->OPJ ? FALSE: TRUE );					// 事務所名
//	if( pSyzShin->Snd.Sn_Sign2&0x10 )	m_Check71.SetCheck( TRUE );		// 事務所名を圧縮で出力
//	else								m_Check71.SetCheck( FALSE );
//	m_Check72.SetCheck( pSyzShin->OPK ? FALSE: TRUE );					// 税理士名
//	m_Check73.SetCheck( pSyzShin->OPE ? FALSE: TRUE );					// 電話番号（本店)
///////////////////
//	if( !pSyzShin->OfSw ){
	if( !m_pShinInfo->isExistOffice ){
		m_pPrintCmInfo->OP2 = 1;
		m_pPrintCmInfo->OP9 = 1;
		m_Check70.SetCheck( FALSE );
		m_Check71.SetCheck( FALSE );
		m_Check72.SetCheck( FALSE );
		m_Check73.SetCheck( FALSE );
		m_Check70.EnableWindow( FALSE );
		m_Check71.EnableWindow( FALSE );
		m_Check72.EnableWindow( FALSE );
		m_Check73.EnableWindow( FALSE );
		GetDlgItem(IDC_STATIC_ITEM2)->EnableWindow( FALSE );
//--> '15.02.25 INS START
		m_Check74.EnableWindow( FALSE );
		m_Check75.EnableWindow( FALSE );
//<-- '15.02.25 INS END
	}
	else{
		if( !m_pPrintCmInfo->OP2 ){
			switch( m_pPrintCmInfo->OPA ) {
				case 1 :	
					m_Check70.SetCheck( TRUE );
					m_Check71.EnableWindow( TRUE );
					m_Check72.SetCheck( FALSE );
					if( m_pPrintCmInfo->OPsign&0x01 ){
						m_Check71.SetCheck( 1 );
					}
					else{
						m_Check71.SetCheck( 0 );
					}
					break;
				case 2 :	
					m_Check70.SetCheck( TRUE );
					m_Check71.EnableWindow( TRUE );
					m_Check72.SetCheck( TRUE );
					if( m_pPrintCmInfo->OPsign&0x01 ){
						m_Check71.SetCheck( 1 );
					}
					else{
						m_Check71.SetCheck( 0 );
					}
					break;
				default:
					m_Check70.SetCheck( FALSE );
					m_Check71.SetCheck( FALSE );
					m_Check71.EnableWindow( FALSE );
					m_Check72.SetCheck( TRUE );
					break;
			}
		}
		m_Check73.SetCheck( m_pPrintCmInfo->OP9 ? FALSE: TRUE );

//--> '15.02.25 INS START
		if( (m_pPrintCmInfo->zr_kbn==1) || (m_pPrintCmInfo->zr_kbn==0) ){
			m_Check74.EnableWindow( FALSE );
			m_Check74.ShowWindow( SW_HIDE );
			m_Check75.EnableWindow( FALSE );
			m_Check75.ShowWindow( SW_HIDE );
//--> '15.03.03 CUT START
//			m_pSnHeadData->m_ZroutSgn &= ~0x03;
//<-- '15.03.03 CUT END
		}
		else if( m_pPrintCmInfo->zr_kbn == 2 ){
//2017.07.06 INSERT START
			if( IsCompanyPackage() ){
				m_pSnHeadData->m_ZroutSgn &= ~0x03;
			}else{
//2017.07.06 INSERT END
				if( m_Check72.GetCheck() ){
					m_Check74.EnableWindow( TRUE );
					// チェックのON/OFF
					if( m_pSnHeadData->m_ZroutSgn&0x01 ){
						m_Check74.SetCheck( TRUE );
						m_Check75.EnableWindow( TRUE );
						if( m_pSnHeadData->m_ZroutSgn&0x02 ){
							m_Check75.SetCheck( TRUE );
						}
					}
					else{
						m_Check75.EnableWindow( FALSE );
						m_pSnHeadData->m_ZroutSgn &= ~0x02;
					}
				}
				else{
					m_Check74.EnableWindow( FALSE );
					m_Check75.EnableWindow( FALSE );
					m_pSnHeadData->m_ZroutSgn &= ~0x03;
				}
//2017.07.06 INSERT START
			}
//2017.07.06 INSERT END
		}
//<-- '15.02.25 INS END
	}
	SetJSMoede();

///////////////////
	// 個人各計算表　出力項目設定 //shimizu 課税取引のときに？復活
//-- '15.04.04 --
//	if( m_pSnHeadData->IsSoleProprietor() || m_pShinInfo->sgSpc ){
//		if( m_pShinInfo->sgSpc ){
//---------------
	if( m_pSnHeadData->IsSoleProprietor() || *m_pShinInfo->pSgSpc ){
		//2割特例適用時に特定収入計算表を表示しない様に条件追加。
		if ( !( m_pSnHeadData->m_DispTabSgn & 0x2000 ) ) {
			if (*m_pShinInfo->pSgSpc) {
				CString	Title;
				Title = _T("特定収入各計算表 出力項目設定");
				GetDlgItem(IDC_STATIC_ITEM20)->SetWindowText(Title);
				//2016.02.23 INSERT START
				m_Check62.ShowWindow(SW_HIDE);						// 欄外メモ
	//2016.02.23 INSERT END
			}
			else {
				m_Check61.ShowWindow(SW_HIDE);						// 欄外印字
	//2016.02.23 INSERT START
				//m_Check62.ShowWindow( SW_SHOW );						// 欄外メモ
				m_Check62.ShowWindow(SW_HIDE);						// 欄外メモ
	//2016.02.23 INSERT END
			}
			//m_Check80.SetCheck(m_pPrintCmInfo->OPL ? FALSE : TRUE);				// 会社名(氏名)を出力
		}
		else {
			GetDlgItem(IDC_STATIC_ITEM20)->ShowWindow(SW_HIDE);
			//m_Check80.ShowWindow(SW_HIDE);
		}
	}
	else{
		GetDlgItem( IDC_STATIC_ITEM20 )->ShowWindow( SW_HIDE );
		//m_Check80.ShowWindow( SW_HIDE );
	}
	m_Check80.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ITEM20)->ShowWindow(SW_HIDE);

	if( m_pPrintCmInfo->OPM ){
		m_Check63.SetCheck( 1 );
	}
	else{
		m_Check63.SetCheck( 0 );
	}

	// 元号出力
	// 付表5 控除対象仕入税額を1項目のみ出力のチェックボックスでもビューのバージョンを判定するため、
	// 関数のトップに移動
	//CDBSyzShinMainView	*pMainView = ( CDBSyzShinMainView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 0 ) );
	// 個人マスターの場合、改正バージョンにより平成/令和の出力を判定
	//if( m_Util.GetPrePrintGengo(m_pSnHeadData, true) == 2 ){
	if( m_Util.GetPrePrintGengo(m_pSnHeadData, pMainView->GetActViewVer(), true) == 2 ){
		m_Check101.ShowWindow(SW_SHOW);
		if( m_pSnHeadData->Sn_TYPE&0x01 ){
			m_Check101.EnableWindow(FALSE);
			m_Check101.SetCheck(1);
		}
		else{
			if(( eymd >= ICS_SH_R04_KOJINREFORM_DAY ) && ( m_pSnHeadData->IsSoleProprietor() )) {
				// 令和4年4月1日以降の課税期間かつ個人業種の場合、元号に〇をつけるは選択不可
				// 令和4年3月31日より前の課税期間でチェックを付けていた場合には、チェックを付けた状態で選択不可とする
				// 品質依頼分（修正依頼なし）
				m_Check101.EnableWindow( FALSE );
				m_Check101.SetCheck( m_pPrintCmInfo->OP_GENGO ? 1 : 0 );
			}
			else {
				m_Check101.EnableWindow(TRUE);
				m_Check101.SetCheck(m_pPrintCmInfo->OP_GENGO ? 1 : 0);
			}
		}
	}
	else{
		m_Check101.ShowWindow(SW_HIDE);
	}

// 改良依頼 20-0352 修正 add -->
	if ( pMainView->GetActViewVer() == ID_VER_SYZ31 ) {
		// 課税標準額等の内訳書
		// 代表者氏名 又は 氏名
		if(m_pPrintCmInfo->OPW & 0x01) {
			m_Check110.SetCheck(0);
		}
		else {
			m_Check110.SetCheck(1);
		}
		// 代表者氏名又は氏名 フリガナ
		if(m_pPrintCmInfo->OPW & 0x02) {
			m_Check111.SetCheck(0);
		}
		else {
			m_Check111.SetCheck(1);
		}
		if( m_pSnHeadData->Sn_Syuusei & 0x02 ){
			// 更正の請求
			// 代表者氏名 又は 氏名
			if(m_pPrintCmInfo->OPW & 0x04) {
				m_Check112.SetCheck(0);
			}
			else {
				m_Check112.SetCheck(1);
			}
			// 代表者氏名又は氏名 フリガナ
			if(m_pPrintCmInfo->OPW & 0x08) {
				m_Check113.SetCheck(0);
			}
			else {
				m_Check113.SetCheck(1);
			}
		}
		else {
			GetDlgItem ( IDC_STATIC_ITEM22 )->ShowWindow ( SW_HIDE );
			m_Check112.EnableWindow(FALSE);
			m_Check112.ShowWindow(SW_HIDE);
			m_Check113.EnableWindow(FALSE);
			m_Check113.ShowWindow(SW_HIDE);
		}
	}
	else {
		GetDlgItem ( IDC_STATIC_ITEM21 )->ShowWindow ( SW_HIDE );
		m_Check110.EnableWindow(FALSE);
		m_Check110.ShowWindow(SW_HIDE);
		m_Check111.EnableWindow(FALSE);
		m_Check111.ShowWindow(SW_HIDE);
		GetDlgItem ( IDC_STATIC_ITEM22 )->ShowWindow ( SW_HIDE );
		m_Check112.EnableWindow(FALSE);
		m_Check112.ShowWindow(SW_HIDE);
		m_Check113.EnableWindow(FALSE);
		m_Check113.ShowWindow(SW_HIDE);
	}
// 改良依頼 20-0352 修正 add <--

	if( m_pSnHeadData->IsSoleProprietor() ){
		CString	Mes;
		Mes  = _T("※国税庁から個人事業者の欄外プレプリント印字は記載不要との指導がありましたので、\n");
		Mes += _T("欄外プレプリント印字の出力には対応しておりません。");
		GetDlgItem(IDC_STATIC_INCOME)->SetWindowText( Mes );

		if (m_pSnHeadData->m_DispTabSgn & 0x2000) { //２割特例計算を使用する
			GetDlgItem(IDC_STATIC_INCOME)->SetWindowText(" ");
		}

	}
	m_SelSw = 0;

//2017.07.06 INSERT START
	if( IsCompanyPackage() ){
		GetDlgItem(IDC_STATIC_ITEM2)->ShowWindow(SW_HIDE);	//税理士欄 出力項目設定
		GetDlgItem(IDC_CHECK70)->ShowWindow(SW_HIDE);		//事務所名
		GetDlgItem(IDC_CHECK71)->ShowWindow(SW_HIDE);		//事務所名を縮小して出力
		GetDlgItem(IDC_CHECK72)->ShowWindow(SW_HIDE);		//税理士名
		GetDlgItem(IDC_CHECK73)->ShowWindow(SW_HIDE);		//電話番号
		GetDlgItem(IDC_CHECK74)->ShowWindow(SW_HIDE);		//税理士区分
		GetDlgItem(IDC_CHECK75)->ShowWindow(SW_HIDE);		//(直接受任)を付記する
	}
//2017.07.06 INSERT END

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CPrnItem::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->wParam == VK_F10 ) 
		return TRUE;
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 )	return TRUE;
		if( pMsg->wParam == VK_ESCAPE ) {
			CWnd*	p1	=	GetParent();
			CWnd*	p2	=	p1->GetParent();
			((CPrnReportDlg*)p2)->OnBnClickedCancel();
			return TRUE;
		}
		if( pMsg->wParam == VK_F9 ) {
			OnBnClickedButton1();
			return TRUE;
		}
		if( pMsg->wParam == VK_F11 ) {
			OnBnClickedButton2();
			return TRUE;
		}
		if( pMsg->wParam == VK_F12 ) {
			OnBnClickedButton3();
			return TRUE;
		}
		if( pMsg->wParam == VK_F5 ) {
			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F5);		
			return TRUE;
		}
// 依頼No.971
		if(pMsg->wParam == VK_END && m_swBeForked)	{
			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_END);
			return(TRUE);
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 初期情報セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		pZmSub		：	公益クラス
//		pShinInfo	：	共通情報
//		pPrintCmInfo：	印刷情報
//		pBeForked	：	1:一括印刷
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CPrnItem::InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

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

	ASSERT( pPrintCmInfo );
	if( pPrintCmInfo == NULL ){
		return -1;
	}
	m_pPrintCmInfo = pPrintCmInfo;

	m_swBeForked = pBeForked;

	return 0;
}


//////////////////////////////
// 申告書本表　出力項目設定 //
//////////////////////////////
// 課税期間、整理番号	▲ＯＣＲ印刷有効
void CPrnItem::OnBnClickedCheck50()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OP1 )	m_pPrintCmInfo->OP1 = 0;
	else				m_pPrintCmInfo->OP1 = 1;
}
// 中間納税額			▲ＯＣＲ印刷有効
void CPrnItem::OnBnClickedCheck51()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPB )	m_pPrintCmInfo->OPB = 0;
	else				m_pPrintCmInfo->OPB = 1;		
}
// 申告区分（確定時のみ）▲ＯＣＲ印刷有効	
void CPrnItem::OnBnClickedCheck52()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if(	m_pPrintCmInfo->OPC )	m_pPrintCmInfo->OPC = 0;
	else				m_pPrintCmInfo->OPC = 1;		
}
// 納税地　住所・郵便番号
void CPrnItem::OnBnClickedCheck53()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OP3 )	m_pPrintCmInfo->OP3 = 0;
	else				m_pPrintCmInfo->OP3 = 1;	
}
// 納税地　電話番号
void CPrnItem::OnBnClickedCheck54()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPE )	m_pPrintCmInfo->OPE = 0;
	else				m_pPrintCmInfo->OPE = 1;		
}
// 名称又は屋号
void CPrnItem::OnBnClickedCheck55()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OP4 )	m_pPrintCmInfo->OP4 = 0;
	else				m_pPrintCmInfo->OP4 = 1;	
}
// 名称又は屋号　フリガナ
void CPrnItem::OnBnClickedCheck56()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPF )	m_pPrintCmInfo->OPF = 0;
	else				m_pPrintCmInfo->OPF = 1;		
}
// 代表者氏名又は氏名
void CPrnItem::OnBnClickedCheck57()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OP5 )	m_pPrintCmInfo->OP5 = 0;
	else				m_pPrintCmInfo->OP5 = 1;	
}
// 代表者氏名又は氏名　フリガナ
void CPrnItem::OnBnClickedCheck58()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPG )	m_pPrintCmInfo->OPG = 0;
	else				m_pPrintCmInfo->OPG = 1;		
}
// 経理担当者　氏名
void CPrnItem::OnBnClickedCheck59()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OP6 )	m_pPrintCmInfo->OP6 = 0;
	else				m_pPrintCmInfo->OP6 = 1;	
}
// 納付日時　関布先金融機関等
void CPrnItem::OnBnClickedCheck60()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPI )	m_pPrintCmInfo->OPI = 0;
	else				m_pPrintCmInfo->OPI = 1;		
}
// 欄外印字項目			▲法人のみ
void CPrnItem::OnBnClickedCheck61()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OP7 )	m_pPrintCmInfo->OP7 = 0;
	else				m_pPrintCmInfo->OP7 = 1;		
}
//2016.02.23 INSERT START
void CPrnItem::OnBnClickedCheck62()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPT )	m_pPrintCmInfo->OPT = 0;
	else				m_pPrintCmInfo->OPT = 1;		
}
//2016.02.23 INSERT END
////////////////////////////////////////
// 申告書（複写分兼控用）出力項目設定 //
////////////////////////////////////////
// 欄外に【控用】を印刷する。		▲法人のみ（個人は無条件印刷）
void CPrnItem::OnBnClickedCheck90()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPD )	m_pPrintCmInfo->OPD = 0;
	else				m_pPrintCmInfo->OPD = 1;		
}
// 電子申告済を印刷する
void CPrnItem::OnBnClickedCheck91()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
//	pSyzShin->m_FinishSw = 0;	
	*m_pShinInfo->pFinishSw = 0;
	m_pSnHeadData->Sn_Sign2 &= 0x7f;
	if( m_Check91.GetCheck() )	{
//		pSyzShin->m_FinishSw = 1;
		*m_pShinInfo->pFinishSw = 1;
		m_pSnHeadData->Sn_Sign2 |= 0x80;
	}
}

////////////////////////////
// 税理士欄　出力項目設定 //
////////////////////////////
// 事務所名（税理士法人名）
void CPrnItem::OnBnClickedCheck70()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	chk70, chk72;
	chk70 = m_Check70.GetCheck();
	chk72 = m_Check72.GetCheck();
	m_pPrintCmInfo->OP2 = 1;
	if( chk70 || chk72 )	m_pPrintCmInfo->OP2 = 0;
	if( chk70 && chk72 )	m_pPrintCmInfo->OPA = 2;
	if( chk70 && !chk72 )	m_pPrintCmInfo->OPA = 1;
	if( !chk70 && chk72 )	m_pPrintCmInfo->OPA = 0;
	if( !chk70 && !chk72 )	m_pPrintCmInfo->OPA = 0;
	switch( m_pPrintCmInfo->OPA ) {
		case 1 :	m_Check70.SetCheck( TRUE );
					m_Check71.EnableWindow( TRUE );
					m_Check72.SetCheck( FALSE );
					if( m_pPrintCmInfo->OPsign&0x01 )	m_Check71.SetCheck( 1 );
					else						m_Check71.SetCheck( 0 );
					break;
		case 2 :	m_Check70.SetCheck( TRUE );
					m_Check71.SetCheck( FALSE );
					m_Check71.EnableWindow( TRUE );
					m_Check72.SetCheck( TRUE );
					if( m_pPrintCmInfo->OPsign&0x01 )	m_Check71.SetCheck( 1 );
					else						m_Check71.SetCheck( 0 );
					break;
		default:	m_Check70.SetCheck( FALSE );
					m_Check71.SetCheck( FALSE );
					m_Check71.EnableWindow( FALSE );
					m_pPrintCmInfo->OPsign &= 0xfe;
					if( chk72 )	m_Check72.SetCheck( TRUE );
					else		m_Check72.SetCheck( FALSE );
					break;
	}
	SetJSMoede();
}
// 事務所名を縮小して出力（税理士法人名を縮小して出力）
void CPrnItem::OnBnClickedCheck71()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_pPrintCmInfo->OPsign &= 0xfe;

	//---->yoshida210114
	if (m_Check71.GetCheck()) {
		m_pPrintCmInfo->OPsign |= 0x01;
		m_pSnHeadData->Sn_Sign2 |= 0x10;
	}
	else {
		m_pSnHeadData->Sn_Sign2 &= ~0x10;
	}
	//-----
	//if( m_Check71.GetCheck() ){		m_pPrintCmInfo->OPsign |= 0x01;
	//<-----
}
// 税理士名
void CPrnItem::OnBnClickedCheck72()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	chk70, chk72;
	chk70 = m_Check70.GetCheck();
	chk72 = m_Check72.GetCheck();
	m_pPrintCmInfo->OP2 = 1;
	if( chk70 || chk72 )	m_pPrintCmInfo->OP2 = 0;
	if( chk70 && chk72 )	m_pPrintCmInfo->OPA = 2;
	if( chk70 && !chk72 )	m_pPrintCmInfo->OPA = 1;
	if( !chk70 && chk72 )	m_pPrintCmInfo->OPA = 0;
	if( !chk70 && !chk72 )	m_pPrintCmInfo->OPA = 0;

	SetJSMoede();

//--> '15.02.25 INS START
	if( chk72 ){
		// ON
		m_Check74.EnableWindow( TRUE );
		if( m_pSnHeadData->m_ZroutSgn&0x01 ){
			m_Check74.SetCheck( TRUE );
			m_Check75.EnableWindow( TRUE );
			if( m_pSnHeadData->m_ZroutSgn&0x02 ){
				m_Check75.SetCheck( TRUE );
			}
		}
	}
	else{
		// 全OFF
		m_Check74.SetCheck( FALSE );
		m_Check74.EnableWindow( FALSE );
		m_Check75.SetCheck( FALSE );
		m_Check75.EnableWindow( FALSE );
		if( m_pPrintCmInfo->zr_kbn == 2 ){
			m_pSnHeadData->m_ZroutSgn &= ~0x03;
		}
	}
//<-- '15.02.25 INS END
}

void CPrnItem::SetJSMoede()
{
//2017.07.06 INSERT START
	if( IsCompanyPackage() ){
		return;
	}
//2017.07.06 INSERT END
	int	chk70, chk72;
	chk70 = m_Check70.GetCheck();
	chk72 = m_Check72.GetCheck();
	if( chk70 && chk72 ){
		// 10％本表では「事務所名」と「税理士名」にチェックがついている場合でも
		// 「事務所名を縮小して出力」を選択できるようにする
		CDBSyzShinMainView	*pMainView = (CDBSyzShinMainView*)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(0));
		if( pMainView->GetActViewVer() == ID_VER_SYZ26 ){
			m_pPrintCmInfo->OPsign &= 0xfe;
			m_Check71.SetCheck( FALSE );
			m_Check71.EnableWindow( FALSE );
		}
	}
	else{
		if( chk70 ){
			m_Check71.EnableWindow( TRUE );
		}
		else{
			m_Check71.EnableWindow( FALSE );
		}
	}
}
// 電話番号（本店）
void CPrnItem::OnBnClickedCheck73()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OP9 )	m_pPrintCmInfo->OP9 = 0;
	else						m_pPrintCmInfo->OP9 = 1;		
}

// 個人・非営利法人　各計算表の会社コードを出力
void CPrnItem::OnBnClickedCheck80()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPL )	m_pPrintCmInfo->OPL = 0;
	else				m_pPrintCmInfo->OPL = 1;		
}

void CPrnItem::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ChoiceItem();
}

void CPrnItem::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F11);
}

void CPrnItem::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F12);
}

// 全選択＆全解除
void CPrnItem::ChoiceItem( void )
{
// 改良依頼 20-0352 修正 add -->
	CDBSyzShinMainView	*pMainView = ( CDBSyzShinMainView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 0 ) );
// 改良依頼 20-0352 修正 add <--

	// 個人を選択
//	pSyzShin->m_Kojin = (pSyzShin->Snh.Sn_KOJIN&0x01);									// 個人を選択時は選択可
//	if( (pSyzShin->Vol1.APNO&0xf0) == 0x10 )	pSyzShin->m_Kojin = 1;					// 個人業種マスターは選択可

	if( m_SelSw ) {
		// 全選択解除
		m_SelSw = 0;

		// 申告書本表　出力項目設定
		if( !(m_pSnHeadData->Sn_TYPE&0x01) ) {
			// イメージ印刷
			m_pPrintCmInfo->OP1 = 1;
			m_Check50.EnableWindow( TRUE );									// 課税期間、整理番号
			m_Check50.SetCheck( m_pPrintCmInfo->OP1 ? FALSE: TRUE );
			m_pPrintCmInfo->OPB = 1;
			m_Check51.EnableWindow( TRUE );									// 中間納税額
			m_Check51.SetCheck( m_pPrintCmInfo->OPB ? FALSE: TRUE );
			m_pPrintCmInfo->OPC = 1;
			m_Check52.EnableWindow( TRUE );									// 申告区分（確定時のみ）
			m_Check52.SetCheck( m_pPrintCmInfo->OPC ? FALSE: TRUE );
//--> '17.03.29 INS START
			if( IsShowHojinNoChk() ){
				m_pPrintCmInfo->OPU = 0;
				m_Check64.EnableWindow( TRUE );								// 法人番号
				m_Check64.SetCheck( m_pPrintCmInfo->OPU ? 1 : 0 );
			}
//<-- '17.03.29 INS END
		}
		m_pPrintCmInfo->OP3 = 1;
		m_Check53.SetCheck( m_pPrintCmInfo->OP3 ? FALSE: TRUE );					// 納税地　住所・郵便番号
		m_pPrintCmInfo->OPE = 1;
		m_Check54.SetCheck( m_pPrintCmInfo->OPE ? FALSE: TRUE );					// 納税地　電話番号
		m_pPrintCmInfo->OP4 = 1;
		m_Check55.SetCheck( m_pPrintCmInfo->OP4 ? FALSE: TRUE );					// 名称又は屋号
		m_pPrintCmInfo->OPF = 1;
		m_Check56.SetCheck( m_pPrintCmInfo->OPF ? FALSE: TRUE );					// 名称又は屋号　フリガナ
		m_pPrintCmInfo->OP5 = 1;
		m_Check57.SetCheck( m_pPrintCmInfo->OP5 ? FALSE: TRUE );					// 代表者氏名又は氏名
		m_pPrintCmInfo->OPG = 1;
		m_Check58.SetCheck( m_pPrintCmInfo->OPG ? FALSE: TRUE );					// 代表者氏名又は氏名　フリガナ
//-- '15.11.26 --
//		m_pPrintCmInfo->OP6 = 1;
//		m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );
//---------------
		// 経理担当者氏名
		if( m_pPrintCmInfo->Shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			;
		}
		else{
			m_pPrintCmInfo->OP6 = 1;
			m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );					// 経理担当者氏名
		}
//---------------
		m_pPrintCmInfo->OPI = 0;
		m_Check60.SetCheck( m_pPrintCmInfo->OPI ? TRUE: FALSE );					// 納付時　還付金融機関等
		if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
			m_pPrintCmInfo->OP7 = 1;
			m_Check61.EnableWindow( TRUE );									// 欄外印字項目
			m_Check61.SetCheck( m_pPrintCmInfo->OP7 ? FALSE: TRUE );
		}
//2016.02.23 INSERT START
		//else{
		//	m_pPrintCmInfo->OPT = 1;
		//	m_Check62.EnableWindow( TRUE );									// 欄外印字項目
		//	m_Check62.SetCheck( m_pPrintCmInfo->OPT ? FALSE: TRUE );
		//}
//2016.02.23 INSERT END
//2018.03.13 INSERT START
		if( m_pSnHeadData->Sn_Syuusei&0x10 ){
			m_pPrintCmInfo->OPV = 1;										// 相続人等
			m_Check65.SetCheck( m_pPrintCmInfo->OPV ? FALSE: TRUE );
		}

		if ( ( m_Check66.IsWindowVisible() ) && ( m_Check66.IsWindowEnabled() ) ) {
			m_pSnHeadData->Sn_GenericSgn &= ~0x10;										// 申告・納付期限延長申請
			m_Check66.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x10 ) ? TRUE: FALSE );
		}

		// 付表６様式選択
		if ( m_Check67.IsWindowVisible() ) {
			m_pSnHeadData->Sn_GenericSgn &= ~0x20;
			m_Check67.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x20 ) ? TRUE: FALSE );
		}

		// 付表5 控除対象仕入税額を1項目のみ出力
		if ( m_Check68.IsWindowVisible() ) {
			m_pSnHeadData->Sn_GenericSgn &= ~0x40;
			m_Check68.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x40 ) ? TRUE: FALSE );
		}

//2018.03.13 INSERT END
		// 申告書（複写分兼控用）　出力項目設定
		m_pPrintCmInfo->OPD = 0;
		m_Check90.EnableWindow( TRUE );										// 欄外に【控用】を印刷する
		m_Check90.SetCheck( m_pPrintCmInfo->OPD ? TRUE : FALSE );
		if( *m_pShinInfo->pProgress ){										// 電子申告済を印刷する（進捗管理）	
			m_pSnHeadData->Sn_Sign2 &= 0x7f;
			if( m_pSnHeadData->Sn_Sign2 & 0x80 ){
				*m_pShinInfo->pFinishSw = 1;
			}
			else{
				*m_pShinInfo->pFinishSw = 0;
			}
			m_Check91.SetCheck( *m_pShinInfo->pFinishSw );
//			if( m_pSnHeadData->Sn_Sign2 & 0x80 )		pSyzShin->m_FinishSw = 1;
//			else									pSyzShin->m_FinishSw = 0;
//			m_Check91.SetCheck( pSyzShin->m_FinishSw );
		}
		
		// 税理士欄　出力項目設定
		if( m_pShinInfo->isExistOffice ){
			m_pPrintCmInfo->OP2 = 1;
			m_pPrintCmInfo->OPA = 1;
			m_pPrintCmInfo->OP9 = 1;
			m_pPrintCmInfo->OPsign &= 0xfe;
			m_Check70.SetCheck( FALSE );
			m_Check71.SetCheck( FALSE );
			m_Check72.SetCheck( FALSE );
			m_Check73.SetCheck( FALSE );
			m_Check71.EnableWindow( FALSE );
//--> '15.02.25 INS START
			if( m_pPrintCmInfo->zr_kbn == 2 ){
				m_Check74.EnableWindow( FALSE );
				m_Check75.EnableWindow( FALSE );
				m_Check74.SetCheck( FALSE );
				m_Check75.SetCheck( FALSE );
				m_pSnHeadData->m_ZroutSgn &= ~0x03;
			}
//<-- '15.02.25 INS END
		}

//		// 個人各計算表　出力項目設定
////-- '15.04.04 --
////		if( m_pSnHeadData->IsSoleProprietor() || m_pShinInfo->sgSpc ){
////---------------
//		if( m_pSnHeadData->IsSoleProprietor() || *m_pShinInfo->pSgSpc ){
////---------------
//			m_pPrintCmInfo->OPL = 1;
//			m_Check80.SetCheck( m_pPrintCmInfo->OPL ? FALSE: TRUE );				// 会社名(氏名)を出力
//		}
////★//[13'02.04]///

		m_pPrintCmInfo->OPM = 0;
		if( m_pPrintCmInfo->OPM )		m_Check63.SetCheck( 1 );
		else					m_Check63.SetCheck( 0 );
///////////////////

		if( m_Check101.IsWindowEnabled() ){
			m_pPrintCmInfo->OP_GENGO = 0;
			m_Check101.SetCheck(0);
		}

// 改良依頼 20-0352 修正 add -->
		if ( pMainView->GetActViewVer() == ID_VER_SYZ31 ) {
			m_pPrintCmInfo->OPW |= 0x01;
			m_Check110.SetCheck(FALSE);
			m_pPrintCmInfo->OPW |= 0x02;
			m_Check111.SetCheck(FALSE);
			// 更正の請求のチェックが有効
			if(m_Check112.IsWindowEnabled()) {
				m_pPrintCmInfo->OPW |= 0x04;
				m_Check112.SetCheck(FALSE);
			}
			if(m_Check113.IsWindowEnabled()) {
				m_pPrintCmInfo->OPW |= 0x08;
				m_Check113.SetCheck(FALSE);
			}
		}
// 改良依頼 20-0352 修正 add <--
	}
	else	{
		// 全選択
		m_SelSw = 1;

		// 申告書本表　出力項目設定
		if( !(m_pSnHeadData->Sn_TYPE&0x01) ) {
			// イメージ印刷
			m_pPrintCmInfo->OP1 = 0;
			m_Check50.EnableWindow( TRUE );									// 課税期間、整理番号
			m_Check50.SetCheck( m_pPrintCmInfo->OP1 ? FALSE: TRUE );
			m_pPrintCmInfo->OPB = 0;
			m_Check51.EnableWindow( TRUE );									// 中間納税額
			m_Check51.SetCheck( m_pPrintCmInfo->OPB ? FALSE: TRUE );
			m_pPrintCmInfo->OPC = 0;
			m_Check52.EnableWindow( TRUE );									// 申告区分（確定時のみ）
			m_Check52.SetCheck( m_pPrintCmInfo->OPC ? FALSE: TRUE );
//--> '17.03.29 INS START
			if( IsShowHojinNoChk() ){
				m_pPrintCmInfo->OPU = 1;
				m_Check64.EnableWindow( TRUE );								// 法人番号
				m_Check64.SetCheck( m_pPrintCmInfo->OPU ? 1 : 0 );
			}
//<-- '17.03.29 INS END
		}
		m_pPrintCmInfo->OP3 = 0;
		m_Check53.SetCheck( m_pPrintCmInfo->OP3 ? FALSE: TRUE );					// 納税地　住所・郵便番号
		m_pPrintCmInfo->OPE = 0;
		m_Check54.SetCheck( m_pPrintCmInfo->OPE ? FALSE: TRUE );					// 納税地　電話番号
		m_pPrintCmInfo->OP4 = 0;
		m_Check55.SetCheck( m_pPrintCmInfo->OP4 ? FALSE: TRUE );					// 名称又は屋号
		m_pPrintCmInfo->OPF = 0;
		m_Check56.SetCheck( m_pPrintCmInfo->OPF ? FALSE: TRUE );					// 名称又は屋号　フリガナ
		m_pPrintCmInfo->OP5 = 0;
		m_Check57.SetCheck( m_pPrintCmInfo->OP5 ? FALSE: TRUE );					// 代表者氏名又は氏名
		m_pPrintCmInfo->OPG = 0;
		m_Check58.SetCheck( m_pPrintCmInfo->OPG ? FALSE: TRUE );					// 代表者氏名又は氏名　フリガナ
//-- '15.11.26 --
//		m_pPrintCmInfo->OP6 = 0;
//		m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );					// 経理担当者氏名
//---------------
		// 経理担当者氏名
		if( m_pPrintCmInfo->Shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			;
		}
		else{
			m_pPrintCmInfo->OP6 = 0;
			m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );
		}
//---------------
		m_pPrintCmInfo->OPI = 1;
		m_Check60.SetCheck( m_pPrintCmInfo->OPI ? TRUE: FALSE );					// 納付時　還付金融機関等
		if( m_pSnHeadData->IsSoleProprietor() == FALSE ){
			m_pPrintCmInfo->OP7 = 0;
			m_Check61.EnableWindow( TRUE );									// 欄外印字項目
			m_Check61.SetCheck( m_pPrintCmInfo->OP7 ? FALSE: TRUE );
		}
//2016.02.23 INSERT START
		//else{
		//	m_pPrintCmInfo->OPT = 0;
		//	m_Check62.EnableWindow( TRUE );									// 欄外メモ
		//	m_Check62.SetCheck( m_pPrintCmInfo->OPT ? FALSE: TRUE );
		//}
//2018.03.13 INSERT START
		if( m_pSnHeadData->Sn_Syuusei&0x10 ){								// 相続人等
			m_pPrintCmInfo->OPV = 0;
			m_Check65.SetCheck( m_pPrintCmInfo->OPV ? FALSE: TRUE );
		}

		if ( ( m_Check66.IsWindowVisible() ) && ( m_Check66.IsWindowEnabled() ) ) {
			m_pSnHeadData->Sn_GenericSgn |= 0x10;									// 申告・納付期限延長申請
			m_Check66.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x10 ) ? TRUE: FALSE );
		}

		// 付表６様式選択
		if ( m_Check67.IsWindowVisible() ) {
			m_pSnHeadData->Sn_GenericSgn |= 0x20;
			m_Check67.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x20 ) ? TRUE: FALSE );
		}

		// 付表5 控除対象仕入税額を1項目のみ出力
		if ( m_Check68.IsWindowVisible() ) {
			m_pSnHeadData->Sn_GenericSgn |= 0x40;
			m_Check68.SetCheck ( ( m_pSnHeadData->Sn_GenericSgn & 0x40 ) ? TRUE: FALSE );
		}

//2018.03.13 INSERT END
//2016.02.23 INSERT END
		// 申告書（複写分兼控用）　出力項目設定
		m_pPrintCmInfo->OPD = 1;
		m_Check90.EnableWindow( TRUE );									// 欄外に【控用】を印刷する
		m_Check90.SetCheck( m_pPrintCmInfo->OPD ? TRUE : FALSE );

		if( *m_pShinInfo->pProgress ){	// 電子申告済を印刷する（進捗管理）
			m_pSnHeadData->Sn_Sign2 &= 0x7f;
			m_pSnHeadData->Sn_Sign2 |= 0x80;
			if( m_pSnHeadData->Sn_Sign2 & 0x80 )		*m_pShinInfo->pFinishSw = 1;
			else										*m_pShinInfo->pFinishSw = 0;
			m_Check91.SetCheck( *m_pShinInfo->pFinishSw );
		}
		// 税理士欄　出力項目設定
		if( m_pShinInfo->isExistOffice ){
			m_pPrintCmInfo->OP2 = 0;
			m_pPrintCmInfo->OPA = 2;
			m_pPrintCmInfo->OP9 = 0;
			m_pPrintCmInfo->OPsign |= 0x01;
			if( !m_pPrintCmInfo->OP2 )	{
				switch( m_pPrintCmInfo->OPA ) {
					case 1 :	m_Check70.SetCheck( TRUE );
								m_Check72.SetCheck( FALSE );
								break;
					case 2 :	m_Check70.SetCheck( TRUE );
								m_Check72.SetCheck( TRUE );
								break;
					default:	m_Check70.SetCheck( FALSE );
								m_Check72.SetCheck( TRUE );
								break;
				}
				m_Check73.SetCheck( m_pPrintCmInfo->OP9 ? FALSE: TRUE );
				m_Check71.EnableWindow( TRUE );
				m_Check71.SetCheck( TRUE );
			}
//--> '15.02.25 INS START
			if( m_pPrintCmInfo->zr_kbn == 2 ){
//2017.07.06 INSERT START
				if( IsCompanyPackage() ){
				}else{
//2017.07.06 INSERT END
					if( m_Check72.GetCheck() ){
						m_Check74.EnableWindow( TRUE );
						m_Check74.SetCheck( TRUE );
						m_Check75.EnableWindow( TRUE );
						m_Check75.SetCheck( TRUE );
						m_pSnHeadData->m_ZroutSgn |= 0x03;
					}
//2017.07.06 INSERT START
				}
//2017.07.06 INSERT END
			}
//<-- '15.02.25 INS END
		}

//		// 個人各計算表　出力項目設定
////-- '15.04.04 --
////		if( m_pSnHeadData->IsSoleProprietor() || m_pShinInfo->sgSpc ){
////---------------
//		if( m_pSnHeadData->IsSoleProprietor() || *m_pShinInfo->pSgSpc ){
////---------------
//			m_pPrintCmInfo->OPL = 0;
//			m_Check80.SetCheck( m_pPrintCmInfo->OPL ? FALSE: TRUE );				// 会社名(氏名)を出力
//		}
////★//[13'02.04]///

		m_pPrintCmInfo->OPM = 1;
		if( m_pPrintCmInfo->OPM )		m_Check63.SetCheck( 1 );
		else					m_Check63.SetCheck( 0 );
///////////////////

		if( m_Check101.IsWindowEnabled() ){
			m_pPrintCmInfo->OP_GENGO = 1;
			m_Check101.SetCheck(1);
		}

// 改良依頼 20-0352 修正 add -->
		if ( pMainView->GetActViewVer() == ID_VER_SYZ31 ) {
			m_pPrintCmInfo->OPW &= ~0x01;
			m_Check110.SetCheck(TRUE);
			m_pPrintCmInfo->OPW &= ~0x02;
			m_Check111.SetCheck(TRUE);
			// 更正の請求のチェックが有効
			if(m_Check112.IsWindowEnabled()) {
				m_pPrintCmInfo->OPW &= ~0x04;
				m_Check112.SetCheck(TRUE);
			}
			if(m_Check113.IsWindowEnabled()) {
				m_pPrintCmInfo->OPW &= ~0x08;
				m_Check113.SetCheck(TRUE);
			}
		}
// 改良依頼 20-0352 修正 add <--
	}
	SetJSMoede();
}

// 初期化
void CPrnItem::ItemInit( void )
{
	// 個人を選択
//	pSyzShin->m_Kojin = (pSyzShin->Snh.Sn_KOJIN&0x01);									// 個人を選択時は選択可
//	if( (pSyzShin->Vol1.APNO&0xf0) == 0x10 )	pSyzShin->m_Kojin = 1;					// 個人業種マスターは選択可

	// 申告書本表　出力項目設定
	if( m_pSnHeadData->Sn_TYPE&0x01 ) {
		// ＯＣＲ用紙
		m_Check50.SetCheck( 1 );										// 課税期間、整理番号
		m_Check50.EnableWindow( FALSE );
		m_Check51.SetCheck( 1 );										// 中間納税額
		m_Check51.EnableWindow( FALSE );
		m_Check52.SetCheck( 1 );										// 申告区分（確定時のみ）
		m_Check52.EnableWindow( FALSE );
//--> '17.03.29 INS START
		if( IsShowHojinNoChk() ){
			//m_Check64.SetCheck( 0 );									// 法人番号
			m_Check64.SetCheck( 1 );									// 法人番号
			m_Check64.EnableWindow( FALSE );
		}
//<-- '17.03.29 INS END
	}
	else	{
		// ＯＣＲ印刷
		m_Check50.EnableWindow( TRUE );									// 課税期間、整理番号
		m_Check50.SetCheck( m_pPrintCmInfo->OP1 ? FALSE: TRUE );
		m_Check51.EnableWindow( TRUE );									// 中間納税額
		m_Check51.SetCheck( m_pPrintCmInfo->OPB ? FALSE: TRUE );
		m_Check52.EnableWindow( TRUE );									// 申告区分（確定時のみ）
		m_Check52.SetCheck( m_pPrintCmInfo->OPC ? FALSE: TRUE );
//--> '17.03.29 INS START
		if( IsShowHojinNoChk() ){
			m_Check64.EnableWindow( TRUE );								// 法人番号
			m_Check64.SetCheck( m_pPrintCmInfo->OPU ? 1 : 0 );
		}
//<-- '17.03.29 INS END
	}

	m_Check53.SetCheck( m_pPrintCmInfo->OP3 ? FALSE: TRUE );					// 納税地　住所・郵便番号
	m_Check54.SetCheck( m_pPrintCmInfo->OPE ? FALSE: TRUE );					// 納税地　電話番号
	m_Check55.SetCheck( m_pPrintCmInfo->OP4 ? FALSE: TRUE );					// 名称又は屋号
	m_Check56.SetCheck( m_pPrintCmInfo->OPF ? FALSE: TRUE );					// 名称又は屋号　フリガナ
	m_Check57.SetCheck( m_pPrintCmInfo->OP5 ? FALSE: TRUE );					// 代表者氏名又は氏名
	m_Check58.SetCheck( m_pPrintCmInfo->OPG ? FALSE: TRUE );					// 代表者氏名又は氏名　フリガナ
//-- '15.11.26 --
//	m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );					// 経理担当者氏名
//---------------
	// 経理担当者氏名
	if( m_pPrintCmInfo->Shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
		m_Check59.EnableWindow( FALSE );
		m_pPrintCmInfo->OP6 = 0;
	}
	else{
		m_Check59.SetCheck( m_pPrintCmInfo->OP6 ? FALSE: TRUE );				
	}
//---------------
	m_Check60.SetCheck( m_pPrintCmInfo->OPI ? TRUE: FALSE );					// 納付時　還付金融機関等
	if( m_pSnHeadData->IsSoleProprietor() ){
		m_Check61.SetCheck( 0 );										// 欄外印字項目
		m_Check61.EnableWindow( FALSE );
//2016.02.23 INSERT START
		//m_Check62.EnableWindow( TRUE );									// 欄外印字項目
		//m_Check62.SetCheck( m_pPrintCmInfo->OPT ? FALSE: TRUE );
//2016.02.23 INSERT END
	}
	else	{
		m_Check61.EnableWindow( TRUE );									// 欄外印字項目
		m_Check61.SetCheck( m_pPrintCmInfo->OP7 ? FALSE: TRUE );
//2016.02.23 INSERT START
		//m_Check62.SetCheck( 0 );										// 欄外印字項目
		//m_Check62.EnableWindow( FALSE );
//2016.02.23 INSERT END
	}
//2018.03.13 INSERT START
	if( m_pSnHeadData->Sn_Syuusei&0x10 ){
		m_Check65.EnableWindow( TRUE );									// 相続人等
		m_Check65.SetCheck( m_pPrintCmInfo->OPV ? FALSE: TRUE );
	}else{
		m_Check65.SetCheck( 0 );
		m_Check65.EnableWindow( FALSE );
	}
//2018.03.13 INSERT END
	// 申告書（複写分兼控用）　出力項目設定
	m_Check90.EnableWindow( TRUE );									// 欄外に【控用】を印刷する
	m_Check90.SetCheck( m_pPrintCmInfo->OPD ? TRUE : FALSE );
	if( *m_pShinInfo->pProgress ){										// 電子申告済を印刷する（進捗管理）
		m_Check91.ShowWindow( SW_SHOW );
		if( m_pSnHeadData->Sn_Sign2 & 0x80 )		*m_pShinInfo->pFinishSw = 1;
		else										*m_pShinInfo->pFinishSw = 0;
		m_Check91.SetCheck( *m_pShinInfo->pFinishSw );
	}
	else	{
		m_Check91.SetCheck( 0 );
		m_Check91.ShowWindow( SW_HIDE );
//		pSyzShin->m_FinishSw = 0;	
		m_pSnHeadData->Sn_Sign2 &= 0x7f;
	}
	// 税理士欄　出力項目設定
	if( m_pShinInfo->pO_DATA->o_hojn&0x01 ) {
		GetDlgItem(IDC_CHECK70)->SetWindowText("税理士法人名");
		GetDlgItem(IDC_CHECK71)->SetWindowText("税理士法人名を縮小して出力");
	}
	else	{
		GetDlgItem(IDC_CHECK70)->SetWindowText("事務所名");
		GetDlgItem(IDC_CHECK71)->SetWindowText("事務所名を縮小して出力");
	}
//★//[13'02.12]///
//	m_Check70.SetCheck( m_pPrintCmInfo->OPJ ? FALSE: TRUE );					// 事務所名
//	if( pSyzShin->Snd.Sn_Sign2&0x10 )	m_Check71.SetCheck( TRUE );		// 事務所名を圧縮で出力
//	else								m_Check71.SetCheck( FALSE );
//	m_Check72.SetCheck( m_pPrintCmInfo->OPK ? FALSE: TRUE );					// 税理士名
//	m_Check73.SetCheck( m_pPrintCmInfo->OPE ? FALSE: TRUE );					// 電話番号
///////////////////
	if( !m_pShinInfo->isExistOffice ){
		m_pPrintCmInfo->OP2 = 1;
		m_Check70.SetCheck( FALSE );
		m_Check71.SetCheck( FALSE );
		m_Check72.SetCheck( FALSE );
		m_Check73.SetCheck( FALSE );
		m_Check70.EnableWindow( FALSE );
		m_Check71.EnableWindow( FALSE );
		m_Check72.EnableWindow( FALSE );
		m_Check73.EnableWindow( FALSE );
		GetDlgItem(IDC_STATIC_ITEM2)->EnableWindow( FALSE );
//--> '15.02.25 INS START
		m_Check74.EnableWindow( FALSE );
		m_Check75.EnableWindow( FALSE );
		m_pSnHeadData->m_ZroutSgn &= ~0x03;
//<-- '15.02.25 INS END
	}
	else	{
		if( !m_pPrintCmInfo->OP2 )	{
			switch( m_pPrintCmInfo->OPA ) {
				case 1 :	m_Check70.SetCheck( TRUE );
							m_Check71.EnableWindow( TRUE );
							m_Check72.SetCheck( FALSE );
							if( m_pPrintCmInfo->OPsign&0x01 )	m_Check71.SetCheck( 1 );
							else						m_Check71.SetCheck( 0 );
							break;
				case 2 :	m_Check70.SetCheck( TRUE );
							m_Check71.EnableWindow( TRUE );
							m_Check72.SetCheck( TRUE );
							if( m_pPrintCmInfo->OPsign&0x01 )	m_Check71.SetCheck( 1 );
							else						m_Check71.SetCheck( 0 );
							break;
				default:	m_Check70.SetCheck( FALSE );
							m_Check71.SetCheck( FALSE );
							m_Check71.EnableWindow( FALSE );
							m_Check72.SetCheck( TRUE );
							break;
			}
		}
		m_Check73.SetCheck( m_pPrintCmInfo->OP9 ? FALSE: TRUE );

//--> '15.02.25 INS START
		if( (m_pPrintCmInfo->zr_kbn==1) || (m_pPrintCmInfo->zr_kbn==0) ){
			m_Check74.EnableWindow( FALSE );
			m_Check74.ShowWindow( SW_HIDE );
			m_Check75.EnableWindow( FALSE );
			m_Check75.ShowWindow( SW_HIDE );
//--> '15.03.03 CUT START
//			m_pSnHeadData->m_ZroutSgn &= ~0x03;
//<-- '15.03.03 CUT END
		}
		else if( m_pPrintCmInfo->zr_kbn == 2 ){
//2017.07.06 INSERT START
			if( IsCompanyPackage() ){
				m_pSnHeadData->m_ZroutSgn &= ~0x03;
			}else{
//2017.07.06 INSERT END
				if( m_Check72.GetCheck() ){
					m_Check74.EnableWindow( TRUE );
					// チェックのON/OFF
					if( m_pSnHeadData->m_ZroutSgn&0x01 ){
						m_Check74.SetCheck( TRUE );
						m_Check75.EnableWindow( TRUE );
						if( m_pSnHeadData->m_ZroutSgn&0x02 ){
							m_Check75.SetCheck( TRUE );
						}
					}
					else{
						m_Check75.EnableWindow( FALSE );
					}
				}
				else{
					m_Check74.EnableWindow( FALSE );
					m_Check75.EnableWindow( FALSE );
					m_pSnHeadData->m_ZroutSgn &= ~0x03;
				}
//2017.07.06 INSERT START
			}
//2017.07.06 INSERT END
		}
//<-- '15.02.25 INS END
	}
	SetJSMoede();
///////////////////

//	// 個人各計算表　出力項目設定
////-- '15.04.04 --
////	if( m_pSnHeadData->IsSoleProprietor() || m_pShinInfo->sgSpc ){
////		if( m_pShinInfo->sgSpc ){
////---------------
//	if( m_pSnHeadData->IsSoleProprietor() || *m_pShinInfo->pSgSpc ){
//		if ( !( m_pSnHeadData->m_DispTabSgn & 0x2000 ) ) {
//			if (*m_pShinInfo->pSgSpc) {
//				CString	Title;
//				Title = _T("特定収入各計算表 出力項目設定");
//				GetDlgItem(IDC_STATIC_ITEM20)->SetWindowText(Title);
//			}
//			m_Check80.SetCheck(m_pPrintCmInfo->OPL ? FALSE : TRUE);				// 会社名(氏名)を出力
//		}
//		else m_Check80.ShowWindow( SW_HIDE );
//	}
//	else	m_Check80.ShowWindow( SW_HIDE );
	m_Check80.ShowWindow(SW_HIDE);

	// 個人マスターの場合、改正バージョンにより平成/令和の出力を判定
	CDBSyzShinMainView	*pMainView = ( CDBSyzShinMainView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 0 ) );
	//if( m_Util.GetPrePrintGengo(m_pSnHeadData, true) == 2 ){
	if( m_Util.GetPrePrintGengo(m_pSnHeadData, pMainView->GetActViewVer(), true) == 2 ){
			m_Check101.ShowWindow(SW_SHOW);
		if( m_pSnHeadData->Sn_TYPE&0x01 ){
			m_Check101.EnableWindow(FALSE);
			m_Check101.SetCheck(1);
		}
		else{
			long symd = 0, eymd = 0;
			pMainView->GetKazeiKikan( symd, eymd );
			if(( eymd >= ICS_SH_R04_KOJINREFORM_DAY ) && ( m_pSnHeadData->IsSoleProprietor() )) {
				// 令和4年4月1日以降の課税期間かつ個人業種の場合、元号に〇をつけるは選択不可
				// 令和4年3月31日より前の課税期間でチェックを付けていた場合には、チェックを付けた状態で選択不可とする
				// 品質依頼分（修正依頼なし）
				m_Check101.EnableWindow( FALSE );
				m_Check101.SetCheck( m_pPrintCmInfo->OP_GENGO ? 1 : 0 );
			}
			else {
				m_Check101.EnableWindow(TRUE);
				m_Check101.SetCheck(m_pPrintCmInfo->OP_GENGO ? 1 : 0);
			}
		}
	}
	else{
		m_Check101.ShowWindow(SW_HIDE);
	}

// 改良依頼 20-0352 修正 add -->
	if ( pMainView->GetActViewVer() == ID_VER_SYZ31 ) {
		// 課税標準額等の内訳書
		// 代表者氏名 又は 氏名
		if(m_pPrintCmInfo->OPW & 0x01) {
			m_Check110.SetCheck(0);
		}
		else {
			m_Check110.SetCheck(1);
		}
		// 代表者氏名又は氏名 フリガナ
		if(m_pPrintCmInfo->OPW & 0x02) {
			m_Check111.SetCheck(0);
		}
		else {
			m_Check111.SetCheck(1);
		}
		// 更正の請求
		if( m_pSnHeadData->Sn_Syuusei & 0x02 ){
			// 代表者氏名 又は 氏名
			if(m_pPrintCmInfo->OPW & 0x04) {
				m_Check112.SetCheck(0);
			}
			else {
				m_Check112.SetCheck(1);
			}
			// 代表者氏名又は氏名 フリガナ
			if(m_pPrintCmInfo->OPW & 0x08) {
				m_Check113.SetCheck(0);
			}
			else {
				m_Check113.SetCheck(1);
			}
		}
		else {
			GetDlgItem ( IDC_STATIC_ITEM22 )->ShowWindow ( SW_HIDE );
			m_Check112.EnableWindow(FALSE);
			m_Check112.ShowWindow(SW_HIDE);
			m_Check113.EnableWindow(FALSE);
			m_Check113.ShowWindow(SW_HIDE);
		}
	}
	else {
		GetDlgItem ( IDC_STATIC_ITEM21 )->ShowWindow ( SW_HIDE );
		m_Check110.EnableWindow(FALSE);
		m_Check110.ShowWindow(SW_HIDE);
		m_Check111.EnableWindow(FALSE);
		m_Check111.ShowWindow(SW_HIDE);
		GetDlgItem ( IDC_STATIC_ITEM22 )->ShowWindow ( SW_HIDE );
		m_Check112.EnableWindow(FALSE);
		m_Check112.ShowWindow(SW_HIDE);
		m_Check113.EnableWindow(FALSE);
		m_Check113.ShowWindow(SW_HIDE);
	}

// 改良依頼 20-0352 修正 add <--
}

void CPrnItem::OnBnClickedCheck63()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPM )	m_pPrintCmInfo->OPM = 0;
	else				m_pPrintCmInfo->OPM = 1;		
	
}

void CPrnItem::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

void CPrnItem::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnCancel();
}

HBRUSH CPrnItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if(GetDlgItem(IDC_STATIC_INCOME) == pWnd)
		pDC->SetTextColor(RGB(255,0,0));
	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CPrnItem::OnBnClickedCheck74()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	chk74;
	chk74 = m_Check74.GetCheck();
	if( chk74 ){
		m_Check75.EnableWindow( TRUE );
		m_pSnHeadData->m_ZroutSgn |= 0x01;
	}
	else{
		m_Check75.SetCheck( FALSE );
		m_Check75.EnableWindow( FALSE );
		m_pSnHeadData->m_ZroutSgn &= ~0x01;
//--> '16.08.26 INS START
		m_pSnHeadData->m_ZroutSgn &= ~0x02;
//<-- '16.08.26 INS END
	}
}

void CPrnItem::OnBnClickedCheck75()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	chk75;
	chk75 = m_Check75.GetCheck();
	if( chk75 ){
		m_pSnHeadData->m_ZroutSgn |= 0x02;
	}
	else{
		m_pSnHeadData->m_ZroutSgn &= ~0x02;
	}
}

void CPrnItem::OnBnClickedCheck64()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_Check64.GetCheck() ){
		m_pPrintCmInfo->OPU = 1;
	}
	else{
		m_pPrintCmInfo->OPU = 0;
	}
}

//-----------------------------------------------------------------------------
// 法人番号チェックを表示？（'17.03.29）
//-----------------------------------------------------------------------------
// 返送値	true	：	表示する
//			false	：	表示しない
//-----------------------------------------------------------------------------
bool CPrnItem::IsShowHojinNoChk()
{
	if( (m_pPrintCmInfo->Shrevtype>=ID_ICSSH_MYNUMBER_REVTYPE) && (m_pSnHeadData->IsSoleProprietor()==FALSE) ){
		return true;
	}
	else{
		return false;
	}
}

void CPrnItem::OnBnClickedCheck65()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_pPrintCmInfo->OPV )	m_pPrintCmInfo->OPV = 0;
	else				m_pPrintCmInfo->OPV = 1;		
}

void CPrnItem::OnBnClickedCheck101()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if( m_Check101.GetCheck() ){
		m_pPrintCmInfo->OP_GENGO = 1;
	}
	else{
		m_pPrintCmInfo->OP_GENGO = 0;
	}
}


void CPrnItem::OnBnClickedCheck66()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( m_Check66.GetCheck() ) {
		m_pSnHeadData->Sn_GenericSgn |= 0x10;
	}
	else {
		m_pSnHeadData->Sn_GenericSgn &= ~0x10;
	}
}


void CPrnItem::OnBnClickedCheck67()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( m_Check67.GetCheck() ) {
		m_pSnHeadData->Sn_GenericSgn |= 0x20;
	}
	else {
		m_pSnHeadData->Sn_GenericSgn &= ~0x20;
	}
}


// 改良依頼 20-0352 修正 add -->
void CPrnItem::OnBnClickedCheck110()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( m_Check110.GetCheck() ) {
		m_pPrintCmInfo->OPW &= ~0x01;
	}
	else {
		m_pPrintCmInfo->OPW |= 0x01;
	}
}

void CPrnItem::OnBnClickedCheck111()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( m_Check111.GetCheck() ) {
		m_pPrintCmInfo->OPW &= ~0x02;
	}
	else {
		m_pPrintCmInfo->OPW |= 0x02;
	}
}

void CPrnItem::OnBnClickedCheck112()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( m_Check112.GetCheck() ) {
		m_pPrintCmInfo->OPW &= ~0x04;
	}
	else {
		m_pPrintCmInfo->OPW |= 0x04;
	}
}

void CPrnItem::OnBnClickedCheck113()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( m_Check113.GetCheck() ) {
		m_pPrintCmInfo->OPW &= ~0x08;
	}
	else {
		m_pPrintCmInfo->OPW |= 0x08;
	}
}
// 改良依頼 20-0352 修正 add <--

// 付表5 控除対象仕入税額を1項目のみ出力
void CPrnItem::OnBnClickedCheck68()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ( m_Check68.GetCheck() ) {
		m_pSnHeadData->Sn_GenericSgn |= 0x40;
	}
	else {
		m_pSnHeadData->Sn_GenericSgn &= ~0x40;
	}

	// チェックの切り替えを行うが出力せずに業務を終了し電子申告へ移動すると正常に出力されない可能性があるため、このタイミングで出力サインを更新する
	CDBSyzShinMainView	*pMainView = NULL;
	pMainView = ( CDBSyzShinMainView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 0 ) );
	if ( pMainView ) {
		CH31HyoView *pH31HyoView = NULL;
		pH31HyoView = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( H31_HONPYO_WND_IDX ) );
		if ( pH31HyoView ) {
			pH31HyoView->SetPintArea();
		}
	}
}
