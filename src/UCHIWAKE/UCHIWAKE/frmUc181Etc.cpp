
#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc181Etc.h"


IMPLEMENT_DYNCREATE(CfrmUc181Etc, CfrmUc000Common)


BEGIN_MESSAGE_MAP(CfrmUc181Etc, CfrmUc000Common)
END_MESSAGE_MAP()


CfrmUc181Etc::CfrmUc181Etc()
	: CfrmUc000Common(CfrmUc181Etc::IDD)
{
}

CfrmUc181Etc::~CfrmUc181Etc()
{
}

void CfrmUc181Etc::DoDataExchange(CDataExchange* pDX)
{
	CfrmUc000Common::DoDataExchange(pDX);
}

void CfrmUc181Etc::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();
}

#ifdef _DEBUG
void CfrmUc181Etc::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc181Etc::Dump(CDumpContext& dc) const
{
	CfrmUc000Common::Dump(dc);
}
#endif
#endif //_DEBUG

