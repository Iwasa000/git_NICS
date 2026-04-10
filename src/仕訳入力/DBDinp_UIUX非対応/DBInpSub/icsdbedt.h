#ifndef __ICSDBEDT_H__
#define __ICSDBEDT_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。


// このインターフェイスによって参照されるディスパッチ インターフェイス。
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CICSDBEDT ラップ クラス

class CICSDBEDT : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSDBEDT)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xed3ad240, 0x3068, 0x44e5, { 0xa8, 0x07, 0x91, 0xd6, 0xb6, 0x57, 0x14, 0x8f } };
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
	OLE_COLOR GetForeColor();
	void SetForeColor(OLE_COLOR);
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	unsigned long GetFocusColor();
	void SetFocusColor(unsigned long);
	unsigned long GetInverseColor();
	void SetInverseColor(unsigned long);
	BOOL GetInverse();
	void SetInverse(BOOL);
	BOOL GetMultiLine();
	void SetMultiLine(BOOL);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	short GetLayout();
	void SetLayout(short);
	BOOL GetNumeric();
	void SetNumeric(BOOL);
	BOOL GetAlphabet();
	void SetAlphabet(BOOL);
	BOOL GetKana();
	void SetKana(BOOL);
	BOOL GetZenkaku();
	void SetZenkaku(BOOL);
	BOOL GetAscii();
	void SetAscii(BOOL);
	BOOL GetVector();
	void SetVector(BOOL);
	BOOL GetFormat();
	void SetFormat(BOOL);
	CString GetMinval();
	void SetMinval(LPCTSTR);
	CString GetMaxval();
	void SetMaxval(LPCTSTR);
	long GetMaxLen();
	void SetMaxLen(long);
	BOOL GetCmove();
	void SetCmove(BOOL);
	BOOL GetAsynch();
	void SetAsynch(BOOL);
	BOOL GetRegColor();
	void SetRegColor(BOOL);

// オペレーション
public:
	long GetData(VARIANT* data, long type, short bcdlen);
	long SetData(VARIANT* data, long type, short bcdlen);
	void SetCaretPosition(short pos);
	short GetCaretPosition();
	long InsertData(VARIANT* data, long type, short bcdlen, short pos);
	void SetDisplayString(LPCTSTR string);
	void SetCalendar(BOOL enable, long pwnd, short dtype, short dformat);
	void SetCalculator(BOOL enable, long pwnd);
	void SetToolTip(LPCTSTR string);
	void SetKanaTerm(BOOL sw, short len, long type);
	void ImeEndMode(short mode);
	void SetFrame(short mode, unsigned long color);

	void SetVscroll(short mode);
	void AllowReturn(short mode);
	void EnableDelete(short mode);

	void SetZei(short mode);
	void SetZeiritsu(double ritsu);
	long GetSelect();

	void SetSelect(long start, long end);
	void SetKaigyo(long lb);
	void EnableHome(short mode);
};

#endif // __ICSDBEDT_H__
