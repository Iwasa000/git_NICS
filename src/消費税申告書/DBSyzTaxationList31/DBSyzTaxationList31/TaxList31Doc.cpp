// DBSyzShinMainDoc.cpp : CTaxList31Doc クラスの実装
//

#include "stdafx.h"
//#include "DBSyzShinMain.h"

#include "TaxList31Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTaxList31Doc

IMPLEMENT_DYNCREATE(CTaxList31Doc, CDocument)

BEGIN_MESSAGE_MAP(CTaxList31Doc, CDocument)
END_MESSAGE_MAP()


// CTaxList31Doc コンストラクション/デストラクション

CTaxList31Doc::CTaxList31Doc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CTaxList31Doc::~CTaxList31Doc()
{
}

BOOL CTaxList31Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CTaxList31Doc シリアル化

void CTaxList31Doc::Serialize(CArchive& ar)
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


// CTaxList31Doc 診断

#ifdef _DEBUG
void CTaxList31Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTaxList31Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTaxList31Doc コマンド
