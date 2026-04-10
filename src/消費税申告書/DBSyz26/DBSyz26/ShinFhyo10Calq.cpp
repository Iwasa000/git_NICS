
#include "stdafx.h"
#include "ShinFhyo10.h"
#include "ShinFhyo10Idx.h"

//-----------------------------------------------------------------------------
// 課税標準　計算
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_3( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0};

	m_Util.l_calq( WORK0, F11A, 0 );	// 1000円未満切り捨て
	m_Util.l_calq( WORK1, F11B, 0 );	// 1000円未満切り捨て
	m_Util.l_calq( WORK2, F11C, 0 );	// 1000円未満切り捨て

	m_pArith->l_add( F11T, WORK0, WORK1 );
	m_pArith->l_add( F11T, F11T, WORK2 );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID11A, WORK0 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID11B, WORK1 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID11C, WORK2 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID11D, F11T );
	}
}

//-----------------------------------------------------------------------------
// 消費税額(4%税率分)
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_4( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	if( sw < 2 ){

		//ここの判定で本表の手入力サインをみるようにする。（印刷側で付表のデータを考慮しているので集計かけるとデータがずれる為）
		if( (!(*m_pSnHeadData)->SVmzsw) &&  ((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){	// 経過措置無しで本表手入力サインがONの場合には計算しない

		}
		else if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

		}
		else{
			if( !(F1c&0x80) ){//[Y]
	//		if( (F1c&0x10) || (m_pArith->l_test(F12B)==0) ){
				if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
					ChangeColor( IDC_ICSDIAGCTRL2, ID12B, 0 );	//[Y]
				}
/*- '14.05.17 -*/
//				memset( F12B, '\0', MONY_BUF_SIZE );
//				if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
//					m_Util.l_calq( WORK0, F11B, 0 );
//				}
//				else{
//					memmove( WORK0, F11B, MONY_BUF_SIZE );
//				}
//				m_Util.percent( F12B, WORK0, 1, 40, 0 );
/*-------------*/
				if( (F1c&0x10) || (m_pArith->l_test(F12B)==0) ){
					memset( F12B, '\0', MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
						m_Util.l_calq( WORK0, F11B, 0 );
					}
					else{
						memmove( WORK0, F11B, MONY_BUF_SIZE );
					}
					m_Util.percent( F12B, WORK0, 1, 40, 0 );
				}
/*-------------*/
			}
		}
	}
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID12B, F12B );
	}

	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if((F1c&0x80) ){//[Y]
			ChangeColor( IDC_ICSDIAGCTRL2, ID12B, 3 );	// 2
		}
	}
//--> '14.05.17 INS START
	F1c &= ~0x10;
//<-- '14.05.17 INS END
}

//-----------------------------------------------------------------------------
// 消費税額(3%税率分)
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_5( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	if( sw < 2 ){

		//[Y]ここの判定で本表の手入力サインをみるようにする。（印刷側で付表のデータを考慮しているので集計かけるとデータがずれる為）
		if( (!(*m_pSnHeadData)->SVmzsw) &&  ((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){	// 経過措置無しで本表手入力サインがONの場合には計算しない

		}
		else if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

		}
		else{
			if( !(F1c&0x08) ){//[Y]
	//		if( (F1c&0x01) || (m_pArith->l_test(F12A)==0) ){
				if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
					ChangeColor( IDC_ICSDIAGCTRL2, ID12A, 0 );	//[Y]
				}
/*- '14.05.17 -*/
//				memset( F12A, '\0', MONY_BUF_SIZE );
//				if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
//					m_Util.l_calq(  WORK0, F11A, 0 );
//				}
//				else{
//					memmove( WORK0, F11A, MONY_BUF_SIZE );
//				}
//				m_Util.percent( F12A, WORK0, 1, 30, 0 );
/*-------------*/
				if( (F1c&0x01) || (m_pArith->l_test(F12A)==0) ){
					memset( F12A, '\0', MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
						m_Util.l_calq(  WORK0, F11A, 0 );
					}
					else{
						memmove( WORK0, F11A, MONY_BUF_SIZE );
					}
					m_Util.percent( F12A, WORK0, 1, 30, 0 );
				}
/*-------------*/
			}
		}
	}
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID12A, F12A );
	}

	//[Y]
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if((F1c&0x08) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID12A, 3 );	// 2
		}
	}
//--> '14.05.17 INS START
	F1c &= ~0x01;
//<-- '14.05.17 INS END
}

//-----------------------------------------------------------------------------
// 消費税額(6.3%税率分)
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_5_2( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	if( sw < 2 ){

		//[Y]ここの判定で本表の手入力サインをみるようにする。（印刷側で付表のデータを考慮しているので集計かけるとデータがずれる為）
		if( (!(*m_pSnHeadData)->SVmzsw) &&  ((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){	// 経過措置無しで本表手入力サインがONの場合には計算しない

		}
		else if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

		}
		else{
			if( !(F1c&0x800) ){//[Y]
	//		if( (F1c&0x100) || (m_pArith->l_test(F12C)==0) ){
				if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
					ChangeColor( IDC_ICSDIAGCTRL2, ID12C, 0 );//[Y]
				}
/*- '14.05.17 -*/
//				memset( F12C, '\0', MONY_BUF_SIZE );
//				if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
//					m_Util.l_calq(  WORK0, F11C, 0 );
//				}
//				else{
//					memmove( WORK0, F11C, MONY_BUF_SIZE );
//				}
//				m_Util.percent( F12C, WORK0, 1, 63, 0 );
/*-------------*/
				if( (F1c&0x100) || (m_pArith->l_test(F12C)==0) ){
					memset( F12C, '\0', MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_TUMIAG&0x01) ){
						m_Util.l_calq(  WORK0, F11C, 0 );
					}
					else{
						memmove( WORK0, F11C, MONY_BUF_SIZE );
					}
					m_Util.percent( F12C, WORK0, 1, 63, 0 );
				}
/*-------------*/
			}
		}
	}
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID12C, F12C );
	}

	//[Y]
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( (F1c&0x800) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID12C, 3 );	// 2
		}
	}
//--> '14.05.17 INS START
	F1c &= ~0x100;
//<-- '14.05.17 INS END
}

//-----------------------------------------------------------------------------
// 消費税額計
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_6( int sw )
{
	m_pArith->l_add( F12T, F12A, F12B );
	m_pArith->l_add( F12T, F12T, F12C );

	if( !sw ){
//--> 不要？？？
//		if( !m_pArith->l_test( F14B ) ){
//			dsp_prs( IDC_ICSDIAGCTRL2, ID14C, F14B );
//		}
//		if( ! m_pArith->l_test( F15A ) ){
//			dsp_prs( IDC_ICSDIAGCTRL2, ID15C, F15A );
//		}
//		dsp_prs( IDC_ICSDIAGCTRL2, ID16A, F15A );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID16B, F14B );
//<---
		dsp_prs( IDC_ICSDIAGCTRL2, ID12D, F12T );
	}
}

//-----------------------------------------------------------------------------
// 控除過大調整税額	･ 控除対象仕入税額
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_78( int sw )
{
	m_pArith->l_add( F13T, F13A, F13B );
	m_pArith->l_add( F13T, F13T, F13C );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID13A, F13A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID13B, F13B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID13C, F13C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID13D, F13T );
	}
	
	m_pArith->l_add( F14T, F14A, F14B );
	m_pArith->l_add( F14T, F14T, F14C );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID14A, F14A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID14B, F14B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID14C, F14C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID14D, F14T );
	}
}

//-----------------------------------------------------------------------------
// 返還等対価に係る税額
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_9( int sw )
{
	m_pArith->l_add( F15T, F15A, F15B );
	m_pArith->l_add( F15T, F15T, F15C );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID15A, F15A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID15B, F15B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID15C, F15C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID15D, F15T );
	}
}

//-----------------------------------------------------------------------------
// 貸倒れに係る税額
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_10( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	m_pArith->l_add( WORK0, F16A, F16B );
	m_pArith->l_add( WORK0, WORK0, F16C );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID16A, F16A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID16B, F16B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID16C, F16C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID16D, WORK0 );
	}
}

//-----------------------------------------------------------------------------
// 控除税額小計
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_11( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	// 3%
	m_pArith->l_add( F17A, F14A, F15A );
	m_pArith->l_add( F17A, F17A, F16A );
	// 4%
	m_pArith->l_add( F17B, F14B, F15B );
	m_pArith->l_add( F17B, F17B, F16B );
	// 6.3%
	m_pArith->l_add( F17C, F14C, F15C );
	m_pArith->l_add( F17C, F17C, F16C );
	// 合計
	m_pArith->l_add( WORK0, F16A, F16B );
	m_pArith->l_add( WORK0, WORK0, F16C );
	m_pArith->l_add( F17T, F14T,  F15T );
	m_pArith->l_add( F17T, F17T, WORK0 );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID17A, F17A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID17B, F17B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID17C, F17C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID17D, F17T );
	}
}

//-----------------------------------------------------------------------------
// 控除不足税額（11－6－7）･ 差引税額（6+7-11)
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_1213( int sw )
{
	if( !sw ){
		dsp_cls( IDC_ICSDIAGCTRL2, ID18A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID18B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID18C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID18D );

		dsp_cls( IDC_ICSDIAGCTRL2, ID19A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID19B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID19C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID19D );
		dsp_cls( IDC_ICSDIAGCTRL2, ID110D );

		dsp_cls( IDC_ICSDIAGCTRL2, ID111B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID111C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID111D );

		dsp_cls( IDC_ICSDIAGCTRL2, ID112B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID112C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID112D );
	}

	// 3%
	m_pArith->l_sub( F18A, F17A, F12A );
	m_pArith->l_sub( F18A, F18A, F13A );

	if( m_pArith->l_test(F18A) < 0 ){
		memmove( F19A, F18A, MONY_BUF_SIZE );
		m_pArith->l_neg( F19A );
		memset( F18A, '\0', MONY_BUF_SIZE );
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID19A, F19A );
		}
	}
	else{
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID18A, F18A );
		}
		memset( F19A, '\0', MONY_BUF_SIZE );
	}

	// 4%
	m_pArith->l_sub( F18B, F17B, F12B );
	m_pArith->l_sub( F18B, F18B, F13B );

	if( m_pArith->l_test(F18B) < 0 ){
		memmove( F19B, F18B, MONY_BUF_SIZE );
		m_pArith->l_neg( F19B );
		memset( F18B, '\0', MONY_BUF_SIZE );
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID19B, F19B );
			dsp_prs( IDC_ICSDIAGCTRL2, ID112B, F19B );
		}
	}
	else{
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID18B, F18B );
			dsp_prs( IDC_ICSDIAGCTRL2, ID111B, F18B );
		}
		memset( F19B, '\0', MONY_BUF_SIZE );
	}

	// 6.3%
	m_pArith->l_sub( F18C, F17C, F12C );
	m_pArith->l_sub( F18C, F18C, F13C );

	if( m_pArith->l_test(F18C) < 0 ){
		memmove( F19C, F18C, MONY_BUF_SIZE );
		m_pArith->l_neg( F19C );
		memset( F18C, '\0', MONY_BUF_SIZE );
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID19C, F19C );
			dsp_prs( IDC_ICSDIAGCTRL2, ID112C, F19C );
		}
	}
	else{
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID18C, F18C );
			dsp_prs( IDC_ICSDIAGCTRL2, ID111C, F18C );
		}
		memset( F19C, '\0', MONY_BUF_SIZE );
	}

	// 金額コピー
	memmove( F111B, F18B, MONY_BUF_SIZE );
	memmove( F111C, F18C, MONY_BUF_SIZE );
	memmove( F112B, F19B, MONY_BUF_SIZE );
	memmove( F112C, F19C, MONY_BUF_SIZE );

	// 原則課税・付表１
	m_pArith->l_add( F18T, F18A, F18B );
	m_pArith->l_add( F18T, F18T, F18C );
	m_pArith->l_add( F19T, F19A, F19B );
	m_pArith->l_add( F19T, F19T, F19C );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID18D, F18T );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID111D, F18T );

		dsp_prs( IDC_ICSDIAGCTRL2, ID19D, F19T );
//		dsp_prs( IDC_ICSDIAGCTRL2, ID112D, F19T );
	}

	// 金額計算
	m_pArith->l_add( F111T, F18B, F18C );
	m_pArith->l_add( F112T, F19B, F19C );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID111D, F111T );
		dsp_prs( IDC_ICSDIAGCTRL2, ID112D, F112T );
	}
	
//	memmove( F111T, F18T, MONY_BUF_SIZE );
//	memmove( F112T, F19T, MONY_BUF_SIZE );


	// 合計差引地方消費税の課税標準となる消費税額
	m_pArith->l_sub( F113T, F112T, F111T );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID113D, F113T );
	}
}

//-----------------------------------------------------------------------------
// 合計差引税額（13－12)
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_14( int sw )
{
	m_pArith->l_sub( F110T, F19T, F18T );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID110D, F110T );
	}
}

//-----------------------------------------------------------------------------
// 譲渡割額(14,15)
//-----------------------------------------------------------------------------
// 引数	sw	：
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_15( int sw )
{
	char	WORK0[MONY_BUF_SIZE]={0};

	m_Util.percent( F114B, F111B, 7, 25, 0 );
	m_Util.percent( F115B, F112B, 7, 25, 0 );

	m_Util.percent( F114C, F111C, 8, 17, 0 );
	m_Util.percent( F115C, F112C, 8, 17, 0 );

	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID114B, F114B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID114C, F114C );

		dsp_prs( IDC_ICSDIAGCTRL2, ID115B, F115B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID115C, F115C );
	}

	// 合計
	m_pArith->l_add( F114T, F114B, F114C );
	m_pArith->l_add( F115T, F115B, F115C );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID114D, F114T );
		dsp_prs( IDC_ICSDIAGCTRL2, ID115D, F115T );
	}

	// 合計差引譲渡割額
	m_pArith->l_sub( F116T, F115T, F114T );
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID116D, F116T );
	}
}

//-----------------------------------------------------------------------------
// 各入力の計算表示
//-----------------------------------------------------------------------------
// 引数	atno	：	
//		sw		：	
//-----------------------------------------------------------------------------
void CShinFhyo10::f1calq_all( int atno, int sw )
{
	while( 1 ){
		switch( atno ){
			case	1 :
			case	2 :
				//  ３．（１＋２）課税標準額
				f1calq_3( sw );
				//  ４．消費税額（4%税率分）
				f1calq_4( sw );
				//	５．消費税額（3%税率分）
				f1calq_5( sw );
				//	５．消費税額（6.3%税率分）
				f1calq_5_2( sw );

			case	3 :
			case	4 :
				//  ６．（４＋５）消費税額合計
				f1calq_6( sw );

			case	5 :
			case	6 :	
				//	７．控除過大仕入額
				//	８．控除対象仕入税額
				f1calq_78( sw );
					
				//	９．返還等対価に係る税額
				f1calq_9( sw );

			case	7 :
			case	8 :
				//１０．貸倒に係る税額
				f1calq_10( sw );
				//１１．控除税額小計
				f1calq_11( sw );
				//１２．控除不足還付税額・１３．差引税額
				f1calq_1213( sw );
				//１４．合計差引税額
				f1calq_14( sw );
				// １５．譲渡割額
				f1calq_15( sw );

			default	:
				break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// 他表からの計算
//-----------------------------------------------------------------------------
// 引数	sw	：	
//-----------------------------------------------------------------------------
void CShinFhyo10::calq_by_OtherChange( int sw )
{
	ReadData( 0 );
	f1calq_all( 1, sw );
	WriteData();
}
