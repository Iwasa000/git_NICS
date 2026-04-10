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

// 還付付表2　(3)固定資産用
enum{
	ITM_KOTEI_TYPE,
	ITM_KOTEI_YEAR,
	ITM_KOTEI_MNY,
	ITM_KOTEI_NAME,
	ITM_KOTEI_ADDR,
};

//----> 20230704
// 還付付表2　(4)棚卸資産用 取引先の登録番号追加
enum{
	ITM2_TYPE,
	ITM2_YEAR,
	ITM2_YEARKZK,
	ITM2_MNY,
	ITM2_INVNO,
	ITM2_NAME,
	ITM2_ADDR,
};

// 還付付表2　(3)固定資産用 取引先の登録番号追加
enum{
	ITM2_KOTEI_TYPE,
	ITM2_KOTEI_YEAR,
	ITM2_KOTEI_MNY,
	ITM2_KOTEI_INVNO,
	ITM2_KOTEI_NAME,
	ITM2_KOTEI_ADDR,
};
//<---- 20230704

enum{
	ITM_TORIHIKI = 3,
	ITM_KOMI,
	ITM_NUKI,
};

//const int  torihiki_item_cnt = 6;	// (3)固定資産のみ項目数が変わるため使用しない

CDiagCtlTorihiki::CDiagCtlTorihiki(CIcsdiagctrl& dg, int l_max, int s_index, int item_cnt)
//	: CDiagCtlMeisai( dg, torihiki_item_cnt, l_max, s_index)
	: CDiagCtlMeisai( dg, item_cnt, l_max, s_index)
{
	// 1行目の「取引金額等」列のキー移動の設定

	int ofs;
	if( IsKoteiSisanDlg() )	ofs = 2;	// 20230704 これは何のオフセットやったか確認
	else					ofs = 3;

	int index = GetCurrentIndex(0,ofs);

	PositionMap.SetAt( 3, CRect(MOVE_PREV,MOVE_PREV,4,4) );
	PositionMap.SetAt( 4, CRect(3,3,5,index) );
	PositionMap.SetAt( 5, CRect(4,3,start_meisai,index) );

	PositionMap[index].top = 5;
	PositionMap[start_meisai].left = 5;

	RadioMap.SetAt(4,5);
	RadioMap.SetAt(5,4);

	//----> 20230704
	m_SyzInvoice.SetDBZmSub( pZmSub );
	//<---- 20230704

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
		
		//----> 20230704
		int idx = 0, itmidx = 0, sw = 0;
		int diagtype = GetDiagType();
		if( diagtype == 1 ){
			idx = index;
			itmidx = ITM2_KOTEI_YEAR;			
		}
		else if( diagtype == 2 ){
			idx = index - 1;	// 20230704 -1いるか確認
			itmidx = ITM2_YEAR;			
		}
		else if( diagtype == 3 ){
			idx = index;
			itmidx = ITM_KOTEI_YEAR;			
		}
		else{
			idx = index - 1;	// 20230704 -1いるか確認
			itmidx = ITM_YEAR;			
		}
		if( itmidx == GetItemOfs( idx ) ){
			sw = 1;
		}
		//<---- 20230704

		//----> 20230704 del
		/*int idx = 0, sw = 0;
		if( IsKoteiSisanDlg() ){
			idx = index;
			if( ITM_KOTEI_YEAR == GetItemOfs(idx) ) sw = 1;
		}
		else{
			idx = index - 1;
			if( ITM_YEAR == GetItemOfs(idx) ) sw = 1;
		}*/
		//<---- 20230704 del

		if( sw ){
			CDiagData DD;
			if(!flg){
				//memmove(DD.data_day,syzdata->GetRec(GetItemLine(idx)).KPSday,3);
				DD.data_day[0] = syzdata->GetRec(GetItemLine(idx)).KPSgengo;
				char day[3];
				memmove(day,syzdata->GetRec(GetItemLine(idx)).KPSday,3);
				DD.data_day[1] = day[0];
				DD.data_day[2] = day[1];
				DD.data_day[3] = day[2];
			}
			diag.SetData( idx, DD );
		}
		SetAttribMod( idx, !flg ? EDCOLOR : GetBkcolor() , !flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY);
	}
	return 0;
}



void CDiagCtlTorihiki::DispInit()
{
	//----> 20230704
	for(int line = 0; line < line_max; line++){
		REFUNDREC& refunrec = syzdata->GetRec(line);

		int diagtype = GetDiagType();

		if( diagtype == 1 ){

//★//[12'09.19]///
			BOOL	flag;
			if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
			else													flag = TRUE;
			SetAttribMod(GetCurrentIndex(line,ITM2_KOTEI_TYPE),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_KOTEI_YEAR),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_KOTEI_MNY),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_KOTEI_INVNO),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_KOTEI_NAME),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_KOTEI_ADDR),flag);
///////////////////

			CDiagData DD;
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM2_KOTEI_TYPE), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM2_KOTEI_TYPE), DD );

//			memmove(DD.data_day,refunrec.KPSday,3);
			DD.data_day[0] = refunrec.KPSgengo;
			DD.data_day[1] = refunrec.KPSday[0];
			DD.data_day[2] = refunrec.KPSday[1];
			DD.data_day[3] = refunrec.KPSday[2];
			diag.SetData(GetCurrentIndex(line,ITM2_KOTEI_YEAR), DD );
			ControlChange(GetCurrentIndex(line,ITM2_KOTEI_YEAR), !flag );

			ConvEditVal(DD.data_val, refunrec.KPSval, 0);
			diag.SetData(GetCurrentIndex(line,ITM2_KOTEI_MNY), DD );

			DD.data_edit = refunrec.KPInvno;
			diag.SetData( GetCurrentIndex( line, ITM2_KOTEI_INVNO ), DD );

			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM2_KOTEI_NAME), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM2_KOTEI_NAME), DD );
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM2_KOTEI_ADDR), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM2_KOTEI_ADDR), DD );

		}
		else if( diagtype == 2 ){

//★//[12'09.19]///
			BOOL	flag;
			if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
			else													flag = TRUE;
			SetAttribMod(GetCurrentIndex(line,ITM2_TYPE),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_YEAR),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_YEARKZK),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_MNY),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_INVNO),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_NAME),flag);
			SetAttribMod(GetCurrentIndex(line,ITM2_ADDR),flag);
///////////////////

			CDiagData DD;
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM2_TYPE), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM2_TYPE), DD );

//			memmove(DD.data_day,refunrec.KPSday,3);
			DD.data_day[0] = refunrec.KPSgengo;
			DD.data_day[1] = refunrec.KPSday[0];
			DD.data_day[2] = refunrec.KPSday[1];
			DD.data_day[3] = refunrec.KPSday[2];
			diag.SetData(GetCurrentIndex(line,ITM2_YEAR), DD );

			DD.data_check = refunrec.KPSdys;
			diag.SetData(GetCurrentIndex(line,ITM2_YEARKZK), DD );
			ControlChange(GetCurrentIndex(line,ITM2_YEARKZK), !flag || refunrec.KPSdys );

			ConvEditVal(DD.data_val, refunrec.KPSval, 0);
			diag.SetData(GetCurrentIndex(line,ITM2_MNY), DD );

			DD.data_edit = refunrec.KPInvno;
			diag.SetData( GetCurrentIndex( line, ITM2_INVNO ), DD );

			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM2_NAME), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM2_NAME), DD );
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM2_ADDR), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM2_ADDR), DD );

		}
		else if( diagtype == 3 ){

//★//[12'09.19]///
			BOOL	flag;
			if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
			else													flag = TRUE;
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_TYPE),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_YEAR),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_MNY),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_NAME),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_ADDR),flag);
///////////////////

			CDiagData DD;
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_KOTEI_TYPE), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM_KOTEI_TYPE), DD );

//			memmove(DD.data_day,refunrec.KPSday,3);
			DD.data_day[0] = refunrec.KPSgengo;
			DD.data_day[1] = refunrec.KPSday[0];
			DD.data_day[2] = refunrec.KPSday[1];
			DD.data_day[3] = refunrec.KPSday[2];
			diag.SetData(GetCurrentIndex(line,ITM_KOTEI_YEAR), DD );
			ControlChange(GetCurrentIndex(line,ITM_KOTEI_YEAR), !flag );

			ConvEditVal(DD.data_val, refunrec.KPSval, 0);
			diag.SetData(GetCurrentIndex(line,ITM_KOTEI_MNY), DD );

			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_KOTEI_NAME), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM_KOTEI_NAME), DD );
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_KOTEI_ADDR), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM_KOTEI_ADDR), DD );

		}
		else{

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

//			memmove(DD.data_day,refunrec.KPSday,3);
			DD.data_day[0] = refunrec.KPSgengo;
			DD.data_day[1] = refunrec.KPSday[0];
			DD.data_day[2] = refunrec.KPSday[1];
			DD.data_day[3] = refunrec.KPSday[2];
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
	}
	//<---- 20230704

	//----> 20230704 del
	/*for(int line = 0; line < line_max; line++){
		REFUNDREC& refunrec = syzdata->GetRec(line);

		// (3)主な固定資産等の取得
		if( IsKoteiSisanDlg() ){

//★//[12'09.19]///
			BOOL	flag;
			if( (CDiagControlBase::pSnHeadData->Sn_Sign4&0x01) )	flag = FALSE;
			else													flag = TRUE;
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_TYPE),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_YEAR),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_MNY),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_NAME),flag);
			SetAttribMod(GetCurrentIndex(line,ITM_KOTEI_ADDR),flag);
///////////////////

			CDiagData DD;
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_KOTEI_TYPE), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM_KOTEI_TYPE), DD );

//			memmove(DD.data_day,refunrec.KPSday,3);
			DD.data_day[0] = refunrec.KPSgengo;
			DD.data_day[1] = refunrec.KPSday[0];
			DD.data_day[2] = refunrec.KPSday[1];
			DD.data_day[3] = refunrec.KPSday[2];
			diag.SetData(GetCurrentIndex(line,ITM_KOTEI_YEAR), DD );
			ControlChange(GetCurrentIndex(line,ITM_KOTEI_YEAR), !flag );

			ConvEditVal(DD.data_val, refunrec.KPSval, 0);
			diag.SetData(GetCurrentIndex(line,ITM_KOTEI_MNY), DD );

			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_KOTEI_NAME), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM_KOTEI_NAME), DD );
			DD.data_edit = 	ZyougedanBufToString(refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			if( DD.data_edit.IsEmpty() == TRUE ) diag.DataClear(GetCurrentIndex(line,ITM_KOTEI_ADDR), TRUE );
			else diag.SetData(GetCurrentIndex(line,ITM_KOTEI_ADDR), DD );

		}
		else{

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

//			memmove(DD.data_day,refunrec.KPSday,3);
			DD.data_day[0] = refunrec.KPSgengo;
			DD.data_day[1] = refunrec.KPSday[0];
			DD.data_day[2] = refunrec.KPSday[1];
			DD.data_day[3] = refunrec.KPSday[2];
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
	}*/
	//<---- 20230704 del
	
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

		if( syzdata->Check ) {
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
	if( syzdata->Check ) {
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

void CDiagCtlTorihiki::SetItemData(int line,int ofs)
{
	CDiagData DD;
	
	int index = GetCurrentIndex(line,ofs);
	diag.GetData(index,DD);
	
	REFUNDREC& refunrec = syzdata->GetRec(line);

	//----> 20230704
	int diagtype = GetDiagType();

	if( diagtype == 1 ){

		switch(ofs){
		case ITM2_KOTEI_TYPE:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			TRACE("種類\n");break;
		case ITM2_KOTEI_YEAR:
			DateValidate(index,DD);
//			memmove(refunrec.KPSday,DD.data_day,3);
			refunrec.KPSgengo = DD.data_day[0];
			refunrec.KPSday[0] = DD.data_day[1];
			refunrec.KPSday[1] = DD.data_day[2];
			refunrec.KPSday[2] = DD.data_day[3];
			TRACE("年月日\n");break;
		case ITM2_KOTEI_MNY:
			ConvEditVal( refunrec.KPSval, DD.data_val, 1);
			TRACE("金額\n");break;
		case ITM2_KOTEI_INVNO:
			//CheckInvno( 1 );
			TRACE("登録番号\n");break;
		case ITM2_KOTEI_NAME:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			TRACE("名称\n");break;
		case ITM2_KOTEI_ADDR:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			TRACE("住所\n");break;
		default:
			TRACE("ofs error!");
			break;
		}
	}
	else if( diagtype == 2 ){

		switch(ofs){
		case ITM2_TYPE:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			TRACE("種類\n");break;
		case ITM2_YEAR:
			DateValidate(index,DD);
//			memmove(refunrec.KPSday,DD.data_day,3);
			refunrec.KPSgengo = DD.data_day[0];
			refunrec.KPSday[0] = DD.data_day[1];
			refunrec.KPSday[1] = DD.data_day[2];
			refunrec.KPSday[2] = DD.data_day[3];
			TRACE("年月日\n");break;
		case ITM2_YEARKZK:
			refunrec.KPSdys = (char)DD.data_check;
			TRACE("継続\n");break;
		case ITM2_MNY:
			ConvEditVal( refunrec.KPSval, DD.data_val, 1);
			TRACE("金額\n");break;
		case ITM2_INVNO:
			//CheckInvno( 1 );
			TRACE("登録番号\n");break;
		case ITM2_NAME:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			TRACE("名称\n");break;
		case ITM2_ADDR:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			TRACE("住所\n");break;
		default:
			TRACE("ofs error!");
			break;
		}
	}
	else if( diagtype == 3 ){

		switch(ofs){
		case ITM_KOTEI_TYPE:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			TRACE("種類\n");break;
		case ITM_KOTEI_YEAR:
			DateValidate(index,DD);
//			memmove(refunrec.KPSday,DD.data_day,3);
			refunrec.KPSgengo = DD.data_day[0];
			refunrec.KPSday[0] = DD.data_day[1];
			refunrec.KPSday[1] = DD.data_day[2];
			refunrec.KPSday[2] = DD.data_day[3];
			TRACE("年月日\n");break;
		case ITM_KOTEI_MNY:
			ConvEditVal( refunrec.KPSval, DD.data_val, 1);
			TRACE("金額\n");break;
		case ITM_KOTEI_NAME:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			TRACE("名称\n");break;
		case ITM_KOTEI_ADDR:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			TRACE("住所\n");break;
		default:
			TRACE("ofs error!");
			break;
		}
	}
	else{

		switch(ofs){
		case ITM_TYPE:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			TRACE("種類\n");break;
		case ITM_YEAR:
			DateValidate(index,DD);
//			memmove(refunrec.KPSday,DD.data_day,3);
			refunrec.KPSgengo = DD.data_day[0];
			refunrec.KPSday[0] = DD.data_day[1];
			refunrec.KPSday[1] = DD.data_day[2];
			refunrec.KPSday[2] = DD.data_day[3];
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
	}
	//<---- 20230704

	//----> 20230704 del
	// (3)主な固定資産等の取得
	/*if( IsKoteiSisanDlg() ){

		switch(ofs){
		case ITM_KOTEI_TYPE:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			TRACE("種類\n");break;
		case ITM_KOTEI_YEAR:
			DateValidate(index,DD);
//			memmove(refunrec.KPSday,DD.data_day,3);
			refunrec.KPSgengo = DD.data_day[0];
			refunrec.KPSday[0] = DD.data_day[1];
			refunrec.KPSday[1] = DD.data_day[2];
			refunrec.KPSday[2] = DD.data_day[3];
			TRACE("年月日\n");break;
		case ITM_KOTEI_MNY:
			ConvEditVal( refunrec.KPSval, DD.data_val, 1);
			TRACE("金額\n");break;
		case ITM_KOTEI_NAME:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSnup,refunrec.KPSnlw,KPSnLN);
			TRACE("名称\n");break;
		case ITM_KOTEI_ADDR:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSaup,refunrec.KPSalw,KPSaLN);
			TRACE("住所\n");break;
		default:
			TRACE("ofs error!");
			break;
		}
	}
	else{
		switch(ofs){
		case ITM_TYPE:
			ZyougedanStringToBuf(DD.data_edit,refunrec.KPSsup,refunrec.KPSslw,KPSsLN);
			TRACE("種類\n");break;
		case ITM_YEAR:
			DateValidate(index,DD);
//			memmove(refunrec.KPSday,DD.data_day,3);
			refunrec.KPSgengo = DD.data_day[0];
			refunrec.KPSday[0] = DD.data_day[1];
			refunrec.KPSday[1] = DD.data_day[2];
			refunrec.KPSday[2] = DD.data_day[3];
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
	}*/
	//<---- 20230704 del
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

//-----------------------------------------------
// 「(3)固定資産等の取得」ダイアログかどうか
//-----------------------------------------------
// 返送値	0		：	固定資産以外
//			1		：	固定資産
//-----------------------------------------------
int CDiagCtlTorihiki::IsKoteiSisanDlg()
{
	int st;
	switch( diag.GetDlgCtrlID() ){
		case IDC_ICSDIAGCTRL_MEISAI_H4:		st = 1;	break;
		case IDC_ICSDIAGCTRL_MEISAI_K4:		st = 1;	break;
		default:							st = 0;	break;
	}
	return st;
}

//-----------------------------------------------
// diagの種別を取得　20230704
//-----------------------------------------------
// 返送値	diagtype：	1 固定資産（インボイス改正で登録番号列が追加されたもの）
//					：	2 棚卸資産（インボイス改正で登録番号列が追加されたもの）
//					：	3 固定資産（インボイス改正前）
//					：	4 課税資産、棚卸資産（インボイス改正前）
//-----------------------------------------------
int CDiagCtlTorihiki::GetDiagType()
{
	int eymd = 0;
	if( !( pSnHeadData->Sn_SKKBN % 2 ))		eymd = pSnHeadData->Sn_MDAYE;
	else									eymd = pSnHeadData->Sn_KDAYE;

	int diagtype = 0;
	int id = diag.GetDlgCtrlID();
	if( id == IDC_ICSDIAGCTRL_MEISAI_H1 ||
		id == IDC_ICSDIAGCTRL_MEISAI_K1 ){
		diagtype = 4;
	}
	else if( id == IDC_ICSDIAGCTRL_MEISAI_H3 ||
			 id == IDC_ICSDIAGCTRL_MEISAI_K3 ){
		if( eymd >= ICS_SH_INVOICE_DAY )	diagtype = 2;
		else								diagtype = 4;
	}
	else if( id == IDC_ICSDIAGCTRL_MEISAI_H4 ||
			 id == IDC_ICSDIAGCTRL_MEISAI_K4 ){
		if( eymd >= ICS_SH_INVOICE_DAY )	diagtype = 1;
		else								diagtype = 3;		
	}

	return diagtype;
}

//住所検索
void CDiagCtlTorihiki::HOME(int index) {

	char	buf[512] = { 0 };
	int		len = 0;
	CString	ZipBf, AddBf;

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	//----> 20230704
	int itmidx = 0;
	int diagtype = GetDiagType();
	if( diagtype == 1 ){
		itmidx = ITM2_KOTEI_ADDR;
	}
	else if( diagtype == 2 ){
		itmidx = ITM2_ADDR;
	}
	else if( diagtype == 3 ){
		itmidx = ITM_KOTEI_ADDR;
	}
	else{
		itmidx = ITM_ADDR;
	}

	if (GetCurrentcolumn(index) == itmidx) {
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
	//<---- 20230704

	//----> 20230704 del
	/*if (IsKoteiSisanDlg()) {
		//固定資産ダイアグ
		if (GetCurrentcolumn(index) == ITM_KOTEI_ADDR) {
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
	}
	else {
		//固定資産以外
		if(GetCurrentcolumn(index) == ITM_ADDR){
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
	}*/
	//<---- 20230704 del

	return;
}

//-----------------------------------------------
// 登録番号のチェックとセット　20230704
//-----------------------------------------------
// 引数		sgn		：	0　科目選択をマウスクリックした場合
//					：　1　diagに入力した場合
//					：	2　F4プレビューやタブ移動など
//					：	3　処理終了
//			zd		：	Zmselデータ
//-----------------------------------------------
bool CDiagCtlTorihiki::CheckInvno( int sgn, ZmselDBData *zd )
{
	bool isNotError = true;
	int index = diag.GetPosition();
	CDiagData DD;
	DD.data_imgdata = NULL;


	if( sgn == 0 ){

		// 科目選択をマウスクリックした場合
		if( strlen( zd->sel_name ) > 0 ){
			DD.data_edit.Format( zd->sel_name );
			diag.SetData( index, DD );
			diag.SetPosition( index );
		}
	}
	else if( sgn == 1 ){
	
		// diagに入力した場合
		if( IsTorihikiInvnoClm( index ) == FALSE ){
			return true;
		}

		diag.GetData( index, DD );

		if( DD.data_edit.GetLength() > 0 ){

			if( pZmSel && pZmSel->IsDisplay() != FALSE && DD.data_edit.GetLength() < 3 ){
				// 2桁の場合は科目選択から
				ZmselDBData tmpzd;
				int invno = atoi( DD.data_edit );
				pZmSel->InputData( 1, invno, -1, ZSEL_INVNO );
				pZmSel->ResultData( &tmpzd );
				if( tmpzd.errflg != -1 ){
					DD.data_edit.Format( tmpzd.sel_name );
					diag.SetData( index, DD );
				}
				else{
					// 科目選択にないものはチェックする
					CString chkInvno;
					chkInvno.Format( "T%s", DD.data_edit );
					if( m_SyzInvoice.CheckInvoiceNumber( chkInvno ) == -1 ){
						//DD.data_edit.Empty();
						//diag.DataClear( index, TRUE );
						ICSMessageBox( m_SyzInvoice.GetErrorMessage(), MB_ICONSTOP );
						isNotError = false;
					}
				}
			}
			else{
				// 3桁以上の場合は手入力と判断
				CString chkInvno;
				chkInvno.Format( "T%s", DD.data_edit );
				if( m_SyzInvoice.CheckInvoiceNumber( chkInvno ) == -1 ){
					//DD.data_edit.Empty();
					//diag.DataClear( index, TRUE );
					ICSMessageBox( m_SyzInvoice.GetErrorMessage(), MB_ICONSTOP );
					isNotError = false;
				}
			}
		}
	}
	else if( sgn == 2 ){
		// F4プレビューやタブ移動など
		// 本来はメッセージを出して処理を止めるのが全体の仕様だが、メッセージが何度も出てしまうタイミングがあり
		// 一旦メッセージは出さずにクリアする形で対応する（勘定科目内訳書と統一）
		if( IsTorihikiInvnoClm( index ) == FALSE ){
			return true;
		}
		diag.GetData( index, DD );
		CString chkInvno;
		chkInvno.Format( "T%s", DD.data_edit );
		if( m_SyzInvoice.CheckInvoiceNumber( chkInvno ) == -1 ){
			diag.DataClear( index, TRUE );
			isNotError = false;
		}
	}
	else if( sgn == 3 ){
		// 処理終了時
		// 本来はメッセージを出して処理を止めるのが全体の仕様だが、メッセージが何度も出てしまうタイミングがあり
		// 一旦メッセージは出さずにクリアする形で対応する（勘定科目内訳書と統一）
		// sgn==2と同じ処理だとうまくいかなかったため全てチェックする形にする
		int start_idx = 0, end_idx = 0;
		switch( diag.GetDlgCtrlID() ){
			case IDC_ICSDIAGCTRL_MEISAI_H3:
				start_idx = HT1_TYPE;
				end_idx	  = HT5_ADDR;	
				break;
			case IDC_ICSDIAGCTRL_MEISAI_H4:
				start_idx = HK1_TYPE;
				end_idx	  = HK10_ADDR;	
				break;
			case IDC_ICSDIAGCTRL_MEISAI_K3:
				start_idx = KT1_TYPE;
				end_idx	  = KT5_ADDR;
				break;
			case IDC_ICSDIAGCTRL_MEISAI_K4:
				start_idx = KK1_TYPE;
				end_idx	  = KK5_ADDR;	
				break;
		}

		for( int i = start_idx; i <= end_idx; i++ ){
			if( IsTorihikiInvnoClm( i ) ){
				diag.GetData( i, DD );
				CString chkInvno;
				chkInvno.Format( "T%s", DD.data_edit );
				if( m_SyzInvoice.CheckInvoiceNumber( chkInvno ) == -1 ){
					DD.data_edit.Empty();
					diag.DataClear( i, TRUE );
					int rowidx = GetItemLine( i );
					REFUNDREC& refunrec = syzdata->GetRec( rowidx );
					memmove( refunrec.KPInvno, DD.data_edit, sizeof(refunrec.KPInvno) );
					isNotError = false;
				}	
			}
		}
	}

	// 終了処理時は更新しない
	// データ更新
	if( sgn != 3 ) {
		if( isNotError ) {
			int rowidx = GetItemLine( index );
			REFUNDREC& refunrec = syzdata->GetRec( rowidx );
			memmove( refunrec.KPInvno, DD.data_edit, sizeof(refunrec.KPInvno) );
		}
	}

	return isNotError;
}

//-----------------------------------------------
// 登録番号列かどうか　20230704
//-----------------------------------------------
// 引数		index	：	ダイアグインデックス
//-----------------------------------------------
BOOL CDiagCtlTorihiki::IsTorihikiInvnoClm( int index )
{
	BOOL sgn = FALSE;

	int clm = GetCurrentcolumn( index );

	int itmidx = -1;
	int diagtype = GetDiagType();
	if( diagtype == 1 ){
		itmidx = ITM2_KOTEI_INVNO;
	}
	else if( diagtype == 2 ){
		itmidx = ITM2_INVNO;
	}

	if( itmidx == clm ){
		sgn = TRUE;
	}

	TRACE("★index = %d, clm = %d itmidx = %d sgn = %d\n", index, clm, itmidx, sgn );

	return sgn;
}
