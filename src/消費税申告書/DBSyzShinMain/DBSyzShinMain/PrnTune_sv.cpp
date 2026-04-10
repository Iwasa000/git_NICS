// PrnTune.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "PrnTune.h"


// CPrnTune ダイアログ

IMPLEMENT_DYNAMIC(CPrnTune, ICSDialog)

CPrnTune::CPrnTune(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnTune::IDD, pParent)
{

}

CPrnTune::~CPrnTune()
{
}

void CPrnTune::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrnTune, ICSDialog)
END_MESSAGE_MAP()


// CPrnTune メッセージ ハンドラ
