#include "StdAfx.h"
#include "H31SnFhyo5_01Data.h"

static SH_TAGNAME tagData501[] = {
	// 課税標準額に対する消費税額
	{ _T("CTB00020"), 0 },
	{ _T("CTB00030"), 1 },
	{ _T("CTB00040"), 3 },
	{ _T("CTB00050"), 999 },
	// 貸倒回収に係る消費税額
	{ _T("CTB00070"), 0 },
	{ _T("CTB00080"), 1 },
	{ _T("CTB00090"), 3 },
	{ _T("CTB00100"), 999 },
	// 売上対価の返還等に係る消費税額
	{ _T("CTB00120"), 0 },
	{ _T("CTB00130"), 1 },
	{ _T("CTB00140"), 3 },
	{ _T("CTB00150"), 999 },
	// 控除対象仕入税額の計算の基礎となる消費税額
	{ _T("CTB00170"), 0 },
	{ _T("CTB00180"), 1 },
	{ _T("CTB00190"), 3 },
	{ _T("CTB00200"), 999 },
	// 1種類の事業の専業者の場合の控除対象仕入税額
	// ④×みなし仕入率
	{ _T("CTC00010"), 999 },	// 区分
	{ _T("CTC00020"), 0 },
	{ _T("CTC00030"), 1 },
	{ _T("CTC00040"), 3 },
	{ _T("CTC00050"), 999 },
	// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
	// (1) 事業区分別の課税売上高(税抜き)の明細
	// 事業区分別の合計額
	{ _T("CTD00030"), 0 },
	{ _T("CTD00040"), 1 },
	{ _T("CTD00050"), 3 },
	{ _T("CTD00060"), 999 },
	// 第一種事業(卸売業)
	{ _T("CTD00080"), 0 },
	{ _T("CTD00090"), 1 },
	{ _T("CTD00100"), 3 },
	//{ _T("CTD00110"), 999 },
	// ↓金額と売上割合　番号何で振ればいいか？ 第二種～第六種も
	{ _T("CTD00120"), 999 },	// 金額
	//{ _T("CTD00130"), 999 },	// 売上割合
	// 第二種事業(小売業)
	{ _T("CTD00150"), 0 },
	{ _T("CTD00160"), 1 },
	{ _T("CTD00170"), 3 },
	//{ _T("CTD00180"), 999 },
	{ _T("CTD00190"), 999 },	// 金額
	//{ _T("CTD00200"), 999 },	// 売上割合
	// 第三種事業(製造業)
	{ _T("CTD00220"), 0 },
	{ _T("CTD00230"), 1 },
	{ _T("CTD00240"), 3 },
	//{ _T("CTD00250"), 999 },
	{ _T("CTD00260"), 999 },	// 金額
	//{ _T("CTD00270"), 999 },	// 売上割合
	// 第四種事業(その他)
	{ _T("CTD00290"), 0 },
	{ _T("CTD00300"), 1 },
	{ _T("CTD00310"), 3 },
	//{ _T("CTD00320"), 999 },
	{ _T("CTD00330"), 999 },	// 金額
	//{ _T("CTD00340"), 999 },	// 売上割合
	// 第五種事業(サービス業)
	{ _T("CTD00360"), 0 },
	{ _T("CTD00370"), 1 },
	{ _T("CTD00380"), 3 },
	//{ _T("CTD00390"), 999 },
	{ _T("CTD00400"), 999 },	// 金額
	//{ _T("CTD00410"), 999 },	// 売上割合
	// 第六種事業(不動産業)
	{ _T("CTD00430"), 0 },
	{ _T("CTD00440"), 1 },
	{ _T("CTD00450"), 3 },
	//{ _T("CTD00460"), 999 },
	{ _T("CTD00470"), 999 },	// 金額
	//{ _T("CTD00480"), 999 },	// 売上割合
	// (2) (1)の事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	{ _T("CTD00510"), 0 },
	{ _T("CTD00520"), 1 },
	{ _T("CTD00530"), 3 },
	{ _T("CTD00540"), 999 },
	// 第一種事業(卸売業)
	{ _T("CTD00560"), 0 },
	{ _T("CTD00570"), 1 },
	{ _T("CTD00580"), 3 },
	{ _T("CTD00590"), 999 },
	// 第二種事業(小売業)
	{ _T("CTD00610"), 0 },
	{ _T("CTD00620"), 1 },
	{ _T("CTD00630"), 3 },
	{ _T("CTD00640"), 999 },
	// 第三種事業(製造業等)
	{ _T("CTD00660"), 0 },
	{ _T("CTD00670"), 1 },
	{ _T("CTD00680"), 3 },
	{ _T("CTD00690"), 999 },
	// 第四種事業(その他)
	{ _T("CTD00710"), 0 },
	{ _T("CTD00720"), 1 },
	{ _T("CTD00730"), 3 },
	{ _T("CTD00740"), 999 },
	// 第五種事業(サービス業等)
	{ _T("CTD00760"), 0 },
	{ _T("CTD00770"), 1 },
	{ _T("CTD00780"), 3 },
	{ _T("CTD00790"), 999 },
	// 第六種事業(不動産業)
	{ _T("CTD00810"), 0 },
	{ _T("CTD00820"), 1 },
	{ _T("CTD00830"), 3 },
	{ _T("CTD00840"), 999 },

	{ _T("") }

};

CH31SnFhyo5_01Data::CH31SnFhyo5_01Data( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure ( isTransitionalMeasure )
{
	memset ( Sn_F501_1X, '\0', sizeof ( Sn_F501_1X ) );
	memset ( Sn_F501_1D, '\0', sizeof ( Sn_F501_1D ) );
	memset ( Sn_F501_1E, '\0', sizeof ( Sn_F501_1E ) );
	memset ( Sn_F501_1F, '\0', sizeof ( Sn_F501_1F ) );

	memset ( Sn_F501_2X, '\0', sizeof ( Sn_F501_2X ) );
	memset ( Sn_F501_2D, '\0', sizeof ( Sn_F501_2D ) );
	memset ( Sn_F501_2E, '\0', sizeof ( Sn_F501_2E ) );
	memset ( Sn_F501_2F, '\0', sizeof ( Sn_F501_2F ) );

	memset ( Sn_F501_3X, '\0', sizeof ( Sn_F501_3X ) );
	memset ( Sn_F501_3D, '\0', sizeof ( Sn_F501_3D ) );
	memset ( Sn_F501_3E, '\0', sizeof ( Sn_F501_3E ) );
	memset ( Sn_F501_3F, '\0', sizeof ( Sn_F501_3F ) );

	memset ( Sn_F501_4X, '\0', sizeof ( Sn_F501_4X ) );
	memset ( Sn_F501_4D, '\0', sizeof ( Sn_F501_4D ) );
	memset ( Sn_F501_4E, '\0', sizeof ( Sn_F501_4E ) );
	memset ( Sn_F501_4F, '\0', sizeof ( Sn_F501_4F ) );

	memset ( Sn_F501_5X, '\0', sizeof ( Sn_F501_5X ) );
	memset ( Sn_F501_5D, '\0', sizeof ( Sn_F501_5D ) );
	memset ( Sn_F501_5E, '\0', sizeof ( Sn_F501_5E ) );
	memset ( Sn_F501_5F, '\0', sizeof ( Sn_F501_5F ) );

	memset ( Sn_F501_6X, '\0', sizeof ( Sn_F501_6X ) );
	memset ( Sn_F501_6D, '\0', sizeof ( Sn_F501_6D ) );
	memset ( Sn_F501_6E, '\0', sizeof ( Sn_F501_6E ) );
	memset ( Sn_F501_6F, '\0', sizeof ( Sn_F501_6F ) );

	memset ( Sn_F501_7X, '\0', sizeof ( Sn_F501_7X ) );
	memset ( Sn_F501_7D, '\0', sizeof ( Sn_F501_7D ) );
	memset ( Sn_F501_7E, '\0', sizeof ( Sn_F501_7E ) );
	memset ( Sn_F501_7F, '\0', sizeof ( Sn_F501_7F ) );

	memset ( Sn_F501_8X, '\0', sizeof ( Sn_F501_8X ) );
	memset ( Sn_F501_8D, '\0', sizeof ( Sn_F501_8D ) );
	memset ( Sn_F501_8E, '\0', sizeof ( Sn_F501_8E ) );
	memset ( Sn_F501_8F, '\0', sizeof ( Sn_F501_8F ) );

	memset ( Sn_F501_9X, '\0', sizeof ( Sn_F501_9X ) );
	memset ( Sn_F501_9D, '\0', sizeof ( Sn_F501_9D ) );
	memset ( Sn_F501_9E, '\0', sizeof ( Sn_F501_9E ) );
	memset ( Sn_F501_9F, '\0', sizeof ( Sn_F501_9F ) );

	memset ( Sn_F501_10X, '\0', sizeof ( Sn_F501_10X ) );
	memset ( Sn_F501_10D, '\0', sizeof ( Sn_F501_10D ) );
	memset ( Sn_F501_10E, '\0', sizeof ( Sn_F501_10E ) );
	memset ( Sn_F501_10F, '\0', sizeof ( Sn_F501_10F ) );

	memset ( Sn_F501_11X, '\0', sizeof ( Sn_F501_11X ) );
	memset ( Sn_F501_11D, '\0', sizeof ( Sn_F501_11D ) );
	memset ( Sn_F501_11E, '\0', sizeof ( Sn_F501_11E ) );
	memset ( Sn_F501_11F, '\0', sizeof ( Sn_F501_11F ) );

	memset ( Sn_F501_12X, '\0', sizeof ( Sn_F501_12X ) );
	memset ( Sn_F501_12D, '\0', sizeof ( Sn_F501_12D ) );
	memset ( Sn_F501_12E, '\0', sizeof ( Sn_F501_12E ) );
	memset ( Sn_F501_12F, '\0', sizeof ( Sn_F501_12F ) );

	memset ( Sn_F501_13X, '\0', sizeof ( Sn_F501_13X ) );
	memset ( Sn_F501_13D, '\0', sizeof ( Sn_F501_13D ) );
	memset ( Sn_F501_13E, '\0', sizeof ( Sn_F501_13E ) );
	memset ( Sn_F501_13F, '\0', sizeof ( Sn_F501_13F ) );

	memset ( Sn_F501_14X, '\0', sizeof ( Sn_F501_14X ) );
	memset ( Sn_F501_14D, '\0', sizeof ( Sn_F501_14D ) );
	memset ( Sn_F501_14E, '\0', sizeof ( Sn_F501_14E ) );
	memset ( Sn_F501_14F, '\0', sizeof ( Sn_F501_14F ) );

	memset ( Sn_F501_15X, '\0', sizeof ( Sn_F501_15X ) );
	memset ( Sn_F501_15D, '\0', sizeof ( Sn_F501_15D ) );
	memset ( Sn_F501_15E, '\0', sizeof ( Sn_F501_15E ) );
	memset ( Sn_F501_15F, '\0', sizeof ( Sn_F501_15F ) );

	memset ( Sn_F501_16X, '\0', sizeof ( Sn_F501_16X ) );
	memset ( Sn_F501_16D, '\0', sizeof ( Sn_F501_16D ) );
	memset ( Sn_F501_16E, '\0', sizeof ( Sn_F501_16E ) );
	memset ( Sn_F501_16F, '\0', sizeof ( Sn_F501_16F ) );

	memset ( Sn_F501_17X, '\0', sizeof ( Sn_F501_17X ) );
	memset ( Sn_F501_17D, '\0', sizeof ( Sn_F501_17D ) );
	memset ( Sn_F501_17E, '\0', sizeof ( Sn_F501_17E ) );
	memset ( Sn_F501_17F, '\0', sizeof ( Sn_F501_17F ) );

	memset ( Sn_F501_18X, '\0', sizeof ( Sn_F501_18X ) );
	memset ( Sn_F501_18D, '\0', sizeof ( Sn_F501_18D ) );
	memset ( Sn_F501_18E, '\0', sizeof ( Sn_F501_18E ) );
	memset ( Sn_F501_18F, '\0', sizeof ( Sn_F501_18F ) );

	memset ( Sn_F501_19X, '\0', sizeof ( Sn_F501_19X ) );
	memset ( Sn_F501_19D, '\0', sizeof ( Sn_F501_19D ) );
	memset ( Sn_F501_19E, '\0', sizeof ( Sn_F501_19E ) );
	memset ( Sn_F501_19F, '\0', sizeof ( Sn_F501_19F ) );

	Sn_F501_5kbn = 0;
	Sn_F501_UP1 = 0;
	Sn_F501_UP2 = 0;
	Sn_F501_UP3 = 0;
	Sn_F501_UP4 = 0;
	Sn_F501_UP5 = 0;
	Sn_F501_UP6 = 0;

	Sn_F501_6sw = 0;
	Sn_F501_14sw = 0;
	Sn_F501_15sw = 0;
	Sn_F501_16sw = 0;
	Sn_F501_17sw = 0;
	Sn_F501_18sw = 0;
	Sn_F501_19sw = 0;

	m_Arith.l_defn ( 0x16 );


	rsv = NULL;
}

CH31SnFhyo5_01Data::~CH31SnFhyo5_01Data()
{
	m_DBSyzAccess.CloseShLstVal();
	m_DBSyzAccess.CloseShLstRate();
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
int CH31SnFhyo5_01Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	CString filter;
	int		nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_5_01_HYOID, nowVersion );

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
		if ( pShLstVal->itmId == _T( "CTB00020" ) ) {
			m_Util.val_to_bin ( Sn_F501_1X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00030" ) ) {
			m_Util.val_to_bin ( Sn_F501_1D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00040" ) ) {
			m_Util.val_to_bin ( Sn_F501_1E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00050" ) ) {
			m_Util.val_to_bin ( Sn_F501_1F, pShLstVal->val );
		}
		// 貸倒回収に係る消費税額
		else if ( pShLstVal->itmId == _T( "CTB00070" ) ) {
			m_Util.val_to_bin ( Sn_F501_2X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00080" ) ) {
			m_Util.val_to_bin ( Sn_F501_2D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00090" ) ) {
			m_Util.val_to_bin ( Sn_F501_2E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00100" ) ) {
			m_Util.val_to_bin ( Sn_F501_2F, pShLstVal->val );
		}
		// 売上対価の返還等に係る消費税額
		else if ( pShLstVal->itmId == _T( "CTB00120" ) ) {
			m_Util.val_to_bin ( Sn_F501_3X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00130" ) ) {
			m_Util.val_to_bin ( Sn_F501_3D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00140" ) ) {
			m_Util.val_to_bin ( Sn_F501_3E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00150" ) ) {
			m_Util.val_to_bin ( Sn_F501_3F, pShLstVal->val );
		}
		// 控除対象仕入税額の計算の基礎となる消費税額
		else if ( pShLstVal->itmId == _T( "CTB00170" ) ) {
			m_Util.val_to_bin ( Sn_F501_4X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00180" ) ) {
			m_Util.val_to_bin ( Sn_F501_4D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00190" ) ) {
			m_Util.val_to_bin ( Sn_F501_4E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTB00200" ) ) {
			m_Util.val_to_bin ( Sn_F501_4F, pShLstVal->val );
		}
		// 4×みなし仕入率
		else if ( pShLstVal->itmId == _T( "CTC00010" ) ) {	// 区分
			Sn_F501_5kbn = atoi(pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTC00020" ) ) {
			m_Util.val_to_bin ( Sn_F501_5X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTC00030" ) ) {
			m_Util.val_to_bin ( Sn_F501_5D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTC00040" ) ) {
			m_Util.val_to_bin ( Sn_F501_5E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTC00050" ) ) {
			m_Util.val_to_bin ( Sn_F501_5F, pShLstVal->val );
		}
		// 事業区分別の合計額
		else if ( pShLstVal->itmId == _T( "CTD00030" ) ) {
			m_Util.val_to_bin ( Sn_F501_6X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00040" ) ) {
			m_Util.val_to_bin ( Sn_F501_6D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00050" ) ) {
			m_Util.val_to_bin ( Sn_F501_6E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00060" ) ) {
			m_Util.val_to_bin ( Sn_F501_6F, pShLstVal->val );
			Sn_F501_6sw = pShLstVal->sign1;
		}
		// 第一種事業(卸売業)
		else if ( pShLstVal->itmId == _T( "CTD00080" ) ) {
			m_Util.val_to_bin ( Sn_F501_7X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00090" ) ) {
			m_Util.val_to_bin ( Sn_F501_7D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00100" ) ) {
			m_Util.val_to_bin ( Sn_F501_7E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00120" ) ) {
			m_Util.val_to_bin ( Sn_F501_7F, pShLstVal->val );
		}

		// 第二種事業(小売業)
		else if ( pShLstVal->itmId == _T( "CTD00150" ) ) {
			m_Util.val_to_bin ( Sn_F501_8X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00160" ) ) {
			m_Util.val_to_bin ( Sn_F501_8D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00170" ) ) {
			m_Util.val_to_bin ( Sn_F501_8E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00190" ) ) {
			m_Util.val_to_bin ( Sn_F501_8F, pShLstVal->val );
		}
		// 第三種事業(製造業等)
		else if ( pShLstVal->itmId == _T( "CTD00220" ) ) {
			m_Util.val_to_bin ( Sn_F501_9X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00230" ) ) {
			m_Util.val_to_bin ( Sn_F501_9D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00240" ) ) {
			m_Util.val_to_bin ( Sn_F501_9E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00260" ) ) {
			m_Util.val_to_bin ( Sn_F501_9F, pShLstVal->val );
		}
		// 第四種事業(その他)
		else if ( pShLstVal->itmId == _T( "CTD00290" ) ) {
			m_Util.val_to_bin ( Sn_F501_10X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00300" ) ) {
			m_Util.val_to_bin ( Sn_F501_10D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00310" ) ) {
			m_Util.val_to_bin ( Sn_F501_10E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00330" ) ) {
			m_Util.val_to_bin ( Sn_F501_10F, pShLstVal->val );
		}
		// 第五種事業(サービス業等)
		else if ( pShLstVal->itmId == _T( "CTD00360" ) ) {
			m_Util.val_to_bin ( Sn_F501_11X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00370" ) ) {
			m_Util.val_to_bin ( Sn_F501_11D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00380" ) ) {
			m_Util.val_to_bin ( Sn_F501_11E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00400" ) ) {
			m_Util.val_to_bin ( Sn_F501_11F, pShLstVal->val );
		}
		// 第六種事業(不動産業)
		else if ( pShLstVal->itmId == _T( "CTD00430" ) ) {
			m_Util.val_to_bin ( Sn_F501_12X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00440" ) ) {
			m_Util.val_to_bin ( Sn_F501_12D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00450" ) ) {
			m_Util.val_to_bin ( Sn_F501_12E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00470" ) ) {
			m_Util.val_to_bin ( Sn_F501_12F, pShLstVal->val );
		}
		// 事業区分別の合計額
		else if ( pShLstVal->itmId == _T( "CTD00510" ) ) {
			m_Util.val_to_bin ( Sn_F501_13X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00520" ) ) {
			m_Util.val_to_bin ( Sn_F501_13D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00530" ) ) {
			m_Util.val_to_bin ( Sn_F501_13E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CTD00540" ) ) {
			m_Util.val_to_bin ( Sn_F501_13F, pShLstVal->val );
		}
		// 第一種事業(卸売業)
		else if ( pShLstVal->itmId == _T( "CTD00560" ) ) {
			m_Util.val_to_bin ( Sn_F501_14X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_14X) >= 0)
				memmove( Sn_F501_14XP, Sn_F501_14X, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_14XP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00570" ) ) {
			m_Util.val_to_bin ( Sn_F501_14D, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_14D) >= 0)
				memmove( Sn_F501_14DP, Sn_F501_14D, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_14DP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00580" ) ) {
			m_Util.val_to_bin ( Sn_F501_14E, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_14E) >= 0)
				memmove( Sn_F501_14EP, Sn_F501_14E, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_14EP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00590" ) ) {
			m_Util.val_to_bin ( Sn_F501_14F, pShLstVal->val );
			Sn_F501_14sw = pShLstVal->sign1;
		}
		// 第二種事業(小売業)
		else if ( pShLstVal->itmId == _T( "CTD00610" ) ) {
			m_Util.val_to_bin ( Sn_F501_15X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_15X) >= 0)
				memmove( Sn_F501_15XP, Sn_F501_15X, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_15XP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00620" ) ) {
			m_Util.val_to_bin ( Sn_F501_15D, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_15D) >= 0)
				memmove( Sn_F501_15DP, Sn_F501_15D, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_15DP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00630" ) ) {
			m_Util.val_to_bin ( Sn_F501_15E, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_15E) >= 0)
				memmove( Sn_F501_15EP, Sn_F501_15E, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_15EP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00640" ) ) {
			m_Util.val_to_bin ( Sn_F501_15F, pShLstVal->val );
			Sn_F501_15sw = pShLstVal->sign1;
		}
		// 第三種事業(製造業等)
		else if ( pShLstVal->itmId == _T( "CTD00660" ) ) {
			m_Util.val_to_bin ( Sn_F501_16X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_16X) >= 0)
				memmove( Sn_F501_16XP, Sn_F501_16X, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_16XP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00670" ) ) {
			m_Util.val_to_bin ( Sn_F501_16D, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_16D) >= 0)
				memmove( Sn_F501_16DP, Sn_F501_16D, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_16DP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00680" ) ) {
			m_Util.val_to_bin ( Sn_F501_16E, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_16E) >= 0)
				memmove( Sn_F501_16EP, Sn_F501_16E, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_16EP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00690" ) ) {
			m_Util.val_to_bin ( Sn_F501_16F, pShLstVal->val );
			Sn_F501_16sw = pShLstVal->sign1;
		}
		// 第四種事業(その他)
		else if ( pShLstVal->itmId == _T( "CTD00710" ) ) {
			m_Util.val_to_bin ( Sn_F501_17X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_17X) >= 0)
				memmove( Sn_F501_17XP, Sn_F501_17X, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_17XP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00720" ) ) {
			m_Util.val_to_bin ( Sn_F501_17D, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_17D) >= 0)
				memmove( Sn_F501_17DP, Sn_F501_17D, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_17DP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00730" ) ) {
			m_Util.val_to_bin ( Sn_F501_17E, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_17E) >= 0)
				memmove( Sn_F501_17EP, Sn_F501_17E, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_17EP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00740" ) ) {
			m_Util.val_to_bin ( Sn_F501_17F, pShLstVal->val );
			Sn_F501_17sw = pShLstVal->sign1;
		}
		// 第五種事業(サービス業等)
		else if ( pShLstVal->itmId == _T( "CTD00760" ) ) {
			m_Util.val_to_bin ( Sn_F501_18X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_18X) >= 0)
				memmove( Sn_F501_18XP, Sn_F501_18X, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_18XP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00770" ) ) {
			m_Util.val_to_bin ( Sn_F501_18D, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_18D) >= 0)
				memmove( Sn_F501_18DP, Sn_F501_18D, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_18DP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00780" ) ) {
			m_Util.val_to_bin ( Sn_F501_18E, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_18E) >= 0)
				memmove( Sn_F501_18EP, Sn_F501_18E, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_18EP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00790" ) ) {
			m_Util.val_to_bin ( Sn_F501_18F, pShLstVal->val );
			Sn_F501_18sw = pShLstVal->sign1;
		}
		// 第六種事業(不動産業)
		else if ( pShLstVal->itmId == _T( "CTD00810" ) ) {
			m_Util.val_to_bin ( Sn_F501_19X, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_19X) >= 0)
				memmove( Sn_F501_19XP, Sn_F501_19X, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_19XP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00820" ) ) {
			m_Util.val_to_bin ( Sn_F501_19D, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_19D) >= 0)
				memmove( Sn_F501_19DP, Sn_F501_19D, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_19DP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00830" ) ) {
			m_Util.val_to_bin ( Sn_F501_19E, pShLstVal->val );
			if (m_Arith.l_test(Sn_F501_19E) >= 0)
				memmove( Sn_F501_19EP, Sn_F501_19E, MONY_BUF_SIZE );
			else
				memset ( Sn_F501_19EP, '\0', MONY_BUF_SIZE );
		}
		else if ( pShLstVal->itmId == _T( "CTD00840" ) ) {
			m_Util.val_to_bin ( Sn_F501_19F, pShLstVal->val );
			Sn_F501_19sw = pShLstVal->sign1;
		}
	}

	// 特例計算適用サイン取得処理


	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_5_01_HYOID, nowVersion );

	if ( m_DBSyzAccess.m_pSH_LST_RATE ) {
		m_DBSyzAccess.m_pSH_LST_RATE->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstRate ( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_RATE *pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

	for (int i = 0;; i++) {
		if (i == 0) {
			if ((pShLstRate->MoveFirst() == -1) || (pShLstRate->st == -1)) {
				break;
			}
		}
		else {
			if ((pShLstRate->MoveNext() == -1) || (pShLstRate->st == -1)) {
				break;
			}
		}

		if (pShLstRate->itmId == _T("CTD00130")) {
			Sn_F501_UP1 = pShLstRate->rate;
		}
		else if (pShLstRate->itmId == _T("CTD00200")) {
			Sn_F501_UP2 = pShLstRate->rate;
		}
		else if (pShLstRate->itmId == _T("CTD00270")) {
			Sn_F501_UP3 = pShLstRate->rate;
		}
		else if (pShLstRate->itmId == _T("CTD00340")) {
			Sn_F501_UP4 = pShLstRate->rate;
		}
		else if (pShLstRate->itmId == _T("CTD00410")) {
			Sn_F501_UP5 = pShLstRate->rate;
		}
		else if (pShLstRate->itmId == _T("CTD00480")) {
			Sn_F501_UP6 = pShLstRate->rate;
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
int CH31SnFhyo5_01Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	CString filter;
	char *pHyoID = NULL;
	int nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_5_01_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_5_01_HYOID;

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
		pShLstVal->itmId = _T( "CTB00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 貸倒回収に係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00090" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00100" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 売上対価の返還等に係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00120" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00130" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00140" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00150" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 売上対価の返還等に係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00170" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00180" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00190" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTB00200" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 4×みなし仕入率
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTC00010" );
		pShLstVal->val.Format("%d", Sn_F501_5kbn);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTC00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTC00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTC00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTC00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 事業区分別の合計額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00060" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6F );
		pShLstVal->sign1 = Sn_F501_6sw;//21.03.02
//		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第一種事業(卸売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00090" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00100" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00120" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		/*
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00120");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_7E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00130");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_7F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		*/

		// 第二種事業(小売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00150" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00160" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00170" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00190" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		/*
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00190");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_8E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00200");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_8F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		*/

		// 第三種事業(製造業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00220" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00230" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00240" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00260" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		/*
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00260");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_9E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00270");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_9F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		*/

		// 第四種事業(その他)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00290" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00300" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00310" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00330" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		/*
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00330");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_10E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00340");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_10F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		*/

		// 第五種事業(サービス業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00360" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00370" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00380" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00400" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		/*
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00400");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_11E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00410");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_11F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		*/

		// 第六種事業(不動産業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00430" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00440" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00450" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00470" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		/*
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00470");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_12E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("CTD00480");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F501_12F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		*/

		// 事業区分別の合計額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00510" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00520" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00530" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00540" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 第一種事業(卸売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00560" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00570" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00580" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00590" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14F );
		//pShLstVal->sign1 = 0;
		pShLstVal->sign1 = Sn_F501_14sw;
		pShLstVal->Update();

		// 第二種事業(小売業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00610" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00620" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00630" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00640" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15F );
		//pShLstVal->sign1 = 0;
		pShLstVal->sign1 = Sn_F501_15sw;
		pShLstVal->Update();

		// 第三種事業(製造業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00660" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00670" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00680" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00690" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16F );
		//pShLstVal->sign1 = 0;
		pShLstVal->sign1 = Sn_F501_16sw;
		pShLstVal->Update();

		// 第四種事業(その他)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00710" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00720" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00730" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00740" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17F );
		//pShLstVal->sign1 = 0;
		pShLstVal->sign1 = Sn_F501_17sw;
		pShLstVal->Update();

		// 第五種事業(サービス業等)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00760" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00770" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00780" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00790" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18F );
		//pShLstVal->sign1 = 0;
		pShLstVal->sign1 = Sn_F501_18sw;
		pShLstVal->Update();

		// 第六種事業(不動産業)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00810" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00820" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00830" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CTD00840" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19F );
		//pShLstVal->sign1 = 0;
		pShLstVal->sign1 = Sn_F501_19sw;
		pShLstVal->Update();
	}
	else {
		CStringArray updTagAry;
		for ( int i = 0; ; i ++ ) {
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

			// 課税標準額に対する消費税
			if ( pShLstVal->itmId == _T( "CTB00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_1F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 貸倒回収に係る消費税額
			else if ( pShLstVal->itmId == _T( "CTB00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00090" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00100" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_2F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 売上対価の返還等に係る消費税額
			else if ( pShLstVal->itmId == _T( "CTB00120" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00130" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00140" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00150" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_3F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 控除対象仕入税額の基礎となる消費税額
			else if ( pShLstVal->itmId == _T( "CTB00170" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00180" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00190" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTB00200" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_4F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 4×みなし仕入率
			else if (pShLstVal->itmId == _T("CTC00010")) {
				pShLstVal->Edit();
				pShLstVal->val.Format("%d", Sn_F501_5kbn);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if ( pShLstVal->itmId == _T( "CTC00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTC00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTC00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTC00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_5F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 事業区分別の合計額
			else if ( pShLstVal->itmId == _T( "CTD00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00060" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_6F );
				pShLstVal->sign1 = Sn_F501_6sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第一種事業(卸売業)
			else if ( pShLstVal->itmId == _T( "CTD00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00090" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00100" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00120" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_7F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			/*
			else if (pShLstVal->itmId == _T("CTD00120")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_7E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CTD00130")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_7X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}*/
			// 第二種事業(小売業)
			else if ( pShLstVal->itmId == _T( "CTD00150" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00160" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00170" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00190" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_8F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			/*
			else if (pShLstVal->itmId == _T("CTD00190")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_8E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CTD00200")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_8X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			*/
			// 第三種事業(製造業等)
			else if ( pShLstVal->itmId == _T( "CTD00220" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00230" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00240" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00260" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_9F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			/*
			else if (pShLstVal->itmId == _T("CTD00260")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_9E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CTD00270")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_9X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			*/
			// 第四種事業(その他)
			else if ( pShLstVal->itmId == _T( "CTD00290" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00300" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00310" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00330" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_10F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			/*
			else if (pShLstVal->itmId == _T("CTD00330")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_10E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CTD00340")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_10X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			*/
			// 第五種事業(サービス業等)
			else if ( pShLstVal->itmId == _T( "CTD00360" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00370" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00380" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00400" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_11F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			/*
			else if (pShLstVal->itmId == _T("CTD00400")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_11E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CTD00410")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_11X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			*/
			// 第六種事業(不動産業)
			else if ( pShLstVal->itmId == _T( "CTD00430" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00440" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00450" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00470" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_12F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			/*
			else if (pShLstVal->itmId == _T("CTD00470")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_12E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CTD00480")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F501_12X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}*/
			
			// 事業区分別の合計額
			else if ( pShLstVal->itmId == _T( "CTD00510" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00520" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00530" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00540" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_13F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第一種事業(卸売業)
			else if ( pShLstVal->itmId == _T( "CTD00560" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00570" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00580" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00590" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_14F );
				pShLstVal->sign1 = Sn_F501_14sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第二種事業(小売業)
			else if ( pShLstVal->itmId == _T( "CTD00610" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00620" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00630" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00640" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_15F );
				pShLstVal->sign1 = Sn_F501_15sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第三種事業(製造業等)
			else if ( pShLstVal->itmId == _T( "CTD00660" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00670" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00680" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00690" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_16F );
				pShLstVal->sign1 = Sn_F501_16sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第四種事業(その他)
			else if ( pShLstVal->itmId == _T( "CTD00710" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00720" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00730" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00740" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_17F );
				pShLstVal->sign1 = Sn_F501_17sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第五種事業(サービス業等)
			else if ( pShLstVal->itmId == _T( "CTD00760" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00770" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00780" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00790" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_18F );
				pShLstVal->sign1 = Sn_F501_18sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// 第六種事業(不動産業)
			else if ( pShLstVal->itmId == _T( "CTD00810" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00820" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00830" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CTD00840" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F501_19F );
				pShLstVal->sign1 = Sn_F501_19sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
		}

		// 更新データ確認
		int adCnt = 0;
		for ( int i = 0; ; i++ ) {
			if ( strlen ( tagData501[i].tagName ) == 0 ) {
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
			MonyPtrAry.Add ( Sn_F501_1X );
			MonyPtrAry.Add ( Sn_F501_1D );
			MonyPtrAry.Add ( Sn_F501_1E );
			MonyPtrAry.Add ( Sn_F501_1F );
			MonyPtrAry.Add ( Sn_F501_2X );
			MonyPtrAry.Add ( Sn_F501_2D );
			MonyPtrAry.Add ( Sn_F501_2E );
			MonyPtrAry.Add ( Sn_F501_2F );
			MonyPtrAry.Add ( Sn_F501_3X );
			MonyPtrAry.Add ( Sn_F501_3D );
			MonyPtrAry.Add ( Sn_F501_3E );
			MonyPtrAry.Add ( Sn_F501_3F );
			MonyPtrAry.Add ( Sn_F501_4X );
			MonyPtrAry.Add ( Sn_F501_4D );
			MonyPtrAry.Add ( Sn_F501_4E );
			MonyPtrAry.Add ( Sn_F501_4F );
			_itoa_s(Sn_F501_5kbn, dmybuf, 10);
			m_Arith.l_input(aribuf, dmybuf);
			MonyPtrAry.Add ( aribuf );
			MonyPtrAry.Add ( Sn_F501_5X );
			MonyPtrAry.Add ( Sn_F501_5D );
			MonyPtrAry.Add ( Sn_F501_5E );
			MonyPtrAry.Add ( Sn_F501_5F );
			MonyPtrAry.Add ( Sn_F501_6X );
			MonyPtrAry.Add ( Sn_F501_6D );
			MonyPtrAry.Add ( Sn_F501_6E );
			MonyPtrAry.Add ( Sn_F501_6F );
			MonyPtrAry.Add ( Sn_F501_7X );
			MonyPtrAry.Add ( Sn_F501_7D );
			MonyPtrAry.Add ( Sn_F501_7E );
			MonyPtrAry.Add ( Sn_F501_7F );
			MonyPtrAry.Add ( Sn_F501_8X );
			MonyPtrAry.Add ( Sn_F501_8D );
			MonyPtrAry.Add ( Sn_F501_8E );
			MonyPtrAry.Add ( Sn_F501_8F );
			MonyPtrAry.Add ( Sn_F501_9X );
			MonyPtrAry.Add ( Sn_F501_9D );
			MonyPtrAry.Add ( Sn_F501_9E );
			MonyPtrAry.Add ( Sn_F501_9F );
			MonyPtrAry.Add ( Sn_F501_10X );
			MonyPtrAry.Add ( Sn_F501_10D );
			MonyPtrAry.Add ( Sn_F501_10E );
			MonyPtrAry.Add ( Sn_F501_10F );
			MonyPtrAry.Add ( Sn_F501_11X );
			MonyPtrAry.Add ( Sn_F501_11D );
			MonyPtrAry.Add ( Sn_F501_11E );
			MonyPtrAry.Add ( Sn_F501_11F );
			MonyPtrAry.Add ( Sn_F501_12X );
			MonyPtrAry.Add ( Sn_F501_12D );
			MonyPtrAry.Add ( Sn_F501_12E );
			MonyPtrAry.Add ( Sn_F501_12F );
			MonyPtrAry.Add ( Sn_F501_13X );
			MonyPtrAry.Add ( Sn_F501_13D );
			MonyPtrAry.Add ( Sn_F501_13E );
			MonyPtrAry.Add ( Sn_F501_13F );
			MonyPtrAry.Add ( Sn_F501_14X );
			MonyPtrAry.Add ( Sn_F501_14D );
			MonyPtrAry.Add ( Sn_F501_14E );
			MonyPtrAry.Add ( Sn_F501_14F );
			MonyPtrAry.Add ( Sn_F501_15X );
			MonyPtrAry.Add ( Sn_F501_15D );
			MonyPtrAry.Add ( Sn_F501_15E );
			MonyPtrAry.Add ( Sn_F501_15F );
			MonyPtrAry.Add ( Sn_F501_16X );
			MonyPtrAry.Add ( Sn_F501_16D );
			MonyPtrAry.Add ( Sn_F501_16E );
			MonyPtrAry.Add ( Sn_F501_16F );
			MonyPtrAry.Add ( Sn_F501_17X );
			MonyPtrAry.Add ( Sn_F501_17D );
			MonyPtrAry.Add ( Sn_F501_17E );
			MonyPtrAry.Add ( Sn_F501_17F );
			MonyPtrAry.Add ( Sn_F501_18X );
			MonyPtrAry.Add ( Sn_F501_18D );
			MonyPtrAry.Add ( Sn_F501_18E );
			MonyPtrAry.Add ( Sn_F501_18F );
			MonyPtrAry.Add ( Sn_F501_19X );
			MonyPtrAry.Add ( Sn_F501_19D );
			MonyPtrAry.Add ( Sn_F501_19E );
			MonyPtrAry.Add ( Sn_F501_19F );

			BOOL findFlg = FALSE;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagData501[i].tagName ) == 0 ) {
					break;
				}

				findFlg = FALSE;
				for ( int j = 0; j < updCnt; j++ ) {
					if ( updTagAry[j].CompareNoCase ( tagData501[i].tagName ) == 0 ) {
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
					pShLstVal->itmId = tagData501[i].tagName;
					m_Util.val_to_asci ( &pShLstVal->val, MonyPtrAry[i] );
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
				}
			}
		}
	}


	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_5_01_HYOID, nowVersion );

	if ( m_DBSyzAccess.m_pSH_LST_RATE ) {
		m_DBSyzAccess.m_pSH_LST_RATE->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstRate ( pDBNpSub->m_database, filter ) ) {
			return -1;
		
		}
	}
	CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

	if ( ( pShLstRate->MoveFirst() == -1 ) || ( pShLstRate->st == -1 ) ) {	// データ無し
																			// 課税売上割合
		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("CTD00130");
		pShLstRate->rate = Sn_F501_UP1;
		pShLstRate->sign1 = 0;
		pShLstRate->Update();

		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("CTD00200");
		pShLstRate->rate = Sn_F501_UP2;
		pShLstRate->sign1 = 0;
		pShLstRate->Update();

		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("CTD00270");
		pShLstRate->rate = Sn_F501_UP3;
		pShLstRate->sign1 = 0;
		pShLstRate->Update();

		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("CTD00340");
		pShLstRate->rate = Sn_F501_UP4;
		pShLstRate->sign1 = 0;
		pShLstRate->Update();

		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("CTD00410");
		pShLstRate->rate = Sn_F501_UP5;
		pShLstRate->sign1 = 0;
		pShLstRate->Update();

		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("CTD00480");
		pShLstRate->rate = Sn_F501_UP6;
		pShLstRate->sign1 = 0;
		pShLstRate->Update();
	}
	else{
		for ( int i = 0; ; i++ ) {
			if ( i == 0 ){
				if ( ( pShLstRate->MoveFirst() == -1 ) || ( pShLstRate->st == -1 ) ) {
					break;
				}
			}
			else {
				if ( ( pShLstRate->MoveNext() == -1 ) || ( pShLstRate->st == -1 ) ) {
					break;
				}
			}

			if (pShLstRate->itmId == _T("CTD00130")) {
				pShLstRate->Edit();
				pShLstRate->rate = Sn_F501_UP1;
				pShLstRate->Update();
			}
			else if (pShLstRate->itmId == _T("CTD00200")) {
				pShLstRate->Edit();
				pShLstRate->rate = Sn_F501_UP2;
				pShLstRate->Update();
			}
			else if (pShLstRate->itmId == _T("CTD00270")) {
				pShLstRate->Edit();
				pShLstRate->rate = Sn_F501_UP3;
				pShLstRate->Update();
			}
			else if (pShLstRate->itmId == _T("CTD00340")) {
				pShLstRate->Edit();
				pShLstRate->rate = Sn_F501_UP4;
				pShLstRate->Update();
			}
			else if (pShLstRate->itmId == _T("CTD00410")) {
				pShLstRate->Edit();
				pShLstRate->rate = Sn_F501_UP5;
				pShLstRate->Update();
			}
			else if (pShLstRate->itmId == _T("CTD00480")) {
				pShLstRate->Edit();
				pShLstRate->rate = Sn_F501_UP6;
				pShLstRate->Update();
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
int CH31SnFhyo5_01Data::SetCalqedData ( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei, CSnHeadData *pSnHeadData )
{
	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData ( _T( "35" ) );

	int cnt = ( int )money.GetCount();
	int incnt = 0;

	unsigned char exZeiFlg = 0x00;	// □□□□ □■■■
									// D0：旧税率
									// D1：税率6.24%
									// D2：税率7.8%
	
	int hanrendo = 0;
	if (!(pSnHeadData->Sn_Sign4 & 0x80) && ((pSnHeadData->m_DispTabSgn & 0x80) && (pSnHeadData->m_DispTabSgn & 0x100)))
		hanrendo = 1;	// 特例５－１　半連動
	
	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			// 課税標準額に対する消費税額
			if ( money[i][j].code.Left ( 4 ) == _T( "3571" ) ) {
				if ( money[i][j].code == _T( "357100" ) ) {
					memmove ( Sn_F501_1F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357101" ) ) {
					memmove ( Sn_F501_1X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357102" ) ) {
					memmove ( Sn_F501_1D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357103" ) ) {
					memmove ( Sn_F501_1E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 貸倒回収に係る消費税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3572" ) ) {
				if ( money[i][j].code == _T( "357200" ) ) {
					memmove ( Sn_F501_2F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357201" ) ) {
					memmove ( Sn_F501_2X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357202" ) ) {
					memmove ( Sn_F501_2D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357203" ) ) {
					memmove ( Sn_F501_2E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 売上対価の返還等に係る消費税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3573" ) ) {
				if ( money[i][j].code == _T( "357300" ) ) {
					memmove ( Sn_F501_3F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357301" ) ) {
					memmove ( Sn_F501_3X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357302" ) ) {
					memmove ( Sn_F501_3D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357303" ) ) {
					memmove ( Sn_F501_3E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 控除対象仕入税額の計算の基礎となる消費税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3574" ) ) {
				if ( money[i][j].code == _T( "357400" ) ) {
					memmove ( Sn_F501_4F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "354201" ) ) {
					memmove ( Sn_F501_4X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357402" ) ) {
					memmove ( Sn_F501_4D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357403" ) ) {
					memmove ( Sn_F501_4E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 1種類の事業の専業者の場合の控除対象仕入税額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3575" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "357500" ) ) {
					memmove ( Sn_F501_5F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357501" ) ) {
					memmove ( Sn_F501_5X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357502" ) ) {
					memmove ( Sn_F501_5D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357503" ) ) {
					memmove ( Sn_F501_5E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 事業区分別の合計額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3576" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "357600" ) ) {
					memmove ( Sn_F501_6F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357601" ) ) {
					memmove ( Sn_F501_6X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357602" ) ) {
					memmove ( Sn_F501_6D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357603" ) ) {
					memmove ( Sn_F501_6E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第一種事業(卸売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3577" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "357700" ) ) {
					memmove ( Sn_F501_7F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357701" ) ) {
					memmove ( Sn_F501_7X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357702" ) ) {
					memmove ( Sn_F501_7D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357703" ) ) {
					memmove ( Sn_F501_7E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第二種事業(小売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3578" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "357800" ) ) {
					memmove ( Sn_F501_8F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357801" ) ) {
					memmove ( Sn_F501_8X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357802" ) ) {
					memmove ( Sn_F501_8D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357803" ) ) {
					memmove ( Sn_F501_8E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第三種事業(製造業等)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3579" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "357900" ) ) {
					memmove ( Sn_F501_9F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "357901" ) ) {
					memmove ( Sn_F501_9X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "357902" ) ) {
					memmove ( Sn_F501_9D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "357903" ) ) {
					memmove ( Sn_F501_9E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第四種事業(その他)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3580" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358000" ) ) {
					memmove ( Sn_F501_10F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358001" ) ) {
					memmove ( Sn_F501_10X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358002" ) ) {
					memmove ( Sn_F501_10D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358003" ) ) {
					memmove ( Sn_F501_10E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第五種事業(サービス業等)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3581" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358100" ) ) {
					memmove ( Sn_F501_11F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358101" ) ) {
					memmove ( Sn_F501_11X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358102" ) ) {
					memmove ( Sn_F501_11D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358103" ) ) {
					memmove ( Sn_F501_11E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第六種事業(不動産業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3582" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358200" ) ) {
					memmove ( Sn_F501_12F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358201" ) ) {
					memmove ( Sn_F501_12X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358202" ) ) {
					memmove ( Sn_F501_12D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358203" ) ) {
					memmove ( Sn_F501_12E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 事業区分別の課税売上高に係る消費税額の明細
			// 事業区分別の合計額
			else if ( money[i][j].code.Left ( 4 ) == _T( "3583" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358300" ) ) {
					memmove ( Sn_F501_13F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358301" ) ) {
					memmove ( Sn_F501_13X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358302" ) ) {
					memmove ( Sn_F501_13D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358303" ) ) {
					memmove ( Sn_F501_13E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第一種事業(卸売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3584" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358400" ) ) {
					memmove ( Sn_F501_14F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358401" ) ) {
					memmove ( Sn_F501_14X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358402" ) ) {
					memmove ( Sn_F501_14D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358403" ) ) {
					memmove ( Sn_F501_14E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第二種事業(小売業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3585" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358500" ) ) {
					memmove ( Sn_F501_15F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358501" ) ) {
					memmove ( Sn_F501_15X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358502" ) ) {
					memmove ( Sn_F501_15D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358503" ) ) {
					memmove ( Sn_F501_15E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第三種事業(製造業等)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3586" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358600" ) ) {
					memmove ( Sn_F501_16F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358601" ) ) {
					memmove ( Sn_F501_16X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358602" ) ) {
					memmove ( Sn_F501_16D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358603" ) ) {
					memmove ( Sn_F501_16E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第四種事業(その他)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3587" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358700" ) ) {
					memmove ( Sn_F501_17F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358701" ) ) {
					memmove ( Sn_F501_17X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358702" ) ) {
					memmove ( Sn_F501_17D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358703" ) ) {
					memmove ( Sn_F501_17E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第五種事業(サービス業等)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3588" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358800" ) ) {
					memmove ( Sn_F501_18F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358801" ) ) {
					memmove ( Sn_F501_18X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358802" ) ) {
					memmove ( Sn_F501_18D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358803" ) ) {
					memmove ( Sn_F501_18E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
					}
				}
			}
			// 第六種事業(不動産業)
			else if ( money[i][j].code.Left ( 4 ) == _T( "3589" ) ) {
				if ( hanrendo ) continue;
				if ( money[i][j].code == _T( "358900" ) ) {
					memmove ( Sn_F501_19F, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "358901" ) ) {
					memmove ( Sn_F501_19X, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "358902" ) ) {
					memmove ( Sn_F501_19D, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x02;
					}
				}
				else if ( money[i][j].code == _T( "358903" ) ) {
					memmove ( Sn_F501_19E, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x04;
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
int CH31SnFhyo5_01Data::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{

	m_Arith.l_clear(Sn_F501_1X);
	m_Arith.l_clear(Sn_F501_1D);
	m_Arith.l_clear(Sn_F501_1E);
	m_Arith.l_clear(Sn_F501_1F);

	m_Arith.l_clear(Sn_F501_2X);
	m_Arith.l_clear(Sn_F501_2D);
	m_Arith.l_clear(Sn_F501_2E);
	m_Arith.l_clear(Sn_F501_2F);

	m_Arith.l_clear(Sn_F501_3X);
	m_Arith.l_clear(Sn_F501_3D);
	m_Arith.l_clear(Sn_F501_3E);
	m_Arith.l_clear(Sn_F501_3F);

	m_Arith.l_clear(Sn_F501_4X);
	m_Arith.l_clear(Sn_F501_4D);
	m_Arith.l_clear(Sn_F501_4E);
	m_Arith.l_clear(Sn_F501_4F);

	m_Arith.l_clear(Sn_F501_5X);
	m_Arith.l_clear(Sn_F501_5D);
	m_Arith.l_clear(Sn_F501_5E);
	m_Arith.l_clear(Sn_F501_5F);

	m_Arith.l_clear(Sn_F501_6X);
	m_Arith.l_clear(Sn_F501_6D);
	m_Arith.l_clear(Sn_F501_6E);
	m_Arith.l_clear(Sn_F501_6F);

	m_Arith.l_clear(Sn_F501_7X);
	m_Arith.l_clear(Sn_F501_7D);
	m_Arith.l_clear(Sn_F501_7E);
	m_Arith.l_clear(Sn_F501_7F);

	m_Arith.l_clear(Sn_F501_8X);
	m_Arith.l_clear(Sn_F501_8D);
	m_Arith.l_clear(Sn_F501_8E);
	m_Arith.l_clear(Sn_F501_8F);

	m_Arith.l_clear(Sn_F501_9X);
	m_Arith.l_clear(Sn_F501_9D);
	m_Arith.l_clear(Sn_F501_9E);
	m_Arith.l_clear(Sn_F501_9F);

	m_Arith.l_clear(Sn_F501_10X);
	m_Arith.l_clear(Sn_F501_10D);
	m_Arith.l_clear(Sn_F501_10E);
	m_Arith.l_clear(Sn_F501_10F);

	m_Arith.l_clear(Sn_F501_11X);
	m_Arith.l_clear(Sn_F501_11D);
	m_Arith.l_clear(Sn_F501_11E);
	m_Arith.l_clear(Sn_F501_11F);

	m_Arith.l_clear(Sn_F501_12X);
	m_Arith.l_clear(Sn_F501_12D);
	m_Arith.l_clear(Sn_F501_12E);
	m_Arith.l_clear(Sn_F501_12F);

	m_Arith.l_clear(Sn_F501_13X);
	m_Arith.l_clear(Sn_F501_13D);
	m_Arith.l_clear(Sn_F501_13E);
	m_Arith.l_clear(Sn_F501_13F);

	m_Arith.l_clear(Sn_F501_14X);
	m_Arith.l_clear(Sn_F501_14D);
	m_Arith.l_clear(Sn_F501_14E);
	m_Arith.l_clear(Sn_F501_14F);

	m_Arith.l_clear(Sn_F501_15X);
	m_Arith.l_clear(Sn_F501_15D);
	m_Arith.l_clear(Sn_F501_15E);
	m_Arith.l_clear(Sn_F501_15F);

	m_Arith.l_clear(Sn_F501_16X);
	m_Arith.l_clear(Sn_F501_16D);
	m_Arith.l_clear(Sn_F501_16E);
	m_Arith.l_clear(Sn_F501_16F);

	m_Arith.l_clear(Sn_F501_17X);
	m_Arith.l_clear(Sn_F501_17D);
	m_Arith.l_clear(Sn_F501_17E);
	m_Arith.l_clear(Sn_F501_17F);

	m_Arith.l_clear(Sn_F501_18X);
	m_Arith.l_clear(Sn_F501_18D);
	m_Arith.l_clear(Sn_F501_18E);
	m_Arith.l_clear(Sn_F501_18F);

	m_Arith.l_clear(Sn_F501_19X);
	m_Arith.l_clear(Sn_F501_19D);
	m_Arith.l_clear(Sn_F501_19E);
	m_Arith.l_clear(Sn_F501_19F);

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
int CH31SnFhyo5_01Data::ClearManualInputSign(BOOL isChgKzritu)
{

	Sn_F501_6sw = 0;

	//---->19.10.23
	//連動から非連動切替時、手入力ONにする処理を追加する為、コメント化
//	Sn_F501_14sw = 0;
//	Sn_F501_15sw = 0;
//	Sn_F501_16sw = 0;
//	Sn_F501_17sw = 0;
//	Sn_F501_18sw = 0;
//	Sn_F501_19sw = 0;
	//<--------------

	/*if (isChgKzritu) {
		Sn_F501_6sw |= 0x888;
		Sn_F501_14sw |= 0x888;
		Sn_F501_15sw |= 0x888;
		Sn_F501_16sw |= 0x888;
		Sn_F501_17sw |= 0x888;
		Sn_F501_18sw |= 0x888;
		Sn_F501_19sw |= 0x888;
	}
	*/
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
bool CH31SnFhyo5_01Data::IsExistData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ) {
		return false;
	}

	int nowVersion = 2;
	CString filter;
	if( sn_seq != -1 ){
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_5_01_HYOID, nowVersion);
	}
	else{
		filter.Format(_T("hyoId = \'%s\' and version = %d"), ICS_SN_FHYO_5_01_HYOID, nowVersion);
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

