#include "TDNLIB.h"
#include "../system/FrameworkEx.h"
#include "SceneResult.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../Effect/Particle.h"
#include "../Camera/camera.h"
#include "../Stage/Stage.h"
#include "../Camera/EffectCamera.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../BaseEntity/Message/Message.h"
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"
#include "../Data/SelectData.h"
#include "../DeferredEx/DeferredEx.h"

#include "../PointLight/PointLight.h"

#include "../ResultPerformance/BaseResultPerformance.h"
#include "SceneMenu.h"
#include "../SceneSwitch/SceneSwitch.h"

#define USE_EFFECT_CAMERA

//tdn2DAnim* anim;

//******************************************************************
//		初期化・解放
//******************************************************************
sceneResult::sceneResult(SIDE WinnerSide) :m_WinnerSide(WinnerSide){}

bool sceneResult::Initialize()
{
	m_grand = new iexMesh("Data/Result/persona5_grand.IMO");
	m_grand->SetScale(Vector3(2, 1, 0.25f));
	m_grand->Update();
	//anim = new tdn2DAnim("Data/Result/persona5Alpha.png");
	//anim->OrderShrink(32, 1, 3);
	//anim->OrderMoveAppeared(12, 1080, 500);

	//	ビュー設定
	tdnView::Init();

#ifdef USE_EFFECT_CAMERA
	//CameraMgr->InitializeResult();
	m_pEffectCamera=new EffectCamera(&m_ViewData,"DATA/Camera/result.txt");
	m_pEffectCamera->Start(0);
#else
	tdnView::Set(Vector3(-10, 100, -30), Vector3(-10, 5, 0));
#endif

	m_dirLight = Vector3(1, -1, 1);

	m_fLoadPercentage = .25f;	// ロード割合
	m_fLoadPercentage = .5f;	// ロード割合
	m_fLoadPercentage = 1.0f;	// ロード割合

	// パーティクル初期化
	//ParticleManager::Initialize("DATA/Effect/particle.png", 256);



	m_bShaderFlag = true;

	//m_moveMgr = new tdnMovie("DATA/Result/Aramitama/backmovie.wmv");
	//m_move = new tdn2DObj(m_moveMgr->GetTexture());
	
	// 3つのレンダーターゲット
	m_BackScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);
	m_CameraScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);
	m_FrontScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);
	m_AllScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);

	// 勝者
	switch ((SelectDataMgr->Get()->tagSideDatas[(int)m_WinnerSide].CharacterType))
	{
	case CHARACTER::AIROU:
		m_WinPlayer = new iex3DObj("DATA/CHR/Airou/result.IEM");
		m_ResultPerformance = new AirouResultPerformance();
		break;

	//case CHARACTER::ARAMIATAMA:
	//	m_WinPlayer = new iex3DObj("DATA/CHR/Aramitama/aramitama_test.IEM");
	//	m_ResultPerformance = new AramitamaResultPerformance();
	//	break;

	case CHARACTER::END:
		MyAssert(0, "そんなキャラは存在しない");
		break;
	}
	m_WinPlayer->SetScale(1);
	m_WinPlayer->SetMotion(0);
	m_WinPlayer->SetFrame(0);
	m_WinPlayer->SetAngle(PI * 1.5f);
	m_winPos = Vector3(0, 0, 0);

	// 敗者
	const SIDE LoserSide(m_WinnerSide == SIDE::LEFT ? SIDE::RIGHT : SIDE::LEFT);
	switch ((SelectDataMgr->Get()->tagSideDatas[(int)LoserSide].CharacterType))
	{
	case CHARACTER::AIROU:
		m_LosePlayer = new iex3DObj("DATA/CHR/Airou/airou.IEM");
		m_iLoseMotion = 36;
		break;

	case CHARACTER::END:
		MyAssert(0, "そんなキャラは存在しない");
		break;
	}
	m_LosePlayer->SetAngle(PI * 1.5f);
	m_LosePlayer->SetScale(1.5f);		// ★★★★★　今だけスケール大きくしてる！！
	m_losePos=Vector3(-50, 0, 50);
	m_LosePlayer->SetPos(m_losePos);
	m_LosePlayer->Update();

	////　★勝ったキャラクターの勝利演出へ
	//WINNER_PLAYER winPlayer;
	//winPlayer = WINNER_PLAYER::AIROU;
	//switch (winPlayer)
	//{
	//case sceneResult::WINNER_PLAYER::ARAMITAMA:
	//	m_ResultPerformance = new AramitamaResultPerformance();

	//	break;
	//case sceneResult::WINNER_PLAYER::AIROU:
	//	m_ResultPerformance = new AirouResultPerformance();
	//	break;
	//default:
	//	assert(0);
	//	break;
	//}

	m_iPerformanceActionTimer = 0;
	m_iPerformanceTimer = 0;
	m_bPerformanceFlag = false;
	
	m_bEndFlag = false;

	shader2D->SetValue("chroma", 1.0f);
	shader2D->SetValue("ScreenColor", Vector3(1.0f, 1.0f, 1.0f));

	// 最初の演出
	m_sceneSwitch = new SceneSwitch();
	m_sceneSwitch->Action();
	
	//
	m_winnerPic = new tdn2DAnim("Data/Result/winner1.png");
	m_winnerPic->OrderMoveAppeared(12, -600, 600);
	m_iWinnerPicX = 0.0f;


	m_UVEffectMgr = new UVEffectManager();
	
	m_wavePic = new tdn2DObj("Data/Result/waveScreen.png");
	m_waveScreen = new tdn2DObj(1280, 720, TDN2D::RENDERTARGET);
	m_waveTU = 0.0f;

	m_bFirstUpdate = true;

	return true;
}


sceneResult::~sceneResult()
{
	m_pStream->Stop();
	//ParticleManager::Release();
	delete m_pEffectCamera;
	SAFE_DELETE(m_grand);

	SAFE_DELETE(m_WinPlayer);
	SAFE_DELETE(m_LosePlayer);

	SAFE_DELETE(m_BackScreen);
	SAFE_DELETE(m_CameraScreen);
	SAFE_DELETE(m_FrontScreen);
	SAFE_DELETE(m_AllScreen);

	SAFE_DELETE(m_sceneSwitch);

	SAFE_DELETE(m_ResultPerformance);

	SAFE_DELETE(m_winnerPic);

	SAFE_DELETE(m_UVEffectMgr);

	SAFE_DELETE(m_wavePic);

	SAFE_DELETE(m_waveScreen);
}

//******************************************************************
//		処理
//******************************************************************

void sceneResult::Update()
{
	// ★★★シームレスに読み込むので、Initialize関数で初期化すると都合が悪い(例えばストリーミングBGMとか)やつをUpdateで1回だけ呼び出したいときに使う
	if (m_bFirstUpdate)
	{
		// ▼▲チキン
		se->Play("リザルト1");

		// ストリーミング初期化
		m_pStream = bgm->PlayStream("DATA/Sound/BGM/Result/Period.ogg");
		m_pStream->SetVolume(0.4f);

		m_bFirstUpdate = false;
	}

	static float angle = 0;
	angle += 0.01f;

#ifdef USE_EFFECT_CAMERA
	if (m_bEndFlag == false)
	{
		//CameraMgr->Update();	// 灰色になったら止める
		m_pEffectCamera->Update();
		tdnView::Set(m_ViewData);
	}
#endif

	// パーティクル更新
	ParticleManager::Update();

	static float lightAngle = 2.14f;
	// アングル
	if (KeyBoard(KB_R)) { lightAngle -= 0.05f; }
	if (KeyBoard(KB_T)) { lightAngle += 0.05f; }
	m_dirLight.x = sinf(lightAngle);
	m_dirLight.z = cosf(lightAngle);
	m_dirLight.y = -0.8f;

	// アクションフラグ
	if (m_bPerformanceFlag == false)
	{
		m_iPerformanceActionTimer++;
		if (m_iPerformanceActionTimer >= PERFORMANCE_ACTION_TIME_MAX)
		{
			m_ResultPerformance->Action();
			
			// ▼ズブシャアア
			se->Play("リザルト3");

			m_winnerPic->Action();// 勝利した方の文字もここでアクション
			m_iWinnerPicX = 0;
			
			m_UVEffectMgr->AddEffectRoop(m_losePos+Vector3(0,5,+2), UV_EFFECT_TYPE::SPLASH); // しぶきエフェクト
			
			// ★やられモーション
			m_LosePlayer->SetMotion(m_iLoseMotion);

			m_iPerformanceActionTimer = 0;
			m_bPerformanceFlag = true;
		}

		// ふわー
		if (m_iPerformanceActionTimer == 60)
		{
			se->Play("リザルト2");
		}
	}



	// アクション中
	if (m_bPerformanceFlag == true && m_bEndFlag == false)
	{
		ParticleManager::EffectResultSplash(m_losePos + Vector3(2, 5, 0));

		int startFrame = (PERFORMANCE_TIME_MAX - 30);
		int maxrate = PERFORMANCE_TIME_MAX - startFrame;
		int myFlame = m_iPerformanceTimer - startFrame;
		myFlame = max(0, myFlame);

		float rate = (float)myFlame / (float)maxrate;
		rate = 1.0f - rate;

		shader2D->SetValue("chroma", rate);
		float colrate= Math::Blend(rate, 0.5f, 1.0f);
		shader2D->SetValue("ScreenColor", Vector3(colrate, colrate, colrate));


		if (++m_iPerformanceTimer >= PERFORMANCE_TIME_MAX)
		{
			m_iPerformanceTimer = 0;
			m_bEndFlag = true;
			shader2D->SetValue("chroma", 0.0f);
			shader2D->SetValue("ScreenColor", Vector3(0.5f, 0.5f, 0.5f));
		}


	}
	
	if (KeyBoardTRG(KB_Z)) 
	{
		m_ResultPerformance->Action();
	}


	m_ResultPerformance->Update();

	m_LosePlayer->Animation();
	m_LosePlayer->Update();

	m_WinPlayer->Animation();
	m_WinPlayer->Update();

	m_sceneSwitch->Update();

	m_winnerPic->Update();
	m_iWinnerPicX += 0.2f;

	m_UVEffectMgr->Update();

	if (m_bEndFlag)
	{
		FOR(4) if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			MainFrameEx->ChangeScene(new sceneMenu());
			return;
		}
	}
}

//******************************************************************
//		描画
//******************************************************************

void sceneResult::Render()
{

	// BackBufferの保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);


#ifdef USE_EFFECT_CAMERA
	//CameraMgr->Activate();
	tdnView::Activate();
	tdnView::Clear(0xff0800);
#else
	tdnView::Activate();
	tdnView::Clear();
#endif


	WaveRender();

	if (m_bEndFlag == false)
	{
		CameraRender();
		BackRender();
		FrontRender();

		AllRender();
	}


	if (m_bEndFlag == false)m_AllScreen->Render(0,0,shader2D,"Mastering");// 最終結果
	else m_AllScreen->Render(0, 0, shader2D, "Mastering_Wave");// 波うち描画

	m_sceneSwitch->Render();

	//m_waveScreen->Render(0, 0, 1280/4, 720/4, 0, 0, 1280, 720);

	//m_move->Render(0, 0);
}

// 後ろの２D
void sceneResult::BackRender()
{
	// BackBufferの保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);

	m_BackScreen->RenderTarget();
	CameraMgr->Activate();

	// 仮 後ろの赤背景	
	if (m_bPerformanceFlag == false)
	{
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xffdd2222, 1);
	}

	m_ResultPerformance->BackRender();

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);

}

// 3Dゲーム画面
void sceneResult::CameraRender()
{
	// BackBufferの保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);

	m_CameraScreen->RenderTarget();
	CameraMgr->Activate();

	// 負けたプレイヤー描画
	m_LosePlayer->Render(shaderM,"black");

	// 勝ったプレイヤー描画
	if (m_bPerformanceFlag == false)
	{
		m_WinPlayer->Render();
	}

	m_grand->Render(/*shaderM,"black"*/);

	m_UVEffectMgr->Render();

	ParticleManager::Render();

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);

}


// ★ここは２D専用の場所にしよう　ALPHAの問題で
void sceneResult::FrontRender()
{
	// BackBufferの保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);

	m_FrontScreen->RenderTarget();
	
	// ★★★一時停止するとカメラがこちらのものに切り替わるのでコメントアウト　なんで？
	//tdnView::Set(Vector3(0, 0, -100), Vector3(0, 0, 0), .0f);	// 勝ったキャラ用のカメラを設定
	tdnView::Activate();
	tdnView::Clear(0x00ff80);

	// リザルトの手前の2D
	m_ResultPerformance->FrontRender();

	// 勝者の
	m_winnerPic->RenderSpecial((int)m_iWinnerPicX, 600);

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);

}

void sceneResult::AllRender()
{
	/****************************/
	// ここからゲーム画面
	m_AllScreen->RenderTarget(); //全スクリーンを囲む

	m_BackScreen->Render(0, 0);
	if (m_iPerformanceActionTimer == PERFORMANCE_ACTION_TIME_MAX-4)		m_CameraScreen->SetARGB(0xffbbbbbb);
	else if (m_iPerformanceActionTimer == PERFORMANCE_ACTION_TIME_MAX - 3)	m_CameraScreen->SetARGB(0xff888888);
	else if (m_iPerformanceActionTimer == PERFORMANCE_ACTION_TIME_MAX - 2 || 
		m_iPerformanceActionTimer == PERFORMANCE_ACTION_TIME_MAX - 1 ||
		m_iPerformanceActionTimer == PERFORMANCE_ACTION_TIME_MAX )
		m_CameraScreen->SetARGB(0xff333333);
	else m_CameraScreen->SetARGB(0xffffffff);

	m_CameraScreen->Render(0, 0);

	m_FrontScreen->Render(0, 0);

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);

}


void sceneResult::WaveRender()
{
	m_waveScreen->RenderTarget();

	m_waveTU -= 6.0f;

	m_wavePic->Render(0, 0, 1280, 720, (int)m_waveTU, 0, 1280 * 4, 720 * 4);

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);

	shader2D->SetValue("WaveTexture", m_waveScreen);

}