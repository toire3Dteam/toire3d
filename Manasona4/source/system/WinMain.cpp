#include	"TDNLIB.h"
#include	"FrameworkEx.h"
#include	"../Scene/sceneMain.h"
#include	"../Scene/sceneRenderingTest.h"
#include	"../Scene/sceneResult.h"
#include	"../Scene/sceneTitle.h"
#include	"../Scene/SceneMenu.h"
#include	"../Scene/SceneCollect.h"
#include	"../Scene/SceneSelect.h"
#include	"System.h"
#include	"../Sound/SoundManager.h"
#include	"../Data/PlayerData.h"
#include	"../Effect/Particle.h"
#include    "../MenuUI/TutorialManager.h"

//*****************************/
//		WinMain
//*****************************/

//Framework*	MainFrame = NULL;			// フレームワーク

char*	AppTitle = "真夏(仮)";// ゲームのタイトル 
BOOL	bFullScreen = FALSE;			// フルスクリーンにしますか？
DWORD	ScreenMode = SCREEN1280x720;	// Windowの画面のサイズ

/***************************/
//	アプリケーション初期化
/***************************/
BOOL InitApp(HINSTANCE hInstance, int nCmdShow)									// ゲーム起動時の最初の初期化　必要な初期化を全てここに
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) bFullScreen = TRUE;// Ctrlでフルスクリーン

	if (!tdnSystem::InitWindow(hInstance, nCmdShow, AppTitle, ScreenMode, bFullScreen))
	{
		MessageBox(0, "ウィンドウ初期化失敗。", "System", MB_OK);
	}

	tdnRandom::Initialize();
	tdnRenderState::Initialize();
	tdnTexture::Initialize();
	tdnText::Init();
	tdnInput::Initialize();
	SoundManager::Initialize();
	tdnMovieManager::Initialize();
	ParticleManager::Initialize("DATA/Effect/particle.png", 2048);	// パーティクル初期化(★なぜここに書くかというと、シームレスシーンにて、初期化→初期化→解放になってしまうため)

	//IEXのシステム関数を一時的採用、話し合いでSystem.hを抹消するか決める
	SYSTEM_Initialize();

	//MainFrameEx = new Framework();			 // メインフレームワーク生成
	//MainFrameEx->ChangeScene(new sceneMain, true); // 初期シーン
	//MainFrameEx->ChangeScene(new sceneRenderingTest); // 描画テストシーン
	//MainFrameEx->ChangeScene(new sceneResult); // リザルトシーン
	//MainFrameEx->ChangeScene(new sceneEffectTool); // 
	//MainFrameEx->ChangeScene(new sceneTitle);
	MainFrameEx->ChangeScene(new sceneMenu);
	//MainFrameEx->ChangeScene(new sceneCollect);
	//MainFrameEx->ChangeScene(new sceneSelect);
	
	// データ
	PlayerDataMgr->Initialize();

	return TRUE;
}

/****************************/
//	アプリケーション解放
/****************************/
BOOL EndApp()																	// この中でシステムも含めた全ての後処理をするのでクラスの中には入れれない
{
	//delete MainFrame;			// [注意]SystemがReleaseされる前に消す事
	MainFrameEx->Release();
	ParticleManager::Release();
	SYSTEM_Release();
	TutorialMgr->Rerease();
	tdnFont::Release();
	tdnSystem::Release();
	tdnText::CleanUpModule();
	tdnInput::Release();
	SoundManager::Release();
	tdnMovieManager::Release();

	return TRUE;
}


//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	// メモリリーク検知器
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// メモリ確保した瞬間検知器
	//_CrtSetBreakAlloc(1601);

	// 最初の初期化
	InitApp(hInstance, nCmdShow);

	MSG		msg;	// ウィンドウメッセージを取得
	// メインループ
	for (;;)
	{
	//NOTACTIVE:
		// メッセージ処理
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	// メッセージがあればTRUE
		{
			if (!GetMessage(&msg, NULL, 0, 0)) break;	// WM_QUIT 以外のメッセージを取得した場合、0 以外の値が返ります。
			TranslateMessage(&msg);						// メッセージ入力をプログラムで処理できる形式に変換
			DispatchMessage(&msg);						// WindowProcを呼びだし、メッセージ処理を行う。

			// ウィンドウがアクティブ状態じゃなかったら止める
			if (tdnSystem::GetWindowActive() == false)
			{
				WaitMessage();
				//goto NOTACTIVE;
			}
		}
		else
		{
			//static int TDNtimer = 0;

			// ゲームの更新・描画
			if (MainFrameEx->Update())
			{
				MainFrameEx->Render();
				//TDNtimer++;
				//MyAssert((TDNtimer != 60 * 60), "遊びすぎいいいいいいいい！！！！！");
			}			

		}


	}

	//	全解放
	EndApp();

	return true;
}
