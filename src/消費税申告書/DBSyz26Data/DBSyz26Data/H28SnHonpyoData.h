#pragma once

//-----------------------------------------------------------------------------
// 一般本表データ（平成27年10月改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH28SnHonpyoData : public CH27SnHonpyoData
{
public:
	unsigned char	Sn_KSJT3[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(3%)
					Sn_KSJT4[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(4%)
					Sn_KSJT63[MONY_BUF_SIZE],	// 課税資産の譲渡等の対価の額(6.3%)
					Sn_KSJTKEI[MONY_BUF_SIZE];	// 課税資産の譲渡等の対価の額(合計)

	unsigned char	Sn_TKST63[MONY_BUF_SIZE],	// 特定課税仕入に係る支払対価の額(6.3%)
					Sn_TKSTKEI[MONY_BUF_SIZE];	// 特定課税仕入に係る支払対価の額(合計)

	unsigned char	Sn_URHKNZ[MONY_BUF_SIZE],	// 売上の返還等対価に係る税額
					Sn_TKHKNZ[MONY_BUF_SIZE];	// 特定課税仕入の返還等対価に係る税額

protected:
	char			rsv4[512];

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;

public:
	CH28SnHonpyoData();   // 標準コンストラクタ
	virtual ~CH28SnHonpyoData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq, int wrtype=0 );
	// 集計済みデータのセット
	int SetCalqedData( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei );
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
};
