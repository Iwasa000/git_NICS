/*****************************************************************
	UcFunctionCommonクラス

	<クラス方針>
	帳表、ダイアログいずれのクラスでも使用できる関数を記述する

	2006.08.25	New
******************************************************************/
#include "StdAfx.h"
#include "UcFunctionCommon.h"

#include "UCHIWAKE.h"	// 2010/08/12 一括印刷 印刷時メッセージ非表示対応

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CUcFunctionCommon::CUcFunctionCommon(void)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CUcFunctionCommon::~CUcFunctionCommon(void)
{
}

/**********************************************************************
	GetKcdKubun()
		財務科目コードから、大分類、中分類、小分類の区分を取得する

	引数
			CString		inKcd		財務科目コード
	戻値
			int						 0：入力科目
									 1：小分類科目
									 2：中分類科目
									 3：大分類科目
									 4："00000000"
									-1：エラー
***********************************************************************/
int CUcFunctionCommon::GetKcdKubun(CString inKcd)
{
	int			intRet = -1;

	// 財務科目コードは８桁の文字列
	if (inKcd.GetLength() == 8) {
		// "00000000"
		if (inKcd == _T("00000000")) {
			intRet = 4;
		}
		// 大分類科目
		else if (inKcd.Right(6) == _T("000000")) {
			intRet = 3;
		}
		// 中分類科目
		else if (inKcd.Right(4) == _T("0000")) {
			intRet = 2;
		}
		// 小分類科目
		else if (inKcd.Right(2) == _T("00")) {
			intRet = 1;
		}
		// 入力科目
		else {
			intRet = 0;
		}
	}
	return (intRet);
}

/**********************************************************************
	GetKcdFromTo()
		KcdStartから、財務科目コードの範囲を取得する

	引数
			CString		inKcdStart	KcdStartフォールドの科目コード
			CString*	outFrom		財務科目の範囲（From）
			CString*	outTo		財務科目の範囲（To）
	戻値
			BOOL					TRUE ：成功
									FALSE：失敗
***********************************************************************/
BOOL CUcFunctionCommon::GetKcdFromTo(CString inKcdStart, CString* outFrom, CString* outTo)
{
	// 初期化
	*outFrom = _T("");
	*outTo = _T("");

	// 財務科目コードの区分を取得
	int	intKcdKubun = GetKcdKubun(inKcdStart);

	switch (intKcdKubun) {
	case 4:			// "00000000"
		*outFrom =	inKcdStart;
		*outTo =	_T("ffffffff");
		break;
	case 3:			// 大分類科目
		*outFrom =	inKcdStart;
		*outTo =	inKcdStart.Left(2) + _T("ffffff");
		break;
	case 2:			// 中分類科目
		*outFrom =	inKcdStart;
		*outTo =	inKcdStart.Left(4) + _T("ffff");
		break;
	case 1:			// 小分類科目
		*outFrom =	inKcdStart;
		*outTo =	inKcdStart.Left(6) + _T("ff");
		break;
	case 0:			// 入力科目
		*outFrom =	inKcdStart;
		*outTo =	inKcdStart;
		break;
	default:		// エラー発生
		return (FALSE);
	}

	return (TRUE);
}

/**********************************************************************
	Trim()
		文字列の空白を文言から削除する
		テーブル操作ではないが、財務科目から内訳書科目の名称を取得する
		場合に必須となる処理なので、このクラスに作成する（暫定）

	引数
			CString		strOrg		空白を取り除きたい文言
	戻値
			CString					サッパリ文字列

	2006.03		旧財務のCut～から移動＋部門処理の"CAegisBase::Trim()"
***********************************************************************/
CString CUcFunctionCommon::Trim(CString strOrg)
{
	//	左右を取り除く
	strOrg.TrimLeft();
	strOrg.TrimRight();

	//	真ん中の文字列
	char	szText[64];
	ZeroMemory( szText, sizeof(szText));
	strcpy_s( szText, strOrg);
	
	char*	p = szText;
	int		len = (int)strlen(p);
	for( int i = 0; i < len; i++){
		if( IsDBCSLeadByte( (BYTE)p[i])){
			//	全角空白
			if( !strncmp( &p[i], "　", 2)){
				CopyMemory( &p[i], &p[i + 2], len - i - 2);
				p[len - 1] = 0;
				p[len - 2] = 0;
				len -= 2;
				i--;
			}
		}
		else{
			//	半角空白
			if( p[i] == ' '){
				CopyMemory( &p[i], &p[i + 1], len - i - 1);
				p[len - 1] = 0;
				len -= 1;
				i--;
			}
		}
	}
	CString	strRet = szText;
	return strRet;
}//	CAegisBase::Trim

//************************************************************
// 指定された範囲の摘要名を取得
//【引数】
// ZmSub		…	DB財務クラス
// inttkcod		…	指定摘要コード
// strtknam		…	摘要名格納バッファ
//【戻り値】
// 0		…　成功
// -1		…　摘要テーブルオープン失敗
// -2		…　摘要コードに対応するレコードが無かった
//*************************************************************
int CUcFunctionCommon::GetTekiyoData( CDBZmSub* ZmSub , long lngtkcod , CString *strtknam ){

	CString strFilter;		// クエリ文
	int		intRet = -1;		// 戻り値

	// 初期化
	*strtknam = _T("");

	// 摘要コード指定有り?
	if ( ( lngtkcod > 0 ) ){
		// クエリ文作成
		strFilter.Format( "tkcod='%d'" , lngtkcod );
		strFilter = _T(strFilter);
	
		// 摘要名取得
		if ( ZmSub->KanaTekiyoOpen( strFilter , 0 ) == 0 ){
			// 取得に成功すれば取得数分読込み
			if ( ZmSub->tkrec->GetRecordCount() > 0 ){
				// 摘要名取得
				*strtknam = ZmSub->tkrec->tkname;
				intRet = 0;
			}
			else{
				intRet = -2;
			}
			// 摘要名称テーブル閉じる
			ZmSub->KanaTekiyoClose();
		}
		else{
			// 2007.03 DBエラー対応
			ZmSubErrMsg( ZmSub );
			return FUNCTION_NG;
		}
	}

	return intRet;
}

// midori 152137 add -->
//************************************************************
// 指定された範囲の摘要カナを取得
//【引数】
// ZmSub		…	DB財務クラス
// inttkcod		…	指定摘要コード
// strtknam		…	摘要名格納バッファ
//【戻り値】
// 0		…　成功
// -1		…　摘要テーブルオープン失敗
// -2		…　摘要コードに対応するレコードが無かった
//*************************************************************
int CUcFunctionCommon::GetTekiyoKana( CDBZmSub* ZmSub , long lngtkcod , CString *strtknam )
{
	CString strFilter;		// クエリ文
	int		intRet = -1;		// 戻り値

	// 初期化
	*strtknam = _T("");

	// 摘要コード指定有り?
	if ( ( lngtkcod > 0 ) ){
		// クエリ文作成
		strFilter.Format( "tkcod='%d'" , lngtkcod );
		strFilter = _T(strFilter);
	
		// 摘要名取得
		if ( ZmSub->KanaTekiyoOpen( strFilter , 0 ) == 0 ){
			// 取得に成功すれば取得数分読込み
			if ( ZmSub->tkrec->GetRecordCount() > 0 ){
				// 摘要名取得
				*strtknam = ZmSub->tkrec->tkana;
				intRet = 0;
			}
			else{
				intRet = -2;
			}
			// 摘要名称テーブル閉じる
			ZmSub->KanaTekiyoClose();
		}
		else{
			// 2007.03 DBエラー対応
			ZmSubErrMsg( ZmSub );
			return FUNCTION_NG;
		}
	}

	return intRet;
}
// midori 152137 add <--

//************************************************************
// DB財務クラスのエラーメッセージを表示する
//【引数】
//		CDBZmSub*	pZmSub	…	DB財務クラスのポインタ
// 【戻り値】
//		BOOL		TRUE	…　成功
//					FALSE	…　失敗
//*************************************************************
BOOL CUcFunctionCommon::ZmSubErrMsg( CDBZmSub* pZmSub )
{
	//	DBエラー文字列を取得する
	CString strDBErrMsg;
	strDBErrMsg.Empty();
	strDBErrMsg = pZmSub->GetZmErrMsg();

	// "。"で改行する
	//strDBErrMsg.Replace( "。" , "。\r\n" );

	//	DBエラーメッセージを表示する
	//	アプリケーションを終了させる
	//	2007.05.21 メッセージダイアログ変更(ICSExit→ICSMessageBox)
//	ICSExit( 0 , (char*)(LPCSTR)strDBErrMsg);

	// メッセージ表示
	ICSMessageBox( strDBErrMsg , MB_ICONSTOP );

	// 終了メッセージ送信
	AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );

	return TRUE;
}

//************************************************************
// ラベルコントロール(ICSStatic)の有効/無効を切り替える
// ※ICSStaticコントロールの「EnableWindow」では文言がリソース時に戻ってしまうため
//	 の対応
//【引数】
//		ICSStatic*	lblData	…	ICSStaticコントロールのポインタ
//		BOOL		blnFlg	…	有効:TRUE 無効:FALSE
// 【戻り値】
//		なし
//*************************************************************
void CUcFunctionCommon::ICSStaticEnableWindow( ICSStatic* lblData , BOOL blnFlg )
{
	CString strData;

	strData.Empty();

	// 現在の文言保持
	strData = lblData->GetCaptionText();
	// 有効/無効切替
	lblData->EnableWindow( blnFlg );
	// 文言を戻す
	lblData->UpdateCaption( strData , 0 );
}

//************************************************************
// テーブルが開けるかチェックを行う
//【引数】
//		CdbBase* mfcRec
// 【戻り値】
//		TRUE	…	オープン可
//		FALSE	…	オープン不可
//*************************************************************
BOOL CUcFunctionCommon::CheckTableOpen( CdbBase* mfcRec )
{
	// テーブルを開く
	if ( mfcRec->Init() != DB_ERR_OPEN ){
		// テーブル閉じる
		mfcRec->Fin();
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//******************************************************************************************
// 末尾スペースを削除する。
// 　【引数】	CString			strData			対象の文字列
// 　【戻値】	末尾スペース削除後の文字列
//******************************************************************************************
CString CUcFunctionCommon::DeleteRightSpace(CString strData)
{
	// マルチ バイト文字セットの環境では、CString.Trim***()にて、
	// 全角文字の削除ができない。
	// 本関数は、この対応のために使用する。
	// 
	// 「マルチ バイト文字セット」、「Unicode文字セット」
	// どちらでも同じ結果を得る。
	// 
	// CUcFunctionCommon::Trim()は文字列中のすべてのスペースを
	// 削除する関数であり、本関数とは動作が異なる。

	// 1文字＝2バイトの文字セットに変換する。
	// 外字も文字化けなく変換できる。
	CStringW strWork = (CStringW)strData;

	// トリム後、マルチ バイト文字セットに再変換する。
	return (CString)strWork.TrimRight();
}//DeleteRightSpace

// midori 190301 add -->
//******************************************************************************************
// 末尾の改行マークを削除する。
// 　【引数】	CString			strData			対象の文字列
// 　【戻値】	末尾スペース削除後の文字列
//******************************************************************************************
CString CUcFunctionCommon::DeleteRightCrlf(CString strData)
{
	int			cnt=0;
	int			lng=0;
	char		bf[128]={0};
	char		bf2[128]={0};
	CString		rcs=_T("");

	strcpy_s(bf,sizeof(bf),strData);
	if(strlen(bf) < 2)	{
		return(strData);
	}
	for(cnt=strlen(bf); cnt>=2; cnt=cnt-2)	{
		if(bf[cnt-1] == 0x0a && bf[cnt-2] == 0x0a)	{
			lng = lng + 2;
		}
	}
	memcpy(bf2,bf,strlen(bf)-lng);
	rcs.Format(_T("%s"),bf2);
	return(rcs);
}
// midori 190301 add <--

//******************************************************************************************
// 空行を削除する。
// 　【引数】	CStringArray*	pstrArray		対象の文字列配列のポインタ
// 　【戻値】	なし
//******************************************************************************************
void CUcFunctionCommon::DeleteNullRow(CStringArray* pstrArray)
{
	int	iLast = (int)pstrArray->GetCount() - 1;	// 文字列配列の最終インデックス
	
	for ( int i = 0; i < iLast; i++ ){
		// 1行目が空行か調べる。
		if ( pstrArray->GetAt( 0 ).IsEmpty() ){
			// 空行の場合は繰り上げる。
			pstrArray->RemoveAt( 0 );
			pstrArray->Add( NULL );
		}
		else{
			// 空行でない場合は処理を終了する。
			break;
		}
	}
}//DeleteNullRow

//******************************************************************************************
// 空行を削除する。
// 　【引数】	CString*		pstrTarget1		1行目の文字列のポインタ
//				CString*		pstrTarget2		2行目の文字列のポインタ
// 　【戻値】	なし
//******************************************************************************************
void CUcFunctionCommon::DeleteNullRow(CString* pstrTarget1, CString* pstrTarget2)
{
	if ( pstrTarget1->IsEmpty() ){
		*pstrTarget1 = *pstrTarget2;
		*pstrTarget2 = _T("");
	}
}//DeleteNullRow

//******************************************************************************************
//	文字列の分割関数(用途：名称→名称１、名称２への分割)
//	【引数】	CString			strTaget		分割を行う文字列
//				CStringArray*	pstrArray		分割された文字列を格納する文字列のポインタ
//				int				intRow			分割行数(2行に分割したい場合は2を指定)
//				BOOL			blnDelSpace		TRUE：分割した各行の末尾スペースを削除する
//				BOOL			blnDelNullRow	TRUE：空行を削除する
//	【戻値】	なし
//****************************************************************************************
void CUcFunctionCommon::StrDivision(CString strTarget, CStringArray* pstrArray, int intRow,
									BOOL blnDelSpace /*= FALSE*/,  BOOL blnDelNullRow /*= FALSE*/ )
{
	// 不当なサイズなら何もしない
	if( intRow <= 0 ){
		return;
	}

	int	nChCode	= 0;							// 改行コードの位置
	int nLen	= 0;							// 文字列の長さ
	int	iCnt	= 0;							// 編集先のインデックス
	int	iLast	= (intRow - 1);					// 格納先の最終インデックス

	// 格納先の初期化
	pstrArray->RemoveAll();
	pstrArray->SetSize( intRow );

	while( 1 ){

		// 改行コードの検索
		nChCode = strTarget.Find( _T("\r\n") );

		// 改行コードが見つからない場合は、文字列をそのまま格納
		if( nChCode < 0 ){

			if (blnDelSpace == TRUE){
				// 末尾スペースを削除
				pstrArray->SetAt( iCnt , DeleteRightSpace( strTarget ) );
			}
			else
			{
				pstrArray->SetAt( iCnt , strTarget );
			}
			break;
		}

		// 改行コードより前の文字列を格納
		if (blnDelSpace == TRUE){
			// 末尾スペースを削除
			pstrArray->SetAt( iCnt, DeleteRightSpace( strTarget.Left( nChCode ) ) );
		}
		else
		{
			pstrArray->SetAt( iCnt, strTarget.Left( nChCode ) );
		}

		// 次のターゲット文字列を作成
		nLen = strTarget.GetLength();
		strTarget = strTarget.Right( nLen - (nChCode + 2) );	// 「2」は、改行コードの文字数

		if ( iCnt == iLast ){
			// 最終行なら終了
			break;
		}
		else{
			// 次行へ
			iCnt++;
		}
	}

	if (blnDelNullRow == TRUE){
		// 空行削除
		DeleteNullRow( pstrArray );
	}
}//StrDivision

//******************************************************************************************
// 複数の文字列を改行コード（CR+LF）で連結する。
//　【引数】	CStringArray*	pstrArray		連結する文字列配列のポインタ
//　【戻値】	連結された文字列
//******************************************************************************************
CString CUcFunctionCommon::StrDocking(CStringArray* pstrArray)
{
	int nCount = (int)pstrArray->GetCount();	// 連結する文字列の数
	CString	strData	= _T("");	// 連結後の文字列

	// 文字列を連結する。
	for ( int i = 0; i < nCount; i++ ){
		strData +=  pstrArray->GetAt( i ) + _T("\r\n");
	}

	// 末尾の改行コードを削除する。
	CStringW strWork = (CStringW)strData;
	strWork.TrimRight( L"\r\n" );

	return (CString)strWork;
}//StrDocking

//******************************************************************************************
// 複数の文字列を改行コード（CR+LF）で連結する。
//	【引数】	CString			strTaget1		連結する文字列1
//				CString			strTaget2		連結する文字列2
//	【戻値】	連結された文字列
//****************************************************************************************
CString CUcFunctionCommon::StrDocking(CString strTarget1, CString strTarget2)
{
	CString	strData	= _T("");	// 連結後の文字列

	// 全ての文字列がNULLの場合、改行文字は付加しない
	if ( strTarget1 == "" && strTarget2 == "" ){
		return strData;
	}

	strData = strTarget1 + _T("\r\n") + strTarget2;

	// 末尾の改行コードを削除する。
	CStringW strWork = (CStringW)strData;
	strWork.TrimRight( L"\r\n" );

	return (CString)strWork;
}//StrDocking

//********************************************************************************
//	ICS-ControlのDIAGRAM_DATA初期化
//　【引数】	DIAGRAM_DATA*	diadata			データ
//　【戻値】	なし
//********************************************************************************
void CUcFunctionCommon::DiagInit(DIAGRAM_DATA* diadata)
{
	// 初期化
	diadata->data_edit.Empty();
	diadata->data_disp.Empty();
	diadata->data_combo = 0;
	diadata->data_check = 0;
	memset( diadata->data_val, '\0', sizeof(diadata->data_val));
	memset( diadata->data_day,'\0', sizeof(diadata->data_day));
	diadata->data_imgdata = (char*)NULL;
	diadata->data_imgwidth = 0;
	diadata->data_imgheight = 0;
	diadata->text_combo.Empty();
	diadata->text_check.Empty();
	diadata->movepos = 0;
}//DiagInit

//********************************************************************************
//	ICS-Controlから文字列を取得
//　【引数】	CICSDiag*		pDiag			対象コントロール
//				int				intIndex		対象コントロールのインデックス番号
//　【引数】	対象コントロールの文字列
//********************************************************************************
CString CUcFunctionCommon::DiagGetString(CICSDiag* pDiag, int intIndex)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	// 初期化
	DiagInit( &diadata );
	
	// 文字列取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
	
	// 取得文字列返送
	return ( diadata.data_edit );
}//DiagGetString

// インボイス登録番号追加対応_23/11/06 add -->
//********************************************************************************
//	ICS-Controlから文字列を取得
//　【引数】	CICSDiag*		pDiag			対象コントロール
//				int				intIndex		対象コントロールのインデックス番号
//　【引数】	対象コントロールの文字列
//********************************************************************************
CString CUcFunctionCommon::DiagGetDisp(CICSDiag* pDiag, int intIndex)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	// 初期化
	DiagInit( &diadata );
	
	// 文字列取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
	
	// 取得文字列返送
	return ( diadata.data_disp );
}//DiagGetDisp

//********************************************************************************
//	ICS-Controlからコンボボックスの選択値を取得
//　【引数】	CICSDiag*		pDiag			対象コントロール
//				int				intIndex		対象コントロールのインデックス番号
//　【引数】	対象コントロールの文字列
//********************************************************************************
short CUcFunctionCommon::DiagGetCombo(CICSDiag* pDiag, int intIndex)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	// 初期化
	DiagInit(&diadata);

	// 文字列取得
	pDiag->GetData(intIndex, (LPUNKNOWN)&diadata);

	// 取得文字列返送
	return (diadata.data_combo);
}//DiagGetCombo
// インボイス登録番号追加対応_23/11/06 add <--

//********************************************************************************
//	ICS-Controlの文字列を設定
//　【引数】	CICSDiag*		pDiag			対象コントロール
//				int				intIndex		対象コントロールのインデックス番号
//				int				flg				表示用か入力用か
//				CString			str				入力文字列
//　【引数】	int				成功:0 失敗:-1
//********************************************************************************
int CUcFunctionCommon::DiagSetString(CICSDiag* pDiag, int intIndex, int flg, CString str)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	// 初期化
	DiagInit( &diadata );
	
	// 全ダイアグラムデータ取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	if( flg == ID_DIAGDATA_DISP ){
		diadata.data_disp = str;
	}
	else{
		diadata.data_edit = str;
	}
	
	// ダイアグラムデータセット
	return ( pDiag->SetData( intIndex, (LPUNKNOWN)&diadata ));
}//DiagSetString

//****************************************************************
//	文字列を最大入力文字で分割
//	【引数】	CString			chrData			分割元の文字列
//				CStringArray*	strArry			分割文字列の格納先
//				int				intMax			分割を行う文字数(1行の文字数)
//				int				intRow			分割行数
//	【戻値】	なし
//*****************************************************************
void CUcFunctionCommon::StrDivisionEx( CString strData , CStringArray* strArry , int intMax , int intRow )
{
	CString strBuff		= "";	// バッファ
	char	chrBuff[200];		// 文字化け防止のためのバッファ
	int		intTrueMax	= 0;	// 全角/半角を考慮した分割位置数
	int		intStart	= 0;	// 取り出し位置
	int		intLen		= 0;	// 文字列のサイズ
	int		intCnt		= 0;	// カウント変数1
	int		intbbRet	= 0;	// _ismbbleadの戻り値(0以外：文字がマルチバイト文字の先行バイト　0：文字が先行バイト以外)
	int		intbsRet	= 0;	// _ismbsleadの戻り値(-1:文字が先行バイト -1以外：文字が先行バイト以外)
	int		iLast		= (intRow - 1);	// 格納先の最終インデックス

	// 格納先の初期化
	strArry->RemoveAll();
	if ( intRow <= 0 ){
		strArry->SetSize( 1 );
	}
	else{
		strArry->SetSize( intRow );
	}

	// 行数分ループ
	for ( intCnt = 0;intCnt < intRow;intCnt++ ){

		// 空欄は空白のみ入れる(改行文字は無し)
		if ( strData != "" ){
		
			// バッファクリア
			ZeroMemory( chrBuff , sizeof(char[200]));

			strcpy_s( chrBuff , sizeof(char[200]) , (const char *)strData );

			// 分割位置の文字列がマルチバイト文字の先行バイト?(0以外?)
			intbbRet = _ismbblead( (unsigned int)chrBuff[(intMax*2)-1] );

			// 分割位置の文字が先行バイト?(-1?)
			intbsRet = _ismbslead( (const unsigned char*)&chrBuff[0] , (const unsigned char*)&chrBuff[(intMax*2)-1] );

			// 文字列判断(全角?半角?)
			if ( ( intbbRet != 0 ) && ( intbsRet == -1 ) ){
				// 分割位置の文字列が全角2バイト文字の1バイト目なら1バイト前の文字で分割
				intTrueMax = ( intMax * 2 ) - 1;
			}
			else{
				intTrueMax = ( intMax * 2 );
			}

			// 指定文字数分取得
			strBuff = strData.Left( intTrueMax );

			// 現在の文字列のサイズを取得
			intLen = strData.GetLength();

			// 格納したデータ分は切り詰める
			strData = strData.Mid( intTrueMax  , intLen );

			// 「intCnt」行目のデータを格納
			strArry->SetAt( intCnt , DeleteRightSpace( strBuff ) );
		}
		else{
			strArry->SetAt( intCnt , "" );
		}	
	}

	// 空行削除
	DeleteNullRow( strArry );
}//StrDivisionEx

//****************************************************************
//	ダイアログを画面の作業領域で中央揃え
//	【引数】	CWnd*			対象ダイアログのハンドル
//	【戻値】	なし
//*****************************************************************
void CUcFunctionCommon::DlgMoveCenter(CWnd* pParent)
{

	// 2010/08/30 業務統合メニュー マルチウィンドウ対応
	int nMode = GetMultiWindowType();
	switch ( nMode ) {
	case MULTIWINDOW_NOP:		// 制御なしモード
	case MULTIWINDOW_NONE:		// 業務選択でマルチウィンドウ制御無効
		// 従来通り 最大化で起動するため
		// センタリングを行う（Windows7 対応を実施）
		break;
	case MULTIWINDOW_FULL:		// フルコントロール
	case MULTIWINDOW_SIMPLE:	// 簡易コントロールモード
	default:
		// アプリ側でのセンタリングは実施しない
		//
		//	マルチウィンドウ対応により、下記機能がモジュールにて提供される。
		//	よって、本ケースの場合、Windows7対応の条件に当てはまらない。 
		//
		//	　問題：解像度によってはタスクバーにはみ出る形で表示されるため(例：Windows7 1366×768)中央表示を行う
		//
		// ・メイン画面は、デスクトップ解像度より小さいサイズで起動。
		// ・ICSモジュールにて、ダイアログは、メイン画面のサイズに合わせて縮小表示を行う
		// ・ICSモジュールにて、呼び出し元ウィンドウの中央に表示を行う。
		return;
	}

	CRect	rectPrimaryWork, rectWnd;
	CPoint	ptCenter;

	// RECT構造体を初期化します
	rectPrimaryWork.SetRectEmpty();
	rectWnd.SetRectEmpty();
	// POINT構造体を初期化します
	ptCenter.SetPoint(0, 0);

	// プライマリモニタの作業領域を取得します
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectPrimaryWork, 0);
	// 自身の画面サイズを取得します
	pParent->GetWindowRect(&rectWnd);

	// プライマリモニタの作業領域の中央位置に該当する右上座標を算出します
	ptCenter.x = max(0, rectPrimaryWork.left + (rectPrimaryWork.Width() - rectWnd.Width()) / 2);
	ptCenter.y = max(0, rectPrimaryWork.top + (rectPrimaryWork.Height() - rectWnd.Height()) / 2);
	
	// プライマリモニタの作業領域の中央に移動します
	pParent->MoveWindow(ptCenter.x, ptCenter.y, rectWnd.Width(), rectWnd.Height(), TRUE);

	return;
}//DlgMoveCenter

//****************************************************************
//	グループNOを取得する
//	【引数】	int				戻値のアルファベットが含まれる場合の大文字か小文字か　0:大文字 ／ 1:小文字()
//	【戻値】	CString			GrNo("1B", "1f"...)
//*****************************************************************
CString CUcFunctionCommon::GetGrNo( BOOL type/* = 0*/ )
{
	char			szGrNum[16];
	CString			strGrNum, strBuff;

	memset( szGrNum, '\0', sizeof(szGrNum) );		// バッファ初期化

	PCOM_GetString( _T( "ExecGrJobs" ), szGrNum );
	strBuff.Format( "%s", szGrNum );

	if( type == 1 ){
		strGrNum = strBuff.MakeLower();					// 大文字→小文字変換
	}
	else{
		strGrNum = strBuff;
	}

	return strGrNum;
}

//***************************************************************************
//	ファイル名に業種Noを付与する。財務DBの場合は従来のファイルを使用する。
//
//	IN:		CString			ファイル(SQL,SCH)
//
//	RET:	CString			GrNoがファイル名の末尾に付与されたファイル名。エラー時にはNULL			
//***************************************************************************
CString CUcFunctionCommon::GetFileNameAddGR(CString strFile)
{
	CString strGrNum;

	// GrNo取得
	strGrNum = GetGrNo();

	// GrNo取得失敗のときはNULLを戻す
	if( strGrNum.IsEmpty() != FALSE ){
		return strGrNum;
	}

	// 1Bの場合は従来のファイルを使用
	if( strGrNum.Compare( STR_GYOUSHU_ZAIMU ) == 0 ){
		return strFile;
	}

	// GrNo挿入
	int nRet = strFile.Insert( strFile.Find( "." ), _T("GR") + strGrNum );

	if( nRet > 0 ){
		return strFile;
	}
	
	return "";
}

////////////////////////////////////////////////////////////////////////////////
// GetDividedBkName()
//
// ■指定された文字数目で分割（先行バイト、後続バイトの判断も行う）
//
// 引数		：CString		分割する文字列
//			：CString		分割された文字列（前半）
//			：CString		分割された文字列（後半）
//			：int			分割するバイト数（必ずbkname[]の半分以下）
//
// 戻り値	：int			0
//
////////////////////////////////////////////////////////////////////////////////
//int CUcFunctionCommon::GetDividedName(CString strBase, CString& strResult1, CString& strResult2, int nDivideByte/*必ずbkname[]の半分以下*/)
//{
//	int one_line_char_num = nDivideByte*2;
//	unsigned char bkname[128] = {0};
//	int max_size = (sizeof(bkname) / sizeof(bkname[0])) - 1/*NULL文字を入れる為の-1*/;
//	if(one_line_char_num > max_size)	return -1;
//	int strBaseLen = strBase.GetLength();
//	if(strBaseLen < max_size)	max_size = strBaseLen;	// bknameとstrBaseで小さいほうのサイズを採用
//
//	memmove(bkname,(LPCSTR)strBase, max_size);
//
//	strResult1.Empty();
//	strResult2.Empty();
//
//	if( _ismbstrail(bkname, &bkname[nDivideByte*2]) ){
//		one_line_char_num--;
//	}
//
//	strResult1 = strBase.Left( one_line_char_num );
//	strBase.Delete( 0, one_line_char_num );
//	strResult2 = strBase;
//
//	return 0;
//}

////////////////////////////////////////////////////////////////////////////////
// GetDividedName()
//
// ■CprtJobFunction::GetDividedBkNameとほぼ同様
//
// 引数		：CString		分割する文字列
//			：CString		分割された文字列（前半）
//			：int			分割するバイト数（必ずname[]の半分以下）
//
// 戻り値	：int			0
//
////////////////////////////////////////////////////////////////////////////////
//int CUcFunctionCommon::GetDividedName(CString strBase, CString& strResult, int nDivideByte/*必ずname[]の半分以下*/)
//{
//	int one_line_char_num = nDivideByte*2;
//	unsigned char name[128] = {0};
//	int max_size = (sizeof(name) / sizeof(name[0])) - 1/*NULL文字を入れる為の-1*/;
//	//if(one_line_char_num > (sizeof(name)-1))	return -1;
//	//memmove(name,(LPCSTR)strBase, sizeof(name)-1/*NULL文字を入れる為の-1*/); // (sizeof(name)-1)なんか使用すると未処理の例外が発生する GetDividedName()内も同様
//	if(one_line_char_num > max_size)	return -1; 
//	int strBaseLen = strBase.GetLength();
//	if(strBaseLen < max_size)	max_size = strBaseLen;	// bknameとstrBaseで小さいほうのサイズを採用
//
//	memmove(name,(LPCSTR)strBase, max_size);
//
//	strResult.Empty();
//
//	if( _ismbstrail(name, &name[nDivideByte*2]) ){
//		one_line_char_num--;
//	}
//
//	strResult = strBase.Left( one_line_char_num );
//
//	return 0;
//}

////////////////////////////////////////////////////////////////////////////////
// ■レポートコントロールの全選択/全解除制御
//　１つでもチェックが入っていれば全てオフにし、
//　１つもチェックが入っていないときのみ、全てオンにする
//
// 引数		：ICSReportControl*		対象となるレポートコントロール
//			：int					カラム
//
// 戻り値	：なし
//
////////////////////////////////////////////////////////////////////////////////
void CUcFunctionCommon::ChangeReportColumnAll(ICSReportControl* report, int clm)
{
	if(report == NULL)	return;
	ICSReportRows *rrs = report->GetRows();
	if(rrs == NULL)	return;
	int cnt_max = rrs->GetCount();
	

	// チェックが入っている数取得
	int cnt = GetReportCheckNum(report, clm, cnt_max);

	// チェック数が０以外は全てオフ
	if( cnt > 0 ){
		for(int i=0; i<cnt_max; i++){
			report->SetChecked(i, clm, FALSE);
		}
	}
	else{
		for(int i=0; i<cnt_max; i++){
			report->SetChecked(i, clm, TRUE);
		}
	}

	report->Populate();
}

////////////////////////////////////////////////////////////////////////////////
// ■引数で指定されたカラムのチェックボックスのチェック数をカウント
//
// 引数		：ICSReportControl*		対象となるレポートコントロール
//			：int					カラム
//			：int					最大行数
//
// 戻り値	：int					チェックされているレコード数
//
////////////////////////////////////////////////////////////////////////////////
int CUcFunctionCommon::GetReportCheckNum(ICSReportControl* report, int clm, int cnt_max)
{
	if(report == NULL)	return -1;
	if(cnt_max == 0){
		ICSReportRows *rrs = report->GetRows();
		if(rrs == NULL)	return -1;
		cnt_max = rrs->GetCount();
	}
	int cnt = 0;

	// チェックが入っている数取得
	for(int i=0; i<cnt_max; i++){
		if( report->IsChecked( i, clm ) == TRUE )	cnt++;
	}

	return cnt;
}

////////////////////////////////////////////////////////////////////////////////
// ■現在の様式から前/後ろの様式にデータがあるかチェック
//
// 引数		：pdb			CDatabase
//			：mode			TRUE:前　FALSE:後ろ
//			：FormSeq		現在のFormSeq or FormOrder return_typeによる
//			：return_type	1:FormSeq 2:FormOrder
//
// 戻り値	：int					チェックされているレコード数
//
////////////////////////////////////////////////////////////////////////////////
int CUcFunctionCommon::IsExistData(CDatabase* pdb, int index, BOOL mode, int return_type)
{
	int					nRet			= -1;
	int					nFormOrder		= 1;
	CdbUcInfSub			mfcRecSub(pdb);				//	uc_inf_subテーブル
	CdbUc000Common*		rsData			= NULL;		// テーブルアクセスクラス オブジェクトへのポインタ

	// FormOrder番号を取得
	if(return_type == 1){
		if ( mfcRecSub.RequeryFormSeq(index) == DB_ERR_OK ) {
			//	レコードあり？
			if ( !( mfcRecSub.IsEOF() ) ){
				mfcRecSub.MoveFirst();					//	レコードを先頭に移動	
				nFormOrder = mfcRecSub.m_FormOrder;
			}
		}
	}
	else	nFormOrder = index;

	if( mode != FALSE ){	// 現在の様式より前
		for(int i= nFormOrder - 1; i > 0; i--){
			mfcRecSub.RequeryFormOrder(i);
			if(!mfcRecSub.IsEOF()){
				rsData = CreateDbObject(mfcRecSub.m_FormSeq, pdb);
				// データが入力されている様式が一つでもあったら、ループを抜ける
// midori 160607 cor -->
				//if( rsData->GetCountDataRecord() != 0 ){
// ---------------------
				if( rsData->GetCountDataRecord(1) != 0 ){
// midori 160607 cor <--
					if(return_type == 1)		nRet = mfcRecSub.m_FormSeq;
					else if(return_type == 2)	nRet = mfcRecSub.m_FormOrder;

					rsData->Fin();
					delete rsData;
					break;
				}
				rsData->Fin();
				delete rsData;
			}
		}
	}
	else{	// 現在の様式より後ろ
		for(int i=nFormOrder + 1; i<UC_ID_FORMNO_MAX + 1; i++){
			mfcRecSub.RequeryFormOrder(i);
			if(!mfcRecSub.IsEOF()){
				rsData = CreateDbObject(mfcRecSub.m_FormSeq, pdb);
				// データが入力されている様式が一つでもあったら、ループを抜ける
// midori 160607 cor -->
				//if( rsData->GetCountDataRecord() != 0 ){
// ---------------------
				if( rsData->GetCountDataRecord(1) != 0 ){
// midori 160607 cor <--
					if(return_type == 1)		nRet = mfcRecSub.m_FormSeq;
					else if(return_type == 2)	nRet = mfcRecSub.m_FormOrder;

					rsData->Fin();
					delete rsData;
					break;
				}
				rsData->Fin();
				delete rsData;
			}
		}
	}
	mfcRecSub.Fin();

	return nRet;
}

//********************************************************************************
//	新財務のFormSeq番号から、データベース操作クラスのオブジェクト取得
//　※本関数をCUcFunctionCommonに定義（CdbUc171Sonota1、CdbUc181Sonota2を使用）したことにより、
//　その他の内訳書（dbUc171Sonota1、dbUc181Sonota2）をUCHIIMPプロジェクトに追加した。
//　ただし、UCHIIMPではその他の内訳書は未対応！！！
//		【引数】	intFormSeq		新財務のFormSeq番号
//		【戻値】	CdbUc000Common*	新財務のデータベース操作クラスのオブジェクト
//********************************************************************************
CdbUc000Common*	CUcFunctionCommon::CreateDbObject(int intFormSeq, CDatabase* pdb)
{
	switch (intFormSeq) {
	case ID_FORMNO_011:
		return (CdbUc000Common*)(new CdbUc011Yotyokin(pdb));
	case ID_FORMNO_021:
		return (CdbUc000Common*)(new CdbUc021Uketoritegata(pdb));
	case ID_FORMNO_031:
		return (CdbUc000Common*)(new CdbUc031Urikakekin(pdb));
	case ID_FORMNO_041:
		return (CdbUc000Common*)(new CdbUc041Karibarai(pdb));
	case ID_FORMNO_042:
		return (CdbUc000Common*)(new CdbUc042Kasituke(pdb));
	case ID_FORMNO_051:
		return (CdbUc000Common*)(new CdbUc051Tanaoroshi(pdb));
	case ID_FORMNO_061:
		return (CdbUc000Common*)(new CdbUc061Yuukasyouken(pdb));
	case ID_FORMNO_071:
		return (CdbUc000Common*)(new CdbUc071Koteishisan(pdb));
	case ID_FORMNO_081:
		return (CdbUc000Common*)(new CdbUc081Siharaitegata(pdb));
	case ID_FORMNO_091:
		return (CdbUc000Common*)(new CdbUc091Kaikakekin(pdb));
	case ID_FORMNO_101:
		return (CdbUc000Common*)(new CdbUc101Kariukekin(pdb));
	case ID_FORMNO_102:
		return (CdbUc000Common*)(new CdbUc102Gensen(pdb));
	case ID_FORMNO_111:
		return (CdbUc000Common*)(new CdbUc111Kariirekin(pdb));
	case ID_FORMNO_121:
		return (CdbUc000Common*)(new CdbUc121Totiuriage(pdb));
	case ID_FORMNO_131:
		return (CdbUc000Common*)(new CdbUc131Uriagedaka(pdb));
	case ID_FORMNO_141:
		return (CdbUc000Common*)(new CdbUc141Yakuin(pdb));
	case ID_FORMNO_142:
		return (CdbUc000Common*)(new CdbUc142Jinkenhi(pdb));
	case ID_FORMNO_151:
		return (CdbUc000Common*)(new CdbUc151Tidaiyatin(pdb));
	case ID_FORMNO_152:
		return (CdbUc000Common*)(new CdbUc152Kenrikin(pdb));
	case ID_FORMNO_153:
		return (CdbUc000Common*)(new CdbUc153Kougyou(pdb));
	case ID_FORMNO_161:
		return (CdbUc000Common*)(new CdbUc161Zatueki(pdb));
	case ID_FORMNO_162:
		return (CdbUc000Common*)(new CdbUc162Zatuson(pdb));

	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:
		return (CdbUc000Common*)(new CdbUc171Sonota1(pdb, intFormSeq));

	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:
		return (CdbUc000Common*)(new CdbUc181Sonota2(pdb, intFormSeq));

	default:
		return (CdbUc000Common*)NULL;
	}
}

//**************************************************
//	フォーカスのあるコントロールのID取得
//	【引数】	pParent		…	親ハンドル
//	【戻値】	0以外		…	コントロールID
//				0			…	失敗
//**************************************************
int CUcFunctionCommon::GetFocusControlID( CWnd* pParent /*=NULL*/)
{
	int		nRet = 0;			//	戻値
	CWnd*	pControl = NULL;	//	コントロール
	
	//	親ハンドルあり？
	if ( pParent != NULL ){

		//	フォーカスのあるコントロールを取得
		pControl = pParent->GetFocus();
		
		//	コントロール取得成功？
		if ( pControl != NULL ){
			//	コントロールID取得
			nRet = pControl->GetDlgCtrlID();
		}
	}

	//	戻値を返す
	return( nRet );
}


// midori 153907 del -->
////**************************************************
////	改行コードを無視して比較する
////	【引数】	_str1		…	比較対象１
////				_stt2		…　比較対象２
////	【戻値】	0以外		…	改行を無視した結果、不一致
////				0			…	改行を無視した結果、一致
////**************************************************
//int CUcFunctionCommon::CompareNoCRLF(const CString& _str1, const CString& _str2)
//{
//	CString str1 = _str1;
//	CString str2 = _str2;
//
//	str1.Replace(_T("\r"), _T(""));
//	str1.Replace(_T("\n"), _T(""));
//
//	str2.Replace(_T("\r"), _T(""));
//	str2.Replace(_T("\n"), _T(""));
//
//	return str1.Compare(str2);
//}
// midori 153907 del <--

// midori 152137 add -->
//**************************************************
//	ソート項目（カナ入力有り）の種類を取得
//	【引数】	intFormSeq  …　新財務のFormSeq番号
//				intItemSeq	…　ソート項目のシーケンス番号
//	【戻値】	1			…	科目
//				2			…	銀行
//				3			…	取引先
//				0			…	上記以外
//**************************************************
int CUcFunctionCommon::GetSortItem(int intFormSeq,int intItemSeq)
{
	int ret = 0;		// 戻り値

	switch (intFormSeq) {
	case ID_FORMNO_011:
		// 金融機関
		if(intItemSeq == 1)			ret = 2;
		// 種類
		else if(intItemSeq == 2)	ret = 1;
		break;
	case ID_FORMNO_021:
		// 振出人
		if(intItemSeq == 1)			ret = 3;
		// 科目
		else if(intItemSeq == 8)	ret = 1;
		break;
	case ID_FORMNO_031:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先(名称)
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_041:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先(名称)
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_042:
		// 貸付先
		if(intItemSeq == 1)			ret = 3;
		break;
	case ID_FORMNO_051:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_061:
		// 区分
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_071:
		break;
	case ID_FORMNO_081:
		// 支払先
		if(intItemSeq == 1)			ret = 3;
// 改良No.21-0086,21-0529 add -->
		// 科目
		else if(intItemSeq == 9)	ret = 1;
// 改良No.21-0086,21-0529 add <--
		break;
	case ID_FORMNO_091:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_101:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 2)	ret = 3;
		break;
	case ID_FORMNO_102:
		// 所得の種類
		if(intItemSeq == 3)			ret = 1;
		break;
	case ID_FORMNO_111:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 借入先
		else if(intItemSeq == 2)	ret = 2;
		break;
	case ID_FORMNO_121:
		// 区分
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_131:
		break;
	// ソート無し
	case ID_FORMNO_141:
	case ID_FORMNO_142:
		break;
	case ID_FORMNO_151:
		// 地代・家賃の区分
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_152:
		break;
	case ID_FORMNO_153:
		break;
	case ID_FORMNO_161:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 3)	ret = 3;
		break;
	case ID_FORMNO_162:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		// 相手先
		else if(intItemSeq == 3)	ret = 3;
		break;
	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		break;
	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:
		// 科目
		if(intItemSeq == 1)			ret = 1;
		break;
	default:
		break;
	}

	return(ret);
}

int CUcFunctionCommon::IsKamokuSort(int nFormSeq, CDatabase* pdb)
{
	int			nRet = 0;
	CdbUcInfSub	mfcRecSub( pdb);

	if( mfcRecSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		if( !(mfcRecSub.IsEOF()) ){
			if( mfcRecSub.m_GeneralVar[2] & BIT_D1 ){
				CCtrlSubGeneralVar cs( mfcRecSub );
				if( cs.bKamokuSort )	nRet = 1;
				else					nRet = 0;
			}
		}
		mfcRecSub.Fin();
	}
	return(nRet);
}

//**************************************************
//	５０音順（銀行、取引先）を取得
//	【引数】	nFormSeq	…　新財務のFormSeq番号
//				CDatabase	…　CDatabase
//				
//**************************************************
void CUcFunctionCommon::IsSort(int nFormSeq, CDatabase* pdb,int* rbank,int* raddress)
{
	CdbUcInfMain	mfcRecMain(pdb);

	*rbank = 0;
	*raddress = 0;

	if( mfcRecMain.Init() == DB_ERR_OK ){
		if( !(mfcRecMain.IsEOF()) ){
			if( mfcRecMain.m_GeneralVar[1] & BIT_D5 )	*rbank = 1;
// midori 152745 del -->
			//if( mfcRecMain.m_GeneralVar[1] & BIT_D6 )	*raddress = 1;
// midori 152745 del <--
// midori 152745 add -->
			switch(nFormSeq)	{
				// ② 受取手形の内訳書
				case	ID_FORMNO_021:
				// ③ 売掛金（未収入金）の内訳書
				case	ID_FORMNO_031:
				// ④－２ 貸付金及び受取利息の内訳書
				case	ID_FORMNO_042:
				// ⑩－１ 仮受金（前受金・預り金）の内訳書
				case	ID_FORMNO_101:
				// ⑯－２ 雑損失等の内訳書
				case	ID_FORMNO_162:
					// 得意先を参照
					if(mfcRecMain.m_GeneralVar[1] & BIT_D7)	*raddress = 1;
					break;
				// ④－１ 仮払金（前渡金）の内訳書
				// ⑧ 支払手形の内訳書
				// ⑨ 買掛金（未払金・未払費用）の内訳書
				// ⑯－１ 雑益、雑損失等の内訳書
				default:
					// 仕入先を参照
					if(mfcRecMain.m_GeneralVar[1] & BIT_D6)	*raddress = 1;
					break;
			}
// midori 152745 add <--
		}
		mfcRecMain.Fin();
	}
}
// midori 152137 add <--

// midori 180104 add -->
//-----------------------------------------------------------------------------
// 会社情報の取込み
//-----------------------------------------------------------------------------
// 引数	type	：	表示タイプ	(0:変更分取込み, 1:全て取込み)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CUcFunctionCommon::SetCpdataByCpinfo( int type, CWnd* pWnd )
{

	int			st = 0;
	CDBPrtbl	pt;
	CDBZmSub*	pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;		// 財務クラス取得

	pt.SetMasterDB(pZmSub->m_database);

	// 会社情報
	if( pt.CpdataOpen() == -1 ){
		ICSMessageBox( pt.GetLastErrMsg(), MB_ICONSTOP );
		return(-1);
	}

	// zvolumeテーブル参照
	if (pZmSub->VolumeOpen() != 0) {
		// エラー
		ZmSubErrMsg( pZmSub );
		return(-1);
	}

	// 会社コード
	unsigned char	cocode[4];
	int_bcd( cocode, pZmSub->zvol->v_cod, 4 );
	CCoinfCtl	coinfctl;

	char	sdev[16]={0}, tmpdev[16]={0};
	PCOM_GetString( _T("MasterDevice"), tmpdev );
	wsprintf( sdev, _T("%s:"), tmpdev );

	if( coinfctl.OpenFile(1, sdev) == FALSE ){
		// 環境によっては、新総合会社情報ファイルが存在しないことがあるので！
		pt.CpdataClose();
		pt.cpdata = NULL;
		pZmSub->VolumeClose();
		return(0);
	}

	st = 0;
	if( (st=coinfctl.IsRegistMcode(1, pZmSub->zvol->apno, 0, cocode, NULL)) == 1 ){

//		// 総合会社情報に登録有り
//		m_hasCoinfo = TRUE;

		// 該当のものが法人or個人？
		CINF_REC	cinfrec;
		CTBL_PAR	ctblpar;
		char		moji[128];
		memset( &cinfrec, '\0', sizeof(CINF_REC) );
		memset( &ctblpar, '\0', sizeof(CTBL_PAR) );
		ctblpar.tpDtyp = 1;
		ctblpar.tpMtyp = 0;
		ctblpar.tpApno = pZmSub->zvol->apno;
		memmove( ctblpar.tpCcod, cocode, sizeof(cocode) );

		if( coinfctl.FindRecFromMcod(0, &cinfrec, &ctblpar) == 1 ){

			NCOF_PAR	ncofpar;
			if( type == 0 ){
				memset( &cinfrec, '\0', sizeof(CINF_REC) );
			}
			memset( &ncofpar, '\0', sizeof(NCOF_PAR) );
			ncofpar.cpPare = pWnd;
			ncofpar.cpMtyp = 0;
			ncofpar.cpDtyp = 1;
			ncofpar.cpApno = pZmSub->zvol->apno;
			if( ctblpar.tpHorK == 0 ){
				ncofpar.cpEflg[0] |= (0x00000004|0x00001000|0x00004000);
			}
			else if( ctblpar.tpHorK == 1 ){
// midori 7 del -->
				//ncofpar.cpEflg[0] |= (0x00000080|0x00001000|0x00004000);
// midori 7 del <--
// midori 7 add -->
				ncofpar.cpEflg[0] |= (0x00001000|0x00004000);
// midori 7 add <--
			}
			memmove( ncofpar.cpCode, cocode, sizeof(cocode) );

			long	nFunc = 0;
			if( type == 0 ){
				nFunc |= 0x10;
			}
			else if( type == 1 ){
				nFunc |= (0x10|0x20);
			}

// 顧問先情報表示対応 add -->
			CINF_REC	cinfnow;
			CString		cst;

			// ---------------------------------------------------------------------
			// 現在表示している表紙の情報をセット
			// ---------------------------------------------------------------------
			memset(&cinfnow, '\0', sizeof(CINF_REC));
			//memmove(&cinfnow, &cinfrec, sizeof(CINF_REC));

			for(int ii=0; ; ii++)	{
				if(ii == 0)	{
					if((pt.cpdata->MoveFirst() == -1) || (pt.cpdata->st == -1))	{
						break;
					}
				}
				else	{
					if((pt.cpdata->MoveNext() == -1) || (pt.cpdata->st == -1))	{
						break;
					}
				}

				// 当期のみでOK？
				if(pt.cpdata->cpkofs == 0)	{
					// 会社名
					cst = pt.cpdata->cpyg;
					cst.TrimRight();
					memset(moji, 0, sizeof(moji));
					sprintf_s(moji, 128, _T("%s"), cst);
					memcpy(cinfnow.ciName, moji, cst.GetLength());

					// 住所（上段）
					cst = pt.cpdata->cpua;
					cst.TrimRight();
					memset(moji, 0, sizeof(moji));
					sprintf_s(moji, 128, _T("%s"), cst);
					memcpy(cinfnow.ciAdrs1, moji, cst.GetLength());

					// 住所（下段）
					cst = pt.cpdata->cpda;
					cst.TrimRight();
					memset(moji, 0, sizeof(moji));
					sprintf_s(moji, 128, _T("%s"), cst);
					memcpy(cinfnow.ciAdrs2, moji, cst.GetLength());

					break;
				}
			}
			// ---------------------------------------------------------------------
// 顧問先情報表示対応 add <--

			coinfctl.SetMstDat(&cinfnow, KMNCTL_GKIND_KNJ);	// 顧問先情報表示対応 add
			if( coinfctl.GetCoinf(nFunc, &cinfrec, &ncofpar) == 0 ){
				if( ncofpar.cpRflg[0]&(0x00000004|0x00001000|0x00004000|0x00000080) ){
					// 更新
					for( int i=0;; i++ ){
						if( i==0 ){
							if( (pt.cpdata->MoveFirst()==-1) || (pt.cpdata->st==-1) ){
								break;
							}
						}
						else{
							if( (pt.cpdata->MoveNext()==-1) || (pt.cpdata->st==-1) ){
								break;
							}
						}
						// 当期のみでOK？
						if( pt.cpdata->cpkofs == 0 ){
							// 編集
							pt.cpdata->Edit();

							if( cinfrec.ciHKkbn == 0 ){		// 法人
								if( ncofpar.cpRflg[0]&0x04 ){
									// 会社名
									memset( moji, '\0', sizeof(moji) );
									memmove( moji, cinfrec.ciName, sizeof(cinfrec.ciName) );
									pt.cpdata->cpyg.Format( _T("%s"), moji );
								}

								if( ncofpar.cpRflg[0]&0x1000 ){	// 住所（上段）
									memset( moji, '\0', sizeof(moji) );
									memmove( moji, cinfrec.ciAdrs1, sizeof(cinfrec.ciAdrs1) );
									pt.cpdata->cpua.Format( _T("%s"), moji );
								}
								if( ncofpar.cpRflg[0]&0x4000 ){	// 住所（下段）
									memset( moji, '\0', sizeof(moji) );
									memmove( moji, cinfrec.ciAdrs2, sizeof(cinfrec.ciAdrs2) );
									pt.cpdata->cpda.Format( _T("%s"), moji );
								}
							}
							else if( cinfrec.ciHKkbn == 1 ){	// 個人
								if( ncofpar.cpRflg[0]&0x80 ){
									// 会社名
									memset( moji, '\0', sizeof(moji) );
									memmove( moji, cinfrec.ciYagou, sizeof(cinfrec.ciYagou) );
									pt.cpdata->cpyg.Format( _T("%s"), moji );
								}

								if( ncofpar.cpRflg[0]&0x1000 ){	// 住所（上段）
									memset( moji, '\0', sizeof(moji) );
									memmove( moji, cinfrec.ciAdrs1, sizeof(cinfrec.ciAdrs1) );
									pt.cpdata->cpua.Format( _T("%s"), moji );
								}
								if( ncofpar.cpRflg[0]&0x4000 ){	// 住所（下段）
									memset( moji, '\0', sizeof(moji) );
									memmove( moji, cinfrec.ciAdrs2, sizeof(cinfrec.ciAdrs2) );
									pt.cpdata->cpda.Format( _T("%s"), moji );
								}
							}

							pt.cpdata->Update();

							break;
						}
					}
				}
			}
		}
	}
	else if( (st!=-2) && (type==1) ){
		char		moji[128];
		CINF_REC	cinfrec;
		NCOF_PAR	ncofpar;

		memset( &cinfrec, '\0', sizeof(CINF_REC) );

		memset( &ncofpar, '\0', sizeof(NCOF_PAR) );
		ncofpar.cpPare = pWnd;
		ncofpar.cpMtyp = 0;
		ncofpar.cpDtyp = 1;
		ncofpar.cpApno = pZmSub->zvol->apno;
// midori 3 add -->
		// 内訳書（非連動）の場合は法人のみを表示する
// midori UC_0037 del -->
		//if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwInfoRegister == 1)	{
// midori UC_0037 del <--
// midori UC_0037 add -->
		if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 1)	{
// midori UC_0037 add <--
			ncofpar.cpHKsg = 1;
		}
// midori 3 add <--
		// 法人・個人の区別無く屋号と顧問先を選択対象とする
// midori 7 del -->
//		ncofpar.cpEflg[0] |= (0x00000080|0x00000004|0x00001000|0x00004000);
// midori 7 del <--
// midori 7 add -->
		ncofpar.cpEflg[0] |= (0x00000004|0x00001000|0x00004000);
// midori 7 add <--
		memmove( ncofpar.cpCode, cocode, sizeof(cocode) );

// 顧問先情報表示対応 add -->
		// ---------------------------------------------------------------------
		// 現在表示している表紙の情報をセット
		// ---------------------------------------------------------------------
		CINF_REC	cinfnow;
		CString		cst;

		memset(&cinfnow, '\0', sizeof(CINF_REC));

		for(int ii=0; ; ii++)	{
			if(ii == 0)	{
				if((pt.cpdata->MoveFirst() == -1) || (pt.cpdata->st == -1))	{
					break;
				}
			}
			else	{
				if((pt.cpdata->MoveNext() == -1) || (pt.cpdata->st == -1))	{
					break;
				}
			}

			// 当期のみでOK？
			if(pt.cpdata->cpkofs == 0)	{
				// 会社名
				cst = pt.cpdata->cpyg;
				cst.TrimRight();
				memset(moji, 0, sizeof(moji));
				sprintf_s(moji, 128, _T("%s"), cst);
				memcpy(cinfnow.ciName, moji, cst.GetLength());

				// 住所（上段）
				cst = pt.cpdata->cpua;
				cst.TrimRight();
				memset(moji, 0, sizeof(moji));
				sprintf_s(moji, 128, _T("%s"), cst);
				memcpy(cinfnow.ciAdrs1, moji, cst.GetLength());

				// 住所（下段）
				cst = pt.cpdata->cpda;
				cst.TrimRight();
				memset(moji, 0, sizeof(moji));
				sprintf_s(moji, 128, _T("%s"), cst);
				memcpy(cinfnow.ciAdrs2, moji, cst.GetLength());

				break;
			}
		}
		// ---------------------------------------------------------------------
// 顧問先情報表示対応 add <--

		long	nFunc = 0;
		coinfctl.SetMstDat(&cinfnow, KMNCTL_GKIND_KNJ);	// 顧問先情報表示対応 add
		// GetCoinf() 関数の第１引数には”０”を指定しないと正しく動かない！
		if( coinfctl.GetCoinf(nFunc, &cinfrec, &ncofpar) == 0 )	{
			if( ncofpar.cpRflg[0]&(0x00000004|0x00001000|0x00004000|0x00000080) )	{
				// 更新
				for( int i=0;; i++ ){
					if( i==0 ){
						if( (pt.cpdata->MoveFirst()==-1) || (pt.cpdata->st==-1) ){
							break;
						}
					}
					else{
						if( (pt.cpdata->MoveNext()==-1) || (pt.cpdata->st==-1) ){
							break;
						}
					}
					// 当期のみでOK？
					if( pt.cpdata->cpkofs == 0 ){
						// 編集
						pt.cpdata->Edit();

						if( cinfrec.ciHKkbn == 0 ){		// 法人
							if( ncofpar.cpRflg[0]&0x04 ){
								// 会社名
								memset( moji, '\0', sizeof(moji) );
								memmove( moji, cinfrec.ciName, sizeof(cinfrec.ciName) );
								pt.cpdata->cpyg.Format( _T("%s"), moji );
							}

							if( ncofpar.cpRflg[0]&0x1000 ){	// 住所（上段）
								memset( moji, '\0', sizeof(moji) );
								memmove( moji, cinfrec.ciAdrs1, sizeof(cinfrec.ciAdrs1) );
								pt.cpdata->cpua.Format( _T("%s"), moji );
							}
							if( ncofpar.cpRflg[0]&0x4000 ){	// 住所（下段）
								memset( moji, '\0', sizeof(moji) );
								memmove( moji, cinfrec.ciAdrs2, sizeof(cinfrec.ciAdrs2) );
								pt.cpdata->cpda.Format( _T("%s"), moji );
							}
						}
						else if( cinfrec.ciHKkbn == 1 ){	// 個人
							if( ncofpar.cpRflg[0]&0x80 ){
								// 会社名
								memset( moji, '\0', sizeof(moji) );
								memmove( moji, cinfrec.ciYagou, sizeof(cinfrec.ciYagou) );
								pt.cpdata->cpyg.Format( _T("%s"), moji );
							}

							if( ncofpar.cpRflg[0]&0x1000 ){	// 住所（上段）
								memset( moji, '\0', sizeof(moji) );
								memmove( moji, cinfrec.ciAdrs1, sizeof(cinfrec.ciAdrs1) );
								pt.cpdata->cpua.Format( _T("%s"), moji );
							}
							if( ncofpar.cpRflg[0]&0x4000 ){	// 住所（下段）
								memset( moji, '\0', sizeof(moji) );
								memmove( moji, cinfrec.ciAdrs2, sizeof(cinfrec.ciAdrs2) );
								pt.cpdata->cpda.Format( _T("%s"), moji );
							}
						}

						pt.cpdata->Update();

						break;
					}
				}
			}
		}
	}

	coinfctl.CloseFile();

	pZmSub->VolumeClose();

	pt.CpdataClose();
	pt.cpdata = NULL;

	return 0;
}
// midori 180104 add <--

// midori 190301 add -->
////////////////////////////////////////////////////////////////////////////////
// GetKaigyoCount()
//
// ■文字列中の改行の数を返す
//
// 
// 引数		：CString			strTarget			文字列
// 
// 戻り値	：CString			帳表タイトル
//
////////////////////////////////////////////////////////////////////////////////
int CUcFunctionCommon::GetKaigyoCount( CString strTarget )
{
	int		rv=0;
	int		nChCode=0;	// 改行コードの位置
	CString	cs=_T("");

	// 改行コードの検索
	cs = strTarget;
	if(strTarget.IsEmpty() == FALSE)	{
		while(1)	{
			nChCode = cs.Find(PRT_FORMAT_CHCODE);
			if(nChCode < 0)	{
				break;
			}
			else	{
				rv++;
				cs = cs.Right(cs.GetLength() - (nChCode + 2));	// +2は改行マーク
			}
		}
	}
	return(rv);
}

//**************************************************
//	文字列の先頭から指定位置まで取得（２５６バイトまで対応）
//	【引数】	szSrc		…	文字列（元）
//				szDst		…	文字列（先）
//				nPos		…	分割位置までのバイト数
//	【戻値】	指定位置以降の文字列を返す
//**************************************************
CString CUcFunctionCommon::GetSpritString( CString szSrc, CString* szDst, int nPos )
{
	CString					szRet;					//	戻値
	char					szBuf[AS_SPLIT_MAX];	//	バッファ
	const unsigned char*	pBuf = 0;				//	バッファ
	int						nLen;					//	文字列長
	int						nSplitPos;				//	分割位置
	
	//	初期化
	ZeroMemory( szBuf, sizeof( szBuf ) );
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
		//	文字列のポインタを取得
		pBuf = (const unsigned char *)szBuf;
		
		//	分割位置のバイトがマルチバイトの先頭バイト？
		if ( _ismbslead( pBuf, &pBuf[nSplitPos - 1]  ) != 0 ){
			//	分割位置を１バイト前に移動
			nSplitPos--;
		}

		//	文字列の先頭から指定位置まで取得
		*szDst = szSrc.Left( nSplitPos );
		//	指定位置以降の文字列を取得
		szRet = szSrc.Mid( nSplitPos );
	}

	//	戻値を返す
	return( szRet );
}
// midori 190301 add <--

// midori 191002 add -->
// ------------------------------------------------------------------------------------------------
//	科目名称付きの小計行を文字数によって文字列の調整を行う
//	【引数】	szSrc		…	文字列（元）
//	【戻値】	なし
// ------------------------------------------------------------------------------------------------
void CUcFunctionCommon::ChgKeiStr( CString szSrc, CString* szDst )
{
	int				tp=0;
	int				ii=0;
	int				nbf=0;
	int				nbf2=0;
	int				nbf3=0;
	CString			sykakkoname=_T("");
	CString			sykakkoname2=_T("");
	char			buf[128]={0};
	CString			cst = _T("");
	CString			cst2 = _T("");

	*szDst = _T("");

	memset(buf,0,128);
	strcpy_s(buf,128,szSrc);
	// 左の括弧を取得
	tp=1;
	if(_mbsbtype((LPBYTE)buf,tp) == _MBC_TRAIL)	{
		tp++;
	}
	sykakkoname = szSrc.Left(tp);
	// 右の括弧を取得
	tp=1;
	if(_mbsbtype((LPBYTE)buf,(szSrc.GetLength()-1)) == _MBC_TRAIL)	{
		tp++;
	}
	sykakkoname2 = szSrc.Right(tp);

	nbf = szSrc.GetLength();
	nbf2 = 0;
	nbf3 = 0;
	for(ii=0;ii<ID_BRACKET_MAX;ii++) {
		// 左括弧を比較
		if(sykakkoname.Compare(BRACKET_KIND[ii].szLeft) == 0) {
			nbf2 = sykakkoname.GetLength();
		}
		// 右括弧を比較
		if(sykakkoname2.Compare(BRACKET_KIND[ii].szRight) == 0) {
			nbf3 = sykakkoname2.GetLength();
		}
		if(nbf2 > 0 && nbf3 > 0) break;
	}
	if(nbf2 == 0) sykakkoname.Empty();
	if(nbf3 == 0) sykakkoname2.Empty();

	// 括弧を除いた文字列を取得
	nbf = nbf - nbf2 - nbf3;
	cst = szSrc;
	if(nbf > 0) {
		cst = szSrc.Mid(nbf2,nbf);
		// 科目の括弧は半角に変更する
		cst.Replace(_T("（"),_T("("));
		cst.Replace(_T("）"),_T(")"));
	}
	cst2 = sykakkoname + cst + sykakkoname2;
	*szDst = cst2;
}

// ------------------------------------------------------------------------------------------------
//	小計行の改行を行う
//	【引数】	szSrc		…	文字列（元）
//	【戻値】	なし
// ------------------------------------------------------------------------------------------------
void CUcFunctionCommon::KeiStrSprit(CString szSrc, CString* szDst, CString* szDst2)
{
	int				tp = 0;
	int				ii = 0;
	int				nbf = 0;
	int				nbf2 = 0;
	int				nbf3 = 0;
	CString			sykakkoname = _T("");
	CString			sykakkoname2 = _T("");
	char			buf[128] = { 0 };
	CString			cst = _T("");
	CString			cst2 = _T("");
	CString			cst3 = _T("");
	CString			cst4 = _T("");

	*szDst = _T("");
	*szDst2 = _T("");

// midori UC_0055 add -->
	if(szSrc.IsEmpty() == TRUE)	{
		return;
	}
// midori UC_0055 add <--

	memset(buf, 0, 128);
	strcpy_s(buf, 128, szSrc);
	// 左の括弧を取得
	tp = 1;
	if (_mbsbtype((LPBYTE)buf, tp) == _MBC_TRAIL) {
		tp++;
	}
	sykakkoname = szSrc.Left(tp);
	// 右の括弧を取得
	tp = 1;
	if (_mbsbtype((LPBYTE)buf, (szSrc.GetLength() - 1)) == _MBC_TRAIL) {
		tp++;
	}
	sykakkoname2 = szSrc.Right(tp);

	nbf = szSrc.GetLength();
	nbf2 = 0;
	nbf3 = 0;
	for (ii = 0; ii < ID_BRACKET_MAX; ii++) {
		// 左括弧を比較
		if (sykakkoname.Compare(BRACKET_KIND[ii].szLeft) == 0) {
			nbf2 = sykakkoname.GetLength();
		}
		// 右括弧を比較
		if (sykakkoname2.Compare(BRACKET_KIND[ii].szRight) == 0) {
			nbf3 = sykakkoname2.GetLength();
		}
		if (nbf2 > 0 && nbf3 > 0) break;
	}
	if (nbf2 == 0) sykakkoname.Empty();
	if (nbf3 == 0) sykakkoname2.Empty();

	// 括弧を除いた文字列を取得
	nbf = nbf - nbf2 - nbf3;
	cst = szSrc;
	if (nbf > 0)		cst = szSrc.Mid(nbf2, nbf);

	cst4.Empty();
	cst2 = sykakkoname + cst + sykakkoname2;
	if (cst2.GetLength() > 24) {
		nbf = cst.Find(_T("("));
		cst4 = GetSpritString(cst, &cst3, nbf);
		// 括弧を付ける
		cst2 = sykakkoname + cst3 + sykakkoname2;
	}
	*szDst = cst2;
	*szDst2 = cst4;
}
// midori 191002 add <--

// midori 191201 add -->
// midori 151730,151263 del -->
//// --------------------------------------------------------------------------------
//// 暦年表示のサインを取得する
//// 暦年表示対応がされていない場合 0:暦年表示未対応を返す
//// 学校の場合、西暦対応サインが'1'になっている場合のみ 1:暦年表示対応 を返す
////
//// 戻り値		-1:エラー
////				 0:暦年表示未対応 or 非営利法人の暦年表示対応の業務以外	
////				 1:暦年表示対応済み
//// --------------------------------------------------------------------------------
//int CUcFunctionCommon::GetRekinen(CDatabase* pdatabase,int apno,int pver,int* pRekinen)
//{
//	int			ret = 0;
//	CString		strSQL =_T("");
//	CString		strData =_T("");
//	CRecordset	rs(pdatabase);
//
//	*pRekinen = 0;
//	// 非営利法人の暦年表示対応の業務以外の場合は未対応を返す
//	if(ChkRekinenAp(apno,pver) == 0)	return(0);
//
//	// 公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59
//	// NULLで無い暦年表示のサインを取得
//	switch(apno) {
//	case 0x51:
//		strSQL = _T("select vl16 from psfvol where aplno = 0 and itmno = 12 and vl16 is not null ");
//		break;
//	case 0x52:
//		strSQL = _T("select vl17 from psfvol where aplno = 0 and itmno = 12 and vl17 is not null ");
//		break;
//	case 0x58:
//		// 学校の場合は初期値がvl16=NULLとなっていないため、
//		// vl17=1になっているかどうかで判断を行う
//		strSQL = _T("select vl16,vl17 from psfvol where aplno = 1 and itmno = 12 and vl16 is not null ");
//		break;
//	case 0x59:
//		strSQL = _T("select vl20 from psfvol where aplno = 0 and itmno = 11 and vl20 is not null ");
//		break;
//	}
//	try{
//		if(!rs.Open(CRecordset::forwardOnly,strSQL))	{
//			rs.Close();
//			return(DB_ERR_OPEN);
//		}
//		if(!rs.IsEOF()) {
//			rs.GetFieldValue((short)0,strData);
//			*pRekinen = _tstoi(strData);
//			// 学校法人db
//			if(apno == 0x58) {
//				rs.GetFieldValue((short)1,strData);
//				if(_tstoi(strData) == 1)		ret = 1;
//			}
//			// 学校法人db以外の非営利法人
//			else {
//				ret = 1;
//			}
//		}
//		rs.Close();
//	}catch(CDBException* e) {
//		// 開放
//		e->Delete();
//		return(-1);
//	}
//
//	return(ret);
//}
//
//// --------------------------------------------------------------------------------
//// 非営利法人の暦年表示対応の業務かチェックを行う
//// 1:暦年表示対応の業務
//// --------------------------------------------------------------------------------
//int CUcFunctionCommon::ChkRekinenAp(int apno,int pver)
//{
//	int ret = 0;
//
//	// 公益Ⅲ:0x51
//	// 社会福祉法人:0x52	新会計基準 ver 0x11
//	// 学校:0x58			新会計基準 ver 0x11
//	// 宗教法人:0x59
//	if(apno == 0x51 || (apno == 0x52 && pver == 0x11) || (apno == 0x58 && pver == 0x11) || apno == 0x59)	{
//		ret = 1;
//	}
//	return(ret);
//}
//
// midori 151730,151263 del <--
// midori 151730,151263 add -->
// --------------------------------------------------------------------------------
// 暦年表示のサインを取得する
//
// 戻り値		暦年表示　0:和暦	1:西暦
// --------------------------------------------------------------------------------
int CUcFunctionCommon::GetRekinen(CDBNpSub* pnpmod)
{
	int rekinen = 0;

	//------------------------------
	//	暦年表示の選択値取得
	//------------------------------
	pnpmod->NpRekinenOpen();
	rekinen = pnpmod->m_pRekinen->m_rsel;				//暦年表示
	pnpmod->NpRekinenClose();

	return(rekinen);
}
// midori 151730,151263 add <--

//----------------------------------------------------------------
// 年月日入力の変換処理
//		引数:		int		0:西暦から和暦に変換
//							1:和暦から西暦に変換
//					int		変換対象の日付
//					int		元号（返送）
//					int		年月日(返送)
//----------------------------------------------------------------
void CUcFunctionCommon::GetYmd(int pRekinen,int pymd,int *rgengo,int* rymd)
{
	int gengo=0;
	int d_gengo=0;
	int d_year=0;

	// 西暦
	if(pRekinen == 1)	{
		//	年月日の入力あり
		if(pymd > 0) {
			//	年の入力あり
			if((pymd / 10000) > 0) {
				// 西暦に変換
				gengo = *rgengo;
				db_datecnvGen(gengo, pymd,&d_gengo, &d_year, 1, 0);
				*rymd = d_year;
			}
		}
	}
	// 和暦
	else {
		//	年月日の入力あり
		if(pymd > 0) {
			//	年の入力あり
			if((pymd / 10000) > 0) {
				if(pymd >= 19890108 && pymd <= 20881231) {
					db_datecnvGen(0, pymd,&d_gengo, &d_year, 0, 0);
					*rgengo = d_gengo;
					*rymd = d_year;
				}
				else {
					*rymd = (pymd % 1000000);
				}
			}
		}
	}
}

//----------------------------------------------------------------
// 年の変換処理
//		引数:		int		0:西暦から和暦に変換
//							1:和暦から西暦に変換
//					int		変換対象の日付
//					int		元号（返送）
//					int		年月日(返送)
//----------------------------------------------------------------
void CUcFunctionCommon::GetYmd2(int ss_ymd,int pRekinen,int pymd,int *rgengo,int* rymd)
{
	int ymd=0;
	int gengo=0;
	int d_gengo=0;
	int d_year=0;

	//	年の入力あり
	if(pymd > 0) {
		ymd = (pymd * 10000) + (ss_ymd % 10000);
		// 西暦
		if(pRekinen == 1)	{
			// 西暦に変換
			gengo = *rgengo;
			db_datecnvGen(gengo, ymd,&d_gengo, &d_year, 1, 0);
			*rymd = (d_year / 10000);
		}
		// 和暦
		else {
			if(ymd >= 19890108 && ymd <= 20881231) {
				db_datecnvGen(0, ymd,&d_gengo, &d_year, 0, 0);
				*rgengo = d_gengo;
				*rymd = (d_year / 10000);
			}
			else {
				*rymd = (pymd % 100);
			}
		}
	}
}

// midori 151258,151262 add -->
//----------------------------------------------------------------
// 年（元号無し）の変換処理
//		引数:		int		事業期間（期首）
//					int		0:西暦から和暦に変換
//							1:和暦から西暦に変換
//					int		変換対象の日付
//					int		年月日(返送)
//----------------------------------------------------------------
void CUcFunctionCommon::GetYmd3(int ss_ymd,int pRekinen,int pymd,int* rymd)
{
	int ymd=0;
	int gengo=0;
	int d_gengo=0;
	int d_year=0;

	//	年の入力あり
	if(pymd > 0) {
		// 西暦
		if(pRekinen == 1)	{
			// 1年が入力されている場合は令和とする
			if(pymd == 1)			ymd = (pymd * 10000) + 1231;
			// 31年が入力されている場合は平成とする
			else if(pymd == 31)		ymd = (pymd * 10000) + 101;
			// 上記以外の年度は事業期間（自）の月日を付加して判断を行う
			else					ymd = (pymd * 10000) + (ss_ymd % 10000);

			// 西暦に変換
			db_datecnvGen(0, ymd,&d_gengo, &d_year, 1, 0);
			*rymd = (d_year / 10000);
		}
		// 和暦
		else {
			// 事業期間（自）の月日を付加して判断を行う
			ymd = (pymd * 10000) + (ss_ymd % 10000);
			// 平成20年01月01日より前の日付は元号無しでの判断を行えないため、
			// エラーとして扱う
			if(ymd >= 20080101 && ymd <= 20881231) {
				db_datecnvGen(0, ymd,&d_gengo, &d_year, 0, 0);
				*rymd = (d_year / 10000);
			}
			else {
				*rymd = (pymd % 100);
			}
		}
	}
}
// midori 151258,151262 add <--

// 共通タブの暦年表示が変更された場合、以下の項目も和暦⇔西暦変換を行う
// → 決算書(予算書)表紙タブの「タイトルに年度を付加する」「表紙表示期間」「各ページ表示期間」
void CUcFunctionCommon::YmdConvert(CDBZmSub* pZmSub, int pSw )
{
	int			gengo=0;
	int			ymd=0;
	int			rymd=0;
	int			opsw=0;
	int			apno=0;
	CDBPrtbl*	pPrtbl;
	int			sw = 0;
	CString		strSQL = _T("");
	CString		strSQL2 = _T("");
	CString		strData = _T("");
	CRecordset	rs(pZmSub->m_database);

	if(pZmSub->zvol == 0)	{
		opsw = 1;
		pZmSub->VolumeOpen();
	}

	pPrtbl = new CDBPrtbl();
	if(pPrtbl->InitProof(pZmSub) != 0)	{
		return;
	}

	apno = pZmSub->zvol->apno;

	while(1)	{
		if(pPrtbl->SetMasterDB(pZmSub->m_database) != 0)	break;
		if(pPrtbl->SetSysCtlDB(pZmSub->m_database) != 0)	break;
		if(pPrtbl->PrfkinOpen(_T("pkofs = 0")) != 0)		break;
		if(pPrtbl->PrfvolOpen() != 0)						break;
		if(pPrtbl->InitProof(pZmSub) != 0)					break;
		// 共通 タイトルに年度を表示する
		// 公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59
		strSQL.Empty();
		strSQL2.Empty();
		switch(apno) {
		// 公益Ⅲ
		case 0x51:
			// 年度
			strSQL = _T("select vl5 from psfvol where aplno = 0 and itmno = 11 ");
			// 元号
			strSQL2 = _T("select vl15 from psfvol where aplno = 0 and itmno = 12 ");
			break;
		// 社会福祉法人
		case 0x52:
			// 年度
			strSQL = _T("select vl5 from psfvol where aplno = 0 and itmno = 11 ");
			// 元号
			strSQL2 = _T("select vl16 from psfvol where aplno = 0 and itmno = 12 ");
			break;
		// 学校法人
		case 0x58:
			// 年度
			strSQL = _T("select vl5 from psfvol where aplno = 1 and itmno = 11 ");
			// 元号
			strSQL2 = _T("select vl12 from psfvol where aplno = 1 and itmno = 12 ");
			break;
		// 宗教法人
		case 0x59:
			// 年度,元号
			strSQL = _T("select vl5,vl19 from psfvol where aplno = 0 and itmno = 11 ");
			break;
		default:
			break;
		}
// midori 151730,151263 del -->
		//// 公益Ⅲ、社会福祉法人、学校、宗教法人以外は処理を行わない
		//if(strSQL.IsEmpty() == TRUE) break;
// midori 151730,151263 del <--

		// HYOCMN（年度を取得、宗教法人は元号も取得）
		rs.Open(CRecordset::forwardOnly,strSQL);
		if(!rs.IsEOF()) {
			rs.GetFieldValue((short)0,strData);
			ymd = _tstoi(strData);
			sw |= 1;
			if(apno == 0x59) {
				rs.GetFieldValue((short)1,strData);
				gengo = _tstoi(strData);
				sw |= 2;
			}
		}
		rs.Close();

		// HYOCMN2（宗教法人以外、元号を取得）
		if(strSQL2.IsEmpty() == FALSE) {
			// 元号
			rs.Open(CRecordset::forwardOnly,strSQL2);
			if(!rs.IsEOF()) {
				rs.GetFieldValue((short)0,strData);
				gengo = _tstoi(strData);
				// 学校以外
				if(apno != 0x58 )					gengo = gengo + 4;
				sw |= 2;
			}
			rs.Close();
		}

		// 年度と元号が取得できた場合、更新処理を行う
		if(sw == 3) {
			rymd = ymd;
			GetYmd2(pZmSub->zvol->ss_ymd,pSw,ymd,&gengo,&rymd);
			strSQL.Empty();
			strSQL2.Empty();
			switch(apno) {
			// 公益Ⅲ
			case 0x51:
				// 年度
				strSQL.Format(_T("update psfvol set vl5 = %d where aplno = 0 and itmno = 11"),rymd);
				// 元号
				strSQL2.Format(_T("update psfvol set vl15 = %d where aplno = 0 and itmno = 12"),(gengo - 4));
				break;
			// 社会福祉法人
			case 0x52:
				// 年度
				strSQL.Format(_T("update psfvol set vl5 = %d where aplno = 0 and itmno = 11"),rymd);
				// 元号
				strSQL2.Format(_T("update psfvol set vl16 = %d where aplno = 0 and itmno = 12"),(gengo - 4));
				break;
			// 学校法人
			case 0x58:
				// 年度
				strSQL.Format(_T("update psfvol set vl5 = %d where aplno = 1 and itmno = 11"),rymd);
				// 元号
				strSQL2.Format(_T("update psfvol set vl12 = %d where aplno = 1 and itmno = 12"),gengo);
				break;
			// 宗教法人
			case 0x59:
				// 年度、元号
				strSQL.Format(_T("update psfvol set vl5 = %d, vl19 = %d where aplno = 0 and itmno = 11"),rymd,gengo);
				break;
			default:
				break;
			}
			pZmSub->m_database->ExecuteSQL(strSQL);
			if(strSQL2.IsEmpty() == FALSE) {
				pZmSub->m_database->ExecuteSQL(strSQL2);
			}
		}
		// 表紙 タイトルに年度を表示する（学校法人dbのみ）
		if(apno == 0x58) {
			if(pPrtbl->prfvol->Requery(_T(""),0) != -1)	{
				pPrtbl->prfvol->Edit();
				ymd = pPrtbl->prfvol->pvsw19;
				GetYmd2(pZmSub->zvol->ss_ymd,pSw,ymd,(int *)&pPrtbl->prfvol->pvsw20,(int *)&pPrtbl->prfvol->pvsw19);
				pPrtbl->prfvol->Update();
			}
		}

		// 表紙 各ページ表示期間
		if(pPrtbl->prfkin->Requery(_T("pkofs=0")) != -1)	{
			pPrtbl->prfkin->Edit();
			// 各ページ表示期間(自)
			ymd = pPrtbl->prfkin->pkmsdy;
			GetYmd(pSw,ymd,(int *)&pPrtbl->prfkin->pkHyGen3,(int *)&pPrtbl->prfkin->pkmsdy);
			// 各ページ表示期間(至)
			ymd = pPrtbl->prfkin->pkmdy;
			GetYmd(pSw,ymd,(int *)&pPrtbl->prfkin->pkHyGen4,(int *)&pPrtbl->prfkin->pkmdy);
			// 書き込み
			pPrtbl->prfkin->Update();
		}

		// 監査報告書（公益法人Ⅲのみ）
		if(apno == 0x51) {
// midori 151730,151263 add -->
			if(pZmSub->object_check(2,_T("kansa_out")) != 0)	{
// midori 151730,151263 add <--
				// 作成日:kandyswを取得	元号:kangengo
				strSQL = _T("select kanymdy,kangengo from kansa_out where kankin = 0");
				rs.Open(CRecordset::forwardOnly,strSQL);
				ymd = 0;
				sw = 0;
				if(!rs.IsEOF()) {
					// 作成日
					rs.GetFieldValue((short)0,strData);
					ymd = _tstoi(strData);
					// 元号
					rs.GetFieldValue((short)1,strData);
					gengo = _tstoi(strData);
					sw = 1;
				}
				rs.Close();
				if(sw == 1) {
					rymd = ymd;
					gengo = gengo + 4;
					GetYmd(pSw,ymd,&gengo,&rymd);
					strSQL.Format(_T("update kansa_out set kanymdy = %d,kangengo = %d where kankin = 0"),rymd,(gengo - 4));
					pZmSub->m_database->ExecuteSQL(strSQL);
				}
// midori 151730,151263 add -->
			}
// midori 151730,151263 add <--
		}

		break;
	}

	if(pPrtbl != NULL)	{
		if(pPrtbl->prfkin != NULL)	{
			pPrtbl->PrfkinClose();
			pPrtbl->prfkin = NULL;
		}
		if(pPrtbl->prfvol != NULL)	{
			pPrtbl->PrfvolClose();
			pPrtbl->prfvol = NULL;
		}
		delete pPrtbl;
		pPrtbl = NULL;
	}
	if(opsw != 0)	{
		pZmSub->VolumeClose();
	}
}
// midori 191201 add <--

// midori 151258,151262 add -->
// 学校法人「消費支出準備金　会計年度」の更新
// 暦年表示が変更された場合、以下の項目も和暦⇔西暦変換を行う
// → 決算書(予算書)消費収支、貸借対照表タブの「消費支出準備金　会計年度」
void CUcFunctionCommon::YmdConvert2(CDBZmSub* pZmSub, int pSw )
{
	int			ymd=0;
	int			rymd=0;
	int			sw = 0;
	int			opsw=0;
// midori 151730,151263 del -->
	//CDBPrtbl*	pPrtbl;
// midori 151730,151263 del <--
	CString		strSQL = _T("");
	CString		strData = _T("");
	CRecordset	rs(pZmSub->m_database);

	if(pZmSub->zvol == 0)	{
		opsw = 1;
		pZmSub->VolumeOpen();
	}

	// 決算書の「消費支出準備金　会計年度」の更新を行う
	// aplno = 1	決算書
	// itmno = 21	貸借対照表
	// itmno = 31	消費収支計算書
	// 会計年度を取得				
	//		貸借対照表		：itmno=21のvl12から取得
	//		消費支出計算書	：itmno=31のvl13から取得

	// 貸借対照表
	sw = 0;
	strSQL = _T("select vl12 from psfvol where aplno = 1 and itmno = 21 ");
	rs.Open(CRecordset::forwardOnly,strSQL);
	if(!rs.IsEOF()) {
		rs.GetFieldValue((short)0,strData);
		ymd = _tstoi(strData);
		sw = 1;
	}
	rs.Close();

	// 年度が取得できた場合、更新処理を行う
	if(sw == 1) {
		GetYmd3(pZmSub->zvol->ss_ymd,pSw,ymd,&rymd);
		strSQL.Empty();
		strSQL.Format(_T("update psfvol set vl12 = %d where aplno = 1 and itmno = 21"),rymd);
		pZmSub->m_database->ExecuteSQL(strSQL);
	}

	// 消費収支計算書
	sw = 0;
	ymd = 0;
	rymd = 0;
	strSQL = _T("select vl13 from psfvol where aplno = 1 and itmno = 31 ");
	rs.Open(CRecordset::forwardOnly,strSQL);
	if(!rs.IsEOF()) {
		rs.GetFieldValue((short)0,strData);
		ymd = _tstoi(strData);
		sw = 1;
	}
	rs.Close();

	// 年度が取得できた場合、更新処理を行う
	if(sw == 1) {
		GetYmd3(pZmSub->zvol->ss_ymd,pSw,ymd,&rymd);
		strSQL.Empty();
		strSQL.Format(_T("update psfvol set vl13 = %d where aplno = 1 and itmno = 31"),rymd);
		pZmSub->m_database->ExecuteSQL(strSQL);
	}

	// 予算書の「消費支出準備金　会計年度」の更新を行う
	// aplno = 3	予算書
	// itmno = 31	消費支出予算書
	// 会計年度を取得				消費支出予算書	：itmno=31のvl12から取得
	sw = 0;
	ymd = 0;
	rymd = 0;
	strSQL = _T("select vl12 from psfvol where aplno = 3 and itmno = 31 ");
	rs.Open(CRecordset::forwardOnly,strSQL);
	if(!rs.IsEOF()) {
		rs.GetFieldValue((short)0,strData);
		ymd = _tstoi(strData);
		sw = 1;
	}
	rs.Close();

	// 年度が取得できた場合、更新処理を行う
	if(sw == 1) {
		GetYmd3(pZmSub->zvol->ss_ymd,pSw,ymd,&rymd);
		strSQL.Empty();
		strSQL.Format(_T("update psfvol set vl12 = %d where aplno = 3 and itmno = 31"),rymd);
		pZmSub->m_database->ExecuteSQL(strSQL);
	}

	// 内訳表の「消費支出準備金　会計年度」の更新を行う
	// aplno = 4	内訳表
	// itmno = 31	消費支出内訳表
	// 会計年度を取得				消費支出内訳表	：itmno=31のvl12から取得
	sw = 0;
	ymd = 0;
	rymd = 0;
	strSQL = _T("select vl12 from psfvol where aplno = 4 and itmno = 31 ");
	rs.Open(CRecordset::forwardOnly,strSQL);
	if(!rs.IsEOF()) {
		rs.GetFieldValue((short)0,strData);
		ymd = _tstoi(strData);
		sw = 1;
	}
	rs.Close();

	// 年度が取得できた場合、更新処理を行う
	if(sw == 1) {
		GetYmd3(pZmSub->zvol->ss_ymd,pSw,ymd,&rymd);
		strSQL.Empty();
		strSQL.Format(_T("update psfvol set vl12 = %d where aplno = 4 and itmno = 31"),rymd);
		pZmSub->m_database->ExecuteSQL(strSQL);
	}

	// 予算内訳表の「消費支出準備金　会計年度」の更新を行う
	// aplno = 5	予算内訳表
	// itmno = 31	消費支出予算内訳表
	// 会計年度を取得				消費支出予算内訳表	：itmno=31のvl13から取得
	sw = 0;
	ymd = 0;
	rymd = 0;
	strSQL = _T("select vl13 from psfvol where aplno = 5 and itmno = 31 ");
	rs.Open(CRecordset::forwardOnly,strSQL);
	if(!rs.IsEOF()) {
		rs.GetFieldValue((short)0,strData);
		ymd = _tstoi(strData);
		sw = 1;
	}
	rs.Close();

	// 年度が取得できた場合、更新処理を行う
	if(sw == 1) {
		GetYmd3(pZmSub->zvol->ss_ymd,pSw,ymd,&rymd);
		strSQL.Empty();
		strSQL.Format(_T("update psfvol set vl13 = %d where aplno = 5 and itmno = 31"),rymd);
		pZmSub->m_database->ExecuteSQL(strSQL);
	}

	if(opsw != 0)	{
		pZmSub->VolumeClose();
	}
}
// midori 151258,151262 add <--
// midori 151730,151263 add -->
// 財務処理db 決算書情報の更新
// 暦年表示が変更され、西暦→和暦に変換処理を行う場合、
// 下記の項目に上２桁を取り除く処理を行う
// → 表紙 各ページ表示期間（自、至）
//    役員 年月日
//    監査 年月日
void CUcFunctionCommon::YmdConvert3(CDBZmSub* pZmSub, int pSw )
{
	CString		cst="",cst2="";
	CDBPrtbl*	pPrtbl;

	// 和暦 → 西暦に変換する場合は処理を行わない
	if(pSw == 1)	return;

	pPrtbl = new CDBPrtbl();
	if(pPrtbl->InitProof(pZmSub) != 0)	{
		return;
	}

	while(1)	{
		if(pPrtbl->SetMasterDB(pZmSub->m_database) != 0)	break;
		if(pPrtbl->SetSysCtlDB(pZmSub->m_database) != 0)	break;
		if(pPrtbl->PrfkinOpen(_T("pkofs = 0")) != 0)		break;
		if(pPrtbl->InitProof(pZmSub) != 0)					break;

		// 表紙 各ページ表示期間
		if(pPrtbl->prfkin->Requery(_T("pkofs=0")) != -1)	{
			pPrtbl->prfkin->Edit();
			// 表紙タブ
			// 各ページ表示期間(自)
			//pPrtbl->prfkin->pkmsdy = (pPrtbl->prfkin->pkmsdy / 1000000);
			cst.Empty();
			cst2.Empty();
			cst.Format(_T("%08d"), pPrtbl->prfkin->pkmsdy);
			cst2 = cst.Right(6);
			pPrtbl->prfkin->pkmsdy = _tstoi(cst2);
			// 各ページ表示期間(至)
			//pPrtbl->prfkin->pkmdy = (pPrtbl->prfkin->pkmdy / 1000000);
			cst.Empty();
			cst2.Empty();
			cst.Format(_T("%08d"), pPrtbl->prfkin->pkmdy);
			cst2 = cst.Right(6);
			pPrtbl->prfkin->pkmdy = _tstoi(cst2);
			// 役員タブ
			//pPrtbl->prfkin->pkrdy = (pPrtbl->prfkin->pkrdy / 1000000);
			cst.Empty();
			cst2.Empty();
			cst.Format(_T("%08d"), pPrtbl->prfkin->pkrdy);
			cst2 = cst.Right(6);
			pPrtbl->prfkin->pkrdy = _tstoi(cst2);
			// 監査タブ
			//pPrtbl->prfkin->pkrdy = (pPrtbl->prfkin->pkkdy / 1000000);
			cst.Empty();
			cst2.Empty();
			cst.Format(_T("%08d"), pPrtbl->prfkin->pkkdy);
			cst2 = cst.Right(6);
			pPrtbl->prfkin->pkkdy = _tstoi(cst2);
			// 書き込み
			pPrtbl->prfkin->Update();
		}

		break;
	}

	if(pPrtbl != NULL)	{
		if(pPrtbl->prfkin != NULL)	{
			pPrtbl->PrfkinClose();
			pPrtbl->prfkin = NULL;
		}
		delete pPrtbl;
		pPrtbl = NULL;
	}
}
// midori 151730,151263 add <--

// midori 155882 add -->
// ------------------------------------------------------------------------------
// 「指定期間と連動させる」ONの時
// 下記条件の場合、勘定科目内訳書表紙の年月日に和暦、西暦表示の変換を行う
// ・「指定期間と連動させる」ON
// ・財務の暦年表示が「和暦」、勘定科目内訳書の年月日が「西暦」
// 　または、財務の暦年表示が「西暦」、勘定科目内訳書の年月日が「和暦」
//
//	引数:		CDBZmSub*		財務クラス
//				CDBPrtbl*		決算書テーブル読み込み用クラス
// ------------------------------------------------------------------------------
void CUcFunctionCommon::YmdChg(CDBZmSub* pZmSub,CDBPrtbl* pt)
{
	int				gengo=0;
	int				rymd=0;

	// 「指定期間と連動させる」ONの時
	// 下記条件の場合、勘定科目内訳書表紙の年月日に和暦、西暦表示の変換を行う
	// ・「指定期間と連動させる」ON
	// ・財務の暦年表示が「和暦」、勘定科目内訳書の年月日が「西暦」
	// 　または、財務の暦年表示が「西暦」、勘定科目内訳書の年月日が「和暦」
	if((pt->prfkin->pkmsw & BIT_D3) != 0) {
		// 西暦
		if(pt->prfkin->pkmsw&0x04){
			// 年月日が和暦の場合、西暦に変換する
			if( (pZmSub->owntb->vl[0] >= 10000 && pZmSub->owntb->vl[0] < 1000000) ||
				(pZmSub->owntb->vl[1] >= 10000 && pZmSub->owntb->vl[1] < 1000000) ||
				(pZmSub->owntb->vl[2] >= 10000 && pZmSub->owntb->vl[2] < 1000000) ||
				(pZmSub->owntb->vl[3] >= 10000 && pZmSub->owntb->vl[3] < 1000000)) {
				pZmSub->owntb->Edit();
				// 決算期間（自）
				if(pZmSub->owntb->vl[0] < 1000000) {
					rymd = pZmSub->owntb->vl[0];
					gengo = pZmSub->owntb->vl[4];
					GetYmd(1,pZmSub->owntb->vl[0],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[0] = rymd;
				}
				// 決算期間（至）
				if(pZmSub->owntb->vl[1] < 1000000) {
					rymd = pZmSub->owntb->vl[1];
					gengo = pZmSub->owntb->vl[5];
					GetYmd(1,pZmSub->owntb->vl[1],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[1] = rymd;
				}
				// 比較用の決算期間（自）
				if(pZmSub->owntb->vl[2] < 1000000) {
					rymd = pZmSub->owntb->vl[2];
					gengo = pZmSub->owntb->vl[6];
					GetYmd(1,pZmSub->owntb->vl[2],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[2] = rymd;
				}
				// 比較用の決算期間（至）
				if(pZmSub->owntb->vl[3] < 1000000) {
					rymd = pZmSub->owntb->vl[3];
					gengo = pZmSub->owntb->vl[7];
					GetYmd(1,pZmSub->owntb->vl[3],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[3] = rymd;
				}
				pZmSub->owntb->Update();
			}

			if( (pt->prfkin->pksdy >= 10000 && pt->prfkin->pksdy < 1000000) ||
				(pt->prfkin->pkedy >= 10000 && pt->prfkin->pkedy < 1000000)) {
				pt->prfkin->Edit();
				// 決算書の決算期間（自）
				if(pt->prfkin->pksdy < 1000000) {
					rymd = pt->prfkin->pksdy;
					gengo = pt->prfkin->pkHyGen1;
					GetYmd(1,pt->prfkin->pksdy,&gengo,&rymd);
					// 年月日
					pt->prfkin->pksdy = rymd;
				}
				// 決算書の決算期間（至）
				if(pt->prfkin->pkedy < 1000000) {
					rymd = pt->prfkin->pkedy;
					gengo = pt->prfkin->pkHyGen2;
					GetYmd(1,pt->prfkin->pkedy,&gengo,&rymd);
					// 年月日
					pt->prfkin->pkedy = rymd;
				}
				pt->prfkin->Update();
			}
		}
		// 和暦
		else {
			// 年月日が西暦の場合、和暦に変換する
			if( pZmSub->owntb->vl[0] >= 10000000 || pZmSub->owntb->vl[1] >= 10000000 ||
				pZmSub->owntb->vl[2] >= 10000000 || pZmSub->owntb->vl[3] >= 10000000) {
				// 更新
				pZmSub->owntb->Edit();
				// 決算期間（自）
				if(pZmSub->owntb->vl[0] >= 10000000) {
					rymd = pZmSub->owntb->vl[0];
					gengo = pZmSub->owntb->vl[4];
					GetYmd(0,pZmSub->owntb->vl[0],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[0] = rymd;
					// 元号
					pZmSub->owntb->vl[4] = gengo;
				}
				// 決算期間（至）
				if(pZmSub->owntb->vl[1] >= 10000000) {
					rymd = pZmSub->owntb->vl[1];
					gengo = pZmSub->owntb->vl[5];
					GetYmd(0,pZmSub->owntb->vl[1],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[1] = rymd;
					// 元号
					pZmSub->owntb->vl[5] = gengo;
				}
				// 比較用の決算期間（自）
				if(pZmSub->owntb->vl[2] >= 10000000) {
					rymd = pZmSub->owntb->vl[2];
					gengo = pZmSub->owntb->vl[6];
					GetYmd(0,pZmSub->owntb->vl[2],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[2] = rymd;
					// 元号
					pZmSub->owntb->vl[6] = gengo;
				}
				// 比較用の決算期間（至）
				if(pZmSub->owntb->vl[3] >= 10000000) {
					rymd = pZmSub->owntb->vl[3];
					gengo = pZmSub->owntb->vl[7];
					GetYmd(0,pZmSub->owntb->vl[3],&gengo,&rymd);
					// 年月日
					pZmSub->owntb->vl[3] = rymd;
					// 元号
					pZmSub->owntb->vl[7] = gengo;
				}
				pZmSub->owntb->Update();
			}
			if( pt->prfkin->pksdy >= 10000000 || pt->prfkin->pkedy >= 10000000) {
				pt->prfkin->Edit();
				// 決算書の決算期間（自）
				if(pt->prfkin->pksdy >= 10000000) {
					rymd = pt->prfkin->pksdy;
					gengo = pt->prfkin->pkHyGen1;
					GetYmd(0,pt->prfkin->pksdy,&gengo,&rymd);
					// 年月日
					pt->prfkin->pksdy = rymd;
					// 元号
					pt->prfkin->pkHyGen1 = gengo;
				}
				// 決算書の決算期間（至）
				if(pt->prfkin->pkedy >= 10000000) {
					rymd = pt->prfkin->pkedy;
					gengo = pt->prfkin->pkHyGen2;
					GetYmd(0,pt->prfkin->pkedy,&gengo,&rymd);
					// 年月日
					pt->prfkin->pkedy = rymd;
					// 元号
					pt->prfkin->pkHyGen2 = gengo;
				}
				pt->prfkin->Update();
			}
		}
	}
}
// midori 155882 add <--

// 158063 add -->
//**************************************************************
//	取引先の総額を取得するSQL文を作成
//	【引数】	CDatabase*		データベース操作クラス
//				int				様式番号
//	【戻値】	CString			作成したSQL文
//**************************************************************
// 210715 del -->
//CString CUcFunctionCommon::MakeAmountSql1( CDatabase* pDB, int pFormSeq )
//{
//	CString				cs=_T("");
//	CString				tbl=_T("");
//	CUcFunctionCommon	ufc;
//	int					is50ont=0,dummy=0;
//
//	if(pFormSeq == ID_FORMNO_021)	tbl = _T("uc_021_uketoritegata");
//	else							tbl = _T("uc_081_siharaitegata");
//
//	ufc.IsSort(pFormSeq,pDB,&dummy,&is50ont);
//
//	cs.Format(_T("select a.AdOrder,a.AdName1,a.AdName2,sum(Val) as amount,b.IkkatuVal from %s a"),tbl);
//	cs += _T(" left join uc_lst_address b on a.AdSeq=b.AdSeq");
//	cs.Format(cs + _T(" where (FgShow=%d or FgShow=%d) and FgFunc=%d"),ID_FGSHOW_OFF,ID_FGSHOW_IKKATU,ID_FGFUNC_DATA);
//// 158138 del -->
//	//cs += _T(" group by a.AdOrder,a.AdName1,a.AdName2,a.AdKana,b.IkkatuVal");
//// 158138 del <--
//// 158138 add -->
//	// 取引先、金融機関を自動で登録するチェックＯＦＦの状態だと
//	// 同じ名称でもカナがセットされていたりされていなかったりするため
//	cs += _T(" group by a.AdOrder,a.AdName1,a.AdName2,b.IkkatuVal");
//// 158138 add <--
//	if(is50ont == 0)	{
//		// 番号順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdOrder ASC,");
//		cs += _T("(a.AdName1 + a.AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName2 COLLATE Japanese_CS_AS_KS_WS ASC");
//	}
//	else	{
//		// 50音順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdKana COLLATE Japanese_CS_AS_KS_WS ASC, a.AdOrder ASC");
//	}
//
//	return(cs);
//}
////**************************************************************
////	取引先－科目の総額を取得するSQL文を作成
////	【引数】	CDatabase*		データベース操作クラス
////				int				様式番号
////	【戻値】	CString			作成したSQL文
////**************************************************************
//CString CUcFunctionCommon::MakeAmountSql2( CDatabase* pDB, int pFormSeq )
//{
//	CString				cs=_T("");
//	CString				tbl=_T("");
//	CUcFunctionCommon	ufc;
//	int					is50ont=0,is50onk=0,dummy=0;
//
//	if(pFormSeq == ID_FORMNO_021)	tbl = _T("uc_021_uketoritegata");
//	else							tbl = _T("uc_081_siharaitegata");
//
//	ufc.IsSort(pFormSeq,pDB,&dummy,&is50ont);
//	is50onk = ufc.IsKamokuSort(pFormSeq,pDB);
//
//	cs.Format(_T("select AdOrder,AdName1,AdName2,a.KnOrder,a.KnName,sum(Val) as amount,b.IkkatuVal from %s a"),tbl);
//	cs += _T(" left join uc_lst_kamoku b on a.KnSeq=b.KnSeq");
//// 158065,158066 del -->
//	//cs.Format(cs + _T(" where (FgShow=%d or FgShow=%d) and FgFunc=%d and a.KnOrder!=0"),ID_FGSHOW_OFF,ID_FGSHOW_IKKATU,ID_FGFUNC_DATA);
//// 158065,158066 del <--
//// 158065,158066 add -->
//	cs.Format(cs + _T(" where (FgShow=%d or FgShow=%d) and FgFunc=%d"),ID_FGSHOW_OFF,ID_FGSHOW_IKKATU,ID_FGFUNC_DATA);
//// 158065,158066 add <--
//// 158138 del -->
//	//cs += _T(" group by AdOrder,AdName1,AdName2,AdKana,a.KnOrder,a.KnName,a.KnKana,b.IkkatuVal");
//// 158138 del <--
//// 158138 add -->
//	cs += _T(" group by AdOrder,AdName1,AdName2,a.KnOrder,a.KnName,a.KnKana,b.IkkatuVal");
//// 158138 add <--
//	// 取引先
//	if(is50ont == 0)	{
//		// 番号順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdOrder ASC,");
//		cs += _T("(a.AdName1 + a.AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, a.AdName2 COLLATE Japanese_CS_AS_KS_WS ASC");
//	}
//	else	{
//		// 50音順
//		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdKana COLLATE Japanese_CS_AS_KS_WS ASC, a.AdOrder ASC");
//	}
//	// 科目
//// 158065,158066 del -->
//	//if(is50onk == 0)	{
//	//	// 番号順
//	//	cs += _T(",a.KnOrder ASC");
//	//}
//	//else	{
//	//	// 50音順
//	//	cs += _T(",a.KnKana COLLATE Japanese_CS_AS_KS_WS ASC, a.KnOrder ASC");
//	//}
//// 158065,158066 del <--
//// 158065,158066 del -->
//	// 科目未入力はソート順が最後になるようにする
//	if(is50onk == 0)	{
//		// 番号順
//		cs += _T(",CASE a.KnOrder WHEN 0 THEN (select max(KnOrder) + 1 from uc_lst_kamoku) END, a.KnOrder ASC");
//	}
//	else	{
//		// 50音順
//		cs += _T(",CASE a.KnOrder WHEN 0 THEN (select max(KnOrder) + 1 from uc_lst_kamoku) END, a.KnKana COLLATE Japanese_CS_AS_KS_WS ASC, a.KnOrder ASC");
//	}
//// 158065,158066 del <--
//	return(cs);
//}
// 210715 del <--
// 210715 add -->
// 修正２点
// ①取引先、金融機関を自動で登録するチェックＯＦＦの状態だと同じ名称でもカナがセットされていたりされていなかったりするが
// 　(「取引先、金融機関を自動で登録する」チェックを途中で付けたり外したりした場合）
// 　その場合はもとからソート処理でも違うグループとして判定していたため、ここでも違うグループとして判定する
// ②[F9 参照]内で取引先の削除を行うと、上段のみの名称は AdName2='' になる。
// 　そのため、NULLと '' 混在して異なるグループで判定されるため、NULL を '' に置き換える
CString CUcFunctionCommon::MakeAmountSql1( CDatabase* pDB, int pFormSeq )
{
	CString				cs=_T("");
	CString				tbl=_T("");
	CString				adnm1=_T(""),adnm2=_T("");
	CUcFunctionCommon	ufc;
	int					is50ont=0,dummy=0;

	if(pFormSeq == ID_FORMNO_021)	tbl = _T("uc_021_uketoritegata");
	else							tbl = _T("uc_081_siharaitegata");

	adnm1=_T("isnull(a.AdName1,'')");
	adnm2=_T("isnull(a.AdName2,'')");

	ufc.IsSort(pFormSeq,pDB,&dummy,&is50ont);

	cs.Format(_T("select a.AdOrder,%s,%s,sum(Val) as amount,b.IkkatuVal from %s a"),adnm1,adnm2,tbl);
	cs += _T(" left join uc_lst_address b on a.AdSeq=b.AdSeq");
	cs.Format(cs + _T(" where (FgShow=%d or FgShow=%d) and FgFunc=%d"),ID_FGSHOW_OFF,ID_FGSHOW_IKKATU,ID_FGFUNC_DATA);
	cs.Format(cs + _T(" group by a.AdOrder,%s,%s,a.AdKana,b.IkkatuVal"),adnm1,adnm2);
	if(is50ont == 0)	{
		// 番号順
		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdOrder ASC,");
		cs.Format(cs + _T("(%s + %s) COLLATE Japanese_CS_AS_KS_WS ASC, %s COLLATE Japanese_CS_AS_KS_WS ASC, %s COLLATE Japanese_CS_AS_KS_WS ASC"),adnm1,adnm2,adnm1,adnm2);
	}
	else	{
		// 50音順
		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdKana COLLATE Japanese_CS_AS_KS_WS ASC, a.AdOrder ASC");
	}

	return(cs);
}

//**************************************************************
//	取引先－科目の総額を取得するSQL文を作成
//	【引数】	CDatabase*		データベース操作クラス
//				int				様式番号
//	【戻値】	CString			作成したSQL文
//**************************************************************
CString CUcFunctionCommon::MakeAmountSql2( CDatabase* pDB, int pFormSeq )
{
	CString				cs=_T("");
	CString				tbl=_T("");
	CString				adnm1=_T(""),adnm2=_T("");
	CUcFunctionCommon	ufc;
	int					is50ont=0,is50onk=0,dummy=0;

	if(pFormSeq == ID_FORMNO_021)	tbl = _T("uc_021_uketoritegata");
	else							tbl = _T("uc_081_siharaitegata");

	adnm1=_T("isnull(a.AdName1,'')");
	adnm2=_T("isnull(a.AdName2,'')");

	ufc.IsSort(pFormSeq,pDB,&dummy,&is50ont);
	is50onk = ufc.IsKamokuSort(pFormSeq,pDB);

	cs.Format(_T("select AdOrder,%s,%s,a.KnOrder,a.KnName,sum(Val) as amount,b.IkkatuVal from %s a"),adnm1,adnm2,tbl);
	cs += _T(" left join uc_lst_kamoku b on a.KnSeq=b.KnSeq");
	cs.Format(cs + _T(" where (FgShow=%d or FgShow=%d) and FgFunc=%d"),ID_FGSHOW_OFF,ID_FGSHOW_IKKATU,ID_FGFUNC_DATA);
	cs.Format(cs + _T(" group by AdOrder,%s,%s,AdKana,a.KnOrder,a.KnName,a.KnKana,b.IkkatuVal"),adnm1,adnm2);
	// 取引先
	if(is50ont == 0)	{
		// 番号順
		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdOrder ASC,");
		cs.Format(cs + _T("(%s + %s) COLLATE Japanese_CS_AS_KS_WS ASC, %s COLLATE Japanese_CS_AS_KS_WS ASC, %s COLLATE Japanese_CS_AS_KS_WS ASC"),adnm1,adnm2,adnm1,adnm2);
	}
	else	{
		// 50音順
		cs += _T(" order by CASE a.AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, a.AdKana COLLATE Japanese_CS_AS_KS_WS ASC, a.AdOrder ASC");
	}
	// 科目
	// 科目未入力はソート順が最後になるようにする
	if(is50onk == 0)	{
		// 番号順
		cs += _T(",CASE a.KnOrder WHEN 0 THEN (select max(KnOrder) + 1 from uc_lst_kamoku) END, a.KnOrder ASC");
	}
	else	{
		// 50音順
		cs += _T(",CASE a.KnOrder WHEN 0 THEN (select max(KnOrder) + 1 from uc_lst_kamoku) END, a.KnKana COLLATE Japanese_CS_AS_KS_WS ASC, a.KnOrder ASC");
	}
	return(cs);
}
// 210715 add <--

//**************************************************************
//	取引先の総額をマップに書き込む（ソート処理から呼ばれる）
//	【引数】	CDatabase*		データベース操作クラス
//				int				様式番号
//	【戻値】	なし
//**************************************************************
void CUcFunctionCommon::MakeAmountMap1( CDatabase* pDB, int pFormSeq )
{
	CRecordset			rs(pDB);
	CString				dt[5];
	AMOUNTREC1			rec;
	short				scnt=0;
	int					cnt=0;

	m_AmountMap1.RemoveAll();

	rs.Open(CRecordset::forwardOnly,MakeAmountSql1(pDB,pFormSeq));
	while(!rs.IsEOF())	{
		for(scnt=0; scnt<5; scnt++)	{
			rs.GetFieldValue(scnt,dt[scnt]);
		}
		// 取引先コード
		dt[0].Trim();
		rec.code = _tstoi(dt[0]);
		// 取引先名称
		dt[1].TrimRight();
		dt[2].TrimRight();
		rec.name = dt[1] + dt[2];
		// 総額
		dt[3].Trim();
		rec.valu = dt[3];
		// 書き込み
		m_AmountMap1.SetAt(cnt,rec);
		cnt++;
		// 次のレコードへ
		rs.MoveNext();
	}
	rs.Close();
}

//**************************************************************
//	取引先－科目の総額をマップに書き込む（ソート処理から呼ばれる）
//	【引数】	CDatabase*		データベース操作クラス
//				int				様式番号
//	【戻値】	なし
//**************************************************************
void CUcFunctionCommon::MakeAmountMap2( CDatabase* pDB, int pFormSeq )
{
	CRecordset			rs(pDB);
	CString				dt[7];
	AMOUNTREC2			rec;
	short				scnt=0;
	int					cnt=0;

	m_AmountMap2.RemoveAll();

	rs.Open(CRecordset::forwardOnly,MakeAmountSql2(pDB,pFormSeq));
	while(!rs.IsEOF())	{
		for(scnt=0; scnt<7; scnt++)	{
			rs.GetFieldValue(scnt,dt[scnt]);
		}
		// 取引先コード
		dt[0].Trim();
		rec.tcode = _tstoi(dt[0]);
		// 科目コード
		dt[3].Trim();
		rec.kcode = _tstoi(dt[3]);
		// 取引先名称
		dt[1].TrimRight();
		dt[2].TrimRight();
		rec.tname = dt[1] + dt[2];
		// 総額
		dt[5].Trim();
		rec.valu = dt[5];
		// 書き込み
		m_AmountMap2.SetAt(cnt,rec);
		cnt++;
		// 次のレコードへ
		rs.MoveNext();
	}
	rs.Close();
}

//**************************************************************
//	取引先の総額をマップから読み込む（ソート処理から呼ばれる）
//	【引数】	int			取引先コード
//				CString		取引先名称
//	【戻値】	CString		総額
//**************************************************************
CString CUcFunctionCommon::GetAmount1( int pCode, CString pName )
{
	int				ii=0,max=0;
	CString			cs=_T("");
	AMOUNTREC1		rec;

	max = (int)m_AmountMap1.GetCount();
	for(ii=0; ii<max; ii++)	{
		m_AmountMap1.Lookup(ii,rec);
		if(rec.code == pCode && rec.name == pName)	{
			cs = rec.valu;
			break;
		}
	}
	return(cs);
}

//**************************************************************
//	取引先－科目の総額をマップから読み込む（ソート処理から呼ばれる）
//	【引数】	int			取引先コード
//				CString		取引先名称
//				int			科目コード
//	【戻値】	CString		総額
//**************************************************************
CString CUcFunctionCommon::GetAmount2( int pTcode, CString pTname, int pKcode )
{
	int				ii=0,max=0;
	CString			cs=_T("");
	AMOUNTREC2		rec;

	max = (int)m_AmountMap2.GetCount();
	for(ii=0; ii<max; ii++)	{
		m_AmountMap2.Lookup(ii,rec);
		if(rec.tcode == pTcode && rec.tname == pTname && rec.kcode == pKcode)	{
			cs = rec.valu;
			break;
		}
	}
	return(cs);
}
// 158063 add <--
