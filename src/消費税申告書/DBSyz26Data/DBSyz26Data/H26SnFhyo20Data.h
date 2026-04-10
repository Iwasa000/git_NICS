#pragma once

//-----------------------------------------------------------------------------
// 付表２、付表２－２データ（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH26SnFhyo20Data
{
public:

	unsigned	char	Sn_2F1A[MONY_BUF_SIZE],		// 課税標準額
						Sn_2F1B[MONY_BUF_SIZE],
						Sn_2F1C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F2A[MONY_BUF_SIZE],		// 免税売上額
						Sn_2F2B[MONY_BUF_SIZE],
						Sn_2F2C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F3A[MONY_BUF_SIZE],		// 輸出資産価額
						Sn_2F3B[MONY_BUF_SIZE],
						Sn_2F3C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F6A[MONY_BUF_SIZE],		// 非課税売上額
						Sn_2F6B[MONY_BUF_SIZE],
						Sn_2F6C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F8A[MONY_BUF_SIZE],		// 課税仕入に係る支払対価の額
						Sn_2F8B[MONY_BUF_SIZE],
						Sn_2F8C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F9A[MONY_BUF_SIZE],		// 課税仕入に係る消費税額
						Sn_2F9B[MONY_BUF_SIZE],
						Sn_2F9C[MONY_BUF_SIZE];
	int					Sn_2F9sw;					//[Y] 課税仕入に係る消費税額　手入力サイン D0: D4:
	
	unsigned	char	Sn_2F10A[MONY_BUF_SIZE],	// 課税貨物に係る支払対価の額
						Sn_2F10B[MONY_BUF_SIZE],
						Sn_2F10C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F11A[MONY_BUF_SIZE],	// 納税義務の免除を受けない（受ける）こととなった場合における消費税額の調整額
						Sn_2F11B[MONY_BUF_SIZE],
						Sn_2F11C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F15A[MONY_BUF_SIZE],	// 個別対応：課税売上に係る
						Sn_2F15B[MONY_BUF_SIZE],
						Sn_2F15C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F16A[MONY_BUF_SIZE],	// 個別対応：共通の売上に係る
						Sn_2F16B[MONY_BUF_SIZE],
						Sn_2F16C[MONY_BUF_SIZE];

	unsigned	char	Sn_2F17A[MONY_BUF_SIZE],	// 付表2｢16｣- 個別対応控除税額手入力額
						Sn_2F17B[MONY_BUF_SIZE],
						Sn_2F17C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F18A[MONY_BUF_SIZE],	// 付表2｢17｣- 比例配分控除税額手入力額
						Sn_2F18B[MONY_BUF_SIZE],
						Sn_2F18C[MONY_BUF_SIZE];

	unsigned	char	Sn_2F19A[MONY_BUF_SIZE],	// 控除税額の調整
						Sn_2F19B[MONY_BUF_SIZE],
						Sn_2F19C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F20A[MONY_BUF_SIZE],	// 控除税額の調整
						Sn_2F20B[MONY_BUF_SIZE],
						Sn_2F20C[MONY_BUF_SIZE];

	unsigned	char	Sn_2F21A[MONY_BUF_SIZE],	// 付表2｢20｣- 控除対象仕入税額手入力額
						Sn_2F21B[MONY_BUF_SIZE],
						Sn_2F21C[MONY_BUF_SIZE];
	int					Sn_2F21sw;					// 控除対象仕入税額　手入力サイン D0: D4:

	unsigned	char	Sn_2F23A[MONY_BUF_SIZE],	// 貸倒回収に係る消費税額
						Sn_2F23B[MONY_BUF_SIZE],
						Sn_2F23C[MONY_BUF_SIZE];
	long				Sn_2FUWS,		// 課税売上割合
						Sn_2FUWSEX;		// 課税売上割合 手入力
protected:
	char				rsv1[512];

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	BOOL			m_isTransitionalMeasure;	// 経過措置あり？
	CArithEx		m_Arith;

protected:
	char				rsv2[256];

public:
	CH26SnFhyo20Data( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH26SnFhyo20Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );
	// 現行データ構造からのデータの引継ぎ
	int GetPastData( CDBNpSub *pDBNpSub, int sn_seq );
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type , int ClearType );//引数追加yoshida140422
//	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
	// 手入力サインのクリア
	int ClearManualInputSign( BOOL isChgKzritu );
};