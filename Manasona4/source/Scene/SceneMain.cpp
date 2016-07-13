#include "TDNLIB.h"
#include "../system/Framework.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../particle_2d/particle_2d.h"
#include "../Effect/EffectManager.h"
#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/Player.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

//******************************************************************
//		初期化・解放
//******************************************************************

bool sceneMain::Initialize()
{
	// ステージ初期化
	Stage::Base::CreateStage(&m_pStage, STAGE_ID::SENJO);	// 関数の中で作ってもらう

	// プレイヤー初期化
	m_pPlayerMgr = new PlayerManager(4);

	// カメラ初期化
	m_pCamera = new Camera(m_pStage, m_pPlayerMgr);

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
	delete m_pPlayerMgr;
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
	m_pPlayerMgr->Update(m_pStage);

	// エンターでエフェクトカメラ発動してみる
	if (KeyBoardTRG(KB_ENTER))
	{
		EFFECT_CAMERA_INFO eci;
		eci.scriptID = 3;
		MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
	}

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
	m_pPlayerMgr->Render();

	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_pCamera->m_pos.x, m_pCamera->m_pos.y, m_pCamera->m_pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_pCamera->m_target.x, m_pCamera->m_target.y, m_pCamera->m_target.z);
}