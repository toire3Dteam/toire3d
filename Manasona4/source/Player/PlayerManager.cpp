#include "PlayerManager.h"
#include "Airou/Airou.h"
#include "Nazenara/Nazenara.h"
#include "Teki/Teki.h"
#include "Aramitama/Aramitama.h"
#include "Aniki/Aniki.h"
#include "Stage/Stage.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stand/Stand.h"
#include "../Number/Number.h"
#include "../Collision/Collision.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "UI\GameUI.h"


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

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage, SideData SideDatas[(int)SIDE::ARRAY_MAX], int iRoundNum)
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
		switch (SideDatas[(int)side].eCharacter)
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
		case CHARACTER::ARAMITAMA:
			m_pPlayers[i] = new Aramitama(side, SideDatas[(int)side]);
			break;
		case CHARACTER::ANIKI:
			m_pPlayers[i] = new Aniki(side, SideDatas[(int)side]);
			break;
		default:
			m_pPlayers[i] = new Airou(side, SideDatas[(int)side]);
			//MyAssert(false,"����ȃL�����N�^�[�͂��Ȃ�")
			break;
		}
		m_pPlayers[i]->InitActionDatas();		// ���U�������e�L�����ɏ�����������
		m_pPlayers[i]->InitMotionDatas();		// �e�L�������Ƃ̃��[�V�����ԍ�
	}

	// �L����������Ă���2P�̐F��ς���
	if (SideDatas[(int)SIDE::LEFT].eCharacter== SideDatas[(int)SIDE::RIGHT].eCharacter)
	{
		m_pPlayers[(int)SIDE::RIGHT]->ChangeColor();
	}


	// �v���C���[��2�l�Ƃ����O��̃R�[�h
	assert(NumPlayer == 2);
	m_pPlayers[0]->SetTargetPlayer(m_pPlayers[1]);
	m_pPlayers[1]->SetTargetPlayer(m_pPlayers[0]);

	if (iRoundNum <= 1)
	{
		m_pPlayers[0]->SetLastOneWin();
		m_pPlayers[1]->SetLastOneWin();
	}

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

	m_pDamageInfoPlate = new tdn2DObj("Data/UI/Game/Training/DamageInfoPlate.png");


}

PlayerManager::~PlayerManager()
{
	// �_�u���|�C���^�[�̊J��
	FOR(m_NumPlayer)
	{
		SAFE_DELETE(m_pPlayers[i]);
	}
	delete[] m_pPlayers;

	SAFE_DELETE(m_pDamageInfoPlate);
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

// �g���[�j���O�E���K�̎��̂ݍX�V
void PlayerManager::UpdateTraining()
{
	// (��)
	//PlayerDataMgr->m_TrainingData.iHpRecovery = (int)HP_RECOVERY_TYPE::AUTO_RECOVERY;
	//PlayerDataMgr->m_TrainingData.iHp1P = 50;
	//PlayerDataMgr->m_TrainingData.iHp2P = 75;
	//PlayerDataMgr->m_TrainingData.iSpGage = 50;
	//PlayerDataMgr->m_TrainingData.iPartnerRecovery= (int)PARTNER_RECOVERY_TYPE::MAX;
	//PlayerDataMgr->m_TrainingData.iInfo = (int)TRAINING_INFO_TYPE::DAMEGE;

	// AI�ɑ΂��Ă̖���
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->isAI() == true)
		{
			m_pPlayers[i]->GetAI()->UpdateTraining();
		}
	}

	// �����񕜂��ǂ���
	if (SelectDataMgr->Get()->tagTrainingDatas.eHpRecovery == HP_RECOVERY_TYPE::AUTO_RECOVERY)
	{

		FOR(m_NumPlayer)
		{
			// �d�����Ԃ��Ȃ���Ή�
			if (m_pPlayers[i]->GetRecoveryFrame() <= 0)
			{

				m_pPlayers[i]->AddHP(100);

			}
		}

	}


	// �ݒ肵���ő�HP��葽�������猸�炷
	float fHpPersent1P = (float)(SelectDataMgr->Get()->tagTrainingDatas.iHp1P) / (float)(100);
	float fHpPersent2P = (float)(SelectDataMgr->Get()->tagTrainingDatas.iHp2P) / (float)(100);
	float fTrainingMaxHp1P = m_pPlayers[(int)SIDE::LEFT]->GetMaxHP()*fHpPersent1P;
	float fTrainingMaxHp2P = m_pPlayers[(int)SIDE::RIGHT]->GetMaxHP()*fHpPersent2P;

	// ���̍ő�HP
	if (m_pPlayers[(int)SIDE::LEFT]->GetHP() > m_pPlayers[(int)SIDE::LEFT]->GetMaxHP()*fHpPersent1P)
	{
		m_pPlayers[(int)SIDE::LEFT]->SetHP((int)fTrainingMaxHp1P);
	};
	// �E�̍ő�HP
	if (m_pPlayers[(int)SIDE::RIGHT]->GetHP() > m_pPlayers[(int)SIDE::RIGHT]->GetMaxHP()*fHpPersent2P)
	{
		m_pPlayers[(int)SIDE::RIGHT]->SetHP((int)fTrainingMaxHp2P);
	};


	// �ݒ肵��SP�Q�[�W��菭�Ȃ�������ݒ肵���l��
	// ���݂��̍d������������
	if (m_pPlayers[(int)SIDE::LEFT]->GetRecoveryFrame() <= 0 &&
		m_pPlayers[(int)SIDE::RIGHT]->GetRecoveryFrame() <= 0)
	{
		FOR(m_NumPlayer)
		{
			// �������U�����ĂȂ��Ƃ� + �p�[�g�i�[���U�����Ă��Ȃ�
			if (m_pPlayers[i]->isAttackState() == false && 
				m_pPlayers[i]->GetStand()->isActive() == false)
			{
				if (m_pPlayers[i]->GetOverDriveGage() < SelectDataMgr->Get()->tagTrainingDatas.iSpGage)
				{
					m_pPlayers[i]->SetOverDriveGage((float)SelectDataMgr->Get()->tagTrainingDatas.iSpGage);
				}
			}
		}
	};
	

	// �p�[�g�i�[�Q�[�W��MAX�Œ�ɂ��Ă�����
	if (SelectDataMgr->Get()->tagTrainingDatas.ePartnerRecovery == PARTNER_RECOVERY_TYPE::MAX)
	{
		// ���݂��̍d������������
		if (m_pPlayers[(int)SIDE::LEFT]->GetRecoveryFrame() <= 0 &&
			m_pPlayers[(int)SIDE::RIGHT]->GetRecoveryFrame() <= 0)
		{
			FOR(m_NumPlayer)
			{
				// �������U�����ĂȂ��Ƃ� + �p�[�g�i�[���U�����Ă��Ȃ�
				if (m_pPlayers[i]->isAttackState() == false &&
					m_pPlayers[i]->GetStand()->isActive() == false)
				{	
					if (m_pPlayers[i]->GetStand()->GetStandGage() < m_pPlayers[i]->GetStand()->GetStandGageMAX())
					{
						m_pPlayers[i]->GetStand()->SetStandGage(m_pPlayers[i]->GetStand()->GetStandGageMAX());
					}
				}
			}
		};
	}


	// �L�����N�^�[�ʂ̐ݒ�
	FOR(m_NumPlayer)
	{
		// �����A���~�^�}��������
		if (m_pPlayers[i]->GetCharacterType() == CHARACTER::ARAMITAMA)
		{
			AramitamaDesk l_tagDesk;
			l_tagDesk.iWassyoiGauge = 0;
			// ��ɂ�����傢�Q�[�W�ő�Ȃ�
			if (SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType == WASSYOI_TYPE::MAX)
			{
				l_tagDesk.iWassyoiGauge = Aramitama::c_WASSYOIGAUGE_MAX;
				m_pPlayers[i]->SetCharacterData((void*)&l_tagDesk);
			}


		}
	}



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
		{
			m_pPlayers[i]->Render();
#ifdef _DEBUG
			m_pPlayers[i]->RenderDebug();
#endif
		}
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

void PlayerManager::RenderTraining()
{
	// �S���N
	SIDE eOrderSide = SelectDataMgr->Get()->eOrderTrainingSide;
	SIDE eTargetSide = m_pPlayers[(int)eOrderSide]->GetTargetPlayer()->GetSide();

	// �_���[�W�t�H���g�z�u
	int iFontX = 200;
	int iFontY = 120;
	if (SelectDataMgr->Get()->eOrderTrainingSide == SIDE::RIGHT)
	{
		iFontX = 750;
		iFontY = 120;
	}


	// �S�Ă̏���`��
	if (SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo == TRAINING_INFO_TYPE::ALL)
	{
		// �R�}���h����
		m_pPlayers[(int)eOrderSide]->RenderCommandFrame(10, 625);

		// �_���[�W�̃C���t�H���[�V�����̗��ɓY����v���[�g
		m_pDamageInfoPlate->Render(iFontX - 16, iFontY - 8);

		std::string pStr = "�Ȃ�";
		// �ꔭ�_���[�W
		pStr = "�_���[�W�@�@�@�@�@      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetNowDamage());
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX + 2, iFontY + 2 , 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);
			
		// �R���{�_���[�W
		pStr = "�R���{�_���[�W �@�@     ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetComboDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

		// �ő�_���[�W
		pStr = "�ő�R���{�_���[�W      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetMaxDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

	}
	else if (SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo == TRAINING_INFO_TYPE::COMMAND)
	{
		// �R�}���h����
		m_pPlayers[(int)eOrderSide]->RenderCommandFrame(10, 625);

	}
	else if (SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo == TRAINING_INFO_TYPE::DAMEGE)
	{

		// �_���[�W�̃C���t�H���[�V�����̗��ɓY����v���[�g
		m_pDamageInfoPlate->Render(iFontX - 16, iFontY - 8);

		std::string pStr = "�Ȃ�";
		// �ꔭ�_���[�W
		pStr = "�_���[�W�@�@�@�@�@      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetNowDamage());
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

		// �R���{�_���[�W
		pStr = "�R���{�_���[�W �@�@     ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetComboDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

		// �ő�_���[�W
		pStr = "�ő�R���{�_���[�W      ";
		pStr += std::to_string(GameUIMgr->GetComboUI(eTargetSide)->GetMaxDamage());
		iFontY += 40;
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX + 2, iFontY + 2, 0xaa000000, RS::COPY);
		tdnFont::RenderString(pStr.c_str(), "HGS�n�p�p�޼��UB", 24, iFontX, iFontY, 0xffeeeeee, RS::COPY);

	}


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