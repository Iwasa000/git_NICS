// dbUc011Yotyokin.h : ヘッダー ファイル
//

#ifndef	_DB_UC_011_YOTYOKIN_H_
#define	_DB_UC_011_YOTYOKIN_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc011Yotyokin レコードセット

class CdbUc011Yotyokin : public CdbUc000Common
{
public:
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual long GetColumnSortAddressBank(BYTE ItemSeq);
	virtual int UpdateDataTableWork();
	virtual CString GetSortItemType();
// 157153 del -->
//// midori 156968 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 156968 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--

	CdbUc011Yotyokin(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc011Yotyokin)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc011Yotyokin, CRecordset)
	CString	m_AcNum;
	CString	m_BkName1;
	CString	m_BkName2;
	long	m_BkOrder;
	long	m_BkSeq;
	CString	m_KnName;
	long	m_KnOrder;
	long	m_KnSeq;
	CString	m_Teki;
	CString	m_Val;
// midori 152137 add -->
	CString	m_BkKana1;
	CString	m_BkKana2;
	CString	m_KnKana;
// midori 152137 add <--
	//}}AFX_FIELD

/*
	long	m_Seq;
	int		m_NumPage;
	BYTE	m_NumRow;
	BYTE	m_FgFunc;
	BYTE	m_FgShow;
	CString	m_KeiStr;
	long	m_NumGroup;
*/

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbUc011Yotyokin)
	public:
	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#endif	//_DB_UC_011_YOTYOKIN_H_
