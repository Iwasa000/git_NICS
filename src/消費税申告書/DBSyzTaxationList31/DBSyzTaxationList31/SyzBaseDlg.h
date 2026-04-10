#pragma once
#include "icsdialog.h"
#include "icsdiagctrl.h"

class CH31TaxListData;
class CSnHeadData;
class CSyzUtil;
class CShzTax31Calc;
class CSyzBaseDlg : public ICSDialog
{
public:
	CSyzBaseDlg ( CWnd* pParent = NULL );
	CSyzBaseDlg ( UINT id, CWnd* pParent = NULL );
	~CSyzBaseDlg(void);

	int Make_StaticFont();
	void Static_Update( unsigned char Sn_Sign4 );


	// スクロール再描画
	void SetRedrawScroll( int mpos );
	void SetRedrawScrollToIdx ( int nowpos, int DiagId );	// 190709 kasai

	// 初期情報のセット
	int InitInfo( CH31TaxListData **pTaxListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub, CArithEx *pArith , CShzTax31Calc **Scalc );

	virtual BOOL OnInitDialog();

	// 画面表示
	virtual void DispList ( int mode );

	// フォーカスセット
	virtual void SetItemPosition();
	void Switch_KamokuEdit(unsigned short id, short index); //科目名称の入力可⇔入力不可を切り替える
	virtual void SetFocusRange() { return; };
	virtual void DiagReSetPosition() { return; };		// 修正No.168690 add

protected:
	CWnd			*m_pParent;			// 親ウィンドウ
	CShzTax31Calc	**m_Scalc;			// 集計クラス
	CH31TaxListData	**m_pTaxListData;	// 課税取引金額計算表データ
	
	CShinCom		m_ShinCom;			// 消費税申告書 共通情報

	CSnHeadData		**m_pSnHeadData;	// 消費税ヘッダ情報
	CArithEx		*m_pArith;			// 倍長演算
	CDBNpSub		*m_pZmsub;	

	CIcsdiagctrl	*m_pBottomDiag;		//　底合わせするダイアグコントロール
	int				m_BottomIdx;		// 底合わせするダイアグ内のインデックス

	CFont			m_Font;
	ICSStatic		m_InputLock;

//スクロール関連
	int					m_wPos;				// 
	int					m_cmPos;			// 
	int					m_ScW;				// 
	BOOL				m_bInitFlg;
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