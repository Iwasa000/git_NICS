#pragma once
#include "icsdialog.h"
#include "icsdiagctrl.h"

//-----------------------------------------------------------------------------
// 消費税 本表・付表ダイアログ 基本クラス
//-----------------------------------------------------------------------------

//== 定数 ==
const	COLORREF	SYZBASE_WHITE	=	RGB(255, 255, 255);		// 白色
const	COLORREF	SYZBASE_RED		=	RGB(255, 0, 0);			// 赤色

class CSyzBaseDlg :	public ICSDialog
{
public:
	CSyzBaseDlg(CWnd* pParent = NULL){ TRACE(_T("CSyzBaseDlg::CSyzBaseDlg()\n")); };	// 標準コンストラクタ
	CSyzBaseDlg(UINT id, CWnd* pParent = NULL);
	~CSyzBaseDlg(void);
	virtual BOOL OnInitDialog();

protected:
	CWnd			*m_pParent;		// 親ウィンドウ
	int				m_wPos;			// 
	int				m_cmPos;		// 
	int				m_ScW;			// 
	BOOL			m_bInitFlg;
	BOOL			m_IsStartEnd;	// 消去処理中？


	CShinCom		m_ShinCom;		// 消費税申告書 共通情報


	CIcsdiagctrl	*m_pBottomDiag;	// 底合わせするダイアグコントロール
	int				m_BottomIdx;	// 底合わせするダイアグ内のインデックス

//2016.02.23 INSERT START
//	CBrush	m_hBrushLock;
	CFont	m_font;
	ICSStatic m_InputLock;
//2016.02.23 INSERT END

//2016.06.22 INSERT START
	char			FZERO[MONY_BUF_SIZE];		// ０円表示
//2016.06.22 INSERT END

protected:
	// スクロール再描画
	void SetRedrawScroll( int mpos );

public:
	// 終了処理スタート
	void SetStartEnd( BOOL isStartEnd ){ m_IsStartEnd = isStartEnd; };

//2016.02.23 INSERT START
	int Make_StaticFont();
	void Static_Update( unsigned char Sn_Sign4 ); //CSnHeadDataのSn_Sign4
//2016.02.23 INSERT END

	// 再描画
	void RedrawInputLock();
	// 入力ロックコントロール色設定
	void SetInputLockColor();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);
};
