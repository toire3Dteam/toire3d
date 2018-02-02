#include "TDNLIB.h"
#include "system/FrameworkEx.h"
#include "sceneLoading.h"
#include "LoadSceneThread.h"

//=============================================================================================
//		初	期	化	と	開	放
sceneLoading::sceneLoading(BaseScene *newScene) :BaseScene(),m_pNewScene(newScene),

// スレッド関連初期化ゾーン
//m_thread(&sceneLoading::ThreadFunction, this, newScene),	// コンストラクタでスレッドを生成 ※メンバ関数を指定する場合は、第二引数にthisを入れること。
//m_bThread(true),											// スレッドフラグ

// 0初期化ゾーン
m_pImage{},
m_pGauge{},
m_AnimeCount{},
m_AnimeFrame{},
m_fLoadGaugePercent{},
m_fLoadAngle{}
{

}

bool sceneLoading::Initialize()
{
	m_pImage = new tdn2DObj("DATA/UI/Loading/loadPic.png");
	m_pGauge = new tdn2DObj("DATA/UI/Loading/loading_gauge.png");
	m_AnimeCount = m_AnimeFrame = 0;
	m_fLoadGaugePercent = 0;
	m_fLoadAngle = 0.0f;

	// ★ローディング時にFPSを落とす
	MainFrameEX->SetFPSMode(FPS_MODE::FPS_45);

	// ★スレッド生成
	LoadSceneThreadMgr->Initialize(m_pNewScene);

	//bThread = true;
	//_beginthread(ThreadFunction, 0, (void*)m_newScene);

	return true;
}

sceneLoading::~sceneLoading()
{
	// ★スレッド解放
	LoadSceneThreadMgr->Release();

	MainFrameEX->SetFPSMode(FPS_MODE::NORMAL);

	SAFE_DELETE(m_pImage);
	SAFE_DELETE(m_pGauge);
}
//=============================================================================================

//=============================================================================================
//		更			新
void sceneLoading::Update()
{
	// 画像アニメーション処理
	static const int AnimeSpeed = 10;
	if (++m_AnimeFrame > AnimeSpeed)
	{
		m_AnimeFrame = 0;
		if (++m_AnimeCount > 5) m_AnimeFrame = 0;
	}

	m_fLoadAngle -= 0.1f;
	m_pImage->SetAngle(m_fLoadAngle);

	// ロード割合補間処理
	m_fLoadGaugePercent = m_fLoadGaugePercent*.5f + m_pNewScene->m_fLoadPercentage*.5f;

	// ロードのコメント
	strcpy_s(m_LoadComment, 256, m_pNewScene->m_LoadComment);

	// ★ロードが終わったら、シーンをチェンジ
	if (LoadSceneThreadMgr->isInitializeEnd())
	{
		MainFrameEX->ChangeScene(m_pNewScene);
	}
}
//
//=============================================================================================


//=============================================================================================
//		描			画
void sceneLoading::Render()
{
	tdnView::Clear();

	//tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom , RS::COPY, 0xff000000);
	//m_pImage->Render(720, 540, 512, 64, 0, m_AnimeCount * 64, 512, 64);
	
	m_pImage->Render(1280 - 256 + 32, 720 - 256 + 32);

	// ゲージ中身
	//m_pGauge->Render(720, 620, (int)(m_fLoadGaugePercent * 512), 64, 0, 64, (int)(m_fLoadGaugePercent * 512), 64);
	// ゲージ枠
	//m_pGauge->Render(720, 620, 512, 64, 0, 0, 512, 64);

	// ロードのコメント
	tdnText::Draw(320, 620, 0xffffffff, "%s", m_LoadComment);
}
//
//=============================================================================================