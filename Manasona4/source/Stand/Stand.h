#pragma once

#include "../Player/BasePlayer.h"

namespace Stand
{
	enum class ACTION_TYPE
	{
		LAND,
		SQUAT,
		AERIAL,
		AERIALDROP,
		MAX
	};
	class Base
	{
	public:
		Base(BasePlayer *pPlayer);
		virtual ~Base();
		virtual void Update();
		virtual void Render();

		// プレイヤーがペルソナボタン押したら呼び出す
		virtual void Action(ACTION_TYPE type) = 0;

		// ゲッター
		bool isAttackFrame()
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

	protected:
		BasePlayer *m_pPlayer;	// 自分の実体を持っているプレイヤーへのポインタ
		iex3DObj *m_pObj;		// 3D実体
		Vector3 m_pos;			// 座標
		Vector3 m_move;			// 移動値
		DIR m_dir;				// 向き
		bool m_bActive;			// 出てるかどうか

		FRAME_STATE m_ActionFrameList[(int)ACTION_TYPE::MAX][FRAME_MAX];
		ACTION_TYPE m_ActionType;	// アクションの種類(↑の左の添え字の中身)
		int m_CurrentActionFrame;	// アクションフレームリストの中を再生しているフレーム(↑↑の右の添え字の中身)

		void LoadActionFrameList(char *filename);

		AttackData *m_pAttackData[(int)ACTION_TYPE::MAX];	// 攻撃データ
	};

	class Mokoi : public Base
	{
	public:
		Mokoi(BasePlayer *pPlayer);
		//~Mokoi();

		// 純粋仮想オーバーライド
		void Action(ACTION_TYPE type);
	};
}