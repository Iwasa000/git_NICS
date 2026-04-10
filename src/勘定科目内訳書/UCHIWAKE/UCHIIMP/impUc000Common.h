#pragma once
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc000Common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfSub.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfFormType.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcLstBank.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcLstAddress.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcLstKamoku.h"
#include "..\..\UCHIWAKE\UCHIWAKE\UcFunctionCommon.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dateConvert.h"

class CimpUc000Common
{
public:
	CimpUc000Common(void);
	~CimpUc000Common(void);

public:		// 変数
	IMP_CSV_INFO			m_uImpCsvInfo;							// CSVの情報
// midori 150899 add -->
	CMap<int, int, IMP_CSV_INFO, IMP_CSV_INFO>			m_ErrMap;	// エラーマップ(１行分)の初期化
// midori 150899 add <--

	// 一括集計金額データ
	CMap<long, long, IKKATU_CSV_DATA, IKKATU_CSV_DATA>*	m_mapIkkatuData;

public:		// 関数
	// 各帳票固有の処理
	virtual int		Init(CDatabase* pDB);
	virtual int		SetCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add -->
	virtual int		ImpCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add <--
// midori 150905 add -->
	virtual int		SetCsvNullLineData142(void);
// midori 150905 add <--
// midori 150954 add -->
	virtual int		SetCsvNullLineData091(void);
// midori 150954 add <--
	virtual int		GetCsvOneLineData(CStringArray* pstrCsvDataAry);
	virtual void	virTableEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	virtual int		virSpecialLineToNull(int nPage, int MaxRow);
// 157766 del -->
//	virtual int		virFormatedIsNoKamokuVer(CStringArray* pstrCsvDataAry);
//// 210113_5301_1 add -->
//	virtual BOOL	virFormatedIsNewVer(CStringArray* pstrCsvDataAry);
//// 210113_5301_1 add <--
// 157766 del <--
// 210113_5301_1 del -->
// ※未使用のため削除
	//virtual int		virFormatedIsKamokuAddedVer(CStringArray* pstrCsvDataAry);
// 210113_5301_1 del <--
	

	// 全帳表共通の処理
	int			GetAllData(void);
	int			DeleteAllData(void);
	int			Fin(void);
	int			GetIkkatuData(void);
	void		SetIkkatuData(void);
	int			OutIkkatuData(CCsv* csvUcFile);
	void		DeleteIkkatuData(void);
	int			GetFormSeq(void);
// midori 170504 add -->
	BOOL		CheckImportStr(CString strVal, int nSize, BOOL fCheckHankaku = FALSE);
// midori 170504 add <--

	//インポートオプション
	int			ImpPageNew();
	int			ImpRowNew(int nFormSeq, CDatabase* pDB);
	int			ImpUpdatePageNumRowNumAdd(int nTargetPage, int nTargetRow, int nTargetCount);
	int			TableCountMaxRow(void);
	int			TableInsertNullLine( int nTargetPage, int nTargetRow, int nTargetCount );
	int			TableUpdatePageNumRowNumReset(int inPage, int inRow);
	int			TableInsertPageKeiRuiKei(void);
	int			TableInsertSpecialLine(int nPage, int nRow, int nType, CString strName);
	int			TableCalcKei();
	int			GetNextPageRow(int* nCountPage, int* nCountRow);
	int			GetRowMax(void);
	int			GetRowMaxData(void);
	int			GetRowKei(void);

	// その他
// midori 150894 cor -->
	//int			ShowMessageBox(int nErrorCode, CWnd* pWnd = NULL);
// ---------------------
	int			ShowMessageBox(int nErrorCode, CWnd* pWnd = NULL, CDBZmSub* pZmSub = NULL);
// midori 150894 cor <--
// midori 180404 add -->
	int			ShowMessageBox2(int nErrorCode, CWnd* pWnd = NULL);
	CString		GetErrorMessage(void);
// midori 180404 add <--
// midori 156935 add -->
	BYTE		GetOneLineFgFunc(void);
// midori 156935 add <--
// midori 157044 add -->
	void		CheckKeiStr(CDatabase* pDB,int nFormSeq,CStringArray* pstrCsvDataAry);	//※現在未使用
// midori 157044 add <--
// 157766 add <--
	BOOL		FormatedIsNoKamokuVer( CStringArray* pstrCsvDataAry );
// 改良No.21-0086,21-0529 add -->
	//BOOL		FormatedIsNoKamokuVer2(CStringArray* pstrCsvDataAry);		// 様式⑧用
	int			FormatedIsNoKamokuVer2(CStringArray* pstrCsvDataAry);		// 様式⑧用
// 改良No.21-0086,21-0529 add <--
// 登録番号（法人番号）_23/11/21 cor -->
	//int			NumberOfItemCheck( BOOL bKanso, int nFormSeq, CStringArray* pstrCsvDataAry );
// -------------------------------------
	int			NumberOfItemCheck( BOOL bThno, BOOL bKanso, int nFormSeq, CStringArray* pstrCsvDataAry );
// 登録番号（法人番号）_23/11/21 cor <--
// 157766 add <--

protected:	// 変数
	// テーブル
//	CdbUc000Common*			m_pUcCmn;				// 内訳書テーブル（派生クラスにて各帳票テーブルを指定）
//	CdbUcInfSub*			m_pUcInfSub;			// 帳票ごとの管理情報テーブル
	CdbUcLstBank*			m_pUcListBank;			// 金融機関名称テーブル
	CdbUcLstAddress*		m_pUcListAddress;		// 取引先名称テーブル
	CdbUcLstKamoku*			m_pUcListKamoku;		// 科目名称テーブル

	// 操作
	CUcFunctionCommon		m_clsFunc;				// 共通関数クラス
	CdateConvert			m_clsDate;				// 日付変換クラス

	// 内部保持データ
	int						m_NumPage;				// 頁番号
	int						m_NumRow;				// 行番号
	int						m_nFormSeq;				// 様式シーケンス（ID_FORMNO_XXX）
// midori 160609 add -->
	int						m_nKaikakeRangai;		// 1:買掛金の内訳書(欄外)
// midori 160609 add <--
	CString					m_strSyokeiName;		// 小計文言
	CString					m_strChukeiName;		// 中計文言
	int						m_nGroupNum;			// グループ番号
	int						m_nItemCount;			// 項目数
	long					m_nImportCount;			// インポートしようとした回数

	int						m_RowMax;				// １頁内の合計行を含む全体の行数
	int						m_RowMaxData;			// １頁内のデータ行数（合計行は含まず）
	int						m_RowKei;				// 帳表固定の合計行数（1:合計あり，0:なし）
	int						m_FormType;				// 通常か個別か
// midori 152137 add -->
	CFuriganaConverter		fc;
// midori 152137 add <--
// midori 180404 add -->
	BYTE					m_FgFunc;				// 特殊行フラグ
// midori 180404 add <--

public:
	CdbUc000Common*			m_pUcCmn;				// 内訳書テーブル（派生クラスにて各帳票テーブルを指定）
	CdbUcInfSub*			m_pUcInfSub;			// 帳票ごとの管理情報テーブル
	CdbUcInfFormType*		m_pUcInfFormType;		// uc_inf_form_typeテーブル						// 20111007 インポートオプション
	BOOL					m_KamokuVer;			// 科目が追加されているバージョンか(TRUE:追加　FALSE:未追加)CimpUc021Uketoritegataで使用
// 改良No.21-0086,21-0529 add -->
	//BOOL					m_KamokuVer2;			// 科目追加（電子記録債権/債務等）のバージョンか（TRUE:追加版、FALSE:未追加）CimpUc081Siharaitegata で使用
	int						m_KamokuVer2;			// 科目追加（電子記録債権/債務等）のバージョンか（1:追加版、0:未追加）CimpUc081Siharaitegata で使用
// 改良No.21-0086,21-0529 add <--
// 210113_5301_1 add -->
	BOOL					m_NewVer;				// 新様式のバージョンか(TRUE:新様式　FALSE:旧様式)
													// CimpUc042Kasituke,CimpUc111Kariirekin,CimpUc131Uriagedakaで使用
// 登録番号（法人番号）_23/11/21 add -->
	BOOL					m_NewVer2;				// 登録番号（法人番号）追加のバージョンか(TRUE:新様式　FALSE:旧様式)
													// CimpUc021Uketoritegata
													// CimpUc031Urikakekin
													// CimpUc041Karibarai
													// CimpUc042Kasituke
													// CimpUc071Koteisisan
													// CimpUc081Siharaitegata
													// CimpUc091Kaikakekin
													// CimpUc101Kariukekin
													// CimpUc121Totiuriage
													// CimpUc151Tidaiyatin
													// CimpUc152Kenrikin
													// CimpUc153Kougyou
													// CimpUc161Zatueki
													// CimpUc162Zatuson
													// CimpUc171Sonota1
													// で使用
// 登録番号（法人番号）_23/11/21 add <--
	BOOL					m_IsExport;				// TRUE:エクスポート　FALSE:インポート
// 210113_5301_1 add <--
	static BOOL				m_RegAutoRef;			// 「取引先、金融機関を自動で登録する」
// midori 170504 add -->
	CString					m_CSV_181_ITEM_02;		// 項目１（タイトル）CimpUc182Sonota2で使用
	CString					m_CSV_181_ITEM_03;		// 項目２（タイトル）CimpUc182Sonota2で使用
// midori 170504 add <--

protected:	// 関数
	// 全帳表共通処理
// midori 156936 del -->
	//int			SetCsvOneLineDataSub(CStringArray* pstrCsvDataAry, BYTE* pcFgFunc);
// midori 156936 del <--
// midori 156936 add -->
	int			SetCsvOneLineDataSub(CStringArray* pstrCsvDataAry, BYTE* pcFgFunc,int pSw = 0);
// midori 156936 add <--
	int			GetCsvOneLineDataSub(CStringArray* pstrCsvDataAry, CString* pstrCsvId);
	void		ConnectCmnTables(CDatabase* pDB);
	void		GetKeiStr(int nFormSeq, CString* pstrSyokeiName, CString* pstrChukeiName);

	// 名称登録
	BOOL		CheckEntryBank(CString strBkName1, CString strBkName2, long* pnBkSeq, long* pnBkOrder);
// 登録番号（法人番号）_23/11/21 cor -->
	//BOOL		CheckEntryAddress(CString strAdName1, CString strAdName2, int nGrSeq, long* pnAdSeq, long* pnAdOrder);
	//BOOL		CheckEntryAddress(CString strAdName1, CString strAdName2, CString strAdAdd1, CString strAdAdd2, int nGrSeq, long* pnAdSeq, long* pnAdOrder);
// -------------------------------------
	BOOL		CheckEntryAddress(CString strAdName1, CString strAdName2, int nGrSeq, long* pnAdSeq, long* pnAdOrder, CString strInvNo=NULL);
	BOOL		CheckEntryAddress(CString strAdName1, CString strAdName2, CString strAdAdd1, CString strAdAdd2, int nGrSeq, long* pnAdSeq, long* pnAdOrder, CString strInvNo=NULL);
// 登録番号（法人番号）_23/11/21 cor <--
	BOOL		CheckEntryAddressSub(int nGrSeq, long nAdSeq, long* pnAdOrder);
	BOOL		CheckEntryKamoku(CString strKnName, int nFormSeq, long* pnKnSeq, long* pnKnOrder);

	// データチェック
//	BOOL		CheckImportNum(CString strVal, int nDigit);
//	BOOL		CheckImportNum(CString strVal, int nIntegerDigit, int nDecimalDigit);
	BOOL		CheckImportNum(CString strVal, int nDigit, BOOL fMinus);							// [2009/07/09 Minus Value Support]
	BOOL		CheckImportNum(CString strVal, int nIntegerDigit, int nDecimalDigit, BOOL fMinus);	// [2009/07/09 Minus Value Support]
	BOOL		CheckImportNum2(CString strVal, int nDigit, BOOL fMinus);							// [2011/07/15 comma import Value Support]
	BOOL		CheckImportNum2(CString strVal, int nIntegerDigit, int nDecimalDigit, BOOL fMinus);	// [2011/07/15 comma import Value Support]
// midori 170504 del -->
//	BOOL		CheckImportStr(CString strVal, int nSize, BOOL fCheckHankaku = FALSE);
// midori 170504 del <--
	BOOL		CheckStrLength(CString strVal, int nSize);
	BOOL		CheckStrIsHankaku(CString strVal);
// 登録番号（法人番号）_23/11/21 add -->
	BOOL		CheckImportTno( CString& strVal );
// 登録番号（法人番号）_23/11/21 add <--

	// データ加工（チェックを含む）
// midori 156936 del -->
	//BOOL		ConvCsvDataIdStrToDbVal(CString strCsvId, BYTE* pcFgFunc);
// midori 156936 del <--
// midori 156936 add -->
	BOOL		ConvCsvDataIdStrToDbVal(CString strCsvId, BYTE* pcFgFunc, int pSw);
// midori 156936 add <--
	BOOL		ConvCsvDataIdDbValToStr(BYTE cFgFunc, BYTE cFgShow, CString* pstrCsvId);
	// 日付データ加工（チェックを含む）
	BOOL		ConvDateStrToDbVal(int nConvType, CString strDate, long* pnDate);
	BOOL		ConvDateDbValToStr(int nConvType, long nDate, CString* pstrDate);
	BOOL		ConvRomanGgengouStrToInt(CString strGengou, int* pnGengou);
	BOOL		ConvRomanGgengouIntToStr(int nGengou, CString* pstrGengou);
	BOOL		ConvGgengouStrToInt(CString strGengou, int* pnGengou);

// midori 16/05/20 add -->
	// ＣＳＶ情報からインポートエラーに関する項目のみクリア
	void		CsvInfoClr(IMP_CSV_INFO* pCi);
// midori 16/05/20 add -->

	// 文字列計算
	CString		CalcStrMul(CString strTarget, double nMagnify , int nDecimalDigit = 0, double* pnResult = NULL);

	// その他
	void		CheckNumPageRow(int* pnNumPage, int* pnNumRow);
	int			GetRow(int import_count);
	int			GetPage(int row);
// midori 180404 add -->
	CString		GetSpritString( CString szSrc, CString* szDst, int nPos );
// midori 180404 add -->
// midori 156937 add -->
	int			GetKamokuKeiStrMain(int nFormSeq,int pKmkRowSw,CString szSrc,CString strDefSyokei, CString sKamoku, CString* szDst);
// midori 156937 add <--
// midori 156189,156190,156191 add -->
	void		GetKamokuKeiStr(CString szSrc,CString sKamoku, CString* szDst);
// midori 156189,156190,156191 add <--
};
