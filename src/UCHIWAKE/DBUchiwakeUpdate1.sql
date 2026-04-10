declare @uchiver int

BEGIN
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	set @uchiver = -1
	select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

	----------------
	--	ver2->3 (1/2)
	----------------
	if( @uchiver <= 2 ) begin
		-- uc_inf_main (Table update)
		ALTER TABLE uc_inf_main WITH NOCHECK ADD OutCoName tinyint;
	end
END
GO

BEGIN
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	set @uchiver = -1
	select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

	----------------
	--	ver4->5
	----------------
	if( @uchiver <= 4 ) begin

		-- 各様式にShowKeiZero追加
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_011_yotyokin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_011_yotyokin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_021_uketoritegata add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_031_urikakekin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_041_karibarai add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_042_kasituke'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_042_kasituke add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_051_tanaoroshi'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_051_tanaoroshi add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_052_tanaoroshi2'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_052_tanaoroshi2 add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_061_yuukasyouken'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_061_yuukasyouken add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_071_koteishisan'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_071_koteishisan add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_081_siharaitegata add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_091_kaikakekin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_092_kaikakekin2'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_092_kaikakekin2 add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_101_kariukekin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_102_gensen'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_102_gensen add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_111_kariirekin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_111_kariirekin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_121_totiuriage'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_121_totiuriage add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_131_uriagedaka'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_131_uriagedaka add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_141_yakuin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_141_yakuin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_142_jinkenhi'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_142_jinkenhi add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_151_tidaiyatin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_151_tidaiyatin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_152_kenrikin'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_152_kenrikin add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_153_kougyou'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_153_kougyou add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_161_zatueki add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_162_zatuson add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_171_sonota1 add ShowKeiZero tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_181_sonota2'),'ShowKeiZero','PRECISION')) is null
			ALTER TABLE uc_181_sonota2 add ShowKeiZero tinyint

		-- 棚卸資産、買掛金の内訳書欄外登録テーブルに出力形式の追加
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_052_tanaoroshi2'),'OutRangai','PRECISION')) is null
			ALTER TABLE uc_052_tanaoroshi2 add OutRangai tinyint
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_092_kaikakekin2'),'OutRangai','PRECISION')) is null
			ALTER TABLE uc_092_kaikakekin2 add OutRangai tinyint
	end
END
GO

BEGIN
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	set @uchiver = -1
	select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

	----------------
	--	ver5->6(1/2)
	----------------
	if( @uchiver <= 5 ) begin

		-- uc_021_uketoritegata (Table update)
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'KnSeq','PRECISION')) is null
			ALTER TABLE uc_021_uketoritegata add KnSeq int
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'KnOrder','PRECISION')) is null
			ALTER TABLE uc_021_uketoritegata add KnOrder int
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'KnName','PRECISION')) is null
			ALTER TABLE uc_021_uketoritegata add KnName varchar(20) COLLATE Japanese_CI_AS

		-- uc_inf_sub (Table update)
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_sub'),'GeneralConstVal','PRECISION')) is null
			ALTER TABLE uc_inf_sub add GeneralConstVal binary(16)
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_sub'),'GeneralVar','PRECISION')) is null
			ALTER TABLE uc_inf_sub add GeneralVar binary(16)
	end
END
GO

BEGIN
		----------------
		--	ver6->7(1/2)
		----------------

		SET NOCOUNT ON;

  		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver <= 6 ) begin
		
			-- uc_inf_main 汎用カラム追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralConstVal','PRECISION')) is null
				ALTER TABLE uc_inf_main add GeneralConstVal binary(16)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')) is null
				ALTER TABLE uc_inf_main add GeneralVar binary(16)

			-- uc_lst_bank カナ文字カラム追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana1','PRECISION')) is null
				ALTER TABLE uc_lst_bank add BkKana1 varchar(6)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana2','PRECISION')) is null
				ALTER TABLE uc_lst_bank add BkKana2 varchar(6)

			-- uc_lst_address カナ文字カラム追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_lst_address add AdKana varchar(6)
			
			-- uc_lst_kamoku カナ文字カラム追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_lst_kamoku add KnKana varchar(6)

			-- lst_hokanテーブル追加
			IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_hokan]') AND type in (N'U'))
			BEGIN
				CREATE TABLE [dbo].[uc_lst_hokan](
					[FormSeq] [smallint] NOT NULL,
					[Seq] [int] NOT NULL,
					[OrderNum] [int] NULL,
				 CONSTRAINT [PK_uc_lst_hokoan] PRIMARY KEY CLUSTERED 
				(
					[FormSeq] ASC,
					[Seq] ASC
				)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
				) ON [PRIMARY]
			END
			
		end
END
GO

BEGIN
	SET NOCOUNT ON;

	-- uc_inf_main 汎用カラム追加
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralConstVal','PRECISION')) is null
		ALTER TABLE uc_inf_main add GeneralConstVal binary(16)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')) is null
		ALTER TABLE uc_inf_main add GeneralVar binary(16)

	-- uc_lst_bank カナ文字カラム追加
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana1','PRECISION')) is null
		ALTER TABLE uc_lst_bank add BkKana1 varchar(6)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana2','PRECISION')) is null
		ALTER TABLE uc_lst_bank add BkKana2 varchar(6)

	-- uc_lst_address カナ文字カラム追加
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'AdKana','PRECISION')) is null
		ALTER TABLE uc_lst_address add AdKana varchar(6)

	-- uc_lst_kamoku カナ文字カラム追加
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'KnKana','PRECISION')) is null
		ALTER TABLE uc_lst_kamoku add KnKana varchar(6)

END
GO

BEGIN
		----------------
		--	ver7->8(1/2)
		----------------

		SET NOCOUNT ON;

  		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver <= 7 ) begin
			
			-- uc_lst_kamoku 科目毎の一括金額カラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'IkkatuVal','PRECISION')) is null
				ALTER TABLE uc_lst_kamoku add IkkatuVal decimal(14, 0)
				
			-- uc_lst_address 取引先毎の一括金額カラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'IkkatuVal','PRECISION')) is null
				ALTER TABLE uc_lst_address add IkkatuVal decimal(14, 0)

		end
END
GO

BEGIN
		-- uc_lst_kamoku 科目毎の一括金額カラムを追加
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'IkkatuVal','PRECISION')) is null
			ALTER TABLE uc_lst_kamoku add IkkatuVal decimal(14, 0)
		-- uc_lst_address 取引先毎の一括金額カラムを追加
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'IkkatuVal','PRECISION')) is null
			ALTER TABLE uc_lst_address add IkkatuVal decimal(14, 0)
END
GO

BEGIN
		----------------
		--	ver8->9(1/2)
		----------------

		SET NOCOUNT ON;

		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver <= 8 ) begin
			
			-- uc_lst_kamoku 科目指定時の出力サインカラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'PrSign','PRECISION')) is null
				ALTER TABLE uc_lst_kamoku add PrSign tinyint

		end
END
GO

BEGIN
		----------------
		--	ver9->10(1/2)
		----------------

		SET NOCOUNT ON;

		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver <= 9 ) begin

			-- uc_ren_main 枝番未登録･未入力のデータを連動するサインカラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_main'),'OpEdaren','PRECISION')) is null
				ALTER TABLE uc_ren_main add OpEdaren tinyint

			-- uc_ren_main 科目名称を取引先名称に転記するサインカラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_main'),'OpKmkname','PRECISION')) is null
				ALTER TABLE uc_ren_main add OpKmkname tinyint

			-- uc_ren_main 取引先の[F9 参照]に登録するサインカラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_main'),'OpF9Upd','PRECISION')) is null
				ALTER TABLE uc_ren_main add OpF9Upd tinyint

			-- uc_inf_yokuki 翌期更新に「金額欄を空欄にする 対象帳表である/ない」サインカラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_yokuki'),'OpNullClearFg','PRECISION')) is null
				ALTER TABLE uc_inf_yokuki add OpNullClearFg tinyint

			--uc_inf_yokuki 翌期更新に「金額欄を空欄にする」サインカラムを追加
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_yokuki'),'OpNullClear','PRECISION')) is null
				ALTER TABLE uc_inf_yokuki add OpNullClear tinyint

			-- uc_lst_item_sort 50音ソート用のカラムを追加。
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_item_sort'),'OrderStr2','PRECISION')) is null
				ALTER TABLE uc_lst_item_sort add OrderStr2 varchar(300) COLLATE Japanese_CI_AS NULL

			-- 各様式にカナカラムを追加
			-- uc_011_yotyokin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_011_yotyokin'),'BkKana1','PRECISION')) is null
				ALTER TABLE uc_011_yotyokin add BkKana1 varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_011_yotyokin'),'BkKana2','PRECISION')) is null
				ALTER TABLE uc_011_yotyokin add BkKana2 varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_011_yotyokin'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_011_yotyokin add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_021_uketoritegata (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_021_uketoritegata add AdKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_021_uketoritegata add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_031_urikakekin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_031_urikakekin add KnKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_031_urikakekin add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_041_karibarai (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_041_karibarai add KnKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_041_karibarai add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_042_kasituke (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_042_kasituke'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_042_kasituke add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_051_tanaoroshi (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_051_tanaoroshi'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_051_tanaoroshi add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_061_yuukasyouken (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_061_yuukasyouken'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_061_yuukasyouken add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_081_siharaitegata (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_081_siharaitegata add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_091_kaikakekin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_091_kaikakekin add KnKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_091_kaikakekin add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_101_kariukekin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_101_kariukekin add KnKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_101_kariukekin add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_102_gensen (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_102_gensen'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_102_gensen add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_111_kariirekin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_111_kariirekin'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_111_kariirekin add KnKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_111_kariirekin'),'AdKana1','PRECISION')) is null
				ALTER TABLE uc_111_kariirekin add AdKana1 varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_111_kariirekin'),'AdKana2','PRECISION')) is null
				ALTER TABLE uc_111_kariirekin add AdKana2 varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_121_totiuriage (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_121_totiuriage'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_121_totiuriage add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_151_tidaiyatin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_151_tidaiyatin'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_151_tidaiyatin add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_161_zatueki (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_161_zatueki add KnKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_161_zatueki add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_162_zatuson (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_162_zatuson add KnKana varchar(6) COLLATE Japanese_CI_AS NULL
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_162_zatuson add AdKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_171_sonota1 (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_171_sonota1 add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

			-- uc_181_sonota2 (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_181_sonota2'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_181_sonota2 add KnKana varchar(6) COLLATE Japanese_CI_AS NULL

		end
END
GO
