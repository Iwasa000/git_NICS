#pragma once

#ifdef _SYZ_TAXATIONLIST31_EXPORT_CHANGE_
#define SYZ_TAXATIONLIST31_CLASS_EXPORT __declspec(dllexport)
#else
#define SYZ_TAXATIONLIST31_CLASS_EXPORT
#endif

#include "BaseTaxHyoView.h"

#define		WM_USER_UNLOCK31			( WM_APP + 100 )
#define		WM_USER_RECALQ31			( WM_APP + 101 )
#define		WM_USER_UPDATE_INDICATE31	( WM_APP + 103 )
#define		WM_USER_SYZINIT31			( WM_APP + 105 )
#define		WM_USER_TENKI31				( WM_APP + 106 )
#define		WM_USER_SHINTAB_INIT31		( WM_APP + 108 )
#define		WM_USER_SPCPOST_RECALQ31	( WM_APP + 109 )
#define		WM_MYMESSAGE31				( WM_APP + 110 )
#define		WM_USER_SET_ENDSTART31		( WM_APP + 112 )
#define		WM_MYMESSAGESPEADDTAB		( WM_APP + 113 )
//// 24/02/15_特定収入 add -->
//#define		WM_MYMESSAGESELCHANGE		( WM_APP + 114 )
//// 24/02/15_特定収入 add <--
// 240228_No3.higuchi add -->
#define		WM_MYMESSAGESETFOCUS		( WM_APP + 114 )
// 240228_No3.higuchi add <--

#define		LINES_PER_PAGE_21_31 16		//1ページの行数　２(１)
#define		LINES_PER_PAGE_22_31 16		//1ページの行数　２(２) 表面
#define		LINES_PER_PAGE_22_2_31 16	//1ページの行数　２(２) 裏面

extern "C" SYZ_TAXATIONLIST31_CLASS_EXPORT HINSTANCE WINAPI GetTaxationList31Instance();

// CTaxationList31View ダイアログ

const	int	ICSSH_TAXLIST31_TAB_MAX	=	16;	// 課税取引高計算書 最大タブ数

class CH31TaxListData;
class CShinJigyo31;
// kasai add -->
class CShinJigyo31Ex;
// kasai add <--
// 2023/07/11 h.wada add -> 
class CShinJigyo31Ex2;
// h.wada add <-
// 2023/07/14 tanaka add -> 
class CShinNougyou31Ex2;
// tanaka add <-
class CShinFudousan31;
class CShinNougyou31;
// 211129_h add -->
class CShinFudousan31Ex;
class CShinNougyou31Ex;
// 211129_h add <--
class CShinFudousan31Ex2;
class CShinSell31;
class CShinBuy31;
// kasai add -->
class CShinSell31Ex;
class CShinBuy31Ex;
// kasai add <--
// tanaka add -->
class CShinSell31Ex2;
class CShinBuy31Ex2;
// tanaka add <--
class CShinKniinp31;
// 211201_h add -->
class CShinKniinp31Ex;
// 211201_h add <--
class CShzTax31Calc;
class CSyzBaseDlg;
class CH31SpcListData;
class CShinSpecific1_31;
class CShinSpecific1_31Ex;
class CShinSpecific2_31;
class CShinSpecific2_31Ex;
class CShinSpecific22_31;
class CShinSpecific22_2_31;
class CShinSpecific22_2_31Ex;
class CShinSpecific34_31;
class CShinSpecific51_31;
class CShinSpecific51_31Ex;
class CShinSpecific51_31Ex2; // 20231124
class CShinSpecific52_31;
class CShinSpecific52_31Ex;
class CShinSpecific52_31Ex2; // 20231128
class CShinSpecific53_31;
class CShinSpecific53_31Ex;
class CShinSpecific53_31Ex2; // 20231128
class CShinSpecific52_1_31; // 20231128
class CShinSpecific52_2_31; // 20231128
class CShinSpecific52_31_31; // 20231128
class CShinSpecific52_32_31; // 20231128
class CShinSpecific52_33_31; // 20231128
class CShinSpecific52_41_31; // 20231128
class CShinSpecific52_42_31; // 20231128
class CShinSpecific52_43_31; // 20231128

class CSpcBaseDlg;
class SYZ_TAXATIONLIST31_CLASS_EXPORT CTaxationList31View : public CBaseTaxHyoView
{
	DECLARE_DYNCREATE(CTaxationList31View)

public:
	CTaxationList31View();				 // 標準コンストラクター
	virtual ~CTaxationList31View();

// ダイアログ データ
public:
	//enum { IDD = IDD_TAXATIONLIST31_VIEW };
	enum { IDD = 101 };

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	CH31TaxListData			*m_pTaxListData;					// 課税取引金額計算書
	
	CShinJigyo31			*m_pShinJigyo31;					// 事業
// kasai add -->
	CShinJigyo31Ex			*m_pShinJigyo31Ex;					// 事業（旧税率なし）
// kasai add <--
// 2023/07/11 h.wada add -> 
	CShinJigyo31Ex2         *m_pShinJigyo31Ex2;                 // 事業(令和5年10月1日以後終了する課税期間)
// h.wada add <-
	CShinFudousan31			*m_pShinFudousan31;					// 不動産
	CShinNougyou31			*m_pShinNougyou31;					// 農業
// 2023/07/14 tanaka add -> 
	CShinNougyou31Ex2       *m_pShinNougyou31Ex2;               // 農業(令和5年10月1日以後終了する課税期間)
// tanaka add <-
// 211129_h add -->
	CShinFudousan31Ex		*m_pShinFudousan31Ex;				// 不動産	（旧税率なし）
	CShinNougyou31Ex		*m_pShinNougyou31Ex;				// 農業		（旧税率なし）
// 211129_h add <--
	CShinFudousan31Ex2      *m_pShinFudousan31Ex2;           // 不動産(令和5年10月1日以後終了する課税期間)
	CShinSell31				*m_pShinSell31;						// 課税仕入
	CShinBuy31				*m_pShinBuy31;						// 課税売上
// kasai add -->
	CShinSell31Ex			*m_pShinSell31Ex;					// 課税仕入（旧税率なし）
	CShinBuy31Ex			*m_pShinBuy31Ex;					// 課税売上（旧税率なし）
// kasai add <--
// tanaka add -->
	CShinSell31Ex2			*m_pShinSell31Ex2;					// 課税仕入(令和5年10月1日以後終了する課税期間)
	CShinBuy31Ex2			*m_pShinBuy31Ex2;					// 課税売上(令和5年10月1日以後終了する課税期間)
// tanaka add <--
	CShinKniinp31			*m_pShinKniinp31;					// 事業別課税売上高
// 211201_h add -->
	CShinKniinp31Ex			*m_pShinKniinp31Ex;					// 事業別課税売上高（旧税率なし）
// 211201_h add <--
	CDBNpSub				*m_pZmsub;
	CSnHeadData				*m_pSnHeadData;						// 消費税ヘッダ情報
	CSyzSyukei				*m_pSyzSyukei;						// 消費税集計クラス
	CShzTax31Calc			*m_pShzCalc;						// 課税取引集計クラス

	CH31SpcListData			*m_pSpcListData;					// 特定収入計算表	
	CShinSpecific1_31		*m_pShinSpecific131;				// 特定収入計算表１
	CShinSpecific1_31Ex		*m_pShinSpecific131Ex;				// 特定収入計算表１（旧税率なし）
	CShinSpecific2_31		*m_pShinSpecific231;				// 特定収入計算表２（１）
	CShinSpecific2_31Ex		*m_pShinSpecific231Ex;				// 特定収入計算表２（１）（旧税率なし）
	CShinSpecific22_31		*m_pShinSpecific2231;				// 特定収入計算表２（２）表面
	CShinSpecific22_2_31	*m_pShinSpecific22_2_31;			// 特定収入計算表２（２）裏面
	CShinSpecific22_2_31Ex	*m_pShinSpecific22_2_31Ex;			// 特定収入計算表２（２）（旧税率なし）
	CShinSpecific34_31		*m_pShinSpecific3431;				// 特定収入計算表３
	CShinSpecific51_31		*m_pShinSpecific5131;				// 特定収入計算表５（１）
	CShinSpecific51_31Ex	*m_pShinSpecific5131Ex;				// 特定収入計算表５（１）（旧税率なし）
	CShinSpecific51_31Ex2	*m_pShinSpecific5131Ex2;			// 特定収入計算表５（１）（インボイス改正）20231124
	CShinSpecific52_31		*m_pShinSpecific5231;				// 特定収入計算表５（２）
	CShinSpecific52_31Ex	*m_pShinSpecific5231Ex;				// 特定収入計算表５（２）（旧税率なし）
	CShinSpecific52_31Ex2	*m_pShinSpecific5231Ex2;			// 特定収入計算表５（２）（インボイス改正）20231128
	CShinSpecific53_31		*m_pShinSpecific5331;				// 特定収入計算表５（３）
	CShinSpecific53_31Ex	*m_pShinSpecific5331Ex;				// 特定収入計算表５（３）（旧税率なし）
	CShinSpecific53_31Ex2	*m_pShinSpecific5331Ex2;			// 特定収入計算表５（３）（インボイス改正）20231128
	CShinSpecific52_1_31	*m_pShinSpecific52131;				// 特定収入計算表５－２（１）（インボイス改正）20231128
	CShinSpecific52_2_31	*m_pShinSpecific52231;				// 特定収入計算表５－２（２）（インボイス改正）20231128
	CShinSpecific52_31_31	*m_pShinSpecific523131;				// 特定収入計算表５－２（３）－１（インボイス改正）20231128
	CShinSpecific52_32_31	*m_pShinSpecific523231;				// 特定収入計算表５－２（３）－２（インボイス改正）20231128
	CShinSpecific52_33_31	*m_pShinSpecific523331;				// 特定収入計算表５－２（３）－３（インボイス改正）20231128
	CShinSpecific52_41_31	*m_pShinSpecific524131;				// 特定収入計算表５－２（４）－１（インボイス改正）20231128
	CShinSpecific52_42_31	*m_pShinSpecific524231;				// 特定収入計算表５－２（４）－２（インボイス改正）20231128
	CShinSpecific52_43_31	*m_pShinSpecific524331;				// 特定収入計算表５－２（４）－３（インボイス改正）20231128

	SF_PAR					m_SfPar;							// 文字出力定義パラメータ
	long					m_TopLine, 
							m_ColmCount,
							m_lineCount,
							m_XPosition,
							m_YPosition; 
	long					m_SXPoint,
							m_SYPoint,
							m_EXPoint,
							m_EYPoint;
	long					m_SXPkmn,
							m_SXPinp,
							m_SXPzen,
							m_SXPkar,
							m_SXPksi,
							m_SXPtzn,
							m_SXPexp;
	long					m_RightPoint,
							m_XPFrame,
							m_LeftCl,
							m_StartLine,
							m_EndLine,
							m_WorkLine;

	char					m_SDATE[128], m_EDATE[128], m_COPNM[128], m_COPNO[20], m_DMME[20];

	CArray<int>				m_PageManage_22_31;
	CArray<int>				m_PageManage_22_2_31;


	CSyzUtil				m_Util;
	SYC_SHININFO			*m_pShinInfo;							// 共通情報

	// 内部タブ
	CArray<SH_TABMNG>		m_TabMng;								// タブ管理
	ICSDialog				*m_pFirstPage;							// 初期ページ保存
	CRect					vwRect;

	EnumIdIcsSelTaxtype		m_SelType;

	int						SHIN_TABMAX;							// タブ最大数
	short					m_ListType[ICSSH_TAXLIST31_TAB_MAX];	// 内部タブ管理

	CString					m_ErrMsg;								// エラーメッセージ
	CArithEx				m_Arith;								// 倍長演算

	BOOL					m_bShowed;								// 画面表示フラグ

	int						m_SnSeq;								// 履歴シーケンス
	int						m_SvSnSeq;								// 履歴シーケンス（前回保存）
	BOOL					m_SixKindFlg;
	int						m_nowTabIdx;							// 現表示タブNo.

	BOOL					m_isLinkingSpcToHonpyo;					// 本表へ特定収入を連動中？

	long					m_DispTabSgn;							// タブ表示サイン
	
	//== 印刷 ==
	CVPrint					*m_pVprn;								// 出力クラス
	NPRT_PAR				*m_pPset;								// 印刷情報
	char					*m_pBp;									// 印刷用エリア			BP
	char					*m_pBpSv;								// 印刷用エリア（保存）	BP1
	int						m_Pno;									//						Pno
	SH_PRINT_CMINFO			*m_PrintCmInfo;

	LIST_PAR				m_ListPar;								// 作表

public:
	// エラーメッセージの取得
	CString& GetErrMsg() { return m_ErrMsg; };

	// 初期化
	int InitView ( CDBNpSub *pZmsub, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei );

//CBaseTaxHyoViewクラス
	// 履歴シーケンスセット
	void SetSnSeq( int snseq );
	// 表示帳票の最終データ取得
	void GetDispHyoData();
	// 終了処理
	int EndJob();
	// 終了処理　20191112 追加
	int EndJob( int wflg = 1 );
	// データ書込み
	int WriteData( BOOL isEnd, BOOL isGetData );
	// データ書き込み 2019/10/29追加
	int WriteData( BOOL isEnd );
	// 特定収入データ書き込み
	int WriteSpcData( BOOL isEnd );
	// 全画面：画面表示 再初期化
	int ReInitDisp(BOOL isCompChange, BOOL SixKindFlg, BOOL isResetFocus = TRUE);
	int ReInitSpcDisp ( int selno );
	int ReInitSpcDisp();

	// 特定収入の再計算
	int RecalqSpc ( SYZTOSPC_DATA *pSyzToSpc );

	int ChgKoketsuMeisyoGet( int *pKoketsu );
	// 過年度プロ呼出し後にデータを吸い上げる
	int GetPastData();
	// 画面表示フラグを切り換える
	int ChangeSnSeq( int snseq );
	// 現申告データを消去して、データの再読込み
	int ReReadData();
	// 本表データへの書き込み
	int SetHonpyoMony();
		// 本表データとの連動
	int LinkHonpyoData ( BOOL IsChgDoSpc );

	// 課税売上割合95%以上？
	BOOL IsRatioOver95();

/*出力関係*/
	// 印刷情報のセット
	int SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar , SH_PRINT_CMINFO *pPrintCmInfo);

	// 印刷（課税売上高）
	int PrintSell31List( int pno, int fno, CDC *pDC );
	// 印刷（課税仕入高）
	int PrintBuy31List( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 事業所得）
	int PrintJigyo31List( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 不動産所得）
	int PrintFudousan31List( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 農業所得）
	int PrintNougyou31List( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 事業別売上高）
	int PrintKniinp31List( int pno, int fno, CDC *pDC );

	//課税取引金額計算表 事業別売上高 印刷関連　経過措置
	int PrtKniInpData( int hlist );
	int Mk_List_31( int* plist );
	void Title_Print_31( int hlist );
	void GyoshuTitle_31( int hlist );
	void GyoshuTitle_31_6syu( int hlist );
	int PrtKniInpData_31( int hlist );
// 211203h add -->
	int Mk_List_31_Ex( int* plist );
	void GyoshuTitle_31_Ex( int hlist );
	int PrtKniInpData_31_Ex( int hlist );
// 211203h add <--

	// 特定収入に係る調整額計算書（印刷）
	int SpecificList31 ( int ListType, int ListPage );
	// 計算表１　資産の譲渡等の対価の額の計算（印刷）
	int SpecList1_31 ( int sign );
	int SpecList1_31Ex( int sign );	// 旧税率なし
	// 計算表２（1）　特定収入金額合計（印刷）
	int SpecList21_31 ( int sign, int page );
	int SpecList21_31Ex( int sign, int page ); // 旧税率なし
	// 計算表２（２）　特定収入金額合計（個別対応方式 経過措置対応）表面	（印刷）
	int SpecList22_31 ( int sign, int page );
	// 計算表２（２）　特定収入金額合計（個別対応方式 経過措置対応）裏面	（印刷）
	int SpecList22_2_31 ( int sign, int page );
	int SpecList22_2_31Ex( int sign, int page ); // 旧税率なし
	// 計算表３４　特定収入割合の計算表, 計算表-４　調整割合の計算表 （印刷）
	int SpecList34_31 ( int sign );
	// 計算表５（１）　控除対象仕入税額の調整計算表 （印刷）
	int SpecList51_31 ( int sign );
	int SpecList51_31Ex( int sign ); // 旧税率なし
	int SpecList51_31Ex2(int sign);	 // インボイス改正
	// 計算表５（２）　控除対象仕入税額の調整計算表 個別対応（印刷）
	int SpecList52_31 ( int sign );
	int SpecList52_31Ex( int sign ); // 旧税率なし
	int SpecList52_31Ex2(int sign);	 // インボイス改正
	// 計算表５（３）　控除対象仕入税額の調整計算表 比例配分 （印刷）
	int SpecList53_31 ( int sign );
	int SpecList53_31Ex( int sign ); // 旧税率なし
	int SpecList53_31Ex2(int sign);	 // インボイス改正

	// 計算表５－２（１）
	int SpecList52_1_31(int sign);
	// 計算表５－２（２）
	int SpecList52_2_31(int sign);
	// 計算表５－２（３）－１
	int SpecList52_31_31(int sign);
	// 計算表５－２（３）－２
	int SpecList52_32_31(int sign);
	// 計算表５－２（３）－３
	int SpecList52_33_31(int sign);
	// 計算表５－２（４）－１
	int SpecList52_41_31(int sign);
	// 計算表５－２（４）－２
	int SpecList52_42_31(int sign);
	// 計算表５－２（４）－３
	int SpecList52_43_31(int sign);

	// 計算表２（１） ページ数取得
	int GetMxList21_31();
	// 計算表２（２）表面 ページ数取得
	int GetMxList22_31();
	// 計算表２（２）裏面 ページ数取得
	int GetMxList22_2_31();

	// 計算表２（２）表面　ページの先頭のレコード番号配列作成
	int MakePageManage22_31();
	// 計算表２（２）裏面　ページの先頭のレコード番号配列作成
	int MakePageManage22_2_31();

	void GetTenkiData(SH_TENKIDATA31* tenki_data);
	void GetTenkiData(SH_INVOICE_TENKIDATA31* inv_tenki_data);

	// 手入力項目あり？
	BOOL IsManualInputSign();
	BOOL IsManualInputSign52(void);	// 24/02/29wd_特定収入対応 add

	// 個人決算書金額の取得
	int GetKoketsuKingaku();

	// 所得税連動
	int StzRendo( int idx );

// 24/02/19wd_特定収入対応 add -->
	// 特定収入計算表 最大頁数を返す
	int GetSpecListMaxPage(void);
	// 特定収入計算表 各帳表の使用制限を取得
	int IsSpecificEnable(long *pFlg);
// 24/02/19wd_特定収入対応 add <--
// 24/02/26_特定収入対応 add -->
	// 特定収入計算表 指定した頁の帳票種類を返す
	int GetSpecListType( int pPage );
// 24/02/26_特定収入対応 add <--
// 240308_h_160357 add -->
	BOOL	IsGetSpecListData( int pPage, int pList );
// 240308_h_160357 add <--

private:
	// 所得税　一般業種連動
	int StzRendo_Ippan( CStringArray *pRetAry );
	// 所得税　不動産連動
	int StzRendo_Fudousan( CStringArray *pRetAry );
	// 所得税　農業連動
	int StzRendo_Nougyou( CStringArray *pRetAry );

protected:
	// 内部タブの初期化
	int InitTab ( BOOL bDelete );
	int InitTabMain ( BOOL bDelete );
	int InitTabSub( BOOL bDelete );
	int InitSpecificTab( BOOL bDelete );//不要？

	

	// タブ内のダイアログを全消去
	void DeleteInsideDlg();
	void DeleteInsideSpcDlg();

	// 切替
	void PageChange( int move );
	void IndicateTasList(int selno);
	//void IndicateTasListSpc(int selno);		// 修正No.168722 add

	void IndicateItemPosition(int selno);
	void DiagReSetPosition(void);			// 修正No.168690 add

	// 平成継続出力？
	BOOL IsJcContinuePrint();
	
	int ChgTaxationSystem(int *pKHstax);

	int ReInitTaxDisp();
	void SetNowPosition(BOOL IsInEnd/*=FALSE*/);
	void SetItemPosition(int selno);
	void SetItemPositionEx ( int selno );
	int ConnectedTaxList();
	int ConnectedSpcList();
	int ClearManualInputSign();
	int ReGetCalqedMony(BOOL isResetFocus);
	int SpecPassOffExDlg();
	void ChgLockDisp();
	BOOL IsEmpty_SonekiSyunyu();
	BOOL IsEmpty_SonekiShiireKeihi();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CSyzBaseDlg* GetBaseList( int no );
	//CSpcBaseDlg* GetBaseListSpc(int no);	// 修正No.168722 add

	ICSTabCtrl	m_MainTab;
	virtual void OnInitialUpdate();
	void OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result);

	afx_msg void OnButtonF2();
	afx_msg void OnButtonF4();
	afx_msg void OnButtonF5();
	afx_msg void OnButtonF7();
	afx_msg void OnButtonF8();
	afx_msg void OnButtonF9();
	afx_msg void OnButtonF11();
	afx_msg void OnButtonF12();
	afx_msg void OnUpdateButtonF7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF11(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF12(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateKomonsakiInf(CCmdUI *pCmdUI);
	LRESULT OnUserShintabInit(WPARAM wParam, LPARAM lParam);
	LRESULT OnMyMessage( WPARAM wParam, LPARAM lParam );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
