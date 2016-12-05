#include "TDNLIB.h"
#include "SceneMainState.h"
#include "../BaseEntity/Message/Message.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../Stage/Stage.h"
#include "../Timer/Timer.h"
#include "../Fade/Fade.h"
#include "../system/FrameworkEx.h"
#include "sceneResult.h"
#include "../Sound/SoundManager.h"
#include "../Effect/Particle.h"
#include "RoundCall\RoundCallManager.h"
#include "../Camera/camera.h"
#include "LoadSceneThread.h"
#include "../SceneSwitch/SceneSwitch.h"
#include "MenuUI\TutorialManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "SceneMenu.h"
#include "SceneSelect.h"
#include "Data\/SelectData.h"

// ������`����ƃ��E���h�R�[�����X�L�b�v�����(�f�o�b�O���Z�p)
//#define ROUND_SKIP

//#ifdef ROUND_SKIP
#include "../UI/GameUI.h"
//#endif

/*******************************************************/
//					�Q�[���J�n�̃X�e�[�W�J�����X�e�[�g
/*******************************************************/

void SceneMainState::StageIntro::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

	// �J��������
	EFFECT_CAMERA_INFO EffectCameraInfo;
	EffectCameraInfo.scriptID = (int)pMain->GetStage()->GetIntroCameraScriptID();
	MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MAIN, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &EffectCameraInfo);

	m_bSkip = false;
}

void SceneMainState::StageIntro::Execute(sceneMain *pMain)
{
	if (m_bSkip)
	{
		if (Fade::isFadeOutCompletion())
		{
			pMain->GetFSM()->ChangeState(CharaIntro::GetInstance());
			return;
		}
	}
	else
	{
		FOR(PlayerMgr->GetNumPlayer())
		{
			const int PlayerDeviceID(PlayerMgr->GetPlayer(i)->GetDeviceID());
			if (KEY(KEY_A, PlayerDeviceID) == 3 || KEY(KEY_B, PlayerDeviceID) == 3 || KEY(KEY_C, PlayerDeviceID) == 3 || KEY(KEY_D, PlayerDeviceID) == 3)
			{
				m_bSkip = true;
				Fade::Set(Fade::FADE_OUT, 8);
				break;
			}
		}
	}

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();

	// �G�t�F�N�g�J�����I�������|�������ɍs��
	if (!CameraMgr->GetFSM()->isInState(*EffectCameraState::GetInstance()))
	{
		// �|�������X�e�[�g��
		pMain->GetFSM()->ChangeState(CharaIntro::GetInstance());
	}
}

void SceneMainState::StageIntro::Exit(sceneMain *pMain){}
void SceneMainState::StageIntro::Render(sceneMain *pMain){}

bool SceneMainState::StageIntro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�L�����|�������X�e�[�g
/*******************************************************/

void SceneMainState::CharaIntro::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	if(Fade::m_alpha >= 128) Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

	// �L�����N�^�[�����̃C���g��
	PlayerMgr->SetIntroState();

	// �G�t�F�N�g�J���������������
	if (CameraMgr->GetFSM()->isInState(*EffectCameraState::GetInstance())) CameraMgr->OffEffectCamera();

	m_bSkip = false;

	m_iKakeaiTimer = 0;
}

void SceneMainState::CharaIntro::Execute(sceneMain *pMain)
{
	if (m_bSkip)
	{
		if (Fade::isFadeOutCompletion())
		{
			pMain->GetFSM()->ChangeState(Round::GetInstance());
			return;
		}
	}
	else
	{
		FOR(PlayerMgr->GetNumPlayer())
		{
			const int PlayerDeviceID(PlayerMgr->GetPlayer(i)->GetDeviceID());
			if (KEY(KEY_A, PlayerDeviceID) == 3 || KEY(KEY_B, PlayerDeviceID) == 3 || KEY(KEY_C, PlayerDeviceID) == 3 || KEY(KEY_D, PlayerDeviceID) == 3)
			{
				m_bSkip = true;
				Fade::Set(Fade::FADE_OUT, 8);
				break;
			}
		}
	}

	// ���������背�x���ō��͎��Ԍo�߂Ŏ��̃��E���h�R�[����
	if(++m_iKakeaiTimer > 180) pMain->GetFSM()->ChangeState(Round::GetInstance());

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();
}

void SceneMainState::CharaIntro::Exit(sceneMain *pMain)
{
	// ���|�������I�����b�Z�[�W�𑗂�
	FOR(PlayerMgr->GetNumPlayer())
	{
		MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MAIN, PlayerMgr->GetPlayer(i)->GetID(), MESSAGE_TYPE::KAKEAI_END, nullptr);
	}
}
void SceneMainState::CharaIntro::Render(sceneMain *pMain){}

bool SceneMainState::CharaIntro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					���E���h�R�[���X�e�[�g
/*******************************************************/

void SceneMainState::Round::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	if (Fade::m_alpha >= 128) Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

#ifdef ROUND_SKIP
	//if (pMain->GetRoundNum() == 0)
	{
		GameUIMgr->Action();
	}
#else
	// ���E���h����0�̎��̓R�[�����Ȃ�
	if (pMain->GetRoundNum() != 0)
	{
		// The Wheel of Fate is Turning.
		const int RoundNumber(PlayerMgr->GetRoundNumber());

		// ����Ȃ�
		if (RoundNumber == 0)pMain->GetRoundCall()->CallFirstGameRound();
		else pMain->GetRoundCall()->CallRound(RoundNumber + 1);
	}
	else
	{
		GameUIMgr->Action();
	}

#endif
}
void SceneMainState::Round::Execute(sceneMain *pMain)
{
#ifdef ROUND_SKIP
	//if (pMain->GetRoundNum() == 0)
	{
		pMain->GetFSM()->ChangeState(Main::GetInstance());
	}
#endif
	
	if (pMain->GetRoundNum() == 0)
	{
		pMain->GetFSM()->ChangeState(Main::GetInstance());
	}

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::NO_FSM);
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();
}
void SceneMainState::Round::Exit(sceneMain *pMain){}
void SceneMainState::Round::Render(sceneMain *pMain){}

bool SceneMainState::Round::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::END_ROUNDCALL:	// ���E���h�R�[�����I������Ƃ������b�Z�[�W���͂�����
		pMain->GetFSM()->ChangeState(Main::GetInstance());
		break;
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					���C���X�e�[�g
/*******************************************************/

void SceneMainState::Main::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	if (Fade::m_alpha >= 128) Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

	// �^�C�}�[�n��
	GameUIMgr->TimerStart();
}

void SceneMainState::Main::Execute(sceneMain *pMain)
{
	// (TODO) 0�b�ɂȂ�����^�C���A�b�v�I�I
	if (GameUIMgr->isTimerUp())
	{

		//return;
	}

	// �K�E�X�e�[�g�ɒN������������~�߂� (TODO) KO�t���O�Ƃ������if�ň݂͂���
	if (PlayerMgr->GetPlayer(SIDE::LEFT)->isHeavehoDriveState() ||
		PlayerMgr->GetPlayer(SIDE::RIGHT)->isHeavehoDriveState())
	{
		GameUIMgr->TimerStop();
	}
	else
	{
		GameUIMgr->TimerStart();
	}

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);

	// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
	Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

	// �v���C���[�ʒu�m��
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();



	// �^�C�}�[�X�V&�^�C�}�[���I���Ȃ�I���X�e�[�g�ɍs��
	//if (TimeMgr->Update())
	//{
	//	//pMain->GetFSM()->ChangeState(Finish::GetInstance());
	//}

	//+--------------------------------------------
	//	�|�[�Y���j���[�̑���
	//+--------------------------------------------

	// �p�b�h���X�V
	const int NumDevice(tdnInputManager::GetNumDevice());

	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)
	{
		// ���j���[�؂�ւ�
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, 0) == 3)
		{
			pMain->GetFSM()->ChangeState(PauseMenu::GetInstance());
			return;
		}

	}else
	{
		for (int i = 0; i < NumDevice; i++)
		{
			// ���j���[�؂�ւ�
			if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, i) == 3)
			{
				pMain->GetFSM()->ChangeState(PauseMenu::GetInstance());
				return;
			}
		}
	}

}
void SceneMainState::Main::Exit(sceneMain *pMain)
{
	// �^�C�}�[�X�g�b�v
	GameUIMgr->TimerStop();

}
void SceneMainState::Main::Render(sceneMain *pMain){}


bool SceneMainState::Main::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::OVER_DRIVE_STAGE:
	{
	   bool *bAction = (bool*)msg.ExtraInfo;
	   if (*bAction)
		   pMain->OverDriveAction();
	   else
		   pMain->OverDriveEnd();
	   return true;
	}
		break;
		// �J�b�g�C����������u��
	case MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START:
		// BGM�̃N���X�t�F�[�h(���K�EBGM�𗬂�)
		bgm->StopStreamIn();
		bgm->PlayStreamIn("DATA/Sound/BGM/HeaveHo/HeaveHo.ogg");

		break;
		// �J�b�g�C���I���������Ċ���
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
	{
		pMain->GetFSM()->ChangeState(HeaveHoDriveOverFlowSuccess::GetInstance());
		
		// �Œ�HP�ۑ�
		GameUIMgr->GetHpGage((SIDE)((int)msg.Sender - (int)ENTITY_ID::ID_PLAYER_FIRST))->JudeRemainingHP();

		// �V�[�����X�X���b�h����
		ResultData data;// ���U���g�ɕK�v�ȃf�[�^�������ŋl�߂�
		data.eWinnerSide = PlayerMgr->GetPlayer(msg.Sender)->GetSide();
		data.iMaxDamage = 10000;
		data.iRemainingHP= PlayerMgr->GetPlayer(msg.Sender)->GetHPPercentage();
		data.iElapsedTime = GameUIMgr->GetTimer()->GetElapsedTime();

		LoadSceneThreadMgr->Initialize(new sceneResult(data));
		return true;
	}	break;
	case MESSAGE_TYPE::KO:	// �N����HP��0�ɂȂ�����؂�ւ�
	{
		KO_INFO *KOInfo = (KO_INFO*)msg.ExtraInfo;

		// �Œ�HP�ۑ�
		GameUIMgr->GetHpGage(KOInfo->WinnerSide)->JudeRemainingHP();

		// �V�[�����X�X���b�h����
		ResultData data;// ���U���g�ɕK�v�ȃf�[�^�������ŋl�߂�
		data.eWinnerSide = KOInfo->WinnerSide;
		data.iMaxDamage = GameUIMgr->GetComboUI(KOInfo->LoseSide)->GetMaxDamage();		
		data.iRemainingHP =GameUIMgr->GetHpGage(KOInfo->WinnerSide)->GetHPPercentage();
		data.iElapsedTime = GameUIMgr->GetTimer()->GetElapsedTime();

		// ���E���h�S������ď����m�肾������A�t�B�j�b�V���̒i�K�Ń��U���g�V�[����ǂݍ���
		if (KOInfo->iNumWinnerRound == pMain->GetRoundNum() - 1)
		{
			LoadSceneThreadMgr->Initialize(new sceneResult(data));
		}

		pMain->GetFSM()->ChangeState(Finish::GetInstance());

		if(KOInfo->FinishType == FINISH_TYPE::NORMAL)pMain->GetRoundCall()->CallFinish(msg.Sender);
		else if (KOInfo->FinishType == FINISH_TYPE::OVER_DRIVE) pMain->GetRoundCall()->CallOverDriveFinish(msg.Sender);

		return true;
	}
		break;
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/*******************************************************/
//					KO���X�e�[�g
/*******************************************************/

void SceneMainState::Finish::Enter(sceneMain *pMain)
{
	// �t�B�j�b�V���R�[���t���O�I�t
	m_bEndCall = false;
}
void SceneMainState::Finish::Execute(sceneMain *pMain)
{
	// �V�[���؂�ւ���ʒ�
	if (pMain->GetSceneSwitchPrev()->IsAction())
	{
		// �V�[���`�F���W����X�V
		pMain->GetSceneSwitchPrev()->Update();

		// �V�[���؂�ւ��I�������
		if (pMain->GetSceneSwitchPrev()->IsEnd())
		{
			// ���o���I������i�K�ł��V�[����ǂݍ���ł�����A�I���܂Ŗ������[�v
			while (!LoadSceneThreadMgr->isInitializeEnd());

			// ���łɓǂݍ��񂾃��U���g�V�[���ɍs��
			MainFrameEx->ChangeScene(LoadSceneThreadMgr->GetNewScene());

			// �V���O���g��������V�[���ς���Ă��f���[�g�Ƃ����v�̂͂�
			LoadSceneThreadMgr->Release();
		}
	}

	else
	{
		// �v���C���[�X�V
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();

		// �J�����X�V
		CameraMgr->Update();

		// �t�B�j�b�V���R�[���I�������
		if (m_bEndCall)
		{
			// �t�F�[�h�I�������
			if (Fade::isFadeStop())
			{
				// �܂�������
				//else
				{
					// ��񏉊���
					pMain->Reset();

					pMain->GetFSM()->ChangeState(Round::GetInstance());
					return;
				}
			}
		}
		else
		{
			// �����ɗ����Ƃ������Ƃ́A���Ԃ�g���[�j���O�œG��|�����ꍇ
			if (Fade::isFadeOutCompletion())
			{
				//�`���[�g���A���r���ł�߂ăQ�[��������o�O��
				// �`���[�g���A���ŎE���Ă��܂���
				if(pMain->GetFSM()->isPrevState(*SceneMainState::TutorialMain::GetInstance()))
					pMain->GetFSM()->ChangeState(SceneMainState::TutorialIntro::GetInstance());

				// (TODO)�g���[�j���O���[�h�ŎE���Ă��܂���
				//if()
			}
		}
	}
}
void SceneMainState::Finish::Exit(sceneMain *pMain){}
void SceneMainState::Finish::Render(sceneMain *pMain){}
bool SceneMainState::Finish::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::OVER_DRIVE_STAGE:
	{
										   bool *bAction = (bool*)msg.ExtraInfo;
										   if (*bAction)
											   pMain->OverDriveAction();
										   else
											   pMain->OverDriveEnd();
										   return true;
	}
		break;

	case MESSAGE_TYPE::END_FINISHCALL:

		// �`���[�g���A��or�g���[�j���O
		if (pMain->GetRoundNum() == 0)
		{
			Fade::Set(Fade::FLAG::FADE_OUT, 8, 0x00000000);
		}

		// �}�W�ΐ�
		else
		{
			// �v���C���[�����E���h�Ƃ��ă��U���g�ɍs���̂��m�肵�Ă���Ȃ�
			if (LoadSceneThreadMgr->isAction())
			{
				// ���U���g�؂�ւ�蔭��
				pMain->GetSceneSwitchPrev()->Action();
			}

			// ����ȊO�Ȃ珟�����v���C���[�̏������o
			else
			{
				ENTITY_ID *WinnerID = (ENTITY_ID*)msg.ExtraInfo;
				MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MAIN, *WinnerID, MESSAGE_TYPE::END_FINISHCALL, nullptr);
			}
		}
		break;
	case MESSAGE_TYPE::END_WINNER:
		m_bEndCall = true;	// �t�B�j�b�V���R�[���I�������t���O�𗧂Ă�
		Fade::Set(Fade::FLAG::FADE_OUT, 8, 0x00000000);
		break;
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

/*******************************************************/
//					���K�X�e�[�g
/*******************************************************/

void SceneMainState::HeaveHoDriveOverFlowSuccess::Enter(sceneMain *pMain)
{
	// �X�e�[�W�`�F���W
	pMain->GetStage()->ChangeObj((0) ? "DATA/airou_stage.IMO" : "DATA/Stage/HeaveHo/HeaveHoStage.IMO", nullptr);
	pMain->GetStage()->GetObj()->SetScale(4);
	//pMain->GetStage()->GetObj()->SetPos(0, -20, 0);
	pMain->GetStage()->GetObj()->Update();
}
void SceneMainState::HeaveHoDriveOverFlowSuccess::Execute(sceneMain *pMain)
{
	// �V�[���؂�ւ���ʒ�
	if (pMain->GetSceneSwitchPrev()->IsAction())
	{
		// �V�[���`�F���W����X�V
		pMain->GetSceneSwitchPrev()->Update();

		// �A�N�V�������I�������
		if (pMain->GetSceneSwitchPrev()->IsEnd())
		{
			// �Ȃ��Ǝv�����A���o���I������i�K�ł��V�[����ǂݍ���ł�����A�I���܂Ŗ������[�v
			while (!LoadSceneThreadMgr->isInitializeEnd());

			// ���łɓǂݍ��񂾃��U���g�V�[���ɍs��
			MainFrameEx->ChangeScene(LoadSceneThreadMgr->GetNewScene());

			// �V���O���g��������V�[���ς���Ă��f���[�g�Ƃ����v�̂͂�
			LoadSceneThreadMgr->Release();
		}
	}

	else
	{
		// �v���C���[�X�V
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();

		// �J�����X�V
		CameraMgr->Update();

		// �G�t�F�N�g�J�����I������烊�U���g�ɍs��
		if (!CameraMgr->GetFSM()->isInState(*EffectCameraState::GetInstance()))
		{
			// �V�[���؂�ւ���ʃG�t�F�N�g�����I
			pMain->GetSceneSwitchPrev()->Action();
		}
	}
}
void SceneMainState::HeaveHoDriveOverFlowSuccess::Exit(sceneMain *pMain) {}
void SceneMainState::HeaveHoDriveOverFlowSuccess::Render(sceneMain *pMain) {}
bool SceneMainState::HeaveHoDriveOverFlowSuccess::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�`���[�g���A���J�n�X�e�[�g
/*******************************************************/

void SceneMainState::TutorialIntro::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 16, 0x00000000);

	// ����`���[�g���A���ݒ�I
	TutorialMgr->Init(pMain->GetSelectTutorial(), SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);
	TutorialMgr->GetTutorial()->ActionIntroTips();// �C���g����Tips��p��


	// ��񏉊���
	pMain->Reset();

	//  �`���[�g���A�����ɕK�v�Ȑݒ�������ł��Ă�����
	if (pMain->GetSelectTutorial() == TUTORIAL_TYPE::OVER_DRIVE)
	{
		PlayerMgr->GetPlayerByDeviceID
			(SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID)->SetOverDriveGage(100);
	}
}

// �X�V
void SceneMainState::TutorialIntro::Execute(sceneMain *pMain)
{
//#ifdef ROUND_SKIP
//	pMain->GetFSM()->ChangeState(Main::GetInstance());
//#endif
	
	// �`���[�g���A���X�V
	TutorialMgr->Update();

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::NO_FSM);
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();

	// �q���g�J�[�h��ǂݏI������
	if (TutorialMgr->GetTutorial()->
		GetIntroTipsCard()->GetSelectState()== TipsCard::SELECT_STATE::OK)
	{
		pMain->GetFSM()->ChangeState(TutorialMain::GetInstance());
		return;
	}
}

void SceneMainState::TutorialIntro::Exit(sceneMain *pMain)
{
	// �C���g����Tips�����
	TutorialMgr->GetTutorial()->StopIntroTips();

}

void SceneMainState::TutorialIntro::Render(sceneMain *pMain)
{
	TutorialMgr->Render();
}

bool SceneMainState::TutorialIntro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::END_ROUNDCALL:	// ���E���h�R�[�����I������Ƃ������b�Z�[�W���͂�����
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//			�`���[�g���A�����C���X�e�[�g
/*******************************************************/

void SceneMainState::TutorialMain::Enter(sceneMain *pMain) 
{

	// ��TODO�@�{�^���@���������c�����܂܂Ȃ̂Ł@�͂���I�������炻�̂܂܍U�����Ă��܂�
	// �����ŗ�����kesu


}

void SceneMainState::TutorialMain::Execute(sceneMain *pMain)
{
	// �v���C���[�X�V
	if (TutorialMgr->GetTutorial()->isClear())
	{
		// �N���A�ɂ��̂œ������~�߂�
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();
	}
	else
	{
		// �ʏ�
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();
	}

	// �J�����X�V
	CameraMgr->Update();

	// 1P�̑��삵�Ă���v���C���[�̏����m�F���N���A������o���I
	TutorialMgr->TaskUpdate(PlayerMgr->GetPlayer(0));

	// �`���[�g���A���X�V
	TutorialMgr->Update();
	
	// �^�X�N��S�ăN���A���Ďw�肵�����Ԃ��߂���΃N���A�X�e�[�g��
	if (TutorialMgr->GetTutorial()->EndTipsStart())
	{
		pMain->GetFSM()->ChangeState(TutorialClear::GetInstance());
		return;
	}

	/********************************************/
	// �������X�^�[�g
	if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
	{

		// �|�[�Y�Ń`���[�g���A����p�̃|�[�Y���j���[��
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
		{
			pMain->GetFSM()->ChangeState(TutorialPauseMenu::GetInstance());
			return;
		}

		// �Z���N�g�ł�����x�C���g���֖߂�
		if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
		{
			// �t�F�[�h�A�E�g
			Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

			// (TODO)�K�E�̎��Ƀ��[�v����ƃo�O��
			
			return;
		}

	}

	// �t�F�[�h�A�E�g������
	if (Fade::isFadeOutCompletion())
	{
		// �`���[�g���A���̃C���g����
		pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());
		return;
	}
	/********************************************/
}

void SceneMainState::TutorialMain::Exit(sceneMain *pMain) 
{

}

void SceneMainState::TutorialMain::Render(sceneMain *pMain)
{
	TutorialMgr->Render();
}

bool SceneMainState::TutorialMain::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::OVER_DRIVE_STAGE:
	{
		bool *bAction = (bool*)msg.ExtraInfo;
		if (*bAction)
			pMain->OverDriveAction();
		else
			pMain->OverDriveEnd();
		return true;
	}
	break;
	// �J�b�g�C����������u��
	case MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START:
		// BGM�̃N���X�t�F�[�h(���K�EBGM�𗬂�)
		//pMain->GetMyMusicManager()->PlayHeaveHo();
		bgm->StopStreamIn();
		bgm->PlayStreamIn("DATA/Sound/BGM/HeaveHo/HeaveHo.ogg");
		break;
		// �J�b�g�C���I���������Ċ���
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
		pMain->GetFSM()->ChangeState(HeaveHoDriveOverFlowSuccess::GetInstance());
		return true;
		break;
	case MESSAGE_TYPE::KO:	// �N����HP��0�ɂȂ�����؂�ւ�
	{
		FINISH_TYPE *type = (FINISH_TYPE*)msg.ExtraInfo;
		pMain->GetFSM()->ChangeState(Finish::GetInstance());
		if (*type == FINISH_TYPE::NORMAL)pMain->GetRoundCall()->CallFinish(msg.Sender);
		else if (*type == FINISH_TYPE::OVER_DRIVE) pMain->GetRoundCall()->CallOverDriveFinish(msg.Sender);

		return true;
	}
	break;
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

/*******************************************************/
//			�`���[�g���A���N���A(�G���h)�X�e�[�g
/*******************************************************/

void SceneMainState::TutorialClear::Enter(sceneMain *pMain)
{
	// �N���A���Tips��p��
	TutorialMgr->GetTutorial()->ActionClearTips();

	// ���̃`���[�g���A����
	// (TODO)�����Ŕz��̍Ō�܂ł����烁�j���[�ɖ߂鏈��
	// �����l�ł����I�Ƃ��͍Ō�̃N���A�e�B�b�v�X�ŏ������B
	pMain->SetSelectTutorial(TUTORIAL_TYPE(pMain->GetSelectTutorial() + (TUTORIAL_TYPE)1));
	if (pMain->GetSelectTutorial() == TUTORIAL_TYPE::ARRAY_END)
	{

	}

}

// �X�V
void SceneMainState::TutorialClear::Execute(sceneMain *pMain)
{

	// �`���[�g���A���X�V
	TutorialMgr->Update();

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();

	if (Fade::isFadeOutCompletion() == false)
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
			// �q���g�J�[�h��ǂݏI������
			if (TutorialMgr->GetTutorial()->
				GetClearTipsCard()->GetSelectState() == TipsCard::SELECT_STATE::OK)
			{
				// �N���A���Tips�����
				TutorialMgr->GetTutorial()->StopClearTips();

				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

			}
		}

	}else// �t�F�[�h�A�E�g������	
	{
		// (TODO)�����Ŕz��̍Ō�܂ł����烁�j���[�ɖ߂鏈��
		if (pMain->GetSelectTutorial() == TUTORIAL_TYPE::ARRAY_END)
		{
			MainFrameEx->ChangeScene(new sceneMenu());
			return;
		}
		else
		{
			// �`���[�g���A���̃C���g����
			pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());
			return;
		}

	}

}

void SceneMainState::TutorialClear::Exit(sceneMain *pMain)
{

}

void SceneMainState::TutorialClear::Render(sceneMain *pMain)
{
	TutorialMgr->Render();
}

bool SceneMainState::TutorialClear::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::END_ROUNDCALL:	// ���E���h�R�[�����I������Ƃ������b�Z�[�W���͂�����
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//			�|�[�Y���j���[�X�e�[�g
/*******************************************************/

void SceneMainState::PauseMenu::Enter(sceneMain *pMain)
{	
	// �|�[�Y�E�B���h�E�N�� 
	pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Action();
	pMain->SetPause(true);

}

// �X�V
void SceneMainState::PauseMenu::Execute(sceneMain *pMain)
{

	//+--------------------------------------------
	//	�|�[�Y���j���[�̑���
	//+--------------------------------------------
	// �p�b�h���X�V
	const int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)
	{
		// �|�[�Y�E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Ctrl(0);
	}
	else
	{
		for (int i = 0; i < NumDevice; i++)
		{
			// �|�[�Y�E�B���h�E�̑���
			pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Ctrl(i);
		}
	}



	// �v���C���[�X�V���~�߂�
	//PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	//PlayerMgr->UpdatePos();

	// �J�����X�V
	//CameraMgr->Update();

	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState() == PauseWindow::BATTLE_STATE::BACK)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Stop();

		//// �������ŏ�Ԃɉ����ĕ���
		//// �`���[�g���A��
		//if (pMain->GetTutorialFlag())
		//{
		//	pMain->GetFSM()->ChangeState(TutorialMain::GetInstance());	// �`���[�g�����֖߂�
		//	
		//}
		//else
		//{
			pMain->GetFSM()->ChangeState(Main::GetInstance());	// ���C���֖߂�

		//}

	}

	// �L�����N�^�[�Z���N�g��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState() == PauseWindow::BATTLE_STATE::BACK_CHARA_SELECT)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneSelect());
		return;
	}

	// ���j���[��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState() == PauseWindow::BATTLE_STATE::BACK_MENU_SELECT)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneMenu());
		return;
	}




}

void SceneMainState::PauseMenu::Exit(sceneMain *pMain)
{
	pMain->SetPause(false);
}

void SceneMainState::PauseMenu::Render(sceneMain *pMain)
{

}

bool SceneMainState::PauseMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::END_ROUNDCALL:	// ���E���h�R�[�����I������Ƃ������b�Z�[�W���͂�����
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//			�`���[�g���A���|�[�Y���j���[�X�e�[�g
/*******************************************************/

void SceneMainState::TutorialPauseMenu::Enter(sceneMain *pMain)
{
	// �`���[�g���A���|�[�Y�E�B���h�E�N�� 
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Action();
	pMain->SetPause(true);

}

// �X�V
void SceneMainState::TutorialPauseMenu::Execute(sceneMain *pMain)
{

	//+--------------------------------------------
	//	�`���[�g���A���|�[�Y���j���[�̑���
	//+--------------------------------------------
	// �p�b�h���X�V
	const int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)
	{
		// �|�[�Y�E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Ctrl(0);
	}
	else
	{
		for (int i = 0; i < NumDevice; i++)
		{
			// �|�[�Y�E�B���h�E�̑���
			pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Ctrl(i);
		}
	}



	// �v���C���[�X�V���~�߂�
	//PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	//PlayerMgr->UpdatePos();

	// �J�����X�V
	//CameraMgr->Update();

	// ��O�̃`���[�g���A���֖߂�{�^������������
	//if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::BACK_PREV_TUTORIAL)
	//{
	//	// �|�[�Y�E�B���h�E�~�߂�
	//	pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();

	//	pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());	

	//}

	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::BACK)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();

		pMain->GetFSM()->ChangeState(TutorialMain::GetInstance());	// �`���[�g���A���֖߂�

	}

	// ���j���[��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::BACK_MENU_SELECT)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneMenu());
		return;
	}
	
}

void SceneMainState::TutorialPauseMenu::Exit(sceneMain *pMain)
{
	pMain->SetPause(false);
}

void SceneMainState::TutorialPauseMenu::Render(sceneMain *pMain)
{

}

bool SceneMainState::TutorialPauseMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::END_ROUNDCALL:	// ���E���h�R�[�����I������Ƃ������b�Z�[�W���͂�����
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}