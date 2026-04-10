#include "stdafx.h"
#include "DiagCtlYusyutsu.h"


enum{
	ITM_GNAME = 12,
	ITM_GTYPE,
	ITM_SNAME,
	ITM_STYPE,
	ITM_YNAME,

	ITM_BANKCH = 19,
	ITM_KOZANO,
	ITM_YUNO1,
	ITM_YUNO2  = 23,
	ITM_NAME,
	ITM_ADDR,
};

CDiagCtlYusyutsu::CDiagCtlYusyutsu(CIcsdiagctrl& dg)
	: CDiagControlBase(dg)
{
	pair<int,CRect> list[] = {
		pair<int,CRect>( 12,CRect(MOVE_PREV,MOVE_PREV,13,16 )),
		pair<int,CRect>( 13,CRect(12,NO_MOVE,14,NO_MOVE )),
		pair<int,CRect>( 14,CRect(13,MOVE_PREV,15,20 )),
		pair<int,CRect>( 15,CRect(14,NO_MOVE,16,NO_MOVE )),
		pair<int,CRect>( 16,CRect(15,12,19,NO_MOVE )),

		pair<int,CRect>( ITM_BANKCH,CRect(16,13,20,24 )),
		pair<int,CRect>( ITM_KOZANO,CRect(19,14,21,21 )),
		pair<int,CRect>( ITM_YUNO1 ,CRect(20,20,23,24 )),
		pair<int,CRect>( ITM_YUNO2 ,CRect(21,20,24,24 )),
		pair<int,CRect>( ITM_NAME  ,CRect(23,21,25,25 )),
		pair<int,CRect>( ITM_ADDR  ,CRect(24,24,MOVE_NEXT,MOVE_NEXT )),
	};

	for(int i = 0;i<ARRAYCOUNT(list); i++){
		PositionMap.SetAt(list[i].first,list[i].second);
		SetZenHan(list[i].first);
	}

}

int CDiagCtlYusyutsu::ControlChange(int index,int flg)
{
	if( index == ITM_BANKCH ){
		SetAttribMod( ITM_YUNO1, flg ? EDCOLOR : GetBkcolor() , flg ? DIAG_MDFY_EDIT : DIAG_MDFY_TRANSPARENT);
		SetAttribMod( ITM_YUNO1+1, flg ? EDCOLOR : GetBkcolor()  , flg ? DIAG_MDFY_READONLY : DIAG_MDFY_TRANSPARENT);
		SetAttribMod( ITM_YUNO2, flg ? EDCOLOR : GetBkcolor() , flg ? DIAG_MDFY_EDIT : DIAG_MDFY_TRANSPARENT);
		SetAttribMod( ITM_KOZANO, !flg ? EDCOLOR : GetBkcolor() , !flg ? DIAG_MDFY_EDIT : DIAG_MDFY_TRANSPARENT);
	}

	return 0;
}

void CDiagCtlYusyutsu::DispInit()
{
	REFUNDVOL& refundvol = syzdata->Getvol();

	CDiagData DD;
	DD.data_edit = MKString(refundvol.KPEbnk,KPEbnLN);
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_GNAME, TRUE );
	else diag.SetData(ITM_GNAME, DD );

	DD.data_combo = refundvol.KPEbsg;
	diag.SetData(ITM_GTYPE, DD );

	DD.data_edit = MKString(refundvol.KPEstn,KPEstLN);
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_SNAME, TRUE );
	else diag.SetData(ITM_SNAME, DD );

	DD.data_combo = refundvol.KPEssg;
	diag.SetData(ITM_STYPE, DD );

	DD.data_edit = MKString(refundvol.KPEykn,KPEykLN);
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_YNAME, TRUE );
	else diag.SetData(ITM_YNAME, DD );

	if( refundvol.KPEkoz[0]	!= (char)0xff ){
		char kozacode[128] = {0};
		arith.l_unpac(kozacode,refundvol.KPEkoz,10);
		DD.data_edit = kozacode;
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_KOZANO, TRUE );
		else diag.SetData(ITM_KOZANO, DD );
	}
//Åö//[12'02.01]///
	else	diag.DataClear(ITM_KOZANO,FALSE); 
//		DD.data_edit = _T("");
//		diag.SetData(ITM_KOZANO, DD );
//	}
///////////////////
	DD.data_edit = ZyougedanBufToString(refundvol.KPEsup,refundvol.KPEslw,KPEsnLN);
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_NAME, TRUE );
	else diag.SetData(ITM_NAME, DD );

	DD.data_edit = ZyougedanBufToString(refundvol.KPEaup,refundvol.KPEalw,KPEaLN);
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_ADDR, TRUE );
	else diag.SetData(ITM_ADDR, DD );

	DD.data_edit = MKString(refundvol.KPEyn1,5);
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_YUNO1, TRUE );
	else diag.SetData(ITM_YUNO1, DD );

	DD.data_edit = MKString(refundvol.KPEyn2,8);
	if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(ITM_YUNO2, TRUE );
	else diag.SetData(ITM_YUNO2, DD );

	DD.data_check = refundvol.KPEbdm;
	diag.SetData(ITM_BANKCH, DD );
	ControlChange(ITM_BANKCH,refundvol.KPEbdm);

//Åö//[12'09.19]///
	BOOL	flag;
	if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
	else													flag = TRUE;
	SetAttribMod(ITM_GNAME,flag);
	SetAttribMod(ITM_GTYPE,flag);
	SetAttribMod(ITM_SNAME,flag);
	SetAttribMod(ITM_STYPE,flag);
	SetAttribMod(ITM_YNAME,flag);
	SetAttribMod(ITM_BANKCH,flag);
	if( refundvol.KPEbdm )	{
		// óXíô
		SetAttribMod(ITM_YUNO1,flag);
		SetAttribMod(ITM_YUNO1 + 1,flag);
		SetAttribMod(ITM_YUNO2,flag);
	}
	else	{
		// ã‚çs
		SetAttribMod(ITM_KOZANO,flag);
	}
	SetAttribMod(ITM_NAME,flag);
	SetAttribMod(ITM_ADDR,flag);
///////////////////

}

void CDiagCtlYusyutsu::EditOff(int index)
{
	CDiagData DD;
	diag.GetData(index,DD);

	REFUNDVOL& refundvol = syzdata->Getvol();
	
	switch(index){
	case ITM_GNAME:
		StringToBuf(DD.data_edit,refundvol.KPEbnk,KPEbnLN);
		TRACE("ã‚çsñº\n");break;
	case ITM_GTYPE:
		refundvol.KPEbsg = (char)DD.data_combo;
		TRACE("ã‚çséÌï \n");break;
	case ITM_SNAME:
		StringToBuf(DD.data_edit,refundvol.KPEstn,KPEstLN);
		TRACE("éxìXñº\n");break;
	case ITM_STYPE:
		refundvol.KPEssg = (char)DD.data_combo;
		TRACE("éxìXéÌï \n");break;
	case ITM_YNAME:
		StringToBuf(DD.data_edit,refundvol.KPEykn,KPEykLN);
		TRACE("óaã‡ñºèÃ\n");break;
	case ITM_BANKCH:
		refundvol.KPEbdm = (char)DD.data_check;
		TRACE("å˚ç¿éÌï \n");break;
	case ITM_KOZANO:
		if( !DD.data_edit.GetLength() )
			memset(refundvol.KPEkoz,0xff,sizeof(refundvol.KPEkoz));
		else
			arith.l_pack(refundvol.KPEkoz,(LPSTR)(LPCSTR)DD.data_edit,10);
		TRACE("å˚ç¿î‘çÜ\n");break;
	case ITM_YUNO1:
		StringToBuf(DD.data_edit,refundvol.KPEyn1,5);
		TRACE("Ç‰Ç§ÇøÇÂÇP\n");break;
	case ITM_YUNO2:
		StringToBuf(DD.data_edit,refundvol.KPEyn2,8);
		TRACE("Ç‰Ç§ÇøÇÂÇQ\n");break;
	case ITM_NAME:
		ZyougedanStringToBuf(DD.data_edit,refundvol.KPEsup,refundvol.KPEslw,KPEsnLN);
		TRACE("ñºèÃ\n");break;
	case ITM_ADDR:
		ZyougedanStringToBuf(DD.data_edit,refundvol.KPEaup,refundvol.KPEalw,KPEaLN);
		TRACE("èZèä\n");break;
	default:
		TRACE("index error!");
		break;
	}
	TRACE("index = %d\n",index);
}

//èZèäåüçı
void CDiagCtlYusyutsu::HOME(int index) {

	if (index == ITM_ADDR) {

		char	buf[512] = { 0 };
		int		len = 0;
		CString	ZipBf, AddBf;

		DIAGRAM_DATA	DiagData;
		DiagData.data_imgdata = NULL;

		if (ZipToAddress(ZipBf, AddBf)) {
			//èZèäÇÃÇ›ÉZÉbÉg
			memset(buf, '\0', sizeof(buf));
			len = AddBf.GetLength();
			memmove(buf, AddBf, len);
			DiagData.data_edit = buf;
			diag.SetData(index, (struct IUnknown *)&DiagData);
			diag.SetPosition(index);
		}
	}

	return;
}