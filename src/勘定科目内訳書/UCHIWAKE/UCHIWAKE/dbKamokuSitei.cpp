//**************************************************************************************************
//
// ClassName	: CdbKamokuSitei
// FileName		:  dbKamokuSitei.cpp
//
//**************************************************************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "StdAfx.h"
#include ".\dbKamokuSitei.h"
// midori 156189,156190,156191 add -->
#include "UcFunctionCommon.h"
// midori 156189,156190,156191 add <--

extern	BOOL	bG_Kanso;			// midori 190505 add
extern	BOOL	bG_InvNo;			// 改良No.21-0086,21-0529 add

CdbKamokuSitei::CdbKamokuSitei(CDatabase* pdb, int pFormSeq)
{
	m_pDb		= pdb;
	m_nFormSeq	= pFormSeq;
#ifdef _DEBUG
	m_sTblName.Format(_T("##temp_utiwake_tbl_%d"),m_nFormSeq);
#else
	m_sTblName.Format(_T("#temp_utiwake_tbl_%d"),m_nFormSeq);
#endif
	m_ErrorMsg = _T("");
	m_ErrorCode = DB_ERR_OK;
	m_NullSyokeiRow = FALSE;
// midori 152137 add -->
	m_KamokuSort = 0;
// midori 152137 add <--
}

CdbKamokuSitei::~CdbKamokuSitei(void)
{
}

//******************************************************************************
// SQL文を実行する 補助メソッド
//******************************************************************************
int CdbKamokuSitei::ExecuteSQLWork(CString inStrSql)
{
	int			retVal = DB_ERR;

	TRY
	{
		m_pDb->ExecuteSQL(inStrSql);
		retVal = DB_ERR_OK;						//成功
	}
	CATCH(CDBException, e)
	{
		// エラー コードは e->m_nRetCode に入ります。
		retVal = DB_ERR_EXESQL;					// ExecuteSQL() 失敗
	}

	END_CATCH

	return retVal;
}

//******************************************************************************
// テーブルが開けるかチェックを行う
//******************************************************************************
BOOL CdbKamokuSitei::CheckTableOpen( CdbBase* mfcRec )
{
	// テーブルを開く
	if ( mfcRec->Init() != DB_ERR_OPEN ){
		// テーブル閉じる
		mfcRec->Fin();
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//******************************************************************************
// 科目指定に必要な必要な情報を取得する
//******************************************************************************
// midori 190505 del -->
//int CdbKamokuSitei::Initialize( void )
// midori 190505 del <--
// midori 190505 add -->
int CdbKamokuSitei::Initialize( int pSw )
// midori 190505 add <--
{
	CdbUcInfSub			mfcRec(m_pDb);		// uc_inf_subテーブルクラス
	CdbUcInfFormType	mfcRec2(m_pDb);		// uc_inf_form_typeテーブルクラス

// midori 190505 add -->
	m_pSw = pSw;
// midori 190505 add <--
	// -------------------------------------------------------------
	// 科目指定、小計、一括の情報を取得
	// -------------------------------------------------------------
	m_ErrorMsg = _T("Initialize CdbUcInfSub 該当レコードなし");
	m_ErrorCode = DB_ERR_EOF;
	if(mfcRec.RequeryFormSeq(m_nFormSeq) == DB_ERR_OK)	{
		if(!mfcRec.IsEOF())	{
			mfcRec.MoveFirst();
		}
		// 
		// 科目指定
		// 
		// 科目未入力を出力する
		if(mfcRec.m_GeneralVar[2] & BIT_D0)	m_KmPrMiny=0;
		else								m_KmPrMiny=1;
// midori M-16113008 del -->
		//// 科目でソートを行う
		//if(mfcRec.m_GeneralVar[1] & BIT_D4)	m_KmPrSort=1;
		//else								m_KmPrSort=0;
// midori M-16113008 del <--
		// 改頁を行う
		if(mfcRec.m_GeneralVar[1] & BIT_D5)	m_KmPrKaip=1;
		else								m_KmPrKaip=0;
// midori 190505 add -->
		// 科目行を自動挿入する
		if(m_pSw == 0) {
			CCtrlSubGeneralVar cs( mfcRec );
			if( cs.bOutKamokuRow )				m_KmPrKtitle=1;
			else								m_KmPrKtitle=0;
		}
		else {
			if(mfcRec.m_GeneralVar[2] & BIT_D3)	m_KmPrKtitle=1;
			else								m_KmPrKtitle=0;
		}
// midori 190505 add <--
		// 小計を自動挿入する
		if(mfcRec.m_GeneralVar[1] & BIT_D6)	m_KmPrSkei=1;
		else								m_KmPrSkei=0;
		// 一括金額を行う
		if(mfcRec.m_GeneralVar[1] & BIT_D7)	m_KmPrIkat=1;
		else								m_KmPrIkat=0;
		// 
		// 小計
		// 
		// 一行でも小計を挿入する
		m_OpOutSyokei = mfcRec.m_OpOutSyokei;
		// 小計の名称
		m_OpSykeiName = mfcRec.m_OpSykeiName;
		// 小計用括弧の種類番号
		m_OpLeft = BRACKET_KIND[mfcRec.m_OpSykeiKakko].szLeft;
		m_OpRight = BRACKET_KIND[mfcRec.m_OpSykeiKakko].szRight;
		// 小計の次に空行を挿入する
		m_OpOutNullLine = mfcRec.m_OpOutNullLine;
		// 
		// 一括
		// 
		// 一括金額
		m_OpIkkatuVal = mfcRec.m_OpIkkatuVal;
		// 詳細設定を使用する
		if(mfcRec.m_GeneralVar[1] & BIT_D2)	m_isAdvSet=TRUE;
		else								m_isAdvSet=FALSE;
		// 一括金額に０円を含まない
		if(mfcRec.m_GeneralVar[0] & BIT_D5)	m_IkkatuOutZero=1;
		else								m_IkkatuOutZero=0;
		// 
		// 様式ごとの情報
		// 
		// 計設定内容	（uc_inf_sub）
		intOutKei = mfcRec.m_OutKei;
		// 計の名称		（uc_inf_sub）
		if(mfcRec.m_GeneralVar[0] & BIT_D6)	m_NameKei=1;
		else								m_NameKei=0;

// midori 152137 add -->
		// 表示順 0:番号順 1:表示順
		if(mfcRec.m_GeneralVar[2] & BIT_D1)	m_KamokuSort=1;
		else								m_KamokuSort=0;
// midori 152137 add <--

		mfcRec.Fin();

		m_ErrorMsg = _T("");
		m_ErrorCode = DB_ERR_OK;
	}

	// 帳表の通常/個別の管理情報
	m_FormType04 = 0;
	m_FormType10 = 0;
	m_FormType15 = 0;
	m_FormType16 = 0;
	if(mfcRec2.Init(4) == DB_ERR_OK)	{
		if(!(mfcRec2.IsEOF()))	mfcRec2.MoveFirst();
		mfcRec2.Fin();
		m_FormType04 = mfcRec2.m_FormType;	// 仮払/貸付
	}
	if(mfcRec2.Init(10) == DB_ERR_OK)	{
		if(!(mfcRec2.IsEOF()))	mfcRec2.MoveFirst();
		mfcRec2.Fin();
		m_FormType10 = mfcRec2.m_FormType;	// 仮受/源泉
	}
	if(mfcRec2.Init(15) == DB_ERR_OK)	{
		if(!(mfcRec2.IsEOF()))	mfcRec2.MoveFirst();
		mfcRec2.Fin();
		m_FormType15 = mfcRec2.m_FormType;	// 地代/権利/工業
	}
	if(mfcRec2.Init(16) == DB_ERR_OK)	{
		if(!(mfcRec2.IsEOF()))	mfcRec2.MoveFirst();
		mfcRec2.Fin();
		m_FormType16 = mfcRec2.m_FormType;	// 雑益/雑損
	}

	// -------------------------------------------------------------
	// 様式ごとの情報(固定値)をセット
	// -------------------------------------------------------------
	if(m_nFormSeq == ID_FORMNO_011)			{
		intRowMax		= 24;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_021)	{
		intRowMax		= 24;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_031)	{
		intRowMax		= 27;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_041)	{
		if(m_FormType04 == 0)	{
			intRowMax		= 11;
			intRowMaxData	= intRowMax - 1;
		}
		else					{
			intRowMax		= 29;
			intRowMaxData	= intRowMax - 1;
		}
		intRowKei		= 0;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_051)	{
		intRowMax		= 25;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_061)	{
		intRowMax		= 16;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 2;
		strField[0]		= _T("Val2");
		strField[1]		= _T("Val3");
	}
// 改良No.21-0086,21-0529 add -->
	else if(m_nFormSeq == ID_FORMNO_081) {
		if(bG_InvNo == TRUE) {
			intRowMax		= 24;
			intRowMaxData	= intRowMax - 1;
			intRowKei		= 1;
			intMaxCount		= 1;
			strField[0]		= _T("Val");
		}
	}
// 改良No.21-0086,21-0529 add <--
	else if(m_nFormSeq == ID_FORMNO_091)	{
		intRowMax		= 24;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_101)	{
		if(m_FormType10 == 0)	{
			intRowMax		= 12;
			intRowMaxData	= intRowMax - 1;
		}
		else					{
			intRowMax		= 24;
			intRowMaxData	= intRowMax - 1;
		}
		intRowKei		= 0;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_102)	{
		if(m_FormType10 == 0)	{
			intRowMax		= 14;
			intRowMaxData	= intRowMax - 1;
		}
		else					{
			intRowMax		= 52;
			intRowMaxData	= intRowMax - 1;
		}
		intRowKei		= 0;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_111)	{
		intRowMax		= 15;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 2;
		strField[0]		= _T("Val");
		strField[1]		= _T("Risoku");
	}
	else if(m_nFormSeq == ID_FORMNO_121)	{
		intRowMax		= 10;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 0;
		intMaxCount		= 2;
		strField[0]		= _T("Val");
		strField[1]		= _T("Val2");
	}
	else if(m_nFormSeq == ID_FORMNO_151)	{
		if(m_FormType15 == 0)	{
			intRowMax		= 8;
			intRowMaxData	= intRowMax - 1;
		}
		else					{
			intRowMax		= 19;
			intRowMaxData	= intRowMax - 1;
		}
		intRowKei		= 0;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_161)	{
		if(m_FormType16 == 0)	{
			intRowMax		= 10;
			intRowMaxData	= intRowMax - 1;
		}
		else					{
			intRowMax		= 20;
			intRowMaxData	= intRowMax - 1;
		}
		intRowKei		= 0;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq == ID_FORMNO_162)	{
		if(m_FormType16 == 0)	{
			intRowMax		= 10;
			intRowMaxData	= intRowMax - 1;
		}
		else					{
			intRowMax		= 20;
			intRowMaxData	= intRowMax - 1;
		}
		intRowKei		= 0;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}
	else if(m_nFormSeq >= ID_FORMNO_171)	{
		intRowMax		= 27;
		intRowMaxData	= intRowMax - 1;
		intRowKei		= 1;
		intMaxCount		= 1;
		strField[0]		= _T("Val");
	}

	// -------------------------------------------------------------
	// 出力形式から１頁の最大行数を取得
	// -------------------------------------------------------------
	if(intRowKei == 1)	{
		// 計あり帳表
		intRowMaxData = intRowMax - 1;
	}
	else if((intOutKei == ID_OUTKEI_PAGEKEI) || (intOutKei == ID_OUTKEI_BOTH))	{
		// 頁計 or 頁計/累計
		intRowMaxData = intRowMax - 1;
	}
	else {
		// 最下段もデータとして使用する
		intRowMaxData = intRowMax;
	}

	// -------------------------------------------------------------
	// ソート項目情報(固定値)をセット
	// -------------------------------------------------------------
	if(m_nFormSeq == ID_FORMNO_011)			m_Sort1 = 2;	// ①  :預貯金等の内訳書	-> 2:種類
	else if(m_nFormSeq == ID_FORMNO_021)	m_Sort1 = 8;	// ②  :受取手形の内訳書	-> 8:科目
// 改良No.21-0086,21-0529 add -->
	else if(m_nFormSeq == ID_FORMNO_081)	{
		if(bG_InvNo == TRUE)				m_Sort1 = 9;	// ⑧  :支払手形の内訳書	-> 9:科目
	}
// 改良No.21-0086,21-0529 add <--
	else if(m_nFormSeq == ID_FORMNO_102)	m_Sort1 = 3;	// ⑩-2:源泉所得税預り金	-> 3:所得の種類
	else									m_Sort1 = 1;	// 上記以外					-> 1:科目

	return(m_ErrorCode);
}

//******************************************************************************
// 指定科目以外を一時テーブルから削除する
//******************************************************************************
// midori M-16113008 del -->
//int CdbKamokuSitei::SiteigaiDelete( int pSort )
// midori M-16113008 del <--
// midori M-16113008 add -->
int CdbKamokuSitei::SiteigaiDelete( void )
// midori M-16113008 add <--
{
	CString		strSQL=_T("");
	CString		strTemp=_T("");

	m_ErrorMsg = _T("");
	m_ErrorCode = DB_ERR_OK;

	// ① 指定科目以外を削除
	strSQL.Format(_T("delete a from %s a left join uc_lst_kamoku b on a.knseq=b.knseq where b.prsign=1"),m_sTblName);
	if(ExecuteSQLWork(strSQL) != DB_ERR_OK)	{
		return(DB_ERR_EXESQL);
	}
	// ②【科目未入力】チェックＯＦＦの場合は科目未入力を削除
	if(m_KmPrMiny == 0)	{
		// GetCountDataRecord(1,1)と同じ条件で抽出したデータを削除
		strSQL.Format(_T("delete %s "),m_sTblName);
		// 1.空行以外のデータ
		strTemp.Format("WHERE FgFunc<>%d AND FgFunc<>%d AND FgFunc<>%d",ID_FGFUNC_NULL,ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
		strSQL += strTemp;
		//// 2.表示データ
		//strTemp.Format(_T(" AND FgShow=%d"),ID_FGSHOW_OFF);
		//strSQL += strTemp;
		// 3.科目未入力かつ、通常データまたは一括集計金額行(手動)
		// ID_FGFUNC_DATA				1	データ行
		// ID_FGFUNC_DATA_TEMP			4	データ行（０円出力での一時テーブルで使用）
		// ID_FGFUNC_IKKATUMANUAL_ZERO	5	一括集計金額行（手動）（０円出力での一時テーブルで使用）
		// ID_FGFUNC_IKKATUAUTO_ZERO	6	一括集計金額行（自動）（０円出力での一時テーブルで使用）
		// ID_FGFUNC_IKKATUMANUAL		21	一括集計金額行（手動）
		// ID_FGFUNC_IKKATUAUTO			22	一括集計金額行（自動）
		//strTemp.Format(_T(" AND (KnSeq=0 OR KnSeq IS NULL) AND (FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d)"),
		strTemp.Format(_T(" AND (KnOrder=0 OR KnOrder IS NULL) AND (FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d)"),
			ID_FGFUNC_DATA,ID_FGFUNC_IKKATUMANUAL,ID_FGFUNC_KAMOKU,ID_FGFUNC_DATA_TEMP,ID_FGFUNC_IKKATUMANUAL_ZERO,ID_FGFUNC_KAMOKU);
		strSQL += strTemp;
		if(ExecuteSQLWork(strSQL) != DB_ERR_OK)	{
			return(DB_ERR_EXESQL);
		}
	}
// midori M-16113008 del -->
	//else if(pSort)	{
	//	// 科目でソートを行う場合、未入力は最終に出力する
// midori M-16113008 del <--
// midori M-16113008 add -->
	else	{
		// 未入力は最終に出力する
// midori M-16113008 add <--
		// GetCountDataRecord(1,1)と同じ条件で抽出したデータを更新
// midori 152137 del -->
//		strSQL.Format(_T("update %s set KnOrder=99999"),m_sTblName);	// 4:科目の最大桁数
// midori 152137 del <--
// midori 152137 add -->
		strSQL.Format(_T("update %s set KnOrder=99999, KnKana='ﾝﾝﾝﾝﾝﾝ'"),m_sTblName);	// 4:科目の最大桁数
// midori 152137 add <--
		// 1.空行以外のデータ
		strTemp.Format("WHERE FgFunc<>%d AND FgFunc<>%d AND FgFunc<>%d",ID_FGFUNC_NULL,ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
		strSQL += strTemp;
		// 2.表示データ
		strTemp.Format(_T(" AND FgShow=%d"),ID_FGSHOW_OFF);
		strSQL += strTemp;
		// 3.科目未入力かつ、通常データまたは一括集計金額行(手動)
		strTemp.Format(_T(" AND (KnOrder=0 OR KnOrder IS NULL) AND (FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d)"),
			ID_FGFUNC_DATA,ID_FGFUNC_IKKATUMANUAL,ID_FGFUNC_DATA_TEMP,ID_FGFUNC_IKKATUMANUAL_ZERO);
		strSQL += strTemp;
		if(ExecuteSQLWork(strSQL) != DB_ERR_OK)	{
			return(DB_ERR_EXESQL);
		}
	}

	return(m_ErrorCode);
}

// midori 190505 del -->
////******************************************************************************
//// ０円除外、科目行を出力する場合は一時テーブルに非表示の
//// 一括行(FgShow=2)が抽出されていないので抽出する
////******************************************************************************
//int CdbKamokuSitei::AddIkkatuData( CString strMtbl )
//{
//	CString		strSQL=_T("");
//
//	m_ErrorMsg = _T("");
//	m_ErrorCode = DB_ERR_OK;
//
//	strSQL.Format(_T("insert into %s select * from %s where FgShow=%d"),m_sTblName,strMtbl,ID_FGSHOW_IKKATU);
//	if(ExecuteSQLWork(strSQL) != DB_ERR_OK)	{
//		return(DB_ERR_EXESQL);
//	}
//
//	return(m_ErrorCode);
//}
// midori 190505 del <--

//******************************************************************************
//	ソート(OnMenuSort)実行前に必要なテーブルがオープン可かチェックする
//******************************************************************************
BOOL CdbKamokuSitei::Ks_CmnCheckTableSort()
{
	// ソート項目情報リストテーブルのチェック
	CdbUcLstItemSort mfcSort(m_pDb);
	if(CheckTableOpen((CdbBase*)&mfcSort) != TRUE)	{
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
//	ソート：ソート処理（メイン）
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblSortMainProc()
{
	int			save=0;
	CString		strSQL=_T("");
	CString		strFilter=_T("");
	CString		strSort=_T("");

	// ソート実行の前処理 ･･･ 小計/中計等の不要データ削除＋一括データを通常データに戻す
	if(Ks_CmnTblSortSubSortBeforeProc() != FUNCTION_OK)	{
		return(FUNCTION_NG);
	}

	// 帳表毎のテーブルクラスを"New"する
	CdbPrtWork*		rsData;
	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

// midori 190505 del -->
	// 画面上の科目行は Ks_CmnTblSortSubSortBeforeProc 関数で削除しているので、以下は不要
	//// ※1 科目見出し行がソートの対象になるように、フラグ(FgShow)を書き換える
	//strSQL.Format(_T("UPDATE %s SET FgShow=%d WHERE FgFunc=%d AND FgShow IS NULL"),m_sTblName,ID_FGSHOW_OFF,ID_FGFUNC_KAMOKU);
	//ExecuteSQLWork(strSQL);

	//// ※2 この後ソートを行うので、同じ科目見出し行が２行以上ある場合１行にする
	//Ks_CmnTblSortSubGetSortParam(&strFilter,&strSort);
	//strSQL.Format(_T(" AND FgFunc=%d"),ID_FGFUNC_KAMOKU);
	//strFilter = strFilter + strSQL;
	//if(rsData->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
	//	rsData->Fin();
	//	delete rsData;
	//	return(FUNCTION_NG);
	//}
	//save=-1;
	//while(!rsData->IsEOF())	{
	//	if(rsData->m_KnSeq == save)	{
	//		save = rsData->m_KnSeq;
	//		rsData->Delete();
	//	}
	//	else	{
	//		save = rsData->m_KnSeq;
	//	}
	//	// 次のレコードへ
	//	rsData->MoveNext();
	//}
// midori 190505 del <--

	// ソート実行
	if(Ks_TblSortSubSortProc(rsData) != FUNCTION_OK)	{
		rsData->Fin();
		delete rsData;
		return(FUNCTION_NG);
	}

	// ソート実行の後処理･･･改頁挿入，小計行挿入，暫定頁設定
	if(Ks_CmnTblSortSubSortAfterProc(rsData) != FUNCTION_OK)	{
		rsData->Fin();
		delete rsData;
		return(FUNCTION_NG);
	}

	rsData->Fin();
	delete rsData;

	return(FUNCTION_OK);
}

//******************************************************************************
//	ソート：初期データ設定
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblSortSubSortBeforeProc()
{
	CdbPrtWork*			rsData;
	CString				strSQL=_T("");
	CString				strTemp=_T("");

	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 帳表テーブルをデータのみとする
	//	→ "FgFunc>200"は帳表固有データのため扱いに注意する事
	// 空行
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_NULL);
	ExecuteSQLWork(strSQL);
	// 小計行
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_SYOKEI);
	ExecuteSQLWork(strSQL);
	// 小計行（空行）
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_SYOKEINULL);
	ExecuteSQLWork(strSQL);
	// 中計行
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_CHUKEI);
	ExecuteSQLWork(strSQL);
	// 中計行（空行）
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_CHUKEINULL);
	ExecuteSQLWork(strSQL);
	// 累計行
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_RUIKEI);
	ExecuteSQLWork(strSQL);
	// 頁計行
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_PAGEKEI);
	ExecuteSQLWork(strSQL);
	// 一括集計金額行（自動）
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_IKKATUAUTO);
	ExecuteSQLWork(strSQL);
	// 一括集計金額行（自動）（０円出力での一時テーブルで使用）
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_IKKATUAUTO_ZERO);
	ExecuteSQLWork(strSQL);
// midori 190505 add -->
	// 科目行(見出し)
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_KAMOKU);
	ExecuteSQLWork(strSQL);
// midori 190505 add <--

	// 一括データを表示データとする（FgShow:2→0）･･･ 一時保管データは対象外
	// ※一括データだったデータが必ず後ろに並ぶように、頁番号を(最大値 + 1)に更新
	if (rsData->UpdateFgShowPageRow(ID_FGSHOW_IKKATU, ID_FGSHOW_OFF, ID_DATA_PAGE_IKKATU_TEMP, 0) != DB_ERR_OK) {
		rsData->Fin();
		delete rsData;
		return(FUNCTION_NG);
	}

	// ※一括データを表示データとしたので、科目未入力のソート順を最後にもってくる
// midori 152137 del -->
//	strSQL.Format(_T("update %s set KnOrder=99999"),m_sTblName);
// midori 152137 del <--
// midori 152137 add -->
	strSQL.Format(_T("update %s set KnOrder=99999, KnKana = 'ﾝﾝﾝﾝﾝﾝ' "),m_sTblName);
// midori 152137 add <--
	// 1.空行以外のデータ
	strTemp.Format("WHERE FgFunc<>%d AND FgFunc<>%d AND FgFunc<>%d",ID_FGFUNC_NULL,ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
	strSQL += strTemp;
	// 2.表示データ
	strTemp.Format(_T(" AND FgShow=%d"),ID_FGSHOW_OFF);
	strSQL += strTemp;
	// 3.科目未入力かつ、通常データまたは一括集計金額行(手動)
	strTemp.Format(_T(" AND (KnOrder=0 OR KnOrder IS NULL) AND (FgFunc=%d OR FgFunc=%d OR FgFunc=%d OR FgFunc=%d)"),
		ID_FGFUNC_DATA,ID_FGFUNC_IKKATUMANUAL,ID_FGFUNC_DATA_TEMP,ID_FGFUNC_IKKATUMANUAL_ZERO);
	strSQL += strTemp;
	ExecuteSQLWork(strSQL);

	// グループ番号をクリア
	//	　本来は"FgShow=2"だけ実行すれば良いが、一括集計処理でグループ番号を使って
	//	　抽出するため、より安全のために"FgShow:2→0"実行後に行う
	if (rsData->UpdateFgShowClearGroup(ID_FGSHOW_OFF) != DB_ERR_OK) {
		rsData->Fin();
		delete rsData;
		return(FUNCTION_NG);
	}

	rsData->Fin();
	delete rsData;

	return(FUNCTION_OK);
}

//******************************************************************************
// ソート項目を再設定後に、ソートを実行する
//******************************************************************************
int	CdbKamokuSitei::Ks_TblSortSubSortProc( CdbPrtWork* rsData )
{
	CdbPrtWork*			rs;
	CString				strFilter;
	CString				strSort;

	// ソート条件取得
	Ks_CmnTblSortSubGetSortParam(&strFilter,&strSort);

	rs = rsData;

	// ソート実行
	return(rs->RequerySortParameter(strFilter,strSort));
}

//******************************************************************************
//	ソート：ソート条件取得
//******************************************************************************
void CdbKamokuSitei::Ks_CmnTblSortSubGetSortParam(CString* str1, CString* str2)
{
// midori 152137 add -->
	int					nTemp=0;
	int					sw = 0;
// midori 152137 add <--
	CString				strFilter=_T("");
	CString				strSort=_T("");
	ITEMSORT_INFO		uItemSort;

	// 第１ソート項目の項目種類 ･･･ 他でも使っているけどまたここで取得
	Ks_CmnUcLstItemSortGetData(m_Sort1, &uItemSort);

	// ソート順を取得
// midori 152137 del -->
	//if (uItemSort.strOrderStr != "") {
	//	// ソート条件を設定
	//	strSort = uItemSort.strOrderStr;
	//}
// midori 152137 del <--
// midori 152137 add -->
	sw = 0;
	if(m_Sort1 > 0)	{
		nTemp = GetSortItem(m_nFormSeq,m_Sort1);
		sw = 0;
		// 科目（カナ入力有）
		if(nTemp == 1)	{
			// 0:番号順 1:50音順
			if(m_KamokuSort == 1)	sw = 1;
		}
	}

	if(sw == 0)	{
		if (uItemSort.strOrderStr != "") {
			// ソート条件を設定
			strSort = uItemSort.strOrderStr;
		}
	}
	else		{
		if (uItemSort.strOrderStr2 != "") {
			// ソート条件を設定
			strSort = uItemSort.strOrderStr2;
		}
	}
// midori 152137 add <--

	// 第１ソート項目が科目だった場合、次のソート条件にFgFuncを指定
	if (strSort == "") {
		strSort = _T("FgFunc ASC");
	}
	else {
		strSort = strSort + _T(", FgFunc ASC");
	}
	
	// 表示データのみでソート実行（FgShow=0）
	//	　科目指定：手動一括集計金額(21)を通常データ(0)の科目が一致するがでも最後に表示（"FgFunc"が後で必要）
	//	　科目なし：通常データ(0)→手動一括集計金額(21)として手動データは最後に表示（"FgFunc"が先に必要）
	strFilter.Format("FgShow = %d", ID_FGSHOW_OFF);

	// ソートしたデータが、入力順に並ぶよう対応
	// ※指定しない場合、並び順が保障されないため
	if (strSort == "") {
		strSort = _T("NumPage ASC, NumRow ASC");
	}
	else {
		strSort = strSort + _T(", NumPage ASC, NumRow ASC");
	}

	// ソート条件
	*str1 = strFilter;
	*str2 = strSort;
}

//******************************************************************************
//	uc_lst_item_sort：データ取得
//******************************************************************************
BOOL CdbKamokuSitei::Ks_CmnUcLstItemSortGetData(int intItemSeq, ITEMSORT_INFO* uItemSort)
{
	CdbUcLstItemSort	rsLstIS( m_pDb );
	BOOL				bRet = FALSE;

	// データ初期化
	uItemSort->bytFgSykei		= 0;			// 小計の対象である/ない
	uItemSort->bytFgItem		= 0;			// 項目の種類を示す番号
	uItemSort->strOrderStr		= "";			// 並び替え用SQL文字列
	uItemSort->strFieldSykei	= "";			// 小計のグループ判定に使用するフィールド名
	uItemSort->strFieldSykei2	= "";			// 小計のグループ判定に使用するフィールド名
// midori 152137 add -->
	uItemSort->strOrderStr2		= _T("");		// 50音並び替え用SQL文字列
// midori 152137 add <--
// 20-0450 add -->
	uItemSort->intItemSeq		= 0;
// 20-0450 add <--

	// パラメータチェック
	if ((m_nFormSeq <= 0) || (intItemSeq <= 0)) {
		rsLstIS.Fin();
		return FALSE;
	}

	// クエリー実行
	if (rsLstIS.RequeryFormItem(m_nFormSeq, intItemSeq) == DB_ERR_OK) {
		if (!rsLstIS.IsEOF()) {
			// 必要情報をセット
			uItemSort->bytFgSykei		= rsLstIS.m_FgSykei;		// 小計の対象である/ない
			uItemSort->bytFgItem		= rsLstIS.m_FgItem;			// 項目の種類を示す番号
			uItemSort->strOrderStr		= rsLstIS.m_OrderStr;		// 並び替え用SQL文字列
			uItemSort->strFieldSykei	= rsLstIS.m_FieldSykei;		// 小計のグループ判定に使用するフィールド名
			uItemSort->strFieldSykei2	= rsLstIS.m_FieldSykei2;	// 小計のグループ判定に使用するフィールド名
// midori 152137 add -->
			uItemSort->strOrderStr2		= rsLstIS.m_OrderStr2;		// 50音並び替え用SQL文字列
// midori 152137 add <--
// 20-0450 add -->
			uItemSort->intItemSeq		= rsLstIS.m_ItemSeq;
// 20-0450 add <--

			bRet = TRUE;
		}
	}
	rsLstIS.Fin();

	return bRet;
}

// 158063 add -->
//*************************************************************************************************
//	取引先毎の総額で判定するかを返送する
//		【引数】	int		様式番号
//		【戻値】	int		0:判定しない　1:判定する	2:判定する（詳細設定チェックあり）
//*************************************************************************************************
int CdbKamokuSitei::IsTorihikiAmount( int pFormSeq )
{
	int		ret=0;

	if(pFormSeq == ID_FORMNO_021 || pFormSeq == ID_FORMNO_081)	{
		ret = 1;
		//if(pFormSeq == ID_FORMNO_021)	{	// 改良No.21-0086,21-0529 del
			if(ret != 0)	{
				if(m_isAdvSet == TRUE)	{
					ret = 2;
				}
			}
		//}									// 改良No.21-0086,21-0529 del
	}
	return(ret);
}
// 158063 add <--

//******************************************************************************
// ソート：ソート後の一括集計金額行＋小計行＋改頁挿入処理
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblSortSubSortAfterProc(CdbPrtWork* rsData)
{
	ITEMSORT_INFO		uItemSort;					// 第１ソート項目のソート情報
	CString				strOldItem1 = "";			// 第１ソート項目の値（old）
	CString				strNewItem1 = "";			// 第１ソート項目の値（new）
	CString				strOldItem2 = "";			// 第１ソート項目の値（old）
	CString				strNewItem2 = "";			// 第１ソート項目の値（new）
	BOOL				bChangeItem = FALSE;		// 第１ソート項目の変化フラグ
// midori 190505 add -->
	BOOL				bKmkTitle1 = FALSE;			// 科目(見出し)行の挿入サイン①
	BOOL				bKmkTitle2 = FALSE;			// 科目(見出し)行の挿入サイン②
// midori 190505 add <--
	SORTKAMOKU_INFO		uOldKamoku;					// 前レコードの科目情報
	SORTKAMOKU_INFO		uNullKamoku;				// 空の科目情報
	CString				strSykeiName = "";			// カッコ付き小計名称
	BOOL				bIkkatu = FALSE;			// 一括集計対象の判断
	CString				strVal;						// 一括集計で使用する金額値
	CString				strTemp;
	int					nNumGroup = 1;				// 一括集計で使用するグループ番号
	int					nIkkatuLine = 0;			// 一括集計の対象行
	int					nPage = 1;					// 暫定頁設定で使用する頁番号
	int					nRow = 0;					// 暫定頁設定で使用する行番号
	int					nSyokeiLine = 0;			// 小計対象行数
	int					nKaiPageLine = 0;			// 改頁対象行数
	int					i = 0;
// 158063 add -->
	int					amsw=0;
	CUcFunctionCommon	uc;
// 158063 add <--

	// 全行が空行なら"EOF"のため処理しない
	if (rsData->IsEOF()) {
		return	FUNCTION_OK;
	}

	// uc_lst_item_sortを参照して必要情報を取得
	Ks_CmnUcLstItemSortGetData(m_Sort1, &uItemSort);

	// カッコ付き小計名称を取得
	strSykeiName = Ks_CmnGetSykeiNameWithKakko(m_OpSykeiName,m_OpLeft,m_OpRight);

	// 科目情報を初期化
	uOldKamoku.intKnSeq = 0;
	uOldKamoku.intKnOrder = 0;
	uOldKamoku.strKnName = _T("");
// midori 152137 add -->
	uOldKamoku.strKnKana = _T("");
// midori 152137 add <--

	uNullKamoku = uOldKamoku;

// midori 190505 add -->
	// 科目(見出し)行の挿入
	bKmkTitle1 = FALSE;
	if(m_pSw == 1) {
// 改良No.21-0086,21-0529 cor -->
		//if((m_nFormSeq == ID_FORMNO_021) || (m_nFormSeq == ID_FORMNO_111))	{
// ------------------------------
		if((m_nFormSeq == ID_FORMNO_021) || (m_nFormSeq == ID_FORMNO_081 && bG_InvNo == TRUE) || (m_nFormSeq == ID_FORMNO_111))	{
// 改良No.21-0086,21-0529 cor <--
			if((uItemSort.bytFgItem == ID_FGITEM_KAMOKU) && (m_KmPrKtitle == 1))	{
				bKmkTitle1 = TRUE;
			}
		}
	}
	bKmkTitle2 = TRUE;
// midori 190505 add <--

	// １件目の第１ソート項目の値を"old"へセット
	strOldItem1 = rsData->GetFieldValueString(uItemSort.strFieldSykei);
	strOldItem2 = rsData->GetFieldValueString(uItemSort.strFieldSykei2);

// 158063 add -->
	if(m_KmPrIkat == 1)	{
		amsw = IsTorihikiAmount(m_nFormSeq);
		if(amsw != 0)	{
			// ②受取手形の内訳書、⑧支払手形の内訳書は総額のマップを作成する
			if(amsw == 1)	uc.MakeAmountMap1(m_pDb,m_nFormSeq);
			else			uc.MakeAmountMap2(m_pDb,m_nFormSeq);
		}
	}
// 158063 add <--

	while (!rsData->IsEOF()) {
		// 暫定：頁設定（改ページによる空行の数を把握したい）→ 後で正式な頁設定を行う
		if (rsData->m_FgShow == ID_FGSHOW_OFF) {
			uOldKamoku.intKnSeq		= rsData->m_KnSeq;
			uOldKamoku.intKnOrder	= rsData->m_KnOrder;
			uOldKamoku.strKnName	= rsData->m_KnName;
// midori 152137 add -->
			uOldKamoku.strKnKana	= rsData->m_KnKana;
// midori 152137 add <--

			// 一括集計対象を判断 ･･･ 手動一括集計金額行は対象外とする
			bIkkatu = FALSE;
			//if ((m_KmPrIkat == 1) && (rsData->m_FgFunc != ID_FGFUNC_IKKATUMANUAL)) {
			if ((m_KmPrIkat == 1) && 
				(rsData->m_FgFunc != ID_FGFUNC_IKKATUMANUAL) && 
				(rsData->m_FgFunc != ID_FGFUNC_IKKATUMANUAL_ZERO) && 
				(rsData->m_FgFunc != ID_FGFUNC_KAMOKU)) {
				// 対象フィールドから値を取り出し、金額の大小判断を行う（未満で判断）
// 158096 del -->
//// 158063 del -->
//				//strVal = rsData->GetFieldValueString(SORT_IKKATU_FIELD);
//// 158063 del <--
//// 158063 add -->
//				if(amsw != 0 && (m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081))	{
//					// 取引先毎の総額をマップから取得する
//					CString	cs1=_T(""),cs2=_T("");
//					cs1 = rsData->m_AdName1;
//					cs2 = rsData->m_AdName2;
//					cs1.TrimRight();
//					cs2.TrimRight();
//					if(amsw == 1)	strVal = uc.GetAmount1(rsData->m_AdOrder,cs1 + cs2);
//					else			strVal = uc.GetAmount2(rsData->m_AdOrder,cs1 + cs2,rsData->m_KnOrder);
//				}
//				else	{
//					strVal = rsData->GetFieldValueString(SORT_IKKATU_FIELD);
//				}
//// 158063 add <--
//				//CString ikkatu_val = pCdlgSort->GetIkkatuVal(rsData->GetSortIkkatuOrder());
//				CString ikkatu_val = Ks_GetIkkatuVal(rsData->m_KnSeq);
//
//				if (Ks_CmnKingakuCmpFunc(strVal, ikkatu_val) < 0) {
//					// 一括集計対象
//					bIkkatu = TRUE;
//					nIkkatuLine = nIkkatuLine + 1;
//				}
// 158096 del <--
// 158096 add -->
				CString	sougaku=_T("");
				strVal = rsData->GetFieldValueString(SORT_IKKATU_FIELD);
				if(amsw != 0 && (m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_081))	{
					// 取引先毎の総額をマップから取得する
					CString	cs1=_T(""),cs2=_T("");
					cs1 = rsData->m_AdName1;
					cs2 = rsData->m_AdName2;
					cs1.TrimRight();
					cs2.TrimRight();
					if(amsw == 1)	sougaku = uc.GetAmount1(rsData->m_AdOrder,cs1 + cs2);
					else			sougaku = uc.GetAmount2(rsData->m_AdOrder,cs1 + cs2,rsData->m_KnOrder);
				}
				else	{
					sougaku = strVal;
				}
				CString ikkatu_val = Ks_GetIkkatuVal(rsData->m_KnSeq);
				if(Ks_CmnKingakuCmpFunc(sougaku,ikkatu_val,strVal) < 0)	{
					// 一括集計対象
					bIkkatu = TRUE;
					nIkkatuLine = nIkkatuLine + 1;
				}
// 158096 add <--
			}

			if (bIkkatu == TRUE) {
// midori 190505 add -->
				// 科目(見出し)行の挿入(【小計】を挿入する かつ、０円(空欄)除外の場合は金額の発生がある)
				if(m_KmPrSkei == 1 && Ks_ZeroNullCheck(rsData,m_nFormSeq,1) == 0)	{
					if(bKmkTitle1 == TRUE && bKmkTitle2 == TRUE)	{
						// 科目(見出し)データ：頁＋行の連番を設定
						Ks_CmnGetNextPageRow(&nPage,&nRow);
						// 科目行追加
						Ks_CmnTblInsertSpecialLineKm(nPage,nRow,ID_FGFUNC_KAMOKU,rsData->m_KnSeq, rsData->m_KnOrder,rsData->m_KnName,rsData->m_KnKana);
						// 科目(見出し)行の挿入をOFFにする
						bKmkTitle2 = FALSE;
					}
				}
// midori 190505 add <--
				// 一括対象データ：頁＋行はクリア，更にグループ番号を設定
				rsData->Edit();
				rsData->m_NumPage = 0;
				rsData->m_NumRow = 0;
				rsData->m_FgShow = ID_FGSHOW_IKKATU;
				rsData->m_NumGroup = nNumGroup;
				rsData->Update();
			}
			else {
// midori 190505 del -->
				//// 通常データ：頁＋行の連番を設定
				//Ks_CmnGetNextPageRow(&nPage, &nRow);

				//rsData->Edit();
				//rsData->m_NumPage = nPage;
				//rsData->m_NumRow = nRow;
				//rsData->Update();

				//nSyokeiLine = nSyokeiLine + 1;			// 小計対象行数を加算
				//nKaiPageLine = nKaiPageLine + 1;		// 改頁対象行数を加算
// midori 190505 del <--
// midori 190505 add -->
				// 科目(見出し)行の挿入
				if(bKmkTitle1 == TRUE && bKmkTitle2 == TRUE)	{
					// 科目(見出し)データ：頁＋行の連番を設定
					Ks_CmnGetNextPageRow(&nPage,&nRow);
					// 科目行追加
					Ks_CmnTblInsertSpecialLineKm(nPage,nRow,ID_FGFUNC_KAMOKU,rsData->m_KnSeq, rsData->m_KnOrder,rsData->m_KnName,rsData->m_KnKana);
					// 科目(見出し)行の挿入をOFFにする
					bKmkTitle2 = FALSE;
				}
				// 通常データ：頁＋行の連番を設定
				Ks_CmnGetNextPageRow(&nPage, &nRow);

				rsData->Edit();
				rsData->m_NumPage = nPage;
				rsData->m_NumRow = nRow;
				rsData->Update();

				nSyokeiLine = nSyokeiLine + 1;			// 小計対象行数を加算
				nKaiPageLine = nKaiPageLine + 1;		// 改頁対象行数を加算
// midori 190505 add <--
			}
		}

		// 次のレコードへ
		rsData->MoveNext();

		// 第１ソート項目の値を"new"へセット
		if (!rsData->IsEOF()) {
			strNewItem1 = rsData->GetFieldValueString(uItemSort.strFieldSykei);
			strNewItem2 = rsData->GetFieldValueString(uItemSort.strFieldSykei2);

			// "old"と"new"を比較：第１ソート項目が変わった？
			bChangeItem = FALSE;
			if ((strOldItem1 != strNewItem1) || (strOldItem2 != strNewItem2)) {
				// 変化した
				bChangeItem = TRUE;
// midori 190505 add -->
				bKmkTitle2 = TRUE;	// 科目(見出し)行の挿入をONにする
// midori 190505 add <--
			}
		}
		else {
			// EOFの場合、１ソート項目が変わった事にしておく
			bChangeItem = TRUE;
		}

		// 第１ソート項目が変わった？orレコード終了？
		if ((bChangeItem == TRUE) || (rsData->IsEOF())) {
			// 自動一括集計金額行の挿入：ダイアログの設定あり
			if ((m_KmPrIkat == 1) && (nIkkatuLine > 0)) {
				// 小計あり：１ソート項目が変化したタイミングで挿入
				// 小計なし：最後レコードに挿入
				if (((m_KmPrSkei == 1) && (bChangeItem == TRUE)) || (rsData->IsEOF())) {
					// 一括対象レコード作成
					if ((m_KmPrSkei == 1) && (uItemSort.bytFgItem == ID_FGITEM_KAMOKU)) {
						// 小計ありand科目指定の場合、一括レコードは科目情報付きとする
						Ks_CmnTblIkkatuAutoUpdateProc(0, nNumGroup, &nPage, &nRow, uOldKamoku);
					}
					else {
						// ※小計単位ではなく、最終に出力される一括金額行の場合
						// 　直前のデータが科目未入力でないかつ、改頁を行う設定の場合は
						// 　改頁を行って、最終の科目と別の頁に出力する
						// 　（科目未入力のデータの中に、KnSeq IS NULL のデータがあり、KnSeqのみでは判定できない）
						//if((m_KmPrKaip == 1) && (uOldKamoku.intKnSeq != 0))	{
// midori M-17082102 del -->
						//if((m_KmPrKaip == 1) && ((uOldKamoku.intKnSeq != 0) && (uOldKamoku.intKnOrder != 99999)))	{
// midori M-17082102 del <--
// midori M-17082102 add -->
						// 　全てのデータが一括金額の対象になった場合、この時点でデータが無くなっているため(nRow=0)
						// 　その場合は、改頁を行わずに「その他○件」を作成する
						if((m_KmPrKaip == 1) && ((uOldKamoku.intKnSeq != 0) && (uOldKamoku.intKnOrder != 99999)) && (nRow > 0))	{
// midori M-17082102 add <--
							for(i = nRow + 1; i <= intRowMaxData; i++)	{
								// 空行の頁と行番号を取得
								Ks_CmnGetNextPageRow(&nPage, &nRow);
								// 対象頁を空行で埋める
								Ks_CmnTblInsertNullLine(nPage, nRow, 1);
							}
							// 改頁対象行数をクリア
							nKaiPageLine = 0;
						}
						// 一括レコードに科目情報は不要（但し、最後にソートされるように KnOrder に "99999" をセットする）
						uNullKamoku.intKnOrder = 99999;
// midori 152137 add -->
						uNullKamoku.strKnKana = _T("ﾝﾝﾝﾝﾝﾝ");
// midori 152137 add <--
						Ks_CmnTblIkkatuAutoUpdateProc(0, nNumGroup, &nPage, &nRow, uNullKamoku);
					}
					// 次のデータに向けてクリア
					nNumGroup = nNumGroup + 1;			// 一括対象グループ番号を加算
					nIkkatuLine = 0;					// 一括対象行数をクリア

					nSyokeiLine = nSyokeiLine + 1;			// 小計対象行数を加算
					nKaiPageLine = nKaiPageLine + 1;		// 改頁対象行数を加算
				}
			}

			// 小計行の挿入：ダイアログの設定あり＋小計対象項目である
			if ((m_KmPrSkei == 1) && (uItemSort.bytFgSykei == 1)) {
// 改良No.21-0086,21-0529 cor -->
				//// 特例処理：⑪借入金（2016/01/06②受取手形の内訳書も追加）且つ第１ソートが科目の場合、小計名称を科目名付きとする	
				//if (((m_nFormSeq == ID_FORMNO_111) || (m_nFormSeq == ID_FORMNO_021)) && (uItemSort.bytFgItem == ID_FGITEM_KAMOKU)) {
// ------------------------------
				// 特例処理：②受取手形、⑪借入金、登録番号追加版の⑧支払手形 且つ 第１ソートが科目の場合、小計名称を科目名付きとする	
				if (((m_nFormSeq == ID_FORMNO_021) || (m_nFormSeq == ID_FORMNO_081 && bG_InvNo == TRUE) || (m_nFormSeq == ID_FORMNO_111)) 
					&& (uItemSort.bytFgItem == ID_FGITEM_KAMOKU)) {
// 改良No.21-0086,21-0529 cor <--
					CdbUcInfSub mfcRecSub(m_pDb);
					if( mfcRecSub.RequeryFormSeq( m_nFormSeq ) == DB_ERR_OK ){
						if( !(mfcRecSub.IsEOF()) ){
							CCtrlSubGeneralVar cs( mfcRecSub );
							if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
// midori 190505 del -->
//								//if( !(cs.bOutKamokuRow) ){
// midori 190505 del <--
// midori 190505 add -->
								if(m_KmPrKtitle == 0)	{
// midori 190505 add <--
									strSykeiName = Ks_CmnGetSykeiNameWithKakko(m_OpSykeiName, m_OpLeft, m_OpRight, uOldKamoku.strKnName);
								}
							}
						}

						mfcRecSub.Fin();
					}
				}

				// データ２行以上 or ダイアログ：１行でも小計挿入オン
// midori 151396 cor -->
//				if ((nSyokeiLine >= 2) || (nSyokeiLine == 1 && m_KmPrSkei == 1)) {
// ---------------------
				if ((nSyokeiLine >= 2) || (nSyokeiLine == 1 && m_OpOutSyokei == 1)) {
// midori 151396 cor <--
					// 小計行の挿入：Sum値は後で集計する
					Ks_CmnGetNextPageRow(&nPage, &nRow);
					Ks_CmnTblInsertSpecialLine(nPage, nRow, ID_FGFUNC_SYOKEI, strSykeiName);
					// 空行の挿入：ダイアログで空行挿入する＋先頭行なら挿入しない
					if ((m_OpOutNullLine == 1) && (Ks_CmnGetNextFirstRow(nRow) == FALSE)) {
						// 小計行の後に空行を挿入
						Ks_CmnGetNextPageRow(&nPage, &nRow);
						Ks_CmnTblInsertNullLine(nPage, nRow, 1);
					}
				}
				// ダイアログ：１行でも小計挿入オフは、小計（空行）を挿入する
				else {
					// 小計（空行）行の挿入：Sum値は後で集計する
					Ks_CmnGetNextPageRow(&nPage, &nRow);
					// ページの頭に小計（空行）を挿入しない
					if( nRow != 1 ){
						Ks_CmnTblInsertSpecialLine(nPage, nRow, ID_FGFUNC_SYOKEINULL, "");
					}
					else{
						m_NullSyokeiRow = TRUE;
					}
				}

				// 小計対象行数をクリア
				nSyokeiLine = 0;
			}

			// 改頁の挿入：ダイアログの設定あり
			if ((m_KmPrKaip == 1) && (nKaiPageLine > 0)) {
				// 
				for (i = nRow + 1; i <= intRowMaxData; i++) {
					// 空行の頁と行番号を取得
					Ks_CmnGetNextPageRow(&nPage, &nRow);
					
					// 対象頁を空行で埋める
					Ks_CmnTblInsertNullLine(nPage, nRow, 1);
				}

				// 改頁対象行数をクリア
				nKaiPageLine = 0;
			}

			// "new"→"old"
			if (!rsData->IsEOF()) {
				strOldItem1 = strNewItem1;
				strOldItem2 = strNewItem2;
			}
		}
	}
	return(FUNCTION_OK);
}

//******************************************************************************
// 小計/中計の括弧付き名称を取得
//******************************************************************************
CString	CdbKamokuSitei::Ks_CmnGetSykeiNameWithKakko(CString strSykeiName, CString strLeft, CString strRight, CString strKamoku)
{
	CString		strTemp = strSykeiName;
	CString		strRet;

	// 特例の科目名付きの場合は、"小計（科目名）"とする
	if (strKamoku != "") {
// midori 191002 del -->
		//strTemp = strTemp + _T("（") + strKamoku + _T("）");
// midori 191002 del <--
// midori 191002 add -->
		strTemp = strTemp + _T("(") + strKamoku + _T(")");
// midori 191002 add <--
	}

	// 小計/中計名称の作成 ･･･ 指定カッコで括る
	if ((strLeft == "") || (strRight == "")) {
		strRet = strTemp;
	}
	else {
		strRet = strLeft + strTemp + strRight;
	}
	return strRet;
}

//******************************************************************************
// 一括金額取得
//******************************************************************************
CString CdbKamokuSitei::Ks_GetIkkatuVal(long pKnSeq)
{
	CString			val=_T("");
	CdbUcLstKamoku	mfcRec2(m_pDb);	// uc_lst_kamokuテーブルクラス

	// 共通の一括金額
	if(m_isAdvSet == FALSE)		return(m_OpIkkatuVal);

	// 個別の一括金額
	if(mfcRec2.RequeryKnSeq(m_nFormSeq,pKnSeq) == DB_ERR_OK)	{
		if(!(mfcRec2.IsEOF()))	{
			mfcRec2.MoveFirst();
			val = mfcRec2.m_IkkatuVal;
		}
		// レコード閉じる
		mfcRec2.Fin();
	}

	if(val.IsEmpty())			return(m_OpIkkatuVal);	// 取得に失敗した場合は共通の一括金額

	return(val);
}

// 158096 del -->
////******************************************************************************
//// 金額を比較する
////******************************************************************************
//int CdbKamokuSitei::Ks_CmnKingakuCmpFunc(CString strKingaku1, CString strKingaku2)
// 158096 del <--
// 158096 add -->
//******************************************************************************
// 金額を比較する
//******************************************************************************
int CdbKamokuSitei::Ks_CmnKingakuCmpFunc( CString strKingaku1, CString strKingaku2, CString strKingaku3 )
// 158096 add <--
{
	char		prmKingaku1[32];
	char		prmKingaku2[32];
	__int64		i64Kingaku1;
	__int64		i64Kingaku2;
	int			intRet;

	// Char型をクリア
	ZeroMemory(prmKingaku1, sizeof(prmKingaku1));
	ZeroMemory(prmKingaku2, sizeof(prmKingaku2));

	// CString型 → Char型
	lstrcpy(prmKingaku1, strKingaku1);
	lstrcpy(prmKingaku2, strKingaku2);

	// Char型 → __int64型
	i64Kingaku1 = _strtoi64(prmKingaku1, NULL, 10);
	i64Kingaku2 = _strtoi64(prmKingaku2, NULL, 10);

	// 一括金額に０円を含めるかチェック
	if(m_IkkatuOutZero){	// 一括金額に０円を含めない
		// 比較対象金額１が０円の場合、一括対象としない
// 158096 del -->
		//if( i64Kingaku1 == 0 ){
// 158096 del <--
// 158096 add -->
		char		prmKingaku3[32]={0};
		__int64		i64Kingaku3;
		lstrcpy(prmKingaku3,strKingaku3);
		i64Kingaku3 = _strtoi64(prmKingaku3,NULL,10);
		if(i64Kingaku3 == 0)	{
// 158096 add <--
			return 0;
		}
	}

	// __int64型で大小を比較
	if (i64Kingaku1 == i64Kingaku2) {
		intRet = 0;
	}
	else if (i64Kingaku1 > i64Kingaku2) {
		intRet = 1;
	}
	else {
		intRet = -1;
	}
	return intRet;
}

//******************************************************************************
// 頁＋行から次の行を取得する
//******************************************************************************
int CdbKamokuSitei::Ks_CmnGetNextPageRow(int* pPage, int* pRow)
{
	// 次の行は改行が必要？
	if (*pRow + 1 > intRowMaxData) {
		*pPage = *pPage + 1;
		*pRow = 1;
	}
	else {
		*pRow = *pRow + 1;
	}
	
	return	FUNCTION_OK;
}

//******************************************************************************
// 自動一括集計金額行の追加/更新処理
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblIkkatuAutoUpdateProc(int nType, int nGroup, int* nPage, int* nRow, SORTKAMOKU_INFO uKamoku)
{
	CString			strName;			// 名称
	CALCKEI_INFO	uCalcKei;			// 集計金額
	int				nCount;				// 集計件数
	int				nTempPage = 0;
	int				nTempRow = 0;

	// 一時テーブルクラスを"New"する
	CdbPrtWork*		rsData;
	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 金額と件数を集計
	Ks_TblGetIkkatuSumValue(rsData, ID_FGSHOW_IKKATU, nGroup, &uCalcKei);
	nCount = rsData->GetCountVal(ID_FGSHOW_IKKATU, nGroup);

	// 一括金額レコードの名称
	strName.Format("%s %d%s", NAME_IKKATU_1, nCount, NAME_IKKATU_2);

	// 自動一括集計金額行の追加：１件以上あれば追加
	if (nCount > 0) {
		// 追加行の頁と行を取得
		if (nType == 0) {
			Ks_CmnGetNextPageRow(nPage, nRow);
		}

		// 新規レコード追加
		rsData->CreateNewRecord();					// レコード初期化済み

		// 特殊行データへ更新
		rsData->Edit();
		rsData->m_NumPage = *nPage;					// 頁番号
		rsData->m_NumRow = *nRow;					// 行番号
		rsData->m_FgFunc = ID_FGFUNC_IKKATUAUTO;	// 特殊行フラグ
		rsData->m_FgShow = ID_FGSHOW_OFF;			// 表示フラグ
		rsData->m_NumGroup = nGroup;				// グループ番号

		Ks_TblEditIkkatuLine(0, rsData, strName, uCalcKei, uKamoku);		// 帳表個別データ設定

		rsData->Update();
	}
	rsData->Fin();
	delete rsData;

	return	FUNCTION_OK;
}

//******************************************************************************
// virtual関数：各帳表で実装する処理
//		ソート（一括集計）で使用する集計処理
//******************************************************************************
void CdbKamokuSitei::Ks_TblGetIkkatuSumValue(CdbPrtWork* rsData, int nFgShow, int nGroup, CALCKEI_INFO* uCalcKei)
{
	// 通常は"Val"項目のみ集計する。
	// ただし④-2，⑪-1は複数フィールド集計するため、"virtual"にて各画面Viewで集計を行う。
	// なお、一括集計の集計フィールドと小計などの集計フィールドは異なるので注意が必要。
	if(m_nFormSeq != ID_FORMNO_111)	{
		uCalcKei->intMaxCount = 1;
		uCalcKei->strField[0] = "Val";
		uCalcKei->strKei[0] = "";
		rsData->GetSumValSub(nFgShow, nGroup, uCalcKei->strField[0], &uCalcKei->strKei[0]);		// "0:Val"項目の集計
	}
	else	{
		uCalcKei->intMaxCount = 21;
		uCalcKei->strField[0] = "Val";
		uCalcKei->strKei[0] = "";
		uCalcKei->strField[1] = "Risoku";
		uCalcKei->strKei[1] = "";
		rsData->GetSumValSub(nFgShow, nGroup, uCalcKei->strField[0], &uCalcKei->strKei[0]);		// "0:Val"項目の集計
		rsData->GetSumValSub(nFgShow, nGroup, uCalcKei->strField[1], &uCalcKei->strKei[1]);		// "1:Risoku"項目の集計
	}
	return;
}

//******************************************************************************
// 一括集計金額行のデータ設定
//******************************************************************************
void CdbKamokuSitei::Ks_TblEditIkkatuLine( int nType, CdbPrtWork* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	CdbPrtWork*	rs = rsData;

	if( m_nFormSeq == ID_FORMNO_021 || 
		m_nFormSeq == ID_FORMNO_031 || 
		m_nFormSeq == ID_FORMNO_041 || 
// midori 190505 add -->
		m_nFormSeq == ID_FORMNO_081 || 
// midori 190505 add <--
		m_nFormSeq == ID_FORMNO_091 || 
		m_nFormSeq == ID_FORMNO_101 || 
		(m_nFormSeq >= ID_FORMNO_171 && m_nFormSeq <= ID_FORMNO_1720) )	{
		rs->m_AdName1	= strName;				// 名称：その他?件
	}
	else if(m_nFormSeq == ID_FORMNO_051)	{
		rs->m_HdName	= strName;				// 名称：その他?件
	}
	else if(m_nFormSeq == ID_FORMNO_111)	{
// midori 190505 del -->
		//rs->m_AdAdd2	= strName;				// 名称：その他?件
// midori 190505 del <--
// midori 190505 add -->
		if(bG_Kanso == TRUE)	{
			rs->m_AdName1	= strName;			// 名称：その他?件
		}
		else	{
			rs->m_AdAdd2	= strName;			// 名称：その他?件
		}
// midori 190505 add <--
		rs->m_Risoku	= uCalcKei.strKei[1];	// 支払利息
	}
	else if(m_nFormSeq == ID_FORMNO_161 || m_nFormSeq == ID_FORMNO_162)	{
		rs->m_Naiyou	= strName;				// 名称：その他?件
	}
	else if(m_nFormSeq >= ID_FORMNO_181)	{
		rs->m_Item		= strName;				// 名称：その他?件
	}
	rs->m_Val		= uCalcKei.strKei[0];		// 金額："0:Val"固定
	rs->m_KnSeq		= uKamoku.intKnSeq;			// 科目情報
	rs->m_KnOrder	= uKamoku.intKnOrder;		// 科目情報
	rs->m_KnName	= uKamoku.strKnName;		// 科目情報
// midori 152137 add -->
	rs->m_KnKana	= uKamoku.strKnKana;		// 科目情報
// midori 152137 add <--
}

//******************************************************************************
// 特殊行挿入･･･１行のみ可能
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblInsertSpecialLine(int nPage, int nRow, int nType, CString strName)
{
	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0) {
		return FUNCTION_NG;
	}

	// 一時テーブルクラスを"New"する
	CdbPrtWork*		rsData;
	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 新規レコード追加
	rsData->CreateNewRecord();				// レコード初期化済み

	// 特殊行データ編集（共通データ）
	rsData->Edit();
	rsData->m_NumPage = nPage;				// ページ番号
	rsData->m_NumRow = nRow;				// 行番号
	rsData->m_FgFunc = nType;				// 特殊行フラグ
	rsData->m_FgShow = ID_FGSHOW_OFF;		// 表示フラグ
	rsData->m_KeiStr = strName;

	// レコード更新
	rsData->Update();
	rsData->Fin();

	delete rsData;
	return FUNCTION_OK;
}

// midori 190505 add -->
//******************************************************************************
// 特殊行挿入（科目行）･･･１行のみ可能
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblInsertSpecialLineKm(int nPage, int nRow, int nType, int nKnSeq, int nKnOrder, CString strKnName, CString strKnKana)
{
// midori 156189,156190,156191 add -->
	CUcFunctionCommon clsfunc;
	CString strKnName1=_T("");
	CString strKnName2=_T("");
// midori 156189,156190,156191 add <--

	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0) {
		return FUNCTION_NG;
	}

	// 一時テーブルクラスを"New"する
	CdbPrtWork*		rsData;
	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 新規レコード追加
	rsData->CreateNewRecord();				// レコード初期化済み

	// 特殊行データ編集（共通データ）
	rsData->Edit();
	rsData->m_NumPage	= nPage;			// ページ番号
	rsData->m_NumRow	= nRow;				// 行番号
	rsData->m_FgFunc	= nType;			// 特殊行フラグ
	//rsData->m_FgShow = ID_FGSHOW_OFF;		// 表示フラグ
	//rsData->m_KeiStr = strName;
	rsData->m_KnSeq		= nKnSeq;		// 科目情報
	rsData->m_KnOrder	= nKnOrder;		// 科目情報
	rsData->m_KnName	= strKnName;	// 科目情報
	rsData->m_KnKana	= strKnKana;	// 科目情報（カナ)
// midori 156189,156190,156191 add -->
	// 全角１０文字を超えていれば改行マークを挿入して取り込む
	if(strKnName.GetLength() > 20)	{
		strKnName2 = clsfunc.GetSpritString(strKnName,&strKnName1,20);
		strKnName = strKnName1 + _T("\r\n") + strKnName2;
	}
	rsData->m_KeiStr	= strKnName;	// 科目情報
// midori 156189,156190,156191 add <--

	// レコード更新
	rsData->Update();
	rsData->Fin();

	delete rsData;
	return FUNCTION_OK;
}
// midori 190505 add <--

//******************************************************************************
// 頁＋行から次の行が次頁の先頭行か判断する
//******************************************************************************
BOOL CdbKamokuSitei::Ks_CmnGetNextFirstRow(int intRow)
{
	// 次の行は先頭行？
	if (intRow + 1 > intRowMaxData) {
		return	TRUE;
	}
	
	return	FALSE;
}

//******************************************************************************
// 空行挿入･･･複数行可能
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblInsertNullLine(int nPage, int nRow, int nLine)
{
	int					i = 0;
	int					nCountPage = nPage;
	int					nCountRow = nRow;

	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0 || nLine <= 0) {
		return FUNCTION_NG;
	}

	// 一時テーブルクラスを"New"する
	CdbPrtWork*		rsData;
	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// ｎ件分のレコード追加
	for (i = 0; i < nLine; i++){
		// 新規レコード追加
		rsData->CreateNewRecord();				// レコード初期化済み

		// 空行データとする
		rsData->Edit();
		rsData->m_NumPage = nCountPage;			// ページ番号
		rsData->m_NumRow = nCountRow;			// 行番号
		rsData->m_FgFunc = ID_FGFUNC_NULL;		// 特殊行フラグ
		rsData->m_FgShow = ID_FGSHOW_OFF;		// 表示フラグ

		// レコード更新
		rsData->Update();

		// 次の頁と行を取得
		Ks_CmnGetNextPageRow(&nCountPage, &nCountRow);
	}
	rsData->Fin();

	delete rsData;
	return FUNCTION_OK;
}

//******************************************************************************
// テーブル全体に対して頁と行の再設定を行う（メイン）（ソート後）
// 小計を自動挿入する際、のエラーを回避する為にソート後はこちらを呼ぶ
//		例）ソート，出力形式，計設定など
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblUpdatePageRowAllResetMainAfterSort()
{
	CString				strSQL=_T("");

	// 頁と行の調整処理
	CdbPrtWork*	rsData;
	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 頁計の削除
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_PAGEKEI);
	ExecuteSQLWork(strSQL);

	// 累計の削除
	strSQL.Format(_T("DELETE FROM %s WHERE FgFunc=%d"),m_sTblName,ID_FGFUNC_RUIKEI);
	ExecuteSQLWork(strSQL);

	rsData->Fin();
	delete rsData;

	Ks_CmnTblUpdatePageNumRowNumReset(1, 1);		// 先頭頁からページ再設定
	Ks_CmnTblUpdateEndPageSetting();				// 最終頁を調整する
	Ks_CmnTblInsertPageKeiRuiKei();					// 頁計/累計挿入（最終ページの削除or空行追加）

	// 小計/中計/頁計/累計の再計算
	Ks_CmnTblCalcKei();
	// 小計の自動挿入のときのみ
	if( m_NullSyokeiRow != FALSE ){
		// 小計のみ上書き
		Ks_CmnTblCalcKeiAfterSort();
		m_NullSyokeiRow = FALSE;
	}

	return FUNCTION_OK;
}

//******************************************************************************
// パラメータの頁番号(inPage)、行番号(inRow)以降の
// 頁番号と行番号を振り直す。
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblUpdatePageNumRowNumReset(int inPage, int inRow)
{
	CdbPrtWork*			rsData;
	int					intNumPage;
	int					intNumRow;
	int					nBaseRow;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = Ks_CmnTblCountMaxRow();

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	intNumPage = inPage;
	intNumRow = inRow;

	// 頁番、行番振り直し
	while (!rsData->IsEOF()) {

		// パラメータの頁番号、行番号以上のレコードから番号を振り直す
		if ((rsData->m_NumPage > inPage) || ((rsData->m_NumPage == inPage) && (rsData->m_NumRow >= inRow))) {
			rsData->Edit();
			rsData->m_NumPage = intNumPage;		// 頁番号をセット
			rsData->m_NumRow = intNumRow;		// 行番号をセット
			rsData->Update();
		}

		// 頁番号と行番号をインクリメント
		if (intNumRow >= nBaseRow) {
			intNumPage++;
			intNumRow = 1;
		}
		else {
			intNumRow++;
		}

		// 次のレコードへ
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return retVal;
}

//******************************************************************************
// 行数をカウントアップする際の、ベースになる１頁の行数を求める
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblCountMaxRow()
{
	int		nBaseRow;

	// デフォルトに「計あり」の行数をセットしておく
	nBaseRow = intRowMaxData;

	// ベースになる行数を決める
	// 「計なし」帳表で、計設定が「オフ」または「累計のみ」の場合は、「計なし」の場合の行数をセットする
	if ((intRowKei == 0) && ((intOutKei == ID_OUTKEI_OFF) || (intOutKei == ID_OUTKEI_RUIKEI))) {
		nBaseRow = intRowMax;
	}

	return nBaseRow;
}

//******************************************************************************
// 最終頁を調整する。最終頁の全行が空行なら、頁まるごと削除する。
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblUpdateEndPageSetting(void)
{
	CdbPrtWork*	rsData;
	int			nBaseMaxPage, nMaxPage;	// 最大頁数
	int			nRowNum;				// 空行且つ表示可能な行の行数
	int			nNullRowNum;			// 空行の行数
	int			retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 最大頁数を取得
	nBaseMaxPage = rsData->GetNumPage();
	nMaxPage = nBaseMaxPage;
	
	// 最終頁が全て空行なら頁まるごと削除する。1頁目は対象外とする。
	while (nMaxPage >= 2) {
		
		// 空行(FgFunc=0)、且つ表示行(FgShow=0)の行数を取得する。
		nRowNum = rsData->GetNumRowFgFuncFgShow(nMaxPage, ID_FGFUNC_NULL, ID_FGSHOW_OFF);

		// 表示行(FgShow=0)の行数を取得する。
		nNullRowNum = rsData->GetNumRowFgShow(nMaxPage, ID_FGSHOW_OFF);
		
		// 最終頁の削除有無をチェック
		if (nRowNum == nNullRowNum) {
			nMaxPage--;
		} else {
			break;	// ループから抜ける
		}

	}

	// 空行頁を削除する。
	if (nMaxPage < nBaseMaxPage) {
		rsData->DeletePage(nMaxPage + 1);
	}

	rsData->Fin();
	delete rsData;

	return retVal;
}

//******************************************************************************
// 頁計、累計行を挿入する。
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblInsertPageKeiRuiKei(void)
{
	CdbPrtWork*		rs;						// 全レコード取得用
	int				nSetNumPage;			// 頁番号保存変数
	int				nSetNumRow;				// 行番号保存変数
	int				retVal = FUNCTION_NG;
	CString			strNameKei;

	retVal = FUNCTION_OK;

	// 計の名称セット
	if(m_NameKei)	strNameKei = NAME_GOUKEI;
	else			strNameKei = NAME_KEI;

	rs = new CdbPrtWork(m_pDb,m_nFormSeq);

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rs->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	// 1頁目の1行目からチェックする。
	nSetNumPage = 1;
	nSetNumRow = 1;

	// Step1：先頭頁の先頭行から順に参照して、頁計/累計行を挿入する
	switch (intOutKei) {
	case ID_OUTKEI_OFF:		// なし
	case ID_OUTKEI_PAGEKEI:	// 頁計
	case ID_OUTKEI_RUIKEI:	// 累計
	case ID_OUTKEI_BOTH:	// 累計と累計

		// 計行、頁計行、累計行を挿入していく。
		while (!rs->IsEOF()) {
			// 頁計を挿入する条件式
			if (nSetNumRow > intRowMaxData) {
				if (intRowKei == 1) {
					// 計あり帳表
					switch (intOutKei) {
					case ID_OUTKEI_OFF:		// なし
					case ID_OUTKEI_RUIKEI:	// 累計のみを挿入する
						// 頁計なしの場合、"計"or"合計"文字をセット
						//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
						Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, strNameKei);
						break;

					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
						// 頁計をセット
						Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
						break;
					}
				}
				else {
					// 計なし帳表
					switch (intOutKei) {
					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
						// 頁計をセット
						Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
						break;
					}
				}
				nSetNumPage++;		// 頁番号をインクリメント
				nSetNumRow = 1;		// 行番号を先頭に戻す
			}
			else {
				rs->MoveNext();
				nSetNumRow++;		// 行番号をインクリメント
			}
		}
		break;
	}

	// Step2：最終頁の空行挿入＋頁計/累計作成
	switch (intOutKei) {
	case ID_OUTKEI_OFF:		// 計なし
		// 計あり帳表：最終行は合計行とする
		// 計なし帳表：最終行まで空行で埋める
		while (nSetNumRow <= intRowMax) {
			if ((nSetNumRow == intRowMax) && (intRowKei == 1)) {
				// 最終行に合計行を挿入
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
				Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, strNameKei);
			}
			else {
				// 空行挿入
				Ks_CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
		break;

	case ID_OUTKEI_PAGEKEI:	// 頁計
		// 計あり帳表，計なし帳表共に、最終行は合計行とする
		while (nSetNumRow <= intRowMax) {
			if (nSetNumRow == intRowMax) {
				// 最終行に合計行を挿入
				Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
			}
			else {
				// 空行挿入
				Ks_CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
		break;

	case ID_OUTKEI_RUIKEI:	// 累計
		// 計あり帳表：最終行は合計行とする
		// 計なし帳表：最終行は合計行とする。また最終行がデータ行なら次頁を空行で埋め最終行を累計とする
		if (intRowKei == 1) {
			// 計あり帳表
			while (nSetNumRow <= intRowMax) {
				if (nSetNumRow == intRowMax) {
					// 最終行に合計行を挿入
					//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
					Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
				}
				else {
					// 空行挿入
					Ks_CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
				}
				nSetNumRow++;		// 行番号をインクリメント
			}
		}
		else {
			// 計なし帳表：最終データが空行なら削除する（最終行の条件を付けるべきかも）
			if (rs->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
				rs->MovePrev();
				if (!rs->IsEOF()) {
					if ((rs->m_FgFunc == 0) && (rs->m_FgShow == 0)) {
						rs->Delete();
						nSetNumRow--;
					}
				}
			}
			// すでに最終行がデータで埋まっている場合は次頁に累計行を入れる
			if (nSetNumRow > intRowMax) {
				nSetNumPage++;			// 頁番号をインクリメント
				nSetNumRow = 1;			// 行番号をクリア
			}
			// あとは空行で埋め、最終行に合計行を挿入
			if (nSetNumRow <= intRowMax) {
				while (nSetNumRow <= intRowMax) {
					if (nSetNumRow == intRowMax) {
						// 最終行に合計行を挿入
						//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
						Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
					}
					else {
						// 空行挿入
						Ks_CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
					}
					nSetNumRow++;		// 行番号をインクリメント
				}
			}
		}
		break;

	case ID_OUTKEI_BOTH:	// 頁計と累計
		// 計あり帳表，計なし帳表共に、データの最後を頁計と累計とする。
		//	１頁しかない場合は、累計行のみ追加する
		//	合計行を１行しか挿入できない場合、該当頁は頁計として、次頁に頁計と累計行とする
		if (rs->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
			rs->MovePrev();
			if (!rs->IsEOF()) {
				if ((rs->m_FgFunc == 0) && (rs->m_FgShow == 0)) {
					rs->Delete();
					nSetNumRow--;
				}
			}
		}
		// 既に最終行－１がデータで埋まっている場合
		if (nSetNumRow >= intRowMax) {
			if (nSetNumPage == 1) {
				// 累計を挿入して終了
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
				Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
				nSetNumRow++;			// 行番号をインクリメント
			}
			else {
				// 頁計を挿入して次頁に頁計と累計を出す
				Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
				nSetNumPage++;			// 頁番号をインクリメント
				nSetNumRow = 1;			// 行番号をクリア
			}
		}
		// あとは空行で埋め、最終行－１と最終行に合計行を挿入
		while (nSetNumRow <= intRowMax) {
			if (nSetNumRow == intRowMax) {
				// 最終行に累計行を挿入
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
				Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
			}
			else if ((nSetNumRow == intRowMax - 1) && (nSetNumPage >= 2)) {
				// 最終行－１に頁計行を挿入
				Ks_CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
			}
			else {
				// 空行挿入
				Ks_CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
	}
	delete rs;

	// カレント頁の調整
	intMaxPage = nSetNumPage;

//	if (intCurPage > intMaxPage) {
//		intCurPage = nSetNumPage;
//	}

	return retVal;	
}

//******************************************************************************
// 小計、中計、頁計、累計の合計を求める
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblCalcKei()
{
	CALCKEI_INFO	uCalcInfo;
	CString		strSyoKei[CALCKEI_MAXCOUNT];		// 小計計算用
	CString		strChuKei[CALCKEI_MAXCOUNT];		// 中計計算用
	CString		strPageKei[CALCKEI_MAXCOUNT];		// 頁計計算用
	CString		strRuiKei[CALCKEI_MAXCOUNT];		// 累計計算用
	BOOL		bSyoKei[CALCKEI_MAXCOUNT];			// 小計用０表示フラグ
	BOOL		bChuKei[CALCKEI_MAXCOUNT];			// 中計用０表示フラグ
	BOOL		bPageKei[CALCKEI_MAXCOUNT];			// 頁計用０表示フラグ
	BOOL		bRuiKei[CALCKEI_MAXCOUNT];			// 累計用０表示フラグ
	CString		strTemp = "";
	int			intMax;
	int			retVal = FUNCTION_OK;
	int			i = 0;

	uCalcInfo.intMaxCount = intMaxCount;
	uCalcInfo.strField[0] = strField[0];
	uCalcInfo.strField[1] = strField[1];
// midori 190505 add -->
	uCalcInfo.strField[2] = strField[2];
// midori 190505 add <--

	intMax = uCalcInfo.intMaxCount;

	// ０円or空欄を出力しないサイン取得
	CdbUcInfSub mfcSub(m_pDb);
	mfcSub.RequeryFormSeq(m_nFormSeq);
	CCtrlSubGeneralVar cs(mfcSub);
	int OutZeroNull = 0;

	if(cs.bPrintZeroData)	OutZeroNull |= BIT_D0;
	if(cs.bPrintNullData)	OutZeroNull |= BIT_D1;

	mfcSub.Fin();

	// 帳表毎のテーブルクラスを"New"する
	CdbPrtWork*		rs;
	rs = new CdbPrtWork(m_pDb,m_nFormSeq);		// 全レコード取得用
	CdbPrtWork*		rsData;
	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);	// 集計値更新用：Field名称可変で集計値を更新するため

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rs->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	BOOL isZeroNullData = 0;
	CString strTemp2[CALCKEI_MAXCOUNT] = {0};
	// 初期化
	for (int i = 0; i < CALCKEI_MAXCOUNT; i++) {
		uCalcInfo.strKei[i] = "";

		strSyoKei[i].Format("%d", 0);
		strChuKei[i].Format("%d", 0);
		strPageKei[i].Format("%d", 0);
		strRuiKei[i].Format("%d", 0);

		bSyoKei[i]	= FALSE;
		bChuKei[i]	= FALSE;
		bPageKei[i] = FALSE;
		bRuiKei[i]	= FALSE;
	}

	// 全レコードに対し１行目から計算していく
	while (!rs->IsEOF()) {
		switch (rs->m_FgFunc) {
		case ID_FGFUNC_SYOKEI:		// 小計値をセット
			// 金額欄へ登録＋小計値を初期化
			for (int i = 0; i < intMax; i++) {
				uCalcInfo.strKei[i] = strSyoKei[i].TrimLeft();		// 集計値で更新
				strSyoKei[i].Format("%d", 0);						// クリア

				uCalcInfo.bExistNum[i] = bSyoKei[i];				// フラグ更新
				bSyoKei[i] = FALSE;									// クリア
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		case ID_FGFUNC_SYOKEINULL:
			// 小計値を初期化
			for (int i = 0; i < intMax; i++) {
				strSyoKei[i].Format("%d", 0);						// クリア
				bSyoKei[i] = FALSE;									// クリア
			}
			break;

		case ID_FGFUNC_CHUKEI:		// 中計値をセット
			// 金額欄へ登録＋中計値を初期化
			for (int i = 0; i < intMax; i++) {
				uCalcInfo.strKei[i] = strChuKei[i].TrimLeft();		// 集計値で更新
				strChuKei[i].Format("%d", 0);						// クリア

				uCalcInfo.bExistNum[i] = bChuKei[i];				// フラグ更新
				bChuKei[i] = FALSE;									// クリア
// midori M-17061302 add -->
				// 小計をクリアする
				strSyoKei[i].Format("%d", 0);						// クリア
				bSyoKei[i] = FALSE;									// クリア
// midori M-17061302 add <--
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		case ID_FGFUNC_CHUKEINULL:
			// 中計値を初期化
			for (int i = 0; i < intMax; i++) {
				strChuKei[i].Format("%d", 0);						// クリア
				bChuKei[i] = FALSE;									// クリア
			}
			break;

		case ID_FGFUNC_PAGEKEI:		// 頁計値をセット
			// 金額欄へ登録＋頁計値を初期化
			for (int i = 0; i < intMax; i++) {
				switch (intOutKei) {
				case ID_OUTKEI_OFF:			// なし
				case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
					uCalcInfo.strKei[i] = _T("");
					uCalcInfo.bExistNum[i] = FALSE;
					break;
				case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
				case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
					uCalcInfo.strKei[i] = strPageKei[i].TrimLeft();;	// 集計値で更新
					uCalcInfo.bExistNum[i] = bPageKei[i];				// フラグ更新
					break;
				}
				strPageKei[i].Format("%d", 0);						// クリア
				bPageKei[i] = FALSE;								// クリア
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		case ID_FGFUNC_RUIKEI:		// 累計値をセット
			// 金額欄へ登録＋クリアしない
			for (int i = 0; i < intMax; i++) {
				switch (intOutKei) {
				case ID_OUTKEI_OFF:			// なし
				case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
					uCalcInfo.strKei[i] = _T("");
					uCalcInfo.bExistNum[i] = FALSE;
					break;
				case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
				case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
					uCalcInfo.strKei[i] = strRuiKei[i].TrimLeft();;		// 集計値で更新
					uCalcInfo.bExistNum[i] = bRuiKei[i];				// フラグ更新
					break;
				}
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		default:
			isZeroNullData = 0;

			// 小計、中計、頁計、累計を足しこむ
			for (int i = 0; i < intMax; i++) {
				// フィールド名称から値を取得
				strTemp = rs->GetFieldValueString(uCalcInfo.strField[i]);
				strTemp = strTemp.TrimLeft();
				strTemp2[i] = strTemp;	// 下のforループでGetFieldValueStringをコールすると記述子インデックスが正しくありませんと例外を吐くので仕方なく

				// 加算
				Ks_CmnKingakuAddFunc(&strSyoKei[i], strTemp);
				Ks_CmnKingakuAddFunc(&strChuKei[i], strTemp);
				Ks_CmnKingakuAddFunc(&strPageKei[i], strTemp);
				Ks_CmnKingakuAddFunc(&strRuiKei[i], strTemp);

				if((OutZeroNull & BIT_D0) && (OutZeroNull & BIT_D1)){
					if(!(strTemp.IsEmpty() || (strTemp.Compare("0") == 0)))	isZeroNullData++;
				}
				else if(OutZeroNull & BIT_D0){
					if(strTemp.Compare("0") != 0)			isZeroNullData++;
				}
				else if(OutZeroNull & BIT_D1){
					if(!(strTemp.IsEmpty()))				isZeroNullData++;
				}
				else 		isZeroNullData++;
			}

		if(isZeroNullData == 0)		break;

		for (int i = 0; i < intMax; i++) {
			// 集計対象に1つでも入力があれば、計０円表示フラグを立てる
			if( bSyoKei[i] == FALSE )	bSyoKei[i]	= Ks_GetShowKeiZero( strTemp2[i]);		// 一回でもTRUEになったら書き換えない
			if( bChuKei[i] == FALSE )	bChuKei[i]	= Ks_GetShowKeiZero( strTemp2[i]);
			if( bPageKei[i] == FALSE )	bPageKei[i] = Ks_GetShowKeiZero( strTemp2[i]);
			if( bRuiKei[i] == FALSE )	bRuiKei[i]	= Ks_GetShowKeiZero( strTemp2[i]);
		}
		break;
		}

		rs->MoveNext();
	}

	rs->Fin();
	rsData->Fin();

	delete rs;
	delete rsData;

	return retVal;
}

//******************************************************************************
// 金額を加算/減算して合計値を求める
//******************************************************************************
void CdbKamokuSitei::Ks_CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag)
{
	char	prmTotalVal[32];		// 引数の合計金額格納用（CString型をchar型で格納）
	char	prmAddVal[32];			// 引数の加算金額格納用（CString型をchar型で格納）
	char	tmpTotalVal[32];		// 計算後の合計金額格納用（char型で格納）
	char	btAddVal[32];			// 計算後の合計金額格納用（倍長フィールド）
	char	buffAnser[32];			// 計算後の合計金額格を金額フォーマットで格納（char型で格納）

	//--------------------------------------------------------------
	// 初期化
	//--------------------------------------------------------------
	ZeroMemory(prmTotalVal, sizeof(prmTotalVal));
	ZeroMemory(prmAddVal, sizeof(prmAddVal));
	ZeroMemory(tmpTotalVal, sizeof(tmpTotalVal));
	ZeroMemory(btAddVal, sizeof(btAddVal));
	ZeroMemory(buffAnser, sizeof(buffAnser));
	
	//--------------------------------------------------------------
	// CString -> char* 変換
	//--------------------------------------------------------------
	lstrcpy(prmTotalVal, *inTotalVal);
	lstrcpy(prmAddVal, inAddVal);

	//--------------------------------------------------------------
	// 加算/減算
	//--------------------------------------------------------------
	__int64	i64;

	// char型を__int64に変換して加算/減算
	if (inAddFlag == 1) {
		i64 = _strtoi64(prmTotalVal, NULL, 10) + _strtoi64(prmAddVal, NULL, 10);
	}
	else {
		i64 = _strtoi64(prmTotalVal, NULL, 10) - _strtoi64(prmAddVal, NULL, 10);
	}

	// 上限、下限チェック
	if (i64 < DB_DECIMAL_MIN) {
		i64 = DB_DECIMAL_MIN;
	} else if (DB_DECIMAL_MAX < i64) {
		i64 = DB_DECIMAL_MAX;
	}

	// 合計値をchar型へ変換
	_i64toa_s(i64, tmpTotalVal, 32, 10);

	// CString型変数にchar型の合計金額値を格納
	*inTotalVal = tmpTotalVal;
}

//******************************************************************************
// ０円計の表示フラグをゲットする
//******************************************************************************
BOOL CdbKamokuSitei::Ks_GetShowKeiZero(CString strTemp)
{
	BOOL nRet = TRUE;
	if(strTemp.IsEmpty() != FALSE)	nRet = FALSE;

	return nRet;
}

//******************************************************************************
// 小計、中計、頁計、累計の合計を求める（ソート後）
//******************************************************************************
int CdbKamokuSitei::Ks_CmnTblCalcKeiAfterSort()
{
	// 帳表毎のテーブルクラスを"New"する
	CdbPrtWork*		rs;
	CALCKEI_INFO	uCalcInfo;
	ITEMSORT_INFO	uItemSort;					// 第１ソート項目のソート情報

	// 共通データ取得
	uCalcInfo.intMaxCount = intMaxCount;
	uCalcInfo.strField[0] = strField[0];
	uCalcInfo.strField[1] = strField[1];
// midori 190505 add -->
	uCalcInfo.strField[2] = strField[2];
// midori 190505 add <--

	// DBオブジェクト作成
	rs = new CdbPrtWork(m_pDb,m_nFormSeq);

	// uc_lst_item_sortを参照して必要情報を取得
	Ks_CmnUcLstItemSortGetData(m_Sort1, &uItemSort);

	// 集計後、値セット
	rs->CalcSykeiAfterSort(uCalcInfo, uItemSort.strFieldSykei, uItemSort.strFieldSykei2);

	delete rs;

	return FUNCTION_OK;
}

// midori 152137 del -->
////******************************************************************************
//// 科目指定する＋科目でソートを行わない
//// 一括金額行再集計
////	【引数】	CString		元データのテーブル名(uc_???_xxxxx)
////******************************************************************************
//int CdbKamokuSitei::Kn_CmnIkkatuSaisyukei( CString strMtbl )
//{
//	int				cnt=0;
//	__int64			vl[2];
//	__int64			total[2];
//	short			ind=0,max=0;
//	char			bf[128];
//	CString			strSQL=_T("");
//	CString			strSQL2=_T("");
//	CString			strFilter=_T("");
//	CString			strSort=_T("");
//	CString			strData[2];
//	CString			strName=_T("");
//	CdbPrtWork*		rsData;
//	CRecordset		rs(m_pDb);
//
//	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);
//
//	// 一括集計金額行(自動)で絞り込む
//	// ※DeleteKeiNullKamokuRow関数で値をID_FGFUNC_IKKATUAUTOから
//	// 　ID_FGFUNC_IKKATUAUTO_ZEROに書き換えているので注意
//	strFilter.Format(_T("FgFunc=%d"),ID_FGFUNC_IKKATUAUTO_ZERO);
//	if(rsData->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
//		rsData->Fin();
//		delete rsData;
//		return(FUNCTION_NG);
//	}
//	while(!rsData->IsEOF())	{
//		strSQL = _T("select a.val");
//		max=1;
//		if(m_nFormSeq == ID_FORMNO_111)	{
//			strSQL += _T(",a.risoku");
//			max++;
//		}
//		strSQL += _T(" from ");
//		strSQL += strMtbl;
//		strSQL += _T(" a left join uc_lst_kamoku b on a.knseq=b.knseq");
//// cor -->
//		//strSQL2.Format(_T(" where a.fgshow=%d and a.numgroup=%d and b.prsign!=1"),ID_FGSHOW_IKKATU,rsData->m_NumGroup);
//// -------
//		strSQL2.Format(_T(" where a.fgshow=%d and a.numgroup=%d"),ID_FGSHOW_IKKATU,rsData->m_NumGroup);
//		if(m_KmPrMiny == 0)	{
//			strSQL2 += _T(" and b.prsign!=1");
//		}
//		else	{
//			strSQL2 += _T(" and (b.prsign!=1 or b.knseq is null)");
//		}
//// cor <--
//		strSQL += strSQL2;
//		rs.Open(CRecordset::forwardOnly,strSQL);
//		cnt=0;
//		total[0]=total[1]=0;
//		while(!rs.IsEOF())	{
//			for(ind=0; ind<max; ind++)	{
//				rs.GetFieldValue(ind,strData[ind]);
//				// CString型 → char型
//				memset(bf,0,128);
//				lstrcpy(bf,strData[ind]);
//				// char型 → __int64型
//				vl[ind] = _strtoi64(bf,NULL,10);
//				total[ind] = total[ind] + vl[ind];
//			}
//			cnt++;
//			rs.MoveNext();
//		}
//		rs.Close();
//		// 一括金額レコードの名称（その他○件）
//		if(cnt > 0)	{
//			strName.Format("%s %d%s",NAME_IKKATU_1,cnt,NAME_IKKATU_2);
//			for(ind=0; ind<max; ind++)	{
//				// __int64型 → char型
//				memset(bf,0,128);
//				_i64toa_s(total[ind],bf,32,10);
//				strData[ind].Format(_T("%s"),bf);
//			}
//			// 更新
//			rsData->Edit();
//			if( m_nFormSeq == ID_FORMNO_021 || 
//				m_nFormSeq == ID_FORMNO_031 || 
//				m_nFormSeq == ID_FORMNO_041 || 
//				m_nFormSeq == ID_FORMNO_091 || 
//				m_nFormSeq == ID_FORMNO_101 || 
//				(m_nFormSeq >= ID_FORMNO_171 && m_nFormSeq <= ID_FORMNO_1720) )	{
//				rsData->m_AdName1	= strName;			// 名称：その他?件
//			}
//			else if(m_nFormSeq == ID_FORMNO_051)	{
//				rsData->m_HdName	= strName;			// 名称：その他?件
//			}
//			else if(m_nFormSeq == ID_FORMNO_111)	{
//				rsData->m_AdAdd2	= strName;			// 名称：その他?件
//				rsData->m_Risoku	= strData[1];		// 支払利息
//			}
//			else if(m_nFormSeq == ID_FORMNO_161 || m_nFormSeq == ID_FORMNO_162)	{
//				rsData->m_Naiyou	= strName;			// 名称：その他?件
//			}
//			else if(m_nFormSeq >= ID_FORMNO_181)	{
//				rsData->m_Item		= strName;			// 名称：その他?件
//			}
//			rsData->m_Val			= strData[0];		// 金額："0:Val"固定
//			rsData->Update();
//		}
//		else	{
//			rsData->Delete();
//		}
//		// 次のレコードへ
//		rsData->MoveNext();
//	}
//
//	rsData->Fin();
//	delete rsData;
//
//	return FUNCTION_OK;
//}
//
////******************************************************************************
//// 科目指定する＋科目でソートを行わない
//// 不要になった計行の削除
////******************************************************************************
//int CdbKamokuSitei::Kn_CmnKeiDelete()
//{
//	int				mcnt=0;				// 【小計】間の明細カウント
//	int				mcnt2=0;			// 
//	int				save=0;				// 前行のフラグ
//	int				dflg=0;				// 削除フラグ
//	CString			strFilter=_T("");
//	CString			strSort=_T("");
//	CdbPrtWork*		rsData;
//	CdbPrtWork*		rsData2;
//	CdbUcInfSub		mfcRec(m_pDb);		// uc_inf_subテーブルクラス
//
//	rsData = new CdbPrtWork(m_pDb,m_nFormSeq);
//	rsData2 = new CdbPrtWork(m_pDb,m_nFormSeq);
//
//	// -----------------------------------------------------------------
//	// 不要な小計行の削除
//	// -----------------------------------------------------------------
//	strSort = _T("NumPage,NumRow");
//	if(rsData->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
//		rsData->Fin();
//		delete rsData;
//		return(FUNCTION_NG);
//	}
//	mcnt=0;
//	while(!rsData->IsEOF())	{
//		// 小計行
//		if(rsData->m_FgFunc == ID_FGFUNC_SYOKEI || rsData->m_FgFunc == ID_FGFUNC_SYOKEINULL)	{
//			// 明細のない【小計】行は削除する
//			if(mcnt == 0)	{
//				rsData->Delete();
//			}
//			else	{
//				if(m_OpOutSyokei == 0)	{
//					// ソートの小計設定を参照し「一行でも小計を挿入する」にチェックが付いていない場合
//					if(mcnt == 1)	{
//						// 明細が一行しかない小計行は空白に置き換える
//						if(rsData->m_FgFunc == ID_FGFUNC_SYOKEI)	{
//							rsData->Edit();
//							rsData->m_FgFunc = ID_FGFUNC_SYOKEINULL;
//							rsData->m_KeiStr = _T("");
//							rsData->m_KnSeq = 0;
//							rsData->m_KnOrder = 0;
//							rsData->m_KnName = _T("");
//							rsData->m_Val = _T("");
//							switch(m_nFormSeq)	{
//								case	ID_FORMNO_021:	// ②  :受取手形の内訳書
//								case	ID_FORMNO_031:	// ③  :売掛金の内訳書
//								case	ID_FORMNO_041:	// ④-1:仮払金の内訳書
//								case	ID_FORMNO_091:	// ⑨  :買掛金の内訳書
//								case	ID_FORMNO_101:	// ⑩-1:仮受金の内訳書
//									rsData->m_AdName1 = _T("");
//									break;
//								case	ID_FORMNO_061:	// ⑥  :有価証券の内訳書
//									rsData->m_Val2 = _T("");
//									rsData->m_Val3 = _T("");
//									break;
//								case	ID_FORMNO_051:	// ⑤  :棚卸資産の内訳書
//									rsData->m_HdName = _T("");
//									break;
//								case	ID_FORMNO_111:	// ⑪  :借入金及び支払利子の内訳書
//									rsData->m_AdAdd2 = _T("");
//									rsData->m_Risoku = _T("");
//									break;
//								case	ID_FORMNO_121:	// ⑫  :土地の売上高等の内訳書
//									rsData->m_Val2 = _T("");
//									break;
//								case	ID_FORMNO_161:	// ⑯-1:雑益等の内訳書
//								case	ID_FORMNO_162:	// ⑯-2:雑益等の内訳書
//									rsData->m_Naiyou = _T("");
//									break;
//								case	ID_FORMNO_171:	// ⑰  :その他の内訳書１
//								case	ID_FORMNO_172:
//								case	ID_FORMNO_173:
//								case	ID_FORMNO_174:
//								case	ID_FORMNO_175:
//								case	ID_FORMNO_176:
//								case	ID_FORMNO_177:
//								case	ID_FORMNO_178:
//								case	ID_FORMNO_179:
//								case	ID_FORMNO_1710:
//								case	ID_FORMNO_1711:
//								case	ID_FORMNO_1712:
//								case	ID_FORMNO_1713:
//								case	ID_FORMNO_1714:
//								case	ID_FORMNO_1715:
//								case	ID_FORMNO_1716:
//								case	ID_FORMNO_1717:
//								case	ID_FORMNO_1718:
//								case	ID_FORMNO_1719:
//								case	ID_FORMNO_1720:
//									rsData->m_AdName1 = _T("");
//									break;
//								case	ID_FORMNO_181:	// ⑱  :その他の内訳書２
//								case	ID_FORMNO_182:
//								case	ID_FORMNO_183:
//								case	ID_FORMNO_184:
//								case	ID_FORMNO_185:
//								case	ID_FORMNO_186:
//								case	ID_FORMNO_187:
//								case	ID_FORMNO_188:
//								case	ID_FORMNO_189:
//								case	ID_FORMNO_1810:
//								case	ID_FORMNO_1811:
//								case	ID_FORMNO_1812:
//								case	ID_FORMNO_1813:
//								case	ID_FORMNO_1814:
//								case	ID_FORMNO_1815:
//								case	ID_FORMNO_1816:
//								case	ID_FORMNO_1817:
//								case	ID_FORMNO_1818:
//								case	ID_FORMNO_1819:
//								case	ID_FORMNO_1820:
//									rsData->m_Item = _T("");
//									break;
//								default:
//									break;
//							}
//							rsData->Update();
//						}
//					}
//				}
//			}
//			mcnt=0;
//		}
//		// 通常(データ)行または、一括集計金額行
//		// ※DeleteKeiNullKamokuRow関数で値を書き換えているので注意
//		if( rsData->m_FgFunc == ID_FGFUNC_DATA_TEMP || 
//			rsData->m_FgFunc == ID_FGFUNC_IKKATUMANUAL_ZERO || 
//			rsData->m_FgFunc == ID_FGFUNC_IKKATUAUTO_ZERO )	{
//			mcnt++;
//		}
//		// 次のレコードへ
//		rsData->MoveNext();
//	}
//
//	// -----------------------------------------------------------------
//	// 不要な中計行の削除
//	// -----------------------------------------------------------------
//	strSort = _T("NumPage,NumRow");
//	if(rsData->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
//		rsData->Fin();
//		delete rsData;
//		return(FUNCTION_NG);
//	}
//	mcnt=0;
//	while(!rsData->IsEOF())	{
//		// 中計行
//		if(rsData->m_FgFunc == ID_FGFUNC_CHUKEI || rsData->m_FgFunc == ID_FGFUNC_CHUKEINULL)	{
//			// 中計～中計の間に、小計行が１件以下の場合
//			if(mcnt <= 1)	{
//				// 次行以降に有効な中計行がある場合は削除しない
//				dflg=1;
//				strSort = _T("NumPage,NumRow");
//				if(rsData2->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
//					rsData2->Fin();
//					delete rsData2;
//					return(FUNCTION_NG);
//				}
//				// 次行以降のチェック
//				mcnt2=0;
//				while(!rsData2->IsEOF())	{
//					// 次行以降のみチェックする
//					if((rsData2->m_NumPage > rsData->m_NumPage) || 
//						(rsData2->m_NumPage == rsData->m_NumPage && rsData2->m_NumRow > rsData->m_NumRow))	{
//						// 中計行
//						if(rsData2->m_FgFunc == ID_FGFUNC_CHUKEI || rsData2->m_FgFunc == ID_FGFUNC_CHUKEINULL)	{
//							if(mcnt2 > 1)	{
//								dflg=0;
//								break;
//							}
//							mcnt2=0;
//						}
//						// 小計行
//						if(rsData2->m_FgFunc == ID_FGFUNC_SYOKEI || rsData2->m_FgFunc == ID_FGFUNC_SYOKEINULL)	{
//							mcnt2++;
//						}
//					}
//					// 次のレコードへ
//					rsData2->MoveNext();
//				}
//				if(dflg == 1)	{
//					rsData->Delete();
//				}
//			}
//			mcnt=0;
//		}
//		// 小計行
//		if(rsData->m_FgFunc == ID_FGFUNC_SYOKEI || rsData->m_FgFunc == ID_FGFUNC_SYOKEINULL)	{
//			mcnt++;
//		}
//		// 次のレコードへ
//		rsData->MoveNext();
//	}
//
//	// -----------------------------------------------------------------
//	// 不要な科目見出し行の削除
//	// -----------------------------------------------------------------
//	if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111)	{
//		strFilter.Format(_T("FgFunc=%d"),ID_FGFUNC_KAMOKU);
//		strSort = _T("NumPage,NumRow");
//		if(rsData->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
//			rsData->Fin();
//			delete rsData;
//			return(FUNCTION_NG);
//		}
//		save=-1;
//		while(!rsData->IsEOF())	{
//			if(rsData->m_KnSeq == save)	{
//				save = rsData->m_KnSeq;
//				rsData->Delete();
//			}
//			else	{
//				save = rsData->m_KnSeq;
//			}
//			// 次のレコードへ
//			rsData->MoveNext();
//		}
//	}
//
//	// -----------------------------------------------------------------
//	// ２行以上続く空白行の削除
//	// -----------------------------------------------------------------
//	strSort = _T("NumPage,NumRow");
//	if(rsData->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
//		rsData->Fin();
//		delete rsData;
//		return(FUNCTION_NG);
//	}
//	save=-1;
//	while(!rsData->IsEOF())	{
//		if(rsData->m_FgFunc == ID_FGFUNC_NULL && (save == ID_FGFUNC_NULL || save == ID_FGFUNC_SYOKEINULL))	{
//			save = rsData->m_FgFunc;
//			rsData->Delete();
//		}
//		else	{
//			save = rsData->m_FgFunc;
//		}
//		// 次のレコードへ
//		rsData->MoveNext();
//	}
//
//	// -----------------------------------------------------------------
//	// 先頭空白行の削除
//	// -----------------------------------------------------------------
//	strSort = _T("NumPage,NumRow");
//	if(rsData->RequerySortParameter(strFilter,strSort) != FUNCTION_OK)	{
//		rsData->Fin();
//		delete rsData;
//		return(FUNCTION_NG);
//	}
//	save=-1;
//	if(!rsData->IsEOF())	{
//		if(rsData->m_FgFunc == ID_FGFUNC_NULL)	{
//			rsData->Delete();
//		}
//	}
//
//	// -----------------------------------------------------------------
//	// 小計/中計/頁計/累計の再計算
//	// -----------------------------------------------------------------
//	Ks_CmnTblCalcKei();
//	// 小計の自動挿入のときのみ
//	if( m_NullSyokeiRow != FALSE ){
//		// 小計のみ上書き
//		Ks_CmnTblCalcKeiAfterSort();
//		m_NullSyokeiRow = FALSE;
//	}
//
//	rsData->Fin();
//	delete rsData;
//
//	rsData2->Fin();
//	delete rsData2;
//
//	return FUNCTION_OK;
//}
// midori 152137 del <--

// midori 152137 add -->
//**************************************************
//	ソート項目（カナ入力有り）の種類を取得
//	【引数】	intFormSeq  …　新財務のFormSeq番号
//				intItemSeq	…　ソート項目のシーケンス番号
//	【戻値】	1			…	科目
//				2			…	銀行
//				3			…	取引先
//				0			…	上記以外
//**************************************************
int CdbKamokuSitei::GetSortItem(int intFormSeq,int intItemSeq)
{
	int ret = 0;		// 戻り値

	switch (intFormSeq) {
	case ID_FORMNO_011:
		// 金融機関
		if(intItemSeq == 1)			ret = 2;
		// 種類
		else if(intItemSeq == 2)	ret = 1;
		break;
	case ID_FORMNO_021:
		// 振出人
		if(intItemSeq == 1)			ret = 3;
		// 科目
		else if(intItemSeq == 8)	ret = 1;
		break;
	case ID_FORMNO_031:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先(名称)
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_041:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先(名称)
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_042:
		// 貸付先
		if(intItemSeq == 1)			ret = 3;
		break;
	case ID_FORMNO_051:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_061:
		// 区分
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_071:
		break;
	case ID_FORMNO_081:
		// 支払先
		if(intItemSeq == 1)			ret = 3;
// 改良No.21-0086,21-0529 add -->
		// 科目
		else if(intItemSeq == 9)	ret = 1;
// 改良No.21-0086,21-0529 add <--
		break;
	case ID_FORMNO_091:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_101:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_102:
		// 所得の種類
		if(intItemSeq == 3)			ret = 1;
		break;
	case ID_FORMNO_111:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 借入先
		else if(intItemSeq == 2)	ret = 2;
		break;
	case ID_FORMNO_121:
		// 区分
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_131:
		break;
	// ソート無し
	case ID_FORMNO_141:
	case ID_FORMNO_142:
		break;
	case ID_FORMNO_151:
		// 地代・家賃の区分
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_152:
		break;
	case ID_FORMNO_153:
		break;
	case ID_FORMNO_161:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 3)	ret = 3;
		break;
	case ID_FORMNO_162:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 3)	ret = 3;
		break;
	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		break;
	default:
		break;
	}

	return(ret);
}
// midori 152137 add <--

// midori 190505 add -->
int CdbKamokuSitei::Ks_ZeroNullCheck( CdbPrtWork* rsData, int pFormSeq, int pSw )
{
	int			ret=0;
	int			hikaku=0;
	CString		strKingaku1=_T("");
	CString		strKingaku2=_T("");
	CString		strKingaku3=_T("");
	char		prmKingaku1[32]={0};
	char		prmKingaku2[32]={0};
	char		prmKingaku3[32]={0};
	__int64		i64Kingaku1=0;
	__int64		i64Kingaku2=0;
	__int64		i64Kingaku3=0;

	// ⑥有価証券の内訳書
	if(pFormSeq == ID_FORMNO_061)	{
		// 金額①
		strKingaku1 = rsData->m_Val2;					// CString型 → CString型
		if(pSw != 0)	{
			// 一括金額の判定時、金額空欄は０に置き換える(従来は集約した結果で判定を行っていたため)
			if(strKingaku1.IsEmpty() == TRUE)	strKingaku1 = _T("0");
		}
		lstrcpy(prmKingaku1,strKingaku1);				// CString型 → Char型
		i64Kingaku1 = _strtoi64(prmKingaku1,NULL,10);	// Char型 → __int64型
		// 金額②
		strKingaku2 = rsData->m_Val3;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku2.IsEmpty() == TRUE)	strKingaku2  = _T("0");
		}
		lstrcpy(prmKingaku2,strKingaku2);				// CString型 → Char型
		i64Kingaku2 = _strtoi64(prmKingaku2,NULL,10);	// Char型 → __int64型
		// 比較する金額の数
		hikaku = 2;
	}
	// ⑦固定資産の内訳書
	else if(pFormSeq == ID_FORMNO_071)	{
		// 金額①
		strKingaku1 = rsData->m_Val;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku1.IsEmpty() == TRUE)	strKingaku1 = _T("0");
		}
		lstrcpy(prmKingaku1,strKingaku1);				// CString型 → Char型
		i64Kingaku1 = _strtoi64(prmKingaku1,NULL,10);	// Char型 → __int64型
		// 金額②
		strKingaku2 = rsData->m_MsVal;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku2.IsEmpty() == TRUE)	strKingaku2 = _T("0");
		}
		lstrcpy(prmKingaku2,strKingaku2);				// CString型 → Char型
		i64Kingaku2 = _strtoi64(prmKingaku2,NULL,10);	// Char型 → __int64型
		// 金額③
		strKingaku3 = rsData->m_MsVal2;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku3.IsEmpty() == TRUE)	strKingaku3 = _T("0");
		}
		lstrcpy(prmKingaku3,strKingaku3);				// CString型 → Char型
		i64Kingaku3 = _strtoi64(prmKingaku3,NULL,10);	// Char型 → __int64型
		// 比較する金額の数
		hikaku = 3;
	}
	// ⑪借入金及び支払利子の内訳書
	else if(pFormSeq == ID_FORMNO_111)	{
		// 金額①
		strKingaku1 = rsData->m_Val;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku1.IsEmpty() == TRUE)	strKingaku1 = _T("0");
		}
		lstrcpy(prmKingaku1,strKingaku1);				// CString型 → Char型
		i64Kingaku1 = _strtoi64(prmKingaku1,NULL,10);	// Char型 → __int64型
		// 金額②
		strKingaku2 = rsData->m_Risoku;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku2.IsEmpty() == TRUE)	strKingaku2 = _T("0");
		}
		lstrcpy(prmKingaku2,strKingaku2);				// CString型 → Char型
		i64Kingaku2 = _strtoi64(prmKingaku2,NULL,10);	// Char型 → __int64型
		// 比較する金額の数
		hikaku = 2;
	}
	// ⑫土地の売上高等の内訳書
	else if(pFormSeq == ID_FORMNO_121)	{
		// 金額①
		strKingaku1 = rsData->m_Val;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku1.IsEmpty() == TRUE)	strKingaku1 = _T("0");
		}
		lstrcpy(prmKingaku1,strKingaku1);				// CString型 → Char型
		i64Kingaku1 = _strtoi64(prmKingaku1,NULL,10);	// Char型 → __int64型
		// 金額②
		strKingaku2 = rsData->m_Val2;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku2.IsEmpty() == TRUE)	strKingaku2 = _T("0");
		}
		lstrcpy(prmKingaku2,strKingaku2);				// CString型 → Char型
		i64Kingaku2 = _strtoi64(prmKingaku2,NULL,10);	// Char型 → __int64型
		// 比較する金額の数
		hikaku = 2;
	}
	// その他
	else	{
		// 金額①
		strKingaku1 = rsData->m_Val;					// CString型 → CString型
		if(pSw != 0)	{
			if(strKingaku1.IsEmpty() == TRUE)	strKingaku1 = _T("0");
		}
		lstrcpy(prmKingaku1,strKingaku1);				// CString型 → Char型
		i64Kingaku1 = _strtoi64(prmKingaku1,NULL,10);	// Char型 → __int64型
		// 比較する金額の数
		hikaku = 1;
	}

	// 比較する金額の数
	if(hikaku == 3)	{
		// 判定
		if((m_OutZero & BIT_D0) && (m_OutZero & BIT_D1))	{
			// 金額０円を出力しない ＆ 金額空欄を出力しない
			if( (i64Kingaku1 == 0 || strKingaku1.IsEmpty() == TRUE) && 
				(i64Kingaku2 == 0 || strKingaku2.IsEmpty() == TRUE) && 
				(i64Kingaku3 == 0 || strKingaku3.IsEmpty() == TRUE) )	{
				ret = 1;
			}
		}
		else if(m_OutZero & BIT_D0)	{
			// 金額０円を出力しない
			if( (i64Kingaku1 == 0 && strKingaku1.IsEmpty() != TRUE) && 
				(i64Kingaku2 == 0 && strKingaku2.IsEmpty() != TRUE) &&
				(i64Kingaku3 == 0 && strKingaku3.IsEmpty() != TRUE) )	{
				ret = 1;
			}
		}
		else if(m_OutZero & BIT_D1)	{
			// 金額空欄を出力しない
			if((strKingaku1.IsEmpty() == TRUE) && (strKingaku2.IsEmpty() == TRUE) && (strKingaku3.IsEmpty() == TRUE))	{
				ret = 1;
			}
		}
	}
	else if(hikaku == 2)	{
		// 判定
		if((m_OutZero & BIT_D0) && (m_OutZero & BIT_D1))	{
			// 金額０円を出力しない ＆ 金額空欄を出力しない
			if( (i64Kingaku1 == 0 || strKingaku1.IsEmpty() == TRUE) && 
				(i64Kingaku2 == 0 || strKingaku2.IsEmpty() == TRUE) )	{
				ret = 1;
			}
		}
		else if(m_OutZero & BIT_D0)	{
			// 金額０円を出力しない
			if( (i64Kingaku1 == 0 && strKingaku1.IsEmpty() != TRUE) && 
				(i64Kingaku2 == 0 && strKingaku2.IsEmpty() != TRUE) )	{
				ret = 1;
			}
		}
		else if(m_OutZero & BIT_D1)	{
			// 金額空欄を出力しない
			if((strKingaku1.IsEmpty() == TRUE) && (strKingaku2.IsEmpty() == TRUE))	{
				ret = 1;
			}
		}
	}
	else	{
		// 判定
		if((m_OutZero & BIT_D0) && (m_OutZero & BIT_D1))	{
			// 金額０円を出力しない ＆ 金額空欄を出力しない
			if( (i64Kingaku1 == 0 || strKingaku1.IsEmpty() == TRUE) )	{
				ret = 1;
			}
		}
		else if(m_OutZero & BIT_D0)	{
			// 金額０円を出力しない
			if( (i64Kingaku1 == 0 && strKingaku1.IsEmpty() != TRUE) )	{
				ret = 1;
			}
		}
		else if(m_OutZero & BIT_D1)	{
			// 金額空欄を出力しない
			if( strKingaku1.IsEmpty() == TRUE )	{
				ret = 1;
			}
		}
	}

	return(ret);
}
// midori 190505 add <--
