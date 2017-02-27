#pragma once

//*****************************************************************************
//
//		�v���C���[�f�[�^�p(�V���O���g��)
//
//*****************************************************************************

// �v���C���[�Ɋւ�����̍\���̂������ɂ܂Ƃ߂�
namespace PlayerData
{
	// �v���C���
	struct PrevInfomation
	{
		unsigned int PlayCount	: 16;	// �V�񂾉�(65535�܂�)
		unsigned int PlayTime	: 32;	// �v���C����(4294967295)
		unsigned int coin		: 20;	// �^�ăp�`���R�R�C��(1048575�܂�)
	};

	struct Infomation
	{
		union
		{
			struct
			{
				int PlayCount;	// �V�񂾉�
				int PlayTime;	// �v���C����
				int coin;		// �^�ăp�`���R�R�C��
				int BattleCount;// �ΐ��
				int �󂫘g0;
				int �󂫘g1;
				int �󂫘g2;
				int �󂫘g3;
				int �󂫘g4;
				int �󂫘g5;
				int �󂫘g6;
				int �󂫘g7;
				int �󂫘g8;
				int �󂫘g9;
				int �󂫘g10;
				int �󂫘g11;
			};

			int iAllData[16];
		};

	};

	// �ݒ�̏��
	struct Confing
	{
		unsigned int iDifficultyAI	: 4;	// AI�̓�Փx(15�܂�)
		signed int iBGMVolume		: 8;
		signed int iSEVolume		: 8;
		signed int iVoiceVolume		: 8;
		unsigned int iRoundNumType	: 4;
		unsigned int iRoundTimeType : 4;
		unsigned int iAutoSave		: 1;
		unsigned int �󂫘g2		: 31;
		unsigned int �󂫘g3		: 32;
		unsigned int �󂫘g4		: 32;
		unsigned int �󂫘g5		: 32;
		unsigned int �󂫘g6		: 32;
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
		int �󂫘g14;
		int �󂫘g15;
		int �󂫘g16;
		int �󂫘g17;
		int �󂫘g18;
		int �󂫘g19;
		int �󂫘g20;
		int �󂫘g21;
		int �󂫘g22;
		int �󂫘g23;
		int �󂫘g24;
		int �󂫘g25;
		int �󂫘g26;
		int �󂫘g27;
		int �󂫘g28;
	};

	// �g���t�B�[
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
				int �󂫘g13;
				int �󂫘g14;
				int �󂫘g15;
				int �󂫘g16;
				int �󂫘g17;
				int �󂫘g18;
				int �󂫘g19;
				int �󂫘g20;
				int �󂫘g21;
				int �󂫘g22;
				int �󂫘g23;
				int �󂫘g24;
				int �󂫘g25;
				int �󂫘g26;
				int �󂫘g27;
				int �󂫘g28;
				int �󂫘g29;
				int �󂫘g30;
				int �󂫘g31;
				int �󂫘g32;
			};
			int iAllData[32];
		};
	};

	//struct PrevTrophy
	//{
	//	unsigned int iFirstPlay : 1;
	//	unsigned int iFirstBattle : 1;
	//	unsigned int �󂫘g : 31;
	//};

	// �B���v�f
	struct PrevSecret
	{
		unsigned int �G�E���V�A���� : 1;
		unsigned int �Z�M����		: 1;
		unsigned int �󂫘g1		: 32;
		unsigned int �󂫘g2		: 32;
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
						int Movie�󂫘g1;
						int Movie�󂫘g2;
						int Movie�󂫘g3;
						int Movie�󂫘g4;
						int Movie�󂫘g5;
						int Movie�󂫘g6;
						int Movie�󂫘g7;
						int Movie�󂫘g8;
					};
					int iAllMovie[8];
				};
				union
				{
					struct
					{
						int Pic�󂫘g1;
						int Pic�󂫘g2;
						int Pic�󂫘g3;
						int Pic�󂫘g4;
						int Pic�󂫘g5;
						int Pic�󂫘g6;
						int Pic�󂫘g7;
						int Pic�󂫘g8;
						int Pic�󂫘g9;
						int Pic�󂫘g10;
						int Pic�󂫘g11;
						int Pic�󂫘g12;
						int Pic�󂫘g13;
						int Pic�󂫘g14;
						int Pic�󂫘g15;
						int Pic�󂫘g16;
						int Pic�󂫘g17;
						int Pic�󂫘g18;
						int Pic�󂫘g19;
						int Pic�󂫘g20;
						int Pic�󂫘g21;
						int Pic�󂫘g22;
						int Pic�󂫘g23;
						int Pic�󂫘g24;
						int Pic�󂫘g25;
						int Pic�󂫘g26;
						int Pic�󂫘g27;
						int Pic�󂫘g28;
						int Pic�󂫘g29;
						int Pic�󂫘g30;
						int Pic�󂫘g31;
						int Pic�󂫘g32;
					};
					int iAllPic[32];
				};

			};	// AllUnionStruct.

			int iAllData[40];

		};	// AllUnion.
	};

	//// �v���C���[�̃I�v�V����
	//struct Option
	//{
	//	unsigned int bSudden		:1;		// �T�h�D��
	//	unsigned int SuddenPos		:10;	// �T�h�D�����W(Y)10�r�b�g(1023)
	//	unsigned int HiSpeed		:5;		// �n�C�X�s 5�r�b�g(31) * 0.25
	//	unsigned int iGaugeOption	:3;		// �Q�[�W(enum���Ə����n�ˑ��Ȃ̂Ŋ댯) 3�r�b�g(7)
	//	//unsigned int BasisOctave	:7;		// �J�n�I�N�^�[�u 7�r�b�g(127) �����\�p�ɂɕς�肻���Ȃ̂ŁA�e�L�X�g�ŊǗ�����
	//	unsigned int bMoviePlay		:1;		// ���t���ɓ�����Đ����邩�ǂ���
	//	unsigned int MovieAlpha		:8;		// ���t�w�i�ŗ�������̓����x(255)
	//	unsigned int bAutoPlay		:1;		// DJ_AUTO����
	//	unsigned int bGuide			:1;		// �K�C�h���肩�Ȃ���
	//};
}



class PlayerDataManager
{
public:
	static PlayerDataManager *GetInstance(){ static PlayerDataManager i; return &i; }

	// �Z�[�u�t���OON
	void OnSaveFlag(){ m_bSaveFlag = true; }

	// ������(�Z�[�u�f�[�^�̃��[�h)
	bool Initialize();

	PlayerData::Infomation m_PlayerInfo;	// �v���C���
	PlayerData::Confing m_ConfigData;		// �v���C���[�̐ݒ���
	PlayerData::Training m_TrainingData;	// �g���[�j���O
	PlayerData::Trophy m_TrophyData;			// �g���t�B�[
	PlayerData::Secret m_SecretData;			// �B���v�f


	// �f�X�g���N�^(�_�u���|�C���^�֘A�����̒��Ŋ��������Ă�̂ŁA�g���l�͋C�ɂ��Ȃ���OK)
	~PlayerDataManager();

	// �V�K�f�[�^�쐬
	void NewCreate();

	// �Z�[�u
	void SavePlayerData();

	void ClearData();		// ������炠������(�f�[�^�S����)

private:
	//static PlayerDataManager *m_pInstance;
	bool LoadPlayerData();

	bool m_bSaveFlag;	// true�ɂ�����������Ƃ��ɃZ�[�u����

	clock_t m_StartClock;	// �v���C���ԉ��Z�p

	PlayerDataManager();
	PlayerDataManager(const PlayerDataManager&){}
	PlayerDataManager &operator=(const PlayerDataManager&){}
};

#define PlayerDataMgr (PlayerDataManager::GetInstance())