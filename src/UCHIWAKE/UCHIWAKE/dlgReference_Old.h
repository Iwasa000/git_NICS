// 改良No.22-0404,22-0812 add -->
//リストのコントロールを ListInp から ICSReport に変更するため、従来のソースを
//IDD_DIALOG_REFERENCE_OLD、dlgReference_Old.cpp、dlgReference_Old.h で保存

//--------------------------------------------------
//	dlgReference.h
//
//	2006.02.21～	
//--------------------------------------------------

//#ifndef	__DLGREFERENCE_H__
//#define	__DLGREFERENCE_H__

#pragma once
#include "icslistinp.h"
#include "icsdisp.h"
#include "icsbutton.h"
#include "icsocx.h"
#include "icsgenl8.h"

#include "tbReference.h"
//#include "dlgReferenceSub.h"
#include "dlgReferenceSub_old.h"
#include "dbUcRenKamoku.h"
#include "UcCommon.h"			//	定数ヘッダ
#include "UcFunctionCommon.h"	// 内訳書関数郡　共通宣言
#include "afxwin.h"

// midori 161109 del -->
//#define ID_DIAG_INDEX_LABEL_MAIN	0
//#define ID_DIAG_INDEX_INPUT_MAIN	1
//#define ID_DIAG_INDEX_LABEL_SUB		2
//#define ID_DIAG_INDEX_INPUT_SUB		3
// midori 161109 del <--
// midori 161109 add -->
#define ID_DIAG_INDEX_LABEL_SORT	0	// 表示順タイトル
#define ID_DIAG_INDEX_INPUT_SORT	1	// 表示順コンボボックス
#define ID_DIAG_INDEX_LABEL_MAIN	2
#define ID_DIAG_INDEX_INPUT_MAIN	3
#define ID_DIAG_INDEX_LABEL_SUB		4
#define ID_DIAG_INDEX_INPUT_SUB		5
// midori 161109 add <--

//	金融機関
struct _RD_BANK_OLD{
	long	m_Seq;			//	シーケンス番号
	long	m_OrderNum;		//	順序番号
	CString	m_BkName1;		//	金融機関名
	CString	m_BkName2;		//	支店名
	BYTE	m_FgDel;		//	削除フラグ
	CString m_BkKana1;		//  金融機関ｶﾅ名
	CString m_BkKana2;		//  支店ｶﾅ名
};

//	取引先
struct _RD_ADDRESS_OLD{
	long	m_AdSeq;		//	シーケンス番号
	int		m_GrSeq;		//	グループ番号
	long	m_OrderNum;		//	順序番号
	CString	m_AdName1;		//	取引先1
	CString	m_AdName2;		//	取引先2
	CString	m_AdAdd1;		//	住所1
	CString	m_AdAdd2;		//	住所2
	BYTE	m_FgDel;		//	削除フラグ
	CString m_AdKana;		//  取引先ｶﾅ名
};

//	科目
struct _RD_KAMOKU_OLD{
	long	m_FormSeq;		//	様式シーケンス番号
	long	m_KnSeq;		//	科目シーケンス番号
	long	m_KnOrder;		//	順序番号
	CString	m_KnCode;		//	科目コード
	CString	m_KnName;		//	科目名称
	BYTE	m_FgDel;		//	削除フラグ
	BYTE	m_FgDft;		//	デフォルト値フラグ
	CString m_KnKana;		//  科目ｶﾅ名
};


// CdlgReference_Old ダイアログ

class CdlgReference_Old : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgReference_Old)

public:
	CdlgReference_Old(CWnd* pParent = NULL);			//	標準コンストラクタ
	CdlgReference_Old(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgReference_Old();

	enum { IDD = IDD_DIALOG_REFERENCE_OLD };
	CICSListInp m_lstReference;
	CICSDisp	m_lblEntry;
	ICSButton	m_btnOK;
	ICSButton	m_btnCancel;
	CComboBox	m_cmbTorihikiChange;
	CICSDiag	m_DiagKanaSearch;

private:
	CtbReference		m_tbrToolBar;	//	ツールバー
	CDatabase*			m_pDB;			//	データベースハンドル
	EnumIdDlgType		m_nType;		//	参照型
	short				m_nFormSeq;		//	様式シーケンス番号
	int					m_nRet;			//	戻値
	long				m_nItemSeq;		//	帳表で選択されている項目のシーケンス番号
	int					m_nGrSeq;		//	グループ番号
	int					m_nColumnMax;	//	最大カラム数
	long				m_nOldRow;		//	前回選択行
	REFERENCE_INFO		m_udReference;	//	参照のみ（＝自動登録しない）の場合のデフォルト選択項目検索用文字列
	COLORREF			m_ViewColor;	//	ダイアログの背景色
	CUcFunctionCommon	m_clsFunc;		//	共通関数クラス
	BOOL				m_isRegAuto;	//  「取引先、金融機関を自動で登録する」チェックボックス
// midori 161106 add -->
	BOOL				m_F9OnSw;		//	コンボボックスで「新規作成」が選択されて呼び出された
// midori 161106 add <--
// midori 161109 add -->
// midori 152745 del -->
//	int					m_nSort;		//	表示順選択 0:番号順 1:50音順
// midori 152745 del <--
// midori 152745 add -->
	int					m_nSort[2];		//	表示順選択 0:番号順 1:50音順
	int					m_nSortNum;		//　0:科目、銀行、取引先（仕入先） 1:取引先（得意先）
// midori 152745 add <--
	CDBZmSub*			m_pZmSub;		//	財務dbクラス
// midori 161109 add <--
// midori M-16113002 add -->
	CString				m_Kana1;		//	ｶﾅ入力1
	CString				m_Kana2;		//	ｶﾅ入力2
// midori M-16113002 add <--
// midori 152165_2 add -->
	int					m_OldCombo;		//	変更前のコンボボックスの値
	CWnd*				m_pOwner;		//	呼び元のウインドウ
// midori 152165_2 add <--

public:
	_RD_BANK_OLD		m_udBank;		//	金融機関
	_RD_ADDRESS_OLD		m_udAddress;	//	取引先
	_RD_KAMOKU_OLD		m_udKamoku;		//	科目
	BOOL				m_fUpdate;		//	更新フラグ

private:
	void OnToolButtonF6();
	void OnToolButtonF7();
	void OnToolButtonF8();
	void OnToolButtonF9();
	void OnToolButtonF11();
	void OnToolButtonF12();

private:
	int SetToolBar();																					//	ツールバー設定
	void InitList( EnumIdDlgType nType );																//	リスト初期化
	void ToolBarEventExecute( short nKey );																//	ツールバーのキーイベントを実行
// midori 161109 del -->
//	void GetListData( CDatabase* pDB, EnumIdDlgType nType, 
//		short nFormSeq, int nGrSeq, CString strFilterKana1 = NULL, CString strFilterKana2 = NULL );		//	リストデータ取得
//	long GetKinyuukikanItem( CDatabase* pDB, CString strFilter, CString strFilter2 );					//	金融機関アイテム取得
//	long GetTorihikisakiItem( CDatabase* pDB, int nGrSeq, CString strFilter );							//	取引先アイテム取得
//	long GetKamokuItem( CDatabase* pDB, short nFormSeq, CString strFilter );							//	科目アイテム取得
//	long SetRecordEdit( CDatabase* pDB, EnumIdDlgType nType,
//						short nFormSeq, int nGrSeq, 
//						BOOL fSearch = FALSE );															//	レコードの編集
// midori 161109 del <--
// midori 161109 add -->
	void GetListData( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
		CString strFilterKana1 = NULL, CString strFilterKana2 = NULL );									//	リストデータ取得
	long GetKinyuukikanItem( CDatabase* pDB, int nSort, CString strFilter, CString strFilter2 );		//	金融機関アイテム取得
	long GetTorihikisakiItem( CDatabase* pDB, int nGrSeq, int nSort, CString strFilter );				//	取引先アイテム取得
	long GetKamokuItem( CDatabase* pDB, short nFormSeq, int nSort, CString strFilter );					//	科目アイテム取得
	long SetRecordEdit( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						BOOL fSearch = FALSE );															//	レコードの編集
// midori 161109 add <--
	long EditKinyuukikan( CDatabase* pDB, BOOL fSearch = FALSE );										//	金融機関のレコード編集
	long EditTorihikisaki( CDatabase* pDB, int nGrSeq, BOOL fSearch = FALSE );							//	取引先のレコード編集
	long EditKamoku( CDatabase* pDB, short nFormSeq );													//	科目のレコード編集
// midori 161109 del -->
	//long SetRecordInsert( CDatabase* pDB, EnumIdDlgType nType,
	//					  short nFormSeq, int nGrSeq, 
	//					  BOOL fAddLine = FALSE );														//	レコードの挿入（追加）
	//long InsertKinyuukikan( CDatabase* pDB, CString szTitle,  BOOL fAddLine );							//	金融機関のレコード挿入（追加）
	//long InsertTorihikisaki( CDatabase* pDB, int nGrSeq, CString szTitle,  BOOL fAddLine );				//	取引先のレコード挿入（追加）
	//long InsertKamoku( CDatabase* pDB, short nFormSeq, CString szTitle, BOOL fAddLine );				//	科目のレコード挿入（追加）
// midori 161109 del <--
// midori 161109 add -->
	long SetRecordInsert( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						  BOOL fAddLine = FALSE );														//	レコードの挿入（追加）
	long InsertKinyuukikan( CDatabase* pDB, int nSort, CString szTitle,  BOOL fAddLine );				//	金融機関のレコード挿入（追加）
	long InsertTorihikisaki( CDatabase* pDB, int nGrSeq, int nSort, CString szTitle,  BOOL fAddLine );	//	取引先のレコード挿入（追加）
	long InsertKamoku( CDatabase* pDB, short nFormSeq, int nSort, CString szTitle, BOOL fAddLine );		//	科目のレコード挿入（追加）
// midori 161109 add <--
	long GetListIndex();																				//	賞味のリストインデックス取得
// midori 161109 add -->
	long GetListIndex2( CDatabase* pDB, int pType, int pGrSeq, short pFormSeq, int pSort );				//	最大番号を持っているリストインデックスを取得
// midori 161109 add <--
	void MovePosition( long nTop, long nIndex );															//	位置移動
// midori 161109 add -->
	void MovePosition2( int nOrder );																	//	位置移動
// midori 161109 add <--
// midori 161109 del -->
//	long SetRecordDelete( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq );			//	レコードの削除
// midori 161109 del <--
// midori 161109 add -->
	long SetRecordDelete( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort );	//	レコードの削除
// midori 161109 add <--
	long DeleteKinyuukikan( CDatabase* pDB );															//	金融機関のレコード削除
	long DeleteTorihikisaki( CDatabase* pDB, int nGrSeq );												//	取引先のレコード削除
	long DeleteKamoku( CDatabase* pDB, short nFormSeq );												//	科目のレコード削除
// midori 161109 del -->
	//long SetRecordUpDown( CDatabase* pDB, EnumIdDlgType nType,
	//					  short nFormSeq, int nGrSeq,
	//					  int nUpDown );																//	レコードの上下移動
// midori 161109 del <--
// midori 161109 add -->
	long SetRecordUpDown( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						  int nUpDown );																//	レコードの上下移動
// midori 161109 add <--
	long UpDownKinyuukikan( CDatabase* pDB, int nUpDown );												//	金融機関のレコード上下移動
	long UpDownTorihikisaki( CDatabase* pDB, int nGrSeq, int nUpDown );									//	取引先のレコード上下移動
	long UpDownKamoku( CDatabase* pDB, short nFormSeq, int nUpDown );									//	科目のレコード上下移動
	int GetKinnyuukikanRecord( CDatabase* pDB, long nSeq, _RD_BANK_OLD* pudBank );							//	金融機関のレコード取得
	int GetTorihikisakiRecord( CDatabase* pDB, int GrSeq, long nAdSeq, _RD_ADDRESS_OLD* pudAddress );		//	取引先のレコード取得
	int GetKamokuRecord( CDatabase* pDB, short nFormSeq, long nKnSeq, _RD_KAMOKU_OLD* pudKamoku );			//	科目のレコード取得
	long GetSeq( int nIndex );																			//	リストからシーケンス番号取得
	int GetSelectRecord( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq );				//	選択されたリストのレコード取得
// midori 161109 del -->
//	void InitMovePosition( CDatabase* pDB, EnumIdDlgType nType,
//						   short nFormSeq, int nGrSeq,
//						   long nItemSeq );																//	初期位置移動
// midori 161109 del <--
// midori 161109 add -->
	void InitMovePosition( CDatabase* pDB, EnumIdDlgType nType, 
						   short nFormSeq, int nGrSeq, int nSort, long nItemSeq );						//	初期位置移動
// midori 161109 add <--
	void InitLinpData( LINP_DATA* pData );																//	LINP_DATA構造体初期化
	void CheckNowRowPosition();																			//	現在行確認
	void SetColor( int nIndex, COLORREF nFont, COLORREF nBack );										//	色を設定
// midori M-16113003 del -->
//	void ReDrawRowColor();																				//	行の背景色を更新
// midori M-16113003 del <--
// midori M-16113003 add -->
	void ReDrawRowColor(int pSw);																		//	行の背景色を更新
// midori M-16113003 add <--
	long GetFgDft( int nIndex );																		//	リストからデフォルトフラグ取得
	int GetItemSeq();																					//	帳表で選択されている項目のシーケンス番号取得
	int SearchBankItemSeq();																			//	金融機関のシーケンス番号検索
	int SearchAddressItemSeq();																			//	取引先のシーケンス番号検索
	int GetFocusControlID( CWnd* pParent = NULL );
	int InitOtherControls( EnumIdDlgType nType, int nGrSeq);
	int SetInitKanaSearchCtrl( EnumIdDlgType nType );													// ダイアグラム初期データセット

// midori 161109 add -->
	void KanaSearch_ComboSelChenge(short index);														//	コンボボックスのデータが変わったかどうか

// midori 152135,152136 del -->
//	int Owntb_DataGet(void);																			//	Owntbからデータを取得
//	int Owntb_DataSet(void);																			//	Owntbにデータをセット
// midori 152135,152136 del <--
// midori 161109 add <--
// midori M-16113003 add -->
	void FocusSet(int pSw);
// midori M-16113003 add <--

public:
// midori 152135,152136 del -->
//// midori 161106 del -->
////	int ShowDialog( BOOL& isRegAuto, CDatabase* pDB = NULL, EnumIdDlgType nType = ID_DLGTYPE_BANK,
////					short nFormSeq = ID_FORMNO_011, int nGrSeq = 0,
////					long nItemSeq = 0, REFERENCE_INFO* pudReference = 0 );
//// midori 161106 del <--
//// midori 161106 add -->
//	int ShowDialog( BOOL& isRegAuto, CDatabase* pDB = NULL, EnumIdDlgType nType = ID_DLGTYPE_BANK,
//					short nFormSeq = ID_FORMNO_011, int nGrSeq = 0,
//					long nItemSeq = 0, REFERENCE_INFO* pudReference = 0, BOOL pF9OnSw = FALSE );
//// midori 161106 add <--
// midori 152135,152136 del <--
// midori 152745 del -->
//// midori 152135,152136 add -->
//	int ShowDialog( BOOL& isRegAuto, int& pSort, CDatabase* pDB = NULL, EnumIdDlgType nType = ID_DLGTYPE_BANK,
//					short nFormSeq = ID_FORMNO_011, int nGrSeq = 0,
//					long nItemSeq = 0, REFERENCE_INFO* pudReference = 0, BOOL pF9OnSw = FALSE);
//// midori 152135,152136 add <--
// midori 152745 del <--
// midori 152745 add -->
	int ShowDialog( BOOL& isRegAuto, int pSort[], CDatabase* pDB = NULL, EnumIdDlgType nType = ID_DLGTYPE_BANK,
					short nFormSeq = ID_FORMNO_011, int nGrSeq = 0,
					long nItemSeq = 0, REFERENCE_INFO* pudReference = 0, BOOL pF9OnSw = FALSE);
// midori 152745 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	DECLARE_EVENTSINK_MAP()
public:
	void LButtonDblClickReferenceList(short index);
public:
	afx_msg void OnBnClickedOk();
public:
	void SelChangeReferenceList(short index);
public:
	void LButtonClickReferenceList(short index);
public:
	void FocusedReferenceList();
protected:
	virtual void OnOK();	
public:
	afx_msg void OnCbnSelchangeComboTorihikichange();
	void TerminationKanaSearchDiag(short index, short nChar, short length, LPUNKNOWN data);
	CButton m_chkRegAuto;
	afx_msg void OnBnClickedCheckRegAuto();

// midori 161109 add -->
	void ComboSelKanaSearchDiag(short index, LPUNKNOWN data);
// midori 161109 add <--
// midori M-16113002 add -->
	void EditOFFIcsdiag8Kanasearch(short index);
// midori M-16113002 add <--
// midori 152165_2 add -->
	void EditONIcsdiag8Kanasearch(short index);
// midori 152165_2 add <--
};

//#endif	//	__DLGREFERENCE_H__

// 改良No.22-0404,22-0812 add <--
