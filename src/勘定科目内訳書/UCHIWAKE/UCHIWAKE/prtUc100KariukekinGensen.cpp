//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc100KariukekinGensen.cpp
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
#include "prtUc100KariukekinGensen.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--
extern BOOL bG_InvNo;	// インボイス登録番号追加対応_23/10/31 add

int CprtUc100KariukekinGensen::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int nRowMax		= 0;
	int nRowMax2	= 0;
// インボイス登録番号追加対応_23/10/31 del -->
	//if(po.nPageNum[0] != 0)	nRowMax	 = GetDbDataUc101( nPage, parent, po );
	//if(po.nPageNum[1] != 0)	nRowMax2 = GetDbDataUc102( nPage, parent, po );

	//m_nColumn = Field101Max;

	//// 行数の多い方を使用
	//if( nRowMax2 > nRowMax ){

	//	nRowMax = nRowMax2;
	//}

	//m_nRowMax = nRowMax;
	//m_nColumn += Field102Max;
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
	if(po.nPageNum[0] != 0) {
		if(bG_InvNo == TRUE)	nRowMax = GetDbDataUc101New(nPage, parent, po);
		else					nRowMax = GetDbDataUc101Old(nPage, parent, po);
	}

	if(po.nPageNum[1] != 0)		{
		nRowMax2 = GetDbDataUc102(nPage, parent, po);
	}

	if(bG_InvNo == TRUE)	m_nColumn = Field101IMax;
	else					m_nColumn = Field101Max;

	// 行数の多い方を使用
	if( nRowMax2 > nRowMax ){
		nRowMax = nRowMax2;
	}

	m_nRowMax = nRowMax;
	m_nColumn += Field102Max;	
// インボイス登録番号追加対応_23/10/31 add <--

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc101()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑩-1．仮受金
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/10/31 add -->
int CprtUc100KariukekinGensen::GetDbDataUc101New(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				nVisualPage = nPage;
	CStringArray	strArray;
	CString			strAdName = _T("");
	CString			strAdAdd = _T("");
	CString			strKnNameOld = _T("");
	CString			strAdNameOld = _T("");
	CString			strAdAddOld = _T("");
	CString			strKankeiOld = _T("");
	CString			strSQL;
	CharController	cc;

	// 省略文字の取得
	m_nFormSeq = ID_FORMNO_101;
	m_nColumn = Field101IMax;
	GetPrintInfo(parent);

	CdbUc101Kariukekin	rc(m_pDB);

	try {
		if ((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || m_nOutZero == 1) {
			// 指定されたページの行数を取得
			m_nRowMax = rc.GetNumRow(nPage);

			rc.Init(nPage);
		}
		else {
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl(m_nFormSeq);

			// ---------------------
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
					strAdNameOld = _T("");
					strAdAddOld = _T("");
					strKankeiOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[0]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
					}
				}

				// 期末現在高
				m_PrtDataBox[Field101IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strKnNameOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");
					strKankeiOld = _T("");

					m_PrtDataBox[Field101IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field101IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch (nItemSeq) {

							// 科目（名称）
						case 1:

							if (CheckOmit(rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field101IKnNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								CString strDividedName1, strDividedName2;
								// 分割後の文字列取得
								cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD);
								// 9文字以上なら2行表示
								if (!(strDividedName2.IsEmpty())) {
									// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
									if (strDividedName1.GetLength() == 15) {
										strDividedName1 += _T(" ");
									}
									// 半角スペースで16バイトまで埋める
									strDividedName2 = FillSpace(strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2);

									m_PrtDataBox[Field101IKnName1].Data[nCurRow] = strDividedName1;
									m_PrtDataBox[Field101IKnName2].Data[nCurRow] = strDividedName2;
								}
								else {
									m_PrtDataBox[Field101IKnName2].Data[nCurRow] = rc.m_KnName;
								}
							}

							strKnNameOld = rc.m_KnName;
							break;

							// 相手先（名称）
						case 2:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field101IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[Field101IAdName1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field101IAdName2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 相手先（所在地）
						case 3:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field101IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								//									strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;
								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[Field101IAdAdd1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field101IAdAdd2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

							// 法人・代表者との関係
						case 4:

							if (CheckOmit(rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field101IKankeiOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								m_PrtDataBox[Field101IKankei].Data[nCurRow] = rc.m_Kankei;
							}

							strKankeiOld = rc.m_Kankei;
							break;

						default:
							break;
						}
					}

					// 取引の内容
					StrDivision(rc.m_Naiyou, &strArray, 2);
					m_PrtDataBox[Field101INaiyou1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[Field101INaiyou2].Data[nCurRow] = strArray.GetAt(1);
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

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc101()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/10/31 add <--

// インボイス登録番号追加対応_23/10/31 del -->
//int CprtUc100KariukekinGensen::GetDbDataUc101(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
int CprtUc100KariukekinGensen::GetDbDataUc101Old(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/10/31 add <--
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
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strKankeiOld	= _T( "" );
	CString			strSQL;
	CharController	cc;

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_101;
	m_nColumn	= Field101Max;
	GetPrintInfo( parent );

	CdbUc101Kariukekin	rc( m_pDB );

	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 160608,160610 cor -->
//////		if(!(po.OutZeroNull[0])){
////// ----------------------------
////		if((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || m_nOutZero == 1){
////// midori 160608,160610 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[0] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[0] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(m_nFormSeq, ck1, ck2);
//
//		if(((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || m_nOutZero == 1)|| (po.OutZeroNull[0] != 0 && rt == 2))	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || m_nOutZero == 1){
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
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strKankeiOld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 期末現在高
				m_PrtDataBox[Field101Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strKankeiOld	= _T( "" );

					m_PrtDataBox[Field101KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 科目（名称）
							case 1:

								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field101KnNameOm].Data[nCurRow] =
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

										m_PrtDataBox[Field101KnName1].Data[nCurRow] = strDividedName1;
										m_PrtDataBox[Field101KnName2].Data[nCurRow] = strDividedName2;
									}
									else{
										m_PrtDataBox[Field101KnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

								strKnNameOld = rc.m_KnName;
								break;

							// 相手先（名称）
							case 2:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field101AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									
//									strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field101AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field101AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 相手先（所在地）
							case 3:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field101AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

//									strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;
									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field101AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field101AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							// 法人・代表者との関係
							case 4:

								if( CheckOmit( rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field101KankeiOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									m_PrtDataBox[Field101Kankei].Data[nCurRow] = rc.m_Kankei;
								}

								strKankeiOld = rc.m_Kankei;
								break;

							default:
								break;
						}
					}

					// 取引の内容
					StrDivision( rc.m_Naiyou, &strArray, 2 );
					m_PrtDataBox[Field101Naiyou1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field101Naiyou2].Data[nCurRow] = strArray.GetAt( 1 );
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc101()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc102()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑩-2．源泉所得税預り金
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc100KariukekinGensen::GetDbDataUc102(int nPage, CprtJobFunction* parent, const PRT_OPTION& po )
{
	int				nCurRow			= 0;
	int				nFirstRow		= PRT_ROW_102_KOBETU;
	int				index			= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CString			strKnNameLOld	= _T( "" );
	CString			strKnNameROld	= _T( "" );
	CString			strSQL;

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_102;
	m_nColumn	= Field102Max;
	GetPrintInfo( parent );

	// データボックス内の格納場所を変更
	nFirstRow	= PRT_ROW_102_NORMAL;
// インボイス登録番号追加対応_23/10/31 del -->
	//index		= Field101Max;
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
	if(bG_InvNo == TRUE)	index	= Field101IMax;
	else					index	= Field101Max;
// インボイス登録番号追加対応_23/10/31 add <--

	CdbUc102Gensen	rc( m_pDB );

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
//		rt = rc.GetCountDataRecord2(m_nFormSeq, ck1, ck2);
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
					
					strKnNameLOld	= _T( "" );
					strKnNameROld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[1]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}
				
				// 左列
				if( nCurRow < nFirstRow ){

					// 計行
					if(	CheckDbDataFgFuncKei( &rc ) ){

						strKnNameLOld	= _T( "" );
						strKnNameROld	= _T( "" );
						m_PrtDataBox[index + Field102KeiL].Data[nCurRow] = rc.m_KeiStr;

						// 期末現在高（左列）
// No.200903 del -->
						//m_PrtDataBox[index + Field102ValL].Data[nCurRow] =
						//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
						if(bG_Kanso == TRUE)	{
							m_PrtDataBox[index + Field102ValL].Data[nCurRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
						}
						else	{
							m_PrtDataBox[index + Field102ValL].Data[nCurRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
						}
// No.200903 add <--
					}
					else{
						// 省略文字使用欄の判別
						// 所得の種類（左列）
						if( CheckOmit( rc.m_KnName, strKnNameLOld, 0, m_PrtInfOmitList2 ) ){

							m_PrtDataBox[index + Field102KnNameLOm].Data[nCurRow] =
								m_PrtInfOmitList2[0].OmitStr;
						}
						else{

							m_PrtDataBox[index + Field102KnNameL].Data[nCurRow] =
								rc.m_KnName;
						}
						strKnNameLOld = rc.m_KnName;

						// 年月日（左列）
// midori 200101 del -->
						//m_PrtDataBox[index + Field102SpDateL].Data[nCurRow] =
						//	ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 );
// midori 200101 del <--
// midori 200101 add -->
						if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
							m_PrtDataBox[index + Field102SpDateL].Data[nCurRow] =
								ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E1 );
						}
						else	{
							m_PrtDataBox[index + Field102SpDateL].Data[nCurRow] =
								ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 );
						}
// midori 200101 add <--

						// 期末現在高（左列）
// No.200903 del -->
						//m_PrtDataBox[index + Field102ValL].Data[nCurRow] =
						//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
						if(bG_Kanso == TRUE)	{
							m_PrtDataBox[index + Field102ValL].Data[nCurRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
						}
						else	{
							m_PrtDataBox[index + Field102ValL].Data[nCurRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
						}
// No.200903 add <--
					}
				}
				// 右列
				else{
					// 計行
					if(	CheckDbDataFgFuncKei( &rc ) ){

						strKnNameLOld	= _T( "" );
						strKnNameROld	= _T( "" );
						m_PrtDataBox[index + Field102KeiR].Data[nCurRow - nFirstRow] = rc.m_KeiStr;

						// 期末現在高（右列）
// No.200903 del -->
						//m_PrtDataBox[index + Field102ValR].Data[nCurRow - nFirstRow] =
						//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
						if(bG_Kanso == TRUE)	{
							m_PrtDataBox[index + Field102ValR].Data[nCurRow - nFirstRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
						}
						else	{
							m_PrtDataBox[index + Field102ValR].Data[nCurRow - nFirstRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
						}
// No.200903 add <--
					}
					else{
						// 省略文字使用欄の判別
						// 所得の種類（右列）
						if( CheckOmit( rc.m_KnName, strKnNameROld, 0, m_PrtInfOmitList2 ) ){

							m_PrtDataBox[index + Field102KnNameROm].Data[nCurRow - nFirstRow] =
								m_PrtInfOmitList2[0].OmitStr;
						}
						else{

							m_PrtDataBox[index + Field102KnNameR].Data[nCurRow - nFirstRow] =
								rc.m_KnName;
						}
						strKnNameROld = rc.m_KnName;

						// 年月日（右列）
// midori 200101 del -->
						//m_PrtDataBox[index + Field102SpDateR].Data[nCurRow - nFirstRow] =
						//	ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 );
// midori 200101 del <--
// midori 200101 add -->
						if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
							m_PrtDataBox[index + Field102SpDateR].Data[nCurRow - nFirstRow] =
								ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E1 );
						}
						else	{
							m_PrtDataBox[index + Field102SpDateR].Data[nCurRow - nFirstRow] =
								ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_A1 );
						}
// midori 200101 add <--

						// 期末現在高（右列）
// No.200903 del -->
						//m_PrtDataBox[index + Field102ValR].Data[nCurRow - nFirstRow] =
						//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
						if(bG_Kanso == TRUE)	{
							m_PrtDataBox[index + Field102ValR].Data[nCurRow - nFirstRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
						}
						else	{
							m_PrtDataBox[index + Field102ValR].Data[nCurRow - nFirstRow] =
								ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
						}
// No.200903 add <--
					}
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc102()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc100KariukekinGensen::CprtUc100KariukekinGensen(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_101;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/31 del <--
	//m_nColumn	= Field101Max;
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field101IMax;
	else					m_nColumn	= Field101Max;
// インボイス登録番号追加対応_23/10/31 add <--
}

CprtUc100KariukekinGensen::~CprtUc100KariukekinGensen(void)
{
}

CprtJobFunction* CprtUc100KariukekinGensen::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc100KariukekinGensen(m_pDB));
}