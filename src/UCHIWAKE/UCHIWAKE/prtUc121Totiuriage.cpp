//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc121totiuriage.cpp
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
#include "prtUc121Totiuriage.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--

extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/25 add

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc121()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑫．土地の売上高等
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc121Totiuriage::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;

	if(bG_InvNo == TRUE) {
		rv = GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} else {
		rv = GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

int CprtUc121Totiuriage::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow = 0;
	CStringArray	strArray;
	CString			strAd2Add = _T("");
	CString			strAdName = _T("");
	CString			strAdAdd = _T("");
	CString			strKnNameOld = _T("");
	CString			strAd2AddOld = _T("");
	CString			strHdNameOld = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAddOld = _T("");
	CString			strSQL;
	CString			strSpGen = _T("");
	CdateConvert	clsDate;		//	日付変換

	// 省略文字の取得
	GetPrintInfo(parent);

	CdbUc121Totiuriage	rc(m_pDB);

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
			strSQL.Format(_T("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow"), m_nFormSeq, nPage);
#else
			strSQL.Format(_T("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow"), m_nFormSeq, nPage);
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

					strKnNameOld = _T("");
					strAd2AddOld = _T("");
					strHdNameOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[0]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
						rc.m_Val2 = HideZeroMoney(rc.m_Val2);
					}
				}

				// 売上金額
				if (bG_Kanso == TRUE) {
					m_PrtDataBox[Field121IVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(13), BIT_D0);
					m_PrtDataBox[Field121IVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val2, &rc, MakeFormatOfNum(13), BIT_D1);
				}
				else {
					m_PrtDataBox[Field121IVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);
					m_PrtDataBox[Field121IVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val2, &rc, MakeFormatOfNum(12), BIT_D1);
				}

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {
					strKnNameOld = _T("");
					strAd2AddOld = _T("");
					strHdNameOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");

					m_PrtDataBox[Field121IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field121IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);
					
					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch (nItemSeq) {

							// 区分
						case 1:

							if (CheckOmit(rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field121IKnNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								m_PrtDataBox[Field121IKnName].Data[nCurRow] = rc.m_KnName;
							}

							strKnNameOld = rc.m_KnName;
							break;

							// 商品の所在地
						case 2:

							strAd2Add = rc.m_Ad2Add1 + PRT_FORMAT_CHCODE + rc.m_Ad2Add2;

							if (CheckOmit(strAd2Add, strAd2AddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field121IAd2AddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAd2Add, &strArray, 2);
								m_PrtDataBox[Field121IAd2Add1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field121IAd2Add2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAd2AddOld = strAd2Add;
							break;

							// 地目
						case 3:

							if (CheckOmit(rc.m_HdName, strHdNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field121IHdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								m_PrtDataBox[Field121IHdName].Data[nCurRow] = rc.m_HdName;
							}

							strHdNameOld = rc.m_HdName;
							break;

							// 売上先（名称）
						case 4:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field121IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[Field121IAdName1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field121IAdName2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 売上先（所在地）
						case 5:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field121IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[Field121IAdAdd1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field121IAdAdd2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}

					// 総面積
					if (bG_Kanso == TRUE) {
						m_PrtDataBox[Field121IArea].Data[nCurRow] =
							ShowFieldOfVal(rc.m_Area, &rc, MakeFormatOfNum(8, 2));
					}
					else {
						m_PrtDataBox[Field121IArea].Data[nCurRow] =
							ShowFieldOfVal(rc.m_Area, &rc, MakeFormatOfNum(6, 2));
					}

					if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
						// 売上年月の元号
						strSpGen.Empty();
						if (rc.m_SpDate > 0) {
							clsDate.Convert(rc.m_SpDate);
							strSpGen = clsDate.m_szGengou;
						}
						m_PrtDataBox[Field121ISpGengo].Data[nCurRow] = strSpGen;
					}
					else {
						m_PrtDataBox[Field121ISpGengo].Data[nCurRow].Empty();
					}

										// 売上年月
					m_PrtDataBox[Field121ISpDate].Data[nCurRow] =
						ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_A1);

					// 売上面積
					if (bG_Kanso == TRUE) {
						m_PrtDataBox[Field121IArea2].Data[nCurRow] =
							ShowFieldOfVal(rc.m_Area2, &rc, MakeFormatOfNum(8, 2));
					}
					else {
						m_PrtDataBox[Field121IArea2].Data[nCurRow] =
							ShowFieldOfVal(rc.m_Area2, &rc, MakeFormatOfNum(6, 2));
					}

					// 売上商品の取得年の元号
					m_PrtDataBox[Field121ISpGengo2].Data[nCurRow] =
						ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_G1, 1);
					// 売上商品の取得年
					m_PrtDataBox[Field121ISpDate2].Data[nCurRow] =
						ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_A2);
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

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc121()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/10/25 add <--

// インボイス登録番号追加対応_23/10/25 del -->
//int CprtUc121Totiuriage::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc121Totiuriage::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 add <--
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
	CString			strAdName		= _T( "" );
	CString			strAdAdd		= _T( "" );
//	CString			strKubunOld		= _T( "" );
	CString			strKnNameOld	= _T( "" );
	CString			strAd2AddOld	= _T( "" );
	CString			strHdNameOld	= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
//	CString			Val2Kei[4]		={_T( "" )};
	CString			strSQL;
// midori 200101 add -->
	CString			strSpGen		= _T("");
	CdateConvert	clsDate;		//	日付変換
// midori 200101 add <--

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc121Totiuriage	rc( m_pDB );

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
			//strSQL.Format(_T("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow"), m_nFormSeq, nPage );
// ---------------------
#ifdef _DEBUG
			strSQL.Format(_T("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow"), m_nFormSeq, nPage );
#else
			strSQL.Format(_T("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow"), m_nFormSeq, nPage );
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
					
//					strKubunOld		= _T( "" );
					strKnNameOld	= _T( "" );
					strAd2AddOld	= _T( "" );
					strHdNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_Val2	= HideZeroMoney( rc.m_Val2 );
					}
				}

				// 売上金額
// No.200903 del -->
				//m_PrtDataBox[Field121Val].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				//m_PrtDataBox[Field121Val2].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D1 );
// No.200903 del <--
// No.200903 add -->
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[Field121Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
					m_PrtDataBox[Field121Val2].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 13 ), BIT_D1 );
				}
				else	{
					m_PrtDataBox[Field121Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
					m_PrtDataBox[Field121Val2].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D1 );
				}
// No.200903 add <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

//					strKubunOld		= _T( "" );
					strKnNameOld	= _T( "" );
					strAd2AddOld	= _T( "" );
					strHdNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[Field121KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 区分
							case 1:

//								if( CheckOmit( rc.m_Kubun, strKubunOld, i, m_PrtInfOmitList1 ) ){
								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

//									m_PrtDataBox[Field121KubunOm].Data[nCurRow] =
									m_PrtDataBox[Field121KnNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

//									m_PrtDataBox[Field121Kubun].Data[nCurRow] = rc.m_Kubun;
									m_PrtDataBox[Field121KnName].Data[nCurRow] = rc.m_KnName;
								}
								
//								strKubunOld = rc.m_Kubun;
								strKnNameOld = rc.m_KnName;
								break;

							// 商品の所在地
							case 2:

								strAd2Add = rc.m_Ad2Add1 + PRT_FORMAT_CHCODE + rc.m_Ad2Add2;

								if( CheckOmit( strAd2Add , strAd2AddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field121Ad2AddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAd2Add, &strArray, 2 );
									m_PrtDataBox[Field121Ad2Add1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field121Ad2Add2].Data[nCurRow] = strArray.GetAt( 1 );
								}
								
								strAd2AddOld = strAd2Add;
								break;

							// 地目
							case 3:

								if( CheckOmit( rc.m_HdName, strHdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field121HdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									
									m_PrtDataBox[Field121HdName].Data[nCurRow] = rc.m_HdName;
								}
								
								strHdNameOld = rc.m_HdName;
								break;

							// 売上先（名称）
							case 4:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field121AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field121AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field121AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 売上先（所在地）
							case 5:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field121AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field121AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field121AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}

					// 総面積
// No.200903 del -->
					//m_PrtDataBox[Field121Area].Data[nCurRow] =
					//	ShowFieldOfVal( rc.m_Area, &rc, MakeFormatOfNum( 6, 2 ) );
// No.200903 del <--
// No.200903 add -->
					if(bG_Kanso == TRUE)	{
						m_PrtDataBox[Field121Area].Data[nCurRow] =
							ShowFieldOfVal( rc.m_Area, &rc, MakeFormatOfNum( 8, 2 ) );
					}
					else	{
						m_PrtDataBox[Field121Area].Data[nCurRow] =
							ShowFieldOfVal( rc.m_Area, &rc, MakeFormatOfNum( 6, 2 ) );
					}
// No.200903 add <--

// midori 200101 add -->
					if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
						// 売上年月の元号
						strSpGen.Empty();
						if(rc.m_SpDate > 0)	{
							clsDate.Convert(rc.m_SpDate);
							strSpGen = clsDate.m_szGengou;
						}
						m_PrtDataBox[Field121SpGengo].Data[nCurRow] = strSpGen;
					}
					else	{
						m_PrtDataBox[Field121SpGengo].Data[nCurRow].Empty();
					}
// midori 200101 add <--

					// 売上年月
					m_PrtDataBox[Field121SpDate].Data[nCurRow] =
						ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 );

					// 売上面積
// No.200903 del -->
					//m_PrtDataBox[Field121Area2].Data[nCurRow] =
					//	ShowFieldOfVal( rc.m_Area2, &rc, MakeFormatOfNum( 6, 2 ) );
// No.200903 del <--
// No.200903 add -->
					if(bG_Kanso == TRUE)	{
						m_PrtDataBox[Field121Area2].Data[nCurRow] =
							ShowFieldOfVal( rc.m_Area2, &rc, MakeFormatOfNum( 8, 2 ) );
					}
					else	{
						m_PrtDataBox[Field121Area2].Data[nCurRow] =
							ShowFieldOfVal( rc.m_Area2, &rc, MakeFormatOfNum( 6, 2 ) );
					}
// No.200903 add <--

// midori 200101 del -->
					//// 売上商品の取得年
					//m_PrtDataBox[Field121SpDate2].Data[nCurRow] =
					//	ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_D1 );
// midori 200101 del <--
// midori 200101 add -->
					//if(bG_Kanso == TRUE)	{
						//m_PrtDataBox[Field121SpDate2].Data[nCurRow] =
						//	ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_D1, 1);
					//}
					//else	{
						//m_PrtDataBox[Field121SpDate2].Data[nCurRow] =
						//	ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_D1 );
					//}

					// 売上商品の取得年の元号
					m_PrtDataBox[Field121SpGengo2].Data[nCurRow] =
						ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_G1, 1);
					// 売上商品の取得年
					m_PrtDataBox[Field121SpDate2].Data[nCurRow] =
						ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_A2 );
// midori 200101 add <--
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc121()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc121Totiuriage::CprtUc121Totiuriage(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_121;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/25 del -->
	//m_nColumn	= Field121Max;
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field121IMax;
	else					m_nColumn	= Field121Max;
// インボイス登録番号追加対応_23/10/25 add <--
}

CprtUc121Totiuriage::~CprtUc121Totiuriage(void)
{
}

CprtJobFunction* CprtUc121Totiuriage::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc121Totiuriage(m_pDB));
}