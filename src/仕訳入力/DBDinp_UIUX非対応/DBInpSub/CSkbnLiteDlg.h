#pragma once

#include "resource.h"
#include "icsdbedt.h"
#include <SyzInvoice.h>


class CSyzInpLiteSub;

class CSkbnLiteDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSkbnLiteDlg)

public:
	CSkbnLiteDlg(CWnd* pParent = NULL, BOOL bToku = FALSE, BOOL bResize = TRUE, bool bInvMaster = false, 
					bool bInvUse = false, bool bInvDisp = false, int koujo8050 = SYZ_MEN_NOT );// 標準コンストラクター
	virtual ~CSkbnLiteDlg();

// ダイアログ データ
#ifndef AFX_DESIGN_TIME
	enum { IDD = IDD_SYKBNLITE_DLG };
#endif
	CICSSel		m_Select;
	CICSDBEDT	m_DBedt2;
	CICSDBEDT	m_DBedt3;
	CICSDBEDT	m_DBedtInv;
	CICSDBEDT	m_MenWari;
	CICSDBEDT	m_DBedtInvDisp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

	LRESULT	RetroFocus(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSelectedIcsselctrl(short position, long Number);
	afx_msg void OnSetFocusIcsdbedtctrl(UINT ID);
	afx_msg void OnKillFocusIcsdbedtctrl(UINT ID, long inplen);
	afx_msg void OnTerminationIcsdbedtctrl(UINT ID, long nChar, long inplen, long kst);

	int		OnTerminationDBEdt2(long nChar, long inplen, long kst);
	int		OnTerminationDBEdt3(long nChar, long inplen, long kst);
	int		OnTerminationDBEdtInv( long nChar, long inplen, long kst );
	int		OnTerminationCheckMenzei( DWORD nChar );

	void	kbn_select_make(struct _SY_MSG_TBL *sel, int pn);
	int		set_nowpn(int pn);
	int		get_nowpn(void);

	void	set_focus(int pn);
	void	DispSyzIchiran(int max, CString* str);

private:
	bool m_bInvMaster;	// インボイスバージョンアップマスター
	bool m_bInvUse;		// インボイス期間マスター
	bool m_bInvDisp;	// インボイス登録番号表示フラグ

	BOOL m_bIgnoreFocus; //フォーカス無視

	BOOL m_bMsgDsp;
	int	m_Koujo80_50;

private:
	// インボイスコントロール表示・非表示切り替え
	void ShowInvCtrl();
	// 免税事業者からの課税仕入れのチェック切り替え
	void SetMenzeiCheck();
	void	MoveInvCtrl();	// インボイス対応
public:
	CWnd*		m_pParent;
	CDBZmSub*	m_pZm;
	CDBSyohi*	m_pSy;
	CDBDATA*	m_pDBDta;

	CSyzInpLiteSub* m_Litesub;
	// インボイス対応
	CSyzInvoice	syzInvoice;
	CInvnoMng	invmng;

	CRect		m_DlgRect, m_orgRect;
	BOOL		m_Dispflg;

	BOOL	m_MODEFY_OK;
	int		m_THIS_DIALOG_OK;
	int		m_selbox_user;

	BOOL	m_bResize;

	struct _SY_MSG_PACK2* m_pSmpk;
	CString* m_pLineTxt;

	HACCEL	m_hAccelTable;
	CBrush	m_brBk, m_brDefBk;

	struct _InpCtl *getINP_CTL(int pn);
	void set_fastfocus(BOOL modefy_sg, int pn);
	BOOL skbn_dataset(int *set_pn);
	int	 SetSkbnSelNo(int no);
	void set_nowfocus();
	void set_nextpos();

	BOOL sydata_set(int *set_pn);

	void DlgRectCheck(BOOL& bVert, BOOL& bHorz);
	int GetDlgWidth();
	int GetDlgHeight();

	int  get_nextpn(int now, int vk = 0);
	int  get_prevpn(int now);
	void to_parent(CWnd *parent, long nChar);

	void SyGetMessage();
	void SyMsgJob();

	void SetInvPageCount(int count) {
		invmng.m_invPGCnt = count;
	}
	void SetInvArray(CArray<INVOICE_INFO> *ary)
	{
		invmng.SetInvArray(ary);
	}
	CString m_ErrMsg;

public:
	CButton m_MenzeiCheck;
	void	OnBnClickedMenzei();
	void	OnSetfocusCheckMenzei();

	void MenWariDBEdit(BOOL bEnable);
	void MenWariSetDBedtData();
	int  OnTerminationDBEdtMen(long nChar, long inplen, long kst);

	void MenCheckButtonJob(long nChar);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void ImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);
	long CheckDataDBeditInv(long nChar, LPCTSTR string);
	LRESULT OnMyMessage(WPARAM wp, LPARAM lp);

};
