#ifndef _DIAG_CONTROL_BASE_
#define _DIAG_CONTROL_BASE_

#include "icsdiagctrl.h"
#include "DBSyzShinMain.h"
#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"

#include "DBSyzShinMainView.h"
class CDiagData : public DIAGRAM_DATA
{
public:
	CDiagData(){
		data_combo =  data_imgwidth = data_imgheight = movepos = data_check = 0;
		data_imgdata = 0;
		memset(data_val,0,6);
		memset(data_day,0,4);
	}

    operator LPUNKNOWN() {
        return (LPUNKNOWN)this;
    }
};

class CDiagAttribute : public DIAGRAM_ATTRIBUTE 
{
public:
	CDiagAttribute(){
		attr_bcolor  = attr_fcolor = attr_frcolor   = attr_lfcolor = attr_lbcolor = 0;
		attr_ofstx   = attr_ofsty   = attr_align   = attr_linestyle = attr_linewidth = 
		attr_rframe  = attr_rwidth  = attr_rheight = attr_inpkind = attr_inpmax   = attr_linemax = 
		attr_frctmax = attr_editkind = attr_editattr = attr_dayattr = 0;
	}

    operator LPUNKNOWN() {
        return (LPUNKNOWN)this;
    }
};

class CSyzDataSet
{
	REFUNDVOL& refundvol;

public:

	CSyzDataSet(REFUNDVOL& vol, char* c = 0,char* r = 0) : refundvol(vol), Check(c),Radio(r)
	{
	}

	CSyzDataSet(REFUNDVOL& vol,REFUNDREC* rec, int rec_cnt, char* c = 0,char* r = 0) : refundvol(vol), Check(c),Radio(r)
	{
		for(int i = 0;i<rec_cnt;i++)
			MeisaiDataArray.Add(rec + i);
	}

	virtual ~CSyzDataSet()
	{
	}

	char *Check, *Radio;
	REFUNDVOL& Getvol() {return refundvol;}

	CArray<REFUNDREC*,REFUNDREC*> MeisaiDataArray;

	REFUNDREC& GetRec(int line)
	{
		return *MeisaiDataArray[line];
	}
};

class CDiagControlBase
{
public:
//	static CDBSyzShinView *pSyzShin;
	static CDBSyzShinMainView *pSyzShin;

protected:
	
	
	CArith arith;

	CSyzDataSet* syzdata;
	CIcsdiagctrl& diag;

	CDiagControlBase* prev_diag_control, *next_diag_control;
	CMap<int,int,RECT,RECT> PrevCrossOver,NextCrossOver;

	CMap<int,int,RECT,RECT> PositionMap;
	CMap<int,int,int,int>   RadioMap;
	
	virtual int ControlChange(int index,int flg){ return 0; }

	short CheckInputKey(short nChar)
	{
		if( !( nChar==VK_DOWN || nChar==VK_UP || nChar==VK_RIGHT ||
			   nChar==VK_LEFT || nChar==VK_RETURN || nChar==VK_TAB ||
			   nChar==VK_F2   || nChar==VK_F3 ) ) return 0;

		short key = nChar;

		int shift = ::GetKeyState(VK_SHIFT)&0x80;
		if( nChar == VK_F3 || nChar == VK_RETURN || ( nChar == VK_TAB && !shift ) )
			key = VK_RIGHT;
		if( nChar == VK_F2 || nChar == VK_TAB && shift )
			key = VK_LEFT;

		return key;
	}

	int SerchNextPos(int& index,RECT& move,LONG RECT::*ptr)
	{
		CDiagAttribute DA;

		int st = TRUE;
		do{
			if(	st = PositionMap.Lookup(index,move) ) {
				if( move.*ptr < 0 ){ st = 0; break; }
				diag.GetAttribute( (short)(move.*ptr), DA );
				index = move.*ptr;
				if( DA.attr_bcolor == BKCOLORW || DA.attr_bcolor == BKCOLORG ) continue;
				return 1;
				break;
			}
		}while(st);

		return 0;
	}

	int ChangeRadioButton(int index,LONG RECT::*ptr)
	{
		CDiagData DD;
		CDiagAttribute DA;
	
		diag.GetAttribute( index, DA );
		if(DA.attr_inpkind != 3 ) return 0;

		if( index < RadioMap[index] && ptr == &RECT::right ){
			DD.data_check = 0;
			diag.SetData( index, DD );
			DD.data_check = 1;
			diag.SetData( RadioMap[index], DD );
			diag.SetPosition(RadioMap[index]);
			return 1;
		}
		if( index > RadioMap[index] && ptr == &RECT::left ) {
			DD.data_check = 1;
			diag.SetData( RadioMap[index], DD );
			DD.data_check = 0;
			diag.SetData( index, DD );
			diag.SetPosition(RadioMap[index]);
			return 1;
		}
		return 0;
	}

	void RadioNextPos(int index,int next,RECT& move)
	{
		CDiagData DD;
		CDiagAttribute DA;

		diag.GetAttribute( index, DA );
		if( DA.attr_inpkind == 3 ){
			diag.GetData( index, DD );
			if( DD.data_check && next == RadioMap[index] )
				move = PositionMap[next];
		}
	}

	COLORREF GetBkcolor(int flg = 0)
	{
		if( flg ) return BKCOLORW;

		if(	pSyzShin->m_pSnHeadData->Sn_Sign4&0x01 )
			return BKCOLORG;

		return BKCOLORW;
	}

	void SetAttribMod(int index,int flg, int colorflg = 0 )
	{
		COLORREF color = flg ? EDCOLOR : GetBkcolor(colorflg);
		int mdfy =  flg ? DIAG_MDFY_EDIT : DIAG_MDFY_READONLY;
	
		SetAttribMod(index,color, mdfy);
	}


	void SetAttribMod(int index,COLORREF color, int mdfy)
	{
		CDiagAttribute DA;

		diag.GetAttribute( index, DA );
		DA.attr_bcolor = color;
		diag.SetAttribute( index, DA ,TRUE);
		diag.ModifyItem( index, mdfy );
	}

	void SetZenHan(int index)
	{
		CDiagAttribute DA;

		diag.GetAttribute( index, DA );
		if(DA.attr_editkind == 2){
			DA.attr_editattr |= DIAG_EATTR_ZENHAN;
			diag.SetAttribute( index, DA, TRUE);
		}
	}

	int IsEditable(int index)
	{
		CDiagAttribute DA;

		diag.GetAttribute( index, DA );
		return DA.attr_inpkind == 0;
	}

	inline CString MKString(char* str,int len)
	{
		char temp[256] = {0};
		memmove(temp,str,len);

		return temp;
	}
	
	void StringToBuf(CString& str,char* buf1,int len)
	{
		char temp[256] = {0};
		
		memmove(temp,str,str.GetLength());
		memmove(buf1,temp,len);
	}

	void ZyougedanStringToBuf(CString& str,char* buf1,char* buf2,int len)
	{
		char temp[256] = {0};
		
		memset(buf1,0,len);
		memset(buf2,0,len);
		
		int kaigyo = str.Find("\r\n");

		if( kaigyo < 0){

			strcpy_s(temp,str);
			memmove(buf1,temp,len);

		}else{
			char temp[128] = {0};
			CString jodan = str.Left(kaigyo);
			CString gedan = str.Right(str.GetLength() - kaigyo - 2);

			strcpy_s(temp,jodan);
			memmove(buf1,temp,len);
			strcpy_s(temp,gedan);
			memmove(buf2,temp,len);
		}
	}

	CString ZyougedanBufToString(char* buf1,char* buf2,int len)
	{
		CString str1 = MKString(buf1,len);
		CString str2 = MKString(buf2,len);

		if( !str2.GetLength() )
			return str1;

		return str1 + "\r\n" + str2;
	}

	void ConvEditVal(char* val1,char* val2,int mul_div)
	{
		char	sen[6];
		arith.l_input( sen, syzdata->Getvol().KPType == 1 ? "1000" : "1" );
		
		if(mul_div){
			memmove(val1,val2,6);
			arith.l_mul(val1,val1,sen);
		}else{
			memmove(val1,val2,6);
			arith.l_div(val1,val1,sen);
		}
	}

public:
	CDiagControlBase(CIcsdiagctrl& dg) : diag(dg), syzdata(0), arith(0x16), prev_diag_control(0), next_diag_control(0) 
	{
	}

	virtual ~CDiagControlBase()
	{
		delete syzdata;
	}

	virtual void EditOff(int index) = 0;
	virtual void DispInit() = 0;
	virtual void SetInitPos(){}

	virtual void Delete(int index)
	{
		diag.DataClear(index,TRUE);
	}

	int Termination(int index,short nChar)
	{
		if(nChar == VK_DELETE){
			if(IsEditable(index)) Delete(index);
			return 0;
		}

		short key = CheckInputKey(nChar);
		if( !key ) return 0;

		if( nChar==VK_UP || nChar== VK_DOWN ) {
			if( IsCombobox(index) ) return 0;
		}

		int next = index;
		LONG RECT::*ptr;

		switch(key){
		case VK_UP:	   ptr = &RECT::top;    break;
		case VK_DOWN:  ptr = &RECT::bottom; break;
		case VK_LEFT:  ptr = &RECT::left;   break;
		case VK_RIGHT: ptr = &RECT::right;  break;
		}

		if( nChar==VK_LEFT || nChar== VK_RIGHT ) {
			if( ChangeRadioButton(index,ptr) ) return 1;
		}

		RECT move;
		if( SerchNextPos(next,move,ptr) ){
			RadioNextPos(index,next,move);
			SetPosition(move,ptr);
		}

		if( move.*ptr == MOVE_PREV && prev_diag_control )
			prev_diag_control->SetPosition(PrevCrossOver[next],ptr);

		if( move.*ptr == MOVE_NEXT && next_diag_control )
			next_diag_control->SetPosition(NextCrossOver[next],ptr);

		return 1;
	}

	void SetPosition(CRect move,LONG RECT::*ptr)
	{
		CDiagAttribute DA;
		int index = move.*ptr;
	
		diag.GetAttribute(index,DA );
		if( DA.attr_inpkind == 3 ){
			CDiagData DD;
			diag.GetData( index, DD );
			if( !DD.data_check ){
				diag.SetPosition(RadioMap[index]);
				return;
			}
		}
		if( DA.attr_bcolor == BKCOLORG || DA.attr_bcolor == BKCOLORW ){
			RECT move;
			if( !SerchNextPos(index,move,ptr) ) return;
		}
		diag.SetPosition(index);
	}

	virtual void check(int index,DIAGRAM_DATA* dd)
	{
		CDiagData DD;
		CDiagAttribute DA;
	
		diag.GetAttribute( index, DA );
		if( DA.attr_inpkind == 3 ){
			int f = RadioMap[index];
			int s = RadioMap[f];
			diag.GetData( f, DD );

			DD.data_check = !DD.data_check;
			diag.SetData( f, DD );
			DD.data_check = !DD.data_check;
			diag.SetData( s, DD );
		}else{
			if( ControlChange(index,dd->data_check) ){
				dd->data_check = !dd->data_check;
				diag.SetData( index, (LPUNKNOWN)dd );
			}
			diag.SetPosition( index );
		}
	}

	void SetCrossOverPrev(CDiagControlBase* prev, pair<int,CRect> list[], int count )
	{
		prev_diag_control = prev;
		for(int i = 0;i<count;i++)
			PrevCrossOver.SetAt(list[i].first,list[i].second);
	}

	void SetCrossOverNext(CDiagControlBase* next, pair<int,CRect> list[], int count )
	{
		next_diag_control = next;
		for(int i = 0;i<count;i++)
			NextCrossOver.SetAt(list[i].first,list[i].second);
	}

	int IsCombobox(int index)
	{
		CDiagAttribute DA;

		diag.GetAttribute( index, DA );
		return DA.attr_inpkind == 1;
	}

	void SetSyzData(CSyzDataSet* syz)
	{
		syzdata = syz;
	}
};

#endif