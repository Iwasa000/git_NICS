#pragma once


//-----------------------------------------------------------------------------
// 更正の請求（平成26年改正分）
//-----------------------------------------------------------------------------

#include "DBSbillData.h"


class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;



class SYZSHIN31DATA_CLASS_EXPORT CH31SnKskData
{

public:

	char*				rsv;

	KSIHEAD				KsHead;				// 更正の請求 管理部
	KSIDATA				KsData;				// 更正の請求 データ部

	
	
protected:
	char				rsv1[512];


private:
	CDBSyzAccess		m_DBSyzAccess;
	CSyzUtil			m_Util;


protected:
	char				rsv2[256];


public:
	CH31SnKskData(void);
	~CH31SnKskData(void);


	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, int sn_seq );
	// データクリア
	void ClearData();

	//＋＋＋＋＋＋＋更正の請求は集計はアプリ側で行う＋＋＋＋＋＋＋//

	// データ計算
	int CalqBillVal( CSnHeadData *pSnHeadData );
	// 集計済みデータのセット
	int SetCalqedData( int mode, SH_ANS *pAns );
	// 集計済みデータのセット 令和4年12月31日以後開始分
	int SetCalqedData2( int mode, SH_ANS *pAns );
	// 金融機関の取込
	void SetReaskFinancial( CSnHeadData *pSnHeadData );
	// テーブル拡張されているか
	int TableCheck( CDBNpSub *pDBNpSub );




};
