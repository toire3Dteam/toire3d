#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseTrophy.h"

//+--------------------------
//	�g���t�B�[�̊Ǘ��N���X
//+--------------------------

// �g���t�B�[�̎��
enum class TROPHY_TYPE
{
	FIRST,				//	�n�߂̈��
	FIRST_BATTLE,		//	�n�߂Ẵo�g��
	BIG_DAMAGE,			//	�������Η�
	_2,_3,_4,_5,_6,
	_7, _8, _9, _10, _11, _12,
	_13, _14, _15, _16, _17, _18,
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

private:

	// �g���t�B�[�B
	BaseTrophy* m_pTrophy[(int)TROPHY_TYPE::ARRAY_END];

	// �I�����Ă���g���t�B�[
	TROPHY_TYPE m_eSelectTrophy;

	// ���b�N�A�C�R��
	tdn2DObj* m_pRockIcon;

public:
	//+---------------------------------------------
	//	���F�X�ȃg���t�B�[�̏���������Ă���
	//+---------------------------------------------
	void InitBattleMission();
	void UpdateBattleMission();


private:
	

};

// ���̉�
#define TrophyMgr (TrophyManager::GetInstance())