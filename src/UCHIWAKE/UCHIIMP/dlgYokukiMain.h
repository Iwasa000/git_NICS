// dlgYokukiMain.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CdlgYokukiMain ダイアログ
#ifndef __YOKUKIMAIN_H__
#define __YOKUKIMAIN_H__

#pragma once
#include "afxwin.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfYokuki.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfSub.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc000Common.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc011Yotyokin.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc021Uketoritegata.h"	// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc031Urikakekin.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc041Karibarai.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc042Kasituke.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc051Tanaoroshi.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc052Tanaoroshi2.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc061Yuukasyouken.h"	// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc071Koteishisan.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc081Siharaitegata.h"	// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc091Kaikakekin.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc092Kaikakekin2.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc101Kariukekin.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc102Gensen.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc111Kariirekin.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc121Totiuriage.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc131Uriagedaka.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc141Yakuin.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc142Jinkenhi.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc151Tidaiyatin.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc152Kenrikin.h"		// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc153Kougyou.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc161Zatueki.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc162Zatuson.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc171Sonota1.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc181Sonota2.h"			// 
#include "..\..\UCHIWAKE\UCHIWAKE\UcFunctionCommon.h"
#include "icsbutton.h"
#include "icsreportcontrol.h"

static char strYokukiHeader[][64] = {
	_T("選択"),
	_T("様式番号"),
	_T("様式名称"),
	_T("金額等を\r\n０クリア"),
	_T("金額等を\r\n空欄にする"),
	_T("データを\r\n保管へ移動"),
};

enum enumIdColumnYokuki{	// レポートコントロールのカラムID
	yk_select = 0,
	yk_youshiki_no,
	yk_youshiki_name,
	yk_0clear,
	yk_nullclear,
	yk_move_hokan,
};

typedef struct tag_infoYokuki{
	BOOL	select;
	BOOL	zero_clear;
	BOOL	null_clear;
	BOOL	move_hokan;
	BYTE	m_OpYokukiFg;
	BYTE	m_OpClearFg;
	BYTE	m_OpNullClearFg;
	BYTE	m_OpHokanFg;
	BOOL	m_isEnable;		// 選択可/不可
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
	ICSButton m_btnNullClearChk;	//	「全票選択」ボタン
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
	int		m_flg;

private:
	void	InitReportHeader();
	int		AddRecords();
	int		CreateYokukiMap();
	void	SetCheckBoxState(int indexRow, BOOL state);
	void	SetData();
	void	UpdateYokukiVec(int row, BOOL flag);
	BOOL	SaveData();
	void	UpdateStateFromKey(int key);
	void	SetGrid();
	BOOL	IsExistData(int row);
	void	SetColor(int nIndex, BOOL check);

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
	int ShowDialog( CDatabase* pDB = NULL,int pFlg = 0 );
	void OnNMReportCheckedCUSTOM_REPORT2(NMHDR * pNotifyStruct, LRESULT * result);
	BOOL CheckYousikiData( int intFormSeq );
	afx_msg void OnBnClickedNullclearchkButton();
};

#endif	// __YOKUKIMAIN_H__
