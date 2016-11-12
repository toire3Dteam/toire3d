#include "TDNLIB.h"
#include "../Sound/SoundManager.h"
#include "system/FrameworkEx.h"
#include "../Data/PlayerData.h"
#include "SceneMenu.h"
#include "SceneCollect.h"
#include "../Fade/Fade.h"


//=============================================================================================
//		初	期	化	と	開	放
bool sceneCollect::Initialize()
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	m_fLoadPercentage = .5f;	// ロード割合

	// 画像初期化
	m_pImages[IMAGE::BACK] = new tdn2DObj("DATA/UI/Collect/bg.png");
	m_pImages[IMAGE::SCROLL_BAR] = new tdn2DObj("DATA/UI/Collect/ScrollBar.png");
	m_pImages[IMAGE::SCROLL_ARROW] = new tdn2DObj("DATA/UI/Collect/ScrollArrow.png");
	m_fLoadPercentage = 1.0f;	// ロード割合

	m_pPlayerInfo = &PlayerDataMgr->m_PlayerInfo;

	bgm->PlayStreamIn("DATA/Sound/BGM/System/Collect.ogg");

	// スクロールバー
	m_iScrollHeight = 1440;
	m_iScrollScreenSize= 720;

	return true;
}

sceneCollect::~sceneCollect()
{
	bgm->StopStreamIn();
	FOR(IMAGE::MAX) delete m_pImages[i];
}
//=============================================================================================


//=============================================================================================
//		更			新
void sceneCollect::Update()
{
	static bool bEnd(false);

	// フェード
	Fade::Update();

	if (bEnd)
	{
		// フェード終わったら帰る
		if (Fade::isFadeStop())
		{
			bEnd = false;
			MainFrameEx->ChangeScene(new sceneMenu);
		}
	}

	// スペースキーで抜ける
	else if (KEY(KEYCODE::KEY_A, 0) == 3)
	{
		//se->Play("戻る");

		// フェードアウト設定
		Fade::Set(Fade::FLAG::FADE_OUT, 4);
		bEnd = true;
	}
}
//
//=============================================================================================


//=============================================================================================
//		描			画
void sceneCollect::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// 背景
	m_pImages[IMAGE::BACK]->Render(0, 0);

	tdnPolygon::Rect((int)(1280 * .25f) / 2, 96, (int)(1280 * .75f), (int)(720 * .75f), RS::COPY, 0xff808080);

	// プレイ回数
	tdnText::Draw(400, 400, 0xffffffff, "プレイ回数: %d", m_pPlayerInfo->PlayCount);

	// プレイ時間
	const int minutes(m_pPlayerInfo->PlayTime % 60), hour(m_pPlayerInfo->PlayTime / 60);
	tdnText::Draw(400, 460, 0xffffffff, "プレイ時間: %d時間 %d分", hour, minutes);

	// コイン
	tdnText::Draw(400, 520, 0xffffffff, "コイン: %d", m_pPlayerInfo->coin);

	// スクロールバー
	m_pImages[IMAGE::SCROLL_BAR]->Render(0, 0);
	float rate= (m_iScrollScreenSize / m_iScrollHeight);
	m_pImages[IMAGE::SCROLL_ARROW]->Render(0, 0,64, (int)(512* rate), 0, 0, 64, 512);

	// フェード
	Fade::Render();
}
//
//=============================================================================================