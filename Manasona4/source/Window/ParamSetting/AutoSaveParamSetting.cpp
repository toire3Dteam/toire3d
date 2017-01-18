#include "AutoSaveParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	�I�[�g�Z�[�u�̃p�����[�^�[�ݒ�
//+----------------------------

AutoSaveParamSetting::AutoSaveParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

AutoSaveParamSetting::~AutoSaveParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void AutoSaveParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void AutoSaveParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_ConfigData.iAutoSave != (int)AUTO_SAVE_TYPE::YES)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_ConfigData.iAutoSave != (int)AUTO_SAVE_TYPE::NO)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


	// ����͕����\�L�ŕ`��

	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "�Ȃ�";

	switch ((AUTO_SAVE_TYPE)PlayerDataMgr->m_ConfigData.iAutoSave)
	{
	case AUTO_SAVE_TYPE::YES:
		l_pString = "�����ōs��";
		break;
	case AUTO_SAVE_TYPE::NO:
		l_pString = "�����ōs��Ȃ�";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);


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
	//RenderNumber(x + m_iWidthSize - 24, y, PlayerDataMgr->m_ConfigData.iAutoSave, selectFlag);

}


//	����
void AutoSaveParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iAutoSave = (PlayerDataMgr->m_ConfigData.iAutoSave > (int)AUTO_SAVE_TYPE::YES) ? PlayerDataMgr->m_ConfigData.iAutoSave - 1 : (int)AUTO_SAVE_TYPE::NO;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iAutoSave = (PlayerDataMgr->m_ConfigData.iAutoSave < (int)AUTO_SAVE_TYPE::NO) ? PlayerDataMgr->m_ConfigData.iAutoSave + 1 : (int)AUTO_SAVE_TYPE::YES;
	}
}
