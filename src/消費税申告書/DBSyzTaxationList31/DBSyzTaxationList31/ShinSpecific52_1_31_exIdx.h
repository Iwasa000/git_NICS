#pragma once

#define	ID502_name			6	//課税仕入れ等に係る特定収入の種類・名称等
#define	ID502_year_from		15  //課税仕入れ等に係る特定収入のあった課税期間（自）
#define	ID502_month_from	16  //課税仕入れ等に係る特定収入のあった課税期間（自）
#define	ID502_day_from		17  //課税仕入れ等に係る特定収入のあった課税期間（自）

#define	ID502_year_to		18  //課税仕入れ等に係る特定収入のあった課税期間（至）
#define	ID502_month_to		19  //課税仕入れ等に係る特定収入のあった課税期間（至）
#define	ID502_day_to		20  //課税仕入れ等に係る特定収入のあった課税期間（至）

#define	ID502_1				32 //課税仕入れ等に係る特定収入により支出された課税仕入れに係る支払対価の額の合計額
#define	ID502_2				33 //課税仕入れ等に係る特定収入により支出された控除対象外仕入れ等に係る支払対価の額の合計額
#define	ID502_3				34 //取戻し対象特定収入の判定（２÷１）

#define	ID502_check2		36
#define	ID502_check3		37
#define	ID502_check4		38

//入力の可不可
//｛インデックス、連動時、非連動時、確定時入力不可｝
SH_ITEMINDEX Specific52_1_31[] = {
{	ID502_name			,1, 1, 0		},
{	ID502_year_from		,1, 1, 0		},
{	ID502_month_from	,1, 1, 0		},
{	ID502_day_from		,1, 1, 0		},
{	ID502_year_to		,1, 1, 0		},
{	ID502_month_to		,1, 1, 0		},
{	ID502_day_to		,1, 1, 0		},
{	ID502_1				,1, 1, 0		},
{	ID502_2				,1, 1, 0		},
{	ID502_3				,0, 0, 0		},
/*{	ID502_check2		,1, 1, 0		},
{	ID502_check3		,1, 1, 0		},
{	ID502_check4		,1, 1, 0		},*/
{						 0,	0, 0, 0		}//この構造体の終わりを示している
};

//入力ボックスのインデックス
//縦方向
static FCSUPDOWN_INFO	FCS_Specific52_1_31[] = {
{	ID502_name				},
{	ID502_year_from			},
{	ID502_month_from		},
{	ID502_day_from			},
{	ID502_year_to			},
{	ID502_month_to			},
{	ID502_day_to			},
{	ID502_1					},
{	ID502_2					},
{	ID502_3					},
{	ID502_check2			},
{	ID502_check3			},
{	ID502_check4			}
};
//横方向
static FCSUPDOWN_INFO	FCS_Specific52_1_31_LR[] = {
{	ID502_name		 		},
{	ID502_year_from	 		},
{	ID502_month_from 		},
{	ID502_day_from	 		},
{	ID502_year_to	 		},
{	ID502_month_to	 		},
{	ID502_day_to	 		},
{	ID502_1			 		},
{	ID502_2			 		},
{	ID502_3			 		},
{	ID502_check2			},
{	ID502_check3			},
{	ID502_check4			}
};

//入力ボックスの数
#define		FCS_SPECIFIC1CNT	13