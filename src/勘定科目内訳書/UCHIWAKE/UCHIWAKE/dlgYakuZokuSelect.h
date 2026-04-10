#pragma once
#include "afxwin.h"


// CdlgYakuZokuSelect ダイアログ

class CdlgYakuZokuSelect : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgYakuZokuSelect)

public:
	CdlgYakuZokuSelect(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual			~CdlgYakuZokuSelect();
	BOOL			OnInitDialog();
// midori 154816 del -->
//	virtual int		ShowDialog( int type = 0/*ID_KUBUN_SELECT_YAKUSYOKU*/);
// midori 154816 del <--
// midori 154816 add -->
	virtual int		ShowDialog(int type = 0/*ID_KUBUN_SELECT_YAKUSYOKU*/, CString strKankei = "");
// midori 154816 add <--
	virtual	BOOL	PreTranslateMessage(MSG* pMsg);
			void	OnLbnDblclkListYakuzoku();

private:
	int		InitListBox();
	int		GetData();
	int		InitTitel();

private:
	int		m_nRet;						// 戻値
	int		m_nType;					// 区分タイプ(0:役職名　1:代表者との関係)
// midori 154816 add -->
	CString	m_strKankei;				// 現在入力されている「役職名」もしくは「代表者との関係」
// midori 154816 add <--
	CStringArray m_arryKubunData;		// 区分データ

public:
	CString	m_RetStr;					// 選択された区分データ(キャンセル時はNULL）

// ダイアログ データ
	enum { IDD = IDD_DIALOG_YAKU_ZOKU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listbox;
};
