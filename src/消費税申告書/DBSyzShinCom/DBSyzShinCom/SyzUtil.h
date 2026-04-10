#pragma once

#include "SnComDef.h"
#include <icsocx.h>

class CZVOLUME;
class CDBNpSub;
class CSnHeadData;
class SYZSHINCOM_CLASS_EXPORT CSyzUtil
{
public:
	CSyzUtil();
	~CSyzUtil();

	// 文字列を６倍長に変換
	void val_to_bin( unsigned char *d_val, CString s_val );
	// ６倍長を文字列に変換
	void val_to_asci( CString *d_val, unsigned char *s_val );
	// ６倍長　切り捨て
	void l_calq( char *des, char *sou, char sgn );
	// 税率計算
	void percent( char *des, char *sou, int sgn, char ztno, char hagaku, BOOL isUseDotnet=TRUE );
	// ６倍長　集計　モジュ－ル
	void l_6calq( char *dis, char *sou, char *buns, char *bunb );
	// 整数（小数点含む）入力
	void perinp( short hiln, short loln, char *xbuf, short *per1 );
	// ６倍長を数値に変換
	int	val_to_int( unsigned char *s_val );

	// CString　から　Char へ
	void cstring_to_char( unsigned char *d_asci, CString s_asci, int len );
	// 文字列をBCDにパック
	void asci_to_bcd( unsigned char *bcdbf, CString *ascbf, int len );
	// 文字列の後ろの全角・半角スペースカット後のバッファ数を取得
	int MixSpaceCutLength( char *buf, int len );
	// 漢字文字列検索（内容的には全角スペースカット）
	int	MojiSearch(  char *DKnji, char *SKnji, int Moji );
	// 文字列転送（Char　から　CString へ）
	void char_to_cstring( CString *s_asci, unsigned char *d_asci, int lng );
	// 半角文字カット
	CString CutHankaku( LPCSTR check,int size );
	// 何バイト分の文字列？
	int GetByteStrLen( unsigned char *asci, int max );
	// 文字列をBCDに変換（科目コードの変換etc）
	void AscToBcd( char* buff, char* data, int len );
	// 電話番号変換
	void CnvTelno( char *buf, char *telno, int len, int lng );
	// 文字列の後ろの半角SPカット
	void AnkSpaceCut( char *buf, int length );
	// 漢字2文字を（ひらがな・カタカナ）をカタカナに変換
	void LetterAlter( char *Knji, char *Kana, int Moji );
	// 空文字セット
	void LineSpaceSet( char *data, char *buf, int len );
	// 文字列セット
	void LineStrSet( char *data, char *st1, char *st2, int len, int ln2 );
	// カナを漢字に変換 [maxはカナ文字数]
	void AtoJis( char *buf, char *ank, int max );

	// 日付をＢＩＮからＢＣＤに変換（BIN側の年は西暦）
	void date_to_bcd( char type, unsigned char *d_ymd, long s_ymd );
	// 日付をＢＣＤからＢＩＮに変換（BIN側の年は西暦）
	void date_to_bin( int sg, long *d_ymd, unsigned char *s_ymd );
	// BCDを文字列に変換
	void bcd_to_asci( CString *ascbf, unsigned char *bcdbf, int lng );

	// 課税方式の取得
	EnumIdIcsShKazeihoushiki GetKazeihoushiki( CZVOLUME *pZvol );
	// 消費税申告書：非連動マスター？
	BOOL IsUnConnectMst( CZVOLUME *pZvol );
	// 消費税申告書：非連動マスターで簡易課税マスター？
	BOOL IsUnConnectKaniMst( CZVOLUME *pZvol );
	// 個人決算書対応マスター？
	BOOL IsBasicMst( CZVOLUME *pZvol );

	// ダイアグラムデータ 初期化
	void DiagOcxIniz( DIAGRAM_DATA *pDgda );
	// ダイアグラムデータ コピー
	void MoveDiagData( DIAGRAM_DATA *xdata, DIAGRAM_DATA *idata );
	// 
	void LineDataSet( char *data, char *buf, int data_len );
	//
	void LineDataGet( char *data, char *buf, int data_len );
	//
	void LineDataSet( char *data, char *buf, int len, int lin );
	// 
	void LineDataGet( CString &str, char *data, int len, int lin );

	// 科目名称の取得
	int GetKnrecName( CDBNpSub *pZmSub, char *Code, char *Name );

	// 文字列比較
	int StringCompare( BYTE *text1, BYTE *text2, int length );
	// 全てのAsciiのスペースをカット
	void AllAnkSpaceCut( char *, char *, int );				

	// 実数変換後の割り算
	void l_pardotnet_syz( char *dest, const char *src, const char *nume, const char *deno );
	// 実数変換後の割り算(.NET 未使用)
	void l_parwari_syz( char *dest, char *src, char *nume, char *deno );

//2018.03.13 INSERT START
	// 西暦→和暦変換
	int ADtoJC( char *Nengo, char *Year, char *Month, char *Day,  int AD );
	// 和暦→西暦変換
	int JCtoAD( int *AD, char Nengo, char Year, char Month, char Day );

	//'20.07.27
	// 実数変換後の割り算（モード追加版作成。特定収入割合に使用）
	void l_pardotnet_syz2(char *dest, const char *src, const char *nume, const char *deno , char mode);


private:
	// 日付入力変換
	int data_day_conv_DecToHex( char *hex ,int dec );
	// 日付入力変換
	int data_day_conv_HexToDec( int *dec, char hex );
//2018.03.13 INSERT END

private:
	// 掛け算
	void l_multi_d( char* dest, char* a, char* b );
	// ６倍長を文字列に
	CString numstring( char* num );

private:
	CArithEx	m_Arith;	// 倍長演算

public:
	// 特定収入計算表マスター？	('15.02.06)
	BOOL IsSpcMaster( CDBNpSub *pDBNpSub );
	// どの改正様式？('15.08.27)
	EnumIdIcsShRevType GetShRevType( CZVOLUME *pZvol, long symd, long eymd, EnumIdIcsShShinkokuKubun shkubun=ID_ICSSH_KAKUTEI );
	// どの改正様式？('17.12.26)：上位行の関数は他で使用されているかもしれないので残していますが、今後はこの関数を使用してください
	EnumIdIcsShRevType GetShRevType( CZVOLUME *pZvol, long ksymd, long keymd, long msymd, long meymd, EnumIdIcsShShinkokuKubun shkubun=ID_ICSSH_KAKUTEI );
	// 日付をＢＩＮからＢＣＤに変換（BIN側の年は西暦）
	void date_to_bcdGen(char type, int *d_gengo, unsigned char *d_ymd, long s_ymd);
	// 平成継続出力？
	bool IsJcContinuePrint( CSnHeadData *pSnHeadData, bool IsJudgeKjn=true );

	// 元号無し和暦データを変換ルールに則って、元号付き和暦データに変換
	void CnvNoGengoToGengoWareki( UCHAR *pSday, int *pDGengo, UCHAR *pDday );
	// どの元号のプレプリントを使用するのか？
	int GetPrePrintGengo( CSnHeadData *pSnHeadData, bool IsJudgeKjn=true );
	// どの元号のプレプリントを使用するのか？('19.09.13)：上位行の関数は他で使用されているかもしれないので残していますが、今後はこの関数を使用してください
	int GetPrePrintGengo ( CSnHeadData *pSnHeadData, int SyzVer, bool IsJudgeKjn = true );
	// 有効な令和年月日？
	bool IsValidReiwaYmd( int gengo, char yy, char mm, char dd );

};
