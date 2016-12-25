#include "EnemyGuardSwitchParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	�K�[�h�̐؂�ւ��ݒ�
//+----------------------------

EnemyGuardSwitchParamSetting::EnemyGuardSwitchParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

EnemyGuardSwitchParamSetting::~EnemyGuardSwitchParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void EnemyGuardSwitchParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void EnemyGuardSwitchParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch != (int)ENEMY_GUARD_SWITCH_TYPE::NO)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch != (int)ENEMY_GUARD_SWITCH_TYPE::OK)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


																							// ����͕����\�L�ŕ`��

																							// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((ENEMY_GUARD_SWITCH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch)
	{
	case ENEMY_GUARD_SWITCH_TYPE::NO:
		l_pString = "�s��Ȃ�";
		break;
	case ENEMY_GUARD_SWITCH_TYPE::OK:
		l_pString = "�s��";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void EnemyGuardSwitchParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch = (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch > (int)ENEMY_GUARD_SWITCH_TYPE::NO)
			? PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch - 1 : (int)ENEMY_GUARD_SWITCH_TYPE::OK;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch = (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch < (int)ENEMY_GUARD_SWITCH_TYPE::OK)
			? PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch + 1 : (int)ENEMY_GUARD_SWITCH_TYPE::NO;
	}
}
