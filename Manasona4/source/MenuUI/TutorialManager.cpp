#include "TutorialManager.h"

//+----------------------------
//	チュートリアルマネージャー
//+----------------------------

// 実体の宣言
TutorialManager* TutorialManager::m_pInstance = nullptr;

TutorialManager::TutorialManager()
{
	// 全てのチュートリアルを初期化
	for (int i = 0; i < TUTORIAL_TYPE::ARRAY_END; i++)
	{
		switch ((TUTORIAL_TYPE)i)
		{
		case WALK:
			m_pTutorial[i] = new WalkTutorial();
			break;
		case ARRAY_END:
			assert(0);
			break;
		default:			
			assert(0);
			break;
		}

	}

	// 初期設定
	m_eType = TUTORIAL_TYPE::WALK;
}

TutorialManager::~TutorialManager()
{
	// 全てのチュートリアルを解放
	for (int i = 0; i < TUTORIAL_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTutorial[i]);
	}

}

// 初期化
void TutorialManager::Init(TUTORIAL_TYPE type)
{
	m_eType = type;// チュートリアルのタイプ変更

	m_pTutorial[m_eType]->Init();

}

// 更新
void TutorialManager::Update()
{
	// チュートリアル更新
	m_pTutorial[m_eType]->Update();

}

// タスク用更新
void TutorialManager::TaskUpdate(BasePlayer * pPerson)
{
	// タスク用更新
	m_pTutorial[m_eType]->TaskUpdate(pPerson);

}

// 描画
void TutorialManager::Render()
{
	// チュートリアル描画
	m_pTutorial[m_eType]->Render();
}
