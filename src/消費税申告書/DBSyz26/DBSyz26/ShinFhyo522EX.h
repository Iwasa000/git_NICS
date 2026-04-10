#pragma once
#include "icsdiagctrl.h"


// CShinFhyo522EX ダイアログ

class CShinFhyo52EX;
class CShinFhyo522EX : public CSyzBaseDlg
{
	DECLARE_DYNAMIC(CShinFhyo522EX)

public:
	CShinFhyo522EX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinFhyo522EX();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FHY522 };

	// 初期情報のセット
	int InitInfo( CH27SnFhyo50Data **pSnFhyo52Data, CH27SnFhyo10Data **pSnFhyo40Data, CH27SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CShinFhyo52EX *pShinFhyo52, CDBNpSub *pZmSub );
//	int InitInfo( CH26SnFhyo50Data **pSnFhyo52Data, CH26SnFhyo10Data **pSnFhyo40Data, CH26SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CShinFhyo52EX *pShinFhyo52, CDBNpSub *pZmSub );
	// 初期化
	int Init();
	// ダイアグラム初期属性セット
	int InitDiagAttr();

	// データの読込み
	int ReadData( int upck );
	// データの書込み
	int WriteData();

	// 画面出力
	void DispFh522();
	// 画面出力（非連動）
	void DispFh522_Unconnect();

	// 控除対象仕入税額の計算式区分の表示
	void f52dsp_tk();
	// 金額表示
	void dsp_prs( unsigned short Id, short Index, char *Mony );
	// 金額表示クリア
	void dsp_cls( unsigned short Id, short Index );
	// 入力項目　色変え
	void ChangeColor( unsigned short id, short index, int sign );
	// ダイアグラムのインデックスから関連項目を取得する
	int GetRelInfoByDiagIdx( short Idx, char **pRtMony );
	// 複数事業で強制入力不可行？
	int IsProvideEditCell( short Idx );
	// 関連項目の手入力サインをONにする
	void TrunOnInputSign( short index );

	// 横計(3-イ)
	void f52calq_sum1( short index );
	// 横計(3-ロ-イ)
	void f52calq_sum2( short index );
	// 横計(3-ロ-ロ)
	void f52calq_sum3( short index );
	// 最も有利な方式を選択
	void f52calq_sel( short index=-1 );
	// 簡易課税　2種類以上の業種での特例計算チェック
	int f52_toku75( char *Base, char *Add1, char *Add2 );

	//--------->フォーカス移動処理（上下）
	int UpDownFocusMoveCheck(int Index , int Move);// Move = 0 : 下方向		Move = 1: 上方向
	//<----------------
	// 入力可能ポジションの取得 ('14.06.28)
	int GetAbleInputPosition( int index );

	// 初期ポジションセット
	void SetItemPosition();


private:

	char				*pF5220T,		// 20. 原則計算適用･合計 仕入税額（期間計）
						*pF5220A,		// 20. 　　　　　　　 〃　　　　  （3%分）
						*pF5220B,		// 20. 　　　　　　　 〃　　　　  （4%分）
						*pF5220C;		// 20. 　　　　　　　 〃　　　　  （6.3%分）
	int					*pF5220c;		// 20. 原則計算 再計算スイッチ
	
	char				*pF5221T,		// 21. 特例適用･合計 仕入税額(期間計)	
						*pF5221A,		// 21. 　　　　　　　　〃　　(3%分)	　
						*pF5221B,		// 21. 　　　　　　　　〃　　(4%分)	　
						*pF5221C;		// 21. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5221c;		// 21. 原則計算 再計算スイッチ

	char				*pF5222T,		// 22. 原則計算適用･合計 仕入税額（期間計）
						*pF5222A,		// 22. 　　　　　　　 〃　　　　  （3%分）
						*pF5222B,		// 22. 　　　　　　　 〃　　　　  （4%分）
						*pF5222C;		// 22. 　　　　　　　 〃　　　　  （6.3%分）
	int					*pF5222c;		// 22. 原則計算 再計算スイッチ
	
	char				*pF5223T,		// 23. 特例適用･合計 仕入税額(期間計)	
						*pF5223A,		// 23. 　　　　　　　　〃　　(3%分)	　
						*pF5223B,		// 23. 　　　　　　　　〃　　(4%分)	　
						*pF5223C;		// 23. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5223c;		// 23. 原則計算 再計算スイッチ

	char				*pF5224T,		// 24. 原則計算適用･合計 仕入税額（期間計）
						*pF5224A,		// 24. 　　　　　　　 〃　　　　  （3%分）
						*pF5224B,		// 24. 　　　　　　　 〃　　　　  （4%分）
						*pF5224C;		// 24. 　　　　　　　 〃　　　　  （6.3%分）
	int					*pF5224c;		// 24. 原則計算 再計算スイッチ
	
	char				*pF5225T,		// 25. 特例適用･合計 仕入税額(期間計)	
						*pF5225A,		// 25. 　　　　　　　　〃　　(3%分)	　
						*pF5225B,		// 25. 　　　　　　　　〃　　(4%分)	　
						*pF5225C;		// 25. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5225c;		// 25. 原則計算 再計算スイッチ

	char				*pF5226T,		// 26. 原則計算適用･合計 仕入税額（期間計）
						*pF5226A,		// 26. 　　　　　　　 〃　　　　  （3%分）
						*pF5226B,		// 26. 　　　　　　　 〃　　　　  （4%分）
						*pF5226C;		// 26. 　　　　　　　 〃　　　　  （6.3%分）
	int					*pF5226c;		// 26. 原則計算 再計算スイッチ

	char				*pF5227T,		// 27. 特例適用･合計 仕入税額(期間計)	
						*pF5227A,		// 27. 　　　　　　　　〃　　(3%分)	　
						*pF5227B,		// 27. 　　　　　　　　〃　　(4%分)	　
						*pF5227C;		// 27. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5227c;		// 27. 原則計算 再計算スイッチ

	char				*pF5228T,		// 28. 原則計算適用･合計 仕入税額（期間計）
						*pF5228A,		// 28. 　　　　　　　 〃　　　　  （3%分）
						*pF5228B,		// 28. 　　　　　　　 〃　　　　  （4%分）
						*pF5228C;		// 28. 　　　　　　　 〃　　　　  （6.3%分）
	int					*pF5228c;		// 28. 原則計算 再計算スイッチ
	
	char				*pF5229T,		// 29. 特例適用･合計 仕入税額(期間計)	
						*pF5229A,		// 29. 　　　　　　　　〃　　(3%分)	　
						*pF5229B,		// 29. 　　　　　　　　〃　　(4%分)	　
						*pF5229C;		// 29. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5229c;		// 29. 原則計算 再計算スイッチ

	char				*pF5230T,		// 30. 原則計算適用･合計 仕入税額（期間計）
						*pF5230A,		// 30. 　　　　　　　 〃　　　　  （3%分）
						*pF5230B,		// 30. 　　　　　　　 〃　　　　  （4%分）
						*pF5230C;		// 30. 　　　　　　　 〃　　　　  （6.3%分）
	int					*pF5230c;		// 30. 原則計算 再計算スイッチ8

	char				*pF5231T,		// 31. 特例適用･合計 仕入税額(期間計)	
						*pF5231A,		// 31. 　　　　　　　　〃　　(3%分)	　
						*pF5231B,		// 31. 　　　　　　　　〃　　(4%分)	　
						*pF5231C;		// 31. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5231c;		// 31. 原則計算 再計算スイッチ8

	char				*pF5232T,		// 32. 特例適用･合計 仕入税額(期間計)	
						*pF5232A,		// 32. 　　　　　　　　〃　　(3%分)	　
						*pF5232B,		// 32. 　　　　　　　　〃　　(4%分)	　
						*pF5232C;		// 32. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5232c;		// 32. 原則計算 再計算スイッチ8

	char				*pF5233T,		// 33. 特例適用･合計 仕入税額(期間計)	
						*pF5233A,		// 33. 　　　　　　　　〃　　(3%分)	　
						*pF5233B,		// 33. 　　　　　　　　〃　　(4%分)	　
						*pF5233C;		// 33. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5233c;		// 33. 原則計算 再計算スイッチ8

	char				*pF5234T,		// 34. 特例適用･合計 仕入税額(期間計)	
						*pF5234A,		// 34. 　　　　　　　　〃　　(3%分)	　
						*pF5234B,		// 34. 　　　　　　　　〃　　(4%分)	　
						*pF5234C;		// 34. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5234c;		// 34. 原則計算 再計算スイッチ8

	char				*pF5235T,		// 35. 特例適用･合計 仕入税額(期間計)	
						*pF5235A,		// 35. 　　　　　　　　〃　　(3%分)	　
						*pF5235B,		// 35. 　　　　　　　　〃　　(4%分)	　
						*pF5235C;		// 35. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5235c;		// 35. 原則計算 再計算スイッチ8

	char				*pF5236T,		// 36. 特例適用･合計 仕入税額(期間計)	
						*pF5236A,		// 36. 　　　　　　　　〃　　(3%分)	　
						*pF5236B,		// 36. 　　　　　　　　〃　　(4%分)	　
						*pF5236C;		// 36. 　　　　　　　　〃　　(6.3%分)	　
	int					*pF5236c;		// 36. 原則計算 再計算スイッチ8

	char				*pF5237T,		// 37. 選択した 仕入税額(期間計)	
						*pF5237A,		// 37. 　　　　　　　　〃　　(3%分)	　
						*pF5237B,		// 37. 　　　　　　　　〃　　(4%分)	　
						*pF5237C;		// 37. 　　　　　　　　〃　　(6.3%分)	　
private:

	CWnd				*m_pParent;					// 親ウィンドウ

	CShinFhyo52EX			*m_pShinFhyo52;				// 付表５－２（１）ウィンドウ

	//== 帳票データ本体 ==
	CH27SnFhyo50Data	**m_pSnFhyo52Data;			// 付表５－２
	CH27SnFhyo10Data	**m_pSnFhyo40Data;			// 付表４
	CH27SnHonpyoData	**m_pSnHonpyoData;			// 本表
	CSnHeadData			**m_pSnHeadData;			// 消費税ヘッダ情報

	CDBNpSub			*m_pZmSub;					// 財務クラス
	CArithEx			*m_pArith;
	SYC_SHININFO		*m_pShinInfo;				// 共通情報
	CSyzUtil			m_Util;

	//== 付表５－２との共有情報 ==
	int					*m_ptypeCnt;				// 業種数
	char				*m_pCQsw;					// 仕入税額集計サイン（このサインの意味付けは？）
	char				*m_pSzsw;					//
	char				*m_pSBsw;					//
	int					*m_ptkSw;					// 特例計算摘要スイッチ
	int					*m_pdspSkjSw;				// 仕入控除税額計算の、どの行が表示されているか（付表５－２－１と連動）

	int					m_FrSw;						// ???
	int					m_DpSw;						// 再表示スイッチ
	BOOL				m_IsDeleteKeyIn;			// DELETE キー入力

	int					m_curwnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_Fhyo522Diag01;
	CIcsdiagctrl m_Fhyo522Diag02;
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiagctrl1(short index);
	void EditOFFIcsdiagctrl1(short index);
	void TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
