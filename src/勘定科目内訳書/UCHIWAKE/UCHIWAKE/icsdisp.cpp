// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "icsdisp.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "font.h"
#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CICSDisp

IMPLEMENT_DYNCREATE(CICSDisp, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CICSDisp プロパティ

CString CICSDisp::GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}

void CICSDisp::SetCaption(LPCTSTR propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}

OLE_COLOR CICSDisp::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSDisp::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

COleFont CICSDisp::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CICSDisp::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

OLE_COLOR CICSDisp::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSDisp::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

short CICSDisp::GetCaptionPitch()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CICSDisp::SetCaptionPitch(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

CString CICSDisp::GetCaptionArrange()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void CICSDisp::SetCaptionArrange(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString CICSDisp::GetCaptionEscapement()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void CICSDisp::SetCaptionEscapement(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

BOOL CICSDisp::GetCaptionPitchUse()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CICSDisp::SetCaptionPitchUse(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL CICSDisp::GetPictureType()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CICSDisp::SetPictureType(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

BOOL CICSDisp::GetCaptionMode()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void CICSDisp::SetCaptionMode(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

short CICSDisp::GetPictureOffsetY()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void CICSDisp::SetPictureOffsetY(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

short CICSDisp::GetPictureOffsetX()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}

void CICSDisp::SetPictureOffsetX(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}

CPicture CICSDisp::GetCtlpicture()
{
	LPDISPATCH pDispatch;
	GetProperty(0xa, VT_DISPATCH, (void*)&pDispatch);
	return CPicture(pDispatch);
}

void CICSDisp::SetCtlpicture(LPDISPATCH propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}

BOOL CICSDisp::GetFrame()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void CICSDisp::SetFrame(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

BOOL CICSDisp::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CICSDisp::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CICSDisp オペレーション

void CICSDisp::ColmDisp(short colmn, LPCTSTR data)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 colmn, data);
}

void CICSDisp::ClearDisp()
{
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CICSDisp::LinClmDisp(short lin, short clm, LPCTSTR data)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lin, clm, data);
}

void CICSDisp::DispRect(short xpos, short ypos, short width, short height, unsigned long cl)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 xpos, ypos, width, height, cl);
}

void CICSDisp::DluDisp(LPUNKNOWN data)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data);
}

void CICSDisp::DotDisp(LPUNKNOWN data)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data);
}

void CICSDisp::ImgDisp(long xpos, long ypos, short width, short hight, LPUNKNOWN data, short flg)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I2 VTS_I2 VTS_UNKNOWN VTS_I2;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 xpos, ypos, width, hight, data, flg);
}

void CICSDisp::CheckData()
{
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
