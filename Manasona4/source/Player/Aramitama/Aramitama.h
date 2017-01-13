#pragma once
#include "../AI\AI.h"
#include "../BasePlayer.h"

class Aramitama : public BasePlayer
{
public:
	Aramitama(SIDE side, const SideData &data);
	~Aramitama();
	void Update(PLAYER_UPDATE flag)override;
	void Render()override;
	void RenderDrive()override;

	/****************************/
	//	キャラクター固有の様々なポジション
	/****************************/
	Vector3 GetCenterPos(){ return m_vPos + Vector3(0, 5, 0); };	 // プレイヤーの真ん中の場所
	Vector3 GetFlontPos() { return m_vPos + Vector3((m_dir == DIR::RIGHT) ? 5.0f : -5.0f, 5.0f, 0); };   // つかみの相手の場所

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
			Base(Aramitama *pAramitama, BASE_ACTION_STATE state);
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Aramitama *m_pAramitama;
			int m_iOrgHitStopFrame[2];
			int m_iOrgHitRecoveryFrame[2];
			int m_OrgDamage;
			float m_fOrgComboRate;
			CollisionShape::Square m_OrgHitSquare;
			Vector2 m_OrgFlyVector[2];

			void SetOrgInfo(BASE_ACTION_STATE state);
		};

		class Land:public Base
		{
		public:
			Land(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitCount;
		};

		class Squat :public Base
		{
		public:
			Squat(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
		};

		class Aerial :public Base
		{
		public:
			Aerial(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
			bool m_bMoved;
		};

		class AerialDrop :public Base
		{
		public:
			AerialDrop(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
			bool m_bMoved;
		};
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
		m_pAI->PushInputList(PLAYER_INPUT::L2);
		return true;
	};

	// 中距離スキル
	bool AIMiddleSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::DOWN);
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
	float AIAttackRange() { return 14; }
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

	// エフェクト
	AramitamaChargeWaveEffect*	m_pAChargeWave;		// チャージのウェーブ
	AramitamaChargeAuraEffect*	m_pAChargeAura;		// チャージのオーラ
	AramitamaCircleEffect*		m_pACircle;			// サークル
	AramitamaCanonEffect*		m_pACanon;			// 必殺キャノン
	AramitamaNozzleFlashEffect* m_pANozzleFlash;	// 必殺マズルフラッシュ

	// 烙印
	bool m_bWassyoi;
	int m_iWassyoiGauge;

	enum class MUSHI_TYPE
	{
		LAND, SQUAT, AERIAL, MAX
	};
	class MushiData
	{
	public:
		AttackData *tagpAttackData;

		MushiData(LPSTR filepath, int iCoolTime) :iCoolTime(0), c_COOL_TIME(iCoolTime), pBullet(nullptr), tagpAttackData(nullptr)
		{
			pBullet = new iex3DObj(filepath);
			tagpAttackData = new AttackData;
		}
		~MushiData()
		{
			SAFE_DELETE(pBullet);
			SAFE_DELETE(tagpAttackData);
		}
		void Update()
		{
			// クールタイム更新
			if (iCoolTime > 0) iCoolTime--;
		}

		void Action(Aramitama *pAramitama, MUSHI_TYPE type);
	private:
		int iCoolTime;
		const int c_COOL_TIME;
		iex3DObj *pBullet;
	};
	MushiData *m_pMushi[(int)MUSHI_TYPE::MAX];

	void InitMushiDatas();
};

