// ICS Application1Doc.cpp : CSimulateKuriwariDriverDoc クラスの実装
//

#include "stdafx.h"
#include "SimulateKuriwariDriver.h"

#include "SimulateKuriwariDriverDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimulateKuriwariDriverDoc

IMPLEMENT_DYNCREATE(CSimulateKuriwariDriverDoc, CDocument)

BEGIN_MESSAGE_MAP(CSimulateKuriwariDriverDoc, CDocument)
END_MESSAGE_MAP()


// CSimulateKuriwariDriverDoc コンストラクション/デストラクション

CSimulateKuriwariDriverDoc::CSimulateKuriwariDriverDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CSimulateKuriwariDriverDoc::~CSimulateKuriwariDriverDoc()
{
}

BOOL CSimulateKuriwariDriverDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

// CSimulateKuriwariDriverDoc シリアル化

void CSimulateKuriwariDriverDoc::Serialize(CArchive& ar)
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


// CSimulateKuriwariDriverDoc 診断

#ifdef _DEBUG
void CSimulateKuriwariDriverDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSimulateKuriwariDriverDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSimulateKuriwariDriverDoc コマンド
