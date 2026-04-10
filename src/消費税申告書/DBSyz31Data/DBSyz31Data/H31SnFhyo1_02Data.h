#pragma once

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo1_02Data
{

public:

	char*				rsv;

	unsigned	char	Sn_F102_1A[MONY_BUF_SIZE],				// 課税標準額　税率3%適用分　A
						Sn_F102_1B[MONY_BUF_SIZE],				// 課税標準額　税率4％適用分　B
						Sn_F102_1C[MONY_BUF_SIZE],				// 課税標準額　税率6.3％適用分　C
						Sn_F102_1X[MONY_BUF_SIZE];				// 課税標準額　旧税率分小計 X

	unsigned	char	Sn_F102_1_1A[MONY_BUF_SIZE],			// 
						Sn_F102_1_1B[MONY_BUF_SIZE],			// 
						Sn_F102_1_1C[MONY_BUF_SIZE],			// 
						Sn_F102_1_1X[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F102_1_2C[MONY_BUF_SIZE],			// 
						Sn_F102_1_2X[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F102_2A[MONY_BUF_SIZE],				// 
						Sn_F102_2B[MONY_BUF_SIZE],				// 
						Sn_F102_2C[MONY_BUF_SIZE],				// 
						Sn_F102_2X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_3A[MONY_BUF_SIZE],				// 
						Sn_F102_3B[MONY_BUF_SIZE],				// 
						Sn_F102_3C[MONY_BUF_SIZE],				// 
						Sn_F102_3X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_4A[MONY_BUF_SIZE],				// 
						Sn_F102_4B[MONY_BUF_SIZE],				// 
						Sn_F102_4C[MONY_BUF_SIZE],				// 
						Sn_F102_4X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_5A[MONY_BUF_SIZE],				// 
						Sn_F102_5B[MONY_BUF_SIZE],				// 
						Sn_F102_5C[MONY_BUF_SIZE],				// 
						Sn_F102_5X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_5_1A[MONY_BUF_SIZE],			// 
						Sn_F102_5_1B[MONY_BUF_SIZE],			// 
						Sn_F102_5_1C[MONY_BUF_SIZE],			// 
						Sn_F102_5_1X[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F102_5_2C[MONY_BUF_SIZE],			// 
						Sn_F102_5_2X[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F102_6A[MONY_BUF_SIZE],				// 
						Sn_F102_6B[MONY_BUF_SIZE],				// 
						Sn_F102_6C[MONY_BUF_SIZE],				// 
						Sn_F102_6X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_7A[MONY_BUF_SIZE],				// 
						Sn_F102_7B[MONY_BUF_SIZE],				// 
						Sn_F102_7C[MONY_BUF_SIZE],				// 
						Sn_F102_7X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_8A[MONY_BUF_SIZE],				// 
						Sn_F102_8B[MONY_BUF_SIZE],				// 
						Sn_F102_8C[MONY_BUF_SIZE],				// 
						Sn_F102_8X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_9A[MONY_BUF_SIZE],				// 
						Sn_F102_9B[MONY_BUF_SIZE],				// 
						Sn_F102_9C[MONY_BUF_SIZE],				// 
						Sn_F102_9X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_11B[MONY_BUF_SIZE],				// 
						Sn_F102_11C[MONY_BUF_SIZE],				// 
						Sn_F102_11X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_12B[MONY_BUF_SIZE],				// 
						Sn_F102_12C[MONY_BUF_SIZE],				// 
						Sn_F102_12X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_13B[MONY_BUF_SIZE],				// 
						Sn_F102_13C[MONY_BUF_SIZE],				// 
						Sn_F102_13X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_14B[MONY_BUF_SIZE],				// 
						Sn_F102_14C[MONY_BUF_SIZE],				// 
						Sn_F102_14X[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F102_15B[MONY_BUF_SIZE],				// 
						Sn_F102_15C[MONY_BUF_SIZE],				// 
						Sn_F102_15X[MONY_BUF_SIZE];				// 

	int					Sn_F102_2c;								// 手入力サイン

private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	CArithEx			m_Arith;
	BOOL				m_isTransitionalMeasure;				// 経過措置あり?


protected:
	char				rsv1[512];


protected:
	char				rsv2[256];

public:
	CH31SnFhyo1_02Data( BOOL isTransitionalMeasure);
	virtual ~CH31SnFhyo1_02Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqed1_02Data(CSyzSyukei *pSyzSyukei, BOOL &isPluralZei);
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
