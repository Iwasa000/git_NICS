// midori UC_0005 del -->
//// midori 170504 add -->
//#include "..\..\UCHIWAKE\UCHIWAKE\dbUc000Common.h"
//// midori 170504 add <--
// midori UC_0005 del <--

#ifndef	_DBUCHIIMP8_H_
#define _DBUCHIIMP8_H_

// midori 161103 -->
__declspec(dllexport) BOOL ZenUpdate( CWnd* pWnd, int pKakutei );
__declspec(dllexport) void ZenGetErrorMsg( CString& pErr );
// midori 161103 <--

// midori 152376 add -->
// 勘定科目内訳書翌期更新サインの取得	0:更新不可	1:更新可能 -1:エラー
__declspec(dllexport) int ZenGetSgn( void );
// midori 152376 add <--

// midori UC_0005 add -->
// 勘定科目内訳書マスターが作成されているかチェックを行う
__declspec(dllexport) int ChkUti( CDBZmSub* pZmsub );
// 翌期更新時、過年度確定サインを更新する
__declspec(dllexport) int YokUpdate( CWnd* pWnd, int pKakutei );
// midori UC_0005 add <--

// midori UC_0010 add -->
__declspec(dllexport) int VersionUp( CDBZmSub* pZms, CString pSqlServer, int pApNo, int pCoCode, int pKikan, int cosel );
// midori UC_0010 add <--

// midori 180503 add -->
// midori UC_0048 del -->
//// midori UC_0039 del -->
////__declspec(dllexport) int UchiYokukiUpdateMainProc( CWnd* pWnd, CDBZmSub* pZmSub, long lngUchiSSymd, long lngUchiEEymd );
//// midori UC_0039 del <--
//// midori UC_0039 add -->
//__declspec(dllexport) int UchiYokukiUpdateMainProc( CWnd* pWnd, CDBZmSub* pZmSub, long lngUchiSSymd, long lngUchiEEymd, int pHirendo );
//// midori UC_0039 add <--
// midori UC_0048 del <--
// midori UC_0048 add -->
__declspec(dllexport) int UchiYokukiUpdateMainProc( CWnd* pWnd, CDBZmSub* pZmSub, long lngUchiSSymd, long lngUchiEEymd );
// midori UC_0048 add <--
// midori 181002 0221 del -->
//__declspec(dllexport) int UchiYokukiUpdateMainProc2( CWnd* pWnd, CDBZmSub* pZmSub,int exupd );
// midori 181002 0221 del <--
// midori 181002 0221 add -->
__declspec(dllexport) int UchiYokukiUpdateMainProc2( CWnd* pWnd, CDBZmSub* pZmSub );
// midori 181002 0221 add <--
// midori 181002 0223 del -->
//__declspec(dllexport) int UchiYokukiUpdateMainProc2_2( CWnd* pWnd, CDBZmSub* pZmSub );
// midori 181002 0223 del <--
// midori 181002 0223 add -->
__declspec(dllexport) int UchiYokukiUpdateMainProc2_2( CWnd* pWnd, CDBZmSub* pZmSub, int pSw );
// midori 181002 0223 add <--
// midori 181001 add -->
__declspec(dllexport) int UchiYokukiUpdateMainProc3( CWnd* pWnd, CDBZmSub* pZmSub );
__declspec(dllexport) int UchiYokukiUpdateMainProc3_2( CWnd* pWnd, CDBZmSub* pZmSub );
// midori 181001 add <--
// midori 181101 del -->
//__declspec(dllexport) void UchiCnvDbToBCD( long pYmd, UCHAR* pYear, UCHAR* pMon, UCHAR* pDay );
// midori 181101 del <--
// midori 181101 add -->
__declspec(dllexport) void UchiCnvDbToBCD( long pYmd, UCHAR* pMon, UCHAR* pDay );
// midori 181101 add <--
// midori 180503 add <--

// midori 181002 add -->
__declspec(dllexport) int MasterCheck(void);
__declspec(dllexport) int DeleteUtiTables(CDBZmSub* zmsub,int pSw,int kikan);
__declspec(dllexport) int ChkCoRec(int apno,int code,int eymd,CString* errmsg);
__declspec(dllexport) int SetCoRec(int apno,int code,int eymd,int pSw,CString* errmsg);
// midori 181002 add <--
// midori UC_0035 add -->
__declspec(dllexport) int SetCoRec2(int apno,int code,int eymd,int pSw,CString* errmsg);
// midori UC_0035 add <--

// 24/02/09_ScoreLink add -->
__declspec(dllexport) BOOL SL_CsvExport( void );
__declspec(dllexport) void SL_GetErrorMsg( CString& pErr );
// 24/02/09_ScoreLink add <--

// ------------------------------------------------------------------------------------------
// 勘定科目内訳書データインポート／エクスポートクラス
// ------------------------------------------------------------------------------------------
class CDBUchiImp
{
public:
	__declspec(dllexport) CDBUchiImp(void);
	__declspec(dllexport) virtual~CDBUchiImp(void);

public:
	// データインポート
	__declspec(dllexport) int Import(CDBZmSub* pZmSub, CWnd* pParent = NULL);
	__declspec(dllexport) int Import(CDBZmSub* pZmSub, CString strFolderPath, CWnd* pParent = NULL);
	__declspec(dllexport) int ImportForDenshi(CDBZmSub* pZmSub,CString strFolderPath, CWnd* pParent = NULL);

	// データエクスポート
	__declspec(dllexport) int Export(CDBZmSub* pZmSub, CWnd* pParent = NULL);
	__declspec(dllexport) int Export(CDBZmSub* pZmSub, CString strFolderPath, CWnd* pParent = NULL);

protected:
	CWnd*					m_pWnd;					// 親ウィンドウのポインタ

	BOOL		SearchFolder(CString strFolderPath);
	CString		GetFolderPath(BOOL fImport);
	void		SaveFolderPath(BOOL fImport, CString strFolderPath);
	CString		GetIniKey(BOOL fImport);
	CString		GetIcswinDrive(void);								// [2009/07/27 Add]
	CString		GetGrNo(BOOL strType = 0);

private:
	ICSProgressWnd*	InitProgress( CWnd* pParent, int start, int end, BOOL imp );
	ICSProgressWnd*	HideProgress( ICSProgressWnd* pPrgWnd, CWnd* pParent, int start, int end, BOOL imp );
	void		UpdateProgress( ICSProgressWnd* pPrgWnd, int pos );
	int			ShowProgress( ICSProgressWnd* pPrgWnd, int pos );
// midori UC_0005 del -->
//// midori 170502 add -->
//	void				CmnTblUpdatePageRowAllResetMain(CDBZmSub* pZmSub,CdbUcInfSub* pUcInfSub);
//	CdbUc000Common*		GetDataTableObject(CDBZmSub* pZmSub,int pFormSeq);
//	int					CmnTblDeleteFgFunc(CDBZmSub* pZmSub,int inFgFuncKind,int pFormSeq);
//	int					CmnTblUpdatePageNumRowNumReset(CDBZmSub* pZmSub,int inPage, int inRow,int pFormSeq,int pintFormType,int pintOutKei);
//	int					CmnTblCountMaxRow(int pFormSeq,int pintFormType,int pintOutKei);
//	int					CmnTblUpdateEndPageSetting(CDBZmSub* pZmSub,int pFormSeq);
//	int					CmnUcInfFormTypeGetFormType(CDBZmSub* pZmSub,int* intFormType);
//	void				GetFormRow(int pFormSeq,int pintFormType,int intOutKei,int* pintRowMax,int* pintRowMaxData,int* pintRowKei);
//	int					CmnTblInsertPageKeiRuiKei(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int NameKei);
//	int					CmnGetNextPageRow(int* pPage, int* pRow,int intRowMaxData);
//	int					CmnTblInsertNullLine(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int nPage, int nRow, int nLine);
//	int					CmnTblInsertSpecialLine(CDBZmSub* pZmSub,int pFormSeq,int nPage, int nRow, int nType, CString strName);
//	int					CmnTblCalcKei(CDBZmSub* pZmSub,int pFormSeq,int intOutKei,BOOL bPrintZeroData,BOOL bPrintNullData);
//	void				CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag = 1);
//	BOOL				GetShowKeiZero(CString strTemp);
//	BOOL				CmnUcInfFormTypeGetFormType(CDBZmSub* pZmSub,int intFormSeq,int* intFormType);
//// midori 170502 add <--
// midori UC_0005 del <--
// midori 170504 add -->
	CString				GetSpritString( CString szSrc, CString* szDst, int nPos );
// midori 170504 add <--

protected:
	char		rsv[256];											// [2009/08/25 Add]
};

#endif /* _DBUCHIIMP8_H_ */
