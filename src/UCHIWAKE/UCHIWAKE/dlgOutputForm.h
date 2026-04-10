//--------------------------------------------------
//	dlgOutputForm.h
//
//	2006.02.18～
//--------------------------------------------------

#ifndef	__DLGOUTPUTFORM_H__
#define	__DLGOUTPUTFORM_H__

#pragma once
#include "icsdisp.h"
#include "afxwin.h"
#include "icsbutton.h"

#include "dbUcInfSub.h"			//	uc_inf_subテーブルクラス
#include "dbUcInfFormType.h"	//	uc_inf_form_typeテーブルクラス
#include "UcCommon.h"			//	定数ヘッダ
#include "dlgAddressSearch.h"	//	住所検索クラス（※文字列操作メソッドのみ使用）

class CdlgOutputForm : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgOutputForm)

public:
	CdlgOutputForm(CWnd* pParent = NULL);			//	標準コンストラクタ
	CdlgOutputForm(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgOutputForm();						//	デストラクタ

	enum { IDD = IDD_DIALOG_OUTPUTFORM };
	
	//--------------------------------------------------
	//	コントロールに割り当てられている変数
	//--------------------------------------------------
	ICSStatic m_YoushikiLabel1;	//	様式名称ラベル１
	ICSStatic m_YoushikiLabel2;	//	様式名称ラベル２
	ICSStatic m_YoushikiLabel3;	//	様式名称ラベル３
	CButton m_rdoNormal;		//	通常ラジオボタン
	CButton m_rdoOne;			//	個別ラジオボタン
	ICSButton m_btnOK;			//	ＯＫボタン
	ICSButton m_btnCancel;		//	キャンセルボタン

private:
	CDatabase*	m_pDB;			//	データベースハンドル
	short		m_nFormSeq;		//	様式シーケンス番号
	int			m_nRet;			//	戻値

public:
	BYTE		m_FormType;		//	出力形式
								//		ID_FORMTYPE_NORMAL	…	通常
								//		ID_FORMTYPE_KOBETU	…	個別

private:
	void SetLabel( CDatabase* pDB, short nFormSeq );							//	ラベル設定
	void GetYoushikiTitle( CDatabase* pDB, short nFormSeq, CString* pszTitle );	//	様式タイトル取得
	void GetFormType( CDatabase* pDB, short nFormSeq );							//	出力形式取得
	int SetFormType( CDatabase* pDB, short nFormSeq );							//	出力形式設定
	short GetParentFromOrder( CDatabase* pDB, short nFormSeq );					//	親番号取得

public:
	int ShowDialog( CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011 );

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
	afx_msg void OnBnClickedRadioNormal();
public:
	afx_msg void OnBnClickedRadioOne();
public:
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif	//	__DLGOUTPUTFORM_H__