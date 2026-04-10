#pragma once
#include "icsreportpaintmanager.h"

class ICSReportPaintManagerEX :
	public ICSReportPaintManager
{
	ICSReportControl* pControl;    // 親コントロールICSReportControlクラスポインタ

public:

	ICSReportPaintManagerEX(ICSReportControl* report);
	~ICSReportPaintManagerEX(void);

	virtual void FillHeaderControl(CDC* pDC, CRect rcHeader)
	{
//		if( !pControl->IsWindowEnabled() )
//			m_clrHeaderControl.SetCustomValue( RGB(218,218,218) );
		ICSReportPaintManager::FillHeaderControl(pDC, rcHeader);
	}

	virtual void DrawFocusedRow(CDC* pDC, CRect rcRow)
	{
		ICSReportPaintManager::DrawFocusedRow(pDC, rcRow);
	}

	virtual void DrawColumn(CDC* pDC, ICSReportColumn* pColumn, ICSReportHeader* pHeader, CRect rcColumn, BOOL bDrawExternal = FALSE)
	{
		ICSReportPaintManager::DrawColumn( pDC, pColumn, pHeader, rcColumn, bDrawExternal );
	}

	virtual void DrawItemCaption(ICS_REPORTRECORDITEM_DRAWARGS* pDrawArgs, ICS_REPORTRECORDITEM_METRICS* pMetrics)
	{
		if( !pControl->IsWindowEnabled() )
			pDrawArgs->pDC->SetTextColor(RGB(109,109,109));
		ICSReportPaintManager::DrawItemCaption( pDrawArgs, pMetrics);
	}

	BOOL& GetSetHideSelection(){
		return m_bHideSelection;
	}

};
