#include "TDNLIB.h"
#include "Framework.h"
#include "../Scene/sceneLoading.h"
#include "../Sound/SoundManager.h"

//*****************************************************************************************************************************
//
//		�t���[�����[�N
//
//*****************************************************************************************************************************

//------------------------------------------------------
//		�������E���
//------------------------------------------------------
Framework::Framework(int FPSMode)
{
	this->FPSMode = FPSMode;

	scene = NULL;

	dwGameFrame = 0;
	dwFrameTime = clock();

	dwFPS = dwRenderFPS = 0;
	dwCurFrame = dwRCurFrame = 0;
}

Framework::~Framework()
{
	if (scene != NULL) delete scene;
}


//*****************************************************************************************************************************
//		�X�V
//*****************************************************************************************************************************
bool Framework::Update()
{
	static 	DWORD	dwSec = 0;
	DWORD CurrentTime = clock() * 10;

	//	�t���[������
	if (CurrentTime < dwFrameTime + 167) return false;

	//	�o�ߎ���
	DWORD	dTime = CurrentTime - dwFrameTime;
	if (dTime > 2000) dwFrameTime = CurrentTime;

	//	�X�L�b�v�^�C�v 
	switch (FPSMode){
	case FPS_60:	bRender = TRUE;	break;
	case FPS_30:	if (dwGameFrame & 0x01) bRender = TRUE; else bRender = FALSE;
		break;
	case FPS_FLEX:	if (dTime > 167 * 2) bRender = FALSE; else bRender = TRUE;
		break;
	}

	//	�t���[�����ԍX�V
	if (GetKeyState(VK_LCONTROL) < 0) dwFrameTime += 300;
	dwFrameTime += 167;

	//	�b�ԃt���[�����ۑ�
	if (dwSec < CurrentTime){
		dwFPS = dwCurFrame;
		dwRenderFPS = dwRCurFrame;
		dwCurFrame = dwRCurFrame = 0;
		dwSec += 10000;
	}
	dwCurFrame++;	//	�t���[�����X�V
	dwGameFrame++;	//	�Q�[���t���[�����X�V

	tdnInput::Update();

	// �T�E���h�X�V
	SoundManager::Update();

	/*������Đ����Ă�����`�悳���Ȃ�*/
	if (scene != NULL){
		//movie_play = scene->Update();
		//return movie_play;
	}

	return true;
}

//*****************************************************************************************************************************
//		�`��
//*****************************************************************************************************************************
void Framework::Render()
{
	if (!bRender) return;
	//	�V�[���J�n
	tdnSystem::BeginScene();
	//	�V�[���`��
	if (scene != NULL) scene->Render();


#ifdef _DEBUG
	//	�t���[���\��
	tdnText::Draw(0, 0, 0xff00ffff, "FPS %d/%d", dwFPS, dwRenderFPS);
#endif


	// �V�[���I��
	tdnSystem::EndScene();

	dwRCurFrame++;	//	�`��t���[�����X�V
}


//*****************************************************************************************************************************
//		�V�[���̐؂�ւ�
//*****************************************************************************************************************************
void Framework::ChangeScene(BaseScene* newScene, bool bLoadingScene)
{
	//	���݂̃V�[�����
	if (scene != nullptr) delete scene;
	//	�V�[���̐؂�ւ���������

	// ���[�f�B���O��ʂ���
	if (bLoadingScene)
	{
		scene = new sceneLoading;
		scene->Initialize(newScene);	// �����ŁA�J�ڐ��Initialize���Ă�ł���B
	}

	// ���[�f�B���O��ʂȂ�
	else
	{
		scene = newScene;

		// Loading�ł�2�dInitialize�j�~
		if (scene->m_bLoad) return;

		scene->Initialize();
	}
}