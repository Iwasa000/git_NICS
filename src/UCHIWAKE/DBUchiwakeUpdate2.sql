declare @uchiver int
declare @udcount int;

BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	set @uchiver = -1
	select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

	----------------
	--	ver1->2
	----------------
	if( @uchiver = 1 ) begin
		-- uc_inf_ver
		DELETE uc_inf_ver WHERE Seq=1
		DELETE uc_inf_ver WHERE Seq=2
		INSERT INTO uc_inf_ver(Seq,Version) VALUES (1, 2);
		INSERT INTO uc_inf_ver(Seq,Version) VALUES (2, 2);

		-- uc_lst_item_sort
		DELETE uc_lst_item_sort WHERE FormSeq=12 AND ItemSeq=3
		INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
		VALUES (12, 3, 'Š“¾‚Ìí—Ş', 0, 1, 0, 'KnOrder ASC', 'KnOrder', NULL);

		-- vtbl
		DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
		INSERT INTO vtbl (type, objname, vers)
		VALUES (1001, 'DBUchiwake', 2)

		set @uchiver = 2
	end

END
GO

BEGIN
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	set @uchiver = -1
	select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

	----------------
	--	ver2->3 (2/2)
	----------------
	if( @uchiver = 2 ) begin
		-- uc_inf_ver
		DELETE uc_inf_ver WHERE Seq=1
		INSERT INTO uc_inf_ver(Seq,Version) VALUES (1, 3);

		-- uc_inf_main (Table update)
		UPDATE uc_inf_main SET OutCoName=1;

		-- vtbl
		DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
		INSERT INTO vtbl (type, objname, vers)
		VALUES (1001, 'DBUchiwake', 3)

		set @uchiver = 3
	end
END
GO

BEGIN
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	set @uchiver = -1
	select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

	----------------
	--	ver3->4
	----------------
	if( @uchiver = 3 ) begin
		-- uc_inf_ver
		DELETE uc_inf_ver WHERE Seq=1
		INSERT INTO uc_inf_ver(Seq,Version) VALUES (1, 4);

		-- uc_inf_sub (Table update)
		UPDATE uc_inf_sub SET TitleOrg='Œ¹òŠ“¾Å—a‚è‹à‚Ì“à–ó' WHERE FormSeq=12;
		UPDATE uc_inf_sub SET TitleOrgTujo='Œ¹òŠ“¾Å—a‚è‹à‚Ì“à–ó' WHERE FormSeq=12;

		-- vtbl
		DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
		INSERT INTO vtbl (type, objname, vers)
		VALUES (1001, 'DBUchiwake', 4)

		set @uchiver = 4
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
	if( @uchiver = 4 ) begin
		-- uc_inf_ver
		DELETE uc_inf_ver WHERE Seq=1
		INSERT INTO uc_inf_ver(Seq,Version) VALUES (1, 5);
	end
END
GO

BEGIN
		SET NOCOUNT ON;

  		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver = 4 ) begin

		-- Še—l®‚É‰Šú’lƒZƒbƒg
		UPDATE uc_011_yotyokin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_021_uketoritegata set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_031_urikakekin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_041_karibarai set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_042_kasituke set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_051_tanaoroshi set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_052_tanaoroshi2 set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_061_yuukasyouken set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_071_koteishisan set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_081_siharaitegata set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_091_kaikakekin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_092_kaikakekin2 set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_101_kariukekin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_102_gensen set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_111_kariirekin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_121_totiuriage set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_131_uriagedaka set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_141_yakuin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_142_jinkenhi set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_151_tidaiyatin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_152_kenrikin set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_153_kougyou set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_161_zatueki set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_162_zatuson set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_171_sonota1 set ShowKeiZero = 0 where ShowKeiZero is null
		UPDATE uc_181_sonota2 set ShowKeiZero = 0 where ShowKeiZero is null

		-- —“ŠO“o˜^‚Ìo—ÍŒ`®‚É‰Šú’lƒZƒbƒg
		UPDATE uc_052_tanaoroshi2 set OutRangai = 0 where OutRangai is null
		UPDATE uc_092_kaikakekin2 set OutRangai = 0 where OutRangai is null

		-- vtbl
		DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
		INSERT INTO vtbl (type, objname, vers)
		VALUES (1001, 'DBUchiwake', 5)

		set @uchiver = 5
	end
END
GO

BEGIN
	SET NOCOUNT ON;
	-- Še—l®‚É‰Šú’lƒZƒbƒg
	UPDATE uc_092_kaikakekin2 set ShowKeiZero = 0 where ShowKeiZero is null
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
	if( @uchiver = 5 ) begin
		-- uc_inf_ver
		DELETE uc_inf_ver WHERE Seq=1
		INSERT INTO uc_inf_ver(Seq,Version) VALUES (1, 6);
	end
END
GO

BEGIN
		SET NOCOUNT ON;

		----------------
		--	ver5->6(2/2)
		----------------

  		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver = 5 ) begin

		-- uc_021_uketoritegata ’lƒZƒbƒg
		UPDATE uc_021_uketoritegata set KnSeq = 0
		UPDATE uc_021_uketoritegata set KnOrder = 0
		-- *KnName‚Í‰Šú’lNULL‚ÅOK

		-- ”Ä—p•Ï”‚Æ”Ä—p’è”‚Ì’Ç‰ÁB¡Œã—l®–ˆ‚ÉˆÙ‚È‚é’l‚ğ‚Â‚à‚Ì‚Í‚±‚±‚É“ü—Í‚·‚éB
		-- GeneralConstVal D0 : ‹à—Z‹@ŠÖ‚ğˆê’i•\¦‚·‚é‚©H‚Ì§Œä
		-- GeneralVar D0 : ã‹L‚Ìƒ`ƒFƒbƒNó‘Ô
		-- uc_inf_sub (Table update)
		UPDATE uc_inf_sub set GeneralConstVal = 1, GeneralVar = 0 where FormSeq = 1
		UPDATE uc_inf_sub set GeneralConstVal = 1, GeneralVar = 0 where FormSeq = 2
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 3
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 4
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 5
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 6
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 7
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 8
		UPDATE uc_inf_sub set GeneralConstVal = 1, GeneralVar = 0 where FormSeq = 9
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 10
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 11
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 12
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 13
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 14
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 15
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 16
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 17
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 18
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 19
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 20
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 21
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 22
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 101
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 102
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 103
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 104
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 105
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 201
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 202
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 203
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 204
		UPDATE uc_inf_sub set GeneralConstVal = 0, GeneralVar = 0 where FormSeq = 205

		-- vtbl
		DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
		INSERT INTO vtbl (type, objname, vers)
		VALUES (1001, 'DBUchiwake', 6)

		set @uchiver = 6
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

		if( @uchiver = 6 ) begin
		
			-- uc_inf_ver
			DELETE uc_inf_ver WHERE Seq=1
			DELETE uc_inf_ver WHERE Seq=2
			INSERT INTO uc_inf_ver(Seq,Version) VALUES (1, 7);
			INSERT INTO uc_inf_ver(Seq,Version) VALUES (2, 2);
			
			-- uc_inf_sub óæèŒ`‚Ì“à–ó‘‚Ìu‰È–Ú–¼‚ğ“ü‚èƒ^ƒCƒgƒ‹‚Åˆóü‚·‚évON
			UPDATE uc_inf_sub SET OpTitleKnFg = 1 WHERE FormSeq = 2

			-- 			  u‰È–Ús‚ğˆóü‚·‚év‚ÌEnableİ’è’l
			UPDATE uc_inf_sub SET GeneralConstVal = 3 WHERE FormSeq = 2
			UPDATE uc_inf_sub SET GeneralConstVal = 2 WHERE FormSeq = 13
			
			-- uc_inf_sub_omit –ğˆõ•ñVè“–‹y‚ÑlŒ”ï‚Ì“à–ó‘‚ÌÈ—ª•¶šİ’è‚ÉZŠ‚ğ’Ç‰Á
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 16 AND ItemSeq = 4
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES( 16, 4, 'ZŠ', 0, 'V', 8 )

			-- uc_ren_main ‰È–Úc‚‚ğ‘I‘ğ‰Â”\
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 2
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 3
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 4
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 5
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 9
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 10
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 11
			UPDATE uc_ren_main SET OpRenTypeKz = 1 WHERE FormSeq = 13

			-- 				 ‰È–Ú–¼‚ğÅ‘å8•¶š‚©‚ç12•¶š‚ÉŠg’£
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_lst_kamoku ALTER COLUMN KnName varchar(30) 

			-- uc_lst_item_sort óæèŒ`‚Ì“à–ó‘‚É‰È–Úƒ\[ƒg’Ç‰Á
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 2 AND ItemSeq = 8
			IF(@udcount = 0)
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES(2, 8, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL)

			-- Še—l®ƒe[ƒuƒ‹‚Ì‰È–Ú–¼‚ğ12•¶š‚ÉŠg’£
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_011_yotyokin'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_011_yotyokin ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_021_uketoritegata ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_031_urikakekin ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_041_karibarai ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_051_tanaoroshi'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_051_tanaoroshi ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_061_yuukasyouken'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_061_yuukasyouken ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_091_kaikakekin ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_101_kariukekin ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_111_kariirekin'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_111_kariirekin ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_151_tidaiyatin'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_161_zatueki ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_162_zatuson ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_171_sonota1 ALTER COLUMN KnName varchar(30)
			IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_181_sonota2'),'KnName','PRECISION')) is not null
				ALTER TABLE uc_181_sonota2 ALTER COLUMN KnName varchar(30)

		end
END
GO

BEGIN
	SET NOCOUNT ON;

	-- uc_inf_sub_omit –ğˆõ•ñVè“–‹y‚ÑlŒ”ï‚Ì“à–ó‘‚ÌÈ—ª•¶šİ’è‚ÉZŠ‚ğ’Ç‰Á
	set @udcount = 0;
	SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 16 AND ItemSeq = 4;
	IF(@udcount = 0)
		INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES( 16, 4, 'ZŠ', 0, 'V', 8 )

	--  ‰È–Ú–¼‚ğÅ‘å8•¶š‚©‚ç12•¶š‚ÉŠg’£
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_kamoku'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_lst_kamoku ALTER COLUMN KnName varchar(30) 

	-- Še—l®ƒe[ƒuƒ‹‚Ì‰È–Ú–¼‚ğ12•¶š‚ÉŠg’£
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_011_yotyokin'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_011_yotyokin ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_021_uketoritegata ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_031_urikakekin ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_041_karibarai ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_051_tanaoroshi'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_051_tanaoroshi ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_061_yuukasyouken'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_061_yuukasyouken ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_091_kaikakekin ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_101_kariukekin ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_111_kariirekin'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_111_kariirekin ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_161_zatueki ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_162_zatuson ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_171_sonota1 ALTER COLUMN KnName varchar(30)
	IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_181_sonota2'),'KnName','PRECISION')) is not null
		ALTER TABLE uc_181_sonota2 ALTER COLUMN KnName varchar(30)

END
GO

BEGIN
		----------------
		--	ver6->7(2/2)
		----------------

		SET NOCOUNT ON;

  		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver = 6 ) begin
			-- uc_ren_main
			UPDATE uc_inf_main set GeneralConstVal = 0, GeneralVar = 0 where  GeneralConstVal is null and GeneralVar is null

			-- uc_lst_kamoku
			UPDATE uc_lst_kamoku set KnKana = 'Ä³»ŞÖ·' WHERE KnSeq = 101 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ÌÂ³Ö·İ' WHERE KnSeq = 102 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ã²·Ö·İ' WHERE KnSeq = 103 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Â³ÁÖ·İ' WHERE KnSeq = 104 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ÂĞÀÃÖ·' WHERE KnSeq = 105 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ÍŞÂÀŞİ' WHERE KnSeq = 106 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¹Şİ·İ' WHERE KnSeq = 107 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '³¹ÄØÃ¶' WHERE KnSeq = 201 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '³Ø¶¹·İ' WHERE KnSeq = 301 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ğ¼­³Æ­' WHERE KnSeq = 302 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¶İ¾²º³' WHERE KnSeq = 303 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¶ØÊŞ×²' WHERE KnSeq = 401 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ï´ÜÀ¼·' WHERE KnSeq = 402 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ï´ÊŞ×²' WHERE KnSeq = 403 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¶¼Â¹·İ' WHERE KnSeq = 501 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¼®³Ëİ' WHERE KnSeq = 601 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¾²Ëİ' WHERE KnSeq = 602 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Êİ¾²Ëİ' WHERE KnSeq = 603 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¼¶¶ØËİ' WHERE KnSeq = 604 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¹Şİ»Ş²' WHERE KnSeq = 605 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Á®¿Ş³Ë' WHERE KnSeq = 606 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '»·Ş®³¸' WHERE KnSeq = 607 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ì¸»İÌŞ' WHERE KnSeq = 608 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ğ¾²º³' WHERE KnSeq = 609 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ÊŞ²ÊŞ²' WHERE KnSeq = 701 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ïİ·' WHERE KnSeq = 702 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¿ÉÀ' WHERE KnSeq = 703 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¼Ê×²Ã¶' WHERE KnSeq = 901 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¶²¶¹·İ' WHERE KnSeq = 1001 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ĞÊŞ×²·' WHERE KnSeq = 1002 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ĞÊŞ×²Ë' WHERE KnSeq = 1003 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'º³¼ŞĞÊ' WHERE KnSeq = 1004 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¶Ø³¹·İ' WHERE KnSeq = 1101 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ï´³¹·İ' WHERE KnSeq = 1102 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '±½Ş¶Ø·' WHERE KnSeq = 1103 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '·­³' WHERE KnSeq = 1201 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'À²' WHERE KnSeq = 1202 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Î³' WHERE KnSeq = 1203 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ø' WHERE KnSeq = 1204 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ê²' WHERE KnSeq = 1205 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ë' WHERE KnSeq = 1206 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Àİ·¶Ø²' WHERE KnSeq = 1301 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Á®³·¶Ø' WHERE KnSeq = 1302 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '³Ø±¹Ş' WHERE KnSeq = 1401 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Á­³¶²Ã' WHERE KnSeq = 1402 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'Ô¸²İÎ³' WHERE KnSeq = 1701 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¼Ş­³·Ş' WHERE KnSeq = 1702 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¼Ş­³·Ş' WHERE KnSeq = 1703 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ÁÀŞ²' WHERE KnSeq = 1801 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ÔÁİ' WHERE KnSeq = 1802 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '»Ş¯¼­³' WHERE KnSeq = 2101 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '³¹ÄØØ¿' WHERE KnSeq = 2102 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '³¹ÄØÊ²' WHERE KnSeq = 2103 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ºÃ²¼»İ' WHERE KnSeq = 2104 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '»Ş¯¿İ¼' WHERE KnSeq = 2201 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¼Ê×²Ø¿' WHERE KnSeq = 2202 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = '¶¼ÀŞµÚ' WHERE KnSeq = 2203 and KnKana is null
			UPDATE uc_lst_kamoku set KnKana = 'ºÃ²¼»İ' WHERE KnSeq = 2204 and KnKana is null

			set @udcount = 0;
			SELECT @udcount = count(KnSeq) FROM uc_lst_kamoku WHERE KnSeq = 202 AND FormSeq = 2
			IF(@udcount = 0)
				INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana)
				VALUES (202, 2, 2, NULL, 'Š„ˆøèŒ`', 0, 1, 'ÜØËŞ·Ã')

			-- vtbl
			DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
			INSERT INTO vtbl (type, objname, vers)
			VALUES (1001, 'DBUchiwake', 7)

			set @uchiver = 7
		end
END
GO

BEGIN
	SET NOCOUNT ON;

	-- uc_ren_main
	UPDATE uc_inf_main set GeneralConstVal = 0 WHERE GeneralConstVal is null
	UPDATE uc_inf_main set GeneralVar = 0 WHERE GeneralVar is null

	-- uc_lst_kamoku
	UPDATE uc_lst_kamoku set KnKana = 'Ä³»ŞÖ·' WHERE KnSeq = 101 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ÌÂ³Ö·İ' WHERE KnSeq = 102 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ã²·Ö·İ' WHERE KnSeq = 103 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Â³ÁÖ·İ' WHERE KnSeq = 104 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ÂĞÀÃÖ·' WHERE KnSeq = 105 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ÍŞÂÀŞİ' WHERE KnSeq = 106 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¹Şİ·İ' WHERE KnSeq = 107 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '³¹ÄØÃ¶' WHERE KnSeq = 201 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '³Ø¶¹·İ' WHERE KnSeq = 301 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ğ¼­³Æ­' WHERE KnSeq = 302 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¶İ¾²º³' WHERE KnSeq = 303 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¶ØÊŞ×²' WHERE KnSeq = 401 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ï´ÜÀ¼·' WHERE KnSeq = 402 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ï´ÊŞ×²' WHERE KnSeq = 403 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¶¼Â¹·İ' WHERE KnSeq = 501 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¼®³Ëİ' WHERE KnSeq = 601 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¾²Ëİ' WHERE KnSeq = 602 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Êİ¾²Ëİ' WHERE KnSeq = 603 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¼¶¶ØËİ' WHERE KnSeq = 604 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¹Şİ»Ş²' WHERE KnSeq = 605 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Á®¿Ş³Ë' WHERE KnSeq = 606 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '»·Ş®³¸' WHERE KnSeq = 607 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ì¸»İÌŞ' WHERE KnSeq = 608 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ğ¾²º³' WHERE KnSeq = 609 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ÊŞ²ÊŞ²' WHERE KnSeq = 701 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ïİ·' WHERE KnSeq = 702 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¿ÉÀ' WHERE KnSeq = 703 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¼Ê×²Ã¶' WHERE KnSeq = 901 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¶²¶¹·İ' WHERE KnSeq = 1001 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ĞÊŞ×²·' WHERE KnSeq = 1002 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ĞÊŞ×²Ë' WHERE KnSeq = 1003 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'º³¼ŞĞÊ' WHERE KnSeq = 1004 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¶Ø³¹·İ' WHERE KnSeq = 1101 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ï´³¹·İ' WHERE KnSeq = 1102 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '±½Ş¶Ø·' WHERE KnSeq = 1103 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '·­³' WHERE KnSeq = 1201 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'À²' WHERE KnSeq = 1202 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Î³' WHERE KnSeq = 1203 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ø' WHERE KnSeq = 1204 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ê²' WHERE KnSeq = 1205 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ë' WHERE KnSeq = 1206 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Àİ·¶Ø²' WHERE KnSeq = 1301 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Á®³·¶Ø' WHERE KnSeq = 1302 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '³Ø±¹Ş' WHERE KnSeq = 1401 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Á­³¶²Ã' WHERE KnSeq = 1402 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'Ô¸²İÎ³' WHERE KnSeq = 1701 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¼Ş­³·Ş' WHERE KnSeq = 1702 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¼Ş­³·Ş' WHERE KnSeq = 1703 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ÁÀŞ²' WHERE KnSeq = 1801 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ÔÁİ' WHERE KnSeq = 1802 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '»Ş¯¼­³' WHERE KnSeq = 2101 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '³¹ÄØØ¿' WHERE KnSeq = 2102 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '³¹ÄØÊ²' WHERE KnSeq = 2103 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ºÃ²¼»İ' WHERE KnSeq = 2104 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '»Ş¯¿İ¼' WHERE KnSeq = 2201 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¼Ê×²Ø¿' WHERE KnSeq = 2202 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = '¶¼ÀŞµÚ' WHERE KnSeq = 2203 AND KnKana is null
	UPDATE uc_lst_kamoku set KnKana = 'ºÃ²¼»İ' WHERE KnSeq = 2204 AND KnKana is null

	set @udcount = 0;
	SELECT @udcount = count(KnSeq) FROM uc_lst_kamoku WHERE KnSeq = 202 AND FormSeq = 2
	IF(@udcount = 0)
		INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana) VALUES (202, 2, 2, NULL, 'Š„ˆøèŒ`', 0, 1, 'ÜØËŞ·Ã')
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

		if( @uchiver = 7 ) begin
			-- uc_inf_ver
			UPDATE uc_inf_ver SET Version = 8 WHERE Seq = 1
			
			-- ƒ†[ƒU[İ’è¨uƒvƒŒƒrƒ…[Aˆóü‚ÉˆêŠ‡ˆóü‰æ–Ê‚ğ•\¦‚·‚évƒfƒtƒHƒ‹ƒgON
			UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, 0x04, 1)
			-- ƒ†[ƒU[İ’è¨uæˆøæA‹à—Z‹@ŠÖ‚ğ©“®‚Å“o˜^‚·‚évƒfƒtƒHƒ‹ƒgON
			UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar, 1, 0x01, 1)
			
			
			-- uc_lst_item_sort.OrderStrŠg’£ŒãAXV
			ALTER TABLE uc_lst_item_sort ALTER COLUMN OrderStr varchar(300)
			-- ]—ˆ‚©‚ç©“®“o˜^‚ğ‚µ‚È‚©‚Á‚½ƒe[ƒuƒ‹
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' WHERE FormSeq = 5  AND ItemSeq = 1
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) 	END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' WHERE FormSeq = 13 AND ItemSeq = 2
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' WHERE FormSeq = 21 AND ItemSeq = 3
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' WHERE FormSeq = 22 AND ItemSeq = 3
			-- ]—ˆ‚©‚ç©“®“o˜^‚ğ‚µ‚Ä‚¢‚½ƒe[ƒuƒ‹i‚¾‚ªA¡‰ñ‚©‚ç©“®“o˜^‚·‚é‚©‚Ç‚¤‚©‚Ìƒ`ƒFƒbƒNƒ{ƒbƒNƒX‚ª‚Å‚«‚½‚Ì‚Å‚µ‚È‚­‚È‚é‚±‚Æ‚à‚ ‚éj
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE BkOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) END, BkOrder, (BkName1 + BkName2) COLLATE Japanese_CS_AS_KS_WS ASC, BkName1 COLLATE Japanese_CS_AS_KS_WS ASC, BkName2 COLLATE Japanese_CS_AS_KS_WS ASC' 		, FieldSykei = 'BkName1', FieldSykei2 = 'BkName1' WHERE FormSeq = 1  AND ItemSeq = 1
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' , FieldSykei = 'AdName1', FieldSykei2 = 'AdName2' WHERE FormSeq = 2  AND ItemSeq = 1
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' , FieldSykei = 'AdName1', FieldSykei2 = 'AdName2' WHERE FormSeq = 3  AND ItemSeq = 2
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' , FieldSykei = 'AdName1', FieldSykei2 = 'AdName2' WHERE FormSeq = 4  AND ItemSeq = 2
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' , FieldSykei = 'AdName1', FieldSykei2 = 'AdName2' WHERE FormSeq = 9  AND ItemSeq = 1
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' , FieldSykei = 'AdName1', FieldSykei2 = 'AdName2' WHERE FormSeq = 10  AND ItemSeq = 2
			UPDATE uc_lst_item_sort SET OrderStr = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC' , FieldSykei = 'AdName1', FieldSykei2 = 'AdName2' WHERE FormSeq = 11  AND ItemSeq = 2
			
			-- uc_inf_sub
			UPDATE uc_inf_sub SET FormOrder = 43 where FormSeq = 201
			UPDATE uc_inf_sub SET FormOrder = 44 where FormSeq = 202
			UPDATE uc_inf_sub SET FormOrder = 45 where FormSeq = 203
			UPDATE uc_inf_sub SET FormOrder = 46 where FormSeq = 204
			UPDATE uc_inf_sub SET FormOrder = 47 where FormSeq = 205
			
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 106 AND FormNo = 17 AND FormNo2 = 6
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (106, 17, 6, 0, 28, '‡P', '|‚U',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚U',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚U',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚U',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 107 AND FormNo = 17 AND FormNo2 = 7
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (107, 17, 7, 0, 29, '‡P', '|‚V',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚V',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚V',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚V',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 108 AND FormNo = 17 AND FormNo2 = 8
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (108, 17, 8, 0, 30, '‡P', '|‚W',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚W',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚W',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚W',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 109 AND FormNo = 17 AND FormNo2 = 9
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (109, 17, 9, 0, 31, '‡P', '|‚X',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚X',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚X',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚X',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 110 AND FormNo = 17 AND FormNo2 = 10
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (110, 17,10, 0, 32, '‡P', '|‚P‚O', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚O',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚O', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚O', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 111 AND FormNo = 17 AND FormNo2 = 11
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (111, 17,11, 0, 33, '‡P', '|‚P‚P', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚P', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚P', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚P', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 112 AND FormNo = 17 AND FormNo2 = 12
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (112, 17,12, 0, 34, '‡P', '|‚P‚Q', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚Q',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚Q', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚Q', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 113 AND FormNo = 17 AND FormNo2 = 13
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (113, 17,13, 0, 35, '‡P', '|‚P‚R', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚R',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚R', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚R', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 114 AND FormNo = 17 AND FormNo2 = 14
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (114, 17,14, 0, 36, '‡P', '|‚P‚S', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚S',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚S', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚S', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 115 AND FormNo = 17 AND FormNo2 = 15
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (115, 17,15, 0, 37, '‡P', '|‚P‚T', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚T',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚T', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚T', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 116 AND FormNo = 17 AND FormNo2 = 16
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (116, 17,16, 0, 38, '‡P', '|‚P‚U', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚U',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚U', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚U', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 117 AND FormNo = 17 AND FormNo2 = 17
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (117, 17,17, 0, 39, '‡P', '|‚P‚V', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚V',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚V', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚V', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 118 AND FormNo = 17 AND FormNo2 = 18
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (118, 17,18, 0, 40, '‡P', '|‚P‚W', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚W',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚W', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚W', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 119 AND FormNo = 17 AND FormNo2 = 19
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (119, 17,19, 0, 41, '‡P', '|‚P‚X', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚X',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚X', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚P‚X', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 120 AND FormNo = 17 AND FormNo2 = 20
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (120, 17,20, 0, 42, '‡P', '|‚Q‚O', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚Q‚O',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚Q‚O', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡@|‚Q‚O', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);

			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 206 AND FormNo = 18 AND FormNo2 = 6
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (206, 18, 6, 0, 48, '‡Q', '|‚U',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚U',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚U',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚U',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 207 AND FormNo = 18 AND FormNo2 = 7
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (207, 18, 7, 0, 49, '‡Q', '|‚V',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚V',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚V',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚V',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 208 AND FormNo = 18 AND FormNo2 = 8
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (208, 18, 8, 0, 50, '‡Q', '|‚W',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚W',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚W',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚W',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 209 AND FormNo = 18 AND FormNo2 = 9
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (209, 18, 9, 0, 51, '‡Q', '|‚X',   '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚X',    NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚X',   NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚X',   0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 210 AND FormNo = 18 AND FormNo2 = 10
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (210, 18,10, 0, 52, '‡Q', '|‚P‚O', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚O',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚O', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚O', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 211 AND FormNo = 18 AND FormNo2 = 11
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (211, 18,11, 0, 53, '‡Q', '|‚P‚P', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚P', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚P', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚P', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 212 AND FormNo = 18 AND FormNo2 = 12
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (212, 18,12, 0, 54, '‡Q', '|‚P‚Q', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚Q',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚Q', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚Q', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 213 AND FormNo = 18 AND FormNo2 = 13
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (213, 18,13, 0, 55, '‡Q', '|‚P‚R', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚R',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚R', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚R', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 214 AND FormNo = 18 AND FormNo2 = 14
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (214, 18,14, 0, 56, '‡Q', '|‚P‚S', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚S',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚S', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚S', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 215 AND FormNo = 18 AND FormNo2 = 15
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (215, 18,15, 0, 57, '‡Q', '|‚P‚T', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚T',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚T', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚T', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 216 AND FormNo = 18 AND FormNo2 = 16
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (216, 18,16, 0, 58, '‡Q', '|‚P‚U', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚U',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚U', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚U', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 217 AND FormNo = 18 AND FormNo2 = 17
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (217, 18,17, 0, 59, '‡Q', '|‚P‚V', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚V',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚V', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚V', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 218 AND FormNo = 18 AND FormNo2 = 18
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (218, 18,18, 0, 60, '‡Q', '|‚P‚W', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚W',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚W', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚W', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 219 AND FormNo = 18 AND FormNo2 = 19
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (219, 18,19, 0, 61, '‡Q', '|‚P‚X', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚X',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚X', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚P‚X', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub WHERE FormSeq = 220 AND FormNo = 18 AND FormNo2 = 20
			IF(@udcount = 0)
				INSERT INTO uc_inf_sub(FormSeq,FormNo,FormNo2,FormNo3,FormOrder,TitleNo,TitleNo2,TitleOrg,TitleOrgTujo,TitleSave,TitleSaveTujo,Title,Sort1,Sort2,Sort3,OpSykeiAuto,OpSykeiName,OpCykeiName,OpSykeiKakko,OpCykeiKakko,OpIkkatuFg,OpIkkatu,OpIkkatuVal,OpIkkatuValOrg,OpOutSyokei,OpOutNullLine,OpKaiPage,OutKei,OpTitleKnFg,OpTitleKn,CurPage,CurRow,CurField,CurCtlIndex,GeneralConstVal,GeneralVar)
				VALUES (220, 18,20, 0, 62, '‡Q', '|‚Q‚O', '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚Q‚O',  NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚Q‚O', NULL, '‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‡A|‚Q‚O', 0, 0, 0, 0, '¬Œv', '’†Œv', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0);
			
			-- uc_inf_yokuki
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 106
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (106, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 107
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (107, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 108
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (108, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 109
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (109, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 110
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (110, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 111
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (111, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 112
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (112, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 113
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (113, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 114
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (114, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 115
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (115, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 116
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (116, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 117
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (117, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 118
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (118, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 119
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (119, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 120
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (120, 1, 1, 1, 1, 1, 1);

			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 206
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (206, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 207
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (207, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 208
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (208, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 209
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (209, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 210
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (210, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 211
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (211, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 212
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (212, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 213
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (213, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 214
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (214, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 215
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (215, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 216
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (216, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 217
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (217, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 218
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (218, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 219
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (219, 1, 1, 1, 1, 1, 1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_yokuki WHERE FormSeq = 220
			IF(@udcount = 0)
				INSERT INTO uc_inf_yokuki(FormSeq,OpYokukiFg,OpYokuki,OpClearFg,OpClear,OpHokanFg,OpHokan)
				VALUES (220, 1, 1, 1, 1, 1, 1);
			
			-- uc_inf_sub_omit
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 106
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (106, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (106, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (106, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 107
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (107, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (107, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (107, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 108
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (108, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (108, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (108, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 109
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (109, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (109, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (109, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 110
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (110, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (110, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (110, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 111
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (111, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (111, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (111, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 112
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (112, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (112, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (112, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 113
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (113, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (113, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (113, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 114
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (114, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (114, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (114, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 115
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (115, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (115, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (115, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 116
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (116, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (116, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (116, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 117
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (117, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (117, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (117, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 118
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (118, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (118, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (118, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 119
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (119, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (119, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (119, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 120
			IF(@udcount = 0) begin
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (120, 1, '‰È–Ú', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (120, 2, '‘Šèæi–¼Ìj', 0, 'V', 8);
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (120, 3, '‘ŠèæiŠİ’nj', 0, 'V', 8);
			end

			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 206
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (206, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 207
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (207, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 208
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (208, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 209
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (209, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 210
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (210, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 211
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (211, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 212
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (212, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 213
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (213, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 214
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (214, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 215
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (215, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 216
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (216, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 217
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (217, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 218
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (218, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 219
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (219, 1, '‰È–Ú', 0, 'V', 8);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 220
			IF(@udcount = 0) 
				INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) VALUES (220, 1, '‰È–Ú', 0, 'V', 8);
			
			-- uc_lst_item_sort
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 106
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (106, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (106, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (106, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (106, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (106, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 107
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (107, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (107, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (107, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (107, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (107, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 108
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (108, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (108, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (108, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (108, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (108, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 109
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (109, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (109, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (109, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (109, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (109, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 110
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (110, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (110, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (110, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (110, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (110, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 111
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (111, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (111, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (111, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (111, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (111, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 112
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (112, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (112, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (112, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (112, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (112, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 113
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (113, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (113, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (113, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (113, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (113, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 114
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (114, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (114, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (114, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (114, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (114, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 115
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (115, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (115, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (115, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (115, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (115, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 116
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (116, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (116, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (116, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (116, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (116, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 117
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (117, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (117, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (117, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (117, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (117, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 118
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (118, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (118, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (118, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (118, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (118, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 119
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (119, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (119, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (119, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (119, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (119, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 120
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (120, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (120, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (120, 2, '‘Šèæi–¼Ìj', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (120, 3, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (120, 4, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 206
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (206, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (206, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (206, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (206, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 207
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (207, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (207, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (207, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (207, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 208
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (208, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (208, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (208, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (208, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 209
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (209, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (209, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (209, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (209, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 210
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (210, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (210, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (210, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (210, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 211
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (211, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (211, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (211, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (211, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 212
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (212, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (212, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (212, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (212, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 213
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (213, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (213, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (213, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (213, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 214
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (214, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (214, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (214, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (214, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 215
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (215, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (215, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (215, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (215, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 216
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (216, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (216, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (216, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (216, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 217
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (217, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (217, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (217, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (217, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 218
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (218, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (218, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (218, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (218, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 219
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (219, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (219, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (219, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (219, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 220
			IF(@udcount = 0) begin
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (220, 0, '‚È‚µ', 0, 0, 0, NULL, NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (220, 1, '‰È–Ú', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (220, 2, 'Šú––Œ»İ‚i‘å‚«‚¢‡j', 0, 0, 0, 'Val DESC', NULL, NULL);
				INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2)
				VALUES (220, 3, 'Šú––Œ»İ‚i¬‚³‚¢‡j', 0, 0, 0, 'Val ASC', NULL, NULL);
			end

			-- uc_ren_main
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 106
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (106, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 107
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (107, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 108
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (108, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 109
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (109, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 110
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (110, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 111
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (111, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 112
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (112, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 113
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (113, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 114
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (114, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 115
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (115, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 116
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (116, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 117
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (117, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 118
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (118, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 119
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (119, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 120
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (120, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 206
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (206, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 207
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (207, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 208
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (208, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 209
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (209, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 210
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (210, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 211
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (211, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 212
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (212, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 213
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (213, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 214
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (214, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 215
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (215, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 216
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (216, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 217
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (217, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 218
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (218, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 219
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (219, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_ren_main WHERE FormSeq = 220
			IF(@udcount = 0)
				INSERT INTO uc_ren_main(FormSeq,OpRenType,OpRenTypeKz,OpRenTypeEz,OpRenTypeDa,OpRenTypeKs,OpTkcodFrom,OpTkcodTo,OpOutZeroEn,OpGassan,OpUpdType,OpOldRenType)
				VALUES (220, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1);

			-- uc_inf_sonota_item
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 206
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (206, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 207
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (207, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 208
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (208, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 209
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (209, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 210
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (210, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 211
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (211, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 212
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (212, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 213
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (213, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 214
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (214, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 215
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (215, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 216
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (216, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 217
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (217, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 218
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (218, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 219
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (219, '€–Ú');
			set @udcount = 0;
			SELECT @udcount = count(FormSeq) FROM uc_inf_sonota_item WHERE FormSeq = 220
			IF(@udcount = 0)
				INSERT INTO uc_inf_sonota_item(FormSeq,ItemName) VALUES (220, '€–Ú');
		end
END
GO

BEGIN
		----------------
		--	ver7->8(2/2)
		----------------

		SET NOCOUNT ON;

  		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver = 7 ) begin
			-- uc_lst_kamoku.IkkatuVal‚É‘Î‰‚·‚é—l®ƒV[ƒPƒ“ƒX‚ÌˆêŠ‡‹àŠz‚ğƒRƒs[‚·‚é
			UPDATE uc_lst_kamoku SET IkkatuVal = OpIkkatuVal FROM uc_lst_kamoku INNER JOIN uc_inf_sub as tbl1 ON uc_lst_kamoku.FormSeq = tbl1.FormSeq WHERE uc_lst_kamoku.IkkatuVal is null
			
			-- uc_lst_address.IkkatuVali“¾ˆÓæj‚É‘İ•t‹à‹y‚Ñóæ—˜‘§‚Ì“à–ó‘‚ÌˆêŠ‡‹àŠz‚ğƒRƒs[‚·‚éitable_version8“_‚É‚¨‚¢‚ÄA“¾ˆÓæ‚ÌˆêŠ‡‹àŠz‚ğs‚¤‚Ì‚Í‘İ•t‹à‹y‚Ñóæ—˜‘§‚Ì“à–ó‘‚Ì‚İ‚Ìˆ×)
			-- WHERE GrSeq = 3‚Ì3‚ÍID_ADDRESSGR_URI
			UPDATE uc_lst_address SET IkkatuVal = OpIkkatuVal FROM uc_lst_address INNER JOIN uc_inf_sub as tbl1 ON tbl1.FormSeq = 5 WHERE GrSeq = 3 AND uc_lst_address.IkkatuVal is null
			
			-- uc_lst_address.IkkatuValid“üæj‚Éx•¥èŒ`‚Ì“à–ó‘ˆêŠ‡‹àŠz‚ğƒRƒs[‚·‚éitable_version8“_‚É‚¨‚¢‚ÄAd“üæ‚ÌˆêŠ‡‹àŠz‚ğs‚¤‚Íx•¥èŒ`‚Ì“à–ó‘‚Ì‚İ‚Ìˆ×)
			-- WHERE GrSeq = 10‚Ì10‚ÍID_ADDRESSGR_KAI
			UPDATE uc_lst_address SET IkkatuVal = OpIkkatuVal FROM uc_lst_address INNER JOIN uc_inf_sub as tbl1 ON tbl1.FormSeq = 9  WHERE GrSeq = 10 AND uc_lst_address.IkkatuVal is null

			-- vtbl
			DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
			INSERT INTO vtbl (type, objname, vers) VALUES (1001, 'DBUchiwake', 8)

			set @uchiver = 8
		end
END
GO

BEGIN
		-- uc_lst_kamoku.IkkatuVal‚É‘Î‰‚·‚é—l®ƒV[ƒPƒ“ƒX‚ÌˆêŠ‡‹àŠz‚ğƒRƒs[‚·‚é
		UPDATE uc_lst_kamoku SET IkkatuVal = OpIkkatuVal FROM uc_lst_kamoku INNER JOIN uc_inf_sub as tbl1 ON uc_lst_kamoku.FormSeq = tbl1.FormSeq WHERE uc_lst_kamoku.IkkatuVal is null

		-- uc_lst_address.IkkatuVali“¾ˆÓæj‚É‘İ•t‹à‹y‚Ñóæ—˜‘§‚Ì“à–ó‘‚ÌˆêŠ‡‹àŠz‚ğƒRƒs[‚·‚éitable_version8“_‚É‚¨‚¢‚ÄA“¾ˆÓæ‚ÌˆêŠ‡‹àŠz‚ğs‚¤‚Ì‚Í‘İ•t‹à‹y‚Ñóæ—˜‘§‚Ì“à–ó‘‚Ì‚İ‚Ìˆ×)
		-- WHERE GrSeq = 3‚Ì3‚ÍID_ADDRESSGR_URI
		UPDATE uc_lst_address SET IkkatuVal = OpIkkatuVal FROM uc_lst_address INNER JOIN uc_inf_sub as tbl1 ON tbl1.FormSeq = 5 WHERE GrSeq = 3 AND uc_lst_address.IkkatuVal is null

		-- uc_lst_address.IkkatuValid“üæj‚Éx•¥èŒ`‚Ì“à–ó‘ˆêŠ‡‹àŠz‚ğƒRƒs[‚·‚éitable_version8“_‚É‚¨‚¢‚ÄAd“üæ‚ÌˆêŠ‡‹àŠz‚ğs‚¤‚Íx•¥èŒ`‚Ì“à–ó‘‚Ì‚İ‚Ìˆ×)
		-- WHERE GrSeq = 10‚Ì10‚ÍID_ADDRESSGR_KAI
		UPDATE uc_lst_address SET IkkatuVal = OpIkkatuVal FROM uc_lst_address INNER JOIN uc_inf_sub as tbl1 ON tbl1.FormSeq = 9  WHERE GrSeq = 10 AND uc_lst_address.IkkatuVal is null
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

		if( @uchiver = 8 ) begin
			-- uc_inf_ver
			UPDATE uc_inf_ver SET Version = 9 WHERE Seq = 1
		end
END
GO

BEGIN
		----------------
		--	ver8->9(2/2)
		----------------

		SET NOCOUNT ON;

		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver = 8 ) begin
			-- uc_lst_kamoku.PrSign‚É‰Šú’l(0)‚ğƒZƒbƒg‚·‚é
			UPDATE uc_lst_kamoku SET PrSign = 0

			-- No.160608 —l®Ø‘ÖAu‘S‚Ä‹ós‚Å‚·v‚ÌƒƒbƒZ[ƒW‚ğ•\¦‚µ‚È‚¢‚æ‚¤‚É‚µ‚½‚±‚Æ‚É”º‚¤‘Î‰
			-- ‘S‚Ä‹ós‚Ì“à–ó‘ƒf[ƒ^‚Ííœ‚·‚é

			SET NOCOUNT ON;

			-- ‡@—a’™‹à“™‚Ì“à–ó‘
			-- ‹ó”’A—İŒvA•ÅŒvA•ÛŠÇˆÈŠO‚ÌŒ”‚ğæ“¾
			set @udcount = 0
			select @udcount = count(seq) from uc_011_yotyokin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			-- ã‹LˆÈŠO‚Ìƒf[ƒ^‚ª–³‚¯‚ê‚ÎA•ÛŠÇˆÈŠO‚Ìƒf[ƒ^‚ğíœ‚·‚é
			if( @udcount = 0 ) begin
			delete from uc_011_yotyokin where FgShow<>1
			end

			-- ‡AFóæèŒ`‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_021_uketoritegata where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_021_uketoritegata where FgShow<>1
			end

			-- ‡BF”„Š|‹à‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_031_urikakekin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_031_urikakekin where FgShow<>1
			end

			-- ‡C]‚PF‰¼•¥‹à‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_041_karibarai where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_041_karibarai where FgShow<>1
			end

			-- ‡C]‚QF‘İ•t‹à‹y‚Ñóæ—˜‘§‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_042_kasituke where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_042_kasituke where FgShow<>1
			end

			-- ‡DF’I‰µ‘Y‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_051_tanaoroshi where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_051_tanaoroshi where FgShow<>1
			end

			-- ‡DF’I‰µ‘Y‚Ì“à–ó‘i—“ŠOj
			set @udcount = 0
			select @udcount = count(seq) from uc_052_tanaoroshi2 where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_052_tanaoroshi2
			end

			-- ‡EF—L‰¿ØŒ”‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_061_yuukasyouken where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_061_yuukasyouken where FgShow<>1
			end

			-- ‡FFŒÅ’è‘Y‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_071_koteishisan where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_071_koteishisan where FgShow<>1
			end

			-- ‡GFx•¥èŒ`‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_081_siharaitegata where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_081_siharaitegata where FgShow<>1
			end

			-- ‡HF”ƒŠ|‹à‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_091_kaikakekin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_091_kaikakekin where FgShow<>1
			end

			-- ‡HF”ƒŠ|‹à‚Ì“à–ó‘i—“ŠOj
			set @udcount = 0
			select @udcount = count(seq) from uc_092_kaikakekin2 where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_092_kaikakekin2
			end

			-- ‡I]‚PF‰¼ó‹à‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_101_kariukekin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_101_kariukekin where FgShow<>1
			end

			-- ‡I]‚QFŒ¹òŠ“¾Å—a‚è‹à‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_102_gensen where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_102_gensen where FgShow<>1
			end

			-- ‡JFØ“ü‹à‹y‚Ñx•¥—˜q‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_111_kariirekin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_111_kariirekin where FgShow<>1
			end

			-- ‡KF“y’n‚Ì”„ã‚“™‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_121_totiuriage where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_121_totiuriage where FgShow<>1
			end

			-- ‡LF”„ã‚“™‚Ì–‹ÆŠ•Ê“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_131_uriagedaka where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_131_uriagedaka where FgShow<>1
			end

			-- ‡M]‚PF–ğˆõ•ñVè“–“™‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_141_yakuin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_141_yakuin where FgShow<>1
			end

			-- ‡M]‚QFlŒ”ï‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_142_jinkenhi where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_142_jinkenhi where FgShow<>1
			end

			-- ‡N]‚PF’n‘ã‰Æ’À“™‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_151_tidaiyatin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_151_tidaiyatin where FgShow<>1
			end

			-- ‡N]‚QFŒ —˜‹à“™‚ÌŠú’†x•¥‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_152_kenrikin where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_152_kenrikin where FgShow<>1
			end

			-- ‡N]‚RFH‹ÆŠ—LŒ “™‚Ìg—p—¿‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_153_kougyou where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_153_kougyou where FgShow<>1
			end

			-- ‡O]‚PFG‰v“™‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_161_zatueki where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_161_zatueki where FgShow<>1
			end

			-- ‡O]‚QFG‘¹¸“™‚Ì“à–ó‘
			set @udcount = 0
			select @udcount = count(seq) from uc_162_zatuson where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_162_zatuson where FgShow<>1
			end

			-- ‡PF‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‚P
			set @udcount = 0
			select @udcount = count(seq) from uc_171_sonota1 where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_171_sonota1 where FgShow<>1
			end

			-- ‡QF‚»‚Ì‘¼‰È–Ú‚Ì“à–ó‘‚Q
			set @udcount = 0
			select @udcount = count(seq) from uc_181_sonota2 where FgFunc<>0 AND FgFunc<>14 AND FgFunc<>15 AND FgShow<>1
			if( @udcount = 0 ) begin
			delete from uc_181_sonota2 where FgShow<>1
			end

			-- vtbl
			DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
			INSERT INTO vtbl (type, objname, vers) VALUES (1001, 'DBUchiwake', 9)

			set @uchiver = 9
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

		if( @uchiver = 9 ) begin
			-- uc_inf_ver
			UPDATE uc_inf_ver SET Version = 10 WHERE Seq = 1
		end
END
GO

BEGIN
		----------------
		--	ver9->10(2/2)
		----------------

		SET NOCOUNT ON;

		-- Insert statements for procedure here
		set @uchiver = -1
		select @uchiver = vers from vtbl where type=1001 AND objname='DBUchiwake'

		if( @uchiver = 9 ) begin

			-- uc_ren_main.OpEdaren‚É‰Šú’l(0)‚ğƒZƒbƒg‚·‚é
			UPDATE uc_ren_main SET OpEdaren = 0 WHERE OpEdaren is null

			-- uc_ren_main.OpKmknameAuc_ren_main.OpF9Upd‚É‰Šú’l(0)‚ğƒZƒbƒg‚·‚é
			UPDATE uc_ren_main SET OpKmkname = 0,OpF9Upd = 0 WHERE OpKmkname is null and OpF9Upd is null

			-- uc_inf_yokuki.OpNullClearFgAuc_inf_yokuki.OpNullClear‚É‰Šú’l‚ğƒZƒbƒg‚·‚é
			UPDATE uc_inf_yokuki SET OpNullClearFg = 1,OpNullClear = 0 WHERE OpNullClearFg is null and OpNullClear is null

			-- uc_lst_item_sortƒe[ƒuƒ‹‚ÌOrderStr2‚É‰Šú’l‚ğƒZƒbƒg‚·‚é

			-- ‹âs‚ÌOrderStr2‚É‰Šú’l‚ğƒZƒbƒg
			UPDATE uc_lst_item_sort SET OrderStr2 = 'CASE BkOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) END, (isnull(BkKana1,'') + isnull(BkKana2,'')) COLLATE Japanese_CS_AS_KS_WS ASC, BkKana1 COLLATE Japanese_CS_AS_KS_WS ASC, BkKana2 COLLATE Japanese_CS_AS_KS_WS ASC, BkOrder ASC'
			WHERE FormSeq = 1 AND ItemSeq = 1 and OrderStr2 is null

			UPDATE uc_lst_item_sort SET OrderStr2 = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) END, (isnull(AdKana1,'') + isnull(AdKana2,'')) COLLATE Japanese_CS_AS_KS_WS ASC, AdKana1 COLLATE Japanese_CS_AS_KS_WS ASC, AdKana2 COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC'
			WHERE FormSeq = 13 AND ItemSeq = 2 and OrderStr2 is null

			-- æˆøæ‚ÌOrderStr2‚É‰Šú’l‚ğƒZƒbƒg
			UPDATE uc_lst_item_sort SET OrderStr2 = 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC'
			WHERE ((FormSeq = 2 AND ItemSeq = 1) OR (FormSeq = 3 AND ItemSeq = 2)  OR (FormSeq = 4 AND ItemSeq = 2) OR (FormSeq = 5 AND ItemSeq = 1) OR
			      (FormSeq = 9 AND ItemSeq = 1) OR (FormSeq = 10 AND ItemSeq = 2) OR (FormSeq = 11 AND ItemSeq = 2) OR (FormSeq = 21 AND ItemSeq = 3) OR
			      (FormSeq = 22 AND ItemSeq = 3)) and OrderStr2 is null

			-- ‰È–Ú‚ÌOrderStr2‚É‰Šú’l‚ğƒZƒbƒg
			UPDATE uc_lst_item_sort SET OrderStr2 = 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC'
			WHERE ((FormSeq = 1 AND ItemSeq = 2) OR (FormSeq = 2 AND ItemSeq = 8) OR (FormSeq = 3 AND ItemSeq = 1) OR (FormSeq = 4 AND ItemSeq = 1) OR
			      (FormSeq = 6 AND ItemSeq = 1) OR (FormSeq = 7 AND ItemSeq = 1) OR (FormSeq = 10 AND ItemSeq = 1) OR (FormSeq = 11 AND ItemSeq = 1) OR
			      (FormSeq = 12 AND ItemSeq = 3) OR (FormSeq = 13 AND ItemSeq = 1) OR (FormSeq = 14 AND ItemSeq = 1) OR (FormSeq = 18 AND ItemSeq = 1) OR
			      (FormSeq = 21 AND ItemSeq = 1) OR (FormSeq = 22 AND ItemSeq = 1) OR (FormSeq >= 101 AND FormSeq <= 120 AND ItemSeq = 1) OR 
				  (FormSeq >= 201 AND FormSeq <= 220 AND ItemSeq = 1)) AND OrderStr2 is null

			-- Še—l®‚ÌƒJƒi‚ğXV
			-- uc_011_yotyokin (Table update)
			UPDATE uc_011_yotyokin
			SET uc_011_yotyokin.BkKana1 = (CASE WHEN uc_011_yotyokin.BkOrder = 0 THEN NULL ELSE uc_lst_bank.BkKana1 END),
			uc_011_yotyokin.BkKana2 = (CASE WHEN uc_011_yotyokin.BkOrder = 0 THEN NULL ELSE uc_lst_bank.BkKana2 END),
			uc_011_yotyokin.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_011_yotyokin
			LEFT JOIN uc_lst_bank on uc_011_yotyokin.BkOrder = uc_lst_bank.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_011_yotyokin.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 1

			-- uc_021_uketoritegata (Table update)
			UPDATE uc_021_uketoritegata
			SET uc_021_uketoritegata.AdKana = (CASE WHEN uc_021_uketoritegata.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END),
			uc_021_uketoritegata.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_021_uketoritegata
			LEFT JOIN uc_lst_address on uc_021_uketoritegata.AdOrder = uc_lst_address.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_021_uketoritegata.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 2

			-- uc_031_urikakekin (Table update)
			UPDATE uc_031_urikakekin
			SET uc_031_urikakekin.AdKana = (CASE WHEN uc_031_urikakekin.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END),
			uc_031_urikakekin.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_031_urikakekin
			LEFT JOIN uc_lst_address on uc_031_urikakekin.AdOrder = uc_lst_address.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_031_urikakekin.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 3

			-- uc_041_karibarai (Table update)
			UPDATE uc_041_karibarai
			SET uc_041_karibarai.AdKana = (CASE WHEN uc_041_karibarai.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END),
			uc_041_karibarai.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_041_karibarai
			LEFT JOIN uc_lst_address on uc_041_karibarai.AdOrder = uc_lst_address.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_041_karibarai.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 4

			-- uc_042_kasituke (Table update)
			UPDATE uc_042_kasituke
			SET uc_042_kasituke.AdKana = (CASE WHEN uc_042_kasituke.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END)
			FROM uc_042_kasituke
			LEFT JOIN uc_lst_address on uc_042_kasituke.AdOrder = uc_lst_address.OrderNum

			-- uc_051_tanaoroshi (Table update)
			UPDATE uc_051_tanaoroshi
			SET uc_051_tanaoroshi.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_051_tanaoroshi
			LEFT JOIN uc_lst_Kamoku on uc_051_tanaoroshi.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 6

			-- uc_061_yuukasyouken (Table update)
			UPDATE uc_061_yuukasyouken
			SET uc_061_yuukasyouken.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_061_yuukasyouken
			LEFT JOIN uc_lst_Kamoku on uc_061_yuukasyouken.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 7

			-- uc_081_siharaitegata (Table update)
			UPDATE uc_081_siharaitegata
			SET uc_081_siharaitegata.AdKana = (CASE WHEN uc_081_siharaitegata.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END)
			FROM uc_081_siharaitegata
			LEFT JOIN uc_lst_address on uc_081_siharaitegata.AdOrder = uc_lst_address.OrderNum

			-- uc_091_kaikakekin (Table update)
			UPDATE uc_091_kaikakekin
			SET uc_091_kaikakekin.AdKana = (CASE WHEN uc_091_kaikakekin.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END),
			uc_091_kaikakekin.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_091_kaikakekin
			LEFT JOIN uc_lst_address on uc_091_kaikakekin.AdOrder = uc_lst_address.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_091_kaikakekin.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 10

			-- uc_101_kariukekin (Table update)
			UPDATE uc_101_kariukekin
			SET uc_101_kariukekin.AdKana = (CASE WHEN uc_101_kariukekin.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END),
			uc_101_kariukekin.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_101_kariukekin
			LEFT JOIN uc_lst_address on uc_101_kariukekin.AdOrder = uc_lst_address.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_101_kariukekin.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 11

			-- uc_102_gensen (Table update)
			UPDATE uc_102_gensen
			SET uc_102_gensen.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_102_gensen
			LEFT JOIN uc_lst_Kamoku on uc_102_gensen.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 12

			-- uc_111_kariirekin (Table update)
			UPDATE uc_111_kariirekin
			SET uc_111_kariirekin.AdKana1 = (CASE WHEN uc_111_kariirekin.AdOrder = 0 THEN NULL ELSE uc_lst_bank.BkKana1 END),
			uc_111_kariirekin.AdKana2 = (CASE WHEN uc_111_kariirekin.AdOrder = 0 THEN NULL ELSE uc_lst_bank.BkKana2 END),
			uc_111_kariirekin.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_111_kariirekin
			LEFT JOIN uc_lst_bank on uc_111_kariirekin.AdOrder = uc_lst_bank.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_111_kariirekin.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 13

			-- uc_121_totiuriage (Table update)
			UPDATE uc_121_totiuriage
			SET uc_121_totiuriage.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_121_totiuriage
			LEFT JOIN uc_lst_Kamoku on uc_121_totiuriage.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 14

			-- uc_151_tidaiyatin (Table update)
			UPDATE uc_151_tidaiyatin
			SET uc_151_tidaiyatin.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_151_tidaiyatin
			LEFT JOIN uc_lst_Kamoku on uc_151_tidaiyatin.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 18

			-- uc_161_zatueki (Table update)
			UPDATE uc_161_zatueki
			SET uc_161_zatueki.AdKana = (CASE WHEN uc_161_zatueki.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END),
			uc_161_zatueki.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_161_zatueki
			LEFT JOIN uc_lst_address on uc_161_zatueki.AdOrder = uc_lst_address.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_161_zatueki.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 21

			-- uc_162_zatuson (Table update)
			UPDATE uc_162_zatuson
			SET uc_162_zatuson.AdKana = (CASE WHEN uc_162_zatuson.AdOrder = 0 THEN NULL ELSE uc_lst_address.AdKana END),
			uc_162_zatuson.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_162_zatuson
			LEFT JOIN uc_lst_address on uc_162_zatuson.AdOrder = uc_lst_address.OrderNum
			LEFT JOIN uc_lst_Kamoku on uc_162_zatuson.KnOrder = uc_lst_Kamoku.KnOrder and uc_lst_Kamoku.FormSeq = 22

			-- uc_171_sonota1 (Table update)
			UPDATE uc_171_sonota1
			SET uc_171_sonota1.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_171_sonota1
			LEFT JOIN uc_lst_Kamoku on uc_171_sonota1.KnOrder = uc_lst_Kamoku.KnOrder and uc_171_sonota1.FormSeq = uc_lst_Kamoku.FormSeq

			-- uc_181_sonota2 (Table update)
			UPDATE uc_181_sonota2
			SET uc_181_sonota2.KnKana = uc_lst_Kamoku.KnKana
			FROM uc_181_sonota2
			LEFT JOIN uc_lst_Kamoku on uc_181_sonota2.KnOrder = uc_lst_Kamoku.KnOrder and uc_181_sonota2.FormSeq = uc_lst_Kamoku.FormSeq
			-- ‡B”„Š|‹ài–¢û“ü‹àj‚Ì“à–ó‘A‡C|‚P‰¼•¥‹ài‘O“n‹àj‚Ì“à–ó‘‚ÌƒAƒCƒeƒ€–¼Ì‚ğ•ÏX
			-- ‘Šèæi–¼Ìj¨ ‘Šèæ
			UPDATE uc_lst_item_sort
			SET ItemName = '‘Šèæ'
			FROM uc_lst_item_sort
			WHERE (FormSeq = 3 AND ItemSeq = 2) OR (FormSeq = 4 AND ItemSeq = 2)

			-- vtbl
			DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'
			INSERT INTO vtbl (type, objname, vers) VALUES (1001, 'DBUchiwake', 10)

			set @uchiver = 10

		end
END
GO
