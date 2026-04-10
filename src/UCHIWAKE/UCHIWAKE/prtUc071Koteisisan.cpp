//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc071Keteisisan.cpp
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
#include "prtUc071Koteisisan.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--

extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/23 add

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc071()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑦．固定資産
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/10/23 add -->
int CprtUc071Koteisisan::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;

	if(bG_InvNo == TRUE) {
		GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} else {
		GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

// ---------------------------------------------------------
// インボイス登録番号追加版
// ---------------------------------------------------------
int CprtUc071Koteisisan::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow = 0;
	CStringArray	strArray;
	CString			strAd2Add = _T("");
	CString			strSyuruiOld = _T("");
	CString			strAd2AddOld = _T("");
	CString			strRiyuuOld = _T("");
	CString			strAdName = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAdd = _T("");
	CString			strAdAddOld = _T("");
	CString			strSQL;

	// 省略文字の取得
	GetPrintInfo(parent);

	CdbUc071Koteishisan	rc(m_pDB);

	// 計の出力設定
	int nOutKei = rc.GetOutKei(ID_FORMNO_071);

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

					strSyuruiOld = _T("");
					strAd2AddOld = _T("");
					strRiyuuOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[0]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
						rc.m_MsVal = HideZeroMoney(rc.m_MsVal);
						rc.m_MsVal2 = HideZeroMoney(rc.m_MsVal2);
					}
				}

				// 期末現在高
				m_PrtDataBox[Field071IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(13), BIT_D0);

				// 取得価額
				m_PrtDataBox[Field071IMsVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_MsVal, &rc, MakeFormatOfNum(12), BIT_D1);

				// 異動直前の帳簿価額
				m_PrtDataBox[Field071IMsVal2].Data[nCurRow] =
					ShowFieldOfVal(rc.m_MsVal2, &rc, MakeFormatOfNum(12), BIT_D2);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strSyuruiOld = _T("");
					strAd2AddOld = _T("");
					strRiyuuOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");

					m_PrtDataBox[Field071IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field071IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch (nItemSeq) {

							// 種類・構造
						case 1:

							if (CheckOmit(rc.m_Syurui, strSyuruiOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field071ISyuruiOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(rc.m_Syurui, &strArray, 2);
								m_PrtDataBox[Field071ISyurui1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field071ISyurui2].Data[nCurRow] = strArray.GetAt(1);
							}

							strSyuruiOld = rc.m_Syurui;
							break;

							// 物件の所在地
						case 2:

							strAd2Add = rc.m_Ad2Add1 + PRT_FORMAT_CHCODE + rc.m_Ad2Add2;

							if (CheckOmit(strAd2Add, strAd2AddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field071IAd2AddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAd2Add, &strArray, 2);
								m_PrtDataBox[Field071IAd2Add1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field071IAd2Add2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAd2AddOld = strAd2Add;
							break;

							// 異動事由
						case 3:

							if (CheckOmit(rc.m_Riyuu, strRiyuuOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field071IRiyuu1Om].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(rc.m_Riyuu, &strArray, 2);
								m_PrtDataBox[Field071IRiyuu1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field071IRiyuu2].Data[nCurRow] = strArray.GetAt(1);
							}

							strRiyuuOld = rc.m_Riyuu;
							break;

							// 売却先（名称）
						case 4:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field071IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[Field071IAdName1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field071IAdName2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 売却先（所在地）
						case 5:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field071IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[Field071IAdAdd1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field071IAdAdd2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}

					// 用途
					StrDivision(rc.m_Youto, &strArray, 2);
					m_PrtDataBox[Field071IYouto1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[Field071IYouto2].Data[nCurRow] = strArray.GetAt(1);

					// 面積
					m_PrtDataBox[Field071IArea].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Area, &rc, MakeFormatOfNum(8, 2));

					// 異動年月日の元号
					if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
						m_PrtDataBox[Field071ISpGengo].Data[nCurRow] =
							ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_G1, 1);
					}

					// 異動年月日
					m_PrtDataBox[Field071ISpDate].Data[nCurRow] =
						ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_B1);		//_T( "%2d.%2d.%2d" )

					// 売却物件の取得年月の元号
					m_PrtDataBox[Field071ISpDate21].Data[nCurRow] =
						ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_G1, 1);

					// 売却物件の取得年月
					m_PrtDataBox[Field071ISpDate22].Data[nCurRow] =
						ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_A1);
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

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc71()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

// インボイス登録番号追加対応_23/10/23 del -->
//int CprtUc071Koteisisan::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
int CprtUc071Koteisisan::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/23 add <--
{
	int				nCurRow			= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strAd2Add		= _T( "" );
	CString			strSyuruiOld	= _T( "" );
	CString			strAd2AddOld	= _T( "" );
	CString			strRiyuuOld		= _T( "" );
	CString			strAdName		= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAdd		= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc071Koteishisan	rc( m_pDB );

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_071 );

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
//		if((!(po.OutZeroNull[0]))|| (po.OutZeroNull[0] != 0 && rt == 2))	{
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
					
					strSyuruiOld	= _T( "" );
					strAd2AddOld	= _T( "" );
					strRiyuuOld		= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_MsVal	= HideZeroMoney( rc.m_MsVal );
						rc.m_MsVal2 = HideZeroMoney( rc.m_MsVal2 );
					}
				}

				// 期末現在高
				m_PrtDataBox[Field071Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );

				// 取得価額
				m_PrtDataBox[Field071MsVal].Data[nCurRow] =
					ShowFieldOfVal( rc.m_MsVal, &rc, MakeFormatOfNum( 12 ), BIT_D1 );

				// 異動直前の帳簿価額
				m_PrtDataBox[Field071MsVal2].Data[nCurRow] =
					ShowFieldOfVal( rc.m_MsVal2, &rc, MakeFormatOfNum( 12 ), BIT_D2 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strSyuruiOld	= _T( "" );
					strAd2AddOld	= _T( "" );
					strRiyuuOld		= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[Field071KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

						// 種類・構造
						case 1:

							if( CheckOmit( rc.m_Syurui, strSyuruiOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field071SyuruiOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								StrDivision( rc.m_Syurui, &strArray, 2 );
								m_PrtDataBox[Field071Syurui1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field071Syurui2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strSyuruiOld = rc.m_Syurui;
							break;

						// 物件の所在地
						case 2:

//							 strAd2Add = rc.m_Ad2Add1 + rc.m_Ad2Add2;
							 strAd2Add = rc.m_Ad2Add1 + PRT_FORMAT_CHCODE + rc.m_Ad2Add2;

							if( CheckOmit( strAd2Add, strAd2AddOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field071Ad2AddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

//								StrDivision(
//									(rc.m_Ad2Add1 + PRT_FORMAT_CHCODE + rc.m_Ad2Add2),
//									&strArray,
//									2 );
								StrDivision( strAd2Add, &strArray, 2 );
								m_PrtDataBox[Field071Ad2Add1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field071Ad2Add2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strAd2AddOld = strAd2Add;
							break;

						// 異動事由
						case 3:

							if( CheckOmit( rc.m_Riyuu, strRiyuuOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field071Riyuu1Om].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								StrDivision( rc.m_Riyuu, &strArray, 2 );
								m_PrtDataBox[Field071Riyuu1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field071Riyuu2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strRiyuuOld = rc.m_Riyuu;
							break;

						// 売却先（名称）
						case 4:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

//							if( CheckOmit( rc.m_Riyuu, strRiyuuOld, i, m_PrtInfOmitList1 ) ){
							if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field071AdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

//								StrDivision(
//									(rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2),
//									&strArray,
//									2 );
								StrDivision( strAdName, &strArray, 2 );
								m_PrtDataBox[Field071AdName1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field071AdName2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strAdNameOld = strAdName;
							break;

						// 売却先（所在地）
						case 5:

							 strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

//							if( CheckOmit( rc.m_Riyuu, strRiyuuOld, i, m_PrtInfOmitList1 ) ){
							if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field071AdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

//								StrDivision(
//									(rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2),
//									&strArray,
//									2 );
								StrDivision( strAdAdd, &strArray, 2 );
								m_PrtDataBox[Field071AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field071AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}

					// 用途
					StrDivision( rc.m_Youto, &strArray, 2 );
					m_PrtDataBox[Field071Youto1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field071Youto2].Data[nCurRow] = strArray.GetAt( 1 );

					// 面積
// No.200903 del -->
					//m_PrtDataBox[Field071Area].Data[nCurRow] =
					//	ShowFieldOfVal( rc.m_Area, &rc, MakeFormatOfNum( 6, 2 ) );
// No.200903 del <--
// No.200903 add -->
					if(bG_Kanso == TRUE)	{
						m_PrtDataBox[Field071Area].Data[nCurRow] =
							ShowFieldOfVal( rc.m_Area, &rc, MakeFormatOfNum( 8, 2 ) );
					}
					else	{
						m_PrtDataBox[Field071Area].Data[nCurRow] =
							ShowFieldOfVal( rc.m_Area, &rc, MakeFormatOfNum( 6, 2 ) );
					}
// No.200903 add <--

// midori 200101 del -->
					//// 異動年月日
					//m_PrtDataBox[Field071SpDate].Data[nCurRow] =
					//	ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
// midori 200101 del <--
// midori 200101 add -->
					// 異動年月日の元号
					if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
						m_PrtDataBox[Field071SpGengo].Data[nCurRow] =
							ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_G1, 1 );	
					}

					// 異動年月日
					m_PrtDataBox[Field071SpDate].Data[nCurRow] =
						ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );		//_T( "%2d.%2d.%2d" )
// midori 200101 add <--

// midori 200101 del -->
					//// 売却物件の取得年月
					//m_PrtDataBox[Field071SpDate21].Data[nCurRow] =
					//	ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_G1 );
// midori 200101 del <--
// midori 200101 add -->
					// 売却物件の取得年月の元号
					m_PrtDataBox[Field071SpDate21].Data[nCurRow] =
						ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_G1, 1 );
// midori 200101 add <--
					// 売却物件の取得年月
					m_PrtDataBox[Field071SpDate22].Data[nCurRow] =
						ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_A1 );
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc71()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc071Koteisisan::CprtUc071Koteisisan(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_071;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/23 del -->
	//m_nColumn	= Field071Max;
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 del -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field071IMax;
	else					m_nColumn	= Field071Max;
// インボイス登録番号追加対応_23/10/23 del <--
}

CprtUc071Koteisisan::~CprtUc071Koteisisan(void)
{
}

CprtJobFunction* CprtUc071Koteisisan::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc071Koteisisan(m_pDB));
}