#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneMain.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Window/OptionWindow.h"	// 必要なウィンドウ
#include "Window/SoundWindow.h"	// 
#include "Window/GameWindow.h"
#include "../Scene/sceneResult.h"
#include	"Trophy\TrophyManager.h"
#include "BaseEntity\Message\MessageDispatcher.h"
#include "Data\PlayerData.h"

//#include "sceneResult.h"
//#include "SceneSwitch/SceneSwitch.h"
//+------------------------
//	シーンメニュー
//+------------------------
int sceneMenu::m_iSelectNo(0);	// セレクトナンバー(staticにすることによって、前回選択したモードに合わせたままメニューシーンが始まる)
int sceneMenu::m_iCtrlDevice(0);// 操作していたデバイス
//int sceneMenu::m_iTrainingCtrlDevice(0);// トレーニングに入ったプレイヤーのID
//******************************************************************
//		初期化・解放
//******************************************************************
sceneMenu::sceneMenu() :BaseGameEntity(ENTITY_ID::SCENE_MENU) {}
bool sceneMenu::Initialize()
{	
	// セレクトナンバー
	//m_iSelectNo = 0;
	m_iTrainingCtrlDevice = 0;

	// UIマネージャー
	m_pMenuUIMgr = new MenuUIManager(m_iSelectNo);
	m_pMenuUIMgr->ChangeSelectNo(m_iSelectNo);
	m_pMenuUIMgr->Action();

	m_pCtrlSelectUI = new ControllerSelectUI();


	// ヒントカード覧
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		switch ((TIPS_TYPE)i)
		{
		case TIPS_TYPE::TUTORIAL:
			m_pTips[i] = new TipsCard("チュートリアルを始めますか？", true);
			break;
		default:
			MyAssert(0, "そんなヒントはない");
			break;
		}
	}
	
	// ウィンドウ覧
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		switch ((WINDOW_TYPE)i)
		{
		case WINDOW_TYPE::OPTION:
			m_pWindow[i] = new OptionWindow(Vector2(500, 50));
			break;
		case WINDOW_TYPE::GAME:
			m_pWindow[i] = new GameWindow(Vector2(200, 250));
			break;
		case WINDOW_TYPE::SOUND:
			m_pWindow[i] = new SoundWindow(Vector2(200, 250));
			break;
		default:
			MyAssert(0, "そんなウィンドウはない");
			break;
		}
	}


	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneMenu>(this);
	m_pStateMachine->SetCurrentState(SceneMenuState::Intro::GetInstance());	// ステートの設定

	// BGMをストリーミング再生
	bgm->PlayStreamIn("DATA/Sound/BGM/System/ModeSelect.ogg");

	return true;
}

sceneMenu::~sceneMenu()
{
	// トロフィーの演出を止める
	TrophyMgr->Stop();

	bgm->StopStreamIn();
	delete m_pMenuUIMgr;
	SAFE_DELETE(m_pCtrlSelectUI);
	delete m_pStateMachine;
	// ヒントカード覧
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTips[i]);
	}
	// ウィンドウ覧
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pWindow[i]);
	}
}

//******************************************************************
//		処理
//******************************************************************

void sceneMenu::Update()
{

	// ヒントカード覧
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		m_pTips[i]->Update(m_iCtrlDevice);
	}

	// ウィンドウ覧
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Update();
	}

	// フェード
	Fade::Update();

	// トロフィー
	TrophyMgr->Update();

	{
		// ★ステートマシン更新(何故ここに書くかというと、中でシーンチェンジの処理を行っているため)
		m_pStateMachine->Update();

#ifdef DEBUG
		// Tでトロフィー解禁
		if (KeyBoard(KB_T) == 2)
		{
			TROPHY_TYPE eType;
			eType = TROPHY_TYPE::FIRST;
			MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MENU, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);
			
		}
		if (KeyBoard(KB_U) == 2)
		{
			TROPHY_TYPE eType;
			eType = TROPHY_TYPE::FIRST_BATTLE;
			MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MENU, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}
		// Yでトロフィーロック
		if (KeyBoard(KB_Y) == 2)
		{
			TrophyMgr->AllReset();
		}


		// エンターでワープ
		if (KeyBoard(KB_ENTER) == 2)
		{
			ResultData data;
			data.eWinnerSide = SIDE::RIGHT;
			data.iMaxDamage = 2500;
			data.iRemainingHP = 80;
			data.iElapsedTime = 35;

			MainFrameEx->ChangeScene(new sceneResult(data));
			return;
		}

#endif // DEBUG

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

	// ウィンドウ覧
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Redner();
	}

	// ヒントカード覧
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		m_pTips[i]->Render();
	}
	
	// トロフィー
	TrophyMgr->Render();

	// 処理不可実験
	//std::vector<int> v;
	//int N = 1000 * 100;
	//for (int i = 0; i < N; i++)
	//{
	//	v.push_back(i);
	//}


	// フェード
	Fade::Render();
	
}

