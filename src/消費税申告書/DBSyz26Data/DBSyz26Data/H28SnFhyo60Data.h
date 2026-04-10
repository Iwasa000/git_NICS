#pragma once

//-----------------------------------------------------------------------------
// 付表６データ
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

#define SOZOKU_MAX 40

class SYZSHIN26DATA_CLASS_EXPORT CH28SnFhyo60Data
{

public:

	//int				Sn_SEQ;
	int				Sn_6FSDEATH;
	CString			Sn_6FSDAIHYO;
	int				Sn_6FSLIMIT;

	//int				Sn_SEQ[SOZOKU_MAX];
	int				Sn_NO[SOZOKU_MAX];
	CString			Sn_6FSJUSYO[SOZOKU_MAX];
	CString			Sn_6FSFURI[SOZOKU_MAX];
	CString			Sn_6FSNAME[SOZOKU_MAX];
	char			Sn_6FSHASH[SOZOKU_MAX][33];
	CString			Sn_6FSSYOKU[SOZOKU_MAX];
	CString			Sn_6FSTSUDUKI[SOZOKU_MAX];
	int				Sn_6FSBIRTH[SOZOKU_MAX];
	CString			Sn_6FSTEL1[SOZOKU_MAX];
	CString			Sn_6FSTEL2[SOZOKU_MAX];
	CString			Sn_6FSTEL3[SOZOKU_MAX];
	int				Sn_6FSHOTEI[SOZOKU_MAX];
	CString			Sn_6FSBUNSHI[SOZOKU_MAX];
	CString			Sn_6FSBUNBO[SOZOKU_MAX];
	unsigned char	Sn_6FSKAGAKU[SOZOKU_MAX][6];
	unsigned char	Sn_6FSNOFUZEI[SOZOKU_MAX][6];
	unsigned char	Sn_6FSNOFUCHIHO[SOZOKU_MAX][6];
	unsigned char	Sn_6FSKANPUZEI[SOZOKU_MAX][6];
	unsigned char	Sn_6FSKANPUCHIHO[SOZOKU_MAX][6];
	int				Sn_6FSNOFUKANPUsw[SOZOKU_MAX];
	CString			Sn_6FSGNAME[SOZOKU_MAX];
	int				Sn_6FSGSEL[SOZOKU_MAX];
	CString			Sn_6FSSNAME[SOZOKU_MAX];
	int				Sn_6FSSSEL[SOZOKU_MAX];
	CString			Sn_6FSYOKIN[SOZOKU_MAX];
	CString			Sn_6FSKOUZA[SOZOKU_MAX];
	CString			Sn_6FSKIGO1[SOZOKU_MAX];
	CString			Sn_6FSKIGO2[SOZOKU_MAX];
	CString			Sn_6FSYUBIN[SOZOKU_MAX];

protected:
	char			rsv1[512];

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	CArithEx		m_Arith;

public:
	CH28SnFhyo60Data(void);
	~CH28SnFhyo60Data(void);

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );

};
