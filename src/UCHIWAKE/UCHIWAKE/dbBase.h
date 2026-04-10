// dbBase.h : ヘッダー ファイル
//

#ifndef		_DB_BASE_H_
#define		_DB_BASE_H_

#include	"dbErrCode.h"
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CdbBase レコードセット

class CdbBase : public CRecordset
{
public:
	CdbBase(CDatabase* pDatabase = NULL);
	virtual ~CdbBase();
	DECLARE_DYNAMIC(CdbBase)
	virtual int Fin();
	virtual int Init();
	BOOL	OpenEx();
	BOOL	OpenEx( CRecordset* rs , CString strSqlStr , CString strTblName );
	CString ConvSqlString( CString strOrg );
	void	DBErrExit( CString strError , CString strTblName );


// フィールド/パラメータ データ
	//{{AFX_FIELD(CdbBase, CRecordset)
	//}}AFX_FIELD

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CdbBase)
	public:
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

//	public:
//	virtual CString GetDefaultSQL();    // レコードセットのデフォルト SQL 
//	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//private:
protected:
	virtual int ExecuteSQLWork(CString inStrSql);
	virtual int RequeryWork();
	CString m_lo_TableName;
	CDatabase* m_lo_pdb;
};

#endif		//_DB_BASE_H_
