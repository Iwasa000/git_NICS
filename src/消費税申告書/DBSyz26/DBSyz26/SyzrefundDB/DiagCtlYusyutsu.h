class CDiagCtlYusyutsu : public CDiagControlBase
{
protected:
	virtual void EditOff(int index);
	virtual void DispInit();
	virtual int ControlChange(int index,int flg);
public:
	CDiagCtlYusyutsu(CIcsdiagctrl& dg);
};