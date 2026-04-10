#pragma once

// 課税期間
#define ID0010		220	//自年号
#define ID0020		221	//自年
#define ID0030		222	//自月
#define ID0040		223	//自日
#define ID0050		224	//至年号
#define ID0060		225	//至年
#define ID0070		226	//至月
#define ID0080		227	//至日
#define	ID0090		228 //整理番号
#define ID1010		229	//納税地
#define	ID1020		230	//フリガナ
#define	ID1030		231	//氏名
#define	ID1040		232	//死亡年月日
#define ID6010		233	// 事業承継　有
#define ID6011		234	// 〃　　　　無
#define ID6020		235	// 住所等 上
#define ID6021		236	// 住所等 下
#define ID6030		237	// 電話番号
#define ID6031		238	// 〃
#define ID6032		239	// 〃
#define ID6040		240	// フリガナ
#define ID6041		241	// 氏名
#define ID6050		242	// その他 上
#define ID6051		243	// その他 下
#define	ID2010		244	//相続人等の代表者の氏名
#define	ID3010		245	//限定承認
#define	ID4010		246	//①
#define	ID4020		247	//②
#define	ID4030		248	//③
#define	ID4040		249	//④
#define	ID4050		250	//⑤
#define	ID4060		251	//⑥

#define	ID5011		252	//住所又は居所１列目
#define	ID5012		253	//　　　　　　２列目
#define	ID5013		254	//　　　　　　３列目
#define	ID5014		255	//　　　　　　４列目
#define	ID5021		256	//フリガナ　　１列目
#define	ID5031		257	//氏名　　　　１列目
#define	ID5022		258	//　　　　　　２列目
#define	ID5032		259	//　　　　　　２列目
#define	ID5023		260	//			  ３列目
#define	ID5033		261	//			  ３列目
#define	ID5024		262	//			  ４列目
#define	ID5034		263	//			  ４列目
#define	ID5041_1	264	//個人番号１　１列目
#define	ID5041_2	265	//個人番号２　１列目
#define	ID5041_3	266	//個人番号３　１列目
#define	ID5042_1	267	//　　　　　　２列目
#define	ID5042_2	268	//　　　　　　２列目
#define	ID5042_3	269	//　　　　　　２列目
#define	ID5043_1	270	//			  ３列目
#define	ID5043_2	271	//			  ３列目
#define	ID5043_3	272	//			  ３列目
#define	ID5044_1	273	//			  ４列目
#define	ID5044_2	274	//			  ４列目
#define	ID5044_3	275	//			  ４列目
#define	ID5051		276	//職業　　　　１列目
#define	ID5061		277	//続柄　　　　１列目
#define	ID5052		278	//　　　　　　２列目
#define	ID5062		279	//　　　　　　２列目
#define	ID5053		280	//　　　　　　３列目
#define	ID5063		281	//　　　　　　３列目
#define	ID5054		282	//　　　　　　４列目
#define	ID5064		283	//　　　　　　４列目
#define	ID5071		284	//生年月日　　１列目
#define	ID5072		285	//　　　　　　２列目
#define	ID5073		286	//　　　　　　３列目
#define	ID5074		287	//　　　　　　４列目
#define	ID5081_1	288	//電話番号１　１列目
#define	ID5081_2	289	//電話番号２　１列目
#define	ID5081_3	290	//電話番号３　１列目
#define	ID5082_1	291	//　　　　　　２列目
#define	ID5082_2	292	//　　　　　　２列目
#define	ID5082_3	293	//　　　　　　２列目
#define	ID5083_1	294	//			  ３列目
#define	ID5083_2	295	//			  ３列目
#define	ID5083_3	296	//			  ３列目
#define	ID5084_1	297	//			  ４列目
#define	ID5084_2	298	//			  ４列目
#define	ID5084_3	299	//			  ４列目
#define	ID5091		300	//⑦法定・指定１列目
#define	ID5101		301	//⑦分子　　　１列目
#define	ID5111		302	//⑦分母　　　１列目
#define	ID5092		303	//　　　　　　２列目
#define	ID5102		304	//　　　　　　２列目
#define	ID5112		305	//　　　　　　２列目
#define	ID5093		306	//　　　　　　３列目
#define	ID5103		307	//　　　　　　３列目
#define	ID5113		308	//　　　　　　３列目
#define	ID5094		309	//　　　　　　４列目
#define	ID5104		310	//　　　　　　４列目
#define	ID5114		311	//　　　　　　４列目
#define	ID5121		312	//⑧　　　　　１列目
#define	ID5122		313	//　　　　　　２列目
#define	ID5123		314	//　　　　　　３列目
#define	ID5124		315	//　　　　　　４列目
#define	ID5131		316	//⑨　　　　　１列目
#define	ID5132		317	//　　　　　　２列目
#define	ID5133		318	//　　　　　　３列目
#define	ID5134		319	//　　　　　　４列目
#define	ID5141		320	//⑩　　　　　１列目
#define	ID5142		321	//　　　　　　２列目
#define	ID5143		322	//　　　　　　３列目
#define	ID5144		323//　　　　　　４列目
#define	ID5151		324	//⑪　　　　　１列目
#define	ID5152		325	//　　　　　　２列目
#define	ID5153		326	//　　　　　　３列目
#define	ID5154		327	//　　　　　　４列目
#define	ID5161		328	//⑫　　　　　１列目
#define	ID5162		329	//　　　　　　２列目
#define	ID5163		330	//　　　　　　３列目
#define	ID5164		331	//　　　　　　４列目
#define	ID5171		332	//⑬　　　　　１列目
#define	ID5172		333	//　　　　　　２列目
#define	ID5173		334	//　　　　　　３列目
#define	ID5174		335	//　　　　　　４列目
#define	ID5181		336	//⑭　　　　　１列目
#define	ID5182		337	//　　　　　　２列目
#define	ID5183		338	//　　　　　　３列目
#define	ID5184		339	//　　　　　　４列目
#define	ID5191_1	340	//銀行名　　  １列目
#define	ID5191_2	341	//銀行選択	  １列目
#define	ID5192_1	342	//			  ２列目
#define	ID5192_2	343	//			  ２列目
#define	ID5193_1	344	//			  ３列目
#define	ID5193_2	345	//			  ３列目
#define	ID5194_1	346	//			  ４列目
#define	ID5194_2	347	//			  ４列目
#define	ID5201_1	348	//支店名　　  １列目
#define	ID5201_2	349	//支店選択	  １列目
#define	ID5202_1	350	//			  ２列目
#define	ID5202_2	351	//			  ２列目
#define	ID5203_1	352	//			  ３列目
#define	ID5203_2	353	//			  ３列目
#define	ID5204_1	354	//			  ４列目
#define	ID5204_2	355	//			  ４列目
#define	ID5211		356	//預金の種類　１列目
#define	ID5212		357	//　　　　　　２列目
#define	ID5213		358	//　　　　　　３列目
#define	ID5214		359	//　　　　　　４列目
#define	ID5221		360	//口座番号　　１列目
#define	ID5222		361	//　　　　　　２列目
#define	ID5223		362	//　　　　　　３列目
#define	ID5224		363	//　　　　　　４列目
#define	ID5231_1	364	//記号番号１  １列目
#define	ID5231_2	365	//記号番号２　１列目
#define	ID5232_1	366	//			  ２列目
#define	ID5232_2	367	//			  ２列目
#define	ID5233_1	368	//			  ３列目
#define	ID5233_2	369	//			  ３列目
#define	ID5234_1	370	//			  ４列目
#define	ID5234_2	371	//			  ４列目
#define	ID5241		372	//郵便局名　　１列目
#define	ID5242		373	//　　　　　　２列目
#define	ID5243		374	//　　　　　　３列目
#define	ID5244		375	//　　　　　　４列目

#define ID_BOTTOM		175	//最も下にあるセル(下がりすぎるためひとつ上を指定)
#define ID_UPPER_RIGHT	255	//右上端セル(相続人)
#define ID_LOWER_LEFT	371	//左下端のセル(相続人)
#define ID_UPPER_LEFT	252	//左上端のセル(相続人)

#define ID_NUMBER1		61
#define ID_NUMBER2		62
#define ID_NUMBER3		63
#define ID_NUMBER4		64

static SH_ITEMINDEX	Fhyo6[]	=	{
	{ID1010		, 0, 0, 0},
	{ID1020		, 0, 0, 0},
	{ID1030		, 0, 0, 0},
	{ID1040		, 1, 1, 0},
	{ID2010		, 1, 1, 0},
	{ID6010		, 1, 1, 0},
	{ID6011		, 1, 1, 0},
	{ID6020		, 1, 1, 0},
	{ID6021		, 1, 1, 0},
	{ID6030		, 1, 1, 0},
	{ID6031		, 1, 1, 0},
	{ID6032		, 1, 1, 0},
	{ID6040		, 1, 1, 0},
	{ID6041		, 1, 1, 0},
	{ID6050		, 1, 1, 0},
	{ID6051		, 1, 1, 0},
	{ID3010		, 1, 1, 0},
	{ID4010		, 0, 0, 0},
	{ID4020		, 0, 0, 0},
	{ID4030		, 0, 0, 0},
	{ID4040		, 0, 0, 0},
	{ID4050		, 0, 0, 0},
	{ID4060		, 0, 0, 0},
	{ID5011		, 1, 1, 0},
	{ID5012		, 1, 1, 0},
	{ID5013		, 1, 1, 0},
	{ID5014		, 1, 1, 0},
	{ID5021		, 1, 1, 0},
	{ID5022		, 1, 1, 0},
	{ID5023		, 1, 1, 0},
	{ID5024		, 1, 1, 0},
	{ID5031		, 1, 1, 0},
	{ID5032		, 1, 1, 0},
	{ID5033		, 1, 1, 0},
	{ID5034		, 1, 1, 0},
	{ID5041_1	, 1, 1, 0},
	{ID5041_2	, 1, 1, 0},
	{ID5041_3	, 1, 1, 0},
	{ID5042_1	, 1, 1, 0},
	{ID5042_2	, 1, 1, 0},
	{ID5042_3	, 1, 1, 0},
	{ID5043_1	, 1, 1, 0},
	{ID5043_2	, 1, 1, 0},
	{ID5043_3	, 1, 1, 0},
	{ID5044_1	, 1, 1, 0},
	{ID5044_2	, 1, 1, 0},
	{ID5044_3	, 1, 1, 0},
	{ID5051	 	, 1, 1, 0},
	{ID5052	 	, 1, 1, 0},
	{ID5053	 	, 1, 1, 0},
	{ID5054	 	, 1, 1, 0},
	{ID5061	 	, 1, 1, 0},
	{ID5062	 	, 1, 1, 0},
	{ID5063	 	, 1, 1, 0},
	{ID5064	 	, 1, 1, 0},
	{ID5071	 	, 1, 1, 0},
	{ID5072	 	, 1, 1, 0},
	{ID5073	 	, 1, 1, 0},
	{ID5074	 	, 1, 1, 0},
	{ID5081_1	, 1, 1, 0},
	{ID5081_2	, 1, 1, 0},
	{ID5081_3	, 1, 1, 0},
	{ID5082_1	, 1, 1, 0},
	{ID5082_2	, 1, 1, 0},
	{ID5082_3	, 1, 1, 0},
	{ID5083_1	, 1, 1, 0},
	{ID5083_2	, 1, 1, 0},
	{ID5083_3	, 1, 1, 0},
	{ID5084_1	, 1, 1, 0},
	{ID5084_2	, 1, 1, 0},
	{ID5084_3	, 1, 1, 0},
	{ID5091		, 1, 1, 0},
	{ID5092		, 1, 1, 0},
	{ID5093		, 1, 1, 0},
	{ID5094		, 1, 1, 0},
	{ID5101		, 1, 1, 0},
	{ID5102		, 1, 1, 0},
	{ID5103		, 1, 1, 0},
	{ID5104		, 1, 1, 0},
	{ID5111		, 1, 1, 0},
	{ID5112		, 1, 1, 0},
	{ID5113		, 1, 1, 0},
	{ID5114		, 1, 1, 0},
	{ID5121		, 1, 1, 0},
	{ID5122		, 1, 1, 0},
	{ID5123		, 1, 1, 0},
	{ID5124		, 1, 1, 0},
	{ID5131		, 1, 1, 0},
	{ID5132		, 1, 1, 0},
	{ID5133		, 1, 1, 0},
	{ID5134		, 1, 1, 0},
	{ID5141		, 1, 1, 0},
	{ID5142		, 1, 1, 0},
	{ID5143		, 1, 1, 0},
	{ID5144		, 1, 1, 0},
	{ID5151		, 0, 0, 0},
	{ID5152		, 0, 0, 0},
	{ID5153		, 0, 0, 0},
	{ID5154		, 0, 0, 0},
	{ID5161		, 1, 1, 0},
	{ID5162		, 1, 1, 0},
	{ID5163		, 1, 1, 0},
	{ID5164		, 1, 1, 0},
	{ID5171		, 1, 1, 0},
	{ID5172		, 1, 1, 0},
	{ID5173		, 1, 1, 0},
	{ID5174		, 1, 1, 0},
	{ID5181		, 0, 0, 0},
	{ID5182		, 0, 0, 0},
	{ID5183		, 0, 0, 0},
	{ID5184		, 0, 0, 0},
	{ID5191_1	, 1, 1, 0},
	{ID5191_2	, 1, 1, 0},
	{ID5192_1	, 1, 1, 0},
	{ID5192_2	, 1, 1, 0},
	{ID5193_1	, 1, 1, 0},
	{ID5193_2	, 1, 1, 0},
	{ID5194_1	, 1, 1, 0},
	{ID5194_2	, 1, 1, 0},
	{ID5201_1	, 1, 1, 0},
	{ID5201_2	, 1, 1, 0},
	{ID5202_1	, 1, 1, 0},
	{ID5202_2	, 1, 1, 0},
	{ID5203_1	, 1, 1, 0},
	{ID5203_2	, 1, 1, 0},
	{ID5204_1	, 1, 1, 0},
	{ID5204_2	, 1, 1, 0},
	{ID5211		, 1, 1, 0},
	{ID5212		, 1, 1, 0},
	{ID5213		, 1, 1, 0},
	{ID5214		, 1, 1, 0},
	{ID5221		, 1, 1, 0},
	{ID5222		, 1, 1, 0},
	{ID5223		, 1, 1, 0},
	{ID5224		, 1, 1, 0},
	{ID5231_1	, 1, 1, 0},
	{ID5231_2	, 1, 1, 0},
	{ID5232_1	, 1, 1, 0},
	{ID5232_2	, 1, 1, 0},
	{ID5233_1	, 1, 1, 0},
	{ID5233_2	, 1, 1, 0},
	{ID5234_1	, 1, 1, 0},
	{ID5234_2	, 1, 1, 0},
	{ID5241		, 1, 1, 0},
	{ID5242		, 1, 1, 0},
	{ID5243		, 1, 1, 0},
	{ID5244		, 1, 1, 0},
	{   0		, 0, 0, 0}
};

static FCSUPDOWN_INFO	FCS_Fhyo60[]	=	{
	{ ID1040	},
	{ ID6010	},
	{ ID6011	},
	{ ID6020	},
	{ ID6021	},
	{ ID6030	},
	{ ID6031	},
	{ ID6032	},
	{ ID6040	},
	{ ID6041	},
	{ ID6050	}, // 10
	{ ID6051	},
	{ ID2010	},
	{ ID3010	},
	{ ID5011	},
	{ ID5021	},
	{ ID5031	},
	{ ID5041_1	},
	{ ID5041_2	},
	{ ID5041_3	},
	{ ID5051	},
	{ ID5061	}, // 20
	{ ID5071	},
	{ ID5081_1	},
	{ ID5081_2	},
	{ ID5081_3	},
	{ ID5091	},
	{ ID5101	},
	{ ID5111	},
	{ ID5121	},
	{ ID5131	},
	{ ID5141	}, // 30
	{ ID5151	},
	{ ID5161	},
	{ ID5171	},
	{ ID5181	},
	{ ID5191_1	},
	{ ID5191_2	},
	{ ID5201_1	},
	{ ID5201_2	},
	{ ID5211	},
	{ ID5221	}, // 40
	{ ID5231_1	},
	{ ID5231_2	},
	{ ID5241	},
	{ ID5012	},
	{ ID5022	},
	{ ID5032	},
	{ ID5042_1	},
	{ ID5042_2	},
	{ ID5042_3	},
	{ ID5052	}, // 50
	{ ID5062	},
	{ ID5072	},
	{ ID5082_1	},
	{ ID5082_2	},
	{ ID5082_3	},
	{ ID5092	},
	{ ID5102	},
	{ ID5112	},
	{ ID5122	},
	{ ID5132	}, // 60
	{ ID5142	},
	{ ID5152	},
	{ ID5162	},
	{ ID5172	},
	{ ID5182	},
	{ ID5192_1	},
	{ ID5192_2	},
	{ ID5202_1	},
	{ ID5202_2	},
	{ ID5212	}, // 70
	{ ID5222	},
	{ ID5232_1	},
	{ ID5232_2	},
	{ ID5242	},
	{ ID5013	},
	{ ID5023	},
	{ ID5033	},
	{ ID5043_1	},
	{ ID5043_2	},
	{ ID5043_3	}, // 80
	{ ID5053	},
	{ ID5063	},
	{ ID5073	},
	{ ID5083_1	},
	{ ID5083_2	},
	{ ID5083_3	},
	{ ID5093	},
	{ ID5103	},
	{ ID5113	},
	{ ID5123	}, // 90
	{ ID5133	},
	{ ID5143	},
	{ ID5153	},
	{ ID5163	},
	{ ID5173	},
	{ ID5183	},
	{ ID5193_1	},
	{ ID5193_2	},
	{ ID5203_1	},
	{ ID5203_2	}, // 100
	{ ID5213	},
	{ ID5223	},
	{ ID5233_1	},
	{ ID5233_2	},
	{ ID5243	},
	{ ID5014	},
	{ ID5024	},
	{ ID5034	},
	{ ID5044_1	},
	{ ID5044_2	}, // 110
	{ ID5044_3	},
	{ ID5054	},
	{ ID5064	},
	{ ID5074	},
	{ ID5084_1	},
	{ ID5084_2	},
	{ ID5084_3	},
	{ ID5094	},
	{ ID5104	},
	{ ID5114	}, // 120
	{ ID5124	},
	{ ID5134	},
	{ ID5144	},
	{ ID5154	},
	{ ID5164	},
	{ ID5174	},
	{ ID5184	},
	{ ID5194_1	},
	{ ID5194_2	},
	{ ID5204_1	}, //130
	{ ID5204_2	},
	{ ID5214	},
	{ ID5224	},
	{ ID5234_1	},
	{ ID5234_2	},
	{ ID5244	} 
};

static FCSUPDOWN_INFO	FCS_Fhyo60_LR[]	=	{
	{ ID1040	},
	{ ID6010	},
	{ ID6011	},
	{ ID6020	},
	{ ID6040	},
	{ ID6050	},
	{ ID6021	},
	{ ID6041	},
	{ ID6051	},
	{ ID6030	},
	{ ID6031	},
	{ ID6032	},
	{ ID2010	},
	{ ID3010	},
	{ ID5011	},
	{ ID5012	},
	{ ID5013	},
	{ ID5014	},
	{ ID5021	},
	{ ID5022	},
	{ ID5023	},
	{ ID5024	},
	{ ID5031	},
	{ ID5032	},
	{ ID5033	},
	{ ID5034	},
	{ ID5041_1	},
	{ ID5041_2	},
	{ ID5041_3	},
	{ ID5042_1	},
	{ ID5042_2	},
	{ ID5042_3	},
	{ ID5043_1	},
	{ ID5043_2	},
	{ ID5043_3	},
	{ ID5044_1	},
	{ ID5044_2	},
	{ ID5044_3	},
	{ ID5051	},
	{ ID5061	},
	{ ID5052	},
	{ ID5062	},
	{ ID5053	},
	{ ID5063	},
	{ ID5054	},
	{ ID5064	},
	{ ID5071	},
	{ ID5072	},
	{ ID5073	},
	{ ID5074	},
	{ ID5081_1	},
	{ ID5081_2	},
	{ ID5081_3	},
	{ ID5082_1	},
	{ ID5082_2	},
	{ ID5082_3	},
	{ ID5083_1	},
	{ ID5083_2	},
	{ ID5083_3	},
	{ ID5084_1	},
	{ ID5084_2	},
	{ ID5084_3	},
	{ ID5091	},
	{ ID5101	},
	{ ID5111	},
	{ ID5092	},
	{ ID5102	},
	{ ID5112	},
	{ ID5093	},
	{ ID5103	},
	{ ID5113	},
	{ ID5094	},
	{ ID5104	},
	{ ID5114	},
	{ ID5121	},
	{ ID5122	},
	{ ID5123	},
	{ ID5124	},
	{ ID5131	},
	{ ID5132	},
	{ ID5133	},
	{ ID5134	},
	{ ID5141	},
	{ ID5142	},
	{ ID5143	},
	{ ID5144	},
	{ ID5151	},
	{ ID5152	},
	{ ID5153	},
	{ ID5154	},
	{ ID5161	},
	{ ID5162	},
	{ ID5163	},
	{ ID5164	},
	{ ID5171	},
	{ ID5172	},
	{ ID5173	},
	{ ID5174	},
	{ ID5181	},
	{ ID5182	},
	{ ID5183	},
	{ ID5184	},
	{ ID5191_1	},
	{ ID5191_2	},
	{ ID5192_1	},
	{ ID5192_2	},
	{ ID5193_1	},
	{ ID5193_2	},
	{ ID5194_1	},
	{ ID5194_2	},
	{ ID5201_1	},
	{ ID5201_2	},
	{ ID5202_1	},
	{ ID5202_2	},
	{ ID5203_1	},
	{ ID5203_2	},
	{ ID5204_1	},
	{ ID5204_2	},
	{ ID5211	},
	{ ID5212	},
	{ ID5213	},
	{ ID5214	},
	{ ID5221	},
	{ ID5222	},
	{ ID5223	},
	{ ID5224	},
	{ ID5231_1	},
	{ ID5231_2	},
	{ ID5232_1	},
	{ ID5232_2	},
	{ ID5233_1	},
	{ ID5233_2	},
	{ ID5234_1	},
	{ ID5234_2	},
	{ ID5241	},
	{ ID5242	},
	{ ID5243	},
	{ ID5244	}
};	  
	  
#define FCS_FHYO6CNT 138
 

static int	JUSYO_ID[SOZOKU_PER_PAGE]		={ID5011,	ID5012,		ID5013,		ID5014};
static int	FURI_ID[SOZOKU_PER_PAGE]		={ID5021,	ID5022,		ID5023,		ID5024};
static int	NAME_ID[SOZOKU_PER_PAGE]		={ID5031,	ID5032,		ID5033,		ID5034};
static int	MYNUM1_ID[SOZOKU_PER_PAGE]		={ID5041_1,	ID5042_1,	ID5043_1,	ID5044_1};
static int	MYNUM2_ID[SOZOKU_PER_PAGE]		={ID5041_2,	ID5042_2,	ID5043_2,	ID5044_2};
static int	MYNUM3_ID[SOZOKU_PER_PAGE]		={ID5041_3,	ID5042_3,	ID5043_3,	ID5044_3};
static int	SYOKU_ID[SOZOKU_PER_PAGE]		={ID5051,	ID5052,		ID5053,		ID5054};
static int	TSUDUKI_ID[SOZOKU_PER_PAGE]		={ID5061,	ID5062,		ID5063,		ID5064};
static int	BIRTH_ID[SOZOKU_PER_PAGE]		={ID5071,	ID5072,		ID5073,		ID5074};
static int	TEL1_ID[SOZOKU_PER_PAGE]		={ID5081_1,	ID5082_1,	ID5083_1,	ID5084_1};
static int	TEL2_ID[SOZOKU_PER_PAGE]		={ID5081_2,	ID5082_2,	ID5083_2,	ID5084_2};
static int	TEL3_ID[SOZOKU_PER_PAGE]		={ID5081_3,	ID5082_3,	ID5083_3,	ID5084_3,};
static int	HOTEI_ID[SOZOKU_PER_PAGE]		={ID5091,	ID5092,		ID5093,		ID5094};
static int	BUNSHI_ID[SOZOKU_PER_PAGE]		={ID5101,	ID5102,		ID5103,		ID5104};
static int	BUNBO_ID[SOZOKU_PER_PAGE]		={ID5111,	ID5112,		ID5113,		ID5114};
static int	KAGAKU_ID[SOZOKU_PER_PAGE]		={ID5121,	ID5122,		ID5123,		ID5124};
static int	NOFUZEI_ID[SOZOKU_PER_PAGE]		={ID5131,	ID5132,		ID5133,		ID5134};
static int	NOFUCHIHO_ID[SOZOKU_PER_PAGE]	={ID5141,	ID5142,		ID5143,		ID5144};
static int	NOFUGOKEI_ID[SOZOKU_PER_PAGE]	={ID5151,	ID5152,		ID5153,		ID5154};
static int	KANPUZEI_ID[SOZOKU_PER_PAGE]	={ID5161,	ID5162,		ID5163,		ID5164};
static int	KANPUCHIHO_ID[SOZOKU_PER_PAGE]	={ID5171,	ID5172,		ID5173,		ID5174};
static int	KANPUGOKEI_ID[SOZOKU_PER_PAGE]	={ID5181,	ID5182,		ID5183,		ID5184};
static int	GNAME_ID[SOZOKU_PER_PAGE]		={ID5191_1,	ID5192_1,	ID5193_1,	ID5194_1};
static int	GSEL_ID[SOZOKU_PER_PAGE]		={ID5191_2,	ID5192_2,	ID5193_2,	ID5194_2};
static int	SNAME_ID[SOZOKU_PER_PAGE]		={ID5201_1,	ID5202_1,	ID5203_1,	ID5204_1};
static int	SSEL_ID[SOZOKU_PER_PAGE]		={ID5201_2,	ID5202_2,	ID5203_2,	ID5204_2};
static int	YOKIN_ID[SOZOKU_PER_PAGE]		={ID5211,	ID5212,		ID5213,		ID5214};
static int	KOUZA_ID[SOZOKU_PER_PAGE]		={ID5221,	ID5222,		ID5223,		ID5224};
static int	KIGO1_ID[SOZOKU_PER_PAGE]		={ID5231_1,	ID5232_1,	ID5233_1,	ID5234_1};
static int	KIGO2_ID[SOZOKU_PER_PAGE]		={ID5231_2,	ID5232_2,	ID5233_2,	ID5234_2};
static int	YUBIN_ID[SOZOKU_PER_PAGE]		={ID5241,	ID5242,		ID5243,		ID5244};
