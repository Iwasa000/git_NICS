// ExEdit.h : ヘッダー ファイル
/////////////////////////////////////////////////////////////////////////////
//				<<<<<クラス使用方法>>>>>>
//
//icsgenl8(d).lib  imm32.libが必要。
//
//①コントロールのメンバ変数の宣言を子クラスに変更（CEdit testedit;	→　CExEdit testedit;）
//②コントロールのプロパティの「Multiline」「Want Return」をTRUEにする。必要ならば「Vertical Scroll」もTRUEにする
//③Make_Editを使い初期化する
//		void	Make_Edit(int gyo,int line_byte,CString str,int kind = ZEN_ST);
//				
//				引数	gyo				最大行数
//						line_byte		一行あたりの最大文字バイト数
//						str				一番初めctr+zで表示される文字（基本デフォルトデータと同じにしておくとよい）
//						kind			入力フォーカスが入った時に、入力モードの切替フラグ(ZEN_ST == 全角　ANK_ST == ANK)
//				
/////////////////////////////////////////////////////////////////////
#ifndef __CExEdit_
#define __CExEdit_

#include "imm.h"

#define		ZEN_ST		0
#define		ANK_ST		1

#define		EX_EDIT_MAX	3840

class CExEdit : public CEdit
{
// コンストラクション
public:
	CExEdit();

// アトリビュート
public:

private:

// オペレーション
public:

// オーバーライド
	public:
	protected:
//	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
public:
	int Gyo_Max;			//最大行数
	int LineByte_Max;		//一行当たりの最大バイト数
	CString str_z_edit;		//ctr + z 用

	HIMC	hIMC;

	int fst_kind;
// インプリメンテーション
public:
	void Make_Edit(int gyo,int line_byte,CString str,int kind = ZEN_ST);

	void DrawCaption(CDC *pDC, CRect rect,const COLORREF color,const CString& str);

	virtual ~CExEdit();

	int	GetLineOfs();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG()
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnSetfocus();
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void Killfocus();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif
/////////////////////////////////////////////////////////////////////////////