// dllmain.cpp : DLL の初期化ルーチンを定義します。
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#include "SimulateKuriwari.h"
#include "SimKuriwariDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE SimulateKuriwariDLL = { false, nullptr };

HINSTANCE g_hInstance;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SimulateKuriwari.DLL を初期化しています。\n");

		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(SimulateKuriwariDLL, hInstance))
			return 0;

		g_hInstance = hInstance;

		// この DLL をリソース チェーンへ挿入します。
		// メモ : この拡張 DLL が暗黙的に、MFC アプリケーションではなく
		//  ActiveX コントロールなどの MFC 標準 DLL によってリンクされている場合、
		//  以下の行を DllMain から削除して
		//  から削除して、この拡張 DLL からエクスポート
		//  配置してください。したがって、この拡張 DLL を使う標準 DLL は、
		//  その関数を明示的に呼び出して、
		//  を初期化するために明示的にその関数を呼び出します。
		//  それ以外の場合は、CDynLinkLibrary オブジェクトは
		//  標準 DLL のリソース チェーンへアタッチされず、
		//  その結果重大な問題となります。

		new CDynLinkLibrary(SimulateKuriwariDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SimulateKuriwari.DLL を終了しています。\n");

		// デストラクターが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(SimulateKuriwariDLL);
	}
	return 1;   // OK
}

HINSTANCE _GetSimulateKuriwariInstanceHandle() {
	return g_hInstance;
}

CSimKuriwari::CSimKuriwari(CDBZmSub* pZmSub)
{
	m_pSimKuriwariW = new CSimKuriwariW(pZmSub);
}

CSimKuriwari::~CSimKuriwari()
{
    delete m_pSimKuriwariW;
}

int CSimKuriwari::CreateDlg(CWnd* pParent)
{
    int st = 0;

    //An HINSTANCE handle where the default resources of the application are loaded.
    HINSTANCE svhInstResource = AfxGetResourceHandle();

	//This function sets the HINSTANCE handle that determines where the default resources of the application are loaded.
    AfxSetResourceHandle( _GetSimulateKuriwariInstanceHandle() );

    st = m_pSimKuriwariW->CreateDlg(pParent);

	AfxSetResourceHandle(svhInstResource);
	return st;
}

int CSimKuriwari::CreateSyukeiDlg(CWnd* pParent, 
                                  long sddate, long eddate)
{
	int st = 0;

	//An HINSTANCE handle where the default resources of the application are loaded.
	HINSTANCE svhInstResource = AfxGetResourceHandle();

	//This function sets the HINSTANCE handle that determines where the default resources of the application are loaded.
	AfxSetResourceHandle(_GetSimulateKuriwariInstanceHandle());

	st = m_pSimKuriwariW->CreateSyukeiDlg(pParent, sddate, eddate);

	AfxSetResourceHandle(svhInstResource);
	return st;

}

void CSimKuriwari::EndDlg(int nResult)
{

	return m_pSimKuriwariW->EndSimKuriwariDlg(nResult);

}

void CSimKuriwari::EndSyukeiDlg(int nResult)
{

	return m_pSimKuriwariW->EndSyukeiDlg(nResult);

}

int CSimKuriwari::ReadDlgSyukeiData(SIMKURIWARIZREC* pRec)
{

	m_pSimKuriwariW->ReadDlgSyukeiData(pRec);

	return 0;
}

int CSimKuriwari::SyukeiKuriwari10Eigyobi(SIMKURIWARIZREC* pRec, long sddate, long eddate)
{
	m_pSimKuriwariW->SyukeiKuriwari10Eigyobi(pRec, sddate, eddate);

    return 0;
}

int CSimKuriwari::SyukeiKsirewari(SIMKSIREWARIZREC* pRec, bool IsNeedTkkz, long sddate, long eddate)
{

	m_pSimKuriwariW->SyukeiKsirewari(pRec, IsNeedTkkz, sddate, eddate);

    return 0;
}

