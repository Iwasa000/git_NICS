//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtUc142Jinkenhi.cpp
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
#include "prtUc142Jinkenhi.h"

// No.200903 add -->
extern BOOL	bG_Kanso;
// No.200903 add <--

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
int CprtUc142Jinkenhi::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq/*=0*/)
{
	int	nCurRow	= 0;
	int	index	= 0;

	// 省略文字の取得
	GetPrintInfo( parent );

	CdbUc142Jinkenhi	rc( m_pDB );

	try{

		// 指定されたページの行数を取得
		m_nRowMax = rc.GetNumRow( nPage );

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
					if(po.HideZero[0]){
						rc.m_Val	= HideZeroMoney( rc.m_Val );
						rc.m_Val2	= HideZeroMoney( rc.m_Val2 );
					}
				}

// No.200903 del -->
				//// 総額
				//m_PrtDataBox[index + Field142Val].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );

				//// 総額のうち代表者及びその家族分
				//m_PrtDataBox[index + Field142Val2].Data[nCurRow] =
				//	ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D1 );
// No.200903 del <--
// No.200903 add -->
				// 総額
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[index + Field142Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 13 ), BIT_D0 );
				}
				else	{
					m_PrtDataBox[index + Field142Val].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val, &rc, MakeFormatOfNum( 12 ), BIT_D0 );
				}

				// 総額のうち代表者及びその家族分
				if(bG_Kanso == TRUE)	{
					m_PrtDataBox[index + Field142Val2].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 13 ), BIT_D1 );
				}
				else	{
					m_PrtDataBox[index + Field142Val2].Data[nCurRow] =
						ShowFieldOfVal( rc.m_Val2, &rc, MakeFormatOfNum( 12 ), BIT_D1 );
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

		return m_nRowMax;
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

CprtUc142Jinkenhi::CprtUc142Jinkenhi(CDatabase* pdb)
{
	m_pDB			= pdb;
	m_nFormSeq		= ID_FORMNO_142;
	m_nRowMax	= 0;
	m_nColumn		= Field142Max;
}

CprtUc142Jinkenhi::~CprtUc142Jinkenhi(void)
{
}

CprtJobFunction* CprtUc142Jinkenhi::virGetPrintObject(void)
{
	return (CprtJobFunction*)(new CprtUc142Jinkenhi(m_pDB));
}