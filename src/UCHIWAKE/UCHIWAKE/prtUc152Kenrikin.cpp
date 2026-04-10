//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc152Kenrikin.cpp
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
#include "prtUc152Kenrikin.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--

extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/25 add

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
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc152Kenrikin::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;

	if(bG_InvNo == TRUE) {
		rv = GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} 
	else {
		rv = GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

int CprtUc152Kenrikin::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
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
	GetPrintInfo(parent);

	CdbUc152Kenrikin	rc(m_pDB);

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
					strNaiyouOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[0]) {
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
				else {
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
// インボイス登録番号追加対応_23/10/25 add <--

// インボイス登録番号追加対応_23/10/25 del -->
//int CprtUc152Kenrikin::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc152Kenrikin::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
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
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strNaiyouOld	= _T( "" );
	CString			strSQL;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc152Kenrikin	rc( m_pDB );

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
					
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strNaiyouOld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
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

CprtUc152Kenrikin::CprtUc152Kenrikin(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_152;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/25 del -->
	//m_nColumn	= Field152Max;
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field152IMax;
	else					m_nColumn	= Field152Max;
// インボイス登録番号追加対応_23/10/25 add <--
}

CprtUc152Kenrikin::~CprtUc152Kenrikin(void)
{
}

CprtJobFunction* CprtUc152Kenrikin::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc152Kenrikin(m_pDB));
}
