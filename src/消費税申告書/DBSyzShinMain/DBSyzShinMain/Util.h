#pragma once

class CUtil
{
public:
	CUtil();
	~CUtil();

	// •¶š—ñ‚ğ‚U”{’·‚É•ÏŠ·
	void val_to_bin( unsigned char *d_val, CString s_val );
	// CString@‚©‚ç@Char ‚Ö
	void cstring_to_char( unsigned char *d_asci, CString s_asci, int len );
	// •¶š—ñ‚ğBCD‚ÉƒpƒbƒN
	void asci_to_bcd( unsigned char *bcdbf, CString *ascbf, int len );

private:
	CArithEx	m_Arith;	// ”{’·‰‰Z
};