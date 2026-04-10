#ifndef INCLUDE_DBINPSUBFUNC_H
#define INCLUDE_DBINPSUBFUNC_H


#define SW_HOJIN		0x0001		//法人
#define SW_KOJIN		0x0002		//個人
#define SW_UNSO			0x0004		//運送
#define SW_KENST		0x0008		//建設
#define SW_BYOIN		0x0010		//病院
#define SW_PUB			0x0020		//公益法人
#define SW_PUB3			0x0040		//公益法人III
#define SW_SOCIAL		0x0080		//社会福祉
#define SW_SOCIAL2		0x0100		//社会福祉II
#define SW_SYUKYO		0x0200		//宗教
#define SW_SCHOOL		0x0400		//学校


// 摘要レコード
typedef struct _DIS_TKREC
{
	long	tk_code;
	char	tk_kana[6];
	char	tk_name[64];
	char	tk_sign[2];

	char	tk_rsv[12];
} DIS_TKREC;


// 文字列科目コードより名称レコードを取得
struct _DBKNREC* DB_PjisToKnrec( CString pjiscode, CDBZmSub* pZm, BOOL bALL = FALSE );
// DWORD 科目コードより名称レコードを取得
struct _DBKNREC* DB_PjisToKnrec( DWORD dwPjis, CDBZmSub* pZm, BOOL bALL = FALSE );
// 合計名称拡張元 取得
DBGKREC* get_gksrc( DWORD gkcode, CDBZmSub* pZm );

// 収支名称レコード取得
struct _DBKNREC* DB_SyusiPjisToKnrec( CString pjiscode, CDBZmSub* pZm );
struct _DBKNREC* DB_SyusiPjisToKnrec( DWORD dwPjis, CDBZmSub* pZm );

/////////////////////////////////////
// int 型コードを 0をつめて 文字列可
/////////////////////////////////////
void set_codestr( char* str, int strsize, int code, int keta );

// 文字列 型コードを 0をつめて 文字列可
void set_codestring( char* str, int strsize, char* codestring, int keta );

// int 型コードを文字列可
void TV_kamokucodeConv( WORD& kmkcode, CString& codestr, int sgn );

TKY_REGPAR* get_tkyregpar();
TKY_VER2*	get_tkyver2();
BOOL& tkquery_flag();
void tkdb_init( CDBZmSub* pZm );
void skbn_msg( CString& symsg, unsigned char TKszk, CDBZmSub* pZm, CDBSyohi* pSy );

int tkyngetr( int tkcode, CDBZmSub* pZm );
int tkykana_search( char* kana, CDBZmSub* pZm );
int get_tknum( CDBZmSub* pZm );

int getKamokucodeSign();

#include "afxtempl.h"

typedef struct _SWKCOD_DATA
{
	CString sc_dbt,
			sc_cre;

	int		sc_sgn;	// 0 ... データベースにはまだない
					// 1 ... データベースに登録あり

	int		sc_chg;	// 1 ... 仕訳の借方・貸方科目に変更があった

} SWKCOD_DATA;

typedef CArray<SWKCOD_DATA, SWKCOD_DATA&>	CSwcdArray;

void furikana_chk( char* buf, int bfsize, CString& strText );

void Dlg_GetRate( ICSDialog* pDlg, double& grx, double& gry );

BOOL IsSyafuKaisei( CZVOLUME* zv );

int ZmsubOwntbOpen( CDBZmSub* pZm, BOOL& bOpen, BOOL bAddRecord );

int vector_finder(std::vector<int> vec, int number);

BOOL sotozei_data( CDBipDataRec *drec );
BOOL sotozei_data(CDBINPDataRec *drec);
BOOL _sotozei_data( BYTE *dsign );
BOOL _joto_data( BYTE *dsign );

BOOL _isSyzdiff_data( BYTE *newdsign, BYTE *motodsign );

BOOL IsOKGyosyu( int apno, DWORD check_bit );
BYTE KaniDefaultKubun( CZVOLUME* zv );

int brn_inptype( int nowtype, CString nowcode, int nowbrn, CString mdfycode, int mdfybrn );

//消費税の課税期間サインを返送
int	GetSyzDateSign(int date);

int GetInvoiceInfo(CDBZmSub* pZm, CArray<INVOICE_INFO>& InvAry);
LPCTSTR GetMenzeiWariaiString(int sel);

void clear_TKYV2_SAVE(TKYV2_SAVE* psv);

void convNewDataRec(CDBipDataRec* old, CDBINPDataRec* newdata);

// カード通帳の消費税ダイアログ簡易版(税率・税区分のみ）
class CSyzInpLiteSub
{
public:
	CSyzInpLiteSub();
	~CSyzInpLiteSub();

public:
	int	SetPara(CDBZmSub* pZm, CWnd* parent);
	int	SetParaNoDlg(CDBZmSub* pZm);

	int SetSyzData(CDBDATA* pdata);

	int	TekiyoSyzData(int tksyz);
	int	TekiyoSyzData(struct _SY_TKREC_* psytk);
	int	UpdateSyzData(CDBDATA* pdata);

	int	SyGetMessage();

	int		DialogON(BOOL bOK, CRect rect, int posflg, int pn);
	int		DialogOFF();
	BOOL	IsDisplay();
	void	GetCrntRect(CSize& size);

	int		SetSkbnSelNo(int no);
	void	set_nextpos();
	void	set_nowfocus();
	CString GetSyzLineText();
	void	GetNowSyzData(CDBDATA* pdata);
	void	Close();
	int		szvol_check();
	int		karisyz_check(CDBDATA* pData = NULL);
	//インボイス対応
	int		GetInvoiceInfo();
	void	SetDispMode( int mode );//D0:インボイスを表示する
	void	ReloadData();
	BOOL	CheckInvno();
	BOOL	SotoNotCut();

public:
	CDBZmSub*	m_pZm;
	CDBSyohi*	m_pSy;
	CWnd*		m_pParent;

	CSkbnLiteDlg*	m_pDialog;
	BOOL			m_bResize;

	CDBDATA		m_dbdata;
	CString		m_sylinetext;

	struct _SY_MSG_PACK2 m_smpk;

	CString		m_ErrMsg;

	BOOL		m_bNoDlg;

	BOOL		m_bSzvolOpen;

private:
	// インボイス対応
	bool m_bInvMaster;
	bool m_bInvUse;
	bool m_bInvDisp;
	int  m_dispmode;
	CArray<INVOICE_INFO> m_InvAry;
	int m_Koujo80_50;
};




#endif	// INCLUDE_DBINPSUBFUNC_H
