#include "PartnerRecoveryParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	�p�[�g�i�[�Q�[�W�̐ݒ�
//+----------------------------

PartnerRecoveryParamSetting::PartnerRecoveryParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

PartnerRecoveryParamSetting::~PartnerRecoveryParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void PartnerRecoveryParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void PartnerRecoveryParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iPartnerRecovery != (int)PARTNER_RECOVERY_TYPE::DEFAULT)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iPartnerRecovery != (int)PARTNER_RECOVERY_TYPE::MAX)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E

																							// ����͕����\�L�ŕ`��
																							// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((PARTNER_RECOVERY_TYPE)PlayerDataMgr->m_TrainingData.iPartnerRecovery)
	{
	case PARTNER_RECOVERY_TYPE::DEFAULT:
		l_pString = "�ʏ�";
		break;
	case PARTNER_RECOVERY_TYPE::MAX:
		l_pString = "������";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void PartnerRecoveryParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iPartnerRecovery = (PlayerDataMgr->m_TrainingData.iPartnerRecovery > (int)PARTNER_RECOVERY_TYPE::DEFAULT)
			? PlayerDataMgr->m_TrainingData.iPartnerRecovery - 1 : (int)PARTNER_RECOVERY_TYPE::MAX;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iPartnerRecovery = (PlayerDataMgr->m_TrainingData.iPartnerRecovery < (int)PARTNER_RECOVERY_TYPE::MAX)
			? PlayerDataMgr->m_TrainingData.iPartnerRecovery + 1 : (int)PARTNER_RECOVERY_TYPE::DEFAULT;
	}
}
