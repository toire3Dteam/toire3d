#include "../BasePlayer.h"
#include "Airou.h"

Airou::Airou(int deviceID, bool bAI) :BasePlayer(deviceID, bAI)
{
	// �L�����ŗL�̏��̐ݒ�
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);
	m_maxSpeed = 1.25f;
	BasePlayer::LoadAttackFrameList("DATA/CHR/Airou/FrameList.txt");

	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
}

void Airou::InitActionDatas()
{
	// ���
	m_ActionDatas[(int)BASE_ACTION_STATE::ESCAPE].pAttackData = nullptr;	// ���̃f�[�^�͍U���ł͂Ȃ�

	//==============================================================================================================
	//	�ʏ�1�i
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->HitScore = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->damage = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->LandFlyVector.Set(.25f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->AerialFlyVector.Set(.25f, 1.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->recoveryFlame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->HitSE = "�ʏ�1�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->effectType = EFFECT_TYPE::DAMAGE;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->height = 6.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->pos.Set(9, 4.5f, 0);

	//==============================================================================================================
	//	�ʏ�2�i
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->HitScore = 240;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->damage = 240;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->LandFlyVector.Set(.35f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->AerialFlyVector.Set(.35f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->recoveryFlame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->HitSE = "�ʏ�2�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->effectType = EFFECT_TYPE::DAMAGE;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->pos.Set(9, 5.5, 0);

	//==============================================================================================================
	//	�ʏ�3�i
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->LandFlyVector.Set(3.5f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->AerialFlyVector.Set(3.5f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->hitStopFlame = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->recoveryFlame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->HitSE = "�ʏ�3�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->WhiffSE = "��U��2";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->effectType = EFFECT_TYPE::DAMAGE;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pCollisionShape->width = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pCollisionShape->height = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pCollisionShape->pos.Set(0, 5, 0);

	//==============================================================================================================
	//	���i�U��
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->LandFlyVector.Set(1.0f, 0.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->AerialFlyVector.Set(.5f, 2.1f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->recoveryFlame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->HitSE = "�ʏ�3�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->effectType = EFFECT_TYPE::DAMAGE;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pCollisionShape->height = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	�󒆍U��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->LandFlyVector.Set(1.0f, 0.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->AerialFlyVector.Set(.7f, 2.1f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->recoveryFlame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->HitSE = "�󒆍U���q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->effectType = EFFECT_TYPE::DAMAGE;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->height = 9;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->pos.Set(6.5f, 4, 0);

	//==============================================================================================================
	//	�󒆉��U��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->LandFlyVector.Set(1.0f, 0.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->AerialFlyVector.Set(1.5f, -2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->recoveryFlame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->HitSE = "�󒆉��U���q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->WhiffSE = "��U��2";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->effectType = EFFECT_TYPE::DAMAGE;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->width = 12;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->pos.Set(5, -3.0f, 0);

	//==============================================================================================================
	//	�S����
	FOR((int)BASE_ACTION_STATE::END)
	{
		if (m_ActionDatas[i].isAttackData())
		{
			MyAssert((m_ActionDatas[i].pAttackData->LandFlyVector.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
			MyAssert((m_ActionDatas[i].pAttackData->pCollisionShape->pos.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
			m_ActionDatas[i].pAttackData->bHit = false;
			// ���蔭�������u�Ԃɋ�U��SE���Đ����Ă݂�
			for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_ActionDatas[i].pAttackData->WhiffDelayFrame = frame; break; }
		}
	}
}