//**************************************************************************************************
//
// ClassName	: CdbKamokuSitei
// FileName		:  dbKamokuSitei.h
//
//**************************************************************************************************

#ifndef	__UCHIWAKE__DBKAMOKUSITEI_H__
#define	__UCHIWAKE__DBKAMOKUSITEI_H__

#pragma once

#include "dbUcInfSub.h"
// midori 152137 add -->
#include "dbUcInfMain.h"
// midori 152137 add <--
#include "dbUcLstItemSort.h"
#include "dbUcLstKamoku.h"
#include "dbPrtWork.h"
#include "dbUcInfFormType.h"
#include "CtrlSubGeneralVar.h"

class CdbKamokuSitei
{
public:
	CdbKamokuSitei(CDatabase* pDatabase, int pFormSeq);
	virtual ~CdbKamokuSitei(void);

public:
	CString				m_ErrorMsg;
	int					m_ErrorCode;
//	CUcFunctionCommon	m_clsFunc;			// 共通関数クラス

protected:
	int					ExecuteSQLWork( CString inStrSql );
	BOOL				CheckTableOpen( CdbBase* mfcRec );

public:
// midori 190505 del -->
	//int					Initialize( void );
// midori 190505 del <--
// midori 190505 add -->
	int					Initialize( int pSw );
// midori 190505 add <--
// midori M-16113008 del -->
	//int					SiteigaiDelete( int pSort );
// midori M-16113008 del <--
// midori M-16113008 add -->
	int					SiteigaiDelete( void );
// midori M-16113008 add <--
// midori 190505 del -->
	//int					AddIkkatuData( CString strMtbl );
// midori 190505 del <--

	// ①科目でソートを行うＯＮ
	BOOL				Ks_CmnCheckTableSort();
	int					Ks_CmnTblSortMainProc();
	int					Ks_CmnTblSortSubSortBeforeProc();
	int					Ks_TblSortSubSortProc( CdbPrtWork* rsData );
	void				Ks_CmnTblSortSubGetSortParam(CString* str1, CString* str2);
// 158063 add -->
	int					IsTorihikiAmount( int pFormSeq );
// 158063 add <--
	int					Ks_CmnTblSortSubSortAfterProc(CdbPrtWork* rsData);
// midori 156968 del -->
//// midori 190505 add -->
//	int					Ks_CmnTblSortSubSortAfterKaipage(CdbPrtWork* rsData);
//// midori 190505 add <--
// midori 156968 del <--
	BOOL				Ks_CmnUcLstItemSortGetData(int intItemSeq, ITEMSORT_INFO* uItemSort);
	CString				Ks_CmnGetSykeiNameWithKakko(CString strSykeiName, CString strLeft, CString strRight, CString strKamoku=_T(""));
	CString				Ks_GetIkkatuVal(long pKnSeq);
// 158096 del -->
	//int					Ks_CmnKingakuCmpFunc(CString strKingaku1, CString strKingaku2);
// 158096 del <--
// 158096 add -->
	int					Ks_CmnKingakuCmpFunc( CString strKingaku1, CString strKingaku2, CString strKingaku3 );
// 158096 add <--
	int					Ks_CmnGetNextPageRow(int* pPage, int* pRow);
	int					Ks_CmnTblIkkatuAutoUpdateProc(int nType, int nGroup, int* nPage, int* nRow, SORTKAMOKU_INFO uKamoku);
	void				Ks_TblGetIkkatuSumValue(CdbPrtWork* rsData, int nFgShow, int nGroup, CALCKEI_INFO* uCalcKei);
	void				Ks_TblEditIkkatuLine( int nType, CdbPrtWork* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku );
	int					Ks_CmnTblInsertSpecialLine(int nPage, int nRow, int nType, CString strName);
// midori 190505 add -->
	int					Ks_CmnTblInsertSpecialLineKm(int nPage, int nRow, int nType, int nKnSeq, int nKnOrder, CString strKnName, CString strKnKana);
// midori 190505 add <--
	BOOL				Ks_CmnGetNextFirstRow(int intRow);
	int					Ks_CmnTblInsertNullLine(int nPage, int nRow, int nLine);
	int					Ks_CmnTblUpdatePageRowAllResetMainAfterSort();
	int					Ks_CmnTblUpdatePageNumRowNumReset(int inPage, int inRow);
	int					Ks_CmnTblCountMaxRow();
	int					Ks_CmnTblUpdateEndPageSetting(void);
	int					Ks_CmnTblInsertPageKeiRuiKei(void);
	int					Ks_CmnTblCalcKei();
	void				Ks_CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag=1);
	BOOL				Ks_GetShowKeiZero(CString strTemp);
	int					Ks_CmnTblCalcKeiAfterSort();

// midori 152137 del -->
//	int					Kn_CmnIkkatuSaisyukei( CString strMtbl );
//	int					Kn_CmnKeiDelete();
// midori 152137 del <--
// midori 152137 add -->
	int					GetSortItem(int intFormSeq,int intItemSeq);
// midori 152137 add <--
// midori 190505 add -->
	int					Ks_ZeroNullCheck( CdbPrtWork* rsData, int pFormSeq, int pSw );
// midori 190505 add <--

protected:
	CDatabase*			m_pDb;
	int					m_nFormSeq;
	CString				m_sTblName;
	BOOL				m_NullSyokeiRow;	// ソート後、1行目に小計空白(FgFunc = ID_FGFUNC_SYOKEINULL)が発生したか（TRUE:発生した）

public:
	// 科目指定の情報
	int					m_KmPrMiny;			// 科目未入力を出力する
// midori M-16113008 del -->
	//int					m_KmPrSort;			// 科目でソートを行う
// midori M-16113008 del <--
	int					m_KmPrKaip;			// 改頁を行う
// midori 190505 add -->
	int					m_KmPrKtitle;		// 科目行を自動挿入する
// midori 190505 add <--
	int					m_KmPrSkei;			// 小計を自動挿入する
	int					m_KmPrIkat;			// 一括金額を行う
// midori 190505 add -->
	int					m_KmPrIkatZero;		// 一括金額に０円を含む(０円[空欄]削除＋改頁で、印刷前にソート及び改頁を行う場合のみ参照)
// midori 190505 add <--

	// 小計の情報
	int					m_OpOutSyokei;		// 一行でも小計を挿入する
	CString				m_OpSykeiName;		// 小計の名称
	CString				m_OpLeft;			// 小計用括弧(左)
	CString				m_OpRight;			// 小計用括弧(右)
	int					m_OpOutNullLine;	// 小計行の次に空行を挿入する

	// 一括の情報
	CString				m_OpIkkatuVal;		// 一括金額
	BOOL				m_isAdvSet;			// 詳細設定を使用する
	int					m_IkkatuOutZero;	// 一括金額に０円を含まない

	// ソート項目情報
	int					m_Sort1;			// 項目番号

	// 帳表の通常/個別の管理情報
	int					m_FormType04;		// 仮払/貸付
	int					m_FormType10;		// 仮受/源泉
	int					m_FormType15;		// 地代/権利/工業
	int					m_FormType16;		// 雑益/雑損

	// 様式ごとの情報
	int					intRowMaxData;		// １頁内のデータ行数（合計行は含まず）
	int					intRowKei;			// 帳表固定の合計行数（1:合計あり，0:なし）
	int					intRowMax;			// １頁内の合計行を含む全体の行数
	int					intOutKei;			// 計設定内容（uc_inf_sub）
	int					m_NameKei;			// 計の名称
	int					intMaxCount;					// 集計フィールド件数
	CString				strField[CALCKEI_MAXCOUNT];		// 集計フィールド名称
// midori 190505 add -->
	int					m_OutZero;			// D0=0円を出力しない　D1=空欄を出力しない
// midori 190505 add <--

	// ソートの結果
	int					intMaxPage;			// 最大頁数

// midori 152137 add -->
	// F9 参照の情報
	int					m_KamokuSort;		// 参照ダイアログ::表示順（科目）		0:番号順	1:50音順
// midori 152137 add <--

// midori 190505 add -->
	int					m_pSw;				// 0:画面への科目行挿入不可	1:画面への科目行挿入可能
// midori 190505 add <--
};

#endif	// __UCHIWAKE__DBKAMOKUSITEI_H__
