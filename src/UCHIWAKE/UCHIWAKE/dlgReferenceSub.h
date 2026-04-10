//--------------------------------------------------
//	dlgreferencesub.h
//
//	2023.11.14～
//	インボイス登録番号追加版に変更
//--------------------------------------------------

#ifndef	__DLGREFERENCESUB_H__
#define	__DLGREFERENCESUB_H__

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

// 金融機関用カラム番号
typedef enum {
	ID_COL_BANK_NO_T		= 0,			// 番号（タイトル）
	ID_COL_BANK_NO			= 1,			// 番号
	ID_COL_BANK_NAME1_T		= 2,			// 金融機関名（タイトル）
	ID_COL_BANK_NAME1		= 3,			// 金融機関名
	ID_COL_BANK_NAME2_T		= 4,			// 支店名（タイトル）
	ID_COL_BANK_NAME2		= 5,			// 支店名
	ID_COL_BANK_KANA1_T		= 13,			// 金融機関カナ（タイトル）
	ID_COL_BANK_KANA1		= 14,			// 金融機関カナ
	ID_COL_BANK_KANA2_T		= 19,			// 支店名カナ（タイトル）
	ID_COL_BANK_KANA2		= 20,			// 支店名カナ
} EnumIdColBank;

// 取引先用カラム番号
typedef enum {
	ID_COL_ADD_NO_T			= 0,			// 番号（タイトル）
	ID_COL_ADD_NO			= 1,			// 番号
	ID_COL_ADD_NAME1_T		= 6,			// 名称（タイトル）
	ID_COL_ADD_NAME1		= 8,			// 名称
	ID_COL_ADD_ADD_T		= 9,			// 所在地（タイトル）
	ID_COL_ADD_ADD1			= 11,			// 所在地（上段）
	ID_COL_ADD_ADD2			= 12,			// 所在地（下段）
	ID_COL_ADD_KANA1_T		= 15,			// カナ（タイトル）
	ID_COL_ADD_KANA1		= 16,			// カナ
} EnumIdColAddress;

// 取引先用カラム番号（インボイス登録番号（法人番号）版）
typedef enum {
	ID_COL_ADDI_NO_T		= 0,			// 番号（タイトル）
	ID_COL_ADDI_NO			= 1,			// 番号
	ID_COL_ADDI_INVNO_T		= 21,			// 登録番号（タイトル１）
	ID_COL_ADDI_INVNOTSEL	= 22,			// 登録番号（T選択）
	ID_COL_ADDI_INVNO		= 23,			// 登録番号
	ID_COL_ADDI_NAME1_T		= 24,			// 名称（タイトル）
	ID_COL_ADDI_NAME1		= 26,			// 名称
	ID_COL_ADDI_ADD_T		= 27,			// 所在地（タイトル）
	ID_COL_ADDI_ADD1		= 29,			// 所在地（上段）
	ID_COL_ADDI_ADD2		= 30,			// 所在地（下段）
	ID_COL_ADDI_KANA1_T		= 31,			// カナ（タイトル）
	ID_COL_ADDI_KANA1		= 32,			// カナ
} EnumIdColAddressI;

// 科目用カラム番号
typedef enum {
	ID_COL_KAMOKU_NO_T		= 0,			// 番号（タイトル）
	ID_COL_KAMOKU_NO		= 1,			// 番号
	ID_COL_KAMOKU_NAME1_T	= 2,			// 科目（タイトル）
	ID_COL_KAMOKU_NAME1		= 3,			// 科目
	ID_COL_KAMOKU_KANA1_T	= 17,			// カナ（タイトル）
	ID_COL_KAMOKU_KANA1		= 18,			// カナ
} EnumIdColKamoku;

class CdlgReferenceSub : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgReferenceSub)

public:
	CdlgReferenceSub(CWnd* pParent = NULL);				//	標準コンストラクタ
	CdlgReferenceSub(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgReferenceSub();						//	デストラクタ

	enum { IDD = IDD_DIALOG_REFERENCE_SUB };
	CICSDiag	m_diagInput;
	ICSButton	m_btnSearch;
	//ICSButton	m_btnSearch2;
	ICSButton	m_btnOK;
	ICSButton	m_btnCancel;

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
	CString				m_szInvno;		//　登録番号
	int					m_nInputLen;	//	入力文字数（※科目参照ダイアログのみ）
	int					m_nGrSeq;		//	グループ番号または様式シーケンス番号
										//		金融機関	…	未使用
										//		取引先		…	「GrSeq」を格納
										//		科目		…	「FormSeq」を格納
	int					m_nSeq;			//	シーケンス番号
										//		金融機関	…	「BkSeq」を格納
										//		取引先		…	「AdSeq」を格納
										//		科目		…	「KnSeq」を格納
	COLORREF			m_ViewColor;	//	ダイアログの背景色

	CUcFunctionCommon	m_clsFunc;		//	共通関数クラス

	CWnd*				m_pOwner;		//	呼び元のウインドウ

public:
	CString				m_RetName1;		//	変更名称１
	CString				m_RetName2;		//	変更名称２
	CString				m_RetAdd1;		//	変更住所１
	CString				m_RetAdd2;		//	変更住所２
	CString				m_RetKana1;		//  変更ｶﾅ１
	CString				m_RetKana2;		//  変更ｶﾅ２
	CString				m_RetInvno;		//  変更登録番号

private:
	void InitDisplay( EnumIdDlgType nType, CString szTitle,
					  CString szOrder, CString szName1, 
					  CString szName2, CString szAdd1,
					  CString szAdd2, CString szKana1,
					  CString szKana2, CString szInvno);							//	ディスプレイ初期化
	int CheckInputData( EnumIdDlgType nType,
						CString szName1, CString szName2,
						CString szAdd1, CString szAdd2,
						CString szKana1, CString szKana2, CString szInvno, 
						int nGrSeq, int nSeq);										//	入力データチェック
	void DiagSetBackColor( short nIndex );											//	背景色の変更
	void DiagSetDataDisp( short nIndex, CString szTitle );							//	表示データセット
	void DiagSetEnabled( short nIndex );											//	セルを非表示
	void DiagSetDataEdit( short nIndex, CString szData );							//	入力データセット
	void DiagSetCombo(int intIndex, CString strData);								//	コンボボックスデータセット
	void DiagSetInputMax( short nIndex, int nMax );									//	最大入力文字数設定
	//void ShowSearchDialog( EnumIdDlgType nType );									//	銀行／住所検索ダイアログ表示
	void ShowSearchDialog(EnumIdDlgType nType, int nSign);							//	銀行／住所検索／法人検索ダイアログ表示
	void InitAttr( DIAGRAM_ATTRIBUTE *diaatt );										//	DIAGRAM_ATTRIBUTE構造体初期化
	CString DelCrLfString( CString szBuf );											//	改行文字削除
	int GetFocusControlID( CWnd* pParent = NULL );									//	フォーカスのあるコントロールのID取得

	BOOL CheckRepeatBank( CString szName1, CString szName2, CString szKana1, CString szKana2 );													//	重複確認（金融機関）
	//BOOL CheckRepeatAddress( CString szName1, CString szName2, CString szAdd1, CString szAdd2, CString szKana, int nGrSeq );					//	重複確認（取引先）
	BOOL CheckRepeatAddress( CString szName1, CString szName2, CString szAdd1, CString szAdd2, CString szKana, CString szInvno, int nGrSeq );	//	重複確認（取引先）
	BOOL CheckRepeatKamoku( CString szName1, CString szName2, int nFormSeq );																	//	重複確認（科目）

	void BankKanaSelect(UCHAR* pBkCode, UCHAR* pBcCode, CString* pBuf1, CString* pBuf2);	// 銀行カナ、支店カナを取得

public:
	int ShowDialog( CDatabase* pDB, EnumIdDlgType nType = ID_DLGTYPE_BANK, 
					EnumIdDlgRefMode nMopde = ID_DLGREF_APPEND,
					CString m_szTitle = "", long nOrder = 0, 
					CString szName1 = "", CString szName2 = "", 
					CString szAdd1 = "", CString szAdd2 = "",
					CString szKana1 = "", CString szKana2 = "", CString szInvno = "",
					int nInputLen = DR_INPUTLENGTH_NORMAL, int nGrSeq = 0, int nSeq = 0 );

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
	void EditOFFInputDiag(short index);
	void EditONInputDiag(short index);						// 修正No.168365,168369 add
	void ComboSelInputDiag(short index, LPUNKNOWN data);	// 修正No.168377 add

public:
	afx_msg void OnBnClickedSearch();
	//afx_msg void OnBnClickedSearch2();
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();

public:
	CZmselDB		*pRfZmSel;
	CDBZmSub		*pRfZmSub;		// 修正No.168444 add
	int				opsw[8];		// 修正No.168444 add

	// ZmSel（登録番号一覧）の初期化
	void	InitZmSelSub(void);
	void	DiagSetHjnData(CString strInvoNo, CString strHjnName);
	friend	void CALLBACK callback2( UINT sgn, void* p, CWnd* pwnd );

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);		// 修正No.168444 add
};

#endif	//	__DLGREFERENCESUB_H__