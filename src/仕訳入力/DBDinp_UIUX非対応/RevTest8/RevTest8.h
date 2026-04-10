// RevTest8.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CRevTest8App:
// このクラスの実装については、RevTest8.cpp を参照してください。
//

class CRevTest8App : public CWinApp
{
public:
	CRevTest8App();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CRevTest8App theApp;