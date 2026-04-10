
#include "stdafx.h"
#include "ShinFhyo40.h"
#include "ShinFhyo40Idx.h"
#include "H26HyoView.h"

//-----------------------------------------------------------------------------
// 課税標準　計算
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_3( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0};

	m_Util.l_calq( WORK0, F41A, 0 );	// 1000円未満切り捨て
	m_Util.l_calq( WORK1, F41B, 0 );	// 1000円未満切り捨て
	m_Util.l_calq( WORK2, F41C, 0 );	// 1000円未満切り捨て

	m_pArith->l_add( F41T, WORK0, WORK1 );
	m_pArith->l_add( F41T, F41T, WORK2 );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID41A, WORK0 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID41B, WORK1 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID41C, WORK2 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID41D, F41T );
	}
}

#ifdef _20140122_EXTEND_
//-----------------------------------------------------------------------------
// 消費税額(4%税率分)
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_4( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	if( sw < 2 ){
		if( (F4c&0x01) || (m_pArith->l_test(F42B)==0) ){
			memset( F42B, '\0', MONY_BUF_SIZE );
			if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
				m_Util.l_calq( WORK0, F41B, 0 );
			}
			else{
				memmove( WORK0, F41B, MONY_BUF_SIZE );
			}
			m_Util.percent( F42B, WORK0, 1, 40, 0 );
		}
	}
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID42B, F42B );
	}
}
#else
//-----------------------------------------------------------------------------
// 消費税額（個々の税率）
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_4( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	if( sw < 2 ){
		// 3%

		//[Y]ここの判定で本表の手入力サインをみるようにする。（印刷側で付表のデータを考慮しているので集計かけるとデータがずれる為）
		if( (!(*m_pSnHeadData)->SVmzsw) &&  ((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){	// 経過措置無しで本表手入力サインがONの場合には計算しない

		}
		else if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

		}

		else{
			if( !(F4c&0x08) ){//[Y]
	//		if( (F4c&0x01) || (m_pArith->l_test(F42A)==0) ){
				if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
					ChangeColor( IDC_ICSDIAGCTRL2, ID42A, 0 );
				}
/*- '14.05.17 -*/
//				memset( F42A, '\0', MONY_BUF_SIZE );
//				if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
//					m_Util.l_calq( WORK0, F41A, 0 );
//				}
//				else{
//					memmove( WORK0, F41A, MONY_BUF_SIZE );
//				}
//				m_Util.percent( F42A, WORK0, 1, 30, 0 );
/*-------------*/
				if( (F4c&0x01) || (m_pArith->l_test(F42A)==0) ){
					memset( F42A, '\0', MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
						m_Util.l_calq( WORK0, F41A, 0 );
					}
					else{
						memmove( WORK0, F41A, MONY_BUF_SIZE );
					}
					m_Util.percent( F42A, WORK0, 1, 30, 0 );
				}
/*-------------*/
			}
		}

		// 4%

		//[Y]ここの判定で本表の手入力サインをみるようにする。（印刷側で付表のデータを考慮しているので集計かけるとデータがずれる為）
		if( (!(*m_pSnHeadData)->SVmzsw) &&  ((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){	// 経過措置無しで本表手入力サインがONの場合には計算しない

		}
		else if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

		}
		else{
			if( !(F4c&0x80) ){//[Y]
	//		if( (F4c&0x10) || (m_pArith->l_test(F42B)==0) ){
				if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
					ChangeColor( IDC_ICSDIAGCTRL2, ID42B, 0 );
				}
/*- '14.05.17 -*/
//				memset( F42B, '\0', MONY_BUF_SIZE );
//				if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
//					m_Util.l_calq( WORK0, F41B, 0 );
//				}
//				else{
//					memmove( WORK0, F41B, MONY_BUF_SIZE );
//				}
//				m_Util.percent( F42B, WORK0, 1, 40, 0 );
/*-------------*/
				if( (F4c&0x10) || (m_pArith->l_test(F42B)==0) ){
					memset( F42B, '\0', MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
						m_Util.l_calq( WORK0, F41B, 0 );
					}
					else{
						memmove( WORK0, F41B, MONY_BUF_SIZE );
					}
					m_Util.percent( F42B, WORK0, 1, 40, 0 );
				}
/*-------------*/
			}
		}

		// 6.3%
		//[Y]ここの判定で本表の手入力サインをみるようにする。（印刷側で付表のデータを考慮しているので集計かけるとデータがずれる為）
		if( (!(*m_pSnHeadData)->SVmzsw) &&  ((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){	// 経過措置無しで本表手入力サインがONの場合には計算しない

		}
		else if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

		}
		else{
			if( !(F4c&0x800) ){//[Y]
	//		if( (F4c&0x100) || (m_pArith->l_test(F42C)==0) ){
				if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
					ChangeColor( IDC_ICSDIAGCTRL2, ID42C, 0 );
				}
/*- '14.05.17 -*/
//				memset( F42C, '\0', MONY_BUF_SIZE );
//				if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
//					m_Util.l_calq( WORK0, F41C, 0 );
//				}
//				else{
//					memmove( WORK0, F41C, MONY_BUF_SIZE );
//				}
//				m_Util.percent( F42C, WORK0, 1, 63, 0 );
/*-------------*/
				if( (F4c&0x100) || (m_pArith->l_test(F42C)==0) ){
					memset( F42C, '\0', MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
						m_Util.l_calq( WORK0, F41C, 0 );
					}
					else{
						memmove( WORK0, F41C, MONY_BUF_SIZE );
					}
					m_Util.percent( F42C, WORK0, 1, 63, 0 );
				}
/*-------------*/
			}
		}
	}
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID42A, F42A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID42B, F42B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID42C, F42C );
	}

	//[Y]
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( (F4c&0x08) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID42A, 3 );	// 2
		}
		if( (F4c&0x80) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID42B, 3 );	// 2
		}
		if( (F4c&0x800) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID42C, 3 );	// 2
		}
	}
//--> '14.05.17 INS START
	F4c&= ~0x01;
//<-- '14.05.17 INS END

}
#endif

#ifdef _20140122_CUT_
//-----------------------------------------------------------------------------
// 消費税額(3%税率分)
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_5( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	if( sw < 2 ){
		if( (F4c&0x10) || (m_pArith->l_test(F42A)==0) ){
			memset( F42A, '\0', MONY_BUF_SIZE );
			if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
				m_Util.l_calq( WORK0, F41A, 0 );
			}
			else{
				memmove( WORK0, F41A, MONY_BUF_SIZE );
			}
			m_Util.percent( F42A, WORK0, 1, 30, 0 );
		}
	}
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID42A, F42A );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID45C, F45A );	不要？？？
	}
}
#endif

//-----------------------------------------------------------------------------
// 消費税額計
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_6( int sw )
{
	m_pArith->l_add( F42T, F42A, F42B );
	m_pArith->l_add( F42T, F42T, F42C );

	if( !sw ){
//--> 不要？？？
//		if( m_pArith->l_test(F42B) == 0 ){
//			dsp_prs( IDC_ICSDIAGCTRL2, ID44C, F44B );
//		}
//		if( m_pArith->l_test(F45A) == 0 ){
//			dsp_prs( IDC_ICSDIAGCTRL2, ID45C, F45A );
//		}
//		dsp_prs( IDC_ICSDIAGCTRL2, ID46A, F45A );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID46B, F44B );
//<---
		dsp_prs( IDC_ICSDIAGCTRL2, ID42D, F42T );
	}
}

//-----------------------------------------------------------------------------
// 控除過大調整税額	･ 控除対象仕入税額
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_78( int sw )
{
	m_pArith->l_add( F43T, F43A, F43B );
	m_pArith->l_add( F43T, F43T, F43C );

//-- '14.10.17 --
//	m_pArith->l_add( F44T, F44A, F44B );
//	m_pArith->l_add( F44T, F44T, F44C );
//---------------
	if( m_pParent && ((CH26HyoView*)m_pParent)->IsNeedExpCalq() ){
		;
	}
	else{
		m_pArith->l_add( F44T, F44A, F44B );
		m_pArith->l_add( F44T, F44T, F44C );
	}
//---------------

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID43A, F43A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID43B, F43B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID43C, F43C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID43D, F43T );
	}
	
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID44A, F44A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID44B, F44B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID44C, F44C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID44D, F44T );
	}
}

//-----------------------------------------------------------------------------
// 返還等対価に係る税額
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_9( int sw )
{
	m_pArith->l_add( F45T, F45A, F45B );
	m_pArith->l_add( F45T, F45T, F45C );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID45A, F45A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID45B, F45B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID45C, F45C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID45D, F45T );
	}
}

//-----------------------------------------------------------------------------
// 貸し倒れに係る税額
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_10( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};
	m_pArith->l_add( WORK0, F46A, F46B );
	m_pArith->l_add( WORK0, WORK0, F46C );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID46A, F46A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID46B, F46B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID46C, F46C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID46D, WORK0 );
	}
}

//-----------------------------------------------------------------------------
// 控除税額小計
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_11( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};
	// 3%
	m_pArith->l_add( F47A, F44A, F45A );
	m_pArith->l_add( F47A, F47A, F46A );
	// 4%
	m_pArith->l_add( F47B, F44B, F45B );
	m_pArith->l_add( F47B, F47B, F46B );
	// 6.3%
	m_pArith->l_add( F47C, F44C, F45C );
	m_pArith->l_add( F47C, F47C, F46C );
	// 合計
	m_pArith->l_add( WORK0, F46A, F46B );
	m_pArith->l_add( WORK0, WORK0, F46C );
//-- '14.12.19 --
//	m_pArith->l_add( F47T, F44T,  F45T );
//	m_pArith->l_add( F47T, F47T, WORK0 );
//---------------
	m_pArith->l_add( F47T, F44T, F45T );
	m_pArith->l_add( F47T, F47T, WORK0 );
//---------------

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID47A, F47A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID47B, F47B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID47C, F47C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID47D, F47T );
	}
}

//-----------------------------------------------------------------------------
// 控除不足税額（11－6－7）･ 差引税額（6+7-11)
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_1213( int sw )
{
	if( !sw ){
		dsp_cls( IDC_ICSDIAGCTRL2, ID48A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID48B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID48C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID48D );

		dsp_cls( IDC_ICSDIAGCTRL2, ID49A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID49B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID49C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID49D );
		dsp_cls( IDC_ICSDIAGCTRL2, ID410D );

		dsp_cls( IDC_ICSDIAGCTRL2, ID411B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID411C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID411D );

		dsp_cls( IDC_ICSDIAGCTRL2, ID412B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID412C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID412D );
	}

	// 3%
	m_pArith->l_sub( F48A, F47A, F42A );
	m_pArith->l_sub( F48A, F48A, F43A );

	if( m_pArith->l_test(F48A) < 0 ){
		memmove( F49A, F48A, MONY_BUF_SIZE );
		m_pArith->l_neg( F49A );
		memset( F48A, '\0', MONY_BUF_SIZE );
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID49A, F49A );
		}
	}
	else{
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID48A, F48A );
		}
		memset( F49A, '\0', MONY_BUF_SIZE );
	}

	// 4%
	m_pArith->l_sub( F48B, F47B, F42B );
	m_pArith->l_sub( F48B, F48B, F43B );

	if( m_pArith->l_test(F48B) < 0 ){
		memmove( F49B, F48B, MONY_BUF_SIZE );
		m_pArith->l_neg( F49B );
		memset( F48B, '\0', MONY_BUF_SIZE );
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID49B, F49B );
		}
	}
	else{
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID48B, F48B );
		}
		memset( F49B, '\0', MONY_BUF_SIZE );
	}
	// コピー
	memmove( F411B, F48B, MONY_BUF_SIZE );
	memmove( F412B, F49B, MONY_BUF_SIZE );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID411B, F411B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID412B, F412B );
	}

	// 6.3%
	m_pArith->l_sub( F48C, F47C, F42C );
	m_pArith->l_sub( F48C, F48C, F43C );

	if( m_pArith->l_test(F48C) < 0 ){
		memmove( F49C, F48C, MONY_BUF_SIZE );
		m_pArith->l_neg( F49C );
		memset( F48C, '\0', MONY_BUF_SIZE );
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID49C, F49C );
		}
	}
	else{
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID48C, F48C );
		}
		memset( F49C, '\0', MONY_BUF_SIZE );
	}
	// コピー
	memmove( F411C, F48C, MONY_BUF_SIZE );
	memmove( F412C, F49C, MONY_BUF_SIZE );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID411C, F411C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID412C, F412C );
	}

	// 簡易課税・付表４
//-- '14.10.17 --
//	m_pArith->l_add( F48T, F48A, F48B );
//	m_pArith->l_add( F48T, F48T, F48C );
//	m_pArith->l_add( F49T, F49A, F49B );
//	m_pArith->l_add( F49T, F49T, F49C );
//
//	if( !sw ){
//		dsp_prs( IDC_ICSDIAGCTRL2, ID48D, F48T );
////		dsp_prs( IDC_ICSDIAGCTRL2, ID411D, F48T );
//
//		dsp_prs( IDC_ICSDIAGCTRL2, ID49D, F49T );
////		dsp_prs( IDC_ICSDIAGCTRL2, ID412D, F49T );
//	}
//---------------
	if( m_pParent && ((CH26HyoView*)m_pParent)->IsNeedExpCalq() ){
		m_pArith->l_sub( F48T, F47T, F42T );
		m_pArith->l_sub( F48T, F48T, F43T );

		if( m_pArith->l_test(F48T) < 0 ){
			memmove( F49T, F48T, MONY_BUF_SIZE );
			m_pArith->l_neg( F49T );
			memset( F48T, '\0', MONY_BUF_SIZE );
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID49D, F49T );
			}
		}
		else{
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID48D, F48T );
			}
			memset( F49T, '\0', MONY_BUF_SIZE );
		}
	}
	else{
		m_pArith->l_add( F48T, F48A, F48B );
		m_pArith->l_add( F48T, F48T, F48C );
		m_pArith->l_add( F49T, F49A, F49B );
		m_pArith->l_add( F49T, F49T, F49C );

		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID48D, F48T );

			dsp_prs( IDC_ICSDIAGCTRL2, ID49D, F49T );
		}
	}
//---------------

	// 金額計算
	m_pArith->l_add( F411T, F48B, F48C );
	m_pArith->l_add( F412T, F49B, F49C );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID411D, F411T );
		dsp_prs( IDC_ICSDIAGCTRL2, ID412D, F412T );
	}

//	memmove( F411T, F48T, MONY_BUF_SIZE );
//	memmove( F412T, F49T, MONY_BUF_SIZE );
}

//-----------------------------------------------------------------------------
// 合計差引税額（13－12)
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_14( int sw )
{
	m_pArith->l_sub( F410T, F49T, F48T );
	m_pArith->l_sub( F413T, F412T, F411T );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID410D, F410T );
		dsp_prs( IDC_ICSDIAGCTRL2, ID413D, F413T );
	}
}

//-----------------------------------------------------------------------------
// 譲渡割額(14,15)
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_15( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	m_Util.percent( F414B, F411B, 7, 25, 0 );
	m_Util.percent( F415B, F412B, 7, 25, 0 );

	m_Util.percent( F414C, F411C, 8, 17, 0 );
	m_Util.percent( F415C, F412C, 8, 17, 0 );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID414B, F414B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID414C, F414C );

		dsp_prs( IDC_ICSDIAGCTRL2, ID415B, F415B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID415C, F415C );
	}

	// 合計
	m_pArith->l_add( F414T, F414B, F414C );
	m_pArith->l_add( F415T, F415B, F415C );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID414D, F414T );
		dsp_prs( IDC_ICSDIAGCTRL2, ID415D, F415T );
	}

	// 合計差引譲渡割額
	m_pArith->l_sub( F416T, F415T, F414T );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID416D, F416T );
	}
}

//-----------------------------------------------------------------------------
// 各入力の計算表示
//-----------------------------------------------------------------------------
// 引数	atno	：	
//		sw		：	
//-----------------------------------------------------------------------------
void CShinFhyo40::f4calq_all( int atno, int sw )
{
	while(1){
		switch( atno ){
			case	1 :
			case	2 :
				//  ３．（１＋２）課税標準額
				f4calq_3( sw );
				//  ４．消費税額（個々の税率）
				f4calq_4( sw );
/*- '14.01.20_CUT -*/
//				//	５．消費税額（3%税率分）
//				f4calq_5( sw );
/*-----------------*/
			case	3 :
			case	4 :
				//  ６．（４＋５）消費税額合計
				f4calq_6( sw );

			case	5 :
			case	6 :	
				//	７．控除過大仕入額
				//	８．控除対象仕入税額
				f4calq_78( sw );
			case	7 :
			case	8 :
				//	９．返還等対価に係る税額
				f4calq_9( sw );
			case	9 :
			case	10:
				//１０．貸倒に係る税額
				f4calq_10( sw );
				//１１．控除税額小計
				f4calq_11( sw );
				//１２．控除不足還付税額・１３．差引税額
				f4calq_1213( sw );
				//１４．合計差引税額
				f4calq_14( sw );
				//１５．譲渡割額(14,15)
				f4calq_15( sw );
			default	:
				break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// 他表からの計算
//-----------------------------------------------------------------------------
void CShinFhyo40::calq_by_OtherChange()
{
	ReadData( 0 );
	f4calq_all( 1, 1 );
	WriteData();
}
void CShinFhyo40::calqx_otherlists(){

	if( m_pParent ){
		((CH26HyoView*)m_pParent)->CalqFromKanHonpyo();
	}
}