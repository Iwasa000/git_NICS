// SyzAcsTest.h : SyzAcsTest アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル


// CSyzAcsTestApp:
// このクラスの実装については、SyzAcsTest.cpp を参照してください。
//

class CSyzAcsTestApp : public ICSWinApp
{
public:
	CSyzAcsTestApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int		BeginApp();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSyzAcsTestApp theApp;
