#pragma once

// 前方宣言
class AttackData;
class BasePlayer;
class BaseUVEffect;
class AramitamaShotEffect;
class AramitamaRippleEffect;

namespace Shot
{
	//------------------------------------------------------
	//	滞在時間なし(時間による消滅がない)
	//------------------------------------------------------
	static const int c_NO_SOJOURN_TIME = -1;

	enum class TYPE
	{
		MAYA  = 100,
		MUSHI1,
		MUSHI2,
		MUSHI3
	};

	//------------------------------------------------------
	//	ショットの情報を持つ構造体
	//------------------------------------------------------
	struct ParamDesc
	{
		//------------------------------------------------------
		//	位置・向き・移動量など
		//------------------------------------------------------
		Vector3 vPos;			// 位置
		Vector3 vVec;			// 単位化された向きベクトル
		Vector3 vVelocity;		// 移動量
		Vector3 vAccel;			// 加速度(Moveに足していく)
		int		iSojournTime;	// 飛び道具の滞在時間

		//------------------------------------------------------
		//	フラグ
		//------------------------------------------------------
		bool bCollisionOK;	// これがtrueなら判定が有効(貫通弾とか作ったときに多段ヒットしないようにする)
		bool bPenetration;	// 貫通するかどうか(trueなら壁にぶつかるまで存在する)
		bool bErase;		// 消去フラグ

		ParamDesc() :vPos(0,0,0), vVec(0,1,0), vVelocity(0,0,0), vAccel(0,0,0), iSojournTime(c_NO_SOJOURN_TIME), bCollisionOK(true), bErase(false), bPenetration(false){}
		//ParamDesc(const Vector3 &vPos, const Vector3 &vVec, const Vector3 &vVelocity, const Vector3 &vAccel = Vector3(0, 0, 0), int iSojournTime = c_NO_SOJOURN_TIME, bool bPenetration = false, bool bCollisionOK = true) :vPos(vPos), vVec(vVec), vVelocity(vVelocity), vAccel(vAccel), iSojournTime(iSojournTime), bCollisionOK(bCollisionOK), bPenetration(bPenetration), bErase(false){}
	};

	//------------------------------------------------------
	//	飛び道具基底クラス
	//------------------------------------------------------
	class Base
	{
	public:

		//------------------------------------------------------
		//	コンストラクタ・初期化(ショットの情報を引数に、コンストラクタだけにすると、ショットの情報を設定できない)
		//------------------------------------------------------
		Base(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj, bool bAttitude);
		Base(BasePlayer *pPlayer, AttackData *pAttackData, BaseUVEffect *pObj);
		void Initialize(const ParamDesc &tagParamDesc){ memcpy_s(&m_tagParamDesc, sizeof(ParamDesc), &tagParamDesc, sizeof(ParamDesc)); }

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
		Vector3 &GetPos(){ return m_tagParamDesc.vPos; }
		Vector3 &GetVec(){ return m_tagParamDesc.vVec; }
		Vector3 &GetMove(){ return m_tagParamDesc.vVelocity; }
		BasePlayer *GetPlayer(){ return m_pPlayer; }
		BaseUVEffect *GetUVEffect() { return m_pUVEffect; }
		iex3DObj *Get3DObj(){ return m_p3DObj; }
		AttackData *GetAttackData(){ return m_ptagAttackData; }
		bool EraseOK(){ return m_tagParamDesc.bErase; }
		bool isPenetration(){ return m_tagParamDesc.bPenetration; }
		bool isCollisionOK(){ return m_tagParamDesc.bCollisionOK; }
		virtual TYPE GetType() = 0;

		//------------------------------------------------------
		//	セッター
		//------------------------------------------------------
		void Erase(){ m_tagParamDesc.bErase = true; }
		void SetCollisionFlag(bool bCollisionOK){ m_tagParamDesc.bCollisionOK = bCollisionOK; }
		virtual void SetHitStopFrame(int iHitStopFrame) { m_iHitStopFrame = iHitStopFrame; }

	protected:

		//------------------------------------------------------
		//	メッシュ
		//------------------------------------------------------
		BaseUVEffect *m_pUVEffect;	// メッシュの実体(参照するだけ)
		iex3DObj *m_p3DObj;
		bool m_bAttitude;			// 姿勢制御フラグ

		//------------------------------------------------------
		//	プレイヤーの実体(あまり使うことはないが一応、)
		//------------------------------------------------------
		BasePlayer *m_pPlayer;

		//------------------------------------------------------
		//	位置・向き・移動量などの情報
		//------------------------------------------------------
		ParamDesc m_tagParamDesc;
		int m_iHitStopFrame;

		//------------------------------------------------------
		//	判定・攻撃力とかの情報
		//------------------------------------------------------
		AttackData *m_ptagAttackData;
	};


	class Maya :public Base
	{
	public:
		Maya(BasePlayer *pPlayer, AttackData *pAttackData,  BaseUVEffect *pObj, const Vector3 &vPos, const Vector3 &vVec);
		~Maya();
		void Update();
		void Render();

		TYPE GetType(){ return TYPE::MAYA; }
	};

	namespace AramitamaMushi
	{
		//------------------------------------------------------
		//	地上固有スキル: トスを上げる
		//------------------------------------------------------
		class Land :public Base
		{
		public:
			Land(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj, AramitamaRippleEffect* pEffect);
			~Land();
			void Update();
			void Render();

			TYPE GetType(){ return TYPE::MUSHI1; }
		private:
			//------------------------------------------------------
			//	滞在している時間・攻撃発生時間
			//------------------------------------------------------
			static const int c_SOJOURN_TIME;
			static const int c_ATTACK_FRAME;

			AramitamaRippleEffect* m_pRefShot;

			bool m_bHit;
		};

		//------------------------------------------------------
		//	しゃがみ固有スキル: 隕石のように突撃する
		//------------------------------------------------------
		class Squat :public Base
		{
		public:
			Squat(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj, AramitamaShotEffect *pEffect);
			~Squat();
			void Update();
			void Render();

			TYPE GetType(){ return TYPE::MUSHI2; }
		private:
			//------------------------------------------------------
			//	移動速度
			//------------------------------------------------------
			static const float c_SPEED;

			// エフェクト
			//AramitamaShotEffect* m_pShotEF;
			AramitamaShotEffect* m_pRefShot;

		};

		//------------------------------------------------------
		//	空中固有スキル: 回転しながら昇っていく
		//------------------------------------------------------
		class Aerial :public Base
		{
		public:
			Aerial(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj);
			~Aerial();
			void Update();
			void Render();

			TYPE GetType(){ return TYPE::MUSHI3; }
			void SetHitStopFrame(int iHitStopFrame) {}	// ヒットストップを無視する

			//------------------------------------------------------
			//	滞在時間・移動速度・射程範囲
			//------------------------------------------------------
			static const int c_SOJOURN_TIME;
			static const float c_SPEED;
			static const float c_RANGE;
		};
	};
}