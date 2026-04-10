// ScanKey.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CScanKey ダイアログ

//#include "ICSDialog.h"
//{{AFX_INCLUDES()
#include "icsdbedt.h"
#include "icssel.h"
//}}AFX_INCLUDES
#include "HusenCombo.h"

//#ifdef _ICSPEN_	// 01.20 /01
//#include "ICSPEN.h"
//#endif

#include "../DBDinp/ocxdef.h"



#define SCN_ORDER_PN	0	//表示順
#define SCN_DUSER_PN	1	//データユーザ
#define SCN_STYPE_PN	2	//検索タイプ

#define SCN_SEQ1_PN		3	//番号（開始）
#define SCN_SEQ2_PN		4	//番号（終了）
#define SCN_DATE1_PN	5	//日付（開始）
#define SCN_DATE2_PN	6	//日付（終了）
#define SCN_DENP1_PN	7	//伝票番号（開始）
#define SCN_DENP2_PN	8	//伝票番号（終了）
#define SCN_BMON1_PN	9	//部門（開始）
#define SCN_BMON2_PN	10	//部門（終了）
#define SCN_CBMN1_PN	11	//部門（開始）
#define SCN_CBMN2_PN	12	//部門（終了）
#define SCN_KOJI1_PN	13	//工事（開始）
#define SCN_KOJI2_PN	14	//工事（終了）
#define SCN_CKJI1_PN	15	//工事（開始）
#define SCN_CKJI2_PN	16	//工事（終了）

#define SCN_DEBT_PN		17	//借方
#define SCN_DBR1_PN		18	//借方枝番（開始）
#define SCN_DBR2_PN		19	//借方枝番（終了）
#define SCN_CRED_PN		20	//貸方
#define SCN_CBR1_PN		21	//貸方枝番（開始）
#define SCN_CBR2_PN		22	//貸方枝番（終了）
#define SCN_DBCR_PN		23	//貸借
#define SCN_DCBR1_PN	24	//貸借枝番（開始）
#define SCN_DCBR2_PN	25	//貸借枝番（終了）
#define SCN_VAL1_PN		26	//金額（開始）
#define SCN_VAL2_PN		27	//金額（終了）
#define SCN_ZEI1_PN		28	//税額（開始）
#define SCN_ZEI2_PN		29	//税額（終了）
#define SCN_SYZSW_PN	30	//消費税仕訳区分
#define SCN_ZRITU_PN	31	//税率
#define SCN_SKBN_PN		32	//消費税区分
#define SCN_URISIRE_PN	33	//業種・売り仕入区分
#define SCN_TKY_PN		34	//摘要
#define SCN_SNUM1_PN	35	//証憑番号
#define SCN_SNUM2_PN	36	//証憑番号
#define SCN_TDAT1_PN	37	//手形期日
#define SCN_TDAT2_PN	38	//手形期日
#define SCN_HUSEN_PN	39	//付箋
#define SCN_NOINP_PN	40	//入力漏れ


#define SCN_CHKDBM_PN	41	//借方部門チェックボタン
#define SCN_CHKCBM_PN	42	//貸方部門チェックボタン
#define SCN_CHKDKJ_PN	43	//借方工事チェックボタン
#define SCN_CHKCKJ_PN	44	//借方工事チェックボタン
#define SCN_CHKDBR_PN	45	//借方チェックボタン
#define SCN_CHKCBR_PN	46	//貸方チェックボタン

#define SNOT_SEQ_PN			47
#define SNOT_DATE_PN		48
#define SNOT_DENP_PN		49
#define SNOT_DBMN_PN		50
#define SNOT_CBMN_PN		51
#define SNOT_DKJI_PN		52
#define SNOT_CKJI_PN		53
#define SNOT_DEB_PN			54
#define SNOT_DBRN_PN		55
#define SNOT_CRE_PN			56
#define SNOT_CBRN_PN		57
#define SNOT_DBCR_PN		58
#define SNOT_DCBRN_PN		59
#define SNOT_VAL_PN			60
#define SNOT_ZEI_PN			61
#define SNOT_SYZSW_PN		62
#define SNOT_ZRITU_PN		63
#define SNOT_SKBN_PN		64
#define SNOT_URISIRE_PN		65
#define SNOT_SNUM_PN		66
#define SNOT_TGDATE_PN		67
#define SNOT_HUSEN_PN		68




#define SCN_IDOK		(SNOT_HUSEN_PN+1)	// ＯＫボタン
#define SCN_IDCANCEL	(SNOT_HUSEN_PN+2)	// キャンセルボタン
#define SCN_BRNINP_PN	(SNOT_HUSEN_PN+3)	// 枝番入力
//#* 05.16 /01
#define SCN_TKYONLY		(SNOT_HUSEN_PN+4)	// 摘要のみ修正
//#* 06.19 /01
#define SCN_SELTKY_PN	(SNOT_HUSEN_PN+5)	// イメージ摘要切り替えコンボ
#define SCN_IMGEDT_PN	(SNOT_HUSEN_PN+6)	// イメージ摘要と表示するエディットボックス
//#* 08.28 /02
//#define SCN_BMNINP_PN	(SCN_TDAT2_PN+7)	// 部門入力

//#define TSM_NORMAL		0	// 通常摘要検索
//#define TSM_IMG			1	// イメージ摘要のみ検索
//#define TSM_STR			2	// 文字摘要のみ検索
//#define TSM_STRNONE		3	// 文字未入力摘要の検索

//入力データ
struct _SKEY_LINE {
	struct _ItemData	_XXDTA2[ SCN_HUSEN_PN + 1 ];
#define SCN_ORDER_DT	_XXDTA2[ SCN_ORDER_PN ]	//表示順
#define SCN_DUSER_DT	_XXDTA2[ SCN_DUSER_PN ]	//データユーザ
#define SCN_STYPE_DT	_XXDTA2[ SCN_STYPE_PN ]	//検索タイプ

#define SCN_SEQ1_DT		_XXDTA2[ SCN_SEQ1_PN ]	//番号（開始）
#define SCN_SEQ2_DT		_XXDTA2[ SCN_SEQ2_PN ]	//番号（終了）
#define SCN_DATE1_DT	_XXDTA2[ SCN_DATE1_PN ]	//日付（開始）
#define SCN_DATE2_DT	_XXDTA2[ SCN_DATE2_PN ]	//日付（終了）
#define SCN_DENP1_DT	_XXDTA2[ SCN_DENP1_PN ]	//伝票番号（開始）
#define SCN_DENP2_DT	_XXDTA2[ SCN_DENP2_PN ]	//伝票番号（終了）
#define SCN_BMON1_DT	_XXDTA2[ SCN_BMON1_PN ]	//借方部門（開始）
#define SCN_BMON2_DT	_XXDTA2[ SCN_BMON2_PN ]	//　　部門（終了）
#define SCN_CBMN1_DT	_XXDTA2[ SCN_CBMN1_PN ]	//貸方部門（開始）
#define SCN_CBMN2_DT	_XXDTA2[ SCN_CBMN2_PN ]	//　　部門（終了）

#define SCN_KOJI1_DT	_XXDTA2[ SCN_KOJI1_PN ]	//借方工事（開始）
#define SCN_KOJI2_DT	_XXDTA2[ SCN_KOJI2_PN ]	//　　工事（終了）
#define SCN_CKJI1_DT	_XXDTA2[ SCN_CKJI1_PN ]	//貸方工事（開始）
#define SCN_CKJI2_DT	_XXDTA2[ SCN_CKJI2_PN ]	//　　工事（終了）

#define SCN_DEBT_DT		_XXDTA2[ SCN_DEBT_PN ]	//借方
#define SCN_DBR1_DT		_XXDTA2[ SCN_DBR1_PN ]	//借方枝番（開始）
#define SCN_DBR2_DT		_XXDTA2[ SCN_DBR2_PN ]	//借方枝番（終了）
#define SCN_CRED_DT		_XXDTA2[ SCN_CRED_PN ]	//貸方
#define SCN_CBR1_DT		_XXDTA2[ SCN_CBR1_PN ]	//貸方枝番（開始）
#define SCN_CBR2_DT		_XXDTA2[ SCN_CBR2_PN ]	//貸方枝番（終了）
#define SCN_DBCR_DT		_XXDTA2[ SCN_DBCR_PN ]	//貸借
#define SCN_DCBR1_DT	_XXDTA2[ SCN_DCBR1_PN ]	//貸借枝番（開始）
#define SCN_DCBR2_DT	_XXDTA2[ SCN_DCBR2_PN ]	//貸借枝番（終了）
#define SCN_VAL1_DT		_XXDTA2[ SCN_VAL1_PN ]	//金額（開始）
#define SCN_VAL2_DT		_XXDTA2[ SCN_VAL2_PN ]	//金額（終了）
#define SCN_ZEI1_DT		_XXDTA2[ SCN_ZEI1_PN ]	//税額（開始）
#define SCN_ZEI2_DT		_XXDTA2[ SCN_ZEI2_PN ]	//税額（終了）
#define SCN_SYZSW_DT	_XXDTA2[ SCN_SYZSW_PN ]		//消費税仕訳区分
#define SCN_ZRITU_DT	_XXDTA2[ SCN_ZRITU_PN ]		//消費税仕訳区分
#define SCN_SKBN_DT		_XXDTA2[ SCN_SKBN_PN ]		//消費税区分
#define SCN_URISIRE_DT	_XXDTA2[ SCN_URISIRE_PN ]	//売り仕入・業種
#define SCN_TKY_DT		_XXDTA2[ SCN_TKY_PN ]	//摘要

#define SCN_SNUM1_DT	_XXDTA2[ SCN_SNUM1_PN ]	//証憑番号（開始）
#define SCN_SNUM2_DT	_XXDTA2[ SCN_SNUM2_PN ]	//証憑番号（開始）
#define SCN_TDAT1_DT	_XXDTA2[ SCN_TDAT1_PN ]	//手形期日（開始）
#define SCN_TDAT2_DT	_XXDTA2[ SCN_TDAT2_PN ]	//手形期日（開始）
#define SCN_HUSEN_DT	_XXDTA2[ SCN_HUSEN_PN ] //付箋
#define SCN_NOINP_DT	_XXDTA2[ SCN_NOINP_PN ] //入力漏れ

	struct _LINE_CND LINE_CND[ SCN_NOINP_PN + 1 ];

};


class CScanKey : public ICSDialog
{
// コンストラクション
public:
	CScanKey(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CScanKey)
	enum { IDD = IDD_SCANKEY_DLG };
	CComboBox	m_NipBrn;
	CButton	m_AllDel;
	CComboBox	m_SType;
	CButton	m_OK;
	CButton	m_Cancel;
	CButton	m_TKonly;
	CButton	m_ChkDBM;
	CButton	m_ChkCBM;
	CButton	m_ChkDKJ;
	CButton	m_ChkCKJ;
	CButton	m_ChkDbr;
	CButton	m_ChkCbr;
	CComboBox	m_Order;
	CComboBox	m_DUser;
	CComboBox	m_Tkycb;
	CComboBox	m_Zritu;
	CComboBox	m_Urisire;
	CComboBox	m_Syzsw;
	CComboBox	m_Skbn;
	CCheckListBox	m_LIST1;
	CICSDBEDT	m_Bmon1;
	CICSDBEDT	m_Bmon2;
	CICSDBEDT	m_Cbmn1;
	CICSDBEDT	m_Cbmn2;
	CICSDBEDT	m_Cbr1;
	CICSDBEDT	m_Cbr2;
	CICSDBEDT	m_Ckji1;
	CICSDBEDT	m_Ckji2;
	CICSDBEDT	m_Cred;
	CICSDBEDT	m_Dbr1;
	CICSDBEDT	m_Dbr2;
	CICSDBEDT	m_Debt;
	CICSDBEDT	m_DBcr;
	CICSDBEDT	m_DCbr1;
	CICSDBEDT	m_DCbr2;
	CICSDBEDT	m_Denp1;
	CICSDBEDT	m_Denp2;
	CICSDBEDT	m_Koji1;
	CICSDBEDT	m_Koji2;
	CICSDBEDT	m_Seq1;
	CICSDBEDT	m_Seq2;
	CICSDBEDT	m_Snum1;
	CICSDBEDT	m_Snum2;
	CICSDBEDT	m_Tdat1;
	CICSDBEDT	m_Tdat2;
	CICSDBEDT	m_Tky;
	CICSDBEDT	m_Val1;
	CICSDBEDT	m_Val2;
	CICSDBEDT	m_Zei1;
	CICSDBEDT	m_Zei2;
	CICSSel	m_selfix8;
	CICSSel	m_selval32;
	//}}AFX_DATA
	CHusenCombo	m_Hcbo;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CScanKey)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	BOOL Create();
	int	 InitWork( SCAN_ITEM* pSI, DWORD nFlag, CDBZmSub* pZm );

	void keyline_inz();
	void keyline_dataset( SCAN_ITEM* pSI );
	void set_syselinz();

	void get_originalsize( double& w, double& h );
	void disp_caution( int sw );
	void ScanKeyAllDel();

	int get_nowpn( void );		// 現在ポジションゲット
	void SelectedIcsselctrl(short position, long Number, int place );

	void last_data_set();		// 最終データセット
	// フォーカスが見えないようにスタティックコントロールに移す。
	void FocusBhnd();
	void EndView();

	void set_nowfocus();	// 現在フォーカス位置にフォーカス
	void set_topfocus();	// 先頭項目位置にフォーカス
	// データスキャン
	int		DB_DataScan();
	BOOL	isScanKeyInp();

	void _disp_change();

protected:
	void set_focus( int pn );	// カーソルセット
#ifdef _ICSPEN_	// 01.20 /01
	void _set_focus( int pn );	// カーソルセット
#endif
	int set_nowpn( int pn );	// 現在ポジションセット

	void	ctrldspjun( int *tbl, int n );
	struct _ItemData *_getDATA( int pn );
	struct _ItemData *getDATA();

	struct _InpCtl *scGetINP_CTL( int pn );
	void keyline_dsp( struct _InpCtl *ctl, struct _SKEY_LINE *dtline );
	void def_syselinz();

	//入力コントロール，１行イニシャライズ
	void fnc_set( struct _InpCtl *ctl);
	void inpctlline_inz( struct _InpCtl *ctl);

	void knam_set( struct _ItemData *data );

	void CtrlShow( int idc, BOOL sw );
	void dsp_etc( int pn, BOOL dspsw );
	void set_etcdsp( int pn );

	void disp_change();

	void disp_bmoninp( int sw );
	void _disp_caution( int sw );

	void get_disp2_originalsize( double& w, double& h );

	void SelItemReset( int now_pn, int prev_pn );

	void inp_bkcol( int pn );
	void select_back_col( int pn );
	void cmntbox( int pn );
	void _cmnt_disp( int idc, int idc_box );
	void selbox( int pn );
	void box_disp( int idc1, int idc2 );
	void _box_disp( int idc, int idc_box );

	long SeqInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long CheckDataIcsinputDate( int pn, long nChar );
	long DateInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long DenpInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long BmonInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long KojiInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	void bmon_select_dsp( int pn, struct _ItemData *keybuf, BOOL bTerm );

	void ScnbmonFocusSub( int pn );
	void ScnKojiFocusSub( int pn );

	BOOL ItemKanaSearch( long data);
	long ItemDataCheck(int pn, long nChar, LPCTSTR string, int dcsw );
	long ItemInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	void FocusScnBrnSub( int pn );
	struct _ItemData *get_brndata( int pn );
	void	TerminationBrn(int pn, long nChar, long inplen, long kst );
	long BrInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	BOOL brnsel_set( int pn, int num );
	BOOL brinp_to_next( int pn, short nChar );

	long ValInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	void tky_select_dsp( CDBipTKREC *ptk, BOOL F12_KEY );

	int set_nonICSpn( tagMSG* pMsg );
	long OnTerminationScnSyzsw( long nChar);
	long OnTerminationScnZritu( long nChar);
	long OnTerminationScnskbn( long nChar);
	long OnTerminationScnurisire(long nChar);

	long OnTerminateOnTkyOnly( long nChar );
	long OnTerminateOnOK( long nChar );
	long OnTerminateOnCancel( long nChar );

	long OnTermCheckDBm( long nChar );
	long OnTermCheckCBm( long nChar );
	long OnTermCheckDkoji( long nChar );
	long OnTermCheckCkoji( long nChar );

	long OnTermCheckDBR( long nChar );
	long OnTermCheckCBR( long nChar );

	long OnTermComboOrder( long nChar );
	long OnTermComboDUser( long nChar );
	long OnTermComboSType( long nChar );

	long OnTermComboHusen( long nChar );
	long OnTermComboNoInp( long nChar );

	struct _KeyMoveTbl* Get_KeyMoveTbl( int nowpn );
	long term_focus( int nowpn, long nChar );
	
	int InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick = FALSE );
	int tky_select( CDBipTKREC* ptkrec, char *data );
	int bmncode_todata( int* bmn, int number, int dc_sw );
	int kojicode_todata( CString& kjcd, int number, int dc_sw );

	BOOL _dsp_etc( CWnd *pwnd, struct _ItemData *data, struct _LINE_CND *cnd, struct _InpCtl *ctl, double x_rate, BOOL dspsw, BOOL focusoff_disp );
	void ChangeEnableDcbr();


#ifndef CLOSE //#* 06.19 /01 // ｲﾒｰｼﾞ摘要の検索の切り替え
public:	
	int is_mdfy_imgtky();
	
protected:	
//	int sel_imgstr();
//	void seltky_btnjob( UINT msg );
	void disp_imgtkyedit( int sw );
//	void selimg_btntxt( int sw );
	long OnTerminateOnImgTkyEdit( long nChar );
	long OnTerminateOnSelTky( long nChar );

	HBRUSH m_EditColor[3];
#endif

public:
	struct _SKEY_LINE SCAN_KEY;
	BOOL	m_bTKYONLY;

// インプリメンテーション
protected:
	CWnd*	m_pParent;
	int		m_nID;
	BOOL	bKamokuCode;
	BOOL	m_DIALOG_OK;
	BOOL	bSCAN_ONLY_MODE;
	int		m_nDtOwner, m_nDtMode;

	double	m_Xrate, m_Yrate;

	CDBZmSub*	m_pZm;
	CDBSyohi	m_Sy;
	CSelectData m_Seldata;
	DWORD		m_nFlag;

	BOOL	BRNTKY_sel;
	int		IMG_master;

	RECT m_original_rect;
	RECT m_disp2_rect;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CScanKey)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTkycombo();
	afx_msg void OnSelchangeScnskbn();
	afx_msg void OnSelchangeScnzritu();
	afx_msg void OnSelchangeScnsyzsw();
	afx_msg void OnSelchangeScnurisire();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeDataUser();
	afx_msg void OnSelchangeDisporder();
	afx_msg void OnSelchangeScnhusen();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTkyonly();
	afx_msg void OnCheckCbm();
	afx_msg void OnCheckCbr();
	afx_msg void OnCheckCkj();
	afx_msg void OnCheckDbm();
	afx_msg void OnCheckDbr();
	afx_msg void OnCheckDkj();
	afx_msg void OnSelchangeComboStype();
	afx_msg void OnSelectedIcsselctrl1(short position, long Number);
	afx_msg void OnSelectedIcsselctrl2(short position, long Number);
	afx_msg void OnAllkeydel();
	afx_msg void OnSelchangeScnnoinpbrn();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnSetFocusScnDBedt( UINT ID );
	void OnKillFocusScnDBedt( UINT ID, long inplen );
	void OnTerminationScnDBedt( UINT ID, long nChar, long inplen, long kst );
	void OnImeEndCompositionScnDBedt( UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);
	void OnKanaTerminationScnDBedt(UINT ID, LPCTSTR Kana);

	void OnSetFocusScnseq1();
	void OnKillFocusScnseq1( long inplen );
	void OnTerminationScnseq1(long nChar, long inplen, long kst);
	void OnSetFocusScnseq2();
	void OnKillFocusScnseq2( long inplen );
	void OnTerminationScnseq2(long nChar, long inplen, long kst);
	void OnSetFocusScndate1();
	void OnKillFocusScndate1( long inplen );
	void OnTerminationScndate1(long nChar, long inplen, long kst);
	void OnSetFocusScndate2();
	void OnKillFocusScndate2( long inplen );
	void OnTerminationScndate2(long nChar, long inplen, long kst);
	void OnSetFocusScndenp1();
	void OnKillFocusScndenp1( long inplen );
	void OnTerminationScndenp1(long nChar, long inplen, long kst);
	void OnSetFocusScndenp2();
	void OnKillFocusScndenp2( long inplen );
	void OnTerminationScndenp2(long nChar, long inplen, long kst);
	void OnSetFocusScnbmon1();
	void OnKillFocusScnbmon1( long inplen );
	void OnTerminationScnbmon1(long nChar, long inplen, long kst);
	void OnSetFocusScnbmon2();
	void OnKillFocusScnbmon2( long inplen );
	void OnTerminationScnbmon2(long nChar, long inplen, long kst);
	void OnSetFocusScncbmn1();
	void OnKillFocusScncbmn1( long inplen );
	void OnTerminationScncbmn1(long nChar, long inplen, long kst);
	void OnSetFocusScncbmn2();
	void OnKillFocusScncbmn2( long inplen );
	void OnTerminationScncbmn2(long nChar, long inplen, long kst);
	void OnSetFocusScnkoji1();
	void OnKillFocusScnkoji1( long inplen );
	void OnTerminationScnkoji1(long nChar, long inplen, long kst);
	void OnSetFocusScnkoji2();
	void OnKillFocusScnkoji2( long inplen );
	void OnTerminationScnkoji2(long nChar, long inplen, long kst);
	void OnSetFocusScnckji1();
	void OnKillFocusScnckji1( long inplen );
	void OnTerminationScnckji1(long nChar, long inplen, long kst);
	void OnSetFocusScnckji2();
	void OnKillFocusScnckji2( long inplen );
	void OnTerminationScnckji2(long nChar, long inplen, long kst);
	void OnSetFocusScnDebt();
	void OnKillFocusScnDebt( long inplen );
	void OnTerminationScnDebt(long nChar, long inplen, long kst );
	void OnSetFocusScnCred();
	void OnKillFocusScnCred( long inplen );
	void OnTerminationScnCred(long nChar, long inplen, long kst );
	void OnSetFocusScnDBcr();
	void OnKillFocusScnDBcr( long inplen );
	void OnTerminationScnDBcr(long nChar, long inplen, long kst );

	void OnSetFocusScndbr1();
	void OnKillFocusScndbr1( long inplen );
	void OnSetFocusScndbr2();
	void OnKillFocusScndbr2( long inplen );
	void OnSetFocusScncbr1();
	void OnKillFocusScncbr1( long inplen );
	void OnSetFocusScncbr2();
	void OnKillFocusScncbr2( long inplen );
	void OnSetFocusScnDCbr1();
	void OnKillFocusScnDCbr1( long inplen );
	void OnSetFocusScnDCbr2();
	void OnKillFocusScnDCbr2( long inplen );

	void OnSetFocusScnval1(); 
	void OnKillFocusScnval1( long inplen );
	void OnTerminationScnval1(long nChar, long inplen, long kst );
	void OnSetFocusScnval2(); 
	void OnKillFocusScnval2( long inplen );
	void OnTerminationScnval2(long nChar, long inplen, long kst );
	void OnSetFocusScnzei1(); 
	void OnKillFocusScnzei1( long inplen );
	void OnTerminationScnzei1(long nChar, long inplen, long kst );
	void OnSetFocusScnzei2(); 
	void OnKillFocusScnzei2( long inplen );
	void OnTerminationScnzei2(long nChar, long inplen, long kst );

	void OnSetFocusScntky();
	void OnKillFocusScntky( long inplen );
	void OnKanaTermScntky(LPCSTR data);
	void OnTerminationScntky(long nChar, long inplen, long kst);

	void OnSetFocusScnTgdate1();
	void OnKillFocusScnTgdate1( long inplen ); 
	void OnTerminationScnTgdate1(long nChar, long inplen, long kst);
	void OnSetFocusScnTgdate2();
	void OnKillFocusScnTgdate2( long inplen ); 
	void OnTerminationScnTgdate2(long nChar, long inplen, long kst); 

	long TgdateInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long CheckDataIcsinputTgDate( int pn, long nChar );

	void OnSetFocusScnSnum1();
	void OnKillFocusScnSnum1( long inplen );
	void OnTerminationScnSnum1(long nChar, long inplen, long kst);
	void OnSetFocusScnSnum2();
	void OnKillFocusScnSnum2( long inplen );
	void OnTerminationScnSnum2(long nChar, long inplen, long kst);

	long SnumInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );

	static int CALLBACK SetSelectFix8( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp );
	static int CALLBACK SetSelectSel32( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp );

	int	 IsNotChkbox( HWND hwnd );
	long OnTermNotChkbox( int pn, long nChar );
	void NotCheckSet( int pn, int sgn );

public:
	BOOL fix8cnd();
	void fix8cnd( BOOL sg );
	BOOL fixvalcnd();
	void fixvalcnd( BOOL sg );
	void fix8disp( BOOL sg );
	
	/* 固定選択画面の状態 */
	void enable_fix8( BOOL sg );
	
	void enable_fixval( BOOL sg );
	void set_valtitle( char *msg );
	void valtitle( BOOL sg );

protected:
	BOOL	fix8sg;
	BOOL	fixvalsg;
	RECT m_selfix8_rect;
	RECT m_valtitle_rect;

#ifdef _ICSPEN_	// 01.20 /01
private:
//	ICSPen*	icspen;
	BOOL	flgPen;
	CRect	rectPen;

protected:

	void set_nextpos();
	void OnICSInputFocus( int pn );

#ifdef LATER_CLOSE
	void _OnSetFocusScnseq1();
	void _OnSetFocusScnseq2();
	void _OnSetFocusScndate1(); 
	void _OnSetFocusScndate2();
	void _OnSetFocusScndenp1();
	void _OnSetFocusScndenp2(); 
	void _OnSetFocusScnbmon1();
	void _OnSetFocusScnbmon2(); 
	void _OnSetFocusScndebt();
	void _OnSetFocusScncred();
	void _OnSetFocusScndbcr();
	void _OnSetFocusScnval1(); 
	void _OnSetFocusScnval2();
	void _OnSetFocusScntaika1(); 
	void _OnSetFocusScntaika2(); 
	void _OnSetFocusScnskbn();
	void _OnSetFocusScnurisire();
	void _OnSetFocusScntky();
	void _OnFocusIcsheditctrl1();
	void _OnFocusIcsheditctrl2();

#endif

public:
//	short SetPen( BOOL flg, ICSPen* ppen );
	void SetPenRect( CRect& rect );
#endif

private:
	HACCEL m_hAccelTable;

	int	m_caution_dispcnd;
	int& caution_dispcnd() { return	m_caution_dispcnd; }

	int m_bmon_dispcnd;
	int& bmon_dispcnd() { return m_bmon_dispcnd; }
	
	int LbuttonDownJob( struct tagMSG * pMsg );

};
