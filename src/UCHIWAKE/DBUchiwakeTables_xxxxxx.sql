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

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_011_yotyokin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_011_yotyokin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[BkSeq] [int] NULL,
	[BkOrder] [int] NULL,
	[BkName1] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[BkName2] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AcNum] [varchar](14) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[BkKana1] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[BkKana2] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_011_yotyokin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_021_uketoritegata]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_021_uketoritegata](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[SpDate] [int] NULL,
	[PmDate] [int] NULL,
	[BkSeq] [int] NULL,
	[BkOrder] [int] NULL,
	[BkName1] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[BkName2] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Bk2Seq] [int] NULL,
	[Bk2Order] [int] NULL,
	[Bk2Name1] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Bk2Name2] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_021_uketoritegata] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_031_urikakekin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_031_urikakekin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_031_urikakekin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_041_karibarai]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_041_karibarai](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Kankei] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_041_karibarai] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_042_kasituke]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_042_kasituke](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Kankei] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Risoku] [decimal](14, 0) NULL,
	[Rate] [decimal](14, 0) NULL,
	[Riyuu] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_042_kasituke] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_051_tanaoroshi]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_051_tanaoroshi](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[HdName] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Suuryou] [decimal](14, 0) NULL,
	[Tanni] [varchar](10) COLLATE Japanese_CI_AS NULL,
	[Price] [decimal](14, 0) NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[FgInput] [tinyint] NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_051_tanaoroshi] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_052_tanaoroshi2]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_052_tanaoroshi2](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[Method] [tinyint] NULL,
	[HdName] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[SpDate] [int] NULL,
	[ShowKeiZero] [tinyint] NULL,
	[OutRangai] [tinyint] NULL,
 CONSTRAINT [PK_uc_052_tanaoroshi2] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_061_yuukasyouken]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_061_yuukasyouken](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Syurui] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Suuryou] [decimal](14, 0) NULL,
	[Tanni] [varchar](10) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Val2] [decimal](14, 0) NULL,
	[SpDate] [int] NULL,
	[Riyuu] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Suuryou2] [decimal](14, 0) NULL,
	[Tanni2] [varchar](10) COLLATE Japanese_CI_AS NULL,
	[Val3] [decimal](14, 0) NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_061_yuukasyouken] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_071_koteishisan]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_071_koteishisan](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[Syurui] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Youto] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Area] [decimal](14, 0) NULL,
	[Ad2Add1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Ad2Add2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[SpDate] [int] NULL,
	[Riyuu] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[MsVal] [decimal](14, 0) NULL,
	[MsVal2] [decimal](14, 0) NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[SpDate2] [int] NULL,
	[ShowKeiZero] [tinyint] NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_071_koteishisan] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_081_siharaitegata](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[SpDate] [int] NULL,
	[PmDate] [int] NULL,
	[BkSeq] [int] NULL,
	[BkOrder] [int] NULL,
	[BkName1] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[BkName2] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_081_siharaitegata] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_091_kaikakekin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_091_kaikakekin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_091_kaikakekin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_092_kaikakekin2]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_092_kaikakekin2](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[SpDate] [int] NULL,
	[Val] [decimal](14, 0) NULL,
	[ShowKeiZero] [tinyint] NULL,
	[OutRangai] [tinyint] NULL,
 CONSTRAINT [PK_uc_092_kaikakekin2] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_101_kariukekin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_101_kariukekin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Kankei] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_101_kariukekin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_102_gensen]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_102_gensen](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[SpDate] [int] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](20) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_102_gensen] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_111_kariirekin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_111_kariirekin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Kankei] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Risoku] [decimal](14, 0) NULL,
	[Rate] [decimal](14, 0) NULL,
	[Riyuu] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana1] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana2] [varchar](6) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_111_kariirekin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_121_totiuriage]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_121_totiuriage](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](20) COLLATE Japanese_CI_AS NULL,
	[Ad2Add1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Ad2Add2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[HdName] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Area] [decimal](14, 0) NULL,
	[SpDate] [int] NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Area2] [decimal](14, 0) NULL,
	[Val] [decimal](14, 0) NULL,
	[Val2] [decimal](14, 0) NULL,
	[SpDate2] [int] NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_121_totiuriage] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_131_uriagedaka]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_131_uriagedaka](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[HdName] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Kankei] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Val2] [decimal](14, 0) NULL,
	[Worker] [decimal](14, 0) NULL,
	[Area] [decimal](14, 0) NULL,
	[HdName2] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
 CONSTRAINT [PK_uc_131_uriagedaka] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_141_yakuin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_141_yakuin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[NameYaku] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[NameTant] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Kankei] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[FpTime] [tinyint] NULL,
	[EmVal] [decimal](14, 0) NULL,
	[PrVal] [decimal](14, 0) NULL,
	[BfVal] [decimal](14, 0) NULL,
	[PfVal] [decimal](14, 0) NULL,
	[EtVal] [decimal](14, 0) NULL,
	[RtVal] [decimal](14, 0) NULL,
	[Val] [decimal](14, 0) NULL,
	[FgInput] [tinyint] NULL,
	[ShowKeiZero] [tinyint] NULL,
 CONSTRAINT [PK_uc_141_yakuin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_142_jinkenhi]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_142_jinkenhi](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[Val] [decimal](14, 0) NULL,
	[Val2] [decimal](14, 0) NULL,
	[ShowKeiZero] [tinyint] NULL,
 CONSTRAINT [PK_uc_142_jinkenhi] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_151_tidaiyatin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_151_tidaiyatin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](20) COLLATE Japanese_CI_AS NULL,
	[Youto] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[Ad2Add1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Ad2Add2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[SpDate] [int] NULL,
	[SpDate2] [int] NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_151_tidaiyatin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_152_kenrikin]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_152_kenrikin](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[SpDate] [int] NULL,
	[Val] [decimal](14, 0) NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_152_kenrikin] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_153_kougyou]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_153_kougyou](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[HdName] [varchar](60) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[KyDate] [int] NULL,
	[KyDate2] [int] NULL,
	[SpDate] [int] NULL,
	[SpDate2] [int] NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_153_kougyou] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_161_zatueki]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_161_zatueki](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_161_zatueki] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_162_zatuson]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_162_zatuson](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Naiyou] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_162_zatuson] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_171_sonota1]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_171_sonota1](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[FormSeq] [smallint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[AdSeq] [int] NULL,
	[AdOrder] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_171_sonota1] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_181_sonota2]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_181_sonota2](
	[Seq] [int] NOT NULL,
	[NumPage] [smallint] NULL,
	[NumRow] [tinyint] NULL,
	[FgFunc] [tinyint] NULL,
	[FgShow] [tinyint] NULL,
	[KeiStr] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[NumGroup] [int] NULL,
	[RenKcd] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[RenEcd] [int] NULL,
	[RenFgTemp] [tinyint] NULL,
	[FormSeq] [smallint] NULL,
	[KnSeq] [int] NULL,
	[KnOrder] [int] NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[Item] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[Val] [decimal](14, 0) NULL,
	[Teki] [varchar](100) COLLATE Japanese_CI_AS NULL,
	[ShowKeiZero] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_181_sonota2] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_counter]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_counter](
	[TableName] [varchar](40) COLLATE Japanese_CI_AS NOT NULL,
	[Counter] [int] NULL,
 CONSTRAINT [PK_uc_inf_counter] PRIMARY KEY CLUSTERED 
(
	[TableName] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_form_type]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_form_type](
	[FormNo] [tinyint] NOT NULL,
	[FormType] [tinyint] NULL,
 CONSTRAINT [PK_uc_inf_form_type] PRIMARY KEY CLUSTERED 
(
	[FormNo] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_main]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_main](
	[Seq] [smallint] NOT NULL,
	[FormSeq] [smallint] NULL,
	[OutKikan] [tinyint] NULL,
	[OutKingaku] [tinyint] NULL,
	[OutPage] [tinyint] NULL,
	[FgUchiConv] [tinyint] NULL,
	[ssymd] [int] NULL,
	[eeymd] [int] NULL,
	[OutCoName] [tinyint] NULL,
	[GeneralConstVal] [binary](16) NULL,
	[GeneralVar] [binary](16) NULL,
 CONSTRAINT [PK_uc_inf_main] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_sonota_item]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_sonota_item](
	[FormSeq] [smallint] NOT NULL,
	[ItemName] [varchar](20) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_inf_sonota_item] PRIMARY KEY CLUSTERED 
(
	[FormSeq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_sub]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_sub](
	[FormSeq] [smallint] NOT NULL,
	[FormNo] [tinyint] NULL,
	[FormNo2] [tinyint] NULL,
	[FormNo3] [tinyint] NULL,
	[FormOrder] [smallint] NULL,
	[TitleNo] [varchar](10) COLLATE Japanese_CI_AS NULL,
	[TitleNo2] [varchar](10) COLLATE Japanese_CI_AS NULL,
	[TitleOrg] [varchar](80) COLLATE Japanese_CI_AS NULL,
	[TitleOrgTujo] [varchar](80) COLLATE Japanese_CI_AS NULL,
	[TitleSave] [varchar](80) COLLATE Japanese_CI_AS NULL,
	[TitleSaveTujo] [varchar](80) COLLATE Japanese_CI_AS NULL,
	[Title] [varchar](80) COLLATE Japanese_CI_AS NULL,
	[Sort1] [tinyint] NULL,
	[Sort2] [tinyint] NULL,
	[Sort3] [tinyint] NULL,
	[OpSykeiAuto] [tinyint] NULL,
	[OpSykeiName] [varchar](20) COLLATE Japanese_CI_AS NULL,
	[OpCykeiName] [varchar](20) COLLATE Japanese_CI_AS NULL,
	[OpSykeiKakko] [tinyint] NULL,
	[OpCykeiKakko] [tinyint] NULL,
	[OpIkkatuFg] [tinyint] NULL,
	[OpIkkatu] [tinyint] NULL,
	[OpIkkatuVal] [decimal](14, 0) NULL,
	[OpIkkatuValOrg] [decimal](14, 0) NULL,
	[OpOutSyokei] [tinyint] NULL,
	[OpOutNullLine] [tinyint] NULL,
	[OpKaiPage] [tinyint] NULL,
	[OutKei] [tinyint] NULL,
	[OpTitleKnFg] [tinyint] NULL,
	[OpTitleKn] [tinyint] NULL,
	[CurPage] [smallint] NULL,
	[CurRow] [tinyint] NULL,
	[CurField] [tinyint] NULL,
	[CurCtlIndex] [smallint] NULL,
	[GeneralConstVal] [binary](16) NULL,
	[GeneralVar] [binary](16) NULL,
 CONSTRAINT [PK_uc_inf_sub] PRIMARY KEY CLUSTERED 
(
	[FormSeq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_sub_omit]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_sub_omit](
	[FormSeq] [smallint] NOT NULL,
	[ItemSeq] [tinyint] NOT NULL,
	[ItemName] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[Omit] [tinyint] NULL,
	[OmitStr] [varchar](20) COLLATE Japanese_CI_AS NULL,
	[OmitMaxLen] [smallint] NULL,
 CONSTRAINT [PK_uc_inf_sub_omit] PRIMARY KEY CLUSTERED 
(
	[FormSeq] ASC,
	[ItemSeq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_ver]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_ver](
	[Seq] [smallint] NOT NULL,
	[Version] [smallint] NULL,
 CONSTRAINT [PK_uc_inf_ver] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_inf_yokuki]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_inf_yokuki](
	[FormSeq] [smallint] NOT NULL,
	[OpYokukiFg] [tinyint] NULL,
	[OpYokuki] [tinyint] NULL,
	[OpClearFg] [tinyint] NULL,
	[OpClear] [tinyint] NULL,
	[OpHokanFg] [tinyint] NULL,
	[OpHokan] [tinyint] NULL,
	[OpNullClearFg] [tinyint] NULL,
	[OpNullClear] [tinyint] NULL,
 CONSTRAINT [PK_uc_inf_yokuki] PRIMARY KEY CLUSTERED 
(
	[FormSeq] ASC
)WITH (PAD_INDEX  = OFF, IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_address]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_lst_address](
	[AdSeq] [int] NOT NULL,
	[GrSeq] [smallint] NULL,
	[OrderNum] [int] NULL,
	[AdName1] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdName2] [varchar](40) COLLATE Japanese_CI_AS NULL,
	[AdAdd1] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[AdAdd2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[FgDel] [tinyint] NULL,
	[AdKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[IkkatuVal] [decimal](14, 0) NULL,
	[Invno] [varchar](14) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_lst_address] PRIMARY KEY CLUSTERED 
(
	[AdSeq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_bank]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_lst_bank](
	[Seq] [int] NOT NULL,
	[OrderNum] [int] NULL,
	[BkName1] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[BkName2] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[FgDel] [tinyint] NULL,
	[BkKana1] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[BkKana2] [varchar](6) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_m00_bank] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_item_sort]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_lst_item_sort](
	[FormSeq] [smallint] NOT NULL,
	[ItemSeq] [tinyint] NOT NULL,
	[ItemName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[FgPage] [tinyint] NULL,
	[FgSykei] [tinyint] NULL,
	[FgItem] [tinyint] NULL,
	[OrderStr] [varchar](300) COLLATE Japanese_CI_AS NULL,
	[FieldSykei] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[FieldSykei2] [varchar](50) COLLATE Japanese_CI_AS NULL,
	[OrderStr2] [varchar](300) COLLATE Japanese_CI_AS NULL,
 CONSTRAINT [PK_uc_lst_item_sort] PRIMARY KEY CLUSTERED 
(
	[FormSeq] ASC,
	[ItemSeq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_kamoku]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_lst_kamoku](
	[KnSeq] [int] NOT NULL,
	[FormSeq] [smallint] NULL,
	[KnOrder] [int] NULL,
	[KnCode] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[KnName] [varchar](30) COLLATE Japanese_CI_AS NULL,
	[FgDel] [tinyint] NULL,
	[FgDft] [tinyint] NULL,
	[KnKana] [varchar](6) COLLATE Japanese_CI_AS NULL,
	[IkkatuVal] [decimal](14, 0) NULL,
	[PrSign] [tinyint] NULL,
 CONSTRAINT [PK_uc_lst_kamoku_1] PRIMARY KEY CLUSTERED 
(
	[KnSeq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_main]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_ren_main](
	[FormSeq] [smallint] NOT NULL,
	[OpRenType] [smallint] NULL,
	[OpRenTypeKz] [tinyint] NULL,
	[OpRenTypeEz] [tinyint] NULL,
	[OpRenTypeDa] [tinyint] NULL,
	[OpRenTypeKs] [tinyint] NULL,
	[OpTkcodFrom] [int] NULL,
	[OpTkcodTo] [int] NULL,
	[OpOutZeroEn] [tinyint] NULL,
	[OpGassan] [tinyint] NULL,
	[OpUpdType] [tinyint] NULL,
	[OpOldRenType] [smallint] NULL,
	[OpEdaren] [tinyint] NULL,
	[OpKmkname] [tinyint] NULL,
	[OpF9Upd] [tinyint] NULL,
 CONSTRAINT [PK_uc_ren_main] PRIMARY KEY CLUSTERED 
(
	[FormSeq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_kamoku]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_ren_kamoku](
	[Seq] [int] NOT NULL,
	[FormSeq] [smallint] NULL,
	[FgCoType] [tinyint] NULL,
	[KcdStart] [varchar](8) COLLATE Japanese_CI_AS NULL,
	[KnSeq] [int] NULL,
	[FgDft] [tinyint] NULL,
 CONSTRAINT [PK_uc_ren_kamoku] PRIMARY KEY CLUSTERED 
(
	[Seq] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_darec_temp]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[uc_ren_darec_temp](
	[seq] [int] NOT NULL,
	[ddate] [int] NULL,
	[dbt] [nchar](8) COLLATE Japanese_CI_AS NULL,
	[cre] [nchar](8) COLLATE Japanese_CI_AS NULL,
	[val] [decimal](12, 0) NULL,
	[tekiyo] [nchar](80) COLLATE Japanese_CI_AS NULL,
	[FgConv] [tinyint] NULL,
 CONSTRAINT [PK_uc_ren_darec_temp] PRIMARY KEY CLUSTERED 
(
	[seq] ASC
)WITH (PAD_INDEX  = OFF, IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
END
GO

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
GO


/*uc_inf_counter*/
/* MEMO: 2006/04/17時点 このテーブルへの初期値は不要 */

/*uc_inf_form_type*/
INSERT INTO uc_inf_form_type VALUES (4, 0);
INSERT INTO uc_inf_form_type VALUES (10, 0);
INSERT INTO uc_inf_form_type VALUES (14, 0);
INSERT INTO uc_inf_form_type VALUES (15, 0);
INSERT INTO uc_inf_form_type VALUES (16, 0);

/*uc_inf_main*/
INSERT INTO uc_inf_main VALUES (1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0x37010000000000000000000000000000);

-- tblver8アップデート時にGeneralVarの初期値セットをUPDATE文からINSERT文でのセットに変更　←ExecMode > 0時のRETURNでRETURN文以降にGO文があるとRETURNされないから（UPDATE文を走らせる為にはGO文が必要）
-- 桁が多すぎるのでset_byte2で値のセット
-- フリガナ振り済み(0x01)、保管データメンテナンス済み(0x02)、「プレビュー時、印刷時に一括印刷画面を表示する」(0x04)、owntblをコピー済み(0x10)、1.0.1.12アップデート済み(0x20)
--UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, 0x37, 1)
--ユーザー設定→「取引先、金融機関を自動で登録する」(0x01)をデフォルトON
--UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar, 1, 0x01, 1)


/*uc_inf_sonota_item*/
INSERT INTO uc_inf_sonota_item VALUES (201, '項目');
INSERT INTO uc_inf_sonota_item VALUES (202, '項目');
INSERT INTO uc_inf_sonota_item VALUES (203, '項目');
INSERT INTO uc_inf_sonota_item VALUES (204, '項目');
INSERT INTO uc_inf_sonota_item VALUES (205, '項目');
INSERT INTO uc_inf_sonota_item VALUES (206, '項目');
INSERT INTO uc_inf_sonota_item VALUES (207, '項目');
INSERT INTO uc_inf_sonota_item VALUES (208, '項目');
INSERT INTO uc_inf_sonota_item VALUES (209, '項目');
INSERT INTO uc_inf_sonota_item VALUES (210, '項目');
INSERT INTO uc_inf_sonota_item VALUES (211, '項目');
INSERT INTO uc_inf_sonota_item VALUES (212, '項目');
INSERT INTO uc_inf_sonota_item VALUES (213, '項目');
INSERT INTO uc_inf_sonota_item VALUES (214, '項目');
INSERT INTO uc_inf_sonota_item VALUES (215, '項目');
INSERT INTO uc_inf_sonota_item VALUES (216, '項目');
INSERT INTO uc_inf_sonota_item VALUES (217, '項目');
INSERT INTO uc_inf_sonota_item VALUES (218, '項目');
INSERT INTO uc_inf_sonota_item VALUES (219, '項目');
INSERT INTO uc_inf_sonota_item VALUES (220, '項目');

/*uc_inf_sub*/ -- OpIkkatuValの値はuc_lst_kamoku.IkkatuValでも同じ値を持つので注意すること
INSERT INTO uc_inf_sub VALUES (1, 1, 1, 0, 1, '①', NULL, '預貯金等の内訳書', NULL, '預貯金等の内訳書', NULL, '預貯金等の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 1, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (2, 2, 1, 0, 2, '②', NULL, '受取手形の内訳書', NULL, '受取手形の内訳書', NULL, '受取手形の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 1000000, 1000000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 3, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (3, 3, 1, 0, 3, '③', NULL, '売掛金（未収入金）の内訳書', NULL, '売掛金（未収入金）の内訳書', NULL, '売掛金（未収入金）の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 500000, 500000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (4, 4, 1, 0, 4, '④', '－１', '仮払金（前渡金）の内訳書', '仮払金（前渡金）の内訳書', '仮払金（前渡金）の内訳書', '仮払金（前渡金）の内訳書', '仮払金（前渡金）の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 500000, 500000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (5, 4, 2, 0, 5, '④', '－２', '貸付金及び受取利息の内訳書', '貸付金及び受取利息の内訳書', '貸付金及び受取利息の内訳書', '貸付金及び受取利息の内訳書', '貸付金及び受取利息の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 500000, 500000, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (6, 5, 1, 0, 6, '⑤', NULL, '棚卸資産（商品又は製品、半製品、仕掛品、原材料、貯蔵品）の内訳書', NULL, '棚卸資産（商品又は製品、半製品、仕掛品、原材料、貯蔵品）の内訳書', NULL, '棚卸資産（商品又は製品、半製品、仕掛品、原材料、貯蔵品）の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (7, 6, 1, 0, 7, '⑥', NULL, '有価証券の内訳書', NULL, '有価証券の内訳書', NULL, '有価証券の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (8, 7, 1, 0, 8, '⑦', NULL, '固定資産（土地、土地の上に存する権利及び建物に限る。）の内訳書', NULL, '固定資産（土地、土地の上に存する権利及び建物に限る。）の内訳書', NULL, '固定資産（土地、土地の上に存する権利及び建物に限る。）の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (9, 8, 1, 0, 9, '⑧', NULL, '支払手形の内訳書', NULL, '支払手形の内訳書', NULL, '支払手形の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 1000000, 1000000, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 1, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (10, 9, 1, 0, 10, '⑨', NULL, '買掛金（未払金・未払費用）の内訳書', NULL, '買掛金（未払金・未払費用）の内訳書', NULL, '買掛金（未払金・未払費用）の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 500000, 500000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (11, 10, 1, 0, 11, '⑩', '－１', '仮受金（前受金・預り金）の内訳書', '仮受金（前受金・預り金）の内訳書', '仮受金（前受金・預り金）の内訳書', '仮受金（前受金・預り金）の内訳書', '仮受金（前受金・預り金）の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 500000, 500000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (12, 10, 2, 0, 12, '⑩', '－２', '源泉所得税預り金の内訳', '源泉所得税預り金の内訳', '源泉所得税預り金の内訳', '源泉所得税預り金の内訳', '源泉所得税預り金の内訳', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (13, 11, 1, 0, 13, '⑪', NULL, '借入金及び支払利子の内訳書', NULL, '借入金及び支払利子の内訳書', NULL, '借入金及び支払利子の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 500000, 500000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 2, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (14, 12, 1, 0, 14, '⑫', NULL, '土地の売上高等の内訳書', NULL, '土地の売上高等の内訳書', NULL, '土地の売上高等の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (15, 13, 1, 0, 15, '⑬', NULL, '売上高等の事業所別内訳書', NULL, '売上高等の事業所別内訳書', NULL, '売上高等の事業所別内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (16, 14, 1, 0, 16, '⑭', '－１', '役員報酬手当等の内訳書', '役員報酬手当等及び人件費の内訳書', '役員報酬手当等の内訳書', '役員報酬手当等及び人件費の内訳書', '役員報酬手当等及び人件費の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (17, 14, 2, 0, 17, '⑭', '－２', '人件費の内訳書', '人件費の内訳書', '人件費の内訳書', '人件費の内訳書', '人件費の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (18, 15, 1, 0, 18, '⑮', '－１', '地代家賃の内訳書', '地代家賃等の内訳書', '地代家賃の内訳書', '地代家賃等の内訳書', '地代家賃等の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (19, 15, 2, 0, 19, '⑮', '－２', '権利金等の期中支払の内訳書', '権利金等の期中支払の内訳書', '権利金等の期中支払の内訳書', '権利金等の期中支払の内訳書', '権利金等の期中支払の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (20, 15, 3, 0, 20, '⑮', '－３', '工業所有権等の使用料の内訳書', '工業所有権等の使用料の内訳書', '工業所有権等の使用料の内訳書', '工業所有権等の使用料の内訳書', '工業所有権等の使用料の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (21, 16, 1, 0, 21, '⑯', '－１', '雑益等の内訳書', '雑益、雑損失等の内訳書', '雑益等の内訳書', '雑益、雑損失等の内訳書', '雑益、雑損失等の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 100000, 100000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (22, 16, 2, 0, 22, '⑯', '－２', '雑損失等の内訳書', '雑損失等の内訳書', '雑損失等の内訳書', '雑損失等の内訳書', '雑損失等の内訳書', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 100000, 100000, 1, 1, 0, 2, 1, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);

INSERT INTO uc_inf_sub VALUES (101, 17, 1, 0, 23, '⑰', '－１', 'その他科目の内訳書①－１', NULL, 'その他科目の内訳書①－１', NULL, 'その他科目の内訳書①－１', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (102, 17, 2, 0, 24, '⑰', '－２', 'その他科目の内訳書①－２', NULL, 'その他科目の内訳書①－２', NULL, 'その他科目の内訳書①－２', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (103, 17, 3, 0, 25, '⑰', '－３', 'その他科目の内訳書①－３', NULL, 'その他科目の内訳書①－３', NULL, 'その他科目の内訳書①－３', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (104, 17, 4, 0, 26, '⑰', '－４', 'その他科目の内訳書①－４', NULL, 'その他科目の内訳書①－４', NULL, 'その他科目の内訳書①－４', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (105, 17, 5, 0, 27, '⑰', '－５', 'その他科目の内訳書①－５', NULL, 'その他科目の内訳書①－５', NULL, 'その他科目の内訳書①－５', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (106, 17, 6, 0, 28, '⑰', '－６',   'その他科目の内訳書①－６',    NULL, 'その他科目の内訳書①－６',   NULL, 'その他科目の内訳書①－６',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (107, 17, 7, 0, 29, '⑰', '－７',   'その他科目の内訳書①－７',    NULL, 'その他科目の内訳書①－７',   NULL, 'その他科目の内訳書①－７',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (108, 17, 8, 0, 30, '⑰', '－８',   'その他科目の内訳書①－８',    NULL, 'その他科目の内訳書①－８',   NULL, 'その他科目の内訳書①－８',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (109, 17, 9, 0, 31, '⑰', '－９',   'その他科目の内訳書①－９',    NULL, 'その他科目の内訳書①－９',   NULL, 'その他科目の内訳書①－９',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (110, 17,10, 0, 32, '⑰', '－１０', 'その他科目の内訳書①－１０',  NULL, 'その他科目の内訳書①－１０', NULL, 'その他科目の内訳書①－１０', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (111, 17,11, 0, 33, '⑰', '－１１', 'その他科目の内訳書①－１１',  NULL, 'その他科目の内訳書①－１１', NULL, 'その他科目の内訳書①－１１', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (112, 17,12, 0, 34, '⑰', '－１２', 'その他科目の内訳書①－１２',  NULL, 'その他科目の内訳書①－１２', NULL, 'その他科目の内訳書①－１２', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (113, 17,13, 0, 35, '⑰', '－１３', 'その他科目の内訳書①－１３',  NULL, 'その他科目の内訳書①－１３', NULL, 'その他科目の内訳書①－１３', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (114, 17,14, 0, 36, '⑰', '－１４', 'その他科目の内訳書①－１４',  NULL, 'その他科目の内訳書①－１４', NULL, 'その他科目の内訳書①－１４', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (115, 17,15, 0, 37, '⑰', '－１５', 'その他科目の内訳書①－１５',  NULL, 'その他科目の内訳書①－１５', NULL, 'その他科目の内訳書①－１５', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (116, 17,16, 0, 38, '⑰', '－１６', 'その他科目の内訳書①－１６',  NULL, 'その他科目の内訳書①－１６', NULL, 'その他科目の内訳書①－１６', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (117, 17,17, 0, 39, '⑰', '－１７', 'その他科目の内訳書①－１７',  NULL, 'その他科目の内訳書①－１７', NULL, 'その他科目の内訳書①－１７', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (118, 17,18, 0, 40, '⑰', '－１８', 'その他科目の内訳書①－１８',  NULL, 'その他科目の内訳書①－１８', NULL, 'その他科目の内訳書①－１８', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (119, 17,19, 0, 41, '⑰', '－１９', 'その他科目の内訳書①－１９',  NULL, 'その他科目の内訳書①－１９', NULL, 'その他科目の内訳書①－１９', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (120, 17,20, 0, 42, '⑰', '－２０', 'その他科目の内訳書①－２０',  NULL, 'その他科目の内訳書①－２０', NULL, 'その他科目の内訳書①－２０', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);

INSERT INTO uc_inf_sub VALUES (201, 18, 1, 0, 43, '⑱', '－１', 'その他科目の内訳書②－１', NULL, 'その他科目の内訳書②－１', NULL, 'その他科目の内訳書②－１', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (202, 18, 2, 0, 44, '⑱', '－２', 'その他科目の内訳書②－２', NULL, 'その他科目の内訳書②－２', NULL, 'その他科目の内訳書②－２', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (203, 18, 3, 0, 45, '⑱', '－３', 'その他科目の内訳書②－３', NULL, 'その他科目の内訳書②－３', NULL, 'その他科目の内訳書②－３', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (204, 18, 4, 0, 46, '⑱', '－４', 'その他科目の内訳書②－４', NULL, 'その他科目の内訳書②－４', NULL, 'その他科目の内訳書②－４', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (205, 18, 5, 0, 47, '⑱', '－５', 'その他科目の内訳書②－５', NULL, 'その他科目の内訳書②－５', NULL, 'その他科目の内訳書②－５', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (206, 18, 6, 0, 48, '⑱', '－６',   'その他科目の内訳書②－６',    NULL, 'その他科目の内訳書②－６',   NULL, 'その他科目の内訳書②－６',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (207, 18, 7, 0, 49, '⑱', '－７',   'その他科目の内訳書②－７',    NULL, 'その他科目の内訳書②－７',   NULL, 'その他科目の内訳書②－７',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (208, 18, 8, 0, 50, '⑱', '－８',   'その他科目の内訳書②－８',    NULL, 'その他科目の内訳書②－８',   NULL, 'その他科目の内訳書②－８',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (209, 18, 9, 0, 51, '⑱', '－９',   'その他科目の内訳書②－９',    NULL, 'その他科目の内訳書②－９',   NULL, 'その他科目の内訳書②－９',   0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (210, 18,10, 0, 52, '⑱', '－１０', 'その他科目の内訳書②－１０',  NULL, 'その他科目の内訳書②－１０', NULL, 'その他科目の内訳書②－１０', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (211, 18,11, 0, 53, '⑱', '－１１', 'その他科目の内訳書②－１１',  NULL, 'その他科目の内訳書②－１１', NULL, 'その他科目の内訳書②－１１', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (212, 18,12, 0, 54, '⑱', '－１２', 'その他科目の内訳書②－１２',  NULL, 'その他科目の内訳書②－１２', NULL, 'その他科目の内訳書②－１２', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (213, 18,13, 0, 55, '⑱', '－１３', 'その他科目の内訳書②－１３',  NULL, 'その他科目の内訳書②－１３', NULL, 'その他科目の内訳書②－１３', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (214, 18,14, 0, 56, '⑱', '－１４', 'その他科目の内訳書②－１４',  NULL, 'その他科目の内訳書②－１４', NULL, 'その他科目の内訳書②－１４', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (215, 18,15, 0, 57, '⑱', '－１５', 'その他科目の内訳書②－１５',  NULL, 'その他科目の内訳書②－１５', NULL, 'その他科目の内訳書②－１５', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (216, 18,16, 0, 58, '⑱', '－１６', 'その他科目の内訳書②－１６',  NULL, 'その他科目の内訳書②－１６', NULL, 'その他科目の内訳書②－１６', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (217, 18,17, 0, 59, '⑱', '－１７', 'その他科目の内訳書②－１７',  NULL, 'その他科目の内訳書②－１７', NULL, 'その他科目の内訳書②－１７', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (218, 18,18, 0, 60, '⑱', '－１８', 'その他科目の内訳書②－１８',  NULL, 'その他科目の内訳書②－１８', NULL, 'その他科目の内訳書②－１８', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (219, 18,19, 0, 61, '⑱', '－１９', 'その他科目の内訳書②－１９',  NULL, 'その他科目の内訳書②－１９', NULL, 'その他科目の内訳書②－１９', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);
INSERT INTO uc_inf_sub VALUES (220, 18,20, 0, 62, '⑱', '－２０', 'その他科目の内訳書②－２０',  NULL, 'その他科目の内訳書②－２０', NULL, 'その他科目の内訳書②－２０', 0, 0, 0, 0, '小計', '中計', 1, 2, 1, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0x08000000000000000000000000000000);

-- 未使用だが、念のためbAddHokanを立てておく
--UPDATE uc_inf_sub SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, 0x08, 1)

/*uc_inf_sub_omit*/
INSERT INTO uc_inf_sub_omit VALUES (1, 1, '金融機関名', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (1, 2, '種類', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (2, 1, '振出人', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (2, 2, '支払銀行名', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (2, 3, '割引銀行名', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (3, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (3, 2, '相手先（名称）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (3, 3, '相手先（所在地）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (4, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (4, 2, '相手先（名称）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (4, 3, '相手先（所在地）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (4, 4, '相手先（関係）', 0, '〃', 6);
INSERT INTO uc_inf_sub_omit VALUES (5, 1, '貸付先', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (5, 2, '貸付先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (5, 3, '貸付先（関係）', 0, '〃', 6);
INSERT INTO uc_inf_sub_omit VALUES (6, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (6, 2, '品目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (7, 1, '区分', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (7, 2, '種類・銘柄', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (7, 3, '異動事由', 0, '〃', 6);
INSERT INTO uc_inf_sub_omit VALUES (7, 4, '売却先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (7, 5, '売却先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (8, 1, '種類・構造', 0, '〃', 7);
INSERT INTO uc_inf_sub_omit VALUES (8, 2, '物件の所在地', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (8, 3, '異動事由', 0, '〃', 6);
INSERT INTO uc_inf_sub_omit VALUES (8, 4, '売却先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (8, 5, '売却先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (9, 1, '支払先', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (9, 2, '支払銀行名', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (10, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (10, 2, '相手先（名称）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (10, 3, '相手先（所在地）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (11, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (11, 2, '相手先（名称）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (11, 3, '相手先（所在地）　', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (11, 4, '相手先（関係）', 0, '〃', 6);
INSERT INTO uc_inf_sub_omit VALUES (12, 1, '所得の種類', 0, '〃', 2);
INSERT INTO uc_inf_sub_omit VALUES (13, 1, '借入先', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (13, 2, '借入先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (13, 3, '借入先（関係）', 0, '〃', 6);
INSERT INTO uc_inf_sub_omit VALUES (14, 1, '区分', 0, '〃', 5);
INSERT INTO uc_inf_sub_omit VALUES (14, 2, '商品の所在地', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (14, 3, '地目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (14, 4, '売上先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (14, 5, '売上先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (15, 1, '事業所の名称', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (15, 2, '事業所（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (15, 3, '責任者氏名', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (15, 4, '代表者との関係', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (16, 1, '役職名', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (16, 2, '担当業務', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (16, 3, '代表者との関係', 0, '〃', 6);
INSERT INTO uc_inf_sub_omit VALUES (16, 4, '住所', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (18, 1, '地代・家賃の区分', 0, '〃', 7);
INSERT INTO uc_inf_sub_omit VALUES (18, 2, '借地物件の用途', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (18, 3, '所在地', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (18, 4, '貸主の名称', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (18, 5, '貸主の所在地', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (19, 1, '支払先の名称', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (19, 2, '支払先の所在地', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (19, 3, '権利金等の内容', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (20, 1, '名称', 0, '〃', 5);
INSERT INTO uc_inf_sub_omit VALUES (20, 2, '支払先の名称', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (20, 3, '支払先の所在地', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (21, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (21, 2, '取引の内容', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (21, 3, '相手先', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (21, 4, '所在地', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (22, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (22, 2, '取引の内容', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (22, 3, '相手先', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (22, 4, '所在地', 0, '〃', 8);

INSERT INTO uc_inf_sub_omit VALUES (101, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (101, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (101, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (102, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (102, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (102, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (103, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (103, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (103, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (104, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (104, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (104, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (105, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (105, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (105, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (106, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (106, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (106, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (107, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (107, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (107, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (108, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (108, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (108, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (109, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (109, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (109, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (110, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (110, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (110, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (111, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (111, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (111, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (112, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (112, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (112, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (113, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (113, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (113, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (114, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (114, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (114, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (115, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (115, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (115, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (116, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (116, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (116, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (117, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (117, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (117, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (118, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (118, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (118, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (119, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (119, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (119, 3, '相手先（所在地）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (120, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (120, 2, '相手先（名称）', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (120, 3, '相手先（所在地）', 0, '〃', 8);

INSERT INTO uc_inf_sub_omit VALUES (201, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (202, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (203, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (204, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (205, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (206, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (207, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (208, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (209, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (210, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (211, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (212, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (213, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (214, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (215, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (216, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (217, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (218, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (219, 1, '科目', 0, '〃', 8);
INSERT INTO uc_inf_sub_omit VALUES (220, 1, '科目', 0, '〃', 8);

/*uc_inf_ver*/
INSERT INTO uc_inf_ver VALUES (1, 10);
INSERT INTO uc_inf_ver VALUES (2, 2);

/*uc_inf_yokuki*/
INSERT INTO uc_inf_yokuki VALUES (1, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (2, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (3, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (4, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (5, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (6, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (7, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (8, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (9, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (10, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (11, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (12, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (13, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (14, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (15, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (16, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (17, 1, 1, 1, 1, 0, 0, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (18, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (19, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (20, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (21, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (22, 1, 1, 1, 1, 1, 1, 1, 0);

INSERT INTO uc_inf_yokuki VALUES (101, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (102, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (103, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (104, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (105, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (106, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (107, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (108, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (109, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (110, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (111, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (112, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (113, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (114, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (115, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (116, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (117, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (118, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (119, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (120, 1, 1, 1, 1, 1, 1, 1, 0);
			
INSERT INTO uc_inf_yokuki VALUES (201, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (202, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (203, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (204, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (205, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (206, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (207, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (208, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (209, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (210, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (211, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (212, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (213, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (214, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (215, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (216, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (217, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (218, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (219, 1, 1, 1, 1, 1, 1, 1, 0);
INSERT INTO uc_inf_yokuki VALUES (220, 1, 1, 1, 1, 1, 1, 1, 0);

/*uc_lst_kamoku*/
INSERT INTO uc_lst_kamoku VALUES (101, 1, 1, NULL, '当座預金', 0, 1, 'ﾄｳｻﾞﾖｷ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (102, 1, 2, NULL, '普通預金', 0, 1, 'ﾌﾂｳﾖｷﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (103, 1, 3, NULL, '定期預金', 0, 1, 'ﾃｲｷﾖｷﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (104, 1, 4, NULL, '通知預金', 0, 1, 'ﾂｳﾁﾖｷﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (105, 1, 5, NULL, '積立預金', 0, 1, 'ﾂﾐﾀﾃﾖｷ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (106, 1, 6, NULL, '別段預金', 0, 1, 'ﾍﾞﾂﾀﾞﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (107, 1, 7, NULL, '現金', 0, 1, 'ｹﾞﾝｷﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (201, 2, 1, NULL, '受取手形', 0, 1, 'ｳｹﾄﾘﾃｶ', 1000000,0);
INSERT INTO uc_lst_kamoku VALUES (202, 2, 2, NULL, '割引手形', 0, 1, 'ﾜﾘﾋﾞｷﾃ', 1000000,0);
INSERT INTO uc_lst_kamoku VALUES (301, 3, 1, NULL, '売掛金', 0, 1, 'ｳﾘｶｹｷﾝ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (302, 3, 2, NULL, '未収入金', 0, 1, 'ﾐｼｭｳﾆｭ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (303, 3, 3, NULL, '完成工事未収入金', 0, 1, 'ｶﾝｾｲｺｳ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (401, 4, 1, NULL, '仮払金', 0, 1, 'ｶﾘﾊﾞﾗｲ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (402, 4, 2, NULL, '前渡金', 0, 1, 'ﾏｴﾜﾀｼｷ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (403, 4, 3, NULL, '前払費用', 0, 1, 'ﾏｴﾊﾞﾗｲ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (501, 5, 1, NULL, '貸付金', 0, 1, 'ｶｼﾂｹｷﾝ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (601, 6, 1, NULL, '商品', 0, 1, 'ｼｮｳﾋﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (602, 6, 2, NULL, '製品', 0, 1, 'ｾｲﾋﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (603, 6, 3, NULL, '半製品', 0, 1, 'ﾊﾝｾｲﾋﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (604, 6, 4, NULL, '仕掛品', 0, 1, 'ｼｶｶﾘﾋﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (605, 6, 5, NULL, '原材料', 0, 1, 'ｹﾞﾝｻﾞｲ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (606, 6, 6, NULL, '貯蔵品', 0, 1, 'ﾁｮｿﾞｳﾋ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (607, 6, 7, NULL, '作業くず', 0, 1, 'ｻｷﾞｮｳｸ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (608, 6, 8, NULL, '副産物', 0, 1, 'ﾌｸｻﾝﾌﾞ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (609, 6, 9, NULL, '未成工事支出金', 0, 1, 'ﾐｾｲｺｳ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (701, 7, 1, NULL, '売買', 0, 1, 'ﾊﾞｲﾊﾞｲ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (702, 7, 2, NULL, '満期', 0, 1, 'ﾏﾝｷ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (703, 7, 3, NULL, 'その他', 0, 1, 'ｿﾉﾀ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (901, 9, 1, NULL, '支払手形', 0, 1, 'ｼﾊﾗｲﾃｶ', 1000000,0);
INSERT INTO uc_lst_kamoku VALUES (1001, 10, 1, NULL, '買掛金', 0, 1, 'ｶｲｶｹｷﾝ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1002, 10, 2, NULL, '未払金', 0, 1, 'ﾐﾊﾞﾗｲｷ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1003, 10, 3, NULL, '未払費用', 0, 1, 'ﾐﾊﾞﾗｲﾋ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1004, 10, 4, NULL, '工事未払金', 0, 1, 'ｺｳｼﾞﾐﾊ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1101, 11, 1, NULL, '仮受金', 0, 1, 'ｶﾘｳｹｷﾝ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1102, 11, 2, NULL, '前受金', 0, 1, 'ﾏｴｳｹｷﾝ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1103, 11, 3, NULL, '預り金', 0, 1, 'ｱｽﾞｶﾘｷ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1201, 12, 1, NULL, '給', 0, 1, 'ｷｭｳ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1202, 12, 2, NULL, '退', 0, 1, 'ﾀｲ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1203, 12, 3, NULL, '報', 0, 1, 'ﾎｳ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1204, 12, 4, NULL, '利', 0, 1, 'ﾘ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1205, 12, 5, NULL, '配', 0, 1, 'ﾊｲ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1206, 12, 6, NULL, '非', 0, 1, 'ﾋ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1301, 13, 1, NULL, '短期借入金', 0, 1, 'ﾀﾝｷｶﾘｲ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1302, 13, 2, NULL, '長期借入金', 0, 1, 'ﾁｮｳｷｶﾘ', 500000,0);
INSERT INTO uc_lst_kamoku VALUES (1401, 14, 1, NULL, '売上', 0, 1, 'ｳﾘｱｹﾞ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1402, 14, 2, NULL, '仲介手数料', 0, 1, 'ﾁｭｳｶｲﾃ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1701, 17, 1, NULL, '役員報酬手当', 0, 1, 'ﾔｸｲﾝﾎｳ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1702, 17, 2, NULL, '従業員給料手当', 0, 1, 'ｼﾞｭｳｷﾞ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1703, 17, 3, NULL, '従業員賃金手当', 0, 1, 'ｼﾞｭｳｷﾞ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1801, 18, 1, NULL, '地代', 0, 1, 'ﾁﾀﾞｲ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (1802, 18, 2, NULL, '家賃', 0, 1, 'ﾔﾁﾝ', 0,0);
INSERT INTO uc_lst_kamoku VALUES (2101, 21, 1, NULL, '雑収入', 0, 1, 'ｻﾞｯｼｭｳ', 100000,0);
INSERT INTO uc_lst_kamoku VALUES (2102, 21, 2, NULL, '受取利息', 0, 1, 'ｳｹﾄﾘﾘｿ', 100000,0);
INSERT INTO uc_lst_kamoku VALUES (2103, 21, 3, NULL, '受取配当金', 0, 1, 'ｳｹﾄﾘﾊｲ', 100000,0);
INSERT INTO uc_lst_kamoku VALUES (2104, 21, 4, NULL, '固定資産売却益', 0, 1, 'ｺﾃｲｼｻﾝ', 100000,0);
INSERT INTO uc_lst_kamoku VALUES (2201, 22, 1, NULL, '雑損失', 0, 1, 'ｻﾞｯｿﾝｼ', 100000,0);
INSERT INTO uc_lst_kamoku VALUES (2202, 22, 2, NULL, '支払利息', 0, 1, 'ｼﾊﾗｲﾘｿ', 100000,0);
INSERT INTO uc_lst_kamoku VALUES (2203, 22, 3, NULL, '貸倒損失', 0, 1, 'ｶｼﾀﾞｵﾚ', 100000,0);
INSERT INTO uc_lst_kamoku VALUES (2204, 22, 4, NULL, '固定資産売却損', 0, 1, 'ｺﾃｲｼｻﾝ', 100000,0);

/*uc_lst_item_sort*/
INSERT INTO uc_lst_item_sort VALUES (1, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (1, 1, '金融機関名', 1, 1, 0, 'CASE BkOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) END, BkOrder ASC, (BkName1 + BkName2) COLLATE Japanese_CS_AS_KS_WS ASC, BkName1 COLLATE Japanese_CS_AS_KS_WS ASC, BkName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'BkName1', 'BkName2', 'CASE BkOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) END, (isnull(BkKana1,'') + isnull(BkKana2,'')) COLLATE Japanese_CS_AS_KS_WS ASC, BkKana1 COLLATE Japanese_CS_AS_KS_WS ASC, BkKana2 COLLATE Japanese_CS_AS_KS_WS ASC, BkOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (1, 2, '種類', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (1, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (1, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 1, '振出人', 1, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (2, 2, '振出年月日（新しい順）', 0, 0, 0, 'SpDate DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 3, '振出年月日（古い順）', 0, 0, 0, 'SpDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 4, '支払期日（新しい順）', 0, 0, 0, 'PmDate DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 5, '支払期日（古い順）', 0, 0, 0, 'PmDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 6, '金額（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 7, '金額（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (2, 8, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (2, 9, '登録番号', 1, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (3, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (3, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (3, 2, '相手先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (3, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (3, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (3, 5, '登録番号', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (4, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (4, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (4, 2, '相手先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (4, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (4, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (4, 5, '登録番号', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (5, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (5, 1, '貸付先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (5, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (5, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (5, 4, '登録番号', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (6, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (6, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (6, 2, '品目', 0, 0, 0, 'HdName COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (6, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (6, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (7, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (7, 1, '区分', 1, 1, 0, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (7, 2, '種類・銘柄', 0, 1, 0, 'Syurui COLLATE Japanese_CS_AS_KS_WS ASC', 'Syurui', NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (7, 3, '期末現在高（上段）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (7, 4, '期末現在高（下段）（大きい順）', 0, 0, 0, 'Val2 DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (7, 5, '期末現在高（下段）（小さい順）', 0, 0, 0, 'Val2 ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (7, 6, '異動年月日', 0, 0, 0, 'SpDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 1, '種類・構造', 1, 1, 0, 'Syurui COLLATE Japanese_CS_AS_KS_WS ASC', 'Syurui', NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 2, '物件の所在地', 0, 0, 0, '(Ad2Add1 + Ad2Add2) COLLATE Japanese_CS_AS_KS_WS ASC, Ad2Add1 COLLATE Japanese_CS_AS_KS_WS ASC, Ad2Add2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 5, '異動年月日', 0, 0, 0, 'SpDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 6, '取得年月', 0, 0, 0, 'SpDate2 ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (8, 7, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 1, '支払先', 1, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (9, 2, '振出年月日（新しい順）', 0, 0, 0, 'SpDate DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 3, '振出年月日（古い順）', 0, 0, 0, 'SpDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 4, '支払期日（新しい順）', 0, 0, 0, 'PmDate DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 5, '支払期日（古い順）', 0, 0, 0, 'PmDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 6, '金額（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 7, '金額（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (9, 8, '登録番号', 1, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (10, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (10, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (10, 2, '相手先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (10, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (10, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (10, 5, '登録番号', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (11, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (11, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (11, 2, '相手先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (11, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (11, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (11, 5, '登録番号', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (12, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (12, 1, '年月日（新しい順）', 0, 0, 0, 'SpDate DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (12, 2, '年月日（古い順）', 0, 0, 0, 'SpDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (12, 3, '所得の種類', 0, 1, 0, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (13, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (13, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (13, 2, '借入先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_bank) END, (isnull(AdKana1,'') + isnull(AdKana2,'')) COLLATE Japanese_CS_AS_KS_WS ASC, AdKana1 COLLATE Japanese_CS_AS_KS_WS ASC, AdKana2 COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (13, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (13, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 1, '区分', 1, 1, 0, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (14, 2, '売上年月（新しい順）', 0, 0, 0, 'SpDate DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 3, '売上年月（古い順）', 0, 0, 0, 'SpDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 4, '売上金額（上段）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 5, '売上金額（下段）（大きい順）', 0, 0, 0, 'Val2 DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 6, '売上金額（下段）（小さい順）', 0, 0, 0, 'Val2 ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 7, '取得年', 0, 0, 0, 'SpDate2 ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (14, 8, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (15, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (15, 1, '事業所の名称', 0, 0, 0, 'AdName1 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (15, 2, '売上高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (15, 3, '売上高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (15, 4, '期末棚卸高', 0, 0, 0, 'Val2 DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (18, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (18, 1, '地代・家賃の区分', 1, 1, 0, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (18, 2, '貸主の名称', 0, 0, 0, 'AdName1 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (18, 3, '支払貸借料（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (18, 4, '支払貸借料（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (18, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (19, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (19, 1, '支払先の名称', 0, 0, 0, 'AdName1 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (19, 2, '支払年月日（新しい順）', 0, 0, 0, 'SpDate DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (19, 3, '支払年月日（古い順）', 0, 0, 0, 'SpDate ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (19, 4, '支払金額（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (19, 5, '支払金額（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (19, 6, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (20, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (20, 1, '名称', 0, 1, 0, 'HdName COLLATE Japanese_CS_AS_KS_WS ASC', 'HdName', NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (20, 2, '支払先の名称', 0, 0, 0, 'AdName1 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (20, 3, '支払金額（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (20, 4, '支払金額（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (20, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (21, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (21, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (21, 2, '取引の内容', 0, 0, 0, 'Naiyou COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (21, 3, '相手先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (21, 4, '金額（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (21, 5, '金額（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (21, 6, '登録番号', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (22, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (22, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (22, 2, '取引の内容', 0, 0, 0, 'Naiyou COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (22, 3, '相手先', 0, 1, 0, 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdOrder ASC, (AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', 'AdName1', 'AdName2', 'CASE AdOrder WHEN 0 THEN (select max(OrderNum) + 1 from uc_lst_address) END, AdKana COLLATE Japanese_CS_AS_KS_WS ASC, AdOrder ASC' );
INSERT INTO uc_lst_item_sort VALUES (22, 4, '金額（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (22, 5, '金額（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (22, 6, '登録番号', 0, 1, 0, 'Invno ASC', NULL, NULL, NULL);

INSERT INTO uc_lst_item_sort VALUES (101, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (101, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (101, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (101, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (101, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (101, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (102, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (102, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (102, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (102, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (102, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (102, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (103, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (103, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (103, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (103, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (103, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (103, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (104, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (104, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (104, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (104, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (104, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (104, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (105, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (105, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (105, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (105, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (105, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (105, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (106, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (106, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (106, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (106, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (106, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (106, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (107, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (107, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (107, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (107, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (107, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (107, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (108, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (108, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (108, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (108, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (108, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (108, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (109, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (109, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (109, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (109, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (109, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (109, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (110, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (110, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (110, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (110, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (110, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (110, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (111, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (111, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (111, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (111, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (111, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (111, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (112, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (112, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (112, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (112, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (112, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (112, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (113, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (113, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (113, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (113, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (113, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (113, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (114, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (114, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (114, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (114, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (114, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (114, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (115, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (115, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (115, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (115, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (115, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (115, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (116, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (116, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (116, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (116, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (116, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (116, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (117, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (117, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (117, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (117, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (117, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (117, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (118, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (118, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (118, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (118, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (118, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (118, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (119, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (119, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (119, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (119, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (119, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (119, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (120, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (120, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (120, 2, '相手先（名称）', 0, 0, 0, '(AdName1 + AdName2) COLLATE Japanese_CS_AS_KS_WS ASC, AdName1 COLLATE Japanese_CS_AS_KS_WS ASC, AdName2 COLLATE Japanese_CS_AS_KS_WS ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (120, 3, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (120, 4, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (120, 5, '登録番号', 0, 0, 0, 'Invno ASC', NULL, NULL, NULL);

INSERT INTO uc_lst_item_sort VALUES (201, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (201, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (201, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (201, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (202, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (202, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (202, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (202, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (203, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (203, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (203, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (203, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (204, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (204, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (204, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (204, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (205, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (205, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (205, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (205, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (206, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (206, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (206, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (206, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (207, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (207, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (207, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (207, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (208, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (208, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (208, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (208, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (209, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (209, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (209, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (209, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (210, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (210, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (210, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (210, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (211, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (211, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (211, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (211, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (212, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (212, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (212, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (212, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (213, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (213, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (213, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (213, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (214, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (214, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (214, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (214, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (215, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (215, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (215, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (215, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (216, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (216, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (216, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (216, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (217, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (217, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (217, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (217, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (218, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (218, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (218, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (218, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (219, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (219, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (219, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (219, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (220, 0, 'なし', 0, 0, 0, NULL, NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (220, 1, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC');
INSERT INTO uc_lst_item_sort VALUES (220, 2, '期末現在高（大きい順）', 0, 0, 0, 'Val DESC', NULL, NULL, NULL);
INSERT INTO uc_lst_item_sort VALUES (220, 3, '期末現在高（小さい順）', 0, 0, 0, 'Val ASC', NULL, NULL, NULL);

/*uc_ren_main*/
INSERT INTO uc_ren_main VALUES (1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (3, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (4, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (5, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (6, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (7, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (9, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (10, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (11, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (12, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (13, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (14, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (15, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (16, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (17, 3, 0, 0, 0, 1, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (18, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (19, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (20, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (21, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (22, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);

INSERT INTO uc_ren_main VALUES (101, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (102, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (103, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (104, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (105, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (106, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (107, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (108, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (109, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (110, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (111, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (112, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (113, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (114, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (115, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (116, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (117, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (118, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (119, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (120, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
			
INSERT INTO uc_ren_main VALUES (201, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (202, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (203, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (204, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (205, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (206, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (207, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (208, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (209, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (210, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (211, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (212, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (213, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (214, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (215, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (216, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (217, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (218, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (219, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);
INSERT INTO uc_ren_main VALUES (220, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, -1, 0, 0, 0);

/*uc_ren_kamoku*/
INSERT INTO uc_ren_kamoku VALUES (101, 1, 0, '01020100', 101, 1);
INSERT INTO uc_ren_kamoku VALUES (102, 1, 0, '01020200', 102, 1);
INSERT INTO uc_ren_kamoku VALUES (103, 1, 0, '01030200', 103, 1);
INSERT INTO uc_ren_kamoku VALUES (104, 1, 0, '01020300', 104, 1);
INSERT INTO uc_ren_kamoku VALUES (105, 1, 0, '01030100', 105, 1);
INSERT INTO uc_ren_kamoku VALUES (106, 1, 0, '01030300', 106, 1);
INSERT INTO uc_ren_kamoku VALUES (107, 1, 0, '01010100', 107, 1);
INSERT INTO uc_ren_kamoku VALUES (201, 2, 0, '01040100', 201, 1);
INSERT INTO uc_ren_kamoku VALUES (301, 3, 0, '01040200', 301, 1);
INSERT INTO uc_ren_kamoku VALUES (302, 3, 0, '01070400', 302, 1);
INSERT INTO uc_ren_kamoku VALUES (401, 4, 0, '01070500', 401, 1);
INSERT INTO uc_ren_kamoku VALUES (402, 4, 0, '01070100', 402, 1);
INSERT INTO uc_ren_kamoku VALUES (403, 4, 0, '01070600', 403, 1);
INSERT INTO uc_ren_kamoku VALUES (501, 5, 0, '01070300', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (502, 5, 0, '02030400', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (901, 9, 0, '04010100', 901, 1);
INSERT INTO uc_ren_kamoku VALUES (1001, 10, 0, '04010200', 1001, 1);
INSERT INTO uc_ren_kamoku VALUES (1002, 10, 0, '04030100', 1002, 1);
INSERT INTO uc_ren_kamoku VALUES (1003, 10, 0, '04040100', 1003, 1);
INSERT INTO uc_ren_kamoku VALUES (1101, 11, 0, '04050400', 1101, 1);
INSERT INTO uc_ren_kamoku VALUES (1102, 11, 0, '04050100', 1102, 1);
INSERT INTO uc_ren_kamoku VALUES (1103, 11, 0, '04050300', 1103, 1);
INSERT INTO uc_ren_kamoku VALUES (1301, 13, 0, '04020100', 1301, 1);
INSERT INTO uc_ren_kamoku VALUES (1302, 13, 0, '05010200', 1302, 1);
INSERT INTO uc_ren_kamoku VALUES (1701, 17, 0, '0A010100', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (1702, 17, 0, '0A010200', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (1703, 17, 0, '0A010300', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (1704, 17, 0, '0A010400', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (1705, 17, 0, '0A010500', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (1706, 17, 0, '0A010600', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (1707, 17, 0, '0A010700', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (1708, 17, 0, '0F020100', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (1709, 17, 0, '0F020200', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (1710, 17, 0, '0F020300', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (1711, 17, 0, '0F020400', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (2101, 21, 0, '0B010B00', 2101, 1);
INSERT INTO uc_ren_kamoku VALUES (2102, 21, 0, '0B010100', 2102, 1);
INSERT INTO uc_ren_kamoku VALUES (2103, 21, 0, '0B010200', 2103, 1);
INSERT INTO uc_ren_kamoku VALUES (2104, 21, 0, '0C010100', 2104, 1);
INSERT INTO uc_ren_kamoku VALUES (2201, 22, 0, '0B020900', 2201, 1);
INSERT INTO uc_ren_kamoku VALUES (2202, 22, 0, '0B020101', 2202, 1);
INSERT INTO uc_ren_kamoku VALUES (2203, 22, 0, '0B020500', 2203, 1);
INSERT INTO uc_ren_kamoku VALUES (2204, 22, 0, '0C020100', 2204, 1);

INSERT INTO uc_ren_kamoku VALUES (10101, 1, 1, '01020100', 101, 1);
INSERT INTO uc_ren_kamoku VALUES (10102, 1, 1, '01020200', 102, 1);
INSERT INTO uc_ren_kamoku VALUES (10103, 1, 1, '01030200', 103, 1);
INSERT INTO uc_ren_kamoku VALUES (10104, 1, 1, '01020300', 104, 1);
INSERT INTO uc_ren_kamoku VALUES (10105, 1, 1, '01030100', 105, 1);
INSERT INTO uc_ren_kamoku VALUES (10106, 1, 1, '01030300', 106, 1);
INSERT INTO uc_ren_kamoku VALUES (10107, 1, 1, '01010100', 107, 1);
INSERT INTO uc_ren_kamoku VALUES (10201, 2, 1, '01040100', 201, 1);
INSERT INTO uc_ren_kamoku VALUES (10301, 3, 1, '01040200', 301, 1);
INSERT INTO uc_ren_kamoku VALUES (10302, 3, 1, '01070400', 302, 1);
INSERT INTO uc_ren_kamoku VALUES (10401, 4, 1, '01070500', 401, 1);
INSERT INTO uc_ren_kamoku VALUES (10402, 4, 1, '01070100', 402, 1);
INSERT INTO uc_ren_kamoku VALUES (10403, 4, 1, '01070600', 403, 1);
INSERT INTO uc_ren_kamoku VALUES (10501, 5, 1, '01070300', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (10502, 5, 1, '02030400', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (10901, 9, 1, '04010100', 901, 1);
INSERT INTO uc_ren_kamoku VALUES (11001, 10, 1, '04010200', 1001, 1);
INSERT INTO uc_ren_kamoku VALUES (11002, 10, 1, '04030100', 1002, 1);
INSERT INTO uc_ren_kamoku VALUES (11003, 10, 1, '04040100', 1003, 1);
INSERT INTO uc_ren_kamoku VALUES (11101, 11, 1, '04050400', 1101, 1);
INSERT INTO uc_ren_kamoku VALUES (11102, 11, 1, '04050100', 1102, 1);
INSERT INTO uc_ren_kamoku VALUES (11103, 11, 1, '04050300', 1103, 1);
INSERT INTO uc_ren_kamoku VALUES (11301, 13, 1, '04020100', 1301, 1);
INSERT INTO uc_ren_kamoku VALUES (11302, 13, 1, '05010200', 1302, 1);
INSERT INTO uc_ren_kamoku VALUES (11701, 17, 1, '0A010100', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (11702, 17, 1, '0A010200', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (11703, 17, 1, '0A010300', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (11704, 17, 1, '0A010400', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (11705, 17, 1, '0A010500', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (11706, 17, 1, '0A010600', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (11707, 17, 1, '0A010700', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (11708, 17, 1, '0F020100', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (11709, 17, 1, '0F020200', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (11710, 17, 1, '0F020300', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (11711, 17, 1, '0F020400', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (12101, 21, 1, '0B010B00', 2101, 1);
INSERT INTO uc_ren_kamoku VALUES (12102, 21, 1, '0B010100', 2102, 1);
INSERT INTO uc_ren_kamoku VALUES (12103, 21, 1, '0B010200', 2103, 1);
INSERT INTO uc_ren_kamoku VALUES (12104, 21, 1, '0C010100', 2104, 1);
INSERT INTO uc_ren_kamoku VALUES (12201, 22, 1, '0B020900', 2201, 1);
INSERT INTO uc_ren_kamoku VALUES (12202, 22, 1, '0B020101', 2202, 1);
INSERT INTO uc_ren_kamoku VALUES (12203, 22, 1, '0B020500', 2203, 1);
INSERT INTO uc_ren_kamoku VALUES (12204, 22, 1, '0C020100', 2204, 1);

INSERT INTO uc_ren_kamoku VALUES (320101, 1, 32, '01020100', 101, 1);
INSERT INTO uc_ren_kamoku VALUES (320102, 1, 32, '01020200', 102, 1);
INSERT INTO uc_ren_kamoku VALUES (320103, 1, 32, '01030200', 103, 1);
INSERT INTO uc_ren_kamoku VALUES (320104, 1, 32, '01020300', 104, 1);
INSERT INTO uc_ren_kamoku VALUES (320105, 1, 32, '01030100', 105, 1);
INSERT INTO uc_ren_kamoku VALUES (320106, 1, 32, '01030300', 106, 1);
INSERT INTO uc_ren_kamoku VALUES (320107, 1, 32, '01010100', 107, 1);
INSERT INTO uc_ren_kamoku VALUES (320201, 2, 32, '01040100', 201, 1);
INSERT INTO uc_ren_kamoku VALUES (320301, 3, 32, '01040200', 301, 1);
INSERT INTO uc_ren_kamoku VALUES (320302, 3, 32, '01070400', 302, 1);
INSERT INTO uc_ren_kamoku VALUES (320303, 3, 32, '01040300', 303, 1);
INSERT INTO uc_ren_kamoku VALUES (320401, 4, 32, '01070500', 401, 1);
INSERT INTO uc_ren_kamoku VALUES (320402, 4, 32, '01070100', 402, 1);
INSERT INTO uc_ren_kamoku VALUES (320403, 4, 32, '01070600', 403, 1);
INSERT INTO uc_ren_kamoku VALUES (320501, 5, 32, '01070300', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (320502, 5, 32, '02030400', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (320901, 9, 32, '04010100', 901, 1);
INSERT INTO uc_ren_kamoku VALUES (321001, 10, 32, '04010200', 1001, 1);
INSERT INTO uc_ren_kamoku VALUES (321002, 10, 32, '04030100', 1002, 1);
INSERT INTO uc_ren_kamoku VALUES (321003, 10, 32, '04040100', 1003, 1);
INSERT INTO uc_ren_kamoku VALUES (321004, 10, 32, '04010300', 1004, 1);
INSERT INTO uc_ren_kamoku VALUES (321101, 11, 32, '04050400', 1101, 1);
INSERT INTO uc_ren_kamoku VALUES (321102, 11, 32, '04050100', 1102, 1);
INSERT INTO uc_ren_kamoku VALUES (321103, 11, 32, '04050300', 1103, 1);
INSERT INTO uc_ren_kamoku VALUES (321301, 13, 32, '04020100', 1301, 1);
INSERT INTO uc_ren_kamoku VALUES (321302, 13, 32, '05010200', 1302, 1);
INSERT INTO uc_ren_kamoku VALUES (321701, 17, 32, '0A010100', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (321702, 17, 32, '0A010200', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (321703, 17, 32, '0A010300', 1701, 1);
INSERT INTO uc_ren_kamoku VALUES (321704, 17, 32, '0A010400', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (321705, 17, 32, '0A010500', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (321706, 17, 32, '0A010600', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (321707, 17, 32, '0A010700', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (321708, 17, 32, '0F020100', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (321709, 17, 32, '0F020200', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (321710, 17, 32, '0F020300', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (321711, 17, 32, '0F020400', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (321712, 17, 32, '0F020500', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (321713, 17, 32, '0F020600', 1703, 1);
INSERT INTO uc_ren_kamoku VALUES (322101, 21, 32, '0B010B00', 2101, 1);
INSERT INTO uc_ren_kamoku VALUES (322102, 21, 32, '0B010100', 2102, 1);
INSERT INTO uc_ren_kamoku VALUES (322103, 21, 32, '0B010200', 2103, 1);
INSERT INTO uc_ren_kamoku VALUES (322104, 21, 32, '0C010100', 2104, 1);
INSERT INTO uc_ren_kamoku VALUES (322201, 22, 32, '0B020900', 2201, 1);
INSERT INTO uc_ren_kamoku VALUES (322202, 22, 32, '0B020101', 2202, 1);
INSERT INTO uc_ren_kamoku VALUES (322203, 22, 32, '0B020500', 2203, 1);
INSERT INTO uc_ren_kamoku VALUES (322204, 22, 32, '0C020100', 2204, 1);

INSERT INTO uc_ren_kamoku VALUES (480101, 1, 48, '01020100', 101, 1);
INSERT INTO uc_ren_kamoku VALUES (480102, 1, 48, '01020200', 102, 1);
INSERT INTO uc_ren_kamoku VALUES (480103, 1, 48, '01030200', 103, 1);
INSERT INTO uc_ren_kamoku VALUES (480104, 1, 48, '01020300', 104, 1);
INSERT INTO uc_ren_kamoku VALUES (480105, 1, 48, '01030100', 105, 1);
INSERT INTO uc_ren_kamoku VALUES (480106, 1, 48, '01030300', 106, 1);
INSERT INTO uc_ren_kamoku VALUES (480107, 1, 48, '01010100', 107, 1);
INSERT INTO uc_ren_kamoku VALUES (480301, 3, 48, '01040100', 302, 1);
INSERT INTO uc_ren_kamoku VALUES (480302, 3, 48, '01040200', 302, 1);
INSERT INTO uc_ren_kamoku VALUES (480401, 4, 48, '01070800', 401, 1);
INSERT INTO uc_ren_kamoku VALUES (480402, 4, 48, '01070100', 402, 1);
INSERT INTO uc_ren_kamoku VALUES (480403, 4, 48, '01070200', 403, 1);
INSERT INTO uc_ren_kamoku VALUES (480501, 5, 48, '01070400', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (480502, 5, 48, '01070500', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (480503, 5, 48, '01070600', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (480504, 5, 48, '02030400', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (480505, 5, 48, '02030500', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (480506, 5, 48, '02030600', 501, 1);
INSERT INTO uc_ren_kamoku VALUES (480901, 9, 48, '04010100', 901, 1);
INSERT INTO uc_ren_kamoku VALUES (481001, 10, 48, '04010200', 1001, 1);
INSERT INTO uc_ren_kamoku VALUES (481002, 10, 48, '04010300', 1002, 1);
INSERT INTO uc_ren_kamoku VALUES (481003, 10, 48, '04040100', 1003, 1);
INSERT INTO uc_ren_kamoku VALUES (481101, 11, 48, '04050400', 1101, 1);
INSERT INTO uc_ren_kamoku VALUES (481102, 11, 48, '04050100', 1102, 1);
INSERT INTO uc_ren_kamoku VALUES (481103, 11, 48, '04050200', 1103, 1);
INSERT INTO uc_ren_kamoku VALUES (481301, 13, 48, '04020100', 1301, 1);
INSERT INTO uc_ren_kamoku VALUES (481302, 13, 48, '04020200', 1301, 1);
INSERT INTO uc_ren_kamoku VALUES (481303, 13, 48, '04020300', 1301, 1);
INSERT INTO uc_ren_kamoku VALUES (481304, 13, 48, '04020400', 1301, 1);
INSERT INTO uc_ren_kamoku VALUES (481305, 13, 48, '05010200', 1302, 1);
INSERT INTO uc_ren_kamoku VALUES (481306, 13, 48, '05010400', 1302, 1);
INSERT INTO uc_ren_kamoku VALUES (481307, 13, 48, '05010500', 1302, 1);
INSERT INTO uc_ren_kamoku VALUES (481701, 17, 48, '0A010400', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (481702, 17, 48, '0A010500', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (481703, 17, 48, '0A010600', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (481704, 17, 48, '0A010700', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (481705, 17, 48, '0A010800', 1702, 1);
INSERT INTO uc_ren_kamoku VALUES (482101, 21, 48, '0B010B00', 2101, 1);
INSERT INTO uc_ren_kamoku VALUES (482102, 21, 48, '0B010100', 2102, 1);
INSERT INTO uc_ren_kamoku VALUES (482103, 21, 48, '0B010200', 2103, 1);
INSERT INTO uc_ren_kamoku VALUES (482104, 21, 48, '0C010100', 2104, 1);
INSERT INTO uc_ren_kamoku VALUES (482201, 22, 48, '0B020900', 2201, 1);
INSERT INTO uc_ren_kamoku VALUES (482202, 22, 48, '0B020101', 2202, 1);
INSERT INTO uc_ren_kamoku VALUES (482203, 22, 48, '0B020500', 2203, 1);
INSERT INTO uc_ren_kamoku VALUES (482204, 22, 48, '0C020100', 2204, 1);

/* vtbl に、内訳書のバージョンを登録 */

DELETE vtbl WHERE type=1001 AND objname='DBUchiwake'

INSERT INTO vtbl (type, objname, vers)
VALUES (1001, 'DBUchiwake', 11)
