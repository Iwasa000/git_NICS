#ifndef _TAIDATA_H_
#define _TAIDATA_H_

#include "input1.h"

#define TAICLQ_NON 0
#define TAICLQ_CNT 1
#define TAICLQ_END 9

#define	MODE_NORMAL		0	// 通常のF9での表示
#define	MODE_END		1	// 終了時の表示
#define MODE_COSEL		2	// 会社再選択での表示

// 科目別 借方 貸方 合計
struct STaiKmkData {
	char			kmcod[10];				/* 科目 code */
	unsigned char	kmk_ksi[_BIN&0x0f],		/* 科目（貸方）合計 */
					kmk_kri[_BIN&0x0f];		/* 科目（借方）合計 */
};


// 科目別 借方 貸方 合計
typedef struct _STaiBrnData {
	char			kmk_cd[10];				/* 科目 code */
	int				brn_no;					/* 枝番 code */
	unsigned char	brn_kasi[_BIN&0x0f],	/* 科目枝番（貸方）合計 */
					brn_kari[_BIN&0x0f];	/* 科目枝番（借方）合計 */
} STaiBrnData;


typedef CArray<STaiBrnData, STaiBrnData&>	CTaiBrnArray;


#ifdef OLD_CLOSE
//
// 貸借合計データ
struct STaiData {
	unsigned char	comm_ttl[_BIN&0x0f],	/* 通常合計 */
					syog_ksi[_BIN&0x0f],	/* 諸口（貸方）合計 */
					syog_kri[_BIN&0x0f],	/* 諸口（借方）合計 */
					blan_ttl[_BIN&0x0f];	/* 貸借合計 */
	unsigned int	data_cnt,				/* 仕訳件数 */
					ezdt_cnt;				/* 枝番件数　*/

	STaiKmkData*	pkm_ttl;				/* 科目の 借・貸合計 */
	int				km_cnt;					/* 借・貸 数 */

	int				clq_sw;					/* 計算サイン 0...未 1...計算中 9...計算終了 */
};

#endif // OLD_CLOSE

struct STaiData {
	unsigned char	comm_deb[_BIN&0x0f],	/* 通常（借方）合計 */
					comm_cre[_BIN&0x0f],	/* 　　（貸方）合計 */
					syog_deb[_BIN&0x0f],	/* 諸口（借方）合計 */
					syog_cre[_BIN&0x0f];	/* 諸口（貸方）合計 */
	unsigned int	data_cnt,				/* 仕訳件数 */
					ezdt_cnt;				/* 枝番件数　*/

	char			clq_kmk[10],			/* 計算科目コード */
					kmk_cnt;				/* 科目数 */
	char			dmy[5];

	STaiKmkData*	pkm_ttl;				/* 科目の 借・貸合計 */
	int				km_cnt;					/* 借・貸 数 */

	int				clq_sw;					/* 計算サイン 0...未 1...計算中 9...計算終了 */

	CTaiBrnArray	tbary;		/* 科目枝番データ配列 */

};



#endif	// end of _TAIDATA_H_