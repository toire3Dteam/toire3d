#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// �L�����ŗL�̏��̐ݒ�
	m_maxSpeed = 1.25f;
	BasePlayer::LoadAttackFrameList("DATA/CHR/Airou/FrameList.txt");

	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
}

void Airou::InitAttackDatas()
{
	// �ʏ�1�i
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].bBeInvincible = false;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].damage = 120;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].FlyVector.Set(.25f, .5f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].hitStopFlame = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].recoveryFlame = 40;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH1].SE_ID = "�ʏ�1�i";

	// �ʏ�2�i
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].bBeInvincible = false;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].damage = 240;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].FlyVector.Set(.35f, .75f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].hitStopFlame = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].recoveryFlame = 40;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH2].SE_ID = "�ʏ�2�i";

	// �ʏ�3�i
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].bBeInvincible = true;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].damage = 360;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].FlyVector.Set(3.5f, 2.0f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].hitStopFlame = 6;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].recoveryFlame = 90;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::RUSH3].SE_ID = "�ʏ�3�i";

	// ���i�U��
	m_AttackDatas[(int)BASE_ATTACK_STATE::SQUAT].bBeInvincible = false;
	m_AttackDatas[(int)BASE_ATTACK_STATE::SQUAT].damage = 360;
	m_AttackDatas[(int)BASE_ATTACK_STATE::SQUAT].FlyVector.Set(1.0f, 2.5f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::SQUAT].hitStopFlame = 4;
	m_AttackDatas[(int)BASE_ATTACK_STATE::SQUAT].recoveryFlame = 60;
	m_AttackDatas[(int)BASE_ATTACK_STATE::SQUAT].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::SQUAT].SE_ID = "�ʏ�3�i";

	// �󒆍U��
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIAL].bBeInvincible = false;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIAL].damage = 360;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIAL].FlyVector.Set(1.0f, 2.2f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIAL].hitStopFlame = 4;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIAL].recoveryFlame = 60;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIAL].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIAL].SE_ID = "�ʏ�3�i";

	// �󒆉��U��
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIALDROP].bBeInvincible = false;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIALDROP].damage = 360;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIALDROP].FlyVector.Set(1.0f, -2.5f);
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIALDROP].hitStopFlame = 6;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIALDROP].recoveryFlame = 60;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIALDROP].pierceLV = 0;
	m_AttackDatas[(int)BASE_ATTACK_STATE::AERIALDROP].SE_ID = "�ʏ�3�i";

	// �S����
	FOR((int)BASE_ATTACK_STATE::END)
	{
		MyAssert((m_AttackDatas[i].FlyVector.x >= 0), "������уx�N�g��-�ɂ���ȁ[");
		m_AttackDatas[i].bHit = false;
	}
}