// dbUc052Tanaoroshi2.h : ヘッダー ファイル
//
#ifndef	_DB_UC_052_TANAOROSHI2_H_
#define	_DB_UC_052_TANAOROSHI2_H_

#include "UcCommon.h"
#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc052Tanaoroshi2 レコードセット

class CdbUc052Tanaoroshi2 : public CdbUc000Common
{
public:
	virtual int Init();		//	Init()をオーバーライド
	virtual int Init(int page);		// 棚卸方法登録の複数ページ対応（旧：欄外登録）
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual int UpdateYokukiMoveToHokan();
	virtual int UpdateZeroMoneyMoveToHokan( HOKAN_DATA *hDat );
	virtual int GetRangaiData();
	CString		GetRangaiTblName();
// midori 155525 del -->
//// midori 161107 add -->
//	virtual int RequeryPageRangai( int pType );
//// midori 161107 add <--
// midori 155525 del <--
// midori 155525 add -->
	virtual int RequeryPageRangai( int pType, int pOutZero );
// midori 155525 add <--
	CdbUc052Tanaoroshi2(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc052Tanaoroshi2)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc052Tanaoroshi2, CRecordset)
	BYTE	m_Method;
	CString	m_HdName;
	long	m_SpDate;
	BYTE	m_OutRangai;
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
	//{{AFX_VIRTUAL(CdbUc052Tanaoroshi2)
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

#endif	//_DB_UC_052_TANAOROSHI2_H_
