//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc140YakuinJinkenhi.cpp
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
#include "prtUc140YakuinJinkenhi.h"

extern BOOL	bG_Kanso;		// midori 190301 add
extern BOOL bG_InvNo;		// 調査依頼20240712_24/09/19 add

int CprtUc140YakuinJinkenhi::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int nRowMax		= 0;
	int nRowMax2	= 0;
// midori UC_0024 add -->
	int Nod141		= 0;
// midori UC_0024 add <--

// 調査依頼20240712_24/09/19 cor -->
//	if(po.nPageNum[0] != 0)	nRowMax	 = GetDbDataUc141( nPage, parent, po );
//	if(po.nPageNum[1] != 0)	nRowMax2 = GetDbDataUc142( nPage, parent, po);
//
//// midori UC_0024 add -->
//	if(nRowMax == 0 && nRowMax2 > 0)	{
//		if(po.NameKei[0] != 0)	{
//			m_PrtDataBox[Field141KeiStr].Data[10] = NAME_GOUKEI;
//		}
//		else	{
//			m_PrtDataBox[Field141KeiStr].Data[10] = NAME_KEI;
//		}
//		Nod141 = 1;
//	}
//// midori UC_0024 add <--
//// midori 154119 add -->
//	if(nRowMax > 0 && nRowMax2 == 0)	{
//		if(po.NameKei[1] != 0)	{
//			m_PrtDataBox[Field141Max+Field142KeiStr].Data[3] = NAME_GOUKEI;
//		}
//		else	{
//			m_PrtDataBox[Field141Max+Field142KeiStr].Data[3] = NAME_KEI;
//		}
//	}
//// midori 154119 add <--
//
//	m_nColumn = Field141Max;
// ---------------------------------
	if(bG_Kanso == TRUE || bG_InvNo == TRUE) {
		if(po.nPageNum[0] != 0)		nRowMax = GetDbDataUc141K(nPage, parent, po);
		if(po.nPageNum[1] != 0)		nRowMax2 = GetDbDataUc142(nPage, parent, po);

		if(nRowMax == 0 && nRowMax2 > 0) {
			if(po.NameKei[0] != 0) {
				m_PrtDataBox[Field141KKeiStr].Data[10] = NAME_GOUKEI;
			}
			else {
				m_PrtDataBox[Field141KKeiStr].Data[10] = NAME_KEI;
			}
			Nod141 = 1;
		}
		if(nRowMax > 0 && nRowMax2 == 0) {
			if(po.NameKei[1] != 0) {
				m_PrtDataBox[Field141KMax + Field142KeiStr].Data[3] = NAME_GOUKEI;
			}
			else {
				m_PrtDataBox[Field141KMax + Field142KeiStr].Data[3] = NAME_KEI;
			}
		}

		m_nColumn = Field141KMax;
	}
	else	{
		if(po.nPageNum[0] != 0)		nRowMax = GetDbDataUc141(nPage, parent, po);
		if(po.nPageNum[1] != 0)		nRowMax2 = GetDbDataUc142(nPage, parent, po);

		if(nRowMax == 0 && nRowMax2 > 0) {
			if(po.NameKei[0] != 0) {
				m_PrtDataBox[Field141KeiStr].Data[10] = NAME_GOUKEI;
			}
			else {
				m_PrtDataBox[Field141KeiStr].Data[10] = NAME_KEI;
			}
			Nod141 = 1;
		}
		if(nRowMax > 0 && nRowMax2 == 0) {
			if(po.NameKei[1] != 0) {
				m_PrtDataBox[Field141Max + Field142KeiStr].Data[3] = NAME_GOUKEI;
			}
			else {
				m_PrtDataBox[Field141Max + Field142KeiStr].Data[3] = NAME_KEI;
			}
		}

		m_nColumn = Field141Max;
	}
// 調査依頼20240712_24/09/19 cor <--

	// 行数の多い方を使用
// midori UC_0024 add -->
	if(Nod141 != 0)	{
		// 役員報酬等のデータが無い場合、"計" を出力するため最大行数を10にする
		nRowMax = 11;
	}
// midori UC_0024 add <--
	if( nRowMax2 > nRowMax ){

		nRowMax = nRowMax2;
	}

	m_nRowMax = nRowMax;
	m_nColumn += Field142Max;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc141()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑭-1．役員報酬手当金等
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc140YakuinJinkenhi::GetDbDataUc141(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
{
	int				nCurRow			= 0;
// midori 190301 add -->
	int				keta=0;
// midori 190301 add <--
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strNameYakuOld	= _T( "" );
	CString			strNameTantOld	= _T( "" );
	CString			strKankeiOld	= _T( "" );
	CString			strSQL;
	CString			strFrom, strWhere, strData;			// 改良No.23-0961 add
// midori 190301 del -->
	//CString			strAdAdd;
	//CString			strAdAddOld;
// midori 190301 del <--
// midori 190301 add -->
	CString			strAdAdd=_T("");
	CString			strAdAddOld=_T("");
// midori 190301 add <--

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_141;
	m_nColumn	= Field141Max;
	GetPrintInfo( parent );

	CdbUc141Yakuin	rc( m_pDB );
	CRecordset		rs(m_pDB);							// 改良No.23-0961 add

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_141 );

// 調査依頼20240712_24/09/19 del -->
//// 改良No.23-0961 add -->
//	m_141ValSign = 0;
//	try	{
//		if(!(po.OutZeroNull[0]) || m_nOutZero == 1) {
//			strFrom = STR_TBL_NAME_141;
//		}
//		else {
//			#ifdef _DEBUG
//				strFrom.Format(_T("##temp_utiwake_tbl_%d"), m_nFormSeq);
//			#else
//				strFrom.Format(_T("#temp_utiwake_tbl_%d"), m_nFormSeq);
//			#endif
//		}
//		strWhere.Format(_T("(FgFunc = %d or FgFunc = %d or FgFunc = %d or FgFunc = %d or FgFunc = %d) and "),
//						ID_FGFUNC_DATA, ID_FGFUNC_SYOKEI, ID_FGFUNC_CHUKEI, ID_FGFUNC_RUIKEI, ID_FGFUNC_PAGEKEI);
//		strWhere.Format(strWhere + _T("NumPage = %d and "), nPage);
//		strWhere += _T("( ");
//		strWhere += _T("(FpTime >= 1000000000 or FpTime <= -100000000) or ");
//		strWhere += _T("(EmVal >= 1000000000 or EmVal <= -100000000) or ");
//		strWhere += _T("(PrVal >= 1000000000 or PrVal <= -100000000) or ");
//		strWhere += _T("(BfVal >= 1000000000 or BfVal <= -100000000) or ");
//		strWhere += _T("(PfVal >= 1000000000 or PfVal <= -100000000) or ");
//		strWhere += _T("(EtVal >= 1000000000 or EtVal <= -100000000) or ");
//		strWhere += _T("(RtVal >= 1000000000 or RtVal <= -100000000) or ");
//		strWhere += _T("(Val >= 10000000000 or Val <= -1000000000) ");
//		strWhere += _T(") ");
//		// 結合
//		strSQL.Format(_T("select count(SEQ) as 'DtCnt' from %s where %s "), strFrom, strWhere);
//		// 検索
//		if(rs.Open(CRecordset::forwardOnly, strSQL))	{
//			rs.GetFieldValue(_T("DtCnt"), strData);
//			if(_tstoi(strData) > 0)	{
//				m_141ValSign = 1;
//			}
//		}
//		rs.Close();
//	}
//	catch (CDBException *e)	{
//		rs.Close();
//		ICSMessageBox(e->m_strError, MB_ICONSTOP);
//		e->Delete();
//		return FUNCTION_NG;
//	}
//// 改良No.23-0961 add <--
// 調査依頼20240712_24/09/19 del <--

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
					
					strNameYakuOld	= _T( "" );
					strNameTantOld	= _T( "" );
					strKankeiOld	= _T( "" );
// midori 190301 add -->
					strAdAddOld	= _T( "" );
// midori 190301 add <--
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
						rc.m_EmVal = HideZeroMoney( rc.m_EmVal );
						rc.m_PrVal = HideZeroMoney( rc.m_PrVal );
						rc.m_BfVal = HideZeroMoney( rc.m_BfVal );
						rc.m_PfVal = HideZeroMoney( rc.m_PfVal );
						rc.m_EtVal = HideZeroMoney( rc.m_EtVal );
						rc.m_RtVal = HideZeroMoney( rc.m_RtVal );
					}
				}

// 調査依頼20240712_24/09/19 del -->
//// 改良No.23-0961 add -->
//				if(m_141ValSign == 0)	{	// 従来通り
//// 改良No.23-0961 add <--
// 調査依頼20240712_24/09/19 del <--
					// 使用人職務分
					m_PrtDataBox[Field141EmVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_EmVal, &rc, MakeFormatOfNum(9), BIT_D0);

					// 定期同額給与
					m_PrtDataBox[Field141PrVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_PrVal, &rc, MakeFormatOfNum(9), BIT_D1);

					// 事前確定届出給与
					m_PrtDataBox[Field141BfVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_BfVal, &rc, MakeFormatOfNum(9), BIT_D2);

					// 利益連動給与
					m_PrtDataBox[Field141PfVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_PfVal, &rc, MakeFormatOfNum(9), BIT_D3);

					// その他
					m_PrtDataBox[Field141EtVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_EtVal, &rc, MakeFormatOfNum(9), BIT_D4);

					// 退職給与
					m_PrtDataBox[Field141RtVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_RtVal, &rc, MakeFormatOfNum(9), BIT_D5);

					// 役員給与計
// midori 190301 del -->
					//m_PrtDataBox[Field141Val].Data[nCurRow]	  =
					//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 9 ), BIT_D6 );
// midori 190301 del <--
// midori 190301 add -->
					if(bG_Kanso == TRUE)	keta = 10;
					else					keta = 9;
					m_PrtDataBox[Field141Val].Data[nCurRow] = ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(keta), BIT_D6);
// midori 190301 add <--
// 調査依頼20240712_24/09/19 del -->
//// 改良No.23-0961 add -->
//				}
//				else	{					// 金額10桁入力用
//					// 使用人職務分
//					m_PrtDataBox[Field141EmVal2].Data[nCurRow] =
//						ShowFieldOfVal(rc.m_EmVal, &rc, MakeFormatOfNum(10), BIT_D0);
//
//					// 定期同額給与
//					m_PrtDataBox[Field141PrVal2].Data[nCurRow] =
//						ShowFieldOfVal(rc.m_PrVal, &rc, MakeFormatOfNum(10), BIT_D1);
//
//					// 事前確定届出給与
//					m_PrtDataBox[Field141BfVal2].Data[nCurRow] =
//						ShowFieldOfVal(rc.m_BfVal, &rc, MakeFormatOfNum(10), BIT_D2);
//
//					// 利益連動給与
//					m_PrtDataBox[Field141PfVal2].Data[nCurRow] =
//						ShowFieldOfVal(rc.m_PfVal, &rc, MakeFormatOfNum(10), BIT_D3);
//
//					// その他
//					m_PrtDataBox[Field141EtVal2].Data[nCurRow] =
//						ShowFieldOfVal(rc.m_EtVal, &rc, MakeFormatOfNum(10), BIT_D4);
//
//					// 退職給与
//					m_PrtDataBox[Field141RtVal2].Data[nCurRow] =
//						ShowFieldOfVal(rc.m_RtVal, &rc, MakeFormatOfNum(10), BIT_D5);
//
//					// 役員給与計
//					m_PrtDataBox[Field141Val2].Data[nCurRow] =
//						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(11), BIT_D6);
//				}
//// 改良No.23-0961 add <--
// 調査依頼20240712_24/09/19 del <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strNameYakuOld	= _T( "" );
					strNameTantOld	= _T( "" );
					strKankeiOld	= _T( "" );
// midori 190301 add -->
					strAdAddOld	= _T( "" );
// midori 190301 add <--

// midori 190301 del -->
					//m_PrtDataBox[Field141KeiStr].Data[nCurRow] = rc.m_KeiStr;
// midori 190301 del <--
// midori 190301 add -->
					if(bG_Kanso == TRUE && rc.m_NumRow < ID_ROWNORMAL_141)	{
						// 最終ページの"頁計"、"小計"、"中計"は、
						// データと同じ行に出力するので下寄りに出力する
						m_PrtDataBox[Field141KeiStr2].Data[nCurRow] = rc.m_KeiStr;
					}
					else	{
						m_PrtDataBox[Field141KeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
// midori 190301 add <--
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

						// 役職名
						case 1:

							if( CheckOmit( rc.m_NameYaku, strNameYakuOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141NameYakuOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{
// midori 190301 del -->
								//m_PrtDataBox[Field141NameYaku].Data[nCurRow] = rc.m_NameYaku;
// midori 190301 del <--
// midori 190301 add -->
								if (bG_Kanso == TRUE) {
									if(rc.m_NameYaku.GetLength() >= 16)	{
										m_PrtDataBox[Field141NameYaku3].Data[nCurRow] = rc.m_NameYaku;
									}
									else if(rc.m_NameYaku.GetLength() >= 15)	{
										m_PrtDataBox[Field141NameYaku2].Data[nCurRow] = rc.m_NameYaku;
									}
									else	{
										m_PrtDataBox[Field141NameYaku].Data[nCurRow] = rc.m_NameYaku;
									}
								}
								else	{
									m_PrtDataBox[Field141NameYaku].Data[nCurRow] = rc.m_NameYaku;
								}
// midori 190301 add <--
							}

							strNameYakuOld	= rc.m_NameYaku;
							break;

						// 担当業務
						case 2:

							if( CheckOmit( rc.m_NameTant, strNameTantOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141NameTantOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{
// midori 190301 del -->
								//m_PrtDataBox[Field141NameTant].Data[nCurRow] = rc.m_NameTant;
// midori 190301 del <--
// midori 190301 add -->
								if (bG_Kanso == TRUE) {
									if (rc.m_NameTant.GetLength() >= 16) {
										m_PrtDataBox[Field141NameTant3].Data[nCurRow] = rc.m_NameTant;
									}
									else if (rc.m_NameTant.GetLength() >= 15) {
										m_PrtDataBox[Field141NameTant2].Data[nCurRow] = rc.m_NameTant;
									}
									else {
										m_PrtDataBox[Field141NameTant].Data[nCurRow] = rc.m_NameTant;
									}
								}
								else {
									m_PrtDataBox[Field141NameTant].Data[nCurRow] = rc.m_NameTant;
								}
// midori 190301 add <--
							}

							strNameTantOld	= rc.m_NameTant;
							break;

						// 代表者との関係
						case 3:

							if( CheckOmit( rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141KankeiOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								m_PrtDataBox[Field141Kankei].Data[nCurRow] = rc.m_Kankei;
							}

							strKankeiOld	= rc.m_Kankei;
							break;

						// 住所
						case 4:

							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141AdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								StrDivision( (rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2), &strArray, 2 );
								m_PrtDataBox[Field141AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field141AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}

					// 氏名
					StrDivision( (rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2), &strArray, 2 );
					m_PrtDataBox[Field141AdName1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field141AdName2].Data[nCurRow] = strArray.GetAt( 1 );

					// 常勤・非常勤の別
					switch( rc.m_FpTime ){

					// 常
					case 1:
						m_PrtDataBox[Field141FpTime1].Data[nCurRow] = PRT_FORMAT_CHECK;
						break;

					// 非
					case 2:

						m_PrtDataBox[Field141FpTime2].Data[nCurRow] = PRT_FORMAT_CHECK;
						break;

					default:
						break;
					}
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return m_nRowMax;

		int nItem = Field141Max;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc141()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

// 調査依頼20240712_24/09/19 add -->
////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc141K() 簡素化以降
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑭-1．役員報酬手当金等
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc140YakuinJinkenhi::GetDbDataUc141K(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
{
	int				nCurRow = 0;
	int				keta=0;
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strNameYakuOld	= _T( "" );
	CString			strNameTantOld	= _T( "" );
	CString			strKankeiOld	= _T( "" );
	CString			strSQL;
	CString			strFrom, strWhere, strData;
	CString			strAdAdd=_T("");
	CString			strAdAddOld=_T("");

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_141;
	m_nColumn	= Field141KMax;
	GetPrintInfo( parent );

	CdbUc141Yakuin	rc(m_pDB);
	CRecordset		rs(m_pDB);

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_141 );

	m_141ValSign = 0;
	try	{
		if(!(po.OutZeroNull[0]) || m_nOutZero == 1) {
			strFrom = STR_TBL_NAME_141;
		}
		else {
			#ifdef _DEBUG
				strFrom.Format(_T("##temp_utiwake_tbl_%d"), m_nFormSeq);
			#else
				strFrom.Format(_T("#temp_utiwake_tbl_%d"), m_nFormSeq);
			#endif
		}
		strWhere.Format(_T("(FgFunc = %d or FgFunc = %d or FgFunc = %d or FgFunc = %d or FgFunc = %d) and "),
						ID_FGFUNC_DATA, ID_FGFUNC_SYOKEI, ID_FGFUNC_CHUKEI, ID_FGFUNC_RUIKEI, ID_FGFUNC_PAGEKEI);
		strWhere.Format(strWhere + _T("NumPage = %d and "), nPage);
		strWhere += _T("( ");
		strWhere += _T("(FpTime >= 1000000000 or FpTime <= -100000000) or ");
		strWhere += _T("(EmVal >= 1000000000 or EmVal <= -100000000) or ");
		strWhere += _T("(PrVal >= 1000000000 or PrVal <= -100000000) or ");
		strWhere += _T("(BfVal >= 1000000000 or BfVal <= -100000000) or ");
		strWhere += _T("(PfVal >= 1000000000 or PfVal <= -100000000) or ");
		strWhere += _T("(EtVal >= 1000000000 or EtVal <= -100000000) or ");
		strWhere += _T("(RtVal >= 1000000000 or RtVal <= -100000000) or ");
		strWhere += _T("(Val >= 10000000000 or Val <= -1000000000) ");
		strWhere += _T(") ");
		// 結合
		strSQL.Format(_T("select count(SEQ) as 'DtCnt' from %s where %s "), strFrom, strWhere);
		// 検索
		if(rs.Open(CRecordset::forwardOnly, strSQL))	{
			rs.GetFieldValue(_T("DtCnt"), strData);
			if(_tstoi(strData) > 0)	{
				m_141ValSign = 1;
			}
		}
		rs.Close();
	}
	catch (CDBException *e)	{
		rs.Close();
		ICSMessageBox(e->m_strError, MB_ICONSTOP);
		e->Delete();
		return FUNCTION_NG;
	}

	try{
		if(!(po.OutZeroNull[0]) || m_nOutZero == 1){
			// 指定されたページの行数を取得
			m_nRowMax = rc.GetNumRow( nPage );

			rc.Init( nPage );
		}
		else{
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#endif

			rc.Open( CRecordset::forwardOnly, strSQL );
		}

		// 最終レコードまで処理を実行
		while( !rc.IsEOF() ){
			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if( nCurRow >= 0 ){
				// 空行
				if( CheckDbDataFgFuncNull( &rc ) ){					
					strNameYakuOld	= _T( "" );
					strNameTantOld	= _T( "" );
					strKankeiOld	= _T( "" );
					strAdAddOld		= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
						rc.m_EmVal = HideZeroMoney( rc.m_EmVal );
						rc.m_PrVal = HideZeroMoney( rc.m_PrVal );
						rc.m_BfVal = HideZeroMoney( rc.m_BfVal );
						rc.m_PfVal = HideZeroMoney( rc.m_PfVal );
						rc.m_EtVal = HideZeroMoney( rc.m_EtVal );
						rc.m_RtVal = HideZeroMoney( rc.m_RtVal );
					}
				}

				if(m_141ValSign == 0)	{	// 従来通り
					// 使用人職務分
					m_PrtDataBox[Field141KEmVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_EmVal, &rc, MakeFormatOfNum(9), BIT_D0);

					// 定期同額給与
					m_PrtDataBox[Field141KPrVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_PrVal, &rc, MakeFormatOfNum(9), BIT_D1);

					// 事前確定届出給与
					m_PrtDataBox[Field141KBfVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_BfVal, &rc, MakeFormatOfNum(9), BIT_D2);

					// 利益連動給与
					m_PrtDataBox[Field141KPfVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_PfVal, &rc, MakeFormatOfNum(9), BIT_D3);

					// その他
					m_PrtDataBox[Field141KEtVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_EtVal, &rc, MakeFormatOfNum(9), BIT_D4);

					// 退職給与
					m_PrtDataBox[Field141KRtVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_RtVal, &rc, MakeFormatOfNum(9), BIT_D5);

					// 役員給与計
					if(bG_Kanso == TRUE)	keta = 10;
					else					keta = 9;
					m_PrtDataBox[Field141KVal].Data[nCurRow] = ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(keta), BIT_D6);
				}
				else	{					// 金額10桁入力用
					// 使用人職務分
					m_PrtDataBox[Field141KEmVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_EmVal, &rc, MakeFormatOfNum(10), BIT_D0);

					// 定期同額給与
					m_PrtDataBox[Field141KPrVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_PrVal, &rc, MakeFormatOfNum(10), BIT_D1);

					// 事前確定届出給与
					m_PrtDataBox[Field141KBfVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_BfVal, &rc, MakeFormatOfNum(10), BIT_D2);

					// 利益連動給与
					m_PrtDataBox[Field141KPfVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_PfVal, &rc, MakeFormatOfNum(10), BIT_D3);

					// その他
					m_PrtDataBox[Field141KEtVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_EtVal, &rc, MakeFormatOfNum(10), BIT_D4);

					// 退職給与
					m_PrtDataBox[Field141KRtVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_RtVal, &rc, MakeFormatOfNum(10), BIT_D5);

					// 役員給与計
					m_PrtDataBox[Field141KVal2].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(11), BIT_D6);
				}

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){
					strNameYakuOld	= _T( "" );
					strNameTantOld	= _T( "" );
					strKankeiOld	= _T( "" );
					strAdAddOld	= _T( "" );

					if(bG_Kanso == TRUE && rc.m_NumRow < ID_ROWNORMAL_141)	{
						// 最終ページの"頁計"、"小計"、"中計"は、
						// データと同じ行に出力するので下寄りに出力する
						m_PrtDataBox[Field141KKeiStr2].Data[nCurRow] = rc.m_KeiStr;
					}
					else	{
						m_PrtDataBox[Field141KKeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){
						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){
						// 役職名
						case 1:
							if( CheckOmit( rc.m_NameYaku, strNameYakuOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141KNameYakuOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{
								if (bG_Kanso == TRUE) {
									if(rc.m_NameYaku.GetLength() >= 16)	{
										m_PrtDataBox[Field141KNameYaku3].Data[nCurRow] = rc.m_NameYaku;
									}
									else if(rc.m_NameYaku.GetLength() >= 15)	{
										m_PrtDataBox[Field141KNameYaku2].Data[nCurRow] = rc.m_NameYaku;
									}
									else	{
										m_PrtDataBox[Field141KNameYaku].Data[nCurRow] = rc.m_NameYaku;
									}
								}
								else	{
									m_PrtDataBox[Field141KNameYaku].Data[nCurRow] = rc.m_NameYaku;
								}
							}

							strNameYakuOld	= rc.m_NameYaku;
							break;

						// 担当業務
						case 2:
							if( CheckOmit( rc.m_NameTant, strNameTantOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141KNameTantOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {
								if (bG_Kanso == TRUE) {
									if (rc.m_NameTant.GetLength() >= 16) {
										m_PrtDataBox[Field141KNameTant3].Data[nCurRow] = rc.m_NameTant;
									}
									else if (rc.m_NameTant.GetLength() >= 15) {
										m_PrtDataBox[Field141KNameTant2].Data[nCurRow] = rc.m_NameTant;
									}
									else {
										m_PrtDataBox[Field141NameTant].Data[nCurRow] = rc.m_NameTant;
									}
								}
								else {
									m_PrtDataBox[Field141KNameTant].Data[nCurRow] = rc.m_NameTant;
								}
							}

							strNameTantOld	= rc.m_NameTant;
							break;

						// 代表者との関係
						case 3:
							if( CheckOmit( rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141KKankeiOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								m_PrtDataBox[Field141KKankei].Data[nCurRow] = rc.m_Kankei;
							}

							strKankeiOld	= rc.m_Kankei;
							break;

						// 住所
						case 4:
							strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

							if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field141KAdAddOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								StrDivision( (rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2), &strArray, 2 );
								m_PrtDataBox[Field141KAdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field141KAdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strAdAddOld = strAdAdd;
							break;

						default:
							break;
						}
					}

					// 氏名
					StrDivision( (rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2), &strArray, 2 );
					m_PrtDataBox[Field141KAdName1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field141KAdName2].Data[nCurRow] = strArray.GetAt( 1 );

					// 常勤・非常勤の別
					switch( rc.m_FpTime ){
					// 常
					case 1:
						m_PrtDataBox[Field141KFpTime1].Data[nCurRow] = PRT_FORMAT_CHECK;
						break;

					// 非
					case 2:
						m_PrtDataBox[Field141KFpTime2].Data[nCurRow] = PRT_FORMAT_CHECK;
						break;

					default:
						break;
					}
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return m_nRowMax;

		int nItem = Field141KMax;
	}
	catch( ... ){
		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc141()\n" );

		if( rc.IsOpen() ){
			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// 調査依頼20240712_24/09/19 add <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc142()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑭-2．人件費
//
// 引数		：int				nPage				ページ番号
//			：int				nFormType			出力形式	ID_FORMTYPE_NORMAL：通常
//																ID_FORMTYPE_KOBETU：個別
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc140YakuinJinkenhi::GetDbDataUc142(int nPage, CprtJobFunction* parent, const PRT_OPTION& po)
{
	int	nCurRow	= 0;
	int	index	= 0;

	// 省略文字の取得
	m_nFormSeq	= ID_FORMNO_142;
	m_nColumn	= Field142Max;

	GetPrintInfo( parent );

	// データボックス内の格納場所を変更
// 調査依頼20240712_24/09/19 cor -->
//	index		= Field141Max;
// ---------------------------------
	//if(bG_Kanso == TRUE)	index = Field141Max;
	//else					index = Field141Max - 7;	// 改良No.23-0961で追加した分を減らす
	if(bG_Kanso == TRUE || bG_InvNo == TRUE)	index = Field141KMax;
	else										index = Field141Max;
// 調査依頼20240712_24/09/19 cor <--

	CdbUc142Jinkenhi	rc( m_pDB );

	try{

		// 指定されたページの行数を取得
		int nRowMax = rc.GetNumRow( nPage );

		rc.Init( nPage );

		// 最終レコードまで処理を実行
		while( !rc.IsEOF() ){

			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if( nCurRow >= 0 ){

				// 空行
				if( CheckDbDataFgFuncNull( &rc ) ){
					
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[1]){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_Val2	= HideZeroMoney( rc.m_Val2 );
					}
				}

// No.200903 del -->
				//// 総額
				//m_PrtDataBox[index + Field142Val].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ) );

				//// 総額のうち代表者及びその家族分
				//m_PrtDataBox[index + Field142Val2].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ) );
// No.200903 del <--
// No.200903 add -->
				// 総額
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[index + Field142Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ) );
				}
				else	{
					m_PrtDataBox[index + Field142Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ) );
				}

				// 総額のうち代表者及びその家族分
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[index + Field142Val2].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 13 ) );
				}
				else	{
					m_PrtDataBox[index + Field142Val2].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ) );
				}
// No.200903 add <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					m_PrtDataBox[index + Field142KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}

			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc142()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}


CprtUc140YakuinJinkenhi::CprtUc140YakuinJinkenhi(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_141;
	m_nRowMax	= 0;
// 調査依頼20240712_24/09/19 cor -->
//	m_nColumn	= Field141Max;
// ---------------------------------
	if(bG_Kanso == TRUE || bG_InvNo == TRUE)	m_nColumn = Field141KMax;
	else										m_nColumn = Field141Max;
// 調査依頼20240712_24/09/19 cor <--
}

CprtUc140YakuinJinkenhi::~CprtUc140YakuinJinkenhi(void)
{
}

CprtJobFunction* CprtUc140YakuinJinkenhi::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc140YakuinJinkenhi(m_pDB));
}