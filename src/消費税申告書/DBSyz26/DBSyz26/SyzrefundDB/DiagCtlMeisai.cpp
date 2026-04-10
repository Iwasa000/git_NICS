#include "stdafx.h"
#include "DiagCtlMeisai.h"

CDiagCtlMeisai::CDiagCtlMeisai(CIcsdiagctrl& dg, int i_cnt,int l_max,int s_index)
	: CDiagControlBase(dg), itm_cnt(i_cnt), line_max(l_max), start_meisai(s_index)
{
	for(int line = 0;line < line_max; line++ ){
		for(int i = 0;i < itm_cnt;i++){

			int index = GetCurrentIndex(line,i);

			int up	  = index - itm_cnt;
			int	down  = index + itm_cnt;

			int	left  = index - 1;
			int	right = index + 1;

			if( i == 0 && line == 0 ) left = MOVE_PREV;
			if( line == 0 )			  up   = MOVE_PREV;
			if( line == line_max - 1) down = MOVE_NEXT;
			if( line == line_max - 1 && i == itm_cnt - 1 ) 
									 right = MOVE_NEXT;

			CRect rec = CRect(left, up, right, down);
			PositionMap.SetAt( index , rec );

			SetZenHan(index);
			TRACE("%d,%d,%d,%d,%d\n",index,rec.top,rec.left,rec.right,rec.bottom);
		}
	}
}

int CDiagCtlMeisai::GetCurrentIndex(int line, int ofs)
{
	return start_meisai + ( itm_cnt * line ) + ofs;
}

int CDiagCtlMeisai::GetItemOfs(int index)
{
	return (index - start_meisai) % itm_cnt;
}

int CDiagCtlMeisai::GetItemLine(int index)
{
	return (index - start_meisai) / itm_cnt;
}

void CDiagCtlMeisai::EditOff(int index)
{
	if(index < start_meisai){
		SetItemData(index);
		return;
	}
	SetItemData( GetItemLine(index), GetItemOfs(index) );
}

//2016.02.23 INSERT START
void CDiagCtlMeisai::Delete(int index)
{
	if( GetItemOfs(index) == 0 ){
		if( Del_Message(index) == TRUE ){
			Line_Delete(index);
		}
		return;
	}
	if( !IsCheckBox(index) ){
		diag.DataClear(index,TRUE);
	}
	EditOff(index);
}

void CDiagCtlMeisai::Line_Delete( int sel_idx )
{

	//CDiagControlBase* control = GetDiagControl(control_id);
	int line_idx = GetItemLine(sel_idx);
	if( line_idx < 0 ){
		return;
	}

	//REFUNDREC& refunrec0 = syzdata->GetRec(line_idx);

	//CString	msg;
	//if(( control_id == IDC_ICSDIAGCTRL_MEISAI_H2 )||( control_id == IDC_ICSDIAGCTRL_MEISAI_K2 )){
	//	msg.Format( _T("取引先の氏名(名称)「%s%s」を削除します。よろしいですか？"), refunrec0.KPSnup, refunrec0.KPSnlw );
	//	//if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0, this ) == IDNO ){
	//	if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0 ) == IDNO ){
	//		return;
	//	}
	//}else{
	//	msg.Format( _T("資産の種類等「%s%s」を削除します。よろしいですか？"), refunrec0.KPSsup, refunrec0.KPSslw );
	//	//if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0, this ) == IDNO ){
	//	if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0 ) == IDNO ){
	//		return;
	//	}
	//}

	for( int l_idx = line_idx+1; l_idx < line_max; l_idx++ ){

		REFUNDREC& refunrec = syzdata->GetRec(l_idx-1);
		REFUNDREC& refunrec2 = syzdata->GetRec(l_idx);

		memmove( &refunrec, &refunrec2, sizeof(REFUNDREC));
	}

	REFUNDREC& refunrec3 = syzdata->GetRec(line_max-1);
	::ZeroMemory( &refunrec3, sizeof(REFUNDREC));

	DispInit();
}

BOOL CDiagCtlMeisai::Del_Message(int sel_idx)
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
	//msg.Format( _T("資産の種類等「%s%s」を削除します。よろしいですか？"), refunrec.KPSsup, refunrec.KPSslw );
	msg.Format( _T("資産の種類等「%s%s」の行を削除します。よろしいですか？"), refunrec.KPSsup, refunrec.KPSslw );
	//if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0, this ) == IDNO ){
	if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0 ) == IDNO ){
		return FALSE;
	}

	return TRUE;
}

void CDiagCtlMeisai::Insert(int index)
{
//	if( GetItemOfs(index) == 0 ){
		Line_Insert(index);
//	}
	return;
}

void CDiagCtlMeisai::Line_Insert( int sel_idx )
{

	//CDiagControlBase* control = GetDiagControl(control_id);

	int line_idx = GetItemLine(sel_idx);
	if( line_idx < 0 ){
		return;
	}

	//最終行が空白行か判定
	if( Line_BlankJugde(line_max-1) == FALSE ){
		ICSMessageBox( "最大行数に達しているため、行挿入はできません。", MB_OK, 0 );
		return;
	}
	//REFUNDREC& refunrec = control->syzdata->GetRec(line_max-1);
	//if(( control_id == IDC_ICSDIAGCTRL_MEISAI_H2 )||( control_id == IDC_ICSDIAGCTRL_MEISAI_K2 )){
	//	if( strlen( refunrec.KPSnup ) != 0 ){ //取引先名（上段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSnlw ) != 0 ){ //取引先名（下段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSaup ) != 0 ){ //取引先住所（上段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSalw ) != 0 ){ //取引先住所（下段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSval ) != 0 ){ //取引金額
	//		return;
	//	}else
	//	if( strlen( refunrec.KPStup ) != 0 ){ //主な取引商品（上段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPStlw ) != 0 ){ //主な取引商品（下段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSzup ) != 0 ){ //所轄税関名（上段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSzlw ) != 0 ){ //所轄税関名（下段）
	//		return;
	//	}
	//}else{
	//	if( strlen( refunrec.KPSsup ) != 0 ){ //資産の種類等（上段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSslw ) != 0 ){ //資産の種類等（下段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSday ) != 0 ){ //譲渡年月日
	//		return;
	//	}else
	//	if( refunrec.KPSdys ){ //譲渡年月日 継続サイン
	//		return;	
	//	}else
	//	if( strlen( refunrec.KPSval ) != 0 ){ //取引金額
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSnup ) != 0 ){ //取引先名（上段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSnlw ) != 0 ){ //取引先名（下段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSaup ) != 0 ){ //取引先住所（上段）
	//		return;
	//	}else
	//	if( strlen( refunrec.KPSalw ) != 0 ){ //取引先住所（下段）
	//		return;
	//	}
	//}


	for( int l_idx = line_max-1; l_idx > line_idx; l_idx-- ){

		REFUNDREC& refunrec = syzdata->GetRec(l_idx);
		REFUNDREC& refunrec2 = syzdata->GetRec(l_idx-1);

		memmove( &refunrec, &refunrec2, sizeof(REFUNDREC));
	}

	REFUNDREC& refunrec3 = syzdata->GetRec(line_idx);
	::ZeroMemory( &refunrec3, sizeof(REFUNDREC));

	DispInit();
}

BOOL CDiagCtlMeisai::Line_BlankJugde(int line_idx)
{

	REFUNDREC& refunrec = syzdata->GetRec(line_idx);

	if( strlen( refunrec.KPSsup ) != 0 ){ //資産の種類等（上段）
		return FALSE;
	}else
	if( strlen( refunrec.KPSslw ) != 0 ){ //資産の種類等（下段）
		return FALSE;
	}else
	if( strlen( refunrec.KPSday ) != 0 ){ //譲渡年月日
		return FALSE;
	}else
	if( refunrec.KPSdys ){ //譲渡年月日 継続サイン
		return FALSE;
	}else
	//if( strlen( refunrec.KPSval ) != 0 ){ //取引金額
	if( arith.l_test(refunrec.KPSval) != 0 ){
		return FALSE;
	}else
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
	}

	return TRUE;
}

BOOL CDiagCtlMeisai::IsLineHead(int index)
{
	if( index < start_meisai ){
		return FALSE;
	}
	if( GetItemOfs(index) == 0 ){
		return TRUE;
	}
	return FALSE;
}
//2016.02.23 INSERT END