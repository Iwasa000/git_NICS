//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc161Zatueki.cpp
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
#include "prtUc161Zatueki.h"

extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/25 add

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc161()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑯-1．雑益等
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc161Zatueki::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;
	
	if(bG_InvNo == TRUE) {
		rv = GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} else {
		rv = GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

int CprtUc161Zatueki::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow = 0;
	int				nVisualPage = nPage;
	CStringArray	strArray;
	CString			strAdName = _T("");
	CString			strAdAdd = _T("");
	CString			strKnNameOld = _T("");
	CString			strNaiyouOld = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAddOld = _T("");
	CString			strSQL;
	CharController	cc;

	// 省略文字の取得
	GetPrintInfo(parent);

	CdbUc161Zatueki	rc(m_pDB);

	try {
		if (!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) {
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
					strNaiyouOld = _T("");
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

				// 金額
				m_PrtDataBox[Field161IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strKnNameOld = _T("");
					strNaiyouOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");

					m_PrtDataBox[Field161IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field161IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch (nItemSeq) {

							// 科目（名称）
						case 1:

							if (CheckOmit(rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field161IKnNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								CString strDividedName1, strDividedName2;
								// 分割後の文字列取得
								cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD);	// 修正No.168374 add
								//cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_INVOICE);		// 修正No.168374 del
								// 9文字以上なら2行表示
								if (!(strDividedName2.IsEmpty())) {
									// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
									if (strDividedName1.GetLength() == 15) {
										strDividedName1 += _T(" ");
									}
									// 半角スペースで16バイトまで埋める
									strDividedName2 = FillSpace(strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2);	// 修正No.168374 add
									//strDividedName2 = FillSpace(strDividedName2, DR_INPUTLENGTH_INVOICE * 2);		// 修正No.168374 del

									m_PrtDataBox[Field161IKnName1].Data[nCurRow] = strDividedName1;
									m_PrtDataBox[Field161IKnName2].Data[nCurRow] = strDividedName2;
								}
								else {
									m_PrtDataBox[Field161IKnName2].Data[nCurRow] = rc.m_KnName;
								}
							}

							strKnNameOld = rc.m_KnName;
							break;

							// 取引の内容
						case 2:

							if (CheckOmit(rc.m_Naiyou, strNaiyouOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field161INaiyouOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(rc.m_Naiyou, &strArray, 2);
								m_PrtDataBox[Field161INaiyou1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field161INaiyou2].Data[nCurRow] = strArray.GetAt(1);
							}

							strNaiyouOld = rc.m_Naiyou;
							break;

							// 相手先（名称）
						case 3:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field161IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[Field161IAdName1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field161IAdName2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 相手先（所在地）
						case 4:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field161IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[Field161IAdAdd1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field161IAdAdd2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		int nItem = Field161Max;

		return m_nRowMax;
	}
	catch (...) {

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc161()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/10/25 add <--

// インボイス登録番号追加対応_23/10/25 del -->
//int CprtUc161Zatueki::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc161Zatueki::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
// インボイス登録番号追加対応_23/10/25 add <--
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
	CString			strAdName		= _T( "" );
	CString			strAdAdd		= _T( "" );
	CString			strKnNameOld	= _T( "" );
	CString			strNaiyouOld	= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;
	CharController	cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc161Zatueki	rc( m_pDB );

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
					
					strKnNameOld	= _T( "" );
					strNaiyouOld	= _T( "" );
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

				// 金額
				m_PrtDataBox[Field161Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					strNaiyouOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[Field161KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 科目（名称）
							case 1:

								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field161KnNameOm].Data[nCurRow] =
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

										m_PrtDataBox[Field161KnName1].Data[nCurRow] = strDividedName1;
										m_PrtDataBox[Field161KnName2].Data[nCurRow] = strDividedName2;
									}
									else{
										m_PrtDataBox[Field161KnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

								strKnNameOld = rc.m_KnName;
								break;

							// 取引の内容
							case 2:

								if( CheckOmit( rc.m_Naiyou, strNaiyouOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field161NaiyouOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( rc.m_Naiyou, &strArray, 2 );
									m_PrtDataBox[Field161Naiyou1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field161Naiyou2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strNaiyouOld = rc.m_Naiyou;
								break;

							// 相手先（名称）
							case 3:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field161AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field161AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field161AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 相手先（所在地）
							case 4:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field161AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field161AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field161AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							default:
								break;
						}
					}
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		int nItem = Field161Max;

		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc161()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}


CprtUc161Zatueki::CprtUc161Zatueki(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_161;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/25 del -->
	//m_nColumn	= Field161Max;
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field161IMax;
	else					m_nColumn	= Field161Max;
// インボイス登録番号追加対応_23/10/25 add <--
}

CprtUc161Zatueki::~CprtUc161Zatueki(void)
{
}

CprtJobFunction* CprtUc161Zatueki::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc161Zatueki(m_pDB));
}