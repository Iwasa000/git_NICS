// CdlgRenTekiyo ダイアログ
#ifndef __RENTEKIYO_H__
#define __RENTEKIYO_H__

#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "icsbutton.h"
#include "icslistinp.h"
#include "icsocx.h"
#include "icsgenl8.h"
#include "UcCommon.h"		//	定数ヘッダ

// CdlgRenTekiyo ダイアログ

class CdlgRenTekiyo : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgRenTekiyo)

public:
	CdlgRenTekiyo(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgRenTekiyo();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_RENTEKIYO };
	CICSListInp m_lstTekiyo;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;

private:
	CDBZmSub*		m_pZmSub;		//  財務DBクラスハンドル
	int				m_nRet;			//	戻値
	int				m_nColumnMax;	//	最大カラム数
	int				m_nStartIndex;	//	指定コードと一致するインデックス
	long			m_nOldRow;		//	前回選択行
	long			m_nTkcod;		//	指定摘要コード
	CUcFunctionCommon m_clsFunc;	//	共通関数クラス
public:
	long	m_Tkcod;		// 指定摘要コード

public:
	void InitList();
	void GetListData( CDBZmSub* ZmSub );
	long GetListIndex();
	void MovePosition( long nTop, long nIndex );
	long GetTekiyoCode( int nIndex );
	void InitLinpData( LINP_DATA* pData );
	void CheckNowRowPosition();
	void SetColor( int nIndex, COLORREF nFont, COLORREF nBack );
	void ReDrawRowColor();
	int GetFocusControlID( CWnd* pParent = NULL );
	int ShowDialog( long Tkcod );	//	ダイアログ表示

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SelChangeTekiyoList(short index);
	void LButtonClickTekiyoList(short index);
	void LButtonDblClickTekiyoList(short index);
	void FocusedTekiyoList();
public:
	DECLARE_EVENTSINK_MAP()
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOkButton();
};

#endif	//	__RENTEKIYO_H__