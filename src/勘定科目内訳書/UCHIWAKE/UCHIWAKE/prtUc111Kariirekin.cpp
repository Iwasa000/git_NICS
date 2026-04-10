//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc111Kariirekin.cpp
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
#include "prtUc111Kariirekin.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

// 改良No.21-0086,21-0529 cor -->
//// midori 156188 add -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 156188 add <--
// ------------------------------
extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

// midori 157042 del -->
//// midori 190505 add -->
//extern	int		g_PrnHoho[2];
//// midori 190505 add <--
// midori 157042 del <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc111()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑪．借入金
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc111Kariirekin::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
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
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strKankeiOld	= _T( "" );
	//CString			RisokuKei[4]	={_T( "" )};
	CString			strSQL;
	CharController	cc;
// midori 154641 add -->
	int				ii=0,jj=0;
	int				pos[4];
// midori 156188_2 add -->
	int				sw=0;
// midori 156188_2 add <--
// midori 157042 add -->
	int				sw2=0;
// midori 157042 add <--
	CString			cst[4];
// midori 154641 add <--
// midori 191002 add -->
	CString			strkei=_T("");
	CString			strkei2=_T("");
// midori 191002 add <--

// midori 190505 del -->
//// midori 156188_2 add -->
//	sw = KamokuRowEnableSgn(m_pDB,0);
//// midori 156188_2 add <--
// midori 190505 del <--

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc111Kariirekin	rc( m_pDB );
// midori 190505 add -->
	CdbUcInfSub			mfcRecSub( m_pDB );
// midori 190505 add <--

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_111 );

// midori 190505 add -->
// 改良No.21-0086,21-0529 cor -->
	//sw = KamokuRowEnableSgn(m_pDB,0);
// ------------------------------
	sw = KamokuRowEnableSgn(m_pDB, 0, nFormSeq);
// 改良No.21-0086,21-0529 cor <--
// midori 157042 del -->
//	if(sw == 1) {
//// midori 20/10/08u del -->
//		//if(g_PrnHoho[1] == 2) sw = 0;
//// midori 20/10/08u del <--
//// midori 20/10/08u add -->
//		if(g_PrnHoho[1] == 1) sw = 0;
//// midori 20/10/08u add <--
//	}
//// midori 190505 add <--
// midori 157042 del <--
	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 156188_2 del -->
//////// midori 190505 del -->
////////// midori 160610 cor -->
//////////		if(po.OutZeroNull[0] || po.KamokuRow ){
////////// ---------------------
////////		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
////////// midori 160610 cor <--
//////// midori 190505 del <--
//////// midori 190505 add -->
//////		if(po.OutZeroNull[0] || po.KamokuSitei[0])	{
//////// midori 190505 add <--
////// midori 156188_2 del <--
////// midori 156188_2 add -->
////		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
////// midori 156188_2 add <--
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[0] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[0] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(nFormSeq, ck1, ck2);
//
//		if((po.OutZeroNull[0] && rt != 2) || po.KamokuRow || po.KamokuSitei[0])	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157042 del -->
//// midori 157001 add -->
//		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
//// midori 157001 add <--
// midori 157042 del <--
// midori 157042 add -->
		sw2 = 0;
		if(po.OutZeroNull[0])	sw2 = 1;
		if(po.KamokuSitei[0])	sw2 = 1;
		if(po.KamokuRow){
			if (sw == 0) {
				sw2 = 1;
			}
			else {
				if(rc.GetKmkRowSw() == 0) {
					sw2 = 1;
				}
			}
		}
// midori 157099,157119_2 del -->
//// midori 157098 add -->
//		if(po.KamokuRow == FALSE){
//			if(sw == 1) {
//				if(rc.GetKmkRowSw() == 1) {
//					sw2 = 1;
//				}
//			}
//		}
//// midori 157098 add <--
// midori 157099,157119_2 del <--

		if(sw2 == 1)	{
// midori 157042 add <--
			// 指定されたページの行数を取得（一時テーブル）
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
		else{
			// 通常のテーブルオープン
			m_nRowMax = rc.GetNumRow( nPage );

			rc.Init( nPage );
		}

		// 最終レコードまで処理を実行
		while( !rc.IsEOF() ){

			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if( nCurRow >= 0 ){

				// 空行
				if( CheckDbDataFgFuncNull( &rc ) ){
					
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strKankeiOld	= _T( "" );
					continue;
				}

				// 科目行
				if( rc.m_FgFunc == ID_FGFUNC_KAMOKU ){
					// 科目
					CString strDividedName1, strDividedName2;
					// 分割後の文字列取得
// midori 157042 del -->
//// midori 156189,156190,156191 del -->
//					//cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
//// midori 156189,156190,156191 del <--
//// midori 156189,156190,156191 add -->
//					if(sw == 1) {
//						// 改行文字で2行に分割する
//						m_clsFunc.StrDivision( rc.m_KeiStr, &strArray, 2 );
//						strDividedName1 = strArray.GetAt( 0 );
//						strDividedName2 = strArray.GetAt( 1 );
//					}
//					else {
//						cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
//					}
//// midori 156189,156190,156191 add <--
// midori 157042 del <--
// midori 157042 add -->
					if(sw == 1) {
						strDividedName1.Empty();
						strDividedName2.Empty();
						if (rc.m_KeiStr.IsEmpty() == FALSE) {
							// 改行コードが見つからない場合
							if (rc.m_KeiStr.Find(_T("\r\n")) < 0) {
								// 文字列が10文字を超えている場合は、10文字で分割
								cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, 10 );
							}
							// 改行コードが見つかった場合
							else {
								// 改行文字で2行に分割する
								m_clsFunc.StrDivision( rc.m_KeiStr, &strArray, 2 );
								strDividedName1 = strArray.GetAt( 0 );
								strDividedName2 = strArray.GetAt( 1 );
							}
						}
					}
					else {
						cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
					}
// midori 157042 add <--
					// 9文字以上なら2行表示
					if( !(strDividedName2.IsEmpty()) ){
						// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
						if( strDividedName1.GetLength() == 15 ){
							strDividedName1 += _T(" ");
						}
						// 半角スペースで16バイトまで埋める
						strDividedName2 = FillSpace( strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2 );

						m_PrtDataBox[Field111KnName1].Data[nCurRow] = strDividedName1;
						m_PrtDataBox[Field111KnName2].Data[nCurRow] = strDividedName2;
					}
					else{
// midori 156189,156190,156191 del -->
						//m_PrtDataBox[Field111KnName].Data[nCurRow] = rc.m_KnName;
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
						if(sw == 1)		m_PrtDataBox[Field111KnName].Data[nCurRow] = rc.m_KeiStr;
						else			m_PrtDataBox[Field111KnName].Data[nCurRow] = rc.m_KnName;
// midori 156189,156190,156191 add <--
					}

					rc.MoveNext();
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_Risoku	= HideZeroMoney( rc.m_Risoku );
					}
				}

				// 期末現在高
				m_PrtDataBox[Field111Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 期中の支払利子額
				m_PrtDataBox[Field111Risoku].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Risoku, &rc, MakeFormatOfNum( 10 ), BIT_D1 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strKankeiOld	= _T( "" );
					// 最終行
					if( nCurRow == (PRT_ROW_111 - 1) ){

						m_PrtDataBox[Field111DnKeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
					else{
// midori 190301 del -->
						//m_PrtDataBox[Field111KeiStr].Data[nCurRow] = rc.m_KeiStr;
// midori 190301 del <--
// midori 190301 add -->
						if(bG_Kanso == TRUE)	{
// midori 156189,156190,156191 del -->
//// midori 191002 del -->
////// midori 10 del -->
////							//m_clsFunc.StrDivisionEx(rc.m_KeiStr,&strArray,9,2);
////// midori 10 del <--
////// midori 10 add -->
////							m_clsFunc.StrDivisionEx(rc.m_KeiStr,&strArray,11,2);
////// midori 10 add <--
////							if(strArray.GetAt(1).IsEmpty() == FALSE)	{
////								m_PrtDataBox[Field111KeiStr].Data[nCurRow] = strArray.GetAt(0);
////								m_PrtDataBox[Field111KeiStr2].Data[nCurRow] = strArray.GetAt(1);
////							}
////							else	{
////								m_PrtDataBox[Field111KeiStr].Data[nCurRow] = rc.m_KeiStr;
////							}
//// midori 191002 del <--
//// midori 191002 add -->
//							m_clsFunc.KeiStrSprit(rc.m_KeiStr, &strkei, &strkei2);
//							if(strkei2.IsEmpty() == FALSE)	{
//								// 2段
//								m_PrtDataBox[Field111KeiStr].Data[nCurRow] = strkei;
//// midori 155541,155542 del -->
//								//if (strkei2.GetLength() > 20) {
//								//	m_PrtDataBox[Field111KeiStr2].Data[nCurRow] = strkei2;
//								//}
//								//else	{
//								//	m_PrtDataBox[Field111KeiStr4].Data[nCurRow] = strkei2;
//								//}
//// midori 155541,155542 del <--
//// midori 155541,155542 add -->
//								if (strkei2.GetLength() > 24) {
//									m_PrtDataBox[Field111KeiStr2].Data[nCurRow] = strkei2;
//								}
//								else if (strkei2.GetLength() > 22) {
//									m_PrtDataBox[Field111KeiStr7].Data[nCurRow] = strkei2;
//								}
//								else if (strkei2.GetLength() > 18) {
//									m_PrtDataBox[Field111KeiStr6].Data[nCurRow] = strkei2;
//								}
//								else	{
//									m_PrtDataBox[Field111KeiStr4].Data[nCurRow] = strkei2;
//								}
//// midori 155541,155542 add <--
//							}
//							else	{
//								if(rc.m_KeiStr.GetLength() > 22)		m_PrtDataBox[Field111KeiStr3].Data[nCurRow] = rc.m_KeiStr;
//								else if(rc.m_KeiStr.GetLength() > 20)	m_PrtDataBox[Field111KeiStr5].Data[nCurRow] = rc.m_KeiStr;
//								else									m_PrtDataBox[Field111KeiStr].Data[nCurRow] = rc.m_KeiStr;
//							}
//// midori 191002 add <--
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
// 157154 del -->
							//// 小計名称編集済みサインON
							//if(rc.m_ShowKeiZero & BIT_D7) {
// 157154 del <--
// 157154 add -->
							// (小計名称 または 中計名称) かつ 小計名称編集可能
							if((rc.m_FgFunc == ID_FGFUNC_SYOKEI || rc.m_FgFunc == ID_FGFUNC_CHUKEI) && sw == 1 ) {
// 157154 add <--
// 改良No.21-0086,21-0529 cor -->
								//m_clsFunc.StrDivision( rc.m_KeiStr, &strArray, 2 );
								//strkei = strArray.GetAt( 0 );
								//strkei2 = strArray.GetAt( 1 );
// ------------------------------
								// 科目指定で「小計を出力する」場合、科目名称の文字数によっては枠からはみ出してしまうため、
								// 文字数と改行マーク(\r\n)の有無を見て、分割する関数を分ける。
								int		nCh=0;
								int		nLen=0;
								nCh = rc.m_KeiStr.Find(_T("\r\n"));
								nLen = rc.m_KeiStr.GetLength();
								if(nCh < 0 && nLen > 26)	{
									m_clsFunc.KeiStrSprit(rc.m_KeiStr, &strkei, &strkei2);
								}
								else	{
									m_clsFunc.StrDivision(rc.m_KeiStr, &strArray, 2);
									strkei = strArray.GetAt(0);
									strkei2 = strArray.GetAt(1);
								}
// 改良No.21-0086,21-0529 cor <--
								// 2行出力
								if(strkei2.IsEmpty() == FALSE)	{
									// 1行目
									if (strkei.GetLength() > 24) {
										m_PrtDataBox[Field111KeiStr8].Data[nCurRow] = strkei;
									}
									else if (strkei.GetLength() > 22) {
										m_PrtDataBox[Field111KeiStr9].Data[nCurRow] = strkei;
									}
									else	{
										m_PrtDataBox[Field111KeiStr].Data[nCurRow] = strkei;
									}
									// 2行目
									if (strkei2.GetLength() > 24) {
										m_PrtDataBox[Field111KeiStr2].Data[nCurRow] = strkei2;
									}
									else if (strkei2.GetLength() > 22) {
										m_PrtDataBox[Field111KeiStr7].Data[nCurRow] = strkei2;
									}
									else	{
										m_PrtDataBox[Field111KeiStr4].Data[nCurRow] = strkei2;
									}
								}
								// 1行出力
								else	{
									if (strkei.GetLength() > 24) {
										m_PrtDataBox[Field111KeiStr8].Data[nCurRow] = strkei;
									}
									else if (strkei.GetLength() > 22) {
										m_PrtDataBox[Field111KeiStr9].Data[nCurRow] = strkei;
									}
									else if(rc.m_KeiStr.GetLength() > 20)	{
										m_PrtDataBox[Field111KeiStr5].Data[nCurRow] = strkei;
									}
									else	{
										m_PrtDataBox[Field111KeiStr].Data[nCurRow] = strkei;
									}
								}
							}
							// 小計名称編集済みサインOFF
							else {
								m_clsFunc.KeiStrSprit(rc.m_KeiStr, &strkei, &strkei2);
								if(strkei2.IsEmpty() == FALSE)	{
									// 2段
									m_PrtDataBox[Field111KeiStr].Data[nCurRow] = strkei;
									if (strkei2.GetLength() > 24) {
										m_PrtDataBox[Field111KeiStr2].Data[nCurRow] = strkei2;
									}
									else if (strkei2.GetLength() > 22) {
										m_PrtDataBox[Field111KeiStr7].Data[nCurRow] = strkei2;
									}
									else if (strkei2.GetLength() > 18) {
										m_PrtDataBox[Field111KeiStr6].Data[nCurRow] = strkei2;
									}
									else	{
										m_PrtDataBox[Field111KeiStr4].Data[nCurRow] = strkei2;
									}
								}
								else	{
									if(rc.m_KeiStr.GetLength() > 22)		m_PrtDataBox[Field111KeiStr3].Data[nCurRow] = rc.m_KeiStr;
									else if(rc.m_KeiStr.GetLength() > 20)	m_PrtDataBox[Field111KeiStr5].Data[nCurRow] = rc.m_KeiStr;
									else									m_PrtDataBox[Field111KeiStr].Data[nCurRow] = rc.m_KeiStr;
								}
							}
// midori 156189,156190,156191 add <--
						}
						else	{
							m_PrtDataBox[Field111KeiStr].Data[nCurRow] = rc.m_KeiStr;
						}
// midori 190301 add <--
					}
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 借入先（名称）
							case 1:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field111AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field111AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field111AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 借入先（所在地）
							case 2:
// midori 190301 del -->
								//strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								//if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

								//	m_PrtDataBox[Field111AdAddOm].Data[nCurRow] =
								//		m_PrtInfOmitList1[i].OmitStr;
								//}
								//else{

								//	StrDivision( strAdAdd, &strArray, 2 );
								//	m_PrtDataBox[Field111AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
								//	m_PrtDataBox[Field111AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								//}
// midori 190301 del <--
// midori 190301 add -->
								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;
								if(CheckOmit(strAdAdd,strAdAddOld,i,m_PrtInfOmitList1))	{
									m_PrtDataBox[Field111AdAddOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
								}
								else	{
									if(bG_Kanso == TRUE)	{
// midori 154641 del -->
										//// 上段
										//StrDivision(rc.m_AdAdd1,&strArray,2);
										//m_PrtDataBox[Field111AdAdd1J].Data[nCurRow]	= strArray.GetAt(0);
										//m_PrtDataBox[Field111AdAdd1].Data[nCurRow]	= strArray.GetAt(1);
										//// 下段
										//StrDivision(rc.m_AdAdd2,&strArray,2);
										//m_PrtDataBox[Field111AdAdd2J].Data[nCurRow]	= strArray.GetAt(0);
										//m_PrtDataBox[Field111AdAdd2].Data[nCurRow]	= strArray.GetAt(1);
// midori 154641 del <--
// midori 154641 add -->
										pos[0] = Field111AdAdd2;
										pos[1] = Field111AdAdd2J;
										pos[2] = Field111AdAdd1;
										pos[3] = Field111AdAdd1J;
										StrDivision(rc.m_AdAdd2,&strArray,2);
										cst[0] = strArray.GetAt(1);
										cst[1] = strArray.GetAt(0);
										StrDivision(rc.m_AdAdd1,&strArray,2);
										cst[2] = strArray.GetAt(1);
										cst[3] = strArray.GetAt(0);
										// 下から詰めて出力する
										for(ii=0,jj=0; ii<4; ii++)	{
											if(cst[ii].IsEmpty() != TRUE)	{
												m_PrtDataBox[pos[jj]].Data[nCurRow]	= cst[ii];
												jj++;
											}
										}
// midori 154641 add <--
									}
									else	{
										StrDivision(strAdAdd,&strArray,2);
										m_PrtDataBox[Field111AdAdd1].Data[nCurRow] = strArray.GetAt(0);
										m_PrtDataBox[Field111AdAdd2].Data[nCurRow] = strArray.GetAt(1);
									}
								}
// midori 190301 add <--
								strAdAddOld = strAdAdd;
								break;

							// 法人・代表者との関係
							case 3:

								if( CheckOmit( rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field111KankeiOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
// midori 154640 del -->
//// midori 190301 add -->
//									if(bG_Kanso == TRUE)	{
//										// 簡素化
//										CString		DivBkName2Up, DivBkName2Down;	// 分割後の1行目、2行目の名称
//										cc.Divide(rc.m_Kankei, DivBkName2Up, DivBkName2Down, 3);
//										if( !DivBkName2Down.IsEmpty() ){
//											m_PrtDataBox[Field111Kankei].Data[nCurRow] = DivBkName2Up;
//											m_PrtDataBox[Field111Kankei2].Data[nCurRow] = DivBkName2Down;
//										}
//										else	{
//											m_PrtDataBox[Field111Kankei2].Data[nCurRow] = rc.m_Kankei;
//										}
//									}
//									else	{
//										// これまで通り
//// midori 190301 add <--
//										m_PrtDataBox[Field111Kankei].Data[nCurRow] = rc.m_Kankei;
//// midori 190301 add -->
//									}
//// midori 190301 add -->
// midori 154640 del <--
// midori 154640 add -->
									m_PrtDataBox[Field111Kankei].Data[nCurRow] = rc.m_Kankei;
// midori 154640 add <--
								}

								strKankeiOld = rc.m_Kankei;
								break;

							default:
								break;
						}
					}

					// 利率
					m_PrtDataBox[Field111Rate].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Rate, &rc, MakeFormatOfNum( 3, 3 ) );

					// 借入理由
// midori 190301 add -->
					if(bG_Kanso == FALSE)	{
// midori 190301 add <--
						StrDivision( rc.m_Riyuu, &strArray, 2 );
						m_PrtDataBox[Field111Riyuu1].Data[nCurRow] = strArray.GetAt( 0 );
						m_PrtDataBox[Field111Riyuu2].Data[nCurRow] = strArray.GetAt( 1 );
// midori 190301 add -->
					}
// midori 190301 add <--

					// 担保の内容
// midori 190301 del -->
					//StrDivision( rc.m_Naiyou, &strArray, 2 );
					//m_PrtDataBox[Field111Naiyou1].Data[nCurRow] = strArray.GetAt( 0 );
					//m_PrtDataBox[Field111Naiyou2].Data[nCurRow] = strArray.GetAt( 1 );
// midori 190301 del <--
// midori 190301 add -->
					if(bG_Kanso == TRUE)	{
						StrDivision(rc.m_Naiyou,&strArray,3);
						m_PrtDataBox[Field111Naiyou0].Data[nCurRow] = strArray.GetAt(0);
						m_PrtDataBox[Field111Naiyou1].Data[nCurRow] = strArray.GetAt(1);
						m_PrtDataBox[Field111Naiyou2].Data[nCurRow] = strArray.GetAt(2);
					}
					else	{
						StrDivision(rc.m_Naiyou,&strArray,2);
						m_PrtDataBox[Field111Naiyou1].Data[nCurRow] = strArray.GetAt(0);
						m_PrtDataBox[Field111Naiyou2].Data[nCurRow] = strArray.GetAt(1);
					}
// midori 190301 add <--
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc111()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc111Kariirekin::CprtUc111Kariirekin(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_111;
	m_nRowMax	= 0;
	m_nColumn	= Field111Max;
}

CprtUc111Kariirekin::~CprtUc111Kariirekin(void)
{
}

CprtJobFunction* CprtUc111Kariirekin::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc111Kariirekin(m_pDB));
}