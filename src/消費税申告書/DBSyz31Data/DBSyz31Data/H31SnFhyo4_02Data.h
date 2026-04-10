#pragma once

class CDBNpSub;
class CDBSyzAccess;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo4_02Data
{
	
public:

	char*				rsv;

	unsigned	char	Sn_F402_1A[MONY_BUF_SIZE],				// 課税標準額　税率3%適用分　A
						Sn_F402_1B[MONY_BUF_SIZE],				// 課税標準額　税率4％適用分　B
						Sn_F402_1C[MONY_BUF_SIZE],				// 課税標準額　税率6.3％適用分　C
						Sn_F402_1X[MONY_BUF_SIZE];				// 課税標準額　旧税率分小計 X

	unsigned	char	Sn_F402_1_1A[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額　税率3%適用分　A
						Sn_F402_1_1B[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額　税率4％適用分　B
						Sn_F402_1_1C[MONY_BUF_SIZE],			// 課税資産の譲渡等の対価の額　税率6.3％適用分　C
						Sn_F402_1_1X[MONY_BUF_SIZE];			// 課税資産の譲渡等の対価の額　旧税率分小計 X

	unsigned	char	Sn_F402_2A[MONY_BUF_SIZE],				// 消費税額　税率3%適用分　A
						Sn_F402_2B[MONY_BUF_SIZE],				// 消費税額　税率4％適用分　B
						Sn_F402_2C[MONY_BUF_SIZE],				// 消費税額　税率6.3％適用分　C
						Sn_F402_2X[MONY_BUF_SIZE];				// 消費税額　旧税率分小計 X
	int					Sn_F402_2sw;							// 手入力サイン

	unsigned	char	Sn_F402_3A[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率3%適用分　A
						Sn_F402_3B[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率4％適用分　B
						Sn_F402_3C[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額　税率6.3％適用分　C
						Sn_F402_3X[MONY_BUF_SIZE];				// 貸倒回収に係る消費税額　旧税率分小計 X

	unsigned	char	Sn_F402_4A[MONY_BUF_SIZE],				// 控除対象仕入税額　税率3%適用分　A
						Sn_F402_4B[MONY_BUF_SIZE],				// 控除対象仕入税額　税率4％適用分　B
						Sn_F402_4C[MONY_BUF_SIZE],				// 控除対象仕入税額　税率6.3％適用分　C
						Sn_F402_4X[MONY_BUF_SIZE];				// 控除対象仕入税額　旧税率分小計 X

	unsigned	char	Sn_F402_5A[MONY_BUF_SIZE],				// 返還等対価に係る税額　税率3%適用分　A
						Sn_F402_5B[MONY_BUF_SIZE],				// 返還等対価に係る税額　税率4％適用分　B
						Sn_F402_5C[MONY_BUF_SIZE],				// 返還等対価に係る税額　税率6.3％適用分　C
						Sn_F402_5X[MONY_BUF_SIZE];				// 返還等対価に係る税額　旧税率分小計 X

	unsigned	char	Sn_F402_6A[MONY_BUF_SIZE],				// 貸倒れに係る税額　税率3%適用分　A
						Sn_F402_6B[MONY_BUF_SIZE],				// 貸倒れに係る税額　税率4％適用分　B
						Sn_F402_6C[MONY_BUF_SIZE],				// 貸倒れに係る税額　税率6.3％適用分　C
						Sn_F402_6X[MONY_BUF_SIZE];				// 貸倒れに係る税額　旧税率分小計 X

	unsigned	char	Sn_F402_7A[MONY_BUF_SIZE],				// 控除税額小計　税率3%適用分　A
						Sn_F402_7B[MONY_BUF_SIZE],				// 控除税額小計　税率4％適用分　B
						Sn_F402_7C[MONY_BUF_SIZE],				// 控除税額小計　税率6.3％適用分　C
						Sn_F402_7X[MONY_BUF_SIZE];				// 控除税額小計　旧税率分小計 X

	unsigned	char	Sn_F402_8A[MONY_BUF_SIZE],				// 控除不足還付税額　税率3%適用分　A
						Sn_F402_8B[MONY_BUF_SIZE],				// 控除不足還付税額　税率4％適用分　B
						Sn_F402_8C[MONY_BUF_SIZE],				// 控除不足還付税額　税率6.3％適用分　C
						Sn_F402_8X[MONY_BUF_SIZE];				// 控除不足還付税額　旧税率分小計 X

	unsigned	char	Sn_F402_9A[MONY_BUF_SIZE],				// 差引税額　税率3%適用分　A
						Sn_F402_9B[MONY_BUF_SIZE],				// 差引税額　税率4％適用分　B
						Sn_F402_9C[MONY_BUF_SIZE],				// 差引税額　税率6.3％適用分　C
						Sn_F402_9X[MONY_BUF_SIZE];				// 差引税額　旧税率分小計 X

	unsigned	char	Sn_F402_11B[MONY_BUF_SIZE],				// 控除不足還付税額　税率4％適用分　B
						Sn_F402_11C[MONY_BUF_SIZE],				// 控除不足還付税額　税率6.3％適用分　C
						Sn_F402_11X[MONY_BUF_SIZE];				// 控除不足還付税額　旧税率分小計 X

	unsigned	char	Sn_F402_12B[MONY_BUF_SIZE],				// 差引税額　税率4％適用分　B
						Sn_F402_12C[MONY_BUF_SIZE],				// 差引税額　税率6.3％適用分　C
						Sn_F402_12X[MONY_BUF_SIZE];				// 差引税額　旧税率分小計 X

	unsigned	char	Sn_F402_13B[MONY_BUF_SIZE],				// 合計差引地方消費税の課税標準となる消費税額　税率4％適用分　B
						Sn_F402_13C[MONY_BUF_SIZE],				// 合計差引地方消費税の課税標準となる消費税額　税率6.3％適用分　C
						Sn_F402_13X[MONY_BUF_SIZE];				// 合計差引地方消費税の課税標準となる消費税額　旧税率分小計 X

	unsigned	char	Sn_F402_14B[MONY_BUF_SIZE],				// 還付額　税率4％適用分　B
						Sn_F402_14C[MONY_BUF_SIZE],				// 還付額　税率6.3％適用分　C
						Sn_F402_14X[MONY_BUF_SIZE];				// 還付額　旧税率分小計 X

	unsigned	char	Sn_F402_15B[MONY_BUF_SIZE],				// 納税額　税率4％適用分　B
						Sn_F402_15C[MONY_BUF_SIZE],				// 納税額　税率6.3％適用分　C
						Sn_F402_15X[MONY_BUF_SIZE];				// 納税額　旧税率分小計 X

private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	BOOL				m_isTransitionalMeasure;	// 経過措置あり？
	CArithEx			m_Arith;

protected:
	char				rsv1[512];


protected:
	char				rsv2[256];

public:
	CH31SnFhyo4_02Data( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo4_02Data();

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
