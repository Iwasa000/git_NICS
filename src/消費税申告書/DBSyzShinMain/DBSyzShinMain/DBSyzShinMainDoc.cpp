// DBSyzShinMainDoc.cpp : CDBSyzShinMainDoc クラスの実装
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "DBSyzShinMainDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBSyzShinMainDoc

IMPLEMENT_DYNCREATE(CDBSyzShinMainDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBSyzShinMainDoc, CDocument)
END_MESSAGE_MAP()


// CDBSyzShinMainDoc コンストラクション/デストラクション

CDBSyzShinMainDoc::CDBSyzShinMainDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CDBSyzShinMainDoc::~CDBSyzShinMainDoc()
{
}

BOOL CDBSyzShinMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CDBSyzShinMainDoc シリアル化

void CDBSyzShinMainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}


// CDBSyzShinMainDoc 診断

#ifdef _DEBUG
void CDBSyzShinMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBSyzShinMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDBSyzShinMainDoc コマンド
