//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc131uriagedaka.cpp
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
#include "prtUc131Uriagedaka.h"

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataUc131()
//
// ■DBから、指定されたページのデータを取得し、データボックスに格納する。
//
// ⑬．売上高等の事業所別
//
// 引数		：int				nPage				ページ番号
//
// 戻り値	：int				行数				（FUNCTION_NG：失敗）
//
////////////////////////////////////////////////////////////////////////////////
int CprtUc131Uriagedaka::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int				nCurRow			= 0;
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
	CString			strHdName1Old	= _T( "" );
	CString			strKankeiOld	= _T( "" );
	CString			strSQL;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc131Uriagedaka	rc( m_pDB );

	// 計の出力設定
	int nOutKei = rc.GetOutKei( ID_FORMNO_131 );

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
					
					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strHdName1Old	= _T( "" );
					strKankeiOld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_Val2	= HideZeroMoney( rc.m_Val2 );
					}
				}

				// 売上高
				m_PrtDataBox[Field131Val].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				// 期末棚卸高
				m_PrtDataBox[Field131Val2].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D1 );

				// 期末従業員数
				m_PrtDataBox[Field131Worker].Data[nCurRow] =
					ShowFieldOfVal( rc.m_Worker, &rc, MakeFormatOfNum( 7 ), BIT_D2 );

// midori 190301 add -->
				if(bG_Kanso == FALSE)	{
// midori 190301 add <--
					// 使用建物の延面積
					m_PrtDataBox[Field131Area].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Area, &rc, MakeFormatOfNum( 7, 2 ), BIT_D3 );
// midori 190301 add -->
				}
// midori 190301 add <--

				// 計行
				if(	CheckDbDataFgFuncKei( &rc ) ){

					strAdNameOld	= _T( "" );
					strAdAddOld		= _T( "" );
					strHdName1Old	= _T( "" );
					strKankeiOld	= _T( "" );
					// 最終行
					if( nCurRow == (PRT_ROW_131 - 1) ){

						m_PrtDataBox[Field131DnKeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
					else{

						m_PrtDataBox[Field131KeiStr].Data[nCurRow] = rc.m_KeiStr;
					}
				}
				else{
					// 省略文字使用欄の判別
					for( int i = 0; i < PRT_MAX_OMIT; i++ ){

						int nItemSeq = m_PrtInfOmitList1[i].ItemSeq;

						switch( nItemSeq ){

							// 事業所の名称（名称）
							case 1:

								strAdName = rc.m_AdName1 + PRT_FORMAT_CHCODE + rc.m_AdName2;

								if( CheckOmit( strAdName, strAdNameOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field131AdNameOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{
									
									StrDivision( strAdName, &strArray, 2 );
									m_PrtDataBox[Field131AdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field131AdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdNameOld = strAdName;
								break;

							// 所在地（所在地）
							case 2:

								strAdAdd = rc.m_AdAdd1 + PRT_FORMAT_CHCODE + rc.m_AdAdd2;

								if( CheckOmit( strAdAdd, strAdAddOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field131AdAddOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( strAdAdd, &strArray, 2 );
									m_PrtDataBox[Field131AdAdd1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field131AdAdd2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strAdAddOld = strAdAdd;
								break;

							// 責任者氏名
							case 3:

								if( CheckOmit( rc.m_HdName, strHdName1Old, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field131HdName1Om].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									StrDivision( rc.m_HdName, &strArray, 2 );
									m_PrtDataBox[Field131HdName1].Data[nCurRow] = strArray.GetAt( 0 );
									m_PrtDataBox[Field131HdName2].Data[nCurRow] = strArray.GetAt( 1 );
								}

								strHdName1Old = rc.m_HdName;
								break;

							// 代表者との関係
							case 4:

								if( CheckOmit( rc.m_Kankei, strKankeiOld, i, m_PrtInfOmitList1 ) ){

									m_PrtDataBox[Field131KankeiOm].Data[nCurRow] =
										m_PrtInfOmitList1[i].OmitStr;
								}
								else{

									m_PrtDataBox[Field131Kankei].Data[nCurRow] = rc.m_Kankei;
								}

								strKankeiOld = rc.m_Kankei;
								break;

							default:
								break;
						}
					}

					// 事業等の内容
					StrDivision( rc.m_Naiyou, &strArray, 3 );
					m_PrtDataBox[Field131Naiyou1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field131Naiyou2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[Field131Naiyou3].Data[nCurRow] = strArray.GetAt( 2 );

					// 源泉取得納税付署
					StrDivision( rc.m_HdName2, &strArray, 2 );
					m_PrtDataBox[Field131HdName21].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field131HdName22].Data[nCurRow] = strArray.GetAt( 1 );

					// 摘要
					StrDivision( rc.m_Teki, &strArray, 3 );
					m_PrtDataBox[Field131Teki1].Data[nCurRow] = strArray.GetAt( 0 );
					m_PrtDataBox[Field131Teki2].Data[nCurRow] = strArray.GetAt( 1 );
					m_PrtDataBox[Field131Teki3].Data[nCurRow] = strArray.GetAt( 2 );
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

		TRACE( "\n# ERROR: CJobFunction::GetDbDataUc131()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

CprtUc131Uriagedaka::CprtUc131Uriagedaka(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_131;
	m_nRowMax	= 0;
	m_nColumn	= Field131Max;
}

CprtUc131Uriagedaka::~CprtUc131Uriagedaka(void)
{
}

CprtJobFunction* CprtUc131Uriagedaka::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc131Uriagedaka(m_pDB));
}