#include	"TDNLIB.h"
#include	"system/FrameworkEx.h"
#include	"LoadSceneThread.h"

//=============================================================================================
//		グ	ロ	ー	バ	ル	領	域
bool LoadSceneThreadManager::LoadSceneThread::bThread(false);


//=============================================================================================
//		初	期	化	と	開	放
LoadSceneThreadManager::LoadSceneThread::LoadSceneThread(BaseScene *pNewScene) : m_pNewScene(pNewScene)
{
	// シーン読み込みスレッド作成
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
//		ス	レ	ッ	ド	処	理
void LoadSceneThreadManager::LoadSceneThread::ThreadFunc(void *arg)
{
	BaseScene* scene = (BaseScene*)arg;

	scene->Initialize();	// ★ここで本体のsceneが必要な時間をかかる初期化を行う
	scene->m_bLoad = true;	// この時点でロードフラグをON。シーンを切り替える際の読み込みをしないようにする

	//	スレッド終了処理
	bThread = false;
	_endthread();
}


//
//=============================================================================================