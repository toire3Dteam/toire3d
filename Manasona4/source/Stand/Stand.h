#pragma once

#include "../Player/BasePlayer.h"


namespace Stand
{
	class Base
	{
	public:
		Base(BasePlayer *pPlayer);
		virtual ~Base();
		virtual void Update(bool bControl);
		virtual void Render(tdnShader* shader,char* name);

		// プレイヤーがペルソナボタン押したら呼び出す
		virtual void Action(SKILL_ACTION_TYPE type);

		// ペルソナブレイク
		void Break();

		// ★★★今攻撃しているかどうかと、ガードしないようにしたいスタンドへのオーバーライドも兼ねている
		virtual bool isAttackFrame()
		{
			// 攻撃判定出てる
			return (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
		}
		//virtual bool isActiveBeforeFrom()
		//{
		//	FRAME_STATE eState = m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame];
		//	return (eState == FRAME_STATE::ACTIVE_BEFORE || eState == FRAME_STATE::ACTIVE);
		//}

		// ★アッタカイだけガードしないようにするために、スタンドごとにガードするスタンドか
		virtual bool isCanGuardStand() = 0;

		bool isActive(){ return m_bActive; }
		bool isHit()
		{
			// アタックデータがそもそもない
			if (!m_pAttackData[(int)m_ActionType]) return false;
			return m_pAttackData[(int)m_ActionType]->bHit;
		}
		AttackData *GetAttackData(){ return m_pAttackData[(int)m_ActionType]; }
		DIR GetDir(){ return m_dir; }
		Vector3 &GetPos(){ return m_pos; }
		int GetDeviceID(){ return m_pPlayer->GetDeviceID(); }
		BasePlayer *GetPlayer(){ return m_pPlayer; }

		int GetStandGageMAX(){ return m_standGageMAX; };
		int GetStandGage(){ return m_standGage; };
		void SetStandGage(int gage){  m_standGage = gage; }
		int GetStandStockMAX(){ return m_standStockMAX; };
		int GetStandStock(){ return m_standStock; };
		void SetStandStock(int stock){ m_standStock= stock; }

		// アイコンのアクセサ
		tdn2DAnim* GetIcon() { return m_pIcon; }
		tdn2DAnim* GetIconRip() { return m_pIconRip; }

		//パートナーの種類
		PARTNER GetPartnerType() { return m_ePartnerType; }

		// 中心の座標
		virtual Vector3 GetCenterPos() { return m_pos + Vector3(0, 7.5f, 0); };



	protected:
		BasePlayer *m_pPlayer;	// 自分の実体を持っているプレイヤーへのポインタ
		iex3DObj *m_pObj;		// 3D実体
		Vector3 m_pos;			// 座標
		Vector3 m_move;			// 移動値
		DIR m_dir;				// 向き
		bool m_bActive;			// 出てるかどうか
		float m_fScale;			// 拡大縮小

		// エフェクト
		SummonEffect* m_pSummonEffect;
		SummonWaveEffect* m_pSummonWaveEffect;
		ExitEffect*	  m_pExitEffect;

		// アイコン
		tdn2DAnim* m_pIcon;
		tdn2DAnim* m_pIconRip;

		// 出せるペルソナ
		int m_standStockMAX;	// スタンドストック最大数
		int m_standStock;		// スタンドストック数
		int m_standGageMAX;		// スタンドゲージ最大値
		int m_standGage;		// スタンドゲージ


		FRAME_STATE m_ActionFrameList[(int)SKILL_ACTION_TYPE::MAX][FRAME_MAX * 2];// ★スタンドだけフレーム長く
		SKILL_ACTION_TYPE m_ActionType;	// アクションの種類(↑の左の添え字の中身)
		int m_CurrentActionFrame;	// アクションフレームリストの中を再生しているフレーム(↑↑の右の添え字の中身)

		void LoadActionFrameList(char *filename);

		AttackData *m_pAttackData[(int)SKILL_ACTION_TYPE::MAX];	// 攻撃データ

		PARTNER m_ePartnerType;

	private:
		void StandGageUpdate();


	};

	/**************************/
	//		モコイ
	class Mokoi : public Base
	{
	public:
		Mokoi(BasePlayer *pPlayer);
		//~Mokoi();
		void Update(bool bControl);
		// 純粋仮想オーバーライド
		void Action(SKILL_ACTION_TYPE type);
		bool isCanGuardStand() { return true; }	// ガードできるスタンド
	};

	/**************************/
	//		マーヤ
	class Maya : public Base
	{
	public:
		Maya(BasePlayer *pPlayer);
		~Maya();
		void Update(bool bControl);
		// 純粋仮想オーバーライド
		void Action(SKILL_ACTION_TYPE type);
		bool isAttackFrame(){ return false; }	// マーヤは判定なし
		bool isCanGuardStand() { return false; }	// ガードできないスタンド
	private:
		BaseUVEffect *m_pBullet;	// 飛び道具のメッシュの実体(玉に参照させる)
		BaseUVEffect *m_pBullet2;	// 飛び道具のメッシュの実体(玉に参照させる)

	};

	/**************************/
	//		ヘテ
	class Hete : public Base
	{
	public:
		Hete(BasePlayer *pPlayer);
		//~Mokoi();
		void Update(bool bControl);

		// 純粋仮想オーバーライド
		void Action(SKILL_ACTION_TYPE type);
		bool isCanGuardStand() { return true; }	// ガードできるスタンド
	};


	/**************************/
	//		アッタカイ
	class Attakai : public Base
	{
	public:
		Attakai(BasePlayer *pPlayer);
		~Attakai();
		void Update(bool bControl);
		void Render(tdnShader* shader, char* name);

		// 純粋仮想オーバーライド
		void Action(SKILL_ACTION_TYPE type);
		bool isCanGuardStand() { return false; }	// ガードできないスタンド
	private:
		BasePanelEffect* m_pBomEf;
		BasePanelEffect* m_pBomAddEf;
		float m_fDangerRate;
		int m_iActiveStartFrame;
		int m_iSoundFrame;						// チカチカSE用のフレーム
		static const int c_SE_INTERVAL = 90;	// チカチカSEの間隔

	};
}