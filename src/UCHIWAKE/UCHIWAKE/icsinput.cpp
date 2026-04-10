// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "icsinput.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "font.h"

/////////////////////////////////////////////////////////////////////////////
// CICSInput

IMPLEMENT_DYNCREATE(CICSInput, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CICSInput プロパティ

CString CICSInput::GetKind()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void CICSInput::SetKind(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

OLE_COLOR CICSInput::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSInput::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

CString CICSInput::GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}

void CICSInput::SetCaption(LPCTSTR propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}

COleFont CICSInput::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CICSInput::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

OLE_COLOR CICSInput::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSInput::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

short CICSInput::GetMax()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CICSInput::SetMax(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

BOOL CICSInput::GetMinus()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CICSInput::SetMinus(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL CICSInput::GetKeta()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CICSInput::SetKeta(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

CString CICSInput::GetForm()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void CICSInput::SetForm(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

BOOL CICSInput::GetEda()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void CICSInput::SetEda(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

BOOL CICSInput::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CICSInput::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

short CICSInput::GetBorder()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}

void CICSInput::SetBorder(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}

BOOL CICSInput::GetNextCtrl()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}

void CICSInput::SetNextCtrl(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}

BOOL CICSInput::GetAnkMix()
{
	BOOL result;
	GetProperty(0x16, VT_BOOL, (void*)&result);
	return result;
}

void CICSInput::SetAnkMix(BOOL propVal)
{
	SetProperty(0x16, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CICSInput オペレーション

void CICSInput::ModeSet(short mode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 mode);
}

void CICSInput::SetData(LPUNKNOWN data)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data);
}

void CICSInput::ReplyData(short flg, LPUNKNOWN data)
{
	static BYTE parms[] =
		VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 flg, data);
}

short CICSInput::GetData(LPUNKNOWN data)
{
	short result;
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		data);
	return result;
}

short CICSInput::GetCursorPos()
{
	short result;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CICSInput::TekiInsert(LPCTSTR data)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data);
}

short CICSInput::SetCursorPos(short pos)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		pos);
	return result;
}

void CICSInput::NextControl(BOOL flg)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 flg);
}

short CICSInput::EntryKey(short key)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		key);
	return result;
}

short CICSInput::RemoveKey(short key)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		key);
	return result;
}

void CICSInput::ZenHanMix(short mode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 mode);
}

void CICSInput::SetCursorBytePos(short pos)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pos);
}

short CICSInput::GetCursorBytePos()
{
	short result;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long CICSInput::GetKanaTermChar()
{
	long result;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CICSInput::TerminationMode(short mode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 mode);
}
