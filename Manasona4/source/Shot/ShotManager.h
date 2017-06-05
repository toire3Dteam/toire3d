#pragma once

#include "../BaseEntity/Entity/BaseGameEntity.h"

// �O���錾
enum class SIDE;
namespace Shot{class Base;}

class ShotManager : public BaseGameEntity
{
public:

	//+-----------------------------------------------------
	//	����
	//+-----------------------------------------------------
	static ShotManager* GetInstance()
	{
		if (m_pInstance == nullptr)m_pInstance = new ShotManager();
		return m_pInstance;
	}

	static void Release() { SAFE_DELETE(m_pInstance); }

	//------------------------------------------------------
	//	�f�X�g���N�^(���X�g�̊J��)
	//------------------------------------------------------
	~ShotManager();

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update();
	void EraseCheakUpdate();
	void AllEraseSide(SIDE eSide);

	//------------------------------------------------------
	//	�`��
	//------------------------------------------------------
	void Render();
	//void Render(tdnShader *shader, char *name);


	//------------------------------------------------------
	//	���b�Z�[�W��M(BaseGameEntity�̃I�[�o�[���C�h)
	//------------------------------------------------------
	bool  HandleMessage(const Message& msg);

	//------------------------------------------------------
	//	�V���b�g���X�g�ǉ�
	//------------------------------------------------------
	void AddShot(SIDE side, Shot::Base *pNewShot){ m_list[(int)side].push_back(pNewShot); }
	void Reset();

	//------------------------------------------------------
	//	�Q�b�^�[
	//------------------------------------------------------
	std::list<Shot::Base*> *GetList(SIDE side){ return &m_list[(int)side]; }

private:
	// 1�����Ȃ�����
	static ShotManager *m_pInstance;

	//------------------------------------------------------
	//	�R���X�g���N�^
	//------------------------------------------------------
	ShotManager();
	ShotManager(const ShotManager&);
	ShotManager &operator=(const ShotManager&) {};

	//------------------------------------------------------
	//	��ѓ�����i�[���郊�X�g
	//------------------------------------------------------
	std::list<Shot::Base*> m_list[(int)SIDE::ARRAY_MAX];

};

#define ShotMgr	ShotManager::GetInstance()
