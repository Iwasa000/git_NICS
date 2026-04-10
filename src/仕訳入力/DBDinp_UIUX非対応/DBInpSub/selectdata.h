/*========================================================================
		選択画面用データ作成 ＆ 切替 
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
#ifndef INCLUDE_SELECTDATA_H
#define	INCLUDE_SELECTDATA_H


// 選択欄表示ポジション
#define	SL_KAMOKU_PN	0
#define	SL_KMKANA_PN	1
#define	SL_BRNTKY_PN	2
#define SL_BUMON_PN		3
#define SL_KOJI_PN		4
#define SL_TKY_PN		5
#define SL_TKYKANA_PN	6
#define SL_HJIYU_PN		7
#define SL_TKYALL_PN	8
#define SL_KOJIKANA_PN	9
#define SL_NPFURI_PN	10	// 公益 振替区分
#define SL_NPSYUS_PN	11	// 公益 収支科目
#define SL_NPSYKANA_PN	12	// 公益 収支カナ検索

#define	SL_KMINPCD_PN		13	// 入力コード [03.11 /09]
#define	SL_NPSKMINPCD_PN	14	// 公益 収支入力コード [03.11 /09]

#define SL_KMKODER_PN	15		// 科目順並び			//-091228
#define SL_NPODER_PN	16		// 公益法人収支科目順	//-091228

#define SL_NAIBU_PN		17		// 内部取引消去
#ifdef _SLIP_ACCOUNTS_
#define	SL_SLIP_TKY_PN	18	// 原票会計 摘要
#endif
#define SL_KMHANI_PN	19		// 科目範囲

#define SL_BMNKANA_PN	20		// 部門カナ検索

#define	SL_AITKKMK_PN	21		// 摘要からの科目履歴
#define	SL_BKAITKKMK_PN	22		// 摘要からの科目履歴(通帳)

#define SL_KMHANIKANA_PN	23	// 科目範囲 カナ
#define SL_INVNO_PN			24	// インボイス登録番号
#define SL_INVKANA_PN		25	// インボイス登録番号

#define SL_RESET		99

// 摘要選択モード
#define SL_TKY_RENSO	0
#define SL_TKY_SIWAKE	1

// 科目コード表示選択
#define SL_KMK_CODE		0	// 科目コード表示
#define SL_KMK_NUM		1	// 番号表示

// カナ順表示選択(カナ検索時)
#define SL_KJUN_DSP		1	// 科目コード表示

#define SL_NORMAL_MODE	0	// 標準のモード


// 枝番摘要 名称
// 枝番名称 や 諸口名称の 出力サインは BrnTkyDef.h の
//	BT_NOMALBRN_TEK		を使用する
//	BT_SYOGTBRN_TEK
//
#define SL_BRNTKY_ONLY	0x1000	// HOME キーで 枝番摘要 -> 科目選択にしない

// 個人科目表示制限
#define	SL_IND_IPPAN		0x01
#define	SL_IND_HUDOSAN		0x02
#define	SL_IND_NOGYO		0x04

#define SL_IND_ALL			0x1000

// 選択画面のバッファ
struct _SelTable {
	CString	Data;		// 表示テキスト
	int		number;		// 表示番号 (BIN)
};

// 選択画面にセットする関数に渡す構造体
typedef struct _SelSetProcPar
{
	int	sp_prevpn;	// 前回表示ポジション
	int	sp_nowpn;	// 今回表示ポジション

	char	seljob[40];
} SelSetProcPar;


// 選択パラメータ
typedef struct _SelPar
{
	int		sel_pn;			// 選択表示ポジション
	DWORD	sel_flg;		// 各選択に個別のサインをセット
							// 摘要枝番時、1 = 常に表示
							// AI摘要科目時: 0 借方, 1 貸方
	char	kmk_kana[10];	// 科目カナ

	// 摘要部分(借方･貸方科目は変動事由でも使用)
	CString dbt;
	CString cre;
	char	tky_kana[10];

	// 枝番摘要
	CString tkybrn;		//AI摘要科目時は、摘要文字列をセットする。
	int		brn_bmn;
	int		brn_cd;
	char	brn_kana[10];

} SelPar;


// 選択パラメータ拡張版
typedef struct _SelectParam
{
	int		sel_pn;			// 選択表示ポジション
	DWORD	sel_flg;		// 各選択に個別のサインをセット
							// 摘要枝番時、1 = 常に表示
							// AI摘要科目時: 0 借方, 1 貸方
	char	kmk_kana[10];	// 科目カナ

	// 摘要部分(借方･貸方科目は変動事由でも使用)
	CString dbt;
	CString cre;
	char	tky_kana[10];

	// 枝番摘要
	CString tkybrn;		//AI摘要科目時は、摘要文字列をセットする。
	int		brn_bmn;
	int		brn_cd;
	char	brn_kana[10];
	
	long	kmk_inpcd;

	// AI摘要の通帳対応
	int		bkcnt;		//GetTekiyoList2 対応
	int		bkinfo[4];	//
//	int		bankcode,	//銀行コード
//			branch,		//支店番号
//			kouza;		//口座番号

	char	dmy[256-8];

} SelectParam;




// 選択結果
typedef struct _SelResult 
{
	int		sel_pn;
	int		bmn;		//部門コード
	int		code;		//変動事由コード
	CString	kjcode;		//工事番号
	CString selname;	//部門名称・工事名称・変動事由名称
						//登録番号

	int		brn_bmn;	//枝番摘要(部門)
	CString km_code;	//枝番摘要科目コード
	int		brn_no;		//枝番摘要コード
	CString brn_name;	//枝番摘要名称

	int		tky_code;	//摘要コード
	CString tky;		//摘要名称

} SelResult;


// 枝番摘要 データ
typedef struct _BRNTKY_PAR
{
	int			bt_bmn;
	CString		bt_code;
	CString		bt_kana;
	int			bt_page;
	int			bt_reccnt;

	COLORREF	bt_txtcol;

} BRNTKY_PAR;


// スキャン用変動事由
typedef struct _KSH_SCAN
{
	DWORD	item;
	char	ks_name[60];

} KSH_SCAN;

// 変動事由名称
typedef struct _KSH_NAMDTA
{
	BYTE	hn_cod;		// 変動事由コード
	BYTE	hn_pflg;	// 出力サイン
	CString	hn_name;	// 事由名称

} KSH_NAMDTA;


// 公益法人用 振替科目
typedef struct _NP_FURIDTA
{
	WORD	nf_swno;		// 仕訳格納番号
	DWORD	nf_scod;		// 指定正味財産特殊コード

	BYTE	nf_att;			// 属性
	BYTE	nf_sg0;			// 特殊サイン

	char	nf_name[32];	// 選択名称

	DWORD	nf_sszc,		// 連動する正味財産増減計算書内の科目コード
			nf_sifc;		// 　　〃　　の一般振替科目コード
		
} NP_FURIDTA;

// 選択パラメータ拡張 ('09.03.11)
typedef struct _SelParEx
{
	long	kmk_inpcd;		// 科目入力コード
	char	dmy[28];

} SelParEx;

// 選択テーブルを コントロールにセットする 関数ポインタ
//	_SelTable* .... 選択結果テーブル
//	SelSetProcPar*	選択状態返送
//	int ........... 選択結果個数
//	CWnd* ........ CSelectData::Init() で渡した CWnd のポインタ
//
typedef int (CALLBACK* SELECTDATAPROC)( _SelTable*, SelSetProcPar*, int, CWnd* );


class CSelectSub;
#ifdef _SLIP_ACCOUNTS_
class CImgMng;
#endif
class AnalyzeAJ;

#ifdef _DBINPSUB
class AFX_EXT_CLASS CSelectData
#else
class CSelectData
#endif
{

public:
	CSelectData();
	~CSelectData();

public:
	int Init( CDBZmSub* pZm, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 );
	int Init( CDBNpSub* pZm, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 );

	void Close();
	void SetTekiyoMode( int mode );
	void SetKmDispMode( int mode );
	void SetBrnTkyMode( int mode );
	void SetKanaDspMode( int mode );
	void SetKamokuPageHold( BOOL bHold );

	void SetBrnSelDispMode( int mode );

	int GetTekiyoMode();
	int GetKmDispMode();
	int GetBrnTkyMode();
	int GetKanaDspMode();
	BOOL GetKamokuPageHold();

	void  SetSelOptMode( DWORD opt );
	DWORD GetSelOptMode();

	int	SelectJob( SelPar* par, int nChar );
	int SelectUpdate();

	struct _SelTable* GetNowFix8Select();
	struct _SelTable* GetNowSel32Select();

	COLORREF GetSel32TextColor();
	COLORREF GetSel32BackColor();

	BOOL InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick = FALSE );
	BOOL GetSelResult( SelResult* result, char* selstr );

	BOOL IsHenJiyuCodeRegisted( int h_code );
	BOOL GetJiyuMeisyo( BYTE jycode, char* pJynam );
	int  ScanKHKbnMake();
	int  GetScanKHKbn( int index, KSH_SCAN* pKshScan );

	int	 GetHjiyuCount(int sgn);
	const KSH_NAMDTA*	GetHjiyuData(int sgn, int index);

	BOOL GetKojiName( CString& kjcode, CString& name,BOOL bFull = FALSE );
	BOOL Kojicode_Ok( CString& kjcode );
	void tekiyo_update();

	BOOL IsNPfuridata( LPCTSTR deb, LPCTSTR cre );
	BOOL GetNPfuriStr( int frcode, CString& str );
	BOOL IsNPfuricode( int frcode );
	void NPfuriHani( DWORD& scod, DWORD& ecod );
	int	 GetNPfuriCount();
	const NP_FURIDTA*	GetNPfuriData(int index);

	BOOL NP_InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick = FALSE );

	BOOL GetNaibuString( int type, int nbcd, CString& str );
	BOOL IsNaibuData( LPCTSTR deb, LPCTSTR cre );

	// 科目の表示制限[個人]
	void KamokuRestrict( DWORD dwFlag );

	// 選択結果を表示するウィンドウのセット
	void SetWindow( CWnd* pwnd );

	BRNTKY_PAR GetBrnTkyPar();

	// 科目の表示制限[公益] ('08.12.01)
	void NpKamokuRestrict( DWORD Jgykcd, DWORD Knrkcd );

	// 科目入力コードセット用に拡張 ('09.03.11)
	int	SelectJob( SelPar* par, int nChar, SelParEx* parex );

	// 工事チェック用
	BOOL Kojicode_NoInp( CString kno, CString kmkcode );
	BOOL Kojicode_KanChk( CString kno, CString kmkcode );

	// 科目順セット用に拡張 ('09.03.11)
	void	OrderKamokuRestrict( int order );
	void	SetOrder( int order, int old_order = -1 );

	int		BrnTekClear();
	void	ResetTkallPage();

	// AI摘要科目クラス
	void	SetAITkmkClass(AnalyzeAJ *pAnaAJ);
	int		AITKkmkCount(CString tkstr, int dcsw);
	int		BankAITKkmkCount(int* bankseq, int bncnt, CString tkstr, int dcsw);
	// 科目入力コードセット用に拡張 ('09.03.11)
	int		SelectJob(SelectParam* par, int nChar);

	void	RenewInvno();

#ifdef _SLIP_ACCOUNTS_
	// イメージ文字列取得用に拡張
	void	SetImgMng( CImgMng *pImgMng );
#endif
private:
	CSelectSub* m_pSub;


};




#endif