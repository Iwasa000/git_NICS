//--------------------------------------------------
//	dlgMessageBox.h
//
//	2006.03.09～
//--------------------------------------------------

#ifndef	__DLGMESSAGEBOX_H__
#define	__DLGMESSAGEBOX_H__

#pragma once
#include "icseditex.h"
#include "icsbutton.h"

// CdlgMessageBox ダイアログ

class CdlgMessageBox : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgMessageBox)

public:
	CdlgMessageBox(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgMessageBox();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MESSAGEBOX };
	ICSStatic	m_lblMessage;	//	メッセージラベル
	ICSButton	m_btn1;			//	第１ボタン
	ICSButton	m_btn2;			//	第２ボタン
	ICSButton	m_btn3;			//	第３ボタン

private:
	CString		m_szTitle;		//	ダイアログタイトル
	CString		m_szMessage;	//	メッセージ
	CString		m_szButton1;	//	第１ボタンの文言
	CString		m_szButton2;	//	第２ボタンの文言
	CString		m_szButton3;	//	第３ボタンの文言
	BOOL		m_fButton2;		//	第２ボタン表示有無（TRUE：表示／FALSE：非表示）
	BOOL		m_fButton3;		//	第３ボタン表示有無（TRUE：表示／FALSE：非表示）
	int			m_nRet;			//	戻値

public:

private:
	int GetFocusControlID( CWnd* pParent = NULL );				//	フォーカスのあるコントロールのID取得

public:
	int	ShowDialog( CString szTitle, CString szMessage,
					CString szButton1 );						//	ダイアログ表示（ボタン１個）
	int	ShowDialog( CString szTitle, CString szMessage,
					CString szButton1, CString szButton2 );		//	ダイアログ表示（ボタン２個）
	int	ShowDialog( CString szTitle, CString szMessage,
		            CString szButton1, CString szButton2, 
					CString szButton3 );						//	ダイアログ表示（ボタン３個）


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL OnInitDialog();	
public:
	afx_msg void OnBnClickedButtonFirst();
public:
	afx_msg void OnBnClickedButtonSecond();
public:
	afx_msg void OnBnClickedButtonThird();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
};

#endif	//	__DLGMESSAGEBOX_H__