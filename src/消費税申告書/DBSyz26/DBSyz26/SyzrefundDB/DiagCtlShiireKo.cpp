#include "stdafx.h"
#include "DiagCtlShiire.h"
#include "DiagCtlShiireKo.h"

enum{
	ITM_KOMI    = 71, // ê≈çû
	ITM_NUKI,		  // ê≈î≤
	ITM_ZSHIIR1,
	ITM_ZSHIIR2,
	ITM_ZSHIIR3,
	ITM_ZKEIHI1,
	ITM_ZKEIHI2,
	ITM_ZKEIHI3,
	ITM_ZKOTEI1,
	ITM_ZKOTEI2,
	ITM_ZKOTEI3,
	ITM_ZSYOKE1,
	ITM_ZSYOKE2,
	ITM_ZSYOKE3,

	ITM_FKEIHI1,
	ITM_FKEIHI2,
	ITM_FKEIHI3,
	ITM_FKOTEI1,
	ITM_FKOTEI2,
	ITM_FKOTEI3,
	ITM_FSYOKE1,
	ITM_FSYOKE2,
	ITM_FSYOKE3,

	ITM_ONAME,		// ÅõÅõèäìæ

	ITM_OSHIIR1,
	ITM_OSHIIR2,
	ITM_OSHIIR3,
	ITM_OKEIHI1,
	ITM_OKEIHI2,
	ITM_OKEIHI3,
	ITM_OKOTEI1,
	ITM_OKOTEI2,
	ITM_OKOTEI3,
	ITM_OSYOKE1,
	ITM_OSYOKE2,
	ITM_OSYOKE3,

	ITM_KGOKEI,
	ITM_SKOJO,
};

CDiagCtlShiireKo::CDiagCtlShiireKo(CIcsdiagctrl& dg)
	: CDiagCtlShiire(dg)
{
	pair<int,CRect> list[] = {
		pair<int,CRect>( 71,CRect(MOVE_PREV,MOVE_PREV,72,73 )),
		pair<int,CRect>( 72,CRect(71,MOVE_PREV,73,73 )),
		pair<int,CRect>( 73,CRect(72,72,74,76 )),
		pair<int,CRect>( 74,CRect(73,NO_MOVE,76,77 )),
		pair<int,CRect>( 76,CRect(74,73,77,79 )),
		pair<int,CRect>( 77,CRect(76,74,79,80 )),
		pair<int,CRect>( 79,CRect(77,76,80,85 )),
		pair<int,CRect>( 80,CRect(79,77,85,86 )),
		pair<int,CRect>( 85,CRect(80,79,86,88 )),
		pair<int,CRect>( 86,CRect(85,80,88,89 )),
		pair<int,CRect>( 88,CRect(86,85,89,95 )),
		pair<int,CRect>( 89,CRect(88,86,94,96 )),
		pair<int,CRect>( 94,CRect(89,88,95,MOVE_NEXT )),
		pair<int,CRect>( 95,CRect(94,88,96,98 )),
		pair<int,CRect>( 96,CRect(95,89,98,99 )),
		pair<int,CRect>( 98,CRect(96,95,99,101 )),
		pair<int,CRect>( 99,CRect(98,96,101,102 )),
		pair<int,CRect>( 101,CRect(99,98,102,MOVE_NEXT )),
		pair<int,CRect>( 102,CRect(101,99,108,MOVE_NEXT )),
		pair<int,CRect>( 108,CRect(102,102,MOVE_NEXT,MOVE_NEXT )),
	};

	for(int i = 0;i<ARRAYCOUNT(list); i++)
		PositionMap.SetAt(list[i].first,list[i].second);

	RadioMap.SetAt(71,72);
	RadioMap.SetAt(72,71);
}

void CDiagCtlShiireKo::SetInitPos()
{
	if( pSnHeadData->Sn_Sign4&0x80 ){
		diag.SetPosition(syzdata->Getvol().KPSsgn&0x01 ? 71 : 72);
	}
	else{
		diag.SetPosition(ITM_ONAME);		
	}
}

void CDiagCtlShiireKo::DispInit()
{
	CDiagData DD;

	REFUNDVOL& refundvol = syzdata->Getvol();

	DD.data_check = refundvol.KPSsgn&0x01 ? 1 : 0;
	diag.SetData(71, DD);
	DD.data_check = refundvol.KPSsgn&0x01 ? 0 : 1;
	diag.SetData(72, DD);

	//SetInitPos();	// çÌèú harino ('14.03.07)

	CString kpspn[] = { MKString(&refundvol.KPSnam[0] ,2),
						MKString(&refundvol.KPSnam[2] ,2),
						MKString(&refundvol.KPSnam[4] ,2),
						MKString(&refundvol.KPSnam[6] ,2),
						MKString(&refundvol.KPSnam[8] ,2),
						MKString(&refundvol.KPSnam[10],2), 
						MKString(&refundvol.KPSnam[12],2),
						MKString(&refundvol.KPSnam[14],2), 
					  };

	int max = sizeof(kpspn)/sizeof(kpspn[0]);

	for(int i = 0;i<max;i++){
		if( kpspn[i] == CString("Å@")){
			DD.data_edit += "\r\n";
			continue;
		}else
			DD.data_edit += kpspn[i];

		int mojicnt = 0;
		for(int j = i + 1; j < max && !mojicnt ;j++)
			mojicnt += kpspn[j].GetLength();
	
		if( mojicnt ) DD.data_edit += "\r\n";
	}
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_ONAME, TRUE );
	else diag.SetData(ITM_ONAME, DD );

	MnyBufMap.SetAt( ITM_ZSHIIR1,syzdata->GetRec(0).KPSvl1 );
	MnyBufMap.SetAt( ITM_ZKEIHI1,syzdata->GetRec(1).KPSvl1 );
	MnyBufMap.SetAt( ITM_ZKOTEI1,syzdata->GetRec(2).KPSvl1 );
	MnyBufMap.SetAt( ITM_ZSYOKE1,syzdata->GetRec(3).KPSvl1 );
	MnyBufMap.SetAt( ITM_FKEIHI1,syzdata->GetRec(4).KPSvl1 );
	MnyBufMap.SetAt( ITM_FKOTEI1,syzdata->GetRec(5).KPSvl1 );
	MnyBufMap.SetAt( ITM_FSYOKE1,syzdata->GetRec(6).KPSvl1 );
	MnyBufMap.SetAt( ITM_OSHIIR1,syzdata->GetRec(7).KPSvl1 );
	MnyBufMap.SetAt( ITM_OKEIHI1,syzdata->GetRec(8).KPSvl1 );
	MnyBufMap.SetAt( ITM_OKOTEI1,syzdata->GetRec(9).KPSvl1 );
	MnyBufMap.SetAt( ITM_OSYOKE1,syzdata->GetRec(10).KPSvl1 );

	MnyBufMap.SetAt( ITM_ZSHIIR2,syzdata->GetRec(0).KPSvl2 );
	MnyBufMap.SetAt( ITM_ZKEIHI2,syzdata->GetRec(1).KPSvl2 );
	MnyBufMap.SetAt( ITM_ZKOTEI2,syzdata->GetRec(2).KPSvl2 );
	MnyBufMap.SetAt( ITM_ZSYOKE2,syzdata->GetRec(3).KPSvl2 );
	MnyBufMap.SetAt( ITM_FKEIHI2,syzdata->GetRec(4).KPSvl2 );
	MnyBufMap.SetAt( ITM_FKOTEI2,syzdata->GetRec(5).KPSvl2 );
	MnyBufMap.SetAt( ITM_FSYOKE2,syzdata->GetRec(6).KPSvl2 );
	MnyBufMap.SetAt( ITM_OSHIIR2,syzdata->GetRec(7).KPSvl2 );
	MnyBufMap.SetAt( ITM_OKEIHI2,syzdata->GetRec(8).KPSvl2 );
	MnyBufMap.SetAt( ITM_OKOTEI2,syzdata->GetRec(9).KPSvl2 );
	MnyBufMap.SetAt( ITM_OSYOKE2,syzdata->GetRec(10).KPSvl2 );

	MnyBufMap.SetAt( ITM_ZSHIIR3,syzdata->GetRec(0).KPSvl3 );
	MnyBufMap.SetAt( ITM_ZKEIHI3,syzdata->GetRec(1).KPSvl3 );
	MnyBufMap.SetAt( ITM_ZKOTEI3,syzdata->GetRec(2).KPSvl3 );
	MnyBufMap.SetAt( ITM_ZSYOKE3,syzdata->GetRec(3).KPSvl3 );
	MnyBufMap.SetAt( ITM_FKEIHI3,syzdata->GetRec(4).KPSvl3 );
	MnyBufMap.SetAt( ITM_FKOTEI3,syzdata->GetRec(5).KPSvl3 );
	MnyBufMap.SetAt( ITM_FSYOKE3,syzdata->GetRec(6).KPSvl3 );
	MnyBufMap.SetAt( ITM_OSHIIR3,syzdata->GetRec(7).KPSvl3 );
	MnyBufMap.SetAt( ITM_OKEIHI3,syzdata->GetRec(8).KPSvl3 );
	MnyBufMap.SetAt( ITM_OKOTEI3,syzdata->GetRec(9).KPSvl3 );
	MnyBufMap.SetAt( ITM_OSYOKE3,syzdata->GetRec(10).KPSvl3 );

	MnyBufMap.SetAt( ITM_KGOKEI, syzdata->GetRec(11).KPSvl3 );

	ZitsugakuPair = pair<char*,char*>(syzdata->GetRec(12).KPSvl3,syzdata->GetRec(12).KPSvl4);
	MnyBufMap.SetAt( ITM_SKOJO, refundvol.KPSstx ? ZitsugakuPair.second : ZitsugakuPair.first );
//	SetAttribMod( ITM_SKOJO, refundvol.KPSstx ? ZICOLOR : EDCOLOR, DIAG_MDFY_EDIT );
	
	CalqDisp();

//Åö//[12'09.19]///
	BOOL	flag;
	if( !(pSnHeadData->Sn_Sign4&0x80) || (pSnHeadData->Sn_Sign4&0x01) ){
		flag = FALSE;
	}
	else{
		flag = TRUE;
	}

	int idlist[] = {
		ITM_KOMI, 
		ITM_NUKI,
		ITM_ZSHIIR1,
		ITM_ZSHIIR2,
		ITM_ZKEIHI1,
		ITM_ZKEIHI2,
		ITM_ZKOTEI1,
		ITM_ZKOTEI2,
		ITM_FKEIHI1,
		ITM_FKEIHI2,
		ITM_FKOTEI1,
		ITM_FKOTEI2,
		ITM_OSHIIR1,
		ITM_OSHIIR2,
		ITM_OKEIHI1,
		ITM_OKEIHI2,
		ITM_OKOTEI1,
		ITM_OKOTEI2,
	};

	for(int i = 0;i<ARRAYCOUNT(idlist);i++)
		SetAttribMod(idlist[i], flag, !(pSnHeadData->Sn_Sign4&0x80) );

	int flg = 0;
	if( pSnHeadData->Sn_Sign4&0x01){
		SetAttribMod(ITM_ONAME, GetBkcolor() , DIAG_MDFY_READONLY);
		SetAttribMod(ITM_SKOJO, GetBkcolor() , DIAG_MDFY_READONLY);

	}
	else{
		SetAttribMod(ITM_ONAME, EDCOLOR , DIAG_MDFY_EDIT);
		if( refundvol.KPSstx ){
			flg = 1;
			SetAttribMod(ITM_SKOJO, ZICOLOR , DIAG_MDFY_EDIT);
		}
		else{
			SetAttribMod(ITM_SKOJO, EDCOLOR , DIAG_MDFY_EDIT);
		}
	}
//	SetAttribMod(ITM_ONAME, flg ? EDCOLOR : GetBkcolor() , flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY);
	
	///////////////////
}

void CDiagCtlShiireKo::Delete(int index)
{
	if( index == ITM_SKOJO ){
		ZitsugakuClear( index, syzdata->Getvol().KPSstx );
		return;
	}
	diag.DataClear(index,TRUE);
	EditOff(index);
}

extern void SplitCmntToArray(CStringArray& CStrArray,CString cmnt);

void CDiagCtlShiireKo::EditOff(int index)
{
	CDiagData DD;
	diag.GetData(index,DD);

	REFUNDVOL& refundvol = syzdata->Getvol();

	switch(index){
	case ITM_KOMI:
		if(DD.data_check)	refundvol.KPSsgn |=  0x01;
		else				refundvol.KPSsgn &= ~0x01;
		TRACE("ê≈çû\n");break;
	case ITM_NUKI:
		if(!DD.data_check)	refundvol.KPSsgn |=  0x01;
		else				refundvol.KPSsgn &= ~0x01;
		TRACE("ê≈î≤\n");break;
	case ITM_ONAME:
		{	CStringArray CStrArray;
			SplitCmntToArray(CStrArray,DD.data_edit);

			CString edit;
			for(int i = 0;i< CStrArray.GetSize() ;i++){
				edit += CStrArray[i];
				if( !CStrArray[i].GetLength() ) edit += "Å@";
			}
			StringToBuf(edit,refundvol.KPSnam,16);
		}
		TRACE("ñºèÃ\n");
		break;
	default:
		{	char edit_val[6];
			ConvEditVal( edit_val, DD.data_val, 1 );
			
			char* money = GetMnyBuf(index);

//-- '15.02.03 --
//			if( index == ITM_SKOJO )
//				money = Zitsugaku( index, refundvol.KPSstx, edit_val );
//---------------
			if( index == ITM_SKOJO ){
				money = Zitsugaku( 1, index, refundvol.KPSstx, edit_val );
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

void CDiagCtlShiireKo::CalqDisp()
{
	SumVal(MnyBufMap[ITM_ZSYOKE1],0,2, (char *REFUNDREC::*)&REFUNDREC::KPSvl1 );
	SumVal(MnyBufMap[ITM_ZSYOKE2],0,2, (char *REFUNDREC::*)&REFUNDREC::KPSvl2 );

	SumVal(MnyBufMap[ITM_FSYOKE1],4,5, (char *REFUNDREC::*)&REFUNDREC::KPSvl1 );
	SumVal(MnyBufMap[ITM_FSYOKE2],4,5, (char *REFUNDREC::*)&REFUNDREC::KPSvl2 );

	SumVal(MnyBufMap[ITM_OSYOKE1],7,9, (char *REFUNDREC::*)&REFUNDREC::KPSvl1 );
	SumVal(MnyBufMap[ITM_OSYOKE2],7,9, (char *REFUNDREC::*)&REFUNDREC::KPSvl2 );
	
	int const line_max = 11;

	for(int line = 0;line<line_max;line++ ){
		REFUNDREC& meisai =  syzdata->GetRec(line);
		arith.l_sub(meisai.KPSvl3,meisai.KPSvl1,meisai.KPSvl2);
	}

	arith.l_clear( MnyBufMap[ITM_KGOKEI] );
	arith.l_add( MnyBufMap[ITM_KGOKEI], MnyBufMap[ITM_KGOKEI], MnyBufMap[ITM_ZSYOKE3] );
	arith.l_add( MnyBufMap[ITM_KGOKEI], MnyBufMap[ITM_KGOKEI], MnyBufMap[ITM_FSYOKE3] );
	arith.l_add( MnyBufMap[ITM_KGOKEI], MnyBufMap[ITM_KGOKEI], MnyBufMap[ITM_OSYOKE3] );

//-- '15.02.03 --
//	CalqZei(syzdata->GetRec(12).KPSvl3,MnyBufMap[ITM_KGOKEI],syzdata->Getvol().KPSsgn&0x01);
//---------------
	if( pUtil->GetKazeihoushiki(pZmSub->zvol) != ID_ICSSH_GENSOKU ){
		CalqZei(syzdata->GetRec(12).KPSvl3,MnyBufMap[ITM_KGOKEI],syzdata->Getvol().KPSsgn&0x01);
	}
//---------------
	CDiagCtlShiire::CalqDisp();
}