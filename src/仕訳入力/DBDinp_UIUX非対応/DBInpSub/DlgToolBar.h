// DlgToolBar.h : ヘッダー ファイル
//
#ifndef INCLUDE_DLGTOOLBAR_H
#define INCLUDE_DLGTOOLBAR_H

/////////////////////////////////////////////////////////////////////////////
// CDlgToolBar ウィンドウ

class CDlgToolBar : public CToolBar
{
// コンストラクション
public:
	CDlgToolBar();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDlgToolBar)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDlgToolBar();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDlgToolBar)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	LRESULT OnIdleUpdateCmdUI(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


#endif // INCLUDE_DLGTOOLBAR_H