#include "TDNLIB.h"
#include "BattleLoading/BattleLoading.h"
#include "system/FrameworkEx.h"
#include "SceneVS.h"
#include "SceneMain.h"
#include "LoadSceneThread.h"
#include "../Fade/Fade.h"

//=============================================================================================
//		��	��	��	��	�J	��
bool sceneVS::Initialize()
{
	// �o�g�����o������
	BattleLoadInst->Initialize();

	// ���[�h�V�[���X���b�h����
	LoadSceneThreadMgr->Initialize(new sceneMain);

	m_bEndVS = false;

	return true;
}

sceneVS::~sceneVS()
{
	// �X���b�h�̊J�� �V���O���g��������V�[���ς���Ă��f���[�g�Ƃ����v�̂͂�
	LoadSceneThreadMgr->Release();
}
//=============================================================================================


//=============================================================================================
//		�X			�V
void sceneVS::Update()
{
	// �t�F�[�h
	//Fade::Update();

	// VS�I�������̃��[�h��
	if (m_bEndVS)
	{
		// �V�[����ǂݍ��ݏI����Ă�����
		if (LoadSceneThreadMgr->isInitializeEnd())
		{
			// ���łɓǂݍ��񂾃V�[���ɍs��
			MainFrameEX->ChangeScene(LoadSceneThreadMgr->GetNewScene());
		}

		BattleLoadInst->LoadingCircleUpdate();// �҂��Ă�ԃ��[�f�B���O�̃T�[�N������������
		return;
	}


	// VS���o�̍X�V
	BattleLoadInst->Update();
	
	// �V�[����ǂݍ��ݏI����Ă�����
	if (LoadSceneThreadMgr->isInitializeEnd())
	{
		BattleLoadInst->SetSkipFlag(true);
	}

	// VS���o�I������u��
	if (BattleLoadInst->isEnd())
	{
		// �I���t���O�I��
		m_bEndVS = true;
	}

}
//
//=============================================================================================


//=============================================================================================
//		�`			��
void sceneVS::Render()
{
	//tdnView::Clear();

	BattleLoadInst->Render();
	//Fade::Render();
}