#include "PlayerManager.h"
#include "Airou\Airou.h"
#include "Stage\Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

// ���̂̐錾
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// �G���e�B�e�BID�o�^
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr)
{

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
		m_pPlayers[i] = new Airou(i, (i == 3));
		m_pPlayers[i]->InitActionDatas();		// ���U�������e�L�����ɏ�����������
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
	/* �v���C���[�����X�V */
	FOR(m_NumPlayer)
	{
		m_pPlayers[i]->Update();			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
	}

	/* �v���C���[VS�v���C���[�̍U������ */
	bool *bHit = new bool[m_NumPlayer];					// �����q�b�g�p
	memset(bHit, false, sizeof(bool) * m_NumPlayer);	// false�ŏ�����
	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			bool receive;	// ���茋�ʎ���p�ϐ�

			receive = CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you]);
			bHit[my] = (bHit[my]) ? true : receive;		// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���

			receive = CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my]);
			bHit[you] = (bHit[you]) ? true : receive;	// �������Ă���A���̌�̔��茋�ʂƂ��C�ɂ��Ȃ��Ă���
		}
	}
	// �U�����ʊm��
	FOR(m_NumPlayer) if(bHit[i]) m_pPlayers[i]->GetAttackData()->bHit = true;	// 2�d�q�b�g�h�~�p�̃t���O��ON�ɂ���
	delete[] bHit;	// �|�C���^�z��̉��


	/* �ʒu���m�� */
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i]);	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V
	}
}

void PlayerManager::Render()
{
	// �v���C���[�����`��
	FOR(m_NumPlayer) m_pPlayers[i]->Render();
}

bool PlayerManager::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you)
{
	// �U���n�̃X�e�[�g����Ȃ���I
	if (!my->isAttackState()) return false;

	// ���肪�G�X�P�[�v������I
	if (you->isEscape()) return false;

	if (my->isActiveFrame()) // �U���t���[�����Ȃ�
	{
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

				// �܂��A�U�����q�b�g�������l�ɑ��M
				HIT_ATTACK_INFO hai;
				hai.HitPlayerDeviceID = you->GetDeviceID();					// �_���[�W��^��������̔ԍ�
				hai.hitStopFlame = my->GetAttackData()->hitStopFlame;		// �������g�ɂ��̎����̃q�b�g�X�g�b�v
				hai.HitScore = my->GetAttackData()->HitScore;				// �_���[�W(�X�R�A)
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + my->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);

				// �����āA�_���[�W���󂯂��l�ɑ��M
				HIT_DAMAGE_INFO hdi;
				hdi.BeInvincible = my->GetAttackData()->bBeInvincible;	// ���G�ɂȂ邩�ǂ���
				hdi.damage = my->GetAttackData()->damage;				// �_���[�W(�X�R�A)
				hdi.FlyVector = (you->isLand()) ? my->GetAttackData()->LandFlyVector : my->GetAttackData()->AerialFlyVector;			// ������уx�N�g��
				hdi.hitStopFlame = my->GetAttackData()->hitStopFlame;		// �q�b�g�X�g�b�v
				hdi.recoveryFlame = my->GetAttackData()->recoveryFlame;		// �d������
				hdi.effectType = (int)my->GetAttackData()->effectType;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���

				if (my->GetPos().x > you->GetPos().x) hdi.FlyVector.x *= -1;
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + you->GetDeviceID()), MESSAGE_TYPE::HIT_DAMAGE, &hdi);

				// ���o��
				LPCSTR seID = my->GetAttackData()->HitSE;
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