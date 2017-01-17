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
		void Break()
		{
			if (m_bActive)
			{
				// ここにブレイクの音とかエフェクトとか

				m_bActive = false;
			}
		}

		// ゲッター
		virtual bool isAttackFrame()
		{
			// 攻撃判定出てる
			return (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
		}
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

	protected:
		BasePlayer *m_pPlayer;	// 自分の実体を持っているプレイヤーへのポインタ
		iex3DObj *m_pObj;		// 3D実体
		Vector3 m_pos;			// 座標
		Vector3 m_move;			// 移動値
		DIR m_dir;				// 向き
		bool m_bActive;			// 出てるかどうか

		// アイコン
		tdn2DAnim* m_pIcon;
		tdn2DAnim* m_pIconRip;

		// 出せるペルソナ
		int m_standStockMAX;	// スタンドストック最大数
		int m_standStock;		// スタンドストック数
		int m_standGageMAX;		// スタンドゲージ最大値
		int m_standGage;		// スタンドゲージ


		FRAME_STATE m_ActionFrameList[(int)SKILL_ACTION_TYPE::MAX][FRAME_MAX];
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

		// 純粋仮想オーバーライド
		void Action(SKILL_ACTION_TYPE type);
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
	private:
		BaseUVEffect *m_pBullet;	// 飛び道具のメッシュの実体(玉に参照させる)

	};
}