#include "StdAfx.h"
#include "H27SnHonpyoData.h"

// 簡易課税
static SH_TAGNAME	tagDataKaniVal_27[] = {
	{_T("ABE00150"), 999 },
	{_T("") }
};

static SH_TAGNAME	tagDataKaniRate_27[] = {
	{_T("ABE00160"), 999 },
	{_T("") }
};

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH27SnHonpyoData::CH27SnHonpyoData()
	: CH26SnHonpyoData()
{
	memset( Sn_KURI6, '\0', sizeof(Sn_KURI6) );
	Sn_UP6 = 0;

	memset( rsv3, '\0', sizeof(rsv3) );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH27SnHonpyoData::~CH27SnHonpyoData()
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
int CH27SnHonpyoData::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH26SnHonpyoData::GetData(pDBNpSub, sn_seq) ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}
	else if( KzHoushiki == ID_ICSSH_GENSOKU ){
		// 原則課税では変更無し
		return 0;
	}

	// 金額
	CString	filter;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );

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
		if( pShLstVal->itmId == _T("ABE00150") ){		// 第６種(売上高)
			m_Util.val_to_bin( Sn_KURI6, pShLstVal->val );
		}
	}

	// 割合
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );

	if( m_DBSyzAccess.m_pSH_LST_RATE ){
		m_DBSyzAccess.m_pSH_LST_RATE->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}
	CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pShLstRate->MoveFirst()==-1) || (pShLstRate->st==-1) ){
				break;
			}
		}
		else{
			if( (pShLstRate->MoveNext()==-1) || (pShLstRate->st==-1) ){
				break;
			}
		}
		if( pShLstRate->itmId == _T("ABE00160") ){
			Sn_UP6 = pShLstRate->rate;
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
int CH27SnHonpyoData::SetData( CDBNpSub *pDBNpSub, int sn_seq, int wrtype/*=0*/ )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータのセット
	if( CH26SnHonpyoData::SetData(pDBNpSub, sn_seq, wrtype) ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}
	else if( KzHoushiki == ID_ICSSH_GENSOKU ){
		// 原則課税では変更無し
		return 0;
	}

	// 金額
	CString	filter;
	int		nowVersion = 1;
	char	*pHyoID = NULL;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );
	pHyoID = ICS_SN_KANNI_HONPYO_HYOID;

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

		// 第６種
		if( pShLstVal->itmId == _T("ABE00150") ){
			pShLstVal->Edit();
			m_Util.val_to_asci( &pShLstVal->val, Sn_KURI6 );
			pShLstVal->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
	}

	// 更新データ確認
	int	adCnt = 0;
	for( int i=0;; i++ ){
		if( strlen(tagDataKaniVal_27[i].tagName) == 0 ){
			break;
		}
		adCnt++;
	}
	int updCnt = (int)updTagAry.GetCount();
	if( adCnt != updCnt ){
		// 無いものを追加
		CArray<unsigned char*>	MonyPtrAry;
		MonyPtrAry.Add( Sn_KURI6 );
		
		int	aryMax = (int)MonyPtrAry.GetCount();
		BOOL findFlg = FALSE;
		for( int i=0;; i++ ){
			if( strlen(tagDataKaniVal_27[i].tagName) == 0 ){
				break;
			}

			findFlg = FALSE;
			for( int j=0; j<updCnt; j++ ){
				if( updTagAry[j].CompareNoCase(tagDataKaniVal_27[i].tagName) == 0 ){
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
					pShLstVal->itmId = tagDataKaniVal_27[i].tagName;
					m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
					pShLstVal->Update();
				}
			}
		}
	}

	// 割合
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );

	if( m_DBSyzAccess.m_pSH_LST_RATE ){
		m_DBSyzAccess.m_pSH_LST_RATE->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;
	updTagAry.RemoveAll();
	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pShLstRate->MoveFirst()==-1) || (pShLstRate->st==-1) ){
				break;
			}
		}
		else{
			if( (pShLstRate->MoveNext()==-1) || (pShLstRate->st==-1) ){
				break;
			}
		}

		// 第６種
		if( pShLstRate->itmId == _T("ABE00160") ){
			pShLstRate->Edit();
			pShLstRate->rate = Sn_UP6;
			pShLstRate->Update();
			updTagAry.Add( pShLstVal->itmId );
		}
	}
	// 更新データ確認
	adCnt = 0;
	for( int i=0;; i++ ){
		if( strlen(tagDataKaniRate_27[i].tagName) == 0 ){
			break;
		}
		adCnt++;
	}
	updCnt = (int)updTagAry.GetCount();
	if( adCnt != updCnt ){
		// 無いものを追加
		CArray<unsigned short>	RateAry;
		RateAry.Add( Sn_UP6 );

		int	aryMax = (int)RateAry.GetCount();

		BOOL findFlg = FALSE;
		for( int i=0;; i++ ){
			if( strlen(tagDataKaniRate_27[i].tagName) == 0 ){
				break;
			}

			findFlg = FALSE;
			for( int j=0; j<updCnt; j++ ){
				if( updTagAry[j].CompareNoCase(tagDataKaniRate_27[i].tagName) == 0 ){
					findFlg = TRUE;
					break;
				}
			}
			if( findFlg == FALSE ){
				// 該当タグの追加
				if( i < aryMax ){
					pShLstRate->AddNew();
					pShLstRate->sn_seq = sn_seq;
					pShLstRate->version = nowVersion;
					pShLstRate->hyoId = pHyoID;
					pShLstRate->itmId = _T("ABE00160");
					pShLstRate->rate = RateAry[i];
					pShLstRate->sign1 = 0;
					pShLstRate->Update();
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
int CH27SnHonpyoData::SetCalqedData( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei )
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
	if( CH26SnHonpyoData::SetCalqedData(pDBNpSub, pSyzSyukei) ){
		return -1;
	}

	// 実際には、データ連動が必要！

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
int CH27SnHonpyoData::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでの更新時の金額クリア
	if( CH26SnHonpyoData::ClearDataByRenew(pDBNpSub, type) ){
		return -1;
	}

	memset( Sn_KURI6, '\0', sizeof(Sn_KURI6) );
	Sn_UP6 = 0;

	return 0;
}

