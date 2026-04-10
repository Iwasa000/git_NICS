//--------------------------------------------------
//	dlgAddressSearch.h
//
//	2006.03.11～
//--------------------------------------------------

#ifndef	__DLGADDRESSSEARCH_H__
#define	__DLGADDRESSSEARCH_H__

#pragma once
// midori 180101 del -->
//#include "ICSZipAd8.h"			//	住所検索用ヘッダ
// midori 180101 del <--
#include "UcCommon.h"			//	共通ヘッダ

#define	AS_SPLIT_MAX	256

class CdlgAddressSearch
{
public:
	CdlgAddressSearch(CWnd* pParent = NULL);		//	コンストラクタ
	~CdlgAddressSearch();							//	デストラクタ

public:
	CWnd*		m_pParent;
	CString		m_AdZip;		//	郵便番号（フォーマットは「***-****」）
	CString		m_AdAdd1;		//	住所１（最大文字数２０文字）
	CString		m_AdAdd2;		//	住所２（最大文字数２０文字）

public:
	int ShowDialog( int nAdAdd1, int nAdAdd2 );								//	ダイアログ表示
	CString GetSpritString( CString szSrc, CString* szDst, int nPos );		//	文字列の先頭から指定位置まで取得
};

#endif	//	__DLGADDRESSSEARCH_H__