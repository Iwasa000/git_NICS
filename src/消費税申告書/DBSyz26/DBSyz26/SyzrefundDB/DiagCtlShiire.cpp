#include "stdafx.h"
#include "DiagCtlShiire.h"

CDiagCtlShiire::CDiagCtlShiire(CIcsdiagctrl& dg)
	: CDiagControlBase(dg)
{
}

char* CDiagCtlShiire::GetMnyBuf(int index)
{
	char* money = 0;
	MnyBufMap.Lookup(index,money);

	return money;
}

#ifdef _20150203_EXTEND_
char* CDiagCtlShiire::Zitsugaku(int index, char& flg, char* val)
{
	char* money = GetMnyBuf(index);

	if(!flg){
		if( arith.l_cmp( val, ZitsugakuPair.first ) ){
			flg = 1;
			money = ZitsugakuPair.second;
			MnyBufMap.SetAt( index, money );
			SetAttribMod( index, flg ? ZICOLOR : EDCOLOR, DIAG_MDFY_EDIT );
		}
	}
	
	return money;
}
#else
char* CDiagCtlShiire::Zitsugaku(int sw, int index, char& flg, char* val)
{
	char* money = GetMnyBuf(index);

	if( pUtil->GetKazeihoushiki(pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		if(!flg){
			if( sw == 0/*ñ@êl*/ ){
				char	tmpBuf[6]={0};
				memmove( tmpBuf, ZitsugakuPair.first, sizeof(tmpBuf) );
				l_cut1000( tmpBuf );
				if( arith.l_cmp( val, tmpBuf ) ){
					flg = 1;
					money = ZitsugakuPair.second;
					MnyBufMap.SetAt( index, money );
					SetAttribMod( index, flg ? ZICOLOR : EDCOLOR, DIAG_MDFY_EDIT );
				}
			}
			else if( sw == 1/*å¬êl*/ ){
				if( arith.l_cmp( val, ZitsugakuPair.first ) ){
					flg = 1;
					money = ZitsugakuPair.second;
					MnyBufMap.SetAt( index, money );
					SetAttribMod( index, flg ? ZICOLOR : EDCOLOR, DIAG_MDFY_EDIT );
				}
			}
		}
	}
	else{
		if(!flg){
			if( arith.l_cmp( val, ZitsugakuPair.first ) ){
				flg = 1;
				money = ZitsugakuPair.second;
				MnyBufMap.SetAt( index, money );
				SetAttribMod( index, flg ? ZICOLOR : EDCOLOR, DIAG_MDFY_EDIT );
			}
		}
	}
	
	return money;
}
#endif

void CDiagCtlShiire::ZitsugakuClear(int index, char& flg )
{
	flg = 0;
	memset( ZitsugakuPair.second, 0, 6 );
	MnyBufMap.SetAt( index, ZitsugakuPair.first );

	CDiagData DD;
	ConvEditVal(DD.data_val, MnyBufMap[index], 0 );
	diag.SetData( index, DD );

	SetAttribMod( index, EDCOLOR, DIAG_MDFY_EDIT );
	return;
}

void CDiagCtlShiire::CalqDisp()
{
	POSITION	pos = MnyBufMap.GetStartPosition();
	while( pos != NULL ){
		int index;	char* val;
		MnyBufMap.GetNextAssoc( pos, index, val);

		CDiagData DD;
		ConvEditVal( DD.data_val, val, 0 );
		diag.SetData(index,DD);
	}
}

void CDiagCtlShiire::SumVal(char* dst,int st,int end, char* REFUNDREC::*val )
{
	arith.l_clear( dst );

	for(int line = st;line <= end;line++){
		REFUNDREC meisai =  syzdata->GetRec(line);
		arith.l_add(dst,dst, &(meisai.*val) );
	}
}

// édì¸çTèúê≈äzÇÃåvéZ
void CDiagCtlShiire::CalqZei(char* dst,char* in,int komi_nuki)
{
	arith.l_clear( dst );

	memmove( dst, in, 6 );
	char temp[6] = {0};

	// Å~ÇS
	memset( temp, 0, sizeof(temp) );
	arith.l_input( temp, "63" );
	arith.l_mul( dst, dst, temp );

	if( komi_nuki ){	// ê≈çû
		// ÅÄÇPÇOÇT
		memset( temp, 0, sizeof(temp) );
		arith.l_input( temp, "1080" );
	}
	else{				// ê≈î≤
		// ÅÄÇPÇOÇO
		memset( temp, 0, sizeof(temp) );
		arith.l_input( temp, "1000" );
	}

	arith.l_div( dst, dst, temp );
}

void CDiagCtlShiire::l_cut1000( void *val )
{
	char	sen[6];
	arith.l_input( sen, "1000" );

	// 1000Ç≈äÑÇÈ
	arith.l_div( val, val, sen );
	// 1000î{Ç∑ÇÈ
	arith.l_mul( val, val, sen );
}

void CDiagCtlShiire::check(int index,DIAGRAM_DATA* dd)
{
	CDiagControlBase::check(index, dd);
	EditOff(index);
}