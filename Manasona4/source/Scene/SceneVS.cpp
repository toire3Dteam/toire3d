#include "TDNLIB.h"
#include "BattleLoading/BattleLoading.h"
#include "system/FrameworkEx.h"
#include "SceneVS.h"
#include "SceneMain.h"
#include "LoadSceneThread.h"
#include "../Fade/Fade.h"

//=============================================================================================
//		初	期	化	と	開	放
bool sceneVS::Initialize()
{
	// バトル演出初期化
	BattleLoadInst->Initialize();

	// ロードシーンスレッド発動
	LoadSceneThreadMgr->Initialize(new sceneMain);

	m_bEndVS = false;

	return true;
}

sceneVS::~sceneVS()
{
	// スレッドの開放 シングルトンだからシーン変わってもデリートとか大丈夫のはず
	LoadSceneThreadMgr->Release();
}
//=============================================================================================


//=============================================================================================
//		更			新
void sceneVS::Update()
{
	// フェード
	Fade::Update();

	// VS終わった後のロード中
	if (m_bEndVS)
	{
		// シーンを読み込み終わっていたら
		if (LoadSceneThreadMgr->isInitializeEnd())
		{
			// すでに読み込んだシーンに行く
			MainFrameEX->ChangeScene(LoadSceneThreadMgr->GetNewScene());
		}
		return;
	}

	// VS演出の更新
	BattleLoadInst->Update();
	
	// VS演出終わった瞬間
	if (BattleLoadInst->isEnd())
	{
		// シーンを読み込み終わっていたら
		if (LoadSceneThreadMgr->isInitializeEnd())
		{
			// すでに読み込んだシーンに行く
			MainFrameEX->ChangeScene(LoadSceneThreadMgr->GetNewScene());
		}
		// 終了フラグオン
		m_bEndVS = true;
	}
}
//
//=============================================================================================


//=============================================================================================
//		描			画
void sceneVS::Render()
{
	tdnView::Clear();

	BattleLoadInst->Render();
	Fade::Render();
}