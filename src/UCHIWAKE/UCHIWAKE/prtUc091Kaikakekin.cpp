//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc091Kaikakekin.cpp
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
#include "prtUc091Kaikakekin.h"
#include "ConvOutRangai.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--

extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/25 add

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc091()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑨．買掛金
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc091Kaikakekin::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;

	if(bG_InvNo == TRUE) {
		rv = GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} else {
		rv = GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

int CprtUc091Kaikakekin::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow			= 0;
	int				nMaxPage		= 0;
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strAdAdd		= _T( "" );
	CString			strKnNameOld	= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;
	CString			strMaxPage;
	CharController	cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc091Kaikakekin	rc( m_pDB );
	CRecordset			rs( m_pDB );

	try{
		if(!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0))	{
			// 指定されたページの行数を取得
			m_nRowMax = rc.GetNumRow( nPage );

			rc.Init( nPage );

			// 最大ページ数取得
			nMaxPage = rc.GetNumPage();
		}
		else{
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

			// 最大ページ数取得
#ifdef _DEBUG
			strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM ##temp_utiwake_tbl_%d ", m_nFormSeq );
#else
			strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM #temp_utiwake_tbl_%d ", m_nFormSeq );
#endif
			rs.Open( CRecordset::forwardOnly, strSQL );

			rs.GetFieldValue( _T("maxpage"), strMaxPage );

			sscanf_s( strMaxPage, "%d", &nMaxPage);
			rs.Close();

#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#endif
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
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 期末現在高
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[Field091IVal].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
				}
				else 	{
					m_PrtDataBox[Field091IVal].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				}

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[Field091IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field091IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 科目（名称）
							case 1:

								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field091IKnNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									CString strDividedName1, strDividedName2;
									// 分割後の文字列取得
									cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_INVOICE );
									// 9文字以上なら2行表示
									if( !(strDividedName2.IsEmpty()) ){
										// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
										if( strDividedName1.GetLength() == 15 ){
											strDividedName1 += _T(" ");
										}
										// 半角スペースで16バイトまで埋める
										strDividedName2 = FillSpace( strDividedName2, DR_INPUTLENGTH_INVOICE * 2 );

										m_PrtDataBox[Field091IKnName1].Data[nCurRow] = strDividedName1;
										m_PrtDataBox[Field091IKnName2].Data[nCurRow] = strDividedName2;
									}
									else{
										m_PrtDataBox[Field091IKnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

								strKnNameOld = rc.m_KnName;
								break;

							// 相手先（名称）
							case 2:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field091IAdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field091IAdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field091IAdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 相手先（所在地）
							case 3:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field091IAdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field091IAdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field091IAdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 2 );
					m_PrtDataBox[Field091ITeki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field091ITeki2].Data[nCurRow] = strArray.GetAt( 1 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		// 出力状態に応じて、下欄のデータを表示
		// 1ページ目のみ、下欄のデータを表示
		int nRowMax2 = GetDbDataUc092New(nPage,nMaxPage,po);

		// 行数の多い方を使用
		if( nRowMax2 > m_nRowMax ){

			m_nRowMax = nRowMax2;
		}

		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc091()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/10/25 add <--

// インボイス登録番号追加対応_23/10/25 del -->
//int CprtUc091Kaikakekin::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc091Kaikakekin::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 add <--
{
	int				nCurRow			= 0;
	int				nMaxPage		= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strAdAdd		= _T( "" );
	CString			strKnNameOld	= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;
	CString			strMaxPage;
	CharController	cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc091Kaikakekin	rc( m_pDB );
	CRecordset			rs( m_pDB );

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

			// 最大ページ数取得
			nMaxPage = rc.GetNumPage();
		}
		else{
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

			// 最大ページ数取得
// midori 160610 cor -->
			//strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM #temp_utiwake_tbl_%d ", m_nFormSeq );
// ---------------------
#ifdef _DEBUG
			strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM ##temp_utiwake_tbl_%d ", m_nFormSeq );
#else
			strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM #temp_utiwake_tbl_%d ", m_nFormSeq );
#endif
// midori 160610 cor <--
			rs.Open( CRecordset::forwardOnly, strSQL );

			rs.GetFieldValue( _T("maxpage"), strMaxPage );

			sscanf_s( strMaxPage, "%d", &nMaxPage);
			rs.Close();

// midori 160610 cor -->
			//strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
// ---------------------
#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
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
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 期末現在高
// No.200903 del -->
				//m_PrtDataBox[Field091Val].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[Field091Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
				}
				else 	{
					m_PrtDataBox[Field091Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				}
// No.200903 add <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[Field091KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 科目（名称）
							case 1:

								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field091KnNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
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

										m_PrtDataBox[Field091KnName1].Data[nCurRow] = strDividedName1;
										m_PrtDataBox[Field091KnName2].Data[nCurRow] = strDividedName2;
									}
									else{
										m_PrtDataBox[Field091KnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

								strKnNameOld = rc.m_KnName;
								break;

							// 相手先（名称）
							case 2:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field091AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field091AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field091AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 相手先（所在地）
							case 3:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field091AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field091AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field091AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 2 );
					m_PrtDataBox[Field091Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field091Teki2].Data[nCurRow] = strArray.GetAt( 1 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		// 出力状態に応じて、下欄のデータを表示
		// 1ページ目のみ、下欄のデータを表示
		//if( nPage == 1 ){

// midori 155525 del -->
		//int nRowMax2 = GetDbDataUc092( nPage, nMaxPage );
// midori 155525 del <--
// midori 155525 add -->
		int nRowMax2 = GetDbDataUc092(nPage,nMaxPage,po);
// midori 155525 add <--

		// 行数の多い方を使用
		if( nRowMax2 > m_nRowMax ){

			m_nRowMax = nRowMax2;
		}
		//}
		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc091()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc092()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑨．買掛金（下欄）
// 引数		：int				現在のページ
//			：int				最大ページ
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// midori 155525 del -->
//int CprtUc091Kaikakekin::GetDbDataUc092(int nPage, int nMaxPage)
// midori 155525 del <--
// midori 155525 add -->
int CprtUc091Kaikakekin::GetDbDataUc092( int nPage, int nMaxPage, const PRT_OPTION& po )
// midori 155525 add <--
{
	int		nCurRow		= 0;
	int		nOutPage	= 1;	// 欄外出力ページ
// midori 155525 add -->
	int		nPrtRowL	= 0;
	int		nPrtRowR	= 0;
// midori 155525 add <--

	CdbUc092Kaikakekin2	rc( m_pDB );
	CdbUc092Kaikakekin2	rc2( m_pDB );

	try{
		rc.RequeryPage(1);
		if(rc.IsEOF())	{
			rc.Fin();
			return 0;
		}
		
		// 欄外出力形式	出力ページはNumPage = 1のレコードにのみセットされている
		CConvOutRangai cr;
		int PrintType = 0;
		cr.ToMemory(rc.m_OutRangai, m_OutType, PrintType);
		int requery_page = nPage;
		if(m_OutType != 2 || PrintType == 0){	// 全ページ以外　またはページ共通出力なら強制的に欄外1ページ目を出力する
			requery_page = 1;	// 全ページ以外
			rc2.RequeryPage(requery_page);
			if(rc2.IsEOF()){
				rc.Fin();
				rc2.Fin();
				return 0;
			}
		}
		else{
			CString strSQL;
			CString tmp_tbl = rc2.GetRangaiTblName();
			strSQL.Format("SELECT * FROM %s WHERE NumPage = %d ORDER BY NumPage, NumRow", tmp_tbl, requery_page);
			rc2.Open(CRecordset::forwardOnly, strSQL);
		}

		if(rc2.IsEOF()){
			rc.Fin();
			rc2.Fin();
			return 4;	// 全て印字しないor指定ページが印字しない
		}
		// 印刷しない
		if( rc2.m_FgShow == ID_FGSHOW_KAIKAKEKIN ){
			rc.Fin();
			rc2.Fin();
			return 4;	// 必ず4行
		}

		switch( m_OutType ){
			case 0:				// 1ページ
				nOutPage = 1;
				break;
			case 1:				// 最終ページ
				nOutPage = nMaxPage;		
// midori M-16113005 add -->
				if(nOutPage < 1)	nOutPage = 1;
// midori M-16113005 add <--
				break;
			case 2:				// 全ページ
				nOutPage = nPage;
				break;
			default:
				nOutPage = 0;
				break;
		}

		if( nPage == nOutPage ){

// midori 155525 add -->
			nPrtRowL = 0;
			nPrtRowR = 0;
// midori 155525 add <--

			// 最終レコードまで処理を実行
			while( !rc2.IsEOF() ){

				nCurRow = rc2.m_NumRow - 1;

				// 行番号が存在するときのみ実行
				if( nCurRow >= 0 ){

					// 空行
					if( CheckDbDataFgFuncNull( &rc2 ) ){

						continue;
					}

// midori 155525 add -->
					if(po.OutZeroNull[0] & BIT_D0)	{
						if(rc2.m_Val == _T("0"))	{
							rc2.MoveNext();
							continue;
						}
					}
					if(po.OutZeroNull[0] & BIT_D1)	{
						if(rc2.m_Val.IsEmpty() == TRUE)	{
							rc2.MoveNext();
							continue;
						}
					}
					if(po.HideZero[0])	{
						rc2.m_Val = HideZeroMoney(rc2.m_Val);
					}
// midori 155525 add <--

// midori 200101 del -->
//					// 左列
//					if( nCurRow < PRT_ROW_092 ){
//// midori 155525 del -->
//						//// 支払確定年月日（役員賞与）
//						//m_PrtDataBox[Field092SpDateL].Data[nCurRow] =
//						//	ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
//
//						//// 期末現在高（役員賞与）
//						//m_PrtDataBox[Field092ValL].Data[nCurRow] =
//						//	ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
//// midori 155525 del <--
//// midori 155525 add -->
//						if((po.OutZeroNull[0] & BIT_D0) || (po.OutZeroNull[0] & BIT_D1))	{
//							// 支払確定年月日（役員賞与）
//							m_PrtDataBox[Field092SpDateL].Data[nPrtRowL] =
//								ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
//							// 期末現在高（役員賞与）
//							m_PrtDataBox[Field092ValL].Data[nPrtRowL] =
//								ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
//							nPrtRowL++;
//						}
//						else	{
//							// 支払確定年月日（役員賞与）
//							m_PrtDataBox[Field092SpDateL].Data[nCurRow] =
//								ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
//							// 期末現在高（役員賞与）
//							m_PrtDataBox[Field092ValL].Data[nCurRow] =
//								ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
//						}
//// midori 155525 add <--
//					}
//					// 右列
//					else{
//// midori 155525 del -->
//						//// 支払確定年月日（役員賞与）
//						//m_PrtDataBox[Field092SpDateR].Data[nCurRow - PRT_ROW_092] =
//						//	ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
//
//						//// 期末現在高（役員賞与）
//						//m_PrtDataBox[Field092ValR].Data[nCurRow - PRT_ROW_092] =
//						//	ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
//// midori 155525 del <--
//// midori 155525 add -->
//						if((po.OutZeroNull[0] & BIT_D0) || (po.OutZeroNull[0] & BIT_D1))	{
//							// 支払確定年月日（役員賞与）
//							m_PrtDataBox[Field092SpDateR].Data[nPrtRowR] =
//								ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
//							// 期末現在高（役員賞与）
//							m_PrtDataBox[Field092ValR].Data[nPrtRowR] =
//								ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
//							nPrtRowR++;
//						}
//						else	{
//							// 支払確定年月日（役員賞与）
//							m_PrtDataBox[Field092SpDateR].Data[nCurRow - PRT_ROW_092] =
//								ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
//							// 期末現在高（役員賞与）
//							m_PrtDataBox[Field092ValR].Data[nCurRow - PRT_ROW_092] =
//								ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
//						}
//// midori 155525 add <--
//					}
// midori 200101 del <--
// midori 200101 add -->
					// 左列
					if( nCurRow < PRT_ROW_092 )	{
						if((po.OutZeroNull[0] & BIT_D0) || (po.OutZeroNull[0] & BIT_D1))	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092SpDateL].Data[nPrtRowL] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092SpDateL].Data[nPrtRowL] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
// No.200903 del -->
							//m_PrtDataBox[Field092ValL].Data[nPrtRowL] =
							//	ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
// No.200903 del <--
// No.200903 add -->
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092ValL].Data[nPrtRowL]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else	{
								m_PrtDataBox[Field092ValL].Data[nPrtRowL]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}
// No.200903 add <--

							nPrtRowL++;
						}
						else	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092SpDateL].Data[nCurRow] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092SpDateL].Data[nCurRow] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
// No.200903 del -->
							//m_PrtDataBox[Field092ValL].Data[nCurRow] =
							//	ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
// No.200903 del <--
// No.200903 add -->
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092ValL].Data[nCurRow]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else	{
								m_PrtDataBox[Field092ValL].Data[nCurRow]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}
// No.200903 add <--
						}
					}
					// 右列
					else	{
						if((po.OutZeroNull[0] & BIT_D0) || (po.OutZeroNull[0] & BIT_D1))	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092SpDateR].Data[nPrtRowR] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092SpDateR].Data[nPrtRowR] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
// No.200903 del -->
							//m_PrtDataBox[Field092ValR].Data[nPrtRowR] =
							//	ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
// No.200903 del <--
// No.200903 add -->
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092ValR].Data[nPrtRowR]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else 	{
								m_PrtDataBox[Field092ValR].Data[nPrtRowR]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}
// No.200903 add <--

							nPrtRowR++;
						}
						else	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092SpDateR].Data[nCurRow - PRT_ROW_092] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092SpDateR].Data[nCurRow - PRT_ROW_092] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
// No.200903 del -->
							//m_PrtDataBox[Field092ValR].Data[nCurRow - PRT_ROW_092] =
							//	ShowFieldOfVal( rc2.m_Val, &rc2, MakeFormatOfNum( 12 ) );
// No.200903 del <--
// No.200903 add -->
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092ValR].Data[nCurRow - PRT_ROW_092]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else	{
								m_PrtDataBox[Field092ValR].Data[nCurRow - PRT_ROW_092]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}
// No.200903 add <--
						}
					}
// midori 200101 add <--
				}

				// 次のレコードへ移動
				rc2.MoveNext();
			}

		}

		// DBアクセスの終了
		rc.Fin();
		rc2.Fin();

		return 4;	// 必ず4行
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc092()\n" );

		if(rc.IsOpen())		rc.Fin();
		if(rc2.IsOpen())	rc2.Fin();
		return FUNCTION_NG;
	}

	return FUNCTION_OK;
}

int CprtUc091Kaikakekin::GetDbDataUc092New( int nPage, int nMaxPage, const PRT_OPTION& po )
{
	int		nCurRow		= 0;
	int		nOutPage	= 1;	// 欄外出力ページ
	int		nPrtRowL	= 0;
	int		nPrtRowR	= 0;

	CdbUc092Kaikakekin2	rc( m_pDB );
	CdbUc092Kaikakekin2	rc2( m_pDB );

	try{
		rc.RequeryPage(1);
		if(rc.IsEOF())	{
			rc.Fin();
			return 0;
		}
		
		// 欄外出力形式	出力ページはNumPage = 1のレコードにのみセットされている
		CConvOutRangai cr;
		int PrintType = 0;
		cr.ToMemory(rc.m_OutRangai, m_OutType, PrintType);
		int requery_page = nPage;
		if(m_OutType != 2 || PrintType == 0){	// 全ページ以外　またはページ共通出力なら強制的に欄外1ページ目を出力する
			requery_page = 1;	// 全ページ以外
			rc2.RequeryPage(requery_page);
			if(rc2.IsEOF()){
				rc.Fin();
				rc2.Fin();
				return 0;
			}
		}
		else{
			CString strSQL;
			CString tmp_tbl = rc2.GetRangaiTblName();
			strSQL.Format("SELECT * FROM %s WHERE NumPage = %d ORDER BY NumPage, NumRow", tmp_tbl, requery_page);
			rc2.Open(CRecordset::forwardOnly, strSQL);
		}

		if(rc2.IsEOF()){
			rc.Fin();
			rc2.Fin();
			return 4;	// 全て印字しないor指定ページが印字しない
		}
		// 印刷しない
		if( rc2.m_FgShow == ID_FGSHOW_KAIKAKEKIN ){
			rc.Fin();
			rc2.Fin();
			return 4;	// 必ず4行
		}

		switch( m_OutType ){
			case 0:				// 1ページ
				nOutPage = 1;
				break;
			case 1:				// 最終ページ
				nOutPage = nMaxPage;		
				if(nOutPage < 1)	nOutPage = 1;
				break;
			case 2:				// 全ページ
				nOutPage = nPage;
				break;
			default:
				nOutPage = 0;
				break;
		}

		if( nPage == nOutPage ){
			nPrtRowL = 0;
			nPrtRowR = 0;

			// 最終レコードまで処理を実行
			while( !rc2.IsEOF() ){
				nCurRow = rc2.m_NumRow - 1;

				// 行番号が存在するときのみ実行
				if( nCurRow >= 0 ){
					// 空行
					if( CheckDbDataFgFuncNull( &rc2 ) ){

						continue;
					}

					if(po.OutZeroNull[0] & BIT_D0)	{
						if(rc2.m_Val == _T("0"))	{
							rc2.MoveNext();
							continue;
						}
					}
					if(po.OutZeroNull[0] & BIT_D1)	{
						if(rc2.m_Val.IsEmpty() == TRUE)	{
							rc2.MoveNext();
							continue;
						}
					}
					if(po.HideZero[0])	{
						rc2.m_Val = HideZeroMoney(rc2.m_Val);
					}

					// 左列
					if( nCurRow < PRT_ROW_092 )	{
						if((po.OutZeroNull[0] & BIT_D0) || (po.OutZeroNull[0] & BIT_D1))	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092ISpDateL].Data[nPrtRowL] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092ISpDateL].Data[nPrtRowL] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092IValL].Data[nPrtRowL]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else	{
								m_PrtDataBox[Field092IValL].Data[nPrtRowL]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}

							nPrtRowL++;
						}
						else	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092ISpDateL].Data[nCurRow] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092ISpDateL].Data[nCurRow] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092IValL].Data[nCurRow]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else	{
								m_PrtDataBox[Field092IValL].Data[nCurRow]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}
						}
					}
					// 右列
					else	{
						if((po.OutZeroNull[0] & BIT_D0) || (po.OutZeroNull[0] & BIT_D1))	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092ISpDateR].Data[nPrtRowR] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092ISpDateR].Data[nPrtRowR] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092IValR].Data[nPrtRowR]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else 	{
								m_PrtDataBox[Field092IValR].Data[nPrtRowR]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}

							nPrtRowR++;
						}
						else	{
							// 支払確定年月日（役員賞与）
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								m_PrtDataBox[Field092ISpDateR].Data[nCurRow - PRT_ROW_092] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_E2 );
							}
							else	{
								m_PrtDataBox[Field092ISpDateR].Data[nCurRow - PRT_ROW_092] =
									ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_B1 );
							}

							// 期末現在高（役員賞与）
							if(bG_Kanso == TRUE)	{
								m_PrtDataBox[Field092IValR].Data[nCurRow - PRT_ROW_092]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(13));
							}
							else	{
								m_PrtDataBox[Field092IValR].Data[nCurRow - PRT_ROW_092]=
									ShowFieldOfVal(rc2.m_Val, &rc2, MakeFormatOfNum(12));
							}
						}
					}
				}

				// 次のレコードへ移動
				rc2.MoveNext();
			}

		}

		// DBアクセスの終了
		rc.Fin();
		rc2.Fin();

		return 4;	// 必ず4行
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc092()\n" );

		if(rc.IsOpen())		rc.Fin();
		if(rc2.IsOpen())	rc2.Fin();
		return FUNCTION_NG;
	}

	return FUNCTION_OK;
}


CprtUc091Kaikakekin::CprtUc091Kaikakekin(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_091;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/25 del -->
	//m_nColumn	= Field091Max;
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field091IMax;
	else					m_nColumn	= Field091Max;
// インボイス登録番号追加対応_23/10/25 add <--
}

CprtUc091Kaikakekin::~CprtUc091Kaikakekin(void)
{
}

CprtJobFunction* CprtUc091Kaikakekin::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc091Kaikakekin(m_pDB));
}