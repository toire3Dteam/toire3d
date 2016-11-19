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
}
//
//=============================================================================================