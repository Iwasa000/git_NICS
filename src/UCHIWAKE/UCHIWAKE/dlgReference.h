// 改良No.22-0404,22-0812 add -->
//--------------------------------------------------
//	dlgReference.h
//
//	2006.02.21～	
//--------------------------------------------------

#ifndef	__DLGREFERENCE_H__
#define	__DLGREFERENCE_H__

#pragma once
#include "icslistinp.h"
#include "icsdisp.h"
#include "icsbutton.h"
#include "icsocx.h"
#include "icsgenl8.h"
#include "icsreportcontrol.h"

#include "tbReference.h"
#include "dlgReferenceSub.h"
#include "dbUcRenKamoku.h"
#include "UcCommon.h"			// 定数ヘッダ
#include "UcFunctionCommon.h"	// 内訳書関数郡　共通宣言
#include "afxwin.h"

// 修正No.162490 cor -->
//#define ID_DIAG_INDEX_LABEL_SORT	0	// 表示順タイトル
//#define ID_DIAG_INDEX_INPUT_SORT	1	// 表示順コンボボックス
//#define ID_DIAG_INDEX_LABEL_MAIN	2
//#define ID_DIAG_INDEX_INPUT_MAIN	3
//#define ID_DIAG_INDEX_LABEL_SUB		4
//#define ID_DIAG_INDEX_INPUT_SUB		5
// ---------------------
#define ID_DIAG_INDEX_INPUT_KUBUN	0	// 仕入先/得意先 選択コンボボックス
#define ID_DIAG_INDEX_LABEL_SORT	1	// 表示順タイトル
#define ID_DIAG_INDEX_INPUT_SORT	2	// 表示順コンボボックス
#define ID_DIAG_INDEX_LABEL_MAIN	3
#define ID_DIAG_INDEX_INPUT_MAIN	4
#define ID_DIAG_INDEX_LABEL_SUB		5
#define ID_DIAG_INDEX_INPUT_SUB		6
// 修正No.162490 cor <--

//	金融機関
struct _RD_BANK {
	long	m_Seq;			//	シーケンス番号
	long	m_OrderNum;		//	順序番号
	CString	m_BkName1;		//	金融機関名
	CString	m_BkName2;		//	支店名
	BYTE	m_FgDel;		//	削除フラグ
	CString m_BkKana1;		//  金融機関ｶﾅ名
	CString m_BkKana2;		//  支店ｶﾅ名
};

//	取引先
struct _RD_ADDRESS {
	long	m_AdSeq;		//	シーケンス番号
	int		m_GrSeq;		//	グループ番号
	long	m_OrderNum;		//	順序番号
	CString	m_AdName1;		//	取引先1
	CString	m_AdName2;		//	取引先2
	CString	m_AdAdd1;		//	住所1
	CString	m_AdAdd2;		//	住所2
	BYTE	m_FgDel;		//	削除フラグ
	CString m_AdKana;		//	取引先ｶﾅ名
	CString m_InvNo;		//	登録番号(法人番号)	// インボイス登録番号追加対応_23/11/06 add
};

//	科目
struct _RD_KAMOKU {
	long	m_FormSeq;		//	様式シーケンス番号
	long	m_KnSeq;		//	科目シーケンス番号
	long	m_KnOrder;		//	順序番号
	CString	m_KnCode;		//	科目コード
	CString	m_KnName;		//	科目名称
	BYTE	m_FgDel;		//	削除フラグ
	BYTE	m_FgDft;		//	デフォルト値フラグ
	CString m_KnKana;		//  科目ｶﾅ名
};

class CdlgReference : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgReference)

public:
	CdlgReference(CWnd* pParent = NULL);			//	標準コンストラクタ
	CdlgReference(UINT id, CWnd* pParent = NULL);	//	コンストラクタ
	virtual ~CdlgReference();						//	デストラクタ

	enum { IDD = IDD_DIALOG_REFERENCE };

	CICSDisp			m_lblEntry;
	ICSButton			m_btnOK;
	ICSButton			m_btnCancel;
	//CComboBox			m_cmbTorihikiChange;		// 修正No.162490 del
	CICSDiag			m_DiagKanaSearch;
	ICSReportControl	m_report;
	CStatic				m_guide;					// 改良No.22-0404,22-0812_230414 add

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
	BOOL				m_F9OnSw;		//	コンボボックスで「新規作成」が選択されて呼び出された
	int					m_nSort[2];		//	表示順選択 0:番号順 1:50音順
	int					m_nSortNum;		//　0:科目、銀行、取引先（仕入先） 1:取引先（得意先）
	CDBZmSub*			m_pZmSub;		//	財務dbクラス
	CString				m_Kana1;		//	ｶﾅ入力1
	CString				m_Kana2;		//	ｶﾅ入力2
	int					m_OldCombo;		//	変更前のコンボボックスの値
	int					m_OldKCombo;	//	変更前のコンボボックスの値		// 修正No.162490 add
	CWnd*				m_pOwner;		//	呼び元のウインドウ

	long				m_DataLineMax;	//	データの最大件数（空白行が入るため、最大行数はこれで管理）

public:
	_RD_BANK			m_udBank;		//	金融機関
	_RD_ADDRESS			m_udAddress;	//	取引先
	_RD_KAMOKU			m_udKamoku;		//	科目
	BOOL				m_fUpdate;		//	更新フラグ

private:
	void OnToolButtonF6();
	void OnToolButtonF7();
	void OnToolButtonF8();
	void OnToolButtonF9();
	void OnToolButtonF11();
	void OnToolButtonF12();

private:
	int SetToolBar(void);																				//	ツールバー設定
	void InitReport( EnumIdDlgType nType );																//	レポートコントロール初期化
	void GetReportData( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						CString strFilterKana1 = NULL, CString strFilterKana2 = NULL);					//	レポートデータ取得
	long GetKinyuukikanItemRep(CDatabase* pDB, int nSort, CString strFilter, CString strFilter2);		//	金融機関アイテム取得(レポート用)
	long GetTorihikisakiItemRep(CDatabase* pDB, int nGrSeq, int nSort, CString strFilter);				//	取引先アイテム取得(レポート用)
	long GetKamokuItemRep(CDatabase* pDB, short nFormSeq, int nSort, CString strFilter);				//	科目アイテム取得(レポート用)
	long SetRecordEdit( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						BOOL fSearch = FALSE );															//	レコードの編集
	long EditKinyuukikan( CDatabase* pDB, BOOL fSearch = FALSE );										//	金融機関のレコード編集
	long EditTorihikisaki( CDatabase* pDB, int nGrSeq, BOOL fSearch = FALSE );							//	取引先のレコード編集
	long EditKamoku( CDatabase* pDB, short nFormSeq );													//	科目のレコード編集
	long SetRecordInsert( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						  BOOL fAddLine = FALSE );														//	レコードの挿入（追加）
	long InsertKinyuukikan( CDatabase* pDB, int nSort, CString szTitle,  BOOL fAddLine );				//	金融機関のレコード挿入（追加）
	long InsertTorihikisaki( CDatabase* pDB, int nGrSeq, int nSort, CString szTitle,  BOOL fAddLine );	//	取引先のレコード挿入（追加）
	long InsertKamoku( CDatabase* pDB, short nFormSeq, int nSort, CString szTitle, BOOL fAddLine );		//	科目のレコード挿入（追加）
	long GetListIndex(void);																			//	賞味のリストインデックス取得
	long GetListIndex2(CDatabase* pDB, int pType, int pGrSeq, short pFormSeq, int pSort);				//	最大番号を持っているリストインデックスを取得
	void MovePosition( long nTop, long nIndex );														//	位置移動
	void MovePosition2( int nOrder );																	//	位置移動
	long SetRecordDelete(CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						 CArray<long, long>* pDelseq);													//	レコードの削除

	long DeleteCheckKinyuukikan(CDatabase* pDB, short nFormSeq, CArray<long, long>* pDelseq);			//	金融機関を削除する前に、削除可能かどうかチェック
	long DeleteCheckTorihikisaki(CDatabase* pDB, short nFormSeq, int nGrSeq,
								 CArray<long, long>* pDelseq);											//	取引先を削除する前に、削除可能かどうかチェック
	long DeleteCheckKamoku(CDatabase* pDB, short nFormSeq, CArray<long, long>* pDelseq);				//	科目を削除する前に、削除可能かどうかチェック

	long DeleteKinyuukikan(CDatabase* pDB, CArray<long, long>* pDelseq);								//	金融機関のレコード削除
	long DeleteTorihikisaki(CDatabase* pDB, int nGrSeq, CArray<long, long>* pDelseq);					//	取引先のレコード削除
	long DeleteKamoku(CDatabase* pDB, short nFormSeq, CArray<long, long>* pDelseq);						//	科目のレコード削除
	long SetRecordUpDown( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq, int nSort,
						  int nUpDown );																//	レコードの上下移動
	long UpDownKinyuukikan( CDatabase* pDB, int nUpDown );												//	金融機関のレコード上下移動
	long UpDownTorihikisaki( CDatabase* pDB, int nGrSeq, int nUpDown );									//	取引先のレコード上下移動
	long UpDownKamoku( CDatabase* pDB, short nFormSeq, int nUpDown );									//	科目のレコード上下移動
	int GetKinnyuukikanRecord( CDatabase* pDB, long nSeq, _RD_BANK* pudBank );							//	金融機関のレコード取得
	int GetTorihikisakiRecord( CDatabase* pDB, int GrSeq, long nAdSeq, _RD_ADDRESS* pudAddress );		//	取引先のレコード取得
	int GetKamokuRecord( CDatabase* pDB, short nFormSeq, long nKnSeq, _RD_KAMOKU* pudKamoku );			//	科目のレコード取得
	long GetSeq( int nIndex );																			//	リストからシーケンス番号取得
	int GetSelectRecord( CDatabase* pDB, EnumIdDlgType nType, short nFormSeq, int nGrSeq );				//	選択されたリストのレコード取得
	void InitMovePosition( CDatabase* pDB, EnumIdDlgType nType, 
						   short nFormSeq, int nGrSeq, int nSort, long nItemSeq );						//	初期位置移動
	void InitLinpData( LINP_DATA* pData );																//	LINP_DATA構造体初期化
	//void CheckNowRowPosition(void);																	//	現在行確認
	void CheckNowRowPosition(long pIndx = -1);															//	現在行確認
	void SetColor( int nIndex, COLORREF nFont, COLORREF nBack );										//	色を設定
	//void ReDrawRowColor(int pSw);																		//	行の背景色を更新
	long GetFgDft( int nIndex );																		//	リストからデフォルトフラグ取得
	int GetItemSeq(void);																				//	帳表で選択されている項目のシーケンス番号取得
	int SearchBankItemSeq(void);																		//	金融機関のシーケンス番号検索
	int SearchAddressItemSeq(void);																		//	取引先のシーケンス番号検索
	int GetFocusControlID( CWnd* pParent = NULL );
	int InitOtherControls( EnumIdDlgType nType, int nGrSeq);
	int SetInitKanaSearchCtrl( EnumIdDlgType nType );													// ダイアグラム初期データセット

	void KanaSearch_ComboSelChenge(short index);														//	コンボボックスのデータが変わったかどうか
	void KanaSearch_ComboSelChengeTorihiki(short index);						// 修正No.162490 add

	void FocusSet(int pSw);

// 修正No.162491 add -->
	int GetMaxOrderNo(CDatabase* pDB, int pType, int pGrSeq, short pFormSeq);							//	科目、金融機関、取引先のＭＡＸ番号を取得する
// 修正No.162491 add <--

public:
	int ShowDialog( BOOL& isRegAuto, int pSort[], CDatabase* pDB = NULL, EnumIdDlgType nType = ID_DLGTYPE_BANK,
					short nFormSeq = ID_FORMNO_011, int nGrSeq = 0,
					long nItemSeq = 0, REFERENCE_INFO* pudReference = 0, BOOL pF9OnSw = FALSE);

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
	afx_msg void OnBnClickedOk();

protected:
	virtual void OnOK();

public:
	//afx_msg void OnCbnSelchangeComboTorihikichange();		// 修正No.162490 del
	void TerminationKanaSearchDiag(short index, short nChar, short length, LPUNKNOWN data);
	CButton m_chkRegAuto;
	afx_msg void OnBnClickedCheckRegAuto();
	//afx_msg void OnCbnSetfocusComboTorihikichange();		// 修正No.162437 add	// 修正No.162490 del

	void ComboSelKanaSearchDiag(short index, LPUNKNOWN data);
	void EditOFFIcsdiag8Kanasearch(short index);
	void EditONIcsdiag8Kanasearch(short index);

protected:
	afx_msg	LRESULT OnUser1Message(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnUserFocusset(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnUserFocusset3(WPARAM wParam, LPARAM lParam);		// 修正_23/04/21 add
	afx_msg	LRESULT OnUserSelectCheck(WPARAM wParam, LPARAM lParam);	// 修正No.162394 add

private:
	void	OnNMDblclkReport(NMHDR * pNotifyStruct, LRESULT * result);
	void	OnNMRdblclkReport(NMHDR * pNotifyStruct, LRESULT * result); // 修正No.162345 add

};

#endif	//	__DLGREFERENCE_H__

// 改良No.22-0404,22-0812 add <--
