#pragma once

// shzcalc.h
// 計算表集計クラス

class CShzCalc{

public:
	CShzCalc();
	~CShzCalc();

public:

	// 初期化
	int	InitShzCalc( CH26TaxListData **pTaxListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub );
//	int	InitShzCalc( CH26TaxListData *pTaxListData, CSnHeadData *pSnHeadData, CDBNpSub *pZmSub );

//	static	CDBSyzShinView *pSyzShin;

	int  Shz_AllCalc( int mode=0 );
	int	 Shz_LinCalc( KKVAL* pK, int mode=0 );	// 1ラインの集計

	int  Shz_JigyoCalc();		// 事業所得
	int  Shz_FudohCalc();		// 不動産所得
	int  Shz_NohgyoCalc();		// 農業所得

	int  Shz_SalePost();		// 課税売上高への転記
	int  Shz_BuyPost();			// 課税仕入高への転記

	int  Shz_SaleCalc();		// 課税売上高
	int  Shz_BuyCalc();			// 課税仕入高

	int	 Shz_KniInput();		// 簡易課税
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
	int	 Shz_DataCpyKni();


	//'14.08.20
	//------>経過措置用集計関数追加
	int  Shz_BuyKeiPost();							// 課税仕入高への転記（経過措置）
	int  Shz_BuyKeiCalc();							// 課税仕入高(経過措置)
	int  Shz_SaleKeiPost();							// 課税売上高への転記（経過措置）
	int  Shz_SaleKeiCalc();							// 課税売上高(経過措置)
	int	 Shz_shohizei4( char* ans, char* val );		//税率４％計算
	int	 Shz_shohizei63( char* ans, char* val );	//税率６．３％計算
	int  Shz_KeiAllCalc( int mode = 0 );				
	int  Shz_JigyoKeiCalc();						// 事業所得（経過措置）
	int	 Shz_FudohKeiCalc();						// 不動産所得（経過措置）
	int  Shz_NohgyoKeiCalc();						// 農業所得（経過措置）

	int	 Shz_KniKeiInput();							// 簡易課税（経過措置）
	int	 Shz_KniKeiPost();							// 課税売上高への転記（簡易課税） （経過措置）

	int	 Shz_shohizei100_105( char* ans, char* val );	//税率４％計算
	int	 Shz_shohizei100_108( char* ans, char* val );	//税率６．３％計算
	int	 Shz_shohizei5_100( char* ans, char* val );		//税率４％計算
	int	 Shz_shohizei8_100( char* ans, char* val );		//税率６．３％計算
	int	 Shz_DataCpyKniKei();							// 非連動で原則課税  申告書へ転記（経過措置用）
	int  Shz_DataCpyUsuKei();							// 非連動で原則課税  申告書へ転記（経過措置用）

	//<----------------------------

//2016.10.20 INSERT START
	//EnumIdIcsShRevType	m_ShRevType;
	BOOL m_SixKindFlg;
//2016.10.20 INSERT END

private:
	CArithEx			m_Arith;			// 倍長演算
	CH26TaxListData		**m_pTaxListData;	// 課税取引金額計算表データ
//	CH26TaxListData		*m_pTaxListData;	// 課税取引金額計算表データ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
//	CSnHeadData			*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub			*m_pZmSub;			// 公益クラス

	CSyzUtil			Util;				// ユーティリティクラス

public:
	SH_TENKIDATA		tenkidata;			//'14.10.08
};
