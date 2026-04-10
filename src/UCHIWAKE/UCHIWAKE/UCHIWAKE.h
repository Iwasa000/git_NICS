// UCHIWAKE.h : UCHIWAKE アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル
#include "MainFrm.h"

#include "ChildFrm.h"
#include "UCHIWAKEDoc.h"
#include "UCHIWAKEView.h"
#include "frmUc011Yotyokin.h"
#include "frmUc021Uketoritegata.h"
#include "frmUc031Urikakekin.h"
#include "frmUc041Karibarai.h"
#include "frmUc042Kasituke.h"
#include "frmUc051Tanaorosi.h"
#include "frmUc051Tanaorosi.h"
#include "frmUc061Yuukasyouken.h"
#include "frmUc071Koteisisan.h"
#include "frmUc081Siharaitegata.h"
#include "frmUc091Kaikakekin.h"
#include "frmUc101Kariukekin.h"
#include "frmUc102Gensen.h"
#include "frmUc111Kariirekin.h"
#include "frmUc121Totiuriage.h"
#include "frmUc131Uriagedaka.h"
#include "frmUc141Yakuin.h"
#include "frmUc142Jinkenhi.h"
#include "frmUc151Tidaiyatin.h"
#include "frmUc152Kenrikin.h"
#include "frmUc153Kougyou.h"
#include "frmUc161Zatueki.h"
#include "frmUc162Zatuson.h"
#include "frmUc171Sonota1.h"
#include "frmUc181Sonota2.h"
// midori 190301 add -->
#include "frmUc011YotyokinK.h"
#include "frmUc021UketoritegataK.h"
#include "frmUc042KasitukeK.h"
#include "frmUc061YuukasyoukenK.h"
#include "frmUc081SiharaitegataK.h"
#include "frmUc111KariirekinK.h"
#include "frmUc131UriagedakaK.h"
#include "frmUc141YakuinK.h"
// midori 190301 add <--

// インボイス登録番号追加対応_23/09/11 add -->
#include "frmUc021UketoritegataI.h"
#include "frmUc031UrikakekinI.h"
#include "frmUc041KaribaraiI.h"
#include "frmUc042KasitukeI.h"
#include "frmUc071KoteisisanI.h"
#include "frmUc081SiharaitegataI.h"
#include "frmUc091KaikakekinI.h"
#include "frmUc101KariukekinI.h"
#include "frmUc121TotiuriageI.h"
#include "frmUc151TidaiyatinI.h"
#include "frmUc152KenrikinI.h"
#include "frmUc153KougyouI.h"
#include "frmUc161ZatuekiI.h"
#include "frmUc162ZatusonI.h"
#include "frmUc171Sonota1I.h"
// インボイス登録番号追加対応_23/09/11 add <--

#include "dlgYokukiMessage.h"
// midori 180503 del -->
//#include "dlgYokukiMain.h"
// midori 180503 del <--

#include "UcCommon.h"		//	定数ヘッダ
#include "dbUcInfMain.h"	//	uc_inf_mainテーブルクラス
#include "dbUcInfSub.h"		//	uc_inf_subテーブルクラス
#include "CoSel8.h"			//　会社選択実装
#include "dbIcsVtbl.h"		//	vtblテーブルクラス
#include "dbUcLstHokan.h"	//　保管テーブルクラス

// 会社選択実装
#include "UcRenCommon.h"

// CUCHIWAKEApp:
// このクラスの実装については、UCHIWAKE.cpp を参照してください。
//

class CUCHIWAKEApp : public ICSWinApp
{
public:
	CUCHIWAKEApp();
	~CUCHIWAKEApp();

private:
	UINT	m_indexDT[24];	// 24帳票分
public:
	UINT GetIndex( int i ) { return m_indexDT[i]; }

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	財務連動で追加
//++++++++++++++++++++++++++++++++++++++++++++++++++
public:
// midori 151730,151263 class del -->
	//CDBZmSub*	  m_pZmSub;
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
	CDBNpSub*	  m_pZmSub;
// midori 151730,151263 class add <--
	RENZVOL_INFO  m_RenZvol;

// midori UC_0015 add -->
	int		m_swUtwInfoRegister;	// 勘定科目内訳書（非連動）
// midori UC_0015 add <--
// midori UC_0037 add -->
	int		m_swUtwTandoku;			// 内訳書単独マスター
// midori UC_0037 add <--
// 157759 add -->
	int		m_swIcspCoop;			// ICSP連携
// 157759 add <--
// 157768 add -->
	CString	m_cmdlin;
// 157768 add <--

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	帳表切替で追加
//++++++++++++++++++++++++++++++++++++++++++++++++++
public:		// MainFrameで使用するため private → public に変更(2009.09.08 TS自動ログオフ対応)
// midori 190301 del -->
	//DOCUMENT_INFO	m_uDocInfo[UC_ID_FORMNO_MAX];	//	ドキュメント管理情報
// midori 190301 del <--
// midori 190301 add -->
	//DOCUMENT_INFO	m_uDocInfo[UC_ID_FORMNO_MAX_KANSO];	//	ドキュメント管理情報	// インボイス登録番号追加対応_23/09/11 del
	DOCUMENT_INFO	m_uDocInfo[UC_ID_FORMNO_MAX_INVO];	//	ドキュメント管理情報	// インボイス登録番号追加対応_23/09/11 add
// midori 190301 add <--
public:
	CDatabase*	m_pDB;								//	データベースハンドル
	CString		m_strPath;							//	アプリ実行フォルダのパス
	int			m_intCoSelType;						//	会社選択タイプ
	int			m_intCoSelFormOrder;				//	会社選択時の表示する画面のFormOrder（-1:初期値）
	CUcFunctionCommon m_clsFunc;					//	共通関数クラス
// midori M-17061301 add -->
	CoSel		m_Cs;								// 会社選択クラス
// midori M-17061301 add <--
// midori 154237,154238 del -->
//// midori 154190 add -->
//	int			m_YsChg;							//	1:様式を切替中
//// midori 154190 add <--
// midori 154237,154238 del <--
// midori UC_0026 del -->
//// midori 181101 add -->
//	int			m_APNO;								// アプリケーションNo
//// midori 181101 add <--
// midori UC_0026 del <--
// 24/01/30_23-0845 add -->
	BOOL		m_CloudSansyoFlg;
	BOOL		m_CloudSansyoFlg2;
// 24/01/30_23-0845 add <--
private:
	CString		m_DatabaseName;						//　データベース名(Z??_C????????_Y????)
public:
	int  StartApp( int intAppType = 0 );			//	アプリケーションの開始
	void EndApp();									//	アプリケーションの終了
	BOOL InitDocTemplate();							//	ドキュメントテンプレート初期化
	void SetDocTemplate( int nCnt, int nFormSeq );	//	ドキュメントテンプレート設定
	void OpenDocument( int nFormSeq );				//	ドキュメントを開く
// midori 154237,154238 del -->
	//int	 ChangeDocument( int nFormOrder );			//	ドキュメントを切り替える
// midori 154237,154238 del <--
// midori 154237,154238 add -->
	int	 ChangeDocument( int nFormOrder, int pSw=0 );	//	ドキュメントを切り替える
// midori 154237,154238 add <--
	void CloseDocument();							//	ドキュメントを閉じる
	int	 GetFormOrder( int nFormSeq );				//	シーケンス番号からオーダー番号取得
	void UpdateViewDBInfo();						//	オープン済みViewのDB情報を更新する
// midori 180104 del -->
	//int  CorporationSelect();						//	会社選択
// midori 180104 del <--
// midori 180104 add -->
	int		CorporationSelect( int intCoSelType );	//	会社選択
	int		co_ok_ex( int pSyori );
// midori 180104 add <--
	CfrmUc000Common* UchiwakeCreateFormViewObject(int intFormSeq);
	BOOL GetPCOMSELECTCODSNPublic(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan);
// 21/07/21 add -->
//	BOOL SetPCOMSELECTCODSNPublic(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan);
// 21/07/21 add <--
	CString MakeDatabaseNamePublic(char* inApno, char* inCoCode, char *inKikan);
	CString GetTargetDatabaseName();
// midori 180104 add -->
	BOOL	IsKanendoMaster( void );
// midori 180104 add <--
// midori 153790 add -->
// midori UC_0038 del -->
	//int		OrderCheck();
// midori UC_0038 del <--
// midori UC_0038 add -->
	int		OrderCheckEx( int intFormSeq );
// midori UC_0038 add <--
// midori 153790 add <--
// midori 190301 add -->
	void	KansoCheck(void);
	void	KansoMessage(void);
	int		KansoDataConvert( void );
// midori 190301 add <--

// インボイス登録番号追加対応_23/10/23 add -->
	int		InvNoColumnConvert(CDatabase* pdb);
	void	InvoNoCheck(void);
	// インボイス登録番号を追加したことにより、様式②と様式⑧の支払銀行、支払銀行支店名に改行マークを差し込む
	BOOL	BankNameResetConvert(void);
// インボイス登録番号追加対応_23/10/23 add <--
// 改良No.21-0086,21-0529 add -->
	// 様式⑧支払手形の内訳書の科目入力追加、「電子記録○○」等科目追加に関するカラム拡張、データ追加・変更等
	int		KamokuAddColumnConvert1(CDatabase* pdb);
	int		KamokuAddColumnConvert2(CDatabase* pdb);
// 改良No.21-0086,21-0529 add <--

// midori UC_0037 add -->
	int		GetUcTandokuSw( int pApno, int pCocod, int pEymd );
// midori UC_0037 add <--
// midori 190504 add -->
	int		DeleteSpaceMaster(CDatabase* pdb);
// midori 190504 add <--
// midori 155262 add -->
	int		KansoOmitConvert(BOOL bKansosw);
// midori 155262 add <--
// midori UC_0060 add -->
	BOOL	KansoOmitRepair( CDatabase* pdb );
// midori UC_0060 add <--
// midori 155314 del -->
	int		ChkOmit(CDatabase* pdb,BOOL bKansosw);
// midori 155314 del <--
// midori 191002 add -->
	void	KeiStrConvert(CDatabase* pdb);
// midori 191002 add <--
// midori 155327 add -->
	int		BankConvert(CDatabase* pdb);
	int		AddressConvert(CDatabase* pdb);
// midori 155327 add <--
// midori 156188 add -->
	void	KamokuRowConvert(CDatabase* pdb);
// midori 156188 add <--
// 20-0450 add -->
	int		SyuruiMeigaraConvert( int pSw, BOOL pSel );
// 20-0450 add <--

// 158138 add -->
	BOOL	Convert210701( CDatabase* pdb );
// 158138 add <--
// 22-0003_220324 add -->
	BOOL	Convert220324( CDatabase* pdb );
// 22-0003_220324 add <--

private:
	BOOL CheckAndMakeTable(CString inSQLServer, CString inDatabaseName, CString inFileName, CString inSQLUse, int mode);
// midori UC_0006 del -->
//	BOOL CheckAndMakeTableSub(CString inFileName, CString inSQLServer, CString inDatabaseName, CString inSQLUse, int mode);
// midori UC_0006 del <--
// midori UC_0006 add -->
	int CheckAndMakeTableSub(CString inFileName, CString inSQLServer, CString inDatabaseName, CString inSQLUse, int mode);
// midori UC_0006 add <--
	CString MakeDatabaseName(char* inApno, char* inCoCode, char *inKikan);
	void SetExitMessage( int intAppType , CString strMsg , int intExitNo = 0 );
	int ICSGetZvolRenData(CDBZmSub* pZmSub, RENZVOL_INFO* uRenZvol);
	BOOL GetPCOMSELECTCODSN(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan);
// 21/07/21 add -->
//	BOOL SetPCOMSELECTCODSN(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan);
// 21/07/21 add <--
	BOOL ExecAfterConvert();
// midori 181002 del -->
	//BOOL DeleteAllTables();
// midori 181002 del <--
	int  MaintenanceHokanData();
	BOOL MantenanceHokanDataSub(int nFormSeq);
	BOOL SetONAddHokanSign(int nFormSeq);
//	BOOL IsUnmatchHokanTable(int nFormSeq, CfrmUc000Common* pView);
	void SetNonActiveFlag(BOOL st);
	
	int	UchiConvUpdateMainProc();
	int	UchiConvUpdateDataTable(int intFormSeq);
// midori 180503 del -->
//	int	UchiYokukiUpdateMainProc(long lngUchiSSymd, long lngUchiEEymd);
//	int	UchiYokukiUpdateSubProc(int intFormSeq);
//	int UchiYokukiGetZvolumeYmd(long* lngSSymd, long* lngEEymd);

//// midori 160612 add -->
//	void	CnvDbToBCD( long pYmd, UCHAR* pYear, UCHAR* pMon, UCHAR* pDay );
//	int		KakuteiSet();
//// midori 160612 add <--
//// midori UC_0004 add -->
//	int		ChkKakuteiUpd();
//// midori UC_0004 add <--
//// midori 160612_2 add -->
//	int		KanendoKakuteiSet();
//// midori 160612_2 add <--
// midori 180503 del <--
// midori 170503 add -->
	int		NumRowCheck();
// midori 170503 add <--
// midori UC_0006 add -->
	int		CheckTable();		// テーブルの重複チェック及び修正
// midori UC_0006 add <--

// midori 180503 del -->
//// midori UC_0009 add -->
//	int		CheckGeneralVar( CDBZmSub* pZmSub );
//// midori UC_0009 add <--
// midori 180503 del <--

// midori 180104 del -->
//// midori 153112 add -->
//	BOOL	IsKanendoMaster( void );
//// midori 153112 add <--
// midori 180104 del <--

// midori 180101 add -->
	int		KessankiCheck(void);	// 決算期の連動
// midori 180101 add -->

// 会社選択実装（DBZmSub8d.libをリンクに追加すること！）
private:
//	CDBZmSub	m_ZmSub;

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int		BeginApp();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUCHIWAKEApp theApp;

extern void ICSExitEx(CString strErrMsg);		// 2010/07/XX 一括印刷対応

// 2010/07/XX 一括印刷対応
extern CBatchPrt	g_BatchPrt;		// 一括印刷管理クラス

