// DenpScan.h : 伝票形式スキャンクラス
#ifndef INCLUDE__DENPSCAN_H_
#define INCLUDE__DENPSCAN_H_


typedef struct tab_SheetSykbn
{
	DWORD		swk_sw,		//消費税仕訳区分
				ritu_sw,	//税率
				zeikbn_sw,	//課税区分
				uri_sw,		//売上・仕入
				toku_sw,	//特定収入[06.03 /08]
				kts_sw;		//固定資産譲渡・取得
} SHEET_SYKBN;


typedef	struct	tag_SheetTable {
	int			icno;			//内部伝票番号
	int			dtype;			//伝票種別
	int			s_seq,			//この伝票での仕訳開始SEQ
				e_seq;			//この伝票での仕訳終了SEQ
	int			page,			//ページ番号(1～)
				dp_end;			//伝票終了サイン
	int			newcnt;			//新規追加仕訳数

	int			kari[DP_SCROLL_MAX];		//借方位置のシーケンス番号
	int			kasi[DP_SCROLL_MAX];		//貸方位置のシーケンス番号
	int			karisgn[DP_SCROLL_MAX];	//借方位置データ有無サイン
	int			kasisgn[DP_SCROLL_MAX];	//貸方位置データ有無サイン
	int			delsgn[DP_SCROLL_MAX];	//一行取消サイン
	int			inssgn[DP_SCROLL_MAX];	//一行挿入サイン

	char		karittl[6];		//仕訳区切り単位での借方金額合計
	char		kasittl[6];		//仕訳区切り単位での貸方金額合計

	CDBINPDataRec	kariorg[DP_SCROLL_MAX],	//修正前 借方データ 
					kasiorg[DP_SCROLL_MAX];	//		 貸方データ

	//これ以降は、仕訳区切りが複数伝票に渡る時使用する
	CDBINPDataRec	karirec[DP_SCROLL_MAX],	//修正	借方データ 
					kasirec[DP_SCROLL_MAX];	//		貸方データ

	int			addkari[DP_SCROLL_MAX], addkasi[DP_SCROLL_MAX];		//追加データ有無サイン
	SHEET_SYKBN	zeikari[DP_SCROLL_MAX], zeikasi[DP_SCROLL_MAX];		//追加データの消費税区分コード
	int			delkari[DP_SCROLL_MAX], delkasi[DP_SCROLL_MAX];		//取消サイン

} SHEET_TABLE;


// 追加データ用構造体
typedef struct tag_AddNewTbl
{
	int		ln;			// 追加行
	int		karikasi;	// 0:借方　1:貸方　2:単一仕訳
	int		insseq;		// 挿入用SEQ
} ADDNEWTBL;


// 内部伝票ごとの日付情報の構造体。
typedef struct tag_IcnoInfo
{
	int		ii_icno;	//内部伝票番号
	int		ii_date;	//伝票の先頭仕訳の日付
	int		ii_mofs;	//伝票の先頭仕訳の月オフセット
	int		ii_dpNo;	//伝票番号
} ICNOINFO;


class CDenpScan
{
private:
	enum {
		LINE_FURIKAE			=	7,
		LINE_NYUSYUKKIN			=	6,
		LINE_DENP				=	DP_SCROLL_NL,
		TYPE_FURIKAE			=	0x30,
		TYPE_NYUKIN				=	0x10,
		TYPE_SYUKKIN			=	0x20,
	};

public:
	CDenpScan();
	virtual ~CDenpScan();

	void	DenpScanInit( CDBZmSub* pZm );
	void	DenpScanInit( CDInputSub* pDInput, DENP_INPUTPAR* denpara );

	int		DenpScanOpen( CString filter = _T( "" ), WORD dnpsgn = 0 );
	void	Close();
	
	int		SetFirstSheet();
	int		SetLastSheet(int dseq);
	int		SetNextSheet( BOOL bNext = FALSE );
	int		SetPrevSheet( BOOL bPrev = FALSE );
	int		SetCurrentSheet();

	int		DenpyoType() { return	m_Sheet.dtype; };

	int		IsOriginData( int ln, int karikasi );
	int		IsCrctData( int ln, int karikasi );
	BOOL	IsAddData( int ln, int pn );
	int		IsDeleteData( int ln, int pn );
	BOOL	IsFurikaeEnd( int vect );
	BOOL	IsFukugoSiwake( int ln );

	// 伝票検索モード
	//	0： 直接伝票画面に切り替え
	//	1： 検索より、伝票画面を起動した。
	//
	int&	DenpScanMode() {	return m_DnpMode;	};

	int		GetSheetCount();
	int		GetSeq( int ln, int karikasi );

	CDBINPDataRec&	GetOriginData( int ln, int karikasi );
	void			GetCrctData( int ln, int karikasi, CDBINPDataRec* drec );
	// 02.20 /08
	void	SetOriginData( int ln, int karikasi, CDBINPDataRec& rec );
	void	SheetTableUpdate( int newcnt, CDBINPDataRec* rec, ADDNEWTBL* antbl );

	int&	DeleteLine( int ln ) { return m_Sheet.delsgn[ln-1]; }
	char*	GetKariTotal() { return m_Sheet.karittl; }
	char*	GetKasiTotal() { return m_Sheet.kasittl; }
	int		GetSheetICNO() { return m_Sheet.icno; }

	void	SetSheetTotal( unsigned char* kari, unsigned char* kasi );

	int&	SheetNewCount() { return m_Sheet.newcnt; }
	void	AddSheetTable( int ln, int mode, CDBINPDataRec* drec, struct __SYDATA* syd );

	SHEET_SYKBN&		GetSheetSykbn( int ln, int karikasi );

	void	SetOriginValttl();
	int		DenpDelelteJob();

	int		InsertDenpLine( int ln );
	void	InsertLineJob( int ln, SHEET_TABLE* ptb,  SHEET_TABLE* prev );
	int		LastLineCheck( SHEET_TABLE* ptb );
	BOOL	IsInsertLine( int ln );

	int		DeleteDenpLine( int ln );
	void	DeleteLineJob( int ln, SHEET_TABLE* ptbl,  SHEET_TABLE* next );

	BOOL&	SyogSiwakeMode() {	return m_syogMode; }
	int		get_MaxLine( int dtype );
	BOOL	IsFuriSyogPostion( int ln, int pn );
	BOOL	IsFuriNotSyogPostion(int ln, int dcsw);
	BOOL	IsTanitsuSiwake( int ln );
	int		GetLastDataLine();
	BOOL	GetDenpyoLineFromSeq( int seq, int& line, int& dc_sw );

protected:
	int		set_sheet( DWORD icno, int vect, int dseq=-1);
	int		set_sheet_old( DWORD icno, int vect );

	void	init_sheet( SHEET_TABLE* tbl );
	
	int		get_scanuser();
	int		data_authority();

private:
	CDBZmSub*	m_pZm;
	CDInputSub*		m_pDInput;
	DENP_INPUTPAR*	m_pDENPPAR;

	int			m_DnpMode;

	CDWordArray	m_daICNO;
	int			m_daINDEX;

	SHEET_TABLE m_Sheet;
	CArray<SHEET_TABLE, SHEET_TABLE&>	m_ShtArray;

	CArray<ICNOINFO, ICNOINFO&>		m_ICNOArray;

	BOOL		m_syogMode;

	int			m_user_att;
};


#endif // INCLUDE__DENPSCAN_H_