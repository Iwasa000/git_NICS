// dlgYokukiMain.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CdlgYokukiMain ダイアログ
#ifndef __YOKUKIMAIN_H__
#define __YOKUKIMAIN_H__

#pragma once

#include "afxwin.h"
#include "dbUcInfYokuki.h"
#include "dbUcInfSub.h"
#include "dbUc000Common.h"			// 
#include "dbUc011Yotyokin.h"		// 
#include "dbUc021Uketoritegata.h"	// 
#include "dbUc031Urikakekin.h"		// 
#include "dbUc041Karibarai.h"		// 
#include "dbUc042Kasituke.h"		// 
#include "dbUc051Tanaoroshi.h"		// 
#include "dbUc052Tanaoroshi2.h"		// 
#include "dbUc061Yuukasyouken.h"	// 
#include "dbUc071Koteishisan.h"		// 
#include "dbUc081Siharaitegata.h"	// 
#include "dbUc091Kaikakekin.h"		// 
#include "dbUc092Kaikakekin2.h"		// 
#include "dbUc101Kariukekin.h"		// 
#include "dbUc102Gensen.h"			// 
#include "dbUc111Kariirekin.h"		// 
#include "dbUc121Totiuriage.h"		// 
#include "dbUc131Uriagedaka.h"		// 
#include "dbUc141Yakuin.h"			// 
#include "dbUc142Jinkenhi.h"		// 
#include "dbUc151Tidaiyatin.h"		// 
#include "dbUc152Kenrikin.h"		// 
#include "dbUc153Kougyou.h"			// 
#include "dbUc161Zatueki.h"			// 
#include "dbUc162Zatuson.h"			// 
#include "dbUc171Sonota1.h"			// 
#include "dbUc181Sonota2.h"			// 
#include "icsbutton.h"
#include "icsreportcontrol.h"

static char strYokukiHeader[][64] = {
	_T("選択"),
	_T("様式番号"),
	_T("様式名称"),
// midori 152387 del -->
//	_T("金額等を０クリア"),
//	_T("データを保管へ移動"),
// midori 152387 del <--
// midori 152387 add -->
	_T("金額等を\r\n０クリア"),
	_T("金額等を\r\n空欄にする"),
	_T("データを\r\n保管へ移動"),
// midori 152387 add <--
};

enum enumIdColumnYokuki{	// レポートコントロールのカラムID
	yk_select = 0,
	yk_youshiki_no,
	yk_youshiki_name,
	yk_0clear,
// midori 152387 add -->
	yk_nullclear,
// midori 152387 add <--
	yk_move_hokan,
};

typedef struct tag_infoYokuki{
	BOOL	select;
	BOOL	zero_clear;
// midori 152387 add -->
	BOOL	null_clear;
// midori 152387 add <--
	BOOL	move_hokan;
	BYTE	m_OpYokukiFg;
	BYTE	m_OpClearFg;
// midori 152387 add -->
	BYTE	m_OpNullClearFg;
// midori 152387 add <--
	BYTE	m_OpHokanFg;
// midori 160605 add -->
	BOOL	m_isEnable;		// 選択可/不可
// midori 160605 add <--
}infoYokuki;

// CdlgYokukiMain ダイアログ
class CdlgYokukiMain : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgYokukiMain)

public:
	CdlgYokukiMain(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgYokukiMain();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_YOKUKI_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	ICSButton m_btnOK;				//	「実行」ボタン
	ICSButton m_btnCancel;			//	「ｷｬﾝｾﾙ[ESC]」ボタン
	ICSButton m_btnHyoChk;			//	「全票選択」ボタン
	ICSButton m_btnClearChk;		//	「全票選択」ボタン
// midori 152387 add -->
	ICSButton m_btnNullClearChk;	//	「全票選択」ボタン
// midori 152387 add <--
	ICSButton m_btnHokanChk;		//	「全票選択」ボタン

private:
	BOOL	m_blnHyoChk;	// 帳表チェックの全選択(1)or全解除(0)確認フラグ
	BOOL	m_blnClearChk;	// 0クリアチェックの全選択(1)or全解除(0)確認フラグ
	BOOL	m_blnHokanChk;	// 一時保管チェックの全選択(1)or全解除(0)確認フラグ
	int		m_nRet;				// 戻り値
	CDatabase*			m_pDB;				// データベースハンドル
	CUcFunctionCommon	m_clsFunc;			//	共通関数クラス
	ICSReportControl	m_report;
	vector<infoYokuki>	vecInfoYokuki;
// midori 161103 add -->
	int		m_flg;
// midori 161103 add <--

private:
	void	InitReportHeader();
	int		AddRecords();
// midori 160605 cor -->
	//void	CreateYokukiMap();
// ---------------------
	int		CreateYokukiMap();
// midori 160605 cor <--
	void	SetCheckBoxState(int indexRow, BOOL state);
	void	SetData();
	void	UpdateYokukiVec(int row, BOOL flag);
	BOOL	SaveData();
	void	UpdateStateFromKey(int key);
	void	SetGrid();
// midori 160605 add -->
	BOOL	IsExistData(int row);
	void	SetColor(int nIndex, BOOL check);
// midori 160605 add <--

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedHyochkButton();
	afx_msg void OnBnClickedClearchkButton();
	afx_msg void OnBnClickedHokanchkButton();
	afx_msg void OnBnClickedOkButton();

public:
	int GetFocusControlID( CWnd* pParent = NULL );
	int SetMoveControlFocus( int intID , int intArrow );
// midori 161103 cor -->
//	int ShowDialog( CDatabase* pDB = NULL );
// ---------------------
	int ShowDialog( CDatabase* pDB = NULL,int pFlg = 0 );
// midori 161103 cor <--
	void OnNMReportCheckedCUSTOM_REPORT2(NMHDR * pNotifyStruct, LRESULT * result);
// midori 160605 add -->
	BOOL CheckYousikiData( int intFormSeq );
// midori 160605 add <--
// midori 152387 add -->
	afx_msg void OnBnClickedNullclearchkButton();
// midori 152387 add <--
};

#endif	// __YOKUKIMAIN_H__
