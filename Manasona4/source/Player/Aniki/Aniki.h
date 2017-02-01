#pragma once
#include "../AI\AI.h"
#include "../BasePlayer.h"

// 前方宣言
namespace Ketsudram{class Base;}
class Aniki : public BasePlayer
{
public:
	Aniki(SIDE side, const SideData &data);
	~Aniki();
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
	//	足払い
	/****************************/
	void DownAttackInit()
	{
		BasePlayer::DownAttackInit();

		// 移動
		SetMove(Vector3(GetDirVecX() * 1, 1, 0));
	}

	/****************************/
	//	キャラクター固有スキル
	/****************************/
	void SkillInit();
	void SkillExit();
	bool SkillUpdate();//

	class SkillAction
	{
	public:
		class Base
		{
		public:
			Base(Aniki *pAniki, BASE_ACTION_STATE state);
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Aniki *m_pAniki;
			int m_OrgDamage;
			float m_fOrgComboRate;
			CollisionShape::Square m_OrgHitSquare;
			Vector2 m_OrgFlyVector[2];
			LPCSTR m_lpcOrgHitSE;
			int m_iOrgRecoveryFrame;
		};

		class Land :public Base
		{
		public:
			Land(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitCount;
		};

		class Squat :public Base
		{
		public:
			Squat(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
		};

		// 地上掴んだ後
		class Land2 :public Base
		{
		public:
			Land2(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitFrame;
			bool m_bKetudoramed;
		};

		// 対空掴んだ後
		class Land3 :public Base
		{
		public:
			Land3(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitFrame;
			bool m_bKetudoramed;
		};

		class Aerial :public Base
		{
		public:
			Aerial(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_ActiveFrame;	// 初段、二段と攻撃を分けるので、時間の計測で
		};

		// 着地にしたろ
		class AerialDrop :public Base
		{
		public:
			AerialDrop(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		};
	};

	SkillAction::Base *m_pSkillActions[(int)SKILL_ACTION_TYPE::MAX];

	class HeaveHoAction
	{
	public:
		class Base
		{
		public:
			Base(Aniki *pAniki) :m_pAniki(pAniki){}
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Aniki *m_pAniki;
		};

		class Strike:public Base
		{
		public:
			Strike(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		};

		class Throw:public Base
		{
		public:
			Throw(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		};
	};

	HeaveHoAction::Base *m_pHeaveHoActions[2];

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
	void ChangeColor();

private:
	// 純粋仮想関数オーバーライド
	void InitActionDatas();
	void InitMotionDatas();

	// ケツドラム用
	Ketsudram::Base *m_pKetsudram;
	bool m_bHeavehoHit;	// 兄貴の必殺が当たったとき用

	void Ketsudram()
	{
		m_pDefaultObj->SetFrame(462);
	}
};

