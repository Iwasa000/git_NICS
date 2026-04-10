#include "stdafx.h"
#include "DiagCtlMeisai.h"
#include "DiagCtlTorihiki.h"

enum{
	ITM_TYPE,
	ITM_YEAR,
	ITM_YEARKZK,
	ITM_MNY,
	ITM_NAME,
	ITM_ADDR,
};

enum{
	ITM_TORIHIKI = 3,
	ITM_KOMI,
	ITM_NUKI,
};

const int  torihiki_item_cnt = 6;

CDiagCtlTorihiki::CDiagCtlTorihiki(CIcsdiagctrl& dg, int l_max,int s_index)
	: CDiagCtlMeisai( dg, torihiki_item_cnt, l_max, s_index)
{
	int index = GetCurrentIndex(0,3);

	PositionMap.SetAt( 3, CRect(MOVE_PREV,MOVE_PREV,4,4) );
	PositionMap.SetAt( 4, CRect(3,3,5,index) );
	PositionMap.SetAt( 5, CRect(4,3,start_meisai,index) );

	PositionMap[index].top = 5;
	PositionMap[start_meisai].left = 5;

	RadioMap.SetAt(4,5);
	RadioMap.SetAt(5,4);
}

void CDiagCtlTorihiki::TorihikiChange(int flg)
{
	POSITION pos = RadioMap.GetStartPosition();
	while( pos != NULL ){
		int key,val;
		RadioMap.GetNextAssoc(pos,key,val);
		SetAttribMod( key, flg ? EDCOLOR : GetBkcolor() , flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY);
	}
	
}

int CDiagCtlTorihiki::ControlChange(int index,int flg) {
	const CString FMT_MS_TRH_DL	= "このチェックを外すと取引金額等の経理方式が出力されません。\nチェックを外してもよろしいですか？";
	
	if( index == ITM_TORIHIKI ){
		if( !flg && ICSMessageBox(FMT_MS_TRH_DL,(MB_YESNO | MB_DEFBUTTON2)) == IDNO  ) return 1;
		TorihikiChange(flg);
	}else{
		int idx = index - 1;
		if( ITM_YEAR == GetItemOfs(idx) ){
			CDiagData DD;
			if(!flg) memmove(DD.data_day,syzdata->GetRec(GetItemLine(idx)).KPSday,3);
			diag.SetData( idx, DD );
		}
		SetAttribMod( idx, !flg ? EDCOLOR : GetBkcolor() , !flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY);
	}
	return 0;
}



void CDiagCtlTorihiki::DispInit()
{
	for(int line = 0; line < line_max; line++){
		REFUNDREC& refunrec = syzdata->GetRec(line);

//★//[12'09.19]///
		BOOL	flag;
		if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
		else													flag = TRUE;
		SetAttribMod(GetCurrentIndex(line,ITM_TYPE),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_YEAR),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_YEARKZK),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_MNY),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_NAME),flag);
		SetAttribMod(GetCurrentIndex(line,ITM_ADDR),flag);
///////////////////

		CDiagData DD;
		DD.data_edit = 	ZyougedanBufToString(refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_TYPE), TRUE );
		else diag.SetData(GetCurrentIndex(line,ITM_TYPE), DD );

		memmove(DD.data_day,refunrec.KPSday,3);
		diag.SetData(GetCurrentIndex(line,ITM_YEAR), DD );

		DD.data_check = refunrec.KPSdys;
		diag.SetData(GetCurrentIndex(line,ITM_YEARKZK), DD );

		ControlChange(GetCurrentIndex(line,ITM_YEARKZK), !flag || refunrec.KPSdys );

		ConvEditVal(DD.data_val, refunrec.KPSval, 0);
		diag.SetData(GetCurrentIndex(line,ITM_MNY), DD );

		DD.data_edit = 	ZyougedanBufToString(refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_NAME), TRUE );
		else diag.SetData(GetCurrentIndex(line,ITM_NAME), DD );
		DD.data_edit = 	ZyougedanBufToString(refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
		if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_ADDR), TRUE );
		else diag.SetData(GetCurrentIndex(line,ITM_ADDR), DD );

	}
	
	if( syzdata->Check ) {
		CDiagData DD;
		DD.data_check = *syzdata->Check ? 1 : 0;
		diag.SetData(ITM_TORIHIKI, DD );
		TorihikiChange(*syzdata->Check);
	}

	if( syzdata->Radio ) {
		CDiagData DD;
		DD.data_check = *syzdata->Radio ? 1 : 0;
		diag.SetData(ITM_KOMI, DD );
		DD.data_check = *syzdata->Radio ? 0 : 1;
		diag.SetData(ITM_NUKI, DD );
	}
//★//[12'09.19]///
	BOOL	flag;
	if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) ){//入力ロック時
		flag = FALSE;

		SetAttribMod(ITM_TORIHIKI,flag);
		SetAttribMod(ITM_KOMI,flag);
		SetAttribMod(ITM_NUKI,flag);

	}
	else{//入力ロック解除時
		flag = TRUE;
		SetAttribMod(ITM_TORIHIKI,flag);

		if( syzdata->Check ) {//[Y]
			flag = *syzdata->Check ? TRUE : FALSE;		
		}
		SetAttribMod(ITM_KOMI,flag);
		SetAttribMod(ITM_NUKI,flag);
	}

	/*
	BOOL	flag;
	if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
	else													flag = TRUE;
	SetAttribMod(ITM_TORIHIKI,flag);
	if( syzdata->Check ) {//[Y]
		flag = *syzdata->Check ? TRUE : FALSE;		
	}
	SetAttribMod(ITM_KOMI,flag);
	SetAttribMod(ITM_NUKI,flag);
	*/
///////////////////

}

void CDiagCtlTorihiki::DateValidate(int index,CDiagData& DD)
{
	if(DD.data_day[0] == (char)0xff || !DD.data_day[0] ){
		memset(DD.data_day,0,4);
		diag.DataClear(index,TRUE);
	}else
		diag.SetData(index,DD);
}

//-----------------------------------------------------------------------------
// 年月日変換
//-----------------------------------------------------------------------------
// 引数	pSday
//		pDGengo
//		pDday
//-----------------------------------------------------------------------------
void CDiagCtlTorihiki::CnvYmd(unsigned char* pSday, int* pDGengo, unsigned char* pDday)
{
	if( IsValidKaigen() == FALSE ){
		memmove(pDday, pSday, sizeof(UCHAR)*4);
		*pDGengo = 0;
		return;
	}

	if( (pSday[0]==0x00) && (pSday[1]==0x00) && (pSday[2]==0x00) ){
		*pDGengo = 0;
		memset(pDday, '\0', sizeof(UCHAR)*4);
		return;
	}
	else{
		UCHAR	chkYmd[4]={ 0 };
		chkYmd[0] = 0x31;
		chkYmd[1] = 0x05;
		if( memcmp(pSday, chkYmd, sizeof(UCHAR)*2) >= 0 ){
			// 平成31年5月以降が入力された時→令和1年5月が入力されたと自動変換
			pSday[0] -= 0x30;
		}
	}
	UCHAR	svYmd[4]={ 0 };
	memmove(svYmd, pSday, sizeof(svYmd));

	// 和暦→西暦変換
	// 西暦→和暦変換
	CVolDateDB	voldate;
	int		tmpGengo=0;
	UCHAR	SeYmd[4]={ 0 }, WaYmd[4]={ 0 };
	int		st = 0;
	if( pSday[1] ){
		if( pSday[2] ){
			st = voldate.db_datecnvGen(0, pSday, &tmpGengo, SeYmd, 1, 0);
			voldate.db_datecnvGen(0, SeYmd, &tmpGengo, WaYmd, 0, 0);
		}
		else{
			st = voldate.db_datecnvGen(0, pSday, &tmpGengo, SeYmd, 1, 1);
			voldate.db_datecnvGen(0, SeYmd, &tmpGengo, WaYmd, 0, 1);
		}
	}
	else{
		st = voldate.db_datecnvGenEx(0, pSday, &tmpGengo, SeYmd, 1, 2);
		voldate.db_datecnvGenEx(0, SeYmd, &tmpGengo, WaYmd, 0, 2);
	}
	WaYmd[1] = svYmd[1];
	WaYmd[2] = svYmd[2];

	if( st != -1 ){
		*pDGengo = tmpGengo;
		memmove(pDday, WaYmd, sizeof(WaYmd));
	}
	else{
		*pDGengo = 0;
		memset(pDday, '\0', sizeof(UCHAR)*4);
	}
}

void CDiagCtlTorihiki::SetItemData(int line,int ofs)
{
	CDiagData DD;
	
	int index = GetCurrentIndex(line,ofs);
	diag.GetData(index,DD);
	
	REFUNDREC& refunrec = syzdata->GetRec(line);

	unsigned char tmpday[4]={ 0 };
	int	tmpGengo=0;

	switch(ofs){
	case ITM_TYPE:
		ZyougedanStringToBuf(DD.data_edit,refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
		TRACE("種類\n");break;
	case ITM_YEAR:
		DateValidate(index,DD);

		memmove(tmpday, DD.data_day, sizeof(tmpday));
		CnvYmd((UCHAR*)tmpday, &tmpGengo, (UCHAR*)DD.data_day);

		memmove(refunrec.KPSday,DD.data_day,3);
		refunrec.KPSgengo = tmpGengo;

		memmove(DD.data_day, refunrec.KPSday, 3);
		diag.SetData(GetCurrentIndex(line, ITM_YEAR), DD);

		TRACE("年月日\n");break;
	case ITM_YEARKZK:
		refunrec.KPSdys = (char)DD.data_check;
		TRACE("継続\n");break;
	case ITM_MNY:
		ConvEditVal( refunrec.KPSval, DD.data_val, 1);
		TRACE("金額\n");break;
	case ITM_NAME:
		ZyougedanStringToBuf(DD.data_edit,refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
		TRACE("名称\n");break;
	case ITM_ADDR:
		ZyougedanStringToBuf(DD.data_edit,refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
		TRACE("住所\n");break;
	default:
		TRACE("ofs error!");
		break;
	}
	TRACE("line = %d,ofs = %d\n",line,ofs);
}

void CDiagCtlTorihiki::SetItemData(int index )
{
	CDiagData DD;
	diag.GetData(index,DD);

	switch(index){
	case ITM_TORIHIKI:
		*syzdata->Check = (char)DD.data_check;
		TRACE("取引金額等\n");break;
	case ITM_KOMI:
		*syzdata->Radio = DD.data_check ? 1 : 0;
		TRACE("税込\n");break;
	case ITM_NUKI:
		*syzdata->Radio = DD.data_check ? 0 : 1;
		TRACE("税抜\n");break;
	default:
		TRACE("index error!");
		break;
	}
	TRACE("index = %d\n",index);
}

void CDiagCtlTorihiki::SetInitPos()
{
	if(!(pSnHeadData->Sn_Sign4&0x80)){
		diag.SetPosition(ITM_TORIHIKI);
	}
}
