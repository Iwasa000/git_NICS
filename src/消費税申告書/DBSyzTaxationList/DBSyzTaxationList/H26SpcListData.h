#pragma once

//-----------------------------------------------------------------------------
// 特定収入計算表データ（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;
class CArith;
class CSyzSyukei;
class CH26SnFhyo20Data;

class CH26SpcListData
{
public:
	CH26SpcListData( BOOL isTransitionalMeasure );			 // 標準コンストラクタ
	virtual ~CH26SpcListData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );
	// 特定収入集計テ－ブルの作成＆読み込み
	int InitSpData( CDBNpSub *pDBNpSub );
	// 課税売上割合計算用集計
	int CalqForUriRatio( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, CSnHeadData *pSnHeadData, int smon, int emon );
	// 科目名称の取得
	int GetKnrecNameEx( CDBNpSub *pDBNpSub, unsigned char *Code, char *Name, long &Seq );
	// 控除対象仕入税額＋課税売上の取込('15.04.03)
	int SetSkjUriMony( SYZTOSPC_DATA *pSyzToSpc );
	// 特定収入を加味した控除対象仕入税額の取得('15.04.04)
	int GetSkjMonyAfterSpc( SYZTOSPC_DATA *pSyzToSpc );

//-- '15.03.12_CUT --
//	// 特定収入にかかる調整税額計算(経過措置無し)
//	void SPassOffCalqEx( CSnHeadData *pSnHeadData );
//	// 特定収入に係る調整税額計算(経過措置有り)
//	void SPassOnCalqEx( CSnHeadData *pSnHeadData );
//-------------------
	// 特定収入にかかる調整税額計算（メイン）
	void SPassOffCalqMain( CSnHeadData *pSnHeadData );
	// 特定収入にかかる調整税額計算（サブ）
	void SPassOffCalqSub( CSnHeadData *pSnHeadData, int zeiType );
	// 特定収入計算テーブルの再初期化
	int ReInitTsTbl();

	// 課税売上割合95%以上？	('15.03.20)
	BOOL IsRatioOver95( CSnHeadData *pSnHeadData );

public:
	BOOL			m_isTransitionalMeasure;	// 経過措置あり？
	CString			m_ErrMsg;					// エラーメッセージ

	CTblhdl			m_TblHdl;					// テーブルハンドル
	CArithEx		m_Arith;					// 倍長演算
	CSyzUtil		m_Util;						// ユーティリティクラス

	struct TB_PAR	SpTbl;						// 特定収入　消費税集計テーブル
	struct TB_PAR	TsTbl;						// 
	struct	_SPCVAL	Spval;						// 特定収入計算表
	struct	_STUVAL	stval;						// 課税売上割合計算用
};