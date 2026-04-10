#pragma once


class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo4_01Data
{

public:

	char*				rsv;

	unsigned	char	Sn_F401_1X[MONY_BUF_SIZE],				// 課税標準額　旧税率分小計　X
						Sn_F401_1D[MONY_BUF_SIZE],				// 課税標準額　税率6.24％適用分　D
						Sn_F401_1E[MONY_BUF_SIZE],				// 課税標準額　税率7.8％適用分　E
						Sn_F401_1F[MONY_BUF_SIZE];				// 課税標準額　合計 F

	unsigned	char	Sn_F401_1_1X[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額 旧税率分小計 X
						Sn_F401_1_1D[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額 税率6.24％適用分　D
						Sn_F401_1_1E[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額 税率7.8％適用分　E
						Sn_F401_1_1F[MONY_BUF_SIZE];			// 課税資産の譲渡等の対価の額 合計 F

	unsigned	char	Sn_F401_2X[MONY_BUF_SIZE],				// 消費税額　旧税率分小計　X
						Sn_F401_2D[MONY_BUF_SIZE],				// 消費税額　税率6.24％適用分　D
						Sn_F401_2E[MONY_BUF_SIZE],				// 消費税額　税率7.8％適用分　E
						Sn_F401_2F[MONY_BUF_SIZE];				// 消費税額　合計 F
	int					Sn_F401_2sw;							// 手入力サイン

	unsigned	char	Sn_F401_3X[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　旧税率分小計　X
						Sn_F401_3D[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率6.24％適用分　D
						Sn_F401_3E[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率7.8％適用分　E
						Sn_F401_3F[MONY_BUF_SIZE];				// 貸倒回収に係る消費税額　合計 F

	unsigned	char	Sn_F401_4X[MONY_BUF_SIZE],				// 控除対象仕入れ税額　旧税率分小計　X
						Sn_F401_4D[MONY_BUF_SIZE],				// 控除対象仕入れ税額　税率6.24％適用分　D
						Sn_F401_4E[MONY_BUF_SIZE],				// 控除対象仕入れ税額　税率7.8％適用分　E
						Sn_F401_4F[MONY_BUF_SIZE];				// 控除対象仕入れ税額　合計 F

	unsigned	char	Sn_F401_5X[MONY_BUF_SIZE],				// 返還等対価に係る税額　旧税率分小計　X
						Sn_F401_5D[MONY_BUF_SIZE],				// 返還等対価に係る税額　税率6.24％適用分　D
						Sn_F401_5E[MONY_BUF_SIZE],				// 返還等対価に係る税額　税率7.8％適用分　E
						Sn_F401_5F[MONY_BUF_SIZE];				// 返還等対価に係る税額　合計 F

	unsigned	char	Sn_F401_6X[MONY_BUF_SIZE],				// 貸倒れに係る税額　旧税率分小計　X
						Sn_F401_6D[MONY_BUF_SIZE],				// 貸倒れに係る税額　税率6.24％適用分　D
						Sn_F401_6E[MONY_BUF_SIZE],				// 貸倒れに係る税額　税率7.8％適用分　E
						Sn_F401_6F[MONY_BUF_SIZE];				// 貸倒れに係る税額　合計 F

	unsigned	char	Sn_F401_7X[MONY_BUF_SIZE],				// 控除税額小計　旧税率分小計　X
						Sn_F401_7D[MONY_BUF_SIZE],				// 控除税額小計　税率6.24％適用分　D
						Sn_F401_7E[MONY_BUF_SIZE],				// 控除税額小計　税率7.8％適用分　E
						Sn_F401_7F[MONY_BUF_SIZE];				// 控除税額小計　合計 F

	unsigned	char	Sn_F401_8X[MONY_BUF_SIZE],				// 控除不足還付税額　旧税率分小計　X
						Sn_F401_8D[MONY_BUF_SIZE],				// 控除不足還付税額　税率6.24％適用分　D
						Sn_F401_8E[MONY_BUF_SIZE],				// 控除不足還付税額　税率7.8％適用分　E
						Sn_F401_8F[MONY_BUF_SIZE];				// 控除不足還付税額　合計 F

	unsigned	char	Sn_F401_9X[MONY_BUF_SIZE],				// 差引税額　旧税率分小計　X
						Sn_F401_9D[MONY_BUF_SIZE],				// 差引税額　税率6.24％適用分　D
						Sn_F401_9E[MONY_BUF_SIZE],				// 差引税額　税率7.8％適用分　E
						Sn_F401_9F[MONY_BUF_SIZE];				// 差引税額　合計 F

	unsigned	char	Sn_F401_10F[MONY_BUF_SIZE];				// 合計差引税額　合計 F

	unsigned	char	Sn_F401_11X[MONY_BUF_SIZE],				// 控除不足還付税額　旧税率分小計　X
						Sn_F401_11E[MONY_BUF_SIZE],				// 控除不足還付税額　税率7.8％適用分　E
						Sn_F401_11F[MONY_BUF_SIZE];				// 控除不足還付税額　合計 F

	unsigned	char	Sn_F401_12X[MONY_BUF_SIZE],				// 差引税額　旧税率分小計　X
						Sn_F401_12E[MONY_BUF_SIZE],				// 差引税額　税率7.8％適用分　E
						Sn_F401_12F[MONY_BUF_SIZE];				// 差引税額　合計 F

	unsigned	char	Sn_F401_13X[MONY_BUF_SIZE],				// 合計差引地方消費税の課税標準となる消費税額　旧税率分小計　X
						Sn_F401_13E[MONY_BUF_SIZE],				// 合計差引地方消費税の課税標準となる消費税額　税率7.8％適用分　E
						Sn_F401_13F[MONY_BUF_SIZE];				// 合計差引地方消費税の課税標準となる消費税額　合計 F

	unsigned	char	Sn_F401_14X[MONY_BUF_SIZE],				// 還付額　旧税率分小計　X
						Sn_F401_14E[MONY_BUF_SIZE],				// 還付額　税率7.8％適用分　E
						Sn_F401_14F[MONY_BUF_SIZE];				// 還付額　合計 F

	unsigned	char	Sn_F401_15X[MONY_BUF_SIZE],				// 納付額　旧税率分小計　X
						Sn_F401_15E[MONY_BUF_SIZE],				// 納付額　税率7.8％適用分　E
						Sn_F401_15F[MONY_BUF_SIZE];				// 納付額　合計 F

	unsigned	char	Sn_F401_16F[MONY_BUF_SIZE];				// 合計差引譲渡割額　合計 F


private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	BOOL				m_isTransitionalMeasure;				// 経過措置あり？
	CArithEx			m_Arith;

protected:
	char				rsv1[512];


protected:
	char				rsv2[256];



public:
	CH31SnFhyo4_01Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo4_01Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット(付表4-1)
	int SetCalqedData(CSyzSyukei *pSyzSyukei, BOOL &isPluralZei);

public:
	// 更新時の金額クリア
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type);
	// 手入力サインのクリア
	int ClearManualInputSign(BOOL isChgKzritu);
	// 更新時の金額追加書き込み
	int SetDataInRenew(CDBNpSub *pDBNpSub, int sn_seq);
	// DB上にデータ有り？
	bool IsExistData(CDBNpSub *pDBNpSub, int sn_seq);
};
