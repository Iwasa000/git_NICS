#pragma once

//-----------------------------------------------------------------------------
// 一般本表データ（平成27年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH27SnHonpyoData : public CH26SnHonpyoData
{
public:
	unsigned char	Sn_KURI6[MONY_BUF_SIZE];	// 第六種事業　課税売上
	unsigned short	Sn_UP6;						// 第六種事業　売上割合

protected:
	char			rsv3[512];

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;

public:
	CH27SnHonpyoData();   // 標準コンストラクタ
	virtual ~CH27SnHonpyoData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq, int wrtype=0 );
	// 集計済みデータのセット
	int SetCalqedData( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei );
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
};
