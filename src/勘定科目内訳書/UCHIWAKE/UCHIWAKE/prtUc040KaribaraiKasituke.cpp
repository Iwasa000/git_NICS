//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc040KaribaraiKasituke.cpp
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
#include "prtUc040KaribaraiKasituke.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--
extern BOOL	bG_InvNo;		// インボイス登録番号追加対応_23/10/31 add

// インボイス登録番号追加対応_23/10/31 del -->
//int CprtUc040KaribaraiKasituke::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
//{
//	int nRowMax		= 0;
//	int nRowMax2	= 0;
//
//	if(po.nPageNum[0] != 0)	nRowMax		= GetDbDataUc041( nPage, parent, po );
//	if(po.nPageNum[1] != 0)	nRowMax2	= GetDbDataUc042( nPage, parent, po );
//
//// midori UC_0024 add -->
//	if(nRowMax > 0 && nRowMax2 == 0)	{
//		if(po.NameKei[1] != 0)	{
//// midori 190301 del -->
//			//m_PrtDataBox[Field041Max+Field042KeiStr].Data[PRT_ROW_042_NORMAL-1] = NAME_GOUKEI;
//// midori 190301 del <--
//// midori 190301 add -->
//			m_PrtDataBox[Field041Max+Field042DnKeiStr].Data[PRT_ROW_042_NORMAL-1] = NAME_GOUKEI;
//// midori 190301 add <--
//		
//		}
//		else	{
//// midori 190301 del -->
//			//m_PrtDataBox[Field041Max+Field042KeiStr].Data[PRT_ROW_042_NORMAL-1] = NAME_KEI;
//// midori 190301 del <--
//// midori 190301 add -->
//			m_PrtDataBox[Field041Max+Field042DnKeiStr].Data[PRT_ROW_042_NORMAL-1] = NAME_KEI;
//// midori 190301 add <--
//		}
//	}
//// midori UC_0024 add <--
//
//	m_nColumn = Field041Max;
//
//	// 行数の多い方を使用
//	if( nRowMax2 > nRowMax ){
//
//		nRowMax = nRowMax2;
//	}
//
//	m_nRowMax = nRowMax;
//	m_nColumn += Field042Max;
//
//	return 0;
//}
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
int CprtUc040KaribaraiKasituke::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int nRowMax		= 0;
	int nRowMax2	= 0;

	if(po.nPageNum[0] != 0) {
		if(bG_InvNo == TRUE)	nRowMax = GetDbDataUc041New(nPage, parent, po);
		else					nRowMax = GetDbDataUc041Old(nPage, parent, po);
	}
	if(po.nPageNum[1] != 0) {
		if(bG_InvNo == TRUE)	nRowMax2 = GetDbDataUc042New(nPage, parent, po);
		else					nRowMax2 = GetDbDataUc042Old(nPage, parent, po);
	}

	if(nRowMax > 0 && nRowMax2 == 0)	{
		if(po.NameKei[1] != 0)	{
			if(bG_InvNo == TRUE) {
				m_PrtDataBox[Field041IMax + Field042IDnKeiStr].Data[PRT_ROW_042_NORMAL - 1] = NAME_GOUKEI;
			}
			else {
				m_PrtDataBox[Field041Max + Field042DnKeiStr].Data[PRT_ROW_042_NORMAL - 1] = NAME_GOUKEI;
			}
		
		}
		else	{
			if(bG_InvNo == TRUE) {
				m_PrtDataBox[Field041IMax + Field042IDnKeiStr].Data[PRT_ROW_042_NORMAL - 1] = NAME_KEI;
			}
			else {
				m_PrtDataBox[Field041Max + Field042DnKeiStr].Data[PRT_ROW_042_NORMAL - 1] = NAME_KEI;
			}
		}
	}

	if(bG_InvNo == TRUE)	m_nColumn = Field041IMax;
	else					m_nColumn = Field041Max;

	// 行数の多い方を使用
	if( nRowMax2 > nRowMax ){
		nRowMax = nRowMax2;
	}

	m_nRowMax = nRowMax;

	if(bG_InvNo == TRUE)	m_nColumn += Field042IMax;
	else					m_nColumn += Field042Max;

	return 0;
}
// インボイス登録番号追加対応_23/10/31 add <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc041()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ④-1．仮払金
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号追加対応_23/10/31 add -->
int CprtUc040KaribaraiKasituke::GetDbDataUc041New(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				nVisualPage = nPage;			//見かけ上のページ数
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
	m_nFormSeq = ID_FORMNO_041;
	m_nColumn = Field041IMax;
	GetPrintInfo(parent);

	CdbUc041Karibarai	rc(m_pDB);

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
				m_PrtDataBox[Field041IVal].Data[nCurRow] =
					ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strKnNameOld = _T("");
					strAdNameOld = _T("");
					strAdAddOld = _T("");
					strKankeiOld = _T("");

					m_PrtDataBox[Field041IKeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else {
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field041IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for (int i = 0; i < PRT_MAX_OMIT; i++) {

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch (nItemSeq) {

							// 科目（名称）
						case 1:

							if (CheckOmit(rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field041IKnNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								CString strDividedName1, strDividedName2;
								// 分割後の文字列取得
								cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD);	// 修正No.168432 add
								//cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_INVOICE);		// 修正No.168432 del
								// 9文字以上なら2行表示
								if (!(strDividedName2.IsEmpty())) {
									// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
									if (strDividedName1.GetLength() == 15) {
										strDividedName1 += _T(" ");
									}
									// 半角スペースで16バイトまで埋める
									strDividedName2 = FillSpace(strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2);	// 修正No.168374 add
									//strDividedName2 = FillSpace(strDividedName2, DR_INPUTLENGTH_INVOICE * 2);		// 修正No.168374 del

									m_PrtDataBox[Field041IKnName1].Data[nCurRow] = strDividedName1;
									m_PrtDataBox[Field041IKnName2].Data[nCurRow] = strDividedName2;
								}
								else {
									m_PrtDataBox[Field041IKnName2].Data[nCurRow] = rc.m_KnName;
								}
							}

							strKnNameOld = rc.m_KnName;
							break;

							// 相手先（名称）
						case 2:

							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field041IAdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[Field041IAdName1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field041IAdName2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdNameOld = strAdName;
							break;

							// 相手先（所在地）
						case 3:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if (CheckOmit(strAdAdd, strAdAddOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field041IAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {
								StrDivision(strAdAdd, &strArray, 2);
								m_PrtDataBox[Field041IAdAdd1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field041IAdAdd2].Data[nCurRow] = strArray.GetAt(1);
							}

							strAdAddOld = strAdAdd;
							break;

							// 法人・代表者との関係
						case 4:

							if (CheckOmit(rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field041IKankeiOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {

								m_PrtDataBox[Field041IKankei].Data[nCurRow] = rc.m_Kankei;
							}

							strKankeiOld = rc.m_Kankei;
							break;

						default:
							break;
						}
					}

					// 取引の内容
					StrDivision(rc.m_Naiyou, &strArray, 2);
					m_PrtDataBox[Field041INaiyou1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[Field041INaiyou2].Data[nCurRow] = strArray.GetAt(1);
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

		TRACE("\n# ERROR: CJobFunction::GetDbDataUc041()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/10/31 add <--

// インボイス登録番号追加対応_23/10/31 del -->
//int CprtUc040KaribaraiKasituke::GetDbDataUc041(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
int CprtUc040KaribaraiKasituke::GetDbDataUc041Old(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/10/31 add <--
{
	int				nCurRow			= 0;
	int				nVisualPage		= nPage;			//見かけ上のページ数
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
	m_nFormSeq	= ID_FORMNO_041;
	m_nColumn	= Field041Max;
	GetPrintInfo( parent );

	CdbUc041Karibarai	rc( m_pDB );

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
//		if((!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)) || m_nOutZero == 1 || (po.OutZeroNull[0] != 0 && rt == 2))	{
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
					if( po.HideZero[0] ){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 期末現在高
				m_PrtDataBox[Field041Val].Data[nCurRow] = 
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strKankeiOld	= _T( "" );

					m_PrtDataBox[Field041KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{

					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 科目（名称）
							case 1:

								if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field041KnNameOm].Data[nCurRow] =
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

										m_PrtDataBox[Field041KnName1].Data[nCurRow] = strDividedName1;
										m_PrtDataBox[Field041KnName2].Data[nCurRow] = strDividedName2;
									}
									else{
										m_PrtDataBox[Field041KnName2].Data[nCurRow] = rc.m_KnName;
									}
								}

								strKnNameOld = rc.m_KnName;
								break;

							// 相手先（名称）
							case 2:
								
								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field041AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									
									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field041AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field041AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 相手先（所在地）
							case 3:
								
								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field041AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field041AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field041AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							// 法人・代表者との関係
							case 4:
								
								if( CheckOmit( rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field041KankeiOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									m_PrtDataBox[Field041Kankei].Data[nCurRow] = rc.m_Kankei;
								}

								strKankeiOld = rc.m_Kankei;
								break;

							default:
								break;
						}
					}

					// 取引の内容
					StrDivision( rc.m_Naiyou, &strArray, 2 );
					m_PrtDataBox[Field041Naiyou1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field041Naiyou2].Data[nCurRow] = strArray.GetAt( 1 );
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc041()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

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
// インボイス登録番号追加対応_23/10/31 add -->
int CprtUc040KaribaraiKasituke::GetDbDataUc042New(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				nLastRow = PRT_ROW_042_KOBETU;
	int				index = 0;
	int				nRisokuKei[4] = { 0 };
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
	m_nFormSeq = ID_FORMNO_042;
	m_nColumn = Field042IMax;

	GetPrintInfo(parent);

	// データボックス内の格納場所を変更
	nLastRow = PRT_ROW_042_NORMAL;
	index = Field041IMax;

	CdbUc042Kasituke	rc(m_pDB);

	// 計の出力設定
	int nOutKei = rc.GetOutKei(ID_FORMNO_042);

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
					strKankeiOld = _T("");
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[1]) {
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
									m_PrtDataBox[index + Field042IAdAdd1].Data[nCurRow] = strArray.GetAt(0);
									m_PrtDataBox[index + Field042IAdAdd2].Data[nCurRow] = strArray.GetAt(1);
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
// インボイス登録番号追加対応_23/10/31 add -->

// インボイス登録番号追加対応_23/10/31 del -->
//int CprtUc040KaribaraiKasituke::GetDbDataUc042(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
int CprtUc040KaribaraiKasituke::GetDbDataUc042Old(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
// インボイス登録番号追加対応_23/10/31 add <--
{
	int				nCurRow			= 0;
	int				nLastRow		= PRT_ROW_042_KOBETU;
	int				index			= 0;
	int				nRisokuKei[4]	= {0};
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
	m_nFormSeq	= ID_FORMNO_042;
	m_nColumn	= Field042Max;

	GetPrintInfo( parent );

	// データボックス内の格納場所を変更
	nLastRow	= PRT_ROW_042_NORMAL;
	index		= Field041Max;

	CdbUc042Kasituke	rc( m_pDB );

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_042 );

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
					
					strAdNameOld	= _T ( "" );
					strAdAddOld		= _T ( "" );
					strKankeiOld	= _T ( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if( po.HideZero[1] ){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_Risoku = HideZeroMoney( rc.m_Risoku );
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
										m_PrtDataBox[index + Field042AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
										m_PrtDataBox[index + Field042AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
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
//										m_PrtDataBox[index + Field042Kankei].Data[nCurRow] = rc.m_Kankei;
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

CprtUc040KaribaraiKasituke::CprtUc040KaribaraiKasituke(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_041;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/31 del -->
	//m_nColumn	= Field041Max;
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field041IMax;
	else					m_nColumn	= Field041Max;
// インボイス登録番号追加対応_23/10/31 add <--
}

CprtUc040KaribaraiKasituke::~CprtUc040KaribaraiKasituke(void)
{
}

CprtJobFunction* CprtUc040KaribaraiKasituke::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc040KaribaraiKasituke(m_pDB));
}