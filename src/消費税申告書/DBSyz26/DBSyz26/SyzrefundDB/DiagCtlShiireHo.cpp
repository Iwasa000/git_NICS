#include "stdafx.h"
#include "DiagCtlShiire.h"
#include "DiagCtlShiireHo.h"

enum{
	ITM_PLZEIKOMI = 59, // komi
	ITM_PLZEINUKI,		// nuki

	ITM_PLSHIIRE1,
	ITM_PLSHIIRE2,
	ITM_PLSHIIRE3,
	ITM_PLHANKAN1,
	ITM_PLHANKAN2,
	ITM_PLHANKAN3,
	ITM_PLEIGYOU1,
	ITM_PLEIGYOU2,
	ITM_PLEIGYOU3,
	ITM_PLSONOTA1,
	ITM_PLSONOTA2,
	ITM_PLSONOTA3,
	ITM_PLSYOKEI1,
	ITM_PLSYOKEI2,
	ITM_PLSYOKEI3,

	ITM_BSZEIKOMI, // komi
	ITM_BSZEINUKI, // nuki

	ITM_BSKOTEIS1,
	ITM_BSKOTEIS2,
	ITM_BSKOTEIS3,
	ITM_BSKURINS1,
	ITM_BSKURINS2,
	ITM_BSKURINS3,
	ITM_BSSONOTA1,
	ITM_BSSONOTA2,
	ITM_BSSONOTA3,
	ITM_BSSYOKEI1,
	ITM_BSSYOKEI2,
	ITM_BSSYOKEI3,
	ITM_KGOKEI,
};

CDiagCtlShiireHo::CDiagCtlShiireHo(CIcsdiagctrl& dg)
	: CDiagCtlShiire(dg)
{
	pair<int,CRect> list[] = {
		pair<int,CRect>( 59,CRect(MOVE_PREV,MOVE_PREV,60,61 )),
		pair<int,CRect>( 60,CRect(59,MOVE_PREV,61,61 )),
		pair<int,CRect>( 61,CRect(60,60,62,64 )),
		pair<int,CRect>( 62,CRect(61,NO_MOVE,64,65 )),
		pair<int,CRect>( 64,CRect(62,61,65,67 )),
		pair<int,CRect>( 65,CRect(64,62,67,68 )),
		pair<int,CRect>( 67,CRect(65,64,68,70 )),
		pair<int,CRect>( 68,CRect(67,65,70,71 )),
		pair<int,CRect>( 70,CRect(68,67,71,76 )),
		pair<int,CRect>( 71,CRect(70,68,76,79 )),
		pair<int,CRect>( 76,CRect(71,70,77,78 )),
		pair<int,CRect>( 77,CRect(76,70,78,78 )),
		pair<int,CRect>( 78,CRect(77,77,79,81 )),
		pair<int,CRect>( 79,CRect(78,71,81,82 )),
		pair<int,CRect>( 81,CRect(79,78,82,84 )),
		pair<int,CRect>( 82,CRect(81,79,84,85 )),
		pair<int,CRect>( 84,CRect(82,81,85,MOVE_NEXT )),
		pair<int,CRect>( 85,CRect(84,82,90,MOVE_NEXT )),
		pair<int,CRect>( 90,CRect(85,85,MOVE_NEXT,MOVE_NEXT )),
	};

	for(int i = 0;i<ARRAYCOUNT(list); i++)
		PositionMap.SetAt(list[i].first,list[i].second);

	RadioMap.SetAt(59,60);
	RadioMap.SetAt(60,59);

	RadioMap.SetAt(76,77);
	RadioMap.SetAt(77,76);
}

void CDiagCtlShiireHo::SetInitPos()
{
	if( pSnHeadData->Sn_Sign4&0x80 ){
		diag.SetPosition(syzdata->Getvol().KPSsgn&0x01 ? 59 : 60);
	}
	else{
		diag.SetPosition(syzdata->Getvol().KPSsgn&0x10 ? 3: 4);	
	}
}

void CDiagCtlShiireHo::DispInit()
{
	CDiagData DD;

	REFUNDVOL& refundvol = syzdata->Getvol();

	DD.data_check = refundvol.KPSsgn&0x01 ? 1 : 0;
	diag.SetData(59, DD);
	DD.data_check = refundvol.KPSsgn&0x01 ? 0 : 1;
	diag.SetData(60, DD);

	DD.data_check = refundvol.KPSsgn&0x10 ? 1 : 0;
	diag.SetData(76, DD);
	DD.data_check = refundvol.KPSsgn&0x10 ? 0 : 1;
	diag.SetData(77, DD);

	//SetInitPos();	// íœ harino ('14.03.07)

	MnyBufMap.SetAt( ITM_PLSHIIRE1,syzdata->GetRec(0).KPSvl1 );
	MnyBufMap.SetAt( ITM_PLHANKAN1,syzdata->GetRec(1).KPSvl1 );
	MnyBufMap.SetAt( ITM_PLEIGYOU1,syzdata->GetRec(2).KPSvl1 );
	MnyBufMap.SetAt( ITM_PLSONOTA1,syzdata->GetRec(3).KPSvl1 );
	MnyBufMap.SetAt( ITM_PLSYOKEI1,syzdata->GetRec(4).KPSvl1 );
	MnyBufMap.SetAt( ITM_BSKOTEIS1,syzdata->GetRec(5).KPSvl1 );
	MnyBufMap.SetAt( ITM_BSKURINS1,syzdata->GetRec(6).KPSvl1 );
	MnyBufMap.SetAt( ITM_BSSONOTA1,syzdata->GetRec(7).KPSvl1 );
	MnyBufMap.SetAt( ITM_BSSYOKEI1,syzdata->GetRec(8).KPSvl1 );

	MnyBufMap.SetAt( ITM_PLSHIIRE2,syzdata->GetRec(0).KPSvl2 );
	MnyBufMap.SetAt( ITM_PLHANKAN2,syzdata->GetRec(1).KPSvl2 );
	MnyBufMap.SetAt( ITM_PLEIGYOU2,syzdata->GetRec(2).KPSvl2 );
	MnyBufMap.SetAt( ITM_PLSONOTA2,syzdata->GetRec(3).KPSvl2 );
	MnyBufMap.SetAt( ITM_PLSYOKEI2,syzdata->GetRec(4).KPSvl2 );
	MnyBufMap.SetAt( ITM_BSKOTEIS2,syzdata->GetRec(5).KPSvl2 );
	MnyBufMap.SetAt( ITM_BSKURINS2,syzdata->GetRec(6).KPSvl2 );
	MnyBufMap.SetAt( ITM_BSSONOTA2,syzdata->GetRec(7).KPSvl2 );
	MnyBufMap.SetAt( ITM_BSSYOKEI2,syzdata->GetRec(8).KPSvl2 );

	MnyBufMap.SetAt( ITM_PLSHIIRE3,syzdata->GetRec(0).KPSvl3 );
	MnyBufMap.SetAt( ITM_PLHANKAN3,syzdata->GetRec(1).KPSvl3 );
	MnyBufMap.SetAt( ITM_PLEIGYOU3,syzdata->GetRec(2).KPSvl3 );
	MnyBufMap.SetAt( ITM_PLSONOTA3,syzdata->GetRec(3).KPSvl3 );
	MnyBufMap.SetAt( ITM_PLSYOKEI3,syzdata->GetRec(4).KPSvl3 );
	MnyBufMap.SetAt( ITM_BSKOTEIS3,syzdata->GetRec(5).KPSvl3 );
	MnyBufMap.SetAt( ITM_BSKURINS3,syzdata->GetRec(6).KPSvl3 );
	MnyBufMap.SetAt( ITM_BSSONOTA3,syzdata->GetRec(7).KPSvl3 );
	MnyBufMap.SetAt( ITM_BSSYOKEI3,syzdata->GetRec(8).KPSvl3 );
	
	ZitsugakuPair = pair<char*,char*>( syzdata->GetRec(9).KPSvl3,syzdata->GetRec(9).KPSvl4);
	MnyBufMap.SetAt( ITM_KGOKEI, refundvol.KPSstx ? ZitsugakuPair.second : ZitsugakuPair.first );

//	if( !(pSnHeadData->Sn_Sign4&0x80) ){
//		refundvol.KPSstx = 0x00;
//	}
	
	CalqDisp();

//	if( !arith.l_test( syzdata->GetRec(9).KPSvl3 ) && !Arith.l_test( syzdata->GetRec(9).KPSvl4 ){
//		refundvol.KPSstx = 0x00;
//	}

	SetAttribMod( ITM_KGOKEI, refundvol.KPSstx ? ZICOLOR : EDCOLOR, DIAG_MDFY_EDIT );

//š//[12'09.19]///
	BOOL	flag;
	if( !(pSnHeadData->Sn_Sign4&0x80) || (pSnHeadData->Sn_Sign4&0x01) ){
		flag = FALSE;
	}
	else{
		flag = TRUE;
	}

	int idlist[] = {
		ITM_PLZEIKOMI,
		ITM_PLZEINUKI,
		ITM_PLSHIIRE1,
		ITM_PLSHIIRE2,
		ITM_PLHANKAN1,
		ITM_PLHANKAN2,
		ITM_PLEIGYOU1,
		ITM_PLEIGYOU2,
		ITM_PLSONOTA1,
		ITM_PLSONOTA2,
		ITM_BSZEIKOMI,
		ITM_BSZEINUKI,
		ITM_BSKOTEIS1,
		ITM_BSKOTEIS2,
		ITM_BSKURINS1,
		ITM_BSKURINS2,
		ITM_BSSONOTA1,
		ITM_BSSONOTA2,
		ITM_KGOKEI,
	};

	for(int i = 0;i<ARRAYCOUNT(idlist);i++){
		if( idlist[i] == 90 && flag == TRUE ) continue;
		SetAttribMod(idlist[i], flag, !(pSnHeadData->Sn_Sign4&0x80) );
	}
}

void CDiagCtlShiireHo::Delete(int index)
{
	if( index == ITM_KGOKEI ){
		ZitsugakuClear( index, syzdata->Getvol().KPSstx );
		return;
	}
	diag.DataClear(index,TRUE);
	EditOff(index);
}

void CDiagCtlShiireHo::EditOff(int index)
{
	CDiagData DD;
	diag.GetData(index,DD);

	REFUNDVOL& refundvol = syzdata->Getvol();

	switch(index){
	case ITM_PLZEIKOMI:
		if(DD.data_check)
			refundvol.KPSsgn |= 0x01;
		else
			refundvol.KPSsgn &= ~0x01;
		TRACE("‘¹‰vÅž\n");break;
	case ITM_PLZEINUKI:
		if(!DD.data_check)
			refundvol.KPSsgn |= 0x01;
		else
			refundvol.KPSsgn &= ~0x01;
		TRACE("‘¹‰vÅ”²\n");break;
	case ITM_BSZEIKOMI:
		if(DD.data_check)
			refundvol.KPSsgn |= 0x10;
		else
			refundvol.KPSsgn &= ~0x10;
		TRACE("‘ÝŽØÅž\n");break;
	case ITM_BSZEINUKI:
		if(!DD.data_check)
			refundvol.KPSsgn |= 0x10;
		else
			refundvol.KPSsgn &= ~0x10;
		TRACE("‘ÝŽØÅ”²\n");break;
	default:
		{	char edit_val[6];
			ConvEditVal( edit_val, DD.data_val, 1 );

			char* money = GetMnyBuf(index);

//-- '15.02.03 --
//			if( index == ITM_KGOKEI )
//				money = Zitsugaku( index, refundvol.KPSstx, edit_val );
//---------------
			if( index == ITM_KGOKEI ){
				money = Zitsugaku( 0, index, refundvol.KPSstx, edit_val );
			}
//---------------

			if(money)	memmove(money, edit_val, 6);
			else		TRACE("index error!");
		}
		break;
	}

	CalqDisp();
	TRACE("index = %d\n",index);
}

void CDiagCtlShiireHo::CalqDisp()
{
	REFUNDVOL& refundvol = syzdata->Getvol();
	
	SumVal(MnyBufMap[ITM_PLSYOKEI1],0,3, (char *REFUNDREC::*)&REFUNDREC::KPSvl1 );
	SumVal(MnyBufMap[ITM_PLSYOKEI2],0,3, (char *REFUNDREC::*)&REFUNDREC::KPSvl2 );
	SumVal(MnyBufMap[ITM_BSSYOKEI1],5,7, (char *REFUNDREC::*)&REFUNDREC::KPSvl1 );
	SumVal(MnyBufMap[ITM_BSSYOKEI2],5,7, (char *REFUNDREC::*)&REFUNDREC::KPSvl2 );

	int const line_max = 9;

	for(int line = 0;line<line_max;line++ ){
		REFUNDREC& meisai =  syzdata->GetRec(line);
		arith.l_sub(meisai.KPSvl3,meisai.KPSvl1,meisai.KPSvl2);
	}

	char PL[6]={0},BS[6]={0};

//	CalqZei(PL,MnyBufMap[ITM_PLSYOKEI3],syzdata->Getvol().KPSsgn&0x01);
//	CalqZei(BS,MnyBufMap[ITM_BSSYOKEI3],syzdata->Getvol().KPSsgn&0x10);

//-- '15.02.03 --
//	if(	syzdata->Getvol().KPSsgn&0x01 ){//ž‚Ý
//		m_Util.percent( PL, MnyBufMap[ITM_PLSYOKEI3] ,9, 63, 0 );
//	}
//	else{
//		m_Util.percent( PL, MnyBufMap[ITM_PLSYOKEI3], 1, 63, 0 );
//	}
//	if( syzdata->Getvol().KPSsgn&0x10 ){
//		m_Util.percent( BS, MnyBufMap[ITM_BSSYOKEI3], 9, 63, 0 );
//	}
//	else{
//		m_Util.percent( BS, MnyBufMap[ITM_BSSYOKEI3], 1, 63, 0 );
//	}
//
//	arith.l_add( syzdata->GetRec(9).KPSvl3, PL, BS);
//	l_cut1000( syzdata->GetRec(9).KPSvl3 );
//---------------
	if( m_Util.GetKazeihoushiki(pZmSub->zvol) != ID_ICSSH_GENSOKU ){
		if(	syzdata->Getvol().KPSsgn&0x01 ){//ž‚Ý
			m_Util.percent( PL, MnyBufMap[ITM_PLSYOKEI3] ,9, 63, 0 );
		}
		else{
			m_Util.percent( PL, MnyBufMap[ITM_PLSYOKEI3], 1, 63, 0 );
		}
		if( syzdata->Getvol().KPSsgn&0x10 ){
			m_Util.percent( BS, MnyBufMap[ITM_BSSYOKEI3], 9, 63, 0 );
		}
		else{
			m_Util.percent( BS, MnyBufMap[ITM_BSSYOKEI3], 1, 63, 0 );
		}

		arith.l_add( syzdata->GetRec(9).KPSvl3, PL, BS);
		l_cut1000( syzdata->GetRec(9).KPSvl3 );
	}
//---------------

	CDiagCtlShiire::CalqDisp();
}