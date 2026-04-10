// DBSyzShinMainDoc.cpp : CTaxListDoc クラスの実装
//

#include "stdafx.h"
//#include "DBSyzShinMain.h"

#include "TaxListDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTaxListDoc

IMPLEMENT_DYNCREATE(CTaxListDoc, CDocument)

BEGIN_MESSAGE_MAP(CTaxListDoc, CDocument)
END_MESSAGE_MAP()


// CTaxListDoc コンストラクション/デストラクション

CTaxListDoc::CTaxListDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CTaxListDoc::~CTaxListDoc()
{
}

BOOL CTaxListDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CTaxListDoc シリアル化

void CTaxListDoc::Serialize(CArchive& ar)
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


// CTaxListDoc 診断

#ifdef _DEBUG
void CTaxListDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTaxListDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTaxListDoc コマンド
