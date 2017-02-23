#include "TDNLIB.h"
#include "system/FrameworkEx.h"
#include "sceneTitle.h"
#include "sceneTitleState.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"

//=============================================================================================
//		初	期	化	と	開	放
//=============================================================================================
sceneTitle::sceneTitle() :BaseGameEntity(ENTITY_ID::SCENE_TITLE) {}
bool sceneTitle::Initialize()
{
	// 動画関連初期化
	m_pMovie = new tdnMovie("DATA/Title/op.wmv", true, false);	

	m_pMovieBGLine = new tdnMovie("DATA/Title/titleBG2.wmv");
	m_pBGLine = new tdn2DObj(m_pMovieBGLine->GetTexture());

	// 画像初期化
	m_pImages[IMAGE::MOVIE] = new tdn2DObj(m_pMovie->GetTexture());
	m_pImages[IMAGE::BG] = new tdn2DObj("Data/Title/BG.png");
	m_pImages[IMAGE::TITLE] = new tdn2DObj("Data/Title/Title.png");
	m_pImages[IMAGE::CLOUD1] = new tdn2DObj("Data/Title/cloud1.png");
	m_pImages[IMAGE::CLOUD2] = new tdn2DObj("Data/Title/cloud2.png");

	m_pLogo = new tdn2DAnim("Data/Title/logo.png");
	m_pLogo->OrderMoveAppeared(90, 0, 720 / 4);
	
	m_pPreaseAnyButton = new tdn2DAnim("Data/Title/PreaseAnyButton.png");
	m_pPreaseAnyButton->OrderAlphaMove(150, 70, 80);

	m_fCloudU = 0.0f;

	m_fLoadPercentage = .5f;	// ロード割合
	
	m_fLoadPercentage = 1.0f;	// ロード割合

	m_iGameTimer = 0;

	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneTitle>(this);
	m_pStateMachine->SetCurrentState(SceneTitleState::Intro::GetInstance());	// ステートの設定


	return true;
}

sceneTitle::~sceneTitle()
{
	FOR(IMAGE::MAX) delete m_pImages[i];
	SAFE_DELETE(m_pMovie);
	SAFE_DELETE(m_pLogo);
	SAFE_DELETE(m_pPreaseAnyButton);

	SAFE_DELETE(m_pStateMachine);

	SAFE_DELETE(m_pMovieBGLine);
	SAFE_DELETE(m_pBGLine);

}
//=============================================================================================


//=============================================================================================
//		更			新
void sceneTitle::Update()
{
	// フェード更新
	Fade::Update();

	{
		// ★ステートマシン更新(何故ここに書くかというと、中でシーンチェンジの処理を行っているため)
		m_pStateMachine->Update();
		return;
	}



}
//
//=============================================================================================


//=============================================================================================
//		描			画
void sceneTitle::Render()
{
	tdnView::Clear();

	// ★ステートマシン描画
	m_pStateMachine->Render();

	// フェード
	Fade::Render();

	//int work;
	//m_pMovie->GetTextureRenderer()->get_AvgFrameRate(&work);
	//tdnText::Draw(0, 30, 0xffffffff, "平均フレームレート          :%d", work);
	//m_pMovie->GetTextureRenderer()->get_AvgSyncOffset(&work);
	//tdnText::Draw(0, 60, 0xffffffff, "平均予定時間差(ミリ秒)　    :%d", work);
	//m_pMovie->GetTextureRenderer()->get_DevSyncOffset(&work);
	//tdnText::Draw(0, 90, 0xffffffff, "時間の間の標準偏差(ミリ秒)  :%d", work);
	//m_pMovie->GetTextureRenderer()->get_FramesDrawn(&work);
	//tdnText::Draw(0, 120, 0xffffffff, "描画フレーム数             :%d", work);
	//m_pMovie->GetTextureRenderer()->get_FramesDroppedInRenderer(&work);
	//tdnText::Draw(0, 150, 0xffffffff, "ドロップフレーム数         :%d", work);
	//m_pMovie->GetTextureRenderer()->get_Jitter(&work);
	//tdnText::Draw(0, 180, 0xffffffff, "次のフレーム標準偏差       :%d", work);
}
//
//=============================================================================================