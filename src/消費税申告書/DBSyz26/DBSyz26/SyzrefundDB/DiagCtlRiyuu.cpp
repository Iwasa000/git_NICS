#include "stdafx.h"
#include "DiagCtlRiyuu.h"

enum{
	ITM_WARIAI  = 1,
	ITM_SETSUBI = 3,
	ITM_SONOTA  = 5,
	ITM_RIYUU   = 8,
};

CDiagCtlRiyuu::CDiagCtlRiyuu(CIcsdiagctrl& dg)
	: CDiagControlBase(dg)
{
	pair<int,CRect> list[] = {
		pair<int,CRect>( 1,CRect(MOVE_PREV,MOVE_PREV,3,3 )),
		pair<int,CRect>( 3,CRect(1,1,5,MOVE_NEXT )),
		pair<int,CRect>( 5,CRect(3,NO_MOVE,8,MOVE_NEXT )),
		pair<int,CRect>( 8,CRect(5,NO_MOVE,MOVE_NEXT,MOVE_NEXT )),
	};

	for(int i = 0;i< ARRAYCOUNT(list); i++)
		PositionMap.SetAt(list[i].first,list[i].second);

	SetAttribMod( 8,  GetBkcolor() , DIAG_MDFY_READONLY);
	SetZenHan(8);
}

void CDiagCtlRiyuu::SetInitPos()
{
	diag.SetPosition(1);
}

void CDiagCtlRiyuu::DispInit()
{
	CDiagData DD;

	REFUNDVOL& refundvol = syzdata->Getvol();

	DD.data_check = refundvol.KPRsgn&0x01 ? 1 : 0;
	diag.SetData(ITM_WARIAI, DD);
	DD.data_check = refundvol.KPRsgn&0x02 ? 1 : 0;
	diag.SetData(ITM_SETSUBI, DD);
	DD.data_check = refundvol.KPRsgn&0x04 ? 1 : 0;
	diag.SetData(ITM_SONOTA, DD);

	int flg = refundvol.KPRsgn&0x04;
	SetAttribMod( ITM_RIYUU, flg ? EDCOLOR : GetBkcolor() , flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY);

	DD.data_edit = 	flg ? ZyougedanBufToString(refundvol.KPRrup,refundvol.KPRrlw,KPRrLN) : "";
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_RIYUU, TRUE );
	else diag.SetData(ITM_RIYUU, DD );

	BOOL	flag;
	if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) ){
		flag = FALSE;
	}
	else{
		flag = TRUE;
	}
	SetAttribMod(ITM_WARIAI,flag);
	SetAttribMod(ITM_SETSUBI,flag);
	SetAttribMod(ITM_SONOTA,flag);
	if( flag == TRUE )	{
		if( refundvol.KPRsgn & 0x04 )	flag = TRUE;
		else							flag = FALSE;
	}
	SetAttribMod(ITM_RIYUU,flag);	

	SetInitPos();
}

int CDiagCtlRiyuu::ControlChange(int index,int flg)
{
	const CString FMT_MS_ETC_DL	= _T("このチェックを外すとその他の理由の詳細は削除されます。\nチェックを外してもよろしいですか？");

	if(index == ITM_SONOTA){
		if( !flg ){
			if( ICSMessageBox(FMT_MS_ETC_DL,(MB_YESNO | MB_DEFBUTTON2)) == IDNO  ) return 1; 
			else{
				diag.DataClear(ITM_RIYUU,TRUE);
				EditOff(ITM_RIYUU);
			}
		}
		SetAttribMod( ITM_RIYUU, flg ? EDCOLOR : GetBkcolor() , flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY);
	}
	return 0;
}

void CDiagCtlRiyuu::EditOff(int index)
{
	CDiagData DD;
	diag.GetData(index,DD);

	REFUNDVOL& refundvol = syzdata->Getvol();

	switch(index){
	case ITM_WARIAI:
		if(DD.data_check)	refundvol.KPRsgn |= 0x01;
		else				refundvol.KPRsgn &=~0x01;
		TRACE("割合\n");break;
	case ITM_SETSUBI:
		if(DD.data_check)	refundvol.KPRsgn |= 0x02;
		else				refundvol.KPRsgn &=~0x02;
		TRACE("設備投資\n");break;
	case ITM_SONOTA:
		if(DD.data_check)	refundvol.KPRsgn |= 0x04;
		else				refundvol.KPRsgn &=~0x04;
		TRACE("その他\n");break;
	case ITM_RIYUU:
		ZyougedanStringToBuf(DD.data_edit,refundvol.KPRrup,refundvol.KPRrlw,KPRrLN);
		TRACE("理由\n");break;
	default:
		TRACE("ofs error!");
		break;
	}
	TRACE("index = %d\n",index);
}