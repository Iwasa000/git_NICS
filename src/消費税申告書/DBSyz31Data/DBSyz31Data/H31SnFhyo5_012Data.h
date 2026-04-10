#pragma once

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo5_012Data
{
				
public:

	char*				rsv;

	unsigned	char	Sn_F5012_20X[MONY_BUF_SIZE],				// 4×みなし仕入率　旧税率分小計　X
						Sn_F5012_20D[MONY_BUF_SIZE],				// 4×みなし仕入率　税率6.24％適用分　D
						Sn_F5012_20E[MONY_BUF_SIZE],				// 4×みなし仕入率　税率7.8％適用分　E
						Sn_F5012_20F[MONY_BUF_SIZE];				// 4×みなし仕入率　合計 F
	int					Sn_F5012_20sw;								// 手入力サイン

	unsigned	char	Sn_F5012_21X[MONY_BUF_SIZE],				// 4×みなし仕入率　旧税率分小計　X
						Sn_F5012_21D[MONY_BUF_SIZE],				// 4×みなし仕入率　税率6.24％適用分　D
						Sn_F5012_21E[MONY_BUF_SIZE],				// 4×みなし仕入率　税率7.8％適用分　E
						Sn_F5012_21F[MONY_BUF_SIZE];				// 4×みなし仕入率　合計 F
	int					Sn_F5012_21sw;								// 手入力サイン
	int					Sn_F5012_21kbn;								// みなし仕入率　1:90％...

	unsigned	char	Sn_F5012_22X[MONY_BUF_SIZE],				// 第一種事業及び第二種事業　旧税率分小計　X
						Sn_F5012_22D[MONY_BUF_SIZE],				// 第一種事業及び第二種事業　税率6.24％適用分　D
						Sn_F5012_22E[MONY_BUF_SIZE],				// 第一種事業及び第二種事業　税率7.8％適用分　E
						Sn_F5012_22F[MONY_BUF_SIZE];				// 第一種事業及び第二種事業　合計 F
	int					Sn_F5012_22sw;								// 手入力サイン

	unsigned	char	Sn_F5012_23X[MONY_BUF_SIZE],				// 第一種事業及び第三種事業　旧税率分小計　X
						Sn_F5012_23D[MONY_BUF_SIZE],				// 第一種事業及び第三種事業　税率6.24％適用分　D
						Sn_F5012_23E[MONY_BUF_SIZE],				// 第一種事業及び第三種事業　税率7.8％適用分　E
						Sn_F5012_23F[MONY_BUF_SIZE];				// 第一種事業及び第三種事業　合計 F
	int					Sn_F5012_23sw;								// 手入力サイン

	unsigned	char	Sn_F5012_24X[MONY_BUF_SIZE],				// 第一種事業及び第四種事業　旧税率分小計　X
						Sn_F5012_24D[MONY_BUF_SIZE],				// 第一種事業及び第四種事業　税率6.24％適用分　D
						Sn_F5012_24E[MONY_BUF_SIZE],				// 第一種事業及び第四種事業　税率7.8％適用分　E
						Sn_F5012_24F[MONY_BUF_SIZE];				// 第一種事業及び第四種事業　合計 F
	int					Sn_F5012_24sw;								// 手入力サイン

	unsigned	char	Sn_F5012_25X[MONY_BUF_SIZE],				// 第一種事業及び第五種事業　旧税率分小計　X
						Sn_F5012_25D[MONY_BUF_SIZE],				// 第一種事業及び第五種事業　税率6.24％適用分　D
						Sn_F5012_25E[MONY_BUF_SIZE],				// 第一種事業及び第五種事業　税率7.8％適用分　E
						Sn_F5012_25F[MONY_BUF_SIZE];				// 第一種事業及び第五種事業　合計 F
	int					Sn_F5012_25sw;								// 手入力サイン

	unsigned	char	Sn_F5012_26X[MONY_BUF_SIZE],				// 第一種事業及び第六種事業　旧税率分小計　X
						Sn_F5012_26D[MONY_BUF_SIZE],				// 第一種事業及び第六種事業　税率6.24％適用分　D
						Sn_F5012_26E[MONY_BUF_SIZE],				// 第一種事業及び第六種事業　税率7.8％適用分　E
						Sn_F5012_26F[MONY_BUF_SIZE];				// 第一種事業及び第六種事業　合計 F
	int					Sn_F5012_26sw;								// 手入力サイン

	unsigned	char	Sn_F5012_27X[MONY_BUF_SIZE],				// 第二種事業及び第三種事業　旧税率分小計　X
						Sn_F5012_27D[MONY_BUF_SIZE],				// 第二種事業及び第三種事業　税率6.24％適用分　D
						Sn_F5012_27E[MONY_BUF_SIZE],				// 第二種事業及び第三種事業　税率7.8％適用分　E
						Sn_F5012_27F[MONY_BUF_SIZE];				// 第二種事業及び第三種事業　合計 F
	int					Sn_F5012_27sw;								// 手入力サイン

	unsigned	char	Sn_F5012_28X[MONY_BUF_SIZE],				// 第二種事業及び第四種事業　旧税率分小計　X
						Sn_F5012_28D[MONY_BUF_SIZE],				// 第二種事業及び第四種事業　税率6.24％適用分　D
						Sn_F5012_28E[MONY_BUF_SIZE],				// 第二種事業及び第四種事業　税率7.8％適用分　E
						Sn_F5012_28F[MONY_BUF_SIZE];				// 第二種事業及び第四種事業　合計 F
	int					Sn_F5012_28sw;								// 手入力サイン

	unsigned	char	Sn_F5012_29X[MONY_BUF_SIZE],				// 第二種事業及び第五種事業　旧税率分小計　X
						Sn_F5012_29D[MONY_BUF_SIZE],				// 第二種事業及び第五種事業　税率6.24％適用分　D
						Sn_F5012_29E[MONY_BUF_SIZE],				// 第二種事業及び第五種事業　税率7.8％適用分　E
						Sn_F5012_29F[MONY_BUF_SIZE];				// 第二種事業及び第五種事業　合計 F
	int					Sn_F5012_29sw;								// 手入力サイン

	unsigned	char	Sn_F5012_30X[MONY_BUF_SIZE],				// 第二種事業及び第六種事業　旧税率分小計　X
						Sn_F5012_30D[MONY_BUF_SIZE],				// 第二種事業及び第六種事業　税率6.24％適用分　D
						Sn_F5012_30E[MONY_BUF_SIZE],				// 第二種事業及び第六種事業　税率7.8％適用分　E
						Sn_F5012_30F[MONY_BUF_SIZE];				// 第二種事業及び第六種事業　合計 F
	int					Sn_F5012_30sw;								// 手入力サイン

	unsigned	char	Sn_F5012_31X[MONY_BUF_SIZE],				// 第三種事業及び第四種事業　旧税率分小計　X
						Sn_F5012_31D[MONY_BUF_SIZE],				// 第三種事業及び第四種事業　税率6.24％適用分　D
						Sn_F5012_31E[MONY_BUF_SIZE],				// 第三種事業及び第四種事業　税率7.8％適用分　E
						Sn_F5012_31F[MONY_BUF_SIZE];				// 第三種事業及び第四種事業　合計 F
	int					Sn_F5012_31sw;								// 手入力サイン

	unsigned	char	Sn_F5012_32X[MONY_BUF_SIZE],				// 第三種事業及び第五種事業　旧税率分小計　X
						Sn_F5012_32D[MONY_BUF_SIZE],				// 第三種事業及び第五種事業　税率6.24％適用分　D
						Sn_F5012_32E[MONY_BUF_SIZE],				// 第三種事業及び第五種事業　税率7.8％適用分　E
						Sn_F5012_32F[MONY_BUF_SIZE];				// 第三種事業及び第五種事業　合計 F
	int					Sn_F5012_32sw;								// 手入力サイン

	unsigned	char	Sn_F5012_33X[MONY_BUF_SIZE],				// 第三種事業及び第六種事業　旧税率分小計　X
						Sn_F5012_33D[MONY_BUF_SIZE],				// 第三種事業及び第六種事業　税率6.24％適用分　D
						Sn_F5012_33E[MONY_BUF_SIZE],				// 第三種事業及び第六種事業　税率7.8％適用分　E
						Sn_F5012_33F[MONY_BUF_SIZE];				// 第三種事業及び第六種事業　合計 F
	int					Sn_F5012_33sw;								// 手入力サイン

	unsigned	char	Sn_F5012_34X[MONY_BUF_SIZE],				// 第四種事業及び第五種事業　旧税率分小計　X
						Sn_F5012_34D[MONY_BUF_SIZE],				// 第四種事業及び第五種事業　税率6.24％適用分　D
						Sn_F5012_34E[MONY_BUF_SIZE],				// 第四種事業及び第五種事業　税率7.8％適用分　E
						Sn_F5012_34F[MONY_BUF_SIZE];				// 第四種事業及び第五種事業　合計 F
	int					Sn_F5012_34sw;								// 手入力サイン

	unsigned	char	Sn_F5012_35X[MONY_BUF_SIZE],				// 第四種事業及び第六種事業　旧税率分小計　X
						Sn_F5012_35D[MONY_BUF_SIZE],				// 第四種事業及び第六種事業　税率6.24％適用分　D
						Sn_F5012_35E[MONY_BUF_SIZE],				// 第四種事業及び第六種事業　税率7.8％適用分　E
						Sn_F5012_35F[MONY_BUF_SIZE];				// 第四種事業及び第六種事業　合計 F
	int					Sn_F5012_35sw;								// 手入力サイン

	unsigned	char	Sn_F5012_36X[MONY_BUF_SIZE],				// 第五種事業及び第六種事業　旧税率分小計　X
						Sn_F5012_36D[MONY_BUF_SIZE],				// 第五種事業及び第六種事業　税率6.24％適用分　D
						Sn_F5012_36E[MONY_BUF_SIZE],				// 第五種事業及び第六種事業　税率7.8％適用分　E
						Sn_F5012_36F[MONY_BUF_SIZE];				// 第五種事業及び第六種事業　合計 F
	int					Sn_F5012_36sw;								// 手入力サイン

	unsigned	char	Sn_F5012_37X[MONY_BUF_SIZE],				// 選択可能な計算式区分の内から選択した金額　旧税率分小計　X
						Sn_F5012_37D[MONY_BUF_SIZE],				// 選択可能な計算式区分の内から選択した金額　税率6.24％適用分　D
						Sn_F5012_37E[MONY_BUF_SIZE],				// 選択可能な計算式区分の内から選択した金額　税率7.8％適用分　E
						Sn_F5012_37F[MONY_BUF_SIZE];				// 選択可能な計算式区分の内から選択した金額　合計 F

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
	CH31SnFhyo5_012Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo5_012Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData ( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );


public:
	// 更新時の金額クリア
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type);
	// 手入力サインのクリア
	int ClearManualInputSign(BOOL isChgKzritu);
	// DB上にデータ有り？
	bool IsExistData(CDBNpSub *pDBNpSub, int sn_seq);
};
