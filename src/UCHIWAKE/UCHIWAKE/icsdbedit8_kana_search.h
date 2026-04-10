#pragma once
#include "_Font.h"

// Microsoft Visual C++ で作成された IDispatch ラッパー クラスを生成しました。

// メモ: このファイルの内容を変更しないでください。このクラスが
//  Microsoft Visual C++ で再生成された場合、変更は上書きされます。

/////////////////////////////////////////////////////////////////////////////
// CIcsdbedit8_kana_search ラッパー クラス

class CIcsdbedit8_kana_search : public CWnd
{
protected:
	DECLARE_DYNCREATE(CIcsdbedit8_kana_search)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xED3AD240, 0x3068, 0x44E5, { 0xA8, 0x7, 0x91, 0xD6, 0xB6, 0x57, 0x14, 0x8F } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// _DICSDBEDT8

// Functions
//

	long GetData(VARIANT * data, long type, short bcdlen)
	{
		long result;
		static BYTE parms[] = VTS_PVARIANT VTS_I4 VTS_I2 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, data, type, bcdlen);
		return result;
	}
	long SetData(VARIANT * data, long type, short bcdlen)
	{
		long result;
		static BYTE parms[] = VTS_PVARIANT VTS_I4 VTS_I2 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, data, type, bcdlen);
		return result;
	}
	void SetCaretPosition(short pos)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pos);
	}
	short GetCaretPosition()
	{
		short result;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	long InsertData(VARIANT * data, long type, short bcdlen, short pos)
	{
		long result;
		static BYTE parms[] = VTS_PVARIANT VTS_I4 VTS_I2 VTS_I2 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, data, type, bcdlen, pos);
		return result;
	}
	void SetDisplayString(LPCTSTR string)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, string);
	}
	void SetCalendar(signed char enable, long pwnd, short dtype, short dformat)
	{
		static BYTE parms[] = VTS_I1 VTS_I4 VTS_I2 VTS_I2 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, enable, pwnd, dtype, dformat);
	}
	void SetCalculator(signed char enable, long pwnd)
	{
		static BYTE parms[] = VTS_I1 VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, enable, pwnd);
	}
	void SetToolTip(LPCTSTR string)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, string);
	}
	void SetKanaTerm(signed char sw, short len, long type)
	{
		static BYTE parms[] = VTS_I1 VTS_I2 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sw, len, type);
	}
	void ImeEndMode(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void SetFrame(short mode, unsigned long color)
	{
		static BYTE parms[] = VTS_I2 VTS_UI4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode, color);
	}
	void SetVscroll(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void AllowReturn(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void EnableDelete(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void SetZei(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void SetZeiritsu(double ritsu)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ritsu);
	}
	long GetSelect()
	{
		long result;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetSelect(long start, long end)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, start, end);
	}
	void SetKaigyo(long lb)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lb);
	}
	void EnableHome(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void CharInMode(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void SetExtension(long mode1, long mode2)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode1, mode2);
	}

// Properties
//

unsigned long GetBackColor()
{
	unsigned long result;
	GetProperty(DISPID_BACKCOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetBackColor(unsigned long propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_UI4, propVal);
}
unsigned long GetForeColor()
{
	unsigned long result;
	GetProperty(DISPID_FORECOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetForeColor(unsigned long propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_UI4, propVal);
}
COleFont GetFont()
{
	LPDISPATCH result;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
	return COleFont(result);
}
void SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}
unsigned long GetFocusColor()
{
	unsigned long result;
	GetProperty(0x1, VT_UI4, (void*)&result);
	return result;
}
void SetFocusColor(unsigned long propVal)
{
	SetProperty(0x1, VT_UI4, propVal);
}
unsigned long GetInverseColor()
{
	unsigned long result;
	GetProperty(0x2, VT_UI4, (void*)&result);
	return result;
}
void SetInverseColor(unsigned long propVal)
{
	SetProperty(0x2, VT_UI4, propVal);
}
signed char GetInverse()
{
	signed char result;
	GetProperty(0x3, VT_I1, (void*)&result);
	return result;
}
void SetInverse(signed char propVal)
{
	SetProperty(0x3, VT_I1, propVal);
}
signed char GetMultiLine()
{
	signed char result;
	GetProperty(0x4, VT_I1, (void*)&result);
	return result;
}
void SetMultiLine(signed char propVal)
{
	SetProperty(0x4, VT_I1, propVal);
}
signed char GetEnabled()
{
	signed char result;
	GetProperty(DISPID_ENABLED, VT_I1, (void*)&result);
	return result;
}
void SetEnabled(signed char propVal)
{
	SetProperty(DISPID_ENABLED, VT_I1, propVal);
}
short GetLayout()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}
void SetLayout(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}
signed char GetNumeric()
{
	signed char result;
	GetProperty(0x6, VT_I1, (void*)&result);
	return result;
}
void SetNumeric(signed char propVal)
{
	SetProperty(0x6, VT_I1, propVal);
}
signed char GetAlphabet()
{
	signed char result;
	GetProperty(0x7, VT_I1, (void*)&result);
	return result;
}
void SetAlphabet(signed char propVal)
{
	SetProperty(0x7, VT_I1, propVal);
}
signed char GetKana()
{
	signed char result;
	GetProperty(0x8, VT_I1, (void*)&result);
	return result;
}
void SetKana(signed char propVal)
{
	SetProperty(0x8, VT_I1, propVal);
}
signed char GetZenkaku()
{
	signed char result;
	GetProperty(0x9, VT_I1, (void*)&result);
	return result;
}
void SetZenkaku(signed char propVal)
{
	SetProperty(0x9, VT_I1, propVal);
}
signed char GetAscii()
{
	signed char result;
	GetProperty(0xa, VT_I1, (void*)&result);
	return result;
}
void SetAscii(signed char propVal)
{
	SetProperty(0xa, VT_I1, propVal);
}
signed char GetVector()
{
	signed char result;
	GetProperty(0xb, VT_I1, (void*)&result);
	return result;
}
void SetVector(signed char propVal)
{
	SetProperty(0xb, VT_I1, propVal);
}
signed char GetFormat()
{
	signed char result;
	GetProperty(0xc, VT_I1, (void*)&result);
	return result;
}
void SetFormat(signed char propVal)
{
	SetProperty(0xc, VT_I1, propVal);
}
CString GetMinval()
{
	CString result;
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}
void SetMinval(CString propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}
CString GetMaxval()
{
	CString result;
	GetProperty(0xe, VT_BSTR, (void*)&result);
	return result;
}
void SetMaxval(CString propVal)
{
	SetProperty(0xe, VT_BSTR, propVal);
}
long GetMaxLen()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}
void SetMaxLen(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}
signed char GetCmove()
{
	signed char result;
	GetProperty(0x10, VT_I1, (void*)&result);
	return result;
}
void SetCmove(signed char propVal)
{
	SetProperty(0x10, VT_I1, propVal);
}
signed char GetAsynch()
{
	signed char result;
	GetProperty(0x11, VT_I1, (void*)&result);
	return result;
}
void SetAsynch(signed char propVal)
{
	SetProperty(0x11, VT_I1, propVal);
}
signed char GetRegColor()
{
	signed char result;
	GetProperty(0x12, VT_I1, (void*)&result);
	return result;
}
void SetRegColor(signed char propVal)
{
	SetProperty(0x12, VT_I1, propVal);
}


};
