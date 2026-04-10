#pragma once

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo2_01Data
{

public:

	char*				rsv;

	unsigned	char	Sn_F201_1X[MONY_BUF_SIZE],				// 課税売上額（税抜き）　旧税率分小計　X
						Sn_F201_1D[MONY_BUF_SIZE],				// 課税売上額（税抜き）　税率6.24％適用分　D
						Sn_F201_1E[MONY_BUF_SIZE],				// 課税売上額（税抜き）　税率7.8％適用分　E
						Sn_F201_1F[MONY_BUF_SIZE];				// 課税売上額（税抜き）　合計 F

	unsigned	char	Sn_F201_2F[MONY_BUF_SIZE];				// 免税売上額　合計 F

	unsigned	char	Sn_F201_3F[MONY_BUF_SIZE];				// 非課税資産の輸出等の金額・・　合計 F

	unsigned	char	Sn_F201_4F[MONY_BUF_SIZE];				// 課税資産の譲渡等の対価の額（1+2+3）　合計 F

	unsigned	char	Sn_F201_5F[MONY_BUF_SIZE];				// 課税資産の譲渡等の対価の額(4の金額)　合計 F

	unsigned	char	Sn_F201_6F[MONY_BUF_SIZE];				// 非課税売上額	合計F

	unsigned	char	Sn_F201_7F[MONY_BUF_SIZE];				// 資産の譲渡等の対価の額　合計 F

	unsigned	char	Sn_F201_8F[MONY_BUF_SIZE];				// 課税売上割合　合計 F

	unsigned	char	Sn_F201_9X[MONY_BUF_SIZE],				// 課税仕入れに係る支払対価の額(税込み)　旧税率分小計　X
						Sn_F201_9D[MONY_BUF_SIZE],				// 課税仕入れに係る支払対価の額(税込み)　税率6.24％適用分　D
						Sn_F201_9E[MONY_BUF_SIZE],				// 課税仕入れに係る支払対価の額(税込み)　税率7.8％適用分　E
						Sn_F201_9F[MONY_BUF_SIZE];				// 課税仕入れに係る支払対価の額(税込み)　合計 F

	unsigned	char	Sn_F201_10X[MONY_BUF_SIZE],				// 課税仕入れに係る消費税額　旧税率分小計　X
						Sn_F201_10D[MONY_BUF_SIZE],				// 課税仕入れに係る消費税額　税率6.24％適用分　D
						Sn_F201_10E[MONY_BUF_SIZE],				// 課税仕入れに係る消費税額　税率7.8％適用分　E
						Sn_F201_10F[MONY_BUF_SIZE];				// 課税仕入れに係る消費税額　合計 F
	int					Sn_F201_10sw;							// 手入力サイン

	//----> 20230120
	unsigned	char	Sn_F201_27X[MONY_BUF_SIZE],				// 適格請求書発行事業者～…支払い対価の額(税込み)　旧税率分小計　X
						Sn_F201_27D[MONY_BUF_SIZE],				// 適格請求書発行事業者～…支払い対価の額(税込み)　税率6.24％適用分　D
						Sn_F201_27E[MONY_BUF_SIZE],				// 適格請求書発行事業者～…支払い対価の額(税込み)　税率7.8％適用分　E
						Sn_F201_27F[MONY_BUF_SIZE];				// 適格請求書発行事業者～…支払い対価の額(税込み)　合計 F

	unsigned	char	Sn_F201_28X[MONY_BUF_SIZE],				// 適格請求書発行事業者～…消費税額とみなされる額　旧税率分小計　X
						Sn_F201_28D[MONY_BUF_SIZE],				// 適格請求書発行事業者～…消費税額とみなされる額　税率6.24％適用分　D
						Sn_F201_28E[MONY_BUF_SIZE],				// 適格請求書発行事業者～…消費税額とみなされる額　税率7.8％適用分　E
						Sn_F201_28F[MONY_BUF_SIZE];				// 適格請求書発行事業者～…消費税額とみなされる額　合計 F
	int					Sn_F201_28sw;							// 手入力サイン
	//<---- 20230120

	unsigned	char	Sn_F201_11X[MONY_BUF_SIZE],				// 特定課税仕入れに係る支払い対価の額　旧税率分小計　X
						Sn_F201_11E[MONY_BUF_SIZE],				// 特定課税仕入れに係る支払い対価の額　税率7.8％適用分　E
						Sn_F201_11F[MONY_BUF_SIZE];				// 特定課税仕入れに係る支払い対価の額　合計 F

	unsigned	char	Sn_F201_12X[MONY_BUF_SIZE],				// 特定課税仕入れに係る消費税額　旧税率分小計　X
						Sn_F201_12E[MONY_BUF_SIZE],				// 特定課税仕入れに係る消費税額　税率7.8％適用分　E
						Sn_F201_12F[MONY_BUF_SIZE];				// 特定課税仕入れに係る消費税額　合計 F
	int					Sn_F201_12sw;							// 手入力サイン

	unsigned	char	Sn_F201_13X[MONY_BUF_SIZE],				// 課税貨物に係る消費税額　旧税率分小計　X
						Sn_F201_13D[MONY_BUF_SIZE],				// 課税貨物に係る消費税額　税率6.24％適用分　D
						Sn_F201_13E[MONY_BUF_SIZE],				// 課税貨物に係る消費税額　税率7.8％適用分　E
						Sn_F201_13F[MONY_BUF_SIZE];				// 課税貨物に係る消費税額　合計 F

	unsigned	char	Sn_F201_14X[MONY_BUF_SIZE],				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　旧税率分小計　X
						Sn_F201_14D[MONY_BUF_SIZE],				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率6.24％適用分　D
						Sn_F201_14E[MONY_BUF_SIZE],				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率7.8％適用分　E
						Sn_F201_14F[MONY_BUF_SIZE];				// 納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　合計 F

	unsigned	char	Sn_F201_15X[MONY_BUF_SIZE],				// 課税仕入れ等の税額の合計額　旧税率分小計　X
						Sn_F201_15D[MONY_BUF_SIZE],				// 課税仕入れ等の税額の合計額　税率6.24％適用分　D
						Sn_F201_15E[MONY_BUF_SIZE],				// 課税仕入れ等の税額の合計額　税率7.8％適用分　E
						Sn_F201_15F[MONY_BUF_SIZE];				// 課税仕入れ等の税額の合計額　合計 F

	unsigned	char	Sn_F201_16X[MONY_BUF_SIZE],				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　旧税率分小計　X
						Sn_F201_16D[MONY_BUF_SIZE],				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　税率6.24％適用分　D
						Sn_F201_16E[MONY_BUF_SIZE],				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　税率7.8％適用分　E
						Sn_F201_16F[MONY_BUF_SIZE];				// 課税売上高が5億円以下、かつ、課税売上割合が95%以上の場合　合計 F

	unsigned	char	Sn_F201_17X[MONY_BUF_SIZE],				// 15のうち、課税売上げにのみ要するもの　旧税率分小計　X
						Sn_F201_17D[MONY_BUF_SIZE],				// 15のうち、課税売上げにのみ要するもの　税率6.24％適用分　D
						Sn_F201_17E[MONY_BUF_SIZE],				// 15のうち、課税売上げにのみ要するもの　税率7.8％適用分　E
						Sn_F201_17F[MONY_BUF_SIZE];				// 15のうち、課税売上げにのみ要するもの　合計 F

	unsigned	char	Sn_F201_18X[MONY_BUF_SIZE],				// 15のうち、課税売上げと非課税売り上げに共通して要するもの　旧税率分小計　X
						Sn_F201_18D[MONY_BUF_SIZE],				// 15のうち、課税売上げと非課税売り上げに共通して要するもの　税率6.24％適用分　D
						Sn_F201_18E[MONY_BUF_SIZE],				// 15のうち、課税売上げと非課税売り上げに共通して要するもの　税率7.8％適用分　E
						Sn_F201_18F[MONY_BUF_SIZE];				// 15のうち、課税売上げと非課税売り上げに共通して要するもの　合計 F

	unsigned	char	Sn_F201_19X[MONY_BUF_SIZE],				// 個別対応方式により控除する課税仕入れ等の税額　旧税率分小計　X
						Sn_F201_19D[MONY_BUF_SIZE],				// 個別対応方式により控除する課税仕入れ等の税額　税率6.24％適用分　D
						Sn_F201_19E[MONY_BUF_SIZE],				// 個別対応方式により控除する課税仕入れ等の税額　税率7.8％適用分　E
						Sn_F201_19F[MONY_BUF_SIZE];				// 個別対応方式により控除する課税仕入れ等の税額　合計 F
	int					Sn_F201_19sw;							// 手入力サイン

	unsigned	char	Sn_F201_20X[MONY_BUF_SIZE],				// 一括比例配分方式により控除する課税仕入れ等の税額　旧税率分小計　X
						Sn_F201_20D[MONY_BUF_SIZE],				// 一括比例配分方式により控除する課税仕入れ等の税額　税率6.24％適用分　D
						Sn_F201_20E[MONY_BUF_SIZE],				// 一括比例配分方式により控除する課税仕入れ等の税額　税率7.8％適用分　E
						Sn_F201_20F[MONY_BUF_SIZE];				// 一括比例配分方式により控除する課税仕入れ等の税額　合計 F
	int					Sn_F201_20sw;							// 手入力サイン

	unsigned	char	Sn_F201_21X[MONY_BUF_SIZE],				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　旧税率分小計　X
						Sn_F201_21D[MONY_BUF_SIZE],				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　税率6.24％適用分　D
						Sn_F201_21E[MONY_BUF_SIZE],				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　税率7.8％適用分　E
						Sn_F201_21F[MONY_BUF_SIZE];				// 課税売上割合変動時の調整対象固定資産に係る消費税額の調整(加算又は減算)額　合計 F

	unsigned	char	Sn_F201_22X[MONY_BUF_SIZE],				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　旧税率分小計　X
						Sn_F201_22D[MONY_BUF_SIZE],				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　税率6.24％適用分　D
						Sn_F201_22E[MONY_BUF_SIZE],				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　税率7.8％適用分　E
						Sn_F201_22F[MONY_BUF_SIZE];				// 調整対象固定資産を課税業務用(非課税業務用)に転用した場合の調整(加算又は減算)額　合計 F

	unsigned	char	Sn_F201_26X[MONY_BUF_SIZE],				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 旧税率分小計　X
						Sn_F201_26D[MONY_BUF_SIZE],				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 税率7.8％適用分　D
						Sn_F201_26E[MONY_BUF_SIZE],				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 税率7.8％適用分　E
						Sn_F201_26F[MONY_BUF_SIZE];				// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額 合計 F


	unsigned	char	Sn_F201_23X[MONY_BUF_SIZE],				// 控除対象仕入税額　旧税率分小計　X
						Sn_F201_23D[MONY_BUF_SIZE],				// 控除対象仕入税額　税率6.24％適用分　D
						Sn_F201_23E[MONY_BUF_SIZE],				// 控除対象仕入税額　税率7.8％適用分　E
						Sn_F201_23F[MONY_BUF_SIZE];				// 控除対象仕入税額　合計 F
	int					Sn_F201_23sw;							// 手入力サイン

	unsigned	char	Sn_F201_24X[MONY_BUF_SIZE],				// 控除過大調整税額　旧税率分小計　X
						Sn_F201_24D[MONY_BUF_SIZE],				// 控除過大調整税額　税率6.24％適用分　D
						Sn_F201_24E[MONY_BUF_SIZE],				// 控除過大調整税額　税率7.8％適用分　E
						Sn_F201_24F[MONY_BUF_SIZE];				// 控除過大調整税額　合計 F

	unsigned	char	Sn_F201_25X[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　旧税率分小計　X
						Sn_F201_25D[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率6.24％適用分　D
						Sn_F201_25E[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率7.8％適用分　E
						Sn_F201_25F[MONY_BUF_SIZE];				// 貸倒回収に係る消費税額　合計 F

	unsigned short		Sn_F201_8EX;							// 8.F課税売上割合
	int					Sn_F201_8F_UW;							// 手入力された課税売上割合

protected:
	//char				rsv1[512];
	//char				rsv1[488];	// 居住用貸借建物を課税賃貸用に供した(譲渡した)場合の加算額追加 2020/04/21
									// Sn_F201_26X[MONY_BUF_SIZE]～Sn_F201_26F[MONY_BUF_SIZE]
	char				rsv1[436];	// 適格請求書発行事業者～…支払い対価の額(税込み) / 消費税額とみなされる額　追加 20230120
									// Sn_F201_27X[MONY_BUF_SIZE]～Sn_F201_28F[MONY_BUF_SIZE] + 手入力サイン


protected:
	char				rsv2[256];

private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	BOOL				m_isTransitionalMeasure;	//　経過措置あり？
	CArithEx			m_Arith;

public:
	CH31SnFhyo2_01Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo2_01Data();

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
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type, int ClearType);
	// 手入力サインのクリア
	int ClearManualInputSign(BOOL isChgKzritu);
	// DB上にデータ有り？
	bool IsExistData(CDBNpSub *pDBNpSub, int sn_seq);
};
