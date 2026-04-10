// 自動仕訳 のクラス

#ifndef	_SWKJOB_INCLUDE_
#define	_SWKJOB_INCLUDE_


#include <DBNPSUB8.H>


typedef struct _SHKN_KMK
{
	DWORD		chkcod;		// チェックコード
	CDWordArray	chkArray;	// チェックコードエリア
} SHKN_KMK;


typedef struct _NPAUTO_SWK
{
	DWORD	inpDeb;
	DWORD	inpCre;
	DWORD	autDeb;
	DWORD	autCre;

} NPAUTO_SWK;


class CSwkJob
{
public:
	CSwkJob();
	virtual ~CSwkJob();

public:
	int Initial();
	int MemdataMake();

	int IsAutoSwk();
	int	AutoSwk( CDBINPDataRec* atrec, CDBINPDataRec* crec );
	int IsShikin( CDBINPDataRec* data, int dbcr_sg );

	void KeepCrec(CDBINPDataRec* crec);
	void GetMotoRec(CDBINPDataRec* moto);
	void KeepKrswk(CDBINPDataRec* pkrswk);
	void GetKrswk(CDBINPDataRec* pkrswk );

	// 切返し仕訳モードか
	BOOL& IsKrmod() { return m_bKrmod; };

	void GetAutoSwk(CDBINPDataRec* pKrec, CDBINPDataRec* crec, int index);

public:
	//同じ資金仕訳が複数登録されていた場合、選択させるために保存
	CArray<NPAUTO_SWK, NPAUTO_SWK&>	m_Npsame;

private:
	CDBNpSub*	m_pNPb;
	int m_SHKN_OPN_MODE;
	
	// 資金・非資金 資産・負債のテーブル
	SHKN_KMK	m_Sisan[2];
	SHKN_KMK	m_Husai[2];

	CArray<NPAUTO_SWK, NPAUTO_SWK&>	m_NpAuto;

	BOOL			m_bKrmod;	// 切返し仕訳の自動入力モード

	CDBINPDataRec	m_Moto;		// 自動仕訳　対象
	CDBINPDataRec	m_Krirec;	// 保存切返し仕訳
};


#endif //_SWKJOB_INCLUDE_ 