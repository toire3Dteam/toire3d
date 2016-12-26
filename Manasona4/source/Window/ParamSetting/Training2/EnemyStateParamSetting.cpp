#include "EnemyStateParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	�G�̏�Ԃ̐ݒ�
//+----------------------------

EnemyStateParamSetting::EnemyStateParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

EnemyStateParamSetting::~EnemyStateParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void EnemyStateParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void EnemyStateParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iEnemyState != (int)ENEMY_STATE_TYPE::STAND)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iEnemyState != (int)ENEMY_STATE_TYPE::COM)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E

																							// ����͕����\�L�ŕ`��
																							// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((ENEMY_STATE_TYPE)PlayerDataMgr->m_TrainingData.iEnemyState)
	{
	case ENEMY_STATE_TYPE::STAND:
		l_pString = "����";
		break;
	case ENEMY_STATE_TYPE::SQUAT:
		l_pString = "���Ⴊ��";
		break;
	case ENEMY_STATE_TYPE::JUMP:
		l_pString = "�W�����v";
		break;
	case ENEMY_STATE_TYPE::ATTACK:
		l_pString = "�U����������";
		break;
	case ENEMY_STATE_TYPE::COM:
		l_pString = "COM";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void EnemyStateParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyState = (PlayerDataMgr->m_TrainingData.iEnemyState > (int)ENEMY_STATE_TYPE::STAND)
			? PlayerDataMgr->m_TrainingData.iEnemyState - 1 : (int)ENEMY_STATE_TYPE::COM;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyState = (PlayerDataMgr->m_TrainingData.iEnemyState < (int)ENEMY_STATE_TYPE::COM)
			? PlayerDataMgr->m_TrainingData.iEnemyState + 1 : (int)ENEMY_STATE_TYPE::STAND;
	}
}
