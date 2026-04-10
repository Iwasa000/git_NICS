class CDiagCtlTorihiki : public CDiagCtlMeisai
{
protected:
	void DateValidate(int index,CDiagData& DD);
	virtual int ControlChange(int index,int flg);
	virtual void SetItemData(int index );
	virtual void SetItemData(int line,int ofs);
	virtual void CnvYmd(unsigned char* pSday, int* pDGengo, unsigned char* pDday);

	virtual void DispInit();
	void TorihikiChange(int flg);

public:
	CDiagCtlTorihiki(CIcsdiagctrl& dg, int l_max,int s_index);
	virtual void SetInitPos();
};
