#include "stdafx.h"
#include "DiagCtlTokusyu.h"

enum{
	ITM_NEN     = 1,
	ITM_TOKUSYU = 6,
};

CDiagCtlTokusyu::CDiagCtlTokusyu(CIcsdiagctrl& dg,int flg)
	: CDiagControlBase(dg)
{
	pair<int,CRect> list[] = {
		pair<int,CRect>( 1,CRect(MOVE_PREV,MOVE_PREV,6,6 )),
		pair<int,CRect>( 6,CRect(1,1,MOVE_NEXT,MOVE_NEXT )),
	};

	for(int i = 0;i<ARRAYCOUNT(list); i++)
		PositionMap.SetAt(list[i].first,list[i].second);

    if(flg){
		SetAttribMod(1, GetBkcolor(), DIAG_MDFY_TRANSPARENT);
		SetAttribMod(2, GetBkcolor(), DIAG_MDFY_TRANSPARENT);
	}
	SetZenHan(6);



}
void CDiagCtlTokusyu::DispInit()
{
	CDiagData DD;

	REFUNDVOL& refundvol = syzdata->Getvol();

	CString kpspn[] = { MKString(refundvol.KPSpn1,KPSpLN),
						MKString(refundvol.KPSpn2,KPSpLN),
						MKString(refundvol.KPSpn3,KPSpLN),
						MKString(refundvol.KPSpn4,KPSpLN),
						MKString(refundvol.KPSpn5,KPSpLN),
						MKString(refundvol.KPSpn6,KPSpLN), 
					  };

	int max = sizeof(kpspn)/sizeof(kpspn[0]);

	for(int i = 0;i<max;i++){

		DD.data_edit += kpspn[i];

		int mojicnt = 0;
		for(int j = i + 1; j < max && !mojicnt ;j++)
			mojicnt += kpspn[j].GetLength();
	
		if( mojicnt ) DD.data_edit += "\r\n";
	}

	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_TOKUSYU, TRUE );
	else diag.SetData(ITM_TOKUSYU, DD );

	DD.data_day[0] = refundvol.KPSnen;
	diag.SetData(ITM_NEN, DD );

//š//[12'09.19]///
	BOOL	flag;
	if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
	else													flag = TRUE;
	if( refundvol.KPType == 2 )	SetAttribMod(ITM_NEN,flag);
	SetAttribMod(ITM_TOKUSYU,flag);
///////////////////

}

void SplitCmntToArray(CStringArray& CStrArray,CString cmnt)
{
	while(cmnt.GetLength()){
		CString AddStr;
		int kaigyo = cmnt.Find("\r\n");
		if( kaigyo != -1 ){
			AddStr = cmnt.Left(kaigyo);
			cmnt = cmnt.Right(cmnt.GetLength() - kaigyo - 2 );
		}else{
			AddStr = cmnt;
			cmnt.Empty();
		}

		CStrArray.Add(AddStr);
		TRACE("%s\n",AddStr);
	}
}

void CDiagCtlTokusyu::EditOff(int index)
{
	CDiagData DD;
	diag.GetData( index, DD );
	REFUNDVOL& refundvol = syzdata->Getvol();

	switch(index){
	case ITM_NEN:
		refundvol.KPSnen = DD.data_day[0];
		TRACE("”N\n");
		break;
	case ITM_TOKUSYU:
		{	CStringArray _array;
			SplitCmntToArray(_array,DD.data_edit);
			char *buf = 0;
			for(int i= 0;i<6;i++){
				switch(i){
				default: buf = refundvol.KPSpn1; break;
				case 1:  buf = refundvol.KPSpn2; break;
				case 2:  buf = refundvol.KPSpn3; break;
				case 3:  buf = refundvol.KPSpn4; break;
				case 4:  buf = refundvol.KPSpn5; break;
				case 5:  buf = refundvol.KPSpn6; break;
				}
				if( i < _array.GetSize() ) 
					StringToBuf(_array[i],buf,KPSpLN);
				else
					memset(buf,0,KPSpLN);
			}
		}
		TRACE("“ÁŽêŽ–î");
		break;
	}
	TRACE("index = %d\n",index);
}
void CDiagCtlTokusyu::SetPos()
{
	if( !(pSnHeadData->Sn_Sign4&0x80) ){
		diag.SetPosition(6);		
	}
}