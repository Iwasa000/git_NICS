#pragma once

//-----------------------------------------------------------------------------
// 特定収入計算表データ（平成31年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;
class CArith;
class CSyzSyukei;
class CH31SnFhyo20Data;

class CH31SpcListData
{
public:
	CH31SpcListData( BOOL isTransitionalMeasure );			 // 標準コンストラクタ
	virtual ~CH31SpcListData();

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
	// 控除対象仕入税額＋課税売上の取込
	//int SetSkjUriMony( SYZTOSPC_DATA *pSyzToSpc );
	int SetSkjUriMony ( SYZTOSPC_DATA *pSyzToSpc, CSnHeadData *pSnHeadData );
	// 特定収入を加味した控除対象仕入税額の取得
	int GetSkjMonyAfterSpc( SYZTOSPC_DATA *pSyzToSpc );
	// 特定収入にかかる調整税額計算（メイン）
	void SPassOffCalqMain( CSnHeadData *pSnHeadData );
	// 特定収入にかかる調整税額計算（サブ）
	void SPassOffCalqSub( CSnHeadData *pSnHeadData, int zeiType );
// 24/02/19wd_特定収入対応 add -->
	// 特定収入にかかる調整税額計算（サブ２／控除対象仕入れに係る調整対象額の合計額の計算）
	void SPassOffCalqSub2( CSnHeadData *pSnHeadData, int zeiType );
// 24/02/19wd_特定収入対応 add <--
	// 特定収入計算テーブルの再初期化
	int ReInitTsTbl();

	// 課税売上割合95%以上？
	BOOL IsRatioOver95( CSnHeadData *pSnHeadData );

// 24/02/14_特定収入 add -->
	void Ts52_DbToTbl( class Csplist* pSp );
// 24/03/26_特定収入 cor -->
	//void Ts52_TblToDb( TSREC52* pTb, class Csplist* pSp, int pSeq, int pPg );
// -------------------------
	void Ts52_TblToDb( TSREC52* pTb, class Csplist* pSp, int pSeq, int pPg, int pVer );
// 24/03/26_特定収入 cor <--
	//void Ts52_Calc( TSREC52* pTr, int pSw );
// 24/02/14_特定収入 add <--

	//// 初期化
	//int Spc_DelData(int pRendo);		// 修正No.168722 add

public:
	BOOL			m_isTransitionalMeasure;	// 経過措置あり？
	CString			m_ErrMsg;					// エラーメッセージ

	CTblhdl			m_TblHdl;					// テーブルハンドル
	CArithEx		m_Arith;					// 倍長演算
	CSyzUtil		m_Util;						// ユーティリティクラス

	struct TB_PAR	SpTbl;						// 特定収入　消費税集計テーブル
	struct TB_PAR	TsTbl;						// 
// 24/02/14_特定収入 add -->
	struct TB_PAR	TsTbl52;					// 特定収入　計算表５－２
// 24/02/14_特定収入 add <--
	struct	_SPCVAL	Spval;						// 特定収入計算表
	struct	_STUVAL	stval;						// 課税売上割合計算用
// 24/04/26特定収入追加 add -->
	BOOL			m_bIs52Data;
// 24/04/26特定収入追加 add <--
};