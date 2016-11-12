#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
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
int sceneMenu::m_iSelectNo(0);	// セレクトナンバー(staticにすることによって、前回選択したモードに合わせたままメニューシーンが始まる)

//******************************************************************
//		初期化・解放
//******************************************************************
sceneMenu::sceneMenu() :BaseGameEntity(ENTITY_ID::SCENE_MENU) {}
bool sceneMenu::Initialize()
{
	//prevEF = new SceneSwitchPrev();

	// セレクトナンバー
	//m_iSelectNo = 0;

	// UIマネージャー
	m_pMenuUIMgr = new MenuUIManager();
	m_pMenuUIMgr->ChangeSelectNo(m_iSelectNo);
	m_pMenuUIMgr->Action();

	m_pCtrlSelectUI = new ControllerSelectUI();


	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneMenu>(this);
	m_pStateMachine->SetCurrentState(SceneMenuState::Intro::GetInstance());	// ステートの設定

	// BGMをストリーミング再生
	bgm->PlayStreamIn("DATA/Sound/BGM/System/CharaSelect.ogg");

	return true;
}

sceneMenu::~sceneMenu()
{
	bgm->StopStreamIn();
	delete m_pMenuUIMgr;
	SAFE_DELETE(m_pCtrlSelectUI);
	delete m_pStateMachine;
}

//******************************************************************
//		処理
//******************************************************************

void sceneMenu::Update()
{
	//if (KeyBoardTRG(KB_F))
	//{
	//	prevEF->Action();
	//}
	//prevEF->Update();
	//if (prevEF->IsEnd())
	//{
	//	MainFrameEx->ChangeScene(new sceneResult);
	//	return;
	//}

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

void sceneMenu::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	
	// ★ここにステートマシン描画(多分2D関係が多いんじゃないかと)
	m_pStateMachine->Render();

	// フェード
	Fade::Render();
	
	//prevEF->Render();
}

