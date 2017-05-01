#include "TDNLIB.h"
#include "FrameworkEx.h"
#include "../Scene/sceneMain.h"
#include "../Scene/sceneRenderingTest.h"
#include "../Scene/sceneRenderingTest2.h"
#include "../Scene/sceneResult.h"
#include "../Scene/sceneTitle.h"
#include "../Scene/SceneMenu.h"
#include "../Scene/SceneCollect.h"
#include "../Scene/SceneSelect.h"
#include "../Scene/SceneVS.h"
#include "System.h"
#include "../Sound/SoundManager.h"
#include "../Data/PlayerData.h"
#include "../Effect/Particle.h"
#include "../MenuUI/TutorialManager.h"
#include "Trophy/TrophyManager.h"
#include "../Challenge/ChallengeManagerManager.h"
#include "BattleLoading/BattleLoading.h"

//*****************************/
//		WinMain
//*****************************/

//Framework*	MainFrame = NULL;			// �t���[�����[�N

char*	AppTitle = "�^�Ẵt�@�C�g4";// �Q�[���̃^�C�g�� 
BOOL	bFullScreen = FALSE;			// �t���X�N���[���ɂ��܂����H
DWORD	ScreenMode = SCREEN1280x720;	// Window�̉�ʂ̃T�C�Y

/***************************/
//	�A�v���P�[�V����������
/***************************/
BOOL InitApp(HINSTANCE hInstance, int nCmdShow)									// �Q�[���N�����̍ŏ��̏������@�K�v�ȏ�������S�Ă�����
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) bFullScreen = TRUE;// Ctrl�Ńt���X�N���[��

	if (!tdnSystem::InitWindow(hInstance, nCmdShow, AppTitle, ScreenMode, bFullScreen))
	{
		MessageBox(0, "�E�B���h�E���������s�B", "System", MB_OK);
	}

	// �Z�[�u�f�[�^�ǂݍ���
	PlayerDataMgr->Initialize();

	tdnRandom::Initialize();
	tdnRenderState::Initialize();
	tdnTexture::Initialize();
	tdnText::Init();
	tdnInput::Initialize();
	SoundManager::Initialize();
	SoundManager::SetBaseVolume(PlayerDataMgr->m_ConfigData.iBGMVolume, PlayerDataMgr->m_ConfigData.iSEVolume, PlayerDataMgr->m_ConfigData.iVoiceVolume);
	tdnMovieManager::Initialize();
	ParticleManager::Initialize("DATA/Effect/particle.png", 1024);	// �p�[�e�B�N��������(���Ȃ������ɏ������Ƃ����ƁA�V�[�����X�V�[���ɂāA��������������������ɂȂ��Ă��܂�����)
	TrophyMgr;
	BattleLoadInst;
	//IEX�̃V�X�e���֐����ꎞ�I�̗p�A�b��������System.h�𖕏����邩���߂�
	SYSTEM_Initialize();

	//MainFrameEX = new Framework();			 // ���C���t���[�����[�N����
	MainFrameEX->ChangeScene(new sceneMain); // �����V�[��
	//MainFrameEX->ChangeScene(new sceneRenderingTest); // �`��e�X�g�V�[��
	//MainFrameEX->ChangeScene(new sceneRenderingTest2); // �`��e�X�g�V�[��
	//MainFrameEX->ChangeScene(new sceneResult(SIDE::RIGHT)); // ���U���g�V�[��
	//MainFrameEX->ChangeScene(new sceneVS); // 
	//MainFrameEX->ChangeScene(new sceneTitle);
	//MainFrameEX->ChangeScene(new sceneMenu);
	//MainFrameEX->ChangeScene(new sceneCollect);
	//MainFrameEX->ChangeScene(new sceneSelect);
	


	return TRUE;
}

/****************************/
//	�A�v���P�[�V�������
/****************************/
BOOL EndApp()																	// ���̒��ŃV�X�e�����܂߂��S�Ă̌㏈��������̂ŃN���X�̒��ɂ͓����Ȃ�
{
	//delete MainFrame;			// [����]System��Release�����O�ɏ�����
	MainFrameEX->Release();
	BattleLoadInst->Rerease();
	ParticleManager::Release();
	SYSTEM_Release();
	TrophyMgr->Rerease();
	TutorialMgr->Rerease();
	ChallengeMgrMgr->Rerease();
	// ���C�u�����[�֌W�͍Ō�ɏ���
	tdnFont::Release();
	tdnSystem::Release();
	tdnText::CleanUpModule();
	tdnInput::Release();
	SoundManager::Release();
	tdnMovieManager::Release();

	return TRUE;
}


//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	// ���������[�N���m��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// �������m�ۂ����u�Ԍ��m��
	//_CrtSetBreakAlloc(1601);

	// �ŏ��̏�����
	InitApp(hInstance, nCmdShow);

	MSG		msg;	// �E�B���h�E���b�Z�[�W���擾
	// ���C�����[�v
	for (;;)
	{
		// ���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	// ���b�Z�[�W�������TRUE
		{
			if (!GetMessage(&msg, NULL, 0, 0)) break;	// WM_QUIT �ȊO�̃��b�Z�[�W���擾�����ꍇ�A0 �ȊO�̒l���Ԃ�܂��B
			TranslateMessage(&msg);						// ���b�Z�[�W���͂��v���O�����ŏ����ł���`���ɕϊ�
			DispatchMessage(&msg);						// WindowProc���Ăт����A���b�Z�[�W�������s���B

			//// �E�B���h�E���A�N�e�B�u��Ԃ���Ȃ�������~�߂�
			//if (tdnSystem::GetWindowActive() == false)
			//{
			//	WaitMessage();
			//}
		}
		else
		{
			//static int TDNtimer = 0;

			// �Q�[���̍X�V�E�`��
			if (MainFrameEX->Update())
			{
				MainFrameEX->Render();
				//TDNtimer++;
				//MyAssert((TDNtimer != 60 * 60), "�V�т��������������������I�I�I�I�I");
			}			

		}


	}

	//	�S���
	EndApp();

	return true;
}
