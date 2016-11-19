#include "TDNLIB.h"
#include "sceneSelect.h"
#include "sceneSelectState.h"
#include "SceneMain.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"

//#include "sceneResult.h"
//#include "SceneSwitch/SceneSwitch.h"
//SceneSwitchPrev* prevEF;
//+------------------------
//	シーンメニュー
//+------------------------
//int sceneSelect::m_iSelectNo(0);	// セレクトナンバー(staticにすることによって、前回選択したモードに合わせたままメニューシーンが始まる)

//******************************************************************
//		初期化・解放
//******************************************************************
sceneSelect::sceneSelect() :BaseGameEntity(ENTITY_ID::SCENE_SELECT) {}
bool sceneSelect::Initialize()
{
	// セレクトナンバー
	//m_iSelectNo = 0;

	// Selectマネージャー
	m_pSelectUIMgr = new SelectUIManager();
	//m_pMenuUIMgr->ChangeSelectNo(m_iSelectNo);
	//m_pMenuUIMgr->Action();

	//m_pCtrlSelectUI = new ControllerSelectUI();

	
	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneSelect>(this);
	m_pStateMachine->SetCurrentState(SceneSelectState::Intro::GetInstance());	// ステートの設定

	// BGMをストリーミング再生
	bgm->PlayStreamIn("DATA/Sound/BGM/System/CharaSelect.ogg");

	return true;
}

sceneSelect::~sceneSelect()
{


	bgm->StopStreamIn();
	SAFE_DELETE(m_pSelectUIMgr);
	//SAFE_DELETE(m_pCtrlSelectUI);
	delete m_pStateMachine;


}

//******************************************************************
//		処理
//******************************************************************

void sceneSelect::Update()
{

	// セレクトマネージャー
	m_pSelectUIMgr->Update();

	// フェード
	Fade::Update();

	{
		// ★ステートマシン更新(何故ここに書くかというと、中でシーンチェンジの処理を行っているため)
		m_pStateMachine->Update();
		return;
	}
}

//******************************************************************
//		描画
//******************************************************************

void sceneSelect::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// セレクトマネージャー
	m_pSelectUIMgr->Render();

	// ★ここにステートマシン描画(多分2D関係が多いんじゃないかと)
	m_pStateMachine->Render();


	// フェード
	Fade::Render();
	
}

