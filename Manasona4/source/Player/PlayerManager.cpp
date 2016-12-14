#include "PlayerManager.h"
#include "Airou/Airou.h"
#include "Nazenara/Nazenara.h"
#include "Teki\Teki.h"
#include "Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stand/Stand.h"
#include "../Number/Number.h"
#include "../Collision/Collision.h"

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
		// (TODO)�`�[�������͉��ŐU�蕪��!! 2016/10.04���@�����E��Team�ŕς��Ă�
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// �L�����N�^�[�ŕ��򂷂�
		switch (SideDatas[(int)side].character)
		{
		case CHARACTER::AIROU:
			m_pPlayers[i] = new Airou(side, SideDatas[(int)side]);
			break;
		case CHARACTER::TEKI:
			m_pPlayers[i] = new Teki(side, SideDatas[(int)side]);
			break;
		case CHARACTER::NAZENARA:
			m_pPlayers[i] = new Nazenara(side, SideDatas[(int)side]);
			break;
		default:
			m_pPlayers[i] = new Airou(side, SideDatas[(int)side]);
			//MyAssert(false,"����ȃL�����N�^�[�͂��Ȃ�")
			break;
		}
		m_pPlayers[i]->InitActionDatas();		// ���U�������e�L�����ɏ�����������
		m_pPlayers[i]->InitMotionDatas();		// �e�L�������Ƃ̃��[�V�����ԍ�
	}

	// �v���C���[��2�l�Ƃ����O��̃R�[�h
	assert(NumPlayer == 2);
	m_pPlayers[0]->SetTargetPlayer(m_pPlayers[1]);
	m_pPlayers[1]->SetTargetPlayer(m_pPlayers[0]);

	FOR(NumPlayer)
	{
		// (TODO)�`�[�������͉��ŐU�蕪��!! 2016/10.04���@�����E��Team�ŕς��Ă�
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// AI������ �Ȃ������ŏ���������Ƃ����ƁA�������̒��Ńv���C���[���Q�Ƃ��邱�Ƃ�����A�S���̏�����������������ɌĂяo����������
		m_pPlayers[i]->InitAI(SideDatas[(int)side].eAIType);
	}
}

PlayerManager::~PlayerManager()
{
	// �_�u���|�C���^�[�̊J��
	FOR(m_NumPlayer)delete m_pPlayers[i];
	delete[] m_pPlayers;
}

void PlayerManager::Update(PLAYER_UPDATE flag)
{	
	// �N����1More�o�����Ă�����S���̓������~�߂�
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->isGameTimerStopFlag() == true)
		{
			m_pPlayers[i]->Update(flag);			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
			
			//m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
			//m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V

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
		else m_pPlayers[i]->Update(flag);
	}

	// �`�[���|�C���g�v�Z
	//CalcTeamPoint();
}

void PlayerManager::UpdateHit()
{

}

void PlayerManager::UpdatePos()
{
	/* �ʒu���m�� */
	FOR(m_NumPlayer)
	{
		// ���肪�����~�߂Ă���X���[����
		if (m_pPlayers[i]->GetTargetPlayer()->isGameTimerStopFlag()) continue;

		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V
	}
}

void PlayerManager::Render()
{
	// �v���C���[�����`��
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)
			m_pPlayers[i]->RenderDrive();
		else 
			m_pPlayers[i]->Render();

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

//void PlayerManager::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo)
//{
//	/* ���b�Z�[�W���M */
//
//	// �_���[�W�󂯂��惁�b�Z�[�W
//	MsgMgr->Dispatch(0, pAttackPlayer->GetID(), pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);
//
//	// �_���[�W��^�����ɑ΂��ăq�b�g�X�g�b�v��������
//	pAttackPlayer->SetHitStopFrame(pHitDamageInfo->iHitStopFrame);
//	//pDamagePlayer->SetHitStopFrame(pHitDamageInfo->iHitStopFrame);
//}

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

//void PlayerManager::UpdatePlayerTargetDir(BasePlayer *my, BasePlayer *you)
//{
//	const Vector3 v(you->GetPos() - my->GetPos());
//	if (v.x > 0)
//	{
//		my->SetTargetDir(DIR::RIGHT);
//		you->SetTargetDir(DIR::LEFT);
//	}
//	else if (v.x < 0)
//	{
//		my->SetTargetDir(DIR::LEFT);
//		you->SetTargetDir(DIR::RIGHT);
//	}
//}