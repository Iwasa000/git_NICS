#include "StdAfx.h"
#include "H28SnFhyo20Data.h"

static SH_TAGNAME	tagData20_28[] = {
	{_T("AEC00270"), 3 },
	{_T("AEC00300"), 3 },
	{_T("") }
};


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH28SnFhyo20Data::CH28SnFhyo20Data( BOOL isTransitionalMeasure )
	: CH26SnFhyo20Data(isTransitionalMeasure)
	, m_isTransitionalMeasure(isTransitionalMeasure)
{

	memset( Sn_2F10C, '\0', sizeof(Sn_2F10C) );

	memset( Sn_2F11C, '\0', sizeof(Sn_2F11C) );
	Sn_2F11sw = 0;

	memset( Sn_2F12A, '\0', sizeof(Sn_2F12A) );
	memset( Sn_2F12B, '\0', sizeof(Sn_2F12B) );
	memset( Sn_2F12C, '\0', sizeof(Sn_2F12C) );

	memset( Sn_2F13A, '\0', sizeof(Sn_2F13A) );
	memset( Sn_2F13B, '\0', sizeof(Sn_2F13B) );
	memset( Sn_2F13C, '\0', sizeof(Sn_2F13C) );

	memset( Sn_2F16A, '\0', sizeof(Sn_2F16A) );
	memset( Sn_2F16B, '\0', sizeof(Sn_2F16B) );
	memset( Sn_2F16C, '\0', sizeof(Sn_2F16C) );

	memset( Sn_2F17A, '\0', sizeof(Sn_2F17A) );
	memset( Sn_2F17B, '\0', sizeof(Sn_2F17B) );
	memset( Sn_2F17C, '\0', sizeof(Sn_2F17C) );

	memset( Sn_2F18A, '\0', sizeof(Sn_2F18A) );
	memset( Sn_2F18B, '\0', sizeof(Sn_2F18B) );
	memset( Sn_2F18C, '\0', sizeof(Sn_2F18C) );

	memset( Sn_2F19A, '\0', sizeof(Sn_2F19A) );
	memset( Sn_2F19B, '\0', sizeof(Sn_2F19B) );
	memset( Sn_2F19C, '\0', sizeof(Sn_2F19C) );

	memset( Sn_2F20A, '\0', sizeof(Sn_2F20A) );
	memset( Sn_2F20B, '\0', sizeof(Sn_2F20B) );
	memset( Sn_2F20C, '\0', sizeof(Sn_2F20C) );

	memset( Sn_2F21A, '\0', sizeof(Sn_2F21A) );
	memset( Sn_2F21B, '\0', sizeof(Sn_2F21B) );
	memset( Sn_2F21C, '\0', sizeof(Sn_2F21C) );

	memset( Sn_2F22A, '\0', sizeof(Sn_2F22A) );
	memset( Sn_2F22B, '\0', sizeof(Sn_2F22B) );
	memset( Sn_2F22C, '\0', sizeof(Sn_2F22C) );
	Sn_2F22sw = 0;

	memset( Sn_2F24A, '\0', sizeof(Sn_2F24A) );
	memset( Sn_2F24B, '\0', sizeof(Sn_2F24B) );
	memset( Sn_2F24C, '\0', sizeof(Sn_2F24C) );

	m_Arith.l_defn( 0x16 );

	memset( rsv1, '\0', sizeof(rsv1) );
	memset( rsv2, '\0', sizeof(rsv2) );

}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH28SnFhyo20Data::~CH28SnFhyo20Data()
{
	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.CloseShLstVal();
	}
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
int CH28SnFhyo20Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{

	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH26SnFhyo20Data::GetData(pDBNpSub, sn_seq) ){
		return -1;
	}

	// データのコピー

	// 課税貨物に係る支払対価の額 Sn_2F12A <- Sn_2F10A
	memmove( Sn_2F12A, CH26SnFhyo20Data::Sn_2F10A, sizeof(Sn_2F12A) );
	memmove( Sn_2F12B, CH26SnFhyo20Data::Sn_2F10B, sizeof(Sn_2F12B) );
	memmove( Sn_2F12C, CH26SnFhyo20Data::Sn_2F10C, sizeof(Sn_2F12C) );

	// 納税義務の免除を受けない（受ける）こととなった場合における消費税額の調整額  Sn_2F13A <- Sn_2F11A
	memmove( Sn_2F13A, CH26SnFhyo20Data::Sn_2F11A, sizeof(Sn_2F13A) );
	memmove( Sn_2F13B, CH26SnFhyo20Data::Sn_2F11B, sizeof(Sn_2F13B) );
	memmove( Sn_2F13C, CH26SnFhyo20Data::Sn_2F11C, sizeof(Sn_2F13C) );

	// 個別対応：課税売上にのみ要するもの  Sn_2F16A <- Sn_2F15A
	memmove( Sn_2F16A, CH26SnFhyo20Data::Sn_2F15A, sizeof(Sn_2F16A) );
	memmove( Sn_2F16B, CH26SnFhyo20Data::Sn_2F15B, sizeof(Sn_2F16B) );
	memmove( Sn_2F16C, CH26SnFhyo20Data::Sn_2F15C, sizeof(Sn_2F16C) );

	// 個別対応：共通の売上に係る Sn_2F17A <- Sn_2F16A
	memmove( Sn_2F17A, CH26SnFhyo20Data::Sn_2F16A, sizeof(Sn_2F17A) );
	memmove( Sn_2F17B, CH26SnFhyo20Data::Sn_2F16B, sizeof(Sn_2F17B) );
	memmove( Sn_2F17C, CH26SnFhyo20Data::Sn_2F16C, sizeof(Sn_2F17C) );

	// 個別対応：控除する課税仕入れ等の税額 Sn_2F18A <- Sn_2F17A
	memmove( Sn_2F18A, CH26SnFhyo20Data::Sn_2F17A, sizeof(Sn_2F18A) );
	memmove( Sn_2F18B, CH26SnFhyo20Data::Sn_2F17B, sizeof(Sn_2F18B) );
	memmove( Sn_2F18C, CH26SnFhyo20Data::Sn_2F17C, sizeof(Sn_2F18C) );

	// 比例配分：控除する課税仕入れ等の税額 Sn_2F19A <- Sn_2F18A
	memmove( Sn_2F19A, CH26SnFhyo20Data::Sn_2F18A, sizeof(Sn_2F19A) );
	memmove( Sn_2F19B, CH26SnFhyo20Data::Sn_2F18B, sizeof(Sn_2F19B) );
	memmove( Sn_2F19C, CH26SnFhyo20Data::Sn_2F18C, sizeof(Sn_2F19C) );

	// 控除税額の調整 Sn_2F20A <- Sn_2F19A
	memmove( Sn_2F20A, CH26SnFhyo20Data::Sn_2F19A, sizeof(Sn_2F20A) );
	memmove( Sn_2F20B, CH26SnFhyo20Data::Sn_2F19B, sizeof(Sn_2F20B) );
	memmove( Sn_2F20C, CH26SnFhyo20Data::Sn_2F19C, sizeof(Sn_2F20C) );

	// 控除税額の調整 Sn_2F21A <- Sn_2F20A
	memmove( Sn_2F21A, CH26SnFhyo20Data::Sn_2F20A, sizeof(Sn_2F21A) );
	memmove( Sn_2F21B, CH26SnFhyo20Data::Sn_2F20B, sizeof(Sn_2F21B) );
	memmove( Sn_2F21C, CH26SnFhyo20Data::Sn_2F20C, sizeof(Sn_2F21C) );

	// 付表2｢22｣- 控除対象仕入税額手入力額 Sn_2F22A <- Sn_2F21A
	memmove( Sn_2F22A, CH26SnFhyo20Data::Sn_2F21A, sizeof(Sn_2F22A) );
	memmove( Sn_2F22B, CH26SnFhyo20Data::Sn_2F21B, sizeof(Sn_2F22B) );
	memmove( Sn_2F22C, CH26SnFhyo20Data::Sn_2F21C, sizeof(Sn_2F22C) );
	Sn_2F22sw = CH26SnFhyo20Data::Sn_2F21sw;// 控除対象仕入税額　手入力サイン D0: D4:

	// 貸倒回収に係る消費税額 Sn_2F24A <- Sn_2F23A
	memmove( Sn_2F24A, CH26SnFhyo20Data::Sn_2F23A, sizeof(Sn_2F24A) );
	memmove( Sn_2F24B, CH26SnFhyo20Data::Sn_2F23B, sizeof(Sn_2F24B) );
	memmove( Sn_2F24C, CH26SnFhyo20Data::Sn_2F23C, sizeof(Sn_2F24C) );


	CString	filter;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );

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

		//特定課税仕入れに係る支払対価の額
		if( pShLstVal->itmId == _T("AEC00270") ){
			m_Util.val_to_bin( Sn_2F10C, pShLstVal->val );
		}

		//特定課税仕入れに係る消費税額
		if( pShLstVal->itmId == _T("AEC00300") ){
			m_Util.val_to_bin( Sn_2F11C, pShLstVal->val );
			Sn_2F11sw = pShLstVal->sign1;
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
int CH28SnFhyo20Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{

	ASSERT( pDBNpSub );

	if( pDBNpSub == NULL ){
		return -1;
	}

	
	//----> データのコピー(CH26SnFhyo20Dataを呼ぶ前に前の場所にデータをセットする)
	// 課税貨物に係る支払対価の額 Sn_2F12A -> Sn_2F10A
	memmove( CH26SnFhyo20Data::Sn_2F10A, Sn_2F12A , sizeof(Sn_2F12A) );
	memmove( CH26SnFhyo20Data::Sn_2F10B, Sn_2F12B , sizeof(Sn_2F12B) );
	memmove( CH26SnFhyo20Data::Sn_2F10C, Sn_2F12C,  sizeof(Sn_2F12C) );

	// 納税義務の免除を受けない（受ける）こととなった場合における消費税額の調整額  Sn_2F13A -> Sn_2F11A
	memmove( CH26SnFhyo20Data::Sn_2F11A, Sn_2F13A, sizeof(Sn_2F13A) );
	memmove( CH26SnFhyo20Data::Sn_2F11B, Sn_2F13B, sizeof(Sn_2F13B) );
	memmove( CH26SnFhyo20Data::Sn_2F11C, Sn_2F13C, sizeof(Sn_2F13C) );

	// 個別対応：課税売上にのみ要するもの  Sn_2F16A -> Sn_2F15A
	memmove( CH26SnFhyo20Data::Sn_2F15A, Sn_2F16A, sizeof(Sn_2F16A) );
	memmove( CH26SnFhyo20Data::Sn_2F15B, Sn_2F16B, sizeof(Sn_2F16B) );
	memmove( CH26SnFhyo20Data::Sn_2F15C, Sn_2F16C, sizeof(Sn_2F16C) );

	// 個別対応：共通の売上に係る Sn_2F17A -> Sn_2F16A
	memmove( CH26SnFhyo20Data::Sn_2F16A, Sn_2F17A, sizeof(Sn_2F17A) );
	memmove( CH26SnFhyo20Data::Sn_2F16B, Sn_2F17B, sizeof(Sn_2F17B) );
	memmove( CH26SnFhyo20Data::Sn_2F16C, Sn_2F17C, sizeof(Sn_2F17C) );

	// 個別対応：控除する課税仕入れ等の税額 Sn_2F18A -> Sn_2F17A
	memmove( CH26SnFhyo20Data::Sn_2F17A, Sn_2F18A, sizeof(Sn_2F18A) );
	memmove( CH26SnFhyo20Data::Sn_2F17B, Sn_2F18B, sizeof(Sn_2F18B) );
	memmove( CH26SnFhyo20Data::Sn_2F17C, Sn_2F18C, sizeof(Sn_2F18C) );

	// 比例配分：控除する課税仕入れ等の税額 Sn_2F19A -> Sn_2F18A
	memmove( CH26SnFhyo20Data::Sn_2F18A, Sn_2F19A, sizeof(Sn_2F19A) );
	memmove( CH26SnFhyo20Data::Sn_2F18B, Sn_2F19B, sizeof(Sn_2F19B) );
	memmove( CH26SnFhyo20Data::Sn_2F18C, Sn_2F19C, sizeof(Sn_2F19C) );

	// 控除税額の調整 Sn_2F20A -> Sn_2F19A
	memmove( CH26SnFhyo20Data::Sn_2F19A, Sn_2F20A, sizeof(Sn_2F20A) );
	memmove( CH26SnFhyo20Data::Sn_2F19B, Sn_2F20B, sizeof(Sn_2F20B) );
	memmove( CH26SnFhyo20Data::Sn_2F19C, Sn_2F20C, sizeof(Sn_2F20C) );

	// 控除税額の調整 Sn_2F21A -> Sn_2F20A
	memmove( CH26SnFhyo20Data::Sn_2F20A, Sn_2F21A, sizeof(Sn_2F21A) );
	memmove( CH26SnFhyo20Data::Sn_2F20B, Sn_2F21B, sizeof(Sn_2F21B) );
	memmove( CH26SnFhyo20Data::Sn_2F20C, Sn_2F21C, sizeof(Sn_2F21C) );

	// 付表2｢22｣- 控除対象仕入税額手入力額 Sn_2F22A -> Sn_2F21A
	memmove( CH26SnFhyo20Data::Sn_2F21A, Sn_2F22A, sizeof(Sn_2F22A) );
	memmove( CH26SnFhyo20Data::Sn_2F21B, Sn_2F22B, sizeof(Sn_2F22B) );
	memmove( CH26SnFhyo20Data::Sn_2F21C, Sn_2F22C, sizeof(Sn_2F22C) );
	CH26SnFhyo20Data::Sn_2F21sw = Sn_2F22sw;// 控除対象仕入税額　手入力サイン D0: D4:

	// 貸倒回収に係る消費税額 Sn_2F24A -> Sn_2F23A
	memmove( CH26SnFhyo20Data::Sn_2F23A, Sn_2F24A, sizeof(Sn_2F24A) );
	memmove( CH26SnFhyo20Data::Sn_2F23B, Sn_2F24B, sizeof(Sn_2F24B) );
	memmove( CH26SnFhyo20Data::Sn_2F23C, Sn_2F24C, sizeof(Sn_2F24C) );
	//<-----------------------------

	// 基底クラスでのデータのセット
	if( CH26SnFhyo20Data::SetData(pDBNpSub, sn_seq) ){
		return -1;
	}
	
	CString	filter;
	char	*pHyoID = NULL;
	int	nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_2_2_HYOID;

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

		//特定課税仕入れに係る支払対価の額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEC00270");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F10C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		//特定課税仕入れに係る消費税額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEC00300");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F11C );
		pShLstVal->sign1 = Sn_2F11sw;
		pShLstVal->Update();
	}
	else{// データ有り*/
				
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

			//特定課税仕入れに係る支払対価の額
			if( pShLstVal->itmId == _T("AEC00270") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F10C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}

			//特定課税仕入れに係る消費税額
			if( pShLstVal->itmId == _T("AEC00300") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F11C );
				pShLstVal->sign1 = Sn_2F11sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
		}

		// 更新データ確認
		int	adCnt = 0;
		for( int i=0;; i++ ){
			if( strlen(tagData20_28[i].tagName) == 0 ){
				break;
			}
			if( m_isTransitionalMeasure== FALSE ){
				if( (tagData20_28[i].sign1!=0) && (tagData20_28[i].sign1!=1) ){
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
			MonyPtrAry.Add( Sn_2F10C );
			MonyPtrAry.Add( Sn_2F11C );
			
			BOOL findFlg = FALSE;
			for( int i=0;; i++ ){
				if( strlen(tagData20_28[i].tagName) == 0 ){
					break;
				}
				if( m_isTransitionalMeasure == FALSE ){
					if( (tagData20_28[i].sign1==0) || (tagData20_28[i].sign1==1) ){
						continue;
					}
				}

				findFlg = FALSE;
				for( int j=0; j<updCnt; j++ ){
					if( updTagAry[j].CompareNoCase(tagData20_28[i].tagName) == 0 ){
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
					pShLstVal->itmId = tagData20_28[i].tagName;
					m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );

					if( pShLstVal->itmId == _T("AEC00300") ){
						pShLstVal->sign1 = Sn_2F11sw;
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
int CH28SnFhyo20Data::SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	
	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("22") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	// □□□□ □■■■
											//  D0：3%
											//  D1：5%
											//  D2：8%

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			//課税売上額（税抜き）
			if( money[i][j].code.Left(4) == _T("2201") ){
				if( money[i][j].code == _T("220101") ){//税率３％適用分
					memmove( Sn_2F1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("220102") ){//税率４％適用分
					memmove( Sn_2F1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("220103") ){//税率６．３％適用分
					memmove( Sn_2F1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			//免税売上額
			else if( money[i][j].code.Left(4) == _T("2202") ){
				memmove( Sn_2F2C, money[i][j].arith, MONY_BUF_SIZE );
			}
			//非課税資産の輸出等の金額、海外支店
			else if( money[i][j].code.Left(4) == _T("2203") ){
				memmove( Sn_2F3C, money[i][j].arith, MONY_BUF_SIZE );
			}
			//非課税売上額
			else if( money[i][j].code.Left(4) == _T("2206") ){
				if( money[i][j].code == _T("220600") ){
					memmove( Sn_2F6C, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			//課税仕入れに係る支払対価の額（税込み）
			else if( money[i][j].code.Left(4) == _T("2208") ){
				if( money[i][j].code == _T("220801") ){
					memmove( Sn_2F8A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("220802") ){
					memmove( Sn_2F8B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("220803") ){
					memmove( Sn_2F8C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			//課税仕入れに係る消費税額
			else if( money[i][j].code.Left(4) == _T("2209") ){
				if( money[i][j].code == _T("220901") ){
					memmove( Sn_2F9A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("220902") ){
					memmove( Sn_2F9B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("220903") ){
					memmove( Sn_2F9C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			//特定課税仕入れに係る支払対価の額
			else if( money[i][j].code.Left(4) == _T("2210") ){
				if( money[i][j].code == _T("221003") ){
					memmove( Sn_2F10C, money[i][j].arith, MONY_BUF_SIZE );
				}
			}

			//特定課税仕入れに係る消費税額
			else if( money[i][j].code.Left(4) == _T("2211") ){
				if( money[i][j].code == _T("221103") ){
					memmove( Sn_2F11C, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			//課税貨物に係る消費税額
			else if( money[i][j].code.Left(4) == _T("2212") ){
				if( money[i][j].code == _T("221201") ){
					memmove( Sn_2F12A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("221202") ){
					memmove( Sn_2F12B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("221203") ){
					memmove( Sn_2F12C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			// 納税義務の免除を受けない（受ける）こととなっ・・
			else if( money[i][j].code.Left(4) == _T("2212") ){
				//H26SnFhyo20Dataでは処理してなかったのでこっちも処理作成していない
			}

			// 個別対応：課税売上にのみ要するもの
			else if( money[i][j].code.Left(4) == _T("2216") ){
				if( money[i][j].code == _T("221601") ){
					memmove( Sn_2F16A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("221602") ){
					memmove( Sn_2F16B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("221603") ){
					memmove( Sn_2F16C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			// 個別対応：課税売上と非課税売上に共通して要するもの
			else if( money[i][j].code.Left(4) == _T("2217") ){
				if( money[i][j].code == _T("221701") ){
					memmove( Sn_2F17A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("221702") ){
					memmove( Sn_2F17B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("221703") ){
					memmove( Sn_2F17C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			//----->H26SnFhyo20Dataでは処理してなかったのでこっちも処理作成していない
			// 個別対応：控除する課税仕入れ等の税額
			else if( money[i][j].code.Left(4) == _T("2218") ){
			}
			// 比例配分：控除する課税仕入れ等の税額
			else if( money[i][j].code.Left(4) == _T("2219") ){
			}
			//<----------------------------------------------------------------------
		
			//// 貸倒回収に係る消費税額
			else if( money[i][j].code.Left(4) == _T("2224") ){
				if( money[i][j].code == _T("222401") ){
					memmove( Sn_2F24A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("222402") ){
					memmove( Sn_2F24B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("222403") ){
					memmove( Sn_2F24C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
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
// 更新時の金額クリア
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		type		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CH28SnFhyo20Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type , int ClearType)
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// 基底クラスでのデータの取得
	if( CH26SnFhyo20Data::ClearDataByRenew(pDBNpSub, type , ClearType) ){
		return -1;
	}

	// 特定課税仕入れに係る支払対価の額
	m_Arith.l_clear( Sn_2F10C );
	// 特定課税仕入れに係る消費税額
	m_Arith.l_clear( Sn_2F11C );
	Sn_2F11sw = 0;

	m_Arith.l_clear( Sn_2F12A );
	m_Arith.l_clear( Sn_2F12B );
	m_Arith.l_clear( Sn_2F12C );

	m_Arith.l_clear( Sn_2F13A );
	m_Arith.l_clear( Sn_2F13B );
	m_Arith.l_clear( Sn_2F13C );
	
	m_Arith.l_clear( Sn_2F16A );
	m_Arith.l_clear( Sn_2F16B );
	m_Arith.l_clear( Sn_2F16C );

	m_Arith.l_clear( Sn_2F17A );
	m_Arith.l_clear( Sn_2F17B );
	m_Arith.l_clear( Sn_2F17C );

	m_Arith.l_clear( Sn_2F18A );
	m_Arith.l_clear( Sn_2F18B );
	m_Arith.l_clear( Sn_2F18C );

	m_Arith.l_clear( Sn_2F19A );
	m_Arith.l_clear( Sn_2F19B );
	m_Arith.l_clear( Sn_2F19C );

	if( (ClearType & 0x01) == 0x00 ){
		m_Arith.l_clear( Sn_2F20A );
		m_Arith.l_clear( Sn_2F20B );
		m_Arith.l_clear( Sn_2F20C );

		m_Arith.l_clear( Sn_2F21A );
		m_Arith.l_clear( Sn_2F21B );
		m_Arith.l_clear( Sn_2F21C );
	}

	m_Arith.l_clear( Sn_2F22A );
	m_Arith.l_clear( Sn_2F22B );
	m_Arith.l_clear( Sn_2F22C );
	Sn_2F22sw = 0;

	m_Arith.l_clear( Sn_2F24A );
	m_Arith.l_clear( Sn_2F24B );
	m_Arith.l_clear( Sn_2F24C );

	return 0;

}