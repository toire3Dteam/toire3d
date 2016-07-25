#include "TDNLIB.h"
#include "../system/Framework.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../particle_2d/particle_2d.h"

#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"

//BaseEffect* g_eff;
//EffectManager;
PanelEffectManager* m_panel;
UVEffectManager* g_uvEffect;

//******************************************************************
//		初期化・解放
//******************************************************************

bool sceneMain::Initialize()
{

	//g_eff = new HitEffect();
	m_panel = new PanelEffectManager();
	g_uvEffect = new UVEffectManager();


	// カメラ初期化
	m_pCamera = new Camera();

	m_fLoadPercentage = .25f;	// ロード割合

	// ステージ初期化
	Stage::Base::CreateStage(&m_pStage, STAGE_ID::SYUTEN, m_pCamera);	// 関数の中で作ってもらう

	m_fLoadPercentage = .5f;	// ロード割合

	// プレイヤー初期化
	//m_pPlayerMgr = new PlayerManager(4, m_pStage);
	PlayerMgr->Initialize(4, m_pStage);

	// プレイヤーの座標のアドレスをカメラに渡してあげる(いじることは絶対に無く、ただ参照するだけ)
	m_pCamera->SetPlayersPos();

	m_fLoadPercentage = 1.0f;	// ロード割合

	// オレ曲初期化
	m_pMyMusicMgr = new MyMusicManager(MY_MUSIC_ID::TOILE);
	m_pMyMusicMgr->Play();

	return true;
}

sceneMain::~sceneMain()
{
	delete m_pMyMusicMgr;
	delete m_pStage;
	delete m_pCamera;
	PlayerMgr->Release();
	//EffectMgr.Release();
	SAFE_DELETE(m_panel);
	SAFE_DELETE(g_uvEffect);

}

//******************************************************************
//		処理
//******************************************************************

bool sceneMain::Update()
{
	// カメラ更新
	m_pCamera->Update();

	// ステージ更新
	m_pStage->Update();

	// プレイヤー更新
	PlayerMgr->Update();

	// エンターでエフェクトカメラ発動してみる
	if (KeyBoardTRG(KB_ENTER))
	{
		EFFECT_CAMERA_INFO eci;
		eci.scriptID = 3;
		MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
	}

	if (KeyBoardTRG(KB_L))
	{
		//g_eff->Action(0,0);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::BURN);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::NOTICE);

		m_panel->AddEffect(Vector3(0, 0, -5), PANEL_EFFECT_TYPE::DAMAGE);
		g_uvEffect->AddEffect(Vector3(0, 0, -5), UV_EFFECT_TYPE::WAVE);
	}

	//g_eff->Update();
	//EffectMgr.Update();
	m_panel->Update();
	g_uvEffect->Update();
	return true;
}

//******************************************************************
//		描画
//******************************************************************

void sceneMain::Render()
{
	// カメラ
	m_pCamera->Activate();

	// ステージ描画
	m_pStage->Render();

	// プレイヤー
	PlayerMgr->Render();

	//g_eff->Render3D();
	//EffectMgr.Render();
	m_panel->Render();

	//EffectMgr.Render3D();
	m_panel->Render3D();
	g_uvEffect->Render();


	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_pCamera->m_pos.x, m_pCamera->m_pos.y, m_pCamera->m_pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_pCamera->m_target.x, m_pCamera->m_target.y, m_pCamera->m_target.z);
}