// DBDinp.h : DBDinp アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

// CDBDinpApp:
// このクラスの実装については、DBDinp.cpp を参照してください。
//

class CDBDinpApp : public ICSWinApp
{
public:
	CDBDinpApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int		BeginApp();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDBDinpApp theApp;

extern CWnd* GetDBDinpMainFrame();
