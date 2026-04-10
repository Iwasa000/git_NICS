#ifndef  __ICS_SYZBASEHYOVIEW__
#define  __ICS_SYZBASEHYOVIEW__

#pragma once

#include <H26HyoViewRec.h>

class SYZSHINCOM_CLASS_EXPORT CBaseHyoView : public ICSFormView
{
protected:
	CBaseHyoView( UINT id );
	~CBaseHyoView(void);

public:
	// 履歴シーケンスセット
	virtual void SetSnSeq( int snseq ){};
	// 表示帳票の最終データ取得
	virtual void GetDispHyoData(){};
	// 終了処理
	virtual int EndJob( int wflg = 1 ){ return 0; };
	// データ書込み
	virtual int WriteData( BOOL isEnd, BOOL isGetData ){ return 0; };
	// 部分データ書込み
	virtual int WriteSepData( int flg ){ return 0; };
	// 部分データ読込み
	virtual int ReadSepData( int flg ){ return 0; };
	// 全画面：画面表示 再初期化
	virtual int ReInitDisp( BOOL isCompChange, BOOL SixKindFlg=FALSE, EnumIdIcsShRevType shrevtype=ID_ICSSH_NO_REVTYPE ){ return 0; };
	//本表・26号様式　上部のみ再表示
	virtual void ReDispHonpyoUpper(){};

	// 現在の入力位置にフォーカスセット
	virtual void SetNowPosition(){};
	// 過年度プロ呼出し後にデータを吸い上げる
	virtual int GetPastData(){ return 0; };
	// 画面表示フラグを切り換える
	virtual void SetDispedFlg( BOOL bShow ){};
	// 履歴シーケンス切替
	virtual int ChangeSnSeq( int snseq ){ return 0; };
	// 現申告データを消去して、データの再読込み
	virtual int ReReadData(){ return 0; };
	// 手入力サインのクリア
	virtual int ClearManualInputSign( BOOL isChgKzritu ){ return 0; };
	// 次回申告データ作成に必要になるデータの読込み
	virtual int ReadDataForNextShinkoku(){ return 0; };

	// 金額取得
	virtual int GetMony( CString &tag, CString &chtag, char *pArith ){ return 0; };
	// 金額セット
	virtual int SetMony( CString &tag, CString &chtag, char *pArith ){ return 0; };
	// 金額の再取得
	virtual int ReGetCalqedMony(){ return 0; };
	// 仕入控除税額の財務連動
	virtual int ConnectedSkj( MoneyBasejagArray &MoneyBaseArray, bool isUpdDisp ){ return 0; };
	// 仕入金額等の明細のクリア
	virtual int ClearShiire(){ return 0; };
	// 更正の請求用　申告書金額計算
	virtual int CalqSyzForKsk( int mode ){ return 0; };
	// 中間及び期間短縮の更新
	virtual int RenewSyzShin( SH_PASTINFO *pPastInfo ){ return 0; };
	// 本表の計算(1)～(26)迄
	virtual void AlreadyDecisionCalc( int type ){};
	// 棚卸調整額の連動
	virtual int CalqStatckSyz(){ return 0; };
	// 前年の消費税額取得
	virtual int GetLastYearTax( CDBNpSub *pBfNpSub, char *pArith ){ return 0; };
	// 集計済み金額の取得
	virtual int GetCalqedMony(){ return 0; };
	// 既確定税額のクリア('15.01.28)
	virtual int ClearAlreadyDecision(){ return 0; };

	// 各表の更新処理
	virtual void UpdateSheetFromOuter(){};
	// 全表の更新処理
	virtual void UpdateAllSheet( int sgn = 0 ){};
	//[Y] 選択表より前までの表の更新処理
	virtual void UpdateUntilSheet(){};

	// 予定26号の前課税期間のセット
//	virtual void ZenkiDate_Chsk(char* ZenkSymd , char* ZenkEymd ){};
	virtual void ZenkiDate_Chsk(char* ZenkSymd , char* ZenkEymd, int ZenkSg, int ZenkEg ){};
	// 表示中頁の更新処理後の再表示
	virtual void IndicateSheet(){};
	// タブへフォーカスセット
	virtual void EndJobFocus(){};

	// 入力ロック呼び出し
	virtual void InputRock(int mode){};




	// 終了時のフォーカス移動
	virtual void MoveFocusInEnd(){};


	
	

	

	// 税務署入力にフォーカス誘導
	virtual int GuideTaxOfficeInput(){ return FALSE; };
	// 簡易課税第六種フラグをセット
	virtual void SetSixKindFlg( BOOL SixKindFlg ){};
	// 還付申告？
	virtual BOOL IsKanpuShinkoku(){ return FALSE; };
	virtual BOOL IsKanpuShinkoku2(){ return FALSE; };

	// 入力位置を保存
	virtual void SaveInputPosition(){};
	// 入力位置を復元
	virtual void RestoreInputPosition(){};



	// 特定課税仕入と課税売上割合のチェック
	virtual int ChkTkkzAndKzwari( int srctab, int calltype ){ return 0; };

	

	

	

	
	
	
	// 還付金融機関入力にフォーカス誘導('16.12.26)
	virtual int GuideKanpuKinyu( int dsInput ){ return 0; };

	// 改正様式をセットする
	virtual void SetShRevType( EnumIdIcsShRevType shrevtype ){};

	virtual int Message_BlankKinyu_Kousei(){ return 0; };
	//納付還付税額の合計と合計額が一致するか
	virtual int Message_NofuKanpuGokei(){ return 0; };

	virtual int Message_MyNumber_error(){ return 0; };
	virtual int Message_MyNumber_overlap(){ return 0; };
	virtual int Message_SozokuWariai(){ return 0; };

	virtual void EditOff_Fuhyo6(){};

	virtual int IsExist_MyNumber_Fuhyo6(){ return 0; };

	// 予定申告の月数クリア
	virtual int ClearYoteiMonth(){ return 0; };

	// 終了処理フラグの切り替え
	virtual void SetEndStartFlg( BOOL flg ){};

	// 初期化完了フラグの切り替え
	virtual void SetInitedFlg( bool flg ){};
	// View の初期化完了？
	virtual bool IsInitedView(){ return false; };
	// マイナンバー閲覧サインのセット
	virtual void SetMyNumBrowseSgn( BOOL NoHistory_browse ){};

	// 仕入控除税額のみの財務連動
	virtual int ConnectedSkjZeigaku(bool isUpdDisp){ return 0; };

	// 還付付表2　インボイス登録番号のチェック
	virtual bool CheckInvno( int sgn ){ return true; };
};

#endif
