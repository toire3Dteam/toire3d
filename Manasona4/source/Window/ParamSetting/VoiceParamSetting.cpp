#include "VoiceParamSetting.h"
#include "Data\PlayerData.h"
#include "../../Sound/SoundManager.h"

//+----------------------------
//	Voice�̃p�����[�^�[�ݒ�
//+----------------------------

VoiceParamSetting::VoiceParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

VoiceParamSetting::~VoiceParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void VoiceParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void VoiceParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_ConfigData.iVoiceVolume > 0)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_ConfigData.iVoiceVolume < 100)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E

	//  ����́��\�L�ŕ`��
	
	// %�̃T�C�Y
	float fPersentSize = (float)(m_iWidthSize*0.725f);

	// �ő�l�ƌ��݂̒l�������Ċ������擾
	float fPersent = (float)(PlayerDataMgr->m_ConfigData.iVoiceVolume) / (float)(100);
	float fAns = fPersentSize *fPersent;


	// �Q�[�W�̍���
	m_pPersentFrame->Render(x + 32 - 2, y, 2, 32, (selectFlag * 2) , 0, 2, 32);	
	// ���g
	for (int i = 0; i < (int)fPersentSize; i++)
	{
		// ���g������Γh��Ԃ�
		if (i < (int)fAns)
		{
			m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 0, 1, 32);
		}
		else
		{
			m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 32, 1, 32);
		}
	}
	// �Q�[�W�̉E��
	m_pPersentFrame->Render(x + 32+ (int)fPersentSize, y, 2, 32, (selectFlag * 2), 0, 2, 32);

	// ���l
	RenderNumber(x + m_iWidthSize - 24, y, PlayerDataMgr->m_ConfigData.iVoiceVolume, selectFlag);

}


//	����
void VoiceParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iVoiceVolume--;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iVoiceVolume++;
	}

	if (m_bRightPush || m_bLeftPush)
	{
		// 100�𒴂�����0�� 0��艺������100��
		if (PlayerDataMgr->m_ConfigData.iVoiceVolume > 100)PlayerDataMgr->m_ConfigData.iVoiceVolume = 0;
		if (PlayerDataMgr->m_ConfigData.iVoiceVolume < 0)PlayerDataMgr->m_ConfigData.iVoiceVolume = 100;

		// ���ʐݒ�
		voice->SetBaseVolume(PlayerDataMgr->m_ConfigData.iVoiceVolume * 0.01f);
	}

	// ��100�܂���0�܂ŗ������x���S�Ɏ~�߂āA������x�������ނ܂Ńp�����[�^��ύX�ł��Ȃ�
	if (PlayerDataMgr->m_ConfigData.iVoiceVolume == 100||
		PlayerDataMgr->m_ConfigData.iVoiceVolume == 0)
	{
		m_iLeftPushFrame = 0;
		m_iRightPushFrame = 0;
		m_iSpeedLv = 0;
	}

	// 0~100�̊ԂɎ~�߂�
	//PlayerDataMgr->m_ConfigData.iVoiceVolume = min(100, max(0, PlayerDataMgr->m_ConfigData.iVoiceVolume));

	// �e�X�g���񂩂��`���[�n��
	//if (!voice->isPlay(VOICE_TYPE::DOWN_ATTACK, CHARACTER::ANIKI)) voice->Play(VOICE_TYPE::DOWN_ATTACK, CHARACTER::ANIKI);
}
