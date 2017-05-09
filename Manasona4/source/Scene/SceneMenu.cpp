#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneMain.h"
#include "SceneVS.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Window/OptionWindow.h"	// 必要なウィンドウ
#include "Window/SoundWindow.h"	// 
#include "Window/GameWindow.h"
#include "Window/SystemWindow.h"
#include "Window/ChallengeWindow.h"
#include "Window/ChallengeSelectWindow.h"
#include "../Scene/sceneResult.h"
#include	"Trophy\TrophyManager.h"
#include "BaseEntity\Message\MessageDispatcher.h"
#include "Data\PlayerData.h"
#include "Data\CommandData.h"
#include "KeyConfig\KeyConfig.h"

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
			m_pTips[i] = new TipsCard("チュートリアルを始めますか？", true, true);
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
		case WINDOW_TYPE::SYSTEM:
			m_pWindow[i] = new SystemWindow(Vector2(200, 250));
			break;
		case WINDOW_TYPE::GAME:
			m_pWindow[i] = new GameWindow(Vector2(200, 250));
			break;
		case WINDOW_TYPE::SOUND:
			m_pWindow[i] = new SoundWindow(Vector2(200, 250));
			break;
		case WINDOW_TYPE::CHALLENGE:
			m_pWindow[i] = new ChallengeWindow(Vector2(450, 50));
			break;
		case WINDOW_TYPE::CHALLENGE_SELECT:
			m_pWindow[i] = new ChallengeSelectWindow(Vector2(150, 250));
			break;
		default:
			MyAssert(0, "そんなウィンドウはない");
			break;
		}
	}
	// キーコンフィグ
	for (int i = 0; i < (int)SIDE::ARRAY_MAX; i++)
	{
		m_pKeyWindow[i] = new KeyConfig();
	}


	// 最初の一回目のトロフィの解放確認
	TrophyMgr->CheakPlayCount(PlayerDataMgr->m_PlayerInfo.PlayCount);

	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneMenu>(this);
	m_pStateMachine->SetCurrentState(SceneMenuState::Intro::GetInstance());	// ステートの設定

	// BGMをストリーミング再生
	bgm->PlayStreamIn("DATA/Sound/BGM/System/ModeSelect.ogg");

	// ポーズ押した人のデバイスID
	m_iPauseDeviceID = 0;
	m_eCommandSide = SIDE::LEFT;// 今は使っていない

	// (TODO)仮
	CommandMgr->SetReplayFlag(false);

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
	
	// キーコンフィグ
	for (int i = 0; i < (int)SIDE::ARRAY_MAX; i++)
	{
		SAFE_DELETE(m_pKeyWindow[i]);
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


		if (KeyBoardTRG(KB_SPACE))
		{
			CommandMgr->SetReplayFlag(true);

			// バイナリ
			FILE *fp;
			MyAssert(fopen_s(&fp, "DATA/Replay/SelectData.bin", "rb") == 0, "デデドン(絶望)\nセーブデータ読み込みに失敗した！");	// まず止まることはないと思うが…

																											// ラウンド数
			int l_iRoundNum;
			fread_s((LPSTR)&l_iRoundNum, sizeof(int), sizeof(int), 1, fp);
			PlayerDataMgr->m_ConfigData.iRoundNumType = l_iRoundNum;

			// 制限時間
			int l_iRoundTime;
			fread_s((LPSTR)&l_iRoundTime, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iRoundTime = l_iRoundTime;

			// ステージ
			int l_iStage;
			fread_s((LPSTR)&l_iStage, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->eStage = (STAGE)l_iStage;

			// 曲の番号
			int l_iBattleMusicID;
			fread_s((LPSTR)&l_iBattleMusicID, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iBattleMusicID = l_iBattleMusicID;

			FOR((int)SIDE::ARRAY_MAX)
			{
				// 使用きゃら
				int l_iCharacter;
				fread_s((LPSTR)&l_iCharacter, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].eCharacter = (CHARACTER)l_iCharacter;
				// 使用パートナー
				int l_iPartner;
				fread_s((LPSTR)&l_iPartner, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].ePartner = (PARTNER)l_iPartner;
			}

			// ファイル閉じる
			fclose(fp);

			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

			// チュートリアルではない
			SelectDataMgr->Get()->bTutorial = false;

			// トレーニングでもない
			SelectDataMgr->Get()->bTraining = false;

			// チャレンジでもない
			SelectDataMgr->Get()->bChallenge = false;
			MainFrameEX->ChangeScene(new sceneVS);
			return;
		}

#ifdef DEBUG


		// エンターでワープ
		if (KeyBoard(KB_ENTER) == 2)
		{
			ResultData data;
			data.eWinnerSide = SIDE::LEFT;
			data.iMaxDamage = 2500;
			data.iRemainingHP = 80;
			data.iElapsedTime = 35;

			MainFrameEX->ChangeScene(new sceneResult(data));
			return;
		}

		if (KeyBoardTRG(KB_SPACE))
		{
			CommandMgr->SetReplayFlag(true);

			// バイナリ
			FILE *fp;
			MyAssert(fopen_s(&fp, "DATA/Replay/SelectData.bin", "rb") == 0, "デデドン(絶望)\nセーブデータ読み込みに失敗した！");	// まず止まることはないと思うが…

			// ラウンド数
			int l_iRoundNum;
			fread_s((LPSTR)&l_iRoundNum, sizeof(int), sizeof(int), 1, fp);
			PlayerDataMgr->m_ConfigData.iRoundNumType = l_iRoundNum;

			// 制限時間
			int l_iRoundTime;
			fread_s((LPSTR)&l_iRoundTime, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iRoundTime = l_iRoundTime;

			// ステージ
			int l_iStage;
			fread_s((LPSTR)&l_iStage, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->eStage = (STAGE)l_iStage;

			// 曲の番号
			int l_iBattleMusicID;
			fread_s((LPSTR)&l_iBattleMusicID, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iBattleMusicID = l_iBattleMusicID;

			FOR((int)SIDE::ARRAY_MAX)
			{
				// 使用きゃら
				int l_iCharacter;
				fread_s((LPSTR)&l_iCharacter, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].eCharacter = (CHARACTER)l_iCharacter;
				// 使用パートナー
				int l_iPartner;
				fread_s((LPSTR)&l_iPartner, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].ePartner = (PARTNER)l_iPartner;
			}

			// ファイル閉じる
			fclose(fp);

			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

			// チュートリアルではない
			SelectDataMgr->Get()->bTutorial = false;

			// トレーニングでもない
			SelectDataMgr->Get()->bTraining = false;

			// チャレンジでもない
			SelectDataMgr->Get()->bChallenge = false;
			MainFrameEX->ChangeScene(new sceneMain);
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
	for (int i = 0; i < (int)SIDE::ARRAY_MAX; i++)
	{
		// キーウィンドウ
		m_pKeyWindow[i]->Render();
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

