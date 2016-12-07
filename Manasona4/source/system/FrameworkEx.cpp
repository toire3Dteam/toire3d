#include "FrameworkEx.h"
#include "../Scene/sceneLoading.h"
#include	"../Sound/SoundManager.h"

// 実体化
FrameworkEx* FrameworkEx::m_pInstance = nullptr;

// ミリ秒でのFPS
#define FPS_1 16.66667
#define FPS_2 33.33333
#define FPS_60 1000

/************************/
//	初期化・解放
/************************/
FrameworkEx::FrameworkEx()
{
	// マイクロ秒まで調べるのに必要な構造体
	LARGE_INTEGER count;

	// 1秒あたりのクロック数
	QueryPerformanceFrequency(&count);  // 1秒当たりクロック数を取得
	m_dFreq= (double)count.QuadPart;	// 1秒当たりクロックカウント数（周波数
  
	// ミリ秒で今のフレームを詰め込む
	QueryPerformanceCounter(&count);		 // クロックカウント数	
	m_dFlameTime = (double)count.QuadPart * 1000 / m_dFreq;// 差分クロック数を倍精度浮動小数点に変換
	
	// 表記用
	m_iFPS = 0;
	m_iRenderFPS = 0;

	m_iWorkFPS = 0;
	m_iWorkRenderFPS = 0;

	// 描画・更新フラグ
	m_bRenderFlag = true;
	m_bUpdateFlag = true;
	m_bSlowFlag = false;

	// ゲームのシーン
	m_scene = NULL;

	// ムービーフラグ
	m_bMoviePlay = false;
}

FrameworkEx::~FrameworkEx()
{
	SAFE_DELETE(m_scene);
}

FrameworkEx * FrameworkEx::GetInstance()
{
	if (FrameworkEx::m_pInstance == nullptr)
	{
		m_pInstance = new FrameworkEx();
	}

	return m_pInstance;
}

void FrameworkEx::Release()
{
	SAFE_DELETE(m_pInstance);
}

/************************/
//	初期化・解放
/************************/
bool FrameworkEx::Update()
{
	// マイクロ秒まで調べるのに必要な構造体
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);	// クロックカウント数取得	

	// 一秒毎の変数
	static 	double	oneSecond =  (double)count.QuadPart * 1000 / m_dFreq;//　最初の時間に合わせる 

	// 毎回現在の時間を更新		 	
	m_dCurrentTime = (double)count.QuadPart * 1000 / m_dFreq;
	
	// ★1fpsに満たない更新させない
	if (m_dCurrentTime < m_dFlameTime+FPS_1) return false;
	
	//	経過時間
	double	intervalTime = m_dCurrentTime - m_dFlameTime;

	// 2秒以上離れていたらすごい速度で更新し続けるので時間を合わす
	if (intervalTime > (FPS_60 * 2))
	{
		m_dFlameTime = m_dCurrentTime;	// ゲーム自体が止まるようにしているならあまり関係ないかと
	}

	// 一回の更新に2FPS以上かかってしまった場合 描画を止めて更新だけする
	if (intervalTime > FPS_1 * 2) m_bRenderFlag = false;
	else m_bRenderFlag = true;
	m_bRenderFlag = true;

	// 新たに1FPSを加算
	// この数値を触ることにより1フレームの長さを変えれる
	m_dFlameTime += FPS_1;


	// 一秒間にどれだけ更新されているか記入
	if (oneSecond < m_dCurrentTime)
	{
		m_iFPS	=	m_iWorkFPS;				// 1秒間に更新した回数
		m_iRenderFPS = m_iWorkRenderFPS;	// 1秒間に描画した回数
		
		m_iWorkFPS = m_iWorkRenderFPS = 0;	// 初期化

		oneSecond += FPS_60;				// 1秒毎入る用に1秒加算
	}

	// FPSカウント加算
	m_iWorkFPS++;

	// システム更新
	tdnInput::Update();
	SoundManager::Update();


	if (FPSCtrl() == true)return true;// FPSの操作

	// シーンの更新
	if (m_scene != NULL)
	{
		// シーンの更新
		m_scene->Update();	
		
		// 動画が再生していたら描画させない
		return true;
	}

	return true;
}

void FrameworkEx::Render()
{
	// 描画フラグが立っていたら描画
	if (m_bRenderFlag == false)return;

	//【描画開始】
	tdnSystem::BeginScene();



	//	シーン描画
	if (m_scene != NULL) m_scene->Render();
	


#ifdef _DEBUG



	//	フレーム表示
	tdnText::Draw(tdnSystem::GetScreenSize().right - 128, tdnSystem::GetScreenSize().bottom-36, 0xff00ffff, "FPS %d/%d", m_iFPS, m_iRenderFPS);

#endif
	// ストップウォッチの描画
	tdnStopWatch::DrawResult(300, 0);
	//【描画終了】
	tdnSystem::EndScene();

	// RenderFPSカウント加算
	m_iWorkRenderFPS++;
}


// FPSの操作
bool FrameworkEx::FPSCtrl()
{
	if (KeyBoardTRG(KB_1))
	{
		if (m_bUpdateFlag == false)m_bUpdateFlag = true;
		else m_bUpdateFlag = false;
	}

	// 更新フラグが立っていたら更新
	if (m_bUpdateFlag == false)
	{
		if (KeyBoardTRG(KB_2)) /*	更新が通る	*/;
		else return true;
	}

	if (KeyBoardTRG(KB_3))
	{
		if (m_bSlowFlag == false)
		{
			m_bSlowFlag = true;
		} else
		{
			m_bSlowFlag = false;
		}
	}
	if (m_bSlowFlag == true)
	{
		m_dFlameTime += FPS_1 + FPS_1 + FPS_1;
	}

	return false;
}

//*****************************************************************************************************************************
//		シーンの切り替え
//*****************************************************************************************************************************
void FrameworkEx::ChangeScene(BaseScene* newScene, bool bLoadingScene)
{
	//	現在のシーン解放
	if (m_scene != nullptr) delete m_scene;
	//	シーンの切り替え＆初期化

	// ローディング画面あり
	if (bLoadingScene)
	{
		m_scene = new sceneLoading(newScene);
		m_scene->Initialize();	// ここで、遷移先のInitializeを呼んでいる。
	}

	// ローディング画面なし
	else
	{
		m_scene = newScene;

		// Loadingでの2重Initialize阻止
		if (m_scene->m_bLoad) return;

		m_scene->Initialize();
	}
}