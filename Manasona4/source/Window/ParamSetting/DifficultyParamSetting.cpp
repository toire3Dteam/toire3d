#include "DifficultyParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	CPU��Փx�̃p�����[�^�[�ݒ�
//+----------------------------

DifficultyParamSetting::DifficultyParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

DifficultyParamSetting::~DifficultyParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void DifficultyParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void DifficultyParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_ConfigData.iVoiceVolume > 0)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_ConfigData.iVoiceVolume < 100)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


	// ����͕����\�L�ŕ`��

	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((AI_TYPE)PlayerDataMgr->m_ConfigData.iDifficultyAI)
	{
	case AI_TYPE::CPU_EASY:
		l_pString = "�C�[�W�[";
		break;
	case AI_TYPE::CPU_NORMAL:
		l_pString = "�m�[�}��";
		break;
	case AI_TYPE::CPU_HARD:
		l_pString = "�n�[�h";
		break;
	case AI_TYPE::CPU_YOKOE:
		l_pString = "���]����";
		break;
	default:
		break;
	}
	tdnFont::RenderString(l_pString, "HG�޼��E", 24, x + 32, y + 4, l_dCol, RS::COPY);


	//  ����́��\�L�ŕ`��

	//// %�̃T�C�Y
	//float fPersentSize = (float)(m_iWidthSize*0.725f);

	//// �ő�l�ƌ��݂̒l�������Ċ������擾
	//float fPersent = (float)(PlayerDataMgr->m_ConfigData.iVoiceVolume) / (float)(100);
	//float fAns = fPersentSize *fPersent;
	//// �Q�[�W�̍���
	//m_pPersentFrame->Render(x + 32 - 2, y, 2, 32, (selectFlag * 2) , 0, 2, 32);	
	//// ���g
	//for (int i = 0; i < (int)fPersentSize; i++)
	//{
	//	// ���g������Γh��Ԃ�
	//	if (i < (int)fAns)
	//	{
	//		m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 0, 1, 32);
	//	}
	//	else
	//	{
	//		m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 32, 1, 32);
	//	}
	//}
	//// �Q�[�W�̉E��
	//m_pPersentFrame->Render(x + 32+ (int)fPersentSize, y, 2, 32, (selectFlag * 2), 0, 2, 32);

	//// ���l
	//RenderNumber(x + m_iWidthSize - 24, y, PlayerDataMgr->m_ConfigData.iDifficultyAI, selectFlag);

}


//	����
void DifficultyParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iDifficultyAI = (PlayerDataMgr->m_ConfigData.iDifficultyAI > 0) ? PlayerDataMgr->m_ConfigData.iDifficultyAI - 1 : 3;	// �� 3�Ƃ���������0,1,2,3�Ƃ�������z�肵�����R�[�h
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iDifficultyAI = (PlayerDataMgr->m_ConfigData.iDifficultyAI < 3) ? PlayerDataMgr->m_ConfigData.iDifficultyAI + 1 : 0;	// �� 3�Ƃ���������0,1,2,3�Ƃ�������z�肵�����R�[�h
	}
}
