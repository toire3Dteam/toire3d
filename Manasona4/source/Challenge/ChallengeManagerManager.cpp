#include "ChallengeManagerManager.h"

//+----------------------------------------------------------
//	各キャラクターのチャレンジマネージャーを管理するクラス
//+----------------------------------------------------------

// 実体の宣言
ChallengeManagerManager* ChallengeManagerManager::m_pInstance = nullptr;

ChallengeManagerManager::ChallengeManagerManager()
{
	// 初期化
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_pChallengeManager[i] = new AirouChallengeManager();
			break;
		case CHARACTER::TEKI:
			m_pChallengeManager[i] = new TekiChallengeManager();
			break;
		case CHARACTER::NAZENARA:
			m_pChallengeManager[i] = new NazenarabaChallengeManager();
			break;
		case CHARACTER::ARAMITAMA:
			m_pChallengeManager[i] = new AramitamaChallengeManager();
			break;
		case CHARACTER::ANIKI:
			m_pChallengeManager[i] = new AramitamaChallengeManager();
			break;
		case CHARACTER::BALANCE:
			m_pChallengeManager[i] = new AirouChallengeManager();
			break;
		default:
			MyAssert(0, "そのタイプは存在しない");
			break;
		}
	}

	// 初期で選択してるキャラ
	m_eSelectCharaType = CHARACTER::AIROU;

}

ChallengeManagerManager::~ChallengeManagerManager()
{
	// 解放
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		SAFE_DELETE(m_pChallengeManager[i]);
	}
}
