//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc160ZatuekiZatuson.cpp
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
#include "prtUc160ZatuekiZatuson.h"

extern BOOL bG_InvNo;	// インボイス登録番号追加対応_23/11/01 add


int CprtUc160ZatuekiZatuson::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int nRowMax		= 0;
	int nRowMax2	= 0;
// インボイス登録番号追加対応_23/11/01 del -->
	//if(po.nPageNum[0] != 0)	nRowMax	 = GetDbDataUc161( nPage, parent, po );
	//if(po.nPageNum[1] != 0)	nRowMax2 = GetDbDataUc162( nPage, parent, po );

	//m_nColumn = Field161Max;
// インボイス登録番号追加対応_23/11/01 del <--
// インボイス登録番号追加対応_23/11/01 add -->
	if(po.nPageNum[0] != 0) {
		if(bG_InvNo == TRUE)	nRowMax = GetDbDataUc161New(nPage, parent, po);
		else					nRowMax = GetDbDataUc161Old(nPage, parent, po);
	}
	if(po.nPageNum[1] != 0) {
		if(bG_InvNo == TRUE)	nRowMax2 = GetDbDataUc162New(nPage, parent, po);
		else					nRowMax2 = GetDbDataUc162Old(nPage, parent, po);
	}

	if(bG_InvNo == TRUE)	m_nColumn = Field161IMax;
	else					m_nColumn = Field161Max;
// インボイス登録番号追加対応_23/11/01 add <--

	// 行数の多い方を使用
	if( nRowMax2 > nRowMax ){
		nRowMax = nRowMax2;
	}

	m_nRowMax = nRowMax;

// インボイス登録番号追加対応_23/11/01 del -->
	//m_nColumn += Field162Max;
// インボイス登録番号追加対応_23/11/01 del -->
// インボイス登録番号追加対応_23/11/01 add -->
	if(bG_InvNo == TRUE)	m_nColumn += Field162IMax;
	else					m_nColumn += Field162Max;
// インボイス登録番号追加対応_23/11/01 add -->

	return 0;
}

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
// インボイス登録番号追加対応_23/11/01 add -->
int CprtUc160ZatuekiZatuson::GetDbDataUc161New(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
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
	m_nFormSeq = ID_FORMNO_161;
	m_nColumn = Field161IMax;
	GetPrintInfo(parent);

	CdbUc161Zatueki	rc(m_pDB);

	try {
		if ((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || m_nOutZero == 1) {
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
								cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD);
								// 9文字以上なら2行表示
								if (!(strDividedName2.IsEmpty())) {
									// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
									if (strDividedName1.GetLength() == 15) {
										strDividedName1 += _T(" ");
									}
									// 半角スペースで16バイトまで埋める
									strDividedName2 = FillSpace(strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2);

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

		int nItem = Field161IMax;

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
// インボイス登録番号追加対応_23/11/01 add <--

// インボイス登録番号追加対応_23/11/01 del -->
//int CprtUc160ZatuekiZatuson::GetDbDataUc161(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/01 del <--
// インボイス登録番号追加対応_23/11/01 add -->
int CprtUc160ZatuekiZatuson::GetDbDataUc161Old(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/01 add <--
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
	m_nFormSeq	= ID_FORMNO_161;
	m_nColumn	= Field161Max;
	GetPrintInfo( parent );

	CdbUc161Zatueki	rc( m_pDB );

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
//		if(((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || m_nOutZero == 1) || (po.OutZeroNull[0] != 0 && rt == 2))	{
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

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc162()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑯-2．雑損失等
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/11/01 add -->
int CprtUc160ZatuekiZatuson::GetDbDataUc162New(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				index = 0;
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
	m_nFormSeq = ID_FORMNO_162;
	m_nColumn = Field162IMax;
	GetPrintInfo(parent);

	// データボックス内の格納場所を変更
	index = Field161IMax;

	CdbUc162Zatuson	rc(m_pDB);

	try {
		if ((!(po.OutZeroNull[1]) && (po.KamokuSitei[1] == 0)) || m_nOutZero == 1) {
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
					if (po.HideZero[1]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
					}
				}

				// 金額
				m_PrtDataBox[index + Field162IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strKnNameOld = _T("");
					strNaiyouOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");

					m_PrtDataBox[index + Field162IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[index + Field162IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList2[i].ItemSeq;

						switch (nItemSeq) {

							// 科目（名称）
						case 1:

							if (CheckOmit(rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field162IKnNameOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
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

									m_PrtDataBox[index + Field162IKnName1].Data[nCurRow] = strDividedName1;
									m_PrtDataBox[index + Field162IKnName2].Data[nCurRow] = strDividedName2;
								}
								else {
									m_PrtDataBox[index + Field162IKnName2].Data[nCurRow] = rc.m_KnName;
								}
							}

							strKnNameOld = rc.m_KnName;
							break;

							// 取引の内容
						case 2:

							if (CheckOmit(rc.m_Naiyou, strNaiyouOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field162INaiyouOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {

								StrDivision(rc.m_Naiyou, &strArray, 2);
								m_PrtDataBox[index + Field162INaiyou1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field162INaiyou2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strNaiyouOld = rc.m_Naiyou;
							break;

							// 相手先（名称）
						case 3:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field162IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[index + Field162IAdName1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field162IAdName2].Data[nCurRow] =
									strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 相手先（所在地）
						case 4:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList2)) {

								m_PrtDataBox[index + Field162IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList2[i].OmitStr;
							}
							else {

								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[index + Field162IAdAdd1].Data[nCurRow] =
									strArray.GetAt(0);
								m_PrtDataBox[index + Field162IAdAdd2].Data[nCurRow] =
									strArray.GetAt(1);
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
// インボイス登録番号追加対応_23/11/01 add <--

// インボイス登録番号追加対応_23/11/01 del -->
//int CprtUc160ZatuekiZatuson::GetDbDataUc162(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/01 del <--
// インボイス登録番号追加対応_23/11/01 add -->
int CprtUc160ZatuekiZatuson::GetDbDataUc162Old(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/11/01 add <--
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
	CString			strKnNameOld	= _T( "" );
	CString			strNaiyouOld	= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strAdAddOld		= _T( "" );
	CString			strSQL;
	CharController	cc;

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_162;
	m_nColumn	= Field162Max;
	GetPrintInfo( parent );

	// データボックス内の格納場所を変更
	index		= Field161Max;

	CdbUc162Zatuson	rc( m_pDB );

	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 160608,160610 cor -->
//////		if(!(po.OutZeroNull[1])){
////// ----------------------------
////		if((!(po.OutZeroNull[1]) && (po.KamokuSitei[1] == 0)) || m_nOutZero == 1){
////// midori 160608,160610 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[1] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[1] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(m_nFormSeq, ck1, ck2);
//
//		if(((!(po.OutZeroNull[1]) && (po.KamokuSitei[1] == 0)) || m_nOutZero == 1) || (po.OutZeroNull[1] != 0 && rt == 2))	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if((!(po.OutZeroNull[1]) && (po.KamokuSitei[1] == 0)) || m_nOutZero == 1){
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
					if(po.HideZero[1]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 金額
				m_PrtDataBox[index + Field162Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					strNaiyouOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );

					m_PrtDataBox[index + Field162KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList2[i].ItemSeq;

						switch( nItemSeq ){

							// 科目（名称）
							case 1:

								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field162KnNameOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
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

										m_PrtDataBox[index + Field162KnName1].Data[nCurRow] = strDividedName1;
										m_PrtDataBox[index + Field162KnName2].Data[nCurRow] = strDividedName2;
									}
									else{
										m_PrtDataBox[index + Field162KnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

								strKnNameOld = rc.m_KnName;
								break;

							// 取引の内容
							case 2:

								if( CheckOmit( rc.m_Naiyou, strNaiyouOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field162NaiyouOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{

									StrDivision( rc.m_Naiyou, &strArray, 2 );
									m_PrtDataBox[index + Field162Naiyou1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field162Naiyou2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strNaiyouOld = rc.m_Naiyou;
								break;

							// 相手先（名称）
							case 3:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field162AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[index + Field162AdName1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field162AdName2].Data[nCurRow] =
										strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 相手先（所在地）
							case 4:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList2 ) ){

									m_PrtDataBox[index + Field162AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList2[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[index + Field162AdAdd1].Data[nCurRow] =
										strArray.GetAt( 0 );
									m_PrtDataBox[index + Field162AdAdd2].Data[nCurRow] =
										strArray.GetAt( 1 );
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

CprtUc160ZatuekiZatuson::CprtUc160ZatuekiZatuson(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_161;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/11/01 del -->
	//m_nColumn	= Field161Max;
// インボイス登録番号追加対応_23/11/01 del <--
// インボイス登録番号追加対応_23/11/01 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field161IMax;
	else					m_nColumn	= Field161Max;
// インボイス登録番号追加対応_23/11/01add <--
}

CprtUc160ZatuekiZatuson::~CprtUc160ZatuekiZatuson(void)
{
}

CprtJobFunction* CprtUc160ZatuekiZatuson::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc160ZatuekiZatuson(m_pDB));
}
