#include "HPRecoveryParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	Hp�̉񕜂̏�Ԑݒ�
//+----------------------------

HpRecoveryParamSetting::HpRecoveryParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

HpRecoveryParamSetting::~HpRecoveryParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void HpRecoveryParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void HpRecoveryParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iHpRecovery != (int)HP_RECOVERY_TYPE::DEFAULT)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iHpRecovery != (int)HP_RECOVERY_TYPE::AUTO_RECOVERY)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


	// ����͕����\�L�ŕ`��

	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((HP_RECOVERY_TYPE)PlayerDataMgr->m_TrainingData.iHpRecovery)
	{
	case HP_RECOVERY_TYPE::DEFAULT:
		l_pString = "�񕜂��Ȃ�";
		break;
	case HP_RECOVERY_TYPE::AUTO_RECOVERY:
		l_pString = "������";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void HpRecoveryParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iHpRecovery = (PlayerDataMgr->m_TrainingData.iHpRecovery > (int)HP_RECOVERY_TYPE::DEFAULT)
			? PlayerDataMgr->m_TrainingData.iHpRecovery  - 1 : (int)HP_RECOVERY_TYPE::AUTO_RECOVERY;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iHpRecovery = (PlayerDataMgr->m_TrainingData.iHpRecovery < (int)HP_RECOVERY_TYPE::AUTO_RECOVERY)
			? PlayerDataMgr->m_TrainingData.iHpRecovery + 1 : (int)HP_RECOVERY_TYPE::DEFAULT;
	}
}
