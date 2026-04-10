// dlgAmount.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CdlgAmount ダイアログ
#ifndef __AMOUNT_H__
#define __AMOUNT_H__

#pragma once
#include "afxwin.h"
#include "icsbutton.h"
#include "dbUcInfSub.h"		//	uc_inf_subテーブルクラス
#include "UcCommon.h"		//	定数ヘッダ

#define AM_TYPE_OFF			0x01															//	オフ
#define AM_TYPE_PAGEKEI		0x02															//	頁計
#define AM_TYPE_RUIKEI		0x04															//	累計
#define AM_TYPE_BOTH		0x08															//	頁計と累計
#define AM_TYPE_ALL			AM_TYPE_OFF | AM_TYPE_PAGEKEI |	AM_TYPE_RUIKEI | AM_TYPE_BOTH	//	全て

class CdlgAmount : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgAmount)

public:
	CdlgAmount(CWnd* pParent = NULL);   // 標準コンストラクタ
	CdlgAmount(UINT id, CWnd* pParent = NULL);
	virtual ~CdlgAmount();

	enum { IDD = IDD_DIALOG_AMOUNT };
	CButton m_rdoNothing;
	CButton m_rdoPage;
	CButton m_rdoAmount;
	CButton m_rdoBoth;
	CButton m_rdoKei;
	CButton m_rdoGoukei;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	CDatabase*	m_pDB;			//	データベースハンドル
	short		m_nFormSeq;		//	様式シーケンス番号
	BYTE		m_bType;		//	計の種別
	int			m_nRet;			//	戻値
								//		ID_DLG_OK		…	OKボタンが押された
								//		UC_DLG_CANCEL	…	キャンセルボタンが押された
public:
	int			m_nNameKei;		//　計の名称（"計"か"合計"か）
	BYTE		m_OutKei;		//	頁計・累計出力
								//		ID_OUTKEI_OFF		…	OFF
								//		ID_OUTKEI_PAGEKEI	…	頁計のみ	
								//		ID_OUTKEI_RUIKEI	…	累計のみ
								//		ID_OUTKEI_BOTH		…	頁計・累計

private:
	void GetAmountData( CDatabase* pDB, short nFormSeq );		//	データ取得
	int SetAmountData( CDatabase* pDB, short nFormSeq );		//	データ設定

public:
	//int	ShowDialog( CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011, BYTE bType = AM_TYPE_ALL, int nNameKei = 0 );	//	ダイアログ表示
	int	ShowDialog( CDatabase* pDB, short nFormSeq, BYTE bType, int nNameKei );	//	ダイアログ表示
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOkButton();
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif	//	__AMOUNT_H__
