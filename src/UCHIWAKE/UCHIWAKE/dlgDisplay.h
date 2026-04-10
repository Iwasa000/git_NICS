//--------------------------------------------------
//	dlgDisplay.h
//
//	2006.02.20～
//--------------------------------------------------

#ifndef	__DLGDISPLAY_H__
#define	__DLGDISPLAY_H__

#pragma once
#include "icslistinp.h"
#include "icsbutton.h"
#include "icsgenl8.h"
#include "icsocx.h"
#include "icsarith8.h"

#include "dbUc011Yotyokin.h"
#include "dbUc021Uketoritegata.h"
#include "dbUc031Urikakekin.h"
#include "dbUc041Karibarai.h"
#include "dbUc042Kasituke.h"
#include "dbUc051Tanaoroshi.h"
#include "dbUc061Yuukasyouken.h"
#include "dbUc071Koteishisan.h"
#include "dbUc081Siharaitegata.h"
#include "dbUc091Kaikakekin.h"
#include "dbUc101Kariukekin.h"
#include "dbUc102Gensen.h"
#include "dbUc111Kariirekin.h"
#include "dbUc121Totiuriage.h"
#include "dbUc131Uriagedaka.h"
#include "dbUc141Yakuin.h"
#include "dbUc142Jinkenhi.h"
#include "dbUc151Tidaiyatin.h"
#include "dbUc152Kenrikin.h"
#include "dbUc153Kougyou.h"
#include "dbUc161Zatueki.h"
#include "dbUc162Zatuson.h"
#include "dbUc171Sonota1.h"
#include "dbUc181Sonota2.h"

#include "UcCommon.h"			//	定数ヘッダ
#include "dateConvert.h"
#include "dbUcInfSonotaItem.h"
#include "dbUcLstHokan.h"

#define	DD_SELECT_OFF	"0"		//	未選択
#define	DD_SELECT_ON	"1"		//	選択

class CdlgDisplay : public ICSDialog 
{
	DECLARE_DYNAMIC(CdlgDisplay)

public:
	CdlgDisplay(CWnd* pParent = NULL);				//	標準コンストラクタ
	CdlgDisplay(UINT id, CWnd* pParent = NULL);		//	コンストラクタ
	virtual ~CdlgDisplay();							//	デストラクタ

	enum { IDD = IDD_DIALOG_DISPLAY };
	CICSListInp m_lstDisplay;
	ICSButton m_btnDelete;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	ICSStatic m_lblHosoku;
	ICSButton m_btnAllOn;
	ICSButton m_btnAllOff;

private:
	EnumIdFgShow		m_nType;		//	種別
	CDatabase*			m_pDB;			//	データベースハンドル
	short				m_nFormSeq;		//	様式シーケンス番号
	int					m_nRet;			//	戻値
	int					m_nColumnMax;	//	カラム最大数
	int					m_nNumGroup;	//	グループ番号
	CFont				m_font;			// フォント情報
// midori 160612 add -->
	BOOL				m_DataKakutei;	// 入力データを確定する
// midori 160612 add <--
public:
	long				m_Seq;			//	シーケンス番号（※未使用）
	
private:
	void InitList( short nFormSeq, CDatabase* pDB, EnumIdFgShow nType, BOOL bHeader = TRUE  );			//	リスト初期化
	void SetList( int nColumnMax, CString szTitle[], int nWidth[] );				//	リスト設定
	void SetYoushiki011Column( int nColumnMax );									//	様式011カラム設定
	int SetYoushiki011Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式011データ設定
	void SetYoushiki021Column( int nColumnMax );									//	様式021データ設定
	int SetYoushiki021Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式021データ設定
	void SetYoushiki031Column( int nColumnMax );									//	様式031データ設定
	int SetYoushiki031Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式031データ設定
	void SetYoushiki041Column( int nColumnMax );									//	様式041データ設定
	int SetYoushiki041Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式041データ設定
	void SetYoushiki042Column( int nColumnMax );									//	様式042データ設定
	int SetYoushiki042Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式042データ設定
	void SetYoushiki051Column( int nColumnMax );									//	様式051カラム設定
	int SetYoushiki051Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式051データ設定
	void SetYoushiki061Column( int nColumnMax );									//	様式061カラム設定
	int SetYoushiki061Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式061データ設定
	void SetYoushiki071Column( int nColumnMax );									//	様式071カラム設定
	int SetYoushiki071Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式071データ設定
	void SetYoushiki081Column( int nColumnMax );									//	様式081カラム設定
	int SetYoushiki081Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式081データ設定
	void SetYoushiki091Column( int nColumnMax );									//	様式091カラム設定
	int SetYoushiki091Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式091データ設定
	void SetYoushiki101Column( int nColumnMax );									//	様式101カラム設定
	int SetYoushiki101Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式101データ設定
	void SetYoushiki102Column( int nColumnMax );									//	様式102カラム設定
	int SetYoushiki102Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式102データ設定
	void SetYoushiki111Column( int nColumnMax );									//	様式111カラム設定
	int SetYoushiki111Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式111データ設定
	void SetYoushiki121Column( int nColumnMax );									//	様式121カラム設定
	int SetYoushiki121Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式121データ設定
	void SetYoushiki131Column( int nColumnMax );									//	様式131カラム設定
	int SetYoushiki131Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式131データ設定
	void SetYoushiki141Column( int nColumnMax );									//	様式141カラム設定
	int SetYoushiki141Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式141データ設定
	void SetYoushiki142Column( int nColumnMax );									//	様式142カラム設定
	int SetYoushiki142Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式142データ設定
	void SetYoushiki151Column( int nColumnMax );									//	様式151カラム設定
	int SetYoushiki151Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式151データ設定
	void SetYoushiki152Column( int nColumnMax );									//	様式152カラム設定
	int SetYoushiki152Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式152データ設定
	void SetYoushiki153Column( int nColumnMax );									//	様式153カラム設定
	int SetYoushiki153Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式153データ設定
	void SetYoushiki16Column( int nColumnMax );										//	様式161／162カラム設定
	int SetYoushiki161Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式161データ設定
	int SetYoushiki162Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式162データ設定
	void SetYoushiki171Column( int nColumnMax );									//	様式171カラム設定
	int SetYoushiki171Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式171データ設定
	void SetYoushiki181Column( int nColumnMax );									//	様式181カラム設定
	int SetYoushiki181Data( CDatabase* pDB, EnumIdFgShow nType, int nColumnMax );	//	様式181データ設定

	int GetFocusControlID( CWnd* pParent = NULL );									//	フォーカスのあるコントロールのID取得
	CString GetFormatValue( CString szSrc, char* szFormat = "sssssssssssss9" );		//	フォーマットした値取得
	void InitLinpData( LINP_DATA* pData );											//	LINP_DATA構造体初期化
	void SetSelectRow( int nIndex );												//	選択行を設定
	CString GetSelectData( int nIndex );											//	選択データを取得
	void SetSelectData( int nIndex, CString szSelect );								//	選択データを設定
	void SetColor( int nIndex, COLORREF nColor );									//	色を設定
	int CheckListItam(  BOOL fDel = FALSE );										//	リストアイテムをチェック
	int SetFgShow( CdbUc000Common* pRec,  BOOL fDel = FALSE );						//	選択行の表示フラグ設定
	int GetSeq( int nIndex );														//	シーケンス番号取得（各様式共通）
	int CheckSelectItem();															//	選択アイテムの確認
	CString DelCrLfString( CString szBuf );											//	改行文字列削除
	CString GetSonota2ItemName();														//

public:
// midori 160612 cor -->
//	int ShowDialog( CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011, 
//					EnumIdFgShow nType = ID_FGSHOW_HOKAN, int nNumGroup = 0 );		//	ダイアログ表示
// ---------------------
	int ShowDialog( CDatabase* pDB = NULL, short nFormSeq = ID_FORMNO_011, 
					EnumIdFgShow nType = ID_FGSHOW_HOKAN, int nNumGroup = 0, BOOL pDataKakutei = 0x04 );	//	ダイアログ表示
// midori 160612 cor <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedAllButton();
	void OnBnClickedAllButtonOff();
public:
	DECLARE_EVENTSINK_MAP()
public:
	void LButtonDblClickDisplayList(short index);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	void LButtonClickDisplayList(short index);
protected:
	virtual void OnOK();	
};

#endif	//	__DLGDISPLAY_H__