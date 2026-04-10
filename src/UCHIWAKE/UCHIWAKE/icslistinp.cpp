// Microsoft Visual C++ で自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。

#include "stdafx.h"
#include "icslistinp.h"

// このインターフェイスによって参照されるディスパッチ インターフェイス
#include "Font.h"

/////////////////////////////////////////////////////////////////////////////
// CICSListInp

IMPLEMENT_DYNCREATE(CICSListInp, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CICSListInp プロパティ

OLE_COLOR CICSListInp::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSListInp::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

COleFont CICSListInp::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CICSListInp::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

OLE_COLOR CICSListInp::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CICSListInp::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

BOOL CICSListInp::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CICSListInp::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

BOOL CICSListInp::GetHorzGrid()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void CICSListInp::SetHorzGrid(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL CICSListInp::GetVertGrid()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CICSListInp::SetVertGrid(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL CICSListInp::GetReadOnly()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CICSListInp::SetReadOnly(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

short CICSListInp::GetHorzPerLine()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CICSListInp::SetHorzPerLine(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

BOOL CICSListInp::GetHorzScroll()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CICSListInp::SetHorzScroll(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

BOOL CICSListInp::GetNoResize()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void CICSListInp::SetNoResize(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

BOOL CICSListInp::GetInverse()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void CICSListInp::SetInverse(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CICSListInp オペレーション

void CICSListInp::SetColumn(short count, LPUNKNOWN data)
{
	static BYTE parms[] =
		VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 count, data);
}

short CICSListInp::AddData(LPUNKNOWN data, BOOL redraw)
{
	short result;
	static BYTE parms[] =
		VTS_UNKNOWN VTS_BOOL;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		data, redraw);
	return result;
}

short CICSListInp::SetData(short index, LPUNKNOWN data, BOOL redraw)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_UNKNOWN VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, data, redraw);
	return result;
}

short CICSListInp::SetColumnData(short index, short column, LPUNKNOWN data, BOOL redraw)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_UNKNOWN VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, column, data, redraw);
	return result;
}

short CICSListInp::RemoveAll()
{
	short result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CICSListInp::RemoveData(short index)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index);
	return result;
}

void CICSListInp::ReplyTermination(long flg, LPUNKNOWN data)
{
	static BYTE parms[] =
		VTS_I4 VTS_UNKNOWN;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 flg, data);
}

short CICSListInp::AddNewLine(BOOL redraw)
{
	short result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		redraw);
	return result;
}

void CICSListInp::ReplyKana(long flg, LPCTSTR data)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 flg, data);
}

short CICSListInp::GetData(short index, LPUNKNOWN data)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, data);
	return result;
}

short CICSListInp::GetColumnData(short index, short clm, LPUNKNOWN data)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, clm, data);
	return result;
}

void CICSListInp::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CICSListInp::LineMove(short direction)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 direction);
}

void CICSListInp::PageMove(short direction)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 direction);
}

short CICSListInp::SetInputPosition(short index, short clm)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, clm);
	return result;
}

short CICSListInp::ModifyColumn(short clm, LPUNKNOWN data)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		clm, data);
	return result;
}

short CICSListInp::SetPosition(short index)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index);
	return result;
}

void CICSListInp::RedrawControl()
{
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CICSListInp::StrikeOut(short index)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 index);
}

short CICSListInp::GetTopIndex()
{
	short result;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CICSListInp::GetCount()
{
	short result;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CICSListInp::SetHorzGrid(short line, unsigned long color, short type)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I2;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 line, color, type);
}

short CICSListInp::GetIndex()
{
	short result;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CICSListInp::GetCrntRect(LPUNKNOWN pos)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pos);
}

short CICSListInp::InsertData(short index, LPUNKNOWN data, BOOL redraw)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_UNKNOWN VTS_BOOL;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, data, redraw);
	return result;
}

short CICSListInp::ShiftMove(short startindex, short direction, LPUNKNOWN newdata)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		startindex, direction, newdata);
	return result;
}

short CICSListInp::GetColumn()
{
	short result;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CICSListInp::GetRect(short index, short clm, LPUNKNOWN rct)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 index, clm, rct);
}

void CICSListInp::SetVartGrid(short cnt, LPUNKNOWN data)
{
	static BYTE parms[] =
		VTS_I2 VTS_UNKNOWN;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 cnt, data);
}

short CICSListInp::SetInputPositionEX(short index, short column)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, column);
	return result;
}

void CICSListInp::DeleteInput()
{
	InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short CICSListInp::GetCursorPos()
{
	short result;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CICSListInp::InsertString(short index, short clm, LPCTSTR string)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		index, clm, string);
	return result;
}

void CICSListInp::SetLineHeight(short height)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 height);
}

void CICSListInp::SetLineCount(short line)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 line);
}

short CICSListInp::GetCursorBytePos()
{
	short result;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CICSListInp::SetCursorPos(short pos)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pos);
}

void CICSListInp::SetCursorBytePos(short pos)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pos);
}
