//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc021Uketoritegata.cpp
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
#include "prtUc021Uketoritegata.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

extern BOOL bG_InvNo; // インボイス登録番号追加対応_23/10/25 add

// 改良No.21-0086,21-0529 cor -->
//// midori 156188 add -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 156188 add <--
// ------------------------------
extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

// midori 157042 del -->
//// midori 190505 add -->
//extern	int		g_PrnHoho[2];
//// midori 190505 add <--
// midori 157042 del <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc021()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ②．受取手形
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
// midori 190301 del -->
//int CprtUc021Uketoritegata::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
//{
//	int				nCurRow			= 0;
//	CStringArray	strArray;
//	CString			strAdName		= _T( "" );
//	CString			strBkName		= _T( "" );
//	CString			strBk2Name		= _T( "" );
//	CString			strAdNameOld	= _T( "" );
//	CString			strBkNameOld	= _T( "" );
//	CString			strBk2NameOld	= _T( "" );
//	CString			strSQL;
//	CharController		cc;
//
//	// 省略文字の取得
//	GetPrintInfo( parent );
//
//	CdbUc021Uketoritegata	rc( m_pDB );
//	CdbUcInfSub				mfcRecSub( m_pDB );
//
//	try{
//// midori 160610 cor -->
////		if(po.OutZeroNull[0] || po.KamokuRow){
//// ---------------------
//		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
//// midori 160610 cor <--
//			// 指定されたページの行数を取得（一時テーブル）
//			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );
//
//// midori 160610 cor -->
////			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
//// ---------------------
//#ifdef _DEBUG
//			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
//#else
//			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
//#endif
//// midori 160610 cor <--
//			rc.Open( CRecordset::forwardOnly, strSQL );
//		}
//		else{
//			// 通常のテーブルオープン
//			m_nRowMax = rc.GetNumRow( nPage );
//
//			rc.Init( nPage );
//		}
//
//		// 最終レコードまで処理を実行
//		while( !rc.IsEOF() ){
//
//			nCurRow = rc.m_NumRow - 1;
//
//			// 行番号が存在するときのみ実行
//			if( nCurRow >= 0 ){
//
//				// 空行
//				if( CheckDbDataFgFuncNull( &rc ) ){
//					
//					strAdNameOld	= _T( "" );
//					strBkNameOld	= _T( "" );
//					strBk2NameOld	= _T( "" );
//					continue;
//				}
//
//				// 科目行
//				if( rc.m_FgFunc == ID_FGFUNC_KAMOKU ){
//					// 科目
//					CString strDividedName1, strDividedName2;
//					// 分割後の文字列取得
//					cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
//					// 9文字以上なら2行表示
//					if( !(strDividedName2.IsEmpty()) ){
//						// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
//						if( strDividedName1.GetLength() == 15 ){
//							strDividedName1 += _T(" ");
//						}
//						// 半角スペースで16バイトまで埋める
//						strDividedName2 = FillSpace( strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2 );
//
//						m_PrtDataBox[Field021KnName1].Data[nCurRow] = strDividedName1;
//						m_PrtDataBox[Field021KnName2].Data[nCurRow] = strDividedName2;
//					}
//					else{
//						m_PrtDataBox[Field021KnName].Data[nCurRow] = rc.m_KnName;
//					}
//					
//
//					rc.MoveNext();
//					continue;
//				}
//
//				// データ行の０円非表示
//				//if( m_PrtInfCommon.HideZero && !m_PrtInfCommon.OutZero ){
//				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
//					if(po.HideZero[0]){
//						rc.m_Val = HideZeroMoney( rc.m_Val );
//					}
//				}
//
//				// 金額
//				m_PrtDataBox[Field021Val].Data[nCurRow] =
//					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
//
//				// 計行
//				if(	CheckDbDataFgFuncKei( &rc ) ){
//
//					strAdNameOld	= _T( "" );
//					strBkNameOld	= _T( "" );
//					strBk2NameOld	= _T( "" );
//
//// midori 150917 cor -->
//					//m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
//// ---------------------
//					m_clsFunc.StrDivisionEx(rc.m_KeiStr,&strArray,10,2);
//					if(strArray.GetAt(1).IsEmpty() == FALSE)	{
//						m_PrtDataBox[Field021KeiStr1].Data[nCurRow] = strArray.GetAt(0);
//						m_PrtDataBox[Field021KeiStr2].Data[nCurRow] = strArray.GetAt(1);
//					}
//					else	{
//						m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
//					}
//// midori 150917 cor <--
//				}
//				else{
//					// 省略文字使用欄の判別
//					for( int i = 0; i < PRT_MAX_OMIT; i++ ){
//
//						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;
//
//						switch( nItemSeq ){
//
//							// 振出人
//							case 1:
//
//								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;
//
//								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){
//
//									m_PrtDataBox[Field021AdNameOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//
//									StrDivision( strAdName, &strArray, 2 );
//									m_PrtDataBox[Field021AdName1].Data[nCurRow] = strArray.GetAt( 0 );
//									m_PrtDataBox[Field021AdName2].Data[nCurRow] = strArray.GetAt( 1 );
//								}
//
//								strAdNameOld = strAdName;
//								break;
//
//								// 支払銀行名
//							case 2:
//
//								//								strBkName = rc.m_BkName1 + PRT_FORMAT_SLASH + rc.m_BkName2;
//								strBkName = rc.m_BkName1 + PRT_FORMAT_CHCODE + rc.m_BkName2;
//
//								if( CheckOmit( strBkName, strBkNameOld, i, m_PrtInfOmitList1 ) ){
//
//									m_PrtDataBox[Field021BkNameOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//									if( po.KinyuLine == 0 ){	// 印刷設定→金融機関を一段で表示する
//
//										m_PrtDataBox[Field021BkName1].Data[nCurRow] = rc.m_BkName1;
//										m_PrtDataBox[Field021BkName2].Data[nCurRow] =
//											InsertSlashInHead( rc.m_BkName1, rc.m_BkName2 );
//									}
//									else{
//										CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名
//
//										// 分割後の金融機関名（銀行名）取得
//										cc.Divide(rc.m_BkName1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);
//
//										// 金融機関名（銀行名）が1行？
//										if( !DivBkNameDown.IsEmpty() ){
//											m_PrtDataBox[Field021BkName1_1Slash].Data[nCurRow] = DivBkNameUp;
//											m_PrtDataBox[Field021BkName1_2Slash].Data[nCurRow] = DivBkNameDown;
//										}
//										else{	// 1行の場合はスラッシュに高さをあわせる
//											m_PrtDataBox[Field021BkName1Slash].Data[nCurRow] = DivBkNameUp;
//										}
//
//										// 分割後の金融機関名（支店名）取得
//										cc.Divide(rc.m_BkName2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);
//
//										// 金融機関名（支店名）が1行？
//										if( !DivBkNameDown.IsEmpty() ){
//											m_PrtDataBox[Field021BkName2_1Slash].Data[nCurRow] = DivBkNameUp;
//											m_PrtDataBox[Field021BkName2_2Slash].Data[nCurRow] = DivBkNameDown;
//										}
//										else{	// 1行の場合はスラッシュに高さをあわせる
//											m_PrtDataBox[Field021BkName2Slash].Data[nCurRow] = DivBkNameUp;
//										}
//
//										// "/"
//										if(( rc.m_BkName1.IsEmpty() == 0) || ( rc.m_BkName2.IsEmpty() == 0 )){
//											m_PrtDataBox[Field021Slash].Data[nCurRow] = "/";
//										}
//									}
//								}
//
//								strBkNameOld = strBkName;
//								break;
//
//							// 割引銀行名等
//							case 3:
//
////								strBk2Name = rc.m_Bk2Name1 + PRT_FORMAT_SLASH + rc.m_Bk2Name2;
//								strBk2Name = rc.m_Bk2Name1 + PRT_FORMAT_CHCODE + rc.m_Bk2Name2;
//
//								if( CheckOmit( strBk2Name, strBk2NameOld, i, m_PrtInfOmitList1 ) ){
//
//									m_PrtDataBox[Field021Bk2NameOm].Data[nCurRow] =
//										m_PrtInfOmitList1[i].OmitStr;
//								}
//								else{
//									if( po.KinyuLine == 0 ){
//
//										m_PrtDataBox[Field021Bk2Name1].Data[nCurRow] = rc.m_Bk2Name1;
//										m_PrtDataBox[Field021Bk2Name2].Data[nCurRow] =
//											InsertSlashInHead( rc.m_Bk2Name1, rc.m_Bk2Name2 );
//									}
//									else{
//										CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名
//
//										// 分割後の金融機関名（銀行名）取得
//										cc.Divide(rc.m_Bk2Name1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);
//
//										// 金融機関名（銀行名）が1行？
//										if( !DivBkNameDown.IsEmpty() ){
//											m_PrtDataBox[Field021Bk2Name1_1Slash].Data[nCurRow] = DivBkNameUp;
//											m_PrtDataBox[Field021Bk2Name1_2Slash].Data[nCurRow] = DivBkNameDown;
//										}
//										else{	// 1行の場合はスラッシュに高さをあわせる
//											m_PrtDataBox[Field021Bk2Name1Slash].Data[nCurRow] = DivBkNameUp;
//										}
//
//										// 分割後の金融機関名（支店名）取得
//										cc.Divide(rc.m_Bk2Name2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);
//
//										// 金融機関名（支店名）が1行？
//										if( !DivBkNameDown.IsEmpty() ){
//											m_PrtDataBox[Field021Bk2Name2_1Slash].Data[nCurRow] = DivBkNameUp;
//											m_PrtDataBox[Field021Bk2Name2_2Slash].Data[nCurRow] = DivBkNameDown;
//										}
//										else{	// 1行の場合はスラッシュに高さをあわせる
//											m_PrtDataBox[Field021Bk2Name2Slash].Data[nCurRow] = DivBkNameUp;
//										}
//
//										// "/"
//										if(( rc.m_BkName1.IsEmpty() == 0) || ( rc.m_BkName2.IsEmpty() == 0 )){
//											m_PrtDataBox[Field021Slash2].Data[nCurRow] = "/";
//										}
//									}
//								}
//
//								strBk2NameOld = strBk2Name;
//								break;
//
//							default:
//								break;
//						}
//					}
//
//					// 振出年月日
//					m_PrtDataBox[Field021SpDate].Data[nCurRow] =
//						ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
//
//					// 支払期日
//					m_PrtDataBox[Field021PmDate].Data[nCurRow] =
//						ConvertDate( rc.m_PmDate, PRT_FORMAT_DATE_B1 );
//
//					// 摘要
//					StrDivision( rc.m_Teki, &strArray, 2 );
//					m_PrtDataBox[Field021Teki1].Data[nCurRow] = strArray.GetAt( 0 );
//					m_PrtDataBox[Field021Teki2].Data[nCurRow] = strArray.GetAt( 1 );
//				}
//			}
//
//			// 次のレコードへ移動
//			rc.MoveNext();
//		}
//
//		// DBアクセスの終了
//		rc.Fin();
//
//		//return m_nRowMax;
//		return 0;
//	}
//	catch( ... ){
//
//		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc021()\n" );
//
//		if( rc.IsOpen() ){
//
//			rc.Fin();
//			return FUNCTION_NG;
//		}
//	}
//
//	return FUNCTION_OK;
//}
// midori 190301 del <--
// midori 190301 add -->
// インボイス登録番号追加対応_23/10/25 del -->
//int CprtUc021Uketoritegata::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
//{
//	int	rv=0;
//
//	if(bG_Kanso == TRUE)	rv = GetDbDataSheetNew(nPage,parent,po,nFormSeq);
//	else					rv = GetDbDataSheetOld(nPage,parent,po,nFormSeq);
//	return(rv);
//}
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
int CprtUc021Uketoritegata::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int rv = 0;

	if(bG_InvNo == TRUE) {
		rv = GetDbDataSheetNew2(nPage, parent, po, nFormSeq);
	} 
	else if(bG_Kanso == TRUE) {
		rv = GetDbDataSheetNew(nPage, parent, po, nFormSeq);
	} 
	else {
		rv = GetDbDataSheetOld(nPage, parent, po, nFormSeq);
	}

	return(rv);
}

// インボイス登録番号版
int CprtUc021Uketoritegata::GetDbDataSheetNew2(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow = 0;
	int				sw = 0;
	int				sw2 = 0;
	CStringArray	strArray;
	CString			strAdName = _T("");
	CString			strBkName = _T("");
	CString			strStName = _T("");
	CString			strBk2Name = _T("");
	CString			strAdNameOld = _T("");
	CString			strBkNameOld = _T("");
	CString			strStNameOld = _T("");
	CString			strBk2NameOld = _T("");
	CString			strSQL;
	CharController		cc;
	CString			strkei = _T("");
	CString			strkei2 = _T("");


	// 省略文字の取得
	GetPrintInfo(parent);

	CdbUc021Uketoritegata	rc(m_pDB);
	CdbUcInfSub				mfcRecSub(m_pDB);

// 改良No.21-0086,21-0529 cor -->
	//sw = KamokuRowEnableSgn(m_pDB, 0);
// ------------------------------
	sw = KamokuRowEnableSgn(m_pDB, 0, nFormSeq);
// 改良No.21-0086,21-0529 cor <--
	try {
		sw2 = 0;
		if (po.OutZeroNull[0])	sw2 = 1;
		if (po.KamokuSitei[0])	sw2 = 1;
		if (po.KamokuRow) {
			if (sw == 0) {
				sw2 = 1;
			}
			else {
				if (rc.GetKmkRowSw() == 0) {
					sw2 = 1;
				}
			}
		}

		if (sw2 == 1) {
			// 指定されたページの行数を取得（一時テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl(m_nFormSeq);

#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage);
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage);
#endif
			rc.Open(CRecordset::forwardOnly, strSQL);
		}
		else {
			// 通常のテーブルオープン
			m_nRowMax = rc.GetNumRow(nPage);

			rc.Init(nPage);
		}

		// 最終レコードまで処理を実行
		while (!rc.IsEOF()) {

			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if (nCurRow >= 0) {

				// 空行
				if (CheckDbDataFgFuncNull(&rc)) {

					strAdNameOld = _T("");
					strBkNameOld = _T("");
					strStNameOld = _T("");
					strBk2NameOld = _T("");
					continue;
				}

				// 科目行
				if (rc.m_FgFunc == ID_FGFUNC_KAMOKU) {
					// 科目
					CString strDividedName1, strDividedName2;
					// 分割後の文字列取得
					if (sw == 1) {
						strDividedName1.Empty();
						strDividedName2.Empty();
						if (rc.m_KeiStr.IsEmpty() == FALSE) {
							// 改行コードが見つからない場合
							if (rc.m_KeiStr.Find(_T("\r\n")) < 0) {
								// 文字列が10文字を超えている場合は、10文字で分割
								cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, 10);
							}
							// 改行コードが見つかった場合
							else {
								// 改行文字で2行に分割する
								m_clsFunc.StrDivision(rc.m_KeiStr, &strArray, 2);
								strDividedName1 = strArray.GetAt(0);
								strDividedName2 = strArray.GetAt(1);
							}
						}
					}
					else {
						cc.Divide(rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD);
					}
					// 9文字以上なら2行表示
					if (!(strDividedName2.IsEmpty())) {
						// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
						if (strDividedName1.GetLength() == 15) {
							strDividedName1 += _T(" ");
						}
						// 半角スペースで16バイトまで埋める
						strDividedName2 = FillSpace(strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2);

						m_PrtDataBox[Field021IKnName1].Data[nCurRow] = strDividedName1;
						m_PrtDataBox[Field021IKnName2].Data[nCurRow] = strDividedName2;
					}
					else {
						if (sw == 1)	m_PrtDataBox[Field021IKnName].Data[nCurRow] = rc.m_KeiStr;
						else			m_PrtDataBox[Field021IKnName].Data[nCurRow] = rc.m_KnName;
					}

					rc.MoveNext();
					continue;
				}

				// データ行の０円非表示
				if (CheckDbDataFgFuncKei(&rc) == FALSE) {
					if (po.HideZero[0]) {
						rc.m_Val = HideZeroMoney(rc.m_Val);
					}
				}

				// 金額
				if (bG_Kanso == TRUE) {
					m_PrtDataBox[Field021IVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(13), BIT_D0);
				}
				else {
					m_PrtDataBox[Field021IVal].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum(12), BIT_D0);
				}

				// 計行
				if (CheckDbDataFgFuncKei(&rc)) {

					strAdNameOld = _T("");
					strBkNameOld = _T("");
					strStNameOld = _T("");
					strBk2NameOld = _T("");

					// (小計名称 または 中計名称) かつ 小計名称編集可能
					if ((rc.m_FgFunc == ID_FGFUNC_SYOKEI || rc.m_FgFunc == ID_FGFUNC_CHUKEI) && sw == 1) {
						// 157154 add <--
// 改良No.21-0086,21-0529 cor -->
						//m_clsFunc.StrDivision(rc.m_KeiStr, &strArray, 2);
						//strkei = strArray.GetAt(0);
						//strkei2 = strArray.GetAt(1);
// ------------------------------
						// 科目指定で「小計を出力する」場合、科目名称の文字数によっては枠からはみ出してしまうため、
						// 文字数と改行マーク(\r\n)の有無を見て、分割する関数を分ける。
						int		nCh=0;
						int		nLen=0;
						nCh = rc.m_KeiStr.Find(_T("\r\n"));
						nLen = rc.m_KeiStr.GetLength();
						//if(nCh < 0 && nLen > 20)	{
						if(nCh < 0 && nLen > 26)	{
							m_clsFunc.KeiStrSprit(rc.m_KeiStr, &strkei, &strkei2);
						}
						else	{
							m_clsFunc.StrDivision(rc.m_KeiStr, &strArray, 2);
							strkei = strArray.GetAt(0);
							strkei2 = strArray.GetAt(1);
						}
// 改良No.21-0086,21-0529 cor <--
						// 2行出力
						if (strkei2.IsEmpty() == FALSE) {
							// 1行目
							if (strkei.GetLength() > 24) {
								m_PrtDataBox[Field021IKeiStr8].Data[nCurRow] = strkei;
							}
							else if (strkei.GetLength() > 22) {
								m_PrtDataBox[Field021IKeiStr7].Data[nCurRow] = strkei;
							}
							else {
								m_PrtDataBox[Field021IKeiStr1].Data[nCurRow] = strkei;
							}
							// 2行目
							if (strkei2.GetLength() > 24) {
								m_PrtDataBox[Field021IKeiStr2].Data[nCurRow] = strkei2;
							}
							else if (strkei2.GetLength() > 22) {
								m_PrtDataBox[Field021IKeiStr6].Data[nCurRow] = strkei2;
							}
							else {
								m_PrtDataBox[Field021IKeiStr4].Data[nCurRow] = strkei2;
							}
						}
						// 1行出力
						else {
							if (strkei.GetLength() > 24) {
								m_PrtDataBox[Field021IKeiStr10].Data[nCurRow] = strkei;
							}
							else if (strkei.GetLength() > 22) {
								m_PrtDataBox[Field021IKeiStr9].Data[nCurRow] = strkei;
							}
							//else if (rc.m_KeiStr.GetLength() > 20) {	// 改良No.21-0086,21-0529 del
							else if (rc.m_KeiStr.GetLength() > 18) {	// 改良No.21-0086,21-0529 add
								m_PrtDataBox[Field021IKeiStr5].Data[nCurRow] = strkei;
							}
							else {
								m_PrtDataBox[Field021IKeiStr].Data[nCurRow] = strkei;
							}
						}
					}
// 修正No.168370 add -->
					// 累計行、頁計行
					else if(rc.m_FgFunc == ID_FGFUNC_RUIKEI || rc.m_FgFunc == ID_FGFUNC_PAGEKEI) {
						m_PrtDataBox[Field021IKeiStr11].Data[nCurRow] = rc.m_KeiStr;
					}
// 修正No.168370 add <--
					// 小計名称編集済みサインOFF
					else {
						m_clsFunc.KeiStrSprit(rc.m_KeiStr, &strkei, &strkei2);
						if (strkei2.IsEmpty() == FALSE) {
							m_PrtDataBox[Field021IKeiStr1].Data[nCurRow] = strkei;
							if (strkei2.GetLength() > 24) {
								m_PrtDataBox[Field021IKeiStr2].Data[nCurRow] = strkei2;
							}
							else if (strkei2.GetLength() > 22) {
								m_PrtDataBox[Field021IKeiStr6].Data[nCurRow] = strkei2;
							}
							else {
								m_PrtDataBox[Field021IKeiStr4].Data[nCurRow] = strkei2;
							}
						}
						else {
// 修正No.168370 del -->
							//if (rc.m_KeiStr.GetLength() > 22)		m_PrtDataBox[Field021KeiStr3].Data[nCurRow] = rc.m_KeiStr;
							//else if (rc.m_KeiStr.GetLength() > 20)	m_PrtDataBox[Field021KeiStr5].Data[nCurRow] = rc.m_KeiStr;
							//else									m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
// 修正No.168370 del -->
// 修正No.168370 add -->
							if(rc.m_KeiStr.GetLength() > 22) {
								m_PrtDataBox[Field021IKeiStr3].Data[nCurRow] = rc.m_KeiStr;
							}
							else if(rc.m_KeiStr.GetLength() > 20) {
								m_PrtDataBox[Field021IKeiStr5].Data[nCurRow] = rc.m_KeiStr;
							}
							else {
								m_PrtDataBox[Field021IKeiStr].Data[nCurRow] = rc.m_KeiStr;
							}
// 修正No.168370 add -->
						}
					}
				}
				else	{
					// インボイス登録番号（法人番号）
					m_PrtDataBox[Field021IInvoNo].Data[nCurRow].Format(_T("%s"), rc.m_InvNo);

					// 省略文字使用欄の判別
					for(int i = 0; i < PRT_MAX_OMIT; i++) {
						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch (nItemSeq) {
						// 振出人
						case 1:
							strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;
							if (CheckOmit(strAdName, strAdNameOld, i, m_PrtInfOmitList1)) {
								m_PrtDataBox[Field021IAdNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
							}
							else {
								StrDivision(strAdName, &strArray, 2);
								m_PrtDataBox[Field021IAdName1].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field021IAdName2].Data[nCurRow] = strArray.GetAt(1);
							}
							strAdNameOld = strAdName;
							break;

						// 支払銀行名称
						case 2:
							strBkName = rc.m_BkName1;
							if(CheckOmit(strBkName, strBkNameOld, i, m_PrtInfOmitList1)) {
								m_PrtDataBox[Field021IBkNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
							}
							else {
								StrDivision(rc.m_BkName1, &strArray, 2);
								m_PrtDataBox[Field021IBkName1_1Slash].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field021IBkName1_2Slash].Data[nCurRow] = strArray.GetAt(1);
							}
							strBkNameOld = strBkName;
							break;

						// 支払銀行支店名
						case 3:
							strStName = rc.m_BkName2;
							if (CheckOmit(strStName, strStNameOld, i, m_PrtInfOmitList1)) {
								m_PrtDataBox[Field021IStNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
							}
							else {
								StrDivision(rc.m_BkName2, &strArray, 2);
								m_PrtDataBox[Field021IBkName2_1Slash].Data[nCurRow] = strArray.GetAt(0);
								m_PrtDataBox[Field021IBkName2_2Slash].Data[nCurRow] = strArray.GetAt(1);
							}
							strStNameOld = strStName;
							break;

						// 割引銀行名等
						case 4:
							strBk2Name = rc.m_Bk2Name1 + PRT_FORMAT_CHCODE + rc.m_Bk2Name2;

							if (CheckOmit(strBk2Name, strBk2NameOld, i, m_PrtInfOmitList1)) {

								m_PrtDataBox[Field021IBk2NameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else {
								if (po.KinyuLine == 0) {

									m_PrtDataBox[Field021IBk2Name1].Data[nCurRow] = rc.m_Bk2Name1;
									m_PrtDataBox[Field021IBk2Name2].Data[nCurRow] =
										InsertSlashInHead(rc.m_Bk2Name1, rc.m_Bk2Name2);
								}
								else {
									CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名

									// 分割後の金融機関名（銀行名）取得
									cc.Divide(rc.m_Bk2Name1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

									// 金融機関名（銀行名）が1行？
									if (!DivBkNameDown.IsEmpty()) {
										m_PrtDataBox[Field021IBk2Name1_1Slash].Data[nCurRow] = DivBkNameUp;
										m_PrtDataBox[Field021IBk2Name1_2Slash].Data[nCurRow] = DivBkNameDown;
									}
									else {	// 1行の場合はスラッシュに高さをあわせる
										m_PrtDataBox[Field021IBk2Name1Slash].Data[nCurRow] = DivBkNameUp;
									}

									// 分割後の金融機関名（支店名）取得
									cc.Divide(rc.m_Bk2Name2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

									// 金融機関名（支店名）が1行？
									if (!DivBkNameDown.IsEmpty()) {
										m_PrtDataBox[Field021IBk2Name2_1Slash].Data[nCurRow] = DivBkNameUp;
										m_PrtDataBox[Field021IBk2Name2_2Slash].Data[nCurRow] = DivBkNameDown;
									}
									else {	// 1行の場合はスラッシュに高さをあわせる
										m_PrtDataBox[Field021IBk2Name2Slash].Data[nCurRow] = DivBkNameUp;
									}

									// "/"
									if ((rc.m_BkName1.IsEmpty() == 0) || (rc.m_BkName2.IsEmpty() == 0)) {
										m_PrtDataBox[Field021ISlash2].Data[nCurRow] = "/";
									}
								}
							}

							strBk2NameOld = strBk2Name;
							break;

						default:
							break;
						}
					}

					// 振出年月日
					if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
						m_PrtDataBox[Field021ISpDate2].Data[nCurRow] =
							ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_E2);
					}
					else {
						m_PrtDataBox[Field021ISpDate].Data[nCurRow] =
							ConvertDate(rc.m_SpDate, PRT_FORMAT_DATE_B1);
					}

					// 支払期日
					if (bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1) {
						m_PrtDataBox[Field021IPmDate2].Data[nCurRow] =
							ConvertDate(rc.m_PmDate, PRT_FORMAT_DATE_E2);
					}
					else {
						m_PrtDataBox[Field021IPmDate].Data[nCurRow] =
							ConvertDate(rc.m_PmDate, PRT_FORMAT_DATE_B1);
					}

					// 摘要
					StrDivision(rc.m_Teki, &strArray, 3);
					m_PrtDataBox[Field021ITeki1].Data[nCurRow] = strArray.GetAt(0);
					m_PrtDataBox[Field021ITeki2].Data[nCurRow] = strArray.GetAt(1);
					m_PrtDataBox[Field021ITeki3].Data[nCurRow] = strArray.GetAt(2);
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		return 0;
	}
	catch (...) {
		TRACE("\n# ERROR: CJobFunction::GetDbDataUc021()\n");

		if (rc.IsOpen()) {

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// インボイス登録番号追加対応_23/10/25 add <--

// 簡素化版
int CprtUc021Uketoritegata::GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow			= 0;
// midori 156188_2 add -->
	int				sw=0;
// midori 156188_2 add <--
// midori 157042 add -->
	int				sw2=0;
// midori 157042 add <--
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strBkName		= _T( "" );
	CString			strStName		= _T( "" );
	CString			strBk2Name		= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strBkNameOld	= _T( "" );
	CString			strStNameOld	= _T( "" );
	CString			strBk2NameOld	= _T( "" );
	CString			strSQL;
	CharController		cc;
// midori 191002 add -->
	CString			strkei = _T("");
	CString			strkei2 = _T("");
// midori 191002 add <--

// midori 190505 del -->
//// midori 156188_2 add -->
//	sw = KamokuRowEnableSgn(m_pDB,0);
//// midori 156188_2 add <--
// midori 190505 del <--

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc021Uketoritegata	rc( m_pDB );
	CdbUcInfSub				mfcRecSub( m_pDB );

// midori 190505 add -->
// 改良No.21-0086,21-0529 cor -->
	//sw = KamokuRowEnableSgn(m_pDB,0);
// ------------------------------
	sw = KamokuRowEnableSgn(m_pDB, 0, nFormSeq);
// 改良No.21-0086,21-0529 cor <--
// midori 157042 del -->
//	if(sw == 1) {
//// midori 20/10/08u del -->
//		//if(g_PrnHoho[0] == 2) sw = 0;
//// midori 20/10/08u del <--
//// midori 20/10/08u add -->
//		if(g_PrnHoho[0] == 1) sw = 0;
//// midori 20/10/08u add <--
//	}
//// midori 190505 add <--
// midori 157042 del <--
	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 156188_2 del -->
//////// midori 190505 del -->
//////		//if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
//////// midori 190505 del <--
//////// midori 190505 add -->
//////		if(po.OutZeroNull[0] || po.KamokuSitei[0])	{
//////// midori 190505 add <--
////// midori 156188_2 del <--
////// midori 156188_2 add -->
////		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
////// midori 156188_2 add <--			// 指定されたページの行数を取得（一時テーブル）
//// midori 190505 del <--
//// midori 190505 add -->
//		ck1 = ck2 = 0;
//		if(po.OutZeroNull[0] & BIT_D0)	ck1 = 1;
//		if(po.OutZeroNull[0] & BIT_D1)	ck2 = 1;
//		rt = rc.GetCountDataRecord2(nFormSeq, ck1, ck2);
//		if((po.OutZeroNull[0] && rt != 2) || po.KamokuRow || po.KamokuSitei[0])	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157042 del -->
//// midori 157001 add -->
//		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
//// midori 157001 add <--
// midori 157042 del <--
// midori 157042 add -->
		sw2 = 0;
		if(po.OutZeroNull[0])	sw2 = 1;
		if(po.KamokuSitei[0])	sw2 = 1;
		if(po.KamokuRow){
			if (sw == 0) {
				sw2 = 1;
			}
			else {
				if(rc.GetKmkRowSw() == 0) {
					sw2 = 1;
				}
			}
		}
// midori 157099,157119_2 del -->
//// midori 157098 add -->
//		if(po.KamokuRow == FALSE){
//			if(sw == 1) {
//				if(rc.GetKmkRowSw() == 1) {
//					sw2 = 1;
//				}
//			}
//		}
//// midori 157098 add <--
// midori 157099,157119_2 del <--

		if(sw2 == 1)	{
// midori 157042 add <--
			// 指定されたページの行数を取得（一時テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#endif
			rc.Open( CRecordset::forwardOnly, strSQL );
		}
		else{
			// 通常のテーブルオープン
			m_nRowMax = rc.GetNumRow( nPage );

			rc.Init( nPage );
		}

		// 最終レコードまで処理を実行
		while( !rc.IsEOF() ){

			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if( nCurRow >= 0 ){

				// 空行
				if( CheckDbDataFgFuncNull( &rc ) ){
					
					strAdNameOld	= _T( "" );
					strBkNameOld	= _T( "" );
					strStNameOld	= _T( "" );
					strBk2NameOld	= _T( "" );
					continue;
				}

				// 科目行
				if( rc.m_FgFunc == ID_FGFUNC_KAMOKU ){
					// 科目
					CString strDividedName1, strDividedName2;
					// 分割後の文字列取得
// midori 157042 del -->
//// midori 156189,156190,156191 del -->
//					//cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
//// midori 156189,156190,156191 del <--
//// midori 156189,156190,156191 add -->
//					if(sw == 1) {
//						// 改行文字で2行に分割する
//						m_clsFunc.StrDivision( rc.m_KeiStr, &strArray, 2 );
//						strDividedName1 = strArray.GetAt( 0 );
//						strDividedName2 = strArray.GetAt( 1 );
//					}
//					else {
//						cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
//					}
//// midori 156189,156190,156191 add <--
// midori 157042 del <--
// midori 157042 add -->
					if(sw == 1) {
						strDividedName1.Empty();
						strDividedName2.Empty();
						if (rc.m_KeiStr.IsEmpty() == FALSE) {
							// 改行コードが見つからない場合
							if (rc.m_KeiStr.Find(_T("\r\n")) < 0) {
								// 文字列が10文字を超えている場合は、10文字で分割
								cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, 10 );
							}
							// 改行コードが見つかった場合
							else {
								// 改行文字で2行に分割する
								m_clsFunc.StrDivision( rc.m_KeiStr, &strArray, 2 );
								strDividedName1 = strArray.GetAt( 0 );
								strDividedName2 = strArray.GetAt( 1 );
							}
						}
					}
					else {
						cc.Divide( rc.m_KnName, strDividedName1, strDividedName2, DR_INPUTLENGTH_NORMAL_OLD );
					}
// midori 157042 add <--
					// 9文字以上なら2行表示
					if( !(strDividedName2.IsEmpty()) ){
						// 科目名が15バイトの場合、均等割り付けがずれるので半角スペースで16バイトまで埋める
						if( strDividedName1.GetLength() == 15 ){
							strDividedName1 += _T(" ");
						}
						// 半角スペースで16バイトまで埋める
						strDividedName2 = FillSpace( strDividedName2, DR_INPUTLENGTH_NORMAL_OLD * 2 );

						m_PrtDataBox[Field021KnName1].Data[nCurRow] = strDividedName1;
						m_PrtDataBox[Field021KnName2].Data[nCurRow] = strDividedName2;
					}
					else{
// midori 156189,156190,156191 del -->
						//m_PrtDataBox[Field021KnName].Data[nCurRow] = rc.m_KnName;
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
						if(sw == 1)		m_PrtDataBox[Field021KnName].Data[nCurRow] = rc.m_KeiStr;
						else			m_PrtDataBox[Field021KnName].Data[nCurRow] = rc.m_KnName;
// midori 156189,156190,156191 add <--
					}
					

					rc.MoveNext();
					continue;
				}

				// データ行の０円非表示
				//if( m_PrtInfCommon.HideZero && !m_PrtInfCommon.OutZero ){
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 金額
// No.200903 del -->
				//m_PrtDataBox[Field021Val].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
// No.200903 del <--
// No.200903 add -->
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[Field021Val].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0);
				}
				else	{
					m_PrtDataBox[Field021Val].Data[nCurRow] =
						ShowFieldOfVal(rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0);
				}
// No.200903 add <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strAdNameOld	= _T( "" );
					strBkNameOld	= _T( "" );
					strStNameOld	= _T( "" );
					strBk2NameOld	= _T( "" );

// midori 156189,156190,156191 del -->
//// midori 191002 del -->
//					//m_clsFunc.StrDivisionEx(rc.m_KeiStr,&strArray,10,2);
//					//if(strArray.GetAt(1).IsEmpty() == FALSE)	{
//					//	m_PrtDataBox[Field021KeiStr1].Data[nCurRow] = strArray.GetAt(0);
//					//	m_PrtDataBox[Field021KeiStr2].Data[nCurRow] = strArray.GetAt(1);
//					//}
//					//else	{
//					//	m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
//					//}
//// midori 191002 del <--
//// midori 191002 add -->
//					m_clsFunc.KeiStrSprit(rc.m_KeiStr, &strkei, &strkei2);
//					if (strkei2.IsEmpty() == FALSE) {
//						m_PrtDataBox[Field021KeiStr1].Data[nCurRow] = strkei;
//// midori 155541,155542 del -->
//						//if (strkei2.GetLength() > 20) {
//						//	m_PrtDataBox[Field021KeiStr2].Data[nCurRow] = strkei2;
//						//}
//						//else	{
//						//	m_PrtDataBox[Field021KeiStr4].Data[nCurRow] = strkei2;
//						//}
//// midori 155541,155542 del <--
//// midori 155541,155542 add -->
//						if (strkei2.GetLength() > 24) {
//							m_PrtDataBox[Field021KeiStr2].Data[nCurRow] = strkei2;
//						}
//						else if (strkei2.GetLength() > 22) {
//							m_PrtDataBox[Field021KeiStr6].Data[nCurRow] = strkei2;
//						}
//						else	{
//							m_PrtDataBox[Field021KeiStr4].Data[nCurRow] = strkei2;
//						}
//// midori 155541,155542 add <--
//					}
//					else {
//						if (rc.m_KeiStr.GetLength() > 22)		m_PrtDataBox[Field021KeiStr3].Data[nCurRow] = rc.m_KeiStr;
//						else if (rc.m_KeiStr.GetLength() > 20)	m_PrtDataBox[Field021KeiStr5].Data[nCurRow] = rc.m_KeiStr;
//						else									m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
//					}
//// midori 191002 add <--
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
// 157154 del -->
					//// 小計名称編集済みサインON
					//if(rc.m_ShowKeiZero & BIT_D7) {
// 157154 del <--
// 157154 add -->
					// (小計名称 または 中計名称) かつ 小計名称編集可能
					if((rc.m_FgFunc == ID_FGFUNC_SYOKEI || rc.m_FgFunc == ID_FGFUNC_CHUKEI) && sw == 1 ) {
// 157154 add <--
						m_clsFunc.StrDivision( rc.m_KeiStr, &strArray, 2 );
						strkei = strArray.GetAt( 0 );
						strkei2 = strArray.GetAt( 1 );
						// 2行出力
						if (strkei2.IsEmpty() == FALSE) {
							// 1行目
							if (strkei.GetLength() > 24) {
								m_PrtDataBox[Field021KeiStr8].Data[nCurRow] = strkei;
							}
							else if (strkei.GetLength() > 22) {
								m_PrtDataBox[Field021KeiStr7].Data[nCurRow] = strkei;
							}
							else	{
								m_PrtDataBox[Field021KeiStr1].Data[nCurRow] = strkei;
							}
							// 2行目
							if (strkei2.GetLength() > 24) {
								m_PrtDataBox[Field021KeiStr2].Data[nCurRow] = strkei2;
							}
							else if (strkei2.GetLength() > 22) {
								m_PrtDataBox[Field021KeiStr6].Data[nCurRow] = strkei2;
							}
							else	{
								m_PrtDataBox[Field021KeiStr4].Data[nCurRow] = strkei2;
							}
						}
						// 1行出力
						else {
							if (strkei.GetLength() > 24) {
								m_PrtDataBox[Field021KeiStr10].Data[nCurRow] = strkei;
							}
							else if (strkei.GetLength() > 22) {
								m_PrtDataBox[Field021KeiStr9].Data[nCurRow] = strkei;
							}
							else if (rc.m_KeiStr.GetLength() > 20)	{
								m_PrtDataBox[Field021KeiStr5].Data[nCurRow] = strkei;
							}
							else	{
								m_PrtDataBox[Field021KeiStr].Data[nCurRow] = strkei;
							}
						}
					}
					// 小計名称編集済みサインOFF
					else {
						m_clsFunc.KeiStrSprit(rc.m_KeiStr, &strkei, &strkei2);
						if (strkei2.IsEmpty() == FALSE) {
							m_PrtDataBox[Field021KeiStr1].Data[nCurRow] = strkei;
							if (strkei2.GetLength() > 24) {
								m_PrtDataBox[Field021KeiStr2].Data[nCurRow] = strkei2;
							}
							else if (strkei2.GetLength() > 22) {
								m_PrtDataBox[Field021KeiStr6].Data[nCurRow] = strkei2;
							}
							else	{
								m_PrtDataBox[Field021KeiStr4].Data[nCurRow] = strkei2;
							}
						}
						else {
							if (rc.m_KeiStr.GetLength() > 22)		m_PrtDataBox[Field021KeiStr3].Data[nCurRow] = rc.m_KeiStr;
							else if (rc.m_KeiStr.GetLength() > 20)	m_PrtDataBox[Field021KeiStr5].Data[nCurRow] = rc.m_KeiStr;
							else									m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
						}
					}
// midori 156189,156190,156191 add <--
				}
				else	{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 振出人
							case 1:
								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;
								if(CheckOmit(strAdName,strAdNameOld,i,m_PrtInfOmitList1))	{
									m_PrtDataBox[Field021AdNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
								}
								else	{
									StrDivision(strAdName,&strArray,2);
									m_PrtDataBox[Field021AdName1].Data[nCurRow] = strArray.GetAt(0);
									m_PrtDataBox[Field021AdName2].Data[nCurRow] = strArray.GetAt(1);
								}
								strAdNameOld = strAdName;
								break;

							// 支払銀行名称
							case 2:
								strBkName = rc.m_BkName1;
								if(CheckOmit(strBkName,strBkNameOld,i,m_PrtInfOmitList1))	{
									m_PrtDataBox[Field021BkNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
								}
								else	{
									CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名
									// 分割後の金融機関名（銀行名）取得
									cc.Divide(rc.m_BkName1,DivBkNameUp,DivBkNameDown,ONE_LINE_CHAR_NUM);
									// 金融機関名（銀行名）が1行？
									if(!DivBkNameDown.IsEmpty())	{
										// 2行出力
										m_PrtDataBox[Field021BkName1_1Slash].Data[nCurRow] = DivBkNameUp;
										m_PrtDataBox[Field021BkName1_2Slash].Data[nCurRow] = DivBkNameDown;
									}
									else	{
										// 1行出力
										m_PrtDataBox[Field021BkName1_2Slash].Data[nCurRow] = DivBkNameUp;
									}
								}
								strBkNameOld = strBkName;
								break;

							// 支払銀行支店名
							case 3:
								strStName = rc.m_BkName2;
								if(CheckOmit(strStName,strStNameOld,i,m_PrtInfOmitList1))	{
									m_PrtDataBox[Field021StNameOm].Data[nCurRow] = m_PrtInfOmitList1[i].OmitStr;
								}
								else	{
									CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名
									// 分割後の金融機関名（銀行名）取得
									cc.Divide(rc.m_BkName2,DivBkNameUp,DivBkNameDown,ONE_LINE_CHAR_NUM);
									// 金融機関名（銀行名）が1行？
									if(!DivBkNameDown.IsEmpty())	{
										// 2行出力
										m_PrtDataBox[Field021BkName2_1Slash].Data[nCurRow] = DivBkNameUp;
										m_PrtDataBox[Field021BkName2_2Slash].Data[nCurRow] = DivBkNameDown;
									}
									else	{
										// 1行出力
										m_PrtDataBox[Field021BkName2_2Slash].Data[nCurRow] = DivBkNameUp;
									}
								}
								strStNameOld = strStName;
								break;

							// 割引銀行名等
							case 4:

								strBk2Name = rc.m_Bk2Name1 + PRT_FORMAT_CHCODE + rc.m_Bk2Name2;

								if( CheckOmit( strBk2Name, strBk2NameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field021Bk2NameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									if( po.KinyuLine == 0 ){

										m_PrtDataBox[Field021Bk2Name1].Data[nCurRow] = rc.m_Bk2Name1;
										m_PrtDataBox[Field021Bk2Name2].Data[nCurRow] =
											InsertSlashInHead( rc.m_Bk2Name1, rc.m_Bk2Name2 );
									}
									else{
										CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名

										// 分割後の金融機関名（銀行名）取得
										cc.Divide(rc.m_Bk2Name1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

										// 金融機関名（銀行名）が1行？
										if( !DivBkNameDown.IsEmpty() ){
											m_PrtDataBox[Field021Bk2Name1_1Slash].Data[nCurRow] = DivBkNameUp;
											m_PrtDataBox[Field021Bk2Name1_2Slash].Data[nCurRow] = DivBkNameDown;
										}
										else{	// 1行の場合はスラッシュに高さをあわせる
											m_PrtDataBox[Field021Bk2Name1Slash].Data[nCurRow] = DivBkNameUp;
										}

										// 分割後の金融機関名（支店名）取得
										cc.Divide(rc.m_Bk2Name2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

										// 金融機関名（支店名）が1行？
										if( !DivBkNameDown.IsEmpty() ){
											m_PrtDataBox[Field021Bk2Name2_1Slash].Data[nCurRow] = DivBkNameUp;
											m_PrtDataBox[Field021Bk2Name2_2Slash].Data[nCurRow] = DivBkNameDown;
										}
										else{	// 1行の場合はスラッシュに高さをあわせる
											m_PrtDataBox[Field021Bk2Name2Slash].Data[nCurRow] = DivBkNameUp;
										}

										// "/"
										if(( rc.m_BkName1.IsEmpty() == 0) || ( rc.m_BkName2.IsEmpty() == 0 )){
											m_PrtDataBox[Field021Slash2].Data[nCurRow] = "/";
										}
									}
								}

								strBk2NameOld = strBk2Name;
								break;

							default:
								break;
						}
					}

// midori 200101 del -->
					//// 振出年月日
					//m_PrtDataBox[Field021SpDate].Data[nCurRow] =
					//	ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );

					//// 支払期日
					//m_PrtDataBox[Field021PmDate].Data[nCurRow] =
					//	ConvertDate( rc.m_PmDate, PRT_FORMAT_DATE_B1 );
// midori 200101 del <--
// midori 200101 add -->
					// 振出年月日
					if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
// midori 155870 del -->
						//m_PrtDataBox[Field021SpDate].Data[nCurRow] =
						//	ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E2 );
// midori 155870 del <--
// midori 155870 add -->
						m_PrtDataBox[Field021SpDate2].Data[nCurRow] =
							ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_E2 );
// midori 155870 add <--
					}
					else	{
						m_PrtDataBox[Field021SpDate].Data[nCurRow] =
							ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );
					}

					// 支払期日
					if(bG_Kanso == TRUE && m_PrtInfCommon.OutGengo == 1)	{
// midori 155870 del -->
						//m_PrtDataBox[Field021PmDate].Data[nCurRow] =
						//	ConvertDate( rc.m_PmDate, PRT_FORMAT_DATE_E2 );
// midori 155870 del <--
// midori 155870 add -->
						m_PrtDataBox[Field021PmDate2].Data[nCurRow] =
							ConvertDate( rc.m_PmDate, PRT_FORMAT_DATE_E2 );
// midori 155870 add <--
					}
					else	{
						m_PrtDataBox[Field021PmDate].Data[nCurRow] =
							ConvertDate( rc.m_PmDate, PRT_FORMAT_DATE_B1 );
					}
// midori 200101 add <--

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 3 );
					m_PrtDataBox[Field021Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field021Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[Field021Teki3].Data[nCurRow] = strArray.GetAt( 2 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		//return m_nRowMax;
		return 0;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc021()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

int CprtUc021Uketoritegata::GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow			= 0;
	CStringArray	strArray;
	CString			strAdName		= _T( "" );
	CString			strBkName		= _T( "" );
	CString			strBk2Name		= _T( "" );
	CString			strAdNameOld	= _T( "" );
	CString			strBkNameOld	= _T( "" );
	CString			strBk2NameOld	= _T( "" );
	CString			strSQL;
	CharController		cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc021Uketoritegata	rc( m_pDB );
	CdbUcInfSub				mfcRecSub( m_pDB );

	try{
// midori 156188_2 del -->
//// midori 190505 del -->
////// midori 160610 cor -->
//////		if(po.OutZeroNull[0] || po.KamokuRow){
////// ---------------------
////		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
////// midori 160610 cor <--
//// midori 190505 del <--
//// midori 190505 add -->
//		if(po.OutZeroNull[0] || po.KamokuSitei[0])	{
//// midori 190505 add <--
// midori 156188_2 del <--
// midori 156188_2 add -->
		if(po.OutZeroNull[0] || po.KamokuRow || po.KamokuSitei[0])	{
// midori 156188_2 add <--			// 指定されたページの行数を取得（一時テーブル）
			// 指定されたページの行数を取得（一時テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

// midori 160610 cor -->
//			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
// ---------------------
#ifdef _DEBUG
			strSQL.Format("SELECT * FROM ##temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#else
			strSQL.Format("SELECT * FROM #temp_utiwake_tbl_%d WHERE NumPage = %d ORDER BY NumPage, NumRow", m_nFormSeq, nPage );
#endif
// midori 160610 cor <--
			rc.Open( CRecordset::forwardOnly, strSQL );
		}
		else{
			// 通常のテーブルオープン
			m_nRowMax = rc.GetNumRow( nPage );

			rc.Init( nPage );
		}

		// 最終レコードまで処理を実行
		while( !rc.IsEOF() ){

			nCurRow = rc.m_NumRow - 1;

			// 行番号が存在するときのみ実行
			if( nCurRow >= 0 ){

				// 空行
				if( CheckDbDataFgFuncNull( &rc ) ){
					
					strAdNameOld	= _T( "" );
					strBkNameOld	= _T( "" );
					strBk2NameOld	= _T( "" );
					continue;
				}

				// 科目行
				if( rc.m_FgFunc == ID_FGFUNC_KAMOKU ){
					// 科目
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

						m_PrtDataBox[Field021KnName1].Data[nCurRow] = strDividedName1;
						m_PrtDataBox[Field021KnName2].Data[nCurRow] = strDividedName2;
					}
					else{
						m_PrtDataBox[Field021KnName].Data[nCurRow] = rc.m_KnName;
					}
					

					rc.MoveNext();
					continue;
				}

				// データ行の０円非表示
				//if( m_PrtInfCommon.HideZero && !m_PrtInfCommon.OutZero ){
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val = HideZeroMoney( rc.m_Val );
					}
				}

				// 金額
				m_PrtDataBox[Field021Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strAdNameOld	= _T( "" );
					strBkNameOld	= _T( "" );
					strBk2NameOld	= _T( "" );

// midori 150917 cor -->
					//m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
// ---------------------
					m_clsFunc.StrDivisionEx(rc.m_KeiStr,&strArray,10,2);
					if(strArray.GetAt(1).IsEmpty() == FALSE)	{
						m_PrtDataBox[Field021KeiStr1].Data[nCurRow] = strArray.GetAt(0);
						m_PrtDataBox[Field021KeiStr2].Data[nCurRow] = strArray.GetAt(1);
					}
					else	{
						m_PrtDataBox[Field021KeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
// midori 150917 cor <--
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 振出人
							case 1:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field021AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field021AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field021AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

								// 支払銀行名
							case 2:

								//								strBkName = rc.m_BkName1 + PRT_FORMAT_SLASH + rc.m_BkName2;
								strBkName = rc.m_BkName1 + PRT_FORMAT_CHCODE + rc.m_BkName2;

								if( CheckOmit( strBkName, strBkNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field021BkNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									if( po.KinyuLine == 0 ){	// 印刷設定→金融機関を一段で表示する

										m_PrtDataBox[Field021BkName1].Data[nCurRow] = rc.m_BkName1;
										m_PrtDataBox[Field021BkName2].Data[nCurRow] =
											InsertSlashInHead( rc.m_BkName1, rc.m_BkName2 );
									}
									else{
										CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名

										// 分割後の金融機関名（銀行名）取得
										cc.Divide(rc.m_BkName1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

										// 金融機関名（銀行名）が1行？
										if( !DivBkNameDown.IsEmpty() ){
											m_PrtDataBox[Field021BkName1_1Slash].Data[nCurRow] = DivBkNameUp;
											m_PrtDataBox[Field021BkName1_2Slash].Data[nCurRow] = DivBkNameDown;
										}
										else{	// 1行の場合はスラッシュに高さをあわせる
											m_PrtDataBox[Field021BkName1Slash].Data[nCurRow] = DivBkNameUp;
										}

										// 分割後の金融機関名（支店名）取得
										cc.Divide(rc.m_BkName2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

										// 金融機関名（支店名）が1行？
										if( !DivBkNameDown.IsEmpty() ){
											m_PrtDataBox[Field021BkName2_1Slash].Data[nCurRow] = DivBkNameUp;
											m_PrtDataBox[Field021BkName2_2Slash].Data[nCurRow] = DivBkNameDown;
										}
										else{	// 1行の場合はスラッシュに高さをあわせる
											m_PrtDataBox[Field021BkName2Slash].Data[nCurRow] = DivBkNameUp;
										}

										// "/"
										if(( rc.m_BkName1.IsEmpty() == 0) || ( rc.m_BkName2.IsEmpty() == 0 )){
											m_PrtDataBox[Field021Slash].Data[nCurRow] = "/";
										}
									}
								}

								strBkNameOld = strBkName;
								break;

							// 割引銀行名等
							case 3:

//								strBk2Name = rc.m_Bk2Name1 + PRT_FORMAT_SLASH + rc.m_Bk2Name2;
								strBk2Name = rc.m_Bk2Name1 + PRT_FORMAT_CHCODE + rc.m_Bk2Name2;

								if( CheckOmit( strBk2Name, strBk2NameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field021Bk2NameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									if( po.KinyuLine == 0 ){

										m_PrtDataBox[Field021Bk2Name1].Data[nCurRow] = rc.m_Bk2Name1;
										m_PrtDataBox[Field021Bk2Name2].Data[nCurRow] =
											InsertSlashInHead( rc.m_Bk2Name1, rc.m_Bk2Name2 );
									}
									else{
										CString DivBkNameUp, DivBkNameDown;	// 分割後の1行目、2行目の金融機関名

										// 分割後の金融機関名（銀行名）取得
										cc.Divide(rc.m_Bk2Name1, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

										// 金融機関名（銀行名）が1行？
										if( !DivBkNameDown.IsEmpty() ){
											m_PrtDataBox[Field021Bk2Name1_1Slash].Data[nCurRow] = DivBkNameUp;
											m_PrtDataBox[Field021Bk2Name1_2Slash].Data[nCurRow] = DivBkNameDown;
										}
										else{	// 1行の場合はスラッシュに高さをあわせる
											m_PrtDataBox[Field021Bk2Name1Slash].Data[nCurRow] = DivBkNameUp;
										}

										// 分割後の金融機関名（支店名）取得
										cc.Divide(rc.m_Bk2Name2, DivBkNameUp, DivBkNameDown, ONE_LINE_CHAR_NUM);

										// 金融機関名（支店名）が1行？
										if( !DivBkNameDown.IsEmpty() ){
											m_PrtDataBox[Field021Bk2Name2_1Slash].Data[nCurRow] = DivBkNameUp;
											m_PrtDataBox[Field021Bk2Name2_2Slash].Data[nCurRow] = DivBkNameDown;
										}
										else{	// 1行の場合はスラッシュに高さをあわせる
											m_PrtDataBox[Field021Bk2Name2Slash].Data[nCurRow] = DivBkNameUp;
										}

										// "/"
										if(( rc.m_BkName1.IsEmpty() == 0) || ( rc.m_BkName2.IsEmpty() == 0 )){
											m_PrtDataBox[Field021Slash2].Data[nCurRow] = "/";
										}
									}
								}

								strBk2NameOld = strBk2Name;
								break;

							default:
								break;
						}
					}

					// 振出年月日
					m_PrtDataBox[Field021SpDate].Data[nCurRow] =
						ConvertDate( rc.m_SpDate, PRT_FORMAT_DATE_B1 );

					// 支払期日
					m_PrtDataBox[Field021PmDate].Data[nCurRow] =
						ConvertDate( rc.m_PmDate, PRT_FORMAT_DATE_B1 );

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 2 );
					m_PrtDataBox[Field021Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field021Teki2].Data[nCurRow] = strArray.GetAt( 1 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		//return m_nRowMax;
		return 0;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc021()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}
// midori 190301 add <--

CprtUc021Uketoritegata::CprtUc021Uketoritegata(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_021;
	m_nRowMax	= 0;
// インボイス登録番号追加対応_23/10/25 del -->
	//m_nColumn	= Field021Max;
// インボイス登録番号追加対応_23/10/25 del <--
// インボイス登録番号追加対応_23/10/25 add -->
	if(bG_InvNo == TRUE)	m_nColumn	= Field021IMax;
	else					m_nColumn	= Field021Max;
// インボイス登録番号追加対応_23/10/25 add <--
}

CprtUc021Uketoritegata::~CprtUc021Uketoritegata(void)
{
}

CprtJobFunction* CprtUc021Uketoritegata::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc021Uketoritegata(m_pDB));
}