#include "Hp2pParamSetting.h"
#include "Data\PlayerData.h"

//+--------------------------------
//	2p��HP�ő�l�̃p�����[�^�[�ݒ�
//+--------------------------------

Hp2pParamSetting::Hp2pParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

Hp2pParamSetting::~Hp2pParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void Hp2pParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void Hp2pParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaHp2pParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (PlayerDataMgr->m_TrainingData.iHp2P > 1)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (PlayerDataMgr->m_TrainingData.iHp2P < 100)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E

	//  ����́��\�L�ŕ`��
	
	// %�̃T�C�Y
	float fPersentSize = (float)(m_iWidthSize*0.725f);

	// �ő�l�ƌ��݂̒l�������Ċ������擾
	float fPersent = (float)(PlayerDataMgr->m_TrainingData.iHp2P) / (float)(100);
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
	RenderNumber(x + m_iWidthSize - 24, y, PlayerDataMgr->m_TrainingData.iHp2P, selectFlag);

}


//	����
void Hp2pParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iHp2P--;
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iHp2P++;
	}

	// ��100�܂���0�܂ŗ������x���S�Ɏ~�߂āA������x�������ނ܂Ńp�����[�^��ύX�ł��Ȃ�
	if (PlayerDataMgr->m_TrainingData.iHp2P == 100||
		PlayerDataMgr->m_TrainingData.iHp2P == 1)
	{
		m_iLeftPushFrame = 0;
		m_iRightPushFrame = 0;
		m_iSpeedLv = 0;
	}


	// 100�𒴂�����1�� 1��艺������100��
	if (PlayerDataMgr->m_TrainingData.iHp2P > 100)PlayerDataMgr->m_TrainingData.iHp2P = 1;
	if (PlayerDataMgr->m_TrainingData.iHp2P < 1)PlayerDataMgr->m_TrainingData.iHp2P = 100;
}
