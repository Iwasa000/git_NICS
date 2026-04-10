#pragma once

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo5_02Data
{
			
public:

	char*				rsv;

	unsigned	char	Sn_F502_1A[MONY_BUF_SIZE],				// 課税標準額に対する消費税額　税率3%適用分　A
						Sn_F502_1B[MONY_BUF_SIZE],				// 課税標準額に対する消費税額　税率4％適用分　B
						Sn_F502_1C[MONY_BUF_SIZE],				// 課税標準額に対する消費税額　税率6.3％適用分　C
						Sn_F502_1X[MONY_BUF_SIZE];				// 課税標準額に対する消費税額　旧税率分小計 X

	unsigned	char	Sn_F502_2A[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率3%適用分　A
						Sn_F502_2B[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率4％適用分　B
						Sn_F502_2C[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率6.3％適用分　C
						Sn_F502_2X[MONY_BUF_SIZE];				// 貸倒回収に係る消費税額　旧税率分小計 X

	unsigned	char	Sn_F502_3A[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額　税率3%適用分　A
						Sn_F502_3B[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額　税率4％適用分　B
						Sn_F502_3C[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額　税率6.3％適用分　C
						Sn_F502_3X[MONY_BUF_SIZE];				// 売上対価の返還等に係る消費税額　旧税率分小計 X

	unsigned	char	Sn_F502_4A[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額　税率3%適用分　A
						Sn_F502_4B[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額　税率4％適用分　B
						Sn_F502_4C[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額　税率6.3％適用分　C
						Sn_F502_4X[MONY_BUF_SIZE];				// 控除対象仕入税額の計算の基礎となる消費税額　旧税率分小計 X

	unsigned	char	Sn_F502_5A[MONY_BUF_SIZE],				// ４×みなし仕入れ率　税率3%適用分　A
						Sn_F502_5B[MONY_BUF_SIZE],				// ４×みなし仕入れ率　税率4％適用分　B
						Sn_F502_5C[MONY_BUF_SIZE],				// ４×みなし仕入れ率　税率6.3％適用分　C
						Sn_F502_5X[MONY_BUF_SIZE];				// ４×みなし仕入れ率　旧税率分小計 X
	int					Sn_F502_5kbn;							// みなし仕入率　1:90％...

	unsigned	char	Sn_F502_6A[MONY_BUF_SIZE],				// 事業区分別の合計額　税率3%適用分　A
						Sn_F502_6B[MONY_BUF_SIZE],				// 事業区分別の合計額　税率4％適用分　B
						Sn_F502_6C[MONY_BUF_SIZE],				// 事業区分別の合計額　税率6.3％適用分　C
						Sn_F502_6X[MONY_BUF_SIZE];				// 事業区分別の合計額　旧税率分小計 X
	int					Sn_F502_6sw;

	unsigned	char	Sn_F502_7A[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率3%適用分　A
						Sn_F502_7B[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率4％適用分　B
						Sn_F502_7C[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_7X[MONY_BUF_SIZE];				// 第一種事業(卸売業)　旧税率分小計 X

	unsigned	char	Sn_F502_8A[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率3%適用分　A
						Sn_F502_8B[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率4％適用分　B
						Sn_F502_8C[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率6.3％適用分　C
						Sn_F502_8X[MONY_BUF_SIZE];				// 第二種事業(小売業)　旧税率分小計 X

	unsigned	char	Sn_F502_9A[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率3%適用分　A
						Sn_F502_9B[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率4％適用分　B
						Sn_F502_9C[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率6.3％適用分　C
						Sn_F502_9X[MONY_BUF_SIZE];				// 第三種事業(製造業等)　旧税率分小計 X

	unsigned	char	Sn_F502_10A[MONY_BUF_SIZE],				// 第四種事業(その他)　税率3%適用分　A
						Sn_F502_10B[MONY_BUF_SIZE],				// 第四種事業(その他)　税率4％適用分　B
						Sn_F502_10C[MONY_BUF_SIZE],				// 第四種事業(その他)　税率6.3％適用分　C
						Sn_F502_10X[MONY_BUF_SIZE];				// 第四種事業(その他)　旧税率分小計 X

	unsigned	char	Sn_F502_11A[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率3%適用分　A
						Sn_F502_11B[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率4％適用分　B
						Sn_F502_11C[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率6.3％適用分　C
						Sn_F502_11X[MONY_BUF_SIZE];				// 第五種事業(サービス業等)　旧税率分小計 X

	unsigned	char	Sn_F502_12A[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率3%適用分　A
						Sn_F502_12B[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率4％適用分　B
						Sn_F502_12C[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率6.3％適用分　C
						Sn_F502_12X[MONY_BUF_SIZE];				// 第六種事業(不動産業)　旧税率分小計 X

	unsigned	char	Sn_F502_13A[MONY_BUF_SIZE],				// 事業区分別の合計額　税率3%適用分　A
						Sn_F502_13B[MONY_BUF_SIZE],				// 事業区分別の合計額　税率4％適用分　B
						Sn_F502_13C[MONY_BUF_SIZE],				// 事業区分別の合計額　税率6.3％適用分　C
						Sn_F502_13X[MONY_BUF_SIZE];				// 事業区分別の合計額　旧税率分小計 X

	unsigned	char	Sn_F502_14A[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率3%適用分　A（画面用）
						Sn_F502_14B[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率4％適用分　B
						Sn_F502_14C[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_14X[MONY_BUF_SIZE];				// 第一種事業(卸売業)　旧税率分小計 X
	int					Sn_F502_14sw;							// 手入力サイン

	unsigned	char	Sn_F502_15A[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率3%適用分　A
						Sn_F502_15B[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率4％適用分　B
						Sn_F502_15C[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率6.3％適用分　C
						Sn_F502_15X[MONY_BUF_SIZE];				// 第二種事業(小売業)　旧税率分小計 X
	int					Sn_F502_15sw;							// 手入力サイン

	unsigned	char	Sn_F502_16A[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率3%適用分　A
						Sn_F502_16B[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率4％適用分　B
						Sn_F502_16C[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率6.3％適用分　C
						Sn_F502_16X[MONY_BUF_SIZE];				// 第三種事業(製造業等)　旧税率分小計 X
	int					Sn_F502_16sw;							// 手入力サイン

	unsigned	char	Sn_F502_17A[MONY_BUF_SIZE],				// 第四種事業(その他)　税率3%適用分　A
						Sn_F502_17B[MONY_BUF_SIZE],				// 第四種事業(その他)　税率4％適用分　B
						Sn_F502_17C[MONY_BUF_SIZE],				// 第四種事業(その他)　税率6.3％適用分　C
						Sn_F502_17X[MONY_BUF_SIZE];				// 第四種事業(その他)　旧税率分小計 X
	int					Sn_F502_17sw;							// 手入力サイン

	unsigned	char	Sn_F502_18A[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率3%適用分　A
						Sn_F502_18B[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率4％適用分　B
						Sn_F502_18C[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率6.3％適用分　C
						Sn_F502_18X[MONY_BUF_SIZE];				// 第五種事業(サービス業等)　旧税率分小計 X
	int					Sn_F502_18sw;							// 手入力サイン

	unsigned	char	Sn_F502_19A[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率3%適用分　A
						Sn_F502_19B[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率4％適用分　B
						Sn_F502_19C[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率6.3％適用分　C
						Sn_F502_19X[MONY_BUF_SIZE];				// 第六種事業(不動産業)　旧税率分小計 X
	int					Sn_F502_19sw;							// 手入力サイン

	// 印刷用消費税額（0円以下を0とする）
	unsigned	char	Sn_F502_14AP[MONY_BUF_SIZE],			// 第一種事業(卸売業)　税率3%適用分　A（印刷用）
						Sn_F502_14BP[MONY_BUF_SIZE],			// 第一種事業(卸売業)　税率4％適用分　B
						Sn_F502_14CP[MONY_BUF_SIZE],			// 第一種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_14XP[MONY_BUF_SIZE];			// 第一種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F502_15AP[MONY_BUF_SIZE],			// 第二種事業(卸売業)　税率3%適用分　A
						Sn_F502_15BP[MONY_BUF_SIZE],			// 第二種事業(卸売業)　税率4％適用分　B
						Sn_F502_15CP[MONY_BUF_SIZE],			// 第二種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_15XP[MONY_BUF_SIZE];			// 第二種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F502_16AP[MONY_BUF_SIZE],			// 第三種事業(卸売業)　税率3%適用分　A
						Sn_F502_16BP[MONY_BUF_SIZE],			// 第三種事業(卸売業)　税率4％適用分　B
						Sn_F502_16CP[MONY_BUF_SIZE],			// 第三種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_16XP[MONY_BUF_SIZE];			// 第三種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F502_17AP[MONY_BUF_SIZE],			// 第四種事業(卸売業)　税率3%適用分　A
						Sn_F502_17BP[MONY_BUF_SIZE],			// 第四種事業(卸売業)　税率4％適用分　B
						Sn_F502_17CP[MONY_BUF_SIZE],			// 第四種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_17XP[MONY_BUF_SIZE];			// 第四種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F502_18AP[MONY_BUF_SIZE],			// 第五種事業(卸売業)　税率3%適用分　A
						Sn_F502_18BP[MONY_BUF_SIZE],			// 第五種事業(卸売業)　税率4％適用分　B
						Sn_F502_18CP[MONY_BUF_SIZE],			// 第五種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_18XP[MONY_BUF_SIZE];			// 第五種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F502_19AP[MONY_BUF_SIZE],			// 第六種事業(卸売業)　税率3%適用分　A
						Sn_F502_19BP[MONY_BUF_SIZE],			// 第六種事業(卸売業)　税率4％適用分　B
						Sn_F502_19CP[MONY_BUF_SIZE],			// 第六種事業(卸売業)　税率6.3％適用分　C
						Sn_F502_19XP[MONY_BUF_SIZE];			// 第六種事業(卸売業)　旧税率合計 X

private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	BOOL				m_isTransitionalMeasure;
	CArithEx			m_Arith;

protected:
	char				rsv1[512];


protected:
	char				rsv2[256];

public:
	CH31SnFhyo5_02Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo5_02Data();

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
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type);
	// 手入力サインのクリア
	int ClearManualInputSign(BOOL isChgKzritu);
	// DB上にデータ有り？
	bool IsExistData(CDBNpSub *pDBNpSub, int sn_seq);
};
