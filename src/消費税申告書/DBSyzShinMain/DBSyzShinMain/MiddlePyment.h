#pragma once
#include "icsdiagctrl.h"


// CMiddlePyment ダイアログ

class CMiddlePyment : public ICSDialog
{
	DECLARE_DYNAMIC(CMiddlePyment)

	CArith arith;
	CMap<int,int,RECT,RECT> PositionMap;
	
	vector<MPAYMENTVAL> MPrec;

	int IsEditable(int index);
	int Termination(int index,short nChar);
	int SerchNextPos(int& index,RECT& move,LONG RECT::*ptr);
	void SetPosition(CRect move,LONG RECT::*ptr);

	void recalq();

	BOOL CMiddlePyment::CheckOldTyukan();

public:
	CMiddlePyment(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMiddlePyment();

	int IsRireki;

	unsigned char	sn_sign4;			// 特殊処理サイン(m_pSnHeadData->Sn_Sign4)
	int				m_ActViewVer;		// 起動中のビューのバージョン（CMainFrame.m_ActViewVer）


	BOOL Init(int oldread);

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TYUKANNOZE };
//	static	CDBSyzShinView	*pSyzShin;
	static	CDBSyzShinMainView	*pSyzShin;

	BOOL ReadNoufu();
	BOOL UpdateNoufu();

public:
	unsigned char	Sn_ZZURI[MONY_BUF_SIZE];	// 基準期間の課税売上高
	unsigned char	Sn_ZZNOFZ[MONY_BUF_SIZE];	// 前々回納付税額
	unsigned char	Sn_ZURI[MONY_BUF_SIZE];		// 前年の課税売上高
	unsigned char	Sn_ZNOFZ[MONY_BUF_SIZE];	// 前回納付税額
	unsigned char	Sn_KazeiUriage[MONY_BUF_SIZE];	// 課税期間における課税売上高

private:
	bool m_isSaveKzUri;	// 課税期間における課税売上高の保存の必要あり？

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CIcsdiagctrl m_MPeyment;
	
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnOldtyukan();
};
