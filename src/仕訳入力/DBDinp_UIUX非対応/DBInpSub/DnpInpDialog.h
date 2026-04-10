#pragma once

#include "SelGuideCtrl.h"
//#include "DnpBaseClass.h"
#include "DlgToolBar.h"

// CDnpInpDialog ダイアログ

class CDnpBaseClass;

class CDnpInpDialog : public ICSDialog
{
	DECLARE_DYNAMIC(CDnpInpDialog)

public:
	CDnpInpDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDnpInpDialog();

	CICSSel	m_selfix8;
	CICSSel	m_selval32;
	CSelGuideCtrl m_selGuid;

	CDnpBaseClass*	m_base;
	DENP_INPUTPAR*	m_pInpPar;

	CDlgToolBar	m_wndToolBar;
	CToolBarCtrl& TBarCtrl() { return m_wndToolBar.GetToolBarCtrl(); };
	//モードレスタイプ呼び出し回数
	int modelessDomodal;
	int defWidth, defHeight;
	int show_flg; //20230307
	void Set_show_flg(int setF);

// ダイアログ データ
	enum { IDD = IDD_DLG_DNP };
//	enum { IDD = 	IDD_TKYV2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	BOOL CreateDlg();
	void SetInputPar(DENP_INPUTPAR* para);
	int  ModelessDoModal();
	void EndDialogFunc(int nResult);
	void MoveDenpDialog();

	void ToolBarUpdate();

	LRESULT OnRetroFocus( WPARAM wParam, LPARAM lParam );
	LRESULT OnRetroFocus2( WPARAM wParam, LPARAM lParam );
	LRESULT SetSkbnLineDisp( WPARAM wParam, LPARAM lParam );
	LRESULT OnSkbnEnd( WPARAM wParam, LPARAM lParam );
	LRESULT DspSkbnIchiran( WPARAM wParam, LPARAM lParam );

	LRESULT OnHjiyuInEnd( WPARAM w, LPARAM l );
	LRESULT OnHjiyuSel( WPARAM w, LPARAM l );
	LRESULT OnHusenInpMsg( WPARAM w, LPARAM l );
	LRESULT OnSelGuideMsg(WPARAM w, LPARAM l );

	void	OnCommandTlbar( UINT id );
	LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);

	void	OnButtonEnd();
	BOOL	MouseHookRbuttonJob(MOUSEHOOKSTRUCT *pmh);
	void	OnTimer(UINT_PTR nIDEvent);

public:
	DECLARE_EVENTSINK_MAP()
public:
	void SelectedIcsselctrl1(short position, long Number);
	void SelectedIcsselctrl2(short position, long Number);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

