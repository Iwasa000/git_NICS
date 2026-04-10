//--------------------------------------------------
//	dlgreferencesub_old.h
//
//	2023.11.14～
//	dlgreferencesub.cppの変更前。
//	dlgreferencesub.cppはインボイス登録番号追加版に変更
//--------------------------------------------------

#ifndef	__DLGREFERENCESUB_OLD_H__
#define	__DLGREFERENCESUB_OLD_H__

#pragma once
#include "icsdiag.h"
#include "icsbutton.h"

#include "icsocx.h"
#include "UcCommon.h"			//	定数ヘッダ

#include "WBankDef8.h"			//	銀行検索用ヘッダ
#include "ICSBank8.h"			//	銀行検索用ヘッダ
#include "dlgAddressSearch.h"	//	住所検索クラス
#include "dbUcLstBank.h"		//	uc_lst_bankテーブルクラス
#include "dbUcLstAddress.h"		//	uc_lst_adressテーブルクラス
#include "dbUcLstKamoku.h"		//	uc_lst_kamokuテーブルクラス
#include "UcFunctionCommon.h"	// 内訳書関数郡　共通宣言



class CdlgReferenceSub_Old : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgReferenceSub_Old)

public:
	CdlgReferenceSub_Old(CWnd* pParent = NULL);				//	標準コンストラクタ
	CdlgReferenceSub_Old(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgReferenceSub_Old();						//	デストラクタ

	enum { IDD = IDD_DIALOG_REFERENCE_SUB };
	CICSDiag m_diagInput;
	ICSButton m_btnSearch;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;

private:
	CDatabase*			m_pDB;			//	データベースハンドル
	EnumIdDlgType		m_nType;		//	参照型
	EnumIdDlgRefMode	m_nMode;		//	動作モード
	int					m_nRet;			//	戻値
	CString				m_szOrder;		//	順序番号
	CString				m_szTitle;		//	ダイアログタイトル
	CString				m_szName1;		//	文字列１
	CString				m_szName2;		//	文字列２
	CString				m_szAdd1;		//	住所１
	CString				m_szAdd2;		//	住所２
	CString				m_szKana1;		//　ｶﾅ文字
	CString				m_szKana2;		//　ｶﾅ文字２
	int					m_nInputLen;	//	入力文字数（※科目参照ダイアログのみ）
// 改良No.22-0404,22-0812 del -->
	//int					m_nSeq;			//	シーケンス番号
	//									//		金融機関	…	未使用
	//									//		取引先		…	「GrSeq」を格納
	//									//		科目		…	「FormSeq」を格納
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
	int					m_nGrSeq;		//	グループ番号または様式シーケンス番号
										//		金融機関	…	未使用
										//		取引先		…	「GrSeq」を格納
										//		科目		…	「FormSeq」を格納
	int					m_nSeq;			//	シーケンス番号
										//		金融機関	…	「BkSeq」を格納
										//		取引先		…	「AdSeq」を格納
										//		科目		…	「KnSeq」を格納
// 改良No.22-0404,22-0812 add <--
	COLORREF			m_ViewColor;	//	ダイアログの背景色

	CUcFunctionCommon	m_clsFunc;					//	共通関数クラス

// 修正No.157628 add -->
	CWnd*				m_pOwner;		//	呼び元のウインドウ
// 修正No.157628 add <--

public:
	CString				m_RetName1;		//	変更名称１
	CString				m_RetName2;		//	変更名称２
	CString				m_RetAdd1;		//	変更住所１
	CString				m_RetAdd2;		//	変更住所２
	CString				m_RetKana1;		//  変更ｶﾅ１
	CString				m_RetKana2;		//  変更ｶﾅ２

private:
	void InitDisplay( EnumIdDlgType nType, CString szTitle,
					  CString szOrder, CString szName1, 
					  CString szName2, CString szAdd1,
					  CString szAdd2, CString szKana1,
					  CString szKana2);												//	ディスプレイ初期化
// 改良No.22-0404,22-0812 del -->
	//int CheckInputData( EnumIdDlgType nType, CString szName1, 
	//					CString szName2, CString szAdd1,
	//					CString szAdd2, CString szKana1,
	//					CString szKana2, int nSeq );								//	入力データチェック
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
	int CheckInputData( EnumIdDlgType nType,
						CString szName1, CString szName2,
						CString szAdd1, CString szAdd2,
						CString szKana1, CString szKana2, 
						int nGrSeq, int nSeq);										//	入力データチェック
// 改良No.22-0404,22-0812 add <--
	void DiagSetBackColor( short nIndex );											//	背景色の変更
	void DiagSetDataDisp( short nIndex, CString szTitle );							//	表示データセット
	void DiagSetEnabled( short nIndex );											//	セルを非表示
	void DiagSetDataEdit( short nIndex, CString szData );							//	入力データセット
	//CString DiagGetDataEdit( short nIndex );										//	入力データ取得
	void DiagSetInputMax( short nIndex, int nMax );									//	最大入力文字数設定
	void ShowSearchDialog( EnumIdDlgType nType );									//	銀行／住所検索ダイアログ表示
	//void InitDiag( DIAGRAM_DATA *diadata );											//	DIAGRAM_DATA構造体初期化
	void InitAttr( DIAGRAM_ATTRIBUTE *diaatt );										//	DIAGRAM_ATTRIBUTE構造体初期化
	CString DelCrLfString( CString szBuf );											//	改行文字削除
	int GetFocusControlID( CWnd* pParent = NULL );									//	フォーカスのあるコントロールのID取得

	BOOL CheckRepeatBank( CString szName1, CString szName2, CString szKana1, CString szKana2 );									//	重複確認（金融機関）							
	BOOL CheckRepeatAddress( CString szName1, CString szName2, CString szAdd1, CString szAdd2, CString szKana, int nGrSeq );	//	重複確認（取引先）
	BOOL CheckRepeatKamoku( CString szName1, CString szName2, int nFormSeq );													//	重複確認（科目）

//// 改良No.22-0404,22-0812 add -->
//	int	CheckDuplicationBank(CString szName1, CString szName2, int nSeq);														// 重複確認 小文字/大文字、全角/半角の違い（金融機関）
//	int	CheckDuplicationAddress(CString szName1, CString szName2, CString szAdd1, CString szAdd2, int nGrSeq, int nSeq);		// 重複確認 小文字/大文字、全角/半角の違い（取引先）
//	int CheckDuplicationKamoku(CString szName1, int nFormSeq, int nSeq);														// 重複確認 小文字/大文字、全角/半角の違い（科目）
//// 改良No.22-0404,22-0812 add <--

// midori 161109 add -->
	void BankKanaSelect(UCHAR* pBkCode, UCHAR* pBcCode, CString* pBuf1, CString* pBuf2);	// 銀行カナ、支店カナを取得
// midori 161109 add <--

	//void GetAdName( CString szBuf, CString* pszAdName1, CString* pszAdName2 );
	//CString SetAdName( CString szAdName1, CString szAdName2 );

public:
	int ShowDialog( CDatabase* pDB, EnumIdDlgType nType = ID_DLGTYPE_BANK, 
					EnumIdDlgRefMode nMopde = ID_DLGREF_APPEND,
					CString m_szTitle = "", long nOrder = 0, 
					CString szName1 = "", CString szName2 = "", 
					CString szAdd1 = "", CString szAdd2 = "",
					CString szKana1 = "", CString szKana2 = "",
// 改良No.22-0404,22-0812 del -->
					//int nInputLen = DR_INPUTLENGTH_NORMAL, int nSeq = 0 );
// 改良No.22-0404,22-0812 del <--
// 改良No.22-0404,22-0812 add -->
					int nInputLen = DR_INPUTLENGTH_NORMAL, int nGrSeq = 0, int nSeq = 0 );
// 改良No.22-0404,22-0812 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

public:
	DECLARE_EVENTSINK_MAP()
public:
	void TerminationInputDiag(short index, short nChar, short length, LPUNKNOWN data);
	void ReadStringIcsdiag8Kanasearch(short index, LPCTSTR kana);
public:
	afx_msg void OnBnClickedSearch();
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
};

#endif	//	__DLGREFERENCESUB_OLD_H__