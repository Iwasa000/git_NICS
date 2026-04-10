// PrnTune.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

//#include "MainFrm.h"
//#include "DBSyzShinDoc.h"
//#include "DBSyzShinView.h"

#include "PrnReportDlg.h"
#include "PrnTune.h"

#define XYmax	500
#define	XYmin	-500

// メインビューの財務クラスを参照
//CDBSyzShinView *CPrnTune::pSyzShin;

static	char	CBF[80], DBF[80], KBF[128];
//static	int		CbCt, AnCt, ItCt, XVP, XHP, XDVP, XDHP, XELA, Answer;
// 財務入力
//static	VARIANT			Val;
//static	ICSColorInfo	ColorInfo;

// CPrnTune ダイアログ

IMPLEMENT_DYNAMIC(CPrnTune, ICSDialog)

CPrnTune::CPrnTune(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnTune::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_swBeForked(0)
{
}

CPrnTune::~CPrnTune()
{
}

void CPrnTune::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL1, m_DBEdit1);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL2, m_DBEdit2);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL3, m_DBEdit3);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL4, m_DBEdit4);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL5, m_DBEdit5);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_SPIN4, m_Spin4);
	DDX_Control(pDX, IDC_SPIN5, m_Spin5);
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_Control(pDX, IDC_STATIC_OCR, m_STATIC_OCR);
	DDX_Control(pDX, IDC_STATIC_IMG, m_STATIC_IMG);
}


BEGIN_MESSAGE_MAP(CPrnTune, ICSDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CPrnTune::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CPrnTune::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CPrnTune::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CPrnTune::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CPrnTune::OnDeltaposSpin5)
//	ON_BN_CLICKED(IDOK, &CPrnTune::OnBnClickedOk)
//	ON_BN_CLICKED(IDCANCEL, &CPrnTune::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPrnTune::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPrnTune::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPrnTune メッセージ ハンドラ

BOOL CPrnTune::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// 個人を選択
//	pSyzShin->m_Kojin = (pSyzShin->Snh.Sn_KOJIN&0x01);									// 個人を選択時は選択可
//	if( (pSyzShin->Vol1.APNO&0xf0) == 0x10 )	pSyzShin->m_Kojin = 1;					// 個人業種マスターは選択可

	EnumIdIcsShKazeihoushiki kzhoushiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );

	BOOL isNoneMTForm = FALSE;
	CDBSyzShinMainView	*pMainView = ( CDBSyzShinMainView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( 0 ) );
	if ( pMainView->GetActViewVer() == ID_VER_SYZ31 ) {
		CH31HyoView	*pH31View = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( H31_HONPYO_WND_IDX ) );
		isNoneMTForm = pH31View->IsNoneMTForm();
	}

	//for( ItCt = 0; ItCt != 20; ItCt++ ){
	for (ItCt = 0; ItCt != 30; ItCt++) {
		memset( &m_pPrintCmInfo->pMvRec[ItCt], '\0', sizeof(SH_MvTbl) );
		m_pPrintCmInfo->pMvRec[ItCt].HyoCn = -1;
	}
	ItCt = 0;
	strcpy_s( m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, "申告書本表");
	if( kzhoushiki == ID_ICSSH_GENSOKU ){
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_IPPAN;
	}
	else{
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_KANI;
	}
	m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
	ItCt++;

//--> '15.11.24 INS START	
	if ( m_pShinInfo->ActViewVer == ID_VER_SYZ26 ) {
		if( (kzhoushiki==ID_ICSSH_GENSOKU) && (m_pSnHeadData->IsNeedTkkzDisp()) ){
			strcpy_s( m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, "課税標準額等内訳書");
			m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_IPPAN2;
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;
		}
	}
	else {
		strcpy_s( m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, "課税標準額等内訳書");
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_IPPAN2;
		m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
		ItCt++;
	}

//<-- '15.11.24 INS END

	if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
		
		//経過措置あり
		/*
		if (kzhoushiki == ID_ICSSH_GENSOKU) {
			
			if (m_pSnHeadData->SVmzsw) {
				//++++付表１－２++++//
				strcpy_s(KBF, sizeof(KBF), "付表１－２");
				strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 16;
				m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
				ItCt++;
			}

			//++++付表１－１++++//
			strcpy_s(KBF, sizeof(KBF), "付表１－１");
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 17;
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;

			if (m_pSnHeadData->SVmzsw) {
				//++++付表２－２++++//
				strcpy_s(KBF, sizeof(KBF), "付表２－２");
				strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 18;
				m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
				ItCt++;
			}

			//++++付表２－１++++//
			strcpy_s(KBF, sizeof(KBF), "付表２－１");
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 19;
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;
			

		}
		else {

			if (m_pSnHeadData->SVmzsw) {
				//++++付表４－２++++//
				strcpy_s(KBF, sizeof(KBF), "付表４－２");
				strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 20;
				m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
				ItCt++;
			}

			//++++付表４－１++++//
			strcpy_s(KBF, sizeof(KBF), "付表４－１");
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 21;
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;

			if (m_pSnHeadData->SVmzsw) {
				//++++付表５－２++++//
				strcpy_s(KBF, sizeof(KBF), "付表５－２");
				strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 22;
				m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
				ItCt++;
			}

			//++++付表５－１++++//
			strcpy_s(KBF, sizeof(KBF), "付表５－１");
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 23;
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;			
		}
		*/
	
		//----->('23.05.01 Add)
		long eymd = 0;
		if (!(m_pSnHeadData->Sn_SKKBN % 2)) eymd = m_pSnHeadData->Sn_MDAYE;
		else								eymd = m_pSnHeadData->Sn_KDAYE;
		
		if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) {
			//----> 20230616
			if( m_pSnHeadData->SVmzsw ){
				strcpy_s(KBF, sizeof(KBF), "付表４－１・４－２");
				strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO4;
				m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
				ItCt++;

				strcpy_s(KBF, sizeof(KBF), "付表５－１・５－２");
				strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO5;
				m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
				ItCt++;
			}
			//<---- 20230616
			else{
				//2割特例計算を使用する
				strcpy_s(KBF, sizeof(KBF), _T("付表６"));
				strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO6_2wari;
				m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
				ItCt++;
			}
		}
		else {
		//<-----
			//経過措置あり
			if (m_pSnHeadData->SVmzsw) {
				if (kzhoushiki == ID_ICSSH_GENSOKU) {
					strcpy_s(KBF, sizeof(KBF), "付表１－１・１－２");
				}
				else {
					strcpy_s(KBF, sizeof(KBF), "付表４－１・４－２");
				}
			}
			else {
				if (kzhoushiki == ID_ICSSH_GENSOKU) {
					//strcpy_s(KBF, sizeof(KBF), "付表１－１");
					if (isNoneMTForm) {
						strcpy_s(KBF, sizeof(KBF), _T("付表１－３"));
					}
					else {
						strcpy_s(KBF, sizeof(KBF), _T("付表１－１"));
					}
				}
				else {
					//strcpy_s(KBF, sizeof(KBF), "付表４－１");
					if (isNoneMTForm) {
						strcpy_s(KBF, sizeof(KBF), _T("付表４－３"));
					}
					else {
						strcpy_s(KBF, sizeof(KBF), _T("付表４－１"));
					}
				}
			}
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO1;
			}
			else {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO4;
			}
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;

			if (m_pSnHeadData->SVmzsw) {
				if (kzhoushiki == ID_ICSSH_GENSOKU) {
					strcpy_s(KBF, sizeof(KBF), "付表２－１・２－２");
				}
				else {
					strcpy_s(KBF, sizeof(KBF), "付表５－１・５－２");
				}
			}
			else {
				if (kzhoushiki == ID_ICSSH_GENSOKU) {
					//strcpy_s(KBF, sizeof(KBF), "付表２－１");
					if (isNoneMTForm) {
						strcpy_s(KBF, sizeof(KBF), _T("付表２－３"));
					}
					else {
						strcpy_s(KBF, sizeof(KBF), _T("付表２－１"));
					}
				}
				else {
					//strcpy_s(KBF, sizeof(KBF), "付表５－１");
					if (isNoneMTForm) {
						strcpy_s(KBF, sizeof(KBF), _T("付表５－３"));
					}
					else {
						strcpy_s(KBF, sizeof(KBF), _T("付表５－１"));
					}
				}
			}
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO2;
			}
			else {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO5;
			}
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;

			if (m_pSnHeadData->m_DispTabSgn & 0x80) {
				// 特例1
				if (m_pSnHeadData->m_DispTabSgn & 0x10) {
					strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, _T("第５－（１）号"));
					m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_TOKUREI1;
					m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
					ItCt++;
				}

				// 特例2
				if (m_pSnHeadData->m_DispTabSgn & 0x20) {
					strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, _T("第５－（２）号"));
					m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_TOKUREI2;
					m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
					ItCt++;
				}

				// 特例3
				if (m_pSnHeadData->m_DispTabSgn & 0x40) {
					strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, _T("第５－（３）号"));
					m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_TOKUREI3;
					m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
					ItCt++;
				}
			}
	//----->('23.05.01 Add)
		}
	//<-----
	}
	else {

		if (m_pSnHeadData->SVmzsw) {
			// 経過措置有
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				strcpy_s(KBF, sizeof(KBF), "付表１");
			}
			else {
				strcpy_s(KBF, sizeof(KBF), "付表４");
			}
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO1;//yoshida190816
//				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 1;
			}
			else {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO4;//yoshida190816
//				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 5;
			}
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				strcpy_s(KBF, sizeof(KBF), "付表２-(2)");
			}
			else {
				strcpy_s(KBF, sizeof(KBF), "付表５-(2)");
			}
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO2;//yoshida190816
//				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 3;
			}
			else {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO5;//yoshida190816
//				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 7;
			}
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;
		}
		else {
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				strcpy_s(KBF, sizeof(KBF), "付表２");
			}
			else {
				strcpy_s(KBF, sizeof(KBF), "付表５");
			}
			strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF);
			if (kzhoushiki == ID_ICSSH_GENSOKU) {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 2;
			}
			else {
				m_pPrintCmInfo->pMvRec[ItCt].HyoTn = 6;
			}
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
			ItCt++;
		}
	}
//2018.03.13 INSERT START
	if( m_pSnHeadData->Sn_Syuusei&0x10 ){
		//----->インボイス施工日以降は準確定の付表６を付表７へ名称変更('23.07.10 cor)
		long eymd = 0;
		if (!(m_pSnHeadData->Sn_SKKBN % 2)) {
			eymd = m_pSnHeadData->Sn_MDAYE;
		}
		else {
			eymd = m_pSnHeadData->Sn_KDAYE;
		}

		if ( eymd >= ICS_SH_INVOICE_DAY ) {
			strcpy_s(KBF, sizeof(KBF), "付表７");
		}
		else {
			strcpy_s(KBF, sizeof(KBF), "付表６");
		}
		//<-----
		strcpy_s( m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF );
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_FHYO6;
		m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
		ItCt++;
	}
//2018.03.13 INSERT END
	if( m_pSnHeadData->IsUseTaxRefund() ){
		strcpy_s( KBF, sizeof( KBF ),"還付申告明細書" );
	}
	else{
		strcpy_s( KBF, sizeof( KBF ),"仕入控除明細書" );
	}
	strcpy_s( m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF );
	if( m_pSnHeadData->IsSoleProprietor() ){
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_KANPU_K;
	}
	else{
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_KANPU_H;
	}
	m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
	ItCt++;
	if( m_pSnHeadData->Sn_Syuusei & 0x04 ){
		strcpy_s( KBF, sizeof( KBF ),"更正の申出書" );
	}
	else{
		strcpy_s( KBF, sizeof( KBF ),"更正の請求書" );
	}
	strcpy_s( m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, KBF );
	if( m_pSnHeadData->IsSoleProprietor() ){
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_KOSEI_K;
	}
	else{
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_KOSEI_H;
	}
	m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
	ItCt++;
	if( m_Util.IsBasicMst(m_pZmSub->zvol) ){
		strcpy_s( m_pPrintCmInfo->pMvRec[ItCt].HyoMs, 20, "課税取引金額計算表");
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_KAZEI;
		m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
		ItCt++;
	}

// 24/03/19_税務代理書面 add -->
	long	eymd=0;
	if(!(m_pSnHeadData->Sn_SKKBN % 2))	{
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else	{
		eymd = m_pSnHeadData->Sn_KDAYE;
	}
	if(eymd >= R06_ZEIMUDAIRI)	{
		strcpy_s(m_pPrintCmInfo->pMvRec[ItCt].HyoMs,20,"税務代理書面");
		m_pPrintCmInfo->pMvRec[ItCt].HyoTn = AD_ZEIMUDAIRI;
		m_pPrintCmInfo->pMvRec[ItCt].HyoCn = 0;
		ItCt++;
	}
// 24/03/19_税務代理書面 add <--

	// コンボボックス描画
	m_Combo1.ResetContent();
	//for( ItCt = 0; ItCt != 20; ItCt++ ) {
	for (ItCt = 0; ItCt != 30; ItCt++) {
		if( m_pPrintCmInfo->pMvRec[ItCt].HyoCn == -1 ){
			break;
		}
		m_Combo1.AddString( m_pPrintCmInfo->pMvRec[ItCt].HyoMs );
	}

	//for( ItCt = 0; ItCt != 20; ItCt++ ){
	for (ItCt = 0; ItCt != 30; ItCt++) {
		memmove( &m_pPrintCmInfo->pPrMove[ItCt], &m_pPrintCmInfo->pPrMv[ItCt], sizeof(SH_PRMOVE) );
	}
	CbCt = 0;
	m_Combo1.SetCurSel( CbCt );

	XDVP = (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKVad / 10;
	XDHP = (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKHad / 10;
	XELA = (int )m_pPrintCmInfo->OPEla;
	XVP =  (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIVad / 10;
	XHP =  (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIHad / 10;

	VARIANT			Val;
	m_Spin1.SetBuddy( GetDlgItem(IDC_ICSDBEDT8CTRL1 ) );
	m_Spin1.SetBase( 10 );				//	基本値は、10進数
	m_Spin1.SetRange( XYmin, XYmax );	//	上下限値をセット
	m_Spin1.SetPos( (int)XDVP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XDVP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit1.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin2.SetBuddy( GetDlgItem(IDC_ICSDBEDT8CTRL2) );
	m_Spin2.SetBase( 10 );				//	基本値は、10進数
	m_Spin2.SetRange( XYmin, XYmax );	//	上下限値をセット
	m_Spin2.SetPos( (int)XDHP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XDHP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit2.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin3.SetBuddy( GetDlgItem(IDC_ICSDBEDT8CTRL3) );
	m_Spin3.SetBase( 10 );				//	基本値は、10進数
	m_Spin3.SetRange( XYmin, XYmax );	//	上下限値をセット
	m_Spin3.SetPos( (int)XELA );		//	デフォルト値
	sprintf_s( CBF,  sizeof( CBF ),"%5d", XELA );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit3.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin4.SetBuddy( GetDlgItem(IDC_ICSDBEDT8CTRL4 ) );
	m_Spin4.SetBase( 10 );				//	基本値は、10進数
	m_Spin4.SetRange( XYmin, XYmax );	//	上下限値をセット
	m_Spin4.SetPos( (int)XVP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XVP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit4.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin5.SetBuddy( GetDlgItem(IDC_ICSDBEDT8CTRL5) );
	m_Spin5.SetBase( 10 );				//	基本値は、10進数
	m_Spin5.SetRange( XYmin, XYmax );	//	上下限値をセット
	m_Spin5.SetPos( (int)XHP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XHP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit5.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

// 依頼No.970 一括印刷の場合、ＯＣＲ用紙の設定を不可にする
	if(m_swBeForked)	{
		m_DBEdit1.EnableWindow(FALSE);
		m_DBEdit2.EnableWindow(FALSE);
		m_DBEdit3.EnableWindow(FALSE);
		m_Spin1.EnableWindow(FALSE);
		m_Spin2.EnableWindow(FALSE);
		m_Spin3.EnableWindow(FALSE);
	}

	c_set( IDC_COMBO1 );
	
	// TODO:  ここに初期化を追加してください
	ICSDialog::OnInitDialogEX();


	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CPrnTune::PreTranslateMessage(MSG* pMsg)
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

		if( pMsg->wParam == VK_F11 ) {
			OnBnClickedButton2();
			return TRUE;
		}
		if( pMsg->wParam == VK_F5 ) {
			GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F5);		
			return TRUE;
		}
		if( pMsg->wParam == VK_TAB ) {
			short shift = ::GetKeyState(VK_SHIFT)&0x80;
			int id = GetFocus()->GetDlgCtrlID();
			if( id == IDC_COMBO1 ){
				if( !shift ){
					//m_DBEdit1.SetFocus();
					if(m_DBEdit1.IsWindowEnabled() == TRUE)	{
						m_DBEdit1.SetFocus();
					}
					else	{
						m_DBEdit4.SetFocus();
					}
					return TRUE;
				}
			}
			else{
				if( id == IDC_BUTTON2 ){
					m_DBEdit5.SetFocus();
					return TRUE;
				}
			}
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
int CPrnTune::InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked )
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

BEGIN_EVENTSINK_MAP(CPrnTune, ICSDialog)
	ON_EVENT(CPrnTune, IDC_ICSDBEDT8CTRL1, 1, CPrnTune::TerminationIcsdbedt8ctrl1, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrnTune, IDC_ICSDBEDT8CTRL2, 1, CPrnTune::TerminationIcsdbedt8ctrl2, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrnTune, IDC_ICSDBEDT8CTRL3, 1, CPrnTune::TerminationIcsdbedt8ctrl3, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrnTune, IDC_ICSDBEDT8CTRL4, 1, CPrnTune::TerminationIcsdbedt8ctrl4, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrnTune, IDC_ICSDBEDT8CTRL5, 1, CPrnTune::TerminationIcsdbedt8ctrl5, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CPrnTune::TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst)
{

	short shift = ::GetKeyState(VK_SHIFT)&0x80;
	if( nChar == VK_TAB ){
		if( shift ) nChar = VK_LEFT;
		else		nChar = VK_RIGHT;
	}
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;
	switch( nChar ) {
		case NULL :
			return;
		case VK_RETURN: case VK_RIGHT: case VK_LEFT:
			m_DBEdit1.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			memset( DBF, '\0', sizeof( DBF ) );
			memset( CBF, '\0', sizeof( CBF ) );
			XDVP = atoi( (char *)Val.pbVal );
			if( XDVP < XYmin || XDVP > XYmax ) {
				XDVP = 0;
				strcpy_s( CBF, sizeof( CBF ), "0" );
				Val.pbVal = (unsigned char*)CBF;
				m_DBEdit1.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			}

			m_Spin1.SetPos( (int)XDVP );			//	デフォルト値

			if( nChar == VK_LEFT ){
				c_set( IDC_COMBO1 );
			}
			else{
				((ICSDialog*)this)->NextDlgCtrl();
			}

			break;

		case VK_DELETE	:
			break;
		default			:
			break;
	}
}

void CPrnTune::TerminationIcsdbedt8ctrl2(long nChar, long inplen, long kst)
{
	short shift = ::GetKeyState(VK_SHIFT)&0x80;
	if( nChar == VK_TAB ){
		if( shift ) nChar = VK_LEFT;
		else		nChar = VK_RIGHT;
	}

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;
	switch( nChar ) {
		case NULL :
			return;
		case VK_RETURN : case VK_RIGHT: case VK_LEFT:
			m_DBEdit2.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			memset( DBF, '\0', sizeof( DBF ) );
			memset( CBF, '\0', sizeof( CBF ) );
			XDHP = atoi( (char *)Val.pbVal );
			if( XDHP < XYmin || XDHP > XYmax ) {
				XDHP = 0;
				strcpy_s( CBF, sizeof( CBF ), "0" );
				Val.pbVal = (unsigned char*)CBF;
				m_DBEdit2.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			}

			m_Spin2.SetPos( (int)XDVP );			//	デフォルト値

			if( nChar == VK_LEFT ){
				((ICSDialog*)this)->PrevDlgCtrl();
			}
			else{
				((ICSDialog*)this)->NextDlgCtrl();
			}

			break;
		case VK_DELETE	:
			break;
		default			:
			break;
	}
}

void CPrnTune::TerminationIcsdbedt8ctrl3(long nChar, long inplen, long kst)
{
	short shift = ::GetKeyState(VK_SHIFT)&0x80;
	if( nChar == VK_TAB ){
		if( shift ) nChar = VK_LEFT;
		else		nChar = VK_RIGHT;
	}

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;
	switch( nChar ) {
		case NULL :
			return;
		case VK_RETURN : case VK_RIGHT: case VK_LEFT:
			m_DBEdit3.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			memset( DBF, '\0', sizeof( DBF ) );
			memset( CBF, '\0', sizeof( CBF ) );
			XELA = atoi( (char *)Val.pbVal );
			if( XELA < XYmin || XELA > XYmax ) {
				XELA = 0;
				strcpy_s( CBF, sizeof( CBF ), "0" );
				Val.pbVal = (unsigned char*)CBF;
				m_DBEdit3.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			}

			m_Spin3.SetPos( (int)XELA );			//	デフォルト値

			if( nChar == VK_LEFT ){
				((ICSDialog*)this)->PrevDlgCtrl();
			}
			else{
				((ICSDialog*)this)->NextDlgCtrl();
			}

			break;
		case VK_DELETE	:
			break;
		default			:
			break;
	}
}

void CPrnTune::TerminationIcsdbedt8ctrl4(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	short shift = ::GetKeyState(VK_SHIFT)&0x80;
	if( nChar == VK_TAB ){
		if( shift ) nChar = VK_LEFT;
		else		nChar = VK_RIGHT;
	}

	VARIANT	Val;
	switch( nChar ) {
		case NULL :
			return;
		case VK_RETURN : case VK_RIGHT: case VK_LEFT:
			m_DBEdit4.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			memset( DBF, '\0', sizeof( DBF ) );
			memset( CBF, '\0', sizeof( CBF ) );
			XVP = atoi( (char *)Val.pbVal );
			if( XVP < XYmin || XVP > XYmax ) {
				XVP = 0;
				strcpy_s( CBF, sizeof( CBF ), "0" );
				Val.pbVal = (unsigned char*)CBF;
				m_DBEdit4.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			}

			m_Spin4.SetPos( (int)XVP );			//	デフォルト値

			if( nChar == VK_LEFT ){
				((ICSDialog*)this)->PrevDlgCtrl();
			}
			else{
				((ICSDialog*)this)->NextDlgCtrl();
			}

			break;
		case VK_DELETE	:
			break;
		default			:
			break;
	}
}

void CPrnTune::TerminationIcsdbedt8ctrl5(long nChar, long inplen, long kst)
{
	short shift = ::GetKeyState(VK_SHIFT)&0x80;
	if( nChar == VK_TAB ){
		if( shift ) nChar = VK_LEFT;
		else		nChar = VK_RIGHT;
	}

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;
	switch( nChar ) {
		case NULL :
			return;
		case VK_RETURN : case VK_RIGHT: case VK_LEFT:
			m_DBEdit5.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			memset( DBF, '\0', sizeof( DBF ) );
			memset( CBF, '\0', sizeof( CBF ) );
			XHP = atoi( (char *)Val.pbVal );
			if( XHP < XYmin || XHP > XYmax ) {
				XHP = 0;
				strcpy_s( CBF, sizeof( CBF ), "0" );
				Val.pbVal = (unsigned char*)CBF;
				m_DBEdit5.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
			}

			m_Spin5.SetPos( (int)XVP );			//	デフォルト値

			if( nChar == VK_LEFT ){
				((ICSDialog*)this)->PrevDlgCtrl();
			}
			else{
				((ICSDialog*)this)->NextDlgCtrl();
			}

			break;
		case VK_DELETE	:
			break;
		default			:
			break;
	}
}

void CPrnTune::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int pos = pNMUpDown->iPos;
	*pResult = TRUE;

	VARIANT	Val;
	if( (pos & 0x00000000) == 0x00000000 ) {

		pos = pos + pNMUpDown->iDelta; // ポジションの増減
		if( (pos < XYmin) || (pos > XYmax) ) pos = pNMUpDown->iPos;

		if( XDVP != pos ) {
			XDVP = pos;
			*pResult = FALSE;
		}

		sprintf_s( CBF, sizeof( CBF ), "%d", XDVP );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit1.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
}

void CPrnTune::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int pos = pNMUpDown->iPos;
	*pResult = TRUE;

	VARIANT	Val;
	if( (pos & 0x00000000) == 0x00000000 ) {

		pos = pos + pNMUpDown->iDelta; // ポジションの増減
		if( (pos < XYmin) || (pos > XYmax) ) pos = pNMUpDown->iPos;

		if( XDVP != pos ) {
			XDVP = pos;
			*pResult = FALSE;
		}

		sprintf_s( CBF, sizeof( CBF ), "%d", XDVP );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit2.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
}

void CPrnTune::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int pos = pNMUpDown->iPos;
	*pResult = TRUE;

	VARIANT	Val;
	if( (pos & 0x00000000) == 0x00000000 ) {

		pos = pos + pNMUpDown->iDelta; // ポジションの増減
		if( (pos < XYmin) || (pos > XYmax) ) pos = pNMUpDown->iPos;

		if( XDVP != pos ) {
			XDVP = pos;
			*pResult = FALSE;
		}

		sprintf_s( CBF, sizeof( CBF ), "%d", XDVP );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit3.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
}

void CPrnTune::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int pos = pNMUpDown->iPos;
	*pResult = TRUE;

	VARIANT	Val;
	if( (pos & 0x00000000) == 0x00000000 ) {

		pos = pos + pNMUpDown->iDelta; // ポジションの増減
		if( (pos < XYmin) || (pos > XYmax) ) pos = pNMUpDown->iPos;

		if( XDVP != pos ) {
			XDVP = pos;
			*pResult = FALSE;
		}

		sprintf_s( CBF, sizeof( CBF ), "%d", XDVP );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit4.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
}

void CPrnTune::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int pos = pNMUpDown->iPos;
	*pResult = TRUE;

	VARIANT	Val;
	if( (pos & 0x00000000) == 0x00000000 ) {

		pos = pos + pNMUpDown->iDelta; // ポジションの増減
		if( (pos < XYmin) || (pos > XYmax) ) pos = pNMUpDown->iPos;

		if( XDVP != pos ) {
			XDVP = pos;
			*pResult = FALSE;
		}

		sprintf_s( CBF, sizeof( CBF ), "%d", XDVP );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit5.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
}

//void CPrnTune::OnBnClickedOk()
//{
//	// TODO: ここにコントロール通知ハンドラ コードを追加します。
//	MovePrintGetData();
//
//	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKVad = (short )( XDVP * 10 );
//	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKHad = (short )( XDHP * 10 );
//	pSyzShin->OPEla = (short)XELA;
//	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIVad = (short )( XVP * 10 );
//	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIHad = (short )( XHP * 10 );
//
//	for( ItCt = 0; ItCt != 16; ItCt++ ) {
//		memmove( &pSyzShin->PrMv[ItCt], &m_pPrintCmInfo->pPrMove[ItCt], sizeof( struct _PRMOVE ) );
//	}
//	ICSDialog::OnOK();
//}

//void CPrnTune::OnBnClickedCancel()
//{
//	// TODO: ここにコントロール通知ハンドラ コードを追加します。
//	ICSDialog::OnCancel();
//}

void CPrnTune::OnCbnSelchangeCombo1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
//★//[13'03.14]///
	MovePrintGetData();
///////////////////
	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKVad = (short )( XDVP * 10 );
	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKHad = (short )( XDHP * 10 );
	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIVad = (short )( XVP * 10 );
	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIHad = (short )( XHP * 10 );

	CbCt = m_Combo1.GetCurSel();

	XDVP = (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKVad / 10;
	XDHP = (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKHad / 10;
	XVP = (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIVad / 10;
	XHP = (int )m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIHad / 10;

	VARIANT			Val;
	m_Spin1.SetPos( (int)XDVP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XDVP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit1.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin2.SetPos( (int)XDHP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XDHP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit2.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin3.SetPos( (int)XELA );		//	デフォルト値
	sprintf_s( CBF,  sizeof( CBF ),"%5d", XELA );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit3.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin4.SetPos( (int)XVP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XVP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit4.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

	m_Spin5.SetPos( (int)XHP );		//	デフォルト値
	sprintf_s( CBF, sizeof( CBF ), "%5d", XHP );
	Val.pbVal = (unsigned char*)CBF;
	m_DBEdit5.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );

//★//[13'03.18]///
//-- '15.11.24 --
//	if( CbCt ){
//---------------
// 依頼No.970 一括印刷の場合、ＯＣＲ用紙の設定を不可にする
//	if( IsNeedKSK(CbCt) == false ){
	if(IsNeedKSK(CbCt) == false || m_swBeForked)	{
//---------------
		m_DBEdit1.EnableWindow( FALSE );
		m_DBEdit2.EnableWindow( FALSE );
		m_DBEdit3.EnableWindow( FALSE );
		m_Spin1.EnableWindow( FALSE );
		m_Spin2.EnableWindow( FALSE );
		m_Spin3.EnableWindow( FALSE );
	}
	else{
		m_DBEdit1.EnableWindow( TRUE );
		m_DBEdit2.EnableWindow( TRUE );
		m_DBEdit3.EnableWindow( TRUE );
		m_Spin1.EnableWindow( TRUE );
		m_Spin2.EnableWindow( TRUE );
		m_Spin3.EnableWindow( TRUE );
	}
///////////////////
}

//-----------------------------------------------------------------------------
// KSK用紙設定が必要？ ('15.11.24)
//-----------------------------------------------------------------------------
// 引数		sel		：	コンボボックス選択項目
//-----------------------------------------------------------------------------
// 返送値	true	：	KSK用紙設定が必要
//			false	：	KSK用紙設定が不要
//-----------------------------------------------------------------------------
bool CPrnTune::IsNeedKSK( int sel )
{
	if( (m_pPrintCmInfo->pMvRec[sel].HyoTn==0) || (m_pPrintCmInfo->pMvRec[sel].HyoTn==4) || (m_pPrintCmInfo->pMvRec[sel].HyoTn==14) ){
		return true;
	}
	else{
		return false;
	}
}

// 全データ読み込み
void CPrnTune::MovePrintGetData( void )
{
	VARIANT	Val;
	m_DBEdit1.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	sprintf_s( CBF, sizeof( CBF ), "%s", Val.pbVal );
	XDVP = atoi( CBF );
	if( XDVP < XYmin || XDVP > XYmax ) {
		XDVP = 0;
		strcpy_s( CBF, sizeof( CBF ),"0" );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit1.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
	m_Spin1.SetPos( (int)XDVP );

	m_DBEdit2.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	sprintf_s( CBF, sizeof( CBF ), "%s", Val.pbVal );
	XDHP = atoi( CBF );
	if( XDHP < XYmin || XDHP > XYmax ) {
		XDHP = 0;
		strcpy_s( CBF, sizeof( CBF ),"0" );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit2.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
	m_Spin2.SetPos( (int)XDHP );

	m_DBEdit3.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	sprintf_s( CBF, sizeof( CBF ), "%s", Val.pbVal );
	XELA = atoi( CBF );
	if( XELA < XYmin || XELA > XYmax ) {
		XELA = 0;
		strcpy_s( CBF, sizeof( CBF ),"0" );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit3.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
	m_Spin3.SetPos( (int)XVP );

	m_DBEdit4.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	sprintf_s( CBF, sizeof( CBF ), "%s", Val.pbVal );
	XVP = atoi( CBF );
	if( XVP < XYmin || XVP > XYmax ) {
		XVP = 0;
		strcpy_s( CBF, sizeof( CBF ),"0" );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit4.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
	m_Spin4.SetPos( (int)XVP );

	m_DBEdit5.GetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	sprintf_s( CBF, sizeof( CBF ), "%s", Val.pbVal );
	XHP = atoi( CBF );
	if( XHP < XYmin || XHP > XYmax ) {
		XHP = 0;
		strcpy_s( CBF, sizeof( CBF ),"0" );
		Val.pbVal = (unsigned char*)CBF;
		m_DBEdit5.SetData( &Val, ICSDBEDT_TYPE_STRING, 0 );
	}
	m_Spin5.SetPos( (int)XDHP );
}

void CPrnTune::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN,VK_F11);
}


void CPrnTune::SetInitPrnTune( void )
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	MovePrintGetData();

	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKVad = (short )( XDVP * 10 );
	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PKHad = (short )( XDHP * 10 );
	m_pPrintCmInfo->OPEla = (short)XELA;
	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIVad = (short )( XVP * 10 );
	m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[CbCt].HyoTn]._PIHad = (short )( XHP * 10 );

	for( ItCt = 0; ItCt != 30; ItCt++ ) {
		memmove( &m_pPrintCmInfo->pPrMv[ItCt], &m_pPrintCmInfo->pPrMove[ItCt], sizeof(SH_PRMOVE) );
	}
//	ICSDialog::OnOK();
}

void CPrnTune::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

void CPrnTune::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnCancel();
}
