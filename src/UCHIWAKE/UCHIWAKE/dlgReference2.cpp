// dlgReference2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "dlgReference2.h"

// 改良No.21-0086,21-0529 cor -->
//// midori 156188 add -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 156188 add <--
// ------------------------------
extern	BOOL	bG_InvNo;
extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

// CdlgReference2 ダイアログ

IMPLEMENT_DYNAMIC(CdlgReference2, ICSDialog)

//--------------------------------------------------
//	標準コンストラクタ
//--------------------------------------------------
CdlgReference2::CdlgReference2(CWnd* pParent /*=NULL*/)	: ICSDialog(CdlgReference2::IDD, pParent)
{
// midori 152137 add -->
	m_KamokuSort = 0;
// midori 152137 add <--
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgReference2::~CdlgReference2()
{
}

void CdlgReference2::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REF2_COMBO1, m_Ref2Combo1);	// 様式選択コンボ
	DDX_Control(pDX, IDC_REF2_CHECK5, m_Ref2Check5);	// 科目指定を行うチェック
	DDX_Control(pDX, IDC_REF2_REPORT, m_Ref2Report);	// 科目選択リスト
	DDX_Control(pDX, IDC_REF2_CHECK1, m_Ref2Check1);	// 科目でソートを行うチェック
// midori 151397 cor -->
//	DDX_Control(pDX, IDC_REF2_CHECK2, m_Ref2Check2);	// 改頁を行うチェック
// ---------------------
	DDX_Control(pDX, IDC_REF2_CHECK2, m_Ref2Check2);	// 科目毎に改頁を行うチェック
// midori 151397 cor <--
// midori 190505 add -->
	DDX_Control(pDX, IDC_REF2_CHECK6, m_Ref2Check6);	// 科目行を自動挿入する
// midori 190505 add <--
	DDX_Control(pDX, IDC_REF2_CHECK3, m_Ref2Check3);	// 「小計」を自動挿入するチェック
	DDX_Control(pDX, IDC_REF2_CHECK4, m_Ref2Check4);	// 一括金額を行うチェック
	DDX_Control(pDX, IDC_REF2_BUTTON, m_Ref2Button);	// 科目 全選択/全解除ボタン
	DDX_Control(pDX, ID_OK_BUTTON, m_btnOK);			// ＯＫボタン
	DDX_Control(pDX, IDCANCEL, m_btnCancel);			// キャンセルボタン
}

BEGIN_MESSAGE_MAP(CdlgReference2, ICSDialog)
	ON_BN_CLICKED(ID_OK_BUTTON, &CdlgReference2::OnBnClickedOk)
	ON_BN_CLICKED(IDC_REF2_CHECK5, &CdlgReference2::OnBnClickedRef2Check5)
	ON_BN_CLICKED(IDC_REF2_CHECK1, &CdlgReference2::OnBnClickedRef2Check1)
	ON_BN_CLICKED(IDC_REF2_CHECK2, &CdlgReference2::OnBnClickedRef2Check2)
// midori 190505 add -->
	ON_BN_CLICKED(IDC_REF2_CHECK6, &CdlgReference2::OnBnClickedRef2Check6)
// midori 190505 add <--
	ON_CBN_SELCHANGE(IDC_REF2_COMBO1, &CdlgReference2::OnCbnSelchangeRef2Combo1)
	ON_BN_CLICKED(IDC_REF2_BUTTON, &CdlgReference2::OnBnClickedRef2Button)
END_MESSAGE_MAP()


// CdlgReference2 メッセージ ハンドラ

// midori 151404 cor -->
////**************************************************
////	ダイアログ表示
////	【引数】	pDB				…	データベースハンドル
////				nFormSeq		…	様式シーケンス番号
////	【戻値】	ID_DLG_OK		…	OKボタン押された
////				ID_DLG_CANCEL	…	キャンセルボタン押された
////**************************************************
//int CdlgReference2::ShowDialog(BOOL& isChange, CDatabase* pDB, short nFormSeq)
// ---------------------
//**************************************************
//	ダイアログ表示
//	【引数】	isChange		…	変更サイン TRUE:再表示が必要
//				isYousiki[]		…	様式毎の変更サイン TRUE:再表示が必要
//				pDB				…	データベースハンドル
//				nFormSeq		…	様式シーケンス番号
//	【戻値】	ID_DLG_OK		…	OKボタン押された
//				ID_DLG_CANCEL	…	キャンセルボタン押された
//**************************************************
int CdlgReference2::ShowDialog(BOOL& isChange, BOOL isYousiki[] , CDatabase* pDB, short nFormSeq)
// midori 151404 cor <--
{
// midori 151404 add -->
	int ii = 0;
// midori 151404 add <--

	// 戻値初期化
	m_nRet = ID_DLG_CANCEL;
	m_isChangePageNum = FALSE;
// midori 151404 add -->
	for(ii=0;ii<UC_ID_FORMNO_MAX;ii++)	{
		m_isYousiki[ii] = isYousiki[ii];
	}
// midori 151404 add <--

	// データベースハンドルあり？
	if(pDB != NULL)	{
		try	{
			// データベース開いてる？
			if(pDB->IsOpen())	{
				m_pDB = pDB;			// データベースハンドル
				m_nFormSeq = nFormSeq;	// 様式シーケンス番号取得
				DoModal();				// モーダルで表示
				if(m_nRet != ID_DLG_CANCEL){
					isChange = m_isChangePageNum;
// midori 151404 add -->
					for(ii=0;ii<UC_ID_FORMNO_MAX;ii++)	{
						isYousiki[ii] = m_isYousiki[ii];
					}
// midori 151404 add <--
				}
			}
		}
		catch(...){
		}
	}
	// 戻値を返す
	return(m_nRet);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ダイアログ初期化
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReference2::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// 背景色取得
	ICSColorInfo	icsColInfo;
	m_ViewColor = GetSysColor(COLOR_WINDOW);
	if(((ICSWinApp *)AfxGetApp())->GetStanderdColor(icsColInfo) == 0)	{
		if(icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON)	{
			m_ViewColor = icsColInfo.BackColor();
		}
	}

	// レポートのヘッダ部作成
	InitReportHeader();

	// 様式コンボボックス初期化
	InitYoushikiCombo( m_pDB, &m_nFormSeq );

	//	様式コンボボックス変更
	ChangeYoushikiCombo( m_pDB, m_nFormSeq );

	// リサイズ
	OnInitDialogEX();
// midori 16/09/06 add -->
	m_Ref2Check1.ShowWindow(SW_HIDE);
// midori 16/09/06 add <--
	//	戻値を返す
	return( TRUE );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	選択変更（様式）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference2::OnCbnSelchangeRef2Combo1()
{
	int				nYoushikiIndex;		//	様式コンボボックスのインデックス
	int				nFormSeq = 0;
	int				nFormOrder = 0;
	
	//	様式コンボボックスのインデックス取得
	nYoushikiIndex = m_Ref2Combo1.GetCurSel();
	//	シーケンス番号取得
	nFormSeq = m_nFormSeqArray[nYoushikiIndex];

	if ( nFormSeq == 0 )			return;	//	シーケンス番号取得失敗？
	if ( m_nFormSeq == nFormSeq )	return;	//	様式を変えた？
	//	様式データ変更あり？
	if ( CheckYoushikiData( m_pDB, m_nFormSeq ) != 0 ){
		//	様式データ設定
		if(SetYoushikiData(m_pDB,m_nFormSeq) != 0)	{
			m_Ref2Combo1.SetCurSel(m_SaveCombo);
			nYoushikiIndex = m_Ref2Combo1.GetCurSel();
			nFormSeq = m_nFormSeqArray[nYoushikiIndex];
// midori 151395 add -->
			ICSMessageBox(_T("科目を選択してください。"));
			m_Ref2Report.SetFocus();
			m_Ref2Report.SetFocusedRow(0);
// midori 151395 add <--
			return;
		}
	}

	// 最新の様式シーケンス番号を取得
	m_nFormSeq = nFormSeq;
	// 様式変更
	ChangeYoushikiCombo( m_pDB, m_nFormSeq );
	// セーブを書き換える
	m_SaveCombo = m_Ref2Combo1.GetCurSel();
}

//**************************************************
//	様式データ確認
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	0			…	前回と同じ
//				0以外		…	前回から変更あり
//**************************************************
int CdlgReference2::CheckYoushikiData( CDatabase* pDB, short nFormSeq )
{
	int				nRet=0;
	int				sw=0;
	int				row=0;
	BOOL			brv=FALSE;
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	CdbUcLstKamoku	mfcRec2( pDB );	//	uc_lst_kamokuテーブルクラス

	// チェックボックスが変更されたか？
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();

			// 科目指定を行う
			if(mfcRec.m_GeneralVar[1] & BIT_D3)	sw=1;
			else								sw=0;
			if(sw != m_Ref2Check5.GetCheck())	{
				nRet++;	
			}
// midori 16/09/06 del -->
			//// 科目でソートを行う
			//if(mfcRec.m_GeneralVar[1] & BIT_D4)	sw=1;
			//else								sw=0;
			//if(sw != m_Ref2Check1.GetCheck())	{
			//	nRet++;	
			//}
// midori 16/09/06 del <--
// midori 151397 cor -->
//			// 改頁を行う
// ---------------------
			// 科目毎に改頁を行う
// midori 151397 cor <--
			if(mfcRec.m_GeneralVar[1] & BIT_D5)	sw=1;
			else								sw=0;
			if(sw != m_Ref2Check2.GetCheck())	{
				nRet++;	
			}
// midori 190505 add -->
			// 科目行を自動挿入する
			if(mfcRec.m_GeneralVar[2] & BIT_D3)	sw=1;
			else								sw=0;
			if(sw != m_Ref2Check6.GetCheck())	{
				nRet++;	
			}
// midori 190505 add <--
			// 「小計」を自動挿入する
			if(mfcRec.m_GeneralVar[1] & BIT_D6)	sw=1;
			else								sw=0;
			if(sw != m_Ref2Check3.GetCheck())	{
				nRet++;	
			}
			// 一括金額を行う
			if(mfcRec.m_GeneralVar[1] & BIT_D7)	sw=1;
			else								sw=0;
			if(sw != m_Ref2Check4.GetCheck())	{
				nRet++;	
			}

			// 【科目未入力】
			if(m_KamokuMinyu == TRUE)	{
				if(mfcRec.m_GeneralVar[2] & BIT_D0)	brv=FALSE;
				else								brv=TRUE;
				if(brv != m_Ref2Report.IsChecked(m_KamokuCount - 1,0))	{
					nRet++;
				}
			}
		}
		//	閉じる
		mfcRec.Fin();
	}

	// レポートの内容が変更されたか？
// midori 152137 del -->
//	if(mfcRec2.Init(nFormSeq) == DB_ERR_OK)	{
// midori 152137 del <--
// midori 152137 add -->
	if(mfcRec2.Init_M(nFormSeq,m_KamokuSort) == DB_ERR_OK)	{
// midori 152137 add <--
		if(!(mfcRec2.IsEOF()))	{
			mfcRec2.MoveFirst();
			row=0;
			while(!mfcRec2.IsEOF())	{
				if(mfcRec2.m_PrSign == 0)	brv=TRUE;
				else						brv=FALSE;
				if(brv != m_Ref2Report.IsChecked(row,0))	{
					nRet++;
					break;
				}
				mfcRec2.MoveNext();	// 次のレコードに移動
				row++;
			}
		}
		// レコード閉じる
		mfcRec2.Fin();
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	レポートコントロールの初期化
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CdlgReference2::InitReportHeader()
{
	m_Ref2Report.RemoveAllRecords();
	m_Ref2Report.RemoveColumn(-1);
	
	m_Ref2Report.AddColumn(new ICSReportColumn(0, NULL, 14));
	m_Ref2Report.AddColumn(new ICSReportColumn(1, NULL, 14));
// midori 152137 del -->
//	m_Ref2Report.AddColumn(new ICSReportColumn(2, NULL, 72));
// midori 152137 del <--
// midori 152137 add -->
	m_Ref2Report.AddColumn(new ICSReportColumn(2, NULL, 58));
	m_Ref2Report.AddColumn(new ICSReportColumn(3, NULL, 14));
// midori 152137 add <--

	m_Ref2Report.SetSortable ( FALSE );			// ソート禁止
	m_Ref2Report.SetDraggable( FALSE );			// ヘッダードラッグ禁止
	m_Ref2Report.SetRemovable( FALSE );			// ヘッダー削除禁止
	m_Ref2Report.SetMultipleSelection( FALSE );	// 複数行選択禁止

	SetGrid();

	ICSReportColumns *m_pClms = m_Ref2Report.GetColumns();
	int cnt = sizeof(strPrintAllHeader2) / sizeof(strPrintAllHeader2[0]);
	for(int i = 0; i < cnt; i++) {
		ICSReportColumn *m_pClm = m_pClms->GetAt(i);
		m_pClm->SetCaption(strPrintAllHeader2[i]);
	}

	// フォント設定
	CFont*		pFont;
	LOGFONT		font = {0};

	pFont	=	this->GetFont();
	pFont->GetLogFont(&font);
	m_Ref2Report.SetTextFont(font);
	m_Ref2Report.SetHeaderFont(font);
}

void CdlgReference2::SetGrid()
{
	CDWordArray arryGridColumn;
	arryGridColumn.Add(0);
	arryGridColumn.Add(1);
// midori 152137 add -->
	arryGridColumn.Add(2);
// midori 152137 add <--

	CDWordArray arrayGridColor;
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
	arrayGridColor.Add(RGB(255, 0, 0));
// midori 152137 del -->
//	arrayGridColor.Add(RGB(255, 0, 0));
// midori 152137 del <--

	m_Ref2Report.SetVerGrid(&arryGridColumn, &arrayGridColor, RGS_SOLID);
}

//**************************************************
//	様式コンボボックス初期化
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgReference2::InitYoushikiCombo( CDatabase* pDB, short *nFormSeq )
{
	int					nCnt=0;			// カウント用
	int					nCnt2=0;		// カウント用
	int					nIndex=-1;		// インデックス番号
	int					nSw=0;			// サイン
	CString				szBuf=_T("");	// バッファ
	CString				szShow=_T("");	// 表示用
	CdlgAddressSearch	clsAdd(this);	// 住所検索クラス（※文字列操作メソッドのみ使用）
	CdbUcInfSub			mfcRec(pDB);	// uc_inf_subテーブルクラス
	CdbUcLstKamoku		mfcRec2(pDB);	// uc_lst_kamokuテーブルクラス

	//	コンボボックスをクリア
	m_Ref2Combo1.ResetContent();
	
	try{
		
		//	様式数分ループ
		for(nCnt=0,nCnt2=0; nCnt<UC_ID_FORMNO_MAX; nCnt++)	{

			m_nFormSeqArray[nCnt] = 0;

			//	初期化成功？
			if ( mfcRec.Init( nCnt + 1 ) == DB_ERR_OK ){
				
				//	レコードあり？
				if ( !( mfcRec.IsEOF() ) ){

					CdbUc000Common* rsData = m_clsFunc.CreateDbObject(mfcRec.m_FormSeq,pDB);

					// 科目指定の対象となる様式のみ
					nSw=0;
					switch(mfcRec.m_FormSeq)	{
						case	ID_FORMNO_011:	//	①
						case	ID_FORMNO_021:	//	②
						case	ID_FORMNO_031:	//	③
						case	ID_FORMNO_041:	//	④-1
						case	ID_FORMNO_051:	//	⑤
						case	ID_FORMNO_091:	//	⑨
						case	ID_FORMNO_101:	//	⑩-1
						case	ID_FORMNO_111:	//	⑪
						case	ID_FORMNO_161:	//	⑯-1
						case	ID_FORMNO_162:	//	⑯-2
						case	ID_FORMNO_171:	//	⑰ 1
						case	ID_FORMNO_172:	//	⑰ 2
						case	ID_FORMNO_173:	//	⑰ 3
						case	ID_FORMNO_174:	//	⑰ 4
						case	ID_FORMNO_175:	//	⑰ 5
						case	ID_FORMNO_176:	//	⑰ 6
						case	ID_FORMNO_177:	//	⑰ 7
						case	ID_FORMNO_178:	//	⑰ 8
						case	ID_FORMNO_179:	//	⑰ 9
						case	ID_FORMNO_1710:	//	⑰ 10
						case	ID_FORMNO_1711:	//	⑰ 11
						case	ID_FORMNO_1712:	//	⑰ 12
						case	ID_FORMNO_1713:	//	⑰ 13
						case	ID_FORMNO_1714:	//	⑰ 14
						case	ID_FORMNO_1715:	//	⑰ 15
						case	ID_FORMNO_1716:	//	⑰ 16
						case	ID_FORMNO_1717:	//	⑰ 17
						case	ID_FORMNO_1718:	//	⑰ 18
						case	ID_FORMNO_1719:	//	⑰ 19
						case	ID_FORMNO_1720:	//	⑰ 20
						case	ID_FORMNO_181:	//	⑱ 1
						case	ID_FORMNO_182:	//	⑱ 2
						case	ID_FORMNO_183:	//	⑱ 3
						case	ID_FORMNO_184:	//	⑱ 4
						case	ID_FORMNO_185:	//	⑱ 5
						case	ID_FORMNO_186:	//	⑱ 6
						case	ID_FORMNO_187:	//	⑱ 7
						case	ID_FORMNO_188:	//	⑱ 8
						case	ID_FORMNO_189:	//	⑱ 9
						case	ID_FORMNO_1810:	//	⑱ 10
						case	ID_FORMNO_1811:	//	⑱ 11
						case	ID_FORMNO_1812:	//	⑱ 12
						case	ID_FORMNO_1813:	//	⑱ 13
						case	ID_FORMNO_1814:	//	⑱ 14
						case	ID_FORMNO_1815:	//	⑱ 15
						case	ID_FORMNO_1816:	//	⑱ 16
						case	ID_FORMNO_1817:	//	⑱ 17
						case	ID_FORMNO_1818:	//	⑱ 18
						case	ID_FORMNO_1819:	//	⑱ 19
						case	ID_FORMNO_1820:	//	⑱ 20
							// 科目が登録されているか？
							if(mfcRec2.Init(mfcRec.m_FormSeq) == DB_ERR_OK)	{
								if(!(mfcRec2.IsEOF()))	{
									nSw=1;
								}
							}
							// データが登録されているか？
							if(nSw == 0)	{
								if(rsData->GetCountDataRecord(1,1) != 0)	{
									nSw=1;
								}
							}
							break;

// 改良No.21-0086,21-0529 add -->
						case	ID_FORMNO_081:	//	⑧
							if(bG_InvNo == TRUE) {
								// 科目が登録されているか？
								if(mfcRec2.Init(mfcRec.m_FormSeq) == DB_ERR_OK) {
									if(!(mfcRec2.IsEOF())) {
										nSw = 1;
									}
								}
								// データが登録されているか？
								if(nSw == 0) {
									if(rsData->GetCountDataRecord(1, 1) != 0) {
										nSw = 1;
									}
								}
							}
							break;
// 改良No.21-0086,21-0529 add <--

						default:
							nSw=0;
							break;
					}

					delete rsData;

					if(nSw == 1)	{
						szBuf.Empty();									//	バッファクリア
						szShow.Empty();									//	表示用 
						szBuf = mfcRec.m_TitleNo + mfcRec.m_TitleNo2;	//	タイトル番号文字列連結
						szBuf += "．" ;									//	ドット連結
						szBuf +=  mfcRec.m_Title;						//	タイトル文字列連結
						clsAdd.GetSpritString( szBuf, &szShow, 64 );	//	表示範囲までの文字列を切り取り
						m_Ref2Combo1.AddString( szShow );				//	コンボボックスに追加
						m_nFormSeqArray[nCnt2] = mfcRec.m_FormSeq;
						//	シーケンス番号が一致した？
						if ( mfcRec.m_FormSeq == *nFormSeq ){
							//	インデックス番号を取得
							nIndex = nCnt2;
						}
						nCnt2++;
					}
				}
				//	レコード閉じる
				mfcRec.Fin();
			}
		}
	}
	catch(...){
		
		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	
	//	インデックスが範囲内か？
	if (( nIndex >= 0 ) && ( nIndex < UC_ID_FORMNO_MAX )){
		//	指定インデックスにカーソルセット
		m_Ref2Combo1.SetCurSel( nIndex );
	}
	else	{
		// 該当がなければ「預貯金等の内訳書」にセット
		*nFormSeq = ID_FORMNO_011;
		m_Ref2Combo1.SetCurSel(0);
	}
	m_SaveCombo = m_Ref2Combo1.GetCurSel();
}

//**************************************************
//	様式コンボボックス変更
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
void CdlgReference2::ChangeYoushikiCombo( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	int				sw=0;

	//	シーケンス番号で絞込み
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();

			// 科目指定を行う
			if(mfcRec.m_GeneralVar[1] & BIT_D3)	sw=1;
			else								sw=0;
			m_Ref2Check5.SetCheck(sw);
			// 科目でソートを行うチェック
			if(mfcRec.m_GeneralVar[1] & BIT_D4)	sw=1;
			else								sw=0;
// midori 16/09/06 add -->
			sw=1;
// midori 16/09/06 add <--
			m_Ref2Check1.SetCheck(sw);
// midori 151397 cor -->
//			// 改頁を行うチェック
// ---------------------
			// 科目毎に改頁を行うチェック
// midori 151397 cor <--
			if(mfcRec.m_GeneralVar[1] & BIT_D5)	sw=1;
			else								sw=0;
			m_Ref2Check2.SetCheck(sw);
// midori 190505 add -->
			// 科目行を自動挿入するチェック
			if(mfcRec.m_GeneralVar[2] & BIT_D3)	sw=1;
			else								sw=0;
			m_Ref2Check6.SetCheck(sw);
// midori 190505 add <--
			// 「小計」を自動挿入するチェック
			if(mfcRec.m_GeneralVar[1] & BIT_D6)	sw=1;
			else								sw=0;
			m_Ref2Check3.SetCheck(sw);
			// 一括金額を行うチェック
			if(mfcRec.m_GeneralVar[1] & BIT_D7)	sw=1;
			else								sw=0;
			m_Ref2Check4.SetCheck(sw);
// midori 152137 add -->
			// 表示順 0:番号順 1:表示順
			if(mfcRec.m_GeneralVar[2] & BIT_D1)	m_KamokuSort=1;
			else								m_KamokuSort=0;
// midori 152137 add <--

			// 科目一覧を表示
			SetListData( m_pDB, m_nFormSeq );

			// チェックボックスEnable/Disableの制御
			SetRef2Check();
		}
		//	閉じる
		mfcRec.Fin();
	}
}

//**************************************************
//	様式データ設定
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//	【戻値】	なし
//**************************************************
int CdlgReference2::SetYoushikiData( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub		mfcRec( pDB );	//	uc_inf_subテーブルクラス
	CdbUcLstKamoku	mfcRec2( pDB );	//	uc_lst_kamokuテーブルクラス
	int				st=0;
	int				old_st=0;
	int				row=0;
// midori 151404 add -->
	int				no=-1;			//	FormOrder - 1
// midori 151404 add <--
	BOOL			brv=FALSE;

	// チェック
	if(CheckYoushikiData() != 0)	{
		return(1);
	}

	// チェックボックスの内容を書き込む
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	先頭に移動
			mfcRec.MoveFirst();

// midori 151404 add -->
			no = mfcRec.m_FormOrder - 1;
// midori 151404 add <--
			// -------------------------------------------------
			// 科目指定を行う
			// -------------------------------------------------
			st = m_Ref2Check5.GetCheck();
			// 前回の値を取得
			if(mfcRec.m_GeneralVar[1] & BIT_D3)	old_st = 1;
			else								old_st = 0;
			if(st != old_st)	{	// 変更された
				// 編集開始
				mfcRec.Edit();
				if(st)	mfcRec.m_GeneralVar[1] |= BIT_D3;
				else	mfcRec.m_GeneralVar[1] &= ~BIT_D3;
				//	更新
				mfcRec.Update();
				m_isChangePageNum = TRUE;
// midori 151404 add -->
				m_isYousiki[no] = TRUE;
// midori 151404 add <--
			}

			// -------------------------------------------------
			// 科目でソートを行う
			// -------------------------------------------------
			st = m_Ref2Check1.GetCheck();
// midori 16/09/06 add -->
			st = 1;
// midori 16/09/06 add <--
			// 前回の値を取得
			if(mfcRec.m_GeneralVar[1] & BIT_D4)	old_st = 1;
			else								old_st = 0;
			if(st != old_st)	{	// 変更された
				// 編集開始
				mfcRec.Edit();
				if(st)	mfcRec.m_GeneralVar[1] |= BIT_D4;
				else	mfcRec.m_GeneralVar[1] &= ~BIT_D4;
				//	更新
				mfcRec.Update();
				m_isChangePageNum = TRUE;
// midori 151404 add -->
				m_isYousiki[no] = TRUE;
// midori 151404 add <--
			}

			// -------------------------------------------------
// midori 151397 cor -->
//			// 改頁を行う
// ---------------------
			// 科目毎に改頁を行う
// midori 151397 cor <--
			// -------------------------------------------------
			st = m_Ref2Check2.GetCheck();
			// 前回の値を取得
			if(mfcRec.m_GeneralVar[1] & BIT_D5)	old_st = 1;
			else								old_st = 0;
			if(st != old_st)	{	// 変更された
				// 編集開始
				mfcRec.Edit();
				if(st)	mfcRec.m_GeneralVar[1] |= BIT_D5;
				else	mfcRec.m_GeneralVar[1] &= ~BIT_D5;
				//	更新
				mfcRec.Update();
				m_isChangePageNum = TRUE;
// midori 151404 add -->
				m_isYousiki[no] = TRUE;
// midori 151404 add <--
			}

// midori 190505 add -->
			// -------------------------------------------------
			// 科目行を自動挿入する
			// -------------------------------------------------
			st = m_Ref2Check6.GetCheck();
			// 前回の値を取得
			if(mfcRec.m_GeneralVar[2] & BIT_D3)	old_st = 1;
			else								old_st = 0;
			if(st != old_st)	{	// 変更された
				// 編集開始
				mfcRec.Edit();
				if(st)	mfcRec.m_GeneralVar[2] |= BIT_D3;
				else	mfcRec.m_GeneralVar[2] &= ~BIT_D3;
				//	更新
				mfcRec.Update();
				m_isChangePageNum = TRUE;
				m_isYousiki[no] = TRUE;
			}
// midori 190505 add <--

			// -------------------------------------------------
			// 「小計」を自動挿入する
			// -------------------------------------------------
			st = m_Ref2Check3.GetCheck();
			// 前回の値を取得
			if(mfcRec.m_GeneralVar[1] & BIT_D6)	old_st = 1;
			else								old_st = 0;
			if(st != old_st)	{	// 変更された
				// 編集開始
				mfcRec.Edit();
				if(st)	mfcRec.m_GeneralVar[1] |= BIT_D6;
				else	mfcRec.m_GeneralVar[1] &= ~BIT_D6;
				//	更新
				mfcRec.Update();
				m_isChangePageNum = TRUE;
// midori 151404 add -->
				m_isYousiki[no] = TRUE;
// midori 151404 add <--
			}
			// -------------------------------------------------
			// 一括金額を行う
			// -------------------------------------------------
			st = m_Ref2Check4.GetCheck();
			// 前回の値を取得
			if(mfcRec.m_GeneralVar[1] & BIT_D7)	old_st = 1;
			else								old_st = 0;
			if(st != old_st)	{	// 変更された
				// 編集開始
				mfcRec.Edit();
				if(st)	mfcRec.m_GeneralVar[1] |= BIT_D7;
				else	mfcRec.m_GeneralVar[1] &= ~BIT_D7;
				//	更新
				mfcRec.Update();
				m_isChangePageNum = TRUE;
// midori 151404 add -->
				m_isYousiki[no] = TRUE;
// midori 151404 add <--
			}

			// -------------------------------------------------
			// 【科目未入力】
			// -------------------------------------------------
			if(m_KamokuMinyu == TRUE)	{
				if(mfcRec.m_GeneralVar[2] & BIT_D0)	brv=FALSE;
				else								brv=TRUE;
				if(brv != m_Ref2Report.IsChecked(m_KamokuCount-1,0))	{
					mfcRec.Edit();
					if(m_Ref2Report.IsChecked(m_KamokuCount-1,0) == TRUE)	mfcRec.m_GeneralVar[2] &= ~BIT_D0;
					else													mfcRec.m_GeneralVar[2] |= BIT_D0;
					mfcRec.Update();
					m_isChangePageNum = TRUE;
// midori 151404 add -->
					m_isYousiki[no] = TRUE;
// midori 151404 add <--
				}
			}

		}
		//	閉じる
		mfcRec.Fin();
	}

	// レポートの内容を書き込む
// midori 152137 del -->
//	if(mfcRec2.Init(nFormSeq) == DB_ERR_OK)	{
// midori 152137 del <--
// midori 152137 add -->
	if(mfcRec2.Init_M(nFormSeq,m_KamokuSort) == DB_ERR_OK)	{
// midori 152137 add <--
		if(!(mfcRec2.IsEOF()))	{
			mfcRec2.MoveFirst();
			row=0;
			while(!mfcRec2.IsEOF())	{
				if(mfcRec2.m_PrSign == 0)	brv=TRUE;
				else						brv=FALSE;
				if(brv != m_Ref2Report.IsChecked(row,0))	{
					mfcRec2.Edit();
					if(m_Ref2Report.IsChecked(row,0) == TRUE)	mfcRec2.m_PrSign=0;
					else										mfcRec2.m_PrSign=1;
					mfcRec2.Update();
					m_isChangePageNum = TRUE;
// midori 151404 add -->
					if(no >= 0) m_isYousiki[no] = TRUE;
// midori 151404 add <--
				}
				mfcRec2.MoveNext();	// 次のレコードに移動
				row++;
			}
		}
		// レコード閉じる
		mfcRec2.Fin();
	}

	return(0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（OKボタン）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference2::OnBnClickedOk()
{
	// 様式データ設定
	if(SetYoushikiData(m_pDB,m_nFormSeq) != 0)	{
// midori 151395 add -->
		ICSMessageBox(_T("科目を選択してください。"));
		m_Ref2Report.SetFocus();
		m_Ref2Report.SetFocusedRow(0);
// midori 151395 add <--
		return;
	}

	// 戻り値セット
	m_nRet = ID_DLG_OK;

	ICSDialog::OnOK();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	OnOkイベント
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CdlgReference2::OnOK()
{
//	ICSDialog::OnOK();
}

//**************************************************
// チェックボックスEnable/Disableの制御
//**************************************************
void CdlgReference2::SetRef2Check()
{
	// 科目指定を行うＯＮ
	if(m_Ref2Check5.GetCheck())	{
		KamokuSelControl(TRUE);
		m_Ref2Button.EnableWindow(TRUE);
// midori 16/09/06 del -->
		//m_Ref2Check1.EnableWindow(TRUE);
// midori 16/09/06 del <--
		// 科目でソートを行うＯＮ
		if(m_Ref2Check1.GetCheck())	{
			m_Ref2Check2.EnableWindow(TRUE);
// midori 190505 add -->
// 改良No.21-0086,21-0529 cor -->
			//// 呼び出し元の様式が ②受取手形、⑪借入金の内訳書の科目行 の場合のみ「科目行」の選択を有効にする
			//if(m_nFormSeq == ID_FORMNO_021 || m_nFormSeq == ID_FORMNO_111)	{
// ------------------------------
			// 呼び出し元の様式が ②受取手形、ｲﾝﾎﾞｲｽ番号追加版の⑧支払手形、⑪借入金の内訳書の科目行 の場合のみ「科目行」の選択を有効にする
			if(m_nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_nFormSeq == ID_FORMNO_081) || m_nFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
				m_Ref2Check6.EnableWindow(TRUE);
			}
			else {
				m_Ref2Check6.EnableWindow(FALSE);
			}
// midori 190505 add <--
			m_Ref2Check3.EnableWindow(TRUE);
			if( m_nFormSeq != ID_FORMNO_011 )	{
				m_Ref2Check4.EnableWindow(TRUE);
			}
			else	{
				// 以下の内訳書は「一括金額」なし
				// ・預貯金等の内訳書
				m_Ref2Check4.EnableWindow(FALSE);
			}
		}
		else	{
			m_Ref2Check2.EnableWindow(FALSE);
// midori 190505 add -->
			m_Ref2Check6.EnableWindow(FALSE);
// midori 190505 add <--
			m_Ref2Check3.EnableWindow(FALSE);
			m_Ref2Check4.EnableWindow(FALSE);
		}
	}
	else	{
		KamokuSelControl(FALSE);
		m_Ref2Button.EnableWindow(FALSE);
// midori 16/09/06 del -->
		//m_Ref2Check1.EnableWindow(FALSE);
// midori 16/09/06 del <--
		m_Ref2Check2.EnableWindow(FALSE);
// midori 190505 add -->
		m_Ref2Check6.EnableWindow(FALSE);
// midori 190505 add <--
		m_Ref2Check3.EnableWindow(FALSE);
		m_Ref2Check4.EnableWindow(FALSE);
	}
// midori 156188 add -->
	//if(KamokuRowEnableSgn(m_pDB,0) == 0) {				// 改良No.21-0086,21-0529 del
	if(KamokuRowEnableSgn(m_pDB, 0, m_nFormSeq) == 0)	{	// 改良No.21-0086,21-0529 add
		m_Ref2Check6.EnableWindow(FALSE);
		m_Ref2Check6.ShowWindow(SW_HIDE);
	}
// midori 156188 add <--
}

//**************************************************
// 「科目指定を行う」クリックイベント
//**************************************************
void CdlgReference2::OnBnClickedRef2Check5()
{
	SetRef2Check();
}

//**************************************************
// 「科目でソートを行う」クリックイベント
//**************************************************
void CdlgReference2::OnBnClickedRef2Check1()
{
	SetRef2Check();
}

//**************************************************
// 「改頁を行う」クリックイベント
//**************************************************
void CdlgReference2::OnBnClickedRef2Check2()
{
	SetRef2Check();
}

// midori 190505 add -->
//**************************************************
// 「科目行を自動挿入する」クリックイベント
//**************************************************
void CdlgReference2::OnBnClickedRef2Check6()
{
	SetRef2Check();
}
// midori 190505 add <--

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	メッセージ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CdlgReference2::PreTranslateMessage(MSG* pMsg)
{
	int						sw=0;
	int						nc=-1;
	int						indexRow=0;
	char					cs=0;
	ICSReportSelectedRows*	s_rows;
	ICSReportRow*			r_row;

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		
		switch(pMsg->wParam)	{
			case	VK_RETURN:
			case	VK_RIGHT:
			case	VK_DOWN:
				nc=0;
				break;
			case	VK_TAB:
				cs = (char)GetKeyState(VK_SHIFT);
				if(cs & 0x80)	nc=1;
				else			nc=0;
				break;
			case	VK_F2:
			case	VK_LEFT:
			case	VK_UP:
				nc=1;
				break;
			default:
				break;
		}

		// レポートにフォーカスをセットする動作
		s_rows = m_Ref2Report.GetSelectedRows();
		if(s_rows != NULL)	{
			r_row = s_rows->GetAt(0);
			if(r_row != NULL)	{
				indexRow = r_row->GetIndex();
			}
		}
		switch(m_clsFunc.GetFocusControlID(this))	{
			case	IDC_REF2_CHECK5:
				// ①科目指定を行うにフォーカスがあり、Enterキー等の進む動作
				if(nc == 0)	{
					sw=1;
				}
				break;
			//case	IDC_REF2_CHECK1:
			//	// ②科目でソートを行うにフォーカスがあり、F2キー等の戻る動作
			case	IDC_REF2_CHECK2:
				// ②改頁を行うにフォーカスがあり、F2キー等の戻る動作
				if(nc == 1)	{
					sw=1;
				}
				break;
			case	ID_OK_BUTTON:
				// ③ＯＫボタンにフォーカスがあり、科目指定を行うにチェックが付いていないとき、F2キー等の戻る動作
				if(nc == 1)	{
					if(m_Ref2Check5.GetCheck() == 0)	{
						sw=1;
					}
				}
				break;
		}
		if(sw == 1)	{
			m_Ref2Report.SetFocus();
			m_Ref2Report.SetFocusedRow(indexRow);
		}
		else	{
			// 上記以外の動作
			switch( pMsg->wParam )	{
				case VK_F2:
					// F2キーが押された
					keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
					keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
					keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
					keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
					sw = 1;
					break;
				case VK_SPACE:
				case VK_RETURN:
				case VK_TAB:
					// レポートコントロールにフォーカスがある場合
					if(m_clsFunc.GetFocusControlID(this) == IDC_REF2_REPORT)	{
						UpdateStateFromKey((int)pMsg->wParam);
						sw=1;
					}
					break;
			}
		}
	}
	if(sw == 1)	{
		return(1);
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

//**************************************************
//	レポートに科目一覧を表示する
//	【引数】	pDB			…	データベースハンドル
//				nFormSeq	…	様式シーケンス番号
//				strFilterKana	…　ｶﾅ検索用フィルタ
//	【戻値】	なし
//**************************************************
void CdlgReference2::SetListData( CDatabase* pDB, short nFormSeq )
{
	CdbUcInfSub		mfcRec( pDB );		//	uc_inf_subテーブルクラス
	CdbUcLstKamoku	mfcRec2( pDB );		//	uc_lst_kamokuテーブルクラス
	char			szBuf[8];			//	文字列変換用バッファ
	BOOL			bsw=FALSE;
	CdbUc000Common* rsData = m_clsFunc.CreateDbObject(nFormSeq,pDB);

	m_KamokuCount = 0;
	m_KamokuMinyu = FALSE;

	m_Ref2Report.RemoveAllRecords();

	try{

		//	初期化成功？
// midori 152137 del -->
//		if ( mfcRec2.Init( nFormSeq ) == DB_ERR_OK ){
// midori 152137 del <--
// midori 152137 add -->
		if ( mfcRec2.Init_M( nFormSeq, m_KamokuSort ) == DB_ERR_OK ){
// midori 152137 add <--
			
			//	レコードあり？
			if ( !( mfcRec2.IsEOF() ) ){

				mfcRec2.MoveFirst();		//	レコードを先頭に移動
				
				//	レコード終端までループ
				while( !mfcRec2.IsEOF() ){

					ICSReportRecord*  pRecord = m_Ref2Report.AddRecord(new ICSReportRecord());

					// 1.選択
					if(mfcRec2.m_PrSign == 0)	bsw=TRUE;
					else						bsw=FALSE;
					pRecord->AddItem( new ICSReportRecordItemCheckBox(_T(""),bsw));

					// 2.番号
					ZeroMemory( szBuf, sizeof( szBuf ) );
					_itoa_s( mfcRec2.m_KnOrder, szBuf, 10 );
					pRecord->AddItem( new ICSReportRecordItemText(szBuf,ICS_EDS_NUMERIC,TRUE,1,4,ICS_EDS_CENTER));

					// 3.名称
					pRecord->AddItem( new ICSReportRecordItemText(mfcRec2.m_KnName));

// midori 152137 add -->
					// 4.カナ
					pRecord->AddItem( new ICSReportRecordItemText(mfcRec2.m_KnKana));
// midori 152137 add <--

					m_KamokuCount++;

					mfcRec2.MoveNext();	//	次のレコードに移動
				}	
			}
			//	レコード閉じる
			mfcRec2.Fin();
		}
	}
	catch(...){
		
		//	レコード開いてる？
		if ( mfcRec2.IsOpen() ){
			//	レコード閉じる
			mfcRec2.Fin();
		}
	}

	// 科目未入力データがあれば、一覧に追加する
	if(rsData->GetCountDataRecord(1,1) != 0)	{
		if(mfcRec.RequeryFormSeq(nFormSeq) == DB_ERR_OK)	{
			if(!mfcRec.IsEOF())	{
				mfcRec.MoveFirst();
				ICSReportRecord*  pRecord = m_Ref2Report.AddRecord(new ICSReportRecord());

				// 1.選択
				if(mfcRec.m_GeneralVar[2] & BIT_D0)	bsw=FALSE;
				else								bsw=TRUE;
				pRecord->AddItem( new ICSReportRecordItemCheckBox(_T(""),bsw));

				// 2.番号
				ZeroMemory( szBuf, sizeof( szBuf ) );
				pRecord->AddItem( new ICSReportRecordItemText(szBuf,ICS_EDS_NUMERIC,TRUE,1,4,ICS_EDS_CENTER));

				// 3.名称
				pRecord->AddItem( new ICSReportRecordItemText(_T("【科目未入力】")));

// midori 152137 add -->
				// 4.カナ
				pRecord->AddItem( new ICSReportRecordItemText(_T("")));
// midori 152137 add <--

				m_KamokuCount++;
				m_KamokuMinyu = TRUE;
			}
			mfcRec.Fin();
		}
	}

	delete rsData;

	// ここ以降でないと、効かない
	m_Ref2Report.AllowEdit(TRUE);				// 全て編集許可にしたあと
	m_Ref2Report.AllowEditColumn(1, FALSE);		// 様式番号と
	m_Ref2Report.AllowEditColumn(2, FALSE);		// 様式名称はEdit不可に

	// 行の高さ
	m_Ref2Report.SetItemHeight(20);

	m_Ref2Report.Populate();
}

//**************************************************
//	科目選択レポートの制御(Enable/Disable)
//**************************************************
void CdlgReference2::KamokuSelControl( BOOL pSw )
{
	int		ii=0;

	for(ii=0; ii<m_KamokuCount; ii++)	{
		if(pSw != FALSE)	{
			m_Ref2Report.AllowEditItem(ii,0,TRUE);
			m_Ref2Report.AllowEditItem(ii,1,FALSE);
			m_Ref2Report.AllowEditItem(ii,2,FALSE);
			m_Ref2Report.AllowEditItem(ii,3,FALSE);
			m_Ref2Report.SetRowBackColor(ii,m_ViewColor,TRUE);
		}
		else	{
			m_Ref2Report.AllowEditRow(ii,pSw);
			m_Ref2Report.SetRowBackColor(ii,GetSysColor(COLOR_BTNFACE),TRUE);
		}
	}
}

//**************************************************
//	科目 全選択/全解除ボタンの動作
//**************************************************
void CdlgReference2::OnBnClickedRef2Button()
{
	m_clsFunc.ChangeReportColumnAll(&m_Ref2Report,0);
}

//**************************************************
// レポートコントロールにフォーカスがある場合に
// Space,Tab,Enterキーが押下された場合の動作
//**************************************************
void CdlgReference2::UpdateStateFromKey(int key)
{
	ICSReportSelectedRows*	s_rows;
	ICSReportRow*			r_row;
	int						indexRow=0;
	char					cShift=0;
	BOOL					check=FALSE;

	s_rows = m_Ref2Report.GetSelectedRows();
	if(s_rows == NULL)	return;
	r_row = s_rows->GetAt(0);
	if(r_row == NULL)	return;
	indexRow = r_row->GetIndex();
	check = m_Ref2Report.IsChecked(indexRow,0);	// 0:選択列

	switch(key){
		case VK_SPACE:
			// 科目指定を行うにチェックが付いていない場合はチェック入れない
			if(m_Ref2Check5.GetCheck() == 0)	return;
			// 選択カラムの状態反転
			check = !check;
			m_Ref2Report.SetChecked(indexRow,0,check);
			m_Ref2Report.RedrawControl();
			break;
		case VK_RETURN:
			m_Ref2Report.MoveDown(FALSE,FALSE);
			break;
		case VK_TAB:
			cShift = (char)GetKeyState(VK_SHIFT);	// SHIFTキー状態取得
			//	SHIFTキーが押されてる？
			if(cShift & 0x80)	PrevDlgCtrl();
			else				NextDlgCtrl();
			break;
	}
}

//**************************************************
// データ書き込み前のチェック
//**************************************************
int CdlgReference2::CheckYoushikiData()
{
	int				cnt_max=0;
	int				cnt=0;
	int				ii=0;
	ICSReportRows	*rrs;

	if(m_Ref2Check5.GetCheck() == 0)	return(0);
	rrs = m_Ref2Report.GetRows();
	if(rrs == NULL)						return(0);
	cnt_max = rrs->GetCount();
	// チェックが入っている数取得
	for(ii=0; ii<cnt_max; ii++)	{
		if(m_Ref2Report.IsChecked(ii,0) == TRUE)	cnt++;
	}
	if(cnt == 0)	{
// midori 151395 del -->
		//ICSMessageBox(_T("科目を選択してください。"));
		//m_Ref2Report.SetFocus();
		//m_Ref2Report.SetFocusedRow(0);
// midori 151395 del <--
		return(1);
	}
	else								return(0);
}
