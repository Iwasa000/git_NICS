//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc142Kasituke.cpp
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
//#include ".\prtJobFunction.h"
#include "prtUc042Kasituke.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--
extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/25 add

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc042()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ④-2．貸付金及び受取利息
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc042Kasituke::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;

	if(bG_InvNo == TRUE) {
		rv = GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} else {
		rv = GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

int CprtUc042Kasituke::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow = 0;
	int				nLastRow = PRT_ROW_042_KOBETU;
	int				index = 0;
	CStringArray	strArray;
	CString			strAdName = _T("");
	CString			strAdAdd = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAddOld = _T("");
	CString			strKankeiOld = _T("");
	CString			strSQL;
	CString				strAdAdd1 = _T("");
	CString				strAdAdd2 = _T("");
	CString				strAdAdd3 = _T("");
	CUcFunctionCommon	clsFunc;
	int				ii = 0, jj = 0;
	int				pos[4];
	CString			cst[4];

	// 省略文字の取得
	GetPrintInfo(parent);

	CdbUc042Kasituke	rc(m_pDB);

	// 計の出力設定
	int nOutKei = rc.GetOutKei(ID_FORMNO_042);

	try {
		if (!(po.OutZeroNull[0])) {
			// 指定されたページの行数を取得
			m_nRowMax = rc.GetNumRow(nPage);

			rc.Init(nPage);
		}
		else {
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl(m_nFormSeq);

#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage);
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage);
#endif
			rc.Open(CRecordset::forwardOnly, strSQL);
		}


		// 最終レコードまで処理を実行
		while (!rc.IsEOF()) {

			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if (nCurRow >= 0) {

				// 空行
				if (CheckDbDataFgFuncNull(&rc)) {

					strAdNameOld = _T("");
					strAdAddOld = _T("");
					strKankeiOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[0]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
						rc.m_Risoku = HideZeroMoney(rc.m_Risoku);
					}
				}

				// 期末現在高
				m_PrtDataBox[index + Field042IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);
				// 期中の受取利息額
				m_PrtDataBox[index + Field042IRisoku].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Risoku, &rc, MakeFormatOfNum(10), BIT_D1);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strAdNameOld = _T("");
					strAdAddOld = _T("");
					strKankeiOld = _T("");
					// 最終行
					if (nCurRow == (nLastRow - 1)) {

						m_PrtDataBox[index + Field042IDnKeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
					else {

						m_PrtDataBox[index + Field042IKeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[index + Field042IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList2[i].ItemSeq;

						switch (nItemSeq) {

							// 貸付先（名称）
						case 1:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field042IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[index + Field042IAdName1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field042IAdName2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 貸付先（所在地）
						case 2:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field042IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {
								if (bG_Kanso == TRUE) {
									pos[0] = index + Field042IAdAdd2;
									pos[1] = index + Field042IAdAdd2J;
									pos[2] = index + Field042IAdAdd1;
									pos[3] = index + Field042IAdAdd1J;
									StrDivision(rc.m_AdAdd2, &strArray, 2);
									cst[0] = strArray.GetAt(1);
									cst[1] = strArray.GetAt(0);
									StrDivision(rc.m_AdAdd1, &strArray, 2);
									cst[2] = strArray.GetAt(1);
									cst[3] = strArray.GetAt(0);
									// 下から詰めて出力する
									for (ii = 0, jj = 0; ii < 4; ii++) {
										if (cst[ii].IsEmpty() != TRUE) {
											m_PrtDataBox[pos[jj]].Data[nCurRow] = cst[ii];
											jj++;
										}
									}
								}
								else {
									// これまで通り
									StrDivision(strAdAdd, &strArray, 2);
									m_PrtDataBox[index + Field042IAdAdd1].Data[nCurRow] =
										strArray.GetAt(0);
									m_PrtDataBox[index + Field042IAdAdd2].Data[nCurRow] =
										strArray.GetAt(1);
								}
							}

							strAdAddOld = strAdAdd;
							break;

							// 貸付先（代表者との関係）
						case 3:

							if (CheckOmit(rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field042IKankeiOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {
								m_PrtDataBox[index + Field042IKankei].Data[nCurRow] = rc.m_Kankei;
							}

							strKankeiOld = rc.m_Kankei;
							break;

						default:
							break;
						}
					}

					// 利率
					m_PrtDataBox[index + Field042IRate].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Rate, &rc, MakeFormatOfNum(2, 3));

					// 貸付理由
					if (bG_Kanso == FALSE) {
						StrDivision(rc.m_Riyuu, &strArray, 2);
						m_PrtDataBox[index + Field042IRiyuu1].Data[nCurRow] = strArray.GetAt(0);
						m_PrtDataBox[index + Field042IRiyuu2].Data[nCurRow] = strArray.GetAt(1);
					}

					// 担保の内容
					StrDivision(rc.m_Naiyou, &strArray, 2);
					m_PrtDataBox[index + Field042INaiyou1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[index + Field042INaiyou2].Data[nCurRow] = strArray.GetAt(1);
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return m_nRowMax;
	}
	catch (...) {

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc042()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/10/25 add <--

// インボイス登録番号追加対応_23/10/25 del -->
//int CprtUc042Kasituke::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc042Kasituke::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 add <--
{
	int				nCurRow			= 0;
	int				nLastRow		= PRT_ROW_042_KOBETU;
	int				index			= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strAdName		= _T ( "" );
	CString			strAdAdd		= _T ( "" );
	CString			strAdNameOld	= _T ( "" );
	CString			strAdAddOld		= _T ( "" );
	CString			strKankeiOld	= _T ( "" );
	//CString			RisokuKei[4]	= {_T ( "" )};
	CString			strSQL;
// midori 190301 add -->
	CString				strAdAdd1		= _T ( "" );
	CString				strAdAdd2		= _T ( "" );
	CString				strAdAdd3		= _T ( "" );
	CUcFunctionCommon	clsFunc;
// midori 190301 add <--
// midori 154641 add -->
	int				ii=0,jj=0;
	int				pos[4];
	CString			cst[4];
// midori 154641 add <--

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc042Kasituke	rc( m_pDB );

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_042 );

	try{
// midori 157001 del -->
//// midori 190505 del -->
//		//if(!(po.OutZeroNull[0])){
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
					
					strAdNameOld	= _T ( "" );
					strAdAddOld		= _T ( "" );
					strKankeiOld	= _T ( "" );
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
				m_PrtDataBox[index + Field042Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				// 期中の受取利息額
				m_PrtDataBox[index + Field042Risoku].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Risoku, &rc, MakeFormatOfNum( 10 ), BIT_D1 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strAdNameOld	= _T ( "" );
					strAdAddOld		= _T ( "" );
					strKankeiOld	= _T ( "" );
					// 最終行
					if( nCurRow == (nLastRow - 1) ){

						m_PrtDataBox[index + Field042DnKeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
					else{
						
						m_PrtDataBox[index + Field042KeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList2[i].ItemSeq;

						switch( nItemSeq ){

							// 貸付先（名称）
							case 1:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field042AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{
									
									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[index + Field042AdName1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field042AdName2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 貸付先（所在地）
							case 2:
								
								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field042AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{
// midori 190301 add -->
									if(bG_Kanso == TRUE)	{
// midori 154641 del -->
										//// 上段
										//StrDivision(rc.m_AdAdd1,&strArray,2);
										//m_PrtDataBox[index + Field042AdAdd1J].Data[nCurRow]	= strArray.GetAt(0);
										//m_PrtDataBox[index + Field042AdAdd1].Data[nCurRow]	= strArray.GetAt(1);
										//// 下段
										//StrDivision(rc.m_AdAdd2,&strArray,2);
										//m_PrtDataBox[index + Field042AdAdd2J].Data[nCurRow]	= strArray.GetAt(0);
										//m_PrtDataBox[index + Field042AdAdd2].Data[nCurRow]	= strArray.GetAt(1);
// midori 154641 del <--
// midori 154641 add -->
										pos[0] = index + Field042AdAdd2;
										pos[1] = index + Field042AdAdd2J;
										pos[2] = index + Field042AdAdd1;
										pos[3] = index + Field042AdAdd1J;
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
										// これまで通り
// midori 190301 add <--
										StrDivision( strAdAdd, &strArray, 2 );
										m_PrtDataBox[index + Field042AdAdd1].Data[nCurRow] =
											strArray.GetAt( 0 );
										m_PrtDataBox[index + Field042AdAdd2].Data[nCurRow] =
											strArray.GetAt( 1 );
// midori 190301 add -->
									}
// midori 190301 add <--
								}

								strAdAddOld = strAdAdd;
								break;

							// 貸付先（代表者との関係）
							case 3:
								
								if( CheckOmit( rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field042KankeiOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{
// midori 154640 del -->
//// midori 190301 add -->
//									if(bG_Kanso == TRUE)	{
//										// 簡素化
//										CString			strKankei1, strKankei2;
//										CharController	cc;
//										// 分割後の文字列取得
//										cc.Divide(rc.m_Kankei, strKankei1, strKankei2, 3);
//										// 3文字以上なら2行表示
//										if( !(strKankei2.IsEmpty()) ){
//											m_PrtDataBox[index + Field042Kankei].Data[nCurRow]  = strKankei1;
//											m_PrtDataBox[index + Field042Kankei2].Data[nCurRow] = strKankei2;
//										}
//										else	{
//											m_PrtDataBox[index + Field042Kankei2].Data[nCurRow] = strKankei1;
//										}
//									}
//									else	{
//										// これまで通り
//// midori 190301 add <--
//										m_PrtDataBox[index + Field042Kankei].Data[nCurRow] =
//											rc.m_Kankei;
//// midori 190301 add -->
//									}
//// midori 190301 add <--
// midori 154640 del <--
// midori 154640 add -->
									m_PrtDataBox[index + Field042Kankei].Data[nCurRow] = rc.m_Kankei;
// midori 154640 add <--
								}

								strKankeiOld = rc.m_Kankei;
								break;

							default:
								break;
						}
					}

					// 利率
					m_PrtDataBox[index + Field042Rate].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Rate, &rc, MakeFormatOfNum( 2, 3 ) );

					// 貸付理由
// midori 190301 add -->
					if(bG_Kanso == FALSE)	{
// midori 190301 add <--
						StrDivision( rc.m_Riyuu, &strArray, 2 );
						m_PrtDataBox[index + Field042Riyuu1].Data[nCurRow] = strArray.GetAt( 0 );
						m_PrtDataBox[index + Field042Riyuu2].Data[nCurRow] = strArray.GetAt( 1 );
// midori 190301 add -->
					}
// midori 190301 add <--

					// 担保の内容
					StrDivision( rc.m_Naiyou, &strArray, 2 );
					m_PrtDataBox[index + Field042Naiyou1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[index + Field042Naiyou2].Data[nCurRow] = strArray.GetAt( 1 );
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc042()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc042Kasituke::CprtUc042Kasituke(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_042;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/25 del -->
	//m_nColumn	= Field042Max;
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field042IMax;
	else					m_nColumn	= Field042Max;
// インボイス登録番号追加対応_23/10/25 add <--
}

CprtUc042Kasituke::~CprtUc042Kasituke(void)
{
}

CprtJobFunction* CprtUc042Kasituke::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc042Kasituke(m_pDB));
}