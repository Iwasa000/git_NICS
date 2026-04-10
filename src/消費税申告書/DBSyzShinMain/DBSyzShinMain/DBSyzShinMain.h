// DBSyzShinMain.h : DBSyzShinMain アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

// CDBSyzShinMainApp:
// このクラスの実装については、DBSyzShinMain.cpp を参照してください。
//

class CDBSyzShinMainApp : public ICSWinApp
{
public:
	CDBSyzShinMainApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int		BeginApp();

private:
	int		Kaisei_VER;	// EnumIdKaiseiVer：作成されているビューの内、どのビューが有効であるのかを判定する為に使用。

// 実装
public:
	CWnd* GetHonHyoView();
	CWnd* GetTaxListView();

	CWnd* GetHonHyoView(int version);
	CWnd* GetTaxListView(int version);

	void  SetKaiseiVer(int ver);
	int GetKaiseiVer(){ return Kaisei_VER; };

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CDBSyzShinMainApp theApp;
