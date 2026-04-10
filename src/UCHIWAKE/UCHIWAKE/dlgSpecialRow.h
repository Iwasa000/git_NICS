//--------------------------------------------------
//	dlgSpecialRow.h
//
//	2006.02.18～
//--------------------------------------------------

#ifndef	__DLGSPECIALROW_H__
#define	__DLGSPECIALROW_H__

#pragma once
#include "afxwin.h"
#include "icsknjedit.h"
#include "icsbutton.h"

#include "dbUcInfSub.h"	//	uc_inf_subテーブルクラス
#include "UcCommon.h"	//	定数ヘッダ
#include "icshedit.h"
#include "icsdisp.h"

class CdlgSpecialRow : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgSpecialRow)

public:
	CdlgSpecialRow(CWnd* pParent = NULL);			//	標準コンストラクタ
	CdlgSpecialRow(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgSpecialRow();						//	デストラクタ

	enum { IDD = IDD_DIALOG_SPECIALROW };
	CButton m_rdoSmall;
	CButton m_rdoMiddle;
	CICSKNJEdit m_txtSmall;
	CICSKNJEdit m_txtMiddle;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	CComboBox m_cboSmall;
	CComboBox m_cboMiddle;
	CButton m_rdoOther;
	CICSHedit m_txtOther;
	ICSStatic m_lblName;
	ICSStatic m_lblBracket;
	ICSStatic m_lblOther1;
	ICSStatic m_lblOther2;
// midori 190505 add -->
	CButton m_rdoTitle;
// midori 190505 add <--

private:
	short		m_nFormSeq;		//	様式シーケンス番号
	CDatabase*	m_pDB;			//	データベースハンドル
	int			m_nRet;			//	戻値
	CUcFunctionCommon m_clsFunc;//	共通関数クラス
public:
	short		m_FgFunc;		//	特殊行フラグ
								//		ID_FGFUNC_NULL			…	空行
								//		ID_FGFUNC_SYOKEI		…	小計
								//		ID_FGFUNC_CHUKEI		…	中計
								//		ID_FGFUNC_IKKATUMANUAL	…	一括金額（手動）
	CString		m_OpName;		//	選択された特殊行の名称（小計 or 中計 or 見出し）
	CString		m_OpLeft;		//	選択された括弧の左括弧
	CString		m_OpRight;		//	選択された括弧の右括弧
// midori 190505 add -->
	int			m_RowFgFunc;	//	現在選択されている行の特殊行フラグ
// midori 190505 add <--

private:
	void SetRadioButton( int nCheckID );						//	ラジオボタン設定
	void GetSpecialName( short nFormSeq, CDatabase* pDB );		//	特殊行の名称を取得
	int SetSpecialName( short nFormSeq, CDatabase* pDB );		//	特殊行の名称を設定
	void SetBracketKind( CComboBox*	pCombo );					//	コンボボックスに括弧種別設定
	CString GetOtherNumber( CString szNumber );					//	その他件数文字列取得

public:
// midori 190505 del -->
//	int ShowDialog( CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011 );
// midori 190505 del <--
// midori 190505 add -->
	int ShowDialog( CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011, int nRowFgFunc = ID_FGFUNC_NULL );
// midori 190505 add <--

//	イベント系
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

//	イベント系
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedRadioSmall();
public:
	afx_msg void OnBnClickedRadioMiddle();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedRadioOther();
// midori 190505 add -->
public:
	afx_msg void OnBnClickedRadioTitle();
// midori 190505 add <--
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif	//	__DLGSPECIALROW_H__