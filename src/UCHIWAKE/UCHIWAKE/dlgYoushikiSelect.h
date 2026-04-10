//--------------------------------------------------
//	dlgYoushikiSelect.h
//
//	2006.02.21～
//--------------------------------------------------

#ifndef	__DLGYOUSHIKISELECT_H__
#define	__DLGYOUSHIKISELECT_H__

#pragma once
#include "icslistinp.h"
#include "icsbutton.h"
#include "icsocx.h"

#include "dbUcInfSub.h"		//	uc_inf_subテーブルクラス
#include "dbUcInfMain.h"	//	uc_inf_mainテーブルクラス
#include "UcCommon.h"		//	定数ヘッダ
#include "icsreportcontrol.h"

static char strYoushikiSelectHeader[][64] = {
	_T("様式番号"),
	_T("様式名称"),
// midori 161108_2 add -->
	_T("保管"),
// midori 161108_2 add <--
};

typedef struct YOUSHIKI_SEL_{
	YOUSHIKI_SEL_(){
		FormSeq = this->FormSeq;
		hasData = this->hasData;
	}

	BOOL operator==(BOOL val){
		return this->hasData == val;
	}

	int		FormSeq;	// 様式Seq
	BOOL	hasData;	// TRUE:データある FALSE:データなし
}YOUSHIKI_SEL;

class CdlgYoushikiSelect : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgYoushikiSelect)

public:
	CdlgYoushikiSelect(CWnd* pParent = NULL);			//	標準コンストラクタ
	CdlgYoushikiSelect(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgYoushikiSelect();						//	デストラクタ

	enum { IDD = IDD_DIALOG_YOUSHIKISELECT };
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	ICSButton m_btnSkipDown;
	ICSButton m_btnSkipUp;

private:
	CDatabase*	m_pDB;			//	データベースハンドル
	int			m_nRet;			//	戻値
	long		m_nOldRow;		//	前回選択行
	BOOL		m_fYoushikiSkip;
	CUcFunctionCommon	m_clsFunc;				//	共通関数クラス
	vector<YOUSHIKI_SEL>	m_vecDataExist;		// キー:FormSeq　バリュー:データある/TRUE　ない/FALSE

public:
	short		m_FormSeq;		//	様式番号保持

private:
	int		GetFormSeq( CDatabase* pDB );					//	様式シーケンス番号取得
	int		SetFormSeq( CDatabase* pDB, short nFormOrder );	//	様式シーケンス番号設定
	int		GetFormOrder(CDatabase* pDB);					// 様式オーダー取得
	void	SetColor( int nIndex, COLORREF nColor );		//	色を設定
	void	ReDrawRowColor();								//	行の背景色を更新
	long	GetListIndex();									//	賞味のリストインデックス取得
	int		GetFocusControlID( CWnd* pParent = NULL );
	void	UpdateYoushikiMoveButton();						//　「前の様式へ」と「次の様式へ」のボタン監視
	void	InitReportHeader();								// レポートコントロールのヘッダ初期化
	void	AddRecords();									// 行追加
	void	SetDataExistVec();								// 各様式にデータがあるかどうかのベクタ
	int		GetRecordDataOrder(int index, BOOL mode);
	void	MoveSelectedRow(BOOL mode, BOOL move, int move_dis = 0);
	void	SetGrid();

public:
	int ShowDialog( int fYoushikiSkip, CDatabase* pDB = NULL );			//	ダイアログ表示

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	DECLARE_EVENTSINK_MAP()
public:
	void LButtonDblClickYoushikiList(short index);
public:
	afx_msg void OnBnClickedOk();
public:
	void SelChangeYoushikiList(short index);
public:
	void FocusedYoushikiList();
public:
	void LButtonClickYoushikiList(short index);
protected:
	virtual void OnOK();

public:
	afx_msg void OnBnClickedSkipupButton();
	afx_msg void OnBnClickedSkipdownButton();
	ICSReportControl m_report;
	void OnNMReportSelchangedCUSTOM_REPORT_YOUSHIKI_LIST(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMDblclkReportYoushikiList(NMHDR * pNotifyStruct, LRESULT * result);
};

#endif	//	__DLGYOUSHIKISELECT_H__