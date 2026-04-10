// DBDinpDoc.cpp : CDBDinpDoc クラスの実装
//

#include "stdafx.h"
#include "DBDinp.h"

#include "DBDinpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBDinpDoc

IMPLEMENT_DYNCREATE(CDBDinpDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBDinpDoc, CDocument)
END_MESSAGE_MAP()


// CDBDinpDoc コンストラクション/デストラクション

CDBDinpDoc::CDBDinpDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CDBDinpDoc::~CDBDinpDoc()
{
}

BOOL CDBDinpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CDBDinpDoc シリアル化

void CDBDinpDoc::Serialize(CArchive& ar)
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


// CDBDinpDoc 診断

#ifdef _DEBUG
void CDBDinpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBDinpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDBDinpDoc コマンド
