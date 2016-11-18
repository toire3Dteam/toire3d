#include "PlayerManager.h"
#include "Airou\Airou.h"
#include "Stage\Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stand/Stand.h"
#include "../Number/Number.h"

// ���̂̐錾
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// �G���e�B�e�BID�o�^
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr),
m_bHeaveHoDriveOverFlow(false)
{
	m_PointAteam = 0;
	m_PointBteam = 0;

	m_OverDriveDim = 255;
}
PlayerManager::PlayerManager(const PlayerManager&) :BaseGameEntity(ENTITY_ID::PLAYER_MGR){}

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage, SideData SideDatas[(int)SIDE::ARRAY_MAX])
{
	m_NumPlayer = NumPlayer;
	m_pStage = pStage;

	// �v���C���m��
	m_pPlayers = new BasePlayer*[NumPlayer];

	FOR(NumPlayer)
	{
		// �`�[�������͉��ŐU�蕪��!! 2016/10.04���@�����E��Team�ŕς��Ă�
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// AI���ǂ����t���O
		const bool bAI(
			//(i == 2)/*(i == 1 || i == 2 || i == 3)*/);//*i == 1*/0))
			SideDatas[(int)side].bAI	// �I�΂�Ă��Ȃ��T�C�h��AI
			);


		// �L�����N�^�[�ŕ��򂷂�
		switch (SideDatas[(int)side].CharacterType)
		{
		case CHARACTER::AIROU:
			m_pPlayers[i] = new Airou(SideDatas[(int)side].iDeviceID, side, bAI);
			break;
		default:
			MyAssert(false,"����ȃL�����N�^�[�͂��Ȃ�")
			break;
		}
		m_pPlayers[i]->InitActionDatas();		// ���U�������e�L�����ɏ�����������
		m_pPlayers[i]->InitMotionDatas();		// �e�L�������Ƃ̕K�E�Z
	}

	// �v���C���[��2�l�Ƃ����O��̃R�[�h
	assert(NumPlayer == 2);
	m_pPlayers[0]->SetTargetPlayer(m_pPlayers[1]);
	m_pPlayers[1]->SetTargetPlayer(m_pPlayers[0]);

	FOR(NumPlayer)
	{
		// AI������ �Ȃ������ŏ���������Ƃ����ƁA�������̒��Ńv���C���[���Q�Ƃ��邱�Ƃ�����A�S���̏�����������������ɌĂяo����������
		m_pPlayers[i]->InitAI();
	}
}

PlayerManager::~PlayerManager()
{
	// �_�u���|�C���^�[�̊J��
	FOR(m_NumPlayer)delete m_pPlayers[i];
	delete[] m_pPlayers;
}

void PlayerManager::Update(bool bControl)
{	
	// �N����1More�o�����Ă�����S���̓������~�߂�
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->isGameTimerStopFlag() == true)
		{
			m_pPlayers[i]->Update(bControl);			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
			
			m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
			m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V

			// ���Ó]����
			m_OverDriveDim = max(m_OverDriveDim - 0.1f , 0.25f);

			return; // �ق��̃L�����N�^�[�̏��ւ͒f���Ēʂ��Ȃ�
		}
		
	}
	// �N���o�����Ă��Ȃ������̂Œʏ�ʂ�X�V
	
	// ���Ó]�������
	m_OverDriveDim = min(m_OverDriveDim + 0.1f, 1.0f);
	
	

	/* �v���C���[�����X�V */
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)m_pPlayers[i]->UpdateDrive();			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
		else m_pPlayers[i]->Update(bControl);
	}

	/* �v���C���[VS�v���C���[�̍U������ */
	HIT_DAMAGE_INFO **pHitDamageInfo=new HIT_DAMAGE_INFO*[m_NumPlayer];
	FOR(m_NumPlayer)
	{
		pHitDamageInfo[i] = nullptr;
	}

	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			// �G�ւ̕����t���O�X�V
			UpdatePlayerTargetDir(m_pPlayers[my], m_pPlayers[you]);

			// �߂荞�ݔ���
			if(!m_pPlayers[my]->isEscape() && !m_pPlayers[you]->isEscape())m_pStage->Sinking(m_pPlayers[my], m_pPlayers[you]);

			// ( my->you you->my )�܂���݂ɓ������Ă��Ȃ����`�F�b�N����

			/* �v���C���[VS�v���C���[�̍U������ */
			/* �v���C���[VS�v���C���[�̓����͂ݔ��� */
			if (m_pPlayers[my]->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// �������������[�h�ɓ����Ă���
				CollisionThrowAttack(m_pPlayers[my], m_pPlayers[you]);	// ��������֐�
			else
				CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you], &pHitDamageInfo[you]);

			if (m_pPlayers[you]->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// �������������[�h�ɓ����Ă���
				CollisionThrowAttack(m_pPlayers[you], m_pPlayers[my]);	// ��������֐�
			else
				CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my], &pHitDamageInfo[my]);


			/* �X�^���hVS�v���C���[�̍U������ */
			bool receive;	// ���茋�ʎ���p�ϐ�
			receive = CollisionStandAttack(m_pPlayers[my]->GetStand(), m_pPlayers[you]);
			if (receive) m_pPlayers[my]->GetStand()->GetAttackData()->bHit = true;
			receive = CollisionStandAttack(m_pPlayers[you]->GetStand(), m_pPlayers[my]);
			if (receive) m_pPlayers[you]->GetStand()->GetAttackData()->bHit = true;
		}
	}
	// �v���C���[VS�v���C���[�U�����ʊm��
	for (int DamagedPlayerNo(0); DamagedPlayerNo < m_NumPlayer; DamagedPlayerNo++)
	{
		// �������Ă���
		if (pHitDamageInfo[DamagedPlayerNo])
		{
			// �_���[�W���󂯂��l�̃^�[�Q�b�g�v���C���[�͂��Ȃ킿�_���[�W��^�����l
			BasePlayer *pAttackPlayer(m_pPlayers[DamagedPlayerNo]->GetTargetPlayer());

			// ���b�Z�[�W�𑗐M
			SendHitMessage(pAttackPlayer, m_pPlayers[DamagedPlayerNo], pHitDamageInfo[DamagedPlayerNo]);

			// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
			if (pAttackPlayer->isAttackState())pAttackPlayer->GetAttackData()->bHit = true;
		}
	}


	/* �ʒu���m�� */
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V
	}

	// ���
	FOR(m_NumPlayer)
	{
		if (pHitDamageInfo[i]) delete pHitDamageInfo[i];
	}
	delete[] pHitDamageInfo;

	// �`�[���|�C���g�v�Z
	CalcTeamPoint();

}

void PlayerManager::Render()
{
	// �v���C���[�����`��
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)m_pPlayers[i]->RenderDrive();
		else m_pPlayers[i]->Render();
	}
}

void PlayerManager::Render(tdnShader* shader, char* name)
{
	// �v���C���[�����`��
	FOR(m_NumPlayer) m_pPlayers[i]->Render(shader, name);
}

void PlayerManager::RenderShadow()
{
	// �v���C���[�̉e�`��p
	FOR(m_NumPlayer) m_pPlayers[i]->RenderShadow();

}

void PlayerManager::RenderDeferred()
{
	// �v���C���[�����`��
	FOR(m_NumPlayer) m_pPlayers[i]->RenderDeferred();

}

void PlayerManager::RenderUI()
{
	// �v���C���[��UI�`��
	FOR(m_NumPlayer) m_pPlayers[i]->RenderUI();
}

void PlayerManager::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you, HIT_DAMAGE_INFO **OutDamageInfo)
{
	// �`�[����������I
	if (my->GetSide() == you->GetSide()) return;

	// �U���n�̃X�e�[�g����Ȃ���I
	if (!my->isAttackState()) return;

	// ���肪�G�X�P�[�v������I
	if (you->isEscape()) return;

	if (my->isActiveFrame()) // �U���t���[�����Ȃ�
	{
		// �Ӑg�̑΋�̏���
		/***************************************/
		// ������U�����Ȃ�
		if (you->isAttackState() == true)
		{
			// ���̍U�����@�����n���E�����̊Ԃ�
			// �u�΋�v���ʂ������Ă����ꍇ
			if ((you->GetActionFrame() == FRAME_STATE::ACTIVE ||
				you->GetActionFrame() == FRAME_STATE::START)
				&& you->GetAttackData()->bAntiAir== true)
			{
				// �������󒆂ōU�����Ă����ꍇ
				if (my->isLand() == false)return;
			}

		}
		/******************************************/

		if (you->GetInvincibleLV() <= my->GetAttackData()->pierceLV &&	// ���肪���G�łȂ�
			!my->GetAttackData()->bHit)									// �܂��U���𓖂ĂĂȂ�
		{
			// �U������`��Ƒ���̎l�p�Ŕ�����Ƃ�
			CollisionShape::Square AttackShape, YouShape;
			memcpy_s(&AttackShape, sizeof(CollisionShape::Square), my->GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			memcpy_s(&YouShape, sizeof(CollisionShape::Square), you->GetHitSquare(), sizeof(CollisionShape::Square));
			if (my->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// ����pos�͐��+(�E)�Ȃ̂ŁA�������Ȃ�t�ɂ���
			AttackShape.pos += my->GetPos();
			YouShape.pos += you->GetPos();
			if (Collision::HitCheck(&AttackShape, &YouShape))
			{
				// �q�b�g���̏���
				//my->GetAttackData()->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���

				/* ���b�Z�[�W���M */

				// ���肪�q�b�g�����Ƃ��̒n��ɂ������󒆂ɂ�����
				int iHitPlace((you->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL);

				// (A���)(TODO) ���������ꂾ�ƃt�B�j�b�V���A�[�c���ЂƂ������݂��Ȃ��Ȃ�̂łǂ��ɂ����悤
				// �t�B�j�b�V���A�[�c���ǂ���
				bool bOverDrive(my->GetActionState() == BASE_ACTION_STATE::HEAVEHO_DRIVE);
				
				int hitScore(my->GetAttackData()->HitScore);
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
				(*OutDamageInfo)->BeInvincible = my->GetAttackData()->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				(*OutDamageInfo)->damage = my->GetAttackData()->damage;				// �_���[�W(�X�R�A)
				(*OutDamageInfo)->FlyVector = my->GetAttackData()->places[iHitPlace].FlyVector;			// ������уx�N�g��
				(*OutDamageInfo)->hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
				(*OutDamageInfo)->HitRecoveryFrame = my->GetAttackData()->places[iHitPlace].HitRecoveryFrame;		// �d������
				(*OutDamageInfo)->GuardRecoveryFrame = my->GetAttackData()->GuardRecoveryFrame;		// �d������
				(*OutDamageInfo)->HitEffectType = (int)my->GetAttackData()->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				(*OutDamageInfo)->iAttackType = (int)my->GetActionState();						// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
				(*OutDamageInfo)->bOverDrive = bOverDrive;										// �ӂ��ɂ���A�[�c���ǂ���	
				(*OutDamageInfo)->iAntiGuard = (int)my->GetAttackData()->AntiGuard;				// �K�[�h�˂��j��^�C�v
				(*OutDamageInfo)->HitSE = my->GetAttackData()->HitSE;
				(*OutDamageInfo)->bFinishOK = my->GetAttackData()->bFinish;

				// ���R���{UI �G�t�F�N�g(�J�E���g)����
				// �����v���C���[�̃O���[�o���X�e�[�g�̃_���[�W�̂Ƃ���Ɉڂ��܂���
				//if (you->isGuard() == false)
				//	you->GetComboUI()->Count(hitScore, (*OutDamageInfo)->recoveryFlame); // ���肪�K�[�h���łȂ����
				//else you->GetComboUI()->Guard();// �K�[�h���ꂽ

				// ���������≼
				//my->SetHP(my->GetMaxHP() - my->GetCollectScore());
				//you->SetHP(my->GetMaxHP() - my->GetCollectScore());

				if (
					//my->GetPos().x > you->GetPos().x// �ʒu�֌W�ɂ��x�N�g��
					my->GetDir() == DIR::LEFT			// ���Ă��l�̌����ɂ��x�N�g��
					) (*OutDamageInfo)->FlyVector.x *= -1;
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

				// �J�����ɐU�����b�Z�[�W�𑗂�
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &my->GetAttackData()->ShakeCameraInfo);
			}
		}
	}
}

bool PlayerManager::CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou)
{
	// �`�[����������I
	if (pStand->GetPlayer()->GetSide() == pYou->GetSide()) return false;

	// �X�^���h���A�N�e�B�u����Ȃ���I
	if (!pStand->isActive()) return false;
	
	// �X�^���h���A�^�b�N�^�C�v����Ȃ���I
	if (pStand->GetAttackData() == nullptr)return false;

	// ���肪�G�X�P�[�v������I
	if (pYou->isEscape()) return false;
	
	if (pStand->isAttackFrame()) // �U���t���[�����Ȃ�
	{
		if (pYou->GetInvincibleLV() == 0 &&								// ���肪���G�łȂ�
			!pStand->GetAttackData()->bHit)									// �܂��U���𓖂ĂĂȂ�
		{
			// �U������`��Ƒ���̎l�p�Ŕ�����Ƃ�
			CollisionShape::Square AttackShape, YouShape;
			memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pStand->GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			memcpy_s(&YouShape, sizeof(CollisionShape::Square), pYou->GetHitSquare(), sizeof(CollisionShape::Square));
			if (pStand->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// ����pos�͐��+(�E)�Ȃ̂ŁA�������Ȃ�t�ɂ���
			AttackShape.pos += pStand->GetPos();
			YouShape.pos += pYou->GetPos();
			if (Collision::HitCheck(&AttackShape, &YouShape))
			{
				// �q�b�g���̏���
				//my->GetAttackData()->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
	
				/* ���b�Z�[�W���M */
	
				// ���肪�q�b�g�����Ƃ��̒n��ɂ������󒆂ɂ�����
				int iHitPlace = (pYou->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL;

				// �܂��A�U�����q�b�g�������l�ɑ��M
				//HIT_ATTACK_INFO hai;
				//hai.bOverDrive = false;
				//hai.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// �������g�ɂ��̎����̃q�b�g�X�g�b�v
				//hai.HitScore = pStand->GetAttackData()->HitScore;				// �_���[�W(�X�R�A)
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pStand->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);
				//// �����G�t�F�N�g�ǉ�
				//Vector2 screenPos = Math::WorldToScreen(pStand->GetPos());
				//NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, pStand->GetAttackData()->damage, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

				// �����āA�_���[�W���󂯂��l�ɑ��M
				HIT_DAMAGE_INFO HitDamageInfo;
				HitDamageInfo.bOverDrive = false;// ��(��)
				HitDamageInfo.BeInvincible = pStand->GetAttackData()->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				HitDamageInfo.damage = pStand->GetAttackData()->damage;				// �_���[�W(�X�R�A)
				HitDamageInfo.FlyVector = pStand->GetAttackData()->places[iHitPlace].FlyVector;			// ������уx�N�g��
				HitDamageInfo.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
				HitDamageInfo.HitRecoveryFrame = pStand->GetAttackData()->places[iHitPlace].HitRecoveryFrame;		// �d������
				HitDamageInfo.GuardRecoveryFrame = pStand->GetAttackData()->GuardRecoveryFrame;		// �d������
				HitDamageInfo.HitEffectType = (int)pStand->GetAttackData()->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				HitDamageInfo.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTION�̂Ƃ��́A�R���{�p���֘A�̂𖳎�����
				HitDamageInfo.iAntiGuard = (int)pStand->GetAttackData()->AntiGuard;				// �K�[�h�˂��j��^�C�v
				HitDamageInfo.HitSE = pStand->GetAttackData()->HitSE;
				HitDamageInfo.bFinishOK = pStand->GetAttackData()->bFinish;

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
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, pYou->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);
	

				// �J�����ɐU�����b�Z�[�W�𑗂�
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pStand->GetAttackData()->ShakeCameraInfo);
	
				return true;	// ��������
			}
		}
	}
	return false;	// ������Ȃ�����
}

// �������̍U���ɑ΂���v���C���[�̔���
bool PlayerManager::CollisionThrowAttack(BasePlayer *my, BasePlayer *you)
{
	// �`�[����������I
	if (my->GetSide() == you->GetSide()) return false;

	// ���肪�󒆂���������߂Ȃ�
	if (!you->isLand()) return false;

	// �������łɒ͂�ł��I
	if (my->isThrowSuccess()) return false;

	// ���肪���G���
	if (you->GetInvincibleLV() > my->GetAttackData()->pierceLV) return false;

	// �����P�\�߂�����I
	if (
		//my->GetCurrentFrame() >= BasePlayer::c_THROW_ESCAPE_FRAME
		my->GetActionFrame() != FRAME_STATE::ACTIVE
		) return false;

	// ����������
	Vector3 v(you->GetPos() - my->GetPos());
	if (v.x * v.x + v.y * v.y < 10 * 10 &&
		((v.x > 0 && my->GetDir() == DIR::RIGHT) || (v.x < 0 && my->GetDir() == DIR::LEFT)))
	{
		// �������ł��܂ꂽ����̍��W�����񂾑���̒͂ݍ��W�Ɉړ�(���ʂɑ������ƕǔ�������̂ŁA�ǔ��������낱���ł���Ă��܂����Ƃ�����)
		if (my->GetDir() == DIR::LEFT  && my->isPushInput(PLAYER_COMMAND_BIT::RIGHT) ||
			my->GetDir() == DIR::RIGHT && my->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		{
			// �������t�ɂ���
			my->ReverseDir();

			// ����̍��W�Ǝ����̍��W�����ւ���
			Vector3 temp(my->GetPos());
			my->SetPos(you->GetPos());
			you->SetPos(temp);
		}

		const Vector3 p(my->GetPos());
		Vector3 v(my->GetFlontPos() - p), v2(v);

		m_pStage->Collision(my, &v);	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		if (fabsf(v.x) == 0)
		{
			my->GetPosAddress()->x -= v2.x;
			you->SetPos(my->GetPos() + v2);
		}
		else you->SetPos(p + v);

		/* ���b�Z�[�W���M */

		// �͂ݐ���������ƃ��b�Z�[�W�𑗂�
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, my->GetID(), MESSAGE_TYPE::THROW_SUCCESS, nullptr);

		// �͂܂ꂽ���b�Z�[�W
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::BE_THROWN, nullptr);

		// �͂�SE�������ōĐ�
		se->Play("�͂ݐ���");

		return true;
	}

	return false;
}

void PlayerManager::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo)
{
	/* ���b�Z�[�W���M */

	// �_���[�W�󂯂��惁�b�Z�[�W
	MsgMgr->Dispatch(0, pAttackPlayer->GetID(), pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
		m_bHeaveHoDriveOverFlow = true;
		break;
	}
	return false;
}

// �`�[���|�C���g�v�Z
void PlayerManager::CalcTeamPoint()
{
	m_PointAteam = 0;
	m_PointBteam = 0;
	
	// �v���C���[�����`��
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->GetSide()==SIDE::LEFT)
		{
			m_PointAteam += m_pPlayers[i]->GetScore();
		}
		else
		{
			m_PointBteam += m_pPlayers[i]->GetScore();
		}	
	}


}

void PlayerManager::UpdatePlayerTargetDir(BasePlayer *my, BasePlayer *you)
{
	const Vector3 v(you->GetPos() - my->GetPos());
	if (v.x > 0)
	{
		my->SetTargetDir(DIR::RIGHT);
		you->SetTargetDir(DIR::LEFT);
	}
	else if (v.x < 0)
	{
		my->SetTargetDir(DIR::LEFT);
		you->SetTargetDir(DIR::RIGHT);
	}
}