#pragma once

class CUtil
{
public:
	CUtil();
	~CUtil();

	// •¶š—ñ‚ğ‚U”{’·‚É•ÏŠ·
	void val_to_bin( unsigned char *d_val, CString s_val );
	// ‚U”{’·‚ğ•¶š—ñ‚É•ÏŠ·
	void val_to_asci( CString *d_val, unsigned char *s_val );
	// ‚U”{’·@Ø‚èÌ‚Ä
	void l_calq( char *des, char *sou, char sgn );

private:
	CArithEx	m_Arith;	// ”{’·‰‰Z
};