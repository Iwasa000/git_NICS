#pragma once
#include "icsreportcontrol.h"
#include "ICSReportPaintManagerEX.h"

using namespace std;

template<class TREC>
class ICSReportControlEx : public ICSReportControl
{
protected:

	virtual void SetReportColumn(int width) = 0;
	virtual ICSReportRecord* GetReportRecord(TREC rec) = 0;

	virtual COLORREF SetColor()
	{
		return RGB(240,240,240);
	}


	virtual void SetGrid()
	{
		ICSReportGridStyle	styleHorPer = RGS_SMALLDOT;
		SetGridPerCount( TRUE, 1, RGB(0,0,0), styleHorPer );
		SetGridPerCount( FALSE, 1, RGB(100,100,100), styleHorPer );
	}

	void AddNewRecord(int index)
	{
		InsertRecordAt(index,GetReportRecord(TREC()) );
		SetFocusedRow( index );
	};

	void DeleteRecord(int index)
	{
		RemoveRecordAt( index );
		SetFocusedRow( min(GetRows()->GetCount() - 1,index ) );
	}

public:

	typedef typename TREC RecType;
	typedef typename vector<TREC> VectorType;

	ICSReportControlEx()
	{
	}
	
	virtual ~ICSReportControlEx()
	{
	}

	virtual int Init(CFont* font,int hedder = TRUE)
	{
		RemoveColumn(-1);
		RemoveAllRecords();
		Resize( TRUE, ICSResizeDefault );

		LOGFONT Logfont;
		font->GetLogFont( &Logfont );
		SetHeaderFont( Logfont );
		SetTextFont( Logfont );

		CRect rect;
		GetClientRect( &rect );
		MapWindowPoints(GetParent(),&rect);

		SetReportColumn( rect.Width() );

		AllowEdit(FALSE);
		EditOnClick( TRUE );
		SetRemovable( FALSE );
		SetSortable( FALSE );
		SetDraggable( FALSE );
		ShowHeader(hedder);

		SetGrid();
		return 0;
	}

	virtual void AddRecordArray(VectorType& vec,int remove = TRUE)
	{
		if(remove)	RemoveAllRecords();
		for(VectorType::iterator it = vec.begin(); it != vec.end();it++)
			GetRecords()->Add( GetReportRecord(*it) );
		Populate();
	}

	virtual void AddNewRecord()
	{
		SetFocus();
		ICSReportRow* row = GetFocusedRow();
		int index = row ? row->GetIndex() : GetRows()->GetCount();
		AddNewRecord(index);
	};

	pair<int,int> GetCurrentRowCoulmn()
	{
		int coulmnindex = -1;
		ICSReportColumn* coulmn = GetFocusedColumn();
		if( coulmn ) coulmnindex = coulmn->GetIndex();

		int rowindex = -1;
		ICSReportRow* row = GetFocusedRow();
		if( row ) rowindex = row->GetIndex();

		return pair<int,int>(rowindex,coulmnindex);
	}
};

class HistoryData
{
	CSnset snset;
	CTime update;

public:
	HistoryData()
	{
	}

	HistoryData( CSnset set, CTime u ) : snset(set),update(u)
	{
		str_kubun = set.GetKubunStr();

		TimePair& date = set.IsTyukanShinkoku() ? set.GetSetTyukanKikan() : set.GetSetKessanKikan();
		str_kikan = CTimeControl::GetDateStrPair(date);

		if( u != CTime() )	str_saisyu = CTimeControl::GetDateStr( u, "%2d年%2d月%2d日" );

		if(! (set.S_SGN2 & 0x0f) && !(set.m_DispTabSgn & 0x2000) )
			str_koujo = (set.S_SGN4&0x80) ? "個別対応" : "比例配分";
	}

	CSnset& GetSnset()
	{
		return snset;
	}

	CTime& GetUpdate()
	{
		return update;
	}

	CString	str_kubun,	str_kikan,	str_kanryo,	str_saisyu, str_koujo;
};

class CHistoryReport : public ICSReportControlEx<HistoryData>
{ 
public:
	enum ColumnID
	{
		kubun,
		kikan,
		koujo,
		saisyu,
	};

	void SetRecordText(int rowindex,int coulmnindex,CString str)
	{
		ICSReportRecordItemText* text = (ICSReportRecordItemText*)GetRecords()->GetAt(rowindex)->GetItem(coulmnindex);
		text->SetCaption(str);
		RedrawControl();
	}

protected:

	virtual ICSReportRecord* GetReportRecord(RecType r)
	{
		ICSReportRecord* rec  = new ICSReportRecord();
		rec->AddItem( new ICSReportRecordItemText( r.str_kubun ,ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII, FALSE, TRUE, 80) );
		rec->AddItem( new ICSReportRecordItemText( r.str_kikan ,ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII, FALSE, TRUE, 80) );
		rec->AddItem( new ICSReportRecordItemText( r.str_koujo ,ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII, FALSE, TRUE, 80) );
		rec->AddItem( new ICSReportRecordItemText( r.str_saisyu ,ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII, FALSE, TRUE, 80) );
		return rec;
	}
	
	virtual void SetReportColumn(int width)
	{
		BOOL bResize = TRUE;
		ICSReportColumns* columns = GetColumns();
		columns->Add( new ICSReportColumn( kubun, "申告区分", width / 16 * 3, bResize, ICS_REPORT_NOICON , FALSE, TRUE, DT_LEFT ) );
		columns->Add( new ICSReportColumn( kikan, "課税期間(中間期間)", width / 16 * 6, bResize, ICS_REPORT_NOICON , FALSE, TRUE, DT_LEFT ) );
		columns->Add( new ICSReportColumn( koujo,"控除方式", width / 16 * 2, bResize, ICS_REPORT_NOICON , FALSE, TRUE, DT_LEFT ) );
		columns->Add( new ICSReportColumn( saisyu,"最終変更日", width / 16 * 3, bResize, ICS_REPORT_NOICON , FALSE, TRUE, DT_LEFT ) );
	}
};
