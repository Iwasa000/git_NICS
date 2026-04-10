#pragma once

//-----------------------------------------------------------------------------
// 更正の請求（平成26年改正）
//-----------------------------------------------------------------------------

#include "DBSbillData.h"

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class CH26SnKskData
{
public:
	KSIHEAD	KsHead;		// 更正の請求 管理部
	KSIDATA	KsData;		// 更正の請求 データ部

public:
	CH26SnKskData();   // 標準コンストラクタ
	virtual ~CH26SnKskData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// データクリア
	void ClearData();
	// データ計算
	int CalqBillVal( CSnHeadData *pSnHeadData );
	// 集計済みデータのセット
	int SetCalqedData( int mode, SH_ANS *pAns );
	// 金融機関の取込
	void SetReaskFinancial( CSnHeadData *pSnHeadData );

	// 年月日データの取得
	void GetYmdData( EnumIdIcsShDateType dsDtype, char *pYy, char *pMm, char *pDd );
	// 年月日データの取込
	void SetYmdData( EnumIdIcsShDateType dsDtype, int type, char data );

private:
	CDBSyzAccess	m_DBSyzAccess;
	CSyzUtil		m_Util;
};
