#pragma once
#include "../AI\AI.h"
class AI;

class Airou : public BasePlayer
{
public:
	Airou(int deviceID,TEAM team, bool bAI);

	/****************************/
	//	キャラクター固有の様々なポジション
	/****************************/
	virtual Vector3 GetCenterPos(){ return m_pos + Vector3(0, 5, 0); };	 // プレイヤーの真ん中の場所
	virtual Vector3 GetHoldPos() { return m_pos + Vector3((m_dir == DIR::RIGHT) ? 5.0f : -5.0f, 5.0f, 0); };   // つかみの相手の場所


	/****************************/
	//	キャラクター固有スキル
	/****************************/
	void SkillInit();
	void SkillExit();
	void SkillUpdate();//

	/****************************/
	//	ヒーホードライブ
	/****************************/
	void HeavehoDriveInit();
	void HeavehoDriveExit();
	void HeavehoDriveUpdate();

	/****************************/
	//	ヒーホードライブ_オーバーフロー
	/****************************/
	void HeavehoDriveOverFlowInit();
	void HeavehoDriveOverFlowExit();
	void HeavehoDriveOverFlowUpdate();//
	void HeavehoDriveOverFlowSuccess();

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
	// 純粋仮想関数オーバーライド
	void InitActionDatas();
	void InitMotionDatas();

	// アイルードリル用の変数
	Vector2 m_OrgFlyVector[2];
	int m_DrillFrame;
	int m_DrillHitCount;	// 多段ヒットのカウント
};

