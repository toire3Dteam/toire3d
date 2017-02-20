#include "TDNLIB.h"
#include "FrameworkEx.h"
#include "../Scene/sceneLoading.h"
#include "../Sound/SoundManager.h"

//*****************************************************************************************************************************
//
//		�t���[�����[�N
//
//*****************************************************************************************************************************

// ���̉�
Framework* Framework::m_pInstance = nullptr;

//------------------------------------------------------
//		�������E���
//------------------------------------------------------
Framework::Framework()
{
	this->FPSMode = FPS_30_IEX;

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


Framework * Framework::GetInstance()
{
	if (Framework::m_pInstance == nullptr)
	{
		m_pInstance = new Framework();
	}

	return m_pInstance;
}

void Framework::Release()
{
	SAFE_DELETE(m_pInstance);
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
	case FPS_60_IEX:	bRender = TRUE;	break;
	case FPS_30_IEX:	if (dwGameFrame % 2 == 0) bRender = TRUE; else bRender = FALSE;
		break;
	case FPS_FLEX:	if (dTime > 167 * 2) bRender = FALSE; else bRender = TRUE;
		break;
	}

	//	�t���[�����ԍX�V
	if (GetKeyState(VK_LCONTROL) < 0) dwFrameTime += 600;
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


	// �V�X�e���X�V
	tdnInput::Update();
	SoundManager::Update();

	/*������Đ����Ă�����`�悳���Ȃ�*/
	if (scene != NULL){
		//movie_play = scene->Update();
		//return movie_play;
		
		// �V�[���̍X�V
		scene->Update();

		// ���悪�Đ����Ă�����`�悳���Ȃ�
		return true;
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
	tdnText::Draw(tdnSystem::GetScreenSize().right - 128, tdnSystem::GetScreenSize().bottom - 36, 0xff00ffff, "FPS %d/%d", dwFPS, dwRenderFPS);

	// �X�g�b�v�E�H�b�`�̕`��
	tdnStopWatch::DrawResult(200, 0);
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
		scene = new sceneLoading(newScene);
		scene->Initialize();	// �����ŁA�J�ڐ��Initialize���Ă�ł���B
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