#pragma once

#include "icsdiagctrl.h"

// CShinIppan02 ダイアログ

class CShinIppan02 : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinIppan02)

public:
	CShinIppan02(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinIppan02();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_IPPAN02 };

private:
	CWnd				*m_pParent;						// 親ウィンドウ

	char				XM1[MONY_BUF_SIZE],				// 課税標準額
						XM2[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額 3%適用分
						XM3[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額 4%適用分
						XM4[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額 6.3%適用分
						XM5[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額 6.24%適用分
						XM6[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額 7.8%適用分
						XM7[MONY_BUF_SIZE],				// 課税資産の譲渡等の対価の額
						XM8[MONY_BUF_SIZE],				// 特定課税仕入れに係る支払い対価の額 6.3%適用分
						XM9[MONY_BUF_SIZE],				// 特定課税仕入れに係る支払い対価の額 7.8%適用分
						XM10[MONY_BUF_SIZE],			// 特定課税仕入れに係る支払い対価の額
						XM11[MONY_BUF_SIZE],			// 消費税額
						XM12[MONY_BUF_SIZE],			// 11の内訳 3%適用分
						XM13[MONY_BUF_SIZE],			// 11の内訳 4%適用分
						XM14[MONY_BUF_SIZE],			// 11の内訳 6.3%適用分
						XM15[MONY_BUF_SIZE],			// 11の内訳 6.24%適用分
						XM16[MONY_BUF_SIZE],			// 11の内訳 7.8%適用分
						XM17[MONY_BUF_SIZE],			// 返還等対価に係る税額
						XM18[MONY_BUF_SIZE],			// 売上げの返還等対価に係る税額
						XM19[MONY_BUF_SIZE],			// 特定課税仕入れの返還等対価に係る税額
						XM20[MONY_BUF_SIZE],			// 地方消費税の課税標準となる消費税額
						XM21[MONY_BUF_SIZE],			// 地方消費税の課税標準となる消費税額 4%適用分
						XM22[MONY_BUF_SIZE],			// 地方消費税の課税標準となる消費税額 6.3%適用分
						XM23[MONY_BUF_SIZE];			// 地方消費税の課税標準となる消費税額 6.24%及び7.8%適用分

	int					FUSOKU381,						// 附則３８① 軽減売上割合
						FUSOKU382,						// 附則３８② 小売等軽減仕入割合
						FUSOKU383;						// 附則３８③ 小売等軽減売上割合

	int					m_curwnd;

// 220412 add -->
	BOOL				m_Tok53Mod;						// TRUE : 特例計算（5-3）チェック欄を表示しない
// 220412 add <--

private:
	CSnHeadData			**m_pSnHeadData;				// 消費税ヘッダ情報
	CH31SnHonpyo2Data	**m_pSnHonpyo2Data;
	CSyzUtil			m_Util;
	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;					// 共通情報
	CDBNpSub			*m_pZmsub;
	CSyzSyukei			**m_pSyzSyukei;

public:
	// 初期情報のセット
	int InitInfo ( CSnHeadData **pSnHeadData, CH31SnHonpyo2Data **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo , CDBNpSub *pZmsub , CSyzSyukei **pSyukei);

	// 初期化
	int Init();

	// ダイアログ初期属性セット
	int InitDiagAttr();

	// ダイアグラム属性変更
	int ChgDiagAttr();

	// データの読込み
	int ReadData();

	// データの書込み
	int WriteData();

	// 初期データ表示
	void DispIppan2();

	// 本表上部表示
	void DispIppan2Upper();

	// 金額表示
	void dsp_prs ( unsigned short Id, short Index, char *Mony );

	// 表示クリア
	void dsp_cls ( unsigned short Id, short Index );

	// 初期ポジションセット
	void SetItemPosition();

	// 各付表からデータを取得
	int GetFhyoData();

	// 各項目に金額をセット
	void SetDiagData();

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditONIcsdiag8ctrl2(short index);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_Ippan02Diag01;
	CIcsdiagctrl m_Ippan02Diag02;
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void EditOFFIcsdiag8ctrl1(short index);
};
