class CDiagCtlRiyuu : public CDiagControlBase
{
protected:
	virtual int ControlChange(int index,int flg);
	virtual void EditOff(int index);
	virtual void DispInit();

public:
	CDiagCtlRiyuu(CIcsdiagctrl& dg);
	virtual void SetInitPos();
};