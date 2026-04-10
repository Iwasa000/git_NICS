// dbUc092Kaikakekin2.h : ヘッダー ファイル
//

#ifndef	_DB_UC_092_KAIKAKEKIN2_H_
#define	_DB_UC_092_KAIKAKEKIN2_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc092Kaikakekin2 レコードセット

class CdbUc092Kaikakekin2 : public CdbUc000Common
{
public:
	virtual int Init();		//	Init()をオーバーライド
	virtual int Init(int page);		// 欄外登録の複数ページ対応
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
	virtual int GetCountFgShow(int FgShow);
// 155525_2 add -->
	int			GetCountFgShowEx(int FgShow);
// 155525_2 add <--

// midori 155525 del -->
//// midori 161107 add -->
//	virtual int RequeryPageRangai( int pType );
//// midori 161107 add <--
// midori 155525 del <--
// midori 155525 add -->
	virtual int RequeryPageRangai( int pType, int pOutZero );
// midori 155525 add <--

	CdbUc092Kaikakekin2(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc092Kaikakekin2)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc092Kaikakekin2, CRecordset)
	long	m_SpDate;
	CString	m_Val;
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
	//{{AFX_VIRTUAL(CdbUc092Kaikakekin2)
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

#endif	//_DB_UC_092_KAIKAKEKIN2_H_
