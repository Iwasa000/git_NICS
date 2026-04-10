// DBSyzRec8.h : ヘッダー ファイル

#include <afxdb.h>
#include <vector>

// 部門枝番別消費税拡張
class CBMESZSYK {
public:
	CBMESZSYK( class CBmeszsyk *bmeszsyk = NULL );
	__declspec(dllexport) int Requery( LPCTSTR filter );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();
	__declspec(dllexport) int Update();
	__declspec(dllexport) int Delete();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int AllDelete();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int ReqSt();			// ret 1=Requery Called

public:
	int			st;
	int			SZtype;							// 集計タイプ
	int			SZmnth;							// 月オフセット（★未使用）
	long		SZbumn;							// 部門コード　マイナスを有効にする
	CString		SZkcod;							// 科目（内部）コード (８桁)	
	long		SZedbn;							// 枝番コード　マイナスを有効にする
	BYTE		SZgrp1;							// 消費税グループ区分コード	
	BYTE		SZgrp2;							// 消費税グループ区分明細コード
	BYTE		SZzkbn;							// 税率区分サイン		
	BYTE		SZitem;							// 売上・仕入区分サイン
	BYTE		SZsjsg;							// 固定資産の譲渡及び取得サイン
	BYTE		SZrvsg;							// リザーブ		
	BYTE		SZkatt;							// 貸借サイン
	BYTE		SZzchg;							// リザーブ
	BYTE		SZsgn1;							// リザーブ
	BYTE		SZsgn2;							// リザーブ
	CString		SZtaig;							// 対象外金額
	CString		SZhikz;							// 非課税金額
	CString		SZyuka;							// 非課税有価証券５％
	CString		SZkomi;							// 税込金額
	CString		SZkomz;							// 税込税額
	CString		SZnuki;							// 税抜金額
	CString		SZnukz;							// 税抜税額
	CString		SZbeki;							// 別記金額
	CString		SZikkm;							// 総額一括税抜　税込金額
	CString		SZikkz;							// 総額一括税抜　税込税額
	CString		SZadjs;							// 総額一括税抜　調整税額
	CString		SZiptk;							// 輸入仕入　国税
	CString		SZiptt;							// 輸入仕入　地方税
	CString		SZval1;							// リザーブ
	CString		SZval2;							// リザーブ

	CBmeszsyk*		m_pbmeszsyk;
	int				nodata;
	void			data();
	CString			ErrMsg;

protected:
	char	rsv[256];
};


// 部門枝番別特定収入
class CBMESPSYK {
public:
	CBMESPSYK( class CBmespsyk *bmespsyk = NULL );
	__declspec(dllexport) int Requery( LPCTSTR filter );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();
	__declspec(dllexport) int Update();
	__declspec(dllexport) int Delete();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int AllDelete();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int ReqSt();			// ret 1=Requery Called

public:
	int			st;
	int			SPtype;							// 集計タイプ
	int			SPmnth;							// 月オフセット（★未使用）
	long		SPbumn;							// 部門コード　マイナスを有効にする
	CString		SPkcod;							// 科目（内部）コード (８桁)
	long		SPedbn;							// 枝番コード　マイナスを有効にする
	BYTE		SPgrp1;							// 消費税グループ区分コード
	BYTE		SPgrp2;							// 消費税グループ区分明細コード
	BYTE		SPzkbn;							// 税率区分サイン
	BYTE		SPitem;							// 売上・仕入区分サイン
	BYTE		SPrvsg;							// リザーブ
	BYTE		SPkatt;							// 貸借サイン
	BYTE		SPsgn1;							// リザーブ	
	BYTE		SPsgn2;							// リザーブ
	CString		SPtkzn;							// 対象外金額
	CString		SPthzn;							// 非課税金額
	CString		SPtczn;							// 非課税有価証券５％
	CString		SPtszn;							// 税込金額
	CString		SPtfzn;							// 税込税額
	CString		SPval1;							// リザーブ
	CString		SPval2;							// リザーブ

	CBmespsyk*		m_pbmespsyk;
	int				nodata;
	void			data();
	CString			ErrMsg;

protected:
	char	rsv[256];
};

// 納付書（領収済通知書）
class CSHINPAYMENT {
public:
	CSHINPAYMENT( class CShinPayment *shinpayment = NULL );
	__declspec(dllexport) int Requery( LPCTSTR filter );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();
	__declspec(dllexport) int Update();
	__declspec(dllexport) int Delete();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int AllDelete();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int ReqSt();			// ret 1=Requery Called

public:
	int			st;
	int			NFItem;							// 種別サイン		1=納付書
	int			NFLine;							// 行番号			1～nn 初回は1固定
	int			NFResv;							// 予備 主キー
	int			NFSequ;							// SEQ番号			0～19　履歴保存月
	BYTE		NFKskb;							// 加算税区分		1:不納付2:無申告3:過少申告
	BYTE		NFKsdm;							// リザーブ
	CString		NFStnm;							// 信託の名称
	int			NFJflg;							// 実額フラグ
	CString		NFHval;							// 本税
	CString		NFJval;							// 重加算税
	CString		NFKval;							// 加算税
	CString		NFRval;							// 利子税
	CString		NFEval;							// 延滞税
	CString		NFGval;							// 合計
	CString		NFHjvl;							// 本税（実額）
	CString		NFVal1;							// 予備金額①
	CString		NFVal2;							// 予備金額②
	CByteArray	NFSign;							// 予備フラ[128B]
				   
	CShinPayment*	m_pshinpayment;
	int				nodata;
	void			data();
	CString			ErrMsg;

protected:
	char	rsv[256];
};

// 消費税の還付申告に関する明細書（基本情報）
class CREFUNDVOL {
public:
	CREFUNDVOL( class CRefundvol *refundvol = NULL );
	__declspec(dllexport) int Requery( LPCTSTR filter );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();
	__declspec(dllexport) int Update();
	__declspec(dllexport) int Delete();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int AllDelete();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int ReqSt();			// ret 1=Requery Called

public:
	int			st;
	int			KPItem;							// 種別サイン		1=納付書
	int			KPLine;							// 行番号			1～nn 初回は1固定
	int			KPResv;							// 予備 主キー
	int			KPSequ;							// SEQ番号			0～19　履歴保存月 [12'05.01]
	int			KPType;							// 種別サイン		0:法人 1:個人	
	BYTE		KPRsgn;							// 該当する事項		D0:輸出 D1:設備 D2:その他	
	BYTE		KPRsdm;							// リザーブ	
	CString		KPRrup;							// その他の理由（上段）全額漢字　２０文字	
	CString		KPRrlw;							// 〃　　　　　（下段）〃	
	CString		KPRrdm;							// 〃　　　　　（下段）〃	
	BYTE		KPKuck;							// 取引金額など(2-1)	D0:OFF印刷なし　ON:印刷あり	
	BYTE		KPKusg;							// 税抜・税込(2-1)		D0:OFF税抜　ON税込	
	CString		KPEbnk;							// 銀行名			全角漢字	１０文字			
	BYTE		KPEbsg;							// 銀行種別サイン	1:銀行 2:金庫 3:組合 4:農協 5:漁協	
	BYTE		KPEbdm;							// リザーブ	
	CString		KPEstn;							// 支店名			全角漢字	１０文字	
	BYTE		KPEssg;							// 支店種別サイン	1:本店 2:支店 3:本所 4:支所 5:出張所	
	BYTE		KPEsdm;							// リザーブ	
	CString		KPEykn;							// 預金種別			全角間に	４文字	
	CString		KPEkoz;							// 口座番号			半角数字	１０文字	
	CString		KPEyn1;							// 郵貯番号①	
	CString		KPEyn2;							// 郵便番号②	
	CString		KPEsup;							// 主な通関業者名（上段）全角漢字	２０文字	
	CString		KPEslw;							// 〃　　　　　　（下段）〃	
	CString		KPEaup;							// 主な通関業者住所（上段）全角漢字	２０文字	
	CString		KPEalw;							// 〃　　　　　　　（下段）〃	
	BYTE		KPSsgn;							// 税抜・税込(3-1)		D0:OFF税抜ON税込(損益) D4:OFF税抜ON税込(資産)	
	BYTE		KPSsta;							// リザーブ	
	CString		KPSnam;							// 所得名称			全角漢字８文字	
	BYTE		KPTsck;							// 取引金額など(3-2)	D0:OFF印刷なし　ON:印刷あり	
	BYTE		KPTsgn;							// 税抜・税込(3-2)	
	BYTE		KPKsck;							// 取引金額など(3-3)	D0:OFF印刷なし　ON:印刷あり	
	BYTE		KPKsgn;							// 税抜・税込(3-3)	
	BYTE		KPSnen;							// 年度（個人）	
	BYTE		KPSndm;							// リザーブ	
	CString		KPSpn1;							// 特殊事情１行目　全角漢字　６０文字	
	CString		KPSpn2;							// 〃　　　２行目　〃	
	CString		KPSpn3;							// 〃　　　３行目　〃	
	CString		KPSpn4;							// 〃　　　４行目　〃	
	CString		KPSpn5;							// 〃　　　５行目　〃	
	CString		KPSpn6;							// 〃　　　６行目　〃	
					   		
	CRefundvol*		m_prefundvol;
	int				nodata;
	void			data();
	CString			ErrMsg;

protected:
	char	rsv[256];
};

// 消費税の還付申告に関する明細書（次葉）
class CREFUNDREC {
public:
	CREFUNDREC( class CRefundrec *refundrec = NULL );
	__declspec(dllexport) int Requery( LPCTSTR filter );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();
	__declspec(dllexport) int Update();
	__declspec(dllexport) int Delete();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int AllDelete();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int ReqSt();			// ret 1=Requery Called

public:
	int			st;
	int			KPItem;							// 種別サイン		1=納付書
	int			KPLine;							// 行番号			1～nn
	int			KPResv;							// 予備 主キー
	int			KPSequ;							// SEQ番号			0～19　履歴保存月 [12'05.01]
	CString		KPSvl1;							// 仕入　決算額
	CString		KPSvl2;							// 〃　　課税にならないもの
	CString		KPSvl3;							// 〃　　課税
	CString		KPSvl4;							// リザーブ
	CString		KPSsup;							// 資産の種類等	（上段）全角漢字	８文字
	CString		KPSslw;							// 〃			（下段）〃
	int			KPSday;							// 取得及び譲渡等　年月日	例[20120101]
	BYTE		KPSdys;							// 〃			   継続サイン
	BYTE		KPSddm;							// リザーブ
	CString		KPSval;							// 取引金額
	CString		KPSnup;							// 取引先名		（上段）全角漢字	１０文字
	CString		KPSnlw;							// 〃			（下段）〃
	CString		KPSaup;							// 取引先住所	（上段）全角漢字	１５文字
	CString		KPSalw;							// 〃			（下段）〃
	CString		KPStup;							// 主な取引商品	（上段）全角漢字	１０文字
	CString		KPStlw;							// 〃			（下段）〃
	CString		KPSzup;							// 所轄税関名	（上段）全角漢字	８文字
	CString		KPSzlw;							// 〃			（下段）〃
	CString		KPInvno;						// インボイス登録番号 20230704

	CRefundrec*		m_prefundrec;
	int				nodata;
	void			data();
	CString			ErrMsg;

protected:
//	char	rsv[256];
	char	rsv[252];	// 256->252　20230704 KPInvno追加
};

// 仕入れ控除税額に関する明細書の明細入力
class CSKJREC {
public:
	CSKJREC( class CSkjrec *pskjrec=NULL );
	__declspec(dllexport) int Requery( CString filter=_T("") );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) void Delete();		// ret 1=Requery Called
public:
	int			st;

	int			SkjType;						// 返送値
	int			SkjLine;						// 行番号		(キー)
	int			SkjColm;						// 列番号		(キー)
	long		SkjSaveDay;						// SEQ番号			0～19　履歴保存月 [12'05.01]
	CString		AsKndUpBuf;						// 資産の種類（上段）	[漢字半角8文字]
	CString		AsKndLwBuf;						// 資産の種類（下段）	[漢字半角8文字]
	long		SyutokuDay;						// 取得年月日
	CString		TrMny;							// 取引金額等			[BIN 12桁]
	CString		NameUpBuf;						// 取引先氏名（上段）	[漢字半角10文字]
	CString		NameLwBuf;						// 取引先氏名（下段）	[漢字半角10文字]
	CString		AdrUpBuf;						// 取引先住所（上段）	[漢字半角15文字]
	CString		AdrLwBuf;						// 取引先住所（下段）	[漢字半角15文字]
	
	char		sec[256];

	CSkjrec*	m_pskjrec;
	int			nodata;
	void		data();
};

// 申告書 更正の請求
class CREASK {
public:
	CREASK( class CReask *preask=NULL );
	__declspec(dllexport) int Requery( CString filter=_T("") );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();				// 返送値 0=OK -1=ERROR
	__declspec(dllexport) void Delete();			// ret 1=Requery Called
public:
	int			st;
	int			KStype;
	long		KSdata;								// 提出年月日										(平成固定)
	long		KSktdt;								// 更正決定通知書日付								(平成固定)
	int			KSktsw;								// 0=申告、1=更正、2=決定
	long		KSstdt;								// 修正申告提出年月日(又は更正決定通知書受理年月日)	(平成固定)
	long		KSjrdt;								// 更正決定通知書受理年月日							(平成固定)

	CString		KSreas1;							// 更正の請求をする理由等(40) １行目
	CString		KSreas2;							// 〃						  ２行目
	CString		KSreas3;							// 〃						  ３行目
	CString		KSreas4;							// 〃						  予　備
	CString		KSdocu1;							// 添付書類(15)	１行目
	CString		KSdocu2;							// 〃			２行目
	CString		KSdocu3;							// 〃			予　備
	int			KVvflg1;							//  1 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval1;							//  1 この請求前の金額(確定額)
	CString		KVkval1;							//  1 更正の請求金額(正当とする額)
	int			KVvflg2;							//  2 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval2;							//  2 この請求前の金額(確定額)
	CString		KVkval2;							//  2 更正の請求金額(正当とする額)
	int			KVvflg3;							//  3 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval3;							//  3 この請求前の金額(確定額)
	CString		KVkval3;							//  3 更正の請求金額(正当とする額)
	int			KVvflg4;							//  4 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval4;							//  4 この請求前の金額(確定額)
	CString		KVkval4;							//  4 更正の請求金額(正当とする額)
	int			KVvflg5;							//  5 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval5;							//  5 この請求前の金額(確定額)
	CString		KVkval5;							//  5 更正の請求金額(正当とする額)
	int			KVvflg6;							//  6 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval6;							//  6 この請求前の金額(確定額)
	CString		KVkval6;							//  6 更正の請求金額(正当とする額)
	int			KVvflg7;							//  7 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval7;							//  7 この請求前の金額(確定額)
	CString		KVkval7;							//  7 更正の請求金額(正当とする額)
	int			KVvflg8;							//  8 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval8;							//  8 この請求前の金額(確定額)
	CString		KVkval8;							//  8 更正の請求金額(正当とする額)
	int			KVvflg9;							//  9 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval9;							//  9 この請求前の金額(確定額)
	CString		KVkval9;							//  9 更正の請求金額(正当とする額)
	int			KVvflg10;							// 10 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval10;							// 10 この請求前の金額(確定額)
	CString		KVkval10;							// 10 更正の請求金額(正当とする額)
	int			KVvflg11;							// 11 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval11;							// 11 この請求前の金額(確定額)
	CString		KVkval11;							// 11 更正の請求金額(正当とする額)
	int			KVvflg12;							// 12 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval12;							// 12 この請求前の金額(確定額)
	CString		KVkval12;							// 12 更正の請求金額(正当とする額)
	int			KVvflg13;							// 13 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval13;							// 13 この請求前の金額(確定額)
	CString		KVkval13;							// 13 更正の請求金額(正当とする額)
	int			KVvflg14;							// 14 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval14;							// 14 この請求前の金額(確定額)
	CString		KVkval14;							// 14 更正の請求金額(正当とする額)
	int			KVvflg15;							// 15 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval15;							// 15 この請求前の金額(確定額)
	CString		KVkval15;							// 15 更正の請求金額(正当とする額)
	int			KVvflg16;							// 16 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval16;							// 16 この請求前の金額(確定額)
	CString		KVkval16;							// 16 更正の請求金額(正当とする額)
	int			KVvflg17;							// 17 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval17;							// 17 この請求前の金額(確定額)
	CString		KVkval17;							// 17 更正の請求金額(正当とする額)
	int			KVvflg18;							// 18 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval18;							// 18 この請求前の金額(確定額)
	CString		KVkval18;							// 18 更正の請求金額(正当とする額)
	int			KVvflg19;							// 19 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval19;							// 19 この請求前の金額(確定額)
	CString		KVkval19;							// 19 更正の請求金額(正当とする額)
	int			KVvflg20;							// 20 実額サイン　D0:KVsval D1:KVkval
	CString		KVsval20;							// 20 この請求前の金額(確定額)
	CString		KVkval20;							// 20 更正の請求金額(正当とする額)
													
	CString		Bname;								// 銀行等名称	(40)
	int			Bkbn;								// 銀行等区分(0:銀行/1:金庫/2:組合/3:漁協/4:農協)
	CString		Sname;								// 本支店名称	(40)
	int			Skbn;								// 本支店区分(1:本店/2:支店/3:本所/4:支所)
	CString		Dname;								// 預金			(30)
	CString		Accnt;								// 口座番号		(20)
													
	CString		Pname;								// 郵便局名称	(50)
	CString		Acnt1;								// 口座番号１	(20)
	CString		Acnt2;								// 口座番号２	(20)

	int			KSbpsw;								// 銀行又は郵便局選択サイン
	int			KSthsw;								// 適用法令の区分サイン
	int			KSkksw;								// 確定額区分サインサイン
	CString		Kikakutei1;							// 13 この請求前の既確定税額　更正の請求/新様式対応
	CString		Kikakutei2;							// 21 この請求前の既確定譲渡割額　更正の請求/新様式対応
	char		sec[232];							// 240->232　Kikakutei1/Kikakutei2追加のため

	CReask*		m_preask;
	int			nodata;
	void		data();
};

// 申告書用　基礎計算表
class CKJSNK {
public:
	CKJSNK( class CKjsnk *pkjsnk=NULL );
	__declspec(dllexport) int Requery( CString filter=_T("") );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();				// 返送値 0=OK -1=ERROR
	__declspec(dllexport) void Delete();			// ret 1=Requery Called
public:
	int			st;
	int			kslist;								// 計算表　番号
	int			ksline;								// 行番号
	int			ksitem;								// 列番号
	BYTE		kssgn1;								// 集計サイン
	BYTE		kssgn2;								// 金額サイン
	BYTE		kssgn3;								// 実額入力サイン
	BYTE		kssgn4;								// 項目サイン
	BYTE		kssgn5;								// 処理サイン
	CString		ksname;								// 項目名
	CString		ksval0;								// 金額１
	CString		ksval1;								// 金額２
	CString		ksval2;								// 金額３
	CString		ksval3;								// 金額４
	CString		ksval4;								// 金額５
	CString		ksval5;								// 金額６
	char		sec[252];

	CKjsnk*		m_pkjsnk;
	int			nodata;
	void		data();
};

// 消費税申告書（金額） ('14.01.06)
class CSH_LST_VAL {
public:
	CSH_LST_VAL( class CSh_lst_val *pSh_lst_val=NULL );
	__declspec(dllexport) int Requery( CString filter=_T("") );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();				// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Delete();			// ret 1=Requery Called
public:
	int				st;
	int				sn_seq;				// 履歴シーケンス
	int				version;			// バージョン
	CString			hyoId;				// 帳票ID
	CString			itmId;				// アイテムID
	CString			val;				// 金額
	int				sign1;				// サイン１
	CString			ksname;				// 項目名		230718Aw add
	int				kssgn1;				// 集計サイン	230718Aw add
	//char			sec[256];							// 230718Aw del
	char			sec[248];							// 230718Aw add ksname,kssgn1追加したため

	CSh_lst_val		*m_pSh_lst_val;
	int				nodata;
	void			data();
};

// 消費税申告書（割合） ('14.01.06)
class CSH_LST_RATE {
public:
	CSH_LST_RATE( class CSh_lst_rate *pSh_lst_rate=NULL );
	__declspec(dllexport) int Requery( CString filter=_T("") );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();				// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Delete();			// ret 1=Requery Called
public:
	int				st;
	int				sn_seq;				// 履歴シーケンス
	int				version;			// バージョン
	CString			hyoId;				// 帳票ID
	CString			itmId;				// アイテムID
	int				rate;				// 割合
	int				sign1;				// サイン１
	char			sec[256];

	CSh_lst_rate	*m_pSh_lst_rate;
	int				nodata;
	void			data();
};

//2018.03.13 INSERT START
class CFHYO60REC {
public:
	CFHYO60REC( class CFhyo60Rec *fhyo60rec = NULL );
	__declspec(dllexport) int Requery( CString filter=_T("") );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) void Delete();		// ret 1=Requery Called
public:
	int			st;
	int			Sn_SEQ;
	int			Sn_6FSDEATH;
	CString		Sn_6FSDAIHYO;
	int			Sn_6FSLIMIT;

	int			Sn_6BS_USE;		// 事業承継の有無（1:有, 0:無）
	CString		Sn_6BS_ADR;		// 事業承継の住所
	CString		Sn_6BS_TELNO;	// 事業承継の電話番号
	CString		Sn_6BS_NMFRI;	// 事業承継のフリガナ
	CString		Sn_6BS_NAME;	// 事業承継の氏名
	CString		Sn_6BS_SONOTA;	// 事業承継のその他

	CFhyo60Rec*	m_pfhyo60rec;
	int			nodata;
	void		data();

protected:
	char		rsv[256];
};

class CSYZ_SOZOKU {
public:
	CSYZ_SOZOKU( class CSyz_Sozoku *syz_sozoku = NULL );
	__declspec(dllexport) int Requery( CString filter=_T("") );
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) void Delete();		// ret 1=Requery Called

public:
	int			st;
	int			Sn_SEQ;
	int			Sn_NO;
	CString		Sn_6FSJUSYO;
	CString		Sn_6FSFURI;
	CString		Sn_6FSNAME;
	CString		Sn_6FSHASH;
	CString		Sn_6FSSYOKU;
	CString		Sn_6FSTSUDUKI;
	int			Sn_6FSBIRTH;
	CString		Sn_6FSTEL1;
	CString		Sn_6FSTEL2;
	CString		Sn_6FSTEL3;
	int			Sn_6FSHOTEI;
	CString		Sn_6FSBUNSHI;
	CString		Sn_6FSBUNBO;
	CString		Sn_6FSKAGAKU;
	CString		Sn_6FSNOFUZEI;
	CString		Sn_6FSNOFUCHIHO;
	CString		Sn_6FSKANPUZEI;
	CString		Sn_6FSKANPUCHIHO;
	int			Sn_6FSNOFUKANPUsw;
	CString		Sn_6FSGNAME;
	int			Sn_6FSGSEL;
	CString		Sn_6FSSNAME;
	int			Sn_6FSSSEL;
	CString		Sn_6FSYOKIN;
	CString		Sn_6FSKOUZA;
	CString		Sn_6FSKIGO1;
	CString		Sn_6FSKIGO2;
	CString		Sn_6FSYUBIN;

	CSyz_Sozoku*	m_psyz_sozoku;
	int				nodata;
	void			data();

protected:
	char		rsv[256];
};
//2018.03.13 INSERT END

class CSYZ_TOKUREI {
public:
	CSYZ_TOKUREI(class CSyz_Tokurei *syz_tokurei = NULL);
	__declspec(dllexport) int Requery(CString filter = _T(""));
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) void Delete();		// ret 1=Requery Called

public:
	int           st;
    int           sn_seq;
    int           version;
    CString       hyoId;
    CString       itemId;
    int           idx;
    int           kind;
    CString       val;
    int           rate;
    CString       str;
    int           sign1;

	CSyz_Tokurei* m_psyz_tokurei;
	int			  nodata;
	void	      data();

protected:
	char		  rsv[256];
};

class CSZSYKSUB{
public:
    CSZSYKSUB(class CSzsyksub *szsyksub = NULL);
	__declspec(dllexport) int Requery(CString filter = _T(""));
	__declspec(dllexport) int MoveFirst();
	__declspec(dllexport) int MoveNext();
	__declspec(dllexport) int MovePrev();
	__declspec(dllexport) int MoveLast();
	__declspec(dllexport) int Move(int nrows);
	__declspec(dllexport) int SetAbsolutePosition(int nrows);
	__declspec(dllexport) int GetRecordCount();
	__declspec(dllexport) void Edit();
	__declspec(dllexport) void AddNew();		// 返送値 0=OK -1=ERROR
	__declspec(dllexport) int Update();			// 返送値 0=OK -1=ERROR
	__declspec(dllexport) void Delete();		// ret 1=Requery Called

public:
	int st;
    CString Szkcod;
    BYTE Szgrp1;
    BYTE Szgrp2;
    BYTE SZzkbn;
    BYTE Szitm;
    BYTE SZsjsg;
    BYTE Szkatt;
    BYTE SZzchg;
    CByteArray  SZrsv;
    BYTE Syksubofs;
    CString Sztaig;
    CString Szhikz;
    CString Szyuka;
    CString Szkomi;
    CString Szkomz;
    CString Sznuki;
    CString Sznukz;
    CString Szbeki;

public:
	CSzsyksub*    m_pSzsyksub;
	int			  nodata;
	void	      data();
	CString		  ErrMsg;

protected:
	char		  rsv[256];

};

//int GetSyzZeiCalqForEachMonth2(CDBZmSub* pZmsub, CArray<ZeiCalqEachMonth, ZeiCalqEachMonth&>* pArray, unsigned char& Sn_TUMIAG)
//結果格納用構造体
typedef struct _ZeiCalqEachMonth {
    int month_info_ofs; //month_infoテーブルのofset.
	int zeicalq_uri;      //割戻し:0 積上げ:1 混合:2 期間外:-1
                          //インボイス施工日跨ぎの場合
						  //消費税申告書設定 売上・積上げ(呼出関数の引数Sn_TUMIAGで設定)→10/1以降割戻し 戻り値、混合：3
                          //消費税申告書設定 売上・積上げ(呼出関数の引数Sn_TUMIAGで設定)→10/1以降積上げ 戻り値、混合：4 
                          //消費税申告書設定 売上・割戻し(呼出関数の引数Sn_TUMIAGで設定)→10/1以降割戻し 戻り値、混合：5
                          //消費税申告書設定 売上・割戻し(呼出関数の引数Sn_TUMIAGで設定)→10/1以降積上げ 戻り値、混合：6
	int zeicalq_sire;     //割戻し:0 積上げ:1 混合:2 期間外:-1
						  //インボイス施工日跨ぎの場合
						  //消費税申告書設定 仕入・積上げ(呼出関数の引数Sn_TUMIAGで設定)→10/1以降割戻し 戻り値、混合：3
						  //消費税申告書設定 仕入・積上げ(呼出関数の引数Sn_TUMIAGで設定)→10/1以降積上げ 戻り値、混合：4 
						  //消費税申告書設定 仕入・割戻し(呼出関数の引数Sn_TUMIAGで設定)→10/1以降割戻し 戻り値、混合：5
						  //消費税申告書設定 仕入・割戻し(呼出関数の引数Sn_TUMIAGで設定)→10/1以降積上げ 戻り値、混合：6
} ZeiCalqEachMonth;

typedef struct _SiwakeRec {

	long ddate;
	CString dbt;
	char dbtgroup;
	char dbtgroupmeisai;
	CString cre;
	char cregroup;
	char cregroupmeisai;
	CString val;
	CString zei;
	CString taika;
	char zeikubun;
	char zeiritsu;
	char syutokujoto;
	char jigyokubun;
	char siwakekubun;

} SiwakeRec;

typedef enum {
	ID_KARIKATA = 0, //借方科目
	ID_KASHIKATA     //貸方科目
} EnumIdKariKashiKbn;

/* 消費税仕訳区分 */
typedef enum {

	ID_SKA_URIAGE = 1,                    //  1: 売　　　　　上
	ID_SKA_KASHIDAORESON = 2,             //  2: 貸　倒　損  失
	ID_SKA_KASHIDAOREKAISYU = 3,          //  3: 貸　倒　回　収
	ID_SKA_YUNYUSIRE = 4,                 //  4: 輸　入　仕　入
	ID_SKA_HIKAZEIYUSYUTSUURIAGE = 5,     //  5: 非課税輸出売上  
	ID_SKA_HANBAISYOUREIKINNYU = 6,       //  6: 販売奨励金収入
	ID_SKA_HANBAISYOUREIKINHIYO = 7,      //  7: 販売奨励金費用  
	ID_SKA_SHIIREWARIBIKI = 8,            //  8: 仕　入　割　引
	ID_SKA_URIAGEWARIBIKI = 9,            //  9: 売　上　割　引  
	ID_SKA_TANAKASAN = 10,                // 10: 棚卸調整加算額
	ID_SKA_TANAGENSAN = 11,               // 11: 棚卸調整減算額
	ID_SKA_YUSYUTSUMENZEIURIAGE = 13,     // 13: 輸出　免税売上 
	ID_SKA_URIAGENEBIKIMODORI = 17,       // 17: 売上値引・戻り
	ID_SKA_UKETORIRISOKU = 20,            // 20: 受　取　利　息
	ID_SKA_SHIIRE = 21,                   // 21: 仕　　　　　入
	ID_SKA_KEIHI = 23,                    // 23: 経　　　　　費
	ID_SKA_SYOUHIZEISEISAN = 28,          // 28: 消 費 税 清 算
	ID_SKA_TKKZ_SHIIRE_SUB = 29,          // 29: 特定課税仕入（本体）
	ID_SKA_TKKZ_SHIIREHEN_SUB = 30,       // 30: 特定課税仕入の返還（本体）
	ID_SKA_YUNYUSHIIRE_KOKUZEI = 32,      // 32: 輸入仕入（国税)
	ID_SKA_YUNYUSHIIRE_TIHOUZEI = 33,     // 33: 輸入仕入（地方税）
	ID_SKA_YUNYUSHIIRE_KOKUTIGASSAN = 34, // 34: 輸入仕入（国地合算）

} EnumIdSiwakeKubun;

//消費税処理グループ区分
#define KAMOKUGROUP_SISAN       0x01 //資産
#define KAMOKUGROUP_URIAGE      0x02 //売上
#define KAMOKUGROUP_SIRE        0x04 //仕入
#define KAMOKUGROUP_KEIHI       0x08 //経費
#define KAMOKUGROUP_TAIKASISAN  0x11 //対価&資産

#define KAMOKUMEISAI_SISAN_SYUTOKU 0x00
#define KAMOKUMEISAI_SISAN_JOTO    0x01

#define KAMOKUMEISAI_URIAGE_URIAGE                 0x00
#define KAMOKUMEISAI_URIAGE_URIAGEHENPIN           0x01
#define KAMOKUMEISAI_URIAGE_EIGYOGAISYUEKI         0x02
#define KAMOKUMEISAI_URIAGE_TOKUBETSURIEKI         0x03
#define KAMOKUMEISAI_URIAGE_UKETORIRISOKU          0x04
#define KAMOKUMEISAI_URIAGE_KASHIDAOREKAISYU       0x05
#define KAMOKUMEISAI_URIAGE_HANBAISYOREIKINHIYO    0x06
#define KAMOKUMEISAI_URIAGE_URIAGEWARIBIKI         0x07
#define KAMOKUMEISAI_URIAGE_HIKAZEIYUSYUTSUURIAGE  0x08
#define KAMOKUMEISAI_URIAGE_YUSYUTUMENZEIURIAGE    0x09
#define KAMOKUMEISAI_URIAGE_KARIUKESYOUHIZEISEISAN 0x0A

#define KAMOKUMEISAI_SIRE_SIRE                     0x00
#define KAMOKUMEISAI_SIRE_SIREHENPIN               0x01
#define KAMOKUMEISAI_SIRE_HANBAISYOIREIKINSYUNYU   0x02
#define KAMOKUMEISAI_SIRE_SIREWARIBIKI             0x03
#define KAMOKUMEISAI_SIRE_YUNYUSIRE                0x04
#define KAMOKUMEISAI_SIRE_TANAOROSHIKASAN          0x05
#define KAMOKUMEISAI_SIRE_TANAOROSHIGENSAN         0x06
#define KAMOKUMEISAI_SIRE_KARIBARAISYOUHIZEISEISAN 0x07

#define KAMOKUMEISAI_TAIKASISAN_SYUTOKUJOTO    0x02
#define KAMOKUMEISAI_TAIKASISAN_YUKASYOKENJOTO 0x03
#define KAMOKUMEISAI_TAIKASISAN_TOTIJOTO       0x04

#define KAMOKUMEISAI_KEIHI_KEIHI             0x00
#define KAMOKUMEISAI_KEIHI_EIGYOGAIHIYO      0x01
#define KAMOKUMEISAI_KEIHI_TOKUBETSUSONSITSU 0x02
#define KAMOKUMEISAI_KEIHI_KASIDAORESONSITSU 0x03

const int INVOICE_SYZ_YMD = 20231001;			// 消費税：インボイス施行日

#define SISAN_SYUTOKU      0x00
#define SISAN_JOTO         0x20

#define ZEIKUBUN_SOTOZEI   0x00
#define ZEIKUBUN_UTIZEI    0x10
#define ZEIKUBUN_BEKKI     0x20

#define ZEIRITSU_8         0x03
#define ZEIRITSU_8KEIGEN   0x04
#define ZEIRITSU_10        0x05