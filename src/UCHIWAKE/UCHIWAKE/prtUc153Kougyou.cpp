//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc153Kougyou.cpp
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
#include "prtUc153Kougyou.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--

extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/25 add

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
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc153Kougyou::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;

	if(bG_InvNo == TRUE) {
		rv = GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} else {
		rv = GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

int CprtUc153Kougyou::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
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
	GetPrintInfo(parent);

	CdbUc153Kougyou	rc(m_pDB);

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

					strHdNameOld = _T("");
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

						m_PrtDataBox[Field153IKyDate0].Data[nCurRow] = strKyDate + strKyDate2;
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

						m_PrtDataBox[Field153ISpDate0].Data[nCurRow] = strSpDate + strSpDate2;
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
// インボイス登録番号追加対応_23/10/25 add <--

// インボイス登録番号追加対応_23/10/25 del -->
//int CprtUc153Kougyou::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc153Kougyou::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 add <--
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
	GetPrintInfo( parent );

	CdbUc153Kougyou	rc( m_pDB );

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
					
					strHdNameOld	= _T( "" );
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

						m_PrtDataBox[Field153KyDate0].Data[nCurRow] = strKyDate + strKyDate2;
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

					//	m_PrtDataBox[Field153SpDate0].Data[nCurRow] = strSpDate + strSpDate2;
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
								strSpDate = ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E1 );
							}
							else					{
								strSpDate = ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 );
							}
						}

						if( rc.m_SpDate2 == 0 )	{
							strSpDate2.Empty();
						}
						else					{
							if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
								strSpDate2 = ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_E1 );
							}
							else					{
								strSpDate2 = ConvertDate( rc.m_SpDate2, PRT_FORMAT_DATE_A1 );
							}
						}

						m_PrtDataBox[Field153SpDate0].Data[nCurRow] = strSpDate + strSpDate2;
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
						else	{
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

CprtUc153Kougyou::CprtUc153Kougyou(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_153;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/25 del -->
	//m_nColumn	= Field153Max;
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field153IMax;
	else					m_nColumn	= Field153Max;
// インボイス登録番号追加対応_23/10/25 add <--
}

CprtUc153Kougyou::~CprtUc153Kougyou(void)
{
}

CprtJobFunction* CprtUc153Kougyou::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc153Kougyou(m_pDB));
}