#pragma once
#include "icsdiagctrl.h"


// CShinChsk ダイアログ

class CShinChsk : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinChsk)

public:
	CShinChsk(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinChsk();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CHSK };

	// 初期情報のセット
	int InitInfo( CSnHeadData *pSnHeadData, CH26SnChskData *pSnChskData, CH26SnHonpyoData *pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// 初期表示
	int InitDisp();

	// ダイアグラムデータセット
	void SetDiagData();
	// 計算セット
//	void Keisan_Set( char flg );
	void SetCalq( char flg );
	// 消費税計算
	void CalqSyz( char flg );
	// 分割納付税額の計算
//	void NFshz_tnk();
	void CalqSyzDivide();
	// 修正申告
//	void Shu_ShzKeisan();
	void CalqSyzRevised();
	// 計算
//	void Disp_Cr();
	void Disp_Calq();
	// 月をセットする
	void Tuki_Set();
	// 計算
	void Keisan_Set( char flg );
	// 前課税期間の月数を計算('15.08.28)
	void CalqMonthSpan();

	// 
	// 年月日のチェック
	int _CheckYYMMDD( char *sday, char *eday, char sw );
	// 平成～西暦　変換
	int shin_datecnv( unsigned char year, unsigned char *date, int sign );
	// ポジションセット
	void SetItemPosition( int IndexNo );

	// 背景色の変更
	void disp_clr( char sw, short idx );
	// 税務署入力にフォーカスセット('15.02.03)
	void SetTaxOfficeInput();
	// 終了時のフォーカス移動 ('15.03.02)
	void MoveFocusInEnd();

private:
	//== 帳票データ本体 ==
	CSnHeadData			*m_pSnHeadData;				// 消費税ヘッダ情報
	CH26SnChskData		*m_pSnChskData;				// 中間予定申告データ
	CH26SnHonpyoData	*m_pSnHonpyoData;			// 本表データ

	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	int					m_Crsw;						// ???
	int					m_Index;

	int					m_curwnd;

//2016.06.22 INSERT START
	int					m_bunshi_sw;				// 月の分子の入力sw
//2016.06.22 INSERT END

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_ChskDiag;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
