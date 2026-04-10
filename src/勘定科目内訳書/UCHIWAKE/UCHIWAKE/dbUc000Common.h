// dbUc000Common.h : ヘッダー ファイル
//

#ifndef	_DB_UC_000_COMMON_H_
#define	_DB_UC_000_COMMON_H_

#include "dbBase.h"
#include "dbUcInfSub.h"
#include "dbUcInfFormType.h"	// uc_inf_form_typeテーブルクラス
#include "dbUcLstItemSort.h"	// uc_lst_item_sortテーブルクラス
#include "dbUcLstHokan.h"		// uc_lst_hokanテーブルクラス
#include "CtrlSubGeneralVar.h"
// midori 160610 add -->
#include "dbkamokuSitei.h"
// midori 160610 add <--
// midori 161111 add -->
#include "dbPrtWork2.h"
// midori 161111 add <--
// midori UC_0003 add -->
#include "dbPrtWorkBk.h"
#include "dbPrtWorkAd.h"
// midori UC_0003 add <--

class CMyStringArray : public CStringArray
{
public:
	void operator=(const CMyStringArray& right)
	{
		Copy(right);
		return;
	}
};

class CMyArray : public CArray<CMyStringArray, CMyStringArray&>
{
//public:
//	void operator=(const CMyArray& right)
//	{
//		Copy(right);
//		return;
//	}
};

/////////////////////////////////////////////////////////////////////////////
// CdbUc000Common レコードセット

class CdbUc000Common : public CdbBase
{
protected:
	int GetKnNameFirstRowSub(int inPage, CString* pstrKnName, CString strTable);
	int GetFormType(int nFormSeq);
// midori M-16083101 del -->
	//int	GetFormSeqBit(int nFormSeq);
// midori M-16083101 del -->
// midori 160610 cor -->
	//int CreateKaiPageTable(int nFormSeq, int nMaxRow, CString strColumn, CString strResultTable);
// ---------------------
	int CreateKaiPageTable( int nFormSeq, int nMaxRow, CString strColumn, CString strResultTable, int pSw, int pSw2 );
// midori 160610 cor <--
// midori UC_0003 add -->
	int CreateKaiPageTableBk(int nFormSeq, int nMaxRow, CString strColumn, CString strResultTable, int siten);
	int CreateKaiPageTableAd( int nFormSeq, int nMaxRow, CString strColumn, CString strResultTable );
// midori UC_0003 add <--
// midori 156947 add -->
	void GetKamokuKeiStr(CString szSrc,CString sKamoku, CString* szDst);
// midori 157099,157119_2 del -->
//// midori157118 add -->
//	void ChgSyoKeiStr(int nFormSeq);
//// midori157118 add <--
//	void EditSyokeiStr(int nFormSeq);
// midori 157099,157119_2 del <--
// midori 156947 add <--
// midori157118 add -->
	void ClearSyoKeiStr(int nFormSeq);
	CString GetSpritString(CString szSrc, CString* szDst, int nPos);
	void KeiStrSprit(CString szSrc, CString* szDst, CString* szDst2);
// midori157118 add <--
// midori 152137 del -->
//// midori 151400 del -->
//	//void SetTblSortSubGetSortParam(int nFormSeq, CString* strItemFirst, CString* strItemAll, CString* strSortType);
//// midori 151400 del <--
//// midori 151400 add -->
//	void SetTblSortSubGetSortParam(int nFormSeq, CString* strItemFirst, CString* strItemAll, CString* strSortType, BYTE* fgItemSort1);
//// midori 151400 add <--
// midori 152137 del <--
// midori 152137 add -->
	void SetTblSortSubGetSortParam(int nFormSeq, CString* strItemFirst, CString* strItemAll, CString* strSortType, BYTE* fgItemSort1,int* nSort);
// midori 152137 add <--
// midori UC_0003 add -->
	void SetTblSortSubGetSortParamX(int nFormSeq, CString* strItemAll);
// midori UC_0003 add <--
	BOOL GetUcLstItemSortGetData(int nFormSeq, int intItemSeq, ITEMSORT_INFO* uItemSort);
	CString GetSQLUpdateFgFuncFromIkkatu(CString TableName, int FgFuncBefor, int FgFuncAfter);
// midori 190505 del -->
	//int InsertKamokuRow(CString BaseTbl);
// midori 190505 del <--
// midori 156188_2 add -->
	int InsertKamokuRow(CString BaseTbl);
// midori 156188_2 add <--
// midori 160606 cor -->
	//CString GetSQLUpdateYokukiZeroClear(CString column);
// ---------------------
	CString GetSQLUpdateYokukiZeroClear(CString column, int pSw);
// midori 160606 cor <--

public:
	virtual int UpdateDataTableWork();
	int UpdateDataTableNullToStrNull(CString inFieldName);
// midori 156189,156190,156191 del -->
	//int	UpdateCalcKei(int intFormSeq, CALCKEI_INFO uCalcInfo);
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
	int	UpdateCalcKei(int intFormSeq, CALCKEI_INFO uCalcInfo,int pSw = 0);
// midori 156189,156190,156191 add <--
	int	UpdatePageNumRowNumAdd(int inPage, int inRow, int inAdd, int inAddData);
// midori 154714 add -->
	void UpdateNewPageCheckAndDelete( void );
// midori 154714 add <--
	int UpdateFgShowClearPageRow(int nFgShow);
	int UpdateFgShowClearGroup(int nFgShow);
	int UpdateFgShow(int nOldFgShow, int nNewFgShow);
	int UpdateFgShowPageRow(int nOldFgShow, int nNewFgShow, int nPage, int nRow);
	int UpdateFgShowRetIkkatu(int nGroup);
	int UpdateRenFgTempClear();

	int	DeletePage(int inPage);
	int DeleteAllRecord();
	int DeleteVisualRecord();
	int	DeleteFgFunc(int inFgFuncKind);
	int DeleteFgShow(int inFgShowKind);
	int RequeryNextNotNullLine( int nPage, int nRow );
	int ExecuteDeleteNullLine( int nStartPage, int nStartRow,  int nEndPage, int nEndRow  );
	int DeleteNullLinePageRowAfterAll(int inPage, int inRow);

	int RequerySeq(int inSeq);
	int	RequeryFgFunc(int nFgFunc, int nDescFlag);
	int RequeryFgFuncGroup(int nFgFunc, int nNumGroup);
	int RequeryNextBeforeRecord(int nPage, int nRow, int nTarget, int nData);
	int RequerySortPageRow();
	int	RequeryFgShowSortPageRow(int inFgShow);
	int RequeryFgShowIkkatu();
	int RequeryFgShowIkkatu( int nNumGroup );
// No.158072 add -->
	int RequeryFgShowIkkatuSort(int nNumGroup, CString sSort);
	CString RequeryFgShowIkkatuSort021(int nNumGroup, CString sSort, char pSsw);
	CString RequeryFgShowIkkatuSort061(int nNumGroup, CString sSort, char pSsw);
	CString RequeryFgShowIkkatuSort081(int nNumGroup, CString sSort, char pSsw);
// No.158072 add <--
	int RequeryFgShowZero();
	int RequeryFgShow(int inFgShow, int nData=0);
	int RequeryPage(int inPage);
// midori 155525 del -->
//// midori 161107 add -->
//	virtual int RequeryPageRangai( int pType );
//// midori 161107 add <--
// midori 155525 del <--
// midori 155525 add -->
	virtual int RequeryPageRangai( int pType, int pOutZero );
// midori 155525 add <--
	int RequeryPageRow(int inPage, int inRow);
	int RequeryPageRowAfterRecord(int inPage, int inRow);
	int RequerySortParameter(CString strFilter, CString strSort);
	int RequeryRenKcdRenEcd(CString strRenKcd, long lngRenEcd);
	int RequeryRenFgTempUpdateNone();
	int RequeryLastNotNullLine();

	virtual int GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable);
	CString	GetFieldValueString(CString strValue);
	int	GetNumRowFgShow(int inPage, int FgShow);
	int	GetNumRowFgFuncFgShow(int inPage, int inFgFunc, int inFgShow);
	int GetNumRow(int inPage);
	int GetNumPage();
// midori 190505 del -->
//// midori 160610 cor -->
//	//int GetNumPageFromTempTable(int nFormSeq, int nNameKeiBitArray, int OutZero);
//// ---------------------
//	int GetNumPageFromTempTable(int nFormSeq, int nNameKeiBitArray, int OutZero, int KmkOut, int KmkSitei);
//// midori 160610 cor <--
// midori 190505 del <--
// midori 190505 add -->
	int GetNumPageFromTempTable(int nFormSeq, int nNameKeiBitArray, int OutZero, int KmkOut, int KmkSitei, int nPHoho );
// midori 190505 add <--
	int GetNumGroup();
	int GetCountVal(int inFgShow, int inNumGroup, int bOutZero = 0);
// midori 160607,160610 cor -->
	//int	GetCountDataRecord();
// ----------------------------
	int	GetCountDataRecord( int pFgShow=0, int pKnSeq=0 );
// midori 160607,160610 cor <--
// midori 190505 add -->
	int	GetCountDataRecord2(int pFormSeq, int pOutZero, int pOutNull);		//	データ行数の取得（金額が入力されている行限定）
// midori 190505 add <--
	int GetCountVisualDataRecord(int nMode=0);
	virtual int GetCountFgShow(int FgShow);
	int GetSumVal(int nFgShow, int nNumGroup, CString* pSumVal);
	int GetSumValSub(int nFgShow, int nNumGroup, CString inFieldName, CString* pSumVal);
	int GetOutKei(int nFormSeq);
	int GetColumnName(CStringArray& strColumn, int index);
	int GetRowKei(int nFormSeq);
	int GetMaxRowFromTempTbl(int nFormSeq);
// midori 20/10/08u del -->
//// midori 190505 add -->
//	int	CreateDspPageBreakTable( int nDataRowMax, int nFormSeq, int nNameKeiBitArray );
//// midori 190505 add <--
// midori 20/10/08u del <--
// midori 160610 cor -->
	//int CreateNotExistZeroTable(int nRow, int nFormSeq, int nNameKeiBitArray);
	//int CreateKamokuAddTable(int nRow, int nFormSeq, int nNameKeiBitArray);
// ---------------------
// midori 190505 add -->
	int	CreateNotExistZeroTable(int nRow, int nFormSeq, int nNameKeiBitArray, int pSw, int pSw2, int nPHoho);
// midori 190505 add <--
// midori 156188_2 add -->
	int	CreateKamokuAddTable(int nRow, int nFormSeq, int nNameKeiBitArray, int pSw, int pSw2);
// midori 156188_2 add <--
// midori 157099,157119_2 del -->
//// midori 157098 add -->
//	int	CreateKamokuDelTable(int nDataRowMax, int nFormSeq, int nNameKeiBitArray);
//// midori 157098 add <--
// midori 157099,157119_2 del <--
// midori M-16113008 del -->
	//int	CreateNotExistKamokuTable(CdbKamokuSitei* pks, int nDataRowMax, int nFormSeq, int nNameKeiBitArray);
// midori M-16113008 del <--
	int	TempTableCopy(int nFormSeq);
	int	CreateKamokuSiteiTable(int nFormSeq);
	int	UpdateKamokuSiteiTable(int nDataRowMax, int nFormSeq, int nNameKeiBitArray, int pSw);
	int	DeleteKeiNullKamokuRow(CString strTable);
// midori 160610 cor <--
	CString GetSQLHokan(int nFormSeq);
// No.158072 add -->
	CString GetSQLHokanSort(int nFormSeq, CString sSort);
	CString GetSQLHokanSort021(int nFormSeq, CString sSort, char pSsw);
	CString GetSQLHokanSort061(int nFormSeq, CString sSort, char pSsw);
	CString GetSQLHokanSort081(int nFormSeq, CString sSort, char pSsw);
// No.158072 add <--
	int AddHokanTable(int nFormSeq, CString strCond);
	int GetMaxPageFromTempTable(CString strTable);
	int GetMaxRowFromTempTable(CString strTable, int nPage);
// midori 157098 del -->
//// midori 190505 del -->
////// midori 160610 cor -->
////	//int DeleteKeiNullZeroRow(CString strTable);
////// ---------------------
////	int DeleteKeiNullZeroRow( CString strTable, CString strTable2, int pSw2 );
////// midori 160610 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//	int DeleteKeiNullZeroRow( CString strTable, CString strTable2, int pSw2, int nFormSeq );
//// midori 190505 add <--
// midori 157098 del <--
// midori 157098 add -->
	int DeleteKeiNullZeroRow( CString strTable, CString strTable2,int pSw, int pSw2, int nFormSeq );
// midori 157098 add <--

// midori 152138 del -->
//// midori 161111 add -->
//	int InsertKeiNullLine( int pFormSeq, int pLclDataRowMax );
//// midori 161111 add <--
// midori 152138 del <--
// midori 152138 add -->
	int InsertKeiNullLine( int pFormSeq, int pLclDataRowMax, int* pRet );
	int DeleteFirstNullLine( int pFormSeq, int pLclDataRowMax, int* pRet );
// midori 156952,156964 add -->
	int DeleteFirstNullLine_Dp( int pFormSeq, int pLclDataRowMax, int* pRet );
// midori 156952,156964 add <--
// midori 152138 add <--
	int InsertNullRowExceptLastPage( CString strTempUtiwakeTbl, int nMaxRow, CMyStringArray& strColumnArray );
	int CreateSequence(CString strTable, CString BaseTable);
	int InsertKamokuAddedTableToResultTable(CString strTable, CString BaseTable, CString strColumn);
//	int	SetPageRowNumToReturnData(int nPage, int nRow, int nBaseRow);
	CString GetSQLInsertKamokuRow(CString BaseTbl);
	int InsertNullRowToLastPage(CString strTempUtiwakeTbl, int nKeiType, int nKeiRow, int& nMaxPage, int& nLclDataRowMax, int nCurRow, CMyStringArray& strColumnArray);
// midori 157099,157119_2 del -->
//// midori 157098 del -->
//	//int InsertKeiRow(CString strTempUtiwakeTbl, int nFormSeq, int nKaiPage, int nKeiType, int nKeiRow, int nMaxPage, int nDataRowMax, int nNameKeiBitArray, CMyStringArray& strColumnArray);
//// midori 157098 del <--
//// midori 157098 add -->
//	int InsertKeiRow(CString strTempUtiwakeTbl, int nFormSeq, int nKaiPage, int nKeiType, int nKeiRow, int nMaxPage, int nDataRowMax, int nNameKeiBitArray, CMyStringArray& strColumnArray, int pSw = 0);
//// midori 157098 add <--
// midori 157099,157119_2 del <--
// midori 157099,157119_2 add -->
	int InsertKeiRow(CString strTempUtiwakeTbl, int nFormSeq, int nKaiPage, int nKeiType, int nKeiRow, int nMaxPage, int nDataRowMax, int nNameKeiBitArray, CMyStringArray& strColumnArray);
// midori 157099,157119_2 add <--
// midori 190505 del -->
	//BOOL IsInsertKamokuRow(int nFormSeq);
// midori 190505 del <--
// midori 156188_2 add -->
	BOOL IsInsertKamokuRow(int nFormSeq);
// midori 156188_2 add <--
// 14-1821_15-1030 del -->
//// 20-0450 del -->
//	//BOOL CalcSykeiAfterSort(CALCKEI_INFO uCalcInfo, CString strSortItem1, CString strSortItem2);
//// 20-0450 del <--
//// 20-0450 add -->
//	BOOL CalcSykeiAfterSort(CALCKEI_INFO uCalcInfo, CString strSortItem1, CString strSortItem2, int pSw);
//// 20-0450 add <--
// 14-1821_15-1030 del <--
// midori 152137 add -->
	int GetSortItem(int intFormSeq,int intItemSeq);
	int IsKamokuSort(int nFormSeq, CDatabase* pdb);
	void IsSort(int nFormSeq, CDatabase* pdb,int* rbank,int* raddress);
// midori 152137 add <--
// midori 190505 add -->
	void DeleteKamokuRow(int nFormSeq,int pSw,CString pstrTable);
// midori 190505 add <--

	virtual int CreateNewRecord();
// 157153 del -->
//// midori 152765 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 152765 add <--
//// midori 156188 add -->
//	virtual int CreateNewRecord3(int pNumPage,int pNumRow);
//// midori 156188 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--
	virtual int Init(int inPage);
// midori 160606 cor -->
	//virtual int UpdateYokukiZeroClear();
// ---------------------
	virtual int UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual int UpdateYokukiMoveToHokan();
// midori 180537 add -->
	virtual int UpdateYokukiMoveToHokan2();
// midori 180537 add <--
	virtual int UpdateZeroMoneyMoveToHokan( HOKAN_DATA *hData );
	virtual	CString GetZeroMoneyMoveField();
	virtual int ExtendColumn(CString strTableName, CString strColumn, CString design);
	virtual int ExtendColumnEX(CString strTableName, CString strColumn, CString design, int nDefault);
	virtual CString GetSQLSumColumn();
	virtual int GetColumnNum();
	virtual CString GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei=NULL, int nFgFunc=0, CMyArray* arryTotalAll=NULL);
	virtual CString GetSQLZeroJudgment();
	virtual CString GetSortItemType();
	virtual CString GetSQLInsertColumns(CString BaseTbl);
// midori 152138 del -->
//// midori 161111 cor -->
////// midori 160610 cor -->
//////	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn);
////// ---------------------
////	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto);
////// midori 160610 cor <--
//// ---------------------
//	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto, int pFormSeq);
//// midori 161111 cor <--
// midori 152138 del <--
// midori 152138 add -->
	virtual int CalcPageRowNumToTempTable(CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto, int pFormSeq, int pNullAdd);
// midori 152138 add <--
// midori 190505 add -->
	virtual int CalcPageRowNumToTempTable_Dp( CString strTable1, CString strTable2, int MaxRow, CString strColumn, CString strMoto, CMap<int,int,int,int>* );
// midori 190505 add <--
	virtual CString GetSQLResultSelect(CString strTable);
	virtual CString GetSQLValIsNull();
	virtual long GetColumnSortAddressBank(BYTE ItemSeq);
	virtual int GetSortIkkatuOrder();
// midori 152909_2 del -->
//	CString	GetSQLDeleteNullLineUnderSyokei(const CString& strTable);
// midori 152909_2 del <--
// midori 152909_2 add -->
	CString	GetSQLDeleteNullLineUnderSyokei(const CString& strTable, int pSw);
// midori 152909_2 add <--
// midori 155525_2 del -->
	//virtual int CreateRangaiTempTable();
// midori 155525_2 del <--
// midori 155525_2 add -->
	virtual int CreateRangaiTempTable( int pOutZero );
// midori 155525_2 add <--
	virtual int GetRangaiData();
// midori 155525 del -->
			//int	GetPrintPageNum();	// CdbUc052Tanaoroshi2とCdbUc092Kaikakekin2以外からコールしてはいけない
// midori 155525 del <--
// midori 155525 add -->
			int	GetPrintPageNum( int pOutZero=0 );	// CdbUc052Tanaoroshi2とCdbUc092Kaikakekin2以外からコールしてはいけない
// midori 155525 add <--
// midori 156188_2 add -->
	//int KamokuRowEnableSgn(int pSw);				// 改良No.21-0086,21-0529 del
	int KamokuRowEnableSgn(int pSw, int nFormSeq);	// 改良No.21-0086,21-0529 add
// midori 156188_2 add <--
// midori 190505 add -->
	int UpdateFgFunc(int nFgFunc,int nFgFuncW);
// midori 190505 add <--
// midori 157042 add -->
	int GetKmkRowSw(void);
// midori 157042 add <--
// midori157118 add -->
	CString DelCrLfString(CString szBuf);
// midori157118 add <--
// midori 157127 add -->
	BOOL CmnUcLstItemSortGetData(int intFormSeq,int intItemSeq, ITEMSORT_INFO* uItemSort);
// midori 157127 add <--

	CdbUc000Common(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc000Common)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc000Common, CRecordset)
	long	m_Seq;
	int		m_NumPage;
	BYTE	m_NumRow;
	BYTE	m_FgFunc;
	BYTE	m_FgShow;
	CString	m_KeiStr;
	long	m_NumGroup;
	CString	m_RenKcd;
	long	m_RenEcd;
	BYTE	m_RenFgTemp;
	BYTE	m_ShowKeiZero;
	int		m_OutZeroNull;

	//}}AFX_FIELD


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUc000Common)
public:
	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_lo_FormSeq;			// フォームシーケンス番号(0:使用しない)
								// その他1,2帳表で使用するので、
								// その時に正式なFormSeqの値を格納
};

#endif	//_DB_UC_000_COMMON_H_
