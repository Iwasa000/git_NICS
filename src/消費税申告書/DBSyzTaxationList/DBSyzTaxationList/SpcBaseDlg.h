#pragma once
#include "icsdialog.h"
#include "icsdiagctrl.h"

//-----------------------------------------------------------------------------
// 消費税 特定収入ダイアログ 基本クラス
//-----------------------------------------------------------------------------

class	CH26SpcListData;
class CSpcBaseDlg :	public ICSDialog
{
public:
	CSpcBaseDlg(CWnd* pParent = NULL){ TRACE(_T("CSpcBaseDlg::CSpcBaseDlg()\n")); };	// 標準コンストラクタ
	CSpcBaseDlg(UINT id, CWnd* pParent = NULL);
	~CSpcBaseDlg(void);

	//------>yoshida150310
	// スクロール再描画
	void SetRedrawScroll( int mpos );

	// スクロール再描画(index指定)
	void SetRedrawScrollToIdx( int nowpos );
	//<-------------------


	// 初期情報のセット
	int InitInfo( CH26SpcListData **pSpcListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub, CArithEx *pArith );
	// 入力項目　色変え
	void Color_Change( unsigned short id, short index, int sign );

//2016.02.23 INSERT START
	int Make_StaticFont();
	void Static_Update( unsigned char Sn_Sign4 ); //CSnHeadDataのSn_Sign4
//2016.02.23 INSERT END

protected:
	CWnd				*m_pParent;			// 親ウィンドウ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報
	CH26SpcListData		**m_pSpcListData;	// 特定収入計算表データ
	CArithEx			*m_pArith;			// 倍長演算
	CDBNpSub			*m_pZmSub;			// 公益クラス
	short				m_curwnd;			// 
	int					m_FrSw;				// 
	int					m_svscll;			// 
	int					m_pagesiz;			// 
	int					m_sclmax;			// 
	int					m_initfg;			// 
	float				scpos,				// 
						smax,				// 
						svmax;				// 

//2016.02.23 INSERT START
//	CBrush	m_hBrushLock;
	CFont	m_font;
	ICSStatic m_InputLock;
//2016.02.23 INSERT END

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
};