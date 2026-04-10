//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc102Gensen.cpp
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
#include ".\prtJobFunction.h"
#include "prtUc102Gensen.h"

// midori 200101 add -->
extern BOOL	bG_Kanso;
// midori 200101 add <--

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
int CprtUc102Gensen::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
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
	GetPrintInfo( parent );

	CdbUc102Gensen	rc( m_pDB );

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
					
					strKnNameLOld	= _T( "" );
					strKnNameROld	= _T( "" );
					continue;
				}

				// データ行の０円非表示
				if( CheckDbDataFgFuncKei( &rc ) == FALSE ){
					if(po.HideZero[0]){
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

CprtUc102Gensen::CprtUc102Gensen(CDatabase* pdb)
{
	m_pDB		= pdb;
	m_nFormSeq	= ID_FORMNO_102;
	m_nRowMax	= 0;
	m_nColumn	= Field102Max;
}

CprtUc102Gensen::~CprtUc102Gensen(void)
{
}

CprtJobFunction* CprtUc102Gensen::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc102Gensen(m_pDB));
}