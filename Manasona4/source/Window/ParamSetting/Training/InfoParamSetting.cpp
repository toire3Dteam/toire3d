#include "InfoParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	�g���[�j���O�̏��ݒ�
//+----------------------------

InfoParamSetting::InfoParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

InfoParamSetting::~InfoParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void InfoParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void InfoParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iInfo != (int)TRAINING_INFO_TYPE::NONE)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iInfo != (int)TRAINING_INFO_TYPE::ALL)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E

	// ����͕����\�L�ŕ`��
	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((TRAINING_INFO_TYPE)PlayerDataMgr->m_TrainingData.iInfo)
	{
	case TRAINING_INFO_TYPE::NONE:
		l_pString = "�\�����Ȃ�";
		break;
	case TRAINING_INFO_TYPE::DAMEGE:
		l_pString = "�_���[�W�\��";
		break;
	case TRAINING_INFO_TYPE::COMMAND:
		l_pString = "�R�}���h����";
		break;
	case TRAINING_INFO_TYPE::ALL:
		l_pString = "����";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void InfoParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iInfo = (PlayerDataMgr->m_TrainingData.iInfo > (int)TRAINING_INFO_TYPE::NONE)
			? PlayerDataMgr->m_TrainingData.iInfo - 1 : (int)TRAINING_INFO_TYPE::ALL;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iInfo = (PlayerDataMgr->m_TrainingData.iInfo < (int)TRAINING_INFO_TYPE::ALL)
			? PlayerDataMgr->m_TrainingData.iInfo + 1 : (int)TRAINING_INFO_TYPE::NONE;
	}
}
