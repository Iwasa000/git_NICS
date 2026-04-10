#ifndef __ICSKNJEDIT_H__
#define __ICSKNJEDIT_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。


// このインターフェイスによって参照されるディスパッチ インターフェイス。
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CICSKNJEdit ラップ クラス

class CICSKNJEdit : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSKNJEdit)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xf3eba915, 0x74d9, 0x43b7, { 0xb9, 0x9d, 0xb1, 0x8b, 0x78, 0x6f, 0x72, 0x76 } };
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
	CString GetCaption();
	void SetCaption(LPCTSTR);
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	OLE_COLOR GetForeColor();
	void SetForeColor(OLE_COLOR);
	short GetMaxLength();
	void SetMaxLength(short);
	CString GetAnk();
	void SetAnk(LPCTSTR);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	BOOL GetMaxReturn();
	void SetMaxReturn(BOOL);
	short GetBorder();
	void SetBorder(short);
	BOOL GetNextCtrl();
	void SetNextCtrl(BOOL);
	BOOL GetAnkMix();
	void SetAnkMix(BOOL);

// オペレーション
public:
	void CheckData();
	void NextControl(BOOL flg);
	CString GetReadString();
	void KanaCancelMode(short mode);
	void ZenHanMix(short mode);
	void SetCursorPos(short pos);
	short GetCursorPos();
	void SetCursorBytePos(short pos);
	short GetCursorBytePos();
};

#endif // __ICSKNJEDIT_H__
