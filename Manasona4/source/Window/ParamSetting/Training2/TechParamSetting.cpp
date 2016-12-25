#include "TechParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	�󂯐g�̐ݒ�
//+----------------------------

TechParamSetting::TechParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

TechParamSetting::~TechParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void TechParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void TechParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iEnemyTech != (int)ENEMY_TECH_TYPE::NO)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iEnemyTech != (int)ENEMY_TECH_TYPE::ALL)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E

	// ����͕����\�L�ŕ`��
	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((ENEMY_TECH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyTech)
	{
	case ENEMY_TECH_TYPE::NO:
		l_pString = "�s��Ȃ�";
		break;
	case ENEMY_TECH_TYPE::LAND:
		l_pString = "�n��̂ݍs��";
		break;
	case ENEMY_TECH_TYPE::AIR:
		l_pString = "�󒆂̂ݍs��";
		break;
	case ENEMY_TECH_TYPE::ALL:
		l_pString = "�s��";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void TechParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyTech = (PlayerDataMgr->m_TrainingData.iEnemyTech > (int)ENEMY_TECH_TYPE::NO)
			? PlayerDataMgr->m_TrainingData.iEnemyTech - 1 : (int)ENEMY_TECH_TYPE::ALL;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyTech = (PlayerDataMgr->m_TrainingData.iEnemyTech < (int)ENEMY_TECH_TYPE::ALL)
			? PlayerDataMgr->m_TrainingData.iEnemyTech + 1 : (int)ENEMY_TECH_TYPE::NO;
	}
}
