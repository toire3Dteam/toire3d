#include "AramitamaWassyoiParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	�G�̏�Ԃ̐ݒ�
//+----------------------------

AramitamaWassyoiParamSetting::AramitamaWassyoiParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

AramitamaWassyoiParamSetting::~AramitamaWassyoiParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void AramitamaWassyoiParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void AramitamaWassyoiParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iWassyoiGage != (int)WASSYOI_TYPE::NO)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iWassyoiGage != (int)WASSYOI_TYPE::MAX)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E

																							// ����͕����\�L�ŕ`��
																							// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((WASSYOI_TYPE)PlayerDataMgr->m_TrainingData.iWassyoiGage)
	{
	case WASSYOI_TYPE::NO:
		l_pString = "�f�t�H���g";
		break;
	case WASSYOI_TYPE::MAX:
		l_pString = "MAX�Œ�";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void AramitamaWassyoiParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iWassyoiGage = (PlayerDataMgr->m_TrainingData.iWassyoiGage > (int)WASSYOI_TYPE::NO)
			? PlayerDataMgr->m_TrainingData.iWassyoiGage - 1 : (int)WASSYOI_TYPE::MAX;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iWassyoiGage = (PlayerDataMgr->m_TrainingData.iWassyoiGage < (int)WASSYOI_TYPE::MAX)
			? PlayerDataMgr->m_TrainingData.iWassyoiGage + 1 : (int)WASSYOI_TYPE::NO;
	}
}
