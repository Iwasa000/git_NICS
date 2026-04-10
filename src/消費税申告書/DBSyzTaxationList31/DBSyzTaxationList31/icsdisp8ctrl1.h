#pragma once
#include "_Picture.h"
#include "_Font.h"

// Microsoft Visual C++ で作成された IDispatch ラッパー クラスを生成しました。

// メモ: このファイルの内容を変更しないでください。このクラスが
//  Microsoft Visual C++ で再生成された場合、変更は上書きされます。

/////////////////////////////////////////////////////////////////////////////
// CIcsdisp8ctrl1 ラッパー クラス

class CIcsdisp8ctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CIcsdisp8ctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x9B6935F7, 0xCDC, 0x4909, { 0x93, 0xBC, 0x7F, 0x34, 0x91, 0x65, 0xE9, 0xD6 } };
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

// _DICSDisp8

// Functions
//

	void ColmDisp(short colmn, LPCTSTR data)
	{
		static BYTE parms[] = VTS_I2 VTS_BSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, colmn, data);
	}
	void ClearDisp()
	{
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LinClmDisp(short lin, short clm, LPCTSTR data)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lin, clm, data);
	}
	void DispRect(short xpos, short ypos, short width, short height, unsigned long cl)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_UI4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, xpos, ypos, width, height, cl);
	}
	void DluDisp(LPUNKNOWN data)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, data);
	}
	void DotDisp(LPUNKNOWN data)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, data);
	}
	void ImgDisp(long xpos, long ypos, short width, short hight, LPUNKNOWN data, short flg)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_I2 VTS_UNKNOWN VTS_I2 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, xpos, ypos, width, hight, data, flg);
	}
	void CheckData()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

CString GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}
void SetCaption(CString propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}
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
short GetCaptionPitch()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}
void SetCaptionPitch(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}
CString GetCaptionArrange()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}
void SetCaptionArrange(CString propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}
CString GetCaptionEscapement()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}
void SetCaptionEscapement(CString propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}
signed char GetCaptionPitchUse()
{
	signed char result;
	GetProperty(0x4, VT_I1, (void*)&result);
	return result;
}
void SetCaptionPitchUse(signed char propVal)
{
	SetProperty(0x4, VT_I1, propVal);
}
signed char GetPictureType()
{
	signed char result;
	GetProperty(0x5, VT_I1, (void*)&result);
	return result;
}
void SetPictureType(signed char propVal)
{
	SetProperty(0x5, VT_I1, propVal);
}
signed char GetCaptionMode()
{
	signed char result;
	GetProperty(0x6, VT_I1, (void*)&result);
	return result;
}
void SetCaptionMode(signed char propVal)
{
	SetProperty(0x6, VT_I1, propVal);
}
short GetPictureOffsetY()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}
void SetPictureOffsetY(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}
short GetPictureOffsetX()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}
void SetPictureOffsetX(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}
CPicture GetCtlpicture()
{
	LPDISPATCH result;
	GetProperty(0xa, VT_DISPATCH, (void*)&result);
	return CPicture(result);
}
void SetCtlpicture(LPDISPATCH propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}
signed char GetFrame()
{
	signed char result;
	GetProperty(0x7, VT_I1, (void*)&result);
	return result;
}
void SetFrame(signed char propVal)
{
	SetProperty(0x7, VT_I1, propVal);
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


};
