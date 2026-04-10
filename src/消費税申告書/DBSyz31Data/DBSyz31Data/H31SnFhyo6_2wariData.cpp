#include "StdAfx.h"
#include "H31SnFhyo6_2wariData.h"

static SH_TAGNAME tagData601[] = {	//(メモ)ID番号は未発表の為、一時的に設定
	// 課税資産の譲渡等の対価の額
	{ _T("AYB00020"), 1 },
	{ _T("AYB00030"), 3 },
	{ _T("AYB00040"), 999 },
	// 課税標準額
	{ _T("AYB00060"), 1 },
	{ _T("AYB00070"), 3 },
	{ _T("AYB00080"), 999 },
	// 課税標準額に対する消費税額
	{ _T("AYB00100"), 1 },
	{ _T("AYB00110"), 3 },
	{ _T("AYB00120"), 999 },
	// 貸倒回収に係る消費税額
	{ _T("AYB00140"), 1 },
	{ _T("AYB00150"), 3 },
	{ _T("AYB00160"), 999 },
	// 売上対価の返還等に係る消費税額
	{ _T("AYB00180"), 1 },
	{ _T("AYB00190"), 3 },
	{ _T("AYB00200"), 999 },
	// 控除対象仕入税額の計算の基礎となる消費税額
	{ _T("AYB00220"), 1 },
	{ _T("AYB00230"), 3 },
	{ _T("AYB00240"), 999 },
	// 特別控除税額
	{ _T("AYC00020"), 1 },
	{ _T("AYC00030"), 3 },
	{ _T("AYC00040"), 999 },
	// 貸倒れに係る税額
	{ _T("AYD00020"), 1 },
	{ _T("AYD00030"), 3 },
	{ _T("AYD00040"), 999 },

	{ _T("") }

};

CH31SnFhyo6_2wariData::CH31SnFhyo6_2wariData( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure ( isTransitionalMeasure )
{
	memset ( Sn_F601_1A, '\0', sizeof ( Sn_F601_1A ) );
	memset ( Sn_F601_1B, '\0', sizeof ( Sn_F601_1B ) );
	memset ( Sn_F601_1C, '\0', sizeof ( Sn_F601_1C ) );

	memset ( Sn_F601_2A, '\0', sizeof ( Sn_F601_2A ) );
	memset ( Sn_F601_2B, '\0', sizeof ( Sn_F601_2B ) );
	memset ( Sn_F601_2C, '\0', sizeof ( Sn_F601_2C ) );

	memset ( Sn_F601_3A, '\0', sizeof ( Sn_F601_3A ) );
	memset ( Sn_F601_3B, '\0', sizeof ( Sn_F601_3B ) );
	memset ( Sn_F601_3C, '\0', sizeof ( Sn_F601_3C ) );

	memset ( Sn_F601_4A, '\0', sizeof ( Sn_F601_4A ) );
	memset ( Sn_F601_4B, '\0', sizeof ( Sn_F601_4B ) );
	memset ( Sn_F601_4C, '\0', sizeof ( Sn_F601_4C ) );

	memset ( Sn_F601_5A, '\0', sizeof ( Sn_F601_5A ) );
	memset ( Sn_F601_5B, '\0', sizeof ( Sn_F601_5B ) );
	memset ( Sn_F601_5C, '\0', sizeof ( Sn_F601_5C ) );

	memset ( Sn_F601_6A, '\0', sizeof ( Sn_F601_6A ) );
	memset ( Sn_F601_6B, '\0', sizeof ( Sn_F601_6B ) );
	memset ( Sn_F601_6C, '\0', sizeof ( Sn_F601_6C ) );

	memset ( Sn_F601_7A, '\0', sizeof ( Sn_F601_7A ) );
	memset ( Sn_F601_7B, '\0', sizeof ( Sn_F601_7B ) );
	memset ( Sn_F601_7C, '\0', sizeof ( Sn_F601_7C ) );

	memset ( Sn_F601_8A, '\0', sizeof ( Sn_F601_8A ) );
	memset ( Sn_F601_8B, '\0', sizeof ( Sn_F601_8B ) );
	memset ( Sn_F601_8C, '\0', sizeof ( Sn_F601_8C ) );

	Sn_F601_3sw = 0;

	m_Arith.l_defn ( 0x16 );

	rsv = NULL;
}


CH31SnFhyo6_2wariData::~CH31SnFhyo6_2wariData()
{
	m_DBSyzAccess.CloseShLstVal();
}


//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31SnFhyo6_2wariData::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	ASSERT ( pDBNpSub->zvol );
	if ( pDBNpSub->zvol == NULL ) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( pDBNpSub->zvol );
	if ( KzHoushiki == ID_ICSSH_MENZEI ) {
		return 0;
	}

	CString filter;
	int		nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_6_01_HYOID, nowVersion );

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal ( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	for ( int i = 0; ; i++ ) {
		if ( i == 0 ) {
			if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}
		else {
			if ( ( pShLstVal->MoveNext() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}
		
		// 課税資産の譲渡等の対価の額
		if ( pShLstVal->itmId == _T( "AYB00020" ) ) {
			m_Util.val_to_bin ( Sn_F601_1A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00030" ) ) {
			m_Util.val_to_bin ( Sn_F601_1B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00040" ) ) {
			m_Util.val_to_bin ( Sn_F601_1C, pShLstVal->val );
		}
		// 課税標準額
		else if ( pShLstVal->itmId == _T( "AYB00060" ) ) {
			m_Util.val_to_bin ( Sn_F601_2A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00070" ) ) {
			m_Util.val_to_bin ( Sn_F601_2B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00080" ) ) {
			m_Util.val_to_bin ( Sn_F601_2C, pShLstVal->val );
		}
		// 課税標準額に対する消費税額
		else if ( pShLstVal->itmId == _T( "AYB00100" ) ) {
			m_Util.val_to_bin ( Sn_F601_3A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00110" ) ) {
			m_Util.val_to_bin ( Sn_F601_3B, pShLstVal->val );
			//Sn_F401_2sw = pShLstVal->sign1;
		}
		else if ( pShLstVal->itmId == _T( "AYB00120" ) ) {
			m_Util.val_to_bin ( Sn_F601_3C, pShLstVal->val );
			Sn_F601_3sw = pShLstVal->sign1;
		}
		// 貸倒回収に係る消費税額
		else if ( pShLstVal->itmId == _T( "AYB00140" ) ) {
			m_Util.val_to_bin ( Sn_F601_4A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00150" ) ) {
			m_Util.val_to_bin ( Sn_F601_4B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00160" ) ) {
			m_Util.val_to_bin ( Sn_F601_4C, pShLstVal->val );
		}
		// 売上対価の返還等に係る消費税額
		else if ( pShLstVal->itmId == _T( "AYB00180" ) ) {
			m_Util.val_to_bin ( Sn_F601_5A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00190" ) ) {
			m_Util.val_to_bin ( Sn_F601_5B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00200" ) ) {
			m_Util.val_to_bin ( Sn_F601_5C, pShLstVal->val );
		}
		// 控除対象仕入税額の計算の基礎となる消費税額
		else if ( pShLstVal->itmId == _T( "AYB00220" ) ) {
			m_Util.val_to_bin ( Sn_F601_6A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00230" ) ) {
			m_Util.val_to_bin ( Sn_F601_6B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYB00240" ) ) {
			m_Util.val_to_bin ( Sn_F601_6C, pShLstVal->val );
		}
		// 特別控除税額
		else if ( pShLstVal->itmId == _T( "AYC00020" ) ) {
			m_Util.val_to_bin ( Sn_F601_7A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYC00030" ) ) {
			m_Util.val_to_bin ( Sn_F601_7B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYC00040" ) ) {
			m_Util.val_to_bin ( Sn_F601_7C, pShLstVal->val );
		}
		// 貸倒れに係る税額
		else if ( pShLstVal->itmId == _T( "AYD00020" ) ) {
			m_Util.val_to_bin ( Sn_F601_8A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYD00030" ) ) {
			m_Util.val_to_bin ( Sn_F601_8B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AYD00040" ) ) {
			m_Util.val_to_bin ( Sn_F601_8C, pShLstVal->val );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DB へデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31SnFhyo6_2wariData::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}
	ASSERT ( pDBNpSub->zvol );
	if ( pDBNpSub->zvol == NULL ) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( pDBNpSub->zvol );
	if ( KzHoushiki == ID_ICSSH_MENZEI ) {
		return 0;
	}

	CString filter;
	char	*pHyoID = NULL;
	int		nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_6_01_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_6_01_HYOID;

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal ( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {	// データなし

		// 課税資産の譲渡等の対価の額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_1A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_1B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 課税標準額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00060" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_2A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_2B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_2C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		
		// 課税標準額に対する消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00100" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_3A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00110" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_3B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00120" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_3C );
		pShLstVal->sign1 = Sn_F601_3sw;
		pShLstVal->Update();

		// 貸倒回収に係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00140" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_4A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00150" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_4B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00160" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_4C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 売上対価の返還等に係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00180" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_5A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00190" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_5B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00200" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_5C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 控除対象仕入税額の計算の基礎となる消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00220" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_6A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00230" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_6B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYB00240" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_6C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 特別控除税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYC00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_7A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYC00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_7B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYC00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_7C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 貸倒れに係る税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYD00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_8A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYD00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_8B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "AYD00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_8C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	}
	else {
		CStringArray updTagAry;

		for ( int i = 0; ; i++ ) {
			if ( i == 0 ) {
				if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {
					break;
				}
			}
			else {
				if ( ( pShLstVal->MoveNext() == -1 ) || ( pShLstVal->st == -1 ) ) {
					break;
				}
			}

			// 課税資産の譲渡等の対価の額			
			if ( pShLstVal->itmId == _T( "AYB00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_1A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_1B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_1C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 課税標準額
			else if ( pShLstVal->itmId == _T( "AYB00060" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_2A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_2B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_2C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 課税標準額に対する消費税額
			else if ( pShLstVal->itmId == _T( "AYB00100" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_3A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00110" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_3B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00120" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_3C );
				pShLstVal->sign1 = Sn_F601_3sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 貸倒回収に係る消費税額
			else if ( pShLstVal->itmId == _T( "AYB00140" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_4A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00150" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_4B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00160" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_4C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 売上対価の返還等に係る消費税額
			else if ( pShLstVal->itmId == _T( "AYB00180" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_5A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00190" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_5B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00200" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_5C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 控除対象仕入税額の計算の基礎となる消費税額
			else if ( pShLstVal->itmId == _T( "AYB00220" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_6A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00230" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_6B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYB00240" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_6C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 特別控除税額
			else if ( pShLstVal->itmId == _T( "AYC00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_7A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYC00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_7B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYC00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_7C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 貸倒れに係る税額
			else if ( pShLstVal->itmId == _T( "AYD00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_8A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYD00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_8B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "AYD00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F601_8C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
		}

		int adCnt = 0;
		for ( int i = 0; ; i++ ) {
			if ( strlen ( tagData601[i].tagName ) == 0 ) {
				break;
			}
			adCnt++;
		}

		int updCnt = ( int )updTagAry.GetCount();
		if ( adCnt != updCnt ) {
			CArray<unsigned char*> MonyPrtAry;

			MonyPrtAry.Add ( Sn_F601_1A );
			MonyPrtAry.Add ( Sn_F601_1B );
			MonyPrtAry.Add ( Sn_F601_1C );
			MonyPrtAry.Add ( Sn_F601_2A );
			MonyPrtAry.Add ( Sn_F601_2B );
			MonyPrtAry.Add ( Sn_F601_2C );
			MonyPrtAry.Add ( Sn_F601_3A );
			MonyPrtAry.Add ( Sn_F601_3B );
			MonyPrtAry.Add ( Sn_F601_3C );
			MonyPrtAry.Add ( Sn_F601_4A );
			MonyPrtAry.Add ( Sn_F601_4B );
			MonyPrtAry.Add ( Sn_F601_4C );
			MonyPrtAry.Add ( Sn_F601_5A );
			MonyPrtAry.Add ( Sn_F601_5B );
			MonyPrtAry.Add ( Sn_F601_5C );
			MonyPrtAry.Add ( Sn_F601_6A );
			MonyPrtAry.Add ( Sn_F601_6B );
			MonyPrtAry.Add ( Sn_F601_6C );
			MonyPrtAry.Add ( Sn_F601_7A );
			MonyPrtAry.Add ( Sn_F601_7B );
			MonyPrtAry.Add ( Sn_F601_7C );
			MonyPrtAry.Add ( Sn_F601_8A );
			MonyPrtAry.Add ( Sn_F601_8B );
			MonyPrtAry.Add ( Sn_F601_8C );

			BOOL findFlg = FALSE;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagData601[i].tagName ) == 0 ) {
					break;
				}

				findFlg = FALSE;
				for ( int j = 0; j < updCnt; j++ ) {
					if ( updTagAry[j].CompareNoCase ( tagData601[i].tagName ) == 0 ) {
						findFlg = TRUE;
						break;
					}

					if ( findFlg == FALSE ) {
						// 該当タグの追加
						pShLstVal->AddNew();
						pShLstVal->sn_seq = sn_seq;
						pShLstVal->version = nowVersion;
						pShLstVal->hyoId = pHyoID;
						pShLstVal->itmId = tagData601[i].tagName;
						m_Util.val_to_asci ( &pShLstVal->val, MonyPrtAry[i] );
						pShLstVal->sign1 = 0;
						pShLstVal->Update();
					}
				}
			}
		}
	}
	
	return 0;
}

//=======================================================
// 集計済みデータのセット
//-------------------------------------------------------
// 引数		pDBNpSub	：	財務クラス
//			pSyzSyukei　：　消費税集計クラス
//			isPluralZei	：　複数税率
//-------------------------------------------------------
// 返送値	 0			：　正常終了
//			-1			：　エラー
//=======================================================
int CH31SnFhyo6_2wariData::SetCalqedData(CSyzSyukei *pSyzSyukei, BOOL &isPluralZei)
{
	isPluralZei = FALSE;

	MoneyBasejagArray	money;

	money = pSyzSyukei->GetShinkokuData(_T("34"));

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned char exZeiflg = 0x00;	// D0：旧税率
									// D1：6.24%
									// D2：7.8%
									// D3：合計

	//(メモ)データの取得元と同じ６桁コードを使用する
	//付表４からデータを取得
	for (int i = 0; i < cnt; i++ ) {	
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++ ) {

			// 課税資産の譲渡等の対価の額
			if (money[i][j].code.Left(4) == _T("3419")) {
				if (money[i][j].code == _T("341902")) {
					memmove(Sn_F601_1A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("341903")) {
					memmove(Sn_F601_1B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("341900")) {
					memmove(Sn_F601_1C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// 課税標準額
			else if (money[i][j].code.Left(4) == _T("3418")) {
				if (money[i][j].code == _T("341802")) {
					memmove(Sn_F601_2A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("341803")) {
					memmove(Sn_F601_2B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("341800")) {
					memmove(Sn_F601_2C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// 課税標準額に対する消費税額		(メモ)旧・消費税額
			else if (money[i][j].code.Left(4) == _T("3420")) {
				if (money[i][j].code == _T("342002")) {
					memmove(Sn_F601_3A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
//						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342003")) {
					memmove(Sn_F601_3B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
//						exZeiflg |= 0x08;
					}
				}
				else if (money[i][j].code == _T("342000")) {
					memmove(Sn_F601_3C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// 貸倒回収に係る消費税額
			else if (money[i][j].code.Left(4) == _T("3421")) {
				if (money[i][j].code == _T("342102")) {
					memmove(Sn_F601_4A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342103")) {
					memmove(Sn_F601_4B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342100")) {
					memmove(Sn_F601_4C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// 売上対価の返還等に係る消費税額
			else if (money[i][j].code.Left(4) == _T("3423")) {
				if (money[i][j].code == _T("342302")) {
					memmove(Sn_F601_5A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342303")) {
					memmove(Sn_F601_5B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342300")) {
					memmove(Sn_F601_5C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// 貸倒れに係る税額
			else if (money[i][j].code.Left(4) == _T("3424")) {
				if (money[i][j].code == _T("342402")) {
					memmove(Sn_F601_8A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342403")) {
					memmove(Sn_F601_8B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342400")) {
					memmove(Sn_F601_8C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}
		}
	}

	//付表５からデータを取得
	money = pSyzSyukei->GetShinkokuData(_T("35"));

	cnt = (int)money.GetCount();
	incnt = 0;

	for (int i = 0; i < cnt; i++) {
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++) {
			// 控除対象仕入税額の計算の基礎となる消費税額
			if (money[i][j].code.Left(4) == _T("3574")) {
				if (money[i][j].code == _T("357402")) {
					memmove(Sn_F601_6A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("357403")) {
					memmove(Sn_F601_6B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("357400")) {
					memmove(Sn_F601_6C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// 特別控除税額
			else if (money[i][j].code.Left(4) == _T("3575")) {
				if (money[i][j].code == _T("357502")) {
					memmove(Sn_F601_7A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("357503")) {
					memmove(Sn_F601_7B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("357500")) {
					memmove(Sn_F601_7C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}
		}
	}

	if (((exZeiflg & 0x08) == 0x01) || ((exZeiflg & 0x08) == 0x02) || ((exZeiflg & 0x08) == 0x04) || ((exZeiflg & 0x08) == 0x08) || (exZeiflg == 0x00)) {
		isPluralZei = FALSE;
	}
	else {
		isPluralZei = TRUE;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 更新時の金額クリア
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	
//		type		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	
//-----------------------------------------------------------------------------
int CH31SnFhyo6_2wariData::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{

	m_Arith.l_clear(Sn_F601_1A);
	m_Arith.l_clear(Sn_F601_1B);
	m_Arith.l_clear(Sn_F601_1C);

	m_Arith.l_clear(Sn_F601_2A);
	m_Arith.l_clear(Sn_F601_2B);
	m_Arith.l_clear(Sn_F601_2C);

	m_Arith.l_clear(Sn_F601_3A);
	m_Arith.l_clear(Sn_F601_3B);
	m_Arith.l_clear(Sn_F601_3C);

	m_Arith.l_clear(Sn_F601_4A);
	m_Arith.l_clear(Sn_F601_4B);
	m_Arith.l_clear(Sn_F601_4C);

	m_Arith.l_clear(Sn_F601_5A);
	m_Arith.l_clear(Sn_F601_5B);
	m_Arith.l_clear(Sn_F601_5C);

	m_Arith.l_clear(Sn_F601_6A);
	m_Arith.l_clear(Sn_F601_6B);
	m_Arith.l_clear(Sn_F601_6C);

	m_Arith.l_clear(Sn_F601_7A);
	m_Arith.l_clear(Sn_F601_7B);
	m_Arith.l_clear(Sn_F601_7C);

	m_Arith.l_clear(Sn_F601_8A);
	m_Arith.l_clear(Sn_F601_8B);
	m_Arith.l_clear(Sn_F601_8C);	

	Sn_F601_3sw = 0;

	rsv = NULL;

	memset(rsv1, '\0', sizeof(rsv1));
	memset(rsv2, '\0', sizeof(rsv2));

	return 0;
}


//-----------------------------------------------------------------------------
// 手入力サインのクリア
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH31SnFhyo6_2wariData::ClearManualInputSign(BOOL isChgKzritu)
{

//	Sn_F401_2sw = 0;//yoshida190909

	//if (isChgKzritu) {
	//	Sn_F401_2sw |= 0x111;
	//}

	return 0;
}

//-----------------------------------------------------------------------------
// 更新時の金額追加書き込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH31SnFhyo6_2wariData::SetDataInRenew(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT(pDBNpSub->zvol);
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(pDBNpSub->zvol);
	if( KzHoushiki != ID_ICSSH_KANNI ){
		return 0;
	}

	// 自分自身と違う課税方式の金額に書き込み（相手方：付表１－１）		//(メモ)これは何をしている…？
	CString	filter;
	int		nowVersion = 2;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_01_HYOID, nowVersion);

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
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
		pShLstVal->Edit();
		pShLstVal->val = _T("0");
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DB上にデータ有り？
//-----------------------------------------------------------------------------
// 引数		pDBNpSub：	財務クラスのポインタ
//			sn_seq	：	チェック対象シーケンス（-1:全データ）
//-----------------------------------------------------------------------------
// 返送値	true	：	存在する
//			false	：	存在しない
//-----------------------------------------------------------------------------
bool CH31SnFhyo6_2wariData::IsExistData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ) {
		return false;
	}

	int nowVersion = 2;
	CString filter;
	if( sn_seq != -1 ){
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_6_01_HYOID, nowVersion);
	}
	else{
		filter.Format(_T("hyoId = \'%s\' and version = %d"), ICS_SN_FHYO_6_01_HYOID, nowVersion);
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return false;
		}
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL->GetRecordCount() ){
		return true;
	}
	else{
		return false;
	}
}
