#include "StdAfx.h"
#include "H27SnFhyo50Data.h"

static SH_TAGNAME	tagData50_27[] = {
	{_T("AHG00390"), 1 },
	{_T("AHG00400"), 0 },
	{_T("AHG00410"), 3 },
	//{_T("AHG00420"), 999 },

	{_T("AHG01700"), 1 },
	{_T("AHG01710"), 0 },
	{_T("AHG01720"), 3 },
	{_T("AHG01730"), 999 },

	{_T("AHG01380"), 1 },
	{_T("AHG01390"), 0 },
	{_T("AHG01400"), 3 },
	{_T("AHG01410"), 999 },

	{_T("AHG01430"), 1 },
	{_T("AHG01440"), 0 },
	{_T("AHG01450"), 3 },
	{_T("AHG01460"), 999 },

	{_T("AHG01480"), 1 },
	{_T("AHG01490"), 0 },
	{_T("AHG01500"), 3 },
	{_T("AHG01510"), 999 },

	{_T("AHG01530"), 1 },
	{_T("AHG01540"), 0 },
	{_T("AHG01550"), 3 },
	{_T("AHG01560"), 999 },

	{_T("AHG01580"), 1 },
	{_T("AHG01590"), 0 },
	{_T("AHG01600"), 3 },
	{_T("AHG01610"), 999 },

	{_T("") }
};

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH27SnFhyo50Data::CH27SnFhyo50Data( BOOL isTransitionalMeasure )
	: CH26SnFhyo50Data(isTransitionalMeasure)
	, m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset( Sn_5FURI6A, '\0', sizeof(Sn_5FURI6A) );
	memset( Sn_5FURI6B, '\0', sizeof(Sn_5FURI6B) );
	memset( Sn_5FURI6C, '\0', sizeof(Sn_5FURI6C) );

	memset( Sn_5FUZK6A, '\0', sizeof(Sn_5FUZK6A) );
	memset( Sn_5FUZK6B, '\0', sizeof(Sn_5FUZK6B) );
	memset( Sn_5FUZK6C, '\0', sizeof(Sn_5FUZK6C) );

	Sn_5FUZK6sw = 0;
	Sn_5FTKsw = 0;

	memset( Sn_5FUZT6, '\0', sizeof(Sn_5FUZT6) );

	memset( Sn_5FSZT22T, '\0', sizeof(Sn_5FSZT22T) );
	memset( Sn_5FSZT22A, '\0', sizeof(Sn_5FSZT22A) );
	memset( Sn_5FSZT22B, '\0', sizeof(Sn_5FSZT22B) );
	memset( Sn_5FSZT22C, '\0', sizeof(Sn_5FSZT22C) );
	Sn_5FSZT22sw = 0;

	memset( Sn_5FSZT23T, '\0', sizeof(Sn_5FSZT23T) );
	memset( Sn_5FSZT23A, '\0', sizeof(Sn_5FSZT23A) );
	memset( Sn_5FSZT23B, '\0', sizeof(Sn_5FSZT23B) );
	memset( Sn_5FSZT23C, '\0', sizeof(Sn_5FSZT23C) );
	Sn_5FSZT23sw = 0;

	memset( Sn_5FSZT24T, '\0', sizeof(Sn_5FSZT24T) );
	memset( Sn_5FSZT24A, '\0', sizeof(Sn_5FSZT24A) );
	memset( Sn_5FSZT24B, '\0', sizeof(Sn_5FSZT24B) );
	memset( Sn_5FSZT24C, '\0', sizeof(Sn_5FSZT24C) );
	Sn_5FSZT24sw = 0;

	memset( Sn_5FSZT25T, '\0', sizeof(Sn_5FSZT25T) );
	memset( Sn_5FSZT25A, '\0', sizeof(Sn_5FSZT25A) );
	memset( Sn_5FSZT25B, '\0', sizeof(Sn_5FSZT25B) );
	memset( Sn_5FSZT25C, '\0', sizeof(Sn_5FSZT25C) );
	Sn_5FSZT25sw = 0;

	memset( Sn_5FSZT26T, '\0', sizeof(Sn_5FSZT26T) );
	memset( Sn_5FSZT26A, '\0', sizeof(Sn_5FSZT26A) );
	memset( Sn_5FSZT26B, '\0', sizeof(Sn_5FSZT26B) );
	memset( Sn_5FSZT26C, '\0', sizeof(Sn_5FSZT26C) );
	Sn_5FSZT26sw = 0;

	memset( Sn_5FSZT27T, '\0', sizeof(Sn_5FSZT27T) );
	memset( Sn_5FSZT27A, '\0', sizeof(Sn_5FSZT27A) );
	memset( Sn_5FSZT27B, '\0', sizeof(Sn_5FSZT27B) );
	memset( Sn_5FSZT27C, '\0', sizeof(Sn_5FSZT27C) );
	Sn_5FSZT27sw = 0;

	memset( Sn_5FSZT28T, '\0', sizeof(Sn_5FSZT28T) );
	memset( Sn_5FSZT28A, '\0', sizeof(Sn_5FSZT28A) );
	memset( Sn_5FSZT28B, '\0', sizeof(Sn_5FSZT28B) );
	memset( Sn_5FSZT28C, '\0', sizeof(Sn_5FSZT28C) );
	Sn_5FSZT28sw = 0;

	memset( Sn_5FSZT29T, '\0', sizeof(Sn_5FSZT29T) );
	memset( Sn_5FSZT29A, '\0', sizeof(Sn_5FSZT29A) );
	memset( Sn_5FSZT29B, '\0', sizeof(Sn_5FSZT29B) );
	memset( Sn_5FSZT29C, '\0', sizeof(Sn_5FSZT29C) );
	Sn_5FSZT29sw = 0;

	memset( Sn_5FSZT30T, '\0', sizeof(Sn_5FSZT30T) );
	memset( Sn_5FSZT30A, '\0', sizeof(Sn_5FSZT30A) );
	memset( Sn_5FSZT30B, '\0', sizeof(Sn_5FSZT30B) );
	memset( Sn_5FSZT30C, '\0', sizeof(Sn_5FSZT30C) );
	Sn_5FSZT30sw = 0;

	memset( Sn_5FSZT31T, '\0', sizeof(Sn_5FSZT31T) );
	memset( Sn_5FSZT31A, '\0', sizeof(Sn_5FSZT31A) );
	memset( Sn_5FSZT31B, '\0', sizeof(Sn_5FSZT31B) );
	memset( Sn_5FSZT31C, '\0', sizeof(Sn_5FSZT31C) );
	Sn_5FSZT31sw = 0;

	memset( Sn_5FSZT32T, '\0', sizeof(Sn_5FSZT32T) );
	memset( Sn_5FSZT32A, '\0', sizeof(Sn_5FSZT32A) );
	memset( Sn_5FSZT32B, '\0', sizeof(Sn_5FSZT32B) );
	memset( Sn_5FSZT32C, '\0', sizeof(Sn_5FSZT32C) );
	Sn_5FSZT32sw = 0;

	memset( Sn_5FSZT33T, '\0', sizeof(Sn_5FSZT33T) );
	memset( Sn_5FSZT33A, '\0', sizeof(Sn_5FSZT33A) );
	memset( Sn_5FSZT33B, '\0', sizeof(Sn_5FSZT33B) );
	memset( Sn_5FSZT33C, '\0', sizeof(Sn_5FSZT33C) );
	Sn_5FSZT33sw = 0;

	memset( Sn_5FSZT34T, '\0', sizeof(Sn_5FSZT34T) );
	memset( Sn_5FSZT34A, '\0', sizeof(Sn_5FSZT34A) );
	memset( Sn_5FSZT34B, '\0', sizeof(Sn_5FSZT34B) );
	memset( Sn_5FSZT34C, '\0', sizeof(Sn_5FSZT34C) );
	Sn_5FSZT34sw = 0;

	memset( Sn_5FSZT35T, '\0', sizeof(Sn_5FSZT35T) );
	memset( Sn_5FSZT35A, '\0', sizeof(Sn_5FSZT35A) );
	memset( Sn_5FSZT35B, '\0', sizeof(Sn_5FSZT35B) );
	memset( Sn_5FSZT35C, '\0', sizeof(Sn_5FSZT35C) );
	Sn_5FSZT35sw = 0;

	memset( Sn_5FSZT36T, '\0', sizeof(Sn_5FSZT36T) );
	memset( Sn_5FSZT36A, '\0', sizeof(Sn_5FSZT36A) );
	memset( Sn_5FSZT36B, '\0', sizeof(Sn_5FSZT36B) );
	memset( Sn_5FSZT36C, '\0', sizeof(Sn_5FSZT36C) );
	Sn_5FSZT36sw = 0;

	Sn_5FUP6 = 0;

	m_Arith.l_defn( 0x16 );

	memset( rsv3, '\0', sizeof(rsv3) );
	memset( rsv4, '\0', sizeof(rsv4) );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH27SnFhyo50Data::~CH27SnFhyo50Data()
{
	m_DBSyzAccess.CloseShLstVal();
}

//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対応の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了	
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH27SnFhyo50Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH26SnFhyo50Data::GetData(pDBNpSub, sn_seq) ){
		return -1;
	}

	// データのコピー
	// Sn_5FSZT22A <- Sn_5FSZT20A
	memmove( Sn_5FSZT22A, CH26SnFhyo50Data::Sn_5FSZT20A, sizeof(Sn_5FSZT22A) );
	memmove( Sn_5FSZT22B, CH26SnFhyo50Data::Sn_5FSZT20B, sizeof(Sn_5FSZT22B) );
	memmove( Sn_5FSZT22C, CH26SnFhyo50Data::Sn_5FSZT20C, sizeof(Sn_5FSZT22C) );
	memmove( Sn_5FSZT22T, CH26SnFhyo50Data::Sn_5FSZT20T, sizeof(Sn_5FSZT22T) );
	Sn_5FSZT22sw = CH26SnFhyo50Data::Sn_5FSZT20sw;

	// Sn_5FSZT23A <- Sn_5FSZT21A
	memmove( Sn_5FSZT23A, CH26SnFhyo50Data::Sn_5FSZT21A, sizeof(Sn_5FSZT23A) );
	memmove( Sn_5FSZT23B, CH26SnFhyo50Data::Sn_5FSZT21B, sizeof(Sn_5FSZT23B) );
	memmove( Sn_5FSZT23C, CH26SnFhyo50Data::Sn_5FSZT21C, sizeof(Sn_5FSZT23C) );
	memmove( Sn_5FSZT23T, CH26SnFhyo50Data::Sn_5FSZT21T, sizeof(Sn_5FSZT23T) );
	Sn_5FSZT23sw = CH26SnFhyo50Data::Sn_5FSZT21sw;

	// Sn_5FSZT24A <- Sn_5FSZT22A
	memmove( Sn_5FSZT24A, CH26SnFhyo50Data::Sn_5FSZT22A, sizeof(Sn_5FSZT24A) );
	memmove( Sn_5FSZT24B, CH26SnFhyo50Data::Sn_5FSZT22B, sizeof(Sn_5FSZT24B) );
	memmove( Sn_5FSZT24C, CH26SnFhyo50Data::Sn_5FSZT22C, sizeof(Sn_5FSZT24C) );
	memmove( Sn_5FSZT24T, CH26SnFhyo50Data::Sn_5FSZT22T, sizeof(Sn_5FSZT24T) );
	Sn_5FSZT24sw = CH26SnFhyo50Data::Sn_5FSZT22sw;

	// Sn_5FSZT25A <- Sn_5FSZT23A
	memmove( Sn_5FSZT25A, CH26SnFhyo50Data::Sn_5FSZT23A, sizeof(Sn_5FSZT25A) );
	memmove( Sn_5FSZT25B, CH26SnFhyo50Data::Sn_5FSZT23B, sizeof(Sn_5FSZT25B) );
	memmove( Sn_5FSZT25C, CH26SnFhyo50Data::Sn_5FSZT23C, sizeof(Sn_5FSZT25C) );
	memmove( Sn_5FSZT25T, CH26SnFhyo50Data::Sn_5FSZT23T, sizeof(Sn_5FSZT25T) );
	Sn_5FSZT25sw = CH26SnFhyo50Data::Sn_5FSZT23sw;

	// Sn_5FSZT27A <- Sn_5FSZT24A
	memmove( Sn_5FSZT27A, CH26SnFhyo50Data::Sn_5FSZT24A, sizeof(Sn_5FSZT27A) );
	memmove( Sn_5FSZT27B, CH26SnFhyo50Data::Sn_5FSZT24B, sizeof(Sn_5FSZT27B) );
	memmove( Sn_5FSZT27C, CH26SnFhyo50Data::Sn_5FSZT24C, sizeof(Sn_5FSZT27C) );
	memmove( Sn_5FSZT27T, CH26SnFhyo50Data::Sn_5FSZT24T, sizeof(Sn_5FSZT27T) );
	Sn_5FSZT27sw = CH26SnFhyo50Data::Sn_5FSZT24sw;

	// Sn_5FSZT28A <- Sn_5FSZT25A
	memmove( Sn_5FSZT28A, CH26SnFhyo50Data::Sn_5FSZT25A, sizeof(Sn_5FSZT28A) );
	memmove( Sn_5FSZT28B, CH26SnFhyo50Data::Sn_5FSZT25B, sizeof(Sn_5FSZT28B) );
	memmove( Sn_5FSZT28C, CH26SnFhyo50Data::Sn_5FSZT25C, sizeof(Sn_5FSZT28C) );
	memmove( Sn_5FSZT28T, CH26SnFhyo50Data::Sn_5FSZT25T, sizeof(Sn_5FSZT28T) );
	Sn_5FSZT28sw = CH26SnFhyo50Data::Sn_5FSZT25sw;

	// Sn_5FSZT29A <- Sn_5FSZT26A
	memmove( Sn_5FSZT29A, CH26SnFhyo50Data::Sn_5FSZT26A, sizeof(Sn_5FSZT29A) );
	memmove( Sn_5FSZT29B, CH26SnFhyo50Data::Sn_5FSZT26B, sizeof(Sn_5FSZT29B) );
	memmove( Sn_5FSZT29C, CH26SnFhyo50Data::Sn_5FSZT26C, sizeof(Sn_5FSZT29C) );
	memmove( Sn_5FSZT29T, CH26SnFhyo50Data::Sn_5FSZT26T, sizeof(Sn_5FSZT29T) );
	Sn_5FSZT29sw = CH26SnFhyo50Data::Sn_5FSZT26sw;

	// Sn_5FSZT31A <- Sn_5FSZT27A
	memmove( Sn_5FSZT31A, CH26SnFhyo50Data::Sn_5FSZT27A, sizeof(Sn_5FSZT31A) );
	memmove( Sn_5FSZT31B, CH26SnFhyo50Data::Sn_5FSZT27B, sizeof(Sn_5FSZT31B) );
	memmove( Sn_5FSZT31C, CH26SnFhyo50Data::Sn_5FSZT27C, sizeof(Sn_5FSZT31C) );
	memmove( Sn_5FSZT31T, CH26SnFhyo50Data::Sn_5FSZT27T, sizeof(Sn_5FSZT31T) );
	Sn_5FSZT31sw = CH26SnFhyo50Data::Sn_5FSZT27sw;

	// Sn_5FSZT32A <- Sn_5FSZT28A
	memmove( Sn_5FSZT32A, CH26SnFhyo50Data::Sn_5FSZT28A, sizeof(Sn_5FSZT32A) );
	memmove( Sn_5FSZT32B, CH26SnFhyo50Data::Sn_5FSZT28B, sizeof(Sn_5FSZT32B) );
	memmove( Sn_5FSZT32C, CH26SnFhyo50Data::Sn_5FSZT28C, sizeof(Sn_5FSZT32C) );
	memmove( Sn_5FSZT32T, CH26SnFhyo50Data::Sn_5FSZT28T, sizeof(Sn_5FSZT32T) );
	Sn_5FSZT32sw = CH26SnFhyo50Data::Sn_5FSZT28sw;

	// Sn_5FSZT34A <- Sn_5FSZT29A
	memmove( Sn_5FSZT34A, CH26SnFhyo50Data::Sn_5FSZT29A, sizeof(Sn_5FSZT34A) );
	memmove( Sn_5FSZT34B, CH26SnFhyo50Data::Sn_5FSZT29B, sizeof(Sn_5FSZT34B) );
	memmove( Sn_5FSZT34C, CH26SnFhyo50Data::Sn_5FSZT29C, sizeof(Sn_5FSZT34C) );
	memmove( Sn_5FSZT34T, CH26SnFhyo50Data::Sn_5FSZT29T, sizeof(Sn_5FSZT34T) );
	Sn_5FSZT34sw = CH26SnFhyo50Data::Sn_5FSZT29sw;

	// Sn_5FTKsw <- Sn_5FSZT19sw
	Sn_5FTKsw = CH26SnFhyo50Data::Sn_5FSZT19sw;

	CString	filter;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_5_2_HYOID, nowVersion );

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){
				break;
			}
		}
		else{
			if( (pShLstVal->MoveNext()==-1) || (pShLstVal->st==-1) ){
				break;
			}
		}

		if( pShLstVal->itmId == _T("AHG00420") ){			// 第六種事業
//			m_Util.val_to_bin( Sn_5FURIK, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00390") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI6A, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI6A, '\0', sizeof(Sn_5FURI6A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00400") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI6B, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI6B, '\0', sizeof(Sn_5FURI6B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00410") ){
			m_Util.val_to_bin( Sn_5FURI6C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG01730") ){		// 第六種事業（消費税）
			m_Util.val_to_bin( Sn_5FUZT6, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG01700") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FUZK6A, pShLstVal->val );
			}
			else{
				memset( Sn_5FUZK6A, '\0', sizeof(Sn_5FUZK6A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01710") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FUZK6B, pShLstVal->val );
			}
			else{
				memset( Sn_5FUZK6B, '\0', sizeof(Sn_5FUZK6B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01720") ){
			m_Util.val_to_bin( Sn_5FUZK6C, pShLstVal->val );
			Sn_5FUZK6sw = pShLstVal->sign1;
		}
		// 26
		else if( pShLstVal->itmId == _T("AHG01410") ){
			m_Util.val_to_bin( Sn_5FSZT26T, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG01380") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT26A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT26A, '\0', sizeof(Sn_5FSZT26A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01390") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT26B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT26B, '\0', sizeof(Sn_5FSZT26B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01400") ){
			m_Util.val_to_bin( Sn_5FSZT26C, pShLstVal->val );
			Sn_5FSZT26sw = pShLstVal->sign1;
		}
		// 30
		else if( pShLstVal->itmId == _T("AHG01460") ){
			m_Util.val_to_bin( Sn_5FSZT30T, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG01430") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT30A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT30A, '\0', sizeof(Sn_5FSZT30A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01440") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT30B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT30B, '\0', sizeof(Sn_5FSZT30B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01450") ){
			m_Util.val_to_bin( Sn_5FSZT30C, pShLstVal->val );
			Sn_5FSZT30sw = pShLstVal->sign1;
		}
		// 33
		else if( pShLstVal->itmId == _T("AHG01510") ){
			m_Util.val_to_bin( Sn_5FSZT33T, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG01480") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT33A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT33A, '\0', sizeof(Sn_5FSZT33A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01490") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT33B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT33B, '\0', sizeof(Sn_5FSZT33B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01500") ){
			m_Util.val_to_bin( Sn_5FSZT33C, pShLstVal->val );
			Sn_5FSZT33sw = pShLstVal->sign1;
		}
		// 35
		else if( pShLstVal->itmId == _T("AHG01560") ){
			m_Util.val_to_bin( Sn_5FSZT35T, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG01530") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT35A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT35A, '\0', sizeof(Sn_5FSZT35A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01540") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT35B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT35B, '\0', sizeof(Sn_5FSZT35B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01550") ){
			m_Util.val_to_bin( Sn_5FSZT35C, pShLstVal->val );
			Sn_5FSZT35sw = pShLstVal->sign1;
		}
		// 36
		else if( pShLstVal->itmId == _T("AHG01610") ){
			m_Util.val_to_bin( Sn_5FSZT36T, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG01580") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT36A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT36A, '\0', sizeof(Sn_5FSZT36A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01590") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT36B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT36B, '\0', sizeof(Sn_5FSZT36B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01600") ){
			m_Util.val_to_bin( Sn_5FSZT36C, pShLstVal->val );
			Sn_5FSZT36sw = pShLstVal->sign1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DBへデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対応の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH27SnFhyo50Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// データのコピー
	// Sn_5FSZT22A -> Sn_5FSZT20A
	memmove( CH26SnFhyo50Data::Sn_5FSZT20A, Sn_5FSZT22A, sizeof(Sn_5FSZT22A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT20B, Sn_5FSZT22B, sizeof(Sn_5FSZT22B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT20C, Sn_5FSZT22C, sizeof(Sn_5FSZT22C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT20T, Sn_5FSZT22T, sizeof(Sn_5FSZT22T) );
	CH26SnFhyo50Data::Sn_5FSZT20sw = Sn_5FSZT22sw;

	// Sn_5FSZT23A -> Sn_5FSZT21A
	memmove( CH26SnFhyo50Data::Sn_5FSZT21A, Sn_5FSZT23A, sizeof(Sn_5FSZT23A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT21B, Sn_5FSZT23B, sizeof(Sn_5FSZT23B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT21C, Sn_5FSZT23C, sizeof(Sn_5FSZT23C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT21T, Sn_5FSZT23T, sizeof(Sn_5FSZT23T) );
	CH26SnFhyo50Data::Sn_5FSZT21sw = Sn_5FSZT23sw;

	// Sn_5FSZT24A -> Sn_5FSZT22A
	memmove( CH26SnFhyo50Data::Sn_5FSZT22A, Sn_5FSZT24A, sizeof(Sn_5FSZT24A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT22B, Sn_5FSZT24B, sizeof(Sn_5FSZT24B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT22C, Sn_5FSZT24C, sizeof(Sn_5FSZT24C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT22T, Sn_5FSZT24T, sizeof(Sn_5FSZT24T) );
	CH26SnFhyo50Data::Sn_5FSZT22sw = Sn_5FSZT24sw;

	// Sn_5FSZT25A -> Sn_5FSZT23A
	memmove( CH26SnFhyo50Data::Sn_5FSZT23A, Sn_5FSZT25A, sizeof(Sn_5FSZT25A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT23B, Sn_5FSZT25B, sizeof(Sn_5FSZT25B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT23C, Sn_5FSZT25C, sizeof(Sn_5FSZT25C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT23T, Sn_5FSZT25T, sizeof(Sn_5FSZT25T) );
	CH26SnFhyo50Data::Sn_5FSZT23sw = Sn_5FSZT25sw;

	// Sn_5FSZT27A -> Sn_5FSZT24A
	memmove( CH26SnFhyo50Data::Sn_5FSZT24A, Sn_5FSZT27A, sizeof(Sn_5FSZT27A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT24B, Sn_5FSZT27B, sizeof(Sn_5FSZT27B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT24C, Sn_5FSZT27C, sizeof(Sn_5FSZT27C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT24T, Sn_5FSZT27T, sizeof(Sn_5FSZT27T) );
	CH26SnFhyo50Data::Sn_5FSZT24sw = Sn_5FSZT27sw;

	// Sn_5FSZT28A -> Sn_5FSZT25A
	memmove( CH26SnFhyo50Data::Sn_5FSZT25A, Sn_5FSZT28A, sizeof(Sn_5FSZT28A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT25B, Sn_5FSZT28B, sizeof(Sn_5FSZT28B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT25C, Sn_5FSZT28C, sizeof(Sn_5FSZT28C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT25T, Sn_5FSZT28T, sizeof(Sn_5FSZT28T) );
	CH26SnFhyo50Data::Sn_5FSZT25sw = Sn_5FSZT28sw;

	// Sn_5FSZT29A -> Sn_5FSZT26A
	memmove( CH26SnFhyo50Data::Sn_5FSZT26A, Sn_5FSZT29A, sizeof(Sn_5FSZT29A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT26B, Sn_5FSZT29B, sizeof(Sn_5FSZT29B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT26C, Sn_5FSZT29C, sizeof(Sn_5FSZT29C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT26T, Sn_5FSZT29T, sizeof(Sn_5FSZT29T) );
	CH26SnFhyo50Data::Sn_5FSZT26sw = Sn_5FSZT29sw;

	// Sn_5FSZT31A -> Sn_5FSZT27A
	memmove( CH26SnFhyo50Data::Sn_5FSZT27A, Sn_5FSZT31A, sizeof(Sn_5FSZT31A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT27B, Sn_5FSZT31B, sizeof(Sn_5FSZT31B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT27C, Sn_5FSZT31C, sizeof(Sn_5FSZT31C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT27T, Sn_5FSZT31T, sizeof(Sn_5FSZT31T) );
	CH26SnFhyo50Data::Sn_5FSZT27sw = Sn_5FSZT31sw;

	// Sn_5FSZT32A -> Sn_5FSZT28A
	memmove( CH26SnFhyo50Data::Sn_5FSZT28A, Sn_5FSZT32A, sizeof(Sn_5FSZT32A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT28B, Sn_5FSZT32B, sizeof(Sn_5FSZT32B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT28C, Sn_5FSZT32C, sizeof(Sn_5FSZT32C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT28T, Sn_5FSZT32T, sizeof(Sn_5FSZT32T) );
	CH26SnFhyo50Data::Sn_5FSZT28sw = Sn_5FSZT32sw;

	// Sn_5FSZT34A -> Sn_5FSZT29A
	memmove( CH26SnFhyo50Data::Sn_5FSZT29A, Sn_5FSZT34A, sizeof(Sn_5FSZT34A) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT29B, Sn_5FSZT34B, sizeof(Sn_5FSZT34B) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT29C, Sn_5FSZT34C, sizeof(Sn_5FSZT34C) );
	memmove( CH26SnFhyo50Data::Sn_5FSZT29T, Sn_5FSZT34T, sizeof(Sn_5FSZT34T) );
	CH26SnFhyo50Data::Sn_5FSZT29sw = Sn_5FSZT34sw;

	// Sn_5FTKsw -> Sn_5FSZT19sw
	CH26SnFhyo50Data::Sn_5FSZT19sw = Sn_5FTKsw;

	// 基底クラスでのデータのセット
	if( CH26SnFhyo50Data::SetData(pDBNpSub, sn_seq) ){
		return -1;
	}

	CString	filter;
	int		nowVersion = 1;
	char	*pHyoID = NULL;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_5_2_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_5_2_HYOID;

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// データ無し

		// 第六種事業
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00390");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI6A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00400");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI6B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00410");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI6C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第六種事業（消費税）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01730");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT6 );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01700");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK6A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01710");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK6B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01720");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK6C );
		pShLstVal->sign1 = Sn_5FUZK6sw;
		pShLstVal->Update();

		// 26
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01410");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26T );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01380");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01390");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01400");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26C );
		pShLstVal->sign1 = Sn_5FSZT26sw;
		pShLstVal->Update();

		// 30
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01460");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30T );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01430");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01440");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01450");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30C );
		pShLstVal->sign1 = Sn_5FSZT30sw;
		pShLstVal->Update();

		// 33
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01510");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33T );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01480");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01490");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01500");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33C );
		pShLstVal->sign1 = Sn_5FSZT33sw;
		pShLstVal->Update();

		// 35
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01560");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35T );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01530");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01540");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01550");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35C );
		pShLstVal->sign1 = Sn_5FSZT35sw;
		pShLstVal->Update();

		// 36
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01610");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36T );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01580");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01590");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01600");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36C );
		pShLstVal->sign1 = Sn_5FSZT36sw;
		pShLstVal->Update();
	}
	else{														// データ有り
		CStringArray	updTagAry;
		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){
					break;
				}
			}
			else{
				if( (pShLstVal->MoveNext()==-1) || (pShLstVal->st==-1) ){
					break;
				}
			}

			// 第六種事業
			if( pShLstVal->itmId == _T("AHG00390") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI6A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00400") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI6B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00410") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI6C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 第六種事業（消費税）
			else if( pShLstVal->itmId == _T("AHG01730") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT6 );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01700") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK6A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01710") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK6B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01720") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK6C );
				pShLstVal->sign1 = Sn_5FUZK6sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 26
			else if( pShLstVal->itmId == _T("AHG01410") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01380") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01390") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01400") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26C );
				pShLstVal->sign1 = Sn_5FSZT26sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 30
			else if( pShLstVal->itmId == _T("AHG01460") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01430") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01440") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01450") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT30C );
				pShLstVal->sign1 = Sn_5FSZT30sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 33
			else if( pShLstVal->itmId == _T("AHG01510") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01480") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01490") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01500") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT33C );
				pShLstVal->sign1 = Sn_5FSZT33sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 35
			else if( pShLstVal->itmId == _T("AHG01560") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01530") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01540") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01550") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT35C );
				pShLstVal->sign1 = Sn_5FSZT35sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 36
			else if( pShLstVal->itmId == _T("AHG01610") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01580") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01590") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG01600") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT36C );
				pShLstVal->sign1 = Sn_5FSZT36sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
		}

		// 更新データ確認
		int	adCnt = 0;
		for( int i=0;; i++ ){
			if( strlen(tagData50_27[i].tagName) == 0 ){
				break;
			}
			if( m_isTransitionalMeasure== FALSE ){
				if( (tagData50_27[i].sign1!=0) && (tagData50_27[i].sign1!=1) ){
					adCnt++;
				}
			}
			else{
				adCnt++;
			}
		}

		int updCnt = (int)updTagAry.GetCount();
		if( adCnt != updCnt ){
			// 無いものを追加
			CArray<unsigned char*>	MonyPtrAry;
			MonyPtrAry.Add( Sn_5FURI6A );
			MonyPtrAry.Add( Sn_5FURI6B );
			MonyPtrAry.Add( Sn_5FURI6C );

			MonyPtrAry.Add( Sn_5FUZK6A );
			MonyPtrAry.Add( Sn_5FUZK6B );
			MonyPtrAry.Add( Sn_5FUZK6C );
			MonyPtrAry.Add( Sn_5FUZT6 );

			MonyPtrAry.Add( Sn_5FSZT26A );
			MonyPtrAry.Add( Sn_5FSZT26B );
			MonyPtrAry.Add( Sn_5FSZT26C );
			MonyPtrAry.Add( Sn_5FSZT26T );

			MonyPtrAry.Add( Sn_5FSZT30A );
			MonyPtrAry.Add( Sn_5FSZT30B );
			MonyPtrAry.Add( Sn_5FSZT30C );
			MonyPtrAry.Add( Sn_5FSZT30T );

			MonyPtrAry.Add( Sn_5FSZT33A );
			MonyPtrAry.Add( Sn_5FSZT33B );
			MonyPtrAry.Add( Sn_5FSZT33C );
			MonyPtrAry.Add( Sn_5FSZT33T );

			MonyPtrAry.Add( Sn_5FSZT35A );
			MonyPtrAry.Add( Sn_5FSZT35B );
			MonyPtrAry.Add( Sn_5FSZT35C );
			MonyPtrAry.Add( Sn_5FSZT35T );

			MonyPtrAry.Add( Sn_5FSZT36A );
			MonyPtrAry.Add( Sn_5FSZT36B );
			MonyPtrAry.Add( Sn_5FSZT36C );
			MonyPtrAry.Add( Sn_5FSZT36T );

			BOOL findFlg = FALSE;
			for( int i=0;; i++ ){
				if( strlen(tagData50_27[i].tagName) == 0 ){
					break;
				}
				if( m_isTransitionalMeasure == FALSE ){
					if( (tagData50_27[i].sign1==0) || (tagData50_27[i].sign1==1) ){
						continue;
					}
				}

				findFlg = FALSE;
				for( int j=0; j<updCnt; j++ ){
					if( updTagAry[j].CompareNoCase(tagData50_27[i].tagName) == 0 ){
						findFlg = TRUE;
						break;
					}
				}
				if( findFlg == FALSE ){
					// 該当タグの追加
					pShLstVal->AddNew();
					pShLstVal->sn_seq = sn_seq;
					pShLstVal->version = nowVersion;
					pShLstVal->hyoId = pHyoID;
					pShLstVal->itmId = tagData50_27[i].tagName;
					m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
					if( pShLstVal->itmId == _T("AHG01720") ){
						pShLstVal->sign1 = Sn_5FUZK6sw;
					}
					else if( pShLstVal->itmId == _T("AHG01400") ){
						pShLstVal->sign1 = Sn_5FSZT26sw;
					}
					else if( pShLstVal->itmId == _T("AHG01450") ){
						pShLstVal->sign1 = Sn_5FSZT30sw;
					}
					else if( pShLstVal->itmId == _T("AHG01500") ){
						pShLstVal->sign1 = Sn_5FSZT33sw;
					}
					else if( pShLstVal->itmId == _T("AHG01550") ){
						pShLstVal->sign1 = Sn_5FSZT35sw;
					}
					else if( pShLstVal->itmId == _T("AHG01600") ){
						pShLstVal->sign1 = Sn_5FSZT36sw;
					}
					else{
						pShLstVal->sign1 = 0;
					}
					pShLstVal->Update();
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 集計済みデータのセット
//-----------------------------------------------------------------------------
// 引数	pSyzSyukei	：	消費税集計クラス
//		isPluralZei	：	複数税率
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH27SnFhyo50Data::SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	ASSERT( pSyzSyukei );
	if( pSyzSyukei == NULL ){
		return -1;
	}

	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("25") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	// □□□□ □■■■
											//  D0：3%
											//  D1：5%
											//  D2：8%

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code.Left(4) == _T("2506") ){
				if( money[i][j].code == _T("250601") ){
					memmove( Sn_5FURITA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250602") ){
					memmove( Sn_5FURITB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250603") ){
					memmove( Sn_5FURITC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250600") ){
					memmove( Sn_5FURIK, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2507") ){
				if( money[i][j].code == _T("250701") ){
					memmove( Sn_5FURI1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250702") ){
					memmove( Sn_5FURI1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250703") ){
					memmove( Sn_5FURI1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2508") ){
				if( money[i][j].code == _T("250801") ){
					memmove( Sn_5FURI2A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250802") ){
					memmove( Sn_5FURI2B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250803") ){
					memmove( Sn_5FURI2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2509") ){
				if( money[i][j].code == _T("250901") ){
					memmove( Sn_5FURI3A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250902") ){
					memmove( Sn_5FURI3B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("250903") ){
					memmove( Sn_5FURI3C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2510") ){
				if( money[i][j].code == _T("251001") ){
					memmove( Sn_5FURI4A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251002") ){
					memmove( Sn_5FURI4B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251003") ){
					memmove( Sn_5FURI4C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2511") ){
				if( money[i][j].code == _T("251101") ){
					memmove( Sn_5FURI5A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251102") ){
					memmove( Sn_5FURI5B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251103") ){
					memmove( Sn_5FURI5C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2512") ){
				if( money[i][j].code == _T("251201") ){
					memmove( Sn_5FURI6A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251202") ){
					memmove( Sn_5FURI6B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251203") ){
					memmove( Sn_5FURI6C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2513") ){
				if( money[i][j].code == _T("251301") ){
					memmove( Sn_5FUZKTA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251302") ){
					memmove( Sn_5FUZKTB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251303") ){
					memmove( Sn_5FUZKTC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251300") ){
					memmove( Sn_5FUZTT, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2514") ){
				if( money[i][j].code == _T("251401") ){
					memmove( Sn_5FUZK1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251402") ){
					memmove( Sn_5FUZK1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251403") ){
					memmove( Sn_5FUZK1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251400") ){
					memmove( Sn_5FUZT1, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2515") ){
				if( money[i][j].code == _T("251501") ){
					memmove( Sn_5FUZK2A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251502") ){
					memmove( Sn_5FUZK2B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251503") ){
					memmove( Sn_5FUZK2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251500") ){
					memmove( Sn_5FUZT2, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2516") ){
				if( money[i][j].code == _T("251601") ){
					memmove( Sn_5FUZK3A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251602") ){
					memmove( Sn_5FUZK3B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251603") ){
					memmove( Sn_5FUZK3C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251600") ){
					memmove( Sn_5FUZT3, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2517") ){
				if( money[i][j].code == _T("251701") ){
					memmove( Sn_5FUZK4A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251702") ){
					memmove( Sn_5FUZK4B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251703") ){
					memmove( Sn_5FUZK4C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251700") ){
					memmove( Sn_5FUZT4, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2518") ){
				if( money[i][j].code == _T("251801") ){
					memmove( Sn_5FUZK5A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251802") ){
					memmove( Sn_5FUZK5B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251803") ){
					memmove( Sn_5FUZK5C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251800") ){
					memmove( Sn_5FUZT5, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2519") ){
				if( money[i][j].code == _T("251901") ){
					memmove( Sn_5FUZK6A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251902") ){
					memmove( Sn_5FUZK6B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251903") ){
					memmove( Sn_5FUZK6C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("251900") ){
					memmove( Sn_5FUZT6, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2520") ){
				if( money[i][j].code == _T("252001") ){
					memmove( Sn_5FSZKGA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252002") ){
					memmove( Sn_5FSZKGB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252003") ){
					memmove( Sn_5FSZKGC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252000") ){
					memmove( Sn_5FSZTG, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2521") ){
				if( money[i][j].code == _T("252101") ){
					memmove( Sn_5FSZKTA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252102") ){
					memmove( Sn_5FSZKTB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252103") ){
					memmove( Sn_5FSZKTC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252100") ){
					memmove( Sn_5FSZTT, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2522") ){
				if( money[i][j].code == _T("252201") ){
					memmove( Sn_5FSZT22A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252202") ){
					memmove( Sn_5FSZT22B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252203") ){
					memmove( Sn_5FSZT22C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252200") ){
					memmove( Sn_5FSZT22T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2523") ){
				if( money[i][j].code == _T("252301") ){
					memmove( Sn_5FSZT23A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252302") ){
					memmove( Sn_5FSZT23B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252303") ){
					memmove( Sn_5FSZT23C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252300") ){
					memmove( Sn_5FSZT23T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2524") ){
				if( money[i][j].code == _T("252401") ){
					memmove( Sn_5FSZT24A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252402") ){
					memmove( Sn_5FSZT24B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252403") ){
					memmove( Sn_5FSZT24C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252400") ){
					memmove( Sn_5FSZT24T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2525") ){
				if( money[i][j].code == _T("252501") ){
					memmove( Sn_5FSZT25A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252502") ){
					memmove( Sn_5FSZT25B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252503") ){
					memmove( Sn_5FSZT25C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252500") ){
					memmove( Sn_5FSZT25T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2526") ){
				if( money[i][j].code == _T("252601") ){
					memmove( Sn_5FSZT26A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252602") ){
					memmove( Sn_5FSZT26B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252603") ){
					memmove( Sn_5FSZT26C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252600") ){
					memmove( Sn_5FSZT26T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2527") ){
				if( money[i][j].code == _T("252701") ){
					memmove( Sn_5FSZT27A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252702") ){
					memmove( Sn_5FSZT27B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252703") ){
					memmove( Sn_5FSZT27C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252700") ){
					memmove( Sn_5FSZT27T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2528") ){
				if( money[i][j].code == _T("252801") ){
					memmove( Sn_5FSZT28A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252802") ){
					memmove( Sn_5FSZT28B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252803") ){
					memmove( Sn_5FSZT28C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252800") ){
					memmove( Sn_5FSZT28T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2529") ){
				if( money[i][j].code == _T("252901") ){
					memmove( Sn_5FSZT29A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252902") ){
					memmove( Sn_5FSZT29B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252903") ){
					memmove( Sn_5FSZT29C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("252900") ){
					memmove( Sn_5FSZT29T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2530") ){
				if( money[i][j].code == _T("253001") ){
					memmove( Sn_5FSZT30A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253002") ){
					memmove( Sn_5FSZT30B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253003") ){
					memmove( Sn_5FSZT30C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253000") ){
					memmove( Sn_5FSZT30T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2531") ){
				if( money[i][j].code == _T("253101") ){
					memmove( Sn_5FSZT31A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253102") ){
					memmove( Sn_5FSZT31B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253103") ){
					memmove( Sn_5FSZT31C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253100") ){
					memmove( Sn_5FSZT31T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2532") ){
				if( money[i][j].code == _T("253201") ){
					memmove( Sn_5FSZT32A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253202") ){
					memmove( Sn_5FSZT32B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253203") ){
					memmove( Sn_5FSZT32C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253200") ){
					memmove( Sn_5FSZT32T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2533") ){
				if( money[i][j].code == _T("253301") ){
					memmove( Sn_5FSZT33A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253302") ){
					memmove( Sn_5FSZT33B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253303") ){
					memmove( Sn_5FSZT33C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253300") ){
					memmove( Sn_5FSZT33T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2534") ){
				if( money[i][j].code == _T("253401") ){
					memmove( Sn_5FSZT34A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253402") ){
					memmove( Sn_5FSZT34B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253403") ){
					memmove( Sn_5FSZT34C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253400") ){
					memmove( Sn_5FSZT34T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2535") ){
				if( money[i][j].code == _T("253501") ){
					memmove( Sn_5FSZT35A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253502") ){
					memmove( Sn_5FSZT35B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253503") ){
					memmove( Sn_5FSZT35C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253500") ){
					memmove( Sn_5FSZT35T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("2536") ){
				if( money[i][j].code == _T("253601") ){
					memmove( Sn_5FSZT36A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253602") ){
					memmove( Sn_5FSZT36B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253603") ){
					memmove( Sn_5FSZT36C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("253600") ){
					memmove( Sn_5FSZT36T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else{
				break;
			}
		}
	}

	if( ((exZeiflg&0x07)==0x01) || ((exZeiflg&0x07)==0x02) || ((exZeiflg&0x07)==0x04) || (exZeiflg==0x00) ){
		isPluralZei = FALSE;
	}
	else{
		isPluralZei = TRUE;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 現行データ構造からのデータの引継ぎ
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		sn_seq		：	履歴シーケンス
//-----------------------------------------------------------------------------
int CH27SnFhyo50Data::GetPastData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH26SnFhyo50Data::GetPastData(pDBNpSub, sn_seq) ){
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 更新時の金額クリア
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		type		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH27SnFhyo50Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH26SnFhyo50Data::ClearDataByRenew(pDBNpSub, type) ){
		return -1;
	}

	// 第六種
	m_Arith.l_clear( Sn_5FURI6A );
	m_Arith.l_clear( Sn_5FURI6B );
	m_Arith.l_clear( Sn_5FURI6C );

	// 第六種（消費税）
	m_Arith.l_clear( Sn_5FUZK6A );
	m_Arith.l_clear( Sn_5FUZK6B );
	m_Arith.l_clear( Sn_5FUZK6C );
	m_Arith.l_clear( Sn_5FUZT6 );
	Sn_5FUZK6sw = 0;

	// 22
	m_Arith.l_clear( Sn_5FSZT22T );
	m_Arith.l_clear( Sn_5FSZT22A );
	m_Arith.l_clear( Sn_5FSZT22B );
	m_Arith.l_clear( Sn_5FSZT22C );
	Sn_5FSZT22sw = 0;

	// 23
	m_Arith.l_clear( Sn_5FSZT23T );
	m_Arith.l_clear( Sn_5FSZT23A );
	m_Arith.l_clear( Sn_5FSZT23B );
	m_Arith.l_clear( Sn_5FSZT23C );
	Sn_5FSZT23sw = 0;

	// 24
	m_Arith.l_clear( Sn_5FSZT24T );
	m_Arith.l_clear( Sn_5FSZT24A );
	m_Arith.l_clear( Sn_5FSZT24B );
	m_Arith.l_clear( Sn_5FSZT24C );
	Sn_5FSZT24sw = 0;

	// 25
	m_Arith.l_clear( Sn_5FSZT25T );
	m_Arith.l_clear( Sn_5FSZT25A );
	m_Arith.l_clear( Sn_5FSZT25B );
	m_Arith.l_clear( Sn_5FSZT25C );
	Sn_5FSZT25sw = 0;

	// 26
	m_Arith.l_clear( Sn_5FSZT26T );
	m_Arith.l_clear( Sn_5FSZT26A );
	m_Arith.l_clear( Sn_5FSZT26B );
	m_Arith.l_clear( Sn_5FSZT26C );
	Sn_5FSZT26sw = 0;

	// 27
	m_Arith.l_clear( Sn_5FSZT27T );
	m_Arith.l_clear( Sn_5FSZT27A );
	m_Arith.l_clear( Sn_5FSZT27B );
	m_Arith.l_clear( Sn_5FSZT27C );
	Sn_5FSZT27sw = 0;

	// 28
	m_Arith.l_clear( Sn_5FSZT28T );
	m_Arith.l_clear( Sn_5FSZT28A );
	m_Arith.l_clear( Sn_5FSZT28B );
	m_Arith.l_clear( Sn_5FSZT28C );
	Sn_5FSZT28sw = 0;

	// 29
	m_Arith.l_clear( Sn_5FSZT29T );
	m_Arith.l_clear( Sn_5FSZT29A );
	m_Arith.l_clear( Sn_5FSZT29B );
	m_Arith.l_clear( Sn_5FSZT29C );
	Sn_5FSZT29sw = 0;

	// 30
	m_Arith.l_clear( Sn_5FSZT30T );
	m_Arith.l_clear( Sn_5FSZT30A );
	m_Arith.l_clear( Sn_5FSZT30B );
	m_Arith.l_clear( Sn_5FSZT30C );
	Sn_5FSZT30sw = 0;

	// 31
	m_Arith.l_clear( Sn_5FSZT31T );
	m_Arith.l_clear( Sn_5FSZT31A );
	m_Arith.l_clear( Sn_5FSZT31B );
	m_Arith.l_clear( Sn_5FSZT31C );
	Sn_5FSZT31sw = 0;

	// 32
	m_Arith.l_clear( Sn_5FSZT32T );
	m_Arith.l_clear( Sn_5FSZT32A );
	m_Arith.l_clear( Sn_5FSZT32B );
	m_Arith.l_clear( Sn_5FSZT32C );
	Sn_5FSZT32sw = 0;

	// 33
	m_Arith.l_clear( Sn_5FSZT33T );
	m_Arith.l_clear( Sn_5FSZT33A );
	m_Arith.l_clear( Sn_5FSZT33B );
	m_Arith.l_clear( Sn_5FSZT33C );
	Sn_5FSZT33sw = 0;

	// 34
	m_Arith.l_clear( Sn_5FSZT34T );
	m_Arith.l_clear( Sn_5FSZT34A );
	m_Arith.l_clear( Sn_5FSZT34B );
	m_Arith.l_clear( Sn_5FSZT34C );
	Sn_5FSZT34sw = 0;

	// 35
	m_Arith.l_clear( Sn_5FSZT35T );
	m_Arith.l_clear( Sn_5FSZT35A );
	m_Arith.l_clear( Sn_5FSZT35B );
	m_Arith.l_clear( Sn_5FSZT35C );
	Sn_5FSZT35sw = 0;

	// 36
	m_Arith.l_clear( Sn_5FSZT36T );
	m_Arith.l_clear( Sn_5FSZT36A );
	m_Arith.l_clear( Sn_5FSZT36B );
	m_Arith.l_clear( Sn_5FSZT36C );
	Sn_5FSZT36sw = 0;

	return 0;
}

//----------------------------------------------------------------------------
// 手入力サインのクリア
//----------------------------------------------------------------------------
// 引数	isChgKzritu	：	経過税率の有無に違いがあるかどうか
//----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//----------------------------------------------------------------------------
int CH27SnFhyo50Data::ClearManualInputSign( BOOL isChgKzritu )
{
	// 基底クラスでのデータの取得
	if( CH26SnFhyo50Data::ClearManualInputSign(isChgKzritu) ){
		return -1;
	}

	Sn_5FUZK6sw = 0;
	if( isChgKzritu ){
		Sn_5FUZK6sw |= 0x111;
	}
	Sn_5FSZT22sw = 0;
	Sn_5FSZT23sw = 0;
	Sn_5FSZT24sw = 0;
	Sn_5FSZT25sw = 0;
	Sn_5FSZT26sw = 0;
	Sn_5FSZT27sw = 0;
	Sn_5FSZT28sw = 0;
	Sn_5FSZT29sw = 0;
	Sn_5FSZT30sw = 0;
	Sn_5FSZT31sw = 0;
	Sn_5FSZT32sw = 0;
	Sn_5FSZT33sw = 0;
	Sn_5FSZT34sw = 0;
	Sn_5FSZT35sw = 0;
	Sn_5FSZT36sw = 0;
	Sn_5FTKsw = 0;

	return 0;
}

//----------------------------------------------------------------------------
// 横計ではない例外計算が必要？ ('15.05.07)
//----------------------------------------------------------------------------
// 返送値	TRUE	：	必要
//			FALSE	：	不要
//----------------------------------------------------------------------------
BOOL CH27SnFhyo50Data::IsNeedExpCalq()
{
	BOOL	bRt = FALSE;

	// マイナス消費税額
	if( m_Arith.l_test(Sn_5FUZK1A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK2A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK3A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK4A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK5A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK6A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK1B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK2B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK3B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK4B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK5B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK6B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK1C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK2C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK3C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK4C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK5C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK6C) < 0 ){
		bRt = TRUE;
	}

	return bRt;
}
