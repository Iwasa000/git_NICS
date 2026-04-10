//--------------------------------------------------
//	dlgPrintAll.h
//
//	2006.02.19～
//--------------------------------------------------

#ifndef	__DLGPRINTALL_H__
#define	__DLGPRINTALL_H__

#pragma once
#include <map>
using namespace std;
#include "icsbutton.h"
#include "icshedit.h"

#include "dbUcInfFormType.h"
#include "dbUcInfSub.h"
#include "UcCommon.h"

#include "dlgPrintSetup.h"
#include "dbUc011Yotyokin.h"

#include "prtJobFunction.h"
#include "prtConstData.h"

#include "dlgAddressSearch.h"	//	住所検索クラス（※文字列操作メソッドのみ使用）
#include "icsreportcontrol.h"
#include "dbUcInfFormType.h"
#include "afxwin.h"

// [一括印刷]ダイアログにて取得する情報
typedef struct tagPrtInfDlgPrintAll{
	PRT_INF_LITE		Lite;				// 
	int					FgPrint;			// 印刷 0：しない／1：する
} PRT_INF_DLGPRINTALL;

// 帳表の総ページ数リスト
typedef struct tagPrtInfPageAll{
	int					FormSeq;			// 様式シーケンス番号
	int					PageAll;			// 帳表の総ページ数
} PRT_INF_PAGEALL;

// 様式シーケンスにぶら下がるコントロールたち
typedef struct tagPrtCtrl{
	int chk_box;
	int page_start;
	int page_end;
	int	title;
}PRT_CTRLS;

typedef struct PRINT_ALL_DATA_{
	PRINT_ALL_DATA_(){
		FormSeq = 0;
		FormNo = 0;
// midori 151393 add -->
		FormNo2 = 0;
// midori 151393 add <--
		TitleNo.Empty();
		Title.Empty();
		isSelect = FALSE;
		start = 0;
		end = 0;
		inputStart = 0;
		inputEnd = 0;
		str_start.Empty();
		str_end.Empty();
		isEnable = TRUE;
// midori 160610 add -->
		KamokuSitei = 0;
// midori 160610 add <--
// midori 20/10/08u add -->
		isMainasu = 0;
// midori 20/10/08u add <--
// midori 160608 add -->
		svstart = 0;
		svend = 0;
		start2 = 0;
		end2 = 0;
		inputStart2 = 0;
		inputEnd2 = 0;
		str_start2.Empty();
		str_end2.Empty();
		isEnable2 = TRUE;
		isEnableSgn = TRUE;
// midori 160608 add <--
// midori 190505 add -->
		SortChk = 0;
		SortChk2 = 0;
		inputCmb = -1;
		inputCmb2 = -1;
// midori 190505 add <--
// midori 20/10/08u add -->
		isMainasu2 = 0;
// midori 20/10/08u add <--
	}

// midori 20/10/08u del -->
//// midori 190505 del -->
////// midori 160608 cor -->
//////	PRINT_ALL_DATA_(BOOL isEnable, const CdbUcInfSub& mfcSub, int start, int end){
////// ---------------------
////	PRINT_ALL_DATA_(BOOL isEnable, const CdbUcInfSub& mfcSub, int start, int end,BOOL isEnableSgn){
////// midori 160608 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//	PRINT_ALL_DATA_(BOOL isEnable, const CdbUcInfSub& mfcSub, int start, int end, BOOL isEnableSgn, int isSortChk, int iCmb)	{
//// midori 190505 add <--
// midori 20/10/08u del <--
// midori 20/10/08u add -->
	PRINT_ALL_DATA_(BOOL isEnable, const CdbUcInfSub& mfcSub, int start, int end, BOOL isEnableSgn, int isSortChk, int iCmb, int mainasu)	{
// midori 20/10/08u add <--
		this->isEnable = isEnable;
		this->FormSeq = mfcSub.m_FormSeq;
		this->FormNo = mfcSub.m_FormNo;
// midori 151393 add -->
		this->FormNo2 = mfcSub.m_FormNo2;
// midori 151393 add <--
// midori 151404 add -->
		this->FormOrder = mfcSub.m_FormOrder;
// midori 151404 add <--
		this->TitleNo = mfcSub.m_TitleNo + mfcSub.m_TitleNo2;
		this->Title = mfcSub.m_Title;
		this->start = start;
		this->end = end;
		this->inputStart = this->start;
		this->inputEnd = this->end;
		this->str_start.Format("%d", this->start);
		this->str_end.Format("%d", this->end);
// midori 160610 add -->
		this->KamokuSitei = (mfcSub.m_GeneralVar[1] & BIT_D3);
// midori 160610 add <--
// midori 20/10/08u add -->
		this->isMainasu = mainasu;
// midori 20/10/08u add <--
// midori 160608 add -->
		this->svstart = this->start;
		this->svend = this->end;
		this->start2 = this->start;
		this->end2 = this->end;
		this->inputStart2 = this->start;
		this->inputEnd2 = this->end;
		this->str_start2.Format("%d", this->start);
		this->str_end2.Format("%d", this->end);
		this->isEnable2 = isEnable;
		this->isEnableSgn = isEnableSgn;
// midori 160608 add <--
// midori 190505 add -->
		this->SortChk = isSortChk;
		this->SortChk2 = isSortChk;
		this->inputCmb = iCmb;
		this->inputCmb2 = iCmb;
		this->formType = 0;
		this->isKEnable = TRUE;
// midori 20/10/08u del -->
		//this->sortEdit = TRUE;
// midori 20/10/08u del <--
// midori 190505 add <--

		this->isSelect = FALSE;
// midori 190505 add -->
		this->isSelect2 = FALSE;
// midori 190505 add <--
// midori 20/10/08u add -->
		this->isMainasu2 = mainasu;
// midori 20/10/08u add <--
	}

	BOOL operator==(int val){
		return this->FormSeq == val;
	}

	int			FormSeq;		// 様式シーケンス
	int			FormNo;			// 様式No
// midori 151393 add -->
	int			FormNo2;		// 様式No2
// midori 151393 add <--
// midori 151404 add -->
	int			FormOrder;		// 様式並び順番号
// midori 151404 add <--
	CString		TitleNo;		// 様式番号
	CString		Title;			// 様式タイトル
	BOOL		isSelect;		// 選択チェックボックス
	int			start;			// 開始ページ(int)
	int			end;			// 終了ページ(int)
	CString		str_start;		// 開始ページ(CString)
	CString		str_end;		// 終了ページ(CString)
	int			inputStart;		// 開始ページ（入力値）
	int			inputEnd;		// 終了ページ（入力値）
	BOOL		isEnable;		// 「選択」カラムの入力可／不可　（出力形式が通常の－２、－３はFALSE　データがない様式はFALSE　それ以外はTRUE）
// midori 190505 add -->
	int			inputCmb;		// 確認ダイアログの出力方法（入力値）
	int			SortChk;		// ChkIkkatuSort()関数のリターン値
// midori 190505 add <--
// midori 160610 add -->
	int			KamokuSitei;	// 科目指定
// midori 160610 add <--
// midori 190505 add -->
	int			formType;		// 出力形式 0:通常、1:個別（確認ダイアログで使用）
	int			isKEnable;		// 確認ダイアログ用、頁の入力制限
// midori 20/10/08u del -->
	//int			sortEdit;		// 確認ダイアログ用、出力方法の選択肢
// midori 20/10/08u del <--
// midori 190505 add <--
// midori 20/10/08u add -->
	int			isMainasu;		// 頁にハイフン表示を行う
// midori 20/10/08u add <--
// midori 160608 add -->
	int			svstart;		// 変更前開始ページ(int)
	int			svend;			// 変更前終了ページ(int)
	int			start2;			// 開始ページ(int)
	int			end2;			// 終了ページ(int)
	CString		str_start2;		// 開始ページ(CString)
	CString		str_end2;		// 終了ページ(CString)
	int			inputStart2;	// 開始ページ（入力値）
	int			inputEnd2;		// 終了ページ（入力値）
	int			isEnable2;		// 「選択」カラムの入力可／不可　（出力形式が通常の－２、－３はFALSE　データがない様式はFALSE　それ以外はTRUE）
	int			isEnableSgn;	// カラムの入力可/付不可サイン（「帳表イメージのみ出力する」にチェック有り）
// midori 190505 add -->
	int			inputCmb2;		// 確認ダイアログの出力方法（入力値）
	int			SortChk2;		// ChkIkkatuSort()関数のリターン値
	BOOL		isSelect2;		// 選択チェックボックス(通常タイプの場合、‐1 にチェックが付いている場合、
								// -2,-3 は isSelect が TRUE にならないので、isSelect2 を TRUE にする)
								// ※確認ダイアログ用
// midori 190505 add <--
// midori 20/10/08u add -->
	int			isMainasu2;		// 頁にハイフン表示を行う
// midori 20/10/08u add <--
// midori 160608 add <--
}PRINT_ALL_DATA;

// midori 151393 del -->
//typedef struct FORM_TYPE_DATA_{
//	FORM_TYPE_DATA_(){
//		int FormSeq = 0;
//		int FormType = 0;
//		int num = 2;
//	}
//	int FormSeq;
//	int FormType;
//	int num;
//}FORM_TYPE_DATA;
// midori 151393 del <--

static char strPrintAllHeader[][64] = {
	_T("選択"),
	_T("様式番号"),
	_T("様式名称"),
	_T("開始頁"),
	_T("終了頁"),
// midori 160610 add -->
	_T("科目指定"),
// midori 160610 add <--
};

enum enumIdColumnPrintAll{	// レポートコントロールのカラムID
	pa_select = 0,
	pa_youshiki_no,
	pa_youshiki_name,
	pa_start,
	pa_end,
// midori 160610 add -->
	pa_kamoku_sitei,
// midori 160610 add <--
};

class CdlgPrintAll : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgPrintAll)

public:
	//CdlgPrintAll(CWnd* pParent = NULL);				//	標準コンストラクタ
	CdlgPrintAll(CBatchPrt* pBatchPrt, CWnd* pParent = NULL);	//	標準コンストラクタ
	CdlgPrintAll(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgPrintAll();						//	デストラクタ

	enum { IDD = IDD_DIALOG_PRINTALL };
	ICSButton	m_btnHyoChk;
	ICSButton	m_btnPrintSetup;
// midori 160610 add -->
	ICSButton	m_btnKamoku;
// midori 160610 add <--
// midori 180101 add -->
	ICSButton	m_btnHyoshi;
// midori 180101 add <--

	CButton		m_chkTitle;
	CButton		m_chkNow;
// midori 160608 add -->
	CButton		m_chkZero;
// midori 160608 add <--
	ICSButton	m_btnCancel;
	ICSButton	m_btnOK;

private:
	CDatabase*	m_pDB;			//	データベースハンドル
	int			m_nRet;			//	戻値
	BOOL		m_blnPrnChk;	//	印刷可能帳表判断フラグ(TRUE:印刷可能帳表あり FALSE:印刷可能帳表無し)
	int			m_CheckNum;		//　-1:チェックなし/0:全てチェック/それ以外：様式Seq
	CUcFunctionCommon m_clsFunc;//	共通関数クラス
	PRT_INF_PAGEALL		m_PrtPageAllList[UC_ID_FORMNO_MAX+1];	// 帳票の総ページ数リスト
							// 表紙も含むため、様式最大数＋1
	PRT_INF_DLGPRINTALL	m_PrtWorkList[UC_ID_FORMNO_MAX+1];		// 作業用リスト
							// 表紙も含むため、様式最大数＋1

	// 2010/07/XX 一括印刷対応
	COwntbData*		m_pOwntbData;			// owntbクラス
	CBatchPrt*		m_pBatchPrt;			// BatchPrtクラス
	CDBZmSub*		m_pZmSub;				// 財務dbクラス
	vector<PRINT_ALL_DATA>		m_vecPrintAll;	// レポートコントロールのレコード情報を持つベクタ
	map<int, FORM_TYPE_DATA>	mapFormType;	// FormTypeのデータを持つマップ
	vector<int>					m_vecFormOrder;	// FormOrder順にFormSeqを持つベクタ
	int				m_nFormSeq;

private:
	void SetPageData(void);
	BOOL CheckPrintDataMain(CdbUcInfSub& mfcSub);
	BOOL CheckPrintDataSub( int nFormSeq );
	void SetAllOutputSpecify();

	void SetPageRangeOwntbData( void );							//	2010/07/XX 一括印刷対応　出力ページ範囲 設定（owntb参照）
	void SetOutputChkOwntbData( void );							//	2010/07/XX 一括印刷対応　出力ON/OFF 設定（owntb参照）
	void SetOutputChk(int& focus_row);							//  一括印刷以外のチェック状態セット
// midori 160913 add -->
	void SetOutputChk2(BOOL isYousiki[]);						//  印刷設定、科目設定でデータに影響のある項目が変更された場合、呼び出した様式のチェックを付ける
// midori 160913 add <--

	void	InitReportHeader();
	void	AddRecords();
	void	CreatePrintAllVec();
	BOOL	CreateFormTypeMap(CdbUcInfFormType& mfcFormType, CdbUcInfSub& mfcSub);
	void	UpdateFormTypePage();
	void	UpdateRecordState();
	void	SetColor( int nIndex, BOOL check);
// midori 156910 add -->
	void	SetColorItem(int nIndex, int nColumn, BOOL check);
// midori 156910 add <--
	void	UpdateStateFromKey(int key);
// midori 190505 del -->
	//void	SetCheckBoxState(int indexRow, BOOL state);
// midori 190505 del <--
// midori 190505 add -->
	void	SetCheckBoxState(int indexRow, BOOL state, int state2);
// midori 190505 add <--
	int		GetCheckedNum();
	int		CheckPageNum(int indexRow, int indexClm);
	void	UpdateVectorData();
	void	SetGrid();
	void	SetPage();
// midori 155525 del -->
	//int		GetRangaiPage(int FormSeq);
// midori 155525 del <--
// midori 155525 add -->
	int		GetRangaiPage( int FormSeq, int pOutZero );
// midori 155525 add <--

// midori 190505 add -->
//	一括印刷用の確認ダイアログを表示する
	int		CreateIkkatuConfDlg(void);
// midori 190505 add <--

public:
	int ShowDialog(int call_by, int FormSeq, CDatabase* pDB = NULL );

// midori 180101 del -->
//	int					m_nOutGenzai;						// 「現在」出力
// midori 180101 del <--
// midori 160608 add -->
	int					m_nOutZero;							// 「帳票イメージのみ出力する」
// midori 160608 add <--
	PRT_INF_LITE		m_PrtList[PRT_ID_FORMNO_MAX];		// 出力リスト

//	イベント系
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

//	イベント系
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTitleCheck();
	afx_msg void OnBnClickedAllselect();
	afx_msg void OnBnClickedAllcansel();
	afx_msg void OnBnClickedPrintsetup();
	virtual void OnBnClickedOK();
// midori 180101 del -->
//	afx_msg void OnBnClickedNowCheck();
// midori 180101 del <--

protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ICSReportControl m_report;
	afx_msg void OnBnClickedButtonHyoAll();
	
	void OnNMReportCheckedCUSTOM_REPORT_PRINTALL(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkeydownCUSTOM_REPORT_PRINTALL(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkillfocusCUSTOM_REPORT_PRINTALL(NMHDR * pNotifyStruct, LRESULT * result);
// midori 160608 add -->
	afx_msg void OnBnClickedZeroCheck();
// midori 160608 add <--
// midori 160610 add -->
	afx_msg void OnBnClickedButtonKamoku();
// midori 160610 add <--
// midori 180101 add -->
	afx_msg void OnBnClickedButtonHyoshi();
// midori 180101 add <--
};

#endif	//	__DLGPRINTALL_H__