// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "icshedit.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "font.h"

/////////////////////////////////////////////////////////////////////////////
// CICSHedit

IMPLEMENT_DYNCREATE(CICSHedit, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CICSHedit プロパティ

OLE_COLOR CICSHedit::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSHedit::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

CString CICSHedit::GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}

void CICSHedit::SetCaption(LPCTSTR propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}

COleFont CICSHedit::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CICSHedit::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

OLE_COLOR CICSHedit::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSHedit::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

CString CICSHedit::GetKind()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void CICSHedit::SetKind(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

short CICSHedit::GetMax()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CICSHedit::SetMax(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

CString CICSHedit::GetAlign()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void CICSHedit::SetAlign(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

BOOL CICSHedit::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CICSHedit::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

short CICSHedit::GetBorder()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CICSHedit::SetBorder(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

BOOL CICSHedit::GetNextCtrl()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CICSHedit::SetNextCtrl(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CICSHedit オペレーション

void CICSHedit::SetFormat(LPCTSTR format)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 format);
}

void CICSHedit::SetData(LPCTSTR data)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data);
}

void CICSHedit::GetData(LPUNKNOWN data, short* length)
{
	static BYTE parms[] =
		VTS_UNKNOWN VTS_PI2;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data, length);
}

void CICSHedit::NextControl(BOOL flg)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 flg);
}

void CICSHedit::DataClear()
{
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
