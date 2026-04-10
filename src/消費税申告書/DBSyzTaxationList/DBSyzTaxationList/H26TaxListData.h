#pragma once

//-----------------------------------------------------------------------------
// 課税取引金額計算書データ（平成26年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

#include	<SyzSyukei8.h>

class CH26TaxListData
{
public:
	CH26TaxListData();   // 標準コンストラクタ
	virtual ~CH26TaxListData();

	// DB からデータ読込み
	int GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );
	// DB へデータ書込み
	int SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq );

	// 財務連動('14.08.08) 使用されている可能性があるため、関数は残しておくが使用禁止
	int ConnectZmdata( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, int smon, int emon, BOOL IsTransitionalMeasure );
	// 財務連動('20.12.28) こちらを使用 簡易計算時の別記の集計処理等修正
	int ConnectZmdata( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, int smon, int emon, BOOL IsTransitionalMeasure, CSnHeadData *pSnHeadData );
	// 書き込みオフセットの取得('14.08.08)
	int GetRecodeNo( char type, char ctg, int cnt );
	// 特定課税仕入関連の科目？('15.08.26)
	BOOL IsTkkzRelKmk( MoneyBase * pMbase );

//2016.06.22 INSERT START
	int GetKoketsuMeisyo( CDBNpSub *pDBNpSub, int type, int no, CString &name );
//2016.06.22 INSERT END

	// 個人決算書連動？('16.12.07)
	bool IsLinkKjprfname();

protected:
	// 科目名称の読み込み('14.08.08)
	int ReadKnTabl( CDBNpSub *pDBNpSub );


public:
	// 課税取引金額計算表
	KJNHEAD		Head;									// 管理部
	KJNREC		NRec;									// 計算表集計レコード
	BYTE		YEAR;									// 年
	BYTE		GKBN;									// 業種区分	
	BYTE		KniFlg;									// 非連動ｄｂで簡易課税　D0:ON [13'09.18]
	BYTE		KniDmm;
//	int			ViewFlg;
//	int			m_PNO[10];
//	char		m_CpnSw;
//	char		m_Basic;
//	long		kdat1, kdat2;							//	単位：1/10mm
//	int			pPage[10];								// 出力指定ページ	
//	char		ErrMsg[256];

	struct TB_PAR		KnTbl;

	int			Y_GENGO;								// 元号

private:
	CTblhdl			m_TblHdl;			// テーブルハンドル
	CArithEx		m_Arith;			// 倍長演算
	CSyzUtil		m_Util;
};