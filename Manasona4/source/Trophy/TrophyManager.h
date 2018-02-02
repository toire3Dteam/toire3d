#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseTrophy.h"
#include "Data\SelectData.h"

//+--------------------------
//	�g���t�B�[�̊Ǘ��N���X
//+--------------------------

// �g���t�B�[�̎��
enum class TROPHY_TYPE
{
	FIRST,				//	�n�߂̈��
	FIRST_BATTLE,		//	�n�߂Ẵo�g��
	BIG_DAMAGE,			//	�������Η�
	MAX_HP_FINISH,		//	�ő�HP����
	SPEED_FINISH,		//	�ő�����
	TRAINING_TIME,		//  �g���[�j���O�łЂ�������K
	POWERFUL_ENEMY,		//  �ō���Փx��COM��|����
	SS_RANK_WIN,		//  SS�����N�œ|����
	SS_POWERFUL_ENEMY,	//  �ō���Փx��COM+SS�����N�œ|����
	MENY_CONTENT,		//  �R���e���c����������������
	CHALLENGE_CLEAR,	//  �`�������W���[�h�ŉ����^�X�N�����Ȃ���
	ATTAKAI_BOM,		//  �A�b�^�J�C�̃{��
	AIROU_CHALLENGE,	//	�Ԃ�Ԃ�`�������W
	TEKI_CHALLENGE,		//	�e�L�`�������W
	BIG_BIG_DAMAGE,		//	���������Η�
	COMPLETE_TROPHY,	//  �S�Ẵg���t�B�[���W�߂�


	//_6,
	//_7, _8, _9, _10, _11, _12,
	//_13, _14, _15, _16, _17, _18,
	ARRAY_END
};

// ��ŏ\���Ȃ̂Ƃǂ��ł������̂ŃV���O���g����
class TrophyManager : public BaseGameEntity
{
private:
	TrophyManager();
	~TrophyManager();
	TrophyManager(const TrophyManager&);
	TrophyManager &operator=(const TrophyManager&) {}

	// 1�����Ȃ�����
	static TrophyManager *m_pInstance;

public:
	// ���̎擾
	static TrophyManager *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new TrophyManager();
		return m_pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(m_pInstance);
	}

	void Update();
	void Render();
	void RenderRoom(int iTrophyType, int iX, int iY);	
	void RenderInfo(int iTrophyType, int iX, int iY);

	void Stop();		// ���o��؂�

	// �S�f�[�^����Ȃ�
	void AllReset();

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

	// �A�N�Z�T
	// �g���t�B�[�̒��g
	TROPHY_DATA GetTrophyData(int iTrophyType) { return m_pTrophy[iTrophyType]->GetTrophy(); }
	//tdn2DObj* GetIcon(int iTrophyType) { m_pTrophy[iTrophyType]->GetTrophy().pIcon; }
	BaseTrophy* GetTrophy(int iTrophyType) { return m_pTrophy[iTrophyType]; };

	// �g���t�B�[�ő吔
	int GetMaxTrophyNum() { return (int)TROPHY_TYPE::ARRAY_END; };

	// �g���t�B�[������
	int GetTrophyOwned();

	//void SetTrainingFrame(int iFrame) { m_iTrainingFrame = iFrame; };
private:

	// �g���t�B�[�B
	BaseTrophy* m_pTrophy[(int)TROPHY_TYPE::ARRAY_END];

	// �I�����Ă���g���t�B�[
	TROPHY_TYPE m_eSelectTrophy;

	// ���b�N�A�C�R��
	tdn2DObj* m_pRockIcon;

	// �g���t�B�[��������Ă�������
	std::vector<TROPHY_TYPE> m_aActionStack;
	int m_iIntervalFrame;	//	�A���œ��肵���ꍇ�Ԋu��x�炷�p

public:
	//+---------------------------------------------
	//	���F�X�ȃg���t�B�[�̏���������Ă���
	//+---------------------------------------------
	void InitSeceneMain();

	void CheakPlayCount(int iPlayCount);
	void CheakBattleCount(int iBattleCount);
	void CheakBigDamage(bool bVS = false);// �ΐ�t���O
	void CheakMaxHpFinish(int iRemainingHP);
	void CheakSpeedFinish(int iElapsedTime);
	void CheakTrainingTime();
	void CheakPowerfulEnemyWin(SIDE eWinnerSide);
	void CheakRank(SIDE eWinnerSide, RANK_TYPE eRank);
	void CheakBuyManyContent(int iOwnedContent);
	void CheakChallengeClear();


private:
	int m_iTrainingFrame;

};

// ���̉�
#define TrophyMgr (TrophyManager::GetInstance())