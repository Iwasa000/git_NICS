#pragma once

#define	ID502_1A			76  //1. （税率６．２４％）控除対象外仕入れに係る支払対価の額
#define	ID502_2A			77  //2. （税率６．２４％）１×６．２４／１０８
#define	ID502_3A			78  //3. （税率６．２４％）控除対象外仕入れに係る支払対価の額
#define	ID502_4A			79  //4. （税率６．２４％）３×６．２４／１０８
#define	ID502_9_bunshi		70  //9.  取戻し対象特定収入のあった課税期間の課税売上割合　分子
#define	ID502_9_bunbo		71  //9.  取戻し対象特定収入のあった課税期間の課税売上割合　分母
#define	ID502_10A			80  //10.（税率６．２４％）４×９
#define	ID502_11A			81  //11.（税率６．２４％）２＋１０
#define	ID502_12_bunshi		72  //12. 取戻し対象特定収入のあった課税期間の調整割合　分子
#define	ID502_12_bunbo		73  //12. 取戻し対象特定収入のあった課税期間の調整割合　分母
#define	ID502_13_bunshi		74  //13. １－１２　分子
#define	ID502_13_bunbo		75  //13. １－１２　分母
#define	ID502_14A			82  //14.（税率６．２４％）控除対象外仕入れに係る調整額

#define	ID502_5B			83  //5. （税率７．８％）控除対象外仕入れに係る支払対価の額
#define	ID502_6B			84  //6. （税率７．８％）５×７．８／１１０
#define	ID502_7B			85  //7. （税率７．８％）控除対象外仕入れに係る支払対価の額
#define	ID502_8B			86  //8. （税率７．８％）７×７．８／１１０
#define	ID502_10B			87  //10.（税率７．８％）８×９
#define	ID502_11B			88  //11.（税率７．８％）６＋１０
#define	ID502_14B			89  //14.（税率７．８％）控除対象外仕入れに係る調整額

#define	ID502_15A			102 //15.（税率６．２４％）控除対象外仕入れに係る調整額の合計額

#define	ID502_15B			103 //15.（税率７．８％）控除対象外仕入れに係る調整額の合計額

#define	ID502_check			104 //チェックボックス「変更する」
#define	ID502_9_EX			105 //チェックボックス「変更する」押下時、入力可能に

//入力の可不可
//｛インデックス、連動時、非連動時、確定時入力不可｝
SH_ITEMINDEX Specific52_31_31[] = {
{	ID502_1A		,1, 1, 0		},
{	ID502_2A		,1, 1, 0		},
{	ID502_3A		,1, 1, 0		},
{	ID502_4A		,1, 1, 0		},
{	ID502_9_bunshi	,1, 1, 0		},
{	ID502_9_bunbo	,1, 1, 0		},
{	ID502_10A		,0, 0, 0		},
{	ID502_11A		,0, 0, 0		},
{	ID502_12_bunshi	,1, 1, 0		},
{	ID502_12_bunbo	,1, 1, 0		},
{	ID502_13_bunshi	,0, 0, 0		},
{	ID502_13_bunbo	,0, 0, 0		},
{	ID502_14A		,0, 0, 0		},
{	ID502_5B		,1, 1, 0		},
{	ID502_6B		,1, 1, 0		},
{	ID502_7B		,1, 1, 0		},
{	ID502_8B		,1, 1, 0		},
{	ID502_10B		,0, 0, 0		},
{	ID502_11B		,0, 0, 0		},
{	ID502_14B		,0, 0, 0		},
{	ID502_15A		,0, 0, 0		},
{	ID502_15B		,0, 0, 0		},
{	ID502_check		,1, 1, 0		},
{	ID502_9_EX		,1, 1, 0		},
{					 0,	0, 0, 0		}//この構造体の終わりを示している
};

//入力ボックスのインデックス
//縦方向
static FCSUPDOWN_INFO	FCS_Specific52_31_31[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3A				},
{	ID502_4A				},
{	ID502_5B				},
{	ID502_6B				},
{	ID502_7B				},
{	ID502_8B				},
{	ID502_check				},
{	ID502_9_bunshi			},
{	ID502_9_bunbo			},
{	ID502_12_bunshi			},
{	ID502_12_bunbo			}
};

static FCSUPDOWN_INFO	FCS_Specific52_31_31_EX[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3A				},
{	ID502_4A				},
{	ID502_5B				},
{	ID502_6B				},
{	ID502_7B				},
{	ID502_8B				},
{	ID502_check				},
{	ID502_9_EX				},
{	ID502_10A				},
{	ID502_10B				},
{	ID502_12_bunshi			},
{	ID502_12_bunbo			}
};

//横方向
static FCSUPDOWN_INFO	FCS_Specific52_31_31_LR[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3A				},
{	ID502_4A				},
{	ID502_5B				},
{	ID502_6B				},
{	ID502_7B				},
{	ID502_8B				},
{	ID502_check				},
{	ID502_9_bunshi			},
{	ID502_9_bunbo			},
{	ID502_12_bunshi			},
{	ID502_12_bunbo			}
};

static FCSUPDOWN_INFO	FCS_Specific52_31_31_LR_EX[] = {
{	ID502_1A				},
{	ID502_2A				},
{	ID502_3A				},
{	ID502_4A				},
{	ID502_5B				},
{	ID502_6B				},
{	ID502_7B				},
{	ID502_8B				},
{	ID502_check				},
{	ID502_9_EX				},
{	ID502_10A				},
{	ID502_10B				},
{	ID502_12_bunshi			},
{	ID502_12_bunbo			}
};

//入力ボックスの数
#define		FCS_SPECIFIC1CNT		13
#define		FCS_SPECIFIC1CNT_EX		14