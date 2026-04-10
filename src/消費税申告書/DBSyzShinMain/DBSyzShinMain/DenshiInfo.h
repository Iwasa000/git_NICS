#pragma once
#include "icsdiag8ctrl1.h"


// CDenshiInfo ダイアログ
#define ID_YUBIN	5
#define ID_JUSYO1	6
#define ID_JUSYO2	7
#define	ID_DY_TEL1	8
#define	ID_DY_TEL2	9
#define	ID_DY_TEL3	10
#define	ID_KEIRI	11

class CDenshiInfo : public ICSDialog
{
	DECLARE_DYNAMIC(CDenshiInfo)

public:
	CDenshiInfo(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDenshiInfo();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_DENSHIINFO };

	// 初期表示
	void InitDisp();
	// データ読み込み
	void ReadData();
	// データ書込み
	void WriteData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CIcsdiag8ctrl1 m_denshi_diag1;
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);

public:
	void InitInfo( CSnHeadData *pSnHeadData );

private:
	CSnHeadData		*m_pSnHeadData;
	CSyzUtil		m_Util;
	unsigned char	m_Sn_DIADR[40*2];	// 住所
	unsigned char	m_Sn_DIZIP[10];		// 郵便番号
	unsigned char	m_Sn_DITELNO1[6];	// 電話番号(市外)
	unsigned char	m_Sn_DITELNO2[4];	// 電話番号(市内)
	unsigned char	m_Sn_DITELNO3[4];	// 電話番号(局番)
	unsigned char	m_Sn_KEIRI[40];		// 経理担当者
public:
	virtual BOOL OnInitDialog();
	void EditOFFIcsdiag8ctrl1(short index);
};
