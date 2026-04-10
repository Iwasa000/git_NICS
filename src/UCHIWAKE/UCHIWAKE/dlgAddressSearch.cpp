//--------------------------------------------------
//	addressSearch.cpp
//
//	2006.03.11～
//--------------------------------------------------

#include "StdAfx.h"
#include "dlgAddressSearch.h"

//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CdlgAddressSearch::CdlgAddressSearch(CWnd* pParent)
{
	m_pParent = pParent;
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CdlgAddressSearch::~CdlgAddressSearch()
{
	m_pParent = NULL;
}

//**************************************************
//	文字列の先頭から指定位置まで取得（２５６バイトまで対応）
//	【引数】	szSrc		…	文字列（元）
//				szDst		…	文字列（先）
//				nPos		…	分割位置までのバイト数
//	【戻値】	指定位置以降の文字列を返す
//**************************************************
CString CdlgAddressSearch::GetSpritString( CString szSrc, CString* szDst, int nPos )
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

//**************************************************
//	ダイアログ表示
//	【引数】	nAdAdd1			…	住所１の取得バイト数
//				nAdAdd2			…	住所２の取得バイト数
//	【戻値】	ID_DLG_OK		…	OK押した
//				ID_DLG_CANCEL	…	キャンセル押した
//**************************************************
int CdlgAddressSearch::ShowDialog( int nAdAdd1, int nAdAdd2 )
{
	int			nRet = ID_DLG_CANCEL;	//	戻値
	CString		szZip;					//	郵便番号バッファ
	CString		szAdd;					//	住所バッファ
	CString		szTemp;					//	テンポラリ

	//	初期化
	szZip.Empty();
	szAdd.Empty();
	szTemp.Empty(); 
	m_AdZip.Empty(); 
	m_AdAdd1.Empty(); 
	m_AdAdd2.Empty(); 
	
	//	住所検索
	if ( ZipToAddress( szZip, szAdd, m_pParent ) == TRUE ){
		
		//	郵便番号を取得
		m_AdZip = szZip;

		//	住所１を取得
		szTemp = GetSpritString( szAdd, &m_AdAdd1, nAdAdd1 );
		
		//	住所１以降の文字列あり？
		if ( !szTemp.IsEmpty() ){ 
			//	住所２を取得
			GetSpritString( szTemp, &m_AdAdd2, nAdAdd2 );
		}

		//	戻値をOK押されたに更新
		nRet = ID_DLG_OK;
	}

	//	戻値を返す
	return( nRet );
}