#ifndef __ICSHEDIT_H__
#define __ICSHEDIT_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。


// このインターフェイスによって参照されるディスパッチ インターフェイス。
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CICSHedit ラップ クラス

class CICSHedit : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSHedit)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xaa40e628, 0x8b05, 0x465c, { 0x96, 0x3a, 0x5c, 0xf9, 0x4f, 0x9c, 0xa1, 0x61 } };
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
	CString GetKind();
	void SetKind(LPCTSTR);
	short GetMax();
	void SetMax(short);
	CString GetAlign();
	void SetAlign(LPCTSTR);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	short GetBorder();
	void SetBorder(short);
	BOOL GetNextCtrl();
	void SetNextCtrl(BOOL);

// オペレーション
public:
	void SetFormat(LPCTSTR format);
	void SetData(LPCTSTR data);
	void GetData(LPUNKNOWN data, short* length);
	void NextControl(BOOL flg);
	void DataClear();
};

#endif // __ICSHEDIT_H__
