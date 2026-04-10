#ifndef __ICSINPUT_H__
#define __ICSINPUT_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。


// このインターフェイスによって参照されるディスパッチ インターフェイス。
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CICSInput ラップ クラス

class CICSInput : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSInput)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xc0ba7763, 0x5c05, 0x424d, { 0xa1, 0x6d, 0x4c, 0x7d, 0x58, 0x20, 0xbb, 0x20 } };
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
	CString GetKind();
	void SetKind(LPCTSTR);
	OLE_COLOR GetBackColor();
	void SetBackColor(OLE_COLOR);
	CString GetCaption();
	void SetCaption(LPCTSTR);
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	OLE_COLOR GetForeColor();
	void SetForeColor(OLE_COLOR);
	short GetMax();
	void SetMax(short);
	BOOL GetMinus();
	void SetMinus(BOOL);
	BOOL GetKeta();
	void SetKeta(BOOL);
	CString GetForm();
	void SetForm(LPCTSTR);
	BOOL GetEda();
	void SetEda(BOOL);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	short GetBorder();
	void SetBorder(short);
	BOOL GetNextCtrl();
	void SetNextCtrl(BOOL);
	BOOL GetAnkMix();
	void SetAnkMix(BOOL);

// オペレーション
public:
	void ModeSet(short mode);
	void SetData(LPUNKNOWN data);
	void ReplyData(short flg, LPUNKNOWN data);
	short GetData(LPUNKNOWN data);
	short GetCursorPos();
	void TekiInsert(LPCTSTR data);
	short SetCursorPos(short pos);
	void NextControl(BOOL flg);
	short EntryKey(short key);
	short RemoveKey(short key);
	void ZenHanMix(short mode);
	void SetCursorBytePos(short pos);
	short GetCursorBytePos();
	long GetKanaTermChar();
	void TerminationMode(short mode);
};

#endif // __ICSINPUT_H__
