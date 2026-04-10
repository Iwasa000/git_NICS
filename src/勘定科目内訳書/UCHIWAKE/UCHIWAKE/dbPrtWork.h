// dbPrtWork.h : ヘッダー ファイル
//

#ifndef	_DB_PRT_WORK_H_
#define	_DB_PRT_WORK_H_

#include "dbBase.h"

class CdbPrtWork : public CdbBase
{
public:
	CdbPrtWork(CDatabase* pDatabase = NULL, int pFormSeq = 0);
	DECLARE_DYNAMIC(CdbPrtWork)

// フィールド/パラメータ データ
	long	m_Seq;			// 01.シーケンス番号
	int		m_NumPage;		// 02.ページ番号
	BYTE	m_NumRow;		// 03.行番号
	BYTE	m_FgFunc;		// 04.特殊行フラグ
	BYTE	m_FgShow;		// 05.表示フラグ
	CString	m_KeiStr;		// 06.「計」文言文字列格納用
	long	m_NumGroup;		// 07.グループ番号
	long	m_KnSeq;		// 08.科目 シーケンス番号
	long	m_KnOrder;		// 09.科目 並び順
	CString	m_KnName;		// 10.科目 名称
	CString	m_Val;			// 11.金額
// 061,121
	CString	m_Val2;			// 12.金額２
// 061
	CString	m_Val3;			// 13.金額３
// 021,031,041,091,101,171
	CString	m_AdName1;		// 12.振出人等
// 051
	CString	m_HdName;		// 13.品目
// 111
	CString	m_AdAdd2;		// 14.借入先（所在地２）
	CString	m_Risoku;		// 15.期中の支払利子額
// 161,162
	CString	m_Naiyou;		// 16.取引の内容
// 181
	CString	m_Item;			// 17.項目
// 171,181
	int		m_FormSeq;		// 18.その他の様式番号
// midori 152137 add -->
	CString	m_KnKana;		// 19.科目 カナ
// midori 152137 add <--
// midori 190505 add -->
	long	m_BkOrder;		// 20.金融機関 並び順
	CString	m_BkName1;		// 21.金融機関名
	CString	m_BkName2;		// 22.支店名
	CString	m_BkKana1;		// 23.金融機関名カナ
	CString	m_BkKana2;		// 24.支店名カナ
	long	m_AdOrder;		// 25.支払(得意)先 並び順
	CString	m_AdName2;		// 26.支払(得意)先名　下段
	CString	m_AdKana;		// 27.支払(得意)先名　カナ
	CString	m_Syurui;		// 28.種類・構造
	CString	m_MsVal;		// 29.取得(処分)価額
	CString	m_MsVal2;		// 30.異動直前の帳簿価額
// midori 190505 add <--
// midori 157118 add -->
	BYTE	m_ShowKeiZero;
// midori 157118 add <--

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
// midori 190505 add -->
	int			UpdateFgFunc(int nFgFunc,int nFgFuncW);
// midori 190505 add <--

public:
	int			m_nFormSeq;
	CString		m_sTblName;
};

#endif	//_DB_PRT_WORK_H_
