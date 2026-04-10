// DBSyzShinMainDoc.cpp : CH31HyoDoc クラスの実装
//

#include "stdafx.h"
//#include "DBSyzShinMain.h"

#include "H31HyoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CH26HyoDoc

IMPLEMENT_DYNCREATE(CH31HyoDoc, CDocument)

BEGIN_MESSAGE_MAP(CH31HyoDoc, CDocument)
END_MESSAGE_MAP()


// CH26HyoDoc コンストラクション/デストラクション

CH31HyoDoc::CH31HyoDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CH31HyoDoc::~CH31HyoDoc()
{
}

BOOL CH31HyoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CH31HyoDoc シリアル化

void CH31HyoDoc::Serialize(CArchive& ar)
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


// CH31HyoDoc 診断

#ifdef _DEBUG
void CH31HyoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CH31HyoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CH31HyoDoc コマンド
