///////////////////////////////////////////////////////////
// 工事名称テーブル 読み取りクラス
//		DBInpSub.lib
//		DBInpSub.dll
//
//


#pragma once


class CKJKKI
{
public:
	CKJKKI();

	__declspec(dllexport) int Requery( LPCTSTR filter );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();

	int		st;

	CString		KKI_KJCD;
	CString		KKI_KN50;
	BYTE		KKI_PGEN;
	long		KKI_PYMD;
	BYTE		KKI_CGEN;
	long		KKI_CYMD;
	BYTE		KKI_YGEN;
	long		KKI_YYMD;
	BYTE		KKI_KGEN;
	long		KKI_KYMD;
	BYTE		KKI_HAIF;
	BYTE		KKI_FURI;
	long		KKI_HSAG;
	CString		KKI_HACD;
	CString		KKI_EICD;
	CString		KKI_GBCD;
	long		KKI_RIT01;
	long		KKI_RIT02;
	long		KKI_RIT03;
	CString		KKI_NAME;
	CString		KKI_RYNM;
	CString		KKI_SENM;
	CString		KKI_BIKO;
	CString		KKI_GENB;
	long		REG_DATE;
	long		REG_TIME;
	int			REG_USER;
	long		UPD_DATE;
	long		UPD_TIME;
	int			UPD_USER;

//protected:
	char	rsv[256];
	class	CKJKKIrecord*	m_rec;

	int		nodata;
	int		reqst;
	void	data();
};


// 工事名称読み込みクラス
#ifdef _DBKJNAM_
class AFX_EXT_CLASS CKJnamAccess
#else
class CKJnamAccess
#endif
{
public:
	CKJnamAccess();
	virtual ~CKJnamAccess();

	//-----------------------------------------------------------
	//	マスターのDB に接続済みの財務クラスをセット
	//
	//-----------------------------------------------------------
	void	SetDBZmSub( CDBZmSub* pZmSub );

	//-----------------------------------------------------------
	//	工事名称テーブルを読み込みます。
	//		返送値	-1: エラー
	//				 0: OK
	//
	//		工事マスターが作成されていない場合でも 0 を返送します。
	//		その場合、kjnam には NULL をセットします。
	//
	//-----------------------------------------------------------
	int		KjNameOpen( LPCTSTR filter = _T(""));
	void	KjNameClose();

public:
	CKJKKI*	kjnam;

	char	rsv[128];

private:
	CDBZmSub*	pZm;
	char	rsv2[256];
};