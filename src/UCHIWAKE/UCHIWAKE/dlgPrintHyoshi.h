// midori 180101 add -->
//--------------------------------------------------
//	dlgPrintHyoshi.h
//
//	2018.01.12～
//--------------------------------------------------

#ifndef	__DLGPRINTHYOSHI_H__
#define	__DLGPRINTHYOSHI_H__

#pragma once
#include "icsbutton.h"
#include "CDICSDBEDT8.h"
//#include "ICSZipAd8.h"			//	住所検索用ヘッダ
#include "afxwin.h"

// CdlgPrintHyoshi ダイアログ

class CdlgPrintHyoshi : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgPrintHyoshi)

public:
	CdlgPrintHyoshi(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgPrintHyoshi();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRINTHYOSHI };

	CDICSDBEDT8			m_HSDE010;			// 期
	CButton				m_HSCK010;			// 期を出力する
	CDICSDBEDT8			m_HSDE020;			// 自・年
	CDICSDBEDT8			m_HSDE021;			// 自・月
	CDICSDBEDT8			m_HSDE022;			// 自・日
	CDICSDBEDT8			m_HSDE023;			// 至・年
	CDICSDBEDT8			m_HSDE024;			// 至・月
	CDICSDBEDT8			m_HSDE025;			// 至・日
	CButton				m_HSCK020;			// 「現在」を出力する
	CDICSDBEDT8			m_HSDE030;			// 会社名
	CDICSDBEDT8			m_HSDE040;			// 住所（上段）
	CDICSDBEDT8			m_HSDE041;			// 住所（下段）
	CComboBox			m_HSCB050;			// 暦年表示
// midori 181101 add -->
	CComboBox			m_HSCB020;			// 元号（自）
	CComboBox			m_HSCB030;			// 元号（至）
// midori 181101 add <--
	ICSButton			m_btnHome;			// 住所検索ボタン
	ICSButton			m_btnOK;			// OKボタン
	ICSButton			m_btnCancel;		// キャンセルボタン

private:
	CDatabase*			m_pDB;				// データベースハンドル
	int					m_nRet;				// 戻値
	CUcFunctionCommon	m_clsFunc;			// 共通関数クラス
// midori 151730,151263 class del -->
	//CDBZmSub*			m_pZmSub;			// 財務dbクラス
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
	CDBNpSub*			m_pZmSub;			// 財務dbクラス
// midori 151730,151263 class add <--

	PRT_INF_COMMON		m_PrtInfCommon;		// 内訳書の共通の情報

// midori 191201 add -->
// midori 151730,151263 del -->
//	int					m_ChkReki;			// 非営利法人暦年対応サイン		1:非営利法人かつ暦年対応済み
// midori 151730,151263 del <--
	int					m_RekiSave;			// 起動時の暦年表示
	int					m_RekiSave2;		// 変更前の暦年表示
// midori 191201 add <--

// midori 180104 add -->
	long				m_ChgFlg;			// 変更フラグ
											// D0：会社名, D1：住所（上段）, D2：住所（下段）
	CString				m_CoName;			// 起動時の会社名
	CString				m_CoAddr1;			// 　〃　の住所（上段）
	CString				m_CoAddr2;			// 　〃　の住所（下段）
// midori 180104 add <--
// midori 151730,151263 add -->
	BYTE				m_apno;				// アプリケーションNo
	int					m_ver;				// マスターバージョン
// midori 151730,151263 add <--

public:
	int ShowDialog( CDatabase* pDB = NULL );
// midori 180104 add -->
	int	CmpStrWithoutBspace( CString *pCStr1, char *pStr2 );
	int	PutCoinfo( void );
// midori 180104 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

//	イベント系
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void OnOK();

	int			InitDisp(void);			// 初期設定
	int			ReadData(void);			// データ読込み
	void		SetData(void);			// 画面表示

	void		GetData(void);			// 画面取得
	int			WriteData(void);		// データ書込み

	// 文字列の後ろからあるだけスペースカット
	int			LstKjsCut(char sbuf[], char dbuf[], int kjmx);
	// 移動
	void		MoveCursel(long nChar, long inplen, long kst, int nID);

public:
	DECLARE_EVENTSINK_MAP()
	void TerminationHsde010(long nChar, long inplen, long kst);
	void TerminationHsde020(long nChar, long inplen, long kst);
	void TerminationHsde021(long nChar, long inplen, long kst);
	void TerminationHsde022(long nChar, long inplen, long kst);
	void TerminationHsde023(long nChar, long inplen, long kst);
	void TerminationHsde024(long nChar, long inplen, long kst);
	void TerminationHsde025(long nChar, long inplen, long kst);
	void TerminationHsde030(long nChar, long inplen, long kst);
	void TerminationHsde040(long nChar, long inplen, long kst);
	void TerminationHsde041(long nChar, long inplen, long kst);
	afx_msg void OnBnClickedHomeButton();
	virtual void OnBnClickedOK();
	afx_msg void OnCbnSelchangeHscb050();
};

#endif	//	__DLGPRINTHYOSHI_H__
// midori 180101 add <--
