#include "RoundNumTypeParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	���E���h���̃p�����[�^�[�ݒ�
//+----------------------------

RoundNumTypeParamSetting::RoundNumTypeParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

RoundNumTypeParamSetting::~RoundNumTypeParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void RoundNumTypeParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void RoundNumTypeParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_ConfigData.iRoundNumType != (int)ROUND_NUM_TYPE::ROUND_1)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_ConfigData.iRoundNumType != (int)ROUND_NUM_TYPE::ROUND_4)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


	// ����͕����\�L�ŕ`��

	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((ROUND_NUM_TYPE)PlayerDataMgr->m_ConfigData.iRoundNumType)
	{
	case ROUND_NUM_TYPE::ROUND_1:
		l_pString = "1���E���h���";
		break;
	case ROUND_NUM_TYPE::ROUND_2:
		l_pString = "2���E���h���";
		break;
	case ROUND_NUM_TYPE::ROUND_3:
		l_pString = "3���E���h���";
		break;
	case ROUND_NUM_TYPE::ROUND_4:
		l_pString = "4���E���h���";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void RoundNumTypeParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundNumType = (PlayerDataMgr->m_ConfigData.iRoundNumType > (int)ROUND_NUM_TYPE::ROUND_1) ? PlayerDataMgr->m_ConfigData.iRoundNumType - 1 : 3;	// �� 3�Ƃ���������0,1,2,3�Ƃ�������z�肵�����R�[�h
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundNumType = (PlayerDataMgr->m_ConfigData.iRoundNumType < (int)ROUND_NUM_TYPE::ROUND_4) ? PlayerDataMgr->m_ConfigData.iRoundNumType + 1 : 0;	// �� 3�Ƃ���������0,1,2,3�Ƃ�������z�肵�����R�[�h
	}
}
