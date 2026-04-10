#pragma once

#include "afxwin.h"
#include "icsbutton.h"

//#include "ICSGENL8.h"
#include "UcCommon.h"			//	定数ヘッダ
#include "dbUcInfSub.h"			//	uc_inf_subテーブルクラス
#include "icsreportcontrol.h"

#define COLUMN_TSUJO		0
#define COLUMN_HOKAN		1

// CdlgDeleteDataEachYoushiki ダイアログ

class CdlgDeleteDataEachYoushiki : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgDeleteDataEachYoushiki)

public:
	CdlgDeleteDataEachYoushiki(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual		~CdlgDeleteDataEachYoushiki();
	virtual		BOOL OnInitDialog();

	int			ShowDialog(CDatabase* pDB, CMap<int, int, DELETE_YOUSHIKI, DELETE_YOUSHIKI> &map);

private:
	void		InitControls();
	int			GetData();
	void		ChangeColumnAll(int column);
	void		SetGrid();

private:
	CDatabase*	m_pDB;			//	データベースハンドル
	int			m_nRet;			//	戻値
	CMap<int, int, DELETE_YOUSHIKI, DELETE_YOUSHIKI> m_map;	// 様式削除マップ
	CUcFunctionCommon m_clsFunc;//	共通関数クラス

// ダイアログ データ
	enum { IDD = IDD_DIALOG_DELETE_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSReportControl m_report;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	ICSButton m_btn_All_Tsujo;
	ICSButton m_btn_All_Hokan;
	afx_msg void OnBnClickedButtonAllTujo();
	afx_msg void OnBnClickedButtonAllHokan();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
};
