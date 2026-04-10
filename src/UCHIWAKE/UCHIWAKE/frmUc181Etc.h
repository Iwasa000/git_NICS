
#pragma once

#include "frmUc000Common.h"

class CfrmUc181Etc: public CfrmUc000Common
{
	DECLARE_DYNCREATE(CfrmUc181Etc)

protected:
	CfrmUc181Etc();
	virtual ~CfrmUc181Etc();

public:
	enum{ IDD = IDD_YOUSHIKI181 };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()

};

