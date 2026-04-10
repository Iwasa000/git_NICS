//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtJobFunction.h
//
// ■印刷関連 動作定義
//
//**************************************************************************************************

#ifndef	__UCHIWAKE__PRTJOBFUNCTION_H__
#define	__UCHIWAKE__PRTJOBFUNCTION_H__

#pragma once

#include "MainFrm.h"
#include "UcCommon.h"

// ICS
#include <XYSPrn8.h>			// 帳表スケジュールによるデータ出力用ヘッダ
								// （内部でICSPrn8.hをインクルード）

// Print
#include "prtConstData.h"		// 定数定義

// Date
#include "dateConvert.h"		// 日付変換

// DB
#include "dbUcInfMain.h"			// 
#include "dbUcInfSub.h"				// 
#include "dbUcInfSubOmit.h"			// 
#include "dbUcInfFormType.h"		// 
#include "dbUcInfSonotaItem.h"		// 
#include "dbUc000Common.h"			// 
#include "dbUc011Yotyokin.h"		// 
#include "dbUc021Uketoritegata.h"	// 
#include "dbUc031Urikakekin.h"		// 
#include "dbUc041Karibarai.h"		// 
#include "dbUc042Kasituke.h"		// 
#include "dbUc051Tanaoroshi.h"		// 
#include "dbUc052Tanaoroshi2.h"		// 
#include "dbUc061Yuukasyouken.h"	// 
#include "dbUc071Koteishisan.h"		// 
#include "dbUc081Siharaitegata.h"	// 
#include "dbUc091Kaikakekin.h"		// 
#include "dbUc092Kaikakekin2.h"		// 
#include "dbUc101Kariukekin.h"		// 
#include "dbUc102Gensen.h"			// 
#include "dbUc111Kariirekin.h"		// 
#include "dbUc121Totiuriage.h"		// 
#include "dbUc131Uriagedaka.h"		// 
#include "dbUc141Yakuin.h"			// 
#include "dbUc142Jinkenhi.h"		// 
#include "dbUc151Tidaiyatin.h"		// 
#include "dbUc152Kenrikin.h"		// 
#include "dbUc153Kougyou.h"			// 
#include "dbUc161Zatueki.h"			// 
#include "dbUc162Zatuson.h"			// 
#include "dbUc171Sonota1.h"			// 
#include "dbUc181Sonota2.h"			// 

// Cmn
#include "UcFunctionCommon.h"			//

using namespace std;

class CprtJobFunction
{
public:
	CprtJobFunction(void);
	virtual ~CprtJobFunction(void);

// midori 151730,151263 class del -->
	//int SelectJobOne(CDBZmSub* pZmSub, CString strPath, int nFormSeq, int nNameKeiBitArray);
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
	int SelectJobOne(CDBNpSub* pZmSub, CString strPath, int nFormSeq, int nNameKeiBitArray);
// midori 151730,151263 class add <--
// midori 180101 del -->
//// midori 160608 cor -->
////	int SelectJobAll(CDBZmSub* pZmSub, CString strPath, int nOutGenzai, PRT_INF_LITE* pPrtInfLite, int nNameKeiBitArray, int call_by);
//// ---------------------
//	int SelectJobAll(CDBZmSub* pZmSub, CString strPath, int nOutGenzai, PRT_INF_LITE* pPrtInfLite,int nOutZero, int nNameKeiBitArray, int call_by);
//// midori 160608 cor <--
// midori 180101 del <--
// midori 180101 add -->
// midori 151730,151263 class del -->
	//int SelectJobAll(CDBZmSub* pZmSub, CString strPath, PRT_INF_LITE* pPrtInfLite,int nOutZero, int nNameKeiBitArray, int call_by);
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
	int SelectJobAll(CDBNpSub* pZmSub, CString strPath, PRT_INF_LITE* pPrtInfLite,int nOutZero, int nNameKeiBitArray, int call_by);
// midori 151730,151263 class add <--
// midori 180101 add <--
	int GetOutZero(const CdbUcInfSub& mfcSub);
	int GetHideZero(const CdbUcInfSub& mfcSub);
// midori 190505 del -->
	//int GetDbDataPageAll(int nFormSeq, CDatabase* pDb, int OutZero, int nNameKeiBitArray = -1, int nFormType = 0);
// midori 190505 del <--
// midori 190505 add -->
	int GetDbDataPageAll(int nFormSeq, CDatabase* pDb, int OutZero, int nNameKeiBitArray = -1, int nFormType = 0, int nPrnHoho = -1);
// midori 190505 add <--
	int GetDbDataTableUcInfFormType(int index, CDatabase* pDB = NULL);
	int	GetDbDataPageAllSub(int index, int nFormSeq, CDatabase* pDB, int nNameKeiBitArray, int FormNo2);

	CprtJobFunction* virGetPrintObject(int nSchList, int nFormSeq);
	virtual int GetDbDataSheet(int nPage, CprtJobFunction* parent,const PRT_OPTION& po, int nFormSeq = 0);
	BOOL JobPreparePrinting(CPrintInfo* pInfo, CView* pView);
	void JobBeginPrinting(CDC* pDC, CPrintInfo* pInfo = NULL, CView* pView = NULL);
	void JobEndPrinting(CDC* pDC = NULL, CPrintInfo* pInfo = NULL, CView* pView = NULL);
	void JobPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL, CView* pView = NULL);
	void JobPrint(CDC* pDC, CPrintInfo* pInfo, CView* pView);
	void JobFilePrintPreview(CView* pView);
// midori 181101 del -->
//	void JobFilePrintToCsv(UCHIWAKE_INFO uInfo);
// midori 181101 del <--
	void DeletePageMapAll();
	void SetIsPrintFromPrev();

//private:
protected:
	int CheckInit(void);
	void InitProp(void);
	int PrintDataPageOne(int index, int nPage, CDC* pDC);
	int PrintDataCommon(int index, int nPage, CDC* pDC);
	int CheckFileSch(int nErrFlag);
	int MakeListSch(int index);
	void ShowErrMsg(CString strMsg, BOOL bFgProgEnd = FALSE);
//	void SetDataBuff(int nColumn, int nRow);
	void SetDataBuff(CprtJobFunction* parent, int index);
	void SetDataBuffCover(int nColumn, int nRow);
	int GetDataInitVal(void);
	int GetDataOmit(int index);
	int GetDataPageOne(int index, int nPage);
	int GetDataCover(void);
	int GetDbDataKnNameFirstRow(int nFormSeq, int nPage, CString* pstrKnName, int OutZero);
	int GetDbDataTitleAll(void);
// midori 180101 del -->
//	int GetDbDataTableZVolume(void);
// midori 180101 del <--
	int	GetDbDataTableCpdata(void);
	int	GetDbDataTablePrfkin(void);
	int GetDbDataTableUcInfMain(void);
// midori 190505 del -->
//	int GetDbDataTableUcInfSub(int index);
// midori 190505 del <--
// midori 190505 add -->
	int GetDbDataTableUcInfSub(int index, int pflg);
// midori 190505 add <--
	int GetDbDataTableUcInfSubOmit(int nFormSeq, PRT_INF_OMIT* pPrtInfOmit);
// インボイス登録番号追加対応_23/10/23 del -->
//	int GetDbDataUc011(int nPage);
//	int GetDbDataUc021(int nPage);
//	int GetDbDataUc031(int nPage);
//	int GetDbDataUc041(int nPage, int nFormType);
//	int GetDbDataUc042(int nPage, int nFormType);
//	int GetDbDataUc051(int nPage);
//	int GetDbDataUc052(void);
//	int GetDbDataUc061(int nPage);
//	int GetDbDataUc071(int nPage);
//	int GetDbDataUc081(int nPage);
//	int GetDbDataUc091(int nPage);
//	int GetDbDataUc092(void);
//	int GetDbDataUc101(int nPage, int nFormType);
//	int GetDbDataUc102(int nPage, int nFormType);
//	int GetDbDataUc111(int nPage);
//	int GetDbDataUc121(int nPage);
//	int GetDbDataUc131(int nPage);
//	int GetDbDataUc141(int nPage, int nFormType);
//	int GetDbDataUc142(int nPage, int nFormType);
//	int GetDbDataUc151(int nPage, int nFormType);
//	int GetDbDataUc152(int nPage, int nFormType);
//	int GetDbDataUc153(int nPage, int nFormType);
//	int GetDbDataUc161(int nPage, int nFormType);
//	int GetDbDataUc162(int nPage, int nFormType);
//	int GetDbDataUc171(int nPage, int nFormSeq);
//	int GetDbDataUc181(int nPage, int nFormSeq);
// インボイス登録番号追加対応_23/10/23 del <--
	int GetDbDataUc182(int nFormSeq);
	BOOL CheckDbDataFgFuncNull(CdbUc000Common* prc);
	BOOL CheckDbDataFgFuncKei(CdbUc000Common* prc);
	BOOL CheckDbDataFgFuncData(CdbUc000Common* prc);
	BOOL CheckOmit(CString strTarget, CString strOld, int index, PRT_INF_OMIT* pPrtInfOmit);
	CString ShowFieldOfVal(CString strTarget, CdbUc000Common* prc, CString strFormat = _T( "" ), int nBit = 0);
	CString ConvertNum(CString strTarget, CString strFormat);
// No.200903 del -->
	//CString MakeFormatOfNum(int nIntDigit, int nDecDigit = 0, BOOL bFgComma = TRUE);
// No.200903 del <--
// No.200903 add -->
	CString MakeFormatOfNum(int nIntDigit, int nDecDigit = 0, BOOL bFgComma = TRUE, BOOL bComma13 = TRUE);
// No.200903 add <--
	void StrDivision(CString strTarget, CStringArray* pstrArray, int nSize);
	void StrArrayEmpty(CString* pstrTarget, int nSize);
	CString GetTitleOfKnName(int index, int nPage);
	CString GetTitleFromList(int nFormSeq);
	BOOL CheckUseTitleOfKnName(int nFormSeq);
	void SetTitleToList(int nFormSeq, CString strTitle);
	CString InsertSlashInHead(CString str1, CString str2);
	CString ConvertNullIntoSpace(CString strTarget, int nSize);
// midori 200101 del -->
//	CString ConvertDate(int nDate, CString strFormat, int KesKikanSw = 0);
// midori 200101 del <--
// midori 200101 add -->
	CString ConvertDate(int nDate, CString strFormat, int swSub=0);
// midori 200101 add <--
// midori 181101 del -->
//	CString ConvertDateCover(int nDate, int nType);
// midori 181101 del <--
	int GetPageAll(int index, int nNameKeiBitArray);
	void GetPrtPageInfo(int nCurPage);
	void SetNprtParam();
	CString GetStringByteSize(CString strData, int intByteSize);
// midori 181101 del -->
//	void ConvertEEymdToEtrm(BYTE* pBcdBuf, long lngBcdSize, long lngEEymd);
// midori 181101 del <--
	void GetCsvHcelVcel(int nFormSeq, int nFormType, WORD* wHcel, WORD* wVcel);
	CString GetIkkatuStringValue(CString strIkkatu, CdbUc000Common* rc);
	void KingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag=1);
	int GetPrintInfo(CprtJobFunction* parent);
	CString HideZeroMoney(CString strVal);
	CString FixUnderDotZero(CString strVal);
	CString ChangeMinusToTriangle(CString strVal);
	CString RemoveComma(CString strVal);
	BOOL IsBkOneLine();
	CString FillSpace(CString strBase, int nByte);
	CString GetHjNo(long psSigCoprn, const PRT_INF_COMMON& inf_common);
	CString CprtJobFunction::GetGengo(long nDateWareki);
// midori 190505 add -->
	int	GetPhoho( int nFormSeq, int nPHoho, CBatchPrt* pBp, CDBNpSub* pZmSub );
// midori 190505 add <--

//2010/08/XX 業務統合メニュー対応
public:
	BOOL	SetNprtParTgInf( NPRT_PAR* pPrtPar );
	BOOL	GetTgNameCd( CString& strName );
	void	CVPrintErrorAbortPrc( CVPrint* pPrtMdl, int ApErrCod = 0, LPCTSTR pErrMsg = NULL );	// 2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応

public:
	PCSV_PAR			m_icsPcsv;								// Excel出力用データ
	CVPrint				m_icsVprn;								// プリントモジュール用クラスオブジェクト
	NPRT_PAR			m_icsPset;								// 印刷指定パケット
	BOOL				m_bPrtAll;								// 一括印刷中フラグ（TRUE：一括印刷、FALSE：通常印刷）
	int					m_nFormSeq;								// 様式シーケンス
	int					m_strCoverYear;							// 表紙に出力する年
	int					m_strCoverMonth;						// 表紙に出力する月
	int					m_strCoverDay;							// 表紙に出力する日
	BOOL				m_isF5FromPrev;							// 通常のF5? F4プレビューからのF5？
// 24/04/22_ScoreLink add -->
	int					m_nLastPage;
// 24/04/22_ScoreLink add <--

protected:
// インボイス登録番号追加対応_23/10/23 del -->
	//PBUF				m_icsBufBox[PRT_MAX_COLUMN];			// データ出力バッファ（出力文字列データのポインタを格納）
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	PBUF				m_icsBufBox[PRT_MAX_COLUMN_I];			// データ出力バッファ（出力文字列データのポインタを格納）
// インボイス登録番号追加対応_23/10/23 add <--
							// インデックス0は使用しない
// midori 151730,151263 class del -->
	//CDBZmSub*			m_pZmSub;								// DB財務クラスオブジェクト
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
	CDBNpSub*			m_pZmSub;								// DB財務クラスオブジェクト
// midori 151730,151263 class add <--
	CDatabase*			m_pDB;									// DBハンドル	
	CString				m_strPath;								// 実行ファイルのパス
	PRT_ENUM_SELECT_JOB	m_eJob;									// 処理の種類（1：印刷･プレビュー／2：一括印刷）
	CUcFunctionCommon	m_clsFunc;								// 共通関数クラス
	CprtJobFunction*	m_prtJob;								// 呼び出し元の出力情報を格納
	int					m_index;								// 現在印刷中のインデックス
	int					m_nPageSum;								// 全帳表の印刷対象ページ数（通し番号）
	int					m_nPageAllSum;							// 全帳表の総ページ数（通し番号）
	int					m_nCurPage;								// 現在印刷中のページ番号（帳表ごとの番号）
// midori 180101 del -->
//	int					m_nOutGenzai;							// 「現在」の出力（表紙）
// midori 180101 del <--
// midori 190505 del -->
//// midori 160608 add -->
//	int					m_nOutZero;								// 「帳票イメージのみ出力する」
//// midori 160608 add <--
// midori 190505 del <--
	long				m_lFncFlag;								// ファンクション フラグ（印刷指定パケット）
	int					m_nRowMax;								// 出力様式の最大行数
	int					m_nColumn;								// 出力様式のカラム数
	int					m_CallBy;								// -1:メニューの一括印刷　0:F5キー　左記以外:F4キー（様式シーケンス）

	int					m_nSchList[PRT_ID_FORMNO_MAX];			// スケジュールリスト（出力する帳表番号を格納）
							// インデックス0には共通情報を格納
							// インデックスは、印刷順を意味する

	PRT_INF_MAIN		m_PrtInfMainList[PRT_ID_FORMNO_MAX];	// 出力リスト（印刷時の情報）
							// インデックス0には共通情報を格納
							// インデックスは、印刷順を意味する

	PRT_INF_TITLE		m_PrtInfTitleList[UC_ID_FORMNO_MAX+1];	// タイトルリスト

// インボイス登録番号追加対応_23/10/23 del -->
	//PRT_DATA_LIST		m_PrtDataBox[PRT_MAX_COLUMN];			// 印字データボックス（出力文字列データを格納）
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	PRT_DATA_LIST		m_PrtDataBox[PRT_MAX_COLUMN_I];			// 印字データボックス（出力文字列データを格納）
// インボイス登録番号追加対応_23/10/23 add <--
							// インデックス0は使用しない
	bool				m_bSlash;								// 支店名の先頭に「／」を挿入する
							// 当初、「／」は必ず挿入する、となっていたが、挿入しない仕様となった。
							// 今後、更なる仕様変更（挿入する／しないを選択できるようにする など）が発生した場合に、
							// 柔軟に対応するため、挿入する／しないをフラグで管理する。
							// （このため、現状不要なコードが存在していることに注意）
	CString				m_strCoNameCpdata;

public:
	PRT_INF_COMMON		m_PrtInfCommon;							// 内訳書の共通の情報

	PRT_INF_OMIT		m_PrtInfOmitList1[PRT_MAX_OMIT];		// 帳表○-1の省略文字リスト
							// インデックスには、特に意味を持たせていない

	PRT_INF_OMIT		m_PrtInfOmitList2[PRT_MAX_OMIT];		// 帳表○-2の省略文字リスト
							// インデックスには、特に意味を持たせていない

	PRT_INF_OMIT		m_PrtInfOmitList3[PRT_MAX_OMIT];		// 帳表○-3の省略文字リスト
							// インデックスには、特に意味を持たせていない
// midori 190505 add -->
	int					m_nOutZero;								// 「帳票イメージのみ出力する」
// midori 190505 add <--
};

#endif	// __UCHIWAKE__PRTJOBFUNCTION_H__
