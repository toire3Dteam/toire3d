#include "BaseGameEntity.h"
#include <cassert>
#include "EntityManager.h"

// �錾
// int BaseGameEntity::m_isNextValidID = 0;

//void BaseGameEntity::SetID(int val)
//{
//	// �Q�[��ID�͂ƂĂ��d�v�A����������ID������Ă��Ȃ���
//	// ���̊֐��Ńe�X�g����
//	//assert((val >= m_isNextValidID) && "SetID");
//		
//	m_ID = val;
//	m_isNextValidID = m_ID + 1;	// �C���N�������g
//
//}

/*
	������������
	�}�l�[�W���[�̎����̃f�[�^�������Ă����
	��Γ񒹃��[�h
*/

// ���̂h�c������Ă��Ȃ����̏����́@EntityMGR�@�ōs�����Ƃɂ��܂����B
BaseGameEntity::BaseGameEntity(ENTITY_ID id) :m_ID(id)
{
	// �}�l�[�W���[�N��
	MyAssert(EntityMgr->Register(this), "ID�o�^�Ɏ��s���܂��� \n ID�ԍ�= %d ", m_ID);
}

// �폜�����Ƃ����̎����̗v�f���@EntityMGR �������
BaseGameEntity::~BaseGameEntity()
{
	// �}�l�[�W���[�N��
	MyAssert(EntityMgr->Erace(this), "ID�o�^�Ɏ��s���܂��� \n ID�ԍ�= %d ", m_ID);

}