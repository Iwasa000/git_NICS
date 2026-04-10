#ifndef	__ICSEDITEX_H__
#define	__ICSEDITEX_H__

#pragma once
#include "Font.h"

// Microsoft Visual C++ で作成された IDispatch ラッパー クラスを生成しました。

// メモ: このファイルの内容を変更しないでください。このクラスが
//  Microsoft Visual C++ で再生成された場合、変更は上書きされます。

/////////////////////////////////////////////////////////////////////////////
// CICSEditEX8 ラッパー クラス

class CICSEditEX8 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSEditEX8)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xF1C101AD, 0xA75, 0x47F7, { 0xBB, 0x42, 0xB8, 0xCA, 0xEC, 0xD7, 0xED, 0x39 } };
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

// _DICSEditEX8

// Functions
//


// Properties
//

short GetMaxLength()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}
void SetMaxLength(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}
signed char GetKanji()
{
	signed char result;
	GetProperty(0x2, VT_I1, (void*)&result);
	return result;
}
void SetKanji(signed char propVal)
{
	SetProperty(0x2, VT_I1, propVal);
}
signed char GetKana()
{
	signed char result;
	GetProperty(0x3, VT_I1, (void*)&result);
	return result;
}
void SetKana(signed char propVal)
{
	SetProperty(0x3, VT_I1, propVal);
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
signed char GetBorder()
{
	signed char result;
	GetProperty(0x4, VT_I1, (void*)&result);
	return result;
}
void SetBorder(signed char propVal)
{
	SetProperty(0x4, VT_I1, propVal);
}
signed char GetEdge()
{
	signed char result;
	GetProperty(0x5, VT_I1, (void*)&result);
	return result;
}
void SetEdge(signed char propVal)
{
	SetProperty(0x5, VT_I1, propVal);
}
short GetGyo()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}
void SetGyo(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}
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
signed char GetSelOnFocus()
{
	signed char result;
	GetProperty(0x7, VT_I1, (void*)&result);
	return result;
}
void SetSelOnFocus(signed char propVal)
{
	SetProperty(0x7, VT_I1, propVal);
}
signed char GetNumeric()
{
	signed char result;
	GetProperty(0x8, VT_I1, (void*)&result);
	return result;
}
void SetNumeric(signed char propVal)
{
	SetProperty(0x8, VT_I1, propVal);
}
signed char GetAlphabet()
{
	signed char result;
	GetProperty(0x9, VT_I1, (void*)&result);
	return result;
}
void SetAlphabet(signed char propVal)
{
	SetProperty(0x9, VT_I1, propVal);
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

#endif	//	__ICSEDITEX_H__