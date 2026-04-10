#pragma once

//-----------------------------------------------------------------------------
// 付表１、付表４データ（マイナンバー対応）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH28SnFhyo10Data : public CH26SnFhyo10Data
{

public:
	unsigned char		Sn_1F1_1A[MONY_BUF_SIZE],		// 課税資産の譲渡等の対価の額　			　税率３％適用分
						Sn_1F1_1B[MONY_BUF_SIZE],		//				〃						　税率４％適用分
						Sn_1F1_1C[MONY_BUF_SIZE];		//				〃						　税率６.３％適用分

	unsigned char		Sn_1F1_2C[MONY_BUF_SIZE];		// 特定課税仕入れに係る支払対価の額　	　税率６.３％適用分

	unsigned char		Sn_1F5_1A[MONY_BUF_SIZE],		// 売上げの返還等対価に係る税額　		　税率３％適用分
						Sn_1F5_1B[MONY_BUF_SIZE],		//				〃				　		　税率４％適用分
						Sn_1F5_1C[MONY_BUF_SIZE];		//				〃				　		　税率６.３％適用分

	unsigned char		Sn_1F5_2C[MONY_BUF_SIZE];		// 特定課税仕入れの返還等対価に係る税額　 税率６.３％適用分
						

protected:
	char				rsv1[512];					// リザーブ

private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;
	CArithEx			m_Arith;
	BOOL				m_isTransitionalMeasure;	// 経過措置あり？

protected:
	char				rsv2[256];

public:
	CH28SnFhyo10Data( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH28SnFhyo10Data();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, int sn_seq );

	// DBへデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );

	// 集計済みデータのセット
	int SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei );

	// 更新時の金額クリア
	int ClearDataByRenew( CDBNpSub *pDBNpSub, int type );



/*	// 現行データ構造からのデータの引継ぎ
	int GetPastData( CDBNpSub *pDBNpSub, int sn_seq );
	
	// 手入力サインのクリア
	int ClearManualInputSign( BOOL isChgKzritu );
	// 横計ではない例外計算が必要？ ('15.05.07)
	BOOL IsNeedExpCalq();
	*/

};
