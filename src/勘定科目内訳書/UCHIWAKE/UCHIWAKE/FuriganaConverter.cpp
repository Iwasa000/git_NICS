#include "StdAfx.h"
#include "FuriganaConverter.h"
#include "CharController.h"

CFuriganaConverter::CFuriganaConverter()
{
}

CFuriganaConverter::~CFuriganaConverter()
{
}

//********************************************************************************
//	入力された文字列を半角カタカナにして振り仮名として返す。
//	振り仮名の文字列長は6バイト
//
//		【引数】	CString		振り仮名を取得する元となる文字列
//		【戻値】	CString		振り仮名。取得失敗の場合はNULLを返す
//******************************************************************************
CString CFuriganaConverter::Convert(CString strBase)
{
	// 空なら何もせず終了
	if(strBase.IsEmpty())	return "";

	// 文字列が改行コード、スペース（全角・半角）のみ、まはたNULLの場合
	strBase.Replace( _T( "　" ), _T( "" ) );	// 全角スペースは、Trimで削除されない
	strBase.Trim();							// 改行コード、半角半角スペースを削除

	// 漢字→全角カタカナ変換
	if( StrConv.GetFurigana(RVF_KATAKANA, strBase, strBase) == -1 ){
		return "";
	}

	// 切り詰め
	CharController cc;
	cc.Divide(strBase, strBase, LENGTH_FURIGANA);

	// 全角カタカナ→半角カタカナ変換
	char chResult[6+1] = {};
	LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH, strBase, strBase.GetLength(), chResult, LENGTH_FURIGANA );

	CString strResult = chResult;

	return strResult;
}