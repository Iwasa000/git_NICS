// RpkeyDlg.h : ヘッダー ファイル
//
//#include "icsdialog.h"

/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#ifdef _ICSPEN_	// 01.20 /01
//#include "ICSPEN.h"
#endif

#include "husencombo.h"
#include "imagestatic.h"
#include <SyzInvoice.h>


#define RP_DATE1_PN		0	//日付（前）
#define RP_DATE2_PN		1	//日付（後）
#define RP_DENP1_PN		2	//伝票番号（前）
#define RP_DENP2_PN		3	//伝票番号（後）
// 証憑番号		[11.26 /12]
#define RP_DOCEVI1_PN	4	//証憑番号（前）
#define RP_DOCEVI2_PN	5	//証憑番号（後）

#define RP_BMON1_PN		6	//借方部門（前）
#define RP_BMON2_PN		7	//　　部門（後）
#define RP_CBMN1_PN		8	//貸方部門（前）
#define RP_CBMN2_PN		9	//　　部門（後）
						
#define RP_KOJI1_PN		10	//借方工事（前）
#define RP_KOJI2_PN		11	//　　工事（後）
#define RP_CKJI1_PN		12	//貸方工事（前）
#define RP_CKJI2_PN		13	//　　工事（後）
						
#define RP_DEBT1_PN		14	//借方（前）
#define RP_DEBT2_PN		15	//借方（後）
// インボイス対応
#define RP_INVNO_PN		16	// インボイス登録番号
#define RP_INVNO2_PN	17	// インボイス登録番号
						
#define RP_CRED1_PN		18	//貸方（前）
#define RP_CRED2_PN		19	//貸方（後）
						
#define RP_VAL1_PN		20	//金額（前）
#define RP_VAL2_PN		21	//金額（後）

// インボイス対応
#define RP_MENZEI_PN	22	// 免税事業者からの課税仕入れ

#define RP_MENWARI1_PN	23	// 免税事業者からの課税仕入れ 控除割合（前）
#define RP_MENWARI2_PN	24	// 免税事業者からの課税仕入れ		  （後）

#define RP_KAZEI_PN		25	// 課税事業者からの課税仕入れ

#define RP_SYZSW1_PN	26	//消費税仕訳区分（前）
#define RP_SYZSW2_PN	27	//				（後）
#define RP_ZRITU1_PN	28	//税率（前）
#define RP_ZRITU2_PN	29	//　　（後）
#define RP_SKBN1_PN		30	//消費税区分（前）
#define RP_SKBN2_PN		31	//　　　　　（後）
#define RP_URISIRE1_PN	32	//業種・売り仕入区分（前）
#define RP_URISIRE2_PN	33	//　　　　　　　　　（後）
						
#define RP_TOKUS1_PN	34	//特定収入（前）
#define RP_TOKUS2_PN	35	//　　　　（後）
						
#define RP_TKY1_PN		36	//摘要（前）
#define RP_TKY2_PN		37	//摘要（後）
#define RP_HJIYU1_PN	38	//変動事由	（前）
#define RP_HJIYU2_PN	39	//			（後）
#define RP_SNUM1_PN		40	//手形番号
#define RP_SNUM2_PN		41	//手形番号
#define RP_TDAT1_PN		42	//手形期日
#define RP_TDAT2_PN		43	//手形期日
#define RP_HSEN1_PN		44	//変更前 付箋
#define RP_HSEN2_PN		45	//	　後 付箋
#define RP_HSENCHK_PN	46	//付箋取消
#define RP_NODENP_PN	47	// 伝票番号なし
#define RP_NODOCEVI_PN	48	// 証憑番号なし
#define RP_NOBMON_PN	49	// 借方部門なし
#define RP_NOCBMN_PN	50	// 貸方部門なし
#define RP_NOKOJI_PN	51	// 借方工事なし
#define RP_NOCKJI_PN	52	// 貸方工事なし
#define RP_NOTKY_PN		53	// 摘要取消

#define RP_CONFIRM_PN	54	//仕訳確定
#define RP_CONMDFY_PN	55	//確定仕訳を未確定にする

// インボイス対応
#define RP_IDOK			(RP_CONMDFY_PN+1)	// ＯＫボタン
#define RP_IDCANCEL		(RP_CONMDFY_PN+2)	// キャンセルボタン
#define RP_IDCBTNDEL	(RP_CONMDFY_PN+3)	// 全削除

#define RP_MNONLY_PN	(RP_CONMDFY_PN+4)	// 月のみ修正


#define RP_MAX_PN		(RP_CONMDFY_PN+1)

//入力データ
struct _RPL_LINE {
	struct _ItemData	_RPDTA[ RP_MAX_PN + 1 ];
#define RP_DATE1_DT		_RPDTA[ RP_DATE1_PN ]	//日付（前）
#define RP_DATE2_DT		_RPDTA[ RP_DATE2_PN ]	//日付（後）
#define RP_DENP1_DT		_RPDTA[ RP_DENP1_PN ]	//伝票番号（前）
#define RP_DENP2_DT		_RPDTA[ RP_DENP2_PN ]	//伝票番号（後）

#define RP_DOCEVI1_DT	_RPDTA[ RP_DOCEVI1_PN ]	//証憑番号（前）
#define RP_DOCEVI2_DT	_RPDTA[ RP_DOCEVI2_PN ]	//証憑番号（後）

#define RP_BMON1_DT		_RPDTA[ RP_BMON1_PN ]	//借方部門（前）
#define RP_BMON2_DT		_RPDTA[ RP_BMON2_PN ]	//借方部門（後）
#define RP_CBMN1_DT		_RPDTA[ RP_CBMN1_PN ]	//貸方部門（前）
#define RP_CBMN2_DT		_RPDTA[ RP_CBMN2_PN ]	//貸方部門（後）
#define RP_KOJI1_DT		_RPDTA[ RP_KOJI1_PN ]	//借方工事（前）
#define RP_KOJI2_DT		_RPDTA[ RP_KOJI2_PN ]	//借方工事（後）
#define RP_CKJI1_DT		_RPDTA[ RP_CKJI1_PN ]	//貸方工事（前）
#define RP_CKJI2_DT		_RPDTA[ RP_CKJI2_PN ]	//貸方工事（後）

#define RP_DEBT1_DT		_RPDTA[ RP_DEBT1_PN ]	//借方
#define RP_DEBT2_DT		_RPDTA[ RP_DEBT2_PN ]	//借方

#define RP_INVNO_DT		_RPDTA[ RP_INVNO_PN ]	// 登録番号
#define RP_INVNO2_DT	_RPDTA[ RP_INVNO2_PN ] // 〃

#define RP_CRED1_DT		_RPDTA[ RP_CRED1_PN ]	//貸方
#define RP_CRED2_DT		_RPDTA[ RP_CRED2_PN ]	//貸方
#define RP_VAL1_DT		_RPDTA[ RP_VAL1_PN ]	//金額
#define RP_VAL2_DT		_RPDTA[ RP_VAL2_PN ]	//金額

#define RP_MENZEI_DT	_RPDTA[ RP_MENZEI_PN ]	// 免税事業者からの課税仕入れ

#define RP_MENWARI1_DT	_RPDTA[ RP_MENWARI1_PN ]// 免税事業者 控除割合
#define RP_MENWARI2_DT	_RPDTA[ RP_MENWARI2_PN ]// 免税事業者 控除割合

#define RP_KAZEI_DT		_RPDTA[ RP_KAZEI_PN ]	// 課税事業者からの課税仕入れ

#define RP_SYZSW1_DT	_RPDTA[ RP_SYZSW1_PN ]	//消費税仕訳区分
#define RP_SYZSW2_DT	_RPDTA[ RP_SYZSW2_PN ]	//
#define RP_ZRITU1_DT	_RPDTA[ RP_ZRITU1_PN ]	//税率
#define RP_ZRITU2_DT	_RPDTA[ RP_ZRITU2_PN ]	//
#define RP_SKBN1_DT		_RPDTA[ RP_SKBN1_PN ]	//消費税区分
#define RP_SKBN2_DT		_RPDTA[ RP_SKBN2_PN ]	//
#define RP_URISIRE1_DT	_RPDTA[ RP_URISIRE1_PN ]//売り仕入・業種
#define RP_URISIRE2_DT	_RPDTA[ RP_URISIRE2_PN ]//
#define RP_TOKUS1_DT	_RPDTA[ RP_TOKUS1_PN ]	//特定収入
#define RP_TOKUS2_DT	_RPDTA[ RP_TOKUS2_PN ]	//
#define RP_TKY1_DT		_RPDTA[ RP_TKY1_PN ]	//摘要
#define RP_TKY2_DT		_RPDTA[ RP_TKY2_PN ]	//

#define RP_SNUM1_DT		_RPDTA[ RP_SNUM1_PN ]	//手形番号
#define RP_SNUM2_DT		_RPDTA[ RP_SNUM2_PN ]	//
#define RP_TDAT1_DT		_RPDTA[ RP_TDAT1_PN ]	//手形期日
#define RP_TDAT2_DT		_RPDTA[ RP_TDAT2_PN ]	//
#define RP_HJIYU1_DT	_RPDTA[ RP_HJIYU1_PN ]	//変動事由
#define RP_HJIYU2_DT	_RPDTA[ RP_HJIYU2_PN ]	//

#define RP_CONFIRM_DT	_RPDTA[ RP_CONFIRM_PN ]	//仕訳確定
#define RP_CONMDFY_DT	_RPDTA[ RP_CONMDFY_PN ]	//確定仕訳を修正可にする

#define RP_HSEN1_DT		_RPDTA[ RP_HSEN1_PN ]	//付箋
#define RP_HSEN2_DT		_RPDTA[ RP_HSEN2_PN ]	//

	struct _LINE_CND LINE_CND[ RP_MAX_PN ];

};



class CRpkeyDlg : public ICSDialog
{
// コンストラクション
public:
	CRpkeyDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual ~CRpkeyDlg();

// ダイアログ データ
	//{{AFX_DATA(CRpkeyDlg)
	enum { IDD = IDD_DKEY_DLG };
	CComboBox	m_Zritu;
	CComboBox	m_Urisire;
	CComboBox	m_Syzsw;
	CComboBox	m_Skbn;
	CComboBox	m_Zritu2;
	CComboBox	m_Urisire2;
	CComboBox	m_Syzsw2;
	CComboBox	m_Skbn2;
	CComboBox	m_Tokus;
	CComboBox	m_Tokus2;
	CComboBox	m_Hjiyu1;
	CComboBox	m_Hjiyu2;
	CHusenCombo	m_Hcbo;
	CHusenCombo	m_Hcbo2;
	CComboBox	m_MenWari;
	CComboBox	m_MenWari2;

	CButton	m_Cancel;
	CButton	m_Ok;
	CButton	m_BtnDel;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

	static void keyline_inz();
	static void set_tekmode( BOOL bMode ) { m_TEK_MODE = bMode; };

	static void CALLBACK callback(UINT n,void* p,CWnd* pwnd );

	int datacmp( CDBDATA* data );
	BOOL ReplaceChk(int data_pn);
	void MakeMdfySiwake(CDBINPDataRec* pDt);

	void SetTekiyoDlgRecord( CDBINPDataRec* data );

	BOOL IsConmdfyCheck();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CRpkeyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	BOOL	m_bSwkDel;

protected:
	CWnd* m_pParent;
	static struct _RPL_LINE m_Rpline;
	static short	m_TEK_MODE;
	short	m_PGSW;
	int		m_NOWPN;
	short m_svSyzsw, m_svSyzRitu, m_svSyzSkbn, m_svUriSire, m_svTaika, m_svTokus, m_svMenzei;
	CDBINPDataRec	m_TkyData;		// 摘要ダイアログ表示用データ

	static BYTE	m_MNTHbcd1[4];
	static BYTE	m_MNTHbcd2[4];
	static BYTE	m_MMDDbcd1[4];
	static BYTE	m_MMDDbcd2[4];

	CZmselDB* pZmSel;
	char	m_saveKana[INVKANA_MAX+1];

	BOOL	m_bIgnoreFocus;

	void set_focus( int pn );	// カーソルセット
#ifdef _ICSPEN_	// 01.20 /01
	void _set_focus( int pn );	// カーソルセット
#endif
	short get_tekmode() { return m_TEK_MODE; };
	void keyline_dsp( struct _RPL_LINE *dtline );
	void InpLine_Inz();
	void knam_set( struct _ItemData *data );
	void inpctlline_inz( struct _InpCtl *ctl );
	void set_hjiyuinz();

	struct _InpCtl*		getINP_CTL( int pn );
	struct _KeyMoveTbl* Get_KeyMoveTbl( int nowpn );
	long	term_focus( int nowpn, long nChar );

	int set_nowpn(int pn);
	int get_nowpn();
	
	void def_syselinz();
	void set_syselinz();

	void SetImage( int pn, IMREC* imp );
	void OffImage( int pn );
	void SetFocusToImage( int pn );
	int  DispTekiyoImage( int pn );

	void set_etcdsp( int pn );
	void dsp_etc( int pn, BOOL dspsw );

	void SavePrevSyz( CDBINPDataRec* pDt );
	int Set_Syzsw( struct _RPL_LINE* rpline, CDBINPDataRec* pDt);
	BOOL chk_yumenz( CDBINPDataRec* pDt );
	BOOL chk_kasitaore(CDBINPDataRec* pDt);
	int Set_Zritu( struct _RPL_LINE* rpline, CDBINPDataRec* pDt );
	int Set_Taxsgn( struct _RPL_LINE* rpline, CDBINPDataRec* pDt );
	int Set_UriSire( struct _RPL_LINE* rpline, CDBINPDataRec* pDt );
	int Set_Tokus( struct _RPL_LINE* rpline, CDBINPDataRec* pDt );
	int Set_Taika( struct _RPL_LINE* rpline, CDBINPDataRec* pDt );

	int SetPrevMenzei(CDBINPDataRec* pDt);

	int		SyuseiWarning();
	BOOL	IsInputSyuseigo( int chk_pn );
	void	GetNotInputStr(CString& tmp);
	int		KmkSyzChk();

	BOOL isScanKeyInp();
	int	TkyoIns(char* p, char* pBfo, char* pRp);

	long CheckDataIcsinputDate( int pn, long nChar );
	long CheckDataIcsinputMnthOnly( int pn, long nChar );

	long DateInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long DenpInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long BmonInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long KojiInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	void RpbmonFocusSub( int pn );
	void RpKojiFocusSub( int pn );

	BOOL ItemKanaSearch(int pn, long data);
	long ItemDataCheck(int pn, long nChar, LPCTSTR string, int dcsw );
	long ItemInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long ValInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long SnumInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	long CheckDataIcsinputTgDate( int pn, long nChar );
	long TgdateInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );
	void tky_select_dsp( CDBipTKREC *ptk, BOOL F12_KEY );

	void ClearDateSaveBuf();

	void OnSetFocusRpdate1();
	void OnKillFocusRpdate1( long inplen );
	void OnTerminationRpdate1(long nChar, long inplen, long kst);
	void OnSetFocusRpdate2();
	void OnKillFocusRpdate2( long inplen );
	void OnTerminationRpdate2(long nChar, long inplen, long kst);
	void OnSetFocusRpdenp1();
	void OnKillFocusRpdenp1( long inplen );
	void OnTerminationRpdenp1(long nChar, long inplen, long kst);
	void OnSetFocusRpdenp2();
	void OnKillFocusRpdenp2( long inplen );
	void OnTerminationRpdenp2(long nChar, long inplen, long kst);
	void OnSetFocusRpbmon1();
	void OnKillFocusRpbmon1( long inplen );
	void OnTerminationRpbmon1(long nChar, long inplen, long kst);
	void OnSetFocusRpbmon2();
	void OnKillFocusRpbmon2( long inplen );
	void OnTerminationRpbmon2(long nChar, long inplen, long kst);
	void OnSetFocusRpcbmn1();
	void OnKillFocusRpcbmn1( long inplen );
	void OnTerminationRpcbmn1(long nChar, long inplen, long kst);
	void OnSetFocusRpcbmn2();
	void OnKillFocusRpcbmn2( long inplen );
	void OnTerminationRpcbmn2(long nChar, long inplen, long kst);
	void OnSetFocusRpkoji1();
	void OnKillFocusRpkoji1( long inplen );
	void OnTerminationRpkoji1(long nChar, long inplen, long kst);
	void OnSetFocusRpkoji2();
	void OnKillFocusRpkoji2( long inplen );
	void OnTerminationRpkoji2(long nChar, long inplen, long kst);
	void OnSetFocusRpckji1();
	void OnKillFocusRpckji1( long inplen );
	void OnTerminationRpckji1(long nChar, long inplen, long kst);
	void OnSetFocusRpckji2();
	void OnKillFocusRpckji2( long inplen );
	void OnTerminationRpckji2(long nChar, long inplen, long kst);
	void OnSetFocusRpDebt1();
	void OnKillFocusRpDebt1( long inplen );
	void OnTerminationRpDebt1(long nChar, long inplen, long kst );
	void OnSetFocusRpDebt2();
	void OnKillFocusRpDebt2( long inplen );
	void OnTerminationRpDebt2(long nChar, long inplen, long kst );
	void OnSetFocusRpCred1();
	void OnKillFocusRpCred1( long inplen );
	void OnTerminationRpCred1(long nChar, long inplen, long kst );
	void OnSetFocusRpCred2();
	void OnKillFocusRpCred2( long inplen );
	void OnTerminationRpCred2(long nChar, long inplen, long kst );

	void OnSetFocusRpval1(); 
	void OnKillFocusRpval1( long inplen );
	void OnTerminationRpval1(long nChar, long inplen, long kst );
	void OnSetFocusRpval2(); 
	void OnKillFocusRpval2( long inplen );
	void OnTerminationRpval2(long nChar, long inplen, long kst );

	void OnSetFocusRptky1();
	void OnKillFocusRptky1( long inplen );
	void OnKanaTermRptky1(LPCSTR data);
	void OnTerminationRptky1(long nChar, long inplen, long kst);
	void OnSetFocusRptky2();
	void OnKillFocusRptky2( long inplen );
	void OnKanaTermRptky2(LPCSTR data);
	void OnTerminationRptky2(long nChar, long inplen, long kst);

	void OnSetFocusRpTgdate1();
	void OnKillFocusRpTgdate1( long inplen ); 
	void OnTerminationRpTgdate1(long nChar, long inplen, long kst);
	void OnSetFocusRpTgdate2();
	void OnKillFocusRpTgdate2( long inplen ); 
	void OnTerminationRpTgdate2(long nChar, long inplen, long kst); 

	void OnSetFocusRpSnum1();
	void OnKillFocusRpSnum1( long inplen );
	void OnTerminationRpSnum1(long nChar, long inplen, long kst);
	void OnSetFocusRpSnum2();
	void OnKillFocusRpSnum2( long inplen );
	void OnTerminationRpSnum2(long nChar, long inplen, long kst);

	int set_nonICSpn( tagMSG* pMsg );
	long OnTerminationRpSyzsw( long nChar);
	long OnTerminationRpSyzsw2( long nChar);
	long OnTerminationRpZritu( long nChar);
	long OnTerminationRpZritu2( long nChar);
	long OnTerminationRpSkbn( long nChar);
	long OnTerminationRpSkbn2( long nChar);
	long OnTerminationRpUrisire(long nChar);
	long OnTerminationRpUrisire2(long nChar);
	long OnTerminationRpHjiyu(long nChar);
	long OnTerminationRpHjiyu2(long nChar);
	long OnTerminationRpTokus(long nChar);
	long OnTerminationRpTokus2(long nChar);

	long OnTerminationNoDenp(long nChar);
	long OnTerminationNoBmn(long nChar);
	long OnTerminationNoCreBmn(long nChar);
	long OnTerminationNoKoji(long nChar);
	long OnTerminationNoCreKoji(long nChar);
	long OnTerminationConfirm(long nChar);
	long OnTerminationConMdfy(long nChar);
	long OnTerminationNoTky(long nChar);

	long OnTerminationChkHusen(long nChar);
	long OnTerminationScnhusen(long nChar);
	long OnTerminationScnhusen2(long nChar);
	long OnTerminationChkMnonly(long nChar);

	long OnTerminateOnOK( long nChar );

	void KanaTerminationKamoku( UINT ID, LPCTSTR kana );
	void _KanaTermTekiyo( UINT ID, LPCTSTR string, BOOL bTkcut = FALSE );

	void ZmSelSet( ZmselDBData* zd );
	void ZmSelBackFocus();

	BOOL kobetsu_datacmp( CDBDATA* data, short sign );
	BOOL kani_datacmp( CDBDATA* data, short sign );
	void last_dataget();
	void mikaku_disp( int disp );
	void KamokuSelDisp( int pn, int mode = -1 );
	long KamokuSelTermination( int pn, long nChar, long kst );
	long KamokuSelDataJob( int pn, ZmselDBData* zd );

	void OnSetFocusRpDocEvi1();
	void OnKillFocusRpDocEvi1( long inplen );
	void OnTerminationRpDocEvi1(long nChar, long inplen, long kst);
	void OnSetFocusRpDocEvi2();
	void OnKillFocusRpDocEvi2( long inplen );
	void OnTerminationRpDocEvi2(long nChar, long inplen, long kst);
	long DocEviInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	long OnTerminationNoDocEvi(long nChar);

	BOOL IsSyohizeiData( CDBDATA* dbdata );
	int	 Set_DsignCheck( struct _RPL_LINE* rpline, CDBINPDataRec* pDt );

	void BmonKojiSelDisp( int pn, LPCTSTR kana = NULL );
	long BumonKojiSelDataJob( int pn, ZmselDBData* zd );

	int	CheckImeEndBmon( UINT ID, long nChar, LPCTSTR string );
	int	CheckImeEndKoji( UINT ID, long nChar, LPCTSTR string );

	HWND GetKamokuCtrlHWND();
	int  FukuKanaSearch(MSG* pMsg, int edaonly, int possgn = 0);
	int  KanaCheckVK_BACK(MSG* pMsg, int possgn);
	long CheckDataDBeditInv(UINT ID, long nChar, LPCTSTR string);

	// インボイス対応
	void OnSetFocusRpInvno();
	void OnKillFocusRpInvno( long inplen );
	void OnTerminationRpInvno( long nChar, long inplen, long kst );

	void OnSetFocusRpInvno2();
	void OnKillFocusRpInvno2( long inplen );
	void OnTerminationRpInvno2( long nChar, long inplen, long kst );

	void DispInvno( int pn, CString invno );
	long InvnoInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst );

	long OnTerminationMenzei( long nChar );
	long OnTerminationKazei( long nChar );

	void InvNoSelDisp(int pn, LPCTSTR kana = NULL );
	void InvNoSelDataJob(int pn, ZmselDBData* zd);
	void kazeichkbox_move();

	void EnableMenWariCombo(BOOL bEnable);
	long OnTerminationMenWari(long nChar);

	void KanaTerminationInvno(UINT ID, LPCTSTR kana);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRpkeyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeScnsyzsw();
	afx_msg void OnSelchangeScnsyzsw2();
	afx_msg void OnSelchangeScnzritu();
	afx_msg void OnSelchangeScnzritu2();
	afx_msg void OnSelchangeScnskbn();
	afx_msg void OnSelchangeScnskbn2();
	afx_msg void OnSelchangeScnurisire();
	afx_msg void OnSelchangeScnurisire2();
	afx_msg void OnSelchangeScnTokus();
	afx_msg void OnSelchangeScnTokus2();
	afx_msg void OnBtnDel();
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnSetFocusRpDBedt( UINT ID );
	void OnKillFocusRpDBedt( UINT ID, long inplen );
	void OnTerminationRpDBedt( UINT ID, long nChar, long inplen, long kst );
	void OnImeEndCompositionRpDBedt( UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);
	void OnKanaTerminationRpDBedt(UINT ID, LPCTSTR Kana);

	LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );

#ifdef _ICSPEN_	// 01.20 /01
private:
//	ICSPen*	icspen;
	BOOL	flgPen;
	CRect	rectPen;

	CSyzInvoice syzInvoice;

protected:

	void set_nextpos();
	void OnICSInputFocus( int pn );

public:
//	short SetPen( BOOL flg, ICSPen* ppen );
	void SetPenRect( CRect& rect );
#endif


public:
	BOOL m_NoDenp;
	BOOL m_NoDbmn;
	BOOL m_NoCbmn;
	BOOL m_NoDkoji;
	BOOL m_NoCkoji;
	BOOL m_Confirm;
	BOOL m_ConMdfy;
	BOOL m_NoTky;
	BOOL m_ChkMnonly;

	BOOL m_MenzeiChk;
	BOOL m_KazeiChk;

	BOOL m_NoDocEvi;
	
	ImageStatic	m_image1;
	ImageStatic	m_image2;

public:
	afx_msg void OnBnClickedCheckDenp();
	afx_msg void OnBnClickedCheckDbm();
	afx_msg void OnBnClickedCheckCbm();
	afx_msg void OnBnClickedCheckDkj();
	afx_msg void OnBnClickedCheckCkj();
	afx_msg void OnCbnSelchangeScnhjiyu();
	afx_msg void OnCbnSelchangeScnhjiyu2();
public:
	afx_msg void OnBnClickedChkConfirm();
	afx_msg void OnBnClickedChkConMdfy();

	LRESULT OnImageStaticKey( WPARAM wParam, LPARAM lParam );
	LRESULT OnImageStaticFocus( WPARAM wParam, LPARAM lParam );

	BOOL m_ChkHusen;
	afx_msg void OnBnClickedChkHusen();
	afx_msg void OnCbnSelchangeScnhusen();
	afx_msg void OnCbnSelchangeScnhusen2();
	afx_msg void OnBnClickedSwkdel();
	afx_msg void OnBnClickedCheckTky();
	afx_msg void OnBnClickedCheckMnonly();
	afx_msg void OnBnClickedCheckSno();
	afx_msg void OnBnClickedChkMenzei();
	afx_msg void OnBnClickedChkKazei();
	afx_msg void OnCbnSelchangeScnMenWari();
	afx_msg void OnCbnSelchangeScnMenWari2();
};
