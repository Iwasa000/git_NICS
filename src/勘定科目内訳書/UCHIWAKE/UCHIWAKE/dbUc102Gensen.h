// dbUc102Gensen.h : ヘッダー ファイル
//

#ifndef	_DB_UC_102_GENSEN_H_
#define	_DB_UC_102_GENSEN_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc102Gensen レコードセット

class CdbUc102Gensen : public CdbUc000Common
{
public:
	virtual int CreateNewRecord();
// 157153 del -->
//// midori 152765 add -->
//	virtual int CreateNewRecord2(int pNumPage,int pNumRow);
//// midori 152765 add <--
// 157153 del <--
// 157153 add -->
	virtual int CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw);
// 157153 add <--
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--

	CdbUc102Gensen(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc102Gensen)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc102Gensen, CRecordset)
	long	m_SpDate;
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	CString	m_Val;
// midori 152137 add -->
	CString m_KnKana;
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
	//{{AFX_VIRTUAL(CdbUc102Gensen)
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

#endif	//_DB_UC_102_GENSEN_H_
