// CDiagException.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "CDiagException.h"
#include "afxdialogex.h"


//特例適用期間
//#define SIIRETOKUREI_EDAY 20200930
//#define URIAGETOKUREI_EDAY 20230930


// CDiagException ダイアログ

IMPLEMENT_DYNAMIC(CDiagException, ICSDialog)

CDiagException::CDiagException(CWnd* pParent)
	: ICSDialog(IDD_DIALOG_EXCEPTION_CAL, pParent)
	, m_pSnHeadData(NULL)
	, m_symd(0)
{
	
}

CDiagException::~CDiagException()
{
}

void CDiagException::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_URI1, m_checkuri1);
	DDX_Control(pDX, IDC_CHECK_URI2, m_checkuri2);
	DDX_Control(pDX, IDC_CHECK_SII, m_checksii1);
	DDX_Control(pDX, IDC_STATIC_URI1, m_sturi1);
	DDX_Control(pDX, IDC_STATIC_URI2, m_sturi2);
	DDX_Control(pDX, IDC_STATIC_SII1, m_stsii1);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_STATIC_EMSG1, m_emsg1);
	DDX_Control(pDX, IDC_STATIC_EMSG2, m_emsg2);
	DDX_Control(pDX, IDC_STATIC_WMSG1, m_wmsg1);
	DDX_Control(pDX, IDC_CHECK_HIRENDO, m_HChk);
	DDX_Control(pDX, IDC_STATIC_HIRENDO, m_HSt);
	DDX_Control(pDX, IDC_STATIC_WMSG2, m_wmsg2);
}


BEGIN_MESSAGE_MAP(CDiagException, ICSDialog)
	ON_BN_CLICKED(IDC_CHECK_URI1, &CDiagException::OnBnClickedCheckUri1)
	ON_BN_CLICKED(IDC_CHECK_URI2, &CDiagException::OnBnClickedCheckUri2)
	ON_BN_CLICKED(IDC_CHECK_SII, &CDiagException::OnBnClickedCheckSii)
	ON_BN_CLICKED(IDOK, &CDiagException::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDiagException::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDiagException メッセージ ハンドラー
BOOL CDiagException::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	Dlg_SetCtl();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//void CDiagException::InitInfo(CSnHeadData *pSnHeadData, long symd)
void CDiagException::InitInfo(CSnHeadData *pSnHeadData, long symd, long eymd)
{
	m_pSnHeadData = pSnHeadData;
	m_symd = symd;
	m_eymd = eymd;
}


void CDiagException::Dlg_SetCtl()
{
	m_emsg1.ShowWindow(SW_HIDE);
	m_emsg2.ShowWindow(SW_HIDE);

	if (m_symd > SIIRETOKUREI_EDAY) {
		m_pSnHeadData->m_DispTabSgn &= ~0x40;
	}

	if (m_symd > URIAGETOKUREI_EDAY) {		// 20231025 9月30日をまたぐ間は適用可能なため、開始日が超えているかどうかで判定する
	//if (m_eymd > URIAGETOKUREI_EDAY) {	// 20231025 del
		m_pSnHeadData->m_DispTabSgn &= ~0x70;
	}


	if(m_pSnHeadData->m_DispTabSgn & 0x10)	m_checkuri1.SetCheck(1);
	if(m_pSnHeadData->m_DispTabSgn & 0x20)	m_checkuri2.SetCheck(1);
	if(m_pSnHeadData->m_DispTabSgn & 0x40)	m_checksii1.SetCheck(1);
	if(m_pSnHeadData->m_DispTabSgn & 0x100)	m_HChk.SetCheck(1);

	if ((m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01) {	// 簡易課税
		m_checkuri2.EnableWindow(FALSE);
		m_checksii1.EnableWindow(FALSE);
		m_sturi2.EnableWindow(FALSE);
		m_stsii1.EnableWindow(FALSE);


		m_wmsg1.SetWindowTextA("※簡易課税では、第５－(２)号様式、第５－(３)号様式を使用できません。");

	}
	else {

		//--------------->yokono 2019.9.10.-2019.9.11.
		//「簡易課税」でなく、「仕入税額の按分法が個別対応」かつ「連動」の場合
		if ((m_pSnHeadData->m_s_sgn4 & 0x80) && !(m_pSnHeadData->Sn_Sign4 & 0x80)) {
			//第５－（３）様式のチェックボックスを無効化
			m_checksii1.SetCheck(0);//チェックOFFに
			m_pSnHeadData->m_DispTabSgn &= ~0x40;
			m_checksii1.EnableWindow(FALSE);
			m_stsii1.EnableWindow(FALSE);

			///第５－（１）様式にチェックが入っているとき、第５－（２）様式を無効化
			if (m_pSnHeadData->m_DispTabSgn & 0x10) {
				m_checkuri2.EnableWindow(FALSE);
				m_sturi2.EnableWindow(FALSE);
			}
			///第５－（２）様式にチェックが入っているとき、第５－（１）様式を無効化
			else if (m_pSnHeadData->m_DispTabSgn & 0x20) {
				m_checkuri1.EnableWindow(FALSE);
				m_sturi1.EnableWindow(FALSE);
			}

			m_wmsg2.SetWindowTextA("※第５－(３)号様式は、個別対応方式の集計に非対応のため選択できません。");
		}
		//「簡易課税」でなく、「仕入税額の按分法が比例配分」または「仕入税額の按分法が個別対応かつ、非連動」の場合
		else {
		//<--------------2019.9.10.-2019.9.11
			if ((m_pSnHeadData->m_DispTabSgn & 0x10) || (m_pSnHeadData->m_DispTabSgn & 0x40)) {
				m_checkuri2.EnableWindow(FALSE);
				m_sturi2.EnableWindow(FALSE);
			}
			else if (m_pSnHeadData->m_DispTabSgn & 0x20) {
				m_checkuri1.EnableWindow(FALSE);
				m_checksii1.EnableWindow(FALSE);
				m_sturi1.EnableWindow(FALSE);
				m_stsii1.EnableWindow(FALSE);
			}
		}

		m_wmsg1.SetWindowTextA("※第５－(２)号様式は、第５－(１)号様式または第５－(３)号様式と併用できません。");

	}


	//課税期間末日を確認。特例適用対象外日時の場合には選択不可とする
	//if (m_pSnHeadData->Sn_KDAYE > SIIRETOKUREI_EDAY) {
	if (m_symd > SIIRETOKUREI_EDAY) {
		if ( ( m_pSnHeadData->m_s_sgn2 & 0x0f ) > 0x01 ) {	// 簡易課税
			m_emsg1.ShowWindow ( SW_HIDE );
		}
		else {
			m_emsg1.ShowWindow(SW_SHOW);
		}
		m_checksii1.EnableWindow(FALSE);
		m_stsii1.EnableWindow(FALSE);
		//m_pSnHeadData->m_DispTabSgn &= ~0x40;
		m_checksii1.SetCheck(0);
	}

	//if (m_pSnHeadData->Sn_KDAYE > URIAGETOKUREI_EDAY) {
	if (m_symd > URIAGETOKUREI_EDAY) {		// 20231025 9月30日をまたぐ間は適用可能なため、開始日が超えているかどうかで判定する
	//if (m_eymd > URIAGETOKUREI_EDAY) {	// 20231025 del
		m_emsg2.ShowWindow(SW_SHOW);
		
		m_checkuri1.EnableWindow(FALSE);
		m_checkuri2.EnableWindow(FALSE);
		m_checksii1.EnableWindow(FALSE);

		m_stsii1.EnableWindow(FALSE);
		m_sturi2.EnableWindow(FALSE);
		m_stsii1.EnableWindow(FALSE);

		//m_pSnHeadData->m_DispTabSgn &= ~0x70;
		m_checkuri1.SetCheck(0);
		m_checkuri2.SetCheck(0);
		m_checksii1.SetCheck(0);
	}

	// 非連動時には非連動処理のチェックは選択不可能
	if ( m_pSnHeadData->Sn_Sign4 & 0x80 ) {
		m_HChk.EnableWindow ( FALSE );
		m_HSt.EnableWindow( FALSE );
	}
	else {
		m_HChk.EnableWindow ( TRUE );
		m_HSt.EnableWindow( TRUE );
	}

	Dlg_CheckCtl();

}

void CDiagException::Dlg_GetCtl()
{
	if (m_checkuri1.GetCheck()) {
		m_pSnHeadData->m_DispTabSgn |= 0x10;
	}
	else {
		m_pSnHeadData->m_DispTabSgn &= ~0x10;
	}

	if (m_checkuri2.GetCheck()) {
		m_pSnHeadData->m_DispTabSgn |= 0x20;
	}
	else {
		m_pSnHeadData->m_DispTabSgn &= ~0x20;
	}

	if (m_checksii1.GetCheck()) {
		m_pSnHeadData->m_DispTabSgn |= 0x40;
	}
	else {
		m_pSnHeadData->m_DispTabSgn &= ~0x40;
	}

	if ( m_HChk.GetCheck() ) {
		m_pSnHeadData->m_DispTabSgn |= 0x100;
	}
	else {
		m_pSnHeadData->m_DispTabSgn &= ~0x100;
	}
}


void CDiagException::Dlg_CheckCtl()
{
	if ( (m_checkuri1.GetCheck() == 0) && (m_checkuri2.GetCheck() == 0) && (m_checksii1.GetCheck() == 0) ) {
		m_ok.EnableWindow(FALSE);
	}
	else	{
		m_ok.EnableWindow(TRUE);
	} 

}

void CDiagException::OnBnClickedCheckUri1()
{
	if ((m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01) {	// 簡易課税

	}
	else {
		if (m_checkuri1.GetCheck()) {
			m_checkuri2.EnableWindow(FALSE);
			m_sturi2.EnableWindow(FALSE);
		}
		else {
			if ( !( m_checksii1.GetCheck() ) ) {
				m_checkuri2.EnableWindow(TRUE);
				m_sturi2.EnableWindow(TRUE);
			}
		}
	}
	Dlg_CheckCtl();
}


void CDiagException::OnBnClickedCheckUri2()
{
	if (m_checkuri2.GetCheck()) {
		m_checksii1.SetCheck(0);//チェックOFFに
		m_checkuri1.EnableWindow(FALSE);
		m_sturi1.EnableWindow(FALSE);
		m_checksii1.EnableWindow(FALSE);
		m_stsii1.EnableWindow(FALSE);
	}
	else {
		m_checkuri1.EnableWindow(TRUE);
		m_sturi1.EnableWindow(TRUE);
		//------------>yokono 2019.9.10.-2019.9.11.
		//「仕入税額の按分法が個別対応」かつ「連動」なら、５－（２）のチェックを外しても５－（３）は無効
		if ((m_pSnHeadData->m_s_sgn4 & 0x80) && !(m_pSnHeadData->Sn_Sign4 & 0x80)) {
			m_checksii1.EnableWindow(FALSE);
			m_stsii1.EnableWindow(FALSE);
		}
		//<--------------2019.9.10.-2019.9.11.
		//else if (m_pSnHeadData->Sn_KDAYE > SIIRETOKUREI_EDAY) {
		else if (m_symd > SIIRETOKUREI_EDAY) {
			m_checksii1.EnableWindow(FALSE);
			m_stsii1.EnableWindow(FALSE);
		}
		else {
			m_checksii1.EnableWindow(TRUE);
			m_stsii1.EnableWindow(TRUE);
		}
	}

	Dlg_CheckCtl();
}


void CDiagException::OnBnClickedCheckSii()
{
	if ( m_checksii1.GetCheck() ) {
		m_checkuri2.SetCheck ( 0 );
		m_checkuri2.EnableWindow ( FALSE );
		m_sturi2.EnableWindow ( FALSE );
	}
	else {
		if ( !( m_checkuri1.GetCheck() ) ) {
			m_checkuri2.EnableWindow ( TRUE );
			m_sturi2.EnableWindow ( TRUE );
		}
	}
	Dlg_CheckCtl();
}


void CDiagException::OnBnClickedOk()
{
	Dlg_GetCtl();
	OnOK();
}


void CDiagException::OnBnClickedCancel()
{
	OnCancel();
}





HBRUSH CDiagException::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。

	switch (pWnd->GetDlgCtrlID()) {
		//警告・注意メッセージを赤字で表示する
		case IDC_STATIC_EMSG1://特例禁止メッセージ（売上）
		case IDC_STATIC_EMSG2://特例禁止メッセージ（仕入）
		case IDC_STATIC_WMSG1://警告メッセージ（画面下部。）
		case IDC_STATIC_WMSG2://警告メッセージ（画面下部。）
			pDC->SetTextColor(RGB(0xff, 0, 0));
			break;
	}


	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}