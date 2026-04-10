#pragma once
#include "icsdiagctrl.h"
#include "icsmnedit8.h"


// CShinKjnKskEX ダイアログ

enum {
	KJNKSK_NUM_KOJIN1			= 52,	// 個人番号１
	KJNKSK_NUM_KOJIN2,					// 個人番号２
	KJNKSK_NUM_KOJIN3,					// 個人番号３
	KJNKSK_NUM_KOJINHYPHEN1,			// 個人番号ハイフン
	KJNKSK_NUM_KOJINHYPHEN2,			// 個人番号ハイフン
};


class CShinKjnKskEX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinKjnKskEX)

public:
	CShinKjnKskEX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinKjnKskEX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KJINKSK_EX };

	// 初期情報のセット
	int InitInfo( CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH26SnKskData **pSnKskData );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();
	// 初期化（データクリア）
	void ClearKskKjn();

	// 税務署コードの取得
	int GetTaxationOffice();
	// 画面スクロール処理
	void ChkScroll( WORD id, short index );
	// ポジションセット
	void SetItemPosition( int IndexNo );
	// 還付金融機関入力にフォーカスセット('16.12.26)
	void SetKanpuKinyuInput();
	// 終了時のフォーカス移動 ('16.12.26)
	void MoveFocusInEnd();

	// 画面出力
	void DispKsiKjn( int mode=0 );
	// データ表示
	void DispKsiKjnDig1();
	// データ表示
	void DispKsiKjnDig2();
	// データ表示
	void DispKsiKjnDig3();
	// データ表示
	void DispKsiKjnDigZi();
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

//2015.12.08 INSERT START
	// マイナンバーエディットコントロール位置調整
	void MoveDiagMNEdit();
	// マイナンバーコントロールの表示を消す
	void HideMNEdit();
//2015.12.08 INSERT END

private:
	//== 帳票データ本体 ==
	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報
	CH26SnKskData		**m_pSnKskData;				// 更正の請求データ
	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	int					m_Clm;						// 
	int					m_Idx;						// 

	int					m_curwnd;
	CWnd				*m_pParent;					// 親ウィンドウ('15.12.07)


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_dig4;
	CIcsdiagctrl m_dig1;
	CIcsdiagctrl m_dig5;
	CIcsdiagctrl m_dig2;
	CIcsdiagctrl m_dig3;
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
	void ComboSelIcsdiagctrl40(short index, LPUNKNOWN data);
	void TerminationIcsdiagctrl40(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButtonF2();
	void EditONIcsdiagctrl40(short index);
	CICSMNEditController8	m_ctrl_mynum;
	CICSMNEdit8				m_edit_mynum1;
	CICSMNEdit8				m_edit_mynum2;
	CICSMNEdit8				m_edit_mynum3;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
