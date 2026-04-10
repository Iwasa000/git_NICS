#ifndef __ICSSEL_H__
#define __ICSSEL_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。


// このインターフェイスによって参照されるディスパッチ インターフェイス。
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CICSSel ラップ クラス

class CICSSel : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSSel)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xe63ad959, 0x549e, 0x4492, { 0x8e, 0xe3, 0x64, 0xca, 0x16, 0x44, 0x38, 0x0f } };
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
	short GetGyou();
	void SetGyou(short);
	short GetRetsu();
	void SetRetsu(short);
	BOOL GetNumber();
	void SetNumber(BOOL);
	short GetButtom();
	void SetButtom(short);

// オペレーション
public:
	void SetData(long data);
	void SetLineColor(unsigned long cl);
};

#endif // __ICSSEL_H_