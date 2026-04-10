#pragma once

//-----------------------------------------------------------------------------
// 付表１、付表４データ（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH26SnFhyo10Data
{
public:

	unsigned char		Sn_1F1A[MONY_BUF_SIZE],		// 課税標準額 税率３％適用分
						Sn_1F1B[MONY_BUF_SIZE],		//     〃     税率４％適用分
						Sn_1F1C[MONY_BUF_SIZE];		//     〃     税率６.３％適用分

	unsigned char		Sn_1F2A[MONY_BUF_SIZE],		// 消費税額   税率３％適用分
						Sn_1F2B[MONY_BUF_SIZE],		//     〃     税率４％適用分
						Sn_1F2C[MONY_BUF_SIZE];		//     〃     税率６.３％適用分
	int					Sn_1F2sw;					//[Y] 消費税額　手入力サイン D0: D4:

	unsigned char		Sn_1F3A[MONY_BUF_SIZE],		// 控除過大調整税額 or 貸倒回収に係る消費税額   税率３％適用分
						Sn_1F3B[MONY_BUF_SIZE],		//     〃                                       税率４％適用分
						Sn_1F3C[MONY_BUF_SIZE];		//     〃                                       税率６.３％適用分

	unsigned char		Sn_1F4A[MONY_BUF_SIZE],		// 控除対象仕入税額   税率３％適用分
						Sn_1F4B[MONY_BUF_SIZE],		//     〃             税率４％適用分
						Sn_1F4C[MONY_BUF_SIZE];		//     〃             税率６.３％適用分

	unsigned char		Sn_1F5A[MONY_BUF_SIZE],		// 返還等対価に係る税額   税率３％適用分
						Sn_1F5B[MONY_BUF_SIZE],		//     〃                 税率４％適用分
						Sn_1F5C[MONY_BUF_SIZE];		//     〃                 税率６.３％適用分

	unsigned char		Sn_1F6A[MONY_BUF_SIZE],		// 貸倒に係る税額   税率３％適用分
						Sn_1F6B[MONY_BUF_SIZE],		//     〃           税率４％適用分
						Sn_1F6C[MONY_BUF_SIZE];		//     〃           税率６.３％適用分

	unsigned char		Sn_1F8A[MONY_BUF_SIZE],		// 控除不足還付税額   税率３％適用分
						Sn_1F8B[MONY_BUF_SIZE],		//     〃             税率４％適用分
						Sn_1F8C[MONY_BUF_SIZE];		//     〃             税率６.３％適用分

	unsigned char		Sn_1F9A[MONY_BUF_SIZE],		// 差引税額   税率３％適用分
						Sn_1F9B[MONY_BUF_SIZE],		//     〃     税率４％適用分
						Sn_1F9C[MONY_BUF_SIZE];		//     〃     税率６.３％適用分

	unsigned char		Sn_1F10T[MONY_BUF_SIZE];	// 合計差引税額

	unsigned char		Sn_1F4T[MONY_BUF_SIZE];		// 控除対象仕入税額   合計分

protected:
	char				rsv1[506];					// '14.10.17	512 -> 506

private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	CArithEx			m_Arith;
	BOOL				m_isTransitionalMeasure;	// 経過措置あり？

protected:
	char				rsv2[256];

public:
	CH26SnFhyo10Data( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH26SnFhyo10Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// 集計済みデータのセット
	int SetCalqedData( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );
	// 現行データ構造からのデータの引継ぎ
	int GetPastData( CDBNpSub *pDBNpSub, int sn_seq );

protected:
	// 集計済みデータのセット(付表１)
	int SetCalqed10Data( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );
	// 集計済みデータのセット(付表４)
	int SetCalqed40Data( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );

public:
	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );
	// 手入力サインのクリア
	int ClearManualInputSign( BOOL isChgKzritu );
	// 横計ではない例外計算が必要？ ('14.10.17)
	BOOL IsNeedExpCalq( CDBNpSub *pDBNpSub );
	// 更新時の金額追加書き込み('17.12.08)
	int SetDataInRenew( CDBNpSub *pDBNpSub, int sn_seq );
};