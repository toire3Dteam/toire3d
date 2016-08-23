#include "../BasePlayer.h"
#include "Airou.h"

Airou::Airou(int deviceID, TEAM team, bool bAI) :BasePlayer(deviceID, team, bAI)
{
	// �L�����ŗL�̏��̐ݒ�
	m_pHitSquare->width = 1.5f;
	m_pHitSquare->height = 4;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);
	m_maxSpeed = 1.25f;
	BasePlayer::LoadAttackFrameList("DATA/CHR/Airou/FrameList.txt");

	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	//switch (deviceID)
	//{
	//case 1:
	//	m_pObj->SetTexture(tdnTexture::Load("DATA/CHR/Airou/tex_airou2.png"), 0);
	//	break;
	//case 2:
	//	m_pObj->SetTexture(tdnTexture::Load("DATA/CHR/Airou/tex_airou3.png"), 0);
	//	break;
	//case 3:
	//	m_pObj->SetTexture(tdnTexture::Load("DATA/CHR/Airou/tex_airou4.png"), 0);
	//	break;
	//}

	if (TEAM::B == m_team)
	{
		m_pObj->SetTexture(tdnTexture::Load("DATA/CHR/Airou/tex_airou2.png"), 0);
	}
	//else
	//{
	//	m_pObj->SetTexture(tdnTexture::Load("DATA/CHR/Airou/tex_airou2.png"), 0);
	//}

}

void Airou::InitActionDatas()
{
	m_ActionDatas[(int)BASE_ACTION_STATE::ESCAPE].pAttackData = nullptr;	// ���̃f�[�^�͍U���ł͂Ȃ�
	m_ActionDatas[(int)BASE_ACTION_STATE::STAND].pAttackData = nullptr;	// ���̃f�[�^�͍U���ł͂Ȃ�

	/********************************************************************/
	// (��)�A�N�V�����A�^�b�N�f�[�^�ς�����@�X�^���h�̕����ς������
	/********************************************************************/

	//==============================================================================================================
	//	�ʏ�1�i
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->HitScore = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->damage = 120;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->HitSE = "�ʏ�1�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->ShakeCameraInfo.Set(0, 0);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.25f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.25f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 18;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->height = 6.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH1].pAttackData->pCollisionShape->pos.Set(9, 4.5f, 0);

	//==============================================================================================================
	//	�ʏ�2�i
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->HitScore = 240;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->damage = 240;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->HitSE = "�ʏ�2�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->ShakeCameraInfo.Set(0, 0);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.35f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.35f, .75f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 20;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 20;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH2].pAttackData->pCollisionShape->pos.Set(9, 5.5, 0);

	//==============================================================================================================
	//	�ʏ�3�i
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->HitSE = "�ʏ�3�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->WhiffSE = "��U��2";
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->ShakeCameraInfo.Set(.2f, 3);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(2.5f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(2.5f, 1.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 40;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pCollisionShape->width = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pCollisionShape->height = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::RUSH3].pAttackData->pCollisionShape->pos.Set(0, 5, 0);

	//==============================================================================================================
	//	���i�U��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->HitSE = "�ʏ�3�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->bAntiAir = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->ShakeCameraInfo.Set(.1f, 2);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 30;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 50;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pCollisionShape->width = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pCollisionShape->height = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::SQUAT].pAttackData->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	�󒆍U��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->HitSE = "�󒆍U���q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->WhiffSE = "��U��1";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->ShakeCameraInfo.Set(.1f, 3);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.7f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 4;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 38;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 55;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->height = 9;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIAL].pAttackData->pCollisionShape->pos.Set(6.5f, 5, 0);

	//==============================================================================================================
	//	�󒆉��U��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->HitSE = "�󒆉��U���q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->WhiffSE = "��U��2";
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->ShakeCameraInfo.Set(.3f, 3);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, -2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 38;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 50;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->width = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->height = 8;
	m_ActionDatas[(int)BASE_ACTION_STATE::AERIALDROP].pAttackData->pCollisionShape->pos.Set(5, -3.0f, 0);

	//==============================================================================================================
	//	�t�B�j�b�V���A�[�c�U��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->HitScore = 114;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->damage = 514;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->HitSE = "�t�B�j�b�V����";
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->WhiffSE = "��U��2";
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->HitEffectType = EFFECT_TYPE::FINISH_HIT;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->ShakeCameraInfo.Set(1.0f, 20);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(5.0f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(5.5f, 2.5f);
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 25;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 60;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->pCollisionShape->width = 11;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->pCollisionShape->height = 6.5f;
	m_ActionDatas[(int)BASE_ACTION_STATE::FINISH].pAttackData->pCollisionShape->pos.Set(9, 4.5f, 0);

	//==============================================================================================================
	//	�L�����N�^�[�ŗL�X�L��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].InstanceAttackData();	// �A�^�b�N�f�[�^�쐬
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitScore = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->damage = 360;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pierceLV = 0;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitSE = "�ʏ�3�i�q�b�g";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffSE = "��U��2";
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->bAntiAir = false;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->ShakeCameraInfo.Set(.25f, 4);
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(3.5f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(3.5f, 2.0f);
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 6;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 40;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 40;
	// ����`��
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->width = 18;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->height = 10;
	m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->pCollisionShape->pos.Set(0, 6, 0);


	//==============================================================================================================
	//	�S����
	FOR((int)BASE_ACTION_STATE::END)
	{
		if (m_ActionDatas[i].isAttackData())
		{
			for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX;place++) MyAssert((m_ActionDatas[i].pAttackData->places[place].FlyVector.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
			MyAssert((m_ActionDatas[i].pAttackData->pCollisionShape->pos.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
			m_ActionDatas[i].pAttackData->bHit = false;
			// ���蔭�������u�Ԃɋ�U��SE���Đ����Ă݂�
			for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_ActionDatas[i].pAttackData->WhiffDelayFrame = frame; break; }
		}
	}
}

void Airou::SkillUpdate()
{



}

// AI
//PLAYER_INPUT Airou::AIHighAtackButton()
//{
//	m_pAI->Update();;
//}
