#pragma once

//-----------------------------------------------------------------------------
// 還付申告データ（平成26年改正）
//-----------------------------------------------------------------------------

class CH31SnRefundData
{
public:
	// 本表
	REFUNDREC	RefundJot[10];		// 主な課税資産の譲渡等
	REFUNDREC	RefundExp[10];		// 主な輸出取引等の明細
	REFUNDREC	RefundStk[20];		// 仕入金額等の明細	(個人=13／法人=10)
	REFUNDREC	RefundTna[10];		// 主な棚卸資産・原材料等の取得
	REFUNDREC	RefundKot[10];		// 主な固定資産等の取得
	// 明細（次葉）
	REFUNDREC	RefundRec;

	struct TB_PAR	JotTbl;								// 主な課税資産の譲渡等
	struct TB_PAR	ExpTbl;								// 主な輸出取引等の明細
	struct TB_PAR	TnaTbl;								// 主な棚卸資産・原材料等の取得
	struct TB_PAR	KotTbl;								// 主な固定資産等の取得

private:
	CTblhdl		m_TblHdl;			// テーブルハンドル

public:
	CH31SnRefundData();				// 標準コンストラクタ
	virtual ~CH31SnRefundData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );
	// 還付申告 全テーブル削除
	int DeleteAllData( CDBNpSub *pDBNpSub, int sn_seq );
	// 初期化によるデータクリア
	int ClearDataByInit();

//2016.02.23 INSERT START
	int ClearData_Shiire( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq ); //3-(1) 仕入金額等の明細のクリア
//2016.02.23 INSERT END

	// 財務連動
//-- '15.09.30 --
//	int ConnectZmdata( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl, EnumIdIcsShRevType ShRevtype );
//---------------
	int ConnectZmdata( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei, CTblhdl *pTblhdl, _TB_PAR *pSkjCnvtbl, EnumIdIcsShRevType ShRevtype, MoneyBasejagArray &MoneyBaseArray );
//---------------

	// 控除対象仕入税額の更新
	int ConnectSkjZeigaku( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei );

	// 仮受・仮払消費税？
	bool IsKariukeKaribaraiShouhizei( CDBNpSub *pDBNpSub, CString kamokuCode );

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
	CArithEx		m_Arith;
};