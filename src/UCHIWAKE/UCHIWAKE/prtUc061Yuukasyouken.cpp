//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc061Yuukasyouken.cpp
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
#include "prtUc061Yuukasyouken.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc061()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑥．有価証券
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// midori 190301 del -->
//int CprtUc061Yuukasyouken::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
//{
//	int				nCurRow			= 0;
//	CStringArray	strArray;
//	CString			strAdName		= _T( "" );
//	CString			strAdAdd		= _T( "" );
//	CString			strKnNameOld	= _T( "" );
//	CString			strSyuruiOld	= _T( "" );
//	CString			strRiyuuOld		= _T( "" );
//	CString			strAdNameOld	= _T( "" );
//	CString			strAdAddOld		= _T( "" );
//	CString			strSQL;
//	CharController	cc;
//
//	// 省略文字の取得
//	GetPrintInfo( parent );
//
//	CdbUc061Yuukasyouken	rc( m_pDB );
//
//	// 計の出力設定
//	int nOutKei = rc.GetOutKei( ID_FORMNO_061 );
//
//	try{
//		if(!(po.OutZeroNull[0])){
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
//					//					strKubunNameOld	= _T( "" );
//					strKnNameOld	= _T( "" );
//					strSyuruiOld	= _T( "" );
//					strRiyuuOld		= _T( "" );
//					strAdNameOld	= _T( "" );
//					strAdAddOld		= _T( "" );
//					continue;
//				}
//
//				// データ行の０円非表示
//				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
//					if(po.HideZero[0]){
//						rc.m_Val		= HideZeroMoney( rc.m_Val );
//						rc.m_Val2		= HideZeroMoney( rc.m_Val2 );
//						rc.m_Val3		= HideZeroMoney( rc.m_Val3 );
//					}
//				}
//
//				// 期末現在高（金額）　下段
//				m_PrtDataBox[Field061Val2].Data[nCurRow] =
//					ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
//				// 期中増の明細（金額）
//				m_PrtDataBox[Field061Val3].Data[nCurRow] =
//					ShowFieldOfVal( rc.m_Val3, &rc, MakeFormatOfNum( 12 ), BIT_D1 );
//
//				// 計行
//				if(	CheckDbDataFgFuncKei( &rc ) ){
//
////					strKubunNameOld	= _T( "" );
//					strKnNameOld	= _T( "" );
//					strSyuruiOld	= _T( "" );
//					strRiyuuOld		= _T( "" );
//					strAdNameOld	= _T( "" );
//					strAdAddOld		= _T( "" );
//
//					m_PrtDataBox[Field061KeiStr].Data[nCurRow] = rc.m_KeiStr;
//				}
//				else{
//					// 省略文字使用欄の判別
//					for( int i = 0; i < PRT_MAX_OMIT; i++ ){
//
//						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;
//
//						switch( nItemSeq ){
//
//							// 区分の文言
//							case 1:
//
////								if( CheckOmit( rc.m_KubunName, strKubunNameOld, i, m_PrtInfOmitList1 ) ){
//								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){
//
////									m_PrtDataBox[Field061KubunNameOm].Data[nCurRow] =
//									m_PrtDataBox[Field061KnNameOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//
//									// m_PrtDataBox[Field061KubunName].Data[nCurRow] = rc.m_KubunName;
//									CString strDividedName1, strDividedName2;
//									// 分割後の文字列取得
//									cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
//									// 9文字以上なら2行表示
//									if( !(strDividedName2.IsEmpty()) ){
//										// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
//										if( strDividedName1.GetLength() == 15 ){
//											strDividedName1 += _T(" ");
//										}
//										// 半角スペースで16バイトまで埋める
//										strDividedName2 = FillSpace( strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2 );
//
//										m_PrtDataBox[Field061KnName1].Data[nCurRow] = strDividedName1;
//										m_PrtDataBox[Field061KnName2].Data[nCurRow] = strDividedName2;
//									}
//									else{
//										m_PrtDataBox[Field061KnName2].Data[nCurRow] = rc.m_KnName;
//									}
//								}
//
////								strKubunNameOld = rc.m_KubunName;
//								strKnNameOld = rc.m_KnName;
//								break;
//
//							// 種類・銘柄
//							case 2:
//
//								if( CheckOmit( rc.m_Syurui, strSyuruiOld, i, m_PrtInfOmitList1 ) ){
//
//									m_PrtDataBox[Field061SyuruiOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//
//									StrDivision( rc.m_Syurui, &strArray, 2 );
//									m_PrtDataBox[Field061Syurui1].Data[nCurRow] = strArray.GetAt( 0 );
//									m_PrtDataBox[Field061Syurui2].Data[nCurRow] = strArray.GetAt( 1 );
//								}
//
//								strSyuruiOld = rc.m_Syurui;
//								break;
//
//							// 期中増の明細（異動事由）
//							case 3:
//
//								if( CheckOmit( rc.m_Riyuu, strRiyuuOld, i, m_PrtInfOmitList1 ) ){
//
//									m_PrtDataBox[Field061RiyuuOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//
//									StrDivision( rc.m_Riyuu, &strArray, 2 );
//									m_PrtDataBox[Field061Riyuu1].Data[nCurRow] = strArray.GetAt( 0 );
//									m_PrtDataBox[Field061Riyuu2].Data[nCurRow] = strArray.GetAt( 1 );
//								}
//
//								strRiyuuOld = rc.m_Riyuu;
//								break;
//
//							// 売却先（名称）
//							case 4:
//								
//								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;
//
//								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){
//
//									m_PrtDataBox[Field061AdNameOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//
//									StrDivision( strAdName, &strArray, 2 );
//									m_PrtDataBox[Field061AdName1].Data[nCurRow] = strArray.GetAt( 0 );
//									m_PrtDataBox[Field061AdName2].Data[nCurRow] = strArray.GetAt( 1 );
//								}
//
//								strAdNameOld = strAdName;
//								break;
//
//							// 売却先（所在地）
//							case 5:
//								
//								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;
//
//								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){
//
//									m_PrtDataBox[Field061AdAddOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//
//									StrDivision( strAdAdd, &strArray, 2 );
//									m_PrtDataBox[Field061AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
//									m_PrtDataBox[Field061AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
//								}
//
//								strAdAddOld = strAdAdd;
//								break;
//
//							default:
//								break;
//						}
//					}
//
//					// 期末現在高（数量）
//					// 少数点以下の000をカット　+　整数部分が9桁の場合、6桁目のカンマをカット
//					m_PrtDataBox[Field061Suuryou].Data[nCurRow] =
//						RemoveComma( FixUnderDotZero( ShowFieldOfVal( rc.m_Suuryou, &rc, MakeFormatOfNum( 9, 3 ) ) ) );
//
//					// 期末現在高（単位）
//					m_PrtDataBox[Field061Tanni].Data[nCurRow] = ShowFieldOfVal( rc.m_Tanni, &rc );
//
//					// 期末現在高（金額）　上段
//					m_PrtDataBox[Field061Val].Data[nCurRow] =
//						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ) );
//
//					// 期中増の明細（異動年月日）
//					m_PrtDataBox[Field061SpDate].Data[nCurRow] =
//						ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
//
//					// 期中増の明細（数量）
//					m_PrtDataBox[Field061Suuryou2].Data[nCurRow] =
//						FixUnderDotZero( ShowFieldOfVal( rc.m_Suuryou2, &rc, MakeFormatOfNum( 9/*8*/, 3 ) ) );
//
//					// 期中増の明細（単位）
//					m_PrtDataBox[Field061Tanni2].Data[nCurRow] = ShowFieldOfVal( rc.m_Tanni2, &rc );
//
//					// 摘要
//					StrDivision( rc.m_Teki, &strArray, 3 );
//					m_PrtDataBox[Field061Teki1].Data[nCurRow] = strArray.GetAt( 0 );
//					m_PrtDataBox[Field061Teki2].Data[nCurRow] = strArray.GetAt( 1 );
//					m_PrtDataBox[Field061Teki3].Data[nCurRow] = strArray.GetAt( 2 );
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
//	catch( ... ){
//
//		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc061()\n" );
//
//		if( rc.IsOpen() ){
//
//			rc.Fin();
//			return FUNCTION_NG;
//		}
//	}
//
//	return FUNCTION_OK;
//}
// midori 190301 del <--
// midori 190301 add -->
int CprtUc061Yuukasyouken::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int	rv=0;

	if(bG_Kanso == TRUE)	rv = GetDbDataSheetNew(nPage,parent,po,nFormSeq);
	else					rv = GetDbDataSheetOld(nPage,parent,po,nFormSeq);
	return(rv);
}

int CprtUc061Yuukasyouken::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow			= 0;
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
	CString			strSyuruiOld	= _T( "" );
	CString			strMeigaraOld	= _T( "" );
	CString			strRiyuuOld		= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;
	CharController	cc;
	CUcFunctionCommon	ufc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc061Yuukasyouken	rc( m_pDB );

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_061 );

	try{
// midori 157001 del -->
//// midori 190505 del -->
////		if(!(po.OutZeroNull[0])){
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[0] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[0] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(nFormSeq, ck1, ck2);
//
//		if((!(po.OutZeroNull[0])) || (po.OutZeroNull[0] != 0 && rt == 2))	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if(!(po.OutZeroNull[0])){
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

					//					strKubunNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					strSyuruiOld	= _T( "" );
					strMeigaraOld	= _T( "" );
					strRiyuuOld		= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val		= HideZeroMoney( rc.m_Val );
						rc.m_Val2		= HideZeroMoney( rc.m_Val2 );
						rc.m_Val3		= HideZeroMoney( rc.m_Val3 );
					}
				}

				// 期末現在高（金額）　下段
				m_PrtDataBox[Field061Val2].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				// 期中増の明細（金額）
				m_PrtDataBox[Field061Val3].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val3, &rc, MakeFormatOfNum( 12 ), BIT_D1 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

//					strKubunNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					strSyuruiOld	= _T( "" );
					strMeigaraOld	= _T( "" );
					strRiyuuOld		= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					if((rc.m_FgFunc == ID_FGFUNC_SYOKEI) || (rc.m_FgFunc == ID_FGFUNC_CHUKEI))	{
						m_PrtDataBox[Field061KeiStr2].Data[nCurRow] = rc.m_KeiStr;
					}
					else if(rc.m_NumRow < ID_ROWNORMAL_061)	{
						m_PrtDataBox[Field061KeiStr3].Data[nCurRow] = rc.m_KeiStr;
					}
					else	{
						m_PrtDataBox[Field061KeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 区分の文言
							case 1:

//								if( CheckOmit( rc.m_KubunName, strKubunNameOld, i, m_PrtInfOmitList1 ) ){
								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

//									m_PrtDataBox[Field061KubunNameOm].Data[nCurRow] =
									m_PrtDataBox[Field061KnNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									// m_PrtDataBox[Field061KubunName].Data[nCurRow] = rc.m_KubunName;
									CString strDividedName1, strDividedName2;
									// 分割後の文字列取得
									cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
									// 9文字以上なら2行表示
									if( !(strDividedName2.IsEmpty()) ){
										// === 簡素化 ===
										strDividedName1 = strDividedName1 + strDividedName2;

										m_PrtDataBox[Field061KnName1].Data[nCurRow] = strDividedName1;
									}
									else{
										m_PrtDataBox[Field061KnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

//								strKubunNameOld = rc.m_KubunName;
								strKnNameOld = rc.m_KnName;
								break;

							// 種類
							case 2:
								// [5]番目=FALSE … １行目(種類)が空白でも詰めない
								ufc.StrDivision(rc.m_Syurui,&strArray,2,TRUE,FALSE);
								if(CheckOmit(strArray.GetAt(0),strSyuruiOld,i,m_PrtInfOmitList1))	{
									m_PrtDataBox[Field061SyuruiOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
								}
								else	{
									// [5]番目=FALSE … １行目(種類)が空白でも詰めない
									m_PrtDataBox[Field061Syurui1].Data[nCurRow] = strArray.GetAt(0);
								}
								strSyuruiOld = strArray.GetAt(0);
								break;

							// 銘柄
							case 3:
								// [5]番目=FALSE … １行目(種類)が空白でも詰めない
								ufc.StrDivision(rc.m_Syurui,&strArray,2,TRUE,FALSE);
								if(CheckOmit(strArray.GetAt(1),strMeigaraOld,i,m_PrtInfOmitList1))	{
									m_PrtDataBox[Field061MeigaraOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
								}
								else	{
									m_PrtDataBox[Field061Syurui2].Data[nCurRow] = strArray.GetAt(1);
								}
								strMeigaraOld = strArray.GetAt(1);
								break;

							// 期中増の明細（異動事由）
							case 4:

								if( CheckOmit( rc.m_Riyuu, strRiyuuOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field061RiyuuOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( rc.m_Riyuu, &strArray, 2 );
									m_PrtDataBox[Field061Riyuu1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field061Riyuu2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strRiyuuOld = rc.m_Riyuu;
								break;

							// 売却先（名称）
							case 5:
								
								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field061AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field061AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field061AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 売却先（所在地）
							case 6:
								
								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field061AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field061AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field061AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}

					// 期末現在高（数量）
					// 少数点以下の000をカット　+　整数部分が9桁の場合、6桁目のカンマをカット
					m_PrtDataBox[Field061Suuryou].Data[nCurRow] =
						RemoveComma( FixUnderDotZero( ShowFieldOfVal( rc.m_Suuryou, &rc, MakeFormatOfNum( 9, 3 ) ) ) );

					// 期末現在高（単位）
					m_PrtDataBox[Field061Tanni].Data[nCurRow] = ShowFieldOfVal( rc.m_Tanni, &rc );

					// 期末現在高（金額）　上段
					m_PrtDataBox[Field061Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ) );

					// 期中増の明細（異動年月日）
// midori 200101 del -->
					//m_PrtDataBox[Field061SpDate].Data[nCurRow] =
					//	ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
// midori 200101 del <--
// midori 200101 add -->
					if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
						m_PrtDataBox[Field061SpDate].Data[nCurRow] =
							ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E2 );
					}
					else	{
						m_PrtDataBox[Field061SpDate].Data[nCurRow] =
							ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
					}
// midori 200101 add <--

					// 期中増の明細（数量）
					m_PrtDataBox[Field061Suuryou2].Data[nCurRow] =
						FixUnderDotZero( ShowFieldOfVal( rc.m_Suuryou2, &rc, MakeFormatOfNum( 9/*8*/, 3 ) ) );

					// 期中増の明細（単位）
					m_PrtDataBox[Field061Tanni2].Data[nCurRow] = ShowFieldOfVal( rc.m_Tanni2, &rc );

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 3 );
					m_PrtDataBox[Field061Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field061Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[Field061Teki3].Data[nCurRow] = strArray.GetAt( 2 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc061()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

int CprtUc061Yuukasyouken::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow			= 0;
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strAdAdd		= _T( "" );
	CString			strKnNameOld	= _T( "" );
	CString			strSyuruiOld	= _T( "" );
	CString			strRiyuuOld		= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;
	CharController	cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc061Yuukasyouken	rc( m_pDB );

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_061 );

	try{
		if(!(po.OutZeroNull[0])){
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

					//					strKubunNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					strSyuruiOld	= _T( "" );
					strRiyuuOld		= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val		= HideZeroMoney( rc.m_Val );
						rc.m_Val2		= HideZeroMoney( rc.m_Val2 );
						rc.m_Val3		= HideZeroMoney( rc.m_Val3 );
					}
				}

				// 期末現在高（金額）　下段
				m_PrtDataBox[Field061Val2].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				// 期中増の明細（金額）
				m_PrtDataBox[Field061Val3].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val3, &rc, MakeFormatOfNum( 12 ), BIT_D1 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

//					strKubunNameOld	= _T( "" );
					strKnNameOld	= _T( "" );
					strSyuruiOld	= _T( "" );
					strRiyuuOld		= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[Field061KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 区分の文言
							case 1:

//								if( CheckOmit( rc.m_KubunName, strKubunNameOld, i, m_PrtInfOmitList1 ) ){
								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

//									m_PrtDataBox[Field061KubunNameOm].Data[nCurRow] =
									m_PrtDataBox[Field061KnNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									// m_PrtDataBox[Field061KubunName].Data[nCurRow] = rc.m_KubunName;
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

										m_PrtDataBox[Field061KnName1].Data[nCurRow] = strDividedName1;
										m_PrtDataBox[Field061KnName2].Data[nCurRow] = strDividedName2;
									}
									else{
										m_PrtDataBox[Field061KnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

//								strKubunNameOld = rc.m_KubunName;
								strKnNameOld = rc.m_KnName;
								break;

							// 種類・銘柄
							case 2:

								if( CheckOmit( rc.m_Syurui, strSyuruiOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field061SyuruiOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( rc.m_Syurui, &strArray, 2 );
									m_PrtDataBox[Field061Syurui1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field061Syurui2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strSyuruiOld = rc.m_Syurui;
								break;

							// 期中増の明細（異動事由）
							case 3:

								if( CheckOmit( rc.m_Riyuu, strRiyuuOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field061RiyuuOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( rc.m_Riyuu, &strArray, 2 );
									m_PrtDataBox[Field061Riyuu1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field061Riyuu2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strRiyuuOld = rc.m_Riyuu;
								break;

							// 売却先（名称）
							case 4:
								
								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field061AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field061AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field061AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 売却先（所在地）
							case 5:
								
								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field061AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field061AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field061AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}

					// 期末現在高（数量）
					// 少数点以下の000をカット　+　整数部分が9桁の場合、6桁目のカンマをカット
					m_PrtDataBox[Field061Suuryou].Data[nCurRow] =
						RemoveComma( FixUnderDotZero( ShowFieldOfVal( rc.m_Suuryou, &rc, MakeFormatOfNum( 9, 3 ) ) ) );

					// 期末現在高（単位）
					m_PrtDataBox[Field061Tanni].Data[nCurRow] = ShowFieldOfVal( rc.m_Tanni, &rc );

					// 期末現在高（金額）　上段
					m_PrtDataBox[Field061Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ) );

					// 期中増の明細（異動年月日）
					m_PrtDataBox[Field061SpDate].Data[nCurRow] =
						ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );

					// 期中増の明細（数量）
					m_PrtDataBox[Field061Suuryou2].Data[nCurRow] =
						FixUnderDotZero( ShowFieldOfVal( rc.m_Suuryou2, &rc, MakeFormatOfNum( 9/*8*/, 3 ) ) );

					// 期中増の明細（単位）
					m_PrtDataBox[Field061Tanni2].Data[nCurRow] = ShowFieldOfVal( rc.m_Tanni2, &rc );

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 3 );
					m_PrtDataBox[Field061Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field061Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[Field061Teki3].Data[nCurRow] = strArray.GetAt( 2 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc061()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// midori 190301 add <--

CprtUc061Yuukasyouken::CprtUc061Yuukasyouken(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_061;
	m_nRowMax	= 0;
	m_nColumn	= Field061Max;
}

CprtUc061Yuukasyouken::~CprtUc061Yuukasyouken(void)
{
}

CprtJobFunction* CprtUc061Yuukasyouken::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc061Yuukasyouken(m_pDB));
}