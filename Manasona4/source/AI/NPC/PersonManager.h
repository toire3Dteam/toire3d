#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
�\�̔g��}�l�[�W���[.h

*/

class BasePerson;

class PersonManager:public BaseGameEntity
{
public:
	static PersonManager& GetInstance();
	static void Release();
	virtual ~PersonManager();

private:
	static PersonManager* pInstance;

	// Entity�̃J�E���g
	static int m_IDcount;

	// ����
	PersonManager();
	PersonManager(const PersonManager&);
	PersonManager& operator=(const PersonManager&);

	// �l�f�[�^
	std::list<BasePerson*> m_PersonData;
	std::list<BasePerson*>::iterator m_it;


public:
	// ��ʐ��Y
	/*
		���ŏ���ɂh�c��U��
	*/
	ENTITY_ID AddPerson( Vector3 pos, bool isAi = false);

	//	���Z�b�g
	void Reset();

	// �X�V�E�`��
	void Update();
	void Render();

	// ���b�Z�[�W�X�V
	bool  HandleMessage(const Message& msg);

	// �A�N�Z�T
	BasePerson* GetPerson(int no);
	int GetPersonSize() { return (int)m_PersonData.size(); }

	// �߂��L�����N�^�[��T��
	Vector3 NearPlayerPosSearch(PLAYER_INFO* info );
	// �߂��L�����N�^�[��T��
	BasePerson* NearPlayerSearch(PLAYER_INFO* info);

	// �߂��L�����N�^�[��T��
	ENTITY_ID SoccerNearPlayerSearch(PLAYER_INFO* info);


};


// �C���X�^���X��
#define PersonMgr	(PersonManager::GetInstance())


