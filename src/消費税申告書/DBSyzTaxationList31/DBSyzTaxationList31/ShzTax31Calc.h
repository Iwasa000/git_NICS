#pragma once

// shzcalc.h
// 計算表集計クラス

class CShzTax31Calc{

public:
	CShzTax31Calc();
	~CShzTax31Calc();

public:

	// 初期化
	int	InitShzCalc(CH31TaxListData **pTaxListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub );
//	int	InitShzCalc( CH26TaxListData *pTaxListData, CSnHeadData *pSnHeadData, CDBNpSub *pZmSub );

//	static	CDBSyzShinView *pSyzShin;

// 22-0167,22-0361 cor -->
	//int	 Shz_LinCalc( KKVAL* pK, int mode=0 );	// 1ラインの集計
// -----------------------
	int	 Shz_LinCalc( KKVAL* pK, int mode=0, int mode2=0 );	// 1ラインの集計
// 22-0167,22-0361 cor <--

	int  Shz_JigyoCalc();		// 事業所得
	int  Shz_FudohCalc();		// 不動産所得
	int  Shz_NohgyoCalc();		// 農業所得

	int  Shz_SalePost();		// 課税売上高への転記
	int  Shz_BuyPost();			// 課税仕入高への転記

	int  Shz_SaleCalc();		// 課税売上高
	int  Shz_BuyCalc();			// 課税仕入高

// インボイス_23/07/24_higuchi del -->
// 未使用だったので削除
	//int	 Shz_KniInput();		// 簡易課税
// インボイス_23/07/24_higuchi del <--
	int	 Shz_KniPost();			// 課税売上高への転記（簡易課税）


	int	 Shz_DelData();
	int	 Shz_DelKkval( KKVAL* pK, int max );

	int	 Shz_ChkValChange( CString &str );
	int	 Shz_ChkKKauto( KKVAL* pK, int max );

	int	 Shz_shohizei( char* ans, char* val );
	int	 Shz_urcalc( char* ans, char* val );
	int	 Shz_sicalc( char* ans, char* val );


	int	 Shz_ReportPost( void );		// 非連動　申告書転記
	int	 Shz_FileRead( void );
	int	 Shz_FileWrite( void );
	int	 Shz_DataCpyUsu();
	int	 Shz_InvDataCpyUsu();			//('23.09.14 Tanaka Add)
	int	 Shz_DataCpyKni();
	int	 Shz_InvDataCpyKni();			//('23.09.26 Tanaka Add)


	//'14.08.20
	//------>経過措置用集計関数追加
	int  Shz_Buy31Post();							// 課税仕入高への転記（経過措置）
	int  Shz_Buy31Calc();							// 課税仕入高
	int  Shz_Sale31Post();							// 課税売上高への転記（経過措置）
	int  Shz_Sale31Calc();							// 課税売上高
	int	 Shz_shohizei4( char* ans, char* val );		//税率４％計算
	int	 Shz_shohizei63( char* ans, char* val );	//税率６．３％計算
	int	 Shz_shohizei624(char* ans, char* val);		//税率６．２４％計算
	int	 Shz_shohizei78(char* ans, char* val);		//税率７．８％計算
	int  Shz_31AllCalc( int mode = 0 );				// 計算表全集計（経過措置）
	int  Shz_Jigyo31Calc();							// 事業所得（経過措置）
	int	 Shz_Fudoh31Calc();							// 不動産所得（経過措置）
	int  Shz_Nohgyo31Calc();						// 農業所得（経過措置）

	//清水保留
	int	 Shz_KniKeiInput();							// 簡易課税（経過措置）
	int	 Shz_KniKeiPost();							// 課税売上高への転記（簡易課税） （経過措置）

	int	 Shz_shohizei100_105( char* ans, char* val );	//税率４％計算
	int	 Shz_shohizei100_108( char* ans, char* val );	//税率６．３％/６．２４計算
	int	 Shz_shohizei100_110(char* ans, char* val);		//税率７．８％計算
	int	 Shz_shohizei5_100( char* ans, char* val );		//税率４％計算
	int	 Shz_shohizei8_100( char* ans, char* val );		//税率６．３％計算
	int	 Shz_shohizei10_100(char* ans, char* val);		//税率７．８％計算
// インボイス_23/07/28_higuchi add -->
	int	 Shz_shohizei80_100( char* ans, char* val );	//税率６．３％計算
// インボイス_23/07/28_higuchi add <--
//----->('23.10.17 tanaka Add)
	int	 Shz_shohizei64_1016( char* ans, char* val );	//税率６．３％計算(免税事業者)
	int	 Shz_shohizei80_1020( char* ans, char* val );	//税率７．８％計算(免税事業者)
//<-----

	//清水保留
	int	 Shz_DataCpyKniKei();							// 非連動で簡易課税  申告書へ転記（経過措置用）
	int	 Shz_InvDataCpyKniKei();						// 非連動で簡易課税  申告書へ転記（インボイス期間の経過措置用）	'23.10.10 Add
	int  Shz_DataCpyUsuKei();							// 非連動で原則課税  申告書へ転記（経過措置用）
	int  Shz_InvDataCpyUsuKei();						// 非連動で原則課税  申告書へ転記（インボイス期間の経過措置用）	'23.10.10 Add

	//<----------------------------
	//23/07/12
// インボイス_23/07/24_higuchi del -->
	////------->適格請求書発行事業者以外の者から行った課税仕入に係る経過措置用集計関数追加
	//int  Shz_R5AllCalc(int mode = 0);				// 計算表全集計（経過措置）
	//int  Shz_JigyoR5Calc();							// 事業所得（経過措置）
	////<-------
// インボイス_23/07/24_higuchi del <--

	BOOL m_SixKindFlg;

private:
	CArithEx			m_Arith;			// 倍長演算
	CH31TaxListData		**m_pTaxListData;	// 課税取引金額計算表データ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
	CDBNpSub			*m_pZmSub;			// 公益クラス

	CSyzUtil			Util;				// ユーティリティクラス

public:
	SH_TENKIDATA31				tenkidata;			//'14.10.08
	SH_INVOICE_TENKIDATA31		inv_tenkidata;		//'23.09.14
};
