#pragma once

#define ID502_1A		58 //1.  （税率６．２４％）控除対象外仕入れに係る支払対価の額の合計額
#define ID502_2A		59 //2.  （税率６．２４％）１×６．２４／１０８
#define ID502_5_bunshi	52 //5.	 取戻し対象特定収入のあった課税期間の課税売上割合　分子
#define ID502_5_bunbo	53 //5.	 取戻し対象特定収入のあった課税期間の課税売上割合　分母
#define ID502_6A		60 //6.  （税率６．２４％）２×５
#define ID502_7_bunshi	54 //7.	 取戻し対象特定収入のあった課税期間の調整割合　分子
#define ID502_7_bunbo	55 //7.	 取戻し対象特定収入のあった課税期間の調整割合　分母
#define ID502_8_bunshi	56 //8.	 １－７　分子
#define ID502_8_bunbo	57 //8.	 １－７　分母
#define ID502_9A		61 //9.  （税率６．２４％）６×８
#define ID502_10A		62 //10. （税率６．２４％）控除対象外仕入れに係る調整額

#define ID502_3B		63 //3.  （税率７．８％）控除対象外仕入れに係る支払対価の額の合計額
#define ID502_4B		64 //4.  （税率７．８％）３×７．８／１１０
#define ID502_6B		65 //6.  （税率７．８％）４×５
#define ID502_9B		66 //9.  （税率７．８％）６×８
#define ID502_10B		67 //10. （税率７．８％）控除対象外仕入れに係る調整額

#define	ID502_check		71 //チェックボックス「変更する」
#define	ID502_5_EX		72 //チェックボックス「変更する」押下時、入力可能に

//入力の可不可
//｛インデックス、連動時、非連動時、確定時入力不可｝
SH_ITEMINDEX Specific52_43_31[] = {
{	ID502_1A		,1, 1, 0		},
{	ID502_2A		,1, 1, 0		},
{	ID502_5_bunshi	,1, 1, 0		},
{	ID502_5_bunbo	,1, 1, 0		},
{	ID502_6A		,0, 0, 0		},
{	ID502_7_bunshi	,1, 1, 0		},
{	ID502_7_bunbo	,1, 1, 0		},
{	ID502_8_bunshi	,0, 0, 0		},
{	ID502_8_bunbo	,0, 0, 0		},
{	ID502_9A		,0, 0, 0		},
{	ID502_10A		,0, 0, 0		},
{	ID502_3B		,1, 1, 0		},
{	ID502_4B		,1, 1, 0		},
{	ID502_6B		,0, 0, 0		},
{	ID502_9B		,0, 0, 0		},
{	ID502_10B		,0, 0, 0		},
{	ID502_check		,1, 1, 0		},
{	ID502_5_EX		,1, 1, 0		},
{					 0,	0, 0, 0		}//この構造体の終わりを示している
};

//入力ボックスのインデックス
//縦方向
static FCSUPDOWN_INFO	FCS_Specific52_43_31[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3B				},
{	ID502_4B				},
{	ID502_check				},
{	ID502_5_bunshi			},
{	ID502_5_bunbo			},
{	ID502_7_bunshi			},
{	ID502_7_bunbo			}
};
static FCSUPDOWN_INFO	FCS_Specific52_43_31_EX[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3B				},
{	ID502_4B				},
{	ID502_check				},
{	ID502_5_EX				},
{	ID502_6A				},
{	ID502_6B				},
{	ID502_7_bunshi			},
{	ID502_7_bunbo			}
};
//横方向
static FCSUPDOWN_INFO	FCS_Specific52_43_31_LR[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3B				},
{	ID502_4B				},
{	ID502_check				},
{	ID502_5_bunshi			},
{	ID502_5_bunbo			},
{	ID502_7_bunshi			},
{	ID502_7_bunbo			}
};
static FCSUPDOWN_INFO	FCS_Specific52_43_31_LR_EX[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3B				},
{	ID502_4B				},
{	ID502_check				},
{	ID502_5_EX				},
{	ID502_6A				},
{	ID502_6B				},
{	ID502_7_bunshi			},
{	ID502_7_bunbo			}
};

//入力ボックスの数
#define		FCS_SPECIFIC1CNT		9
#define		FCS_SPECIFIC1CNT_EX		10