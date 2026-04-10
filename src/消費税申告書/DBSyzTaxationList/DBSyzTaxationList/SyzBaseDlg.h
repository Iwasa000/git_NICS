#pragma once
#include "icsdialog.h"
#include "icsdiagctrl.h"

//-----------------------------------------------------------------------------
// 消費税 本表・付表ダイアログ 基本クラス
//-----------------------------------------------------------------------------

class CH26TaxListData;
class CSnHeadData;
class CSyzUtil;
class CShzCalc;//'14.08.06

const	int	R_TO_H_ADNEN	=	0x30;	// 令和を平成に変換する為に加算する年

class CSyzBaseDlg :	public ICSDialog
{
public:
	CSyzBaseDlg(CWnd* pParent = NULL){ TRACE(_T("CSyzBaseDlg::CSyzBaseDlg()\n")); };	// 標準コンストラクタ
	CSyzBaseDlg(UINT id, CWnd* pParent = NULL);
	~CSyzBaseDlg(void);
	virtual BOOL OnInitDialog();

	// スクロール再描画
	void SetRedrawScroll( int mpos );
	// スクロール再描画(index指定)
	void SetRedrawScrollToIdx( int nowpos );
	// スクロール再描画(index指定)
	void SetRedrawScrollToIdx8( int nowpos );
	// 初期情報のセット
	int InitInfo( CH26TaxListData **pTaxListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub, CArithEx *pArith , CShzCalc **Scalc );// 集計クラス追加'14.08.06
	//int InitInfo( CH26TaxListData *pTaxListData, CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, CArithEx *pArith , CShzCalc *Scalc );// 集計クラス追加'14.08.06

	// 画面表示
	virtual void DispList( int mode );

	//フォーカスセット'14.09.01
	virtual void SetItemPosition();

//2016.02.23 INSERT START
	int Make_StaticFont();
	void Static_Update( unsigned char Sn_Sign4 ); //CSnHeadDataのSn_Sign4
//2016.02.23 INSERT END
//2016.06.22 INSERT START
	void Switch_KamokuEdit( unsigned short id, short index ); //科目名称の入力可⇔入力不可を切り替える
	virtual void SetFocusRange(){return;};
//2016.06.22 INSERT END

	// 平成継続出力？
	bool IsJcContinuePrint();
	// 30年プラス印字？
	bool Is30PlusPrint();

protected:
	CWnd				*m_pParent;			// 親ウィンドウ14.08.05
	CShzCalc			**m_Scalc;			// 集計クラス'14.08.06
//	CShzCalc			*m_Scalc;			// 集計クラス'14.08.06
	CH26TaxListData		**m_pTaxListData;	// 課税取引金額計算表データ（会社切替エラー対応の為ポインタのポインタに変更）
//	CH26TaxListData		*m_pTaxListData;	// 課税取引金額計算表データ
	CSnHeadData			**m_pSnHeadData;	// 消費税ヘッダ情報（会社切替エラー対応の為ポインタのポインタに変更）
//	CSnHeadData			*m_pSnHeadData;		// 消費税ヘッダ情報
	CArithEx			*m_pArith;			// 倍長演算
	CDBNpSub			*m_pZmSub;			// 公益クラス

	int					m_wPos;				// 
	int					m_cmPos;			// 
	int					m_ScW;				// 
	BOOL				m_bInitFlg;

	CShinCom			m_ShinCom;			// 消費税申告書 共通情報


	CIcsdiagctrl		*m_pBottomDiag;		// 底合わせするダイアグコントロール
	int					m_BottomIdx;		// 底合わせするダイアグ内のインデックス

//2016.02.23 INSERT START
//	CBrush	m_hBrushLock;
	CFont	m_font;
	ICSStatic m_InputLock;
//2016.02.23 INSERT END

	int					m_JcContinuePrint;	// 和暦継続出力？

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
