#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// �L�����ŗL�̏��̐ݒ�
	m_maxSpeed = 1.25f;
	BasePlayer::LoadAttackFrameList("DATA/CHR/Airou/FrameList.txt");

	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
}

void Airou::InitActionDatas()
{
	// ���
	m_ActionDatas[(int)BASE_ACTION_STATE::ESCAPE].pAttackData = nullptr;	// ���̃f�[�^�͍U���ł͂Ȃ�


	// �ʏ�1�i
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->damage = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->LandFlyVector.Set(.25f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->AerialFlyVector.Set(.25f, 1.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->recoveryFlame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->SE_ID = "�ʏ�1�i";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->effectType = EFFECT_TYPE::DAMAGE;

	// �ʏ�2�i
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->damage = 240;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->LandFlyVector.Set(.35f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->AerialFlyVector.Set(.35f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->recoveryFlame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->SE_ID = "�ʏ�2�i";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->effectType = EFFECT_TYPE::DAMAGE;

	// �ʏ�3�i
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->LandFlyVector.Set(3.5f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->AerialFlyVector.Set(3.5f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->hitStopFlame = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->recoveryFlame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->SE_ID = "�ʏ�3�i";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->effectType = EFFECT_TYPE::DAMAGE;

	// ���i�U��
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->LandFlyVector.Set(1.0f, 0.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->AerialFlyVector.Set(.5f, 2.2f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->recoveryFlame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->SE_ID = "�ʏ�3�i";
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->effectType = EFFECT_TYPE::DAMAGE;

	// �󒆍U��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->LandFlyVector.Set(1.0f, 0.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->AerialFlyVector.Set(.75f, 2.2f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->recoveryFlame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->SE_ID = "�ʏ�3�i";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->effectType = EFFECT_TYPE::DAMAGE;

	// �󒆉��U��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->LandFlyVector.Set(1.0f, 0.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->AerialFlyVector.Set(1.5f, -2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->recoveryFlame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->SE_ID = "�ʏ�3�i";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->effectType = EFFECT_TYPE::DAMAGE;


	// �S����
	FOR((int)BASE_ACTION_STATE::END)
	{
		if (m_ActionDatas[i].isAttackData())
		{
			MyAssert((m_ActionDatas[i].pAttackData->LandFlyVector.x >= 0), "������уx�N�g��-�ɂ���ȁ[");
			m_ActionDatas[i].pAttackData->bHit = false;
		}
	}
}