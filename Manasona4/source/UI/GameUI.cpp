#include "GameUI.h"
#include "../Number/Number.h"
#include "../Player/PlayerManager.h"

// ���̂̐錾
GameUIManager *GameUIManager::pInstance = nullptr;

/***************/
//	�������E���
/***************/
GameUIManager::GameUIManager() :BaseGameEntity(ENTITY_ID::UI_MGR)	// �G���e�B�e�BID�o�^
{
	m_pHpFrame = new tdn2DAnim("Data/UI/Game/HPFrame.png");
	m_pHpFrame->OrderMoveAppeared(18, 0, -100);

	m_pSpFrame = new tdn2DAnim("Data/UI/Game/SPFrame2.png");
	m_pSpFrame->OrderMoveAppeared(16, 0, -80);
	
	m_pHpGage1P = nullptr;
	m_pHpGage2P = nullptr;

	m_pSpGage1P = nullptr;
	m_pSpGage2P = nullptr;

	m_pPersona1P = nullptr;
	m_pPersona2P = nullptr;

	m_pRoundIcon1P = nullptr;
	m_pRoundIcon2P = nullptr;

	m_pFacePic1P = nullptr;
	m_pFacePic2P = nullptr;

	m_pOverDriveCutin1P = nullptr;
	m_pOverDriveCutin2P = nullptr;
	
	m_pCombo1P = nullptr;
	m_pCombo2P = nullptr;

	m_pReferencesRightPlayer = nullptr;
	m_pReferencesLeftPlayer = nullptr;

	m_pTimer = new Timer();

	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(1280, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	}

}


GameUIManager::~GameUIManager()
{
	SAFE_DELETE(m_pHpFrame);
	SAFE_DELETE(m_pSpFrame);

	SAFE_DELETE(m_pHpGage1P);
	SAFE_DELETE(m_pHpGage2P);

	SAFE_DELETE(m_pSpGage1P);
	SAFE_DELETE(m_pSpGage2P);

	SAFE_DELETE(m_pPersona1P);
	SAFE_DELETE(m_pPersona2P);

	SAFE_DELETE(m_pRoundIcon1P);	
	SAFE_DELETE(m_pRoundIcon2P);

	SAFE_DELETE(m_pFacePic1P);
	SAFE_DELETE(m_pFacePic2P);

	SAFE_DELETE(m_pOverDriveCutin1P);
	SAFE_DELETE(m_pOverDriveCutin2P);

	SAFE_RELEASE(m_pStencilSurface);

	SAFE_DELETE(m_pCombo1P);
	SAFE_DELETE(m_pCombo2P);

	SAFE_DELETE(m_pTimer);


}

void GameUIManager::InitData(BasePlayer* pLeftPlayer, BasePlayer* pRightPlayer, int iRoundNum, ROUND_TIME_TYPE eRoundTimeType)
{
	m_pHpGage1P = new HpGage(pLeftPlayer);
	m_pHpGage2P = new HpGage(pRightPlayer);

	m_pSpGage1P = new SpGage(pLeftPlayer);
	m_pSpGage2P = new SpGage(pRightPlayer);

	m_pPersona1P = new PersonaUI(pLeftPlayer);
	m_pPersona2P = new PersonaUI(pRightPlayer);

	m_pRoundIcon1P = new RoundIcon(pLeftPlayer, iRoundNum);
	m_pRoundIcon2P = new RoundIcon(pRightPlayer, iRoundNum);

	m_pFacePic1P = new FacePic(pLeftPlayer);
	m_pFacePic2P = new FacePic(pRightPlayer);

	m_pOverDriveCutin1P = new OverDriveCutin(pLeftPlayer);
	m_pOverDriveCutin2P = new OverDriveCutin(pRightPlayer);

	m_pCombo1P = new ComboUI(pLeftPlayer);
	m_pCombo2P = new ComboUI(pRightPlayer);

	//
	m_pReferencesLeftPlayer  = pLeftPlayer;
	m_pReferencesRightPlayer = pRightPlayer;

	m_pTimer->SetTimerType(eRoundTimeType);
}

void GameUIManager::Update()
{
	if (m_pHpGage1P == nullptr)MyAssert(0,"�v���C���[�f�[�^��n���Ă���");

	m_pHpFrame->Update();
	m_pSpFrame->Update();

	// HP�Q�[�W
	m_pHpGage1P->Update();
	m_pHpGage2P->Update();

	// SP�Q�[�W
	m_pSpGage1P->Update();
	m_pSpGage2P->Update();

	// �y���\�iUI
	m_pPersona1P->Update();
	m_pPersona2P->Update();

	// ���E���h�A�C�R��
	m_pRoundIcon1P->Update();
	m_pRoundIcon2P->Update();

	// ��O��
	m_pFacePic1P->Update();
	m_pFacePic2P->Update();

	// �I�[�o�[�h���C�u�p�̃J�b�g�C���X�V
	m_pOverDriveCutin1P->Update();
	m_pOverDriveCutin2P->Update();

	// �R���{
	m_pCombo1P->Update();
	m_pCombo2P->Update();

	// �^�C�}�[
	m_pTimer->Update();

}

void GameUIManager::Action() 
{
	m_pHpFrame->Action(2);
	m_pSpFrame->Action(16);

	// HP�Q�[�W
	m_pHpGage1P->FirstAction(18);
	m_pHpGage2P->FirstAction(18);

	// �y���\�iUI
	m_pPersona1P->Action(18);
	m_pPersona2P->Action(18);

	// ���E���h�A�C�R��
	m_pRoundIcon1P->Action(18);
	m_pRoundIcon2P->Action(18);

	// ��O��
	m_pFacePic1P->Action(24);
	m_pFacePic2P->Action(24);

	// SP�Q�[�W
	m_pSpGage1P->Action(26);
	m_pSpGage2P->Action(26);

	// �^�C�}�[
	m_pTimer->Action();

	// ��UI
	m_pReferencesLeftPlayer->CharacterDataUIAction(26);
	m_pReferencesRightPlayer->CharacterDataUIAction(26);
}

void GameUIManager::Render()
{
	if (m_pHpGage1P == nullptr)MyAssert(0, "�v���C���[�f�[�^��n���Ă���");

	m_pHpFrame->Render(0, 0);
	m_pSpFrame->Render(0, 0);
	
	// HP�Q�[�W
	m_pHpGage1P->Render();
	m_pHpGage2P->Render();

	// SP�Q�[�W
	m_pSpGage1P->Render();
	m_pSpGage2P->Render();

	// ��O��
	m_pFacePic1P->Render();
	m_pFacePic2P->Render();

	// �y���\�i
	m_pPersona1P->Render();
	m_pPersona2P->Render();

	// ���E���h�A�C�R��
	m_pRoundIcon1P->Render();
	m_pRoundIcon2P->Render();

	// �^�C�}�[
	m_pTimer->Render();

	// ��UI�`��
	m_pReferencesLeftPlayer->CharacterDataUIRender();
	m_pReferencesRightPlayer->CharacterDataUIRender();

	//tdnText::Draw(350,70,0xffff00ff,"%d", PlayerMgr->GetPointA());
	//tdnText::Draw(900, 70, 0xffff00ff, "%d", PlayerMgr->GetPointB());
}

void GameUIManager::RenderBack()
{
	Surface* saveZ;
	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);
	// Z�o�b�t�@��؂�ւ�
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);

	// �I�[�o�[�h���C�u�p�̃J�b�g�C���`��
	m_pOverDriveCutin1P->Render();
	m_pOverDriveCutin2P->Render();

	// �R���{
	m_pCombo1P->Render();
	m_pCombo2P->Render();

	tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ); //�X�e���V���o�b�t�@�̕���

}

// �^�C�}�[�֘A
void GameUIManager::TimerReset()
{
	m_pTimer->Reset();
}

void GameUIManager::TimerStart()
{
	m_pTimer->Start();
}

void GameUIManager::TimerStop()
{
	m_pTimer->Stop();
}

bool GameUIManager::isTimerUp()
{
	return m_pTimer->isTimeUp();
}

void GameUIManager::Reset()
{
	TimerReset();

	m_pPersona1P->DownEffectAction();
	m_pPersona2P->DownEffectAction();

	m_pOverDriveCutin1P->Stop();
	m_pOverDriveCutin2P->Stop();
}

bool GameUIManager::HandleMessage(const Message& msg)
{
	// ���ꂳ������΂Ȃ�ł��ł���

	switch (msg.Msg)
	{
	case PERSONA_CARD_COUNT_UP:
	{
		SIDE *data = (SIDE*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		SIDE side = *data;

		if (side == SIDE::LEFT)
		{
			m_pPersona1P->UpEffectAction();
		}
		else
		{
			m_pPersona2P->UpEffectAction();
		}
		return true;
	}
		break;
	case PERSONA_CARD_COUNT_DOWN:
	{
		SIDE *data = (SIDE*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		SIDE side = *data;

		if (side == SIDE::LEFT)
		{
			m_pPersona1P->DownEffectAction();
		}
		else
		{
			m_pPersona2P->DownEffectAction();
		}
		return true;
	}
		break;
	case APP_WIN_ICON:
	{
		SIDE *data = (SIDE*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		SIDE side = *data;

		if (side == SIDE::LEFT)
		{
			m_pRoundIcon1P->AppIcon();
		}
		else
		{
			m_pRoundIcon2P->AppIcon();
		}
		return true;
	}
		break;
	case OVER_DRIVE_CUTIN:
	{	
		// �K�E�J�b�g�C��
			SIDE *data = (SIDE*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
			SIDE side = *data;

			if (side == SIDE::LEFT)
			{
				m_pOverDriveCutin1P->Action();
			}
			else
			{
				m_pOverDriveCutin2P->Action();
			}
			return true;
	
	}	break;
	case MESSAGE_TYPE::COMBO_COUNT:
	{
		// �R���{�p
		COMBO_DESK *data;
		data = (COMBO_DESK*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��

		SIDE side = data->side;

		if (side == SIDE::LEFT)
		{
			m_pCombo1P->Count(data->damage, data->recoveryFrame, data->bCounter);
		}else
		{
			m_pCombo2P->Count(data->damage, data->recoveryFrame, data->bCounter);
		}

		return true;

	}	break;
	case MESSAGE_TYPE::COMBO_GUARD:
	{
		// �R���{�p
		COMBO_DESK *data;
		data = (COMBO_DESK*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��

		SIDE side = data->side;

		if (side == SIDE::LEFT)
		{
			m_pCombo1P->Guard();
		}
		else
		{
			m_pCombo2P->Guard();
		}
		return true;

	}	break;
	default:
		break;
	}

	return false;
}
