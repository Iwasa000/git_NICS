#pragma once

//-----------------------------------------------------------------------------
// 予定申告（第26号様式）（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH26SnChskData
{
public:
	long			Sn_ZKDYS,						// 前期課税期間　自
					Sn_ZKDYE,						// 〃　　　　　　至
					Sn_ZZDAY;						// 修正・更正・決定の年月日

//					Sn_dmy40,						// リザーブ
	unsigned char	Sn_BSMON,						// 月換算	分子
					Sn_BBMON;						// 〃　　　 分母

	unsigned char	Sn_KTNFZ[MONY_BUF_SIZE],		// 消費税及び地方消費税の合計納付税額
					Sn_ZSKZN[12][MONY_BUF_SIZE],	// 国　税　この申告前の税額
					Sn_ZSKAD[12][MONY_BUF_SIZE],	// 〃　　　この申告により増加する税額
					Sn_ZSTZN[12][MONY_BUF_SIZE],	// 地方税　この申告前の税額
					Sn_ZSTAD[12][MONY_BUF_SIZE],	// 〃　　　この申告により増加する税額
					Sn_ZSKTZ[12][MONY_BUF_SIZE];	// 消費税及び地方消費税の合計納付税額

protected:
	char			rsv1[512];

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	CArithEx		m_Arith;

protected:
	char			rsv2[256];

public:
	CH26SnChskData();   // 標準コンストラクタ
	virtual ~CH26SnChskData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );

	// 年月日データの取得
	void GetYmdData( EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd );
	// 年月日データの取込
	void SetYmdData( EnumIdIcsShDateType dsDtype, int type, char data );

	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );

	// 年月日データの取得（改元対応）
	void GetYmdDataGen( EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd, int *pG=NULL );
	// 年月日データの取込（改元対応）
	void SetYmdDataGen( EnumIdIcsShDateType dsDtype, int type, char data , int gengo = 4 );
	// 年月日データの取込（改元対応）
	void SetYmdDataGen( EnumIdIcsShDateType dsDtype, int mType, char yy, char mm, char dd, int gengo = 4 );
};
