#pragma once

//-----------------------------------------------------------------------------
// 棚卸データ（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH26SnTanaData
{
public:
	unsigned char	Sn_MENZK[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額
					Sn_MENZZ[MONY_BUF_SIZE],		// 				 〃			  消費税額
					Sn_MNK40[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額(4%分)
					Sn_MNZ40[MONY_BUF_SIZE],		//				 〃			  消費税額(〃)
					Sn_MNK63[MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額(6.3%分)
					Sn_MNZ63[MONY_BUF_SIZE];		//				 〃			  消費税額(〃)
	unsigned char	Sn_menzk[3][MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額
					Sn_menzz[3][MONY_BUF_SIZE],		// 				 〃			  消費税額
					Sn_mnk40[3][MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額(4%分)
					Sn_mnz40[3][MONY_BUF_SIZE],		//				 〃			  消費税額(〃)
					Sn_mnk63[3][MONY_BUF_SIZE],		// 調整課税仕入等に係る棚卸資産額(6.3%分)
					Sn_mnz63[3][MONY_BUF_SIZE],		//				 〃			  消費税額(〃)

					Sn_MENZsw,						// 免税⇔課税切換ｻｲﾝ | | | |･| | | |･|
													//						  ↓	  　↓
													//						切換ｱﾘ	 　 ↓
													//					ON:免税に OFF:課税に
					Sn_tansw;						// 棚卸仕訳発生サイン

	BOOL			m_IsIndiviAnbun;				// 仕入税額 按分方法（TRUE:個別対応, FALSE:比例配分）

protected:
	char			rsv1[512];

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	CArithEx		m_Arith;
	BOOL			m_isTransitionalMeasure;		// 経過措置あり？

protected:
	char			rsv2[256];

public:
	CH26SnTanaData( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH26SnTanaData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData( CSyzSyukei *pSyzSyukei );
	// 現行データ構造からのデータの引継ぎ
	int GetPastData( CDBNpSub *pDBNpSub, int sn_seq );
	// 財務連動
	int ConnectZmdata( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei );

protected:
	// DB からデータ読込み（金額箇所）
	int GetDataMonySub( CDBNpSub *pDBNpSub, int sn_seq );
	// DBへデータ書込み（金額箇所）
	int SetDataMonySub( CDBNpSub *pDBNpSub, int sn_seq );

public:
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
};
