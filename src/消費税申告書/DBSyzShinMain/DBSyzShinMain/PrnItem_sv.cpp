// PrnItem.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "PrnItem.h"


// CPrnItem ダイアログ

IMPLEMENT_DYNAMIC(CPrnItem, ICSDialog)

CPrnItem::CPrnItem(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnItem::IDD, pParent)
{

}

CPrnItem::~CPrnItem()
{
}

void CPrnItem::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrnItem, ICSDialog)
END_MESSAGE_MAP()


// CPrnItem メッセージ ハンドラ
