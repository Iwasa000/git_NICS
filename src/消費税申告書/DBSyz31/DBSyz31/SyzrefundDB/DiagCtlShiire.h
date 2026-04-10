class CDiagCtlShiire : public CDiagControlBase
{
protected:
	
	pair<char*,char*> ZitsugakuPair;
	CMap<int,int,char*,char*> MnyBufMap;

	char* GetMnyBuf(int index);
//-- '15.02.03 --
//	char* Zitsugaku(int index, char& flg, char* val);
//---------------
	char* Zitsugaku(int sw, int index, char& flg, char* val);
//---------------
	void  ZitsugakuClear(int index, char& flg );

	void l_cut1000( void *val );
	void CalqZei(char* dst,char* in,int komi_nuki);
	void SumVal(char* dst,int st,int end, char* REFUNDREC::*val );

	virtual void CalqDisp();
	virtual void check(int index,DIAGRAM_DATA* dd);

public:
	CDiagCtlShiire(CIcsdiagctrl& dg);
};