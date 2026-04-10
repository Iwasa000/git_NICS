// SyzMakeTest.h : SyzMakeTest アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル


// CSyzMakeTestApp:
// このクラスの実装については、SyzMakeTest.cpp を参照してください。
//

class CSyzMakeTestApp : public ICSWinApp
{
public:
	CSyzMakeTestApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int		BeginApp();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSyzMakeTestApp theApp;
