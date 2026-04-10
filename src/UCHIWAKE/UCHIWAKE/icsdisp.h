#ifndef __ICSDISP_H__
#define __ICSDISP_H__

//  Microsoft Visual C++ によって自動生成された IDispatch ラップ クラス

// メモ: このファイルの内容を編集しないでください。 このクラスが再度
//  Microsoft Visual C++ で生成された場合、変更を上書きします。


// このインターフェイスによって参照されるディスパッチ インターフェイス。
class COleFont;
class CPicture;

/////////////////////////////////////////////////////////////////////////////
// CICSDisp ラップ クラス

class CICSDisp : public CWnd
{
protected:
	DECLARE_DYNCREATE(CICSDisp)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x9b6935f7, 0xcdc, 0x4909, { 0x93, 0xbc, 0x7f, 0x34, 0x91, 0x65, 0xe9, 0xd6 } };
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
	CString GetCaption();
	void SetCaption(LPCTSTR);
	OLE_COLOR GetBackColor();
	void SetBackColor(OLE_COLOR);
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	OLE_COLOR GetForeColor();
	void SetForeColor(OLE_COLOR);
	short GetCaptionPitch();
	void SetCaptionPitch(short);
	CString GetCaptionArrange();
	void SetCaptionArrange(LPCTSTR);
	CString GetCaptionEscapement();
	void SetCaptionEscapement(LPCTSTR);
	BOOL GetCaptionPitchUse();
	void SetCaptionPitchUse(BOOL);
	BOOL GetPictureType();
	void SetPictureType(BOOL);
	BOOL GetCaptionMode();
	void SetCaptionMode(BOOL);
	short GetPictureOffsetY();
	void SetPictureOffsetY(short);
	short GetPictureOffsetX();
	void SetPictureOffsetX(short);
	CPicture GetCtlpicture();
	void SetCtlpicture(LPDISPATCH);
	BOOL GetFrame();
	void SetFrame(BOOL);
	BOOL GetEnabled();
	void SetEnabled(BOOL);

// オペレーション
public:
	void ColmDisp(short colmn, LPCTSTR data);
	void ClearDisp();
	void LinClmDisp(short lin, short clm, LPCTSTR data);
	void DispRect(short xpos, short ypos, short width, short height, unsigned long cl);
	void DluDisp(LPUNKNOWN data);
	void DotDisp(LPUNKNOWN data);
	void ImgDisp(long xpos, long ypos, short width, short hight, LPUNKNOWN data, short flg);
	void CheckData();
};

#endif // __ICSDISP_H__
