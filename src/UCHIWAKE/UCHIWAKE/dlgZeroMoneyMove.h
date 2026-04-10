// dlgYoshikiMessage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CdlgYoshikiMessage ダイアログ
#ifndef __ZEROMONEYMOVE_H__
#define __ZEROMONEYMOVE_H__

#pragma once

#include "afxwin.h"
#include "icsreportcontrol.h"

static char strZeroMoveHeader[][64] = {
	_T("選択"),
	_T("様式番号"),
	_T("様式名称"),
	_T("金額０円行"),
	_T("金額空欄行"),
};

typedef struct ZERO_MONEY_DATA_{
	ZERO_MONEY_DATA_(){
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
}ZERO_MONEY_DATA;

enum enumIdColumnZeroMove{	// レポートコントロールのカラムID
	zm_select = 0,
	zm_youshiki_no,
	zm_youshiki_name,
	zm_0clear,
	zm_null_clear,
};


// CdlgZeroMoneyMove ダイアログ

class CdlgZeroMoneyMove : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgZeroMoneyMove)

public:
	CdlgZeroMoneyMove(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgZeroMoneyMove();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ZEROMONEYMOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	ICSButton m_btnOK;				//	「実行」ボタン
	ICSButton m_btnCancel;			//	「ｷｬﾝｾﾙ[ESC]」ボタン
	ICSButton m_btnHyoChk;			//	「帳表選択」ボタン
	ICSButton m_btnZeroChk;			//	「０円移動全選択」ボタン
	ICSButton m_btnNullChk;			//	「空欄移動全選択」ボタン

	HOKAN_DATA	m_HokanData[UC_ID_FORMNO_MAX];	// この値をみて本体側で０円移動する

private:
	int		m_nRet;			// 戻り値

	CDatabase* m_pDB;		// データベースハンドル
	int		m_nFormSeq;		// 様式シーケンス番号
	CUcFunctionCommon		m_clsFunc;			//	共通関数クラス
	ICSReportControl m_report;
	vector<ZERO_MONEY_DATA>	m_vecZeroMoveData;

public:

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedHyochkButton();
	afx_msg void OnBnClickedZerochkButton();
	afx_msg void OnBnClickedNullchkButton();
//	afx_msg void OnBnClickedAllcancel();
	afx_msg void OnBnClickedOkButton();

public:
	BOOL CheckYousikiData( int intFormSeq );
	BOOL SetCheckData();
	void ChangeBtnText( ICSButton *pBtn, BOOL blnFlg );
	int ShowDialog( int intFormSeq, CDatabase* pDB = NULL );

private:
	void	InitReportHeader();
	int		AddRecords();
	void	CreateZeroMoveVec();
	void	SetData();
	void	SetCheckBoxState(int indexRow, BOOL state);
	void	SetColor(int nIndex, BOOL check);
	void	ReDrawRowColor(BOOL check);
	void	UpdateStateFromKey(int key);
	void	UpdateZeroMoveData();
	int		GetSelectCheckNum();
	void	SetGrid();
	BOOL	IsExistData(int row);
	void	MoveDown(int cur_row);

public:
	void OnNMReportCheckedCUSTOM_REPORT_ZERO_MOVE(NMHDR * pNotifyStruct, LRESULT * result);
};

#endif //	__ZEROMONEYMOVE_H__
