#pragma once


class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo6_2wariData
{

public:

	char*				rsv;

	unsigned	char	Sn_F601_1A[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額 税率6.24%適用分  A
						Sn_F601_1B[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額 税率7.8%適用分   B
						Sn_F601_1C[MONY_BUF_SIZE];				// 課税資産の譲渡等の対価の額 合計 C

	unsigned	char	Sn_F601_2A[MONY_BUF_SIZE],				// 課税標準額 税率6.24%適用分  A
						Sn_F601_2B[MONY_BUF_SIZE],				// 課税標準額 税率7.8%適用分   B
						Sn_F601_2C[MONY_BUF_SIZE];				// 課税標準額 合計 C

	unsigned	char	Sn_F601_3A[MONY_BUF_SIZE],				// 課税標準額に対する消費税額 税率6.24%適用分  A
						Sn_F601_3B[MONY_BUF_SIZE],				// 課税標準額に対する消費税額 税率7.8%適用分   B
						Sn_F601_3C[MONY_BUF_SIZE];				// 課税標準額に対する消費税額 合計 C
	int					Sn_F601_3sw;							// 手入力サイン

	unsigned	char	Sn_F601_4A[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額 税率6.24%適用分  A
						Sn_F601_4B[MONY_BUF_SIZE],				// 貸倒回収に係る消費税額 税率7.8%適用分   B
						Sn_F601_4C[MONY_BUF_SIZE];				// 貸倒回収に係る消費税額 合計 C

	unsigned	char	Sn_F601_5A[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額 税率6.24%適用分  A
						Sn_F601_5B[MONY_BUF_SIZE],				// 売上対価の返還等に係る消費税額 税率7.8%適用分   B
						Sn_F601_5C[MONY_BUF_SIZE];				// 売上対価の返還等に係る消費税額 合計 C

	unsigned	char	Sn_F601_6A[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額 税率6.24%適用分  A
						Sn_F601_6B[MONY_BUF_SIZE],				// 控除対象仕入税額の計算の基礎となる消費税額 税率7.8%適用分   B
						Sn_F601_6C[MONY_BUF_SIZE];				// 控除対象仕入税額の計算の基礎となる消費税額 合計 C

	unsigned	char	Sn_F601_7A[MONY_BUF_SIZE],				// 特別控除税額 税率6.24%適用分  A
						Sn_F601_7B[MONY_BUF_SIZE],				// 特別控除税額 税率7.8%適用分   B
						Sn_F601_7C[MONY_BUF_SIZE];				// 特別控除税額 合計 C

	unsigned	char	Sn_F601_8A[MONY_BUF_SIZE],				// 貸倒れに係る税額 税率6.24%適用分  A
						Sn_F601_8B[MONY_BUF_SIZE],				// 貸倒れに係る税額 税率7.8%適用分   B
						Sn_F601_8C[MONY_BUF_SIZE];				// 貸倒れに係る税額 合計 C

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
	CH31SnFhyo6_2wariData( BOOL isTransitionalMeasure );
	virtual ~CH31SnFhyo6_2wariData();

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
