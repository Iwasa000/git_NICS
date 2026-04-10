#include "StdAfx.h"
#include "H31SnFhyo5_02Data.h"

static SH_TAGNAME tagData502[] = {
	// 課税標準額に対する消費税額
	{ _T("BCB00020"), 0},
	{ _T("BCB00030"), 1},
	{ _T("BCB00040"), 3},
	{ _T("BCB00050"), 999},
	// 貸倒回収に係る消費税額
	{ _T("BCB00070"), 0},
	{ _T("BCB00080"), 1},
	{ _T("BCB00090"), 3},
	{ _T("BCB00100"), 999},
	// 売上対価の返還等に係る消費税額
	{ _T("BCB00120"), 0},
	{ _T("BCB00130"), 1},
	{ _T("BCB00140"), 3},
	{ _T("BCB00150"), 999},
	// 控除対象仕入税額の計算の基礎となる消費税額
	{ _T("BCB00170"), 0},
	{ _T("BCB00180"), 1},
	{ _T("BCB00190"), 3},
	{ _T("BCB00200"), 999},
	// 1種類の事業の専業者の場合の控除対象仕入税額
	// ④×みなし仕入率
	{ _T("BCC00010"), 999 },	// 区分
	{ _T("BCC00020"), 0},
	{ _T("BCC00030"), 1},
	{ _T("BCC00040"), 3},
	{ _T("BCC00050"), 999},
	// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
	// 事業区分別の合計額
	{ _T("BCD00030"), 0},
	{ _T("BCD00040"), 1},
	{ _T("BCD00050"), 3},
	{ _T("BCD00060"), 999},
	// 第一種事業
	{ _T("BCD00080"), 0},
	{ _T("BCD00090"), 1},
	{ _T("BCD00100"), 3},
	{ _T("BCD00110"), 999},
	// 第二種事業
	{ _T("BCD00140"), 0},
	{ _T("BCD00150"), 1},
	{ _T("BCD00160"), 3},
	{ _T("BCD00170"), 999},
	// 第三種事業
	{ _T("BCD00200"), 0},
	{ _T("BCD00210"), 1},
	{ _T("BCD00220"), 3},
	{ _T("BCD00230"), 999},
	// 第四種事業
	{ _T("BCD00260"), 0},
	{ _T("BCD00270"), 1},
	{ _T("BCD00280"), 3},
	{ _T("BCD00290"), 999},
	// 第五種事業
	{ _T("BCD00320"), 0},
	{ _T("BCD00330"), 1},
	{ _T("BCD00340"), 3},
	{ _T("BCD00350"), 999},
	// 第六種事業
	{ _T("BCD00690"), 0},
	{ _T("BCD00700"), 1},
	{ _T("BCD00710"), 3},
	{ _T("BCD00720"), 999},
	// (1)の事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	{ _T("BCD00390"), 0},
	{ _T("BCD00400"), 1},
	{ _T("BCD00410"), 3},
	{ _T("BCD00420"), 999},
	// 第一種事業
	{ _T("BCD00440"), 0},
	{ _T("BCD00450"), 1},
	{ _T("BCD00460"), 3},
	{ _T("BCD00470"), 999},
	// 第二種事業
	{ _T("BCD00490"), 0},
	{ _T("BCD00500"), 1},
	{ _T("BCD00510"), 3},
	{ _T("BCD00520"), 999},
	// 第三種事業
	{ _T("BCD00540"), 0},
	{ _T("BCD00550"), 1},
	{ _T("BCD00560"), 3},
	{ _T("BCD00570"), 999},
	// 第四種事業
	{ _T("BCD00590"), 0},
	{ _T("BCD00600"), 1},
	{ _T("BCD00610"), 3},
	{ _T("BCD00620"), 999},
	// 第五種事業
	{ _T("BCD00640"), 0},
	{ _T("BCD00650"), 1},
	{ _T("BCD00660"), 3},
	{ _T("BCD00670"), 999},
	// 第六種事業
	{ _T("BCD00750"), 0},
	{ _T("BCD00760"), 1},
	{ _T("BCD00770"), 3},
	{ _T("BCD00780"), 999},

	{ _T("") }
};


CH31SnFhyo5_02Data::CH31SnFhyo5_02Data( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure ( isTransitionalMeasure )
{
	memset ( Sn_F502_1A, '\0', sizeof ( Sn_F502_1A ) );
	memset ( Sn_F502_1B, '\0', sizeof ( Sn_F502_1B ) );
	memset ( Sn_F502_1C, '\0', sizeof ( Sn_F502_1C ) );
	memset ( Sn_F502_1X, '\0', sizeof ( Sn_F502_1X ) );

	memset ( Sn_F502_2A, '\0', sizeof ( Sn_F502_2A ) );
	memset ( Sn_F502_2B, '\0', sizeof ( Sn_F502_2B ) );
	memset ( Sn_F502_2C, '\0', sizeof ( Sn_F502_2C ) );
	memset ( Sn_F502_2X, '\0', sizeof ( Sn_F502_2X ) );

	memset ( Sn_F502_3A, '\0', sizeof ( Sn_F502_3A ) );
	memset ( Sn_F502_3B, '\0', sizeof ( Sn_F502_3B ) );
	memset ( Sn_F502_3C, '\0', sizeof ( Sn_F502_3C ) );
	memset ( Sn_F502_3X, '\0', sizeof ( Sn_F502_3X ) );

	memset ( Sn_F502_4A, '\0', sizeof ( Sn_F502_4A ) );
	memset ( Sn_F502_4B, '\0', sizeof ( Sn_F502_4B ) );
	memset ( Sn_F502_4C, '\0', sizeof ( Sn_F502_4C ) );
	memset ( Sn_F502_4X, '\0', sizeof ( Sn_F502_4X ) );

	memset ( Sn_F502_5A, '\0', sizeof ( Sn_F502_5A ) );
	memset ( Sn_F502_5B, '\0', sizeof ( Sn_F502_5B ) );
	memset ( Sn_F502_5C, '\0', sizeof ( Sn_F502_5C ) );
	memset ( Sn_F502_5X, '\0', sizeof ( Sn_F502_5X ) );

	memset ( Sn_F502_6A, '\0', sizeof ( Sn_F502_6A ) );
	memset ( Sn_F502_6B, '\0', sizeof ( Sn_F502_6B ) );
	memset ( Sn_F502_6C, '\0', sizeof ( Sn_F502_6C ) );
	memset ( Sn_F502_6X, '\0', sizeof ( Sn_F502_6X ) );

	memset ( Sn_F502_7A, '\0', sizeof ( Sn_F502_7A ) );
	memset ( Sn_F502_7B, '\0', sizeof ( Sn_F502_7B ) );
	memset ( Sn_F502_7C, '\0', sizeof ( Sn_F502_7C ) );
	memset ( Sn_F502_7X, '\0', sizeof ( Sn_F502_7X ) );

	memset ( Sn_F502_8A, '\0', sizeof ( Sn_F502_8A ) );
	memset ( Sn_F502_8B, '\0', sizeof ( Sn_F502_8B ) );
	memset ( Sn_F502_8C, '\0', sizeof ( Sn_F502_8C ) );
	memset ( Sn_F502_8X, '\0', sizeof ( Sn_F502_8X ) );

	memset ( Sn_F502_9A, '\0', sizeof ( Sn_F502_9A ) );
	memset ( Sn_F502_9B, '\0', sizeof ( Sn_F502_9B ) );
	memset ( Sn_F502_9C, '\0', sizeof ( Sn_F502_9C ) );
	memset ( Sn_F502_9X, '\0', sizeof ( Sn_F502_9X ) );

	memset ( Sn_F502_10A, '\0', sizeof ( Sn_F502_10A ) );
	memset ( Sn_F502_10B, '\0', sizeof ( Sn_F502_10B ) );
	memset ( Sn_F502_10C, '\0', sizeof ( Sn_F502_10C ) );
	memset ( Sn_F502_10X, '\0', sizeof ( Sn_F502_10X ) );

	memset ( Sn_F502_11A, '\0', sizeof ( Sn_F502_11A ) );
	memset ( Sn_F502_11B, '\0', sizeof ( Sn_F502_11B ) );
	memset ( Sn_F502_11C, '\0', sizeof ( Sn_F502_11C ) );
	memset ( Sn_F502_11X, '\0', sizeof ( Sn_F502_11X ) );

	memset ( Sn_F502_12A, '\0', sizeof ( Sn_F502_12A ) );
	memset ( Sn_F502_12B, '\0', sizeof ( Sn_F502_12B ) );
	memset ( Sn_F502_12C, '\0', sizeof ( Sn_F502_12C ) );
	memset ( Sn_F502_12X, '\0', sizeof ( Sn_F502_12X ) );

	memset ( Sn_F502_13A, '\0', sizeof ( Sn_F502_13A ) );
	memset ( Sn_F502_13B, '\0', sizeof ( Sn_F502_13B ) );
	memset ( Sn_F502_13C, '\0', sizeof ( Sn_F502_13C ) );
	memset ( Sn_F502_13X, '\0', sizeof ( Sn_F502_13X ) );

	memset ( Sn_F502_14A, '\0', sizeof ( Sn_F502_14A ) );
	memset ( Sn_F502_14B, '\0', sizeof ( Sn_F502_14B ) );
	memset ( Sn_F502_14C, '\0', sizeof ( Sn_F502_14C ) );
	memset ( Sn_F502_14X, '\0', sizeof ( Sn_F502_14X ) );

	memset ( Sn_F502_15A, '\0', sizeof ( Sn_F502_15A ) );
	memset ( Sn_F502_15B, '\0', sizeof ( Sn_F502_15B ) );
	memset ( Sn_F502_15C, '\0', sizeof ( Sn_F502_15C ) );
	memset ( Sn_F502_15X, '\0', sizeof ( Sn_F502_15X ) );

	memset ( Sn_F502_16A, '\0', sizeof ( Sn_F502_16A ) );
	memset ( Sn_F502_16B, '\0', sizeof ( Sn_F502_16B ) );
	memset ( Sn_F502_16C, '\0', sizeof ( Sn_F502_16C ) );
	memset ( Sn_F502_16X, '\0', sizeof ( Sn_F502_16X ) );

	memset ( Sn_F502_17A, '\0', sizeof ( Sn_F502_17A ) );
	memset ( Sn_F502_17B, '\0', sizeof ( Sn_F502_17B ) );
	memset ( Sn_F502_17C, '\0', sizeof ( Sn_F502_17C ) );
	memset ( Sn_F502_17X, '\0', sizeof ( Sn_F502_17X ) );

	memset ( Sn_F502_18A, '\0', sizeof ( Sn_F502_18A ) );
	memset ( Sn_F502_18B, '\0', sizeof ( Sn_F502_18B ) );
	memset ( Sn_F502_18C, '\0', sizeof ( Sn_F502_18C ) );
	memset ( Sn_F502_18X, '\0', sizeof ( Sn_F502_18X ) );

	memset ( Sn_F502_19A, '\0', sizeof ( Sn_F502_19A ) );
	memset ( Sn_F502_19B, '\0', sizeof ( Sn_F502_19B ) );
	memset ( Sn_F502_19C, '\0', sizeof ( Sn_F502_19C ) );
	memset ( Sn_F502_19X, '\0', sizeof ( Sn_F502_19X ) );

	Sn_F502_5kbn = 0;

	Sn_F502_6sw = 0;
	Sn_F502_14sw = 0;
	Sn_F502_15sw = 0;
	Sn_F502_16sw = 0;
	Sn_F502_17sw = 0;
	Sn_F502_18sw = 0;
	Sn_F502_19sw = 0;

	m_Arith.l_defn ( 0x16 );


	rsv = NULL;
}

CH31SnFhyo5_02Data::~CH31SnFhyo5_02Data()
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
int CH31SnFhyo5_02Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	CString filter;
	int		nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_5_02_HYOID, nowVersion );

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

		// 課税標準額に対する消費税額
		if ( pShLstVal->itmId == _T( "BCB00020" ) ) {
			m_Util.val_to_bin ( Sn_F502_1A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00030" ) ) {
			m_Util.val_to_bin ( Sn_F502_1B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00040" ) ) {
			m_Util.val_to_bin ( Sn_F502_1C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00050" ) ) {
			m_Util.val_to_bin ( Sn_F502_1X, pShLstVal->val );
		}
		// 貸倒回収に係る消費税額
		else if ( pShLstVal->itmId == _T( "BCB00070" ) ) {
			m_Util.val_to_bin ( Sn_F502_2A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00080" ) ) {
			m_Util.val_to_bin ( Sn_F502_2B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00090" ) ) {
			m_Util.val_to_bin ( Sn_F502_2C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00100" ) ) {
			m_Util.val_to_bin ( Sn_F502_2X, pShLstVal->val );
		}
		// 売上対価の返還等に係る消費税額
		else if ( pShLstVal->itmId == _T( "BCB00120" ) ) {
			m_Util.val_to_bin ( Sn_F502_3A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00130" ) ) {
			m_Util.val_to_bin ( Sn_F502_3B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00140" ) ) {
			m_Util.val_to_bin ( Sn_F502_3C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00150" ) ) {
			m_Util.val_to_bin ( Sn_F502_3X, pShLstVal->val );
		}
		// 控除対象仕入税額の計算基礎となる消費税額
		else if ( pShLstVal->itmId == _T( "BCB00170" ) ) {
			m_Util.val_to_bin ( Sn_F502_4A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00180" ) ) {
			m_Util.val_to_bin ( Sn_F502_4B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00190" ) ) {
			m_Util.val_to_bin ( Sn_F502_4C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCB00200" ) ) {
			m_Util.val_to_bin ( Sn_F502_4X, pShLstVal->val );
		}
		// 1種類の事業の専業者の場合の控除対象仕入税額
		// 4×みなし仕入率
		else if ( pShLstVal->itmId == _T( "BCC00010" ) ) {	// 区分
			Sn_F502_5kbn = atoi(pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCC00020" ) ) {
			m_Util.val_to_bin ( Sn_F502_5A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCC00030" ) ) {
			m_Util.val_to_bin ( Sn_F502_5B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCC00040" ) ) {
			m_Util.val_to_bin ( Sn_F502_5C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCC00050" ) ) {
			m_Util.val_to_bin ( Sn_F502_5X, pShLstVal->val );
		}
		// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
		// 事業区分別の売上高の明細
		// 事業区分別の合計額
		else if ( pShLstVal->itmId == _T( "BCD00030" ) ) {
			m_Util.val_to_bin ( Sn_F502_6A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00040" ) ) {
			m_Util.val_to_bin ( Sn_F502_6B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00050" ) ) {
			m_Util.val_to_bin ( Sn_F502_6C, pShLstVal->val );
			Sn_F502_6sw = pShLstVal->sign1;
		}
		else if ( pShLstVal->itmId == _T( "BCD00060" ) ) {
			m_Util.val_to_bin ( Sn_F502_6X, pShLstVal->val );
		}
		// 第一種事業(卸売業)
		else if ( pShLstVal->itmId == _T( "BCD00080" ) ) {
			m_Util.val_to_bin ( Sn_F502_7A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00090" ) ) {
			m_Util.val_to_bin ( Sn_F502_7B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00100" ) ) {
			m_Util.val_to_bin ( Sn_F502_7C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00110" ) ) {
			m_Util.val_to_bin ( Sn_F502_7X, pShLstVal->val );
		}
		// 第二種事業(小売業)
		else if ( pShLstVal->itmId == _T( "BCD00140" ) ) {
			m_Util.val_to_bin ( Sn_F502_8A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00150" ) ) {
			m_Util.val_to_bin ( Sn_F502_8B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00160" ) ) {
			m_Util.val_to_bin ( Sn_F502_8C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00170" ) ) {
			m_Util.val_to_bin ( Sn_F502_8X, pShLstVal->val );
		}
		// 第三種事業(製造業等)
		else if ( pShLstVal->itmId == _T( "BCD00200" ) ) {
			m_Util.val_to_bin ( Sn_F502_9A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00210" ) ) {
			m_Util.val_to_bin ( Sn_F502_9B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00220" ) ) {
			m_Util.val_to_bin ( Sn_F502_9C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00230" ) ) {
			m_Util.val_to_bin ( Sn_F502_9X, pShLstVal->val );
		}
		// 第四種事業(その他)
		else if ( pShLstVal->itmId == _T( "BCD00260" ) ) {
			m_Util.val_to_bin ( Sn_F502_10A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00270" ) ) {
			m_Util.val_to_bin ( Sn_F502_10B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00280" ) ) {
			m_Util.val_to_bin ( Sn_F502_10C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00290" ) ) {
			m_Util.val_to_bin ( Sn_F502_10X, pShLstVal->val );
		}
		// 第五種事業(サービス業等)
		else if ( pShLstVal->itmId == _T( "BCD00320" ) ) {
			m_Util.val_to_bin ( Sn_F502_11A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00330" ) ) {
			m_Util.val_to_bin ( Sn_F502_11B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00340" ) ) {
			m_Util.val_to_bin ( Sn_F502_11C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00350" ) ) {
			m_Util.val_to_bin ( Sn_F502_11X, pShLstVal->val );
		}
		// 第六種事業(不動産業)
		else if ( pShLstVal->itmId == _T( "BCD00690" ) ) {
			m_Util.val_to_bin ( Sn_F502_12A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00700" ) ) {
			m_Util.val_to_bin ( Sn_F502_12B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00710" ) ) {
			m_Util.val_to_bin ( Sn_F502_12C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00720" ) ) {
			m_Util.val_to_bin ( Sn_F502_12X, pShLstVal->val );
		}
		// 事業区分別の課税売上高に係る消費税額
		// 事業区分別の合計額
		else if ( pShLstVal->itmId == _T( "BCD00390" ) ) {
			m_Util.val_to_bin ( Sn_F502_13A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00400" ) ) {
			m_Util.val_to_bin ( Sn_F502_13B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00410" ) ) {
			m_Util.val_to_bin ( Sn_F502_13C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BCD00420" ) ) {
			m_Util.val_to_bin ( Sn_F502_13X, pShLstVal->val );
		}
		// 第一種事業(卸売業)
		else if ( pShLstVal->itmId == _T( "BCD00440" ) ) {
			m_Util.val_to_bin ( Sn_F502_14A, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_14A) >= 0)
				memmove( Sn_F502_14AP, Sn_F502_14A, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_14AP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00450" ) ) {
			m_Util.val_to_bin ( Sn_F502_14B, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_14B) >= 0)
				memmove( Sn_F502_14BP, Sn_F502_14B, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_14BP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00460" ) ) {
			m_Util.val_to_bin ( Sn_F502_14C, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_14C) >= 0)
				memmove( Sn_F502_14CP, Sn_F502_14C, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_14CP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00470" ) ) {
			m_Util.val_to_bin ( Sn_F502_14X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_14X) >= 0)
				memmove( Sn_F502_14XP, Sn_F502_14X, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_14XP, '\0', MONY_BUF_SIZE );
			Sn_F502_14sw = pShLstVal->sign1;
		}
		// 第二種事業(小売業)
		else if ( pShLstVal->itmId == _T( "BCD00490" ) ) {
			m_Util.val_to_bin ( Sn_F502_15A, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_15A) >= 0)
				memmove( Sn_F502_15AP, Sn_F502_15A, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_15AP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00500" ) ) {
			m_Util.val_to_bin ( Sn_F502_15B, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_15B) >= 0)
				memmove( Sn_F502_15BP, Sn_F502_15B, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_15BP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00510" ) ) {
			m_Util.val_to_bin ( Sn_F502_15C, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_15C) >= 0)
				memmove( Sn_F502_15CP, Sn_F502_15C, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_15CP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00520" ) ) {
			m_Util.val_to_bin ( Sn_F502_15X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_15X) >= 0)
				memmove( Sn_F502_15XP, Sn_F502_15X, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_15XP, '\0', MONY_BUF_SIZE );
			Sn_F502_15sw = pShLstVal->sign1;
		}
		// 第三種事業(製造業等)
		else if ( pShLstVal->itmId == _T( "BCD00540" ) ) {
			m_Util.val_to_bin ( Sn_F502_16A, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_16A) >= 0)
				memmove( Sn_F502_16AP, Sn_F502_16A, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_16AP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00550" ) ) {
			m_Util.val_to_bin ( Sn_F502_16B, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_16B) >= 0)
				memmove( Sn_F502_16BP, Sn_F502_16B, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_16BP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00560" ) ) {
			m_Util.val_to_bin ( Sn_F502_16C, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_16C) >= 0)
				memmove( Sn_F502_16CP, Sn_F502_16C, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_16CP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00570" ) ) {
			m_Util.val_to_bin ( Sn_F502_16X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_16X) >= 0)
				memmove( Sn_F502_16XP, Sn_F502_16X, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_16XP, '\0', MONY_BUF_SIZE );
			Sn_F502_16sw = pShLstVal->sign1;
		}
		// 第四種事業(その他)
		else if ( pShLstVal->itmId == _T( "BCD00590" ) ) {
			m_Util.val_to_bin ( Sn_F502_17A, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_17A) >= 0)
				memmove( Sn_F502_17AP, Sn_F502_17A, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_17AP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00600" ) ) {
			m_Util.val_to_bin ( Sn_F502_17B, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_17B) >= 0)
				memmove( Sn_F502_17BP, Sn_F502_17B, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_17BP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00610" ) ) {
			m_Util.val_to_bin ( Sn_F502_17C, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_17C) >= 0)
				memmove( Sn_F502_17CP, Sn_F502_17C, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_17CP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00620" ) ) {
			m_Util.val_to_bin ( Sn_F502_17X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_17X) >= 0)
				memmove( Sn_F502_17XP, Sn_F502_17X, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_17XP, '\0', MONY_BUF_SIZE );
			Sn_F502_17sw = pShLstVal->sign1;
		}
		// 第五種事業(サービス業等)
		else if ( pShLstVal->itmId == _T( "BCD00640" ) ) {
			m_Util.val_to_bin ( Sn_F502_18A, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_18A) >= 0)
				memmove( Sn_F502_18AP, Sn_F502_18A, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_18AP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00650" ) ) {
			m_Util.val_to_bin ( Sn_F502_18B, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_18B) >= 0)
				memmove( Sn_F502_18BP, Sn_F502_18B, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_18BP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00660" ) ) {
			m_Util.val_to_bin ( Sn_F502_18C, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_18C) >= 0)
				memmove( Sn_F502_18CP, Sn_F502_18C, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_18CP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00670" ) ) {
			m_Util.val_to_bin ( Sn_F502_18X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_18X) >= 0)
				memmove( Sn_F502_18XP, Sn_F502_18X, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_18XP, '\0', MONY_BUF_SIZE );
			Sn_F502_18sw = pShLstVal->sign1;
		}
		// 第六種事業(不動産業)
		else if ( pShLstVal->itmId == _T( "BCD00750" ) ) {
			m_Util.val_to_bin ( Sn_F502_19A, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_19A) >= 0)
				memmove( Sn_F502_19AP, Sn_F502_19A, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_19AP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00760" ) ) {
			m_Util.val_to_bin ( Sn_F502_19B, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_19B) >= 0)
				memmove( Sn_F502_19BP, Sn_F502_19B, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_19BP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00770" ) ) {
			m_Util.val_to_bin ( Sn_F502_19C, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_19C) >= 0)
				memmove( Sn_F502_19CP, Sn_F502_19C, MONY_BUF_SIZE );
			else
				memset ( Sn_F502_19CP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "BCD00780" ) ) {
			m_Util.val_to_bin ( Sn_F502_19X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F502_19X) >= 0)
				memmove( Sn_F502_19XP, Sn_F502_19X, MONY_BUF_SIZE);
			else
				memset ( Sn_F502_19XP, '\0', MONY_BUF_SIZE );
			Sn_F502_19sw = pShLstVal->sign1;
		}
	}

	// 申告書データ

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
int CH31SnFhyo5_02Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	CString filter;
	char *pHyoID = NULL;
	int nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_5_02_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_5_02_HYOID;

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
		// 課税標準額に対する消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 貸倒回収に係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00090" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00100" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 売上対価の返還等に係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00120" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00130" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00140" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00150" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 控除対象仕入税額の計算の基礎となる消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00170" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00180" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00190" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCB00200" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 1種類の事業の専業者の場合の控除対象仕入税額
		// 4×みなし仕入率
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCC00010" );
		pShLstVal->val.Format("%d", Sn_F502_5kbn);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCC00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCC00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCC00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCC00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
		// 事業者区分別の課税売上高の明細
		// 事業区分別の合計額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6C );
		pShLstVal->sign1 = Sn_F502_6sw;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00060" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第一種事業(卸売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00090" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00100" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00110" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第二種事業(小売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00140" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00150" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00160" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00170" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第三種事業(製造業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00200" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00210" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00220" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00230" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第四種事業(その他)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00260" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00270" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00280" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00290" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第五種事業(サービス業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00320" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00330" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00340" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00350" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第六種事業(不動産業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00690" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00700" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00710" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00720" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 事業区分別の課税売上高に係る消費税額の明細
		// 事業区分別の合計額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00390" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00400" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00410" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00420" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第一種事業(卸売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00440" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00450" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00460" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00470" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14X );
		pShLstVal->sign1 = Sn_F502_14sw;
		pShLstVal->Update();

		// 第二種事業(小売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00490" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00500" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00510" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00520" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15X );
		pShLstVal->sign1 = Sn_F502_15sw;
		pShLstVal->Update();

		// 第三種事業(製造業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00540" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00550" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00560" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00570" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16X );
		pShLstVal->sign1 = Sn_F502_16sw;
		pShLstVal->Update();

		// 第四種事業(その他)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00590" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00600" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00610" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00620" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17X );
		pShLstVal->sign1 = Sn_F502_17sw;
		pShLstVal->Update();

		// 第五種事業(サービス業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00640" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00650" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00660" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00670" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18X );
		pShLstVal->sign1 = Sn_F502_18sw;
		pShLstVal->Update();

		// 第六事業(不動産業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00750" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00760" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00770" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BCD00780" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19X );
		pShLstVal->sign1 = Sn_F502_19sw;
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

			// 課税標準額に対する消費税額
			if ( pShLstVal->itmId == _T( "BCB00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_1X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 貸倒回収に係る消費税額
			else if ( pShLstVal->itmId == _T( "BCB00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00090" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00100" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_2X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 売上対価の返還等に係る消費税額
			else if ( pShLstVal->itmId == _T( "BCB00120" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00130" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00140" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00150" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_3X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 控除対象仕入税額の計算の基礎となる消費税額
			else if ( pShLstVal->itmId == _T( "BCB00170" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00180" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00190" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCB00200" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_4X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 1種類の事業の専業者の場合の控除対象仕入税額
			else if (pShLstVal->itmId == _T("BCC00010")) {
				pShLstVal->Edit();
				pShLstVal->val.Format("%d", Sn_F502_5kbn);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if ( pShLstVal->itmId == _T( "BCC00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCC00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCC00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCC00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_5X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
			// 事業区分の課税売上高の明細
			// 事業者区分の合計額
			else if ( pShLstVal->itmId == _T( "BCD00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6C );
				pShLstVal->sign1 = Sn_F502_6sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00060" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_6X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第一種事業(卸売業)
			else if ( pShLstVal->itmId == _T( "BCD00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00090" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00100" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00110" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_7X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第二種事業(小売業)
			else if ( pShLstVal->itmId == _T( "BCD00140" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00150" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00160" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00170" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_8X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第三種事業(製造業等)
			else if ( pShLstVal->itmId == _T( "BCD00200" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00210" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00220" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00230" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_9X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第四種事業(その他)
			else if ( pShLstVal->itmId == _T( "BCD00260" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00270" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00280" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00290" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_10X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第五種事業(サービス業等)
			else if ( pShLstVal->itmId == _T( "BCD00320" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00330" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00340" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00350" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_11X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第六種事業(不動産業)
			else if ( pShLstVal->itmId == _T( "BCD00690" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00700" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00710" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00720" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_12X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 事業区分別の課税売上高に係る消費税額の明細
			// 事業区分別の合計額
			else if ( pShLstVal->itmId == _T( "BCD00390" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00400" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00410" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00420" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_13X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第一種事業(卸売業)
			else if ( pShLstVal->itmId == _T( "BCD00440" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00450" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00460" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00470" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_14X );
				pShLstVal->sign1 = Sn_F502_14sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第二種事業(小売業)
			else if ( pShLstVal->itmId == _T( "BCD00490" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00500" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00510" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00520" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_15X );
				pShLstVal->sign1 = Sn_F502_15sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第三種事業(製造業等)
			else if ( pShLstVal->itmId == _T( "BCD00540" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00550" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00560" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00570" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_16X );
				pShLstVal->sign1 = Sn_F502_16sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第四種事業(その他)
			else if ( pShLstVal->itmId == _T( "BCD00590" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00600" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00610" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00620" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_17X );
				pShLstVal->sign1 = Sn_F502_17sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第五種事業(サービス業等)
			else if ( pShLstVal->itmId == _T( "BCD00640" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00650" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00660" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00670" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_18X );
				pShLstVal->sign1 = Sn_F502_18sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第六種事業(不動産業)
			else if ( pShLstVal->itmId == _T( "BCD00750" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00760" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00770" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BCD00780" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F502_19X );
				pShLstVal->sign1 = Sn_F502_19sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
		}

		// 更新データ確認
		int adCnt = 0;
		for ( int i = 0; ; i++ ) {
			if ( strlen ( tagData502[i].tagName ) == 0 ) {
				break;
			}
			adCnt++;
		}

		unsigned char	aribuf[MONY_BUF_SIZE];
		char			dmybuf[256];
		int updCnt = ( int )updTagAry.GetCount();
		if ( adCnt != updCnt ) {
			// ないものを追加
			CArray<unsigned char*> MonyPtrAry;
			MonyPtrAry.Add ( Sn_F502_1A );
			MonyPtrAry.Add ( Sn_F502_1B );
			MonyPtrAry.Add ( Sn_F502_1C );
			MonyPtrAry.Add ( Sn_F502_1X );
			MonyPtrAry.Add ( Sn_F502_2A );
			MonyPtrAry.Add ( Sn_F502_2B );
			MonyPtrAry.Add ( Sn_F502_2C );
			MonyPtrAry.Add ( Sn_F502_2X );
			MonyPtrAry.Add ( Sn_F502_3A );
			MonyPtrAry.Add ( Sn_F502_3B );
			MonyPtrAry.Add ( Sn_F502_3C );
			MonyPtrAry.Add ( Sn_F502_3X );
			MonyPtrAry.Add ( Sn_F502_4A );
			MonyPtrAry.Add ( Sn_F502_4B );
			MonyPtrAry.Add ( Sn_F502_4C );
			MonyPtrAry.Add ( Sn_F502_4X );
			_itoa_s(Sn_F502_5kbn, dmybuf, 10);
			m_Arith.l_input(aribuf, dmybuf);
			MonyPtrAry.Add ( aribuf );
			MonyPtrAry.Add ( Sn_F502_5A );
			MonyPtrAry.Add ( Sn_F502_5B );
			MonyPtrAry.Add ( Sn_F502_5C );
			MonyPtrAry.Add ( Sn_F502_5X );
			MonyPtrAry.Add ( Sn_F502_6A );
			MonyPtrAry.Add ( Sn_F502_6B );
			MonyPtrAry.Add ( Sn_F502_6C );
			MonyPtrAry.Add ( Sn_F502_6X );
			MonyPtrAry.Add ( Sn_F502_7A );
			MonyPtrAry.Add ( Sn_F502_7B );
			MonyPtrAry.Add ( Sn_F502_7C );
			MonyPtrAry.Add ( Sn_F502_7X );
			MonyPtrAry.Add ( Sn_F502_8A );
			MonyPtrAry.Add ( Sn_F502_8B );
			MonyPtrAry.Add ( Sn_F502_8C );
			MonyPtrAry.Add ( Sn_F502_8X );
			MonyPtrAry.Add ( Sn_F502_9A );
			MonyPtrAry.Add ( Sn_F502_9B );
			MonyPtrAry.Add ( Sn_F502_9C );
			MonyPtrAry.Add ( Sn_F502_9X );
			MonyPtrAry.Add ( Sn_F502_10A );
			MonyPtrAry.Add ( Sn_F502_10B );
			MonyPtrAry.Add ( Sn_F502_10C );
			MonyPtrAry.Add ( Sn_F502_10X );
			MonyPtrAry.Add ( Sn_F502_11A );
			MonyPtrAry.Add ( Sn_F502_11B );
			MonyPtrAry.Add ( Sn_F502_11C );
			MonyPtrAry.Add ( Sn_F502_11X );
			MonyPtrAry.Add ( Sn_F502_12A );
			MonyPtrAry.Add ( Sn_F502_12B );
			MonyPtrAry.Add ( Sn_F502_12C );
			MonyPtrAry.Add ( Sn_F502_12X );
			MonyPtrAry.Add ( Sn_F502_13A );
			MonyPtrAry.Add ( Sn_F502_13B );
			MonyPtrAry.Add ( Sn_F502_13C );
			MonyPtrAry.Add ( Sn_F502_13X );
			MonyPtrAry.Add ( Sn_F502_14A );
			MonyPtrAry.Add ( Sn_F502_14B );
			MonyPtrAry.Add ( Sn_F502_14C );
			MonyPtrAry.Add ( Sn_F502_14X );
			MonyPtrAry.Add ( Sn_F502_15A );
			MonyPtrAry.Add ( Sn_F502_15B );
			MonyPtrAry.Add ( Sn_F502_15C );
			MonyPtrAry.Add ( Sn_F502_15X );
			MonyPtrAry.Add ( Sn_F502_16A );
			MonyPtrAry.Add ( Sn_F502_16B );
			MonyPtrAry.Add ( Sn_F502_16C );
			MonyPtrAry.Add ( Sn_F502_16X );
			MonyPtrAry.Add ( Sn_F502_17A );
			MonyPtrAry.Add ( Sn_F502_17B );
			MonyPtrAry.Add ( Sn_F502_17C );
			MonyPtrAry.Add ( Sn_F502_17X );
			MonyPtrAry.Add ( Sn_F502_18A );
			MonyPtrAry.Add ( Sn_F502_18B );
			MonyPtrAry.Add ( Sn_F502_18C );
			MonyPtrAry.Add ( Sn_F502_18X );
			MonyPtrAry.Add ( Sn_F502_19A );
			MonyPtrAry.Add ( Sn_F502_19B );
			MonyPtrAry.Add ( Sn_F502_19C );
			MonyPtrAry.Add ( Sn_F502_19X );

			BOOL findFlg = FALSE;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagData502[i].tagName ) == 0 ) {
					break;
				}

				findFlg = FALSE;
				for ( int j = 0; j < updCnt; j++ ) {
					if ( updTagAry[j].CompareNoCase ( tagData502[i].tagName ) == 0 ) {
						findFlg = TRUE;
						break;
					}
				}
				if ( findFlg == FALSE ) {
					// 該当タグの追加
					pShLstVal->AddNew();
					pShLstVal->sn_seq = sn_seq;
					pShLstVal->version = nowVersion;
					pShLstVal->hyoId = pHyoID;
					pShLstVal->itmId = tagData502[i].tagName;
					m_Util.val_to_asci ( &pShLstVal->val, MonyPtrAry[i] );
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
				}
			}
		}
	}

	// 申告書データ

	return 0;
}

//===============================================
// 集計済みデータのセット
//-----------------------------------------------
// 引数
//		pSyzSyukei		:		消費税集計クラス
//		isPluralZei		:		複数税率
//-----------------------------------------------
// 返送値
//			0			:		正常終了
//			-1			:		エラー
//===============================================
int CH31SnFhyo5_02Data::SetCalqedData ( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData ( _T( "35" ) );

	int cnt = ( int )money.GetCount();
	int incnt = 0;

	unsigned char exZeiFlg = 0x00;	// □□□□ □■■■
									// D0：税率3%
									// D1：税率4%
									// D2：税率6.3%

	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			// 課税標準額に対する消費税額
			if ( money[i][j].code.Left ( 4 ) == _T( "3501" ) ) {
				if ( money[i][j].code == _T( "350100" ) ) {
					memmove ( Sn_F502_1X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350101" ) ) {
					memmove ( Sn_F502_1A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350102" ) ) {
					memmove ( Sn_F502_1B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350103" ) ) {
					memmove ( Sn_F502_1C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 貸倒回収に係る消費税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3502" ) ) {
				if ( money[i][j].code == _T( "350200" ) ) {
					memmove ( Sn_F502_2X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350201" ) ) {
					memmove ( Sn_F502_2A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350202" ) ) {
					memmove ( Sn_F502_2B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350203" ) ) {
					memmove ( Sn_F502_2C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 売上対価の返還等に係る消費税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3503" ) ) {
				if ( money[i][j].code == _T( "350300" ) ) {
					memmove ( Sn_F502_3X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350301" ) ) {
					memmove ( Sn_F502_3A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350302" ) ) {
					memmove ( Sn_F502_3B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350303" ) ) {
					memmove ( Sn_F502_3C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 控除対象仕入税額の計算の基礎となる消費税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3504" ) ) {
				if ( money[i][j].code == _T( "350400" ) ) {
					memmove ( Sn_F502_4X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350401" ) ) {
					memmove ( Sn_F502_4A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350402" ) ) {
					memmove ( Sn_F502_4B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350403" ) ) {
					memmove ( Sn_F502_4C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 1種類の事業の専業者の場合の控除対象仕入税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3505" ) ) {
				if ( money[i][j].code == _T( "350500" ) ) {
					memmove ( Sn_F502_5X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350501" ) ) {
					memmove ( Sn_F502_5A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350502" ) ) {
					memmove ( Sn_F502_5B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350503" ) ) {
					memmove ( Sn_F502_5C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
			// 事業区分別の課税売上高
			// 事業区分別の合計額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3506" ) ) {
				if ( money[i][j].code == _T( "350600" ) ) {
					memmove ( Sn_F502_6X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350601" ) ) {
					memmove ( Sn_F502_6A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350602" ) ) {
					memmove ( Sn_F502_6B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350603" ) ) {
					memmove ( Sn_F502_6C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第一種事業(卸売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3507" ) ) {
				if ( money[i][j].code == _T( "350700" ) ) {
					memmove ( Sn_F502_7X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350701" ) ) {
					memmove ( Sn_F502_7A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350702" ) ) {
					memmove ( Sn_F502_7B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350703" ) ) {
					memmove ( Sn_F502_7C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第二種事業(小売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3508" ) ) {
				if ( money[i][j].code == _T( "350800" ) ) {
					memmove ( Sn_F502_8X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350801" ) ) {
					memmove ( Sn_F502_8A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350802" ) ) {
					memmove ( Sn_F502_8B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350803" ) ) {
					memmove ( Sn_F502_8C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第三種事業(製造業等)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3509" ) ) {
				if ( money[i][j].code == _T( "350900" ) ) {
					memmove ( Sn_F502_9X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "350901" ) ) {
					memmove ( Sn_F502_9A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "350902" ) ) {
					memmove ( Sn_F502_9B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "350903" ) ) {
					memmove ( Sn_F502_9C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第四種事業(その他)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3510" ) ) {
				if ( money[i][j].code == _T( "351000" ) ) {
					memmove ( Sn_F502_10X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "351001" ) ) {
					memmove ( Sn_F502_10A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "351002" ) ) {
					memmove ( Sn_F502_10B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "351003" ) ) {
					memmove ( Sn_F502_10C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第五種事業(サービス業等)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3511" ) ) {
				if ( money[i][j].code == _T( "351100" ) ) {
					memmove ( Sn_F502_11X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "351101" ) ) {
					memmove ( Sn_F502_11A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "351102" ) ) {
					memmove ( Sn_F502_11B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "351103" ) ) {
					memmove ( Sn_F502_11C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第六種事業(不動産業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3512" ) ) {
				if ( money[i][j].code == _T( "351200" ) ) {
					memmove ( Sn_F502_12X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "351201" ) ) {
					memmove ( Sn_F502_12A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "351202" ) ) {
					memmove ( Sn_F502_12B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "351203" ) ) {
					memmove ( Sn_F502_12C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 事業区分別の課税売上高に係る消費税額の明細
			// 事業区分別の合計額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3513" ) ) {
				if ( money[i][j].code == _T( "351300" ) ) {
					memmove ( Sn_F502_13X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "351301" ) ) {
					memmove ( Sn_F502_13A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "351302" ) ) {
					memmove ( Sn_F502_13B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "351303" ) ) {
					memmove ( Sn_F502_13C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第一種事業(卸売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3514" ) ) {
				if ( money[i][j].code == _T( "351400" ) ) {
					memmove ( Sn_F502_14X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "351401" ) ) {
					memmove ( Sn_F502_14A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "351402" ) ) {
					memmove ( Sn_F502_14B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "351403" ) ) {
					memmove ( Sn_F502_14C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第二種事業(小売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3515" ) ) {
				if ( money[i][j].code == _T( "351500" ) ) {
					memmove ( Sn_F502_15X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "351501" ) ) {
					memmove ( Sn_F502_15A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "351502" ) ) {
					memmove ( Sn_F502_15B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "351503" ) ) {
					memmove ( Sn_F502_15C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
				// 第三種事業(製造業等)
				else if ( money[i][j].code.Left ( 4 ) == _T( "3516" ) ) {
					if ( money[i][j].code == _T( "351600" ) ) {
						memmove ( Sn_F502_16X, money[i][j].arith, MONY_BUF_SIZE );
					}
					else if ( money[i][j].code == _T( "351601" ) ) {
						memmove ( Sn_F502_16A, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x01;
						}
					}
					else if ( money[i][j].code == _T( "351602" ) ) {
						memmove ( Sn_F502_16B, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x02;
						}
					}
					else if ( money[i][j].code == _T( "351603" ) ) {
						memmove ( Sn_F502_16C, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x04;
						}
					}
				}
				// 第四種事業(その他)
				else if ( money[i][j].code.Left ( 4 ) == _T( "3517" ) ) {
					if ( money[i][j].code == _T( "351700" ) ) {
						memmove ( Sn_F502_17X, money[i][j].arith, MONY_BUF_SIZE );
					}
					else if ( money[i][j].code == _T( "351701" ) ) {
						memmove ( Sn_F502_17A, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x01;
						}
					}
					else if ( money[i][j].code == _T( "351702" ) ) {
						memmove ( Sn_F502_17B, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x02;
						}
					}
					else if ( money[i][j].code == _T( "351703" ) ) {
						memmove ( Sn_F502_17C, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x04;
						}
					}
				}
				// 第五種事業(サービス業等)
				else if ( money[i][j].code.Left ( 4 ) == _T( "3518" ) ) {
					if ( money[i][j].code == _T( "351800" ) ) {
						memmove ( Sn_F502_18X, money[i][j].arith, MONY_BUF_SIZE );
					}
					else if ( money[i][j].code == _T( "351801" ) ) {
						memmove ( Sn_F502_18A, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x01;
						}
					}
					else if ( money[i][j].code == _T( "351802" ) ) {
						memmove ( Sn_F502_18B, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x02;
						}
					}
					else if ( money[i][j].code == _T( "351803" ) ) {
						memmove ( Sn_F502_18C, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x04;
						}
					}
				}
				// 第六種事業(不動産業)
				else if ( money[i][j].code.Left ( 4 ) == _T( "3519" ) ) {
					if ( money[i][j].code == _T( "351900" ) ) {
						memmove ( Sn_F502_19X, money[i][j].arith, MONY_BUF_SIZE );
					}
					else if ( money[i][j].code == _T( "351901" ) ) {
						memmove ( Sn_F502_19A, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x01;
						}
					}
					else if ( money[i][j].code == _T( "351902" ) ) {
						memmove ( Sn_F502_19B, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x02;
						}
					}
					else if ( money[i][j].code == _T( "351903" ) ) {
						memmove ( Sn_F502_19C, money[i][j].arith, MONY_BUF_SIZE );
						if ( m_Arith.l_test ( money[i][j].arith ) ) {
							exZeiFlg |= 0x04;
						}
					}
				}
			}
		}
	}

	if ( ( ( exZeiFlg & 0x07 ) == 0x01 ) || ( ( exZeiFlg & 0x07 ) == 0x02 ) || ( ( exZeiFlg & 0x04 ) == 0x04 ) || ( exZeiFlg == 0x00 ) ) {
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
int CH31SnFhyo5_02Data::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{

	m_Arith.l_clear(Sn_F502_1A);
	m_Arith.l_clear(Sn_F502_1B);
	m_Arith.l_clear(Sn_F502_1C);
	m_Arith.l_clear(Sn_F502_1X);

	m_Arith.l_clear(Sn_F502_2A);
	m_Arith.l_clear(Sn_F502_2B);
	m_Arith.l_clear(Sn_F502_2C);
	m_Arith.l_clear(Sn_F502_2X);

	m_Arith.l_clear(Sn_F502_3A);
	m_Arith.l_clear(Sn_F502_3B);
	m_Arith.l_clear(Sn_F502_3C);
	m_Arith.l_clear(Sn_F502_3X);

	m_Arith.l_clear(Sn_F502_4A);
	m_Arith.l_clear(Sn_F502_4B);
	m_Arith.l_clear(Sn_F502_4C);
	m_Arith.l_clear(Sn_F502_4X);

	m_Arith.l_clear(Sn_F502_5A);
	m_Arith.l_clear(Sn_F502_5B);
	m_Arith.l_clear(Sn_F502_5C);
	m_Arith.l_clear(Sn_F502_5X);

	m_Arith.l_clear(Sn_F502_6A);
	m_Arith.l_clear(Sn_F502_6B);
	m_Arith.l_clear(Sn_F502_6C);
	m_Arith.l_clear(Sn_F502_6X);

	m_Arith.l_clear(Sn_F502_7A);
	m_Arith.l_clear(Sn_F502_7B);
	m_Arith.l_clear(Sn_F502_7C);
	m_Arith.l_clear(Sn_F502_7X);

	m_Arith.l_clear(Sn_F502_8A);
	m_Arith.l_clear(Sn_F502_8B);
	m_Arith.l_clear(Sn_F502_8C);
	m_Arith.l_clear(Sn_F502_8X);

	m_Arith.l_clear(Sn_F502_9A);
	m_Arith.l_clear(Sn_F502_9B);
	m_Arith.l_clear(Sn_F502_9C);
	m_Arith.l_clear(Sn_F502_9X);

	m_Arith.l_clear(Sn_F502_10A);
	m_Arith.l_clear(Sn_F502_10B);
	m_Arith.l_clear(Sn_F502_10C);
	m_Arith.l_clear(Sn_F502_10X);

	m_Arith.l_clear(Sn_F502_11A);
	m_Arith.l_clear(Sn_F502_11B);
	m_Arith.l_clear(Sn_F502_11C);
	m_Arith.l_clear(Sn_F502_11X);

	m_Arith.l_clear(Sn_F502_12A);
	m_Arith.l_clear(Sn_F502_12B);
	m_Arith.l_clear(Sn_F502_12C);
	m_Arith.l_clear(Sn_F502_12X);

	m_Arith.l_clear(Sn_F502_13A);
	m_Arith.l_clear(Sn_F502_13B);
	m_Arith.l_clear(Sn_F502_13C);
	m_Arith.l_clear(Sn_F502_13X);

	m_Arith.l_clear(Sn_F502_14A);
	m_Arith.l_clear(Sn_F502_14B);
	m_Arith.l_clear(Sn_F502_14C);
	m_Arith.l_clear(Sn_F502_14X);

	m_Arith.l_clear(Sn_F502_15A);
	m_Arith.l_clear(Sn_F502_15B);
	m_Arith.l_clear(Sn_F502_15C);
	m_Arith.l_clear(Sn_F502_15X);

	m_Arith.l_clear(Sn_F502_16A);
	m_Arith.l_clear(Sn_F502_16B);
	m_Arith.l_clear(Sn_F502_16C);
	m_Arith.l_clear(Sn_F502_16X);

	m_Arith.l_clear(Sn_F502_17A);
	m_Arith.l_clear(Sn_F502_17B);
	m_Arith.l_clear(Sn_F502_17C);
	m_Arith.l_clear(Sn_F502_17X);

	m_Arith.l_clear(Sn_F502_18A);
	m_Arith.l_clear(Sn_F502_18B);
	m_Arith.l_clear(Sn_F502_18C);
	m_Arith.l_clear(Sn_F502_18X);

	m_Arith.l_clear(Sn_F502_19A);
	m_Arith.l_clear(Sn_F502_19B);
	m_Arith.l_clear(Sn_F502_19C);
	m_Arith.l_clear(Sn_F502_19X);

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
int CH31SnFhyo5_02Data::ClearManualInputSign(BOOL isChgKzritu)
{
	Sn_F502_6sw = 0;

	//---->19.10.23
	//連動から非連動切替時、手入力ONにする処理を追加する為、コメント化
//	Sn_F502_14sw = 0;
//	Sn_F502_15sw = 0;
//	Sn_F502_16sw = 0;
//	Sn_F502_17sw = 0;
//	Sn_F502_18sw = 0;
//	Sn_F502_19sw = 0;
	//<--------------

	/*if (isChgKzritu) {
		Sn_F502_6sw |= 0x888;
		Sn_F502_14sw |= 0x888;
		Sn_F502_15sw |= 0x888;
		Sn_F502_16sw |= 0x888;
		Sn_F502_17sw |= 0x888;
		Sn_F502_18sw |= 0x888;
		Sn_F502_19sw |= 0x888;
	}
	*/
	return 0;
}


//===============================================
// 旧様式のデータ取得
//-----------------------------------------------
// 引数
//		pDBNpSub	:	公益クラス
//		sn_seq		:	履歴シーケンス
//		pSnHeadData	:	消費税ヘッダ情報
//-----------------------------------------------
// 返送値
//			0		:	正常終了
//			-1		:	エラー
//===============================================
int CH31SnFhyo5_02Data::GetPastData ( CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData )
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

	// 連動時は旧様式のデータを取得しない
	if ( !( pSnHeadData->Sn_Sign4 & 0x80 ) ) {
		return 0;
	}

	int nowVersion = 1;
	CString filter;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_5_2_HYOID, nowVersion );

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal ( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	for ( int idx = 0; ; idx++ ) {
		if ( idx == 0 ) {
			if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}
		else {
			if ( ( pShLstVal->MoveNext() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}

		// 事業区分別 課税売上高合計額
		if ( pShLstVal->itmId == _T( "AHG00040" ) ) {
			m_Util.val_to_bin ( Sn_F502_6A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00050" ) ) {
			m_Util.val_to_bin ( Sn_F502_6B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00060" ) ) {
			m_Util.val_to_bin ( Sn_F502_6C, pShLstVal->val );
			Sn_F502_6sw = pShLstVal->sign1;
		}
		// 第一種事業(卸売業)
		else if ( pShLstVal->itmId == _T( "AHG00090" ) ) {
			m_Util.val_to_bin ( Sn_F502_7A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00100" ) ) {
			m_Util.val_to_bin ( Sn_F502_7B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00110" ) ) {
			m_Util.val_to_bin ( Sn_F502_7C, pShLstVal->val );
		}
		// 第二種事業(小売業)
		else if ( pShLstVal->itmId == _T( "AHG00150" ) ) {
			m_Util.val_to_bin ( Sn_F502_8A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00160" ) ) {
			m_Util.val_to_bin ( Sn_F502_8B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00170" ) ) {
			m_Util.val_to_bin ( Sn_F502_8C, pShLstVal->val );
		}
		// 第三種事業(製造業等)
		else if ( pShLstVal->itmId == _T( "AHG00210" ) ) {
			m_Util.val_to_bin ( Sn_F502_9A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00220" ) ) {
			m_Util.val_to_bin ( Sn_F502_9B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00230" ) ) {
			m_Util.val_to_bin ( Sn_F502_9C, pShLstVal->val );
		}
		// 第四種事業(その他)
		else if ( pShLstVal->itmId == _T( "AHG00270" ) ) {
			m_Util.val_to_bin ( Sn_F502_10A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00280" ) ) {
			m_Util.val_to_bin ( Sn_F502_10B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00290" ) ) {
			m_Util.val_to_bin ( Sn_F502_10C, pShLstVal->val );
		}
		// 第五種事業(サービス業等)
		else if ( pShLstVal->itmId == _T( "AHG00330" ) ) {
			m_Util.val_to_bin ( Sn_F502_11A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00340" ) ) {
			m_Util.val_to_bin ( Sn_F502_11B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00350" ) ) {
			m_Util.val_to_bin ( Sn_F502_11C, pShLstVal->val );
		}
		// 第六種事業(不動産業)
		else if ( pShLstVal->itmId == _T( "AHG00390" ) ) {
			m_Util.val_to_bin ( Sn_F502_12A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00400" ) ) {
			m_Util.val_to_bin ( Sn_F502_12B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00410" ) ) {
			m_Util.val_to_bin ( Sn_F502_12C, pShLstVal->val );
		}
		// 第一種事業(卸売業) 消費税額
		else if ( pShLstVal->itmId == _T( "AHG00450" ) ) {
			m_Util.val_to_bin ( Sn_F502_14A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00460" ) ) {
			m_Util.val_to_bin ( Sn_F502_14B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00470" ) ) {
			m_Util.val_to_bin ( Sn_F502_14C, pShLstVal->val );
			Sn_F502_14sw = pShLstVal->sign1;

			// 手入力サイン更新
			if ( Sn_F502_14sw & 0x01 ) {
				Sn_F502_14sw &= ~0x01;
			}
			if ( Sn_F502_14sw & 0x10 ) {
				Sn_F502_14sw &= ~0x10;
			}
			if ( Sn_F502_14sw & 0x100 ) {
				Sn_F502_14sw &= ~0x100;
			}
		}
		// 第二種事業(小売業) 消費税額
		else if ( pShLstVal->itmId == _T( "AHG00500" ) ) {
			m_Util.val_to_bin ( Sn_F502_15A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00510" ) ) {
			m_Util.val_to_bin ( Sn_F502_15B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00520" ) ) {
			m_Util.val_to_bin ( Sn_F502_15C, pShLstVal->val );
			Sn_F502_15sw = pShLstVal->sign1;

			// 手入力サイン更新
			if ( Sn_F502_15sw & 0x01 ) {
				Sn_F502_15sw &= ~0x01;
			}
			if ( Sn_F502_15sw & 0x10 ) {
				Sn_F502_15sw &= ~0x10;
			}
			if ( Sn_F502_15sw & 0x100 ) {
				Sn_F502_15sw &= ~0x100;
			}
		}
		// 第三種事業(製造業等) 消費税額
		else if ( pShLstVal->itmId == _T( "AHG00550" ) ) {
			m_Util.val_to_bin ( Sn_F502_16A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00560" ) ) {
			m_Util.val_to_bin ( Sn_F502_16B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00570" ) ) {
			m_Util.val_to_bin ( Sn_F502_16C, pShLstVal->val );
			Sn_F502_16sw = pShLstVal->sign1;

			// 手入力サイン更新
			if ( Sn_F502_16sw & 0x01 ) {
				Sn_F502_16sw &= ~0x01;
			}
			if ( Sn_F502_16sw & 0x10 ) {
				Sn_F502_16sw &= ~0x10;
			}
			if ( Sn_F502_16sw & 0x100 ) {
				Sn_F502_16sw &= ~0x100;
			}
		}
		// 第四種事業(その他) 消費税額
		else if ( pShLstVal->itmId == _T( "AHG00600" ) ) {
			m_Util.val_to_bin ( Sn_F502_17A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00610" ) ) {
			m_Util.val_to_bin ( Sn_F502_17B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00620" ) ) {
			m_Util.val_to_bin ( Sn_F502_17C, pShLstVal->val );
			Sn_F502_17sw = pShLstVal->sign1;

			// 手入力サイン更新
			if ( Sn_F502_17sw & 0x01 ) {
				Sn_F502_17sw &= ~0x01;
			}
			if ( Sn_F502_17sw & 0x10 ) {
				Sn_F502_17sw &= ~0x10;
			}
			if ( Sn_F502_17sw & 0x100 ) {
				Sn_F502_17sw &= ~0x100;
			}
		}
		// 第五種事業(サービス業等) 消費税額
		else if ( pShLstVal->itmId == _T( "AHG00650" ) ) {
			m_Util.val_to_bin ( Sn_F502_18A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00660" ) ) {
			m_Util.val_to_bin ( Sn_F502_18B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG00670" ) ) {
			m_Util.val_to_bin ( Sn_F502_18C, pShLstVal->val );
			Sn_F502_18sw = pShLstVal->sign1;

			// 手入力サイン更新
			if ( Sn_F502_18sw & 0x01 ) {
				Sn_F502_18sw &= ~0x01;
			}
			if ( Sn_F502_18sw & 0x10 ) {
				Sn_F502_18sw &= ~0x10;
			}
			if ( Sn_F502_18sw & 0x100 ) {
				Sn_F502_18sw &= ~0x100;
			}
		}
		// 第六種事業(不動産業) 消費税額
		else if ( pShLstVal->itmId == _T( "AHG01700" ) ) {
			m_Util.val_to_bin ( Sn_F502_19A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG01710" ) ) {
			m_Util.val_to_bin ( Sn_F502_19B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AHG01720" ) ) {
			m_Util.val_to_bin ( Sn_F502_19C, pShLstVal->val );
			Sn_F502_19sw = pShLstVal->sign1;

			// 手入力サイン更新
			if ( Sn_F502_19sw & 0x01 ) {
				Sn_F502_19sw &= ~0x01;
			}
			if ( Sn_F502_19sw & 0x10 ) {
				Sn_F502_19sw &= ~0x10;
			}
			if ( Sn_F502_19sw & 0x100 ) {
				Sn_F502_19sw &= ~0x100;
			}
		}
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
bool CH31SnFhyo5_02Data::IsExistData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ) {
		return false;
	}

	int nowVersion = 2;
	CString filter;
	if( sn_seq != -1 ){
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_5_02_HYOID, nowVersion);
	}
	else{
		filter.Format(_T("hyoId = \'%s\' and version = %d"), ICS_SN_FHYO_5_02_HYOID, nowVersion);
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
