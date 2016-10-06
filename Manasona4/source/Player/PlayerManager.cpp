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
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr)
{
	m_PointAteam = 0;
	m_PointBteam = 0;

	m_OverDriveDim = 255;
}
PlayerManager::PlayerManager(const PlayerManager&) :BaseGameEntity(ENTITY_ID::PLAYER_MGR){}

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage)
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

		m_pPlayers[i] = new Airou(i, side, (i == 2 )/*(i == 1 || i == 2 || i == 3)*/);//*i == 1*/0));
		m_pPlayers[i]->InitActionDatas();		// ���U�������e�L�����ɏ�����������
		m_pPlayers[i]->InitMotionDatas();		// �e�L�������Ƃ̕K�E�Z
	}


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
		m_pPlayers[i]->Update(bControl);			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
	}

	/* �v���C���[VS�v���C���[�̍U������ */
	HIT_ATTACK_INFO **pHitAttackInfo=new HIT_ATTACK_INFO*[m_NumPlayer];
	HIT_DAMAGE_INFO **pHitDamageInfo=new HIT_DAMAGE_INFO*[m_NumPlayer];
	FOR(m_NumPlayer)
	{
		pHitAttackInfo[i] = nullptr;
		pHitDamageInfo[i] = nullptr;
	}
	bool *bHitStand = new bool[m_NumPlayer];
	memset(bHitStand, false, sizeof(bool)* m_NumPlayer);	// false�ŏ�����

	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			m_pStage->Sinking(m_pPlayers[my], m_pPlayers[you]);	// �߂荞�ݔ���

			bool receive;	// ���茋�ʎ���p�ϐ�

			// ( my->you you->my )�܂���݂ɓ������Ă��Ȃ����`�F�b�N����

			/* �v���C���[VS�v���C���[�̍U������ */
			//receive = CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you]);
			//bHit[my] = (bHit[my]) ? true : receive;		// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���
			//receive = CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my]);
			//bHit[you] = (bHit[you]) ? true : receive;	// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���
			CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you], &pHitAttackInfo[my], &pHitDamageInfo[you]);
			CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my], &pHitAttackInfo[you], &pHitDamageInfo[my]);

			/* �v���C���[VS�v���C���[�̓����͂ݔ��� */
			if (m_pPlayers[my]->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// �������������[�h�ɓ����Ă���
				CollisionThrowAttack(m_pPlayers[my], m_pPlayers[you]);	// ��������֐�
			if (m_pPlayers[you]->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// �������������[�h�ɓ����Ă���
				CollisionThrowAttack(m_pPlayers[you], m_pPlayers[my]);	// ��������֐�

			/* �X�^���hVS�v���C���[�̍U������ */
			receive = CollisionStandAttack(m_pPlayers[my]->GetStand(), m_pPlayers[you]);
			bHitStand[my] = (bHitStand[my]) ? true : receive;		// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���
			receive = CollisionStandAttack(m_pPlayers[you]->GetStand(), m_pPlayers[my]);
			bHitStand[you] = (bHitStand[you]) ? true : receive;		// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���
		}
	}
	// �v���C���[VS�v���C���[�U�����ʊm��
	FOR(m_NumPlayer)
	{
		// �������Ă���
		if (pHitAttackInfo[i])
		{
			int DamagePlayerNo(0);
			for (int j = 0; j < m_NumPlayer; j++)
			{
				if (j == i) continue;
				if (m_pPlayers[j]->GetDeviceID() == pHitAttackInfo[i]->HitPlayerDeviceID)
				{
					DamagePlayerNo = j;
					break;
				}
			}
			SendHitMessage(m_pPlayers[i], m_pPlayers[DamagePlayerNo], pHitAttackInfo[i], pHitDamageInfo[DamagePlayerNo]);
			if (m_pPlayers[i]->isAttackState()) m_pPlayers[i]->GetAttackData()->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
		}
		if (bHitStand[i]) m_pPlayers[i]->GetStand()->GetAttackData()->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
	}


	/* �ʒu���m�� */
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V
	}

	// ���
	delete[] bHitStand;
	FOR(m_NumPlayer)
	{
		if (pHitAttackInfo[i]) delete pHitAttackInfo[i];
		if (pHitDamageInfo[i]) delete pHitDamageInfo[i];
	}
	delete[] pHitAttackInfo;
	delete[] pHitDamageInfo;

	// �`�[���|�C���g�v�Z
	CalcTeamPoint();

}

void PlayerManager::Render()
{
	// �v���C���[�����`��
	FOR(m_NumPlayer) m_pPlayers[i]->Render();
}

void PlayerManager::Render(tdnShader* shader, char* name)
{
	// �v���C���[�����`��
	FOR(m_NumPlayer) m_pPlayers[i]->Render(shader, name);
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

void PlayerManager::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you, HIT_ATTACK_INFO **OutAttackInfo, HIT_DAMAGE_INFO **OutDamageInfo)
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
				bool bFinish(my->GetActionState() == BASE_ACTION_STATE::FINISH);
				
				int hitScore(my->GetAttackData()->HitScore);
				if (you->isGuard()) // ���肪�K�[�h���Ȃ�
				{
					hitScore = (int)(hitScore * 0.25f);// �X�R�A����

					// ���e��
					if (my->GetDir() == DIR::RIGHT)
					{
						my->SetMove(Vector3(-0.5f,0,0));
					}
					else
					{
						my->SetMove(Vector3(0.5f, 0, 0));
					}
				}

				// �܂��A�U�����q�b�g�������l�ɑ��M
				*OutAttackInfo = new HIT_ATTACK_INFO;
				(*OutAttackInfo)->HitPlayerDeviceID = you->GetDeviceID();									// �_���[�W��^��������̔ԍ�
				(*OutAttackInfo)->hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// �������g�ɂ��̎����̃q�b�g�X�g�b�v
				(*OutAttackInfo)->HitScore = hitScore;									// �_���[�W(�X�R�A)
				(*OutAttackInfo)->bFinishAttack = bFinish;													// �ӂ��ɂ���A�[�c���ǂ���
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, my->GetID(), MESSAGE_TYPE::HIT_ATTACK, &HitAttackInfo);
				// �����G�t�F�N�g�ǉ�
				Vector2 screenPos= Math::WorldToScreen(my->GetPos());
				NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, hitScore, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);


				// �����āA�_���[�W���󂯂��l�ɑ��M
				*OutDamageInfo = new HIT_DAMAGE_INFO;
				(*OutDamageInfo)->BeInvincible = my->GetAttackData()->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				(*OutDamageInfo)->damage = my->GetAttackData()->damage;				// �_���[�W(�X�R�A)
				(*OutDamageInfo)->FlyVector = my->GetAttackData()->places[iHitPlace].FlyVector;			// ������уx�N�g��
				(*OutDamageInfo)->hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
				(*OutDamageInfo)->recoveryFlame = my->GetAttackData()->places[iHitPlace].recoveryFlame;		// �d������
				(*OutDamageInfo)->HitEffectType = (int)my->GetAttackData()->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				(*OutDamageInfo)->iAttackType = (int)my->GetActionState();						// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
				(*OutDamageInfo)->bFinishAttack = bFinish;										// �ӂ��ɂ���A�[�c���ǂ���
				
				// ���R���{UI �G�t�F�N�g(�J�E���g)����
				if (you->isGuard() == false) you->GetComboUI()->Count(hitScore, (*OutDamageInfo)->recoveryFlame); // ���肪�K�[�h���łȂ����
				else you->GetComboUI()->Guard();// �K�[�h���ꂽ

				// ���������≼
				my->SetHP(my->GetMaxHP() - my->GetCollectScore());
				you->SetHP(my->GetMaxHP() - my->GetCollectScore());

				if (
					//my->GetPos().x > you->GetPos().x// �ʒu�֌W�ɂ��x�N�g��
					my->GetDir() == DIR::LEFT			// ���Ă��l�̌����ɂ��x�N�g��
					) (*OutDamageInfo)->FlyVector.x *= -1;
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

				// �J�����ɐU�����b�Z�[�W�𑗂�
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &my->GetAttackData()->ShakeCameraInfo);

				// ���o��
				// �K�[�h�Ȃ�SE���L���b�I�ł��ȃV���X
				LPCSTR seID = (you->isGuard() && !bFinish) ? "�K�[�h" : my->GetAttackData()->HitSE;
				if (seID) se->Play((LPSTR)seID);
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
				HIT_ATTACK_INFO hai;
				hai.bFinishAttack = false;// ��(��)
				hai.HitPlayerDeviceID = pYou->GetDeviceID();					// �_���[�W��^��������̔ԍ�
				hai.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// �������g�ɂ��̎����̃q�b�g�X�g�b�v
				hai.HitScore = pStand->GetAttackData()->HitScore;				// �_���[�W(�X�R�A)
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pStand->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);
				// �����G�t�F�N�g�ǉ�
				Vector2 screenPos = Math::WorldToScreen(pStand->GetPos());
				NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, pStand->GetAttackData()->HitScore, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

				// �����āA�_���[�W���󂯂��l�ɑ��M
				HIT_DAMAGE_INFO hdi;
				hdi.bFinishAttack = false;// ��(��)
				hdi.BeInvincible = pStand->GetAttackData()->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				hdi.damage = pStand->GetAttackData()->damage;				// �_���[�W(�X�R�A)
				hdi.FlyVector = pStand->GetAttackData()->places[iHitPlace].FlyVector;			// ������уx�N�g��
				hdi.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
				hdi.recoveryFlame = pStand->GetAttackData()->places[iHitPlace].recoveryFlame;		// �d������
				hdi.HitEffectType = (int)pStand->GetAttackData()->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				hdi.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTION�̂Ƃ��́A�R���{�p���֘A�̂𖳎�����
			
				// ���R���{UI �G�t�F�N�g(�J�E���g)����
				if (pYou->isGuard() == false) pYou->GetComboUI()->Count(hai.HitScore, hdi.recoveryFlame); // ���肪�K�[�h���łȂ����
				else pYou->GetComboUI()->Guard();// �K�[�h���ꂽ

				if (
					//my->GetPos().x > you->GetPos().x// �ʒu�֌W�ɂ��x�N�g��
					pStand->GetDir() == DIR::LEFT			// ���Ă��l�̌����ɂ��x�N�g��
					) hdi.FlyVector.x *= -1;
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, pYou->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &hdi);
	

				// �J�����ɐU�����b�Z�[�W�𑗂�
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pStand->GetAttackData()->ShakeCameraInfo);

				// ���o��
				LPCSTR seID = pStand->GetAttackData()->HitSE;
				if (seID) se->Play((LPSTR)seID);
	
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

	// �����P�\�߂�����I
	if (my->GetCurrentFrame() >= BasePlayer::c_THROW_ESCAPE_FRAME) return false;

	// ����������
	Vector3 v(you->GetPos() - my->GetPos());
	if (v.x * v.x + v.y * v.y < 10 * 10 &&
		((v.x > 0 && my->GetDir() == DIR::RIGHT) || (v.x < 0 && my->GetDir() == DIR::LEFT)))
	{
		// �������ł��܂ꂽ����̍��W�����񂾑���̒͂ݍ��W�Ɉړ�(���ʂɑ������ƕǔ�������̂ŁA�ǔ��������낱���ł���Ă��܂����Ƃ�����)
		const Vector3 p(my->GetPos());
		Vector3 v(my->GetHoldPos() - p);
		m_pStage->Collision(you, &v);	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		you->SetPos(p + v);

		/* ���b�Z�[�W���M */

		// �͂ݐ���������ƃ��b�Z�[�W�𑗂�
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, my->GetID(), MESSAGE_TYPE::THROW_SUCCESS, nullptr);

		// �����āA�͂܂ꂽ�l�ɑ��M
		BE_THROWN_INFO ThrownInfo;
		ThrownInfo.ThrowPlayerID = my->GetID();
		ThrownInfo.iThrowPlayerDir = (int)my->GetDir();
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::BE_THROWN, &ThrownInfo);

		// �͂�SE�������ōĐ�
		se->Play("�͂ݐ���");

		return true;
	}

	return false;
}

void PlayerManager::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_ATTACK_INFO *pHitAttackInfo, HIT_DAMAGE_INFO *pHitDamageInfo)
{
	/* ���b�Z�[�W���M */
	// ����Ƀ_���[�W�^�����惁�b�Z�[�W
	MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, pAttackPlayer->GetID(), MESSAGE_TYPE::HIT_ATTACK, pHitAttackInfo);

	// �_���[�W�󂯂��惁�b�Z�[�W
	MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	// ���̂Ƃ��둗���邱�Ƃ͂Ȃ��̂���
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
