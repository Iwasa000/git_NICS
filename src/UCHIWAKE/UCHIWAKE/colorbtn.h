/*- colorbtn.h -*/

#ifndef ___CCOLORBTN_H_
#define ___CCOLORBTN_H_

class CColorButton : public CButton
{
	DECLARE_DYNAMIC(CColorButton)

private:
	COLORREF	coltext;		// キャプションカラー(通常時)
	COLORREF	colbackground;	// 背景色
	COLORREF	coldisabled;	// ボタンが無効時のキャプションカラー

public:
	// コンストラクタ・デストラクタ
	CColorButton(); 			// コンストラクタ
	virtual ~CColorButton();	// デストラクタ

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// メンバ関数
	BOOL AutoLoad(const UINT nID, CWnd* pParent);				// ボタンをCColorButtonにアタッチ
	void SetTextColor(const COLORREF color){coltext = color;}		// キャプションカラー設定
	void SetBackGroundColor(const COLORREF color){colbackground = color;}	// 背景色設定
	void SetDisabledColor(const COLORREF color){coldisabled = color;}	// ボタン無効時のキャプションカラー設定
	COLORREF GetTextColor() { return coltext; }				// キャプションカラー取得
	COLORREF GetBackGroundColor() { return colbackground; }		// 背景色取得
	COLORREF GetDisabledColor() { return coldisabled; }			// ボタン無効時のキャプションカラー取得

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);				// ボタンのオーナー描画
	void DrawBkGround(CDC *pDC, CRect rect,const COLORREF color);			// 背景色描画
	void DrawCaption(CDC *pDC, CRect rect,const COLORREF color,const CString& str);	// キャプション描画

};
#endif
