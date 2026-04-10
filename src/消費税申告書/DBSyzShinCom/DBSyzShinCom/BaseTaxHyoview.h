#ifndef  __ICS_SYZBASETAXHYOVIEW__
#define  __ICS_SYZBASETAXHYOVIEW__

#pragma once

#include <H31TaxHyoViewRec.h>

class SYZSHINCOM_CLASS_EXPORT CBaseTaxHyoView : public ICSFormView
{
protected:
	CBaseTaxHyoView( UINT id );
	~CBaseTaxHyoView(void);

public:
	// 履歴シーケンスセット
	virtual void SetSnSeq( int snseq ){};
	// 表示帳票の最終データ取得
	virtual void GetDispHyoData(){};
	// 終了処理
	virtual int EndJob(){ return 0; };
	// 終了処理
	virtual int EndJob( int wflg = 1 ){ return 0; };	// 20191112追加
	// データ書込み
	virtual int WriteData( BOOL isEnd, BOOL isGetData ){ return 0; };
	// データ書込み　2019/10/29追加
	virtual int WriteData( BOOL isEnd ){ return 0; };
	// 部分データ書込み
	virtual int WriteSepData( int flg ){ return 0; };
	// 部分データ読込み
	virtual int ReadSepData( int flg ){ return 0; };
	// 全画面：画面表示 再初期化
	virtual int ReInitDisp( BOOL isCompChange, BOOL SixKindFlg , BOOL isResetFocus=TRUE ){ return 0; };

	// 現在の入力位置にフォーカスセット
	virtual void SetNowPosition( BOOL IsInEnd = FALSE ){};
	// 過年度プロ呼出し後にデータを吸い上げる
	virtual int GetPastData(){ return 0; };
	// 画面表示フラグを切り換える
	virtual void SetDispedFlg( BOOL bShow ){};
	// 履歴シーケンス切替
	virtual int ChangeSnSeq( int snseq ){ return 0; };
	// 現申告データを消去して、データの再読込み
	virtual int ReReadData(){ return 0; };
	// 手入力サインのクリア
	virtual int ClearManualInputSign(){ return 0; };
	// 次回申告データ作成に必要になるデータの読込み
	virtual int ReadDataForNextShinkoku(){ return 0; };

	// 金額取得
	virtual int GetMony( CString &tag, CString &chtag, char *pArith ){ return 0; };
	// 金額セット
	virtual int SetMony( CString &tag, CString &chtag, char *pArith ){ return 0; };
	// 金額の再取得
	virtual int ReGetCalqedMony(){ return 0; };

	// 各年度の計算
	virtual void AlreadyDecisionCalc( int type ){};

	// 集計済み金額の取得
	virtual int GetCalqedMony(){ return 0; };

	// 終了時のフォーカス移動
	virtual void MoveFocusInEnd(){};

	// 各表の更新処理
	virtual void UpdateSheetFromOuter(){};
	// 全表の更新処理
	virtual void UpdateAllSheet( int sgn = 0 ){};

	//[Y] 選択表より前までの表の更新処理
	virtual void UpdateUntilSheet(){};

	// 表示中頁の更新処理後の再表示
	virtual void IndicateSheet(){};
	// タブへフォーカスセット
	virtual void EndJobFocus(){};

	// 入力ロック呼び出し
	virtual void InputRock(int mode){};

	// 簡易課税第六種フラグをセット
	virtual void SetSixKindFlg( BOOL SixKindFlg ){};
	// 還付申告？
	virtual BOOL IsKanpuShinkoku(){ return FALSE; };
	virtual BOOL IsKanpuShinkoku2(){ return FALSE; };
	// 入力位置を保存
	virtual void SaveInputPosition(){};
	// 入力位置を復元
	virtual void RestoreInputPosition(){};

	// 改正様式をセットする
	virtual void SetShRevType( EnumIdIcsShRevType shrevtype ){};

	// 終了処理フラグの切り替え
	virtual void SetEndStartFlg( BOOL flg ){};
	// 経理方式の切替
	virtual int ChgTaxationSystem( int *pKHstax ){return 0;};
	virtual int ReGetCalqedMony( BOOL isResetFocus ){return 0;};

	// 特定収入計算表との連動（'15.02.13）
	virtual int ConnectedSpcList(){return 0;};
	virtual int ConnectedTaxList(){return 0;};

	// 消費税簡易計算表　ダイアログ表示
	virtual int SpecPassOffExDlg(){return 0;};
	// 特定収入の再計算('15.04.03)
	virtual int RecalqSpc( SYZTOSPC_DATA *pSyzToSpc ){return 0;};
	// 入力ロック切替時の再表示('16.07.22)
	virtual void ChgLockDisp(){};
	// 個人決算書名称変更の取得切替
	virtual int ChgKoketsuMeisyoGet( int *pKoketsu ){return 0;};

	//課税売上高の「損益計算書の収入金額」が入力されているか
	virtual BOOL IsEmpty_SonekiSyunyu(){return FALSE;};
	//課税売上高の「損益計算書の仕入金額と経費の金額の合計額」が入力されているか
	virtual BOOL IsEmpty_SonekiShiireKeihi(){return FALSE;};
};

#endif

