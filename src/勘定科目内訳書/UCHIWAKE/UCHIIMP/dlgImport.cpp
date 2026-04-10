//--------------------------------------------------
//	dlgImport.cpp
//
//	データインポート／エクスポートダイアログ
//
//	2009.06.17～
//--------------------------------------------------

#include "stdafx.h"
#include "resource.h"       // メイン シンボル
#include "dlgImport.h"
// midori 160604 add -->
#include "SortStringArray.h"
// midori 152395 del -->
//#define	AS_SPLIT_MAX	256
// midori 152395 del <--
// midori 160604 add <--
// midori 152395 add -->
#define	AS_SPLIT_MAX	2048
// midori 152395 add <--

// CdlgImport ダイアログ

IMPLEMENT_DYNAMIC(CdlgImport, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgImport::CdlgImport(CWnd* pParent /*=NULL*/)
	: ICSDialog(CdlgImport::IDD, pParent)
	, m_fImport(FALSE)
	, m_strFolderPath(_T(""))
	, m_nRet(ID_DLG_CANCEL)
{

}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgImport::~CdlgImport()
{
}

void CdlgImport::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_stcMessage);
	DDX_Control(pDX, IDC_STATIC_FOLDER, m_stcFolder);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_edtFolder);
	DDX_Control(pDX, IDC_BUTTON_REFERENCE, m_btnReference);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CdlgImport, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CdlgImport::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CdlgImport::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_REFERENCE, &CdlgImport::OnBnClickedButtonReference)
END_MESSAGE_MAP()


// CdlgImport メッセージ ハンドラ

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgImport::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	//--------------------------------------------------
	// 表示文言設定
	//--------------------------------------------------
	CString		strTitle		= _T("");		// タイトル
	CString		strMsg			= _T("");		// メッセージ

	// 文字列生成
	if (m_fImport == TRUE) {
		// インポート
		strTitle	= IMP_DLGMSG_IMPORT_TITLE;
//		strMsg.Format(_T("%s\r\n\r\n%s"), IMP_DLGMSG_IMPORT_EXECUTE, IMP_DLGMSG_IMPORT_ATTENTION);
		strMsg.Format(_T("%s\r\n\r\n%s"), IMP_DLGMSG_IMPORT_EXECUTE, IMP_DLGMSG_IMPORT_SET);
	}
	else {
		// エクスポート
		strTitle	= IMP_DLGMSG_EXPORT_TITLE;
//		strMsg.Format(_T("%s"), IMP_DLGMSG_EXPORT_EXECUTE);
		strMsg.Format(_T("%s\r\n\r\n%s"), IMP_DLGMSG_EXPORT_EXECUTE, IMP_DLGMSG_EXPORT_SET);		// [2009/07/09 Add Attention Message]
	}

	// 文字列→ダイアログにセット
	SetWindowText(strTitle);										// タイトル
	m_stcMessage.UpdateCaption(strMsg, NULL);						// メッセージ		メッセージの一部をオプションダイアログへ移動 20111007
	SetDlgItemText(m_edtFolder.GetDlgCtrlID(), m_strFolderPath);	// フォルダパス

	// テキストボックス内の文字列をすべて選択
	m_edtFolder.SetSel(0, -1);

	//--------------------------------------------------
	// リサイズ
	//--------------------------------------------------
	OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgImport::PreTranslateMessage(MSG* pMsg)
{
	// ダイアログにフォーカスがない場合は処理しない
	// （他のウィンドウに移った時など）
	if (GetFocus() != NULL) {
		// テキストボックスにフォーカスが当たっているときのみチェック
		// ※他のコントロールはICSモジュールを使用しているため、
		// 　下記の機能は実装されている
		if (GetFocus()->GetDlgCtrlID() == m_edtFolder.GetDlgCtrlID()) {
			// キーが押された
			if (pMsg->message == WM_KEYDOWN) {
				switch (pMsg->wParam) {
				case VK_RETURN:		// Enterキー
					if (GetKeyState(VK_SHIFT) & 0x8000) {
						PrevDlgCtrl();
					}
					else {
						NextDlgCtrl();
					}
					return TRUE;
				case VK_F2:			// F2キー
					PrevDlgCtrl();
					return TRUE;
				}
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// midori 160604 add -->
//**************************************************
//	文字列の先頭から指定位置まで取得（２５６バイトまで対応）
//	【引数】	szSrc		…	文字列（元）
//				szDst		…	文字列（先）
//				nPos		…	分割位置までのバイト数
//	【戻値】	指定位置以降の文字列を返す
//**************************************************
CString CdlgImport::GetSpritString( CString szSrc, CString* szDst, int nPos )
{
	CString					szRet;					//	戻値
	CString					cs;						//	戻値
// midori 152395 del -->
//// midori 152374 cor -->
////	char					szBuf[AS_SPLIT_MAX];	//	バッファ
////	char					szBuf1[AS_SPLIT_MAX];	//	バッファ
////	char					szBuf2[AS_SPLIT_MAX];	//	バッファ
//// ---------------------
//	char					szBuf[2048];	//	バッファ
//	char					szBuf1[2048];	//	バッファ
//	char					szBuf2[2048];	//	バッファ
//// midori 152374 cor <--
// midori 152395 del <--
// midori 152395 add -->
	char					szBuf[AS_SPLIT_MAX];	//	バッファ
	char					szBuf1[AS_SPLIT_MAX];	//	バッファ
	char					szBuf2[AS_SPLIT_MAX];	//	バッファ
// midori 152395 add <--
	const unsigned char*	pBuf = 0;				//	バッファ
	int						nLen;					//	文字列長
	int						nSplitPos;				//	分割位置
	
	//	初期化
	ZeroMemory( szBuf, sizeof( szBuf ) );
	ZeroMemory( szBuf1, sizeof( szBuf1 ) );
	ZeroMemory( szBuf2, sizeof( szBuf2 ) );
	szDst->Empty();
	szRet.Empty(); 

	//	文字列（元）長取得
	nLen = szSrc.GetLength();
	
	//	文字列（元）長か分割位置が最大文字数を超えている？
	if (( nLen > AS_SPLIT_MAX ) ||
		( nPos > AS_SPLIT_MAX )){
			//	抜ける
			return( szRet );
	}

	//	文字列長が分割位置に満たない？
	if ( nLen <= nPos ){
		//	文字列（元）をそのまま取得
		*szDst = szSrc;
	}
	else{
		//	分割位置を取得
		nSplitPos = nPos;
		
		//	分割位置までの文字列を取得
		strcpy_s( szBuf, sizeof( szBuf ), szSrc );

		if(_mbsbtype((LPBYTE)szBuf,nSplitPos) == _MBC_TRAIL)	nSplitPos--;

		memcpy(szBuf1,szBuf,nSplitPos);
		if((int)strlen(szBuf) > nSplitPos)	{
			memcpy(szBuf2,&szBuf[nSplitPos],strlen(szBuf) - nSplitPos);
		}

		////	文字列のポインタを取得
		//pBuf = (const unsigned char *)szBuf;
		//
		////	分割位置のバイトがマルチバイトの先頭バイト？
		//if ( _ismbslead( pBuf, &pBuf[nSplitPos - 1]  ) != 0 ){
		//	//	分割位置を１バイト前に移動
		//	nSplitPos--;
		//}

		////	文字列の先頭から指定位置まで取得
		//*szDst = szSrc.Left( nSplitPos );
		////	指定位置以降の文字列を取得
		//szRet = szSrc.Mid( nSplitPos );

		//	文字列の先頭から指定位置まで取得
		cs.Format(_T("%s"),szBuf1);
		*szDst = cs;
		//	指定位置以降の文字列を取得
		szRet.Format(_T("%s"),szBuf2);
	}

	//	戻値を返す
	return( szRet );
}
// midori 160604 add <--

//++++++++++++++++++++++++++++++++++++++++++++++++++
// [OK]ボタン クリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgImport::OnBnClickedButtonOk()
{
// midori 160603 add -->
	int			rv;
	CFileFind	FileFind;
// midori 160603 add <--
// midori 160604 add -->
	int			ii=0;
	int			cnt1=0;
	int			cnt2=0;
	CString		cst1=_T("");
	CString		cst2=_T("");
	CString		cerr=_T("");
	CSortStringArray	csta;
// midori 160604 add <--

	// フォルダパスの取得
	GetDlgItemText(m_edtFolder.GetDlgCtrlID(), m_strFolderPath);

// midori 160604 del -->
//	// 戻り値を指定
//	m_nRet = ID_DLG_OK;
// midori 160604 del <--

// midori 160603 add -->
	// 指定したフォルダがあるかチェック
	rv = FileFind.FindFile(m_strFolderPath);
	FileFind.Close();
	if( rv == 0 )	{	// 指定したフォルダが存在しません。
		::ICSMessageBox( IMP_ERRMSG_FINDFOLDER, 0, 0, 0, this );
		return;
	}
// midori 160603 add <--

// midori 160604 cor -->
	//// 指定フォルダにCSVファイルがあるかチェック
	//CCsv		Csv;
	//int			cntFalse = 0;
	//CString		strPath;	strPath.Empty();

	//if( m_fImport != FALSE ){
	//	for(int i=0; i<SHEET_NUM; i++){
	//		strPath = m_strFolderPath + _T("\\") + UcCsvFile[i];
	//		if( Csv.CsvOpen( strPath ) != FALSE ){
	//			Csv.Close();
	//			continue;
	//		}
	//		cntFalse++;
	//	}
	//	if( cntFalse == SHEET_NUM ){	// 指定フォルダにCSVファイルが一つもない
	//		::ICSMessageBox( IMP_ERRMSG_FINDFILE, 0, 0, 0, this );
	//		return;
	//	}
	//}
// ---------------------
	if(m_fImport != FALSE)	{
		// 指定フォルダにCSVファイルがあるかチェック
		// err-1 : ファイルが１件も存在しない
		// err-2 : 名称が規定以外のファイルのみ存在する
		// err-3 : 名称が規定のファイルと規定以外のファイルが存在する
		cnt1=0;		// 名称が規定以外のファイル件数
		cnt2=0;		// 名称が規定のファイル件数
		csta.RemoveAll();
		rv = FileFind.FindFile(m_strFolderPath + _T("\\*"));
		while(rv)	{
			rv = FileFind.FindNextFile();
			if(FileFind.IsDirectory() == TRUE)	continue;
			if(FileFind.IsDots() == TRUE)		continue;
// midori 152374 add -->
			// Thumbs.dbが表示されないようにする
			if(FileFind.IsHidden() == TRUE)		continue;
// midori 152374 add <--
			cst1 = FileFind.GetFileName();
			for(ii=0; ii<SHEET_NUM; ii++)	{
				cst2.Format(_T("%s"),UcCsvFile[ii]);
				if(cst1 == cst2)	break;
			}
			if(ii >= SHEET_NUM)	{
// midori 152374 cor -->
//				GetSpritString(cst1,&cst2,100); 
// ---------------------
				GetSpritString(cst1,&cst2,50); 
// midori 152374 cor <--
				csta.Add(cst2);
				cnt1++;
			}
			else	{
				cnt2++;
			}
		}
		if(cnt1 == 0 && cnt2 == 0)	{
			// err-1 : ファイルが１件も存在しない
			::ICSMessageBox(IMP_ERRMSG_FINDFILE,0,0,0,this);
			return;
		}
		else if(cnt1 != 0)	{
			// err-2 : 名称が規定以外のファイルのみ存在する
			// err-3 : 名称が規定のファイルと規定以外のファイルが存在する
			// ファイル名をソート
			csta.Sort();
			// 
			cerr = _T("CSVファイルのファイル名が正しくありません。");
			cerr += _T("\r\n\r\n");
// 158023 del -->
			//if(cnt1 > SHEET_NUM)	cnt1 = SHEET_NUM;
			//for(ii=0; ii<cnt1; ii++)	{
			//	cerr += _T("　");
			//	cerr += csta[ii];
			//	cerr += _T("\r\n");
			//}
// 158023 del <--
// 158023 add -->
			int	sw=0;
			if(cnt1 > 20)	{
				cnt1=20;
				sw=1;
			}
			for(ii=0; ii<cnt1; ii++)	{
				cerr += _T("　");
				cerr += csta[ii];
				if((ii == (cnt1 - 1)) && sw != 0)	{
					cerr += _T("　･･･");
				}
				cerr += _T("\r\n");
			}
// 158023 add <--
			cerr += _T("\r\n");
			if(cnt2 != 0)	{
				cerr += _T("上記のファイルをスキップし、データインポート処理を継続しますか？");
				if(::ICSMessageBox(cerr,MB_YESNO,0,0,this) == IDNO)	return;
			}
			else	{
				cerr += _T("すべてのファイルが取り込めません。処理を中断します。");
				::ICSMessageBox(cerr,0,0,0,this);
				return;
			}
		}
	}
// midori 160604 cor <--

// midori 160604 add -->
	// 戻り値を指定
	m_nRet = ID_DLG_OK;
// midori 160604 add <--

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// [キャンセル]ボタン クリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgImport::OnBnClickedButtonCancel()
{
	// 戻り値を指定
	m_nRet = ID_DLG_CANCEL;

	ICSDialog::OnCancel();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// [参照]ボタン クリック
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgImport::OnBnClickedButtonReference()
{
	// ダイアログに表示するメッセージ
	CString		strMsg		= _T("");			// メッセージ
	CString		strFolderPath	= _T("");		// フォルダパス

	// 文字列生成
	if (m_fImport == TRUE) {
		// インポート
		strMsg.Format(_T("%s"), IMP_DLGMSG_IMPORT_SELECTFOLDER);
	}
	else {
		// エクスポート
		strMsg.Format(_T("%s"), IMP_DLGMSG_EXPORT_SELECTFOLDER);
	}

	// 画面からフォルダパスを取得
	GetDlgItemText(m_edtFolder.GetDlgCtrlID(), strFolderPath);

	// [フォルダの参照]ダイアログの表示
//	if (GetFolderPathFromBrowser(strMsg, &strFolderPath) == FUNCTION_OK) {
// 210409 del -->
	//if (GetFolderPathFromBrowser(strMsg, &strFolderPath, m_fImport) == FUNCTION_OK) {		// [2009/07/09 Add Import/Export judgment]
// 210409 del <--
// 210409 add -->
	CString		errMsg=_T("");
	if(GetFolderPathFromBrowser(strMsg,&strFolderPath,m_fImport,&errMsg) == FUNCTION_OK) {		// [2009/07/09 Add Import/Export judgment]
// 210409 add <--
		// 取得したフォルダパスを画面に表示
		SetDlgItemText(m_edtFolder.GetDlgCtrlID(), strFolderPath);
	}
	else {
		// エラーメッセージ
// 210409 del -->
		//ICSMessageBox(IMP_DLGMSG_ERROR_SELECTFOLDER, (MB_ICONWARNING | MB_OK), 0, 0, this);
// 210409 del <--
// 210409 add -->
		CString	err=_T("");
		if(errMsg.IsEmpty() != TRUE)	err=errMsg;
		else							err=IMP_DLGMSG_ERROR_SELECTFOLDER;
		ICSMessageBox(err,(MB_ICONWARNING | MB_OK),0,0,this);
// 210409 add <--
	}
}

//**************************************************
//	データインポート／エクスポートダイアログの表示
//	【引数】	fImport			…	[IN] 検索対象
//										TRUE ：インポート元フォルダ
//										FALSE：エクスポート先フォルダ
//				pstrFolderPath	…	[IN, OUT]フォルダパス
//	【戻値】	ID_DLG_OK		…	[OK]ボタンで終了
//				ID_DLG_CANCEL	…	[キャンセル]ボタンで終了
//**************************************************
int CdlgImport::ShowDialog(BOOL fImport, CString* pstrFolderPath)
{
	// 引数→メンバ変数
	m_fImport			= fImport;
	m_strFolderPath		= *pstrFolderPath;

	// モーダルでダイアログを表示
	DoModal();

	// [OK]ボタンが押されたらフォルダパスを返す
	if (m_nRet == ID_DLG_OK) {
		// 呼び出し元に返す
		*pstrFolderPath = m_strFolderPath;
	}

	return m_nRet;
}

// 210409 del -->
////**************************************************
////	[フォルダの参照]ダイアログにてフォルダパスを取得
////	【引数】	strMsg			…	[IN] ダイアログに表示するメッセージ
////				pstrFolderPath	…	[IN, OUT]フォルダパス
////				fImport			…	[IN] インポート/エクスポート判断フラグ
////										ID_IMPORT (TRUE) ：インポート
////										ID_EXPORT (FALSE)：エクスポート
////	【戻値】	FUNCTION_OK		…	成功
////				FUNCTION_NG		…	失敗
////**************************************************
//int CdlgImport::GetFolderPathFromBrowser(CString strMsg, CString* pstrFolderPath, BOOL fImport)
// 210409 del <--
// 210409 add -->
//**************************************************
//	[フォルダの参照]ダイアログにてフォルダパスを取得
//	【引数】	strMsg			…	[IN] ダイアログに表示するメッセージ
//				pstrFolderPath	…	[IN, OUT]フォルダパス
//				fImport			…	[IN] インポート/エクスポート判断フラグ
//										ID_IMPORT (TRUE) ：インポート
//										ID_EXPORT (FALSE)：エクスポート
//				pstrErrMsg		…	[OUT]SelectDirectory関数が失敗したときのメッセージ
//	【戻値】	FUNCTION_OK		…	成功
//				FUNCTION_NG		…	失敗
//**************************************************
int CdlgImport::GetFolderPathFromBrowser(CString strMsg, CString* pstrFolderPath, BOOL fImport, CString* pstrErrMsg)
// 210409 add <--
{
	LPMALLOC		pMalloc;		// IMallocインターフェース

	// Shellアロケータ（IMalloc）の取得 … Shellのメモリ管理オブジェクト
	if (SHGetMalloc(&pMalloc) != NOERROR) {
		// Shellアロケータ（IMalloc）の取得に失敗
		return FUNCTION_NG;
	}

	BROWSEINFO		uBrowseInfo;				// [フォルダの参照]ダイアログの情報
	ITEMIDLIST*		lpuItemIdList;				// IDリスト
	char			szPathBuf[MAX_PATH];		// フォルダパスバッファ

	// 初期化
	ZeroMemory(&uBrowseInfo, sizeof(uBrowseInfo));
	ZeroMemory(&szPathBuf, sizeof(szPathBuf));

	// ダイアログ情報の設定
	uBrowseInfo.hwndOwner		= m_hWnd;								// 親ウインドウのハンドル 
	uBrowseInfo.lpszTitle		= strMsg;								// ダイアログに表示するメッセージ
	uBrowseInfo.ulFlags			= BIF_RETURNONLYFSDIRS					// 実ディレクトリのみ選択可能
								| BIF_USENEWUI;							// 新しいインターフェイスを使用する
	if ( fImport == ID_IMPORT ) {
		uBrowseInfo.ulFlags |= BIF_NONEWFOLDERBUTTON;					// [新しいフォルダの作成]ボタン消去		[2009/07/09 Add for Import]
	}
	uBrowseInfo.lpfn			= SHBrowseCallbackProc;					// コールバック関数（初期フォルダの指定）
// 210409 del -->
//// 157848 del -->
//	uBrowseInfo.lParam			= (LPARAM)pstrFolderPath->GetBuffer();	// コールバック関数のパラメタ（初期フォルダ）
//// 157848 del <--
//// 157848 add -->
//	//CString	cs=_T("");
//	//if(pstrFolderPath->IsEmpty() == FALSE)	{
//	//	cs.Format(_T("%s"),pstrFolderPath);
//	//}
//	//else	{
//	//	if(IsCloudEnv() == 1)	{
//	//		CString	def=_T("");
//	//		CString	errmsg=_T("");
//	//		if(GetSaveDefaultDirectory(def,errmsg,_T("")) == -1)	{
//	//			ICSMessageBox(errmsg,MB_ICONSTOP);
//	//			return FUNCTION_NG;
//	//		}
//	//		cs = def;
//	//	}
//	//}
//	//uBrowseInfo.lParam			= (LPARAM)cs.GetBuffer();	// コールバック関数のパラメタ（初期フォルダ）
//// 157848 add <--
//
//	// [フォルダの参照]ダイアログの表示 … [キャンセル]ボタンが押された場合はNULLを返す
//	lpuItemIdList = SHBrowseForFolder(&uBrowseInfo);
//
//	// [OK]ボタンが押された
//	if (lpuItemIdList != NULL) {
//		SHGetPathFromIDList(lpuItemIdList, szPathBuf);		// 選択したフォルダのパスを取得
//		CoTaskMemFree(lpuItemIdList);						// IDリストの解放
//
//		// 呼び出し元に返す
//		*pstrFolderPath = szPathBuf;
//	}
// 210409 del <--
// 210409 add -->
	uBrowseInfo.lParam = (LPARAM)pstrFolderPath->GetBuffer();	// コールバック関数のパラメタ（初期フォルダ）
	// オンクラウド対応
	// [フォルダの参照]ダイアログの表示 … [キャンセル]ボタンが押された場合はNULLを返す
	if(IsCloudEnv() == 1)	{
		CString		path=_T(""),emsg=_T("");
		int			rv=0;
		rv = SelectDirectory(path,emsg,_T(""),&uBrowseInfo);
		if(rv == -1)	{
			// エラー
			*pstrErrMsg = emsg;
			return(FUNCTION_NG);
		}
		else if(rv != 0)	{
			// [OK]ボタンが押された → 呼び出し元に返す
			strcpy_s(szPathBuf,sizeof(szPathBuf),path);
			*pstrFolderPath = szPathBuf;
		}

	}
	else	{
		lpuItemIdList = SHBrowseForFolder(&uBrowseInfo);
		// [OK]ボタンが押された
		if(lpuItemIdList != NULL)	{
			SHGetPathFromIDList(lpuItemIdList, szPathBuf);		// 選択したフォルダのパスを取得
			CoTaskMemFree(lpuItemIdList);						// IDリストの解放
			// 呼び出し元に返す
			*pstrFolderPath = szPathBuf;
		}
	}
// 210409 add <--

	// Shellアロケータの解放
	pMalloc->Release();

	return FUNCTION_OK;
}

//**************************************************
// SHBrowseForFolder()用のコールバック関数
//**************************************************
int CALLBACK CdlgImport::SHBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// 初期処理
	if (uMsg == BFFM_INITIALIZED) {
		// フォルダを指定（lpDataにフォルダパスが入っている）
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return	0;
}