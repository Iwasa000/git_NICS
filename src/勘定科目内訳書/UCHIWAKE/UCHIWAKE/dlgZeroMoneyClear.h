/////////////////////////////////////////////////////////////////////////////
// CdlgZeroMoneyClear ダイアログ
#ifndef __ZEROMONEYCLEAR_H__
#define __ZEROMONEYCLEAR_H__

#pragma once

#include "afxwin.h"
#include "icsreportcontrol.h"

static char strZeroClearHeader[][64] = {
	_T("選択"),
	_T("様式番号"),
	_T("様式名称"),
	_T("金額等を０クリアする"),
	_T("金額等を空欄にする"),
};

typedef struct ZERO_CLEAR_DATA_{
	ZERO_CLEAR_DATA_(){
		FormSeq = 0;
		TitleNo.Empty();
		Title.Empty();
		is_select = FALSE;
		is0_clear = FALSE;
		isNULL_clear = FALSE;
		isEnable = FALSE;
	}

	int			FormSeq;		// 様式シーケンス
	CString		TitleNo;		// 様式番号
	CString		Title;			// 様式タイトル
	BOOL		is_select;		// 選択チェックボックス
	BOOL		is0_clear;		// 金額０円行を保管へ移動するチェック状態
	BOOL		isNULL_clear;	// 金額空欄行を保管へ移動するチェック状態
	BOOL		isEnable;		// 選択可/不可
}ZERO_CLEAR_DATA;

enum enumIdColumnZeroClear{	// レポートコントロールのカラムID
	zc_select = 0,
	zc_youshiki_no,
	zc_youshiki_name,
	zc_0clear,
	zc_null_clear,
};

// CdlgZeroMoneyClear ダイアログ

class CdlgZeroMoneyClear : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgZeroMoneyClear)

public:
	CdlgZeroMoneyClear(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgZeroMoneyClear();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ZEROMONEYCLEAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	ICSButton m_btnOK;				//	「実行」ボタン
	ICSButton m_btnCancel;			//	「ｷｬﾝｾﾙ[ESC]」ボタン
	ICSButton m_btnHyoChk;			//	「帳表選択」ボタン
	ICSButton m_btnZeroChk;			//	「０クリア全選択」ボタン
	ICSButton m_btnNullChk;			//	「空欄全選択」ボタン

	CLEAR_DATA	m_ClearData[UC_ID_FORMNO_MAX];	// この値をみて本体側で０円移動する

private:
	int						m_nRet;				// 戻り値
	CDatabase*				m_pDB;				// データベースハンドル
	int						m_nFormSeq;			// 様式シーケンス番号
	CUcFunctionCommon		m_clsFunc;			// 共通関数クラス
	ICSReportControl		m_report;
	vector<ZERO_CLEAR_DATA>	m_vecZeroClearData;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedHyochkButton();
	afx_msg void OnBnClickedZerochkButton();
	afx_msg void OnBnClickedNullchkButton();
	afx_msg void OnBnClickedOkButton();

public:
	BOOL CheckYousikiData( int intFormSeq );
	BOOL SetCheckData();
	void ChangeBtnText( ICSButton *pBtn, BOOL blnFlg );
	int ShowDialog( int intFormSeq, CDatabase* pDB = NULL );

private:
	void	InitReportHeader();
	int		AddRecords();
	void	CreateZeroClearVec();
	void	SetData();
	void	SetCheckBoxState(int indexRow, BOOL state);
	void	SetColor(int nIndex, BOOL check);
	void	UpdateStateFromKey(int key);
	void	UpdateZeroClearData();
	int		GetSelectCheckNum();
	void	SetGrid();
	BOOL	IsExistData(int row);
	void	MoveDown(int cur_row);

public:
	void OnNMReportCheckedCUSTOM_REPORT_ZERO_CLEAR(NMHDR * pNotifyStruct, LRESULT * result);
};

#endif //	__ZEROMONEYCLEAR_H__
