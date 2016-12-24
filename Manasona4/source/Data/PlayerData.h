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
	struct Infomation
	{
		unsigned int PlayCount	: 16;	// �V�񂾉�(65535�܂�)
		unsigned int PlayTime	: 32;	// �v���C����(4294967295)
		unsigned int coin		: 20;	// �^�ăp�`���R�R�C��(1048575�܂�)
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
		unsigned int �󂫘g2		: 32;
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
		int �󂫘g7;
		int �󂫘g8;
		int �󂫘g9;
		int �󂫘g10;
		int �󂫘g11;
		int �󂫘g12;
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
	};

	// �g���t�B�[
	struct Trophy
	{
		unsigned int �Q�[�����v���C : 1;
		unsigned int �󂫘g			: 32;
	};

	// �B���v�f
	struct Secret
	{
		unsigned int �G�E���V�A���� : 1;
		unsigned int �Z�M����		: 1;
		unsigned int �󂫘g1		: 32;
		unsigned int �󂫘g2		: 32;
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

private:
	//static PlayerDataManager *m_pInstance;
	bool LoadPlayerData();
	void ClearData();		// ������炠������(�f�[�^�S����)

	bool m_bSaveFlag;	// true�ɂ�����������Ƃ��ɃZ�[�u����

	clock_t m_StartClock;	// �v���C���ԉ��Z�p

	PlayerDataManager();
	PlayerDataManager(const PlayerDataManager&){}
	PlayerDataManager &operator=(const PlayerDataManager&){}
};

#define PlayerDataMgr (PlayerDataManager::GetInstance())