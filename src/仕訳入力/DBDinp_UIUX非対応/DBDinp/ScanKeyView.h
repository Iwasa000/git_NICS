#pragma once

#include "icsdbedt.h"
#include "icssel.h"
#include "husencombo.h"
#include "DBbaseView.h"
#include "icsdateselect.h"
#include "afxwin.h"
#include "ownerbutton.h"
#include "ColoredComboDD.h"
#include <SyzInvoice.h>


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

#define SCN_DCBMN1_PN	13	//貸借部門（開始）
#define SCN_DCBMN2_PN	14	//貸借部門（終了）

#define SCN_KOJI1_PN	15	//工事（開始）
#define SCN_KOJI2_PN	16	//工事（終了）
#define SCN_CKJI1_PN	17	//工事（開始）
#define SCN_CKJI2_PN	18	//工事（終了）

#define SCN_DCKJI1_PN	19	//貸借工事（開始）
#define SCN_DCKJI2_PN	20	//貸借工事（終了）

#define SCN_DEBT_PN		21	//借方（開始）
#define SCN_DEBT2_PN	22	//借方（終了）
#define SCN_DBR1_PN		23	//借方枝番（開始）
#define SCN_DBR2_PN		24	//借方枝番（終了）
#define SCN_CRED_PN		25	//貸方（開始）
#define SCN_CRED2_PN	26	//貸方（終了）
#define SCN_CBR1_PN		27	//貸方枝番（開始）
#define SCN_CBR2_PN		28	//貸方枝番（終了）
#define SCN_DBCR_PN		29	//貸借（開始）
#define SCN_DBCR2_PN	30	//貸借（終了）
#define SCN_DCBR1_PN	31	//貸借枝番（開始）
#define SCN_DCBR2_PN	32	//貸借枝番（終了）
#define SCN_VAL1_PN		33	//金額（開始）
#define SCN_VAL2_PN		34	//金額（終了）
#define SCN_ZEI1_PN		35	//税額（開始）
#define SCN_ZEI2_PN		36	//税額（終了）
#define SCN_TAI1_PN		37	//対価（開始）
#define SCN_TAI2_PN		38	//対価（終了）

#define SCN_SYZSW_PN	39	//消費税仕訳区分
#define SCN_ZRITU_PN	40	//税率
#define SCN_SKBN_PN		41	//消費税区分
#define SCN_URISIRE_PN	42	//業種・売り仕入区分
#define SCN_TOKUS_PN	43	//特定収入
#define SCN_TKY_PN		44	//摘要
#define SCN_INVNO_PN	45	// インボイス登録番号
#define SCN_SNUM1_PN	46	//手形番号
#define SCN_SNUM2_PN	47	//手形番号
#define SCN_TDAT1_PN	48	//手形期日
#define SCN_TDAT2_PN	49	//手形期日
#define SCN_HUSEN_PN	50	//付箋
#define SCN_HUCMT_PN	51	//付箋コメント
#define SCN_MENZEI_PN	52	// 免税事業者からの課税仕入れ
#define SCN_MENWARI_PN	53	// 免税事業者からの課税仕入れ
#define SCN_NOINP_PN	54	//入力漏れ
#define SCN_GYOKBN_PN	55	//業種の区分
#define SCN_HJIYU_PN	56	//変動事由

#define SCN_DOCEVI1_PN	57	//証憑番号１
#define SCN_DOCEVI2_PN	58	//証憑番号２

#define SCN_DOCSEQ1_PN	59	//文書番号１
#define SCN_DOCSEQ2_PN	60	//文書番号２
#define SCN_DOCSHTP_PN	61	//書類種別
#define SCN_SYZKTSN_PN	62	//固定資産
#define SCN_DTTYPE_PN	63	//仕訳データタイプ


#define SCN_CHKFURI_PN	64	//振替伝票
#define SCN_CHKNYUK_PN	65	//入金伝票
#define SCN_CHKSYUK_PN	66	//出金伝票
#define SCN_CHKDBM_PN	67	//借方部門チェックボタン
#define SCN_CHKCBM_PN	68	//貸方部門チェックボタン
#define SCN_CHKDKJ_PN	69	//借方工事チェックボタン
#define SCN_CHKCKJ_PN	70	//借方工事チェックボタン
#define SCN_CHKDBR_PN	71	//借方チェックボタン
#define SCN_CHKCBR_PN	72	//貸方チェックボタン
#define SCN_CHKKMSYZ_PN	73	//科目と異なる設定の消費税仕訳

#define SCN_CHKINPDT_PN	74	//入力した仕訳
#define SCN_CHKMDFDT_PN	75	//修正した仕訳

#define SNOT_SEQ_PN			76
#define SNOT_DATE_PN		77
#define SNOT_DENP_PN		78
#define SNOT_DBMN_PN		79
#define SNOT_CBMN_PN		80
#define SNOT_DCBMN_PN		81
#define SNOT_DKJI_PN		82
#define SNOT_CKJI_PN		83
#define SNOT_DCKJI_PN		84
#define SNOT_DEB_PN			85
#define SNOT_DBRN_PN		86
#define SNOT_CRE_PN			87
#define SNOT_CBRN_PN		88
#define SNOT_DBCR_PN		89
#define SNOT_DCBRN_PN		90
#define SNOT_VAL_PN			91
#define SNOT_ZEI_PN			92
#define SNOT_TAI_PN			93
#define SNOT_SYZSW_PN		94
#define SNOT_ZRITU_PN		95
#define SNOT_SKBN_PN		96
#define SNOT_URISIRE_PN		97
#define SNOT_TOKUS_PN		98
#define SNOT_TKY_PN			99
#define SNOT_INVNO_PN		100
#define SNOT_SNUM_PN		101
#define SNOT_TGDATE_PN		102
#define SNOT_HUSEN_PN		103
#define SNOT_DOCEVI_PN		104
#define SNOT_DOCSEQ_PN		105
#define SNOT_MENZEI_PN		106

#define SNOT_LAST_PN		SNOT_MENZEI_PN


#define SCN_IDOK		(SNOT_LAST_PN+1)	// ＯＫボタン
#define SCN_IDCANCEL	(SNOT_LAST_PN+2)	// キャンセルボタン
#define SCN_BRNINP_PN	(SNOT_LAST_PN+3)	// 枝番入力

#define SCN_TKYONLY		(SNOT_LAST_PN+4)	// 摘要のみ修正

#define SCN_SELTKY_PN	(SNOT_LAST_PN+5)	// イメージ摘要切り替えコンボ
#define SCN_IMGEDT_PN	(SNOT_LAST_PN+6)	// イメージ摘要と表示するエディットボックス

#define SCN_ALLDEL_PN	(SNOT_LAST_PN+7)	// 全削除ボタン
#define SCN_DUPLI_PN	(SNOT_LAST_PN+8)	// 重複仕訳ボタン
#define SCN_DELSWK_PN	(SNOT_LAST_PN+9)	// 取消仕訳検索ボタン


//#define SCN_BMNINP_PN	(SCN_TDAT2_PN+7)	// 部門入力

//#define TSM_NORMAL		0	// 通常摘要検索
//#define TSM_IMG			1	// イメージ摘要のみ検索
//#define TSM_STR			2	// 文字摘要のみ検索
//#define TSM_STRNONE		3	// 文字未入力摘要の検索

//入力データ
struct _SKEY_LINE {
	struct _ItemData	_XXDTA2[ SCN_DTTYPE_PN + 1 ];
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
#define SCN_DCBMN1_DT	_XXDTA2[ SCN_DCBMN1_PN ]//貸借部門（開始）
#define SCN_DCBMN2_DT	_XXDTA2[ SCN_DCBMN2_PN ]//　　部門（終了）

#define SCN_KOJI1_DT	_XXDTA2[ SCN_KOJI1_PN ]	//借方工事（開始）
#define SCN_KOJI2_DT	_XXDTA2[ SCN_KOJI2_PN ]	//　　工事（終了）
#define SCN_CKJI1_DT	_XXDTA2[ SCN_CKJI1_PN ]	//貸方工事（開始）
#define SCN_CKJI2_DT	_XXDTA2[ SCN_CKJI2_PN ]	//　　工事（終了）
#define SCN_DCKJI1_DT	_XXDTA2[ SCN_DCKJI1_PN ]//貸借工事（開始）
#define SCN_DCKJI2_DT	_XXDTA2[ SCN_DCKJI2_PN ]//　　工事（終了）

#define SCN_DEBT_DT		_XXDTA2[ SCN_DEBT_PN ]	//借方 （開始）
#define SCN_DEBT2_DT	_XXDTA2[ SCN_DEBT2_PN ]	//借方 （終了）
#define SCN_DBR1_DT		_XXDTA2[ SCN_DBR1_PN ]	//借方枝番（開始）
#define SCN_DBR2_DT		_XXDTA2[ SCN_DBR2_PN ]	//借方枝番（終了）

#define SCN_CRED_DT		_XXDTA2[ SCN_CRED_PN ]	//貸方 （開始）
#define SCN_CRED2_DT	_XXDTA2[ SCN_CRED2_PN ]	//貸方 （終了）
#define SCN_CBR1_DT		_XXDTA2[ SCN_CBR1_PN ]	//貸方枝番（開始）
#define SCN_CBR2_DT		_XXDTA2[ SCN_CBR2_PN ]	//貸方枝番（終了）

#define SCN_DBCR_DT		_XXDTA2[ SCN_DBCR_PN ]	//貸借 （開始）
#define SCN_DBCR2_DT	_XXDTA2[ SCN_DBCR2_PN ]	//貸借 （終了）
#define SCN_DCBR1_DT	_XXDTA2[ SCN_DCBR1_PN ]	//貸借枝番（開始）
#define SCN_DCBR2_DT	_XXDTA2[ SCN_DCBR2_PN ]	//貸借枝番（終了）
#define SCN_VAL1_DT		_XXDTA2[ SCN_VAL1_PN ]	//金額（開始）
#define SCN_VAL2_DT		_XXDTA2[ SCN_VAL2_PN ]	//金額（終了）
#define SCN_ZEI1_DT		_XXDTA2[ SCN_ZEI1_PN ]	//税額（開始）
#define SCN_ZEI2_DT		_XXDTA2[ SCN_ZEI2_PN ]	//税額（終了）
#define SCN_TAI1_DT		_XXDTA2[ SCN_TAI1_PN ]	//対価（開始）
#define SCN_TAI2_DT		_XXDTA2[ SCN_TAI2_PN ]	//対価（終了）

#define SCN_SYZSW_DT	_XXDTA2[ SCN_SYZSW_PN ]		//消費税仕訳区分
#define SCN_ZRITU_DT	_XXDTA2[ SCN_ZRITU_PN ]		//消費税仕訳区分
#define SCN_SKBN_DT		_XXDTA2[ SCN_SKBN_PN ]		//消費税区分
#define SCN_URISIRE_DT	_XXDTA2[ SCN_URISIRE_PN ]	//売り仕入・業種
#define SCN_TOKUS_DT	_XXDTA2[ SCN_TOKUS_PN ]		//特定収入
#define SCN_TKY_DT		_XXDTA2[ SCN_TKY_PN ]	//摘要
#define SCN_INVNO_DT	_XXDTA2[SCN_INVNO_PN]		// 登録番号

#define SCN_SNUM1_DT	_XXDTA2[ SCN_SNUM1_PN ]	//手形番号（開始）
#define SCN_SNUM2_DT	_XXDTA2[ SCN_SNUM2_PN ]	//手形番号（開始）
#define SCN_TDAT1_DT	_XXDTA2[ SCN_TDAT1_PN ]	//手形期日（開始）
#define SCN_TDAT2_DT	_XXDTA2[ SCN_TDAT2_PN ]	//手形期日（開始）
#define SCN_HUSEN_DT	_XXDTA2[ SCN_HUSEN_PN ] //付箋
#define SCN_HUCMT_DT	_XXDTA2[ SCN_HUCMT_PN ] //付箋コメント
#define SCN_MENZEI_DT	_XXDTA2[SCN_MENZEI_PN]	// 免税事業者からの課税仕入れ
#define SCN_MENWARI_DT	_XXDTA2[SCN_MENWARI_PN]	// 免税事業者からの課税仕入れ 控除割合

#define SCN_NOINP_DT	_XXDTA2[ SCN_NOINP_PN ] //入力漏れ
#define SCN_GYOKBN_DT	_XXDTA2[ SCN_GYOKBN_PN] //業種の区分
#define SCN_HJIYU_DT	_XXDTA2[ SCN_HJIYU_PN ] //変動事由

#define SCN_DOCEVI1_DT	_XXDTA2[ SCN_DOCEVI1_PN ]	//証憑番号（開始）
#define SCN_DOCEVI2_DT	_XXDTA2[ SCN_DOCEVI2_PN ]	//証憑番号（開始）

#define SCN_DOCSEQ1_DT	_XXDTA2[ SCN_DOCSEQ1_PN ]	//文書番号１
#define SCN_DOCSEQ2_DT	_XXDTA2[ SCN_DOCSEQ2_PN ]	//文書番号２
#define SCN_DOCSHTP_DT	_XXDTA2[ SCN_DOCSHTP_PN ]	//書類種別
#define SCN_SYZKTSN_DT	_XXDTA2[ SCN_SYZKTSN_PN ]	//書類種別
#define SCN_DTTYPE_DT	_XXDTA2[ SCN_DTTYPE_PN ]		//仕訳データタイプ

	struct _LINE_CND LINE_CND[ SCN_DTTYPE_PN + 1 ];

};



// CScanKeyView フォーム ビュー

class CScanKeyView : public CDBbaseView
{
	DECLARE_DYNCREATE(CScanKeyView)

protected:
	CScanKeyView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CScanKeyView();

public:
	enum { IDD = IDD_SCANKEYFORM };
	CComboBox	m_NipBrn;
	CComboBox	m_GyoKbn;
	CButton	m_AllDel;
	CComboBox	m_SType;
	CButton	m_OK;
	CButton	m_Cancel;
	CButton	m_TKonly;
	CComboBox	m_Order;
	CComboBox	m_DUser;
	CComboBox	m_Tkycb;
	CComboBox	m_Zritu;
	CComboBox	m_Urisire;
	CComboBox	m_Syzsw;
	CComboBox	m_Skbn;
	CICSDBEDT	m_Bmon1;
	CICSDBEDT	m_Bmon2;
	CICSDBEDT	m_Cbmn1;
	CICSDBEDT	m_Cbmn2;
	CICSDBEDT	m_Cbr1;
	CICSDBEDT	m_Cbr2;
	CICSDBEDT	m_Ckji1;
	CICSDBEDT	m_Ckji2;

	CICSDBEDT	m_DCbmn1;
	CICSDBEDT	m_DCbmn2;
	CICSDBEDT	m_DCkji1;
	CICSDBEDT	m_DCkji2;

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
	CICSDBEDT	m_Tai1;
	CICSDBEDT	m_Tai2;
	CICSDBEDT	m_Debt2;
	CICSDBEDT	m_Cred2;
	CICSDBEDT	m_DBcr2;
	CICSDBEDT	m_HUcmt;

	CICSDBEDT	m_DocEvi1;
	CICSDBEDT	m_DocEvi2;
	CICSDBEDT	m_DocSeq1;
	CICSDBEDT	m_DocSeq2;
	CColoredComboDD	m_DShtCombo;
	CComboBox	m_SyzKtsn;
	CComboBox	m_DTtypeCmb;
	CComboBox	m_MenWariCmb;

	CHusenComboEX	m_Hcbo;
	ICSDateSelect	m_selDate;
	CComboBox		m_KSHcombo;
	CComboBox		m_Tokus;

	COwnerButton	m_NotSeq;
	COwnerButton	m_NotDate;
	COwnerButton	m_NotDenp;
	COwnerButton	m_NotDbmn;
	COwnerButton	m_NotCbmn;
	COwnerButton	m_NotDkji;
	COwnerButton	m_NotCkji;
	COwnerButton	m_NotDbt;
	COwnerButton	m_NotDbrn;
	COwnerButton	m_NotCre;
	COwnerButton	m_NotCbrn;
	COwnerButton	m_NotDBCR;
	COwnerButton	m_NotDCbrn;

	COwnerButton	m_NotVal;
	COwnerButton	m_NotZei;
	COwnerButton	m_NotTai;
	COwnerButton	m_NotSyzsw;
	COwnerButton	m_NotZritu;
	COwnerButton	m_NotSkbn;
	COwnerButton	m_NotUrisire;
	COwnerButton	m_NotTokus;

	COwnerButton	m_NotTky;
	COwnerButton	m_NotSnum;
	COwnerButton	m_NotTgdate;
	COwnerButton	m_NotHusen;
	COwnerButton	m_NotDocEvi;	//証憑番号
	COwnerButton	m_NotDCBmn;
	COwnerButton	m_NotDCKji;
	COwnerButton	m_NotDocSeq;

	// インボイス対応
	CICSDBEDT		m_InvnoEdt;		// 登録番号
	CICSDBEDT		m_InvnoDisp;	// 表示用
	COwnerButton	m_NotInvno;
	COwnerButton	m_NotMenzei;


	ICSReportControl	m_Report;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void to_scankey();

	void keyline_inz( BOOL bDisp = TRUE );
	void keyline_dataset( SCAN_ITEMEX* pSI );
	void set_syselinz();

	void get_originalsize( double& w, double& h );
	void disp_caution( int sw );
	void ScanKeyAllDel();
	void ScanKeyClear( BOOL bMust = FALSE );

	int get_nowpn( void );		// 現在ポジションゲット
	void SelectedIcsselctrl(short position, long Number, int place );

	void last_data_set();		// 最終データセット
	// フォーカスが見えないようにスタティックコントロールに移す。
	void FocusBhnd();

	void set_nowfocus();	// 現在フォーカス位置にフォーカス
	void set_topfocus();	// 先頭項目位置にフォーカス
	// データスキャン
	BOOL	isScanKeyInp();

	void _disp_change();
	int  ReloadView();
	int  get_skeyitem( int sgn );
	void skeyitem_init();

	int GetSkeyKmkType();

	HWND GetKamokuCtrlHWND();
	void KanaSearchUpdate();
	int  GetToolSub();

protected:
	void set_focus( int pn );	// カーソルセット
#ifdef _ICSPEN_	// 01.20 /01
	void _set_focus( int pn );	// カーソルセット
#endif
	int set_nowpn( int pn );	// 現在ポジションセット
	void TermBuzzer();

	void	ctrldspjun( int *tbl, int n );
	struct _ItemData *_getDATA( int pn );
	struct _ItemData *getDATA();

	struct _InpCtl *scGetINP_CTL( int pn );
	void keyline_dsp( struct _InpCtl *ctl, struct _SKEY_LINE *dtline );
	void def_syselinz();

	//入力コントロール，１行イニシャライズ
	void fnc_set( struct _InpCtl *ctl);
	void inpctlline_inz( struct _InpCtl *ctl);
	void SetTokusWnd( BOOL bEnable );

	void InitTkyReport();
	void TkyReportDisp();

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
	void koji_select_dsp( int pn, struct _ItemData *keybuf, BOOL bTerm );

	void ScnbmonFocusSub( int pn );
	void ScnKojiFocusSub( int pn );

	BOOL ItemKanaSearch( LPCTSTR data);
	long ItemDataCheck(int pn, long nChar, LPCTSTR string, int dcsw );
	long ItemInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	void KamokuTermJob( int pn, long nChar, long inplen, long kst );

	void FocusScnBrnSub( int pn );
	struct _ItemData *get_brndata( int pn );
	int		CheckImeEndBranch( UINT ID, long nChar, LPCTSTR string );
	void	TerminationBrn(int pn, long nChar, long inplen, long kst );
	long BrInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	BOOL brnsel_set( int pn, int num );

	long ValInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	void tky_select_dsp( CDBipTKREC *ptk, BOOL F12_KEY );

	int set_nonICSpn( tagMSG* pMsg );
	long OnTerminationScnSyzsw( long nChar);
	long OnTerminationScnZritu( long nChar);
	long OnTerminationScnskbn( long nChar);
	long OnTerminationScnurisire(long nChar);
	long OnTerminationScnTokus(long nChar);

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
	long OnTermComboGyoKbn( long nChar );
	long OnTermComboHjiyu( long nChar );

	long OnTermCheckKMSYZ( long nChar );

	struct _KeyMoveTbl* Get_KeyMoveTbl( int nowpn );
	long term_focus( int nowpn, long nChar );
	
//	BOOL _dsp_etc( CWnd *pwnd, struct _ItemData *data, struct _LINE_CND *cnd, struct _InpCtl *ctl, double x_rate, BOOL dspsw, BOOL focusoff_disp );
	void ChangeEnableDcbr();
	void gotokeyscan();

	void set_dateselect();
	int skey_dateconv( BYTE* bcddate, int* year, int* ofs );
	void DateSelectToSkeyLine();
	BOOL disp_brnselect( int pn );

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
	HBRUSH m_ChkColor;

public:
	struct _SKEY_LINE SCAN_KEY;

// インプリメンテーション
protected:
	CWnd*	m_pParent;
	int		m_nID;
	BOOL	m_DIALOG_OK;
	int		m_nDtOwner, m_nDtMode;
	int		m_HenCnt;

	double	m_Xrate, m_Yrate;

	int		IMG_master;

	RECT	m_original_rect;
	RECT	m_disp2_rect;
	// 摘要複数選択の エディット
	int		m_idxTkyReport;
	// 科目検索モード 単一( = 0)、範囲( = 1)
	int		m_KmkType;

	BOOL	m_bDataTypeCombo;

	BOOL	m_bIMEendBrn;

	BOOL	m_bIgnoreFocus;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	afx_msg void OnSelchangeTkycombo();
	afx_msg void OnSelchangeScnskbn();
	afx_msg void OnSelchangeScnzritu();
	afx_msg void OnSelchangeScnsyzsw();
	afx_msg void OnSelchangeScnurisire();
	afx_msg void OnSelchangeScnTokus();
//	virtual void OnCancel();
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
	afx_msg void OnCheckKMSYZ();
	afx_msg void OnSelchangeComboStype();
	afx_msg void OnSelectedIcsselctrl1(short position, long Number);
	afx_msg void OnSelectedIcsselctrl2(short position, long Number);
	afx_msg void OnAllkeydel();
	afx_msg void OnSelchangeScnnoinpbrn();
	afx_msg void OnSelchangeScnGyoKbn();
	afx_msg void OnSelchangeScnSyzKtsn();
	afx_msg void OnSelchangeDataType();
	afx_msg void OnSelchangeMenWari();
	DECLARE_EVENTSINK_MAP()
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
	
	void OnSetFocusScnDCbmn1();
	void OnKillFocusScnDCbmn1( long inplen );
	void OnTerminationScnDCbmn1(long nChar, long inplen, long kst);
	void OnSetFocusScnDCbmn2();
	void OnKillFocusScnDCbmn2( long inplen );
	void OnTerminationScnDCbmn2(long nChar, long inplen, long kst);

	void OnSetFocusScnDCkji1();
	void OnKillFocusScnDCkji1( long inplen );
	void OnTerminationScnDCkji1(long nChar, long inplen, long kst);
	void OnSetFocusScnDCkji2();
	void OnKillFocusScnDCkji2( long inplen );
	void OnTerminationScnDCkji2(long nChar, long inplen, long kst);

	void OnSetFocusScnDebt();
	void OnKillFocusScnDebt( long inplen );
	void OnTerminationScnDebt(long nChar, long inplen, long kst );
	void OnSetFocusScnDebt2();
	void OnKillFocusScnDebt2( long inplen );
	void OnTerminationScnDebt2(long nChar, long inplen, long kst );

	void OnSetFocusScnCred();
	void OnKillFocusScnCred( long inplen );
	void OnTerminationScnCred(long nChar, long inplen, long kst );
	void OnSetFocusScnCred2();
	void OnKillFocusScnCred2( long inplen );
	void OnTerminationScnCred2(long nChar, long inplen, long kst );

	void OnSetFocusScnDBcr();
	void OnKillFocusScnDBcr( long inplen );
	void OnTerminationScnDBcr(long nChar, long inplen, long kst );
	void OnSetFocusScnDBcr2();
	void OnKillFocusScnDBcr2( long inplen );
	void OnTerminationScnDBcr2(long nChar, long inplen, long kst );

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

	void OnSetFocusScntai1(); 
	void OnKillFocusScntai1( long inplen );
	void OnTerminationScntai1(long nChar, long inplen, long kst );
	void OnSetFocusScntai2(); 
	void OnKillFocusScntai2( long inplen );
	void OnTerminationScntai2(long nChar, long inplen, long kst );

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

	void OnSetFocusScnHUcmt();
	void OnKillFocusScnHUcmt( long inplen ); 
	void OnTerminationScnHUcmt(long nChar, long inplen, long kst);
	long HUcmtInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );

	int	 IsNotChkbox( HWND hwnd );
	long OnTermNotChkbox( int pn, long nChar );
	void NotCheckSet( int pn, int sgn );

	int		IsDenpChkbox( HWND hwnd );
	long	OnTermDenpChkbox( int pn, long nChar );

	int		CheckImeEndBmon( UINT ID, long nChar, LPCTSTR string );
	int		CheckImeEndKoji( UINT ID, long nChar, LPCTSTR string );

	long	OnReportKeyJob( long nChar );

	void	ButtonF8Job(int duplioff = 1 );

	void OnSetFocusScnDocEvi1();
	void OnKillFocusScnDocEvi1( long inplen );
	void OnTerminationScnDocEvi1(long nChar, long inplen, long kst);

	void OnSetFocusScnDocEvi2();
	void OnKillFocusScnDocEvi2( long inplen );
	void OnTerminationScnDocEvi2(long nChar, long inplen, long kst);

	long DocEviInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	void set_docevi();
	void enable_docevi( BOOL bEnable );

	void ChangeEnableDCbmn();
	void ChangeEnableDCkji();

	void set_husencmt();

	void set_DataType();

	void OnSetFocusScnDocSeq1();
	void OnKillFocusScnDocSeq1( long inplen );
	void OnTerminationScnDocSeq1(long nChar, long inplen, long kst);
	void OnSetFocusScnDocSeq2();
	void OnKillFocusScnDocSeq2( long inplen );
	void OnTerminationScnDocSeq2(long nChar, long inplen, long kst);

	long OnTermComboDocSheetTyp( long nChar );
	void set_DocSeqAndShtyp();
	void OnSelchangeScnShtTyp();
	long OnTermComboSyzKtsisan( long nChar );

	int	 EdabanKanaSearch(UINT ID, LPCTSTR data);
	void SelchangeDuserJob( int sel, int init = 0 );

	long OnTermComboDataType( long nChar );

	// インボイス対応
	void OnSetFocusScnInvno();
	void OnKillFocusScnInvno( long inplen );
	void OnTerminationScnInvno( long nChar, long inplen, long kst );

	void DispInvno( int pn, CString invno );
	long InvnoInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	long OnTerminationMenzei( long nChar );
	void InvoiceKeyMoveChange();

	long OnTerminationMenWari(long nChar);

public:

#ifdef CLOSE
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

#endif

#ifdef _ICSPEN_	// 01.20 /01
private:
//	ICSPen*	icspen;
	BOOL	flgPen;
	CRect	rectPen;


	// インボイス対応
	CSyzInvoice syzInvoice;

protected:

	void set_nextpos();
	void OnICSInputFocus( int pn );

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

	BOOL	m_checkfirst;
	BOOL&	FirstTime() { return	m_checkfirst; }

public:
	void	BeginApp();
	BOOL	EndApp( WPARAM wParam = 0, BOOL bCosel = FALSE  );

	virtual void CalcMouseEvent();
	virtual int  TabLbuttonDownJob();
	virtual void ReCoselSetFocus(BOOL bNewCo);
	
	void	set_DocShtypCombo();
	void	InvoiceCtrlMove(int begin = 0);

	long	CheckDataDBeditInv(long nChar, LPCTSTR string);
	void	TGnumWndMove();

public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedNchkSyzsw();
protected:
	virtual void OnDraw(CDC* /*pDC*/);

public:
	afx_msg void OnButtonF8();
	afx_msg void OnButtonF11();
	afx_msg void OnButtonEnd();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	afx_msg void OnCbnSelchangeScnhjiyu();
	afx_msg void OnUpdateButtonF4(CCmdUI *pCmdUI);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnButtonF9();
	afx_msg void OnButtonInsert();
	void OnUpdateButtonInsert(CCmdUI *pCmdUI);

	afx_msg void OnButtonF6();
	void OnNMReportSelchangedTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkeydownTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditsetfocusTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkillfocusTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnUpdateButtonF6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);

	afx_msg void OnButtonF5();
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);

	LRESULT OnSelGuideMsg(WPARAM w, LPARAM l );
	afx_msg void OnBnClickedNchkSno();
	LRESULT OnSelChgMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckMenzei();
	afx_msg void OnBnClickedNchkInvno();
	afx_msg void OnBnClickedNchkMenzei();
	afx_msg void OnBnClickedBtnDupli();

	long OnTerminateOnAllKeyDel(long nChar);
};


