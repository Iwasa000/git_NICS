// dbPrtWork.h : ヘッダー ファイル
//

#ifndef	_DB_PRT_WORK3_H_
#define	_DB_PRT_WORK3_H_

#include "dbBase.h"

class CdbPrtWork3 : public CdbBase
{
public:
	CdbPrtWork3(CDatabase* pDatabase = NULL, int pFormSeq = 0, CString pstrTable = _T(""));
	DECLARE_DYNAMIC(CdbPrtWork3)

// フィールド/パラメータ データ
	long	m_row;			// 01.行番号
	long	m_Seq;			// 02.シーケンス番号
	int		m_NumPage;		// 03.ページ番号
	BYTE	m_NumRow;		// 04.行番号
	BYTE	m_FgFunc;		// 05.特殊行フラグ
	BYTE	m_FgShow;		// 06.表示フラグ
	CString	m_KeiStr;		// 07.「計」文言文字列格納用
	long	m_NumGroup;		// 08.グループ番号
	long	m_KnSeq;		// 09.科目 シーケンス番号
	long	m_KnOrder;		// 10.科目 並び順
	CString	m_KnName;		// 11.科目 名称
	CString	m_Val;			// 12.金額
// 021,081
	CString	m_AdName1;		// 13.振出人等
// 111
	CString	m_AdAdd2;		// 14.借入先（所在地２）
	CString	m_Risoku;		// 15.期中の支払利子額
	CString	m_KnKana;		// 16.科目 カナ

// オーバーライド
	// ウィザードで生成された仮想関数のオーバーライド
public:
	virtual CString GetDefaultSQL();	// レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	int			UpdateFgShowPageRow(int nOldFgShow, int nNewFgShow, int nPage, int nRow);
	int			UpdateFgShowClearGroup(int nFgShow);
	int			RequerySortParameter(CString strFilter, CString strSort);
	CString		GetFieldValueString(CString strValue);
	int			GetSumValSub(int nFgShow, int nNumGroup, CString inFieldName, CString* pSumVal);
	int			GetCountVal(int inFgShow, int inNumGroup);
	int			CreateNewRecord();
	int			RequeryFgShowSortPageRow(int inFgShow);
	int			GetNumPage();
	int			GetNumRowFgFuncFgShow(int inPage, int inFgFunc, int inFgShow);
	int			GetNumRowFgShow(int inPage, int FgShow);
	int			DeletePage(int inPage);
	int			UpdateCalcKei(int intSeq, CALCKEI_INFO uCalcInfo);
	BOOL		CalcSykeiAfterSort(CALCKEI_INFO uCalcInfo, CString strSortItem1, CString strSortItem2);
	int			GetColumnName(CStringArray& strColumn, int clmIndex);

public:
	int			m_nFormSeq;
	CString		m_sTblName;
};

#endif	//_DB_PRT_WORK3_H_
