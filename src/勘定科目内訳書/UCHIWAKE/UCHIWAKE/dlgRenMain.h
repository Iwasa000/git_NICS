// dlgRenMain.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CdlgRenMain ダイアログ
#ifndef __RENMAIN_H__
#define __RENMAIN_H__

#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "icsbutton.h"
#include "icshedit.h"
#include "dbUcRenMain.h"	//　uc_ren_mainクラス
#include "dbUcInfSub.h"		//  uc_inf_subクラス
#include "dlgRenKamoku.h"	//	連動科目一覧ダイアログクラス
#include "dlgRenTekiyo.h"	//	連動摘要一覧ダイアログクラス
#include "UcCommon.h"		//	定数ヘッダ
#include "UcFunctionCommon.h"	// 共通関数クラス
#include "icsdisp.h"
#include "icsdateselect.h"
class CdlgRenMain : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgRenMain)

public:
	CdlgRenMain(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgRenMain();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_RENMAIN };
	
	CButton	m_btnKz;
	CButton m_btnEz;
	CButton m_btnDa;
	CButton m_btnKs;

	CButton m_btnAllClear;
	CButton m_btnValRendo;

	CButton m_chkZero;
	CButton m_chkGassan;
// midori 161110 add -->
	CButton m_chkEdaren;
// midori 161110 add <--
// midori 152153 add -->
	CButton m_chkKmkname;
	CButton m_chkF9Upd;
// midori 152153 add <--

	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	ICSButton m_btnKamoku;

	CICSHedit m_txtTkcodf;
	CICSHedit m_txtTkcodt;

	ICSStatic m_lblYoushiki;
	ICSStatic m_lblStartTeki;
	ICSStatic m_lblEndTeki;
	ICSStatic m_lblStartTekiName;
	ICSStatic m_lblEndTekiName;
	ICSStatic m_lblTekiMessage;


private:
	CDBZmSub*		m_pZmSub;		//  財務DBクラスハンドル
	CDatabase*		m_pDB;			//	データベースハンドル
	int 			m_nFormSeq;		//	様式シーケンス番号
	RENZVOL_INFO*	m_pRenInfo;		//  会社情報構造体のポインタ
	int				m_nRet;			//	戻値
	BOOL			m_blnTkcodf;	//	摘要開始番号のTermination制御に使用
	BOOL			m_blnTkcodt;	//	摘要終了番号のTermination制御に使用
	int				m_nOpOldRenType;//	前回実行した財務データ
	CUcFunctionCommon m_clsFunc;	//	共通関数クラス

public:
	int		m_FormSeq;			// 様式シーケンス番号
	int		m_OpRenType;		// 選択元データ
	BYTE	m_OpRenTypeKz;		// 科目残高On/Offフラグ
	BYTE	m_OpRenTypeEz;		// 枝番残高On/Offフラグ
	BYTE	m_OpRenTypeDa;		// 仕訳データOn/Offフラグ
	BYTE	m_OpRenTypeKs;		// 科目集計残高On/Offフラグ
	long	m_OpTkcodFrom;		// 仕訳時の摘要開始番号
	long	m_OpTkcodTo;		// 仕訳時の摘要終了番号
	BYTE	m_OpOutZeroEn;		// 残高0円時のデータ作成On/Offフラグ
	BYTE	m_OpGassan;			// 同一取引先合算On/Offフラグ
	BYTE	m_UcKmkEntry;		// 新規内訳書科目追加確認フラグ
	BYTE	m_OpUpdType;		// 連動方法選択フラグ
	int		m_OpOldRenType;		// 前回選択した財務データ
// midori 161110 add -->
	BYTE	m_OpEdaren;			// 枝番未登録･未入力のデータを連動するOn/Offフラグ
// midori 161110 add <--
// midori 152153 add -->
	BYTE	m_OpKmkName;		// 科目名称を取引先、金融機関名称に転記する
	BYTE	m_OpF9Upd;			// 取引先、金融機関の[F9 参照]に登録する
	BOOL	m_RegAutoRef;		// 取引先、金融機関を自動で登録する
// midori 152153 add <--

public:
// midori 152153 del -->
//	int ShowDialog( CDatabase* pDB = NULL , RENZVOL_INFO* pRenInfo = NULL , int nFormSeq = ID_FORMNO_011 );	//	ダイアログ表示z
// midori 152153 del <--
// midori 152153 add -->
	int ShowDialog( CDatabase* pDB = NULL , RENZVOL_INFO* pRenInfo = NULL , int nFormSeq = ID_FORMNO_011,BOOL nRegAutoRef = FALSE );	//	ダイアログ表示z
// midori 152153 add <--
	int	RendoGetSyukeiTerm(RENTEMP_INFO* uRenInfo);
private:
	void GetRenMainData( CDatabase* pDB , RENZVOL_INFO* pRenInfo , int nFormSeq );
	void SetOriginalData( CdbUcRenMain* mfcRec , RENZVOL_INFO* pRenInfo );
	void SetRendoType( CdbUcRenMain* mfcRec );
	void SetDetailData( CdbUcRenMain* mfcRec ); 
	void GetYoushikiTitle( CDatabase* pDB, short nFormSeq, CString* pszTitle );
	void TerminationCommon( short nChar, LPCTSTR data, ICSStatic* lblData , BOOL* blnFlag , CICSHedit* picsHedt );
	void UpdateTekiyoData( int intID );
	BOOL SetMainData( CDatabase* pDB , int nFormSeq );
	void SetSiwakeControl( BOOL blnFlag );
	BOOL SetRendoTypeControl();
	BOOL CheckSetData();
	BOOL CheckTekiyoNo();
	BOOL CheckKamokuEntry();
	BOOL CheckKamokuEntrySub( CDBZmSub* ZmSub , CString strkncod );
	BOOL IsRendoValDflt();
	int  InitOutputTermCtrl();
	int SetMonthOffsetRuikei();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void FocusStarttekiEdit();
	void FocusEndtekiEdit();
	void TerminationStarttekiEdit(short nChar, short length, LPCTSTR data);
	void TerminationEndtekiEdit(short nChar, short length, LPCTSTR data);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRadioSiwake();
	afx_msg void OnBnClickedRadioKamoku();
	afx_msg void OnBnClickedRadioEdaban();
	afx_msg void OnBnClickedRadioKamokuSyuukei();
	afx_msg void OnBnClickedRadioAlldataclear();
	afx_msg void OnBnClickedRadioValrendo();
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedKamokuButton();
	afx_msg void OnCbnSelchangeComboMakeupMethod();
protected:
	virtual void OnOK();
public:
	DECLARE_EVENTSINK_MAP()
	ICSDateSelect m_SelMonth;
	LRESULT OnDateSelectMessageChangeDATESELECT_RENDO(WPARAM wParam, LPARAM lParam);
	CComboBox m_cmbMakeupMethod;
protected:
	virtual void OnCancel();
public:
// midori 152153 add -->
	afx_msg void OnBnClickedEdarenCheck();
	afx_msg void OnBnClickedKmknameCheck();
// midori 152153 add <--
};

#endif	//	__RENMAIN_H__