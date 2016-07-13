#pragma once

/*
	�p���p
	�������z�֐���
*/

//class Miner;	// �S�v

struct Message;// 

// State���N���X���L�����N�^���ɍė��p�\��
// �N���X�e���v���[�g�ɂ���Ɛl�����y�ɂȂ�B
template <class entity_type>
class State
{
public:
	virtual ~State() {};

	// ����
	virtual void Enter(entity_type*) = 0;

	// ���s���܂�
	virtual void Execute(entity_type*) = 0;

	// �A��
	virtual void Exit(entity_type*) = 0;

	
	// [�ǉ�]�`��
	virtual void Render(entity_type*){}

	// �摜�Ƃ��̕`�悪�K�v�ɂȂ����̂ŁA2D�̕`������܂���
	virtual void Render2D(entity_type*){}

	/*
		BaseGameEntity�̃X�e�[�g�����b�Z�[�W���󂯎��
		������I���ł���悤�ɏC������K�v������
	*/
	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(entity_type*, const Message&) = 0;

};
