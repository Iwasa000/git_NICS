// DBSyzShinMainDoc.cpp : CH26HyoDoc クラスの実装
//

#include "stdafx.h"
//#include "DBSyzShinMain.h"

#include "H26HyoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CH26HyoDoc

IMPLEMENT_DYNCREATE(CH26HyoDoc, CDocument)

BEGIN_MESSAGE_MAP(CH26HyoDoc, CDocument)
END_MESSAGE_MAP()


// CH26HyoDoc コンストラクション/デストラクション

CH26HyoDoc::CH26HyoDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CH26HyoDoc::~CH26HyoDoc()
{
}

BOOL CH26HyoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CH26HyoDoc シリアル化

void CH26HyoDoc::Serialize(CArchive& ar)
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


// CH26HyoDoc 診断

#ifdef _DEBUG
void CH26HyoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CH26HyoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CH26HyoDoc コマンド
