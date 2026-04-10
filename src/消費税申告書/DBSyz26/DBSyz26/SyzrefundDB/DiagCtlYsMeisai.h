class CDiagCtlYsMeisai : public CDiagCtlMeisai
{
protected:
	virtual void SetItemData(int line,int ofs);
	virtual void DispInit();
//2016.02.23 INSERT START
	virtual BOOL Del_Message(int sel_idx);
	virtual BOOL Line_BlankJugde(int line_idx);
//2016.02.23 INSERT END

public:
	CDiagCtlYsMeisai(CIcsdiagctrl& dg, int l_max,int s_index);
};