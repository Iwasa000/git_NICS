#pragma once


class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo1_01Data
{

public:

	char*				rsv;

	unsigned	char	Sn_F101_1X[MONY_BUF_SIZE],				// 課税標準額　旧税率分小計　X
						Sn_F101_1D[MONY_BUF_SIZE],				// 課税標準額　税率6.24％適用分　D
						Sn_F101_1E[MONY_BUF_SIZE],				// 課税標準額　税率7.8％適用分　E
						Sn_F101_1F[MONY_BUF_SIZE];				// 課税標準額　合計 F

	unsigned	char	Sn_F101_1_1X[MONY_BUF_SIZE],			// 
						Sn_F101_1_1D[MONY_BUF_SIZE],			// 
						Sn_F101_1_1E[MONY_BUF_SIZE],			// 
						Sn_F101_1_1F[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F101_1_2X[MONY_BUF_SIZE],			// 
						Sn_F101_1_2E[MONY_BUF_SIZE],			// 
						Sn_F101_1_2F[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F101_2X[MONY_BUF_SIZE],				// 
						Sn_F101_2D[MONY_BUF_SIZE],				// 
						Sn_F101_2E[MONY_BUF_SIZE],				// 
						Sn_F101_2F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_3X[MONY_BUF_SIZE],				// 
						Sn_F101_3D[MONY_BUF_SIZE],				// 
						Sn_F101_3E[MONY_BUF_SIZE],				// 
						Sn_F101_3F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_4X[MONY_BUF_SIZE],				// 
						Sn_F101_4D[MONY_BUF_SIZE],				// 
						Sn_F101_4E[MONY_BUF_SIZE],				// 
						Sn_F101_4F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_5X[MONY_BUF_SIZE],				// 
						Sn_F101_5D[MONY_BUF_SIZE],				// 
						Sn_F101_5E[MONY_BUF_SIZE],				// 
						Sn_F101_5F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_5_1X[MONY_BUF_SIZE],			// 
						Sn_F101_5_1D[MONY_BUF_SIZE],			// 
						Sn_F101_5_1E[MONY_BUF_SIZE],			// 
						Sn_F101_5_1F[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F101_5_2X[MONY_BUF_SIZE],			// 
						Sn_F101_5_2E[MONY_BUF_SIZE],			// 
						Sn_F101_5_2F[MONY_BUF_SIZE];			// 

	unsigned	char	Sn_F101_6X[MONY_BUF_SIZE],				// 
						Sn_F101_6D[MONY_BUF_SIZE],				// 
						Sn_F101_6E[MONY_BUF_SIZE],				// 
						Sn_F101_6F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_7X[MONY_BUF_SIZE],				// 
						Sn_F101_7D[MONY_BUF_SIZE],				// 
						Sn_F101_7E[MONY_BUF_SIZE],				// 
						Sn_F101_7F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_8X[MONY_BUF_SIZE],				// 
						Sn_F101_8D[MONY_BUF_SIZE],				// 
						Sn_F101_8E[MONY_BUF_SIZE],				// 
						Sn_F101_8F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_9X[MONY_BUF_SIZE],				// 
						Sn_F101_9D[MONY_BUF_SIZE],				// 
						Sn_F101_9E[MONY_BUF_SIZE],				// 
						Sn_F101_9F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_10F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_11X[MONY_BUF_SIZE],				// 
						Sn_F101_11E[MONY_BUF_SIZE],				// 
						Sn_F101_11F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_12X[MONY_BUF_SIZE],				// 
						Sn_F101_12E[MONY_BUF_SIZE],				// 
						Sn_F101_12F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_13X[MONY_BUF_SIZE],				// 
						Sn_F101_13E[MONY_BUF_SIZE],				// 
						Sn_F101_13F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_14X[MONY_BUF_SIZE],				// 
						Sn_F101_14E[MONY_BUF_SIZE],				// 
						Sn_F101_14F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_15X[MONY_BUF_SIZE],				// 
						Sn_F101_15E[MONY_BUF_SIZE],				// 
						Sn_F101_15F[MONY_BUF_SIZE];				// 

	unsigned	char	Sn_F101_16E[MONY_BUF_SIZE],				// 
						Sn_F101_16F[MONY_BUF_SIZE];				// 

	int					Sn_F101_2c;								// 手入力サイン

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
	CH31SnFhyo1_01Data( BOOL isTransitionalMeasure);
	virtual ~CH31SnFhyo1_01Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット(付表1－1)
	int SetCalqed1_01Data(CSyzSyukei *pSyzSyukei, BOOL &isPluralZei);


public:
	// 更新時の金額クリア
	int ClearDataByRenew(CDBNpSub *pDBNpSub, int type);
	// 手入力サインのクリア
	int ClearManualInputSign(BOOL isChgKzritu);
	// 更新時の金額追加書き込み
	int SetDataInRenew(CDBNpSub *pDBNpSub, int sn_seq);
	// DB上にデータ有り？
	bool IsExistData( CDBNpSub *pDBNpSub, int sn_seq );
};
