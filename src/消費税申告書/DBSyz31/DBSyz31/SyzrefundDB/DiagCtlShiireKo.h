class CDiagCtlShiireKo : public CDiagCtlShiire
{
protected:
	virtual void EditOff(int index);
	virtual void DispInit();
	virtual void CalqDisp();
	virtual void Delete(int index);

public:
	CDiagCtlShiireKo(CIcsdiagctrl& dg);
	virtual void SetInitPos();
};