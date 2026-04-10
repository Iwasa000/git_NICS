//--------------------------------------------------
//	dlgSort.h
//
//	2006.02.19～
//--------------------------------------------------

#ifndef	__DLGSORT_H__
#define	__DLGSORT_H__

#pragma once
#include "icscombobox.h"
#include "afxwin.h"
#include "icsknjedit.h"
#include "icshedit.h"
#include "icsbutton.h"

#include "dbUcLstItemSort.h"	//	uc_lst_item_sortテーブルクラス
#include "dbUcInfSub.h"			//	uc_inf_subテーブルクラス
#include "UcCommon.h"			//	定数ヘッダ
#include "icsdisp.h"
#include "dlgIkkatuAdvSetting.h"
// 20-0873 add -->
#include "dlgTorihikiAmount1.h"
#include "dlgTorihikiAmount2.h"
// 20-0873 add <--

//	ソートアイテム構造体
struct _SD_SORT_ITEM{
	int			FormSeq;		//	シーケンス番号
	BYTE		ItemSeq;		//	アイテム番号
	CString		ItemName;		//	アイテム名称
	BYTE		FgPage;			//	改頁フラグ
	BYTE		FgSykei;		//	自動小計フラグ
	BYTE		FgItem;			//	アイテム種別
	CString		OrderStr;		//	オーダークエリ
	CString		FieldSykei;		//	フィールドクエリ
	CString		FieldSykei2;	//	フィールドクエリ２
};

struct _IKKATU_ADV_DATA;	// プロトタイプ宣言
class CdlgSort : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgSort)

public:
	CdlgSort(CWnd* pParent = NULL);				//	標準コンストラクタ
	CdlgSort(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgSort();						//	デストラクタ

	enum { IDD = IDD_DIALOG_SORT };
	CComboBox m_cboSortOrder1;
	CComboBox m_cboSortOrder2;
	CComboBox m_cboSortOrder3;
	CButton m_chkKaiPage;
	CButton m_chkTotalAuto;
	CButton m_chkTotalOutput;
	CButton m_chkTotalSpace;
	CICSKNJEdit m_txtTotalName;
	CButton m_chkAllMoney;
	CICSHedit m_txtAllMoney;
// midori 151397 add -->
	ICSButton m_btnSAVE;
// midori 151397 add <--
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	CComboBox m_cboSmall;
	ICSButton m_btnReturn;
	CButton m_chkAllMoneyOutZero;
	ICSButton m_btnIkkatuAdvSet;
	CButton m_chkUseAdv;

// midori M-16120101 add -->
	CButton m_chkShitenCheck;
// midori M-16120101 add <--

// midori 190505 add -->
	CButton m_chkKmkTitle;
// midori 190505 add <--

// 20-0873 add -->
	ICSButton m_btnAmount;
// 20-0873 add <--

	ICSStatic m_lblSortOrder1;
	ICSStatic m_lblSortOrder2;
	ICSStatic m_lblSortOrder3;
	ICSStatic m_lblTotal;
	ICSStatic m_lblBracket;
	ICSStatic m_lblAllMoney;

private:
	short				m_nFormSeq;				//	様式シーケンス番号
	CDatabase*			m_pDB;					//	データベースハンドル
	int					m_nRet;					//	戻値
	BOOL				m_fSort;				//	ソート有効フラグ
	_SD_SORT_ITEM		m_udSortItem[9];		//	ソートアイテム構造体		// 改良No.21-0086,21-0529 del
	//_SD_SORT_ITEM		m_udSortItem[10];		//	ソートアイテム構造体		// 改良No.21-0086,21-0529 add
	CString				m_OpIkkatuValOrg;		//	閾値の初期値
	CUcFunctionCommon	m_clsFunc;				//	共通関数フラグ
	//int				m_SortItemSeqID[9];		// ソートアイテムID配列			// 改良No.21-0086,21-0529 del
	int					m_SortItemSeqID[10];	// ソートアイテムID配列			// 改良No.21-0086,21-0529 add
	BOOL				m_isAdvSet;				// 詳細設定を使用する
	IkkatuValHandler	m_IkkatuValHandler;
// midori 152137 add -->
	int					m_BankSort;				//	参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
	int					m_AddressSort;			//	参照ダイアログ::表示順（取引先）	0:番号順	1:50音順
// midori 152137 add <--

public:
	BYTE	m_Sort1;			//	第一ソート項目
	BYTE	m_Sort2;			//	第二ソート項目
	BYTE	m_Sort3;			//	第三ソート項目
	BYTE	m_OpKaiPage;		//	改頁
// midori 190505 add -->
	BYTE	m_OpKmkTitle;		//	科目行を挿入する
// midori 190505 add <--
	BYTE	m_OpSykeiAuto;		//	自動小計
	BYTE	m_OpOutSyokei;		//	一行小計
	BYTE	m_OpOutNullLine;	//	小計行の次に空行を挿入する
	CString	m_OpSykeiName;		//	小計名称
	BYTE	m_OpSykeiKakko;		//	小計の括弧
	BYTE	m_OpOutZero;		//	0円仕訳
	BYTE	m_OpIkkatu;			//	一括金額
	CString	m_OpIkkatuVal;		//	一括金額閾値
	int		m_OpIkkatuOutZero;	//　一括金額に０円を含まない

	CString	m_OpLeft;			//	選択された括弧の左括弧
	CString	m_OpRight;			//	選択された括弧の右括弧

// midori 151397 add -->
	int		m_SortSaveSign;		//	エンドサイン 0:実行ボタン 1:保存ボタン -1:キャンセルボタン
// midori 151397 add <--

private:
	int SetSortItem( short nFormSeq, CDatabase* pDB );				//	ソートアイテム設定
	void GetSortData( short nFormSeq, CDatabase* pDB, BOOL fSort );	//	ソートデータ取得
	int SetSortData( short nFormSeq, CDatabase* pDB );				//	ソートデータ保存
	void SelectSortOrder( CComboBox* pcboSort, int nIndex );		//	ソート順選択
	void CheckSortOrder();											//	ソート順チェック
	void CheckKaiPage();											//	改頁チェック
// midori 190505 add -->
	void CheckKamokuTitle(void);									//	「科目行を挿入する」の制御切替
// midori 190505 add <--
	void CheckAllMoney();											//	一括金額チェック
	void SetBracketKind();											//	コンボボックスに括弧種別設定
	void SetSortItemEnable( BOOL fFlag, BOOL fAll = TRUE );			//	ソート項目の有効／無効設定
	void InitSortItem( _SD_SORT_ITEM* pudData );
	int CheckInputData();
	CString	GetSortItemFieldString( int cSortItemNo );
	void InitSortItemSeq(int*);
	void InitSortItemSeq_Inv(int* sortitemseq);				// インボイス登録番号追加対応_23/11/06 add
	void SetMap(BOOL isDefault);
	void SetTable();

public:
// midori 152137 del -->
//	int ShowDialog( int IkkatuOutZero, CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011 );	//	ダイアログ表示
// midori 152137 del <--
// midori 152137 add -->
	int ShowDialog( int IkkatuOutZero, CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011, int banksort = 0, int addresssort = 0 );	//	ダイアログ表示
// midori 152137 add <--
	CString GetIkkatuVal(long KnSeq);

// 20-0873 add -->
	int IsTorihikiAmount( int pFormSeq );
// 20-0873 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeSortorderCombo1();
public:
	afx_msg void OnCbnSelchangeSortorderCombo2();
public:
	afx_msg void OnCbnSelchangeSortorderCombo3();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedAllmoneyCheck();		
public:
	afx_msg void OnBnClickedReturnButton();
public:
	afx_msg void OnBnClickedTotalAuto();	
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnBnClickedButtonAdvancedSetting();
	afx_msg void OnBnClickedButtonCheckUseAdvSetting();
// midori M-16120101 add -->
	afx_msg void OnBnClickedKaipageCheck();
// midori M-16120101 add <--
// midori 151397 add -->
	afx_msg void OnBnClickedSaveButton();
// midori 151397 add <--
// 20-0873 add -->
	afx_msg void OnBnClickedButtonAmount();
// 20-0873 add <--
};

#endif	//	__DLGSORT_H__