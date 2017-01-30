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
#include "Trophy\TrophyManager.h"
#include "Challenge\ChallengeManagerManager.h"
#include "Data\CommandData.h"
#include "../Data/CommandData.h"

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
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0, Fade::m_alpha, 0x00000000);

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
	if (Fade::m_alpha > 0) Fade::Set(Fade::FLAG::FADE_IN, 8, 0, Fade::m_alpha, 0x00000000);

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

		// ����̂�
		if (PlayerMgr->GetRoundNumber() == 0)
		{
			if (!pMain->GetFSM()->isPrevState(*SceneMainState::PauseMenu::GetInstance()))
			{
				if (CommandMgr->isReplay())
				{
					// ���v���C���Đ�����
					CommandMgr->Action("DATA/Replay/LeftSide.bin", SIDE::LEFT);
					CommandMgr->Action("DATA/Replay/RightSide.bin", SIDE::RIGHT);
				}
				else
				{
					// �v���J�n����
					CommandMgr->SaveStart(PlayerMgr->GetPlayer(SIDE::LEFT), SIDE::LEFT);
					CommandMgr->SaveStart(PlayerMgr->GetPlayer(SIDE::RIGHT), SIDE::RIGHT);
				}
			}
		}
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

	// ���ۑ������R�}���h����͂���
	if (CommandMgr->isAction())
	{
		CommandMgr->Update(SIDE::LEFT);
		CommandMgr->Update(SIDE::RIGHT);
	}

	PlayerMgr->UpdatePos();


	// �J�����X�V
	CameraMgr->Update();

	// �X�^�[�g�{�^�������ĂȂ��Ƃ��ɃZ�[�u�X�V
	if (CommandMgr->SaveIsAction())
	{
		CommandMgr->SaveUpdate(SIDE::LEFT);
		CommandMgr->SaveUpdate(SIDE::RIGHT);
	}
}
void SceneMainState::Round::Exit(sceneMain *pMain)
{

}
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
	// �^�C�}�[�n��
	GameUIMgr->TimerStart();
}

void SceneMainState::Main::Execute(sceneMain *pMain)
{
	// 0�b�ɂȂ�����^�C���A�b�v�I�I
	if (GameUIMgr->isTimerUp())
	{
		BasePlayer *l_pWinnePlayer(PlayerMgr->GetHPWinner());
		if (l_pWinnePlayer)
		{
			// �^�C���A�b�v�R�[�������肢����
			pMain->GetRoundCall()->CallTimeUp(l_pWinnePlayer->GetID());

			ResultData data;// ���U���g�ɕK�v�ȃf�[�^�������ŋl�߂�
			data.eWinnerSide = l_pWinnePlayer->GetSide();
			data.iMaxDamage = GameUIMgr->GetComboUI((data.eWinnerSide == SIDE::LEFT) ? SIDE::RIGHT : SIDE::LEFT)->GetMaxDamage();
			data.iRemainingHP = GameUIMgr->GetHpGage(data.eWinnerSide)->GetHPPercentage();
			data.iElapsedTime = GameUIMgr->GetTimer()->GetElapsedTime();

			// ���E���h�S������ď����m�肾������A�t�B�j�b�V���̒i�K�Ń��U���g�V�[����ǂݍ���
			if (l_pWinnePlayer->GetWinNum() == pMain->GetRoundNum() - 1)
			{
				LoadSceneThreadMgr->Initialize(new sceneResult(data));
			}
		}

		else
		{
			// �^�C���A�b�v�R�[�������肢����
			pMain->GetRoundCall()->CallTimeUp(ENTITY_ID::ID_ERROR);
		}

		// �X�e�[�g�ύX
		pMain->GetFSM()->ChangeState(Finish::GetInstance());
		return;
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

	// ���ۑ������R�}���h����͂���
	if (CommandMgr->isAction())
	{
		CommandMgr->Update(SIDE::LEFT);
		CommandMgr->Update(SIDE::RIGHT);
	}

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
	int NumDevice(tdnInputManager::GetNumDevice());

	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;	
	for (int i = 0; i < NumDevice; i++)
	{
		// ���j���[�؂�ւ�
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, i) == 3)
		{
			// (12/24) �����Ń��j���[�N��
			//pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Action();
			pMain->SetPauseDeviceID(i);// ���|�[�Y���j���[���������l�ۑ�
			pMain->GetFSM()->ChangeState(PauseMenu::GetInstance());
			return;
		}
	}
	
	// �X�^�[�g�{�^�������ĂȂ��Ƃ��ɃZ�[�u�X�V
	if (CommandMgr->SaveIsAction())
	{
		CommandMgr->SaveUpdate(SIDE::LEFT);
		CommandMgr->SaveUpdate(SIDE::RIGHT);
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
		
		// ������������AI����Ȃ�������
		if (SelectDataMgr->Get()->tagSideDatas[(int)KOInfo->WinnerSide].bAI == false)
		{
			// ����������HP��n��
			TrophyMgr->CheakMaxHpFinish(data.iRemainingHP);
			// �o�ߎ��Ԃ�n��
			TrophyMgr->CheakSpeedFinish(data.iElapsedTime);
		}

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

			// �X���b�h�̊J�� �V���O���g��������V�[���ς���Ă��f���[�g�Ƃ����v�̂͂�
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
				if (pMain->GetFSM()->isPrevState(*SceneMainState::Training::GetInstance()))
					pMain->GetFSM()->ChangeState(SceneMainState::TrainingIntro::GetInstance());

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
				ENTITY_ID *WinnerID((ENTITY_ID*)msg.ExtraInfo);

				// �ǂ����������Ă��Ȃ����
				if (*WinnerID == ENTITY_ID::ID_ERROR)
				{
					m_bEndCall = true;	// �t�B�j�b�V���R�[���I�������t���O�𗧂Ă�
					Fade::Set(Fade::FLAG::FADE_OUT, 8, 0x00000000);
					return true;
				}

				// �v���C���[������͉��񏟂����炢���̂��킩��Ȃ��̂ŁA���b�Z�[�W�ŏ������E���h���𑗂�
				int RoundNum(pMain->GetRoundNum());
				MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MAIN, *WinnerID, MESSAGE_TYPE::END_FINISHCALL, &RoundNum);
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
	//TutorialMgr->Init(pMain->GetSelectTutorial(), SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);
	
	// �����݂̃`���[�g���A����ύX
	SelectDataMgr->Get()->iTutorialType = TutorialMgr->GetSelectNo();
	
	// (1/21) �}�l�[�W���[�̒������Ŋ���������	
	TutorialMgr->Init(SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);
	TutorialMgr->GetTutorial()->ActionIntroTips();// �C���g����Tips��p��


	// ��񏉊���
	pMain->Reset();

	// �S���N
	// SP�Q�[�W��������
	PlayerMgr->GetPlayer(SIDE::LEFT)->SetOverDriveGage(0);
	PlayerMgr->GetPlayer(SIDE::RIGHT)->SetOverDriveGage(0);

	//  �`���[�g���A�����ɕK�v�Ȑݒ�������ł�����
	// ����Ƃ̋����͊�{�߂Â���
	PlayerMgr->GetPlayer(SIDE::LEFT)->SetPos(Vector3(-7, 0, 0));
	PlayerMgr->GetPlayer(SIDE::RIGHT)->SetPos(Vector3(7, 0, 0));
	//if (pMain->GetSelectTutorial() == TUTORIAL_TYPE::STAND_GUARD)
	//{
	//	PlayerMgr->GetPlayer(SIDE::LEFT)->SetPos(Vector3(-97.5, 0, 0));
	//	PlayerMgr->GetPlayer(SIDE::RIGHT)->SetPos(Vector3(-60, 0, 0));
	//}


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
	PlayerMgr->Update(PLAYER_UPDATE::NO_FSM);// NO_FSM
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

}

void SceneMainState::TutorialMain::Execute(sceneMain *pMain)
{
	// �������@(12/28)�`���[�g���A���ł��X�V
	PlayerMgr->UpdateTraining();

	// �v���C���[�X�V
	if (TutorialMgr->GetTutorial()->isClear())
	{
		// �N���A�����̂œ������~�߂�
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();
	}
	else
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
			// �ʏ�
			PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);
		}
		else
		{
			// �t�F�[�h���͑�����󂯕t���Ȃ�
			PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
		}

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
			// (12/24) �����Ń��j���[�N��
			//pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Action();
			pMain->SetPauseDeviceID(SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);// ���|�[�Y���j���[���������l�ۑ�
			pMain->GetFSM()->ChangeState(TutorialPauseMenu::GetInstance());
			return;
		}

		// �N���A�\�L���̓��X�^�[�g�ł��Ȃ�
		if (TutorialMgr->GetTutorial()->isClear() == false)
		{
			// �Z���N�g�ł�����x�C���g���֖߂�
			if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
			{
				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

				// (TODO)�K�E�̎��Ƀ��[�v����ƃo�O��

				return;
			}
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

#ifdef _DEBUG

	if (KeyBoardTRG(KB_E))
	{
		TutorialMgr->SetSelectType(TUTORIAL_TYPE::OD_ONEMORE);
	}

#endif // _DEBUG

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
	//pMain->SetSelectTutorial(TUTORIAL_TYPE((int)pMain->GetSelectTutorial() + 1));



}

// �X�V
void SceneMainState::TutorialClear::Execute(sceneMain *pMain)
{

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();

	if (Fade::isFadeOutCompletion() == false)
	{

		// �`���[�g���A���X�V
		TutorialMgr->Update();

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

		// (1/21) �����̃`���[�g���A����
		TutorialMgr->NectStep();


		// �Z�����l�͂����ŏI��
		if (TutorialMgr->isBusy() == true && 
			TutorialMgr->GetSelectType() == TUTORIAL_TYPE::PARTNER)
		{
			MainFrameEx->ChangeScene(new sceneMenu());
			return;
		}

		// (TODO)�����Ŕz��̍Ō�܂ł����烁�j���[�ɖ߂鏈��
		if (TutorialMgr->GetSelectType() == TUTORIAL_TYPE::ARRAY_END)
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
	if (Fade::isFadeOutCompletion() == false)
	{
		TutorialMgr->Render();
	}
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
//					�`�������W�J�n�X�e�[�g
/*******************************************************/

void SceneMainState::ChallengeIntro::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 16, 0x00000000);

	// �����݂̃`�������W��ύX
	SelectDataMgr->Get()->iChallengeType = ChallengeMgrMgr->GetSelectMgr()->GetSelectType();

	// (1/21) �}�l�[�W���[�̒������Ŋ���������	
	ChallengeMgrMgr->GetSelectMgr()->Init(SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);
	ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->ActionIntroTips();// �C���g����Tips��p��


												  // ��񏉊���
	pMain->Reset();

	// �S���N
	// SP�Q�[�W��������
	PlayerMgr->GetPlayer(SIDE::LEFT)->SetOverDriveGage(0);
	PlayerMgr->GetPlayer(SIDE::RIGHT)->SetOverDriveGage(0);

	////  �`�������W���ɕK�v�Ȑݒ�������ł�����
	//// ����Ƃ̋����͊�{�߂Â���
	//PlayerMgr->GetPlayer(SIDE::LEFT)->SetPos(Vector3(-7, 0, 0));
	//PlayerMgr->GetPlayer(SIDE::RIGHT)->SetPos(Vector3(7, 0, 0));
	//if (pMain->GetSelectChallenge() == Challenge_TYPE::STAND_GUARD)
	//{
	//	PlayerMgr->GetPlayer(SIDE::LEFT)->SetPos(Vector3(-97.5, 0, 0));
	//	PlayerMgr->GetPlayer(SIDE::RIGHT)->SetPos(Vector3(-60, 0, 0));
	//}

	// �����|�W�V�����̕ύX(�E����X�^�[�g�Ƃ��^�񒆂���Ƃ�)
	pMain->StartChallengePosition();

}

// �X�V
void SceneMainState::ChallengeIntro::Execute(sceneMain *pMain)
{
	//#ifdef ROUND_SKIP
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//#endif

	// �`�������W�X�V
	ChallengeMgrMgr->GetSelectMgr()->Update();

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::NO_FSM);// NO_FSM
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();

	// �q���g�J�[�h��ǂݏI������
	if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->
		GetIntroTipsCard()->GetSelectState() == TipsCard::SELECT_STATE::OK)
	{
		pMain->GetFSM()->ChangeState(ChallengeMain::GetInstance());
		return;
	}
}

void SceneMainState::ChallengeIntro::Exit(sceneMain *pMain)
{
	// �C���g����Tips�����
	ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->StopIntroTips();

}

void SceneMainState::ChallengeIntro::Render(sceneMain *pMain)
{
	ChallengeMgrMgr->GetSelectMgr()->Render();
}

bool SceneMainState::ChallengeIntro::OnMessage(sceneMain *pMain, const Message & msg)
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
//			�`�������W���C���X�e�[�g
/*******************************************************/

void SceneMainState::ChallengeMain::Enter(sceneMain *pMain)
{

	// ��TODO�@�{�^���@���������c�����܂܂Ȃ̂Ł@�͂���I�������炻�̂܂܍U�����Ă��܂�

}

void SceneMainState::ChallengeMain::Execute(sceneMain *pMain)
{
	// �������@(12/28)�`�������W�ł��X�V
	PlayerMgr->UpdateTraining();

	// �v���C���[�X�V
	if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->isClear())
	{
		// �N���A�����̂œ������~�߂�
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();
	}
	else
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
			// �ʏ�
			PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);
		}
		else
		{
			// �t�F�[�h���͑�����󂯕t���Ȃ�
			PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
		}

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();
	}

	// �J�����X�V
	CameraMgr->Update();

	// 1P�̑��삵�Ă���v���C���[�̏����m�F���N���A������o���I
	ChallengeMgrMgr->GetSelectMgr()->TaskUpdate(PlayerMgr->GetPlayer(0));

	// �`�������W�X�V
	ChallengeMgrMgr->GetSelectMgr()->Update();

	// �^�X�N��S�ăN���A���Ďw�肵�����Ԃ��߂���΃N���A�X�e�[�g��
	if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->EndTipsStart())
	{
		pMain->GetFSM()->ChangeState(ChallengeClear::GetInstance());
		return;
	}

	/********************************************/
	// �������X�^�[�g
	if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
	{

		// �|�[�Y�Ń`�������W��p�̃|�[�Y���j���[��
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
		{
			// (12/24) �����Ń��j���[�N��
			//pMain->GetWindow(BATTLE_WINDOW_TYPE::Challenge_PAUSE)->Action();
			pMain->SetPauseDeviceID(SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);// ���|�[�Y���j���[���������l�ۑ�
			pMain->GetFSM()->ChangeState(ChallengePauseMenu::GetInstance());
			return;
		}

		// �N���A�\�L���̓��X�^�[�g�ł��Ȃ�
		if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->isClear() == false)
		{
			// �Z���N�g�ł�����x�C���g���֖߂�
			if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
			{
				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

				// (TODO)�K�E�̎��Ƀ��[�v����ƃo�O��

				return;
			}
		}
	}

	// �t�F�[�h�A�E�g������
	if (Fade::isFadeOutCompletion())
	{
		{
			// �`�������W�̃C���g����
			pMain->GetFSM()->ChangeState(ChallengeIntro::GetInstance());
			return;
		}
	}
	/********************************************/

#ifdef _DEBUG

	if (KeyBoardTRG(KB_N)) CommandMgr->SaveStart(PlayerMgr->GetPlayer(ENTITY_ID::ID_PLAYER_FIRST));
	if (KeyBoardTRG(KB_M))
	{
		CommandMgr->SaveEnd();
		CommandMgr->SaveWrite(ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->GetReplayDataName().c_str());
	}
	if (KeyBoardTRG(KB_COMMA)) CommandMgr->Action(ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->GetReplayDataName().c_str());
	CommandMgr->SaveUpdate();
#endif // _DEBUG

}

void SceneMainState::ChallengeMain::Exit(sceneMain *pMain)
{

}

void SceneMainState::ChallengeMain::Render(sceneMain *pMain)
{
	ChallengeMgrMgr->GetSelectMgr()->Render();
}

bool SceneMainState::ChallengeMain::OnMessage(sceneMain *pMain, const Message & msg)
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
//			�`�������W�N���A(�G���h)�X�e�[�g
/*******************************************************/

void SceneMainState::ChallengeClear::Enter(sceneMain *pMain)
{
	// �N���A���Tips��p��
	ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->ActionClearTips();

	// ���̃`�������W��
	// (TODO)�����Ŕz��̍Ō�܂ł����烁�j���[�ɖ߂鏈��
	// �����l�ł����I�Ƃ��͍Ō�̃N���A�e�B�b�v�X�ŏ������B
	//pMain->SetSelectChallenge(Challenge_TYPE((int)pMain->GetSelectChallenge() + 1));



}

// �X�V
void SceneMainState::ChallengeClear::Execute(sceneMain *pMain)
{

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// �J�����X�V
	CameraMgr->Update();

	if (Fade::isFadeOutCompletion() == false)
	{

		// �`�������W�X�V
		ChallengeMgrMgr->GetSelectMgr()->Update();

		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
			// �q���g�J�[�h��ǂݏI������
			if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->
				GetClearTipsCard()->GetSelectState() == TipsCard::SELECT_STATE::OK)
			{
				// �N���A���Tips�����
				ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->StopClearTips();

				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);


			}
		}

	}
	else// �t�F�[�h�A�E�g������	
	{

		// (1/21) �����̃`�������W��
		ChallengeMgrMgr->GetSelectMgr()->NectStep();

		// (TODO)�����Ŕz��̍Ō�܂ł����烁�j���[�ɖ߂鏈��
		if ((CHALLENGE_TYPE)ChallengeMgrMgr->GetSelectMgr()->GetSelectType() == CHALLENGE_TYPE::ARRAY_END)
		{
			MainFrameEx->ChangeScene(new sceneMenu());
			return;
		}
		else
		{
			// �`�������W�̃C���g����
			pMain->GetFSM()->ChangeState(ChallengeIntro::GetInstance());
			return;
		}

	}

#ifdef _DEBUG

	if (KeyBoardTRG(KB_N)) CommandMgr->SaveStart(PlayerMgr->GetPlayer(ENTITY_ID::ID_PLAYER_FIRST));
	if (KeyBoardTRG(KB_M))
	{
		CommandMgr->SaveEnd();
		CommandMgr->SaveWrite(ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->GetReplayDataName().c_str());
	}
	if (KeyBoardTRG(KB_COMMA)) CommandMgr->Action(ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->GetReplayDataName().c_str());
	CommandMgr->SaveUpdate();
#endif // _DEBUG
}

void SceneMainState::ChallengeClear::Exit(sceneMain *pMain)
{

}

void SceneMainState::ChallengeClear::Render(sceneMain *pMain)
{
	if (Fade::isFadeOutCompletion() == false)
	{
		ChallengeMgrMgr->GetSelectMgr()->Render();
	}
}

bool SceneMainState::ChallengeClear::OnMessage(sceneMain *pMain, const Message & msg)
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
//			�`�������W���v���C�X�e�[�g
/*******************************************************/

void SceneMainState::ChallengeReplay::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 16, 0x00000000);

	// �����v���C�t���O�𗧂Ă�Ɓ@�N���A�\�L����Ȃ�
	ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->SetReplay(true);

	// �����݂̃`�������W��ύX
	SelectDataMgr->Get()->iChallengeType = ChallengeMgrMgr->GetSelectMgr()->GetSelectType();

	// (1/21) �}�l�[�W���[�̒������Ŋ���������	
	ChallengeMgrMgr->GetSelectMgr()->Init(SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);

	// ��񏉊���
	pMain->Reset();

	// SP�Q�[�W��������
	PlayerMgr->GetPlayer(SIDE::LEFT)->SetOverDriveGage(0);
	PlayerMgr->GetPlayer(SIDE::RIGHT)->SetOverDriveGage(0);
	
	// �����|�W�V�����̕ύX(�E����X�^�[�g�Ƃ��^�񒆂���Ƃ�)
	pMain->StartChallengePosition();

	// �����I�����Ă郊�v���C�f�[�^��ǂݍ��ރA�N�V����
	CommandMgr->Action(ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->GetReplayDataName().c_str());

}

void SceneMainState::ChallengeReplay::Execute(sceneMain *pMain)
{
	// �������@(12/28)�`�������W�ł��X�V
	PlayerMgr->UpdateTraining();


	// �v���C���[�X�V
	if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->isClear())
	{
		// �N���A�����̂œ������~�߂�
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();
	}
	else
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
			// �ʏ�
			PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);
		}
		else
		{
			// �t�F�[�h���͑�����󂯕t���Ȃ�
			PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
		}

		// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// �v���C���[�ʒu�m��
		PlayerMgr->UpdatePos();
	}

	// �J�����X�V
	CameraMgr->Update();

	// 1P�̑��삵�Ă���v���C���[�̏����m�F���N���A������o���I
	ChallengeMgrMgr->GetSelectMgr()->TaskUpdate(PlayerMgr->GetPlayer(0));

	// �`�������W�X�V
	ChallengeMgrMgr->GetSelectMgr()->Update();

	//// �������@�^�X�N��S�ăN���A���Ďw�肵�����Ԃ��߂���΁@���C���̃X�e�[�g�ց@������
	//if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->EndTipsStart())
	//{
	//	pMain->GetFSM()->ChangeState(ChallengeMain::GetInstance());
	//	return;
	//}

	/********************************************/
	// �������X�^�[�g

	// �t�F�[�h�A�E�g������
	if (Fade::isFadeOutCompletion())
	{
		// �`�������W�̃C���g����
		pMain->GetFSM()->ChangeState(ChallengeMain::GetInstance());
		return;
	}

	if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
	{
		// �N���A�\�L���̓��X�^�[�g�ł��Ȃ�
		if (ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->isClear() == false)
		{
			// �Z���N�g�ł�����x�C���g���֖߂�
			if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
			{
				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);
				return;
			}
		}
		
		// ���v���C�X�V
		CommandMgr->Update(SIDE::LEFT);


		// �R�}���h�̍Đ����I�������߂�
		if (CommandMgr->isEnd())
		{
			// �t�F�[�h�A�E�g
			Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);
			return;
		}
	}


	/********************************************/

#ifdef _DEBUG



#endif // _DEBUG

}

void SceneMainState::ChallengeReplay::Exit(sceneMain *pMain)
{
	// �����v���C�t���O��߂�
	ChallengeMgrMgr->GetSelectMgr()->GetChallenge()->SetReplay(false);

	// �X�g�b�v
	CommandMgr->Stop();
}

void SceneMainState::ChallengeReplay::Render(sceneMain *pMain)
{
	ChallengeMgrMgr->GetSelectMgr()->Render();
}

bool SceneMainState::ChallengeReplay::OnMessage(sceneMain *pMain, const Message & msg)
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
//			�g���[�j���O�X�e�[�g
/*******************************************************/

void SceneMainState::TrainingIntro::Enter(sceneMain *pMain)
{
	if (Fade::isFadeOutCompletion())
	{
		// �t�F�[�h������
		Fade::Set(Fade::FLAG::FADE_IN, 12, 0x00000000);
	}

	// ��񏉊���
	pMain->Reset();
	
	// �������g���[�j���O�f�[�^�K�p
	pMain->InitTrainingData();

	// �S���N
	// SP�Q�[�W��������
	PlayerMgr->GetPlayer(SIDE::LEFT)->SetOverDriveGage(0);
	PlayerMgr->GetPlayer(SIDE::RIGHT)->SetOverDriveGage(0);

	// �����|�W�V�����̕ύX(�E����X�^�[�g�Ƃ��^�񒆂���Ƃ�)
	pMain->StartPosition();

	// �g���[�j���O��
	pMain->GetFSM()->ChangeState(Training::GetInstance());
	return;
}

// �X�V
void SceneMainState::TrainingIntro::Execute(sceneMain *pMain)
{
	// �g���[�j���O��
	pMain->GetFSM()->ChangeState(Training::GetInstance());
	return;
}

void SceneMainState::TrainingIntro::Exit(sceneMain *pMain)
{

}

void SceneMainState::TrainingIntro::Render(sceneMain *pMain)
{

}

bool SceneMainState::TrainingIntro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//			�g���[�j���O�X�e�[�g
/*******************************************************/

void SceneMainState::Training::Enter(sceneMain *pMain)
{
	// ���j���[����߂��Ă����Ƃ��Ƀg���[�j���O�f�[�^�K�p
	pMain->InitTrainingData();
}

// �X�V
void SceneMainState::Training::Execute(sceneMain *pMain)
{
	//#ifdef ROUND_SKIP
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//#endif

	// ���K���Ԍv��
	TrophyMgr->CheakTrainingTime();

	// �v���C���[�X�V
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);
	PlayerMgr->UpdatePos();

	// �������@(12/24)�g���[�j���O�p�̍X�V
	PlayerMgr->UpdateTraining();


	// �v���C���[�Ƃ��낢�딻��(�������ɏ��������R�̓V���b�g�}�l�[�W���[�Ƃ��X�e�[�W��sceneMain�������Ă��邩��)
	Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

	// �J�����X�V
	CameraMgr->Update();

	/********************************************/
	// �������X�^�[�g
	if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
	{
		// �p�b�h���X�V
		int NumDevice(tdnInputManager::GetNumDevice());
		if (NumDevice == 0)NumDevice = 1;// �f�o�C�X���Ȃ������牼�z��1�ǉ�
		for (int i = 0; i < NumDevice; i++)
		{
			// ���|�[�Y�Ńg���[�j���O��p�̃|�[�Y���j���[��
			if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, i) == 3)
			{
				// (12/24) �����ŋN��
				//pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Action();
				pMain->SetPauseDeviceID(i);// ���|�[�Y���j���[���������l�ۑ�
				pMain->GetFSM()->ChangeState(TrainingPause::GetInstance());
				return;
			}

			// �Z���N�g�ł�����x�C���g���֖߂�
			if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, i) == 3)
			{
				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 14, 0x00000000);
				return;
			}

		}


	}
	else
	{
		// �\���L�[�������ςŏ����ʒu��ς���
		// �p�b�h���X�V
		// �X�e�B�b�N�̎擾
		float x, y;
		int NumDevice(tdnInputManager::GetNumDevice());
		if (NumDevice == 0)NumDevice = 1;// �f�o�C�X���Ȃ������牼�z��1�ǉ�
		for (int i = 0; i < NumDevice; i++)
		{
			// 
			tdnInput::GetAxisXYf(&x, &y, i);
			static const float gosa(.2f);
			if (x > .5f - gosa ||
				tdnInput::KeyGet(KEYCODE::KEY_RIGHT, i) == 1)
			{
				SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;
			}
			else if (x < -.5f + gosa ||
				tdnInput::KeyGet(KEYCODE::KEY_LEFT, i) == 1)
			{
				SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::LEFT;
			}
			else if (y < -.5f + gosa ||
				tdnInput::KeyGet(KEYCODE::KEY_DOWN, i) == 1)
			{
				SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::MIDDLE;
			}

		}
	}

	// �t�F�[�h�A�E�g������
	if (Fade::isFadeOutCompletion())
	{
		// �`���[�g���A���̃C���g����
		pMain->GetFSM()->ChangeState(TrainingIntro::GetInstance());
		return;
	}

}

void SceneMainState::Training::Exit(sceneMain *pMain)
{

}

void SceneMainState::Training::Render(sceneMain *pMain)
{
	// �������@(12/24)�g���[�j���O�p�̕`��
	PlayerMgr->RenderTraining();

}

bool SceneMainState::Training::OnMessage(sceneMain *pMain, const Message & msg)
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
//			�|�[�Y���j���[�X�e�[�g
/*******************************************************/

void SceneMainState::PauseMenu::Enter(sceneMain *pMain)
{	
	// (12/24) �����ŋN������Ɩ߂����ꍇ�ł��Ă΂��̂Ń��j���[���ĂԂƂ��ŋN��
	// (�ǋL)�@����ς肱����if���Ȃǎg���ď���
	// �|�[�Y�E�B���h�E�N�� 
	pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Action();
	pMain->SetPause(true);
	
}

// �X�V
void SceneMainState::PauseMenu::Execute(sceneMain *pMain)
{
	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+--------------------------------------------
	//	�|�[�Y���j���[�̑���
	//+--------------------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	 {
		 // �|�[�Y�E�B���h�E�̑���
		 pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Ctrl(l_iDeviceID);
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

		// �|�[�Y�I��
		pMain->SetPause(false);
			pMain->GetFSM()->ChangeState(Main::GetInstance());	// ���C���֖߂�

		//}

	}

	// �R�}���h���X�g��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState()
		== PauseWindow::BATTLE_STATE::COMMAND_LIST)
	{
		// 
		pMain->GetFSM()->ChangeState(CommandMenu::GetInstance());
		return;
	}

	// �T�E���h��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState()
		== PauseWindow::BATTLE_STATE::SOUND_SETTING)
	{
		// �T�E���h�ݒ��
		pMain->GetFSM()->ChangeState(SoundMenu::GetInstance());
		return;
	}

	// ���j���[��\����
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState() 
		== PauseWindow::BATTLE_STATE::HIDE_MENU)
	{
		pMain->GetFSM()->ChangeState(HideMenu::GetInstance());
		return;
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
	// ������Exit�ł̓E�B���h�E�͎~�߂Ȃ�

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
	// (12/24) �����ŋN������Ɩ߂����ꍇ�ł��Ă΂��̂Ń��j���[���ĂԂƂ��ŋN��
	// (�ǋL)�@����ς肱����if���Ȃǎg���ď���
	// ���`���[�g���A���|�[�Y�E�B���h�E�N�� 
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
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		// �|�[�Y�E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Ctrl(i);
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
	

	// �`���[�g���A���Z���N�g����������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState()
		== TutorialPauseWindow::TUTORIAL_PAUSE_STATE::SELECT_TUTORIAL)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();
		// �|�[�Y�I��
		pMain->SetPause(false);

		// �����݂̃`���[�g���A����ύX
		TutorialMgr->SetSelectType((TUTORIAL_TYPE)SelectDataMgr->Get()->iTutorialType);

		pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());	// �`���[�g���A���֖߂�

		return;
	}


	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::BACK)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();
		// �|�[�Y�I��
		pMain->SetPause(false);

		// ���Z���N�g�`���[�g���A����ύX�������ǌ���������Ȃ����������I�����Ă���`���[�g���A���ւ��ǂ�
		SelectDataMgr->Get()->iTutorialType = TutorialMgr->GetSelectNo();

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
	
	// �R�}���h���X�g��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::COMMAND_LIST)
	{
		// �R�}���h���X�g
		pMain->GetFSM()->ChangeState(SceneMainState::CommandMenu::GetInstance());	
		return;
	}

}

void SceneMainState::TutorialPauseMenu::Exit(sceneMain *pMain)
{

}

void SceneMainState::TutorialPauseMenu::Render(sceneMain *pMain)
{
	// 
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->RednerInfo();
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


/*******************************************************/
//			�`�������W�|�[�Y���j���[�X�e�[�g
/*******************************************************/

void SceneMainState::ChallengePauseMenu::Enter(sceneMain *pMain)
{

	pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Action();
	pMain->SetPause(true);

}

// �X�V
void SceneMainState::ChallengePauseMenu::Execute(sceneMain *pMain)
{
	// �t�F�[�h���ĂȂ���
	if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
	{

		//+--------------------------------------------
		//	�`�������W�|�[�Y���j���[�̑���
		//+--------------------------------------------
		// �p�b�h���X�V
		int NumDevice(tdnInputManager::GetNumDevice());
		// �p�b�h�����������ĂȂ��Ƃ��p
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			// �|�[�Y�E�B���h�E�̑���
			pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Ctrl(i);
		}




		// �v���C���[�X�V���~�߂�
		//PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
		//PlayerMgr->UpdatePos();

		// �J�����X�V
		//CameraMgr->Update();

		// ��O�̃`�������W�֖߂�{�^������������
		//if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState() == TutorialPauseWindow::CHALLENGE_PAUSE_STATE::BACK_PREV_TUTORIAL)
		//{
		//	// �|�[�Y�E�B���h�E�~�߂�
		//	pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Stop();

		//	pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());	

		//}


		// �`�������W�Z���N�g����������
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState()
			== ChallengePauseWindow::CHALLENGE_PAUSE_STATE::SELECT_CHALLENGE)
		{
			// �|�[�Y�E�B���h�E�~�߂�
			pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Stop();
			// �|�[�Y�I��
			pMain->SetPause(false);

			// �����݂̃`�������W��ύX
			ChallengeMgrMgr->GetSelectMgr()->SetSelectType(SelectDataMgr->Get()->iChallengeType);

			pMain->GetFSM()->ChangeState(ChallengeIntro::GetInstance());	// �`�������W�֖߂�

			return;
		}

		// �`�������W���v���C����������
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState()
			== ChallengePauseWindow::CHALLENGE_PAUSE_STATE::CHALLENGE_REPLAY)
		{
			// �|�[�Y�E�B���h�E�~�߂�
			pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Stop();

			// ���t�F�[�h�A�E�g(���̃t�F�[�h�Ń��v���C��)
			Fade::Set(Fade::FLAG::FADE_OUT, 14, 0x00000000);

			// �|�[�Y�I��
			pMain->SetPause(false);

			return;
		}

		// �|�W�V�������Z�b�g�{�^������������
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState()
			== ChallengePauseWindow::CHALLENGE_PAUSE_STATE::POSITION_RESET)
		{
			// �|�[�Y�E�B���h�E�~�߂�
			pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Stop();
			// �t�F�[�h�A�E�g(�|�W�V�������Z�b�g���ʂ̈�������)
			Fade::Set(Fade::FLAG::FADE_OUT, 14, 0x00000000);
			// �|�[�Y�I��
			pMain->SetPause(false);
			pMain->GetFSM()->ChangeState(ChallengeIntro::GetInstance());	// �߂�
			return;
		}

		// �T�E���h��
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState()
			== ChallengePauseWindow::CHALLENGE_PAUSE_STATE::SOUND_SETTING)
		{
			// �T�E���h�ݒ��
			pMain->GetFSM()->ChangeState(SoundMenu::GetInstance());
			return;
		}

		// ���j���[��\����
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState()
			== ChallengePauseWindow::CHALLENGE_PAUSE_STATE::HIDE_MENU)
		{
			pMain->GetFSM()->ChangeState(HideMenu::GetInstance());
			return;
		}

		// �߂�{�^������������
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState() == ChallengePauseWindow::CHALLENGE_PAUSE_STATE::BACK)
		{
			// �|�[�Y�E�B���h�E�~�߂�
			pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Stop();
			// �|�[�Y�I��
			pMain->SetPause(false);

			// ���Z���N�g�`�������W��ύX�������ǌ���������Ȃ����������I�����Ă���`�������W�ւ��ǂ�
			SelectDataMgr->Get()->iChallengeType = ChallengeMgrMgr->GetSelectMgr()->GetSelectType();

			pMain->GetFSM()->ChangeState(ChallengeMain::GetInstance());	// �`�������W�֖߂�

		}

		// ���j���[��
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState() == ChallengePauseWindow::CHALLENGE_PAUSE_STATE::BACK_MENU_SELECT)
		{
			// �|�[�Y�E�B���h�E�~�߂�
			pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Stop();
			MainFrameEx->ChangeScene(new sceneMenu());
			return;
		}

		// �R�}���h���X�g��
		if (pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->GetChoiceState() == ChallengePauseWindow::CHALLENGE_PAUSE_STATE::COMMAND_LIST)
		{
			// �R�}���h���X�g
			pMain->GetFSM()->ChangeState(SceneMainState::CommandMenu::GetInstance());
			return;
		}


	}

	// �t�F�[�h�A�E�g������
	if (Fade::isFadeOutCompletion())
	{
		// ���`�������W�̃��v���C��
		pMain->GetFSM()->ChangeState(ChallengeReplay::GetInstance());
		return;
	}

}

void SceneMainState::ChallengePauseMenu::Exit(sceneMain *pMain)
{

}

void SceneMainState::ChallengePauseMenu::Render(sceneMain *pMain)
{
	// �t�F�[�h���ĂȂ���
	if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
	{
		// 
		pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->RednerInfo();
	}

}

bool SceneMainState::ChallengePauseMenu::OnMessage(sceneMain *pMain, const Message & msg)
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
//			�g���[�j���O�|�[�Y���j���[�X�e�[�g
/*******************************************************/

void SceneMainState::TrainingPause::Enter(sceneMain *pMain)
{
	// (12/24) �����ŋN������Ɩ߂����ꍇ�ł��Ă΂��̂Ń��j���[���ĂԂƂ��ŋN��
	// (�ǋL)�@����ς肱����if���Ȃǎg���ď���
	// �g���[�j���O�|�[�Y�E�B���h�E�N�� 
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Action();
	pMain->SetPause(true);

}

// �X�V
void SceneMainState::TrainingPause::Execute(sceneMain *pMain)
{

	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();
	
	//+--------------------------------------------
	//	�g���[�j���O�|�[�Y���j���[�̑���
	//+--------------------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		// �|�[�Y�E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Ctrl(l_iDeviceID);
	}
	


	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState() == TrainingPauseWindow::TRAINING_PAUSE_STATE::BACK)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Stop();
		
		// �|�[�Y�I��
		pMain->SetPause(false);

		pMain->GetFSM()->ChangeState(Training::GetInstance());	// �g���[�j���O�֖߂�
		return;

	}

	// �g���[�j���O�I�v�V������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState()
		== TrainingPauseWindow::TRAINING_PAUSE_STATE::TRAINING_SETTING)
	{
		// �g���[�j���O�I�v�V�����ݒ��
		pMain->GetFSM()->ChangeState(TrainingOptionMenu::GetInstance());
		return;
	}

	// �g���[�j���O�_�~�[��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState()
		== TrainingPauseWindow::TRAINING_PAUSE_STATE::TRAINING_DUMMY_SETTING)
	{
		// �g���[�j���O�_�~�[�ݒ��
		pMain->GetFSM()->ChangeState(TrainingDummyMenu::GetInstance());
		return;
	}

	// �g���[�j���O�L�����N�^�[��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState()
		== TrainingPauseWindow::TRAINING_PAUSE_STATE::TRAINING_CHARACTER_SETTING)
	{
		// �g���[�j���O�L�����N�^�[�ݒ��
		pMain->GetFSM()->ChangeState(TrainingCharacterMenu::GetInstance());
		return;
	}

	// �|�W�V�������Z�b�g�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState()
		== TrainingPauseWindow::TRAINING_PAUSE_STATE::POSITION_RESET)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Stop();	
		// �t�F�[�h�A�E�g(�|�W�V�������Z�b�g���ʂ̈�������)
		Fade::Set(Fade::FLAG::FADE_OUT, 14, 0x00000000);
		// �|�[�Y�I��
		pMain->SetPause(false);
		pMain->GetFSM()->ChangeState(Training::GetInstance());	// �g���[�j���O�֖߂�
		return;
	}

	// �R�}���h���X�g��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState()
		== TrainingPauseWindow::TRAINING_PAUSE_STATE::COMMAND_LIST)
	{
		// 
		pMain->GetFSM()->ChangeState(CommandMenu::GetInstance());
		return;
	}

	// �T�E���h��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState() == TrainingPauseWindow::TRAINING_PAUSE_STATE::SOUND_SETTING)
	{
		// �T�E���h�ݒ��
		pMain->GetFSM()->ChangeState(SoundMenu::GetInstance());	
		return;
	}

	// ���j���[��\����
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState() == TrainingPauseWindow::TRAINING_PAUSE_STATE::HIDE_MENU)
	{
		pMain->GetFSM()->ChangeState(HideMenu::GetInstance());
		return;
	}

	// �L�����N�^�[�Z���N�g��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState() == TrainingPauseWindow::TRAINING_PAUSE_STATE::BACK_CHARA_SELECT)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneSelect());
		return;
	}

	// ���j���[��
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->GetChoiceState() == TrainingPauseWindow::TRAINING_PAUSE_STATE::BACK_MENU_SELECT)
	{
		// �|�[�Y�E�B���h�E�~�߂�
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneMenu());
		return;
	}

}

void SceneMainState::TrainingPause::Exit(sceneMain *pMain)
{
	//pMain->SetPause(false);// ������Exit�ł̓E�B���h�E�͎~�߂Ȃ�
}

void SceneMainState::TrainingPause::Render(sceneMain *pMain)
{
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->RednerInfo();
}

bool SceneMainState::TrainingPause::OnMessage(sceneMain *pMain, const Message & msg)
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
//				�T�E���h�E�B���h�E�I��
/*******************************************************/

void SceneMainState::SoundMenu::Enter(sceneMain *pMain)
{
	// �T�E���h�E�B���h�E�N��
	pMain->GetWindow(BATTLE_WINDOW_TYPE::SOUND)->Action();
}

void SceneMainState::SoundMenu::Execute(sceneMain *pMain)
{

	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::SOUND)->GetChoiceState()
		== SoundWindow::SOUND_STATE::BACK)
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::SOUND)->Stop();	// �E�B���h�E�����
		pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
		return;
	}



	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{	
		// �E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::SOUND)->Ctrl(l_iDeviceID);
		
	}

}

void SceneMainState::SoundMenu::Exit(sceneMain *pMain)
{
	
}

void SceneMainState::SoundMenu::Render(sceneMain *pMain)
{

	// �I���E�B���h�E�̐���
	pMain->GetWindow(BATTLE_WINDOW_TYPE::SOUND)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "SoundMenu");
#endif // _DEBUG

}

bool SceneMainState::SoundMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//				�g���[�j���O�I�v�V�����E�B���h�E�I��
/*******************************************************/

void SceneMainState::TrainingOptionMenu::Enter(sceneMain *pMain)
{
	// �g���[�j���O�I�v�V�����N��
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_OPTION)->Action();
}

void SceneMainState::TrainingOptionMenu::Execute(sceneMain *pMain)
{

	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_OPTION)->GetChoiceState()
		== TrainingOptionWindow::TRAINING_OPTION_STATE::BACK)
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_OPTION)->Stop();	// �E�B���h�E�����
		pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
		return;
	}



	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		// �E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_OPTION)->Ctrl(l_iDeviceID);
	}
	

}

void SceneMainState::TrainingOptionMenu::Exit(sceneMain *pMain)
{

}

void SceneMainState::TrainingOptionMenu::Render(sceneMain *pMain)
{
	// �I���E�B���h�E�̐���
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_OPTION)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TrainingOptionMenu");
#endif // _DEBUG

}

bool SceneMainState::TrainingOptionMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//				�g���[�j���O�_�~�[�E�B���h�E�I��
/*******************************************************/

void SceneMainState::TrainingDummyMenu::Enter(sceneMain *pMain)
{
	// �g���[�j���O�I�v�V�����N��
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_DUMMY)->Action();
}

void SceneMainState::TrainingDummyMenu::Execute(sceneMain *pMain)
{

	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_DUMMY)->GetChoiceState()
		== TrainingDummyWindow::TRAINING_DUMMY_STATE::BACK)
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_DUMMY)->Stop();	// �E�B���h�E�����
		pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
		return;
	}



	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		// �E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_DUMMY)->Ctrl(l_iDeviceID);
	}


}

void SceneMainState::TrainingDummyMenu::Exit(sceneMain *pMain)
{

}

void SceneMainState::TrainingDummyMenu::Render(sceneMain *pMain)
{
	// �I���E�B���h�E�̐���
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_DUMMY)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TRAINING_DUMMY");
#endif // _DEBUG

}

bool SceneMainState::TrainingDummyMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//				�g���[�j���O�L�����N�^�[�E�B���h�E�I��
/*******************************************************/

void SceneMainState::TrainingCharacterMenu::Enter(sceneMain *pMain)
{
	// �g���[�j���O�I�v�V�����N��
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_CHARACTER)->Action();
}

void SceneMainState::TrainingCharacterMenu::Execute(sceneMain *pMain)
{

	// �߂�{�^������������
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_CHARACTER)->GetChoiceState()
		== TrainingCharacterWindow::TRAINING_CHARACTER_STATE::BACK)
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_CHARACTER)->Stop();	// �E�B���h�E�����
		pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
		return;
	}



	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		// �|�[�Y�E�B���h�E�̑���
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_CHARACTER)->Ctrl(l_iDeviceID);
	}


}

void SceneMainState::TrainingCharacterMenu::Exit(sceneMain *pMain)
{

}

void SceneMainState::TrainingCharacterMenu::Render(sceneMain *pMain)
{
	// �I���E�B���h�E�̐���
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_CHARACTER)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TrainingCharacterMenu");
#endif // _DEBUG

}

bool SceneMainState::TrainingCharacterMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//				���j���[��\��
/*******************************************************/

void SceneMainState::HideMenu::Enter(sceneMain *pMain)
{
	// �O��̃X�e�[�g�ŕ\������Ă��郁�j���[��ʂ�����
	if (pMain->GetFSM()->isPrevState(*SceneMainState::PauseMenu::GetInstance()))
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Stop();
	}
	if (pMain->GetFSM()->isPrevState(*SceneMainState::TrainingPause::GetInstance()))
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TRAINING_PAUSE)->Stop();
	}
	if (pMain->GetFSM()->isPrevState(*SceneMainState::TutorialPauseMenu::GetInstance()))
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();
	}
	if (pMain->GetFSM()->isPrevState(*SceneMainState::ChallengePauseMenu::GetInstance()))
	{
		pMain->GetWindow(BATTLE_WINDOW_TYPE::CHALLENGE_PAUSE)->Stop();
	}

	// ��\���C���t�H�J�n
	pMain->GetHideInfo()->Action();
}

void SceneMainState::HideMenu::Execute(sceneMain *pMain)
{
	// ��\���C���t�H�_��
	if (pMain->GetHideInfo()->GetAction()->IsEnd() == true)
	{
		pMain->GetHideInfo()->Action();
	}

	// ��\���C���t�H
	pMain->GetHideInfo()->Update();
	
	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		// UI��\��
		if (KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{	
			if (pMain->GetHideUI() == false)
			{
				pMain->SetHideUI(true);
			}
			else
			{
				pMain->SetHideUI(false);
			}
		}

		// �߂�{�^������������
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 )
		{
			pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
			return;
		}

	}


}

void SceneMainState::HideMenu::Exit(sceneMain *pMain)
{
	pMain->SetHideUI(false);// UI��\��������i���ɂ��ǂ��j
}

void SceneMainState::HideMenu::Render(sceneMain *pMain)
{
	// ��\���C���t�H
	pMain->GetHideInfo()->Render(464, 640);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "HideMenu");
#endif // _DEBUG

}

bool SceneMainState::HideMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//				�R�}���h���X�g�\��
/*******************************************************/

void SceneMainState::CommandMenu::Enter(sceneMain *pMain)
{
	// �R�}���h�\��I�������v���C���[��SIDE��ۑ�	
	pMain->SetCommandSide(PlayerMgr->GetPlayerByDeviceID(pMain->GetPauseDeviceID())->GetSide());
	
	// ����AI��������AI�͓����f�o�C�X�Ȃ̂ŕς���K�v������
	if (PlayerMgr->GetPlayer(pMain->GetCommandSide())->isAI() == true)
	{
		// SIDE��ς��܂�
		if (pMain->GetCommandSide() == SIDE::LEFT)
		{
			pMain->SetCommandSide(SIDE::RIGHT);
		}
		else
		{
			pMain->SetCommandSide(SIDE::LEFT);
		}
	}

	// �{�^�������������̃R�}���h�\�\��
	PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->Action();

}

void SceneMainState::CommandMenu::Execute(sceneMain *pMain)
{
	int l_iDeviceID = pMain->GetPauseDeviceID();
	

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{

		// �R�}���h�\�؂�ւ�
		if (KEY(KEY_LEFT, i) == 3||
			KEY(KEY_RIGHT, i) == 3)
		{
			// ���݂̃R�}���h�E�B���h�E���~�߂܂�
			PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->Stop();

			// SIDE��ς��܂�
			if (pMain->GetCommandSide() == SIDE::LEFT)
			{
				pMain->SetCommandSide(SIDE::RIGHT);
			}
			else
			{
				pMain->SetCommandSide(SIDE::LEFT);
			}

			// �V�����R�}���h�E�B���h�E���L����
			PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->Action();

		}

		// �߂�{�^������������
		if (KEY(KEY_A, i) == 1)
		{
			PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->Stop();	// �E�B���h�E�����
			pMain->GetFSM()->RevertToPreviousState();				// �O�̃X�e�[�g�֖߂�
			return;
		}

	}

	// �R�}���h�E�B���h�E�̑���
	PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->Ctrl(l_iDeviceID);

	// �R�}���h�E�B���h�E�̍X�V
	PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->Update();

}

void SceneMainState::CommandMenu::Exit(sceneMain *pMain)
{

}

void SceneMainState::CommandMenu::Render(sceneMain *pMain)
{
	// �I���E�B���h�E�̊G
	PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->Redner();

	// �I���E�B���h�E�̐���
	PlayerMgr->GetPlayer(pMain->GetCommandSide())->GetCommandWindow()->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "CommandMenu");

#endif // _DEBUG

}

bool SceneMainState::CommandMenu::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}
