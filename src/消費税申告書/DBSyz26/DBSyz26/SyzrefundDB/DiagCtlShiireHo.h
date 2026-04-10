class CDiagCtlShiireHo : public CDiagCtlShiire
{
protected:
	virtual void EditOff(int index);
	virtual void DispInit();
	virtual void CalqDisp();
	virtual void Delete(int index);
	CSyzUtil		m_Util;
public:
	CDiagCtlShiireHo(CIcsdiagctrl& dg);
	virtual void SetInitPos();
};