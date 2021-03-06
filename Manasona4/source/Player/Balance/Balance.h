#pragma once
#include "../AI\AI.h"
#include "../BasePlayer.h"

//+-------------------------
//	バランス型
//+-------------------------

class Balance : public BasePlayer
{
public:
	Balance(SIDE side, const SideData &data);
	~Balance();
	void Update(PLAYER_UPDATE flag)override;
	void Render()override;
	void RenderDrive()override;

	/****************************/
	//	キャラクター固有の様々なポジション
	/****************************/
	Vector3 GetCenterPos() { return m_vPos + Vector3(0, 5, 0); };	 // プレイヤーの真ん中の場所
	Vector3 GetFlontPos() { return m_vPos + Vector3((m_dir == DIR::RIGHT) ? 6.0f : -6.0f, 5.0f, 0); };   // つかみの相手の場所

	/****************************/
	// ラウンド跨ぎの初期化
	/****************************/
	void Reset();

	/****************************/
	// 掛け合いのキャラごとの更新
	/****************************/
	void KakeaiInit();
	void KakeaiExit();
	void KakeaiUpdate();

	/****************************/
	//	通常三段目
	/****************************/
	void ThirdRushInit();		// スキル発動時に呼び出す
	void ThirdRushExit();
	void ThirdRushUpdate();//

	/****************************/
	//	カウンター成功
	/****************************/
	void OnInvincibleCounterSuccess();

	/****************************/
	//	キャラクター固有スキル
	/****************************/
	void SkillInit();
	void SkillExit();
	bool SkillUpdate();//

	/****************************/
	//	キャラクター固有無敵
	/****************************/
	void InvincibleAttackInit() override;
	void InvincibleAttackExit() override;
	bool InvincibleAttackUpdate() override;//

	class SkillAction
	{
	public:
		class Base
		{
		public:
			Base(Balance *pBalance, BASE_ACTION_STATE state);
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Balance *m_pBalance;
			int m_OrgDamage;
			float m_fOrgComboRate;
			CollisionShape::Square m_OrgHitSquare;
			Vector2 m_OrgFlyVector[2];

			void SetOrgInfo(BASE_ACTION_STATE state);
		};

		class Land :public Base
		{
		public:
			Land(Balance *pBalance);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitCount;
		};

		class Squat :public Base
		{
		public:
			Squat(Balance *pBalance);
			void Enter();
			bool Execute();
			void Exit();
		private:
		};

		//// 突進攻撃
		//class Land2 :public Base
		//{
		//public:
		//	Land2(Balance *pBalance);
		//	void Enter();
		//	bool Execute();
		//	void Exit();
		//};

		class Aerial :public Base
		{
		public:
			Aerial(Balance *pBalance);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_ActiveFrame;	// 初段、二段と攻撃を分けるので、時間の計測で
		};

		//// 着地にしたろ
		//class AerialDrop :public Base
		//{
		//public:
		//	AerialDrop(Balance *pBalance);
		//	void Enter();
		//	bool Execute();
		//	void Exit();
		//};
	};

	SkillAction::Base *m_pSkillActions[(int)SKILL_ACTION_TYPE::MAX];

	/****************************/
	//	ヒーホードライブ
	/****************************/
	void HeavehoDriveInit();
	void HeavehoDriveExit();
	bool HeavehoDriveUpdate();
	void HeavehoDriveHitEvent();

	/****************************/
	//	ヒーホードライブ_オーバーフロー
	/****************************/
	void HeavehoDriveOverFlowInit();
	void HeavehoDriveOverFlowExit();
	void HeavehoDriveOverFlowUpdate();//
	void HeavehoDriveOverFlowSuccessInit();
	void HeavehoDriveOverFlowSuccessUpdate();

	/****************************/
	//	AI
	/****************************/
	// それぞれのキャラクター毎のAIによる技のボタンを設定！
	void AIHighAttackButton()
	{
		//m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		//return PLAYER_INPUT::D;
	};
	// 攻撃系スキル
	void AIAttackSkillButton()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
	};

	// 近距離スキル
	bool AINearSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// 中距離スキル
	bool AIMiddleSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// 遠距離スキル
	bool AIFarSkill()
	{
		return false;	// このキャラクターに遠距離技はない
	};

	// 空中近距離スキル
	bool AIAerialNearSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// AIの攻撃範囲
	float AIAttackRange() { return 15; }
	float AIRunAttackRange() { return 27; }
	float AIThrowRange() { return 6; }

	/****************************/
	//	 カラー
	/****************************/
	void ChangeColor(COLOR_TYPE eColType)override;

private:
	// 純粋仮想関数オーバーライド
	void InitActionDatas();
	void InitMotionDatas();

	// 波動用
	AttackData *m_pShotData;
	BaseUVEffect *m_pBullet;	// 飛び道具のメッシュの実体(玉に参照させる)
	BaseUVEffect *m_pUSABullet;	// 飛び道具のメッシュの実体(玉に参照させる)

	// Balance用のエフェクト
	BaseUVEffect* m_pCanonEffect;
	BaseUVEffect *m_pNozzleFlashEffect;	// 必殺の風
	BaseUVEffect* m_pBlueImpact;
	int m_iBlueImpactActFrame;
	//StepEffect* m_pStepEffect;

	BasePanelEffect* m_pHeartEffect;

	// 人参
	iex3DObj* m_pNinjin;

};

