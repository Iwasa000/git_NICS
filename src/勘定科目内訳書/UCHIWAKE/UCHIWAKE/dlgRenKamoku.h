// dlgRenTekiyo.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CdlgRenTekiyo ダイアログ
#ifndef __RENKAMOKU_H__
#define __RENKAMOKU_H__

#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "icslistinp.h"
#include "icsdisp.h"
#include "icsbutton.h"
#include "icsocx.h"
#include "icsgenl8.h"
//#include "DBKmksel8.h"	//	※仕様変更により必要なくなった
#include "tbReference.h"
#include "dbUcRenKamoku.h"	//　連動科目一覧テーブルクラス
#include "dbUcLstKamoku.h"	//	内訳書科目テーブルクラス
#include "UcCommon.h"		//	定数ヘッダ
#include "UcFunctionCommon.h"// 内訳書関数郡　共通宣言
#include "dlgRenKamokuAdd.h" // 財務科目追加ダイアログ
#include "dlgRenKamokuSelect.h"	// 内訳書科目選択ダイアログ

class CdlgRenKamoku : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgRenKamoku)

public:
	CdlgRenKamoku(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgRenKamoku();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_RENKAMOKU };
	CICSListInp m_lstKamoku;	// 科目リスト
	ICSButton m_btnCancel;		// 「閉じる[ESC]」ボタン
// 20-0451 add -->
	ICSButton m_btnReset;		// 「初期化」ボタン
// 20-0451 add <--
private:
	CtbReference	m_tbrToolBar;	//	ツールバー
	CDBZmSub*		m_pZmSub;		//  財務DBクラスハンドル
	CDatabase*		m_pDB;			//	データベースハンドル
	int 			m_nFormSeq;		//	様式シーケンス番号
	RENZVOL_INFO*	m_pRenInfo;		//  会社情報構造体のポインタ
	int				m_nRet;			//	戻値
	int				m_nColumnMax;	//	最大カラム数
	long			m_nOldRow;		//	前回選択行

	CUcFunctionCommon	m_clsFunc;	//	共通関数クラス

public:
	BYTE m_UcKmkEntry;	// 新規内訳書科目追加確認フラグ

public:
	int ShowDialog( CDatabase* pDB = NULL , RENZVOL_INFO* pRenInfo = NULL ,  int nFormSeq = ID_FORMNO_011 );	//	ダイアログ表示
	void OnToolButtonF7();
	void OnToolButtonF9();
	int SetToolBar();
	void InitList();
	void GetListData();
	long GetListIndex();
	void MovePosition( long nTop, long nIndex );
	CString GetKamokuRecord( long nKnSeq );
	void InitLinpData( LINP_DATA* pData );
	void CheckNowRowPosition();
	void SetColor( int nIndex, COLORREF nFont, COLORREF nBack );
	void ReDrawRowColor();
	long GetFgDft( int nIndex );
	int GetFocusControlID( CWnd* pParent = NULL);
	BOOL GetKamokuEnable( CDBZmSub* ZmSub , CString strkncod );
	CString GetKamokuName( CDBZmSub* ZmSub , CString strkncod );
//	CString GetKamokuCode( CDBZmSub* ZmSub );
	long SetRecordDelete();
	long GetSeq( int nIndex );
// 20-0451 add -->
	void RecordReset( int nFormSeq, BYTE nCoType);
// 20-0451 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SelChangeKamokuList(short index);
	void LButtonClickKamokuList(short index);
	void FocusedKamokuList();

protected:
	virtual void OnOK();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	DECLARE_EVENTSINK_MAP()
// 20-0451 add -->
	afx_msg void OnBnClickedButtonReset();
// 20-0451 add <--
};

#endif	//	__RENKAMOKU_H__
