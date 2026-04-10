#pragma once

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo2_02Data
{
	
public:

	char*				rsv;

	unsigned	char	Sn_F202_1A[MONY_BUF_SIZE],				// 課税売上額（税抜き）　税率3%適用分　A
						Sn_F202_1B[MONY_BUF_SIZE],				// 課税売上額（税抜き）　税率4％適用分　B
						Sn_F202_1C[MONY_BUF_SIZE],				// 課税売上額（税抜き）　税率6.3％適用分　C
						Sn_F202_1X[MONY_BUF_SIZE];				// 課税売上額（税抜き）　旧税率分小計 X

	unsigned	char	Sn_F202_4X[MONY_BUF_SIZE];				// 課税資産の譲渡等の対価の額（1+2+3）　旧税率分小計 X

	unsigned	char	Sn_F202_7X[MONY_BUF_SIZE];				// 資産の譲渡等の対価の額　旧税率分小計 X

	unsigned	char	Sn_F202_8X[MONY_BUF_SIZE];				// 課税売上割合　旧税率分小計 X

	unsigned	char	Sn_F202_9A[MONY_BUF_SIZE],				// 課税仕入れに係る支払い対価の額(税込み)　税率3%適用分　A
						Sn_F202_9B[MONY_BUF_SIZE],				// 課税仕入れに係る支払い対価の額(税込み)　税率4％適用分　B
						Sn_F202_9C[MONY_BUF_SIZE],				// 課税仕入れに係る支払い対価の額(税込み)　税率6.3％適用分　C
						Sn_F202_9X[MONY_BUF_SIZE];				// 課税仕入れに係る支払い対価の額(税込み)　旧税率分小計 X

	unsigned	char	Sn_F202_10A[MONY_BUF_SIZE],				// 課税仕入れに係る消費税額　税率3%適用分　A
						Sn_F202_10B[MONY_BUF_SIZE],				// 課税仕入れに係る消費税額　税率4％適用分　B
						Sn_F202_10C[MONY_BUF_SIZE],				// 課税仕入れに係る消費税額　税率6.3％適用分　C
						Sn_F202_10X[MONY_BUF_SIZE];				// 課税仕入れに係る消費税額　旧税率分小計 X
	int					Sn_F202_10sw;							// 手入力サイン

	//----> 20230120
	unsigned	char	Sn_F202_27A[MONY_BUF_SIZE],				// 適格請求書発行事業者～…支払い対価の額(税込み)　税率3%適用分　A
						Sn_F202_27B[MONY_BUF_SIZE],				// 適格請求書発行事業者～…支払い対価の額(税込み)　税率4％適用分　B
						Sn_F202_27C[MONY_BUF_SIZE],				// 適格請求書発行事業者～…支払い対価の額(税込み)　税率6.3％適用分　C
						Sn_F202_27X[MONY_BUF_SIZE];				// 適格請求書発行事業者～…支払い対価の額(税込み)　旧税率分小計 X

	unsigned	char	Sn_F202_28A[MONY_BUF_SIZE],				// 適格請求書発行事業者～…消費税額とみなされる額　税率3%適用分　A
						Sn_F202_28B[MONY_BUF_SIZE],				// 適格請求書発行事業者～…消費税額とみなされる額　税率4％適用分　B
						Sn_F202_28C[MONY_BUF_SIZE],				// 適格請求書発行事業者～…消費税額とみなされる額　税率6.3％適用分　C
						Sn_F202_28X[MONY_BUF_SIZE];				// 適格請求書発行事業者～…消費税額とみなされる額　旧税率分小計 X
	int					Sn_F202_28sw;							// 手入力サイン
	//<---- 20230120

	unsigned	char	Sn_F202_11C[MONY_BUF_SIZE],				// 特定課税仕入れに係る支払い対価の額　税率6.3％適用分　C
						Sn_F202_11X[MONY_BUF_SIZE];				// 特定課税仕入れに係る支払い対価の額　旧税率分小計 X

	unsigned	char	Sn_F202_12C[MONY_BUF_SIZE],				// 特定課税仕入れに係る消費税額　税率6.3％適用分　C
						Sn_F202_12X[MONY_BUF_SIZE];				// 特定課税仕入れに係る消費税額　旧税率分小計 X
	int					Sn_F202_12sw;							// 手入力サイン

	// 課税貨物に係る消費税額 3% 4%の仕訳が発生することがないため、金額が入ることはない
	// ただしカットするとクラスサイズが変わるため、コメントアウトせずに置いている
	unsigned	char	Sn_F202_13A[MONY_BUF_SIZE],				// 課税貨物に係る消費税額　税率3%適用分　A
						Sn_F202_13B[MONY_BUF_SIZE],				// 課税貨物に係る消費税額　税率4％適用分　B
						Sn_F202_13C[MONY_BUF_SIZE],				// 課税貨物に係る消費税額　税率6.3％適用分　C
						Sn_F202_13X[MONY_BUF_SIZE];				// 課税貨物に係る消費税額　旧税率分小計 X

	unsigned	char	Sn_F202_14A[MONY_BUF_SIZE],				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率3%適用分　A
						Sn_F202_14B[MONY_BUF_SIZE],				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率4％適用分　B
						Sn_F202_14C[MONY_BUF_SIZE],				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率6.3％適用分　C
						Sn_F202_14X[MONY_BUF_SIZE];				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　旧税率分小計 X

	unsigned	char	Sn_F202_15A[MONY_BUF_SIZE],				// 課税仕入れ等の税額の合計額　税率3%適用分　A
						Sn_F202_15B[MONY_BUF_SIZE],				// 課税仕入れ等の税額の合計額　税率4％適用分　B
						Sn_F202_15C[MONY_BUF_SIZE],				// 課税仕入れ等の税額の合計額　税率6.3％適用分　C
						Sn_F202_15X[MONY_BUF_SIZE];				// 課税仕入れ等の税額の合計額　旧税率分小計 X

	unsigned	char	Sn_F202_16A[MONY_BUF_SIZE],				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　税率3%適用分　A
						Sn_F202_16B[MONY_BUF_SIZE],				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　税率4％適用分　B
						Sn_F202_16C[MONY_BUF_SIZE],				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　税率6.3％適用分　C
						Sn_F202_16X[MONY_BUF_SIZE];				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　旧税率分小計 X

	unsigned	char	Sn_F202_17A[MONY_BUF_SIZE],				// 15のうち、課税売上にのみ要するもの　税率3%適用分　A
						Sn_F202_17B[MONY_BUF_SIZE],				// 15のうち、課税売上にのみ要するもの　税率4％適用分　B
						Sn_F202_17C[MONY_BUF_SIZE],				// 15のうち、課税売上にのみ要するもの　税率6.3％適用分　C
						Sn_F202_17X[MONY_BUF_SIZE];				// 15のうち、課税売上にのみ要するもの　旧税率分小計 X

	unsigned	char	Sn_F202_18A[MONY_BUF_SIZE],				// 15のうち、課税売上と非課税売り上げに共通して要するもの　税率3%適用分　A
						Sn_F202_18B[MONY_BUF_SIZE],				// 15のうち、課税売上と非課税売り上げに共通して要するもの　税率4％適用分　B
						Sn_F202_18C[MONY_BUF_SIZE],				// 15のうち、課税売上と非課税売り上げに共通して要するもの　税率6.3％適用分　C
						Sn_F202_18X[MONY_BUF_SIZE];				// 15のうち、課税売上と非課税売り上げに共通して要するもの　旧税率分小計 X

	unsigned	char	Sn_F202_19A[MONY_BUF_SIZE],				// 個別対応方式により控除する課税仕入れ等の税額　税率3%適用分　A
						Sn_F202_19B[MONY_BUF_SIZE],				// 個別対応方式により控除する課税仕入れ等の税額　税率4％適用分　B
						Sn_F202_19C[MONY_BUF_SIZE],				// 個別対応方式により控除する課税仕入れ等の税額　税率6.3％適用分　C
						Sn_F202_19X[MONY_BUF_SIZE];				// 個別対応方式により控除する課税仕入れ等の税額　旧税率分小計 X
	int					Sn_F202_19sw;							// 手入力サイン

	unsigned	char	Sn_F202_20A[MONY_BUF_SIZE],				// 一括比例配分方式により控除する課税仕入れ等の税額　税率3%適用分　A
						Sn_F202_20B[MONY_BUF_SIZE],				// 一括比例配分方式により控除する課税仕入れ等の税額　税率4％適用分　B
						Sn_F202_20C[MONY_BUF_SIZE],				// 一括比例配分方式により控除する課税仕入れ等の税額　税率6.3％適用分　C
						Sn_F202_20X[MONY_BUF_SIZE];				// 一括比例配分方式により控除する課税仕入れ等の税額　旧税率分小計 X
	int					Sn_F202_20sw;							// 手入力サイン

	unsigned	char	Sn_F202_21A[MONY_BUF_SIZE],				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　税率3%適用分　A
						Sn_F202_21B[MONY_BUF_SIZE],				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　税率4％適用分　B
						Sn_F202_21C[MONY_BUF_SIZE],				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　税率6.3％適用分　C
						Sn_F202_21X[MONY_BUF_SIZE];				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　旧税率分小計 X

	unsigned	char	Sn_F202_22A[MONY_BUF_SIZE],				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　税率3%適用分　A
						Sn_F202_22B[MONY_BUF_SIZE],				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　税率4％適用分　B
						Sn_F202_22C[MONY_BUF_SIZE],				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　税率6.3％適用分　C
						Sn_F202_22X[MONY_BUF_SIZE];				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　旧税率分小計 X

	unsigned	char	Sn_F202_26A[MONY_BUF_SIZE],				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 税率3%適用分　A
						Sn_F202_26B[MONY_BUF_SIZE],				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 税率4％適用分　B
						Sn_F202_26C[MONY_BUF_SIZE],				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 税率6.3％適用分　C
						Sn_F202_26X[MONY_BUF_SIZE];				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 旧税率分小計 X

	unsigned	char	Sn_F202_23A[MONY_BUF_SIZE],				// 控除対象仕入税額　税率3%適用分　A
						Sn_F202_23B[MONY_BUF_SIZE],				// 控除対象仕入税額　税率4％適用分　B
						Sn_F202_23C[MONY_BUF_SIZE],				// 控除対象仕入税額　税率6.3％適用分　C
						Sn_F202_23X[MONY_BUF_SIZE];				// 控除対象仕入税額　旧税率分小計 X
	int					Sn_F202_23sw;							// 手入力サイン

	unsigned	char	Sn_F202_24A[MONY_BUF_SIZE],				// 控除過大調整税額　税率3%適用分　A
						Sn_F202_24B[MONY_BUF_SIZE],				// 控除過大調整税額　税率4％適用分　B
						Sn_F202_24C[MONY_BUF_SIZE],				// 控除過大調整税額　税率6.3％適用分　C
						Sn_F202_24X[MONY_BUF_SIZE];				// 控除過大調整税額　旧税率分小計 X

	unsigned	char	Sn_F202_25A[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率3%適用分　A
						Sn_F202_25B[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率4％適用分　B
						Sn_F202_25C[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率6.3％適用分　C
						Sn_F202_25X[MONY_BUF_SIZE];				// 貸倒回収に係る消費税額　旧税率分小計 X

	unsigned short		Sn_F202_8EX;							// 8.X課税売上割合
	int					Sn_F202_8X_UW;							// 手入力された課税売上割合

private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	BOOL				m_isTransitionalMeasure;	// 経過措置あり？
	CArithEx			m_Arith;
	
protected:
	//char				rsv1[512];
	//char				rsv1[488];	// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額追加
									// Sn_F202_26A[MONY_BUF_SIZE]～Sn_F202_26X[MONY_BUF_SIZE]
	char				rsv1[436];	// 適格請求書発行事業者～…支払い対価の額(税込み) / 消費税額とみなされる額　追加 20230120
									// Sn_F202_27A[MONY_BUF_SIZE]～Sn_F202_28X[MONY_BUF_SIZE] + 手入力サイン


protected:
	char				rsv2[256];

public:
	CH31SnFhyo2_02Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo2_02Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData ( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );
	// 旧様式のデータ取得
	int GetPastData ( CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData );

public:
	// 更新時の金額クリア
	// 廃止：今後はint ClearTypeの引数が追加された関数を使用する
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type);
	// 手入力サインのクリア
	int ClearManualInputSign(BOOL isChgKzritu);
	// DB上にデータ有り？
	bool IsExistData(CDBNpSub *pDBNpSub, int sn_seq);
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type, int ClearType );
};
	