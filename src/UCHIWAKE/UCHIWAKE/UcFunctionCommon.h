//--------------------------------------------------
//	UcFunctionCommon.h
//
//	2006.08.25～
//--------------------------------------------------

#ifndef __UCFUNCTIONCOMMON_H__
#define __UCFUNCTIONCOMMON_H__

#include "StdAfx.h"
//#include "UCHIWAKE.h"
#include "dbUcRenKamoku.h"
#include "dbUc000Common.h"

#define ID_DIAGDATA_DISP	0
#define ID_DIAGDATA_EDIT	1

#pragma once

// 158063 add -->
typedef struct _AMOUNT_REC1	{
	int		code;	// 取引先コード
	CString	name;	// 取引先名称
	CString	valu;	// 総額
}AMOUNTREC1;

typedef struct _AMOUNT_REC2	{
	int		tcode;	// 取引先コード
	int		kcode;	// 科目コード
	CString	tname;	// 取引先名称
	CString	valu;	// 総額
}AMOUNTREC2;
// 158063 add <--

class CUcFunctionCommon
{
public:
	CUcFunctionCommon(void);
public:
	~CUcFunctionCommon(void);

public:
	int		GetKcdKubun(CString inKcd);
	BOOL	GetKcdFromTo(CString inKcdStart, CString* outFrom, CString* outTo);
	CString	Trim(CString strOrg);
	int		GetTekiyoData( CDBZmSub* ZmSub , long lngtkcod , CString *strtknam );
// midori 152137 add -->
	int		GetTekiyoKana( CDBZmSub* ZmSub , long lngtkcod , CString *strtknam );
// midori 152137 add <--
	int		CheckKcdStart(int inFormSeq, BYTE inFgCoType, CString inKcdStart );
	BOOL	ZmSubErrMsg( CDBZmSub* pZmSub );
	void	ICSStaticEnableWindow( ICSStatic* lblData , BOOL blnFlg );
	BOOL	CheckTableOpen( CdbBase* mfcRec );
	CString DeleteRightSpace(CString strData);
// midori 190301 add -->
	CString DeleteRightCrlf(CString strData);
// midori 190301 add <--
	void	DeleteNullRow(CStringArray* pstrArray);
	void	DeleteNullRow(CString* pstrTarget1, CString* pstrTarget2);
	void	StrDivision(CString strTarget, CStringArray* pstrArray, int intRow, BOOL blnDelSpace = FALSE, BOOL blnDelNullRow = FALSE );
	void	StrDivisionEx( CString strData , CStringArray* strArry , int intMax , int intRow );
	CString StrDocking(CStringArray* pstrArray);
	CString	StrDocking(CString strTarget1, CString strTarget2);
	void	DiagInit(DIAGRAM_DATA* diadata);
	CString DiagGetString(CICSDiag* pDiag, int intIndex);
// インボイス登録番号追加対応_23/11/06 add -->
	CString	DiagGetDisp(CICSDiag* pDiag, int intIndex);
	short	DiagGetCombo(CICSDiag* pDiag, int intIndex);
// インボイス登録番号追加対応_23/11/06 add <--
	void	DlgMoveCenter(CWnd* pParent);
	CString GetGrNo(BOOL type = 0);
	CString GetFileNameAddGR(CString strFile);
	int		DiagSetString(CICSDiag* pDiag, int intIndex, int flg, CString str);
	//int		GetDividedName(CString strBase, CString& strResult1, CString& strResult2, int nDivideByte);
	//int		GetDividedName(CString strBase, CString& strResult, int nDivideByte);
	void	ChangeReportColumnAll(ICSReportControl* report, int clm);
	int		GetReportCheckNum(ICSReportControl* report, int clm, int row_max=0);
	int		IsExistData(CDatabase* pdb, int index, int mode, int return_type);
	CdbUc000Common*	CreateDbObject(int intFormSeq, CDatabase* pdb);
	int		GetFocusControlID( CWnd* pParent=NULL);
// midori 153907 del -->
	//int		CompareNoCRLF(const CString& str1, const CString& str2);
// midori 153907 del <--
	//CString	ConvertFurigana(CRevConv& StrConv, CString strBase);
// midori 152137 add -->
	int		GetSortItem(int intFormSeq,int intItemSeq);
	int		IsKamokuSort(int nFormSeq, CDatabase* pdb);
	void	IsSort(int nFormSeq, CDatabase* pdb,int* rbank,int* raddress);
// midori 152137 add <--
// midori 180104 add -->
	// 会社情報の取込み
	int		SetCpdataByCpinfo( int type, CWnd* pWnd );
// midori 180104 add <--
// midori 190301 add -->
	int		GetKaigyoCount( CString strTarget );
	CString GetSpritString( CString szSrc, CString* szDst, int nPos );
// midori 190301 add -->
// midori 191002 add -->
	void	ChgKeiStr( CString szSrc, CString* szDst );
	void	KeiStrSprit(CString szSrc, CString* szDst, CString* szDst2);
// midori 191002 add <--
// midori 191201 add -->
// midori 151730,151263 del -->
	//int		GetRekinen(CDatabase* pdatabase,int apno,int pver,int* pRekinen);
	//int		ChkRekinenAp(int apno,int pver);
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	int		GetRekinen(CDBNpSub* pnpmod);
// midori 151730,151263 add <--
	void	YmdConvert( CDBZmSub* pZmSub,int pSw );
// midori 151258,151262 add -->
	void	YmdConvert2( CDBZmSub* pZmSub,int pSw );
// midori 151258,151262 add <--
// midori 151730,151263 add -->
	void	YmdConvert3( CDBZmSub* pZmSub,int pSw );
// midori 151730,151263 add <--
	void	GetYmd(int pRekinen,int pymd,int *rgengo,int* rymd);
	void	GetYmd2(int ss_ymd,int pRekinen,int pymd,int *rgengo,int* rymd);
// midori 151258,151262 add -->
	void	GetYmd3(int ss_ymd,int pRekinen,int pymd,int* rymd);
// midori 151258,151262 add <--
// midori 191201 add <--
// midori 155882 add -->
	void	YmdChg(CDBZmSub* pZmSub,CDBPrtbl* pt);
// midori 155882 add <--
// 158063 add -->
	CString	MakeAmountSql1( CDatabase* pDB, int pFormSeq );
	CString MakeAmountSql2( CDatabase* pDB, int pFormSeq );
	void	MakeAmountMap1( CDatabase* pDB, int pFormSeq );
	void	MakeAmountMap2( CDatabase* pDB, int pFormSeq );
	CString	GetAmount1( int pCode, CString pName );
	CString GetAmount2( int pTcode, CString pTname, int pKcode );
	CMap<int,int,AMOUNTREC1,AMOUNTREC1>	m_AmountMap1;
	CMap<int,int,AMOUNTREC2,AMOUNTREC2>	m_AmountMap2;
// 158063 add <--
};

#endif __UCFUNCTIONCOMMON_H__