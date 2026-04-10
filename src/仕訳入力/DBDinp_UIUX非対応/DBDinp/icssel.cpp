// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "icssel.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "font.h"

/////////////////////////////////////////////////////////////////////////////
// CICSSel

IMPLEMENT_DYNCREATE(CICSSel, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CICSSel プロパティ

OLE_COLOR CICSSel::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSSel::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

COleFont CICSSel::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CICSSel::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

OLE_COLOR CICSSel::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSSel::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

short CICSSel::GetGyou()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CICSSel::SetGyou(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

short CICSSel::GetRetsu()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CICSSel::SetRetsu(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

BOOL CICSSel::GetNumber()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CICSSel::SetNumber(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

short CICSSel::GetButtom()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CICSSel::SetButtom(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CICSSel オペレーション

void CICSSel::SetData(long data)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data);
}

void CICSSel::SetLineColor(unsigned long cl)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 cl);
}
