#include "stdafx.h"
#include "DiagCtlMeisai.h"
#include "DiagCtlTorihiki.h"
#include "DiagCtlKotei.h"

CDiagCtlKotei::CDiagCtlKotei(CIcsdiagctrl& dg, int l_max, int s_index, int item_cnt )
	: CDiagCtlTorihiki( dg, l_max, s_index, item_cnt )
{
	for(int line = 0;line < line_max; line++ )
		SetAttribMod( GetCurrentIndex(line,2),GetBkcolor(),DIAG_MDFY_TRANSPARENT);
}