#include "TDNLIB.h"
#include "system/FrameworkEx.h"
#include "sceneTitle.h"
#include "SceneMenu.h"

//=============================================================================================
//		初	期	化	と	開	放
bool sceneTitle::Initialize()
{
	// 動画関連初期化
	m_pMovie = new tdnMovie("DATA/Title/op.wmv", true);
	m_pMovie->Play();

	// 画像初期化
	m_pImages[IMAGE::BACK] = new tdn2DObj(m_pMovie->GetTexture());
	m_fLoadPercentage = .5f;	// ロード割合

	m_fLoadPercentage = 1.0f;	// ロード割合

	return true;
}

sceneTitle::~sceneTitle()
{
	FOR(IMAGE::MAX) delete m_pImages[i];
	delete m_pMovie;
}
//=============================================================================================


//=============================================================================================
//		更			新
void sceneTitle::Update()
{
	// テアリング防止の苦肉の策
	m_pMovie->Update();

	// エンターキーでスタート
	FOR(4)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, 1) == 3 || KEY(KEY_C, 1) == 3 || KEY(KEY_D, 1) == 3)
		{
			MainFrameEx->ChangeScene(new sceneMenu);
			break;
		}
	}
}
//
//=============================================================================================


//=============================================================================================
//		描			画
void sceneTitle::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// 動画画像
	m_pImages[IMAGE::BACK]->Render(0,0);

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