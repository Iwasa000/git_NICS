// SyzAcsTestDoc.cpp : CSyzAcsTestDoc クラスの実装
//

#include "stdafx.h"
#include "SyzAcsTest.h"

#include "SyzAcsTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSyzAcsTestDoc

IMPLEMENT_DYNCREATE(CSyzAcsTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CSyzAcsTestDoc, CDocument)
END_MESSAGE_MAP()


// CSyzAcsTestDoc コンストラクション/デストラクション

CSyzAcsTestDoc::CSyzAcsTestDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CSyzAcsTestDoc::~CSyzAcsTestDoc()
{
}

BOOL CSyzAcsTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CSyzAcsTestDoc シリアル化

void CSyzAcsTestDoc::Serialize(CArchive& ar)
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


// CSyzAcsTestDoc 診断

#ifdef _DEBUG
void CSyzAcsTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSyzAcsTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSyzAcsTestDoc コマンド
