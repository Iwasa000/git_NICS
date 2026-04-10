#pragma once

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo5_022Data
{
		
public:

	char*				rsv;

	unsigned	char	Sn_F5022_20A[MONY_BUF_SIZE],				// 4×みなし仕入率　税率3%適用分　A
						Sn_F5022_20B[MONY_BUF_SIZE],				// 4×みなし仕入率　税率4％適用分　B
						Sn_F5022_20C[MONY_BUF_SIZE],				// 4×みなし仕入率　税率6.3％適用分　C
						Sn_F5022_20X[MONY_BUF_SIZE];				// 4×みなし仕入率　旧税率分小計 X
	int					Sn_F5022_20sw;								// 手入力サイン

	unsigned	char	Sn_F5022_21A[MONY_BUF_SIZE],				// 4×みなし仕入率　税率3%適用分　A
						Sn_F5022_21B[MONY_BUF_SIZE],				// 4×みなし仕入率　税率4％適用分　B
						Sn_F5022_21C[MONY_BUF_SIZE],				// 4×みなし仕入率　税率6.3％適用分　C
						Sn_F5022_21X[MONY_BUF_SIZE];				// 4×みなし仕入率　旧税率分小計 X
	int					Sn_F5022_21sw;								// 手入力サイン
	int					Sn_F5022_21kbn;								// みなし仕入率　1:90％...

	unsigned	char	Sn_F5022_22A[MONY_BUF_SIZE],				// 第一種事業及び第二種事業　税率3%適用分　A
						Sn_F5022_22B[MONY_BUF_SIZE],				// 第一種事業及び第二種事業　税率4％適用分　B
						Sn_F5022_22C[MONY_BUF_SIZE],				// 第一種事業及び第二種事業　税率6.3％適用分　C
						Sn_F5022_22X[MONY_BUF_SIZE];				// 第一種事業及び第二種事業　旧税率分小計 X
	int					Sn_F5022_22sw;								// 手入力サイン

	unsigned	char	Sn_F5022_23A[MONY_BUF_SIZE],				// 第一種事業及び第三種事業　税率3%適用分　A
						Sn_F5022_23B[MONY_BUF_SIZE],				// 第一種事業及び第三種事業　税率4％適用分　B
						Sn_F5022_23C[MONY_BUF_SIZE],				// 第一種事業及び第三種事業　税率6.3％適用分　C
						Sn_F5022_23X[MONY_BUF_SIZE];				// 第一種事業及び第三種事業　旧税率分小計 X
	int					Sn_F5022_23sw;								// 手入力サイン

	unsigned	char	Sn_F5022_24A[MONY_BUF_SIZE],				// 第一種事業及び第四種事業　税率3%適用分　A
						Sn_F5022_24B[MONY_BUF_SIZE],				// 第一種事業及び第四種事業　税率4％適用分　B
						Sn_F5022_24C[MONY_BUF_SIZE],				// 第一種事業及び第四種事業　税率6.3％適用分　C
						Sn_F5022_24X[MONY_BUF_SIZE];				// 第一種事業及び第四種事業　旧税率分小計 X
	int					Sn_F5022_24sw;								// 手入力サイン

	unsigned	char	Sn_F5022_25A[MONY_BUF_SIZE],				// 第一種事業及び第五種事業　税率3%適用分　A
						Sn_F5022_25B[MONY_BUF_SIZE],				// 第一種事業及び第五種事業　税率4％適用分　B
						Sn_F5022_25C[MONY_BUF_SIZE],				// 第一種事業及び第五種事業　税率6.3％適用分　C
						Sn_F5022_25X[MONY_BUF_SIZE];				// 第一種事業及び第五種事業　旧税率分小計 X
	int					Sn_F5022_25sw;								// 手入力サイン

	unsigned	char	Sn_F5022_26A[MONY_BUF_SIZE],				// 第一種事業及び第六種事業　税率3%適用分　A
						Sn_F5022_26B[MONY_BUF_SIZE],				// 第一種事業及び第六種事業　税率4％適用分　B
						Sn_F5022_26C[MONY_BUF_SIZE],				// 第一種事業及び第六種事業　税率6.3％適用分　C
						Sn_F5022_26X[MONY_BUF_SIZE];				// 第一種事業及び第六種事業　旧税率分小計 X
	int					Sn_F5022_26sw;								// 手入力サイン

	unsigned	char	Sn_F5022_27A[MONY_BUF_SIZE],				// 第二種事業及び第三種事業　税率3%適用分　A
						Sn_F5022_27B[MONY_BUF_SIZE],				// 第二種事業及び第三種事業　税率4％適用分　B
						Sn_F5022_27C[MONY_BUF_SIZE],				// 第二種事業及び第三種事業　税率6.3％適用分　C
						Sn_F5022_27X[MONY_BUF_SIZE];				// 第二種事業及び第三種事業　旧税率分小計 X
	int					Sn_F5022_27sw;								// 手入力サイン

	unsigned	char	Sn_F5022_28A[MONY_BUF_SIZE],				// 第二種事業及び第四種事業　税率3%適用分　A
						Sn_F5022_28B[MONY_BUF_SIZE],				// 第二種事業及び第四種事業　税率4％適用分　B
						Sn_F5022_28C[MONY_BUF_SIZE],				// 第二種事業及び第四種事業　税率6.3％適用分　C
						Sn_F5022_28X[MONY_BUF_SIZE];				// 第二種事業及び第四種事業　旧税率分小計 X
	int					Sn_F5022_28sw;								// 手入力サイン

	unsigned	char	Sn_F5022_29A[MONY_BUF_SIZE],				// 第二種事業及び第五種事業　税率3%適用分　A
						Sn_F5022_29B[MONY_BUF_SIZE],				// 第二種事業及び第五種事業　税率4％適用分　B
						Sn_F5022_29C[MONY_BUF_SIZE],				// 第二種事業及び第五種事業　税率6.3％適用分　C
						Sn_F5022_29X[MONY_BUF_SIZE];				// 第二種事業及び第五種事業　旧税率分小計 X
	int					Sn_F5022_29sw;								// 手入力サイン

	unsigned	char	Sn_F5022_30A[MONY_BUF_SIZE],				// 第二種事業及び第六種事業　税率3%適用分　A
						Sn_F5022_30B[MONY_BUF_SIZE],				// 第二種事業及び第六種事業　税率4％適用分　B
						Sn_F5022_30C[MONY_BUF_SIZE],				// 第二種事業及び第六種事業　税率6.3％適用分　C
						Sn_F5022_30X[MONY_BUF_SIZE];				// 第二種事業及び第六種事業　旧税率分小計 X
	int					Sn_F5022_30sw;								// 手入力サイン

	unsigned	char	Sn_F5022_31A[MONY_BUF_SIZE],				// 第三種事業及び第四種事業　税率3%適用分　A
						Sn_F5022_31B[MONY_BUF_SIZE],				// 第三種事業及び第四種事業　税率4％適用分　B
						Sn_F5022_31C[MONY_BUF_SIZE],				// 第三種事業及び第四種事業　税率6.3％適用分　C
						Sn_F5022_31X[MONY_BUF_SIZE];				// 第三種事業及び第四種事業　旧税率分小計 X
	int					Sn_F5022_31sw;								// 手入力サイン

	unsigned	char	Sn_F5022_32A[MONY_BUF_SIZE],				// 第三種事業及び第五種事業　税率3%適用分　A
						Sn_F5022_32B[MONY_BUF_SIZE],				// 第三種事業及び第五種事業　税率4％適用分　B
						Sn_F5022_32C[MONY_BUF_SIZE],				// 第三種事業及び第五種事業　税率6.3％適用分　C
						Sn_F5022_32X[MONY_BUF_SIZE];				// 第三種事業及び第五種事業　旧税率分小計 X
	int					Sn_F5022_32sw;								// 手入力サイン

	unsigned	char	Sn_F5022_33A[MONY_BUF_SIZE],				// 第三種事業及び第六種事業　税率3%適用分　A
						Sn_F5022_33B[MONY_BUF_SIZE],				// 第三種事業及び第六種事業　税率4％適用分　B
						Sn_F5022_33C[MONY_BUF_SIZE],				// 第三種事業及び第六種事業　税率6.3％適用分　C
						Sn_F5022_33X[MONY_BUF_SIZE];				// 第三種事業及び第六種事業　旧税率分小計 X
	int					Sn_F5022_33sw;								// 手入力サイン

	unsigned	char	Sn_F5022_34A[MONY_BUF_SIZE],				// 第四種事業及び第五種事業　税率3%適用分　A
						Sn_F5022_34B[MONY_BUF_SIZE],				// 第四種事業及び第五種事業　税率4％適用分　B
						Sn_F5022_34C[MONY_BUF_SIZE],				// 第四種事業及び第五種事業　税率6.3％適用分　C
						Sn_F5022_34X[MONY_BUF_SIZE];				// 第四種事業及び第五種事業　旧税率分小計 X
	int					Sn_F5022_34sw;								// 手入力サイン

	unsigned	char	Sn_F5022_35A[MONY_BUF_SIZE],				// 第四種事業及び第六種事業　税率3%適用分　A
						Sn_F5022_35B[MONY_BUF_SIZE],				// 第四種事業及び第六種事業　税率4％適用分　B
						Sn_F5022_35C[MONY_BUF_SIZE],				// 第四種事業及び第六種事業　税率6.3％適用分　C
						Sn_F5022_35X[MONY_BUF_SIZE];				// 第四種事業及び第六種事業　旧税率分小計 X
	int					Sn_F5022_35sw;								// 手入力サイン

	unsigned	char	Sn_F5022_36A[MONY_BUF_SIZE],				// 第五種事業及び第六種事業　税率3%適用分　A
						Sn_F5022_36B[MONY_BUF_SIZE],				// 第五種事業及び第六種事業　税率4％適用分　B
						Sn_F5022_36C[MONY_BUF_SIZE],				// 第五種事業及び第六種事業　税率6.3％適用分　C
						Sn_F5022_36X[MONY_BUF_SIZE];				// 第五種事業及び第六種事業　旧税率分小計 X
	int					Sn_F5022_36sw;								// 手入力サイン

	unsigned	char	Sn_F5022_37A[MONY_BUF_SIZE],				// 選択可能な計算式区分の内から選択した金額　税率3%適用分　A
						Sn_F5022_37B[MONY_BUF_SIZE],				// 選択可能な計算式区分の内から選択した金額　税率4％適用分　B
						Sn_F5022_37C[MONY_BUF_SIZE],				// 選択可能な計算式区分の内から選択した金額　税率6.3％適用分　C
						Sn_F5022_37X[MONY_BUF_SIZE];				// 選択可能な計算式区分の内から選択した金額　旧税率分小計 X

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
	CH31SnFhyo5_022Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo5_022Data();

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
