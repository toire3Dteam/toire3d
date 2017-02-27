#pragma once

//*****************************************************************************
//
//		ƒvƒŒƒCƒ„[ƒf[ƒ^—p(ƒVƒ“ƒOƒ‹ƒgƒ“)
//
//*****************************************************************************

// ƒvƒŒƒCƒ„[‚ÉŠÖ‚·‚éî•ñ‚Ì\‘¢‘Ì‚ğ‚±‚±‚É‚Ü‚Æ‚ß‚é
namespace PlayerData
{
	// ƒvƒŒƒCî•ñ
	struct PrevInfomation
	{
		unsigned int PlayCount	: 16;	// —V‚ñ‚¾‰ñ”(65535‚Ü‚Å)
		unsigned int PlayTime	: 32;	// ƒvƒŒƒCŠÔ(4294967295)
		unsigned int coin		: 20;	// ^‰Äƒpƒ`ƒ“ƒRƒRƒCƒ“(1048575‚Ü‚Å)
	};

	struct Infomation
	{
		union
		{
			struct
			{
				int PlayCount;	// —V‚ñ‚¾‰ñ”
				int PlayTime;	// ƒvƒŒƒCŠÔ
				int coin;		// ^‰Äƒpƒ`ƒ“ƒRƒRƒCƒ“
				int BattleCount;// ‘Îí‰ñ”
				int ‹ó‚«˜g0;
				int ‹ó‚«˜g1;
				int ‹ó‚«˜g2;
				int ‹ó‚«˜g3;
				int ‹ó‚«˜g4;
				int ‹ó‚«˜g5;
				int ‹ó‚«˜g6;
				int ‹ó‚«˜g7;
				int ‹ó‚«˜g8;
				int ‹ó‚«˜g9;
				int ‹ó‚«˜g10;
				int ‹ó‚«˜g11;
			};

			int iAllData[16];
		};

	};

	// İ’è‚Ìî•ñ
	struct Confing
	{
		unsigned int iDifficultyAI	: 4;	// AI‚Ì“ïˆÕ“x(15‚Ü‚Å)
		signed int iBGMVolume		: 8;
		signed int iSEVolume		: 8;
		signed int iVoiceVolume		: 8;
		unsigned int iRoundNumType	: 4;
		unsigned int iRoundTimeType : 4;
		unsigned int iAutoSave		: 1;
		unsigned int ‹ó‚«˜g2		: 31;
		unsigned int ‹ó‚«˜g3		: 32;
		unsigned int ‹ó‚«˜g4		: 32;
		unsigned int ‹ó‚«˜g5		: 32;
		unsigned int ‹ó‚«˜g6		: 32;
	};

	struct Training
	{
		int iHpRecovery;
		int iHp1P;
		int iHp2P;
		int iSpGage;
		int iPartnerRecovery;
		int iInfo;
		int iEnemyState;
		int iEnemyGuard;
		int iEnemyTech;
		int iEnemyThrowTech;
		int iEnemyGuardSwitch;
		int iEnemyLv;
		int iWassyoiGage;
		int ‹ó‚«˜g14;
		int ‹ó‚«˜g15;
		int ‹ó‚«˜g16;
		int ‹ó‚«˜g17;
		int ‹ó‚«˜g18;
		int ‹ó‚«˜g19;
		int ‹ó‚«˜g20;
		int ‹ó‚«˜g21;
		int ‹ó‚«˜g22;
		int ‹ó‚«˜g23;
		int ‹ó‚«˜g24;
		int ‹ó‚«˜g25;
		int ‹ó‚«˜g26;
		int ‹ó‚«˜g27;
		int ‹ó‚«˜g28;
	};

	// ƒgƒƒtƒB[
	struct Trophy
	{
		union
		{
			struct
			{
				int iFirstPlay;
				int iFirstBattle;
				int iBigDamage;
				int iMaxHpFinish;
				int iSpeedFinish;
				int iTrainingTime;
				int iPowerfulEnemyWin;
				int iSSRankWin;
				int iSSRankPowerfulEnemyWin;
				int iBuyManyContent;
				int iChallengeClear;	
				int iCompleteTrophy;
				int ‹ó‚«˜g13;
				int ‹ó‚«˜g14;
				int ‹ó‚«˜g15;
				int ‹ó‚«˜g16;
				int ‹ó‚«˜g17;
				int ‹ó‚«˜g18;
				int ‹ó‚«˜g19;
				int ‹ó‚«˜g20;
				int ‹ó‚«˜g21;
				int ‹ó‚«˜g22;
				int ‹ó‚«˜g23;
				int ‹ó‚«˜g24;
				int ‹ó‚«˜g25;
				int ‹ó‚«˜g26;
				int ‹ó‚«˜g27;
				int ‹ó‚«˜g28;
				int ‹ó‚«˜g29;
				int ‹ó‚«˜g30;
				int ‹ó‚«˜g31;
				int ‹ó‚«˜g32;
			};
			int iAllData[32];
		};
	};

	//struct PrevTrophy
	//{
	//	unsigned int iFirstPlay : 1;
	//	unsigned int iFirstBattle : 1;
	//	unsigned int ‹ó‚«˜g : 31;
	//};

	// ‰B‚µ—v‘f
	struct PrevSecret
	{
		unsigned int ƒGƒEƒŒƒVƒA‰ğ‹Ö : 1;
		unsigned int ŒZ‹M‰ğ‹Ö		: 1;
		unsigned int ‹ó‚«˜g1		: 32;
		unsigned int ‹ó‚«˜g2		: 32;
	};

	struct Secret
	{
		union
		{
			struct
			{
				union
				{
					struct
					{
						int Movie‹ó‚«˜g1;
						int Movie‹ó‚«˜g2;
						int Movie‹ó‚«˜g3;
						int Movie‹ó‚«˜g4;
						int Movie‹ó‚«˜g5;
						int Movie‹ó‚«˜g6;
						int Movie‹ó‚«˜g7;
						int Movie‹ó‚«˜g8;
					};
					int iAllMovie[8];
				};
				union
				{
					struct
					{
						int Pic‹ó‚«˜g1;
						int Pic‹ó‚«˜g2;
						int Pic‹ó‚«˜g3;
						int Pic‹ó‚«˜g4;
						int Pic‹ó‚«˜g5;
						int Pic‹ó‚«˜g6;
						int Pic‹ó‚«˜g7;
						int Pic‹ó‚«˜g8;
						int Pic‹ó‚«˜g9;
						int Pic‹ó‚«˜g10;
						int Pic‹ó‚«˜g11;
						int Pic‹ó‚«˜g12;
						int Pic‹ó‚«˜g13;
						int Pic‹ó‚«˜g14;
						int Pic‹ó‚«˜g15;
						int Pic‹ó‚«˜g16;
						int Pic‹ó‚«˜g17;
						int Pic‹ó‚«˜g18;
						int Pic‹ó‚«˜g19;
						int Pic‹ó‚«˜g20;
						int Pic‹ó‚«˜g21;
						int Pic‹ó‚«˜g22;
						int Pic‹ó‚«˜g23;
						int Pic‹ó‚«˜g24;
						int Pic‹ó‚«˜g25;
						int Pic‹ó‚«˜g26;
						int Pic‹ó‚«˜g27;
						int Pic‹ó‚«˜g28;
						int Pic‹ó‚«˜g29;
						int Pic‹ó‚«˜g30;
						int Pic‹ó‚«˜g31;
						int Pic‹ó‚«˜g32;
					};
					int iAllPic[32];
				};

			};	// AllUnionStruct.

			int iAllData[40];

		};	// AllUnion.
	};

	//// ƒvƒŒƒCƒ„[‚ÌƒIƒvƒVƒ‡ƒ“
	//struct Option
	//{
	//	unsigned int bSudden		:1;		// ƒTƒhƒDƒ“
	//	unsigned int SuddenPos		:10;	// ƒTƒhƒDƒ“À•W(Y)10ƒrƒbƒg(1023)
	//	unsigned int HiSpeed		:5;		// ƒnƒCƒXƒs 5ƒrƒbƒg(31) * 0.25
	//	unsigned int iGaugeOption	:3;		// ƒQ[ƒW(enum‚¾‚Æˆ—ŒnˆË‘¶‚È‚Ì‚ÅŠëŒ¯) 3ƒrƒbƒg(7)
	//	//unsigned int BasisOctave	:7;		// ŠJnƒIƒNƒ^[ƒu 7ƒrƒbƒg(127) ¦Œ‹\•p”É‚É•Ï‚í‚è‚»‚¤‚È‚Ì‚ÅAƒeƒLƒXƒg‚ÅŠÇ—‚·‚é
	//	unsigned int bMoviePlay		:1;		// ‰‰‘t‚É“®‰æ‚ğÄ¶‚·‚é‚©‚Ç‚¤‚©
	//	unsigned int MovieAlpha		:8;		// ‰‰‘t”wŒi‚Å—¬‚·“®‰æ‚Ì“§–¾“x(255)
	//	unsigned int bAutoPlay		:1;		// DJ_AUTO‚³‚ñ
	//	unsigned int bGuide			:1;		// ƒKƒCƒh‚ ‚è‚©‚È‚µ‚©
	//};
}



class PlayerDataManager
{
public:
	static PlayerDataManager *GetInstance(){ static PlayerDataManager i; return &i; }

	// ƒZ[ƒuƒtƒ‰ƒOON
	void OnSaveFlag(){ m_bSaveFlag = true; }

	// ‰Šú‰»(ƒZ[ƒuƒf[ƒ^‚Ìƒ[ƒh)
	bool Initialize();

	PlayerData::Infomation m_PlayerInfo;	// ƒvƒŒƒCî•ñ
	PlayerData::Confing m_ConfigData;		// ƒvƒŒƒCƒ„[‚Ìİ’èî•ñ
	PlayerData::Training m_TrainingData;	// ƒgƒŒ[ƒjƒ“ƒO
	PlayerData::Trophy m_TrophyData;			// ƒgƒƒtƒB[
	PlayerData::Secret m_SecretData;			// ‰B‚µ—v‘f


	// ƒfƒXƒgƒ‰ƒNƒ^(ƒ_ƒuƒ‹ƒ|ƒCƒ“ƒ^ŠÖ˜A‚ğ‚±‚Ì’†‚ÅŠ®Œ‹‚³‚¹‚Ä‚é‚Ì‚ÅAg‚¤l‚Í‹C‚É‚µ‚È‚­‚ÄOK)
	~PlayerDataManager();

	// V‹Kƒf[ƒ^ì¬
	void NewCreate();

	// ƒZ[ƒu
	void SavePlayerData();

	void ClearData();		// ‚â‚Á‚½‚ç‚ ‚©‚ñ‚â‚Â(ƒf[ƒ^‘SÁ‹)

private:
	//static PlayerDataManager *m_pInstance;
	bool LoadPlayerData();

	bool m_bSaveFlag;	// true‚É‚µ‚½‚çÁ‚µ‚½‚Æ‚«‚ÉƒZ[ƒu‚·‚é

	clock_t m_StartClock;	// ƒvƒŒƒCŠÔ‰ÁZ—p

	PlayerDataManager();
	PlayerDataManager(const PlayerDataManager&){}
	PlayerDataManager &operator=(const PlayerDataManager&){}
};

#define PlayerDataMgr (PlayerDataManager::GetInstance())