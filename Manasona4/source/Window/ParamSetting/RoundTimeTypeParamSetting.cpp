#include "RoundTimeTypeParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	���E���h���Ԃ̃p�����[�^�[�ݒ�
//+----------------------------

RoundTimeTypeParamSetting::RoundTimeTypeParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

RoundTimeTypeParamSetting::~RoundTimeTypeParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void RoundTimeTypeParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void RoundTimeTypeParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_ConfigData.iRoundTimeType != (int)ROUND_TIME_TYPE::SEC_30)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_ConfigData.iRoundTimeType != (int)ROUND_TIME_TYPE::SEC_INFINITY)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


	// ����͕����\�L�ŕ`��

	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((ROUND_TIME_TYPE)PlayerDataMgr->m_ConfigData.iRoundTimeType)
	{
	case ROUND_TIME_TYPE::SEC_30:
		l_pString = "30�b";
		break;
	case ROUND_TIME_TYPE::SEC_45:
		l_pString = "45�b";
		break;
	case ROUND_TIME_TYPE::SEC_60:
		l_pString = "60�b";
		break;
	case ROUND_TIME_TYPE::SEC_75:
		l_pString = "75�b";
		break;
	case ROUND_TIME_TYPE::SEC_99:
		l_pString = "99�b";
		break;
	case ROUND_TIME_TYPE::SEC_INFINITY:
		l_pString = "������";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void RoundTimeTypeParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundTimeType = (PlayerDataMgr->m_ConfigData.iRoundTimeType > (int)ROUND_TIME_TYPE::SEC_30)
			? PlayerDataMgr->m_ConfigData.iRoundTimeType - 1 : 3;	
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundTimeType = (PlayerDataMgr->m_ConfigData.iRoundTimeType < (int)ROUND_TIME_TYPE::SEC_INFINITY)
			? PlayerDataMgr->m_ConfigData.iRoundTimeType + 1 : 0;	
	}
}
