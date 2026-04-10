#pragma once

// Microsoft Visual C++ で作成された IDispatch ラッパー クラスを生成しました。

// メモ: このファイルの内容を変更しないでください。このクラスが
//  Microsoft Visual C++ で再生成された場合、変更は上書きされます。

/////////////////////////////////////////////////////////////////////////////
// CIcsdiagctrl ラッパー クラス

// 入力ロック関係マクロ
//#define GET_BK_COLOR(color,snd, edit, flg )\
//	color = edit ? BC_CREAM : flg ? BC_WHITE : ( snd.Sn_Sign4&0x01 ) ? BC_GRAY : BC_WHITE
#define GET_BK_COLOR(color, Sn_Sign4, edit, flg)\
	color = edit ? BC_CREAM : flg ? BC_WHITE : ( Sn_Sign4&0x01 ) ? BC_GRAY : BC_WHITE

//#define ATRIB_MOD( diag, index, editable, snd, connect )\
//	DIAGRAM_ATTRIBUTE	DA;\
//	diag_getattr( diag, index, &DA, CIcsdiagctrl1 );\
//	GET_BK_COLOR( DA.attr_bcolor, snd, editable, ( !(snd.Sn_Sign4&0x80) && !(connect) ) );\
//	diag_setattr( diag, index, &DA, FALSE, CIcsdiagctrl1 );\
//	diag_modify( diag, index, editable ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY , CIcsdiagctrl1 );
#define ATRIB_MOD( diag, index, editable, Sn_Sign4, connect )\
	DIAGRAM_ATTRIBUTE	DA;\
	diag_getattr( diag, index, &DA, CIcsdiagctrl );\
	GET_BK_COLOR( DA.attr_bcolor, Sn_Sign4, editable, ( !(Sn_Sign4&0x80) && !(connect) ) );\
	diag_setattr( diag, index, &DA, FALSE, CIcsdiagctrl );\
	diag_modify( diag, index, editable ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY , CIcsdiagctrl );


class CIcsdiagctrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CIcsdiagctrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x8A8B65FB, 0xAFE2, 0x4D25, { 0xA7, 0xC, 0x69, 0x76, 0x4E, 0xD, 0xC3, 0x71 } };
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

// _DICSDiag8

// Functions
//

//--> '14.02.18 INS START
	short ForceSetPosition(short index )
	{
		DIAGRAM_ATTRIBUTE DA;

		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index);
		return result;
	}
//<-- '14.02.18 INS START

	short SetPosition(short index)
	{
//--> '14.01.29 INS START
		DIAGRAM_ATTRIBUTE DA;
		GetAttribute( index, (LPUNKNOWN)&DA );
		if( DA.attr_bcolor == RGB( 255, 255, 255 ) ) return 0;
		if( DA.attr_bcolor == RGB( 192, 192, 192 ) ) return 0;
//<-- '14.01.29 INS START

		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index);
		return result;
	}
	short SetData(short index, LPUNKNOWN data)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_UNKNOWN ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index, data);
		return result;
	}
	short GetData(short index, LPUNKNOWN data)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_UNKNOWN ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index, data);
		return result;
	}
	void AllClear()
	{
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DataClear(short index, signed char redraw)
	{
		static BYTE parms[] = VTS_I2 VTS_I1 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, redraw);
	}
	void Refresh()
	{
		InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DeleteInput()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short GetPosition()
	{
		short result;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	short GetRect(short index, LPUNKNOWN rect)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_UNKNOWN ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index, rect);
		return result;
	}
	void ModifyItem(short index, long flg)
	{
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, flg);
	}
	short GetAttribute(short index, LPUNKNOWN data)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_UNKNOWN ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index, data);
		return result;
	}
	short SetAttribute(short index, LPUNKNOWN data, signed char redraw)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_UNKNOWN VTS_I1 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index, data, redraw);
		return result;
	}
	short GetCursorPos(short index)
	{
		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index);
		return result;
	}
	void SetCursorPos(short index, short cpos)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, cpos);
	}
	void OffControl(signed char flg)
	{
		static BYTE parms[] = VTS_I1 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, flg);
	}
	void ComboDropDown(short index, signed char flg)
	{
		static BYTE parms[] = VTS_I2 VTS_I1 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, flg);
	}
	void SetNextPosition()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetPrevPosition()
	{
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString GetReadString(short index)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, index);
		return result;
	}
	short GetCursorBytePos(short index)
	{
		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index);
		return result;
	}
	void SetCursorBytePos(short index, short pos)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, pos);
	}
	void ScrollPos(long code, short pos)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, code, pos);
	}
	long GetDataInputPosition(short vect)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, vect);
		return result;
	}
	short ChangeFontSize(short index, short pt)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index, pt);
		return result;
	}
	void ComboxFontMode(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void CursorEndMode(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void EnableDelete(short mode)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, mode);
	}
	void ChangeFontBold(short index, short weight)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, index, weight);
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
CString GetEmphasis()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}
void SetEmphasis(CString propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}
CString GetRectInfo()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}
void SetRectInfo(CString propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
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
CString GetRectInfo1()
{
	CString result;
	GetProperty(0x17, VT_BSTR, (void*)&result);
	return result;
}
void SetRectInfo1(CString propVal)
{
	SetProperty(0x17, VT_BSTR, propVal);
}
CString GetRectInfo2()
{
	CString result;
	GetProperty(0x18, VT_BSTR, (void*)&result);
	return result;
}
void SetRectInfo2(CString propVal)
{
	SetProperty(0x18, VT_BSTR, propVal);
}
CString GetRectInfo3()
{
	CString result;
	GetProperty(0x19, VT_BSTR, (void*)&result);
	return result;
}
void SetRectInfo3(CString propVal)
{
	SetProperty(0x19, VT_BSTR, propVal);
}
signed char GetMemDisp()
{
	signed char result;
	GetProperty(0x1a, VT_I1, (void*)&result);
	return result;
}
void SetMemDisp(signed char propVal)
{
	SetProperty(0x1a, VT_I1, propVal);
}
signed char GetScroll()
{
	signed char result;
	GetProperty(0x1e, VT_I1, (void*)&result);
	return result;
}
void SetScroll(signed char propVal)
{
	SetProperty(0x1e, VT_I1, propVal);
}
short GetScOrgX()
{
	short result;
	GetProperty(0x1f, VT_I2, (void*)&result);
	return result;
}
void SetScOrgX(short propVal)
{
	SetProperty(0x1f, VT_I2, propVal);
}
short GetScOrgY()
{
	short result;
	GetProperty(0x20, VT_I2, (void*)&result);
	return result;
}
void SetScOrgY(short propVal)
{
	SetProperty(0x20, VT_I2, propVal);
}
signed char GetScrollPage()
{
	signed char result;
	GetProperty(0x22, VT_I1, (void*)&result);
	return result;
}
void SetScrollPage(signed char propVal)
{
	SetProperty(0x22, VT_I1, propVal);
}
short GetScPageCnt()
{
	short result;
	GetProperty(0x23, VT_I2, (void*)&result);
	return result;
}
void SetScPageCnt(short propVal)
{
	SetProperty(0x23, VT_I2, propVal);
}
short GetScPageOfs()
{
	short result;
	GetProperty(0x24, VT_I2, (void*)&result);
	return result;
}
void SetScPageOfs(short propVal)
{
	SetProperty(0x24, VT_I2, propVal);
}


};
