#include "EntityManager.h"






EntityManager* EntityManager::Instance()
{
	static EntityManager instance;

	return &instance;
}

BaseGameEntity* EntityManager::GetEntityFromID(ENTITY_ID id)const
{
	// �G���e�B�e�B�������܂�
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	// �G���e�B�e�B���}�b�v�̃����o�[���ǂ���
	assert((ent != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

	// BaseEntity�̃|�C���^��Ԃ�
	return ent->second;
}



// �v�f�o�^
bool EntityManager::Register(BaseGameEntity* NewEntity)
{
	// ���̊֐���BaseGameEntity�̃R���X�g���N�^�ŌĂ΂�܂��B
	// �R���X�g���N�^�łh�c�����łɍ~���Ă��܂�

	// �܂�ID��Map�ɂ��邩���ׂ邺�I
	ENTITY_ID id = NewEntity->GetID();
	// �G���e�B�e�B�������܂�
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	// �G���e�B�e�B���}�b�v�̃����o�[�ɂ������݂��Ă���
	MyAssert((ent == m_EntityMap.end()) , "��������ID�N�����o�^���Ƃ��񂯁I");


	// �v�f��ǉ�����
	m_EntityMap.insert(std::make_pair(NewEntity->GetID(), NewEntity));
	MyDebugString("ID�ԍ��@%d ���o�^����܂���\n", id);

	return true;
}

// �폜
bool EntityManager::Erace(BaseGameEntity* pEntity)
{
	// ���̊֐���BaseGameEntity�̃f�X�g���N�^�ŌĂ΂�܂��B
	// �R���X�g���N�^�łh�c�����łɍ~���Ă��܂�

	// �܂�ID��Map�ɂ��邩���ׂ邺�I
	ENTITY_ID id = pEntity->GetID();
	// �G���e�B�e�B�������܂�
	EntityMap::const_iterator ent = m_EntityMap.find(id);
	// �G���e�B�e�B���}�b�v�̃����o�[�ɂ������݂��Ă���
	MyAssert((ent != m_EntityMap.end()), "��������ID�̏ꏊ�ɗv�f�Ȃ������ŁI");


	// �v�f������
	m_EntityMap.erase(m_EntityMap.find(pEntity->GetID()));
	MyDebugString("ID�ԍ��@%d ���폜����܂���\n", id);

	return true;
}