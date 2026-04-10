#pragma once
#include "afxwin.h"
#include "icsreportcontrol.h"
#include "icsbutton.h"
#include "dbUcLstKamoku.h"
#include "dbUcLstAddress.h"

static const int IkkatuHeaderNum = 2;

enum enumIdColumnIkkatuAdvSetting{	// レポートコントロールのカラムID
	ias_name = 0,
	ias_val
};

typedef struct _IkkatuAdvSettingHeader
{
	CString name;
	CString val;
}IkkatuAdvSettingHeader;

typedef struct _IKKATU_ADV_DATA
{
	_IKKATU_ADV_DATA():seq(0), name(""), ikkatu_val("0"){}	// デフォルトコンストラクタ
	_IKKATU_ADV_DATA(long seq, CString name, CString val):seq(seq), name(name), ikkatu_val(val){}	// (long, CString, CString)のコンストラクタ
	_IKKATU_ADV_DATA(const _IKKATU_ADV_DATA& obj):seq(obj.seq), name(obj.name), ikkatu_val(obj.ikkatu_val){}	// コピーコンストラクタ

	BOOL operator==(const _IKKATU_ADV_DATA& obj) const
	{
		return ((this->ikkatu_val == obj.ikkatu_val) && (this->seq == obj.seq/*念の為*/) && (this->name == obj.name/*念の為*/));
	}

	long seq;
	CString name;
	CString ikkatu_val;
}IKKATU_ADV_DATA;

class IkkatuValHandler
{
public:
	IkkatuValHandler():FormSeq(ID_FORMNO_021),HeaderNum(IkkatuHeaderNum){}

	void Init(int argFormSeq, BOOL pInvNo)
	{
		FormSeq = argFormSeq;
		bInvNo = pInvNo;
		// 支払手形の内訳書のみ取引先に対して一括金額を行う
// 改良No.21-0086,21-0529 cor -->
		//if((FormSeq == ID_FORMNO_042) || (FormSeq == ID_FORMNO_081))	Header.name = "取引先名称";
// ------------------------------
		if((FormSeq == ID_FORMNO_042) || (bInvNo == FALSE && FormSeq == ID_FORMNO_081))	Header.name = "取引先名称";
		else																			Header.name = "科目名称";
// 改良No.21-0086,21-0529 cor <--
		Header.val = "一括金額";
	}

	int GetHeaderNum()
	{
		return HeaderNum;
	}

	CString GetHeader(int clm_index)
	{
		switch(clm_index)
		{
			case ias_name:
				return Header.name;
			case ias_val:
				return Header.val;
		}

		return _T("");
	}

	map<long, IKKATU_ADV_DATA>& GetMap()
	{
		return IkkatuMap;
	}

	const CString GetIkkatuVal(long order)
	{
		map<long, IKKATU_ADV_DATA>::iterator it = IkkatuMap.find(order);
		if(it == IkkatuMap.end())	return "";	// 見つからなかった場合はEmptyを返す

		return (*it).second.ikkatu_val;
	}

	template<typename TABLE_CLASS> int SetMap(TABLE_CLASS& tc, long key, CString defalut_val)
	{
		IkkatuMap.clear();

		try{
			if(tc.Init((int)key) == DB_ERR_OK){
				while(!tc.IsEOF()){
					// Orderをキーに一括金額をバリューで持つ
					long seq = 0;
					int order = 0;
					CString val;
					CString name;
					// 一括金額設定に必要な値を取得
					tc.GetIkkatuNeedVal(seq, order, val, name);
					// 一括金額構造体にセット
					if(!(defalut_val.IsEmpty()))	val = defalut_val;	// デフォルト金額が入っていたら、その値で作成
					IKKATU_ADV_DATA kid(seq, name, val);
					// インサート
					IkkatuMap[order] = kid;
					// 次のレコードへ
					tc.MoveNext();
				}
			}
		}
		catch(...)
		{
			if ( tc.IsOpen() ){
				tc.Fin();
				return -1;
			}
		}

		return 0;
	}

	template<typename TABLE_CLASS> int SetTable(TABLE_CLASS& tc, long key)
	{
		try{
			if(tc.Init(key) == DB_ERR_OK){
				while(!tc.IsEOF()){
					long seq = 0;
					int order = 0;
					// 一致の判断に必要な値を取得
					tc.GetIkkatuNeedVal(seq, order);
					// マップから項目を取得
					map<long,IKKATU_ADV_DATA>::iterator it = IkkatuMap.find(order);
					// シーケンスが一致するならテーブルの一括金額を更新
					tc.SetIkkatuVal((*it).second.seq, (*it).second.ikkatu_val);
					// 次のレコードへ
					tc.MoveNext();
				}
			}
		}
		catch(...){
			if ( tc.IsOpen() ){
				tc.Fin();
				return -1;
			}
		}

		return 0;
	}

private:
	int FormSeq;
	int HeaderNum;
	BOOL bInvNo;					// 改良No.21-0086,21-0529 add
	IkkatuAdvSettingHeader Header;
	map<long, IKKATU_ADV_DATA> IkkatuMap;
};

// CdlgIkkatuAdvSetting ダイアログ

class CdlgIkkatuAdvSetting : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgIkkatuAdvSetting)

public:
	CdlgIkkatuAdvSetting(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgIkkatuAdvSetting();

// No.200903 del -->
	//int ShowDialog(IkkatuValHandler& ivh, BOOL& isChange);
// No.200903 del <--
// No.200903 add -->
	int ShowDialog(IkkatuValHandler& ivh, BOOL& isChange, int pFormSeq);
// No.200903 add <--

private:
	void InitReportHeader();
	void AddRecords();
	void BindKey(int row, int key);

// ダイアログ データ
	enum { IDD = IDD_DIALOG_IKKATU_ADV_SETTING };

private:
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	ICSReportControl m_report;

	int m_nRet;
	BOOL m_isAdvSet;
	IkkatuValHandler m_IkkatuValHandler;
	CUcFunctionCommon	m_clsFunc;			//	共通関数クラス
	int m_RowMax;
// No.200903 add -->
	int	m_FormSeq;
// No.200903 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnNMReportEditkeydownCUSTOM1_IKK_ADV_REPORT(NMHDR * pNotifyStruct, LRESULT * result);
	
};
