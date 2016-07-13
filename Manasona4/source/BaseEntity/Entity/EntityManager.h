#pragma once
#include <map>
#include <cassert>
#include <string>
#include "BaseGameEntity.h"

/*
	�G�[�W�F���g�̃|�C���^�̃}�l�[�W���[

*/




class BaseGameEntity;

class EntityManager
{
public:
	static EntityManager* Instance();
	~EntityManager() {};

	//���̊֐��̓G���e�B�e�B��ID�Ŏ����ꂽ�C���f�b�N�X�̈ʒu�ɂ���
	// �G���e�B�e�B�o�^�I
	bool            Register(BaseGameEntity* NewEntity);

	//�@�p�����[�^�Ƃ��ė^����ꂽID�����G���e�B�e�B�̃|�C���^��Ԃ�
	BaseGameEntity* GetEntityFromID(ENTITY_ID id)const;

	// ���̃��\�b�h�̓��X�g����G���e�B�e�B���폜����
	bool            Erace(BaseGameEntity* pEntity);

private:

	// ���͂��y�ɂ��邽�߂�
	typedef std::map<ENTITY_ID, BaseGameEntity*> EntityMap;

	// �G���e�B�e�B�������ɒ��ׂ���p��
	// ���̂Ȃ��ł̓G���e�B�e�B�ւ̃|�C���^�͂���ID�ő��ݎQ�Ƃ����
	EntityMap m_EntityMap;

	// ����
	EntityManager() {};
	
	// ctor���R�s�[�����蓖�Ă�Private��
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);



};

// �C���X�^���X��
#define EntityMgr EntityManager::Instance()
