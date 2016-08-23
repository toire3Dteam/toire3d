#pragma once
#include "../AI\AI.h"
class AI;

class Airou : public BasePlayer
{
public:
	Airou(int deviceID,TEAM team, bool bAI);

	/****************************/
	//	キャラクター固有スキル
	/****************************/
	void SkillUpdate();//

	/****************************/
	//	AI
	/****************************/
	// それぞれのキャラクター毎のAIによる技のボタンを設定！
	void AIHighAtackButton()
	{
		//m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		//return PLAYER_INPUT::D;
	};
	// 攻撃系スキル
	void AIAtackSkillButton()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
	};

	// AIの攻撃範囲
	float AIAtackRange() { return 20; }
	float AIRunAtackRange() { return 35; }

private:
	void InitActionDatas();	// 純粋仮想関数オーバーライド



};

