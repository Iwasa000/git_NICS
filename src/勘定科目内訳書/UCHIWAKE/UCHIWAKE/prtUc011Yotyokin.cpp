//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc011Yotyokin.cpp
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
#include "prtUc011Yotyokin.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--


////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc011()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ①．預貯金等
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// midori 190301 del -->
//int CprtUc011Yotyokin::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
//{
//	int					nCurRow			= 0;
//	CString				strBkName		= _T( "" );
//	CString				strBkName2		= _T( "" );
//	CString				strBkNameOld	= _T( "" );
//	CString				strKnNameOld	= _T( "" );
//	CStringArray		strArray;
//	CString				strSQL;
//	CharController		cc;
//
//	// 省略文字の取得
//	GetPrintInfo( parent );
//
//	CdbUc011Yotyokin	rc( m_pDB );
//
//	try{
//// midori 160610 cor -->
////		if(!(po.OutZeroNull[0])){
//// ---------------------
//		if(!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0))	{
//// midori 160610 cor <--
//			// 指定されたページの行数を取得
//			m_nRowMax = rc.GetNumRow( nPage );
//
//			rc.Init( nPage );
//		}
//		else{
//			// 指定されたページの行数を取得（０円テーブル）
//			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );
//
//// midori 160610 cor -->
//			//strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
//// ---------------------
//#ifdef _DEBUG
//			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
//#else
//			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
//#endif
//// midori 160610 cor <--
//			rc.Open( CRecordset::forwardOnly, strSQL );
//		}
//	
//		// 最終レコードまで処理を実行
//		while( !rc.IsEOF() ){			
//
//			nCurRow = rc.m_NumRow - 1;
//
//			// 行番号が存在するときのみ実行
//			if( nCurRow >= 0 ){
//
//				// 空行
//				if( CheckDbDataFgFuncNull( &rc ) ){
//					
//					strBkNameOld	= _T( "" );
//					strKnNameOld	= _T( "" );
//					continue;
//				}
//
//				// データ行の０円非表示
//				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
//					if(po.HideZero[0]){
//						rc.m_Val = HideZeroMoney( rc.m_Val );
//					}
//				}
//
//				// 期末現在高
//				m_PrtDataBox[Field011Val].Data[nCurRow] =
//					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
//
//				// 計行
//				if(	CheckDbDataFgFuncKei( &rc ) ){
//					
//					strBkNameOld	= _T( "" );
//					strKnNameOld	= _T( "" );
//					m_PrtDataBox[Field011KeiStr].Data[nCurRow] = rc.m_KeiStr;
//				}
//				else{
//					// 省略文字使用欄の判別
//					for( int i = 0; i < PRT_MAX_OMIT; i++ ){
//
//						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;
//
//						switch( nItemSeq ){
//
//						// 金融機関
//						case 1:
//
////							strBkName = rc.m_BkName1 + PRT_FORMAT_SLASH + rc.m_BkName2;
//							strBkName = rc.m_BkName1 + PRT_FORMAT_CHCODE + rc.m_BkName2;
//
//							if( CheckOmit( strBkName, strBkNameOld, i, m_PrtInfOmitList1 ) ){
//								
//								m_PrtDataBox[Field011BkNameOm].Data[nCurRow] =
//									m_PrtInfOmitList1[i].OmitStr;
//							}
//							else{
//								if( po.KinyuLine == 0 ){
//									m_PrtDataBox[Field011BkName1].Data[nCurRow] = rc.m_BkName1;
//									m_PrtDataBox[Field011BkName2].Data[nCurRow] =
//										InsertSlashInHead( rc.m_BkName1, rc.m_BkName2 );
//								}
//								else{
//									CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名
//
//									// 分割後の金融機関名（銀行名）取得
//									cc.Divide(rc.m_BkName1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);
//
//									// 金融機関名（銀行名）が1行？
//									if( !DivBkNameDown.IsEmpty() ){
//										m_PrtDataBox[Field011BkName1_1Slash].Data[nCurRow] = DivBkNameUp;
//										m_PrtDataBox[Field011BkName1_2Slash].Data[nCurRow] = DivBkNameDown;
//									}
//									else{	// 1行の場合はスラッシュに高さをあわせる
//										m_PrtDataBox[Field011BkName1Slash].Data[nCurRow] = DivBkNameUp;
//									}
//
//									// 分割後の金融機関名（支店名）取得
//									cc.Divide(rc.m_BkName2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);
//
//									// 金融機関名（支店名）が1行？
//									if( !DivBkNameDown.IsEmpty() ){
//										m_PrtDataBox[Field011BkName2_1Slash].Data[nCurRow] = DivBkNameUp;
//										m_PrtDataBox[Field011BkName2_2Slash].Data[nCurRow] = DivBkNameDown;
//									}
//									else{	// 1行の場合はスラッシュに高さをあわせる
//										m_PrtDataBox[Field011BkName2Slash].Data[nCurRow] = DivBkNameUp;
//									}
//
//									// "/"
//									if(( rc.m_BkName1.IsEmpty() == 0) || ( rc.m_BkName2.IsEmpty() == 0 )){
//										m_PrtDataBox[Field011Slash].Data[nCurRow] = "/";
//									}
//								}
//							}
//
//							strBkNameOld = strBkName;
//							break;
//
//						// 種類
//						case 2:
//
//							if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){
//
//								m_PrtDataBox[Field011KnNameOm].Data[nCurRow] =
//									m_PrtInfOmitList1[i].OmitStr;
//							}
//							else{
//
//								CString strDividedName1, strDividedName2;
//								// 分割後の文字列取得
//								cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
//								// 9文字以上なら2行表示
//								if( !(strDividedName2.IsEmpty()) ){
//									// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
//									if( strDividedName1.GetLength() == 15 ){
//										strDividedName1 += _T(" ");
//									}
//									// 半角スペースで16バイトまで埋める
//									strDividedName2 = FillSpace( strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2 );
//
//									m_PrtDataBox[Field011KnName1].Data[nCurRow] = strDividedName1;
//									m_PrtDataBox[Field011KnName2].Data[nCurRow] = strDividedName2;
//								}
//								else{
//									m_PrtDataBox[Field011KnName2].Data[nCurRow] = rc.m_KnName;
//								}
//							}
//
//							strKnNameOld = rc.m_KnName;
//							break;
//
//						default:
//							break;
//						}
//					}
//
//					// 口座番号
//					m_PrtDataBox[Field011AcNum].Data[nCurRow] = rc.m_AcNum;
//
//					// 摘要
//					StrDivision( rc.m_Teki, &strArray, 2 );
//					m_PrtDataBox[Field011Teki1].Data[nCurRow] = strArray.GetAt( 0 );
//					m_PrtDataBox[Field011Teki2].Data[nCurRow] = strArray.GetAt( 1 );
//				}
//			}
//
//			// 次のレコードへ移動
//			rc.MoveNext();
//		}
//
//		// DBアクセスの終了
//		rc.Fin();
//
//		return m_nRowMax;
//	}
//	catch( CException* e ){
//
//		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc011()\n" );
//
//		if( rc.IsOpen() ){
//
//			rc.Fin();
//		}
//		e->Delete();
//
//		return FUNCTION_NG;
//	}
//
//	return FUNCTION_OK;
//}
// midori 190301 del <--
// midori 190301 add -->
int CprtUc011Yotyokin::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int	rv=0;

	if(bG_Kanso == TRUE)	rv = GetDbDataSheetNew(nPage,parent,po,nFormSeq);
	else					rv = GetDbDataSheetOld(nPage,parent,po,nFormSeq);
	return(rv);
}

int CprtUc011Yotyokin::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int					nCurRow			= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int					rt=0;
//	int					ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CString				strBkName		= _T( "" );
	CString				strStName		= _T( "" );
	CString				strBkNameOld	= _T( "" );
	CString				strStNameOld	= _T( "" );
	CString				strKnNameOld	= _T( "" );
	CString				strTkyo			= _T( "" );
	CString				strTkyo1		= _T( "" );
	CString				strTkyo2		= _T( "" );
	CUcFunctionCommon	clsFunc;
	CStringArray		strArray;
	CString				strSQL;
	CharController		cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc011Yotyokin	rc( m_pDB );

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
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

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
					
					strBkNameOld	= _T( "" );
					strStNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

// No.200903 del -->
				//// 期末現在高
				//m_PrtDataBox[Field011Val].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
				// 期末現在高
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[Field011Val].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0);
				}
				else	{
					m_PrtDataBox[Field011Val].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0);
				}
// No.200903 add <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){
					
					strBkNameOld	= _T( "" );
					strStNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					if(rc.m_NumRow < ID_ROWNORMAL_011)	{
						// 最終ページの"頁計"は、データと同じ行に出力するので左寄り(従来と同じ位置)に出力する
						m_PrtDataBox[Field011KeiStr2].Data[nCurRow] = rc.m_KeiStr;
					}
					else	{
						m_PrtDataBox[Field011KeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

						// 金融機関
						case 1:
							strBkName = rc.m_BkName1;
							if(CheckOmit(strBkName,strBkNameOld,i,m_PrtInfOmitList1))	{
								m_PrtDataBox[Field011BkNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
							}
							else	{
								// 金融 銀行名
								m_PrtDataBox[Field011BkName1].Data[nCurRow] = rc.m_BkName1;
							}
							strBkNameOld = strBkName;
							break;

						// 支店名
						case 2:
							strStName = rc.m_BkName2;
							if(CheckOmit(strStName,strStNameOld,i,m_PrtInfOmitList1))	{
								m_PrtDataBox[Field011StNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
							}
							else	{
								// 支店名
								StrDivision(rc.m_BkName2,&strArray,2);
								if(strArray.GetAt(0).IsEmpty() == FALSE && strArray.GetAt(0).IsEmpty() == FALSE)	{
									m_PrtDataBox[Field011BkName2_1Slash].Data[nCurRow] = strArray.GetAt(0);
									m_PrtDataBox[Field011BkName2_2Slash].Data[nCurRow] = strArray.GetAt(1);
								}
								else	{
									m_PrtDataBox[Field011BkName2].Data[nCurRow] = rc.m_BkName2;
								}
							}
							strStNameOld = strStName;
							break;

						// 種類
						case 3:

							if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field011KnNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								CString strDividedName1, strDividedName2;
								// 分割後の文字列取得
								cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, 6 );

								// 6文字以上なら2行表示
								if( !(strDividedName2.IsEmpty()) ){
									// 科目名が11バイトの場合、均等割り付けがずれるので半角スペースで12バイトまで埋める
									if( strDividedName1.GetLength() == 11 ){
										strDividedName1 += _T(" ");
									}
									// 半角スペースで12バイトまで埋める
									strDividedName2 = FillSpace( strDividedName2, 12 );

									m_PrtDataBox[Field011KnName1].Data[nCurRow] = strDividedName1;
									m_PrtDataBox[Field011KnName2].Data[nCurRow] = strDividedName2;
								}
								else{
									m_PrtDataBox[Field011KnName2].Data[nCurRow] = rc.m_KnName;
								}
							}

							strKnNameOld = rc.m_KnName;
							break;

						default:
							break;
						}
					}

					// 口座番号
					m_PrtDataBox[Field011AcNum].Data[nCurRow] = rc.m_AcNum;

					// 摘要
					StrDivision(rc.m_Teki,&strArray,3);
					m_PrtDataBox[Field011Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field011Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[Field011Teki3].Data[nCurRow] = strArray.GetAt( 2 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return m_nRowMax;
	}
	catch( CException* e ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc011()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
		}
		e->Delete();

		return FUNCTION_NG;
	}

	return FUNCTION_OK;
}

int CprtUc011Yotyokin::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int					nCurRow			= 0;
	CString				strBkName		= _T( "" );
	CString				strBkName2		= _T( "" );
	CString				strBkNameOld	= _T( "" );
	CString				strKnNameOld	= _T( "" );
	CStringArray		strArray;
	CString				strSQL;
	CharController		cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc011Yotyokin	rc( m_pDB );

	try{
// midori 160610 cor -->
//		if(!(po.OutZeroNull[0])){
// ---------------------
		if(!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0))	{
// midori 160610 cor <--
			// 指定されたページの行数を取得
			m_nRowMax = rc.GetNumRow( nPage );

			rc.Init( nPage );
		}
		else{
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

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
					
					strBkNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 期末現在高
				m_PrtDataBox[Field011Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){
					
					strBkNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					m_PrtDataBox[Field011KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

						// 金融機関
						case 1:

//							strBkName = rc.m_BkName1 + PRT_FORMAT_SLASH + rc.m_BkName2;
							strBkName = rc.m_BkName1 + PRT_FORMAT_CHCODE + rc.m_BkName2;

							if( CheckOmit( strBkName, strBkNameOld, i, m_PrtInfOmitList1 ) ){
								
								m_PrtDataBox[Field011BkNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{
								if( po.KinyuLine == 0 ){
									m_PrtDataBox[Field011BkName1].Data[nCurRow] = rc.m_BkName1;
									m_PrtDataBox[Field011BkName2].Data[nCurRow] =
										InsertSlashInHead( rc.m_BkName1, rc.m_BkName2 );
								}
								else{
									CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名

									// 分割後の金融機関名（銀行名）取得
									cc.Divide(rc.m_BkName1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

									// 金融機関名（銀行名）が1行？
									if( !DivBkNameDown.IsEmpty() ){
										m_PrtDataBox[Field011BkName1_1Slash].Data[nCurRow] = DivBkNameUp;
										m_PrtDataBox[Field011BkName1_2Slash].Data[nCurRow] = DivBkNameDown;
									}
									else{	// 1行の場合はスラッシュに高さをあわせる
										m_PrtDataBox[Field011BkName1Slash].Data[nCurRow] = DivBkNameUp;
									}

									// 分割後の金融機関名（支店名）取得
									cc.Divide(rc.m_BkName2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

									// 金融機関名（支店名）が1行？
									if( !DivBkNameDown.IsEmpty() ){
										m_PrtDataBox[Field011BkName2_1Slash].Data[nCurRow] = DivBkNameUp;
										m_PrtDataBox[Field011BkName2_2Slash].Data[nCurRow] = DivBkNameDown;
									}
									else{	// 1行の場合はスラッシュに高さをあわせる
										m_PrtDataBox[Field011BkName2Slash].Data[nCurRow] = DivBkNameUp;
									}

									// "/"
									if(( rc.m_BkName1.IsEmpty() == 0) || ( rc.m_BkName2.IsEmpty() == 0 )){
										m_PrtDataBox[Field011Slash].Data[nCurRow] = "/";
									}
								}
							}

							strBkNameOld = strBkName;
							break;

						// 種類
						case 2:

							if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field011KnNameOm].Data[nCurRow] =
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

									m_PrtDataBox[Field011KnName1].Data[nCurRow] = strDividedName1;
									m_PrtDataBox[Field011KnName2].Data[nCurRow] = strDividedName2;
								}
								else{
									m_PrtDataBox[Field011KnName2].Data[nCurRow] = rc.m_KnName;
								}
							}

							strKnNameOld = rc.m_KnName;
							break;

						default:
							break;
						}
					}

					// 口座番号
					m_PrtDataBox[Field011AcNum].Data[nCurRow] = rc.m_AcNum;

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 2 );
					m_PrtDataBox[Field011Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field011Teki2].Data[nCurRow] = strArray.GetAt( 1 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return m_nRowMax;
	}
	catch( CException* e ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc011()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
		}
		e->Delete();

		return FUNCTION_NG;
	}

	return FUNCTION_OK;
}
// midori 190301 add <--

CprtUc011Yotyokin::CprtUc011Yotyokin(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_011;
	m_nRowMax	= 0;
	m_nColumn	= Field011Max;
}

CprtUc011Yotyokin::~CprtUc011Yotyokin(void)
{
}

CprtJobFunction* CprtUc011Yotyokin::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc011Yotyokin(m_pDB));
}