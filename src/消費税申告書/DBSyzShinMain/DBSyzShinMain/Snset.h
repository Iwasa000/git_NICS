#pragma once

enum ShinkokuKubun {	// 申告区分　…	１＝確定 ２＝中間 ３＝予定(26号) ４＝修正確定 ５＝修正中間 ６＝更正 ７＝準確定
	kakutei = 1,
	tyuukan,
	yotei,
	kakutei_syuuse,
	tyuukan_syuuse,
	kousei,
//2018.03.13 INSERT START
	junkakutei,
//2018.03.13 INSERT END
};

using namespace std;

extern map<int,CString> GetMapSkbnName();
extern CString GetKubunStr(int kubun,int minashi);

// 現在申告区分と前回申告区分、履歴時の情報の読み込み用に。
// コンボボックス切り替えやチェックボックスなどを保持し、入力処理まで保留する。
// 区分や月単位など適度に隠ぺいしたかった。
// また、履歴時には最終申告分の情報のバックアップとなる
// GetSetでまとめて処理可であるが 要望やTBHeadの扱いのため結果的に個別に対応した。

class CSnset
{
	int		mcount;
	BYTE	Sn_SKKBN;				// 申告区分

	TimePair tyukankikan;
	TimePair kessankikan;

public:

	// 常時有効
	unsigned char	Sn_ZeiHou;			// 特殊処理サイン													szsnk->Sn_ZeiHou
	unsigned char	Sn_Syuusei;			// 特殊処理サイン													szsnk->Sn_Syuusei
	unsigned char	Sn_Sign1;			// 特殊処理サイン													szsnk->Sn_Sign1
	unsigned char	Sn_Sign2;			// 特殊処理サイン													szsnk->Sn_Sign2
	unsigned char	Sn_Sign3;			// 特殊処理サイン													szsnk->Sn_Sign3
	unsigned char	Sn_Sign4;			// 特殊処理サイン													szsnk->Sn_Sign4
	unsigned char	Sn_SPECIAL;			// 特殊サイン														szsnk->Sn_SPECIAL
	unsigned char	Sn_TUMIAG;			// 規則２２条１項の適用＆法第３０条第１項の適用（積み上げ方式）		szsnk->Sn_TUMIAG

	BYTE	Sn_JCOUNT;					// 中間申告　現在申告状態											szsnk->Sn_JCOUNT
	BYTE	Sn_MCOUNT;					// 中間申告　年間回数												szsnk->Sn_MCOUNT
	BYTE	Sn_KZTNSK;					// 課税期間短縮														szsnk->Sn_KZTNSK

	BYTE	SVsasw;						// 申告書エリア　登録ＳＷ											szvol->SVsasw		shininfo->SVsasw
	BYTE	SVmzsw;						// 複数税率処理スイッチ												szvol->SVmzsw		shininfo->SVmzsw
	BYTE	SVhksw;						// 販売奨励金収入科目登録スイッチ									szvol->SVhksw		shininfo->SVhksw
	BYTE	SVd5sw;						// 第５種事業に係る経過措置に関するスイッチ							szvol->SVd5sw		shininfo->SVd5sw
	BYTE	SVkojn;						// 個人事業種選択スイッチ											szvol->SVkojn		shininfo->SVkojn
	BYTE	SVzchg;						// 免税⇔課税業者切り替えスイッチ									szvol->SVzchg		shininfo->SVzchg
	BYTE	SVkazei;					// 消費税　課税区分／仕入税額按分法スイッチ							szvol->SVkazei		shininfo->SVkazei
	BYTE	SVsacq;						// 申告書集計スイッチ												szvol->SVsacq		shininfo->SVsacq
	BYTE	SVzisq;						// 担当税理士番号スイッチ											szvol->SVzisq		shininfo->SVzisq
	BYTE	SVsign;						// 特殊処理サイン													szvol->SVsign		shininfo->SVsign
	char	SVzuri[6];					// 基準期間の課税売上高												szvol->SVzuri		shininfo->SVzuri

	// 履歴時有効
	int		ind_type;					//																	zvolume->ind_type	shininfo->ind_type
	BYTE	S_SGN2;						//																	zvolume->s_sgn2		shininfo->S_SGN2
	BYTE	S_SGN4;						//																	zvolume->s_sgn4		shininfo->S_SGN4

	//------>特定課税仕入対応150713
	long	m_DispTabSgn;
	//<-------------------------------

	CSnset()
	{
		mcount   = 
		Sn_SKKBN = 0;

		Sn_KZTNSK = 
		Sn_MCOUNT =
		Sn_JCOUNT =
		Sn_ZeiHou =
		Sn_Syuusei=
		Sn_Sign1  =
		Sn_Sign2  =
		Sn_Sign3  =
		Sn_Sign4  =
		Sn_SPECIAL=
		Sn_TUMIAG = 0;

		ind_type =
		S_SGN2   =
		S_SGN4   = 0;

		SVsasw  =
		SVmzsw  =
		SVhksw  =
		SVd5sw  =
		SVkojn  =
		SVzchg  =
		SVkazei =
		SVsacq  =
		SVzisq  =
		SVsign  = 0;

		//------>特定課税仕入対応150713
		m_DispTabSgn = 0;
		//<-------------------------------

		memset(SVzuri,0,6);
	}
	
	CSnset(const SH_SN_HEAD& Snh,const SH_SN_DATA& Snd,const SH_SVHED& TBh)
	{
		GetSetTyukanKikan() = TimePair( CTimeControl::GetCTime(Snh.Sn_MDAYS),CTimeControl::GetCTime(Snh.Sn_MDAYE) );
		GetSetKessanKikan() = TimePair( CTimeControl::GetCTime(Snh.Sn_KDAYS),CTimeControl::GetCTime(Snh.Sn_KDAYE) );

		SetSn(Snh,Snd);
#ifdef _DEBUG
		AfxMessageBox( _T("abc") );
#endif
		SetTbh(TBh);
		SetMcount();
	}

//--> '14.01.26 INS START
	CSnset(const CSnHeadData& Snh)
	{
		GetSetTyukanKikan() = TimePair( CTimeControl::GetCTime(Snh.Sn_MDAYS),CTimeControl::GetCTime(Snh.Sn_MDAYE) );
		GetSetKessanKikan() = TimePair( CTimeControl::GetCTime(Snh.Sn_KDAYS),CTimeControl::GetCTime(Snh.Sn_KDAYE) );

		SetSnh( Snh );
		SetMcount();
	}
//<-- '14.01.26 INS END

	void SetSn(const SH_SN_HEAD& Snh,const SH_SN_DATA& Snd)
	{
		Sn_SKKBN  = Snh.Sn_SKKBN;		// 申告区分
		Sn_KZTNSK = Snh.Sn_KZTNSK;		// 課税期間短縮
		Sn_MCOUNT = Snh.Sn_MCOUNT;		// 中間申告　年間回数
		Sn_JCOUNT = Snh.Sn_JCOUNT;		// 中間申告　現在申告状態
		Sn_ZeiHou = Snd.Sn_ZeiHou;		// 特殊処理サイン
		Sn_Syuusei= Snd.Sn_Syuusei;		// 〃
		Sn_Sign1  = Snd.Sn_Sign1;		// 〃
		Sn_Sign2  = Snd.Sn_Sign2;		// 〃
		Sn_Sign3  = Snd.Sn_Sign3;		// 〃
		Sn_Sign4  = Snd.Sn_Sign4;		// 〃
		Sn_SPECIAL= Snh.Sn_SPECIAL;		// 特殊サイン
		Sn_TUMIAG = Snh.Sn_TUMIAG;		// 規則２２条１項の適用＆法第３０条第１項の適用（積み上げ方式）
	}

	void SetTbh(const SH_SVHED& TBh) // SyzShin_UpDate用に必要となった
	{
		SVsasw = TBh.SVsasw;
		SVmzsw = TBh.SVmzsw;
		SVhksw = TBh.SVhksw;
		SVd5sw = TBh.SV_5sw;
		SVkojn = TBh.SVkojn;
		SVzchg = TBh.SVzchg;
		SVkazei = TBh.SVkazei;
		SVsacq = TBh.SVsacq;
		SVzisq = TBh.SVzisq;
		SVsign = TBh.SVsign;

		memmove( SVzuri, TBh.SVzuri, 6 );
	}

//--> '14.01.26 INS START
	void SetSnh(const CSnHeadData& Snh)
	{
		Sn_SKKBN  = Snh.Sn_SKKBN;		// 申告区分
		Sn_KZTNSK = Snh.Sn_KZTNSK;		// 課税期間短縮
		Sn_MCOUNT = Snh.Sn_MCOUNT;		// 中間申告　年間回数
		Sn_JCOUNT = Snh.Sn_JCOUNT;		// 中間申告　現在申告状態
		Sn_ZeiHou = Snh.Sn_ZeiHou;		// 特殊処理サイン
		Sn_Syuusei= Snh.Sn_Syuusei;		// 〃
		Sn_Sign1  = Snh.Sn_Sign1;		// 〃
		Sn_Sign2  = Snh.Sn_Sign2;		// 〃
		Sn_Sign3  = Snh.Sn_Sign3;		// 〃
		Sn_Sign4  = Snh.Sn_Sign4;		// 〃
		Sn_SPECIAL= Snh.Sn_SPECIAL;		// 特殊サイン
		Sn_TUMIAG = Snh.Sn_TUMIAG;		// 規則２２条１項の適用＆法第３０条第１項の適用（積み上げ方式）
		SVsasw = Snh.SVsasw;
		SVmzsw = Snh.SVmzsw;
		SVhksw = Snh.SVhksw;
		SVd5sw = Snh.SVd5sw;
		SVkojn = Snh.SVkojn;
		SVzchg = Snh.SVzchg;
		SVkazei = Snh.SVkazei;
		SVsacq = Snh.SVsacq;
		SVzisq = Snh.SVzisq;
		SVsign = Snh.SVsign;

		//------>特定課税仕入対応150713
		m_DispTabSgn = Snh.m_DispTabSgn;
		//<--------------------------------

		memmove( SVzuri, Snh.SVzuri, 6 );
	}
//<-- '14.01.26 INS END

	// 第一画面でのみ変更されうるサイン
/*- '14.01.26 -*/
//	void GetSnData(SH_SN_HEAD& Snh,SH_SN_DATA& Snd,SH_SVHED& TBh)
//	{
//		CTimeControl::SetDateYMD(Snh.Sn_MDAYS, Snh.Sn_MDAYE, GetSetTyukanKikan() );
//		CTimeControl::SetDateYMD(Snh.Sn_KDAYS, Snh.Sn_KDAYE, GetSetKessanKikan() );
//		
//		Snh.Sn_MCOUNT = Sn_MCOUNT;		// 中間申告　年間回数
//		Snh.Sn_KZTNSK = Sn_KZTNSK;		// 中間申告　現在申告状態
//	
//		Snh.Sn_SKKBN  = Sn_SKKBN;		// 申告区分
//		Snh.Sn_JCOUNT = Sn_JCOUNT;		// 中間申告　現在申告状態
//	
//		Snd.Sn_ZeiHou = Sn_ZeiHou;		// 特殊処理サイン
//		Snd.Sn_Syuusei= Sn_Syuusei;		// 〃
//		Snd.Sn_Sign1  = Sn_Sign1;		// 〃
//		Snd.Sn_Sign2  = Sn_Sign2;		// 〃
//		Snd.Sn_Sign3  = Sn_Sign3;		// 〃
//		Snd.Sn_Sign4  = Sn_Sign4;		// 〃
//		Snh.Sn_SPECIAL= Sn_SPECIAL;		// 特殊サイン
//		Snh.Sn_TUMIAG = Sn_TUMIAG;		// 規則２２条１項の適用＆法第３０条第１項の適用（積み上げ方式）
//
//		TBh.SVsasw = SVsasw;
//		TBh.SVmzsw = SVmzsw;
//		TBh.SVhksw = SVhksw;
//		TBh.SV_5sw = SVd5sw;
//		TBh.SVkojn = SVkojn;
//		TBh.SVzchg = SVzchg;
//		TBh.SVkazei = SVkazei;
//		TBh.SVsacq = SVsacq;
//		TBh.SVzisq = SVzisq;
//		TBh.SVsign = SVsign;
//		memmove( TBh.SVzuri, SVzuri, 6 );
//	}
/*-------------*/
	void GetSnData( CSnHeadData& Snh )
	{
		unsigned char	stYymmdd[4]={0}, edYymmdd[4]={0};
		CTimeControl::SetDateYMD(stYymmdd, edYymmdd, GetSetTyukanKikan() );
		//Snh.SetYmdData( ID_ICSSH_CHUUKAN_FROM, 0, stYymmdd[0] );
		//Snh.SetYmdData( ID_ICSSH_CHUUKAN_FROM, 1, stYymmdd[1] );
		//Snh.SetYmdData( ID_ICSSH_CHUUKAN_FROM, 2, stYymmdd[2] );
		//Snh.SetYmdData( ID_ICSSH_CHUUKAN_TO, 0, edYymmdd[0] );
		//Snh.SetYmdData( ID_ICSSH_CHUUKAN_TO, 1, edYymmdd[1] );
		//Snh.SetYmdData( ID_ICSSH_CHUUKAN_TO, 2, edYymmdd[2] );
		Snh.SetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, 0, stYymmdd[1], stYymmdd[0] );
		Snh.SetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, 1, stYymmdd[2] );
		Snh.SetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, 2, stYymmdd[3] );
		Snh.SetYmdDataGen( ID_ICSSH_CHUUKAN_TO, 0, edYymmdd[1], edYymmdd[0] );
		Snh.SetYmdDataGen( ID_ICSSH_CHUUKAN_TO, 1, edYymmdd[2] );
		Snh.SetYmdDataGen( ID_ICSSH_CHUUKAN_TO, 2, edYymmdd[3] );
		memset( stYymmdd, '\0', sizeof(stYymmdd) );
		memset( edYymmdd, '\0', sizeof(edYymmdd) );
		CTimeControl::SetDateYMD(stYymmdd, edYymmdd, GetSetKessanKikan() );
		//Snh.SetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, 0, stYymmdd[0] );
		//Snh.SetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, 1, stYymmdd[1] );
		//Snh.SetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, 2, stYymmdd[2] );
		//Snh.SetYmdData( ID_ICSSH_KAZEIKIKAN_TO, 0, edYymmdd[0] );
		//Snh.SetYmdData( ID_ICSSH_KAZEIKIKAN_TO, 1, edYymmdd[1] );
		//Snh.SetYmdData( ID_ICSSH_KAZEIKIKAN_TO, 2, edYymmdd[2] );
		Snh.SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, 0, stYymmdd[1], stYymmdd[0] );
		Snh.SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, 1, stYymmdd[2] );
		Snh.SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, 2, stYymmdd[3] );
		Snh.SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, 0, edYymmdd[1], edYymmdd[0] );
		Snh.SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, 1, edYymmdd[2] );
		Snh.SetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, 2, edYymmdd[3] );
		
		Snh.Sn_MCOUNT = Sn_MCOUNT;		// 中間申告　年間回数
		Snh.Sn_KZTNSK = Sn_KZTNSK;		// 中間申告　現在申告状態
	
		Snh.Sn_SKKBN  = Sn_SKKBN;		// 申告区分
		Snh.Sn_JCOUNT = Sn_JCOUNT;		// 中間申告　現在申告状態
	
		Snh.Sn_ZeiHou = Sn_ZeiHou;		// 特殊処理サイン
		Snh.Sn_Syuusei= Sn_Syuusei;		// 〃
		Snh.Sn_Sign1  = Sn_Sign1;		// 〃
		Snh.Sn_Sign2  = Sn_Sign2;		// 〃
		Snh.Sn_Sign3  = Sn_Sign3;		// 〃
//-- '15.07.25 --
//		Snh.Sn_Sign4  = Sn_Sign4;		// 〃
//---------------
		Snh.Sn_Sign4  &= ~(0x80|0x40);
		Snh.Sn_Sign4  |= (Sn_Sign4&(0x80|0x40));		// 〃
//---------------
		Snh.Sn_SPECIAL= Sn_SPECIAL;		// 特殊サイン
		Snh.Sn_TUMIAG = Sn_TUMIAG;		// 規則２２条１項の適用＆法第３０条第１項の適用（積み上げ方式）

		Snh.SVsasw = SVsasw;
		Snh.SVmzsw = SVmzsw;
		Snh.SVhksw = SVhksw;
		Snh.SVd5sw = SVd5sw;
		Snh.SVkojn = SVkojn;
		Snh.SVzchg = SVzchg;
		Snh.SVkazei = SVkazei;
		Snh.SVsacq = SVsacq;
		Snh.SVzisq = SVzisq;
		Snh.SVsign = SVsign;

		//------>特定課税仕入対応150713
		Snh.m_DispTabSgn = m_DispTabSgn;
		//<--------------------------------

		memmove( Snh.SVzuri, SVzuri, 6 );
	}
/*-------------*/

	// 第一画面での変更と入力以降の変更のマージ(みなし課税期間のチェックボックスは除く)
	void MargeSnData(SH_SN_HEAD& Snh,SH_SN_DATA& Snd,SH_SVHED& TBh)
	{
		// 第一画面のチェックボックスの内容を反映します。
		// OnBnClickedCheck1()
		// OnBnClickedCheck2()
		Snh.Sn_TUMIAG &= ~(0x01|0x02);
		Snh.Sn_TUMIAG |= Sn_TUMIAG&(0x01|0x02);

		// OnBnClickedCheck3()
		Snh.Sn_SPECIAL &= ~0x01;
		Snh.Sn_SPECIAL |= Sn_SPECIAL&0x01;

		// OnBnClickedCheck4()
		Snd.Sn_Sign2 &= ~0x40;
		Snd.Sn_Sign2 |= Sn_Sign2&0x40;
 
		// OnBnClickedCheck5()
		TBh.SVmzsw &= ~0x01;
		TBh.SVmzsw |= SVmzsw&0x01;

		// OnBnClickedCheck6()
		Snd.Sn_Sign4 &= ~0x80;
		Snd.Sn_Sign4 |= Sn_Sign4&0x80;
	
		// OnBnClickedCheck8()
		Snd.Sn_ZeiHou &= ~0x80;
		Snd.Sn_ZeiHou |= Sn_ZeiHou&0x80;
	
		SetSn(Snh,Snd);
#ifdef _DEBUG
		AfxMessageBox( _T("def") );
#endif
		SetTbh(TBh);
	}

	// 第一画面での変更と入力以降の変更のマージ(みなし課税期間のチェックボックスは除く)
	void MargeSnData( CSnHeadData& Snh )
	{
		// 第一画面のチェックボックスの内容を反映します。
		// OnBnClickedCheck1()
		// OnBnClickedCheck2()
		Snh.Sn_TUMIAG &= ~(0x01|0x02);
		Snh.Sn_TUMIAG |= Sn_TUMIAG&(0x01|0x02);

		// OnBnClickedCheck3()
		Snh.Sn_SPECIAL &= ~0x01;
		Snh.Sn_SPECIAL |= Sn_SPECIAL&0x01;

		// OnBnClickedCheck4()
		Snh.Sn_Sign2 &= ~0x40;
		Snh.Sn_Sign2 |= Sn_Sign2&0x40;
 
		// OnBnClickedCheck5()
		Snh.SVmzsw &= ~0x01;
		Snh.SVmzsw |= SVmzsw&0x01;

		// OnBnClickedCheck6()
		Snh.Sn_Sign4 &= ~0x80;
		Snh.Sn_Sign4 |= Sn_Sign4&0x80;
	
		// OnBnClickedCheck8()
		Snh.Sn_ZeiHou &= ~0x80;
		Snh.Sn_ZeiHou |= Sn_ZeiHou&0x80;
	
		SetSnh(Snh);
	}

	void SetMcount()
	{
		if( IsTyukanShinkoku() ) {
			mcount = Sn_MCOUNT;
		}else{
			switch( Sn_KZTNSK&0x0f ) {
			case 0x01 :	mcount = 3;	break;	// ３ヶ月
			case 0x02 :	mcount = 11;	break;	// １ヶ月
			default :	mcount = 0;	break;	// 無し
			}
		}
	}

	TimePair& GetSetTyukanKikan()
	{
		return tyukankikan;
	}

	TimePair& GetSetKessanKikan()
	{
		return kessankikan;
	}

	BYTE& GetSn_SKKBN(){ return Sn_SKKBN; }

	BYTE  GetKubun(){ 
		if( (Sn_SKKBN == 0x01) && (Sn_Syuusei&0x02) ) return kousei;
		if( (Sn_SKKBN == 0x02) && (Sn_Syuusei&0x08) ) return yotei;
//2018.03.13 INSERT START
		if( (Sn_SKKBN == 0x01) && (Sn_Syuusei&0x10) ) return junkakutei;
//2018.03.13 INSERT END
		switch( Sn_SKKBN ) {
			case 1 :	return kakutei;
			case 2 :	return tyuukan;
			case 3 :	return kakutei_syuuse;
			default:	return tyuukan_syuuse;
		}
	}
	
	void  SetKubun( int kubun ){
//2018.03.13 UPDATE START
//		Sn_Syuusei &= ~0x0E;
		Sn_Syuusei &= ~0x1E;
//2018.03.13 UPDATE END
//		Snh.Sn_Syuusei &= ~0x0E;
		switch( kubun ) {
			case kousei :	// 更正の請求書
						Sn_SKKBN = 0x01;
//						Snh.Sn_SKKBN = 0x01;
						Sn_Syuusei |= 0x02;
//						Snh.Sn_Syuusei |= 0x02;
						break;
			case yotei :	// 予定(26号)
						Sn_SKKBN = 0x02;
//						Snh.Sn_SKKBN = 0x02;
						Sn_Syuusei |= 0x08;
//						Snh.Sn_Syuusei |= 0x08;
						break;
			case kakutei:
						Sn_SKKBN = 1;
//						Snh.Sn_SKKBN = 1;
						break;
			case tyuukan:
						Sn_SKKBN = 2;
//						Snh.Sn_SKKBN = 2;
						break;
			case kakutei_syuuse:
						Sn_SKKBN = 3;
//						Snh.Sn_SKKBN = 3;
						break;
//2018.03.13 INSERT START
			case junkakutei: //準確定
						Sn_SKKBN = 0x01;
						Sn_Syuusei |= 0x10;
						break;
//2018.03.13 INSERT END
			default:
						Sn_SKKBN = 4;
//						Snh.Sn_SKKBN = 4;
						break;
		}
	}

	int& GetSetMcount()
	{
		return mcount;
	}

	CString GetKubunStr()
	{
		return ::GetKubunStr( GetKubun(), IsMinashi() );
	}

	int IsMinashi()
	{
		return Sn_Sign4&0x40;
	}

	int IsKikanTansyuku()
	{
		return Sn_KZTNSK ? TRUE : FALSE;
	}

	int IsTyukanShinkoku()
	{
		const int skkbn = GetKubun();
		return skkbn == tyuukan || skkbn == tyuukan_syuuse || skkbn == yotei;
	}

	int IsKakuteiShinkoku()
	{
		const int skkbn = GetKubun();
//2018.03.13 UPDATE START
//		return skkbn == kakutei || skkbn == kakutei_syuuse || skkbn == kousei;
		return skkbn == kakutei || skkbn == kakutei_syuuse || skkbn == kousei || skkbn == junkakutei;
//2018.03.13 UPDATE END
	}

//--> '15.11.14 INS START
	int IsHirendou()
	{
		return Sn_Sign4&0x80;
	}
//<-- '15.11.14 INS END

};