#pragma once

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo5_01Data
{
		
public:

	char*				rsv;


	unsigned	char	Sn_F501_1X[MONY_BUF_SIZE],				// 課税標準額に対する消費税額　旧税率分小計　X
						Sn_F501_1D[MONY_BUF_SIZE],				// 課税標準額に対する消費税額　税率6.24％適用分　D
						Sn_F501_1E[MONY_BUF_SIZE],				// 課税標準額に対する消費税額　税率7.8％適用分　E
						Sn_F501_1F[MONY_BUF_SIZE];				// 課税標準額に対する消費税額　合計 F

	unsigned	char	Sn_F501_2X[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　旧税率分小計　X
						Sn_F501_2D[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率6.24％適用分　D
						Sn_F501_2E[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率7.8％適用分　E
						Sn_F501_2F[MONY_BUF_SIZE];				// 貸倒回収に係る消費税額　合計 F

	unsigned	char	Sn_F501_3X[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額　旧税率分小計　X
						Sn_F501_3D[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額　税率6.24％適用分　D
						Sn_F501_3E[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額　税率7.8％適用分　E
						Sn_F501_3F[MONY_BUF_SIZE];				// 売上対価の返還等に係る消費税額　合計 F

	unsigned	char	Sn_F501_4X[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額　旧税率分小計　X
						Sn_F501_4D[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額　税率6.24％適用分　D
						Sn_F501_4E[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額　税率7.8％適用分　E
						Sn_F501_4F[MONY_BUF_SIZE];				// 控除対象仕入税額の計算の基礎となる消費税額　合計 F

	unsigned	char	Sn_F501_5X[MONY_BUF_SIZE],				// ④×みなし仕入率　旧税率分小計　X
						Sn_F501_5D[MONY_BUF_SIZE],				// ④×みなし仕入率　税率6.24％適用分　D
						Sn_F501_5E[MONY_BUF_SIZE],				// ④×みなし仕入率　税率7.8％適用分　E
						Sn_F501_5F[MONY_BUF_SIZE];				// ④×みなし仕入率　合計 F
	int					Sn_F501_5kbn;							// みなし仕入率　1:90％...

	unsigned	char	Sn_F501_6X[MONY_BUF_SIZE],				// 事業区分別の合計額　旧税率分小計　X
						Sn_F501_6D[MONY_BUF_SIZE],				// 事業区分別の合計額　税率6.24％適用分　D
						Sn_F501_6E[MONY_BUF_SIZE],				// 事業区分別の合計額　税率7.8％適用分　E
						Sn_F501_6F[MONY_BUF_SIZE];				// 事業区分別の合計額　合計 F
	int					Sn_F501_6sw;							// 手入力サイン

	unsigned	char	Sn_F501_7X[MONY_BUF_SIZE],				// 第一種事業(卸売業)　旧税率分小計　X
						Sn_F501_7D[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率6.24％適用分　D
						Sn_F501_7E[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_7F[MONY_BUF_SIZE];				// 第一種事業(卸売業)　合計 F

	unsigned	char	Sn_F501_8X[MONY_BUF_SIZE],				// 第二種事業(小売業)　旧税率分小計　X
						Sn_F501_8D[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率6.24％適用分　D
						Sn_F501_8E[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率7.8％適用分　E
						Sn_F501_8F[MONY_BUF_SIZE];				// 第二種事業(小売業)　合計 F

	unsigned	char	Sn_F501_9X[MONY_BUF_SIZE],				// 第三種事業(製造業等)　旧税率分小計　X
						Sn_F501_9D[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率6.24％適用分　D
						Sn_F501_9E[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率7.8％適用分　E
						Sn_F501_9F[MONY_BUF_SIZE];				// 第三種事業(製造業等)　合計 F

	unsigned	char	Sn_F501_10X[MONY_BUF_SIZE],				// 第四種事業(その他)　旧税率分小計　X
						Sn_F501_10D[MONY_BUF_SIZE],				// 第四種事業(その他)　税率6.24％適用分　D
						Sn_F501_10E[MONY_BUF_SIZE],				// 第四種事業(その他)　税率7.8％適用分　E
						Sn_F501_10F[MONY_BUF_SIZE];				// 第四種事業(その他)　合計 F

	unsigned	char	Sn_F501_11X[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　旧税率分小計　X
						Sn_F501_11D[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率6.24％適用分　D
						Sn_F501_11E[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率7.8％適用分　E
						Sn_F501_11F[MONY_BUF_SIZE];				// 第五種事業(サービス業等)　合計 F

	unsigned	char	Sn_F501_12X[MONY_BUF_SIZE],				// 第六種事業(不動産業)　旧税率分小計　X
						Sn_F501_12D[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率6.24％適用分　D
						Sn_F501_12E[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率7.8％適用分　E
						Sn_F501_12F[MONY_BUF_SIZE];				// 第六種事業(不動産業)　合計 F

	unsigned	char	Sn_F501_13X[MONY_BUF_SIZE],				// 事業区分別の合計額　旧税率分小計　X
						Sn_F501_13D[MONY_BUF_SIZE],				// 事業区分別の合計額　税率6.24％適用分　D
						Sn_F501_13E[MONY_BUF_SIZE],				// 事業区分別の合計額　税率7.8％適用分　E
						Sn_F501_13F[MONY_BUF_SIZE];				// 事業区分別の合計額　合計 F

	unsigned	char	Sn_F501_14X[MONY_BUF_SIZE],				// 第一種事業(卸売業)　旧税率分小計　X
						Sn_F501_14D[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率6.24％適用分　D（画面用）
						Sn_F501_14E[MONY_BUF_SIZE],				// 第一種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_14F[MONY_BUF_SIZE];				// 第一種事業(卸売業)　合計 F
	int					Sn_F501_14sw;							// 手入力サイン

	unsigned	char	Sn_F501_15X[MONY_BUF_SIZE],				// 第二種事業(小売業)　旧税率分小計　X
						Sn_F501_15D[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率6.24％適用分　D
						Sn_F501_15E[MONY_BUF_SIZE],				// 第二種事業(小売業)　税率7.8％適用分　E
						Sn_F501_15F[MONY_BUF_SIZE];				// 第二種事業(小売業)　合計 F
	int					Sn_F501_15sw;							// 手入力サイン

	unsigned	char	Sn_F501_16X[MONY_BUF_SIZE],				// 第三種事業(製造業等)　旧税率分小計　X
						Sn_F501_16D[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率6.24％適用分　D
						Sn_F501_16E[MONY_BUF_SIZE],				// 第三種事業(製造業等)　税率7.8％適用分　E
						Sn_F501_16F[MONY_BUF_SIZE];				// 第三種事業(製造業等)　合計 F
	int					Sn_F501_16sw;							// 手入力サイン

	unsigned	char	Sn_F501_17X[MONY_BUF_SIZE],				// 第四種事業(その他)　旧税率分小計　X
						Sn_F501_17D[MONY_BUF_SIZE],				// 第四種事業(その他)　税率6.24％適用分　D
						Sn_F501_17E[MONY_BUF_SIZE],				// 第四種事業(その他)　税率7.8％適用分　E
						Sn_F501_17F[MONY_BUF_SIZE];				// 第四種事業(その他)　合計 F
	int					Sn_F501_17sw;							// 手入力サイン

	unsigned	char	Sn_F501_18X[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　旧税率分小計　X
						Sn_F501_18D[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率6.24％適用分　D
						Sn_F501_18E[MONY_BUF_SIZE],				// 第五種事業(サービス業等)　税率7.8％適用分　E
						Sn_F501_18F[MONY_BUF_SIZE];				// 第五種事業(サービス業等)　合計 F
	int					Sn_F501_18sw;							// 手入力サイン

	unsigned	char	Sn_F501_19X[MONY_BUF_SIZE],				// 第六種事業(不動産業)　旧税率分小計　X
						Sn_F501_19D[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率6.24％適用分　D
						Sn_F501_19E[MONY_BUF_SIZE],				// 第六種事業(不動産業)　税率7.8％適用分　E
						Sn_F501_19F[MONY_BUF_SIZE];				// 第六種事業(不動産業)　合計 F
	int					Sn_F501_19sw;							// 手入力サイン

	unsigned short		Sn_F501_UP1,							// 第一種事業　売り上げ割合
						Sn_F501_UP2,							// 第二種事業　　　〃	
						Sn_F501_UP3,							// 第三種事業　　　〃	
						Sn_F501_UP4,							// 第四種事業　　　〃	
						Sn_F501_UP5,							// 第五種事業　　　〃	
						Sn_F501_UP6;							// 第六種事業　　　〃	

	// 印刷用消費税額（0円以下を0とする）
	unsigned	char	Sn_F501_14DP[MONY_BUF_SIZE],			// 第一種事業(卸売業)　税率6.24％適用分　D（印刷用）
						Sn_F501_14EP[MONY_BUF_SIZE],			// 第一種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_14XP[MONY_BUF_SIZE];			// 第一種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F501_15DP[MONY_BUF_SIZE],			// 第二種事業(卸売業)　税率6.24％適用分　D
						Sn_F501_15EP[MONY_BUF_SIZE],			// 第二種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_15XP[MONY_BUF_SIZE];			// 第二種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F501_16DP[MONY_BUF_SIZE],			// 第三種事業(卸売業)　税率6.24％適用分　D
						Sn_F501_16EP[MONY_BUF_SIZE],			// 第三種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_16XP[MONY_BUF_SIZE];			// 第三種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F501_17DP[MONY_BUF_SIZE],			// 第四種事業(卸売業)　税率6.24％適用分　D
						Sn_F501_17EP[MONY_BUF_SIZE],			// 第四種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_17XP[MONY_BUF_SIZE];			// 第四種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F501_18DP[MONY_BUF_SIZE],			// 第五種事業(卸売業)　税率6.24％適用分　D
						Sn_F501_18EP[MONY_BUF_SIZE],			// 第五種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_18XP[MONY_BUF_SIZE];			// 第五種事業(卸売業)　旧税率合計 X
	unsigned	char	Sn_F501_19DP[MONY_BUF_SIZE],			// 第六種事業(卸売業)　税率6.24％適用分　D
						Sn_F501_19EP[MONY_BUF_SIZE],			// 第六種事業(卸売業)　税率7.8％適用分　E
						Sn_F501_19XP[MONY_BUF_SIZE];			// 第六種事業(卸売業)　旧税率合計 X


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
	CH31SnFhyo5_01Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo5_01Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData ( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei, CSnHeadData *pSnHeadData );


public:
	// 更新時の金額クリア
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type);
	// 手入力サインのクリア
	int ClearManualInputSign(BOOL isChgKzritu);
	// DB上にデータ有り？
	bool IsExistData(CDBNpSub *pDBNpSub, int sn_seq);
};
