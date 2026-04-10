// SyzMakeTestDoc.cpp : CSyzMakeTestDoc クラスの実装
//

#include "stdafx.h"
#include "SyzMakeTest.h"

#include "SyzMakeTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSyzMakeTestDoc

IMPLEMENT_DYNCREATE(CSyzMakeTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CSyzMakeTestDoc, CDocument)
END_MESSAGE_MAP()


// CSyzMakeTestDoc コンストラクション/デストラクション

CSyzMakeTestDoc::CSyzMakeTestDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CSyzMakeTestDoc::~CSyzMakeTestDoc()
{
}

BOOL CSyzMakeTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CSyzMakeTestDoc シリアル化

void CSyzMakeTestDoc::Serialize(CArchive& ar)
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


// CSyzMakeTestDoc 診断

#ifdef _DEBUG
void CSyzMakeTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSyzMakeTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSyzMakeTestDoc コマンド
