#include "StdAfx.h"
#include "H28SnHonpyoData.h"

// 原則課税
static SH_TAGNAME	tagDataIppanVal_28[] = {
	{_T("AAH00010"), 1 },
	{_T("AAH00020"), 0 },
	{_T("AAH00030"), 3 },
	{_T("AAH00040"), 999 },
	{_T("AAI00010"), 3 },
	{_T("AAI00020"), 999 },
	{_T("AAJ00010"), 999 },
	{_T("AAK00010"), 999 },
	{_T("") }
};

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH28SnHonpyoData::CH28SnHonpyoData()
	: CH27SnHonpyoData()
{
	memset( Sn_KSJT3, '\0', sizeof(Sn_KSJT3) );
	memset( Sn_KSJT4, '\0', sizeof(Sn_KSJT4) );
	memset( Sn_KSJT63, '\0', sizeof(Sn_KSJT63) );
	memset( Sn_KSJTKEI, '\0', sizeof(Sn_KSJTKEI) );
	memset( Sn_TKST63, '\0', sizeof(Sn_TKST63) );
	memset( Sn_TKSTKEI, '\0', sizeof(Sn_TKSTKEI) );
	memset( Sn_URHKNZ, '\0', sizeof(Sn_URHKNZ) );
	memset( Sn_TKHKNZ, '\0', sizeof(Sn_TKHKNZ) );

	memset( rsv4, '\0', sizeof(rsv4) );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH28SnHonpyoData::~CH28SnHonpyoData()
{
	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.CloseShLstVal();
	}
	if( m_DBSyzAccess.m_pSH_LST_RATE ){
		m_DBSyzAccess.CloseShLstRate();
	}
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
int CH28SnHonpyoData::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH27SnHonpyoData::GetData(pDBNpSub, sn_seq) ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		return 0;
	}

	// 金額
	CString	filter;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion );

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
		// 金額を変換して代入
		if( pShLstVal->itmId == _T("AAH00010") ){			// 課税資産の譲渡等の対価の額(3%)
			m_Util.val_to_bin( Sn_KSJT3, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAH00020") ){		// 課税資産の譲渡等の対価の額(4%)
			m_Util.val_to_bin( Sn_KSJT4, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAH00030") ){		// 課税資産の譲渡等の対価の額(6.3%)
			m_Util.val_to_bin( Sn_KSJT63, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAH00040") ){		// 課税資産の譲渡等の対価の額(合計)
			m_Util.val_to_bin( Sn_KSJTKEI, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAI00010") ){		// 特定課税仕入に係る支払対価の額(6.3%)
			m_Util.val_to_bin( Sn_TKST63, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAI00020") ){		// 特定課税仕入に係る支払対価の額(合計)
			m_Util.val_to_bin( Sn_TKSTKEI, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAJ00010") ){		// 売上の返還等対価に係る税額
			m_Util.val_to_bin( Sn_URHKNZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAK00010") ){		// 特定課税仕入の返還等対価に係る税額
			m_Util.val_to_bin( Sn_TKHKNZ, pShLstVal->val );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DBへデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		sn_seq		：	対応の履歴シーケンス
//		wrtype		：	書込みタイプ	□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□■
//						 D0：szsnk へデータを書き込まない
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH28SnHonpyoData::SetData( CDBNpSub *pDBNpSub, int sn_seq, int wrtype/*=0*/ )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータのセット
	if( CH27SnHonpyoData::SetData(pDBNpSub, sn_seq, wrtype) ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		return 0;
	}

	// 金額
	CString	filter;
	char	*pHyoID = NULL;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion );
	pHyoID = ICS_SN_IPPAN_HONPYO_HYOID;

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	BOOL			isNodata = TRUE;
	
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

		// 課税資産の譲渡等の対価の額
		if( pShLstVal->itmId == _T("AAH00010") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSJT3 );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
		else if( pShLstVal->itmId == _T("AAH00020") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSJT4 );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
		else if( pShLstVal->itmId == _T("AAH00030") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSJT63 );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
		else if( pShLstVal->itmId == _T("AAH00040") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSJTKEI );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
		// 特定課税仕入に係る支払対価の額
		else if( pShLstVal->itmId == _T("AAI00010") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_TKST63 );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
		else if( pShLstVal->itmId == _T("AAI00020") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_TKSTKEI );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
		// 売上の返還等対価に係る税額
		else if( pShLstVal->itmId == _T("AAJ00010") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_URHKNZ );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
		// 特定課税仕入の返還等対価に係る税額
		else if( pShLstVal->itmId == _T("AAK00010") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_TKHKNZ );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
	}

	// 更新データ確認
	int	adCnt = 0;
	for( int i=0;; i++ ){
		if( strlen(tagDataIppanVal_28[i].tagName) == 0 ){
			break;
		}
		adCnt++;
	}
	int updCnt = (int)updTagAry.GetCount();
	if( adCnt != updCnt ){
		// 無いものを追加
		CArray<unsigned char*>	MonyPtrAry;
		MonyPtrAry.Add( Sn_KSJT3 );
		MonyPtrAry.Add( Sn_KSJT4 );
		MonyPtrAry.Add( Sn_KSJT63 );
		MonyPtrAry.Add( Sn_KSJTKEI );
		MonyPtrAry.Add( Sn_TKST63 );
		MonyPtrAry.Add( Sn_TKSTKEI );
		MonyPtrAry.Add( Sn_URHKNZ );
		MonyPtrAry.Add( Sn_TKHKNZ );
		
		int	aryMax = (int)MonyPtrAry.GetCount();
		BOOL findFlg = FALSE;
		for( int i=0;; i++ ){
			if( strlen(tagDataIppanVal_28[i].tagName) == 0 ){
				break;
			}

			findFlg = FALSE;
			for( int j=0; j<updCnt; j++ ){
				if( updTagAry[j].CompareNoCase(tagDataIppanVal_28[i].tagName) == 0 ){
					findFlg = TRUE;
					break;
				}
			}
			if( findFlg == FALSE ){
				// 該当タグの追加
				if( i < aryMax ){
					pShLstVal->AddNew();
					pShLstVal->sn_seq = sn_seq;
					pShLstVal->version = nowVersion;
					pShLstVal->hyoId = pHyoID;
					pShLstVal->itmId = tagDataIppanVal_28[i].tagName;
					m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
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
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH28SnHonpyoData::SetCalqedData( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pSyzSyukei );
	if( pSyzSyukei == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH27SnHonpyoData::SetCalqedData(pDBNpSub, pSyzSyukei) ){
		return -1;
	}

	MoneyBasejagArray money;

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		return 0;
	}

	money = pSyzSyukei->GetShinkokuData( _T("23") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code == _T("230701") ){
				memmove( Sn_KSJT3, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code == _T("230702") ){
				memmove( Sn_KSJT4, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code == _T("230703") ){
				memmove( Sn_KSJT63, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code == _T("230700") ){
				memmove( Sn_KSJTKEI, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code == _T("230803") ){
				memmove( Sn_TKST63, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code == _T("230800") ){
				memmove( Sn_TKSTKEI, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code == _T("231000") ){
				memmove( Sn_URHKNZ, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code == _T("231100") ){
				memmove( Sn_TKHKNZ, money[i][j].arith, MONY_BUF_SIZE );
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 更新時の金額クリア
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		type		：	クリア種別
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH28SnHonpyoData::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでの更新時の金額クリア
	if( CH27SnHonpyoData::ClearDataByRenew(pDBNpSub, type) ){
		return -1;
	}

	memset( Sn_KSJT3, '\0', sizeof(Sn_KSJT3) );
	memset( Sn_KSJT4, '\0', sizeof(Sn_KSJT4) );
	memset( Sn_KSJT63, '\0', sizeof(Sn_KSJT63) );
	memset( Sn_KSJTKEI, '\0', sizeof(Sn_KSJTKEI) );
	memset( Sn_TKST63, '\0', sizeof(Sn_TKST63) );
	memset( Sn_TKSTKEI, '\0', sizeof(Sn_TKSTKEI) );
	memset( Sn_URHKNZ, '\0', sizeof(Sn_URHKNZ) );
	memset( Sn_TKHKNZ, '\0', sizeof(Sn_TKHKNZ) );

	return 0;
}

