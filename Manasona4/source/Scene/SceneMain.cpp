#include "TDNLIB.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../Effect/Particle.h"
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"
#include "../Timer/Timer.h"
#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "SceneMainState.h"
#include "../Fade/Fade.h"
#include "../DeferredEx/DeferredEx.h"
#include "../SceneSwitch/SceneSwitch.h"
#include "../PointLight/PointLight.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "../Number/Number.h"
#include "../UI/GameUI.h"
#include	"../Cutin/CutIn.h"
#include "../Sound/BattleMusic.h"
#include "UI/HeaveHoFinish/HeaveHoFinish.h"
#include "../Shot/ShotManager.h"
#include "RoundCall\RoundCallManager.h"
#include "Stage\OverDriveStage\OverDriveStage.h"



//BaseEffect* g_eff;
//EffectManager;
PanelEffectManager* m_panel;
UVEffectManager* g_uvEffect;

int stopTimer = 0;

//******************************************************************
//		�������E���
//******************************************************************
sceneMain::sceneMain() :BaseGameEntity(ENTITY_ID::SCENE_MAIN){}
bool sceneMain::Initialize()
{
	// ���E���h��
	// (TODO)[11/4]���E���h����0�̎��̓��E���h�R�[���𔲂�������������
	// �g���[�j���O��`���[�g���A���̂���
	if (SelectDataMgr->Get()->bTutorial == false &&
		SelectDataMgr->Get()->bTraining == false)
	{
		//m_iRoundNum = SelectDataMgr->Get()->iWinRound;
		switch ((ROUND_NUM_TYPE)PlayerDataMgr->m_ConfigData.iRoundNumType)
		{
		case ROUND_NUM_TYPE::ROUND_1:
			m_iRoundNum = 1;
			break;
		case ROUND_NUM_TYPE::ROUND_2:
			m_iRoundNum = 2;
			break;
		case ROUND_NUM_TYPE::ROUND_3:
			m_iRoundNum = 3;
			break;
		case ROUND_NUM_TYPE::ROUND_4:
			m_iRoundNum = 4;
			break;
		default:
			MyAssert(0, "���̃��E���h���̃^�C�v�͂Ȃ�");
			break;
		}
	}
	else
	{
		m_iRoundNum = 0;// ���E���h����0���Ɩ������[�v(�`���[�g���A���E�g���[�j���O)
	}

	m_dirLight = Vector3(1, -1, 1);

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "�p�l���G�t�F�N�g�������҂�");
#endif
	m_panel = new PanelEffectManager();


#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "UV�G�t�F�N�g������");
#endif
	g_uvEffect = new UVEffectManager();


#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "�X�e�[�W������");
#endif

	// �X�e�[�W������(���Z���N�g�����X�e�[�W�̃^�C�v�������ɓn��)
	Stage::Base::CreateStage(&m_pStage, SelectDataMgr->Get()->eStage, CameraMgr);	// �֐��̒��ō���Ă��炤
	Collision::SetStage(m_pStage);

	//std::unique_ptr<std::thread> PlayerLoadThread(new std::thread([&](){
#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "�v���C���[������");
#endif
	PlayerMgr->Initialize(2, m_pStage, SelectDataMgr->Get()->tagSideDatas);

	// �v���C���[�̍��W�̃A�h���X���J�����ɓn���Ă�����(�����邱�Ƃ͐�΂ɖ����A�����Q�Ƃ��邾��)
	CameraMgr->SetPlayersPos();

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "�Q�[��UI������");
#endif
	BasePlayer* p1 = PlayerMgr->GetPlayer_TeamInSearch(SIDE::LEFT);
	BasePlayer* p2 = PlayerMgr->GetPlayer_TeamInSearch(SIDE::RIGHT);
	GameUIMgr;
	
	if (SelectDataMgr->Get()->bTutorial == false &&
		SelectDataMgr->Get()->bTraining == false)
	{
		GameUIMgr->InitData(p1, p2, m_iRoundNum, (ROUND_TIME_TYPE)PlayerDataMgr->m_ConfigData.iRoundTimeType);
	}
	else
	{
		// �퓬�ȊO�͂ǂ̐ݒ�ɂ��Ă���
		GameUIMgr->InitData(p1, p2, m_iRoundNum, (ROUND_TIME_TYPE::SEC_INFINITY));
	}

	//}));


#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "�f�B�t�@�[�h������");
#endif

	DeferredManagerEx;
	DeferredManagerEx.InitShadowMap(1024);
	m_bShaderFlag = true;

	m_fLoadPercentage = .6f;	// ���[�h����

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "�|�C���g���C�g������");
#endif

	PointLightMgr;

	NumberEffect;

	m_stageScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "���̑�������");
#endif

	m_pShotMgr = new ShotManager;

	m_fLoadPercentage = .8f;	// ���[�h����

	HeaveHoFinishUI;
	CutInMgr;

	m_fLoadPercentage = .85f;	// ���[�h����

	m_fLoadPercentage = 1.0f;	// ���[�h����

	// �V�[���X�C�b�`
	prevEF = new SceneSwitchPrev();

	// ���E���h�R�[���}�l�[�W���[
	m_pRoundCallMgr = new RoundCallManager();

	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneMain>(this);

	//�iTODO�j�`���[�g���A���E�g���[�j���O�������番��
	m_bTutorialFlag = SelectDataMgr->Get()->bTutorial;
	m_eSelectTutorial = (TUTORIAL_TYPE)0;// (TODO)�I�񂾃`���[�g���A����ݒ肵�Ă����� 					 

	m_bTrainingFlag = SelectDataMgr->Get()->bTraining;


	if (m_bTutorialFlag == true)
	{
		// UI���ŏ��̈�񂾂��N��
		GameUIMgr->Action();
		// �`���[�g���A���p�̑ΐ�
		m_pStateMachine->SetCurrentState(SceneMainState::TutorialIntro::GetInstance());

	}
	else if (m_bTrainingFlag == true)
	{
		// UI���ŏ��̈�񂾂��N��
		GameUIMgr->Action();
		// �g���[�j���O�p�̑ΐ�
		m_pStateMachine->SetCurrentState(SceneMainState::Training::GetInstance());
	}
	else
	{
		// �ʏ�̑ΐ�
		m_pStateMachine->SetCurrentState(SceneMainState::StageIntro::GetInstance());	// �X�e�[�g�̐ݒ�
	}

	m_pMaskScreen = new tdn2DObj("Data/UI/Game/stageScreenMask.png");
	m_fMaskRate = 0.0f;
	m_fMaskPower = 0.0f;
	m_bOverDriveStageFlag = false;

	m_pOverDriveStage = new OverDriveStage();

	// �E�B���h�E��
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		switch ((BATTLE_WINDOW_TYPE)i)
		{
		case BATTLE_WINDOW_TYPE::PAUSE:
			m_pWindow[i] = new PauseWindow(Vector2(424, 128));
			break;
		case BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE:
			m_pWindow[i] = new TutorialPauseWindow(Vector2(424, 128));
			break;
		case BATTLE_WINDOW_TYPE::TRAINING_PAUSE:
			m_pWindow[i] = new TrainingPauseWindow(Vector2(424, 128));
			break;
		case BATTLE_WINDOW_TYPE::SOUND:
			m_pWindow[i] = new SoundWindow(Vector2(324, 228));
			break;
		case BATTLE_WINDOW_TYPE::TRAINING_OPTION:
			m_pWindow[i] = new TrainingOptionWindow(Vector2(224, 128));
			break;
		case BATTLE_WINDOW_TYPE::TRAINING_DUMMY:
			m_pWindow[i] = new TrainingDummyWindow(Vector2(224, 128));
			break;
		default:
			MyAssert(0, "����ȃE�B���h�E�͂Ȃ�");
			break;
		}
	}

	// �|�[�Y���Ă邩
	m_bPause = false;

	// �X�e�[�W�e�Ă�����
	m_bBakeStageShadow = true;

	// �v���C���[�̓ǂݍ��݂��I���܂őҋ@
	//PlayerLoadThread->join();

	// BGM����
	bgm->PlayStreamIn((LPSTR)BattleMusicMgr->GetMusicFilePath(SelectDataMgr->Get()->iBattleMusicID).c_str());

	return true;
}

sceneMain::~sceneMain()
{
	delete prevEF;
	delete m_pStateMachine;
	//delete m_pMyMusicMgr;
	bgm->StopStreamIn();
	delete m_pStage;
	//delete CameraMgr;
	PlayerMgr->Release();
	//EffectMgr.Release();
	SAFE_DELETE(m_panel);
	SAFE_DELETE(g_uvEffect);
	//ParticleManager::Release();
	DeferredManagerEx.Release();
	SAFE_DELETE(m_stageScreen);
	HeaveHoFinishUI->Rerease();
	PointLightMgr->Release();
	delete m_pShotMgr;
	NumberEffect.Release();
	GameUIMgr->Rerease();
	//TimeMgr->Release();
	CutInMgr->Rerease();
	SAFE_DELETE(m_pRoundCallMgr);
	SAFE_DELETE(m_pMaskScreen);
	SAFE_DELETE(m_pOverDriveStage);
	//SAFE_DELETE(com);
	// �E�B���h�E��
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pWindow[i]);
	}
}

//******************************************************************
//		����
//******************************************************************

void sceneMain::Update()
{
	HeaveHoFinishUI->Update();
	if (HeaveHoFinishUI->IsAction() == true)return ;
	
	// �t�F�[�h�X�V
	Fade::Update();

	// �|�[�Y���͍X�V���Ȃ�
	if (m_bPause == false)
	{

		//	com->Update();

		stopTimer++;
		//if (stopTimer > 60 * 60)
		//{
		//	return true;
		//}

		// �J�����X�V(�X�e�[�g�}�V���ɏ�����)
		//CameraMgr->Update();

		// �V���b�g�X�V
		m_pShotMgr->Update();

		// �X�e�[�W�X�V
		m_pStage->Update();

		// �v���C���[�X�V
		//PlayerMgr->Update();

		// �p�[�e�B�N���X�V
		ParticleManager::Update();

		static float lightAngle = 2.14f;
		// �A���O��
		if (KeyBoard(KB_R)) { lightAngle -= 0.05f; }
		if (KeyBoard(KB_T)) { lightAngle += 0.05f; }

		m_dirLight.x = sinf(lightAngle);
		m_dirLight.z = cosf(lightAngle);
		m_dirLight.y = -0.99f;

		// �u���[�X�V
		DeferredManagerEx.RadialBlurUpdate();

		// �|�C���g���C�g�X�V
		PointLightMgr->Update();

		// 
		NumberEffect.Update();

		// �G���^�[�ŃG�t�F�N�g�J�����������Ă݂�
		if (KeyBoardTRG(KB_ENTER))
		{
			//m_pMyMusicMgr->PlayHeaveHo();
			//EFFECT_CAMERA_INFO eci;
			//eci.scriptID = 0;
			//MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
		}


		if (KeyBoardTRG(KB_N))
		{
			// UI�Ƀ��b�Z�[�W�𑗂�
			SIDE side = SIDE::LEFT;
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::APP_WIN_ICON, &side);
		}
		if (KeyBoardTRG(KB_M))
		{
			// UI�Ƀ��b�Z�[�W�𑗂�
			CUTIN_TYPE_NAME data = CUTIN_TYPE_NAME::AIROU;
			MsgMgr->Dispatch(0, ENTITY_ID::CUTIN_MGR, ENTITY_ID::CUTIN_MGR,
				MESSAGE_TYPE::CUTIN_ACTION, &data);

		}

		// 
		if (KeyBoardTRG(KB_H))
		{
			SIDE data = SIDE::LEFT;
			// 
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::OVER_DRIVE_CUTIN, &data);

		}
		if (KeyBoardTRG(KB_J))
		{

			SIDE data = SIDE::RIGHT;
			//
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::OVER_DRIVE_CUTIN, &data);
		}


		if (KeyBoardTRG(KB_K))
		{
			if (m_bShaderFlag)
			{
				m_bShaderFlag = false;
			}
			else
			{
				m_bShaderFlag = true;
			}
		}

		if (KeyBoardTRG(KB_U))
		{
			//TimeMgr->a
			GameUIMgr->Action();
		}
		if (KeyBoardTRG(KB_I))
		{
			// UI�Ƀ��b�Z�[�W�𑗂�
			SIDE side = SIDE::LEFT;
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::APP_WIN_ICON, &side);
		}

		if (KeyBoardTRG(KB_N))
		{
			OverDriveAction();
		}

		if (KeyBoardTRG(KB_B))
		{
			OverDriveEnd();
		}

		// UI
		GameUIMgr->Update();

		//g_eff->Update();
		//EffectMgr.Update();
		m_panel->Update();
		g_uvEffect->Update();

		//TimeMgr->Update();
		CutInMgr->Update();

		m_pRoundCallMgr->Update();// ���E���h�R�[��

		m_pOverDriveStage->Update();// �K�E�w�i

		OverDriveUpdate();// �I�[�o�[�h���C�u
	}

	// �E�B���h�E�� (�|�[�Y�E�B���h�E�Ȃ�)
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Update();
	}
	// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
	m_pStateMachine->Update();
}

//******************************************************************
//		�`��
//******************************************************************

void sceneMain::Render()
{
	tdnStopWatch::Start();
	// �J����
	CameraMgr->Activate();

	if (m_bShaderFlag)
	{

		// G_Buffer�N���A (�œK��)�@20
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();


		// �V�F�[�_�X�V
		DeferredManagerEx.G_Update(CameraMgr->m_ViewData.pos);

		
		// [1206](�œK��)�@500�O��

		// �e (�œK��) 200
		RenderShadow();



		// �V�F�[�_  (�œK��)�@85 �X�e�[�W�ɂ���ėl�X��������͎d�����Ȃ�
		DeferredManagerEx.G_Begin();
		{
			// �X�e�[�W�`��
			m_pStage->RenderDeferred();
			// �v���C���[
			PlayerMgr->RenderDeferred();
		}
		// �V�F�[�_�I���
		DeferredManagerEx.G_End();


		// ���s���Ɗ����@(�œK��) 50->38�R�X�g��
		//DeferredManagerEx.DirLight(m_pStage->GetShaderParam().vDirLightVec, m_pStage->GetShaderParam().vDirLightColor);
		//DeferredManagerEx.HemiLight(m_pStage->GetShaderParam().vSkyColor, m_pStage->GetShaderParam().vGroundColor);
		DeferredManagerEx.AllLight(m_pStage->GetShaderParam().vDirLightVec, m_pStage->GetShaderParam().vDirLightColor,
			m_pStage->GetShaderParam().vSkyColor, m_pStage->GetShaderParam().vGroundColor);

		// �|�C���g���C�g�`��
		DeferredManagerEx.GpuPointLightRender();


		// ���K�E�p�̃X�e�[�W���`�悳���Ȃ�`�悵�Ȃ�
		if (m_bOverDriveStageFlag == false)
		{		
			// (�œK��)�@120 ���������ȏ�͎d�����Ȃ�
			RenderStage();// �����ŃX�e�[�W�������܂Ƃ߂ĕ`��
		}else
		{
			m_pOverDriveStage->Render();// �K�E�w�i
		}


		// �Ō�̏���  �@[1206](�œK��)�@700�O��
		{



			DeferredManagerEx.FinalBegin();


			//int dim= PlayerMgr->GetOverDriveDim();
			//m_stageScreen->SetARGB(255, dim, dim, dim);
			
			// �@[1206](�œK��)�@40->10
			// �K�E�������Đ؂�ւ��鉉�o�܂ł͂ӂ��ɕ`��
			if (m_bOverDriveStageFlag == false)
			{

				m_stageScreen->Render(0, 0, RS::COPY_NOZ);// ��Z�l�l�������ĂȂ����R�́��̊G��`�悷�邽��
			
			}else
			{
				shader2D->SetValue("MaskTex", m_pMaskScreen);			// �΂߃u���C���h�̉摜
				shader2D->SetValue("g_fMaskEdgeRate", m_fMaskRate);		// �΂߃u���C���h�̉摜��M�郌�[�g
				m_pOverDriveStage->GetScreen()->Render(0, 0, RS::COPY_NOZ);// �R�X�g10		
				m_stageScreen->Render(0, 0, shader2D, "MaskEdge");// ��Z�l�l�������ĂȂ����R�́��̊G��`�悷�邽��
			}

			// �L�����N�^�[��艺�ɕ`�悷��UI
			GameUIMgr->RenderBack();

			// �X�g�b�v�E�H�b�`�J�n
	
			// �v���C���[�@[1206](�œK��)�@400~500->   ���Ƃ��Ă��œK��������	
			PlayerMgr->Render();
			
			// �V���b�g
			m_pShotMgr->Render();




			// �p�[�e�B�N�� (�œK��) 50~90->30
			ParticleManager::Render();

	
			
			// UV�E�p�l���@(�œK��) �R�X�g6�@�S�R���Ȃ� 
			//m_panel->Render();
			//m_panel->Render3D();
			//g_uvEffect->Render();
			DeferredManagerEx.FinalEnd();
		}



		// �u���[�� (�œK��) 80~90-> 40 �����R�X�g�_�E��
		DeferredManagerEx.BloomRender();
		
	

		

	}
	else
	{

		// �L�����N�^�[��艺�ɕ`�悷��UI
		GameUIMgr->RenderBack();

		// �X�e�[�W�`��
		m_pStage->Render(shader, "copy");
		// �v���C���[
		PlayerMgr->Render(shader, "copy");


	}

	//+---------------------------
	// ��O��UI �i��200->150�j
	//+---------------------------


	NumberEffect.Render();
	// PlayerMgr->RenderUI();		// (1206) �R���{�Ŏg���Ă������S�ăQ�[��UI�ŕ\���������̂ŃR�����g�A�E�g
	if (m_pStateMachine->isInState(*SceneMainState::HeaveHoDriveOverFlowSuccess::GetInstance()) == false)
	{
		GameUIMgr->Render();
	}

	HeaveHoFinishUI->Render();


	//+----------------------------------
	// �E�B���h�E�E�V�[���ړ��E�t�F�[�h��
	//+----------------------------------

	// �����ŃE�B���h�E����`�悷�邩�X�e�[�g�}�V���Ōʂŕ`�悷�邩�͌�Ŕ��f
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Redner();
	}

	// �������ɃX�e�[�g�}�V���`��(����2D�֌W�������񂶂�Ȃ�����)
	m_pStateMachine->Render();

	if (KeyBoard(KB_J))
	{
		SurfaceRender();
	}

	// ���E���h�R�[��
	m_pRoundCallMgr->Render();

	// �t�F�[�h�`��
	Fade::Render();

	// �V�[���X�C�b�`
	prevEF->Render();

	//DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->Render(0, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);

	CutInMgr->Render();

	//com->Render(400, 400);

#ifdef _DEBUG
	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", CameraMgr->m_ViewData.pos.x, CameraMgr->m_ViewData.pos.y, CameraMgr->m_ViewData.pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", CameraMgr->m_ViewData.target.x, CameraMgr->m_ViewData.target.y, CameraMgr->m_ViewData.target.z);
	tdnText::Draw(0, 90, 0xffffffff, "CameraEvent  : %d", CameraMgr->GetEventFrame());
#endif


	// �����s����
	//std::vector<int> v;
	//int N = 500 * 100;
	//for (int i = 0; i < N; i++)
	//{
	//	v.push_back(i);
	//}
	tdnStopWatch::End();
}

void sceneMain::RenderStage()
{
	Surface* save;
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &save);

	m_stageScreen->RenderTarget(0); //�؂�ւ�
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->RenderTarget(1); //�؂�ւ�

	// �X�e�[�W�p�̃p�����[�^
	shaderM->SetValue("g_OverDriveDim", PlayerMgr->GetOverDriveDim());

	// �X�e�[�W�`��
	m_pStage->Render(shaderM, "Stage");


	tdnSystem::GetDevice()->SetRenderTarget(0, save);//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// [1206] (�œK��) 200  (TODO)���Ă�����ŏ������H
void sceneMain::RenderShadow()
{
	if (DeferredManagerEx.GetShadowFlag() == false)return;

	// �t���O�������Ă鎞�̂ݕ`��  [1206] �ꉞ�p�ӂ��Ƃ���
	//if (m_bBakeStageShadow == true)
	{
		DeferredManagerEx.CreateShadowMatrix
			(m_pStage->GetShaderParam().vDirLightVec, Vector3(0, 0, 0), Vector3(0, 0, 1), 400);

		{
			DeferredManagerEx.ShadowBegin();


			m_pStage->RenderShadow();


			// �v���C���[
			PlayerMgr->RenderShadow();

			DeferredManagerEx.ShadowEnd();
		}

		m_bBakeStageShadow = false;		// ��x�����e���Ă�����

	}
}

void sceneMain::SurfaceRender()
{
	enum {
		X = 320 , Y = 180 
	};

	int texX = 0;
	int texY = 0;

	DeferredManagerEx.GetTex(SURFACE_NAME_EX::NORMAL_DEPTH)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::ROUGHNESS)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SPEC)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::FORWARD)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SCREEN)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX = 0;
	texY++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::POINT_LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	if (DeferredManagerEx.GetShadowFlag() == false)return;
	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SHADOW_MAP)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX = 0;
	//texY ++;
	//DeferredManager.GetTex(SURFACE_NAME::MRT_NORMAL)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::MRT_ROUGHNESS)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::SPECULAR)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);
}

void sceneMain::OverDriveUpdate()
{
	if (m_bOverDriveStageFlag == true)m_fMaskPower = -0.2f;
	else m_fMaskPower = 0.2f;

	// �}�X�N
	m_fMaskRate += m_fMaskPower;
	m_fMaskRate = Math::Clamp(m_fMaskRate, 0.0f, 1.0f);

}

void sceneMain::OverDriveAction()
{
	m_bOverDriveStageFlag = true;
}

void sceneMain::OverDriveEnd()
{
	m_bOverDriveStageFlag = false;
}

void sceneMain::Reset()
{
	m_bOverDriveStageFlag = false;
	PlayerMgr->Reset();
	CameraMgr->SetPlayersPos();
	m_pShotMgr->Reset();
	ParticleManager::Reset();
	GameUIMgr->TimerReset();
}