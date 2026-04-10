// dbUc142Jinkenhi.h : ヘッダー ファイル
//

#ifndef	_DB_UC_142_JINKENHI_H_
#define	_DB_UC_142_JINKENHI_H_

#include "dbUc000Common.h"

/////////////////////////////////////////////////////////////////////////////
// CdbUc142Jinkenhi レコードセット

class CdbUc142Jinkenhi : public CdbUc000Common
{
public:
	virtual int CreateNewRecord();
// midori 160606 cor -->
	//virtual int	UpdateYokukiZeroClear();
// ---------------------
	virtual int	UpdateYokukiZeroClear( int pSw );
// midori 160606 cor <--
	virtual	int UpdateZeroMoneyMoveToHokan( HOKAN_DATA *hDat );


	CdbUc142Jinkenhi(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CdbUc142Jinkenhi)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbUc142Jinkenhi, CRecordset)
	CString	m_Val;
	CString	m_Val2;
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
	//{{AFX_VIRTUAL(CdbUc142Jinkenhi)
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

#endif	//_DB_UC_142_JINKENHI_H_
