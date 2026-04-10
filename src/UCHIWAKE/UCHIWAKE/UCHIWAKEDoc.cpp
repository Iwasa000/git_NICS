// UCHIWAKEDoc.cpp : CUCHIWAKEDoc クラスの実装
//

#include "stdafx.h"
#include "UCHIWAKE.h"

#include "UCHIWAKEDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUCHIWAKEDoc

IMPLEMENT_DYNCREATE(CUCHIWAKEDoc, CDocument)

BEGIN_MESSAGE_MAP(CUCHIWAKEDoc, CDocument)
END_MESSAGE_MAP()


// CUCHIWAKEDoc コンストラクション/デストラクション

CUCHIWAKEDoc::CUCHIWAKEDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CUCHIWAKEDoc::~CUCHIWAKEDoc()
{
}

BOOL CUCHIWAKEDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CUCHIWAKEDoc シリアル化

void CUCHIWAKEDoc::Serialize(CArchive& ar)
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


// CUCHIWAKEDoc 診断

#ifdef _DEBUG
void CUCHIWAKEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUCHIWAKEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUCHIWAKEDoc コマンド
