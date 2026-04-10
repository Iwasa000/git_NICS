#pragma	once

#define	ID60 54		// 1. 調整前の新税率適用分の課税仕入れ等の税額			[SpKgz4]
#define	ID61 55		// 2. 調整前の旧税率適用分の課税仕入れ等の税額			[SpKgz3]
#define	ID62 56		// 3. ①＋②											[SpKgzg]
#define	ID63 57		// 4. 特定収入に係る税額								[SpGszg]
#define	ID64 58		// 5. 調整額											[SpTwag]
#define	ID65 59		// 6. 課税仕入れ等の税額								[SpKsig]
#define	ID66 60		// 7. 課税仕入れにのみ使途が特定されている特定収入		[SpTsyu]
#define	ID67 61		// 8. 課税仕入れ等に係る対価の返還等の金額				[SpKsbg]
#define	ID68 62		// 9. 新税率適用分の課税仕入れ等に係る金額				[SpKsi4]
#define	ID69 63		// 10.新税率適用分の課税仕入れ等に係る返還等の金額		[SpKsb4]
#define	ID6A 64		// 11.配賦割合	分子									[SpHfgs]
#define	ID6B 65		// 11.配賦割合　分母									[SpHfgb]
#define	ID6C 66		// 12.新税率適用分における配賦後の調整額				[SpGgt4]
#define	ID6D 67		// 13.新税率適用分の控除対象仕入税額					[SpSiz4]			
#define	ID6E 68		// 14.旧税率適用分における配賦後の調整額				[SpGgt3]
#define	ID6F 69		// 15.旧税率適用分の控除対象仕入税額					[SpSiz3]
#define	ID6G 70		// 16.調整後の控除対象仕入税額の合計					[SpSizg]


SH_ITEMINDEX	Specific6[]	=		{
	{	ID60,	0, 1, 0		},
	{	ID61,	0, 1, 0		},
	{	ID65,	0, 1, 0		},
	{	ID67,	0, 1, 0		},
	{	ID68,	0, 1, 0		},
	{	ID69,	0, 1, 0		},
	{	   0,	0, 0, 0		}
};
