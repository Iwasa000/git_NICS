// TISW修正[22-0676] 2026/03/19 START
#include "stdafx.h"
#include "SkjUserSettings.h"

IMPLEMENT_DYNAMIC(SkjUserSelectDlg, ICSDialog)

SkjUserSelectDlg::SkjUserSelectDlg(CWnd* pParent /*=NULL*/) : ICSDialog(SkjUserSelectDlg::IDD, pParent)
{
}

SkjUserSelectDlg::SkjUserSelectDlg( UINT id, CWnd* pParent /*=NULL*/) : ICSDialog(id, pParent)
{
}

/// <summary>
/// 表示前の初期化処理
/// </summary>
/// <param name="initParam">初期化パラメータ</param>
void SkjUserSelectDlg::Initialize(InitialParam&& initParam)
{
	m_initParam = std::move(initParam);
}

SkjUserSelectDlg::~SkjUserSelectDlg()
{
}

void SkjUserSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_USER_COMBO, m_userCombo);
}


BEGIN_MESSAGE_MAP(SkjUserSelectDlg, ICSDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TEMP_APPLY, OnTempApplyClicked)
END_MESSAGE_MAP()


void SkjUserSelectDlg::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);
}

BOOL SkjUserSelectDlg::OnInitDialog()
{
	BOOL	bSt	=	ICSDialog::OnInitDialog();
	
	// ユーザー名のセット
	bool existsDefSettingUser = false;
	for (auto& user : m_initParam.m_users)
	{
		CString name = user.real_name.IsEmpty() ? user.user_name : user.real_name;
		if (m_initParam.m_defSettingUsers.find(user.user_id) == m_initParam.m_defSettingUsers.end())
		{
			m_userCombo.AddString(name);
		}
		else
		{
			// デフォルト設定ユーザー
			m_userCombo.AddString(_T("※") + name);
			existsDefSettingUser = true;
		}
	}
	m_userCombo.SetCurSel(0);

	// 初期設定ラベルの表示
	if (existsDefSettingUser)
	{
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
	}
	
	ICSDialog::OnInitDialogEX();
	return	bSt;
}

HBRUSH SkjUserSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return	ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

/// <summary>選択ユーザーIDを取得する</summary>
int SkjUserSelectDlg::GetSelectedUserId()
{
	if (m_selectedIndex < 0)
	{
		return -1;
	}

	return m_initParam.m_users[m_selectedIndex].user_id;
}

/// <summary>確定ボタンのクリック</summary>
void SkjUserSelectDlg::OnOK()
{
	// 続行確認メッセージ
	CString message =
		_T("選択した利用者の設定で確定します。\n")
		_T("他の利用者の設定は削除され、元には戻せません。\n")
		_T("続行してもよろしいですか？");
	if (ICSMessageBox(message, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
	{
		return;
	}

	m_selectedIndex = m_userCombo.GetCurSel();
	__super::OnOK();
}

/// <summary>一時適用ボタンのクリック</summary>
void SkjUserSelectDlg::OnTempApplyClicked()
{
	m_selectedIndex = m_userCombo.GetCurSel();
	EndDialog(RES_TEMP_APPLY);
}

// TISW修正[22-0676] 2026/03/19 END