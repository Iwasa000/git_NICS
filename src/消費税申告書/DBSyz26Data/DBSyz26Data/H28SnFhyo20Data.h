#pragma once

//-----------------------------------------------------------------------------
// 付表２、付表２－２データ（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH28SnFhyo20Data : public CH26SnFhyo20Data
{

public:
	
	unsigned	char	Sn_2F10C[MONY_BUF_SIZE];	// 特定課税仕入れに係る支払対価の額
	unsigned	char	Sn_2F11C[MONY_BUF_SIZE];	// 特定課税仕入れに係る消費税額
	int					Sn_2F11sw;					// 特定課税仕入れに係る消費税額　手入力サイン(D3:, D7:, D11:6.3%)
	unsigned	char	Sn_2F12A[MONY_BUF_SIZE],	// 課税貨物に係る消費税額
						Sn_2F12B[MONY_BUF_SIZE],
						Sn_2F12C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F13A[MONY_BUF_SIZE],	// 納税義務の免除を受けない（受ける）こととなっ・・
						Sn_2F13B[MONY_BUF_SIZE],
						Sn_2F13C[MONY_BUF_SIZE];

	unsigned	char	Sn_2F16A[MONY_BUF_SIZE],	// 個別対応：課税売上にのみ要するもの
						Sn_2F16B[MONY_BUF_SIZE],
						Sn_2F16C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F17A[MONY_BUF_SIZE],	// 個別対応：課税売上と非課税売上に共通して要するもの
						Sn_2F17B[MONY_BUF_SIZE],
						Sn_2F17C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F18A[MONY_BUF_SIZE],	// 個別対応：控除する課税仕入れ等の税額
						Sn_2F18B[MONY_BUF_SIZE],
						Sn_2F18C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F19A[MONY_BUF_SIZE],	// 比例配分：控除する課税仕入れ等の税額
						Sn_2F19B[MONY_BUF_SIZE],
						Sn_2F19C[MONY_BUF_SIZE];
	
	unsigned	char	Sn_2F20A[MONY_BUF_SIZE],	// 控除税額の調整
						Sn_2F20B[MONY_BUF_SIZE],
						Sn_2F20C[MONY_BUF_SIZE];
	unsigned	char	Sn_2F21A[MONY_BUF_SIZE],	// 控除税額の調整
						Sn_2F21B[MONY_BUF_SIZE],
						Sn_2F21C[MONY_BUF_SIZE];

	unsigned	char	Sn_2F22A[MONY_BUF_SIZE],	// 付表2｢22｣- 控除対象仕入税額手入力額
						Sn_2F22B[MONY_BUF_SIZE],
						Sn_2F22C[MONY_BUF_SIZE];
	int					Sn_2F22sw;					// 控除対象仕入税額　手入力サイン D0: D4:

	unsigned	char	Sn_2F24A[MONY_BUF_SIZE],	// 貸倒回収に係る消費税額
						Sn_2F24B[MONY_BUF_SIZE],
						Sn_2F24C[MONY_BUF_SIZE];


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
	CH28SnFhyo20Data( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH28SnFhyo20Data();

	
	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );

	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );

	// 集計済みデータのセット
	int SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );

	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type , int ClearType );
	

	/*
	// 現行データ構造からのデータの引継ぎ
	int GetPastData( CDBNpSub *pDBNpSub, int sn_seq );
	// 手入力サインのクリア
	int ClearManualInputSign( BOOL isChgKzritu );
	*/
};
