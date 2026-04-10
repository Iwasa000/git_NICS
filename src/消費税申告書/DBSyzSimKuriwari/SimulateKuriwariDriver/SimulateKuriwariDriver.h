// ICS Application1.h : ICS Application1 アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル


// CSimulateKuriwariDriverApp:
// このクラスの実装については、ICS Application1.cpp を参照してください。
//

class CSimulateKuriwariDriverApp : public ICSWinApp
{
public:
	CSimulateKuriwariDriverApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int		BeginApp();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSimulateKuriwariDriverApp theApp;
