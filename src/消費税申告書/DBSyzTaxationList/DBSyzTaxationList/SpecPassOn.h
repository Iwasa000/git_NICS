#pragma once


// CSpecPassOn ダイアログ

class CSpecPassOn : public ICSDialog
{
	DECLARE_DYNAMIC(CSpecPassOn)

public:
	CSpecPassOn(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSpecPassOn();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SPECON };

	// 初期表示
	int InitDisp();
	// 初期値セット
	int Init( CSnHeadData **pSnHeadData, CH26SpcListData **pSpcListData, CArithEx *pArith );
	// スクロール再描画
	void sykscroll( int mpos );

private:
	CWnd				*m_pParent;			// 親ウィンドウ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
	CH26SpcListData		**m_pSpcListData;	// 特定収入計算表データ
	CArithEx			*m_pArith;			// 倍長演算クラス
	int					m_initfg;			// 初期化フラグ
	int					m_wPos;
	int					m_cmPos;
	int					m_ScW;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
