#ifndef __ICSLISTINP_H__
#define __ICSLISTINP_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。


// このインターフェイスによって参照されるディスパッチ インターフェイス。
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CICSListInp ラップ クラス

class CICSListInp : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSListInp)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xeb4fb675, 0x0515, 0x4513, { 0xa3, 0x74, 0x40, 0x97, 0x3, 0xfe, 0x52, 0xb5 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// アトリビュート
public:
	OLE_COLOR GetBackColor();
	void SetBackColor(OLE_COLOR);
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	OLE_COLOR GetForeColor();
	void SetForeColor(OLE_COLOR);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	BOOL GetHorzGrid();
	void SetHorzGrid(BOOL);
	BOOL GetVertGrid();
	void SetVertGrid(BOOL);
	BOOL GetReadOnly();
	void SetReadOnly(BOOL);
	short GetHorzPerLine();
	void SetHorzPerLine(short);
	BOOL GetHorzScroll();
	void SetHorzScroll(BOOL);
	BOOL GetNoResize();
	void SetNoResize(BOOL);
	BOOL GetInverse();
	void SetInverse(BOOL);

// オペレーション
public:
	void SetColumn(short count, LPUNKNOWN data);
	short AddData(LPUNKNOWN data, BOOL redraw);
	short SetData(short index, LPUNKNOWN data, BOOL redraw);
	short SetColumnData(short index, short column, LPUNKNOWN data, BOOL redraw);
	short RemoveAll();
	short RemoveData(short index);
	void ReplyTermination(long flg, LPUNKNOWN data);
	short AddNewLine(BOOL redraw);
	void ReplyKana(long flg, LPCTSTR data);
	short GetData(short index, LPUNKNOWN data);
	short GetColumnData(short index, short clm, LPUNKNOWN data);
	void Refresh();
	void LineMove(short direction);
	void PageMove(short direction);
	short SetInputPosition(short index, short clm);
	short ModifyColumn(short clm, LPUNKNOWN data);
	short SetPosition(short index);
	void RedrawControl();
	void StrikeOut(short index);
	short GetTopIndex();
	short GetCount();
	void SetHorzGrid(short line, unsigned long color, short type);
	short GetIndex();
	void GetCrntRect(LPUNKNOWN pos);
	short InsertData(short index, LPUNKNOWN data, BOOL redraw);
	short ShiftMove(short startindex, short direction, LPUNKNOWN newdata);
	short GetColumn();
	void GetRect(short index, short clm, LPUNKNOWN rct);
	void SetVartGrid(short cnt, LPUNKNOWN data);
	short SetInputPositionEX(short index, short column);
	void DeleteInput();
	short GetCursorPos();
	short InsertString(short index, short clm, LPCTSTR string);
	void SetLineHeight(short height);
	void SetLineCount(short line);
	short GetCursorBytePos();
	void SetCursorPos(short pos);
	void SetCursorBytePos(short pos);

	void SetLineCountEX(short line);
	void EnableDelete(short mode);

};

#endif // __ICSLISTINP_H__