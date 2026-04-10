class CDiagCtlTokusyu : public CDiagControlBase
{
protected:
	virtual void EditOff(int index);
	virtual void DispInit();

public:
	CDiagCtlTokusyu(CIcsdiagctrl& dg,int flg);
	virtual void SetPos();
};
