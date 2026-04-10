/*========================================================================
		消費税入力画面 モジュール
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
#ifndef INCLUDE_SYZINP_H
#define	INCLUDE_SYZINP_H


#define WM_SKBN_INEND		(WM_APP+0x20)	// 消費税ダイアログ キー END
#define WM_SKBN_ICHIRN		(WM_APP+0x21)	// 消費税区分一覧 文字列 送信
#define WM_SKBN_LINEDISP	(WM_APP+0x22)	// 消費税データ表示

// 消費税入力ダイアログの表示位置サイン
#define SYP_TOPLEFT			0	// 指定矩形の上左側に表示
#define SYP_TOPRIGHT		1	// 　　〃　　上右側に表示
#define SYP_BOTTOMLEFT		2	// 　　〃　　下左側に表示
#define SYP_BOTTOMRIGHT		3	// 　　〃　　下右側に表示

// 消費税入力ダイアログ内のコントロール表示ポジション
#define SY_SYZSWKBN_PN		0
#define SY_ZRITU_PN			1
#define SY_SZKBN_PN			2
#define SY_URISIRE_PN		3
#define SY_TAISEL_PN		4
#define SY_TAIKA_PN			5
#define SY_TOKUS_PN			6	// 特定収入[06.03 /08]
#define SY_INV_PN			7	// インボイス対応 登録番号
#define SY_MENZEI_PN		8	// 免税事業者からの課税仕入れ
#define SY_MENWARI_PN		9	// 免税事業者からの課税仕入れ 控除割合

#define INPMODE_NORMMAL		1	// 通常入力
#define INPMODE_DENP		2	// 伝票入力


#ifdef _DBINPSUB
class AFX_EXT_CLASS CSyzInp
#else
class CSyzInp
#endif
{
public:
	CSyzInp();
	virtual ~CSyzInp();
	
	void SetDenpMode();	// 伝票入力で消費税ダイアログにインボイス項目を表示しない場合、SetPara()より前に呼び出してください
	void SetInvNoDispCount(int count); //インボイス登録番号の摘要名称表示数(デフォルトは32)

	void SetDialogResize(BOOL bResize);
	int	 SetPara( CDBZmSub* pZm, CDBSyohi* pSy, CWnd* parent );
	// DialogON を 呼び出す前に 該当仕訳で、CDBSyohiのSyIniz()を呼び出す必要あり
	int	DialogON( BOOL bOK, CRect rect, int posflg, int pn );
	int	DialogOFF();
	// 消費税入力項目停止位置チェック
	int CheckSyzPn( int vect );
	// セットされた時の消費税仕訳区分
	DWORD	GetSyzswkKbn();

	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );
	void	GetCrntWindowRect(CRect& rect);

	int		SetSkbnSelNo( int no );
	void	set_nextpos();
	void	set_nowfocus();
	//入力中の登録番号チェック
	BOOL	CheckInvno();
	int		RenewInvNo();
	void	CancelInvNoIME();

private:

	class CSyzInpSub* pSub;
};



//SetPara で指定した CWnd* parent に メッセージを送る
#define WM_SKBNLITE_KEYDOWN		(WM_APP+0x100)	// 消費税選択よりキーの操作
#define WM_SKBNLITE_DISPTXT		(WM_APP+0x101)	// 消費税 税区分・税率を変更したことによる表示要求


#ifdef _DBINPSUB
class AFX_EXT_CLASS CSyzInpLite
#else
class CSyzInpLite
#endif
{
public:
	CSyzInpLite();
	virtual ~CSyzInpLite();

	void SetDialogResize(BOOL bResize);
	//CDBSyohi はモジュール内で作成
	int	 SetPara(CDBZmSub* pZm, CWnd* parent);

	//ダイアログを作成しないで初期化(以下の関数以外は使用不可)
	// int	SetSyzData(CDBDATA* pdata);
	// int	UpdateSyzData(CDBDATA* pdata);
	// int	TekiyoSyzData(int tksyz);
	// CString GetSyzLineText();
	// void	GetNowSyzData(CDBDATA* pdata);
	// void	Close();
	//
	int  SetParaNoDlg(CDBZmSub* pZm);

	//日付や科目をセットする。
	int	 SetSyzData(CDBDATA* pdata);
	int	 UpdateSyzData(CDBDATA* pdata);
	//摘要の消費税サイン[tksgn]をセット
	int	 TekiyoSyzData(int tksgn);
	int	 TekiyoSyzData(struct _SY_TKREC_* psytk);

	//ダイアログはポップアップで表示する
	// 基本位置の rect には スクリーン座標をセット
	int	 DialogON(BOOL bOK, CRect rect, int posflg, int pn);
	int	 DialogOFF();

	BOOL	IsDisplay();
	void	GetCrntRect(CSize& size);

	int		SetSkbnSelNo(int no);
	void	set_nextpos();
	void	set_nowfocus();

	//現在処理中の消費税文字列
	CString GetSyzLineText();
	//現在処理中の消費税データ(dsign[0]-[5] に datarecと同じ 消費税サインがある)
	void	GetNowSyzData(CDBDATA* pdata);

	//終了処理
	void	Close();

	void SetDispMode( int mode );//D0:インボイスを表示する
								 //D1:税区分で 外税をカットしない	
	//財務クラス再読み込み(摘要新規追加時)
	void ReloadData();
	//入力中の登録番号チェック
	BOOL CheckInvno();
private:

	class CSyzInpLiteSub* pSub;
};



#endif // INCLUDE_SYZINP_H