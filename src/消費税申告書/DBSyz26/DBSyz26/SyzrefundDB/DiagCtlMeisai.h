class CDiagCtlMeisai : public CDiagControlBase
{
protected:
	int itm_cnt,line_max,start_meisai;
	int GetCurrentIndex(int line, int ofs);

	int GetItemOfs(int index);
//2016.02.23 UPDATE START
//	int GetItemLine(int index);
	virtual int GetItemLine(int index);
//2016.02.23 UPDATE END

	virtual void EditOff(int index);

	virtual	void SetItemData(int index ){}
	virtual	void SetItemData(int line, int ofs) = 0;

//2016.02.23 INSERT START
	virtual void Delete(int index);
	virtual BOOL Del_Message(int index);
	virtual void Line_Delete(int line_idx);

	virtual void Insert(int index);
	virtual void Line_Insert(int line_idx);
	virtual BOOL Line_BlankJugde(int line_idx);
	virtual BOOL IsLineHead(int index);
//2016.02.23 INSERT END

public:
	CDiagCtlMeisai(CIcsdiagctrl& dg,int i_cnt,int l_max,int s_index);
};
