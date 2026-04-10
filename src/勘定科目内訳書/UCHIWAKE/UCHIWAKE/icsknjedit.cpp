// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "icsknjedit.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "font.h"

/////////////////////////////////////////////////////////////////////////////
// CICSKNJEdit

IMPLEMENT_DYNCREATE(CICSKNJEdit, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CICSKNJEdit プロパティ

OLE_COLOR CICSKNJEdit::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSKNJEdit::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

CString CICSKNJEdit::GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}

void CICSKNJEdit::SetCaption(LPCTSTR propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}

COleFont CICSKNJEdit::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CICSKNJEdit::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

OLE_COLOR CICSKNJEdit::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSKNJEdit::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

short CICSKNJEdit::GetMaxLength()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CICSKNJEdit::SetMaxLength(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

CString CICSKNJEdit::GetAnk()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void CICSKNJEdit::SetAnk(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

BOOL CICSKNJEdit::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CICSKNJEdit::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

BOOL CICSKNJEdit::GetMaxReturn()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CICSKNJEdit::SetMaxReturn(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

short CICSKNJEdit::GetBorder()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CICSKNJEdit::SetBorder(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

BOOL CICSKNJEdit::GetNextCtrl()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CICSKNJEdit::SetNextCtrl(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

BOOL CICSKNJEdit::GetAnkMix()
{
	BOOL result;
	GetProperty(0xf, VT_BOOL, (void*)&result);
	return result;
}

void CICSKNJEdit::SetAnkMix(BOOL propVal)
{
	SetProperty(0xf, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CICSKNJEdit オペレーション

void CICSKNJEdit::CheckData()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CICSKNJEdit::NextControl(BOOL flg)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 flg);
}

CString CICSKNJEdit::GetReadString()
{
	CString result;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CICSKNJEdit::KanaCancelMode(short mode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 mode);
}

void CICSKNJEdit::ZenHanMix(short mode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 mode);
}

void CICSKNJEdit::SetCursorPos(short pos)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pos);
}

short CICSKNJEdit::GetCursorPos()
{
	short result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CICSKNJEdit::SetCursorBytePos(short pos)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pos);
}

short CICSKNJEdit::GetCursorBytePos()
{
	short result;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}
