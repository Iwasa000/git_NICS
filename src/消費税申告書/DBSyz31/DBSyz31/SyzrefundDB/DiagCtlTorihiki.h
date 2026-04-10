
//----> 20230704
#include <SyzInvoice.h>
#include <imgmng8.h>
#include <ZmselDB.h>
//<---- 20230704

class CDiagCtlTorihiki : public CDiagCtlMeisai
{
protected:
	void DateValidate(int index,CDiagData& DD);
	virtual int ControlChange(int index,int flg);
	virtual void SetItemData(int index );
	virtual void SetItemData(int line,int ofs);

	virtual void DispInit();
	void TorihikiChange(int flg);
	int	 IsKoteiSisanDlg();

	//ZŠŒŸõ
	virtual void HOME(int index);

	//----> 20230704
	virtual bool CheckInvno( int sgn, ZmselDBData *zd = NULL );
	virtual BOOL IsTorihikiInvnoClm( int index );
	int			 GetDiagType();

	CSyzInvoice m_SyzInvoice;
	//<---- 20230704

public:
	CDiagCtlTorihiki(CIcsdiagctrl& dg, int l_max, int s_index, int item_cnt);
	virtual void SetInitPos();
};
