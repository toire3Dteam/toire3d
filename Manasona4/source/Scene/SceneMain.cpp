#include "TDNLIB.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../Effect/Particle.h"
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"
#include "../Timer/Timer.h"
#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "SceneMainState.h"
#include "../Fade/Fade.h"
#include "../DeferredEx/DeferredEx.h"
#include "../SceneSwitch/SceneSwitch.h"
#include "../PointLight/PointLight.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "../Number/Number.h"
#include "../UI/GameUI.h"
#include	"../Cutin/CutIn.h"
#include "../Sound/BattleMusic.h"
#include "UI/HeaveHoFinish/HeaveHoFinish.h"
#include "../Shot/ShotManager.h"
#include "RoundCall\RoundCallManager.h"
#include "Stage\OverDriveStage\OverDriveStage.h"



//BaseEffect* g_eff;
//EffectManager;
PanelEffectManager* m_panel;
UVEffectManager* g_uvEffect;

int stopTimer = 0;

//******************************************************************
//		初期化・解放
//******************************************************************
sceneMain::sceneMain() :BaseGameEntity(ENTITY_ID::SCENE_MAIN){}
bool sceneMain::Initialize()
{
	// ラウンド数
	// (TODO)[11/4]ラウンド数が0の時はラウンドコールを抜く処理がしたい
	// トレーニングやチュートリアルのため
	if (SelectDataMgr->Get()->bTutorial == false &&
		SelectDataMgr->Get()->bTraining == false)
	{
		//m_iRoundNum = SelectDataMgr->Get()->iWinRound;
		switch ((ROUND_NUM_TYPE)PlayerDataMgr->m_ConfigData.iRoundNumType)
		{
		case ROUND_NUM_TYPE::ROUND_1:
			m_iRoundNum = 1;
			break;
		case ROUND_NUM_TYPE::ROUND_2:
			m_iRoundNum = 2;
			break;
		case ROUND_NUM_TYPE::ROUND_3:
			m_iRoundNum = 3;
			break;
		case ROUND_NUM_TYPE::ROUND_4:
			m_iRoundNum = 4;
			break;
		default:
			MyAssert(0, "そのラウンド数のタイプはない");
			break;
		}
	}
	else
	{
		m_iRoundNum = 0;// ラウンド数が0だと無限ループ(チュートリアル・トレーニング)
	}

	m_dirLight = Vector3(1, -1, 1);

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "パネルエフェクト初期化待ち");
#endif
	m_panel = new PanelEffectManager();


#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "UVエフェクト初期化");
#endif
	g_uvEffect = new UVEffectManager();


#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "ステージ初期化");
#endif

	// ステージ初期化(★セレクトしたステージのタイプを引数に渡す)
	Stage::Base::CreateStage(&m_pStage, SelectDataMgr->Get()->eStage, CameraMgr);	// 関数の中で作ってもらう
	Collision::SetStage(m_pStage);

	//std::unique_ptr<std::thread> PlayerLoadThread(new std::thread([&](){
#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "プレイヤー初期化");
#endif
	PlayerMgr->Initialize(2, m_pStage, SelectDataMgr->Get()->tagSideDatas);

	// プレイヤーの座標のアドレスをカメラに渡してあげる(いじることは絶対に無く、ただ参照するだけ)
	CameraMgr->SetPlayersPos();

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "ゲームUI初期化");
#endif
	BasePlayer* p1 = PlayerMgr->GetPlayer_TeamInSearch(SIDE::LEFT);
	BasePlayer* p2 = PlayerMgr->GetPlayer_TeamInSearch(SIDE::RIGHT);
	GameUIMgr;
	
	if (SelectDataMgr->Get()->bTutorial == false &&
		SelectDataMgr->Get()->bTraining == false)
	{
		GameUIMgr->InitData(p1, p2, m_iRoundNum, (ROUND_TIME_TYPE)PlayerDataMgr->m_ConfigData.iRoundTimeType);
	}
	else
	{
		// 戦闘以外はどの設定にしても∞
		GameUIMgr->InitData(p1, p2, m_iRoundNum, (ROUND_TIME_TYPE::SEC_INFINITY));
	}

	//}));


#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "ディファード初期化");
#endif

	DeferredManagerEx;
	DeferredManagerEx.InitShadowMap(1024);
	m_bShaderFlag = true;

	m_fLoadPercentage = .6f;	// ロード割合

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "ポイントライト初期化");
#endif

	PointLightMgr;

	NumberEffect;

	m_stageScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

#ifdef _DEBUG
	sprintf_s(m_LoadComment, 256, "その他初期化");
#endif

	m_pShotMgr = new ShotManager;

	m_fLoadPercentage = .8f;	// ロード割合

	HeaveHoFinishUI;
	CutInMgr;

	m_fLoadPercentage = .85f;	// ロード割合

	m_fLoadPercentage = 1.0f;	// ロード割合

	// シーンスイッチ
	prevEF = new SceneSwitchPrev();

	// ラウンドコールマネージャー
	m_pRoundCallMgr = new RoundCallManager();

	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneMain>(this);

	//（TODO）チュートリアル・トレーニングだったら分岐
	m_bTutorialFlag = SelectDataMgr->Get()->bTutorial;
	m_eSelectTutorial = (TUTORIAL_TYPE)0;// (TODO)選んだチュートリアルを設定してあげる 					 

	m_bTrainingFlag = SelectDataMgr->Get()->bTraining;


	if (m_bTutorialFlag == true)
	{
		// UIを最初の一回だけ起動
		GameUIMgr->Action();
		// チュートリアル用の対戦
		m_pStateMachine->SetCurrentState(SceneMainState::TutorialIntro::GetInstance());

	}
	else if (m_bTrainingFlag == true)
	{
		// UIを最初の一回だけ起動
		GameUIMgr->Action();
		// トレーニング用の対戦
		m_pStateMachine->SetCurrentState(SceneMainState::Training::GetInstance());
	}
	else
	{
		// 通常の対戦
		m_pStateMachine->SetCurrentState(SceneMainState::StageIntro::GetInstance());	// ステートの設定
	}

	m_pMaskScreen = new tdn2DObj("Data/UI/Game/stageScreenMask.png");
	m_fMaskRate = 0.0f;
	m_fMaskPower = 0.0f;
	m_bOverDriveStageFlag = false;

	m_pOverDriveStage = new OverDriveStage();

	// ウィンドウ覧
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		switch ((BATTLE_WINDOW_TYPE)i)
		{
		case BATTLE_WINDOW_TYPE::PAUSE:
			m_pWindow[i] = new PauseWindow(Vector2(424, 128));
			break;
		case BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE:
			m_pWindow[i] = new TutorialPauseWindow(Vector2(424, 128));
			break;
		case BATTLE_WINDOW_TYPE::TRAINING_PAUSE:
			m_pWindow[i] = new TrainingPauseWindow(Vector2(424, 128));
			break;
		case BATTLE_WINDOW_TYPE::SOUND:
			m_pWindow[i] = new SoundWindow(Vector2(324, 228));
			break;
		case BATTLE_WINDOW_TYPE::TRAINING_OPTION:
			m_pWindow[i] = new TrainingOptionWindow(Vector2(224, 128));
			break;
		case BATTLE_WINDOW_TYPE::TRAINING_DUMMY:
			m_pWindow[i] = new TrainingDummyWindow(Vector2(224, 128));
			break;
		default:
			MyAssert(0, "そんなウィンドウはない");
			break;
		}
	}

	// ポーズしてるか
	m_bPause = false;

	// ステージ影焼き込み
	m_bBakeStageShadow = true;

	// プレイヤーの読み込みが終わるまで待機
	//PlayerLoadThread->join();

	// BGM流す
	bgm->PlayStreamIn((LPSTR)BattleMusicMgr->GetMusicFilePath(SelectDataMgr->Get()->iBattleMusicID).c_str());

	return true;
}

sceneMain::~sceneMain()
{
	delete prevEF;
	delete m_pStateMachine;
	//delete m_pMyMusicMgr;
	bgm->StopStreamIn();
	delete m_pStage;
	//delete CameraMgr;
	PlayerMgr->Release();
	//EffectMgr.Release();
	SAFE_DELETE(m_panel);
	SAFE_DELETE(g_uvEffect);
	//ParticleManager::Release();
	DeferredManagerEx.Release();
	SAFE_DELETE(m_stageScreen);
	HeaveHoFinishUI->Rerease();
	PointLightMgr->Release();
	delete m_pShotMgr;
	NumberEffect.Release();
	GameUIMgr->Rerease();
	//TimeMgr->Release();
	CutInMgr->Rerease();
	SAFE_DELETE(m_pRoundCallMgr);
	SAFE_DELETE(m_pMaskScreen);
	SAFE_DELETE(m_pOverDriveStage);
	//SAFE_DELETE(com);
	// ウィンドウ覧
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pWindow[i]);
	}
}

//******************************************************************
//		処理
//******************************************************************

void sceneMain::Update()
{
	HeaveHoFinishUI->Update();
	if (HeaveHoFinishUI->IsAction() == true)return ;
	
	// フェード更新
	Fade::Update();

	// ポーズ中は更新しない
	if (m_bPause == false)
	{

		//	com->Update();

		stopTimer++;
		//if (stopTimer > 60 * 60)
		//{
		//	return true;
		//}

		// カメラ更新(ステートマシンに書いた)
		//CameraMgr->Update();

		// ショット更新
		m_pShotMgr->Update();

		// ステージ更新
		m_pStage->Update();

		// プレイヤー更新
		//PlayerMgr->Update();

		// パーティクル更新
		ParticleManager::Update();

		static float lightAngle = 2.14f;
		// アングル
		if (KeyBoard(KB_R)) { lightAngle -= 0.05f; }
		if (KeyBoard(KB_T)) { lightAngle += 0.05f; }

		m_dirLight.x = sinf(lightAngle);
		m_dirLight.z = cosf(lightAngle);
		m_dirLight.y = -0.99f;

		// ブラー更新
		DeferredManagerEx.RadialBlurUpdate();

		// ポイントライト更新
		PointLightMgr->Update();

		// 
		NumberEffect.Update();

		// エンターでエフェクトカメラ発動してみる
		if (KeyBoardTRG(KB_ENTER))
		{
			//m_pMyMusicMgr->PlayHeaveHo();
			//EFFECT_CAMERA_INFO eci;
			//eci.scriptID = 0;
			//MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
		}


		if (KeyBoardTRG(KB_N))
		{
			// UIにメッセージを送る
			SIDE side = SIDE::LEFT;
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::APP_WIN_ICON, &side);
		}
		if (KeyBoardTRG(KB_M))
		{
			// UIにメッセージを送る
			CUTIN_TYPE_NAME data = CUTIN_TYPE_NAME::AIROU;
			MsgMgr->Dispatch(0, ENTITY_ID::CUTIN_MGR, ENTITY_ID::CUTIN_MGR,
				MESSAGE_TYPE::CUTIN_ACTION, &data);

		}

		// 
		if (KeyBoardTRG(KB_H))
		{
			SIDE data = SIDE::LEFT;
			// 
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::OVER_DRIVE_CUTIN, &data);

		}
		if (KeyBoardTRG(KB_J))
		{

			SIDE data = SIDE::RIGHT;
			//
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::OVER_DRIVE_CUTIN, &data);
		}


		if (KeyBoardTRG(KB_K))
		{
			if (m_bShaderFlag)
			{
				m_bShaderFlag = false;
			}
			else
			{
				m_bShaderFlag = true;
			}
		}

		if (KeyBoardTRG(KB_U))
		{
			//TimeMgr->a
			GameUIMgr->Action();
		}
		if (KeyBoardTRG(KB_I))
		{
			// UIにメッセージを送る
			SIDE side = SIDE::LEFT;
			MsgMgr->Dispatch(0, ENTITY_ID::UI_MGR, ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::APP_WIN_ICON, &side);
		}

		if (KeyBoardTRG(KB_N))
		{
			OverDriveAction();
		}

		if (KeyBoardTRG(KB_B))
		{
			OverDriveEnd();
		}

		// UI
		GameUIMgr->Update();

		//g_eff->Update();
		//EffectMgr.Update();
		m_panel->Update();
		g_uvEffect->Update();

		//TimeMgr->Update();
		CutInMgr->Update();

		m_pRoundCallMgr->Update();// ラウンドコール

		m_pOverDriveStage->Update();// 必殺背景

		OverDriveUpdate();// オーバードライブ
	}

	// ウィンドウ覧 (ポーズウィンドウなど)
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Update();
	}
	// ★ステートマシン更新(何故ここに書くかというと、中でシーンチェンジの処理を行っているため)
	m_pStateMachine->Update();
}

//******************************************************************
//		描画
//******************************************************************

void sceneMain::Render()
{
	tdnStopWatch::Start();
	// カメラ
	CameraMgr->Activate();

	if (m_bShaderFlag)
	{

		// G_Bufferクリア (最適化)　20
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();


		// シェーダ更新
		DeferredManagerEx.G_Update(CameraMgr->m_ViewData.pos);

		
		// [1206](最適化)　500前後

		// 影 (最適化) 200
		RenderShadow();



		// シェーダ  (最適化)　85 ステージによって様々だがこれは仕方がない
		DeferredManagerEx.G_Begin();
		{
			// ステージ描画
			m_pStage->RenderDeferred();
			// プレイヤー
			PlayerMgr->RenderDeferred();
		}
		// シェーダ終わり
		DeferredManagerEx.G_End();


		// 平行光と環境光　(最適化) 50->38コストへ
		//DeferredManagerEx.DirLight(m_pStage->GetShaderParam().vDirLightVec, m_pStage->GetShaderParam().vDirLightColor);
		//DeferredManagerEx.HemiLight(m_pStage->GetShaderParam().vSkyColor, m_pStage->GetShaderParam().vGroundColor);
		DeferredManagerEx.AllLight(m_pStage->GetShaderParam().vDirLightVec, m_pStage->GetShaderParam().vDirLightColor,
			m_pStage->GetShaderParam().vSkyColor, m_pStage->GetShaderParam().vGroundColor);

		// ポイントライト描画
		DeferredManagerEx.GpuPointLightRender();


		// ★必殺用のステージが描画されるなら描画しない
		if (m_bOverDriveStageFlag == false)
		{		
			// (最適化)　120 これもこれ以上は仕方がない
			RenderStage();// ここでステージだけをまとめて描画
		}else
		{
			m_pOverDriveStage->Render();// 必殺背景
		}


		// 最後の処理  　[1206](最適化)　700前後
		{



			DeferredManagerEx.FinalBegin();


			//int dim= PlayerMgr->GetOverDriveDim();
			//m_stageScreen->SetARGB(255, dim, dim, dim);
			
			// 　[1206](最適化)　40->10
			// 必殺発動して切り替える演出まではふつうに描画
			if (m_bOverDriveStageFlag == false)
			{

				m_stageScreen->Render(0, 0, RS::COPY_NOZ);// ※Z値考慮させてない理由は↓の絵を描画するため
			
			}else
			{
				shader2D->SetValue("MaskTex", m_pMaskScreen);			// 斜めブラインドの画像
				shader2D->SetValue("g_fMaskEdgeRate", m_fMaskRate);		// 斜めブラインドの画像を弄るレート
				m_pOverDriveStage->GetScreen()->Render(0, 0, RS::COPY_NOZ);// コスト10		
				m_stageScreen->Render(0, 0, shader2D, "MaskEdge");// ※Z値考慮させてない理由は↓の絵を描画するため
			}

			// キャラクターより下に描画するUI
			GameUIMgr->RenderBack();

			// ストップウォッチ開始
	
			// プレイヤー　[1206](最適化)　400~500->   何としても最適化したい	
			PlayerMgr->Render();
			
			// ショット
			m_pShotMgr->Render();




			// パーティクル (最適化) 50~90->30
			ParticleManager::Render();

	
			
			// UV・パネル　(最適化) コスト6　全然問題なし 
			//m_panel->Render();
			//m_panel->Render3D();
			//g_uvEffect->Render();
			DeferredManagerEx.FinalEnd();
		}



		// ブルーム (最適化) 80~90-> 40 半分コストダウン
		DeferredManagerEx.BloomRender();
		
	

		

	}
	else
	{

		// キャラクターより下に描画するUI
		GameUIMgr->RenderBack();

		// ステージ描画
		m_pStage->Render(shader, "copy");
		// プレイヤー
		PlayerMgr->Render(shader, "copy");


	}

	//+---------------------------
	// 手前のUI （約200->150）
	//+---------------------------


	NumberEffect.Render();
	// PlayerMgr->RenderUI();		// (1206) コンボで使っていたが全てゲームUIで表示させたのでコメントアウト
	if (m_pStateMachine->isInState(*SceneMainState::HeaveHoDriveOverFlowSuccess::GetInstance()) == false)
	{
		GameUIMgr->Render();
	}

	HeaveHoFinishUI->Render();


	//+----------------------------------
	// ウィンドウ・シーン移動・フェード類
	//+----------------------------------

	// ここでウィンドウ覧を描画するかステートマシンで個別で描画するかは後で判断
	for (int i = 0; i < (int)BATTLE_WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Redner();
	}

	// ★ここにステートマシン描画(多分2D関係が多いんじゃないかと)
	m_pStateMachine->Render();

	if (KeyBoard(KB_J))
	{
		SurfaceRender();
	}

	// ラウンドコール
	m_pRoundCallMgr->Render();

	// フェード描画
	Fade::Render();

	// シーンスイッチ
	prevEF->Render();

	//DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->Render(0, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);

	CutInMgr->Render();

	//com->Render(400, 400);

#ifdef _DEBUG
	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", CameraMgr->m_ViewData.pos.x, CameraMgr->m_ViewData.pos.y, CameraMgr->m_ViewData.pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", CameraMgr->m_ViewData.target.x, CameraMgr->m_ViewData.target.y, CameraMgr->m_ViewData.target.z);
	tdnText::Draw(0, 90, 0xffffffff, "CameraEvent  : %d", CameraMgr->GetEventFrame());
#endif


	// 処理不可実験
	//std::vector<int> v;
	//int N = 500 * 100;
	//for (int i = 0; i < N; i++)
	//{
	//	v.push_back(i);
	//}
	tdnStopWatch::End();
}

void sceneMain::RenderStage()
{
	Surface* save;
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &save);

	m_stageScreen->RenderTarget(0); //切り替え
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->RenderTarget(1); //切り替え

	// ステージ用のパラメータ
	shaderM->SetValue("g_OverDriveDim", PlayerMgr->GetOverDriveDim());

	// ステージ描画
	m_pStage->Render(shaderM, "Stage");


	tdnSystem::GetDevice()->SetRenderTarget(0, save);//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// [1206] (最適化) 200  (TODO)一回焼きこんで消すか？
void sceneMain::RenderShadow()
{
	if (DeferredManagerEx.GetShadowFlag() == false)return;

	// フラグが立ってる時のみ描画  [1206] 一応用意しといた
	//if (m_bBakeStageShadow == true)
	{
		DeferredManagerEx.CreateShadowMatrix
			(m_pStage->GetShaderParam().vDirLightVec, Vector3(0, 0, 0), Vector3(0, 0, 1), 400);

		{
			DeferredManagerEx.ShadowBegin();


			m_pStage->RenderShadow();


			// プレイヤー
			PlayerMgr->RenderShadow();

			DeferredManagerEx.ShadowEnd();
		}

		m_bBakeStageShadow = false;		// 一度だけ影を焼きこむ

	}
}

void sceneMain::SurfaceRender()
{
	enum {
		X = 320 , Y = 180 
	};

	int texX = 0;
	int texY = 0;

	DeferredManagerEx.GetTex(SURFACE_NAME_EX::NORMAL_DEPTH)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::ROUGHNESS)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SPEC)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::FORWARD)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SCREEN)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX = 0;
	texY++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::POINT_LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	if (DeferredManagerEx.GetShadowFlag() == false)return;
	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SHADOW_MAP)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX = 0;
	//texY ++;
	//DeferredManager.GetTex(SURFACE_NAME::MRT_NORMAL)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::MRT_ROUGHNESS)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::SPECULAR)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);
}

void sceneMain::OverDriveUpdate()
{
	if (m_bOverDriveStageFlag == true)m_fMaskPower = -0.2f;
	else m_fMaskPower = 0.2f;

	// マスク
	m_fMaskRate += m_fMaskPower;
	m_fMaskRate = Math::Clamp(m_fMaskRate, 0.0f, 1.0f);

}

void sceneMain::OverDriveAction()
{
	m_bOverDriveStageFlag = true;
}

void sceneMain::OverDriveEnd()
{
	m_bOverDriveStageFlag = false;
}

void sceneMain::Reset()
{
	m_bOverDriveStageFlag = false;
	PlayerMgr->Reset();
	CameraMgr->SetPlayersPos();
	m_pShotMgr->Reset();
	ParticleManager::Reset();
	GameUIMgr->TimerReset();
}