
#include "AI\NPC\PersonManager.h"

//�@�ЂƂ̎��
#include "AI\NPC\BasePerson\BasePerson.h"
#include "../../Sound/SoundManager.h"
#include "FieldPlayer\FieldPlayer.h"
#include "SoccerBall\SoccerBall.h"


// �錾
PersonManager* PersonManager::pInstance = nullptr;

int PersonManager::m_IDcount = 0;

/***********/
//	���̎擾
/***********/
PersonManager & PersonManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new PersonManager();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *pInstance;
}

/***********/
//	���
/***********/
void PersonManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// ������
PersonManager::PersonManager():
	BaseGameEntity(ENTITY_ID::PERSON_MNG)// ���l�̃}�l�[�W���[�p�̂h�c�ԍ���n��
{
	m_IDcount = 0;
}


// ���
PersonManager::~PersonManager()
{
	// �S��������
	for (auto it : m_PersonData)
	{
		SAFE_DELETE(it);
	}

	//�@�f�[�^�����
	m_PersonData.clear();
}

// �L�����ǉ�
ENTITY_ID PersonManager::AddPerson(Vector3 pos, bool isAi)
{
	// �l
	BasePerson* data;

	ENTITY_ID id = (ENTITY_ID)(GAME_ID::id_person_first + m_IDcount);

	data = new FieldPlayer(id, isAi);

	// �����Őݒ��ύX
	data->SetPos(pos);

	// �z��ɉ�����
	m_PersonData.push_back(data);

	++m_IDcount;// ���̐l�̔ԍ���

	// �ݒu�����l��ID��Ԃ�
	return id;
}

void PersonManager::Reset()
{
	// �J�E���g���~�߂�
	m_IDcount = 0;
}

void PersonManager::Update()
{
	if (m_PersonData.empty())return;// �󂾂����烊�^�[��

	// �l�����X�V
	for (auto it : m_PersonData)
	{
		it->Update();
	}
}

void PersonManager::Render()
{
	if (m_PersonData.empty())return;// �󂾂����烊�^�[��


								   // �l�����`��
	for (auto it : m_PersonData)
	{

		it->RangeRender();

		it->Render();


	}
	// �l�����̂t�h�`��
	for (auto it : m_PersonData)
	{

		it->UIRender();
	}
}

// ���b�Z�[�W
bool  PersonManager::HandleMessage(const Message& msg)
{
	// �܂�����MGR�ɃX�e�[�g�}�V���͂������ɂ������ȁE�E�E
	// �Ƃ������Ƃł����Œ��Ƀ��b�Z�[�W�ɑ΂��鏈����

	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TARGET_PLAYERS:
	{
		// �I���W�i���\���̂̃|�C���^�\�^�Ŏ󂯎�邺�I
		PLAYER_INFO* exInfo = (PLAYER_INFO*)msg.ExtraInfo;

		// �߂��v���C���[������
		//Vector3 nearPos = NearPlayerPosSearch(exInfo);
		BasePerson* nearPlayer = NearPlayerSearch(exInfo);

		//int a = 114;
		//int* pa = &a;

		// ���M 
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			exInfo->id,
			SEND_PLAYER_POS,
			(void*)nearPlayer	// [�ǋL���]�@���|�C���^�\��n���Ƃ��́��𔲂�
			);

		return true;// [��肭���b�Z�[�W���͂���!]
		break;
	}
	case MESSAGE_TYPE::SOCCER_BALL_NEAR_PLAYER:
	{
		// �I���W�i���\���̂̃|�C���^�\�^�Ŏ󂯎�邺�I
		PLAYER_INFO* exInfo = (PLAYER_INFO*)msg.ExtraInfo;

		// �߂��v���C���[������
		ENTITY_ID nearID = SoccerNearPlayerSearch(exInfo);

		// ���M 
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			exInfo->id,
			SEND_NEAR_PLAYER,
			(void*)&nearID	// [�ǋL���]�@���|�C���^�\��n���Ƃ��́��𔲂�
			);
	}
		break;
	default:
		MyAssert(0, "����Ȗ��߂͎󂯕t���ĂȂ��ł�");
		break;
	}


	return false;// [��肭���b�Z�[�W���͂��Ȃ�����]
}

BasePerson * PersonManager::GetPerson(int no)
{
	if (m_PersonData.empty())
	{
		MyAssert(0, "�f�[�^����");
	}

	// �G���[����
	if (no >= (int)m_PersonData.size())
	{
		MyAssert(0, "�w�肵���ꏊ�Ƀf�[�^���Ȃ�");
	}

	int i(0);
	m_it = m_PersonData.begin();
	while (i != no)
	{
		m_it++;
		i++;
	}

	return (*m_it);

}

// �߂��L�����N�^�[��T��
Vector3 PersonManager::NearPlayerPosSearch(PLAYER_INFO* info)
{
	float nearPlayer = FLT_MAX;
	Vector3 nearPos = VECTOR_ZERO;
	// �l����
	for (auto it : m_PersonData)
	{
		//if (it->GetPos())
		if (it->GetID() == info->id)continue;
		
		float len = Math::Length(it->GetPos(), info->pos);
		if (len <= nearPlayer)
		{
			nearPlayer = len;
			nearPos = it->GetPos();
		}


	}

	return  nearPos;

}

BasePerson* PersonManager::NearPlayerSearch(PLAYER_INFO* info)
{
	float nearPlayerlen = FLT_MAX;
	BasePerson* nearPlayer = NULL;
	BasePerson* myPlayer;
	// �l����
	for (auto it : m_PersonData)
	{
		//if (it->GetPos())
		if (it->GetID() == info->id)
		{
			myPlayer = it;
			continue;
		}
		if (it->m_LifePoint >= 3)continue;// ����ł��͂Ȃ�

		float len = Math::Length(it->GetPos(), info->pos);
		if (len <= nearPlayerlen)
		{
			nearPlayerlen = len;
			nearPlayer = it;
		}

	}

	if (nearPlayer==NULL)
	{
		//MyAssert(0, "�N�����Ȃ��Ȃ���");
		return myPlayer;
	}
	return nearPlayer;
}

// �T�b�J�[�{�[����
ENTITY_ID PersonManager::SoccerNearPlayerSearch(PLAYER_INFO * info)
{
	float nearPlayer = FLT_MAX;
	ENTITY_ID nearID;
	// �l����
	for (auto it : m_PersonData)
	{
		if (it->m_LifePoint >= 3)continue;// ����ł��͂Ȃ�

		float len = Math::Length(it->GetPos(), SOCCER_BALL.GetPos());
		if (len <= nearPlayer)
		{
			nearPlayer = len;
			nearID = it->GetID();
		}

	}

	return nearID;
}
