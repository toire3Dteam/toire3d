#include	"TDNLIB.h"
#include	"FrameworkEx.h"
#include	"../Scene/sceneMain.h"
#include	"../Scene/sceneRenderingTest.h"
#include	"../Scene/sceneResult.h"
#include	"../Scene/sceneTitle.h"
#include	"../Scene/SceneMenu.h"
#include	"../Scene/SceneCollect.h"
#include	"../Scene/SceneSelect.h"
#include	"System.h"
#include	"../Sound/SoundManager.h"
#include	"../Data/PlayerData.h"
#include	"../Effect/Particle.h"
#include    "../MenuUI/TutorialManager.h"

//*****************************/
//		WinMain
//*****************************/

//Framework*	MainFrame = NULL;			// �t���[�����[�N

char*	AppTitle = "�^��(��)";// �Q�[���̃^�C�g�� 
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

	tdnRandom::Initialize();
	tdnRenderState::Initialize();
	tdnTexture::Initialize();
	tdnText::Init();
	tdnInput::Initialize();
	SoundManager::Initialize();
	tdnMovieManager::Initialize();
	ParticleManager::Initialize("DATA/Effect/particle.png", 2048);	// �p�[�e�B�N��������(���Ȃ������ɏ������Ƃ����ƁA�V�[�����X�V�[���ɂāA��������������������ɂȂ��Ă��܂�����)

	//IEX�̃V�X�e���֐����ꎞ�I�̗p�A�b��������System.h�𖕏����邩���߂�
	SYSTEM_Initialize();

	//MainFrameEx = new Framework();			 // ���C���t���[�����[�N����
	//MainFrameEx->ChangeScene(new sceneMain, true); // �����V�[��
	//MainFrameEx->ChangeScene(new sceneRenderingTest); // �`��e�X�g�V�[��
	//MainFrameEx->ChangeScene(new sceneResult); // ���U���g�V�[��
	//MainFrameEx->ChangeScene(new sceneEffectTool); // 
	//MainFrameEx->ChangeScene(new sceneTitle);
	MainFrameEx->ChangeScene(new sceneMenu);
	//MainFrameEx->ChangeScene(new sceneCollect);
	//MainFrameEx->ChangeScene(new sceneSelect);
	
	// �f�[�^
	PlayerDataMgr->Initialize();

	return TRUE;
}

/****************************/
//	�A�v���P�[�V�������
/****************************/
BOOL EndApp()																	// ���̒��ŃV�X�e�����܂߂��S�Ă̌㏈��������̂ŃN���X�̒��ɂ͓����Ȃ�
{
	//delete MainFrame;			// [����]System��Release�����O�ɏ�����
	MainFrameEx->Release();
	ParticleManager::Release();
	SYSTEM_Release();
	TutorialMgr->Rerease();
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
	//NOTACTIVE:
		// ���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	// ���b�Z�[�W�������TRUE
		{
			if (!GetMessage(&msg, NULL, 0, 0)) break;	// WM_QUIT �ȊO�̃��b�Z�[�W���擾�����ꍇ�A0 �ȊO�̒l���Ԃ�܂��B
			TranslateMessage(&msg);						// ���b�Z�[�W���͂��v���O�����ŏ����ł���`���ɕϊ�
			DispatchMessage(&msg);						// WindowProc���Ăт����A���b�Z�[�W�������s���B

			// �E�B���h�E���A�N�e�B�u��Ԃ���Ȃ�������~�߂�
			if (tdnSystem::GetWindowActive() == false)
			{
				WaitMessage();
				//goto NOTACTIVE;
			}
		}
		else
		{
			//static int TDNtimer = 0;

			// �Q�[���̍X�V�E�`��
			if (MainFrameEx->Update())
			{
				MainFrameEx->Render();
				//TDNtimer++;
				//MyAssert((TDNtimer != 60 * 60), "�V�т��������������������I�I�I�I�I");
			}			

		}


	}

	//	�S���
	EndApp();

	return true;
}
