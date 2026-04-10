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

		-- äeóléÆÇ…ShowKeiZeroí«â¡
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

		-- íIâµéëéYÅAîÉä|ã‡ÇÃì‡ñÛèëóìäOìoò^ÉeÅ[ÉuÉãÇ…èoóÕå`éÆÇÃí«â¡
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
		
			-- uc_inf_main îƒópÉJÉâÉÄí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralConstVal','PRECISION')) is null
				ALTER TABLE uc_inf_main add GeneralConstVal binary(16)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')) is null
				ALTER TABLE uc_inf_main add GeneralVar binary(16)

			-- uc_lst_bank ÉJÉiï∂éöÉJÉâÉÄí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana1','PRECISION')) is null
				ALTER TABLE uc_lst_bank add BkKana1 varchar(6)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana2','PRECISION')) is null
				ALTER TABLE uc_lst_bank add BkKana2 varchar(6)

			-- uc_lst_address ÉJÉiï∂éöÉJÉâÉÄí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'AdKana','PRECISION')) is null
				ALTER TABLE uc_lst_address add AdKana varchar(6)
			
			-- uc_lst_kamoku ÉJÉiï∂éöÉJÉâÉÄí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'KnKana','PRECISION')) is null
				ALTER TABLE uc_lst_kamoku add KnKana varchar(6)

			-- lst_hokanÉeÅ[ÉuÉãí«â¡
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

	-- uc_inf_main îƒópÉJÉâÉÄí«â¡
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralConstVal','PRECISION')) is null
		ALTER TABLE uc_inf_main add GeneralConstVal binary(16)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')) is null
		ALTER TABLE uc_inf_main add GeneralVar binary(16)

	-- uc_lst_bank ÉJÉiï∂éöÉJÉâÉÄí«â¡
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana1','PRECISION')) is null
		ALTER TABLE uc_lst_bank add BkKana1 varchar(6)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_bank'),'BkKana2','PRECISION')) is null
		ALTER TABLE uc_lst_bank add BkKana2 varchar(6)

	-- uc_lst_address ÉJÉiï∂éöÉJÉâÉÄí«â¡
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'AdKana','PRECISION')) is null
		ALTER TABLE uc_lst_address add AdKana varchar(6)

	-- uc_lst_kamoku ÉJÉiï∂éöÉJÉâÉÄí«â¡
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
			
			-- uc_lst_kamoku â»ñ⁄ñàÇÃàÍäáã‡äzÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'IkkatuVal','PRECISION')) is null
				ALTER TABLE uc_lst_kamoku add IkkatuVal decimal(14, 0)
				
			-- uc_lst_address éÊà¯êÊñàÇÃàÍäáã‡äzÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'IkkatuVal','PRECISION')) is null
				ALTER TABLE uc_lst_address add IkkatuVal decimal(14, 0)

		end
END
GO

BEGIN
		-- uc_lst_kamoku â»ñ⁄ñàÇÃàÍäáã‡äzÉJÉâÉÄÇí«â¡
		IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'IkkatuVal','PRECISION')) is null
			ALTER TABLE uc_lst_kamoku add IkkatuVal decimal(14, 0)
		-- uc_lst_address éÊà¯êÊñàÇÃàÍäáã‡äzÉJÉâÉÄÇí«â¡
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
			
			-- uc_lst_kamoku â»ñ⁄éwíËéûÇÃèoóÕÉTÉCÉìÉJÉâÉÄÇí«â¡
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

			-- uc_ren_main é}î‘ñ¢ìoò^•ñ¢ì¸óÕÇÃÉfÅ[É^ÇòAìÆÇ∑ÇÈÉTÉCÉìÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_main'),'OpEdaren','PRECISION')) is null
				ALTER TABLE uc_ren_main add OpEdaren tinyint

			-- uc_ren_main â»ñ⁄ñºèÃÇéÊà¯êÊñºèÃÇ…ì]ãLÇ∑ÇÈÉTÉCÉìÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_main'),'OpKmkname','PRECISION')) is null
				ALTER TABLE uc_ren_main add OpKmkname tinyint

			-- uc_ren_main éÊà¯êÊÇÃ[F9 éQè∆]Ç…ìoò^Ç∑ÇÈÉTÉCÉìÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_main'),'OpF9Upd','PRECISION')) is null
				ALTER TABLE uc_ren_main add OpF9Upd tinyint

			-- uc_inf_yokuki óÇä˙çXêVÇ…Åuã‡äzóìÇãÛóìÇ…Ç∑ÇÈ ëŒè€í†ï\Ç≈Ç†ÇÈ/Ç»Ç¢ÅvÉTÉCÉìÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_yokuki'),'OpNullClearFg','PRECISION')) is null
				ALTER TABLE uc_inf_yokuki add OpNullClearFg tinyint

			--uc_inf_yokuki óÇä˙çXêVÇ…Åuã‡äzóìÇãÛóìÇ…Ç∑ÇÈÅvÉTÉCÉìÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_yokuki'),'OpNullClear','PRECISION')) is null
				ALTER TABLE uc_inf_yokuki add OpNullClear tinyint

			-- uc_lst_item_sort 50âπÉ\Å[ÉgópÇÃÉJÉâÉÄÇí«â¡ÅB
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_item_sort'),'OrderStr2','PRECISION')) is null
				ALTER TABLE uc_lst_item_sort add OrderStr2 varchar(300) COLLATE Japanese_CI_AS NULL

			-- äeóléÆÇ…ÉJÉiÉJÉâÉÄÇí«â¡
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

--ÉCÉìÉ{ÉCÉXìoò^î‘çÜí«â¡ëŒâû -->
BEGIN
		----------------
		--	ver10->11(1/2)
		----------------

		SET NOCOUNT ON;

		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver = 10 ) begin
			-- uc_inf_ver
			UPDATE uc_inf_ver SET Version = 11 WHERE Seq = 1
			
			-- äeóléÆÇ…ÉCÉìÉ{ÉCÉXìoò^î‘çÜÉJÉâÉÄÇí«â¡
			-- uc_021_uketoritegata (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'Invno','PRECISION')) is null
				ALTER TABLE uc_021_uketoritegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_031_urikakekin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'Invno','PRECISION')) is null
				ALTER TABLE uc_031_urikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_041_karibarai (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'Invno','PRECISION')) is null
				ALTER TABLE uc_041_karibarai add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_042_kasituke (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_042_kasituke'),'Invno','PRECISION')) is null
				ALTER TABLE uc_042_kasituke add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_071_koteishisan (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_071_koteishisan'),'Invno','PRECISION')) is null
				ALTER TABLE uc_071_koteishisan add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_081_siharaitegata (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'Invno','PRECISION')) is null
				ALTER TABLE uc_081_siharaitegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_091_kaikakekin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'Invno','PRECISION')) is null
				ALTER TABLE uc_091_kaikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_101_kariukekin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'Invno','PRECISION')) is null
				ALTER TABLE uc_101_kariukekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_121_totiuriage (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_121_totiuriage'),'Invno','PRECISION')) is null
				ALTER TABLE uc_121_totiuriage add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_151_tidaiyatin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_151_tidaiyatin'),'Invno','PRECISION')) is null
				ALTER TABLE uc_151_tidaiyatin add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_152_kenrikin (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_152_kenrikin'),'Invno','PRECISION')) is null
				ALTER TABLE uc_152_kenrikin add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_153_kougyou (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_153_kougyou'),'Invno','PRECISION')) is null
				ALTER TABLE uc_153_kougyou add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_161_zatueki (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'Invno','PRECISION')) is null
				ALTER TABLE uc_161_zatueki add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_162_zatuson (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'Invno','PRECISION')) is null
				ALTER TABLE uc_162_zatuson add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_171_sonota1 (Table update)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'Invno','PRECISION')) is null
				ALTER TABLE uc_171_sonota1 add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_lst_addressÇ…ÉCÉìÉ{ÉCÉXìoò^î‘çÜÉJÉâÉÄÇí«â¡
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'Invno','PRECISION')) is null
				ALTER TABLE uc_lst_address add Invno varchar(14) COLLATE Japanese_CI_AS NULL

			-- uc_lst_item_sort äeóléÆÇÃÉ\Å[ÉgÇ…"ìoò^î‘çÜ"Çí«â¡
			declare @udcount int;
			-- óléÆÇQ
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 2 AND ItemSeq = 9
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(2, 9, 'ìoò^î‘çÜ', 1, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇR
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 3 AND ItemSeq = 5
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(3, 5, 'ìoò^î‘çÜ', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇSÅ|ÇP
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 4 AND ItemSeq = 5
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(4, 5, 'ìoò^î‘çÜ', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇSÅ|ÇQ
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 5 AND ItemSeq = 4
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(5, 4, 'ìoò^î‘çÜ', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇV
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 8 AND ItemSeq = 7
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(8, 7, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇW
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 9 AND ItemSeq = 8
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(9, 8, 'ìoò^î‘çÜ', 1, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇX
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 10 AND ItemSeq = 5
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(10, 5, 'ìoò^î‘çÜ', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇOÅ|ÇP
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 11 AND ItemSeq = 5
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(11, 5, 'ìoò^î‘çÜ', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇQ
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 14 AND ItemSeq = 8
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(14, 8, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇTÅ|ÇP
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 18 AND ItemSeq = 5
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(18, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇTÅ|ÇQ
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 19 AND ItemSeq = 6
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(19, 6, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇTÅ|ÇR
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 20 AND ItemSeq = 5
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(20, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇUÅ|ÇP
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 21 AND ItemSeq = 6
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(21, 6, 'ìoò^î‘çÜ', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇUÅ|ÇQ
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 22 AND ItemSeq = 6
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(22, 6, 'ìoò^î‘çÜ', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL)

			-- óléÆÇPÇVÅ|ÇPÅ`ÇQÇO
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 101 AND ItemSeq = 5
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(101, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(102, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(103, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(104, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(105, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(106, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(107, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(108, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(109, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(110, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(111, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(112, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(113, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(114, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(115, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(116, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(117, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(118, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(119, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2)
				VALUES(120, 5, 'ìoò^î‘çÜ', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL)
			end

			set @uchiver = 11

		end
END
GO
--ÉCÉìÉ{ÉCÉXìoò^î‘çÜí«â¡ëŒâû <--
