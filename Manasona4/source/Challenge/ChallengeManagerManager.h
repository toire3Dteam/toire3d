#pragma once
#include "BaseChallengeManager.h"

//+----------------------------------------------------------
//	各キャラクターのチャレンジマネージャーを管理するクラス
//+----------------------------------------------------------

// スーパサイヤ人ゴットスーパーサイヤ人かよ

class ChallengeManagerManager
{
private:
	// シングルトンの作法
	ChallengeManagerManager();
	~ChallengeManagerManager();
	ChallengeManagerManager(const ChallengeManagerManager&);
	ChallengeManagerManager &operator=(const ChallengeManagerManager&) {}

	// 1つしかない実体
	static ChallengeManagerManager *m_pInstance;
public:
	// 実体取得
	static ChallengeManagerManager *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new ChallengeManagerManager;
		return m_pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(m_pInstance);
	}

	// ★メインに使う
	BaseChallengeManager* GetSelectMgr() { return m_pChallengeManager[(int)m_eSelectCharaType]; };

	// 選択している変数の操作
	CHARACTER GetSelectCharaType() { return m_eSelectCharaType; }
	void SetSelectCharaType(CHARACTER eType) { m_eSelectCharaType = eType; }

private:
	// それぞれのチャレンジ
	BaseChallengeManager* m_pChallengeManager[(int)CHARACTER::END];

	// 現在選択してるチャレンジデータ
	CHARACTER m_eSelectCharaType;

};

// 実体化
#define ChallengeMgrMgr (ChallengeManagerManager::GetInstance())
