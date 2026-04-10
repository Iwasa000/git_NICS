//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc150TidaiyatinKenrikinKougyou.cpp
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
#include "prtUc150TidaiyatinKenrikinKougyou.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--
extern BOOL bG_InvNo;		// インボイス登録番号追加対応_23/11/02 add

int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int nRowMax		= 0;
	int nRowMax2	= 0;
	int nRowMax3	= 0;
// インボイス登録番号追加対応_23/11/02 del -->
	//if(po.nPageNum[0] != 0)	nRowMax	 = GetDbDataUc151( nPage, parent, m_nFormSeq, po );
	//if(po.nPageNum[1] != 0)	nRowMax2 = GetDbDataUc152( nPage, parent, m_nFormSeq, po );
	//if(po.nPageNum[2] != 0)	nRowMax3 = GetDbDataUc153( nPage, parent, m_nFormSeq, po );
	//m_nColumn = Field151Max;
// インボイス登録番号追加対応_23/11/02 del <--
// インボイス登録番号追加対応_23/11/02 add -->
	if(po.nPageNum[0] != 0) {
		if(bG_InvNo == TRUE)	nRowMax = GetDbDataUc151New(nPage, parent, m_nFormSeq, po);
		else					nRowMax = GetDbDataUc151Old(nPage, parent, m_nFormSeq, po);
	}
	if(po.nPageNum[1] != 0) {
		if(bG_InvNo == TRUE)	nRowMax2 = GetDbDataUc152New(nPage, parent, m_nFormSeq, po);
		else					nRowMax2 = GetDbDataUc152Old(nPage, parent, m_nFormSeq, po);
	}
	if(po.nPageNum[2] != 0) {
		if(bG_InvNo == TRUE)	nRowMax3 = GetDbDataUc153New(nPage, parent, m_nFormSeq, po);
		else					nRowMax3 = GetDbDataUc153Old(nPage, parent, m_nFormSeq, po);
	}

	if(bG_InvNo == TRUE)	m_nColumn = Field151IMax;
	else					m_nColumn = Field151Max;
// インボイス登録番号追加対応_23/11/02 add <--

	// 行数の多い方を使用
	if( nRowMax2 > nRowMax ){
		nRowMax = nRowMax2;
	}

	if( nRowMax3 > nRowMax ){
		nRowMax = nRowMax3;
	}

	m_nRowMax = nRowMax;
// インボイス登録番号追加対応_23/11/02 del -->
	//m_nColumn += Field152Max;
	//m_nColumn += Field153Max;
// インボイス登録番号追加対応_23/11/02 del <--
// インボイス登録番号追加対応_23/11/02 del -->
	if(bG_InvNo == TRUE) {
		m_nColumn += Field152IMax;
		m_nColumn += Field153IMax;
	}
	else	{
		m_nColumn += Field152Max;
		m_nColumn += Field153Max;
	}	
// インボイス登録番号追加対応_23/11/02 del <--

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc151()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑮-1．地代家賃
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/11/02 add -->
int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc151New(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				nVisualPage = nPage;
	CStringArray	strArray;
	CString			strAd2Add = _T("");
	CString			strAdName = _T("");
	CString			strAdAdd = _T("");
	CString			strKnNameOld = _T("");
	CString			strYoutoOld = _T("");
	CString			strAd2AddOld = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAddOld = _T("");
	CString			strSQL;
	CString			strSpDate = _T("");
	CString			strSpDate2 = _T("");
	CString			strWave = PRT_FORMAT_WAVE;
	CdateConvert	clsDate;		//	日付変換

	// 省略文字の取得
	m_nFormSeq = ID_FORMNO_151;
	m_nColumn = Field151IMax;
	GetPrintInfo(parent);

	CdbUc151Tidaiyatin	rc(m_pDB);

	try {
		if (!(po.OutZeroNull[0]) || m_nOutZero == 1) {
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

					strKnNameOld = _T("");
					strYoutoOld = _T("");
					strAd2AddOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[0]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
					}
				}

				// 支払貸借料
				m_PrtDataBox[Field151IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strKnNameOld = _T("");
					strYoutoOld = _T("");
					strAd2AddOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");

					m_PrtDataBox[Field151IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field151IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch (nItemSeq) {

							// 地代・家賃の区分
						case 1:

							if (CheckOmit(rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field151IKnNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								m_PrtDataBox[Field151IKnName].Data[nCurRow] = rc.m_KnName;
							}

							strKnNameOld = rc.m_KnName;
							break;

							// 借地物件の用途
						case 2:

							if (CheckOmit(rc.m_Youto, strYoutoOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field151IYoutoOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(rc.m_Youto, &strArray, 2);
								m_PrtDataBox[Field151IYouto1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field151IYouto2].Data[nCurRow] = strArray.GetAt(1);
							}

							strYoutoOld = rc.m_Youto;
							break;

							// 所在地
						case 3:

							strAd2Add = rc.m_Ad2Add1 + PRT_FORMAT_CHCODE + rc.m_Ad2Add2;

							if (CheckOmit(strAd2Add, strAd2AddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field151IAd2AddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAd2Add, &strArray, 2);
								m_PrtDataBox[Field151IAd2Add1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field151IAd2Add2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAd2AddOld = strAd2Add;
							break;

							// 貸主（名称）
						case 4:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field151IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[Field151IAdName1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field151IAdName2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 貸主（所在地）
						case 5:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field151IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[Field151IAdAdd1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field151IAdAdd2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}

					// 支払対象期間
					// 自
					strSpDate.Empty();
					if (rc.m_SpDate > 0) {
						if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
							// 元号年月日
							strSpDate = ConvertNullIntoSpace(ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_E2), 8);
						}
						else {
							// 年月日
							strSpDate = ConvertNullIntoSpace(ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_B1), 8);
						}
					}

					// 至
					strSpDate2.Empty();
					if (rc.m_SpDate2 > 0) {
						if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
							// 元号年月日
							strSpDate2 = ConvertNullIntoSpace(ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_E2), 8);
						}
						else {
							// 年月日
							strSpDate2 = ConvertNullIntoSpace(ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_B1), 8);
						}
					}

					// 「～」
					strWave = PRT_FORMAT_WAVE;
					// 期間を指定できない場合、「～」を出力しない
					if ((rc.m_SpDate == 0) || (rc.m_SpDate2 == 0))	strWave.Empty();

					// 自、至ともに入力あり
					if (strWave.IsEmpty() == FALSE) {
						// 期間
						if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
							m_PrtDataBox[Field151ISpDate2].Data[nCurRow] = strSpDate + " " + strWave;
							m_PrtDataBox[Field151ISpDate3].Data[nCurRow] = strSpDate2;
						}
						else {
							m_PrtDataBox[Field151ISpDate5].Data[nCurRow] = strSpDate + " " + strWave;
							m_PrtDataBox[Field151ISpDate6].Data[nCurRow] = strSpDate2;
						}
					}
					else {
						if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
							m_PrtDataBox[Field151ISpDate].Data[nCurRow] = strSpDate + strWave + strSpDate2;
						}
						else {
							m_PrtDataBox[Field151ISpDate4].Data[nCurRow] = strSpDate + strWave + strSpDate2;
						}
					}

					// 摘要
					StrDivision(rc.m_Teki, &strArray, 3);
					m_PrtDataBox[Field151ITeki1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[Field151ITeki2].Data[nCurRow] = strArray.GetAt(1);
					m_PrtDataBox[Field151ITeki3].Data[nCurRow] = strArray.GetAt(2);
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		int nItem = Field151IMax;

		return m_nRowMax;
	}
	catch (...) {

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc151()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/11/02 add -->

// インボイス登録番号追加対応_23/11/02 del -->
//int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc151(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/02 del <--
// インボイス登録番号追加対応_23/11/02 add -->
int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc151Old(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/02 add <--
{
	int				nCurRow			= 0;
	int				nVisualPage		= nPage;
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
	CString			strKnNameOld	= _T( "" );
	CString			strYoutoOld		= _T( "" );
	CString			strAd2AddOld	= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;
// midori 200101 add -->
	CString			strSpDate		= _T("");
	CString			strSpDate2		= _T("");
	CString			strWave			= PRT_FORMAT_WAVE;
	CdateConvert	clsDate;		//	日付変換
// midori 200101 add <--

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_151;
	m_nColumn	= Field151Max;
	GetPrintInfo( parent );

	CdbUc151Tidaiyatin	rc( m_pDB );

	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 160608 cor -->
//////		if(!(po.OutZeroNull[0])){
////// ---------------------
////		if(!(po.OutZeroNull[0]) || m_nOutZero == 1){
////// midori 160608 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[0] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[0] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(m_nFormSeq, ck1, ck2);
//
//		if((!(po.OutZeroNull[0]) || m_nOutZero == 1) || (po.OutZeroNull[0] != 0 && rt == 2))	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if(!(po.OutZeroNull[0]) || m_nOutZero == 1){
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
					
//					strKubunOld		= _T( "" );
					strKnNameOld	= _T( "" );
					strYoutoOld		= _T( "" );
					strAd2AddOld	= _T( "" );
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

				// 支払貸借料
				m_PrtDataBox[Field151Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

//					strKubunOld		= _T( "" );
					strKnNameOld	= _T( "" );
					strYoutoOld		= _T( "" );
					strAd2AddOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[Field151KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 地代・家賃の区分
							case 1:

//								if( CheckOmit( rc.m_Kubun, strKubunOld, i, m_PrtInfOmitList1 ) ){
								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

//									m_PrtDataBox[Field151KubunOm].Data[nCurRow] =
									m_PrtDataBox[Field151KnNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

//									m_PrtDataBox[Field151Kubun].Data[nCurRow] = rc.m_Kubun;
									m_PrtDataBox[Field151KnName].Data[nCurRow] = rc.m_KnName;
								}

//								strKubunOld = rc.m_Kubun;
								strKnNameOld = rc.m_KnName;
								break;

							// 借地物件の用途
							case 2:

								if( CheckOmit( rc.m_Youto, strYoutoOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field151YoutoOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( rc.m_Youto, &strArray, 2 );
									m_PrtDataBox[Field151Youto1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field151Youto2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strYoutoOld = rc.m_Youto;
								break;

							// 所在地
							case 3:

								strAd2Add = rc.m_Ad2Add1 + PRT_FORMAT_CHCODE + rc.m_Ad2Add2;

								if( CheckOmit( strAd2Add, strAd2AddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field151Ad2AddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAd2Add, &strArray, 2 );
									m_PrtDataBox[Field151Ad2Add1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field151Ad2Add2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAd2AddOld = strAd2Add;
								break;

							// 貸主（名称）
							case 4:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field151AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field151AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field151AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 貸主（所在地）
							case 5:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field151AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field151AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field151AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}

// midori 200101 del -->
					//// 支払対象期間
					//CString strSpDate, strSpDate2;
					//CString strWave = PRT_FORMAT_WAVE;

					//if( rc.m_SpDate == 0 )	strSpDate.Empty();
					//else					strSpDate = ConvertNullIntoSpace( ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 ), 8 );	// ConvertNullIntoSpace( ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 ), 5 )

					//if( rc.m_SpDate2 == 0 )	strSpDate2.Empty();
					//else					strSpDate2 = ConvertNullIntoSpace( ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_B1 ), 8 );	// ConvertNullIntoSpace( ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_A1 ), 5 );

					//// 期間を指定できない場合、「～」を出力しない
					//if(( rc.m_SpDate == 0 )||( rc.m_SpDate2 == 0 ))	strWave.Empty();
					//m_PrtDataBox[Field151SpDate].Data[nCurRow] = strSpDate + strWave + strSpDate2;
// midori 200101 del <--
// midori 200101 add -->
					// 支払対象期間
					// 自
					strSpDate.Empty();
					if(rc.m_SpDate > 0)	{
						if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
							// 元号年月日
							strSpDate = ConvertNullIntoSpace( ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E2 ), 8 );
						}
						else {
							// 年月日
							strSpDate = ConvertNullIntoSpace( ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 ), 8 );
						}
					}

					// 至
					strSpDate2.Empty();
					if(rc.m_SpDate2 > 0)	{
						if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
							// 元号年月日
							strSpDate2 = ConvertNullIntoSpace( ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_E2 ), 8 );
						}
						else {
							// 年月日
							strSpDate2 = ConvertNullIntoSpace( ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_B1 ), 8 );
						}
					}

					// 「～」
					strWave = PRT_FORMAT_WAVE;
					// 期間を指定できない場合、「～」を出力しない
					if( (rc.m_SpDate == 0) || (rc.m_SpDate2 == 0) )	strWave.Empty();

					// 自、至ともに入力あり
					if(strWave.IsEmpty() == FALSE)	{
						// 期間
// midori 155870 add -->
						if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
// midori 155870 add <--
							m_PrtDataBox[Field151SpDate2].Data[nCurRow] = strSpDate + " " + strWave;
							m_PrtDataBox[Field151SpDate3].Data[nCurRow] = strSpDate2;
// midori 155870 add -->
						}
						else {
							m_PrtDataBox[Field151SpDate5].Data[nCurRow] = strSpDate + " " + strWave;
							m_PrtDataBox[Field151SpDate6].Data[nCurRow] = strSpDate2;
						}
// midori 155870 add <--
					}
					else	{
// midori 155870 add -->
						if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
// midori 155870 add <--
							m_PrtDataBox[Field151SpDate].Data[nCurRow] = strSpDate + strWave + strSpDate2;
// midori 155870 add -->
						}
						else	{
							m_PrtDataBox[Field151SpDate4].Data[nCurRow] = strSpDate + strWave + strSpDate2;
						}
// midori 155870 add <--
					}
// midori 200101 add <--

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 3 );
					m_PrtDataBox[Field151Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field151Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[Field151Teki3].Data[nCurRow] = strArray.GetAt( 2 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		int nItem = Field151Max;

		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc151()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc152()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑮-2．権利金等の期中支払
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/11/02 add -->
int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc152New(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				index = 0;
	CStringArray	strArray;
	CString			strAdName = _T("");
	CString			strAdAdd = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAddOld = _T("");
	CString			strNaiyouOld = _T("");
	CString			strSQL;

	// 省略文字の取得
	m_nFormSeq = ID_FORMNO_152;
	m_nColumn = Field152IMax;

	GetPrintInfo(parent);

	// データボックス内の格納場所を変更
	index = Field151IMax;

	CdbUc152Kenrikin	rc(m_pDB);

	try {
		if (!(po.OutZeroNull[1]) || m_nOutZero == 1) {
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
					strNaiyouOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[1]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
					}
				}

				// 支払金額
				m_PrtDataBox[index + Field152IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strAdNameOld = _T("");
					strAdAddOld = _T("");
					strNaiyouOld = _T("");

					m_PrtDataBox[index + Field152IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else	{
					// インボイス登録番号（法人番号）
					m_PrtDataBox[index + Field152IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList2[i].ItemSeq;

						switch (nItemSeq) {

							// 支払先（名称）
						case 1:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field152IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[index + Field152IAdName1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field152IAdName2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 支払先（所在地）
						case 2:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field152IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {

								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[index + Field152IAdAdd1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field152IAdAdd2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

							// 権利金等の内容
						case 3:

							if (CheckOmit(rc.m_Naiyou, strNaiyouOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field152INaiyouOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {

								StrDivision(rc.m_Naiyou, &strArray, 2);
								m_PrtDataBox[index + Field152INaiyou1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field152INaiyou2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strNaiyouOld = rc.m_Naiyou;
							break;

						default:
							break;
						}
					}

					// 支払年月日
					if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
						m_PrtDataBox[index + Field152ISpDate].Data[nCurRow] =
							ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_E2);
					}
					else {
						m_PrtDataBox[index + Field152ISpDate].Data[nCurRow] =
							ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_B1);
					}

					// 摘要
					StrDivision(rc.m_Teki, &strArray, 3);
					m_PrtDataBox[index + Field152ITeki1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[index + Field152ITeki2].Data[nCurRow] = strArray.GetAt(1);
					m_PrtDataBox[index + Field152ITeki3].Data[nCurRow] = strArray.GetAt(2);
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

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc152()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/11/02 add <--

// インボイス登録番号追加対応_23/11/02 del -->
//int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc152(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/02 del <--
// インボイス登録番号追加対応_23/11/02 add -->
int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc152Old(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/02 add <--
{
	int				nCurRow			= 0;
	int				index			= 0;
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
	CString			strNaiyouOld	= _T( "" );
	CString			strSQL;

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_152;
	m_nColumn	= Field152Max;

	GetPrintInfo( parent );

	// データボックス内の格納場所を変更
	index		= Field151Max;

	CdbUc152Kenrikin	rc( m_pDB );

	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 160608 cor -->
//////		if(!(po.OutZeroNull[1])){
////// ---------------------
////		if(!(po.OutZeroNull[1]) || m_nOutZero == 1){
////// midori 160608 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[1] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[1] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(nFormSeq, ck1, ck2);
//
//		if((!(po.OutZeroNull[1]) || m_nOutZero == 1) || (po.OutZeroNull[1] != 0 && rt == 2))	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if(!(po.OutZeroNull[1]) || m_nOutZero == 1){
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
					
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strNaiyouOld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[1]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 支払金額
				m_PrtDataBox[index + Field152Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strNaiyouOld	= _T( "" );

					m_PrtDataBox[index + Field152KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList2[i].ItemSeq;

						switch( nItemSeq ){

							// 支払先（名称）
							case 1:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field152AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[index + Field152AdName1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field152AdName2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 支払先（所在地）
							case 2:
								
								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field152AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[index + Field152AdAdd1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field152AdAdd2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							// 権利金等の内容
							case 3:

								if( CheckOmit( rc.m_Naiyou, strNaiyouOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field152NaiyouOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{

									StrDivision( rc.m_Naiyou, &strArray, 2 );
									m_PrtDataBox[index + Field152Naiyou1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field152Naiyou2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strNaiyouOld = rc.m_Naiyou;
								break;

							default:
								break;
						}
					}

					// 支払年月日
// midori 200101 del -->
					//m_PrtDataBox[index + Field152SpDate].Data[nCurRow] = 
					//	ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
// midori 200101 del <--
// midori 200101 add -->
					if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
						m_PrtDataBox[index + Field152SpDate].Data[nCurRow] = 
							ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E2 );
					}
					else	{
						m_PrtDataBox[index + Field152SpDate].Data[nCurRow] = 
							ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
					}
// midori 200101 add <--

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 3 );
					m_PrtDataBox[index + Field152Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[index + Field152Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[index + Field152Teki3].Data[nCurRow] = strArray.GetAt( 2 );
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc152()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc153()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑮-3．工業所有権の使用料
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/11/02 add -->
int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc153New(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				index = 0;
	CStringArray	strArray;
	CString			strAdName = _T("");
	CString			strAdAdd = _T("");
	CString			strHdNameOld = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAddOld = _T("");
	CString			strSQL;

	// 省略文字の取得
	m_nFormSeq = ID_FORMNO_153;
	m_nColumn = Field153IMax;

	GetPrintInfo(parent);

	// データボックス内の格納場所を変更
	index = Field151IMax + Field152IMax;

	CdbUc153Kougyou	rc(m_pDB);

	try {
		if (!(po.OutZeroNull[2]) || m_nOutZero == 1) {
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

					strHdNameOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[2]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
					}
				}

				// 支払金額
				m_PrtDataBox[index + Field153IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strHdNameOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");

					m_PrtDataBox[index + Field153IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[index + Field153IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList3[i].ItemSeq;

						switch (nItemSeq) {

							// 名称
						case 1:

							if (CheckOmit(rc.m_HdName, strHdNameOld, i, m_PrtInfOmitList3)) {

								m_PrtDataBox[index + Field153IHdNameOm].Data[nCurRow] =
									m_PrtInfOmitList3[i].OmitStr;
							}
							else {

								StrDivision(rc.m_HdName, &strArray, 2);
								m_PrtDataBox[index + Field153IHdName1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field153IHdName2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strHdNameOld = rc.m_HdName;
							break;

							// 支払先（名称）
						case 2:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList3)) {

								m_PrtDataBox[index + Field153IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList3[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[index + Field153IAdName1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field153IAdName2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 支払先（所在地）
						case 3:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList3)) {

								m_PrtDataBox[index + Field153IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList3[i].OmitStr;
							}
							else {

								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[index + Field153IAdAdd1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field153IAdAdd2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}

					// 契約期間
					if ((rc.m_KyDate == 0) || (rc.m_KyDate2 == 0)) {	// 期間を指定できない場合（開始か終了のどちらかしか入力されていない）
						CString strKyDate, strKyDate2;

						if (rc.m_KyDate == 0)	strKyDate.Empty();
						else					strKyDate = ConvertDate(rc.m_KyDate, PRT_FORMAT_DATE_E1);

						if (rc.m_KyDate2 == 0)	strKyDate2.Empty();
						else					strKyDate2 = ConvertDate(rc.m_KyDate2, PRT_FORMAT_DATE_E1);

						m_PrtDataBox[index + Field153IKyDate0].Data[nCurRow] = strKyDate + strKyDate2;
					}
					else {
						// 契約期間（開始）
						m_PrtDataBox[index + Field153IKyDate].Data[nCurRow] =
							ConvertNullIntoSpace(ConvertDate(rc.m_KyDate, PRT_FORMAT_DATE_E1), 7) +
							PRT_FORMAT_WAVE;

						// 契約期間（終了）
						m_PrtDataBox[index + Field153IKyDate2].Data[nCurRow] =
							ConvertDate(rc.m_KyDate2, PRT_FORMAT_DATE_E1);
					}

					// 支払対象期間
					if ((rc.m_SpDate == 0) || (rc.m_SpDate2 == 0)) {
						CString strSpDate, strSpDate2;

						if (rc.m_SpDate == 0) {
							strSpDate.Empty();
						}
						else {
							if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
								strSpDate = ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_E1);
							}
							else {
								strSpDate = ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_A1);
							}
						}

						if (rc.m_SpDate2 == 0) {
							strSpDate2.Empty();
						}
						else {
							if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
								strSpDate2 = ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_E1);
							}
							else {
								strSpDate2 = ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_A1);
							}
						}

						m_PrtDataBox[index + Field153ISpDate0].Data[nCurRow] = strSpDate + strSpDate2;
					}
					else {
						if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
							// 支払対象期間（開始）
							m_PrtDataBox[index + Field153ISpDate].Data[nCurRow] =
								ConvertNullIntoSpace(ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_E1), 7) +
								PRT_FORMAT_WAVE;

							// 支払対象期間（終了）
							m_PrtDataBox[index + Field153ISpDate2].Data[nCurRow] =
								ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_E1);
						}
						else {
							// 支払対象期間（開始）
							m_PrtDataBox[index + Field153ISpDate].Data[nCurRow] =
								ConvertNullIntoSpace(ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_A1), 5) +
								PRT_FORMAT_WAVE;

							// 支払対象期間（終了）
							m_PrtDataBox[index + Field153ISpDate2].Data[nCurRow] =
								ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_A1);
						}
					}

					// 摘要
					StrDivision(rc.m_Teki, &strArray, 3);
					m_PrtDataBox[index + Field153ITeki1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[index + Field153ITeki2].Data[nCurRow] = strArray.GetAt(1);
					m_PrtDataBox[index + Field153ITeki3].Data[nCurRow] = strArray.GetAt(2);
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

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc153()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/11/02 add <--

// インボイス登録番号追加対応_23/11/02 del -->
//int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc153(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/02 del <--
// インボイス登録番号追加対応_23/11/02 add -->
int CprtUc150TidaiyatinKenrikinKougyou::GetDbDataUc153Old(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/02 add <--
{
	int				nCurRow			= 0;
	int				index			= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strAdAdd		= _T( "" );
	CString			strHdNameOld	= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_153;
	m_nColumn	= Field153Max;

	GetPrintInfo( parent );

	// データボックス内の格納場所を変更
	index		= Field151Max + Field152Max;

	CdbUc153Kougyou	rc( m_pDB );

	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 160608 cor -->
//////		if(!(po.OutZeroNull[2])){
////// ---------------------
////		if(!(po.OutZeroNull[2]) || m_nOutZero == 1){
////// midori 160608 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[2] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[2] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(nFormSeq, ck1, ck2);
//
//		if((!(po.OutZeroNull[2]) || m_nOutZero == 1) || (po.OutZeroNull[2] != 0 && rt == 2))	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if(!(po.OutZeroNull[2]) || m_nOutZero == 1){
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
					
					strHdNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[2]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 支払金額
				m_PrtDataBox[index + Field153Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strHdNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[index + Field153KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList3[i].ItemSeq;

						switch( nItemSeq ){

							// 名称
							case 1:

								if( CheckOmit( rc.m_HdName, strHdNameOld, i, m_PrtInfOmitList3 ) ){

									m_PrtDataBox[index + Field153HdNameOm].Data[nCurRow] =
										m_PrtInfOmitList3[i].OmitStr;
								}
								else{

									StrDivision( rc.m_HdName, &strArray, 2 );
									m_PrtDataBox[index + Field153HdName1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field153HdName2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strHdNameOld = rc.m_HdName;
								break;

							// 支払先（名称）
							case 2:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList3 ) ){

									m_PrtDataBox[index + Field153AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList3[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[index + Field153AdName1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field153AdName2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 支払先（所在地）
							case 3:
								
								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList3 ) ){

									m_PrtDataBox[index + Field153AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList3[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[index + Field153AdAdd1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field153AdAdd2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}

					// 契約期間
					if(( rc.m_KyDate == 0 )||( rc.m_KyDate2 == 0 )){	// 期間を指定できない場合（開始か終了のどちらかしか入力されていない）
						CString strKyDate, strKyDate2;

						if( rc.m_KyDate == 0 )	strKyDate.Empty();
						else					strKyDate = ConvertDate( rc.m_KyDate, PRT_FORMAT_DATE_E1 );

						if( rc.m_KyDate2 == 0 )	strKyDate2.Empty();
						else					strKyDate2 = ConvertDate( rc.m_KyDate2, PRT_FORMAT_DATE_E1 );

						m_PrtDataBox[index + Field153KyDate0].Data[nCurRow] = strKyDate + strKyDate2;
					}
					else{
						// 契約期間（開始）
						m_PrtDataBox[index + Field153KyDate].Data[nCurRow] =
							ConvertNullIntoSpace( ConvertDate( rc.m_KyDate, PRT_FORMAT_DATE_E1 ), 7 ) +
							PRT_FORMAT_WAVE;

						// 契約期間（終了）
						m_PrtDataBox[index + Field153KyDate2].Data[nCurRow] =
							ConvertDate( rc.m_KyDate2, PRT_FORMAT_DATE_E1 );
					}
					
// midori 200101 del -->
					//// 支払対象期間
					//if(( rc.m_SpDate == 0 )||( rc.m_SpDate2 == 0 )){
					//	CString strSpDate, strSpDate2;

					//	if( rc.m_SpDate == 0 )	strSpDate.Empty();
					//	else					strSpDate = ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 );

					//	if( rc.m_SpDate2 == 0 )	strSpDate2.Empty();
					//	else					strSpDate2 = ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_A1 );

					//	m_PrtDataBox[index + Field153SpDate0].Data[nCurRow] = strSpDate + strSpDate2;
					//}
					//else{
					//	// 支払対象期間（開始）
					//	m_PrtDataBox[index + Field153SpDate].Data[nCurRow] =
					//		ConvertNullIntoSpace( ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 ), 5 ) +
					//		PRT_FORMAT_WAVE;

					//	// 支払対象期間（終了）
					//	m_PrtDataBox[index + Field153SpDate2].Data[nCurRow] =
					//		ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_A1 );
					//}
// midori 200101 del <--
// midori 200101 add -->
					// 支払対象期間
					if(( rc.m_SpDate == 0 )||( rc.m_SpDate2 == 0 )){
						CString strSpDate, strSpDate2;

						if( rc.m_SpDate == 0 )	{
							strSpDate.Empty();
						}
						else					{
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								strSpDate = ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_E1);
							}
							else					{
								strSpDate = ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_A1);
							}
						}

						if( rc.m_SpDate2 == 0 )	{
							strSpDate2.Empty();
						}
						else					{
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								strSpDate2 = ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_E1);
							}
							else					{
								strSpDate2 = ConvertDate(rc.m_SpDate2, PRT_FORMAT_DATE_A1);
							}
						}

						m_PrtDataBox[index + Field153SpDate0].Data[nCurRow] = strSpDate + strSpDate2;
					}
					else	{
						if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
							// 支払対象期間（開始）
							m_PrtDataBox[index + Field153SpDate].Data[nCurRow] =
								ConvertNullIntoSpace( ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E1 ), 7 ) +
								PRT_FORMAT_WAVE;

							// 支払対象期間（終了）
							m_PrtDataBox[index + Field153SpDate2].Data[nCurRow] =
								ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_E1 );
						}
						else					{
							// 支払対象期間（開始）
							m_PrtDataBox[index + Field153SpDate].Data[nCurRow] =
								ConvertNullIntoSpace( ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 ), 5 ) +
								PRT_FORMAT_WAVE;

							// 支払対象期間（終了）
							m_PrtDataBox[index + Field153SpDate2].Data[nCurRow] =
								ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_A1 );
						}
					}
// midori 200101 add <--

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 3 );
					m_PrtDataBox[index + Field153Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[index + Field153Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[index + Field153Teki3].Data[nCurRow] = strArray.GetAt( 2 );
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc153()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc150TidaiyatinKenrikinKougyou::CprtUc150TidaiyatinKenrikinKougyou(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_151;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/11/02 del -->
	//m_nColumn	= Field151Max;
// インボイス登録番号追加対応_23/11/02 del <--
// インボイス登録番号追加対応_23/11/02 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field151IMax;
	else					m_nColumn	= Field151Max;
// インボイス登録番号追加対応_23/11/02 add <--
}

CprtUc150TidaiyatinKenrikinKougyou::~CprtUc150TidaiyatinKenrikinKougyou(void)
{
}

CprtJobFunction* CprtUc150TidaiyatinKenrikinKougyou::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc150TidaiyatinKenrikinKougyou(m_pDB));
}