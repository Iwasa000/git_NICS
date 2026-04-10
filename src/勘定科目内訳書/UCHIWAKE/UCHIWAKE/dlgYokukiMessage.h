// dlgYokukiMessage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CdlgYokukiMessage ダイアログ
#ifndef __YOKUKIMESSAGE_H__
#define __YOKUKIMESSAGE_H__

#pragma once

#include "dateConvert.h"
#include "icseditex.h"
#include "icsbutton.h"

// CdlgYokukiMessage ダイアログ

class CdlgYokukiMessage : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgYokukiMessage)

public:
	CdlgYokukiMessage(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgYokukiMessage();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_YOKUKI_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	int			m_nRet;				//	戻値
	long		m_lngZvolSSymd;		//	財務マスター開始期間
	long		m_lngZvolEEymd;		//	財務マスター終了期間
	long		m_lngUchiSSymd;		//	内訳書アプリ開始期間
	long		m_lngUchiEEymd;		//	内訳書マスター終了期間
	ICSStatic	m_lblMessage;		//	確認メッセージラベル
	ICSButton m_btnOK;				//	「はい」ボタン
	ICSButton m_btnCancel;			//	「いいえ」ボタン

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOkButton();

public:
	int	ShowDialog( long lngZvolSSymd , long lngZvolEEymd , long lngUchiSSymd , long lngUchiEEymd );
private:
	CString CreateYokukiMessage();
};

#endif //	__YOKUKIMESSAGE_H__