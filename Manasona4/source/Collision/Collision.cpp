#include "TDNLIB.h"
#include "Collision.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stage/Stage.h"
#include "../Shot/ShotManager.h"
#include "../Shot/BaseShot.h"
#include "../Stand/Stand.h"

Stage::Base *Collision::m_pStage = nullptr;

void Collision::PlayerCollision(PlayerManager *pPlayerMgr, ShotManager *pShotMgr)
{
	BasePlayer *pPlayer1(pPlayerMgr->GetPlayer(0)), *pPlayer2(pPlayerMgr->GetPlayer(1));

	/* �v���C���[VS�v���C���[�̍U������ */
	HIT_DAMAGE_INFO *pHitDamageInfo[(int)SIDE::ARRAY_MAX] = { nullptr };

	// �G�ւ̕����t���O�X�V
	{
		const Vector3 v(pPlayer1->GetPos() - pPlayer2->GetPos());
		if (v.x > 0)
		{
			pPlayer1->SetTargetDir(DIR::LEFT);
			pPlayer2->SetTargetDir(DIR::RIGHT);
		}
		else if (v.x < 0)
		{
			pPlayer1->SetTargetDir(DIR::RIGHT);
			pPlayer2->SetTargetDir(DIR::LEFT);
		}
	}

	// �߂荞�ݔ���
	if (!pPlayer1->isEscape() && !pPlayer2->isEscape()) Collision::Sinking(pPlayer1, pPlayer2);

	/* �v���C���[VS�v���C���[�̍U������ */
	//if (pPlayer1->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// �������������[�h�ɓ����Ă���
	//	CollisionThrowAttack(pPlayer1, pPlayer2, pStage);	// ��������֐�
	//else
		CollisionPlayerAttack(pPlayer1, pPlayer2, &pHitDamageInfo[1]);

	//if (pPlayer2->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// �������������[�h�ɓ����Ă���
	//	CollisionThrowAttack(pPlayer2, pPlayer1, pStage);	// ��������֐�
	//else
		CollisionPlayerAttack(pPlayer2, pPlayer1, &pHitDamageInfo[0]);


	/* �X�^���hVS�v���C���[�̍U������ */
	bool receive;	// ���茋�ʎ���p�ϐ�
	receive = CollisionStandAttack(pPlayer1->GetStand(), pPlayer2);
	if (receive) pPlayer1->GetStand()->GetAttackData()->bHit = true;
	receive = CollisionStandAttack(pPlayer2->GetStand(), pPlayer1);
	if (receive) pPlayer2->GetStand()->GetAttackData()->bHit = true;


	// �v���C���[VS�v���C���[�U�����ʊm��

	// �������Ă���
	if (pHitDamageInfo[0])
	{
		// ���b�Z�[�W�𑗐M
		SendHitMessage(pPlayer2, pPlayer1, pHitDamageInfo[0]);

		// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
		if (pPlayer2->isAttackState())pPlayer2->GetAttackData()->bHit = true;
	}
	if (pHitDamageInfo[1])
	{
		// ���b�Z�[�W�𑗐M
		SendHitMessage(pPlayer1, pPlayer2, pHitDamageInfo[1]);

		// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
		if (pPlayer1->isAttackState())pPlayer1->GetAttackData()->bHit = true;
	}

	/* ��VS���� */
	{
		SIDE Player1Side(pPlayer1->GetSide());
		for (auto it : *pShotMgr->GetList(Player1Side))
		{
			// ��VS�X�e�[�W
			if(RaypicShot(m_pStage, it)) it->Erase();

			// ��VS�v���C���[
			else if (CollisionShot(it,pPlayer2)) it->Erase();
		}
	}

	{
		SIDE Player2Side(pPlayer2->GetSide());
		for (auto it : *pShotMgr->GetList(Player2Side))
		{
			// ��VS�X�e�[�W
			if (RaypicShot(m_pStage, it)) it->Erase();

			// ��VS�v���C���[
			else if (CollisionShot(it, pPlayer1)) it->Erase();
		}
	}


	// ���
	FOR((int)SIDE::ARRAY_MAX)
	{
		if (pHitDamageInfo[i]) delete pHitDamageInfo[i];
	}

}

void Collision::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo)
{
	/* ���b�Z�[�W���M */

	// �_���[�W�󂯂��惁�b�Z�[�W
	MsgMgr->Dispatch(0, pAttackPlayer->GetID(), pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);

	// �_���[�W��^�����ɑ΂��ăq�b�g�X�g�b�v��������
	pAttackPlayer->SetHitStopFrame(pHitDamageInfo->hitStopFlame);
	//pDamagePlayer->SetHitStopFrame(pHitDamageInfo->hitStopFlame);
}

void Collision::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you, HIT_DAMAGE_INFO **OutDamageInfo)
{
	// �`�[����������I
	if (my->GetSide() == you->GetSide()) return;

	// �U���n�̃X�e�[�g����Ȃ���I
	if (!my->isAttackState()) return;

	// ���肪�G�X�P�[�v������I
	if (you->isEscape()) return;

	// ���łɓ������Ă���
	AttackData *pAttackData(my->GetAttackData());
	if (pAttackData->bHit) return;

	if (!my->isActiveFrame()) return; // �U���t���[��������Ȃ�
	// �Ӑg�̑΋�̏���
	/***************************************/
	// ������U�����Ȃ�
	if (you->isAttackState() == true)
	{
		// ���̍U�����@�����n���E�����̊Ԃ�
		// �u�΋�v���ʂ������Ă����ꍇ
		if ((you->GetActionFrame() == FRAME_STATE::ACTIVE ||
			you->GetActionFrame() == FRAME_STATE::START)
			&& you->GetAttackData()->bAntiAir == true)
		{
			// �������󒆂ōU�����Ă����ꍇ
			if (my->isLand() == false)return;
		}

	}
	/******************************************/


	if (you->GetInvincibleLV() <= pAttackData->pierceLV)	// ���肪���G�łȂ�
	{
		// �U������`��Ƒ���̎l�p�Ŕ�����Ƃ�
		CollisionShape::Square AttackShape, YouShape;
		memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pAttackData->pCollisionShape, sizeof(CollisionShape::Square));
		memcpy_s(&YouShape, sizeof(CollisionShape::Square), you->GetHitSquare(), sizeof(CollisionShape::Square));
		if (my->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// ����pos�͐��+(�E)�Ȃ̂ŁA�������Ȃ�t�ɂ���
		AttackShape.pos += my->GetPos();
		YouShape.pos += you->GetPos();
		if (Collision::HitCheck(&AttackShape, &YouShape))
		{
			// �����������Ȃ�
			if (pAttackData->attribute == ATTACK_ATTRIBUTE::THROW)
			{
				// ���肪��������n�̃X�e�[�g����Ȃ���������߂Ȃ�
				if (!isThrownState(you)) return;

				// bHit�X�V
				pAttackData->bHit = true;

				// �����艟���Ńq�b�g�X�g�b�v��������
				my->SetHitStopFrame(pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame);
				you->SetHitStopFrame(pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame);

				// �͂܂ꂽ���b�Z�[�W
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::BE_THROWN, nullptr);
			}

			else
			{
				// �q�b�g���̏���
				//pAttackData->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���

				/* ���b�Z�[�W���M */

				// ���肪�q�b�g�����Ƃ��̒n��ɂ������󒆂ɂ�����
				int iHitPlace((!you->isLand() || you->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) || you->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance())) ? (int)AttackData::HIT_PLACE::AERIAL : (int)AttackData::HIT_PLACE::LAND);

				// (A���)(TODO) ���������ꂾ�ƃt�B�j�b�V���A�[�c���ЂƂ������݂��Ȃ��Ȃ�̂łǂ��ɂ����悤
				// �t�B�j�b�V���A�[�c���ǂ���
				bool bOverDrive(my->GetActionState() == BASE_ACTION_STATE::HEAVEHO_DRIVE);

				int hitScore(pAttackData->HitScore);
				//if (you->isGuard()) // ���肪�K�[�h���Ȃ�
				//{
				//	hitScore = (int)(hitScore * 0.25f);// �X�R�A����
				//
				//	// ���e��
				//	if (my->GetDir() == DIR::RIGHT)
				//	{
				//		my->SetMove(Vector3(-0.5f,0,0));
				//	}
				//	else
				//	{
				//		my->SetMove(Vector3(0.5f, 0, 0));
				//	}
				//}

				// �����āA�_���[�W���󂯂��l�ɑ��M
				*OutDamageInfo = new HIT_DAMAGE_INFO;
				(*OutDamageInfo)->BeInvincible = pAttackData->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				(*OutDamageInfo)->damage = pAttackData->damage;				// �_���[�W(�X�R�A)
				(*OutDamageInfo)->FlyVector = pAttackData->places[iHitPlace].FlyVector;			// ������уx�N�g��
				(*OutDamageInfo)->hitStopFlame = pAttackData->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
				(*OutDamageInfo)->HitRecoveryFrame = pAttackData->places[iHitPlace].HitRecoveryFrame;		// �d������
				(*OutDamageInfo)->GuardRecoveryFrame = pAttackData->GuardRecoveryFrame;		// �d������
				(*OutDamageInfo)->HitEffectType = (int)pAttackData->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				(*OutDamageInfo)->iAttackType = (int)my->GetActionState();						// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
				(*OutDamageInfo)->bOverDrive = bOverDrive;										// �ӂ��ɂ���A�[�c���ǂ���	
				(*OutDamageInfo)->iAntiGuard = (int)pAttackData->AntiGuard;				// �K�[�h�˂��j��^�C�v
				(*OutDamageInfo)->HitSE = pAttackData->HitSE;
				(*OutDamageInfo)->bFinishOK = pAttackData->bFinish;
				(*OutDamageInfo)->fGuardKnockBackPower = pAttackData->fGuardKnockBackPower;
				(*OutDamageInfo)->DamageMotion = pAttackData->places[iHitPlace].DamageMotion;
				(*OutDamageInfo)->fComboRate = pAttackData->fComboRate;
				(*OutDamageInfo)->iAttribute = (int)pAttackData->attribute;

				// ���R���{UI �G�t�F�N�g(�J�E���g)����
				// �����v���C���[�̃O���[�o���X�e�[�g�̃_���[�W�̂Ƃ���Ɉڂ��܂���
				//if (you->isGuard() == false)
				//	you->GetComboUI()->Count(hitScore, (*OutDamageInfo)->recoveryFlame); // ���肪�K�[�h���łȂ����
				//else you->GetComboUI()->Guard();// �K�[�h���ꂽ

				if (
					//my->GetPos().x > you->GetPos().x// �ʒu�֌W�ɂ��x�N�g��
					my->GetDir() == DIR::LEFT			// ���Ă��l�̌����ɂ��x�N�g��
					) (*OutDamageInfo)->FlyVector.x *= -1;
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

				// �J�����ɐU�����b�Z�[�W�𑗂�
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pAttackData->ShakeCameraInfo);
			}
		}
	}
}

bool Collision::CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou)
{
	// �`�[����������I
	if (pStand->GetPlayer()->GetSide() == pYou->GetSide()) return false;

	// �X�^���h���A�N�e�B�u����Ȃ���I
	if (!pStand->isActive()) return false;

	// ���肪�G�X�P�[�v������I
	if (pYou->isEscape()) return false;

	// �U���t���[��������Ȃ�
	if (!pStand->isAttackFrame()) return false;

	// �X�^���h���A�^�b�N�^�C�v����Ȃ���I
	AttackData *pStandAttackData(pStand->GetAttackData());
	if (pStandAttackData == nullptr)return false;

	// ���łɓ������Ă���
	if (pStandAttackData->bHit) return false;

	if (pYou->GetInvincibleLV() == 0)								// ���肪���G�łȂ�
	{
		// �U������`��Ƒ���̎l�p�Ŕ�����Ƃ�
		CollisionShape::Square AttackShape, YouShape;
		memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pStandAttackData->pCollisionShape, sizeof(CollisionShape::Square));
		memcpy_s(&YouShape, sizeof(CollisionShape::Square), pYou->GetHitSquare(), sizeof(CollisionShape::Square));
		if (pStand->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// ����pos�͐��+(�E)�Ȃ̂ŁA�������Ȃ�t�ɂ���
		AttackShape.pos += pStand->GetPos();
		YouShape.pos += pYou->GetPos();
		if (Collision::HitCheck(&AttackShape, &YouShape))
		{
			// �q�b�g���̏���
			//pAttackData->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���

			/* ���b�Z�[�W���M */

			// ���肪�q�b�g�����Ƃ��̒n��ɂ������󒆂ɂ�����
			int iHitPlace((!pYou->isLand() || pYou->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) || pYou->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance())) ? (int)AttackData::HIT_PLACE::AERIAL : (int)AttackData::HIT_PLACE::LAND);
			// �܂��A�U�����q�b�g�������l�ɑ��M
			//HIT_ATTACK_INFO hai;
			//hai.bOverDrive = false;
			//hai.hitStopFlame = pStandAttackData->places[iHitPlace].hitStopFlame;		// �������g�ɂ��̎����̃q�b�g�X�g�b�v
			//hai.HitScore = pStandAttackData->HitScore;				// �_���[�W(�X�R�A)
			//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pStand->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);
			//// �����G�t�F�N�g�ǉ�
			//Vector2 screenPos = Math::WorldToScreen(pStand->GetPos());
			//NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, pStandAttackData->damage, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

			// �����āA�_���[�W���󂯂��l�ɑ��M
			HIT_DAMAGE_INFO HitDamageInfo;
			HitDamageInfo.bOverDrive = false;// ��(��)
			HitDamageInfo.BeInvincible = pStandAttackData->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
			HitDamageInfo.damage = pStandAttackData->damage;				// �_���[�W(�X�R�A)
			HitDamageInfo.FlyVector = pStandAttackData->places[iHitPlace].FlyVector;			// ������уx�N�g��
			HitDamageInfo.hitStopFlame = pStandAttackData->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
			HitDamageInfo.HitRecoveryFrame = pStandAttackData->places[iHitPlace].HitRecoveryFrame;		// �d������
			HitDamageInfo.GuardRecoveryFrame = pStandAttackData->GuardRecoveryFrame;		// �d������
			HitDamageInfo.HitEffectType = (int)pStandAttackData->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
			HitDamageInfo.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTION�̂Ƃ��́A�R���{�p���֘A�̂𖳎�����
			HitDamageInfo.iAntiGuard = (int)pStandAttackData->AntiGuard;				// �K�[�h�˂��j��^�C�v
			HitDamageInfo.HitSE = pStandAttackData->HitSE;
			HitDamageInfo.bFinishOK = pStandAttackData->bFinish;
			HitDamageInfo.fGuardKnockBackPower = pStandAttackData->fGuardKnockBackPower;
			HitDamageInfo.DamageMotion = pStandAttackData->places[iHitPlace].DamageMotion;
			HitDamageInfo.fComboRate = pStandAttackData->fComboRate;
			HitDamageInfo.iAttribute = (int)pStandAttackData->attribute;

			// �R���{�p
			//COMBO_DESK comboDesk;
			//comboDesk.side = pYou->GetSide();
			//comboDesk.damage = HitDamageInfo.damage;
			//comboDesk.recoveryFrame = HitDamageInfo.recoveryFlame;
			//
			//// ���R���{UI �G�t�F�N�g(�J�E���g)����
			//if (pYou->GetGuardState() == GUARD_STATE::NO_GUARD)
			//	MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);
			//else
			//	MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_GUARD, &comboDesk);
			if (
				//my->GetPos().x > you->GetPos().x// �ʒu�֌W�ɂ��x�N�g��
				pStand->GetDir() == DIR::LEFT			// ���Ă��l�̌����ɂ��x�N�g��
				) HitDamageInfo.FlyVector.x *= -1;
			MsgMgr->Dispatch(0, pStand->GetPlayer()->GetID(), pYou->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);


			// �J�����ɐU�����b�Z�[�W�𑗂�
			MsgMgr->Dispatch(0, pStand->GetPlayer()->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pStandAttackData->ShakeCameraInfo);

			return true;	// ��������
		}
	}
	return false;	// ������Ȃ�����
}

bool Collision::CollisionShot(Shot::Base *shot, BasePlayer *you)
{
	// ���肪�G�X�P�[�v������I
	if (you->isEscape()) return false;

	if (you->GetInvincibleLV() != 0) return false;									// ���肪���G����Ȃ�

	AttackData *pShotAttackData(shot->GetAttackData());

	if (isInputGuardCommand(you))
	{
		if (Math::Length(you->GetPos(), shot->GetPos()) < BasePlayer::c_GUARD_DISTANCE)// �K�[�h��������
		{
			you->GetFSM()->ChangeState(BasePlayerState::Guard::GetInstance());
			//return false;
		}
	}

	// �U������`��Ƒ���̎l�p�Ŕ�����Ƃ�
	CollisionShape::Square AttackShape, YouShape;
	memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pShotAttackData->pCollisionShape, sizeof(CollisionShape::Square));
	memcpy_s(&YouShape, sizeof(CollisionShape::Square), you->GetHitSquare(), sizeof(CollisionShape::Square));
	AttackShape.pos += shot->GetPos();
	YouShape.pos += you->GetPos();
	if (Collision::HitCheck(&AttackShape, &YouShape))
	{
		// �q�b�g���̏���
		//pAttackData->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���

		/* ���b�Z�[�W���M */

		// ���肪�q�b�g�����Ƃ��̒n��ɂ������󒆂ɂ�����
		int iHitPlace = (you->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL;

		// �����āA�_���[�W���󂯂��l�ɑ��M
		HIT_DAMAGE_INFO HitDamageInfo;
		HitDamageInfo.bOverDrive = false;// ��(��)
		HitDamageInfo.BeInvincible = pShotAttackData->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
		HitDamageInfo.damage = pShotAttackData->damage;				// �_���[�W(�X�R�A)
		HitDamageInfo.FlyVector = pShotAttackData->places[iHitPlace].FlyVector;			// ������уx�N�g��
		HitDamageInfo.hitStopFlame = pShotAttackData->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
		HitDamageInfo.HitRecoveryFrame = pShotAttackData->places[iHitPlace].HitRecoveryFrame;		// �d������
		HitDamageInfo.GuardRecoveryFrame = pShotAttackData->GuardRecoveryFrame;		// �d������
		HitDamageInfo.HitEffectType = (int)pShotAttackData->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
		HitDamageInfo.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTION�̂Ƃ��́A�R���{�p���֘A�̂𖳎�����
		HitDamageInfo.iAntiGuard = (int)pShotAttackData->AntiGuard;				// �K�[�h�˂��j��^�C�v
		HitDamageInfo.HitSE = pShotAttackData->HitSE;
		HitDamageInfo.bFinishOK = pShotAttackData->bFinish;
		HitDamageInfo.fGuardKnockBackPower = pShotAttackData->fGuardKnockBackPower;
		HitDamageInfo.DamageMotion = pShotAttackData->places[iHitPlace].DamageMotion;
		HitDamageInfo.iAttribute = (int)pShotAttackData->attribute;

		if (shot->GetVec().x < 0) HitDamageInfo.FlyVector.x *= -1;
		MsgMgr->Dispatch(0, shot->GetPlayer()->GetID(), you->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

		// �J�����ɐU�����b�Z�[�W�𑗂�
		MsgMgr->Dispatch(0, shot->GetPlayer()->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pShotAttackData->ShakeCameraInfo);

		return true;	// ��������
	}
	else return false;
}


//// �������̍U���ɑ΂���v���C���[�̔���
//bool Collision::CollisionThrowAttack(BasePlayer *my, BasePlayer *you, Stage::Base *pStage)
//{
//	// �`�[����������I
//	if (my->GetSide() == you->GetSide()) return false;
//
//	// ���肪��������n�̃X�e�[�g����Ȃ���������߂Ȃ�
//	if (!isThrownState(you)) return false;
//
//	// �������łɒ͂�ł��I
//	if (my->isThrowSuccess()) return false;
//
//	// ���肪���G���
//	if (you->GetInvincibleLV() > my->GetAttackData()->pierceLV) return false;
//
//	// �������蒆����Ȃ�
//	if (
//		//my->GetCurrentFrame() >= BasePlayer::c_THROW_ESCAPE_FRAME
//		my->GetActionFrame() != FRAME_STATE::ACTIVE
//		) return false;
//
//	// ����������
//	Vector3 v(you->GetPos() - my->GetPos());
//	if (v.x * v.x + v.y * v.y < 10 * 10 &&
//		((v.x > 0 && my->GetDir() == DIR::RIGHT) || (v.x < 0 && my->GetDir() == DIR::LEFT)))
//	{
//		// �������ł��܂ꂽ����̍��W�����񂾑���̒͂ݍ��W�Ɉړ�(���ʂɑ������ƕǔ�������̂ŁA�ǔ��������낱���ł���Ă��܂����Ƃ�����)
//		if (my->GetDir() == DIR::LEFT  && my->isPushInput(PLAYER_COMMAND_BIT::RIGHT) ||
//			my->GetDir() == DIR::RIGHT && my->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		{
//			// �������t�ɂ���
//			my->ReverseDir();
//
//			// ����̍��W�Ǝ����̍��W�����ւ���
//			Vector3 temp(my->GetPos());
//			my->SetPos(you->GetPos());
//			you->SetPos(temp);
//		}
//
//		const Vector3 p(my->GetPos());
//		Vector3 v(my->GetFlontPos() - p), v2(v);
//
//		pStage->Collision(my, &v);	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
//		if (fabsf(v.x) == 0)
//		{
//			my->GetPosAddress()->x -= v2.x;
//			you->SetPos(my->GetPos() + v2);
//		}
//		else you->SetPos(p + v);
//
//		/* ���b�Z�[�W���M */
//
//		// �͂ݐ���������ƃ��b�Z�[�W�𑗂�
//		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, my->GetID(), MESSAGE_TYPE::THROW_SUCCESS, nullptr);
//
//		// �͂܂ꂽ���b�Z�[�W
//		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::BE_THROWN, nullptr);
//
//		return true;
//	}
//
//	return false;
//}

bool Collision::RaypicShot(Stage::Base *obj, Shot::Base *shot)
{
	const Vector3 move(shot->GetMove());
	const Vector3 pos(shot->GetPos());

	if (move.y > 0) // ��
	{
		if (pos.y + move.y > 100) return true;
	}
	else if (move.y < 0) // ��
	{
		if (pos.y + move.y < obj->GetBottom()) return true;
	}

	if (move.x > 0) // �E
	{
		if (pos.x + move.x > obj->GetWidth() / 2) return true;
	}
	else if (move.x < 0) // ��
	{
		if (pos.x + move.x < -obj->GetWidth() / 2) return true;
	}

	return false;
}

void Collision::Raypic(BasePlayer *player, Vector3 *move) // �X�e�[�W�ƃv���C���[
{
	if (move->y > 0) // ��
	{
		//RaypicUp(obj, player, move);

		// �������@move�l��0�ȏゾ�����ꍇ�AisLand��������Ă��郌�C�s�b�N���Ă΂�Ȃ����߁A�ʏ�3�i�ڊk�̔h�����؋�ɂȂ�Ȃ��B
		// move�lY��0�ȏ�Ȃ�A�ԈႢ�Ȃ��n��ł͂Ȃ��ꏊ�ɂ��邾�낤�Ɖ���A�����I�ɒn��t���O���I�t�ɂ���
		player->SetLand(false);//�v���C���[�̃|�W�V�������Z�b�g������ɕύX
	}
	else if (move->y <= 0) // ��
	{
		RaypicDown(m_pStage, player, move);
	}

	if (move->x != 0)
	{
		// �v���C���[�ƃv���C���[�̋���
		const float fTargetVecX(player->GetTargetPlayer()->GetPos().x - player->GetPos().x);

		if (move->x > 0) // �E
		{
			RaypicRight(m_pStage, player, move, fTargetVecX);
		}
		else if (move->x < 0) // ��
		{
			RaypicLeft(m_pStage, player, move, fTargetVecX);
		}
	}
}

float Collision::CheckMove(BasePlayer *pPlayer, const Vector3 &vMove)
{
	Vector3 pos(pPlayer->GetPos()), move(vMove);
	const CollisionShape::Square *square(pPlayer->GetHitSquare());
	float width(m_pStage->GetWidth() / 2);

	if (move.x < 0) // ��
	{
		width *= -1;
		if (pos.x - square->width + move.x <= width)
		{
			pos.x = width + square->width - square->pos.x;

			// �����̍��W����ǂւ�X������Ԃ�
			return pos.x - pPlayer->GetPos().x;
		}
	}

	else if(move.x > 0) // �E
	{
		if (pos.x + square->width + move.x >= width)
		{
			pos.x = width - square->width + square->pos.x;

			// �����̍��W����ǂւ�X������Ԃ�
			return pos.x - pPlayer->GetPos().x;
		}
	}

	// �ς��Ȃ��ړ���
	return move.x;
}

void Collision::RaypicDown(Stage::Base *obj, BasePlayer *player, Vector3 *move)
{
	if (move->y <= 0) // ��
	{
		bool hit(false);

		Vector3 pos(player->GetPos());
		const CollisionShape::Square *square(player->GetHitSquare());
		Vector3 ray_pos;

		ray_pos = pos;
		ray_pos += square->pos;
		//ray_pos.x += square->width; // �E����

		const float bottom(obj->GetBottom());

		//if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // ��������
		{
			if (pos.y + move->y <= bottom)
			{
				pos.y = bottom + square->height - square->pos.y;
				move->y = 0;

				// �v���C���[�ɃZ�b�g����̂ł͂Ȃ��Amove�l�̃A�h���X�������ł�����Ē��ڏ���������`�ɂ����B(�߂荞�ݔ���ŁA�߂荞�ݔ���p��move�l���g�����ƂɂȂ����̂�)
				//player->SetMove(move);
				player->SetPos(pos);

				// �������@�n�ʂɕt������
				player->SetLand(true);//�v���C���[�̃|�W�V�������Z�b�g������ɕύX
				player->SetAerialJump(true);// �󒆃W�����v�̌������߂�
				hit = true;
			}
			// �󒆂ɂ����I
			else
			{
				//player->SetLand(false);//�v���C���[�̃|�W�V�������Z�b�g������ɕύX
				// �������b�Z�[�W���M
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + (int)player->GetSide()), MESSAGE_TYPE::FALL, nullptr);
			}
		}


		//if (!hit)
		//{
		//	ray_pos = pos;
		//	ray_pos += square->pos;
		//	ray_pos.x -= square->width; // ������
		//	ray_vec.Set(0, -1, 0);
		//	dist = square->height - move->y + 1;
		//
		//	if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // ��������
		//	{
		//		dist = abs(hit_pos.y - ray_pos.y);
		//		if (dist <= square->height - move->y)
		//		{
		//			pos.y = hit_pos.y + square->height - square->pos.y;
		//			move->y = 0;
		//
		//			// �v���C���[�ɃZ�b�g����̂ł͂Ȃ��Amove�l�̃A�h���X�������ł�����Ē��ڏ���������`�ɂ����B(�߂荞�ݔ���ŁA�߂荞�ݔ���p��move�l���g�����ƂɂȂ����̂�)
		//			//player->SetMove(move);
		//			player->SetPos(pos);
		//
		//			//player->Land();//�v���C���[�̃|�W�V�������Z�b�g������ɕύX
		//		}
		//	}
		//}
	}
}
void Collision::RaypicLeft(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetVecX)
{
	Vector3 pos(player->GetPos());
	const CollisionShape::Square *square(player->GetHitSquare());

	const float width(
		//(fabsf(fTargetVecX) > 50) ? pos.x : 
		-obj->GetWidth()/2);

	if (move->x < 0) // ��
	{
		bool hit = false;

		//if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // ��������
		{
			if (pos.x - square->width + move->x <= width)
			{
				pos.x = width + square->width - square->pos.x;
				move->x = 0;

				//player->SetMove(move);
				player->SetPos(pos);
				hit = true;
			}
		}
	}
}
void Collision::RaypicRight(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetVecX)
{
	Vector3 pos(player->GetPos());
	const CollisionShape::Square *square(player->GetHitSquare());

	if (move->x > 0) // �E
	{
		bool hit = false;

		const float width(
			//(fabsf(fTargetVecX) > 50) ? pos.x : 
			obj->GetWidth() / 2);

		//if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // ��������
		{
			if (pos.x + square->width + move->x >= width)
			{
				pos.x = width - square->width + square->pos.x;
				move->x = 0;

				// �v���C���[�ɃZ�b�g����̂ł͂Ȃ��Amove�l�̃A�h���X�������ł�����Ē��ڏ���������`�ɂ����B(�߂荞�ݔ���ŁA�߂荞�ݔ���p��move�l���g�����ƂɂȂ����̂�)
				//player->SetMove(move);
				player->SetPos(pos);
				hit = true;
			}
		}

		//if (!hit) // ������Ȃ���������㉺����ray�o�� 
		//{
		//	float h(0);
		//	if (move->y > 0) // ��
		//	{
		//		h = square->height * 0.85f;
		//	}
		//	else // ��
		//	{
		//		h = -square->height * 0.85f;
		//	}
		//	ray_pos = pos;
		//	ray_pos += square->pos;
		//	ray_pos.y += h;
		//	ray_vec.Set(1, 0, 0);
		//	dist = square->width + move->x + 1;
		//
		//	if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // ��������
		//	{
		//		dist = abs(hit_pos.x - ray_pos.x);
		//		if (dist <= square->width + move->x)
		//		{
		//			pos.x = hit_pos.x - square->width - square->pos.x;
		//			move->x = 0;
		//
		//			//player->SetMove(move);
		//			player->SetPos(pos);
		//		}
		//	}
		//}

	}
}


void Collision::Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2)
{
	// �v���C���[�̔��聡
	CollisionShape::Square s1, s2;
	memcpy_s(&s1, sizeof(CollisionShape::Square), pPlayer1->GetHitSquare(), sizeof(CollisionShape::Square));
	memcpy_s(&s2, sizeof(CollisionShape::Square), pPlayer2->GetHitSquare(), sizeof(CollisionShape::Square));
	s1.pos += pPlayer1->GetPos(), s2.pos += pPlayer2->GetPos();

	// �Ƃ肠�����A���������肵�Ă݂�
	Vector3 v = s2.pos - s1.pos;
	if (HitCheck(&s1, &s2))
	{
		const float len = v.Length();
		//v.Normalize();	// ���K��
		Vector3 sinking1((s1.width + s2.width) - sqrtf(v.x*v.x), (s1.height + s2.height) - sqrtf(v.y*v.y), 0);	// �߂荞�ݗ�
		Vector3 sinking2(-sinking1);
		if (v.x > 0)
		{
			sinking1.x *= -1;
			sinking2.x *= -1;
		}

		sinking1.y = 0;
		sinking2.y = 0;
		static const float rate = .5f;
		sinking1 *= rate;	// ���S�ɖ߂��ƕs���R�Ȃ̂�
		sinking2 *= rate;

		// �ށ[�Ԃ��ɑ����Ƌ��������������̂ŁA���W�ɑ����B�ł����̂܂ܑ����ƕǔ�������̂Ń��C�s�b�N
		Raypic(pPlayer1, &sinking1);
		Raypic(pPlayer2, &sinking2);

		pPlayer1->SetPos(pPlayer1->GetPos() + sinking1);
		pPlayer2->SetPos(pPlayer2->GetPos() + sinking2);
	}
}


bool Collision::HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2)
{
	float dist(c1->radius + c2->radius);
	float lenx(c1->pos.x - c2->pos.x),
		leny(c1->pos.y - c2->pos.y);

	return ((dist * dist) > (lenx*lenx + leny*leny));
}

bool Collision::HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2)
{
	if (s1->height + s2->height < abs(s1->pos.y - s2->pos.y))
		return false;

	if ((s1->width + s2->width) < abs(s1->pos.x - s2->pos.x))
		return false;

	return true;
}

bool Collision::HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s)
{
	// �~�Ɉ�ԋ߂����_�̈ʒu
	float vx, vy;
	// x
	if (c->pos.x > s->pos.x)
		vx = s->pos.x + s->width;
	else
		vx = s->pos.x - s->width;
	// y
	if (c->pos.y > s->pos.y)
		vy = s->pos.y + s->height;
	else
		vy = s->pos.y - s->height;


	// �~���璸�_�܂ł̋���
	float lx, ly;
	lx = s->pos.x - c->pos.x;
	ly = s->pos.y - c->pos.y;
	// �~�̒��ɒ��_������
	if (c->radius*c->radius < lx*lx + ly*ly)
		return true;


	bool inx(s->pos.x - s->width <= c->pos.x && c->pos.x <= s->pos.x + s->width)
		, iny(s->pos.y - s->height <= c->pos.y && c->pos.y <= s->pos.y + s->height);
	// �����`�̒��ɉ~�̒��S������
	if (inx && iny)
		return true;

	// �~�������`�ƐG��Ă���
	if (inx)
	{
		if (c->radius + s->height > abs(c->pos.y - s->pos.y))
			return true;
	}
	else if (iny)
	{
		if (c->radius + s->width > abs(c->pos.x - s->pos.x))
			return true;
	}

	return false;
}
