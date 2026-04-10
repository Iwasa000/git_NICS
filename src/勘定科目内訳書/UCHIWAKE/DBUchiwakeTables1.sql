-- 注意！ RETURN文以降にGO文を使用しないこと！
SET NOCOUNT ON	-- osqlからsqlcmdに変更したとき、このSQLファイルが異常に遅くなったので追加
GO

IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_011_yotyokin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_011_yotyokin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_021_uketoritegata]') AND type in (N'U'))
DROP TABLE [dbo].[uc_021_uketoritegata]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_031_urikakekin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_031_urikakekin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_041_karibarai]') AND type in (N'U'))
DROP TABLE [dbo].[uc_041_karibarai]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_042_kasituke]') AND type in (N'U'))
DROP TABLE [dbo].[uc_042_kasituke]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_051_tanaoroshi]') AND type in (N'U'))
DROP TABLE [dbo].[uc_051_tanaoroshi]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_052_tanaoroshi2]') AND type in (N'U'))
DROP TABLE [dbo].[uc_052_tanaoroshi2]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_061_yuukasyouken]') AND type in (N'U'))
DROP TABLE [dbo].[uc_061_yuukasyouken]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_071_koteishisan]') AND type in (N'U'))
DROP TABLE [dbo].[uc_071_koteishisan]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U'))
DROP TABLE [dbo].[uc_081_siharaitegata]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_091_kaikakekin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_091_kaikakekin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_092_kaikakekin2]') AND type in (N'U'))
DROP TABLE [dbo].[uc_092_kaikakekin2]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_101_kariukekin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_101_kariukekin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_102_gensen]') AND type in (N'U'))
DROP TABLE [dbo].[uc_102_gensen]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_111_kariirekin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_111_kariirekin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_121_totiuriage]') AND type in (N'U'))
DROP TABLE [dbo].[uc_121_totiuriage]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_131_uriagedaka]') AND type in (N'U'))
DROP TABLE [dbo].[uc_131_uriagedaka]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_141_yakuin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_141_yakuin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_142_jinkenhi]') AND type in (N'U'))
DROP TABLE [dbo].[uc_142_jinkenhi]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_151_tidaiyatin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_151_tidaiyatin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_152_kenrikin]') AND type in (N'U'))
DROP TABLE [dbo].[uc_152_kenrikin]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_153_kougyou]') AND type in (N'U'))
DROP TABLE [dbo].[uc_153_kougyou]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_161_zatueki]') AND type in (N'U'))
DROP TABLE [dbo].[uc_161_zatueki]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_162_zatuson]') AND type in (N'U'))
DROP TABLE [dbo].[uc_162_zatuson]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_171_sonota1]') AND type in (N'U'))
DROP TABLE [dbo].[uc_171_sonota1]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_181_sonota2]') AND type in (N'U'))
DROP TABLE [dbo].[uc_181_sonota2]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_counter]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_counter]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_form_type]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_form_type]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_main]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_main]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_sonota_item]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_sonota_item]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_sub_omit]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_sub_omit]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_ver]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_ver]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_address]') AND type in (N'U'))
DROP TABLE [dbo].[uc_lst_address]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_bank]') AND type in (N'U'))
DROP TABLE [dbo].[uc_lst_bank]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_item_sort]') AND type in (N'U'))
DROP TABLE [dbo].[uc_lst_item_sort]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_kamoku]') AND type in (N'U'))
DROP TABLE [dbo].[uc_lst_kamoku]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_sub]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_sub]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_yokuki]') AND type in (N'U'))
DROP TABLE [dbo].[uc_inf_yokuki]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_main]') AND type in (N'U'))
DROP TABLE [dbo].[uc_ren_main]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_kamoku]') AND type in (N'U'))
DROP TABLE [dbo].[uc_ren_kamoku]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_darec_temp]') AND type in (N'U'))
DROP TABLE [dbo].[uc_ren_darec_temp]
GO
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_hokan]') AND type in (N'U'))
DROP TABLE [dbo].[uc_lst_hokan]
GO
