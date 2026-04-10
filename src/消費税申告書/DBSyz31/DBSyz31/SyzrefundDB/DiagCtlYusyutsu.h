class CDiagCtlYusyutsu : public CDiagControlBase
{
protected:
	virtual void EditOff(int index);
	virtual void DispInit();
	virtual int ControlChange(int index,int flg);

	//ZŠŒŸõ
	virtual void HOME(int index);

public:
	CDiagCtlYusyutsu(CIcsdiagctrl& dg);
};