#include	"iextreme.h"
#include	"system/Framework.h"

#include	"OKB.h"

#include	"TitleScene.h"
#include	"ModeSelectScene.h"

// デバッグ用
//#define DEBUG_TITLE

extern Framework* MainFrame;


bool TitleScene::Initialize()
{
	//	環境設定
	iexLight::SetFog(800, 1000, 0);

	//	平行光設定							
	Vector3	 dir(1.0f, -1.0f, 1.0f);
	iexLight::DirLight(0, &dir, 1.0f, 1.0f, 1.0f);
	//	環境光設定							
	iexLight::SetAmbient(0x202020);

	// カメラ
	view = new iexView;

	// 2D画像
	image[IMG_BACK] = new iex2DObj("DATA/BG/image/BG.png");

	// テキスト
	text = new Text(50, "ＭＳ ゴシック");

	fade = new Fade;
	fade->Set(fade->FADE_IN, 5, 0, 255, 0x00000000);

	return true;
}

TitleScene::~TitleScene()
{
	// ビュー
	SAFE_DELETE(view);

	// 2D画像
	for (int i = 0; i < IMG_MAX; i++)
		SAFE_DELETE(image[i]);

	// テキスト
	SAFE_DELETE(text);

	// フェード
	SAFE_DELETE(fade);
}



void TitleScene::Update()
{
	fade->Update();

	for (int i = 0; i < 4; i++)
	{
		if (KEY(KEY_ENTER, i)){
			MainFrame->ChangeScene(new ModeSelectScene());
			break;
		}
	}
}

void TitleScene::Render()
{
#ifndef DEBUG_TITLE

	// 2D画像調整用デバッグ変数
	static int x = 0;
	static int y = 0;
	const int move = 10;

	if (KeyBoardTRG(KB_A))
		x -= move;

	if (KeyBoardTRG(KB_D))
		x += move;

	if (KeyBoardTRG(KB_W))
		y -= move;

	if (KeyBoardTRG(KB_S))
		y += move;

#endif


	//画面クリア
	view->Activate();
	view->Clear(0xff0080ff);


	// タイトル画像
	image[IMG_BACK]->Render(0, 0, 1280, 720, 0, 0, 2024, 1028, RS_COPY, 0xffffffff, 100);

	// テキスト
	text->Draw(iexSystem::ScreenWidth / 2, iexSystem::ScreenHeight / 2, 0xff000000, "タイトル");

	// フェード
	fade->Render();
}