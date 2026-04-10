#pragma once

//-----------------------------------------------------------------------------
// 付表６データ
//-----------------------------------------------------------------------------

class CDBNpSub;
//class CDBSyzAccess;
class CSyzUtil;

#define SOZOKU_MAX 40

class SYZSHIN31DATA_CLASS_EXPORT CH31SnFhyo6Data
{

public:

	int				Sn_F6_DEATH;
	CString			Sn_F6_DAIHYO;
	int				Sn_F6_LIMIT;

	int				Sn_NO[SOZOKU_MAX];
	CString			Sn_F6_JUSYO[SOZOKU_MAX];
	CString			Sn_F6_FURI[SOZOKU_MAX];
	CString			Sn_F6_NAME[SOZOKU_MAX];
	char			Sn_F6_HASH[SOZOKU_MAX][33];
	CString			Sn_F6_SYOKU[SOZOKU_MAX];
	CString			Sn_F6_TSUDUKI[SOZOKU_MAX];
	int				Sn_F6_BIRTH[SOZOKU_MAX];
	CString			Sn_F6_TEL1[SOZOKU_MAX];
	CString			Sn_F6_TEL2[SOZOKU_MAX];
	CString			Sn_F6_TEL3[SOZOKU_MAX];
	int				Sn_F6_HOTEI[SOZOKU_MAX];
	CString			Sn_F6_BUNSHI[SOZOKU_MAX];
	CString			Sn_F6_BUNBO[SOZOKU_MAX];
	unsigned char	Sn_F6_KAGAKU[SOZOKU_MAX][6];
	unsigned char	Sn_F6_NOFUZEI[SOZOKU_MAX][6];
	unsigned char	Sn_F6_NOFUCHIHO[SOZOKU_MAX][6];
	unsigned char	Sn_F6_KANPUZEI[SOZOKU_MAX][6];
	unsigned char	Sn_F6_KANPUCHIHO[SOZOKU_MAX][6];
	int				Sn_F6_NOFUKANPUsw[SOZOKU_MAX];
	CString			Sn_F6_GNAME[SOZOKU_MAX];
	int				Sn_F6_GSEL[SOZOKU_MAX];
	CString			Sn_F6_SNAME[SOZOKU_MAX];
	int				Sn_F6_SSEL[SOZOKU_MAX];
	CString			Sn_F6_YOKIN[SOZOKU_MAX];
	CString			Sn_F6_KOUZA[SOZOKU_MAX];
	CString			Sn_F6_KIGO1[SOZOKU_MAX];
	CString			Sn_F6_KIGO2[SOZOKU_MAX];
	CString			Sn_F6_YUBIN[SOZOKU_MAX];

	int				Sn_6BS_USE;
	unsigned char	Sn_6BS_ADR[40*2];	// 事業承継の住所
	unsigned char	Sn_6BS_TELNO1[6];	// 事業承継の電話番号
	unsigned char	Sn_6BS_TELNO2[4];	// 事業承継の電話番号
	unsigned char	Sn_6BS_TELNO3[4];	// 事業承継の電話番号
	CString			Sn_6BS_NMFRI;		// 事業承継のフリガナ
	CString			Sn_6BS_NAME;		// 事業承継の氏名
	CString			Sn_6BS_SONOTA;		// 事業承継のその他

protected:
	char			rsv1[512-112];		// '20.05.12：512 → 512-112

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	CArithEx		m_Arith;

	char			rsv2[256];

public:
	CH31SnFhyo6Data(void);
	~CH31SnFhyo6Data(void);

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );

};
