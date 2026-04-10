// CdlgIkkatuHenkou ダイアログ
#ifndef __IKKATUHENKOU_H__
#define __IKKATUHENKOU_H__

#pragma once

#include "afxwin.h"
#include "icsreportcontrol.h"

static char strIkkatuHenkouHeader[][64] = {
	_T("様式番号"),
	_T("様式名称"),
	_T("０円データを出力しない"),
	_T("空欄データを出力しない"),
	_T("０円を印字する"),
};

typedef struct IKKATU_HENKOU_DATA_{
	IKKATU_HENKOU_DATA_(){
		FormSeq = 0;
		TitleNo.Empty();
		Title.Empty();
		is_select = FALSE;
		is0_henkou = FALSE;
		isNULL_henkou = FALSE;
		is0pr_henkou = FALSE;
	}

	int			FormSeq;		// 様式シーケンス
	CString		TitleNo;		// 様式番号
	CString		Title;			// 様式タイトル
	BOOL		is_select;		// 選択チェックボックス
	BOOL		is0_henkou;		// ０円データを出力しない	チェック状態
	BOOL		isNULL_henkou;	// 空欄行データを出力しない	チェック状態
	BOOL		is0pr_henkou;	// ０円を印字する			チェック状態
}IKKATU_HENKOU_DATA;

enum enumIdColumnIkkatuHenkou{	// レポートコントロールのカラムID
	ih_youshiki_no0,	// 様式番号
	ih_youshiki_name,	// 様式名称
	ih_0henkou,			// ０円データを出力しない
	ih_null_henkou,		// 空欄データを出力しない
	ih_0prhenkou,		// ０円を印字する
};

// CdlgIkkatuHenkou ダイアログ

class CdlgIkkatuHenkou : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgIkkatuHenkou)

public:
	CdlgIkkatuHenkou(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgIkkatuHenkou();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_IKKATUHENKOU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	ICSButton m_btnNullChk;			//	「空欄データ全選択/解除」ボタン
	ICSButton m_btnZeroChk;			//	「0円データ全選択」ボタン
	ICSButton m_btnZeroPrChk;		//	「0円印字全選択/解除」ボタン
	ICSButton m_btnOK;				//	「OK」ボタン
	ICSButton m_btnCancel;			//	「ｷｬﾝｾﾙ[ESC]」ボタン

private:
	int						m_nRet;				// 戻り値
	CDatabase*				m_pDB;				// データベースハンドル
	int						m_nFormSeq;			// 様式シーケンス番号
	int						m_nOutKingaku;		// 金額欄を空欄で出力するサイン取得
	CUcFunctionCommon		m_clsFunc;			// 共通関数クラス
	ICSReportControl		m_report;
	vector<IKKATU_HENKOU_DATA>	m_vecIkkatuHenkouData;
	BOOL					m_isChangePageNum;	// 変更があった？　一括印刷ダイアログ用
	int						m_isYousiki[UC_ID_FORMNO_MAX];	// 様式毎の変更サイン TRUE:再表示が必要

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	BOOL SetCheckData();
	int ShowDialog( BOOL& isChange, BOOL isYousiki[], int intFormSeq, CDatabase* pDB, int intOutKingaku );

private:
	void	InitReportHeader();
	int		AddRecords();
	void	CreateIkkatuHenkouVec();
	void	SetData();
	void	UpdateStateFromKey(int key);
	void	UpdateZeroClearData();
	void	SetGrid();
	void	MoveDown(int cur_row);

public:
	void OnNMReportCheckedCUSTOM_REPORT_IKKATU_HENKOU(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnBnClickedZerochkButton();
	afx_msg void OnBnClickedNullchkButton();
	afx_msg void OnBnClickedZeroprchkButton();
	afx_msg void OnBnClickedOkButton();
};

#endif //	__IKKATUHENKOU_H__
