#pragma once

#ifdef _SYZ_TAXATIONLIST_EXPORT_CHANGE_
	#define SYZ_TAXATIONLIST_CLASS_EXPORT __declspec(dllexport)
#else
	#define SYZ_TAXATIONLIST_CLASS_EXPORT
#endif

#include <BaseTaxHyoview.h>

//----->'14.10.08
#define		WM_USER_UNLOCK			(WM_APP+100)
#define		WM_USER_RECALQ			(WM_APP+101)
#define		WM_USER_READ_ZEIRISHI	(WM_APP+102)
#define		WM_USER_UPDATE_INDICATE	(WM_APP+103)
#define		WM_USER_SETFOCUS		(WM_APP+104)
#define		WM_USER_SYZINIT			(WM_APP+105)
#define		WM_USER_TENKI			(WM_APP+106)
#define		WM_USER_DISP_SPECPASS	(WM_APP+107)
#define		WM_USER_SHINTAB_INIT	(WM_APP+108)
#define		WM_USER_SPCPOST_RECALQ	(WM_APP+109)
#define		WM_USER_SKJPOST_RECALQ	(WM_APP+111)
#define		WM_USER_SET_ENDSTART	(WM_APP+112)	// '15.07.31
#define		WM_USER_CHG_LOCK_DISP	(WM_APP+116)	// '16.07.22

#define		WM_MYMESSAGE			(WM_APP+110)
//<-------------

//2015.03.05 INSERT START
#define		LINES_PER_PAGE_21 16 //1ページの行数　２(１)
#define		LINES_PER_PAGE_22 16 //1ページの行数　２(２)
//2015.03.05 INSERT END

//2017.03.01 INSERT START
#define		KEISANSYO_KEIKA	20160401	//28年4月以降開始の計算書を経過措置対応レイアウトに切り替える
//2017.03.01 INSERT END

extern "C" SYZ_TAXATIONLIST_CLASS_EXPORT HINSTANCE WINAPI GetTaxationListInstance();


// CTaxationListView フォーム ビュー

class CShinJigyo;
class CShinFudousan;
class CShinNougyou;
class CShinBuy;
class CShinSell;
class CShinKniinpDlg;
class CH26TaxListData;
class CShzCalc;

//----->経過措置用'14.08.07
class CShinBuyKeika;
class CShinFudousanKeika;
class CShinNougyouKeika;
class CShinJigyoKeika;
class CShinSellKeika;
class CShinKniinpKeika;
//<----------------------

//2016.10.20 INSERT START
class CShinKniinp6syuDlg;
class CShinKniinpKeika6syu;
//2016.10.20 INSERT END

//--> '15.02.06 INS STRT
class CH26SpcListData;
class CShinSpecific1;
class CShinSpecific21EX;
class CShinSpecific22;
class CShinSpecific34;
class CShinSpecific51;
class CShinSpecific52;
class CShinSpecific53;
class CShinSpecific6;
//<-- '15.02.06 INS END

//2017.03.01 INSERT START
class CShinSpecific1EX;
class CShinSpecific1EXkeika;
class CShinSpecific21EX2;
class CShinSpecific22keika;
class CShinSpecific51keika;
class CShinSpecific52keika;
class CShinSpecific53keika;
//2017.03.01 INSERT END

//== 定数 ==
//-- '15.02.28 --
//const	int	ICSSH_TAXLIST_TAB_MAX	=	7;	// 課税取引高計算書 最大タブ数
//---------------
const	int	ICSSH_TAXLIST_TAB_MAX	=	16;	// 課税取引高計算書 最大タブ数
//---------------

class SYZ_TAXATIONLIST_CLASS_EXPORT CTaxationListView : public CBaseTaxHyoView
{
	DECLARE_DYNCREATE(CTaxationListView)

protected:
	CTaxationListView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CTaxationListView();

public:
//	enum { IDD = IDD_TAXATIONLIST_VIEW };
	enum { IDD = 101 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

// 操作
public:
	// エラーメッセージの取得
	CString& GetErrMsg(){ return m_ErrMsg; };
	// 初期化
//2016.10.20 UPDATE START
//	int InitView( CDBNpSub *pZmsub, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei );
	int InitView( CDBNpSub *pZmsub, SYC_SHININFO *pShinInfo, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, BOOL SixKindFlg );
//2016.10.20 UPDATE END
	// 履歴シーケンスセット
	void SetSnSeq( int snseq );
	
	// 履歴シーケンス切替//'14.09.01
	int ChangeSnSeq( int snseq );

	// 終了処理
	int EndJob();
	// 終了処理　20191112　追加
	int EndJob( int wflg );
	// データ書込み
	int WriteData( BOOL isEnd );
	// 経理方式の切替
	int ChgTaxationSystem( int *pKHstax );
//2016.06.22 INSERT START
	// 個人決算書名称変更の取得切替
	int ChgKoketsuMeisyoGet( int *pKoketsu );
//2016.06.22 INSERT EMD

	// 画面表示フラグを切り換える'14.08.05
	void SetDispedFlg( BOOL bShow ){ m_bShowed = bShow; };
	// 全画面：画面表示 再初期化'14.08.05
//2016.10.20 INSERT START
//	int ReInitDisp( BOOL isCompChange, BOOL isResetFocus=TRUE );
	int ReInitDisp( BOOL isCompChange, BOOL SixKindFlg , BOOL isResetFocus=TRUE );
//2016.10.20 INSERT END
	// 現在の入力位置にフォーカスセット
	void SetNowPosition( BOOL IsInEnd = FALSE );
	// 全画面：画面表示 課税取引金額計算表 再初期化('15.02.09)
	int ReInitTaxDisp();

	// 印刷情報のセット
	int SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar , SH_PRINT_CMINFO *pPrintCmInfo);
	// 印刷（課税売上高）
	int PrintSellList( int pno, int fno, CDC *pDC );
	// 印刷（課税仕入高）
	int PrintBuyList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 事業所得）
	int PrintJigyoList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 不動産所得）
	int PrintFudousanList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 農業所得）
	int PrintNougyouList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 事業別売上高）
	int PrintKniinpList( int pno, int fno, CDC *pDC );

	// 印刷（課税売上高）
	int PrintSellKeikaList( int pno, int fno, CDC *pDC );
	// 印刷（課税仕入高）
	int PrintBuyKeikaList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 事業所得）
	int PrintJigyoKeikaList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 不動産所得）
	int PrintFudousanKeikaList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 農業所得）
	int PrintNougyouKeikaList( int pno, int fno, CDC *pDC );
	// 印刷（課税取引金額計算表 事業別売上高）
	int PrintKniinpKeikaList( int pno, int fno, CDC *pDC );

	// 手入力サインのクリア
	int ClearManualInputSign();

	// ページ切替 '14.08.08
	void PageChange(int move);

// 課税取引金額計算書との連動('14.08.08)
	int ConnectedTaxList();

	//'14.10.08
	//本表に転記するデータを取得
	void GetTenkiData( SH_TENKIDATA* tenki_data );

	// 現課税取引金額計算表データを消去して、データの再読込み
	int ReReadData();

	// 金額の再取得
	int ReGetCalqedMony( BOOL isResetFocus );

	//'15.01.09
	//過年度プロ呼出し後にデータを吸い上げる
	int GetPastData();

	//== 特定収入対応('15.02.06) ==
	// 全画面：画面表示 特定収入計算表 再初期化
	int ReInitSpcDisp();
	// 全画面：画面表示 特定収入計算表 再初期化
	int ReInitSpcDisp( int selno );
	// 特定収入に係る調整額計算書（印刷）
	int SpecificList( int ListType, int ListPage );
	// 計算表-１　資産の譲渡等の対価の額の計算（印刷）
	int SpecList10( int sign );
//2017.03.01 INSERT START
	// 計算表-１　資産の譲渡等の対価の額の計算（印刷）
	int SpecList10Ex( int sign );
	// 計算表-１　資産の譲渡等の対価の額の計算（経過措置対応）（印刷）
	int SpecList10ExKeika( int sign );
//2017.03.01 INSERT END
	// 計算表２（1）　特定収入金額合計（印刷）
	int SpecList21( int sign, int page );
//2015.03.05 INSERT START
	// 計算表２（1）　特定収入金額合計（印刷）
	int SpecList21Ex( int sign, int page );
//2015.03.05 INSERT END
//2017.03.01 INSERT START
	// 計算表２（1）　特定収入金額合計（印刷）
	int SpecList21Ex2( int sign, int page );
//2017.03.01 INSERT END
	// 計算表２（２）　特定収入金額合計（個別対応方式）	（印刷）
//2015.03.05 UPDATE START
//	int SpecList22( int sign, int page );
	int SpecList22( int sign, int page, int zeiType );
//2015.03.05 UPDATE END
//2017.03.01 INSERT START
	// 計算表２（２）　特定収入金額合計（個別対応方式 経過措置対応）	（印刷）
	int SpecList22Keika( int sign, int page, int zeiType );
//2017.03.01 INSERT END
	// 計算表-３　特定収入割合の計算表, 計算表-４　調整割合の計算表 （印刷）
	int SpecList34( int sign );
	// 計算表-５　調整額の計算表	売上割合９５％以上 （印刷）
	int SpecList50( int sign );
//2015.03.05 INSERT START
	// 計算表-５　調整額の計算表(８％)	売上割合９５％以上 （印刷）
	int SpecList50Ex( int sign );
//2015.03.05 INSERT END
//2017.03.01 INSERT START
	// 計算表-５　調整額の計算表(経過措置対応)	売上割合９５％以上 （印刷）
	int SpecList50Keika( int sign );
//2017.03.01 INSERT END
	// 計算表-５　調整額の計算表	売上割合９５％未満　個別対応 （印刷）
	int SpecList51( int sign );
//2015.03.05 INSERT START
	// 計算表-５　調整額の計算表(８％)	売上割合９５％未満　個別対応 （印刷）
	int SpecList51Ex( int sign );
//2015.03.05 INSERT END
//2017.03.01 INSERT START
	// 計算表-５　調整額の計算表(経過措置対応)	売上割合９５％未満 （印刷）
	int SpecList51Keika( int sign );
//2017.03.01 INSERT END
	// 計算表-５　調整額の計算表	売上割合９５％未満　比例配分 （印刷）
	int SpecList52( int sign );
//2015.03.05 INSERT START
	// 計算表-５　調整額の計算表(８％)	売上割合９５％未満　比例配分 （印刷）
	int SpecList52Ex( int sign );
//2015.03.05 INSERT END
//2017.03.01 INSERT START
	// 計算表-５　調整額の計算表(経過措置対応)	売上割合９５％未満　比例配分 （印刷）
	int SpecList52Keika( int sign );
//2017.03.01 INSERT END
	// 計算表-６　経過措置対象課税仕入れがある場合の特定収入に係る課税仕入れ等の税額の配賦計算表 （印刷）
	int SpecList60( int sign );
	// 特定収入計算表との連動（'15.02.13）
	int ConnectedSpcList();
	// 消費税簡易計算表　ダイアログ表示（経過措置あり）
	int SpecPassOnDlg();
	// 消費税簡易計算表　ダイアログ表示（経過措置なし）
	int SpecPassOffDlg();
//2015.03.05 INSERT START
	// 消費税簡易計算表　ダイアログ表示
	int SpecPassOffExDlg();
//2015.03.05 INSERT END
	// 本表データとの連動（'15.03.09）
	int LinkHonpyoData( BOOL IsChgDoSpc );
	// 本表データへの書き込み（'15.03.09）
	int SetHonpyoMony();
//2015.03.05 INSERT START
	// 計算表２（1）　ページ数取得
	int GetMxList21Ex();
	// 計算表２（2）８％　ページ数取得
	int GetMxList22_8per();
	// 計算表２（2）５％　ページ数取得
	int GetMxList22_5per();

	// 計算表２（2）８％　ページの先頭のレコード番号配列作成
	int MakePageManage22_8per();
	// 計算表２（2）５％　ページの先頭のレコード番号配列作成
	int MakePageManage22_5per();

//2017.03.01 INSERT START
	// 計算表２（1）　ページ数取得
	int GetMxList21Ex2();
	// 計算表２（2）経過措置　ページ数取得
	int GetMxList22_keika();

	// 計算表２（2）経過措置　ページの先頭のレコード番号配列作成
	int MakePageManage22_keika();

	// 計算書を経過措置対応の形式(国からの正式発表の形式)で出力するか
	BOOL IsKeikaType_Keisansyo();
//2017.03.01 INSERT END

	BOOL IsRatioOver95();
//2015.03.05 INSERT END
	// 特定収入の再計算('15.04.03)
	int RecalqSpc( SYZTOSPC_DATA *pSyzToSpc );
	// データ書込み
	int WriteSpcData( BOOL isEnd );

	// 入力ロック切替時の再表示('16.07.22)
	void ChgLockDisp();

//2016.10.20 INSERT START
	//// 簡易課税第六種フラグをセット
	void SetSixKindFlg( BOOL SixKindFlg ){ m_SixKindFlg = SixKindFlg; };
	// 改正様式をセットする
	//void SetShRevType( EnumIdIcsShRevType shrevtype ){ m_ShRevType = shrevtype;};
//2016.10.20 INSERT END
//2017.12.04 INSERT START
	//課税売上高の「損益計算書の収入金額」が入力されているか
	BOOL IsEmpty_SonekiSyunyu();
	//課税売上高の「損益計算書の仕入金額と経費の金額の合計額」が入力されているか
	BOOL IsEmpty_SonekiShiireKeihi();
//2017.12.04 INSERT END

protected:

	//----->'14.08.05
	// 内部タブの初期化
//	int InitTab();
	int InitTab( BOOL bDelete );
	// 内部タブの初期化(実体)
	int InitTabSub( BOOL bDelete );
	// タブ内のダイアログを全消去
	void DeleteInsideDlg();
	// 初期入力位置にフォーカスセット
	void SetItemPosition( int selno );
	//<--------------


	//課税取引金額計算表 事業別売上高 印刷関連
	int Mk_List( int* plist );
	void Title_Print( int hlist );
	void GyoshuTitle( int hlist );
//2016.10.20 INSERT START
	void GyoshuTitle_6syu( int hlist );
//2016.10.20 INSERT END
	int PrtKniInpData( int hlist );

	//課税取引金額計算表 事業別売上高 印刷関連　経過措置
	int Mk_List_Keika( int* plist );
	void Title_Print_Keika( int hlist );
	void GyoshuTitle_Keika( int hlist );
//2016.10.20 INSERT START
	void GyoshuTitle_Keika_6syu( int hlist );
//2016.10.20 INSERT END
	int PrtKniInpData_Keika( int hlist );

	// 各表の更新後の再表示
	void IndicateTasList( int selno );
	// 各表の更新後のフォーカスセット('16.12.26)
	void IndicateItemPosition( int selno );

	//== 特定収入対応('15.02.06) ==
	// 内部タブの初期化（メイン）
	int InitTabMain( BOOL bDelete );
	// 特定収入タブの初期化
	int InitSpecificTab( BOOL bDelete );
	// タブ内のダイアログを全消去(特定収入)
	void DeleteInsideSpcDlg();
	// フォーカスセット
	void SetItemPositionEx( int selno, BOOL IsInEnd=FALSE );//yoshida150311
//	void SetIemPositionEx( int selno );

	// 平成継続出力？
	bool IsJcContinuePrint();

// 属性
private:
	CString				m_ErrMsg;							// エラーメッセージ
	CArithEx			m_Arith;							// 倍長演算
	CDBNpSub			*m_pZmSub;							// 公益クラス
	CSyzUtil			m_Util;								// ユーティリティクラス
	SYC_SHININFO		*m_pShinInfo;						// 共通情報
	int					m_SnSeq;							// 履歴シーケンス
	int					m_SvSnSeq;							// 履歴シーケンス（前回保存）'14.09.01
	CShzCalc			*m_pShzCalc;						// 課税取引集計クラス
	CSyzSyukei			*m_pSyzSyukei;						// 消費税集計クラス	('14.08.08)

	BOOL				m_bShowed;							// 画面表示フラグ'14.08.05

	//== 外部情報 ==
	CSnHeadData			*m_pSnHeadData;						// 消費税ヘッダ情報

	// 内部タブ
	CArray<SH_TABMNG>	m_TabMng;							// タブ管理
	ICSDialog			*m_pFirstPage;						// 初期ページ保存
	int					SHIN_TABMAX;						// タブ最大数
	short				m_ListType[ICSSH_TAXLIST_TAB_MAX];	// 内部タブ管理
	CShinJigyo			*m_pShinJigyo;						// 事業
	CShinFudousan		*m_pShinFudousan;					// 不動産
	CShinNougyou		*m_pShinNougyou;					// 農業
	CShinBuy			*m_pShinBuy;						// 課税仕入
	CShinSell			*m_pShinSell;						// 課税売上
	CShinKniinpDlg		*m_pShinKniinp;						// 事業別売上高
//2016.10.20 INSERT START
	CShinKniinp6syuDlg	*m_pShinKniinp6syu;					// 事業別売上高６種対応
//2016.10.20 INSERT END
	int					m_nowTabIdx;						// 現表示タブNo.

	CRect				vwRect;								// ウィンドウレクト'14.08.01

	// 帳票データ
	CH26TaxListData		*m_pTaxListData;					// 課税取引金額計算書
	CShinBuyKeika		*m_pShinBuyKeika;					// 課税仕入(経過措置用)'14.08.07
	CShinFudousanKeika	*m_pShinFudousanKeika;				// 不動産(経過措置用)'14.08.11
	CShinNougyouKeika	*m_pShinNougyouKeika;				// 農業(経過措置用)'14.08.11
	CShinJigyoKeika		*m_pShinJigyoKeika;					// 事業(経過措置用)'14.08.11
	CShinSellKeika		*m_pShinSellKeika;					// 課税売上(経過措置用)'14.08.11
	CShinKniinpKeika	*m_pShinKniinpKeika;				// 事業別売上高(経過措置用)'14.10.08
//2016.10.20 INSERT START
	CShinKniinpKeika6syu	*m_pShinKniinpKeika6syu;		// 事業別売上高(経過措置用)６種対応
//2016.10.20 INSERT END

	//== 印刷情報 ==
	CVPrint				*m_pVprn;							// 出力クラス
	NPRT_PAR			*m_pPset;							// 印刷情報
	char				*m_pBp;								// 印刷用エリア			BP
	char				*m_pBpSv;							// 印刷用エリア（保存）	BP1
	int					m_Pno;								//						Pno
	SH_PRINT_CMINFO		*m_PrintCmInfo;

	LIST_PAR			m_ListPar;							// 作表

	//== 特定収入計算表 ==
	BOOL				m_isLinkingSpcToHonpyo;				// 本表へ特定収入を連動中？	'15.04.29
	SF_PAR				m_SfPar;							// 文字出力定義パラメータ
	EnumIdIcsSelTaxtype	m_SelType;							// タブ種別
	CH26SpcListData		*m_pSpcListData;					// 特定収入計算表
	CShinSpecific1		*m_pShinSpecific1;					// 特定収入計算表１
	CShinSpecific21EX	*m_pShinSpecific21;					// 特定収入計算表２－１
	CShinSpecific34		*m_pShinSpecific34;					// 特定収入計算表３、４

	CShinSpecific22		*m_pShinSpecific22per8;				// 特定収入計算表２－２（８％）
	CShinSpecific22		*m_pShinSpecific22per5;				// 特定収入計算表２－２（５％）
	CShinSpecific51		*m_pShinSpecific51per8;				// 特定収入計算表５－１（８％）
	CShinSpecific51		*m_pShinSpecific51per5;				// 特定収入計算表５－１（５％）
	CShinSpecific52		*m_pShinSpecific52per8;				// 特定収入計算表５－２（８％）
	CShinSpecific52		*m_pShinSpecific52per5;				// 特定収入計算表５－２（５％）
	CShinSpecific53		*m_pShinSpecific53per8;				// 特定収入計算表５－３（８％）
	CShinSpecific53		*m_pShinSpecific53per5;				// 特定収入計算表５－３（５％）

	CShinSpecific6		*m_pShinSpecific6;					// 特定収入計算表６

//2017.03.01 INSERT START
	CShinSpecific1EX		*m_pShinSpecific1EX;			// 特定収入計算表１
	CShinSpecific1EXkeika	*m_pShinSpecific1EXkeika;		// 特定収入計算表１(経過措置対応)
	CShinSpecific21EX2		*m_pShinSpecific21EX2;			// 特定収入計算表２－１
	CShinSpecific22keika	*m_pShinSpecific22keika;		// 特定収入計算表２－２(経過措置対応)
	CShinSpecific51keika	*m_pShinSpecific51keika;		// 特定収入計算表５－１(経過措置対応)
	CShinSpecific52keika	*m_pShinSpecific52keika;		// 特定収入計算表５－２(経過措置対応)
	CShinSpecific53keika	*m_pShinSpecific53keika;		// 特定収入計算表５－３(経過措置対応)
//2017.03.01 INSERT END

	long				m_TopLine, 
						m_ColmCount,
						m_lineCount,
						m_XPosition,
						m_YPosition; 
	long				m_SXPoint,
						m_SYPoint,
						m_EXPoint,
						m_EYPoint;
	long				m_SXPkmn,
						m_SXPinp,
						m_SXPzen,
						m_SXPkar,
						m_SXPksi,
						m_SXPtzn,
						m_SXPexp;
	long				m_RightPoint,
						m_XPFrame,
						m_LeftCl,
						m_StartLine,
						m_EndLine,
						m_WorkLine;

	char				m_SDATE[128], m_EDATE[128], m_COPNM[128], m_COPNO[20], m_DMME[20];

//2015.03.05 INSERT START
	CArray<int>			m_PageManage_22per5,m_PageManage_22per8;	//ページの先頭のレコード番号記憶
//2015.03.05 INSERT END
//2017.03.01 INSERT START
	CArray<int>			m_PageManage_22keika;
//2017.03.01 INSERT END

//2016.10.20 INSERT START
	BOOL				m_SixKindFlg;		//m_SixKindFlg = TRUE・・第六事業種あり
//	EnumIdIcsShRevType	m_ShRevType;
//2016.10.20 INSERT END

	int					m_JcContinuePrint;	// 和暦継続出力？

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSTabCtrl	m_MainTab;
	virtual void OnInitialUpdate();
	void OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnButtonF2();//'14.08.05
	afx_msg void OnButtonF4();
	afx_msg void OnButtonF5();
	afx_msg void OnButtonF7();
	afx_msg void OnButtonF8();//'14.10.08
	afx_msg void OnButtonF9();
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateKomonsakiInf(CCmdUI *pCmdUI);			//'14.08.05
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButtonF12();									//'14.08.08
	afx_msg void OnUpdateButtonF12(CCmdUI *pCmdUI);				//'14.08.08
	afx_msg void OnUpdateButtonF8(CCmdUI *pCmdUI);				//'14.10.08
	afx_msg void OnUpdateButtonF7(CCmdUI *pCmdUI);				//'14.10.08
	LRESULT OnMyMessage( WPARAM wParam, LPARAM lParam );
	LRESULT OnUserShintabInit(WPARAM wParam, LPARAM lParam);	//'15.02.17
	afx_msg void OnButtonF11();									//'15.03.26
	afx_msg void OnUpdateButtonF11(CCmdUI *pCmdUI);				//'15.03.26
};
