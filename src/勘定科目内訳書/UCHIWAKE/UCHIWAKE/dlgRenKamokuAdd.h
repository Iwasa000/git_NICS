#pragma once

#include "icsbutton.h"
#include "stdafx.h"
#include "afxwin.h"
#include "dbUcRenKamoku.h"	//　連動科目一覧テーブルクラス
#include "UcFunctionCommon.h"// 内訳書関数郡　共通宣言
#include "afxcmn.h"

// CdlgRenKamokuAdd ダイアログ

class CdlgRenKamokuAdd : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgRenKamokuAdd)

public:
	CdlgRenKamokuAdd(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgRenKamokuAdd();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_RENKAMOKUADD };
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	CTreeCtrl m_treeKamoku;
	ICSStatic m_lblMessage;

public:
	CString	m_Kncod;	// 財務科目コード
	CString m_Knnam;	// 財務科目名称
	CString m_Knkana;	// 財務科目カナ

private:
	CDBZmSub*	m_pZmSub;		//  財務DBクラスハンドル
	CDatabase*	m_pDB;			// データベースハンドル
	CUcFunctionCommon	m_clsFunc;	//	共通関数クラス
	int			m_nFormSeq;		// 様式シーケンス番号
	int			m_nRet;			// 戻り値
	RENZVOL_INFO* m_pRenInfo;	// 会社情報構造体
	CBitmap			m_bitImage[4];	// ツリービューに表示するビットマップイメージ
	CImageList		m_imgList;		// イメージリスト

public:
	int ShowDialog( CDatabase* pDB = NULL , RENZVOL_INFO* pRenInfo = NULL ,  int nFormSeq = ID_FORMNO_011 );	//	ダイアログ表示
	void SetTreeItem();
	void SetMeisaiKamoku( CTreeCtrl* pTree, HTREEITEM htree, CString kcod );
	int GetTreeData();
	void Apt_AscToBcd( char *buff, char *data, int leng );
	void hl_rev(BYTE* dp, int ln);
	CString GetHexToString( long lngCode );
	int CheckKcdStart( CString strkcd );
	int GetFocusControlID( CWnd* pParent = NULL );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOkButton();
protected:
	virtual void OnOK();
};
