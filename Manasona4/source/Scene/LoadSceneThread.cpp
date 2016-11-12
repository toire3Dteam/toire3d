#include	"TDNLIB.h"
#include	"system/FrameworkEx.h"
#include	"LoadSceneThread.h"

//=============================================================================================
//		�O	��	�[	�o	��	��	��
bool LoadSceneThreadManager::LoadSceneThread::bThread(false);


//=============================================================================================
//		��	��	��	��	�J	��
LoadSceneThreadManager::LoadSceneThread::LoadSceneThread(BaseScene *pNewScene) : m_pNewScene(pNewScene)
{
	// �V�[���ǂݍ��݃X���b�h�쐬
	_beginthread(ThreadFunc, 0, (void*)m_pNewScene);
	bThread = true;
}

LoadSceneThreadManager::LoadSceneThread::~LoadSceneThread()
{
	if (bThread)
	{
		_endthread();
		bThread = false;
	}
}
//=============================================================================================


//=============================================================================================
//		�X	��	�b	�h	��	��
void LoadSceneThreadManager::LoadSceneThread::ThreadFunc(void *arg)
{
	BaseScene* scene = (BaseScene*)arg;

	scene->Initialize();	// �������Ŗ{�̂�scene���K�v�Ȏ��Ԃ������鏉�������s��
	scene->m_bLoad = true;	// ���̎��_�Ń��[�h�t���O��ON�B�V�[����؂�ւ���ۂ̓ǂݍ��݂����Ȃ��悤�ɂ���

	//	�X���b�h�I������
	bThread = false;
	_endthread();
}


//
//=============================================================================================