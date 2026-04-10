#include "stdafx.h"
#include "DiagCtlMeisai.h"
#include "DiagCtlYsMeisai.h"

enum{
	ITM_NAME,
	ITM_ADDR,
	ITM_MNY,
	ITM_PRINCIPAL,
	ITM_CUSTOMS,
};

const int  ysmeisai_item_cnt = 5;

CDiagCtlYsMeisai::CDiagCtlYsMeisai(CIcsdiagctrl& dg, int l_max,int s_index)
	: CDiagCtlMeisai( dg, ysmeisai_item_cnt, l_max, s_index)
{
}

void CDiagCtlYsMeisai::DispInit()
{
	for(int line = 0; line < line_max; line++)
	{
		REFUNDREC& refunrec = syzdata->GetRec(line);

//★//[12'09.19]///
		BOOL	flag;
		if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
		else													flag = TRUE;
		SetAttribMod(GetCurrentIndex(line,ITM_NAME),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_ADDR),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_MNY),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_MNY),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_PRINCIPAL),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_CUSTOMS),flag);
///////////////////

		CDiagData DD;
		DD.data_edit = 	ZyougedanBufToString(refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_NAME), TRUE );
		else diag.SetData(GetCurrentIndex(line,ITM_NAME), DD );

		DD.data_edit = 	ZyougedanBufToString(refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_ADDR), TRUE );
		else diag.SetData(GetCurrentIndex(line,ITM_ADDR), DD );

		ConvEditVal(DD.data_val, refunrec.KPSval, 0);
/*- '14.06.11 -*/
//		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_MNY), TRUE );
//		else diag.SetData(GetCurrentIndex(line,ITM_MNY), DD );
/*-------------*/
		diag.SetData(GetCurrentIndex(line,ITM_MNY), DD );
/*-------------*/

		DD.data_edit = 	ZyougedanBufToString(refunrec.KPStup,refunrec.KPStlw,KPStLN);
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_PRINCIPAL), TRUE );
		else diag.SetData(GetCurrentIndex(line,ITM_PRINCIPAL), DD );

		DD.data_edit = 	ZyougedanBufToString(refunrec.KPSzup,refunrec.KPSzlw,KPSzLN);
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_CUSTOMS), TRUE );
		else diag.SetData(GetCurrentIndex(line,ITM_CUSTOMS), DD );

	}
}

void CDiagCtlYsMeisai::SetItemData(int line,int ofs)
{
	CDiagData DD;
	int index = GetCurrentIndex(line,ofs);
	diag.GetData(index,DD);
	
	REFUNDREC& refunrec = syzdata->GetRec(line);

	switch(ofs){
	case ITM_NAME:
		ZyougedanStringToBuf(DD.data_edit,refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
		TRACE("名称\n");break;
	case ITM_ADDR:
		ZyougedanStringToBuf(DD.data_edit,refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
		TRACE("住所\n");break;
	case ITM_MNY:
		ConvEditVal(refunrec.KPSval, DD.data_val, 1);
		TRACE("金額\n");break;
	case ITM_PRINCIPAL:
		ZyougedanStringToBuf(DD.data_edit,refunrec.KPStup,refunrec.KPStlw,KPStLN);
		TRACE("主な商品\n");break;
	case ITM_CUSTOMS:
		ZyougedanStringToBuf(DD.data_edit,refunrec.KPSzup,refunrec.KPSzlw,KPSzLN);
		TRACE("所轄税関\n");break;
	default:
		TRACE("ofs error!");
		break;
	}
	TRACE("line = %d,ofs = %d\n",line,ofs);
}

//2016.02.23 INSERT START
BOOL CDiagCtlYsMeisai::Del_Message(int sel_idx)
{
	BOOL all_blank = TRUE;
	for( int line_cnt = 0; line_cnt < line_max; line_cnt++ ){
		if( Line_BlankJugde( line_cnt ) == FALSE ){
			all_blank = FALSE;
			break;
		}
	}
	if( all_blank == TRUE ){
		return FALSE;
	}

	int line_idx = GetItemLine(sel_idx);
	if( line_idx < 0 ){
		return FALSE;
	}

	REFUNDREC& refunrec = syzdata->GetRec(line_idx);

	CString	msg;
	//msg.Format( _T("取引先の氏名(名称)「%s%s」を削除します。よろしいですか？"), refunrec.KPSnup, refunrec.KPSnlw );
	msg.Format( _T("取引先の氏名(名称)「%s%s」の行を削除します。よろしいですか？"), refunrec.KPSnup, refunrec.KPSnlw );
	//if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0, this ) == IDNO ){
	if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0 ) == IDNO ){
		return FALSE;
	}

	return TRUE;
}

BOOL CDiagCtlYsMeisai::Line_BlankJugde(int line_idx)
{

	REFUNDREC& refunrec = syzdata->GetRec(line_idx);

	if( strlen( refunrec.KPSnup ) != 0 ){ //取引先名（上段）
		return FALSE;
	}else
	if( strlen( refunrec.KPSnlw ) != 0 ){ //取引先名（下段）
		return FALSE;
	}else
	if( strlen( refunrec.KPSaup ) != 0 ){ //取引先住所（上段）
		return FALSE;
	}else
	if( strlen( refunrec.KPSalw ) != 0 ){ //取引先住所（下段）
		return FALSE;
	}else
//	if( strlen( refunrec.KPSval ) != 0 ){ //取引金額
	if( arith.l_test(refunrec.KPSval) != 0 ){
		return FALSE;
	}else
	if( strlen( refunrec.KPStup ) != 0 ){ //主な取引商品（上段）
		return FALSE;
	}else
	if( strlen( refunrec.KPStlw ) != 0 ){ //主な取引商品（下段）
		return FALSE;
	}else
	if( strlen( refunrec.KPSzup ) != 0 ){ //所轄税関名（上段）
		return FALSE;
	}else
	if( strlen( refunrec.KPSzlw ) != 0 ){ //所轄税関名（下段）
		return FALSE;
	}

	return TRUE;
}
//2016.02.23 INSERT END

//住所検索
void CDiagCtlYsMeisai::HOME(int index) {

	if(GetCurrentcolumn(index) == ITM_ADDR ){

		char	buf[512] = { 0 };
		int		len = 0;
		CString	ZipBf, AddBf;

		DIAGRAM_DATA	DiagData;
		DiagData.data_imgdata = NULL;

		if (ZipToAddress(ZipBf, AddBf)) {
			//住所のみセット
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