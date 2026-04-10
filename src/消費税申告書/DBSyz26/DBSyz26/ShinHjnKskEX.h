#pragma once
#include "icsdiagctrl.h"
#include "icsmnedit8.h"


// CShinHjnKskEX ダイアログ

enum {
	HJNKSK_NUM_HOJIN1			= 56,	// 法人番号１
	HJNKSK_NUM_HOJIN2,					// 法人番号２
	HJNKSK_NUM_HOJIN3,					// 法人番号３
	HJNKSK_NUM_HOJIN4,					// 法人番号４
	HJNKSK_NUM_HOJINHYPHEN1,			// 法人番号ハイフン
	HJNKSK_NUM_HOJINHYPHEN2,			// 法人番号ハイフン
	HJNKSK_NUM_HOJINHYPHEN3,			// 法人番号ハイフン
};

class CShinHjnKskEX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinHjnKskEX)

public:
	CShinHjnKskEX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinHjnKskEX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_HJINKSK_EX };

	// 初期情報のセット
	int InitInfo( CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH26SnKskData **pSnKskData );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// 画面スクロール処理
	void ChkScroll( WORD id, short index );
	// 初期化（データクリア）
	void ClearKskHjn();

	// 画面出力
	void DispKsiHjn( int mode=0 );
	// データ表示
	void DispKsiHjnDig1();
	// データ表示
	void DispKsiHjnDig2();
	// データ表示
	void DispKsiHjnDig3();
	// データ表示
	void DispKsiHjnDigZi();
	// ポジションセット
	void SetItemPosition( int IndexNo );
	// 還付金融機関入力にフォーカスセット('16.12.26)
	void SetKanpuKinyuInput();
	// 終了時のフォーカス移動 ('16.12.26)
	void MoveFocusInEnd();
	// 元号表示
	void DispGengo();

	// データセット
	int	DataSetDig1( long index, short leng, DIAGRAM_DATA *data, int mode );
	// データセット
	int	DataSetDig2( long index, short leng, DIAGRAM_DATA *data, int mode );
	// データセット
	int	DataSetDig3( long index, short leng, DIAGRAM_DATA *data, int mode );

	// カーソル移動
	int MoveCurselDiag1( short index, UINT flg, short nChar, int mode=0 );
	// カーソル移動
	int MoveCurselDiag2( short index, UINT flg, short nChar, int mode=0 );
	// カーソル移動
	int MoveCurselDiag3( short index, UINT flg, short nChar, int mode=0 );

	// 税務署コードの取得
	int GetTaxationOffice();

//2015.12.08 INSERT START
	// マイナンバーエディットコントロール位置調整
	void MoveDiagMNEdit();
	// マイナンバーコントロールの表示を消す
	void HideMNEdit();
//2015.12.08 INSERT END

private:
	//== 帳票データ本体 ==
	CSnHeadData			**m_pSnHeadData;				// 消費税ヘッダ情報
	CH26SnKskData		**m_pSnKskData;				// 更正の請求データ
	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	int					m_Clm;						// 
	int					m_Idx;						// 

	int					m_curwnd;					//[Y]
	CWnd				*m_pParent;					// 親ウィンドウ('15.12.07)


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_dig5;
	CIcsdiagctrl m_dig1;
	CIcsdiagctrl m_dig2;
	CIcsdiagctrl m_dig3;
	CIcsdiagctrl m_dig4;
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiagctrl10(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl10(short index);
	void EditOFFIcsdiagctrl10(short index);
	void ComboSelIcsdiagctrl10(short index, LPUNKNOWN data);
	void TerminationIcsdiagctrl20(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl20(short index);
	void EditOFFIcsdiagctrl20(short index);
	void TerminationIcsdiagctrl30(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiagctrl30(short index);
	void EditOFFIcsdiagctrl30(short index);
	void ComboSelIcsdiagctrl30(short index, LPUNKNOWN data);
	void ComboSelIcsdiagctrl50(short index, LPUNKNOWN data);
	void TerminationIcsdiagctrl50(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnButtonF2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CICSMNEditController8	m_ctrl_mynum;
	CICSMNEdit8				m_edit_mynum1;
	CICSMNEdit8				m_edit_mynum2;
	CICSMNEdit8				m_edit_mynum3;
	CICSMNEdit8				m_edit_mynum4;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
