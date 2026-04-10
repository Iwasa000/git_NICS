// CvtString.h : ヘッダー ファイル
//

#ifndef __CVTSTRING_H__
#define __CVTSTRING_H__

typedef	struct tag_table {
	char	*ank,
			*kana,
			*hira;
} TABLE;

static TABLE KANATABLE1[] = {
	{"ｦ","ヲ","を"},
	{"ｧ","ァ","ぁ"},{"ｨ","ィ","ぃ"},{"ｩ","ゥ","ぅ"},{"ｪ","ェ","ぇ"},{"ｫ","ォ","ぉ"},
	{"ｬ","ャ","ゃ"},{"ｭ","ュ","ゅ"},{"ｮ","ョ","ょ"},{"ｯ","ッ","っ"},
	{"ｱ","ア","あ"},{"ｲ","イ","い"},{"ｳ","ウ","う"},{"ｴ","エ","え"},{"ｵ","オ","お"},
	{"ｶ","カ","か"},{"ｷ","キ","き"},{"ｸ","ク","く"},{"ｹ","ケ","け"},{"ｺ","コ","こ"},
	{"ｻ","サ","さ"},{"ｼ","シ","し"},{"ｽ","ス","す"},{"ｾ","セ","せ"},{"ｿ","ソ","そ"},
	{"ﾀ","タ","た"},{"ﾁ","チ","ち"},{"ﾂ","ツ","つ"},{"ﾃ","テ","て"},{"ﾄ","ト","と"},
	{"ﾅ","ナ","な"},{"ﾆ","ニ","に"},{"ﾇ","ヌ","ぬ"},{"ﾈ","ネ","ね"},{"ﾉ","ノ","の"},
	{"ﾊ","ハ","は"},{"ﾋ","ヒ","ひ"},{"ﾌ","フ","ふ"},{"ﾍ","ヘ","へ"},{"ﾎ","ホ","ほ"},
	{"ﾏ","マ","ま"},{"ﾐ","ミ","み"},{"ﾑ","ム","む"},{"ﾒ","メ","め"},{"ﾓ","モ","も"},
	{"ﾔ","ヤ","や"},{"ﾕ","ユ","ゆ"},{"ﾖ","ヨ","よ"},
	{"ﾗ","ラ","ら"},{"ﾘ","リ","り"},{"ﾙ","ル","る"},{"ﾚ","レ","れ"},{"ﾛ","ロ","ろ"},
	{"ﾜ","ワ","わ"},{"ﾝ","ン","ん"},
	{0,0,0},0
};

static TABLE KANATABLE2[] = {
	{"ｶﾞ","ガ","が"},{"ｷﾞ","ギ","ぎ"},{"ｸﾞ","グ","ぐ"},{"ｹﾞ","ゲ","げ"},{"ｺﾞ","ゴ","ご"},
	{"ｻﾞ","ザ","ざ"},{"ｼﾞ","ジ","じ"},{"ｽﾞ","ズ","ず"},{"ｾﾞ","ゼ","ぜ"},{"ｿﾞ","ゾ","ぞ"},
	{"ﾀﾞ","ダ","だ"},{"ﾁﾞ","ヂ","ぢ"},{"ﾂﾞ","ヅ","づ"},{"ﾃﾞ","デ","で"},{"ﾄﾞ","ド","ど"},
	{"ﾊﾞ","バ","ば"},{"ﾋﾞ","ビ","び"},{"ﾌﾞ","ブ","ぶ"},{"ﾍﾞ","ベ","べ"},{"ﾎﾞ","ボ","ぼ"},
	{"ﾊﾟ","パ","ぱ"},{"ﾋﾟ","ピ","ぴ"},{"ﾌﾟ","プ","ぷ"},{"ﾍﾟ","ペ","ぺ"},{"ﾎﾟ","ポ","ぽ"},
	{0,0,0},0
};

static TABLE NUMTABLE[] = {
	{"1","１","１"},{"2","２","２"},{"3","３","３"},{"4","４","４"},{"5","５","５"},
	{"6","６","６"},{"7","７","７"},{"8","８","８"},{"9","９","９"},{"0","０","０"},
};

enum MBCHARTYPE {
	MBTYPE_SB = 1,		// シングルバイト文字
	MBTYPE_LEAD,		// マルチバイト文字（１文字目）
	MBTYPE_TRAIL,		// マルチバイト文字（２文字目）
	MBTYPE_ERR = -1		// エラー
};

/////////////////////////////////////////////////////////////////////////////
// CCvtString

class CCvtString
{
// コンストラクション
public:
	CCvtString(int nType = 0);

// アトリビュート
protected:
	int		m_nType;		// 改行タイプ	0 : \n
							//				1 :	\r\n

// オペレーション
private:

public:
	int IsMBCharLeadByte(const char* s, int n);
	int GetMBCharType(const char* s, int n);

	void ChangeCRLFType(int nType);

	short IsKindOfLetter(LPCTSTR string, short offset);

	int ANKtoSJIS(LPCTSTR ank, void* sjis, int mode);
	int	SJIStoANK(LPCTSTR sjis, void* ank);

	int	ConvertMultibyte(LPCTSTR ank, LPSTR kanji, int mode);
	int	ConvertSinglebyte(LPCTSTR kanji, LPSTR ank);

	int AddCRLF(LPTSTR pString, int nSize, int nLmax);
	int AddCRLF(CString& rString, int nLmax);

	int DeleteCRLF(LPTSTR pString, int nSize);
	int DeleteCRLF(CString& rString);

	int GetCRLFCount(LPCTSTR pString, int nSize);
	int GetCRLFCount(CString String);

	void SpaceCut(char* pStr, int nSize);
	void HalfSpaceCut(char* pStr, int nSize);

	void TrimLeft(char* pStr, int nSize);
	void TrimRight(char* pStr, int nSize);
	void TrimLeftRight(char* pStr, int nSize);
	void TrimAll(char* pStr, int nSize);

	int UnionString(LPTSTR pStr, int nSize, LPCTSTR pSvStr, LPCTSTR pAddStr, LONG lPos, int nLmax);
	int UnionString(CString& rString, LPCTSTR pSvStr, LPCTSTR pAddStr, LONG lPos, int nLmax);

	int IsMultiByteOnly(LPCTSTR pStr, int nSize);
	int IsMultiKanaOnly(LPCTSTR pStr, int nSize);
	int IsNumericOnly(LPCTSTR pStr, int nSize);
	int IsPassuseOnly(LPCTSTR pStr, int nSize);
};

#endif			// end of __CVTSTRING_H__
