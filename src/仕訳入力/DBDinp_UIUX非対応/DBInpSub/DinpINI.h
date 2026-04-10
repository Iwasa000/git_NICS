/*========================================================================
		入力の設定を読込 ＆ 書込する クラスモジュール 
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
// DinpINI.h : ヘッダー ファイル
//
#ifndef INCLUDE_DINPINI_H
#define INCLUDE_DINPINI_H


// 入力モード選択データ
//
struct _AUTOSEL {
	short	DATE_OPT;
	short	DENP_OPT;
	short	BMON_OPT;
	short	KOJI_OPT;
	short	DEBT_OPT;
	short	CRED_OPT;
	short	BRN_OPT;
	short	BRTEK_OPT;
	short	BRTOTKY_OPT;
	short	SYGBRN_OPT;
#ifdef DB_CLOSE
	short	SYIMPORT_OPT;
#endif
	short	HJIYU_OPT;

	short	KMNO_OPT;
	short	KMPG_OPT;
	short	BRCPY_OPT;
	short	BMNCHK_OPT;
	short	SWKTEK_OPT;
	short	SYOBUZ_OPT;
	short	BMNDSP_OPT;
	short	BMNSEL_OPT;
	short	RDDATE_OPT;
	short	RDBRN_OPT;
	// 現金貸借
	char	DCKM_CODE[10];
	short	DCKM_OPT;

	// 表示設定
	short	DSPLINE_OPT,
			DSPLNWD_OPT,
			DSPDENP_OPT,
			DSPBMON_OPT,
			DSPKOJI_OPT,
			DSPTGNO_OPT,
			DSPTGDT_OPT;
	// 表示項目幅等
	short	RESOL_OPT,		// 前回起動時の解像度(横幅)
			WDSEQ_OPT,
			WDDATE_OPT,
			WDDENP_OPT,
			WDBMON_OPT,
			WDKOJI_OPT,
			WDDEBT_OPT,
			WDCRED_OPT,
			WDVAL_OPT,
			WDZEI_OPT,
			WDSYZ_OPT,
			WDTKY_OPT;

	short	KANADSP_OPT;	// カナ検索時の表示順
	short	BMNBRN_OPT;		// 部門枝番自動登録
	short	KJTOTKY_OPT;	// 工事名称を仕訳摘要に
	
	short	SCNCD_OPT;		// 検索画面での科目コード表示
	short	DSPHJIYU_OPT;	// 変動事由 表示
	short	DSPZEI_OPT;		// 税額 表示
	short	ZEIMDFY_OPT;	// 税額修正
	short	DELSWK_OPT;		// 取消仕訳処理

	short	INP2BK_OPT;		// 部門･工事を借方・貸方とも入力
	short	INDSGN_OPT;		// 個人科目選択

	ULONG	NPJGY_OPT;		// 公益事業費選択('08.12.01)
	ULONG	NPKNR_OPT;		// 公益管理費選択('08.12.01)

	short	SCNCLR_OPT;		// 検索項目クリア[06.22 /09]
	short	KJIINP_OPT;		// 工事未入力チェック[01.21 /10]
							//  + 部門未入力 [ 04.23 /10]
	short	KANKJI_OPT;		// 完成工事チェック[01.21 /10]
	short	KJIKMK_OPT;		// 工事番号科目チェック[03.14 /11]

	short	BRNKAK_OPT;		// 枝番未入力時確認する[04.27 /12]

	long	SLIP_SEQ;		// ドキュメントシーケンス[09.28 /12]

	short	BLKSEL_OPT;		// 1: 画面から選択を削除する
	BYTE	BLK_OPT[8];		// 画面の並びサイン	[11.01 /12]

	short	SWTKYD_OPT;		// 仕訳対応摘要 表示サイン[11.13 /12]

	short	DOCEVI_OPT;		// 証憑番号オプション[12.14 /12]

	short	BMNTKBR_OPT;	// 部門摘要枝番 を使用[02.08 /13]

	short	BRNKANA_OPT;	// 枝番選択 50音順[04.01 /15]

	short	RTSWKKANA_OPT;	// 科目・仕訳対応摘要 50音順[06.23 /17]

	short	BMNTOTKY_OPT;	// 部門名称を仕訳摘要に[06.07 /18]

	short	DSPSYZ_OPT;		// 消費税欄 表示[11.05 /20]

	short	WDHJYU_OPT;		// 変動事由項目幅[07.19 /21]
	short	AWSEQ_OPT,		// 出納帳項目幅[07.19 /21]
			AWDATE_OPT,
			AWDENP_OPT,
			AWBMON_OPT,
			AWKOJI_OPT,
			AWKMK_OPT,
			AWNKN_OPT,
			AWSKN_OPT,
			AWZEI_OPT,
			AWSYZ_OPT,
			AWTKY_OPT,
			AWHJYU_OPT;

short		WDSYON_OPT,	//証憑番号項目幅
			AWSYON_OPT;	//出納帳 証憑番号項目幅
/*- '08.12.01 -*/
//	char	dmy1[144],
//			dmy2[256];
/*-------------*/
	char	dmy1[98-30],	// _SLIP_ACCOUNTS_	: 126 -> 122 [09.28 /12]
							// 画面の並び・仕訳対応摘要 :
							//					  122 -> 110 [11.13 /12]
							//					  110 -> 108 [12.14 /12]
							//					  108 -> 106 [02.08 /13]
							//					  106 -> 104 [04.01 /15]
							//					  104 -> 102 [06.23 /17]
							//					  102 -> 100 [06.07 /18]
							//					  100 ->  98 [11.05 /20]
							//					  98  ->  72 [07.19 /21]
							//					  72  ->  68 [08.23 /21]
			dmy2[256];
/*-------------*/
};

// 伝票番号の設定
#define	CNO_AUTO	1		//自動加算
#define	CNO_EQU		2		//＝SEQ
#define	CNO_NON		3		//伝票番号なし

// 項目定義
#define OPT_NONOPT			0x00	// 選択なし
#define OPT_NON				0x01
#define OPT_FIX				0x02
#define OPT_DSP				0x03
#define OPT_EXEC			0x04

#define OPT_NOINP			0x05

// チェックON
#define OPT_CHKON			0x06

#define OPT_RESET			0x10
#define OPT_AUTO			0x11
#define OPT_EQU				0x12

#define	OPT_BRCPY			0x13	// 枝番コピー

#define OPT_RGSCHK			0x21	// 枝番未入力をチェック
#define OPT_BMNCHK			0x22	// 部門のチェック
#define OPT_SWKTEK			0x23	// 仕訳摘要
#define OPT_SYOBUZ			0x24	// 諸口ブザー

#define OPT_BMNDSP			0x25	// 部門名称表示
#define OPT_BMNSEL			0x26	// 部門選択画面
#define OPT_RDDATE			0x27	// 定型仕訳 日付
#define OPT_RDBRN			0x28	// 　〃　　 枝番

#define OPT_NODSP			0x29	// 項目を表示しない

#define OPT_KMNO			0x31	// 科目コードを表示
#define OPT_KMPG			0x32	// 科目ページを保持
#define OPT_KANAJUN			0x33	// カナ順で表示
#define OPT_HJIYU			0x34	// 変動事由 -> 仕訳摘要
#define OPT_BMNBRN			0x35	// 部門枝番自動登録
#define OPT_KJTOTKY			0x36	// 工事名称を仕訳摘要に

#define OPT_SCNCD			0x37	// 検索一覧で科目コード表示
#define OPT_ZEIMDFY			0x38	// 税額を修正
#define OPT_DELSWK			0x39	// 取消仕訳処理

#define OPT_INP2BK			0x40	// 部門･工事を借方・貸方とも入力
#define OPT_SCNCLR			0x41	// 検索項目クリア

#define OPT_KJIINP			0x42	// 工事番号未入力
#define OPT_KANKJI			0x43	// 完成工事
#define OPT_KJIKMK			0x44	// 工事番号科目チェック
#define OPT_BRNKAK			0x45	// 枝番未入力時確認する[04.27 /12]

#define OPT_SWTKYD			0x46	// 仕訳対応摘要 表示順

// 証憑の設定( OPT_NONOPT は 行ごとに入力 )
#define	SNO_AUTO	1		//自動加算
#define	SNO_EQCNO	2		//＝伝票番号

// 行の高さ
#define OPT_NARROW			0x71
#define OPT_WIDE			0x72


/////////////////////////////////////////////////////////////////////////////
// CDinpINI ウィンドウ
#ifdef _DBINPSUB
class AFX_EXT_CLASS CDinpINI
#else
class CDinpINI
#endif
{
// コンストラクション
public:
	CDinpINI();

// アトリビュート
public:

// オペレーション
public:
	void Init( CDBZmSub *pZm );
	struct _AUTOSEL *GetDinpSW();
	int SetDinpSW( struct _AUTOSEL *psel, BOOL close = TRUE );

	BOOL isMkSyImpDta();
	int isBrTekDisp();
	int	IsBumonDisp();

private:
//	BOOL syogbrn_ok( unsigned char apno );
	BOOL syogbrn_ok( CZVOLUME* zvol );
	int  get_dinpsetup( char* key, int* par, CString* parstr );
	int	 get_dinpsetup( int key, int* par, CString* parstr );

	int  set_dinpsetup( char* key, int* par, CString* parstr );

// インプリメンテーション
public:
	virtual ~CDinpINI();
protected:
	struct _AUTOSEL *get_inpswtbl();

private:
	struct _AUTOSEL m_AUTOSEL;
	CDBZmSub	*m_pZm;
	int			m_User_id;
	char		rsv[124];
};

/////////////////////////////////////////////////////////////////////////////
#endif	// end of INCLUDE_DINPINI_H