// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "icsdbedt.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "font.h"

/////////////////////////////////////////////////////////////////////////////
// CICSDBEDT

IMPLEMENT_DYNCREATE(CICSDBEDT, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CICSDBEDT プロパティ

OLE_COLOR CICSDBEDT::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSDBEDT::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

OLE_COLOR CICSDBEDT::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSDBEDT::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

COleFont CICSDBEDT::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CICSDBEDT::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

unsigned long CICSDBEDT::GetFocusColor()
{
	unsigned long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CICSDBEDT::SetFocusColor(unsigned long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

unsigned long CICSDBEDT::GetInverseColor()
{
	unsigned long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CICSDBEDT::SetInverseColor(unsigned long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

BOOL CICSDBEDT::GetInverse()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetInverse(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetMultiLine()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetMultiLine(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

short CICSDBEDT::GetLayout()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}

void CICSDBEDT::SetLayout(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}

BOOL CICSDBEDT::GetNumeric()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetNumeric(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetAlphabet()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetAlphabet(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetKana()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetKana(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetZenkaku()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetZenkaku(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetAscii()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetAscii(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetVector()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetVector(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetFormat()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetFormat(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}

CString CICSDBEDT::GetMinval()
{
	CString result;
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}

void CICSDBEDT::SetMinval(LPCTSTR propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}

CString CICSDBEDT::GetMaxval()
{
	CString result;
	GetProperty(0xe, VT_BSTR, (void*)&result);
	return result;
}

void CICSDBEDT::SetMaxval(LPCTSTR propVal)
{
	SetProperty(0xe, VT_BSTR, propVal);
}

long CICSDBEDT::GetMaxLen()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void CICSDBEDT::SetMaxLen(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

BOOL CICSDBEDT::GetCmove()
{
	BOOL result;
	GetProperty(0x10, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetCmove(BOOL propVal)
{
	SetProperty(0x10, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetAsynch()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetAsynch(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}

BOOL CICSDBEDT::GetRegColor()
{
	BOOL result;
	GetProperty(0x12, VT_BOOL, (void*)&result);
	return result;
}

void CICSDBEDT::SetRegColor(BOOL propVal)
{
	SetProperty(0x12, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CICSDBEDT オペレーション

long CICSDBEDT::GetData(VARIANT* data, long type, short bcdlen)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_I4 VTS_I2;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		data, type, bcdlen);
	return result;
}

long CICSDBEDT::SetData(VARIANT* data, long type, short bcdlen)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_I4 VTS_I2;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		data, type, bcdlen);
	return result;
}

void CICSDBEDT::SetCaretPosition(short pos)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pos);
}

short CICSDBEDT::GetCaretPosition()
{
	short result;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long CICSDBEDT::InsertData(VARIANT* data, long type, short bcdlen, short pos)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_I4 VTS_I2 VTS_I2;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		data, type, bcdlen, pos);
	return result;
}

void CICSDBEDT::SetDisplayString(LPCTSTR string)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 string);
}

void CICSDBEDT::SetCalendar(BOOL enable, long pwnd, short dtype, short dformat)
{
	static BYTE parms[] =
		VTS_BOOL VTS_I4 VTS_I2 VTS_I2;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 enable, pwnd, dtype, dformat);
}

void CICSDBEDT::SetCalculator(BOOL enable, long pwnd)
{
	static BYTE parms[] =
		VTS_BOOL VTS_I4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 enable, pwnd);
}

void CICSDBEDT::SetToolTip(LPCTSTR string)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 string);
}

void CICSDBEDT::SetKanaTerm(BOOL sw, short len, long type)
{
	static BYTE parms[] =
		VTS_BOOL VTS_I2 VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sw, len, type);
}

void CICSDBEDT::ImeEndMode(short mode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 mode);
}

void CICSDBEDT::SetFrame(short mode, unsigned long color)
{
	static BYTE parms[] = VTS_I2 VTS_UI4 ;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode, color);
}

void CICSDBEDT::SetVscroll(short mode)
{
	static BYTE parms[] = VTS_I2 ;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
}

void CICSDBEDT::AllowReturn(short mode)
{
	static BYTE parms[] = VTS_I2 ;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
}

void CICSDBEDT::EnableDelete(short mode)
{
	static BYTE parms[] = VTS_I2 ;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
}


void CICSDBEDT::SetZei(short mode)
{
	static BYTE parms[] = VTS_I2 ;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
}

void CICSDBEDT::SetZeiritsu(double ritsu)
{
	static BYTE parms[] = VTS_R8 ;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ritsu);
}

long CICSDBEDT::GetSelect()
{
	long result;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CICSDBEDT::SetSelect(long start, long end)
{
	static BYTE parms[] = VTS_I4 VTS_I4 ;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, start, end);
}
void CICSDBEDT::SetKaigyo(long lb)
{
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lb);
}
void CICSDBEDT::EnableHome(short mode)
{
	static BYTE parms[] = VTS_I2 ;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
}

void CICSDBEDT::CharInMode(short mode)
{
	static BYTE parms[] = VTS_I2 ;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
}
void CICSDBEDT::SetExtension(long mode1, long mode2)
{
	static BYTE parms[] = VTS_I4 VTS_I4 ;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode1, mode2);
}

