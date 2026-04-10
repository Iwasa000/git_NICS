//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc051Tanaorosi.cpp
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
#include "prtUc051Tanaorosi.h"
#include "ConvOutRangai.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc051()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑤．棚卸資産
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc051Tanaorosi::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow			= 0;
	int				nMaxPage		= 0;
// midori 157001 del -->
//// midori 190505 add -->
//	int				rt=0;
//	int				ck1=0,ck2=0;
//// midori 190505 add <--
// midori 157001 del <--
	CStringArray	strArray;
	CString			strKnNameOld	= _T( "" );
	CString			strHdNameOld	= _T( "" );
	CString			strSQL;
	CString			strMaxPage;
	CharController	cc;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc051Tanaoroshi	rc( m_pDB );

	try{
// midori 157001 del -->
//// midori 190505 del -->
////// midori 160610 cor -->
//////		if(!(po.OutZeroNull[0])){
////// ---------------------
////		if(!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)){
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
		if(!(po.OutZeroNull[0]) && (po.KamokuSitei[0] == 0)){
// midori 157001 add <--
			// 指定されたページの行数を取得
			m_nRowMax = rc.GetNumRow( nPage );	

			rc.Init( nPage );

			// 最大ページ数取得
			nMaxPage = rc.GetNumPage();
		}
		else{
			// 指定されたページの行数を取得（０円テーブル）
			m_nRowMax = rc.GetMaxRowFromTempTbl( m_nFormSeq );

			CRecordset rs( m_pDB );
			// 最大ページ数取得
// midori 160610 cor -->
			//strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM #temp_utiwake_tbl_%d ", m_nFormSeq );
// ---------------------
#ifdef _DEBUG
			strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM ##temp_utiwake_tbl_%d ", m_nFormSeq );
#else
			strSQL.Format( "SELECT MAX(NumPage) AS 'maxpage' FROM #temp_utiwake_tbl_%d ", m_nFormSeq );
#endif
// midori 160610 cor <--
			rs.Open( CRecordset::forwardOnly, strSQL );

			rs.GetFieldValue( _T("maxpage"), strMaxPage );

			sscanf_s( strMaxPage, "%d", &nMaxPage);
			rs.Close();

			// オープン
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
					strHdNameOld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_Tanni	= HideZeroMoney( rc.m_Tanni );
						rc.m_Price	= HideZeroMoney( rc.m_Price );
					}
				}

				// 期末現在高
				m_PrtDataBox[Field051Val].Data[nCurRow] = 
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );


				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strKnNameOld	= _T( "" );
					strHdNameOld	= _T( "" );

					m_PrtDataBox[Field051KeiStr].Data[nCurRow] = rc.m_KeiStr;
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

					int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( m_PrtInfOmitList1[i].ItemSeq ){

						// 科目（名称）
						case 1:

							if( CheckOmit( rc.m_KnName, strKnNameOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field051KnNameOm].Data[nCurRow] =
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

									m_PrtDataBox[Field051KnName1].Data[nCurRow] = strDividedName1;
									m_PrtDataBox[Field051KnName2].Data[nCurRow] = strDividedName2;
								}
								else{
									m_PrtDataBox[Field051KnName2].Data[nCurRow] = rc.m_KnName;
								}
							}

							strKnNameOld = rc.m_KnName;
							break;

						// 品目
						case 2:

							if( CheckOmit( rc.m_HdName, strHdNameOld, i, m_PrtInfOmitList1 ) ){

								m_PrtDataBox[Field051HdNameOm].Data[nCurRow] =
									m_PrtInfOmitList1[i].OmitStr;
							}
							else{

								StrDivision( rc.m_HdName, &strArray, 2 );
								m_PrtDataBox[Field051HdName1].Data[nCurRow] = strArray.GetAt( 0 );
								m_PrtDataBox[Field051HdName2].Data[nCurRow] = strArray.GetAt( 1 );
							}

							strHdNameOld = rc.m_HdName;
							break;

						default:
							break;
						}
					}

					// 数量
					m_PrtDataBox[Field051Suuryou].Data[nCurRow] =
						FixUnderDotZero( ShowFieldOfVal( rc.m_Suuryou, &rc, MakeFormatOfNum( 7, 2 ) ) );

					// 単位
					m_PrtDataBox[Field051Tanni].Data[nCurRow] = ShowFieldOfVal( rc.m_Tanni, &rc );

					// 単価
					m_PrtDataBox[Field051Price].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Price, &rc, MakeFormatOfNum( 8, 2 ) );

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 2 );
					m_PrtDataBox[Field051Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field051Teki2].Data[nCurRow] = strArray.GetAt( 1 );
				}
			}

			// 次のレコードへ移動
			rc.MoveNext();
		}

		// DBアクセスの終了
		rc.Fin();

		// 出力状態に応じて、下欄のデータを表示
		// 1ページ目のみ、下欄のデータを表示
		//if( nPage == nOutPage ){
// midori 190301 del -->
		//int nRowMax2 = GetDbDataUc052( nPage, nMaxPage );
// midori 190301 del <--
// midori 190301 add -->
		int nRowMax2=0;
		if(bG_Kanso != TRUE)	{
			nRowMax2 = GetDbDataUc052(nPage,nMaxPage);
		}
// midori 190301 add <--

		// 行数の多い方を使用
		if( nRowMax2 > m_nRowMax ){

			m_nRowMax = nRowMax2;
		}
		//}

		return m_nRowMax;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc051()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc052()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑤．棚卸資産
// 引数		：int				現在のページ数
//			：int				最大ページ数
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc051Tanaorosi::GetDbDataUc052(int nPage, int nMaxPage)
{
	CdbUc052Tanaoroshi2	rc(m_pDB);		// 1ページ目を取得し、欄外出力形式等を取得
	CdbUc052Tanaoroshi2	rc2(m_pDB);		// 実際のデータが入っているテーブル　rcと共通にするとうまく.Open()できない為別インスタンス
	try{
		rc.RequeryPage(1);
		if(rc.IsEOF())	{
			rc.Fin();
			return 0;
		}

		// 欄外出力形式	出力ページはNumPage = 1のレコードにのみセットされている
		CConvOutRangai cr;
		int PrintType = 0;
		cr.ToMemory(rc.m_OutRangai, m_OutType, PrintType);
		int requery_page = nPage;
		if(m_OutType != 2 || PrintType == 0){	// 全ページ以外　またはページ共通出力なら強制的に欄外1ページ目を出力する
			requery_page = 1;	// 全ページ以外
			rc2.RequeryPage(requery_page);
			if(rc2.IsEOF()){
				rc2.Fin();
				return 0;
			}
		}
		else{
			CString strSQL;
			CString tmp_tbl = rc2.GetRangaiTblName();
			strSQL.Format("SELECT * FROM %s WHERE NumPage = %d ORDER BY NumPage, NumRow", tmp_tbl, requery_page);
			rc2.Open(CRecordset::forwardOnly, strSQL);
		}

		if(rc2.IsEOF()){
			rc.Fin();
			rc2.Fin();
			return 0;	// 全て印字しないor指定ページが印字しない
		}
		if(rc2.m_FgShow == ID_FGSHOW_TANAOROSHI){
			rc.Fin();
			rc2.Fin();
			return 0;	// 印字しない
		}

		int	nOutPage = 1;	// 棚卸方法出力ページ（旧：欄外登録）
		switch( m_OutType ){
			case 0:				// 1ページ
				nOutPage = 1;
				break;
			case 1:				// 最終ページ
// midori M-16113005 cor -->
//				nOutPage = nMaxPage;		
// -------------------------
				nOutPage = nMaxPage;
				if(nOutPage == 0)	nOutPage = 1;
// midori M-16113005 cor <--
				break;
			case 2:				// 全ページ
				nOutPage = nPage;
				break;
			default:
				nOutPage = 0;
				break;
		}

		if( nPage == nOutPage ){
			// 期末棚卸の方法
			switch( rc2.m_Method ){
				case 1:	// A
					m_PrtDataBox[Field052Method1].Data[0] = PRT_FORMAT_CHECK;
					break;
				case 2:	// B
					m_PrtDataBox[Field052Method2].Data[0] = PRT_FORMAT_CHECK;
					break;
				case 3:	// C
					m_PrtDataBox[Field052Method3].Data[0] = PRT_FORMAT_CHECK;
					break;
				case 4:	// 登録名称
					m_PrtDataBox[Field052HdName].Data[0] = rc2.m_HdName; 
					break;
				default:
					break;
			}
			// 棚卸を行った時期
			m_PrtDataBox[Field052SpDate].Data[0] = ConvertDate( rc2.m_SpDate, PRT_FORMAT_DATE_C2 );
		}

		rc.Fin();
		rc2.Fin();

		return 1;	// 必ず1行
	}
	catch(...){
		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc052()\n" );
		if(rc.IsOpen())		rc.Fin();
		if(rc2.IsOpen())	rc2.Fin();
		return FUNCTION_NG;
	}

	return FUNCTION_OK;
}

CprtUc051Tanaorosi::CprtUc051Tanaorosi(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_051;
	m_nRowMax	= 0;
	m_nColumn	= Field051Max;
}

CprtUc051Tanaorosi::~CprtUc051Tanaorosi(void)
{
}

void CprtUc051Tanaorosi::SetRangaiData(const CdbUc052Tanaoroshi2& db)
{
	// 期末棚卸の方法
	switch(db.m_Method){
		case 1:	// A
			m_PrtDataBox[Field052Method1].Data[0] = PRT_FORMAT_CHECK;
			break;
		case 2:// B
			m_PrtDataBox[Field052Method2].Data[0] = PRT_FORMAT_CHECK;
			break;
		case 3:// C
			m_PrtDataBox[Field052Method3].Data[0] = PRT_FORMAT_CHECK;
			break;
		case 4:// 登録名称
			m_PrtDataBox[Field052HdName].Data[0] = db.m_HdName; 
			break;
		default:
			break;
	}

	// 棚卸を行った時期
	m_PrtDataBox[Field052SpDate].Data[0] = ConvertDate( db.m_SpDate, PRT_FORMAT_DATE_C2 );
}

int CprtUc051Tanaorosi::GetPrintPage(int position, int max_page, int cur_page)
{
	if(position == 0)		return 1;
	else if(position == 1)	return max_page;
	else if(position == 2)	return cur_page;
	else					return 0;
}