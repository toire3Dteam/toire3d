#include	"TDNLIB.h"
#include	"system/FrameworkEx.h"
#include	"LoadSceneThread.h"

//=============================================================================================
//		�R���X�g���N�^
LoadSceneThreadManager::LoadSceneThread::LoadSceneThread(BaseScene *pNewScene) :

//���̃V�[���̃|�C���^�͌�Ŏg���̂�newScene�ɕۑ����Ă����B
m_pNewScene(pNewScene),

// ���R���X�g���N�^�ŃX���b�h�𐶐� �������o�֐����w�肷��ꍇ�́A��������this�����邱�ƁB
m_pThread(new std::thread(&LoadSceneThreadManager::LoadSceneThread::ThreadFunction, this, pNewScene)),

// �X���b�h�t���O
m_bThread(true)
{

}

//=============================================================================================

//=============================================================================================
//		�f�X�g���N�^
LoadSceneThreadManager::LoadSceneThread::~LoadSceneThread()
{
	// ���[�h���ɏ����Ă��܂�����A�S���ǂݍ��ނ܂őҋ@(������̖h�~)
	if (m_bThread)
	{
		//MessageBoxA(0, "�X���b�h�ǂݍ��ݒ��Ƀv���O��������~���܂����B�X���b�h���I���܂őҋ@���܂��B", "�Ӑ}���ʏI��", MB_OK);
		m_pThread->join();
		//MessageBoxA(0, "�ҋ@�I���B�v���O�����͐���ɉ������܂��B����", "�A�C���[�͐_", MB_OK);
	}
	else m_pThread->join();	// �ǂݍ��݃X���b�h���̂͊������Ă���̂ŁAdetach(���ʂɃX���b�h���Ƃ�)�ł��������ǁA�O�̂��߂�join(�X���b�h�����S�ɏI������܂őҋ@���Ă��痎�Ƃ�)�ɂ���
}
//=============================================================================================


//=============================================================================================
//		�X���b�h����
void LoadSceneThreadManager::LoadSceneThread::ThreadFunction(void *arg)
{
	BaseScene* scene = (BaseScene*)arg;

	// �������Ŗ{�̂�scene���K�v�Ȏ��Ԃ������鏉�������s��
	scene->Initialize();

	// ���̎��_�Ń��[�h�t���O��ON�B�V�[����؂�ւ���ۂ̓ǂݍ��݂����Ȃ��悤�ɂ���
	scene->m_bLoad = true;

	//	�X���b�h�I������
	m_bThread = false;
	//_endthread();
}


//
//=============================================================================================