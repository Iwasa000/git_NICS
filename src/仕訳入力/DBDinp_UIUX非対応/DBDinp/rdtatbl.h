#ifndef INCLUDE_RDTATBL_H
#define	INCLUDE_RDTATBL_H


// 定型仕訳 転送 内容
struct _RDTATBL
{
	int RD_lbnum;	// ラベル番号
	int RD_cnt;		// 定型仕訳数
	int RD_trn;		// 定型仕訳転送済 数
	
	CDWordArray	RD_array;	// 転送仕訳 番号
};




#endif // INCLUDE_RDATATBL_H