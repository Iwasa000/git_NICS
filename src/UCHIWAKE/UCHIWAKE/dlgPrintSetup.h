//--------------------------------------------------
//	dlgPrintSetup.h
//
//	2006.02.19～
//--------------------------------------------------
#ifndef	__DLGPRINTSETUP_H__
#define	__DLGPRINTSETUP_H__

#pragma once
#include "afxwin.h"
#include "icsdisp.h"
#include "icsknjedit.h"
#include "icsbutton.h"

#include "ICSGENL8.h"
#include "dbUcInfMain.h"		//	uc_inf_mainテーブルクラス
#include "dbUcInfSub.h"			//	uc_inf_subテーブルクラス
#include "dbUcInfSubOmit.h"		//	uc_inf_sub_omitテーブルクラス
#include "UcCommon.h"			//	定数ヘッダ
#include "dlgAddressSearch.h"	//	住所検索クラス（※文字列操作メソッドのみ使用）

class CdlgPrintSetup : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgPrintSetup)

public:
	CdlgPrintSetup(CWnd* pParent = NULL);			//	標準コンストラクタ
	CdlgPrintSetup(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgPrintSetup();						//	デストラクタ

	enum { IDD = IDD_DIALOG_PRINTSETUP };
	CComboBox m_cboYoushiki;
	CICSDisp m_lblItem1;
	CICSDisp m_lblItem2;
	CICSDisp m_lblItem3;
	CICSDisp m_lblItem4;
	CICSDisp m_lblItem5;
	CICSDisp m_lblItem6;
	CButton m_rdoOn1;
	CButton m_rdoOn2;
	CButton m_rdoOn3;
	CButton m_rdoOn4;
	CButton m_rdoOn5;
	CButton m_rdoOn6;
	CButton m_rdoOff1;
	CButton m_rdoOff2;
	CButton m_rdoOff3;
	CButton m_rdoOff4;
	CButton m_rdoOff5;
	CButton m_rdoOff6;
	CICSKNJEdit m_txtOmission1;
	CICSKNJEdit m_txtOmission2;
	CICSKNJEdit m_txtOmission3;
	CICSKNJEdit m_txtOmission4;
	CICSKNJEdit m_txtOmission5;
	CICSKNJEdit m_txtOmission6;
	CButton m_chkCoName;
	CButton m_chkSettlement;
	CButton m_chkMoney;
// midori 200101 add -->
	CButton m_chkGengo;
// midori 200101 add <--
	CButton m_chkPage;
	CButton m_chkKamoku;
	CButton m_chkZero;
	CButton m_chkZeroHide;
	CButton m_chkNull;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
// midori 170105 add -->
	ICSButton m_btn1;
// midori 170105 add <--
	CButton m_chkKinyuLine;
// midori 190505 del -->
//	CButton m_chkKamokuRow;
// midori 190505 del <--
// midori 156188 add -->
	CButton m_chkKamokuRow;
// midori 156188 add <--

private:
	CDatabase*	m_pDB;				//	データベースハンドル
	short		m_nFormSeq;			//	様式シーケンス番号
	int			m_nRet;				//	戻値
	BOOL		m_isChangePageNum;	// 変更があった？　一括印刷ダイアログ用
// midori 160608 add -->
	int			m_nOutZero;			// 「帳票イメージのみ出力する」サイン -1:ﾒﾆｭｰから起動 0:「帳表イメージのみ出力する」チェック無 1:「帳表イメージのみ出力する」チェック有
// midori 160608 add <--
// midori 151404 add -->
	int			m_isYousiki[UC_ID_FORMNO_MAX];	// 様式毎の変更サイン TRUE:再表示が必要
// midori 151404 add <--

private:
	void InitYoushikiCombo( CDatabase* pDB, short nFormOrder );					//	様式コンボボックス初期化
	void ChangeYoushikiCombo( CDatabase* pDB, short nFormSeq );					//	様式コンボボックス変更
	void ChangeYoushikiComboSub( CDatabase* pDB, short nFormSeq );				//	様式コンボボックス変更（サブ）
// midori 170105 add -->
	void ChangeYoushikiComboSub2( CDatabase* pDB, short nFormSeq );				//	様式コンボボックス変更（サブ２）
// midori 170105 add <--
	void InitOmissionItem( BOOL fShow );										//	省略項目初期化
	void SetOmissionItem( CICSDisp* lblItem, CButton* prdoOn,	
						  CButton* prdoOff, CICSKNJEdit* ptxtOmission,
						  BOOL fShow );											//	省略項目設定
	void GetOmissionData( CdbUcInfSubOmit* pmfcRec, CICSDisp* lblItem, 
						  CButton* prdoOn, CButton* prdoOff,
						  CICSKNJEdit* ptxtOmission );							//	省略データ取得			
	void SetYoushikiData( CDatabase* pDB, short nFormSeq );						//	様式データ設定
	void SetYoushikiDataSub( CDatabase* pDB, short nFormSeq );					//	様式データ設定（サブ）
// midori 170105 add -->
	void SetYoushikiDataSub2( CDatabase* pDB, short nFormSeq );					//	様式データ設定（サブ２）
// midori 170105 add <--
// midori 157099,157119 add -->
	int YoushikiDataChk(CDatabase* pDB, short nFormSeq);
// midori 157099,157119 add <--
	void SetOmissionData( CdbUcInfSubOmit* pmfcRec, CICSDisp* lblItem, 
						  CButton* prdoOn, CButton* prdoOff, 
						  CICSKNJEdit* ptxtOmission );							//	省略データ設定
	int CheckYoushikiData( CDatabase* pDB, short nFormSeq );					//	様式データ確認
	int CheckYoushikiDataSub( CDatabase* pDB, short nFormSeq );					//	様式データ確認（サブ）
// midori 170105 add -->
	int CheckYoushikiDataSub2( CDatabase* pDB, short nFormSeq );				//	様式データ確認（サブ２）
// midori 170105 add <--
	int CheckOmissionData( CdbUcInfSubOmit* pmfcRec, CICSDisp* lblItem, 
						   CButton* prdoOn, CButton* prdoOff, 
						   CICSKNJEdit* ptxtOmission );							//	省略データ確認		
	void GetOutputSetup( CDatabase* pDB );										//	出力指定取得
	void SetOutputSetup( CDatabase* pDB );										//	出力指定設定
	int GetFormSeq( int nFormOrder );											//	シーケンス番号取得
	int GetFormOrder( int nFormSeq );											//	順序番号取得
	void CheckOmit( CICSKNJEdit* ptxtOmit, BOOL fOnOff );
	void SetChkZeroHideState();													// 「金額０円を印字する」コントロールの制御

	void UpdateShowKeiZero(int nFormSeq);										// frmUc000の計行更新を行う

public:
// midori 151404 cor -->
//// midori 160608 cor -->
////	int ShowDialog(BOOL& isChange, CDatabase* pDB = NULL, short nFormOrder = ID_FORMNO_011 );
//// ---------------------
//	int ShowDialog(BOOL& isChange, CDatabase* pDB = NULL, short nFormOrder = ID_FORMNO_011, int nOutZero = -1);
//// midori 160608 cor <--
// ---------------------
	int ShowDialog(BOOL& isChange, BOOL isYousiki[], CDatabase* pDB = NULL, short nFormOrder = ID_FORMNO_011, int nOutZero = -1 );
// midori 151404 cor <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnCbnSelchangeYoushikiCombo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOmissionOn1();
	afx_msg void OnBnClickedOmissionOff1();
	afx_msg void OnBnClickedOmissionOn2();
	afx_msg void OnBnClickedOmissionOff2();
	afx_msg void OnBnClickedOmissionOn3();
	afx_msg void OnBnClickedOmissionOff3();
	afx_msg void OnBnClickedOmissionOn4();
	afx_msg void OnBnClickedOmissionOff4();
	afx_msg void OnBnClickedOmissionOn5();
	afx_msg void OnBnClickedOmissionOff5();
	afx_msg void OnBnClickedOmissionOn6();
	afx_msg void OnBnClickedOmissionOff6();
	afx_msg void OnBnClickedZeroOutputCheck();
			void OnBnClickedMoneyCheck();
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// midori 170105 add -->
	afx_msg void OnBnClickedButton1();
// midori 170105 add <--
};

#endif	//	__DLGPRINTSETUP_H__