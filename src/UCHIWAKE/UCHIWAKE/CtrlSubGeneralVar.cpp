#include "StdAfx.h"
#include "CtrlSubGeneralVar.h"


// CCtrlSubGeneralVar実装
CCtrlSubGeneralVar::CCtrlSubGeneralVar(const CdbUcInfSub& rc)
{
	bKinyuLine			= rc.m_GeneralVar[0] & BIT_D0;
	bDfltRendoKikan		= rc.m_GeneralVar[0] & BIT_D1;
// midori 150892 del -->
	//bMakeUpMethod		= rc.m_GeneralVar[0] & BIT_D2;
// midori 150892 del <--
	bAddHokan			= rc.m_GeneralVar[0] & BIT_D3;
	bOutKamokuRow		= rc.m_GeneralVar[0] & BIT_D4;
	bIkkatuOutZero		= rc.m_GeneralVar[0] & BIT_D5;
	bNameKei			= rc.m_GeneralVar[0] & BIT_D6;
	bPrintZeroData		= rc.m_GeneralVar[0] & BIT_D7;

	bPrintNullData		= rc.m_GeneralVar[1] & BIT_D0;
	bPrintHideZero		= rc.m_GeneralVar[1] & BIT_D1;
	bUseAdvSetting		= rc.m_GeneralVar[1] & BIT_D2;

// midori 152135,152136 add -->
	bKamokuSort			= rc.m_GeneralVar[2] & BIT_D1;
// midori 152135,152136 add <--
// midori 190505 add -->
// midori 157042 del -->
	//bKamokuTitle		= rc.m_GeneralVar[2] & BIT_D2;
// midori 157042 del <--
// midori 157099,157119 add -->
	bKamokuTitle		= rc.m_GeneralVar[2] & BIT_D2;
// midori 157099,157119 add <--
	bPrnHoho1			= rc.m_GeneralVar[2] & BIT_D4;
	bPrnHoho2			= rc.m_GeneralVar[2] & BIT_D5;
// midori 20/10/08u del -->
	//bPrnHoho3			= rc.m_GeneralVar[2] & BIT_D6;
// midori 20/10/08u del <--
// midori 190505 add <--
// No.200602 add -->
	bKanryakuSw			= rc.m_GeneralVar[2] & BIT_D6;
// No.200602 add <--
}

//***************************************************************************
//	セッタ
//　このクラス自体ではEdit,Updateは発行しないので、呼び出し元で行うこと！
//
//	IN:		CdbUcInfSub&
//
//	RET:	なし 	
//***************************************************************************
void CCtrlSubGeneralVar::Set(CdbUcInfSub& rc)
{
	// m_GeneralVar[0]　※BIT_D0～BIT_D7まで！
	if( bKinyuLine )			rc.m_GeneralVar[0] |= BIT_D0;	else rc.m_GeneralVar[0] &= ~BIT_D0;
	if( bDfltRendoKikan )		rc.m_GeneralVar[0] |= BIT_D1;	else rc.m_GeneralVar[0] &= ~BIT_D1;
// midori 150892 del -->
	//if( bMakeUpMethod )			rc.m_GeneralVar[0] |= BIT_D2;	else rc.m_GeneralVar[0] &= ~BIT_D2;
// midori 150892 del <--
	if( bAddHokan )				rc.m_GeneralVar[0] |= BIT_D3;	else rc.m_GeneralVar[0] &= ~BIT_D3;
	if( bOutKamokuRow )			rc.m_GeneralVar[0] |= BIT_D4;	else rc.m_GeneralVar[0] &= ~BIT_D4;
	if( bIkkatuOutZero )		rc.m_GeneralVar[0] |= BIT_D5;	else rc.m_GeneralVar[0] &= ~BIT_D5;
	if( bNameKei )				rc.m_GeneralVar[0] |= BIT_D6;	else rc.m_GeneralVar[0] &= ~BIT_D6;
	if( bPrintZeroData )		rc.m_GeneralVar[0] |= BIT_D7;	else rc.m_GeneralVar[0] &= ~BIT_D7;

	if( bPrintNullData )		rc.m_GeneralVar[1] |= BIT_D0;	else rc.m_GeneralVar[1] &= ~BIT_D0;
	if( bPrintHideZero )		rc.m_GeneralVar[1] |= BIT_D1;	else rc.m_GeneralVar[1] &= ~BIT_D1;
	if( bUseAdvSetting )		rc.m_GeneralVar[1] |= BIT_D2;	else rc.m_GeneralVar[1] &= ~BIT_D2;
// midori 152135,152136 add -->
	if( bKamokuSort )			rc.m_GeneralVar[2] |= BIT_D1;	else rc.m_GeneralVar[2] &= ~BIT_D1;
// midori 152135,152136 add <--
// midori 190505 add -->
// midori 157042 del -->
	//if( bKamokuTitle )			rc.m_GeneralVar[2] |= BIT_D2;	else rc.m_GeneralVar[2] &= ~BIT_D2;
// midori 157042 del <--
// midori 157099,157119 add -->
	if( bKamokuTitle )			rc.m_GeneralVar[2] |= BIT_D2;	else rc.m_GeneralVar[2] &= ~BIT_D2;
// midori 157099,157119 add <--
	if( bPrnHoho1 )				rc.m_GeneralVar[2] |= BIT_D4;	else rc.m_GeneralVar[2] &= ~BIT_D4;
	if( bPrnHoho2 )				rc.m_GeneralVar[2] |= BIT_D5;	else rc.m_GeneralVar[2] &= ~BIT_D5;
// midori 20/10/08u del -->
	//if( bPrnHoho3 )				rc.m_GeneralVar[2] |= BIT_D6;	else rc.m_GeneralVar[2] &= ~BIT_D6;
// midori 20/10/08u del <--
// midori 190505 add <--
// No.200602 add -->
	if( bKanryakuSw )			rc.m_GeneralVar[2] |= BIT_D6;	else rc.m_GeneralVar[2] &= ~BIT_D6;
// No.200602 add <--
}
