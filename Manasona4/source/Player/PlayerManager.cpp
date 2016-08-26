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
		// �`�[�������͉��ŐU�蕪��!!
		TEAM team;
		if (i % 2 == 0)
		{
			team = TEAM::A;
		}
		else
		{
			team = TEAM::B;
		}

		m_pPlayers[i] = new Airou(i, team, (/*i == 3|| i == 2 || i == 0 ||*/ i == 1));
		m_pPlayers[i]->InitActionDatas();		// ���U�������e�L�����ɏ�����������
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

void PlayerManager::Update()
{	
	// �N����1More�o�����Ă�����S���̓������~�߂�
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == true)
		{
			m_pPlayers[i]->Update();			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
			
			m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
			m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V

			// ���Ó]����
			 m_OverDriveDim = max(m_OverDriveDim-40, 64);	

			return; // �ق��̃L�����N�^�[�̏��ւ͒f���Ēʂ��Ȃ�
		}
		
	}
	// �N���o�����Ă��Ȃ������̂Œʏ�ʂ�X�V
	
	// ���Ó]����
	m_OverDriveDim=min(m_OverDriveDim+40, 255);
	
	

	/* �v���C���[�����X�V */
	FOR(m_NumPlayer)
	{
		m_pPlayers[i]->Update();			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
	}

	/* �v���C���[VS�v���C���[�̍U������ */
	//bool bHit[m_NumPlayer];
	bool *bHit = new bool[m_NumPlayer];					// �����q�b�g�p
	bool *bHitStand = new bool[m_NumPlayer];
	memset(bHit, false, sizeof(bool) * m_NumPlayer);	// false�ŏ�����
	memset(bHitStand, false, sizeof(bool)* m_NumPlayer);	// false�ŏ�����
	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			m_pStage->Sinking(m_pPlayers[my], m_pPlayers[you]);	// �߂荞�ݔ���

			bool receive;	// ���茋�ʎ���p�ϐ�

			// ( my->you you->my )�܂���݂ɓ������Ă��Ȃ����`�F�b�N����

			/* �v���C���[VS�v���C���[�̍U������ */
			receive = CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you]);
			bHit[my] = (bHit[my]) ? true : receive;		// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���
			receive = CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my]);
			bHit[you] = (bHit[you]) ? true : receive;	// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���

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
		if (bHit[i]) if(m_pPlayers[i]->isAttackState()) m_pPlayers[i]->GetAttackData()->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
		if (bHitStand[i]) m_pPlayers[i]->GetStand()->GetAttackData()->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
	}


	/* �ʒu���m�� */
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V
	}


	delete[] bHit;	// �|�C���^�z��̉��
	delete[] bHitStand;


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

bool PlayerManager::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you)
{
	// �`�[����������I
	if (my->GetTeam() == you->GetTeam()) return false;

	// �U���n�̃X�e�[�g����Ȃ���I
	if (!my->isAttackState()) return false;

	// ���肪�G�X�P�[�v������I
	if (you->isEscape()) return false;

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
				if (my->isLand() == false)return false;
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
				int iHitPlace = (you->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL;

				// �t�B�j�b�V���A�[�c���ǂ���
				bool bFinish = (my->GetActionState() == BASE_ACTION_STATE::FINISH);

				// �܂��A�U�����q�b�g�������l�ɑ��M
				HIT_ATTACK_INFO HitAttackInfo;
				HitAttackInfo.HitPlayerDeviceID = you->GetDeviceID();									// �_���[�W��^��������̔ԍ�
				HitAttackInfo.hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// �������g�ɂ��̎����̃q�b�g�X�g�b�v
				HitAttackInfo.HitScore = my->GetAttackData()->HitScore;									// �_���[�W(�X�R�A)
				HitAttackInfo.bFinishAttack = bFinish;													// �ӂ��ɂ���A�[�c���ǂ���
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + my->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &HitAttackInfo);
				// �����G�t�F�N�g�ǉ�
				Vector2 screenPos= Math::WorldToScreen(my->GetPos());
				NumberEffect.AddNumber(screenPos.x, screenPos.y - 200, my->GetAttackData()->HitScore, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);


				// �����āA�_���[�W���󂯂��l�ɑ��M
				HIT_DAMAGE_INFO HitDamageInfo;
				HitDamageInfo.BeInvincible = my->GetAttackData()->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				HitDamageInfo.damage = my->GetAttackData()->damage;				// �_���[�W(�X�R�A)
				HitDamageInfo.FlyVector = my->GetAttackData()->places[iHitPlace].FlyVector;			// ������уx�N�g��
				HitDamageInfo.hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
				HitDamageInfo.recoveryFlame = my->GetAttackData()->places[iHitPlace].recoveryFlame;		// �d������
				HitDamageInfo.HitEffectType = (int)my->GetAttackData()->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				HitDamageInfo.iAttackType = (int)my->GetActionState();						// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
				HitDamageInfo.bFinishAttack = bFinish;										// �ӂ��ɂ���A�[�c���ǂ���
				if (
					//my->GetPos().x > you->GetPos().x// �ʒu�֌W�ɂ��x�N�g��
					my->GetDir() == DIR::LEFT			// ���Ă��l�̌����ɂ��x�N�g��
					) HitDamageInfo.FlyVector.x *= -1;
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + you->GetDeviceID()), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

				// �J�����ɐU�����b�Z�[�W�𑗂�
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &my->GetAttackData()->ShakeCameraInfo);

				// ���o��
				LPCSTR seID = my->GetAttackData()->HitSE;
				if (seID) se->Play((LPSTR)seID);

				return true;	// ��������
			}
		}
	}
	return false;	// ������Ȃ�����
}

bool PlayerManager::CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou)
{
	// �`�[����������I
	if (pStand->GetPlayer()->GetTeam() == pYou->GetTeam()) return false;

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
				hai.HitPlayerDeviceID = pYou->GetDeviceID();					// �_���[�W��^��������̔ԍ�
				hai.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// �������g�ɂ��̎����̃q�b�g�X�g�b�v
				hai.HitScore = pStand->GetAttackData()->HitScore;				// �_���[�W(�X�R�A)
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pStand->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);
				// �����G�t�F�N�g�ǉ�
				Vector2 screenPos = Math::WorldToScreen(pStand->GetPos());
				NumberEffect.AddNumber(screenPos.x, screenPos.y - 200, pStand->GetAttackData()->HitScore, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

				// �����āA�_���[�W���󂯂��l�ɑ��M
				HIT_DAMAGE_INFO hdi;
				hdi.BeInvincible = pStand->GetAttackData()->places[iHitPlace].bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				hdi.damage = pStand->GetAttackData()->damage;				// �_���[�W(�X�R�A)
				hdi.FlyVector = pStand->GetAttackData()->places[iHitPlace].FlyVector;			// ������уx�N�g��
				hdi.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// �q�b�g�X�g�b�v
				hdi.recoveryFlame = pStand->GetAttackData()->places[iHitPlace].recoveryFlame;		// �d������
				hdi.HitEffectType = (int)pStand->GetAttackData()->HitEffectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				hdi.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTION�̂Ƃ��́A�R���{�p���֘A�̂𖳎�����
				if (
					//my->GetPos().x > you->GetPos().x// �ʒu�֌W�ɂ��x�N�g��
					pStand->GetDir() == DIR::LEFT			// ���Ă��l�̌����ɂ��x�N�g��
					) hdi.FlyVector.x *= -1;
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pYou->GetDeviceID()), MESSAGE_TYPE::HIT_DAMAGE, &hdi);
	

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
		if (m_pPlayers[i]->GetTeam()==TEAM::A)
		{
			m_PointAteam += m_pPlayers[i]->GetScore();
		}
		else
		{
			m_PointBteam += m_pPlayers[i]->GetScore();
		}	
	}


}
