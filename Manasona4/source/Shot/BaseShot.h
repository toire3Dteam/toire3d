#pragma once

// 前方宣言
class AttackData;
class BasePlayer;
class BaseUVEffect;

namespace Shot
{

	//------------------------------------------------------
	//	飛び道具基底クラス
	//------------------------------------------------------
	class Base
	{
	public:

		//------------------------------------------------------
		//	コンストラクタ
		//------------------------------------------------------
		Base(BasePlayer *pPlayer, AttackData *pAttackData, BaseUVEffect *pObj, const Vector3 &pos, const Vector3 &vVec, const Vector3 &vVelocity, const Vector3 &vAccel = Vector3(0, 0, 0));

		//------------------------------------------------------
		//	デストラクタ
		//------------------------------------------------------
		virtual ~Base(){}

		//------------------------------------------------------
		//	更新
		//------------------------------------------------------
		virtual void Update();

		//------------------------------------------------------
		//	描画
		//------------------------------------------------------
		virtual void Render();
		//virtual void Render(tdnShader *shader, char *name);

		//------------------------------------------------------
		//	ゲッター
		//------------------------------------------------------
		Vector3 &GetPos(){ return m_vPos; }
		Vector3 &GetVec(){ return m_vVec; }
		Vector3 &GetMove(){ return m_vVelocity; }
		BasePlayer *GetPlayer(){ return m_pPlayer; }
		BaseUVEffect *GetObj() { return m_pObj; }
		AttackData *GetAttackData(){ return m_ptagAttackData; }
		bool EraseOK(){ return m_bErase; }

		//------------------------------------------------------
		//	セッター
		//------------------------------------------------------
		void Erase(){ m_bErase = true; }

	protected:

		//------------------------------------------------------
		//	メッシュ
		//------------------------------------------------------
		BaseUVEffect *m_pObj;	// メッシュの実体(参照するだけ)


		//------------------------------------------------------
		//	プレイヤーの実体(あまり使うことはないが一応、)
		//------------------------------------------------------
		BasePlayer *m_pPlayer;

		//------------------------------------------------------
		//	位置・向き・移動量などの情報
		//------------------------------------------------------
		Vector3 m_vPos;		// 位置
		Vector3 m_vVec;		// 単位化された向きベクトル
		Vector3 m_vVelocity;// 移動量
		Vector3 m_vAccel;	// 加速度(Moveに足していく)

		//------------------------------------------------------
		//	判定・攻撃力とかの情報
		//------------------------------------------------------
		AttackData *m_ptagAttackData;

		//------------------------------------------------------
		//	フラグ
		//------------------------------------------------------
		bool m_bErase;		// 消去フラグ
	};


	class Maya :public Base
	{
	public:
		Maya(BasePlayer *pPlayer, AttackData *pAttackData,  BaseUVEffect *pObj, const Vector3 &vPos, const Vector3 &vVec);
		~Maya();
		void Update();
		void Render();
	};

}