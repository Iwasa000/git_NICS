//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc181Sonota2.cpp
//
// ■印刷関連 動作定義
//
//**************************************************************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "StdAfx.h"
#include ".\prtJobFunction.h"
#include "prtUc181Sonota2.h"

// No.200903 add -->
extern BOOL	bG_Kanso;
// No.200903 add <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc181()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑱．その他２
//
// 引数		：int				nPage				ページ番号
//			：int				nFormSeq			様式シーケンス番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc181Sonota2::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq)
{
	int				nCurRow			= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strKnNameOld	= _T( "" );
	CString			strSQL;
	CharController	cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc181Sonota2	rc( m_pDB, nFormSeq );

// No.200904 add -->
	// 計の出力設定
	int nOutKei = rc.GetOutKei( nFormSeq );
// No.200904 add <--

	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 160610 cor -->
//////		if(!(po.OutZeroNull[0])){
////// ---------------------
////		if(!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0))	{
////// midori 160610 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[0] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[0] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(nFormSeq, ck1, ck2);
//
//		if((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || (po.OutZeroNull[0] != 0 && rt == 2))	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if(!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0))	{
// midori 157001 add <--
			// 指定されたページの行数を取得
			m_nRowMax = rc.GetNumRow( nPage );

			rc.Init( nPage );
		}
		else{
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( nFormSeq );
// midori 160610 cor -->
			//strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", nFormSeq, nPage );
// ---------------------
#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", nFormSeq, nPage );
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", nFormSeq, nPage );
#endif
// midori 160610 cor <--
			rc.Open( CRecordset::forwardOnly, strSQL );
		}

		// 最終レコードまで処理を実行
		while( !rc.IsEOF() ){

			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if( nCurRow >= 0 ){

				// 空行
				if( CheckDbDataFgFuncNull( &rc ) ){
					
					strKnNameOld	= _T( "" );
					continue;
				}

// No.200904 add -->
				// 頁計・累計種別 0:なしの場合、頁計、累計を出力しない
				if(nOutKei == ID_OUTKEI_OFF && (rc.m_FgFunc == ID_FGFUNC_RUIKEI || rc.m_FgFunc == ID_FGFUNC_PAGEKEI)) {
					strKnNameOld	= _T( "" );
					// 次のレコードへ移動
					rc.MoveNext();
					continue;
				}
// No.200904 add <--

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 期末現在高
// No.200903 del -->
				//m_PrtDataBox[Field181Val].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[Field181Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
				}
				else	{
					m_PrtDataBox[Field181Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				}
// No.200903 add <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					//m_PrtDataBox[Field181KeiStr].Data[nKeiRow] = rc.m_KeiStr;
					m_PrtDataBox[Field181KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					// 科目（名称）
					if( CheckOmit( rc.m_KnName, strKnNameOld, 0, m_PrtInfOmitList1 ) ){

						m_PrtDataBox[Field181KnNameOm].Data[nCurRow] =
							m_PrtInfOmitList1[0].OmitStr;
					}
					else{

						CString strDividedName1, strDividedName2;
						// 分割後の文字列取得
						cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
						// 9文字以上なら2行表示
						if( !(strDividedName2.IsEmpty()) ){
							// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
							if( strDividedName1.GetLength() == 15 ){
								strDividedName1 += _T(" ");
							}
							// 半角スペースで16バイトまで埋める
							strDividedName2 = FillSpace( strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2 );

							m_PrtDataBox[Field181KnName1].Data[nCurRow] = strDividedName1;
							m_PrtDataBox[Field181KnName2].Data[nCurRow] = strDividedName2;
						}
						else{
							m_PrtDataBox[Field181KnName2].Data[nCurRow] = rc.m_KnName;
						}
					}
					strKnNameOld = rc.m_KnName;


					// 項目
					StrDivision( rc.m_Item, &strArray, 2 );
					m_PrtDataBox[Field181Item1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field181Item2].Data[nCurRow] = strArray.GetAt( 1 );

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 2 );
					m_PrtDataBox[Field181Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field181Teki2].Data[nCurRow] = strArray.GetAt( 1 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		// 項目タイトルを表示
		int nRowMax2 = GetDbDataUc182( nFormSeq );

		// 行数の多い方を使用
		if( nRowMax2 > m_nRowMax ){

			m_nRowMax = nRowMax2;
		}

		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc181()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}


////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc182()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑱．その他２（項目タイトル）
//
// 引数		：int				nFormSeq			様式シーケンス番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataUc182(int nFormSeq)
{
	CdbUcInfSonotaItem	rc( m_pDB );

	try{

		rc.Init( nFormSeq );

		if( !rc.IsEOF() ){

			// 項目タイトル
			m_PrtDataBox[Field181ItemName].Data[0] = rc.m_ItemName;
		}

		return 1;	// 必ず1行
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc182()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc181Sonota2::CprtUc181Sonota2(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_181;
	m_nRowMax	= 0;
	m_nColumn	= Field181Max;
}

CprtUc181Sonota2::~CprtUc181Sonota2(void)
{
}

CprtJobFunction* CprtUc181Sonota2::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc181Sonota2(m_pDB));
}